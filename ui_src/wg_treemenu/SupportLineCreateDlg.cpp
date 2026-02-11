#include "stdafx.h"
#include "wg_treemenu.h"
#include "SupportLineCreateDlg.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\ServiceProvider.h"

#include "..\wg_gr\Gr_ServiceDef.h"

#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static const char* THIS_FILE = __FILE__;
#endif

#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_base\DlgUtil.h"
#include "..\wg_base\I_GENStateDefine.h"

CSupportLineCreateDlg::CSupportLineCreateDlg(CWnd* pParent)
    : CMenuBarChildDlg(CSupportLineCreateDlg::IDD, pParent)
{
    m_pDoc = CDBDoc::GetDocPoint();

    m_Data.Initialize();

    m_nDirection = 0;
}

CSupportLineCreateDlg::~CSupportLineCreateDlg()
{
    InitSelectionFilter();
}

void CSupportLineCreateDlg::DoDataExchange(CDataExchange* pDX)
{
    CMenuBarChildDlg::DoDataExchange(pDX);

    // Supporting Nodes
    DDX_Control(pDX, IDC_TM_SUPLINE_SUP_NODE_EDT, m_edtSupNode);

    // Guide Line 1 (Start)
    DDX_Control(pDX, IDC_TM_SUPLINE_GUIDE_LINE1_EDT, m_edtGuideLine1);
    DDX_Control(pDX, IDC_TM_SUPLINE_GUIDE_LINE1_ORTHO_CHK, m_chkGuideLine1Ortho);

    // Guide Line 2 (End)
    DDX_Control(pDX, IDC_TM_SUPLINE_GUIDE_LINE2_EDT, m_edtGuideLine2);
    DDX_Control(pDX, IDC_TM_SUPLINE_GUIDE_LINE2_ORTHO_CHK, m_chkGuideLine2Ortho);

	// Direction
	DDX_Radio(pDX, IDC_TM_SUPLINE_DIR_X_EDT, m_nDirection);
}

void CSupportLineCreateDlg::Dlg2Data()
{
    // Supporting Nodes
    CString strSupportingNodes;
    m_edtSupNode.GetWindowText(strSupportingNodes);
    m_Data.aNodeK.RemoveAll();
    CArray<int, int> aNodeKTemp;
    if (CStrParser::ParsingListByTo(strSupportingNodes, aNodeKTemp))
    {
        for (int i = 0; i < aNodeKTemp.GetSize(); i++)
        {
            m_Data.aNodeK.Add(aNodeKTemp[i]);
        }
    }

    T_NODE_K NodeK_Start = 0;
    T_NODE_K NodeK_End = 0;

    // Guide Line (Start)
    if (m_edtGuideLine1.GetWindowTextLength() > 0)
    {
        CString strGuideStart;
        m_edtGuideLine1.GetWindowText(strGuideStart);
        CArray<int, int> aGuide1Temp;
        if (CStrParser::ParsingListByTo(strGuideStart, aGuide1Temp))
        {
            if (aGuide1Temp.GetSize() != 2)
            {
                ASSERT(0); return;
            }
        }

        bool bOrthoStart = (m_chkGuideLine1Ortho.GetCheck() == BST_CHECKED);
        NodeK_Start = GetNodeByGuideLine(aGuide1Temp, bOrthoStart);
    }

    // Guide Line (End)
    if (m_edtGuideLine2.GetWindowTextLength() > 0)
    {
        CString strGuideEnd;
        m_edtGuideLine2.GetWindowText(strGuideEnd);
        CArray<int, int> aGuide2Temp;
        if (CStrParser::ParsingListByTo(strGuideEnd, aGuide2Temp))
        {
            if (aGuide2Temp.GetSize() != 2)
            {
                ASSERT(0); return;
            }
        }

        bool bOrthoEnd = (m_chkGuideLine2Ortho.GetCheck() == BST_CHECKED);
        NodeK_End = GetNodeByGuideLine(aGuide2Temp, bOrthoEnd);
    }

    // StartNodeK와 EndNodeK를 별도 필드에 저장
    m_Data.StartNodeK = NodeK_Start;
    m_Data.EndNodeK = NodeK_End;
    m_Data.nDirection = m_nDirection;

    return;
}

void CSupportLineCreateDlg::Execute()
{
    UpdateData(TRUE);

    Dlg2Data();

    if (!m_pDoc->m_pDataCtrl->AddSptl(m_Data))
    {
        ASSERT(0); return;
    }

    // Edit Control들 초기화
    m_edtSupNode.ClearNodeKeyList();
    m_edtSupNode.SetWindowText(_T(""));
    m_edtGuideLine1.ClearNodeKeyList();
    m_edtGuideLine1.SetWindowText(_T(""));
    m_edtGuideLine2.ClearNodeKeyList();
    m_edtGuideLine2.SetWindowText(_T(""));
    OnSetFocusSupNodeEdit();
}

void CSupportLineCreateDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    HWND hWnd = GetSafeHwnd();
    if (!hWnd || !IsWindow(hWnd)) return;

    switch (lHint)
    {
    case D_UPDATE_DBALL:
    case D_UPDATE_DEFAULT:
        UpdateBuffer();
        break;
    case D_UPDATE_BUFFER_BEFORE:
        break;
    case D_UPDATE_BUFFER_AFTER:
        UpdateBuffer();
        break;
    case D_UPDATE_UNIT:
        break;
    case D_UPDATE_SEL_ADD:
    case D_UPDATE_SEL_DEL:
        break;
    default: ASSERT(TRUE);
    }
}

void CSupportLineCreateDlg::UpdateBuffer()
{
    CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
    int nCount = pViewBuff->GetCount();
    if (nCount == 0) return;
    ASSERT(nCount == 1);

    T_UDRD_BUFFER buffer_ur;
    POSITION pos = pViewBuff->GetStartBuffer();

    BOOL bMFD = FALSE;
    while (pos != NULL)
    {
        buffer_ur = pViewBuff->GetNextBuffer(pos);

        int nCmd = buffer_ur.nCmd;

        switch (nCmd)
        {
        //case(UR_ELEM_ADD):
        //case(UR_ELEM_DEL):
        //case(UR_ELEM_MFD):
        //case(UR_ELEM_MFS):
            break;
        default:
            break;
        }
    }
}

BEGIN_MESSAGE_MAP(CSupportLineCreateDlg, CMenuBarChildDlg)
    // Supporting Nodes Edit
    ON_EN_SETFOCUS(IDC_TM_SUPLINE_SUP_NODE_EDT, OnSetFocusSupNodeEdit)
    ON_EN_KILLFOCUS(IDC_TM_SUPLINE_SUP_NODE_EDT, OnKillFocusSupNodeEdit)
    // Guide Line 1 Edit
    ON_EN_SETFOCUS(IDC_TM_SUPLINE_GUIDE_LINE1_EDT, OnSetFocusGuideLine1Edit)
    ON_EN_KILLFOCUS(IDC_TM_SUPLINE_GUIDE_LINE1_EDT, OnKillFocusGuideLine1Edit)
    // Guide Line 2 Edit
    ON_EN_SETFOCUS(IDC_TM_SUPLINE_GUIDE_LINE2_EDT, OnSetFocusGuideLine2Edit)
    ON_EN_KILLFOCUS(IDC_TM_SUPLINE_GUIDE_LINE2_EDT, OnKillFocusGuideLine2Edit)
    // CMouseEdit View 클릭 통지
    ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
    // CMouseEdit ESC 키 통지
    ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITESCAPE, OnMouseEditEscape)
    // CMouseEdit View에서 ESC 키 통지 (View가 포커스를 가진 상태에서)
    ON_REGISTERED_MESSAGE(CMouseEdit::WM_CANCEL_FROM_MVIEW, OnMViewCancel)
END_MESSAGE_MAP()

BOOL CSupportLineCreateDlg::OnInitDialog()
{
    CMenuBarChildDlg::OnInitDialog();

    // Supporting Nodes - 여러 노드 입력 가능
    m_edtSupNode.SetAttNodeList();
    m_edtSupNode.SetLButtonDownNotifyWindow(this);
    m_edtSupNode.SetEnterNotifyWindow(this);
    m_edtSupNode.SetEscNotifyWindow(this);
    m_edtSupNode.SetMViewCancelNotifyWindow(this);
    m_edtSupNode.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
    m_edtSupNode.SetMaxNodeKeyNum(100);

    // Guide Line 1 (Start) - 2개 노드
    m_edtGuideLine1.SetAttNodeList();
    m_edtGuideLine1.SetLButtonDownNotifyWindow(this);
    m_edtGuideLine1.SetEnterNotifyWindow(this);
    m_edtGuideLine1.SetEscNotifyWindow(this);
    m_edtGuideLine1.SetMViewCancelNotifyWindow(this);
    m_edtGuideLine1.SetModeToUse(MOUSEEDIT_USE_NOTHING);
    m_edtGuideLine1.SetMaxNodeKeyNum(2);

    // Guide Line 2 (End) - 2개 노드
    m_edtGuideLine2.SetAttNodeList();
    m_edtGuideLine2.SetLButtonDownNotifyWindow(this);
    m_edtGuideLine2.SetEnterNotifyWindow(this);
    m_edtGuideLine2.SetEscNotifyWindow(this);
    m_edtGuideLine2.SetMViewCancelNotifyWindow(this);
    m_edtGuideLine2.SetModeToUse(MOUSEEDIT_USE_NOTHING);
    m_edtGuideLine2.SetMaxNodeKeyNum(2);

    InitCtrl();

    GotoDlgCtrl(&m_edtSupNode);

    return FALSE;
}

void CSupportLineCreateDlg::OnSetFocusSupNodeEdit()
{
    m_edtGuideLine1.SetModeToUse(MOUSEEDIT_USE_NOTHING);
    m_edtGuideLine2.SetModeToUse(MOUSEEDIT_USE_NOTHING);
    m_edtSupNode.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);

    // 텍스트가 비어있으면 내부 리스트도 초기화
    if (m_edtSupNode.GetWindowTextLength() == 0)
    {
        m_edtSupNode.ClearNodeKeyList();
    }
}

void CSupportLineCreateDlg::OnKillFocusSupNodeEdit()
{
    // 모드를 유지한다. View 클릭 시 KillFocus가 먼저 발생하므로
    // 여기서 NOTHING으로 바꾸면 AddNodeKeyST()에서 매칭 실패한다.
    // 다른 Edit으로 포커스 이동 시 OnSetFocus에서 모드를 전환한다.
}

void CSupportLineCreateDlg::OnSetFocusGuideLine1Edit()
{
    m_edtSupNode.SetModeToUse(MOUSEEDIT_USE_NOTHING);
    m_edtGuideLine2.SetModeToUse(MOUSEEDIT_USE_NOTHING);
    m_edtGuideLine1.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);

    // 텍스트가 비어있으면 내부 리스트도 초기화
    if (m_edtGuideLine1.GetWindowTextLength() == 0)
    {
        m_edtGuideLine1.ClearNodeKeyList();
    }
}

void CSupportLineCreateDlg::OnKillFocusGuideLine1Edit()
{
}

void CSupportLineCreateDlg::OnSetFocusGuideLine2Edit()
{
    m_edtSupNode.SetModeToUse(MOUSEEDIT_USE_NOTHING);
    m_edtGuideLine1.SetModeToUse(MOUSEEDIT_USE_NOTHING);
    m_edtGuideLine2.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);

    // 텍스트가 비어있으면 내부 리스트도 초기화
    if (m_edtGuideLine2.GetWindowTextLength() == 0)
    {
        m_edtGuideLine2.ClearNodeKeyList();
    }
}

void CSupportLineCreateDlg::OnKillFocusGuideLine2Edit()
{
}

LRESULT CSupportLineCreateDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
    return 0L;
}

LRESULT CSupportLineCreateDlg::OnMouseEditEscape(WPARAM wParam, LPARAM lParam)
{
    HWND hWnd = (HWND)lParam;

    if (m_edtSupNode.GetSafeHwnd() == hWnd)
    {
        m_edtSupNode.ClearNodeKeyList();
    }
    else if (m_edtGuideLine1.GetSafeHwnd() == hWnd)
    {
        m_edtGuideLine1.ClearNodeKeyList();
    }
    else if (m_edtGuideLine2.GetSafeHwnd() == hWnd)
    {
        m_edtGuideLine2.ClearNodeKeyList();
    }

    return 0L;
}

void CSupportLineCreateDlg::InitSelectionFilter()
{
    I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
    pIGM->ChangeSelectionFilterCombo(EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM);
}

void CSupportLineCreateDlg::InitCtrl()
{
    // 초기 Selection Filter 설정
    I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
    pIGM->ChangeSelectionFilterCombo(EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM);
}

// 특정 좌표에서 기존 Node 찾기
UINT CSupportLineCreateDlg::FindNodeByCoord(double x, double y, double z, double tolerance)
{
    CArray<T_NODE_K, T_NODE_K> aAllNodeK;
    m_pDoc->m_pAttrCtrl->GetNodeKeyList(aAllNodeK);

    T_NODE_D nodeD;
    for (int i = 0; i < aAllNodeK.GetSize(); i++)
    {
        if (m_pDoc->m_pAttrCtrl->GetNode(aAllNodeK[i], nodeD))
        {
            double dx = nodeD.x - x;
            double dy = nodeD.y - y;
            double dz = nodeD.z - z;
            double dist = sqrt(dx * dx + dy * dy + dz * dz);

            if (dist < tolerance)
            {
                return aAllNodeK[i];
            }
        }
    }

    return 0; // Node를 찾지 못함
}

// 점과 선분 사이의 최단 거리 계산
double CSupportLineCreateDlg::DistancePointToSegment(const T_NODE_D& point, const T_NODE_D& seg1, const T_NODE_D& seg2)
{
    // 선분의 방향 벡터
    double vx = seg2.x - seg1.x;
    double vy = seg2.y - seg1.y;
    double vz = seg2.z - seg1.z;

    // 점에서 선분 시작점까지의 벡터
    double wx = point.x - seg1.x;
    double wy = point.y - seg1.y;
    double wz = point.z - seg1.z;

    // 내적 계산
    double c1 = vx * wx + vy * wy + vz * wz;
    if (c1 <= 0) // 시작점이 가장 가까움
    {
        return sqrt(wx * wx + wy * wy + wz * wz);
    }

    double c2 = vx * vx + vy * vy + vz * vz;
    if (c1 >= c2) // 끝점이 가장 가까움
    {
        double dx = point.x - seg2.x;
        double dy = point.y - seg2.y;
        double dz = point.z - seg2.z;
        return sqrt(dx * dx + dy * dy + dz * dz);
    }

    // 선분 위의 점이 가장 가까움
    double t = c1 / c2;
    double px = seg1.x + t * vx;
    double py = seg1.y + t * vy;
    double pz = seg1.z + t * vz;

    double dx = point.x - px;
    double dy = point.y - py;
    double dz = point.z - pz;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

// 점에서 직선으로의 수선의 발 계산
void CSupportLineCreateDlg::ProjectPointToLine(const T_NODE_D& point, const T_NODE_D& lineStart, const T_NODE_D& lineEnd,
                                                 T_NODE_D& projPoint, bool& isInsideSegment)
{
    // 선분의 방향 벡터
    double vx = lineEnd.x - lineStart.x;
    double vy = lineEnd.y - lineStart.y;
    double vz = lineEnd.z - lineStart.z;

    // 점에서 선분 시작점까지의 벡터
    double wx = point.x - lineStart.x;
    double wy = point.y - lineStart.y;
    double wz = point.z - lineStart.z;

    // 투영 비율 계산
    double c1 = vx * wx + vy * wy + vz * wz;
    double c2 = vx * vx + vy * vy + vz * vz;

    if (c2 < 1e-10) // 선분의 길이가 0에 가까움
    {
        projPoint = lineStart;
        isInsideSegment = true;
        return;
    }

    double t = c1 / c2;

    // 투영점 계산
    projPoint.x = lineStart.x + t * vx;
    projPoint.y = lineStart.y + t * vy;
    projPoint.z = lineStart.z + t * vz;

    // 선분 내부 여부 판단
    isInsideSegment = (t >= 0.0 && t <= 1.0);
}

// 두 직선의 교점 찾기 (같은 평면상에서만 교점 존재)
bool CSupportLineCreateDlg::FindLineIntersection(const T_NODE_D& line1Start, const T_NODE_D& line1End,
                                                   const T_NODE_D& line2Start, const T_NODE_D& line2End,
                                                   T_NODE_D& intersection)
{
    // Line 1의 방향 벡터
    double d1x = line1End.x - line1Start.x;
    double d1y = line1End.y - line1Start.y;
    double d1z = line1End.z - line1Start.z;

    // Line 2의 방향 벡터
    double d2x = line2End.x - line2Start.x;
    double d2y = line2End.y - line2Start.y;
    double d2z = line2End.z - line2Start.z;

    // 두 직선 시작점 간의 벡터
    double wx = line1Start.x - line2Start.x;
    double wy = line1Start.y - line2Start.y;
    double wz = line1Start.z - line2Start.z;

    // 내적 계산
    double a = d1x * d1x + d1y * d1y + d1z * d1z; // dot(d1, d1)
    double b = d1x * d2x + d1y * d2y + d1z * d2z; // dot(d1, d2)
    double c = d2x * d2x + d2y * d2y + d2z * d2z; // dot(d2, d2)
    double d = d1x * wx + d1y * wy + d1z * wz;    // dot(d1, w)
    double e = d2x * wx + d2y * wy + d2z * wz;    // dot(d2, w)

    double denom = a * c - b * b;

    if (fabs(denom) < 1e-10) // 평행한 경우
    {
        return false;
    }

    // Line 1에서의 파라미터 t1
    double t1 = (b * e - c * d) / denom;

    // Line 2에서의 파라미터 t2
    double t2 = (a * e - b * d) / denom;

    // Line 1 위의 점
    double p1x = line1Start.x + t1 * d1x;
    double p1y = line1Start.y + t1 * d1y;
    double p1z = line1Start.z + t1 * d1z;

    // Line 2 위의 점
    double p2x = line2Start.x + t2 * d2x;
    double p2y = line2Start.y + t2 * d2y;
    double p2z = line2Start.z + t2 * d2z;

    // 두 점이 같은지 확인 (tolerance 이내)
    double dist = sqrt(pow(p1x - p2x, 2) + pow(p1y - p2y, 2) + pow(p1z - p2z, 2));

    if (dist > 1e-6) // 꼬인 위치 (skew lines)
    {
        return false;
    }

    // 교점 반환
    intersection.x = p1x;
    intersection.y = p1y;
    intersection.z = p1z;

    return true;
}

// Guide Line으로부터 Node 추출/생성
T_NODE_K CSupportLineCreateDlg::GetNodeByGuideLine(CArray<int, int>& aGuideNodeK, bool bOrtho)
{
    // Guide Line이 2개의 Node로 구성되어 있는지 확인
    if (aGuideNodeK.GetSize() != 2)
    {
        AfxMessageBox(_T("Guide Line must have exactly 2 nodes."));
        return 0;
    }

    // Guide Line의 두 Node 정보 가져오기
    T_NODE_D guideNode1, guideNode2;
    if (!m_pDoc->m_pAttrCtrl->GetNode(aGuideNodeK[0], guideNode1) ||
        !m_pDoc->m_pAttrCtrl->GetNode(aGuideNodeK[1], guideNode2))
    {
        AfxMessageBox(_T("Failed to get guide line nodes."));
        return 0;
    }

    // m_Data.aNodeK에서 Guide Line과 가장 가까운 점들 찾기
    struct NodeDistance
    {
        UINT nodeK;
        double distance;
        T_NODE_D nodeD;
    };

    CArray<NodeDistance, NodeDistance&> aDistances;

    for (int i = 0; i < m_Data.aNodeK.GetSize(); i++)
    {
        T_NODE_D nodeD;
        if (m_pDoc->m_pAttrCtrl->GetNode(m_Data.aNodeK[i], nodeD))
        {
            double dist = DistancePointToSegment(nodeD, guideNode1, guideNode2);

            NodeDistance nd;
            nd.nodeK = m_Data.aNodeK[i];
            nd.distance = dist;
            nd.nodeD = nodeD;
            aDistances.Add(nd);
        }
    }

    if (aDistances.GetSize() < 1)
    {
        AfxMessageBox(_T("Not enough support nodes to process."));
        return 0;
    }

    // 거리순으로 정렬 (버블 정렬)
    for (int i = 0; i < aDistances.GetSize() - 1; i++)
    {
        for (int j = i + 1; j < aDistances.GetSize(); j++)
        {
            if (aDistances[i].distance > aDistances[j].distance)
            {
                NodeDistance temp = aDistances[i];
                aDistances[i] = aDistances[j];
                aDistances[j] = temp;
            }
        }
    }

    T_NODE_D targetCoord;
    UINT resultNodeK = 0;

    if (bOrtho)
    {
        // ===== Ortho 모드: 가장 가까운 1개의 Node에서 수선의 발 =====
        T_NODE_D projPoint;
        bool isInsideSegment;
        ProjectPointToLine(aDistances[0].nodeD, guideNode1, guideNode2, projPoint, isInsideSegment);

        if (isInsideSegment)
        {
            // 수선의 발이 선분 내부에 있음 → 교점 좌표 사용
            targetCoord = projPoint;

            // 해당 좌표에 기존 Node가 있는지 확인
            m_pDoc->m_pAttrCtrl->ExistNode(targetCoord.x, targetCoord.y, targetCoord.z, resultNodeK);

            if (resultNodeK == 0)
            {
                // 새 Node 생성
                resultNodeK = m_pDoc->m_pAttrCtrl->GetStartNumNode();
                T_NODE_D newNode;
                newNode.x = targetCoord.x;
                newNode.y = targetCoord.y;
                newNode.z = targetCoord.z;

                if (!m_pDoc->m_pDataCtrl->AddNode(resultNodeK, newNode))
                {
                    AfxMessageBox(_T("Failed to create new node."));
                    ASSERT(0);
                    return 0;
                }
            }
        }
        else
        {
            // 수선의 발이 선분 외부 → 수선의 발과 가장 가까운 Guide Line 끝점 사용
            double dist1 = sqrt(pow(projPoint.x - guideNode1.x, 2) +
                               pow(projPoint.y - guideNode1.y, 2) +
                               pow(projPoint.z - guideNode1.z, 2));
            double dist2 = sqrt(pow(projPoint.x - guideNode2.x, 2) +
                               pow(projPoint.y - guideNode2.y, 2) +
                               pow(projPoint.z - guideNode2.z, 2));

            if (dgn::LT(dist1, dist2))
                resultNodeK = aGuideNodeK[0];
            else
                resultNodeK = aGuideNodeK[1];
        }
    }
    else
    {
        // ===== 일반 모드: 가장 가까운 2개의 Node로 만든 직선과 Guide Line의 교점 =====
        if (aDistances.GetSize() < 2)
        {
            AfxMessageBox(_T("Not enough support nodes for normal mode."));
            ASSERT(0);
            return 0;
        }

        // 가장 가까운 2개의 Node로 직선 만들기
        T_NODE_D supportNode1 = aDistances[0].nodeD;
        T_NODE_D supportNode2 = aDistances[1].nodeD;

        // 두 직선의 교점 계산
        T_NODE_D intersection;
        if (!FindLineIntersection(supportNode1, supportNode2, guideNode1, guideNode2, intersection))
        {
            AfxMessageBox(_T("Failed to find intersection (lines are parallel)."));
            ASSERT(0);
            return 0;
        }

        // 교점이 Guide Line 선분 내부에 있는지 확인
        T_NODE_D projToGuide;
        bool isInsideGuideSegment;
        ProjectPointToLine(intersection, guideNode1, guideNode2, projToGuide, isInsideGuideSegment);

        if (isInsideGuideSegment)
        {
            // 교점이 Guide Line 선분 내부 → 교점 좌표 사용
            targetCoord = intersection;

            // 해당 좌표에 기존 Node가 있는지 확인
            m_pDoc->m_pAttrCtrl->ExistNode(targetCoord.x, targetCoord.y, targetCoord.z, resultNodeK);

            if (resultNodeK == 0)
            {
                // 새 Node 생성
                resultNodeK = m_pDoc->m_pAttrCtrl->GetStartNumNode();
                T_NODE_D newNode;
                newNode.x = targetCoord.x;
                newNode.y = targetCoord.y;
                newNode.z = targetCoord.z;

                if (!m_pDoc->m_pDataCtrl->AddNode(resultNodeK, newNode))
                {
                    AfxMessageBox(_T("Failed to create new node."));
                    ASSERT(0);
                    return 0;
                }
            }
        }
        else
        {
            // 교점이 Guide Line 선분 외부 → 교점과 가장 가까운 Guide Line 끝점 사용
            double dist1 = sqrt(pow(intersection.x - guideNode1.x, 2) +
                               pow(intersection.y - guideNode1.y, 2) +
                               pow(intersection.z - guideNode1.z, 2));
            double dist2 = sqrt(pow(intersection.x - guideNode2.x, 2) +
                               pow(intersection.y - guideNode2.y, 2) +
                               pow(intersection.z - guideNode2.z, 2));

            if (dgn::LT(dist1, dist2))
                resultNodeK = aGuideNodeK[0];
            else
                resultNodeK = aGuideNodeK[1];
        }
    }

    return resultNodeK;
}

LRESULT CSupportLineCreateDlg::OnMViewCancel(WPARAM wParam, LPARAM lParam)
{
    HWND hWnd = (HWND)lParam;

    if (m_edtSupNode.GetSafeHwnd() == hWnd)
    {
        m_edtSupNode.ClearNodeKeyList();
        m_edtSupNode.SetWindowText(_T(""));
    }
    else if (m_edtGuideLine1.GetSafeHwnd() == hWnd)
    {
        m_edtGuideLine1.ClearNodeKeyList();
        m_edtGuideLine1.SetWindowText(_T(""));
    }
    else if (m_edtGuideLine2.GetSafeHwnd() == hWnd)
    {
        m_edtGuideLine2.ClearNodeKeyList();
        m_edtGuideLine2.SetWindowText(_T(""));
    }

    return 0L;
}

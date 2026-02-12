#include "stdafx.h"
#include "wg_treemenu.h"
#include "PostTensionDrawTendonLineSlabDlg.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\ServiceProvider.h"

#include "..\wg_gr\Gr_ServiceDef.h"

#include "..\wg_main\wg_mainRes2.h"

#include "PostTensionDrawTendonLineDlg.h"

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

enum
{
    EN_SEL_BY_DGN_SPTL = 0,
    EN_SEL_BY_SUP_NODES,
};

CPostTensionDrawTendonLineSlabDlg::CPostTensionDrawTendonLineSlabDlg(CWnd* pParent)
    : CMenuBarChildDlg(CPostTensionDrawTendonLineSlabDlg::IDD, pParent)
{
    m_pDoc = CDBDoc::GetDocPoint();

    m_pParentDlg = dynamic_cast<CPostTensionDrawTendonLineDlg*>(pParent);

    m_nLayoutType = 0;
    m_nDirection = 0;
    m_nArrangement = 0;
    m_nEndType = 0;
    m_nLineShape = 0;
    m_nSelectDraw = 0;

    m_nCurDistY = 0;
    m_nPrevDistY = 0;
    m_nOriginalPlaceHolderHeight = 0;
    m_nOriginalDialogHeight = 0;
    m_nOriginalLineShapeBottom = 0;
    
    m_Data.Initialize();

    m_aCtrlBandedType.Add(IDC_TENDON_LINE_SLAB_WIDTH_STC);
    m_aCtrlBandedType.Add(IDC_TENDON_LINE_SLAB_TENDON_PROP_EDT2);
    m_aCtrlBandedType.Add(IDC_TENDON_LINE_SLAB_WIDTH_UNIT_STC);
    m_aCtrlBandedType.Add(IDC_TENDON_LINE_SLAB_TENDON_LINE_NO_STC);
    m_aCtrlBandedType.Add(IDC_TENDON_LINE_SLAB_TENDON_PROP_EDT3);
    m_aCtrlBandedType.Add(IDC_TENDON_LINE_SLAB_TENDON_LINE_NO_AUTO_CHK);

    m_aCtrlEndType.Add(IDC_TENDON_LINE_SLAB_END_TYPE_GRP);
    m_aCtrlEndType.Add(IDC_TENDON_LINE_SLAB_END_LF_RDO);
    m_aCtrlEndType.Add(IDC_TENDON_LINE_SLAB_END_LL_RDO);
    m_aCtrlEndType.Add(IDC_TENDON_LINE_SLAB_END_FL_RDO);

    m_aCtrlDraw.Add(IDC_TENDON_LINE_SLAB_DRAW_GRP);
    m_aCtrlDraw.Add(IDC_TENDON_LINE_SLAB_DRAW_DGNSTRIP_RDO);
    m_aCtrlDraw.Add(IDC_TENDON_LINE_SLAB_DRAW_SUPPORTNODE_RDO);
    m_aCtrlDraw.Add(IDC_TENDON_LINE_SLAB_DRAW_NODES_EDT);
    m_aCtrlDraw.Add(IDC_TENDON_LINE_SLAB_OFFSET_STC);
    m_aCtrlDraw.Add(IDC_TENDON_LINE_SLAB_OFFSET_EDT);
    m_aCtrlDraw.Add(IDC_TENDON_LINE_SLAB_UNIT_STC2);

    m_aCtrlLineShape.Add(IDC_TENDON_LINE_SLAB_LINE_TYPE_GRP);
    m_aCtrlLineShape.Add(IDC_TENDON_LINE_SLAB_LINE_TYPE_STRAIGHT_RDO);
    m_aCtrlLineShape.Add(IDC_TENDON_LINE_SLAB_LINE_TYPE_SPLINE_RDO);

    m_aCtrlArrangement.Add(IDC_TENDON_LINE_SLAB_ARR_GRP);
    m_aCtrlArrangement.Add(IDC_TENDON_LINE_SLAB_ARR_SPACE_RDO);
    m_aCtrlArrangement.Add(IDC_TENDON_LINE_SLAB_ARR_SPACE_EDT);
    m_aCtrlArrangement.Add(IDC_TENDON_LINE_SLAB_UNIT_STC);
    m_aCtrlArrangement.Add(IDC_TENDON_LINE_SLAB_ARR_TENDON_LINE_RDO);
    m_aCtrlArrangement.Add(IDC_TENDON_LINE_SLAB_ARR_SPACE_EDT2);

    m_aCtrlMethod.Add(IDC_TENDON_LINE_SLAB_METHOD_GRP);
    m_aCtrlMethod.Add(IDC_TENDON_LINE_SLAB_METHOD_LINE1_STC);
    m_aCtrlMethod.Add(IDC_TENDON_LINE_SLAB_METHOD_LINE1_CMB);
    m_aCtrlMethod.Add(IDC_TENDON_LINE_SLAB_METHOD_LINE1_EDT);
    m_aCtrlMethod.Add(IDC_TENDON_LINE_SLAB_METHOD_LINE2_STC);
    m_aCtrlMethod.Add(IDC_TENDON_LINE_SLAB_METHOD_LINE2_CMB);
    m_aCtrlMethod.Add(IDC_TENDON_LINE_SLAB_METHOD_LINE2_EDT);

    m_aCtrlDlg.Add(IDD_TM_TENDON_LINE_SLAB);
}

CPostTensionDrawTendonLineSlabDlg::~CPostTensionDrawTendonLineSlabDlg()
{
    InitSelectionFilter();
}

void CPostTensionDrawTendonLineSlabDlg::DoDataExchange(CDataExchange* pDX)
{
    CMenuBarChildDlg::DoDataExchange(pDX);
    // Tendon Property
    DDX_Control(pDX, IDC_TENDON_LINE_SLAB_TENDON_PROP_STC, m_stcTendonProp);
    DDX_Control(pDX, IDC_TENDON_LINE_SLAB_TENDON_PROP_CMB, m_cmbTendonProp);
    DDX_Control(pDX, IDC_TENDON_LINE_SLAB_TENDON_PROP_BTN, m_btnTendonProp);
    // Tendon Profile
    DDX_Control(pDX, IDC_TENDON_LINE_SLAB_TENDON_PROF_STC, m_stcTendonProf);
    DDX_Control(pDX, IDC_TENDON_LINE_SLAB_TENDON_PROF_CMB, m_cmbTendonProf);
    DDX_Control(pDX, IDC_TENDON_LINE_SLAB_TENDON_PROF_BTN, m_btnTendonProf);
    // Tendon Property
    DDX_Control(pDX, IDC_TENDON_LINE_SLAB_TENDON_STRS_STC, m_stcTendonStrs);
    DDX_Control(pDX, IDC_TENDON_LINE_SLAB_TENDON_STRS_CMB, m_cmbTendonStrs);
    DDX_Control(pDX, IDC_TENDON_LINE_SLAB_TENDON_STRS_BTN, m_btnTendonStrs);
    // Layout Type
    DDX_Radio(pDX, IDC_TENDON_LINE_SLAB_LAYOUT_SINGLE_RDO, m_nLayoutType);
    // Direction
    DDX_Radio(pDX, IDC_TENDON_LINE_SLAB_XDIR_RDO, m_nDirection);
    // Arrangement
    DDX_Radio(pDX, IDC_TENDON_LINE_SLAB_ARR_SPACE_RDO, m_nArrangement);
    DDX_Control(pDX, IDC_TENDON_LINE_SLAB_ARR_SPACE_EDT, m_edtArrSpace);
    DDX_Control(pDX, IDC_TENDON_LINE_SLAB_ARR_SPACE_EDT2, m_edtArrSpace2);
    // End Type
    DDX_Radio(pDX, IDC_TENDON_LINE_SLAB_END_LF_RDO, m_nEndType);
    // Method
    DDX_Control(pDX, IDC_TENDON_LINE_SLAB_METHOD_LINE1_CMB, m_cmbMethodLine1);
    DDX_Control(pDX, IDC_TENDON_LINE_SLAB_METHOD_LINE1_EDT, m_edtMethodLine1);
    DDX_Control(pDX, IDC_TENDON_LINE_SLAB_METHOD_LINE2_CMB, m_cmbMethodLine2);
    DDX_Control(pDX, IDC_TENDON_LINE_SLAB_METHOD_LINE2_EDT, m_edtMethodLine2);
    // Line Type
    DDX_Radio(pDX, IDC_TENDON_LINE_SLAB_LINE_TYPE_STRAIGHT_RDO, m_nLineShape);
    // Band Width, Tendon Line No
    DDX_Control(pDX, IDC_TENDON_LINE_SLAB_TENDON_PROP_EDT2, m_edtBandWidth);
    DDX_Control(pDX, IDC_TENDON_LINE_SLAB_TENDON_PROP_EDT3, m_edtTendonLineNo);
    DDX_Control(pDX, IDC_TENDON_LINE_SLAB_TENDON_LINE_NO_AUTO_CHK, m_chkTendonLineNoAuto);
    // Draw
    DDX_Radio(pDX, IDC_TENDON_LINE_SLAB_DRAW_DGNSTRIP_RDO, m_nSelectDraw);
    DDX_Control(pDX, IDC_TENDON_LINE_SLAB_DRAW_NODES_EDT, m_edtDrawNodes);
    // Offset
    DDX_Control(pDX, IDC_TENDON_LINE_SLAB_OFFSET_EDT, m_edtOffset);
}

void CPostTensionDrawTendonLineSlabDlg::Dlg2Data()
{
    UpdateData(TRUE);
    m_Data.Initialize();

    // Tendon Property - 콤보박스 문자열로부터 TdntK 가져오기
    int nPropSel = m_cmbTendonProp.GetCurSel();
    if (nPropSel >= 0)
    {
        CString strSelected;
        m_cmbTendonProp.GetLBText(nPropSel, strSelected);

        CArray<T_TDNT_K, T_TDNT_K> aTdntK;
        m_pDoc->m_pAttrCtrl->GetTdntKeyList(aTdntK);
        T_TDNT_D TdntD;
        for (int i = 0; i < aTdntK.GetSize(); i++)
        {
            if (m_pDoc->m_pAttrCtrl->GetTdnt(aTdntK[i], TdntD))
            {
                if (TdntD.TendonTypeName == strSelected)
                {
                    m_Data.TdntK = aTdntK[i];
                    break;
                }
            }
        }
    }
	// Tendon Profile - 콤보박스 문자열로부터 TdpfK 가져오기
	int nProfSel = m_cmbTendonProf.GetCurSel();
    if (nProfSel >= 0)
    {
        CString strSelected;
        m_cmbTendonProf.GetLBText(nProfSel, strSelected);

		CArray<T_TDPF_K, T_TDPF_K> aTdpfK;
		m_pDoc->m_pAttrCtrl2->GetTdpfKeyList(aTdpfK);
        T_TDPF_D TdpfD;
        for (int i = 0; i < aTdpfK.GetSize(); i++)
        {
            if (m_pDoc->m_pAttrCtrl2->GetTdpf(aTdpfK[i], TdpfD))
            {
                if (TdpfD.nElemType != 1) continue;

                if (TdpfD.strName == strSelected)
                {
                    m_Data.TdpfK = aTdpfK[i];
                    break;
                }
            }
        }
    }
	// Tendon Stressing - 콤보박스 문자열로부터 TdstK 가져오기
	int nStrsSel = m_cmbTendonStrs.GetCurSel();
    if (nStrsSel >= 0)
	{
        CString strSelected;
        m_cmbTendonStrs.GetLBText(nStrsSel, strSelected);

		CArray<T_TDST_K, T_TDST_K> aTdstK;
        m_pDoc->m_pAttrCtrl2->GetTdstKeyList(aTdstK);
        T_TDST_D TdstD;
        for (int i = 0; i < aTdstK.GetSize(); i++)
        {
            if (m_pDoc->m_pAttrCtrl2->GetTdst(aTdstK[i], TdstD))
            {
                if (TdstD.strName == strSelected)
                {
                    m_Data.TdstK = aTdstK[i];
                    break;
                }
            }
        }
	}
    m_Data.nLayoutType = m_nLayoutType;
    m_Data.nDirection = m_nDirection;
    m_Data.nArrangement = m_nArrangement;
    m_Data.nEndType = m_nEndType;
    m_Data.nLineType = m_nLineShape;

    // Band Width
    CString strBandWidth;
    m_edtBandWidth.GetWindowText(strBandWidth);
    m_Data.dBandWidth = _tstof(strBandWidth);

    // Tendon Line No
    CString strTendonLineNo;
    m_edtTendonLineNo.GetWindowText(strTendonLineNo);
    m_Data.nTendonLineNo = _ttoi(strTendonLineNo);
    m_Data.bAuto = (m_chkTendonLineNoAuto.GetCheck() == BST_CHECKED);

    // Arrangement Spacing
    CString strArrSpacing;
    m_edtArrSpace.GetWindowText(strArrSpacing);
    m_Data.dArrSpacing = _tstof(strArrSpacing);
    CString strArrTendonLineNo;
    m_edtArrSpace2.GetWindowText(strArrTendonLineNo);
    m_Data.nArrTendonLineNo = _ttoi(strArrTendonLineNo);

    // Method 1
    m_Data.nMethodLine1 = m_cmbMethodLine1.GetCurSel();
    CString strMethodLine1;
    m_edtMethodLine1.GetWindowText(strMethodLine1);
    m_Data.aLine1.RemoveAll();
	GetNodeInfoBySupportingNodes(strMethodLine1, m_Data.aLine1);

	// Method 2
    m_Data.nMethodLine2 = m_cmbMethodLine2.GetCurSel();
    CString strMethodLine2;
    m_edtMethodLine2.GetWindowText(strMethodLine2);
    m_Data.aLine2.RemoveAll();
	GetNodeInfoBySupportingNodes(strMethodLine2, m_Data.aLine2);

    // Draw
    m_Data.nDrawType = m_nSelectDraw;
    
    switch (m_nSelectDraw)
    {
    case EN_SEL_BY_DGN_SPTL:
        {
            DO_NOTHING();
        }
        break;
    case EN_SEL_BY_SUP_NODES:
        {
            CArray<UINT, UINT> aNodeK;
            aNodeK.RemoveAll();
			CString strNodes;
			m_edtDrawNodes.GetWindowText(strNodes);
            GetNodeInfoBySupportingNodes(strNodes, aNodeK);

            m_Data.aNodeK.RemoveAll();
            for (auto i = 0; i < aNodeK.GetCount(); i++)
            {
                m_Data.aNodeK.Add(aNodeK[i]);
            }
        }
        break;
    default:
        {
            ASSERT(0); return;
        }
    }

    // Offset
    CString strOffset;
    m_edtOffset.GetWindowText(strOffset);
    m_Data.dOffset = _tstof(strOffset);

    // Line Type
    m_Data.nLineType = m_nLineShape;
}

void CPostTensionDrawTendonLineSlabDlg::UpdateControlsLayout()
{
    UpdateData(TRUE);

    CRect rectLayoutType;
    GetDlgItem(IDC_TENDON_LINE_SLAB_LAYOUT_GRP)->GetWindowRect(rectLayoutType);
    ScreenToClient(rectLayoutType);

    CRect rectDirection;
    GetDlgItem(IDC_TENDON_LINE_SLAB_DIR_GRP)->GetWindowRect(rectDirection);
    ScreenToClient(rectDirection);

    CRect rectEndType;
    GetDlgItem(IDC_TENDON_LINE_SLAB_END_TYPE_GRP)->GetWindowRect(rectEndType);
    ScreenToClient(rectEndType);

    CRect rectTendonProp;
    GetDlgItem(IDC_TENDON_LINE_SLAB_TENDON_PROP_STC)->GetWindowRect(rectTendonProp);
    ScreenToClient(rectTendonProp);

    CRect rectBandTop;
    GetDlgItem(IDC_TENDON_LINE_SLAB_WIDTH_STC)->GetWindowRect(rectBandTop);
    ScreenToClient(rectBandTop);

    CRect rectBandBot;
    GetDlgItem(IDC_TENDON_LINE_SLAB_TENDON_LINE_NO_STC)->GetWindowRect(rectBandBot);
    ScreenToClient(rectBandBot);

    CRect rectArr;
    GetDlgItem(IDC_TENDON_LINE_SLAB_ARR_GRP)->GetWindowRect(rectArr);
    ScreenToClient(rectArr);

    CRect rectDraw;
    GetDlgItem(IDC_TENDON_LINE_SLAB_DRAW_GRP)->GetWindowRect(rectDraw);
    ScreenToClient(rectDraw);

    CRect rectMethod;
    GetDlgItem(IDC_TENDON_LINE_SLAB_METHOD_GRP)->GetWindowRect(rectMethod);
    ScreenToClient(rectMethod);

    CRect rectLineType;
    GetDlgItem(IDC_TENDON_LINE_SLAB_LINE_TYPE_GRP)->GetWindowRect(rectLineType);
    ScreenToClient(rectLineType);

    CRect rectDialog;
    GetWindowRect(rectDialog);

    // 이전 이동량 롤백
    if (m_nPrevDistY != 0) {
        CDlgUtil::CtrlMoveDistY(this, m_aCtrlEndType, -m_nPrevDistY);
        CDlgUtil::CtrlMoveDistY(this, m_aCtrlDraw, -m_nPrevDistY);
        CDlgUtil::CtrlMoveDistY(this, m_aCtrlMethod, -m_nPrevDistY);
    }
    // LineShape는 별도로 롤백 (Distributed 모드에서 별도 계산되므로)
    if (m_nPrevLineShapeDistY != 0) {
        CDlgUtil::CtrlMoveDistY(this, m_aCtrlLineShape, -m_nPrevLineShapeDistY);
    }

    switch (m_nLayoutType)
    {
    case 0: // Single
        {
            CDlgUtil::CtrlShowHide(this, m_aCtrlBandedType, FALSE);
            CDlgUtil::CtrlShowHide(this, m_aCtrlArrangement, FALSE);
            CDlgUtil::CtrlShowHide(this, m_aCtrlMethod, FALSE);
            CDlgUtil::CtrlShowHide(this, m_aCtrlDraw, TRUE);

            m_nCurDistY = 0;
            m_nCurLineShapeDistY = 0;
        }
        break;
    case 1: // Banded
        {
            CDlgUtil::CtrlShowHide(this, m_aCtrlBandedType, TRUE);
            CDlgUtil::CtrlShowHide(this, m_aCtrlArrangement, FALSE);
            CDlgUtil::CtrlShowHide(this, m_aCtrlMethod, FALSE);
            CDlgUtil::CtrlShowHide(this, m_aCtrlDraw, TRUE);

            int nBandedTypeHeight = rectBandBot.bottom - rectBandTop.top;
            int nSpacing = rectDirection.top - rectLayoutType.bottom;
            m_nCurDistY = nBandedTypeHeight + (4);
            m_nCurLineShapeDistY = m_nCurDistY;
        }
        break;
    case 2: // Distributed
        {
            CDlgUtil::CtrlShowHide(this, m_aCtrlBandedType, FALSE);
            CDlgUtil::CtrlShowHide(this, m_aCtrlArrangement, TRUE);
            CDlgUtil::CtrlShowHide(this, m_aCtrlMethod, TRUE);
            CDlgUtil::CtrlShowHide(this, m_aCtrlDraw, FALSE);

            int nArrHeight = rectArr.bottom - rectArr.top;
            int nSpacing = rectDirection.top - rectLayoutType.bottom;
            m_nCurDistY = nArrHeight + (nSpacing);

            // Distributed 모드: LineShape는 Method 그룹 바로 아래(+nSpacing)에 배치
            // Method 그룹이 이동된 후의 bottom 위치에서 LineType의 원래 top 위치까지의 거리 계산
            // Method 이동 후 bottom = rectMethod.bottom + m_nCurDistY
            // LineType 원래 top = rectLineType.top
            // 필요한 이동량 = (Method 이동 후 bottom + nSpacing) - LineType 원래 top
            int nMethodMovedBottom = rectMethod.bottom + m_nCurDistY;
            m_nCurLineShapeDistY = (nMethodMovedBottom + nSpacing) - rectLineType.top;
        }
        break;
    }

    if (m_nCurDistY != 0)
    {
        CDlgUtil::CtrlMoveDistY(this, m_aCtrlEndType, m_nCurDistY);
        CDlgUtil::CtrlMoveDistY(this, m_aCtrlDraw, m_nCurDistY);
        CDlgUtil::CtrlMoveDistY(this, m_aCtrlMethod, m_nCurDistY);
    }
    // LineShape는 별도 이동량 적용
    if (m_nCurLineShapeDistY != 0)
    {
        CDlgUtil::CtrlMoveDistY(this, m_aCtrlLineShape, m_nCurLineShapeDistY);
    }

    if (m_nLayoutType == 1)
    {
        int nMoveDistBandWidth = rectTendonProp.left - rectBandTop.left;
        CDlgUtil::CtrlMoveDistXY(this, m_aCtrlBandedType, nMoveDistBandWidth, 0);
    }
    if (m_nLayoutType == 2)
    {
        int nMoveDistArr = rectLayoutType.left - rectArr.left;
        CDlgUtil::CtrlMoveDistXY(this, m_aCtrlArrangement, nMoveDistArr, 0);
        int nMoveDistMethod = rectLayoutType.left - rectMethod.left;
        CDlgUtil::CtrlMoveDistXY(this, m_aCtrlMethod, nMoveDistMethod, 0);
    }

    GetDlgItem(IDC_TENDON_LINE_SLAB_LINE_TYPE_GRP)->GetWindowRect(rectLineType);
    int nFinalLineTypeBottomChange = rectLineType.bottom - m_nOriginalLineShapeBottom;
    int nNewDialogHeight = m_nOriginalDialogHeight;
    if (nFinalLineTypeBottomChange > 0)
    {
        nNewDialogHeight = m_nOriginalDialogHeight + nFinalLineTypeBottomChange + 10;
    }
    SetWindowPos(NULL, 0, 0, rectDialog.Width(), nNewDialogHeight, SWP_NOMOVE | SWP_NOZORDER);

    // Distributed 모드에서는 LineShape 이동량 기준으로 부모 리사이즈
    int nResizeHeight = (m_nLayoutType == 2) ? m_nCurLineShapeDistY : m_nCurDistY;
    ResizeParent(nResizeHeight);
    RedrawWindow();

    m_nPrevDistY = m_nCurDistY;
    m_nPrevLineShapeDistY = m_nCurLineShapeDistY;
}

void CPostTensionDrawTendonLineSlabDlg::ResizeParent(int nHeightDelta)
{
    if (m_pParentDlg && ::IsWindow(m_pParentDlg->GetSafeHwnd()))
    {
        CRect PHRect;
        m_pParentDlg->m_wndPlaceHolder.GetWindowRect(&PHRect);
        
        m_nOriginalPlaceHolderHeight = m_pParentDlg->GetOrgSlabPlaceHolderSize();
        int nNewHeight = m_nOriginalPlaceHolderHeight + nHeightDelta;
        m_pParentDlg->SetHolderSize(nNewHeight);
        m_pParentDlg->RedrawWindow();
    }
}

void CPostTensionDrawTendonLineSlabDlg::Execute()
{
    enum
    {
        EN_LAYOUT_SINGLE = 0,
        EN_LAYOUT_BANDED,
        EN_LAYOUT_DISTRIBUTED,
    };

    UpdateData(TRUE);

    m_Data.Initialize();

    Dlg2Data();

    switch (m_nLayoutType)
    {
    case EN_LAYOUT_SINGLE:
    case EN_LAYOUT_BANDED:
        {
            switch (m_nSelectDraw)
            {
            case EN_SEL_BY_DGN_SPTL:
                Execute4SupLine();
                break;
            case EN_SEL_BY_SUP_NODES:
                Execute4SupNode();
                break;
            default:
                ASSERT(0); return;
            }
        }
        break;
    case EN_LAYOUT_DISTRIBUTED:
        {
		switch (m_Data.nMethodLine1) // = m_Data.nMethodLine2
		{
		case EN_SEL_BY_DGN_SPTL:
			Execute4SupLine_Distributed();
			break;
		case EN_SEL_BY_SUP_NODES:
            Execute4SupNode_Distributed();
			break;
		default:
			ASSERT(0); return;
		}
	}
        break;
    default:
        {
            ASSERT(0); return;
        }
    }
}

void CPostTensionDrawTendonLineSlabDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CPostTensionDrawTendonLineSlabDlg::UpdateBuffer()
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
        case(UR_TDNT_ADD):
        case(UR_TDNT_DEL):
        case(UR_TDNT_MFD):
        case(UR_TDNT_MFS):
            {
                // Tendon Property
                UpdateTendonPropertyList();
            }
            break;
        case(UR_TDPF_ADD):
        case(UR_TDPF_DEL):
        case(UR_TDPF_MFD):
        case(UR_TDPF_MFS):
            {
                // Tendon Profile
                UpdateTendonProfileList();
            }
            break;
        case(UR_TDST_ADD):
        case(UR_TDST_DEL):
        case(UR_TDST_MFD):
        case(UR_TDST_MFS):
            {
                // Tendon Stressing
                UpdateTendonStressingList();
            }
            break;
        default:
            break;
        }
    }
}

BEGIN_MESSAGE_MAP(CPostTensionDrawTendonLineSlabDlg, CMenuBarChildDlg)
    // Tendon Property
    ON_BN_CLICKED(IDC_TENDON_LINE_SLAB_TENDON_PROP_BTN, OnClickTendonPropBtn)
	// Tendon Profile
	ON_BN_CLICKED(IDC_TENDON_LINE_SLAB_TENDON_PROF_BTN, OnClickTendonProfBtn)
	// Tendon Stressing
	ON_BN_CLICKED(IDC_TENDON_LINE_SLAB_TENDON_STRS_BTN, OnClickTendonStrsBtn)
    // Layout Type
    ON_BN_CLICKED(IDC_TENDON_LINE_SLAB_LAYOUT_SINGLE_RDO, OnLayoutTypeRadioClicked)
    ON_BN_CLICKED(IDC_TENDON_LINE_SLAB_LAYOUT_BANDED_RDO, OnLayoutTypeRadioClicked)
    ON_BN_CLICKED(IDC_TENDON_LINE_SLAB_LAYOUT_DIST_RDO, OnLayoutTypeRadioClicked)
    // Direction
    ON_BN_CLICKED(IDC_TENDON_LINE_SLAB_XDIR_RDO, OnDirectionRadioClicked)
    ON_BN_CLICKED(IDC_TENDON_LINE_SLAB_YDIR_RDO, OnDirectionRadioClicked)
    // Arrangement
    ON_BN_CLICKED(IDC_TENDON_LINE_SLAB_ARR_SPACE_RDO, OnArrangementRadioClicked)
    ON_BN_CLICKED(IDC_TENDON_LINE_SLAB_ARR_TENDON_LINE_RDO, OnArrangementRadioClicked)
    // End Type
    ON_BN_CLICKED(IDC_TENDON_LINE_SLAB_END_LF_RDO, OnEndTypeRadioClicked)
    ON_BN_CLICKED(IDC_TENDON_LINE_SLAB_END_LL_RDO, OnEndTypeRadioClicked)
    ON_BN_CLICKED(IDC_TENDON_LINE_SLAB_END_FL_RDO, OnEndTypeRadioClicked)
    // Method
    ON_CBN_SELCHANGE(IDC_TENDON_LINE_SLAB_METHOD_LINE1_CMB, OnMethodLine1ComboChanged)
    ON_CBN_SELCHANGE(IDC_TENDON_LINE_SLAB_METHOD_LINE2_CMB, OnMethodLine2ComboChanged)
    ON_EN_SETFOCUS(IDC_TENDON_LINE_SLAB_METHOD_LINE1_EDT, OnSetFocusMethodLine1Edit)
    ON_EN_KILLFOCUS(IDC_TENDON_LINE_SLAB_METHOD_LINE1_EDT, OnKillFocusMethodLine1Edit)
    ON_EN_SETFOCUS(IDC_TENDON_LINE_SLAB_METHOD_LINE2_EDT, OnSetFocusMethodLine2Edit)
    ON_EN_KILLFOCUS(IDC_TENDON_LINE_SLAB_METHOD_LINE2_EDT, OnKillFocusMethodLine2Edit)
    // Line Type
    ON_BN_CLICKED(IDC_TENDON_LINE_SLAB_LINE_TYPE_STRAIGHT_RDO, OnLineTypeRadioClicked)
    ON_BN_CLICKED(IDC_TENDON_LINE_SLAB_LINE_TYPE_SPLINE_RDO, OnLineTypeRadioClicked)
    // Draw
    ON_BN_CLICKED(IDC_TENDON_LINE_SLAB_DRAW_DGNSTRIP_RDO, OnDrawRadioClicked)
    ON_BN_CLICKED(IDC_TENDON_LINE_SLAB_DRAW_SUPPORTNODE_RDO, OnDrawRadioClicked)
END_MESSAGE_MAP()

BOOL CPostTensionDrawTendonLineSlabDlg::OnInitDialog()
{
    CMenuBarChildDlg::OnInitDialog();
    
    // Tendon Property
    UpdateTendonPropertyList();

    // Tendon Profile
    UpdateTendonProfileList();

    // Tendon Stressing
    UpdateTendonStressingList();

    CString strVal1 = _T("0.0");
    m_edtArrSpace.SetWindowText(strVal1);
    CString strVal2 = _T("1");
    m_edtArrSpace2.SetWindowText(strVal2);

    m_edtDrawNodes.Disconnect();

    InitCtrl();

    m_cmbMethodLine1.AddString(_T("Support Line"));
    m_cmbMethodLine1.AddString(_T("Support Nodes"));
    m_cmbMethodLine1.SetCurSel(0);

    m_cmbMethodLine2.AddString(_T("Support Line"));
    m_cmbMethodLine2.AddString(_T("Support Nodes"));
    m_cmbMethodLine2.SetCurSel(0);
    m_cmbMethodLine2.EnableWindow(FALSE); // METHOD_LINE2를 Read Only로 설정

    // METHOD_LINE Edit 컨트롤 초기화 - 포커스가 없을 때는 Disconnect 상태
    m_edtMethodLine1.Disconnect();
    m_edtMethodLine2.Disconnect();

    m_edtOffset.SetWindowText(_T("0.0"));

    CDlgUtil::CtrlShowHide(this, m_aCtrlBandedType, FALSE);
    CDlgUtil::CtrlShowHide(this, m_aCtrlArrangement, FALSE);
    CDlgUtil::CtrlShowHide(this, m_aCtrlMethod, FALSE);
    CDlgUtil::CtrlShowHide(this, m_aCtrlLineShape, FALSE);
    
    if (m_nOriginalDialogHeight == 0) 
    {
        CRect rectDialog, rectLineType;
        GetWindowRect(&rectDialog);
        GetDlgItem(IDC_TENDON_LINE_SLAB_LINE_TYPE_GRP)->GetWindowRect(&rectLineType);
        m_nOriginalDialogHeight = rectDialog.Height();
        m_nOriginalLineShapeBottom = rectLineType.bottom;
    }

    return TRUE;
}

void CPostTensionDrawTendonLineSlabDlg::OnClickTendonPropBtn()
{
    AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_TENDON_PROP, 0));
}

void CPostTensionDrawTendonLineSlabDlg::OnClickTendonProfBtn()
{
    AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_PTDN_TENDON_PROFILE_SET, 0));
}

void CPostTensionDrawTendonLineSlabDlg::OnClickTendonStrsBtn()
{
    AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_PTDN_TENDON_STRESSING, 0));
}

void CPostTensionDrawTendonLineSlabDlg::OnEndTypeRadioClicked()
{
    UpdateData(TRUE);
    switch (m_nEndType)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    default:
        ASSERT(0); return;
    }
}

void CPostTensionDrawTendonLineSlabDlg::OnLayoutTypeRadioClicked()
{
    UpdateData(TRUE);

    UpdateControlsLayout();

    OnDrawRadioClicked();

    switch (m_nLayoutType)
    {
    case 0:
    case 1:
        {
            DO_NOTHING();
        }
        break;
    case 2:
        {
            OnMethodLine1ComboChanged();
        }
        break;
    default:
        {
            ASSERT(0); return;
        }
    }
}

void CPostTensionDrawTendonLineSlabDlg::OnDirectionRadioClicked()
{
    UpdateData(TRUE);
}

void CPostTensionDrawTendonLineSlabDlg::OnArrangementRadioClicked()
{
    enum 
    {
        EN_BY_SPACING = 0,
        EN_BY_TENDON_LINE,
    };

    UpdateData(TRUE);

    switch (m_nArrangement)
    {
    case EN_BY_SPACING:
        {
            GetDlgItem(IDC_TENDON_LINE_SLAB_ARR_SPACE_EDT)->EnableWindow(TRUE);
            GetDlgItem(IDC_TENDON_LINE_SLAB_ARR_SPACE_EDT2)->EnableWindow(FALSE);
        }
        break;
    case EN_BY_TENDON_LINE:
        {
            GetDlgItem(IDC_TENDON_LINE_SLAB_ARR_SPACE_EDT)->EnableWindow(FALSE);
            GetDlgItem(IDC_TENDON_LINE_SLAB_ARR_SPACE_EDT2)->EnableWindow(TRUE);
        }
        break;
    default:
        {
            ASSERT(0);
        }
        break;
    }

    return;
}

void CPostTensionDrawTendonLineSlabDlg::OnMethodLine1ComboChanged()
{
    enum
    {
        EN_DRAW_SEL_SPTL = 0,
        EN_DRAW_SEL_NODE,
    };

    // 콤보박스가 변경될 때 기존 연결 끊기
    m_edtMethodLine1.Disconnect();
    m_edtMethodLine2.Disconnect();

    UpdateData(TRUE);

    // METHOD_LINE1의 Index를 METHOD_LINE2에 동일하게 연동
    int nCurSel = m_cmbMethodLine1.GetCurSel();
    m_cmbMethodLine2.SetCurSel(nCurSel);

    I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
    switch (nCurSel)
    {
    case EN_DRAW_SEL_SPTL:
        {
            pIGM->ChangeSelectionFilterCombo(EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_SPTL);
        }
        break;
    case EN_DRAW_SEL_NODE:
        {
            pIGM->ChangeSelectionFilterCombo(EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM);
        }
        break;
    default:
        {
            ASSERT(0); return;
        }
    }
}

void CPostTensionDrawTendonLineSlabDlg::OnMethodLine2ComboChanged()
{
    // 콤보박스가 변경될 때 기존 연결 끊기
    m_edtMethodLine1.Disconnect();
    m_edtMethodLine2.Disconnect();

    UpdateData(TRUE);
}

void CPostTensionDrawTendonLineSlabDlg::OnLineTypeRadioClicked()
{
    UpdateData(TRUE);
}

void CPostTensionDrawTendonLineSlabDlg::OnDrawRadioClicked()
{
    UpdateData(TRUE);

    enum
    {
        EN_DRAW_SEL_SPTL = 0,
        EN_DRAW_SEL_NODE,
    };

    I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();

    switch (m_nSelectDraw)
    {
    case EN_DRAW_SEL_SPTL:
        {
            pIGM->ChangeSelectionFilterCombo(EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_SPTL);
        }
        break;
    case EN_DRAW_SEL_NODE:
        {
            pIGM->ChangeSelectionFilterCombo(EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM);
        }
        break;
    default:
        {
            ASSERT(0); return;
        }
    }

    SetNodeSelectCtrlCondition();
}

void CPostTensionDrawTendonLineSlabDlg::InitSelectionFilter()
{
    I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
    pIGM->ChangeSelectionFilterCombo(EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM);
}

void CPostTensionDrawTendonLineSlabDlg::InitCtrl()
{
    OnDrawRadioClicked();
    OnArrangementRadioClicked();
}

void CPostTensionDrawTendonLineSlabDlg::SetNodeSelectCtrlCondition()
{
    UpdateData(TRUE);

    enum
    {
        EN_DRAW_SEL_SPTL = 0,
        EN_DRAW_SEL_NODE,
    };

    switch (m_nSelectDraw)
    {
    case EN_DRAW_SEL_SPTL:
        {
            m_edtDrawNodes.EnableWindow(FALSE);
            m_edtDrawNodes.Disconnect();
        }
        break;
    case EN_DRAW_SEL_NODE:
        {
            m_edtDrawNodes.EnableWindow(TRUE);
            m_edtDrawNodes.Connect(SC_ID_NODE, &m_edtDrawNodes);
        }
        break;
    default:
        {
            ASSERT(0); return;
        }
    }

    UpdateData(FALSE);
}

void CPostTensionDrawTendonLineSlabDlg::GetNodeInfoBySupportLine(CArray<UINT, UINT>& aNodeK)
{
    T_SPTL_K_LIST aSptlK;
    GetSelectedSupportLineKeyList(aSptlK);

    T_SPTL_D SptlD;
    for (auto i = 0; i < aSptlK.GetSize(); i++)
    {
        m_pDoc->m_pAttrCtrl2->GetSptl(aSptlK[i], SptlD);
    }

	CArray<UINT, UINT> arNodeK;
    arNodeK.Copy(SptlD.aNodeK);

    // Sort by Direction
	aNodeK.RemoveAll();
	m_pDoc->m_pAttrCtrl2->GetTdslNodeKeyByNode(m_nDirection, arNodeK, aNodeK);
}

void CPostTensionDrawTendonLineSlabDlg::GetNodeInfoBySupportingNodes(CString strNodes, CArray<UINT, UINT>& aNodeK)
{
    if (strNodes.IsEmpty()) return;

	CArray<int, int> aInNodeK;

    if (!CStrParser::ParsingListByTo(strNodes, aInNodeK)) return;

	CArray<UINT, UINT> arNodeK;
	for (int i = 0; i < aInNodeK.GetSize(); ++i)
	{
		int NodeK = static_cast<UINT>(aInNodeK[i]);
        arNodeK.Add(NodeK);
	}
	/// Sort by Direction
	aNodeK.RemoveAll();
    m_pDoc->m_pAttrCtrl2->GetTdslNodeKeyByNode(m_nDirection, arNodeK, aNodeK);
}

void CPostTensionDrawTendonLineSlabDlg::GetSelectedDesignStripKeyList(CArray<T_DSTR_K, T_DSTR_K>& aKey)
{
    I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
    if (!pIGM)
    {
        ASSERT(0); return;
    }

    CArray<T_DSTR_K, T_DSTR_K> aDstrK;
    m_pDoc->m_pAttrCtrl2->GetDstrKeyList(aDstrK);

    CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
    ASSERT(pServiceHeadOffice);

    for (auto i = 0; i < aDstrK.GetSize(); i++)
    {
        if (pServiceHeadOffice->ReqService(_ULS(gr), D_GR_GET_SELECTED_DSTR, (void*)(&aDstrK[i])))
        {
            aKey.Add(aDstrK[i]);
        }
    }
}

void CPostTensionDrawTendonLineSlabDlg::GetSelectedSupportLineKeyList(CArray<T_SPTL_K, T_SPTL_K>& aKey)
{
    aKey.RemoveAll();

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if (!pIGM)
	{
		ASSERT(0); return;
	}

	CArray<T_SPTL_K, T_SPTL_K> aSptlK;
	m_pDoc->m_pAttrCtrl2->GetSptlKeyList(aSptlK);

	CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
	ASSERT(pServiceHeadOffice);

	for (auto i = 0; i < aSptlK.GetSize(); i++)
	{
		if (pServiceHeadOffice->ReqService(_ULS(gr), D_GR_GET_SELECTED_SPTL, (void*)(&aSptlK[i])))
		{
			aKey.Add(aSptlK[i]);
		}
	}
}

void CPostTensionDrawTendonLineSlabDlg::SetNodeInfoByDesignStrip(T_NODE_K NodeIK, T_NODE_K NodeJK)
{
	m_Data.aNodeK.RemoveAll();

    m_Data.aNodeK.Add(NodeIK);
    m_Data.aNodeK.Add(NodeJK);
}

void CPostTensionDrawTendonLineSlabDlg::Execute4SupLine()
{
	T_SPTL_K_LIST aSptlK;
	GetSelectedSupportLineKeyList(aSptlK);

    CArray<T_PTTN_D, T_PTTN_D&> aPttnDTotal;
    CArray<T_PTTN_D, T_PTTN_D&> aPttnD;
	T_SPTL_D SptlD;
	for (auto i = 0; i < aSptlK.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl2->GetSptl(aSptlK[i], SptlD);

		CArray<UINT, UINT> arNodeK;
		arNodeK.Copy(SptlD.aNodeK);

		// Sort by Direction
		CArray<UINT, UINT> aNodeK;
		aNodeK.RemoveAll();
		m_pDoc->m_pAttrCtrl2->GetTdslNodeKeyByNode(m_nDirection, arNodeK, aNodeK);
		m_Data.aNodeK.RemoveAll();
		m_Data.aNodeK.Copy(aNodeK);

        aPttnD.RemoveAll();
        m_pDoc->m_pAttrCtrl->MakePttnFromTdsl(m_Data, aPttnD);

        aPttnDTotal.Append(aPttnD);
	}

	m_pDoc->m_pDataCtrl->AddPttn(aPttnDTotal);
}

void CPostTensionDrawTendonLineSlabDlg::Execute4SupLine_Distributed()
{
	T_SPTL_K_LIST aSptlK;
	GetSelectedSupportLineKeyList(aSptlK);

    if (aSptlK.GetSize() != 2) { ASSERT(0); return; }

    T_SPTL_D SptlD;
	CArray<UINT, UINT> arNodeK;
	CArray<UINT, UINT> aNodeK;

	m_pDoc->m_pAttrCtrl2->GetSptl(aSptlK[0], SptlD);
	arNodeK.RemoveAll();
	aNodeK.RemoveAll();
	arNodeK.Copy(SptlD.aNodeK);
	m_pDoc->m_pAttrCtrl2->GetTdslNodeKeyByNode(m_nDirection, arNodeK, aNodeK);
	m_Data.aLine1.RemoveAll();
	m_Data.aLine1.Copy(aNodeK);

	m_pDoc->m_pAttrCtrl2->GetSptl(aSptlK[1], SptlD);
	arNodeK.RemoveAll();
	aNodeK.RemoveAll();
	arNodeK.Copy(SptlD.aNodeK);
	m_pDoc->m_pAttrCtrl2->GetTdslNodeKeyByNode(m_nDirection, arNodeK, aNodeK);
	m_Data.aLine2.RemoveAll();
	m_Data.aLine2.Copy(aNodeK);

	CArray<T_PTTN_D, T_PTTN_D&> aPttnDTotal;
    m_pDoc->m_pAttrCtrl->MakePttnFromTdsl_Distributed(m_Data, aPttnDTotal);

	m_pDoc->m_pDataCtrl->AddPttn(aPttnDTotal);

}

void CPostTensionDrawTendonLineSlabDlg::Execute4SupNode()
{
    CArray<T_PTTN_D, T_PTTN_D&> aPttnDTotal;
    m_pDoc->m_pAttrCtrl->MakePttnFromTdsl(m_Data, aPttnDTotal);

	m_pDoc->m_pDataCtrl->AddPttn(aPttnDTotal);
}

T_NODE_K CPostTensionDrawTendonLineSlabDlg::FindNearestNodeAtSameHeight(const T_NODE_D& targetNode, double zTolerance, double distanceThreshold)
{
    // 전체 Node 리스트 가져오기
    std::vector<T_NODE_K> allNodeKeys;
    m_pDoc->m_pAttrCtrl->GetNodeKeyList(allNodeKeys);

    if (allNodeKeys.empty())
        return 0;

    T_NODE_K nearestNodeK = 0;
    double minDistance = DBL_MAX;
    BOOL bFoundSameHeight = FALSE;

    // 1단계: 같은 z 좌표(허용오차 이내)를 가진 Node들 중에서 가장 가까운 Node 찾기
    for (size_t i = 0; i < allNodeKeys.size(); i++)
    {
        T_NODE_D nodeD;
        if (!m_pDoc->m_pAttrCtrl->GetNode(allNodeKeys[i], nodeD))
            continue;

        // z 좌표 차이 확인
        double dz = fabs(nodeD.z - targetNode.z);
        if (dz <= zTolerance)
        {
            // 같은 높이에 있는 Node - XY 평면상의 거리 계산
            double dx = nodeD.x - targetNode.x;
            double dy = nodeD.y - targetNode.y;
            double distance = sqrt(dx * dx + dy * dy);

            if (distance < minDistance)
            {
                minDistance = distance;
                nearestNodeK = allNodeKeys[i];
                bFoundSameHeight = TRUE;
            }
        }
    }

    // 같은 높이의 Node를 찾았고 거리 임계값 이내이면 반환
    if (bFoundSameHeight)
    {
        if (minDistance <= distanceThreshold)
            return nearestNodeK;
        else
            return 0;  // 거리 임계값 초과
    }

    // 2단계: 같은 z 좌표의 Node가 없으면 3D 거리상 가장 가까운 Node 찾기
    minDistance = DBL_MAX;
    nearestNodeK = 0;

    for (size_t i = 0; i < allNodeKeys.size(); i++)
    {
        T_NODE_D nodeD;
        if (!m_pDoc->m_pAttrCtrl->GetNode(allNodeKeys[i], nodeD))
            continue;

        // 3D 거리 계산
        double dx = nodeD.x - targetNode.x;
        double dy = nodeD.y - targetNode.y;
        double dz = nodeD.z - targetNode.z;
        double distance = sqrt(dx * dx + dy * dy + dz * dz);

        if (distance < minDistance)
        {
            minDistance = distance;
            nearestNodeK = allNodeKeys[i];
        }
    }

    // 거리 임계값 확인
    if (minDistance <= distanceThreshold)
        return nearestNodeK;
    else
        return 0;  // 거리 임계값 초과
}

void CPostTensionDrawTendonLineSlabDlg::Execute4SupNode_Distributed()
{
    // aLine1과 aLine2가 모두 있는지 확인
    if (m_Data.aLine1.GetSize() < 2) { ASSERT(0);  return; }
    if (m_Data.aLine2.GetSize() < 2) { ASSERT(0);  return; }
    if (m_Data.aLine1.GetSize() != m_Data.aLine2.GetSize()) { ASSERT(0);  return; }

    // 각 Line을 방향별로 정렬
    CArray<UINT, UINT> aSortedLine1, aSortedLine2;
    aSortedLine1.RemoveAll();
    aSortedLine2.RemoveAll();

    m_pDoc->m_pAttrCtrl2->GetTdslNodeKeyByNode(m_nDirection, m_Data.aLine1, aSortedLine1);
    m_pDoc->m_pAttrCtrl2->GetTdslNodeKeyByNode(m_nDirection, m_Data.aLine2, aSortedLine2);

    m_Data.aLine1.Copy(aSortedLine1);
    m_Data.aLine2.Copy(aSortedLine2);

	CArray<T_PTTN_D, T_PTTN_D&> aPttnDTotal;
  	m_pDoc->m_pAttrCtrl->MakePttnFromTdsl_Distributed(m_Data, aPttnDTotal);

    m_pDoc->m_pDataCtrl->AddPttn(aPttnDTotal);
}

void CPostTensionDrawTendonLineSlabDlg::UpdateTendonPropertyList()
{
    m_cmbTendonProp.ResetContent();

    CArray<T_TDNT_K, T_TDNT_K> aTdntK;
    m_pDoc->m_pAttrCtrl->GetTdntKeyList(aTdntK);
    T_TDNT_D TdntD;
    for (auto i = 0; i < aTdntK.GetSize(); i++)
    {
        if (!m_pDoc->m_pAttrCtrl->GetTdnt(aTdntK[i], TdntD))
        {
            ASSERT(0); return;
        }
        m_cmbTendonProp.AddString(TdntD.TendonTypeName);
    }

    m_cmbTendonProp.SetCurSel(0);
}

void CPostTensionDrawTendonLineSlabDlg::UpdateTendonProfileList()
{
	m_cmbTendonProf.ResetContent();

    CArray<T_TDPF_K, T_TDPF_K> aTdpfK;
    m_pDoc->m_pAttrCtrl2->GetTdpfKeyList(aTdpfK);
    T_TDPF_D TdpfD;
    for (auto i = 0; i < aTdpfK.GetSize(); i++)
    {
        if (!m_pDoc->m_pAttrCtrl2->GetTdpf(aTdpfK[i], TdpfD))
        {
            ASSERT(0); return;
        }
        if(TdpfD.nElemType != 1) continue;
        m_cmbTendonProf.AddString(TdpfD.strName);
    }

    m_cmbTendonProf.SetCurSel(0);
}

void CPostTensionDrawTendonLineSlabDlg::UpdateTendonStressingList()
{
    m_cmbTendonStrs.ResetContent();

    CArray<T_TDST_K, T_TDST_K> aTdstK;
    m_pDoc->m_pAttrCtrl2->GetTdstKeyList(aTdstK);
    T_TDST_D TdstD;
    for (auto i = 0; i < aTdstK.GetSize(); i++)
    {
        if (!m_pDoc->m_pAttrCtrl2->GetTdst(aTdstK[i], TdstD))
        {
            ASSERT(0); return;
        }
        m_cmbTendonStrs.AddString(TdstD.strName);
    }

    m_cmbTendonStrs.SetCurSel(0);
}

void CPostTensionDrawTendonLineSlabDlg::OnSetFocusMethodLine1Edit()
{
    enum
    {
        EN_DRAW_SEL_SPTL = 0,
        EN_DRAW_SEL_NODE,
    };

    m_edtMethodLine2.Disconnect();

    // I_GENModelBase 인스턴스 얻기
    I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();

    // METHOD_LINE1 Edit에 포커스가 갔을 때 Connect
    int nCurSel = m_cmbMethodLine1.GetCurSel();

    switch (nCurSel)
    {
    case EN_DRAW_SEL_SPTL:
        {
            pIGM->UnselectAllElem();  // Element 선택 초기화
            m_edtMethodLine1.Connect(SC_ID_ELEM, &m_edtMethodLine1);
        }
        break;
    case EN_DRAW_SEL_NODE:
        {
            pIGM->UnselectAllNode();  // Node 선택 초기화
            m_edtMethodLine1.Connect(SC_ID_NODE, &m_edtMethodLine1);
        }
        break;
    default:
        ASSERT(0);
        break;
    }

}

void CPostTensionDrawTendonLineSlabDlg::OnKillFocusMethodLine1Edit()
{
    // METHOD_LINE1 Edit에서 포커스를 잃었을 때 Disconnect
    m_edtMethodLine1.Disconnect();
}

void CPostTensionDrawTendonLineSlabDlg::OnSetFocusMethodLine2Edit()
{
    enum
    {
        EN_DRAW_SEL_SPTL = 0,
        EN_DRAW_SEL_NODE,
    };

    m_edtMethodLine1.Disconnect();

    // I_GENModelBase 인스턴스 얻기
    I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();

    // METHOD_LINE2 Edit에 포커스가 갔을 때 Connect
    int nCurSel = m_cmbMethodLine2.GetCurSel();

    switch (nCurSel)
    {
    case EN_DRAW_SEL_SPTL:
        {
            pIGM->UnselectAllElem();  // Element 선택 초기화
            m_edtMethodLine2.Connect(SC_ID_ELEM, &m_edtMethodLine2);
        }
        break;
    case EN_DRAW_SEL_NODE:
        {
            pIGM->UnselectAllNode();  // Node 선택 초기화
            m_edtMethodLine2.Connect(SC_ID_NODE, &m_edtMethodLine2);
        }
        break;
    default:
        ASSERT(0);
        break;
    }
}

void CPostTensionDrawTendonLineSlabDlg::OnKillFocusMethodLine2Edit()
{
    // METHOD_LINE2 Edit에서 포커스를 잃었을 때 Disconnect
    m_edtMethodLine2.Disconnect();
}

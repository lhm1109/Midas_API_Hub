#include "stdafx.h"
#include "wg_treemenu.h"
#include "DesignStripModifySectionDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_GTF.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\ServiceProvider.h"

#include "..\wg_gr\Gr_ServiceDef.h"
#include "..\wg_db\DB_ST_DN_DESIGN.H"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDesignStripModifySectionDlg::CDesignStripModifySectionDlg(CWnd* pParent)
    : CMenuBarChildDlg(CDesignStripModifySectionDlg::IDD, pParent)
{
    m_pDoc = CDBDoc::GetDocPoint();
    m_nSelectedDstrK = 0;
    m_nSelectedSpanIdx = -1;
    m_nSelectedSectionIdx = -1;

    m_pEditCtrl = NULL;
    m_nEditItem = -1;
    m_nEditSubItem = -1;
}

CDesignStripModifySectionDlg::~CDesignStripModifySectionDlg()
{
    // Clean up in-place edit control
    if (m_pEditCtrl != NULL)
    {
        m_pEditCtrl->DestroyWindow();
        delete m_pEditCtrl;
        m_pEditCtrl = NULL;
    }

    // Clear Design Section highlight
    ClearDesignSectionHighlight();

    ChangeSelectionFilter(EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM);
}

void CDesignStripModifySectionDlg::DoDataExchange(CDataExchange* pDX)
{
    CMenuBarChildDlg::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TM_MOD_DS_SECT_DESIGNSTRIP_LIST, m_wndDesignStripList);
    DDX_Control(pDX, IDC_TM_MOD_DS_SECT_SPAN_LIST, m_wndSpanList);
    DDX_Control(pDX, IDC_TM_MOD_DS_SECT_DESIGNSECTION_LIST, m_wndDesignSectionList);
}

BEGIN_MESSAGE_MAP(CDesignStripModifySectionDlg, CMenuBarChildDlg)
    ON_NOTIFY(NM_CLICK, IDC_TM_MOD_DS_SECT_DESIGNSTRIP_LIST, OnClickDesignStripList)
    ON_NOTIFY(NM_CLICK, IDC_TM_MOD_DS_SECT_SPAN_LIST, OnClickSpanList)
    ON_NOTIFY(NM_CLICK, IDC_TM_MOD_DS_SECT_DESIGNSECTION_LIST, OnClickDesignSectionList)
    ON_NOTIFY(NM_DBLCLK, IDC_TM_MOD_DS_SECT_DESIGNSECTION_LIST, OnDblClickDesignSectionList)
    ON_EN_KILLFOCUS(IDC_TM_MOD_DS_SECT_INPLACE_EDIT, OnKillFocusEdit)
END_MESSAGE_MAP()

BOOL CDesignStripModifySectionDlg::OnInitDialog()
{
    CMenuBarChildDlg::OnInitDialog();

    InitCtrl();
    InitListCtrlHeaders();

    m_nSelectedDstrK = 0;
    m_nSelectedSpanIdx = -1;
    m_nSelectedSectionIdx = -1;

    InitSelectionFilter();

    UpdateDesignStripList();

    return TRUE;
}

BOOL CDesignStripModifySectionDlg::PreTranslateMessage(MSG* pMsg)
{
    if (m_pEditCtrl != NULL && m_pEditCtrl->GetSafeHwnd() != NULL)
    {
        if (pMsg->message == WM_KEYDOWN)
        {
            switch (pMsg->wParam)
            {
            case VK_RETURN:
            case VK_TAB:
                EndEdit(TRUE);
                return TRUE;
            case VK_UP:
                {
                    int nPrevItem = m_nEditItem - 1;
                    EndEdit(TRUE);
                    if (nPrevItem >= 0)
                    {
                        m_wndDesignSectionList.SetItemState(nPrevItem,
                            LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
                        m_wndDesignSectionList.EnsureVisible(nPrevItem, FALSE);
                    }
                }
                return TRUE;
            case VK_DOWN:
                {
                    int nNextItem = m_nEditItem + 1;
                    int nItemCount = m_wndDesignSectionList.GetItemCount();
                    EndEdit(TRUE);
                    if (nNextItem < nItemCount)
                    {
                        m_wndDesignSectionList.SetItemState(nNextItem,
                            LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
                        m_wndDesignSectionList.EnsureVisible(nNextItem, FALSE);
                    }
                }
                return TRUE;
            case VK_ESCAPE:
                EndEdit(FALSE);
                return TRUE;
            }
        }
    }

    return CMenuBarChildDlg::PreTranslateMessage(pMsg);
}

void CDesignStripModifySectionDlg::OnKillFocusEdit()
{
    EndEdit(TRUE);  // 포커스 이탈 시 저장하고 종료
}

void CDesignStripModifySectionDlg::InitCtrl()
{
    // Extended styles for list controls
    DWORD dwExStyle = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
    m_wndDesignStripList.SetExtendedStyle(dwExStyle);
    m_wndSpanList.SetExtendedStyle(dwExStyle);
    m_wndDesignSectionList.SetExtendedStyle(dwExStyle);
}

void CDesignStripModifySectionDlg::InitListCtrlHeaders()
{
    CStringArray HTitles;
    CArray<float, float> HRatio;

    // Design Strip List Header
    HTitles.RemoveAll();
    HRatio.RemoveAll();
    HTitles.Add(_T("No."));
    HTitles.Add(_T("Name"));
    HRatio.Add(0.3f);
    HRatio.Add(0.7f);
    CDlgUtil::_SetListCtrlHeader(&m_wndDesignStripList, HTitles, &HRatio, NULL);

    // Span List Header
    HTitles.RemoveAll();
    HRatio.RemoveAll();
    HTitles.Add(_T("No."));
    HTitles.Add(_T("T(L)i"));
    HTitles.Add(_T("B(R)i"));
    HTitles.Add(_T("T(L)j"));
    HTitles.Add(_T("B(R)j"));
    HTitles.Add(_T("Design Point No."));
    HRatio.Add(0.15f);
    HRatio.Add(0.20f);
    HRatio.Add(0.20f);
    HRatio.Add(0.20f);
    HRatio.Add(0.20f);
    HRatio.Add(0.50f);
    CDlgUtil::_SetListCtrlHeader(&m_wndSpanList, HTitles, &HRatio, NULL);

    // Design Section List Header
    HTitles.RemoveAll();
    HRatio.RemoveAll();
    HTitles.Add(_T("No."));
    HTitles.Add(_T("Position"));
    HTitles.Add(_T("Left"));
    HTitles.Add(_T("Right"));
    HRatio.Add(0.15f);
    HRatio.Add(0.35f);
    HRatio.Add(0.25f);
    HRatio.Add(0.25f);
    CDlgUtil::_SetListCtrlHeader(&m_wndDesignSectionList, HTitles, &HRatio, NULL);
}

void CDesignStripModifySectionDlg::UpdateDesignStripList()
{
    m_wndDesignStripList.DeleteAllItems();

    CArray<T_DSTR_K, T_DSTR_K> aDstrK;
    m_pDoc->m_pAttrCtrl2->GetDstrKeyList(aDstrK);

    CStringArray Contents;
    CString strTemp;

    for (int i = 0; i < aDstrK.GetSize(); i++)
    {
        T_DSTR_D DstrD;
        if (m_pDoc->m_pAttrCtrl2->GetDstr(aDstrK[i], DstrD))
        {
            Contents.RemoveAll();

            // No.
            strTemp.Format(_T("%d"), aDstrK[i]);
            Contents.Add(strTemp);

            // Name (using Support Line Key as name for now)
            strTemp.Format(_T("Design Strip-%d"), aDstrK[i]);
            Contents.Add(strTemp);

            CDlgUtil::SetListItem(&m_wndDesignStripList, i, Contents, (DWORD)aDstrK[i]);
        }
    }

    // Clear dependent lists
    m_wndSpanList.DeleteAllItems();
    m_wndDesignSectionList.DeleteAllItems();
    m_nSelectedSpanIdx = 0;
    m_nSelectedSectionIdx = -1;
}

void CDesignStripModifySectionDlg::UpdateSpanList()
{
    m_wndSpanList.DeleteAllItems();

    if (m_nSelectedDstrK == 0) return;

    T_DSTR_D DstrD;
    if (!m_pDoc->m_pAttrCtrl2->GetDstr(m_nSelectedDstrK, DstrD))
    {
        return;
    }

    CStringArray Contents;
    CString strTemp;

    for (int i = 0; i < DstrD.aSpan.GetSize(); i++)
    {
        Contents.RemoveAll();

        // No.
        strTemp.Format(_T("%d"), DstrD.aSpan[i].nSpanNo);
        Contents.Add(strTemp);

		// Width Start TL
        strTemp.Format(_T("%.2f"), DstrD.aSpan[i].dWidth_StartTL);
        Contents.Add(strTemp);

        // Width Start BR
        strTemp.Format(_T("%.2f"), DstrD.aSpan[i].dWidth_StartBR);
        Contents.Add(strTemp);

        // Width End TL
        strTemp.Format(_T("%.2f"), DstrD.aSpan[i].dWidth_EndTL);
        Contents.Add(strTemp);

        // Width End BR
        strTemp.Format(_T("%.2f"), DstrD.aSpan[i].dWidth_EndBR);
        Contents.Add(strTemp);

        // Design Point No.
        strTemp.Format(_T("%d"), DstrD.aSpan[i].DgnPointNo);
        Contents.Add(strTemp);

        CDlgUtil::SetListItem(&m_wndSpanList, i, Contents, (DWORD)i);
    }

    m_wndDesignSectionList.DeleteAllItems();
    m_nSelectedSectionIdx = 0;
}

void CDesignStripModifySectionDlg::UpdateDesignSectionList()
{
    m_wndDesignSectionList.DeleteAllItems();

    if (m_nSelectedDstrK == 0 || m_nSelectedSpanIdx < 0) return;

    T_DSTR_D DstrD;
    if (!m_pDoc->m_pAttrCtrl2->GetDstr(m_nSelectedDstrK, DstrD))
    {
        return;
    }

    if (m_nSelectedSpanIdx >= DstrD.aSpan.GetSize()) return;

    const T_DGNSTRIP_SPAN_INFO& SpanInfo = DstrD.aSpan[m_nSelectedSpanIdx];

    CStringArray Contents;
    CString strTemp;

    // Display design sections based on DgnPointNo
    T_DSPS_D DspsD;
    const int nSize = SpanInfo.aDspsK.GetSize();
    for (int i = 0; i < nSize; i++)
    {
        Contents.RemoveAll();

        // No.
        strTemp.Format(_T("%d"), i + 1);
        Contents.Add(strTemp);

        T_DSPS_K DspsK = SpanInfo.aDspsK[i];
        if (!m_pDoc->m_pAttrCtrl2->GetDsps(DspsK, DspsD))
        {
            ASSERT(0); return;
        }

        // Position
        strTemp.Format(_T("%.3f"), DspsD.dDist);
        Contents.Add(strTemp);

        // Left Width
        strTemp.Format(_T("%.2f"), DspsD.dLeft);
        Contents.Add(strTemp);

        // Right Width
        strTemp.Format(_T("%.2f"), DspsD.dRight);
        Contents.Add(strTemp);

        CDlgUtil::SetListItem(&m_wndDesignSectionList, i, Contents, (DWORD)i);
    }
}

void CDesignStripModifySectionDlg::OnDesignStripSelectionChanged()
{
    // 수정사항 확인
    if (!CheckUnsavedChanges())
    {
        return;
    }

    // 편집 중인 Edit 컨트롤 종료
    EndEdit(FALSE);

    POSITION pos = m_wndDesignStripList.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        m_nSelectedDstrK = 0;
        m_wndSpanList.DeleteAllItems();
        m_wndDesignSectionList.DeleteAllItems();
        NotifyDesignSectionHighlight();
        return;
    }

    int nItem = m_wndDesignStripList.GetNextSelectedItem(pos);
    m_nSelectedDstrK = (T_DSTR_K)m_wndDesignStripList.GetItemData(nItem);

    UpdateSpanList();
    NotifyDesignSectionHighlight();
}

void CDesignStripModifySectionDlg::OnSpanSelectionChanged()
{
    // 수정사항 확인
    if (!CheckUnsavedChanges())
    {
        // 이전 Span 선택 복원
        if (m_nSelectedSpanIdx >= 0)
        {
            m_wndSpanList.SetItemState(m_nSelectedSpanIdx,
                LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
        }
        return;
    }

    // 편집 중인 Edit 컨트롤 종료
    EndEdit(FALSE);

    POSITION pos = m_wndSpanList.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        m_nSelectedSpanIdx = -1;
        m_wndDesignSectionList.DeleteAllItems();
        NotifyDesignSectionHighlight();
        return;
    }

    int nItem = m_wndSpanList.GetNextSelectedItem(pos);
    m_nSelectedSpanIdx = (int)m_wndSpanList.GetItemData(nItem);

    UpdateDesignSectionList();
    NotifyDesignSectionHighlight();
}

void CDesignStripModifySectionDlg::OnDesignSectionSelectionChanged()
{
    POSITION pos = m_wndDesignSectionList.GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        m_nSelectedSectionIdx = -1;
        NotifyDesignSectionHighlight();
        return;
    }

    int nItem = m_wndDesignSectionList.GetNextSelectedItem(pos);
    m_nSelectedSectionIdx = (int)m_wndDesignSectionList.GetItemData(nItem);

    NotifyDesignSectionHighlight();
}

void CDesignStripModifySectionDlg::OnClickDesignStripList(NMHDR* pNMHDR, LRESULT* pResult)
{
    OnDesignStripSelectionChanged();
    *pResult = 0;
}

void CDesignStripModifySectionDlg::OnClickSpanList(NMHDR* pNMHDR, LRESULT* pResult)
{
    OnSpanSelectionChanged();
    *pResult = 0;
}

void CDesignStripModifySectionDlg::OnClickDesignSectionList(NMHDR* pNMHDR, LRESULT* pResult)
{
    EndEdit(TRUE);  // 편집 중이면 저장하고 종료
    OnDesignSectionSelectionChanged();
    *pResult = 0;
}

void CDesignStripModifySectionDlg::OnDblClickDesignSectionList(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMITEMACTIVATE* pNMItem = (NMITEMACTIVATE*)pNMHDR;
    int nItem = pNMItem->iItem;
    int nSubItem = pNMItem->iSubItem;

    // Position(1), Left(2), Right(3) 컬럼만 편집 허용
    if (nItem < 0 || nSubItem < 1 || nSubItem > 3)
    {
        *pResult = 0;
        return;
    }

    StartEdit(nItem, nSubItem);
    *pResult = 0;
}

void CDesignStripModifySectionDlg::StartEdit(int nItem, int nSubItem)
{
    // 기존 편집 종료
    EndEdit();

    // 서브아이템 위치 획득
    CRect rect;
    m_wndDesignSectionList.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);

    // Edit 컨트롤 생성
    m_pEditCtrl = new CEdit();
    m_pEditCtrl->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                        rect, &m_wndDesignSectionList, IDC_TM_MOD_DS_SECT_INPLACE_EDIT);

    // 폰트 설정
    m_pEditCtrl->SetFont(m_wndDesignSectionList.GetFont());

    // 현재 값 설정
    CString strValue = m_wndDesignSectionList.GetItemText(nItem, nSubItem);
    m_pEditCtrl->SetWindowText(strValue);
    m_pEditCtrl->SetSel(0, -1);
    m_pEditCtrl->SetFocus();

    m_nEditItem = nItem;
    m_nEditSubItem = nSubItem;
}

void CDesignStripModifySectionDlg::EndEdit(BOOL bSave)
{
    if (m_pEditCtrl == NULL) return;

    if (bSave && m_nEditItem >= 0 && m_nEditSubItem >= 0)
    {
        // 입력값 획득
        CString strValue;
        m_pEditCtrl->GetWindowText(strValue);

        // 리스트에 반영
        m_wndDesignSectionList.SetItemText(m_nEditItem, m_nEditSubItem, strValue);

        // 수정 데이터 저장
        SaveModifiedData(m_nEditItem, m_nEditSubItem, strValue);
    }

    // Edit 컨트롤 삭제
    m_pEditCtrl->DestroyWindow();
    delete m_pEditCtrl;
    m_pEditCtrl = NULL;
    m_nEditItem = -1;
    m_nEditSubItem = -1;
}

void CDesignStripModifySectionDlg::SaveModifiedData(int nItem, int nSubItem, const CString& strValue)
{
    // 현재 선택된 Design Section의 DSPS Key 획득
    T_DSTR_D DstrD;
    if (!m_pDoc->m_pAttrCtrl2->GetDstr(m_nSelectedDstrK, DstrD)) return;
    if (m_nSelectedSpanIdx >= DstrD.aSpan.GetSize()) return;

    const T_DGNSTRIP_SPAN_INFO& SpanInfo = DstrD.aSpan[m_nSelectedSpanIdx];
    if (nItem >= SpanInfo.aDspsK.GetSize()) return;

    T_DSPS_K DspsK = SpanInfo.aDspsK[nItem];

    // 이미 수정된 데이터가 있는지 확인
    T_DSPS_D DspsD;
    BOOL bFound = FALSE;
    for (int i = 0; i < m_aModifiedData.GetSize(); i++)
    {
        if (m_aModifiedData[i].DspsK == DspsK)
        {
            DspsD = m_aModifiedData[i].DspsD;  // 기존 수정 데이터 사용
            bFound = TRUE;
            break;
        }
    }

    if (!bFound)
    {
        // 수정된 적 없으면 DB에서 조회
        m_pDoc->m_pAttrCtrl2->GetDsps(DspsK, DspsD);
    }

    // 값 수정
    double dValue = _ttof(strValue);
    switch (nSubItem)
    {
    case 1: DspsD.dDist = dValue; break;   // Position
    case 2: DspsD.dLeft = dValue; break;   // Left
    case 3: DspsD.dRight = dValue; break;  // Right
    }

    // 수정 목록에 추가/업데이트
    AddOrUpdateModifiedData(DspsK, DspsD);
}

void CDesignStripModifySectionDlg::AddOrUpdateModifiedData(T_DSPS_K DspsK, const T_DSPS_D& DspsD)
{
    // 이미 수정 목록에 있는지 확인
    for (int i = 0; i < m_aModifiedData.GetSize(); i++)
    {
        if (m_aModifiedData[i].DspsK == DspsK)
        {
            // 같은 Design Section → 데이터 업데이트
            m_aModifiedData[i].DspsD = DspsD;
            return;
        }
    }

    // 새로운 Design Section → 추가
    MODIFIED_DSPS_DATA newData;
    newData.DspsK = DspsK;
    newData.DspsD = DspsD;
    newData.bModified = TRUE;
    m_aModifiedData.Add(newData);
}

BOOL CDesignStripModifySectionDlg::CheckUnsavedChanges()
{
    if (m_aModifiedData.GetSize() == 0)
        return TRUE;

    int nResult = AfxMessageBox(
        _T("There are unsaved changes.\nDo you want to discard them?"),
        MB_YESNO | MB_ICONWARNING);

    if (nResult == IDNO)
        return FALSE;

    // 수정사항 버림
    m_aModifiedData.RemoveAll();
    return TRUE;
}

void CDesignStripModifySectionDlg::Dlg2Data()
{
    // 편집 중인 Edit 컨트롤 종료
    EndEdit();

    // 수정된 데이터를 DB에 반영
    for (int i = 0; i < m_aModifiedData.GetSize(); i++)
    {
        if (m_aModifiedData[i].bModified)
        {
            T_DSPS_K Key = m_aModifiedData[i].DspsK;
            T_DSPS_D Data = m_aModifiedData[i].DspsD;

            m_pDoc->m_pDataCtrl->ModifyDsps(Key, Key, Data);
        }
    }

    // 수정 목록 초기화
    m_aModifiedData.RemoveAll();
}

void CDesignStripModifySectionDlg::Data2Dlg()
{
    // Transfer data to dialog controls
    UpdateDesignStripList();
}

void CDesignStripModifySectionDlg::InitSelectionFilter()
{
    I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
    pIGM->ChangeSelectionFilterCombo(EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_DSTR);
}

void CDesignStripModifySectionDlg::ChangeSelectionFilter(EN_SELECT_ELEM_FILTER enFilter)
{
    I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
    pIGM->ChangeSelectionFilterCombo(enFilter);
}

void CDesignStripModifySectionDlg::Execute()
{
    UpdateData(TRUE);
    Dlg2Data();
    Data2Dlg();
}

void CDesignStripModifySectionDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    HWND hWnd = GetSafeHwnd();
    if (!hWnd || !IsWindow(hWnd)) return;

    switch (lHint)
    {
    case D_UPDATE_DBALL:
    case D_UPDATE_DEFAULT:
        UpdateDesignStripList();
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
    default:
        break;
    }
}

void CDesignStripModifySectionDlg::UpdateBuffer()
{
    CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
    int nCount = pViewBuff->GetCount();
    if (nCount == 0) return;
    ASSERT(nCount == 1);

    T_UDRD_BUFFER buffer_ur;
    POSITION pos = pViewBuff->GetStartBuffer();

    BOOL bDstrChanged = FALSE;
    while (pos != NULL)
    {
        buffer_ur = pViewBuff->GetNextBuffer(pos);
        int nCmd = buffer_ur.nCmd;

        switch (nCmd)
        {
        case(UR_DSTR_ADD):
        case(UR_DSTR_DEL):
        case(UR_DSTR_MFD):
        case(UR_DSTR_MFS):
            bDstrChanged = TRUE;
            break;
        default:
            break;
        }
    }

    if (bDstrChanged)
    {
        UpdateDesignStripList();
    }
}

void CDesignStripModifySectionDlg::NotifyDesignSectionHighlight()
{
    T_DSPS_HIGHLIGHT_INFO info;
    info.DstrK = m_nSelectedDstrK;
    info.nSpanIdx = m_nSelectedSpanIdx;
    info.DspsK = 0;

    // 실제 DSPS Key 가져오기
    if (m_nSelectedDstrK > 0 && m_nSelectedSpanIdx >= 0 && m_nSelectedSectionIdx >= 0)
    {
        T_DSTR_D DstrD;
        if (m_pDoc->m_pAttrCtrl2->GetDstr(m_nSelectedDstrK, DstrD))
        {
            if (m_nSelectedSpanIdx < DstrD.aSpan.GetSize())
            {
                const T_DGNSTRIP_SPAN_INFO& SpanInfo = DstrD.aSpan[m_nSelectedSpanIdx];
                if (m_nSelectedSectionIdx < SpanInfo.aDspsK.GetSize())
                {
                    info.DspsK = SpanInfo.aDspsK[m_nSelectedSectionIdx];
                }
            }
        }
    }

    CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
    pServiceHeadOffice->ReqService(_ULS(gr), D_GR_HIGHLIGHT_DESIGN_SECTION, &info);
}

void CDesignStripModifySectionDlg::ClearDesignSectionHighlight()
{
    CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
    pServiceHeadOffice->ReqService(_ULS(gr), D_GR_CLEAR_DESIGN_SECTION_HIGHLIGHT, nullptr);
}

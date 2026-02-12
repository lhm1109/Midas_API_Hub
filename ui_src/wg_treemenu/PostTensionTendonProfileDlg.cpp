#include "stdafx.h"
#include "wg_treemenu.h"
#include "PostTensionTendonProfileDlg.h"

#include "PostTensionTendonProfileMatchDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_base\I_GENStateDefine.h"

#ifndef ID_STRUCT_PTDN_TENDON_PROFILE
#define ID_STRUCT_PTDN_TENDON_PROFILE 0
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace mit::frx;

IMPLEMENT_DYNCREATE(CPostTensionTendonProfileDlg, MChildFormView)

CPostTensionTendonProfileDlg::CPostTensionTendonProfileDlg()
    : MChildFormView(CPostTensionTendonProfileDlg::IDD)
{
    m_ChildDlgs.RemoveAll();
    m_DlgIDs.RemoveAll();
    m_TableIDs.RemoveAll();

    // 메뉴 콤보 문자열 (프로필 관련 항목 정의 - 필요한 경우 확장)
    m_ComboMenuStr.Add(_LS(IDS_WG_TREEMENU_Match_Tendon_Profile)); // 단일 항목 (예시)

    m_nOrgSlabPlaceHolderHeight = 0;

    CreateChildDlgs();
}

CPostTensionTendonProfileDlg::~CPostTensionTendonProfileDlg()
{
    DeleteChildDlgs();
}

void CPostTensionTendonProfileDlg::DoDataExchange(CDataExchange* pDX)
{
    MChildFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TM_TENDON_PROFILE_TABLE_BTN, m_wndTableBtn);
    DDX_Control(pDX, IDC_TM_TENDON_PROFILE_PLACEHOLDER, m_wndPlaceHolder);
    DDX_Control(pDX, IDC_TENDON_PROFILE_MENU_COMBO, m_ComboMenu);
}

BEGIN_MESSAGE_MAP(CPostTensionTendonProfileDlg, MChildFormView)
    ON_CBN_SELCHANGE(IDC_TENDON_PROFILE_MENU_COMBO, OnComboMenuSelect)
    ON_BN_CLICKED(IDC_TM_TENDON_PROFILE_TABLE_BTN, OnTmTableBtn)
END_MESSAGE_MAP()

#ifdef _DEBUG
void CPostTensionTendonProfileDlg::AssertValid() const
{
    MChildFormView::AssertValid();
}

void CPostTensionTendonProfileDlg::Dump(CDumpContext& dc) const
{
    MChildFormView::Dump(dc);
}
#endif

void CPostTensionTendonProfileDlg::OnTmClose()
{
    if (CDBDoc::GetDocPoint())
    {
        CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
        CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
        CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
    }
    CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CPostTensionTendonProfileDlg::CreateChildDlgs()
{
    m_nCurChildDlg = -1;

    // 현재는 Match Profile 기능만 Child 로 추가. 향후 다른 프로필 편집/생성 기능 추가 가능
    m_ChildDlgs.Add(new CPostTensionTendonProfileMatchDlg());
    m_DlgIDs.Add(__TMMODE_PT_TENDON_MATCH_PROFILE__);
    m_TableIDs.Add(0); // No table command ID defined for profile yet
}

void CPostTensionTendonProfileDlg::DeleteChildDlgs()
{
    int nCount = m_ChildDlgs.GetSize();
    for (int i = 0; i < nCount; i++)
    {
        delete m_ChildDlgs[i];
    }
}

BOOL CPostTensionTendonProfileDlg::ShowChildDlgByID(int nID)
{
    int nIDCount = m_DlgIDs.GetSize();
    for (int i = 0; i < nIDCount; i++)
    {
        if (nID == m_DlgIDs[i])
        {
            ShowCurChildDlg(i);
            m_ComboMenu.SetCurSel(i);
            if (m_TableIDs[i] > 0) m_wndTableBtn.EnableWindow(TRUE); else m_wndTableBtn.EnableWindow(FALSE);
            return TRUE;
        }
    }
    return FALSE;
}

void CPostTensionTendonProfileDlg::ShowCurChildDlg(int nDlgIndex)
{
    if (m_nCurChildDlg == nDlgIndex)
        return;

    if (nDlgIndex > m_ChildDlgs.GetSize() - 1)
    {
        m_nCurChildDlg = nDlgIndex;
        return;
    }

    if (m_nCurChildDlg >= 0 && m_ChildDlgs.GetSize() - 1 >= m_nCurChildDlg)
        m_ChildDlgs[m_nCurChildDlg]->End();

    m_ChildDlgs[nDlgIndex]->CreateInit(&m_wndPlaceHolder, 0, 0);
    m_nCurChildDlg = nDlgIndex;

    CRect DlgRect, ParentRect, PHRect;
    GetClientRect(&ParentRect);
    m_ChildDlgs[nDlgIndex]->GetWindowRect(&DlgRect);
    ScreenToClient(&DlgRect);

    int Sx, Sy, Wx, Wy;
    Sx = (ParentRect.Width() - DlgRect.Width()) / 2;
    Sy = DlgRect.top;
    Wx = DlgRect.Width();
    Wy = DlgRect.Height();

    m_wndPlaceHolder.SetWindowPos(NULL, Sx, Sy, Wx, Wy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    m_wndPlaceHolder.GetWindowRect(&PHRect);
    ScreenToClient(&PHRect);

    CSize ScrSize, TSize;
    ScrSize = GetTotalSize();
    TSize.cx = PHRect.Width() - m_RectPlaceHolder.Width();
    TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();
    ScrSize += TSize;
    m_RectPlaceHolder = PHRect;
    SetScrollSizes(MM_TEXT, ScrSize);

    if (m_nOrgSlabPlaceHolderHeight == 0)
    {
        m_nOrgSlabPlaceHolderHeight = m_RectPlaceHolder.Height();
    }
}

int CPostTensionTendonProfileDlg::_GetHeightFromFont()
{
    CFont* pNewFont = GetFont();
    CFont* pSysFont, * pOldFont, TempFont;
    CDC* pCdc;
    TEXTMETRIC tmNew, tmSys;
    int nTemp, nCorrectVal;

    pCdc = GetDC();
    pSysFont = TempFont.FromHandle(((HFONT)GetStockObject(SYSTEM_FONT)));
    pOldFont = pCdc->SelectObject(pSysFont);
    pCdc->GetTextMetrics(&tmSys);
    pCdc->SelectObject(pNewFont);
    pCdc->GetTextMetrics(&tmNew);
    pCdc->SelectObject(pOldFont);
    ReleaseDC(pCdc);

    nCorrectVal = (min(tmNew.tmHeight, tmSys.tmHeight) / 2);
    nTemp = tmNew.tmHeight + nCorrectVal;
    return nTemp;
}

void CPostTensionTendonProfileDlg::AdjustComboListBox()
{
    CString str; CSize sz; int dx = 0; CDC* pDC = m_ComboMenu.GetDC();
    for (int i = 0; i < m_ComboMenu.GetCount(); i++)
    {
        m_ComboMenu.GetLBText(i, str);
        sz = pDC->GetTextExtent(str);
        if (sz.cx > dx) dx = sz.cx;
    }
    m_ComboMenu.ReleaseDC(pDC);
    if (m_ComboMenu.GetDroppedWidth() < dx)
    {
        m_ComboMenu.SetDroppedWidth(dx);
        ASSERT(m_ComboMenu.GetDroppedWidth() == dx);
    }
}

void CPostTensionTendonProfileDlg::InitComboMenu()
{
    int wHeight = (m_ComboMenuStr.GetSize() + 1) * _GetHeightFromFont();
    CRect rectWnd; m_ComboMenu.GetWindowRect(rectWnd);
    m_ComboMenu.SetWindowPos(NULL, 0, 0, rectWnd.Width(), wHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    for (int i = 0; i < m_ComboMenuStr.GetSize(); ++i)
        m_ComboMenu.AddString(m_ComboMenuStr[i]);
    if (m_ComboMenu.GetCount() > 0) m_ComboMenu.SetCurSel(0);
    AdjustComboListBox();
}

void CPostTensionTendonProfileDlg::InitContent()
{
    CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST(); (void)pDoc; // placeholder
}

void CPostTensionTendonProfileDlg::OnInitialUpdate()
{
    MChildFormView::OnInitialUpdate();
    InitContent();
    InitComboMenu();
    m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
}

void CPostTensionTendonProfileDlg::OnComboMenuSelect()
{
    int nCurI = m_ComboMenu.GetCurSel();
    if (nCurI < 0 || nCurI >= m_DlgIDs.GetSize()) { ASSERT(0); return; }
    I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);
    ShowCurChildDlg(nCurI);
}

void CPostTensionTendonProfileDlg::OnTmExecute()
{
    if (m_nCurChildDlg < m_ChildDlgs.GetSize())
        m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CPostTensionTendonProfileDlg::OnTmTableBtn()
{
    int nCurSel = m_ComboMenu.GetCurSel();
    if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
    if (m_TableIDs[nCurSel] <= 0) return;
    AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));
}

void CPostTensionTendonProfileDlg::OnTabSelect(WPARAM, LPARAM)
{
    if (CDBDoc::GetDocPoint())
    {
        CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
        CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
        CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
    }
    int nCurID = (m_nCurChildDlg != -1) ? m_nCurChildDlg : 0;
    ShowCurChildDlg(nCurID);
    CTreeMenuBarBase::SetModeOnlyST(m_DlgIDs[nCurID]);
    if (m_TableIDs[nCurID] > 0) m_wndTableBtn.EnableWindow(TRUE); else m_wndTableBtn.EnableWindow(FALSE);
}

void CPostTensionTendonProfileDlg::OnTabUnSelect(WPARAM, LPARAM)
{
}

void CPostTensionTendonProfileDlg::SetHolderSize(int nHeight)
{
    CWnd* pWndHolder = GetDlgItem(IDC_TM_TENDON_PROFILE_PLACEHOLDER);
    if (!pWndHolder) return;
    CRect rectHolder; pWndHolder->GetWindowRect(rectHolder);
    rectHolder.bottom = rectHolder.top + nHeight;
    pWndHolder->GetParent()->ScreenToClient(rectHolder);
    pWndHolder->MoveWindow(rectHolder);

    CRect PHRect; m_wndPlaceHolder.GetWindowRect(&PHRect); ScreenToClient(&PHRect); m_RectPlaceHolder = PHRect;
}

void CPostTensionTendonProfileDlg::ShowExecuteButton(UINT nShowHide)
{
    CWnd* pWnd = GetDlgItem(IDC_TM_EXECUTE);
    if (pWnd) pWnd->ShowWindow(nShowHide);
}

void CPostTensionTendonProfileDlg::OnTmExecAndClose(BOOL bExecute)
{
    if (bExecute) OnTmExecute(); else OnTmClose();
}

int CPostTensionTendonProfileDlg::GetOrgSlabPlaceHolderSize()
{
    return m_nOrgSlabPlaceHolderHeight;
}

CWnd* CPostTensionTendonProfileDlg::GetDlgByID(int Mode)
{
    int nSize = m_DlgIDs.GetSize();
    for (int i = 0; i < nSize; i++) if (Mode == m_DlgIDs[i]) return m_ChildDlgs[i];
    return 0;
}

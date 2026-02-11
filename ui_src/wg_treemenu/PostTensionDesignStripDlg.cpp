#include "stdafx.h"
#include "wg_treemenu.h"
#include "PostTensionDesignStripDlg.h"
#include "DesignStripAutoGenDlg.h"
#include "DesignStripCreateDlg.h"
#include "DesignStripModifySectionDlg.h"
#include "SupportLineCreateDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"

#include "..\wg_db\ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace mit::frx;

IMPLEMENT_DYNCREATE(CPostTensionDesignStripCommonDlg, MChildFormView)

CPostTensionDesignStripCommonDlg::CPostTensionDesignStripCommonDlg()
    : MChildFormView(CPostTensionDesignStripCommonDlg::IDD)
{
    m_vChildDlg.clear();
    m_acsComboMenu.RemoveAll();
    m_nCurChildDlg = -1;
    m_acsComboMenu.Add(_LS(IDS_WG_TREEMENU_CREATE_SUPPORT_LINE));
    m_acsComboMenu.Add(_LS(IDS_WG_TREEMENU_CREATE_DESIGN_STRIP));
    m_acsComboMenu.Add(_LS(IDS_WG_TREEMENU_AUTO_GEN_DESIGN_STRIP));
    m_acsComboMenu.Add(_LS(IDS_WG_TREEMENU_MODIFY_DESIGN_STRIP_SECTION));
    CreateChildDlgs();
}

CPostTensionDesignStripCommonDlg::~CPostTensionDesignStripCommonDlg()
{
    DeleteChildDlgs();
}

void CPostTensionDesignStripCommonDlg::DoDataExchange(CDataExchange* pDX)
{
    MChildFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TM_DESIGNSTRIP_PLACEHOLDER, m_wndPlaceHolder);
    //DDX_Control(pDX, IDC_TM_DESIGNSTRIP_CLOSE, m_wndClose);
    //DDX_Control(pDX, IDC_TM_DESIGNSTRIP_EXECUTE, m_wndExecute);
    DDX_Control(pDX, IDC_TM_DESIGNSTRIP_MENU_COMBO, m_ComboMenu);
    //DDX_Control(pDX, IDC_TM_DESIGNSTRIP_START_STC, m_gbStartNum);
}

BEGIN_MESSAGE_MAP(CPostTensionDesignStripCommonDlg, MChildFormView)
    //ON_BN_CLICKED(IDC_TM_DESIGNSTRIP_CLOSE, OnTmClose)
    ON_CBN_SELCHANGE(IDC_TM_DESIGNSTRIP_MENU_COMBO, OnComboMenuSelect)
    //ON_BN_CLICKED(IDC_TM_DESIGNSTRIP_EXECUTE, OnTmExecute)
    ON_BN_CLICKED(IDC_TM_DESIGNSTRIP_TABLE_BTN, OnTmTableBtn)
END_MESSAGE_MAP()

#ifdef _DEBUG
void CPostTensionDesignStripCommonDlg::AssertValid() const
{
    MChildFormView::AssertValid();
}
void CPostTensionDesignStripCommonDlg::Dump(CDumpContext& dc) const
{
    MChildFormView::Dump(dc);
}
#endif

void CPostTensionDesignStripCommonDlg::OnTmClose()
{
    if (CDBDoc::GetDocPoint())
    {
        CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
        CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1, -1);
        CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
    }
    CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CPostTensionDesignStripCommonDlg::CreateChildDlgs()
{
    m_vChildDlg.clear();
    m_vChildDlg.push_back(T_PTDS_COMM_DLG_INFO(new CSupportLineCreateDlg(), __TMMODE_PT_SUPPORT_LINE_CREATE__, ID_STRUCT_PTDN_DESIGN_STRIP_TABLE));
    m_vChildDlg.push_back(T_PTDS_COMM_DLG_INFO(new CDesignStripCreateDlg(), __TMMODE_PT_DESIGN_STRIP_CREATE__, ID_STRUCT_PTDN_DESIGN_STRIP_TABLE));
    m_vChildDlg.push_back(T_PTDS_COMM_DLG_INFO(new CDesignStripAutoGenDlg(), __TMMODE_PT_DESIGN_STRIP_AUTO__, ID_STRUCT_PTDN_DESIGN_STRIP_TABLE));
    m_vChildDlg.push_back(T_PTDS_COMM_DLG_INFO(new CDesignStripModifySectionDlg(), __TMMODE_PT_DESIGN_STRIP_MODIFY_SECTION__, ID_STRUCT_PTDN_DESIGN_STRIP_TABLE));
}

void CPostTensionDesignStripCommonDlg::DeleteChildDlgs()
{
    for (auto& info : m_vChildDlg)
        delete info.pDlg;
    m_vChildDlg.clear();
}

BOOL CPostTensionDesignStripCommonDlg::ShowChildDlgByID(int nID)
{
    int nIDCount = m_vChildDlg.size();
    for (int i = 0; i < nIDCount; ++i)
    {
        if (nID == m_vChildDlg[i].nDlgID)
        {
            ShowCurChildDlg(i);
            m_ComboMenu.SetCurSel(i);
            return TRUE;
        }
    }
    return FALSE;
}

void CPostTensionDesignStripCommonDlg::ShowCurChildDlg(int nDlgIndex)
{
    if (m_nCurChildDlg == nDlgIndex) return;
    if (nDlgIndex > m_vChildDlg.size() - 1)
    {
        m_nCurChildDlg = nDlgIndex;
        return;
    }

    if (m_nCurChildDlg >= 0 && m_vChildDlg.size() - 1 >= m_nCurChildDlg)
    {
        m_vChildDlg[m_nCurChildDlg].pDlg->End();
    }

    CMenuBarChildDlg* pChildDlg = m_vChildDlg[nDlgIndex].pDlg;
    if (pChildDlg == nullptr) { ASSERT(0); return; }

    m_nCurChildDlg = nDlgIndex;
    pChildDlg->CreateInit(&m_wndPlaceHolder, 0, 0);

    // Layout Child Dialog...
    CRect DlgRect, ParentRect;
    GetClientRect(&ParentRect);
    pChildDlg->GetWindowRect(&DlgRect);
    //pChildDlg->SetExecButton(&m_wndExecute);
    ScreenToClient(&DlgRect);

    int Sx = (ParentRect.Width() - DlgRect.Width()) / 2;
    int Sy = DlgRect.top;
    int Wx = DlgRect.Width();
    int Wy = DlgRect.Height();
    m_wndPlaceHolder.SetWindowPos(NULL, Sx, Sy, Wx, Wy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

    CRect PHRect;
    m_wndPlaceHolder.GetWindowRect(&PHRect);
    ScreenToClient(&PHRect);

//    CRect CBRect, EBRect;
//    m_wndClose.GetWindowRect(CBRect);
//    m_wndExecute.GetWindowRect(EBRect);
//
//    int BY = Sy + Wy + 5;
//    int CW = CBRect.Width();
//    int EW = EBRect.Width();
//    int SEX = PHRect.right - CW - EW - 10;
//    int SCX = SEX + CW + 5;
//#ifdef _RUS
//    SEX = PHRect.right - CW - EW - 2;
//    SCX = SEX + CW + 15;
//#endif
//    m_wndExecute.SetWindowPos(NULL, SEX, BY, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
//    m_wndClose.SetWindowPos(NULL, SCX, BY, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

    // Scroll Size 조정.
    CSize TSize;
    TSize.cx = PHRect.Width() - m_RectPlaceHolder.Width();
    TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();

    CSize ScrSize = GetTotalSize();
    ScrSize += TSize;
    SetScrollSizes(MM_TEXT, ScrSize);

    m_RectPlaceHolder = PHRect;
}

void CPostTensionDesignStripCommonDlg::OnInitialUpdate()
{
    MChildFormView::OnInitialUpdate();
    InitContent();
    InitComboMenu();
    m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
    //m_gbStartNum.InitControl(this, CPostTensionDesignStripCommonDlg::IDD, IDC_TM_DESIGNSTRIP_START_STC, TRUE, FALSE, 3, 4);
}

void CPostTensionDesignStripCommonDlg::OnTabSelect(WPARAM wParam, LPARAM lParam)
{
    int nCurID = (m_nCurChildDlg != -1) ? m_nCurChildDlg : 0;
    ShowCurChildDlg(nCurID);
    if (!m_vChildDlg.empty())
        CTreeMenuBarBase::SetModeOnlyST(m_vChildDlg[nCurID].nDlgID);
}

void CPostTensionDesignStripCommonDlg::OnTabUnSelect(WPARAM wParam, LPARAM lParam)
{
    // 필요시 구현
}

CWnd* CPostTensionDesignStripCommonDlg::GetDlgByID(int Mode)
{
    for (size_t i = 0; i < m_vChildDlg.size(); ++i)
        if (Mode == m_vChildDlg[i].nDlgID) return m_vChildDlg[i].pDlg;
    return 0;
}

void CPostTensionDesignStripCommonDlg::ShowExecuteButton(UINT nShowHide)
{
    //CWnd* pWnd = GetDlgItem(IDC_TM_DESIGNSTRIP_EXECUTE);
    //if (pWnd)
    //    pWnd->ShowWindow(nShowHide);
}

void CPostTensionDesignStripCommonDlg::OnTmExecAndClose(BOOL bExecute)
{
    if (bExecute)
        OnTmExecute();
    else
        OnTmClose();
}

void CPostTensionDesignStripCommonDlg::InitContent()
{
    // 필요시 구현
}

void CPostTensionDesignStripCommonDlg::InitComboMenu(int nInitSel)
{
    int wHeight = (m_acsComboMenu.GetSize() + 1) * 20;
    CRect rectWnd;
    m_ComboMenu.GetWindowRect(rectWnd);
    m_ComboMenu.SetWindowPos(NULL, 0, 0, rectWnd.Width(), wHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    for (int i = 0; i < m_acsComboMenu.GetSize(); ++i)
        m_ComboMenu.AddString(m_acsComboMenu[i]);
    m_ComboMenu.SetCurSel(nInitSel);
}

void CPostTensionDesignStripCommonDlg::UpdateBuffer()
{
    CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
    int nCount = pViewBuff->GetCount();
    if (nCount == 0) return;
    ASSERT(nCount == 1);

    int nMethod = -1;

    POSITION pos = pViewBuff->GetStartBuffer();
    while (pos != nullptr)
    {
        T_UDRD_BUFFER buffer_ur = pViewBuff->GetNextBuffer(pos);
        switch (buffer_ur.nCmd)
        {
        case UR_SEME_ADD:
            {
                T_SEME_D SemeD;
                pViewBuff->GetSeme(buffer_ur.nKey, SemeD);
                nMethod = SemeD.nMethod;
            }
            break;
        default:
            break;
        }
    }

    if (nMethod > -1)
    {
        GetComboMenuItem(m_acsComboMenu, nMethod);
        CreateChildDlgs();
        InitComboMenu(0);
        ShowCurChildDlg(0);
    }
}

void CPostTensionDesignStripCommonDlg::GetComboMenuItem(CStringArray& raMenuItem, int nMethod)
{
    // 필요시 구현
    raMenuItem.Add(_T("Test1"));
    raMenuItem.Add(_T("Test2"));
}

void CPostTensionDesignStripCommonDlg::OnViewStartNodeNum()
{
    // 필요시 구현 (예: 노드 번호 보기 다이얼로그 호출)
}

void CPostTensionDesignStripCommonDlg::OnViewStartElemNum()
{
    // 필요시 구현 (예: 요소 번호 보기 다이얼로그 호출)
}

void CPostTensionDesignStripCommonDlg::OnComboMenuSelect()
{
    int nCurI = m_ComboMenu.GetCurSel();
    if (nCurI < 0 || nCurI >= (int)m_vChildDlg.size())
        return;
    ShowCurChildDlg(nCurI);
    //m_wndExecute.ShowWindow(SW_SHOW);
    //m_wndClose.ShowWindow(SW_SHOW);
}

void CPostTensionDesignStripCommonDlg::OnTmExecute()
{
    if (m_nCurChildDlg < (int)m_vChildDlg.size())
        m_vChildDlg[m_nCurChildDlg].pDlg->Execute();
}

void CPostTensionDesignStripCommonDlg::OnTmTableBtn()
{
    // 필요시 구현 (예: 테이블 버튼 동작)
}

void CPostTensionDesignStripCommonDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    HWND hWnd = GetSafeHwnd();
    if ((hWnd == nullptr) || !IsWindow(hWnd)) return;

    switch (lHint)
    {
    case D_UPDATE_DBALL:
    case D_UPDATE_DEFAULT:
    case D_UPDATE_BUFFER_AFTER:
        {
            UpdateBuffer();
        }
        break;
    default:
        break;
    }
}

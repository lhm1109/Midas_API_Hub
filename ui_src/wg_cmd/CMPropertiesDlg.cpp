// CMPropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMPropertiesDlg.h"

// #include "..\wg_base\wg_base_XPageSheet.h"

#include "CMMaterialPage.h"
#include "CMSectionPage.h"
#include "CMThicknessPage.h"

#include "..\mit_frx\global_Opt4Perf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMPropertiesDlg dialog


CCMPropertiesDlg::CCMPropertiesDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMPropertiesDlg::IDD, pParent)
{
	// m_pSheet = new CXPropertySheet();
	m_nActivePage = 0 ;
	//{{AFX_DATA_INIT(CCMPropertiesDlg)m_editTip
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCMPropertiesDlg::~CCMPropertiesDlg()
{
	// delete m_pSheet;
}

void CCMPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMPropertiesDlg)
// 	DDX_Control(pDX, IDC_CMD_SECT_SIZE_TIP, m_editTip);
	DDX_Control(pDX, IDC_CMD_PROP_TAB     , m_wndTab );
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCMPropertiesDlg public interface
void CCMPropertiesDlg::SetActivePage(int SelectPage)
{
//   if(::IsWindow(m_pSheet->m_hWnd)) (CXTabCtrl&)(*m_pSheet) = SelectPage; 
//   m_nActivePage = SelectPage;
	m_wndTab.ShowTab(SelectPage);
	m_nActivePage = SelectPage;
}

// 추가 
// void CCMPropertiesDlg::SetShowTip(CString& csTip)
// {
// 	CWnd* pWnd = GetDlgItem(IDC_CMD_SECT_SIZE_TIP);
// 	ASSERT(pWnd);
// 
// // 	if(csTip == _T("")) pWnd->ShowWindow(SW_HIDE);
// // 	else
// // 	{
// 		pWnd->ShowWindow(SW_SHOW);
// 		m_editTip.SetReadOnly();
// // 	}
// 	pWnd->SetWindowText(csTip);
// 	m_csTip = csTip;
// }


#define CDialog CCMDlgBase
BEGIN_MESSAGE_MAP(CCMPropertiesDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CCMPropertiesDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnDlgClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMPropertiesDlg Note
// OnInitDialog, OnNotify, OnDlgClose(Close button)은
// wg_gr의 CDlgUcsDefine이 원본이므로 이것을 참조하여 만든다.

/////////////////////////////////////////////////////////////////////////////
// CCMPropertiesDlg message handlers

BOOL CCMPropertiesDlg::OnInitDialog() 
{
	gGuardSetRedraw_Prop gsRedraw(this);

	CCMDlgBase::OnInitDialog();
	
//   ModifyStyle(0, WS_CLIPCHILDREN);
	// // TODO: Add extra initialization here
//   CWnd * pHolder = GetDlgItem(IDC_CMD_PLACEHOLDER);
//   CRect hRect;
//   pHolder->GetWindowRect(&hRect);
//   ScreenToClient(&hRect);
//   pHolder->ShowWindow(SW_HIDE);
//                                   //| WS_DLGFRAME | WS_THICKFRAME 
//   m_pSheet->Create(TCXS_UP | WS_CHILD | WS_VISIBLE, hRect, this, 0);
	
	// Property Page 초기화  
	m_pMaterial = new CCMMaterialPage(m_pDoc, this);
	m_pSection = new CCMSectionPage(m_pDoc, this);
#if !defined(_TOWER)
	m_pThickness = new CCMThicknessPage(m_pDoc, this);
#endif

	m_wndTab.AddTab(m_pMaterial , _LS(IDS_CMD_PJST_Material ), CCMMaterialPage::IDD , TRUE);
	m_wndTab.AddTab(m_pSection  , _LS(IDS_CMD_PJST_Section  ), CCMSectionPage::IDD  , TRUE);
#if !defined(_TOWER)
	m_wndTab.AddTab(m_pThickness, _LS(IDS_CMD_PJST_Thickness), CCMThicknessPage::IDD, TRUE);
#endif

	m_wndTab.SetPlaceHolder(GetDlgItem(IDC_CMD_PLACEHOLDER));
	m_wndTab.ShowTab(m_nActivePage);

//   // add page to sheet
//   m_pSheet->AddPage(m_pMaterial);
//   m_pSheet->AddPage(m_pSection);
// #if !defined(_TOWER)
//   m_pSheet->AddPage(m_pThickness);
// #endif
//   
//   (CXTabCtrl&)(*m_pSheet) = m_nActivePage;
	
	// add code for active page ...

	return FALSE; // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMPropertiesDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CCMPropertiesDlg::OnDlgClose() 
{
	// TODO: Add your control notification handler code here
	DestroyWindow();
}

void CCMPropertiesDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START:
		DestroyWindow(); return;
	case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
	
	if ( IsWindow(m_pMaterial->GetSafeHwnd()) ) m_pMaterial->OnUpdate(pSender, lHint, pHint);
	if ( IsWindow(m_pSection->GetSafeHwnd()) ) m_pSection->OnUpdate(pSender, lHint, pHint);

#if !defined(_TOWER)
	if ( IsWindow(m_pThickness->GetSafeHwnd()) ) m_pThickness->OnUpdate(pSender, lHint, pHint);
#endif
}

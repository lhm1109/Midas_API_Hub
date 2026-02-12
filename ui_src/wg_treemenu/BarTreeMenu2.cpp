//////////////////////////////////////////////////////////////////////////////
// BarTreeMenu2.cpp : implementation file

#include "stdafx.h"
#include "BarTreeMenu2.h"
#include "TreeMenuCtrl.h"
#include "TreeTableCtrl.h"
#include "TreeGroupCtrl.h"
#include "TreeWorkCtrl.h"
#include "TreeReportCtrl.h"
#include "TreeJUDCtrl.h"
#include "TreeSeisCtrl.h"

#include "..\wg_dbLock\LockOption.h"
#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_base\TreeMenuMode.h"

#ifdef _DEBUG
#define new DEBUG_NEW

#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////
// CBarTreeMenu2
IMPLEMENT_DYNAMIC(CBarTreeMenu2, CControlBarBase)

CBarTreeMenu2::CBarTreeMenu2()
{
	m_nCurMode        = __TMMODE_TREEMENU_MENU__;
	m_nPrevMode       = m_nCurMode;
	m_pTreeMenuCtrl   = new CTreeMenuCtrl;
	m_pTreeTableCtrl  = new CTreeTableCtrl;
	m_pTreeWorkCtrl   = NULL;
	m_pTreeGroupCtrl  = NULL;
	m_pTreeReportCtrl = NULL;
	m_pTreeJUDCtrl    = NULL;
	m_pTreeSeisCtrl   = NULL;
	m_pCurWnd         = NULL; 

	m_TreeCtrlBkColor = ReadBkColor();
}

CBarTreeMenu2::~CBarTreeMenu2()
{
	if(m_pTreeMenuCtrl)
	  delete m_pTreeMenuCtrl;
	if(m_pTreeTableCtrl)
		delete m_pTreeTableCtrl;
	if(m_pTreeGroupCtrl)
		delete m_pTreeGroupCtrl;
	if(m_pTreeSeisCtrl)
		delete m_pTreeSeisCtrl;
}

BEGIN_MESSAGE_MAP(CBarTreeMenu2, CControlBarBase)
 //{{AFX_MSG_MAP(CBarTreeMenu2)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBarTreeMenu2 message handlers
BOOL CBarTreeMenu2::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	//cs.dwExStyle &= ~(LONG)CBRS_EX_GRIPPER_CLOSE;
	return CControlBarBase::PreCreateWindow(cs);
}

void CBarTreeMenu2::OnSize(UINT nType, int cx, int cy) 
{
	_ChildLayout();
	CControlBarBase::OnSize(nType, cx, cy);
}

BOOL CBarTreeMenu2::OnEraseBkgnd(CDC* pDC) 
{
	ASSERT_VALID(pDC);
	return CControlBarBase::OnEraseBkgnd(pDC);
}

int CBarTreeMenu2::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CControlBarBase::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CreateDefaultTreeMenu();	
	
	return 0;
}

int  CBarTreeMenu2::GetCurMode()
{
	return m_nCurMode;
}

void CBarTreeMenu2::_SetCurWnd(CWnd * pCurWnd)
{
	m_pCurWnd = pCurWnd;
}

CWnd* CBarTreeMenu2::_GetCurWnd()
{
	return m_pCurWnd;
}

CWnd* CBarTreeMenu2::GetCurWnd()
{
	if (m_wndTab.GetSafeHwnd() == NULL)
		return nullptr;

	return m_wndTab.GetActiveWnd();
}

void CBarTreeMenu2::_ChildLayout()
{
	/*****************************************************************************
	@@	Control Bar에 위치할 Child Window들의 Sizing을 행한다. 
	@@	GetInsideRect() ==> CControlBarBase에서 Child Window가 위치할 영역에 대한 
	@@	Rectangle을 제공한다. 
	*/
	CRect rectInside;
	
	GetClientRect(rectInside);
		
	switch(GetCurMode())
	{
	case __TMMODE_TREEMENU__:
	case __TMMODE_TREEMENU_MENU__:
	case __TMMODE_TREEMENU_TABLE__:
	case __TMMODE_TREEMENU_GROUP__:
	case __TMMODE_TREEMENU_WORK__:
	case __TMMODE_TREEMENU_REPORT__:
	case __TMMODE_TREEMENU_JUD__:
	case __TMMODE_TREEMENU_SEIS__:
		::SetWindowPos(m_wndTab.m_hWnd, NULL, rectInside.left, rectInside.top,
		rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		break;
	}
}

void CBarTreeMenu2::CreateDefaultTreeMenu()
{
#if !defined(_CIVIL)
	m_pTreeMenuCtrl->SetBkGroundColor (m_TreeCtrlBkColor);
#endif
	m_pTreeTableCtrl->SetBkGroundColor(m_TreeCtrlBkColor);
	
	if(m_pTreeWorkCtrl)
		m_pTreeWorkCtrl->SetBkGroundColor(m_TreeCtrlBkColor);
		
	if(m_pTreeGroupCtrl)
		m_pTreeGroupCtrl->SetBkGroundColor(m_TreeCtrlBkColor);  

#if defined(_MGEN) || defined(_CIVIL)
	if(m_pTreeReportCtrl)
		m_pTreeReportCtrl->SetBkGroundColor(m_TreeCtrlBkColor);
#endif


	// TODO: Add your specialized creation code here
	m_wndTab.Create(this, 1000, TRUE);
	
#if !defined(_CIVIL)
	m_pTreeMenuCtrl->Create(TVS_HASLINES|TVS_HASBUTTONS|WS_VISIBLE|WS_CHILD|TVS_SHOWSELALWAYS|TVS_LINESATROOT,
		CRect(0, 0, 0, 0), &m_wndTab, 0x100);
	m_pTreeMenuCtrl->EnableToolTips(TRUE); 
	m_wndTab.AddTab(m_pTreeMenuCtrl, _LS(IDS_WG_TREEMENU_Menu));
#endif

	m_pTreeTableCtrl->Create(TVS_HASLINES|TVS_HASBUTTONS|WS_VISIBLE|WS_CHILD|TVS_SHOWSELALWAYS|TVS_LINESATROOT,
		CRect(0, 0, 0, 0), &m_wndTab, 0x101);
	m_pTreeTableCtrl->EnableToolTips(TRUE);

	m_wndTab.AddTab(m_pTreeTableCtrl, _LS(IDS_WG_TREEMENU_Tables));
		

	m_wndTab.SetActiveTab(0);
	m_wndTab.ScrollToTab(0);
	_SetCurWnd(&m_wndTab);

#if defined(_MGEN)
	if(CProduct::GetMovingType()==D_PRODUCT_MOVING_JP)
	{
		if(m_pTreeJUDCtrl)
			m_pTreeJUDCtrl->SetBkGroundColor(m_TreeCtrlBkColor);
	}  
#endif
}

void CBarTreeMenu2::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CControlBarBase::OnShowWindow(bShow, nStatus);
}

////////////////////////////////////////////////////////////////////////////////////
// by jbseon
//!@#$void CBarTreeMenu2::DestroyTabWnd(SEC3DTabWnd* pTabWnd)
void CBarTreeMenu2::DestroyTabWnd(mit::frx::MBaseTabWnd* pTabWnd)
{
	if(::IsWindow(pTabWnd->GetSafeHwnd()))
	{
		pTabWnd->RemoveAllTabs();

		pTabWnd->SetFocus();
		pTabWnd->DestroyWindow();
	}
}

////////////////////////////////////////////////////////////////////////////////////
// Creation And Destruction of GPS Tab Window Ended, by mjpark       ///////////////
////////////////////////////////////////////////////////////////////////////////////

BOOL CBarTreeMenu2::ShowDlgWithInit(int Mode, UINT key)
{
	MySEC3DTabWnd* pTabWnd = (MySEC3DTabWnd*)m_pCurWnd;
	CWnd* pView = NULL;
	if (!pTabWnd->GetActiveTab(pView)) return FALSE;
	if (!pView->GetRuntimeClass()->IsDerivedFrom(RUNTIME_CLASS(CFormViewInBar))) return FALSE;

	CFormViewInBar* pFormView = (CFormViewInBar*)pView;
	CMenuBarChildDlg* pChildDlg = (CMenuBarChildDlg*)pFormView->GetDlgByID(Mode);
	if (pChildDlg == 0) return FALSE;
	if (!pChildDlg->GetSafeHwnd() || !IsWindow(pChildDlg->GetSafeHwnd())) return FALSE;

	pChildDlg->ExternalInit(key);

	return TRUE;
}

void CBarTreeMenu2::AddGroupTab()
{
#ifdef _MVIL
	return; // *^^* 빌라 버전에서 뺀다.
#endif

	if (m_pTreeGroupCtrl != NULL)
	{
		TRACE(_LS(IDS_WG_TREEMENU_Warning___Group_tab_already_exist_));
		return;
	}

	m_pTreeGroupCtrl   = new CTreeGroupCtrl;

	if(m_pTreeGroupCtrl)
		m_pTreeGroupCtrl->SetBkGroundColor(m_TreeCtrlBkColor);  

	m_pTreeGroupCtrl->Create(TVS_HASLINES|TVS_HASBUTTONS|WS_VISIBLE|WS_CHILD|TVS_SHOWSELALWAYS|TVS_LINESATROOT|TVS_EDITLABELS,
		CRect(0, 0, 0, 0), &m_wndTab, 0x103);
	m_pTreeGroupCtrl->EnableToolTips(TRUE); 
	m_wndTab.AddTab(m_pTreeGroupCtrl, _LS(IDS_WG_TREEMENU_Group));
}

void CBarTreeMenu2::RemoveGroupTab()
{
#ifdef _MVIL
	return; // *^^* 빌라 버전에서 뺀다.
#endif

	if (m_pTreeGroupCtrl == NULL)
	{
		TRACE(_LS(IDS_WG_TREEMENU_Warning___Group_tab_not_exist_));
		return;
	}

	m_wndTab.ActivateTab(0);
	m_wndTab.ScrollToTab(0);
	
	m_wndTab.RemoveTab(m_pTreeGroupCtrl);
	if (m_pTreeGroupCtrl->GetSafeHwnd() && IsWindow(m_pTreeGroupCtrl->GetSafeHwnd()))
	{
		m_pTreeGroupCtrl->DestroyWindow();
		
	}
	//--------------------------------------------------
	//--- RemoveWorkTab 호출전에 윈도우가 삭제 되었을때 
	//--- Object를 Delete 하지 않아서 Memory Leak발생 
	//--- BCG로 바뀌면서 변경된 내용이 있나보다. 
	// Changkeun 
	delete m_pTreeGroupCtrl;
	m_pTreeGroupCtrl = 0;
}

void CBarTreeMenu2::AddWorkTab()
{
	if (m_pTreeWorkCtrl != NULL)
	{
		TRACE(_LS(IDS_WG_TREEMENU_Warning___Work_tab_already_exist_));
		return;
	}

	m_pTreeWorkCtrl   = new CTreeWorkCtrl;
	
	if(m_pTreeWorkCtrl)
		m_pTreeWorkCtrl->SetBkGroundColor(m_TreeCtrlBkColor);
	
	m_pTreeWorkCtrl->Create(TVS_HASLINES|TVS_HASBUTTONS|WS_VISIBLE|WS_CHILD|TVS_SHOWSELALWAYS|TVS_LINESATROOT,
		CRect(0, 0, 0, 0), &m_wndTab, 0x103);
	m_pTreeWorkCtrl->EnableToolTips(TRUE); 
	m_wndTab.AddTab(m_pTreeWorkCtrl, _LS(IDS_WG_TREEMENU_Works));
}

void CBarTreeMenu2::RemoveWorkTab()
{
	if (m_pTreeWorkCtrl == NULL)
	{
		TRACE(_LS(IDS_WG_TREEMENU_Warning___Work_tab_not_exist_));
		return;
	}

	m_wndTab.ActivateTab(0);
	m_wndTab.ScrollToTab(0);
	
	m_wndTab.RemoveTab(m_pTreeWorkCtrl);
	if (m_pTreeWorkCtrl->GetSafeHwnd() && IsWindow(m_pTreeWorkCtrl->GetSafeHwnd()))
	{
		m_pTreeWorkCtrl->DestroyWindow();

	}
	//--- RemoveWorkTab 호출전에 윈도우가 삭제 되었을때 
	//--- Object를 Delete 하지 않아서 Memory Leak발생 
	//--- BCG로 바뀌면서 변경된 내용이 있나보다. Changkeun 
	delete m_pTreeWorkCtrl;
	m_pTreeWorkCtrl = 0;
}

void CBarTreeMenu2::AddReportTab()
{
#if defined(_MGEN) || defined(_CIVIL)
	if (m_pTreeReportCtrl != NULL)
	{
		TRACE(_LS(IDS_WG_TREEMENU_Warning___Work_tab_already_exist_));
		return;
	}

	m_pTreeReportCtrl   = new CTreeReportCtrl;
	
	if(m_pTreeReportCtrl)
		m_pTreeReportCtrl->SetBkGroundColor(m_TreeCtrlBkColor);
	
	m_pTreeReportCtrl->Create(TVS_HASLINES|TVS_HASBUTTONS|WS_VISIBLE|WS_CHILD|TVS_SHOWSELALWAYS|TVS_LINESATROOT,
		CRect(0, 0, 0, 0), &m_wndTab, 0x103);
	m_pTreeReportCtrl->EnableToolTips(TRUE); 

	m_wndTab.AddTab(m_pTreeReportCtrl, _LS(IDS_TM_WORK_Report));
#endif
}

void CBarTreeMenu2::RemoveReportTab()
{
#if defined(_MGEN) || defined(_CIVIL)
	if (m_pTreeReportCtrl == NULL)
	{
		TRACE(_LS(IDS_WG_TREEMENU_Warning___Work_tab_not_exist_));
		return;
	}

	m_wndTab.ActivateTab(0);
	m_wndTab.ScrollToTab(0);
	
	m_wndTab.RemoveTab(m_pTreeReportCtrl);
	if (m_pTreeReportCtrl->GetSafeHwnd() && IsWindow(m_pTreeReportCtrl->GetSafeHwnd()))
	{
		m_pTreeReportCtrl->DestroyWindow();

	}
	//--- RemoveWorkTab 호출전에 윈도우가 삭제 되었을때 
	//--- Object를 Delete 하지 않아서 Memory Leak발생 
	//--- BCG로 바뀌면서 변경된 내용이 있나보다. Changkeun 
	delete m_pTreeReportCtrl;
	m_pTreeReportCtrl = 0;
#endif
}

void CBarTreeMenu2::AddJUDTab()
{   
#if defined(_MGEN)
	{
		if (m_pTreeJUDCtrl != NULL)
		{
			TRACE(_LS(IDS_WG_TREEMENU_Warning___Work_tab_already_exist_));
			return;
		}
		
		m_pTreeJUDCtrl   = new CTreeJUDCtrl;
		
		if(m_pTreeJUDCtrl)
			m_pTreeJUDCtrl->SetBkGroundColor(m_TreeCtrlBkColor);
		
		m_pTreeJUDCtrl->Create(TVS_HASLINES|TVS_HASBUTTONS|WS_VISIBLE|WS_CHILD|TVS_SHOWSELALWAYS|TVS_LINESATROOT,
			CRect(0, 0, 0, 0), &m_wndTab, 0x103);
		m_pTreeJUDCtrl->EnableToolTips(TRUE); 
		
		m_wndTab.AddTab(m_pTreeJUDCtrl, _LS(IDS_TREE_JUD));
	}
#endif
}

void CBarTreeMenu2::RemoveJUDTab()
{ 
#if defined(_MGEN)
	{
		if (m_pTreeJUDCtrl == NULL)
		{
			TRACE(_LS(IDS_WG_TREEMENU_Warning___Work_tab_not_exist_));
			return;
		}
		
		m_wndTab.ActivateTab(0);
		m_wndTab.ScrollToTab(0);
		
		m_wndTab.RemoveTab(m_pTreeJUDCtrl);
		if (m_pTreeJUDCtrl->GetSafeHwnd() && IsWindow(m_pTreeJUDCtrl->GetSafeHwnd()))  
			m_pTreeJUDCtrl->DestroyWindow();    
		
		//--- RemoveWorkTab 호출전에 윈도우가 삭제 되었을때 
		//--- Object를 Delete 하지 않아서 Memory Leak발생 
		//--- BCG로 바뀌면서 변경된 내용이 있나보다. Changkeun 
		delete m_pTreeJUDCtrl;
		m_pTreeJUDCtrl = 0;
	}  
#endif
}

void CBarTreeMenu2::AddSeisTab()
{
	static BOOL bSeisEvalLockOption = CTestEnvMgr::IsTestEnvST(_LSX(SeisEvalLockOption), _LSX(yes));
	if (!bSeisEvalLockOption && !CDBDoc::GetDocPoint()->AllowCommand(D_OPTN_ID_SEIS_EVAL) && !CDBDoc::GetDocPoint()->AllowCommand(D_OPTN_ID_SPD))
		return;

	if (!CProduct::IsSeisEval()) return;

	if (m_pTreeSeisCtrl != NULL)
	{
		TRACE(_LS(IDS_WG_TREEMENU_Warning___Work_tab_already_exist_));
		return;
	}

	m_pTreeSeisCtrl = new CTreeSeisCtrl;

	if(m_pTreeSeisCtrl)
		m_pTreeSeisCtrl->SetBkGroundColor(m_TreeCtrlBkColor);

	m_pTreeSeisCtrl->Create(TVS_HASLINES|TVS_HASBUTTONS|WS_VISIBLE|WS_CHILD|TVS_SHOWSELALWAYS|TVS_LINESATROOT,
		CRect(0, 0, 0, 0), &m_wndTab, 0x103);
	m_pTreeSeisCtrl->EnableToolTips(TRUE); 

	m_wndTab.AddTab(m_pTreeSeisCtrl, _LS(IDS_WG_TREEMENU_Seismic));
}

void CBarTreeMenu2::RemoveSeisTab()
{
	if (!CProduct::IsSeisEval()) return;

	if (m_pTreeSeisCtrl == NULL)
	{
		TRACE(_LS(IDS_WG_TREEMENU_Warning___Work_tab_not_exist_));
		return;
	}

	m_wndTab.ActivateTab(0);
	m_wndTab.ScrollToTab(0);

	m_wndTab.RemoveTab(m_pTreeSeisCtrl);
	if (m_pTreeSeisCtrl->GetSafeHwnd() && IsWindow(m_pTreeSeisCtrl->GetSafeHwnd()))  
		m_pTreeSeisCtrl->DestroyWindow();    

	//--- RemoveWorkTab 호출전에 윈도우가 삭제 되었을때 
	//--- Object를 Delete 하지 않아서 Memory Leak발생 
	//--- BCG로 바뀌면서 변경된 내용이 있나보다. Changkeun 
	delete m_pTreeSeisCtrl;
	m_pTreeSeisCtrl = 0;
}

void CBarTreeMenu2::SetBkGroundColor(COLORREF BkColor)
{
	m_TreeCtrlBkColor = BkColor;

#if !defined(_CIVIL)  
	m_pTreeMenuCtrl->SetBkGroundColor(BkColor);
#endif
	m_pTreeTableCtrl->SetBkGroundColor(BkColor);
	if(m_pTreeWorkCtrl)
		m_pTreeWorkCtrl->SetBkGroundColor(BkColor);
	
	if(m_pTreeGroupCtrl)
		m_pTreeGroupCtrl->SetBkGroundColor(BkColor);
	
#if defined(_MGEN) || defined(_CIVIL)
	if(m_pTreeReportCtrl)
		m_pTreeReportCtrl->SetBkGroundColor(BkColor);
#endif
	
#if defined(_MGEN)
	if(CProduct::GetMovingType()==D_PRODUCT_MOVING_JP)
	{
		if(m_pTreeJUDCtrl)
			m_pTreeJUDCtrl->SetBkGroundColor(BkColor);
	}
#endif

	if(CProduct::IsSeisEval() && m_pTreeSeisCtrl)
	{
		m_pTreeSeisCtrl->SetBkGroundColor(BkColor);
	}
}

//!@#$ MNET:XXXX 20051028 LBI : Floating 항목을 없앰... 
BOOL CBarTreeMenu2::OnShowControlBarMenu (CPoint point)
{
	if (g_pContextMenuManager == NULL)
	{
		return FALSE;
	}

	const UINT idAutoHide	= (UINT) -104;
	const UINT idHide		= (UINT) -105;

	CMenu menu;
	menu.CreatePopupMenu ();

	{
		CString strItem;

		strItem.Format(_LS(IDS_TM_AUTOHIDE));
		menu.AppendMenu (MF_STRING, idAutoHide, strItem);

		strItem.Format(_LS(IDS_TM_HIDE));
		menu.AppendMenu (MF_STRING, idHide,	strItem);
	}

	if (IsAutoHideMode ())
	{
		menu.CheckMenuItem (idAutoHide, MF_CHECKED);
	}

	int nMenuResult = g_pContextMenuManager->TrackPopupMenu (
			menu, point.x, point.y, this);

	switch (nMenuResult)
	{
	case idAutoHide:
		ToggleAutoHide ();
		break;

	case idHide:
		BOOL bIsAutoHide = IsAutoHideMode();
		ShowControlBar(!IsVisible(), FALSE, bIsAutoHide);
		break;
	}

	return TRUE;
}

// Docking Bar의 Close 버튼을 눌렀을 때 customize...
void CBarTreeMenu2::OnPressCloseButton ()
{
	CBCGPDockingControlBar::OnPressCloseButton();
}
// MySEC3DTabWnd.cpp: implementation of the MySEC3DTabWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MySEC3DTabWnd.h"
#include "..\mit_frx\MBarBaseDlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(MySEC3DTabWnd, MBaseTabWnd)

//#define CWnd 
BEGIN_MESSAGE_MAP(MySEC3DTabWnd, MBaseTabWnd)
//#undef CWnd

	//{{AFX_MSG_MAP(MySEC3DTabWnd)
	ON_MESSAGE(TCM_TABSEL, OnTabSelect)
	ON_MESSAGE(WM_NOTIFY, OnTreeCtrlNotify)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	//!@#$ON_MESSAGE(TCM_TABREACTIVATE, OnTabReActivate)
END_MESSAGE_MAP()

MySEC3DTabWnd::MySEC3DTabWnd()
//!@#$:CBCGPTabWnd()
{
}

MySEC3DTabWnd::~MySEC3DTabWnd()
{

}

void MySEC3DTabWnd::ActivateTab(int nIndex)
{
	//!@#$SEC3DTabWnd::ActivateTab(nIndex);
	CWnd* pWnd = GetTabWnd(nIndex);
	if(pWnd)  
	{ 
		ActivateTab(pWnd, nIndex);
	}
	//GDebugOut("%d",nIndex);
}

void MySEC3DTabWnd::ActivateTab(CWnd* pWnd, int nIndex)
{
	
	//CWnd *_pWnd = NULL;
	//int nIndex;
	//GetActiveTab(nIndex);
	//SEC3DTabWnd::GetActiveTab(_pWnd);
	//if(_pWnd)
	//{
	//  if(_pWnd->IsKindOf(RUNTIME_CLASS(CFormViewInBar)))  
	//    ((CFormViewInBar*)_pWnd)->OnTabUnSelect(NULL,NULL);
	//}
	
	//!@#4SEC3DTabWnd::ActivateTab(pWnd,nIndex);
	if(pWnd && pWnd->GetSafeHwnd())
	{
		if(pWnd->IsKindOf(RUNTIME_CLASS(CFormViewInBar)))
		{
			((CFormViewInBar*)pWnd)->OnTabSelect(NULL,NULL);
		}
		else if (pWnd->IsKindOf(RUNTIME_CLASS(mit::frx::MBarBaseDlg)))
		{
			// add tab 할 때는 실행 안되도록..
			if ( m_bInserting == FALSE)
				((mit::frx::MBarBaseDlg*)pWnd)->OnParentTabChanged();
		}
		SetActiveTab(nIndex);    
	}
}

LRESULT MySEC3DTabWnd::OnTabSelect(WPARAM wParam, LPARAM /*lParam*/) 
{
	/*!@#$
	ASSERT_VALID(m_pTabCtrl);

	LPCTSTR lpszLabel;
	BOOL bSelected;
	CObject* pClient;
	HMENU hMenu;
	void* pExtra;
	
	m_pTabCtrl->GetTabInfo((int)wParam, lpszLabel, bSelected,
			pClient, hMenu, pExtra);
	ASSERT_VALID((CWnd*)pClient);
	ActivateTab((CWnd*)pClient, (int)wParam);
	*/
	
	return 0;
}

static const TCHAR* pszL = NULL;
CString _SS(int i)
{
	CString sTemp; AfxExtractSubString(sTemp, pszL, i, TCHAR(','));
	return sTemp;
}


#ifdef _UNICODE
	#define _LL(i) _ttol(_SS(i))
#else
	#define _LL(i) _ttol(_SS(i))
#endif

#define _BB(i) (BYTE)_LL(i)
#define StartConversion(s) pszL = s
//void FillLogFont(LOGFONT& logFont, LPCTSTR lpszFontString = _defaultFont)
void FillLogFont(LOGFONT& logFont, LPCTSTR lpszFontString)
{
	StartConversion(lpszFontString);
	logFont.lfHeight = _LL(0);
	logFont.lfWidth = _LL(1);
	logFont.lfEscapement = _LL(2);
	logFont.lfOrientation = _LL(3);
	logFont.lfWeight = _LL(4);
	logFont.lfItalic = _BB(5);
	logFont.lfUnderline = _BB(6);
	logFont.lfStrikeOut = _BB(7);
	//logFont.
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfOutPrecision = _BB(9);
	logFont.lfClipPrecision = _BB(10);
	logFont.lfQuality = _BB(11);
	logFont.lfPitchAndFamily = _BB(12);
	CString TempStr = _SS(13);  
	if (TempStr.GetLength() > LF_FACESIZE)
	{
		logFont.lfFaceName[0] = '\0';
	}
	else
	{
		CStringA aFaceName(TempStr);
		memcpy(logFont.lfFaceName, aFaceName, aFaceName.GetLength() + 1);
	}
}



int MySEC3DTabWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MBaseTabWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

LRESULT MySEC3DTabWnd::OnTreeCtrlNotify(WPARAM wParam, LPARAM lParam)
{
	NMHDR* pNHMDR = reinterpret_cast<NMHDR*>(lParam);

	if (pNHMDR != nullptr)
	{

	}
	return MBaseTabWnd::OnNotify(wParam, lParam, nullptr);
}

/*!@#$
BOOL MySEC3DTabWnd::CreateTabCtrl(DWORD dwStyle, UINT nID)
{

	 //if(!SEC3DTabWnd::CreateTabCtrl( dwStyle,  nID)) return FALSE;
	 ASSERT_VALID(this);

	 m_pTabCtrl = new MySEC3DTabControl();
	 ASSERT_VALID(this);
	 VERIFY(m_pTabCtrl->Create(WS_VISIBLE | m_dwTabCtrlStyle,
		CRect(0, 0, 0, 0), this, nID));

	 dwStyle; // UNUSED
	 //return (m_pTabCtrl != NULL);
	 if(!m_pTabCtrl) return FALSE;
	 
	 CFont TFont;
	 CString TempStr;
#if defined(_CH)
	 TempStr = _T("Tahoma");
#else   
	 TempStr = _LS(IDS_WG_TREEMENU___TABFONT);
#endif
	 //TempStr = "궁서";
	 //TempStr = "MS San Sarif";

	 CString _defaultFont = _T("-11,0,0,0,300,0,0,0,0,1,2,1,34,");
	 CString _boldFont    = _T("-11,0,0,0,400,0,0,0,0,1,2,1,34,");
	 
	 _defaultFont += TempStr;
	 _boldFont    += TempStr;

	 LOGFONT logFont;
	 FillLogFont(logFont,_boldFont);
	 TFont.CreateFontIndirect(&logFont);
	 SetFontActiveTab(&TFont,FALSE) ;
	 TFont.DeleteObject();
	 FillLogFont(logFont,_defaultFont);
	 TFont.CreateFontIndirect(&logFont);
	 SetFontInactiveTab(&TFont,FALSE);
	 TFont.DeleteObject();
	 
	 return TRUE;
}
*/

void MySEC3DTabWnd::FireChangeActiveTab (int nNewTab)
{
	CWnd* pWnd = GetTabWnd(nNewTab);
	ActivateTab(pWnd, nNewTab);

	MBaseTabWnd::FireChangeActiveTab(nNewTab);
}

CFrameWnd* MyBCGPGetParentFrame (const CWnd* pWnd)
{
	if (pWnd->GetSafeHwnd () == NULL)
	{
		return NULL;
	}
	ASSERT_VALID (pWnd);

//     CWnd* pMainWnd = AfxGetMainWnd();
//     return  (CFrameWnd*)pMainWnd;


	const CWnd* pParentWnd = pWnd;

	while (pParentWnd != NULL)
	{
		if (pParentWnd->IsKindOf (RUNTIME_CLASS (CBCGPMiniFrameWnd)))
		{
			CBCGPMiniFrameWnd* pMiniFrame = DYNAMIC_DOWNCAST (CBCGPMiniFrameWnd, pParentWnd);
			// pParentWnd = pMiniFrame->GetParent ();
			return (CFrameWnd*)pMiniFrame;
		}
		else
		{
			pParentWnd = pParentWnd->GetParent ();
		}

		if (pParentWnd == NULL)
		{
			return NULL;
		}
		if (pParentWnd->IsFrameWnd ())
		{
			return (CFrameWnd*)pParentWnd;
		}
	}

	return NULL;
}

BOOL MySEC3DTabWnd::SetActiveTab (int iTab)
{
	// Docking 되었을 때와 Floating 되었을 때를 구별하여 적용해줌... 
	CWnd* pWndActive = GetActiveWnd ();
	CView* pActiveView = DYNAMIC_DOWNCAST (CView, pWndActive);
	if (pActiveView != NULL)
	{
		CFrameWnd* pFrame = MyBCGPGetParentFrame (pActiveView);
		ASSERT_VALID (pFrame);

		if (!pFrame->IsKindOf (RUNTIME_CLASS (CBCGPMiniFrameWnd)))
			return MBaseTabWnd::SetActiveTab(iTab);
	}
	else
	{
		return MBaseTabWnd::SetActiveTab(iTab);
	}

	// CMyTabWndBase::SetActiveTab(iTab)를 조금 고쳐서 씀... 
	//

	if (iTab < 0 || iTab >= m_iTabsNum)
	{
		TRACE(_T("SetActiveTab: illegal tab number %d\n"), iTab);
		return FALSE;
	}

	if (iTab >= m_arTabs.GetSize ())
	{
		ASSERT (FALSE);
		return FALSE;
	}

	BOOL bIsFirstTime = (m_iActiveTab == -1);

	if (m_iActiveTab == iTab)	// Already active, do nothing
	{
		return TRUE;
	}

	CWnd* pWndParent = GetParent ();
	ASSERT_VALID (pWndParent);

	if (_BCGCBPRO_VERSION_MAJOR >= 6 && _BCGCBPRO_VERSION_MINOR >= 2)
	{
		if (pWndParent->SendMessage (BCGM_CHANGING_ACTIVE_TAB, iTab, (LPARAM) this))
		{
			return TRUE;
		}
	}
	else
	{
		pWndParent->SendMessage (BCGM_CHANGING_ACTIVE_TAB, iTab, (LPARAM) this);
	}
	

	CBCGPMDIFrameWnd* pParentFrame = DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, GetParentFrame ());
	BOOL bEnableSetRedraw = TRUE;

	if (pParentFrame != NULL)
	{
		//bEnableSetRedraw = !pParentFrame->m_bClosing && !CBCGPMDIFrameWnd::m_bDisableSetRedraw;
	}

	if (m_iTabsNum > 1 && bEnableSetRedraw)
	{
		pWndParent->SetRedraw (FALSE);
	}

	if (m_iActiveTab != -1 && m_bHideInactiveWnd)
	{
		//--------------------
		// Hide active window:
		//--------------------
		CWnd* pWndActive = GetActiveWnd ();
		if (pWndActive != NULL)
		{
			pWndActive->ShowWindow (SW_HIDE);
		}
	}

	m_iActiveTab = iTab;
	
	//------------------------
	// Show new active window:
	//------------------------
	HideActiveWindowHorzScrollBar ();

	pWndActive = GetActiveWnd ();
	if (pWndActive == NULL)
	{
		ASSERT (FALSE);
		pWndParent->SetRedraw (TRUE);
		return FALSE;
	}

	ASSERT_VALID (pWndActive);

	pWndActive->ShowWindow (SW_SHOW);
	if (!m_bHideInactiveWnd)
	{
		pWndActive->BringWindowToTop ();
	}

	if (m_bAutoSizeWindow)
	{
		//----------------------------------------------------------------------
		// Small trick: to adjust active window scroll sizes, I should change an
		// active window size twice (+1 pixel and -1 pixel):
		//----------------------------------------------------------------------
		pWndActive->SetWindowPos (NULL,
				-1, -1,
				m_rectWndArea.Width () + 1, m_rectWndArea.Height (),
				SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
		pWndActive->SetWindowPos (NULL,
				-1, -1,
				m_rectWndArea.Width (), m_rectWndArea.Height (),
				SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
	}

	EnsureVisible (m_iActiveTab);

	if (m_bFlat)
	{
		SynchronizeScrollBar ();
	}

	//--------------------------------------------------
	// Set text to the parent frame/docking control bar:
	//--------------------------------------------------
	CBCGPTabbedControlBar* pTabControlBar = 
		DYNAMIC_DOWNCAST (CBCGPTabbedControlBar, GetParent ());
	if (pTabControlBar != NULL && pTabControlBar->CanSetCaptionTextToTabName ()) // tabbed dock bar - redraw caption only in this case
	{
		CString strCaption;
		GetTabLabel (m_iActiveTab, strCaption);

		// miniframe will take the text from the tab control bar
		pTabControlBar->SetWindowText (strCaption);

		CWnd* pWndToUpdate = pTabControlBar;
		if (!pTabControlBar->IsDocked ())
		{
			pWndToUpdate = pTabControlBar->GetParent ();
		}

		if (pWndToUpdate != NULL)
		{
			pWndToUpdate->RedrawWindow (NULL, NULL, RDW_FRAME | RDW_INVALIDATE);
		}
	}
	
	if (m_bIsActiveTabBold)
	{
		RecalcLayout ();
	}

	//-------------
	// Redraw tabs:
	//-------------
	Invalidate ();
	UpdateWindow ();

	if (!bIsFirstTime)
	{
		CView* pActiveView = DYNAMIC_DOWNCAST (CView, pWndActive);
		if (pActiveView != NULL)
		{
			CFrameWnd* pFrame = MyBCGPGetParentFrame (pActiveView);
			ASSERT_VALID (pFrame);

						// [MQC 9956] floating시 ActiveView를 SetActiveView를 할 때 이전 View의 pointer가 이상할 때가 있음(후처리에서 특히)
						// 그래서 NULL로 Setting을 하고 다시 ActiveView 를 호출함 - JWKWON-2015-07-13
						pFrame->SetActivePreviewView (NULL);
						pFrame->SetActiveView (pActiveView);
		}
		else if (m_bEnableActivate)
		{
			pWndActive->SetFocus ();
		}
	}

	if (m_btnClose.GetSafeHwnd () != NULL)
	{
		//----------------------------------------------------
		// Enable/disable "Close" button according to ability 
		// to close an active window:
		//----------------------------------------------------
		BOOL bEnableClose = TRUE;

		HMENU hSysMenu = pWndActive->GetSystemMenu (FALSE)->GetSafeHmenu ();
		if (hSysMenu != NULL)
		{
			MENUITEMINFO menuInfo;
			ZeroMemory(&menuInfo,sizeof(MENUITEMINFO));
			menuInfo.cbSize = sizeof(MENUITEMINFO);
			menuInfo.fMask = MIIM_STATE;

			if (!::GetMenuItemInfo (hSysMenu, SC_CLOSE, FALSE, &menuInfo) ||
				(menuInfo.fState & MFS_GRAYED) || 
				(menuInfo.fState & MFS_DISABLED))
			{
				bEnableClose = FALSE;
			}
		}

		m_btnClose.EnableWindow (bEnableClose);
	}
	
	if (m_iTabsNum > 1 && bEnableSetRedraw)
	{
		pWndParent->SetRedraw (TRUE);

		const UINT uiRedrawFlags =	RDW_INVALIDATE | RDW_UPDATENOW | 
									RDW_ERASE | RDW_ALLCHILDREN;

		if (m_bSetActiveTabByMouseClick)
		{
			CRect rectWindow;
			GetWindowRect (rectWindow);
			GetParent ()->ScreenToClient (rectWindow);

			pWndParent->RedrawWindow (rectWindow, NULL, uiRedrawFlags);
			pWndParent->RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_FRAME);
		}
		else
		{
			pWndParent->RedrawWindow (NULL, NULL, uiRedrawFlags);
		}
	}

	FireChangeActiveTab (m_iActiveTab);

	/*
	if (m_iActiveTab != -1 && pTabControlBar != NULL)
	{
		CBCGPBaseControlBar* pBar = 
			DYNAMIC_DOWNCAST(CBCGPBaseControlBar, GetTabWnd (m_iActiveTab));
		if (pBar != NULL)
		{
			CBCGPMiniFrameWnd* pParentMiniFrame = pBar->GetParentMiniFrame ();
			
			if (pBar->GetBCGStyle () & CBRS_BCGP_AUTO_ROLLUP)
			{
				pTabControlBar->m_dwBCGStyle |= CBRS_BCGP_AUTO_ROLLUP;
				if (pParentMiniFrame != NULL)
				{
					pParentMiniFrame->OnSetRollUpTimer ();
				}
			}
			else
			{
				pTabControlBar->m_dwBCGStyle &= ~CBRS_BCGP_AUTO_ROLLUP;
				if (pParentMiniFrame != NULL)
				{
					pParentMiniFrame->OnKillRollUpTimer ();
				}

			}
		}
	}
	*/

	return TRUE;
}

const int nMaxTabWidth = 90;
const int nIconWidth = 16;
const int nIconHeight = 16;
const int nPadding = 3;
const int nLeftOffset = 7;
const int nTopOffset = 7;
const int nDefaultHeight = 22;
const int nDefaultWidth = 22;
const int nSizingStart = 12;

void MySEC3DTabControl::SizeTabs(int nTotal, int nTotIcon, int nDimParam)
{	   	
	SEC3DTab* pTabCal;
	BOOL bSz1 = TRUE;
	//determine sizing method to use
	for(int nTab = 0; nTab < GetTabCount(); nTab++)
	{
		pTabCal = GetTabPtr(nTab);
		if(_tcslen(pTabCal->m_csLong) >= _tcslen(_T("...")))
			pTabCal->m_csLabel = _T("...");
	}

	int nTotalLoop   = 0;
	int nTotIconLoop = 0;
	CalcLabelWidth(nTotalLoop, nTotIconLoop, FALSE);

	if(nDimParam < (nTotalLoop+nSizingStart))
		bSz1 = FALSE;
	else
	{
		//reset tab labels and use primary sizing
		for(int nTab = 0; nTab < GetTabCount(); nTab++)
		{
			pTabCal = GetTabPtr(nTab);
			pTabCal->m_csLabel = pTabCal->m_csLong;
			//reset tooltips
			pTabCal->m_bToolTip = FALSE;
		}
	}

	if(nDimParam > nTotIcon)
		m_bShowTabLabels = TRUE;
	else
		m_bShowTabLabels = FALSE;

	//primary sizing
	while((nDimParam > 0) && (nTotal+nSizingStart > nDimParam) && bSz1)
	{
		for(int nTab = 0; nTab < GetTabCount(); nTab++)
		{
			SEC3DTab* pTab = GetTabPtr(nTab);
			if(_tcslen(pTab->m_csLabel) >= _tcslen(_T("...")) && (pTab->m_csLabel != _T("...")) )
				bSz1 = TRUE;
			else
				bSz1 = FALSE;
		}
		
		//truncate label and recheck
		SEC3DTab* pTabBig = GetTabPtr(GetTabBig());
		CString strTemp = pTabBig->m_csLabel; 
																								///////////////////////////////////////
		if(pTabBig->m_csLabel == pTabBig->m_csLong) //변경이 가해지지 않은 상태 
		{
			if(strTemp.Find(_T("...")))
			{
				int nTemp = static_cast<int>(_tcslen(strTemp)-4);
				if(nTemp < 0)
					nTemp = 0;
				pTabBig->m_csLabel = strTemp.Mid(0,nTemp);
			}
			else
			{
				if(strTemp.Find(_T("..")))
				{
					int nTemp = static_cast<int>(_tcslen(strTemp)-4);
					if(nTemp < 0)
						nTemp = 0;
					pTabBig->m_csLabel = strTemp.Mid(0,nTemp); 
				}
				else
				{
					int nTemp = static_cast<int>(_tcslen(strTemp)-3);
					if(nTemp < 0)
						nTemp = 0;
					pTabBig->m_csLabel = strTemp.Mid(0,nTemp);
				}
			}
		}
		else
		//Remove ellipsis & truncate
		{
			int cutoff = strTemp.Find(_T("..."));
			if(cutoff > 0)
			{
				int nTemp = static_cast<int>(_tcslen(strTemp)-3);
				if(nTemp < 0)
					nTemp = 0;
				pTabBig->m_csLabel = strTemp.Left(nTemp); 
			}

			cutoff = strTemp.Find(_T(".."));
			if(cutoff > 0)
			{
				int nTemp = static_cast<int>(_tcslen(strTemp)-3);
				if(nTemp < 0)
					nTemp = 0;
				pTabBig->m_csLabel = strTemp.Left(nTemp); 
			}
			
			int nTemp = static_cast<int>(_tcslen(pTabBig->m_csLabel)-1);
			if(nTemp < 0)
				nTemp = 0;
 			pTabBig->m_csLabel = pTabBig->m_csLabel.Mid(0,nTemp);

		}

		if(_tcslen(pTabBig->m_csLong) >= _tcslen(_T("...")))
			pTabBig->m_csLabel += _T("...");
		//set tooltip to be displayed
		pTabBig->m_bToolTip = TRUE;

		int nTotRecal = 0;
		int nTotIconRecal = 0;
		CalcLabelWidth(nTotRecal, nTotIconRecal, FALSE);


		nTotal = nTotRecal;
	}

	//secondary sizing
	while((nDimParam>0) && (nTotal+nSizingStart>nDimParam) && m_bShowTabLabels && (!bSz1))
	{
		for(int nTab = 0; nTab < GetTabCount(); nTab++)
		{
			SEC3DTab* pTab = GetTabPtr(nTab);
			pTab->m_bToolTip = TRUE;
			if(pTab->m_csLabel != _T(""))
				m_bShowTabLabels = TRUE;
			else
				m_bShowTabLabels = FALSE;
		}

		SEC3DTab* pTabBig = GetTabPtr(GetTabBig());
		CString strTemp = pTabBig->m_csLabel; 

		int nTemp = static_cast<int>(_tcslen(strTemp)-1);
		if(nTemp < 0)
			 nTemp = 0;

		pTabBig->m_csLabel = strTemp.Mid(0,nTemp);

		int nTotRecal = 0;
		int nTotIconRecal = 0;
		CalcLabelWidth(nTotRecal, nTotIconRecal, FALSE);

		nTotal = nTotRecal;
	}
}


// ExtraView.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "TestView.h"
//#include "LinkAll.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestView

IMPLEMENT_DYNCREATE(CTestView, CFormViewInBar)

CTestView::CTestView()
	: CFormViewInBar(CTestView::IDD)
{
	//{{AFX_DATA_INIT(CTestView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_btnEnd.SetMaskedBitmap(IDB_formviewend,20,20,RGB(255,255,255));
}
//" Test View String ";
CTestView::~CTestView()
{
}

void CTestView::DoDataExchange(CDataExchange* pDX)
{
	CFormViewInBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestView)
	DDX_Control(pDX, IDC_TM_EDIT1, m_wndCoord);
	DDX_Control(pDX, IDC_DlgExit, m_btnEnd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestView, CFormViewInBar)
	//{{AFX_MSG_MAP(CTestView)
	ON_BN_CLICKED(IDC_DlgExit, OnLCGExit)
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_TR_OK, OnTrOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestView diagnostics

#ifdef _DEBUG
void CTestView::AssertValid() const
{
	CFormViewInBar::AssertValid();
}

void CTestView::Dump(CDumpContext& dc) const
{
	CFormViewInBar::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestView message handlers
void CTestView::OnLCGExit() 
{
	//CLinkAll::SetMenuBarMenuMode();
	//((CFrameWnd*)AfxGetMainWnd())->GetActiveFrame()->SetFocus();
}

void CTestView::OnSize(UINT nType, int cx, int cy) 
{
	CFormViewInBar::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	_AlignEndButton();
}


void CTestView::_AlignEndButton()
{
	HWND hWnd = m_btnEnd.GetSafeHwnd();
	if(hWnd != NULL)
	{
		CRect rect;
		GetClientRect(&rect);
		::SetWindowPos(hWnd,NULL,rect.right-24,rect.top-2,24,22,SWP_NOZORDER|
		               SWP_NOACTIVATE);
	}
}

void CTestView::OnInitialUpdate() 
{
	CFormViewInBar::OnInitialUpdate();
	
	m_wndCoord.SetAttUcsPos();
	// TODO: Add your specialized code here and/or call the base class
	_AlignEndButton();
}

BOOL CTestView::PreTranslateMessage(MSG* pMsg) 
{
	
	if( pMsg->message == WM_KEYDOWN )
	{
		// When an item is being edited make sure the edit control
		// receives certain important key strokes
		if( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			//GetParentFrame()->SetFocus();
			((CFrameWnd*)AfxGetMainWnd())->GetActiveFrame()->SetFocus();
			return TRUE; // DO NOT process further
		}
	}
	// TODO: Add your specialized code here and/or call the base class
	return CFormViewInBar::PreTranslateMessage(pMsg);
}

LRESULT CTestView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CFormViewInBar::WindowProc(message, wParam, lParam);
}

BOOL CTestView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style &= ~(LONG)WS_VISIBLE;	
	return CFormViewInBar::PreCreateWindow(cs);
}

void CTestView::OnTrOk() 
{
	// TODO: Add your control notification handler code here
	AfxMessageBox(_T("OnOK"));	
}

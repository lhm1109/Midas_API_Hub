// MyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "MyListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


unsigned int CMyListCtrl::NLV_KEYUP     = RegisterWindowMessage(_T("NLV_KEYUP"));;
unsigned int CMyListCtrl::NLV_LBUTTONUP = RegisterWindowMessage(_T("NLV_LBUTTONUP"));;
/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl

CMyListCtrl::CMyListCtrl()
{

}

CMyListCtrl::~CMyListCtrl()
{

}


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMyListCtrl)
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl message handlers

void CMyListCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CListCtrl::OnKeyUp(nChar, nRepCnt, nFlags);

	if(nChar == VK_UP || nChar == VK_DOWN)
		GetParent()->SendMessage(NLV_KEYUP,(WPARAM)GetSafeHwnd(),0L);
}

void CMyListCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CListCtrl::OnLButtonUp(nFlags, point);
	
	GetParent()->SendMessage(NLV_LBUTTONUP,(WPARAM)GetSafeHwnd(),0L);  
}

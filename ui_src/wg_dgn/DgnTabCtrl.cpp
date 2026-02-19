// DgnTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "..\wg_sw\wg_sw.h"
#include "wg_dgn.h"
#include "DgnTabCtrl.h"

#include "..\wg_base\wg_base_InternationalDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDGNTabCtrl

CDGNTabCtrl::CDGNTabCtrl()
{
}

CDGNTabCtrl::~CDGNTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CDGNTabCtrl, CDlgTabCtrl)
	//{{AFX_MSG_MAP(CDGNTabCtrl)
	ON_NOTIFY_REFLECT_EX(TCN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDGNTabCtrl message handlers

int CDGNTabCtrl::AddTab(CDialog* pDlg, LPCTSTR szTabName, UINT nIDD, BOOL bDelete)
{
	int nItemCount = GetItemCount();
	int nTabCount = m_aDialog.GetSize();
	if (nItemCount == 0 && nTabCount > 0)
	{
		for (int i = 0; i < nTabCount; i++)
			if (m_aDeleteFlag[i]) delete m_aDialog[i];
		m_aDialog.RemoveAll();
		m_aDeleteFlag.RemoveAll();
	}

	TC_ITEM item;
	memset(&item, 0, sizeof(TC_ITEM));
	item.mask = TCIF_TEXT|TCIF_PARAM ;
	item.pszText = (TCHAR*)szTabName;
	item.lParam = (LPARAM)nIDD;
	if (InsertItem(nTabCount, &item) == -1) return -1;

	m_aDialog.Add(pDlg);
	m_aDeleteFlag.Add(bDelete);

	CInternationalDlg* pDialog;
	pDialog = (CInternationalDlg*)m_aDialog[nTabCount];
	pDialog->Create((UINT)item.lParam, this);
	pDialog->UpdateData(FALSE);
	CRect rect;
	GetDialogPos(rect);
	pDialog->MoveWindow(rect);
	pDialog->ShowWindow(SW_SHOW);

	return (nTabCount);
}

BOOL CDGNTabCtrl::ShowTab(int nPos)
{
	if(nPos < 0 || nPos >= m_aDialog.GetSize()) return FALSE;
	if(m_nCurTab >= 0) CloseCurTab();
	SetCurSel(nPos);
	ShowCurrentTab();
	return TRUE;
}

void CDGNTabCtrl::ShowCurrentTab()
{
	CRect rect;
	GetDialogPos(rect);
	m_nCurTab = GetCurSel();

	TC_ITEM item;
	memset(&item, 0, sizeof(TC_ITEM));
	item.mask = TCIF_TEXT | TCIF_PARAM;
	GetItem(m_nCurTab, &item);
	
	CInternationalDlg* pDialog;
	for(int i=0; i<m_aDialog.GetSize(); i++)
	{
		pDialog = (CInternationalDlg*)m_aDialog[i];
		if(i==m_nCurTab)
		{
			pDialog->MoveWindow(rect);
			pDialog->ShowWindow(SW_SHOW);
		}
		else	pDialog->ShowWindow(SW_HIDE);
	}
}

BOOL CDGNTabCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ShowCurrentTab();

	*pResult = 0;
	return FALSE;
}


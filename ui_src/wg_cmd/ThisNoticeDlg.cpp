// ThisNoticeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ThisNoticeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThisNoticeDlg dialog


CThisNoticeDlg::CThisNoticeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThisNoticeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThisNoticeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CThisNoticeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThisNoticeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CThisNoticeDlg, CDialog)
	//{{AFX_MSG_MAP(CThisNoticeDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThisNoticeDlg message handlers

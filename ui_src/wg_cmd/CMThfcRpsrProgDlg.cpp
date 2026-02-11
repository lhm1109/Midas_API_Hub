// CMThfcRpsrProgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"

#include "wg_cmdRes2.h"

#include "CMThfcRpsrProgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMThfcRpsrProgDlg dialog


CCMThfcRpsrProgDlg::CCMThfcRpsrProgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMThfcRpsrProgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMThfcRpsrProgDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMThfcRpsrProgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThfcRpsrProgDlg)
	DDX_Control(pDX, IDC_CMD_PROGRESS, m_Progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThfcRpsrProgDlg, CDialog)
	//{{AFX_MSG_MAP(CCMThfcRpsrProgDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThfcRpsrProgDlg message handlers

// CmdAutoLoadCombCvlKSCEUSD10Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombCvlKSCEUSD10Dlg.h"
#include "CmdAutoLoadCombCvlDlg.h"
#include "LoadCombCtrl.h"


#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlKSCEUSD10Dlg dialog
CCmdAutoLoadCombCvlKSCEUSD10Dlg::CCmdAutoLoadCombCvlKSCEUSD10Dlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCmdAutoLoadCombCvlKSCEUSD10Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdAutoLoadCombCvlKSCEUSD10Dlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_pParent = (CCmdAutoLoadCombCvlDlg*)pParent;

	m_bKSCEGravityLoadChk = FALSE;
}

void CCmdAutoLoadCombCvlKSCEUSD10Dlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdAutoLoadCombCvlKSCEUSD10Dlg)  
	DDX_Check  (pDX, IDC_CMD_KSCE_USD_10_GRAVLOAD_CHK,           m_bKSCEGravityLoadChk);
	DDX_Control(pDX, IDC_CMD_KSCE_USD_10_EFFLOAD_EDT,            m_edtKSCEEffLoadFactor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombCvlKSCEUSD10Dlg, CChildDialog)
	//{{AFX_MSG_MAP(CCmdAutoLoadCombCvlKSCEUSD10Dlg)
	ON_BN_CLICKED(IDC_CMD_KSCE_USD_10_GRAVLOAD_CHK,     OnCmdKSCEUSD10GRACHK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCmdAutoLoadCombCvlKSCEUSD10Dlg::GetKSCEUSD10LoadFactor()
{
	UpdateData(TRUE);

}

void CCmdAutoLoadCombCvlKSCEUSD10Dlg::OnCmdKSCEUSD10EnableDisable()
{  
	GetDlgItem(IDC_CMD_KSCE_USD_10_EFFLOAD_EDT)->EnableWindow(m_bKSCEGravityLoadChk);
}

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlKSCEUSD10Dlg message handlers
BOOL CCmdAutoLoadCombCvlKSCEUSD10Dlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	m_edtKSCEEffLoadFactor.SetEditUnit(0.5);

	UpdateData(FALSE);

	OnCmdKSCEUSD10EnableDisable();
	OnCmdKSCEUSD10GRACHK();

	return TRUE;
}

void CCmdAutoLoadCombCvlKSCEUSD10Dlg::OnCmdKSCEUSD10GRACHK() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CMD_KSCE_USD_10_EFFLOAD_EDT )->EnableWindow(m_bKSCEGravityLoadChk);
	GetDlgItem(IDC_CMD_KSCE_USD_10_EFFLOAD_STC )->EnableWindow(m_bKSCEGravityLoadChk);  
}
// CMStageChangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStageChangeDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_StagCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMStageChangeDlg dialog


CCMStageChangeDlg::CCMStageChangeDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMStageChangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMStageChangeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMStageChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMStageChangeDlg)
	DDX_Control(pDX, IDC_CMD_COBX_STAGE, m_cobxStage);
	//}}AFX_DATA_MAP
}

BOOL CCMStageChangeDlg::Apply()
{
	CStageTBarCombo::ChangeStageByStageComboBox(&m_cobxStage);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CCMStageChangeDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMStageChangeDlg)
	ON_BN_CLICKED(IDC_CMD_OK, OnCmdOk)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_CANCEL, OnCmdCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMStageChangeDlg message handlers
BOOL CCMStageChangeDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	CStageTBarCombo::SetStageComboBoxList(&m_cobxStage);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMStageChangeDlg::OnCmdOk() 
{
	// TODO: Add your control notification handler code here
	if (Apply() == FALSE) return;
	CDialogMove::OnOK();
}

void CCMStageChangeDlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	Apply();
}


void CCMStageChangeDlg::OnCmdCancel() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();
}

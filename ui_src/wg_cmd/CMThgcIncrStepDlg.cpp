// CMThgcIncrStepDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThgcIncrStepDlg.h"
#include "..\wg_base\DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMThgcIncrStepDlg dialog


CCMThgcIncrStepDlg::CCMThgcIncrStepDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMThgcIncrStepDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMThgcIncrStepDlg)
	m_aCtrl_ResultOutput.Add(IDC_GEOM_NONLINEAR_TYPE1_RDO);
	m_aCtrl_ResultOutput.Add(IDC_GEOM_NONLINEAR_TYPE2_RDO);
	//}}AFX_DATA_INIT
}

void CCMThgcIncrStepDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThgcIncrStepDlg)
	DDX_Control(pDX, IDC_INC_STEP_EDT, m_edtIncStep);
	DDX_Control(pDX, IDC_USER_STEP_EDT, m_edtUserInputStep);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMThgcIncrStepDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMThgcIncrStepDlg)
	ON_BN_CLICKED(IDC_GEOM_NONLINEAR_TYPE1_RDO, OnResultOutputRdo)
	ON_BN_CLICKED(IDC_GEOM_NONLINEAR_TYPE2_RDO, OnResultOutputRdo)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThgcIncrStepDlg message handlers

BOOL CCMThgcIncrStepDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_edtIncStep.SetRange(1, 1000);
	m_edtIncStep.SetInteger(TRUE);

	Data2Dlg();
	OnResultOutputRdo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThgcIncrStepDlg::Data2Dlg()
{
	m_edtIncStep.SetValue(m_ThgcD.nIncrementStep);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrl_ResultOutput, m_ThgcD.nIncrResult);
	m_edtUserInputStep.SetEditUnit(m_ThgcD.nIncrResultUserInputStep);
}

void CCMThgcIncrStepDlg::Dlg2Data()
{
	m_ThgcD.nIncrementStep = m_edtIncStep.GetEditValueInt();
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrl_ResultOutput, m_ThgcD.nIncrResult);
	m_ThgcD.nIncrResultUserInputStep = m_edtUserInputStep.GetEditValue();
}

void CCMThgcIncrStepDlg::OnOK()
{
	Dlg2Data();

	CDialogMove::OnOK();
}

void CCMThgcIncrStepDlg::OnResultOutputRdo()
{
	int nResultOutput=0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrl_ResultOutput, nResultOutput);
	m_edtUserInputStep.EnableWindow(nResultOutput==D_THGC_INC_OUTPUT_USER_INPUT_STEP);
}



// CMMvldItemEuroBSRailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldItemEuroBSRailDlg.h"

#include "..\wg_base\SpecialChar.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMMvldItemEuroBSRailDlg dialog
CCMMvldItemEuroBSRailDlg::CCMMvldItemEuroBSRailDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldItemEuroBSRailDlg::IDD)
{
	//{{AFX_DATA_INIT(CMMvldItemEuroBSRailDlg)
	//}}AFX_DATA_INIT

	m_dPsi1Fact1 = 0.8;
	m_dPsi1Fact2 = 0.7;
	m_dPsi1Fact3 = 0.6;
	m_bPsi1Fact  = FALSE;
	m_dMultiFact1 = 1.0;
	m_dMultiFact2 = 1.0;
	m_dMultiFact3 = 0.75;
}

CCMMvldItemEuroBSRailDlg::~CCMMvldItemEuroBSRailDlg()
{
}

void CCMMvldItemEuroBSRailDlg::DoDataExchange(CDataExchange* pDX)
{                        
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldItemEuroBSRailDlg)
	DDX_Control(pDX, IDC_CMD_RAIL_PSI1_FACTOR_1_EDT,        m_edtPsi1Fact1);
	DDX_Control(pDX, IDC_CMD_RAIL_PSI1_FACTOR_2_EDT,        m_edtPsi1Fact2);
	DDX_Control(pDX, IDC_CMD_RAIL_PSI1_FACTOR_3_EDT,        m_edtPsi1Fact3);
	DDX_Check  (pDX, IDC_CMD_RAIL_PSI1_FACTOR_CHK,          m_bPsi1Fact);
	DDX_Control(pDX, IDC_CMD_RAIL_MULTI_SCALE_FACTOR_1_EDT, m_edtMultiFact1);
	DDX_Control(pDX, IDC_CMD_RAIL_MULTI_SCALE_FACTOR_2_EDT, m_edtMultiFact2);
	DDX_Control(pDX, IDC_CMD_RAIL_MULTI_SCALE_FACTOR_3_EDT, m_edtMultiFact3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvldItemEuroBSRailDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldItemEuroBSRailDlg)
	ON_BN_CLICKED(IDC_CMD_RAIL_PSI1_FACTOR_CHK, OnCmdIgnorePsi1FactorChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvldItemEuroBSRailDlg::Data2Dlg()
{
	m_edtPsi1Fact1.SetEditUnit(m_dPsi1Fact1);
	m_edtPsi1Fact2.SetEditUnit(m_dPsi1Fact2);
	m_edtPsi1Fact3.SetEditUnit(m_dPsi1Fact3);

	m_edtMultiFact1.SetEditUnit(m_dMultiFact1);
	m_edtMultiFact2.SetEditUnit(m_dMultiFact2);
	m_edtMultiFact3.SetEditUnit(m_dMultiFact3);

	UpdateData(FALSE);
}

BOOL CCMMvldItemEuroBSRailDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_dPsi1Fact1 = m_edtPsi1Fact1.GetEditValue();
	m_dPsi1Fact2 = m_edtPsi1Fact2.GetEditValue();
	m_dPsi1Fact3 = m_edtPsi1Fact3.GetEditValue();

	m_dMultiFact1 = m_edtMultiFact1.GetEditValue();
	m_dMultiFact2 = m_edtMultiFact2.GetEditValue();
	m_dMultiFact3 = m_edtMultiFact3.GetEditValue();
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemEuroBSRailDlg message handlers

BOOL CCMMvldItemEuroBSRailDlg::OnInitDialog()
{               
	CDialogMove::OnInitDialog();

	CString strText;
	strText.Format(_LS(IDS_CMD_MVLD_EURO_PSI1_FACTOR), CSpecialChar::m_csPsi);
	GetDlgItem(IDC_CMD_RAIL_PSI1_FACTOR_STC)->SetWindowText(strText);

	strText.Format(_LS(IDS_CMD_MVLD_EURO_IGNORE_PSI1_FACTOR), CSpecialChar::m_csPsi);
	GetDlgItem(IDC_CMD_RAIL_PSI1_FACTOR_CHK)->SetWindowText(strText);

	strText.Format(_LS(IDS_CMD_MVLD_EURO_PSI_COMMENT), CSpecialChar::m_csPsi);
	GetDlgItem(IDC_CMD_RAIL_PSI_COMMENT_STC)->SetWindowText(strText);
	
	Data2Dlg();
	OnCmdIgnorePsi1FactorChk();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvldItemEuroBSRailDlg::OnOK()
{
	Dlg2Data();

	CDialogMove::OnOK();
}

void CCMMvldItemEuroBSRailDlg::OnCancel()
{
	CDialogMove::OnCancel();
}

void CCMMvldItemEuroBSRailDlg::OnCmdIgnorePsi1FactorChk()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_RAIL_PSI1_FACTOR_1_EDT)->EnableWindow(!m_bPsi1Fact);
	GetDlgItem(IDC_CMD_RAIL_PSI1_FACTOR_2_EDT)->EnableWindow(!m_bPsi1Fact);
	GetDlgItem(IDC_CMD_RAIL_PSI1_FACTOR_3_EDT)->EnableWindow(!m_bPsi1Fact);
}
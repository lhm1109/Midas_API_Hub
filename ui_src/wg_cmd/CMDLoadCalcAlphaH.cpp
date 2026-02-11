// CMDLoadCalcAlphaH.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMDLoadCalcAlphaH.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMDLoadCalcAlphaH dialog


CCMDLoadCalcAlphaH::CCMDLoadCalcAlphaH(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMDLoadCalcAlphaH::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMDLoadCalcAlphaH)
	m_dHeight = 0.0;
	m_nOption = -1;
	//}}AFX_DATA_INIT
}


void CCMDLoadCalcAlphaH::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMDLoadCalcAlphaH)
	DDX_Text(pDX, IDC_CMD_CALC_H, m_dHeight);
	DDX_Radio(pDX, IDC_CMD_CALC_ALPHA_H1, m_nOption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMDLoadCalcAlphaH, CDialogMove)
	//{{AFX_MSG_MAP(CCMDLoadCalcAlphaH)
	ON_BN_CLICKED(IDC_CMD_CALC_ALPHA_H1, OnCmdCalcAlphaH)
	ON_BN_CLICKED(IDC_CMD_CALC_ALPHA_H2, OnCmdCalcAlphaH)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMDLoadCalcAlphaH message handlers
BOOL CCMDLoadCalcAlphaH::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_nOption = 0;
	m_dHeight = 2.0;

	GetDlgItem(IDC_CMD_CALC_H_STR1)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_CALC_H_STR2)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_CALC_H_STR3)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_CALC_H)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_CMD_CALC_ALPHA_H_FRAME)->SetWindowText(_LS(IDS_CMD_CLAC_Alpha_H_Frame));
	GetDlgItem(IDC_CMD_CALC_ALPHA_H1)->SetWindowText(_LS(IDS_CMD_CLAC_Alpha_H_H1));
	GetDlgItem(IDC_CMD_CALC_H_STR3)->SetWindowText(_LS(IDS_CMD_CLAC_Alpha_H_H2));
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMDLoadCalcAlphaH::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if(m_nOption==0)
		m_dAlpha_H = 1.0;
	else
		m_dAlpha_H = max(1.05-0.025*m_dHeight, 0.875);
	
	CDialogMove::OnOK();
}



void CCMDLoadCalcAlphaH::OnCmdCalcAlphaH() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_CMD_CALC_H_STR1)->EnableWindow(m_nOption);
	GetDlgItem(IDC_CMD_CALC_H_STR2)->EnableWindow(m_nOption);
	GetDlgItem(IDC_CMD_CALC_H_STR3)->EnableWindow(m_nOption);
	GetDlgItem(IDC_CMD_CALC_H)->EnableWindow(m_nOption);
	
}

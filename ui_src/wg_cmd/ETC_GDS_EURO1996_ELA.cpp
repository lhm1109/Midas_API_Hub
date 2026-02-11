// ETC_GDS_EURO1996_ELA.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_EURO1996_ELA.h"

#include "SpfcEuro1996ELAUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_EURO1996_ELA dialog


CETC_GDS_EURO1996_ELA::CETC_GDS_EURO1996_ELA(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_EURO1996_ELA::IDD, pParent, std::make_shared<CSpfcEURO1996ELAUtil>())
{
	//{{AFX_DATA_INIT(CETC_GDS_EURO1996_ELA)
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcEURO1996ELAUtil>(m_pSpfcBaseUtil);

	m_wndSc = -1;
	m_dAg = 0.2;
	m_dDamping = 5.0;
	m_dMaxPeriod = PRD_ED;
	//}}AFX_DATA_INIT
}


void CETC_GDS_EURO1996_ELA::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_EURO1996_ELA)
	DDX_Radio(pDX, IDC_ETC_GDS_EURO1996_ELA_RDO_Soil, m_wndSc);
	DDX_Text(pDX, IDC_ETC_GDS_EURO1996_ELA_DAMPING_EDIT, m_dDamping);
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);	
	DDX_Text(pDX, IDC_ETC_GDS_EURO1996_ELA_AG_EDIT, m_dAg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_EURO1996_ELA, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_EURO1996_ELA)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_EURO1996_ELA message handlers

BOOL CETC_GDS_EURO1996_ELA::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	m_wndSc = 0;
	m_dDamping = 5.0;
	m_dAg = 0.2;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_EURO1996_ELA::UpdateDampingWnd(CWnd* pDampingWnd)
{
	CString csDamping;
	csDamping.Format(_T("%g"), m_dDamping/100.0);
	pDampingWnd->SetWindowText(csDamping);
}
	
void CETC_GDS_EURO1996_ELA::End()
{
	UpdateData(TRUE);

	m_pSpfcUtil->SetSc(m_wndSc);
	m_pSpfcUtil->SetParamFromCalc();
	Dlg2UtilData();

	CMyChildDialog::End();
}

void CETC_GDS_EURO1996_ELA::EndNotClose()
{
	UpdateData(TRUE);

	m_pSpfcUtil->SetSc(m_wndSc);
	m_pSpfcUtil->SetParamFromCalc();
	Dlg2UtilData();
}

BOOL CETC_GDS_EURO1996_ELA::ISVALID()
{   
	if(m_dAg <= 0)     {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__GROUND_ACC)); return FALSE;}
	if(m_dDamping < 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__DAMPING_VALUE)); return FALSE;}  
	if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  
	return TRUE;  
}

BOOL CETC_GDS_EURO1996_ELA::Dlg2Data()
{
	if(m_pParamData==NULL) return FALSE;
	
	m_pParamData->CodeParam.EURO1996_ELA.nSc         = m_wndSc;
	m_pParamData->CodeParam.EURO1996_ELA.dAg         = m_dAg;
	m_pParamData->CodeParam.EURO1996_ELA.dDamping    = m_dDamping;
	m_pParamData->CodeParam.EURO1996_ELA.dMaxPeriod  = m_dMaxPeriod;
	return TRUE;
}
void CETC_GDS_EURO1996_ELA::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	if(m_pParamData==NULL) return;
	m_wndSc      = m_pParamData->CodeParam.EURO1996_ELA.nSc       ;
	m_dAg        = m_pParamData->CodeParam.EURO1996_ELA.dAg       ;
	m_dDamping   = m_pParamData->CodeParam.EURO1996_ELA.dDamping  ;
	m_dMaxPeriod = m_pParamData->CodeParam.EURO1996_ELA.dMaxPeriod;
	if(bInit) UpdateData(FALSE);

	Dlg2UtilData();
}

void CETC_GDS_EURO1996_ELA::Dlg2UtilData()
{
	m_pSpfcUtil->SetSc(m_wndSc);
	m_pSpfcUtil->SetAg(m_dAg);
	m_pSpfcUtil->SetDamping(m_dDamping);
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);
}

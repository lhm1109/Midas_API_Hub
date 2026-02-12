// ETC_GDS_IBC2000.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_IBC2000.h"
#include "SpfcIBC2000Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_IBC2000 dialog


CETC_GDS_IBC2000::CETC_GDS_IBC2000(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_IBC2000::IDD, pParent, std::make_shared<CSpfcIBC2000Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_IBC2000)
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcIBC2000Util>(m_pSpfcBaseUtil);

	m_wndSDC = -1;
	m_wndSC = -1;
	m_sSs = _T("0.25");
	m_sS1 = _T("0.1");
	m_sI = _T("1.0");
	m_dR = 0.0;
	m_dMaxPeriod = PRD_ED;
	//}}AFX_DATA_INIT
	m_wndSDC = 0;
	m_wndSC  = 0;
	m_dR = 1.0;	
	SetValue();
}


void CETC_GDS_IBC2000::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_IBC2000)
	DDX_Control(pDX, IDD_ETC_GDS_IBC2000_I_CMB, m_wndI);
	DDX_Control(pDX, IDC_ETC_GDS_IBC2000_S1_CMB, m_wndS1);
	DDX_Control(pDX, IDC_ETC_GDS_IBC2000_SS_CMB, m_wndSs);
	DDX_Radio(pDX, IDD_ETC_GDS_IBC2000_SDC_RDO1, m_wndSDC);
	DDX_Radio(pDX, IDD_ETC_GDS_IBC2000_SC_RDO1, m_wndSC);
	DDX_CBString(pDX, IDC_ETC_GDS_IBC2000_SS_CMB, m_sSs);
	DDX_CBString(pDX, IDC_ETC_GDS_IBC2000_S1_CMB, m_sS1);
	DDX_CBString(pDX, IDD_ETC_GDS_IBC2000_I_CMB, m_sI);
	DDX_Text(pDX, IDD_ETC_GDS_IBC2000_R_EDIT, m_dR);
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_IBC2000, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_IBC2000)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_IBC2000 message handlers

BOOL CETC_GDS_IBC2000::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	((CButton*)GetDlgItem(IDC_ETC_GDS_IBC2000_I_STATIC))->ShowWindow(FALSE);  
	((CButton*)GetDlgItem(IDC_ETC_GDS_IBC2000_R_STATIC))->ShowWindow(FALSE);  
	((CButton*)GetDlgItem(IDD_ETC_GDS_IBC2000_R_EDIT))->ShowWindow(FALSE);  
	m_wndI.ShowWindow(FALSE);  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_IBC2000::End()
{
	UpdateData(TRUE);
	SetValue();
	CMyChildDialog::End();
}

//-----------------------------------------------------------------------------------------------------
// INIT : 2003.09.20. by KYE-HONG
// FUNC : 계산알고리즘에 사용되는 값을 Setting함. 
//-----------------------------------------------------------------------------------------------------
void CETC_GDS_IBC2000::SetValue()
{
	m_pSpfcUtil->SetSs(_tstof(m_sSs));
	m_pSpfcUtil->SetS1(_tstof(m_sS1));
	m_pSpfcUtil->SetI(_tstof(m_sI));

	m_pSpfcUtil->SetWndSC(m_wndSC);
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);

	m_pSpfcUtil->SetValue();
}

BOOL CETC_GDS_IBC2000::ISVALID()
{  
	if(!IsValidCmb_d(&m_wndSs,0,10,1.0)) return FALSE;
	if(!IsValidCmb_d(&m_wndS1,0,50,1.0)) return FALSE;
	if(!IsValidCmb_d(&m_wndI,0,50,1.0)) return FALSE;
	if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  
	return TRUE;  
}

BOOL CETC_GDS_IBC2000::Dlg2Data()
{
	m_pParamData->CodeParam.IBC2000.nSc = m_wndSC;
	m_pParamData->CodeParam.IBC2000.dSs = m_pSpfcUtil->GetSs();
	m_pParamData->CodeParam.IBC2000.dS1 = m_pSpfcUtil->GetS1();

	m_pParamData->CodeParam.IBC2000.dMaxPeriod = m_dMaxPeriod;
	return TRUE;
}
void CETC_GDS_IBC2000::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	m_wndSC      = m_pParamData->CodeParam.IBC2000.nSc;
	m_pSpfcUtil->SetSs(m_pParamData->CodeParam.IBC2000.dSs);
	m_pSpfcUtil->SetS1(m_pParamData->CodeParam.IBC2000.dS1);
	m_dMaxPeriod = m_pParamData->CodeParam.IBC2000.dMaxPeriod;

	m_pSpfcUtil->SetWndSC(m_wndSC);
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);
	
	CString strSs, strS1;
	strSs.Format(_T("%g"), m_pSpfcUtil->GetSs());
	strS1.Format(_T("%g"), m_pSpfcUtil->GetS1());
	
	m_sSs = strSs;
	m_sS1 = strS1;
	m_sI = _T("1.0");
	SetValue();

	if(bInit)
	{
		UpdateData(FALSE);
	
		m_wndSs.SetWindowText(strSs);
		m_wndS1.SetWindowText(strS1);
		GetDlgItem(IDD_ETC_GDS_IBC2000_R_EDIT)->EnableWindow(FALSE);
	}
}
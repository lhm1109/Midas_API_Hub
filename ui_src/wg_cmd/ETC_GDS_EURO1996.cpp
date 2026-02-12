// ETC_GDS_EURO1996.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_EURO1996.h"

#include "SpfcEURO1996Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_EURO1996 dialog


CETC_GDS_EURO1996::CETC_GDS_EURO1996(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_EURO1996::IDD, pParent, std::make_shared<CSpfcEURO1996Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_EURO1996)
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcEURO1996Util>(m_pSpfcBaseUtil);
	m_wndSc = -1;
	m_wndKd = -1;
	m_wndKr = -1;
	m_dKw = 0.0;
	m_dAlpha = 0.0;
	m_dMaxPeriod = PRD_ED;
		
	//}}AFX_DATA_INIT
}


void CETC_GDS_EURO1996::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_EURO1996)
	DDX_Control(pDX, IDD_ETC_GDS_EURO1996_Q0_CMB, m_wndQ0);
	DDX_Radio(pDX, IDD_ETC_GDS_EURO1996_RDO_Soil, m_wndSc);
	DDX_Radio(pDX, IDD_ETC_GDS_EURO1996_RDO_DUCTLE, m_wndKd);
	DDX_Radio(pDX, IDD_ETC_GDS_EURO1996_RDO_REGULAR, m_wndKr);
	DDX_Text(pDX, IDD_ETC_GDS_EURO1996_KW_EDIT, m_dKw);
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);	
	DDX_Text(pDX, IDD_ETC_GDS_EURO1996_ALPHA_EDIT, m_dAlpha);
	//}}AFX_DATA_MAP	
}


BEGIN_MESSAGE_MAP(CETC_GDS_EURO1996, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_EURO1996)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_EURO1996 message handlers

BOOL CETC_GDS_EURO1996::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndQ0.AddString(_T("5.0"));
	m_wndQ0.AddString(_T("4.5"));
	m_wndQ0.AddString(_T("4.0"));
	m_wndQ0.AddString(_T("3.5"));
	m_wndQ0.AddString(_T("2.0"));
	
	m_wndQ0.SetCurSel(0);
	
	m_wndSc = 0;
	m_wndKd = 0;
	m_wndKr = 0;

	m_dKw = 1.00;
	m_dAlpha = 0.2;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
	
void CETC_GDS_EURO1996::End()
{
	UpdateData(TRUE);

	Dlg2UtilData();
 	SetValue();

	CString str;
	m_wndQ0.GetWindowText(str);
	m_pSpfcUtil->SetQ0(_tstof(str));

	CMyChildDialog::End();
}

void CETC_GDS_EURO1996::EndNotClose()
{
	UpdateData(TRUE);

	Dlg2UtilData();
	SetValue();

	
	CString str;
	m_wndQ0.GetWindowText(str);
	m_pSpfcUtil->SetQ0(_tstof(str));
}

void CETC_GDS_EURO1996::SetValue()
{
	m_pSpfcUtil->SetSc(m_wndSc);
	m_pSpfcUtil->SetKd(m_wndKd);
	m_pSpfcUtil->SetKr(m_wndKr);

	m_pSpfcUtil->SetParamFromCalc();
}

BOOL CETC_GDS_EURO1996::ISVALID()
{  
	if(!IsValidCmb_d(&m_wndQ0,0,10,1.0)) return FALSE;
	if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  
	if(m_dKw <= 0)    {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__FAILURE_MODE_FACTOR)); return FALSE;}  
	if(m_dAlpha <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__GROUND_ACC_RATIO)); return FALSE;}  
	return TRUE;  
}

BOOL CETC_GDS_EURO1996::Dlg2Data()
{
	m_pParamData->CodeParam.EURO1996.nSc    = m_wndSc;
	m_pParamData->CodeParam.EURO1996.nKd    = m_wndKd;
	m_pParamData->CodeParam.EURO1996.nKr    = m_wndKr;
	m_pParamData->CodeParam.EURO1996.dQ0    = m_pSpfcUtil->GetQ0();
	m_pParamData->CodeParam.EURO1996.dKw    = m_dKw;
	m_pParamData->CodeParam.EURO1996.dAlpha = m_dAlpha;

	m_pParamData->CodeParam.EURO1996.dMaxPeriod = m_dMaxPeriod;

	return TRUE;
}
void CETC_GDS_EURO1996::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	m_wndSc  = m_pParamData->CodeParam.EURO1996.nSc   ;
	m_wndKd  = m_pParamData->CodeParam.EURO1996.nKd   ;
	m_wndKr  = m_pParamData->CodeParam.EURO1996.nKr   ;
	m_pSpfcUtil->SetQ0(m_pParamData->CodeParam.EURO1996.dQ0);
	m_dKw    = m_pParamData->CodeParam.EURO1996.dKw   ;
	m_dAlpha = m_pParamData->CodeParam.EURO1996.dAlpha;

	m_dMaxPeriod = m_pParamData->CodeParam.EURO1996.dMaxPeriod;

	Dlg2UtilData();
	SetValue();

	if(bInit)
	{
		UpdateData(FALSE);

		CString str;
		str.Format(_T("%g"), m_pSpfcUtil->GetQ0());
		m_wndQ0.SetWindowText(str); 
	}

}

void CETC_GDS_EURO1996::Dlg2UtilData()
{
	m_pSpfcUtil->SetSc(m_wndSc);
	m_pSpfcUtil->SetKd(m_wndKd);
	m_pSpfcUtil->SetKr(m_wndKr);
	m_pSpfcUtil->SetKw(m_dKw);
	m_pSpfcUtil->SetAlpha(m_dAlpha);
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);
}

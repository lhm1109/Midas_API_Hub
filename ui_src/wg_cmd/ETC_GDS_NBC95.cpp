// ETC_GDS_NBC95.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_NBC95.h"

#include "SpfcNBC95Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_NBC95 dialog


CETC_GDS_NBC95::CETC_GDS_NBC95(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_NBC95::IDD, pParent, std::make_shared<CSpfcNBC95Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_NBC95)
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcNBC95Util>(m_pSpfcBaseUtil);
	m_dMaxPeriod = PRD_ED;
	//}}AFX_DATA_INIT
}


void CETC_GDS_NBC95::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_NBC95)
	DDX_Control(pDX, IDC_CMD_GDS_NBC95_ZONE_RATIO_CMB, m_wndZoneRatio);
	DDX_Control(pDX, IDC_CMD_GDS_NBC95_VELO_ZONE_CMB, m_wndVeloZone);
	DDX_Control(pDX, IDC_CMD_GDS_NBC95_MODIFICATION_CMB, m_wndModification);
	DDX_Control(pDX, IDC_CMD_GDS_NBC95_IMPORTANCE_CMB, m_wndImportance);
	DDX_Control(pDX, IDC_CMD_GDS_NBC95_FOUNDATION_CMB, m_wndFoundation);
	DDX_Control(pDX, IDC_CMD_GDS_NBC95_ACCE_ZONE_CMB, m_wndAcceZone);
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_NBC95, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_NBC95)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_NBC95 message handlers

BOOL CETC_GDS_NBC95::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	CString aZoneRatio[] = {_T("0.00"),_T("0.05"),_T("0.10"),_T("0.15"),_T("0.20"),_T("0.30"),_T("0.40")};
	CString aAcceVelo[]  = {_T("0"),_T("1"),_T("2"),_T("3"),_T("4"),_T("5"),_T("6")};  
	CString aIfator[]    = {_T("1.0"),_T("1.3"),_T("1.5")};
	CString aFfator[]    = {_T("1.0"),_T("1.3"),_T("1.5"),_T("2.0")};
	CString aRfator[]    = {_T("1.0"),_T("1.5"),_T("2.0"),_T("3.0"),_T("3.5"),_T("4.0")};
	for(int i = 0; i < 7; i++)
	{
		m_wndZoneRatio.AddString(aZoneRatio[i]);
		m_wndAcceZone.AddString(aAcceVelo[i]);
		m_wndVeloZone.AddString(aAcceVelo[i]);
	}
	for(int i = 0; i < 3; i++)  m_wndImportance.AddString(aIfator[i]);
	for(int i = 0; i < 4; i++)  m_wndFoundation.AddString(aFfator[i]);
	for(int i = 0; i < 6; i++)  m_wndModification.AddString(aRfator[i]);
	m_wndZoneRatio.SetCurSel(1);
	m_wndAcceZone.SetCurSel(1);
	m_wndVeloZone.SetCurSel(1);
	m_wndImportance.SetCurSel(0);
	m_wndFoundation.SetCurSel(0);
	m_wndModification.SetCurSel(0);
	((CButton*)GetDlgItem(IDC_CMD_NBC95_IMPORTANCE_STATIC))->ShowWindow(FALSE);  
	((CButton*)GetDlgItem(IDC_CMD_NBC95_FOUNDATION_STATIC))->ShowWindow(FALSE);  
	((CButton*)GetDlgItem(IDC_CMD_GDS_NBC95_FOUNDATION_STATIC))->ShowWindow(FALSE);
	m_wndImportance.ShowWindow(FALSE);
	m_wndFoundation.ShowWindow(FALSE);
	m_wndModification.ShowWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_NBC95::End()
{
	UpdateData(TRUE);
	CString str;
	m_wndZoneRatio.GetWindowText(str);    m_pSpfcUtil->SetV(_tstof(str));
	m_wndImportance.GetWindowText(str);   m_pSpfcUtil->SetI(_tstof(str));
	m_wndFoundation.GetWindowText(str);   m_pSpfcUtil->SetF(_tstof(str));
	m_wndModification.GetWindowText(str); m_pSpfcUtil->SetR(_tstof(str)); 
	m_pSpfcUtil->SetZa(m_wndAcceZone.GetCurSel());
	m_pSpfcUtil->SetZv(m_wndVeloZone.GetCurSel());

	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);
	CMyChildDialog::End();
}

BOOL CETC_GDS_NBC95::ISVALID()
{  
	if(!IsValidCmb_d(&m_wndZoneRatio,0,10,1.0)) return FALSE;
	if(!IsValidCmb_d(&m_wndImportance,0,10,1.0)) return FALSE;
	if(!IsValidCmb_d(&m_wndFoundation,0,10,1.0)) return FALSE;
	if(!IsValidCmb_d(&m_wndModification,0,10,1.0)) return FALSE;
	if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  
	return TRUE;  
}
BOOL CETC_GDS_NBC95::Dlg2Data()
{
	m_pParamData->CodeParam.NBC95.dV    = m_pSpfcUtil->GetV();
	m_pParamData->CodeParam.NBC95.nZa   = static_cast<int>(m_pSpfcUtil->GetZa());
	m_pParamData->CodeParam.NBC95.nZv   = static_cast<int>(m_pSpfcUtil->GetZv());
	m_pParamData->CodeParam.NBC95.dMaxPeriod = m_dMaxPeriod;
	return TRUE;
}
void CETC_GDS_NBC95::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	m_pSpfcUtil->SetV(m_pParamData->CodeParam.NBC95.dV);
	m_pSpfcUtil->SetZa(m_pParamData->CodeParam.NBC95.nZa);
	m_pSpfcUtil->SetZv(m_pParamData->CodeParam.NBC95.nZv);

	m_dMaxPeriod  = m_pParamData->CodeParam.NBC95.dMaxPeriod;
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);

	if(bInit)
	{
		CString str;
		str.Format(_T("%g"), m_pSpfcUtil->GetV());
		m_wndZoneRatio.SetWindowText(str);
		m_wndAcceZone.SetCurSel(m_pParamData->CodeParam.NBC95.nZa);
		m_wndVeloZone.SetCurSel(m_pParamData->CodeParam.NBC95.nZv);

		UpdateData(FALSE);
	}
}

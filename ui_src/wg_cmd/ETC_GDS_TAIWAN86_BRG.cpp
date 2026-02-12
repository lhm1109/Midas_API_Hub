// ETC_GDS_TAIWAN86_BRG.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_TAIWAN86_BRG.h"

#include "SpfcTWBR89HUtil.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_TAIWAN86_BRG dialog


CETC_GDS_TAIWAN86_BRG::CETC_GDS_TAIWAN86_BRG(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_TAIWAN86_BRG::IDD, pParent, std::make_shared<CSpfcTWBR89HUtil>())
{
	//{{AFX_DATA_INIT(CETC_GDS_TAIWAN86_BRG)
	m_nSoil = 0;
	m_nZone = 0;
	m_dMaxPeriod = PRD_ED;
	//}}AFX_DATA_INIT
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcTWBR89HUtil>(m_pSpfcBaseUtil);
}


void CETC_GDS_TAIWAN86_BRG::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_TAIWAN86_BRG)
	DDX_Radio(pDX, IDC_ETC_GDS_ZONE_RDO, m_nZone);
	DDX_Radio(pDX, IDC_ETC_GDS_SOIL_RDO, m_nSoil);
	DDX_Control(pDX, IDC_ETC_GDS_DAMPING, m_wndDamping);
	DDX_Control(pDX, IDC_ETC_GDS_ALPHA_CMB, m_wndAlphaCmb);
	DDX_Control(pDX, IDC_ETC_GDS_IMPORTANCE_CMB, m_wndImportanceCmb);
	DDX_Control(pDX, IDC_ETC_GDS_RESPONSE_CMB, m_wndRCmb);	
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_TAIWAN86_BRG, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_TAIWAN86_BRG)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_TAIWAN86_BRG message handlers

BOOL CETC_GDS_TAIWAN86_BRG::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	  	
	m_wndDamping.SetEditUnit(0.05);  
		
	CString strImportance[] = {_T("1.0"),_T("1.2")};
	CString strResponse[]   = {_T("2.0"),_T("3.0"),_T("5.0")};
	CString strAlpha[]      = {_T("1.65"),_T("1.70"),_T("1.90")}; 
	for(int i = 0; i < 2; i++)  m_wndImportanceCmb.AddString(strImportance[i]);
	for(int i = 0; i < 3; i++)  m_wndRCmb.AddString(strResponse[i]);  
	for(int i = 0; i < 3; i++)  m_wndAlphaCmb.AddString(strAlpha[i]);  
	
	m_wndImportanceCmb.SetCurSel(0);
	m_wndRCmb.SetCurSel(0);  
	m_wndAlphaCmb.SetCurSel(0);
		 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_TAIWAN86_BRG::UpdateDampingWnd(CWnd* pDampingWnd)
{
	CString csDamping;
	double dDamping = m_wndDamping.GetEditValue();
	csDamping.Format(_T("%g"), dDamping);
	pDampingWnd->SetWindowText(csDamping);
}
	
void CETC_GDS_TAIWAN86_BRG::End()
{
	UpdateData(TRUE);
	Dlg2UtilData();

	CMyChildDialog::End();
}

BOOL CETC_GDS_TAIWAN86_BRG::ISVALID()
{  
	if(!IsValidCmb_d(&m_wndImportanceCmb,0,10,1.0)) return FALSE;
	if(!IsValidCmb_d(&m_wndRCmb,0,10,1.0)) return FALSE;
	if(!IsValidCmb_d(&m_wndAlphaCmb,0,10,1.0)) return FALSE;
	const double dDamping = m_wndDamping.GetEditValue();  
	if(dDamping < 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__DAMPING_VALUE)); return FALSE;}
	if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  
	return TRUE;  
}

void CETC_GDS_TAIWAN86_BRG::SetPushOverData(BOOL bInit/*=TRUE*/)
{ 
	m_nZone          = m_pParamData->CodeParam.TAIWAN89H.nSeisZone;
	m_nSoil          = m_pParamData->CodeParam.TAIWAN89H.nSoilType ;
	m_dMaxPeriod     = m_pParamData->CodeParam.TAIWAN89H.dMaxPeriod ;
	const double dI             = m_pParamData->CodeParam.TAIWAN89H.dIe ;
	const double dR             = 1.0 ;
	const double dAlpha         = m_pParamData->CodeParam.TAIWAN89H.dAlpha ;

	m_pSpfcUtil->SetDamping(0.05);
	m_pSpfcUtil->SetRStar(2.0);
	m_pSpfcUtil->SetNumR(0);
	
	CString strI, strR, strAlpha;
	strI.Format(_T("%g"), dI);
	strR.Format(_T("%g"), dR);
	strAlpha.Format(_T("%g"), dAlpha);

	if(bInit)
	{
		UpdateData(FALSE);

		m_wndImportanceCmb.SetWindowText(strI);   
		m_wndRCmb.SetWindowText(strR);  
		m_wndAlphaCmb.GetWindowText(strAlpha);

		GetDlgItem(IDC_ETC_GDS_DAMPING_STATIC)->ShowWindow(FALSE);
		GetDlgItem(IDC_ETC_GDS_DAMPING)->ShowWindow(FALSE);
		CArray<UINT, UINT> aCtrlID;
		aCtrlID.Add(IDC_ETC_GDS_TAIWAN_R_STC );
		aCtrlID.Add(IDC_ETC_GDS_TAIWAN_AY_STC);
		aCtrlID.Add(IDC_ETC_GDS_RESPONSE_CMB );
		aCtrlID.Add(IDC_ETC_GDS_ALPHA_CMB    );
		CDlgUtil::CtrlShowHide(this, aCtrlID, FALSE);
	}

	m_pSpfcUtil->SetPOMode(true);
	Dlg2UtilData();
}

BOOL CETC_GDS_TAIWAN86_BRG::Dlg2Data()
{
	m_pParamData->CodeParam.TAIWAN89H.nSeisZone      = m_nZone         ;
	m_pParamData->CodeParam.TAIWAN89H.nSoilType      = m_nSoil         ;
	m_pParamData->CodeParam.TAIWAN89H.dIe = m_pSpfcUtil->GetI();
	m_pParamData->CodeParam.TAIWAN89H.dAlpha = m_pSpfcUtil->GetAlpha();
	m_pParamData->CodeParam.TAIWAN89H.dMaxPeriod     = m_dMaxPeriod    ;
	m_pParamData->CodeParam.TAIWAN89H.dCoef = m_pSpfcUtil->GetRStar();
	m_pParamData->CodeParam.TAIWAN89H.dDamping = m_pSpfcUtil->GetDamping();

	m_pParamData->dResModCoef = m_pSpfcUtil->GetR();
	return TRUE;
}

void CETC_GDS_TAIWAN86_BRG::Dlg2UtilData()
{
	CString strI, strR, strAlpha;
	m_wndImportanceCmb.GetWindowText(strI);
	m_wndRCmb.GetWindowText(strR);
	m_wndAlphaCmb.GetWindowText(strAlpha);

	m_pSpfcUtil->SetZone(m_nZone);
	m_pSpfcUtil->SetSoil(m_nSoil);
	m_pSpfcUtil->SetRStar(_tstof(strR));
	m_pSpfcUtil->SetI(_tstof(strI));
	m_pSpfcUtil->SetAlpha(_tstof(strAlpha));
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);
	m_pSpfcUtil->SetDamping(m_wndDamping.GetEditValue());

	double dR = m_pSpfcUtil->GetRStar() / 1.2;
	if (m_pSpfcUtil->GetPOMode() == true)
		dR = 1.0;

	m_pSpfcUtil->SetNumR(m_wndRCmb.GetCurSel());
	m_pSpfcUtil->SetR(dR);
	m_pSpfcUtil->SetZ(TWBR89H::ZONE[m_nZone]);

}

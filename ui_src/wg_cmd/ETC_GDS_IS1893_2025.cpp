// ETC_GDS_IS1893_2025.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_IS1893_2025.h"
#include "..\wg_base\DlgUtil.h"


#include "SpfcIS2025Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_IS1893_2025 dialog


CETC_GDS_IS1893_2025::CETC_GDS_IS1893_2025(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_IS1893_2025::IDD, pParent, std::make_shared<CSpfcIS2025Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_IS1893_2025)
	m_dMaxPeriod = 10.0;
	m_dZoneFact = 0.0375;
	//}}AFX_DATA_INIT  
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcIS2025Util>(m_pSpfcBaseUtil);
}

void CETC_GDS_IS1893_2025::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_IS1893_2025)
	DDX_Control(pDX, IDC_ETC_GDS_IS2025_SPEC_TYPE_CMB, m_cmbSpecType);
	DDX_Control(pDX, IDC_ETC_GDS_IS2025_EQ_ZONE_CMB, m_cmbEqZone);
	DDX_Control(pDX, IDC_ETC_GDS_IS2025_RET_PERIOD_CMB, m_cmbRetPeriod);
	DDX_Control(pDX, IDC_ETC_GDS_IS2025_SITE_CLASS_CMB, m_cmbSiteClass);
	DDX_Control(pDX, IDC_ETC_GDS_IS2025_DAMPING_EDT, m_edtDamping);
	DDX_Control(pDX, IDC_ETC_GDS_IS2025_EQ_ZONE_FACT_EDT, m_wndZoneFact);
	DDX_Control(pDX, IDC_ETC_GDS_IS2025_IMPORTANCE_CMB, m_cmbImportanceFact);
	DDX_Control(pDX, IDC_ETC_GDS_IS2025_RESP_RED_CMB, m_cmbRespReduFact);
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_IS1893_2025, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_IS1893_2025)
	ON_EN_CHANGE(IDC_ETC_GDS_IS2025_DAMPING_EDT, OnChangeEtcGdsIS1893_2016Damping)
	ON_CBN_SELCHANGE(IDC_ETC_GDS_IS2025_SPEC_TYPE_CMB, OnSpecTypeChange)
	ON_CBN_SELCHANGE(IDC_ETC_GDS_IS2025_EQ_ZONE_CMB, OnEqZoneChange)
	ON_CBN_SELCHANGE(IDC_ETC_GDS_IS2025_RET_PERIOD_CMB, OnRetPeriodChange)
	ON_EN_CHANGE(IDC_ETC_GDS_IS2025_EQ_ZONE_FACT_EDT, OnZoneFactorChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_IS1893_2025 message handlers
BOOL CETC_GDS_IS1893_2025::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	  	
	m_edtDamping.SetEditUnit(5.0);
	InitSpectrumTypeCombo();
	InitEqZoneCombo();
	InitRetPeriodCombo();
	InitSiteClassCombo();
	m_wndZoneFact.SetEditUnit(0.0375);
	
	CString strImportance[] = { _T("1.0"),_T("1.15") };
	CString strResponse[] = { _T("1.5"),_T("2.5"),_T("3.0"),_T("4.0"),_T("4.5"),_T("5.0"),_T("5.5") ,_T("6.0") };

	for(int i = 0; i < 2; i++)  m_cmbImportanceFact.AddString(strImportance[i]);
	for(int i = 0; i < 8; i++)  m_cmbRespReduFact.AddString(strResponse[i]);

	CString strImpFact; 
	strImpFact.Format(_T("%4.3f"), m_pSpfcUtil->GetImpFact());
	m_cmbImportanceFact.SetWindowTextW(strImpFact);		// 1.0
	m_cmbRespReduFact.SetCurSel(5);			// 5.0
		 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_IS1893_2025::Initialize()
{
	if(m_pParamData == NULL)
		return;

	m_pParamData->CodeParam.Initialize(D_SPFC_CODE_IS1893_2025);
}

void CETC_GDS_IS1893_2025::InitSpectrumTypeCombo()
{
	// 0: 老馆, 1: Elastic Sepctrum 昏力, 2: Design Spectrum 昏力
	m_cmbSpecType.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbSpecType, _LS(IDS_CMD_IS2025_H_SPECTRUM), 0);
	CDlgUtil::CobxAddItem(m_cmbSpecType, _LS(IDS_CMD_IS2025_V_SPECTRUM), 1);
	m_cmbSpecType.SetCurSel(0);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbSpecType);
}

void CETC_GDS_IS1893_2025::InitEqZoneCombo()
{
	// 0: 老馆, 1: Elastic Sepctrum 昏力, 2: Design Spectrum 昏力
	m_cmbEqZone.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbEqZone, _T("II"), EN_IS2025_EQ_ZONE_II);
	CDlgUtil::CobxAddItem(m_cmbEqZone, _T("III"), EN_IS2025_EQ_ZONE_III);
	CDlgUtil::CobxAddItem(m_cmbEqZone, _T("IV"), EN_IS2025_EQ_ZONE_IV);
	CDlgUtil::CobxAddItem(m_cmbEqZone, _T("V"), EN_IS2025_EQ_ZONE_V);
	CDlgUtil::CobxAddItem(m_cmbEqZone, _T("VI"), EN_IS2025_EQ_ZONE_VI);
	m_cmbEqZone.SetCurSel(0);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbEqZone);
}

void CETC_GDS_IS1893_2025::InitRetPeriodCombo()
{
	m_cmbRetPeriod.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbRetPeriod, _T("75"), EN_IS2025_RET_75);
	CDlgUtil::CobxAddItem(m_cmbRetPeriod, _T("175"), EN_IS2025_RET_175);
	CDlgUtil::CobxAddItem(m_cmbRetPeriod, _T("275"), EN_IS2025_RET_275);
	CDlgUtil::CobxAddItem(m_cmbRetPeriod, _T("475"), EN_IS2025_RET_475);
	CDlgUtil::CobxAddItem(m_cmbRetPeriod, _T("975"), EN_IS2025_RET_975);
	CDlgUtil::CobxAddItem(m_cmbRetPeriod, _T("1275"), EN_IS2025_RET_1275);
	CDlgUtil::CobxAddItem(m_cmbRetPeriod, _T("2475"), EN_IS2025_RET_2475);
	CDlgUtil::CobxAddItem(m_cmbRetPeriod, _T("4975"), EN_IS2025_RET_4975);
	CDlgUtil::CobxAddItem(m_cmbRetPeriod, _T("9975"), EN_IS2025_RET_9975);
	m_cmbRetPeriod.SetCurSel(0);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbRetPeriod);
}

void CETC_GDS_IS1893_2025::InitSiteClassCombo()
{
	m_cmbSiteClass.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbSiteClass, _T("A"), EN_IS2025_SITE_A);
	CDlgUtil::CobxAddItem(m_cmbSiteClass, _T("B"), EN_IS2025_SITE_B);
	CDlgUtil::CobxAddItem(m_cmbSiteClass, _T("C"), EN_IS2025_SITE_C);
	CDlgUtil::CobxAddItem(m_cmbSiteClass, _T("D"), EN_IS2025_SITE_D);
	m_cmbSiteClass.SetCurSel(0);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbSiteClass);
}

void CETC_GDS_IS1893_2025::UpdateDampingWnd(CWnd* pDampingWnd)
{
	CString csDamping;
	csDamping.Format(_T("%g"), m_pSpfcUtil->GetDamping() / 100.0);
	pDampingWnd->SetWindowText(csDamping);
}
	
void CETC_GDS_IS1893_2025::End()
{
	UpdateData(TRUE);
	Dlg2UtilData();

	CMyChildDialog::End();
}

BOOL CETC_GDS_IS1893_2025::ISVALID()
{  
	Dlg2UtilData();
	if(!IsValidCmb_d(&m_cmbImportanceFact,0.0,10.0,1.0)) return FALSE;
	if(!IsValidCmb_d(&m_cmbRespReduFact,0.0,10.0,1.0)) return FALSE;
	if(!IsValidCmb_n(&m_cmbRetPeriod, 73, 9975, 73)) return FALSE;
	if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}
	if(!m_pSpfcUtil->CheckValid()) { return FALSE; }
	return TRUE;  
}

void CETC_GDS_IS1893_2025::OnChangeEtcGdsIS1893_2016Damping() 
{
	//double dDamping = m_wndIs02Damping.GetEditValue();
	//m_wndIs02DampingFactor.SetEditUnit(m_pSpfcUtil->GetDampingFactorIS1893_2016(dDamping));		
}

void CETC_GDS_IS1893_2025::OnRetPeriodChange()
{
	Dlg2UtilData();
	m_dZoneFact = m_pSpfcUtil->m_adZoneFacts[m_pSpfcUtil->GetEqZone()][m_pSpfcUtil->GetRetPeriod()];
	m_pSpfcUtil->SetZoneFact(m_dZoneFact);
	m_wndZoneFact.SetEditUnit(m_dZoneFact);
}

void CETC_GDS_IS1893_2025::OnSpecTypeChange()
{
	Dlg2UtilData();
	int nSpecType = m_cmbSpecType.GetCurSel();
	m_cmbRespReduFact.EnableWindow( nSpecType != EN_IS2025_SPEC_VER );
	
}

void CETC_GDS_IS1893_2025::OnEqZoneChange()
{
	Dlg2UtilData();
	m_dZoneFact = m_pSpfcUtil->m_adZoneFacts[m_pSpfcUtil->GetEqZone()][m_pSpfcUtil->GetRetPeriod()];
	m_pSpfcUtil->SetZoneFact(m_dZoneFact);
	m_wndZoneFact.SetEditUnit(m_dZoneFact);
}

void CETC_GDS_IS1893_2025::OnZoneFactorChange()
{
	CString strZoneFact;
	m_wndZoneFact.GetWindowTextW(strZoneFact);
	m_pSpfcUtil->SetZoneFact(_tstof(strZoneFact));
	m_dZoneFact = m_pSpfcUtil->GetZoneFact();
}

BOOL CETC_GDS_IS1893_2025::Dlg2Data()
{
	m_pParamData->CodeParam.IS2025.nSpecType = m_pSpfcUtil->GetSpecType();
	m_pParamData->CodeParam.IS2025.nEqZone      = m_pSpfcUtil->GetEqZone();
	m_pParamData->CodeParam.IS2025.nReturnPeriod = m_pSpfcUtil->GetRetPeriod();
	m_pParamData->CodeParam.IS2025.nSiteClass      = m_pSpfcUtil->GetSiteClass();
	m_pParamData->CodeParam.IS2025.dZoneFactor = m_pSpfcUtil->GetZoneFact();
	m_pParamData->CodeParam.IS2025.dDamping = m_pSpfcUtil->GetDamping();
	m_pParamData->CodeParam.IS2025.dImpFactor = m_pSpfcUtil->GetImpFact();
	m_pParamData->CodeParam.IS2025.dRespRedFact = m_pSpfcUtil->GetRespReduFact();
	m_pParamData->CodeParam.IS2025.dMaxPeriod     = m_dMaxPeriod    ;

	m_pParamData->dResModCoef = m_pSpfcUtil->GetRespReduFact();
	return TRUE;
}
void CETC_GDS_IS1893_2025::SetPushOverData(BOOL bInit/*=TRUE*/)
{                  
	m_pSpfcUtil->SetSpecType(m_pParamData->CodeParam.IS2025.nSpecType);
	m_pSpfcUtil->SetEqZone(m_pParamData->CodeParam.IS2025.nEqZone);
	m_pSpfcUtil->SetRetPeriod(m_pParamData->CodeParam.IS2025.nReturnPeriod) ;
	m_pSpfcUtil->SetZoneFact(m_pParamData->CodeParam.IS2025.dZoneFactor);
	m_pSpfcUtil->SetSiteClass(m_pParamData->CodeParam.IS2025.nSiteClass);
	m_pSpfcUtil->SetDamping(m_pParamData->CodeParam.IS2025.dDamping);
	m_pSpfcUtil->SetImpFact(m_pParamData->CodeParam.IS2025.dImpFactor);
	m_pSpfcUtil->SetRespReduFact(m_pParamData->CodeParam.IS2025.dRespRedFact);
	m_dMaxPeriod = m_pParamData->CodeParam.IS2025.dMaxPeriod ;

	if(bInit)
	{
		UpdateData(FALSE);

		m_edtDamping.SetEditUnit(m_pSpfcUtil->GetDamping());
		CString strI, strR;
		strI.Format(_T("%g"), m_pSpfcUtil->GetImpFact());
		strR.Format(_T("%g"), m_pSpfcUtil->GetRespReduFact());

		m_cmbImportanceFact.SetWindowText(strI);
		m_cmbRespReduFact.SetWindowText(strR);
	}

	Dlg2UtilData();
}

void CETC_GDS_IS1893_2025::ShowDlgData()
{
	if(m_pParamData == NULL)
		return;

	if(m_pParamData->nSpecCodeType == D_SPFC_CODE_IS1893_2025)
		m_CodeD.IS2025 = m_pParamData->CodeParam.IS2025;
	else
		m_CodeD.IS2025.Initialize();

	m_pSpfcUtil->SetSpecType(m_CodeD.IS2025.nSpecType);
	m_pSpfcUtil->SetEqZone(m_CodeD.IS2025.nEqZone);
	m_pSpfcUtil->SetRetPeriod(m_CodeD.IS2025.nReturnPeriod);
	m_pSpfcUtil->SetZoneFact(m_CodeD.IS2025.dZoneFactor);
	m_pSpfcUtil->SetSiteClass(m_CodeD.IS2025.nSiteClass);
	m_pSpfcUtil->SetImpFact(m_CodeD.IS2025.dImpFactor);
	m_pSpfcUtil->SetDamping(m_CodeD.IS2025.dDamping);
	m_pSpfcUtil->SetRespReduFact(m_CodeD.IS2025.dRespRedFact);
	m_dMaxPeriod = m_CodeD.IS2025.dMaxPeriod;
	m_dZoneFact = m_CodeD.IS2025.dZoneFactor;

	CDlgUtil::CobxSetCurSelItemData(m_cmbSpecType, m_pSpfcUtil->GetSpecType());
	CDlgUtil::CobxSetCurSelItemData(m_cmbSiteClass, m_pSpfcUtil->GetSiteClass());
	CDlgUtil::CobxSetCurSelItemData(m_cmbEqZone, m_pSpfcUtil->GetEqZone());
	CDlgUtil::CobxSetCurSelItemData(m_cmbRetPeriod, m_pSpfcUtil->GetRetPeriod());

	m_cmbRespReduFact.EnableWindow(m_pSpfcUtil->GetSpecType() != EN_IS2025_SPEC_VER);

	m_wndZoneFact.SetEditUnit(m_pSpfcUtil->GetZoneFact());
	m_edtDamping.SetEditUnit(m_pSpfcUtil->GetDamping());
	CString strImpFact; strImpFact.Format(_T("%g"), m_pSpfcUtil->GetImpFact());
	m_cmbImportanceFact.SetWindowTextW(strImpFact);
	CString strRespReduFact; strRespReduFact.Format(_T("%g"), m_pSpfcUtil->GetRespReduFact());
	m_cmbRespReduFact.SetWindowTextW(strRespReduFact);

	UpdateData(FALSE);

	Dlg2UtilData();
	UpdateData(FALSE);

}

void CETC_GDS_IS1893_2025::Dlg2UtilData()
{
	CString strI, strR;
	m_cmbImportanceFact.GetWindowText(strI);
	m_cmbRespReduFact.GetWindowText(strR);

	DWORD nSpecType = 0;
	if(CDlgUtil::CobxGetItemDataByCurSel(m_cmbSpecType, nSpecType))  m_pSpfcUtil->SetSpecType(nSpecType);
	else { ASSERT(0); m_pSpfcUtil->SetSpecType(0); }

	DWORD nSiteClass = 0;
	if(CDlgUtil::CobxGetItemDataByCurSel(m_cmbSiteClass, nSiteClass))  m_pSpfcUtil->SetSiteClass(nSiteClass);
	else { ASSERT(0); m_pSpfcUtil->SetSiteClass(0); }

	DWORD nEqZone = 0;
	if(CDlgUtil::CobxGetItemDataByCurSel(m_cmbEqZone, nEqZone))  m_pSpfcUtil->SetEqZone(nEqZone);
	else { ASSERT(0); m_pSpfcUtil->SetEqZone(0); }

	DWORD nRetPeriod = 0;
	if(CDlgUtil::CobxGetItemDataByCurSel(m_cmbRetPeriod, nRetPeriod))  m_pSpfcUtil->SetRetPeriod(nRetPeriod);
	else { ASSERT(0); m_pSpfcUtil->SetRetPeriod(0); }

	m_pSpfcUtil->SetDamping(m_edtDamping.GetEditValue());
	m_pSpfcUtil->SetImpFact(_tstof(strI));
	m_pSpfcUtil->SetRespReduFact(_tstof(strR));
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);
	m_pSpfcUtil->SetZoneFact(m_dZoneFact);
}

// ETC_GDS_IBC2012.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_NSR2010.h"
#include "ETC_GDS_NSR2010CalcPhiDlg.h"
#include "SpfcNSR2010Util.h"
#include "..\wg_db\CalcNSR2010.h"

#include "..\wg_db\wg_db_AttrCtrl2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_NSR2010 dialog


CETC_GDS_NSR2010::CETC_GDS_NSR2010(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_NSR2010::IDD, pParent, std::make_shared<CSpfcNSR2010Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_NSR2010)
	//}}AFX_DATA_INIT  
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcNSR2010Util>(m_pSpfcBaseUtil);
}

// Pushover Spectrum은 초기값 설정전에 호출되기 때문에 생성자에서 아래 함수를 반드시 먼저 불러야 한다.


void CETC_GDS_NSR2010::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_NSR2010)
	DDX_Control(pDX, IDC_CMD_SS_CMB, m_cmbAA);
	DDX_Control(pDX, IDC_CMD_S1_CMB, m_cmbAV);
	DDX_Control(pDX, IDC_CMD_SITE_CMB, m_cmbSiteClass);
	DDX_Control(pDX, IDC_CMD_FA_EDIT, m_editFa);
	DDX_Control(pDX, IDC_CMD_FV_EDIT, m_editFv);
	DDX_Control(pDX, IDC_CMD_R_CMB, m_cmbR);	
	DDX_Control(pDX, IDC_CMD_IMPORTANCE_CMB, m_cmbImportance);	
	DDX_Control(pDX, IDC_CMD_PERIOD_EDIT, m_editMaxPeriod);	
	DDX_Control(pDX, IDC_CMD_PHI_EDIT, m_editPhi);	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CETC_GDS_NSR2010, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_NSR2010)
	ON_CBN_SELCHANGE(IDC_CMD_SS_CMB, OnSelchangeCmdAaCmb)
	ON_CBN_SELCHANGE(IDC_CMD_S1_CMB, OnSelchangeCmdAvCmb)
	ON_CBN_SELCHANGE(IDC_CMD_SITE_CMB, OnSelchangeCmdSiteCmb)
	ON_CBN_EDITCHANGE(IDC_CMD_SS_CMB, OnChangeCmdAa)
	ON_CBN_EDITCHANGE(IDC_CMD_S1_CMB, OnChangeCmdAv)
	ON_BN_CLICKED(IDC_CMD_PHI_BTN, OnPhiBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_NSR2010 message handlers

BOOL CETC_GDS_NSR2010::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();

	InitAaCombo();
	InitAvCombo();
	InitSiteCombo();
	InitIFactorCombo();  
	InitRCombo();

	SetFa();
	SetFv();

	m_editMaxPeriod.SetEditUnit(PRD_ED);
	m_editPhi.SetEditUnit(m_pSpfcUtil->GetPhi());
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CETC_GDS_NSR2010::SetFa()
{
	CString str; 
	m_cmbAA.GetWindowText(str);
	double dAa = _tstof(str);

	int    nSiteClass   = m_cmbSiteClass.GetCurSel();

	double dFa=0.;  

	if(!CCalcNSR2010::CalcFa(nSiteClass, dAa, dFa)) return;

	m_editFa.SetEditUnit(dFa, 5);
}

void CETC_GDS_NSR2010::SetFv()
{
	CString str; 
	m_cmbAV.GetWindowText(str);
	double dAv = _tstof(str);

	int    nSiteClass   = m_cmbSiteClass.GetCurSel();

	double dFv=0.;  

	if(!CCalcNSR2010::CalcFv(nSiteClass, dAv, dFv)) return;

	m_editFv.SetEditUnit(dFv, 5);
}

void CETC_GDS_NSR2010::OnSelchangeCmdAaCmb() 
{
	// Edit Box 값을 변경후에 Combo 로 값을 변경하면 바로 Update 안되는 문제...

	double dAa = 0.0;
	int nTmp = m_cmbAA.GetCurSel();
	CArray<double, double> aAaItem; aAaItem.RemoveAll();
	CCalcNSR2010::GetAaDefaultList(aAaItem);
	if(nTmp < aAaItem.GetCount() )
	{
		dAa = aAaItem[nTmp];
	}

	CString csValue; 
	csValue.Format(_T("%g"), dAa);
	m_cmbAA.SetWindowText(csValue);

	SetFa();
}

void CETC_GDS_NSR2010::OnSelchangeCmdAvCmb() 
{
	// Edit Box 값을 변경후에 Combo 로 값을 변경하면 바로 Update 안되는 문제...

	double dAv;
	int nTmp = m_cmbAV.GetCurSel();
	CArray<double, double> aAvItem; aAvItem.RemoveAll();
	CCalcNSR2010::GetAvDefaultList(aAvItem);
	if(nTmp < aAvItem.GetCount() )
	{
		dAv = aAvItem[nTmp];
	}

	CString csValue; 
	csValue.Format(_T("%g"), dAv);
	m_cmbAV.SetWindowText(csValue);

	SetFv();
}

void CETC_GDS_NSR2010::OnSelchangeCmdSiteCmb() 
{
	SetFa();
	SetFv();
}

void CETC_GDS_NSR2010::OnChangeCmdAa() 
{
	SetFa();
}

void CETC_GDS_NSR2010::OnChangeCmdAv() 
{
	SetFv();
}

void CETC_GDS_NSR2010::InitAaCombo()
{
	CArray<double, double> aAaItem; aAaItem.RemoveAll();
	CCalcNSR2010::GetAaDefaultList(aAaItem);
	CString strVal = _T("");
	m_cmbAA.ResetContent();

	for(int i = 0; i < aAaItem.GetCount() ; i++){
		strVal.Format(_T("%.2f"), aAaItem[i]);
		m_cmbAA.AddString(strVal);
	}
	m_cmbAA.SetCurSel(2);
}

void CETC_GDS_NSR2010::InitAvCombo()
{
	CArray<double, double> aAvItem; aAvItem.RemoveAll();
	CCalcNSR2010::GetAvDefaultList(aAvItem);
	CString strVal = _T("");

	m_cmbAV.ResetContent();
	for(int i = 0; i < aAvItem.GetCount(); i++){
		strVal.Format(_T("%.2f"), aAvItem[i]);
		m_cmbAV.AddString(strVal);
	}
	m_cmbAV.SetCurSel(2);
}

void CETC_GDS_NSR2010::InitSiteCombo()
{
	CString aItem[5] = {_T("A"), _T("B"), _T("C"), _T("D"), _T("E")};

	m_cmbSiteClass.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSiteClass.AddString(aItem[i]);
	m_cmbSiteClass.SetCurSel(3);
}

void CETC_GDS_NSR2010::InitIFactorCombo()
{
	CString aItem[4] = {_T("1.0"), _T("1.1"), _T("1.25"), _T("1.5")};

	m_cmbImportance.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbImportance.AddString(aItem[i]);
	m_cmbImportance.SetCurSel(0);
}

void CETC_GDS_NSR2010::InitRCombo()
{
	CString aItem[15] = {_T("1.5"), _T("2"), _T("2.5"), _T("3"), _T("3.25"), _T("3.5"), _T("4"), _T("4.5"), _T("5"), _T("5.5"), _T("6"), _T("6.5"), _T("7"), _T("7.5"), _T("8")};

	m_cmbR.ResetContent();	

	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbR.AddString(aItem[i]);		  
	m_cmbR.SetCurSel(6);	
}

// CETC_DESIGN_SPECT에서 ISVALID()를 호출한 후 End()를 호출한다.
BOOL CETC_GDS_NSR2010::ISVALID()
{ 	
	Dlg2UtilData();
	return static_cast<BOOL>(m_pSpfcUtil->CheckValid());
}

void CETC_GDS_NSR2010::End()
{
	Dlg2UtilData();
	CMyChildDialog::End();
}

BOOL CETC_GDS_NSR2010::Dlg2Data()
{
	m_pParamData->CodeParam.NSR10.nSiteClass = m_pSpfcUtil->GetSite();
	m_pParamData->CodeParam.NSR10.dAa   = m_pSpfcUtil->GetAa();
	m_pParamData->CodeParam.NSR10.dAv   = m_pSpfcUtil->GetAv();
	m_pParamData->CodeParam.NSR10.dFa   = m_pSpfcUtil->GetFa();
	m_pParamData->CodeParam.NSR10.dFv   = m_pSpfcUtil->GetFv();
	m_pParamData->CodeParam.NSR10.dIe   = m_pSpfcUtil->GetI();
	m_pParamData->CodeParam.NSR10.dCoef = m_pSpfcUtil->GetR();
	m_pParamData->CodeParam.NSR10.dPhi  = m_pSpfcUtil->GetPhi();
	m_pParamData->CodeParam.NSR10.dMaxPeriod = m_pSpfcUtil->GetMaxPeriod();

	m_pParamData->dResModCoef = m_pSpfcUtil->GetR() * m_pSpfcUtil->GetPhi();
	return TRUE;
}

void CETC_GDS_NSR2010::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	m_pSpfcUtil->SetSite(m_pParamData->CodeParam.NSR10.nSiteClass);
	m_pSpfcUtil->SetAa(m_pParamData->CodeParam.NSR10.dAa);
	m_pSpfcUtil->SetAv(m_pParamData->CodeParam.NSR10.dAv);
	m_pSpfcUtil->SetFa(m_pParamData->CodeParam.NSR10.dFa);
	m_pSpfcUtil->SetFv(m_pParamData->CodeParam.NSR10.dFv);
	m_pSpfcUtil->SetI(m_pParamData->CodeParam.NSR10.dIe);
	m_pSpfcUtil->SetR(m_pParamData->CodeParam.NSR10.dCoef);
	m_pSpfcUtil->SetPhi(m_pParamData->CodeParam.NSR10.dPhi);

	const double dTc = CCalcNSR2010::CalcSpecturmTC(m_pSpfcUtil->GetAa(), m_pSpfcUtil->GetAv(), m_pSpfcUtil->GetFa(), m_pSpfcUtil->GetFv());
	const double dT0 = CCalcNSR2010::CalcSpecturmT0(m_pSpfcUtil->GetAa(), m_pSpfcUtil->GetAv(), m_pSpfcUtil->GetFa(), m_pSpfcUtil->GetFv());
	const double dTL = CCalcNSR2010::CalcSpectrumTL(m_pSpfcUtil->GetFv());
	m_pSpfcUtil->SetT0(dT0);
	m_pSpfcUtil->SetTc(dTc);
	m_pSpfcUtil->SetTL(dTL);

	if(bInit)
	{
		UpdateData(FALSE);

		m_editFa.SetEditUnit(m_pSpfcUtil->GetFa(),5);
		m_editFv.SetEditUnit(m_pSpfcUtil->GetFv(),5);

		CString strAa, strAv;
		strAa.Format(_T("%g"),m_pSpfcUtil->GetAa());
		strAv.Format(_T("%g"),m_pSpfcUtil->GetAv());
		m_cmbAA.SetWindowText(strAa);    
		m_cmbAV.SetWindowText(strAv);    

		CString strI, strR;
		strI.Format(_T("%g"), m_pSpfcUtil->GetI());
		strR.Format(_T("%g"), m_pSpfcUtil->GetR());
		m_cmbImportance.SetWindowText(strI);    
		m_cmbR.SetWindowText(strR);

		m_editPhi.SetEditUnit(m_pSpfcUtil->GetPhi());

		Dlg2UtilData();
	}
}

void CETC_GDS_NSR2010::Dlg2UtilData()
{
	CString strAa;
	CString strAv;
	m_cmbAA.GetWindowText(strAa);
	m_cmbAV.GetWindowText(strAv);

	const double dTc = CCalcNSR2010::CalcSpecturmTC(m_pSpfcUtil->GetAa(), m_pSpfcUtil->GetAv(), m_pSpfcUtil->GetFa(), m_pSpfcUtil->GetFv());
	const double dT0 = CCalcNSR2010::CalcSpecturmT0(m_pSpfcUtil->GetAa(), m_pSpfcUtil->GetAv(), m_pSpfcUtil->GetFa(), m_pSpfcUtil->GetFv());
	const double dTL = CCalcNSR2010::CalcSpectrumTL(m_pSpfcUtil->GetFv());

	CString strI, strR;
	m_cmbImportance.GetWindowText(strI);
	m_cmbR.GetWindowText(strR);

	m_pSpfcUtil->SetSite(m_cmbSiteClass.GetCurSel());
	m_pSpfcUtil->SetAa(_tstof(strAa));
	m_pSpfcUtil->SetAv(_tstof(strAv));
	m_pSpfcUtil->SetFa(m_editFa.GetEditValue());
	m_pSpfcUtil->SetFv(m_editFv.GetEditValue());
	m_pSpfcUtil->SetI(_tstof(strI));
	m_pSpfcUtil->SetR(_tstof(strR));
	m_pSpfcUtil->SetPhi(m_editPhi.GetEditValue());
	m_pSpfcUtil->SetMaxPeriod(m_editMaxPeriod.GetEditValue());
	m_pSpfcUtil->SetT0(dT0);
	m_pSpfcUtil->SetTc(dTc);
	m_pSpfcUtil->SetTL(dTL);
}

void CETC_GDS_NSR2010::OnPhiBtn()
{
	CETC_GDS_NSR2010CalcPhiDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		double dPhi = m_pSpfcUtil->GetPhi();

		dlg.GetResult(dPhi);
		m_editPhi.SetEditUnit(dPhi);

		m_pSpfcUtil->SetPhi(dPhi);
	}
}
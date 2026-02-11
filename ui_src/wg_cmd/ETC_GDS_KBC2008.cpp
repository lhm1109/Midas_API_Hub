// ETC_GDS_KBC2009.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_KBC2008.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "SpfcKBC2009Util.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KBC2009 dialog


CETC_GDS_KBC2009::CETC_GDS_KBC2009(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_KBC2009::IDD, pParent, std::make_shared<CSpfcKBC2009Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_KBC2009)
	//}}AFX_DATA_INIT
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcKBC2009Util>(m_pSpfcBaseUtil);

	SetInitValueforPOSpectrum();
}

// Pushover Spectrum은 초기값 설정전에 호출되기 때문에 생성자에서 아래 함수를 반드시 먼저 불러야 한다.
void CETC_GDS_KBC2009::SetInitValueforPOSpectrum()
{
	//m_dFa  = 1.36;
	//m_dFv  = 1.96;
	//m_dSds = 0.52668;
	//m_dSd1 = 0.33649;
	//m_dTs = m_dSd1 / m_dSds;
	//m_dR = 1.0;
	//m_dI = 1.0;
	//m_dMaxPeriod = PRD_ED;
}

void CETC_GDS_KBC2009::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_KBC2009)
	DDX_Control(pDX, IDC_CMD_ZONE_CMB, m_cmbSeisZone);
	DDX_Control(pDX, IDC_CMD_ZONE_FACTOR__CMB, m_cmbSeisZoneFactor);
	DDX_Control(pDX, IDC_CMD_SITE_CMB, m_cmbSiteClass);
	DDX_Control(pDX, IDC_CMD_FA_EDIT, m_editFa);
	DDX_Control(pDX, IDC_CMD_FV_EDIT, m_editFv);
	DDX_Control(pDX, IDC_CMD_SDS_EDIT, m_editSds);
	DDX_Control(pDX, IDC_CMD_SD1_EDIT, m_editSd1);
	DDX_Control(pDX, IDC_CMD_R_CMB, m_cmbR);	
	DDX_Control(pDX, IDC_CMD_IMPORTANCE_CMB, m_cmbImportance);	
 	DDX_Control(pDX, IDC_CMD_PERIOD_EDIT, m_editMaxPeriod);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_KBC2009, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_KBC2009)
	ON_CBN_SELCHANGE(IDC_CMD_ZONE_CMB, OnSelchangeCmdZoneCmb)
	ON_CBN_SELCHANGE(IDC_CMD_ZONE_FACTOR__CMB, OnSelchangeCmdZoneFactorCmb)
	ON_CBN_SELCHANGE(IDC_CMD_SITE_CMB, OnSelchangeCmdSiteCmb)

	ON_CBN_EDITCHANGE(IDC_CMD_ZONE_FACTOR__CMB, OnChangeCmdZoneFactor)

	ON_EN_CHANGE(IDC_CMD_FA_EDIT,      OnChangeCmdFaEdit)
	ON_EN_CHANGE(IDC_CMD_FV_EDIT,      OnChangeCmdFvEdit)
	ON_EN_CHANGE(IDC_CMD_SDS_EDIT,     OnChangeCmdSdsEdit)
	ON_EN_CHANGE(IDC_CMD_SD1_EDIT,     OnChangeCmdSd1Edit)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KBC2009 message handlers

BOOL CETC_GDS_KBC2009::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	InitZoneCombo();
	InitZoneFactorCombo();
	InitSiteCombo();
	InitIFactorCombo();  
	InitRCombo();

	SetFaFvSdsSd1();

	m_editMaxPeriod.SetEditUnit(PRD_ED);

	//CtrlManager();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_KBC2009::SetFaFvSdsSd1()
{
	int nSeismicZone = m_cmbSeisZone.GetCurSel();

	CString str; 
	m_cmbSeisZoneFactor.GetWindowText(str);
	double dZoneFactor  = _tstof(str);
	int    nSiteClass   = m_cmbSiteClass.GetCurSel();

	double dFa, dFv, dSds, dSd1;  
	// dCu 는 여기서 사용 안함!!!!

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	// PMS:4428-HSSHIM-20120104. ZoneFactor에 의한 보간방법을 사용하도록 변경
	if(!pDoc->m_pAttrCtrl->CalcKBC2009Fa(dZoneFactor, nSeismicZone, nSiteClass, dFa)) return;
	if(!pDoc->m_pAttrCtrl->CalcKBC2009Fv(dZoneFactor, nSeismicZone, nSiteClass, dFv)) return;
	if(!pDoc->m_pAttrCtrl->CalcKBC2009Sds(dZoneFactor, dFa, dSds)) return;
	if(!pDoc->m_pAttrCtrl->CalcKBC2009Sd1(dZoneFactor, dFv, dSd1)) return;
	//if(!pDoc->m_pAttrCtrl->CalcKBC2009FaFvSdsSd1(nSeismicZone, dZoneFactor, nSiteClass, dFa, dFv, dSds, dSd1, dCu)) return;

	m_editFa.SetEditUnit(dFa, 5);
	m_editFv.SetEditUnit(dFv, 5);
	m_editSds.SetEditUnit(dSds, 5);
	m_editSd1.SetEditUnit(dSd1, 5);
}

void CETC_GDS_KBC2009::OnSelchangeCmdZoneCmb() 
{ 
	SetComboValue();
	SetFaFvSdsSd1();
}

void CETC_GDS_KBC2009::OnSelchangeCmdZoneFactorCmb() 
{
	// Edit Box 값을 변경후에 Combo 로 값을 변경하면 바로 Update 안되는 문제...

	CString csValue;

	int nTmp = m_cmbSeisZoneFactor.GetCurSel();
	if (nTmp == 0)
	{
		csValue.Format(_T("%g"), 0.14);
		m_cmbSeisZoneFactor.SetWindowText(csValue);
	}
	else
	{
		csValue.Format(_T("%g"), 0.22);
		m_cmbSeisZoneFactor.SetWindowText(csValue);
	}

	SetFaFvSdsSd1();
}

void CETC_GDS_KBC2009::OnSelchangeCmdSiteCmb() 
{
	SetFaFvSdsSd1();
}

void CETC_GDS_KBC2009::OnChangeCmdZoneFactor() 
{
	SetFaFvSdsSd1();
}

void CETC_GDS_KBC2009::OnChangeCmdFaEdit() 
{
	CString str;

	m_cmbSeisZoneFactor.GetWindowText(str);
	double dZoneFactor  = _tstof(str);
	double dFa = m_editFa.GetEditValue();
	double dSds;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(!pDoc->m_pAttrCtrl->CalcKBC2009Sds(dZoneFactor, dFa, dSds)) return;

	m_editSds.SetEditUnit(dSds, 5);
}

void CETC_GDS_KBC2009::OnChangeCmdFvEdit() 
{
	CString str; 

	m_cmbSeisZoneFactor.GetWindowText(str);
	double dZoneFactor  = _tstof(str);
	double dFv = m_editFv.GetEditValue();
	double dSd1;  
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(!pDoc->m_pAttrCtrl->CalcKBC2009Sd1(dZoneFactor, dFv, dSd1)) return;

	m_editSd1.SetEditUnit(dSd1, 5);
}

void CETC_GDS_KBC2009::OnChangeCmdSdsEdit() 
{
}

void CETC_GDS_KBC2009::OnChangeCmdSd1Edit() 
{
}

void CETC_GDS_KBC2009::InitZoneCombo()
{
	CString aItem[2] = {_T("1"), _T("2")};

	m_cmbSeisZone.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSeisZone.AddString(aItem[i]);
	m_cmbSeisZone.SetCurSel(0);
}

void CETC_GDS_KBC2009::InitZoneFactorCombo()
{
	CString aItem[2] = {_T("0.14"), _T("0.22")};

	m_cmbSeisZoneFactor.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSeisZoneFactor.AddString(aItem[i]);
	m_cmbSeisZoneFactor.SetCurSel(1);
}

void CETC_GDS_KBC2009::InitSiteCombo()
{
	CString aItem[5] = {_T("Sa"), _T("Sb"), _T("Sc"), _T("Sd"), _T("Se")};

	m_cmbSiteClass.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSiteClass.AddString(aItem[i]);
	m_cmbSiteClass.SetCurSel(3);
}

void CETC_GDS_KBC2009::InitIFactorCombo()
{
	CString aItem[3] = {_T("1.0"), _T("1.2"), _T("1.5")};

	m_cmbImportance.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbImportance.AddString(aItem[i]);
	m_cmbImportance.SetCurSel(1);
}

void CETC_GDS_KBC2009::InitRCombo()
{
	CString aItem[15] = {_T("1.25"), _T("1.5"), _T("2.5"), _T("3"), _T("3.25"), _T("3.5"), _T("4"), _T("4.5"), _T("5"), _T("5.5"), _T("6"), _T("6.5"), _T("7"), _T("7.5"), _T("8")};

	m_cmbR.ResetContent();	

	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbR.AddString(aItem[i]);		  
	m_cmbR.SetCurSel(6);	
}

void CETC_GDS_KBC2009::SetComboValue()
{
	int nSeismicZone = m_cmbSeisZone.GetCurSel();
	int nSiteClass   = m_cmbSiteClass.GetCurSel();

	if(nSeismicZone == 0)
	{
		m_cmbSeisZoneFactor.SetWindowText(_T("0.22"));
	}
	else 
	{
		m_cmbSeisZoneFactor.SetWindowText(_T("0.14"));
	}
}

// CETC_DESIGN_SPECT에서 ISVALID()를 호출한 후 End()를 호출한다.
BOOL CETC_GDS_KBC2009::ISVALID()
{ 	
	GetFinalValue();
	return m_pSpfcUtil->CheckValid();
}

void CETC_GDS_KBC2009::End()
{
	CMyChildDialog::End();
}

void CETC_GDS_KBC2009::GetFinalValue()
{
	CString str; 

	m_pSpfcUtil->SetZone(m_cmbSeisZone.GetCurSel());
	
	m_cmbSeisZoneFactor.GetWindowText(str);
	m_pSpfcUtil->SetZoneFactor(_tstof(str));
	
	m_pSpfcUtil->SetSite(m_cmbSiteClass.GetCurSel());
	m_pSpfcUtil->SetFa(m_editFa.GetEditValue());
	m_pSpfcUtil->SetFv(m_editFv.GetEditValue());
	m_pSpfcUtil->SetSds(m_editSds.GetEditValue());
	m_pSpfcUtil->SetSd1(m_editSd1.GetEditValue());

	const double ts = m_pSpfcUtil->GetSd1() / m_pSpfcUtil->GetSds();
	const double t0 = 0.2 * ts;

	m_pSpfcUtil->SetTs(ts);
	m_pSpfcUtil->SetT0(t0);

	CString strI, strR;
	m_cmbImportance.GetWindowText(strI);    
	m_cmbR.GetWindowText(strR);
	m_pSpfcUtil->SetI(_tstof(strI));
	m_pSpfcUtil->SetR(_tstof(strR));
	
	m_pSpfcUtil->SetMaxPeriod(m_editMaxPeriod.GetEditValue());
}

BOOL CETC_GDS_KBC2009::Dlg2Data()
{
	m_pParamData->CodeParam.KBC2009.nSeisZone = m_pSpfcUtil->GetZone();
	m_pParamData->CodeParam.KBC2009.nSiteClass = m_pSpfcUtil->GetSite();
	m_pParamData->CodeParam.KBC2009.dZoneFactor = m_pSpfcUtil->GetZoneFactor();
	m_pParamData->CodeParam.KBC2009.dFa = m_pSpfcUtil->GetFa();
	m_pParamData->CodeParam.KBC2009.dFv = m_pSpfcUtil->GetFv();
	m_pParamData->CodeParam.KBC2009.dSds = m_pSpfcUtil->GetSds();
	m_pParamData->CodeParam.KBC2009.dSd1 = m_pSpfcUtil->GetSd1();
	m_pParamData->CodeParam.KBC2009.dIe = m_pSpfcUtil->GetI();
	m_pParamData->CodeParam.KBC2009.dCoef = m_pSpfcUtil->GetR();
	m_pParamData->CodeParam.KBC2009.dMaxPeriod = m_pSpfcUtil->GetMaxPeriod();

	m_pParamData->dResModCoef = m_pSpfcUtil->GetR();

	CString strSite[] = { _T("Sa"), _T("Sb"), _T("Sc"), _T("Sd"), _T("Se") };
	m_pParamData->Description.Format(_LS(IDS_WG_CMD__ADD_KBC2009_PROFILE),
		m_pSpfcUtil->GetZone() + 1,
		m_pSpfcUtil->GetZoneFactor(),
		strSite[m_pSpfcUtil->GetSite()],
		m_pSpfcUtil->GetFa(),
		m_pSpfcUtil->GetFv(),
		m_pSpfcUtil->GetSds(),
		m_pSpfcUtil->GetSd1(),
		m_pSpfcUtil->GetI(),
		m_pSpfcUtil->GetR());

	return TRUE;
}
void CETC_GDS_KBC2009::Data2Dlg()
{
	SetPushOverData(TRUE);
}

void CETC_GDS_KBC2009::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	m_pSpfcUtil->SetZone(m_pParamData->CodeParam.KBC2009.nSeisZone);
	m_pSpfcUtil->SetSite(m_pParamData->CodeParam.KBC2009.nSiteClass);
	m_pSpfcUtil->SetZoneFactor(m_pParamData->CodeParam.KBC2009.dZoneFactor);
	m_pSpfcUtil->SetFa(m_pParamData->CodeParam.KBC2009.dFa);
	m_pSpfcUtil->SetFv(m_pParamData->CodeParam.KBC2009.dFv);
	m_pSpfcUtil->SetSds(m_pParamData->CodeParam.KBC2009.dSds);
	m_pSpfcUtil->SetSd1(m_pParamData->CodeParam.KBC2009.dSd1);
	m_pSpfcUtil->SetI(m_pParamData->CodeParam.KBC2009.dIe);
	m_pSpfcUtil->SetR(m_pParamData->CodeParam.KBC2009.dCoef);
	m_pSpfcUtil->SetMaxPeriod(m_pParamData->CodeParam.KBC2009.dMaxPeriod);

	const double ts = m_pSpfcUtil->GetSd1() / m_pSpfcUtil->GetSds();
	const double t0 = 0.2 * ts;

	m_pSpfcUtil->SetTs(ts);
	m_pSpfcUtil->SetT0(t0);

	if(bInit)
	{
		CString strI, strR;
		strI.Format(_T("%g"), m_pSpfcUtil->GetI());
		strR.Format(_T("%g"), m_pSpfcUtil->GetR());
		m_cmbImportance.SetWindowText(strI);    
		m_cmbR.SetWindowText(strR);

		// 3. 데이터를 넣는다.
		m_cmbSeisZone.SetCurSel(m_pSpfcUtil->GetZone());
		CString strTemp;
		strTemp.Format(_T("%g"), m_pSpfcUtil->GetZoneFactor());
		m_cmbSeisZoneFactor.SetWindowText(strTemp);
		m_cmbSiteClass.SetCurSel(m_pSpfcUtil->GetSite());
		m_editFa.SetEditUnit(m_pSpfcUtil->GetFa(), 5);
		m_editFv.SetEditUnit(m_pSpfcUtil->GetFv(), 5);
		m_editSds.SetEditUnit(m_pSpfcUtil->GetSds(), 5);
		m_editSd1.SetEditUnit(m_pSpfcUtil->GetSd1(), 5);
	}
}
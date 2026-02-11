// ETC_GDS_KBC2005.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_KBC2005.h"
#include "SpfcKBC2005Util.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KBC2005 dialog


CETC_GDS_KBC2005::CETC_GDS_KBC2005(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_KBC2005::IDD, pParent, std::make_shared<CSpfcKBC2005Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_KBC2005)
	//}}AFX_DATA_INIT
	
	m_aCtrlRadioAcc.Add(IDC_CMD_ACC_METHOD);
	m_aCtrlRadioAcc.Add(IDC_CMD_ACC_METHOD2);

	m_aCtrlPerCode.Add(IDC_CMD_ZONE_CMB);
	m_aCtrlPerCode.Add(IDC_CMD_SITE_CMB);

	m_aCtrlUserDefined.Add(IDC_CMD_SDS_EDIT);
	m_aCtrlUserDefined.Add(IDC_CMD_SD1_EDIT);

	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcKBC2005Util>(m_pSpfcBaseUtil);

	m_pParamData = pParamData;
	SetInitValueforPOSpectrum();
}

// Pushover Spectrum은 초기값 설정전에 호출되기 때문에 생성자에서 아래 함수를 반드시 먼저 불러야 한다.
void CETC_GDS_KBC2005::SetInitValueforPOSpectrum()
{
	//m_dSds = 0.52668;
	//m_dSd1 = 0.33649;
	//m_dTs = m_dSd1 / m_dSds;
	//m_dR = 1.0;
	//m_dI = 1.0;
	//m_bRelieve = FALSE;
	//m_dMaxPeriod = PRD_ED;
}

void CETC_GDS_KBC2005::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_KBC2005)
	DDX_Control(pDX, IDC_CMD_ZONE_CMB, m_cmbSeisZone);
	DDX_Control(pDX, IDC_CMD_SITE_CMB, m_cmbSiteClass);
	DDX_Control(pDX, IDC_CMD_SDS_EDIT, m_editSds);
	DDX_Control(pDX, IDC_CMD_SD1_EDIT, m_editSd1);
	DDX_Control(pDX, IDC_CMD_R_CMB, m_cmbR);	
	DDX_Control(pDX, IDC_CMD_IMPORTANCE_CMB, m_cmbImportance);	
	DDX_Control(pDX, IDC_CMD_ACC_RELIEVE_CHK, m_btnAccRelieve);
 	DDX_Control(pDX, IDC_CMD_PERIOD_EDIT, m_editMaxPeriod);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_KBC2005, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_KBC2005)
	ON_BN_CLICKED(IDC_CMD_ACC_METHOD, OnCmdAccMethod)
	ON_BN_CLICKED(IDC_CMD_ACC_METHOD2, OnCmdAccMethod)
	ON_CBN_SELCHANGE(IDC_CMD_SITE_CMB, OnSelchangeCmdSiteCmb)
	ON_CBN_SELCHANGE(IDC_CMD_ZONE_CMB, OnSelchangeCmdZoneCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KBC2005 message handlers

BOOL CETC_GDS_KBC2005::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	InitZoneCombo();
	InitSiteCombo();
	InitIFactorCombo();  
	InitRCombo();

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRadioAcc, 0);
	
	OnCmdAccMethod(); 

	m_editMaxPeriod.SetEditUnit(PRD_ED);

	CtrlManager();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_KBC2005::OnCmdAccMethod() 
{
	CtrlManager();
	int nAccMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadioAcc, nAccMethod);  
	if(nAccMethod == 0) SetSdsSd1();		  
}

void CETC_GDS_KBC2005::SetSdsSd1()
{
	int nSeismicZone = m_cmbSeisZone.GetCurSel();
	int nSiteClass   = m_cmbSiteClass.GetCurSel();    
	double dSds, dSd1;  

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(!pDoc->m_pAttrCtrl->CalcKBC2005SdsSd1(nSeismicZone, nSiteClass, dSds, dSd1)) return;
	m_editSds.SetEditUnit(dSds);
	m_editSd1.SetEditUnit(dSd1);
}

void CETC_GDS_KBC2005::OnSelchangeCmdSiteCmb() 
{
	CtrlManager();
	SetSdsSd1();
}

void CETC_GDS_KBC2005::OnSelchangeCmdZoneCmb() 
{
	SetSdsSd1();
}

void CETC_GDS_KBC2005::InitZoneCombo()
{
	CString aItem[2] = {_T("Zone1(0.11)"), _T("Zone2(0.07)")};

	m_cmbSeisZone.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSeisZone.AddString(aItem[i]);
	m_cmbSeisZone.SetCurSel(0);
}

void CETC_GDS_KBC2005::InitSiteCombo()
{
	CString aItem[5] = {_T("Sa"), _T("Sb"), _T("Sc"), _T("Sd"), _T("Se")};

	m_cmbSiteClass.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSiteClass.AddString(aItem[i]);
	m_cmbSiteClass.SetCurSel(3);
}

void CETC_GDS_KBC2005::InitIFactorCombo()
{
	CString aItem[4] = {_T("0.8"), _T("1.0"), _T("1.2"), _T("1.5")};

	m_cmbImportance.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbImportance.AddString(aItem[i]);
	m_cmbImportance.SetCurSel(2);
}

void CETC_GDS_KBC2005::InitRCombo()
{
	CString aItem[11] = {_T("1.25"), _T("1.5"), _T("2.5"), _T("3"), _T("4.5"), _T("5"), _T("5.5"), _T("6"), _T("6.5"), _T("7"), _T("8")};

	m_cmbR.ResetContent();	

	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbR.AddString(aItem[i]);		  
	m_cmbR.SetCurSel(4);	
}

void CETC_GDS_KBC2005::CtrlManager()
{
	int nAccMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadioAcc, nAccMethod);
	m_editSds.SetReadOnly(nAccMethod == 0);
	m_editSd1.SetReadOnly(nAccMethod == 0);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlPerCode, nAccMethod == 0);

	int nSite = m_cmbSiteClass.GetCurSel();
	BOOL bRelieveCondition = FALSE;
	if(nAccMethod == 0 && (nSite == 3 || nSite == 4)) bRelieveCondition = TRUE;
	GetDlgItem(IDC_CMD_ACC_RELIEVE_CHK)->EnableWindow(bRelieveCondition);
	if(!bRelieveCondition) m_btnAccRelieve.SetCheck(FALSE);  
}

// CETC_DESIGN_SPECT에서 ISVALID()를 호출한 후 End()를 호출한다.
BOOL CETC_GDS_KBC2005::ISVALID()
{ 	
	GetFinalValue();

	CString strMsg;
	if(m_pSpfcUtil->GetSds() <= 0.)
	{    
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Sds"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if(m_pSpfcUtil->GetSd1() <= 0.)
	{    
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Sd1"));    
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if(m_pSpfcUtil->GetI() <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Ie"));    
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if(m_pSpfcUtil->GetR() <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("R"));    
		AfxMessageBox(strMsg);
		return FALSE;
	}

//  if(!IsValidCmb_d(&m_cmbIF,0,10,1.0)) return FALSE;
//  if(!IsValidCmb_d(&m_cmbNC,0,50,1.0)) return FALSE;  
	if(m_pSpfcUtil->GetMaxPeriod() <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  
	return TRUE;  
}

void CETC_GDS_KBC2005::End()
{
	CMyChildDialog::End();
}

void CETC_GDS_KBC2005::GetFinalValue()
{
	int nAccMethod = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadioAcc, nAccMethod);
	m_pSpfcUtil->SetAccMethod(nAccMethod);

	m_pSpfcUtil->SetZone(m_cmbSeisZone.GetCurSel());
	m_pSpfcUtil->SetSite(m_cmbSiteClass.GetCurSel());
	m_pSpfcUtil->SetRelieve(m_btnAccRelieve.GetCheck());
	m_pSpfcUtil->SetSds(m_editSds.GetEditValue());
	m_pSpfcUtil->SetSd1(m_editSd1.GetEditValue());
	m_pSpfcUtil->SetTs( m_pSpfcUtil->GetSd1() / m_pSpfcUtil->GetSds() );
	m_pSpfcUtil->SetT0(0.2 * m_pSpfcUtil->GetTs());

	CString strI, strR;
	m_cmbImportance.GetWindowText(strI);    
	m_cmbR.GetWindowText(strR);
	m_pSpfcUtil->SetI(_tstof(strI));
	m_pSpfcUtil->SetR(_tstof(strR));
	
	m_pSpfcUtil->SetMaxPeriod(m_editMaxPeriod.GetEditValue());
}

BOOL CETC_GDS_KBC2005::Dlg2Data()
{
	m_pParamData->CodeParam.KBC2005.nAccMethod = m_pSpfcUtil->GetAccMethod();
	m_pParamData->CodeParam.KBC2005.nSeisZone = m_pSpfcUtil->GetZone();
	m_pParamData->CodeParam.KBC2005.nSiteClass = m_pSpfcUtil->GetSite();
	m_pParamData->CodeParam.KBC2005.dSds = m_pSpfcUtil->GetSds();
	m_pParamData->CodeParam.KBC2005.dSd1 = m_pSpfcUtil->GetSd1();
	m_pParamData->CodeParam.KBC2005.dIe = m_pSpfcUtil->GetI();
	m_pParamData->CodeParam.KBC2005.dCoef = m_pSpfcUtil->GetR();
	m_pParamData->CodeParam.KBC2005.bRelieve = m_pSpfcUtil->GetRelieve();
	m_pParamData->CodeParam.KBC2005.dMaxPeriod = m_pSpfcUtil->GetMaxPeriod();

	m_pParamData->dResModCoef = m_pSpfcUtil->GetR();

	CString strZone[] = { _T("ZoneI(0.11)"), _T("ZoneII(0.07)") };
	CString strSite[] = { _T("Sa"), _T("Sb"), _T("Sc"), _T("Sd"), _T("Se") };
	if (m_pSpfcUtil->GetAccMethod() == 0)
		m_pParamData->Description.Format(_LS(IDS_WG_CMD__ADD_KBC2005_PROFILE1), strZone[m_pSpfcUtil->GetZone()], strSite[m_pSpfcUtil->GetSite()], m_pSpfcUtil->GetSds(), m_pSpfcUtil->GetSd1(), m_pSpfcUtil->GetI(), m_pSpfcUtil->GetR());
	else
		m_pParamData->Description.Format(_LS(IDS_WG_CMD__ADD_KBC2005_PROFILE2), m_pSpfcUtil->GetSds(), m_pSpfcUtil->GetSd1(), m_pSpfcUtil->GetI(), m_pSpfcUtil->GetR());

	return TRUE;
}

void CETC_GDS_KBC2005::Data2Dlg()
{
	SetPushOverData(TRUE);
	CtrlManager();	
}

void CETC_GDS_KBC2005::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	m_pSpfcUtil->SetAccMethod(m_pParamData->CodeParam.KBC2005.nAccMethod);
	m_pSpfcUtil->SetZone(m_pParamData->CodeParam.KBC2005.nSeisZone);
	m_pSpfcUtil->SetSite(m_pParamData->CodeParam.KBC2005.nSiteClass);
	m_pSpfcUtil->SetSds(m_pParamData->CodeParam.KBC2005.dSds);
	m_pSpfcUtil->SetSd1(m_pParamData->CodeParam.KBC2005.dSd1);
	m_pSpfcUtil->SetI(m_pParamData->CodeParam.KBC2005.dIe);
	m_pSpfcUtil->SetR(m_pParamData->CodeParam.KBC2005.dCoef);
	m_pSpfcUtil->SetRelieve(m_pParamData->CodeParam.KBC2005.bRelieve);
	m_pSpfcUtil->SetMaxPeriod(m_pParamData->CodeParam.KBC2005.dMaxPeriod);

	m_pSpfcUtil->SetTs(m_pSpfcUtil->GetSd1() / m_pSpfcUtil->GetSds());
	m_pSpfcUtil->SetT0(0.2 * m_pSpfcUtil->GetTs());

	if (bInit)
	{
		CString strI, strR;
		strI.Format(_T("%g"), m_pSpfcUtil->GetI());
		strR.Format(_T("%g"), m_pSpfcUtil->GetR());
		m_cmbImportance.SetWindowText(strI);
		m_cmbR.SetWindowText(strR);

		int nAccMethod = 0;
		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadioAcc, nAccMethod);
		m_pSpfcUtil->SetAccMethod(nAccMethod);

		m_cmbSeisZone.SetCurSel(m_pSpfcUtil->GetZone());
		m_cmbSiteClass.SetCurSel(m_pSpfcUtil->GetSite());
		m_editSds.SetEditUnit(m_pSpfcUtil->GetSds(), 5);
		m_editSd1.SetEditUnit(m_pSpfcUtil->GetSd1(), 5);
		m_btnAccRelieve.SetCheck(m_pSpfcUtil->GetRelieve());

		CtrlManager();
	}
}
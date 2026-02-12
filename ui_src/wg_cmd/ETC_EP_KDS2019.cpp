// ETC_GDS_KS.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_EP_KDS2019.h"
#include "SeisLoadClassifyMethodDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KS dialog


CETC_EP_KDS2019::CETC_EP_KDS2019(T_POSL_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_EP_KDS2019::IDD, pParent)
{
	m_pData = pParamData;

	m_aCtrlRadioMethod.Add(IDC_CMD_EP_SEISMIC_TYPE_EQU_RDO);
	m_aCtrlRadioMethod.Add(IDC_CMD_EP_SEISMIC_TYPE_RDM_RDO);

	m_aCtrlEquMethod.RemoveAll();
	m_aCtrlEquMethod.Add(IDC_WG_CMD_STATIC8);
	m_aCtrlEquMethod.Add(IDC_CMD_SDS_EDIT);
	m_aCtrlEquMethod.Add(IDC_WG_CMD_STATIC9);
	m_aCtrlEquMethod.Add(IDC_WG_CMD_STATIC7);
	m_aCtrlEquMethod.Add(IDC_CMD_FV_EDIT);
	m_aCtrlEquMethod.Add(IDC_WG_CMD_STATIC10);
	m_aCtrlEquMethod.Add(IDC_CMD_SD1_EDIT);
	m_aCtrlEquMethod.Add(IDC_WG_CMD_STATIC11);

	m_aCtrlRdmMethod.RemoveAll();
	m_aCtrlRdmMethod.Add(IDC_WG_CMD_STATIC52);
	m_aCtrlRdmMethod.Add(IDC_CMD_EPGA_EDIT);
	m_aCtrlRdmMethod.Add(IDC_WG_CMD_STATIC53);
	m_aCtrlRdmMethod.Add(IDC_CMD_KAE_EDIT);

	m_aCtrlDisableRdm.RemoveAll();
	m_aCtrlDisableRdm.Add(IDC_CMD_SITE_CMB);
	m_aCtrlDisableRdm.Add(IDC_CMD_FA_EDIT);
	m_aCtrlDisableRdm.Add(IDC_CMD_FV_EDIT);
	m_aCtrlDisableRdm.Add(IDC_CMD_SDS_EDIT);
	m_aCtrlDisableRdm.Add(IDC_CMD_SD1_EDIT);

	m_aCtrlEnableRdm.RemoveAll();
	m_aCtrlEnableRdm.Add(IDC_CMD_EPGA_EDIT);
	m_aCtrlEnableRdm.Add(IDC_CMD_KAE_EDIT);

}

CETC_EP_KDS2019::~CETC_EP_KDS2019()
{
}



void CETC_EP_KDS2019::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_KS)
	DDX_Control(pDX, IDC_CMD_ZONE_CMB, m_cmbSeisZone);
	DDX_Control(pDX, IDC_CMD_ZONE_FACTOR_CMB, m_cmbSeisZoneFactor);
	DDX_Control(pDX, IDC_CMD_SITE_CMB, m_cmbSiteClass);
	DDX_Control(pDX, IDC_CMD_USE_GROUP_CMB, m_cmbUseGroup);
	DDX_Control(pDX, IDC_CMD_IMPORTANCE_CMB, m_cmbImportance);
	DDX_Control(pDX, IDC_CMD_RX_CMB, m_cmbRx);
	DDX_Control(pDX, IDC_CMD_FA_EDIT, m_editFa);
	DDX_Control(pDX, IDC_CMD_FV_EDIT, m_editFv);
	DDX_Control(pDX, IDC_CMD_SDS_EDIT, m_editSds);
	DDX_Control(pDX, IDC_CMD_SD1_EDIT, m_editSd1);
	DDX_Control(pDX, IDC_CMD_EPGA_EDIT, m_editEPGA);
	DDX_Control(pDX, IDC_CMD_KAE_EDIT, m_editKae);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_EP_KDS2019, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_EP_KDS2019)
	ON_CBN_SELCHANGE(IDC_CMD_ZONE_CMB, OnSelchangeCmdZoneCmb)
	ON_CBN_SELCHANGE(IDC_CMD_ZONE_FACTOR_CMB, OnSelchangeCmdZoneFactorCmb)
	ON_CBN_SELCHANGE(IDC_CMD_SITE_CMB, OnSelchangeCmdSiteCmb)
	ON_CBN_SELCHANGE(IDC_CMD_USE_GROUP_CMB, OnSelchangeCmdUseGroupCmb)
	ON_CBN_EDITCHANGE(IDC_CMD_ZONE_FACTOR_CMB, OnChangeCmdZoneFactor)
	ON_EN_CHANGE(IDC_CMD_FA_EDIT, OnChangeCmdFaEdit)

	ON_BN_CLICKED(IDC_CMD_EP_SEISMIC_TYPE_EQU_RDO, OnCmdMethodChange)
	ON_BN_CLICKED(IDC_CMD_EP_SEISMIC_TYPE_RDM_RDO, OnCmdMethodChange)
	ON_BN_CLICKED(IDC_CMD_EP_SEISMIC_TYPE_BTN, OnCmdMethodBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KS message handlers


BOOL CETC_EP_KDS2019::OnInitDialog()
{
	CMyChildDialog::OnInitDialog();
	
	InitZoneCombo();
	InitZoneFactorCombo();
	InitSiteCombo();
	InitUseGroupCombo();
	InitImportanceFactorCombo();
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlEnableRdm, FALSE);
	InitRxRyCombo();

	Data2Dlg();
	OnCmdMethodChange();
	return TRUE;  // return TRUE unless you set the focus to a control
	             // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_EP_KDS2019::End()
{	
	CMyChildDialog::End();
}


void CETC_EP_KDS2019::Data2Dlg()
{	
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRadioMethod, m_pData->KDS2019.nEPSEMethod);
	CString csValue;
	csValue.Format(_T("%g"), m_pData->KDS2019.dEPA);
	m_cmbSeisZone.SetCurSel(m_pData->KDS2019.nSeismicZone);
	m_cmbSeisZoneFactor.SetWindowText(csValue);
	m_cmbSiteClass.SetCurSel(m_pData->KDS2019.nSiteClass);
	m_editFa.SetEditUnit(m_pData->KDS2019.dFa, 5);
	// 	m_editFv.SetEditUnit(m_pData->dFv, 5);
	m_editSds.SetEditUnit(m_pData->KDS2019.dSds, 5);
	m_editSd1.SetEditUnit(m_pData->KDS2019.dSd1, 5);
	m_cmbUseGroup.SetCurSel(m_pData->KDS2019.nSeismicUseGroup);

	csValue.Format(_T("%g"), m_pData->KDS2019.dImportanceFactor);
	if (CB_ERR == m_cmbImportance.FindStringExact(-1, csValue))
	{
		m_cmbImportance.SetWindowText(csValue);
	}
	else
	{
		m_cmbImportance.SelectString(-1, csValue);
	}

	csValue.Format(_T("%g"), m_pData->KDS2019.dResponseModifyFactor);
	m_cmbRx.SetWindowText(csValue);
}

BOOL CETC_EP_KDS2019::Dlg2Data()
{	
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadioMethod, m_pData->KDS2019.nEPSEMethod);
	CString csValue;
	m_pData->KDS2019.nSeismicZone = m_cmbSeisZone.GetCurSel();
	m_cmbSeisZoneFactor.GetWindowText(csValue);
	m_pData->KDS2019.dEPA = _tstof(csValue);
	m_pData->KDS2019.nSiteClass = m_cmbSiteClass.GetCurSel();
	m_pData->KDS2019.dFa = m_editFa.GetEditValue();
	// 	m_editFv.SetEditUnit(m_pData->dFv, 5);
	m_pData->KDS2019.dSds = m_editSds.GetEditValue();
	m_pData->KDS2019.dSd1 = m_editSd1.GetEditValue();
	m_pData->KDS2019.nSeismicUseGroup = m_cmbUseGroup.GetCurSel();

	m_cmbImportance.GetWindowText(csValue);
	m_pData->KDS2019.dImportanceFactor = _tstof(csValue);
	
	m_cmbRx.GetWindowText(csValue);
	m_pData->KDS2019.dResponseModifyFactor = _tstof(csValue);

	return TRUE;
}


void CETC_EP_KDS2019::InitZoneCombo()
{
	CString aItem[2] = { _T("1"), _T("2") };

	m_cmbSeisZone.ResetContent();
	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSeisZone.AddString(aItem[i]);
	m_cmbSeisZone.SetCurSel(0);
}

void CETC_EP_KDS2019::InitZoneFactorCombo()
{
	CString aItem[2] = { _T("0.14"), _T("0.22") };

	m_cmbSeisZoneFactor.ResetContent();
	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSeisZoneFactor.AddString(aItem[i]);
	m_cmbSeisZoneFactor.SetCurSel(1);
}

void CETC_EP_KDS2019::InitSiteCombo()
{
	CString aItem[6] = { _T("S1"), _T("S2"), _T("S3"), _T("S4"), _T("S5"), _T("S6") };

	m_cmbSiteClass.ResetContent();
	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSiteClass.AddString(aItem[i]);
	m_cmbSiteClass.SetCurSel(0);
	m_cmbSiteClass.EnableWindow(FALSE);
}

void CETC_EP_KDS2019::InitUseGroupCombo()
{
	CString aItem[3] = { _LS(IDS_CMD_SEIS_SPECIAL), _T("I"), _T("II") };

	m_cmbUseGroup.ResetContent();
	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbUseGroup.AddString(aItem[i]);
	m_cmbUseGroup.SetCurSel(1);
}

void CETC_EP_KDS2019::InitImportanceFactorCombo()
{
	CString aItem[3] = { _T("1.0"), _T("1.2"), _T("1.5") };

	m_cmbImportance.ResetContent();
	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbImportance.AddString(aItem[i]);
	m_cmbImportance.SetCurSel(1);
}

void CETC_EP_KDS2019::InitRxRyCombo()
{
	CString aItem[15] = { _T("1.25"), _T("1.5"), _T("2.5"), _T("3"), _T("3.25"), _T("3.5"), _T("4"), _T("4.5"), _T("5"), _T("5.5"), _T("6"), _T("6.5"), _T("7"), _T("7.5"), _T("8") };

	m_cmbRx.ResetContent();
	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
	{
		m_cmbRx.AddString(aItem[i]);
	}
	m_cmbRx.SetCurSel(3);
}


void CETC_EP_KDS2019::SetComboValue()
{
	int nSeismicZone = m_cmbSeisZone.GetCurSel();
	int nSiteClass = m_cmbSiteClass.GetCurSel();

	if (nSeismicZone == 0)
	{
		m_cmbSeisZoneFactor.SetWindowText(_T("0.22"));
	}
	else
	{
		m_cmbSeisZoneFactor.SetWindowText(_T("0.14"));
	}
}


void CETC_EP_KDS2019::SetFaFvSdsSd1()
{
	if (m_pData->KDS2019.nEPSEMethod == EN_EPSE_METHOD_EQUIVALENT)
		SetEquivalentData();
	else SetRDMData();
}

void CETC_EP_KDS2019::SetImportanceFactor()
{
	int nSeismicUseGroup = m_cmbUseGroup.GetCurSel();

	if (nSeismicUseGroup == 0)
	{
		m_cmbImportance.SetWindowText(_T("1.5"));
	}
	else if (nSeismicUseGroup == 1)
	{
		m_cmbImportance.SetWindowText(_T("1.2"));
	}
	else
	{
		m_cmbImportance.SetWindowText(_T("1.0"));
	}
}


void CETC_EP_KDS2019::OnChangeCmdFaEdit()
{
	// 	int nSiteClass = m_cmbSiteClass.GetCurSel();
	// 	if (nSiteClass==5) return;
	m_pData->KDS2019.dFa = m_editFa.GetEditValue();
	SetEquivalentData_Calc();
}


void CETC_EP_KDS2019::OnSelchangeCmdZoneCmb()
{
	SetComboValue();

	int nSiteClass = m_cmbSiteClass.GetCurSel();
	if (nSiteClass == 5) return;

	SetFaFvSdsSd1();
	// 	SetSeisDesignCategory();	
}

void CETC_EP_KDS2019::OnSelchangeCmdZoneFactorCmb()
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

	// 	int nSiteClass = m_cmbSiteClass.GetCurSel();
	// 	if(nSiteClass==5) return;

	SetFaFvSdsSd1();
	// 	SetSeisDesignCategory();	
}

void CETC_EP_KDS2019::OnChangeCmdZoneFactor()
{
	// 	int nSiteClass = m_cmbSiteClass.GetCurSel();
	// 	if(nSiteClass==5) return;

	SetFaFvSdsSd1();
	// 	SetSeisDesignCategory();	
}

void CETC_EP_KDS2019::OnSelchangeCmdUseGroupCmb()
{
	SetImportanceFactor();
	// 	SetSeisDesignCategory();	
}


void CETC_EP_KDS2019::OnSelchangeCmdSiteCmb()
{
	SetFaFvSdsSd1();
	//SetSeisDesignCategory();
}

void CETC_EP_KDS2019::OnCmdMethodChange()
{

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadioMethod, m_pData->KDS2019.nEPSEMethod);
	BOOL bEnableEquivalent = m_pData->KDS2019.nEPSEMethod == EN_EPSE_METHOD_EQUIVALENT ? TRUE : FALSE;

	CDlgUtil::CtrlShowHide(this, m_aCtrlEquMethod, !bEnableEquivalent);
	CDlgUtil::CtrlShowHide(this, m_aCtrlRdmMethod, bEnableEquivalent);

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDisableRdm, bEnableEquivalent);

	if (!bEnableEquivalent)
	{
		m_cmbSiteClass.SetCurSel(0);
	}
	SetFaFvSdsSd1();
	m_pData->KDS2019.nEPSEMethod = (bEnableEquivalent) ? 0 : 1;
}

void CETC_EP_KDS2019::OnCmdMethodBtn()
{
	CSeisLoadClassifyMethodDlg dlg;
	dlg.DoModal();

	int nMethod = dlg.m_nMethod;
	if (nMethod == -1) return;

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRadioMethod, nMethod);
	OnCmdMethodChange();
}

void CETC_EP_KDS2019::SetEquivalentData()
{
	CString str;
	m_cmbSeisZoneFactor.GetWindowText(str);
	int    nSiteClass   = m_cmbSiteClass.GetCurSel();
	double dZoneFactor  = _tstof(str);
	double dFa=0.;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (nSiteClass >= 0 && nSiteClass <= 4)
	{
		if (!pDoc->m_pAttrCtrl->CalcKDS2019Fa(dZoneFactor, nSiteClass, dFa)) return;
	}
	m_editFa.SetEditUnit(dFa, 5);

	m_pData->KDS2019.dEPA = _tstof(str);
	m_pData->KDS2019.dFa = m_editFa.GetEditValue();
	
	SetEquivalentData_Calc();
}

void CETC_EP_KDS2019::SetEquivalentData_Calc()
{
	double dEPGAeff =  (2./3.) * m_pData->KDS2019.dEPA * m_pData->KDS2019.dFa;	// ( S * Fa*2/3 로 계산) 
	double dKae = 0.75 * dEPGAeff;							// 0.75*EPGAeff로 계산

	m_editEPGA.SetEditUnit(dEPGAeff);
	m_editKae.SetEditUnit(dKae);
}

void CETC_EP_KDS2019::SetRDMData()
{
	CString str;

	m_cmbSeisZoneFactor.GetWindowText(str);
	double dZoneFactor  = _tstof(str);
	int    nSiteClass   = m_cmbSiteClass.GetCurSel();

	double dFa=0., dFv=0., dSds=0., dSd1=0.;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	dFa = 1.12;
	dFv = 0.84;
	if (nSiteClass >= 0 && nSiteClass <= 4)
	{
		
		if (!pDoc->m_pAttrCtrl->CalcKBC2009Sds(dZoneFactor, dFa, dSds)) return;
		if (!pDoc->m_pAttrCtrl->CalcKBC2009Sd1(dZoneFactor, dFv, dSd1)) return;
	}
	m_editFa.SetEditUnit(dFa, 5);
	m_editFv.SetEditUnit(dFv, 5);
	m_editSds.SetEditUnit(dSds, 5);
	m_editSd1.SetEditUnit(dSd1, 5);
}


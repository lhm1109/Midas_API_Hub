// SeisItemKBC2005.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemKBC2005.h"
#include "SeisKBC2005PeriodDlg.h"
#include "SeisItemDlgNew.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_InitCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisItemKBC2005 dialog


CSeisItemKBC2005::CSeisItemKBC2005(CWnd* pParent /*=NULL*/)
	: CChildDialog(CSeisItemKBC2005::IDD, pParent)
{
	m_aCtrlRadioAcc.Add(IDC_CMD_ACC_METHOD);
	m_aCtrlRadioAcc.Add(IDC_CMD_ACC_METHOD2);

	m_aCtrlRadioPeriod.Add(IDC_CMD_PERIOD_RDO);
	m_aCtrlRadioPeriod.Add(IDC_CMD_PERIOD_RDO2);

	m_aCtrlPerCode.Add(IDC_CMD_ZONE_CMB);
	m_aCtrlPerCode.Add(IDC_CMD_SITE_CMB);

	m_aCtrlUserDefined.Add(IDC_CMD_SDS_EDIT);
	m_aCtrlUserDefined.Add(IDC_CMD_SD1_EDIT);

	m_aCtrlPeriodAnal.Add(IDC_CMD_TX_ANAL_EDIT);
	m_aCtrlPeriodAnal.Add(IDC_CMD_TY_ANAL_EDIT);

	m_bInit = FALSE;
	m_Data.Initialize();
	m_pParent = (CSeisItemDlgNew*)pParent;
}


void CSeisItemKBC2005::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisItemKBC2005)
	DDX_Control(pDX, IDC_CMD_TX_ANAL_EDIT, m_editAnalX);
	DDX_Control(pDX, IDC_CMD_TY_ANAL_EDIT, m_editAnalY);
	DDX_Control(pDX, IDC_CMD_TX_APPR_EDIT, m_editApprX);
	DDX_Control(pDX, IDC_CMD_TY_APPR_EDIT, m_editApprY);
	DDX_Control(pDX, IDC_CMD_TX_FINAL_EDIT, m_editFinalX);
	DDX_Control(pDX, IDC_CMD_TY_FINAL_EDIT, m_editFinalY);
	DDX_Control(pDX, IDC_CMD_ZONE_CMB, m_cmbSeisZone);
	DDX_Control(pDX, IDC_CMD_USE_GROUP_CMB, m_cmbUseGroup);
	DDX_Control(pDX, IDC_CMD_SITE_CMB, m_cmbSiteClass);
	DDX_Control(pDX, IDC_CMD_SDC_EDIT, m_editSDC);
	DDX_Control(pDX, IDC_CMD_SDC_S_EDIT, m_editSDC_S);
	DDX_Control(pDX, IDC_CMD_SDC_1_EDIT, m_editSDC_1);
	DDX_Control(pDX, IDC_CMD_SDS_EDIT, m_editSds);
	DDX_Control(pDX, IDC_CMD_SD1_EDIT, m_editSd1);
	DDX_Control(pDX, IDC_CMD_RY_CMB, m_cmbRy);
	DDX_Control(pDX, IDC_CMD_RX_CMB, m_cmbRx);
	DDX_Control(pDX, IDC_CMD_IMPORTANCE_CMB, m_cmbImportance);
	DDX_Control(pDX, IDC_CMD_CITY_PLAN_CHK, m_btnCityPlan);
	DDX_Control(pDX, IDC_WG_CMD_STATIC0, m_grpFold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeisItemKBC2005, CChildDialog)
	//{{AFX_MSG_MAP(CSeisItemKBC2005)
	ON_BN_CLICKED(IDC_CMD_ACC_METHOD, OnCmdAccMethod)
	ON_BN_CLICKED(IDC_CMD_ACC_METHOD2, OnCmdAccMethod)
	ON_BN_CLICKED(IDC_CMD_CITY_PLAN_CHK, OnCmdCityPlanChk)
	ON_BN_CLICKED(IDC_CMD_PERIOD_BTN, OnCmdPeriodBtn)
	ON_BN_CLICKED(IDC_CMD_PERIOD_RDO, OnCmdPeriodRdo)
	ON_BN_CLICKED(IDC_CMD_PERIOD_RDO2, OnCmdPeriodRdo)
	ON_CBN_SELCHANGE(IDC_CMD_SITE_CMB, OnSelchangeCmdSiteCmb)
	ON_CBN_SELCHANGE(IDC_CMD_ZONE_CMB, OnSelchangeCmdZoneCmb)
	ON_CBN_SELCHANGE(IDC_CMD_USE_GROUP_CMB, OnSelchangeCmdUseGroupCmb)
	ON_EN_CHANGE(IDC_CMD_TX_ANAL_EDIT, OnChangeCmdPeriodEdit)
	ON_EN_CHANGE(IDC_CMD_TX_APPR_EDIT, OnChangeCmdPeriodEdit)
	ON_EN_CHANGE(IDC_CMD_TY_ANAL_EDIT, OnChangeCmdPeriodEdit)
	ON_EN_CHANGE(IDC_CMD_TY_APPR_EDIT, OnChangeCmdPeriodEdit)
	ON_EN_CHANGE(IDC_CMD_SDS_EDIT, OnChangeCmdSdsEdit)
	ON_EN_CHANGE(IDC_CMD_SD1_EDIT, OnChangeCmdSd1Edit)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisItemKBC2005 message handlers

BOOL CSeisItemKBC2005::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	m_editAnalX.SetUnitType(D_UNITSYS_NONE);
	m_editAnalY.SetUnitType(D_UNITSYS_NONE);
	m_editApprX.SetUnitType(D_UNITSYS_NONE);
	m_editApprY.SetUnitType(D_UNITSYS_NONE);
	m_editFinalX.SetUnitType(D_UNITSYS_NONE);
	m_editFinalY.SetUnitType(D_UNITSYS_NONE);
	m_editFinalX.SetReadOnly(TRUE);
	m_editFinalY.SetReadOnly(TRUE);

	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}

	InitZoneCombo();
	InitSiteCombo();
	InitIFactorCombo();
	InitUseGroupCombo();
	InitRxRyCombo();

	SetFoldGroupCtrls();
	if (!m_bModify) m_Data.Initialize();
	Data2Dlg();
	/*
	if(m_bModify) Data2Dlg();
	else 
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRadioAcc, 0);
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRadioPeriod, 0);
		m_editAnalX.SetWindowText(_T("0"));
		m_editAnalY.SetWindowText(_T("0"));
		m_editApprX.SetWindowText(_T("0"));
		m_editApprY.SetWindowText(_T("0"));
		m_editFinalX.SetWindowText(_T("0"));
		m_editFinalY.SetWindowText(_T("0"));
		SetImportanceFactor();
	}
*/
	OnCmdAccMethod();
	OnCmdPeriodRdo();
	SetSeisDesignCategory();

	CtrlManager();

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisItemKBC2005::SetFoldGroupCtrls()
{
	// After AlignControls
	m_grpFold.InitControl(this, CSeisItemKBC2005::IDD, IDC_WG_CMD_STATIC0, TRUE, TRUE);
	m_grpFold.SetFoldState(FALSE); // open
}

BOOL CSeisItemKBC2005::DestroyWindow()
{
	if (m_pParent != NULL)
	{
		if (!m_pParent->IsSaveFoldedInfo())
		{
			m_grpFold.SetFoldState(FALSE, FALSE);
		}
	}

	return CChildDialog::DestroyWindow();
}

void CSeisItemKBC2005::CtrlManager()
{
	int nAccMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadioAcc, nAccMethod);
	m_editSds.SetReadOnly(nAccMethod == 0);
	m_editSd1.SetReadOnly(nAccMethod == 0);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlPerCode, nAccMethod == 0);

	int nPeriodMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadioPeriod, nPeriodMethod);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlPeriodAnal, nPeriodMethod == 0);
	UpdateData(FALSE);
}

void CSeisItemKBC2005::SetData2Dlg(T_SEIS_KBC2005& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CSeisItemKBC2005::SetDlg2Data(T_SEIS_KBC2005& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CSeisItemKBC2005::InitDefaultData()
{
	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl;
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;
	double dHn = 0.0;
	T_STOR_D StorD, BaseStorD;

	// 주의 !
	// Seismic Load 에서는 m 단위로 변환하여 받아온다.
	// Wind Load 에서는 현재 단위로 받아온다.
	//////////////////////////////////////////////
	// <Remember> Data를 받아올때는 Code Unit기준
	T_UNIT_INDEX CurIndex, CodeIndex;
	pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CodeIndex = CurIndex;
	CodeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M; // M 기준
	pUnitCtrl->SetUnitIndexCurrent(CodeIndex);

	CArray<T_STOR_K,T_STOR_K> aStorK;
	pAttrCtrl->GetStorKeyList(aStorK);
	int nStorNum = aStorK.GetSize();
	if (nStorNum > 0) pAttrCtrl->GetStor(aStorK[nStorNum - 1], StorD);
	else StorD.Initialize();

	double dBaseLevel = pAttrCtrl->GetBaseLevel();
	dHn = StorD.dStoryLevel - dBaseLevel;
	if (!pAttrCtrl->GetCountStor()) dHn = 0;

	m_nNX = m_nNY = nStorNum - 1;
	m_dHnX = m_dHnY = dHn;
	m_dAcX = m_dAcY = 0.0;
	m_nMtdX = m_nMtdY = 0;

	// <Remember> Data를 받고나서는 User Unit기준
	pUnitCtrl->SetUnitIndexCurrent(CurIndex);
}

void CSeisItemKBC2005::OnCmdAccMethod()
{
	CtrlManager();

	int nAccMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadioAcc, nAccMethod);
	//  CDlgUtil::CtrlEnableDisable(this, m_aCtrlPerCode, nAccMethod == 0);
	//  CDlgUtil::CtrlEnableDisable(this, m_aCtrlUserDefined, nAccMethod == 1);

	if (nAccMethod == 0) SetSdsSd1();
	SetSeisDesignCategory();
}

void CSeisItemKBC2005::SetSdsSd1()
{
	int nSeismicZone = m_cmbSeisZone.GetCurSel();
	int nSiteClass = m_cmbSiteClass.GetCurSel();
	double dSds, dSd1;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pAttrCtrl->CalcKBC2005SdsSd1(nSeismicZone, nSiteClass, dSds, dSd1)) return;
	m_editSds.SetEditUnit(dSds);
	m_editSd1.SetEditUnit(dSd1);
}

void CSeisItemKBC2005::SetSeisDesignCategory()
{
	int nSeisUseGroup = m_cmbUseGroup.GetCurSel();
	double dSds = m_editSds.GetEditValue();
	double dSd1 = m_editSd1.GetEditValue();

	CString SDC_S, SDC_1, SDC;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pAttrCtrl->CalcKBC2005SeisDesignCategory(nSeisUseGroup, dSds, dSd1, SDC_S, SDC_1, SDC)) return;

	m_editSDC_S.SetWindowText(SDC_S);
	m_editSDC_1.SetWindowText(SDC_1);
	m_editSDC.SetWindowText(SDC);
}

void CSeisItemKBC2005::SetImportanceFactor()
{
	int nSeismicUseGroup = m_cmbUseGroup.GetCurSel();
	BOOL bCityPlan = m_btnCityPlan.GetCheck();
	int nCityPlanRegion = 0;
	if (!bCityPlan) nCityPlanRegion = 1;

	static double T_IE[3][2] = {{1.5, 1.2}, {1.2, 1.0}, {1.0, 0.8}};

	double dImportanceFactor = T_IE[nSeismicUseGroup][nCityPlanRegion];
	CString str;
	str.Format(_T("%g"), dImportanceFactor);
	m_cmbImportance.SetWindowText(str);
}

void CSeisItemKBC2005::OnSelchangeCmdSiteCmb()
{
	SetSdsSd1();
	SetSeisDesignCategory();
}

void CSeisItemKBC2005::OnSelchangeCmdZoneCmb()
{
	SetSdsSd1();
	SetSeisDesignCategory();
}

void CSeisItemKBC2005::OnChangeCmdSdsEdit()
{
	SetSeisDesignCategory();
}

void CSeisItemKBC2005::OnChangeCmdSd1Edit()
{
	SetSeisDesignCategory();
}

void CSeisItemKBC2005::OnSelchangeCmdUseGroupCmb()
{
	SetImportanceFactor();
	SetSeisDesignCategory();
}

void CSeisItemKBC2005::OnCmdCityPlanChk()
{
	SetImportanceFactor();
	SetSeisDesignCategory();
}

void CSeisItemKBC2005::OnCmdPeriodBtn()
{
	CSeisKBC2005PeriodDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);

	dlg.m_nMethodX = m_nMtdX;
	dlg.m_nMethodY = m_nMtdY;
	dlg.m_dHnX = m_dHnX;
	dlg.m_dHnY = m_dHnY;
	dlg.m_nNX = m_nNX;
	dlg.m_nNY = m_nNY;
	dlg.m_dAcX = m_dAcX;
	dlg.m_dAcY = m_dAcY;

	if (dlg.DoModal() == IDOK)
	{
		CString cx, cy;
		cx.Format(_T("%-10.4g"), dlg.m_dPeriodX);
		cy.Format(_T("%-10.4g"), dlg.m_dPeriodY);
		m_editApprX.SetEditUnit(_tstof(cx));
		m_editApprY.SetEditUnit(_tstof(cy));

		m_nMtdX = dlg.m_nMethodX;
		m_nMtdY = dlg.m_nMethodY;
		m_dHnX = dlg.m_dHnX;
		m_dHnY = dlg.m_dHnY;
		m_nNX = dlg.m_nNX;
		m_nNY = dlg.m_nNY;
		m_dAcX = dlg.m_dAcX;
		m_dAcY = dlg.m_dAcY;
	}
}

void CSeisItemKBC2005::OnCmdPeriodRdo()
{
	CtrlManager();
	//  int nPeriodMethod;
	//  CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadioPeriod, nPeriodMethod);
	//  CDlgUtil::CtrlEnableDisable(this, m_aCtrlPeriodAnal, nPeriodMethod == 0);
	OnChangeCmdPeriodEdit();
}

void CSeisItemKBC2005::OnChangeCmdPeriodEdit()
{
	double dTXAnal = m_editAnalX.GetEditValue();
	double dTXAppr = m_editApprX.GetEditValue();
	double dTYAnal = m_editAnalY.GetEditValue();
	double dTYAppr = m_editApprY.GetEditValue();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadioPeriod, nCheck);
	int nCodeType = 13;
	double dTXFinal = pDoc->m_pAttrCtrl->CalcFundamentalPeriod(nCodeType, nCheck, dTXAnal, dTXAppr);
	double dTYFinal = pDoc->m_pAttrCtrl->CalcFundamentalPeriod(nCodeType, nCheck, dTYAnal, dTYAppr);

	CString str;
	str.Format(_T("%-10.4g"), dTXFinal);
	m_editFinalX.SetWindowText(str);
	str.Format(_T("%-10.4g"), dTYFinal);
	m_editFinalY.SetWindowText(str);
}

void CSeisItemKBC2005::InitZoneCombo()
{
	CString aItem[2] = {_T("Zone1(0.11)"), _T("Zone2(0.07)")};

	m_cmbSeisZone.ResetContent();
	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSeisZone.AddString(aItem[i]);
	m_cmbSeisZone.SetCurSel(0);
}

void CSeisItemKBC2005::InitSiteCombo()
{
	CString aItem[5] = {_T("Sa"), _T("Sb"), _T("Sc"), _T("Sd"), _T("Se")};

	m_cmbSiteClass.ResetContent();
	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSiteClass.AddString(aItem[i]);
	m_cmbSiteClass.SetCurSel(3);
}

void CSeisItemKBC2005::InitIFactorCombo()
{
	CString aItem[4] = {_T("0.8"), _T("1.0"), _T("1.2"), _T("1.5")};

	m_cmbImportance.ResetContent();
	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbImportance.AddString(aItem[i]);
	m_cmbImportance.SetCurSel(2);
}

void CSeisItemKBC2005::InitUseGroupCombo()
{
	CString aItem[3] = {_LS(IDS_CMD_SEIS_SPECIAL), _T("I"), _T("II")};

	m_cmbUseGroup.ResetContent();
	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbUseGroup.AddString(aItem[i]);
	m_cmbUseGroup.SetCurSel(1);
}

void CSeisItemKBC2005::InitRxRyCombo()
{
	CString aItem[11] = {
		_T("1.25"), _T("1.5"), _T("2.5"), _T("3"), _T("4.5"), _T("5"), _T("5.5"), _T("6"), _T("6.5"), _T("7"), _T("8")
	};

	m_cmbRx.ResetContent();
	m_cmbRy.ResetContent();

	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
	{
		m_cmbRx.AddString(aItem[i]);
		m_cmbRy.AddString(aItem[i]);
	}
	m_cmbRx.SetCurSel(4);
	m_cmbRy.SetCurSel(4);
}

void CSeisItemKBC2005::Data2Dlg()
{
	CString csValue;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRadioAcc, m_Data.nAccCalcMethod);
	m_cmbSeisZone.SetCurSel(m_Data.nSeismicZone);
	m_cmbSiteClass.SetCurSel(m_Data.nSiteClass);
	m_editSds.SetEditUnit(m_Data.dSds);
	m_editSd1.SetEditUnit(m_Data.dSd1);
	m_cmbUseGroup.SetCurSel(m_Data.nSeismicUseGroup);
	m_btnCityPlan.SetCheck(m_Data.bCityPlanRegion);
	//   csValue.Format(_T("%g"), m_Data.dImportanceFactor);
	// 	m_cmbImportance.SetWindowText(csValue); 
	csValue.Format(_T("%g"), m_Data.dImportanceFactor);
	if (CB_ERR == m_cmbImportance.FindStringExact(-1, csValue))
	{
		m_cmbImportance.SetWindowText(csValue);
	}
	else
	{
		m_cmbImportance.SelectString(-1, csValue);
	}

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRadioPeriod, m_Data.nPeriodMethod);
	m_editAnalX.SetEditUnit(m_Data.dPeriodAnalX);
	m_editAnalY.SetEditUnit(m_Data.dPeriodAnalY);
	m_editApprX.SetEditUnit(m_Data.dPeriodApprX);
	m_editApprY.SetEditUnit(m_Data.dPeriodApprY);
	m_editFinalX.SetEditUnit(m_Data.dPeriodFinalX);
	m_editFinalY.SetEditUnit(m_Data.dPeriodFinalY);

	csValue.Format(_T("%g"), m_Data.dResponseModifyFactorX);
	m_cmbRx.SetWindowText(csValue);
	csValue.Format(_T("%g"), m_Data.dResponseModifyFactorY);
	m_cmbRy.SetWindowText(csValue);
}

BOOL CSeisItemKBC2005::Dlg2Data()
{
	CString str;

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadioAcc, m_Data.nAccCalcMethod);
	m_Data.nSeismicZone = m_cmbSeisZone.GetCurSel();
	m_Data.nSiteClass = m_cmbSiteClass.GetCurSel();
	m_Data.dSds = m_editSds.GetEditValue();
	m_Data.dSd1 = m_editSd1.GetEditValue();
	m_Data.nSeismicUseGroup = m_cmbUseGroup.GetCurSel();
	m_Data.bCityPlanRegion = m_btnCityPlan.GetCheck();
	m_cmbImportance.GetWindowText(str);
	m_Data.dImportanceFactor = _tstof(str);

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadioPeriod, m_Data.nPeriodMethod);
	m_Data.dPeriodAnalX = m_editAnalX.GetEditValue();
	m_Data.dPeriodAnalY = m_editAnalY.GetEditValue();
	m_Data.dPeriodApprX = m_editApprX.GetEditValue();
	m_Data.dPeriodApprY = m_editApprY.GetEditValue();
	//  m_Data.dPeriodFinalX = m_editFinalX.GetEditValue();
	//  m_Data.dPeriodFinalY = m_editFinalY.GetEditValue();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	int nCodeType = 13;
	m_Data.dPeriodFinalX = pDoc->m_pAttrCtrl->CalcFundamentalPeriod(nCodeType, m_Data.nPeriodMethod,
	                                                                m_Data.dPeriodAnalX, m_Data.dPeriodApprX);
	m_Data.dPeriodFinalY = pDoc->m_pAttrCtrl->CalcFundamentalPeriod(nCodeType, m_Data.nPeriodMethod,
	                                                                m_Data.dPeriodAnalY, m_Data.dPeriodApprY);
	m_cmbRx.GetWindowText(str);
	m_Data.dResponseModifyFactorX = _tstof(str);
	m_cmbRy.GetWindowText(str);
	m_Data.dResponseModifyFactorY = _tstof(str);

	return TRUE;
}

LRESULT CSeisItemKBC2005::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	CRect rcChild;
	GetWindowRect(rcChild);
	m_pParent->AlignCtrl2Param(rcChild.bottom);

	return 0l;
}
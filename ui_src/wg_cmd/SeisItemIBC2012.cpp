// SeisItemIBC2012.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemIBC2012.h"
#include "SeisIBC2012PeriodDlg.h"
#include "SeisItemDlgNew.h"

#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\Seismic_NSCP2024.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisItemIBC2012 dialog


CSeisItemIBC2012::CSeisItemIBC2012(CWnd* pParent /*=NULL*/, int nCode/* = 0*/)
	: CChildDialog(CSeisItemIBC2012::IDD, pParent)
{
	m_pParent = (CSeisItemDlgNew*)pParent;
	m_aCtrlRadioPeriod.Add(IDC_CMD_PERIOD_RDO);
	m_aCtrlRadioPeriod.Add(IDC_CMD_PERIOD_RDO2);

	m_aCtrlPeriodAnal.Add(IDC_CMD_TX_ANAL_EDIT);
	m_aCtrlPeriodAnal.Add(IDC_CMD_TY_ANAL_EDIT);

	m_bInit = FALSE;
	m_Data.Initialize();

	m_nCode = nCode;
}

void CSeisItemIBC2012::SetCode(int nCode)
{
	m_nCode = (nCode == 0) ? IBC_E_2012 : nCode;

	const CString& strCategory = [](int nCode)
	{
		switch (nCode)
		{
		case IBC_E_2009:
			return _LS(IDS_WG_CMD_OCCUPANCY_CATEGORY);
		case IBC_E_2012:
		case NSCP_E_2024:
			return _LS(IDS_WG_CMD_RISK_CATEGORY);
		default:
			ASSERT(0);
			return _LS(IDS_WG_CMD_RISK_CATEGORY);
		}
	}(m_nCode);

	SetDlgItemText(IDC_CMD_CATEGORY_STC, strCategory);
}

void CSeisItemIBC2012::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_SS_CMB, m_cmbSs);
	DDX_Control(pDX, IDC_CMD_S1_CMB, m_cmbS1);
	DDX_Control(pDX, IDC_CMD_SITE_CMB, m_cmbSiteClass);
	DDX_Control(pDX, IDC_CMD_CATEGORY_CMB, m_cmbCategory);
	DDX_Control(pDX, IDC_CMD_IMPORTANCE_CMB, m_cmbImportance);
	DDX_Control(pDX, IDC_CMD_RY_CMB, m_cmbRy);
	DDX_Control(pDX, IDC_CMD_RX_CMB, m_cmbRx);
	DDX_Control(pDX, IDC_CMD_CU_EDIT, m_editCu);
	DDX_Control(pDX, IDC_CMD_FA_EDIT, m_editFa);
	DDX_Control(pDX, IDC_CMD_FV_EDIT, m_editFv);
	DDX_Control(pDX, IDC_CMD_SDS_EDIT, m_editSds);
	DDX_Control(pDX, IDC_CMD_SD1_EDIT, m_editSd1);
	DDX_Control(pDX, IDC_CMD_TL_EDIT, m_editTL);
	DDX_Control(pDX, IDC_CMD_SDC_S_EDIT, m_editSDC_S);
	DDX_Control(pDX, IDC_CMD_SDC_1_EDIT, m_editSDC_1);
	DDX_Control(pDX, IDC_CMD_SDC_EDIT, m_editSDC);
	DDX_Control(pDX, IDC_CMD_TX_ANAL_EDIT, m_editAnalX);
	DDX_Control(pDX, IDC_CMD_TY_ANAL_EDIT, m_editAnalY);
	DDX_Control(pDX, IDC_CMD_TX_APPR_EDIT, m_editApprX);
	DDX_Control(pDX, IDC_CMD_TY_APPR_EDIT, m_editApprY);
	DDX_Control(pDX, IDC_CMD_TX_FINAL_EDIT, m_editFinalX);
	DDX_Control(pDX, IDC_CMD_TY_FINAL_EDIT, m_editFinalY);
	DDX_Control(pDX, IDC_WG_CMD_STATIC0, m_grpFold);
}

BEGIN_MESSAGE_MAP(CSeisItemIBC2012, CChildDialog)
	ON_CBN_SELCHANGE(IDC_CMD_SS_CMB, OnSelchangeCmdSsCmb)
	ON_CBN_SELCHANGE(IDC_CMD_S1_CMB, OnSelchangeCmdS1Cmb)
	ON_CBN_SELCHANGE(IDC_CMD_SITE_CMB, OnSelchangeCmdSiteCmb)
	ON_CBN_SELCHANGE(IDC_CMD_CATEGORY_CMB, OnSelchangeCmdCategoryCmb)

	ON_BN_CLICKED(IDC_CMD_PERIOD_BTN, OnCmdPeriodBtn)
	ON_BN_CLICKED(IDC_CMD_PERIOD_RDO, OnCmdPeriodRdo)
	ON_BN_CLICKED(IDC_CMD_PERIOD_RDO2, OnCmdPeriodRdo)

	ON_CBN_EDITCHANGE(IDC_CMD_SS_CMB, OnChangeCmdSs)
	ON_CBN_EDITCHANGE(IDC_CMD_S1_CMB, OnChangeCmdS1)

	ON_EN_CHANGE(IDC_CMD_FA_EDIT, OnChangeCmdFaEdit)
	ON_EN_CHANGE(IDC_CMD_FV_EDIT, OnChangeCmdFvEdit)
	ON_EN_CHANGE(IDC_CMD_SDS_EDIT, OnChangeCmdSdsEdit)
	ON_EN_CHANGE(IDC_CMD_SD1_EDIT, OnChangeCmdSd1Edit)
	ON_EN_CHANGE(IDC_CMD_TX_ANAL_EDIT, OnChangeCmdPeriodEdit)
	ON_EN_CHANGE(IDC_CMD_TX_APPR_EDIT, OnChangeCmdPeriodEdit)
	ON_EN_CHANGE(IDC_CMD_TY_ANAL_EDIT, OnChangeCmdPeriodEdit)
	ON_EN_CHANGE(IDC_CMD_TY_APPR_EDIT, OnChangeCmdPeriodEdit)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisItemIBC2012 message handlers

BOOL CSeisItemIBC2012::OnInitDialog()
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

	InitSsCombo();
	InitS1Combo();
	InitSiteCombo();
	InitCategoryCombo();
	InitImportanceFactorCombo();
	InitRxRyCombo();

	SetFoldGroupCtrls();
	if (!m_bModify) m_Data.Initialize();
	Data2Dlg();

	OnCmdPeriodRdo();

	SetSeisDesignCategory();

	CtrlManager();

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisItemIBC2012::SetFoldGroupCtrls()
{
	// After AlignControls
	m_grpFold.InitControl(this, CSeisItemIBC2012::IDD, IDC_WG_CMD_STATIC0, TRUE, TRUE);
	m_grpFold.SetFoldState(FALSE); // open
}

BOOL CSeisItemIBC2012::DestroyWindow()
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

void CSeisItemIBC2012::CtrlManager()
{
	int nPeriodMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadioPeriod, nPeriodMethod);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlPeriodAnal, nPeriodMethod == 0);
	UpdateData(FALSE);
}

void CSeisItemIBC2012::SetData2Dlg(T_SEIS_IBC2012& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CSeisItemIBC2012::SetDlg2Data(T_SEIS_IBC2012& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CSeisItemIBC2012::InitDefaultData()
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

	double dBaseLevel = pAttrCtrl->GetBaseLevel(FALSE);
	dHn = StorD.dStoryLevel - dBaseLevel;
	if (!pAttrCtrl->GetCountStor()) dHn = 0;

	m_nNX = m_nNY = nStorNum - 1;
	m_dHnX = m_dHnY = dHn;
	m_dCwX = m_dCwY = 0.0;
	m_nMtdX = m_nMtdY = 0;

	// <Remember> Data를 받고나서는 User Unit기준
	pUnitCtrl->SetUnitIndexCurrent(CurIndex);
}

void CSeisItemIBC2012::SetFaFvSdsSd1()
{
	CString str;
	m_cmbSs.GetWindowText(str);
	double dSs = _tstof(str);
	m_cmbS1.GetWindowText(str);
	double dS1 = _tstof(str);

	int nSiteClass = m_cmbSiteClass.GetCurSel();

	double dFa = 0.0;
	double dFv = 0.0;
	double dSds = 0.0;
	double dSd1 = 0.0;
	double dCu = 0.0;
	switch (m_nCode)
	{
	case IBC_E_2009:
	case IBC_E_2012:
		{
			CDBDoc* pDoc = CDBDoc::GetDocPoint();
			if (!pDoc->m_pAttrCtrl2->CalcIBC2012Fa(nSiteClass, dSs, dFa)) return;
			if (!pDoc->m_pAttrCtrl2->CalcIBC2012Fv(nSiteClass, dS1, dFv)) return;
			pDoc->m_pAttrCtrl2->CalcIBC2012Sds(dSs, dFa, dSds);
			pDoc->m_pAttrCtrl2->CalcIBC2012Sd1(dS1, dFv, dSd1);
			pDoc->m_pAttrCtrl2->CalcIBC2012Cu(dSd1, dCu);
		}
		break;
	case NSCP_E_2024:
		{
			CSeismic_NSCP2024 CodeCalc;
			dFa = CodeCalc.CalcFa(nSiteClass, dSs);
			dFv = CodeCalc.CalcFv(nSiteClass, dS1);
			dSds = CodeCalc.CalcSds(dSs, dFa);
			dSd1 = CodeCalc.CalcSd1(dS1, dFv);
			dCu = CodeCalc.CalcCu(dSd1);
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	m_editCu.SetEditUnit(dCu, 5);
	m_editFa.SetEditUnit(dFa, 5);
	m_editFv.SetEditUnit(dFv, 5);
	m_editSds.SetEditUnit(dSds, 5);
	m_editSd1.SetEditUnit(dSd1, 5);
	OnChangeCmdPeriodEdit();
}

void CSeisItemIBC2012::SetSeisDesignCategory()
{
	int nCategory = m_cmbCategory.GetCurSel();

	CString str;
	m_cmbS1.GetWindowText(str);
	double dS1 = _tstof(str);
	double dSds = m_editSds.GetEditValue();
	double dSd1 = m_editSd1.GetEditValue();

	CString SDC_S, SDC_1, SDC;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pAttrCtrl2->CalcIBC2012SeisDesignCategory(nCategory, dS1, dSds, dSd1,
	                                                       SDC_S, SDC_1, SDC))
		return;
	m_editSDC_S.SetWindowText(SDC_S);
	m_editSDC_1.SetWindowText(SDC_1);
	m_editSDC.SetWindowText(SDC);
}

void CSeisItemIBC2012::SetImportanceFactor()
{
	int nCategory = m_cmbCategory.GetCurSel();

	if (nCategory == 0 || nCategory == 1)
	{
		m_cmbImportance.SetWindowText(_T("1.0"));
	}
	else if (nCategory == 2)
	{
		m_cmbImportance.SetWindowText(_T("1.25"));
	}
	else
	{
		m_cmbImportance.SetWindowText(_T("1.5"));
	}
}

void CSeisItemIBC2012::OnSelchangeCmdSiteCmb()
{
	SetFaFvSdsSd1();
	SetSeisDesignCategory();
}

void CSeisItemIBC2012::OnSelchangeCmdSsCmb()
{
	// Edit Box 값을 변경후에 Combo 로 값을 변경하면 바로 Update 안되는 문제...

	double dSs;
	int nTmp = m_cmbSs.GetCurSel();
	if (nTmp == 0) dSs = 0.25;
	else if (nTmp == 1) dSs = 0.5;
	else if (nTmp == 2) dSs = 0.75;
	else if (nTmp == 3) dSs = 1.0;
	else if (nTmp == 4) dSs = 1.25;

	CString csValue;
	csValue.Format(_T("%g"), dSs);
	m_cmbSs.SetWindowText(csValue);

	SetFaFvSdsSd1();
	SetSeisDesignCategory();
}

void CSeisItemIBC2012::OnSelchangeCmdS1Cmb()
{
	// Edit Box 값을 변경후에 Combo 로 값을 변경하면 바로 Update 안되는 문제...

	double dS1;
	int nTmp = m_cmbS1.GetCurSel();
	if (nTmp == 0) dS1 = 0.1;
	else if (nTmp == 1) dS1 = 0.2;
	else if (nTmp == 2) dS1 = 0.3;
	else if (nTmp == 3) dS1 = 0.4;
	else if (nTmp == 4) dS1 = 0.5;

	CString csValue;
	csValue.Format(_T("%g"), dS1);
	m_cmbS1.SetWindowText(csValue);

	SetFaFvSdsSd1();
	SetSeisDesignCategory();
}

void CSeisItemIBC2012::OnChangeCmdSs()
{
	SetFaFvSdsSd1();
	SetSeisDesignCategory();
}

void CSeisItemIBC2012::OnChangeCmdS1()
{
	SetFaFvSdsSd1();
	SetSeisDesignCategory();
}

void CSeisItemIBC2012::OnChangeCmdFaEdit()
{
	CString str;
	m_cmbSs.GetWindowText(str);
	double dSs = _tstof(str);
	double dFa = m_editFa.GetEditValue();
	double dSds = 0.;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl2->CalcIBC2012Sds(dSs, dFa, dSds);

	m_editSds.SetEditUnit(dSds, 5);
	SetSeisDesignCategory();
}

void CSeisItemIBC2012::OnChangeCmdFvEdit()
{
	CString str;
	m_cmbS1.GetWindowText(str);
	double dS1 = _tstof(str);
	double dFv = m_editFv.GetEditValue();
	double dSd1 = 0., dCu = 0.;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl2->CalcIBC2012Sd1(dS1, dFv, dSd1);
	pDoc->m_pAttrCtrl2->CalcIBC2012Cu(dSd1, dCu);

	m_editCu.SetEditUnit(dCu, 5);
	m_editSd1.SetEditUnit(dSd1, 5);
	SetSeisDesignCategory();
	OnChangeCmdPeriodEdit();
}

void CSeisItemIBC2012::OnChangeCmdSdsEdit()
{
	SetSeisDesignCategory();
}

void CSeisItemIBC2012::OnChangeCmdSd1Edit()
{
	double dSd1 = m_editSd1.GetEditValue();
	double dCu = 0.;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl2->CalcIBC2012Cu(dSd1, dCu);

	m_editCu.SetEditUnit(dCu, 5);
	SetSeisDesignCategory();
	OnChangeCmdPeriodEdit();
}

void CSeisItemIBC2012::OnSelchangeCmdCategoryCmb()
{
	SetImportanceFactor();
	SetSeisDesignCategory();
}

void CSeisItemIBC2012::OnCmdPeriodBtn()
{
	CSeisIBC2012PeriodDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.SetCode(m_nCode);

	/// CSeisItemIBC2012::InitDefaultData()에서 m_dHnX, m_dHnY는 M단위계로 들어온다. 
	dlg.m_nMethodX = m_nMtdX;
	dlg.m_nMethodY = m_nMtdY;
	dlg.m_dHnX = m_dHnX;
	dlg.m_dHnY = m_dHnY;
	dlg.m_nNX = m_nNX;
	dlg.m_nNY = m_nNY;
	dlg.m_dCwX = m_dCwX;
	dlg.m_dCwY = m_dCwY;

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
		m_dCwX = dlg.m_dCwX;
		m_dCwY = dlg.m_dCwY;

		int nCheck;
		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadioPeriod, nCheck);
		if(nCheck == 0)  // Analytical
		{
			OnChangeCmdPeriodEdit();
		}
	}
}

void CSeisItemIBC2012::OnCmdPeriodRdo()
{
	CtrlManager();
	OnChangeCmdPeriodEdit();
}

void CSeisItemIBC2012::OnChangeCmdPeriodEdit()
{
	double dTXAnal = m_editAnalX.GetEditValue();
	double dTXAppr = m_editApprX.GetEditValue();
	double dTYAnal = m_editAnalY.GetEditValue();
	double dTYAppr = m_editApprY.GetEditValue();
	double dCu = m_editCu.GetEditValue();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadioPeriod, nCheck);
	int nCodeType = 17;
	double dTXFinal = pDoc->m_pAttrCtrl->CalcFundamentalPeriodKBC2009(nCodeType, nCheck, dTXAnal, dTXAppr, dCu);
	double dTYFinal = pDoc->m_pAttrCtrl->CalcFundamentalPeriodKBC2009(nCodeType, nCheck, dTYAnal, dTYAppr, dCu);

	CString str;
	str.Format(_T("%-10.4g"), dTXFinal);
	m_editFinalX.SetWindowText(str);
	str.Format(_T("%-10.4g"), dTYFinal);
	m_editFinalY.SetWindowText(str);
}

void CSeisItemIBC2012::InitSsCombo()
{
	CString aItem[5] = {_T("0.25"), _T("0.5"), _T("0.75"), _T("1.0"), _T("1.25")};

	m_cmbSs.ResetContent();
	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSs.AddString(aItem[i]);
	m_cmbSs.SetCurSel(2);
}

void CSeisItemIBC2012::InitS1Combo()
{
	CString aItem[5] = {_T("0.1"), _T("0.2"), _T("0.3"), _T("0.4"), _T("0.5")};

	m_cmbS1.ResetContent();
	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbS1.AddString(aItem[i]);
	m_cmbS1.SetCurSel(2);
}

void CSeisItemIBC2012::InitSiteCombo()
{
	CString aItem[5] = {_T("A"), _T("B"), _T("C"), _T("D"), _T("E")};

	m_cmbSiteClass.ResetContent();
	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSiteClass.AddString(aItem[i]);
	m_cmbSiteClass.SetCurSel(3);
}

void CSeisItemIBC2012::InitCategoryCombo()
{
	CString aItem[4] = {_T("I"), _T("II"), _T("III"), _T("IV")};

	m_cmbCategory.ResetContent();
	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbCategory.AddString(aItem[i]);
	m_cmbCategory.SetCurSel(1);
}

void CSeisItemIBC2012::InitImportanceFactorCombo()
{
	CString aItem[3] = {_T("1.0"), _T("1.25"), _T("1.5")};

	m_cmbImportance.ResetContent();
	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbImportance.AddString(aItem[i]);
	m_cmbImportance.SetCurSel(0);
}

void CSeisItemIBC2012::InitRxRyCombo()
{
	CString aItem[15] = {
		_T("1.5"), _T("2"), _T("2.5"), _T("3"), _T("3.25"), _T("3.5"), _T("4"), _T("4.5"), _T("5"), _T("5.5"), _T("6"),
		_T("6.5"), _T("7"), _T("7.5"), _T("8")
	};

	m_cmbRx.ResetContent();
	m_cmbRy.ResetContent();

	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
	{
		m_cmbRx.AddString(aItem[i]);
		m_cmbRy.AddString(aItem[i]);
	}
	m_cmbRx.SetCurSel(6);
	m_cmbRy.SetCurSel(6);
}

void CSeisItemIBC2012::Data2Dlg()
{
	CString csValue;
	csValue.Format(_T("%g"), m_Data.dSs);
	m_cmbSs.SetWindowText(csValue);

	csValue.Format(_T("%g"), m_Data.dS1);
	m_cmbS1.SetWindowText(csValue);

	m_cmbSiteClass.SetCurSel(m_Data.nSiteClass);

	m_editFa.SetEditUnit(m_Data.dFa, 5);
	m_editFv.SetEditUnit(m_Data.dFv, 5);
	m_editSds.SetEditUnit(m_Data.dSds, 5);
	m_editSd1.SetEditUnit(m_Data.dSd1, 5);
	m_editTL.SetEditUnit(m_Data.dTL);

	double dCu = 0.;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl2->CalcIBC2012Cu(m_Data.dSd1, dCu);
	m_editCu.SetEditUnit(dCu, 5);

	m_cmbCategory.SetCurSel(m_Data.nRiskCategory);

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

BOOL CSeisItemIBC2012::Dlg2Data()
{
	CString str;

	m_cmbSs.GetWindowText(str);
	m_Data.dSs = _tstof(str);

	m_cmbS1.GetWindowText(str);
	m_Data.dS1 = _tstof(str);

	m_Data.nSiteClass = m_cmbSiteClass.GetCurSel();

	m_Data.dFa = m_editFa.GetEditValue();
	m_Data.dFv = m_editFv.GetEditValue();
	m_Data.dSds = m_editSds.GetEditValue();
	m_Data.dSd1 = m_editSd1.GetEditValue();
	m_Data.dTL = m_editTL.GetEditValue();

	m_Data.nRiskCategory = m_cmbCategory.GetCurSel();

	m_cmbImportance.GetWindowText(str);
	m_Data.dImportanceFactor = _tstof(str);

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadioPeriod, m_Data.nPeriodMethod);
	m_Data.dPeriodAnalX = m_editAnalX.GetEditValue();
	m_Data.dPeriodAnalY = m_editAnalY.GetEditValue();
	m_Data.dPeriodApprX = m_editApprX.GetEditValue();
	m_Data.dPeriodApprY = m_editApprY.GetEditValue();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dCu = 0.;
	pDoc->m_pAttrCtrl2->CalcIBC2012Cu(m_Data.dSd1, dCu);
	int nCodeType = 17;
	m_Data.dPeriodFinalX = pDoc->m_pAttrCtrl->CalcFundamentalPeriodKBC2009(
		nCodeType, m_Data.nPeriodMethod, m_Data.dPeriodAnalX, m_Data.dPeriodApprX, dCu);
	m_Data.dPeriodFinalY = pDoc->m_pAttrCtrl->CalcFundamentalPeriodKBC2009(
		nCodeType, m_Data.nPeriodMethod, m_Data.dPeriodAnalY, m_Data.dPeriodApprY, dCu);
	m_cmbRx.GetWindowText(str);
	m_Data.dResponseModifyFactorX = _tstof(str);
	m_cmbRy.GetWindowText(str);
	m_Data.dResponseModifyFactorY = _tstof(str);

	return TRUE;
}

LRESULT CSeisItemIBC2012::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	CRect rcChild;
	GetWindowRect(rcChild);
	m_pParent->AlignCtrl2Param(rcChild.bottom);

	return 0l;
}

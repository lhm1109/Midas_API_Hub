// SeisItemIBC2012.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemNSR2010.h"
#include "SeisNSR2010PeroidDlg.h"
#include "SeisNSR2010CalcPhiDlg.h"
#include "SeisItemDlgNew.h"
#include "..\wg_db\CalcNSR2010.h"

#include "..\wg_db\wg_db_AttrCtrl2.h"
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
// CSeisItemNSR2010 dialog


CSeisItemNSR2010::CSeisItemNSR2010(CWnd* pParent /*=NULL*/)
	: CChildDialog(CSeisItemNSR2010::IDD, pParent)
{
	m_aCtrlRadioPeriod.Add(IDC_CMD_PERIOD_RDO);
	m_aCtrlRadioPeriod.Add(IDC_CMD_PERIOD_RDO2);

	m_aCtrlPeriodAnal.Add(IDC_CMD_TX_ANAL_EDIT);
	m_aCtrlPeriodAnal.Add(IDC_CMD_TY_ANAL_EDIT);

	m_bInit = FALSE;
	m_Data.Initialize();
	m_pParent = (CSeisItemDlgNew*)pParent;
}

void CSeisItemNSR2010::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisItemNSR2010)
	DDX_Control(pDX, IDC_CMD_SITE_CMB, m_cmbSiteClass);
	DDX_Control(pDX, IDC_CMD_DSRA_AA_CMB, m_cmbAA);
	DDX_Control(pDX, IDC_CMD_DSRA_AV_CMB, m_cmbAV);
	DDX_Control(pDX, IDC_CMD_DSRA_FA_EDIT, m_editFA);
	DDX_Control(pDX, IDC_CMD_DSRA_FV_EDIT, m_editFV);
	DDX_Control(pDX, IDC_CMD_CU_EDIT, m_editCu);
	DDX_Control(pDX, IDC_CMD_IMPORTANCE_CMB, m_cmbImportance);
	DDX_Control(pDX, IDC_CMD_TX_ANAL_EDIT, m_editAnalX);
	DDX_Control(pDX, IDC_CMD_TY_ANAL_EDIT, m_editAnalY);
	DDX_Control(pDX, IDC_CMD_TX_APPR_EDIT, m_editApprX);
	DDX_Control(pDX, IDC_CMD_TY_APPR_EDIT, m_editApprY);
	DDX_Control(pDX, IDC_CMD_TX_FINAL_EDIT, m_editFinalX);
	DDX_Control(pDX, IDC_CMD_TY_FINAL_EDIT, m_editFinalY);
	DDX_Control(pDX, IDC_CMD_RY_CMB, m_cmbRy);
	DDX_Control(pDX, IDC_CMD_RX_CMB, m_cmbRx);
	DDX_Control(pDX, IDC_CMD_PHI_X_EDIT, m_editPhiX);
	DDX_Control(pDX, IDC_CMD_PHI_Y_EDIT, m_editPhiY);
	DDX_Control(pDX, IDC_WG_CMD_STATIC0, m_grpFold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeisItemNSR2010, CChildDialog)
	//{{AFX_MSG_MAP(CSeisItemNSR2010)
	ON_CBN_SELCHANGE(IDC_CMD_SITE_CMB, OnSelchangeCmdSiteCmb)
	ON_CBN_SELCHANGE(IDC_CMD_DSRA_AA_CMB, OnSelchangeCmdAaCmb)
	ON_CBN_SELCHANGE(IDC_CMD_DSRA_AV_CMB, OnSelchangeCmdAvCmb)
	ON_CBN_EDITCHANGE(IDC_CMD_DSRA_AA_CMB, OnChangeCmdAa)
	ON_CBN_EDITCHANGE(IDC_CMD_DSRA_AV_CMB, OnChangeCmdAv)
	ON_EN_CHANGE(IDC_CMD_DSRA_FV_EDIT, OnChangeCmdFvEdit)

	ON_BN_CLICKED(IDC_CMD_PERIOD_BTN, OnCmdPeriodBtn)
	ON_BN_CLICKED(IDC_CMD_PERIOD_RDO, OnCmdPeriodRdo)
	ON_BN_CLICKED(IDC_CMD_PERIOD_RDO2, OnCmdPeriodRdo)

	ON_EN_CHANGE(IDC_CMD_TX_ANAL_EDIT, OnChangeCmdPeriodEdit)
	ON_EN_CHANGE(IDC_CMD_TX_APPR_EDIT, OnChangeCmdPeriodEdit)
	ON_EN_CHANGE(IDC_CMD_TY_ANAL_EDIT, OnChangeCmdPeriodEdit)
	ON_EN_CHANGE(IDC_CMD_TY_APPR_EDIT, OnChangeCmdPeriodEdit)
	ON_BN_CLICKED(IDC_CMD_PHI_BTN, OnCmdPhiBtn)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisItemNSR2010 message handlers

BOOL CSeisItemNSR2010::OnInitDialog()
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
	m_editPhiX.SetUnitType(D_UNITSYS_NONE);
	m_editPhiY.SetUnitType(D_UNITSYS_NONE);

	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}

	InitAaCombo();
	InitAvCombo();
	InitSiteCombo();
	InitImportanceFactorCombo();
	InitRxRyCombo();

	if (!m_bModify) m_Data.Initialize();
	Data2Dlg();

	SetCu();
	OnCmdPeriodRdo();

	CtrlManager();

	SetFoldGroupCtrls();
	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSeisItemNSR2010::DestroyWindow()
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

void CSeisItemNSR2010::CtrlManager()
{
	int nPeriodMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadioPeriod, nPeriodMethod);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlPeriodAnal, nPeriodMethod == 0);
	UpdateData(FALSE);
}
void CSeisItemNSR2010::SetFoldGroupCtrls()
{
	// After AlignControls
	m_grpFold.InitControl(this, CSeisItemNSR2010::IDD, IDC_WG_CMD_STATIC0, TRUE, TRUE);
	m_grpFold.SetFoldState(FALSE); // open
}

void CSeisItemNSR2010::SetData2Dlg(T_SEIS_NSR2010& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CSeisItemNSR2010::SetDlg2Data(T_SEIS_NSR2010& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CSeisItemNSR2010::InitDefaultData()
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

void CSeisItemNSR2010::SetFa()
{
	CString str;
	m_cmbAA.GetWindowText(str);
	double dAa = _tstof(str);

	int nSiteClass = m_cmbSiteClass.GetCurSel();

	double dFa = 0.;

	if (!CCalcNSR2010::CalcFa(nSiteClass, dAa, dFa)) return;

	m_editFA.SetEditUnit(dFa, 5);
}

void CSeisItemNSR2010::SetFv()
{
	CString str;
	m_cmbAV.GetWindowText(str);
	double dAv = _tstof(str);

	int nSiteClass = m_cmbSiteClass.GetCurSel();

	double dFv = 0.;

	if (!CCalcNSR2010::CalcFv(nSiteClass, dAv, dFv)) return;

	m_editFV.SetEditUnit(dFv, 5);
	SetCu();
}

void CSeisItemNSR2010::SetCu()
{
	CString str;
	m_cmbAV.GetWindowText(str);
	double dAv = _tstof(str);
	m_editFV.GetWindowText(str);
	double dFv = _tstof(str);

	int nSiteClass = m_cmbSiteClass.GetCurSel();

	double dCu = CCalcNSR2010::CalcCu(dAv, dFv);

	m_editCu.SetEditUnit(dCu, 5);
	OnChangeCmdPeriodEdit();
}

void CSeisItemNSR2010::OnSelchangeCmdSiteCmb()
{
	SetFa();
	SetFv();
}

void CSeisItemNSR2010::OnSelchangeCmdAaCmb()
{
	// Edit Box 값을 변경후에 Combo 로 값을 변경하면 바로 Update 안되는 문제...

	double dAa = 0.0;
	int nTmp = m_cmbAA.GetCurSel();
	CArray<double, double> aAaItem;
	aAaItem.RemoveAll();
	CCalcNSR2010::GetAaDefaultList(aAaItem);
	if (nTmp < aAaItem.GetCount())
	{
		dAa = aAaItem[nTmp];
	}

	CString csValue;
	csValue.Format(_T("%g"), dAa);
	m_cmbAA.SetWindowText(csValue);

	SetFa();
}

void CSeisItemNSR2010::OnSelchangeCmdAvCmb()
{
	// Edit Box 값을 변경후에 Combo 로 값을 변경하면 바로 Update 안되는 문제...

	double dAv;
	int nTmp = m_cmbAV.GetCurSel();
	CArray<double, double> aAvItem;
	aAvItem.RemoveAll();
	CCalcNSR2010::GetAvDefaultList(aAvItem);
	if (nTmp < aAvItem.GetCount())
	{
		dAv = aAvItem[nTmp];
	}

	CString csValue;
	csValue.Format(_T("%g"), dAv);
	m_cmbAV.SetWindowText(csValue);

	SetFv();
}

void CSeisItemNSR2010::OnChangeCmdAa()
{
	SetFa();
}

void CSeisItemNSR2010::OnChangeCmdAv()
{
	SetFv();
}

void CSeisItemNSR2010::OnChangeCmdFvEdit()
{
	SetCu();
}

void CSeisItemNSR2010::OnCmdPeriodBtn()
{
	CSeisNSR2010PeriodDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);

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

void CSeisItemNSR2010::OnCmdPeriodRdo()
{
	CtrlManager();
	OnChangeCmdPeriodEdit();
}

void CSeisItemNSR2010::OnChangeCmdPeriodEdit()
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

void CSeisItemNSR2010::InitAaCombo()
{
	CArray<double, double> aAaItem;
	aAaItem.RemoveAll();
	CCalcNSR2010::GetAaDefaultList(aAaItem);
	CString strVal = _T("");
	m_cmbAA.ResetContent();

	for (int i = 0; i < aAaItem.GetCount(); i++)
	{
		strVal.Format(_T("%.2f"), aAaItem[i]);
		m_cmbAA.AddString(strVal);
	}
	m_cmbAA.SetCurSel(2);
}

void CSeisItemNSR2010::InitAvCombo()
{
	CArray<double, double> aAvItem;
	aAvItem.RemoveAll();
	CCalcNSR2010::GetAvDefaultList(aAvItem);
	CString strVal = _T("");

	m_cmbAV.ResetContent();
	for (int i = 0; i < aAvItem.GetCount(); i++)
	{
		strVal.Format(_T("%.2f"), aAvItem[i]);
		m_cmbAV.AddString(strVal);
	}
	m_cmbAV.SetCurSel(2);
}

void CSeisItemNSR2010::InitSiteCombo()
{
	CString aItem[5] = {_T("A"), _T("B"), _T("C"), _T("D"), _T("E")};

	m_cmbSiteClass.ResetContent();
	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSiteClass.AddString(aItem[i]);
	m_cmbSiteClass.SetCurSel(3);
}

void CSeisItemNSR2010::InitImportanceFactorCombo()
{
	CString aItem[4] = {_T("1.0"), _T("1.1"), _T("1.25"), _T("1.5")};

	m_cmbImportance.ResetContent();
	for (int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbImportance.AddString(aItem[i]);
	m_cmbImportance.SetCurSel(0);
}

void CSeisItemNSR2010::InitRxRyCombo()
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

void CSeisItemNSR2010::Data2Dlg()
{
	CString csValue;
	csValue.Format(_T("%g"), m_Data.dAa);
	m_cmbAA.SetWindowText(csValue);

	csValue.Format(_T("%g"), m_Data.dAv);
	m_cmbAV.SetWindowText(csValue);

	m_cmbSiteClass.SetCurSel(m_Data.nSiteClass);

	m_editFA.SetEditUnit(m_Data.dFa, 5);
	m_editFV.SetEditUnit(m_Data.dFv, 5);

	m_editCu.SetEditUnit(m_Data.dCu, 5);

	//   csValue.Format(_T("%g"), m_Data.dImportanceFactor);
	//   m_cmbImportance.SetWindowText(csValue); 
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

	m_editPhiX.SetEditUnit(m_Data.dPhiX);
	m_editPhiY.SetEditUnit(m_Data.dPhiY);
}

BOOL CSeisItemNSR2010::Dlg2Data()
{
	CString str;

	m_cmbAA.GetWindowText(str);
	m_Data.dAa = _tstof(str);

	m_cmbAV.GetWindowText(str);
	m_Data.dAv = _tstof(str);

	m_Data.nSiteClass = m_cmbSiteClass.GetCurSel();

	m_Data.dFa = m_editFA.GetEditValue();
	m_Data.dFv = m_editFV.GetEditValue();

	m_cmbImportance.GetWindowText(str);
	m_Data.dImportanceFactor = _tstof(str);

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadioPeriod, m_Data.nPeriodMethod);
	m_Data.dPeriodAnalX = m_editAnalX.GetEditValue();
	m_Data.dPeriodAnalY = m_editAnalY.GetEditValue();
	m_Data.dPeriodApprX = m_editApprX.GetEditValue();
	m_Data.dPeriodApprY = m_editApprY.GetEditValue();
	m_Data.dCu = m_editCu.GetEditValue();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	int nCodeType = 17;
	m_Data.dPeriodFinalX = pDoc->m_pAttrCtrl->CalcFundamentalPeriodKBC2009(
		nCodeType, m_Data.nPeriodMethod, m_Data.dPeriodAnalX, m_Data.dPeriodApprX, m_Data.dCu);
	m_Data.dPeriodFinalY = pDoc->m_pAttrCtrl->CalcFundamentalPeriodKBC2009(
		nCodeType, m_Data.nPeriodMethod, m_Data.dPeriodAnalY, m_Data.dPeriodApprY, m_Data.dCu);
	m_cmbRx.GetWindowText(str);
	m_Data.dResponseModifyFactorX = _tstof(str);
	m_cmbRy.GetWindowText(str);
	m_Data.dResponseModifyFactorY = _tstof(str);
	m_Data.dPhiX = m_editPhiX.GetEditValue();
	m_Data.dPhiY = m_editPhiY.GetEditValue();

	return TRUE;
}

void CSeisItemNSR2010::OnCmdPhiBtn()
{
	CSeisNSR2010CalcPhiDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);

	if (dlg.DoModal() == IDOK)
	{
		dlg.GetResult(m_Data.dPhiX, m_Data.dPhiY);
		m_editPhiX.SetEditUnit(m_Data.dPhiX);
		m_editPhiY.SetEditUnit(m_Data.dPhiY);
	}
}

LRESULT CSeisItemNSR2010::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	if (!m_grpFold.GetFoldState())
	{
		OnCmdPeriodRdo();
		CtrlManager();
	}

	CRect rcChild;
	GetWindowRect(rcChild);
	m_pParent->AlignCtrl2Param(rcChild.bottom);

	return 0l;
}

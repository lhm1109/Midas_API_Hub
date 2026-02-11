// CMWindPressureVelo_IS875_2015.cpp : implementation file

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindPressureVelo_IS875_2015.h"
#include "CMWindPressureVelocityAddMod.h"
#include "WindAutoCalcDlg.h"
#include "..\wg_db\WindLoadGeneratorCtrl.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureVelo_IS875_2015 dialog

CCMWindPressureVelo_IS875_2015::CCMWindPressureVelo_IS875_2015(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMWindPressureVelo_IS875_2015::IDD, pParent)
{
	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindPressureVelocityAddMod*)pParent;
	//{{AFX_DATA_INIT(CCMWindPressureVelo_IS875_2015)
	//}}AFX_DATA_INIT
	m_aCtrlAutoUser.Add(IDC_CMD_AUTO_RD); // Automatic radio
	m_aCtrlAutoUser.Add(IDC_CMD_USER_RD); // User Defined radio
	//m_Data.Initialize();
	m_bInit = FALSE;
}




void CCMWindPressureVelo_IS875_2015::SetData2Dlg(T_WVEP_IS875_2015& data)
{
	m_Data = data;
	if(!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}
	Data2Dlg();
}

BOOL CCMWindPressureVelo_IS875_2015::SetDlg2Data(T_WVEP_IS875_2015& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}


void CCMWindPressureVelo_IS875_2015::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);

	// Controls binding
	DDX_Control(pDX, IDC_CMD_SPEED_EDIT, m_editSpeed);
	DDX_Control(pDX, IDC_CMD_VRANGE_UNIT, m_unitVRange);
	DDX_Control(pDX, IDC_CMD_CATEGORY_COMBO, m_cobxCategory);
	DDX_Control(pDX, IDC_CMD_BCLASS_COMBO, m_cobxBuildingClass);
	DDX_Control(pDX, IDC_CMD_SCLASS_COMBO, m_cobxStructClass);
	DDX_Control(pDX, IDC_CMD_RISKCOEF_EDIT, m_editRiskCoef);
	DDX_Control(pDX, IDC_CMD_TOPO_CHK, m_chkTopographic);
	DDX_Control(pDX, IDC_CMD_SPEED_UNIT, m_unitSpeed);
	DDX_Control(pDX, IDC_CMD_TOPOFACTOR_EDIT, m_editTopoFactor);
	DDX_Control(pDX, IDC_CMD_VRANGE_EDIT, m_editVRange);
	DDX_Control(pDX, IDC_CMD_CYCLO_CHK1, m_chkCycloRegEffects);
	DDX_Control(pDX, IDC_CMD_CYCLOCLASS_COMBO2, m_cobxImportanceClass);
	DDX_Control(pDX, IDC_CMD_IMPFACT_K4_EDIT, m_editk4Factor);
}

BEGIN_MESSAGE_MAP(CCMWindPressureVelo_IS875_2015, CChildDialog)
	ON_BN_CLICKED(IDC_CMD_AUTO_RD, OnCmdAutoUserRadio)
	ON_BN_CLICKED(IDC_CMD_USER_RD, OnCmdAutoUserRadio)
	ON_BN_CLICKED(IDC_CMD_TOPO_CHK, OnCmdTopoEffectCheck)
	ON_CBN_SELCHANGE(IDC_CMD_SCLASS_COMBO, OnSelchangeCmdSclassCombo)
	ON_EN_CHANGE(IDC_CMD_SPEED_EDIT, OnChangeCmdSpeedEdit)
	ON_BN_CLICKED(IDC_CMD_CYCLO_CHK1, OnCmdCyclonRadio)
	ON_CBN_SELCHANGE(IDC_CMD_CYCLOCLASS_COMBO2, OnSelchangeImportanceCombo)
END_MESSAGE_MAP()

BOOL CCMWindPressureVelo_IS875_2015::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	// Set units for controls
	m_editSpeed.SetUnitType(D_UNITSYS_NONE);
	m_editTopoFactor.SetUnitType(D_UNITSYS_NONE);
	m_editRiskCoef.SetUnitType(D_UNITSYS_NONE);
	m_unitVRange.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dRoofHeight);
	m_editVRange.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dRoofHeight);
	m_editk4Factor.SetUnitType(D_UNITSYS_NONE);
	if(!m_bInit)
	{
		InitDefaultData();
	}
	InitCategoryCombo();
	InitBuildingClassCombo();
	InitStructureClassCombo();
	InitCycloRegCombo();
	Data2Dlg();

	return TRUE;
}

BOOL CCMWindPressureVelo_IS875_2015::DestroyWindow()
{
	Dlg2Data();  // Save current dialog data before destruction
	return CChildDialog::DestroyWindow();
}

void CCMWindPressureVelo_IS875_2015::OnCmdAutoUserRadio()
{
	int nMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlAutoUser, nMethod);
	m_cobxStructClass.EnableWindow(nMethod == 0);
	m_editRiskCoef.SetReadOnly(nMethod == 0);

	if (nMethod == 0) CalculateRiskCoef();
}

void CCMWindPressureVelo_IS875_2015::OnCmdTopoEffectCheck()
{
	int nCheck = m_chkTopographic.GetCheck();
	m_editTopoFactor.EnableWindow(nCheck == 1);
	m_editVRange.EnableWindow(nCheck == 1);
}

void CCMWindPressureVelo_IS875_2015::OnSelchangeCmdSclassCombo()
{
	CalculateRiskCoef();
}


void CCMWindPressureVelo_IS875_2015::OnSelchangeImportanceCombo()
{


	int nIndex = m_cobxImportanceClass.GetCurSel();
	if (nIndex == CB_ERR) return;
	int nStructClass = m_cobxImportanceClass.GetItemData(nIndex);

	double dRiskCoef;
	dRiskCoef = GetK4Factor(nStructClass);
	m_editk4Factor.SetEditUnit(dRiskCoef);
}


void CCMWindPressureVelo_IS875_2015::OnChangeCmdSpeedEdit()
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CChildDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO: Add your control notification handler code here
	CalculateRiskCoef();
}

void CCMWindPressureVelo_IS875_2015::InitDefaultData()
{
	CArray<double, double> caHeight;
	CArray<T_NODE_D, T_NODE_D&> caNode;
	if(!CWindLoadGeneratorCtrl::GetModelHeightAry(caHeight, caNode))
	{
		m_Data.dVertK3 = 0.0;
	}
	else
	{
		m_Data.dVertK3 = caHeight[caHeight.GetSize()-1];  // Set vertical topographic factor range default
	}
}

void CCMWindPressureVelo_IS875_2015::InitCategoryCombo()
{
	//m_cobxCategory.ResetContent();

	// Terrain categories I, II, III, IV
	CString aItem[] = { _T("I"), _T("II"), _T("III"), _T("IV") };
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i = 0; i < nNum; i++)
		m_cobxCategory.AddString(aItem[i]);
	m_cobxCategory.SetCurSel(0);
}

void CCMWindPressureVelo_IS875_2015::InitBuildingClassCombo()
{
	//m_cobxBuildingClass.ResetContent();

	CString aItem[] = { _T("A") };
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i = 0; i < nNum; i++)
		m_cobxBuildingClass.AddString(aItem[i]);
	m_cobxBuildingClass.SetCurSel(0);
}

void CCMWindPressureVelo_IS875_2015::InitStructureClassCombo()
{
	//m_cobxStructClass.ResetContent();

	CString aItem[] = { _LS(IDS_CMD_WIND_IS1987_StrClassA), _LS(IDS_CMD_WIND_IS1987_StrClassB),
		_LS(IDS_CMD_WIND_IS1987_StrClassC), _LS(IDS_CMD_WIND_IS1987_StrClassD) };

	int nIndex;
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i = 0; i < nNum; i++)
	{
		nIndex = m_cobxStructClass.AddString(aItem[i]);
		m_cobxStructClass.SetItemData(nIndex, i);
	}
	m_cobxStructClass.SetCurSel(0);
}

void CCMWindPressureVelo_IS875_2015::InitCycloRegCombo()
{
	//m_cobxImportanceClass.ResetContent();

	CString aItem[] = { _LS(IDS_CMD_WIND_IS875_2015_StrPostCyclo), _LS(IDS_CMD_WIND_IS875_2015_StrIndustrial),
		_LS(IDS_CMD_WIND_IS875_2015_StrOther) };

	int nIndex;
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i = 0; i < nNum; i++)
	{
		nIndex = m_cobxImportanceClass.AddString(aItem[i]);
		m_cobxImportanceClass.SetItemData(nIndex, i);
	}
	m_cobxImportanceClass.SetCurSel(nNum - 1);
}

void CCMWindPressureVelo_IS875_2015::CalculateRiskCoef()
{
	int nMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlAutoUser, nMethod);
	if (nMethod != 0) return;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) { ASSERT(0); return; }

	int nIndex = m_cobxStructClass.GetCurSel();
	if (nIndex == CB_ERR) return;
	int nStructClass = m_cobxStructClass.GetItemData(nIndex);

	CString csVal;
	m_editSpeed.GetWindowText(csVal);

	double dRiskCoef;
	dRiskCoef = pDoc->m_pEditData->GetIS1987RiskCoef(nStructClass, _tstof(csVal));
	m_editRiskCoef.SetEditUnit(dRiskCoef);
}

void CCMWindPressureVelo_IS875_2015::Data2Dlg()
{
	CString csVal;
	//CDlgUtil::CobxSetCurSelItemData(m_cobxCategory, m_Data.dBasicWindSpeed);
	m_editSpeed.SetEditUnit(m_Data.dBasicWindSpeed);
	OnChangeCmdSpeedEdit();
	m_cobxCategory.SetCurSel(m_Data.nTerrainCategory);
	m_cobxBuildingClass.SetCurSel(m_Data.nBuildingClass);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlAutoUser, m_Data.nRiskCalcMode);
	OnCmdAutoUserRadio();
	m_cobxStructClass.SetCurSel(m_Data.nClassOfStruct);
	OnSelchangeCmdSclassCombo();
	m_editRiskCoef.SetEditUnit(m_Data.dRiskCoefK1);

	m_chkTopographic.SetCheck(m_Data.bIncludeTopo);
	OnCmdTopoEffectCheck();
	m_editTopoFactor.SetEditUnit(m_Data.dTopoK3);
	m_editVRange.SetEditUnit(m_Data.dVertK3);

	m_chkCycloRegEffects.SetCheck(m_Data.bIncludeCyclone);
	OnCmdCyclonRadio();
	OnSelchangeImportanceCombo();
	m_editk4Factor.SetEditUnit(m_Data.dImpFactorK4);
	if (m_Data.dImpFactorK4 > 1.0)
	{//In case importing from MCT/MGB
		if (m_Data.dImpFactorK4 == 1.3) m_cobxImportanceClass.SetCurSel(0);
		else                     m_cobxImportanceClass.SetCurSel(1);
	}
}

BOOL CCMWindPressureVelo_IS875_2015::Dlg2Data()
{
	CString csVal;
	//m_Data.Initialize();

	m_Data.dBasicWindSpeed = m_editSpeed.GetEditValue();
	m_Data.nTerrainCategory = m_cobxCategory.GetCurSel();
	m_Data.nBuildingClass = m_cobxBuildingClass.GetCurSel();

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlAutoUser, m_Data.nRiskCalcMode);
	if (m_Data.nRiskCalcMode == 0)
		m_Data.nClassOfStruct = m_cobxStructClass.GetCurSel();
	m_Data.dRiskCoefK1 = m_editRiskCoef.GetEditValue();

	m_Data.bIncludeTopo = m_chkTopographic.GetCheck();
	if (m_Data.bIncludeTopo)
	{
		m_Data.dTopoK3 = m_editTopoFactor.GetEditValue();
		m_Data.dVertK3 = m_editVRange.GetEditValue();
	}

	m_Data.bIncludeCyclone = m_chkCycloRegEffects.GetCheck();
	if (m_Data.bIncludeCyclone)
	{
		//Data.nImpStructure = m_cobxImportanceClass.GetCurSel();
		m_Data.dImpFactorK4 = m_editk4Factor.GetEditValue();
	}
	return TRUE;
}


void CCMWindPressureVelo_IS875_2015::OnCmdCyclonRadio()
{
	int nCheck = m_chkCycloRegEffects.GetCheck();
	m_cobxImportanceClass.EnableWindow(nCheck == 1);
	m_editk4Factor.EnableWindow(FALSE);
}

double CCMWindPressureVelo_IS875_2015::GetK4Factor(int impfactor)
{
	double dk4fact = 0.0;
	if (impfactor == 0) dk4fact = 1.3;
	else if (impfactor == 1)dk4fact = 1.15;
	else dk4fact = 1.0;

	return dk4fact;

}



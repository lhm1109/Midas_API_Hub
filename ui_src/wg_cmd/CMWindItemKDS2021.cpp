// CMWindItemKDS2021.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemDlg.h"
#include "CMWindItemKDS2021.h"
#include "WindClassifyKBC2015Dlg.h"
#include "WindVibrationKDS2021Dlg.h"
#include "WindGustKDS2021Dlg.h"
#include "WindAutoCalcDlg.h"
#include "WindAmpFactorDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\LateralLoad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_CMD_WIND_FORCE_COEF_CALC_TYPE_USER  0
#define D_CMD_WIND_FORCE_COEF_CALC_TYPE_AUTO  1

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemKDS2021 dialog


CCMWindItemKDS2021::CCMWindItemKDS2021(CWnd* pParent /*=NULL*/)
: CChildDialog(CCMWindItemKDS2021::IDD, pParent)
{
	m_pParent = NULL;
	m_bModify = FALSE;
	m_KeyWAorWT = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;
	//{{AFX_DATA_INIT(CCMWindItemKDS2021)
	//}}AFX_DATA_INIT

	m_aCtrlMethod.Add(IDC_CMD_METHOD_RD1);
	m_aCtrlMethod.Add(IDC_CMD_METHOD_RD2);

	m_aCtrlVortex.Add(IDC_CMD_VORTEX_ROOF_HEIGHT_STC);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_ROOF_HEIGHT_EDT);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_ROOF_HEIGHT_UNT);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_DIA_DM_STC);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_DIA_DM_EDT);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_DIA_DM_UNT);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_DIA_DB_STC);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_DIA_DB_EDT);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_DIA_DB_UNT);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_NATURAL_STC);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_NATURAL_EDT);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_NATURAL_UNT);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_MASS_STC);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_MASS_EDT);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_MASS_UNT);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_DAMPING_STC);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_DAMPING_EDT);

	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_SPEED_TITLE);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_SPEED_EDIT);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_SPEED_UNIT);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_ROOF_HEIGHT_TITLE);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_ROOF_HEIGHT_EDIT);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_ROOF_HEIGHT_UNIT);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_CE_TITLE);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_CE_COMBO);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_MIN_X_STC);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_MIN_X_EDT);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_MIN_X_UNIT);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_MIN_Y_STC);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_MIN_Y_EDT);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_MIN_Y_UNIT);

	m_aCtrlGeneral.Add(IDC_CMD_KOR_EC_TITLE);
	m_aCtrlGeneral.Add(IDC_CMD_CATEGORY_COMBO);
	m_aCtrlGeneral.Add(IDC_CMD_KOR_BWS_TITLE);
	m_aCtrlGeneral.Add(IDC_CMD_SPEED_EDIT);
	m_aCtrlGeneral.Add(IDC_CMD_SPEED_UNIT);
	m_aCtrlGeneral.Add(IDC_CMD_KOR_PC_TITLE);
	m_aCtrlGeneral.Add(IDC_CMD_IMPORTANCE_COMBO);
	m_aCtrlGeneral.Add(IDC_WG_CMD_STATIC0);
	m_aCtrlGeneral.Add(IDC_CMD_ROOF_HEIGHT_EDIT);
	m_aCtrlGeneral.Add(IDC_CMD_ROOF_HEIGHT_UNIT);

	m_aCtrlGeneral.Add(IDC_CMD_TOPO_EFFECT_CHECK);
	m_aCtrlGeneral.Add(IDC_WG_CMD_STATIC1);
	m_aCtrlGeneral.Add(IDC_WG_CMD_STATIC2);
	m_aCtrlGeneral.Add(IDC_WG_CMD_STATIC3);
	m_aCtrlGeneral.Add(IDC_CMD_KZT_EDIT);

	m_aCtrlGeneral.Add(IDC_WG_CMD_STATIC40);
	m_aCtrlGeneral.Add(IDC_WG_CMD_STATIC50);
	m_aCtrlGeneral.Add(IDC_WG_CMD_STATIC8);
	m_aCtrlGeneral.Add(IDC_CMD_DIRECT_FACTOR_X_EDIT);
	m_aCtrlGeneral.Add(IDC_CMD_DIRECT_FACTOR_Y_EDIT);

	m_aCtrlGeneral.Add(IDC_CMD_RIGID_RADIO1);
	m_aCtrlGeneral.Add(IDC_CMD_FLEX_RADIO2);
	m_aCtrlGeneral.Add(IDC_WG_CMD_STATIC4);
	m_aCtrlGeneral.Add(IDC_WG_CMD_STATIC5);
	m_aCtrlGeneral.Add(IDC_CMD_GUST_FACTOR_EDIT);
	m_aCtrlGeneral.Add(IDC_WG_CMD_STATIC6);
	m_aCtrlGeneral.Add(IDC_CMD_GUST_FACTOR_EDIT_Y);
	m_aCtrlGeneral.Add(IDC_CMD_GUST_FACTOR_BTN);

	m_aCtrlGeneral.Add(IDC_CMD_LOAD_EVAL_CHECK);
	m_aCtrlGeneral.Add(IDC_WG_CMD_STATIC7);
	m_aCtrlGeneral.Add(IDC_CMD_FORCE_COEF_USER_RDO);
	m_aCtrlGeneral.Add(IDC_CMD_FORCE_COEFF_STC);
	m_aCtrlGeneral.Add(IDC_CMD_FORCE_COEFF_EDIT);
	m_aCtrlGeneral.Add(IDC_CMD_FORCE_COEF_AUTO_RDO);
	m_aCtrlGeneral.Add(IDC_CMD_FORCE_COEF_AUTO_BTN);
	m_aCtrlGeneral.Add(IDC_WIND_AUTOCALC_CMB_STRUCTURE_TYPE);

	m_aCtrlGeneral.Add(IDC_CMD_RISE_TYPE_RDO1);
	m_aCtrlGeneral.Add(IDC_CMD_RISE_TYPE_RDO2);
	m_aCtrlGeneral.Add(IDC_CMD_ACROSS_WIND_CHK);
	m_aCtrlGeneral.Add(IDC_CMD_TORSIONAL_WIND_CHK);
	m_aCtrlGeneral.Add(IDC_CMD_WIND_RESPONSE_CHK);
	m_aCtrlGeneral.Add(IDC_CMD_WIND_VIBRATION_PARAM_BTN);
// 	m_aCtrlGeneral.Add(IDC_CMD_WIND_LSHAPE_CHK);
// 	m_aCtrlGeneral.Add(IDC_CMD_LSAHPE_BTN);

	m_aCtrlType.Add(IDC_CMD_RIGID_RADIO1);
	m_aCtrlType.Add(IDC_CMD_FLEX_RADIO2);
	
	m_aRdoForceCoefCalcType.RemoveAll();
	m_aRdoForceCoefCalcType.Add(IDC_CMD_FORCE_COEF_USER_RDO);
	m_aRdoForceCoefCalcType.Add(IDC_CMD_FORCE_COEF_AUTO_RDO);
	m_aRdoForceCoefCalcType.FreeExtra();
	
	m_aCtrlForceCoef_user.RemoveAll();
	m_aCtrlForceCoef_user.Add(IDC_CMD_FORCE_COEFF_STC );
	m_aCtrlForceCoef_user.Add(IDC_CMD_FORCE_COEFF_EDIT);
	m_aCtrlForceCoef_user.FreeExtra();
	
	m_aCtrlForceCoef_auto.RemoveAll();
	m_aCtrlForceCoef_auto.Add(IDC_CMD_FORCE_COEF_AUTO_BTN);
	m_aCtrlForceCoef_auto.Add(IDC_WIND_AUTOCALC_CMB_STRUCTURE_TYPE);
	m_aCtrlForceCoef_auto.FreeExtra();
	
	m_aCtrlForceCoef.RemoveAll();
	m_aCtrlForceCoef.Add(IDC_CMD_FORCE_COEF_USER_RDO);
	m_aCtrlForceCoef.Append(m_aCtrlForceCoef_user);
	m_aCtrlForceCoef.Add(IDC_CMD_FORCE_COEF_AUTO_RDO);
	m_aCtrlForceCoef.Append(m_aCtrlForceCoef_auto);
	m_aCtrlForceCoef.FreeExtra();

	m_aCtrlRiseType.RemoveAll();
	m_aCtrlRiseType.Add(IDC_CMD_RISE_TYPE_RDO1);
	m_aCtrlRiseType.Add(IDC_CMD_RISE_TYPE_RDO2);
	m_aCtrlRiseType.FreeExtra();

	m_aCtrlRise.RemoveAll();
	m_aCtrlRise.Add(IDC_CMD_ACROSS_WIND_CHK);
	m_aCtrlRise.Add(IDC_CMD_TORSIONAL_WIND_CHK);
// 	m_aCtrlRise.Add(IDC_CMD_WIND_LSHAPE_CHK);
	m_aCtrlRise.FreeExtra();
	
	m_bInit = FALSE;
	m_Data.Initialize();
	m_WnatD.Initialize();
}

void CCMWindItemKDS2021::SetData2Dlg(T_WIND_KDS2021& data, BOOL bModify, T_KEY KeyWAorWT)
{
	m_Data = data;
	m_bModify = bModify;
	m_KeyWAorWT = KeyWAorWT;
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pAttrCtrl2->GetWnat(m_Data.keyWnat, m_WnatD)) m_WnatD.Initialize();
}

BOOL CCMWindItemKDS2021::SetDlg2Data(T_WIND_KDS2021& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CCMWindItemKDS2021::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemKDS2021)
	DDX_Control(pDX, IDC_CMD_SIMPLE_SPEED_UNIT, m_unitSpeed2);
	DDX_Control(pDX, IDC_CMD_SIMPLE_SPEED_EDIT, m_editSpeed2);
	DDX_Control(pDX, IDC_CMD_SIMPLE_ROOF_HEIGHT_UNIT, m_unitRoofHeight2);
	DDX_Control(pDX, IDC_CMD_SIMPLE_ROOF_HEIGHT_EDIT, m_editRoofHeight2);
	DDX_Control(pDX, IDC_CMD_SIMPLE_MIN_X_UNIT, m_unitMinX);
	DDX_Control(pDX, IDC_CMD_SIMPLE_MIN_X_EDT, m_editMinX);
	DDX_Control(pDX, IDC_CMD_SIMPLE_MIN_Y_UNIT, m_unitMinY);
	DDX_Control(pDX, IDC_CMD_SIMPLE_MIN_Y_EDT, m_editMinY);
	DDX_Control(pDX, IDC_CMD_SIMPLE_CE_COMBO, m_cobxCeFactor);
	DDX_Control(pDX, IDC_CMD_TOPO_EFFECT_CHECK, m_chkTopoEffect);
	DDX_Control(pDX, IDC_CMD_LOAD_EVAL_CHECK, m_chkLoadEval);
	DDX_Control(pDX, IDC_CMD_SPEED_UNIT, m_unitSpeed);
	DDX_Control(pDX, IDC_CMD_SPEED_EDIT, m_editSpeed);
	DDX_Control(pDX, IDC_CMD_ROOF_HEIGHT_UNIT, m_unitRoofHeight);
	DDX_Control(pDX, IDC_CMD_ROOF_HEIGHT_EDIT, m_editRoofHeight);
	DDX_Control(pDX, IDC_CMD_KZT_EDIT, m_editKZT);
	DDX_Control(pDX, IDC_CMD_GUST_FACTOR_EDIT, m_editGustFactorX);
	DDX_Control(pDX, IDC_CMD_GUST_FACTOR_EDIT_Y, m_editGustFactorY);
	DDX_Control(pDX, IDC_CMD_FORCE_COEFF_EDIT, m_editForceCoef);
	DDX_Control(pDX, IDC_CMD_CATEGORY_COMBO, m_cobxCategory);
	DDX_Control(pDX, IDC_CMD_IMPORTANCE_COMBO, m_cobxImportFactor);
	DDX_Control(pDX, IDC_CMD_FRAME, m_grpWindParam);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_CMB_STRUCTURE_TYPE, m_cmbStructureType);
	DDX_Control(pDX, IDC_CMD_ACROSS_WIND_CHK, m_chkAcrossWind);
	DDX_Control(pDX, IDC_CMD_TORSIONAL_WIND_CHK, m_chkTorsionalWind);
	DDX_Control(pDX, IDC_CMD_WIND_RESPONSE_CHK, m_chkWindResponse);
	DDX_Control(pDX, IDC_CMD_DIRECT_FACTOR_X_EDIT, m_editDirectFactorX);
	DDX_Control(pDX, IDC_CMD_DIRECT_FACTOR_Y_EDIT, m_editDirectFactorY);

	DDX_Control(pDX, IDC_CMD_WIND_LSHAPE_CHK, m_chkLShape);
	DDX_Control(pDX, IDC_CMD_VORTEX_CHECK, m_chkVortex);
	DDX_Control(pDX, IDC_CMD_VORTEX_ROOF_HEIGHT_UNT, m_unitRoofHeight3);
	DDX_Control(pDX, IDC_CMD_VORTEX_DIA_DM_UNT, m_unitDiaDM);
	DDX_Control(pDX, IDC_CMD_VORTEX_DIA_DB_UNT, m_unitDiaDB);
// 	DDX_Control(pDX, IDC_CMD_VORTEX_NATURAL_UNT, m_unitNatural);
	DDX_Control(pDX, IDC_CMD_VORTEX_MASS_UNT, m_unitMass);
	DDX_Control(pDX, IDC_CMD_VORTEX_ROOF_HEIGHT_EDT, m_editRoofHeight3);
	DDX_Control(pDX, IDC_CMD_VORTEX_DIA_DM_EDT, m_editDiaDM);
	DDX_Control(pDX, IDC_CMD_VORTEX_DIA_DB_EDT, m_editDiaDB);
	DDX_Control(pDX, IDC_CMD_VORTEX_NATURAL_EDT, m_editNatural);
	DDX_Control(pDX, IDC_CMD_VORTEX_MASS_EDT, m_editMass);
	DDX_Control(pDX, IDC_CMD_VORTEX_DAMPING_EDT, m_editDamping);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemKDS2021, CChildDialog)
//{{AFX_MSG_MAP(CCMWindItemKDS2021)
	ON_BN_CLICKED(IDC_CMD_METHOD_RD1, OnCmdMethodRadio)
	ON_BN_CLICKED(IDC_CMD_METHOD_RD2, OnCmdMethodRadio)
	ON_BN_CLICKED(IDC_CMD_VORTEX_CHECK, OnCmdVortexSheddingBtn)
	ON_BN_CLICKED(IDC_CMD_METHOD_GENERAL_BTN, OnCmdGeneralBtn)
	ON_BN_CLICKED(IDC_CMD_GUST_FACTOR_BTN, OnCmdGustFactorBtn)
	ON_BN_CLICKED(IDC_CMD_WIND_VIBRATION_PARAM_BTN, OnCmdVibrationBtn)
	ON_BN_CLICKED(IDC_CMD_LSAHPE_BTN, OnCmdLShapeBtn)
	ON_BN_CLICKED(IDC_CMD_LOAD_EVAL_CHECK, OnCmdLoadEvalCheck)
	ON_BN_CLICKED(IDC_CMD_TOPO_EFFECT_CHECK, OnCmdTopoEffectCheck)
	ON_BN_CLICKED(IDC_CMD_FORCE_COEF_USER_RDO, OnCmdChangeRdo_ForceCoefCalcType)
	ON_BN_CLICKED(IDC_CMD_FORCE_COEF_AUTO_RDO, OnCmdChangeRdo_ForceCoefCalcType)
	ON_BN_CLICKED(IDC_CMD_FORCE_COEF_AUTO_BTN, OnBtnAutoCalc)
	ON_BN_CLICKED(IDC_CMD_RISE_TYPE_RDO1, OnCmdRiseTypeRadio)
	ON_BN_CLICKED(IDC_CMD_RISE_TYPE_RDO2, OnCmdRiseTypeRadio)
	ON_BN_CLICKED(IDC_CMD_ACROSS_WIND_CHK		, OnCmdWindTypeCheck)
	ON_BN_CLICKED(IDC_CMD_TORSIONAL_WIND_CHK, OnCmdWindTypeCheck)
	ON_BN_CLICKED(IDC_CMD_WIND_RESPONSE_CHK	, OnCmdWindTypeCheck)
	ON_BN_CLICKED(IDC_CMD_WIND_LSHAPE_CHK, OnCmdLShapeCheck)
	ON_EN_CHANGE(IDC_CMD_SPEED_EDIT							, OnChangeSpeedEdit)
	ON_EN_CHANGE(IDC_CMD_SIMPLE_SPEED_EDIT			, OnChangeSimpleSpeedEdit)	
	ON_EN_CHANGE(IDC_CMD_ROOF_HEIGHT_EDIT				, OnChangeRoofHeightEdit)
	ON_EN_CHANGE(IDC_CMD_SIMPLE_ROOF_HEIGHT_EDIT, OnChangeSimpleRoofHeightEdit)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemKDS2021 message handlers

BOOL CCMWindItemKDS2021::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	AlignControls();
	
	m_grpWindParam.InitControl(this, CCMWindItemKDS2021::IDD, IDC_CMD_FRAME, TRUE, TRUE); // AlignControls 다음에
	m_grpWindParam.SetFoldState(FALSE); // 무조건 펼친다

	m_editSpeed2.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dBasicWindSpeed);
	m_editSpeed.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dBasicWindSpeed);

	m_unitRoofHeight2.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);
	m_editRoofHeight2.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);
	m_unitRoofHeight.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);
	m_editRoofHeight.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);

	m_unitRoofHeight3.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);
	m_editRoofHeight3.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);
	m_unitDiaDM.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);
	m_editDiaDM.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);
	m_unitDiaDB.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);
	m_editDiaDB.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);
// 	m_unitNatural.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);
// 	m_editNatural.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);
	m_unitMass.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2015_dMx1st);
	m_editMass.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2015_dMx1st);

	m_unitMinX.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);
	m_editMinX.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);
	m_unitMinY.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);
	m_editMinY.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);
	
	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}
	
	InitCategoryCombo();
	InitImportFactorCombo();
	InitStructureTypeCombo();
	InitCeFactorCombo();
	
	Data2Dlg();

	if(m_KeyWAorWT!=NULL)
	{
		CDlgUtil::CtrlEnableDisable(this, IDC_CMD_METHOD_RD1, FALSE);
		CDlgUtil::CtrlEnableDisable(this, IDC_CMD_METHOD_RD2, FALSE);
		CDlgUtil::CtrlEnableDisable(this, IDC_CMD_METHOD_GENERAL_BTN, FALSE);
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_FRAME, FALSE, TRUE);
	}

	// GEN-6193 [고층] L-Shaped Building 옵션 삭제 (최종 고시된 기준에 보니 해당 내용이 포함되어 있지 않음) 
	CArray<UINT, UINT> aCtrlLShape;
	aCtrlLShape.Add(IDC_CMD_WIND_LSHAPE_CHK);
	aCtrlLShape.Add(IDC_CMD_LSAHPE_BTN);
	aCtrlLShape.FreeExtra();
	CDlgUtil::CtrlShowHide(this, aCtrlLShape, FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CCMWindItemKDS2021::DestroyWindow() 
{
	Dlg2Data();

	if (m_pParent != NULL)
	{
		if (!m_pParent->IsSaveFoldedInfo())
		{
			m_grpWindParam.SetFoldState(FALSE, FALSE);
		}
	}
	
	return CChildDialog::DestroyWindow();
}

void CCMWindItemKDS2021::OnCmdMethodRadio()
{
	int nMethod = 1;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, nMethod);

	int nCheck = m_chkVortex.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_VORTEX_CHECK, nMethod == 1);
	
	
// 	if(nCheck) CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlMethod, nMethod);
	

	BOOL bFolded = m_grpWindParam.GetFoldState();
	if(!bFolded)
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlSimple, nMethod == 0);
		CDlgUtil::CtrlShowHide(this, m_aCtrlGeneral, nMethod == 1 && nCheck == 0);
		CDlgUtil::CtrlShowHide(this, m_aCtrlVortex , nMethod == 1 && nCheck == 1);
	}

	CArray<UINT, UINT> aCtrlLShape;
	aCtrlLShape.Add(IDC_CMD_WIND_LSHAPE_CHK);
	aCtrlLShape.Add(IDC_CMD_LSAHPE_BTN);
	aCtrlLShape.FreeExtra();
	CDlgUtil::CtrlShowHide(this, aCtrlLShape, FALSE);

	UpdateGridData(nMethod, nCheck);

}

void CCMWindItemKDS2021::UpdateGridData(int nMethod, int nCheck)
{
	int nAcrossCheck = nMethod == 1 && nCheck == 0 ? m_chkAcrossWind.GetCheck() : 0;
	int nTorsionalCheck = nMethod == 1 && nCheck == 0 ? m_chkTorsionalWind.GetCheck() : 0;

	BOOL bUseAlong = TRUE;
	if (nCheck == 1) bUseAlong = FALSE;
	m_pParent->UpdateGrid(bUseAlong, nAcrossCheck, nTorsionalCheck);
}

void CCMWindItemKDS2021::OnCmdVortexSheddingBtn()
{
	OnCmdMethodRadio();
}

void CCMWindItemKDS2021::OnCmdGeneralBtn() 
{
	CWindClassifyKBC2015Dlg dlg(TRUE);
	dlg.SetInitPos(D_INIT_POS_RT);

	int nCheck = m_chkVortex.GetCheck();

	int nMethod=1;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, nMethod);
	
	if (nMethod == 0) dlg.m_dHeight = m_editRoofHeight2.GetEditValue();
	else
	{
		if(nCheck==0) dlg.m_dHeight = m_editRoofHeight.GetEditValue();
		else dlg.m_dHeight = m_editRoofHeight3.GetEditValue();
	}
	dlg.m_dWidth			= m_dClassifyBY;
	dlg.m_dBreadth			= m_dClassifyBX;
	dlg.m_dArea				= m_dAreadKDS2021;
	dlg.SetWindLoadCode(KDS_W_2022);
	dlg.DoModal();
}

void CCMWindItemKDS2021::OnCmdGustFactorBtn() 
{
	// TODO: Add your control notification handler code here	
	CWindGustKDS2021Dlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	
	CString str = _T("");
	m_cobxImportFactor.GetWindowText(str);
	
	dlg.m_dHeight	  = m_editRoofHeight.GetEditValue();
	dlg.m_dImportance = _tstof(str);
	dlg.m_nExposure	  = m_cobxCategory.GetCurSel();
	dlg.m_dSpeed	  = m_editSpeed.GetEditValue();
	dlg.m_dKzt		  = m_editKZT.GetEditValue();
	dlg.m_dHzt        = 0.0;
	dlg.m_bTopography = m_chkTopoEffect.GetCheck();
	dlg.m_dWidth	  = m_dBLyKDS2021;
	dlg.m_dBreadth	  = m_dBLxKDS2021;
	dlg.m_dFrequencyX = m_dFreqXKDS2021;
	dlg.m_dFrequencyY = m_dFreqYKDS2021;
	dlg.m_dDamping	  = m_dDampingKDS2021;  
	dlg.m_dTotalMass  = m_dGustM;
	dlg.m_dKdx        = m_editDirectFactorX.GetEditValue();
	dlg.m_dKdy        = m_editDirectFactorY.GetEditValue();
	dlg.m_dMx = m_dGustMx;
	dlg.m_dMy = m_dGustMy;
	dlg.m_dVibration = m_dGustVibration;

	dlg.m_dStoryHMax = m_dStoryHMax;	//
	//ASSERT(dlg.m_dHeight== dlg.dStoryHMax);

	if(m_StoryHeight.GetSize() > 0)
	{
		dlg.m_StoryHeight.Copy(m_StoryHeight);
		dlg.m_StoryMass.Copy(m_StoryMass);
	}

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlType, dlg.m_nRigidity); 
	
	if (dlg.DoModal() == IDOK)
	{
		m_editGustFactorX.SetEditUnit(dlg.m_dGustFactorX, 4);
		m_editGustFactorY.SetEditUnit(dlg.m_dGustFactorY, 4);
		
		m_dBLyKDS2021			= dlg.m_dWidth;
		m_dBLxKDS2021			= dlg.m_dBreadth;
		m_dFreqXKDS2021		= dlg.m_dFrequencyX;
		m_dFreqYKDS2021		= dlg.m_dFrequencyY;
		m_dDampingKDS2021 = dlg.m_dDamping;	

		m_dGustM = dlg.m_dTotalMass;
		m_dGustMx = dlg.m_dMx;
		m_dGustMy = dlg.m_dMy;
		m_dGustVibration = dlg.m_dVibration;
	}	
}

void CCMWindItemKDS2021::OnCmdVibrationBtn() 
{
	CWindVibrationKDS2021Dlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRiseType, dlg.m_nBuildingType);
	dlg.m_nAcrossCheck = m_chkAcrossWind.GetCheck();
	dlg.m_nTorsionalCheck = m_chkTorsionalWind.GetCheck();
	dlg.m_nResponseCheck = m_chkWindResponse.GetCheck();

	dlg.m_dBreadthX		= m_dBY;
	dlg.m_dBreadthY		= m_dBX;
	dlg.m_dFreqX = m_dFreqX;
	dlg.m_dFreqY = m_dFreqY;
	dlg.m_dFreqT = m_dFreqT;
	dlg.m_dMassT = m_dTotalM;
	dlg.m_dMassX = m_dMassX;
	dlg.m_dMassY = m_dMassY;
	dlg.m_dMoment = m_dMoment;
	dlg.m_dZf = m_dZf;
	dlg.m_dVibration = m_dVibration;

	if (dlg.DoModal() == IDOK)
	{
		m_dBY= dlg.m_dBreadthX;
		m_dBX = dlg.m_dBreadthY;

		m_dFreqX = dlg.m_dFreqX;
		m_dFreqY = dlg.m_dFreqY;
		m_dFreqT = dlg.m_dFreqT;
		m_dTotalM = dlg.m_dMassT;
		m_dMassX = dlg.m_dMassX;
		m_dMassY = dlg.m_dMassY;
		m_dMoment = dlg.m_dMoment;
		m_dZf = dlg.m_dZf;
		m_dVibration = dlg.m_dVibration;
	}
}

void CCMWindItemKDS2021::OnCmdLShapeBtn()
{
	for (int i = 0; i < 2; i++)
	{
		m_Data.dAmFactorAlong[i] = m_dAmFactorAlong[i];
		m_Data.dAmFactorAcross[i] = m_dAmFactorAcross[i];
		m_Data.dAmFactorTorsional[i] = m_dAmFactorTorsional[i];
	}
	CWindAmpFactorDlg dlg(&m_Data);
	if (dlg.DoModal() == IDOK)
	{
		for (int i = 0; i < 2; i++)
		{
			m_dAmFactorAlong[i] = m_Data.dAmFactorAlong[i];
			m_dAmFactorAcross[i] = m_Data.dAmFactorAcross[i];
			m_dAmFactorTorsional[i] = m_Data.dAmFactorTorsional[i];
		}
	}
}

void CCMWindItemKDS2021::OnCmdLoadEvalCheck() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkLoadEval.GetCheck();
	BOOL bEnable = (nCheck == 1) ? TRUE : FALSE;
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlForceCoef, bEnable);
	
	if (bEnable)  OnCmdChangeRdo_ForceCoefCalcType();
	
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlRiseType, nCheck==0);
	OnCmdWindTypeCheck();
	
	if(nCheck==0)
	{
		OnCmdRiseTypeRadio();
	}
	else
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlRise, FALSE);
		CDlgUtil::CtrlEnableDisable(this, IDC_CMD_WIND_RESPONSE_CHK, FALSE);

		m_chkAcrossWind.SetCheck(FALSE);
		m_chkTorsionalWind.SetCheck(FALSE);
		m_chkWindResponse.SetCheck(FALSE);
		m_chkLShape.SetCheck(FALSE);
	}
	//OnCmdLShapeCheck();
}

void CCMWindItemKDS2021::OnCmdTopoEffectCheck() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkTopoEffect.GetCheck();
	m_editKZT.EnableWindow(nCheck == 1);
}

void CCMWindItemKDS2021::OnCmdChangeRdo_ForceCoefCalcType()
{
	int nCheck = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoForceCoefCalcType, nCheck);
	
	BOOL bEnableUser = (nCheck == D_CMD_WIND_FORCE_COEF_CALC_TYPE_USER) ? TRUE : FALSE;
	BOOL bEnableAuto = (nCheck == D_CMD_WIND_FORCE_COEF_CALC_TYPE_AUTO) ? TRUE : FALSE;
	
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlForceCoef_user, bEnableUser);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlForceCoef_auto, bEnableAuto);
}

void CCMWindItemKDS2021::OnCmdRiseTypeRadio()
{
	int nRiseType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRiseType, nRiseType);

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlRise, nRiseType==1);
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_WIND_RESPONSE_CHK, TRUE);

	if(nRiseType==0)
	{
		m_chkAcrossWind.SetCheck(TRUE);
		m_chkTorsionalWind.SetCheck(FALSE);
	}
	OnCmdWindTypeCheck();
}

void CCMWindItemKDS2021::OnCmdWindTypeCheck()
{
	int nLoadEvalCheck = m_chkLoadEval.GetCheck();
	int nBuildingType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRiseType, nBuildingType);
	int nAcrossCheck = m_chkAcrossWind.GetCheck();
	int nTorsionalCheck = m_chkTorsionalWind.GetCheck();
	int nResponseCheck = m_chkWindResponse.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_WIND_VIBRATION_PARAM_BTN, nLoadEvalCheck==0 && (nAcrossCheck == 1 || nTorsionalCheck == 1 || nResponseCheck == 1));
	
	int nMethod = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, nMethod);

	int nCheck = m_chkVortex.GetCheck();
	UpdateGridData(nMethod, nCheck);
	OnCmdLShapeCheck();
}

void CCMWindItemKDS2021::OnCmdLShapeCheck()
{
	int nRiseType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRiseType, nRiseType);

	int nLShapeCheck = m_chkLShape.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_LSAHPE_BTN, nLShapeCheck == 1 && nRiseType==1);

}
void CCMWindItemKDS2021::OnBtnAutoCalc()
{
	T_WIND_D WindD;
	m_pParent->GetWindData(WindD);
	int nStructCmbType = m_cmbStructureType.GetItemData(m_cmbStructureType.GetCurSel());
	CWindAutoCalcDlg dlg(NULL,WindD,nStructCmbType);
	dlg.SetData(m_WnatD);
	if(dlg.DoModal() == IDOK)
	{
		m_WnatD.arWnatStor.RemoveAll();
		dlg.GetData(m_WnatD);
	}
}

void CCMWindItemKDS2021::OnChangeSpeedEdit()
{
	double dValue = m_editSpeed.GetEditValue();
	m_editSpeed2.SetEditUnit(dValue);
}

void CCMWindItemKDS2021::OnChangeSimpleSpeedEdit()
{
	double dValue = m_editSpeed2.GetEditValue();
	m_editSpeed.SetEditUnit(dValue);
}

void CCMWindItemKDS2021::OnChangeRoofHeightEdit()
{
	double dValue = m_editRoofHeight.GetEditValue();
	m_editRoofHeight2.SetEditUnit(dValue);
}

void CCMWindItemKDS2021::OnChangeSimpleRoofHeightEdit()
{
	double dValue = m_editRoofHeight2.GetEditValue();
	m_editRoofHeight.SetEditUnit(dValue);
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMWindItemKDS2021::InitDefaultData()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	
	// 층개수와 Base Level 구하기 
	double dBaseLevel = pDoc->m_pAttrCtrl->GetGroundLevel();
	T_STOR_D StorD, StorD2;
	CArray<T_STOR_K,T_STOR_K> aStorK;
	pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);  
	int nStorNum = aStorK.GetSize();
	if (nStorNum > 0)	pDoc->m_pAttrCtrl->GetStor(aStorK[nStorNum-1], StorD);
	else StorD.Initialize();
	
	T_BLDC_D BldcD;
	if (!pDoc->m_pAttrCtrl->GetBldc(BldcD)) BldcD.Initialize();
	// 폭 구하기 
	double dBLx = 0.0;
	double dBLy = 0.0;
	double dMinBx = 0.0;
	double dMinBy = 0.0;

	double dDmLevel = 2 * StorD.dStoryLevel / 3.;
	double dMaxUp4Dm = 0.0;	//Dm => Diameter of 2H/3 Level
	double dMaxDn4Dm = 0.0;

	double dMaxUp4DB = 0.0;	//DB=> Diameter of Bottom
	double dMaxDn4DB = 0.0;

	const double dTol = 1.e-10;
	if (nStorNum > 0)
	{	
		BOOL bFirst=TRUE, bFindDmLevel=TRUE;
		for (int i=0; i<nStorNum; i++)
		{
			pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD2);

			// MQC 15282 : Min.Breadth 값을 가져올 때 Story Data에서 Ground Level 아래쪽 Level의 값을 제외하고 
			// 최소값을 가지고 올 수 있도록 수정부탁드립니다. 
			if (StorD2.dStoryLevel < dBaseLevel - (1.e-10))
			{
				dMaxDn4DB = max(dMaxDn4DB, max(StorD2.WindWidthX, StorD2.WindWidthY));
				continue;
			}
			
			if (bFindDmLevel)
			{
				if (fabs(StorD2.dStoryLevel - dDmLevel) <= dTol)
				{
					dMaxUp4Dm = max(StorD2.WindWidthX, StorD2.WindWidthY);
					bFindDmLevel = FALSE;
				}
				else if (StorD2.dStoryLevel > dDmLevel + dTol)
				{
					dMaxUp4Dm = max(StorD2.WindWidthX, StorD2.WindWidthY);
					
					if (i > 0)
					{
						T_STOR_D LowerStor;
						pDoc->m_pAttrCtrl->GetStor(aStorK[i - 1], LowerStor);
						dMaxDn4Dm = max(LowerStor.WindWidthX, LowerStor.WindWidthY);
					}
					bFindDmLevel = FALSE;
				}
			}
			
			if(bFirst)
			{
				dMinBx = StorD2.WindWidthX;
				dMinBy = StorD2.WindWidthY;
				bFirst=FALSE;

				dMaxUp4DB = max(StorD2.WindWidthX, StorD2.WindWidthY);
				continue;
			}
			if(dMinBx>StorD2.WindWidthX) dMinBx = StorD2.WindWidthX;
			if(dMinBy>StorD2.WindWidthY) dMinBy = StorD2.WindWidthY;
		}

		if (bFindDmLevel)
		{
			ASSERT(0);
			dMaxDn4DB = max(dMaxDn4DB, max(StorD2.WindWidthX, StorD2.WindWidthY));
		}

		for (int i=0; i<nStorNum; i++)
		{
			pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD2);
			if (StorD2.dStoryLevel > dBaseLevel) break;
		}
		if(StorD2.dStoryLevel > dBaseLevel) 
		{
			dBLx = StorD2.WindWidthX;
			dBLy = StorD2.WindWidthY;
		}
		else
		{
			dBLx = 0.0; dBLy = 0.0;
		}
	}
	

	// 초기화
	m_nClassifyStructure = 0;
	m_nClassifyPlane = 0;
	m_dClassifyBX = dBLx;
	m_dClassifyBY = dBLy;

	m_dBLxKDS2021 = dBLx;
	m_dBLyKDS2021 = dBLy;
	m_dAreadKDS2021 = dBLx*dBLy;
	m_dFreqXKDS2021 = 0.0;	
	m_dFreqYKDS2021 = 0.0;	
	m_dDampingKDS2021 = 0.0;
	m_dMinBXKDS2021 = dMinBx;
	m_dMinBYKDS2021 = dMinBy;

	m_dBX = m_Data.dBLy;
	m_dBY = m_Data.dBLx;
	m_dFreqX = m_Data.dNox;
	m_dFreqY = m_Data.dNoy;
	m_dFreqT = m_Data.dNot;
	m_dMassX = m_Data.dMx1st;
	m_dMassY = m_Data.dMy1st;
	m_dMoment = m_Data.dMt1st;
	m_dTotalM = m_Data.dTotalMass;
	m_dVibration = m_Data.dVibrationMode;
	m_dZf = m_Data.dDampingRatio;
	
	for (int i=0; i<2; i++)
	{
		m_dAmFactorAlong[i] = m_Data.dAmFactorAlong[i];
		m_dAmFactorAcross[i] = m_Data.dAmFactorAcross[i];
		m_dAmFactorTorsional[i] = m_Data.dAmFactorTorsional[i];
	}

	m_dGustVibration = 0.5;

	m_StoryHeight.RemoveAll();
	m_StoryMass.RemoveAll();

	double dMX = 0., dMY = 0., dMXY = 0., dTotal = 0., dGeneralMass = 0., dGustTotalMass = 0., dHmax = 0.;
	GetMassXYValue(m_dGustVibration, dMX, dMY, dMXY, dTotal, dGeneralMass, dGustTotalMass, dHmax);

	// for gust factor
	m_dGustM = dGustTotalMass;
	m_dGustMx = dGeneralMass;// dTotal/3;
	m_dGustMy = dGeneralMass;// dTotal/3;
	// Separate from Average Height.
	// Keep displaying Average Height in CWindGustKDS2021Dlg, 
	// but define a separate internal height for internal use only, so CEditUnit is not defined.
	m_dStoryHMax = dHmax;

	// Separate the mass for "parameters of wind vibration" and the mass for "gust factor".
	// Be cautious that the reference height used for mass calculation in GetMassXYValue is different.
	if (!m_bModify) 
	{
		m_dMassX  = dMX/3;
		m_dMassY  = dMY/3;
		m_dMoment = dMXY * (pow(dBLx,2)+pow(dBLy,2))/36;
		m_dTotalM = dTotal;
		m_dBX = dBLx;
		m_dBY = dBLy;
		m_Data.dRoofHeight	= StorD.dStoryLevel - dBaseLevel;
		m_Data.dSimpleRoofHeight = StorD.dStoryLevel - dBaseLevel;
		m_Data.dVortexAverageRoof = StorD.dStoryLevel - dBaseLevel;
		m_Data.dVortexDM = max(dMaxDn4Dm, dMaxUp4Dm);
		m_Data.dVortexDB = max(dMaxUp4DB, dMaxDn4DB);
		m_Data.dVortexMass = m_dTotalM;
		m_editGustFactorX.SetWindowText(_T("2.2")); 
		m_editGustFactorY.SetWindowText(_T("2.2"));
	}
}

void CCMWindItemKDS2021::InitCategoryCombo()
{
	m_cobxCategory.ResetContent();
	
	int nIndex;
	CString aItem[] = {_T("A"), _T("B"), _T("C"), _T("D")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
	{
		nIndex = m_cobxCategory.AddString(aItem[i]);
		m_cobxCategory.SetItemData(nIndex , i+1);
	}
	m_cobxCategory.SetCurSel(0);
}

void CCMWindItemKDS2021::InitImportFactorCombo()
{
	m_cobxImportFactor.ResetContent();
	
	CString aItem[] = {_T("0.9"), _T("0.95"), _T("1.0"), _T("1.05")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxImportFactor.AddString(aItem[i]);
	m_cobxImportFactor.SetCurSel(0); 
}

void CCMWindItemKDS2021::InitStructureTypeCombo()
{
	m_cmbStructureType.ResetContent();
	int nIndex = 0;
	nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_CIRCLE_PLAN)); m_cmbStructureType.SetItemData(nIndex, ENM_T_CIRCLE_PLAN);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_LATTICE_TOWERS)); m_cmbStructureType.SetItemData(nIndex, ENM_T_LATTICE_TOWERS);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_OPEN_SIGNS_LATTICE_FRAMEWORKS)); m_cmbStructureType.SetItemData(nIndex, ENM_T_OPENSIGNSLF);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_CHIMNEYS_TANKS_SIMILARSTRUCTURES)); m_cmbStructureType.SetItemData(nIndex, ENM_T_CHIMNEYSTSS);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_FENCES_ON_GROUND)); m_cmbStructureType.SetItemData(nIndex, ENM_T_FENCES_ON_GRND);
	m_cmbStructureType.SetCurSel(0);
}

void CCMWindItemKDS2021::InitCeFactorCombo()
{
	m_cobxCeFactor.ResetContent();

	CString aItem[] = {_T("1.0"), _T("1.5")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxCeFactor.AddString(aItem[i]);
	m_cobxCeFactor.SetCurSel(0); 
}

void CCMWindItemKDS2021::AlignControls()
{
	CRect ToRect, FromRect;
	
	GetDlgItem(IDC_CMD_SIMPLE_SPEED_TITLE)->GetWindowRect(FromRect);
	
	// 간편법 ctrl들을 위로 올림 
	GetDlgItem(IDC_CMD_KOR_EC_TITLE)->GetWindowRect(ToRect);
	int nDistY = ToRect.top - FromRect.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlSimple, nDistY);

	// 와류진동 풍하중 ctrl들을 위로 올림
	GetDlgItem(IDC_CMD_VORTEX_ROOF_HEIGHT_STC)->GetWindowRect(FromRect);
	nDistY = ToRect.top - FromRect.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlVortex, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_FRAME);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+4;
	MoveWindow(r);
}

void CCMWindItemKDS2021::Data2Dlg()
{
	CString csVal;

	m_chkVortex.SetCheck(m_Data.bVortexShedding);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlMethod, m_Data.nMethod);
	OnCmdMethodRadio();

	if(m_Data.nMethod==0)
	{
		m_Data.dBasicWindSpeed = m_Data.dSimpleBasicWindSpeed;
		m_Data.dRoofHeight = m_Data.dSimpleRoofHeight;
	}
	else
	{
		if (m_Data.bVortexShedding)
		{
			m_Data.dSimpleRoofHeight = m_Data.dVortexAverageRoof;
			m_Data.dRoofHeight = m_Data.dVortexAverageRoof;
		}
		else
		{
			m_Data.dSimpleBasicWindSpeed = m_Data.dBasicWindSpeed;
			m_Data.dSimpleRoofHeight = m_Data.dRoofHeight;
			m_Data.dVortexAverageRoof = m_Data.dRoofHeight;
		}
	}

	//////////////////////////////////////////
	// 간편법
	m_editSpeed2.SetEditUnit(m_Data.dSimpleBasicWindSpeed);
	m_editRoofHeight2.SetEditUnit(m_Data.dSimpleRoofHeight);
	if (m_Data.dSimpleCe == 1.0  || m_Data.dSimpleCe == 1.5 )
	{
		csVal.Format(_T("%g"), m_Data.dSimpleCe);
		m_cobxCeFactor.SelectString(-1, csVal);
	}
	else
	{
		csVal.Format(_T("%g"), m_Data.dSimpleCe);
		m_cobxCeFactor.SetWindowText(csVal);
	}
	m_editMinX.SetEditUnit(m_dMinBYKDS2021);
	m_editMinY.SetEditUnit(m_dMinBXKDS2021);
	m_editMinX.SetReadOnly(TRUE);
	m_editMinY.SetReadOnly(TRUE);

	//////////////////////////////////////////
	// 일반법
	m_cobxCategory.SetCurSel(m_Data.nExposureCategory-1);
	m_editSpeed.SetEditUnit(m_Data.dBasicWindSpeed);
	if (m_Data.dImportanceFactor == 0.9  || m_Data.dImportanceFactor == 0.95 || m_Data.dImportanceFactor == 1.0  || m_Data.dImportanceFactor == 1.05 )
	{
		csVal.Format(_T("%g"), m_Data.dImportanceFactor);
		m_cobxImportFactor.SelectString(-1, csVal);
	}
	else
	{
		csVal.Format(_T("%g"), m_Data.dImportanceFactor);
		m_cobxImportFactor.SetWindowText(csVal);
	}
	
	m_editRoofHeight.SetEditUnit(m_Data.dRoofHeight);

	m_chkTopoEffect.SetCheck(m_Data.bTopographicEffect);
	OnCmdTopoEffectCheck();
	m_editKZT.SetEditUnit(m_Data.dKzt);
	m_editDirectFactorX.SetEditUnit(m_Data.dDistFactorX);
	m_editDirectFactorY.SetEditUnit(m_Data.dDistFactorY);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlType, m_Data.nRigidity);
	m_editGustFactorX.SetEditUnit(m_Data.dGustFactorX, 4);
	m_editGustFactorY.SetEditUnit(m_Data.dGustFactorY, 4);
	m_chkLoadEval.SetCheck(m_Data.bUseForceCoef);
	int nCheckForceCoefType = (m_Data.bAutoCalc)? D_CMD_WIND_FORCE_COEF_CALC_TYPE_AUTO : D_CMD_WIND_FORCE_COEF_CALC_TYPE_USER;
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoForceCoefCalcType, nCheckForceCoefType);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRiseType, m_Data.nBuildingType);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlRise, m_Data.nBuildingType==1);
	m_chkAcrossWind.SetCheck(m_Data.bAcrossWind);
	m_chkTorsionalWind.SetCheck(m_Data.bTorsionalWind);
	m_chkWindResponse.SetCheck(m_Data.bWindResponse);
	m_chkLShape.SetCheck(m_Data.bLShapeBuilding);
	OnCmdLoadEvalCheck();
	m_editForceCoef.SetEditUnit(m_Data.dForceCoefficient);

	m_editRoofHeight3.SetEditUnit(m_Data.dVortexAverageRoof);
	m_editDiaDM.SetEditUnit(m_Data.dVortexDM);
	m_editDiaDB.SetEditUnit(m_Data.dVortexDB);
	m_editNatural.SetEditUnit(m_Data.dVortexN);
	m_editMass.SetEditUnit(m_Data.dVortexMass);
	m_editDamping.SetEditUnit(m_Data.dVortexDamp);


	if(m_Data.nMethod==1 && !m_Data.bUseForceCoef)
	{
		m_Data.dBLy = m_dBX;
		m_Data.dBLx = m_dBY;
		m_Data.dNox = m_dFreqX;
		m_Data.dNoy = m_dFreqY;
		m_Data.dNot = m_dFreqT;
		m_Data.dMx1st = m_dMassX;
		m_Data.dMy1st = m_dMassY;
		m_Data.dMt1st = m_dMoment;
		m_Data.dDampingRatio = m_dZf;
	}

	int nStructType = m_cmbStructureType.GetCount();
	for(int i=0; i<nStructType; i++)
	{
		if(m_cmbStructureType.GetItemData(i)!=m_Data.nWnatStructType) continue;
		m_cmbStructureType.SetCurSel(i);
		break;
	}
}

BOOL CCMWindItemKDS2021::Dlg2Data()
{
	CString csVal;
	
	T_WNAT_K WnatK = m_Data.keyWnat;

	m_Data.Initialize();

	if(m_bModify) m_Data.keyWnat = WnatK;

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, m_Data.nMethod);
	m_Data.bVortexShedding = m_chkVortex.GetCheck()==1 ? TRUE : FALSE;

	if(m_Data.nMethod==0)
	{
		m_Data.dBasicWindSpeed = m_Data.dSimpleBasicWindSpeed;
		m_Data.dRoofHeight = m_Data.dSimpleRoofHeight;
	}
	else
	{
		m_Data.dSimpleBasicWindSpeed = m_Data.dBasicWindSpeed;
		m_Data.dSimpleRoofHeight = m_Data.dRoofHeight;
	}

	if(m_Data.nMethod == 0)
	{
		//////////////////////////////////////////
		// 간편법
		m_Data.dSimpleBasicWindSpeed = m_editSpeed2.GetEditValue();
		m_Data.dSimpleRoofHeight = m_editRoofHeight2.GetEditValue();
		m_cobxCeFactor.GetWindowText(csVal);
		m_Data.dSimpleCe = _tstof(csVal);

		if(m_Data.dSimpleRoofHeight>0. && pow(m_dMinBXKDS2021*m_dMinBYKDS2021, 0.5) <= m_Data.dSimpleRoofHeight)
		{
			CString strMsg = _T("");
			strMsg.Format(_LS(IDS_DB_IREG_DT_VAR_ERROR), _LS(IDS_DB_DT_WIND), _T("Building Height(H)"));
			strMsg += _LS(IDS_CMD_ERR_WIND_KDS2021_SIMPLE);
			GSaveHistoryNF(strMsg);
			return FALSE;
		}
	}
	else
	{
		if (m_Data.bVortexShedding)
		{
			m_Data.dVortexAverageRoof = m_editRoofHeight3.GetEditValue();
			m_Data.dVortexDM = m_editDiaDM.GetEditValue();
			m_Data.dVortexDB = m_editDiaDB.GetEditValue();
			m_Data.dVortexN = m_editNatural.GetEditValue();
			m_Data.dVortexMass = m_editMass.GetEditValue();
			m_Data.dVortexDamp = m_editDamping.GetEditValue();
		}
		else
		{
			//////////////////////////////////////////
		// 일반법
			m_Data.nExposureCategory = m_cobxCategory.GetCurSel() + 1;
			m_Data.dBasicWindSpeed = m_editSpeed.GetEditValue();

			m_cobxImportFactor.GetWindowText(csVal);
			m_Data.dImportanceFactor = _tstof(csVal);

			m_Data.dRoofHeight = m_editRoofHeight.GetEditValue();
			m_Data.bTopographicEffect = m_chkTopoEffect.GetCheck();
			m_Data.dDistFactorX = m_editDirectFactorX.GetEditValue();
			m_Data.dDistFactorY = m_editDirectFactorY.GetEditValue();

			if (m_Data.bTopographicEffect)
			{
				m_Data.dKzt = m_editKZT.GetEditValue();
			}
			CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlType, m_Data.nRigidity);
			m_Data.dGustFactorX = m_editGustFactorX.GetEditValue();
			m_Data.dGustFactorY = m_editGustFactorY.GetEditValue();
			m_Data.bUseForceCoef = m_chkLoadEval.GetCheck();
			if (m_Data.bUseForceCoef)
				m_Data.dForceCoefficient = m_editForceCoef.GetEditValue();

			int nCheckForceCoefType = D_CMD_WIND_FORCE_COEF_CALC_TYPE_USER;
			CDlgUtil::CtrlRadioGetCheck(this, m_aRdoForceCoefCalcType, nCheckForceCoefType);
			m_Data.bAutoCalc = (nCheckForceCoefType == D_CMD_WIND_FORCE_COEF_CALC_TYPE_AUTO) ? TRUE : FALSE;

			m_Data.nWnatStructType = m_cmbStructureType.GetItemData(m_cmbStructureType.GetCurSel());

			CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRiseType, m_Data.nBuildingType);
			m_Data.bAcrossWind = m_chkAcrossWind.GetCheck();
			m_Data.bTorsionalWind = m_chkTorsionalWind.GetCheck();
			m_Data.bWindResponse = m_chkWindResponse.GetCheck();
			m_Data.bLShapeBuilding = m_chkLShape.GetCheck();

			m_Data.dBLy = m_dBX;
			m_Data.dBLx = m_dBY;
			m_Data.dNox = m_dFreqX;
			m_Data.dNoy = m_dFreqY;
			m_Data.dNot = m_dFreqT;
			m_Data.dMx1st = m_dMassX;
			m_Data.dMy1st = m_dMassY;
			m_Data.dMt1st = m_dMoment;
			m_Data.dDampingRatio = m_dZf;
			m_Data.dTotalMass = m_dTotalM;
			m_Data.dVibrationMode = m_dVibration;
		}

		for (int i = 0; i < 2; i++)
		{
			m_Data.dAmFactorAlong[i] = m_dAmFactorAlong[i];
			m_Data.dAmFactorAcross[i] = m_dAmFactorAcross[i];
			m_Data.dAmFactorTorsional[i] = m_dAmFactorTorsional[i];
		}
	}
	
	return TRUE;
}

LRESULT CCMWindItemKDS2021::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	BOOL bFolded = m_grpWindParam.GetFoldState();
	if (!bFolded)
	{
		OnCmdMethodRadio();
	}

	CRect rcChild;
	GetWindowRect(rcChild);
	m_pParent->AlignCtrl2Param(rcChild.bottom);

	return 0l;
}

// MQC 15301
// Mx* 계산 시, M = Story Mass Table의 층별 Translational Mass X-DIR 합 
// My* 계산 시, M = Story Mass Table의 층별 Translational Mass Y-DIR 합 
// I*  계산 시, M = (Story Mass Table의 Translational Mass X-DIR 합 + Story Mass Table의 Translational Mass Y-DIR 합)/2
// Story Data에서 Ground Level 아래쪽 Level의 값은 제외하는 것으로 추가 고려가 필요합니다
void CCMWindItemKDS2021::GetMassXYValue(double dVibBeta, double& dMassX, double& dMassY, double& dMassXY, double& dTotal,
						double& dGMass, double& dGustTotal, double& dHmax)
{
	dMassX=0.0;
	dMassY=0.0;
	dMassXY=0.0;
	dMassXY = 0.0;
	dGMass = 0.0;
	dGustTotal = 0.0;
	dHmax = 0.0;

	CArray<T_STOR_K,T_STOR_K> aStorK;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
	int nNum = aStorK.GetSize();
	if (nNum == 0) return;

	T_BLDC_D BldcD;
	if (!pDoc->m_pAttrCtrl->GetBldc(BldcD)) BldcD.Initialize(); 

	double (*dDIAPMS)[3] = new double[nNum][3];
	double (*dDIAPXY)[2] = new double[nNum][2];
	double (*dRLSMSS)[2] = new double[nNum][2];

	CLateralLoad StorMass;
	BOOL bUseBedrockLevel = FALSE;
	StorMass.Make_DiaphramMassTableData(dDIAPMS, dDIAPXY, dRLSMSS, bUseBedrockLevel);

	double dTol = 1.e-10;

	T_STOR_D StorD;
	for(int i=0; i<aStorK.GetSize(); i++)
	{
		if(!pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD)) { ASSERT(0); continue;}
		if(StorD.dStoryLevel < BldcD.dBaseLevel - dTol)
			continue;

		dMassX  += dDIAPMS[i][0] + dRLSMSS[i][0];
		dMassY  += dDIAPMS[i][1] + dRLSMSS[i][1];
	}

	dMassXY = (dMassX+dMassY) / 2.0;
	dTotal = max(dMassX, dMassY);

	m_StoryHeight.SetSize(nNum);
	m_StoryMass.SetSize(nNum);

	double dBaseLevel = pDoc->m_pAttrCtrl->GetGroundLevel();
	double dStoryM, dHLevel;
	double dHmax1 = -DBL_MAX;
	double dMuz = 0;
	double zerov = 0.;

	// for gust factor : "CWindGustKDS2021Dlg"
	double dXmass = 0.,dYmass=0.;
	for(int i=0; i<aStorK.GetSize(); i++)
	{
		if(!pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD)) { ASSERT(0); continue;}
		if(StorD.dStoryLevel < dBaseLevel - dTol)
			continue;

		dXmass += dDIAPMS[i][0] + dRLSMSS[i][0];
		dYmass += dDIAPMS[i][1] + dRLSMSS[i][1];

		dHLevel = StorD.dStoryLevel - dBaseLevel;
		if (dHmax1 < dHLevel) dHmax1 = dHLevel;
	}
	dHmax = dHmax1;
	dGustTotal = max(dXmass,dYmass);

	int idir=0;
	if(dXmass < dYmass) idir = 1;

	for (int i = 0; i < aStorK.GetSize(); i++)
	{
		m_StoryHeight.SetAt(i,zerov);
		m_StoryMass.SetAt(i,zerov);

		if (!pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD)) { ASSERT(0); continue; }
		if (StorD.dStoryLevel < dBaseLevel - dTol) continue;

		dStoryM = dDIAPMS[i][idir] + dRLSMSS[i][idir];

		dHLevel = StorD.dStoryLevel - dBaseLevel;
		dMuz = pow(dHLevel/dHmax,dVibBeta);

		dGMass += dStoryM*pow(dMuz,2.);

		m_StoryHeight.SetAt(i,dHLevel);
		m_StoryMass.SetAt(i,dStoryM);
	}

	delete []dDIAPMS;
	delete []dDIAPXY;
	delete []dRLSMSS;
}


#undef D_CMD_WIND_FORCE_COEF_CALC_TYPE_USER
#undef D_CMD_WIND_FORCE_COEF_CALC_TYPE_AUTO
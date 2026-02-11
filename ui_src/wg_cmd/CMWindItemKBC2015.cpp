// CMWindItemKBC2015.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemDlg.h"
#include "CMWindItemKBC2015.h"
#include "WindClassifyKBC2015Dlg.h"
#include "WindVibrationDlg.h"
#include "WindGustKBC2008Dlg.h"
#include "WindAutoCalcDlg.h"

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
// CCMWindItemKBC2015 dialog


CCMWindItemKBC2015::CCMWindItemKBC2015(CWnd* pParent /*=NULL*/)
: CChildDialog(CCMWindItemKBC2015::IDD, pParent)
{
	m_pParent = NULL;
	m_bModify = FALSE;
	m_KeyWAorWT = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;
	//{{AFX_DATA_INIT(CCMWindItemKBC2015)
	//}}AFX_DATA_INIT

	m_aCtrlMethod.Add(IDC_CMD_METHOD_RD1);
	m_aCtrlMethod.Add(IDC_CMD_METHOD_RD2);

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
	m_aCtrlRise.FreeExtra();
	
	m_bInit = FALSE;
	m_Data.Initialize();
	m_WnatD.Initialize();
}

void CCMWindItemKBC2015::SetData2Dlg(T_WIND_KBC2015& data, BOOL bModify, T_KEY KeyWAorWT)
{
	m_Data = data;
	m_bModify = bModify;
	m_KeyWAorWT = KeyWAorWT;
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pAttrCtrl2->GetWnat(m_Data.keyWnat, m_WnatD)) m_WnatD.Initialize();
}

BOOL CCMWindItemKBC2015::SetDlg2Data(T_WIND_KBC2015& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CCMWindItemKBC2015::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemKBC2015)
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
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemKBC2015, CChildDialog)
//{{AFX_MSG_MAP(CCMWindItemKBC2015)
	ON_BN_CLICKED(IDC_CMD_METHOD_RD1, OnCmdMethodRadio)
	ON_BN_CLICKED(IDC_CMD_METHOD_RD2, OnCmdMethodRadio)
	ON_BN_CLICKED(IDC_CMD_METHOD_GENERAL_BTN, OnCmdGeneralBtn)
	ON_BN_CLICKED(IDC_CMD_GUST_FACTOR_BTN, OnCmdGustFactorBtn)
	ON_BN_CLICKED(IDC_CMD_WIND_VIBRATION_PARAM_BTN, OnCmdVibrationBtn)
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
	ON_EN_CHANGE(IDC_CMD_SPEED_EDIT							, OnChangeSpeedEdit)
	ON_EN_CHANGE(IDC_CMD_SIMPLE_SPEED_EDIT			, OnChangeSimpleSpeedEdit)
	ON_EN_CHANGE(IDC_CMD_ROOF_HEIGHT_EDIT				, OnChangeRoofHeightEdit)
	ON_EN_CHANGE(IDC_CMD_SIMPLE_ROOF_HEIGHT_EDIT, OnChangeSimpleRoofHeightEdit)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemKBC2015 message handlers

BOOL CCMWindItemKBC2015::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	AlignControls();
	
	m_grpWindParam.InitControl(this, CCMWindItemKBC2015::IDD, IDC_CMD_FRAME, TRUE, TRUE); // AlignControls 다음에
	m_grpWindParam.SetFoldState(FALSE); // 무조건 펼친다

	m_editSpeed2.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dBasicWindSpeed);
	m_editSpeed.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dBasicWindSpeed);

	m_unitRoofHeight2.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);
	m_editRoofHeight2.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);
	m_unitRoofHeight.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);
	m_editRoofHeight.SetUnitType(CUnitCtrl::m_WIND_UNIT.KBC2009_dRoofHeight);

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
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CCMWindItemKBC2015::DestroyWindow() 
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

void CCMWindItemKBC2015::OnCmdMethodRadio()
{
	int nMethod=1;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, nMethod);
	BOOL bFolded = m_grpWindParam.GetFoldState();
	if(!bFolded)
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlSimple, nMethod == 0);
		CDlgUtil::CtrlShowHide(this, m_aCtrlGeneral, nMethod == 1);
	}

	int nAcrossCheck = nMethod == 1 ? m_chkAcrossWind.GetCheck() : 0;
	int nTorsionalCheck = nMethod == 1 ? m_chkTorsionalWind.GetCheck() : 0;

	m_pParent->UpdateGrid(TRUE,nAcrossCheck,nTorsionalCheck);
}

void CCMWindItemKBC2015::OnCmdGeneralBtn() 
{
	CWindClassifyKBC2015Dlg dlg(TRUE);
	dlg.SetInitPos(D_INIT_POS_RT);

	int nMethod=1;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, nMethod);
		
	dlg.m_dHeight			= nMethod == 0 ? m_editRoofHeight2.GetEditValue() : m_editRoofHeight.GetEditValue();
	dlg.m_dWidth			= m_dClassifyBY;
	dlg.m_dBreadth		= m_dClassifyBX;
	dlg.m_dArea				= m_dAreadKBC2015;

	dlg.DoModal();
}

void CCMWindItemKBC2015::OnCmdGustFactorBtn() 
{
	// TODO: Add your control notification handler code here	
	CWindGustKBC2009Dlg dlg(KBC_W_2016);
	dlg.SetInitPos(D_INIT_POS_RT);
	
	CString str = _T("");
	m_cobxImportFactor.GetWindowText(str);
	
	dlg.m_dHeight			= m_editRoofHeight.GetEditValue();
	dlg.m_dImportance = _tstof(str);
	dlg.m_nExposure		= m_cobxCategory.GetCurSel();
	dlg.m_dSpeed			= m_editSpeed.GetEditValue();
	dlg.m_dKzt				= m_editKZT.GetEditValue();
	dlg.m_dHzt        = 0.0;
	dlg.m_bTopography = m_chkTopoEffect.GetCheck();
	dlg.m_dWidth			= m_dBLyKBC2015;
	dlg.m_dBreadth		= m_dBLxKBC2015;
	dlg.m_dFrequencyX = m_dFreqXKBC2015;
	dlg.m_dFrequencyY = m_dFreqYKBC2015;
	dlg.m_dDamping	  = m_dDampingKBC2015;  
	
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlType, dlg.m_nRigidity); 
	
	if (dlg.DoModal() == IDOK)
	{
		m_editGustFactorX.SetEditUnit(dlg.m_dGustFactorX, 4);
		m_editGustFactorY.SetEditUnit(dlg.m_dGustFactorY, 4);
		
		m_dBLyKBC2015			= dlg.m_dWidth;
		m_dBLxKBC2015			= dlg.m_dBreadth;
		m_dFreqXKBC2015		= dlg.m_dFrequencyX;
		m_dFreqYKBC2015		= dlg.m_dFrequencyY;
		m_dDampingKBC2015 = dlg.m_dDamping;		
	}	
}

void CCMWindItemKBC2015::OnCmdVibrationBtn() 
{
	CWindVibrationDlg dlg;
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
	dlg.m_dMassX = m_dMassX;
	dlg.m_dMassY = m_dMassY;
	dlg.m_dMoment = m_dMoment;
	dlg.m_dZf = m_dZf;

	if (dlg.DoModal() == IDOK)
	{
		m_dBY= dlg.m_dBreadthX;
		m_dBX = dlg.m_dBreadthY;

		m_dFreqX = dlg.m_dFreqX;
		m_dFreqY = dlg.m_dFreqY;
		m_dFreqT = dlg.m_dFreqT;
		m_dMassX = dlg.m_dMassX;
		m_dMassY = dlg.m_dMassY;
		m_dMoment = dlg.m_dMoment;
		m_dZf = dlg.m_dZf;
	}
}

void CCMWindItemKBC2015::OnCmdLoadEvalCheck() 
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
	}
}

void CCMWindItemKBC2015::OnCmdTopoEffectCheck() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkTopoEffect.GetCheck();
	m_editKZT.EnableWindow(nCheck == 1);
}

void CCMWindItemKBC2015::OnCmdChangeRdo_ForceCoefCalcType()
{
	int nCheck = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoForceCoefCalcType, nCheck);
	
	BOOL bEnableUser = (nCheck == D_CMD_WIND_FORCE_COEF_CALC_TYPE_USER) ? TRUE : FALSE;
	BOOL bEnableAuto = (nCheck == D_CMD_WIND_FORCE_COEF_CALC_TYPE_AUTO) ? TRUE : FALSE;
	
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlForceCoef_user, bEnableUser);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlForceCoef_auto, bEnableAuto);
}

void CCMWindItemKBC2015::OnCmdRiseTypeRadio()
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

void CCMWindItemKBC2015::OnCmdWindTypeCheck()
{
	int nLoadEvalCheck = m_chkLoadEval.GetCheck();
	int nBuildingType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRiseType, nBuildingType);
	int nAcrossCheck = m_chkAcrossWind.GetCheck();
	int nTorsionalCheck = m_chkTorsionalWind.GetCheck();
	int nResponseCheck = m_chkWindResponse.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_WIND_VIBRATION_PARAM_BTN, nLoadEvalCheck==0 && (nAcrossCheck == 1 || nTorsionalCheck == 1 || nResponseCheck == 1));
	m_pParent->UpdateGrid(TRUE,nAcrossCheck,nTorsionalCheck);
}

void CCMWindItemKBC2015::OnBtnAutoCalc()
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

void CCMWindItemKBC2015::OnChangeSpeedEdit()
{
	double dValue = m_editSpeed.GetEditValue();
	m_editSpeed2.SetEditUnit(dValue);
}

void CCMWindItemKBC2015::OnChangeSimpleSpeedEdit()
{
	double dValue = m_editSpeed2.GetEditValue();
	m_editSpeed.SetEditUnit(dValue);
}

void CCMWindItemKBC2015::OnChangeRoofHeightEdit()
{
	double dValue = m_editRoofHeight.GetEditValue();
	m_editRoofHeight2.SetEditUnit(dValue);
}

void CCMWindItemKBC2015::OnChangeSimpleRoofHeightEdit()
{
	double dValue = m_editRoofHeight2.GetEditValue();
	m_editRoofHeight.SetEditUnit(dValue);
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMWindItemKBC2015::InitDefaultData()
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
	if (nStorNum > 0)
	{	
		BOOL bFirst=TRUE;
		for (int i=0; i<nStorNum; i++)
		{
			pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD2);

			// MQC 15282 : Min.Breadth 값을 가져올 때 Story Data에서 Ground Level 아래쪽 Level의 값을 제외하고 
			// 최소값을 가지고 올 수 있도록 수정부탁드립니다. 
			if(StorD2.dStoryLevel < dBaseLevel-(1.e-10))
				continue;
			
			if(bFirst)
			{
				dMinBx = StorD2.WindWidthX;
				dMinBy = StorD2.WindWidthY;
				bFirst=FALSE;
				continue;
			}
			if(dMinBx>StorD2.WindWidthX) dMinBx = StorD2.WindWidthX;
			if(dMinBy>StorD2.WindWidthY) dMinBy = StorD2.WindWidthY;
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

	m_dBLxKBC2015 = dBLx;
	m_dBLyKBC2015 = dBLy;
	m_dAreadKBC2015 = dBLx*dBLy;
	m_dFreqXKBC2015 = 0.0;	
	m_dFreqYKBC2015 = 0.0;	
	m_dDampingKBC2015 = 0.0;
	m_dMinBXKBC2015 = dMinBx;
	m_dMinBYKBC2015 = dMinBy;

	m_dBX = m_Data.dBLy;
	m_dBY = m_Data.dBLx;
	m_dFreqX = m_Data.dNox;
	m_dFreqY = m_Data.dNoy;
	m_dFreqT = m_Data.dNot;
	m_dMassX = m_Data.dMx1st;
	m_dMassY = m_Data.dMy1st;
	m_dMoment = m_Data.dMt1st;
	m_dZf = m_Data.dDampingRatio;
	
	if (!m_bModify) 
	{
		double dMX, dMY, dMXY;
		GetMassXYValue(dMX,dMY,dMXY);
		m_dMassX  = dMX/3;
		m_dMassY  = dMY/3;
		m_dMoment = dMXY * (pow(dBLx,2)+pow(dBLy,2))/36;

		m_dBX = dBLx;
		m_dBY = dBLy;
		m_Data.dRoofHeight	= StorD.dStoryLevel - dBaseLevel;
		m_Data.dSimpleRoofHeight = StorD.dStoryLevel - dBaseLevel;
		m_editGustFactorX.SetWindowText(_T("2.2")); 
		m_editGustFactorY.SetWindowText(_T("2.2"));
	}
}

void CCMWindItemKBC2015::InitCategoryCombo()
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

void CCMWindItemKBC2015::InitImportFactorCombo()
{
	m_cobxImportFactor.ResetContent();
	
	CString aItem[] = {_T("0.9"), _T("0.95"), _T("1.0"), _T("1.05")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxImportFactor.AddString(aItem[i]);
	m_cobxImportFactor.SetCurSel(0); 
}

void CCMWindItemKBC2015::InitStructureTypeCombo()
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

void CCMWindItemKBC2015::InitCeFactorCombo()
{
	m_cobxCeFactor.ResetContent();

	CString aItem[] = {_T("1.0"), _T("1.5"), _T("2.0")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxCeFactor.AddString(aItem[i]);
	m_cobxCeFactor.SetCurSel(0); 
}

void CCMWindItemKBC2015::AlignControls()
{
	CRect ToRect, FromRect;
	
	GetDlgItem(IDC_CMD_SIMPLE_SPEED_TITLE)->GetWindowRect(FromRect);
	GetDlgItem(IDC_CMD_KOR_EC_TITLE)->GetWindowRect(ToRect);
	int nDistY = ToRect.top - FromRect.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlSimple, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_FRAME);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+4;
	MoveWindow(r);
}

void CCMWindItemKBC2015::Data2Dlg()
{
	CString csVal;

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlMethod, m_Data.nMethod);
	OnCmdMethodRadio();

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

	//////////////////////////////////////////
	// 간편법
	m_editSpeed2.SetEditUnit(m_Data.dSimpleBasicWindSpeed);
	m_editRoofHeight2.SetEditUnit(m_Data.dSimpleRoofHeight);
	if (m_Data.dSimpleCe == 1.0  || m_Data.dSimpleCe == 1.5  || m_Data.dSimpleCe == 2.0)
	{
		csVal.Format(_T("%g"), m_Data.dSimpleCe);
		m_cobxCeFactor.SelectString(-1, csVal);
	}
	else
	{
		csVal.Format(_T("%g"), m_Data.dSimpleCe);
		m_cobxCeFactor.SetWindowText(csVal);
	}
	m_editMinX.SetEditUnit(m_dMinBYKBC2015);
	m_editMinY.SetEditUnit(m_dMinBXKBC2015);
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
	OnCmdLoadEvalCheck();
	m_editForceCoef.SetEditUnit(m_Data.dForceCoefficient);

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

BOOL CCMWindItemKBC2015::Dlg2Data()
{
	CString csVal;
	
	T_WNAT_K WnatK = m_Data.keyWnat;

	m_Data.Initialize();

	if(m_bModify) m_Data.keyWnat = WnatK;

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, m_Data.nMethod);

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

		if(m_Data.dSimpleRoofHeight>0. && (m_dMinBXKBC2015/m_Data.dSimpleRoofHeight<=1.0 || m_dMinBYKBC2015/m_Data.dSimpleRoofHeight<=1.0))
		{
			CString strMsg = _T("");
			strMsg.Format(_LS(IDS_DB_IREG_DT_VAR_ERROR), _LS(IDS_DB_DT_WIND), _T("Building Height(H)"));
			strMsg += _T("H<B 조건을 만족해야 합니다.(형상비 1.0이하)");
			GSaveHistoryNF(strMsg);
			return FALSE;
		}
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
		if (m_Data.bTopographicEffect)
		{
			m_Data.dKzt = m_editKZT.GetEditValue();
			m_Data.dVzt = 0.0;
		}
		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlType, m_Data.nRigidity);
		m_Data.dGustFactorX = m_editGustFactorX.GetEditValue();
		m_Data.dGustFactorY = m_editGustFactorY.GetEditValue();
		m_Data.bUseForceCoef = m_chkLoadEval.GetCheck();
		if (m_Data.bUseForceCoef)
			m_Data.dForceCoefficient = m_editForceCoef.GetEditValue();

		int nCheckForceCoefType = D_CMD_WIND_FORCE_COEF_CALC_TYPE_USER;
		CDlgUtil::CtrlRadioGetCheck(this, m_aRdoForceCoefCalcType, nCheckForceCoefType);
		m_Data.bAutoCalc = (nCheckForceCoefType == D_CMD_WIND_FORCE_COEF_CALC_TYPE_AUTO)? TRUE : FALSE;

		m_Data.nWnatStructType = m_cmbStructureType.GetItemData(m_cmbStructureType.GetCurSel());

		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRiseType, m_Data.nBuildingType);
		m_Data.bAcrossWind = m_chkAcrossWind.GetCheck();
		m_Data.bTorsionalWind = m_chkTorsionalWind.GetCheck();
		m_Data.bWindResponse = m_chkWindResponse.GetCheck();

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
	
	return TRUE;
}

LRESULT CCMWindItemKBC2015::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
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
void CCMWindItemKBC2015::GetMassXYValue(double& dMassX, double& dMassY, double& dMassXY)
{
	dMassX=0.0;
	dMassY=0.0;
	dMassXY=0.0;

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

	T_STOR_D StorD;
	for(int i=0; i<aStorK.GetSize(); i++)
	{
		if(!pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD)) { ASSERT(0); continue;}
		if(StorD.dStoryLevel < BldcD.dBaseLevel-(1.e-10)) 
			continue;

		dMassX  += dDIAPMS[i][0] + dRLSMSS[i][0];
		dMassY  += dDIAPMS[i][1] + dRLSMSS[i][1];
	}
	dMassXY = (dMassX+dMassY) / 2.0;

	delete []dDIAPMS;
	delete []dDIAPXY;
	delete []dRLSMSS;
}


#undef D_CMD_WIND_FORCE_COEF_CALC_TYPE_USER
#undef D_CMD_WIND_FORCE_COEF_CALC_TYPE_AUTO
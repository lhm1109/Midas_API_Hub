#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTendonTypeAddDlg.h"
#include "CMDTendonAreaDlg.h"
#include "CMTendonSetLongTermStressLossDlg.h"
#include "CMTimeDepMatFuncDlg.h"

#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\TestEnvMgr.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\DBLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMTendonTypeAddDlg dialog
CCMTendonTypeAddDlg::CCMTendonTypeAddDlg(CWnd* pParent /*=NULL*/)
: CChildDialogMove(CCMTendonTypeAddDlg::IDD, pParent)
{
	m_strDuctArea = _T("");
	m_strFriction = _T("");
	m_strRelCoef = _T("");
	m_strTendonName = _T("");
	m_strTotalTendonArea = _T("");
	m_strUltimate = _T("");
	m_strYield = _T("");
	m_bRelaxation = FALSE;
	m_nBondType = 0;
	m_bOverStreRedFactor = FALSE;
	m_bLowRelax = TRUE;

	m_TdntKey = 0;
	m_TdntData.Initialize();
	m_strAreaNum = _T("1");

#if defined(_US) || defined(_RUS)
	SetDefaultUSData();
#endif
	m_nRelaxation = 0;
	
	m_aGroup.RemoveAll();
	m_aGroup.Add(IDC_CMD_DUCT_AREA_EDIT);
	m_aGroup.Add(IDC_CMD_FRICTION_EDIT);
	m_aGroup.Add(IDC_CMD_WOBBLE_EDIT);
	m_aGroup.Add(IDC_CMD_TPRST_BOND_TYPE1);
	m_aGroup.Add(IDC_CMD_TPRST_BOND_TYPE2);
	
	m_aCtrlRelaxModelCEB.Add(IDC_CMD_ULTIMATE_STRENGTH_STATIC);
	m_aCtrlRelaxModelCEB.Add(IDC_CMD_ULTIMATE_STRENGTH__EDIT);
	m_aCtrlRelaxModelCEB.Add(IDC_CMD_PRESS_UNIT3);
	m_aCtrlRelaxModelCEB.Add(IDC_CMD_YIELD_STRENGTH_STATIC);
	m_aCtrlRelaxModelCEB.Add(IDC_CMD_YIELD_STRENGTH_EDIT);
	m_aCtrlRelaxModelCEB.Add(IDC_CMD_PRESS_UNIT4);  
	m_aCtrlTypeInternal.Add(IDC_CMD_WOBBLE_STATIC);
	m_aCtrlTypeInternal.Add(IDC_CMD_WOBBLE_EDIT);
	m_aCtrlTypeInternal.Add(IDC_CMD_WOBBLE_UNIT);

	m_aWoodle.RemoveAll();
	m_aWoodle.Add(IDC_CMD_WOBBLE_STATIC);
	m_aWoodle.Add(IDC_CMD_WOBBLE_EDIT);
	m_aWoodle.Add(IDC_CMD_WOBBLE_UNIT);

	m_aWoodleEuro.RemoveAll();
	m_aWoodleEuro.Add(IDC_CMD_WOBBLE_EURO_RDO);
	m_aWoodleEuro.Add(IDC_CMD_WOBBLE_EURO_EDIT);
	m_aWoodleEuro.Add(IDC_CMD_WOBBLE_EURO_UNIT);
	m_aWoodleEuro.Add(IDC_CMD_WOBBLE_ANGLE_FACTOR_EURO_RDO);
	m_aWoodleEuro.Add(IDC_CMD_WOBBLE_ANGLE_FACTOR_EURO_EDT);
	m_aWoodleEuro.Add(IDC_CMD_WOBBLE_ANGLE_FACTOR_EURO_UNIT);

	m_aWobbleType.RemoveAll();
	m_aWobbleType.Add(IDC_CMD_WOBBLE_EURO_RDO);
	m_aWobbleType.Add(IDC_CMD_WOBBLE_ANGLE_FACTOR_EURO_RDO);

	//m_aCtrlTypeExternal.Add(IDC_CMD_INADVERTENT_STATIC);
	//m_aCtrlTypeExternal.Add(IDC_CMD_INADVERTENT_EDIT);
	//m_aCtrlTypeExternal.Add(IDC_CMD_INADVERTENT_UNIT);

/*
	m_aGroup.Add(IDC_CMD_TPRST_PREST_CHK);
	m_aGroup.Add(IDC_CMD_TPRST_POSSTR_CHK);
	m_aPreGroup.RemoveAll();
	m_aPreGroup.Add(IDC_CMD_TPRST_ANCHO_BEGIN_ED);
	m_aPreGroup.Add(IDC_CMD_TPRST_ANCHO_END_ED);
	m_aPreGroup.Add(IDC_CMD_DUCT_AREA_EDIT);
	m_aPreGroup.Add(IDC_CMD_FRICTION_EDIT);
	m_aPreGroup.Add(IDC_CMD_WOBBLE_EDIT);

	arBondType.RemoveAll();
	arBondType.Add(IDC_CMD_TPRST_BOND_TYPE1);
	arBondType.Add(IDC_CMD_TPRST_BOND_TYPE2);
*/
}

void CCMTendonTypeAddDlg::SetDefaultUSData()
{
	T_UNIT_INDEX CurIndex;
	CDBDoc::GetDocPoint()->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);

	double Lfactor = 0.0;
	if(CurIndex.nBase_Length==D_UNITSYS_LENGTH_INDEX_MM)			Lfactor = 300.;
	else if(CurIndex.nBase_Length==D_UNITSYS_LENGTH_INDEX_CM)	Lfactor = 30.;
	else if(CurIndex.nBase_Length==D_UNITSYS_LENGTH_INDEX_M)	Lfactor = 0.3;
	else if(CurIndex.nBase_Length==D_UNITSYS_LENGTH_INDEX_IN)	Lfactor = 12.;
	else if(CurIndex.nBase_Length==D_UNITSYS_LENGTH_INDEX_FT)	Lfactor = 1.;

	double Ffactor = 0.0;
	if(CurIndex.nBase_Force==D_UNITSYS_FORCE_INDEX_KG)				Ffactor = 450.;
	else if(CurIndex.nBase_Force==D_UNITSYS_FORCE_INDEX_TON)	Ffactor = 0.45;
	else if(CurIndex.nBase_Force==D_UNITSYS_FORCE_INDEX_N)		Ffactor = 4500.;
	else if(CurIndex.nBase_Force==D_UNITSYS_FORCE_INDEX_KN)		Ffactor = 45000.;
	else if(CurIndex.nBase_Force==D_UNITSYS_FORCE_INDEX_LBF)	Ffactor = 1000.;
	else if(CurIndex.nBase_Force==D_UNITSYS_FORCE_INDEX_KIP)	Ffactor = 1.;

	m_TdntData.dTendonArea = 0.*pow(Lfactor,2);
	m_TdntData.dDuctArea = 0.*Lfactor;
	m_TdntData.dWobbleFactor = 0.0066/Lfactor;
	m_TdntData.dUltimateStrength = 39000*Ffactor/pow(Lfactor,2);
	m_TdntData.dYieldStrength = 33000*Ffactor/pow(Lfactor,2);
	m_TdntData.dAnchoSlipBegin = 0.012*Lfactor;
	m_TdntData.dAnchoSlipEnd = 0.02*Lfactor;
}

void CCMTendonTypeAddDlg::SetData4Euro()
{
	int nIndex = m_cobxRelaxMethod.GetCurSel();
	DWORD data = m_cobxRelaxMethod.GetItemData(nIndex);
	if(data == TDNT_RELAX_EUROPEAN || data == TDNT_RELAX_CEB_FIP || data == TDNT_RELAX_CEB_FIP_1990 || data == TDNT_RELAX_CEB_FIP_2010 || data == TDNT_RELAX_ABNT_NBR_6118_2023)
	{
		int nCurSel = m_wndTendonTypeCombo.GetCurSel();
		if(nCurSel==0)  return; // Internal-Pre

		m_TdntData.dWobbleFactor = m_TdntData.dFrictionFactor * m_TdntData.dWobbleAngle;
		m_wndWobbleEURO.SetEditUnit(m_TdntData.dWobbleFactor);

	}
}

void CCMTendonTypeAddDlg::SetDesignCode()
{
	m_cmbDgnCode.ResetContent();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CArray<CString, CString&> aDgnCode;
	CString strMatlType = _T("S");
	pDoc->m_pMatlDB->GetDesignCodeList4Strand(strMatlType, aDgnCode);

	for (auto i = 0; i < aDgnCode.GetSize(); i++)
	{
		m_cmbDgnCode.AddString(aDgnCode[i]);
	}

	m_cmbDgnCode.AddString(_LS(IDS_CMD_TendonTypeAdd_User));

	CDlgUtil::CobxAdjustListBoxWidth(m_cmbDgnCode);
	m_cmbDgnCode.SetCurSel(0);
}

void CCMTendonTypeAddDlg::SetDesignDia()
{
	m_cmbDgnDia.ResetContent();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CArray<CString, CString&> aStrandNameList;

	CString strDgnCode;
	int nSel = m_cmbDgnCode.GetCurSel();
	if (nSel != CB_ERR) 
	{
		m_cmbDgnCode.GetLBText(nSel, strDgnCode);
	}

	pDoc->m_pMatlDB->GetStrandNameList(strDgnCode, aStrandNameList);

	for (auto i = 0; i < aStrandNameList.GetSize(); i++)
	{
#ifdef _RUS
		ReplaceMMEngToRus(aStrandNameList[i]);
#endif
		m_cmbDgnDia.AddString(aStrandNameList[i]);
	}

	m_cmbDgnDia.SetCurSel(0);
}

void CCMTendonTypeAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialogMove::DoDataExchange(pDX);

	//DDX_Control(pDX, IDC_CMD_INADVERTENT_UNIT, m_wndInadvertentUnit);
	//DDX_Control(pDX, IDC_CMD_INADVERTENT_EDIT, m_wndInadvertent);
	DDX_Control(pDX, IDC_CMD_RELAX_JTG04_CMB, m_wndRelaxJTG04Cmb);
	DDX_Control(pDX, IDC_CMD_RELAX_EDIT, m_wndRelaxEdit);
	DDX_Control(pDX, IDC_CMD_RELAX_COMBO2, m_cobxRelaxMethod);
	DDX_Control(pDX, IDC_CMD_WOBBLE_UNIT, m_wndWobbleUnit);
	DDX_Control(pDX, IDC_CMD_WOBBLE_EDIT, m_wndWobble);

	DDX_Control(pDX, IDC_CMD_WOBBLE_EURO_UNIT,              m_wndWobbleEUROUnit);
	DDX_Control(pDX, IDC_CMD_WOBBLE_EURO_EDIT,              m_wndWobbleEURO);
	DDX_Control(pDX, IDC_CMD_WOBBLE_ANGLE_FACTOR_EURO_UNIT, m_wndWobbleAngleEUROUnit);
	DDX_Control(pDX, IDC_CMD_WOBBLE_ANGLE_FACTOR_EURO_EDT,  m_wndWobbleAngleEURO);
	DDX_Control(pDX, IDC_CMD_FRICTION_EDIT  ,               m_wndWobbleFriction);

	DDX_Control(pDX, IDC_CMD_RELAX_COMBO, m_wndRelaxCombo);
	DDX_Control(pDX, IDC_CMD_TPRST_ANCHO_END_ED, m_wndAnchoEnd);
	DDX_Control(pDX, IDC_CMD_TPRST_ANCHO_BEGIN_ED, m_wndAnchoBegin);
	DDX_Control(pDX, IDC_CMD_TPRST_ANCHO_BEGIN_UNIT, m_wndLengthUnit2);
	DDX_Control(pDX, IDC_CMD_TPRST_ANCHO_END_UNIT, m_wndLengthUnit1);
	DDX_Control(pDX, IDC_CMD_PRESS_UNIT3, m_PressUnit1);
	DDX_Control(pDX, IDC_CMD_PRESS_UNIT4, m_PressUnit2);
	DDX_Control(pDX, IDC_CMD_PRESS_UNIT5, m_PressUnit3); //
	DDX_Control(pDX, IDC_CMD_TDNT_FPK_UNIT, m_wndFpkUnit);  
	DDX_Control(pDX, IDC_CMD_TDNT_FPK_EDIT, m_wndFpkEdit);
	DDX_Control(pDX, IDC_CMD_DUCT_AREA_EDIT, m_wndDuctArea);
	DDX_Control(pDX, IDC_CMD_DESIGN_CODE_COMBO, m_cmbDgnCode);
	DDX_Control(pDX, IDC_CMD_DESIGN_DIA_COMBO, m_cmbDgnDia);
	DDX_Control(pDX, IDC_CMD_STRAND_NUM_EDIT, m_edtTendonNum);
	DDX_Text(pDX, IDC_CMD_STRAND_NUM_EDIT, m_strAreaNum);
	DDX_Control(pDX, IDC_CMD_TOTAL_TENDON_AREA_EDIT, m_wndTArea);
	DDX_Control(pDX, IDC_CMD_TENDON_TYPE_COMBO, m_wndTendonTypeCombo);
	DDX_Control(pDX, IDC_CMD_AREA_UNIT2, m_wndAUnit2);
	DDX_Control(pDX, IDC_CMD_AREA_UNIT, m_wndAUnit1);
	DDX_Control(pDX, IDC_CMD_MAT_EDIT, m_wndMatEdit);
	DDX_Control(pDX, IDC_CMD_MAT_COMBO, m_wndMatCombo);
	DDX_Control(pDX, IDC_CMD_TB05_EDT, m_edtXi);
	DDX_Text(pDX, IDC_CMD_DUCT_AREA_EDIT, m_strDuctArea);
	DDX_Text(pDX, IDC_CMD_FRICTION_EDIT, m_strFriction);
	DDX_Text(pDX, IDC_CMD_REL_COEF_EDIT, m_strRelCoef);
	DDX_Text(pDX, IDC_CMD_TENDON_NAME_EDIT, m_strTendonName);
	DDX_Text(pDX, IDC_CMD_TOTAL_TENDON_AREA_EDIT, m_strTotalTendonArea);
	DDX_Text(pDX, IDC_CMD_ULTIMATE_STRENGTH__EDIT, m_strUltimate);
	DDX_Text(pDX, IDC_CMD_YIELD_STRENGTH_EDIT, m_strYield);
	DDX_Text(pDX, IDC_CMD_YIELD_EXTERNAL_EDIT, m_strExternal);
	DDX_Check(pDX, IDC_CMD_RELAX_CHECK, m_bRelaxation);
	DDX_Radio(pDX, IDC_CMD_TPRST_BOND_TYPE1, m_nBondType);
	DDX_Control(pDX, IDC_CMD_STATIC11, m_wndDuctDiameter); ////// Duct Diameter  
	DDX_Check(pDX, IDC_CMD_OVERRED_FACTOR_CHECK, m_bOverStreRedFactor);
	DDX_Control(pDX, IDC_CMD_RELAX_USER_CMB, m_CobxRelax);
	DDX_Control(pDX, IDC_CMD_RELAX_CASE_COMBO, m_cobxRelaxCase);
	DDX_Control(pDX, IDC_CMD_RELAX_ABNT_NBR_CASE_COMBO, m_cobxABNTRelaxCase);
	DDX_Check(pDX, IDC_CMD_LOW_RELAX_CHK, m_bLowRelax);
}

BEGIN_MESSAGE_MAP(CCMTendonTypeAddDlg, CChildDialogMove)
	ON_BN_CLICKED(IDC_CMD_MAT_BUTTON, OnCmdMatButton)
	ON_BN_CLICKED(IDC_CMD_TAREA_BUTTON, OnCmdTareaButton)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_RELAX_CHECK, OnCmdRelaxCheck)
	ON_CBN_SELCHANGE(IDC_CMD_MAT_COMBO, OnSelchangeMatCombo)
	ON_CBN_SELCHANGE(IDC_CMD_RELAX_COMBO, OnSelchangeCmdRelaxCombo)
	ON_CBN_SELCHANGE(IDC_CMD_TENDON_TYPE_COMBO, OnSelchangeCmdTendonTypeCombo)
	ON_EN_CHANGE(IDC_CMD_TOTAL_TENDON_AREA_EDIT, OnChangeCmdTotalTendonAreaEdit)
	ON_CBN_SELCHANGE(IDC_CMD_RELAX_COMBO2, OnSelchangeCmdRelaxCombo2)
	ON_EN_CHANGE(IDC_CMD_MAT_EDIT, OnChangeCmdMatEdit)
	ON_BN_CLICKED(IDC_CMD_RELAX_USER_BTN, OnCmdRelaxUserBtn)
	ON_BN_CLICKED(IDC_CMD_WOBBLE_EURO_RDO,              OnCmdRelaxWobblrBtn)
	ON_BN_CLICKED(IDC_CMD_WOBBLE_ANGLE_FACTOR_EURO_RDO, OnCmdRelaxWobblrBtn)
	ON_EN_CHANGE(IDC_CMD_FRICTION_EDIT,                OnChangeCmdFrictionEdit)
	ON_EN_CHANGE(IDC_CMD_WOBBLE_EURO_EDIT,             OnChangeCmdWobbleEuroEdit)
	ON_EN_CHANGE(IDC_CMD_WOBBLE_ANGLE_FACTOR_EURO_EDT, OnChangeCmdWobbleAngleEuroEdit)
	ON_CBN_SELCHANGE(IDC_CMD_DESIGN_CODE_COMBO, OnSelChangeCmdDesignCode)
	ON_CBN_SELCHANGE(IDC_CMD_DESIGN_DIA_COMBO, OnSelChangeCmdDesignDia)
	ON_EN_CHANGE(IDC_CMD_STRAND_NUM_EDIT, OnChangeCmdTendonNum)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_TENDON_NUM_SPIN, OnDeltaposCmdTendonNumSpin)
	ON_BN_CLICKED(IDC_CMD_SET_LONG_TERM_STRESS_LOSS_BTN, OnCmdSetLongTermStressLossBtn)
	ON_CBN_SELCHANGE(IDC_CMD_RELAX_ABNT_NBR_CASE_COMBO, OnSelchangeCmdABNTRelaxTypeCombo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMTendonTypeAddDlg message handlers
BOOL CCMTendonTypeAddDlg::OnInitDialog() 
{
	CChildDialogMove::OnInitDialog();
	
	SetControl();

	m_nRelaxation = 0;
	m_bRelaxation = TRUE;
	m_bOverStreRedFactor = FALSE;
	m_bLowRelax = TRUE;

	ShowHideControl();

	AlignControls();

	SetData4Euro();
	SetDesignCode();
	SetDesignDia();

	Data2Dlg(); // 이 안에서 OnSelchangeCmdRelaxCombo2 호출
	
	RelaxCtrlMan();

	if (m_TdntKey == 0)   // Add
	{
		EnableControl(); // Tendon Type에 따라 Enable
		SetFieldData(); // Tendon Type에 따른 기본값 세팅
	}
	else                  // Modify
	{
		EnableControl();
	}
	OnSelchangeCmdTendonTypeCombo();
	CalcTendonArea();	
	return TRUE;
}

void CCMTendonTypeAddDlg::SetControl()
{
	SetUnitType();
	SetMatlCombo();
	SetTendonTypeCombo();
	SetTendonAreaCombo();
	SetRelaxCoeffCombo();
	SetRelaxEtcCombo();

	// for Eurocode
	m_IncCtrls1.RemoveAll();
	m_IncCtrls1.Add(IDC_STATIC_EXTERNAL);
	m_IncCtrls1.Add(IDC_CMD_YIELD_EXTERNAL_EDIT);
	m_IncCtrls1.Add(IDC_CMD_PRESS_UNIT5);

	m_IncCtrls2.RemoveAll();
	CDlgUtil::GetCtrlIDByIncRect(this, m_IncCtrls2, IDC_CMD_SLIP_STATIC, TRUE);

	m_IncCtrls3.RemoveAll();
	CDlgUtil::GetCtrlIDByIncRect(this, m_IncCtrls3, IDC_CMD_BOND_STATIC, TRUE);

	m_IncCtrls4.RemoveAll();
	m_IncCtrls4.Add(IDOK);
	m_IncCtrls4.Add(IDCANCEL);
	m_IncCtrls4.Add(IDC_CMD_APPLY);

	m_IncCtrls5.Add(IDC_CMD_SET_LONG_TERM_STRESS_LOSS_BTN);

	GetDlgItem(IDC_CMD_WOBBLE_EURO_RDO)->SetWindowText(_LS(IDS_CMD_TDNT_ITEM_Wobble_Friction_Factor));	
}

void CCMTendonTypeAddDlg::SetUnitType()
{
	m_wndAUnit1    .SetUnitType(D_UNITSYS_BASE_AREA    );
	m_wndTArea     .SetUnitType(D_UNITSYS_BASE_AREA    );
	m_wndAUnit2    .SetUnitType(D_UNITSYS_BASE_LENGTH  );
	m_wndDuctArea  .SetUnitType(D_UNITSYS_BASE_LENGTH  );
	m_PressUnit1   .SetUnitType(D_UNITSYS_BASE_PRESSURE);
	m_PressUnit2   .SetUnitType(D_UNITSYS_BASE_PRESSURE);
	m_PressUnit3   .SetUnitType(D_UNITSYS_BASE_PRESSURE); //
	m_wndAnchoBegin.SetUnitType(D_UNITSYS_BASE_LENGTH  );
	m_wndAnchoEnd  .SetUnitType(D_UNITSYS_BASE_LENGTH  );
	m_wndLengthUnit1.SetUnitType(D_UNITSYS_BASE_LENGTH  );
	m_wndLengthUnit2.SetUnitType(D_UNITSYS_BASE_LENGTH  );
	m_wndWobble             .SetUnitType(CUnitCtrl::m_TDNT_UNIT.dWobbleFactor);
	m_wndWobbleUnit         .SetUnitType(CUnitCtrl::m_TDNT_UNIT.dWobbleFactor);
	m_wndWobbleEURO         .SetUnitType(CUnitCtrl::m_TDNT_UNIT.dWobbleFactor);
	m_wndWobbleEUROUnit     .SetUnitType(CUnitCtrl::m_TDNT_UNIT.dWobbleFactor);
	m_wndWobbleAngleEURO    .SetUnitType(D_UNITSYS_BASE_RADIAN_L); 
	m_wndWobbleAngleEUROUnit.SetUnitType(D_UNITSYS_BASE_RADIAN_L); 
	m_wndFpkUnit.SetUnitType(D_UNITSYS_BASE_PRESSURE);
	//m_wndInadvertent.SetUnitType(CUnitCtrl::m_TDNT_UNIT.dInadvertentChange);
	//m_wndInadvertentUnit.SetUnitType(CUnitCtrl::m_TDNT_UNIT.dInadvertentChange);
	m_edtXi.SetUnitType(0);
}

void CCMTendonTypeAddDlg::SetMatlCombo()
{
	m_wndMatEdit .SetComboBoxPoint(&m_wndMatCombo);
	m_wndMatCombo.SetEditBoxPoint(&m_wndMatEdit  );

	// Material list는 Steel Type만 보이도록 한다.
	CArray<CString, CString> MatlType;
	MatlType.Add(_T("S"));
	MatlType.Add(_T("U"));
	m_wndMatCombo.SetMatlType(MatlType);
}

void CCMTendonTypeAddDlg::SetTendonTypeCombo()
{
	m_wndTendonTypeCombo.ResetContent();

	int nIndex = m_wndTendonTypeCombo.AddString(_LS(IDS_CMD_TDNT_ITEM_Internal_Pre));
	m_wndTendonTypeCombo.SetItemData(nIndex, 0);
	nIndex = m_wndTendonTypeCombo.AddString(_LS(IDS_CMD_TDNT_ITEM_Internal_Post));
	m_wndTendonTypeCombo.SetItemData(nIndex, 1);
	nIndex = m_wndTendonTypeCombo.AddString(_LS(IDS_CMD_TDNT_ITEM_External));
	m_wndTendonTypeCombo.SetItemData(nIndex, 2);
	
	m_wndTendonTypeCombo.SetCurSel(0);
	CDlgUtil::CobxAdjustListBoxWidth(m_wndTendonTypeCombo);
}

void CCMTendonTypeAddDlg::SetTendonAreaCombo()
{
	m_cmbDgnCode.ResetContent();
	m_cmbDgnDia.ResetContent();


	return;
}

void CCMTendonTypeAddDlg::SetRelaxCoeffCombo()
{
	m_cobxRelaxMethod.ResetContent();

	int nIndex = m_cobxRelaxMethod.AddString(_T("Magura"));
	m_cobxRelaxMethod.SetItemData(nIndex, TDNT_RELAX_MAGURA);
	nIndex = m_cobxRelaxMethod.AddString(_T("CEB-FIP 2010"));
	m_cobxRelaxMethod.SetItemData(nIndex, TDNT_RELAX_CEB_FIP_2010);
	nIndex = m_cobxRelaxMethod.AddString(_T("CEB-FIP 1990"));
	m_cobxRelaxMethod.SetItemData(nIndex, TDNT_RELAX_CEB_FIP_1990);
	nIndex = m_cobxRelaxMethod.AddString(_T("CEB-FIP 1978"));
	m_cobxRelaxMethod.SetItemData(nIndex, TDNT_RELAX_CEB_FIP);
	nIndex = m_cobxRelaxMethod.AddString(_T("JTG18/JTG04"));
	m_cobxRelaxMethod.SetItemData(nIndex, TDNT_RELAX_JTG04);
	nIndex = m_cobxRelaxMethod.AddString(_T("TB10092-17"));
	m_cobxRelaxMethod.SetItemData(nIndex, TDNT_RELAX_TB10092_17);
	nIndex = m_cobxRelaxMethod.AddString(_T("TB05"));
	m_cobxRelaxMethod.SetItemData(nIndex, TDNT_RELAX_TB05);
	nIndex = m_cobxRelaxMethod.AddString(_T("Q/CR 9300-18"));
	m_cobxRelaxMethod.SetItemData(nIndex, TDNT_RELAX_QCR9300_18);
	nIndex = m_cobxRelaxMethod.AddString(_T("JTJ023-85"));
	m_cobxRelaxMethod.SetItemData(nIndex, TDNT_RELAX_JTG023_85);

	// MNET:XXXX-JWKWON-20140212 : v831부터는 Gen에도 들어감
	nIndex = m_cobxRelaxMethod.AddString(_T("IRC:112-2011"));
	m_cobxRelaxMethod.SetItemData(nIndex, TDNT_RELAX_INDIA_IRC112_2011);
	nIndex = m_cobxRelaxMethod.AddString(_T("IRC:18-2000"));
	m_cobxRelaxMethod.SetItemData(nIndex, TDNT_RELAX_INDIA_IRC18_2000);
	nIndex = m_cobxRelaxMethod.AddString(_T("European"));
	m_cobxRelaxMethod.SetItemData(nIndex, TDNT_RELAX_EUROPEAN);

#if defined(_CIVIL)
	nIndex = m_cobxRelaxMethod.AddString(_T("KSCE-LSD15"));
	m_cobxRelaxMethod.SetItemData(nIndex, TDNT_RELAX_KSCE_LSD15);
#endif

	nIndex = m_cobxRelaxMethod.AddString(_T("AS 5100.5-2017"));
	m_cobxRelaxMethod.SetItemData(nIndex, TDNT_RELAX_AS_1500_5_2017);

	nIndex = m_cobxRelaxMethod.AddString(_T("ABNT NBR 6118:2023"));
	m_cobxRelaxMethod.SetItemData(nIndex, TDNT_RELAX_ABNT_NBR_6118_2023);

	nIndex = m_cobxRelaxMethod.AddString(_LS(IDS_CMD_TDNT_ITEM_User_Defined));
	m_cobxRelaxMethod.SetItemData(nIndex, TDNT_RELAX_USER_DEFINED);

#if defined(_RUS)	
	CDlgUtil::CobxSetCurSelItemData(m_cobxRelaxMethod, TDNT_RELAX_INDIA_IRC112_2011);
#elif defined(_CIVIL_ORG)
	CDlgUtil::CobxSetCurSelItemData(m_cobxRelaxMethod, TDNT_RELAX_KSCE_LSD15);
#else
	m_cobxRelaxMethod.SetCurSel(0);
#endif
	//}
}

void CCMTendonTypeAddDlg::SetRelaxEtcCombo()
{
	m_wndRelaxCombo.ResetContent();
	m_wndRelaxCombo.AddString(_T("10"));
	m_wndRelaxCombo.AddString(_T("45"));

	m_wndRelaxJTG04Cmb.ResetContent();
	m_wndRelaxJTG04Cmb.AddString(_T("1.0"));
	m_wndRelaxJTG04Cmb.AddString(_T("0.3"));

	m_CobxRelax.ChangeType(D_COBXTDMF_RELAXATION);

	m_cobxRelaxCase.ResetContent();
	int nIndex = m_cobxRelaxCase.AddString(_LS(IDS_CMD_TDNT_ITEM_Class_1_Slow));  m_cobxRelaxCase.SetItemData(nIndex, 1);
	nIndex = m_cobxRelaxCase.AddString(_LS(IDS_CMD_TDNT_ITEM_Class_2_Mean));      m_cobxRelaxCase.SetItemData(nIndex, 2);
	nIndex = m_cobxRelaxCase.AddString(_LS(IDS_CMD_TDNT_ITEM_Class_3_Rapid));     m_cobxRelaxCase.SetItemData(nIndex, 3);

	m_cobxABNTRelaxCase.ResetContent();
	nIndex = m_cobxABNTRelaxCase.AddString(_LS(IDS_CMD_TDNT_ITEM_Cords));  m_cobxABNTRelaxCase.SetItemData(nIndex, 1);
	nIndex = m_cobxABNTRelaxCase.AddString(_LS(IDS_CMD_TDNT_ITEM_Wires));  m_cobxABNTRelaxCase.SetItemData(nIndex, 2);
	nIndex = m_cobxABNTRelaxCase.AddString(_LS(IDS_CMD_TDNT_ITEM_Bars));   m_cobxABNTRelaxCase.SetItemData(nIndex, 3);
}

void CCMTendonTypeAddDlg::ShowHideControl()
{
	// MNET:XXXX-SHJUNG-20100407 - PMS4169 _T("External Cable Moment Magnifier") Hide
#if defined(_MGEN)
	if (CProduct::GetTestEnvValue(_T("PostTension")).CompareNoCase(_T("Yes")) == 0)
	{
		GetDlgItem(IDC_STATIC_EXTERNAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_YIELD_EXTERNAL_EDIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_PRESS_UNIT5)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_EXTERNAL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_YIELD_EXTERNAL_EDIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_PRESS_UNIT5)->ShowWindow(SW_SHOW);

		CRect rWindow, rOK;

		GetDlgItem(IDC_CMD_SET_LONG_TERM_STRESS_LOSS_BTN)->ShowWindow(SW_HIDE);

		CRect rStatic22, rLongTermBtn;
		GetDlgItem(IDC_CMD_STATIC22)->GetWindowRect(rStatic22);
		GetDlgItem(IDC_CMD_SET_LONG_TERM_STRESS_LOSS_BTN)->GetWindowRect(rLongTermBtn);

		int moveDistance = rLongTermBtn.top - rStatic22.bottom;

		ScreenToClient(rStatic22);
		rStatic22.bottom += moveDistance;
		GetDlgItem(IDC_CMD_STATIC22)->MoveWindow(rStatic22);

		CDlgUtil::CtrlMoveDistXY(this, m_IncCtrls4, 0, moveDistance);

		GetDlgItem(IDOK)->GetWindowRect(rOK);
		GetWindowRect(rWindow);
		rWindow.bottom = rOK.bottom + 15;
		MoveWindow(rWindow);
	}
#else
	GetDlgItem(IDC_STATIC_EXTERNAL)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CMD_YIELD_EXTERNAL_EDIT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CMD_PRESS_UNIT5)->ShowWindow(SW_SHOW);

	CRect rWindow, rOK;

	GetDlgItem(IDC_CMD_SET_LONG_TERM_STRESS_LOSS_BTN)->ShowWindow(SW_HIDE);

	CRect rStatic22, rLongTermBtn;
	GetDlgItem(IDC_CMD_STATIC22)->GetWindowRect(rStatic22);
	GetDlgItem(IDC_CMD_SET_LONG_TERM_STRESS_LOSS_BTN)->GetWindowRect(rLongTermBtn);

	int moveDistance = rLongTermBtn.top - rStatic22.bottom;

	ScreenToClient(rStatic22);
	rStatic22.bottom += moveDistance;
	GetDlgItem(IDC_CMD_STATIC22)->MoveWindow(rStatic22);

	CDlgUtil::CtrlMoveDistXY(this, m_IncCtrls4, 0, moveDistance);

	GetDlgItem(IDOK)->GetWindowRect(rOK);
	GetWindowRect(rWindow);
	rWindow.bottom = rOK.bottom + 15;
	MoveWindow(rWindow);
#endif
}

void CCMTendonTypeAddDlg::AlignControls()
{  
	CRect rRef, rToMove;
	int nDistX, nDistY;

	CArray<UINT, UINT> aCtrl;
	aCtrl.Add(IDC_CMD_RELAX_STC);
	aCtrl.Add(IDC_CMD_RELAX_EDIT);
	aCtrl.Add(IDC_CMD_RELAX_TEXT);
	UINT aFirstCtrlID1[] = {IDC_CMD_RELAX_STC};
	CArray<UINT, UINT> *aCtrlSet1[] = {&aCtrl};
	GetDlgItem(IDC_CMD_RELAX_COMBO)->GetWindowRect(rRef);
	for(int i = 0; i < sizeof(aFirstCtrlID1) / sizeof(UINT); i++)
	{
		GetDlgItem(aFirstCtrlID1[i])->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.top - rToMove.top + globalUtils.ScaleByDPI(5);
		CDlgUtil::CtrlMoveDistXY(this, *aCtrlSet1[i], nDistX, nDistY);
	}

	CArray<UINT, UINT> aCtrlRelaxJTG04;
	aCtrlRelaxJTG04.Add(IDC_CMD_RELAX_JTG04_CMB);
	UINT aFirstCtrlID11[] = {IDC_CMD_RELAX_JTG04_CMB};
	CArray<UINT, UINT> *aCtrlSet11[] = {&aCtrlRelaxJTG04};
	GetDlgItem(IDC_CMD_RELAX_COMBO)->GetWindowRect(rRef);
	for(int i = 0; i < sizeof(aFirstCtrlID11) / sizeof(UINT); i++)
	{
		GetDlgItem(aFirstCtrlID11[i])->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistXY(this, *aCtrlSet11[i], nDistX, nDistY);
	}

	// TB05
	GetDlgItem(IDC_CMD_TB05_STC)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top + globalUtils.ScaleByDPI(2);
	CArray<UINT, UINT> arTmp1; arTmp1.Add(IDC_CMD_TB05_STC);
	CArray<UINT, UINT> arTmp2; arTmp2.Add(IDC_CMD_TB05_EDT);
	CDlgUtil::CtrlMoveDistXY(this, arTmp1, nDistX, nDistY);
	CDlgUtil::CtrlMoveDistXY(this, arTmp2, nDistX, nDistY);

	aCtrl.RemoveAll();
	aCtrl.Add(IDC_CMD_OVERRED_FACTOR_CHECK);
	aCtrl.Add(IDC_CMD_TDNT_FPK_STATIC);
	aCtrl.Add(IDC_CMD_TDNT_FPK_EDIT);
	aCtrl.Add(IDC_CMD_TDNT_FPK_UNIT);
	UINT aFirstCtrlID2[] = {IDC_CMD_TDNT_FPK_EDIT};
	CArray<UINT, UINT> *aCtrlSet2[] = {&aCtrl};
	GetDlgItem(IDC_CMD_YIELD_STRENGTH_EDIT)->GetWindowRect(rRef);
	for(int i = 0; i < sizeof(aFirstCtrlID2) / sizeof(UINT); i++)
	{
		GetDlgItem(aFirstCtrlID2[i])->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistXY(this, *aCtrlSet2[i], nDistX, nDistY);
	}

	// CEB-FIP 2010에마 들어가는 Class 선택창의 위치
	CArray<UINT, UINT> aCtrlCEBFIP2010;
	aCtrlCEBFIP2010.Add(IDC_CMD_RELAX_CASE_COMBO);
	GetDlgItem(IDC_CMD_RELAX_USER_CMB)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_RELAX_CASE_COMBO)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, aCtrlCEBFIP2010, nDistX, nDistY);


	CArray<UINT, UINT> aCtrlABNT;
	aCtrlABNT.Add(IDC_CMD_RELAX_ABNT_NBR_CASE_COMBO);
	GetDlgItem(IDC_CMD_RELAX_CASE_COMBO)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_RELAX_ABNT_NBR_CASE_COMBO)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, aCtrlABNT, nDistX, nDistY);

	CArray<UINT, UINT> aCtrlLowRelax;
	aCtrlLowRelax.Add(IDC_CMD_LOW_RELAX_CHK);
	GetDlgItem(IDC_CMD_RELAX_USER_CMB)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_LOW_RELAX_CHK)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, aCtrlLowRelax, nDistX, nDistY);

	//GetDlgItem(m_aCtrlTypeInternal[0])->GetWindowRect(rToMove);
	//GetDlgItem(m_aCtrlTypeExternal[0])->GetWindowRect(rRef);
	//nDistX = rToMove.left -rRef.left;
	//nDistY = rToMove.top - rRef.top;
	//CDlgUtil::CtrlMoveDistXY(this, m_aCtrlTypeExternal, nDistX, nDistY);

	// Euro, FIP, AS
	GetDlgItem(IDC_CMD_WOBBLE_EDIT)->GetWindowRect(rRef);     
	GetDlgItem(IDC_CMD_WOBBLE_EURO_EDIT)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aWoodleEuro, 0, nDistY);

	// resize self size
	CRect rWindow, rOK;
	GetDlgItem(IDOK)->GetWindowRect(rOK);
	GetWindowRect(rWindow);
	rWindow.bottom = rOK.bottom + globalUtils.ScaleByDPI(15);
	MoveWindow(rWindow);
}

void CCMTendonTypeAddDlg::Data2Dlg()
{
	UpdateData(TRUE);

	m_wndMatCombo.ChangeSelect(m_TdntData.MatlKey);
	m_wndMatCombo.SetEditBoxPoint(&m_wndMatEdit);
	
	CString StrTemp;
	StrTemp.Format(_T("%d"),m_TdntData.MatlKey);
	m_wndMatEdit.SetWindowText(StrTemp);
	
	m_wndDuctArea.SetEditUnit(m_TdntData.dDuctArea);
	m_wndTArea.SetEditUnit(m_TdntData.dTendonArea);
	m_wndAnchoBegin.SetEditUnit(m_TdntData.dAnchoSlipBegin);
	m_wndAnchoEnd  .SetEditUnit(m_TdntData.dAnchoSlipEnd  );

	m_wndWobble.SetEditUnit(m_TdntData.dWobbleFactor);

	CDlgUtil::CtrlRadioSetCheck(this, m_aWobbleType, m_TdntData.nWobbleType);
	m_wndWobbleEURO.SetEditUnit(m_TdntData.dWobbleFactor);
	m_wndWobbleAngleEURO.SetEditUnit(m_TdntData.dWobbleAngle);
	m_wndWobbleFriction.SetEditUnit(m_TdntData.dFrictionFactor);

	//m_wndInadvertent.SetEditUnit(m_TdntData.dInadvertentChange);

	m_nBondType = m_TdntData.nBondType;
	
	int nSel=0;
	if(m_TdntData.nTendonType==1)
	{
		if     (m_TdntData.nLoadType==0) nSel = 0;
		else if(m_TdntData.nLoadType==1) nSel = 1;
	}
	else nSel = 2;
	m_wndTendonTypeCombo.SetCurSel(nSel);
	
	m_nRelaxation = 0;
	if (m_TdntData.nRelaxMethod == TDNT_RELAX_MAGURA) // m_TdntData.nRelaxMethod - 콤보박스 값
	{
		if (m_TdntData.dRelaxCoef == 0.)
		{
			m_bRelaxation = FALSE;
		}
		else if(m_TdntData.dRelaxCoef == 10.)
		{
			m_bRelaxation = TRUE;
			m_nRelaxation = 0;
		}
		else if(m_TdntData.dRelaxCoef == 45.)
		{
			m_bRelaxation = TRUE;
			m_nRelaxation = 1;
		}
	}
	else if (m_TdntData.nRelaxMethod == TDNT_RELAX_CEB_FIP) 
	{
		m_bRelaxation = (m_TdntData.dRelaxRatio == 0.) ? FALSE : TRUE;
	}
	else if(m_TdntData.nRelaxMethod == TDNT_RELAX_JTG04) 
	{
		m_bRelaxation = (m_TdntData.dJTG04Factor == 0.)? FALSE : TRUE;
	}    
	else if (m_TdntData.nRelaxMethod == TDNT_RELAX_JTG023_85)
	{
		m_bRelaxation = (m_TdntData.dJTG04Factor == 0.) ? FALSE : TRUE;
	}
	else if(m_TdntData.nRelaxMethod == TDNT_RELAX_TB05)  // TB05 : dJTG04Factor 값(==0)으로 적용 여부를 판별
	{
		m_bRelaxation = (m_TdntData.dJTG04Factor == 0.)? FALSE : TRUE;
	}
	else if(m_TdntData.nRelaxMethod == TDNT_RELAX_INDIA_IRC18_2000) //INDIA(IRC:18-2000)  dRelaxCoef 값(==0)으로 적용 여부를 판별
	{ 
		m_bRelaxation = (m_TdntData.dRelaxCoef == 0.) ? FALSE : TRUE;
	}
	else if(m_TdntData.nRelaxMethod == TDNT_RELAX_EUROPEAN) // European  dRelaxCoef 값(==0)으로 적용 여부를 판별
	{
		m_bRelaxation = (m_TdntData.dRelaxCoef == 0.) ? FALSE : TRUE;
	}
	else if(m_TdntData.nRelaxMethod == TDNT_RELAX_KSCE_LSD15) // KSCE-LSD15 dRelaxCoef 값(==0)으로 적용 여부를 판별
	{
		m_bRelaxation = (m_TdntData.dRelaxCoef == 0.) ? FALSE : TRUE;
	}
	else if(m_TdntData.nRelaxMethod == TDNT_RELAX_INDIA_IRC112_2011) // KSCE-LSD15 dRelaxCoef 값(==0)으로 적용 여부를 판별
	{
		m_bRelaxation = (m_TdntData.dRelaxCoef == 0.) ? FALSE : TRUE;
	}
	else if (m_TdntData.nRelaxMethod == TDNT_RELAX_CEB_FIP_1990) 
	{
		m_bRelaxation = (m_TdntData.dRelaxRatio == 0.) ? FALSE : TRUE;
	}
	else if (m_TdntData.nRelaxMethod == TDNT_RELAX_CEB_FIP_2010) 
	{
		m_bRelaxation = (m_TdntData.dRelaxRatio == 0.) ? FALSE : TRUE;
	}
	else if(m_TdntData.nRelaxMethod == TDNT_RELAX_TB10092_17)  // TB10092-17 : dJTG04Factor 값(==0)으로 적용 여부를 판별
	{
		m_bRelaxation = (m_TdntData.dJTG04Factor == 0.)? FALSE : TRUE;
	}
	else if(m_TdntData.nRelaxMethod == TDNT_RELAX_QCR9300_18) 
	{
		m_bRelaxation = (m_TdntData.dJTG04Factor == 0.)? FALSE : TRUE;
	}
	else if(m_TdntData.nRelaxMethod == TDNT_RELAX_AS_1500_5_2017) 
	{
		m_bRelaxation = (m_TdntData.dJTG04Factor == 0.)? FALSE : TRUE;
	}
	else if (m_TdntData.nRelaxMethod == TDNT_RELAX_ABNT_NBR_6118_2023)
	{
		m_bRelaxation = (m_TdntData.dRelaxCoef == 0.) ? FALSE : TRUE;
	}
	else if(m_TdntData.nRelaxMethod == TDNT_RELAX_USER_DEFINED)
	{
		m_bRelaxation = m_TdntData.TdmfKey > 0 ? TRUE : FALSE;
	}	
	else
		ASSERT(0);

	if (m_TdntKey == 0) ChangeMaterial();   // Add
	else    // Modify
	{
		int nMethodNum = m_cobxRelaxMethod.GetCount();
		int i = 0;
		for (i=0; i<nMethodNum; i++)
			if (m_TdntData.nRelaxMethod == m_cobxRelaxMethod.GetItemData(i)) break;
		if (i == nMethodNum) ASSERT(0);

		m_cobxRelaxMethod.SetCurSel(i);
		OnSelchangeCmdRelaxCombo2(); // 여기서 타입에 따라 대화창 커스터마이즈

		CString csRelaxCoef;
		csRelaxCoef.Format(_T("%.6g"), m_TdntData.dRelaxRatio);
		m_wndRelaxEdit.SetWindowText(csRelaxCoef);

		m_strRelCoef.Format(_T("%.6g"),m_TdntData.dRelaxCoef);

		if (m_TdntData.nRelaxMethod == TDNT_RELAX_JTG04)
		{
			/*
			CString strRelaxCoefJTG04;
			strRelaxCoefJTG04.Format(_T("%.6g"), m_TdntData.dJTG04Factor);
			m_wndRelaxJTG04Cmb.SetWindowText(strRelaxCoefJTG04);
			*/
			if(m_TdntData.dJTG04Factor==1.0) m_wndRelaxJTG04Cmb.SetCurSel(0);
			if(m_TdntData.dJTG04Factor==0.3) m_wndRelaxJTG04Cmb.SetCurSel(1);
		}
		else if (m_TdntData.nRelaxMethod == TDNT_RELAX_TB05 ||
			     m_TdntData.nRelaxMethod == TDNT_RELAX_TB10092_17 ||
				 m_TdntData.nRelaxMethod == TDNT_RELAX_QCR9300_18 ||
				 m_TdntData.nRelaxMethod == TDNT_RELAX_JTG023_85)
		{
			CString csTmp;
			m_edtXi.GetWindowText(csTmp);
			if (csTmp != _LSX(Auto)) // 자동 계산이 아닌 경우
				m_edtXi.SetEditUnit(m_TdntData.dJTG04Factor);
		}
		else if (m_TdntData.nRelaxMethod == TDNT_RELAX_AS_1500_5_2017)
		{
			m_edtXi.SetEditUnit(m_TdntData.dJTG04Factor);
		}
		else if (m_TdntData.nRelaxMethod == TDNT_RELAX_INDIA_IRC18_2000)
		{
			if (m_TdntData.dRelaxCoef==1.0) m_wndRelaxJTG04Cmb.SetCurSel(0);
			else if(m_TdntData.dRelaxCoef==2.0) m_wndRelaxJTG04Cmb.SetCurSel(1);
		}
		else if (m_TdntData.nRelaxMethod == TDNT_RELAX_EUROPEAN)
		{
			if      (m_TdntData.dRelaxCoef == 1.0) m_wndRelaxJTG04Cmb.SetCurSel(0);
			else if (m_TdntData.dRelaxCoef == 2.0) m_wndRelaxJTG04Cmb.SetCurSel(1);
			else if (m_TdntData.dRelaxCoef == 3.0) m_wndRelaxJTG04Cmb.SetCurSel(2);
		}
		else if (m_TdntData.nRelaxMethod == TDNT_RELAX_KSCE_LSD15)
		{
			if      (m_TdntData.dRelaxCoef == 1.0) m_wndRelaxJTG04Cmb.SetCurSel(0);
			else if (m_TdntData.dRelaxCoef == 2.0) m_wndRelaxJTG04Cmb.SetCurSel(1);
			else if (m_TdntData.dRelaxCoef == 3.0) m_wndRelaxJTG04Cmb.SetCurSel(2);
		}
		else if (m_TdntData.nRelaxMethod == TDNT_RELAX_INDIA_IRC112_2011)
		{
			if (m_TdntData.dRelaxCoef==1.0) m_wndRelaxJTG04Cmb.SetCurSel(0);
			else if(m_TdntData.dRelaxCoef==2.0) m_wndRelaxJTG04Cmb.SetCurSel(1);
		}
		else if(m_TdntData.nRelaxMethod == TDNT_RELAX_CEB_FIP_2010)
		{
			int nCnt =  m_cobxRelaxCase.GetCount();
			for(int i = 0 ; i < nCnt; i++)
			{
				T_TDMF_K TdmfK = (T_TDMF_K)m_cobxRelaxCase.GetItemData(i);
				if(TdmfK == m_TdntData.TdmfKey)
				{
					m_cobxRelaxCase.SetCurSel(i);
					break;
				}
			}

//       m_cobxRelaxCase.GetItem()
//       int nIndex2 = m_cobxRelaxCase.GetCurSel();
//       if (nIndex2 == CB_ERR) m_TdntData.TdmfKey=0;
//       else m_TdntData.TdmfKey = m_cobxRelaxCase.GetItemData(nIndex2);

		}
		else if (m_TdntData.nRelaxMethod == TDNT_RELAX_ABNT_NBR_6118_2023)
		{

			if (m_TdntData.dRelaxCoef == 1.0) 
				m_wndRelaxJTG04Cmb.SetCurSel(0);
			else if (m_TdntData.dRelaxCoef == 2.0) 
				m_wndRelaxJTG04Cmb.SetCurSel(1);
			else
				m_wndRelaxJTG04Cmb.SetCurSel(0);

			int nCnt = m_cobxABNTRelaxCase.GetCount();
			for (int i = 0; i < nCnt; i++)
			{
				T_TDMF_K TdmfK = (T_TDMF_K)m_cobxABNTRelaxCase.GetItemData(i);
				if (TdmfK == m_TdntData.TdmfKey)
				{
					m_cobxABNTRelaxCase.SetCurSel(i);
					break;
				}
			}
			OnSelchangeCmdABNTRelaxTypeCombo();
		}
		else if (m_TdntData.nRelaxMethod == TDNT_RELAX_USER_DEFINED)
		{
			m_CobxRelax.ChangeSelect( m_TdntData.TdmfKey );
		}

		// 이건 왜 빼먹었는지 ㅡㅡ;;;;; 대체 왜이렇게 한거야!!!!
		m_strDuctArea.Format(_T("%.10g"), m_TdntData.dDuctArea);
		m_strTotalTendonArea.Format(_T("%.10g"), m_TdntData.dTendonArea);
		//
	}

	m_bOverStreRedFactor = m_TdntData.bOverStressRedFactor;
	m_wndFpkEdit.SetEditUnit(m_TdntData.dFpk);
	m_strFriction.Format(_T("%.6g"),m_TdntData.dFrictionFactor);
	m_strTendonName = m_TdntData.TendonTypeName;

	m_wndTArea.SetEditUnit(m_TdntData.dTendonArea);//Total Tendon Area
	m_wndDuctArea.SetEditUnit(m_TdntData.dDuctArea);// Duct Diameter
	m_wndWobble.SetEditUnit(m_TdntData.dWobbleFactor);
	//m_wndInadvertent.SetEditUnit(m_TdntData.dInadvertentChange);
	m_strUltimate.Format(_T("%.6g"),m_TdntData.dUltimateStrength);
	m_strYield.Format(_T("%.6g"),m_TdntData.dYieldStrength);
	m_strExternal.Format(_T("%.6g"),m_TdntData.dAlphaExtCable); // 640 추가 
	m_wndAnchoBegin.SetEditUnit(m_TdntData.dAnchoSlipBegin); 
	m_wndAnchoEnd  .SetEditUnit(m_TdntData.dAnchoSlipEnd  );
	m_bLowRelax = m_TdntData.bLowRelax;

	if (!m_TdntData.strDgnCode.IsEmpty())
	{
		int nIndex = -1;
		for (int i = 0; i < m_cmbDgnCode.GetCount(); i++)
		{
			CString strTemp;
			m_cmbDgnCode.GetLBText(i, strTemp);
			if (strTemp.CompareNoCase(m_TdntData.strDgnCode) == 0)
			{
				nIndex = i;
				break;
			}
		}

		if (nIndex >= 0)
		{
			m_cmbDgnCode.SetCurSel(nIndex);
		}
		else if (m_cmbDgnCode.GetCount() > 0)
		{
			m_cmbDgnCode.SetCurSel(0);
		}

		SetDesignDia();
	}

	if (m_TdntData.nStrandDia >= 0)
	{
		m_cmbDgnDia.SetCurSel(m_TdntData.nStrandDia);
	}
	else
	{
		ASSERT(0);
		m_cmbDgnDia.SetCurSel(0);
	}

	if (m_TdntData.nStrandNum > 0)
	{
		m_strAreaNum.Format(_T("%d"), m_TdntData.nStrandNum);
	}
	else
	{
		m_strAreaNum = _T("1");
	}

	UpdateData(FALSE);
}

void CCMTendonTypeAddDlg::RelaxCtrlMan()
{
	m_wndRelaxCombo.SetCurSel(this->m_nRelaxation);
	m_wndRelaxCombo.EnableWindow(this->m_bRelaxation);
	m_cobxRelaxMethod.EnableWindow(this->m_bRelaxation);
	m_wndRelaxEdit.EnableWindow(this->m_bRelaxation);  
	m_wndRelaxJTG04Cmb.EnableWindow(this->m_bRelaxation);


	//GetDlgItem(IDC_CMD_TB05_STC)->ShowWindow(this->m_bRelaxation);
	//GetDlgItem(IDC_CMD_TB05_EDT)->ShowWindow(this->m_bRelaxation);

	EnableRelaxUserDefined();
}

void CCMTendonTypeAddDlg::CalcTendonArea()
{
	int nCodeSel = m_cmbDgnCode.GetCurSel();
	if (nCodeSel == CB_ERR)	return;

	CString strCode;
	m_cmbDgnCode.GetLBText(nCodeSel, strCode);

	EnableControlsByDgnCode(strCode);
	if (strCode == _LS(IDS_CMD_TendonTypeAdd_User)) return;

	int nDiaSel = m_cmbDgnDia.GetCurSel();
	if (nDiaSel == CB_ERR) return;

	CString strDia;
	m_cmbDgnDia.GetLBText(nDiaSel, strDia);
#ifdef _RUS
	ReplaceMMRusToEng(strDia);
#endif

	int nNumStrands = 0;
	if (!CStrParser::GetINumber(m_strAreaNum, nNumStrands))
	{
		nNumStrands = 1;
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc)
	{
		ASSERT(0); return;
	}

	T_MATL_STEEL strandData;
	if (pDoc->m_pMatlDB->GetStrandData(strCode, strDia, strandData))
	{
		double dStrandArea = pDoc->m_pMatlDB->Get_StrandArea(strCode, strDia);
		double dArea = dStrandArea * nNumStrands;

		double dArea_CurrentUnit = pDoc->m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_AREA, dArea);

		m_strTotalTendonArea.Format(_T("%.10g"), dArea_CurrentUnit);
		m_wndTArea.SetEditUnit(dArea_CurrentUnit);

		int nSel = m_wndTendonTypeCombo.GetCurSel();
		if (nSel == 0) SetDiameterField();

		UpdateData(FALSE);
	}
}

void CCMTendonTypeAddDlg::EnableControlsByDgnCode(const CString& strCode)
{
	if (strCode == _LS(IDS_CMD_TendonTypeAdd_User))
	{
		m_wndTArea.EnableWindow(TRUE);
		m_cmbDgnDia.EnableWindow(FALSE);
		m_edtTendonNum.EnableWindow(FALSE);
	}
	else
	{
		m_wndTArea.EnableWindow(FALSE);
		m_cmbDgnDia.EnableWindow(TRUE);
		m_edtTendonNum.EnableWindow(TRUE);
	}
}

void CCMTendonTypeAddDlg::EnableRelaxUserDefined() 
{
	int nIndex = m_cobxRelaxMethod.GetCurSel();
	if (nIndex == CB_ERR) return;
	DWORD dwRelax = m_cobxRelaxMethod.GetItemData(nIndex);

	CArray<UINT,UINT> aUserDefined;
	aUserDefined.Add( IDC_CMD_RELAX_USER_NAME_STC );
	aUserDefined.Add( IDC_CMD_RELAX_USER_CMB );
	aUserDefined.Add( IDC_CMD_RELAX_USER_BTN );

	// 우선 CEB-FIP 2010에 관한 Setting을 먼전한다.
	CArray<UINT,UINT> aCEBFIP2010;
	aCEBFIP2010.Add(IDC_CMD_RELAX_CASE_COMBO);


	CArray<UINT, UINT> aABNT;
	aABNT.Add(IDC_CMD_RELAX_ABNT_NBR_CASE_COMBO);
	
	CArray<UINT, UINT> aLowRelax;
	aLowRelax.Add(IDC_CMD_LOW_RELAX_CHK);

	BOOL bABNT = FALSE;
	BOOL bUser = FALSE;
	BOOL bRelaxCase = FALSE;
	BOOL bLowRelax = FALSE;
	
	if     (dwRelax == TDNT_RELAX_CEB_FIP_2010)		{ bUser = FALSE; bRelaxCase = TRUE;  bLowRelax = FALSE; bABNT=FALSE;}
	else if(dwRelax == TDNT_RELAX_TB05)				{ bUser = FALSE; bRelaxCase = FALSE; bLowRelax = TRUE; bABNT=FALSE;  }
	else if(dwRelax == TDNT_RELAX_TB10092_17)		{ bUser = FALSE; bRelaxCase = FALSE; bLowRelax = TRUE; bABNT=FALSE;  }
	else if(dwRelax == TDNT_RELAX_QCR9300_18)		{ bUser = FALSE; bRelaxCase = FALSE; bLowRelax = TRUE; bABNT=FALSE;  }
	else if(dwRelax == TDNT_RELAX_JTG023_85)		{ bUser = FALSE; bRelaxCase = FALSE; bLowRelax = FALSE; bABNT=FALSE;  }
	else if(dwRelax == TDNT_RELAX_ABNT_NBR_6118_2023) { bUser = FALSE; bRelaxCase = FALSE; bLowRelax = FALSE; bABNT=TRUE; }
	else											{ bUser = TRUE;  bRelaxCase = FALSE; bLowRelax = FALSE; }

	CDlgUtil::CtrlEnableDisable(this, aCEBFIP2010, m_bRelaxation && !bABNT);
	CDlgUtil::CtrlEnableDisable(this, aABNT, m_bRelaxation && bABNT);
	
	CDlgUtil::CtrlShowHide(this, aUserDefined, bUser);
	CDlgUtil::CtrlShowHide(this, aCEBFIP2010,  bRelaxCase && !bABNT);
	CDlgUtil::CtrlShowHide(this, aABNT, bABNT);
	CDlgUtil::CtrlShowHide(this, aLowRelax,    bLowRelax);

	// User Define에 대한 Setting을 함
	BOOL bUserDefined = m_bRelaxation && ( dwRelax == TDNT_RELAX_USER_DEFINED );
	CDlgUtil::CtrlEnableDisable(this, aUserDefined, bUserDefined);
}

void CCMTendonTypeAddDlg::OnCmdMatButton() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,	MAKEWPARAM(ID_STRUCT_PROP_MATERIAL,0));
}

void CCMTendonTypeAddDlg::OnCmdTareaButton() 
{
	T_MATL_K MatlKey = 0;
	m_wndMatEdit.GetKey(MatlKey);

	CCMDTendonAreaDlg Dlg;
	Dlg.MakeTendonAreaDB(MatlKey);
	if (IDOK == Dlg.DoModal())
	{
		UpdateData(TRUE); // 기존값 잃어버리지 않도록 저장
		T_TENDON_AREA TendonArea;
		if (!Dlg.GetTendonAreaData(TendonArea))
		{ ASSERT(0); TendonArea.Initialize(); }

		// area
		m_wndTArea.SetEditUnit(TendonArea.dArea);
		m_strTotalTendonArea.Format(_T("%.10g"), TendonArea.dArea);

		// diameter
		int nSel=m_wndTendonTypeCombo.GetCurSel();
//    if (nSel == 1 || nSel == 0) SetDiameterField();  // 2004.06.11 금창준 차장님 지시로 Internal(Pre-Tension)인 경우 자동계산 않음
		if (nSel == 0) SetDiameterField();

		if (TendonArea.dY_Strength != 0.0 && TendonArea.dU_Strength != 0.0)
		{
			// relaxation
			CString csRelax;
			csRelax.Format(_T("%.10g"), TendonArea.dR_Factor);
			m_wndRelaxEdit.SetWindowText(csRelax);
			// curvature
			m_strFriction.Format(_T("%.10g"), TendonArea.dC_Factor);
			// wobble
			m_wndWobble.SetEditUnit(TendonArea.dW_Factor);
			// ultimate
			m_strUltimate.Format(_T("%.10g"), TendonArea.dU_Strength);
			// yield
			m_strYield.Format(_T("%.10g"), TendonArea.dY_Strength);
			// anchorage slip
			m_wndAnchoBegin.SetEditUnit(TendonArea.dAnchorageSlip);
			m_wndAnchoEnd.SetEditUnit(TendonArea.dAnchorageSlip);
		}

		UpdateData(FALSE);
	} 
}

void CCMTendonTypeAddDlg::SetTDNTD(T_TDNT_K TdntK) // (0) Add (1) Modify
{
	m_TdntKey = TdntK;
	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdnt(m_TdntKey,m_TdntData))
	{
		CDBDoc::GetDocPoint()->m_pAttrCtrl->InitializeTdnt(m_TdntData);
	}
}

BOOL CCMTendonTypeAddDlg::ExecuteDlg()
{
	UpdateData(TRUE);
	m_bIsDBError = FALSE;
	m_nRelaxation = this->m_wndRelaxCombo.GetCurSel();

	if(!CStrParser::GetFloatNumber(m_strDuctArea,m_TdntData.dDuctArea)) return FALSE;
	if(!CStrParser::GetFloatNumber(m_strTotalTendonArea,m_TdntData.dTendonArea)) return FALSE;
	if(!CStrParser::GetFloatNumber(m_strFriction,m_TdntData.dFrictionFactor)) return FALSE;
	if(!CStrParser::GetFloatNumber(m_strRelCoef,m_TdntData.dRelaxCoef)) return FALSE;

	int nDgnCodeSel = m_cmbDgnCode.GetCurSel();
	if (nDgnCodeSel != CB_ERR)
	{
		m_cmbDgnCode.GetLBText(nDgnCodeSel, m_TdntData.strDgnCode);
	}
	else
	{
		m_TdntData.strDgnCode.Empty();
	}

	m_TdntData.nStrandDia = m_cmbDgnDia.GetCurSel();

	int nStrandNum = 1;
	if (!CStrParser::GetINumber(m_strAreaNum, nStrandNum)) nStrandNum = 1;

	m_TdntData.nStrandNum = nStrandNum;

	int nIndex = m_cobxRelaxMethod.GetCurSel();
	if (nIndex == CB_ERR) return FALSE;
	m_TdntData.nRelaxMethod = m_cobxRelaxMethod.GetItemData(nIndex);
 
	if (this->m_bRelaxation)
	{
		if (m_TdntData.nRelaxMethod == TDNT_RELAX_MAGURA)
		{
			if(m_nRelaxation == 0) m_TdntData.dRelaxCoef = 10.;
			else m_TdntData.dRelaxCoef = 45.;
		}
		else if (m_TdntData.nRelaxMethod == TDNT_RELAX_CEB_FIP)
		{
			CString csRelaxCoef;
			m_wndRelaxEdit.GetWindowText(csRelaxCoef);
			m_TdntData.dRelaxRatio = _tstof(csRelaxCoef);
		}
		else if(m_TdntData.nRelaxMethod == TDNT_RELAX_JTG04)
		{
			CString csRelaxCoef;
			m_wndRelaxJTG04Cmb.GetWindowText(csRelaxCoef);
			m_TdntData.dJTG04Factor = _tstof(csRelaxCoef);
		}
		else if (m_TdntData.nRelaxMethod == TDNT_RELAX_JTG023_85)
		{
			double dTmp = m_edtXi.GetEditValue();
			if (dTmp <= 0.0) return FALSE;
			m_TdntData.dJTG04Factor = dTmp;
		}
		else if (m_TdntData.nRelaxMethod == TDNT_RELAX_TB05 ||
			     m_TdntData.nRelaxMethod == TDNT_RELAX_TB10092_17 ||
				 m_TdntData.nRelaxMethod == TDNT_RELAX_QCR9300_18)
		{
			// 밑에서 MatlKey, bOverStreRedFactor 입력받은 후 처리
		}
		else if(m_TdntData.nRelaxMethod == TDNT_RELAX_AS_1500_5_2017)
		{
			if (this->m_bRelaxation == FALSE) // Check Off
			{
				m_TdntData.dJTG04Factor = 0.0;
			}
			else // Check On
			{
				double dTmp = m_edtXi.GetEditValue();
				if (dTmp <= 0.0) return FALSE;
				m_TdntData.dJTG04Factor = dTmp;
			}
		}
		else if (m_TdntData.nRelaxMethod == TDNT_RELAX_INDIA_IRC18_2000)
		{
			if(m_wndRelaxJTG04Cmb.GetCurSel()==0) // Normal
				m_TdntData.dRelaxCoef = 1.0;
			else
				m_TdntData.dRelaxCoef = 2.0; // Low
			// 체크박스 체크 Off 이면 밑에서 0.0 (OFF)
		}
		else if (m_TdntData.nRelaxMethod == TDNT_RELAX_EUROPEAN)
		{
			if(m_wndRelaxJTG04Cmb.GetCurSel() == 0) //Ordinary
			{
				m_TdntData.dRelaxCoef = 1.0;
			}
			else if (m_wndRelaxJTG04Cmb.GetCurSel() == 1) //Low
			{
				m_TdntData.dRelaxCoef = 2.0;
			}
			else // Hot rolled
			{
				m_TdntData.dRelaxCoef = 3.0;
			}
		}
		else if (m_TdntData.nRelaxMethod == TDNT_RELAX_KSCE_LSD15)
		{
			if(m_wndRelaxJTG04Cmb.GetCurSel() == 0) //Ordinary
			{
				m_TdntData.dRelaxCoef = 1.0;
			}
			else if (m_wndRelaxJTG04Cmb.GetCurSel() == 1) //Low
			{
				m_TdntData.dRelaxCoef = 2.0;
			}
			else // Hot rolled
			{
				m_TdntData.dRelaxCoef = 3.0;
			}
		}
		else if (m_TdntData.nRelaxMethod == TDNT_RELAX_INDIA_IRC112_2011)
		{
			if(m_wndRelaxJTG04Cmb.GetCurSel()==0) // Normal
				m_TdntData.dRelaxCoef = 1.0;
			else
				m_TdntData.dRelaxCoef = 2.0; // Low
		}
		else if (m_TdntData.nRelaxMethod == TDNT_RELAX_CEB_FIP_1990)
		{
			CString csRelaxCoef;
			m_wndRelaxEdit.GetWindowText(csRelaxCoef);
			m_TdntData.dRelaxRatio = _tstof(csRelaxCoef);
		}
		else if (m_TdntData.nRelaxMethod == TDNT_RELAX_CEB_FIP_2010)
		{
			CString csRelaxCoef;
			m_wndRelaxEdit.GetWindowText(csRelaxCoef);
			m_TdntData.dRelaxRatio = _tstof(csRelaxCoef);

			int nIndex2 = m_cobxRelaxCase.GetCurSel();
			if (nIndex2 == CB_ERR) m_TdntData.TdmfKey=0;
			else m_TdntData.TdmfKey = m_cobxRelaxCase.GetItemData(nIndex2);

		}
		else if (m_TdntData.nRelaxMethod == TDNT_RELAX_ABNT_NBR_6118_2023)
		{
			if (m_wndRelaxJTG04Cmb.GetCurSel() == 0) // Normal
				m_TdntData.dRelaxCoef = 1.0;
			else
				m_TdntData.dRelaxCoef = 2.0; // Slow

			int nIndex2 = m_cobxABNTRelaxCase.GetCurSel();
			if (nIndex2 == CB_ERR) m_TdntData.TdmfKey = 0;
			else m_TdntData.TdmfKey = m_cobxABNTRelaxCase.GetItemData(nIndex2);

		}
		else if (m_TdntData.nRelaxMethod == TDNT_RELAX_USER_DEFINED)
		{
			m_CobxRelax.GetSelectedTdmf( m_TdntData.TdmfKey );
		}
		else 
			ASSERT(0);
	}
	else // 체크박스 체크 안된 경우
	{
		m_TdntData.dRelaxCoef = 0.;
		m_TdntData.dRelaxRatio = 0.;    
		m_TdntData.dFpk = 0.;
		m_TdntData.dJTG04Factor = 0.0; // TB05 일때도 적용
		m_TdntData.TdmfKey = 0;
	}

	m_TdntData.bOverStressRedFactor = this->m_bOverStreRedFactor;
	m_TdntData.bLowRelax = this->m_bLowRelax;
	m_TdntData.dFpk = m_wndFpkEdit.GetEditValue();  

	if(!m_wndMatEdit.GetKey(m_TdntData.MatlKey)) return FALSE;
	int nSel = m_wndTendonTypeCombo.GetCurSel();

	/*
	TB05 인 경우 Relax Factor값 - Matl 키와 bOverStreRedFactor 입력 받은 후 계산
	변수는 dJTG04Factor 를 재활용하고
	솔버에서 재계산 해야 하는 재질의 경우는 -1, -2, 0.035, 0.05 의 값을 가진다.
	기타 다른 재질의 경우는 사용자 입력 값을 갖는다.
	dJTG04Factor 가 0.0 인 경우를 CheckBox 가 체크 안된 상태, 0이 아닌 경우를 체크된 상태로 간주
	AnalCtrl 에서 재질에 따라 한번 더 계산하여 -1, 2, 0.035, 0.05의 값과 Case를 솔버에 넘긴다.
	*/
	if(m_TdntData.nRelaxMethod == TDNT_RELAX_TB05 ||
	   m_TdntData.nRelaxMethod == TDNT_RELAX_TB10092_17 ||
	   m_TdntData.nRelaxMethod == TDNT_RELAX_QCR9300_18)
	{
		if (this->m_bRelaxation == FALSE) // Check Off
		{
			m_TdntData.dJTG04Factor = 0.0;
		}
		else // Check On
		{
			if (m_edtXi.IsWindowEnabled())
			{
				double dTmp = m_edtXi.GetEditValue();
				if (dTmp <= 0.0) return FALSE;
				m_TdntData.dJTG04Factor = dTmp;
			}
			else
				CDBDoc::GetDocPoint()->m_pAttrCtrl->CalcXiTB(m_TdntData);
		}
	}
	
	if(nSel==0) // Internal - pre 
	{
		m_TdntData.nTendonType = 1;  
		m_TdntData.nLoadType = 0;//m_nLoadType;
		//m_wndDuctDiameter.SetWindowText(_T("Strand Diameter"));
	}
	else if(nSel==1) // Internal - post
	{
		m_TdntData.nTendonType = 1;
		m_TdntData.nLoadType = 1;
	 // m_wndDuctDiameter.SetWindowText(_T("Duct Diameter"));
	}
	else  // External
	{
		m_TdntData.nTendonType = 2;
		m_TdntData.nLoadType = 0;
		//
	}
 
	m_TdntData.TendonTypeName = m_strTendonName;
	m_TdntData.nBondType      = m_nBondType;
	m_TdntData.dAnchoSlipBegin = m_wndAnchoBegin.GetEditValue();
	m_TdntData.dAnchoSlipEnd   = m_wndAnchoEnd.GetEditValue();

	int nIndex3 = m_cobxRelaxMethod.GetCurSel();
	DWORD data = m_cobxRelaxMethod.GetItemData(nIndex3);
	if(data == TDNT_RELAX_EUROPEAN || data == TDNT_RELAX_CEB_FIP || data == TDNT_RELAX_CEB_FIP_1990 || data == TDNT_RELAX_CEB_FIP_2010 || data == TDNT_RELAX_ABNT_NBR_6118_2023)
	{
		int nSel=0;
		CDlgUtil::CtrlRadioGetCheck(this, m_aWobbleType,  nSel);

		m_TdntData.nWobbleType   = nSel;
		m_TdntData.dWobbleAngle  = m_wndWobbleAngleEURO.GetEditValue();
		if(nSel==0) //K
		{
			m_TdntData.dWobbleFactor = m_wndWobbleEURO.GetEditValue();
		}
		else // k
		{
			double dWobbleFactor = m_TdntData.dFrictionFactor * m_TdntData.dWobbleAngle;        
			m_TdntData.dWobbleFactor = dWobbleFactor;
		}
	}
	else
	{
		m_TdntData.dWobbleFactor = m_wndWobble.GetEditValue();
	}
	//m_TdntData.dInadvertentChange = m_wndInadvertent.GetEditValue();

	if(!CStrParser::GetFloatNumber(m_strUltimate,m_TdntData.dUltimateStrength)) return FALSE;
	if(!CStrParser::GetFloatNumber(m_strYield,m_TdntData.dYieldStrength)) return FALSE;
	if(!CStrParser::GetFloatNumber(m_strExternal,m_TdntData.dAlphaExtCable)) return FALSE;

	if(this->m_TdntKey == 0) // Add
	{
		if(!CDBDoc::GetDocPoint()->m_pDataCtrl->AddTdnt(m_TdntData))
		{
			m_bIsDBError = TRUE;
			return TRUE;
		}
	}
	else
	{
		T_TDNT_D TdntD;
		if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdnt(m_TdntKey,TdntD))
		{
			m_bIsDBError = TRUE;
			return TRUE;
		}
		if(!CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyTdnt(TdntD.TendonTypeName,m_TdntData))
		{
			m_bIsDBError = TRUE;
			return TRUE;
		}
	}

	return TRUE;
}


void CCMTendonTypeAddDlg::OnOK() 
{
	if(ExecuteDlg())
	{
		if(m_bIsDBError == FALSE)
	    CChildDialogMove::OnOK();
	}
	else
		GSaveHistoryFormatNF(_LS(IDS_CMD_TDNT_ITEM_Parameter_Error__));
}

void CCMTendonTypeAddDlg::OnCmdApply() 
{
	if(!ExecuteDlg()) 
		GSaveHistoryFormatNF(_LS(IDS_CMD_TDNT_ITEM_Parameter_Error__));
}

void CCMTendonTypeAddDlg::OnCmdRelaxCheck() 
{
	UpdateData(TRUE);
	OnSelchangeCmdRelaxCombo2(); // MNET:XXXX 20070511 mylee
	RelaxCtrlMan();
}

void CCMTendonTypeAddDlg::OnSelchangeMatCombo()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_MATL_K MatlK = 0;
	T_MATL_D MatlD;
	m_wndMatEdit.GetKey(MatlK);
	BOOL bJTJ023_85S = FALSE;
	if (pDoc->m_pAttrCtrl->GetMatl(MatlK, MatlD))
	{
		if (MatlD.Type == _T("S"))
		{
			if (MatlD.Data1.CodeName == MATLCODE_STL_JTJ023_85)
				bJTJ023_85S = TRUE;
		}
	}

	GetDlgItem(IDC_CMD_TAREA_BUTTON)->ShowWindow(!bJTJ023_85S);
}

void CCMTendonTypeAddDlg::OnSelchangeCmdRelaxCombo() 
{
	m_nRelaxation = this->m_wndRelaxCombo.GetCurSel();
}

void CCMTendonTypeAddDlg::OnSelchangeCmdABNTRelaxTypeCombo()
{
	int nIndex = m_cobxABNTRelaxCase.GetCurSel();
	int nIndex2 = 0;
	if (nIndex != CB_ERR)
	{
		nIndex2 = m_cobxABNTRelaxCase.GetItemData(nIndex);
	}
	CArray<UINT, UINT> aABNT;
	aABNT.Add(IDC_CMD_RELAX_JTG04_CMB);
	CDlgUtil::CtrlEnableDisable(this, aABNT, (nIndex2 != 3));
}

void CCMTendonTypeAddDlg::OnSelchangeCmdTendonTypeCombo()
{
	EnableControl();
	SetFieldData();
}

void CCMTendonTypeAddDlg::EnableControl()
{
	int nSel=m_wndTendonTypeCombo.GetCurSel();
	
	CDlgUtil::CtrlEnableDisable(this, m_aGroup, nSel==1);
	GetDlgItem(IDC_CMD_TPRST_ANCHO_BEGIN_ED)->EnableWindow(nSel!=0);
	GetDlgItem(IDC_CMD_TPRST_ANCHO_END_ED)  ->EnableWindow(nSel!=0);

	// MNET:XXXX-SHJUNG-20100407 - PMS4169 _T("External Cable Moment Magnifier") Hide
#if defined(_MGEN)
	GetDlgItem(IDC_CMD_YIELD_EXTERNAL_EDIT) ->EnableWindow(FALSE);
#else
	GetDlgItem(IDC_CMD_YIELD_EXTERNAL_EDIT) ->EnableWindow(nSel==2);
#endif  

	GetDlgItem(IDC_CMD_DUCT_AREA_EDIT)      ->EnableWindow(nSel!=2);
	GetDlgItem(IDC_CMD_FRICTION_EDIT)       ->EnableWindow(nSel!=0);
	//CDlgUtil::CtrlEnableDisable(this,m_aCtrlTypeInternal,nSel==1 || nSel==2);
	CDlgUtil::CtrlEnableDisable(this,m_aCtrlTypeInternal,nSel==1);

	//CDlgUtil::CtrlShowHide(this,m_aCtrlTypeInternal,nSel==0||nSel==1);
	//CDlgUtil::CtrlShowHide(this,m_aCtrlTypeExternal,nSel==2);
	/*뽕 임시 작업*///CDlgUtil::CtrlEnableDisable(this,m_aCtrlTypeExternal,FALSE);

	// for Eurocode
	if(nSel==0 || nSel==2)
		CDlgUtil::CtrlEnableDisable(this,m_aWoodleEuro, FALSE);  
	else
	{
		CDlgUtil::CtrlEnableDisable(this,m_aWoodleEuro, TRUE);  
		OnCmdRelaxWobblrBtn();
	}
	//m_wndDuctArea.EnableWindow(TRUE);
}

void CCMTendonTypeAddDlg::SetFieldData()
{
	int nSel=m_wndTendonTypeCombo.GetCurSel();

	if (nSel==0)  // Internal(Pre)
	{
		GetDlgItem(IDC_CMD_FRICTION_EDIT)->SetWindowText(_T("0"));
		m_wndWobble.SetEditUnit(0.0);
		//m_wndInadvertent.SetEditUnit(0.0);
		m_wndAnchoEnd.SetEditUnit(0.0);
	  m_wndAnchoBegin.SetEditUnit(0.0);
		m_wndDuctDiameter.SetWindowText(_LS(IDS_CMD_Strand_Diameter)); //_T("IDS_CMD_Strand_Diameter")
		//m_wndDuctArea.EnableWindow(TRUE);
	}
	else if (nSel==2)  // External
	{
//    GetDlgItem(IDC_CMD_FRICTION_EDIT)->SetWindowText(_T("0"));
		//m_wndWobble.SetEditUnit(0.0);
		m_wndDuctArea.SetEditUnit(0.0);
		m_wndDuctDiameter.SetWindowText(_LS(IDS_CMD_Duct_Diameter)); //_T("IDS_CMD_Duct_Diameter")
		//GetDlgItem(IDC_CMD_DUCT_AREA_EDIT)-> EnableWindow(FALSE);
 //   m_wndDuctArea.
	}
	else  // Internal(Post)
	{
		//m_wndInadvertent.SetEditUnit(0.0);
		m_wndDuctDiameter.SetWindowText(_LS(IDS_CMD_Duct_Diameter));
		//SetDiameterField();
	}
	 // m_wndDuctArea.EnableWindow(TRUE);

	// for Eurocode
	if(nSel==0)
	{
		m_wndWobbleEURO.SetEditUnit(0.0);
		m_wndWobbleAngleEURO.SetEditUnit(0.0);
	}

	Invalidate();
	UpdateWindow();
}

void CCMTendonTypeAddDlg::OnChangeCmdTotalTendonAreaEdit() 
{
	int nSel=m_wndTendonTypeCombo.GetCurSel();
/*
	if (nSel == 1)
		SetDiameterField();
*/
}

void CCMTendonTypeAddDlg::SetDiameterField()
{
	double dArea = m_wndTArea.GetEditValue();
	double dDiameter = 2*sqrt(dArea/PI);
	dDiameter *= 10000000000;
	ceil(dDiameter);
	dDiameter /= 10000000000;
	m_wndDuctArea.SetEditUnit(dDiameter);
	m_strDuctArea.Format(_T("%.10g"), dDiameter);
}

/*
void CCMTendonTypeAddDlg::OnCmdTensionOpt() 
{
	UpdateData(TRUE);

	if(m_nLoadType == 0)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aPreGroup, FALSE);
		CDlgUtil::CtrlEnableDisable(this, arBondType, FALSE);
	}
	else
	{
		CDlgUtil::CtrlEnableDisable(this, m_aPreGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, arBondType, TRUE);
	}
}
*/

BOOL CCMTendonTypeAddDlg::GetFpbWhenAS(double& dFpb) 
{
	dFpb = 0.0;
	if(m_cobxRelaxMethod.GetItemData(m_cobxRelaxMethod.GetCurSel()) != TDNT_RELAX_AS_1500_5_2017)
		return FALSE;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_MATL_K MatlK = 0;
	T_MATL_D MatlD;  
	m_wndMatEdit.GetKey(MatlK);
	if(!pDoc->m_pAttrCtrl->GetMatl(MatlK, MatlD))
	{
		return FALSE;
	}

	int nReturn = 0;

	if(MatlD.Type == _T("S"))
	{
		T_MATL_STEEL sData;
		if(MatlD.Data1.CodeName== MATLCODE_STL_AS_NZS_4672_1)
		{			
			pDoc->m_pMatlDB->GetSteelData(MatlD.Data1.CodeName, MatlD.Data1.CodeMatlName, sData);

			dFpb = sData.S_Fy1;

			return TRUE;
		}
	}
	return FALSE;

}

void CCMTendonTypeAddDlg::OnSelchangeCmdRelaxCombo2() 
{
	int nSel = m_cobxRelaxMethod.GetCurSel();
	if (nSel == CB_ERR) { ASSERT(0); return; }

	DWORD data = m_cobxRelaxMethod.GetItemData(nSel);

	BOOL bMagura            = data == TDNT_RELAX_MAGURA                 ;
	BOOL bCEB_FIP           = data == TDNT_RELAX_CEB_FIP                ;
	BOOL bJTG04             = data == TDNT_RELAX_JTG04                  ;
	BOOL bTB05              = data == TDNT_RELAX_TB05                   ;
	BOOL bINDIA_IRC18_2000  = data == TDNT_RELAX_INDIA_IRC18_2000       ;
	BOOL bEuropean          = data == TDNT_RELAX_EUROPEAN               ;
	BOOL bKSCE_LSD15        = data == TDNT_RELAX_KSCE_LSD15             ;
	BOOL bINDIA_IRC112_2011 = data == TDNT_RELAX_INDIA_IRC112_2011      ;
	BOOL bUserDefined       = data == TDNT_RELAX_USER_DEFINED           ;
	BOOL bCEB_FIP1990       = data == TDNT_RELAX_CEB_FIP_1990           ;
	BOOL bCEB_FIP2010       = data == TDNT_RELAX_CEB_FIP_2010           ;
	BOOL bTB10092_17        = data == TDNT_RELAX_TB10092_17             ;
	BOOL bAS1500_5_2017     = data == TDNT_RELAX_AS_1500_5_2017         ;
	BOOL bQCR9300_18		= data == TDNT_RELAX_QCR9300_18				;
	BOOL bJTG02385          = data == TDNT_RELAX_JTG023_85              ;
	BOOL bABNT             = data == TDNT_RELAX_ABNT_NBR_6118_2023;

	GetDlgItem(IDC_CMD_RELAX_COMBO)->ShowWindow(bMagura);

	GetDlgItem(IDC_CMD_RELAX_STC)->ShowWindow(bCEB_FIP||bCEB_FIP1990||bCEB_FIP2010);
	GetDlgItem(IDC_CMD_RELAX_EDIT)->ShowWindow(bCEB_FIP||bCEB_FIP1990||bCEB_FIP2010);
	GetDlgItem(IDC_CMD_RELAX_TEXT)->ShowWindow(bCEB_FIP||bCEB_FIP1990||bCEB_FIP2010);


	if(bCEB_FIP||bCEB_FIP1990||bCEB_FIP2010)
	{
		CString strStatic;
		if(bCEB_FIP2010 || bCEB_FIP1990) strStatic = _LS(IDS_CMD_TENDON_TYPE_1); //(_T("ρ1000"));
		else strStatic = _LS(IDS_CMD_TENDON_TYPE_2);//(_T("ρ∞"));

		GetDlgItem(IDC_CMD_RELAX_STC)->SetWindowText(strStatic);
		
		if(bCEB_FIP2010) m_cobxRelaxCase.SetCurSel(0);
	}

	if(bAS1500_5_2017)
	{
		GetDlgItem(IDC_CMD_TB05_STC)->SetWindowText(_LS(IDS_CMD_TENDON_RB));
		GetDlgItem(IDC_CMD_ULTIMATE_STRENGTH_STATIC)->SetWindowText(_LS(IDS_CMD_FPB));
		GetDlgItem(IDC_CMD_YIELD_STRENGTH_STATIC)->SetWindowText(_LS(IDS_CMD_FPY));
	}
	else
	{
		GetDlgItem(IDC_CMD_TB05_STC)->SetWindowText(_LS(IDS_CMD_TENDON_TYPE_XI)); //(_T("Xi:")) = ξ 
		GetDlgItem(IDC_CMD_ULTIMATE_STRENGTH_STATIC)->SetWindowText(_LS(IDS_CMD_TENDON_ULTIMATE_STRENGTH)); //_T("Ultimate Strength")
		GetDlgItem(IDC_CMD_YIELD_STRENGTH_STATIC)->SetWindowText(_LS(IDS_CMD_TENDON_YIELD_STRENGTH)); // _T("Yield Strength")
	}

	CDlgUtil::CtrlShowHide(this, m_aCtrlRelaxModelCEB, bMagura || bCEB_FIP || bINDIA_IRC18_2000 || bEuropean || bKSCE_LSD15 || bINDIA_IRC112_2011 || bUserDefined ||bCEB_FIP1990 || bCEB_FIP2010 || bAS1500_5_2017 || bABNT);
	
	GetDlgItem(IDC_CMD_OVERRED_FACTOR_CHECK)->ShowWindow(bJTG04 || bTB05 || bTB10092_17 || bQCR9300_18 /*|| bJTG02385*/);
	GetDlgItem(IDC_CMD_TDNT_FPK_STATIC)->ShowWindow(bJTG04 || bTB05 || bTB10092_17 || bQCR9300_18 || bJTG02385);
	GetDlgItem(IDC_CMD_TDNT_FPK_EDIT)->ShowWindow(bJTG04 || bTB05 || bTB10092_17 || bQCR9300_18 || bJTG02385);
	GetDlgItem(IDC_CMD_TDNT_FPK_UNIT)->ShowWindow(bJTG04 || bTB05 || bTB10092_17 || bQCR9300_18 || bJTG02385);

	GetDlgItem(IDC_CMD_RELAX_JTG04_CMB)->EnableWindow(TRUE);
	GetDlgItem(IDC_CMD_RELAX_JTG04_CMB)->ShowWindow(bJTG04 || bINDIA_IRC18_2000 || bEuropean || bKSCE_LSD15 || bINDIA_IRC112_2011 || bABNT);
	if(bJTG04 || bINDIA_IRC18_2000 || bEuropean || bKSCE_LSD15 || bINDIA_IRC112_2011 || bABNT) // m_wndRelaxJTG04Cmb 를 같이 사용한다
	{
		m_wndRelaxJTG04Cmb.ResetContent();
		if (bJTG04) 
		{ 
			m_wndRelaxJTG04Cmb.AddString(_T("1.0")); 
			m_wndRelaxJTG04Cmb.AddString(_T("0.3"));
		}
		else if (bINDIA_IRC18_2000) 
		{ 
			m_wndRelaxJTG04Cmb.AddString(_LS(IDS_CMD_TENDON_NORMAL)); 
			m_wndRelaxJTG04Cmb.AddString(_LS(IDS_CMD_TENDON_LOW));
		}
		else if (bINDIA_IRC112_2011) 
		{ 
			m_wndRelaxJTG04Cmb.AddString(_LS(IDS_CMD_TENDON_NORMAL)); 
			m_wndRelaxJTG04Cmb.AddString(_LS(IDS_CMD_TENDON_LOW));
		}
		else if (bABNT)
		{
			m_wndRelaxJTG04Cmb.AddString(_LS(IDS_CMD_TENDON_NORMAL));
			m_wndRelaxJTG04Cmb.AddString(_LS(IDS_CMD_TENDON_SLOW));

			m_cobxABNTRelaxCase.SetCurSel(0);

			OnSelchangeCmdABNTRelaxTypeCombo();
		}
		else                
		{ 
			m_wndRelaxJTG04Cmb.AddString(_LS(IDS_CMD_TENDON_ORDINARY)); 
			m_wndRelaxJTG04Cmb.AddString(_LS(IDS_CMD_TENDON_LOW)); 
			m_wndRelaxJTG04Cmb.AddString(_LS(IDS_CMD_TENDON_HOTROLL));
		}
		m_wndRelaxJTG04Cmb.SetCurSel(0);
	}
//   GetDlgItem(IDC_CMD_ULTIMATE_STRENGTH_STATIC)->ShowWindow(bMagura || bCEB_FIP || bINDIA_IRC18_2000 || bEuropean || bKSCE_LSD15 || bINDIA_IRC112_2011);
//   GetDlgItem(IDC_CMD_ULTIMATE_STRENGTH__EDIT )->ShowWindow(bMagura || bCEB_FIP || bINDIA_IRC18_2000 || bEuropean || bKSCE_LSD15 || bINDIA_IRC112_2011);
//   GetDlgItem(IDC_CMD_PRESS_UNIT3)->ShowWindow(bMagura || bCEB_FIP || bINDIA_IRC18_2000 || bEuropean || bKSCE_LSD15 || bINDIA_IRC112_2011);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// INDIA(IRC:18-2000), European
//   CRect rRef, rToMove;
//   int nDistX, nDistY;
//   GetDlgItem(IDC_CMD_ULTIMATE_STRENGTH__EDIT)->GetWindowRect(rRef);
//   GetDlgItem(IDC_CMD_YIELD_STRENGTH_EDIT)->GetWindowRect(rToMove);
//   nDistX = rRef.left - rToMove.left;
//   nDistY = rRef.top - rToMove.top;
// 
//   CArray<UINT, UINT> aBotCtrlID;
// 
//   aBotCtrlID.Add(IDC_CMD_STATIC19);             aBotCtrlID.Add(IDC_CMD_FRICTION_EDIT);
//   aBotCtrlID.Add(IDC_CMD_WOBBLE_STATIC);        aBotCtrlID.Add(IDC_CMD_WOBBLE_EDIT);
//   aBotCtrlID.Add(IDC_CMD_WOBBLE_UNIT);          aBotCtrlID.Add(IDC_STATIC_EXTERNAL);
//   aBotCtrlID.Add(IDC_CMD_YIELD_EXTERNAL_EDIT);  aBotCtrlID.Add(IDC_CMD_PRESS_UNIT5);
//   aBotCtrlID.Add(IDC_CMD_SLIP_STATIC);          aBotCtrlID.Add(IDC_CMD_STATIC21);
//   aBotCtrlID.Add(IDC_CMD_STATIC23);             aBotCtrlID.Add(IDC_CMD_BOND_STATIC);
//   aBotCtrlID.Add(IDC_CMD_STATIC24);             aBotCtrlID.Add(IDC_CMD_TPRST_ANCHO_BEGIN_ED);
//   aBotCtrlID.Add(IDC_CMD_TPRST_ANCHO_END_ED);   aBotCtrlID.Add(IDC_CMD_TPRST_ANCHO_BEGIN_UNIT);
//   aBotCtrlID.Add(IDC_CMD_TPRST_ANCHO_END_UNIT); aBotCtrlID.Add(IDC_CMD_STATIC_25);
//   aBotCtrlID.Add(IDC_CMD_TPRST_BOND_TYPE1);     aBotCtrlID.Add(IDC_CMD_TPRST_BOND_TYPE2);
//   
//   if (m_bINDIA)
//   {
//     CDlgUtil::CtrlMoveDistXY(this, aBotCtrlID, -nDistX, -nDistY);
//     m_bINDIA = FALSE;
//   }
//   if (m_bEUROPEAN)
//   {
//     CDlgUtil::CtrlMoveDistXY(this, aBotCtrlID, -nDistX, -nDistY);
//     m_bEUROPEAN = FALSE;
//   }
// 
//   if(data == 4)     //INDIA (IRC:18-2000)
//   {
//     CDlgUtil::CtrlMoveDistXY(this, aBotCtrlID, nDistX, nDistY);
//     m_bINDIA = TRUE;
//   }
//   if(data == 5)     // European
//   {
//     CDlgUtil::CtrlMoveDistXY(this, aBotCtrlID, nDistX, nDistY);
//     m_bEUROPEAN = TRUE;
//   }

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	T_MATL_K MatlK = 0;
	T_MATL_D MatlD;  
	BOOL bChinaMatl = FALSE;
	m_wndMatEdit.GetKey(MatlK);
	if(pDoc->m_pAttrCtrl->GetMatl(MatlK, MatlD))
	{
		if(MatlD.Type == _T("S") && pDoc->m_pMatlDB->IsChinaCodeForSteel(MatlD.Data1.CodeName)) // JTG04(S)
		{
			bChinaMatl = TRUE;        
		}
	}
	//m_wndFpkEdit.SetReadOnly(bChinaMatl && (data==2 || bTB05 || bTB10092_17 || bQCR9300_18 || bJTG02385)); 항상 입력가능하도록 변경

	// TB05
	GetDlgItem(IDC_CMD_TB05_STC)->ShowWindow((bTB05 || bTB10092_17 || bAS1500_5_2017 || bQCR9300_18 || bJTG02385) && this->m_bRelaxation);
	GetDlgItem(IDC_CMD_TB05_EDT)->ShowWindow((bTB05 || bTB10092_17 || bAS1500_5_2017 || bQCR9300_18 || bJTG02385) && this->m_bRelaxation);
	if (bTB05 || bTB10092_17 || bQCR9300_18)
	{
		m_edtXi.SetEditUnit(0);

		BOOL bEnableXi = FALSE;
		if(MatlD.Type == _T("S") && 
			(MatlD.Data1.CodeName==_ULS('TB05(S)') || 
			MatlD.Data1.CodeName==_ULS('TB10092-17(S)') ||
			MatlD.Data1.CodeName==_ULS('Q/CR 9300-18(S)')) )
		{
			bEnableXi = FALSE;
		}
		else bEnableXi = TRUE;
		
		if (bEnableXi)
		{
			//GetDlgItem(IDC_CMD_TB05_STC)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_TB05_EDT)->EnableWindow(TRUE);
			m_edtXi.SetEditUnit(0);
			//m_wndFpkEdit.SetReadOnly(FALSE);
		}
		else
		{
			//GetDlgItem(IDC_CMD_TB05_STC)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_TB05_EDT)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_TB05_EDT)->SetWindowText(_LSX(Auto));
			//m_wndFpkEdit.SetReadOnly(TRUE);
		}
	}
	else if (bJTG02385)
	{
		m_edtXi.SetEditUnit(0.05); // 대화창 초기값
	}
	else if(bAS1500_5_2017)
	{
		m_edtXi.SetEditUnit(0);

		BOOL bByMatldB = FALSE;
		double dFpb = 0.0;
		bByMatldB = GetFpbWhenAS(dFpb);
		if(bByMatldB)
		{
			m_wndFpkEdit.SetEditUnit(dFpb);
		}
		//m_wndFpkEdit.SetReadOnly(bByMatldB);
		GetDlgItem(IDC_CMD_TB05_EDT)->EnableWindow(TRUE);
		CString csTmp;
		m_edtXi.GetWindowText(csTmp);
		if (csTmp == _LSX(Auto)) // 자동 계산이 아닌 경우
			m_edtXi.SetEditUnit(0.0);
	}

	EnableRelaxUserDefined();

	if(bEuropean || bCEB_FIP || bCEB_FIP1990 || bCEB_FIP2010 || bABNT)
	{
		GetDlgItem(IDC_CMD_STATIC19)->SetWindowText(_LS(IDS_CMD_TDNT_ITEM_Curvature_Friction_Factor));    
		CDlgUtil::CtrlShowHide(this, m_aWoodle,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aWoodleEuro, TRUE);
		SetData4Euro();
		OnCmdRelaxWobblrBtn();
		EnableControl();
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aWoodle,     TRUE);
		CDlgUtil::CtrlShowHide(this, m_aWoodleEuro, FALSE);
	}
 
	AlignControlsDynamic();

	RedrawWindow();
}

void CCMTendonTypeAddDlg::OnCmdSetLongTermStressLossBtn()
{
	// Todo HJM : Set Long Term Stress Loss Dialog
	CCMTendonSetLongTermStressLossDlg Dlg;
	Dlg.SetData(m_TdntData.LongTermStressLossData);

	if (IDOK == Dlg.DoModal())
	{
		Dlg.GetData(m_TdntData.LongTermStressLossData);
	}
}

void CCMTendonTypeAddDlg::AlignControlsDynamic()
{
	// 사용자 입력에 따라 여러번 move 해야 하는 상황을 제어하는 함수임
	// 바로 위 ctrl 을 기준으로 움직이도록 코드를 넣어 두면 추가 개발이 들어와도 꼬이지 않음..

	int nRelaxMethod = CDlgUtil::CobxGetCurSelItemData(m_cobxRelaxMethod, m_cobxRelaxMethod.GetCurSel());

	CRect rRef, rToMove;
	int nDistX, nDistY;
	CArray<UINT,UINT>  aCtrl;

	aCtrl.RemoveAll();
	aCtrl.Add(IDC_CMD_YIELD_STRENGTH_STATIC);
	aCtrl.Add(IDC_CMD_YIELD_STRENGTH_EDIT);
	aCtrl.Add(IDC_CMD_PRESS_UNIT4);
	aCtrl.Add(IDC_CMD_TDNT_FPK_STATIC);
	aCtrl.Add(IDC_CMD_TDNT_FPK_EDIT);
	aCtrl.Add(IDC_CMD_TDNT_FPK_UNIT);
	aCtrl.Add(IDC_CMD_STATIC19);
	aCtrl.Add(IDC_CMD_FRICTION_EDIT);
	aCtrl.Add(IDC_CMD_WOBBLE_STATIC);
	aCtrl.Add(IDC_CMD_WOBBLE_EDIT);
	aCtrl.Add(IDC_CMD_WOBBLE_UNIT);

	if(nRelaxMethod == TDNT_RELAX_JTG04 || nRelaxMethod == TDNT_RELAX_TB05 || nRelaxMethod == TDNT_RELAX_TB10092_17 || nRelaxMethod == TDNT_RELAX_QCR9300_18)
	{
		GetDlgItem(IDC_CMD_OVERRED_FACTOR_CHECK)->GetWindowRect(rRef);  
		GetDlgItem(IDC_CMD_TDNT_FPK_EDIT)->GetWindowRect(rToMove);     
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(6); // 아래에 배치
	}
	else if (nRelaxMethod == TDNT_RELAX_JTG023_85)
	{	
		GetDlgItem(IDC_CMD_LOW_RELAX_CHK)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_TDNT_FPK_EDIT)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.top - rToMove.top;
	}
	else
	{
		GetDlgItem(IDC_CMD_ULTIMATE_STRENGTH__EDIT)->GetWindowRect(rRef);  
		GetDlgItem(IDC_CMD_TDNT_FPK_EDIT)->GetWindowRect(rToMove);     
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(4); // 아래에 배치
	}	
	CDlgUtil::CtrlMoveDistXY(this, aCtrl, 0, nDistY);

	GetDlgItem(IDC_CMD_WOBBLE_EDIT)->GetWindowRect(rRef);  
	GetDlgItem(IDC_CMD_WOBBLE_EURO_EDIT)->GetWindowRect(rToMove); 
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;  // 동일한 위치에 배치
	CDlgUtil::CtrlMoveDistXY(this, m_aWoodleEuro, 0, nDistY);	

	if(nRelaxMethod == TDNT_RELAX_EUROPEAN || nRelaxMethod == TDNT_RELAX_CEB_FIP || nRelaxMethod == TDNT_RELAX_CEB_FIP_1990 || nRelaxMethod == TDNT_RELAX_CEB_FIP_2010 || nRelaxMethod == TDNT_RELAX_ABNT_NBR_6118_2023)
	{
		GetDlgItem(IDC_CMD_WOBBLE_ANGLE_FACTOR_EURO_EDT)->GetWindowRect(rRef);  
	}
	else
	{
		GetDlgItem(IDC_CMD_WOBBLE_EDIT)->GetWindowRect(rRef);  
	}
	GetDlgItem(IDC_CMD_YIELD_EXTERNAL_EDIT)->GetWindowRect(rToMove);     
	nDistX = rRef.left - rToMove.left;
#if defined(_RUS)	
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);  // 아래에 배치
#else
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(4);  // 아래에 배치
#endif
	CDlgUtil::CtrlMoveDistXY(this, m_IncCtrls1, 0, nDistY);

	GetDlgItem(IDC_CMD_YIELD_EXTERNAL_EDIT)->GetWindowRect(rRef);  
	GetDlgItem(IDC_CMD_SLIP_STATIC)->GetWindowRect(rToMove); 
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(13);  // 아래에 배치
	CDlgUtil::CtrlMoveDistXY(this, m_IncCtrls2, 0, nDistY);

	GetDlgItem(IDC_CMD_YIELD_EXTERNAL_EDIT)->GetWindowRect(rRef);  
	GetDlgItem(IDC_CMD_BOND_STATIC)->GetWindowRect(rToMove); 
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(13);  // 아래에 배치
	CDlgUtil::CtrlMoveDistXY(this, m_IncCtrls3, 0, nDistY);

	// resize
	CRect rRect;
	GetDlgItem(IDC_CMD_STATIC22)->GetWindowRect(rRect);
	rRect.bottom += nDistY;
	ScreenToClient(rRect);
	GetDlgItem(IDC_CMD_STATIC22)->MoveWindow(rRect);

	CDlgUtil::CtrlMoveDistXY(this, m_IncCtrls5, 0, nDistY);

	// move
	GetDlgItem(IDC_CMD_STATIC22)->GetWindowRect(rRef);
	GetDlgItem(IDOK)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
	CDlgUtil::CtrlMoveDistXY(this, m_IncCtrls4, 0, nDistY);

	// resize self size
	CRect rWindow, rOK;
	GetDlgItem(IDOK)->GetWindowRect(rOK);
	GetWindowRect(rWindow);
	rWindow.bottom = rOK.bottom + globalUtils.ScaleByDPI(15);
	MoveWindow(rWindow);
}

void CCMTendonTypeAddDlg::OnChangeCmdMatEdit() 
{
	ChangeMaterial();
}

void CCMTendonTypeAddDlg::ChangeMaterial()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	T_MATL_K MatlK = 0;
	T_MATL_D MatlD;
	int nRelaxCoefCode = 0;
	#if defined(_CH)
	nRelaxCoefCode = TDNT_RELAX_JTG04;
	#elif defined(_JP)
	nRelaxCoefCode = TDNT_RELAX_CEB_FIP_2010;
	#endif

	double dFpk = m_wndFpkEdit.GetEditValue();

	m_wndMatEdit.GetKey(MatlK);
	if (pDoc->m_pAttrCtrl->GetMatl(MatlK, MatlD))
	{
		if (MatlD.Type == _T("S"))
		{
			if (pDoc->m_pMatlDB->IsKoreaCodeForSteel(MatlD.Data1.CodeName)) nRelaxCoefCode = 0; // 국내기준
			else if (pDoc->m_pMatlDB->IsJapanCodeForSteel(MatlD.Data1.CodeName)) nRelaxCoefCode = TDNT_RELAX_CEB_FIP_2010; // 일본기준
			else if (MatlD.Data1.CodeName == _ULS('TB05(S)') ||
				     MatlD.Data1.CodeName == _ULS('TB10092-17(S)') ||
					 MatlD.Data1.CodeName == _ULS('Q/CR 9300-18(S)') )
			{
				if (MatlD.Data1.CodeName == _ULS('TB05(S)')) nRelaxCoefCode = TDNT_RELAX_TB05;
				else if(MatlD.Data1.CodeName == _ULS('TB10092-17(S)')) nRelaxCoefCode = TDNT_RELAX_TB10092_17;
				else if(MatlD.Data1.CodeName == _ULS('Q/CR 9300-18(S)')) nRelaxCoefCode = TDNT_RELAX_QCR9300_18;
				T_MATL_STEEL sData;
				CString strCode    = MatlD.Data1.CodeName;
				CString strMatName = MatlD.Data1.CodeMatlName;				
				pDoc->m_pMatlDB->GetSteelData(strCode, strMatName, sData);
				dFpk = sData.S_Fy1;
				m_wndFpkEdit.SetEditUnit(dFpk);
				//m_wndFpkEdit.SetReadOnly(TRUE);
			}
			else if(pDoc->m_pMatlDB->IsChinaCodeForSteel(MatlD.Data1.CodeName)) // _T("JTG04(S)") 
			{
				nRelaxCoefCode = TDNT_RELAX_JTG04;  // 중국 JTG04
				T_MATL_STEEL sData;
				CString strCode    = MatlD.Data1.CodeName;
				CString strMatName = MatlD.Data1.CodeMatlName;				
				pDoc->m_pMatlDB->GetSteelData(strCode, strMatName, sData);
				dFpk = sData.S_Fy1;
				m_wndFpkEdit.SetEditUnit(dFpk);
				//m_wndFpkEdit.SetReadOnly(TRUE);
			}
			else if (MatlD.Data1.CodeName == _ULS('AS/NZS 4672.1(S)'))
			{	
				nRelaxCoefCode = TDNT_RELAX_AS_1500_5_2017;
			}
		}
	}

	CDlgUtil::CobxSetCurSelItemData(m_cobxRelaxMethod, nRelaxCoefCode);

	if(nRelaxCoefCode==0)
	{
#if defined(_RUS)	
		CDlgUtil::CobxSetCurSelItemData(m_cobxRelaxMethod, TDNT_RELAX_INDIA_IRC112_2011);
#elif defined(_CIVIL_ORG)
		CDlgUtil::CobxSetCurSelItemData(m_cobxRelaxMethod, TDNT_RELAX_KSCE_LSD15);
#endif
	}

	m_wndRelaxCombo.SetCurSel(1);
	m_wndRelaxEdit.SetWindowText(_T("5"));
	m_wndRelaxJTG04Cmb.SetWindowText(_T("1.0"));  
	OnSelchangeCmdRelaxCombo2();
}

void CCMTendonTypeAddDlg::OnCmdRelaxUserBtn() 
{
	CCMTimeDepMatFuncDlg Dlg;
	Dlg.SetCurPage(2);
	Dlg.DoModal();
}

void CCMTendonTypeAddDlg::OnCmdRelaxWobblrBtn() 
{
	int nCurSel = m_wndTendonTypeCombo.GetCurSel();// Internal-Pre
	if(nCurSel==0 || nCurSel ==2)  return;

	int nSel =0;
	CDlgUtil::CtrlRadioGetCheck(this,m_aWobbleType,  nSel);

	GetDlgItem(IDC_CMD_WOBBLE_EURO_RDO              )->EnableWindow(TRUE);
	GetDlgItem(IDC_CMD_WOBBLE_ANGLE_FACTOR_EURO_RDO )->EnableWindow(TRUE);
	GetDlgItem(IDC_CMD_WOBBLE_EURO_EDIT             )->EnableWindow(nSel==0);
	GetDlgItem(IDC_CMD_WOBBLE_EURO_UNIT             )->EnableWindow(nSel==0);
	GetDlgItem(IDC_CMD_WOBBLE_ANGLE_FACTOR_EURO_EDT )->EnableWindow(nSel==1);
	GetDlgItem(IDC_CMD_WOBBLE_ANGLE_FACTOR_EURO_UNIT)->EnableWindow(nSel==1);

	OnChangeCmdFrictionEdit();
}

void CCMTendonTypeAddDlg::OnChangeCmdFrictionEdit() 
{
	int nIndex = m_cobxRelaxMethod.GetCurSel();
	DWORD data = m_cobxRelaxMethod.GetItemData(nIndex);
	if(data == TDNT_RELAX_EUROPEAN || data == TDNT_RELAX_CEB_FIP || data == TDNT_RELAX_CEB_FIP_1990 || data == TDNT_RELAX_CEB_FIP_2010 || data == TDNT_RELAX_ABNT_NBR_6118_2023)
	{
		double dFrictionFactor = m_wndWobbleFriction.GetEditValue();
		//if(!CStrParser::GetFloatNumber(m_strFriction, dFrictionFactor)){ ASSERT(0); return;}
		if(dFrictionFactor < 0) return;
		m_TdntData.dFrictionFactor = dFrictionFactor;

		int nSel=0;
		CDlgUtil::CtrlRadioGetCheck(this, m_aWobbleType,  nSel);
		if(nSel==0) // K
		{
			double dWobble      = m_wndWobbleEURO.GetEditValue();
			double dWobbleAngle = (dFrictionFactor == 0) ? 0.0 : dWobble / dFrictionFactor;
			m_wndWobbleAngleEURO.SetEditUnit(dWobbleAngle);
		}
		else // k
		{
			double dWobbleAngle = m_wndWobbleAngleEURO.GetEditValue();
			double dWobble      = dFrictionFactor * dWobbleAngle;
			m_wndWobbleEURO.SetEditUnit(dWobble);
		}
	}

}

void CCMTendonTypeAddDlg::OnChangeCmdWobbleEuroEdit() 
{
	int nIndex = m_cobxRelaxMethod.GetCurSel();
	DWORD data = m_cobxRelaxMethod.GetItemData(nIndex);
	if(data == TDNT_RELAX_EUROPEAN || data == TDNT_RELAX_CEB_FIP || data == TDNT_RELAX_CEB_FIP_1990 || data == TDNT_RELAX_CEB_FIP_2010 || data == TDNT_RELAX_ABNT_NBR_6118_2023)
	{
		double dFrictionFactor = m_wndWobbleFriction.GetEditValue();
		//if(!CStrParser::GetFloatNumber(m_strFriction, dFrictionFactor)){ ASSERT(0); return;}
		if(dFrictionFactor < 0) return;

		double dWobble      = m_wndWobbleEURO.GetEditValue();
		double dWobbleAngle = (dFrictionFactor == 0) ? 0.0 : dWobble / dFrictionFactor;
		m_wndWobbleAngleEURO.SetEditUnit(dWobbleAngle);

	}

}

void CCMTendonTypeAddDlg::OnChangeCmdWobbleAngleEuroEdit() 
{
	int nIndex = m_cobxRelaxMethod.GetCurSel();
	DWORD data = m_cobxRelaxMethod.GetItemData(nIndex);
	if(data == TDNT_RELAX_EUROPEAN || data == TDNT_RELAX_CEB_FIP || data == TDNT_RELAX_CEB_FIP_1990 || data == TDNT_RELAX_CEB_FIP_2010 || data == TDNT_RELAX_ABNT_NBR_6118_2023)
	{
		double dFrictionFactor = m_wndWobbleFriction.GetEditValue();
		//if(!CStrParser::GetFloatNumber(m_strFriction, dFrictionFactor)){ ASSERT(0); return;}
		if(dFrictionFactor <= 0) return;

		double dWobbleAngle = m_wndWobbleAngleEURO.GetEditValue();
		double dWobble      = dFrictionFactor * dWobbleAngle;
		m_wndWobbleEURO.SetEditUnit(dWobble);

	}

}

void CCMTendonTypeAddDlg::OnSelChangeCmdDesignCode()
{
	SetDesignDia();

	CString strCode;
	int nCodeSel = m_cmbDgnCode.GetCurSel();
	if (nCodeSel != CB_ERR)
	{
		m_cmbDgnCode.GetLBText(nCodeSel, strCode);
		EnableControlsByDgnCode(strCode);
	}

	CalcTendonArea();

	//T_TENDON_AREA TendonArea;
	//TendonArea.Initialize();

	//// area
	//m_wndTArea.SetEditUnit(TendonArea.dArea);
	//m_strTotalTendonArea.Format(_T("%.10g"), TendonArea.dArea);

	//// diameter
	//int nSel = m_wndTendonTypeCombo.GetCurSel();
	//if (nSel == 0) SetDiameterField();

	//if (TendonArea.dY_Strength != 0.0 && TendonArea.dU_Strength != 0.0)
	//{
	//	// relaxation
	//	CString csRelax;
	//	csRelax.Format(_T("%.10g"), TendonArea.dR_Factor);
	//	m_wndRelaxEdit.SetWindowText(csRelax);
	//	// curvature
	//	m_strFriction.Format(_T("%.10g"), TendonArea.dC_Factor);
	//	// wobble
	//	m_wndWobble.SetEditUnit(TendonArea.dW_Factor);
	//	// ultimate
	//	m_strUltimate.Format(_T("%.10g"), TendonArea.dU_Strength);
	//	// yield
	//	m_strYield.Format(_T("%.10g"), TendonArea.dY_Strength);
	//	// anchorage slip
	//	m_wndAnchoBegin.SetEditUnit(TendonArea.dAnchorageSlip);
	//	m_wndAnchoEnd.SetEditUnit(TendonArea.dAnchorageSlip);
	//}

	//UpdateData(FALSE);
}

void CCMTendonTypeAddDlg::OnSelChangeCmdDesignDia()
{
	CalcTendonArea();
}

void CCMTendonTypeAddDlg::OnChangeCmdTendonNum()
{
	UpdateData(TRUE);

	if (!m_strAreaNum.IsEmpty())
	{
		CalcTendonArea();
	}
}

void CCMTendonTypeAddDlg::OnDeltaposCmdTendonNumSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
	CDlgUtil::SetSpinValue(this, IDC_CMD_STRAND_NUM_EDIT, 1, -1, pNMHDR, pResult);
}

void CCMTendonTypeAddDlg::ReplaceMMRusToEng(CString& str)
{
	str.Replace(D_UNITSYS_LENGTH_NAME_MM, _T("mm"));
}

void CCMTendonTypeAddDlg::ReplaceMMEngToRus(CString& str)
{
	str.Replace(_T("mm"), D_UNITSYS_LENGTH_NAME_MM);
}
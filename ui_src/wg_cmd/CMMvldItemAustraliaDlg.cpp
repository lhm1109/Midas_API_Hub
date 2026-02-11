// CMMvldItemAustraliaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldItemAustraliaDlg.h"
#include "CMMvldSubItemDlg.h"

#include "..\wg_base\wg_base_TestEnvMgr.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\VehlDef.h"
#include "..\wg_db\VehlDB.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  AS_General 0
#define  AS_Fatigue 1
#define  AS_Heavy   2
#define  AS_Rail    3
#define  AS_Double  4
#define  AS_Group1  5
#define  AS_Group2  6

union T_MVLD_TYPE_KEY
{
	unsigned int value;
	struct
	{
		unsigned int type : 1;
		unsigned int key  : 31;
	} typekey;
};

/////////////////////////////////////////////////////////////////////////////
// Australia dialog

CCMMvldItemAustraliaDlg::CCMMvldItemAustraliaDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldItemAustraliaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvldItemAustraliaDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDoc = NULL;

	m_Data.Initialize();
	
	m_bModify = FALSE;
	m_bSurface= FALSE;
	m_nCodeType=0;
	
	m_bPermit = FALSE;
	m_bAutoOptim = FALSE;
	m_bFatigue = FALSE;
	m_nCombOption = 1;
	m_nCombOption2 = 1;
	
	m_aLaneList.RemoveAll();
	m_aSelFlag.RemoveAll();
	m_aSelLlan.RemoveAll();


// 	m_aFatigue.RemoveAll();
// 	m_aFatigue.Add(IDC_CMD_MVLD_FATIGUE_CHK);

	m_aModelType.RemoveAll();
	m_aModelType.Add(IDC_SEL_LOAD_MODEL_GRP);
	m_aModelType.Add(IDC_GENERAL_LOAD_RDO);
	m_aModelType.Add(IDC_FATIGUE_LOAD_RDO);
	m_aModelType.Add(IDC_HEAVY_LOAD_RDO);
	m_aModelType.Add(IDC_RAIL_LOAD_RDO);
	m_aModelType.Add(IDC_B_DOUBLE_LOAD_RDO);

	m_aDgnComb.RemoveAll();
	m_aDgnComb.Add(IDC_CMD_FACTOR_GRP);
	m_aDgnComb.Add(IDC_CMD_ULTIMATE_RADIO);
	m_aDgnComb.Add(IDC_CMD_SERVICE_RADIO);

	m_aLaneFactor.RemoveAll();
	m_aLaneFactor.Add(IDC_CMD_MVLD_LANE_FACT_GRP);
	m_aLaneFactor.Add(IDC_CMD_MVLD_NUM_LANE_STC);
	m_aLaneFactor.Add(IDC_CMD_MVLD_NUM_LANE1_STC);
	m_aLaneFactor.Add(IDC_CMD_MVLD_NUM_LANE2_STC);
	m_aLaneFactor.Add(IDC_CMD_MVLD_NUM_LANE3_STC);
	m_aLaneFactor.Add(IDC_CMD_MVLD_SACLE_FACT_STC);
	m_aLaneFactor.Add(IDC_CMD_MVLD_SACLE_FACT1_EDT);
	m_aLaneFactor.Add(IDC_CMD_MVLD_SACLE_FACT2_EDT);
	m_aLaneFactor.Add(IDC_CMD_MVLD_SACLE_FACT3_EDT);
	m_aLaneFactorAdd.RemoveAll();
	m_aLaneFactorAdd.Add(IDC_CMD_MVLD_NUM_LANE4_STC);
	m_aLaneFactorAdd.Add(IDC_CMD_MVLD_NUM_LANE5_STC);
	m_aLaneFactorAdd.Add(IDC_CMD_MVLD_SACLE_FACT4_EDT);
	m_aLaneFactorAdd.Add(IDC_CMD_MVLD_SACLE_FACT5_EDT);
		

	m_aLaneFactorFold.RemoveAll();
	m_aLaneFactorFold.Add(IDC_CMD_MVLD_NUM_LANE_STC);
	m_aLaneFactorFold.Add(IDC_CMD_MVLD_NUM_LANE1_STC);
	m_aLaneFactorFold.Add(IDC_CMD_MVLD_NUM_LANE2_STC);
	m_aLaneFactorFold.Add(IDC_CMD_MVLD_NUM_LANE3_STC);
	m_aLaneFactorFold.Add(IDC_CMD_MVLD_SACLE_FACT_STC);
	m_aLaneFactorFold.Add(IDC_CMD_MVLD_SACLE_FACT1_EDT);
	m_aLaneFactorFold.Add(IDC_CMD_MVLD_SACLE_FACT2_EDT);
	m_aLaneFactorFold.Add(IDC_CMD_MVLD_SACLE_FACT3_EDT);
	m_aLaneFactorFold.Add(IDC_CMD_MVLD_NUM_LANE4_STC);
	m_aLaneFactorFold.Add(IDC_CMD_MVLD_NUM_LANE5_STC);
	m_aLaneFactorFold.Add(IDC_CMD_MVLD_SACLE_FACT4_EDT);
	m_aLaneFactorFold.Add(IDC_CMD_MVLD_SACLE_FACT5_EDT);

	m_aSubCase.RemoveAll();
	m_aSubCase.Add(IDC_CMD_MVLD_SUB_CASE_GRP);
	m_aSubCase.Add(IDC_CMD_MVLD_LOAD_EFFECT_GRP);
	m_aSubCase.Add(IDC_CMD_MVLD_COMBINED_RDO);
	m_aSubCase.Add(IDC_CMD_MVLD_INDEPENDENT_RDO);
	m_aSubCase.Add(IDC_CMD_MVLD_SUB_CASE_LST);
	m_aSubCase.Add(IDC_CMD_MVLD_SUB_CASE_ADD_BTN);
	m_aSubCase.Add(IDC_CMD_MVLD_SUB_CASE_MOD_BTN);
	m_aSubCase.Add(IDC_CMD_MVLD_SUB_CASE_DEL_BTN);
	
	m_aPermit.RemoveAll();
	m_aPermit.Add(IDC_CMD_MVLD_PERMIT_GRP);
	m_aPermit.Add(IDC_CMD_MVLD_PERMIT_STC);
	m_aPermit.Add(IDC_CMD_MVLD_PERMIT_CMB);
	m_aPermit.Add(IDC_CMD_MVLD_PERMIT_REF_LANE_STC);
	m_aPermit.Add(IDC_CMD_MVLD_PERMIT_REF_LANE_CMB);
	m_aPermit.Add(IDC_CMD_MVLD_PERMIT_ECCEN_STC);
	m_aPermit.Add(IDC_CMD_MVLD_PERMIT_ECCEN_EDT);
	m_aPermit.Add(IDC_CMD_MVLD_PERMIT_ECCEN_UNT);
	m_aPermit.Add(IDC_CMD_MVLD_PERMIT_FACT_STC);
	m_aPermit.Add(IDC_CMD_MVLD_PERMIT_FACT_EDT);
		
	m_aOptim01.RemoveAll();
	m_aOptim01.Add(IDC_CMD_MVLD_OPTIM_GRP);
	m_aOptim01.Add(IDC_CMD_MVLD_MIN_VEHL_DIST_STC);
	m_aOptim01.Add(IDC_CMD_MVLD_MIN_VEHL_DIST_EDT);
	m_aOptim01.Add(IDC_CMD_MVLD_MIN_VEHL_DIST_UNT);
	m_aOptim02.RemoveAll();
	m_aOptim02.Add(IDC_CMD_MVLD_LCASE_GRP);
	m_aOptim02.Add(IDC_CMD_MVLD_LANE_STC);
	m_aOptim02.Add(IDC_CMD_MVLD_LANE_CMB);
	m_aOptim02.Add(IDC_CMD_MVLD_MIN_NUM_VEHL_STC);
	m_aOptim02.Add(IDC_CMD_MVLD_MIN_NUM_VEHL_EDT);
	m_aOptim02.Add(IDC_CMD_MVLD_MAX_NUM_VEHL_STC);
	m_aOptim02.Add(IDC_CMD_MVLD_MAX_NUM_VEHL_EDT);
	m_aOptim02.Add(IDC_CMD_MVLD_LOAD_EFFECT_GRP2);
	m_aOptim02.Add(IDC_CMD_MVLD_COMBINED_RDO2);
	m_aOptim02.Add(IDC_CMD_MVLD_INDEPENDENT_RDO2);
	m_aOptim02.Add(IDC_CMD_MVLD_ASSIGN_VEHL_GRP);
	m_aOptim02.Add(IDC_CMD_MVLD_SEL_VEHL_STC);
	m_aOptim02.Add(IDC_CMD_MVLD_SEL_VEHL_CMB);
	m_aOptim02.Add(IDC_CMD_MVLD_SCALE_FACTOR_STC);
	m_aOptim02.Add(IDC_CMD_MVLD_SCALE_FACTOR_EDT);
	m_aOptim02.Add(IDC_CMD_MVLD_ASSIGN_VEHL_LST);
	m_aOptim02.Add(IDC_CMD_MVLD_ASSIGN_VEHL_ADD_BTN);
	m_aOptim02.Add(IDC_CMD_MVLD_ASSIGN_VEHL_MOD_BTN);
	m_aOptim02.Add(IDC_CMD_MVLD_ASSIGN_VEHL_DEL_BTN);
	m_aOptim.RemoveAll();
	m_aOptim.Append(m_aOptim01);
	m_aOptim.Append(m_aOptim02);

	m_aLaneFactor2.RemoveAll();
	m_aLaneFactor2.Add(IDC_CMD_MVLD_LANE_FACT_GRP2);
	m_aLaneFactor2.Add(IDC_CMD_MVLD_LANE_FACTOR_STC);
	m_aLaneFactor2.Add(IDC_CMD_MVLD_SACLE_FACT_MS1600_EDT);   // IDC_CMD_MVLD_LANE_FACTOR_CMB

	m_aLCase2.RemoveAll();
	m_aLCase2.Add(IDC_CMD_MVLD_LCASE2_GRP);
	m_aLCase2.Add(IDC_CMD_LOAD_HEAVY_STC); 
	m_aLCase2.Add(IDC_CMD_LOAD_HEAVY_CMB);
	m_aLCase2.Add(IDC_CMD_LOAD_MS1600_STC);
	m_aLCase2.Add(IDC_CMD_LOAD_MS1600_CMB);

	m_aBDoubleCmb.RemoveAll();
	m_aBDoubleCmb.Add(IDC_CMD_LOAD_BDOUBLE_STC);
	m_aBDoubleCmb.Add(IDC_CMD_LOAD_BDOUBLE_CMB);
		
	m_aBDoubleList.RemoveAll();
	m_aBDoubleList.Add(IDC_CMD_BDOUBLE_TXT);
	m_aBDoubleList.Add(IDC_CMD_BDOUBLE_LIST);

	m_aGroup2Cmb.RemoveAll();
	m_aGroup2Cmb.Add(IDC_CMD_LOAD_WA_RATING_GROUP2_STC);

	m_aGroup2List.RemoveAll();
	m_aGroup2List.Add(IDC_CMD_WA_RATING_GROUP2_TXT);

	m_aAssinLane01.RemoveAll();
	m_aAssinLane01.Add(IDC_CMD_LANES_GRUP);
	m_aAssinLane01.Add(IDC_CMD_MVLD_LANE_MS1600_STC);
	m_aAssinLane01.Add(IDC_CMD_MIN_NUM_STC);
	m_aAssinLane01.Add(IDC_CMD_MAX_NUM_STC);
	m_aAssinLane01.Add(IDC_CMD_MIN_NUM_EDT);
	m_aAssinLane01.Add(IDC_CMD_MAX_NUM_EDT);
	m_aAssinLane02.RemoveAll();
	m_aAssinLane02.Add(IDC_CMD_LIST_TXT);
	m_aAssinLane02.Add(IDC_CMD_SELECTED_TXT);
	m_aAssinLane02.Add(IDC_CMD_HEAVY_TXT);
	m_aAssinLane02.Add(IDC_CMD_BDOUBLE_TXT);
	m_aAssinLane02.Add(IDC_CMD_WA_RATING_GROUP2_TXT);
	m_aAssinLane02.Add(IDC_CMD_UNSEL_LIST);
	m_aAssinLane02.Add(IDC_CMD_SEL_LIST);
	m_aAssinLane02.Add(IDC_CMD_HEAVY_LIST);
	m_aAssinLane02.Add(IDC_CMD_BDOUBLE_LIST);
	m_aAssinLane02.Add(IDC_CMD_SUB_BTN_ADD);
	m_aAssinLane02.Add(IDC_CMD_SUB_BTN_DEL);
	m_aAssinLane02.Add(IDC_CMD_SUB_BTN_ADD2);
	m_aAssinLane02.Add(IDC_CMD_SUB_BTN_DEL2);
	m_aAssinLane03.RemoveAll();
	m_aAssinLane03.Add(IDC_CMD_MVLD_LANE2_STC);
	m_aAssinLane03.Add(IDC_CMD_MVLD_LANE2_CMB);
	m_aAssinLane.RemoveAll();
	m_aAssinLane.Append(m_aAssinLane01);
	m_aAssinLane.Append(m_aAssinLane02);
	m_aAssinLane.Append(m_aAssinLane03);


	m_aMultiLaneFactorFold.RemoveAll();
	m_aMultiLaneFactorFold.Add(IDC_CMD_MVLD_AS_MULT_LANE_STC);
	m_aMultiLaneFactorFold.Add(IDC_CMD_MVLD_AS_MULT_LANE1_STC);
	m_aMultiLaneFactorFold.Add(IDC_CMD_MVLD_AS_MULT_LANE2_STC);
	m_aMultiLaneFactorFold.Add(IDC_CMD_MVLD_AS_MULT_LANE3_STC);
	m_aMultiLaneFactorFold.Add(IDC_CMD_MVLD_AS_MULT_LANE4_STC);
	m_aMultiLaneFactorFold.Add(IDC_CMD_MVLD_AS_MULT_LANE5_STC);
	m_aMultiLaneFactorFold.Add(IDC_CMD_MVLD_AS_MULT_LANE6_STC);
	m_aMultiLaneFactorFold.Add(IDC_CMD_MVLD_AS_MULT_SACLE_FACT_STC);
	m_aMultiLaneFactorFold.Add(IDC_CMD_MVLD_AS_MULT_SACLE_FACT1_EDT);
	m_aMultiLaneFactorFold.Add(IDC_CMD_MVLD_AS_MULT_SACLE_FACT2_EDT);
	m_aMultiLaneFactorFold.Add(IDC_CMD_MVLD_AS_MULT_SACLE_FACT3_EDT);
	m_aMultiLaneFactorFold.Add(IDC_CMD_MVLD_AS_MULT_SACLE_FACT4_EDT);
	m_aMultiLaneFactorFold.Add(IDC_CMD_MVLD_AS_MULT_SACLE_FACT5_EDT);
	m_aMultiLaneFactorFold.Add(IDC_CMD_MVLD_AS_MULT_SACLE_FACT6_EDT);

	m_aMultiLaneFactorList.RemoveAll();
	m_aMultiLaneFactorList.Add(IDC_CMD_MVLD_AS_MULT_LANE_FACT_GRP);
	m_aMultiLaneFactorList.Append(m_aMultiLaneFactorFold);

	m_aOkCancel.RemoveAll();
	m_aOkCancel.Add(IDOK);
	m_aOkCancel.Add(IDCANCEL);
	m_aOkCancel.Add(IDC_CMD_APPLY);		
}

CCMMvldItemAustraliaDlg::~CCMMvldItemAustraliaDlg()
{

}


void CCMMvldItemAustraliaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldItemAustraliaDlg)
	DDX_Control(pDX, IDC_CMD_MVLD_NAME_EDT,            m_edtLoadCase);
	DDX_Control(pDX, IDC_CMD_MVLD_DESC_EDT,            m_edtDesc);
	DDX_Check  (pDX, IDC_CMD_MVLD_PERMIT_CHK,          m_bPermit);
	DDX_Check  (pDX, IDC_CMD_MVLD_OPTIM_CHK,           m_bAutoOptim);
	//DDX_Check  (pDX, IDC_CMD_MVLD_FATIGUE_CHK,         m_bFatigue);
	DDX_Control(pDX, IDC_CMD_MVLD_SACLE_FACT1_EDT,     m_edtLaneFact1);
	DDX_Control(pDX, IDC_CMD_MVLD_SACLE_FACT2_EDT,     m_edtLaneFact2);
	DDX_Control(pDX, IDC_CMD_MVLD_SACLE_FACT3_EDT,     m_edtLaneFact3);	
	DDX_Control(pDX, IDC_CMD_MVLD_SACLE_FACT4_EDT,     m_edtLaneFact4);	
	DDX_Control(pDX, IDC_CMD_MVLD_SACLE_FACT5_EDT,     m_edtLaneFact5);	
	DDX_Radio  (pDX, IDC_CMD_MVLD_COMBINED_RDO,        m_nCombOption);
	DDX_Control(pDX, IDC_CMD_MVLD_SUB_CASE_LST,        m_lstSubCase);
	DDX_Control(pDX, IDC_CMD_MVLD_PERMIT_CMB,          m_cmbPermit);
	DDX_Control(pDX, IDC_CMD_MVLD_PERMIT_REF_LANE_CMB, m_cmbRefLane);
	DDX_Control(pDX, IDC_CMD_MVLD_PERMIT_ECCEN_EDT,    m_edtEccen);
	DDX_Control(pDX, IDC_CMD_MVLD_PERMIT_ECCEN_UNT,    m_untEccen);
	DDX_Control(pDX, IDC_CMD_MVLD_PERMIT_FACT_EDT,     m_edtScaleFactor);
	DDX_Control(pDX, IDC_CMD_MVLD_MIN_VEHL_DIST_EDT,   m_edtMinVehlDist);
	DDX_Control(pDX, IDC_CMD_MVLD_MIN_VEHL_DIST_UNT,   m_untMinVehlDist);
	DDX_Control(pDX, IDC_CMD_MVLD_LANE_CMB,            m_cmbOptimLane);
	DDX_Control(pDX, IDC_CMD_MVLD_MIN_NUM_VEHL_EDT,    m_edtMinNumVehl);
	DDX_Control(pDX, IDC_CMD_MVLD_MAX_NUM_VEHL_EDT,    m_edtMaxNumVehl);
	DDX_Radio  (pDX, IDC_CMD_MVLD_COMBINED_RDO2,       m_nCombOption2);
	DDX_Control(pDX, IDC_CMD_MVLD_SEL_VEHL_CMB,        m_cmbSelVehl);
	DDX_Control(pDX, IDC_CMD_MVLD_SCALE_FACTOR_EDT,    m_edtScaleFactorOptim);
	DDX_Control(pDX, IDC_CMD_MVLD_ASSIGN_VEHL_LST,     m_lstAssignVehl);

	DDX_Control(pDX, IDC_CMD_MVLD_SACLE_FACT_MS1600_EDT,m_edtLaneFact_MS1600);	
	DDX_Control(pDX, IDC_CMD_MIN_NUM_EDT,               m_edtMinNumLane);	
	DDX_Control(pDX, IDC_CMD_MAX_NUM_EDT,               m_edtMaxNumLane);	
	DDX_Control(pDX, IDC_CMD_MVLD_LANE2_CMB,            m_cmbOptimLane2);

	DDX_Control(pDX, IDC_CMD_LOAD_HEAVY_CMB,            m_cbxHeavyLoadVehi);  // heavy
	DDX_Control(pDX, IDC_CMD_LOAD_MS1600_CMB,           m_cbxMS1600LoadVehi);   // M1600 S1600 
	DDX_Control(pDX, IDC_CMD_LOAD_BDOUBLE_CMB,          m_cbxDoubleVehi);  // B Double

	DDX_Control(pDX, IDC_CMD_UNSEL_LIST,                m_wndUnselList);
	DDX_Control(pDX, IDC_CMD_SEL_LIST,                  m_wndSelList);
	DDX_Control(pDX, IDC_CMD_HEAVY_LIST,                m_lstHeavy);
	DDX_Control(pDX, IDC_CMD_BDOUBLE_LIST,              m_lstDouble);

	DDX_Control(pDX, IDC_CMD_MVLD_AS_MULT_SACLE_FACT1_EDT, m_edtMultiLaneFact1);
	DDX_Control(pDX, IDC_CMD_MVLD_AS_MULT_SACLE_FACT2_EDT, m_edtMultiLaneFact2);
	DDX_Control(pDX, IDC_CMD_MVLD_AS_MULT_SACLE_FACT3_EDT, m_edtMultiLaneFact3);
	DDX_Control(pDX, IDC_CMD_MVLD_AS_MULT_SACLE_FACT4_EDT, m_edtMultiLaneFact4);
	DDX_Control(pDX, IDC_CMD_MVLD_AS_MULT_SACLE_FACT5_EDT, m_edtMultiLaneFact5);
	DDX_Control(pDX, IDC_CMD_MVLD_AS_MULT_SACLE_FACT6_EDT, m_edtMultiLaneFact6);

	DDX_Control(pDX, IDC_SEL_LOAD_MODEL_GRP, m_chkLoadModel);
	DDX_Control(pDX, IDC_CMD_MVLD_AS_MULT_LANE_FACT_GRP, m_chkMultiLaneFactor);

	//}}AFX_DATA_MAP
}

//-------------------------------------------------------------------------
// Implementation
void CCMMvldItemAustraliaDlg::InitCtrl()
{
	// Rdo
	m_aLoadModelRdo.RemoveAll();
	m_aLoadModelRdo.Add(IDC_GENERAL_LOAD_RDO);
	m_aLoadModelRdo.Add(IDC_FATIGUE_LOAD_RDO);
	m_aLoadModelRdo.Add(IDC_HEAVY_LOAD_RDO);
	m_aLoadModelRdo.Add(IDC_RAIL_LOAD_RDO);
	m_aLoadModelRdo.Add(IDC_B_DOUBLE_LOAD_RDO);
	m_aLoadModelRdo.Add(IDC_WA_RATING_GROUP1_LOAD_RDO);
	m_aLoadModelRdo.Add(IDC_WA_RATING_GROUP2_LOAD_RDO);

	m_aDgnCombRdo.RemoveAll();
	m_aDgnCombRdo.Add(IDC_CMD_ULTIMATE_RADIO);
	m_aDgnCombRdo.Add(IDC_CMD_SERVICE_RADIO);
}

void CCMMvldItemAustraliaDlg::InitUnit()
{
	m_edtLaneFact1.SetUnitType(D_UNITSYS_NONE);
	m_edtLaneFact2.SetUnitType(D_UNITSYS_NONE);
	m_edtLaneFact3.SetUnitType(D_UNITSYS_NONE);
	m_edtLaneFact4.SetUnitType(D_UNITSYS_NONE);
	m_edtLaneFact5.SetUnitType(D_UNITSYS_NONE);
	m_edtEccen.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dEccentricity);
	m_untEccen.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dEccentricity);
	m_edtScaleFactor.SetUnitType(D_UNITSYS_NONE);
	m_edtMinVehlDist.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
	m_untMinVehlDist.SetUnitType(CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
	m_edtMinNumVehl.SetUnitType(D_UNITSYS_NONE);
	m_edtMaxNumVehl.SetUnitType(D_UNITSYS_NONE);
	m_edtScaleFactorOptim.SetUnitType(D_UNITSYS_NONE);	

	m_edtLaneFact_MS1600.SetUnitType(D_UNITSYS_NONE);
	m_edtMinNumLane.SetEditUnit(D_UNITSYS_NONE);
	m_edtMaxNumLane.SetEditUnit(D_UNITSYS_NONE);

	m_edtMultiLaneFact1.SetUnitType(D_UNITSYS_NONE);
	m_edtMultiLaneFact2.SetUnitType(D_UNITSYS_NONE);
	m_edtMultiLaneFact3.SetUnitType(D_UNITSYS_NONE);
	m_edtMultiLaneFact4.SetUnitType(D_UNITSYS_NONE);
	m_edtMultiLaneFact5.SetUnitType(D_UNITSYS_NONE);
	m_edtMultiLaneFact6.SetUnitType(D_UNITSYS_NONE);

}

void CCMMvldItemAustraliaDlg::InitCombo()
{
	T_MVHC_D MvhcD; MvhcD.Initialize();
	T_MVHL_D MvhlD; MvhlD.Initialize();
	CArray<T_MVHC_K, T_MVHC_K> aMvhcK; aMvhcK.RemoveAll();
	CArray<T_MVHL_K, T_MVHL_K> aMvhlK; aMvhlK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetMvhcKeyList(aMvhcK);
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(aMvhlK);

	int nIndex = 0;
	T_MVLD_TYPE_KEY TypeKey;
	
	// Permit Vehicle
	m_cmbPermit.ResetContent();

	for(int i=0; i<aMvhlK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD)) continue;
		if(!m_pDoc->m_pAttrCtrl->IsPermitVehicle(MvhlD)) continue;

		nIndex = m_cmbPermit.AddString(MvhlD.VehicleLoadName);
		m_cmbPermit.SetItemData(nIndex, aMvhlK[i]);
	}
	m_cmbPermit.SetCurSel(0);

	// Ref. Lane
	m_cmbRefLane.ResetContent();

	if(m_bSurface)
	{
		T_SLAN_D SlanD; SlanD.Initialize();
		CArray<T_SLAN_K, T_SLAN_K> aSlanK; aSlanK.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetSlanKeyList(aSlanK);

		for(int i=0; i<aSlanK.GetSize(); i++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetSlan(aSlanK[i], SlanD)) continue;
			m_cmbRefLane.SetItemData(m_cmbRefLane.AddString(SlanD.LaneName), aSlanK[i]);
		}
	}
	else
	{
		T_LLAN_D LlanD; LlanD.Initialize();
		CArray<T_LLAN_K, T_LLAN_K> aLlanK; aLlanK.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetLlanKeyList(aLlanK);

		for(int i=0; i<aLlanK.GetSize(); i++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetLlan(aLlanK[i], LlanD)) continue;
			m_cmbRefLane.SetItemData(m_cmbRefLane.AddString(LlanD.LineLaneName), aLlanK[i]);
		}
	}

	m_cmbRefLane.SetCurSel(0);
	
	// Loaded Lane
	m_cmbOptimLane.ResetContent();
	m_cmbOptimLane2.ResetContent();

	if(m_bSurface)
	{
		T_SLANop_D SlanD; SlanD.Initialize();
		CArray<T_SLANop_K, T_SLANop_K> aSlanK; aSlanK.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetSlanopKeyList(aSlanK);

		for(int i=0; i<aSlanK.GetSize(); i++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetSlanop(aSlanK[i], SlanD)) continue;	
			if(SlanD.bStradd) 
			{
				m_cmbOptimLane2.SetItemData(m_cmbOptimLane2.AddString(SlanD.LaneName), aSlanK[i]);
			}
			else
			{
				m_cmbOptimLane.SetItemData(m_cmbOptimLane.AddString(SlanD.LaneName), aSlanK[i]);
			}

		}
	}
	else
	{
		T_LLANop_D LlanD; LlanD.Initialize();
		CArray<T_LLANop_K, T_LLANop_K> aLlanK; aLlanK.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetLlanopKeyList(aLlanK);

		for(int i=0; i<aLlanK.GetSize(); i++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetLlanop(aLlanK[i], LlanD)) continue;
		
			if(LlanD.bStradd) 
			{
				m_cmbOptimLane2.SetItemData(m_cmbOptimLane2.AddString(LlanD.LineLaneName), aLlanK[i]);
			}
			else
			{
				m_cmbOptimLane.SetItemData(m_cmbOptimLane.AddString(LlanD.LineLaneName), aLlanK[i]);
			}
		}
	}

	m_cmbOptimLane.SetCurSel(0);
	m_cmbOptimLane2.SetCurSel(0);
		
	// Selected Vehicle
	m_cmbSelVehl.ResetContent();


	// Fatigue 옵션 어떻게 처리 할지 확인 필요
	for(int i=0; i<aMvhcK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetMvhc(aMvhcK[i], MvhcD)) continue;
		if(MvhcD.arVehicleLoad.GetSize() == 0) continue;
		if(!m_pDoc->m_pAttrCtrl->GetMvhl(MvhcD.arVehicleLoad[0], MvhlD)) continue; // Vehicle Class 에서 Check 했으니까 한개만 Check

		if(m_bFatigue)
		{
			if(!MvhlD.bFatigue) continue;
		}
		else
		{
			if(MvhlD.bFatigue) continue;
		}

		nIndex = m_cmbSelVehl.AddString(_T("VC:")+MvhcD.VehicleClassName);
		TypeKey.typekey.type = 0;
		TypeKey.typekey.key = aMvhcK[i];
		m_cmbSelVehl.SetItemData(nIndex, TypeKey.value);
	}

	for(int i=0; i<aMvhlK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD)) continue;
		if( m_pDoc->m_pAttrCtrl->IsPermitVehicle(MvhlD)) continue;

		if(m_bFatigue)
		{
			if(!MvhlD.bFatigue) continue;
		}
		else
		{
			if(MvhlD.bFatigue) continue;
		}
		
		nIndex = m_cmbSelVehl.AddString(_T("VL:")+MvhlD.VehicleLoadName);
		TypeKey.typekey.type = 1;
		TypeKey.typekey.key = aMvhlK[i];
		m_cmbSelVehl.SetItemData(nIndex, TypeKey.value);
	}

	m_cmbSelVehl.SetCurSel(0);


	if(m_Data.nLoadModel==AS_Heavy || m_Data.nLoadModel == AS_Double || m_Data.nLoadModel == AS_Group2)
	{
		InitComboheavy();
	}

}

void CCMMvldItemAustraliaDlg::InitComboheavy()
{
	if(m_Data.nLoadModel!=AS_Heavy && m_Data.nLoadModel != AS_Double && m_Data.nLoadModel != AS_Group2) return;

	m_cbxHeavyLoadVehi.ResetContent();
	m_cbxMS1600LoadVehi.ResetContent();
	m_cbxDoubleVehi.ResetContent();
	CDlgUtil::CobxAddItem(m_cbxMS1600LoadVehi, _LS(IDS_WG_CMD__ADDD__None), 0);

	// Item 추가
	CArray<T_MVHL_K, T_MVHL_K> aMvhlK;
	m_pDoc->m_pAttrCtrl->GetMvhlKeyList(aMvhlK);
	T_MVHL_D MvhlD;
	for (int i = 0; i < aMvhlK.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetMvhl(aMvhlK[i], MvhlD);

		if (m_Data.nLoadModel == AS_Heavy)
		{
			if (MvhlD.bStandard && MvhlD.nStandardCode == D_MVHL_AS_5100_7_HEAVY &&
				(MvhlD.VehicleTypeName == _T("HLP320") || MvhlD.VehicleTypeName == _T("HLP400")))
			{
				CDlgUtil::CobxAddItem(m_cbxHeavyLoadVehi, MvhlD.VehicleLoadName, aMvhlK[i]);
			}
		}
		else if (m_Data.nLoadModel == AS_Group2)
		{
			if (MvhlD.bStandard && MvhlD.nStandardCode == D_MVHL_AS_5100_7_ROAD &&
				(	MvhlD.VehicleTypeName == _T("WA-Group2-Vehicle1 (3.01m O/A)") || 
					MvhlD.VehicleTypeName == _T("WA-Group2-Vehicle1 (3.70m O/A)") || 
					MvhlD.VehicleTypeName == _T("WA-Group2-Vehicle2 (3.01m O/A)") || 
					MvhlD.VehicleTypeName == _T("WA-Group2-Vehicle2 (3.70m O/A)") || 
					MvhlD.VehicleTypeName == _T("WA-Group2-Vehicle4") || 
					MvhlD.VehicleTypeName == _T("WA-Group2-Vehicle4 (Non-Supervised)") || 
					MvhlD.VehicleTypeName == _T("WA-Group2-Vehicle5") || 
					MvhlD.VehicleTypeName == _T("WA-Group2-Vehicle5 (Non-Supervised)") || 
					MvhlD.VehicleTypeName == _T("WA-Group2-Vehicle7") || 
					MvhlD.VehicleTypeName == _T("WA-Group2-Vehicle8")))
			{
				CDlgUtil::CobxAddItem(m_cbxHeavyLoadVehi, MvhlD.VehicleLoadName, aMvhlK[i]);
			}
		}


		if(MvhlD.bStandard && MvhlD.nStandardCode == D_MVHL_AUSTRALIA_AS_5100_2 && 
			(MvhlD.VehicleTypeName == _T("M1600") || MvhlD.VehicleTypeName == _T("M1600 without UDL") || MvhlD.VehicleTypeName == _T("M1600 tri-axle group")  ||
			MvhlD.VehicleTypeName == _T("S1600")))
		{
			CDlgUtil::CobxAddItem(m_cbxMS1600LoadVehi, MvhlD.VehicleLoadName, aMvhlK[i]);
		}

		if (m_pDoc->m_pAttrCtrl->IsBDoubleVehicle(MvhlD))
		{
			CDlgUtil::CobxAddItem(m_cbxDoubleVehi, MvhlD.VehicleLoadName, aMvhlK[i]);
		}
	}

	if (m_cbxHeavyLoadVehi.GetCount() > 0) m_cbxHeavyLoadVehi.SetCurSel(0);
	if (m_cbxMS1600LoadVehi.GetCount() > 0) m_cbxMS1600LoadVehi.SetCurSel(0);
	if (m_cbxDoubleVehi.GetCount() > 0) m_cbxDoubleVehi.SetCurSel(0);
}

void CCMMvldItemAustraliaDlg::Data2Dlg_LaneListByVehicleIndex()
{
	if (m_pDoc->m_pAttrCtrl->GetCountSlan() > 0)
	{
		m_pDoc->m_pAttrCtrl->GetSlanKeyList(m_aLaneList);
	}
	else
	{
		m_pDoc->m_pAttrCtrl->GetLlanKeyList(m_aLaneList);
	}

	int nAllSize = m_aLaneList.GetSize();
	int nSelSize = 0;
	for (nSelSize = 0; nSelSize < m_Data.aNALanes.GetSize(); nSelSize++)
	{
		if (m_Data.aNALanes[nSelSize] == 0) break;
	}
 
	int nStrdSize = 0;
	for (nStrdSize = 0; nStrdSize < m_Data.aStraddLanes.GetSize(); nStrdSize++)
	{
		if (m_Data.aStraddLanes[nStrdSize].Lane1 == 0 || m_Data.aStraddLanes[nStrdSize].Lane2 == 0) break;
	}

	int nBDSize = 0;
	for (nBDSize = 0; nBDSize < m_Data.aFootwayLanes.GetSize(); nBDSize++)
	{
		if (m_Data.aFootwayLanes[nBDSize] == 0) break;
	}
 
 	m_aSelFlag.SetSize(nAllSize);
 
 	CArray<BOOL, BOOL> aStrdFlag;
 	aStrdFlag.SetSize(nAllSize);

	m_aBDFlag.SetSize(nAllSize);

	for (int i=0; i<nAllSize; i++)
	{
		BOOL bFoundInSel = FALSE;
		BOOL bFoundInStrd = FALSE;
		BOOL bFoundInBD   = FALSE;

		for (int j=0; j<nSelSize; j++)
		{
			if (m_aLaneList[i] == m_Data.aNALanes[j]) 
			{
				bFoundInSel = TRUE; break;
			}
		}
		for (int j=0 ; j<nStrdSize ; ++j)
		{
			if (m_aLaneList[i] == m_Data.aStraddLanes[j].Lane1 || m_aLaneList[i] == m_Data.aStraddLanes[j].Lane2)
			{
				bFoundInStrd = TRUE; break;
			}
		}
		for (int j = 0; j < nBDSize; j++)
		{
			if (m_aLaneList[i] == m_Data.aFootwayLanes[j])
			{
				bFoundInBD = TRUE; break;
			}
		}

		m_aSelFlag[i] = bFoundInSel;
		aStrdFlag[i] = bFoundInStrd;
		m_aBDFlag[i] = bFoundInBD;
	}

	int nIndex;

	m_wndSelList.ResetContent();
	m_wndUnselList.ResetContent();
	m_lstHeavy.ResetContent();
	m_lstDouble.ResetContent();

	// Add String
	CString LaneName;
	for (int i = 0; i < nAllSize; i++)
	{
		LaneName = GetLaneName(m_aLaneList[i]);
		if (m_aSelFlag[i]) 
		{
			nIndex = m_wndSelList.AddString(LaneName);
			m_wndSelList.SetItemData(nIndex, i);
		}
		else if (aStrdFlag[i])
		{
			continue;
		}
		else
		{
			nIndex = m_wndUnselList.AddString(LaneName);
			m_wndUnselList.SetItemData(nIndex, i);
		}

		if (m_aBDFlag[i]) // B Double
		{
			for (int j = 0; j < m_Data.aFootwayLanes.GetSize(); j++)
			{
				if (m_Data.aFootwayLanes[j] == m_aLaneList[i])
				{
					T_LLAN_EURO_NEW_KEY key;
					key.key.key1 = m_Data.aFootwayLanes[j];
					key.key.key2 = 0;

					nIndex = m_lstDouble.AddString(LaneName);
					m_lstDouble.SetItemData(nIndex, i);

					m_aSelLlan.Add(key);
				}
			}
		}

	}

	T_LLAN_EURO_NEW_KEY key;
	// Heavy
	for (int i=0; i<m_Data.aStraddLanes.GetSize(); i++)
	{
		if (m_Data.aStraddLanes[i].Lane1 == 0 || m_Data.aStraddLanes[i].Lane2 == 0) break;

		key.key.key1 = m_Data.aStraddLanes[i].Lane1;
		key.key.key2 = m_Data.aStraddLanes[i].Lane2;

		LaneName = GetLaneName(key.key.key1);
		LaneName += _T(" : ");
		LaneName += GetLaneName(key.key.key2);
		nIndex = m_lstHeavy.AddString(LaneName);
		m_lstHeavy.SetItemData(nIndex, key.keymap);

		m_aSelLlan.Add(key);
	}
}
BOOL CCMMvldItemAustraliaDlg::IsDuplicatedItem(T_LLAN_EURO_NEW_KEY& key)
{
	int nNum = m_aSelLlan.GetSize();
	for (int i=0; i<nNum; i++)
	{
		if ((m_aSelLlan[i].key.key1 == key.key.key1 && m_aSelLlan[i].key.key2 == key.key.key2) ||
			(m_aSelLlan[i].key.key1 == key.key.key2 && m_aSelLlan[i].key.key2 == key.key.key1)) 
			return TRUE;
	}
	return FALSE;
}

BOOL CCMMvldItemAustraliaDlg::IsDuplicatedItemBD(T_LLAN_EURO_NEW_KEY& key)
{
	int nNum = m_aSelLlan.GetSize();
	for (int i = 0; i < nNum; i++)
	{
		if ((m_aSelLlan[i].key.key1 == key.key.key1))
			return TRUE;
	}
	return FALSE;
}

void CCMMvldItemAustraliaDlg::AlignControl()
{
	UpdateData(TRUE);

	CRect rRef;
	CRect rToMove;
	int nDistX = 0;
	int nDistY = 0;	

	GetDlgItem(IDC_CMD_MVLD_OPTIM_CHK)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVLD_PERMIT_GRP)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aPermit, nDistX, nDistY);

	GetDlgItem(IDC_CMD_MVLD_SUB_CASE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aOptim, nDistX, nDistY);

	if(m_bPermit)
	{
		GetDlgItem(IDC_CMD_MVLD_PERMIT_GRP )->GetWindowRect(rRef);
	}
	else
	{
		AlignControl_Sub();

		if(m_Data.nLoadModel==AS_Heavy || m_Data.nLoadModel == AS_Double || m_Data.nLoadModel == AS_Group2) // Heavy. B Duoble, Group2
		{
			GetDlgItem(IDC_CMD_LANES_GRUP)->GetWindowRect(rRef); 
		}
		else
		{
			if(m_bAutoOptim) GetDlgItem(IDC_CMD_MVLD_LCASE_GRP )->GetWindowRect(rRef);
			else             GetDlgItem(IDC_CMD_MVLD_SUB_CASE_GRP)->GetWindowRect(rRef); 
		}
	}
	GetDlgItem(IDOK)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, m_aOkCancel, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.right  = rectLast.right + globalUtils.ScaleByDPI(20);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);

	RedrawWindow();
}

void CCMMvldItemAustraliaDlg::AlignControl_Sub()
{
	CRect rRef;
	CRect rToMove;
	int nDistX = 0;
	int nDistY = 0;	

	if(m_Data.nLoadModel==AS_Heavy) // Heavy
	{
		// Accompanying Lane Factor2
		GetDlgItem(IDC_CMD_FACTOR_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP2)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + 10;
		CDlgUtil::CtrlMoveDistXY(this, m_aLaneFactor2, nDistX, nDistY);

		// Load Case Data2
		GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP2)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_LCASE2_GRP    )->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistXY(this, m_aLCase2, nDistX, nDistY);
	
		if(m_bAutoOptim)
		{
			// Optim 1
			GetDlgItem(IDC_CMD_MVLD_LCASE2_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP )->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistXY(this, m_aOptim01, nDistX, nDistY);

			// Assignment Lane
			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_LANES_GRUP     )->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistXY(this, m_aAssinLane, nDistX, nDistY);

			// Load lane
			GetDlgItem(IDC_CMD_LIST_TXT      )->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVLD_LANE2_STC)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistXY(this, m_aAssinLane03, nDistX, nDistY);

			// Accompanying Lane Factor resize
			CRect rectResize;
			GetDlgItem(IDC_CMD_LANES_GRUP)->GetWindowRect(rectResize);
			GetDlgItem(IDC_CMD_MVLD_LANE2_CMB)->GetWindowRect(rRef);
			rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(10);
			ScreenToClient(rectResize);
			GetDlgItem(IDC_CMD_LANES_GRUP)->MoveWindow(rectResize);
		}
		else
		{
			// Assignment Lane
			GetDlgItem(IDC_CMD_MVLD_LCASE2_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_LANES_GRUP     )->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistXY(this, m_aAssinLane, nDistX, nDistY);

			// Assignment Lane resize
			CRect rectResize;
			GetDlgItem(IDC_CMD_LANES_GRUP)->GetWindowRect(rectResize);
			GetDlgItem(IDC_CMD_UNSEL_LIST)->GetWindowRect(rRef);
			rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(10);
			ScreenToClient(rectResize);
			GetDlgItem(IDC_CMD_LANES_GRUP)->MoveWindow(rectResize);
		}

	}
	else if (m_Data.nLoadModel==AS_Double) // B Double
	{
		// Accompanying Lane Factor
		GetDlgItem(IDC_CMD_FACTOR_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistXY(this, m_aLaneFactor, nDistX, nDistY);

		// Accompanying Lane Factor resize
		CRect rectResize;
		GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP)->GetWindowRect(rectResize);
		GetDlgItem(IDC_CMD_MVLD_SACLE_FACT3_EDT)->GetWindowRect(rRef);
		rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(10);
		ScreenToClient(rectResize);
		GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP)->MoveWindow(rectResize);

		// Load Case Data2
		GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_LCASE2_GRP)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistXY(this, m_aLCase2, nDistX, nDistY);

		//B Double Cmb
		GetDlgItem(IDC_CMD_LOAD_HEAVY_STC)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_LOAD_BDOUBLE_STC)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistXY(this, m_aBDoubleCmb, nDistX, nDistY);

		if (m_bAutoOptim)
		{
			// Optim 1
			GetDlgItem(IDC_CMD_MVLD_LCASE2_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistXY(this, m_aOptim01, nDistX, nDistY);

			// Assignment Lane
			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_LANES_GRUP)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistXY(this, m_aAssinLane, nDistX, nDistY);

			//B Double List
			GetDlgItem(IDC_CMD_HEAVY_LIST)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_BDOUBLE_LIST)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistXY(this, m_aBDoubleList, nDistX, nDistY);

			// Load lane
			GetDlgItem(IDC_CMD_LIST_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVLD_LANE2_STC)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistXY(this, m_aAssinLane03, nDistX, nDistY);

			// Accompanying Lane Factor resize
			CRect rectResize;
			GetDlgItem(IDC_CMD_LANES_GRUP)->GetWindowRect(rectResize);
			GetDlgItem(IDC_CMD_MVLD_LANE2_CMB)->GetWindowRect(rRef);
			rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(10);
			ScreenToClient(rectResize);
			GetDlgItem(IDC_CMD_LANES_GRUP)->MoveWindow(rectResize);
		}
		else
		{
			// Assignment Lane
			GetDlgItem(IDC_CMD_MVLD_LCASE2_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_LANES_GRUP)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistXY(this, m_aAssinLane, nDistX, nDistY);

			//B Double List
			GetDlgItem(IDC_CMD_HEAVY_LIST)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_BDOUBLE_LIST)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistXY(this, m_aBDoubleList, nDistX, nDistY);

			// Assignment Lane resize
			CRect rectResize;
			GetDlgItem(IDC_CMD_LANES_GRUP)->GetWindowRect(rectResize);
			GetDlgItem(IDC_CMD_UNSEL_LIST)->GetWindowRect(rRef);
			rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(10);
			ScreenToClient(rectResize);
			GetDlgItem(IDC_CMD_LANES_GRUP)->MoveWindow(rectResize);
		}
	}
	else if (m_Data.nLoadModel == AS_Group1) // Group1
	{
		// Multiple Lane Factor
		GetDlgItem(IDC_CMD_FACTOR_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_AS_MULT_LANE_FACT_GRP)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistXY(this, m_aMultiLaneFactorList, nDistX, nDistY);


		GetDlgItem(IDC_CMD_MVLD_AS_MULT_SACLE_FACT5_EDT)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_AS_MULT_SACLE_FACT6_EDT)->GetWindowRect(rToMove);
		CArray<UINT, UINT> aCtrl;
		aCtrl.RemoveAll();
		aCtrl.Add(IDC_CMD_MVLD_AS_MULT_SACLE_FACT6_EDT);
		aCtrl.Add(IDC_CMD_MVLD_AS_MULT_LANE6_STC);

		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistXY(this, aCtrl, nDistX, nDistY);

		// Accompanying Lane Factor
		GetDlgItem(IDC_CMD_MVLD_AS_MULT_LANE_FACT_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistXY(this, m_aLaneFactor, nDistX, nDistY);

		// Accompanying Lane Factor resize
		CRect rectResize;
		GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP)->GetWindowRect(rectResize);
		GetDlgItem(IDC_CMD_MVLD_SACLE_FACT3_EDT)->GetWindowRect(rRef);
		rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(10);
		ScreenToClient(rectResize);
		GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP)->MoveWindow(rectResize);

		if(m_bAutoOptim)
		{
			// Optim 1
			GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistXY(this, m_aOptim01, nDistX, nDistY);

			// Optim 2
			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVLD_LCASE_GRP)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistXY(this, m_aOptim02, nDistX, nDistY);
		}
		else
		{
			// Sub Load case
			GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVLD_SUB_CASE_GRP)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistXY(this, m_aSubCase, nDistX, nDistY);
		}
	}
	else if (m_Data.nLoadModel == AS_Group2) // Group2
	{
		// Accompanying Lane Factor2
		GetDlgItem(IDC_CMD_FACTOR_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP2)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + 10;
		CDlgUtil::CtrlMoveDistXY(this, m_aLaneFactor2, nDistX, nDistY);

		// Accompanying Lane Factor
		GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP2)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistXY(this, m_aLaneFactor, nDistX, nDistY);

		// Accompanying Lane Factor resize
		CRect rectResize;
		GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP)->GetWindowRect(rectResize);
		GetDlgItem(IDC_CMD_MVLD_SACLE_FACT3_EDT)->GetWindowRect(rRef);
		rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(10);
		ScreenToClient(rectResize);
		GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP)->MoveWindow(rectResize);

		// Load Case Data2
		GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_LCASE2_GRP)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistXY(this, m_aLCase2, nDistX, nDistY);

		//Group2 Cmb
		GetDlgItem(IDC_CMD_LOAD_HEAVY_STC)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_LOAD_WA_RATING_GROUP2_STC)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistXY(this, m_aGroup2Cmb, nDistX, nDistY);

		if (m_bAutoOptim)
		{
			// Optim 1
			GetDlgItem(IDC_CMD_MVLD_LCASE2_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistXY(this, m_aOptim01, nDistX, nDistY);

			// Assignment Lane
			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_LANES_GRUP)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistXY(this, m_aAssinLane, nDistX, nDistY);

			//Group2 List
			GetDlgItem(IDC_CMD_HEAVY_LIST)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_BDOUBLE_LIST)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.top - rToMove.top;

			GetDlgItem(IDC_CMD_BDOUBLE_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_WA_RATING_GROUP2_TXT)->GetWindowRect(rToMove);
			nDistX += rRef.left - rToMove.left;
			nDistY += rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistXY(this, m_aGroup2List, nDistX, nDistY);

			// Load lane
			GetDlgItem(IDC_CMD_LIST_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVLD_LANE2_STC)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistXY(this, m_aAssinLane03, nDistX, nDistY);

			// Accompanying Lane Factor resize
			CRect rectResize;
			GetDlgItem(IDC_CMD_LANES_GRUP)->GetWindowRect(rectResize);
			GetDlgItem(IDC_CMD_MVLD_LANE2_CMB)->GetWindowRect(rRef);
			rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(10);
			ScreenToClient(rectResize);
			GetDlgItem(IDC_CMD_LANES_GRUP)->MoveWindow(rectResize);
		}
		else
		{
			// Assignment Lane
			GetDlgItem(IDC_CMD_MVLD_LCASE2_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_LANES_GRUP)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistXY(this, m_aAssinLane, nDistX, nDistY);

			//Group2 List
			GetDlgItem(IDC_CMD_HEAVY_LIST)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_BDOUBLE_LIST)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.top - rToMove.top;

			GetDlgItem(IDC_CMD_BDOUBLE_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_WA_RATING_GROUP2_TXT)->GetWindowRect(rToMove);
			nDistX += rRef.left - rToMove.left;
			nDistY += rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistXY(this, m_aGroup2List, nDistX, nDistY);

			// Assignment Lane resize
			CRect rectResize;
			GetDlgItem(IDC_CMD_LANES_GRUP)->GetWindowRect(rectResize);
			GetDlgItem(IDC_CMD_UNSEL_LIST)->GetWindowRect(rRef);
			rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(10);
			ScreenToClient(rectResize);
			GetDlgItem(IDC_CMD_LANES_GRUP)->MoveWindow(rectResize);
		}
	}
	else
	{
		// Accompanying Lane Factor
		GetDlgItem(IDC_CMD_FACTOR_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistXY(this, m_aLaneFactor, nDistX, nDistY);

		// Accompanying Lane Factor resize
		CRect rectResize;
		GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP)->GetWindowRect(rectResize);
		if(m_Data.nLoadModel==AS_Rail)
			GetDlgItem(IDC_CMD_MVLD_SACLE_FACT5_EDT)->GetWindowRect(rRef);
		else 
			GetDlgItem(IDC_CMD_MVLD_SACLE_FACT3_EDT)->GetWindowRect(rRef);

		rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(10);
		ScreenToClient(rectResize);
		GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP)->MoveWindow(rectResize);

		if(m_bAutoOptim)
		{
			// Optim 1
			GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistXY(this, m_aOptim01, nDistX, nDistY);

			// Optim 2
			GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVLD_LCASE_GRP)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistXY(this, m_aOptim02, nDistX, nDistY);
		}
		else
		{
			// Sub Load case
			GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVLD_SUB_CASE_GRP)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistXY(this, m_aSubCase, nDistX, nDistY);
		}
	}
}

// void CCMMvldItemAustraliaDlg::AlignControl()
// {
// 	UpdateData(TRUE);
// 
// 	CRect rRef;
// 	CRect rToMove;
// 	int nDistX = 0;
// 	int nDistY = 0;	
// 	
// 	GetDlgItem(IDC_CMD_MVLD_FATIGUE_CHK)->GetWindowRect(rRef);
// 	GetDlgItem(IDC_CMD_MVLD_PERMIT_GRP)->GetWindowRect(rToMove);
// 	nDistX = rRef.left - rToMove.left;
// 	nDistY = rRef.top - rToMove.top;
// 	CDlgUtil::CtrlMoveDistXY(this, m_aPermit, nDistX, nDistY);
// 	
// 	GetDlgItem(IDC_CMD_MVLD_SUB_CASE_GRP)->GetWindowRect(rRef);
// 	GetDlgItem(IDC_CMD_MVLD_OPTIM_GRP)->GetWindowRect(rToMove);
// 	nDistX = rRef.left - rToMove.left;
// 	nDistY = rRef.top - rToMove.top;
// 	CDlgUtil::CtrlMoveDistXY(this, m_aOptim, nDistX, nDistY);
// 	
// 	if(m_bPermit)
// 	{
// 		GetDlgItem(IDC_CMD_MVLD_PERMIT_GRP )->GetWindowRect(rRef);
// 	}
// 	else
// 	{
// 		if(m_bAutoOptim) GetDlgItem(IDC_CMD_MVLD_LCASE_GRP )->GetWindowRect(rRef);
// 		else             GetDlgItem(IDC_CMD_MVLD_SUB_CASE_GRP)->GetWindowRect(rRef); 
// 	}
// 	GetDlgItem(IDOK)->GetWindowRect(rToMove);
// 	nDistY = rRef.bottom - rToMove.top + 10;
// 	CDlgUtil::CtrlMoveDistY(this, m_aOkCancel, nDistY);
// 
// 	// resize self size
// 	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
// 	CRect rectLast;
// 	pWndLast->GetWindowRect(rectLast);
// 	CRect r;
// 	GetWindowRect(r);
// 	r.right  = rectLast.right+15;
// 	r.bottom = rectLast.bottom+15;
// 	MoveWindow(r);
// 
// 	RedrawWindow();
// }

void CCMMvldItemAustraliaDlg::ShowHideControls()
{
	UpdateData(TRUE);

	BOOL bHeavy = (m_Data.nLoadModel==AS_Heavy)? TRUE : FALSE;
	BOOL bRail  = (m_Data.nLoadModel==AS_Rail)? TRUE : FALSE;
	BOOL bDouble = (m_Data.nLoadModel==AS_Double)? TRUE : FALSE;
	BOOL bGroup1 = (m_Data.nLoadModel == AS_Group1) ? TRUE : FALSE;
	BOOL bGroup2 = (m_Data.nLoadModel == AS_Group2) ? TRUE : FALSE;
	BOOL bHeavyForm = (bHeavy || bDouble || bGroup2) ? TRUE : FALSE;

	CDlgUtil::CtrlShowHide(this, m_aPermit,          m_bPermit                );
	CDlgUtil::CtrlShowHide(this, m_aModelType,      !m_bPermit                );
	CDlgUtil::CtrlShowHide(this, m_aDgnComb,        !m_bPermit && !bDouble && !bGroup1);
	CDlgUtil::CtrlShowHide(this, m_aLaneFactor,     !m_bPermit && !bHeavy     );  
	CDlgUtil::CtrlShowHide(this, m_aLaneFactorAdd,  !m_bPermit && !bHeavyForm );
	CDlgUtil::CtrlShowHide(this, m_aSubCase,        !m_bPermit && !bHeavyForm && !m_bAutoOptim);
	CDlgUtil::CtrlShowHide(this, m_aOptim01,        !m_bPermit &&                 m_bAutoOptim);
	CDlgUtil::CtrlShowHide(this, m_aOptim02,        !m_bPermit && !bHeavyForm &&  m_bAutoOptim);
	CDlgUtil::CtrlShowHide(this, m_aLaneFactor2,    !m_bPermit &&  (bHeavy || bGroup2) );
	CDlgUtil::CtrlShowHide(this, m_aLCase2,         !m_bPermit &&  bHeavyForm );
	CDlgUtil::CtrlShowHide(this, m_aAssinLane01,    !m_bPermit &&  bHeavyForm );
	CDlgUtil::CtrlShowHide(this, m_aAssinLane02,    !m_bPermit &&  bHeavyForm && !m_bAutoOptim);
	CDlgUtil::CtrlShowHide(this, m_aAssinLane03,    !m_bPermit &&  bHeavyForm &&  m_bAutoOptim);
	CDlgUtil::CtrlShowHide(this, m_aMultiLaneFactorList, (!m_bPermit && bGroup1));

	if (!m_bPermit && bHeavyForm)
	{
		GetDlgItem(IDC_CMD_LOAD_HEAVY_STC)->ShowWindow(bHeavy);
		GetDlgItem(IDC_CMD_LOAD_HEAVY_CMB)->ShowWindow(bHeavy || bGroup2);
		GetDlgItem(IDC_CMD_HEAVY_TXT     )->ShowWindow(bHeavy && !m_bAutoOptim);
		GetDlgItem(IDC_CMD_HEAVY_LIST    )->ShowWindow((bHeavy || bGroup2) && !m_bAutoOptim);

		GetDlgItem(IDC_CMD_LOAD_BDOUBLE_STC)->ShowWindow(bDouble);
		GetDlgItem(IDC_CMD_LOAD_BDOUBLE_CMB)->ShowWindow(bDouble);
		GetDlgItem(IDC_CMD_BDOUBLE_TXT     )->ShowWindow(bDouble && !m_bAutoOptim);
		GetDlgItem(IDC_CMD_BDOUBLE_LIST    )->ShowWindow(bDouble && !m_bAutoOptim);

		GetDlgItem(IDC_CMD_LOAD_WA_RATING_GROUP2_STC)->ShowWindow(bGroup2);
		GetDlgItem(IDC_CMD_WA_RATING_GROUP2_TXT)->ShowWindow(bGroup2 && !m_bAutoOptim);
	}
	else
	{
		GetDlgItem(IDC_CMD_LOAD_HEAVY_STC  )->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_LOAD_HEAVY_CMB  )->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_HEAVY_TXT       )->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_HEAVY_LIST      )->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_CMD_LOAD_BDOUBLE_STC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_LOAD_BDOUBLE_CMB)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_BDOUBLE_TXT     )->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_BDOUBLE_LIST    )->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_CMD_LOAD_WA_RATING_GROUP2_STC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_WA_RATING_GROUP2_TXT)->ShowWindow(SW_HIDE);
	}

	GetDlgItem(IDC_CMD_MVLD_OPTIM_CHK      )->ShowWindow(!m_bPermit);
	GetDlgItem(IDC_CMD_MVLD_NUM_LANE4_STC  )->ShowWindow(!m_bPermit && bRail);
	GetDlgItem(IDC_CMD_MVLD_NUM_LANE5_STC  )->ShowWindow(!m_bPermit && bRail);
	GetDlgItem(IDC_CMD_MVLD_SACLE_FACT4_EDT)->ShowWindow(!m_bPermit && bRail);
	GetDlgItem(IDC_CMD_MVLD_SACLE_FACT5_EDT)->ShowWindow(!m_bPermit && bRail);

	if(m_Data.nLoadModel==AS_Rail) 
	{	
		GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP)->SetWindowText(_LS(IDS_CMD_MVLD_AUSTRALIA_ACCOMPANYING_LANE_FACTOR_RAIL));
		GetDlgItem(IDC_CMD_MVLD_NUM_LANE3_STC)->SetWindowText(_T("3"));
	}
	else 
	{
		GetDlgItem(IDC_CMD_MVLD_LANE_FACT_GRP)->SetWindowText(_LS(IDS_CMD_MVLD_AUSTRALIA_ACCOMPANYING_LANE_FACTOR));
		GetDlgItem(IDC_CMD_MVLD_NUM_LANE3_STC)->SetWindowText(_LS(IDS_CMD_MVLD_NUM_LANE3_MORE));
	}

	if (!m_bPermit)
 	{
 		GetDlgItem(IDC_B_DOUBLE_LOAD_RDO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_WA_RATING_GROUP1_LOAD_RDO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_WA_RATING_GROUP2_LOAD_RDO)->ShowWindow(SW_SHOW);
 	}
 	else
 	{
		GetDlgItem(IDC_B_DOUBLE_LOAD_RDO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_WA_RATING_GROUP1_LOAD_RDO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_WA_RATING_GROUP2_LOAD_RDO)->ShowWindow(SW_HIDE);
 	}
}

void CCMMvldItemAustraliaDlg::Data2Dlg()
{
	m_edtLoadCase.SetWindowText(m_Data.LoadCaseName);
	m_edtDesc.SetWindowText(m_Data.Description);

	m_bPermit = m_Data.bLoadCaseForPermitLoad;
	m_bAutoOptim = m_Data.bAutoOptimize;

	CDlgUtil::CtrlRadioSetCheck(this, m_aLoadModelRdo, m_Data.nLoadModel);  // 0: General 1:Fatigue 2: Heavy, 3: Rail
	CDlgUtil::CtrlRadioSetCheck(this, m_aDgnCombRdo  , m_Data.nLoadCombType);
	m_bFatigue = (m_Data.nLoadModel==AS_Fatigue)? TRUE : FALSE;

	m_edtLaneFact1.SetEditUnit(m_Data.dScaleFactor[0]);
	m_edtLaneFact2.SetEditUnit(m_Data.dScaleFactor[1]);
	m_edtLaneFact3.SetEditUnit(m_Data.dScaleFactor[2]);
	m_edtLaneFact4.SetEditUnit(m_Data.dScaleFactor[3]);
	m_edtLaneFact5.SetEditUnit(m_Data.dScaleFactor[4]);

	m_edtMultiLaneFact1.SetEditUnit(m_Data.dMultiLaneFactor[0]);
	m_edtMultiLaneFact2.SetEditUnit(m_Data.dMultiLaneFactor[1]);
	m_edtMultiLaneFact3.SetEditUnit(m_Data.dMultiLaneFactor[2]);
	m_edtMultiLaneFact4.SetEditUnit(m_Data.dMultiLaneFactor[3]);
	m_edtMultiLaneFact5.SetEditUnit(m_Data.dMultiLaneFactor[4]);
	m_edtMultiLaneFact6.SetEditUnit(m_Data.dMultiLaneFactor[5]);

	m_nCombOption = m_Data.nCombOption;

	int nSelect = 0;
	for(int i=0; i<m_cmbPermit.GetCount(); i++)
	{
		if(m_Data.PermitVehicleK != m_cmbPermit.GetItemData(i)) continue;
		nSelect = i;
		break;
	}
	m_cmbPermit.SetCurSel(nSelect);

	nSelect = 0;
	for(int i=0; i<m_cmbRefLane.GetCount(); i++)
	{
		if(m_Data.RefLaneK != m_cmbRefLane.GetItemData(i)) continue;
		nSelect = i;
		break;
	}
	m_cmbRefLane.SetCurSel(nSelect);
	
	m_edtEccen.SetEditUnit(m_Data.dEccentricity);
	m_edtScaleFactor.SetEditUnit(m_Data.dPermitScaleFactor);
	m_edtMinVehlDist.SetEditUnit(m_Data.dMinVehlDist);

	nSelect = 0;
	for(int i=0; i<m_cmbOptimLane.GetCount(); i++)
	{
		if(m_Data.OptimizeLane != m_cmbOptimLane.GetItemData(i)) continue;
		nSelect = i;
		break;
	}
	m_cmbOptimLane.SetCurSel(nSelect);

	m_edtMinNumVehl.SetEditUnit(m_Data.nMinNumVehicle);
	m_edtMaxNumVehl.SetEditUnit(m_Data.nMaxNumVehicle);
	m_nCombOption2 = m_Data.nCombOption;
	
	// Default Value
	m_edtScaleFactorOptim.SetWindowText(_T("1.0"));

	// 추가
	//if(m_Data.nLoadModel==2)
	{
		m_edtLaneFact_MS1600.SetEditUnit(m_Data.dMultipleFactor1);
		m_edtMinNumLane.SetEditUnit(m_Data.SubLoadCase.nMinLoadedLanes);
		m_edtMaxNumLane.SetEditUnit(m_Data.SubLoadCase.nMaxLoadedLanes);

		for(int i=0; i<m_cmbOptimLane2.GetCount(); i++)
		{
			if(m_Data.OptimizeLane != m_cmbOptimLane2.GetItemData(i)) continue;
			nSelect = i;
			break;
		}
		m_cmbOptimLane2.SetCurSel(nSelect);


		//
		if(m_cbxHeavyLoadVehi.GetCount() > 0)
			CDlgUtil::CobxSetCurSelItemData(m_cbxHeavyLoadVehi, (DWORD&)m_Data.Vehicle1);
		if(m_cbxMS1600LoadVehi.GetCount() > 0)
			CDlgUtil::CobxSetCurSelItemData(m_cbxMS1600LoadVehi, (DWORD&)m_Data.Vehicle2);
		if(m_cbxDoubleVehi.GetCount() > 0)
			CDlgUtil::CobxSetCurSelItemData(m_cbxDoubleVehi, (DWORD&)m_Data.VehicleK3);

		Data2Dlg_LaneListByVehicleIndex();
	}

	UpdateData(FALSE);
}

BOOL CCMMvldItemAustraliaDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_edtLoadCase.GetWindowText(m_Data.LoadCaseName);
	if(m_pDoc->IsSymbol(m_Data.LoadCaseName)) { AfxMessageBox(_LS(IDS_CMD_MVLD_ERROR_CASE_NAME)); return FALSE; }
	m_edtDesc.GetWindowText(m_Data.Description);

	m_Data.bLoadCaseForPermitLoad = m_bPermit;
	m_Data.bAutoOptimize = m_bAutoOptim;

	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadModelRdo, m_Data.nLoadModel);
	if(m_Data.nLoadModel==AS_Heavy || m_Data.nLoadModel == AS_Group2) // heavy
		CDlgUtil::CtrlRadioGetCheck(this, m_aDgnCombRdo  , m_Data.nLoadCombType);
	else
		m_Data.nLoadCombType = 1;

	m_Data.bFatigue = m_bFatigue;
	m_bFatigue = (m_Data.nLoadModel==AS_Fatigue)? TRUE : FALSE;

	m_Data.dScaleFactor[0] = m_edtLaneFact1.GetEditValue();
	m_Data.dScaleFactor[1] = m_edtLaneFact2.GetEditValue();
	m_Data.dScaleFactor[2] = m_edtLaneFact3.GetEditValue();
	m_Data.dScaleFactor[3] = m_edtLaneFact4.GetEditValue();
	m_Data.dScaleFactor[4] = m_edtLaneFact5.GetEditValue();
	
	m_Data.dMultiLaneFactor[0] = m_edtMultiLaneFact1.GetEditValue();
	m_Data.dMultiLaneFactor[1] = m_edtMultiLaneFact2.GetEditValue();
	m_Data.dMultiLaneFactor[2] = m_edtMultiLaneFact3.GetEditValue();
	m_Data.dMultiLaneFactor[3] = m_edtMultiLaneFact4.GetEditValue();
	m_Data.dMultiLaneFactor[4] = m_edtMultiLaneFact5.GetEditValue();
	m_Data.dMultiLaneFactor[5] = m_edtMultiLaneFact6.GetEditValue();

	m_Data.nCombOption = m_nCombOption;	
	m_Data.PermitVehicleK = m_cmbPermit.GetItemData(m_cmbPermit.GetCurSel());
	m_Data.RefLaneK = m_cmbRefLane.GetItemData(m_cmbRefLane.GetCurSel());
	m_Data.dEccentricity = m_edtEccen.GetEditValue();
	m_Data.dPermitScaleFactor = m_edtScaleFactor.GetEditValue();
	m_Data.dMinVehlDist = m_edtMinVehlDist.GetEditValue();
	m_Data.OptimizeLane = m_cmbOptimLane.GetItemData(m_cmbOptimLane.GetCurSel());
	m_Data.nMinNumVehicle = m_edtMinNumVehl.GetEditValue();
	m_Data.nMaxNumVehicle = m_edtMaxNumVehl.GetEditValue();
	if(m_bAutoOptim) m_Data.nCombOption = m_nCombOption2;

	// 추가
	if(m_Data.nLoadModel==AS_Heavy) // Heavy
	{
		m_Data.nCombOption = 0;

		m_Data.dMultipleFactor1 = m_edtLaneFact_MS1600.GetEditValue();
		m_Data.SubLoadCase.nMinLoadedLanes = m_edtMinNumLane.GetEditValue();
		m_Data.SubLoadCase.nMaxLoadedLanes = m_edtMaxNumLane.GetEditValue();

		if(m_cbxHeavyLoadVehi.GetCount() > 0)
			CDlgUtil::CobxGetItemDataByCurSel(m_cbxHeavyLoadVehi, (DWORD&)m_Data.Vehicle1);
		if(m_cbxMS1600LoadVehi.GetCount() > 0)
			CDlgUtil::CobxGetItemDataByCurSel(m_cbxMS1600LoadVehi, (DWORD&)m_Data.Vehicle2);
		m_Data.VehicleK3 = 0;

		int nSize = m_aLaneList.GetSize();  
		// v795 에서 부터 Lane 완화로 인해서 20 개까지 Check 
		int nMaxLane = 20;
		int nCount=0;
		// 초기화
		m_Data.aNALanes.RemoveAll();
		for (int i=0; i<nSize; i++)
		{
			if (m_aSelFlag[i]) 
			{
				if (nCount >= nMaxLane)
				{
					CString csMsg;
					csMsg.Format(_LS(IDS_CMD_MVLD_SUBLOAD_EXCEED_MAX), nMaxLane);
					AfxMessageBox(csMsg);
					return FALSE;
				}
				m_Data.aNALanes.Add(m_aLaneList[i]);
				nCount++;
			}
		}


		T_LLAN_EURO_NEW_KEY key;
		// 초기화
		m_Data.aStraddLanes.RemoveAll();
		m_Data.aStraddLanes.SetSize(m_aSelLlan.GetSize());

		for (int i=0; i<m_aSelLlan.GetSize(); i++)
		{
			key = m_aSelLlan[i];
			m_Data.aStraddLanes[i].Lane1 = key.key.key1;
			m_Data.aStraddLanes[i].Lane2 = key.key.key2;
		}

		if(m_bAutoOptim) m_Data.OptimizeLane = m_cmbOptimLane2.GetItemData(m_cmbOptimLane2.GetCurSel());

		// 불필요한 정보 삭제
		m_Data.arSubLoadData.RemoveAll();
		m_Data.aFootwayLanes.RemoveAll();
	}
	else if (m_Data.nLoadModel==AS_Double) // B Double
	{
		m_Data.nCombOption = 0;

		m_Data.dMultipleFactor1 = m_edtLaneFact_MS1600.GetEditValue();
		m_Data.SubLoadCase.nMinLoadedLanes = m_edtMinNumLane.GetEditValue();
		m_Data.SubLoadCase.nMaxLoadedLanes = m_edtMaxNumLane.GetEditValue();

		m_Data.Vehicle1 = 0;
		if (m_cbxDoubleVehi.GetCount() > 0)
			CDlgUtil::CobxGetItemDataByCurSel(m_cbxDoubleVehi, (DWORD&)m_Data.VehicleK3);
		if (m_cbxMS1600LoadVehi.GetCount() > 0)
			CDlgUtil::CobxGetItemDataByCurSel(m_cbxMS1600LoadVehi, (DWORD&)m_Data.Vehicle2);

		int nSize = m_aLaneList.GetSize();
		// v795 에서 부터 Lane 완화로 인해서 20 개까지 Check 
		int nMaxLane = 20;
		int nCount = 0;
		// 초기화
		m_Data.aNALanes.RemoveAll();
		for (int i = 0; i < nSize; i++)
		{
			if (m_aSelFlag[i])
			{
				if (nCount >= nMaxLane)
				{
					CString csMsg;
					csMsg.Format(_LS(IDS_CMD_MVLD_SUBLOAD_EXCEED_MAX), nMaxLane);
					AfxMessageBox(csMsg);
					return FALSE;
				}
				m_Data.aNALanes.Add(m_aLaneList[i]);
				nCount++;
			}
		}


		T_LLAN_EURO_NEW_KEY key;
		// 초기화
		m_Data.aFootwayLanes.RemoveAll();
		m_Data.aFootwayLanes.SetSize(m_aSelLlan.GetSize());

		for (int i = 0; i < m_aSelLlan.GetSize(); i++)
		{
			key = m_aSelLlan[i];
			m_Data.aFootwayLanes[i] = key.key.key1;
		}

		if (m_bAutoOptim) m_Data.OptimizeLane = m_cmbOptimLane2.GetItemData(m_cmbOptimLane2.GetCurSel());

		// 불필요한 정보 삭제
		m_Data.arSubLoadData.RemoveAll();
		m_Data.aStraddLanes.RemoveAll();
	}
	else if (m_Data.nLoadModel == AS_Group2) // Group2
	{
		m_Data.nCombOption = 0;

		m_Data.dMultipleFactor1 = m_edtLaneFact_MS1600.GetEditValue();
		m_Data.SubLoadCase.nMinLoadedLanes = m_edtMinNumLane.GetEditValue();
		m_Data.SubLoadCase.nMaxLoadedLanes = m_edtMaxNumLane.GetEditValue();

		if (m_cbxHeavyLoadVehi.GetCount() > 0)
			CDlgUtil::CobxGetItemDataByCurSel(m_cbxHeavyLoadVehi, (DWORD&)m_Data.Vehicle1);
		if (m_cbxMS1600LoadVehi.GetCount() > 0)
			CDlgUtil::CobxGetItemDataByCurSel(m_cbxMS1600LoadVehi, (DWORD&)m_Data.Vehicle2);
		m_Data.VehicleK3 = 0;

		int nSize = m_aLaneList.GetSize();
		// v795 에서 부터 Lane 완화로 인해서 20 개까지 Check 
		int nMaxLane = 20;
		int nCount = 0;
		// 초기화
		m_Data.aNALanes.RemoveAll();
		for (int i = 0; i < nSize; i++)
		{
			if (m_aSelFlag[i])
			{
				if (nCount >= nMaxLane)
				{
					CString csMsg;
					csMsg.Format(_LS(IDS_CMD_MVLD_SUBLOAD_EXCEED_MAX), nMaxLane);
					AfxMessageBox(csMsg);
					return FALSE;
				}
				m_Data.aNALanes.Add(m_aLaneList[i]);
				nCount++;
			}
		}


		T_LLAN_EURO_NEW_KEY key;
		// 초기화
		m_Data.aStraddLanes.RemoveAll();
		m_Data.aStraddLanes.SetSize(m_aSelLlan.GetSize());

		for (int i = 0; i < m_aSelLlan.GetSize(); i++)
		{
			key = m_aSelLlan[i];
			m_Data.aStraddLanes[i].Lane1 = key.key.key1;
			m_Data.aStraddLanes[i].Lane2 = key.key.key2;
		}

		if (m_bAutoOptim) m_Data.OptimizeLane = m_cmbOptimLane2.GetItemData(m_cmbOptimLane2.GetCurSel());

		// 불필요한 정보 삭제
		m_Data.arSubLoadData.RemoveAll();
		m_Data.aFootwayLanes.RemoveAll();
	}
	else
	{
		// 불필요한 정보 삭제
		m_Data.aNALanes.RemoveAll();
		m_Data.aStraddLanes.RemoveAll();
		m_Data.aFootwayLanes.RemoveAll();
	}
	
	return TRUE;
}

BOOL CCMMvldItemAustraliaDlg::ApplyOrOK()
{
	if (!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if (m_bModify)
		bSuccess = m_pDoc->m_pDataCtrl->ModifyMvld(m_csOldName, m_Data);
	else
		bSuccess = m_pDoc->m_pDataCtrl->AddMvld(m_Data);
	if (!bSuccess) return FALSE;

	return TRUE;
}

#define COLCOUNT 8
#define COLCOUNT_ASSIGNVEHL 2
/////////////////////////////////////////////////////////////////////////////
// CCMMvhcListDlg implementation functions
void CCMMvldItemAustraliaDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Vehicle_class), _LS(IDS_WG_CMD__ADDD__ReducFact), _LS(IDS_WG_CMD__ADDD__Lane1), _LS(IDS_WG_CMD__ADDD__Lane2), _LS(IDS_WG_CMD__ADDD__Lane3), _LS(IDS_WG_CMD__ADDD__Lane4), _LS(IDS_WG_CMD__ADDD__Lane5), _LS(IDS_WG_CMD__ADDD__More)};
	int nColWidth[COLCOUNT] = {108, 50, 60, 60, 60, 60, 60, 48};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_lstSubCase.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_lstSubCase.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_lstSubCase.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvldItemAustraliaDlg::MakeItemEx()
{
	m_lstSubCase.DeleteAllItems();

	int nItemCount = m_Data.arSubLoadData.GetSize();
	if(nItemCount == 0) return;

	if(nItemCount)
	{
		for(int nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
			InsertItem(nCount, m_Data.arSubLoadData[nCount]);
		}
	}
}

BOOL CCMMvldItemAustraliaDlg::InsertItem(int nIndex, T_MVLD_BASE &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem;

	lvitem.iItem = nIndex;
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		str = DataToStr(i, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if( i == 0 ) 
		{
			nItem = m_lstSubCase.InsertItem(&lvitem);
		}
		else m_lstSubCase.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CCMMvldItemAustraliaDlg::DeleteItem(int nIndex, T_MVLD_BASE &Data)
{
	Data; // Not Used;
	if (nIndex >= 0) m_lstSubCase.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMMvldItemAustraliaDlg::ModifyItem(int nIndex, T_MVLD_BASE &Data)
{
	int nItem = nIndex;
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr(i, Data);
			m_lstSubCase.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

CString CCMMvldItemAustraliaDlg::DataToStr(int i, T_MVLD_BASE &Data)
{
	CString str;

	T_MVHC_D mvhc;
	T_MVHL_D mvhl;
	if(i==0) 
	{
		if (Data.nVehicleType == 1)
		{
			m_pDoc->m_pAttrCtrl->GetMvhc(Data.VehicleKey, mvhc);
			str = _T("VC:")+mvhc.VehicleClassName;
		}
		else
		{
			m_pDoc->m_pAttrCtrl->GetMvhl(Data.VehicleKey, mvhl);
			str = _T("VL:")+mvhl.VehicleLoadName;
		}
	}
	else if(i==1) str.Format(_T("%g"), Data.dVehicleScaleFactor);
	else if(i>=2 && i<=6)
	{
		if(Data.aSelectedLanes.GetSize() > i-2)
		{
			if(Data.aSelectedLanes[i-2] == 0) str = _T("");
			else
			{
				str = GetLaneName(Data.aSelectedLanes[i-2]);
			}
		}
		else str = _T("");
	}
	else if (i==7)
	{
		if(Data.aSelectedLanes.GetSize() > i-2)
		{
			if (Data.aSelectedLanes[i-2] == 0) str = _T("");
			else str = _T("...");
		}
		else str = _T("");
	}
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

CString CCMMvldItemAustraliaDlg::GetLaneName(UINT nLaneKey)
{
	if (m_bSurface)
	{
		T_SLAN_D slan;
		m_pDoc->m_pAttrCtrl->GetSlan(nLaneKey, slan);
		return slan.LaneName;
	}
	else
	{
		T_LLAN_D llan;
		m_pDoc->m_pAttrCtrl->GetLlan(nLaneKey, llan);
		return llan.LineLaneName;
	}
	return _LS(IDS_WG_CMD__ADDD__Error);
}

void CCMMvldItemAustraliaDlg::SetHeaderTitle_AssignVehl()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Vehicle), _LS(IDS_WG_CMD__ADDD__ReducFact)};
	int nColWidth[COLCOUNT_ASSIGNVEHL] = {140, 140};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_lstAssignVehl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_lstAssignVehl.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < COLCOUNT_ASSIGNVEHL; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_lstAssignVehl.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvldItemAustraliaDlg::MakeItemEx_AssignVehl()
{
	m_lstAssignVehl.DeleteAllItems();

	int nItemCount = m_Data.aOptimize.GetSize();
	if(nItemCount == 0) return;

	if(nItemCount)
	{
		for(int nCount = 0 ; nCount < nItemCount ; nCount++)
		{
			InsertItem_AssignVehl(nCount, m_Data.aOptimize[nCount]);
		}
	}
}

BOOL CCMMvldItemAustraliaDlg::InsertItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem;

	lvitem.iItem = nIndex;
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		str = DataToStr_AssignVehl(i, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if( i == 0 ) 
		{
			nItem = m_lstAssignVehl.InsertItem(&lvitem);
		}
		else m_lstAssignVehl.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CCMMvldItemAustraliaDlg::DeleteItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data)
{
	Data; // Not Used;
	if (nIndex >= 0) m_lstAssignVehl.DeleteItem(nIndex);
	return TRUE;
}

BOOL CCMMvldItemAustraliaDlg::ModifyItem_AssignVehl(int nIndex, T_MVLD_OPTIMIZE &Data)
{
	int nItem = nIndex;
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr_AssignVehl(i, Data);
			m_lstAssignVehl.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

CString CCMMvldItemAustraliaDlg::DataToStr_AssignVehl(int i, T_MVLD_OPTIMIZE &Data)
{
	CString str;

	T_MVHC_D mvhc;
	T_MVHL_D mvhl;
	if(i==0) 
	{
		if (Data.nVehicleType == 1)
		{
			m_pDoc->m_pAttrCtrl->GetMvhc(Data.VehicleK, mvhc);
			str = _T("VC:")+mvhc.VehicleClassName;
		}
		else
		{
			m_pDoc->m_pAttrCtrl->GetMvhl(Data.VehicleK, mvhl);
			str = _T("VL:")+mvhl.VehicleLoadName;
		}
	}
	else if(i==1) str.Format(_T("%g"), Data.dScaleFactor);
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

BOOL CCMMvldItemAustraliaDlg::OnApplySubDlg(BOOL bModify, T_MVLD_BASE& data, int nPos)
{
	if (!bModify)
	{
		int nCount = m_Data.arSubLoadData.GetSize();
		m_Data.arSubLoadData.Add(data);
		InsertItem(nCount, data);
	}
	else
	{
		m_Data.arSubLoadData.SetAt(nPos, data);
		ModifyItem(nPos, data);
	}
	return TRUE;
}


BEGIN_MESSAGE_MAP(CCMMvldItemAustraliaDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldItemAustraliaDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY,                     OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_MVLD_PERMIT_CHK,           OnCmdPermitChk)
	ON_BN_CLICKED(IDC_CMD_MVLD_OPTIM_CHK,            OnCmdAutoOptimChk) 
	ON_BN_CLICKED(IDC_GENERAL_LOAD_RDO,              OnSelLoadModelRdo) 
	ON_BN_CLICKED(IDC_FATIGUE_LOAD_RDO,              OnSelLoadModelRdo) 
	ON_BN_CLICKED(IDC_HEAVY_LOAD_RDO,                OnSelLoadModelRdo) 
	ON_BN_CLICKED(IDC_RAIL_LOAD_RDO,                 OnSelLoadModelRdo) 
	ON_BN_CLICKED(IDC_B_DOUBLE_LOAD_RDO,             OnSelLoadModelRdo)
	ON_BN_CLICKED(IDC_WA_RATING_GROUP1_LOAD_RDO,     OnSelLoadModelRdo)
	ON_BN_CLICKED(IDC_WA_RATING_GROUP2_LOAD_RDO,     OnSelLoadModelRdo)
	ON_BN_CLICKED(IDC_CMD_MVLD_SUB_CASE_ADD_BTN,     OnCmdSubCaseAddBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_SUB_CASE_MOD_BTN,     OnCmdSubCaseModBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_SUB_CASE_DEL_BTN,     OnCmdSubCaseDelBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_ASSIGN_VEHL_ADD_BTN,  OnCmdAssignVehlAddBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_ASSIGN_VEHL_MOD_BTN,  OnCmdAssignVehlModBtn)
	ON_BN_CLICKED(IDC_CMD_MVLD_ASSIGN_VEHL_DEL_BTN,  OnCmdAssignVehlDelBtn)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_MVLD_SUB_CASE_LST,  OnDblclkCmdList)
	ON_CBN_SELCHANGE(IDC_CMD_LOAD_HEAVY_CMB,  OnSelChangeVClass)
	ON_CBN_SELCHANGE(IDC_CMD_LOAD_MS1600_CMB, OnSelChangeVClass)

	ON_LBN_DBLCLK(IDC_CMD_UNSEL_LIST, OnDblclkCmdSubUnselList)  
	ON_LBN_DBLCLK(IDC_CMD_HEAVY_LIST, OnDblclkCmdSubHeavyList)

	ON_BN_CLICKED(IDC_CMD_SUB_BTN_ADD,        OnCmdSubBtnAdd)
	ON_BN_CLICKED(IDC_CMD_SUB_BTN_DEL,        OnCmdSubBtnDelete)
	ON_BN_CLICKED(IDC_CMD_SUB_BTN_ADD2,       OnCmdSubHeavyAddBtn)
	ON_BN_CLICKED(IDC_CMD_SUB_BTN_DEL2,       OnCmdSubHeavyDelBtn)

	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)

    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemAustraliaDlg message handlers

BOOL CCMMvldItemAustraliaDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_pDoc = CDBDoc::GetDocPoint();
	
	m_chkLoadModel.InitControl(this, CCMMvldItemAustraliaDlg::IDD, IDC_SEL_LOAD_MODEL_GRP, TRUE, TRUE);
	m_chkLoadModel.SetFoldCurHeight(TRUE);

	m_chkMultiLaneFactor.InitControl(this, CCMMvldItemAustraliaDlg::IDD, IDC_CMD_MVLD_AS_MULT_LANE_FACT_GRP, TRUE, TRUE);
	m_chkMultiLaneFactor.SetFoldCurHeight(TRUE);
	
	if (m_pDoc->m_pAttrCtrl->GetCountSlan() > 0 || m_pDoc->m_pAttrCtrl->GetCountSlanop() > 0)
	{
		m_bSurface = TRUE;
	}
	else
	{
		m_bSurface = FALSE;
	}

	T_MVCD_D DataMvcd;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
	m_nCodeType = DataMvcd.nCodeType;

	if (!m_bModify) // New
	{
		m_Data.Initialize();
		m_Data.dMinVehlDist = M_InitValueCurUnit(1.0, KN, M, CUnitCtrl::m_MVLD_UNIT.dMinVehlDist);
		m_pDoc->m_pAttrCtrl2->SetMvldFactor(m_Data, m_nCodeType);

		m_Data.dMultipleFactor1 = 0.5;
	}

	InitCtrl();
	InitCombo(); // Data2Dlg();보다 먼저 불리도록.
	InitUnit();

	SetHeaderTitle();
	SetHeaderTitle_AssignVehl();
	Data2Dlg();
	MakeItemEx();
	MakeItemEx_AssignVehl();

	AlignControl();
	ShowHideControls();

	CDlgUtil::CtrlEnableDisable(this, m_aDgnComb, (m_Data.nLoadModel==AS_Heavy||m_Data.nLoadModel==AS_Group2));
	if(m_Data.nLoadModel!=AS_Heavy && m_Data.nLoadModel != AS_Group2)
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aDgnCombRdo, 1);
	}

	if(m_bModify) GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvldItemAustraliaDlg::OnOK() 
{
	if (!ApplyOrOK()) return;
	
	m_chkLoadModel.SetFoldState(FALSE, FALSE);
	m_chkMultiLaneFactor.SetFoldState(FALSE, FALSE);
	CDialogMove::OnOK();
}

void CCMMvldItemAustraliaDlg::OnCmdApply() 
{
	m_chkLoadModel.SetFoldState(FALSE, FALSE);
	m_chkMultiLaneFactor.SetFoldState(FALSE, FALSE);
	ApplyOrOK();	
}

void CCMMvldItemAustraliaDlg::OnCancel()
{
	m_chkLoadModel.SetFoldState(FALSE, FALSE);
	m_chkMultiLaneFactor.SetFoldState(FALSE, FALSE);
	CDialogMove::OnCancel();
}

LRESULT CCMMvldItemAustraliaDlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{

	AlignControl();
	ShowHideControls();
	BOOL bFold = m_chkLoadModel.GetFoldState();
	CDlgUtil::CtrlShowHide(this, m_aLoadModelRdo, !bFold);

	BOOL bGroup1 = (m_Data.nLoadModel == AS_Group1) ? TRUE : FALSE;
	bFold = m_chkMultiLaneFactor.GetFoldState();
	CDlgUtil::CtrlShowHide(this, m_aMultiLaneFactorFold, !bFold && (!m_bPermit && bGroup1));

	return 0L;
}

void CCMMvldItemAustraliaDlg::OnCmdPermitChk() 
{
	UpdateData(TRUE);
	m_bAutoOptim = FALSE;
	UpdateData(FALSE);

	InitCombo();
	AlignControl();
	ShowHideControls();

	if (!m_bPermit)
	{
		BOOL bFold = m_chkLoadModel.GetFoldState();
		CDlgUtil::CtrlShowHide(this, m_aLoadModelRdo, !bFold);

		BOOL bGroup1 = (m_Data.nLoadModel == AS_Group1) ? TRUE : FALSE;
		bFold = m_chkMultiLaneFactor.GetFoldState();
		CDlgUtil::CtrlShowHide(this, m_aMultiLaneFactorFold, !bFold && (!m_bPermit && bGroup1));
	}
}

void CCMMvldItemAustraliaDlg::OnCmdAutoOptimChk() 
{
	UpdateData(TRUE);
	m_bPermit = FALSE;
	UpdateData(FALSE);

	m_Data.aStraddLanes.RemoveAll();
	m_Data.aFootwayLanes.RemoveAll();
	m_Data.aNALanes.RemoveAll();
	Data2Dlg_LaneListByVehicleIndex();

	InitCombo();
	AlignControl();
	ShowHideControls();

	BOOL bFold = m_chkLoadModel.GetFoldState();
	CDlgUtil::CtrlShowHide(this, m_aLoadModelRdo, !bFold);

	BOOL bGroup1 = (m_Data.nLoadModel == AS_Group1) ? TRUE : FALSE;
	bFold = m_chkMultiLaneFactor.GetFoldState();
	CDlgUtil::CtrlShowHide(this, m_aMultiLaneFactorFold, !bFold && (!m_bPermit && bGroup1));
}

void CCMMvldItemAustraliaDlg::OnSelLoadModelRdo() 
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadModelRdo, m_Data.nLoadModel);  // 0: General 1:Fatigue 2: Heavy, 3: Rail, 4: B Double

	m_chkMultiLaneFactor.SetFoldState(FALSE, TRUE);

	if(m_Data.nLoadModel==AS_Fatigue) // Fatigue
	{
		m_bFatigue = TRUE;
		InitCombo();
		AlignControl();
		ShowHideControls();

		if(m_Data.arSubLoadData.GetSize() > 0 || m_Data.aOptimize.GetSize() > 0)
		{
			m_Data.arSubLoadData.RemoveAll();
			m_Data.aOptimize.RemoveAll();

			MakeItemEx();
			MakeItemEx_AssignVehl();
		}
	}
	else if(m_Data.nLoadModel==AS_General || m_Data.nLoadModel==AS_Heavy || m_Data.nLoadModel==AS_Rail || 
		m_Data.nLoadModel==AS_Double || m_Data.nLoadModel == AS_Group1 || m_Data.nLoadModel == AS_Group2)
	{
		m_aSelLlan.RemoveAll();

		m_bFatigue = FALSE;

		if (!m_bModify) {
			if (m_Data.nLoadModel == AS_Rail)
			{
				m_pDoc->m_pAttrCtrl2->SetMvldFactor(m_Data, m_nCodeType);

				m_edtLaneFact1.SetEditUnit(m_Data.dScaleFactor[0]);
				m_edtLaneFact2.SetEditUnit(m_Data.dScaleFactor[1]);
				m_edtLaneFact3.SetEditUnit(m_Data.dScaleFactor[2]);
				m_edtLaneFact4.SetEditUnit(m_Data.dScaleFactor[3]);
				m_edtLaneFact5.SetEditUnit(m_Data.dScaleFactor[4]);
			}
			if (m_Data.nLoadModel == AS_Group1)
			{
				m_edtMultiLaneFact1.SetEditUnit(m_Data.dMultiLaneFactor[0]);
				m_edtMultiLaneFact2.SetEditUnit(m_Data.dMultiLaneFactor[1]);
				m_edtMultiLaneFact3.SetEditUnit(m_Data.dMultiLaneFactor[2]);
				m_edtMultiLaneFact4.SetEditUnit(m_Data.dMultiLaneFactor[3]);
				m_edtMultiLaneFact5.SetEditUnit(m_Data.dMultiLaneFactor[4]);
				m_edtMultiLaneFact6.SetEditUnit(m_Data.dMultiLaneFactor[5]);
			}
		}

		InitCombo();
		AlignControl();
		ShowHideControls();

		m_Data.aStraddLanes.RemoveAll();
		m_Data.aFootwayLanes.RemoveAll();
		m_Data.aNALanes.RemoveAll();
		Data2Dlg_LaneListByVehicleIndex();
	}
	else ASSERT(0);

	CDlgUtil::CtrlEnableDisable(this, m_aDgnComb, (m_Data.nLoadModel==AS_Heavy||m_Data.nLoadModel==AS_Group2));
	if(m_Data.nLoadModel!=AS_Heavy && m_Data.nLoadModel!=AS_Group2)
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aDgnCombRdo, 1);
	}
}

void CCMMvldItemAustraliaDlg::OnCmdSubCaseAddBtn() 
{
	UpdateData(TRUE);

	CCMMvldSubItemDlg dlg(this);
	if(m_Data.nLoadModel==AS_Rail) dlg.m_nLoadModel = 3;// Rail
	else if(m_bFatigue)      dlg.m_nLoadModel = 1;
	else                     dlg.m_nLoadModel = 0; 
	
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

void CCMMvldItemAustraliaDlg::OnCmdSubCaseModBtn() 
{
	UpdateData(TRUE);

	int iItem = m_lstSubCase.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_sub_load_case));
		return;
	}
 
	CCMMvldSubItemDlg dlg(this);
	if(m_Data.nLoadModel==AS_Rail) dlg.m_nLoadModel = 3;// Rail
	else if(m_bFatigue)      dlg.m_nLoadModel = 1;
	else                     dlg.m_nLoadModel = 0;


	dlg.SetParamData(m_Data.arSubLoadData[iItem], iItem);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

void CCMMvldItemAustraliaDlg::OnCmdSubCaseDelBtn() 
{
	int iItem = m_lstSubCase.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_sub_load_case));
		return;
	}
	m_Data.arSubLoadData.RemoveAt(iItem);
	m_lstSubCase.DeleteItem(iItem);

	int nCount = m_lstSubCase.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_lstSubCase.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMMvldItemAustraliaDlg::OnCmdAssignVehlAddBtn() 
{
	UpdateData(TRUE);

	T_MVLD_OPTIMIZE Data; Data.Initialize();

	T_MVLD_TYPE_KEY TypeKey;
	TypeKey.value = m_cmbSelVehl.GetItemData(m_cmbSelVehl.GetCurSel());

	Data.nVehicleType = TypeKey.typekey.type + 1;
	Data.VehicleK = TypeKey.typekey.key;
	Data.dScaleFactor = m_edtScaleFactorOptim.GetEditValue();

	if(Data.nVehicleType == 1)
	{
		if(!m_pDoc->m_pAttrCtrl->ExistMvhc(Data.VehicleK)) return;
	}
	else
	{
		if(!m_pDoc->m_pAttrCtrl->ExistMvhl(Data.VehicleK)) return;
	}

	int nCount = m_Data.aOptimize.GetSize();
	m_Data.aOptimize.Add(Data);
	InsertItem_AssignVehl(nCount, Data);
}

void CCMMvldItemAustraliaDlg::OnCmdAssignVehlModBtn() 
{
	UpdateData(TRUE);

	int iItem = m_lstAssignVehl.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_assignment_vehicle));
		return;
	}

	T_MVLD_OPTIMIZE Data; Data.Initialize();

	T_MVLD_TYPE_KEY TypeKey;
	TypeKey.value = m_cmbSelVehl.GetItemData(m_cmbSelVehl.GetCurSel());

	Data.nVehicleType = TypeKey.typekey.type + 1;
	Data.VehicleK = TypeKey.typekey.key;
	Data.dScaleFactor = m_edtScaleFactorOptim.GetEditValue();

	if(Data.nVehicleType == 1)
	{
		if(!m_pDoc->m_pAttrCtrl->ExistMvhc(Data.VehicleK)) return;
	}
	else
	{
		if(!m_pDoc->m_pAttrCtrl->ExistMvhl(Data.VehicleK)) return;
	}

	m_Data.aOptimize.SetAt(iItem, Data);
	ModifyItem_AssignVehl(iItem, Data);
}

void CCMMvldItemAustraliaDlg::OnCmdAssignVehlDelBtn() 
{
	int iItem = m_lstAssignVehl.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_assignment_vehicle));
		return;
	}

	m_Data.aOptimize.RemoveAt(iItem);
	m_lstAssignVehl.DeleteItem(iItem);

	int nCount = m_lstAssignVehl.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_lstAssignVehl.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMMvldItemAustraliaDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCmdSubCaseModBtn();	
	*pResult = 0;
}

void CCMMvldItemAustraliaDlg::OnSelChangeVClass()
{
	T_MVHL_K Vehicle1 = 0;
	T_MVHL_K Vehicle2 = 0;
	T_MVHL_K Vehicle3 = 0;

	if(m_cbxHeavyLoadVehi.GetCount() > 0)
		CDlgUtil::CobxGetItemDataByCurSel(m_cbxHeavyLoadVehi, (DWORD&)Vehicle1);
	if(m_cbxMS1600LoadVehi.GetCount() > 0)
		CDlgUtil::CobxGetItemDataByCurSel(m_cbxMS1600LoadVehi, (DWORD&)Vehicle2);
	if (m_cbxDoubleVehi.GetCount() > 0)
		CDlgUtil::CobxGetItemDataByCurSel(m_cbxDoubleVehi, (DWORD&)Vehicle3);
}


void CCMMvldItemAustraliaDlg::OnDblclkCmdSubUnselList() 
{
	OnCmdSubBtnAdd();
}
void CCMMvldItemAustraliaDlg::OnDblclkCmdSubHeavyList() 
{
	OnCmdSubHeavyDelBtn();
}

void CCMMvldItemAustraliaDlg::OnCmdSubBtnAdd() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndUnselList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndUnselList.GetItemData(aIndex[i]);
		m_aSelFlag[nIndex] = TRUE;
		m_wndUnselList.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_wndSelList.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aSelFlag[i]) 
		{
			nIndex = m_wndSelList.AddString(csLaneName);
			m_wndSelList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemAustraliaDlg::OnCmdSubBtnDelete() 
{
	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndSelList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;

	// Error Check
	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndSelList.GetItemData(aIndex[i]);

		int HeavydK = m_aLaneList[nIndex];

		for(int j=0; j<m_aSelLlan.GetSize(); j++)
		{

			if (m_Data.nLoadModel == AS_Heavy)
			{
				if (HeavydK == m_aSelLlan[j].key.key1) { AfxMessageBox(_LS(IDS_CMD_MVLD_EURO_HEAVY_LANE_ERROR)); return; }
				if (HeavydK == m_aSelLlan[j].key.key2) { AfxMessageBox(_LS(IDS_CMD_MVLD_EURO_HEAVY_LANE_ERROR)); return; }
			}
			else if (m_Data.nLoadModel == AS_Double)
			{
				if (HeavydK == m_aSelLlan[j].key.key1) { AfxMessageBox(_LS(IDS_CMD_MVLD_EURO_BDOUBLE_LANE_ERROR)); return; }
				if (HeavydK == m_aSelLlan[j].key.key2) { AfxMessageBox(_LS(IDS_CMD_MVLD_EURO_BDOUBLE_LANE_ERROR)); return; }
			}
			else
				ASSERT(0);
		}
	}

	for (int i = nCount-1; i >= 0; i--)
	{
		nIndex = m_wndSelList.GetItemData(aIndex[i]);
		m_aSelFlag[nIndex] = FALSE;
		m_wndSelList.DeleteString(aIndex[i]);
	}

	CString csLaneName;
	m_wndUnselList.ResetContent();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (!m_aSelFlag[i]) 
		{
			nIndex = m_wndUnselList.AddString(csLaneName);
			m_wndUnselList.SetItemData(nIndex, i);
		}
	}
}

void CCMMvldItemAustraliaDlg::OnCmdSubHeavyAddBtn() 
{
	if (m_Data.nLoadModel==AS_Double)
	{
		CmdSubBDoubleAddBtn();
		return;
	}

	int nIndex;
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndSelList.GetSelItems(nMax, aIndex.GetData());
	if (nCount != 2)
	{
		AfxMessageBox(_LS(IDS_WG_CMD_ERR_Selected_Lane_Number2));
		return;
	}

	T_LLAN_EURO_NEW_KEY key;
	nIndex = m_wndSelList.GetItemData(aIndex[0]);
	key.key.key1 = m_aLaneList[nIndex];
	nIndex = m_wndSelList.GetItemData(aIndex[1]);
	key.key.key2 = m_aLaneList[nIndex];

	if (IsDuplicatedItem(key)) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD_ERR_EXIST_SAME_ITEM));
		return;
	}
	if (!m_bSurface)
	{
		T_LLAN_D LlanD1, LlanD2;
		m_pDoc->m_pAttrCtrl->GetLlan(key.key.key1, LlanD1);
		m_pDoc->m_pAttrCtrl->GetLlan(key.key.key2, LlanD2);
// 		if (LlanD1.nLoadDistribution != 2 ||
// 			LlanD2.nLoadDistribution != 2 ) // Cross Beam 아닌 경우
// 		{
// 			AfxMessageBox(_LS(IDS_WG_CMD_MVLD_BS_LANE_TYPE_ERROR));
// 			return;
// 		}
		BOOL bChk = TRUE;
		if(LlanD1.nLoadDistribution == 1)
		{
			if(LlanD2.nLoadDistribution != 1)        bChk = FALSE;
		}
		if(LlanD1.nLoadDistribution == 2)
		{
			if(LlanD2.nLoadDistribution != 2)        bChk = FALSE;
			if(LlanD1.ElemGroup != LlanD2.ElemGroup) bChk = FALSE;
		}
		if(!bChk)
		{
			//[오류] Heavy Load Lanes 또는 WA Rating Vehicle Group2 에 사용된 두 차선의 Vehicular Load Distribution 정보는 서로 같아야 합니다. ( Lane Name = %s, %s )
			GSaveHistoryFormatNF(_LS(IDS_DB_MOVLANE__ERROR_AS_STRADDLING_LOADDISTRNEW), LlanD1.LineLaneName, LlanD2.LineLaneName);
			return;
		}

	}

	CString csLaneName;
	csLaneName = GetLaneName(key.key.key1);
	csLaneName += _T(" : ");
	csLaneName += GetLaneName(key.key.key2);
	nIndex = m_lstHeavy.AddString(csLaneName);
	m_lstHeavy.SetItemData(nIndex, key.keymap);
	m_aSelLlan.Add(key);
}

void CCMMvldItemAustraliaDlg::OnCmdSubHeavyDelBtn() 
{
	if (m_Data.nLoadModel==AS_Double)
	{
		CmdSubBDoubleDelBtn();
		return;
	}

	UINT idata;
	int nMax = m_aSelLlan.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_lstHeavy.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount-1; i >= 0; i--)
	{
		idata = m_lstHeavy.GetItemData(aIndex[i]);
		if (m_lstHeavy.DeleteString(aIndex[i]) == LB_ERR) continue;
		int k = 0;
		for (k=0; k<nMax; k++)
			if (idata == m_aSelLlan[k].keymap) break;
		if (k == nMax) { ASSERT(0); continue; }
		m_aSelLlan.RemoveAt(k);
	}
}

void CCMMvldItemAustraliaDlg::CmdSubBDoubleAddBtn()
{
	int nMax = m_aLaneList.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_wndSelList.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount - 1; i >= 0; i--)
	{
		int nIndex = m_wndSelList.GetItemData(aIndex[i]);

		T_LLAN_EURO_NEW_KEY key;
		key.key.key1 = m_aLaneList[nIndex];
		key.key.key2 = 0;

		if (IsDuplicatedItemBD(key))
		{
			AfxMessageBox(_LS(IDS_WG_CMD_ERR_EXIST_SAME_ITEM));
			return;
		}

		m_aBDFlag[nIndex] = TRUE;
	}
	
	CString csLaneName;
	m_lstDouble.ResetContent();
	m_aSelLlan.RemoveAll();
	int nAllSize = m_aLaneList.GetSize();
	for (int i = 0; i < nAllSize; i++)
	{
		csLaneName = GetLaneName(m_aLaneList[i]);
		if (m_aBDFlag[i])
		{
			T_LLAN_EURO_NEW_KEY key;
			key.key.key1 = m_aLaneList[i];
			key.key.key2 = 0;

			int nIndex = m_lstDouble.AddString(csLaneName);
			m_lstDouble.SetItemData(nIndex, i);
			m_aSelLlan.Add(key);
		}
	}
}

void CCMMvldItemAustraliaDlg::CmdSubBDoubleDelBtn()
{
	int idata;
	int nMax = m_aSelLlan.GetSize();
	if (nMax <= 0) return;
	CArray<int, int> aIndex;
	aIndex.SetSize(nMax);
	int nCount = m_lstDouble.GetSelItems(nMax, aIndex.GetData());
	if (nCount == 0) return;
	for (int i = nCount - 1; i >= 0; i--)
	{
		idata = m_lstDouble.GetItemData(aIndex[i]);
		if (m_lstDouble.DeleteString(aIndex[i]) == LB_ERR) continue;
		int k = 0;
		for (k = 0; k < nMax; k++)
			if (m_aLaneList[idata] == m_aSelLlan[k].keymap) break;
		if (k == nMax) { ASSERT(0); continue; }
		m_aSelLlan.RemoveAt(k);
		m_aBDFlag[idata] = FALSE;
	}
}

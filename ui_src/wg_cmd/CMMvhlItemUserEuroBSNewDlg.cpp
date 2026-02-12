// CMMvhlItemUserEuroBSNewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUserEuroBSNewDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\SpecialChar.h"
#include "..\wg_base\wg_base_TestEnvMgr.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_MvhlDB.h"

#include "CMMvhlItemUsrPermitDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUserEuroBSNewDlg dialog


CCMMvhlItemUserEuroBSNewDlg::CCMMvhlItemUserEuroBSNewDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemUserEuroBSNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemUserEuroBSNewDlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_bAxleSpacing = FALSE;
	m_bDynamic = TRUE;
	m_nDynaType = 0;

	m_bModify = FALSE;
	m_nCodeType = 0;
	m_Data_RoadFootBridge[0].Initialize();
	m_Data_RoadFootBridge[1].Initialize();
	m_Data_RoadFootBridge[2].Initialize();
	m_Data_RoadFootBridge[3].Initialize();
	m_Data_RoadFootBridge[4].Initialize();
	m_Data_RoadFootBridge[5].Initialize();

	m_Data_RailTrafficLoad[0].Initialize(); m_Data_RailTrafficLoad[0].bStandard = TRUE;
	m_Data_RailTrafficLoad[1].Initialize(); m_Data_RailTrafficLoad[1].bStandard = TRUE;
	m_Data_RailTrafficLoad[2].Initialize(); m_Data_RailTrafficLoad[2].bStandard = TRUE;
	m_Data_RailTrafficLoad[3].Initialize(); m_Data_RailTrafficLoad[3].bStandard = TRUE;
	m_Data_RailTrafficLoad[4].Initialize(); m_Data_RailTrafficLoad[4].bStandard = TRUE;
	m_Data_RailTrafficLoad[5].Initialize(); m_Data_RailTrafficLoad[5].bStandard = TRUE;

	m_aCtrlLoadTypeCode1.RemoveAll();
	m_aCtrlLoadTypeCode1.Add(IDC_CMD_ML_MVHL_ITEMU_LM1_RDO);
	m_aCtrlLoadTypeCode1.Add(IDC_CMD_ML_MVHL_ITEMU_LM2_RDO);
	m_aCtrlLoadTypeCode1.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_RDO);
	m_aCtrlLoadTypeCode1.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_RDO);
	m_aCtrlLoadTypeCode1.Add(IDC_CMD_ML_MVHL_ITEMU_FLM3_RDO);
	m_aCtrlLoadTypeCode1.Add(IDC_CMD_ML_MVHL_ITEMU_EURO_BS_RDO);
	
	m_aCtrlLoadTypeCode2.RemoveAll();
	m_aCtrlLoadTypeCode2.Add(IDC_CMD_ML_MVHL_ITEMU_LM71_RDO);
	m_aCtrlLoadTypeCode2.Add(IDC_CMD_ML_MVHL_ITEMU_LMSW_RDO);
	m_aCtrlLoadTypeCode2.Add(IDC_CMD_ML_MVHL_ITEMU_LMSW2_RDO);		
	m_aCtrlLoadTypeCode2.Add(IDC_CMD_ML_MVHL_ITEMU_UNLOADED_RDO);
	m_aCtrlLoadTypeCode2.Add(IDC_CMD_ML_MVHL_ITEMU_HSLM_A_RDO);
	m_aCtrlLoadTypeCode2.Add(IDC_CMD_ML_MVHL_ITEMU_HSLM_B_RDO);

	m_aCtrlCom5.RemoveAll();
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_DYNAMIC_CHK);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_AUTO_RDO);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_USER_RDO);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_PHI_TXT);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_PHI2_TXT);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_PHI_EDT);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_DYNAMIC_CHK);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_AUTO_RDO);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_USER_RDO);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_PHI_TXT);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_PHI2_TXT);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_PHI_EDT);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_DYNAMIC_CHK);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_AUTO_RDO);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_USER_RDO);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_PHI_TXT);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_PHI2_TXT);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_PHI_EDT);
	
	m_aCtrlCom6.RemoveAll();
	m_aCtrlCom6.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_GRP);
	m_aCtrlCom6.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_WHEEL_SPACING_STC);
	m_aCtrlCom6.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_WHEEL_SPACING_EDT);
	m_aCtrlCom6.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_WHEEL_SPACING_UNT);
	m_aCtrlCom6.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_AXLE_SPACING_GRP);
	m_aCtrlCom6.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_AXLE_SPACING_CHK);
	m_aCtrlCom6.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_AXLE_NUM_STC);
	m_aCtrlCom6.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_AXLE_NUM_EDT);
	m_aCtrlCom6.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_MIN_SPACING_STC);
	m_aCtrlCom6.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_MIN_SPACING_EDT);
	m_aCtrlCom6.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_MIN_SPACING_UNT);
	m_aCtrlCom6.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_MAX_SPACING_STC);
	m_aCtrlCom6.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_MAX_SPACING_EDT);
	m_aCtrlCom6.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_MAX_SPACING_UNT);
	m_aCtrlCom6.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_PHI_GRP);
	m_aCtrlCom6.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_DYNAMIC_CHK);
	m_aCtrlCom6.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_PHI_AUTO_RDO);
	m_aCtrlCom6.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_PHI_USER_RDO);
	m_aCtrlCom6.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_PHI_STC);
	m_aCtrlCom6.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_PHI_EDT);
	
	m_aListEtc.RemoveAll();
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_LST);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_W1_STC);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_W1_EDT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_W1_UNT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_DD1_STC);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_DD1_EDT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_DD1_UNT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_D1_STC);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_D1_EDT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_D1_UNT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_W2_STC);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_W2_EDT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_W2_UNT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_DD2_STC);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_DD2_EDT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_DD2_UNT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_D2_STC);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_D2_EDT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_D2_UNT);
	
	m_aHSLM_A.RemoveAll();
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_N_STC);
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_N_EDT);
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_D_STC);
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_D_EDT);
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_D_UNT);
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_D2_STC);
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_D2_EDT);
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_D2_UNT);
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_P_STC);
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_P_EDT);
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_P_UNT);
	
	m_aHSLM_B.RemoveAll();
	m_aHSLM_B.Add(IDC_CMD_MVHL_EURO_HSLM_B_N_STC);
	m_aHSLM_B.Add(IDC_CMD_MVHL_EURO_HSLM_B_N_EDT);
	m_aHSLM_B.Add(IDC_CMD_MVHL_EURO_HSLM_B_P_STC);
	m_aHSLM_B.Add(IDC_CMD_MVHL_EURO_HSLM_B_P_EDT);
	m_aHSLM_B.Add(IDC_CMD_MVHL_EURO_HSLM_B_P_UNT);
	m_aHSLM_B.Add(IDC_CMD_MVHL_EURO_HSLM_B_D_STC);
	m_aHSLM_B.Add(IDC_CMD_MVHL_EURO_HSLM_B_D_EDT);
	m_aHSLM_B.Add(IDC_CMD_MVHL_EURO_HSLM_B_D_UNT);
	
	m_aDynEff.RemoveAll();
	m_aDynEff.Add(IDC_CMD_MVHL_EURO_DYN_GRP);
	m_aDynEff.Add(IDC_CMD_MVHL_EURO_DYN_STC);
	m_aDynEff.Add(IDC_CMD_MVHL_EURO_DYN_FACT_STC);
	m_aDynEff.Add(IDC_CMD_MVHL_EURO_DYN_FACT_EDT);
	m_aDynEff.Add(IDC_CMD_MVHL_EURO_DYN_FACT2_STC);
	m_aDynEff.Add(IDC_CMD_MVHL_EURO_DYN_FACT2_EDT);
	
	m_aETC.RemoveAll();
	m_aETC.Add(IDC_CMD_MVHL_EURO_ALPHA_STC);
	m_aETC.Add(IDC_CMD_MVHL_EURO_ALPHA_EDT);
	m_aETC.Add(IDC_CMD_MVHL_EURO_LONGI_DIST_CHK);
	m_aETC.Add(IDC_CMD_MVHL_EURO_DIST_POINT_STC);
	m_aETC.Add(IDC_CMD_MVHL_EURO_DIST_POINT_EDT);
	m_aETC.Add(IDC_CMD_MVHL_EURO_DIST_POINT_UNT);
	m_aETC.Add(IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_CHK);
	m_aETC.Add(IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_EDT);
	m_aETC.Add(IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_UNT);

	m_aCtrlPatch.RemoveAll();
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_CHK);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_W_STC);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_W_EDT);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_W_UNT);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_L_STC);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_L_EDT);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_L_UNT);

	m_bPermitLoadCall = FALSE;
	m_bEccenVertLoad = FALSE;
}

CCMMvhlItemUserEuroBSNewDlg::~CCMMvhlItemUserEuroBSNewDlg()
{
}

void CCMMvhlItemUserEuroBSNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemUserEuroBSNewDlg) 
	DDX_Control(pDX, IDC_CMD_MVHL_CODE_CMB, m_wndCodeName);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_NAME_EDT, m_wndVehicleName);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM1_D_EDT, m_LM1Dedt);	
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM1_D_UNIT, m_LM1Dunit);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM1_DAF_EDT, m_LM1DAFedt);	
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM2_P_EDT, m_LM2Pedt);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM2_D_EDT, m_LM2Dedt);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM2_LIST, m_lstTruckL);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM2_ADJUST_EDT, m_LM2AFedt);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM2_ADJUST_EDT2, m_LM2AFedt2);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM2_W_EDT, m_LM2Wedt);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM2_W_UNIT, m_LM2Wunit);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM2_TLFACT_EDT, m_edtLM2TSFact);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM2_UDLFACT_EDT, m_edtLM2UDLFact);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_LOAD_CHK, m_LM3LCchk[0]);
	//DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_NUM_SPN, m_LM3NOPspn[0]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_NUM_EDT, m_LM3NOPedt[0]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_DYNAMIC_CHK, m_LM3DAFchk[0]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_PHI_EDT, m_LM3DAFedt[0]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_LOAD_CHK, m_LM3LCchk[1]);
	//DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_NUM_SPN, m_LM3NOPspn[1]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_NUM_EDT, m_LM3NOPedt[1]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_DYNAMIC_CHK, m_LM3DAFchk[1]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_PHI_EDT, m_LM3DAFedt[1]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_LOAD_CHK, m_LM3LCchk[2]);
	//DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_NUM_SPN, m_LM3NOPspn[2]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_NUM_EDT, m_LM3NOPedt[2]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_DYNAMIC_CHK, m_LM3DAFchk[2]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_PHI_EDT, m_LM3DAFedt[2]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_FLM3_INTERVAL_EDT, m_edtFLM3Interval);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_FLM3_INTERVAL_UNT, m_untFLM3Interval);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_FLM3_FIRST_PHI_CHK, m_FLM3DAFchk[0]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_FLM3_FIRST_PHI_EDT, m_FLM3DAFedt[0]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_FLM3_SECOND_PHI_CHK, m_FLM3DAFchk[1]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_FLM3_SECOND_PHI_EDT, m_FLM3DAFedt[1]);
	DDX_Control(pDX, IDC_CMD_MVHL_TSFACT_EDT, m_edtTSFact);
	DDX_Control(pDX, IDC_CMD_MVHL_UDLFACT_EDT, m_edtUDLFact);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_WHEEL_SPACING_EDT, m_edtWheelSpacing);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_WHEEL_SPACING_UNT, m_untWheelSpacing);
	DDX_Check  (pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_AXLE_SPACING_CHK, m_bAxleSpacing);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_AXLE_NUM_EDT, m_edtAxleNum);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_MIN_SPACING_EDT, m_edtMinSpacing);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_MIN_SPACING_UNT, m_untMinSpacing);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_MAX_SPACING_EDT, m_edtMaxSpacing);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_MAX_SPACING_UNT, m_untMaxSpacing);
	DDX_Check  (pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_DYNAMIC_CHK, m_bDynamic);
	DDX_Radio  (pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_PHI_AUTO_RDO, m_nDynaType);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_PHI_EDT, m_edtPhi);

	DDX_Control(pDX, IDC_CMD_MVHL_EURO_LST,                 m_lstTrain);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_W1_EDT,              m_edtTrainW1);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_W1_UNT,              m_untTrainW1);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_DD1_EDT,             m_edtTrainDD1);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_DD1_UNT,             m_untTrainDD1);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_D1_EDT,              m_edtTrainD1);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_D1_UNT,              m_untTrainD1);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_W2_EDT,              m_edtTrainW2);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_W2_UNT,              m_untTrainW2);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_DD2_EDT,             m_edtTrainDD2);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_DD2_UNT,             m_untTrainDD2);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_D2_EDT,              m_edtTrainD2);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_D2_UNT,              m_untTrainD2);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_A_N_EDT,        m_edtHSLMANum);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_A_D_EDT,        m_edtHSLMALength);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_A_D_UNT,        m_untHSLMALength);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_A_D2_EDT,       m_edtHSLMASpacing);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_A_D2_UNT,       m_untHSLMASpacing);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_A_P_EDT,        m_edtHSLMAForce);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_A_P_UNT,        m_untHSLMAForce);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_B_N_EDT,        m_edtHSLMBNum);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_B_P_EDT,        m_edtHSLMBForce);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_B_P_UNT,        m_untHSLMBForce);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_B_D_EDT,        m_edtHSLMBDist);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_B_D_UNT,        m_untHSLMBDist);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_DYN_FACT_EDT,        m_edtDynEffFact1);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_DYN_FACT2_EDT,       m_edtDynEffFact2);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_ALPHA_EDT,           m_edtAlpha);
	DDX_Check  (pDX, IDC_CMD_MVHL_EURO_LONGI_DIST_CHK,      m_bLongiDist);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_DIST_POINT_EDT,      m_edtDistPoint);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_DIST_POINT_UNT,      m_untDistPoint);
	DDX_Check  (pDX, IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_CHK, m_bEccenVertLoad);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_EDT, m_edtEccenVertLoad);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_UNT, m_untEccenVertLoad);

	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM1_GRID, m_wndLM1Grid);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_GRID, m_wndLM3Grid[0]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_GRID, m_wndLM3Grid[1]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_GRID, m_wndLM3Grid[2]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_PIC_WND, m_wndPicture);

	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_CHK, m_chkPatch);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_W_EDT, m_edtPatchW);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_W_UNT, m_untPatchW);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_L_EDT, m_edtPatchL);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_L_UNT, m_untPatchL);

	//}}AFX_DATA_MAP
}

#define COLCOUNT 3
//-------------------------------------------------------------------------
// Implementation

void CCMMvhlItemUserEuroBSNewDlg::SetHeaderTitle4TrainList()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__No), _LS(IDS_WG_CMD__ADDD__Load), _LS(IDS_WG_CMD__ADDD__Spacing)};
	
	// title에 단위 추가
	CString sLdUnit,sDtUnit;
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointLoad,sLdUnit);
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointDistance,sDtUnit);
	aTitle[1] = aTitle[1] +_T("(")+sLdUnit+_T(")");
	aTitle[2] = aTitle[2] +_T("(")+sDtUnit+_T(")");
	
	int nColWidth[COLCOUNT];
	
	CString title;
	int i;
	LV_COLUMN lvcolumn;
	
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_lstTrain.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_lstTrain.GetSafeHwnd(), dwStyle);
	
	// Calculate width for each column
	nColWidth[0] = 28; nColWidth[1] = 70; nColWidth[2] = 90;
	
	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_RIGHT;
		
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_lstTrain.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvhlItemUserEuroBSNewDlg::MakeItemEx4TrainList()
{
	// List 관련 된 것만 여기서 처리 해준다.
	m_lstTrain.DeleteAllItems();    
	int nLoadCount = GetTrainLoadCount();
	int nDistCount = GetTrainDistCount();
	
	if (nLoadCount > 0)
	{
		int i = 0;
		for (i = 0; i < nDistCount; i++)
			InsertItem4TrainList(i, i+1, m_Data.dPointLoad[i], m_Data.dPointDistance[i]);
		
		if (nLoadCount > nDistCount)
			InsertItem4TrainList(i, i+1, m_Data.dPointLoad[i], 0.0);
	}
	
	UpdateData(FALSE);
}

BOOL CCMMvhlItemUserEuroBSNewDlg::InsertItem4TrainList(int nPos, int nNo, double dLoad, double dDist)
{
	LVITEM lvitem;
	CString str;
	
	lvitem.iItem = nPos;
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		if (i == 0) str.Format(_T("%d"), nNo);
		else if (i == 1) str.Format(_T("%g"), dLoad);
		else if (i == 2)
		{
			if (dDist == 0.0) str = _LS(IDS_WG_CMD__ADDD__end);
			else if (dDist == -1.0) str = _LS(IDS_WG_CMD_INFINITE);
			else str.Format(_T("%g"), dDist);
		}
		else str = _LS(IDS_WG_CMD__ADDD__Error);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;
		
		if(i == 0)
			m_lstTrain.InsertItem(&lvitem);
		else m_lstTrain.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	
	return TRUE;
}

int CCMMvhlItemUserEuroBSNewDlg::GetTrainLoadCount()
{
	int i = 0;
	for(i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CCMMvhlItemUserEuroBSNewDlg::GetTrainDistCount()
{
	int i = 0;
	for(i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointDistance[i] == 0.0) break;
	}  
	return i;
}

void CCMMvhlItemUserEuroBSNewDlg::SetHeaderTitle4TruckList()
{
	CString aTitle[] = {_LS(IDS_CMD_MVHL_NO), _LS(IDS_CMD_MVHL_P), _LS(IDS_CMD_MVHL_D)};

	// title에 단위 추가
	CString sLdUnit,sDtUnit;
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointLoad,sLdUnit);
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointDistance,sDtUnit);
	aTitle[1] = aTitle[1] +_T("(")+sLdUnit+_T(")");
	aTitle[2] = aTitle[2] +_T("(")+sDtUnit+_T(")");

	int nColWidth[COLCOUNT];

	CString title;
	int i=0;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_lstTruckL.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_lstTruckL.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 28; nColWidth[1] = 55; nColWidth[2] = 55;

	// Set Title
	for(i=0; i<COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_RIGHT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_lstTruckL.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvhlItemUserEuroBSNewDlg::MakeItemEx4TruckList(int nSubType)
{
	m_lstTruckL.DeleteAllItems();

	int nLoadCount = GetTruckLoadCount(nSubType);
	int nDistCount = GetTruckDistanceCount(nSubType);
	
	if (nLoadCount == 0) return;
	int i = 0;
	if(GetCodeType()==D_MVHL_EURO_BS_ROADBRIDGE_FOOTBRIDGE)
	{
		for (i = 0; i < nDistCount; i++)
			InsertItem4TruckList(i, i+1, m_Data_RoadFootBridge[nSubType].dPointLoad[i], m_Data_RoadFootBridge[nSubType].dPointDistance[i]);
		if (nLoadCount > nDistCount)
			InsertItem4TruckList(i, i+1, m_Data_RoadFootBridge[nSubType].dPointLoad[i], 0.0);
	}
	else
	{
		for (i = 0; i < nDistCount; i++)
			InsertItem4TruckList(i, i+1, m_Data_RailTrafficLoad[nSubType-6].dPointLoad[i], m_Data_RailTrafficLoad[nSubType-6].dPointDistance[i]);
		if (nLoadCount > nDistCount)
			InsertItem4TruckList(i, i+1, m_Data_RailTrafficLoad[nSubType-6].dPointLoad[i], 0.0);
	}
}

BOOL CCMMvhlItemUserEuroBSNewDlg::InsertItem4TruckList(int nPos, int nNo, double dLoad, double dDist)
{
	LVITEM lvitem;
	CString str;

	lvitem.iItem = nPos;
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		if (i == 0) str.Format(_T("%d"), nNo);
		else if (i == 1) str.Format(_T("%g"), dLoad);
		else if (i == 2)
		{
			if (dDist == 0.0) str = _LS(IDS_WG_CMD__ADDD__end);
			else str.Format(_T("%g"), dDist);
		}
		else str = _LS(IDS_WG_CMD__ADDD__Error);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if(i == 0) m_lstTruckL.InsertItem(&lvitem);
		else       m_lstTruckL.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CCMMvhlItemUserEuroBSNewDlg::DeleteItem4TruckList(int nPos)
{
	if (nPos != -1) m_lstTruckL.DeleteItem(nPos);
	return TRUE;
}

BOOL CCMMvhlItemUserEuroBSNewDlg::ModifyItem4TruckList(int nPos, int nNo, double dLoad, double dDist)
{
	int nItem = nPos;
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			if (i == 0) str.Format(_T("%d"), nNo);
			else if (i == 1) str.Format(_T("%g"), dLoad);
			else if (i == 2)
			{
				if (dDist == 0.0) str = _LS(IDS_WG_CMD__ADDD__end);
				else str.Format(_T("%g"), dDist);
			}
			else str = _LS(IDS_WG_CMD__ADDD__Error);
			m_lstTruckL.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

void CCMMvhlItemUserEuroBSNewDlg::Data2Dlg()
{
	int nSubType = m_nSubType;
	CDlgUtil::CobxSetCurSelItemData(m_wndCodeName, m_nCodeType);

	switch(nSubType)
	{
	case 0:
		Data2DlgLM1();
		break;
	case 1:
		Data2DlgLM2();
		break;
	case 2:
		Data2DlgLM3();
		break;
	case 3:
		Data2DlgLM3Stradd();
		break;
	case 4:
		Data2DlgFLM3();
		break;
	case 5:
		break;
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
		Data2DlgRailTrafficLoad();
		break;
	default :
		ASSERT(0);
	}

	T_MVHL_PATCH_LOAD PatchLoadDB;
	T_MVHL_D  TempData;
	// Default 를 받아오기 위한 처리
	TempData.bStandard = TRUE;
	TempData.nStandardCode = D_MVHL_EURO_EN1991_2_2003_ROAD_BRG;
	
	if (nSubType == 0 || nSubType == 1)
	{
		if (nSubType == 0) TempData.VehicleTypeName = _T("Load Model 1");
		else if (nSubType == 1) TempData.VehicleTypeName = _T("Load Model 2");
		CVehlDB db(m_pDoc);
		if (!db.GetStandardVehicleLoadValue(TempData, &PatchLoadDB)) ASSERT(0);

		if (!m_bModify) // new
		{
			m_Data_RoadFootBridge[nSubType].PatchLoad = PatchLoadDB;
		}
		else
		{
			if (m_Data_RoadFootBridge[nSubType].PatchLoad.aPatchLoad.GetSize() == 0)
			{
				m_Data_RoadFootBridge[nSubType].PatchLoad = PatchLoadDB;
			}
		}

		m_chkPatch.SetCheck(m_Data_RoadFootBridge[nSubType].PatchLoad.bPatchLoad);
		m_edtPatchW.SetEditUnit(m_Data_RoadFootBridge[nSubType].PatchLoad.aPatchLoad[0].dWidth);
		m_edtPatchL.SetEditUnit(m_Data_RoadFootBridge[nSubType].PatchLoad.aPatchLoad[0].dLength);
		OnChkPatch();
	}
	
	UpdateData(FALSE);
}

BOOL CCMMvhlItemUserEuroBSNewDlg::Dlg2Data(int nSubType)
{	
	UpdateData(TRUE);

	if (nSubType == 0 || nSubType == 1)
	{
		T_MVHL_PATCH_ITEM Patch;
		Patch.dWidth = m_edtPatchW.GetEditValue();
		Patch.dLength = m_edtPatchL.GetEditValue();
		m_Data_RoadFootBridge[nSubType].PatchLoad.aPatchLoad.RemoveAll();
		m_Data_RoadFootBridge[nSubType].PatchLoad.aPatchLoad.Add(Patch);
		m_Data_RoadFootBridge[nSubType].PatchLoad.bPatchLoad = m_chkPatch.GetCheck();
	}
	else
	{
		m_Data_RoadFootBridge[nSubType].PatchLoad.Initialize();
	}

	switch(nSubType)
	{
	case 0:
		return Dlg2DataLM1();
	case 1:
		return Dlg2DataLM2();
	case 2:
		return Dlg2DataLM3();
	case 3:
		return Dlg2DataLM3Stradd();
	case 4:
		return Dlg2DataFLM3();
	case 5:
		break;
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
		return Dlg2DataRailTrafficLoad();
		break;
	default :
		ASSERT(0);
		return FALSE;
	}
	return FALSE;
}

void CCMMvhlItemUserEuroBSNewDlg::Data2DlgLM1()
{
	CString strTemp;
	strTemp.Format(_T("%g"),m_Data_RoadFootBridge[0].dSpace);
	m_LM1Dedt.SetWindowText(strTemp);
	strTemp.Format(_T("%g"),m_Data_RoadFootBridge[0].dAmplification);
	m_LM1DAFedt.SetWindowText(strTemp); 
	m_wndLM1Grid.SetDataSource(&m_Data_RoadFootBridge[0],this,FALSE);
	m_edtTSFact.SetEditUnit(m_Data_RoadFootBridge[0].dAmplification2[0]);
	m_edtUDLFact.SetEditUnit(m_Data_RoadFootBridge[0].dAmplification2[1]);
}

BOOL CCMMvhlItemUserEuroBSNewDlg::Dlg2DataLM1()
{
	CFormulaEdit::GetEditValue(&m_LM1Dedt, m_Data_RoadFootBridge[0].dSpace);
	CFormulaEdit::GetEditValue(&m_LM1DAFedt, m_Data_RoadFootBridge[0].dAmplification);
	m_Data_RoadFootBridge[0].dAmplification2[0] = m_edtTSFact.GetEditValue();
	m_Data_RoadFootBridge[0].dAmplification2[1] = m_edtUDLFact.GetEditValue();
	return TRUE;
}

void CCMMvhlItemUserEuroBSNewDlg::Data2DlgLM2()
{
	MakeItemEx4TruckList(1);
	CString strTemp;
	strTemp.Format(_T("%g"),m_Data_RoadFootBridge[1].dAdjustment);
	m_LM2AFedt.SetWindowText(strTemp);
	strTemp.Format(_T("%g"),m_Data_RoadFootBridge[1].dAdjustment2);
	m_LM2AFedt2.SetWindowText(strTemp);

	strTemp.Format(_T("%g"),m_Data_RoadFootBridge[1].dPressureLoad);
	m_LM2Wedt.SetWindowText(strTemp);

	m_edtLM2TSFact.SetEditUnit(m_Data_RoadFootBridge[1].dAmplification2[0]);
	m_edtLM2UDLFact.SetEditUnit(m_Data_RoadFootBridge[1].dAmplification2[1]);
}

BOOL CCMMvhlItemUserEuroBSNewDlg::Dlg2DataLM2()
{
	CFormulaEdit::GetEditValue(&m_LM2AFedt, m_Data_RoadFootBridge[1].dAdjustment);
	CFormulaEdit::GetEditValue(&m_LM2AFedt2, m_Data_RoadFootBridge[1].dAdjustment2);
	CFormulaEdit::GetEditValue(&m_LM2Wedt, m_Data_RoadFootBridge[1].dPressureLoad);
	
	m_Data_RoadFootBridge[1].dAmplification2[0] = m_edtLM2TSFact.GetEditValue();
	m_Data_RoadFootBridge[1].dAmplification2[1] = m_edtLM2UDLFact.GetEditValue();

	return TRUE;
}

void CCMMvhlItemUserEuroBSNewDlg::Data2DlgLM3()
{
	CArray<UINT,UINT> arTemp;
	m_LM3LCchk[0].SetCheck(m_Data_RoadFootBridge[2].bUsePointLoad3[0]);
	m_LM3LCchk[1].SetCheck(m_Data_RoadFootBridge[2].bUsePointLoad3[1]);
	m_LM3LCchk[2].SetCheck(m_Data_RoadFootBridge[2].bUsePointLoad3[2]);
	

	//edit box도 값이 반영되는 지 확인해봐야 한다.

	m_LM3NOPedt[0].SetValue(m_Data_RoadFootBridge[2].nNumLoad3[0]);
	m_LM3NOPedt[1].SetValue(m_Data_RoadFootBridge[2].nNumLoad3[1]);
	m_LM3NOPedt[2].SetValue(m_Data_RoadFootBridge[2].nNumLoad3[2]);

	m_wndLM3Grid[0].SetDataSource(&m_Data_RoadFootBridge[2],0,1);
	m_wndLM3Grid[1].SetDataSource(&m_Data_RoadFootBridge[2],1,1);
	m_wndLM3Grid[2].SetDataSource(&m_Data_RoadFootBridge[2],2,1);

	m_LM3DAFchk[0].SetCheck(m_Data_RoadFootBridge[2].bDynamicFactor3[0]);
	m_LM3DAFchk[1].SetCheck(m_Data_RoadFootBridge[2].bDynamicFactor3[1]);
	m_LM3DAFchk[2].SetCheck(m_Data_RoadFootBridge[2].bDynamicFactor3[2]);

	arTemp.RemoveAll();
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_AUTO_RDO);
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_USER_RDO);
	CDlgUtil::CtrlRadioSetCheck(this,arTemp,m_Data_RoadFootBridge[2].bUserInput3[0]);
	
	arTemp.RemoveAll();
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_AUTO_RDO);
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_USER_RDO);
	CDlgUtil::CtrlRadioSetCheck(this,arTemp,m_Data_RoadFootBridge[2].bUserInput3[1]);
	
	arTemp.RemoveAll();
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_AUTO_RDO);
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_USER_RDO);
	CDlgUtil::CtrlRadioSetCheck(this,arTemp,m_Data_RoadFootBridge[2].bUserInput3[2]);
	
	CString strTemp;
	strTemp.Format(_T("%g"),m_Data_RoadFootBridge[2].dAmplification3[0]);
	m_LM3DAFedt[0].SetWindowText(strTemp);
	strTemp.Format(_T("%g"),m_Data_RoadFootBridge[2].dAmplification3[1]);
	m_LM3DAFedt[1].SetWindowText(strTemp);
	strTemp.Format(_T("%g"),m_Data_RoadFootBridge[2].dAmplification3[2]);
	m_LM3DAFedt[2].SetWindowText(strTemp);
 }

BOOL CCMMvhlItemUserEuroBSNewDlg::Dlg2DataLM3()
{
	CArray<UINT,UINT> arTemp;
	m_Data_RoadFootBridge[2].bUsePointLoad3[0] = m_LM3LCchk[0].GetCheck();
	m_Data_RoadFootBridge[2].bUsePointLoad3[1] = m_LM3LCchk[1].GetCheck();
	m_Data_RoadFootBridge[2].bUsePointLoad3[2] = m_LM3LCchk[2].GetCheck();
	
	CFormulaEdit::GetEditValue(&m_LM3NOPedt[0], m_Data_RoadFootBridge[2].nNumLoad3[0]);
	CFormulaEdit::GetEditValue(&m_LM3NOPedt[1], m_Data_RoadFootBridge[2].nNumLoad3[1]);
	CFormulaEdit::GetEditValue(&m_LM3NOPedt[2], m_Data_RoadFootBridge[2].nNumLoad3[2]);
 
			
	m_Data_RoadFootBridge[2].bDynamicFactor3[0] = m_LM3DAFchk[0].GetCheck();
	m_Data_RoadFootBridge[2].bDynamicFactor3[1] = m_LM3DAFchk[1].GetCheck();
	m_Data_RoadFootBridge[2].bDynamicFactor3[2] = m_LM3DAFchk[2].GetCheck();

	arTemp.RemoveAll();
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_AUTO_RDO);
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_USER_RDO);
	CDlgUtil::CtrlRadioGetCheck(this,arTemp,m_Data_RoadFootBridge[2].bUserInput3[0]);
	
	arTemp.RemoveAll();
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_AUTO_RDO);
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_USER_RDO);
	CDlgUtil::CtrlRadioGetCheck(this,arTemp,m_Data_RoadFootBridge[2].bUserInput3[1]);
	
	arTemp.RemoveAll();
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_AUTO_RDO);
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_USER_RDO);
	CDlgUtil::CtrlRadioGetCheck(this,arTemp,m_Data_RoadFootBridge[2].bUserInput3[2]);

	CFormulaEdit::GetEditValue(&m_LM3DAFedt[0], m_Data_RoadFootBridge[2].dAmplification3[0]);
	CFormulaEdit::GetEditValue(&m_LM3DAFedt[1], m_Data_RoadFootBridge[2].dAmplification3[1]);
	CFormulaEdit::GetEditValue(&m_LM3DAFedt[2], m_Data_RoadFootBridge[2].dAmplification3[2]);

	return TRUE;
}

void CCMMvhlItemUserEuroBSNewDlg::Data2DlgLM3Stradd()
{
	MakeItemEx4TruckList(3);

	m_edtWheelSpacing.SetEditUnit(m_Data_RoadFootBridge[3].dWheelSpacing);
	m_bAxleSpacing = m_Data_RoadFootBridge[3].bVarSpacing;
	m_edtAxleNum.SetEditUnit(m_Data_RoadFootBridge[3].nAxleNum);
	m_edtMinSpacing.SetEditUnit(m_Data_RoadFootBridge[3].dMinSpacing);
	m_edtMaxSpacing.SetEditUnit(m_Data_RoadFootBridge[3].dMaxSpacing);

	m_bDynamic = m_Data_RoadFootBridge[3].bDynamicFactor;
	m_nDynaType = m_Data_RoadFootBridge[3].bUserInput;
	m_edtPhi.SetEditUnit(m_Data_RoadFootBridge[3].dAmplification);

	UpdateData(FALSE);
}

BOOL CCMMvhlItemUserEuroBSNewDlg::Dlg2DataLM3Stradd()
{
	UpdateData(TRUE);
	
	m_Data_RoadFootBridge[3].dWheelSpacing = m_edtWheelSpacing.GetEditValue();
	m_Data_RoadFootBridge[3].bVarSpacing = m_bAxleSpacing;
	m_Data_RoadFootBridge[3].nAxleNum = m_edtAxleNum.GetEditValue();
	m_Data_RoadFootBridge[3].dMinSpacing = m_edtMinSpacing.GetEditValue();
	m_Data_RoadFootBridge[3].dMaxSpacing = m_edtMaxSpacing.GetEditValue();
	m_Data_RoadFootBridge[3].bDynamicFactor = m_bDynamic;
	m_Data_RoadFootBridge[3].bUserInput = m_nDynaType;
	m_Data_RoadFootBridge[3].dAmplification = m_edtPhi.GetEditValue();	

	return TRUE;
}

void CCMMvhlItemUserEuroBSNewDlg::Data2DlgFLM3()
{
	CArray<UINT,UINT> arTemp;
	m_LM3LCchk[0].SetCheck(m_Data_RoadFootBridge[4].bUsePointLoad3[0]);
	m_LM3LCchk[1].SetCheck(m_Data_RoadFootBridge[4].bUsePointLoad3[1]);
	m_LM3LCchk[2].SetCheck(m_Data_RoadFootBridge[4].bUsePointLoad3[2]);
	

	//edit box도 값이 반영되는 지 확인해봐야 한다.

	m_LM3NOPedt[0].SetValue(m_Data_RoadFootBridge[4].nNumLoad3[0]);
	m_LM3NOPedt[1].SetValue(m_Data_RoadFootBridge[4].nNumLoad3[1]);
	m_LM3NOPedt[2].SetValue(m_Data_RoadFootBridge[4].nNumLoad3[2]);

	m_wndLM3Grid[0].SetDataSource(&m_Data_RoadFootBridge[4],0,1);
	m_wndLM3Grid[1].SetDataSource(&m_Data_RoadFootBridge[4],1,1);
	m_wndLM3Grid[2].SetDataSource(&m_Data_RoadFootBridge[4],2,1);

	m_FLM3DAFchk[0].SetCheck(m_Data_RoadFootBridge[4].bDynamicFactor2[0]);
	m_FLM3DAFchk[1].SetCheck(m_Data_RoadFootBridge[4].bDynamicFactor2[1]);

	
	CString strTemp;
	strTemp.Format(_T("%g"),m_Data_RoadFootBridge[4].dAmplification2[0]);
	m_FLM3DAFedt[0].SetWindowText(strTemp);
	strTemp.Format(_T("%g"),m_Data_RoadFootBridge[4].dAmplification2[1]);
	m_FLM3DAFedt[1].SetWindowText(strTemp);
	m_edtFLM3Interval.SetEditUnit(m_Data_RoadFootBridge[4].dInterval);
}

BOOL CCMMvhlItemUserEuroBSNewDlg::Dlg2DataFLM3()
{
	CArray<UINT,UINT> arTemp;
	m_Data_RoadFootBridge[4].bUsePointLoad3[0] = m_LM3LCchk[0].GetCheck();
	m_Data_RoadFootBridge[4].bUsePointLoad3[1] = m_LM3LCchk[1].GetCheck();
	m_Data_RoadFootBridge[4].bUsePointLoad3[2] = m_LM3LCchk[2].GetCheck();
	
	CFormulaEdit::GetEditValue(&m_LM3NOPedt[0], m_Data_RoadFootBridge[4].nNumLoad3[0]);
	CFormulaEdit::GetEditValue(&m_LM3NOPedt[1], m_Data_RoadFootBridge[4].nNumLoad3[1]);
	CFormulaEdit::GetEditValue(&m_LM3NOPedt[2], m_Data_RoadFootBridge[4].nNumLoad3[2]);
 
			
	m_Data_RoadFootBridge[4].bDynamicFactor2[0] = m_FLM3DAFchk[0].GetCheck();
	m_Data_RoadFootBridge[4].bDynamicFactor2[1] = m_FLM3DAFchk[1].GetCheck();
 
	CFormulaEdit::GetEditValue(&m_FLM3DAFedt[0], m_Data_RoadFootBridge[4].dAmplification2[0]);
	CFormulaEdit::GetEditValue(&m_FLM3DAFedt[1], m_Data_RoadFootBridge[4].dAmplification2[1]);
	m_Data_RoadFootBridge[4].dInterval = m_edtFLM3Interval.GetEditValue();

	return TRUE;
}

void CCMMvhlItemUserEuroBSNewDlg::Data2DlgRailTrafficLoad()
{
	if(m_Data_RailTrafficLoad[m_nSubType-6].bStandard)
	{
		T_MVHL_D MvhlD; MvhlD.Initialize();


		//////////////////////////////////////////////////////////////////////////////
		// Standard의 디폴트값으로 채워주기 위해 Standard로 값 얻어오고 User 타입으로 변경
		MvhlD.bStandard = TRUE;
		MvhlD.nStandardCode = 23;
		switch(m_nSubType)
		{
		case 6:
			MvhlD.VehicleTypeName = _T("Load Model 71");
			break;
		case 7:
			MvhlD.VehicleTypeName = _T("Load Model SW/0");
			break;
		case 8:
			MvhlD.VehicleTypeName = _T("Load Model SW/2");
			break;	
		case 9:
			MvhlD.VehicleTypeName = _T("Unloaded Train");
			break;
		case 10:
			MvhlD.VehicleTypeName = _T("HSLM A1 ~ HSLM A10");
			MvhlD.SelVehicle = _T("A1");
			break;
		case 11:
			MvhlD.VehicleTypeName = _T("HSLM B");
			break;
		default:
			ASSERT(0);
			break;
		}
		
		CVehlDB db(m_pDoc);  
		if (!db.GetStandardVehicleLoadValue(MvhlD)) MvhlD.Initialize();
		MvhlD.bStandard = FALSE;
		MvhlD.nSubType  = m_nSubType;
//     if(m_nSubType==9)
//     {
//       MvhlD.VehicleTypeName = _T("HSLM A");
//       MvhlD.SelVehicle.Empty();
//     }
		MvhlD.VehicleTypeName.Empty();
		MvhlD.SelVehicle.Empty();
		//////////////////////////////////////////////////////////////////////////////
		m_Data_RailTrafficLoad[m_nSubType-6] = MvhlD;
	}

	m_Data = m_Data_RailTrafficLoad[m_nSubType-6];

	m_edtTrainW1.SetEditUnit(m_Data.dTrainW1);
	m_edtTrainDD1.SetEditUnit(m_Data.dTrainDD1);
	m_edtTrainD1.SetEditUnit(m_Data.dTrainD1);
	m_edtTrainW2.SetEditUnit(m_Data.dTrainW2);
	m_edtTrainDD2.SetEditUnit(m_Data.dTrainDD2);
	m_edtTrainD2.SetEditUnit(m_Data.dTrainD2);
	m_edtHSLMANum.SetEditUnit(m_Data.nHSLMANum);
	m_edtHSLMALength.SetEditUnit(m_Data.dHSLMALength);
	m_edtHSLMASpacing.SetEditUnit(m_Data.dHSLMASpacing);
	m_edtHSLMAForce.SetEditUnit(m_Data.dHSLMAForce);
	m_edtHSLMBForce.SetEditUnit(m_Data.dHSLMBForce);

	m_edtDynEffFact1.SetEditUnit(m_Data.dPhiDynEff1);
	m_edtDynEffFact2.SetEditUnit(m_Data.dPhiDynEff2);
	m_edtHSLMBNum.SetEditUnit(m_Data.nHSLMBNum);
	m_edtHSLMBDist.SetEditUnit(m_Data.dHSLMBDist);

	m_edtAlpha.SetEditUnit(m_Data.dFactorofVLoad);
	m_bLongiDist = m_Data.bLongiDist;
	m_edtDistPoint.SetEditUnit(m_Data.dRailSupPoints);

	m_bEccenVertLoad = m_Data.bEccenVertLoad;
	m_edtEccenVertLoad.SetEditUnit(m_Data.dEccenVertLoad);

	if(m_nSubType==6)
	{
		MakeItemEx4TruckList(m_nSubType);
	}
	else if(m_nSubType==7||m_nSubType==8||m_nSubType==9)
	{
		MakeItemEx4TrainList();
	}
}

BOOL CCMMvhlItemUserEuroBSNewDlg::Dlg2DataRailTrafficLoad()
{
	m_Data_RailTrafficLoad[m_nSubType-6].dTrainW1 = m_edtTrainW1.GetEditValue();
	m_Data_RailTrafficLoad[m_nSubType-6].dTrainDD1 = m_edtTrainDD1.GetEditValue();
	m_Data_RailTrafficLoad[m_nSubType-6].dTrainD1 = m_edtTrainD1.GetEditValue();
	m_Data_RailTrafficLoad[m_nSubType-6].dTrainW2 = m_edtTrainW2.GetEditValue();
	m_Data_RailTrafficLoad[m_nSubType-6].dTrainDD2 = m_edtTrainDD2.GetEditValue();
	m_Data_RailTrafficLoad[m_nSubType-6].dTrainD2 = m_edtTrainD2.GetEditValue();

	m_Data_RailTrafficLoad[m_nSubType-6].nHSLMANum = m_edtHSLMANum.GetEditValue();
	m_Data_RailTrafficLoad[m_nSubType-6].dHSLMALength = m_edtHSLMALength.GetEditValue();
	m_Data_RailTrafficLoad[m_nSubType-6].dHSLMASpacing = m_edtHSLMASpacing.GetEditValue();
	m_Data_RailTrafficLoad[m_nSubType-6].dHSLMAForce = m_edtHSLMAForce.GetEditValue();
	m_Data_RailTrafficLoad[m_nSubType-6].dHSLMBForce = m_edtHSLMBForce.GetEditValue();

	m_Data_RailTrafficLoad[m_nSubType-6].dPhiDynEff1 = m_edtDynEffFact1.GetEditValue();
	m_Data_RailTrafficLoad[m_nSubType-6].dPhiDynEff2 = m_edtDynEffFact2.GetEditValue();
	m_Data_RailTrafficLoad[m_nSubType-6].nHSLMBNum = m_edtHSLMBNum.GetEditValue();
	m_Data_RailTrafficLoad[m_nSubType-6].dHSLMBDist = m_edtHSLMBDist.GetEditValue();

	m_Data_RailTrafficLoad[m_nSubType-6].dFactorofVLoad = m_edtAlpha.GetEditValue();
	m_Data_RailTrafficLoad[m_nSubType-6].bLongiDist = m_bLongiDist;
	m_Data_RailTrafficLoad[m_nSubType-6].dRailSupPoints = m_edtDistPoint.GetEditValue();
	m_Data_RailTrafficLoad[m_nSubType-6].bEccenVertLoad = m_bEccenVertLoad;
	m_Data_RailTrafficLoad[m_nSubType-6].dEccenVertLoad = m_edtEccenVertLoad.GetEditValue();

	if(m_nSubType>=9)
	{
		m_Data_RailTrafficLoad[m_nSubType-6].dAdjustment=1.0;
		m_Data_RailTrafficLoad[m_nSubType-6].dAdjustment2=1.0;
	}

	return TRUE;
}

void CCMMvhlItemUserEuroBSNewDlg::AlignControl(BOOL bInitial/*=FALSE*/)
{
	int nCodeType = m_nCodeType;
	int nSubType = m_nSubType;
	CRect rRef, rRef2;
	CRect rToMove;
	int nDistY, nDistX;

	UINT aFirstCtrlID[] = {IDC_CMD_ML_MVHL_ITEMU_LM1_GRUP, IDC_CMD_ML_MVHL_ITEMU_LM2_GRUP1, 
		IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_LOAD_CHK, IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_LOAD_CHK,
		IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_GRP
	};
	
	CArray<UINT, UINT> *aCtrlSet[] = {&m_aCtrlCom1, &m_aCtrlCom2, &m_aCtrlCom3, &m_aCtrlCom3, &m_aCtrlCom6};
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM1_GRUP)->GetWindowRect(rRef);
	for (int i = 0; i < sizeof(aFirstCtrlID)/sizeof(UINT); i++)
	{
		GetDlgItem(aFirstCtrlID[i])->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		if(i!=0) nDistY += globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistY(this, *aCtrlSet[i], nDistY);
	}
	
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM1_D_EDT)->GetWindowRect(rRef);
	
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_FLM3_INTERVAL_TXT)->GetWindowRect(rToMove);
	nDistY = (rRef.bottom + rRef.top)/2 - rToMove.top + globalUtils.ScaleByDPI(10);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlCom4, nDistY);

	if (nCodeType == D_MVHL_EURO_BS_ROADBRIDGE_FOOTBRIDGE)
	{
		switch (nSubType)
		{
		case 0: //LM1
			{
				GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM1_D_TXT)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM1_D_UNIT)->GetWindowRect(rRef2);	
				GetDlgItem(IDC_CMD_MVHL_PATCH_CHK)->GetWindowRect(rToMove);
				nDistX = rRef2.right - rToMove.left + globalUtils.ScaleByDPI(15);
				nDistY = rRef.top - rToMove.top;

				CDlgUtil::CtrlMoveDistXY(this, m_aCtrlPatch, nDistX, nDistY);
			}
			break;
		case 1: //LM2
			{
				GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_TLFACT_STC)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_PATCH_CHK)->GetWindowRect(rToMove);
				nDistX = rRef.left - rToMove.left;
				nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);

				CDlgUtil::CtrlMoveDistXY(this, m_aCtrlPatch, nDistX, nDistY);
			}
		default:
			break;
		}
	}
	else if(nCodeType==D_MVHL_EURO_BS_RAIL_TRAFFIC_LOAD)
	{
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM1_RDO)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM71_RDO)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
	  CDlgUtil::CtrlMoveDistY(this, m_aCtrlLoadTypeCode2, nDistY);

		switch(nSubType)
		{
		case 6:
		case 7:
		case 8:
		case 9:
			{
				GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM1_GRUP)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_EURO_LST)->GetWindowRect(rToMove);
				nDistY = rRef.top - rToMove.top + globalUtils.ScaleByDPI(10);
				CDlgUtil::CtrlMoveDistY(this, m_aListEtc, nDistY);
				
				GetDlgItem(IDC_CMD_MVHL_EURO_LST)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_EURO_ALPHA_STC)->GetWindowRect(rToMove);
				nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
				CDlgUtil::CtrlMoveDistY(this, m_aETC, nDistY);
			}
			break;
		case 10:
			{
				GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM1_GRUP)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_EURO_HSLM_A_N_STC)->GetWindowRect(rToMove);
				nDistY = rRef.top - rToMove.top + globalUtils.ScaleByDPI(10);
				CDlgUtil::CtrlMoveDistY(this, m_aHSLM_A, nDistY);
				
				GetDlgItem(IDC_CMD_MVHL_EURO_HSLM_A_P_STC)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_EURO_DYN_GRP)->GetWindowRect(rToMove);
				nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
				CDlgUtil::CtrlMoveDistY(this, m_aDynEff, nDistY);
				
				GetDlgItem(IDC_CMD_MVHL_EURO_DYN_GRP)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_EURO_ALPHA_STC)->GetWindowRect(rToMove);
				nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
				CDlgUtil::CtrlMoveDistY(this, m_aETC, nDistY);
			}
			break;
		case 11:
			{
				GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM1_GRUP)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_EURO_HSLM_B_N_STC)->GetWindowRect(rToMove);
				nDistY = rRef.top - rToMove.top + globalUtils.ScaleByDPI(10);
				CDlgUtil::CtrlMoveDistY(this, m_aHSLM_B, nDistY);
				
				GetDlgItem(IDC_CMD_MVHL_EURO_HSLM_B_P_STC)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_EURO_DYN_GRP)->GetWindowRect(rToMove);
				nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
				CDlgUtil::CtrlMoveDistY(this, m_aDynEff, nDistY);
				
				GetDlgItem(IDC_CMD_MVHL_EURO_DYN_GRP)->GetWindowRect(rRef);
				GetDlgItem(IDC_CMD_MVHL_EURO_ALPHA_STC)->GetWindowRect(rToMove);
				nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
				CDlgUtil::CtrlMoveDistY(this, m_aETC, nDistY);
			}
			break;
		}
	}
	
	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);

	Invalidate(TRUE);
	UpdateWindow();
}


BOOL CCMMvhlItemUserEuroBSNewDlg::ApplyOrOK()
{
	T_MVHL_D data;
	int nCodeType = GetCodeType();
	int nSubType = GetSubType(nCodeType);
	BOOL bSuccess = FALSE;

	Dlg2Data(nSubType);
	if(nCodeType==D_MVHL_EURO_BS_ROADBRIDGE_FOOTBRIDGE)
	{
		m_Data = m_Data_RoadFootBridge[nSubType];
	}
	else
	{
		m_Data = m_Data_RailTrafficLoad[nSubType-6];
	}
	m_Data.bStandard = FALSE;
	
	m_wndVehicleName.GetWindowText(m_Data.VehicleLoadName);
	m_Data.nSubType = nSubType;
	m_Data.nLoadType = 4;
	
	if(m_Data.nSubType != 5)
	{
		m_Data.PermitLoad.aPermitLoad.RemoveAll();
	}

	data = m_Data;
	if (m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, data);
	else					 bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(data);
	if (!bSuccess) return FALSE;

	return TRUE;
}

int CCMMvhlItemUserEuroBSNewDlg::GetTruckLoadCount(int nSubType)
{
	int i = 0;
	for (i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if(nSubType == 1)
		{
			if (m_Data_RoadFootBridge[1].dPointLoad[i] == 0.0) break;
		}
		else if(nSubType == 3)
		{
			if (m_Data_RoadFootBridge[3].dPointLoad[i] == 0.0) break;
		}
		else if(nSubType == 6)
		{
			if (m_Data_RailTrafficLoad[0].dPointLoad[i] == 0.0) break;
		}
		else {} // Nothing
	}
	return i;
}

int CCMMvhlItemUserEuroBSNewDlg::GetTruckDistanceCount(int nSubType)
{
	int i = 0;
	for (i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if(nSubType == 1)
		{
			if (m_Data_RoadFootBridge[1].dPointDistance[i] == 0.0) break;
		}
		else if(nSubType == 3)
		{
			if (m_Data_RoadFootBridge[3].dPointDistance[i] == 0.0) break;
		}
		else if(nSubType == 6)
		{
			if (m_Data_RailTrafficLoad[0].dPointDistance[i] == 0.0) break;
		}
		else {} // Nothing
	}
	return i;
}

BOOL CCMMvhlItemUserEuroBSNewDlg::GetLoadAndDistValue(double &dLoad, double &dDist)
{
	if (!CFormulaEdit::GetEditValue(&m_LM2Pedt, dLoad))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_load_value_));
		return FALSE;
	}
	if (!CFormulaEdit::GetEditValue(&m_LM2Dedt, dDist))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_distance_value_));
		return FALSE;
	}

	if (dLoad == 0.0) // Load 0은 허용 안함
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Load_value_must_not_be_ze));
		return FALSE;
	}
	return TRUE;
}

BEGIN_MESSAGE_MAP(CCMMvhlItemUserEuroBSNewDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemUserEuroBSNewDlg)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_CODE_CMB, OnSelchangeCmdMvhlCodeCmb)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM2_DELETE_BTN, OnCmdBtnLM2Delete)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM2_INSERT_BTN, OnCmdBtnLM2Insert)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM2_MODIFY_BTN, OnCmdBtnLM2Modify)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM2_ADD_BTN, OnCmdBtnLM2Add)
	ON_EN_CHANGE(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_NUM_EDT, OnChangeCmdMlcNumEdt1)
	ON_EN_CHANGE(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_NUM_EDT, OnChangeCmdMlcNumEdt2)
	ON_EN_CHANGE(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_NUM_EDT, OnChangeCmdMlcNumEdt3)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_ML_MVHL_ITEMU_LM2_LIST, OnItemchangedCmdLM2List)
	ON_WM_DESTROY()

	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_DYNAMIC_CHK,OnThreeDynamicChk1)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_AUTO_RDO,OnThreeCmdAutoRdo1)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_USER_RDO,OnThreeCmdAutoRdo1)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_DYNAMIC_CHK  ,OnThreeDynamicChk2)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_AUTO_RDO,OnThreeCmdAutoRdo2)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_USER_RDO,OnThreeCmdAutoRdo2)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_DYNAMIC_CHK,OnThreeDynamicChk3)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_AUTO_RDO,OnThreeCmdAutoRdo3)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_USER_RDO,OnThreeCmdAutoRdo3)

	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_FLM3_FIRST_PHI_CHK,OnTwoDynamicChk1)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_FLM3_SECOND_PHI_CHK,OnTwoDynamicChk2)
	
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_LOAD_CHK,OnThreeUseChk1)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_LOAD_CHK,OnThreeUseChk2)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_LOAD_CHK,OnThreeUseChk3)

	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM1_RDO, OnCmdMlMvhlItemTypeRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM2_RDO, OnCmdMlMvhlItemTypeRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_RDO, OnCmdMlMvhlItemTypeRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_RDO, OnCmdMlMvhlItemTypeRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_FLM3_RDO, OnCmdMlMvhlItemTypeRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_EURO_BS_RDO, OnCmdMlMvhlItemTypeRdo)

	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM71_RDO, OnCmdMlMvhlItemTypeRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LMSW_RDO, OnCmdMlMvhlItemTypeRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LMSW2_RDO, OnCmdMlMvhlItemTypeRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_UNLOADED_RDO, OnCmdMlMvhlItemTypeRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_HSLM_A_RDO, OnCmdMlMvhlItemTypeRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_HSLM_B_RDO, OnCmdMlMvhlItemTypeRdo)

	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_AXLE_SPACING_CHK, OnCmdLM3StraddAxleSpacingChk)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_DYNAMIC_CHK,      OnCmdLM3StraddDynamicChk)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_PHI_AUTO_RDO,     OnCmdLM3StraddPhiTypeRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_PHI_USER_RDO,     OnCmdLM3StraddPhiTypeRdo)

	ON_BN_CLICKED(IDC_CMD_MVHL_EURO_LONGI_DIST_CHK, OnCmdMvhlEuroLongiDistChk)
	ON_BN_CLICKED(IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_CHK, OnCmdMvhlEuroEccenVertLoadChk)
	ON_BN_CLICKED(IDC_CMD_MVHL_PATCH_CHK, OnChkPatch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUserEuroBSNewDlg message handlers

BOOL CCMMvhlItemUserEuroBSNewDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	CString strCodeName[] = { _LS(IDS_CMD_MVHL_EURO_BS_ROADBRIDGE_FOOTBRIDGE), _LS(IDS_CMD_MVHL_EURO_BS_TRAIL_LOAD) };
		
	int nCodeSeq[2];
	nCodeSeq[0] = D_MVHL_EURO_BS_ROADBRIDGE_FOOTBRIDGE;
	nCodeSeq[1] = D_MVHL_EURO_BS_RAIL_TRAFFIC_LOAD;

#if defined(_MGEN)
	for(int i = 0; i < 1; i++) 
	{
		CDlgUtil::CobxAddItem(m_wndCodeName, strCodeName[nCodeSeq[i]], nCodeSeq[i]);
	}
#else
	for(int i = 0; i < 2; i++) 
	{
		CDlgUtil::CobxAddItem(m_wndCodeName, strCodeName[nCodeSeq[i]], nCodeSeq[i]);
	}
#endif

	
	CDlgUtil::GetCtrlIDByIncRect(this,m_aCtrlCom1,IDC_CMD_ML_MVHL_ITEMU_LM1_GRUP,TRUE);

	CDlgUtil::GetCtrlIDByIncRect(this,m_aCtrlCom2,IDC_CMD_ML_MVHL_ITEMU_LM2_GRUP1,TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this,m_aCtrlCom2,IDC_CMD_ML_MVHL_ITEMU_LM2_GRUP2,TRUE);
	
	CDlgUtil::GetCtrlIDByIncRect(this,m_aCtrlCom3,IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP1,TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this,m_aCtrlCom3_1,IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP1,FALSE);

	CDlgUtil::GetCtrlIDByIncRect(this,m_aCtrlCom3,IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP2,TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this,m_aCtrlCom3_2,IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP2,FALSE);

	CDlgUtil::GetCtrlIDByIncRect(this,m_aCtrlCom3,IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP3,TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this,m_aCtrlCom3_3,IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP3,FALSE);

	m_aCtrlCom3.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_LOAD_CHK);
	m_aCtrlCom3.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_LOAD_CHK);
	m_aCtrlCom3.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_LOAD_CHK);

	CDlgUtil::GetCtrlIDByIncRect(this,m_aCtrlCom4,IDC_CMD_ML_MVHL_ITEMU_FLM3_GRUP,TRUE);

	m_aCtrlCom4.Add(IDC_CMD_ML_MVHL_ITEMU_FLM3_INTERVAL_TXT);
	m_aCtrlCom4.Add(IDC_CMD_ML_MVHL_ITEMU_FLM3_INTERVAL_EDT);
	m_aCtrlCom4.Add(IDC_CMD_ML_MVHL_ITEMU_FLM3_INTERVAL_UNT);
	
	if(!m_bModify)
	{
		T_MVHL_D MvhlD; MvhlD.Initialize();
		MvhlD.bStandard = TRUE;    
		MvhlD.VehicleTypeName = _T("Load Model 1");

		m_nCodeType = D_MVHL_EURO_BS_ROADBRIDGE_FOOTBRIDGE;
		m_nSubType = 0;
		MvhlD.nStandardCode = m_nCodeType+19;
		CVehlDB db(m_pDoc);  
		if (!db.GetStandardVehicleLoadValue(MvhlD)) MvhlD.Initialize();
		MvhlD.bStandard = FALSE;
		MvhlD.nSubType  = 0;
		MvhlD.VehicleTypeName.Empty();
		m_Data_RoadFootBridge[0] = MvhlD;

		m_Data.Initialize();
		m_Data.nSubType  = 0;
	}
	else
	{
		if(m_Data.nSubType>=0&&m_Data.nSubType<=5)
		{
			m_nCodeType = D_MVHL_EURO_BS_ROADBRIDGE_FOOTBRIDGE;
			if     (m_Data.nSubType == 0) m_Data_RoadFootBridge[0] = m_Data; // LM1
			else if(m_Data.nSubType == 1) m_Data_RoadFootBridge[1] = m_Data; // LM2
			else if(m_Data.nSubType == 2) m_Data_RoadFootBridge[2] = m_Data; // LM3
			else if(m_Data.nSubType == 3) m_Data_RoadFootBridge[3] = m_Data; // LM3 Straddlign
			else if(m_Data.nSubType == 4) m_Data_RoadFootBridge[4] = m_Data; // FLM3
			else if(m_Data.nSubType == 5) m_Data_RoadFootBridge[5] = m_Data; // Permit
			else {} // Nothing
		}
		else
		{
			m_nCodeType = D_MVHL_EURO_BS_RAIL_TRAFFIC_LOAD;
			if     (m_Data.nSubType == 6) m_Data_RailTrafficLoad[0] = m_Data; // LM71
			else if(m_Data.nSubType == 7) m_Data_RailTrafficLoad[1] = m_Data; // LM SW0
			else if(m_Data.nSubType == 8) m_Data_RailTrafficLoad[2] = m_Data; // LM SW2
			else if(m_Data.nSubType == 9) m_Data_RailTrafficLoad[3] = m_Data; // Unloaded Train
			else if(m_Data.nSubType ==10) m_Data_RailTrafficLoad[4] = m_Data; // HSLM A
			else if(m_Data.nSubType ==11) m_Data_RailTrafficLoad[5] = m_Data; // HSLM B
			else {} // Nothing
		}
		m_nSubType = m_Data.nSubType;
	}

	if(!m_bModify)
	{
		m_Data_RoadFootBridge[0].dAmplification = 1.0;
		m_Data_RoadFootBridge[0].dAmplification2[0] = 0.75;
		m_Data_RoadFootBridge[0].dAmplification2[1] = 0.4;
		m_Data_RoadFootBridge[1].dAdjustment = 1.0;
		m_Data_RoadFootBridge[1].dAdjustment2 = 1.0;
	}
	CDlgUtil::CobxSetCurSelItemData(m_wndCodeName, m_nCodeType);
 
	T_MVCD_D MvcdD; MvcdD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) return TRUE;
	if(MvcdD.nCodeType != D_MOVE_CODE_EURO_BS)
	{
		ASSERT(0);
		return FALSE;
	}
 
	AlignControl(TRUE);		  // 코드타입에 따라 프레임 정렬
	InitControls();
	InitUnit();
	ShowHideControls();

	m_edtTSFact.SetUnitType(0);
	m_edtUDLFact.SetUnitType(0);
	m_LM1Dunit.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dSpace);
	m_LM2Wunit.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPressureLoad);
	m_edtFLM3Interval.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untFLM3Interval.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtWheelSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untWheelSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMinSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untMinSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMaxSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untMaxSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	 
	ChangeBitMap(0);  // 먼저 한번 호출

	m_nSubType = m_Data.nSubType;
	SetSubType(m_nCodeType, m_Data.nSubType);
	Data2Dlg();
	
	OnCmdMlMvhlItemTypeRdo();  
	OnCmdLM3StraddAxleSpacingChk();
	OnCmdLM3StraddDynamicChk();
	OnCmdLM3StraddPhiTypeRdo();
 
	if(m_bModify)
	{ 
		m_wndVehicleName.SetWindowText(m_csOldName);
		GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);
	}

	ChangeCtrlText();
	ChangeEditText();
	
#if defined(_MGEN)
	GetDlgItem(IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_CHK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_EDT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_UNT)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_EURO_BS_RDO)->ShowWindow(SW_HIDE);
#endif
	  
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemUserEuroBSNewDlg::InitControls()
{
	m_wndLM1Grid.InitGrid();
	
	m_wndLM3Grid[0].InitGrid();
	m_wndLM3Grid[1].InitGrid();
	m_wndLM3Grid[2].InitGrid();
	
	m_LM3NOPedt[0].SetRange(0, 50);
	m_LM3NOPedt[1].SetRange(0, 50);
	m_LM3NOPedt[2].SetRange(0, 50);

	m_LM3NOPedt[0].SetInteger(TRUE);
	m_LM3NOPedt[1].SetInteger(TRUE);
	m_LM3NOPedt[2].SetInteger(TRUE);

	SetHeaderTitle4TrainList();   // header title
	SetHeaderTitle4TruckList();   // header title
}

void CCMMvhlItemUserEuroBSNewDlg::InitUnit()
{  
	m_untTrainW1     .SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_untTrainDD1    .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untTrainD1     .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untTrainW2     .SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_untTrainDD2    .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untTrainD2     .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untHSLMALength .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untHSLMASpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untHSLMAForce  .SetUnitType(D_UNITSYS_BASE_FORCE);
	m_untHSLMBForce  .SetUnitType(D_UNITSYS_BASE_FORCE);
	m_untHSLMBDist   .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untDistPoint   .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtEccenVertLoad.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untEccenVertLoad.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtPatchW.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untPatchW.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtPatchL.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untPatchL.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMMvhlItemUserEuroBSNewDlg::SetParamData(T_MVHL_D &ParamData) 
{ 
	m_Data.Initialize();
	m_csOldName = ParamData.VehicleLoadName; 
	m_bModify = TRUE;
	m_Data    = ParamData;
	if     (m_Data.nSubType == 0) m_Data_RoadFootBridge[0] = m_Data; // LM1
	else if(m_Data.nSubType == 1) m_Data_RoadFootBridge[1] = m_Data; // LM2
	else if(m_Data.nSubType == 2) m_Data_RoadFootBridge[2] = m_Data; // LM3
	else if(m_Data.nSubType == 3) m_Data_RoadFootBridge[3] = m_Data; // LM3 Straddlign
	else if(m_Data.nSubType == 4) m_Data_RoadFootBridge[4] = m_Data; // FLM3
	else if(m_Data.nSubType == 6) m_Data_RailTrafficLoad[0] = m_Data; 
	else if(m_Data.nSubType == 7) m_Data_RailTrafficLoad[1] = m_Data; 
	else if(m_Data.nSubType == 8) m_Data_RailTrafficLoad[2] = m_Data; 
	else if(m_Data.nSubType == 9) m_Data_RailTrafficLoad[3] = m_Data; 
	else if(m_Data.nSubType ==10) m_Data_RailTrafficLoad[4] = m_Data; 
	else if(m_Data.nSubType ==11) m_Data_RailTrafficLoad[5] = m_Data; 
	else {} // Nothing
}

void CCMMvhlItemUserEuroBSNewDlg::OnOK() 
{
	if (!ApplyOrOK()) return;
	
	CDialogMove::OnOK();
}

void CCMMvhlItemUserEuroBSNewDlg::OnSelchangeCmdMvhlCodeCmb() 
{
	int nCodeType = CDlgUtil::CobxGetCurSelItemData(m_wndCodeName, m_wndCodeName.GetCurSel());
	if(m_nCodeType!=nCodeType)
	{
		Data2Dlg();
		int nLoadType=0;
		if(nCodeType==D_MVHL_EURO_BS_ROADBRIDGE_FOOTBRIDGE)
		{
			CDlgUtil::CtrlRadioSetCheck(this,m_aCtrlLoadTypeCode1,nLoadType);
			m_nSubType=nLoadType;
		}
		else
		{
			CDlgUtil::CtrlRadioSetCheck(this,m_aCtrlLoadTypeCode2,nLoadType);
			m_nSubType=nLoadType+6;
		}

		m_nCodeType=nCodeType;

		ChangeData();
	}
}

void CCMMvhlItemUserEuroBSNewDlg::OnCmdApply() 
{
	ApplyOrOK();
}

void CCMMvhlItemUserEuroBSNewDlg::OnCmdBtnLM2Add() 
{
	int nSubType = GetSubType(m_nCodeType);
	
	int nLoadCount = GetTruckLoadCount(nSubType);
	int nDistCount = GetTruckDistanceCount(nSubType);
	
	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;

	if (nLoadCount >= D_MVHL_NUMLOAD) // MAX 갯수 초과시
	{
		CString csErr;
		csErr.Format(_LS(IDS_WG_CMD__ADDD__Error___Number_of_load_exceeded_l), D_MVHL_NUMLOAD);
		AfxMessageBox(csErr);
		return;
	}
	if (nLoadCount > nDistCount) // Last 입력된 경우 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Delete_last_item_and_try_));
		return;
	}

	if (dDist == 0.0) // Last
	{
		m_Data_RoadFootBridge[nSubType].dPointLoad[nLoadCount] = dLoad;
	}
	else
	{
		m_Data_RoadFootBridge[nSubType].dPointLoad[nLoadCount] = dLoad;
		m_Data_RoadFootBridge[nSubType].dPointDistance[nLoadCount] = dDist;
	}

	int no = nLoadCount+1;
	InsertItem4TruckList(nLoadCount, no, dLoad, dDist);

	int nCount = m_lstTruckL.GetItemCount();
	m_lstTruckL.SetItemState(nCount-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvhlItemUserEuroBSNewDlg::OnCmdBtnLM2Insert() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_lstTruckL.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_inser));
		return;
	}
	
	int nSubType = GetSubType(m_nCodeType);
	
	int nLoadCount = GetTruckLoadCount(nSubType);
	int nDistCount = GetTruckDistanceCount(nSubType);

	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;

	if (nLoadCount >= D_MVHL_NUMLOAD) // MAX 갯수 초과시
	{
		CString csErr;
		csErr.Format(_LS(IDS_WG_CMD__ADDD__Error___Number_of_load_exceeded_l), D_MVHL_NUMLOAD);
		AfxMessageBox(csErr);
		return;
	}
	
	if (dDist == 0.0)  // middle and dist is zero!
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Intermediate_distance_mus));
		return;
	}

	// 데이타 이동하기
	for (int i = D_MVHL_NUMLOAD -1; i > iItem; i--)
	{
		m_Data_RoadFootBridge[nSubType].dPointLoad[i] = m_Data_RoadFootBridge[nSubType].dPointLoad[i-1];
		m_Data_RoadFootBridge[nSubType].dPointDistance[i] = m_Data_RoadFootBridge[nSubType].dPointDistance[i-1];
	}

	// 데이타 삽입한다.
	m_Data_RoadFootBridge[nSubType].dPointLoad[iItem] = dLoad;
	m_Data_RoadFootBridge[nSubType].dPointDistance[iItem] = dDist;

	// List에 Item 삽입
	InsertItem4TruckList(iItem, iItem+1, dLoad, dDist);

	// List 번호 변경
	int nCount = m_lstTruckL.GetItemCount();
	CString csVal;
	for (int i = iItem+1; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_lstTruckL.SetItemText(i, 0, csVal);
	}
	m_lstTruckL.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvhlItemUserEuroBSNewDlg::OnCmdBtnLM2Modify() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_lstTruckL.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_modif));
		return;
	}
	
	int nSubType = GetSubType(m_nCodeType);
	
	int nLoadCount = GetTruckLoadCount(nSubType);
	int nDistCount = GetTruckDistanceCount(nSubType);

	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;

	if (iItem != nLoadCount-1 && dDist == 0.0)  // middle and dist is zero!
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Intermediate_distance_mus));
		return;
	}

	m_Data_RoadFootBridge[nSubType].dPointLoad[iItem] = dLoad;
	m_Data_RoadFootBridge[nSubType].dPointDistance[iItem] = dDist;

	ModifyItem4TruckList(iItem, iItem+1, dLoad, dDist);
}

void CCMMvhlItemUserEuroBSNewDlg::OnCmdBtnLM2Delete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_lstTruckL.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_delet));
		return;
	}

	int nSubType = GetSubType(m_nCodeType);
	
	int nLoadCount = GetTruckLoadCount(nSubType);
	int nDistCount = GetTruckDistanceCount(nSubType);
	int i = iItem;
	for (i = iItem; i < D_MVHL_NUMLOAD -1; i++)
	{
		m_Data_RoadFootBridge[nSubType].dPointLoad[i] = m_Data_RoadFootBridge[nSubType].dPointLoad[i+1];
		m_Data_RoadFootBridge[nSubType].dPointDistance[i] = m_Data_RoadFootBridge[nSubType].dPointDistance[i+1];
	}
	m_Data_RoadFootBridge[nSubType].dPointLoad[i] = 0.0;
	m_Data_RoadFootBridge[nSubType].dPointDistance[i] = 0.0;

	m_lstTruckL.DeleteItem(iItem);
	int nCount = m_lstTruckL.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_lstTruckL.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);

	// List 번호 변경
	CString csVal;
	for (int i = iItem; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_lstTruckL.SetItemText(i, 0, csVal);
	}
}

void CCMMvhlItemUserEuroBSNewDlg::OnItemchangedCmdLM2List(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iItem = m_lstTruckL.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	int nSubType = GetSubType(m_nCodeType);
	
	int nLoadCount = GetTruckLoadCount(nSubType);
	int nDistCount = GetTruckDistanceCount(nSubType);

	CString csVal;
	if (iItem == nLoadCount-1 && nLoadCount != nDistCount)  // fixed, last
	{
		csVal.Format(_T("%g"), m_Data_RoadFootBridge[nSubType].dPointLoad[iItem]);
		m_LM2Pedt.SetWindowText(csVal);
		m_LM2Dedt.SetWindowText(_T(""));
	}
	else
	{
		csVal.Format(_T("%g"), m_Data_RoadFootBridge[nSubType].dPointLoad[iItem]);
		m_LM2Pedt.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data_RoadFootBridge[nSubType].dPointDistance[iItem]);
		m_LM2Dedt.SetWindowText(csVal);
	}
	
	*pResult = 0;
}

void CCMMvhlItemUserEuroBSNewDlg::ChangeBitMap(int which)
{	

	CString aBitmapID[]   = { _T("cmd_mvhl_70.svg"),  _T("cmd_mvhl_71.svg"), _T("cmd_mvhl_72.svg"), _T("cmd_mvhl_72.svg"), _T("cmd_mvhl_73.svg"), _T("cmd_mvhl_73.svg"),
							_T("cmd_mvhl_94_1.svg"), _T("cmd_mvhl_95.svg"), _T("cmd_mvhl_95.svg"), _T("cmd_mvhl_98.svg"), _T("cmd_mvhl_96.svg"), _T("cmd_mvhl_97.svg")};

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + aBitmapID[which]);
}

void CCMMvhlItemUserEuroBSNewDlg::ShowHideControls()
{
	int nCodeType = GetCodeType();
	m_nSubType = GetSubType(m_nCodeType);

	BOOL bCom1 = FALSE, bCom2 = FALSE, bCom3 = FALSE, bCom4 = FALSE, bCom5 = FALSE, bCom6 = FALSE;
	BOOL bHSLM_A = FALSE, bHSLM_B = FALSE, bDynEff = FALSE, bETC = FALSE, bListEtc = FALSE;
	BOOL bPatch = FALSE;

	CDlgUtil::CtrlShowHide(this,m_aCtrlLoadTypeCode1,nCodeType==D_MVHL_EURO_BS_ROADBRIDGE_FOOTBRIDGE);
	CDlgUtil::CtrlShowHide(this,m_aCtrlLoadTypeCode2,nCodeType==D_MVHL_EURO_BS_RAIL_TRAFFIC_LOAD);
	ChangeBitMap(m_nSubType);

#if defined(_MGEN)
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_EURO_BS_RDO)->ShowWindow(SW_HIDE);
#endif

	if(nCodeType==D_MVHL_EURO_BS_ROADBRIDGE_FOOTBRIDGE)
	{
		switch(m_nSubType)
		{
		case 0:
			bCom1=TRUE;
			bPatch = TRUE;
			break;
		case 1:
			bCom2=TRUE;
			bPatch = TRUE;
			break;
		case 2:
			bCom3=TRUE;
			bCom5=TRUE;
			break;
		case 3:
			bCom6=TRUE;
			break;
		case 4:
			bCom3=TRUE;
			bCom4=TRUE;
			break;
		}
	}
	else if(nCodeType==D_MVHL_EURO_BS_RAIL_TRAFFIC_LOAD)
	{
		int nLoadType=m_nSubType-6;
		
		if     (nLoadType == 4)  { bHSLM_A = TRUE; bDynEff = TRUE; bETC = TRUE;}
		else if(nLoadType == 5)  { bHSLM_B = TRUE; bDynEff = TRUE; bETC = TRUE;}
		else if(nLoadType == 0 || nLoadType == 1 || nLoadType == 2 || nLoadType == 3)
		{
			bListEtc = TRUE; bETC = TRUE;
		}
	}
	
	CDlgUtil::CtrlShowHide(this,m_aCtrlCom1,bCom1);
	CDlgUtil::CtrlShowHide(this,m_aCtrlCom2,bCom2);
	CDlgUtil::CtrlShowHide(this,m_aCtrlCom3,bCom3);
	CDlgUtil::CtrlShowHide(this,m_aCtrlCom4,bCom4);
	CDlgUtil::CtrlShowHide(this,m_aCtrlCom5,bCom5);
	CDlgUtil::CtrlShowHide(this,m_aCtrlCom6,bCom6);

	CDlgUtil::CtrlShowHide(this,m_aListEtc, bListEtc);
	CDlgUtil::CtrlShowHide(this,m_aHSLM_A,  bHSLM_A);
	CDlgUtil::CtrlShowHide(this,m_aHSLM_B,  bHSLM_B);
	CDlgUtil::CtrlShowHide(this,m_aDynEff,  bDynEff);
	CDlgUtil::CtrlShowHide(this,m_aETC,     bETC);
	CDlgUtil::CtrlShowHide(this,m_aCtrlPatch, bPatch);
}

void CCMMvhlItemUserEuroBSNewDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	if(m_nCodeType!=D_MVHL_EURO_BS_RAIL_TRAFFIC_LOAD) return;
		
	int nSubType = GetSubType(m_nCodeType); 
	int nLoadType = nSubType-6;
	
	// Trail Load
	GetDlgItem(IDC_CMD_MVHL_EURO_ALPHA_STC)->EnableWindow(nLoadType==0 || nLoadType==1);
	GetDlgItem(IDC_CMD_MVHL_EURO_ALPHA_EDT)->EnableWindow(nLoadType==0 || nLoadType==1);
	GetDlgItem(IDC_CMD_MVHL_EURO_LONGI_DIST_CHK)->EnableWindow(nLoadType==0 || nLoadType==4);
	GetDlgItem(IDC_CMD_MVHL_EURO_DIST_POINT_STC)->EnableWindow((nLoadType==0 || nLoadType==4) && m_bLongiDist);
	GetDlgItem(IDC_CMD_MVHL_EURO_DIST_POINT_EDT)->EnableWindow((nLoadType==0 || nLoadType==4) && m_bLongiDist);
	GetDlgItem(IDC_CMD_MVHL_EURO_DIST_POINT_UNT)->EnableWindow((nLoadType==0 || nLoadType==4) && m_bLongiDist);

	GetDlgItem(IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_EDT)->EnableWindow(m_bEccenVertLoad);
	GetDlgItem(IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_UNT)->EnableWindow(m_bEccenVertLoad);
		
	GetDlgItem(IDC_CMD_MVHL_EURO_DD1_EDT)->EnableWindow(nLoadType==1 || nLoadType==2);
	GetDlgItem(IDC_CMD_MVHL_EURO_D1_EDT)->EnableWindow(nLoadType==0 ||nLoadType==1 || nLoadType==2);
	GetDlgItem(IDC_CMD_MVHL_EURO_W2_EDT)->EnableWindow(nLoadType==0 ||nLoadType==1 || nLoadType==2);
	GetDlgItem(IDC_CMD_MVHL_EURO_DD2_EDT)->EnableWindow(nLoadType==1 || nLoadType==2);
	GetDlgItem(IDC_CMD_MVHL_EURO_D2_EDT)->EnableWindow(nLoadType==0);
}

void CCMMvhlItemUserEuroBSNewDlg::ChangeData()
{
	m_nSubType = GetSubType(m_nCodeType);
	AlignControl();
	Data2Dlg();

	int nDistY=0;
	CRect rRef,rToMove;

	if(m_nSubType!=5)
	{
		ShowHideControls();
	}
	
	switch(m_nSubType)
	{
	case 0: 
		GetDlgItem(IDC_CMD_MVHL_PATCH_CHK)->SetWindowText(_LS(IDS_CMD_MVHL_EURO_BS_LM1));
		break;
	case 1:
		GetDlgItem(IDC_CMD_MVHL_PATCH_CHK)->SetWindowText(_LS(IDS_CMD_MVHL_EURO_BS_LM2));
		break;
	case 2:
		{
			//GetDlgItem(IDC_CMD_PLACE_HOLDER)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_PHI_EDT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP1)->GetWindowRect(rToMove);
			rToMove.bottom = rRef.bottom + 10;      
			this->ScreenToClient(&rToMove);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP1)->MoveWindow(rToMove);

			//GetDlgItem(IDC_CMD_PLACE_HOLDER)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP2)->GetWindowRect(rToMove);
			rToMove.bottom = rRef.bottom + 10;
			this->ScreenToClient(&rToMove);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP2)->MoveWindow(rToMove);

			//GetDlgItem(IDC_CMD_PLACE_HOLDER)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP3)->GetWindowRect(rToMove);
			rToMove.bottom = rRef.bottom + 10;
			this->ScreenToClient(&rToMove);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP3)->MoveWindow(rToMove);
			
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_LOAD_CHK)->SetWindowText(_LS(IDS_CMD_MVLD_loadcase1));
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_LOAD_CHK)->SetWindowText(_LS(IDS_CMD_MVLD_loadcase2));

			if(!m_LM3LCchk[0].GetCheck())
			{
				m_LM3LCchk[1].SetCheck(FALSE);
				m_LM3LCchk[2].SetCheck(FALSE);
			}

			OnThreeUseChk1();
			break;
		 }
	case 3:
		{
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_P_TXT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_P_EDT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_D_TXT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_D_EDT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_LIST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_ADD_BTN)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_INSERT_BTN)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_MODIFY_BTN)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_DELETE_BTN)->ShowWindow(SW_SHOW);
		}
		break;
	case 4:
		{
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_DYNAMIC_CHK)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP1)->GetWindowRect(rToMove);
			rToMove.bottom = (rRef.top+rRef.bottom)/2;      
			this->ScreenToClient(&rToMove);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP1)->MoveWindow(rToMove);

			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_DYNAMIC_CHK)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP2)->GetWindowRect(rToMove);
			rToMove.bottom = (rRef.top+rRef.bottom)/2;      
			this->ScreenToClient(&rToMove);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP2)->MoveWindow(rToMove);

			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_DYNAMIC_CHK)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP3)->GetWindowRect(rToMove);
			rToMove.bottom = (rRef.top+rRef.bottom)/2;      
			this->ScreenToClient(&rToMove);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP3)->MoveWindow(rToMove);

			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_LOAD_CHK)->SetWindowText(_LS(IDS_CMD_MVLD_ONE_VEHICLE));
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_LOAD_CHK)->SetWindowText(_LS(IDS_CMD_MVLD_TWO_VEHICLE));

			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_LOAD_CHK)->ShowWindow(FALSE);
			
			OnThreeUseChk1();
			OnThreeUseChk2();
			OnTwoDynamicChk1();
			OnTwoDynamicChk2();
			break;
		}
	case 5:
		{
			if (m_bPermitLoadCall)
			{
				m_wndVehicleName.GetWindowText(m_Data_RoadFootBridge[5].VehicleLoadName);
				m_bPermitLoadCall = FALSE;
				CCMMvhlItemUsrPermitDlg dlg;
				dlg.SetInitPos(D_INIT_POS_LT);
				CString csRadioText[3];
			
				T_MVCD_D MvcdD; MvcdD.Initialize();
			  if(!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) return;
				dlg.SetModifyData(m_bModify, m_csOldName, &m_Data_RoadFootBridge[5], m_pDoc, csRadioText, MvcdD.nCodeType);
				dlg.DoModal();
				//Type Change이면
				int nLastEvent = dlg.GetNextAction();
				if (nLastEvent == CCMMvhlItemUsrPermitDlg::CHANGE_TYPE)
				{
					CRect r = dlg.GetLastWindowPos();
					CRect rOrg;
					GetWindowRect(rOrg);
					r.right = r.left + rOrg.Width();
					r.bottom = r.top + rOrg.Height();
					MoveWindow(r);
					ShowWindow(SW_SHOW);
					m_wndVehicleName.SetWindowText(m_Data_RoadFootBridge[5].VehicleLoadName);

					int nLoadType = dlg.GetEuroBSType();
					m_nCodeType = dlg.GetCodeType();
					m_nSubType = m_nCodeType==D_MVHL_EURO_BS_ROADBRIDGE_FOOTBRIDGE? nLoadType: nLoadType+6;
					
					SetSubType(m_nCodeType, m_nSubType);
					ChangeData();
				}
				else
				{
					CDialogMove::OnOK();
				}
			}
			else
			{
				m_bPermitLoadCall = TRUE;
				PostMessage(WM_COMMAND, MAKEWPARAM(IDC_CMD_ML_MVHL_ITEMU_LM1_RDO, BN_CLICKED), 0);
			}
		}
		break;
	case 6:
		{
			GetDlgItem(IDC_CMD_MVHL_EURO_LST)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_GRP)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_P_TXT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_P_EDT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_D_TXT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_D_EDT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_LIST)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_ADD_BTN)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_INSERT_BTN)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_MODIFY_BTN)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_DELETE_BTN)->ShowWindow(SW_SHOW);
			EnableDisableControls();
		}
		break;
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
		EnableDisableControls();
		break;
	}

	ChangeEditText();
}

void CCMMvhlItemUserEuroBSNewDlg::ChangeEditText()
{
	if(m_nSubType == 6) // Load Model 71
	{
		m_edtTrainDD1.SetWindowText(CSpecialChar::m_csInfinity);
		m_edtTrainDD2.SetWindowText(CSpecialChar::m_csInfinity);
	}
	else if(m_nSubType == 7) // Load Model SW/0
	{
		m_edtTrainD2.SetWindowText(_T(""));
	}
	else if(m_nSubType == 8) // Load Model SW/2
	{
		m_edtTrainD2.SetWindowText(_T(""));
	}
	else if(m_nSubType == 9) // Unloaded Train
	{
		m_edtTrainDD1.SetWindowText(CSpecialChar::m_csInfinity);
		m_edtTrainD1 .SetWindowText(_T(""));
		m_edtTrainW2 .SetWindowText(_T(""));
		m_edtTrainDD2.SetWindowText(_T(""));
		m_edtTrainD2 .SetWindowText(_T(""));
	}
}

void CCMMvhlItemUserEuroBSNewDlg::OnCmdMlMvhlItemTypeRdo()
{
	Dlg2Data(m_nSubType);
	ChangeData();
}

void CCMMvhlItemUserEuroBSNewDlg::OnChangeCmdMlcNumEdt1() 
{
	if(!IsWindow(m_LM3NOPedt[0]))return;

	int nSubType = GetSubType(m_nCodeType);
	int iLoadNum;
	if(CFormulaEdit::GetEditValue(&m_LM3NOPedt[0], iLoadNum))
	{
		if(iLoadNum>0)
		{
			int gridLines=m_Data_RoadFootBridge[nSubType].nNumLoad3[0];
			if(gridLines>iLoadNum)
			{
				for(int i=iLoadNum;i<gridLines;i++)
				{
					m_Data_RoadFootBridge[nSubType].dPointLoad3[0][i] = 0.0;
					m_Data_RoadFootBridge[nSubType].dPointDistance3[0][i] = 0.0;
				}
			}
			m_Data_RoadFootBridge[nSubType].nNumLoad3[0] = iLoadNum;
			m_wndLM3Grid[0].SetDataSource(&m_Data_RoadFootBridge[nSubType],0,1);
			return;   
		}
		
	}
	//cLoadNum.Format(_T("%d"),m_Data_RoadFootBridge[nSubType].nNumLoad3[0]);
	//m_LM3NOPedt[0].SetWindowText(cLoadNum);	
	
}
void CCMMvhlItemUserEuroBSNewDlg::OnChangeCmdMlcNumEdt2() 
{
	if(!IsWindow(m_LM3NOPedt[1]))return;

	int nSubType = GetSubType(m_nCodeType);
	int iLoadNum = 0;
	if(CFormulaEdit::GetEditValue(&m_LM3NOPedt[1], iLoadNum))
	{
		if(iLoadNum>0)
		{
			int gridLines=m_Data_RoadFootBridge[nSubType].nNumLoad3[0];
			if(gridLines>iLoadNum)
			{
				for(int i=iLoadNum;i<gridLines;i++)
				{
					m_Data_RoadFootBridge[nSubType].dPointLoad3[1][i] = 0.0;
					m_Data_RoadFootBridge[nSubType].dPointDistance3[1][i] = 0.0;
				}
			}
			m_Data_RoadFootBridge[nSubType].nNumLoad3[1] = iLoadNum;
			m_wndLM3Grid[1].SetDataSource(&m_Data_RoadFootBridge[nSubType],1,1);
			return;   
		}
		
	}
	//cLoadNum.Format(_T("%d"),m_Data_RoadFootBridge[nSubType].nNumLoad3[1]);
	//m_LM3NOPedt[1].SetWindowText(cLoadNum);
}
void CCMMvhlItemUserEuroBSNewDlg::OnChangeCmdMlcNumEdt3() 
{
	if(!IsWindow(m_LM3NOPedt[2]))return;
	
	int iLoadNum;
	int nSubType = GetSubType(m_nCodeType);
	if(CFormulaEdit::GetEditValue(&m_LM3NOPedt[2], iLoadNum))
	{
		if(iLoadNum>0)
		{
			int gridLines=m_Data_RoadFootBridge[nSubType].nNumLoad3[2];
			if(gridLines>iLoadNum)
			{
				for(int i=iLoadNum;i<gridLines;i++)
				{
					m_Data_RoadFootBridge[nSubType].dPointLoad3[2][i] = 0.0;
					m_Data_RoadFootBridge[nSubType].dPointDistance3[2][i] = 0.0;
				}
			}
			m_Data_RoadFootBridge[nSubType].nNumLoad3[2] = iLoadNum;
			m_wndLM3Grid[2].SetDataSource(&m_Data_RoadFootBridge[nSubType],2,1);
			return;   
		}
		
	}
	//cLoadNum.Format(_T("%d"),m_Data_RoadFootBridge[nSubType].nNumLoad3[2]);
	//m_LM3NOPedt[2].SetWindowText(cLoadNum);
}

void CCMMvhlItemUserEuroBSNewDlg::OnThreeDynamicChk1()
{
	CArray<UINT,UINT> aDynamicCtrls;
	BOOL bDynamic;
	bDynamic = m_LM3DAFchk[0].GetCheck();
	
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_AUTO_RDO);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_USER_RDO);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_PHI_TXT);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_PHI2_TXT);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_PHI_EDT);

	CDlgUtil::CtrlEnableDisable(this,aDynamicCtrls,bDynamic && m_LM3LCchk[0].GetCheck());
	OnThreeCmdAutoRdo1();

}
void CCMMvhlItemUserEuroBSNewDlg::OnThreeCmdAutoRdo1()
{
	CArray<UINT,UINT> aDynamicCtrls;
	CArray<UINT,UINT> aRadio;
	BOOL bDynamic;
	aRadio.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_AUTO_RDO);
	aRadio.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_USER_RDO);

	CDlgUtil::CtrlRadioGetCheck(this,aRadio,bDynamic);
	 
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_PHI_EDT);

	CDlgUtil::CtrlEnableDisable(this,aDynamicCtrls,bDynamic && m_LM3DAFchk[0].GetCheck() && m_LM3LCchk[0].GetCheck());
}
void CCMMvhlItemUserEuroBSNewDlg::OnThreeDynamicChk2()
{
	CArray<UINT,UINT> aDynamicCtrls;
	BOOL bDynamic;
	bDynamic = m_LM3DAFchk[1].GetCheck();
	
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_AUTO_RDO);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_USER_RDO);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_PHI_TXT);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_PHI2_TXT);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_PHI_EDT);

	CDlgUtil::CtrlEnableDisable(this,aDynamicCtrls,bDynamic && m_LM3LCchk[1].GetCheck());
	OnThreeCmdAutoRdo2();

}
void CCMMvhlItemUserEuroBSNewDlg::OnThreeCmdAutoRdo2()
{
	CArray<UINT,UINT> aDynamicCtrls;
	CArray<UINT,UINT> aRadio;
	BOOL bDynamic;
	aRadio.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_AUTO_RDO);
	aRadio.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_USER_RDO);

	CDlgUtil::CtrlRadioGetCheck(this,aRadio,bDynamic);
	 
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_PHI_EDT);

	CDlgUtil::CtrlEnableDisable(this,aDynamicCtrls,bDynamic && m_LM3DAFchk[1].GetCheck() && m_LM3LCchk[1].GetCheck());

}
void CCMMvhlItemUserEuroBSNewDlg::OnThreeDynamicChk3()
{
	CArray<UINT,UINT> aDynamicCtrls;
	BOOL bDynamic;
	bDynamic = m_LM3DAFchk[2].GetCheck();
	
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_AUTO_RDO);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_USER_RDO);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_PHI_TXT);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_PHI2_TXT);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_PHI_EDT);

	CDlgUtil::CtrlEnableDisable(this,aDynamicCtrls,bDynamic && m_LM3LCchk[2].GetCheck());
	OnThreeCmdAutoRdo3();

}
void CCMMvhlItemUserEuroBSNewDlg::OnThreeCmdAutoRdo3()
{
	CArray<UINT,UINT> aDynamicCtrls;
	CArray<UINT,UINT> aRadio;
	BOOL bDynamic;
	aRadio.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_AUTO_RDO);
	aRadio.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_USER_RDO);
	CDlgUtil::CtrlRadioGetCheck(this,aRadio,bDynamic);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_PHI_EDT);
	CDlgUtil::CtrlEnableDisable(this,aDynamicCtrls,bDynamic && m_LM3DAFchk[2].GetCheck() && m_LM3LCchk[2].GetCheck());

}

void CCMMvhlItemUserEuroBSNewDlg::OnTwoDynamicChk1()
{
	BOOL bDynamic = m_FLM3DAFchk[0].GetCheck();
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_FLM3_FIRST_PHI_EDT)->EnableWindow(bDynamic);
}

void CCMMvhlItemUserEuroBSNewDlg::OnTwoDynamicChk2()
{
	BOOL bDynamic = m_FLM3DAFchk[1].GetCheck();
	
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_FLM3_SECOND_PHI_EDT)->EnableWindow(bDynamic);
}

void CCMMvhlItemUserEuroBSNewDlg::OnThreeUseChk1()
{
	BOOL bDynamic;
	bDynamic = m_LM3LCchk[0].GetCheck();
	CDlgUtil::CtrlEnableDisable(this,m_aCtrlCom3_1,bDynamic);
	m_wndLM3Grid[0].SetEnable(bDynamic);
	
	int nSubType = GetSubType(m_nCodeType);
	if(nSubType==2)
	{
		if(!bDynamic)
		{
			m_LM3LCchk[1].SetCheck(FALSE);
			m_LM3LCchk[2].SetCheck(FALSE);
		}
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_LOAD_CHK)->EnableWindow(bDynamic);
		OnThreeUseChk2();
		OnThreeUseChk3();
	}
	else if(nSubType==3)
	{
		if(!bDynamic) m_FLM3DAFchk[0].SetCheck(FALSE);
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_FLM3_FIRST_PHI_CHK)->EnableWindow(bDynamic);

		OnThreeUseChk2(); // added by mylee
		OnThreeUseChk3(); // added by mylee

		OnTwoDynamicChk1();
	}
	
	OnThreeDynamicChk1();
}
void CCMMvhlItemUserEuroBSNewDlg::OnThreeUseChk2()
{
	BOOL bDynamic;
	bDynamic = m_LM3LCchk[1].GetCheck();
	CDlgUtil::CtrlEnableDisable(this,m_aCtrlCom3_2,bDynamic);
	m_wndLM3Grid[1].SetEnable(bDynamic);
	CArray<UINT,UINT> arTemp;
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_LOAD_CHK);
	if(GetSubType(m_nCodeType) == 4)
	{
		// FLM3 3번째 Load Case 무조건 비활성화
		m_LM3LCchk[2].SetCheck(FALSE);
		if(!bDynamic)
		{
			m_FLM3DAFchk[1].SetCheck(FALSE);
		}
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_FLM3_SECOND_PHI_CHK)->EnableWindow(bDynamic);
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_FLM3_INTERVAL_EDT)  ->EnableWindow(bDynamic);
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_FLM3_INTERVAL_UNT)  ->EnableWindow(bDynamic);
		OnTwoDynamicChk2();
	}
	else
	{
		if(!bDynamic)
			m_LM3LCchk[2].SetCheck(FALSE);
	}
	OnThreeUseChk3();
	CDlgUtil::CtrlEnableDisable(this,arTemp,bDynamic);
	OnThreeDynamicChk2();

}
void CCMMvhlItemUserEuroBSNewDlg::OnThreeUseChk3()
{
	BOOL bDynamic;
	bDynamic = m_LM3LCchk[2].GetCheck();
	CDlgUtil::CtrlEnableDisable(this,m_aCtrlCom3_3,bDynamic);
	m_wndLM3Grid[2].SetEnable(bDynamic);
	OnThreeDynamicChk3();
}
int CCMMvhlItemUserEuroBSNewDlg::GetCodeType()
{
	int nCodeType = 0;
	nCodeType = CDlgUtil::CobxGetCurSelItemData(m_wndCodeName, m_wndCodeName.GetCurSel());

	return nCodeType;
}
int CCMMvhlItemUserEuroBSNewDlg::GetSubType(int nCodeType/*=D_MVHL_EURO_BS_ROADBRIDGE_FOOTBRIDGE*/)
{
	int nSubType=0;
	int nLoadType=0;
	if(nCodeType==D_MVHL_EURO_BS_ROADBRIDGE_FOOTBRIDGE)
	{
		CDlgUtil::CtrlRadioGetCheck(this,m_aCtrlLoadTypeCode1,nLoadType);
		nSubType = nLoadType;
	}
	else
	{
		CDlgUtil::CtrlRadioGetCheck(this,m_aCtrlLoadTypeCode2,nLoadType);
		nSubType = nLoadType+6;
	}
	return nSubType;
}
void CCMMvhlItemUserEuroBSNewDlg::SetSubType(int nCodeType, int nSubType)
{
	if(nCodeType==D_MVHL_EURO_BS_ROADBRIDGE_FOOTBRIDGE)
		CDlgUtil::CtrlRadioSetCheck(this,m_aCtrlLoadTypeCode1,nSubType);
	else
	{
		CDlgUtil::CtrlRadioSetCheck(this,m_aCtrlLoadTypeCode2,nSubType-6);
	}
}

void CCMMvhlItemUserEuroBSNewDlg::ChangeCtrlText()
{
	CString strText;
#if defined(_US) || defined(_RUS)
	strText.Format(_LS(IDS_CMD_MVHL_EURO_BS_s_FACTOR), CSpecialChar::m_csPsi);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_UDLFACT_STC)->SetWindowText(strText);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_TLFACT_STC)->SetWindowText(strText);
	
	strText.Format(_LS(IDS_CMD_MVHL_EURO_BS_s_FACTOR_FOR_TANDEM_SYSTEM), CSpecialChar::m_csPsi);
	GetDlgItem(IDC_CMD_MVHL_TSFACT_STC)->SetWindowText(strText);
	strText.Format(_LS(IDS_CMD_MVHL_EURO_BS_s_FACTOR_FOR_UDL_SYSTEM), CSpecialChar::m_csPsi);
	GetDlgItem(IDC_CMD_MVHL_UDLFACT_STC)->SetWindowText(strText);
	
	strText.Format(_T("%s :"), CSpecialChar::m_csPhi);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM1_DAF_TXT)->SetWindowText(strText);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_PHI2_TXT)->SetWindowText(strText);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_PHI2_TXT)->SetWindowText(strText);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_PHI2_TXT)->SetWindowText(strText);
	
	strText.Format(_T("%s1"), CSpecialChar::m_csPhi);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_FLM3_FIRST_PHI_CHK)->SetWindowText(strText);
	strText.Format(_T("%s2"), CSpecialChar::m_csPhi);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_FLM3_SECOND_PHI_CHK)->SetWindowText(strText);
	
	strText.Format(_T("%s = 1.40 -L / 500  \n     (1<= %s<= 1.40 )"), CSpecialChar::m_csPhi, CSpecialChar::m_csPhi);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_PHI_TXT)->SetWindowText(strText);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_PHI_TXT)->SetWindowText(strText);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_PHI_TXT)->SetWindowText(strText);
#endif

	strText.Format(_LS(IDS_CMD_MVHL_EURO_BS_VERTICAL_LOADS), CSpecialChar::m_csAlpha);
	GetDlgItem(IDC_CMD_MVHL_EURO_ALPHA_STC)->SetWindowText(strText);
}

void CCMMvhlItemUserEuroBSNewDlg::OnCmdLM3StraddAxleSpacingChk()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_AXLE_NUM_STC)->EnableWindow(m_bAxleSpacing);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_AXLE_NUM_EDT)->EnableWindow(m_bAxleSpacing);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_MIN_SPACING_STC)->EnableWindow(m_bAxleSpacing);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_MIN_SPACING_EDT)->EnableWindow(m_bAxleSpacing);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_MIN_SPACING_UNT)->EnableWindow(m_bAxleSpacing);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_MAX_SPACING_STC)->EnableWindow(m_bAxleSpacing);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_MAX_SPACING_EDT)->EnableWindow(m_bAxleSpacing);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_MAX_SPACING_UNT)->EnableWindow(m_bAxleSpacing);
}

void CCMMvhlItemUserEuroBSNewDlg::OnCmdLM3StraddDynamicChk()
{
	UpdateData(TRUE);
	
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_PHI_AUTO_RDO)->EnableWindow(m_bDynamic);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_PHI_USER_RDO)->EnableWindow(m_bDynamic);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_PHI_STC)->EnableWindow(m_bDynamic && m_nDynaType==1);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_PHI_EDT)->EnableWindow(m_bDynamic && m_nDynaType==1);
}

void CCMMvhlItemUserEuroBSNewDlg::OnCmdLM3StraddPhiTypeRdo()
{
	UpdateData(TRUE);
	
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_PHI_STC)->EnableWindow(m_bDynamic && m_nDynaType==1);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_PHI_EDT)->EnableWindow(m_bDynamic && m_nDynaType==1);
}

void CCMMvhlItemUserEuroBSNewDlg::OnCmdMvhlEuroLongiDistChk()
{
	EnableDisableControls();
}

void CCMMvhlItemUserEuroBSNewDlg::OnCmdMvhlEuroEccenVertLoadChk()
{
	EnableDisableControls();
}

void CCMMvhlItemUserEuroBSNewDlg::OnChkPatch()
{
	BOOL bPatchLoad = m_chkPatch.GetCheck();

	GetDlgItem(IDC_CMD_MVHL_PATCH_W_STC)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_W_EDT)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_W_UNT)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_STC)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_EDT)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_UNT)->EnableWindow(bPatchLoad);
}
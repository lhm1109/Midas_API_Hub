// CMMvhlItemUsrRusDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUsrRusDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrRusDlg dialog

#define COLCOUNT 3

CCMMvhlItemUsrRusDlg::CCMMvhlItemUsrRusDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemUsrRusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemUsrRusDlg)
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();

	m_bModify = FALSE;

	m_nDynaFact = 0;
	m_nLoadFact = 0;

	m_bFatigue = FALSE;
	m_bSameLoadedL = FALSE;
	m_bTwoVehi = FALSE;
	m_b2ndGrpReduc = FALSE;

	m_aTrkLoad.Add(IDC_CMD_P_STATIC);
	m_aTrkLoad.Add(IDC_CMD_P);
	m_aTrkLoad.Add(IDC_CMD_D_STATIC);
	m_aTrkLoad.Add(IDC_CMD_D);
	m_aTrkLoad.Add(IDC_CMD_LIST);
	m_aTrkLoad.Add(IDC_CMD_BTN_ADD);
	m_aTrkLoad.Add(IDC_CMD_BTN_INSERT);
	m_aTrkLoad.Add(IDC_CMD_BTN_MODIFY);
	m_aTrkLoad.Add(IDC_CMD_BTN_DELETE);

	m_aVariable.Add(IDC_CMD_MVHL_VARIABLE_STC);
	m_aVariable.Add(IDC_CMD_MVHL_VARIABLE_EDT);
	m_aVariable.Add(IDC_CMD_MVHL_VARIABLE_UNT);

	m_aRatioEmptyCar.Add(IDC_CMD_MVHL_RATIO_RATIO_STC);
	m_aRatioEmptyCar.Add(IDC_CMD_MVHL_RATIO_RATIO_EDT);

	m_aLaneP.Add(IDC_CMD_MVHL_P_STC);
	m_aLaneP.Add(IDC_CMD_MVHL_P_EDT);
	m_aLaneP.Add(IDC_CMD_MVHL_P_UNT);

	m_aLaneW1.Add(IDC_CMD_MVHL_W1_STC);
	m_aLaneW1.Add(IDC_CMD_MVHL_W1_EDT);
	m_aLaneW1.Add(IDC_CMD_MVHL_W1_UNT);

	m_aLaneW2.Add(IDC_CMD_MVHL_W2_STC);
	m_aLaneW2.Add(IDC_CMD_MVHL_W2_EDT);
	m_aLaneW2.Add(IDC_CMD_MVHL_W2_UNT);

	m_aLaneL.Add(IDC_CMD_MVHL_L_STC);
	m_aLaneL.Add(IDC_CMD_MVHL_L_EDT);
	m_aLaneL.Add(IDC_CMD_MVHL_L_UNT);

	m_aCtrl_Fatigue.Add(IDC_CMD_MVHL_FATIGUE_CHK);

	m_aCtrl_SameLoadedL.Add(IDC_CMD_MVHL_SAME_LOADED_L_CHK);

	m_ACtrl_TwoVehi.Add(IDC_CMD_MVHL_TWO_VEHI_GRP);
	m_ACtrl_TwoVehi.Add(IDC_CMD_MVHL_TWO_VEHI_CHK);
	m_ACtrl_TwoVehi.Add(IDC_CMD_MVHL_TWO_VEHI_FACT_STC);
	m_ACtrl_TwoVehi.Add(IDC_CMD_MVHL_TWO_VEHI_FACT_EDT);

	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_GRP);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_AUTO_RDO);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_AUTO_GRP);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_AUTO_MATL_STC);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_AUTO_MATL_EDT);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_AUTO_BRIDGE_STC);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_AUTO_BRIDGE_EDT);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_AUTO_FACT_STC);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_AUTO_FACT_EDT);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_USER_RDO);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_USER_GRP);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_USER_FACT_STC);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_USER_FACT_EDT);

	m_aCtrl_DynaFact_UDL.Add(IDC_CMD_MVHL_DYNA_FACT_UDL_USER_FACT_STC);
	m_aCtrl_DynaFact_UDL.Add(IDC_CMD_MVHL_DYNA_FACT_UDL_USER_FACT_EDT);

	m_aCtrl_LoadFact.Add(IDC_CMD_MVHL_LOAD_FACT_GRP);
	m_aCtrl_LoadFact.Add(IDC_CMD_MVHL_LOAD_FACT_AUTO_RDO);
	m_aCtrl_LoadFact.Add(IDC_CMD_MVHL_LOAD_FACT_USER_RDO);
	m_aCtrl_LoadFact.Add(IDC_CMD_MVHL_LOAD_FACT_USER_GRP);
	m_aCtrl_LoadFact.Add(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_STC);
	m_aCtrl_LoadFact.Add(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_EDT);
	
	m_aCtrl_LoadFact_UDL.Add(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_UDL_STC);
	m_aCtrl_LoadFact_UDL.Add(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_UDL_EDT);

	m_aCtrl_2ndGrpReduc.Add(IDC_CMD_MVHL_2ND_REDUC_GRP);
	m_aCtrl_2ndGrpReduc.Add(IDC_CMD_MVHL_2ND_REDUC_CHK);
	m_aCtrl_2ndGrpReduc.Add(IDC_CMD_MVHL_2ND_REDUC_EDT);

	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_GRP);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_LANE1_STC);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_LANE2_STC);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_LANE3_STC);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_BOGIE_STC);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_BOGIE_EDT);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_BOGIE_EDT2);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_BOGIE_EDT3);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_UDL_STC);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_UDL_EDT);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_UDL_EDT2);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_UDL_EDT3);
	
	m_aCtrl_OKCancel.Add(IDOK);
	m_aCtrl_OKCancel.Add(IDCANCEL);
	m_aCtrl_OKCancel.Add(IDC_CMD_APPLY);

	m_Data.Initialize();

	m_nRussiaUserType = 0;
}

void CCMMvhlItemUsrRusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemUsrRusDlg)  
	DDX_Control(pDX, IDC_CMD_VEHICLE_NAME,      m_edtVehicleName);
	DDX_Control(pDX, IDC_CMD_VEHICLE_TYPE_CMB,  m_cmbVehicleType);
	DDX_Control(pDX, IDC_CMD_P,						      m_edtTrkP);
	DDX_Control(pDX, IDC_CMD_D,					        m_edtTrkD);
	DDX_Control(pDX, IDC_CMD_LIST,				      m_List);
	DDX_Control(pDX, IDC_CMD_MVHL_VARIABLE_EDT, m_edtVariable);
	DDX_Control(pDX, IDC_CMD_MVHL_VARIABLE_UNT, m_untVariable);
	DDX_Control(pDX, IDC_CMD_MVHL_W1_EDT, m_edtLaneW1);
	DDX_Control(pDX, IDC_CMD_MVHL_W1_UNT, m_untLaneW1);
	DDX_Control(pDX, IDC_CMD_MVHL_L_EDT,  m_edtLaneL);
	DDX_Control(pDX, IDC_CMD_MVHL_L_UNT,  m_untLaneL);
	DDX_Control(pDX, IDC_CMD_MVHL_W2_EDT, m_edtLaneW2);
	DDX_Control(pDX, IDC_CMD_MVHL_W2_UNT, m_untLaneW2);
	DDX_Control(pDX, IDC_CMD_MVHL_P_EDT,  m_edtLaneP);
	DDX_Control(pDX, IDC_CMD_MVHL_P_UNT,  m_untLaneP);
	DDX_Radio  (pDX, IDC_CMD_MVHL_DYNA_FACT_AUTO_RDO,           m_nDynaFact);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_FACT_AUTO_MATL_EDT,      m_edtMatlType);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_FACT_AUTO_BRIDGE_EDT,    m_edtBridgeType);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_FACT_AUTO_FACT_EDT,      m_edtAutoDynaFact);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_FACT_USER_FACT_EDT,      m_edtUserDynaFact);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_FACT_UDL_USER_FACT_EDT,  m_edtUserDynaFact_UDL);
	DDX_Check  (pDX, IDC_CMD_MVHL_FATIGUE_CHK,                  m_bFatigue);
	DDX_Check  (pDX, IDC_CMD_MVHL_SAME_LOADED_L_CHK,            m_bSameLoadedL);
	DDX_Radio  (pDX, IDC_CMD_MVHL_LOAD_FACT_AUTO_RDO,           m_nLoadFact);
	DDX_Control(pDX, IDC_CMD_MVHL_LOAD_FACT_USER_FACT_EDT,      m_edtUserLoadFact);
	DDX_Control(pDX, IDC_CMD_MVHL_LOAD_FACT_USER_FACT_UDL_EDT,  m_edtUserLoadFact_UDL);
	DDX_Control(pDX, IDC_CMD_MVHL_RATIO_RATIO_EDT, m_edtRatioEmptyCar);
	DDX_Check  (pDX, IDC_CMD_MVHL_TWO_VEHI_CHK,       m_bTwoVehi);
	DDX_Control(pDX, IDC_CMD_MVHL_TWO_VEHI_FACT_EDT,  m_edtTwoVehi);
	DDX_Check  (pDX, IDC_CMD_MVHL_2ND_REDUC_CHK,      m_b2ndGrpReduc);
	DDX_Control(pDX, IDC_CMD_MVHL_2ND_REDUC_EDT,      m_edt2ndGrpReduc);
	DDX_Control(pDX, IDC_CMD_MVHL_LANE_FACT_S1_BOGIE_EDT,   m_edtLaneFacts1_Bogie[0]);
	DDX_Control(pDX, IDC_CMD_MVHL_LANE_FACT_S1_BOGIE_EDT2,  m_edtLaneFacts1_Bogie[1]);
	DDX_Control(pDX, IDC_CMD_MVHL_LANE_FACT_S1_BOGIE_EDT3,  m_edtLaneFacts1_Bogie[2]);
	DDX_Control(pDX, IDC_CMD_MVHL_LANE_FACT_S1_UDL_EDT,     m_edtLaneFacts1_UDL[0]);
	DDX_Control(pDX, IDC_CMD_MVHL_LANE_FACT_S1_UDL_EDT2,    m_edtLaneFacts1_UDL[1]);
	DDX_Control(pDX, IDC_CMD_MVHL_LANE_FACT_S1_UDL_EDT3,    m_edtLaneFacts1_UDL[2]);
	DDX_Control(pDX, IDC_BITMAP_STATIC, m_wndPicture);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMMvhlItemUsrRusDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemUsrRusDlg)
	ON_CBN_SELCHANGE(IDC_CMD_VEHICLE_TYPE_CMB, OnCmdMvhlVehicleTypeCmb)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD,    OnCmdMvhlAddBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_INSERT, OnCmdMvhlInsBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdMvhlModBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdMvhlDelBtn)
	ON_BN_CLICKED(IDC_CMD_APPLY,      OnCmdMvhlApplyBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST, OnItemchangedCmdList)
	ON_BN_CLICKED(IDC_CMD_MVHL_FATIGUE_CHK,        OnCmdMvhlFatigueChk)
	ON_BN_CLICKED(IDC_CMD_MVHL_DYNA_FACT_AUTO_RDO, OnCmdMvhlDynaFactRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_DYNA_FACT_USER_RDO, OnCmdMvhlDynaFactRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_LOAD_FACT_AUTO_RDO, OnCmdMvhlLoadFactRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_LOAD_FACT_USER_RDO, OnCmdMvhlLoadFactRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_TWO_VEHI_CHK,       OnCmdMvhlTwoVehiChk)
	ON_BN_CLICKED(IDC_CMD_MVHL_2ND_REDUC_CHK,      OnCmdMvhl2ndReducChk)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvhlItemUsrRusDlg::InitUnit()
{
	m_edtLaneW1.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_untLaneW1.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_edtLaneL.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untLaneL.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtLaneW2.SetUnitType(D_UNITSYS_BASE_PRESSURE);
	m_untLaneW2.SetUnitType(D_UNITSYS_BASE_PRESSURE);
	m_edtLaneP.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_untLaneP.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_edtVariable.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untVariable.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMMvhlItemUsrRusDlg::InitCombo()
{
	m_cmbVehicleType.ResetContent();
	m_cmbVehicleType.AddString(_LS(IDS_CMD_MVHL_AK));
	m_cmbVehicleType.AddString(_LS(IDS_CMD_MVHL_SUBWAY_TRAINS));
	m_cmbVehicleType.AddString(_LS(IDS_CMD_MVHL_TRAMCARS));
	m_cmbVehicleType.AddString(_LS(IDS_CMD_MVHL_N14));
	m_cmbVehicleType.AddString(_LS(IDS_CMD_MVHL_N11));
	m_cmbVehicleType.AddString(_LS(IDS_CMD_MVHL_NK80));
	m_cmbVehicleType.AddString(_LS(IDS_CMD_MVHL_NG60));
	m_cmbVehicleType.AddString(_LS(IDS_CMD_MVHL_WALKWAY_PEDESTRIAN));
	m_cmbVehicleType.SetCurSel(0);
}

void CCMMvhlItemUsrRusDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistX = 0;
	int nDistY = 0;

	int nRussiaUserType = m_cmbVehicleType.GetCurSel();

	// Truck Load P
	GetDlgItem(IDC_CMD_P)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_P_STC)->GetWindowRect(rToMove);
	
	nDistX = rRef.left - rToMove.left - globalUtils.ScaleByDPI(15);
	nDistY = rRef.top - rToMove.top;
		
	CDlgUtil::CtrlMoveDistXY(this, m_aLaneP, nDistX, nDistY);
	
	// Lane Load w
	GetDlgItem(IDC_CMD_MVHL_W1_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_W2_STC)->GetWindowRect(rToMove);
	
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;
		
	CDlgUtil::CtrlMoveDistXY(this, m_aLaneW2, nDistX, nDistY);
	
	CRect rectTarget;
	CRect rect;
	CRect rectShort;

	///////////////////////////////////////////////////////////////////////
	// Two Vehicle
// 	GetDlgItem(IDC_CMD_MVHL_FATIGUE_CHK)->GetWindowRect(rRef);
// 	GetDlgItem(IDC_CMD_MVHL_TWO_VEHI_GRP)->GetWindowRect(rToMove);
// 	nDistY = rRef.top - rToMove.top;
// 	CDlgUtil::CtrlMoveDistY(this, m_ACtrl_TwoVehi, nDistY);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Dynamic Factor

	// User Input Group Box 크기
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_USER_GRP)->GetWindowRect(rectShort);
	
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_USER_FACT_EDT)->GetWindowRect(rect);
	nDistY = rect.bottom - rect.top + globalUtils.ScaleByDPI(30);
	
	if(nRussiaUserType == 0)
	{
		GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_UDL_USER_FACT_EDT)->GetWindowRect(rect);
#if !defined(_RUS)
		nDistY += rect.bottom - rect.top + globalUtils.ScaleByDPI(5);
#else
		nDistY += rect.bottom - rect.top + globalUtils.ScaleByDPI(14);
	#endif
	}
	
	rectShort.bottom = rectShort.top + nDistY;
	ScreenToClient(rectShort);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_USER_GRP)->MoveWindow(rectShort);
	
	// Dynamic Factor Group Box 크기
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_GRP)->GetWindowRect(rectShort);
	
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_GRP)->GetWindowRect(rect);
	rectShort.bottom = rectShort.top + rect.bottom - rect.top + nDistY + globalUtils.ScaleByDPI(35);
#if defined(_RUS)
	rectShort.bottom += globalUtils.ScaleByDPI(9);
#endif
	ScreenToClient(rectShort);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_GRP)->MoveWindow(rectShort);

	// Dynamic Factor 
// 	if(nRussiaUserType == 3 || nRussiaUserType == 4) GetDlgItem(IDC_CMD_MVHL_TWO_VEHI_GRP)->GetWindowRect(rRef);
// 	else                                             GetDlgItem(IDC_CMD_MVHL_FATIGUE_CHK)->GetWindowRect(rRef);
// 	
// 	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_GRP)->GetWindowRect(rToMove);
// 	
// 	nDistY = rRef.bottom - rToMove.top + 7;

	GetDlgItem(IDC_CMD_MVHL_TWO_VEHI_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_GRP)->GetWindowRect(rToMove);

	if(nRussiaUserType == 3 || nRussiaUserType == 4) 
	{
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(7);
	}
	else
	{
		nDistY = rRef.top - rToMove.top;
	}

	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_DynaFact, nDistY);

	if(nRussiaUserType == 0)
	{
		GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_USER_FACT_EDT)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_UDL_USER_FACT_EDT)->GetWindowRect(rToMove);
		
	#if !defined(_RUS)
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(5);
	#else
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(14);
	#endif
		
		CDlgUtil::CtrlMoveDistY(this, m_aCtrl_DynaFact_UDL, nDistY);
	}	
	
	///////////////////////////////////////////////////////////////////////
	// Load Reliability Factor
	
	// User Input Group Box 크기
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_GRP)->GetWindowRect(rectShort);
	
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_EDT)->GetWindowRect(rect);
	nDistY = rect.bottom - rect.top + globalUtils.ScaleByDPI(30);
	
	if(nRussiaUserType == 0)
	{
		GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_UDL_EDT)->GetWindowRect(rect);
		nDistY += rect.bottom - rect.top + globalUtils.ScaleByDPI(5);

	#if !defined(_RUS)
		nDistY += rect.bottom - rect.top + globalUtils.ScaleByDPI(5);
	#else
		nDistY += rect.bottom - rect.top + globalUtils.ScaleByDPI(14);
	#endif
	}
	
	rectShort.bottom = rectShort.top + nDistY;
	ScreenToClient(rectShort);
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_GRP)->MoveWindow(rectShort);
	
	// Load Factor Group Box 크기
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_GRP)->GetWindowRect(rectShort);
	
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_AUTO_RDO)->GetWindowRect(rect);
	rectShort.bottom = rectShort.top + rect.bottom - rect.top + nDistY + globalUtils.ScaleByDPI(45);
	ScreenToClient(rectShort);
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_GRP)->MoveWindow(rectShort);
	
// 	if(nRussiaUserType == 7)
// 	{
// 		GetDlgItem(IDC_CMD_MVHL_FATIGUE_CHK)->GetWindowRect(rRef);
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_GRP)->GetWindowRect(rRef);
// 	}

	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_GRP)->GetWindowRect(rToMove);

	if(nRussiaUserType == 7)
	{
		nDistY = rRef.top - rToMove.top;
	}
	else
	{
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(7);
	}
	
	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_LoadFact, nDistY);
	
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_EDT)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_UDL_EDT)->GetWindowRect(rToMove);
	
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(3);
	
	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_LoadFact_UDL, nDistY);
	
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Lane Factor (s1)
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_LANE_FACT_S1_GRP)->GetWindowRect(rToMove);
	
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(3);
	
	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_LaneFacts1, nDistY);
	///////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////////
	// Reduction for Limit State Group 2
	GetDlgItem(IDC_CMD_MVHL_LANE_FACT_S1_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_2ND_REDUC_GRP)->GetWindowRect(rToMove);
	
	nDistY = rRef.top - rToMove.top;
	
	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_2ndGrpReduc, nDistY);
	///////////////////////////////////////////////////////////////////////
	
	if(nRussiaUserType == 0) 
	{
		GetDlgItem(IDC_CMD_MVHL_SAME_LOADED_L_CHK)->GetWindowRect(rRef);
	}
	else if(nRussiaUserType == 3 || nRussiaUserType == 4)
	{
		GetDlgItem(IDC_CMD_MVHL_2ND_REDUC_GRP)->GetWindowRect(rRef);
	}
	else
	{
		GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_GRP)->GetWindowRect(rRef);
	}

	CRect rBotLeftGrp;
	GetDlgItem(IDC_CMD_MVHL_FATIGUE_CHK)->GetWindowRect(rBotLeftGrp);
	rRef.bottom = __max(rRef.bottom, rBotLeftGrp.bottom);

	//가장 아래 있는 Control 16 아래 System Button을 놓는다.
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);	
	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_OKCancel, nDistY);

	//System Button 기준으로 대화상자 크기를 지정한다.
	CRect rectLast;
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rectLast);

	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);
	
	RedrawWindow();
}

void CCMMvhlItemUsrRusDlg::ControlsShowHide()
{
	UpdateData(TRUE);

	int nRussiaUserType = m_cmbVehicleType.GetCurSel();
	
	CDlgUtil::CtrlShowHide(this, m_aTrkLoad,           TRUE);
	CDlgUtil::CtrlShowHide(this, m_aVariable,          TRUE);
	CDlgUtil::CtrlShowHide(this, m_aLaneP,             TRUE);
	CDlgUtil::CtrlShowHide(this, m_aLaneW1,            TRUE);
	CDlgUtil::CtrlShowHide(this, m_aLaneW2,            TRUE);
	CDlgUtil::CtrlShowHide(this, m_aLaneL,             TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_Fatigue,      TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_SameLoadedL,  FALSE);
	CDlgUtil::CtrlShowHide(this, m_ACtrl_TwoVehi,      TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_DynaFact,     TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_DynaFact_UDL, TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_LoadFact,     TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_LoadFact_UDL, TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_2ndGrpReduc,  TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_LaneFacts1,   TRUE);
	CDlgUtil::CtrlShowHide(this, m_aRatioEmptyCar,     FALSE);

	if(nRussiaUserType == 0 || nRussiaUserType == 1 || nRussiaUserType == 2 || 
		 nRussiaUserType == 3 || nRussiaUserType == 4 || nRussiaUserType == 5)
	{
		if(nRussiaUserType != 0) 
		{
			CDlgUtil::CtrlShowHide(this, m_aCtrl_DynaFact_UDL, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aCtrl_LoadFact_UDL, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aCtrl_LaneFacts1,   FALSE);
		}
		if(nRussiaUserType != 2) CDlgUtil::CtrlShowHide(this, m_aVariable, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aLaneP, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aLaneW2, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aLaneL, FALSE);
		if(nRussiaUserType == 1 || nRussiaUserType == 2) CDlgUtil::CtrlShowHide(this, m_aRatioEmptyCar, TRUE);
		if(nRussiaUserType == 3 || nRussiaUserType == 4) CDlgUtil::CtrlShowHide(this, m_aCtrl_Fatigue,  FALSE);
		if(nRussiaUserType == 0) CDlgUtil::CtrlShowHide(this, m_aCtrl_SameLoadedL,  TRUE);
		
		if(nRussiaUserType != 3 && nRussiaUserType != 4)
		{
			CDlgUtil::CtrlShowHide(this, m_ACtrl_TwoVehi,     FALSE);
			CDlgUtil::CtrlShowHide(this, m_aCtrl_2ndGrpReduc, FALSE);
		}
	}
	else if(nRussiaUserType == 6)
	{
		CDlgUtil::CtrlShowHide(this, m_aVariable, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aLaneP,    FALSE);
		CDlgUtil::CtrlShowHide(this, m_aLaneW2,   FALSE);
		CDlgUtil::CtrlShowHide(this, m_ACtrl_TwoVehi,      FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_DynaFact_UDL, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_LoadFact_UDL, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_2ndGrpReduc,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_LaneFacts1,   FALSE);
	}
	else if(nRussiaUserType == 7)
	{
		CDlgUtil::CtrlShowHide(this, m_aTrkLoad,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aVariable, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aLaneW1,   FALSE);
		CDlgUtil::CtrlShowHide(this, m_aLaneL,    FALSE);
		CDlgUtil::CtrlShowHide(this, m_ACtrl_TwoVehi,      FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_DynaFact,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_DynaFact_UDL, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_LoadFact_UDL, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_2ndGrpReduc,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_LaneFacts1,   FALSE);
	}
}

void CCMMvhlItemUsrRusDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	int nRussiaUserType = m_cmbVehicleType.GetCurSel();

	CDlgUtil::CtrlEnableDisable(this, m_aTrkLoad,  TRUE);
	CDlgUtil::CtrlEnableDisable(this, m_aVariable, TRUE);
	CDlgUtil::CtrlEnableDisable(this, m_aLaneP,    TRUE);
	CDlgUtil::CtrlEnableDisable(this, m_aLaneW1,   TRUE);
	CDlgUtil::CtrlEnableDisable(this, m_aLaneW2,   TRUE);
	CDlgUtil::CtrlEnableDisable(this, m_aLaneL,    TRUE);

	if(nRussiaUserType == 0)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aVariable, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aLaneP,  FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aLaneW2, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aLaneL,  FALSE);	
	}
	else if(nRussiaUserType == 1 || nRussiaUserType == 2 || nRussiaUserType == 3 || 
		      nRussiaUserType == 4 || nRussiaUserType == 5)
	{
		if(nRussiaUserType != 2) CDlgUtil::CtrlEnableDisable(this, m_aVariable, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aLaneP,   FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aLaneW1,  FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aLaneW2,  FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aLaneL,   FALSE);
	}
	else if(nRussiaUserType == 6)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aTrkLoad, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aVariable, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aLaneP,   FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aLaneW2,  FALSE);
	}
	else if(nRussiaUserType == 7)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aTrkLoad, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aVariable, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aLaneW1,  FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aLaneL,   FALSE);
	}

	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_MATL_STC)->EnableWindow(m_nDynaFact==0);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_MATL_EDT)->EnableWindow(m_nDynaFact==0);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_BRIDGE_STC)->EnableWindow(m_nDynaFact==0);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_BRIDGE_EDT)->EnableWindow(m_nDynaFact==0);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_FACT_STC)->EnableWindow(m_nDynaFact==0);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_FACT_EDT)->EnableWindow(m_nDynaFact==0);
	
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_USER_FACT_STC)->EnableWindow(m_nDynaFact==1);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_USER_FACT_EDT)->EnableWindow(m_nDynaFact==1);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_UDL_USER_FACT_STC)->EnableWindow(m_nDynaFact==1);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_UDL_USER_FACT_EDT)->EnableWindow(m_nDynaFact==1);
	
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_MATL_STC)->EnableWindow(nRussiaUserType!=5 && m_nDynaFact==0);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_MATL_EDT)->EnableWindow(nRussiaUserType!=5 && m_nDynaFact==0);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_BRIDGE_STC)->EnableWindow(nRussiaUserType!=5 && m_nDynaFact==0);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_BRIDGE_EDT)->EnableWindow(nRussiaUserType!=5 && m_nDynaFact==0);

	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_STC    )->EnableWindow(m_nLoadFact==1);
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_EDT    )->EnableWindow(m_nLoadFact==1);
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_UDL_STC)->EnableWindow(m_nLoadFact==1);
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_UDL_EDT)->EnableWindow(m_nLoadFact==1);

	BOOL bUseDynaAuto = TRUE;
	if(/*nRussiaUserType == 0 || */nRussiaUserType == 3 || nRussiaUserType == 4 || nRussiaUserType == 6) bUseDynaAuto = FALSE;
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_RDO)->EnableWindow(bUseDynaAuto);

	BOOL bUseLoadAuto = TRUE;
	if(nRussiaUserType == 3 || nRussiaUserType == 4 || nRussiaUserType == 5 || nRussiaUserType == 6) bUseLoadAuto = FALSE;
		
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_AUTO_RDO)->EnableWindow(bUseLoadAuto);

	GetDlgItem(IDC_CMD_MVHL_TWO_VEHI_FACT_STC)->EnableWindow(m_bTwoVehi);
	GetDlgItem(IDC_CMD_MVHL_TWO_VEHI_FACT_EDT)->EnableWindow(m_bTwoVehi);
	GetDlgItem(IDC_CMD_MVHL_2ND_REDUC_EDT)->EnableWindow(m_b2ndGrpReduc);

	// 첫번째 값은 비활성화 처리 한다.
	GetDlgItem(IDC_CMD_MVHL_LANE_FACT_S1_BOGIE_EDT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_MVHL_LANE_FACT_S1_UDL_EDT)->EnableWindow(FALSE);
}

void CCMMvhlItemUsrRusDlg::SetDynaFactEditBox()
{
	T_MVCT_D MvctD; MvctD.Initialize();
	m_pDoc->m_pAttrCtrl->GetMvct(MvctD); 

	int nRussiaUserType = m_cmbVehicleType.GetCurSel();

	if(nRussiaUserType == 0)
	{
		if(MvctD.nMatType_AK == 0)
		{
			m_edtMatlType.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_STL));
			
			if(MvctD.nBridgeType_AK == 0)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_STL_BRIDGE_1));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_STL_BRIDGE_1_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_STL_BRIDGE_1_LAMBDA));
			}
			else if(MvctD.nBridgeType_AK == 1)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_STL_BRIDGE_2));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_STL_BRIDGE_2_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_STL_BRIDGE_2_LAMBDA));
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(MvctD.nMatType_AK == 1)
		{
			m_edtMatlType.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_RC));
			
			if(MvctD.nBridgeType_AK == 0)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_RC_BRIDGE_1));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_RC_BRIDGE_1_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_RC_BRIDGE_1_LAMBDA));
			}
		}
	}
	else
	{
		if(MvctD.nMatType == 0)
		{
			m_edtMatlType.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL));
			
			if(MvctD.nBridgeType == 0)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_1));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_1_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_1_LAMBDA));
			}
			else if(MvctD.nBridgeType == 1)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_2));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_2_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_2_LAMBDA));
			}
			else if(MvctD.nBridgeType == 2)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_3));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_3_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_3_LAMBDA));
			}
			else if(MvctD.nBridgeType == 3)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_4));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_4_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_4_LAMBDA));
			}
			else if(MvctD.nBridgeType == 4)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_5));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_5_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_5_LAMBDA));
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(MvctD.nMatType == 1)
		{
			m_edtMatlType.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC));
			
			if(MvctD.nBridgeType == 0)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_1));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_1_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_1_LAMBDA));
			}
			else if(MvctD.nBridgeType == 1)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_2));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_2_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_2_LAMBDA));
			}
			else if(MvctD.nBridgeType == 2)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_3));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_3_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_3_LAMBDA));
			}
		}
	}
	
	if(nRussiaUserType==5)
	{
		m_edtAutoDynaFact.SetWindowText(_STR(_T("1.1 ~ 1.3")));
	}
	
	if(!m_bModify)
	{
		if     (nRussiaUserType == 0) m_edtUserDynaFact.SetWindowText(_STR(_T("1.4")));
		else if(nRussiaUserType == 6) m_edtUserDynaFact.SetWindowText(_STR(_T("1.1")));
		else                          m_edtUserDynaFact.SetWindowText(_STR(_T("1.0")));
	}
	else
	{
		m_edtUserDynaFact.SetEditUnit(m_Data.dDynaFactor);	
	}
}

void CCMMvhlItemUsrRusDlg::ChangeDynaFactText()
{
	int nRussiaUserType = m_cmbVehicleType.GetCurSel();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Dynamic Factor
	CString strDynaFact_Auto = _T("");
	CString strDynaFact_User = _T("");
	CString strDynaFact_UserUDL = _T("");
	
	if(m_bFatigue)
	{	
		strDynaFact_Auto = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_AUTO_FATIGUE);
		
		if(nRussiaUserType == 0)  strDynaFact_User = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_AK_BOGIE_FATIGUE);
		else                      strDynaFact_User = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_NOT_AK_FATIGUE);
		
		strDynaFact_UserUDL = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_USER_UDL_FATIGUE);
	}
	else
	{
		strDynaFact_Auto = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_AUTO);
		
		if(nRussiaUserType == 0)  strDynaFact_User = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_AK_BOGIE);
		else                        strDynaFact_User = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_NOT_AK);
		
		strDynaFact_UserUDL = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_USER_UDL);
	}
	
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_FACT_STC)->SetWindowText(strDynaFact_Auto);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_USER_FACT_STC)->SetWindowText(strDynaFact_User);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_UDL_USER_FACT_STC)->SetWindowText(strDynaFact_UserUDL);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Dynamic Factor - Auto Calculation
	CString strDynaAutoCalc = _T("");
	
	if(nRussiaUserType == 1 || nRussiaUserType == 2)
	{
		strDynaAutoCalc = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_AUTO_CALC_SNIP_SP);
	}
	else if(nRussiaUserType == 0 || nRussiaUserType == 5)
	{
		strDynaAutoCalc = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_AUTO_CALC_SNIP);
	}
	else
	{
		strDynaAutoCalc = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_AUTO_CALC);
	}

	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_RDO)->SetWindowText(strDynaAutoCalc);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load Reliability Factor 
	CString strLoadFact = _T("");

	if(nRussiaUserType == 0)
	{
		strLoadFact = _LS(IDS_CMD_MVHL_LOAD_RELIABILITY_FACTOR_AK_BOGIE);
	}
	else
	{
		strLoadFact = _LS(IDS_CMD_MVHL_LOAD_RELIABILITY_FACTOR_NOT_AK);
	}

	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_STC)->SetWindowText(strLoadFact);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void CCMMvhlItemUsrRusDlg::ChangeLoadFactText()
{
	int nRussiaUserType = m_cmbVehicleType.GetCurSel();
	
	CString strLoadFactText = _T("");

	if(nRussiaUserType == 1 || nRussiaUserType == 2)
	{
		if(m_bFatigue) strLoadFactText = _LS(IDS_CMD_MVHL_LOAD_RELIABILITY_FACTOR_AUTO_FATIGUE_SNIP_SP);
		else           strLoadFactText = _LS(IDS_CMD_MVHL_LOAD_RELIABILITY_FACTOR_AUTO_SNIP_SP);		
	}
	else if(nRussiaUserType == 0)
	{
		if(m_bFatigue) strLoadFactText = _LS(IDS_CMD_MVHL_LOAD_RELIABILITY_FACTOR_AUTO_FATIGUE_SNIP);
		else           strLoadFactText = _LS(IDS_CMD_MVHL_LOAD_RELIABILITY_FACTOR_AUTO_SNIP);		
	}
	else
	{
		if(m_bFatigue) strLoadFactText = _LS(IDS_CMD_MVHL_LOAD_RELIABILITY_FACTOR_AUTO_FATIGUE);
		else           strLoadFactText = _LS(IDS_CMD_MVHL_LOAD_RELIABILITY_FACTOR_AUTO);
	}
	
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_AUTO_RDO)->SetWindowText(strLoadFactText);
}

void CCMMvhlItemUsrRusDlg::SetHeaderTitle()
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
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);
	
	// Calculate width for each column
	nColWidth[0] = 28; nColWidth[1] = 70; nColWidth[2] = 80;
	
	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_RIGHT;
		
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvhlItemUsrRusDlg::MakeItemEx()
{
	m_List.DeleteAllItems();
	
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	
	if (nLoadCount == 0) return;
	int i = 0;
	for (i = 0; i < nDistCount; i++)
		InsertItem(i, i+1, m_Data.dPointLoad[i], m_Data.dPointDistance[i]);
	
	if (nLoadCount > nDistCount)
		InsertItem(i, i+1, m_Data.dPointLoad[i], 0.0);
}

BOOL CCMMvhlItemUsrRusDlg::InsertItem(int nPos, int nNo, double dLoad, double dDist)
{
	int nRussiaUserType = m_cmbVehicleType.GetCurSel();

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
			if(nRussiaUserType == 1)
			{
				str.Format(_T("%g"), dDist);
			}
			else
			{
				if (dDist == 0.0) str = _LS(IDS_WG_CMD__ADDD__end);
				else str.Format(_T("%g"), dDist);
			}
		}
		else str = _LS(IDS_WG_CMD__ADDD__Error);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;
		
		if(i == 0) m_List.InsertItem(&lvitem);
		else       m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	
	return TRUE;
}

BOOL CCMMvhlItemUsrRusDlg::DeleteItem(int nPos)
{
	if (nPos != -1) m_List.DeleteItem(nPos);
	return TRUE;
}

BOOL CCMMvhlItemUsrRusDlg::ModifyItem(int nPos, int nNo, double dLoad, double dDist)
{
	int nRussiaUserType = m_cmbVehicleType.GetCurSel();

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
				if(nRussiaUserType == 1)
				{
					str.Format(_T("%g"), dDist);
				}
				else
				{
					if (dDist == 0.0) str = _LS(IDS_WG_CMD__ADDD__end);
					else str.Format(_T("%g"), dDist);
				}
			}
			else str = _LS(IDS_WG_CMD__ADDD__Error);
			m_List.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

void CCMMvhlItemUsrRusDlg::ChangeBitMap()
{	
	CString aBitmapID[]   = { _T("cmd_mvhl_113.svg"),  _T("cmd_mvhl_117.svg"), _T("cmd_mvhl_114.svg"), _T("cmd_mvhl_113.svg"), _T("cmd_mvhl_113.svg"),
								_T("cmd_mvhl_113.svg"),  _T("cmd_mvhl_115.svg"), _T("cmd_mvhl_116.svg")};

	UpdateData(TRUE);
	int nBitmap = m_cmbVehicleType.GetCurSel();

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + aBitmapID[nBitmap]);
}

void CCMMvhlItemUsrRusDlg::Data2Dlg()
{
	m_edtVehicleName.SetWindowText(m_Data.VehicleLoadName);
	m_cmbVehicleType.SetCurSel(m_Data.nRussiaUserType);
	m_edtVariable.SetEditUnit(m_Data.dRussiaVariable);
	m_edtLaneW1.SetEditUnit(m_Data.dRussiaW);
	m_edtLaneL.SetEditUnit(m_Data.dRussiaD);
	m_edtLaneW2.SetEditUnit(m_Data.dRussiaW2);
	m_edtLaneP.SetEditUnit(m_Data.dRussiaP);
	m_nDynaFact = m_Data.nDynaFactor;
	m_edtUserDynaFact.SetEditUnit(m_Data.dDynaFactor);	
	m_edtUserDynaFact_UDL.SetEditUnit(m_Data.dDynaFactor_UDL);	
	m_edtRatioEmptyCar.SetEditUnit(m_Data.dRatioEmptyCar);

	m_bFatigue = m_Data.bFatigue;
	m_bSameLoadedL = m_Data.bSameLoadedL;
	m_nLoadFact = m_Data.nLoadFactor;
	m_edtUserLoadFact.SetEditUnit(m_Data.dLoadFactor);
	m_edtUserLoadFact_UDL.SetEditUnit(m_Data.dLoadFactor_UDL);

	m_bTwoVehi = m_Data.bTwoVehicle;
	m_edtTwoVehi.SetEditUnit(m_Data.dTwoVehicleFactor);
	m_b2ndGrpReduc = m_Data.b2ndGrpReducFactor;
	m_edt2ndGrpReduc.SetEditUnit(m_Data.d2ndGrpReducFactor);
	m_edtLaneFacts1_Bogie[0].SetEditUnit(m_Data.dLaneFactors1[0]);
	m_edtLaneFacts1_Bogie[1].SetEditUnit(m_Data.dLaneFactors1[1]);
	m_edtLaneFacts1_Bogie[2].SetEditUnit(m_Data.dLaneFactors1[2]);
	m_edtLaneFacts1_UDL[0].SetEditUnit(m_Data.dLaneFactors1_UDL[0]);
	m_edtLaneFacts1_UDL[1].SetEditUnit(m_Data.dLaneFactors1_UDL[1]);
	m_edtLaneFacts1_UDL[2].SetEditUnit(m_Data.dLaneFactors1_UDL[2]);
				
	SetDynaFactEditBox();
	ChangeDynaFactText();
	ChangeLoadFactText();
	MakeItemEx();
	
	UpdateData(FALSE);
}

BOOL CCMMvhlItemUsrRusDlg::Dlg2Data()
{	
	UpdateData(TRUE);  
	m_Data.bStandard = FALSE;    
	m_Data.nLoadType = 7;
	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);	

	m_Data.nRussiaUserType = m_cmbVehicleType.GetCurSel();
	m_Data.dRussiaVariable = m_edtVariable.GetEditValue();
	m_Data.dRussiaW = m_edtLaneW1.GetEditValue();
	m_Data.dRussiaD = m_edtLaneL.GetEditValue();
	m_Data.dRussiaW2 = m_edtLaneW2.GetEditValue();
	m_Data.dRussiaP = m_edtLaneP.GetEditValue();
	m_Data.dRatioEmptyCar = m_edtRatioEmptyCar.GetEditValue();

	m_Data.nDynaFactor = m_nDynaFact;
	m_Data.dDynaFactor = m_edtUserDynaFact.GetEditValue();
	m_Data.dDynaFactor_UDL = m_edtUserDynaFact_UDL.GetEditValue();
	
	m_Data.bFatigue = m_bFatigue;
	m_Data.bSameLoadedL = m_bSameLoadedL;
	m_Data.nLoadFactor = m_nLoadFact;
	m_Data.dLoadFactor = m_edtUserLoadFact.GetEditValue();
	m_Data.dLoadFactor_UDL = m_edtUserLoadFact_UDL.GetEditValue();

	m_Data.bTwoVehicle = m_bTwoVehi;
	m_Data.dTwoVehicleFactor = m_edtTwoVehi.GetEditValue();
	m_Data.b2ndGrpReducFactor = m_b2ndGrpReduc;		
	m_Data.d2ndGrpReducFactor = m_edt2ndGrpReduc.GetEditValue();
	m_Data.dLaneFactors1[0] = m_edtLaneFacts1_Bogie[0].GetEditValue();
	m_Data.dLaneFactors1[1] = m_edtLaneFacts1_Bogie[1].GetEditValue();
	m_Data.dLaneFactors1[2] = m_edtLaneFacts1_Bogie[2].GetEditValue();
	m_Data.dLaneFactors1_UDL[0] = m_edtLaneFacts1_UDL[0].GetEditValue();
	m_Data.dLaneFactors1_UDL[1] = m_edtLaneFacts1_UDL[1].GetEditValue();
	m_Data.dLaneFactors1_UDL[2] = m_edtLaneFacts1_UDL[2].GetEditValue();

	return TRUE;
}

int CCMMvhlItemUsrRusDlg::GetLoadCount()
{
	int i = 0;
	for (i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CCMMvhlItemUsrRusDlg::GetDistCount()
{
	int i = 0;
	for (i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointDistance[i] == 0.0) break;
	}
	return i;
}

BOOL CCMMvhlItemUsrRusDlg::GetLoadAndDistValue(double &dLoad, double &dDist)
{
	int nRussiaUserType = m_cmbVehicleType.GetCurSel();

	if (!CFormulaEdit::GetEditValue(&m_edtTrkP, dLoad))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_load_value_));
		return FALSE;
	}
	if (!CFormulaEdit::GetEditValue(&m_edtTrkD, dDist))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_distance_value_));
		return FALSE;
	}
	
	if (dLoad == 0.0) // Load 0은 허용 안함
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Load_value_must_not_be_ze));
		return FALSE;
	}
	
	if(nRussiaUserType == 1)
	{
		if (dDist == 0.0) // Load 0은 허용 안함
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Dist_value_must_not_be_ze));
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CCMMvhlItemUsrRusDlg::ApplyOrOK()
{
	BOOL bSuccess = FALSE;
	
	if (!Dlg2Data()) return FALSE;

	if (m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else					 bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);

	if (!bSuccess) return FALSE;
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrRusDlg message handlers

BOOL CCMMvhlItemUsrRusDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitUnit();
	InitCombo();

	SetHeaderTitle();
	
	// User Define 일 때만 초기값 변경
	if(!m_bModify)
	{
		m_Data.nDynaFactor = 1;
		m_Data.dRussiaVariable = M_InitValueCurUnit(8.5, N, M, D_UNITSYS_BASE_LENGTH);
		m_Data.nLoadFactor = 1;
		m_Data.dLoadFactor = 1.5;
		m_Data.dLoadFactor_UDL = 1.25; //1.15->1.25로 변경
	}
	
	Data2Dlg();
		
	UpdateData(FALSE);

	ChangeBitMap();

	AlignControl();
	ControlsShowHide();
	EnableDisableControls();
	
	if(m_bModify)	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemUsrRusDlg::OnOK() 
{
	if (!ApplyOrOK()) return;
	
	CDialogMove::OnOK();
}

void CCMMvhlItemUsrRusDlg::OnCmdMvhlVehicleTypeCmb() 
{
	UpdateData(TRUE);

	CString VehicleLoadName = _T("");
	m_edtVehicleName.GetWindowText(VehicleLoadName);	

	int nRussiaUserType = m_cmbVehicleType.GetCurSel();
	if(nRussiaUserType == 0 || nRussiaUserType == 3 || nRussiaUserType == 4 || nRussiaUserType == 6) m_nDynaFact = 1;

	// Vehicle Type 이 변경되면 Data 초기화
	if(m_nRussiaUserType != nRussiaUserType) 
	{
		m_Data.Initialize();
		m_Data.VehicleLoadName = VehicleLoadName;
		m_Data.nRussiaUserType = nRussiaUserType;

		// Dynamic Factor
		if(nRussiaUserType == 1 || nRussiaUserType == 2 || nRussiaUserType == 5)
		{
			m_Data.nDynaFactor = 0;
		}
		else
		{
			m_Data.nDynaFactor = 1;
		}
		m_Data.dRussiaVariable = M_InitValueCurUnit(8.5, N, M, D_UNITSYS_BASE_LENGTH);

		// Load Reliability Factor
		if(nRussiaUserType == 0)
		{
			m_Data.nLoadFactor = 1;
			m_Data.dLoadFactor = 1.5;
			m_Data.dLoadFactor_UDL = 1.25; //1.15->1.25로 변경
		}
		else if(nRussiaUserType == 3 || nRussiaUserType == 4)
		{
			m_Data.nLoadFactor = 1;
			m_Data.dLoadFactor = 1.1;
			m_Data.dLoadFactor_UDL = 1.1;
		}
		else if(nRussiaUserType == 1 || nRussiaUserType == 2)
		{
			m_Data.nLoadFactor = 0;
			m_Data.dLoadFactor = 1.0;
			m_Data.dLoadFactor_UDL = 1.0;
		}
		else if(nRussiaUserType == 5 || nRussiaUserType == 6 || nRussiaUserType == 7)
		{
			m_Data.nLoadFactor = 1;
			m_Data.dLoadFactor = 1.0;
			m_Data.dLoadFactor_UDL = 1.0;
		}
		else
		{
			ASSERT(0);
		}

		Data2Dlg();
	}
	m_nRussiaUserType = m_cmbVehicleType.GetCurSel();
	
	UpdateData(FALSE);

	ChangeBitMap();

	AlignControl();
	ControlsShowHide();
	EnableDisableControls();

	SetDynaFactEditBox();
	ChangeDynaFactText();
	ChangeLoadFactText();
}

void CCMMvhlItemUsrRusDlg::OnCmdMvhlAddBtn() 
{
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();

	int nRussiaUserType = m_cmbVehicleType.GetCurSel();

	if(nRussiaUserType == 1 || nRussiaUserType == 2)
	{
		if(nLoadCount == 4)
		{
			AfxMessageBox(_LS(IDS_CMD_MVHL_RUSSIA_USER_DEFINED_ERROR));
			return;
		}
	}
	
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
		m_Data.dPointLoad[nLoadCount] = dLoad;
	}
	else
	{
		m_Data.dPointLoad[nLoadCount] = dLoad;
		m_Data.dPointDistance[nLoadCount] = dDist;
	}
	
	int no = nLoadCount+1;
	InsertItem(nLoadCount, no, dLoad, dDist);
	
	int nCount = m_List.GetItemCount();
	m_List.SetItemState(nCount-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvhlItemUsrRusDlg::OnCmdMvhlInsBtn() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_inser));
		return;
	}
	
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();

	int nRussiaUserType = m_cmbVehicleType.GetCurSel();
	
	if(nRussiaUserType == 1 || nRussiaUserType == 2)
	{
		if(nLoadCount == 4)
		{
			AfxMessageBox(_LS(IDS_CMD_MVHL_RUSSIA_USER_DEFINED_ERROR));
			return;
		}
	}
	
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
	for (int i = D_MVHL_NUMLOAD-1; i > iItem; i--)
	{
		m_Data.dPointLoad[i] = m_Data.dPointLoad[i-1];
		m_Data.dPointDistance[i] = m_Data.dPointDistance[i-1];
	}
	
	// 데이타 삽입한다.
	m_Data.dPointLoad[iItem] = dLoad;
	m_Data.dPointDistance[iItem] = dDist;
	
	// List에 Item 삽입
	InsertItem(iItem, iItem+1, dLoad, dDist);
	
	// List 번호 변경
	int nCount = m_List.GetItemCount();
	CString csVal;
	for (int i = iItem+1; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_List.SetItemText(i, 0, csVal);
	}
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvhlItemUsrRusDlg::OnCmdMvhlModBtn() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_modif));
		return;
	}
	
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	
	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;
	
	if (iItem != nLoadCount-1 && dDist == 0.0)  // middle and dist is zero!
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Intermediate_distance_mus));
		return;
	}
	
	m_Data.dPointLoad[iItem] = dLoad;
	m_Data.dPointDistance[iItem] = dDist;
	
	ModifyItem(iItem, iItem+1, dLoad, dDist);
}

void CCMMvhlItemUsrRusDlg::OnCmdMvhlDelBtn() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_delet));
		return;
	}
	
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	int i = iItem;
	for (i = iItem; i < D_MVHL_NUMLOAD-1; i++)
	{
		m_Data.dPointLoad[i] = m_Data.dPointLoad[i+1];
		m_Data.dPointDistance[i] = m_Data.dPointDistance[i+1];
	}
	m_Data.dPointLoad[i] = 0.0;
	m_Data.dPointDistance[i] = 0.0;
	
	m_List.DeleteItem(iItem);
	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	
	// List 번호 변경
	CString csVal;
	for (i = iItem; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_List.SetItemText(i, 0, csVal);
	}
}

void CCMMvhlItemUsrRusDlg::OnCmdMvhlApplyBtn() 
{
	ApplyOrOK();
}

void CCMMvhlItemUsrRusDlg::OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	CString csVal;
	if (iItem == nLoadCount-1 && nLoadCount != nDistCount)  // fixed, last
	{
		csVal.Format(_T("%g"), m_Data.dPointLoad[iItem]);
		m_edtTrkP.SetWindowText(csVal);
		m_edtTrkD.SetWindowText(_T(""));
	}
	else
	{
		csVal.Format(_T("%g"), m_Data.dPointLoad[iItem]);
		m_edtTrkP.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data.dPointDistance[iItem]);
		m_edtTrkD.SetWindowText(csVal);
	}
	
	*pResult = 0;
}

void CCMMvhlItemUsrRusDlg::OnCmdMvhlFatigueChk()
{
	UpdateData(TRUE);
	
	SetDynaFactEditBox();
	ChangeDynaFactText();
	ChangeLoadFactText();
}

void CCMMvhlItemUsrRusDlg::OnCmdMvhlDynaFactRdo() 
{
	EnableDisableControls();
}

void CCMMvhlItemUsrRusDlg::OnCmdMvhlLoadFactRdo()
{
	EnableDisableControls();
}

void CCMMvhlItemUsrRusDlg::OnCmdMvhlTwoVehiChk()
{
	EnableDisableControls();
}

void CCMMvhlItemUsrRusDlg::OnCmdMvhl2ndReducChk()
{
	EnableDisableControls();
}








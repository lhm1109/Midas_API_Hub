// CMMvhlItemStdIndiaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdIndiaDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MvhlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IRC_LIST   0
#define IRS1_LIST  1
#define IRS2_LIST  2
#define IRS_12T_LIST  3
#define IRS_40T_LIST  4

// IRC
#define TYPENAME_Class_A                               _T("Class A")
#define TYPENAME_Class_B                               _T("Class B")
#define TYPENAME_Class_70R                             _T("Class 70R")
#define TYPENAME_Class_40R                             _T("Class 40R")
#define TYPENAME_Class_AA                              _T("Class AA")
#define TYPENAME_Footway                               _T("Footway")
#define TYPENAME_Fatigue                               _T("Fatigue Vehicle")

// IRS
#define TYPENAME_Broad_Gauge_1676mm                    _T("Broad Gauge-1676mm")
#define TYPENAME_Metre_Gauge_1000mm                    _T("Metre Gauge-1000mm")
#define TYPENAME_Narrow_Gauge_762mm                    _T("Narrow Gauge-762mm")
#define TYPENAME_Heavy_Mineral_Loadings                _T("Heavy Mineral Loadings")
#define TYPENAME_Footbridge_Footpath                   _T("Footbridge & Footpath")

// IRS - Broad Gauge-1676mm
#define SELNAME_Modified_B_G_Loading_1987_1            _T("Modified B.G. Loading 1987-1")
#define SELNAME_Modified_B_G_Loading_1987_2            _T("Modified B.G. Loading 1987-2")
#define SELNAME_B_G_Standard_Loading_1926_M_L          _T("B.G. Standard Loading 1926-M.L.")
#define SELNAME_B_G_Standard_Loading_1926_B_L          _T("B.G. Standard Loading 1926-B.L.")
#define SELNAME_Revised_B_G_Loading_1975_WG1_WG1       _T("Revised B.G. Loading 1975-WG1+WG1")
#define SELNAME_Revised_B_G_Loading_1975_WAM4A_WAM4A   _T("Revised B.G. Loading 1975-WAM4A+WAM4A")
#define SELNAME_Revised_B_G_Loading_1975_Bo_Bo_Bo_Bo   _T("Revised B.G. Loading 1975-Bo-Bo+Bo-Bo")
#define SELNAME_Revised_B_G_Loading_1975_WAM4A         _T("Revised B.G. Loading 1975-WAM4A")
#define SELNAME_Revised_B_G_Loading_1975_WAM4A_WAM4    _T("Revised B.G. Loading 1975-WAM4A+WAM4")
#define SELNAME_Revised_B_G_Loading_1975_WAM4A_WDM2    _T("Revised B.G. Loading 1975-WAM4A+WDM2")
#define SELNAME_25t_Loading_2008_Combination_1         _T("25t Loading-2008 Combination 1")
#define SELNAME_25t_Loading_2008_Combination_2         _T("25t Loading-2008 Combination 2")
#define SELNAME_25t_Loading_2008_Combination_3         _T("25t Loading-2008 Combination 3")
#define SELNAME_25t_Loading_2008_Combination_4         _T("25t Loading-2008 Combination 4")
#define SELNAME_25t_Loading_2008_Combination_5         _T("25t Loading-2008 Combination 5")
#define SELNAME_DFC_Loading_Combination_1              _T("DFC Loading Combination 1")
#define SELNAME_DFC_Loading_Combination_2              _T("DFC Loading Combination 2")
#define SELNAME_DFC_Loading_Combination_3              _T("DFC Loading Combination 3")
#define SELNAME_DFC_Loading_Combination_4              _T("DFC Loading Combination 4")
#define SELNAME_DFC_Loading_Combination_5              _T("DFC Loading Combination 5")

// IRS - Metre Gauge-1000mm
#define SELNAME_2_Co_Co_Locomotives                    _T("2 Co-Co Locomotives")
#define SELNAME_2_Bo_Bo_Locomotives                    _T("2 Bo-Bo Locomotives")
#define SELNAME_MGML_Loading_of_1929                   _T("MGML Loading of 1929")
#define SELNAME_M_L                                    _T("M.L.")
#define SELNAME_B_L                                    _T("B.L.")
#define SELNAME_C                                      _T("C.")

// IRS - Narrow Gauge-762mm
#define SELNAME_Class_H_B_B_or_Bo_Bo_Type              _T("Class H: B-B or Bo-Bo Type")
#define SELNAME_Class_H_C_C_or_Co_Co_Type              _T("Class H: C-C or Co-Co Type")
#define SELNAME_Class_H_Steam_Zf_1                     _T("Class H: Steam (Zf/1)")
#define SELNAME_Class_H_Diesel_Electric                _T("Class H: Diesel Electric")
#define SELNAME_Class_A_B_B_or_Bo_Bo_Type              _T("Class A: B-B or Bo-Bo Type")
#define SELNAME_Class_A_C_C_or_Co_Co_Type              _T("Class A: C-C or Co-Co Type")
#define SELNAME_Class_A_Diesel_Mech_Elec               _T("Class A: Diesel Mech./Elec.")
#define SELNAME_Class_A_Diesel_Mech_Elec_Articulated   _T("Class A: Diesel Mech./Elec.(Articulated)")
#define SELNAME_Class_A_DRG_No_CSO_C_873               _T("Class A: DRG No. CSO/C-873")
#define SELNAME_Class_B_B_B_or_Bo_Bo_Type              _T("Class B: B-B or Bo-Bo Type")
#define SELNAME_Class_B_Steam_Engine_Tank              _T("Class B: Steam Engine (Tank)")
#define SELNAME_Class_B_Steam_Engine_Tender            _T("Class B: Steam Engine (Tender)")
#define SELNAME_Class_B_Diesel_Electric                _T("Class B: Diesel Electric")

// IRS - Heavy Mineral Loadings
#define SELNAME_Train_Formation_No_1                   _T("Train Formation No.1")
#define SELNAME_Train_Formation_No_2                   _T("Train Formation No.2")
#define SELNAME_Train_Formation_No_3                   _T("Train Formation No.3")
#define SELNAME_Train_Formation_No_4                   _T("Train Formation No.4")
#define SELNAME_Train_Formation_No_5                   _T("Train Formation No.5")
#define SELNAME_Train_Formation_No_6                   _T("Train Formation No.6")
#define SELNAME_Train_Formation_No_7                   _T("Train Formation No.7")
#define SELNAME_Train_Formation_No_8                   _T("Train Formation No.8")
#define SELNAME_Train_Formation_No_9                   _T("Train Formation No.9")
#define SELNAME_Train_Formation_No_10                  _T("Train Formation No.10")
#define SELNAME_Train_Formation_No_11                  _T("Train Formation No.11")
#define SELNAME_Train_Formation_No_12                  _T("Train Formation No.12")
#define SELNAME_Train_Formation_No_13                  _T("Train Formation No.13")
#define SELNAME_Train_Formation_No_14                  _T("Train Formation No.14")
#define SELNAME_Train_Formation_No_15                  _T("Train Formation No.15")
#define SELNAME_Train_Formation_No_16                  _T("Train Formation No.16")
#define SELNAME_Train_Formation_No_17                  _T("Train Formation No.17")

// IRS - Footbridge & Footpath
#define SELNAME_Footbridge_Footpath                    _T("Footbridge & Footpath")

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdIndiaDlg dialog
#define COLCOUNT 3

CCMMvhlItemStdIndiaDlg::CCMMvhlItemStdIndiaDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemStdIndiaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemStdIndiaDlg)	
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();
	m_Data.Initialize();
	m_bModify = FALSE;

	m_nFactorType = 0;
}

CCMMvhlItemStdIndiaDlg::~CCMMvhlItemStdIndiaDlg()
{
}

void CCMMvhlItemStdIndiaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdIndiaDlg)
	DDX_Control(pDX, IDC_CMD_MVHL_CODE_CMB,                    m_cmbCodeName);
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_NAME_EDT,               m_edtVehicleName);
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_TYPE_CMB,               m_cmbVehicleType);
	DDX_Control(pDX, IDC_CMD_MVHL_NOSE_TAIL_DIST_EDT,          m_edtNoseTailDist);
	DDX_Control(pDX, IDC_CMD_MVHL_NOSE_TAIL_DIST_UNT,          m_untNoseTailDist);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_PTLIST,                  m_lstIRC);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_DD1_EDT,                 m_edtIRCdD1);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_DD1_UNT,                 m_untIRCdD1);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_DD2_EDT,                 m_edtIRCdD2);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_DD2_UNT,                 m_untIRCdD2);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_P_EDT,                   m_edtIRCP);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_P_UNT,                   m_untIRCP);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_D_EDT,                   m_edtIRCD);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_D_UNT,                   m_untIRCD);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_PB_EDT,                  m_edtIRCPb);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_PB_UNT,                  m_untIRCPb);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_DB_EDT,                  m_edtIRCDb);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_DB_UNT,                  m_untIRCDb);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_W_EDT,                   m_edtIRCW);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_W_UNT,                   m_untIRCW);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_FW_EDT,                  m_edtIRCFW);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_FW_UNT,                  m_untIRCFW);
	DDX_Control(pDX, IDC_CMD_MVHL_SEL_VEHICLE_CMB,             m_cmbSelVehicle);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_PTLIST1,                 m_lstIRS1);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_PTLIST2,                 m_lstIRS2);
	DDX_Control(pDX, IDC_WG_CMD_IRC_12T_LIST,                  m_lstI12T);
	DDX_Control(pDX, IDC_WG_CMD_IRC_40T_LIST,                  m_lstI40T);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_DD1_EDT2,                m_edtIRSdD12);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_DD1_UNT2,                m_untIRSdD12);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_DD2_EDT2,                m_edtIRSdD22);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_DD2_UNT2,                m_untIRSdD22);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_DD3_EDT,                 m_edtIRSdD3);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_DD3_UNT,                 m_untIRSdD3);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_DW_EDT,                  m_edtIRSdW);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_DW_UNT,                  m_untIRSdW);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_DD1_EDT1,                m_edtIRSdD11);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_DD1_UNT1,                m_untIRSdD11);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_DD2_EDT1,                m_edtIRSdD21);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_DD2_UNT1,                m_untIRSdD21);
	DDX_Control(pDX, IDC_CMD_MVHL_TRACTIVE_EDT,                m_edtTractive);
	DDX_Control(pDX, IDC_CMD_MVHL_TRACTIVE_UNT,                m_untTractive);
	DDX_Control(pDX, IDC_CMD_MVHL_BRAKE_LOCO_RATIO_EDT,        m_edtBrakeLOCORatio);
	DDX_Control(pDX, IDC_CMD_MVHL_BRAKE_TRAIN_RATIO_EDT,       m_edtBrakeTrainRatio);
	DDX_Control(pDX, IDC_CMD_MVHL_BRAKE_LOCO_EDT,              m_edtBrakeLOCO);
	DDX_Control(pDX, IDC_CMD_MVHL_BRAKE_LOCO_UNT,              m_untBrakeLOCO);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_L_EDT,                   m_edtIRSL);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_L_UNT,                   m_untIRSL);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_FW_EDT,                  m_edtIRSFW);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_FW_UNT,                  m_untIRSFW);
	DDX_Control(pDX, IDC_CMD_MVHL_NOSE_TAIL_DIST_WHEELED_EDT,  m_edtNoseTailDistW);
	DDX_Control(pDX, IDC_CMD_MVHL_NOSE_TAIL_DIST_WHEELED_UNT,  m_untNoseTailDistW);
	DDX_Control(pDX, IDC_CMD_MVHL_NOSE_TAIL_DIST_TRACKED_EDT,  m_edtNoseTailDistT);
	DDX_Control(pDX, IDC_CMD_MVHL_NOSE_TAIL_DIST_TRACKED_UNT,  m_untNoseTailDistT);

	DDX_Control(pDX, IDC_CMD_MVHL_PICTURE,					   m_wndPicture);
	DDX_Control(pDX, IDC_WG_CMD_IRC_D_EDT,                     m_edtIRCMinD);
	DDX_Control(pDX, IDC_WG_CMD_IRC_D_UNIT,                    m_untIRCMinD);
	DDX_Control(pDX, IDC_WG_CMD_IRC_ROUGHNESS_EDT,             m_edtRoughFactor);
	DDX_Radio(pDX, IDC_WG_CMD_IRC_ROUGHNESS_GOOD_RDO,          m_nFactorType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMMvhlItemStdIndiaDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemStdIndiaDlg)
 	ON_CBN_SELCHANGE(IDC_CMD_MVHL_CODE_CMB,           OnCmdMvhlCodeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_VEHI_TYPE_CMB,      OnCmdMvhlTypeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_SEL_VEHICLE_CMB,    OnCmdMvhlSelVehlCmb)
	ON_BN_CLICKED(IDC_CMD_APPLY,				      OnCmdMvhlApply)

	ON_BN_CLICKED(IDC_WG_CMD_IRC_ROUGHNESS_GOOD_RDO, OnCmdFactorTypeRdo)
	ON_BN_CLICKED(IDC_WG_CMD_IRC_ROUGHNESS_MEDI_RDO, OnCmdFactorTypeRdo)
	ON_BN_CLICKED(IDC_WG_CMD_IRC_ROUGHNESS_USER_RDO, OnCmdFactorTypeRdo)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvhlItemStdIndiaDlg::InitControl()
{
	m_aNoseTail.RemoveAll();
	m_aNoseTail.Add(IDC_CMD_MVHL_NOSE_TAIL_DIST_STC);
	m_aNoseTail.Add(IDC_CMD_MVHL_NOSE_TAIL_DIST_EDT);
	m_aNoseTail.Add(IDC_CMD_MVHL_NOSE_TAIL_DIST_UNT);

	m_aNoseTail70R.RemoveAll();
	m_aNoseTail70R.Add(IDC_CMD_MVHL_NOSE_TAIL_DIST_WHEELED_STC);
	m_aNoseTail70R.Add(IDC_CMD_MVHL_NOSE_TAIL_DIST_WHEELED_EDT);
	m_aNoseTail70R.Add(IDC_CMD_MVHL_NOSE_TAIL_DIST_WHEELED_UNT);
	m_aNoseTail70R.Add(IDC_CMD_MVHL_NOSE_TAIL_DIST_TRACKED_STC);
	m_aNoseTail70R.Add(IDC_CMD_MVHL_NOSE_TAIL_DIST_TRACKED_EDT);
	m_aNoseTail70R.Add(IDC_CMD_MVHL_NOSE_TAIL_DIST_TRACKED_UNT);

	m_aSelVehicle.RemoveAll();
	m_aSelVehicle.Add(IDC_CMD_MVHL_SEL_VEHICLE_STC);
	m_aSelVehicle.Add(IDC_CMD_MVHL_SEL_VEHICLE_CMB);

	m_aPicture.RemoveAll();
	m_aPicture.Add(IDC_CMD_MVHL_PICTURE);

	m_aIRC.RemoveAll();
	m_aIRC.Add(IDC_CMD_MVHL_IRC_PTLIST);
	m_aIRC.Add(IDC_CMD_MVHL_IRC_DD1_STC);
	m_aIRC.Add(IDC_CMD_MVHL_IRC_DD1_EDT);
	m_aIRC.Add(IDC_CMD_MVHL_IRC_DD1_UNT);
	m_aIRC.Add(IDC_CMD_MVHL_IRC_DD2_STC);
	m_aIRC.Add(IDC_CMD_MVHL_IRC_DD2_EDT);
	m_aIRC.Add(IDC_CMD_MVHL_IRC_DD2_UNT);
	m_aIRC.Add(IDC_CMD_MVHL_IRC_P_STC);
	m_aIRC.Add(IDC_CMD_MVHL_IRC_P_EDT);
	m_aIRC.Add(IDC_CMD_MVHL_IRC_P_UNT);
	m_aIRC.Add(IDC_CMD_MVHL_IRC_D_STC);
	m_aIRC.Add(IDC_CMD_MVHL_IRC_D_EDT);
	m_aIRC.Add(IDC_CMD_MVHL_IRC_D_UNT);
	m_aIRC.Add(IDC_CMD_MVHL_IRC_PB_STC);
	m_aIRC.Add(IDC_CMD_MVHL_IRC_PB_EDT);
	m_aIRC.Add(IDC_CMD_MVHL_IRC_PB_UNT);
	m_aIRC.Add(IDC_CMD_MVHL_IRC_DB_STC);
	m_aIRC.Add(IDC_CMD_MVHL_IRC_DB_EDT);
	m_aIRC.Add(IDC_CMD_MVHL_IRC_DB_UNT);

	m_aIRS_Point1.RemoveAll();
	m_aIRS_Point1.Add(IDC_CMD_MVHL_IRS_PTLIST1_GRP);
	m_aIRS_Point1.Add(IDC_CMD_MVHL_IRS_PTLIST1);

	m_aIRS_Point2.RemoveAll();
	m_aIRS_Point2.Add(IDC_CMD_MVHL_IRS_PTLIST2_GRP);
	m_aIRS_Point2.Add(IDC_CMD_MVHL_IRS_PTLIST2);
	m_aIRS_Point2.Add(IDC_CMD_MVHL_IRS_DD1_STC2);
	m_aIRS_Point2.Add(IDC_CMD_MVHL_IRS_DD1_EDT2);
	m_aIRS_Point2.Add(IDC_CMD_MVHL_IRS_DD1_UNT2);
	m_aIRS_Point2.Add(IDC_CMD_MVHL_IRS_DD2_STC2);
	m_aIRS_Point2.Add(IDC_CMD_MVHL_IRS_DD2_EDT2);
	m_aIRS_Point2.Add(IDC_CMD_MVHL_IRS_DD2_UNT2);
	m_aIRS_Point2.Add(IDC_CMD_MVHL_IRS_DD3_STC);
	m_aIRS_Point2.Add(IDC_CMD_MVHL_IRS_DD3_EDT);
	m_aIRS_Point2.Add(IDC_CMD_MVHL_IRS_DD3_UNT);

	m_aIRS_Dist.RemoveAll();
	m_aIRS_Dist.Add(IDC_CMD_MVHL_IRS_DW_STC);
	m_aIRS_Dist.Add(IDC_CMD_MVHL_IRS_DW_EDT);
	m_aIRS_Dist.Add(IDC_CMD_MVHL_IRS_DW_UNT);
	m_aIRS_Dist.Add(IDC_CMD_MVHL_IRS_DD1_STC1);
	m_aIRS_Dist.Add(IDC_CMD_MVHL_IRS_DD1_EDT1);
	m_aIRS_Dist.Add(IDC_CMD_MVHL_IRS_DD1_UNT1);
	m_aIRS_Dist.Add(IDC_CMD_MVHL_IRS_DD2_STC1);
	m_aIRS_Dist.Add(IDC_CMD_MVHL_IRS_DD2_EDT1);
	m_aIRS_Dist.Add(IDC_CMD_MVHL_IRS_DD2_UNT1);

	m_aFooyWay.RemoveAll();
	m_aFooyWay.Add(IDC_CMD_MVHL_IRC_W_STC);
	m_aFooyWay.Add(IDC_CMD_MVHL_IRC_W_EDT);
	m_aFooyWay.Add(IDC_CMD_MVHL_IRC_W_UNT);
	m_aFooyWay.Add(IDC_CMD_MVHL_IRC_FW_STC);
	m_aFooyWay.Add(IDC_CMD_MVHL_IRC_FW_EDT);
	m_aFooyWay.Add(IDC_CMD_MVHL_IRC_FW_UNT);	

	m_aFooyPath.RemoveAll();
	m_aFooyPath.Add(IDC_CMD_MVHL_IRS_L_STC);
	m_aFooyPath.Add(IDC_CMD_MVHL_IRS_L_EDT);
	m_aFooyPath.Add(IDC_CMD_MVHL_IRS_L_UNT);
	m_aFooyPath.Add(IDC_CMD_MVHL_IRS_FW_STC);
	m_aFooyPath.Add(IDC_CMD_MVHL_IRS_FW_EDT);
	m_aFooyPath.Add(IDC_CMD_MVHL_IRS_FW_UNT);
	m_aFooyPath.Add(IDC_CMD_MVHL_IRS_W_STC1);
	m_aFooyPath.Add(IDC_CMD_MVHL_IRS_W_STC2);
	m_aFooyPath.Add(IDC_CMD_MVHL_IRS_W_STC3);

	m_aLongiForce.RemoveAll();
	m_aLongiForce.Add(IDC_CMD_MVHL_LONGI_GRP);
	m_aLongiForce.Add(IDC_CMD_MVHL_TRACTIVE_STC);
	m_aLongiForce.Add(IDC_CMD_MVHL_TRACTIVE_EDT);
	m_aLongiForce.Add(IDC_CMD_MVHL_TRACTIVE_UNT);
	m_aLongiForce.Add(IDC_CMD_MVHL_BRAKE_LOCO_RATIO_STC1);
	m_aLongiForce.Add(IDC_CMD_MVHL_BRAKE_LOCO_RATIO_EDT);
	m_aLongiForce.Add(IDC_CMD_MVHL_BRAKE_LOCO_RATIO_STC2);
	m_aLongiForce.Add(IDC_CMD_MVHL_BRAKE_TRAIN_RATIO_STC1);
	m_aLongiForce.Add(IDC_CMD_MVHL_BRAKE_TRAIN_RATIO_EDT);
	m_aLongiForce.Add(IDC_CMD_MVHL_BRAKE_TRAIN_RATIO_STC2);
	m_aLongiForce.Add(IDC_CMD_MVHL_BRAKE_LOCO_STC);
	m_aLongiForce.Add(IDC_CMD_MVHL_BRAKE_LOCO_EDT);
	m_aLongiForce.Add(IDC_CMD_MVHL_BRAKE_LOCO_UNT);

	m_aBrakeLOCORatio.RemoveAll();
	m_aBrakeLOCORatio.Add(IDC_CMD_MVHL_BRAKE_LOCO_RATIO_STC1);
	m_aBrakeLOCORatio.Add(IDC_CMD_MVHL_BRAKE_LOCO_RATIO_EDT);
	m_aBrakeLOCORatio.Add(IDC_CMD_MVHL_BRAKE_LOCO_RATIO_STC2);

	m_aBrakeTrainRatio.RemoveAll();
	m_aBrakeTrainRatio.Add(IDC_CMD_MVHL_BRAKE_TRAIN_RATIO_STC1);
	m_aBrakeTrainRatio.Add(IDC_CMD_MVHL_BRAKE_TRAIN_RATIO_EDT);
	m_aBrakeTrainRatio.Add(IDC_CMD_MVHL_BRAKE_TRAIN_RATIO_STC2);

	m_aBrakeLOCO.RemoveAll();
	m_aBrakeLOCO.Add(IDC_CMD_MVHL_BRAKE_LOCO_STC);
	m_aBrakeLOCO.Add(IDC_CMD_MVHL_BRAKE_LOCO_EDT);
	m_aBrakeLOCO.Add(IDC_CMD_MVHL_BRAKE_LOCO_UNT);

	m_aIRCFatigue.RemoveAll();
	m_aIRCFatigue.Add(IDC_WG_CMD_IRC_VEHIC_LOAD_GRP);
	m_aIRCFatigue.Add(IDC_WG_CMD_IRC_12T_STC);
	m_aIRCFatigue.Add(IDC_WG_CMD_IRC_12T_LIST);
	m_aIRCFatigue.Add(IDC_WG_CMD_IRC_40T_STC);
	m_aIRCFatigue.Add(IDC_WG_CMD_IRC_40T_LIST);
	m_aIRCFatigue.Add(IDC_WG_CMD_IRC_MIN_DIST_STC);
	m_aIRCFatigue.Add(IDC_WG_CMD_IRC_D_STC);
	m_aIRCFatigue.Add(IDC_WG_CMD_IRC_D_EDT);
	m_aIRCFatigue.Add(IDC_WG_CMD_IRC_D_UNIT);
	m_aIRCFatigue.Add(IDC_WG_CMD_IRC_ROUGHNESS_GRP);
	m_aIRCFatigue.Add(IDC_WG_CMD_IRC_ROUGHNESS_GOOD_RDO);
	m_aIRCFatigue.Add(IDC_WG_CMD_IRC_ROUGHNESS_MEDI_RDO);
	m_aIRCFatigue.Add(IDC_WG_CMD_IRC_ROUGHNESS_USER_RDO);
	m_aIRCFatigue.Add(IDC_WG_CMD_IRC_ROUGHNESS_EDT);

	m_aOKCancel.RemoveAll();
	m_aOKCancel.Add(IDOK);
	m_aOKCancel.Add(IDCANCEL);
	m_aOKCancel.Add(IDC_CMD_APPLY);
}

void CCMMvhlItemStdIndiaDlg::InitUnit()
{
	m_edtVehicleName.SetUnitType(D_UNITSYS_NONE);
	m_edtNoseTailDist.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dNoseTailDist);
	m_untNoseTailDist.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dNoseTailDist);
	m_edtIRCdD1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD1);
	m_untIRCdD1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD1);
	m_edtIRCdD2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD2);
	m_untIRCdD2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD2);
	m_edtIRCP.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dP);
	m_untIRCP.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dP);
	m_edtIRCD.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDTrailer);
	m_untIRCD.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDTrailer);
	m_edtIRCPb.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPb);
	m_untIRCPb.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPb);
	m_edtIRCDb.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDb);
	m_untIRCDb.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDb);
	m_edtIRCW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dFootway);
	m_untIRCW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dFootway);
	m_edtIRCFW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dFootwayWidth);
	m_untIRCFW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dFootwayWidth);
	m_edtIRSdD12.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTrainD1);
	m_untIRSdD12.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTrainD1);
	m_edtIRSdD22.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTrainD2);
	m_untIRSdD22.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTrainD2);
	m_edtIRSdD3.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTrainD3);
	m_untIRSdD3.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTrainD3);
	m_edtIRSdW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dUniformLoad);
	m_untIRSdW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dUniformLoad);
	m_edtIRSdD11.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTrainD1);
	m_untIRSdD11.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTrainD1);
	m_edtIRSdD21.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTrainD2);
	m_untIRSdD21.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTrainD2);
	m_edtTractive.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTractive);
	m_untTractive.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTractive);
	m_edtBrakeLOCORatio.SetUnitType(D_UNITSYS_NONE);
	m_edtBrakeTrainRatio.SetUnitType(D_UNITSYS_NONE);
	m_edtBrakeLOCO.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dBrakeLoco);
	m_untBrakeLOCO.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dBrakeLoco);
	m_edtIRSL.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dSpanLength);
	m_untIRSL.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dSpanLength);
	m_edtIRSFW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dFootwayWidth);
	m_untIRSFW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dFootwayWidth);
	m_edtNoseTailDistW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dNoseTailDist);
	m_untNoseTailDistW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dNoseTailDist);
	m_edtNoseTailDistT.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dNoseTailDist);
	m_untNoseTailDistT.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dNoseTailDist);

	m_edtIRCMinD.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untIRCMinD.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtRoughFactor.SetUnitType(D_UNITSYS_NONE);
}

void CCMMvhlItemStdIndiaDlg::InitCombo()
{
	// Standard Name
	m_cmbCodeName.ResetContent();

	int nCodeSeq[2];
	nCodeSeq[0] = D_MVHL_IRC6_2000;
	nCodeSeq[1] = D_MVHL_IRS_BRIDGE_RULES;

	int nNumIter = 2;
	for(int i= 0; i<nNumIter; i++) 
	{
		CDlgUtil::CobxAddItem(m_cmbCodeName, CVehlDB::GetTypeNameAr()[nCodeSeq[i]], nCodeSeq[i]);
	}

	m_cmbCodeName.SetCurSel(0); 
}

void CCMMvhlItemStdIndiaDlg::ChangeCodeCombo()
{
	// Vehicular Load Type
	m_cmbVehicleType.ResetContent();

	int nCodeType = CDlgUtil::CobxGetCurSelItemData(m_cmbCodeName, m_cmbCodeName.GetCurSel());

	int i = 0;
	while (i < D_MAX_COUNT && CVehlDB::GetTypeListAr()[nCodeType][i] != _T(""))
	{
		const auto& strTranslation =
			ConvVehicleTypeStrRaw2Trans(CVehlDB::GetTypeListAr()[nCodeType][i++]);
		m_cmbVehicleType.AddString(strTranslation);
	}

	m_cmbVehicleType.SetCurSel(0);
}

void CCMMvhlItemStdIndiaDlg::ChangeVehlTypeCombo()
{
	UpdateData(TRUE);

	m_cmbSelVehicle.ResetContent();
	CString str;

	if(m_Data.VehicleTypeName== TYPENAME_Broad_Gauge_1676mm)
	{
		str = SELNAME_Modified_B_G_Loading_1987_1;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Modified_B_G_Loading_1987_2;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_B_G_Standard_Loading_1926_M_L;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_B_G_Standard_Loading_1926_B_L;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Revised_B_G_Loading_1975_WG1_WG1;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Revised_B_G_Loading_1975_WAM4A_WAM4A;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Revised_B_G_Loading_1975_Bo_Bo_Bo_Bo;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Revised_B_G_Loading_1975_WAM4A;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Revised_B_G_Loading_1975_WAM4A_WAM4;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Revised_B_G_Loading_1975_WAM4A_WDM2;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_25t_Loading_2008_Combination_1;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_25t_Loading_2008_Combination_2;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_25t_Loading_2008_Combination_3;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_25t_Loading_2008_Combination_4;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_25t_Loading_2008_Combination_5;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_DFC_Loading_Combination_1;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_DFC_Loading_Combination_2;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_DFC_Loading_Combination_3;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_DFC_Loading_Combination_4;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_DFC_Loading_Combination_5;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
	}
	else if(m_Data.VehicleTypeName == TYPENAME_Metre_Gauge_1000mm)
	{
		str = SELNAME_2_Co_Co_Locomotives;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_2_Bo_Bo_Locomotives;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_MGML_Loading_of_1929;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_M_L;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_B_L;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_C;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
	}
	else if(m_Data.VehicleTypeName == TYPENAME_Narrow_Gauge_762mm)
	{
		str = SELNAME_Class_H_B_B_or_Bo_Bo_Type;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Class_H_C_C_or_Co_Co_Type;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Class_H_Steam_Zf_1;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Class_H_Diesel_Electric;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Class_A_B_B_or_Bo_Bo_Type;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Class_A_C_C_or_Co_Co_Type;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Class_A_Diesel_Mech_Elec;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Class_A_Diesel_Mech_Elec_Articulated;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Class_A_DRG_No_CSO_C_873;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Class_B_B_B_or_Bo_Bo_Type;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Class_B_Steam_Engine_Tank;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Class_B_Steam_Engine_Tender;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Class_B_Diesel_Electric;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
	}
	else if(m_Data.VehicleTypeName == TYPENAME_Heavy_Mineral_Loadings)
	{
		str = SELNAME_Train_Formation_No_1;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Train_Formation_No_2;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Train_Formation_No_3;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Train_Formation_No_4;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Train_Formation_No_5;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Train_Formation_No_6;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Train_Formation_No_7;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Train_Formation_No_8;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Train_Formation_No_9;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Train_Formation_No_10;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Train_Formation_No_11;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Train_Formation_No_12;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Train_Formation_No_13;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Train_Formation_No_14;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Train_Formation_No_15;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Train_Formation_No_16;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
		str = SELNAME_Train_Formation_No_17;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
	}
	else if(m_Data.VehicleTypeName == TYPENAME_Footbridge_Footpath)
	{
		str = SELNAME_Footbridge_Footpath;
		m_cmbSelVehicle.AddString(ConvSelVihicleStrRaw2Trans(str));
	}
	else
	{
		// Nothing
	}

	m_cmbSelVehicle.SetCurSel(0);
}

void CCMMvhlItemStdIndiaDlg::ChangeText()
{
	CString strPList1;
	CString strPList2;

	if     (m_Data.SelVehicle == SELNAME_Modified_B_G_Loading_1987_1)            strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_Modified_B_G_Loading_1987_2)            strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_B_G_Standard_Loading_1926_M_L)          strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_B_G_Standard_Loading_1926_B_L)          strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_Revised_B_G_Loading_1975_WG1_WG1)       strPList1 = _T("WG1+WG1");
	else if(m_Data.SelVehicle == SELNAME_Revised_B_G_Loading_1975_WAM4A_WAM4A)   strPList1 = _T("WAM4A+WAM4A");
	else if(m_Data.SelVehicle == SELNAME_Revised_B_G_Loading_1975_Bo_Bo_Bo_Bo)   strPList1 = _T("Bo-Bo+Bo-Bo"); 
	else if(m_Data.SelVehicle == SELNAME_Revised_B_G_Loading_1975_WAM4A)         strPList1 = _T("WAM4A"); 
	else if(m_Data.SelVehicle == SELNAME_Revised_B_G_Loading_1975_WAM4A_WAM4)    strPList1 = _T("WAM4A+WAM4");
	else if(m_Data.SelVehicle == SELNAME_Revised_B_G_Loading_1975_WAM4A_WDM2)    strPList1 = _T("WAM4A+WDM2");
	else if(m_Data.SelVehicle == SELNAME_25t_Loading_2008_Combination_1)		 strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_25t_Loading_2008_Combination_2)	     strPList1 = _T("2 WAG9H");
	else if(m_Data.SelVehicle == SELNAME_25t_Loading_2008_Combination_3)	     strPList1 = _T("(Bo-Bo)+(Bo-Bo)");
	else if(m_Data.SelVehicle == SELNAME_25t_Loading_2008_Combination_4)	     strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_25t_Loading_2008_Combination_5)	     strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_DFC_Loading_Combination_1)					     strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_DFC_Loading_Combination_2)					     strPList1 = _T("2 WAG9H");
	else if(m_Data.SelVehicle == SELNAME_DFC_Loading_Combination_3)					     strPList1 = _T("(Bo-Bo)+(Bo-Bo)");
	else if(m_Data.SelVehicle == SELNAME_DFC_Loading_Combination_4)              strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_DFC_Loading_Combination_5)              strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_2_Co_Co_Locomotives)                    strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_2_Bo_Bo_Locomotives)			               strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_MGML_Loading_of_1929)		               strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_M_L)									  	               strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_B_L)									  	               strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_C)										  	               strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_Class_H_B_B_or_Bo_Bo_Type)              strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_Class_H_C_C_or_Co_Co_Type) 						 strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_Class_H_Steam_Zf_1)										 strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_Class_H_Diesel_Electric)								 strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_Class_A_B_B_or_Bo_Bo_Type)							 strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_Class_A_C_C_or_Co_Co_Type)							 strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_Class_A_Diesel_Mech_Elec)							 strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_Class_A_Diesel_Mech_Elec_Articulated)	 strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_Class_A_DRG_No_CSO_C_873)							 strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_Class_B_B_B_or_Bo_Bo_Type)							 strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_Class_B_Steam_Engine_Tank)						   strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_Class_B_Steam_Engine_Tender)					   strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_Class_B_Diesel_Electric)                strPList1 = _T("2 LOCO");
	else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_1)                   strPList1 = _T("2 WAG6c");
	else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_2)		               strPList1 = _T("2 WAG6c");
	else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_3)		               strPList1 = _T("2 WDG2");
	else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_4)		               strPList1 = _T("2 WDG2");
	else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_5)		               strPList1 = _T("3 WAG6A");
	else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_6)		               strPList1 = _T("3 WAG6B");
	else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_7)		               strPList1 = _T("3 WAG6C");
	else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_8)		               strPList1 = _T("4 WDM2");
	else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_9)		               strPList1 = _T("3 WDG2");
	else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_10)		               strPList1 = _T("3 WDG2");
	else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_11)		               strPList1 = _T("3 WDG2");
	else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_12)		               strPList1 = _T("3 WAG6A");
	else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_13)		               strPList1 = _T("3 WAG6B");
	else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_14)		               strPList1 = _T("3 WAG6C");
	else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_15)		               strPList1 = _T("3 WAG6A");
	else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_16)		               strPList1 = _T("3 WAG6B");
	else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_17)		               strPList1 = _T("3 WAG6C");

	GetDlgItem(IDC_CMD_MVHL_IRS_PTLIST1_GRP)->SetWindowText(strPList1);

	strPList2 = _LS(IDS_WG_CMD_India_Gondola_Wagon);
	GetDlgItem(IDC_CMD_MVHL_IRS_PTLIST2_GRP)->SetWindowText(strPList2);
}

void CCMMvhlItemStdIndiaDlg::AlignControl()
{
	UpdateData(TRUE);

	int nCodeType = m_cmbCodeName.GetCurSel();
	int nVehlType = m_cmbVehicleType.GetCurSel();

	CRect rRef;
	CRect rToMove;
	CRect rectGrp;
	int nDistY = 0;

	// Select Vehicle
	GetDlgItem(IDC_CMD_MVHL_NOSE_TAIL_DIST_EDT)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_SEL_VEHICLE_CMB)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aSelVehicle, nDistY);

	// Min Nose to Tail Distance
	GetDlgItem(IDC_CMD_MVHL_NOSE_TAIL_DIST_EDT)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_NOSE_TAIL_DIST_WHEELED_EDT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aNoseTail70R, nDistY);

	// Picture 
	if(nCodeType == 0 && nVehlType == 2) // Class 70R
	{
		GetDlgItem(IDC_CMD_MVHL_NOSE_TAIL_DIST_TRACKED_EDT)->GetWindowRect(rRef);
	}
	else
	{
		GetDlgItem(IDC_CMD_MVHL_NOSE_TAIL_DIST_EDT)->GetWindowRect(rRef);
	}

	GetDlgItem(IDC_CMD_MVHL_PICTURE)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, m_aPicture, nDistY);

	// IRC
	GetDlgItem(IDC_CMD_MVHL_PICTURE)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_IRC_PTLIST)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, m_aIRC, nDistY);

	// IRC Fatigue
	GetDlgItem(IDC_CMD_MVHL_PICTURE)->GetWindowRect(rRef);
	GetDlgItem(IDC_WG_CMD_IRC_VEHIC_LOAD_GRP)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + 8;
	CDlgUtil::CtrlMoveDistY(this, m_aIRCFatigue, nDistY);

	// IRS Point Load 1, 2
	GetDlgItem(IDC_CMD_MVHL_IRC_PTLIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_IRS_PTLIST1_GRP)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aIRS_Point1, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aIRS_Point2, nDistY);

	// IRS Dist
	GetDlgItem(IDC_CMD_MVHL_IRS_PTLIST2)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_IRS_DW_EDT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aIRS_Dist, nDistY);
	
	// FootWay
	GetDlgItem(IDC_CMD_MVHL_IRC_PTLIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_IRC_W_EDT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aFooyWay, nDistY);

	// FootPath
	GetDlgItem(IDC_CMD_MVHL_IRC_PTLIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_IRS_L_EDT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aFooyPath, nDistY);
	
	// Group Box 크기
	GetDlgItem(IDC_CMD_MVHL_VEHI_PROP_GRP)->GetWindowRect(rectGrp);

	if (nCodeType == 0)
	{
		if(nVehlType == 6) GetDlgItem(IDC_WG_CMD_IRC_ROUGHNESS_GRP)->GetWindowRect(rRef); // IRC Fatigue
		else               GetDlgItem(IDC_CMD_MVHL_IRC_PTLIST)->GetWindowRect(rRef);
	}
	else               
		GetDlgItem(IDC_CMD_MVHL_IRS_PTLIST1_GRP)->GetWindowRect(rRef);

	rectGrp.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	ScreenToClient(rectGrp);
	GetDlgItem(IDC_CMD_MVHL_VEHI_PROP_GRP)->MoveWindow(rectGrp);

	// Longitudinal Force
	GetDlgItem(IDC_CMD_MVHL_VEHI_PROP_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_LONGI_GRP)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, m_aLongiForce, nDistY);

	// Braking for LOCO
	GetDlgItem(IDC_CMD_MVHL_BRAKE_LOCO_RATIO_EDT)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_BRAKE_LOCO_EDT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aBrakeLOCO, nDistY);
	
	// OK, Cancel, Apply
	if(nCodeType == 0) 
	{
		GetDlgItem(IDC_CMD_MVHL_VEHI_PROP_GRP)->GetWindowRect(rRef);
	}
	else
	{
		if(nVehlType == 4) GetDlgItem(IDC_CMD_MVHL_VEHI_PROP_GRP)->GetWindowRect(rRef);
		else               GetDlgItem(IDC_CMD_MVHL_LONGI_GRP)->GetWindowRect(rRef);	
	}
		
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, m_aOKCancel, nDistY);

	CRect r;
	GetWindowRect(r);
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rToMove);
	r.bottom = rToMove.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);

	RedrawWindow();	
}

void CCMMvhlItemStdIndiaDlg::ControlsShowHide()
{
	UpdateData(TRUE);

	int nCodeType = m_cmbCodeName.GetCurSel();
	int nVehlType = m_cmbVehicleType.GetCurSel();
	int nSelVehl = m_cmbSelVehicle.GetCurSel();

	BOOL bPointLoad = FALSE;
	BOOL bDistLoad = FALSE;
	GetIRSVehlLoadType(bPointLoad, bDistLoad);
	
	CDlgUtil::CtrlShowHide(this, m_aNoseTail,          nCodeType==0 && (nVehlType!=2 && nVehlType!=5 && nVehlType != 6));
	CDlgUtil::CtrlShowHide(this, m_aNoseTail70R,       nCodeType==0 && nVehlType==2);
	CDlgUtil::CtrlShowHide(this, m_aSelVehicle,        nCodeType==1);
	CDlgUtil::CtrlShowHide(this, m_aIRC,               nCodeType==0 && (nVehlType!=5 && nVehlType!=6) );
	CDlgUtil::CtrlShowHide(this, m_aIRCFatigue,        nCodeType==0 && nVehlType==6);
	CDlgUtil::CtrlShowHide(this, m_aIRS_Point1,        nCodeType==1 && nVehlType!=4);
	CDlgUtil::CtrlShowHide(this, m_aIRS_Point2,        nCodeType==1 && nVehlType!=4 && bPointLoad);
	CDlgUtil::CtrlShowHide(this, m_aIRS_Dist,          nCodeType==1 && nVehlType!=4 && bDistLoad);
	CDlgUtil::CtrlShowHide(this, m_aFooyWay,           nCodeType==0 && nVehlType==5);
	CDlgUtil::CtrlShowHide(this, m_aFooyPath,          nCodeType==1 && nVehlType==4);
	CDlgUtil::CtrlShowHide(this, m_aLongiForce,        nCodeType==1 && nVehlType!=4);
	CDlgUtil::CtrlShowHide(this, m_aBrakeLOCORatio,    nCodeType==1 && nVehlType!=3 && nVehlType!=4 );
	CDlgUtil::CtrlShowHide(this, m_aBrakeTrainRatio,   nCodeType==1 && nVehlType!=4 );
	CDlgUtil::CtrlShowHide(this, m_aBrakeLOCO,         nCodeType==1 && nVehlType==3);
}

void CCMMvhlItemStdIndiaDlg::GetIndex(int nCodeType, int &nVehlType, int &nSelVehi)
{
	for (int i = 0; i < D_MAX_COUNT; i++)
	{
		if(CVehlDB::GetTypeListAr()[m_Data.nStandardCode][i] == m_Data.VehicleTypeName)
		{
			nVehlType = i;
			break;
		}
	}

	if(nCodeType == D_MVHL_IRS_BRIDGE_RULES)
	{
		if(m_Data.VehicleTypeName == TYPENAME_Broad_Gauge_1676mm)
		{
			if     (m_Data.SelVehicle == SELNAME_Modified_B_G_Loading_1987_1)            nSelVehi = 0;
			else if(m_Data.SelVehicle == SELNAME_Modified_B_G_Loading_1987_2)            nSelVehi = 1; 
			else if(m_Data.SelVehicle == SELNAME_B_G_Standard_Loading_1926_M_L)          nSelVehi = 2; 
			else if(m_Data.SelVehicle == SELNAME_B_G_Standard_Loading_1926_B_L)          nSelVehi = 3; 
			else if(m_Data.SelVehicle == SELNAME_Revised_B_G_Loading_1975_WG1_WG1)       nSelVehi = 4; 
			else if(m_Data.SelVehicle == SELNAME_Revised_B_G_Loading_1975_WAM4A_WAM4A)   nSelVehi = 5;  
			else if(m_Data.SelVehicle == SELNAME_Revised_B_G_Loading_1975_Bo_Bo_Bo_Bo)   nSelVehi = 6; 
			else if(m_Data.SelVehicle == SELNAME_Revised_B_G_Loading_1975_WAM4A)         nSelVehi = 7; 
			else if(m_Data.SelVehicle == SELNAME_Revised_B_G_Loading_1975_WAM4A_WAM4)    nSelVehi = 8;
			else if(m_Data.SelVehicle == SELNAME_Revised_B_G_Loading_1975_WAM4A_WDM2)    nSelVehi = 9;
			else if(m_Data.SelVehicle == SELNAME_25t_Loading_2008_Combination_1)		     nSelVehi = 10;
			else if(m_Data.SelVehicle == SELNAME_25t_Loading_2008_Combination_2)	    	 nSelVehi = 11;
			else if(m_Data.SelVehicle == SELNAME_25t_Loading_2008_Combination_3)	    	 nSelVehi = 12;
			else if(m_Data.SelVehicle == SELNAME_25t_Loading_2008_Combination_4)	    	 nSelVehi = 13;
			else if(m_Data.SelVehicle == SELNAME_25t_Loading_2008_Combination_5)	    	 nSelVehi = 14;
			else if(m_Data.SelVehicle == SELNAME_DFC_Loading_Combination_1)					     nSelVehi = 15;
			else if(m_Data.SelVehicle == SELNAME_DFC_Loading_Combination_2)					     nSelVehi = 16;
			else if(m_Data.SelVehicle == SELNAME_DFC_Loading_Combination_3)					     nSelVehi = 17;
			else if(m_Data.SelVehicle == SELNAME_DFC_Loading_Combination_4)              nSelVehi = 18;
			else if(m_Data.SelVehicle == SELNAME_DFC_Loading_Combination_5)              nSelVehi = 19;
		}
		else if(m_Data.VehicleTypeName == TYPENAME_Metre_Gauge_1000mm)
		{
			if     (m_Data.SelVehicle == SELNAME_2_Co_Co_Locomotives)     nSelVehi = 0;
			else if(m_Data.SelVehicle == SELNAME_2_Bo_Bo_Locomotives)			nSelVehi = 1;
			else if(m_Data.SelVehicle == SELNAME_MGML_Loading_of_1929)		nSelVehi = 2;
			else if(m_Data.SelVehicle == SELNAME_M_L)									  	nSelVehi = 3;
			else if(m_Data.SelVehicle == SELNAME_B_L)									  	nSelVehi = 4;
			else if(m_Data.SelVehicle == SELNAME_C)										  	nSelVehi = 5;
		}		
		else if(m_Data.VehicleTypeName == TYPENAME_Narrow_Gauge_762mm)
		{
			if     (m_Data.SelVehicle == SELNAME_Class_H_B_B_or_Bo_Bo_Type)              nSelVehi = 0;
			else if(m_Data.SelVehicle == SELNAME_Class_H_C_C_or_Co_Co_Type) 						 nSelVehi = 1; 
			else if(m_Data.SelVehicle == SELNAME_Class_H_Steam_Zf_1)										 nSelVehi = 2; 
			else if(m_Data.SelVehicle == SELNAME_Class_H_Diesel_Electric)								 nSelVehi = 3; 
			else if(m_Data.SelVehicle == SELNAME_Class_A_B_B_or_Bo_Bo_Type)							 nSelVehi = 4; 
			else if(m_Data.SelVehicle == SELNAME_Class_A_C_C_or_Co_Co_Type)							 nSelVehi = 5; 
			else if(m_Data.SelVehicle == SELNAME_Class_A_Diesel_Mech_Elec)							 nSelVehi = 6; 
			else if(m_Data.SelVehicle == SELNAME_Class_A_Diesel_Mech_Elec_Articulated)	 nSelVehi = 7; 
			else if(m_Data.SelVehicle == SELNAME_Class_A_DRG_No_CSO_C_873)							 nSelVehi = 8;
			else if(m_Data.SelVehicle == SELNAME_Class_B_B_B_or_Bo_Bo_Type)							 nSelVehi = 9;
			else if(m_Data.SelVehicle == SELNAME_Class_B_Steam_Engine_Tank)						   nSelVehi = 10;
			else if(m_Data.SelVehicle == SELNAME_Class_B_Steam_Engine_Tender)					   nSelVehi = 11;
			else if(m_Data.SelVehicle == SELNAME_Class_B_Diesel_Electric)                nSelVehi = 12;
		}
		else if(m_Data.VehicleTypeName == TYPENAME_Heavy_Mineral_Loadings)
		{
			if     (m_Data.SelVehicle == SELNAME_Train_Formation_No_1)    nSelVehi = 0;
			else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_2)		nSelVehi = 1; 
			else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_3)		nSelVehi = 2; 
			else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_4)		nSelVehi = 3; 
			else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_5)		nSelVehi = 4; 
			else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_6)		nSelVehi = 5; 
			else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_7)		nSelVehi = 6; 
			else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_8)		nSelVehi = 7; 
			else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_9)		nSelVehi = 8;
			else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_10)		nSelVehi = 9;
			else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_11)		nSelVehi = 10;
			else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_12)		nSelVehi = 11;
			else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_13)		nSelVehi = 12;
			else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_14)		nSelVehi = 13;
			else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_15)		nSelVehi = 14;
			else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_16)		nSelVehi = 15;
			else if(m_Data.SelVehicle == SELNAME_Train_Formation_No_17)		nSelVehi = 16;
		}
		else if(m_Data.VehicleTypeName == TYPENAME_Footbridge_Footpath)
		{
			if(m_Data.SelVehicle == SELNAME_Footbridge_Footpath)  nSelVehi = 0;
		}
		else
		{
			// Nothing
		}
	}
}

void CCMMvhlItemStdIndiaDlg::ChangeBitmap(int nCodeType, int nVehlType, int nSelVehi)
{
	CString strSVG;

	if(nCodeType == D_MVHL_IRC6_2000)
	{
		if     (nVehlType == 0)   strSVG = _T("cmd_mvhl_208.svg");
		else if(nVehlType == 1)   strSVG = _T("cmd_mvhl_208.svg");
		else if(nVehlType == 2)   strSVG = _T("cmd_mvhl_209.svg");
		else if(nVehlType == 3)   strSVG = _T("cmd_mvhl_210.svg");
		else if(nVehlType == 4)   strSVG = _T("cmd_mvhl_211.svg");
		else if(nVehlType == 5)   strSVG = _T("cmd_mvhl_212.svg");
		else if(nVehlType == 6)   strSVG = _T("cmd_mvhl_IRC_Fatigue.svg");
		else ASSERT(0);
	}
	else if(nCodeType == D_MVHL_IRS_BRIDGE_RULES)
	{
		if(nVehlType == 0)
		{
			if     (nSelVehi == 0) strSVG = _T("cmd_mvhl_213.svg");
			else if(nSelVehi == 1) strSVG = _T("cmd_mvhl_213.svg");
			else if(nSelVehi == 2) strSVG = _T("cmd_mvhl_214.svg");
			else if(nSelVehi == 3) strSVG = _T("cmd_mvhl_214.svg");
			else if(nSelVehi == 4) strSVG = _T("cmd_mvhl_214.svg");
			else if(nSelVehi == 5) strSVG = _T("cmd_mvhl_214.svg");
			else if(nSelVehi == 6) strSVG = _T("cmd_mvhl_214.svg");
			else if(nSelVehi == 7) strSVG = _T("cmd_mvhl_214.svg");
			else if(nSelVehi == 8) strSVG = _T("cmd_mvhl_214.svg");
			else if(nSelVehi == 9) strSVG = _T("cmd_mvhl_214.svg");
			else if(nSelVehi == 10) strSVG = _T("cmd_mvhl_213.svg");
			else if(nSelVehi == 11) strSVG = _T("cmd_mvhl_213.svg");
			else if(nSelVehi == 12) strSVG = _T("cmd_mvhl_213.svg");
			else if(nSelVehi == 13) strSVG = _T("cmd_mvhl_213.svg");
			else if(nSelVehi == 14) strSVG = _T("cmd_mvhl_213.svg");
			else if(nSelVehi == 15) strSVG = _T("cmd_mvhl_215.svg");
			else if(nSelVehi == 16) strSVG = _T("cmd_mvhl_215.svg");
			else if(nSelVehi == 17) strSVG = _T("cmd_mvhl_215.svg");
			else if(nSelVehi == 18) strSVG = _T("cmd_mvhl_215.svg");
			else if(nSelVehi == 19) strSVG = _T("cmd_mvhl_215.svg");
			else ASSERT(0);
		}
		else if(nVehlType == 1)
		{
			if     (nSelVehi == 0) strSVG = _T("cmd_mvhl_213.svg");
			else if(nSelVehi == 1) strSVG = _T("cmd_mvhl_213.svg");
			else if(nSelVehi == 2) strSVG = _T("cmd_mvhl_213.svg");
			else if(nSelVehi == 3) strSVG = _T("cmd_mvhl_214.svg");
			else if(nSelVehi == 4) strSVG = _T("cmd_mvhl_214.svg");
			else if(nSelVehi == 5) strSVG = _T("cmd_mvhl_214.svg");
			else ASSERT(0);
		}
		else if(nVehlType == 2)
		{
			if (nSelVehi == 0) strSVG = _T("cmd_mvhl_214.svg");
			else if (nSelVehi == 1) strSVG = _T("cmd_mvhl_214.svg");
			else if (nSelVehi == 2) strSVG = _T("cmd_mvhl_214.svg");
			else if (nSelVehi == 3) strSVG = _T("cmd_mvhl_214.svg");
			else if (nSelVehi == 4) strSVG = _T("cmd_mvhl_214.svg");
			else if (nSelVehi == 5) strSVG = _T("cmd_mvhl_214.svg");
			else if (nSelVehi == 6) strSVG = _T("cmd_mvhl_214.svg");
			else if (nSelVehi == 7) strSVG = _T("cmd_mvhl_214.svg");
			else if (nSelVehi == 8) strSVG = _T("cmd_mvhl_214.svg");
			else if (nSelVehi == 9) strSVG = _T("cmd_mvhl_214.svg");
			else if(nSelVehi == 10) strSVG = _T("cmd_mvhl_214.svg");
			else if(nSelVehi == 11) strSVG = _T("cmd_mvhl_214.svg");
			else if(nSelVehi == 12) strSVG = _T("cmd_mvhl_214.svg");
			else ASSERT(0);
		}
		else if(nVehlType == 3)
		{
			if     (nSelVehi == 0) strSVG = _T("cmd_mvhl_216.svg");
			else if(nSelVehi == 1) strSVG = _T("cmd_mvhl_214.svg");
			else if(nSelVehi == 2) strSVG = _T("cmd_mvhl_216.svg");
			else if(nSelVehi == 3) strSVG = _T("cmd_mvhl_214.svg");
			else if(nSelVehi == 4) strSVG = _T("cmd_mvhl_215.svg");
			else if(nSelVehi == 5) strSVG = _T("cmd_mvhl_215.svg");
			else if(nSelVehi == 6) strSVG = _T("cmd_mvhl_215.svg");
			else if(nSelVehi == 7) strSVG = _T("cmd_mvhl_216.svg");
			else if(nSelVehi == 8) strSVG = _T("cmd_mvhl_216.svg");
			else if(nSelVehi == 9) strSVG = _T("cmd_mvhl_213.svg");
			else if(nSelVehi == 10) strSVG = _T("cmd_mvhl_215.svg");
			else if(nSelVehi == 11) strSVG = _T("cmd_mvhl_213.svg");
			else if(nSelVehi == 12) strSVG = _T("cmd_mvhl_213.svg");
			else if(nSelVehi == 13) strSVG = _T("cmd_mvhl_213.svg");
			else if(nSelVehi == 14) strSVG = _T("cmd_mvhl_216.svg");
			else if(nSelVehi == 15) strSVG = _T("cmd_mvhl_216.svg");
			else if(nSelVehi == 16) strSVG = _T("cmd_mvhl_216.svg");
			else ASSERT(0);
		}
		else if(nVehlType == 4)
		{
			strSVG = _T("cmd_mvhl_221.svg");
		}
		else
		{
			ASSERT(0);
		}
	}
	else
	{
		ASSERT(0);
	}

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + strSVG);
}

void CCMMvhlItemStdIndiaDlg::SetHeaderTitle()
{
	// IRC
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
	dwStyle = ListView_GetExtendedListViewStyle(m_lstIRC.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_lstIRC.GetSafeHwnd(), dwStyle);

	dwStyle = ListView_GetExtendedListViewStyle(m_lstI12T.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_lstI12T.GetSafeHwnd(), dwStyle);

	dwStyle = ListView_GetExtendedListViewStyle(m_lstI40T.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_lstI40T.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 28; nColWidth[1] = 81; nColWidth[2] = 101;

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_RIGHT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_lstIRC.InsertColumn(i,&lvcolumn);
		m_lstI12T.InsertColumn(i,&lvcolumn);
		m_lstI40T.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}

	// IRS1, IRS2
	aTitle[0] = _LS(IDS_WG_CMD__ADDD__No);
	aTitle[1] = _LS(IDS_CMD_MVHL_P);
	aTitle[2] = _LS(IDS_CMD_MVHL_D);

	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointLoad,sLdUnit);
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointDistance,sDtUnit);
	aTitle[1] = aTitle[1] +_T("(")+sLdUnit+_T(")");
	aTitle[2] = aTitle[2] +_T("(")+sDtUnit+_T(")");
	
	dwStyle = ListView_GetExtendedListViewStyle(m_lstIRS1.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_lstIRS1.GetSafeHwnd(), dwStyle);

	dwStyle = ListView_GetExtendedListViewStyle(m_lstIRS2.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_lstIRS2.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 28; nColWidth[1] = 72; nColWidth[2] = 72;

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_RIGHT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_lstIRS1.InsertColumn(i,&lvcolumn);
		m_lstIRS2.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvhlItemStdIndiaDlg::MakeItemEx()
{
	// IRC
	m_lstIRC.DeleteAllItems();    
	int nLoadCount = GetLoadCount(IRC_LIST);
	int nDistCount = GetDistCount(IRC_LIST);

	int i = 0;
	if (nLoadCount > 0)
	{
		for (i = 0; i < nDistCount; i++)
			InsertItem(IRC_LIST, i, i+1, m_Data.dPointLoad[i], m_Data.dPointDistance[i]);

		if (nLoadCount > nDistCount)
			InsertItem(IRC_LIST, i, i+1, m_Data.dPointLoad[i], 0.0);
	}

	// IRS1
	m_lstIRS1.DeleteAllItems();
	nLoadCount = GetLoadCount(IRS1_LIST);
	nDistCount = GetDistCount(IRS1_LIST);

	if (nLoadCount > 0)
	{
		for (i = 0; i < nDistCount; i++)
			InsertItem(IRS1_LIST, i, i+1, m_Data.dPointLoad[i], m_Data.dPointDistance[i]);

		if (nLoadCount > nDistCount)
			InsertItem(IRS1_LIST, i, i+1, m_Data.dPointLoad[i], 0.0);
	}
	
	// IRS2
	m_lstIRS2.DeleteAllItems();
	nLoadCount = GetLoadCount(IRS2_LIST);
	nDistCount = GetDistCount(IRS2_LIST);

	if (nLoadCount > 0)
	{
		for (i = 0; i < nDistCount; i++)
			InsertItem(IRS2_LIST, i, i+1, m_Data.dPointLoad3[0][i], m_Data.dPointDistance3[0][i]);

		if (nLoadCount > nDistCount)
			InsertItem(IRS2_LIST, i, i+1, m_Data.dPointLoad3[0][i], 0.0);
	}

	//////////////////////////////////////////////////////////////////////////
	// 12T
	m_lstI12T.DeleteAllItems();
	nLoadCount = GetLoadCount(IRS_12T_LIST);
	nDistCount = GetDistCount(IRS_12T_LIST);

	if (nLoadCount > 0)
	{
		for (i = 0; i < nDistCount; i++)
			InsertItem(IRS_12T_LIST, i, i+1, m_Data.dPointLoad3[1][i], m_Data.dPointDistance3[1][i]);

		if (nLoadCount > nDistCount)
			InsertItem(IRS_12T_LIST, i, i+1, m_Data.dPointLoad3[1][i], 0.0);
	}

	// 40T
	m_lstI40T.DeleteAllItems();
	nLoadCount = GetLoadCount(IRS_40T_LIST);
	nDistCount = GetDistCount(IRS_40T_LIST);

	if (nLoadCount > 0)
	{
		for (i = 0; i < nDistCount; i++)
			InsertItem(IRS_40T_LIST, i, i + 1, m_Data.dPointLoad3[2][i], m_Data.dPointDistance3[2][i]);

		if (nLoadCount > nDistCount)
			InsertItem(IRS_40T_LIST, i, i + 1, m_Data.dPointLoad3[2][i], 0.0);
	}

			
	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdIndiaDlg::InsertItem(int nListType, int nPos, int nNo, double dLoad, double dDist)
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

		if(nListType == IRC_LIST)
		{
			if(i == 0) m_lstIRC.InsertItem(&lvitem);
			else       m_lstIRC.SetItem(&lvitem);
		}
		else if(nListType == IRS1_LIST)
		{
			if(i == 0) m_lstIRS1.InsertItem(&lvitem);
			else       m_lstIRS1.SetItem(&lvitem);
		}
		else if(nListType == IRS2_LIST)
		{
			if(i == 0) m_lstIRS2.InsertItem(&lvitem);
			else       m_lstIRS2.SetItem(&lvitem);
		}
		else if(nListType == IRS_12T_LIST)
		{
			if(i == 0) m_lstI12T.InsertItem(&lvitem);
			else       m_lstI12T.SetItem(&lvitem);
		}
		else if(nListType == IRS_40T_LIST)
		{
			if(i == 0) m_lstI40T.InsertItem(&lvitem);
			else       m_lstI40T.SetItem(&lvitem);
		}
		else
		{
			ASSERT(0);
		}		

		str.ReleaseBuffer();
	}

	return TRUE;
}

int CCMMvhlItemStdIndiaDlg::GetLoadCount(int nListType)
{
	int i=0;

	if(nListType == IRC_LIST)
	{
		for(i=0; i<D_MVHL_NUMLOAD; i++)
		{
			if (m_Data.dPointLoad[i] == 0.0) break;
		}
	}
	else if(nListType == IRS1_LIST)
	{
		for(i=0; i<D_MVHL_NUMLOAD; i++)
		{
			if (m_Data.dPointLoad[i] == 0.0) break;
		}
	}
	else if(nListType == IRS2_LIST)
	{
		for(i=0; i<D_MVHL_NUMLOAD; i++)
		{
			if (m_Data.dPointLoad3[0][i] == 0.0) break;
		}
	}
	else if(nListType == IRS_12T_LIST)
	{
		for(i=0; i<D_MVHL_NUMLOAD; i++)
		{
			if (m_Data.dPointLoad3[1][i] == 0.0) break;
		}
	}
	else if(nListType == IRS_40T_LIST)
	{
		for(i=0; i<D_MVHL_NUMLOAD; i++)
		{
			if (m_Data.dPointLoad3[2][i] == 0.0) break;
		}
	}
	else
	{
		ASSERT(0);
	}
	
	return i;
}

int CCMMvhlItemStdIndiaDlg::GetDistCount(int nListType)
{
	int i=0;

	if(nListType == IRC_LIST)
	{
		for(i=0; i<D_MVHL_NUMLOAD; i++)
		{
			if (m_Data.dPointDistance[i] == 0.0) break;
		}
	}
	else if(nListType == IRS1_LIST)
	{
		for(i=0; i<D_MVHL_NUMLOAD; i++)
		{
			if (m_Data.dPointDistance[i] == 0.0) break;
		}
	}
	else if(nListType == IRS2_LIST)
	{
		for(i=0; i<D_MVHL_NUMLOAD; i++)
		{
			if (m_Data.dPointDistance3[0][i] == 0.0) break;
		}
	}
	else if(nListType == IRS_12T_LIST)
	{
		for(i=0; i<D_MVHL_NUMLOAD; i++)
		{
			if (m_Data.dPointDistance3[1][i] == 0.0) break;
		}
	}
	else if(nListType == IRS_40T_LIST)
	{
		for(i=0; i<D_MVHL_NUMLOAD; i++)
		{
			if (m_Data.dPointDistance3[2][i] == 0.0) break;
		}
	}
	else
	{
		ASSERT(0);
	}

	return i;
}

void CCMMvhlItemStdIndiaDlg::Data2Dlg()
{
	if(m_Data.VehicleLoadName.IsEmpty())
		m_edtVehicleName.SetWindowText(ConvVehicleNameStrRaw2Trans(m_Data.VehicleTypeName));
	else
		m_edtVehicleName.SetWindowText(m_Data.VehicleLoadName);
	
	CDlgUtil::CobxSetCurSelItemData(m_cmbCodeName, m_Data.nStandardCode);
	ChangeCodeCombo();
	ChangeVehlTypeCombo();

	int nCodeType = m_Data.nStandardCode;
	int nVehlType = 0;
	int nSelVehi = 0;
	GetIndex(nCodeType, nVehlType, nSelVehi);

	m_cmbVehicleType.SetCurSel(nVehlType);
	m_cmbSelVehicle.SetCurSel(nSelVehi);
	ChangeBitmap(nCodeType, nVehlType, nSelVehi);

	double dTractive = 0.0;
	double dBrakeLocoRatio = 0.0;
	double dBrakeTrainRatio = 0.0;
	double dBrakeLoco = 0.0;

	if(m_bModify)
	{
		dTractive = m_Data.dTractive;
		dBrakeLocoRatio = m_Data.dBrakeLocoRatio;
		dBrakeTrainRatio = m_Data.dBrakeTrainRatio;
		dBrakeLoco = m_Data.dBrakeLoco;
	}

	CVehlDB db(m_pDoc);  
	if (db.GetStandardVehicleLoadValue(m_Data))	MakeItemEx();

	if(m_bModify)
	{
		m_Data.dTractive = dTractive;
		m_Data.dBrakeLocoRatio = dBrakeLocoRatio;
		m_Data.dBrakeTrainRatio = dBrakeTrainRatio;
		m_Data.dBrakeLoco = dBrakeLoco;
	}
	
	m_edtNoseTailDist.SetEditUnit(m_Data.dNoseTailDist);
	m_edtNoseTailDistW.SetEditUnit(m_Data.dNoseTailDist);
	m_edtNoseTailDistT.SetEditUnit(m_Data.dNoseTailDist2);
	m_edtIRCdD1.SetEditUnit(m_Data.dD1);
	m_edtIRCdD2.SetEditUnit(m_Data.dD2);
	m_edtIRCP.SetEditUnit(m_Data.dP);
	m_edtIRCD.SetEditUnit(m_Data.dDTrailer);
	m_edtIRCPb.SetEditUnit(m_Data.dPb);
	m_edtIRCDb.SetEditUnit(m_Data.dDb);
	m_edtIRCW.SetEditUnit(m_Data.dFootway);
	m_edtIRCFW.SetEditUnit(m_Data.dFootwayWidth);

	m_edtIRSdD12.SetEditUnit(m_Data.dTrainD1);
	m_edtIRSdD22.SetEditUnit(m_Data.dTrainD2);
	m_edtIRSdD3.SetEditUnit(m_Data.dTrainD3);
	m_edtIRSdW.SetEditUnit(m_Data.dUniformLoad);
	m_edtIRSdD11.SetEditUnit(m_Data.dTrainD1);
	m_edtIRSdD21.SetEditUnit(m_Data.dTrainD2);
	m_edtTractive.SetEditUnit(m_Data.dTractive);
	m_edtBrakeLOCORatio.SetEditUnit(m_Data.dBrakeLocoRatio);
	m_edtBrakeTrainRatio.SetEditUnit(m_Data.dBrakeTrainRatio);
	m_edtBrakeLOCO.SetEditUnit(m_Data.dBrakeLoco);
	m_edtIRSL.SetEditUnit(m_Data.dSpanLength);
	m_edtIRSFW.SetEditUnit(m_Data.dFootwayWidth);

	m_edtIRCMinD.SetEditUnit(m_Data.dMinSpacing);
	m_edtRoughFactor.SetEditUnit(m_Data.dLaneFactor[0]);
	m_nFactorType = m_Data.nLaneFactor;

	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdIndiaDlg::Dlg2Data()
{
	UpdateData(TRUE);
	
	m_Data.Initialize();  
	m_Data.bStandard = TRUE;
	m_Data.nStandardCode = CDlgUtil::CobxGetCurSelItemData(m_cmbCodeName, m_cmbCodeName.GetCurSel());
	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);
	m_Data.VehicleTypeName = GetVehicleTypeStr();
	m_Data.SelVehicle = GetSelVihicleStr();
	
	CVehlDB db(m_pDoc);
	if(!db.GetStandardVehicleLoadValue(m_Data)) return FALSE;  

	m_Data.dFootway = m_edtIRCW.GetEditValue();
	if(m_Data.nStandardCode == D_MVHL_IRC6_2000) m_Data.dFootwayWidth = m_edtIRCFW.GetEditValue();
	else                                         m_Data.dFootwayWidth = m_edtIRSFW.GetEditValue();
	
	m_Data.dTractive = m_edtTractive.GetEditValue();
	m_Data.dBrakeLocoRatio = m_edtBrakeLOCORatio.GetEditValue();
	m_Data.dBrakeTrainRatio = m_edtBrakeTrainRatio.GetEditValue();
	m_Data.dBrakeLoco = m_edtBrakeLOCO.GetEditValue();
	m_Data.dSpanLength = m_edtIRSL.GetEditValue();

	m_Data.dMinSpacing    = m_edtIRCMinD.GetEditValue();
	m_Data.nLaneFactor    = m_nFactorType;
	if      (m_nFactorType == 0)  m_Data.dLaneFactor[0] = 1.2;
	else if (m_nFactorType == 1)  m_Data.dLaneFactor[0] = 1.4;
	else                          m_Data.dLaneFactor[0] = m_edtRoughFactor.GetEditValue();

	return TRUE;
}

void CCMMvhlItemStdIndiaDlg::ChangeData()
{
	UpdateData(TRUE);

	m_Data.Initialize();
	m_Data.bStandard = TRUE;
	m_Data.nStandardCode = CDlgUtil::CobxGetCurSelItemData(m_cmbCodeName, m_cmbCodeName.GetCurSel());
	m_Data.VehicleTypeName = GetVehicleTypeStr();
	m_Data.SelVehicle = GetSelVihicleStr();

	if(m_Data.VehicleLoadName.IsEmpty())
		m_edtVehicleName.SetWindowText(ConvVehicleNameStrRaw2Trans(m_Data.VehicleTypeName));
	else
		m_edtVehicleName.SetWindowText(m_Data.VehicleLoadName);
	
	int nCodeType = m_Data.nStandardCode;
	int nVehlType = 0;
	int nSelVehi = 0;
	GetIndex(nCodeType, nVehlType, nSelVehi);
	ChangeBitmap(nCodeType, nVehlType, nSelVehi);

	CVehlDB db(m_pDoc);  
	if (db.GetStandardVehicleLoadValue(m_Data))	MakeItemEx();

	m_edtNoseTailDist.SetEditUnit(m_Data.dNoseTailDist);
	m_edtNoseTailDistW.SetEditUnit(m_Data.dNoseTailDist);
	m_edtNoseTailDistT.SetEditUnit(m_Data.dNoseTailDist2);
	m_edtIRCdD1.SetEditUnit(m_Data.dD1);
	m_edtIRCdD2.SetEditUnit(m_Data.dD2);
	m_edtIRCP.SetEditUnit(m_Data.dP);
	m_edtIRCD.SetEditUnit(m_Data.dDTrailer);
	m_edtIRCPb.SetEditUnit(m_Data.dPb);
	m_edtIRCDb.SetEditUnit(m_Data.dDb);

	m_edtIRSdD12.SetEditUnit(m_Data.dTrainD1);
	m_edtIRSdD22.SetEditUnit(m_Data.dTrainD2);
	m_edtIRSdD3.SetEditUnit(m_Data.dTrainD3);
	m_edtIRSdW.SetEditUnit(m_Data.dUniformLoad);
	m_edtIRSdD11.SetEditUnit(m_Data.dTrainD1);
	m_edtIRSdD21.SetEditUnit(m_Data.dTrainD2);
	m_edtTractive.SetEditUnit(m_Data.dTractive);
	m_edtBrakeLOCORatio.SetEditUnit(m_Data.dBrakeLocoRatio);
	m_edtBrakeTrainRatio.SetEditUnit(m_Data.dBrakeTrainRatio);
	m_edtBrakeLOCO.SetEditUnit(m_Data.dBrakeLoco);
}

void CCMMvhlItemStdIndiaDlg::GetIRSVehlLoadType(BOOL& bPointLoad, BOOL& bDistLoad)
{
	bPointLoad = FALSE;
	bDistLoad = FALSE;

	CString strSelVehicle;
	strSelVehicle = GetSelVihicleStr();

	if     (strSelVehicle == SELNAME_Modified_B_G_Loading_1987_1            ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Modified_B_G_Loading_1987_2            ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_B_G_Standard_Loading_1926_M_L          ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_B_G_Standard_Loading_1926_B_L          ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Revised_B_G_Loading_1975_WG1_WG1       ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Revised_B_G_Loading_1975_WAM4A_WAM4A   ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Revised_B_G_Loading_1975_Bo_Bo_Bo_Bo   ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Revised_B_G_Loading_1975_WAM4A         ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Revised_B_G_Loading_1975_WAM4A_WAM4    ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Revised_B_G_Loading_1975_WAM4A_WDM2    ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_25t_Loading_2008_Combination_1         ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_25t_Loading_2008_Combination_2         ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_25t_Loading_2008_Combination_3         ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_25t_Loading_2008_Combination_4         ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_25t_Loading_2008_Combination_5         ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_DFC_Loading_Combination_1              ) { bPointLoad = TRUE;  bDistLoad = FALSE; }
	else if(strSelVehicle == SELNAME_DFC_Loading_Combination_2              ) { bPointLoad = TRUE;  bDistLoad = FALSE; }
	else if(strSelVehicle == SELNAME_DFC_Loading_Combination_3              ) { bPointLoad = TRUE;  bDistLoad = FALSE; }
	else if(strSelVehicle == SELNAME_DFC_Loading_Combination_4              ) { bPointLoad = TRUE;  bDistLoad = FALSE; }
	else if(strSelVehicle == SELNAME_DFC_Loading_Combination_5              ) { bPointLoad = TRUE;  bDistLoad = FALSE; }
	else if(strSelVehicle == SELNAME_2_Co_Co_Locomotives                    ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_2_Bo_Bo_Locomotives                    ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_MGML_Loading_of_1929                   ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_M_L                                    ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_B_L                                    ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_C                                      ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Class_H_B_B_or_Bo_Bo_Type              ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Class_H_C_C_or_Co_Co_Type              ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Class_H_Steam_Zf_1                     ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Class_H_Diesel_Electric                ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Class_A_B_B_or_Bo_Bo_Type              ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Class_A_C_C_or_Co_Co_Type              ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Class_A_Diesel_Mech_Elec               ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Class_A_Diesel_Mech_Elec_Articulated   ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Class_A_DRG_No_CSO_C_873               ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Class_B_B_B_or_Bo_Bo_Type              ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Class_B_Steam_Engine_Tank              ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Class_B_Steam_Engine_Tender            ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Class_B_Diesel_Electric                ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Train_Formation_No_1                   ) { bPointLoad = TRUE;  bDistLoad = FALSE; }
	else if(strSelVehicle == SELNAME_Train_Formation_No_2                   ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Train_Formation_No_3                   ) { bPointLoad = TRUE;  bDistLoad = FALSE; }
	else if(strSelVehicle == SELNAME_Train_Formation_No_4                   ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Train_Formation_No_5                   ) { bPointLoad = TRUE;  bDistLoad = FALSE; }
	else if(strSelVehicle == SELNAME_Train_Formation_No_6                   ) { bPointLoad = TRUE;  bDistLoad = FALSE; }
	else if(strSelVehicle == SELNAME_Train_Formation_No_7                   ) { bPointLoad = TRUE;  bDistLoad = FALSE; }
	else if(strSelVehicle == SELNAME_Train_Formation_No_8                   ) { bPointLoad = TRUE;  bDistLoad = FALSE; }
	else if(strSelVehicle == SELNAME_Train_Formation_No_9                   ) { bPointLoad = TRUE;  bDistLoad = FALSE; }
	else if(strSelVehicle == SELNAME_Train_Formation_No_10                  ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Train_Formation_No_11                  ) { bPointLoad = TRUE;  bDistLoad = FALSE; }
	else if(strSelVehicle == SELNAME_Train_Formation_No_12                  ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Train_Formation_No_13                  ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Train_Formation_No_14                  ) { bPointLoad = FALSE; bDistLoad = TRUE;  }
	else if(strSelVehicle == SELNAME_Train_Formation_No_15                  ) { bPointLoad = TRUE;  bDistLoad = FALSE; }
	else if(strSelVehicle == SELNAME_Train_Formation_No_16                  ) { bPointLoad = TRUE;  bDistLoad = FALSE; }
	else if(strSelVehicle == SELNAME_Train_Formation_No_17                  ) { bPointLoad = TRUE;  bDistLoad = FALSE; }
}																																						  

BOOL CCMMvhlItemStdIndiaDlg::ApplyOrOK()
{
	if(!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if(m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else          bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);
	if(!bSuccess) return FALSE;
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdIndiaDlg message handlers

BOOL CCMMvhlItemStdIndiaDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
 	if(!m_bModify)
 	{
 		m_Data.Initialize();
 		m_Data.bStandard = TRUE;    
 		m_Data.VehicleTypeName = _T("Class A");
 		m_Data.nStandardCode = D_MVHL_IRC6_2000;
		m_Data.dFootway = M_InitValueCurUnit( 0.5, TON, M, D_UNITSYS_BASE_STRESS);
		m_Data.dFootwayWidth = M_InitValueCurUnit( 3.0, TON, M, D_UNITSYS_BASE_LENGTH);
		m_Data.dSpanLength = M_InitValueCurUnit( 7.5, TON, M, D_UNITSYS_BASE_LENGTH);

		m_Data.dMinSpacing = M_InitValueCurUnit(20.0, TON, M, D_UNITSYS_BASE_LENGTH);
 	}

	InitControl();
	InitUnit();
	InitCombo();

	SetHeaderTitle();
		
	Data2Dlg();

	AlignControl();
	ControlsShowHide();
	OnCmdFactorTypeRdo();
	
	if(m_bModify)	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemStdIndiaDlg::OnOK() 
{
	if (!ApplyOrOK()) return;	
	CDialogMove::OnOK();
}

void CCMMvhlItemStdIndiaDlg::OnCmdMvhlCodeCmb() 
{
	ChangeCodeCombo();

	m_Data.VehicleTypeName = GetVehicleTypeStr();
	ChangeVehlTypeCombo();

	ChangeData();
	ChangeText();

	AlignControl();
	ControlsShowHide();
}

void CCMMvhlItemStdIndiaDlg::OnCmdMvhlTypeCmb() 
{
	m_Data.VehicleTypeName = GetVehicleTypeStr();
	ChangeVehlTypeCombo();

	ChangeData();
	ChangeText();

	AlignControl();
	ControlsShowHide();
}

void CCMMvhlItemStdIndiaDlg::OnCmdMvhlSelVehlCmb() 
{
	ChangeData();
	ChangeText();

	AlignControl();
	ControlsShowHide();
}

void CCMMvhlItemStdIndiaDlg::OnCmdMvhlApply() 
{
	ApplyOrOK();
}

void CCMMvhlItemStdIndiaDlg::OnCmdFactorTypeRdo()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_WG_CMD_IRC_ROUGHNESS_EDT)->EnableWindow(m_nFactorType == 2);
}

typedef std::pair<CString, CString> pair_cstring;
template <std::size_t N> using array_pair_cstring = std::array<pair_cstring, N>;

template <std::size_t N>
CString ConvStrRaw2Trans(const array_pair_cstring<N>& aPairStr,
	const CString& strRaw)
{
	for (const auto& strPair : aPairStr)
	{
		const auto& strTarget = strPair.first;
		if (strRaw == strTarget)
		{
			return strPair.second;
		}
	}

	return strRaw;
}

template <std::size_t N>
CString ConvStrTrans2Raw(const array_pair_cstring<N>& aPairStr,
	const CString& strTrans)
{
	for (const auto& strPair : aPairStr)
	{
		const auto& strTarget = strPair.second;
		if (strTrans == strTarget)
		{
			return strPair.first;
		}
	}

	return strTrans;
}

#define __static_caVehicleLoadType static const array_pair_cstring<12> caVehicleLoadType = \
{ \
    std::make_pair(_T("Class A"),                             _LS(IDS_WG_CMD_India_Class_A)), \
    std::make_pair(_T("Class B"),                             _LS(IDS_WG_CMD_India_Class_B)), \
    std::make_pair(_T("Class 70R"),                           _LS(IDS_WG_CMD_India_Class_70R)), \
    std::make_pair(_T("Class 40R"),                           _LS(IDS_WG_CMD_India_Class_40R)), \
    std::make_pair(_T("Class AA"),                            _LS(IDS_WG_CMD_India_Class_AA)), \
    std::make_pair(_T("Footway"),                             _LS(IDS_WG_CMD_India_Footway)), \
    std::make_pair(_T("Fatigue Vehicle"),                     _LS(IDS_WG_CMD_India_Fatigue_Vehicle)), \
    std::make_pair(_T("Broad Gauge-1676mm"),                  _LS(IDS_WG_CMD_India_Broad_Gauge_1676mm)), \
    std::make_pair(_T("Metre Gauge-1000mm"),                  _LS(IDS_WG_CMD_India_Metre_Gauge_1000mm)), \
    std::make_pair(_T("Narrow Gauge-762mm"),                  _LS(IDS_WG_CMD_India_Narrow_Gauge_762mm)), \
    std::make_pair(_T("Heavy Mineral Loadings"),              _LS(IDS_WG_CMD_Heavy_Mineral_Loadings)), \
    std::make_pair(_T("Footbridge & Footpath"),              _LS(IDS_WG_CMD_Footbridge_Footpath)), \
}; \

CString CCMMvhlItemStdIndiaDlg::ConvVehicleTypeStrRaw2Trans(const CString& strRaw) const
{
	__static_caVehicleLoadType;
	return ConvStrRaw2Trans(caVehicleLoadType, strRaw);
}

CString CCMMvhlItemStdIndiaDlg::ConvVehicleTypeStrTrans2Raw(const CString& strTrans) const
{
	__static_caVehicleLoadType;
	return ConvStrTrans2Raw(caVehicleLoadType, strTrans);
}

#undef __static_caVehicleLoadType

#define __static_caSelVehicleLoadType static const array_pair_cstring<54> caSelVehicleLoadType = \
{ \
    std::make_pair(SELNAME_Modified_B_G_Loading_1987_1          ,  _LS(IDS_WG_CMD_India_Modified_B_G_Loading_1987_1)), \
    std::make_pair(SELNAME_Modified_B_G_Loading_1987_2          ,  _LS(IDS_WG_CMD_India_Modified_B_G_Loading_1987_2)), \
    std::make_pair(SELNAME_B_G_Standard_Loading_1926_M_L        ,  _LS(IDS_WG_CMD_India_B_G_Standard_Loading_1926_M_L)), \
    std::make_pair(SELNAME_B_G_Standard_Loading_1926_B_L        ,  _LS(IDS_WG_CMD_India_B_G_Standard_Loading_1926_B_L)), \
    std::make_pair(SELNAME_Revised_B_G_Loading_1975_WG1_WG1     ,  _LS(IDS_WG_CMD_India_Revised_B_G_Loading_1975_WG1_WG1)), \
    std::make_pair(SELNAME_Revised_B_G_Loading_1975_WAM4A_WAM4A ,  _LS(IDS_WG_CMD_India_Revised_B_G_Loading_1975_WAM4A_WAM4A)), \
    std::make_pair(SELNAME_Revised_B_G_Loading_1975_Bo_Bo_Bo_Bo ,  _LS(IDS_WG_CMD_India_Revised_B_G_Loading_1975_Bo_Bo_Bo_Bo)), \
    std::make_pair(SELNAME_Revised_B_G_Loading_1975_WAM4A       ,  _LS(IDS_WG_CMD_India_Revised_B_G_Loading_1975_WAM4A)), \
    std::make_pair(SELNAME_Revised_B_G_Loading_1975_WAM4A_WAM4  ,  _LS(IDS_WG_CMD_India_Revised_B_G_Loading_1975_WAM4A_WAM4)), \
    std::make_pair(SELNAME_Revised_B_G_Loading_1975_WAM4A_WDM2  ,  _LS(IDS_WG_CMD_India_Revised_B_G_Loading_1975_WAM4A_WDM2)), \
    std::make_pair(SELNAME_25t_Loading_2008_Combination_1       ,  _LS(IDS_WG_CMD_India_25t_Loading_2008_Combination_1)), \
    std::make_pair(SELNAME_25t_Loading_2008_Combination_2       ,  _LS(IDS_WG_CMD_India_25t_Loading_2008_Combination_2)), \
    std::make_pair(SELNAME_25t_Loading_2008_Combination_3       ,  _LS(IDS_WG_CMD_India_25t_Loading_2008_Combination_3)), \
    std::make_pair(SELNAME_25t_Loading_2008_Combination_4       ,  _LS(IDS_WG_CMD_India_25t_Loading_2008_Combination_4)), \
    std::make_pair(SELNAME_25t_Loading_2008_Combination_5       ,  _LS(IDS_WG_CMD_India_25t_Loading_2008_Combination_5)), \
    std::make_pair(SELNAME_DFC_Loading_Combination_1            ,  _LS(IDS_WG_CMD_India_DFC_Loading_Combination_1)), \
    std::make_pair(SELNAME_DFC_Loading_Combination_2            ,  _LS(IDS_WG_CMD_India_DFC_Loading_Combination_2)), \
    std::make_pair(SELNAME_DFC_Loading_Combination_3            ,  _LS(IDS_WG_CMD_India_DFC_Loading_Combination_3)), \
    std::make_pair(SELNAME_DFC_Loading_Combination_4            ,  _LS(IDS_WG_CMD_India_DFC_Loading_Combination_4)), \
    std::make_pair(SELNAME_DFC_Loading_Combination_5            ,  _LS(IDS_WG_CMD_India_DFC_Loading_Combination_5)), \
    std::make_pair(SELNAME_2_Co_Co_Locomotives                  ,  _LS(IDS_WG_CMD_India_2_Co_Co_Locomotives)), \
    std::make_pair(SELNAME_2_Bo_Bo_Locomotives                  ,  _LS(IDS_WG_CMD_India_2_Bo_Bo_Locomotives)), \
    std::make_pair(SELNAME_MGML_Loading_of_1929                 ,  _LS(IDS_WG_CMD_India_MGML_Loading_of_1929)), \
    std::make_pair(SELNAME_Class_H_B_B_or_Bo_Bo_Type            ,  _LS(IDS_WG_CMD_India_Class_H_B_B_or_Bo_Bo_Type)), \
    std::make_pair(SELNAME_Class_H_C_C_or_Co_Co_Type            ,  _LS(IDS_WG_CMD_India_Class_H_C_C_or_Co_Co_Type)), \
    std::make_pair(SELNAME_Class_H_Steam_Zf_1                   ,  _LS(IDS_WG_CMD_India_Class_H_Steam_Zf_1)), \
    std::make_pair(SELNAME_Class_H_Diesel_Electric              ,  _LS(IDS_WG_CMD_India_Class_H_Diesel_Electric)), \
    std::make_pair(SELNAME_Class_A_B_B_or_Bo_Bo_Type            ,  _LS(IDS_WG_CMD_India_Class_A_B_B_or_Bo_Bo_Type)), \
    std::make_pair(SELNAME_Class_A_C_C_or_Co_Co_Type            ,  _LS(IDS_WG_CMD_India_Class_A_C_C_or_Co_Co_Type)), \
    std::make_pair(SELNAME_Class_A_Diesel_Mech_Elec             ,  _LS(IDS_WG_CMD_India_Class_A_Diesel_Mech_Elec)), \
    std::make_pair(SELNAME_Class_A_Diesel_Mech_Elec_Articulated ,  _LS(IDS_WG_CMD_India_Class_A_Diesel_Mech_Elec_Articulated)), \
    std::make_pair(SELNAME_Class_A_DRG_No_CSO_C_873             ,  _LS(IDS_WG_CMD_India_Class_A_DRG_No_CSO_C_873)), \
    std::make_pair(SELNAME_Class_B_B_B_or_Bo_Bo_Type            ,  _LS(IDS_WG_CMD_India_Class_B_B_B_or_Bo_Bo_Type)), \
    std::make_pair(SELNAME_Class_B_Steam_Engine_Tank            ,  _LS(IDS_WG_CMD_India_Class_B_Steam_Engine_Tank)), \
    std::make_pair(SELNAME_Class_B_Steam_Engine_Tender          ,  _LS(IDS_WG_CMD_India_Class_B_Steam_Engine_Tender)), \
    std::make_pair(SELNAME_Class_B_Diesel_Electric              ,  _LS(IDS_WG_CMD_India_Class_B_Diesel_Electric)), \
    std::make_pair(SELNAME_Train_Formation_No_1                 ,  _LS(IDS_WG_CMD_India_Train_Formation_No_1)), \
    std::make_pair(SELNAME_Train_Formation_No_2                 ,  _LS(IDS_WG_CMD_India_Train_Formation_No_2)), \
    std::make_pair(SELNAME_Train_Formation_No_3                 ,  _LS(IDS_WG_CMD_India_Train_Formation_No_3)), \
    std::make_pair(SELNAME_Train_Formation_No_4                 ,  _LS(IDS_WG_CMD_India_Train_Formation_No_4)), \
    std::make_pair(SELNAME_Train_Formation_No_5                 ,  _LS(IDS_WG_CMD_India_Train_Formation_No_5)), \
    std::make_pair(SELNAME_Train_Formation_No_6                 ,  _LS(IDS_WG_CMD_India_Train_Formation_No_6)), \
    std::make_pair(SELNAME_Train_Formation_No_7                 ,  _LS(IDS_WG_CMD_India_Train_Formation_No_7)), \
    std::make_pair(SELNAME_Train_Formation_No_8                 ,  _LS(IDS_WG_CMD_India_Train_Formation_No_8)), \
    std::make_pair(SELNAME_Train_Formation_No_9                 ,  _LS(IDS_WG_CMD_India_Train_Formation_No_9)), \
    std::make_pair(SELNAME_Train_Formation_No_10                ,  _LS(IDS_WG_CMD_India_Train_Formation_No_10)), \
    std::make_pair(SELNAME_Train_Formation_No_11                ,  _LS(IDS_WG_CMD_India_Train_Formation_No_11)), \
    std::make_pair(SELNAME_Train_Formation_No_12                ,  _LS(IDS_WG_CMD_India_Train_Formation_No_12)), \
    std::make_pair(SELNAME_Train_Formation_No_13                ,  _LS(IDS_WG_CMD_India_Train_Formation_No_13)), \
    std::make_pair(SELNAME_Train_Formation_No_14                ,  _LS(IDS_WG_CMD_India_Train_Formation_No_14)), \
    std::make_pair(SELNAME_Train_Formation_No_15                ,  _LS(IDS_WG_CMD_India_Train_Formation_No_15)), \
    std::make_pair(SELNAME_Train_Formation_No_16                ,  _LS(IDS_WG_CMD_India_Train_Formation_No_16)), \
    std::make_pair(SELNAME_Train_Formation_No_17                ,  _LS(IDS_WG_CMD_India_Train_Formation_No_17)), \
    std::make_pair(SELNAME_Footbridge_Footpath                  ,  _LS(IDS_WG_CMD_India_Footbridge_Footpath)), \
}; \

CString CCMMvhlItemStdIndiaDlg::ConvSelVihicleStrRaw2Trans(const CString& strRaw) const
{
	__static_caSelVehicleLoadType;
	return ConvStrRaw2Trans(caSelVehicleLoadType, strRaw);
}

CString CCMMvhlItemStdIndiaDlg::ConvSelVihicleStrTrans2Raw(const CString& strTrans) const
{
	__static_caSelVehicleLoadType;
	return ConvStrTrans2Raw(caSelVehicleLoadType, strTrans);
}

#undef __static_caSelVehicleLoadType

#define __static_caVehicleLoadName static const array_pair_cstring<66> caVehicleLoadName = \
{ \
    std::make_pair(_T("Class A"),                             _LS(IDS_WG_CMD_India_Class_A)), \
    std::make_pair(_T("Class B"),                             _LS(IDS_WG_CMD_India_Class_B)), \
    std::make_pair(_T("Class 70R"),                           _LS(IDS_WG_CMD_India_Class_70R)), \
    std::make_pair(_T("Class 40R"),                           _LS(IDS_WG_CMD_India_Class_40R)), \
    std::make_pair(_T("Class AA"),                            _LS(IDS_WG_CMD_India_Class_AA)), \
    std::make_pair(_T("Footway"),                             _LS(IDS_WG_CMD_India_Footway)), \
    std::make_pair(_T("Fatigue Vehicle"),                     _LS(IDS_WG_CMD_India_Fatigue_Vehicle)), \
    std::make_pair(_T("Broad Gauge-1676mm"),                  _LS(IDS_WG_CMD_India_Broad_Gauge_1676mm)), \
    std::make_pair(_T("Metre Gauge-1000mm"),                  _LS(IDS_WG_CMD_India_Metre_Gauge_1000mm)), \
    std::make_pair(_T("Narrow Gauge-762mm"),                  _LS(IDS_WG_CMD_India_Narrow_Gauge_762mm)), \
    std::make_pair(_T("Heavy Mineral Loadings"),              _LS(IDS_WG_CMD_Heavy_Mineral_Loadings)), \
    std::make_pair(_T("Footbridge & Footpath"),              _LS(IDS_WG_CMD_Footbridge_Footpath)), \
    std::make_pair(SELNAME_Modified_B_G_Loading_1987_1          ,  _LS(IDS_WG_CMD_India_Modified_B_G_Loading_1987_1)), \
    std::make_pair(SELNAME_Modified_B_G_Loading_1987_2          ,  _LS(IDS_WG_CMD_India_Modified_B_G_Loading_1987_2)), \
    std::make_pair(SELNAME_B_G_Standard_Loading_1926_M_L        ,  _LS(IDS_WG_CMD_India_B_G_Standard_Loading_1926_M_L)), \
    std::make_pair(SELNAME_B_G_Standard_Loading_1926_B_L        ,  _LS(IDS_WG_CMD_India_B_G_Standard_Loading_1926_B_L)), \
    std::make_pair(SELNAME_Revised_B_G_Loading_1975_WG1_WG1     ,  _LS(IDS_WG_CMD_India_Revised_B_G_Loading_1975_WG1_WG1)), \
    std::make_pair(SELNAME_Revised_B_G_Loading_1975_WAM4A_WAM4A ,  _LS(IDS_WG_CMD_India_Revised_B_G_Loading_1975_WAM4A_WAM4A)), \
    std::make_pair(SELNAME_Revised_B_G_Loading_1975_Bo_Bo_Bo_Bo ,  _LS(IDS_WG_CMD_India_Revised_B_G_Loading_1975_Bo_Bo_Bo_Bo)), \
    std::make_pair(SELNAME_Revised_B_G_Loading_1975_WAM4A       ,  _LS(IDS_WG_CMD_India_Revised_B_G_Loading_1975_WAM4A)), \
    std::make_pair(SELNAME_Revised_B_G_Loading_1975_WAM4A_WAM4  ,  _LS(IDS_WG_CMD_India_Revised_B_G_Loading_1975_WAM4A_WAM4)), \
    std::make_pair(SELNAME_Revised_B_G_Loading_1975_WAM4A_WDM2  ,  _LS(IDS_WG_CMD_India_Revised_B_G_Loading_1975_WAM4A_WDM2)), \
    std::make_pair(SELNAME_25t_Loading_2008_Combination_1       ,  _LS(IDS_WG_CMD_India_25t_Loading_2008_Combination_1)), \
    std::make_pair(SELNAME_25t_Loading_2008_Combination_2       ,  _LS(IDS_WG_CMD_India_25t_Loading_2008_Combination_2)), \
    std::make_pair(SELNAME_25t_Loading_2008_Combination_3       ,  _LS(IDS_WG_CMD_India_25t_Loading_2008_Combination_3)), \
    std::make_pair(SELNAME_25t_Loading_2008_Combination_4       ,  _LS(IDS_WG_CMD_India_25t_Loading_2008_Combination_4)), \
    std::make_pair(SELNAME_25t_Loading_2008_Combination_5       ,  _LS(IDS_WG_CMD_India_25t_Loading_2008_Combination_5)), \
    std::make_pair(SELNAME_DFC_Loading_Combination_1            ,  _LS(IDS_WG_CMD_India_DFC_Loading_Combination_1)), \
    std::make_pair(SELNAME_DFC_Loading_Combination_2            ,  _LS(IDS_WG_CMD_India_DFC_Loading_Combination_2)), \
    std::make_pair(SELNAME_DFC_Loading_Combination_3            ,  _LS(IDS_WG_CMD_India_DFC_Loading_Combination_3)), \
    std::make_pair(SELNAME_DFC_Loading_Combination_4            ,  _LS(IDS_WG_CMD_India_DFC_Loading_Combination_4)), \
    std::make_pair(SELNAME_DFC_Loading_Combination_5            ,  _LS(IDS_WG_CMD_India_DFC_Loading_Combination_5)), \
    std::make_pair(SELNAME_2_Co_Co_Locomotives                  ,  _LS(IDS_WG_CMD_India_2_Co_Co_Locomotives)), \
    std::make_pair(SELNAME_2_Bo_Bo_Locomotives                  ,  _LS(IDS_WG_CMD_India_2_Bo_Bo_Locomotives)), \
    std::make_pair(SELNAME_MGML_Loading_of_1929                 ,  _LS(IDS_WG_CMD_India_MGML_Loading_of_1929)), \
    std::make_pair(SELNAME_Class_H_B_B_or_Bo_Bo_Type            ,  _LS(IDS_WG_CMD_India_Class_H_B_B_or_Bo_Bo_Type)), \
    std::make_pair(SELNAME_Class_H_C_C_or_Co_Co_Type            ,  _LS(IDS_WG_CMD_India_Class_H_C_C_or_Co_Co_Type)), \
    std::make_pair(SELNAME_Class_H_Steam_Zf_1                   ,  _LS(IDS_WG_CMD_India_Class_H_Steam_Zf_1)), \
    std::make_pair(SELNAME_Class_H_Diesel_Electric              ,  _LS(IDS_WG_CMD_India_Class_H_Diesel_Electric)), \
    std::make_pair(SELNAME_Class_A_B_B_or_Bo_Bo_Type            ,  _LS(IDS_WG_CMD_India_Class_A_B_B_or_Bo_Bo_Type)), \
    std::make_pair(SELNAME_Class_A_C_C_or_Co_Co_Type            ,  _LS(IDS_WG_CMD_India_Class_A_C_C_or_Co_Co_Type)), \
    std::make_pair(SELNAME_Class_A_Diesel_Mech_Elec             ,  _LS(IDS_WG_CMD_India_Class_A_Diesel_Mech_Elec)), \
    std::make_pair(SELNAME_Class_A_Diesel_Mech_Elec_Articulated ,  _LS(IDS_WG_CMD_India_Class_A_Diesel_Mech_Elec_Articulated)), \
    std::make_pair(SELNAME_Class_A_DRG_No_CSO_C_873             ,  _LS(IDS_WG_CMD_India_Class_A_DRG_No_CSO_C_873)), \
    std::make_pair(SELNAME_Class_B_B_B_or_Bo_Bo_Type            ,  _LS(IDS_WG_CMD_India_Class_B_B_B_or_Bo_Bo_Type)), \
    std::make_pair(SELNAME_Class_B_Steam_Engine_Tank            ,  _LS(IDS_WG_CMD_India_Class_B_Steam_Engine_Tank)), \
    std::make_pair(SELNAME_Class_B_Steam_Engine_Tender          ,  _LS(IDS_WG_CMD_India_Class_B_Steam_Engine_Tender)), \
    std::make_pair(SELNAME_Class_B_Diesel_Electric              ,  _LS(IDS_WG_CMD_India_Class_B_Diesel_Electric)), \
    std::make_pair(SELNAME_Train_Formation_No_1                 ,  _LS(IDS_WG_CMD_India_Train_Formation_No_1)), \
    std::make_pair(SELNAME_Train_Formation_No_2                 ,  _LS(IDS_WG_CMD_India_Train_Formation_No_2)), \
    std::make_pair(SELNAME_Train_Formation_No_3                 ,  _LS(IDS_WG_CMD_India_Train_Formation_No_3)), \
    std::make_pair(SELNAME_Train_Formation_No_4                 ,  _LS(IDS_WG_CMD_India_Train_Formation_No_4)), \
    std::make_pair(SELNAME_Train_Formation_No_5                 ,  _LS(IDS_WG_CMD_India_Train_Formation_No_5)), \
    std::make_pair(SELNAME_Train_Formation_No_6                 ,  _LS(IDS_WG_CMD_India_Train_Formation_No_6)), \
    std::make_pair(SELNAME_Train_Formation_No_7                 ,  _LS(IDS_WG_CMD_India_Train_Formation_No_7)), \
    std::make_pair(SELNAME_Train_Formation_No_8                 ,  _LS(IDS_WG_CMD_India_Train_Formation_No_8)), \
    std::make_pair(SELNAME_Train_Formation_No_9                 ,  _LS(IDS_WG_CMD_India_Train_Formation_No_9)), \
    std::make_pair(SELNAME_Train_Formation_No_10                ,  _LS(IDS_WG_CMD_India_Train_Formation_No_10)), \
    std::make_pair(SELNAME_Train_Formation_No_11                ,  _LS(IDS_WG_CMD_India_Train_Formation_No_11)), \
    std::make_pair(SELNAME_Train_Formation_No_12                ,  _LS(IDS_WG_CMD_India_Train_Formation_No_12)), \
    std::make_pair(SELNAME_Train_Formation_No_13                ,  _LS(IDS_WG_CMD_India_Train_Formation_No_13)), \
    std::make_pair(SELNAME_Train_Formation_No_14                ,  _LS(IDS_WG_CMD_India_Train_Formation_No_14)), \
    std::make_pair(SELNAME_Train_Formation_No_15                ,  _LS(IDS_WG_CMD_India_Train_Formation_No_15)), \
    std::make_pair(SELNAME_Train_Formation_No_16                ,  _LS(IDS_WG_CMD_India_Train_Formation_No_16)), \
    std::make_pair(SELNAME_Train_Formation_No_17                ,  _LS(IDS_WG_CMD_India_Train_Formation_No_17)), \
    std::make_pair(SELNAME_Footbridge_Footpath                  ,  _LS(IDS_WG_CMD_India_Footbridge_Footpath)), \
}; \

CString CCMMvhlItemStdIndiaDlg::ConvVehicleNameStrRaw2Trans(const CString& strRaw) const
{
	__static_caVehicleLoadName;
	return ConvStrRaw2Trans(caVehicleLoadName, strRaw);
}

CString CCMMvhlItemStdIndiaDlg::ConvVehicleNameStrTrans2Raw(const CString& strTrans) const
{
	__static_caVehicleLoadName;
	return ConvStrTrans2Raw(caVehicleLoadName, strTrans);
}

#undef __static_caVehicleLoadName

/*
 * 지금 콤보박스를 사용하는 곳이 String으로 비교를 하고 있어
 * 일괄적으로 적용하기에는 리스크가 존재해서..
 * 번역이 필요한 String만 따로 추출해서 변환시켜주는 역할ㅠㅠ
 */
CString CCMMvhlItemStdIndiaDlg::GetVehicleTypeStr() const
{
	CString CurTransStr;
	m_cmbVehicleType.GetWindowText(CurTransStr);

	return ConvVehicleTypeStrTrans2Raw(CurTransStr);
}

CString CCMMvhlItemStdIndiaDlg::GetSelVihicleStr() const
{
	CString CurTransStr;
	m_cmbSelVehicle.GetWindowText(CurTransStr);
	return ConvSelVihicleStrTrans2Raw(CurTransStr);
}
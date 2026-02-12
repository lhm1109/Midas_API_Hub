// CMMvhlItemUsrIndiaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUsrIndiaDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MvhlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "CMMvhlItemUsrPermitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IRC_LIST   0
#define IRS1_LIST  1
#define IRS2_LIST  2

#define D_NUMLOAD_MAIN     50
#define D_NUMLOAD_GONDOLA  20

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrIndiaDlg dialog
#define COLCOUNT 3

CCMMvhlItemUsrIndiaDlg::CCMMvhlItemUsrIndiaDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemUsrIndiaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemUsrIndiaDlg)	
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();
	m_Data.Initialize();
	m_bModify = FALSE;

	m_nIRCLoadType = 0;
	m_nIRSLoadType = 0;
	m_nTrainSubType = 1;
	m_nGondolaPos = 2;
	m_nTrainPos = 2;

	m_bPermitLoadCall = FALSE;
}

CCMMvhlItemUsrIndiaDlg::~CCMMvhlItemUsrIndiaDlg()
{
}

void CCMMvhlItemUsrIndiaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemUsrIndiaDlg)
	DDX_Control(pDX, IDC_CMD_MVHL_CODE_CMB,                    m_cmbCodeName);
	DDX_Radio  (pDX, IDC_CMD_MVHL_WHEELED_TRACKED_RDO,         m_nIRCLoadType);
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_NAME_EDT,               m_edtVehicleName);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_WHEELED_P_EDT,           m_edtIRC_P);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_WHEELED_D_EDT,           m_edtIRC_D);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_WHEELED_LST,             m_lstIRC);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_WHEELED_DD1_EDT,         m_edtIRCdD1);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_WHEELED_DD1_UNT,         m_untIRCdD1);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_WHEELED_DD2_EDT,         m_edtIRCdD2);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_WHEELED_DD2_UNT,         m_untIRCdD2);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_TRACK_P_EDT,             m_edtIRCP);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_TRACK_P_UNT,             m_untIRCP);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_TRACK_D_EDT,             m_edtIRCD);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_TRACK_D_UNT,             m_untIRCD);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_BOGIE_PB_EDT,            m_edtIRCPb);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_BOGIE_PB_UNT,            m_untIRCPb);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_BOGIE_DB_EDT,            m_edtIRCDb);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_BOGIE_DB_UNT,            m_untIRCDb);
	DDX_Control(pDX, IDC_CMD_MVHL_NOSE_TAIL_DIST_EDT,          m_edtNoseTailDist);
	DDX_Control(pDX, IDC_CMD_MVHL_NOSE_TAIL_DIST_UNT,          m_untNoseTailDist);
	DDX_Radio  (pDX, IDC_CMD_MVHL_BG_RDO,                      m_nIRSLoadType);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_P1_EDT,                  m_edtIRS1_P);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_D1_EDT,                  m_edtIRS1_D);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_PTLIST1,                 m_lstIRS1);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_DD1_EDT,                 m_edtIRSdD12);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_DD1_UNT,                 m_untIRSdD12);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_DD2_EDT,                 m_edtIRSdD22);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_DD2_UNT,                 m_untIRSdD22);
	DDX_Radio  (pDX, IDC_CMD_MVHL_IRS_GONDOLA_RDO,             m_nTrainSubType);
	DDX_Radio  (pDX, IDC_CMD_MVHL_IRS_PTLIST2_L_RDO,           m_nGondolaPos);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_P2_EDT,                  m_edtIRS2_P);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_D2_EDT,                  m_edtIRS2_D);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_PTLIST2,                 m_lstIRS2);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_DD3_EDT,                 m_edtIRSdD3);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_DD3_UNT,                 m_untIRSdD3);
	DDX_Radio  (pDX, IDC_CMD_MVHL_IRS_TRAIN_L_RDO,             m_nTrainPos);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_DW_EDT,                  m_edtIRSdW);
	DDX_Control(pDX, IDC_CMD_MVHL_IRS_DW_UNT,                  m_untIRSdW);
	DDX_Control(pDX, IDC_CMD_MVHL_TRACTIVE_EDT,                m_edtTractive);
	DDX_Control(pDX, IDC_CMD_MVHL_TRACTIVE_UNT,                m_untTractive);
	DDX_Control(pDX, IDC_CMD_MVHL_BRAKE_LOCO_RATIO_EDT,        m_edtBrakeLOCORatio);
	DDX_Control(pDX, IDC_CMD_MVHL_BRAKE_TRAIN_RATIO_EDT,       m_edtBrakeTrainRatio);
	DDX_Control(pDX, IDC_CMD_MVHL_BRAKE_LOCO_EDT,              m_edtBrakeLOCO);
	DDX_Control(pDX, IDC_CMD_MVHL_BRAKE_LOCO_UNT,              m_untBrakeLOCO);
	DDX_Control(pDX, IDC_CMD_MVHL_NOSE_TAIL_DIST_WHEELED_EDT,  m_edtNoseTailDistW);
	DDX_Control(pDX, IDC_CMD_MVHL_NOSE_TAIL_DIST_WHEELED_UNT,  m_untNoseTailDistW);
	DDX_Control(pDX, IDC_CMD_MVHL_NOSE_TAIL_DIST_TRACKED_EDT,  m_edtNoseTailDistT);
	DDX_Control(pDX, IDC_CMD_MVHL_NOSE_TAIL_DIST_TRACKED_UNT,  m_untNoseTailDistT);
	DDX_Control(pDX, IDC_CMD_MVHL_PICTURE, m_wndPicture);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMMvhlItemUsrIndiaDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemUsrIndiaDlg)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_CODE_CMB,                    OnCmdMvhlCodeCmb)
	ON_BN_CLICKED(IDC_CMD_MVHL_WHEELED_TRACKED_RDO,		         OnCmdMvhlIRCLoadTypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_WHEELED_RDO,       		         OnCmdMvhlIRCLoadTypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_PERMIT_RDO,		                 OnCmdMvhlIRCLoadTypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_IRC_WHEELED_ADD_BTN,		         OnCmdMvhlIRCAddBtn)
	ON_BN_CLICKED(IDC_CMD_MVHL_IRC_WHEELED_INS_BTN,		         OnCmdMvhlIRCInsBtn)
	ON_BN_CLICKED(IDC_CMD_MVHL_IRC_WHEELED_MOD_BTN,		         OnCmdMvhlIRCModBtn)
	ON_BN_CLICKED(IDC_CMD_MVHL_IRC_WHEELED_DEL_BTN,		         OnCmdMvhlIRCDelBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_MVHL_IRC_WHEELED_LST,   OnCmdMvhlIRCItemChange)
	ON_BN_CLICKED(IDC_CMD_MVHL_BG_RDO,              	         OnCmdMvhlIRSLoadTypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_MG_RDO,              	         OnCmdMvhlIRSLoadTypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_NG_RDO,              	         OnCmdMvhlIRSLoadTypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_HML_RDO,              	         OnCmdMvhlIRSLoadTypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_IRS_ADD1_BTN,                   OnCmdMvhlIRS1AddBtn)
	ON_BN_CLICKED(IDC_CMD_MVHL_IRS_INS1_BTN,                   OnCmdMvhlIRS1InsBtn)
	ON_BN_CLICKED(IDC_CMD_MVHL_IRS_MOD1_BTN,                   OnCmdMvhlIRS1ModBtn)
	ON_BN_CLICKED(IDC_CMD_MVHL_IRS_DEL1_BTN,                   OnCmdMvhlIRS1DelBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_MVHL_IRS_PTLIST1,       OnCmdMvhlIRS1ItemChange)
	ON_BN_CLICKED(IDC_CMD_MVHL_IRS_GONDOLA_RDO,                OnCmdMvhlIRSTrainSubTypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_IRS_TRAIN_RDO,                  OnCmdMvhlIRSTrainSubTypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_IRS_ADD2_BTN,                   OnCmdMvhlIRS2AddBtn)
	ON_BN_CLICKED(IDC_CMD_MVHL_IRS_INS2_BTN,                   OnCmdMvhlIRS2InsBtn)
	ON_BN_CLICKED(IDC_CMD_MVHL_IRS_MOD2_BTN,                   OnCmdMvhlIRS2ModBtn)
	ON_BN_CLICKED(IDC_CMD_MVHL_IRS_DEL2_BTN,                   OnCmdMvhlIRS2DelBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_MVHL_IRS_PTLIST2,       OnCmdMvhlIRS2ItemChange)
	ON_BN_CLICKED(IDC_CMD_APPLY,				                       OnCmdMvhlApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvhlItemUsrIndiaDlg::InitControl()
{
	m_aIRCLoadType.RemoveAll();
	m_aIRCLoadType.Add(IDC_CMD_MVHL_WHEELED_TRACKED_RDO);
	m_aIRCLoadType.Add(IDC_CMD_MVHL_WHEELED_RDO);
	m_aIRCLoadType.Add(IDC_CMD_MVHL_PERMIT_RDO);

	m_aIRSLoadType.RemoveAll();
	m_aIRSLoadType.Add(IDC_CMD_MVHL_BG_RDO);
	m_aIRSLoadType.Add(IDC_CMD_MVHL_MG_RDO);
	m_aIRSLoadType.Add(IDC_CMD_MVHL_NG_RDO);
	m_aIRSLoadType.Add(IDC_CMD_MVHL_HML_RDO);

	m_aWheel.RemoveAll();
	m_aWheel.Add(IDC_CMD_MVHL_IRC_WHEELED_GRP);
	m_aWheel.Add(IDC_CMD_MVHL_IRC_WHEELED_P_STC);
	m_aWheel.Add(IDC_CMD_MVHL_IRC_WHEELED_P_EDT);
	m_aWheel.Add(IDC_CMD_MVHL_IRC_WHEELED_D_STC);
	m_aWheel.Add(IDC_CMD_MVHL_IRC_WHEELED_D_EDT);
	m_aWheel.Add(IDC_CMD_MVHL_IRC_WHEELED_LST);
	m_aWheel.Add(IDC_CMD_MVHL_IRC_WHEELED_ADD_BTN);
	m_aWheel.Add(IDC_CMD_MVHL_IRC_WHEELED_INS_BTN);
	m_aWheel.Add(IDC_CMD_MVHL_IRC_WHEELED_MOD_BTN);
	m_aWheel.Add(IDC_CMD_MVHL_IRC_WHEELED_DEL_BTN);
	m_aWheel.Add(IDC_CMD_MVHL_IRC_WHEELED_DD1_STC);
	m_aWheel.Add(IDC_CMD_MVHL_IRC_WHEELED_DD1_EDT);
	m_aWheel.Add(IDC_CMD_MVHL_IRC_WHEELED_DD1_UNT);
	m_aWheel.Add(IDC_CMD_MVHL_IRC_WHEELED_DD2_STC);
	m_aWheel.Add(IDC_CMD_MVHL_IRC_WHEELED_DD2_EDT);
	m_aWheel.Add(IDC_CMD_MVHL_IRC_WHEELED_DD2_UNT);
	m_aWheel.Add(IDC_CMD_MVHL_IRC_WHEELED_GRP);
	m_aWheel.Add(IDC_CMD_MVHL_IRC_WHEELED_GRP);
	m_aWheel.Add(IDC_CMD_MVHL_IRC_WHEELED_GRP);

	m_aTrack.RemoveAll();
	m_aTrack.Add(IDC_CMD_MVHL_IRC_TRACK_GRP);
	m_aTrack.Add(IDC_CMD_MVHL_IRC_TRACK_P_STC);
	m_aTrack.Add(IDC_CMD_MVHL_IRC_TRACK_P_EDT);
	m_aTrack.Add(IDC_CMD_MVHL_IRC_TRACK_P_UNT);
	m_aTrack.Add(IDC_CMD_MVHL_IRC_TRACK_D_STC);
	m_aTrack.Add(IDC_CMD_MVHL_IRC_TRACK_D_EDT);
	m_aTrack.Add(IDC_CMD_MVHL_IRC_TRACK_D_UNT);

	m_aBogie.RemoveAll();
	m_aBogie.Add(IDC_CMD_MVHL_IRC_BOGIE_GRP);
	m_aBogie.Add(IDC_CMD_MVHL_IRC_BOGIE_PB_STC);
	m_aBogie.Add(IDC_CMD_MVHL_IRC_BOGIE_PB_EDT);
	m_aBogie.Add(IDC_CMD_MVHL_IRC_BOGIE_PB_UNT);
	m_aBogie.Add(IDC_CMD_MVHL_IRC_BOGIE_DB_STC);
	m_aBogie.Add(IDC_CMD_MVHL_IRC_BOGIE_DB_EDT);
	m_aBogie.Add(IDC_CMD_MVHL_IRC_BOGIE_DB_UNT);

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

	m_aGondola.RemoveAll();
	m_aGondola.Add(IDC_CMD_MVHL_IRS_PTLIST2_GRP);
	m_aGondola.Add(IDC_CMD_MVHL_IRS_PTLIST2_POS_GRP);
	m_aGondola.Add(IDC_CMD_MVHL_IRS_PTLIST2_L_RDO);
	m_aGondola.Add(IDC_CMD_MVHL_IRS_PTLIST2_R_RDO);
	m_aGondola.Add(IDC_CMD_MVHL_IRS_PTLIST2_LR_RDO);
	m_aGondola.Add(IDC_CMD_MVHL_IRS_P2_STC);
	m_aGondola.Add(IDC_CMD_MVHL_IRS_P2_EDT);
	m_aGondola.Add(IDC_CMD_MVHL_IRS_D2_STC);
	m_aGondola.Add(IDC_CMD_MVHL_IRS_D2_EDT);
	m_aGondola.Add(IDC_CMD_MVHL_IRS_PTLIST2);
	m_aGondola.Add(IDC_CMD_MVHL_IRS_ADD2_BTN);
	m_aGondola.Add(IDC_CMD_MVHL_IRS_INS2_BTN);
	m_aGondola.Add(IDC_CMD_MVHL_IRS_MOD2_BTN);
	m_aGondola.Add(IDC_CMD_MVHL_IRS_DEL2_BTN);
	m_aGondola.Add(IDC_CMD_MVHL_IRS_DD3_STC);
	m_aGondola.Add(IDC_CMD_MVHL_IRS_DD3_EDT);
	m_aGondola.Add(IDC_CMD_MVHL_IRS_DD3_UNT);

	m_aTrain.RemoveAll();
	m_aTrain.Add(IDC_CMD_MVHL_IRS_TRAIN_GRP);
	m_aTrain.Add(IDC_CMD_MVHL_IRS_TRAIN_POS_GRP);
	m_aTrain.Add(IDC_CMD_MVHL_IRS_TRAIN_L_RDO);
	m_aTrain.Add(IDC_CMD_MVHL_IRS_TRAIN_R_RDO);
	m_aTrain.Add(IDC_CMD_MVHL_IRS_TRAIN_LR_RDO);
	m_aTrain.Add(IDC_CMD_MVHL_IRS_DW_STC);
	m_aTrain.Add(IDC_CMD_MVHL_IRS_DW_EDT);
	m_aTrain.Add(IDC_CMD_MVHL_IRS_DW_UNT);
	
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

	m_aIRC.RemoveAll();
	for(int i=0; i<m_aWheel.GetSize();       i++) m_aIRC.Add(m_aWheel[i]);
	for(int i=0; i<m_aTrack.GetSize();       i++) m_aIRC.Add(m_aTrack[i]);
	for(int i=0; i<m_aBogie.GetSize();       i++) m_aIRC.Add(m_aBogie[i]);
	for(int i=0; i<m_aNoseTail.GetSize();    i++) m_aIRC.Add(m_aNoseTail[i]);
	for(int i=0; i<m_aNoseTail70R.GetSize(); i++) m_aIRC.Add(m_aNoseTail70R[i]);

	m_aIRS.RemoveAll();
	m_aIRS.Add(IDC_CMD_MVHL_IRS_PTLIST1_GRP);
	m_aIRS.Add(IDC_CMD_MVHL_IRS_P1_STC);
	m_aIRS.Add(IDC_CMD_MVHL_IRS_P1_EDT);
	m_aIRS.Add(IDC_CMD_MVHL_IRS_D1_STC);
	m_aIRS.Add(IDC_CMD_MVHL_IRS_D1_EDT);
	m_aIRS.Add(IDC_CMD_MVHL_IRS_PTLIST1);
	m_aIRS.Add(IDC_CMD_MVHL_IRS_ADD1_BTN);
	m_aIRS.Add(IDC_CMD_MVHL_IRS_INS1_BTN);
	m_aIRS.Add(IDC_CMD_MVHL_IRS_MOD1_BTN);
	m_aIRS.Add(IDC_CMD_MVHL_IRS_DEL1_BTN);
	m_aIRS.Add(IDC_CMD_MVHL_IRS_DD1_STC);
	m_aIRS.Add(IDC_CMD_MVHL_IRS_DD1_EDT);
	m_aIRS.Add(IDC_CMD_MVHL_IRS_DD1_UNT);
	m_aIRS.Add(IDC_CMD_MVHL_IRS_DD2_STC);
	m_aIRS.Add(IDC_CMD_MVHL_IRS_DD2_EDT);
	m_aIRS.Add(IDC_CMD_MVHL_IRS_DD2_UNT);
	m_aIRS.Add(IDC_CMD_MVHL_IRS_GONDOLA_RDO);
	m_aIRS.Add(IDC_CMD_MVHL_IRS_TRAIN_RDO);
	for(int i=0; i<m_aGondola.GetSize(); i++) m_aIRS.Add(m_aGondola[i]);
	for(int i=0; i<m_aTrain.GetSize();   i++) m_aIRS.Add(m_aTrain[i]);

	m_aOKCancel.RemoveAll();
	m_aOKCancel.Add(IDOK);
	m_aOKCancel.Add(IDCANCEL);
	m_aOKCancel.Add(IDC_CMD_APPLY);
}

void CCMMvhlItemUsrIndiaDlg::InitUnit()
{
	m_edtVehicleName.SetUnitType(D_UNITSYS_NONE);
	m_edtIRC_P.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtIRC_D.SetUnitType(D_UNITSYS_BASE_LENGTH);
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
	m_edtNoseTailDist.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dNoseTailDist);
	m_untNoseTailDist.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dNoseTailDist);	
	m_edtIRS1_P.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtIRS1_D.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtIRSdD12.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTrainD1);
	m_untIRSdD12.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTrainD1);
	m_edtIRSdD22.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTrainD2);
	m_untIRSdD22.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTrainD2);
	m_edtIRS2_P.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtIRS2_D.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtIRSdD3.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTrainD3);
	m_untIRSdD3.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTrainD3);
	m_edtIRSdW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dUniformLoad);
	m_untIRSdW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dUniformLoad);
	m_edtTractive.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTractive);
	m_untTractive.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dTractive);
	m_edtBrakeLOCORatio.SetUnitType(D_UNITSYS_NONE);
	m_edtBrakeTrainRatio.SetUnitType(D_UNITSYS_NONE);
	m_edtBrakeLOCO.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dBrakeLoco);
	m_untBrakeLOCO.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dBrakeLoco);
	m_edtNoseTailDistW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dNoseTailDist);
	m_untNoseTailDistW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dNoseTailDist);
	m_edtNoseTailDistT.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dNoseTailDist);
	m_untNoseTailDistT.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dNoseTailDist);
}

void CCMMvhlItemUsrIndiaDlg::InitCombo()
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

void CCMMvhlItemUsrIndiaDlg::AlignControl()
{
	UpdateData(TRUE);

	int nCodeType = m_cmbCodeName.GetCurSel();

	CRect rRef;
	CRect rToMove;
	CRect rectGrp;
	int nDistY = 0;

	// IRS Load Type
	GetDlgItem(IDC_CMD_MVHL_WHEELED_TRACKED_RDO)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_BG_RDO)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aIRSLoadType, nDistY);

	// Minimum Nose to Tail Distance
	GetDlgItem(IDC_CMD_MVHL_NOSE_TAIL_DIST_EDT)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_NOSE_TAIL_DIST_WHEELED_EDT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aNoseTail70R, nDistY);

	// IRS
	GetDlgItem(IDC_CMD_MVHL_IRC_WHEELED_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_IRS_PTLIST1_GRP)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aIRS, nDistY);
		
	// Group Box 크기
	GetDlgItem(IDC_CMD_MVHL_VEHI_PROP_GRP)->GetWindowRect(rectGrp);

	if(nCodeType == 0) 
	{
		if(m_nIRCLoadType == 0) GetDlgItem(IDC_CMD_MVHL_NOSE_TAIL_DIST_TRACKED_EDT)->GetWindowRect(rRef);
		else                    GetDlgItem(IDC_CMD_MVHL_NOSE_TAIL_DIST_EDT)->GetWindowRect(rRef);
	}
	else               GetDlgItem(IDC_CMD_MVHL_IRS_PTLIST1_GRP)->GetWindowRect(rRef);

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
	if(nCodeType == 0) GetDlgItem(IDC_CMD_MVHL_VEHI_PROP_GRP)->GetWindowRect(rRef);
	else               GetDlgItem(IDC_CMD_MVHL_LONGI_GRP)->GetWindowRect(rRef);	
		
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, m_aOKCancel, nDistY);

	CRect r;
	GetWindowRect(r);
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rToMove);
	r.bottom = rToMove.bottom+ globalUtils.ScaleByDPI(8);
	MoveWindow(r);

	Invalidate();
}

void CCMMvhlItemUsrIndiaDlg::ControlsShowHide()
{
	UpdateData(TRUE);

	int nCodeType = m_cmbCodeName.GetCurSel();

	CDlgUtil::CtrlShowHide(this, m_aIRCLoadType,       nCodeType==0);
	CDlgUtil::CtrlShowHide(this, m_aIRSLoadType,       nCodeType==1);
	CDlgUtil::CtrlShowHide(this, m_aIRC,               nCodeType==0);
	CDlgUtil::CtrlShowHide(this, m_aNoseTail,          nCodeType==0 && m_nIRCLoadType == 1);
	CDlgUtil::CtrlShowHide(this, m_aNoseTail70R,       nCodeType==0 && m_nIRCLoadType == 0);
	CDlgUtil::CtrlShowHide(this, m_aIRS,               nCodeType==1);
	CDlgUtil::CtrlShowHide(this, m_aLongiForce,        nCodeType==1);
	CDlgUtil::CtrlShowHide(this, m_aBrakeLOCORatio,    nCodeType==1 && m_nIRSLoadType!=3);
	//CDlgUtil::CtrlShowHide(this, m_aBrakeTrainRatio,   nCodeType==1 && m_nIRSLoadType!=3);
	CDlgUtil::CtrlShowHide(this, m_aBrakeLOCO,         nCodeType==1 && m_nIRSLoadType==3);
}

void CCMMvhlItemUsrIndiaDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	if(m_nIRSLoadType == 1 || m_nIRSLoadType == 2)
	{
		m_nTrainSubType = 1;
		GetDlgItem(IDC_CMD_MVHL_IRS_GONDOLA_RDO)->EnableWindow(FALSE);

		UpdateData(FALSE);

		ChangeBitmap();
	}
	else
	{
		GetDlgItem(IDC_CMD_MVHL_IRS_GONDOLA_RDO)->EnableWindow(TRUE);
	}

	CDlgUtil::CtrlEnableDisable(this, m_aTrack,    m_nIRCLoadType==0);
	CDlgUtil::CtrlEnableDisable(this, m_aBogie,    m_nIRCLoadType==0);
	CDlgUtil::CtrlEnableDisable(this, m_aGondola,  m_nTrainSubType==0);
	CDlgUtil::CtrlEnableDisable(this, m_aTrain,    m_nTrainSubType==1);
}

void CCMMvhlItemUsrIndiaDlg::ChangeBitmap()
{
	UpdateData(TRUE);

	CString nitmapID;

	int nCodeType = m_cmbCodeName.GetCurSel();

	int nLoadType = 0;
	if(nCodeType == 0) nLoadType = m_nIRCLoadType;
	else               nLoadType = m_nIRSLoadType;

	int nTrainSubType = m_nTrainSubType;

	if(nLoadType == 2) return;

	if(nCodeType == 0) // IRC
	{
		if     (nLoadType == 0) nitmapID = _T("cmd_mvhl_217.svg");
		else if(nLoadType == 1) nitmapID = _T("cmd_mvhl_218.svg");
	}
	else if(nCodeType == 1)
	{
		if     (nTrainSubType == 0) nitmapID = _T("cmd_mvhl_220.svg");
		else if(nTrainSubType == 1) nitmapID = _T("cmd_mvhl_219.svg");
		else ASSERT(0);
	}
	else
	{
		ASSERT(0);
	}

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + nitmapID);
}

void CCMMvhlItemUsrIndiaDlg::SetHeaderTitle()
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

void CCMMvhlItemUsrIndiaDlg::MakeItemEx()
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
			
	UpdateData(FALSE);
}

BOOL CCMMvhlItemUsrIndiaDlg::InsertItem(int nListType, int nPos, int nNo, double dLoad, double dDist)
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
		else
		{
			ASSERT(0);
		}		

		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CCMMvhlItemUsrIndiaDlg::ModifyItem(int nListType, int nPos, int nNo, double dLoad, double dDist)
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

			if     (nListType == IRC_LIST)  m_lstIRC.SetItemText(nItem, i, str);
			else if(nListType == IRS1_LIST) m_lstIRS1.SetItemText(nItem, i, str);
			else if(nListType == IRS2_LIST) m_lstIRS2.SetItemText(nItem, i, str);
			else ASSERT(0);			
		}
	}
	return TRUE;
}

BOOL CCMMvhlItemUsrIndiaDlg::DeleteItem(int nListType, int nPos)
{
	if(nListType == IRC_LIST)
	{
		if (nPos != -1) m_lstIRC.DeleteItem(nPos);
	}
	else if(nListType == IRS1_LIST)
	{
		if (nPos != -1) m_lstIRS1.DeleteItem(nPos);
	}
	else if(nListType == IRS2_LIST)
	{
		if (nPos != -1) m_lstIRS2.DeleteItem(nPos);
	}
	else 
	{
		ASSERT(0);
	}
	
	return TRUE;
}

int CCMMvhlItemUsrIndiaDlg::GetLoadCount(int nListType)
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
		for(i=0; i<D_NUMLOAD_MAIN; i++)
		{
			if (m_Data.dPointLoad[i] == 0.0) break;
		}
	}
	else if(nListType == IRS2_LIST)
	{
		for(i=0; i<D_NUMLOAD_GONDOLA; i++)
		{
			if (m_Data.dPointLoad3[0][i] == 0.0) break;
		}
	}
	else
	{
		ASSERT(0);
	}
	
	return i;
}

int CCMMvhlItemUsrIndiaDlg::GetDistCount(int nListType)
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
		for(i=0; i<D_NUMLOAD_MAIN; i++)
		{
			if (m_Data.dPointDistance[i] == 0.0) break;
		}
	}
	else if(nListType == IRS2_LIST)
	{
		for(i=0; i<D_NUMLOAD_GONDOLA; i++)
		{
			if (m_Data.dPointDistance3[0][i] == 0.0) break;
		}
	}
	else
	{
		ASSERT(0);
	}

	return i;
}

BOOL CCMMvhlItemUsrIndiaDlg::GetLoadAndDistValue(int nListType, double &dLoad, double &dDist)
{
	if(nListType == IRC_LIST)
	{
		if (!CFormulaEdit::GetEditValue(&m_edtIRC_P, dLoad))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_load_value_));
			return FALSE;
		}
		if (!CFormulaEdit::GetEditValue(&m_edtIRC_D, dDist))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_distance_value_));
			return FALSE;
		}
	}
	else if(nListType == IRS1_LIST)
	{
		if (!CFormulaEdit::GetEditValue(&m_edtIRS1_P, dLoad))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_load_value_));
			return FALSE;
		}
		if (!CFormulaEdit::GetEditValue(&m_edtIRS1_D, dDist))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_distance_value_));
			return FALSE;
		}
	}
	else if(nListType == IRS2_LIST)
	{
		if (!CFormulaEdit::GetEditValue(&m_edtIRS2_P, dLoad))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_load_value_));
			return FALSE;
		}
		if (!CFormulaEdit::GetEditValue(&m_edtIRS2_D, dDist))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_distance_value_));
			return FALSE;
		}
	}
	else
	{
		ASSERT(0);
	}
	
	if (dLoad == 0.0) // Load 0은 허용 안함
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Load_value_must_not_be_ze));
		return FALSE;
	}
	return TRUE;
}

void CCMMvhlItemUsrIndiaDlg::Data2Dlg()
{
	CDlgUtil::CobxSetCurSelItemData(m_cmbCodeName, m_Data.nStandardCode);

	if     (m_Data.nLoadType == 0) m_nIRCLoadType = 0;
	else if(m_Data.nLoadType == 1) m_nIRCLoadType = 1;
	else if(m_Data.nLoadType == 2) m_nIRCLoadType = 2;
	else if(m_Data.nLoadType == 3) m_nIRSLoadType = 0;
	else if(m_Data.nLoadType == 4) m_nIRSLoadType = 1;
	else if(m_Data.nLoadType == 5) m_nIRSLoadType = 2;
	else if(m_Data.nLoadType == 6) m_nIRSLoadType = 3;

	m_edtVehicleName.SetWindowText(m_Data.VehicleLoadName);

	m_edtIRCdD1.SetEditUnit(m_Data.dD1);
	m_edtIRCdD2.SetEditUnit(m_Data.dD2);
	m_edtIRCP.SetEditUnit(m_Data.dP);
	m_edtIRCD.SetEditUnit(m_Data.dDTrailer);
	m_edtIRCPb.SetEditUnit(m_Data.dPb);
	m_edtIRCDb.SetEditUnit(m_Data.dDb);
	m_edtNoseTailDist.SetEditUnit(m_Data.dNoseTailDist);
	m_edtNoseTailDistW.SetEditUnit(m_Data.dNoseTailDist);
	m_edtNoseTailDistT.SetEditUnit(m_Data.dNoseTailDist2);

	m_edtIRSdD12.SetEditUnit(m_Data.dTrainD1);
	m_edtIRSdD22.SetEditUnit(m_Data.dTrainD2);
	m_nTrainSubType = m_Data.nTrainSubType;
	m_nGondolaPos = m_Data.nPointLoadPos;
	m_edtIRSdD3.SetEditUnit(m_Data.dTrainD3);
	m_nTrainPos = m_Data.nDistLoadPos;
	m_edtIRSdW.SetEditUnit(m_Data.dUniformLoad);
	m_edtTractive.SetEditUnit(m_Data.dTractive);
	m_edtBrakeLOCORatio.SetEditUnit(m_Data.dBrakeLocoRatio);
	m_edtBrakeTrainRatio.SetEditUnit(m_Data.dBrakeTrainRatio);
	m_edtBrakeLOCO.SetEditUnit(m_Data.dBrakeLoco);

	MakeItemEx();

	UpdateData(FALSE);
}

BOOL CCMMvhlItemUsrIndiaDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.bStandard = FALSE;
	m_Data.nStandardCode = CDlgUtil::CobxGetCurSelItemData(m_cmbCodeName, m_cmbCodeName.GetCurSel());

	if(m_Data.nStandardCode == D_MVHL_IRC6_2000) m_Data.nLoadType = m_nIRCLoadType;
	else                                         m_Data.nLoadType = m_nIRSLoadType+3;
 	
	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);   

	m_Data.dD1 = m_edtIRCdD1.GetEditValue();
	m_Data.dD2 = m_edtIRCdD2.GetEditValue();
	m_Data.dP = m_edtIRCP.GetEditValue();
	m_Data.dDTrailer = m_edtIRCD.GetEditValue();
	m_Data.dPb = m_edtIRCPb.GetEditValue();
	m_Data.dDb = m_edtIRCDb.GetEditValue();

	if(m_Data.nLoadType == 0) m_Data.dNoseTailDist = m_edtNoseTailDistW.GetEditValue();
	else                      m_Data.dNoseTailDist = m_edtNoseTailDist.GetEditValue();
	m_Data.dNoseTailDist2 = m_edtNoseTailDistT.GetEditValue();

	m_Data.dTrainD1 = m_edtIRSdD12.GetEditValue();
	m_Data.dTrainD2 = m_edtIRSdD22.GetEditValue();
	m_Data.nTrainSubType = m_nTrainSubType;
	m_Data.nPointLoadPos = m_nGondolaPos;
	m_Data.dTrainD3 = m_edtIRSdD3.GetEditValue();
	m_Data.nDistLoadPos = m_nTrainPos;
	m_Data.dUniformLoad = m_edtIRSdW.GetEditValue();
	m_Data.dTractive = m_edtTractive.GetEditValue();
	m_Data.dBrakeLocoRatio = m_edtBrakeLOCORatio.GetEditValue();
	m_Data.dBrakeTrainRatio = m_edtBrakeTrainRatio.GetEditValue();
	m_Data.dBrakeLoco = m_edtBrakeLOCO.GetEditValue();

	// IRS
	if(m_Data.nStandardCode == D_MVHL_IRS_BRIDGE_RULES) 
	{
		for(int i=0; i<D_MVHL_NUMLOAD; i++)
		{
			if(i >= D_NUMLOAD_MAIN)
			{
				m_Data.dPointLoad[i] = 0.0;
				m_Data.dPointDistance[i] = 0.0;
			}

			if(i >= D_NUMLOAD_GONDOLA)
			{
				m_Data.dPointLoad3[0][i] = 0.0;
				m_Data.dPointDistance3[0][i] = 0.0;
			}
		}
	}

	return TRUE;
}

BOOL CCMMvhlItemUsrIndiaDlg::ApplyOrOK()
{
	if(!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if(m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else          bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);
	if(!bSuccess) return FALSE;
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrIndiaDlg message handlers

BOOL CCMMvhlItemUsrIndiaDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	if(!m_bModify)
	{
		m_Data.Initialize();
		m_Data.bStandard = FALSE;    
		m_Data.nStandardCode = D_MVHL_IRC6_2000;
	}
	
 	InitControl();
 	InitUnit();
 	InitCombo();

 	SetHeaderTitle();
 		
	Data2Dlg();

	ChangeBitmap();

	AlignControl();
	ControlsShowHide();
	EnableDisableControls();

	OnCmdMvhlIRCLoadTypeRdo();
 	
	if(m_bModify)	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemUsrIndiaDlg::OnOK() 
{
	if (!ApplyOrOK()) return;
	CDialogMove::OnOK();
}

void CCMMvhlItemUsrIndiaDlg::OnCmdMvhlCodeCmb() 
{
	m_Data.Initialize();

	ChangeBitmap();

	AlignControl();
	ControlsShowHide();
	EnableDisableControls();

	MakeItemEx();
}

void CCMMvhlItemUsrIndiaDlg::OnCmdMvhlIRCLoadTypeRdo() 
{
	UpdateData(TRUE);

	if(m_nIRCLoadType == 2)
	{
 		if (m_bPermitLoadCall)
 		{
 			m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);
 			m_bPermitLoadCall = FALSE;
 			CCMMvhlItemUsrPermitDlg dlg;
 			dlg.SetInitPos(D_INIT_POS_LT);
 			CString csRadioText[3];

 			T_MVCD_D MvcdD; MvcdD.Initialize();
 			if(!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) return;
 			dlg.SetModifyData(m_bModify, m_csOldName, &m_Data, m_pDoc, csRadioText, MvcdD.nCodeType);
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
				m_edtVehicleName.SetWindowText(m_Data.VehicleLoadName);

				int nCodeType = dlg.GetCodeType();
 				m_nIRCLoadType = dlg.GetIndiaType();

				UpdateData(FALSE);

				if(nCodeType == D_MVHL_INDIA_IRC6_2000) 
				{
					OnCmdMvhlIRCLoadTypeRdo();
				}
				else
				{
					m_cmbCodeName.SetCurSel(nCodeType);
					OnCmdMvhlCodeCmb();
				}
 			}
 			else
 			{
 				CDialogMove::OnOK();
 			}
 		}
		else
		{
			m_bPermitLoadCall = TRUE;
			PostMessage(WM_COMMAND, MAKEWPARAM(IDC_CMD_MVHL_WHEELED_TRACKED_RDO, BN_CLICKED), 0);
		}
	}
	else
	{
		ChangeBitmap();

		AlignControl();
		ControlsShowHide();
		EnableDisableControls();
	}
}

void CCMMvhlItemUsrIndiaDlg::OnCmdMvhlIRCAddBtn() 
{
	int nLoadCount = GetLoadCount(IRC_LIST);
	int nDistCount = GetDistCount(IRC_LIST);

	double dLoad, dDist;
	if (!GetLoadAndDistValue(IRC_LIST, dLoad, dDist)) return;

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

	int no;
	no = nLoadCount+1;
	InsertItem(IRC_LIST, nLoadCount, no, dLoad, dDist);

	int nCount = m_lstIRC.GetItemCount();
	m_lstIRC.SetItemState(nCount-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvhlItemUsrIndiaDlg::OnCmdMvhlIRCInsBtn() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_lstIRC.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_inser));
		return;
	}

	int nLoadCount = GetLoadCount(IRC_LIST);
	int nDistCount = GetDistCount(IRC_LIST);

	double dLoad, dDist;
	if (!GetLoadAndDistValue(IRC_LIST, dLoad, dDist)) return;

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
	InsertItem(IRC_LIST, iItem, iItem+1, dLoad, dDist);

	// List 번호 변경
	int nCount = m_lstIRC.GetItemCount();
	CString csVal;
	for (int i = iItem+1; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_lstIRC.SetItemText(i, 0, csVal);
	}
	m_lstIRC.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvhlItemUsrIndiaDlg::OnCmdMvhlIRCModBtn() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_lstIRC.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_modif));
		return;
	}

	int nLoadCount = GetLoadCount(IRC_LIST);
	int nDistCount = GetDistCount(IRC_LIST);

	double dLoad, dDist;
	if (!GetLoadAndDistValue(IRC_LIST, dLoad, dDist)) return;

	if (iItem != nLoadCount-1 && dDist == 0.0)  // middle and dist is zero!
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Intermediate_distance_mus));
		return;
	}

	m_Data.dPointLoad[iItem] = dLoad;
	m_Data.dPointDistance[iItem] = dDist;

	ModifyItem(IRC_LIST, iItem, iItem+1, dLoad, dDist);
}

void CCMMvhlItemUsrIndiaDlg::OnCmdMvhlIRCDelBtn() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_lstIRC.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_delet));
		return;
	}

	int nLoadCount = GetLoadCount(IRC_LIST);
	int nDistCount = GetDistCount(IRC_LIST);
	int i = 0;
	for (i = iItem; i < D_MVHL_NUMLOAD-1; i++)
	{
		m_Data.dPointLoad[i] = m_Data.dPointLoad[i+1];
		m_Data.dPointDistance[i] = m_Data.dPointDistance[i+1];
	}
	m_Data.dPointLoad[i] = 0.0;
	m_Data.dPointDistance[i] = 0.0;

	m_lstIRC.DeleteItem(iItem);
	int nCount = m_lstIRC.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_lstIRC.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);

	// List 번호 변경
	CString csVal;
	for (i = iItem; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_lstIRC.SetItemText(i, 0, csVal);
	}
}

void CCMMvhlItemUsrIndiaDlg::OnCmdMvhlIRCItemChange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iItem = m_lstIRC.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	int nLoadCount = GetLoadCount(IRC_LIST);
	int nDistCount = GetDistCount(IRC_LIST);

	CString csVal;
	if (iItem == nLoadCount-1 && nLoadCount != nDistCount)  // fixed, last
	{
		csVal.Format(_T("%g"), m_Data.dPointLoad[iItem]);
		m_edtIRC_P.SetWindowText(csVal);
		m_edtIRC_D.SetWindowText(_T(""));
	}
	else
	{
		csVal.Format(_T("%g"), m_Data.dPointLoad[iItem]);
		m_edtIRC_P.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data.dPointDistance[iItem]);
		m_edtIRC_D.SetWindowText(csVal);
	}

	*pResult = 0;
}

void CCMMvhlItemUsrIndiaDlg::OnCmdMvhlIRSLoadTypeRdo() 
{
	ChangeBitmap();

	ControlsShowHide();
	EnableDisableControls();
}

void CCMMvhlItemUsrIndiaDlg::OnCmdMvhlIRS1AddBtn() 
{
	int nLoadCount = GetLoadCount(IRS1_LIST);
	int nDistCount = GetDistCount(IRS1_LIST);

	double dLoad, dDist;
	if (!GetLoadAndDistValue(IRS1_LIST, dLoad, dDist)) return;

	if (nLoadCount >= D_NUMLOAD_MAIN) // MAX 갯수 초과시
	{
		CString csErr;
		csErr.Format(_LS(IDS_WG_CMD__ADDD__Error___Number_of_load_exceeded_l), D_NUMLOAD_MAIN);
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

	int no;
	no = nLoadCount+1;
	InsertItem(IRS1_LIST, nLoadCount, no, dLoad, dDist);

	int nCount = m_lstIRS1.GetItemCount();
	m_lstIRS1.SetItemState(nCount-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvhlItemUsrIndiaDlg::OnCmdMvhlIRS1InsBtn() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_lstIRS1.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_inser));
		return;
	}

	int nLoadCount = GetLoadCount(IRS1_LIST);
	int nDistCount = GetDistCount(IRS1_LIST);

	double dLoad, dDist;
	if (!GetLoadAndDistValue(IRS1_LIST, dLoad, dDist)) return;

	if (nLoadCount >= D_NUMLOAD_MAIN) // MAX 갯수 초과시
	{
		CString csErr;
		csErr.Format(_LS(IDS_WG_CMD__ADDD__Error___Number_of_load_exceeded_l), D_NUMLOAD_MAIN);
		AfxMessageBox(csErr);
		return;
	}

	if (dDist == 0.0)  // middle and dist is zero!
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Intermediate_distance_mus));
		return;
	}

	// 데이타 이동하기
	int i = 0;
	for (i = D_NUMLOAD_MAIN-1; i > iItem; i--)
	{
		m_Data.dPointLoad[i] = m_Data.dPointLoad[i-1];
		m_Data.dPointDistance[i] = m_Data.dPointDistance[i-1];
	}

	// 데이타 삽입한다.
	m_Data.dPointLoad[iItem] = dLoad;
	m_Data.dPointDistance[iItem] = dDist;

	// List에 Item 삽입
	InsertItem(IRS1_LIST, iItem, iItem+1, dLoad, dDist);

	// List 번호 변경
	int nCount = m_lstIRS1.GetItemCount();
	CString csVal;
	for (i = iItem+1; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_lstIRS1.SetItemText(i, 0, csVal);
	}
	m_lstIRS1.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvhlItemUsrIndiaDlg::OnCmdMvhlIRS1ModBtn() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_lstIRS1.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_modif));
		return;
	}

	int nLoadCount = GetLoadCount(IRS1_LIST);
	int nDistCount = GetDistCount(IRS1_LIST);

	double dLoad, dDist;
	if (!GetLoadAndDistValue(IRS1_LIST, dLoad, dDist)) return;

	if (iItem != nLoadCount-1 && dDist == 0.0)  // middle and dist is zero!
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Intermediate_distance_mus));
		return;
	}

	m_Data.dPointLoad[iItem] = dLoad;
	m_Data.dPointDistance[iItem] = dDist;

	ModifyItem(IRS1_LIST, iItem, iItem+1, dLoad, dDist);
}

void CCMMvhlItemUsrIndiaDlg::OnCmdMvhlIRS1DelBtn() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_lstIRS1.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_delet));
		return;
	}

	int nLoadCount = GetLoadCount(IRS1_LIST);
	int nDistCount = GetDistCount(IRS1_LIST);
	int i = 0;
	for (i = iItem; i < D_NUMLOAD_MAIN-1; i++)
	{
		m_Data.dPointLoad[i] = m_Data.dPointLoad[i+1];
		m_Data.dPointDistance[i] = m_Data.dPointDistance[i+1];
	}
	m_Data.dPointLoad[i] = 0.0;
	m_Data.dPointDistance[i] = 0.0;

	m_lstIRS1.DeleteItem(iItem);
	int nCount = m_lstIRS1.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_lstIRS1.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);

	// List 번호 변경
	CString csVal;
	for (i = iItem; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_lstIRS1.SetItemText(i, 0, csVal);
	}
}

void CCMMvhlItemUsrIndiaDlg::OnCmdMvhlIRS1ItemChange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iItem = m_lstIRS1.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	int nLoadCount = GetLoadCount(IRS1_LIST);
	int nDistCount = GetDistCount(IRS1_LIST);

	CString csVal;
	if (iItem == nLoadCount-1 && nLoadCount != nDistCount)  // fixed, last
	{
		csVal.Format(_T("%g"), m_Data.dPointLoad[iItem]);
		m_edtIRS1_P.SetWindowText(csVal);
		m_edtIRS1_D.SetWindowText(_T(""));
	}
	else
	{
		csVal.Format(_T("%g"), m_Data.dPointLoad[iItem]);
		m_edtIRS1_P.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data.dPointDistance[iItem]);
		m_edtIRS1_D.SetWindowText(csVal);
	}

	*pResult = 0;
}

void CCMMvhlItemUsrIndiaDlg::OnCmdMvhlIRSTrainSubTypeRdo() 
{
	ChangeBitmap();

	EnableDisableControls();
}

void CCMMvhlItemUsrIndiaDlg::OnCmdMvhlIRS2AddBtn() 
{
	int nLoadCount = GetLoadCount(IRS2_LIST);
	int nDistCount = GetDistCount(IRS2_LIST);

	double dLoad, dDist;
	if (!GetLoadAndDistValue(IRS2_LIST, dLoad, dDist)) return;

	if (nLoadCount >= D_NUMLOAD_GONDOLA) // MAX 갯수 초과시
	{
		CString csErr;
		csErr.Format(_LS(IDS_WG_CMD__ADDD__Error___Number_of_load_exceeded_l), D_NUMLOAD_GONDOLA);
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
		m_Data.dPointLoad3[0][nLoadCount] = dLoad;
	}
	else
	{
		m_Data.dPointLoad3[0][nLoadCount] = dLoad;
		m_Data.dPointDistance3[0][nLoadCount] = dDist;
	}

	int no;
	no = nLoadCount+1;
	InsertItem(IRS2_LIST, nLoadCount, no, dLoad, dDist);

	int nCount = m_lstIRS2.GetItemCount();
	m_lstIRS2.SetItemState(nCount-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvhlItemUsrIndiaDlg::OnCmdMvhlIRS2InsBtn() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_lstIRS2.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_inser));
		return;
	}

	int nLoadCount = GetLoadCount(IRS2_LIST);
	int nDistCount = GetDistCount(IRS2_LIST);

	double dLoad, dDist;
	if (!GetLoadAndDistValue(IRS2_LIST, dLoad, dDist)) return;

	if (nLoadCount >= D_NUMLOAD_GONDOLA) // MAX 갯수 초과시
	{
		CString csErr;
		csErr.Format(_LS(IDS_WG_CMD__ADDD__Error___Number_of_load_exceeded_l), D_NUMLOAD_GONDOLA);
		AfxMessageBox(csErr);
		return;
	}

	if (dDist == 0.0)  // middle and dist is zero!
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Intermediate_distance_mus));
		return;
	}

	// 데이타 이동하기
	int i = 0;
	for (i = D_NUMLOAD_GONDOLA-1; i > iItem; i--)
	{
		m_Data.dPointLoad3[0][i] = m_Data.dPointLoad3[0][i-1];
		m_Data.dPointDistance3[0][i] = m_Data.dPointDistance3[0][i-1];
	}

	// 데이타 삽입한다.
	m_Data.dPointLoad3[0][iItem] = dLoad;
	m_Data.dPointDistance3[0][iItem] = dDist;

	// List에 Item 삽입
	InsertItem(IRS2_LIST, iItem, iItem+1, dLoad, dDist);

	// List 번호 변경
	int nCount = m_lstIRS2.GetItemCount();
	CString csVal;
	for (i = iItem+1; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_lstIRS2.SetItemText(i, 0, csVal);
	}
	m_lstIRS2.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvhlItemUsrIndiaDlg::OnCmdMvhlIRS2ModBtn() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_lstIRS2.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_modif));
		return;
	}

	double dLoad, dDist;
	if (!GetLoadAndDistValue(IRS2_LIST, dLoad, dDist)) return;

	m_Data.dPointLoad3[0][iItem] = dLoad;
	m_Data.dPointDistance3[0][iItem] = dDist;

	ModifyItem(IRS2_LIST, iItem, iItem+1, dLoad, dDist);
}

void CCMMvhlItemUsrIndiaDlg::OnCmdMvhlIRS2DelBtn() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_lstIRS2.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_delet));
		return;
	}

	int nLoadCount = GetLoadCount(IRS2_LIST);
	int nDistCount = GetDistCount(IRS2_LIST);
	int i = 0;
	for (i = iItem; i < D_NUMLOAD_GONDOLA-1; i++)
	{
		m_Data.dPointLoad3[0][i] = m_Data.dPointLoad3[0][i+1];
		m_Data.dPointDistance3[0][i] = m_Data.dPointDistance3[0][i+1];
	}
	m_Data.dPointLoad3[0][i] = 0.0;
	m_Data.dPointDistance3[0][i] = 0.0;

	m_lstIRS2.DeleteItem(iItem);
	int nCount = m_lstIRS2.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_lstIRS2.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);

	// List 번호 변경
	CString csVal;
	for (i = iItem; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_lstIRS2.SetItemText(i, 0, csVal);
	}
}

void CCMMvhlItemUsrIndiaDlg::OnCmdMvhlIRS2ItemChange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iItem = m_lstIRS2.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	int nLoadCount = GetLoadCount(IRS2_LIST);
	int nDistCount = GetDistCount(IRS2_LIST);

	CString csVal;
	if (iItem == nLoadCount-1 && nLoadCount != nDistCount)  // fixed, last
	{
		csVal.Format(_T("%g"), m_Data.dPointLoad3[0][iItem]);
		m_edtIRS2_P.SetWindowText(csVal);
		m_edtIRS2_D.SetWindowText(_T(""));
	}
	else
	{
		csVal.Format(_T("%g"), m_Data.dPointLoad3[0][iItem]);
		m_edtIRS2_P.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data.dPointDistance3[0][iItem]);
		m_edtIRS2_D.SetWindowText(csVal);
	}

	*pResult = 0;
}

void CCMMvhlItemUsrIndiaDlg::OnCmdMvhlApply() 
{
	ApplyOrOK();
}
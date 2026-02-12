// CMMvhlItemStdAustraliaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdAustraliaDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_TestEnvMgr.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MvhlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdAustraliaDlg dialog

#define COLCOUNT 3

CCMMvhlItemStdAustraliaDlg::CCMMvhlItemStdAustraliaDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemStdAustraliaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemStdAustraliaDlg)	
	m_bFatigue = FALSE;
	//}}AFX_DATA_INIT  
	m_pDoc = CDBDoc::GetDocPoint();
	m_bModify = FALSE;
	m_dIncreLength = 0.0;

	m_nDistType = 0;
	m_nDLAType = 0;

	m_aDynaAllow.RemoveAll();
	m_aDynaAllow.Add(IDC_CMD_MVHL_DYNA_ALLOW_STC);
	m_aDynaAllow.Add(IDC_CMD_MVHL_DYNA_ALLOW_EDT);

	m_aDynaAllow2.RemoveAll();
	m_aDynaAllow2.Add(IDC_CMD_MVHL_DYNA_ALLOW_STC2);
	m_aDynaAllow2.Add(IDC_CMD_MVHL_DYNA_ALLOW_BEND_EDT2);
	m_aDynaAllow2.Add(IDC_CMD_MVHL_DYNA_ALLOW_BEND_STC);
	m_aDynaAllow2.Add(IDC_CMD_MVHL_DYNA_ALLOW_EDT2);
	m_aDynaAllow2.Add(IDC_CMD_MVHL_DYNA_ALLOW_OTHER_STC);
// 	m_aDynaAllow2.Add(IDC_CMD_MVHL_DYNA_ALLOW_SINGLE_EDT);
// 	m_aDynaAllow2.Add(IDC_CMD_MVHL_DYNA_ALLOW_SINGLE_SUB_STC);
// 	m_aDynaAllow2.Add(IDC_CMD_MVHL_DYNA_ALLOW_OTHER_EDT);
// 	m_aDynaAllow2.Add(IDC_CMD_MVHL_DYNA_ALLOW_OTHER_SUB_STC);
	
	m_aPicture.RemoveAll();
	m_aPicture.Add(IDC_CMD_MVHL_PICTURE);
	
	m_aList.RemoveAll();
	m_aList.Add(IDC_CMD_MVHL_PTLIST);

	m_aW.RemoveAll();
	m_aW.Add(IDC_CMD_MVHL_W_STC);
	m_aW.Add(IDC_CMD_MVHL_W_EDT);
	m_aW.Add(IDC_CMD_MVHL_W_UNT);
	
	m_aDW1DD1.RemoveAll();
	m_aDW1DD1.Add(IDC_CMD_MVHL_W1_STC);
	m_aDW1DD1.Add(IDC_CMD_MVHL_W1_EDT);
	m_aDW1DD1.Add(IDC_CMD_MVHL_W1_UNT);
	m_aDW1DD1.Add(IDC_CMD_MVHL_D1_STC);
	m_aDW1DD1.Add(IDC_CMD_MVHL_D1_EDT);
	m_aDW1DD1.Add(IDC_CMD_MVHL_D1_UNT);

	m_aFatigue.RemoveAll();
	m_aFatigue.Add(IDC_CMD_MVHL_FATIGUE_CHK);
	
	m_aPedestrian.RemoveAll();
	m_aPedestrian.Add(IDC_CMD_MVHL_PEDW1_STC);
	m_aPedestrian.Add(IDC_CMD_MVHL_PEDW1_EDT);
	m_aPedestrian.Add(IDC_CMD_MVHL_PEDW1_UNT);
	m_aPedestrian.Add(IDC_CMD_MVHL_PEDW2_STC);
	m_aPedestrian.Add(IDC_CMD_MVHL_PEDW3_STC);
	m_aPedestrian.Add(IDC_CMD_MVHL_PEDW3_EDT);
	m_aPedestrian.Add(IDC_CMD_MVHL_PEDW3_UNT);
	m_aPedestrian.Add(IDC_CMD_MVHL_PEDA1_COLON);
	m_aPedestrian.Add(IDC_CMD_MVHL_PEDA1_STC);
	m_aPedestrian.Add(IDC_CMD_MVHL_PEDA1_EDT);
	m_aPedestrian.Add(IDC_CMD_MVHL_PEDA1_UNT);
	m_aPedestrian.Add(IDC_CMD_MVHL_PEDA2_COLON);
	m_aPedestrian.Add(IDC_CMD_MVHL_PEDA2_MIN_EDT);
	m_aPedestrian.Add(IDC_CMD_MVHL_PEDA2_STC);
	m_aPedestrian.Add(IDC_CMD_MVHL_PEDA2_MAX_EDT);
	m_aPedestrian.Add(IDC_CMD_MVHL_PEDA2_UNT);
	m_aPedestrian.Add(IDC_CMD_MVHL_PEDA3_COLON);
	m_aPedestrian.Add(IDC_CMD_MVHL_PEDA3_EDT);
	m_aPedestrian.Add(IDC_CMD_MVHL_PEDA3_STC);
	m_aPedestrian.Add(IDC_CMD_MVHL_PEDA3_UNT);

	m_aHeavy.RemoveAll();
	m_aHeavy.Add(IDC_CMD_MVHL_HEAVY_P_STC);
	m_aHeavy.Add(IDC_CMD_MVHL_HEAVY_P_EDT);
	m_aHeavy.Add(IDC_CMD_MVHL_HEAVY_P_UNT);
	m_aHeavy.Add(IDC_CMD_MVHL_HEAVY_D_STC);
	m_aHeavy.Add(IDC_CMD_MVHL_HEAVY_D_EDT);
	m_aHeavy.Add(IDC_CMD_MVHL_HEAVY_D_UNT);
	m_aHeavy.Add(IDC_CMD_MVHL_HEAVY_AXIES_STC);

	m_aRoad.RemoveAll();
	m_aRoad.Add(IDC_CMD_MVHL_ROAD_LANE_W1_STC);
	m_aRoad.Add(IDC_CMD_MVHL_ROAD_LANE_W1_EDT);
	m_aRoad.Add(IDC_CMD_MVHL_ROAD_LANE_W1_UNT);
	m_aRoad.Add(IDC_CMD_MVHL_ROAD_LANE_W2_STC);
	m_aRoad.Add(IDC_CMD_MVHL_ROAD_LANE_W2_EDT);
	m_aRoad.Add(IDC_CMD_MVHL_ROAD_LANE_W2_UNT);
	m_aRoad.Add(IDC_CMD_MVHL_ROAD_LANE_P_STC);
	m_aRoad.Add(IDC_CMD_MVHL_ROAD_LANE_P_EDT);
	m_aRoad.Add(IDC_CMD_MVHL_ROAD_LANE_P_UNT);
	m_aRoad.Add(IDC_CMD_MVHL_ROAD_LANE_L1_STC);
	m_aRoad.Add(IDC_CMD_MVHL_ROAD_LANE_L2_STC);
		
	m_aRail.RemoveAll();
	m_aRail.Add(IDC_CMD_MVHL_RAIL_GRP);
	m_aRail.Add(IDC_CMD_MVHL_RAIL_MIN_STC);
	m_aRail.Add(IDC_CMD_MVHL_RAIL_MAX_STC);
	m_aRail.Add(IDC_CMD_MVHL_RAIL_INC_STC);
	m_aRail.Add(IDC_CMD_MVHL_RAIL_INC_EDT);
	m_aRail.Add(IDC_CMD_MVHL_RAIL_INC_UNT);
	m_aRail.Add(IDC_CMD_MVHL_RAIL_D1_STC);
	m_aRail.Add(IDC_CMD_MVHL_RAIL_D2_STC);

	m_aRailSub.RemoveAll();
	m_aRailSub.Add(IDC_CMD_MVHL_RAIL_INC_STC);
	m_aRailSub.Add(IDC_CMD_MVHL_RAIL_INC_EDT);
	m_aRailSub.Add(IDC_CMD_MVHL_RAIL_INC_UNT);
	m_aRailSub.Add(IDC_CMD_MVHL_RAIL_D1_STC);
	m_aRailSub.Add(IDC_CMD_MVHL_RAIL_D2_STC);

	m_aBDouble.RemoveAll();
	m_aBDouble.Add(IDC_CMD_MVHL_BDOUBLE_MINDIST_GRP);
	m_aBDouble.Add(IDC_CMD_MVHL_BDOUBLE_MINDIST_SLOW_RDO);
	m_aBDouble.Add(IDC_CMD_MVHL_BDOUBLE_MINDIST_NORM_RDO);
	m_aBDouble.Add(IDC_CMD_MVHL_BDOUBLE_DLA_GRP);
	m_aBDouble.Add(IDC_CMD_MVHL_BDOUBLE_DLA_AUTO_RDO);
	m_aBDouble.Add(IDC_CMD_MVHL_BDOUBLE_DLA_USER_RDO);
	m_aBDouble.Add(IDC_CMD_MVHL_BDOUBLE_DLA_EDT);

	m_aLateralEcc.RemoveAll();
	m_aLateralEcc.Add(IDC_CMD_MVHL_LATERAL_ECC_STC);
	m_aLateralEcc.Add(IDC_CMD_MVHL_LATERAL_ECC_EDT);
	m_aLateralEcc.Add(IDC_CMD_MVHL_LATERAL_ECC_UNT);

	m_aEtc.RemoveAll();
	m_aEtc.Add(IDOK);
	m_aEtc.Add(IDCANCEL);
	m_aEtc.Add(IDC_CMD_APPLY);
}

CCMMvhlItemStdAustraliaDlg::~CCMMvhlItemStdAustraliaDlg()
{
}

void CCMMvhlItemStdAustraliaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdAustraliaDlg)  
	DDX_Control(pDX, IDC_CMD_MVHL_CODE_CMB,               m_cmbCodeName);
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_NAME_EDT,          m_edtVehicleName);
	DDX_Control(pDX, IDC_CMD_MVHL_VEHI_TYPE_CMB,          m_cmbVehicleType);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_ALLOW_EDT,         m_edtDynaAllow);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_ALLOW_BEND_EDT2,   m_edtDynaAllow_Bend);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_ALLOW_EDT2,        m_edtDynaAllow_AllOther); 	
// 	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_ALLOW_SINGLE_EDT,  m_edtDynaAllow_Single);
// 	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_ALLOW_OTHER_EDT,   m_edtDynaAllow_Other);

	DDX_Control(pDX, IDC_CMD_MVHL_W_EDT,       m_edtW);
	DDX_Control(pDX, IDC_CMD_MVHL_W_UNT,       m_untW);
	DDX_Control(pDX, IDC_CMD_MVHL_W1_EDT,      m_edtW1);
	DDX_Control(pDX, IDC_CMD_MVHL_W1_UNT,      m_untW1);
	DDX_Control(pDX, IDC_CMD_MVHL_D1_EDT,      m_edtD1);
	DDX_Control(pDX, IDC_CMD_MVHL_D1_UNT,      m_untD1);
	DDX_Check  (pDX, IDC_CMD_MVHL_FATIGUE_CHK, m_bFatigue);

	DDX_Control(pDX, IDC_CMD_MVHL_PEDW1_EDT,   m_edtPedW1);
	DDX_Control(pDX, IDC_CMD_MVHL_PEDW1_UNT,   m_untPedW1);
	DDX_Control(pDX, IDC_CMD_MVHL_PEDW3_EDT,   m_edtPedW3);
	DDX_Control(pDX, IDC_CMD_MVHL_PEDW3_UNT,   m_untPedW3);

	DDX_Control(pDX, IDC_CMD_MVHL_PEDA1_EDT,      m_edtPedA1);
	DDX_Control(pDX, IDC_CMD_MVHL_PEDA1_UNT,      m_untPedA1);
	DDX_Control(pDX, IDC_CMD_MVHL_PEDA2_MIN_EDT,  m_edtPedA2_min);
	DDX_Control(pDX, IDC_CMD_MVHL_PEDA2_MAX_EDT,  m_edtPedA2_max);
	DDX_Control(pDX, IDC_CMD_MVHL_PEDA2_UNT,      m_untPedA2);
	DDX_Control(pDX, IDC_CMD_MVHL_PEDA3_EDT,      m_edtPedA3);
	DDX_Control(pDX, IDC_CMD_MVHL_PEDA3_UNT,      m_untPedA3);

	DDX_Control(pDX, IDC_CMD_MVHL_HEAVY_P_EDT,       m_edtHeavyP);
	DDX_Control(pDX, IDC_CMD_MVHL_HEAVY_P_UNT,       m_untHeavyP);
	DDX_Control(pDX, IDC_CMD_MVHL_HEAVY_D_EDT,       m_edtHeavyD);
	DDX_Control(pDX, IDC_CMD_MVHL_HEAVY_D_UNT,       m_untHeavyD);
	DDX_Control(pDX, IDC_CMD_MVHL_ROAD_LANE_W1_EDT,  m_edtRoadW1);
	DDX_Control(pDX, IDC_CMD_MVHL_ROAD_LANE_W1_UNT,  m_untRoadW1);
	DDX_Control(pDX, IDC_CMD_MVHL_ROAD_LANE_W2_EDT,  m_edtRoadW2);
	DDX_Control(pDX, IDC_CMD_MVHL_ROAD_LANE_W2_UNT,  m_untRoadW2);
	DDX_Control(pDX, IDC_CMD_MVHL_ROAD_LANE_P_EDT,   m_edtRoadP);
	DDX_Control(pDX, IDC_CMD_MVHL_ROAD_LANE_P_UNT,   m_untRoadP);
	DDX_Control(pDX, IDC_CMD_MVHL_RAIL_INC_EDT,      m_edtRailInc);
	DDX_Control(pDX, IDC_CMD_MVHL_RAIL_INC_UNT,      m_untRailInc);

	DDX_Control(pDX, IDC_CMD_MVHL_PTLIST, m_List);
	DDX_Control(pDX, IDC_CMD_MVHL_PICTURE, m_wndPicture);
	DDX_Radio(pDX, IDC_CMD_MVHL_BDOUBLE_MINDIST_SLOW_RDO, m_nDistType);
	DDX_Radio(pDX, IDC_CMD_MVHL_BDOUBLE_DLA_AUTO_RDO    , m_nDLAType);
	DDX_Control(pDX, IDC_CMD_MVHL_BDOUBLE_DLA_EDT       , m_edtDynaAllowDouble);

	DDX_Control(pDX, IDC_CMD_MVHL_LATERAL_ECC_EDT, m_edtLateralEcc);
	DDX_Control(pDX, IDC_CMD_MVHL_LATERAL_ECC_UNT, m_untLateralEcc);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMMvhlItemStdAustraliaDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemStdAustraliaDlg)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_CODE_CMB,        OnCmdMvhlCodeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_VEHI_TYPE_CMB,   OnCmdMvhlTypeCmb)
	ON_BN_CLICKED(IDC_CMD_MVHL_BDOUBLE_MINDIST_SLOW_RDO, OnCmdDistTypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_BDOUBLE_MINDIST_NORM_RDO, OnCmdDistTypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_BDOUBLE_DLA_AUTO_RDO    , OnCmdDLATypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_BDOUBLE_DLA_USER_RDO    , OnCmdDLATypeRdo)
	ON_BN_CLICKED   (IDC_CMD_APPLY,                OnCmdApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvhlItemStdAustraliaDlg::InitUnit()
{
	m_edtDynaAllow.SetUnitType(D_UNITSYS_NONE);
	m_edtDynaAllow_Bend.SetUnitType(D_UNITSYS_NONE);
	m_edtDynaAllow_AllOther.SetUnitType(D_UNITSYS_NONE);
// 	m_edtDynaAllow_Single.SetUnitType(D_UNITSYS_NONE);
// 	m_edtDynaAllow_Other.SetUnitType(D_UNITSYS_NONE);
	m_edtDynaAllowDouble.SetUnitType(D_UNITSYS_NONE);

	m_edtW.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_untW.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_edtW1.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_untW1.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_edtD1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untD1.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtPedW1.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untPedW1.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtPedW3.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untPedW3.SetUnitType(D_UNITSYS_BASE_STRESS);

	m_edtPedA1.SetUnitType(D_UNITSYS_BASE_AREA);
	m_untPedA1.SetUnitType(D_UNITSYS_BASE_AREA);
	m_edtPedA2_min.SetUnitType(D_UNITSYS_BASE_AREA);
	m_edtPedA2_max.SetUnitType(D_UNITSYS_BASE_AREA);
	m_untPedA2.SetUnitType(D_UNITSYS_BASE_AREA);
	m_edtPedA3.SetUnitType(D_UNITSYS_BASE_AREA);
	m_untPedA3.SetUnitType(D_UNITSYS_BASE_AREA);

	m_edtHeavyP.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_untHeavyP.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_edtHeavyD.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untHeavyD.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtRoadW1.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_untRoadW1.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_edtRoadW2.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_untRoadW2.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_edtRoadP.SetUnitType(D_UNITSYS_BASE_FORCE); 
	m_untRoadP.SetUnitType(D_UNITSYS_BASE_FORCE); 
	m_edtRailInc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untRailInc.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_untLateralEcc.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMMvhlItemStdAustraliaDlg::InitCombo()
{
	int nCodeSeq[5];
	nCodeSeq[0] = D_MVHL_AUSTRALIA_AS_5100_2;
	nCodeSeq[1] = D_MVHL_AS_5100_2;
	nCodeSeq[2] = D_MVHL_AS_5100_2_RAIL ; // AS 5100.2 - Rail Traffic Load
	nCodeSeq[3] = D_MVHL_AS_5100_7_HEAVY; // AS 5100.2 - Heavy Load Platform
	nCodeSeq[4] = D_MVHL_AS_5100_7_ROAD ; // AS 5100.7 - Rating Vehicles 
	
	for(int i=0; i<5; i++) 
	{
		CDlgUtil::CobxAddItem(m_cmbCodeName, CVehlDB::GetTypeNameAr()[nCodeSeq[i]], nCodeSeq[i]);
	}
}

void CCMMvhlItemStdAustraliaDlg::AlignControl()
{
	UpdateData(TRUE);
	
	CString strVehicleType = _T("");
	m_cmbVehicleType.GetWindowText(strVehicleType);
	
	CRect rRef;
	CRect rToMove;
	int nDistY = 0;	
	int nDistX = 0;
	// Dynamic Load Allowance - 2
	GetDlgItem(IDC_CMD_MVHL_DYNA_ALLOW_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_DYNA_ALLOW_STC2)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aDynaAllow2, nDistY);

	GetDlgItem(IDC_CMD_MVHL_LATERAL_ECC_STC)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, m_aLateralEcc, nDistY);

	// Picture
	if(strVehicleType == _T("W80") || 
		 strVehicleType == _T("A160") || 
		 strVehicleType == _T("S1600") || 
		 strVehicleType == _T("M1600") || 
		 strVehicleType == _T("M1600 without UDL") || 
		 strVehicleType == _T("M1600 tri-axle group") ||
		 strVehicleType == _T("T44 Truck Load") || 
		 strVehicleType == _T("L44 Lane Load") ||
		
		 strVehicleType == _T("WA-Group1-Vehicle1") ||
		 strVehicleType == _T("WA-Group1-Vehicle2") ||
		 strVehicleType == _T("WA-Group1-Vehicle3") ||
		 strVehicleType == _T("WA-Group1-Vehicle4"))
	{	
		GetDlgItem(IDC_CMD_MVHL_DYNA_ALLOW_STC)->GetWindowRect(rRef);
	}
	else if (strVehicleType == _T("HLP320") ||
		strVehicleType == _T("HLP400") ||
		strVehicleType == _T("WA-Group2-Vehicle1 (3.01m O/A)") ||
		strVehicleType == _T("WA-Group2-Vehicle1 (3.70m O/A)") ||
		strVehicleType == _T("WA-Group2-Vehicle2 (3.01m O/A)") ||
		strVehicleType == _T("WA-Group2-Vehicle2 (3.70m O/A)") ||
		strVehicleType == _T("WA-Group2-Vehicle4") ||
		strVehicleType == _T("WA-Group2-Vehicle4 (Non-Supervised)") ||
		strVehicleType == _T("WA-Group2-Vehicle5") ||
		strVehicleType == _T("WA-Group2-Vehicle5 (Non-Supervised)") ||
		strVehicleType == _T("WA-Group2-Vehicle7") ||
		strVehicleType == _T("WA-Group2-Vehicle8"))
	{
		GetDlgItem(IDC_CMD_MVHL_LATERAL_ECC_EDT)->GetWindowRect(rRef);
	}
	else if(strVehicleType == _T("300LA") || strVehicleType == _T("150LA") )
	{
		GetDlgItem(IDC_CMD_MVHL_DYNA_ALLOW_EDT2)->GetWindowRect(rRef);
	}
	else
	{
		GetDlgItem(IDC_CMD_MVHL_VEHI_TYPE_STC)->GetWindowRect(rRef);
	}

	GetDlgItem(IDC_CMD_MVHL_PICTURE)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, m_aPicture, nDistY);

	// List 
	GetDlgItem(IDC_CMD_MVHL_PICTURE)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, m_aList, nDistY);

	// W
	GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_W_EDT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aW, nDistY);

	// dW1, dD1
	GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_W1_EDT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aDW1DD1, nDistY);
	
	// Fatigue
	GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_FATIGUE_CHK)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aFatigue, nDistY);

	// Pedestrian Live Load 관련 Controls
	GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_PEDW1_STC)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aPedestrian, nDistY);

	// Heavy Load
	GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_HEAVY_P_STC)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aHeavy, nDistY);

	// Road Load
	GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_ROAD_LANE_W1_STC)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aRoad, nDistY);

	// Rail Load
	GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_RAIL_GRP)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aRail, nDistY);

	// Rail Load
	GetDlgItem(IDC_CMD_MVHL_RAIL_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_BDOUBLE_MINDIST_GRP)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aBDouble, nDistX, nDistY);

	// OK, Cancel, Apply
	GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
	GetDlgItem(IDOK)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(24);
	CDlgUtil::CtrlMoveDistY(this, m_aEtc, nDistY);

	// Group Box 크기
	CRect rectTarget;
	CRect rect;
	CRect rectShort;
	
	GetDlgItem(IDC_CMD_MVHL_VEHI_PROP_GRP)->GetWindowRect(rectShort);

	GetDlgItem(IDC_CMD_MVHL_VEHI_NAME_EDT)->GetWindowRect(rectTarget);
	GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rect);
	nDistY = rect.bottom - rectTarget.top + globalUtils.ScaleByDPI(30);

	rectShort.bottom = rectShort.top + nDistY;
	
	ScreenToClient(rectShort);
	GetDlgItem(IDC_CMD_MVHL_VEHI_PROP_GRP)->MoveWindow(rectShort);
	
	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);

	Invalidate();
}

void CCMMvhlItemStdAustraliaDlg::ControlsShowHide()
{
	UpdateData(TRUE);
	
	CString strVehicleType = _T("");
	m_cmbVehicleType.GetWindowText(strVehicleType);

	if(strVehicleType == _T("W80"))
	{
		CDlgUtil::CtrlShowHide(this, m_aDynaAllow,   TRUE);
		CDlgUtil::CtrlShowHide(this, m_aDynaAllow2,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aPicture,     TRUE);
		CDlgUtil::CtrlShowHide(this, m_aList,        TRUE);
		CDlgUtil::CtrlShowHide(this, m_aW,           FALSE);
		CDlgUtil::CtrlShowHide(this, m_aDW1DD1,      FALSE);
		CDlgUtil::CtrlShowHide(this, m_aFatigue,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aPedestrian,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aHeavy,       FALSE);
		CDlgUtil::CtrlShowHide(this, m_aRoad,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aRail,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aBDouble,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aLateralEcc,  FALSE);
	}
	else if(strVehicleType == _T("A160"))
	{
		CDlgUtil::CtrlShowHide(this, m_aDynaAllow,   TRUE);
		CDlgUtil::CtrlShowHide(this, m_aDynaAllow2,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aPicture,     TRUE);
		CDlgUtil::CtrlShowHide(this, m_aList,        TRUE);
		CDlgUtil::CtrlShowHide(this, m_aW,           FALSE);
		CDlgUtil::CtrlShowHide(this, m_aDW1DD1,      FALSE);
		CDlgUtil::CtrlShowHide(this, m_aFatigue,     TRUE);
		CDlgUtil::CtrlShowHide(this, m_aPedestrian,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aHeavy,       FALSE);
		CDlgUtil::CtrlShowHide(this, m_aRoad,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aRail,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aBDouble,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aLateralEcc,  FALSE);
	}
	else if(strVehicleType == _T("M1600") || 
		      strVehicleType == _T("M1600 tri-axle group"))
	{
		CDlgUtil::CtrlShowHide(this, m_aDynaAllow,   TRUE);
		CDlgUtil::CtrlShowHide(this, m_aDynaAllow2,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aPicture,     TRUE);
		CDlgUtil::CtrlShowHide(this, m_aList,        TRUE);
		CDlgUtil::CtrlShowHide(this, m_aW,           TRUE);
		CDlgUtil::CtrlShowHide(this, m_aDW1DD1,      FALSE);
		CDlgUtil::CtrlShowHide(this, m_aFatigue,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aPedestrian,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aHeavy,       FALSE);
		CDlgUtil::CtrlShowHide(this, m_aRoad,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aRail,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aBDouble,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aLateralEcc,  FALSE);
	}
	else if(strVehicleType == _T("M1600 without UDL"))
	{
		CDlgUtil::CtrlShowHide(this, m_aDynaAllow,   TRUE);
		CDlgUtil::CtrlShowHide(this, m_aDynaAllow2,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aPicture,     TRUE);
		CDlgUtil::CtrlShowHide(this, m_aList,        TRUE);
		CDlgUtil::CtrlShowHide(this, m_aW,           FALSE);
		CDlgUtil::CtrlShowHide(this, m_aDW1DD1,      FALSE);
		CDlgUtil::CtrlShowHide(this, m_aFatigue,     TRUE);
		CDlgUtil::CtrlShowHide(this, m_aPedestrian,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aHeavy,       FALSE);
		CDlgUtil::CtrlShowHide(this, m_aRoad,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aRail,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aBDouble,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aLateralEcc,  FALSE);
	}
	else if(strVehicleType == _T("S1600"))
	{
		CDlgUtil::CtrlShowHide(this, m_aDynaAllow,   TRUE);
		CDlgUtil::CtrlShowHide(this, m_aDynaAllow2,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aPicture,     TRUE);
		CDlgUtil::CtrlShowHide(this, m_aList,        TRUE);
		CDlgUtil::CtrlShowHide(this, m_aW,           TRUE);
		CDlgUtil::CtrlShowHide(this, m_aDW1DD1,      FALSE);
		CDlgUtil::CtrlShowHide(this, m_aFatigue,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aPedestrian,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aHeavy,       FALSE);
		CDlgUtil::CtrlShowHide(this, m_aRoad,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aRail,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aBDouble,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aLateralEcc,  FALSE);
	}
	else if(strVehicleType == _T("Attached to Road Bridge") || strVehicleType == _T("Independent of Road Bridge"))
	{
		CDlgUtil::CtrlShowHide(this, m_aDynaAllow,   FALSE);
		CDlgUtil::CtrlShowHide(this, m_aDynaAllow2,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aPicture,     TRUE);
		CDlgUtil::CtrlShowHide(this, m_aList,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aW,           FALSE);
		CDlgUtil::CtrlShowHide(this, m_aDW1DD1,      FALSE);
		CDlgUtil::CtrlShowHide(this, m_aFatigue,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aPedestrian,  TRUE);
		CDlgUtil::CtrlShowHide(this, m_aHeavy,       FALSE);
		CDlgUtil::CtrlShowHide(this, m_aRoad,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aRail,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aBDouble,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aLateralEcc,  FALSE);
	}
	else if(strVehicleType == _T("Service Live Load on Walkways"))
	{
		CDlgUtil::CtrlShowHide(this, m_aDynaAllow,   FALSE);
		CDlgUtil::CtrlShowHide(this, m_aDynaAllow2,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aPicture,     TRUE);
		CDlgUtil::CtrlShowHide(this, m_aList,        TRUE);
		CDlgUtil::CtrlShowHide(this, m_aW,           FALSE);
		CDlgUtil::CtrlShowHide(this, m_aDW1DD1,      TRUE);
		CDlgUtil::CtrlShowHide(this, m_aFatigue,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aPedestrian,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aHeavy,       FALSE);
		CDlgUtil::CtrlShowHide(this, m_aRoad,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aRail,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aBDouble,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aLateralEcc,  FALSE);
	}
	else if(strVehicleType == _T("HLP320") || strVehicleType == _T("HLP400"))
	{
		CDlgUtil::CtrlShowHide(this, m_aDynaAllow,   TRUE);
		CDlgUtil::CtrlShowHide(this, m_aDynaAllow2,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aPicture,     TRUE);
		CDlgUtil::CtrlShowHide(this, m_aList,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aW,           FALSE);
		CDlgUtil::CtrlShowHide(this, m_aDW1DD1,      FALSE);
		CDlgUtil::CtrlShowHide(this, m_aFatigue,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aPedestrian,  FALSE);

		CDlgUtil::CtrlShowHide(this, m_aHeavy,       TRUE);
		CDlgUtil::CtrlShowHide(this, m_aRoad,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aRail,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aBDouble,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aLateralEcc,  TRUE);
	}
	else if(strVehicleType == _T("T44 Truck Load") || 
					strVehicleType == _T("L44 Lane Load") ||
					
					strVehicleType == _T("WA-Group1-Vehicle1") ||
					strVehicleType == _T("WA-Group1-Vehicle2") ||
					strVehicleType == _T("WA-Group1-Vehicle3") ||
					strVehicleType == _T("WA-Group1-Vehicle4"))
	{
		BOOL bLane = (strVehicleType == _T("L44 Lane Load"))? TRUE : FALSE;

		CDlgUtil::CtrlShowHide(this, m_aDynaAllow,   TRUE);
		CDlgUtil::CtrlShowHide(this, m_aDynaAllow2,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aPicture,     TRUE);
		CDlgUtil::CtrlShowHide(this, m_aList,        !bLane);
		CDlgUtil::CtrlShowHide(this, m_aW,           FALSE);
		CDlgUtil::CtrlShowHide(this, m_aDW1DD1,      FALSE);
		CDlgUtil::CtrlShowHide(this, m_aFatigue,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aPedestrian,  FALSE);

		CDlgUtil::CtrlShowHide(this, m_aHeavy,       FALSE);
		CDlgUtil::CtrlShowHide(this, m_aRoad,        bLane);
		CDlgUtil::CtrlShowHide(this, m_aRail,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aBDouble,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aLateralEcc,  FALSE);
	}
	else if (strVehicleType == _T("WA-Group2-Vehicle1 (3.01m O/A)") ||
		strVehicleType == _T("WA-Group2-Vehicle1 (3.70m O/A)") ||
		strVehicleType == _T("WA-Group2-Vehicle2 (3.01m O/A)") ||
		strVehicleType == _T("WA-Group2-Vehicle2 (3.70m O/A)") ||
		strVehicleType == _T("WA-Group2-Vehicle4") ||
		strVehicleType == _T("WA-Group2-Vehicle4 (Non-Supervised)") ||
		strVehicleType == _T("WA-Group2-Vehicle5") ||
		strVehicleType == _T("WA-Group2-Vehicle5 (Non-Supervised)") ||
		strVehicleType == _T("WA-Group2-Vehicle7") ||
		strVehicleType == _T("WA-Group2-Vehicle8"))
		{
			CDlgUtil::CtrlShowHide(this, m_aDynaAllow, TRUE);
			CDlgUtil::CtrlShowHide(this, m_aDynaAllow2, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aPicture, TRUE);
			CDlgUtil::CtrlShowHide(this, m_aList, TRUE);
			CDlgUtil::CtrlShowHide(this, m_aW, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aDW1DD1, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aFatigue, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aPedestrian, FALSE);

			CDlgUtil::CtrlShowHide(this, m_aHeavy, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aRoad, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aRail, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aBDouble, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aLateralEcc, TRUE);
	}
	else if(strVehicleType == _T("300LA") || strVehicleType == _T("150LA"))
	{
		CDlgUtil::CtrlShowHide(this, m_aDynaAllow,   FALSE);
		CDlgUtil::CtrlShowHide(this, m_aDynaAllow2,  TRUE);
		CDlgUtil::CtrlShowHide(this, m_aPicture,     TRUE);
		CDlgUtil::CtrlShowHide(this, m_aList,        TRUE);
		CDlgUtil::CtrlShowHide(this, m_aW,           FALSE);
		CDlgUtil::CtrlShowHide(this, m_aDW1DD1,      FALSE);
		CDlgUtil::CtrlShowHide(this, m_aFatigue,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aPedestrian,  FALSE);

		CDlgUtil::CtrlShowHide(this, m_aHeavy,       FALSE);
		CDlgUtil::CtrlShowHide(this, m_aRoad,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aRail,        TRUE);
		CDlgUtil::CtrlShowHide(this, m_aBDouble,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aLateralEcc,  FALSE);
// 		if(strVehicleType == _T("150LA"))
// 		{
// 			CDlgUtil::CtrlShowHide(this, m_aRailSub,     FALSE);
// 		}
	}
	else if (strVehicleType == _T("VIC-45.5T HML B DOUBLE")         ||
			 strVehicleType == _T("VIC-68T HML B DOUBLE")           ||
		     strVehicleType == _T("QLD-50.5T 8G1 GML 19m B DOUBLE") || 
			 strVehicleType == _T("QLD-50.5T 8G2 GML 19m B DOUBLE") || 
			 strVehicleType == _T("QLD-62.5T 7G GML B DOUBLE")      || 
			 strVehicleType == _T("QLD-68T 7H HML B DOUBLE"))
	{
		CDlgUtil::CtrlShowHide(this, m_aDynaAllow,   FALSE);
		CDlgUtil::CtrlShowHide(this, m_aDynaAllow2,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aPicture,     TRUE);
		CDlgUtil::CtrlShowHide(this, m_aList,        TRUE);
		CDlgUtil::CtrlShowHide(this, m_aW,           FALSE);
		CDlgUtil::CtrlShowHide(this, m_aDW1DD1,      FALSE);
		CDlgUtil::CtrlShowHide(this, m_aFatigue,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aPedestrian,  FALSE);

		CDlgUtil::CtrlShowHide(this, m_aHeavy,       FALSE);
		CDlgUtil::CtrlShowHide(this, m_aRoad,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aRail,        FALSE);
		CDlgUtil::CtrlShowHide(this, m_aBDouble,     TRUE);
		CDlgUtil::CtrlShowHide(this, m_aLateralEcc,  FALSE);
	}
	else 
	{
		ASSERT(0);
	}
}

void CCMMvhlItemStdAustraliaDlg::SetHeaderTitle()
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
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CCMMvhlItemStdAustraliaDlg::InsertItem(int nPos, int nNo, double dLoad, double dDist)
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
			double dDistTmp = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dDist);

// 			if      (dDistTmp ==  0.0) str = _LS(IDS_WG_CMD__ADDD__end);
// 			else if (dDistTmp == -1.0) str = _LS(IDS_WG_CMD_INFINITE);
// 			else                       str.Format(_T("%g"), dDist);

			double dTol = 1.e-10;
			if      (fabs(dDistTmp)       < dTol) str = _LS(IDS_WG_CMD__ADDD__end);
			else if (fabs(dDistTmp + 1.0) < dTol) str = _LS(IDS_WG_CMD_INFINITE);
			else                                  str.Format(_T("%g"), dDist);
		}
		else str = _LS(IDS_WG_CMD__ADDD__Error);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;
		
		if(i == 0)
			m_List.InsertItem(&lvitem);
		else m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	
	return TRUE;
}

void CCMMvhlItemStdAustraliaDlg::MakeItemEx()
{
	m_List.DeleteAllItems();    
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	
	if (nLoadCount > 0)
	{
		if(m_Data.VehicleTypeName == _T("300LA") || m_Data.VehicleTypeName == _T("150LA"))
		{
			int nLoadCountTemp = 5;
			for (int i = 0; i < nLoadCountTemp; i++)
			{
				if(i == nLoadCountTemp-1)
					InsertItem(i, i+1, m_Data.dPointLoad[i], 0.0);
				else
					InsertItem(i, i+1, m_Data.dPointLoad[i], m_Data.dPointDistance[i]);
			}
		}
		else
		{
			int i = 0;
			for (i = 0; i < nDistCount; i++)
				InsertItem(i, i+1, m_Data.dPointLoad[i], m_Data.dPointDistance[i]);

			if (nLoadCount > nDistCount)
				InsertItem(i, i+1, m_Data.dPointLoad[i], 0.0);
		}
	}
	
	UpdateData(FALSE);
}

int CCMMvhlItemStdAustraliaDlg::GetLoadCount()
{
	int i = 0;
	for( i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CCMMvhlItemStdAustraliaDlg::GetDistCount()
{
	int i = 0;
	for( i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointDistance[i] == 0.0) break;
	}  
	return i;
}

void CCMMvhlItemStdAustraliaDlg::ChangeBitmap(int nIndex, int nBitmap)
{
	CString aBitmapID0[]       = { _T("cmd_mvhl_128.svg"), _T("cmd_mvhl_129.svg"), _T("cmd_mvhl_85_AS.svg"), _T("cmd_mvhl_225.svg"), _T("cmd_mvhl_131.svg"), _T("cmd_mvhl_86_AS.svg") };
	CString aBitmapID1[]       = { _T("cmd_mvhl_50_AS.svg"),  _T("cmd_mvhl_50_AS.svg"),  _T("cmd_mvhl_22_AS.svg") };
	CString aBitmapI_2_RAIL[]  = { _T("cmd_mvhl_AS_150LA.svg"), _T("cmd_mvhl_AS_300LA.svg") };  // 150LA, 300LA
	CString aBitmapI_7_ROAD[]  = { _T("cmd_mvhl_AS_T44.svg"), _T("cmd_mvhl_AS_L44.svg"),_T("cmd_mvhl_AS_BDOUBLE_6.svg"),_T("cmd_mvhl_AS_BDOUBLE_9.svg"),_T("cmd_mvhl_AS_BDOUBLE_7.svg"),_T("cmd_mvhl_AS_BDOUBLE_7.svg"),_T("cmd_mvhl_AS_BDOUBLE_9.svg"),_T("cmd_mvhl_AS_BDOUBLE_9.svg"),      // T44, L44
	                               _T("cmd_mvhl_226.svg"),_T("cmd_mvhl_229.svg"),_T("cmd_mvhl_271_ASG1V3.svg"),_T("cmd_mvhl_200_NA_Oregon_STP_4B.svg"), // GROUP1 VEHICLE1 ~  GROUP1 VEHICLE4
																 _T("cmd_mvhl_229.svg"), _T("cmd_mvhl_229.svg"), _T("cmd_mvhl_200_NA_Oregon_STP_4B.svg"), _T("cmd_mvhl_200_NA_Oregon_STP_4B.svg"), _T("cmd_mvhl_231.svg"), _T("cmd_mvhl_205_G2V5.svg"), _T("cmd_mvhl_233.svg"), _T("cmd_mvhl_233.svg") }; // GROUP2 VEHICLE1 ~  GROUP2 VEHICLE8
	CString aBitmapI_7_HEAVY[] = { _T("cmd_mvhl_AS_Heavy.svg"), _T("cmd_mvhl_AS_Heavy.svg") };  // HLP320, HLP400
		
	CString strSVG;
	switch(nIndex)
	{
		case D_MVHL_AUSTRALIA_AS_5100_2: strSVG = aBitmapID0[nBitmap];  break;
		case D_MVHL_AS_5100_2:           strSVG = aBitmapID1[nBitmap];  break;
		case D_MVHL_AS_5100_2_RAIL :     strSVG = aBitmapI_2_RAIL[nBitmap];  break;
		case D_MVHL_AS_5100_7_ROAD :     strSVG = aBitmapI_7_ROAD[nBitmap];  break;
		case D_MVHL_AS_5100_7_HEAVY:     strSVG = aBitmapI_7_HEAVY[nBitmap]; break;
	}

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + strSVG);
}

void CCMMvhlItemStdAustraliaDlg::Data2Dlg()
{
	m_nStndCode = m_Data.nStandardCode;
	if(m_bModify)
	{
		m_dIncreLength = m_Data.dIncreLength;
	}
	if(m_Data.VehicleLoadName.IsEmpty())
		m_edtVehicleName.SetWindowText(m_Data.VehicleTypeName);
	else
		m_edtVehicleName.SetWindowText(m_Data.VehicleLoadName);
	int sIndex;
	CDlgUtil::CobxSetCurSelItemData(m_cmbCodeName, m_nStndCode);
	GetIndex(m_Data.VehicleTypeName, &sIndex);
	m_cmbVehicleType.SetCurSel(sIndex);
	ChangeBitmap(m_nStndCode, sIndex);
	
	m_edtDynaAllow.SetEditUnit(m_Data.dDynLoadAllowance);    
	m_edtDynaAllow_Bend.SetEditUnit(m_Data.dDynLoadAllowance2);
	m_edtDynaAllow_AllOther.SetEditUnit(m_Data.dDynLoadAllowance); // 기존, 
// 	m_edtDynaAllow_Single.SetEditUnit(m_Data.dDynLoadAllowance1);
// 	m_edtDynaAllow_Other.SetEditUnit(m_Data.dDynLoadAllowance2);
	m_edtDynaAllowDouble.SetEditUnit(m_Data.dDynLoadAllowance);

	m_edtLateralEcc.SetEditUnit(m_Data.dLateralEcc);

	CVehlDB db(m_pDoc);  
	if (db.GetStandardVehicleLoadValue(m_Data))	MakeItemEx();

	m_edtW.SetEditUnit(m_Data.dUniformLoad);
	m_edtW1.SetEditUnit(m_Data.dW1);
	m_edtD1.SetEditUnit(m_Data.dD1);

	m_edtPedW1.SetEditUnit(m_Data.dPedW1);
	m_edtPedW3.SetEditUnit(m_Data.dPedW2);

	m_edtPedA1.SetEditUnit(m_Data.dPedA1);
	m_edtPedA2_min.SetEditUnit(m_Data.dPedA1);
	m_edtPedA2_max.SetEditUnit(m_Data.dPedA2);
	m_edtPedA3.SetEditUnit(m_Data.dPedA2);
	m_bFatigue = m_Data.bFatigue;

	// 추가되는 것들을 기존 변수에 저장..
    m_edtHeavyP.SetEditUnit(m_Data.dHeavyP);
	m_edtHeavyD.SetEditUnit(m_Data.dHeavyD);
	m_edtRoadW1.SetEditUnit(m_Data.dRoadW1);
	m_edtRoadW2.SetEditUnit(m_Data.dRoadW2); // user
	m_edtRoadP .SetEditUnit(m_Data.dRoadP);
	m_edtRailInc.SetEditUnit(m_dIncreLength); // user

	m_nDistType = m_Data.nSpaceType;
	m_nDLAType  = m_Data.nDynaFactor;

	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdAustraliaDlg::Dlg2Data()
{
	UpdateData(TRUE);
	
	m_Data.Initialize();  
	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);   
	m_Data.bStandard = TRUE;
	m_cmbVehicleType.GetWindowText(m_Data.VehicleTypeName);

	m_Data.nStandardCode = m_nStndCode;
	m_Data.dIncreLength = m_dIncreLength;

	m_Data.dDynLoadAllowance2 = m_edtDynaAllow_Bend.GetEditValue();
	if(m_Data.VehicleTypeName == _T("300LA") || m_Data.VehicleTypeName == _T("150LA"))
		m_Data.dDynLoadAllowance  = m_edtDynaAllow_AllOther.GetEditValue();
	else
		m_Data.dDynLoadAllowance  = m_edtDynaAllow.GetEditValue();

	// BDouble일 때
	if (m_Data.VehicleTypeName == _T("VIC-45.5T HML B DOUBLE") ||
		m_Data.VehicleTypeName == _T("VIC-68T HML B DOUBLE") ||
		m_Data.VehicleTypeName == _T("QLD-50.5T 8G1 GML 19m B DOUBLE") ||
		m_Data.VehicleTypeName == _T("QLD-50.5T 8G2 GML 19m B DOUBLE") ||
		m_Data.VehicleTypeName == _T("QLD-62.5T 7G GML B DOUBLE") ||
		m_Data.VehicleTypeName == _T("QLD-68T 7H HML B DOUBLE"))
		{
			m_Data.dDynLoadAllowance = m_edtDynaAllowDouble.GetEditValue();
		}

// 	m_Data.dDynLoadAllowance1 = m_edtDynaAllow_Single.GetEditValue();
// 	m_Data.dDynLoadAllowance2 = m_edtDynaAllow_Other.GetEditValue();
	m_Data.bFatigue = m_bFatigue;
	m_Data.dRoadW2       = m_edtRoadW2.GetEditValue(); 
	m_dIncreLength       = m_edtRailInc.GetEditValue();
	m_Data.dIncreLength  = m_dIncreLength;

	m_Data.nSpaceType = m_nDistType;
	m_Data.nDynaFactor = m_nDLAType;

	CVehlDB db(m_pDoc);
	if(!db.GetStandardVehicleLoadValue(m_Data))  return FALSE;  
	
	m_Data.dLateralEcc = m_edtLateralEcc.GetEditValue();

	return TRUE;
}

BOOL CCMMvhlItemStdAustraliaDlg::ApplyOrOK()
{
	if(!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if(m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else          bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);
	if(!bSuccess) return FALSE;
	
	return TRUE;
}

BOOL CCMMvhlItemStdAustraliaDlg::ChangeData()
{
	UpdateData(TRUE);

	m_Data.Initialize();
	m_Data.bStandard = TRUE;
	m_cmbVehicleType.GetWindowText(m_Data.VehicleTypeName);
	m_Data.nStandardCode = m_nStndCode;
	
	int sIndex;
	CDlgUtil::CobxSetCurSelItemData(m_cmbCodeName, m_nStndCode);
	GetIndex(m_Data.VehicleTypeName, &sIndex);
	m_cmbVehicleType.SetCurSel(sIndex);
	ChangeBitmap(m_nStndCode, sIndex);

// 	m_Data.dDynLoadAllowance = m_edtDynaAllow.GetEditValue();
// 	m_Data.dDynLoadAllowance1 = m_edtDynaAllow_Single.GetEditValue();
// 	m_Data.dDynLoadAllowance2 = m_edtDynaAllow_Other.GetEditValue();

	if     (m_Data.VehicleTypeName == _T("W80"))                  m_Data.dDynLoadAllowance = 0.4;
	else if(m_Data.VehicleTypeName == _T("A160"))                 m_Data.dDynLoadAllowance = 0.4;
	else if(m_Data.VehicleTypeName == _T("M1600"))                m_Data.dDynLoadAllowance = 0.3;
	else if(m_Data.VehicleTypeName == _T("M1600 without UDL"))    m_Data.dDynLoadAllowance = 0.3;
	else if(m_Data.VehicleTypeName == _T("M1600 tri-axle group")) m_Data.dDynLoadAllowance = 0.35;
	else if(m_Data.VehicleTypeName == _T("S1600"))                m_Data.dDynLoadAllowance = 0.0;
	else if(m_Data.VehicleTypeName == _T("HLP320"))               m_Data.dDynLoadAllowance = 0.1;
	else if(m_Data.VehicleTypeName == _T("HLP400"))               m_Data.dDynLoadAllowance = 0.1;
	else if(m_Data.VehicleTypeName == _T("T44 Truck Load"))       m_Data.dDynLoadAllowance = 0.4;
	else if(m_Data.VehicleTypeName == _T("L44 Lane Load"))        m_Data.dDynLoadAllowance = 0.4;
	else if(m_Data.VehicleTypeName == _T("300LA"))              { m_Data.dDynLoadAllowance = 0.0; 	m_Data.dDynLoadAllowance2 = 0.0; }
	else if(m_Data.VehicleTypeName == _T("150LA"))              { m_Data.dDynLoadAllowance = 0.0; 	m_Data.dDynLoadAllowance2 = 0.0; }

	else if (m_Data.VehicleTypeName == _T("WA-Group2-Vehicle1 (3.01m O/A)"))        m_Data.dDynLoadAllowance = 0.1;
	else if (m_Data.VehicleTypeName == _T("WA-Group2-Vehicle1 (3.70m O/A)"))        m_Data.dDynLoadAllowance = 0.1;
	else if (m_Data.VehicleTypeName == _T("WA-Group2-Vehicle2 (3.01m O/A)"))        m_Data.dDynLoadAllowance = 0.1;
	else if (m_Data.VehicleTypeName == _T("WA-Group2-Vehicle2 (3.70m O/A)"))        m_Data.dDynLoadAllowance = 0.1;
	else if (m_Data.VehicleTypeName == _T("WA-Group2-Vehicle4"))        m_Data.dDynLoadAllowance = 0.1;
	else if (m_Data.VehicleTypeName == _T("WA-Group2-Vehicle4 (Non-Supervised)"))        m_Data.dDynLoadAllowance = 0.3;
	else if (m_Data.VehicleTypeName == _T("WA-Group2-Vehicle5"))        m_Data.dDynLoadAllowance = 0.1;
	else if (m_Data.VehicleTypeName == _T("WA-Group2-Vehicle5 (Non-Supervised)"))        m_Data.dDynLoadAllowance = 0.3;
	else if (m_Data.VehicleTypeName == _T("WA-Group2-Vehicle7"))        m_Data.dDynLoadAllowance = 0.1;
	else if (m_Data.VehicleTypeName == _T("WA-Group2-Vehicle8"))        m_Data.dDynLoadAllowance = 0.1;

	else                                                      m_Data.dDynLoadAllowance = 0.4;
		

	if(m_Data.VehicleTypeName == _T("L44 Lane Load"))  
	{
		m_Data.dRoadW2 =  m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_UNITFORCE, 12.5);
	}

	CVehlDB db(m_pDoc);
	if(db.GetStandardVehicleLoadValue(m_Data)) MakeItemEx();    

	Data2Dlg();

	m_edtVehicleName.SetWindowText(m_Data.VehicleTypeName);
	
	return TRUE;
}

void CCMMvhlItemStdAustraliaDlg::SetLoadTypeList(int nIndex)
{
	if (nIndex < 0) return;
	
	m_cmbVehicleType.ResetContent();
	int i = 0;
	while (i < D_MAX_COUNT && CVehlDB::GetTypeListAr()[nIndex][i] != _T(""))
	{
		CString strVehicleType = CVehlDB::GetTypeListAr()[nIndex][i++];
// 		if (CTestEnvMgr::GetTestEnvST(_T("TEST_V955")) == _T("yes")) //BDouble
// 		{
// 			// BDouble 차량 보여줌.
// 		}
// 		else
// 		{
// 			// BDouble 차량 skip.
// 			if (strVehicleType == _T("VIC-45.5T HML B DOUBLE") ||
// 				strVehicleType == _T("VIC-68T HML B DOUBLE") ||
// 				strVehicleType == _T("QLD-50.5T 8G1 GML 19m B DOUBLE") ||
// 				strVehicleType == _T("QLD-50.5T 8G2 GML 19m B DOUBLE") ||
// 				strVehicleType == _T("QLD-62.5T 7G GML B DOUBLE") ||
// 				strVehicleType == _T("QLD-68T 7H HML B DOUBLE"))
// 			{
// 				continue;
// 			}
// 		}

		m_cmbVehicleType.AddString(strVehicleType);
	}
	m_cmbVehicleType.SetCurSel(0);
}

void CCMMvhlItemStdAustraliaDlg::GetIndex(CString &csName, int *sitem)
{  
	for (int i = 0; i < D_MAX_COUNT; i++)
	{
		if(CVehlDB::GetTypeListAr()[m_Data.nStandardCode][i] == csName)
		{
			if(sitem) *sitem = i;
			return ;
		}
	}
	
	ASSERT(0);
	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_vehicle_load_name));
}

void CCMMvhlItemStdAustraliaDlg::SetBDoubleFactor()
{
	if (m_nDLAType == 0) // AUTO
	{
		if (m_nDistType == 0) // Slow
		{
			m_edtDynaAllowDouble.SetEditUnit(0.0);
		}
		else if (m_nDistType == 1) // Norma;
		{
			m_edtDynaAllowDouble.SetEditUnit(0.4);
		}
		else
			ASSERT(0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdAustraliaDlg message handlers

BOOL CCMMvhlItemStdAustraliaDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	if(!m_bModify)
	{
		m_Data.Initialize();
		m_Data.bStandard = TRUE;    
		m_Data.VehicleTypeName = _T("W80");
		m_nStndCode = D_MVHL_AUSTRALIA_AS_5100_2;
		m_Data.nStandardCode = D_MVHL_AUSTRALIA_AS_5100_2;
		m_Data.dDynLoadAllowance = 0.4;  // 기존
		//m_Data.dDynLoadAllowance1 = 0.35;
		m_Data.dDynLoadAllowance2 = 0.3;

		double dInLenth = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_NONE, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, 1.0);
		m_dIncreLength = dInLenth;
	}
	GetIndex(m_Data.VehicleTypeName);
	int nIndex = m_Data.nStandardCode;
	if (nIndex < 0) return TRUE;
	
	InitUnit();
	InitCombo();
	SetHeaderTitle();
	SetLoadTypeList(nIndex);
	
	Data2Dlg();
	
	AlignControl();
	ControlsShowHide();
	OnCmdDLATypeRdo();

//	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_mvhl_128.svg"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemStdAustraliaDlg::OnOK() 
{
	if (!ApplyOrOK()) return;	
	CDialogMove::OnOK();
}

void CCMMvhlItemStdAustraliaDlg::OnCmdMvhlCodeCmb() 
{
	int nCodeType = CDlgUtil::CobxGetCurSelItemData(m_cmbCodeName, m_cmbCodeName.GetCurSel());
	SetLoadTypeList(nCodeType);

	int nIndex = m_cmbCodeName.GetCurSel();
	m_nStndCode = m_cmbCodeName.GetItemData(nIndex);

	ChangeData();

	AlignControl();
	ControlsShowHide();
}

void CCMMvhlItemStdAustraliaDlg::OnCmdMvhlTypeCmb() 
{
	ChangeData();  

	AlignControl();
	ControlsShowHide();

	OnCmdDLATypeRdo();
}

void CCMMvhlItemStdAustraliaDlg::OnCmdApply() 
{	
	ApplyOrOK();
}

void CCMMvhlItemStdAustraliaDlg::OnCmdDistTypeRdo()
{
	UpdateData(TRUE);

	m_nDLAType = 0; // Auto 로 세팅

	OnCmdDLATypeRdo();
}
void CCMMvhlItemStdAustraliaDlg::OnCmdDLATypeRdo()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_MVHL_BDOUBLE_DLA_EDT)->EnableWindow(m_nDLAType == 1); // User

	SetBDoubleFactor();

	UpdateData(FALSE);
}

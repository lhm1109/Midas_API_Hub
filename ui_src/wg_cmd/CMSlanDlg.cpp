// CMSlanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSlanDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_I_GridColorMgr.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_FlagCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_StagCtrl.h"

#include "..\wg_base\TestEnvMgr.h"

extern double GetInitialWheelSpacing(int nLaneType);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
#if defined(_US) || defined(_ORG)
	#define D_NUM_COL     4
#else
	#define D_NUM_COL     3
#endif
	*/
#define D_NUM_COL       6

/////////////////////////////////////////////////////////////////////////////
// CCMSlanDlg dialog


CCMSlanDlg::CCMSlanDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMSlanDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSlanDlg)
	m_nMovDir = -1;
	m_nSelectionMethod = 0;
	m_nImpactorSpan = 1;
	//}}AFX_DATA_INIT
	m_bModify = FALSE;
	m_bListMode = TRUE;
	m_bCodeNameINDIA = FALSE;

	m_aCtrlList.Add(IDC_CMD_LIST);
	m_aCtrlList.Add(IDC_CMD_BTN_ADD);
	m_aCtrlList.Add(IDC_CMD_BTN_MODIFY);
	m_aCtrlList.Add(IDC_CMD_BTN_DELETE);
	m_aCtrlList.Add(IDC_CMD_BTN_COPY);
	m_aCtrlList.Add(IDC_CMD_BTN_CLOSE);

	m_aCtrlData.Add(IDC_CMD_LANE_NAME_TITLE);
	m_aCtrlData.Add(IDC_CMD_LANE_NAME_EDIT);
	m_aCtrlData.Add(IDC_CMD_FRAME1);
	m_aCtrlData.Add(IDC_CMD_OFFSET_TITLE1);
	m_aCtrlData.Add(IDC_CMD_OFFSET_TITLE2);
	m_aCtrlData.Add(IDC_CMD_OFFSET_EDIT);
	m_aCtrlData.Add(IDC_CMD_OFFSET_UNIT);
	m_aCtrlData.Add(IDC_CMD_LWIDTH_TITLE);
	m_aCtrlData.Add(IDC_CMD_LWIDTH_EDIT);
	m_aCtrlData.Add(IDC_CMD_LWIDTH_UNIT);
	m_aCtrlData.Add(IDC_CMD_FACTOR_TITLE);
	m_aCtrlData.Add(IDC_CMD_FACTOR_EDIT);
	//m_aCtrlData.Add(IDC_CMD_SELECT_TITLE);
	m_aCtrlData.Add(IDC_CMD_FRAME7);
	m_aCtrlData.Add(IDC_CMD_2POINT_UNIT2);
	m_aCtrlData.Add(IDC_CMD_2POINT_UNIT3);

	m_aCtrlData.Add(IDC_CMD_2POINTS);
	m_aCtrlData.Add(IDC_CMD_2POINTS_EDIT1);
	m_aCtrlData.Add(IDC_CMD_2POINTS_EDIT2);
	m_aCtrlData.Add(IDC_CMD_PICKING);
	m_aCtrlData.Add(IDC_CMD_PICKING_EDIT);
	m_aCtrlData.Add(IDC_CMD_FRAME2);
	m_aCtrlData.Add(IDC_CMD_OP_ADD);
	m_aCtrlData.Add(IDC_CMD_OP_INSERT);
	m_aCtrlData.Add(IDC_CMD_OP_DELETE);
	m_aCtrlData.Add(IDC_CMD_PLACEHOLDER);
	m_aCtrlData.Add(IDC_CMD_BTN_OK);
	m_aCtrlData.Add(IDC_CMD_BTN_CANCEL);
	m_aCtrlData.Add(IDC_CMD_BTN_APPLY);
	m_aCtrlData.Add(IDC_CMD_NODESELECT);
	m_aCtrlData.Add(IDC_CMD_NODESELECT_EDIT);
	m_aCtrlData.Add(IDC_CMD_STATIC_G   );
	m_aCtrlData.Add(IDC_CMD_START_TITLE);
	m_aCtrlData.Add(IDC_CMD_END_TITLE  );
	m_aCtrlData.Add(IDC_CMD_SKEW_START_EDIT);
	m_aCtrlData.Add(IDC_CMD_SKEW_END_EDIT);
	m_aCtrlData.Add(IDC_CMD_START_UNIT);
	m_aCtrlData.Add(IDC_CMD_END_UNIT);
	m_aCtrlData.Add(IDC_CMD_BMP_STATIC);
	m_aCtrlData.Add(IDC_CMD_STATIC_G2 );
	m_aCtrlData.Add(IDC_CMD_ST_MOVINGDIR);
	m_aCtrlData.Add(IDC_CMD_MOV_FORWARD );
	m_aCtrlData.Add(IDC_CMD_MOV_BACKWARD);
	m_aCtrlData.Add(IDC_CMD_MOV_BOTH    );
	
	// Wheel Spacing
	m_aCtrlData.Add(IDC_CMD_WHEEL_TXT);
	m_aCtrlData.Add(IDC_CMD_WHEEL_EDT);
	m_aCtrlData.Add(IDC_CMD_WHEEL_UNT);

	// Lane Optimization
	m_aCtrlData.Add(IDC_CMD_LANE_OPTIM_CHK);
	m_aCtrlData.Add(IDC_CMD_LANE_OPTIM_GRP);
	m_aCtrlData.Add(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_STC);
	m_aCtrlData.Add(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_EDT);
	m_aCtrlData.Add(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_UNT);

	// Eccentricity of Vertical Load to Consider Cant
	m_aCtrlData.Add(IDC_CMD_ECCEN_VERT_LOAD_STC);
	m_aCtrlData.Add(IDC_CMD_ECCEN_VERT_LOAD_STC2);
	m_aCtrlData.Add(IDC_CMD_ECCEN_VERT_LOAD_EDT2);
	m_aCtrlData.Add(IDC_CMD_ECCEN_VERT_LOAD_UNT);

	// Centrifugal Force
	m_aCtrlCF.RemoveAll();
	m_aCtrlCF.Add(IDC_CMD_LANE_CF_GRP);
	m_aCtrlCF.Add(IDC_CMD_LANE_CF_STC);
	m_aCtrlCF.Add(IDC_CMD_LANE_CF_STC2);
	m_aCtrlCF.Add(IDC_CMD_LANE_CF_STC3);
	m_aCtrlCF.Add(IDC_CMD_LANE_CF_EDT);
	m_aCtrlData.Append(m_aCtrlCF);

	m_aCtrlLaneTypeRdo.RemoveAll();
	m_aCtrlLaneTypeRdo.Add(IDC_CMD_ROAD_LANE_RDO);
	m_aCtrlLaneTypeRdo.Add(IDC_CMD_RAIL_LANE_RDO);

	m_aCtrlLaneType.RemoveAll();
	m_aCtrlLaneType.Add(IDC_CMD_LANE_TYPE_GRP);
	m_aCtrlLaneType.Append(m_aCtrlLaneTypeRdo);
	m_aCtrlData.Append(m_aCtrlLaneType);

	m_aCtrlSelect.Add(IDC_CMD_2POINTS);
	m_aCtrlSelect.Add(IDC_CMD_PICKING);
	m_aCtrlSelect.Add(IDC_CMD_NODESELECT);

	m_aLaneWidth.Add(IDC_CMD_LWIDTH_TITLE);
	m_aLaneWidth.Add(IDC_CMD_LWIDTH_EDIT);
	m_aLaneWidth.Add(IDC_CMD_LWIDTH_UNIT);

	m_aWheelSpacing.Add(IDC_CMD_WHEEL_TXT);
	m_aWheelSpacing.Add(IDC_CMD_WHEEL_EDT);
	m_aWheelSpacing.Add(IDC_CMD_WHEEL_UNT);

	m_aOffset.Add(IDC_CMD_OFFSET_TITLE1);
	m_aOffset.Add(IDC_CMD_OFFSET_TITLE2);
	m_aOffset.Add(IDC_CMD_OFFSET_EDIT);
	m_aOffset.Add(IDC_CMD_OFFSET_UNIT);

	m_aImpactFactor.Add(IDC_CMD_FACTOR_TITLE);
	m_aImpactFactor.Add(IDC_CMD_FACTOR_EDIT);

	m_aImpactorSpan.Add(IDC_CMD_SLAN_IMPACT_FACTOR_RDO);
	m_aImpactorSpan.Add(IDC_CMD_SLAN_SPAN_LENGTH_RDO);
	m_aImpactorSpan.Add(IDC_CMD_SLAN_IMPACT_FACTOR_EDT);
	m_aImpactorSpan.Add(IDC_CMD_SLAN_SPAN_LENGTH_EDT);
	m_aImpactorSpan.Add(IDC_CMD_SLAN_SPAN_LENGTH_UNT);
}


void CCMSlanDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSlanDlg)
	DDX_Control(pDX, IDC_CMD_WHEEL_UNT, m_untWheelSpace);
	DDX_Control(pDX, IDC_CMD_WHEEL_EDT, m_edtWheelSpace);
	DDX_Control(pDX, IDC_CMD_2POINT_UNIT3, m_2PointUnit2);
	DDX_Control(pDX, IDC_CMD_2POINT_UNIT2, m_2PointUnit1);
	DDX_Control(pDX, IDC_CMD_SKEW_START_EDIT, m_edtSkewStart);
	DDX_Control(pDX, IDC_CMD_SKEW_END_EDIT, m_edtSkewEnd);
	DDX_Control(pDX, IDC_CMD_NODESELECT_EDIT, m_wndNodeSelectEdit);
	DDX_Control(pDX, IDC_CMD_FACTOR_EDIT, m_wndFactor);
	DDX_Control(pDX, IDC_CMD_LANE_CF_EDT, m_wndCFEdit);
	DDX_Control(pDX, IDC_CMD_PICKING_EDIT, m_wndPickingEdit);
	DDX_Control(pDX, IDC_CMD_2POINTS_EDIT2, m_wnd2PointsEdit2);
	DDX_Control(pDX, IDC_CMD_2POINTS_EDIT1, m_wnd2PointsEdit1);
	DDX_Control(pDX, IDC_CMD_OFFSET_EDIT, m_wndOffsetEdit);
	DDX_Control(pDX, IDC_CMD_OFFSET_UNIT, m_wndOffsetUnit);
	DDX_Control(pDX, IDC_CMD_LWIDTH_EDIT, m_wndLWidthEdit);
	DDX_Control(pDX, IDC_CMD_LWIDTH_UNIT, m_wndLWidthUnit);
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_LANE_NAME_EDIT, m_wndLaneName);
	DDX_Radio  (pDX, IDC_CMD_MOV_FORWARD, m_nMovDir);
	DDX_Radio  (pDX, IDC_CMD_2POINTS, m_nSelectionMethod);

	DDX_Radio  (pDX, IDC_CMD_SLAN_IMPACT_FACTOR_RDO, m_nImpactorSpan);
	DDX_Control(pDX, IDC_CMD_SLAN_IMPACT_FACTOR_EDT, m_wndImpactFactorEdt);
	DDX_Control(pDX, IDC_CMD_SLAN_SPAN_LENGTH_EDT,   m_wndSpanLengthEdt);
	DDX_Control(pDX, IDC_CMD_SLAN_SPAN_LENGTH_UNT,   m_wndSpanLengthUnt);
	DDX_Control(pDX, IDC_CMD_ECCEN_VERT_LOAD_EDT2,    m_edtEccenVertLoad);
	DDX_Control(pDX, IDC_CMD_ECCEN_VERT_LOAD_UNT,    m_untEccenVertLoad);
	DDX_Control(pDX, IDC_CMD_LANE_OPTIM_CHK, m_chkLaneOptim);
	DDX_Control(pDX, IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_EDT, m_edtAllowWidth);
	DDX_Control(pDX, IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_UNT, m_untAllowWidth);

	DDX_Control(pDX, IDC_CMD_PLACEHOLDER, m_wndGrid);
	DDX_Control(pDX, IDC_CMD_BMP_STATIC, m_ImgViewer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSlanDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMSlanDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD,    OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE,  OnCmdBtnClose)
	ON_BN_CLICKED(IDC_CMD_BTN_CANCEL, OnCmdBtnCancel)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CMD_2POINTS,     OnCmdChangeSelectBy)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY,  OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE,  OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_OP_ADD,      OnCmdOpAdd)
	ON_BN_CLICKED(IDC_CMD_OP_INSERT,   OnCmdOpInsert)
	ON_BN_CLICKED(IDC_CMD_OP_DELETE,   OnCmdOpDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_OK,      OnCmdBtnOk)
	ON_BN_CLICKED(IDC_CMD_BTN_APPLY,   OnCmdBtnApply)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	ON_BN_CLICKED(IDC_CMD_PICKING,    OnCmdChangeSelectBy)
	ON_BN_CLICKED(IDC_CMD_NODESELECT, OnCmdChangeSelectBy)
	ON_BN_CLICKED(IDC_CMD_BTN_COPY,   OnCmdBtnCopy)
	ON_BN_CLICKED(IDC_CMD_SLAN_IMPACT_FACTOR_RDO, OnCmdImpactorSpanRdo)
	ON_BN_CLICKED(IDC_CMD_SLAN_SPAN_LENGTH_RDO,   OnCmdImpactorSpanRdo)
	ON_BN_CLICKED(IDC_CMD_LANE_OPTIM_CHK, OnCmdChangeOptimChk)
	ON_BN_CLICKED(IDC_CMD_ROAD_LANE_RDO, OnSelLaneTypeRdo)
	ON_BN_CLICKED(IDC_CMD_RAIL_LANE_RDO, OnSelLaneTypeRdo)

	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSlanDlg message handlers
#define COLCOUNT 1

void CCMSlanDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistX=0;
	int nDistY=0;

	GetDlgItem(IDC_CMD_LIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_LANE_NAME_EDIT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlData, nDistY);

	// 일단 움직여 놓고...
	GetDlgItem(IDC_CMD_FACTOR_TITLE)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_SLAN_IMPACT_FACTOR_RDO)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aImpactorSpan, nDistY);

	if(m_bCodeNameINDIA)
	{
		GetDlgItem(IDC_CMD_OFFSET_TITLE2)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_SLAN_IMPACT_FACTOR_RDO)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(7);
		CDlgUtil::CtrlMoveDistY(this, m_aImpactorSpan, nDistY);

		CDlgUtil::CtrlShowHide(this, m_aImpactFactor, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aImpactorSpan, TRUE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aImpactorSpan, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aImpactorSpan, FALSE);
	}

	// 기준 별 Dlg Controls 위치 조정
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	T_MVCD_D MvcdData; MvcdData.Initialize();
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();
	int nCode = MvcdData.nCodeType;

	CArray<UINT, UINT> aControls;

	if (nCode == D_MOVE_CODE_BS)
	{
		GetDlgItem(IDC_CMD_LWIDTH_TITLE)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_LANE_TYPE_GRP)->GetWindowRect(rToMove);

		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistXY(this, m_aCtrlLaneType, nDistX, nDistY);

		aControls.RemoveAll();
		aControls.Add(IDC_CMD_LWIDTH_TITLE);
		aControls.Add(IDC_CMD_LWIDTH_EDIT);
		aControls.Add(IDC_CMD_LWIDTH_UNIT);
		aControls.Add(IDC_CMD_OFFSET_TITLE1);
		aControls.Add(IDC_CMD_OFFSET_TITLE2);
		aControls.Add(IDC_CMD_OFFSET_EDIT);
		aControls.Add(IDC_CMD_OFFSET_UNIT);
		aControls.Add(IDC_CMD_WHEEL_TXT);
		aControls.Add(IDC_CMD_WHEEL_EDT);
		aControls.Add(IDC_CMD_WHEEL_UNT);
		aControls.Add(IDC_CMD_ECCEN_VERT_LOAD_STC);
		aControls.Add(IDC_CMD_ECCEN_VERT_LOAD_STC2);
		aControls.Add(IDC_CMD_ECCEN_VERT_LOAD_EDT2);
		aControls.Add(IDC_CMD_ECCEN_VERT_LOAD_UNT);

		GetDlgItem(IDC_CMD_LANE_TYPE_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_LWIDTH_TITLE)->GetWindowRect(rToMove);
		//nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(7);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
	}

	// Centrifugal Force
// 	if(nCode == D_MOVE_CODE_AASHTO_STAN)
// 	{
// // 		CRect rRef2;
// // 		CRect rToMove2;
// // 		GetDlgItem(IDC_CMD_WHEEL_TXT)->GetWindowRect(rRef2);
// // 		GetDlgItem(IDC_CMD_FACTOR_TITLE)->GetWindowRect(rToMove2);
// // 		int nDistY2 = rRef2.bottom - rToMove2.top;
// 
// 		GetDlgItem(IDC_CMD_FACTOR_EDIT)->GetWindowRect(rRef);
// 		GetDlgItem(IDC_CMD_LANE_CF_GRP)->GetWindowRect(rToMove);
// 		//nDistX = rRef.left - rToMove.left;
// 		nDistY = rRef.bottom - rToMove.top +7;
// 		CDlgUtil::CtrlMoveDistXY(this, m_aCtrlCF, nDistX, nDistY);
// 
// 	}
	if(nCode == D_MOVE_CODE_AASHTO_LRFD || nCode == D_MOVE_CODE_CANADA)
	{
// 		CRect rRef2;
// 		CRect rToMove2;
// 		GetDlgItem(IDC_CMD_WHEEL_TXT)->GetWindowRect(rRef2);
// 		GetDlgItem(IDC_CMD_FACTOR_TITLE)->GetWindowRect(rToMove2);
// 		int nDistY2 = rRef2.bottom - rToMove2.top;

		GetDlgItem(IDC_CMD_OFFSET_TITLE2)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_LANE_CF_GRP)->GetWindowRect(rToMove);
		//nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top +globalUtils.ScaleByDPI(7);
		CDlgUtil::CtrlMoveDistXY(this, m_aCtrlCF, nDistX, nDistY);
	}

	// Traffic Lane Properties
	CRect rectResize;
	GetDlgItem(IDC_CMD_STATIC_G2)->GetWindowRect(rectResize);
	
	if(nCode == D_MOVE_CODE_AASHTO_STAN || nCode == D_MOVE_CODE_TAIWAN ||
		 nCode == D_MOVE_CODE_KOREA || nCode == D_MOVE_CODE_BRAZIL)
	{
		// Skew
		aControls.RemoveAll();
		aControls.Add(IDC_CMD_STATIC_G);
		aControls.Add(IDC_CMD_START_TITLE);
		aControls.Add(IDC_CMD_END_TITLE);
		aControls.Add(IDC_CMD_SKEW_START_EDIT);
		aControls.Add(IDC_CMD_SKEW_END_EDIT);
		aControls.Add(IDC_CMD_START_UNIT);
		aControls.Add(IDC_CMD_END_UNIT);

		GetDlgItem(IDC_CMD_FACTOR_EDIT)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_STATIC_G)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(7);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

		GetDlgItem(IDC_CMD_STATIC_G)->GetWindowRect(rRef);
		rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	}
	else if(nCode == D_MOVE_CODE_AASHTO_LRFD || nCode == D_MOVE_CODE_CANADA ||
		      nCode == D_MOVE_CODE_PENDOT      || /*nCode == D_MOVE_CODE_BS     ||*/
					nCode == D_MOVE_CODE_RUSSIA      || nCode == D_MOVE_CODE_KOREA_LRFD_2011 ||
					nCode == D_MOVE_CODE_AUSTRALIA   || nCode == D_MOVE_CODE_POLAND ||
					nCode == D_MOVE_CODE_SOUTH_AFRICA|| nCode == D_MOVE_CODE_NEWZEALAND ||
					nCode == D_MOVE_CODE_BRAZIL)
	{
		// Skew
		aControls.RemoveAll();
		aControls.Add(IDC_CMD_STATIC_G);
		aControls.Add(IDC_CMD_START_TITLE);
		aControls.Add(IDC_CMD_END_TITLE);
		aControls.Add(IDC_CMD_SKEW_START_EDIT);
		aControls.Add(IDC_CMD_SKEW_END_EDIT);
		aControls.Add(IDC_CMD_START_UNIT);
		aControls.Add(IDC_CMD_END_UNIT);

		if(nCode == D_MOVE_CODE_AASHTO_LRFD || nCode == D_MOVE_CODE_CANADA)
			GetDlgItem(IDC_CMD_LANE_CF_GRP)->GetWindowRect(rRef);
		else
			GetDlgItem(IDC_CMD_OFFSET_TITLE2)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_STATIC_G)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(7);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
		
		GetDlgItem(IDC_CMD_STATIC_G)->GetWindowRect(rRef);
		rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	}
	else if (nCode == D_MOVE_CODE_BS)
	{

		// Eccentricity of Vertical Load to Consider Cant
		aControls.RemoveAll();
		aControls.Add(IDC_CMD_ECCEN_VERT_LOAD_STC);
		aControls.Add(IDC_CMD_ECCEN_VERT_LOAD_STC2);
		aControls.Add(IDC_CMD_ECCEN_VERT_LOAD_EDT2);
		aControls.Add(IDC_CMD_ECCEN_VERT_LOAD_UNT);

		GetDlgItem(IDC_CMD_OFFSET_EDIT)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_ECCEN_VERT_LOAD_EDT2)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top + globalUtils.ScaleByDPI(34);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

		// Skew
		aControls.RemoveAll();
		aControls.Add(IDC_CMD_STATIC_G);
		aControls.Add(IDC_CMD_START_TITLE);
		aControls.Add(IDC_CMD_END_TITLE);
		aControls.Add(IDC_CMD_SKEW_START_EDIT);
		aControls.Add(IDC_CMD_SKEW_END_EDIT);
		aControls.Add(IDC_CMD_START_UNIT);
		aControls.Add(IDC_CMD_END_UNIT);

		GetDlgItem(IDC_CMD_ECCEN_VERT_LOAD_EDT2)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_STATIC_G)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(7);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

		GetDlgItem(IDC_CMD_STATIC_G)->GetWindowRect(rRef);
		rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	}
	else if(nCode == D_MOVE_CODE_INDIA)
	{
		// Skew
		aControls.RemoveAll();
		aControls.Add(IDC_CMD_STATIC_G);
		aControls.Add(IDC_CMD_START_TITLE);
		aControls.Add(IDC_CMD_END_TITLE);
		aControls.Add(IDC_CMD_SKEW_START_EDIT);
		aControls.Add(IDC_CMD_SKEW_END_EDIT);
		aControls.Add(IDC_CMD_START_UNIT);
		aControls.Add(IDC_CMD_END_UNIT);

		GetDlgItem(IDC_CMD_SLAN_SPAN_LENGTH_EDT)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_STATIC_G)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(7);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
		
		GetDlgItem(IDC_CMD_STATIC_G)->GetWindowRect(rRef);
		rectResize.bottom = rRef.bottom + 8;
	}
	else if(nCode == D_MOVE_CODE_EURO_BS)
	{
		// Eccentricity of Vertical Load to Consider Cant
		aControls.RemoveAll();
		aControls.Add(IDC_CMD_ECCEN_VERT_LOAD_STC);
		aControls.Add(IDC_CMD_ECCEN_VERT_LOAD_STC2);
		aControls.Add(IDC_CMD_ECCEN_VERT_LOAD_EDT2);
		aControls.Add(IDC_CMD_ECCEN_VERT_LOAD_UNT);
	
		GetDlgItem(IDC_CMD_OFFSET_EDIT)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_ECCEN_VERT_LOAD_EDT2)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top + globalUtils.ScaleByDPI(34);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
		
		// Skew
		aControls.RemoveAll();
		aControls.Add(IDC_CMD_STATIC_G);
		aControls.Add(IDC_CMD_START_TITLE);
		aControls.Add(IDC_CMD_END_TITLE);
		aControls.Add(IDC_CMD_SKEW_START_EDIT);
		aControls.Add(IDC_CMD_SKEW_END_EDIT);
		aControls.Add(IDC_CMD_START_UNIT);
		aControls.Add(IDC_CMD_END_UNIT);
		
		GetDlgItem(IDC_CMD_ECCEN_VERT_LOAD_EDT2)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_STATIC_G)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(7);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
		
		GetDlgItem(IDC_CMD_STATIC_G)->GetWindowRect(rRef);
		rectResize.bottom = rRef.bottom + 8;
	}
	ScreenToClient(rectResize);
	GetDlgItem(IDC_CMD_STATIC_G2)->MoveWindow(rectResize);

	// Lane Optimization
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_LANE_OPTIM_CHK);
	aControls.Add(IDC_CMD_LANE_OPTIM_GRP);
	aControls.Add(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_STC);
	aControls.Add(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_EDT);
	aControls.Add(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_UNT);

	GetDlgItem(IDC_CMD_STATIC_G2)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_LANE_OPTIM_CHK)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// Moving Direction
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_ST_MOVINGDIR);
	aControls.Add(IDC_CMD_MOV_FORWARD);
	aControls.Add(IDC_CMD_MOV_BACKWARD);
	aControls.Add(IDC_CMD_MOV_BOTH);

	//GetDlgItem(IDC_CMD_STATIC_G2)->GetWindowRect(rRef);
	if(IsUseOptim(nCode)) GetDlgItem(IDC_CMD_LANE_OPTIM_GRP)->GetWindowRect(rRef); 
	else                  GetDlgItem(IDC_CMD_STATIC_G2)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_ST_MOVINGDIR)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
	
	// Selection by
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_FRAME7);
	aControls.Add(IDC_CMD_2POINTS);
	aControls.Add(IDC_CMD_PICKING);
	aControls.Add(IDC_CMD_NODESELECT);
	aControls.Add(IDC_CMD_2POINTS_EDIT1);
	aControls.Add(IDC_CMD_2POINT_UNIT2);
	aControls.Add(IDC_CMD_2POINTS_EDIT2);
	aControls.Add(IDC_CMD_2POINT_UNIT3);
	
// MNET:XXXX-8851-20141015. LRFD 2011 코드에서 컨트롤 겹치는 문제 수정 (Selection과 Moving Dir이 겹침)
// 	if(nCode == D_MOVE_CODE_BS      || nCode == D_MOVE_CODE_EURO_BS || 
// 		 nCode == D_MOVE_CODE_RUSSIA  || nCode == D_MOVE_CODE_KOREA_LRFD_2011)

	if(nCode == D_MOVE_CODE_EURO_BS || nCode == D_MOVE_CODE_RUSSIA)
	{
		GetDlgItem(IDC_CMD_LANE_OPTIM_GRP)->GetWindowRect(rRef);
	}
	else
	{
		GetDlgItem(IDC_CMD_ST_MOVINGDIR)->GetWindowRect(rRef); 
	}
			
	GetDlgItem(IDC_CMD_FRAME7)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// Operations
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_FRAME2);
	aControls.Add(IDC_CMD_OP_ADD);
	aControls.Add(IDC_CMD_OP_INSERT);
	aControls.Add(IDC_CMD_OP_DELETE);
	
	GetDlgItem(IDC_CMD_FRAME7)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_FRAME2)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// Grid
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_PLACEHOLDER);
	
	GetDlgItem(IDC_CMD_FRAME2)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_PLACEHOLDER)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
	
	// OK, Cancel, Apply
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_BTN_OK);
	aControls.Add(IDC_CMD_BTN_CANCEL);
	aControls.Add(IDC_CMD_BTN_APPLY);
	
	GetDlgItem(IDC_CMD_PLACEHOLDER)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_BTN_CANCEL)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	ResizeDialog();
}

void CCMSlanDlg::ResizeDialog()
{
	UINT nLastCtrlID;
	if (m_bListMode) nLastCtrlID = IDC_CMD_BTN_CLOSE;
	else nLastCtrlID = IDC_CMD_BTN_CANCEL;
	CWnd* pWndLast = GetDlgItem(nLastCtrlID);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);

	MoveWindow(r);
}

void CCMSlanDlg::SetHeaderTitle()
{
	CString aTitle[] = { _LS(IDS_CMD_MVLD_lane_name) };
//18japan  CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Lane_Name)};
	int nColWidth[COLCOUNT];

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 140;

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMSlanDlg::InitGrid()
{
	m_wndGrid.Initialize();
	I_GridColorMgr::ApplyColorStyle(&m_wndGrid);

	CGXGridParam* pParam = m_wndGrid.GetParam();
	ASSERT_VALID(pParam);

	CGXProperties* pProp = pParam->GetProperties();
	ASSERT_VALID(pProp);

	pParam->EnableUndo(FALSE);

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	// Row and column headers
	m_wndGrid.ColHeaderStyle().SetReadOnly(FALSE).SetEnabled(FALSE)
														.SetFont(CGXFont_GC().SetBold(FALSE));
	m_wndGrid.RowHeaderStyle().SetEnabled(FALSE);
	m_wndGrid.ChangeStandardStyle(CGXStyle().SetFont(CGXFont_GC().SetSize(9))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetAllowEnter(FALSE));

	// Special grid settings
	pParam->EnableMoveRows(TRUE);                  // rows cannot be dragged
	pParam->EnableMoveCols(FALSE);                 // columns can be dragged
	//pParam->EnableTrackColWidth(FALSE);             // disable resizing
	pParam->EnableTrackRowHeight(FALSE);
	pParam->EnableThumbTrack(FALSE);               // thumb-tracking is disabled
	pParam->SetNumberedColHeaders(FALSE);
	// Excel과 같은 셀 선택 : DoLButtonDblClk 참조
	pParam->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);
	pParam->SetHideCurrentCell(GX_HIDE_NEVER);

	// removing rows is not undoable for this kind of grid
	pParam->SetRemoveColsFlags(FALSE);
	pParam->SetRemoveRowsFlags(FALSE);

	pProp->SetMarkColHeader(FALSE);    // Turn off pressed button effect for column headers
	pProp->SetPrintRowHeaders(FALSE);  // Don't print column headers

	m_wndGrid.SetRowHeightByDPI(0, 0, 32);
	m_wndGrid.SetDrawingTechnique(gxDrawUsingMemDC);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	if(m_bCodeNameINDIA)
	{
		m_wndGrid.SetColCount(D_NUM_COL);
		SetGridTitle();
		m_wndGrid.SetColWidth(0, 0, 25);
		m_wndGrid.SetColWidth(1, 1, 35);
		m_wndGrid.SetColWidth(2, 2, 55);
		m_wndGrid.SetColWidth(3, 3, 50);
		m_wndGrid.SetColWidth(4, 4, 40);
		m_wndGrid.SetColWidth(5, 5, 0);
		m_wndGrid.SetColWidth(6, 6, 40);
		// Check Box Column의 스타일 변경
		m_wndGrid.SetStyleRange(CGXRange().SetCols(6), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D)
			.SetValueType(GX_VT_NUMERIC)
			.SetHorizontalAlignment(DT_CENTER)
			.SetVerticalAlignment(DT_VCENTER)
		);
	}
	else
	{
		m_wndGrid.SetColCount(D_NUM_COL);
		SetGridTitle();
		m_wndGrid.SetColWidthByDPI(0, 0, 25);
		m_wndGrid.SetColWidthByDPI(1, 1, 35);
		m_wndGrid.SetColWidthByDPI(2, 2, 55);
		m_wndGrid.SetColWidthByDPI(3, 3, 55);
		if (IsUseSpanStart()) m_wndGrid.SetColWidthByDPI(4, 4, 40);
		// Check Box Column의 스타일 변경
		m_wndGrid.SetStyleRange(CGXRange().SetCols(4), CGXStyle()
				.SetControl(GX_IDS_CTRL_CHECKBOX3D)
				.SetValueType(GX_VT_NUMERIC)
				.SetHorizontalAlignment(DT_CENTER)
				.SetVerticalAlignment(DT_VCENTER)
				);
		if(IsUseEccenVertLoad()) m_wndGrid.SetColWidthByDPI(5, 5, 80);
		if(IsUseCentrifugalForce()) m_wndGrid.SetColWidthByDPI(6, 6, 35);
	}
	
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMSlanDlg::SetGridTitle()
{
	CString csUnit;
	m_pDoc->m_pUnitCtrl->GetUnitSystem(CUnitCtrl::m_SLAN_UNIT.Offset, csUnit); //나중에 물어봐야지.. 
																			 // Offset 단위를 기본으로 잡아도 되는지 	
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	if(m_bCodeNameINDIA)
	{
		m_wndGrid.SetValueRange(CGXRange(0, 0), _LS(IDS_CMD_SLAN_no));
		m_wndGrid.SetValueRange(CGXRange(0, 1), _LS(IDS_CMD_SLAN_node));
		m_wndGrid.SetValueRange(CGXRange(0, 2), CString(_LS(IDS_CMD_SLAN_unit1))+csUnit+_T(")"));
		m_wndGrid.SetValueRange(CGXRange(0, 3), CString(_T("Span\n("))+csUnit+_T(")"));
		m_wndGrid.SetValueRange(CGXRange(0, 4), _T("IF/\nCDA")); //_LS(IDS_CMD_SLAN_IF_CDA));
		m_wndGrid.SetValueRange(CGXRange(0, 5), _T("Index"));
		m_wndGrid.SetValueRange(CGXRange(0, 6), _T("EJ"));
	}
	else
	{
		m_wndGrid.SetValueRange(CGXRange(0, 0), _LS(IDS_CMD_SLAN_no));
		m_wndGrid.SetValueRange(CGXRange(0, 1), _LS(IDS_CMD_SLAN_node));
		m_wndGrid.SetValueRange(CGXRange(0, 2), CString(_LS(IDS_CMD_SLAN_unit1))+csUnit+_T(")"));
		m_wndGrid.SetValueRange(CGXRange(0, 3), _LS(IDS_CMD_SLAN_impact_factor));
		if(IsUseSpanStart()) m_wndGrid.SetValueRange(CGXRange(0, 4), _LS(IDS_CMD_SLAN_SPAN_START));
		if(IsUseEccenVertLoad()) m_wndGrid.SetValueRange(CGXRange(0, 5), _LS(IDS_WG_CMD__LANE_ECCEN_VERT_LOAD) + csUnit + _T(")") );
		if(IsUseCentrifugalForce()) m_wndGrid.SetValueRange(CGXRange(0, 6), _LS(IDS_WG_CMD__ADDD__Centrifugal_Force));
	}

//18japan  m_wndGrid.SetValueRange(CGXRange(0, 0), _LS(IDS_WG_CMD__ADDD__No));
//18japan  m_wndGrid.SetValueRange(CGXRange(0, 1), _LS(IDS_WG_CMD__ADDD__Node));
//18japan  m_wndGrid.SetValueRange(CGXRange(0, 2), CString(_T("Offset.\n("))+csUnit+_T(")"));
//18japan  m_wndGrid.SetValueRange(CGXRange(0, 3), _LS(IDS_WG_CMD__ADDD__Impact_nFactor));
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMSlanDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	T_SLAN_D SlanD;
	CArray<T_SLAN_K, T_SLAN_K> aSlanK;
	m_pDoc->m_pAttrCtrl->GetSlanKeyList(aSlanK);

	for (int i = 0; i < aSlanK.GetSize(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetSlan(aSlanK[i], SlanD)) continue;

		InsertItem(aSlanK[i], SlanD);
	}
}

void CCMSlanDlg::InitItemDataVar()
{
	m_wndOffsetEdit.SetWindowText(_T("0.0"));
	m_wndFactor.SetWindowText(_T("0.0"));
	m_wndCFEdit.SetWindowText(_T("0.5"));
	m_wnd2PointsEdit1.SetWindowText(_LS(IDS_WG_CMD__ADDD__0___0___0));
	m_wnd2PointsEdit2.SetWindowText(_LS(IDS_WG_CMD__ADDD__0___0___0));
	m_wndPickingEdit.SetWindowText(_T(""));
	m_edtEccenVertLoad.SetWindowText(_T("0.0"));

	OnCmdImpactorSpanRdo();
}

BOOL CCMSlanDlg::InsertItem(T_SLAN_K Key, T_SLAN_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();

	lvitem.iItem = FindInsertionPos(Data.LaneName);
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		str = DataToStr(i, Key, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if( i == 0 ) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_List.InsertItem(&lvitem);
		}
		else m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

CString CCMSlanDlg::DataToStr(int i, T_SLAN_K Key, T_SLAN_D &Data)
{
	CString str;
	if (i == 0) str = Data.LaneName;
	else str = _T("");

	return str;
}

void CCMSlanDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		MakeItemEx();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		UpdateUnit();
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_POST_STEP_CHANGED:
		//DestroyWindow();
		MakeItemEx();
		ButtonCtrl();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMSlanDlg::ButtonCtrl()
{
	BOOL bEnable = FALSE;
	if (m_pDoc->m_pStagCtrl->IsFinalStag() || m_pDoc->m_pStagCtrl->IsBaseStag())
		bEnable = TRUE;

	GetDlgItem(IDC_CMD_BTN_OK)->EnableWindow(bEnable);
}

int CCMSlanDlg::FindInsertionPos(CString& strName)
{
	T_SLAN_K key;
	T_SLAN_D data;
	int nCount = m_List.GetItemCount();
	int i = 0;
	for (i = 0; i < nCount; i++)
	{
		key = m_List.GetItemData(i);
		m_pDoc->m_pAttrCtrl->GetSlan(key, data);

		if (strName.Compare(data.LaneName) < 0) break;
	}
	return i;    
}

void CCMSlanDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_SLAN_K Key, KeyBak;
	T_SLAN_D Data, DataBak;

	BOOL bMFD = FALSE;
	BOOL bMvcdChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
	  switch(nCmd)
	  {
		case(UR_SLAN_ADD):
			{
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetSlan(nKey, Key, Data);
				InsertItem(Key, Data);
			}
			break;
		case(UR_SLAN_DEL):
			{
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetSlan(nKey, Key, Data);
				DeleteItem(Key, Data);
			}
			break;
		case(UR_SLAN_MFD):
			{
				pViewBuff->GetSlan(nKey, KeyBak, DataBak);
			}
			bMFD = TRUE;
			break;
		case(UR_SLAN_MFS):
			{
				pViewBuff->GetSlan(nKey, Key, Data);
				DeleteItem(Key, Data);
				InsertItem(KeyBak, DataBak);
			}
			bMFD = FALSE;
			break;
		case(UR_MVCD_ADD):
		case(UR_MVCD_DEL):
			bMvcdChanged = TRUE;
			break;
		default:
			break;
		}
	} // end of while

	// 코드가 바뀌면 무조건 닫는다.
	if (bMvcdChanged)
	{
		DestroyWindow();
	}
}

void CCMSlanDlg::UpdateUnit() 
{
	SetGridTitle();
	if (m_bListMode) return;  // unit change할 게 없다.
	
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	int nUnitTypeOffset = CUnitCtrl::m_SLAN_UNIT.Offset;  
	double dblOffset;                                 
	int nUnitTypeWidth = CUnitCtrl::m_SLAN_UNIT.Width;  
	int nCount = m_wndGrid.GetRowCount();

	for (int i = 0; i < nCount; i++)
	{
		dblOffset = _tstof(m_wndGrid.GetValueRowCol(i+1, 2));
		dblOffset = m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(nUnitTypeOffset, dblOffset);
		m_wndGrid.SetValueRange(CGXRange(i+1, 2), dblOffset);
	}
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

BOOL CCMSlanDlg::DeleteItem(T_SLAN_K Key, T_SLAN_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CCMSlanDlg::Dlg2Data()
{
	UpdateData(TRUE);
	T_SLAN_BASE item; item.Initialize();
	m_wndLaneName.GetWindowText(m_Data.LaneName);
	CFormulaEdit::GetEditValue(&m_wndLWidthEdit, m_Data.Width);

	T_MVCD_D MvcdData;
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();
	int nCode = MvcdData.nCodeType;

	int nCount = m_wndGrid.GetRowCount();
	m_Data.arLaneData.SetSize(nCount);
	for (int i = 0; i < nCount; i++)
	{
		if(m_bCodeNameINDIA)
		{
			item.KeyNode = _tstol(m_wndGrid.GetValueRowCol(i+1, 1));
			item.Offset = _tstof(m_wndGrid.GetValueRowCol(i+1, 2));
			item.dSpanLength = _tstof(m_wndGrid.GetValueRowCol(i+1, 3));
			item.dImpactFactor = _tstof(m_wndGrid.GetValueRowCol(i+1, 4));
			item.nImpactorSpan = _tstof(m_wndGrid.GetValueRowCol(i+1, 5));
			item.bEJ           = (m_wndGrid.GetValueRowCol(i+1, 6) == _T("1")) ? TRUE : FALSE;
		}
		else
		{
			item.KeyNode = _tstol(m_wndGrid.GetValueRowCol(i+1, 1));
			item.Offset = _tstof(m_wndGrid.GetValueRowCol(i+1, 2));
			item.fact = _tstof(m_wndGrid.GetValueRowCol(i+1, 3));
			if(IsUseSpanStart()) item.bSpanStart = (m_wndGrid.GetValueRowCol(i+1, 4)==_T("1")) ? TRUE : FALSE;
			if(IsUseEccenVertLoad()) item.dEccenVertLoad = _tstof(m_wndGrid.GetValueRowCol(i+1, 5));
			if(IsUseCentrifugalForce()) item.dCentF = _tstof(m_wndGrid.GetValueRowCol(i+1, 6));

			if (nCode == D_MOVE_CODE_BS)
			{
				if (m_Data.nLaneType == 0)
				{
					item.dEccenVertLoad = 0.;
				}
			}

		}
		m_Data.arLaneData.SetAt(i, item);
	}
	
	m_edtSkewStart.GetEditValue(m_Data.dSkewStart);
	m_edtSkewEnd.GetEditValue(m_Data.dSkewEnd);
	m_Data.dWheelSpace = m_edtWheelSpace.GetEditValue();

	m_Data.nMovingDir = m_nMovDir;
	//CUnitCtrl::m_SLAN_UNIT.dSkewEnd  ;
	//CUnitCtrl::m_SLAN_UNIT.dSkewStart;

	/*
	m_Data.dSkewStart;
	m_Data.dSkewEnd  ;
	*/

	if (IsUseLaneTypeRdo())
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlLaneTypeRdo, m_Data.nLaneType);

		if (m_Data.nLaneType == 1)
		{
			m_Data.Width = max(m_Data.Width, m_Data.dWheelSpace);
		}
	}

	int nCheck = m_chkLaneOptim.GetCheck();
	m_Data.bAutoOptimize = (nCheck == 1 && m_Data.nLaneType == 0) ? TRUE : FALSE;
	m_Data.dAllowWidth = m_edtAllowWidth.GetEditValue();

	return TRUE;
}

void CCMSlanDlg::Data2Dlg()
{
	m_wndLaneName.SetWindowText(m_Data.LaneName);
	CString csVal;
	csVal.Format(_T("%g"), m_Data.Width);
	m_wndLWidthEdit.SetWindowText(csVal);
	InitItemDataVar();

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	 
	int nSize = m_Data.arLaneData.GetSize();

	m_wndGrid.SetRowCount(nSize);
	for (int i = 0; i < nSize; i++)
	{ 
		if(m_bCodeNameINDIA)
		{
			m_wndGrid.SetValueRange(CGXRange(i+1, 1), (long)m_Data.arLaneData[i].KeyNode);  
			m_wndGrid.SetValueRange(CGXRange(i+1, 2), m_Data.arLaneData[i].Offset); 

			if(m_Data.arLaneData[i].nImpactorSpan == 0) // Impact Factor
			{
				m_wndGrid.SetValueRange(CGXRange(i+1, 3), _T("-")); 
				m_wndGrid.SetStyleRange(CGXRange(i+1, 3), CGXStyle().SetEnabled(FALSE));
				m_wndGrid.SetValueRange(CGXRange(i+1, 4), m_Data.arLaneData[i].dImpactFactor); 
			}
			else // Span Length
			{
				m_wndGrid.SetValueRange(CGXRange(i+1, 3), m_Data.arLaneData[i].dSpanLength); 
				m_wndGrid.SetValueRange(CGXRange(i+1, 4), _T("-")); 
				m_wndGrid.SetStyleRange(CGXRange(i+1, 4), CGXStyle().SetEnabled(FALSE));
			}
			m_wndGrid.SetValueRange(CGXRange(i+1, 5), (long)m_Data.arLaneData[i].nImpactorSpan); 
			CString str = (m_Data.arLaneData[i].bEJ) ? _T("1") : _T("0");
			m_wndGrid.SetValueRange(CGXRange(i+1, 6), str);
		}
		else
		{
			m_wndGrid.SetValueRange(CGXRange(i+1, 1), (long)m_Data.arLaneData[i].KeyNode);  
			m_wndGrid.SetValueRange(CGXRange(i+1, 2), m_Data.arLaneData[i].Offset); 
			m_wndGrid.SetValueRange(CGXRange(i+1, 3), m_Data.arLaneData[i].fact); 
			if (IsUseSpanStart())
			{
				CString str = (m_Data.arLaneData[i].bSpanStart) ? _T("1") : _T("0");
				m_wndGrid.SetValueRange(CGXRange(i+1, 4), str);
			}
			if(IsUseEccenVertLoad())
			{
				m_wndGrid.SetValueRange(CGXRange(i+1, 5), m_Data.arLaneData[i].dEccenVertLoad); 
			}
			if(IsUseCentrifugalForce())
			{
				m_wndGrid.SetValueRange(CGXRange(i+1, 6), m_Data.arLaneData[i].dCentF); 
			}
		}
	}
	if (nSize > 0) m_wndGrid.SetCurrentCell(1, 1);
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();

	CString str;

	str.Format(_T("%g"), m_Data.dSkewStart);
	m_edtSkewStart.SetWindowText(str);

	str.Format(_T("%g"), m_Data.dSkewEnd);
	m_edtSkewEnd.SetWindowText(str);

	m_edtWheelSpace.SetEditUnit(m_Data.dWheelSpace);

	if (IsUseLaneTypeRdo())
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlLaneTypeRdo, m_Data.nLaneType);
	}

	int nCheck = (m_Data.bAutoOptimize && m_Data.nLaneType == 0) ? 1 : 0;
	m_chkLaneOptim.SetCheck(nCheck);
	m_edtAllowWidth.SetEditUnit(m_Data.dAllowWidth);

	m_nMovDir = m_Data.nMovingDir;
	
	UpdateData(FALSE);

	SelectNode();
}

BOOL CCMSlanDlg::Grid_IsExistKey(int nKey)
{
	int nCount = m_wndGrid.GetRowCount();
	for (int i = 0; i < nCount; i++)
	{
		if (nKey == _tstol(m_wndGrid.GetValueRowCol(i+1, 1))) return TRUE;
	}
	return FALSE;
}

void CCMSlanDlg::Grid_InsertItem(int nPos, T_SLAN_BASE &item)
{
	ROWCOL nRow;
	if (nPos < 0 || nPos > m_wndGrid.GetRowCount()) nRow = m_wndGrid.GetRowCount();
	else nRow = nPos;
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	m_wndGrid.InsertRows(nRow+1, 1);

	if(m_bCodeNameINDIA)
	{
		m_wndGrid.SetValueRange(CGXRange(nRow+1, 1), (long)item.KeyNode); 
		m_wndGrid.SetValueRange(CGXRange(nRow+1, 2), item.Offset); 

		if(item.nImpactorSpan == 0) // Impact Factor
		{
			m_wndGrid.SetValueRange(CGXRange(nRow+1, 3), _T("-")); 
			m_wndGrid.SetStyleRange(CGXRange(nRow+1, 3), CGXStyle().SetEnabled(FALSE));
			m_wndGrid.SetValueRange(CGXRange(nRow+1, 4), item.dImpactFactor); 
		}
		else // Span Length
		{
			m_wndGrid.SetValueRange(CGXRange(nRow+1, 3), item.dSpanLength); 
			m_wndGrid.SetValueRange(CGXRange(nRow+1, 4), _T("-")); 
			m_wndGrid.SetStyleRange(CGXRange(nRow+1, 4), CGXStyle().SetEnabled(FALSE));
		}

		m_wndGrid.SetValueRange(CGXRange(nRow+1, 5), (long)item.nImpactorSpan); 
		//m_wndGrid.SetValueRange(CGXRange(nRow+1, 6), (long)item.nImpactorSpan);  // not used
	}
	else
	{
		m_wndGrid.SetValueRange(CGXRange(nRow+1, 1), (long)item.KeyNode); 
		m_wndGrid.SetValueRange(CGXRange(nRow+1, 2), item.Offset); 
		m_wndGrid.SetValueRange(CGXRange(nRow+1, 3), item.fact); 
		if (IsUseSpanStart())
		{
			CString str = _T("0");
			m_wndGrid.SetValueRange(CGXRange(nRow+1, 4), str);
		}
		if(IsUseEccenVertLoad())
		{
			m_wndGrid.SetValueRange(CGXRange(nRow+1, 5), item.dEccenVertLoad); 
		}
		if(IsUseCentrifugalForce())
		{
			m_wndGrid.SetValueRange(CGXRange(nRow+1, 6), item.dCentF); 
		}

	}

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMSlanDlg::InsertBaseItem(int nPos, int nType)
{
	T_SLAN_BASE item;

	CFormulaEdit::GetEditValue(&m_wndOffsetEdit,item.Offset);
	CFormulaEdit::GetEditValue(&m_edtEccenVertLoad, item.dEccenVertLoad);

	CArray<UINT, UINT> aImpactorSpan;
	aImpactorSpan.Add(IDC_CMD_SLAN_IMPACT_FACTOR_RDO);
	aImpactorSpan.Add(IDC_CMD_SLAN_SPAN_LENGTH_RDO);
		
	if(m_bCodeNameINDIA)
	{
		CFormulaEdit::GetEditValue(&m_wndSpanLengthEdt, item.dSpanLength);
		CFormulaEdit::GetEditValue(&m_wndImpactFactorEdt, item.dImpactFactor);
		CDlgUtil::CtrlRadioGetCheck(this, aImpactorSpan, item.nImpactorSpan);
	}
	else
	{
		CFormulaEdit::GetEditValue(&m_wndFactor,item.fact);
		CFormulaEdit::GetEditValue(&m_wndCFEdit,item.dCentF);
	}

	if (nType == 0)  // Select By 2 Points
	{
		CArray<T_NODE_K, T_NODE_K> aNodeKey;  
		double ux, uy, uz;
		double WX1, WY1, WZ1;
		double WX2, WY2, WZ2;

		if (!CFormulaEdit::GetPosition(&m_wnd2PointsEdit1, ux, uy, uz))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___1st_position_is_invalid_));
			return;
		}
		m_wnd2PointsEdit1.GetWcsPos(ux, uy, uz, WX1, WY1, WZ1);
		if (!CFormulaEdit::GetPosition(&m_wnd2PointsEdit2, ux, uy, uz))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___2nd_position_is_invalid_));
			return;
		}
		m_wnd2PointsEdit2.GetWcsPos(ux, uy, uz, WX2, WY2, WZ2);

		// Get Node Key List
		// 2000.6.5 Add by scchoi
	  double dbTolerance = m_pDoc->m_pFlagCtrl->GetGeneralCoordinateToleranceCurrentUnit();
	  m_pDoc->m_pDataCtrl->GetNodeBy2Point(WX1, WY1, WZ1, WX2, WY2, WZ2, dbTolerance, aNodeKey, TRUE);

		if (aNodeKey.GetSize() == 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_Node_on_line_));
			return;
		}  

	  int nNodeKey;
		int nCount = aNodeKey.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			nNodeKey = aNodeKey[i];
			if (Grid_IsExistKey(nNodeKey))
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Node_No___d_is_already_ex), nNodeKey);
				AfxMessageBox(msg);
				return;
			}
	    item.KeyNode = (unsigned int)nNodeKey;
			Grid_InsertItem(nPos, item);
			if (nPos >= 0) nPos++;
		}

	 SelectNode();
	 return;
	}
	else if(nType == 1) // Select By Picking
	{
		int nNodeKey;
		CFormulaEdit::GetEditValue(&m_wndPickingEdit, nNodeKey);
		if (nNodeKey <= 0)
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Node_No___d_is_not_exist_), nNodeKey);
			AfxMessageBox(msg);
			return;
		}
		if (Grid_IsExistKey(nNodeKey))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Node_No___d_is_already_ex), nNodeKey);
			AfxMessageBox(msg);
			return;
		}
		T_NODE_D Data;

	  m_pDoc->m_pAttrCtrl->GetNode(nNodeKey, Data);

	  item.KeyNode = (unsigned int)nNodeKey;  
		Grid_InsertItem(nPos, item);
	}
	else if(nType == 2) // Select By Select Node
	{
		CArray<T_NODE_K, T_NODE_K> arNodeKeyList;
		CArray<long, long> arNodeList;

		CString strNodeNumber;
	  m_wndNodeSelectEdit.GetWindowText(strNodeNumber);

		m_pDoc->m_pAttrCtrl->GetNodeKeyList(arNodeKeyList);

		CNumericOptimizer optimizer;

		optimizer.SelectNumByStrBase((long*)arNodeKeyList.GetData(), (long)arNodeKeyList.GetSize(),
			                            strNodeNumber, arNodeList);

		if(arNodeList.GetSize() <= 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_node_));
			return;
		}

		for(int i=0; i<arNodeList.GetSize(); i++)
			if (Grid_IsExistKey((UINT)arNodeList[i]))
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Node_No__d__is_already_ex), (UINT)arNodeList[i]);
				AfxMessageBox(msg);
				return;
			} 
		int nInsertPos = nPos;
		for(int i=0; i<arNodeList.GetSize(); i++)
		{
			T_NODE_D Data;

			m_pDoc->m_pAttrCtrl->GetNode((UINT)arNodeList[i], Data);

			item.KeyNode = (UINT)arNodeList[i];
			Grid_InsertItem(nPos, item);
			if(nInsertPos >= 0) nPos++;
		}
	}


	ROWCOL ncRow, ncCol;
	BOOL bCurrent = m_wndGrid.GetCurrentCell(ncRow, ncCol);
	if (!bCurrent) ncCol = 1;

	if (nPos == -1) ncRow = m_wndGrid.GetRowCount();
	else ncRow = nPos+1;

	m_wndGrid.SetCurrentCell(ncRow, ncCol);

	SelectNode();
}

// add by pig
void CCMSlanDlg::SelectNode() 
{
	int nCount = m_wndGrid.GetRowCount();
	if(nCount >= 0)
	{
	  CArray<T_NODE_K, T_NODE_K>arSelNode;
	  for (int i = 0; i < nCount; i++)arSelNode.Add(_tstol(m_wndGrid.GetValueRowCol(i+1, 1)));
	  m_pDoc->m_pViewCtrl->SelectNode(NULL, arSelNode, TRUE, TRUE);
	}
}

BOOL CCMSlanDlg::OnInitDialog() 
{ // This
	CCMDlgBase::OnInitDialog();

	T_MVCD_D MvcdData;
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();
	int nCode = MvcdData.nCodeType;

	if(nCode == D_MOVE_CODE_INDIA)
	{
		m_bCodeNameINDIA = TRUE;    
	}
	
	// TODO: Add extra initialization here
	AlignControl();
	SetHeaderTitle();
	MakeItemEx();
	ButtonCtrl();

	m_edtSkewStart.SetRange(SHRT_MIN, SHRT_MAX);
	m_edtSkewEnd.SetRange(SHRT_MIN, SHRT_MAX);

	CString strImgPath = _T("SVG\\illustration\\Dialog\\cmd_tsur 2.svg");

	m_ImgViewer.SetImage(strImgPath);

	m_wndOffsetUnit.SetUnitType(CUnitCtrl::m_SLAN_UNIT.Offset);
	m_wndLWidthUnit.SetUnitType(CUnitCtrl::m_SLAN_UNIT.Width );  
	m_edtSkewStart .SetUnitType(CUnitCtrl::m_SLAN_UNIT.dSkewStart);
	m_edtSkewEnd   .SetUnitType(CUnitCtrl::m_SLAN_UNIT.dSkewEnd  );
	// this  
	m_2PointUnit1.SetUnitType(CUnitCtrl::m_LENGTH_UNIT);
	m_2PointUnit2.SetUnitType(CUnitCtrl::m_LENGTH_UNIT);
	m_untAllowWidth.SetUnitType(CUnitCtrl::m_LLAN_UNIT.dAllowWidth);

	m_wndOffsetEdit.SetAttUcsDistance();
	m_wndOffsetEdit.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndOffsetEdit.SetDistValueMode(TRUE);
	m_wndLWidthEdit.SetAttUcsDistance();
	m_wndLWidthEdit.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndLWidthEdit.SetDistValueMode(TRUE);

	/*
	int nSelect = 0;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlSelect, nSelect); 
	m_wnd2PointsEdit1.EnableWindow(nSelect==0);
	m_wnd2PointsEdit2.EnableWindow(nSelect==0);
	m_wndPickingEdit.EnableWindow(nSelect==1); 
	m_wndNodeSelectEdit.EnableWindow(nSelect==2); 
	*/

	m_wnd2PointsEdit1.SetAttUcsPos();
	m_wnd2PointsEdit1.SetModeToUse(MOUSEEDIT_USE_SETP2_FIRST);
	m_wnd2PointsEdit2.SetAttUcsPos();
	m_wnd2PointsEdit2.SetModeToUse(MOUSEEDIT_USE_SETP2_SECOND);
	m_wnd2PointsEdit2.SetLButtonDownNotifyWindow(this);
	m_wnd2PointsEdit2.SetEnterNotifyWindow(this);
	m_wnd2PointsEdit1.SetNextLink(&m_wnd2PointsEdit2);

	m_wndPickingEdit.SetAttNodeList();
	m_wndPickingEdit.SetLButtonDownNotifyWindow(this);
	m_wndPickingEdit.SetEnterNotifyWindow(this);
	m_wndPickingEdit.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_wndPickingEdit.SetMaxNodeKeyNum(1);
	m_edtWheelSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untWheelSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_wndSpanLengthEdt.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndSpanLengthUnt.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_csTitle1 = _LS(IDS_CMD_SLAN_TITLE1); // _T("Traffic Line Lanes")  //2
	m_csTitle2 = _LS(IDS_CMD_SLAN_TITLE2); // _T("Define Design Traffic Lane")//2

	InitGrid();

	InitItemDataVar();

	if(m_bModify)
	{
		SetInitModify();
		SetWindowText(m_csTitle2);          //2
	}
	else
	{
		SetWindowText(m_csTitle1);          //2
		CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
		CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);

		if(m_bCodeNameINDIA)
		{
			CDlgUtil::CtrlShowHide(this, m_aImpactFactor, FALSE);
		}
		else
		{
			CDlgUtil::CtrlShowHide(this, m_aImpactFactor, TRUE);
		}
	}

	SelectMethodMan();

	ShowHideByCode();

	OnCmdImpactorSpanRdo();

#if defined(_MGEN)
	GetDlgItem(IDC_CMD_ECCEN_VERT_LOAD_STC)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_ECCEN_VERT_LOAD_STC2)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_ECCEN_VERT_LOAD_EDT2)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_ECCEN_VERT_LOAD_UNT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_LANE_TYPE_GRP)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_ROAD_LANE_RDO)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_RAIL_LANE_RDO)->EnableWindow(FALSE);
#endif

	GetDlgItem(IDC_CMD_SLAN_IMPACT_FACTOR_RDO)->SetWindowText(_LS(IDS_CMD_SLAN_INDIA_IF_CDA));

	if (nCode == D_MOVE_CODE_NEWZEALAND)
	{
		GetDlgItem(IDC_CMD_LWIDTH_TITLE)->SetWindowText(_LS(IDS_CMD_LANE_WIDTH_NZ));
		GetDlgItem(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_STC)->SetWindowText(_LS(IDS_CMD_ALLOWABLE_WIDTH_NZ));
	}

	return TRUE;   // return TRUE unless you set the focus to a control
								// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMSlanDlg::IsRelevantCode(int nCode)
{
	if (nCode == D_MOVE_CODE_AASHTO_STAN) return TRUE;
	if (nCode == D_MOVE_CODE_AASHTO_LRFD) return TRUE;
	if (nCode == D_MOVE_CODE_TAIWAN) return TRUE;
	if (nCode == D_MOVE_CODE_KOREA) return TRUE;
	if (nCode == D_MOVE_CODE_CANADA) return TRUE;
	if (nCode == D_MOVE_CODE_PENDOT) return TRUE;
	if (nCode == D_MOVE_CODE_BS) return TRUE;
	if (nCode == D_MOVE_CODE_EURO_BS) return TRUE;
	if (nCode == D_MOVE_CODE_RUSSIA) return TRUE;
	if (nCode == D_MOVE_CODE_KOREA_LRFD_2011) return TRUE;
	if (nCode == D_MOVE_CODE_AUSTRALIA) return TRUE;
	if (nCode == D_MOVE_CODE_POLAND) return TRUE;
	if (nCode == D_MOVE_CODE_SOUTH_AFRICA) return TRUE;
	if (nCode == D_MOVE_CODE_NEWZEALAND) return TRUE;
	if (nCode == D_MOVE_CODE_BRAZIL) return TRUE;
	
	return FALSE;
}

BOOL CCMSlanDlg::IsUseImpactSpan()
{
	T_MVCD_D MvcdData;
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();
	int nCode = MvcdData.nCodeType;

	if (nCode == D_MOVE_CODE_AASHTO_LRFD || 
			nCode == D_MOVE_CODE_CANADA || 
			nCode == D_MOVE_CODE_PENDOT || 
			nCode == D_MOVE_CODE_BS || 
			nCode == D_MOVE_CODE_EURO_BS ||
			nCode == D_MOVE_CODE_RUSSIA || 
			nCode == D_MOVE_CODE_KOREA_LRFD_2011 ||
			nCode == D_MOVE_CODE_AUSTRALIA ||
			nCode == D_MOVE_CODE_POLAND ||
			nCode == D_MOVE_CODE_SOUTH_AFRICA ||
			nCode == D_MOVE_CODE_NEWZEALAND) return FALSE;
	return TRUE;
}

BOOL CCMSlanDlg::IsUseSpanStart()
{
	T_MVCD_D MvcdData;
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();
	int nCode = MvcdData.nCodeType;
	
	if (nCode == D_MOVE_CODE_CANADA || 
		  nCode == D_MOVE_CODE_BS || 
			nCode == D_MOVE_CODE_EURO_BS ||
			nCode == D_MOVE_CODE_RUSSIA || 
			nCode == D_MOVE_CODE_KOREA_LRFD_2011 ||
			nCode == D_MOVE_CODE_SOUTH_AFRICA ||
			nCode == D_MOVE_CODE_NEWZEALAND ||
			nCode == D_MOVE_CODE_BRAZIL) return FALSE;
	return TRUE;
}

BOOL CCMSlanDlg::IsUseEccenVertLoad()
{
	T_MVCD_D MvcdData;
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();
	int nCode = MvcdData.nCodeType;

	if (nCode == D_MOVE_CODE_BS) return TRUE;
#if defined(_MGEN)
	return FALSE;
#else
	if (nCode != D_MOVE_CODE_EURO_BS) return FALSE;
	
	return TRUE;
#endif	
	
	return FALSE;
}

BOOL CCMSlanDlg::IsUseCentrifugalForce()
{
	T_MVCD_D MvcdData;
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();
	int nCode = MvcdData.nCodeType;

	if (nCode != D_MOVE_CODE_AASHTO_LRFD && nCode != D_MOVE_CODE_CANADA) return FALSE;

	return TRUE;
}

BOOL CCMSlanDlg::IsUseLaneTypeRdo()
{
	T_MVCD_D MvcdData;
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();
	int nCode = MvcdData.nCodeType;

	if (nCode != D_MOVE_CODE_BS) return FALSE;

	return TRUE;
}

BOOL CCMSlanDlg::IsUseOptim(int nCode)
{
	if (nCode == D_MOVE_CODE_PENDOT          || nCode == D_MOVE_CODE_CANADA       || 
		  nCode == D_MOVE_CODE_AASHTO_LRFD     || nCode == D_MOVE_CODE_AASHTO_STAN  || 
			nCode == D_MOVE_CODE_BS              || nCode == D_MOVE_CODE_EURO_BS      || 
	  	nCode == D_MOVE_CODE_RUSSIA          || nCode == D_MOVE_CODE_KOREA        || 
		  nCode == D_MOVE_CODE_KOREA_LRFD_2011 || nCode == D_MOVE_CODE_AUSTRALIA    ||
			nCode == D_MOVE_CODE_POLAND          || nCode == D_MOVE_CODE_SOUTH_AFRICA ||
			nCode == D_MOVE_CODE_NEWZEALAND      || nCode == D_MOVE_CODE_BRAZIL) return TRUE;

	return FALSE;
}

void CCMSlanDlg::ShowHideByCode()
{
	T_MVCD_D MvcdData;
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();
	int nCode = MvcdData.nCodeType;
	/**
	if (!IsRelevantCode(nCode))
	{
		DestroyWindow();
		return;
	}
	**/
	if (m_bListMode) return;

	CArray<UINT, UINT> aControls;
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_FACTOR_TITLE);
	aControls.Add(IDC_CMD_FACTOR_EDIT);  
	BOOL bShowImpactSpan = TRUE;
	BOOL bShowSpanStart = TRUE;
	BOOL bShowEccenVertLoad = TRUE;
	BOOL bShowCF = TRUE;
	if(!IsUseImpactSpan())    bShowImpactSpan = FALSE;
	if(!IsUseSpanStart())     bShowSpanStart = FALSE;
	if(!IsUseEccenVertLoad()) bShowEccenVertLoad = FALSE;
	if(!IsUseCentrifugalForce()) bShowCF = FALSE;
	CDlgUtil::CtrlShowHide(this, aControls, bShowImpactSpan);

	if(m_bCodeNameINDIA)
	{
		CDlgUtil::CtrlShowHide(this, aControls, FALSE);
	}

	m_wndGrid.HideCols(3, D_NUM_COL, !bShowImpactSpan);
	if (D_NUM_COL >= 4) m_wndGrid.HideCols(4, D_NUM_COL, !bShowSpanStart);
	m_wndGrid.HideCols(5, D_NUM_COL, !bShowEccenVertLoad);

	if(m_bCodeNameINDIA) m_wndGrid.HideCols(6, D_NUM_COL, FALSE);
	else              	 m_wndGrid.HideCols(6, D_NUM_COL, !bShowCF);

	if(bShowImpactSpan)
	{
		m_wndGrid.SetColWidthByDPI(0, 0, 25);
		m_wndGrid.SetColWidthByDPI(1, 1, 35);
		m_wndGrid.SetColWidthByDPI(2, 2, 55);
	}
	else if(bShowSpanStart)
	{
		if(bShowCF)
		{
			m_wndGrid.SetColWidthByDPI(0, 0, 30);
			m_wndGrid.SetColWidthByDPI(1, 1, 60);
			m_wndGrid.SetColWidthByDPI(2, 2, 45);
		}
		else
		{
			m_wndGrid.SetColWidthByDPI(0, 0, 30);
			m_wndGrid.SetColWidthByDPI(1, 1, 60);
			m_wndGrid.SetColWidthByDPI(2, 2, 80);
		}
	}
	else if(bShowEccenVertLoad)
	{
		m_wndGrid.SetColWidthByDPI(0, 0, 30);
		m_wndGrid.SetColWidthByDPI(1, 1, 40);
		m_wndGrid.SetColWidthByDPI(2, 2, 50);
		m_wndGrid.SetColWidthByDPI(3, 3, 80);
	}
	else
	{
		m_wndGrid.SetColWidthByDPI(0, 0, 30);
		m_wndGrid.SetColWidthByDPI(1, 1, 70);
		m_wndGrid.SetColWidthByDPI(2, 2, 90);
	}    

	// Moving Direction  
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_ST_MOVINGDIR);
	aControls.Add(IDC_CMD_MOV_FORWARD);
	aControls.Add(IDC_CMD_MOV_BACKWARD);
	aControls.Add(IDC_CMD_MOV_BOTH);

	CDlgUtil::CtrlShowHide(this, aControls, (nCode != D_MOVE_CODE_EURO_BS && nCode != D_MOVE_CODE_RUSSIA));

	// 이동하중 횡방향 최적화 옵션 (체크)
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_LANE_OPTIM_CHK);
	aControls.Add(IDC_CMD_LANE_OPTIM_GRP);
	aControls.Add(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_STC);
	aControls.Add(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_EDT);
	aControls.Add(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_UNT);

	BOOL bShow = IsUseOptim(nCode);
	CDlgUtil::CtrlShowHide(this, aControls, bShow);
	CDlgUtil::CtrlEnableDisable(this, aControls, bShow);

	aControls.RemoveAll();
	aControls.Add(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_STC);
	aControls.Add(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_EDT);
	aControls.Add(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_UNT);

	int nCheck = m_chkLaneOptim.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, aControls, bShow && nCheck==1);

	// Eccentricity of Vertical Load to Consider Cant
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_ECCEN_VERT_LOAD_STC);
	aControls.Add(IDC_CMD_ECCEN_VERT_LOAD_STC2);
	aControls.Add(IDC_CMD_ECCEN_VERT_LOAD_EDT2);
	aControls.Add(IDC_CMD_ECCEN_VERT_LOAD_UNT);

	if (nCode == D_MOVE_CODE_EURO_BS)
	{
		CDlgUtil::CtrlShowHide(this, aControls, TRUE);
	}
	else if (nCode == D_MOVE_CODE_BS)
	{
		CDlgUtil::CtrlEnableDisable(this, aControls, m_Data.nLaneType == 1);

		if (m_Data.nLaneType == 0)
		{
			m_wndGrid.HideCols(5, 5, TRUE);
			m_wndGrid.SetColWidthByDPI(0, 0, 30);
			m_wndGrid.SetColWidthByDPI(1, 1, 70);
			m_wndGrid.SetColWidthByDPI(2, 2, 90);
		}
		else
		{
			m_wndGrid.HideCols(5, 5, FALSE);
			m_wndGrid.SetColWidthByDPI(0, 0, 30);
			m_wndGrid.SetColWidthByDPI(1, 1, 40);
			m_wndGrid.SetColWidthByDPI(2, 2, 50);
			m_wndGrid.SetColWidthByDPI(3, 3, 80);
		}
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, aControls, FALSE);
	}

	// Centrifugal Force
	CDlgUtil::CtrlShowHide(this, m_aCtrlCF, bShowCF);
	CDlgUtil::CtrlShowHide(this, m_aCtrlLaneType, nCode == D_MOVE_CODE_BS);
	if (nCode == D_MOVE_CODE_BS)
	{
		GetDlgItem(IDC_CMD_LANE_OPTIM_CHK)->EnableWindow(bShow && m_Data.nLaneType == 0);
		GetDlgItem(IDC_CMD_LANE_OPTIM_ALLOW_WIDTH_EDT)->EnableWindow(bShow && nCheck == 1 && m_Data.nLaneType == 0);

		CDlgUtil::CtrlEnableDisable(this, m_aLaneWidth, m_Data.nLaneType == 0);
	}
}

void CCMSlanDlg::SetParamData(T_SLAN_D& data)
{
	m_bModify = TRUE;
	m_Data = data;
}

void CCMSlanDlg::SetInitModify()
{
	m_csOldLaneName = m_Data.LaneName;
	
	Data2Dlg();

	m_bListMode = FALSE;	
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);	

	if(m_bCodeNameINDIA)
	{
		CDlgUtil::CtrlShowHide(this, m_aImpactFactor, FALSE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aImpactFactor, TRUE);
	}

	m_wndLaneName.SetFocus();
	ShowHideByCode();
}

void CCMSlanDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	m_bModify = FALSE;
	m_Data.Initialize();

	//-------------------------------------------------------------
	// MQC 4897-13 단위가 ft인 경우 Lane Width Default 값을 10ft로.
	T_UNIT_INDEX CurrentUnitIndex;
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);

	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrentUnitIndex);
	if(CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT ||
		CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN)
	{
		m_Data.Width = M_InitValueCurUnit(10.0, LBF, FT, CUnitCtrl::m_SLAN_UNIT.Width);
		m_Data.dAllowWidth = M_InitValueCurUnit(10.0, LBF, FT, CUnitCtrl::m_LLAN_UNIT.dAllowWidth);
	}
	else if(CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M ||
		CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM ||
		CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM )
	{
		m_Data.Width = M_InitValueCurUnit(3.0, KN, M, CUnitCtrl::m_SLAN_UNIT.Width);
		m_Data.dAllowWidth = M_InitValueCurUnit(3.0, KN, M, CUnitCtrl::m_LLAN_UNIT.dAllowWidth);
	}

	T_MVCD_D MvcdData;
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();
	if(MvcdData.nCodeType == D_MOVE_CODE_AUSTRALIA)
	{
		m_Data.Width = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, 3.2);
	}

	//-------------------------------------------------------------

	m_Data.dWheelSpace = GetInitialWheelSpacing(m_Data.nLaneType);
	Data2Dlg();

	m_bListMode = FALSE;
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	
	if(m_bCodeNameINDIA)
	{
		CDlgUtil::CtrlShowHide(this, m_aImpactFactor, FALSE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aImpactFactor, TRUE);
	}

	m_wndLaneName.SetFocus();SelectMethodMan();
	SetWindowText(m_csTitle2);
	ShowHideByCode();
}

void CCMSlanDlg::OnCmdBtnClose() 
{
	// TODO: Add your control notification handler code here
	DestroyWindow();
}

void CCMSlanDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnModify();	
	*pResult = 0;
}

void CCMSlanDlg::OnCmdBtnCancel() 
{
	// TODO: Add your control notification handler code here
	m_bListMode = TRUE;
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	
	if(m_bCodeNameINDIA)
	{
		CDlgUtil::CtrlShowHide(this, m_aImpactFactor, FALSE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aImpactFactor, TRUE);
	}

	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END); //현재 작업을 종료한다. 이전 상태로 복귀한다. 
	m_List.SetFocus();
	SetWindowText(m_csTitle1);
}

void CCMSlanDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	BOOL bOldLock = m_wndGrid.LockUpdate(FALSE);
	if (bOldLock) m_wndGrid.Redraw();
	m_wndGrid.LockUpdate(bOldLock);	
	// Do not call CCMDlgBase::OnPaint() for painting messages
}

void CCMSlanDlg::OnCmdChangeSelectBy() 
{
	/*
	// TODO: Add your control notification handler code here
	int nSelect;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlSelect, nSelect);
	
	m_wnd2PointsEdit1.EnableWindow(nSelect==0);
	m_wnd2PointsEdit2.EnableWindow(nSelect==0);
	m_wndPickingEdit.EnableWindow(nSelect==1);
	m_wndNodeSelectEdit.EnableWindow(nSelect==2);
	
	if (nSelect == 0) m_wnd2PointsEdit1.SetFocus();
	else if(nSelect == 1) m_wndPickingEdit.SetFocus();	
	else m_wndNodeSelectEdit.SetFocus();
	*/
	UpdateData(TRUE);
	SelectMethodMan();
	I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);

	if (m_nSelectionMethod == 0) m_wnd2PointsEdit1.SetFocus();
	else if(m_nSelectionMethod == 1) m_wndPickingEdit.SetFocus();
	else m_wndNodeSelectEdit.SetFocus();
}

void CCMSlanDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	T_SLAN_K Key;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_lane__nChoose));
		return;
	}
 
	Key = (T_SLAN_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetSlan(Key, m_Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Moving_load_lane_key__d_d), Key);
		AfxMessageBox(msg);
		return;
	}

	m_csOldLaneName = m_Data.LaneName;
	
	m_bModify = TRUE;
	Data2Dlg();

	m_bListMode = FALSE;	
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);	

	if(m_bCodeNameINDIA)
	{
		CDlgUtil::CtrlShowHide(this, m_aImpactFactor, FALSE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aImpactFactor, TRUE);
	}

	m_wndLaneName.SetFocus();SelectMethodMan();
	SetWindowText(m_csTitle2);
	ShowHideByCode();
}

void CCMSlanDlg::OnCmdBtnCopy() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_lane__nChoose));
		return;
	}
	CString csSLaneName = m_List.GetItemText(iItem, 0);

	CArray<CString,CString&> SLaneNames;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetSlanList(SLaneNames);

	CString StrName;
	if(!CStrParser::MakeUniqueName(SLaneNames,csSLaneName,StrName))
	{
		AfxMessageBox(_LS(IDS_CMD0417__Unable_to_make_lane_Name___));
		return;
	}

	
	T_SLAN_D LaneD, SourceLaneD;
	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetSlan(csSLaneName,SourceLaneD))
	{
		CString StrTe;
		StrTe.Format(_LS(IDS_CMD0417__Not_Exist_Lane__s),csSLaneName);
		AfxMessageBox(StrTe);
		return;
	}

	LaneD = SourceLaneD;
	LaneD.LaneName = StrName;
		
	CDBDoc::GetDocPoint()->m_pDataCtrl->AddSlan(LaneD);
}

void CCMSlanDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_lane__nChoose));
		return;
	}

	CString csLaneName = m_List.GetItemText(iItem, 0);
	if (!m_pDoc->m_pDataCtrl->DelSlan(csLaneName)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

LRESULT CCMSlanDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if (!m_bListMode)
	{
		int nType;
		if (wParam == m_wnd2PointsEdit2.GetDlgCtrlID()) nType = 0;
		else if (wParam == m_wndPickingEdit.GetDlgCtrlID()) nType = 1;
		else if (wParam == m_wndNodeSelectEdit.GetDlgCtrlID()) nType = 2;
		else ASSERT(0);

		InsertBaseItem(-1, nType);
		//if (nType==1) m_wnd2PointsEdit1.SetFocus();
	}

	return 0L;
}

void CCMSlanDlg::OnCmdOpAdd() 
{
	// TODO: Add your control notification handler code here
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlSelect, nCheck);
	InsertBaseItem(-1, nCheck);	
}

void CCMSlanDlg::OnCmdOpInsert() 
{
	// TODO: Add your control notification handler code here
	ROWCOL nRow, nCol;
	if (m_wndGrid.GetCurrentCell(nRow, nCol))
	{
		nRow = nRow-1;
	}
	else nRow = -1;
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlSelect, nCheck);
	InsertBaseItem(nRow, nCheck);	
}

void CCMSlanDlg::OnCmdOpDelete() 
{
	// TODO: Add your control notification handler code here
	ROWCOL ncRow, ncCol;
	BOOL bCurrent = m_wndGrid.GetCurrentCell(ncRow, ncCol);
	CRowColArray awRows;
	m_wndGrid.GetSelectedRows(awRows, TRUE, TRUE);
	if (awRows.GetSize() < 1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_item_to_delete_));
		return;
	}
	ROWCOL nRow;
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	for (int i = awRows.GetSize()-1; i >= 0; i--)
	{
		nRow = awRows.GetAt(i);
		if (nRow == 0) continue;
		m_wndGrid.RemoveRows(nRow, nRow);
	}

	int nRowCount = m_wndGrid.GetRowCount();
	if (bCurrent && nRowCount > 0)
	{
		if (nRowCount < ncRow) ncRow = nRowCount;
		m_wndGrid.SetCurrentCell(ncRow, ncCol);
	}

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();	

	SelectNode();
}

void CCMSlanDlg::OnCmdBtnOk() 
{
	// TODO: Add your control notification handler code here
	BOOL CheckSlan=FALSE;
	CString strTemp;
	m_wndLaneName.GetWindowText(strTemp);

	if((m_wndGrid.GetRowCount()) && ( strTemp == _T("")))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Missing_Lane_Name__),MB_OK);
		return;
	}

	if (!Dlg2Data()) return;
	if (m_bModify) CheckSlan = m_pDoc->m_pDataCtrl->ModifySlan(m_csOldLaneName, m_Data);
	else CheckSlan = m_pDoc->m_pDataCtrl->AddSlan(m_Data); 

	if (!CheckSlan)   // 입력 데이타에 오류가 있을때 Window를 Close하지 않게 한다.
		return; 

	m_bListMode = TRUE;
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);

	if(m_bCodeNameINDIA)
	{
		CDlgUtil::CtrlShowHide(this, m_aImpactFactor, FALSE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aImpactFactor, TRUE);
	}

	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);	
	m_List.SetFocus();
	SetWindowText(m_csTitle1);
}

void CCMSlanDlg::OnCmdBtnApply() 
{
	CString strTemp;
	m_wndLaneName.GetWindowText(strTemp);

	if((m_wndGrid.GetRowCount()) && ( strTemp == _T("")))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Missing_Lane_Name__),MB_OK);
		return;
	}

	if (!Dlg2Data()) return;
	if (m_bModify) m_pDoc->m_pDataCtrl->ModifySlan(m_csOldLaneName, m_Data);
	else m_pDoc->m_pDataCtrl->AddSlan(m_Data); 
}

BOOL CCMSlanDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		CWnd* pWnd = GetFocus();
		if (pWnd && pWnd->GetSafeHwnd() && IsWindow(pWnd->GetSafeHwnd()) &&
			pMsg->wParam == VK_DELETE && pWnd->GetDlgCtrlID() == IDC_CMD_PLACEHOLDER)
		{
			OnCmdOpDelete();
			return TRUE;
		}
	}	
	return CCMDlgBase::PreTranslateMessage(pMsg);
}

void CCMSlanDlg::SelectMethodMan()
{
	CArray<UINT,UINT> PointArr;
	CArray<UINT,UINT> PickingArr;
	CArray<UINT,UINT> ElemSelArr;
	
	PointArr.Add(IDC_CMD_2POINTS_EDIT1    );
	PointArr.Add(IDC_CMD_2POINTS_EDIT2    );
	PointArr.Add(IDC_CMD_2POINT_UNIT2     );
	PointArr.Add(IDC_CMD_2POINT_UNIT3     );
	PickingArr.Add(IDC_CMD_PICKING_EDIT   );
	ElemSelArr.Add(IDC_CMD_NODESELECT_EDIT);

	if(m_nSelectionMethod == 0)
	{
		CDlgUtil::CtrlShowHide(this, PointArr  , TRUE);
		CDlgUtil::CtrlShowHide(this, PickingArr, FALSE);
		CDlgUtil::CtrlShowHide(this, ElemSelArr, FALSE);
	}
	else if( m_nSelectionMethod == 1)
	{
		CRect DRect,SRect;
		
		m_wnd2PointsEdit1.GetWindowRect(&DRect);
		m_wndPickingEdit.GetWindowRect(&SRect);

		int nDistX = DRect.left - SRect.left;
		int nDistY = DRect.top  - SRect.top;

		CDlgUtil::CtrlShowHide(this, PointArr  , FALSE);
		CDlgUtil::CtrlShowHide(this, PickingArr, TRUE);
		CDlgUtil::CtrlShowHide(this, ElemSelArr, FALSE); 

		CDlgUtil::CtrlMoveDistXY(this, PickingArr, nDistX, nDistY);
	}
	else
	{
		CRect DRect, SRect;

		m_wnd2PointsEdit1 .GetWindowRect(&DRect);
		m_wndNodeSelectEdit.GetWindowRect(&SRect);

		int nDistX = DRect.left - SRect.left;
		int nDistY = DRect.top  - SRect.top;

		CDlgUtil::CtrlShowHide(this, PointArr  , FALSE);
		CDlgUtil::CtrlShowHide(this, PickingArr, FALSE);
		CDlgUtil::CtrlShowHide(this, ElemSelArr, TRUE); 

		CDlgUtil::CtrlMoveDistXY(this, ElemSelArr, nDistX, nDistY);
	}
	I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);
}



////////////////////////////////////////////////////////////////////////////
// 외부에서 Data를 Setting 하는 함수  
// 단, Dialog(Modaless)는 이미 Create&Activartion 되어있는 것을 전제로 한다.
// 이 함수를 Call 해주면 입력모드(List Mode가 아님) 상태에서 
// 인자로 넘어온 Key, Data를 Setting 하여 보여준다.
// by TAE
BOOL CCMSlanDlg::ExternalSetting(T_SLAN_K SlanKey)
{
	BOOL bFlag = TRUE;
	bFlag = ::IsWindow(this->GetSafeHwnd());                 // 윈도우가 생성되지 않았으면
	bFlag = m_pDoc->m_pAttrCtrl->ExistSlan(SlanKey);       // 키값이 존재하지 않으면
	bFlag = m_pDoc->m_pAttrCtrl->GetSlan(SlanKey, m_Data); // Data를 가져올 수 없으면
	if(!bFlag) return bFlag;

	m_csOldLaneName = m_Data.LaneName;
	m_bModify = TRUE;
	Data2Dlg();

	m_bListMode = FALSE;
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);	

	if(m_bCodeNameINDIA)
	{
		CDlgUtil::CtrlShowHide(this, m_aImpactFactor, FALSE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aImpactFactor, TRUE);
	}

	m_wndLaneName.SetFocus();
	SelectMethodMan();
	ShowHideByCode();

	return TRUE;
}

void CCMSlanDlg::OnCmdImpactorSpanRdo()
{
	UpdateData(TRUE);

	if(m_nImpactorSpan)
	{
		GetDlgItem(IDC_CMD_SLAN_IMPACT_FACTOR_EDT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SLAN_SPAN_LENGTH_EDT  )->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_SLAN_SPAN_LENGTH_UNT  )->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CMD_SLAN_IMPACT_FACTOR_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_SLAN_SPAN_LENGTH_EDT  )->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SLAN_SPAN_LENGTH_UNT  )->EnableWindow(FALSE);
	}
}

void CCMSlanDlg::OnCmdChangeOptimChk() 
{
	ShowHideByCode();
}

void CCMSlanDlg::OnSelLaneTypeRdo()
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlLaneTypeRdo, m_Data.nLaneType);
	if (m_Data.nLaneType == 1)
	{
		m_chkLaneOptim.SetCheck(0);
	}
	ShowHideByCode();
}
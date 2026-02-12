// CMLlanOpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLlanOpDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\wg_base_strParser.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GridColorMgr.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_FlagCtrl.h"  //added by KYM
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"  //added by KYM
#include "..\wg_db\wg_db_StagCtrl.h"  //added by KYM
#include "..\wg_db\VehlDB.h"


extern double GetInitialWheelSpacing(int nLaneType);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_NUM_COL     7

/////////////////////////////////////////////////////////////////////////////
// CCMLlanOpDlg dialog


CCMLlanOpDlg::CCMLlanOpDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMLlanOpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMLlanOpDlg)
	m_bStadd = FALSE;
	m_nLoadDistType = 0;
	m_nOffsetType  = 0;
	m_nMovDir = -1;
	m_nSelectionMethod = 0;
	//}}AFX_DATA_INIT
	m_bModify = FALSE;
	m_bListMode = TRUE;

	m_aCtrlList.Add(IDC_CMD_LIST);
	m_aCtrlList.Add(IDC_CMD_BTN_ADD);
	m_aCtrlList.Add(IDC_CMD_BTN_MODIFY);
	m_aCtrlList.Add(IDC_CMD_BTN_DELETE);
	m_aCtrlList.Add(IDC_CMD_BTN_COPY);
	m_aCtrlList.Add(IDC_CMD_BTN_CLOSE);

	m_aCtrlData.Add(IDC_CMD_LANE_NAME_TITLE);
	m_aCtrlData.Add(IDC_CMD_ST_LOADDIST);
	m_aCtrlData.Add(IDC_CMD_LANE_ELEM);
	m_aCtrlData.Add(IDC_CMD_CROSSBEAM);
	m_aCtrlData.Add(IDC_CMD_ST_CROSSBEAMGRUP);
	m_aCtrlData.Add(IDC_CMD_CROSSBEAM_GROUP);
	m_aCtrlData.Add(IDC_CMD_LANE_NAME_EDIT);
	m_aCtrlData.Add(IDC_CMD_FRAME1);
	m_aCtrlData.Add(IDC_CMD_ECCEN_TITLE);
	m_aCtrlData.Add(IDC_CMD_ECCEN_EDIT);
	m_aCtrlData.Add(IDC_CMD_ECCEN_UNIT);
	m_aCtrlData.Add(IDC_CMD_FACTOR_TITLE);
	m_aCtrlData.Add(IDC_CMD_FACTOR_EDIT);
	m_aCtrlData.Add(IDC_CMD_SPAN_LENGTH_STC);
	m_aCtrlData.Add(IDC_CMD_SPAN_LENGTH_EDT);
	m_aCtrlData.Add(IDC_CMD_SPAN_LENGTH_UNT);
	m_aCtrlData.Add(IDC_CMD_SCALE_FACTOR_STC);
	m_aCtrlData.Add(IDC_CMD_SCALE_FACTOR_EDT);
	m_aCtrlData.Add(IDC_CMD_STRADD_CHK);
	m_aCtrlData.Add(IDC_CMD_FRAME7);
	m_aCtrlData.Add(IDC_CMD_2POINT_UNIT1);
	m_aCtrlData.Add(IDC_CMD_2POINT_UNIT2);
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
	m_aCtrlData.Add(IDC_CMD_ELEMSELECT);
	m_aCtrlData.Add(IDC_CMD_ELEMSELECT_EDIT);
	m_aCtrlData.Add(IDC_CMD_ST_MOVINGDIR);
	m_aCtrlData.Add(IDC_CMD_MOV_FORWARD );
	m_aCtrlData.Add(IDC_CMD_MOV_BACKWARD);
	m_aCtrlData.Add(IDC_CMD_MOV_BOTH    );
	m_aCtrlData.Add(IDC_CMD_BMP_STATIC	       );
	m_aCtrlData.Add(IDC_CMD_BMP_STATIC_GRP);
	m_aCtrlData.Add(IDC_CMD_SKEW_TITLE_STATIC);
	m_aCtrlData.Add(IDC_CMD_SK_START_STATIC);
	m_aCtrlData.Add(IDC_CMD_SK_END_STATIC);
	m_aCtrlData.Add(IDC_CMD_SK_START_EDIT);
	m_aCtrlData.Add(IDC_CMD_SK_END_EDIT);
	m_aCtrlData.Add(IDC_CMD_SK_DEG_UNIT);
	m_aCtrlData.Add(IDC_CMD_WIDTH_STC);
	m_aCtrlData.Add(IDC_CMD_WIDTH_EDT);
	m_aCtrlData.Add(IDC_CMD_WIDTH_UNT);
	m_aCtrlData.Add(IDC_CMD_OPTIM_WIDTH_STC);
	m_aCtrlData.Add(IDC_CMD_OPTIM_WIDTH_EDT);
	m_aCtrlData.Add(IDC_CMD_OPTIM_WIDTH_UNT);
	m_aCtrlData.Add(IDC_CMD_OFFSET_TYPE_GRP);
	m_aCtrlData.Add(IDC_CMD_ANAL_OFFSET_RDO);
	m_aCtrlData.Add(IDC_CMD_ANAL_OFFSET_EDT);
	m_aCtrlData.Add(IDC_CMD_ANAL_OFFSET_UNT);
	m_aCtrlData.Add(IDC_CMD_EQUAL_DIVIDE_RDO);
	m_aCtrlData.Add(IDC_CMD_EQUAL_DIVIDE_EDT);
	m_aCtrlData.Add(IDC_CMD_EQUAL_DIVIDE_N_STC);
	m_aCtrlData.Add(IDC_CMD_MARGIN_STC);
	m_aCtrlData.Add(IDC_CMD_MARGIN_EDT);
	m_aCtrlData.Add(IDC_CMD_MARGIN_UNT);
	m_aCtrlData.Add(IDC_CMD_WHEEL_TXT);
	m_aCtrlData.Add(IDC_CMD_WHEEL_EDT);
	m_aCtrlData.Add(IDC_CMD_WHEEL_UNT);
	// Centrifugal Force
	m_aCtrlCF.RemoveAll();
	m_aCtrlCF.Add(IDC_CMD_LANE_CF_GRP);
	m_aCtrlCF.Add(IDC_CMD_LANE_CF_STC);
	m_aCtrlCF.Add(IDC_CMD_LANE_CF_STC2);
	m_aCtrlCF.Add(IDC_CMD_LANE_CF_STC3);
	m_aCtrlCF.Add(IDC_CMD_LANE_CF_EDT);
	m_aCtrlData.Append(m_aCtrlCF);
	
	m_aCtrlSelect.Add(IDC_CMD_2POINTS);
	m_aCtrlSelect.Add(IDC_CMD_PICKING);
	m_aCtrlSelect.Add(IDC_CMD_ELEMSELECT);
}


void CCMLlanOpDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLlanOpDlg)
	DDX_Control(pDX, IDC_CMD_LANE_NAME_EDIT, m_wndLaneName);
	DDX_Control(pDX, IDC_CMD_OPTIM_WIDTH_EDT, m_edtOptimWidth);
	DDX_Control(pDX, IDC_CMD_OPTIM_WIDTH_UNT, m_untOptimWidth);
	DDX_Control(pDX, IDC_CMD_WIDTH_EDT, m_edtWidth);
	DDX_Control(pDX, IDC_CMD_WIDTH_UNT, m_untWidth);
	DDX_Control(pDX, IDC_CMD_ANAL_OFFSET_EDT, m_edtAnalOffset);
	DDX_Control(pDX, IDC_CMD_ANAL_OFFSET_UNT, m_untAnalOffset);
	DDX_Control(pDX, IDC_CMD_EQUAL_DIVIDE_EDT, m_edtEqualLaneDivide);
	DDX_Control(pDX, IDC_CMD_WHEEL_EDT, m_edtWheelSpace);
	DDX_Control(pDX, IDC_CMD_WHEEL_UNT, m_untWheelSpace);
	DDX_Control(pDX, IDC_CMD_MARGIN_EDT, m_edtMargin);
	DDX_Control(pDX, IDC_CMD_MARGIN_UNT, m_untMargin);
	DDX_Control(pDX, IDC_CMD_ECCEN_EDIT, m_wndEccenEdit);
	DDX_Control(pDX, IDC_CMD_ECCEN_UNIT, m_wndEccenUnit);
	DDX_Control(pDX, IDC_CMD_FACTOR_EDIT, m_wndFactor);
	DDX_Control(pDX, IDC_CMD_LANE_CF_EDT, m_wndCFEdit);
	DDX_Control(pDX, IDC_CMD_SPAN_LENGTH_EDT, m_edtSpanLength);
	DDX_Control(pDX, IDC_CMD_SPAN_LENGTH_UNT, m_untSpanLength);
	DDX_Control(pDX, IDC_CMD_SCALE_FACTOR_EDT, m_edtScaleFactor);
	DDX_Check  (pDX, IDC_CMD_STRADD_CHK, m_bStadd);
	DDX_Radio  (pDX, IDC_CMD_LANE_ELEM, m_nLoadDistType);
	DDX_Radio  (pDX, IDC_CMD_EQUAL_DIVIDE_RDO, m_nOffsetType);
	DDX_Control(pDX, IDC_CMD_CROSSBEAM_GROUP, m_cmbCrossBeamElemGroup);
	DDX_Control(pDX, IDC_CMD_SK_START_EDIT, m_edtSkewStart);
	DDX_Control(pDX, IDC_CMD_SK_END_EDIT, m_edtSkewEnd);
	DDX_Radio  (pDX, IDC_CMD_MOV_FORWARD, m_nMovDir);
	DDX_Radio  (pDX, IDC_CMD_2POINTS, m_nSelectionMethod);
	DDX_Control(pDX, IDC_CMD_2POINTS_EDIT1, m_wnd2PointsEdit1);
	DDX_Control(pDX, IDC_CMD_2POINT_UNIT1, m_2PointUnit1);
	DDX_Control(pDX, IDC_CMD_2POINTS_EDIT2, m_wnd2PointsEdit2);
	DDX_Control(pDX, IDC_CMD_2POINT_UNIT2, m_2PointUnit2);
	DDX_Control(pDX, IDC_CMD_ELEMSELECT_EDIT, m_wndElemSelectEdit);
	DDX_Control(pDX, IDC_CMD_PICKING_EDIT, m_wndPickingEdit);
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_BMP_STATIC_GRP, m_chkBmp);
	DDX_Control(pDX, IDC_CMD_PLACEHOLDER, m_wndGrid);

	DDX_Control(pDX, IDC_CMD_BMP_STATIC, m_ImgViewer);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMLlanOpDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMLlanOpDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD,    OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE,  OnCmdBtnClose)
	ON_BN_CLICKED(IDC_CMD_BTN_COPY,   OnCmdBtnCopy)
	ON_BN_CLICKED(IDC_CMD_BTN_OK,     OnCmdBtnOk)
	ON_BN_CLICKED(IDC_CMD_BTN_CANCEL, OnCmdBtnCancel)
	ON_BN_CLICKED(IDC_CMD_BTN_APPLY,  OnCmdBtnApply)
	ON_BN_CLICKED(IDC_CMD_OP_ADD,     OnCmdOpAdd)
	ON_BN_CLICKED(IDC_CMD_OP_INSERT,  OnCmdOpInsert)
	ON_BN_CLICKED(IDC_CMD_OP_DELETE,  OnCmdOpDelete)
	ON_WM_PAINT()
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	ON_BN_CLICKED(IDC_CMD_LANE_ELEM,   OnCmdLoadDistType)
	ON_BN_CLICKED(IDC_CMD_CROSSBEAM,   OnCmdLoadDistType)
	ON_BN_CLICKED(IDC_CMD_ANAL_OFFSET_RDO,  OnAnalOffsetType)
	ON_BN_CLICKED(IDC_CMD_EQUAL_DIVIDE_RDO, OnAnalOffsetType)
	ON_BN_CLICKED(IDC_CMD_2POINTS,    OnCmdChangeSelectBy)
	ON_BN_CLICKED(IDC_CMD_PICKING,    OnCmdChangeSelectBy)
	ON_BN_CLICKED(IDC_CMD_ELEMSELECT, OnCmdChangeSelectBy)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

#define COLCOUNT 1
/////////////////////////////////////////////////////////////////////////////
// CCMLlanOpDlg implementation functions
void CCMLlanOpDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY= 0;
	int nDistX= 0;

	// Traffic Line Lane List 위치로 전부 이동
	GetDlgItem(IDC_CMD_LIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_LANE_NAME_EDIT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlData, nDistY);

	// Traffic Lane Properties 안에 그림을 제외한 컨트롤 이동 (Fold Group Box Auto Effect 꺼서 들어간 코드...)
	CArray<UINT, UINT> aControls;
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_WIDTH_STC);
	aControls.Add(IDC_CMD_WIDTH_EDT);
	aControls.Add(IDC_CMD_WIDTH_UNT);
	aControls.Add(IDC_CMD_ECCEN_TITLE);
	aControls.Add(IDC_CMD_ECCEN_EDIT);
	aControls.Add(IDC_CMD_ECCEN_UNIT);
	aControls.Add(IDC_CMD_WHEEL_TXT);
	aControls.Add(IDC_CMD_WHEEL_EDT);
	aControls.Add(IDC_CMD_WHEEL_UNT);
	aControls.Add(IDC_CMD_FACTOR_TITLE);
	aControls.Add(IDC_CMD_FACTOR_EDIT);		
	aControls.Add(IDC_CMD_SPAN_LENGTH_STC);
	aControls.Add(IDC_CMD_SPAN_LENGTH_EDT);
	aControls.Add(IDC_CMD_SPAN_LENGTH_UNT);
	aControls.Add(IDC_CMD_SCALE_FACTOR_STC);
	aControls.Add(IDC_CMD_SCALE_FACTOR_EDT);
	aControls.Add(IDC_CMD_OPTIM_WIDTH_STC);
	aControls.Add(IDC_CMD_OPTIM_WIDTH_EDT);
	aControls.Add(IDC_CMD_OPTIM_WIDTH_UNT);
	aControls.Add(IDC_CMD_OFFSET_TYPE_GRP);
	aControls.Add(IDC_CMD_ANAL_OFFSET_RDO);
	aControls.Add(IDC_CMD_ANAL_OFFSET_EDT);
	aControls.Add(IDC_CMD_ANAL_OFFSET_UNT);
	aControls.Add(IDC_CMD_EQUAL_DIVIDE_RDO);
	aControls.Add(IDC_CMD_EQUAL_DIVIDE_EDT);
	aControls.Add(IDC_CMD_EQUAL_DIVIDE_N_STC);
	aControls.Add(IDC_CMD_MARGIN_STC);
	aControls.Add(IDC_CMD_MARGIN_EDT);
	aControls.Add(IDC_CMD_MARGIN_UNT);
	aControls.Append(m_aCtrlCF);
	
	GetDlgItem(IDC_CMD_BMP_STATIC_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_OPTIM_WIDTH_EDT)->GetWindowRect(rToMove);
	if(m_chkBmp.GetFoldState()) nDistY = rRef.bottom - rToMove.top;
	else                        nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// 기준 별 Dlg Controls 위치 조정
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	T_MVCD_D MvcdData; MvcdData.Initialize();
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();
	int nCode = MvcdData.nCodeType;

	if(nCode == D_MOVE_CODE_CHINA)
	{
		CArray<UINT, UINT> aControls;
		aControls.RemoveAll();
		aControls.Add(IDC_CMD_SPAN_LENGTH_STC);
		aControls.Add(IDC_CMD_SPAN_LENGTH_EDT);
		aControls.Add(IDC_CMD_SPAN_LENGTH_UNT);
		aControls.Add(IDC_CMD_SCALE_FACTOR_STC);
		aControls.Add(IDC_CMD_SCALE_FACTOR_EDT);

		GetDlgItem(IDC_CMD_FACTOR_EDIT)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_SPAN_LENGTH_EDT)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
	}


	// Centrifugal Force
// 	if(nCode == D_MOVE_CODE_AASHTO_STAN)
// 	{
// 		CRect rRef2;
// 		CRect rToMove2;
// 		GetDlgItem(IDC_CMD_ECCEN_TITLE)->GetWindowRect(rRef2);
// 		GetDlgItem(IDC_CMD_FACTOR_TITLE)->GetWindowRect(rToMove2);
// 		int nDistY2 = rRef2.bottom - rToMove2.top;
// 
// 		GetDlgItem(IDC_CMD_FACTOR_TITLE)->GetWindowRect(rRef);
// 		GetDlgItem(IDC_CMD_LANE_CF_GRP)->GetWindowRect(rToMove);
// 		//nDistX = rRef.left - rToMove.left;
// 		nDistY = rRef.bottom - rToMove.top - nDistY2;
// 		CDlgUtil::CtrlMoveDistXY(this, m_aCtrlCF, nDistX, nDistY);
// 
// 	}
	if(nCode == D_MOVE_CODE_AASHTO_LRFD || nCode == D_MOVE_CODE_CANADA)
	{
		CRect rRef2;
		CRect rToMove2;
		GetDlgItem(IDC_CMD_ECCEN_TITLE)->GetWindowRect(rRef2);
		GetDlgItem(IDC_CMD_FACTOR_TITLE)->GetWindowRect(rToMove2);
		int nDistY2 = rRef2.bottom - rToMove2.top;

		GetDlgItem(IDC_CMD_ECCEN_TITLE)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_LANE_CF_GRP)->GetWindowRect(rToMove);
		//nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top - nDistY2;
		CDlgUtil::CtrlMoveDistXY(this, m_aCtrlCF, nDistX, nDistY);
	}


	// Traffic Lane Properties
	CRect rectResize;
	GetDlgItem(IDC_CMD_ST_LOADDIST)->GetWindowRect(rectResize);

	if(  nCode == D_MOVE_CODE_AASHTO_STAN || nCode == D_MOVE_CODE_TAIWAN || 
		 nCode == D_MOVE_CODE_KOREA || nCode == D_MOVE_CODE_BRAZIL)
	{
		GetDlgItem(IDC_CMD_FACTOR_EDIT)->GetWindowRect(rRef);
		rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	}
	else if(/*nCode == D_MOVE_CODE_AASHTO_LRFD     || nCode == D_MOVE_CODE_CANADA    ||*/
		      nCode == D_MOVE_CODE_PENDOT          || nCode == D_MOVE_CODE_BS        || 
					nCode == D_MOVE_CODE_EURO_BS         || nCode == D_MOVE_CODE_RUSSIA    || 
					nCode == D_MOVE_CODE_KOREA_LRFD_2011 ||	nCode == D_MOVE_CODE_AUSTRALIA ||
					nCode == D_MOVE_CODE_POLAND          || nCode == D_MOVE_CODE_SOUTH_AFRICA ||
					nCode == D_MOVE_CODE_NEWZEALAND      || nCode == D_MOVE_CODE_BRAZIL)
	{
		GetDlgItem(IDC_CMD_ECCEN_EDIT)->GetWindowRect(rRef);
		rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	}
	else if(nCode == D_MOVE_CODE_CHINA)
	{
		GetDlgItem(IDC_CMD_SCALE_FACTOR_EDT)->GetWindowRect(rRef);
		rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	}
	else if(nCode == D_MOVE_CODE_AASHTO_LRFD || nCode == D_MOVE_CODE_CANADA)
	{
		GetDlgItem(IDC_CMD_LANE_CF_GRP)->GetWindowRect(rRef);
		rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	}
	else
	{
		ASSERT(0)	;
	}
	ScreenToClient(rectResize);
	GetDlgItem(IDC_CMD_ST_LOADDIST)->MoveWindow(rectResize);

	// StraddLing Lane Type
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_STRADD_CHK);

	GetDlgItem(IDC_CMD_ST_LOADDIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_STRADD_CHK)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
		
	// Vehicular Load Distribution
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_FRAME1);
	aControls.Add(IDC_CMD_LANE_ELEM);
	aControls.Add(IDC_CMD_CROSSBEAM);
	aControls.Add(IDC_CMD_ST_CROSSBEAMGRUP);
	aControls.Add(IDC_CMD_CROSSBEAM_GROUP);
	aControls.Add(IDC_CMD_SKEW_TITLE_STATIC);
	aControls.Add(IDC_CMD_SK_START_STATIC);
	aControls.Add(IDC_CMD_SK_END_STATIC);
	aControls.Add(IDC_CMD_SK_START_EDIT);
	aControls.Add(IDC_CMD_SK_END_EDIT);
	aControls.Add(IDC_CMD_SK_DEG_UNIT);

	if(nCode == D_MOVE_CODE_EURO_BS || nCode == D_MOVE_CODE_BS || nCode == D_MOVE_CODE_AUSTRALIA)
	{
		GetDlgItem(IDC_CMD_STRADD_CHK)->GetWindowRect(rRef);
	}
	else
	{
		GetDlgItem(IDC_CMD_ST_LOADDIST)->GetWindowRect(rRef);
	}

	GetDlgItem(IDC_CMD_FRAME1)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
		
	// Moving Direction
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_ST_MOVINGDIR);
	aControls.Add(IDC_CMD_MOV_FORWARD);
	aControls.Add(IDC_CMD_MOV_BACKWARD);
	aControls.Add(IDC_CMD_MOV_BOTH);

	GetDlgItem(IDC_CMD_FRAME1)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_ST_MOVINGDIR)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// Selection by
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_FRAME7);
	aControls.Add(IDC_CMD_2POINTS);
	aControls.Add(IDC_CMD_PICKING);
	aControls.Add(IDC_CMD_ELEMSELECT);
	aControls.Add(IDC_CMD_2POINTS_EDIT1);
	aControls.Add(IDC_CMD_2POINT_UNIT1);
	aControls.Add(IDC_CMD_2POINTS_EDIT2);
	aControls.Add(IDC_CMD_2POINT_UNIT2);

	GetDlgItem(IDC_CMD_ST_MOVINGDIR)->GetWindowRect(rRef); 
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
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	ResizeDialog();
}

void CCMLlanOpDlg::ResizeDialog()
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

void CCMLlanOpDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Lane_Name)};
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

void CCMLlanOpDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	T_LLANop_D LlanD;
	CArray<T_LLANop_K, T_LLANop_K> aLlanK;
	m_pDoc->m_pAttrCtrl->GetLlanopKeyList(aLlanK);

	for (int i = 0; i < aLlanK.GetSize(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetLlanop(aLlanK[i], LlanD)) continue;

		InsertItem(aLlanK[i], LlanD);
	}
}

void CCMLlanOpDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
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

void CCMLlanOpDlg::ButtonCtrl()
{
	BOOL bEnable = FALSE;
	if (m_pDoc->m_pStagCtrl->IsFinalStag() || m_pDoc->m_pStagCtrl->IsBaseStag())
		bEnable = TRUE;

	GetDlgItem(IDC_CMD_BTN_OK)->EnableWindow(bEnable);
}

void CCMLlanOpDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_LLANop_K Key, KeyBak;
	T_LLANop_D Data, DataBak;

	BOOL bMFD = FALSE;
	BOOL bMvcdChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_LLANop_ADD):
			{
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetLlanop(nKey, Key, Data);
				InsertItem(Key, Data);
			}
			break;
		case(UR_LLANop_DEL):
			{
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetLlanop(nKey, Key, Data);
				DeleteItem(Key, Data);
			}
			break;
		case(UR_LLANop_MFD):
			{
				pViewBuff->GetLlanop(nKey, KeyBak, DataBak);
			}
			bMFD = TRUE;
			break;
		case(UR_LLANop_MFS):
			{
				pViewBuff->GetLlanop(nKey, Key, Data);
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

void CCMLlanOpDlg::UpdateUnit()
{
	SetGridTitle();
	if (m_bListMode) return;  // unit change할 게 없다.
	
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	double dblEccen = 0.0;
	double dblSpanLength = 0.0;
	int nCount = m_wndGrid.GetRowCount();
	for (int i = 0; i < nCount; i++)
	{
		dblEccen = _tstof(m_wndGrid.GetValueRowCol(i+1, 2));
		dblEccen = m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(CUnitCtrl::m_LLANop_UNIT.Eccentricity, dblEccen);
		m_wndGrid.SetValueRange(CGXRange(i+1, 2), dblEccen);

		dblSpanLength = _tstof(m_wndGrid.GetValueRowCol(i+1, 3));
		dblSpanLength = m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(CUnitCtrl::m_LLANop_UNIT.dSpan, dblSpanLength);
		m_wndGrid.SetValueRange(CGXRange(i+1, 3), dblSpanLength);
	}
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

int CCMLlanOpDlg::FindInsertionPos(CString& strName)
{
	T_LLANop_K key;
	T_LLANop_D data;
	int nCount = m_List.GetItemCount();
	int i = 0;
	for (i = 0; i < nCount; i++)
	{
		key = m_List.GetItemData(i);
		m_pDoc->m_pAttrCtrl->GetLlanop(key, data);

		if (strName.Compare(data.LineLaneName) < 0) break;
	}
	return i;    
}

BOOL CCMLlanOpDlg::InsertItem(T_LLANop_K Key, T_LLANop_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();

	lvitem.iItem = FindInsertionPos(Data.LineLaneName);
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

BOOL CCMLlanOpDlg::DeleteItem(T_LLANop_K Key, T_LLANop_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CCMLlanOpDlg::ModifyItem(T_LLANop_K KeyOld, T_LLANop_K Key, T_LLANop_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr(i, Key, Data);
			m_List.SetItemText(nItem, i, str);
		}
		// key 재 설정 
		LVITEM lvitem;
		lvitem.iItem = nItem;
		lvitem.iSubItem=0;
		lvitem.mask = LVIF_PARAM;
		lvitem.lParam = (LPARAM)Key;
		m_List.SetItem(&lvitem);
	}
	return TRUE;
}

CString CCMLlanOpDlg::DataToStr(int i, T_LLANop_K Key, T_LLANop_D &Data)
{
	CString str;
	if (i == 0) str = Data.LineLaneName;
	else str = _T("");

	return str;
}

void CCMLlanOpDlg::Data2Dlg()
{
	m_wndLaneName.SetWindowText(m_Data.LineLaneName);
	InitItemDataVar();
	m_nLoadDistType = m_Data.nLoadDistribution -1;
	m_nOffsetType =  m_Data.nOffsetType;
	
	CDlgUtil::CobxSetCurSelItemData(m_cmbCrossBeamElemGroup, m_Data.ElemGroup);

	CString str;

	str.Format(_T("%g"), m_Data.dSkewStart);
	m_edtSkewStart.SetWindowText(str);

	str.Format(_T("%g"), m_Data.dSkewEnd);
	m_edtSkewEnd.SetWindowText(str);
		
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	int nSize = m_Data.arLaneData.GetSize();
	m_wndGrid.SetRowCount(nSize);
	for (int i = 0; i < nSize; i++)
	{
		m_wndGrid.SetValueRange(CGXRange(i+1, 1), (long)m_Data.arLaneData[i].KeyElem);
		m_wndGrid.SetValueRange(CGXRange(i+1, 2), m_Data.arLaneData[i].Eccentricity);
		m_wndGrid.SetValueRange(CGXRange(i+1, 3), m_Data.arLaneData[i].dSpan);
		m_wndGrid.SetValueRange(CGXRange(i+1, 4), m_Data.arLaneData[i].fact);

		CString str = (m_Data.arLaneData[i].bSpanStart) ? _T("1") : _T("0");
		m_wndGrid.SetValueRange(CGXRange(i+1, 5), str);

		m_wndGrid.SetValueRange(CGXRange(i+1, 6), m_Data.arLaneData[i].dScaleFactor);
		m_wndGrid.SetValueRange(CGXRange(i+1, 7), m_Data.arLaneData[i].dCentF);
	}
	if (nSize > 0) m_wndGrid.SetCurrentCell(1, 1);
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();

	m_nMovDir = m_Data.nMovingDir;

	m_edtOptimWidth.SetEditUnit(m_Data.dOptimWidth);
	m_edtWidth.SetEditUnit(m_Data.dLaneWidth);
	m_edtAnalOffset.SetEditUnit(m_Data.dAnalLaneOffset);
	m_edtEqualLaneDivide.SetEditUnit(m_Data.nDivideNum);
	m_edtWheelSpace.SetEditUnit(m_Data.dWheelSpace);
	m_edtMargin.SetEditUnit(m_Data.dMargin);
	m_bStadd = m_Data.bStradd;
	
	UpdateData(FALSE);
	OnCmdLoadDistType();
	OnAnalOffsetType();

	SelectElem();			// added by KYM
}

BOOL CCMLlanOpDlg::Dlg2Data()
{
	T_LLANop_BASE item; item.Initialize();
//	double dx,dy,dz,dux,duy,duz;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	UpdateData(TRUE);

	m_wndLaneName.GetWindowText(m_Data.LineLaneName);
	int nCount = m_wndGrid.GetRowCount();
	m_Data.nLoadDistribution = m_nLoadDistType +1;
	m_Data.nOffsetType = m_nOffsetType;

	if(m_nLoadDistType == 1)
	{
		DWORD dwCobxItemData = 0;
		CDlgUtil::CobxGetItemDataByCurSel(m_cmbCrossBeamElemGroup, dwCobxItemData);  
		m_Data.ElemGroup = (T_GRUP_K)dwCobxItemData;

		m_edtSkewStart.GetEditValue(m_Data.dSkewStart);
		m_edtSkewEnd.GetEditValue(m_Data.dSkewEnd);
	}

	m_Data.dOptimWidth = m_edtOptimWidth.GetEditValue();
	m_Data.dLaneWidth = m_edtWidth.GetEditValue();

	if(m_nOffsetType==0) m_Data.nDivideNum = m_edtEqualLaneDivide.GetEditValue();
	else                 m_Data.dAnalLaneOffset = m_edtAnalOffset.GetEditValue();

	m_Data.dWheelSpace = m_edtWheelSpace.GetEditValue();
	m_Data.dMargin = m_edtMargin.GetEditValue();
	m_Data.bStradd = m_bStadd;
	
	m_Data.arLaneData.SetSize(nCount);
	for (int i = 0; i < nCount; i++)
	{
		item.KeyElem = _tstol(m_wndGrid.GetValueRowCol(i+1, 1));
		item.Eccentricity = _tstof(m_wndGrid.GetValueRowCol(i+1, 2));
		item.dSpan = _tstof(m_wndGrid.GetValueRowCol(i+1, 3));
		item.fact = _tstof(m_wndGrid.GetValueRowCol(i+1, 4));
		item.bSpanStart = (m_wndGrid.GetValueRowCol(i+1, 5)==_T("1")) ? TRUE : FALSE;
		item.dScaleFactor = _tstof(m_wndGrid.GetValueRowCol(i+1, 6));
		item.dCentF = _tstof(m_wndGrid.GetValueRowCol(i+1, 7));
		m_Data.arLaneData.SetAt(i, item);
	}
	m_Data.nMovingDir = m_nMovDir;

	return TRUE;
}

void CCMLlanOpDlg::InitItemDataVar()
{
	m_wndEccenEdit.SetWindowText(_T("0.0"));
	m_wndFactor.SetWindowText(_T("0.0"));
	m_wndCFEdit.SetWindowText(_T("0.5"));
	m_edtScaleFactor.SetEditUnit(1.0);
	m_wnd2PointsEdit1.SetWindowText(_LS(IDS_WG_CMD__ADDD__0___0___0));
	m_wnd2PointsEdit2.SetWindowText(_LS(IDS_WG_CMD__ADDD__0___0___0));
	m_wndPickingEdit.SetWindowText(_T(""));
	OnCmdLoadDistType();
	OnAnalOffsetType();
}

void CCMLlanOpDlg::InitCombo()
{
	m_cmbCrossBeamElemGroup.ResetContent();

	CArray<T_GRUP_K, T_GRUP_K>GrupKeyList;
	m_pDoc->m_pAttrCtrl->GetGrupKeyList(GrupKeyList);

	T_GRUP_D rData;
	int nGrup=GrupKeyList.GetSize();
	for(int i=0; i<nGrup; i++)
	{
		T_GRUP_K Key = GrupKeyList[i];
		if(!m_pDoc->m_pAttrCtrl->GetGrup(Key, rData)) continue;

		CDlgUtil::CobxAddItem(m_cmbCrossBeamElemGroup, rData.GroupName, Key );
	}

	CDlgUtil::CobxSetCurSelItemData(m_cmbCrossBeamElemGroup, m_Data.ElemGroup);

}

void CCMLlanOpDlg::InitGrid()
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

	m_wndGrid.SetColCount(D_NUM_COL);
	SetGridTitle();
	m_wndGrid.SetColWidthByDPI(0, 0, 25);
	m_wndGrid.SetColWidthByDPI(1, 1, 35);
	m_wndGrid.SetColWidthByDPI(2, 2, 55);
	m_wndGrid.SetColWidthByDPI(3, 3, 55);
	m_wndGrid.SetColWidthByDPI(4, 4, 55);
	m_wndGrid.SetColWidthByDPI(5, 5, 40);
	// Check Box Column의 스타일 변경
	m_wndGrid.SetStyleRange(CGXRange().SetCols(5), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D)
			.SetValueType(GX_VT_NUMERIC)
			.SetHorizontalAlignment(DT_CENTER)
			.SetVerticalAlignment(DT_VCENTER)
			//.SetValue(_T("0"))
			);
	m_wndGrid.SetColWidthByDPI(6, 6, 55);
	m_wndGrid.SetColWidthByDPI(7, 7, 35);

	//m_wndGrid.EnableOleDataSource(GX_DNDDISABLED); // copy & paste 금지

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMLlanOpDlg::SetGridTitle()
{
	CString csEccUnit, csSpanUnit;
	m_pDoc->m_pUnitCtrl->GetUnitSystem(CUnitCtrl::m_LLANop_UNIT.Eccentricity, csEccUnit);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(CUnitCtrl::m_LLANop_UNIT.dSpan, csSpanUnit);

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	m_wndGrid.SetValueRange(CGXRange(0, 0), _LS(IDS_WG_CMD__ADDD__No));
	m_wndGrid.SetValueRange(CGXRange(0, 1), _LS(IDS_WG_CMD__ADDD__Elem));
	m_wndGrid.SetValueRange(CGXRange(0, 2), CString(_LS(IDS_WG_CMD__ADDD__Eccen__n_))+csEccUnit+_T(")"));
	m_wndGrid.SetValueRange(CGXRange(0, 3), CString(_LS(IDS_WG_CMD__ADDD__Span__n__))+csSpanUnit+_T(")"));
	m_wndGrid.SetValueRange(CGXRange(0, 4), _LS(IDS_WG_CMD__ADDD__Impact_nFactor));
	m_wndGrid.SetValueRange(CGXRange(0, 5), _LS(IDS_CMD_SLAN_SPAN_START));
	m_wndGrid.SetValueRange(CGXRange(0, 6), _LS(IDS_CMD_IMPF_ScaleFactor));
	m_wndGrid.SetValueRange(CGXRange(0, 7), _LS(IDS_WG_CMD__ADDD__Centrifugal_Force));
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

BOOL CCMLlanOpDlg::Grid_IsExistKey(int nKey)
{
	int nCount = m_wndGrid.GetRowCount();
	for (int i = 0; i < nCount; i++)
	{
		if (nKey == _tstol(m_wndGrid.GetValueRowCol(i+1, 1))) return TRUE;
	}
	return FALSE;
}

void CCMLlanOpDlg::Grid_InsertItem(int nPos, T_LLANop_BASE &item)
{
	ROWCOL nRow;
	if (nPos < 0 || nPos > m_wndGrid.GetRowCount()) nRow = m_wndGrid.GetRowCount();
	else nRow = nPos;
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	m_wndGrid.InsertRows(nRow+1, 1);
	m_wndGrid.SetValueRange(CGXRange(nRow+1, 1), (long)item.KeyElem);
	m_wndGrid.SetValueRange(CGXRange(nRow+1, 2), item.Eccentricity);
	m_wndGrid.SetValueRange(CGXRange(nRow+1, 3), item.dSpan);
	m_wndGrid.SetValueRange(CGXRange(nRow+1, 4), item.fact);
	
	CString str = _T("0");
	m_wndGrid.SetValueRange(CGXRange(nRow+1, 5), str);

	m_wndGrid.SetValueRange(CGXRange(nRow+1, 6), item.dScaleFactor);

	m_wndGrid.SetValueRange(CGXRange(nRow+1, 7), item.dCentF);

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMLlanOpDlg::InsertBaseItem(int nPos, int nType)
{
	T_LLANop_BASE item;
	CFormulaEdit::GetEditValue(&m_wndEccenEdit, item.Eccentricity);
	CFormulaEdit::GetEditValue(&m_wndFactor, item.fact);
	CFormulaEdit::GetEditValue(&m_edtSpanLength, item.dSpan);
	CFormulaEdit::GetEditValue(&m_edtScaleFactor, item.dScaleFactor);
	CFormulaEdit::GetEditValue(&m_wndCFEdit, item.dCentF);

	if(nType==0) //Select by 2 Points
	{
		CArray<T_ELEM_K, T_ELEM_K> aElemKey;
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

		// Get Elem Key List
		double dbTolerance = m_pDoc->m_pFlagCtrl->GetGeneralCoordinateToleranceCurrentUnit();  // added by KYM
		m_pDoc->m_pDataCtrl->GetFrameElemOnLine(WX1, WY1, WZ1, WX2, WY2, WZ2, aElemKey, TRUE);
		// Remove Element which is not beam
		T_ELEM_D Data;
		for (int i = aElemKey.GetSize()-1; i >= 0; i--)
		{
			m_pDoc->m_pAttrCtrl->GetElem(aElemKey[i], Data);
			if (!m_pDoc->m_pAttrCtrl->IsBeam(Data.eltyp))
				aElemKey.RemoveAt(i);
		}
		if (aElemKey.GetSize() == 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_beam_element_on_line_));
			return;
		}

		int nElemKey;
		int nCount = aElemKey.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			nElemKey = aElemKey[i];
			if (Grid_IsExistKey(nElemKey))
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No___d_is_already), nElemKey);
				AfxMessageBox(msg);
				return;
			}
			item.KeyElem = (unsigned int)nElemKey;
			Grid_InsertItem(nPos, item);
			if (nPos >= 0) nPos++;
		}
		SelectElem();    //added by KYM
		return;
	}

	else if(nType==1) //Select by Picking
	{
		int nElemKey;
		CFormulaEdit::GetEditValue(&m_wndPickingEdit, nElemKey);
		if (nElemKey <= 0)
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No___d_is_not_exi), nElemKey);
			AfxMessageBox(msg);
			return;
		}
		if (Grid_IsExistKey(nElemKey))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No__d__is_already), nElemKey);
			AfxMessageBox(msg);
			return;
		}
		T_ELEM_D Data;
		m_pDoc->m_pAttrCtrl->GetElem(nElemKey, Data);
		if (!m_pDoc->m_pAttrCtrl->IsBeam(Data.eltyp))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No___d_is_not_a_b), nElemKey);
			AfxMessageBox(msg);
			return;
		}
		item.KeyElem = (unsigned int)nElemKey;
		Grid_InsertItem(nPos, item);
	}

	else if(nType==2)  //Select Elem Number
	{
		CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
		CArray<long, long> arElemList;

		CString strElemNumber;
	  m_wndElemSelectEdit.GetWindowText(strElemNumber);

		m_pDoc->m_pAttrCtrl->GetElemKeyList(arElemKeyList);

		CNumericOptimizer optimizer;

		optimizer.SelectNumByStrBase((long*)arElemKeyList.GetData(), (long)arElemKeyList.GetSize(),
			                            strElemNumber, arElemList);

		if(arElemList.GetSize() <= 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_element_));
			return;
		}

		for(int i=0; i<arElemList.GetSize(); i++)
		{
			if (Grid_IsExistKey((UINT)arElemList[i]))
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No__d__is_already), (UINT)arElemList[i]);
				AfxMessageBox(msg);
				return;
			}
		}

		int nInsertPos = nPos;
		for(int i=0; i<arElemList.GetSize(); i++)
		{
			T_ELEM_D Data;
			m_pDoc->m_pAttrCtrl->GetElem((UINT)arElemList[i], Data);

			if (!m_pDoc->m_pAttrCtrl->IsBeam(Data.eltyp))
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Element_No___d_is_not_a_b), (UINT)arElemList[i]);
				AfxMessageBox(msg);
				return;
			}
			item.KeyElem = (UINT)arElemList[i];
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
	SelectElem();   // added by KYM
}

//================== added by KYM : start ====================
void CCMLlanOpDlg::SelectElem()
{
	int nCount = m_wndGrid.GetRowCount();
	if(nCount >= 0)
	{
	  CArray<T_ELEM_K, T_ELEM_K>arSelElem;
	  for (int i = 0; i < nCount; i++) arSelElem.Add(_tstol(m_wndGrid.GetValueRowCol(i+1, 1)));
	  m_pDoc->m_pViewCtrl->SelectElem(NULL, arSelElem, FALSE, TRUE);
	}
}
//================== added by KYM : end ======================

/////////////////////////////////////////////////////////////////////////////
// CCMLlanOpDlg message handlers

BOOL CCMLlanOpDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();

	// Auto Effect 끄자 (Auto Effect 가 왜 안먹을까...)
	m_chkBmp.InitControl(this, CCMLlanOpDlg::IDD, IDC_CMD_BMP_STATIC_GRP,FALSE,TRUE);
	m_chkBmp.SetFoldState(FALSE);
	// TODO: Add extra initialization here
	AlignControl();
	SetHeaderTitle();
	MakeItemEx();
	ButtonCtrl();

	m_edtSkewStart.SetRange(SHRT_MIN, SHRT_MAX);
	m_edtSkewEnd.SetRange(SHRT_MIN, SHRT_MAX);

	CString strImgPath = _T("SVG\\illustration\\Dialog\\cmd_tlin 2.svg");

	m_ImgViewer.SetImage(strImgPath);

	m_wndEccenUnit.SetUnitType(CUnitCtrl::m_LLANop_UNIT.Eccentricity);
	m_edtSpanLength.SetUnitType(CUnitCtrl::m_LLANop_UNIT.dSpan);
	m_edtSpanLength.SetUnitType(CUnitCtrl::m_LLANop_UNIT.dSpan);
	m_edtScaleFactor.SetUnitType(D_UNITSYS_NONE);
	
	// this  
	m_2PointUnit1.SetUnitType(CUnitCtrl::m_LENGTH_UNIT);
	m_2PointUnit2.SetUnitType(CUnitCtrl::m_LENGTH_UNIT);

	m_wndEccenEdit.SetAttUcsDistance();
	m_wndEccenEdit.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndEccenEdit.SetDistValueMode(TRUE);

	m_wnd2PointsEdit1.SetAttUcsPos();
	m_wnd2PointsEdit1.SetModeToUse(MOUSEEDIT_USE_SETP2_FIRST);
	m_wnd2PointsEdit2.SetAttUcsPos();
	m_wnd2PointsEdit2.SetModeToUse(MOUSEEDIT_USE_SETP2_SECOND);
	m_wnd2PointsEdit2.SetLButtonDownNotifyWindow(this);
	m_wnd2PointsEdit2.SetEnterNotifyWindow(this);
	m_wnd2PointsEdit1.SetNextLink(&m_wnd2PointsEdit2);
	m_wnd2PointsEdit2.SetNextLink(&m_wnd2PointsEdit1);

	m_wndPickingEdit.SetAttElemList();
	m_wndPickingEdit.SetLButtonDownNotifyWindow(this);
	m_wndPickingEdit.SetEnterNotifyWindow(this);
	m_wndPickingEdit.SetModeToUse(MOUSEEDIT_USE_GET_ELEM_LIST);
	m_wndPickingEdit.SetMaxElemKeyNum(1);

	m_csTitle1 = _LS(IDS_CMD_LLANOP_TITLE1);
	m_csTitle2 = _LS(IDS_CMD_LLANOP_TITLE2);

	InitCombo();

	InitGrid();

	InitItemDataVar();

	if(m_bModify)
	{
		SetInitModify();    
		SetWindowText(m_csTitle2);
	}
	else
	{
		SetWindowText(m_csTitle1);
		CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
		CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	}
	
	SelectMethodMan();

	m_edtWidth.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untWidth.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtWheelSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untWheelSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);

	ShowHideByCode();

	T_MVCD_D MvcdData;
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();
	if (MvcdData.nCodeType == D_MOVE_CODE_NEWZEALAND)
	{
		GetDlgItem(IDC_CMD_WIDTH_STC)->SetWindowText(_LS(IDS_CMD_LANE_WIDTH_NZ));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMLlanOpDlg::IsUseImpactSpan()
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
			nCode == D_MOVE_CODE_CHINA ||
			nCode == D_MOVE_CODE_NEWZEALAND) return FALSE;
	return TRUE;
}

BOOL CCMLlanOpDlg::IsUseSpanStart()
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

BOOL CCMLlanOpDlg::IsUseSpanLength()
{
	T_MVCD_D MvcdData;
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();
	int nCode = MvcdData.nCodeType;

	if (nCode != D_MOVE_CODE_CHINA) return FALSE;
	return TRUE;
}

BOOL CCMLlanOpDlg::IsUseScaleFactor()
{
	T_MVCD_D MvcdData;
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();
	int nCode = MvcdData.nCodeType;

	if (nCode != D_MOVE_CODE_CHINA) return FALSE;
	return TRUE;
}

BOOL CCMLlanOpDlg::IsUseCentrifugalForce()
{
	T_MVCD_D MvcdData;
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();
	int nCode = MvcdData.nCodeType;

	if (nCode != D_MOVE_CODE_AASHTO_LRFD && nCode != D_MOVE_CODE_CANADA) return FALSE;
	return TRUE;
}

void CCMLlanOpDlg::ShowHideByCode()
{
	T_MVCD_D MvcdData;
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();

	if (m_bListMode) return;

	BOOL bShowImpactSpan = TRUE;
	BOOL bShowSpanStart = TRUE;
	BOOL bShowSpanLength = TRUE;
	BOOL bShowScaleFactor = TRUE;
	BOOL bShowCF = TRUE;

	if(!IsUseImpactSpan())    bShowImpactSpan = FALSE;
	if(!IsUseSpanStart())     bShowSpanStart = FALSE;
	if(!IsUseSpanLength())    bShowSpanLength = FALSE;
	if(!IsUseScaleFactor())   bShowScaleFactor = FALSE;
	if(!IsUseCentrifugalForce()) bShowCF = FALSE;

	CArray<UINT, UINT> aControls;
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_FACTOR_TITLE);
	aControls.Add(IDC_CMD_FACTOR_EDIT);
	CDlgUtil::CtrlShowHide(this, aControls, bShowImpactSpan);

	aControls.RemoveAll();
	aControls.Add(IDC_CMD_SPAN_LENGTH_STC);
	aControls.Add(IDC_CMD_SPAN_LENGTH_EDT);
	aControls.Add(IDC_CMD_SPAN_LENGTH_UNT);
	CDlgUtil::CtrlShowHide(this, aControls, bShowSpanLength);

	aControls.RemoveAll();
	aControls.Add(IDC_CMD_SCALE_FACTOR_STC);
	aControls.Add(IDC_CMD_SCALE_FACTOR_EDT);
	CDlgUtil::CtrlShowHide(this, aControls, bShowScaleFactor);

	// Centrifugal Force
	CDlgUtil::CtrlShowHide(this, m_aCtrlCF, bShowCF);
	  
	m_wndGrid.HideCols(3, 3, !bShowSpanLength);
	m_wndGrid.HideCols(4, 4, !bShowImpactSpan);
	m_wndGrid.HideCols(5, 5, !bShowSpanStart);
	m_wndGrid.HideCols(6, 6, !bShowScaleFactor);
	m_wndGrid.HideCols(7, 7, !bShowCF);

	if(MvcdData.nCodeType == D_MOVE_CODE_CHINA)
	{
		m_wndGrid.SetColWidthByDPI(0, 0, 35);
		m_wndGrid.SetColWidthByDPI(1, 1, 35);
		m_wndGrid.SetColWidthByDPI(2, 2, 45);
	}
	else
	{
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
		else
		{
			m_wndGrid.SetColWidthByDPI(0, 0, 30);
			m_wndGrid.SetColWidthByDPI(1, 1, 70);
			m_wndGrid.SetColWidthByDPI(2, 2, 90);
		}  
	}

	// StraddLing Lane Type
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_STRADD_CHK);
	CDlgUtil::CtrlShowHide(this, aControls, (MvcdData.nCodeType == D_MOVE_CODE_EURO_BS || MvcdData.nCodeType == D_MOVE_CODE_BS || MvcdData.nCodeType == D_MOVE_CODE_AUSTRALIA));
}

void CCMLlanOpDlg::SetParamData(T_LLANop_D& data)
{
	m_bModify = TRUE;
	m_Data = data;
}

void CCMLlanOpDlg::SetInitModify()
{
	m_csOldLineLaneName = m_Data.LineLaneName;

	Data2Dlg();

	m_bListMode = FALSE;	
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	m_wndLaneName.SetFocus();
	ShowHideByCode();
}

void CCMLlanOpDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	m_bModify = FALSE;
	m_Data.Initialize();

	//-------------------------------------------------------------
	// MQC 4897-13 단위가 ft인 경우 Lane Width Default 값을 10ft로.
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);

	T_UNIT_INDEX CurrentUnitIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrentUnitIndex);
	if(CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT ||
		 CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN)
	{
		m_Data.dOptimWidth = M_InitValueCurUnit(10.0, LBF, FT, CUnitCtrl::m_LLANop_UNIT.dOptimWidth);
		m_Data.dLaneWidth  = M_InitValueCurUnit(10.0, LBF, FT, CUnitCtrl::m_LLANop_UNIT.dLaneWidth);
	}
	else if(CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M  ||
					CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM ||
					CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM )
	{
		m_Data.dOptimWidth = M_InitValueCurUnit(3.0, KN, M, CUnitCtrl::m_LLANop_UNIT.dOptimWidth);
		m_Data.dLaneWidth  = M_InitValueCurUnit(3.0, KN, M, CUnitCtrl::m_LLANop_UNIT.dLaneWidth);
	}

	// 호주 이동하중에서는 Lane Width 가 3.2 m 
	T_MVCD_D MvcdData;
	if (!pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();
	if(MvcdData.nCodeType == D_MOVE_CODE_AUSTRALIA)
	{
		m_Data.dLaneWidth = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, 3.2);
	}

	m_Data.dAnalLaneOffset = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, 1.0);
	//-------------------------------------------------------------

	m_Data.dWheelSpace = GetInitialWheelSpacing(0);
	Data2Dlg();

	m_bListMode = FALSE;
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	m_wndLaneName.SetFocus();
	SelectMethodMan();
	SetWindowText(m_csTitle2);
	ShowHideByCode();
	m_chkBmp.SetFoldState(FALSE);
}

void CCMLlanOpDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	T_LLANop_K Key;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_lane__nChoose));
		return;
	}
 
	Key = (T_LLANop_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetLlanop(Key, m_Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Moving_load_lane_key__d_d), Key);
		AfxMessageBox(msg);
		return;
	}

	m_csOldLineLaneName = m_Data.LineLaneName;

	m_bModify = TRUE;
	Data2Dlg();

	m_bListMode = FALSE;	
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	m_wndLaneName.SetFocus();
	SelectMethodMan();
	SetWindowText(m_csTitle2);
	ShowHideByCode();
	m_chkBmp.SetFoldState(FALSE);
}

void CCMLlanOpDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_lane__nChoose));
		return;
	}

	CString csLineLaneName = m_List.GetItemText(iItem, 0);
	if (!m_pDoc->m_pDataCtrl->DelLlanop(csLineLaneName)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMLlanOpDlg::OnCmdBtnCopy() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_lane__nChoose));
		return;
	}
	CString csLineLaneName = m_List.GetItemText(iItem, 0);

	CArray<CString,CString&> LLaneNames;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLlanList(LLaneNames);

	CString StrName;
	if(!CStrParser::MakeUniqueName(LLaneNames,csLineLaneName,StrName))
	{
		AfxMessageBox(_LS(IDS_CMD0417__Unable_to_make_lane_Name___));
		return;
	}
	
	T_LLANop_D LaneD, SourceLaneD;
	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLlanop(csLineLaneName,SourceLaneD))
	{
		CString StrTe;
		StrTe.Format(_LS(IDS_CMD0417__Not_Exist_Lane__s),csLineLaneName);
		AfxMessageBox(StrTe);
		return;
	}

	LaneD = SourceLaneD;
	LaneD.LineLaneName = StrName;
		
	CDBDoc::GetDocPoint()->m_pDataCtrl->AddLlanop(LaneD);
}

void CCMLlanOpDlg::OnCmdBtnClose() 
{
	// TODO: Add your control notification handler code here
	DestroyWindow();	
}

void CCMLlanOpDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnCmdBtnModify();
	*pResult = 0;
}

void CCMLlanOpDlg::OnCmdBtnOk() 
{
	// TODO: Add your control notification handler code here
	BOOL CheckLlan=FALSE;
	CString strTemp;
	m_wndLaneName.GetWindowText(strTemp);
	if((m_wndGrid.GetRowCount()) && ( strTemp == _T("")))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Missing_Lane_Name__),MB_OK);
		return;
	}

	if (!Dlg2Data()) return;
	if (m_bModify) CheckLlan = m_pDoc->m_pDataCtrl->ModifyLlanop(m_csOldLineLaneName, m_Data);
	else CheckLlan = m_pDoc->m_pDataCtrl->AddLlanop(m_Data);

	if (!CheckLlan) // 입력 데이타에 오류가 있을때 Window를 Close하지 않게 한다.
		return;

	m_bListMode = TRUE;
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
	m_List.SetFocus();
	SetWindowText(m_csTitle1);
}

void CCMLlanOpDlg::OnCmdBtnCancel() 
{
	// TODO: Add your control notification handler code here
	m_bListMode = TRUE;
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);
	I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
	m_List.SetFocus();
	SetWindowText(m_csTitle1);
}

void CCMLlanOpDlg::OnCmdBtnApply() 
{
	CString strTemp;
	m_wndLaneName.GetWindowText(strTemp);
	if((m_wndGrid.GetRowCount()) && ( strTemp == _T("")))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Missing_Lane_Name__),MB_OK);
		return;
	}

	if (!Dlg2Data()) return;
	if (m_bModify) m_pDoc->m_pDataCtrl->ModifyLlanop(m_csOldLineLaneName, m_Data);
	else m_pDoc->m_pDataCtrl->AddLlanop(m_Data);
}

void CCMLlanOpDlg::OnCmdChangeSelectBy() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	SelectMethodMan();
	I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);

	if (m_nSelectionMethod == 0) m_wnd2PointsEdit1.SetFocus();
	else if(m_nSelectionMethod == 1) m_wndPickingEdit.SetFocus();
	else m_wndElemSelectEdit.SetFocus();
}

LRESULT CCMLlanOpDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if (!m_bListMode)
	{
		if (m_nSelectionMethod == 0)
		{
			InsertBaseItem(-1, 0);
		}
		else if (m_nSelectionMethod == 1)
		{
			InsertBaseItem(-1, 1);
		}
		else
		{

		}
	}
	return 0L;
}

void CCMLlanOpDlg::OnCmdOpAdd() 
{
	// TODO: Add your control notification handler code here
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlSelect, nCheck);
	InsertBaseItem(-1, nCheck);
}

void CCMLlanOpDlg::OnCmdOpInsert() 
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

void CCMLlanOpDlg::OnCmdOpDelete() 
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

	SelectElem();  // added by KYM
}

void CCMLlanOpDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	BOOL bOldLock = m_wndGrid.LockUpdate(FALSE);
	if (bOldLock) m_wndGrid.Redraw();
	m_wndGrid.LockUpdate(bOldLock);
	// Do not call CCMDlgBase::OnPaint() for painting messages
}

BOOL CCMLlanOpDlg::PreTranslateMessage(MSG* pMsg) 
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

void CCMLlanOpDlg::OnCmdLoadDistType() 
{
	UpdateData(TRUE);
	
	CArray<UINT,UINT> Ctrls;
	Ctrls.Add(IDC_CMD_SK_START_EDIT);
	Ctrls.Add(IDC_CMD_SK_END_EDIT  );

	if(m_nLoadDistType == 0)
	{
		m_cmbCrossBeamElemGroup.EnableWindow(FALSE);
		CDlgUtil::CtrlEnableDisable(this,Ctrls,FALSE);
	}
	else
	{
		m_cmbCrossBeamElemGroup.EnableWindow(TRUE);
		CDlgUtil::CtrlEnableDisable(this,Ctrls,TRUE);
	}
}

void CCMLlanOpDlg::OnAnalOffsetType()
{
	UpdateData(TRUE);

	CArray<UINT,UINT> Ctrls01;
	Ctrls01.Add(IDC_CMD_EQUAL_DIVIDE_EDT);

	CArray<UINT,UINT> Ctrls02;
	Ctrls02.Add(IDC_CMD_ANAL_OFFSET_EDT);
	Ctrls02.Add(IDC_CMD_ANAL_OFFSET_UNT  );

	BOOL bAnalOffset = (m_nOffsetType == 0)? TRUE : FALSE;
	CDlgUtil::CtrlEnableDisable(this, Ctrls01,  bAnalOffset);
	CDlgUtil::CtrlEnableDisable(this, Ctrls02, !bAnalOffset);

}


BOOL CCMLlanOpDlg::GetEQDistance(CString& EQStr, double& Dx, double& Dy, double& Dz)
{
	MyParser Parser;
	
	if(Parser.ParsingPositionArg(EQStr))
	{
		Parser.GetPositionData(Dx,Dy,Dz);
	}
	else
	{
		 Dx = 0. ; Dy = 0. ; Dz = 0.;  
		 return FALSE;
	}
	return TRUE;
}


void CCMLlanOpDlg::SelectMethodMan()
{
	CArray<UINT,UINT> PointArr;
	CArray<UINT,UINT> PickingArr;
	CArray<UINT,UINT> ElemSelArr;
	
	PointArr.Add(IDC_CMD_2POINTS_EDIT1    );
	PointArr.Add(IDC_CMD_2POINTS_EDIT2    );
	PointArr.Add(IDC_CMD_2POINT_UNIT1     );
	PointArr.Add(IDC_CMD_2POINT_UNIT2     );
	PickingArr.Add(IDC_CMD_PICKING_EDIT   );
	ElemSelArr.Add(IDC_CMD_ELEMSELECT_EDIT);

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
		m_wndElemSelectEdit.GetWindowRect(&SRect);

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
BOOL CCMLlanOpDlg::ExternalSetting(T_LLANop_K LlanKey)
{
	BOOL bFlag = TRUE;
	bFlag = ::IsWindow(this->GetSafeHwnd());                // 윈도우가 생성되지 않았으면
	bFlag = m_pDoc->m_pAttrCtrl->ExistLlanop(LlanKey);        // 키값이 존재하지 않으면
	bFlag = m_pDoc->m_pAttrCtrl->GetLlanop(LlanKey, m_Data);  // Data를 가져올 수 없으면
	if(!bFlag) return bFlag;

	if(IsWindow(m_chkBmp.GetSafeHwnd()))
	{
		m_chkBmp.SetFoldState(FALSE);
	}  

	m_csOldLineLaneName = m_Data.LineLaneName;
	m_bModify = TRUE;
	Data2Dlg();

	m_bListMode = FALSE;
	ResizeDialog();
	CDlgUtil::CtrlShowHide(this, m_aCtrlList, m_bListMode);
	CDlgUtil::CtrlShowHide(this, m_aCtrlData, !m_bListMode);	
	m_wndLaneName.SetFocus();
	SelectMethodMan();
	ShowHideByCode();

	return TRUE;
}

LRESULT CCMLlanOpDlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	AlignControl();
	SelectMethodMan();
	ShowHideByCode();

	return 0L;
}

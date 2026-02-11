// BndrNLLinkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrNLLinkDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#if defined (_MEC)
#include "..\wg_base\wg_base_ControlEx.h"
#endif
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_dbLock\LockOption.h"
#include "..\wg_db\wg_db_EditData.h"
#if defined (_MEC)
#include "..\wg_db\wg_db_UndoCtrl.h"
#endif
#include "..\wg_base\I_GENStateDefine.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"
#include "..\wg_main\wg_mainres2.h"

#include "..\wg_db\MITC_CommonTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CMenuBarChildDlg
/////////////////////////////////////////////////////////////////////////////
// CBndrNLLinkDlg dialog

CBndrNLLinkDlg::CBndrNLLinkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBndrNLLinkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBndrNLLinkDlg)
	m_nOption = 0;
	m_bCopyNLLink = FALSE;
	m_nCopyAxis = 0;
	m_nRefCoord = 0;
	m_nElementCoord = 0;
	m_nGlobalCoord = 0;
	m_nSelectType = 0;
	m_bIehp = FALSE;
	//}}AFX_DATA_INIT

	m_aCtrlCopyLoad.Add(IDC_TM_RADIO_NODE_INC);
	m_aCtrlCopyLoad.Add(IDC_TM_RADIO_DISTANCE);
	m_aCtrlCopyLoad.Add(IDC_TM_AXIS_TITLE);
	m_aCtrlCopyLoad.Add(IDC_TM_AXIS_X);
	m_aCtrlCopyLoad.Add(IDC_TM_AXIS_Y);
	m_aCtrlCopyLoad.Add(IDC_TM_AXIS_Z);
	m_aCtrlCopyLoad.Add(IDC_TM_DIST_TITLE);
	m_aCtrlCopyLoad.Add(IDC_TM_DIST_EDIT);
	m_aCtrlCopyLoad.Add(IDC_TM_DIST_UNIT);
	m_aCtrlCopyLoad.Add(IDC_TM_DIST_EXAM);
	m_aCtrlCopyLoad.Add(IDC_TM_TEXT_NTIMES);
	m_aCtrlCopyLoad.Add(IDC_TM_EDIT_NTIMES);
	//m_aCtrlCopyLoad.Add(IDC_TM_SPIN_NTIMES);
	m_aCtrlCopyLoad.Add(IDC_TM_TEXT_NODEINC);
	m_aCtrlCopyLoad.Add(IDC_TM_EDIT_NODEINC);
	//m_aCtrlCopyLoad.Add(IDC_TM_SPIN_NODEINC);

	m_aCtrlCopyType.Add(IDC_TM_RADIO_NODE_INC);
	m_aCtrlCopyType.Add(IDC_TM_RADIO_DISTANCE);

	m_aCtrlCopyDist.Add(IDC_TM_AXIS_TITLE);
	m_aCtrlCopyDist.Add(IDC_TM_AXIS_X);
	m_aCtrlCopyDist.Add(IDC_TM_AXIS_Y);
	m_aCtrlCopyDist.Add(IDC_TM_AXIS_Z);
	m_aCtrlCopyDist.Add(IDC_TM_DIST_TITLE);
	m_aCtrlCopyDist.Add(IDC_TM_DIST_EDIT);
	m_aCtrlCopyDist.Add(IDC_TM_DIST_UNIT);
	m_aCtrlCopyDist.Add(IDC_TM_DIST_EXAM);

	m_aCtrlCopyNodeInc.Add(IDC_TM_TEXT_NTIMES);
	m_aCtrlCopyNodeInc.Add(IDC_TM_EDIT_NTIMES);
	//m_aCtrlCopyNodeInc.Add(IDC_TM_SPIN_NTIMES);
	m_aCtrlCopyNodeInc.Add(IDC_TM_TEXT_NODEINC);
	m_aCtrlCopyNodeInc.Add(IDC_TM_EDIT_NODEINC);
	//m_aCtrlCopyNodeInc.Add(IDC_TM_SPIN_NODEINC);

	m_aCtrlElementAngle.Add(IDC_TM_BANGLE_EDIT);
	m_aCtrlElementAngle.Add(IDC_TM_ANG_UNIT1);
	m_aCtrlElementPoint.Add(IDC_TM_REF_POINT_EDIT);
	m_aCtrlElementPoint.Add(IDC_TM_DIST_UNIT4);
	m_aCtrlElementVector.Add(IDC_TM_DIR_VECTOR_EDIT);
	m_aCtrlElementVector.Add(IDC_TM_VECTOR_UNIT);

	m_aCtrlElementCoord.Add(IDC_TM_ELEMENT_FRM);
	m_aCtrlElementCoord.Add(IDC_TM_REFCOORD_RDO1);
	m_aCtrlElementCoord.Add(IDC_TM_REFCOORD_RDO2);
	m_aCtrlElementCoord.Add(IDC_TM_REFCOORD_RDO3);
	m_aCtrlElementCoord.Append(m_aCtrlElementAngle);
	m_aCtrlElementCoord.Append(m_aCtrlElementPoint);
	m_aCtrlElementCoord.Append(m_aCtrlElementVector);

	m_aCtrlGlobalAngle.Add(IDC_TM_ANGLE_X_TITLE);
	m_aCtrlGlobalAngle.Add(IDC_TM_ANGLE_Y_TITLE);
	m_aCtrlGlobalAngle.Add(IDC_TM_ANGLE_Z_TITLE);
	m_aCtrlGlobalAngle.Add(IDC_TM_ANGLE_X_COBX);
	m_aCtrlGlobalAngle.Add(IDC_TM_ANGLE_Y_COBX);
	m_aCtrlGlobalAngle.Add(IDC_TM_ANGLE_Z_COBX);
	m_aCtrlGlobalAngle.Add(IDC_TM_ANGLE_X_UNIT);
	m_aCtrlGlobalAngle.Add(IDC_TM_ANGLE_Y_UNIT);
	m_aCtrlGlobalAngle.Add(IDC_TM_ANGLE_Z_UNIT);

	m_aCtrlGlobalPoint.Add(IDC_TM_3PT_P0_TITLE);
	m_aCtrlGlobalPoint.Add(IDC_TM_3PT_P1_TITLE);
	m_aCtrlGlobalPoint.Add(IDC_TM_3PT_P2_TITLE);
	m_aCtrlGlobalPoint.Add(IDC_TM_3PT_P0_EDIT);
	m_aCtrlGlobalPoint.Add(IDC_TM_3PT_P1_EDIT);
	m_aCtrlGlobalPoint.Add(IDC_TM_3PT_P2_EDIT);
	m_aCtrlGlobalPoint.Add(IDC_TM_3PT_P0_UNIT);
	m_aCtrlGlobalPoint.Add(IDC_TM_3PT_P1_UNIT);
	m_aCtrlGlobalPoint.Add(IDC_TM_3PT_P2_UNIT);

	m_aCtrlGlobalVector.Add(IDC_TM_VECTOR_V1_TITLE);
	m_aCtrlGlobalVector.Add(IDC_TM_VECTOR_V2_TITLE);
	m_aCtrlGlobalVector.Add(IDC_TM_VECTOR_V1_EDIT);
	m_aCtrlGlobalVector.Add(IDC_TM_VECTOR_V2_EDIT);

	m_aCtrlGlobalCoord.Add(IDC_TM_GLOBAL_FRM);
	m_aCtrlGlobalCoord.Add(IDC_TM_ANGLE);
	m_aCtrlGlobalCoord.Add(IDC_TM_3POINTS);
	m_aCtrlGlobalCoord.Add(IDC_TM_VECTOR);
	m_aCtrlGlobalCoord.Append(m_aCtrlGlobalAngle);
	m_aCtrlGlobalCoord.Append(m_aCtrlGlobalPoint);
	m_aCtrlGlobalCoord.Append(m_aCtrlGlobalVector);

	m_aCtrlSelectNodeType.Add(IDC_TM_NONLIN_2NODE_RADIO);
	m_aCtrlSelectNodeType.Add(IDC_TM_NONLIN_1NODE_RADIO);
	m_aCtrlSelectNodeType.Add(IDC_TM_NONLIN_ELEMFRAME_RADIO);
	
	m_aCtrlSelectNodeTypeCH.Add(IDC_TM_NONLIN_2NODE_RADIO);
	m_aCtrlSelectNodeTypeCH.Add(IDC_TM_NONLIN_1NODE_RADIO);
	m_aCtrlSelectNodeTypeCH.Add(IDC_TM_1NODES_EDIT);
	m_aCtrlSelectNodeTypeCH.Add(IDC_TM_NONLIN_1NODE_DIS_STATIC);
	m_aCtrlSelectNodeTypeCH.Add(IDC_TM_NONLIN_1NODE_DIS_EDIT);
	m_aCtrlSelectNodeTypeCH.Add(IDC_TM_NONLIN_1NODE_DIS_UNIT);
	m_aCtrlSelectNodeTypeCH.Add(IDC_TM_NONLIN_ELEMFRAME_RADIO);
	m_aCtrlSelectNodeTypeCH.Add(IDC_TM_EFRAME_EDIT);
	m_aCtrlSelectNodeTypeCH.Add(IDC_TM_NONLIN_EFRAME_D_STATIC);
	m_aCtrlSelectNodeTypeCH.Add(IDC_TM_EFRAME_D_EDIT);
	m_aCtrlSelectNodeTypeCH.Add(IDC_TM_NONLIN_EFRAME_D_UNIT);
	m_aCtrlSelectNodeTypeCH.Add(IDC_TM_EFRAME_D_CEHCK);
	m_aCtrlSelectNodeTypeCH.Add(IDC_TM_NONLIN_EFRAME_L_STATIC);
	m_aCtrlSelectNodeTypeCH.Add(IDC_TM_EFRAME_L_EDIT);
	m_aCtrlSelectNodeTypeCH.Add(IDC_TM_NONLIN_EFRAME_L_UNIT);
	m_aCtrlSelectNodeTypeCH.Add(IDC_TM_EFRAME_L_CEHCK);

	m_aCtrlSelectNodeTypeCH_Move.Add(IDC_TM_COPY_STATIC);
	m_aCtrlSelectNodeTypeCH_Move.Add(IDC_TM_COPY_LOAD);
	m_aCtrlSelectNodeTypeCH_Move.Add(IDC_TM_RADIO_NODE_INC);
	m_aCtrlSelectNodeTypeCH_Move.Add(IDC_TM_RADIO_DISTANCE);
	m_aCtrlSelectNodeTypeCH_Move.Add(IDC_TM_EXECUTE);
	m_aCtrlSelectNodeTypeCH_Move.Add(IDC_TM_CLOSE);
	m_aCtrlSelectNodeTypeCH_Move.Append(m_aCtrlCopyDist);
}

CBndrNLLinkDlg::~CBndrNLLinkDlg()
{
}

void CBndrNLLinkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrNLLinkDlg)
	DDX_Control(pDX, IDC_TM_DIST_UNIT, m_wndDistUnit);
	DDX_Control(pDX, IDC_TM_NL_LINK_PROP_EDIT, m_wndNllp);
	DDX_Control(pDX, IDC_TM_NL_LINK_PROP_COMBO, m_wndNllpCobx);
	DDX_Control(pDX, IDC_TM_NL_LINK_PROP_COMBO2, m_wndIehpCobx);
	DDX_Control(pDX, IDC_TM_DIST_EDIT, m_wndDistEdit);
	DDX_Control(pDX, IDC_TM_BANGLE_EDIT, m_wndBetaAngle);
	DDX_Control(pDX, IDC_TM_GROUP_COMBO, m_wndGroupCombo);
	DDX_Control(pDX, IDC_TM_2NODES_EDIT, m_wnd2Node);
	DDX_Control(pDX, IDC_TM_1NODES_EDIT, m_wnd1Node);
	DDX_Control(pDX, IDC_TM_EFRAME_EDIT, m_wndEFrame);
	DDX_Radio(pDX, IDC_TM_OPT_ADD, m_nOption);
	DDX_Check(pDX, IDC_TM_COPY_LOAD, m_bCopyNLLink);
	DDX_Radio(pDX, IDC_TM_AXIS_X, m_nCopyAxis);
	DDX_Control(pDX, IDC_TM_EDIT_NTIMES, m_editNtimes);
	DDX_Control(pDX, IDC_TM_EDIT_NODEINC, m_editNodeInc);
	//DDX_Control(pDX, IDC_TM_SPIN_NTIMES, m_spinNtimes);
	//DDX_Control(pDX, IDC_TM_SPIN_NODEINC, m_spinNodeInc);
	DDX_Radio(pDX, IDC_TM_BNDR_COORD_RDO1, m_nRefCoord);
	DDX_Radio(pDX, IDC_TM_ANGLE, m_nGlobalCoord);
	DDX_Radio(pDX, IDC_TM_REFCOORD_RDO1, m_nElementCoord);
	DDX_Radio(pDX, IDC_TM_NONLIN_2NODE_RADIO, m_nSelectType);
	DDX_Control(pDX, IDC_TM_REF_POINT_EDIT, m_wndRefPointEdit);
	DDX_Control(pDX, IDC_TM_DIR_VECTOR_EDIT, m_wndDirVectorEdit);
	DDX_Control(pDX, IDC_TM_NONLIN_1NODE_DIS_EDIT, m_wndSelect1NodeEdit);
	DDX_Control(pDX, IDC_TM_EFRAME_D_EDIT, m_wndSelectEFrameDEdit);
	DDX_Control(pDX, IDC_TM_EFRAME_L_EDIT, m_wndSelectEFrameLEdit);
	DDX_Control(pDX, IDC_TM_ANG_UNIT1, m_wndAngleUnit);
	DDX_Control(pDX, IDC_TM_DIST_UNIT4, m_wndDistUnit2);
	DDX_Control(pDX, IDC_TM_VECTOR_UNIT, m_wndVectorUnit);
	DDX_Control(pDX, IDC_TM_NONLIN_1NODE_DIS_UNIT, m_wndSelect1NodeUnit);
	DDX_Control(pDX, IDC_TM_NONLIN_EFRAME_D_UNIT, m_wndSelectEFrameUnit);
	DDX_Control(pDX, IDC_TM_NONLIN_EFRAME_L_UNIT, m_wndSelectEFrameUnit2);
	DDX_Check(pDX, IDC_TM_IEHP_CHK, m_bIehp);
	DDX_Control(pDX, IDC_TM_PICTURE, m_wndPicture);

	//}}AFX_DATA_MAP

	UINT aAngleID[] = {IDC_TM_ANGLE_X_COBX, IDC_TM_ANGLE_Y_COBX, IDC_TM_ANGLE_Z_COBX};
	UINT a3DPoint[][3] = {
		{IDC_TM_3PT_P0_EDIT, IDC_TM_3PT_P1_EDIT, IDC_TM_3PT_P2_EDIT},
		{IDC_TM_3PT_P0_UNIT, IDC_TM_3PT_P1_UNIT, IDC_TM_3PT_P2_UNIT}
	};
	UINT aVector[] = {IDC_TM_VECTOR_V1_EDIT, IDC_TM_VECTOR_V2_EDIT};
	for (int i = 0; i < 3; i++)
	{
		DDX_Control(pDX, aAngleID[i], AngleCobx[i]);
		DDX_Control(pDX, a3DPoint[0][i], Pt3Edit[i]);
		DDX_Control(pDX, a3DPoint[1][i], Pt3Unit[i]);
	}
	for(int i=0; i < 2; i++)
		DDX_Control(pDX, aVector[i], VtEdit[i]);
}

void CBndrNLLinkDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;

#ifdef _MGEN_CH
	GetDlgItem(IDC_TM_2NODE_STATIC)->GetWindowRect(rRef);
	GetDlgItem(m_aCtrlSelectNodeTypeCH[0])->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlSelectNodeTypeCH, nDistY);

	GetDlgItem(IDC_TM_NONLIN_EFRAME_L_STATIC)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_NONLIN_2NODE_RADIO)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlSelectNodeTypeCH_Move, nDistY);
#endif

	GetDlgItem(IDC_TM_AXIS_TITLE)->GetWindowRect(rRef);
	GetDlgItem(m_aCtrlCopyNodeInc[0])->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlCopyNodeInc, nDistY);

	//Global/3point Ctrl
	GetDlgItem(IDC_TM_ANGLE_X_COBX)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_3PT_P0_EDIT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlGlobalPoint, nDistY);

	//Global/Vector Ctrl
	GetDlgItem(IDC_TM_VECTOR_V1_EDIT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlGlobalVector, nDistY);

	//Element/Ref.Point Ctrl
	GetDlgItem(IDC_TM_BANGLE_EDIT)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_REF_POINT_EDIT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlElementPoint, nDistY);

	//Element/Ref.Vector Ctrl
	GetDlgItem(IDC_TM_DIR_VECTOR_EDIT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlElementVector, nDistY);

	//Element
	GetDlgItem(IDC_TM_GLOBAL_FRM)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_ELEMENT_FRM)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlElementCoord, nDistY);
	
	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_TM_COPY_STATIC);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom;
	CWnd* pParent = GetParent();
	pParent->ScreenToClient(r);
	MoveWindow(r);
}

BEGIN_MESSAGE_MAP(CBndrNLLinkDlg, CDialog)
	//{{AFX_MSG_MAP(CBndrNLLinkDlg)
	ON_BN_CLICKED(IDC_TM_OPT_ADD, OnTmOption)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_COPY_LOAD, OnTmCopyLoad)
	ON_BN_CLICKED(IDC_TM_DEFINE_GROUP_BUTTON, OnTmDefineGroupButton)
	ON_CBN_SELCHANGE(IDC_TM_NL_LINK_PROP_COMBO, OnSelchangeTmNlLinkPropCombo)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnTmOption)
	ON_BN_CLICKED(IDC_TM_NL_LINK_PROP_BUTTON, OnTmNlLinkPropButton)
	ON_BN_CLICKED(IDC_TM_NL_LINK_PROP_BUTTON2, OnTmIeHingePropButton)
	ON_BN_CLICKED(IDC_TM_RADIO_NODE_INC, OnTmCopyType)
	ON_BN_CLICKED(IDC_TM_RADIO_DISTANCE, OnTmCopyType)
	ON_BN_CLICKED(IDC_TM_BNDR_COORD_RDO1, OnCoordRdo)
	ON_BN_CLICKED(IDC_TM_BNDR_COORD_RDO2, OnCoordRdo)
	ON_BN_CLICKED(IDC_TM_NONLIN_2NODE_RADIO, OnTmSelectNodeType)
	ON_BN_CLICKED(IDC_TM_NONLIN_1NODE_RADIO, OnTmSelectNodeType)
	ON_BN_CLICKED(IDC_TM_NONLIN_ELEMFRAME_RADIO, OnTmSelectNodeType)
	ON_BN_CLICKED(IDC_TM_ANGLE, OnGlobalCoordRdo)
	ON_BN_CLICKED(IDC_TM_3POINTS, OnGlobalCoordRdo)
	ON_BN_CLICKED(IDC_TM_VECTOR, OnGlobalCoordRdo)
	ON_BN_CLICKED(IDC_TM_REFCOORD_RDO1, OnElementCoordRdo)
	ON_BN_CLICKED(IDC_TM_REFCOORD_RDO2, OnElementCoordRdo)
	ON_BN_CLICKED(IDC_TM_REFCOORD_RDO3, OnElementCoordRdo)
	ON_BN_CLICKED(IDC_TM_IEHP_CHK, OnIehpChk)
	ON_BN_CLICKED(IDC_TM_START_LINK_NUM_BTN, OnViewStartLinkNum)
	ON_BN_CLICKED(IDC_TM_EFRAME_D_CEHCK, OnEFrameDChk)
	ON_BN_CLICKED(IDC_TM_EFRAME_L_CEHCK, OnEFrameLChk)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER,OnNodeListEnter)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrNLLinkDlg message handlers

BOOL CBndrNLLinkDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//m_wndNllpCobx.
	
	MakeIehpCombo();
	AlignControl();
	CtrlMan();
	ShowHideStartLinkNumCtrl();

	m_wnd2Node.SetAttNodeList();
	m_wnd2Node.SetLButtonDownNotifyWindow(this);
	m_wnd2Node.SetEnterNotifyWindow(this);
	m_wnd2Node.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_wnd2Node.SetMaxNodeKeyNum(2);

	m_wnd1Node.Connect(SC_ID_NODE, &m_wnd1Node);
	m_wndEFrame.Connect(SC_ID_ELEM, &m_wndEFrame);
	//Element
	m_wndBetaAngle.ResetContent();
	m_wndBetaAngle.AddString(_T("0"));
	m_wndBetaAngle.AddString(_LS(IDS_WG_TREEMENU_90));
	m_wndBetaAngle.AddString(_LS(IDS_WG_TREEMENU_180));
	m_wndBetaAngle.SetCurSel(0);

	m_wndRefPointEdit.SetAttUcsPos();
	m_wndRefPointEdit.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndRefPointEdit.SetEnterNotifyWindow(this);
	m_wndRefPointEdit.SetLButtonDownNotifyWindow(this);

	m_wndDirVectorEdit.SetAttUcsDistance();
	m_wndDirVectorEdit.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndDirVectorEdit.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));

	m_wndSelect1NodeEdit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	double dist = 0.1 / CDBDoc::GetDocPoint()->m_pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	m_wndSelect1NodeEdit.SetEditUnit(dist);
	m_wndSelectEFrameDEdit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	double dEFrameD = 1.0 / CDBDoc::GetDocPoint()->m_pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	m_wndSelectEFrameDEdit.SetEditUnit(dEFrameD);
	m_wndSelectEFrameLEdit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	double dEFrameL = 0.2 / CDBDoc::GetDocPoint()->m_pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	m_wndSelectEFrameLEdit.SetEditUnit(dEFrameL);

	m_wndAngleUnit.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_wndDistUnit .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndVectorUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndSelect1NodeUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndSelectEFrameUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndSelectEFrameUnit2.SetUnitType(D_UNITSYS_BASE_LENGTH);

	//Global
	for (int i = 0; i < 3; i++)
	{
		AngleCobx[i].ResetContent();
		AngleCobx[i].AddString(_T("0"));
		AngleCobx[i].AddString(_LS(IDS_WG_TREEMENU_45));
		AngleCobx[i].AddString(_LS(IDS_WG_TREEMENU_90));
		AngleCobx[i].AddString(_LS(IDS_WG_TREEMENU_180));
		AngleCobx[i].SetCurSel(0);

		Pt3Edit[i].SetAttUcsPos();
		Pt3Edit[i].SetModeToUse(MOUSEEDIT_USE_SET_POS);
		Pt3Unit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}
	Pt3Edit[0].SetNextLink(&Pt3Edit[1]);
	Pt3Edit[1].SetNextLink(&Pt3Edit[2]);

	for(int i=0; i < 2; i++)
	{
		VtEdit[i].SetAttUcsDistance();
		VtEdit[i].SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	}

	DisplayCurNllp();
	
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlCopyType, 1);
	OnTmCopyType();
	m_editNtimes.SetRange(1, SHRT_MAX);
	m_editNtimes.SetValue(1);
	m_editNtimes.SetInteger(TRUE);
	m_editNodeInc.SetRange(1, SHRT_MAX);
	m_editNodeInc.SetValue(1);
	m_editNodeInc.SetInteger(TRUE);

	m_wndDistUnit2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	UpdateData(FALSE);
	
	OnCoordRdo();
	if (m_nRefCoord == 0)
		OnElementCoordRdo();
	else
		OnGlobalCoordRdo();
	OnTmSelectNodeType();

	OnTmCopyLoad();

	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

#if defined(_MEC)
	// Inelastic Property 관련 옵션은 Assign Hinge 기능으로 옮김
	GetDlgItem(IDC_TM_IEHP_CHK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_WG_TREEMENU_STATIC4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_WG_TREEMENU_STATIC5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_NL_LINK_PROP_COMBO2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_NL_LINK_PROP_BUTTON2)->ShowWindow(SW_HIDE);

	// calculate the distance controls should move when inelastic property options are hidden
	CRect rGeneralLinkProperty, rPicture;
	CWnd* pWnd = GetDlgItem(IDC_WG_TREEMENU_STATIC1);
	pWnd->GetWindowRect(rGeneralLinkProperty);
	pWnd = GetDlgItem(IDC_TM_PICTURE);
	pWnd->GetWindowRect(rPicture);
	const int nDistY = rPicture.top - rGeneralLinkProperty.bottom;

	// gather controls under general link property in general link data group-box
	CArray<UINT, UINT> aCtrlInGeneralLinkData;
	aCtrlInGeneralLinkData.Add(IDC_TM_PICTURE);
	aCtrlInGeneralLinkData.Add(IDC_WG_TREEMENU_STATIC6);
	aCtrlInGeneralLinkData.Add(IDC_TM_BNDR_COORD_RDO1);
	aCtrlInGeneralLinkData.Add(IDC_TM_BNDR_COORD_RDO2);
	aCtrlInGeneralLinkData.Append(m_aCtrlGlobalCoord);
	aCtrlInGeneralLinkData.Append(m_aCtrlElementCoord);
	// move
	CDlgUtil::CtrlMoveDistY(this, aCtrlInGeneralLinkData, -nDistY);

	// reduce General Link Data group-box control
	CRect rGeneralLinkData;
	GetDlgItem(IDC_TM_NLLD_STATIC)->GetWindowRect(rGeneralLinkData);
	ScreenToClient(rGeneralLinkData);
	rGeneralLinkData.bottom -= nDistY;
	GetDlgItem(IDC_TM_NLLD_STATIC)->MoveWindow(rGeneralLinkData);

	CArray<UINT, UINT> aCtrlNodeInfo;
	aCtrlNodeInfo.Add(IDC_TM_2NODE_STATIC);
	aCtrlNodeInfo.Add(IDC_TM_2NODES_EDIT);
	aCtrlNodeInfo.Add(IDC_TM_COPY_LOAD);
	aCtrlNodeInfo.Add(IDC_TM_COPY_STATIC);
	aCtrlNodeInfo.Add(IDC_TM_RADIO_NODE_INC);
	aCtrlNodeInfo.Add(IDC_TM_RADIO_DISTANCE);
	aCtrlNodeInfo.Add(IDC_TM_AXIS_TITLE);
	aCtrlNodeInfo.Add(IDC_TM_AXIS_X);
	aCtrlNodeInfo.Add(IDC_TM_AXIS_Y);
	aCtrlNodeInfo.Add(IDC_TM_AXIS_Z);
	aCtrlNodeInfo.Add(IDC_TM_DIST_TITLE);
	aCtrlNodeInfo.Add(IDC_TM_DIST_EDIT);
	aCtrlNodeInfo.Add(IDC_TM_DIST_UNIT);
	aCtrlNodeInfo.Add(IDC_TM_DIST_EXAM);
	aCtrlNodeInfo.Add(IDC_TM_NONLIN_2NODE_RADIO);
	aCtrlNodeInfo.Add(IDC_TM_NONLIN_1NODE_RADIO);
	aCtrlNodeInfo.Add(IDC_TM_1NODES_EDIT);
	aCtrlNodeInfo.Add(IDC_TM_NONLIN_1NODE_DIS_STATIC);
	aCtrlNodeInfo.Add(IDC_TM_NONLIN_1NODE_DIS_EDIT);
	aCtrlNodeInfo.Add(IDC_TM_NONLIN_1NODE_DIS_UNIT);
	aCtrlNodeInfo.Add(IDC_TM_NONLIN_ELEMFRAME_RADIO);
	aCtrlNodeInfo.Add(IDC_TM_EFRAME_EDIT);
	aCtrlNodeInfo.Add(IDC_TM_NONLIN_EFRAME_D_STATIC);
	aCtrlNodeInfo.Add(IDC_TM_EFRAME_D_EDIT);
	aCtrlNodeInfo.Add(IDC_TM_NONLIN_EFRAME_D_UNIT);
	aCtrlNodeInfo.Add(IDC_TM_EFRAME_D_CEHCK);
	aCtrlNodeInfo.Add(IDC_TM_NONLIN_EFRAME_L_STATIC);
	aCtrlNodeInfo.Add(IDC_TM_EFRAME_L_EDIT);
	aCtrlNodeInfo.Add(IDC_TM_NONLIN_EFRAME_L_UNIT);
	aCtrlNodeInfo.Add(IDC_TM_EFRAME_L_CEHCK);
	aCtrlNodeInfo.Add(IDC_TM_TEXT_NTIMES);
	aCtrlNodeInfo.Add(IDC_TM_EDIT_NTIMES);
	aCtrlNodeInfo.Add(IDC_TM_TEXT_NODEINC);
	aCtrlNodeInfo.Add(IDC_TM_EDIT_NODEINC);
	// move
	CDlgUtil::CtrlMoveDistY(this, aCtrlNodeInfo, -nDistY);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBndrNLLinkDlg::MakeIehpCombo()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	
	m_wndIehpCobx.ResetContent();
	CArray<T_IEHP_K, T_IEHP_K> rKeyList;
	pDoc->m_pAttrCtrl->GetIehpKeyList(rKeyList);
	int nSize = rKeyList.GetSize();

	T_IEHP_D IehpD;
	int nIndex;
	for (int i=0; i<nSize; i++)
	{
		pDoc->m_pAttrCtrl->GetIehp(rKeyList[i], IehpD);
		if (IehpD.nHingeType == 2)
		{
			//Spring Type
			nIndex = m_wndIehpCobx.AddString(IehpD.strName);
			m_wndIehpCobx.SetItemData(nIndex, rKeyList[i]);
		}
	}
	if (m_wndIehpCobx.GetCount() > 0)
		m_wndIehpCobx.SetCurSel(0);
}

BOOL CBndrNLLinkDlg::ChangeSelectIehp(UINT key)
{
	int nSize = m_wndIehpCobx.GetCount();
	int nIndex;
	BOOL bFind = FALSE;
	for (nIndex=0; nIndex<nSize; nIndex++)
	{
		if (key == m_wndIehpCobx.GetItemData(nIndex))
		{
			bFind = TRUE;
			m_wndIehpCobx.SetCurSel(nIndex);
			break;
		}
	}
	return bFind;
}

void CBndrNLLinkDlg::CtrlMan()
{
	this->m_nOption;

	CArray<UINT, UINT> Ctrls;
	Ctrls.Add(IDC_TM_2NODE_STATIC);
	Ctrls.Add(IDC_TM_2NODES_EDIT);
	Ctrls.Add(IDC_TM_COPY_LOAD);

	CDlgUtil::CtrlEnableDisableByRect(this,IDC_TM_NLLD_STATIC,m_nOption == 0,FALSE);
	if(m_nOption == 0)
		CDlgUtil::CtrlEnableDisableByRect(this,IDC_TM_COPY_STATIC,m_bCopyNLLink,FALSE);
	else
		CDlgUtil::CtrlEnableDisableByRect(this,IDC_TM_COPY_STATIC,FALSE,FALSE);

	CDlgUtil::CtrlEnableDisable      (this,Ctrls,m_nOption == 0);
#ifdef _MGEN_CH
	GetDlgItem(IDC_TM_2NODE_STATIC)->ShowWindow(SW_HIDE);
	CDlgUtil::CtrlEnableDisable(this, IDC_TM_NONLIN_2NODE_RADIO, m_nOption == 0);
	CDlgUtil::CtrlEnableDisable(this, IDC_TM_NONLIN_1NODE_RADIO, m_nOption == 0);
	CDlgUtil::CtrlEnableDisable(this, IDC_TM_NONLIN_1NODE_DIS_STATIC, m_nOption == 0);
	CDlgUtil::CtrlEnableDisable(this, IDC_TM_NONLIN_1NODE_DIS_EDIT, m_nOption == 0);
	CDlgUtil::CtrlEnableDisable(this, IDC_TM_NONLIN_1NODE_DIS_UNIT, m_nOption == 0);
	CDlgUtil::CtrlEnableDisable(this, IDC_TM_1NODES_EDIT, m_nOption == 0);
	OnEFrameDChk();
	OnEFrameLChk();
#endif
}

void CBndrNLLinkDlg::CopyOptCtrlMan()
{
	//m_bCopyNLLink;
	/*
	CDlgUtil::CtrlEnableDisableByRect(this,IDC_TM_COPY_STATIC,m_bCopyNLLink,FALSE);
	CArray<UINT, UINT> Ctrls;
	Ctrls.Add(IDC_TM_COPY_LOAD);
	CDlgUtil::CtrlEnableDisable(this,Ctrls,TRUE);
	*/
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlCopyLoad, m_bCopyNLLink);
}

void CBndrNLLinkDlg::OnTmOption() 
{
	UpdateData(TRUE);
	CtrlMan();
	//!@#$ MNET:XXXX 20061026 LBI - Delete->Add 변경시 Inelastic Hinge Type이 활성화되는 오류 
	//                              General Link의 종류가 spring 인 경우에만 활성화 되도록 하였음
	DisplayCurNllp();
	ChangeBitmap();
}

void CBndrNLLinkDlg::OnTmExecute()
{
	Execute();
}

void CBndrNLLinkDlg::Execute()
{
// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);
#ifdef _MGEN_CH
	int nSelectNodeType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlSelectNodeType, nSelectNodeType);
	if (nSelectNodeType == 1)
	{
		TmSelect1Node();
		return;
	}
	else if (nSelectNodeType == 2)
	{
		TmSelectElemFrame();
		return;
	}
#endif
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CArray<T_NODE_K, T_NODE_K> aSelKey;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	T_NLNK_K key;
	T_NLNK_D data;
	data.Initialize();
	CArray<int, int> aNodes;

	double ux=0.0, uy=0.0, uz=0.0;
	double wx=0.0, wy=0.0, wz=0.0;
	CArray<double, double> aDistance;

	if (m_nOption == 0)
	{
		key = pDoc->m_pAttrCtrl->GetStartNumNlnk();

		CString csNodeList;
		m_wnd2Node.GetWindowText(csNodeList);
		if (!GetNodeList(csNodeList, aNodes) || aNodes.GetSize() != 2)
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Select_2_nodes_));
			return;
		}
		data.Node1 = aNodes[0]; data.Node2 = aNodes[1];
		
		if (!CFormulaEdit::GetEditValue(&m_wndBetaAngle, data.dBetaAngle))
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_angle_));
			return;
		}

		///////////////////////////////////////////////////////////////////////
		//  Non-Linear Link Property 설정 ....
		//  data.PropKey 
		
		if (m_bCopyNLLink)
		{
			switch(m_nCopyAxis)
			{
			case 0: ux = 1.0; break;
			case 1: uy = 1.0; break;
			case 2: uz = 1.0; break;
			}
			pIGM->GetWcsVector(ux, uy, uz, wx, wy, wz);

			CString csDistance;
			m_wndDistEdit.GetWindowText(csDistance);
			if(!GetUEDistance(csDistance, aDistance) ||
				 aDistance.GetSize() <= 0)
			{
				GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_distance_value_));
				return;
			}
		}
	}
	else
	{
		pIGM->GetSelectedNodeKeyList(aSelKey);
		if (aSelKey.GetSize() == 0)
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_nodes_));
			return;
		}
	}

	T_BNGR_K BngrK;
	m_wndGroupCombo.GetSelectedBngr(BngrK);
	data.GroupKey = BngrK;

	int nCopyType;
	int nCopyNum = -1;
	int nNodeInc = -1;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCopyType, nCopyType);
	if (nCopyType == 0) // node inc
	{
		CFormulaEdit::GetEditValue(&m_editNtimes, nCopyNum);
		CFormulaEdit::GetEditValue(&m_editNodeInc, nNodeInc);
	}

	m_wndNllpCobx.GetSelectedNllp(data.PropKey);

	T_NLLP_K NllpK;
	T_NLLP_D NllpD;
	m_wndNllpCobx.GetSelectedNllp(NllpK);

	CString StrTemp;
	if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetNllp(NllpK,NllpD))
	{
		if (NllpD.nApplicationType == 0 && NllpD.nElemType == 0)
		{
			if (m_bIehp)
			{
				int nSel = m_wndIehpCobx.GetCurSel();
				if (nSel != CB_ERR)
				{
					data.IehpKey = m_wndIehpCobx.GetItemData(nSel);
				}
			}
			else
				data.IehpKey = 0;
		}
	}
	data.nRefSystem = m_nRefCoord;
	if (m_nRefCoord == 0)
	{
		//Element
		data.nRotType = m_nElementCoord;
		if (!GetBetaAngle(data.dBetaAngle, aNodes))
			return;
	}
	else
	{
		//Global
		data.nRotType = m_nGlobalCoord;
		if (m_nGlobalCoord == 0)
		{
			for (int i = 0; i < 3; i++)
				if (!CFormulaEdit::GetEditValue(&AngleCobx[i], data.dAngle[i]))
				{
					GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_angle_value_));
					return;
				}
		}
		else if (m_nGlobalCoord == 1)
		{
			double ux, uy, uz;
			CString csPos;
			for (int i = 0; i < 3; i++)
			{
				Pt3Edit[i].GetWindowText(csPos);
				if (!GetPosition(csPos, ux, uy, uz))
				{
					GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_point_coordinate_));
					return;
				}
				pIGM->GetWcsPos(ux, uy, uz, data.dPoints[i][0], data.dPoints[i][1], data.dPoints[i][2]);
			}
		}
		else if (m_nGlobalCoord == 2)
		{
			CString csVector;
			for (int i = 0; i < 2; i++)
			{
				double ux, uy, uz;
				VtEdit[i].GetWindowText(csVector);
				if (!GetPosition(csVector, ux, uy, uz))
				{
					GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_vector_));
					return;
				}
				pIGM->GetWcsVector(ux, uy, uz, data.dVector[i][0], data.dVector[i][1], data.dVector[i][2]);
			}
		}
	}
	
	if (m_nOption == 0) // Add or
	{
		CDBDoc::GetDocPoint()->m_pDataCtrl->AddNlnk(key, data, m_bCopyNLLink, nCopyType, nCopyNum, nNodeInc, wx, wy, wz, aDistance);
	}
	else if (m_nOption == 1)  // delete
	{
		CDBDoc::GetDocPoint()->m_pDataCtrl->DelNlnk1(aSelKey,BngrK);
		CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
	}
}

void CBndrNLLinkDlg::TmSelect1Node()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CArray<T_NODE_K, T_NODE_K> aSelKey;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedNodeKeyList(aSelKey);
	T_BNGR_K BngrK;
	m_wndGroupCombo.GetSelectedBngr(BngrK);
	if (m_nOption == 0)
	{
		MITC_CommonTool::func_CallBack_BndrNLLink_GetBeta funcGetBeta= [&](double& dBetaAngle, CArray<int, int>& aNodes)
		{
			return GetBetaAngle(dBetaAngle, aNodes);
		};

		T_NLLP_K NllpK;
		double ddz = m_wndSelect1NodeEdit.GetEditValue();
		m_wndNllpCobx.GetSelectedNllp(NllpK);
		MITC_CommonTool::AddNlnkBy1Node(aSelKey, BngrK, NllpK, ddz, funcGetBeta);
	}
	else
	{
		CDBDoc::GetDocPoint()->m_pDataCtrl->DelNlnk1(aSelKey, BngrK);
		CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
	}
}

void CBndrNLLinkDlg::TmSelectElemFrame()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	CArray<T_ELEM_K, T_ELEM_K> aSelAllKey;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedElemKeyList(aSelAllKey);
	T_ELEM_D ElemD;
	for (int i = 0; i < aSelAllKey.GetSize(); i++)
	{
		pDoc->m_pAttrCtrl->GetElem(aSelAllKey[i], ElemD);
		if (pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp))
			aSelKey.Add(aSelAllKey[i]);
	}
	if (aSelKey.IsEmpty())
		return;
	//
	T_BNGR_K BngrK;
	m_wndGroupCombo.GetSelectedBngr(BngrK);
	if (m_nOption == 0)
	{
		MITC_CommonTool::func_CallBack_BndrNLLink_GetBeta funcGetBeta = [&](double& dBetaAngle, CArray<int, int>& aNodes)
		{
			return GetBetaAngle(dBetaAngle, aNodes);
		};

		T_NLLP_K NllpK;
		m_wndNllpCobx.GetSelectedNllp(NllpK);
		BOOL bCenter = ((CButton*)GetDlgItem(IDC_TM_EFRAME_D_CEHCK))->GetCheck();
		BOOL bAllLen = ((CButton*)GetDlgItem(IDC_TM_EFRAME_L_CEHCK))->GetCheck();
		double ddi = m_wndSelectEFrameDEdit.GetEditValue();
		double dlen = m_wndSelectEFrameLEdit.GetEditValue();
		MITC_CommonTool::AddNlnkByElemFrame(aSelKey, BngrK, NllpK, ddi, dlen, bCenter, bAllLen, funcGetBeta);
	}
	else
	{
		//CDBDoc::GetDocPoint()->m_pDataCtrl->DelNlnk1(aSelKey, BngrK);
		//CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
	}
}

BOOL CBndrNLLinkDlg::GetBetaAngle(double& dBetaAngle, CArray<int, int>& aNodes)
{
	double RPUx, RPUy, RPUz;
	double RPWx, RPWy, RPWz;
	CString strAngle;
	
	CDBDoc * pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();

	if (m_nElementCoord == 0)
	{
		m_wndBetaAngle.GetWindowText(strAngle);
		GetFloatNumber(strAngle,dBetaAngle);
	}
	else if (m_nElementCoord == 1)
	{
		m_wndRefPointEdit.GetWindowText(strAngle);
		GetPosition(strAngle,RPWx,RPWy,RPWz);
		pIGM->GetWcsPos(RPWx,RPWy,RPWz,RPWx,RPWy,RPWz); // UCS Data를 WCS Data로 변환 한다.
	}
	else if (m_nElementCoord == 2)
	{
		m_wndDirVectorEdit.GetWindowText(strAngle);
		if(TRUE == m_wndDirVectorEdit.IsKeyPressed())
		{
			if(GetDirectionVector(strAngle,RPUx,RPUy,RPUz))
			{
				 pIGM->GetWcsVector(RPUx,RPUy,RPUz,RPWx,RPWy,RPWz);
			}
			else return FALSE;
		}
		else
		{        
			m_wndDirVectorEdit.GetCurMousePosDist(RPWx,RPWy,RPWz,RPUx,RPUy,RPUz);
		}

		if(aNodes.GetSize() != 2) return FALSE;
		T_NODE_D NodeI;
		if(!pDoc->m_pAttrCtrl->GetNode(aNodes.GetAt(0), NodeI)) return FALSE;       
		RPWx = RPWx + NodeI.x;
		RPWy = RPWy + NodeI.y;
		RPWz = RPWz + NodeI.z;
		pIGM->GetWcsPos(RPWx,RPWy,RPWz,RPWx,RPWy,RPWz); // UCS Data를 WCS Data로 변환 한다.      
	}

	if (m_nElementCoord != 0)
	{
		double FrameLocalVectorOld[3][3];
		T_NODE_D Data_I, Data_J, Data_K;
		int nKnode=1;
		pDoc->m_pAttrCtrl->GetNode(aNodes[0], Data_I);
		pDoc->m_pAttrCtrl->GetNode(aNodes[1], Data_J);
		Data_K.Initialize();
		Data_K.x=RPWx, Data_K.y=RPWy, Data_K.z=RPWz;
		if(pDoc->calcFrameLocalVector(Data_I, Data_J, Data_K, nKnode, FrameLocalVectorOld))
		{
			dBetaAngle = pDoc->calcFrameLocalAngle(Data_I, Data_J, FrameLocalVectorOld);
		}
	}
	return TRUE;;
}

void CBndrNLLinkDlg::OnTmClose() 
{
	CloseDlg();	
}

void CBndrNLLinkDlg::OnTmCopyLoad() 
{
	UpdateData(TRUE);
	CopyOptCtrlMan();
}

void CBndrNLLinkDlg::OnTmDefineGroupButton() 
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(),CCMBndrGrupDefDlg::IDD);
}

LRESULT CBndrNLLinkDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();

	if(m_wnd2Node.GetSafeHwnd() == (HWND)lParam)
	{
		CArray<int, int> aKeyNode;
		CString csNodeList;
		m_wnd2Node.GetWindowText(csNodeList);
		if (!GetNodeList(csNodeList, aKeyNode) || aKeyNode.GetSize() != 2)
			return 0L;
		OnTmExecute();
		m_wnd2Node.ClearContents();
	}
	return 0L;  
}

LRESULT CBndrNLLinkDlg::OnNodeListEnter(WPARAM wParam, LPARAM lParam)
{
	if(m_wnd2Node.GetSafeHwnd() == (HWND)lParam)
	{
		OnTmExecute();
	}
	return 0L;
}

void CBndrNLLinkDlg::OnSelchangeTmNlLinkPropCombo() 
{
	// TODO: Add your control notification handler code here
	DisplayCurNllp();
}

void CBndrNLLinkDlg::DisplayCurNllp()
{
	T_NLLP_K NllpK;
	T_NLLP_D NllpD;
	m_wndNllpCobx.GetSelectedNllp(NllpK);

	CString StrTemp;
	if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetNllp(NllpK,NllpD))
	{
		if (NllpD.nApplicationType == 0)
		{
			switch(NllpD.nElemType)
			{
			case 0:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_ELEMENT_Link_Prop1);
				break;
			case 1:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_ELEMENT_Link_Prop2);
				break;
			case 2:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_ELEMENT_Link_Prop3);
				break;
			case 3:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_ELEMENT_Link_Prop4);
				break;
			case 4:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_ELEMENT_Link_Prop5);
				break;
			}
		}
		else if (NllpD.nApplicationType == 1)
		{
			switch(NllpD.nType)
			{
			case 0:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop1);
				break;
			case 1:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop2);
				break;
			case 2:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop3);
				break;
			case 3:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop4);
				break;
			case 4:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop5);
				break;
			case 5:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop6);
				break;
			case 6:
				StrTemp = _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop7);
				break;
			}
		}
		else if (NllpD.nApplicationType == 2)
		{
			switch(NllpD.nSeisType)
			{
			case 0:
				StrTemp = _LS(IDS_CMD_SEIS_PROP_SDVI);
				break;
			case 1:
				StrTemp = _LS(IDS_CMD_SEIS_PROP_SDVE);
				break;
			case 2:
				StrTemp = _LS(IDS_CMD_SEIS_PROP_SDST);
				break;
			case 3:
				StrTemp = _LS(IDS_CMD_SEIS_PROP_SDHY);
				break;
			case 4:
				StrTemp = _LS(IDS_CMD_SEIS_PROP_SDIS);
				break;
			case 5:
				StrTemp = _LS(IDS_CMD_SEIS_PROP_ASGB);
				break;
			case 6:
				StrTemp = _LS(IDS_CMD_SEIS_PROP_DMGB);
				break;
			}
		}
		else ASSERT(0);
	}
	m_wndNllp.SetWindowText(StrTemp);
	BOOL bSpring = FALSE;
	if (NllpD.nApplicationType == 0 && NllpD.nElemType == 0)
		bSpring = TRUE;

	if (!bSpring)
		((CButton*)GetDlgItem(IDC_TM_IEHP_CHK))->SetCheck(0);
	GetDlgItem(IDC_TM_IEHP_CHK)->EnableWindow(bSpring);

	BOOL bNlDynFrame = CDBDoc::GetDocPoint()->AllowCommand(D_OPTN_ID_INELATIC);
	GetDlgItem(IDC_TM_IEHP_CHK)->EnableWindow(bNlDynFrame);

	OnIehpChk();
}

void CBndrNLLinkDlg::OnTmNlLinkPropButton() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND,	MAKEWPARAM(ID_STRUCT_BNDR_NLLINK_PROPERTY	,0));
}

void CBndrNLLinkDlg::OnTmIeHingePropButton()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,	MAKEWPARAM(ID_STRUCT_ITHA_DEFINE_HINGE	,0));
}

void CBndrNLLinkDlg::OnTmCopyType()
{
	int nCopyType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCopyType, nCopyType);
	CDlgUtil::CtrlShowHide(this, m_aCtrlCopyNodeInc, nCopyType == 0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlCopyDist, nCopyType == 1);
}

void CBndrNLLinkDlg::OnTmSelectNodeType()
{
	int nSelectNodeType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlSelectNodeType, nSelectNodeType);
	CMouseEdit::ImAssocWindowST();
	CViewCtrl* pViewCtrl = CDBDoc::GetDocPoint()->m_pViewCtrl;
	pViewCtrl->UnselectAll(NULL);
	if (nSelectNodeType == 0)
	{
		CMouseEdit::SetCurMouseEditST(&m_wnd2Node);
		m_wnd2Node.UseDefaultColor(FALSE);
		m_wndEFrame.Clear();
		
		m_wnd1Node.Disconnect();
		m_wndEFrame.Disconnect();
		I_GENModelBase::SetModelingModeST(IG_STATE_GET_NODE_LIST);
	}
	else if(nSelectNodeType == 1)
	{
		m_wnd2Node.UseDefaultColor(TRUE);
		m_wnd1Node.Clear();
		m_wndEFrame.Clear();
		m_wnd1Node.Connect(SC_ID_NODE, &m_wnd1Node);
		m_wndEFrame.Disconnect();
		I_GENModelBase::SetModelingModeST(IG_STATE_CANCEL_ALL);
		AfxGetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_VIEW_SELECT_SINGLE, 0));
	}
	else if (nSelectNodeType == 2)
	{
		m_wnd2Node.UseDefaultColor(TRUE);
		//pViewCtrl->UnSelectAllNode(NULL);
		m_wnd1Node.Disconnect();
		m_wndEFrame.Clear();
		m_wndEFrame.Connect(SC_ID_ELEM, &m_wndEFrame);
		I_GENModelBase::SetModelingModeST(IG_STATE_CANCEL_ALL);
		AfxGetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_VIEW_SELECT_SINGLE, 0));
	}
	CDlgUtil::CtrlEnableDisable(this, IDC_TM_COPY_LOAD, nSelectNodeType == 0);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlCopyLoad, nSelectNodeType == 0);
	GetDlgItem(IDC_TM_2NODES_EDIT)->EnableWindow(nSelectNodeType == 0);
	GetDlgItem(IDC_TM_1NODES_EDIT)->EnableWindow(nSelectNodeType == 1);
	GetDlgItem(IDC_TM_EFRAME_EDIT)->EnableWindow(nSelectNodeType == 2);
	//RedrawWindow();
	//Invalidate();
	//CMouseEdit::ImNotAssocWindowAllForceST();
}

void CBndrNLLinkDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
		HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CBndrNLLinkDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	POSITION pos = pViewBuff->GetStartBuffer();
	T_UDRD_BUFFER buffer_ur;
	BOOL bNLLP = FALSE;
	BOOL bIEHP = FALSE;
	while(pos)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case UR_NLLP_ADD:
		case UR_NLLP_DEL:
		case UR_NLLP_MFD:
		case UR_NLLP_MFS:
			bNLLP = TRUE;
			goto Exit;
			break;
		case UR_IEHP_ADD:
		case UR_IEHP_DEL:
		case UR_IEHP_MFD:
		case UR_IEHP_MFS:
			bIEHP = TRUE;
			goto Exit;
			break;
		default:
			break;
		}
	}
Exit:
	if(bNLLP)
		DisplayCurNllp();
	if(bIEHP)
		MakeIehpCombo();
}


BOOL CBndrNLLinkDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_NLNK_D data;
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc) ;
	pDoc->m_pAttrCtrl->GetNlnk(key, data);

	MakeIehpCombo();

	// Boundary Group
	m_wndGroupCombo.ChangeSelect(data.GroupKey);

	// Options
	m_nOption = 0;

	// General Link Property
	m_wndNllpCobx.ChangeSelect(data.PropKey);
	DisplayCurNllp();

	// inelastic hinge property
	T_NLLP_D NllpD;
	pDoc->m_pAttrCtrl->GetNllp(data.PropKey, NllpD);
	if (NllpD.nApplicationType == 0 && NllpD.nElemType == 0)
	{
		if (data.IehpKey == 0)
			m_bIehp = FALSE;
		else
		{
			m_bIehp = TRUE;
			ChangeSelectIehp(data.IehpKey);
		}
	}
	else
		m_bIehp = FALSE;
	
	GetDlgItem(IDC_TM_NL_LINK_PROP_COMBO2)->EnableWindow(m_bIehp);
	GetDlgItem(IDC_TM_NL_LINK_PROP_BUTTON2)->EnableWindow(m_bIehp);
	
	CString str;
	m_nRefCoord = data.nRefSystem;
	if (data.nRefSystem == 0)   // element
	{
		m_nElementCoord = 0;
		str.Format(_T("%g"), data.dBetaAngle);
		int ix = m_wndBetaAngle.SelectString(-1, str);
		if (ix == CB_ERR) m_wndBetaAngle.SetWindowText(str);
	}
	else    // global
	{
		m_nGlobalCoord = data.nRotType;
		switch (data.nRotType)
		{
		case 0:
			for(int i=0; i < 3; i++)
			{
				str.Format(_T("%g"), data.dAngle[i]);
				AngleCobx[i].SelectString(-1, str);
			}
			break;
		case 1:
			for(int i=0; i < 3; i++)
			{
				str.Format(_T("%g, %g, %g"), data.dPoints[i][0], data.dPoints[i][1], data.dPoints[i][2]);
				Pt3Edit[i].SetWindowText(str);
			}
			break;
		case 2:
			for(int i=0; i < 2; i++)
			{
				str.Format(_T("%g, %g, %g"), data.dVector[i][0], data.dVector[i][1], data.dVector[i][2]);
				VtEdit[i].SetWindowText(str);
			}
			break;
		default: ASSERT(0); break;
		}
	}

	UpdateData(FALSE);

	OnCoordRdo();

	return TRUE;
}

void CBndrNLLinkDlg::OnCoordRdo()
{
	UpdateData();
	BOOL bElem = FALSE;
	if (m_nRefCoord == 0)
		bElem = TRUE;
	
	CDlgUtil::CtrlShowHide(this, m_aCtrlElementCoord, bElem);
	CDlgUtil::CtrlShowHide(this, m_aCtrlGlobalCoord, !bElem);

	if (bElem)
		OnElementCoordRdo();
	else
		OnGlobalCoordRdo();
	
	ChangeBitmap();
}

void CBndrNLLinkDlg::OnGlobalCoordRdo()
{
	UpdateData();
	CDlgUtil::CtrlShowHide(this, m_aCtrlGlobalAngle, m_nGlobalCoord==0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlGlobalPoint, m_nGlobalCoord==1);
	CDlgUtil::CtrlShowHide(this, m_aCtrlGlobalVector, m_nGlobalCoord==2);
	ChangeBitmap();
}

void CBndrNLLinkDlg::OnElementCoordRdo()
{
	UpdateData();
	CDlgUtil::CtrlShowHide(this, m_aCtrlElementAngle, m_nElementCoord==0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlElementPoint, m_nElementCoord==1);
	CDlgUtil::CtrlShowHide(this, m_aCtrlElementVector, m_nElementCoord==2);
}

void CBndrNLLinkDlg::ChangeBitmap()
{
	UpdateData();
	UINT aBitmapID[] = { IDB_TM_ELNK, IDB_TM_SKEW1,  IDB_TM_NLNK2, IDB_TM_NLNK3 };

	int nBitmap = 0;
	if (m_nRefCoord == 0)
		nBitmap = 0;
	else
		nBitmap = m_nGlobalCoord + 1;
	
	CString aBitmapPath[] = {
		_T("SVG\\illustration\\Dialog\\tm_elnk 1.svg"),
		_T("SVG\\illustration\\Dialog\\tm_skew1.svg"),
		_T("SVG\\illustration\\Dialog\\tm_nlnk2.svg"),
		_T("SVG\\illustration\\Dialog\\tm_nlnk3.svg")
	}; // need change - mylee

	m_wndPicture.SetImage(aBitmapPath[nBitmap]);
}
 
void CBndrNLLinkDlg::OnIehpChk()
{
	int nCheck = ((CButton*)GetDlgItem(IDC_TM_IEHP_CHK))->GetCheck();
#ifdef _MGEN_CH
	nCheck = ((CButton*)GetDlgItem(IDC_TM_EFRAME_D_CEHCK))->GetCheck();
#endif

	GetDlgItem(IDC_TM_NL_LINK_PROP_COMBO2)->EnableWindow(nCheck == 1);
	GetDlgItem(IDC_TM_NL_LINK_PROP_BUTTON2)->EnableWindow(nCheck == 1);
}

void CBndrNLLinkDlg::OnEFrameDChk()
{
	int nCheck = ((CButton*)GetDlgItem(IDC_TM_EFRAME_D_CEHCK))->GetCheck();
	GetDlgItem(IDC_TM_EFRAME_D_EDIT)->EnableWindow(nCheck == 0);
}

void CBndrNLLinkDlg::OnEFrameLChk()
{
	int nCheck = ((CButton*)GetDlgItem(IDC_TM_EFRAME_L_CEHCK))->GetCheck();
	GetDlgItem(IDC_TM_EFRAME_L_EDIT)->EnableWindow(nCheck == 0);
	GetDlgItem(IDC_TM_EFRAME_D_CEHCK)->EnableWindow(nCheck == 0);
	nCheck |= ((CButton*)GetDlgItem(IDC_TM_EFRAME_D_CEHCK))->GetCheck();
	GetDlgItem(IDC_TM_EFRAME_D_EDIT)->EnableWindow(nCheck == 0);
}

void CBndrNLLinkDlg::OnViewStartLinkNum() 
{
}

// 레지스트리 쓰는 동안만 필요한 함수임
void CBndrNLLinkDlg::ShowHideStartLinkNumCtrl()
{
	if(((CDBDoc*)CTreeMenuBarBase::GetDocST())->ExistRegPMS5629())
		return;

	GetDlgItem(IDC_TM_START_LINK_NUM_STC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_START_LINK_NUM_EDT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_START_LINK_NUM_BTN)->ShowWindow(SW_HIDE);

	CArray<UINT, UINT> aCtrl_Under_LinkNum;
	aCtrl_Under_LinkNum.Add(IDC_TM_NLLD_STATIC         );
	aCtrl_Under_LinkNum.Add(IDC_TM_PICTURE             );
	aCtrl_Under_LinkNum.Add(IDC_TM_2NODE_STATIC        );
	aCtrl_Under_LinkNum.Add(IDC_TM_2NODES_EDIT         );
	aCtrl_Under_LinkNum.Add(IDC_TM_COPY_STATIC         );
	aCtrl_Under_LinkNum.Add(IDC_TM_COPY_LOAD           );
	aCtrl_Under_LinkNum.Add(IDC_TM_RADIO_NODE_INC      );
	aCtrl_Under_LinkNum.Add(IDC_TM_RADIO_DISTANCE      );
	aCtrl_Under_LinkNum.Add(IDC_TM_AXIS_TITLE          );
	aCtrl_Under_LinkNum.Add(IDC_TM_AXIS_X              );
	aCtrl_Under_LinkNum.Add(IDC_TM_AXIS_Y              );
	aCtrl_Under_LinkNum.Add(IDC_TM_AXIS_Z              );
	aCtrl_Under_LinkNum.Add(IDC_TM_DIST_TITLE          );
	aCtrl_Under_LinkNum.Add(IDC_TM_DIST_EDIT           );
	aCtrl_Under_LinkNum.Add(IDC_TM_DIST_UNIT           );
	aCtrl_Under_LinkNum.Add(IDC_TM_DIST_EXAM           );
	aCtrl_Under_LinkNum.Add(IDC_TM_TEXT_NTIMES         );
	aCtrl_Under_LinkNum.Add(IDC_TM_EDIT_NTIMES         );
	//aCtrl_Under_LinkNum.Add(IDC_TM_SPIN_NTIMES         );
	aCtrl_Under_LinkNum.Add(IDC_TM_TEXT_NODEINC        );
	aCtrl_Under_LinkNum.Add(IDC_TM_EDIT_NODEINC        );
	//aCtrl_Under_LinkNum.Add(IDC_TM_SPIN_NODEINC        );
	aCtrl_Under_LinkNum.Add(IDC_TM_EXECUTE             );
	aCtrl_Under_LinkNum.Add(IDC_TM_CLOSE               );
	aCtrl_Under_LinkNum.Add(IDC_WG_TREEMENU_STATIC1    );
	aCtrl_Under_LinkNum.Add(IDC_TM_NL_LINK_PROP_COMBO  );
	aCtrl_Under_LinkNum.Add(IDC_WG_TREEMENU_STATIC2    );
	aCtrl_Under_LinkNum.Add(IDC_TM_NL_LINK_PROP_EDIT   );
	aCtrl_Under_LinkNum.Add(IDC_TM_NL_LINK_PROP_BUTTON );
	aCtrl_Under_LinkNum.Add(IDC_WG_TREEMENU_STATIC3    );
	aCtrl_Under_LinkNum.Add(IDC_WG_TREEMENU_STATIC4    );
	aCtrl_Under_LinkNum.Add(IDC_TM_NL_LINK_PROP_COMBO2 );
	aCtrl_Under_LinkNum.Add(IDC_TM_NL_LINK_PROP_BUTTON2);
	aCtrl_Under_LinkNum.Add(IDC_WG_TREEMENU_STATIC5    );
	aCtrl_Under_LinkNum.Add(IDC_WG_TREEMENU_STATIC6    );
	aCtrl_Under_LinkNum.Add(IDC_TM_BNDR_COORD_RDO1     );
	aCtrl_Under_LinkNum.Add(IDC_TM_BNDR_COORD_RDO2     );
	aCtrl_Under_LinkNum.Add(IDC_TM_GLOBAL_FRM          );
	aCtrl_Under_LinkNum.Add(IDC_TM_ANGLE               );
	aCtrl_Under_LinkNum.Add(IDC_TM_3POINTS             );
	aCtrl_Under_LinkNum.Add(IDC_TM_VECTOR              );
	aCtrl_Under_LinkNum.Add(IDC_TM_ANGLE_X_TITLE       );
	aCtrl_Under_LinkNum.Add(IDC_TM_ANGLE_X_COBX        );
	aCtrl_Under_LinkNum.Add(IDC_TM_ANGLE_X_UNIT        );
	aCtrl_Under_LinkNum.Add(IDC_TM_ANGLE_Y_TITLE       );
	aCtrl_Under_LinkNum.Add(IDC_TM_ANGLE_Y_COBX        );
	aCtrl_Under_LinkNum.Add(IDC_TM_ANGLE_Y_UNIT        );
	aCtrl_Under_LinkNum.Add(IDC_TM_ANGLE_Z_TITLE       );
	aCtrl_Under_LinkNum.Add(IDC_TM_ANGLE_Z_COBX        );
	aCtrl_Under_LinkNum.Add(IDC_TM_ANGLE_Z_UNIT        );
	aCtrl_Under_LinkNum.Add(IDC_TM_3PT_P0_TITLE        );
	aCtrl_Under_LinkNum.Add(IDC_TM_3PT_P0_EDIT         );
	aCtrl_Under_LinkNum.Add(IDC_TM_3PT_P0_UNIT         );
	aCtrl_Under_LinkNum.Add(IDC_TM_3PT_P1_TITLE        );
	aCtrl_Under_LinkNum.Add(IDC_TM_3PT_P1_EDIT         );
	aCtrl_Under_LinkNum.Add(IDC_TM_3PT_P1_UNIT         );
	aCtrl_Under_LinkNum.Add(IDC_TM_3PT_P2_TITLE        );
	aCtrl_Under_LinkNum.Add(IDC_TM_3PT_P2_EDIT         );
	aCtrl_Under_LinkNum.Add(IDC_TM_3PT_P2_UNIT         );
	aCtrl_Under_LinkNum.Add(IDC_TM_VECTOR_V1_TITLE     );
	aCtrl_Under_LinkNum.Add(IDC_TM_VECTOR_V1_EDIT      );
	aCtrl_Under_LinkNum.Add(IDC_TM_VECTOR_V2_TITLE     );
	aCtrl_Under_LinkNum.Add(IDC_TM_VECTOR_V2_EDIT      );
	aCtrl_Under_LinkNum.Add(IDC_TM_ELEMENT_FRM         );
	aCtrl_Under_LinkNum.Add(IDC_TM_REFCOORD_RDO1       );
	aCtrl_Under_LinkNum.Add(IDC_TM_REFCOORD_RDO2       );
	aCtrl_Under_LinkNum.Add(IDC_TM_REFCOORD_RDO3       );
	aCtrl_Under_LinkNum.Add(IDC_TM_ANG_UNIT1           );
	aCtrl_Under_LinkNum.Add(IDC_TM_BANGLE_EDIT         );
	aCtrl_Under_LinkNum.Add(IDC_TM_REF_POINT_EDIT      );
	aCtrl_Under_LinkNum.Add(IDC_TM_DIST_UNIT4          );
	aCtrl_Under_LinkNum.Add(IDC_TM_DIR_VECTOR_EDIT     );
	aCtrl_Under_LinkNum.Add(IDC_TM_VECTOR_UNIT         );
	aCtrl_Under_LinkNum.Add(IDC_TM_IEHP_CHK            );

#ifdef _MGEN_CH
	aCtrl_Under_LinkNum.Append(m_aCtrlSelectNodeTypeCH);
#endif

	CRect rRef, rToMove;
	CWnd* pWnd = GetDlgItem(IDC_TM_START_LINK_NUM_STC);
	pWnd->GetWindowRect(rRef);
	pWnd = GetDlgItem(IDC_TM_NLLD_STATIC);
	pWnd->GetWindowRect(rToMove);
	int nDistY = rRef.top- rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aCtrl_Under_LinkNum, nDistY);
}
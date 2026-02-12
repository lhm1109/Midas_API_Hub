// ElemCreateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "..\wg_main\wg_mainres2.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_dbRes2.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_gr\GRenderView.h"
#include "ElemCreateDlg.h"

using namespace mit::frx;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DISTTHKCTRL  35
#define DISTNODE     25

#define STR_WALL_FIXED  _LS(IDS_TM_WALL_ID_FIXED)
#define STR_WALL_INCR1  _LS(IDS_TM_WALL_ID_AUTO) 
#define STR_WALL_LAST1  _LS(IDS_TM_WALL_ID_LAST1)

#define TM_TENS_CABLE_PRETENS    _LS(IDS_TM_ELEM_CREATE_Pretension)
#define TM_TENS_CABLE_HORIZ      _LS(IDS_TM_ELEM_CREATE_Horizontal)
#define TM_TENS_CABLE_LU      _LS(IDS_TM_ELEM_CREATE_Lu)

#define D_LEVEL_USER_DEFINED    -1
/////////////////////////////////////////////////////////////////////////////
// CElemCreateDlg dialog
#ifdef _MVIL
int CElemCreateDlg::m_nCurETypeIndex = 1;
#elif _TOWER
int CElemCreateDlg::m_nCurETypeIndex = 0;
#else
int CElemCreateDlg::m_nCurETypeIndex = 3;
#endif

CElemCreateDlg::CElemCreateDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CElemCreateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElemCreateDlg)
	m_nBetaAngleOrRefPoint = 0;
	m_strBetaAngle = _T("0");
	m_nNodeNumIndex = 0;
	m_bIntersectNode = TRUE;
	m_bIntersectElem = TRUE;
	m_nCheckOrtho = FALSE;
	m_nPreParseType = 0;
	//}}AFX_DATA_INIT
	//m_nCurETypeIndex = 1;
	m_bIsParamError = FALSE;
	m_bPropInit = FALSE;
	m_bVerticalType = FALSE;
	m_nRefAngle = 0;

	SetCtrlGroups();
}

CElemCreateDlg::~CElemCreateDlg()
{
}

void CElemCreateDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemCreateDlg)
	DDX_Control(pDX, IDC_TM_TT_LENGTH_UNIT, m_wndTTLengthUnit);
	DDX_Control(pDX, IDC_TM_TT_LENGTH_EDIT, m_wndTTLengthEdit);
	DDX_Control(pDX, IDC_TM_TT_LNL_TITLE, m_cobxTensCable);
	DDX_Control(pDX, IDC_TM_W_ID_EDIT, m_editWall);
	DDX_Control(pDX, IDC_TM_W_ID_COMBO, m_cboWall);
	DDX_Control(pDX, IDC_TM_ORTHO_CHECK, m_chkOrtho);
	DDX_Control(pDX, IDC_TM_NODES_USEREDIT, m_wndNodeUserEdit);
	DDX_Control(pDX, IDC_TM_NODES_COBX, m_cobxNodeEdit);
	DDX_Control(pDX, IDC_TM_INNER_INTSECT_CHECK, m_chkCreateInnerPoint);
	DDX_Control(pDX, IDC_TM_CT_DIST_EDIT, m_wndCTDistEdit);
	DDX_Control(pDX, IDC_TM_TT_FORCE_EDIT, m_wndTTForceEdit);
	DDX_Control(pDX, IDC_TM_TT_DIST_EDIT, m_wndTTDistEdit);
	DDX_Control(pDX, IDC_TM_TT_ALCOMP_EDIT, m_wndTTAlCompEdit);
	DDX_Control(pDX, IDC_TM_COMP_LMT_EDT, m_edtCompLimit); // MNET:2774 20070507 mylee
	DDX_Control(pDX, IDC_TM_TENS_LMT_EDT, m_edtTensLimit);
	DDX_Control(pDX, IDC_TM_COMP_LMT_UNT, m_untCompLimit);
	DDX_Control(pDX, IDC_TM_TENS_LMT_UNT, m_untTensLimit); // END
	DDX_Control(pDX, IDC_TM_CT_ALTENS_EDIT, m_wndCTAlTensEdit);
	DDX_Control(pDX, IDC_TM_CT_DIST_UNIT, m_wndCTDistUnit);
	DDX_Control(pDX, IDC_TM_TT_FORCE_UNIT, m_wndTTForceUnit);
	DDX_Control(pDX, IDC_TM_TT_DIST_UNIT, m_wndTTDistUnit);
	DDX_Control(pDX, IDC_TM_CT_ALTENS_UNIT, m_wndCTAlTensUnit);
	DDX_Control(pDX, IDC_TM_TT_ALCOMP_UNIT, m_wndTTAlCompUnit);
	DDX_Control(pDX, IDC_TM_ORI_STATIC1, m_wndOriTitle);
	DDX_Control(pDX, IDC_TM_NODES_STATIC, m_wndNodesTitle);
	DDX_Control(pDX, IDC_TM_THICK_STATIC1, m_wndThickTitle);
	DDX_Control(pDX, IDC_TM_SEC_STATIC1, m_wndSecTitle);
	DDX_Control(pDX, IDC_TM_MAT_STATIC1, m_wndMatTitle);
	DDX_Control(pDX, IDC_TM_REF_POINT_EDIT, m_wndRefPointEdit);
	DDX_Control(pDX, IDC_TM_DIR_VECTOR_EDIT, m_wndDirVectorEdit);
	DDX_Control(pDX, IDC_TM_BANGLE_EDIT, m_wndBAngleEdit);
	DDX_Control(pDX, IDC_TM_ANG_UNIT, m_wndAngleUnit);
	DDX_Control(pDX, IDC_TM_DIST_UNIT, m_wndDistUnit);
	DDX_Control(pDX, IDC_TM_VECTOR_UNIT, m_wndVectorUnit);
	DDX_Control(pDX, IDC_TM_NODES_EDIT, m_wndNodeListEdit);
	DDX_Control(pDX, IDC_TM_ELEM_TYPE_COMBO, m_ctrlCobxEType);
	DDX_Control(pDX, IDC_TM_THICK_NAME_COMBO, m_ctrlCobxThik);
	DDX_Control(pDX, IDC_TM_SEC_NAME_COMBO, m_ctrlCobxSect);
	DDX_Control(pDX, IDC_TM_MAT_NAME_COMBO, m_ctrlCobxMatl);
	DDX_Control(pDX, IDC_TM_THICK_NO_EDIT, m_ctrlEditThik);
	DDX_Control(pDX, IDC_TM_SEC_NO_EDIT, m_ctrlEditSect);
	DDX_Control(pDX, IDC_TM_MAT_NO_EDIT, m_ctrlEditMatl);
	DDX_Radio(pDX, IDC_TM_BETA_ANGLE_RADIO, m_nBetaAngleOrRefPoint);
	DDX_Text(pDX, IDC_TM_BANGLE_EDIT, m_strBetaAngle);
	DDX_Radio(pDX, IDC_TM_3NODE_RADIO, m_nNodeNumIndex);
	DDX_Check(pDX, IDC_TM_INTSECT_NODE_CHECK, m_bIntersectNode);
	DDX_Check(pDX, IDC_TM_INTSECT_ELEM_CHECK, m_bIntersectElem);
	DDX_Check(pDX, IDC_TM_ORTHO_CHECK, m_nCheckOrtho);
	DDX_CBIndex(pDX, IDC_TM_NODES_COBX, m_nPreParseType);
	DDX_Control(pDX, IDC_TM_PICTURE, m_wndPicture);

#if defined(_MGEN)
	DDX_Check(pDX, IDC_TM_VERT_OPT_CHECK, m_bVerticalType);
	DDX_Control(pDX, IDC_TM_HEIGHT_CMB, m_cbxHeight);
	DDX_Control(pDX, IDC_TM_HEIGHT_EDT, m_edtHeight);
	DDX_Control(pDX, IDC_TM_HEIGHT_UNT, m_untHeigth);
#endif

	DDX_Radio(pDX, IDC_TM_GCS_RDO, m_nRefAngle);
	DDX_Control(pDX, IDC_TM_REF_ANGLE_EDT, m_edtRefAngle); // Combo 인데 수정 가능
	DDX_Control(pDX, IDC_TM_REF_ANGLE_UNT, m_untRefAngle);
	DDX_Control(pDX, IDC_TM_REF_AXIS_CMB, m_cmbRefAxis);

	//}}AFX_DATA_MAP
}

void CElemCreateDlg::AlignControl_MEC()
{
#ifndef _MEC
	return;
#endif

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	int nElemType;
	int nIndex = m_ctrlCobxEType.GetCurSel();
	nElemType = (int)m_ctrlCobxEType.GetItemData(nIndex);

	BOOL bPlstrn = FALSE;
	BOOL bAxisym = FALSE;
	BOOL bSolid = FALSE;

	if (pDoc->m_pAttrCtrl->IsPlstrn(nElemType)) bPlstrn = TRUE;
	else if (pDoc->m_pAttrCtrl->IsAxisym(nElemType)) bAxisym = TRUE;
	else if (pDoc->m_pAttrCtrl->IsSolid(nElemType)) bSolid = TRUE;

	CRect rRef;
	CRect rToMove;
	int nDistY;

	// Orientation
	GetDlgItem(IDC_TM_ORI_STATIC1)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_GCS_RDO)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top + globalUtils.ScaleByDPI(20);
	CtrlMoveDistY(m_aOrientation, nDistY);

	// Ref Axis
	GetDlgItem(IDC_TM_REF_ANGLE_EDT)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_REF_AXIS_CMB)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CtrlMoveDistY(m_aRefAxis, nDistY);

	// Orientation Group Box resize
	CRect rectResize;
	GetDlgItem(IDC_TM_ORI_STATIC1)->GetWindowRect(rectResize);

	if (bPlstrn || bAxisym)
	{
		GetDlgItem(IDC_TM_REF_ANGLE_EDT)->GetWindowRect(rRef);
	}
	else if (bSolid)
	{
		GetDlgItem(IDC_TM_REF_AXIS_CMB)->GetWindowRect(rRef);
	}
	else
	{
		GetDlgItem(IDC_TM_BANGLE_EDIT)->GetWindowRect(rRef);
	}

	rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	ScreenToClient(rectResize);
	GetDlgItem(IDC_TM_ORI_STATIC1)->MoveWindow(rectResize);

	// Nodal Connectivity
	GetDlgItem(IDC_TM_ORI_STATIC1)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_NODES_STATIC)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CtrlMoveDistY(m_aNodalConnec, nDistY);
}

void CElemCreateDlg::AlignControls()
{
	CRect rRef, rRef2;
	CRect rToMove;
	int nDistY;

	// Reference ctrl 좌표 구하기
	GetDlgItem(IDC_TM_3NODE_RADIO)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_6NODE_RADIO)->GetWindowRect(rRef2);

	// TENS-TRUSS ctrl 좌표 구하기
	GetDlgItem(m_TensTrussCtrls[0])->GetWindowRect(rToMove);
	// Distance 구하기
	nDistY = rRef.top - rToMove.top;
	// 이동하기 
	CtrlMoveDistY(m_TensTrussCtrls, nDistY);
	CtrlMoveDistY(m_TensTruss1Ctrls, nDistY);



	// COMP-TRUSS ctrl
	GetDlgItem(m_CompTrussCtrls[0])->GetWindowRect(rToMove);
	// Distance 구하기
	nDistY = rRef.top - rToMove.top;
	// 이동하기 
	CtrlMoveDistY(m_CompTrussCtrls, nDistY);
	CtrlMoveDistY(m_CompTruss1Ctrls, nDistY);

	// Wall ctrl
	GetDlgItem(m_WallCtrls[0])->GetWindowRect(rToMove);
	// Distance 구하기
	nDistY = rRef.top - rToMove.top;
	// 이동하기 
	CtrlMoveDistY(m_WallCtrls, nDistY);
	CtrlMoveDistY(m_Wall1Ctrls, nDistY);

	// Plate Type 관련 Control 을 같은 높이에
	GetDlgItem(m_arPlateTypeCtrl[0])->GetWindowRect(rToMove);
	nDistY = rRef2.top - rToMove.top;
	CtrlMoveDistY(m_arPlateTypeCtrl, nDistY/*+DISTTHKCTRL*/);

	// TENS-TRUSS의 Ln/L 부분을 Distance 위치로 이동
	GetDlgItem(m_TensTruss1Ctrls[1])->GetWindowRect(rRef);
	GetDlgItem(m_TensTruss2Ctrls[0])->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CtrlMoveDistY(m_TensTruss2Ctrls, nDistY);

	// TENS-TRUSS의 Force 부분을 Distance 위치로 이동
	GetDlgItem(m_TensTruss1Ctrls[1])->GetWindowRect(rRef);
	GetDlgItem(m_TensTruss3Ctrls[0])->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CtrlMoveDistY(m_TensTruss3Ctrls, nDistY);


	// TENS-TRUSS의 AlComp 부분을 Distance 위치로 이동
	GetDlgItem(m_TensTruss1Ctrls[0])->GetWindowRect(rRef);
	GetDlgItem(m_TensTruss0Ctrls[0])->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CtrlMoveDistY(m_TensTruss0Ctrls, nDistY);

	// COMP-TRUSS의 AlTens 부분을 Distance 위치로 이동
	GetDlgItem(m_CompTruss1Ctrls[0])->GetWindowRect(rRef);
	GetDlgItem(m_CompTruss0Ctrls[0])->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CtrlMoveDistY(m_CompTruss0Ctrls, nDistY);

	// Thickness ctrl
	GetDlgItem(m_SectCtrls[0])->GetWindowRect(rRef);
	GetDlgItem(m_ThickCtrls[0])->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CtrlMoveDistY(m_ThickCtrls, nDistY);

	// node list chk
	GetDlgItem(m_BAngleCtrls[0])->GetWindowRect(rRef);
	GetDlgItem(m_RefPointCtrls[0])->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CtrlMoveDistY(m_RefPointCtrls, nDistY);

	// Reference Vector
	GetDlgItem(m_BAngleCtrls[0])->GetWindowRect(rRef);
	GetDlgItem(m_DirVectorCtrls[0])->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CtrlMoveDistY(m_DirVectorCtrls, nDistY);

	AlignControl_MEC();

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_TM_NODES_STATIC);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + 1;
	CWnd* pParent = GetParent();
	pParent->ScreenToClient(r);
	MoveWindow(r);
}

void CElemCreateDlg::ChangeBitmap()
{
#ifdef _MVIL
	return; // Only Truss/Beam
#endif

	// Bitmap을 Oneway, Twoway로 바꾼다.
	CString aBitmapPath[] = {
		_T("SVG\\illustration\\Dialog\\tm_elem1.svg"),
		_T("SVG\\illustration\\Dialog\\tm_elem2.svg"),
		_T("SVG\\illustration\\Dialog\\tm_elem3.svg"),
		_T("SVG\\illustration\\Dialog\\tm_elem4.svg"),
		_T("SVG\\illustration\\Dialog\\tm_elem5.svg"),
		_T("SVG\\illustration\\Dialog\\tm_elem6.svg"),
		_T("SVG\\illustration\\Dialog\\tm_elem7.svg"),
		_T("SVG\\illustration\\Dialog\\tm_elem8.svg"),
		_T("SVG\\illustration\\Dialog\\tm_elem9.svg"),
		_T("SVG\\illustration\\Dialog\\tm_elem10.svg"),
		_T("SVG\\illustration\\Dialog\\tm_elem11.svg"),
		_T("SVG\\illustration\\Dialog\\tm_elem12.svg")
	};

	int nBitmap;
	int nElemType = GetCurElemTypeID();
	int nNodeNum;   // Plate/Plane strane/Solid
	int nSubType;   // Tens-Truss/Comp-Truss
	CDlgUtil::CtrlRadioGetCheck(this, m_NormalElemCtrls, nNodeNum);

	switch (nElemType)
	{
	case TRUSS_EL: case BEAM_EL:
		nBitmap = 1;
		break;
	case PLSTRS_EL: case PLATE_EL:
		if (nNodeNum == 0) nBitmap = 5;       // 3 nodes
		else if (nNodeNum == 1) nBitmap = 6;  // 4 nodes
		else ASSERT(0);
		break;
	case WALL_EL:
		nBitmap = 12;
		break;
	case PLSTRN_EL: case AXISYM_EL:
		if (nNodeNum == 0) nBitmap = 7;       // 3 nodes
		else if (nNodeNum == 1) nBitmap = 8;  // 4 nodes
		else ASSERT(0);
		break;
	case SOLID_EL:
		if (nNodeNum == 1) nBitmap = 9;       // 4 nodes
		else if (nNodeNum == 2) nBitmap = 10; // 6 nodes
		else if (nNodeNum == 3) nBitmap = 11; // 8 nodes;
		else ASSERT(0);
		break;
	case TRTENS_EL:
		CDlgUtil::CtrlRadioGetCheck(this, m_TensTrussCtrls, nSubType);
		if (nSubType == 0) nBitmap = 1;
		else if (nSubType == 1) nBitmap = 3;
		else if (nSubType == 2) nBitmap = 2;
		else ASSERT(0);
		break;
	case TRCOMP_EL:
		CDlgUtil::CtrlRadioGetCheck(this, m_CompTrussCtrls, nSubType);
		if (nSubType == 0) nBitmap = 1;
		else if (nSubType == 1) nBitmap = 4;
		else ASSERT(0);
		break;
	default:
		nBitmap = 1;
		break;
	}

	m_wndPicture.SetImage(aBitmapPath[nBitmap - 1]);
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CElemCreateDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CElemCreateDlg)
	ON_BN_CLICKED(IDC_TM_MAT_BUTTON, OnTmMatButton)
	ON_BN_CLICKED(IDC_TM_SEC_BUTTON, OnTmSecButton)
	ON_BN_CLICKED(IDC_TM_THICK_BUTTON, OnTmThickButton)
	ON_CBN_SELCHANGE(IDC_TM_ELEM_TYPE_COMBO, OnSelchangeTmElemTypeCombo)
	ON_BN_CLICKED(IDC_TM_BETA_ANGLE_RADIO, OnTmBetaAngleOrRefPointRadio)
	ON_EN_SETFOCUS(IDC_TM_NODES_EDIT, OnSetfocusTmNodesEdit)
	ON_BN_CLICKED(IDC_TM_INTSECT_ELEM_CHECK, OnTmIntsectElemCheck)
	ON_BN_CLICKED(IDC_TM_INTSECT_NODE_CHECK, OnTmIntsectNodeCheck)
	ON_BN_CLICKED(IDC_TM_3NODE_RADIO, OnTmClickNodeRadio)
	ON_BN_CLICKED(IDC_TM_TT_RADIO_TRUSS, OnClickTensTrussRadio)
	ON_BN_CLICKED(IDC_TM_CT_RADIO_TRUSS, OnClickCompTrussRadio)
	ON_BN_CLICKED(IDC_TM_INTST_TOL, OnTmIntstTol)
	ON_BN_CLICKED(IDC_TM_ORTHO_CHECK, OnTmOrthoCheck)
	ON_CBN_SELCHANGE(IDC_TM_NODES_COBX, OnSelchangeTmNodesCobx)
	ON_BN_CLICKED(IDC_TM_NODES_ENTER_BTN, OnTmNodesEnterBtn)
	ON_CBN_SELCHANGE(IDC_TM_W_ID_COMBO, OnSelchangeTmWIdCombo)
	ON_BN_CLICKED(IDC_TM_REF_POINT_RADIO, OnTmBetaAngleOrRefPointRadio)
	ON_BN_CLICKED(IDC_TM_DIR_VECTOR_RADIO, OnTmBetaAngleOrRefPointRadio)
	ON_BN_CLICKED(IDC_TM_4NODE_RADIO, OnTmClickNodeRadio)
	ON_BN_CLICKED(IDC_TM_6NODE_RADIO, OnTmClickNodeRadio)
	ON_BN_CLICKED(IDC_TM_8NODE_RADIO, OnTmClickNodeRadio)
	ON_BN_CLICKED(IDC_TM_TT_RADIO_HOOK, OnClickTensTrussRadio)
	ON_BN_CLICKED(IDC_TM_TT_RADIO_CABLE, OnClickTensTrussRadio)
	ON_BN_CLICKED(IDC_TM_CT_RADIO_GAP, OnClickCompTrussRadio)
	ON_CBN_SELCHANGE(IDC_TM_TT_LNL_TITLE, OnSelchangeTensCableCombo)
	ON_BN_CLICKED(IDC_TM_TENS_LMT_CHK, OnTmClickTensLimit) // MNET:XXXX 20070531 mylee
	ON_BN_CLICKED(IDC_TM_COMP_LMT_CHK, OnTmClickCompLimit)
	ON_BN_CLICKED(IDC_TM_W_RADIO_PLATE_TYPE, OnTmClickWallType)
	ON_BN_CLICKED(IDC_TM_W_RADIO_CRB_TYPE, OnTmClickWallType)
	ON_BN_CLICKED(IDC_TM_GCS_RDO, OnTmOrientationRdo)
	ON_BN_CLICKED(IDC_TM_REF_ANGLE_RDO, OnTmOrientationRdo)
	ON_CBN_SELCHANGE(IDC_TM_BETA_DIR_CMB, OnTmRefDirCmb)
	ON_BN_CLICKED(IDC_TM_REF_BUTTON, OnTmRefButton)

#if defined(_MGEN)
	ON_BN_CLICKED(IDC_TM_VERT_OPT_CHECK, OnTmClickVertiType)
	ON_CBN_SELCHANGE(IDC_TM_HEIGHT_CMB, OnSelHeightCmb)
#endif

	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER, OnNodeListEnter)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITESCAPE, OnNodeListEscape)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemCreateDlg message handlers

void CElemCreateDlg::Execute()
{
	_CreateElement();
	/*
	CString str;
	m_wndNodeListEdit.GetWindowText(str);

	CArray <int,int> NList;

	if(GetNodeList(str,NList))
	{
		int NodeCount = NList.GetSize();
		if(NodeCount == 0 )
			//GDebugOut("Empty Node List !!");
		for(int i = 0 ; i < NodeCount ; i++)
		{
			//GDebugOut("No %5d :: Key %5d ",i,NList[i]);
		}
	}
	else
	{
		//GDebugOut("Node List Arg Error");
	}
	*/
}

void CElemCreateDlg::SetCtrlGroups()
{
	m_SectCtrls.RemoveAll();
	m_ThickCtrls.RemoveAll();
	m_KNodeCtrls.RemoveAll();
	m_BAngleCtrls.RemoveAll();
	m_RefPointCtrls.RemoveAll();
	m_PlateNodesCtrls.RemoveAll();
	m_SolidNodesCtrls.RemoveAll();
	m_IntersectNodeCtrls.RemoveAll();
	m_IntersectElemCtrls.RemoveAll();

	m_NormalElemCtrls.RemoveAll();
	m_TensTrussCtrls.RemoveAll();
	m_TensTruss0Ctrls.RemoveAll();
	m_TensTruss1Ctrls.RemoveAll();
	m_TensTruss2Ctrls.RemoveAll();
	m_TensTruss3Ctrls.RemoveAll();

	m_CompTrussCtrls.RemoveAll();
	m_CompTruss0Ctrls.RemoveAll();
	m_CompTruss1Ctrls.RemoveAll();
	m_WallCtrls.RemoveAll();
	m_Wall1Ctrls.RemoveAll();
	m_WallCRB.RemoveAll();
	m_WallNodeConnectRdo.RemoveAll();
	m_WallTypeRdo.RemoveAll();
	m_HeightCtrls.RemoveAll();

	m_SectCtrls.Add(IDC_TM_SEC_STATIC1);
	m_SectCtrls.Add(IDC_TM_SEC_NO_EDIT);
	m_SectCtrls.Add(IDC_TM_SEC_STATIC2);
	m_SectCtrls.Add(IDC_TM_SEC_STATIC3);
	m_SectCtrls.Add(IDC_TM_SEC_NAME_COMBO);
	m_SectCtrls.Add(IDC_TM_SEC_BUTTON);

	m_ThickCtrls.Add(IDC_TM_THICK_STATIC1);
	m_ThickCtrls.Add(IDC_TM_THICK_NO_EDIT);
	m_ThickCtrls.Add(IDC_TM_THICK_STATIC2);
	m_ThickCtrls.Add(IDC_TM_THICK_STATIC3);
	m_ThickCtrls.Add(IDC_TM_THICK_NAME_COMBO);
	m_ThickCtrls.Add(IDC_TM_THICK_BUTTON);

	m_KNodeCtrls.Add(IDC_TM_ORI_STATIC1);
	m_KNodeCtrls.Add(IDC_TM_BETA_ANGLE_RADIO);
	m_KNodeCtrls.Add(IDC_TM_BANGLE_EDIT);
	m_KNodeCtrls.Add(IDC_TM_REF_POINT_RADIO); //사내배포에서 제외 //다시살림 bugboy 2000.9.16
	m_KNodeCtrls.Add(IDC_TM_REF_POINT_EDIT);
	m_KNodeCtrls.Add(IDC_TM_DIST_UNIT);
	m_KNodeCtrls.Add(IDC_TM_ANG_UNIT);
	m_KNodeCtrls.Add(IDC_TM_DIR_VECTOR_RADIO);
	m_KNodeCtrls.Add(IDC_TM_DIR_VECTOR_EDIT);
	m_KNodeCtrls.Add(IDC_TM_VECTOR_UNIT);

	m_BAngleCtrls.Add(IDC_TM_BANGLE_EDIT);
	m_BAngleCtrls.Add(IDC_TM_ANG_UNIT);

	m_RefPointCtrls.Add(IDC_TM_REF_POINT_EDIT);
	m_RefPointCtrls.Add(IDC_TM_DIST_UNIT);

	m_DirVectorCtrls.Add(IDC_TM_DIR_VECTOR_EDIT);
	m_DirVectorCtrls.Add(IDC_TM_VECTOR_UNIT);

	m_PlateNodesCtrls.Add(IDC_TM_3NODE_RADIO);
	m_PlateNodesCtrls.Add(IDC_TM_4NODE_RADIO);

	m_SolidNodesCtrls.Add(IDC_TM_4NODE_RADIO);
	m_SolidNodesCtrls.Add(IDC_TM_6NODE_RADIO);
	m_SolidNodesCtrls.Add(IDC_TM_8NODE_RADIO);

	m_IntersectNodeCtrls.Add(IDC_TM_INTSECT_NODE_CHECK);
	m_IntersectElemCtrls.Add(IDC_TM_INTSECT_ELEM_CHECK);

	m_NormalElemCtrls.Add(IDC_TM_3NODE_RADIO);
	m_NormalElemCtrls.Add(IDC_TM_4NODE_RADIO);
	m_NormalElemCtrls.Add(IDC_TM_6NODE_RADIO);
	m_NormalElemCtrls.Add(IDC_TM_8NODE_RADIO);

	m_TensTrussCtrls.Add(IDC_TM_TT_RADIO_TRUSS);
	m_TensTrussCtrls.Add(IDC_TM_TT_RADIO_HOOK);
	m_TensTrussCtrls.Add(IDC_TM_TT_RADIO_CABLE);

	m_TensTruss0Ctrls.Add(IDC_TM_TT_ALCOMP_TITLE);
	m_TensTruss0Ctrls.Add(IDC_TM_TT_ALCOMP_EDIT);
	m_TensTruss0Ctrls.Add(IDC_TM_TT_ALCOMP_UNIT);
	m_TensTruss0Ctrls.Add(IDC_TM_TENS_LMT_CHK); // MNET:2774 20070507 mylee
	m_TensTruss0Ctrls.Add(IDC_TM_TENS_LMT_EDT);
	m_TensTruss0Ctrls.Add(IDC_TM_TENS_LMT_UNT);

	m_TensTruss1Ctrls.Add(IDC_TM_TT_DIST_TITLE);
	m_TensTruss1Ctrls.Add(IDC_TM_TT_DIST_EDIT);
	m_TensTruss1Ctrls.Add(IDC_TM_TT_DIST_UNIT);

	m_TensTruss2Ctrls.Add(IDC_TM_TT_LNL_TITLE);
	m_TensTruss2Ctrls.Add(IDC_TM_TT_LENGTH_EDIT);
	m_TensTruss2Ctrls.Add(IDC_TM_TT_LENGTH_UNIT);
	m_TensTruss3Ctrls.Add(IDC_TM_TT_FORCE_EDIT);
	m_TensTruss3Ctrls.Add(IDC_TM_TT_FORCE_UNIT);

	/*
	m_TensTruss2Ctrls.Add(IDC_TM_TT_FORCE_TITLE);
	m_TensTruss2Ctrls.Add(IDC_TM_TT_FORCE_EDIT);
	m_TensTruss2Ctrls.Add(IDC_TM_TT_FORCE_UNIT);
	*/

	m_CompTrussCtrls.Add(IDC_TM_CT_RADIO_TRUSS);
	m_CompTrussCtrls.Add(IDC_TM_CT_RADIO_GAP);

	m_CompTruss0Ctrls.Add(IDC_TM_CT_ALTENS_TITLE);
	m_CompTruss0Ctrls.Add(IDC_TM_CT_ALTENS_EDIT);
	m_CompTruss0Ctrls.Add(IDC_TM_CT_ALTENS_UNIT);
	m_CompTruss0Ctrls.Add(IDC_TM_COMP_LMT_CHK); // MNET:2774 20070507 mylee
	m_CompTruss0Ctrls.Add(IDC_TM_COMP_LMT_EDT);
	m_CompTruss0Ctrls.Add(IDC_TM_COMP_LMT_UNT);

	m_CompTruss1Ctrls.Add(IDC_TM_CT_DIST_TITLE);
	m_CompTruss1Ctrls.Add(IDC_TM_CT_DIST_EDIT);
	m_CompTruss1Ctrls.Add(IDC_TM_CT_DIST_UNIT);

	m_WallCtrls.Add(IDC_TM_W_RADIO_MEMBR);
	m_WallCtrls.Add(IDC_TM_W_RADIO_PLATE);

	m_Wall1Ctrls.Add(IDC_TM_W_ID_TITLE);
	m_Wall1Ctrls.Add(IDC_TM_W_ID_COMBO);
	m_Wall1Ctrls.Add(IDC_TM_W_ID_EDIT);
	m_Wall1Ctrls.Add(IDC_TM_W_ST_TITLE);

	m_Wall1Ctrls.Add(IDC_TM_W_WT_TITLE);
	m_Wall1Ctrls.Add(IDC_TM_W_RADIO_PINED);
	m_Wall1Ctrls.Add(IDC_TM_W_RADIO_FIXED);

	m_Wall1Ctrls.Add(IDC_TM_W_NC_TITLE);
	m_Wall1Ctrls.Add(IDC_TM_W_RADIO_PLATE_TYPE);
	m_Wall1Ctrls.Add(IDC_TM_W_RADIO_CRB_TYPE);

	m_WallCRB.Add(IDC_TM_W_WT_TITLE);
	m_WallCRB.Add(IDC_TM_W_RADIO_PINED);
	m_WallCRB.Add(IDC_TM_W_RADIO_FIXED);

	m_WallCRB.Add(IDC_TM_W_NC_TITLE);
	m_WallCRB.Add(IDC_TM_W_RADIO_PLATE_TYPE);
	m_WallCRB.Add(IDC_TM_W_RADIO_CRB_TYPE);

	m_WallNodeConnectRdo.Add(IDC_TM_W_RADIO_PINED);
	m_WallNodeConnectRdo.Add(IDC_TM_W_RADIO_FIXED);

	m_WallTypeRdo.Add(IDC_TM_W_RADIO_PLATE_TYPE);
	m_WallTypeRdo.Add(IDC_TM_W_RADIO_CRB_TYPE);

	m_arPlateTypeCtrl.Add(IDC_TM_ELEM_TYPE_TITLE);
	m_arPlateTypeCtrl.Add(IDC_TM_ELEM_TYPE_THICK);
	m_arPlateTypeCtrl.Add(IDC_TM_ELEM_TYPE_THIN);

#if defined(_MGEN)
	m_HeightCtrls.Add(IDC_TM_HEIGHT_CMB);
	m_HeightCtrls.Add(IDC_TM_HEIGHT_EDT);
	m_HeightCtrls.Add(IDC_TM_HEIGHT_UNT);
#endif

	m_aOrientation.RemoveAll();
	m_aOrientation.Add(IDC_TM_GCS_RDO);
	m_aOrientation.Add(IDC_TM_REF_ANGLE_RDO);
	m_aOrientation.Add(IDC_TM_REF_ANGLE_EDT);
	m_aOrientation.Add(IDC_TM_REF_ANGLE_UNT);
	m_aOrientation.Add(IDC_TM_REF_AXIS_CMB);
	m_aOrientation.Add(IDC_TM_REF_BUTTON);

	m_aRefAngle.RemoveAll();
	m_aRefAngle.Add(IDC_TM_REF_ANGLE_EDT);
	m_aRefAngle.Add(IDC_TM_REF_ANGLE_UNT);

	m_aRefAxis.RemoveAll();
	m_aRefAxis.Add(IDC_TM_REF_AXIS_CMB);

	m_aNodalConnec.Add(IDC_TM_NODES_STATIC);
	m_aNodalConnec.Add(IDC_TM_NODES_EDIT);
	m_aNodalConnec.Add(IDC_TM_ORTHO_CHECK);
	m_aNodalConnec.Add(IDC_TM_NODES_COBX);
	m_aNodalConnec.Add(IDC_TM_NODES_USEREDIT);
	m_aNodalConnec.Add(IDC_TM_NODES_ENTER_BTN);
	m_aNodalConnec.Add(IDC_WG_TREEMENU_STATIC0);
	m_aNodalConnec.Add(IDC_TM_INTSECT_NODE_CHECK);
	m_aNodalConnec.Add(IDC_TM_INTSECT_ELEM_CHECK);
	m_aNodalConnec.Add(IDC_TM_INTST_TOL);
	m_aNodalConnec.Add(IDC_TM_INNER_INTSECT_CHECK);
}

int CElemCreateDlg::GetWallMode()
{
	CString strSel = _T("");
	int nIndex = m_cboWall.GetCurSel();
	if (nIndex < 0) return -1;
	m_cboWall.GetLBText(nIndex, strSel);

	if (strSel == STR_WALL_FIXED) return 0;
	else if (strSel == STR_WALL_INCR1) return 1;
	else if (strSel == STR_WALL_LAST1) return 2;

	return -1;
}

BOOL CElemCreateDlg::OnInitDialog()
{
	MInitCombo initCombo;
	SetRedraw(FALSE);

	CMenuBarChildDlg::OnInitDialog();	

	InitPropertyCtrls();

	InitElemTypeCombo();

	InitTensCableCombo();

	InitHeightCombo();
	
	//  UpdateNodeNumCtrls();
	m_bPlanePos = TRUE; //위치조정.

	int nEType;
	int nIndex = m_ctrlCobxEType.GetCurSel();
	if (nIndex != CB_ERR)
	{
		nEType = (int)m_ctrlCobxEType.GetItemData(nIndex);
		ChildLayOut(nEType);
		AlignControls();
		KNodeEnableDisable();
		ShowHideElemTypeCtrl(nEType);
		UpdateNodeNumCtrls(); // Plate일때 컨트롤이 보이지 않아 위치 수정.
		EnableDisableIntersectCtrl(nEType);
	}
	//  UpdateNodeNumCtrls(); // Plate일때 컨트롤이 보이지 않아 위치 수정.

	m_wndTTDistEdit.SetAttUcsDistance();
	m_wndTTDistEdit.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndTTDistEdit.SetDistValueMode(TRUE);
	m_wndCTDistEdit.SetAttUcsDistance();
	m_wndCTDistEdit.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndCTDistEdit.SetDistValueMode(TRUE);

	m_wndTTDistUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndTTForceUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_wndCTDistUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndTTLengthUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndCTAlTensUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_wndTTAlCompUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_wndCTAlTensEdit.SetUnitType(D_UNITSYS_BASE_FORCE); // MNET:2774 20070508 mylee
	m_wndTTAlCompEdit.SetUnitType(D_UNITSYS_BASE_FORCE);

	m_untCompLimit.SetUnitType(D_UNITSYS_BASE_FORCE); // MNET:2774 20070508 mylee
	m_untTensLimit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_edtCompLimit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_edtTensLimit.SetUnitType(D_UNITSYS_BASE_FORCE);

	m_wndBAngleEdit.AddString(_T("0"));
	m_wndBAngleEdit.AddString(_LS(IDS_WG_TREEMENU_90));
	m_wndBAngleEdit.AddString(_LS(IDS_WG_TREEMENU_180));

	m_wndAngleUnit.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_wndDistUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndVectorUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_wndNodeListEdit.SetAttNothing();
	m_wndNodeListEdit.SetModeToUse(MOUSEEDIT_USE_NODE_LIST_INPUT);
	m_wndNodeListEdit.SetEscNotifyWindow(this);
	m_wndNodeListEdit.SetEnterNotifyWindow(this);
	m_wndNodeListEdit.SetLButtonDownNotifyWindow(this);

	m_wndRefPointEdit.SetAttUcsPos();
	m_wndRefPointEdit.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndRefPointEdit.SetEnterNotifyWindow(this);
	m_wndRefPointEdit.SetLButtonDownNotifyWindow(this);

	m_wndDirVectorEdit.SetAttUcsDistance();
	m_wndDirVectorEdit.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndDirVectorEdit.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));

	m_wndNodeUserEdit.SetElemCreate();

	m_edtHeight.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untHeigth.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtRefAngle.ResetContent();
	m_edtRefAngle.AddString(_T("0"));
	m_edtRefAngle.AddString(_LS(IDS_WG_TREEMENU_90));
	m_edtRefAngle.AddString(_LS(IDS_WG_TREEMENU_180));
	m_untRefAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);

	InitComboNucs();
	//  GetDlgItem(IDC_TM_REF_POINT_RADIO)->EnableWindow(FALSE); // 사내배포에서 제외
		// 다시 기능이 되도록 함. added by Bugboy 2000.9.16

	CDlgUtil::CtrlRadioSetCheck(this, m_TensTrussCtrls, 0);
	CDlgUtil::CtrlRadioSetCheck(this, m_CompTrussCtrls, 0);

#if defined(_CH)
	CDlgUtil::CtrlRadioSetCheck(this, m_WallCtrls, 1);  // MNET:2087-JBSEON-20060421
#else
	CDlgUtil::CtrlRadioSetCheck(this, m_WallCtrls, 0);
#endif

	CDlgUtil::CtrlRadioSetCheck(this, m_WallNodeConnectRdo, 0);
	CDlgUtil::CtrlRadioSetCheck(this, m_WallTypeRdo, 0);

	//*^^*
	m_cboWall.ResetContent();
	m_cboWall.AddString(STR_WALL_FIXED);
	m_cboWall.AddString(STR_WALL_INCR1);
	m_cboWall.AddString(STR_WALL_LAST1);
	m_cboWall.SelectString(-1, STR_WALL_INCR1);
	int nMode = GetWallMode();
	if (nMode >= 0) m_editWall.SetMode(nMode);
	m_editWall.SetEditBoxText(nMode);

	TrussCtrlMan();

	ChangeBitmap();
	ChangeText();

	m_chkCreateInnerPoint.SetCheck(FALSE);

	((CButton*)GetDlgItem(IDC_TM_ELEM_TYPE_THICK))->SetCheck(TRUE);

	// Orthogonal Mode Setting...by L.C.G
	if (m_nCheckOrtho)
		I_GENModelBase::GetCurMySelfST()->SetTemp_E_XYZOrthoMode();
	else
		I_GENModelBase::GetCurMySelfST()->SetTemp_E_FreeMode();

	//////////////////////////////////////////////////////////////////
	// PreParsing Mode 설정...
	m_wndNodeUserEdit.SetPreParsingMode(m_nPreParseType);

	GotoDlgCtrl(&m_wndNodeListEdit);

	// user edit ///////////////
	m_cobxNodeEdit.SetCurSel(0);
	////////////////////////////

	OnTmClickVertiType();
	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CElemCreateDlg::InitComboNucs()
{
#ifndef _MEC
	return;
#endif
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();

	CArray<T_NUCS_K, T_NUCS_K> aNucsK;
	pDoc->m_pAttrCtrl->GetNucsKeyList(aNucsK);

	T_NUCS_D NucsD; NucsD.Initialize();

	m_cmbRefAxis.ResetContent();
	for (int i = 0; i < aNucsK.GetSize(); i++)
	{
		if (!pDoc->m_pAttrCtrl->GetNucs(aNucsK[i], NucsD)) continue;

		m_cmbRefAxis.SetItemData(m_cmbRefAxis.AddString(NucsD.UcsName), aNucsK[i]);
	}
	m_cmbRefAxis.SetCurSel(0);
}

void CElemCreateDlg::ChangeText()
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	int nElemType = GetCurElemTypeID();
	if (pDoc->m_pAttrCtrl->IsPlstrn(nElemType) || pDoc->m_pAttrCtrl->IsAxisym(nElemType))
	{
		GetDlgItem(IDC_TM_REF_ANGLE_RDO)->SetWindowText(_LS(IDS_TM_REF_ANGLE));
	}
	else
	{
		GetDlgItem(IDC_TM_REF_ANGLE_RDO)->SetWindowText(_LS(IDS_TM_REF_AXIS));
	}
}

void CElemCreateDlg::OnTabSelect(WPARAM wParam, LPARAM lParam)
{
	if (::IsWindow(m_hWnd))
	{
		GotoDlgCtrl(&m_wndNodeListEdit);
	}
}


void CElemCreateDlg::InitPropertyCtrls()
{
	m_ctrlCobxMatl.SetEditBoxPoint(&m_ctrlEditMatl);
	m_ctrlCobxSect.SetEditBoxPoint(&m_ctrlEditSect);
	m_ctrlCobxThik.SetEditBoxPoint(&m_ctrlEditThik);

	m_ctrlEditMatl.SetComboBoxPoint(&m_ctrlCobxMatl);
	m_ctrlEditSect.SetComboBoxPoint(&m_ctrlCobxSect);
	m_ctrlEditThik.SetComboBoxPoint(&m_ctrlCobxThik);

	if (!m_bPropInit)
	{
		m_ctrlEditMatl.SetEditBoxText(1);
		m_ctrlEditSect.SetEditBoxText(1);
		m_ctrlEditThik.SetEditBoxText(1);
		m_bPropInit = TRUE;
	}
}

void CElemCreateDlg::InitElemTypeCombo()
{
	m_ElemTypeIDs.RemoveAll();

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	// 추가할 Element Type을 Element Type Array에 설정한다. 
	// Array에 Add한 순서대로 List에 추가된다. Item Data는 Element 
	// Type으로 설정된다. 
	m_ElemTypeIDs.Add(pDoc->m_pAttrCtrl->GetTrussID());			//truss	       
#ifndef _TOWER	
	m_ElemTypeIDs.Add(pDoc->m_pAttrCtrl->GetTensTrussID());	//tension only truss 
	m_ElemTypeIDs.Add(pDoc->m_pAttrCtrl->GetCompTrussID());	//compression only truss 
	m_ElemTypeIDs.Add(pDoc->m_pAttrCtrl->GetBeamID());			//beam         
	m_ElemTypeIDs.Add(pDoc->m_pAttrCtrl->GetPlateID());			//plate	       
	m_ElemTypeIDs.Add(pDoc->m_pAttrCtrl->GetPlstrsID());		//plane	stress 
	m_ElemTypeIDs.Add(pDoc->m_pAttrCtrl->GetPlstrnID());		//plane	strain 
	m_ElemTypeIDs.Add(pDoc->m_pAttrCtrl->GetAxisymID());		//axisymmetric 
	m_ElemTypeIDs.Add(pDoc->m_pAttrCtrl->GetSolidID());			//solid	       
#ifndef _CIVIL
	m_ElemTypeIDs.Add(pDoc->m_pAttrCtrl->GetWallID());		//wall         
#endif
#endif


	//m_ElemTypeIDs.Add(pDoc->m_pAttrCtrl->GetPanelID());			//panel	       
	//m_ElemTypeIDs.Add(pDoc->m_pAttrCtrl->GetBDSSlabID());		//bds slab     
	//m_ElemTypeIDs.Add(pDoc->m_pAttrCtrl->GetSDSSlabID());		//sds slab     
	int nETypes = m_ElemTypeIDs.GetSize();

	for (int i = 0; i < nETypes; i++)
	{
		// m_ctrlCobxEType은 Sort Item Option을 반드시 제거할것...
		//m_ctrlCobxEType.AddString(pDoc->m_pAttrCtrl->GetEltypName(m_ElemTypeIDs[i]));    
		m_ctrlCobxEType.AddString(CDBLib::GetEltypName(m_ElemTypeIDs[i]));
		m_ctrlCobxEType.SetItemData(i, (DWORD)m_ElemTypeIDs[i]);
	}
	AdjustComboListBox(m_ctrlCobxEType);

	m_ctrlCobxEType.SetCurSel(m_nCurETypeIndex);

	if (m_nCurETypeIndex != -1)
	{
		int nEType;
		nEType = (int)m_ctrlCobxEType.GetItemData(m_nCurETypeIndex);
		ChildLayOut(nEType);
	}

	OnTmClickTensLimit(); // MNET:XXXX 20070531 mylee
	OnTmClickCompLimit();
}

void CElemCreateDlg::AdjustComboListBox(MComboBox& Cbx)
/*
콤보박스의 길이를 현재 사용되고 있는 리스트의 길이에
맞추어 변경한다.
*/
{
	CString str;
	CSize   sz;
	int     dx = 0;
	CDC* pDC = Cbx.GetDC();
	for (int i = 0; i < Cbx.GetCount(); i++)
	{
		Cbx.GetLBText(i, str);
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx) dx = sz.cx;
	}
	Cbx.ReleaseDC(pDC);

	if (Cbx.GetDroppedWidth() < dx)
	{
		Cbx.SetDroppedWidth(dx);
		ASSERT(Cbx.GetDroppedWidth() == dx);
	}
}

void CElemCreateDlg::ShowHideElemTypeCtrl(int nElemType)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	BOOL bNormalElem = FALSE;
	BOOL bTensTruss = FALSE;
	BOOL bCompTruss = FALSE;
	BOOL bWall = FALSE;
	BOOL bPlate = FALSE;
	BOOL bPlstrs = FALSE;
	if (pDoc->m_pAttrCtrl->IsTensTruss(nElemType)) bTensTruss = TRUE;
	else if (pDoc->m_pAttrCtrl->IsCompTruss(nElemType)) bCompTruss = TRUE;
	else if (pDoc->m_pAttrCtrl->IsWall(nElemType)) bWall = TRUE;
	else if (pDoc->m_pAttrCtrl->IsPlate(nElemType)) bPlate = TRUE;
	else if (pDoc->m_pAttrCtrl->IsPlstrs(nElemType)) bPlstrs = TRUE;
	else bNormalElem = TRUE;

	if (bNormalElem)
	{
		CtrlShowHide(m_NormalElemCtrls, bNormalElem);
		//    UpdateNodeNumCtrls();
	}
	else
		CtrlShowHide(m_NormalElemCtrls, bNormalElem);

	CtrlShowHide(m_TensTrussCtrls, bTensTruss);
	if (bTensTruss)
	{
		int nCheck;
		CDlgUtil::CtrlRadioGetCheck(this, m_TensTrussCtrls, nCheck);
		CtrlShowHide(m_TensTruss0Ctrls, nCheck == 0);
		CtrlShowHide(m_TensTruss1Ctrls, nCheck == 1);
		CtrlShowHide(m_TensTruss2Ctrls, nCheck == 2);
		CtrlShowHide(m_TensTruss3Ctrls, nCheck == 2);


		if (nCheck == 2) ChangeTensCableCombo();
	}
	else
	{
		CtrlShowHide(m_TensTruss0Ctrls, bTensTruss);
		CtrlShowHide(m_TensTruss1Ctrls, bTensTruss);
		CtrlShowHide(m_TensTruss2Ctrls, bTensTruss);
		CtrlShowHide(m_TensTruss3Ctrls, bTensTruss);
	}

	CtrlShowHide(m_CompTrussCtrls, bCompTruss);
	if (bCompTruss)
	{
		int nCheck;
		CDlgUtil::CtrlRadioGetCheck(this, m_CompTrussCtrls, nCheck);
		CtrlShowHide(m_CompTruss0Ctrls, nCheck == 0);
		CtrlShowHide(m_CompTruss1Ctrls, nCheck == 1);
	}
	else
	{
		CtrlShowHide(m_CompTruss0Ctrls, bCompTruss);
		CtrlShowHide(m_CompTruss1Ctrls, bCompTruss);
	}
	CtrlShowHide(m_WallCtrls, bWall);
	CtrlShowHide(m_Wall1Ctrls, bWall);
	CtrlShowHide(m_arPlateTypeCtrl, bPlate);

	if (bWall)
	{
		OnTmClickWallType();
	}
	if (m_bIntersectNode && (m_nCurETypeIndex > 3))
		m_chkCreateInnerPoint.EnableWindow(TRUE);
	else
		m_chkCreateInnerPoint.EnableWindow(FALSE);

	((CButton*)GetDlgItem(IDC_TM_ELEM_WITH_DRILL_CHK))->ShowWindow(bPlate || bPlstrs);
	//  CButton* pBtn = GetDlgItem(IDC_TM_ELEM_WITH_DRILL_CHK);
	//  pBtn->ShowWindow(bPlate || bPlstrs);
}

//#ifndef _MDEMO
void CElemCreateDlg::EnableDisableIntersectCtrl(int nElemType)
{
	UpdateData(TRUE);
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	BOOL bIntersectNode = FALSE;
	if (pDoc->m_pAttrCtrl->IsFrameType(nElemType)) bIntersectNode = TRUE;
	else if (pDoc->m_pAttrCtrl->IsPlaneType(nElemType))
	{
		if (m_nNodeNumIndex == 1) bIntersectNode = TRUE;
	}
	else if (pDoc->m_pAttrCtrl->IsSolidType(nElemType))
	{
		if (m_nNodeNumIndex == 2 || m_nNodeNumIndex == 3) bIntersectNode = TRUE;
	}
	CMenuBarChildDlg::CtrlEnableDisable(m_IntersectNodeCtrls, bIntersectNode);
	CMenuBarChildDlg::CtrlEnableDisable(m_IntersectElemCtrls, pDoc->m_pAttrCtrl->IsFrameType(nElemType));
}
//#endif
//#ifdef _MDEMO
//void CElemCreateDlg::EnableDisableIntersectCtrl(int nElemType)
//{
//  CDBDoc * pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
//  BOOL bIntersectNode = FALSE;
//  if (pDoc->m_pAttrCtrl->IsFrameType(nElemType)) bIntersectNode = TRUE;
//  else if (pDoc->m_pAttrCtrl->IsPlaneType(nElemType))
//  {
//    if (m_nNodeNumIndex == 1) bIntersectNode = TRUE;
//  }
//  else if (pDoc->m_pAttrCtrl->IsSolidType(nElemType))
//  {
//    bIntersectNode = FALSE; // 무조건 FALSE
//    m_bIntersectNode = FALSE; // Flag 끈다.
//    m_bIntersectElem = FALSE;
//  }
//  CMenuBarChildDlg::CtrlEnableDisable(m_IntersectNodeCtrls,bIntersectNode);
//  CMenuBarChildDlg::CtrlEnableDisable(m_IntersectElemCtrls,pDoc->m_pAttrCtrl->IsFrameType(nElemType));
//}
//#endif


// 현재 설정된 Element Type에 맞게 Control들을 재 배치한다. 
void CElemCreateDlg::ChildLayOut(int nElemType)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	//pDoc->m_pAttrCtrl->IsNeedProp (nElemType);	//property가 필요한	요소종류인가?

	if (pDoc->m_pAttrCtrl->IsNeedSect(nElemType))
	{
		CMenuBarChildDlg::CtrlShowHide(m_SectCtrls, TRUE);
		CMenuBarChildDlg::CtrlEnableDisable(m_SectCtrls, TRUE);
		CMenuBarChildDlg::CtrlShowHide(m_ThickCtrls, FALSE);
		CMenuBarChildDlg::CtrlEnableDisable(m_ThickCtrls, FALSE);
		// KNode는 Frame요소에만 필요하다. 
		CMenuBarChildDlg::CtrlEnableDisable(m_KNodeCtrls, TRUE);		
	}
	else
	{
		// KNode는 Frame요소에만 필요하다. 
		CMenuBarChildDlg::CtrlEnableDisable(m_KNodeCtrls, FALSE);

		if (pDoc->m_pAttrCtrl->IsNeedThik(nElemType))
		{
			CMenuBarChildDlg::CtrlShowHide(m_SectCtrls, FALSE);
			CMenuBarChildDlg::CtrlEnableDisable(m_SectCtrls, FALSE);
			CMenuBarChildDlg::CtrlShowHide(m_ThickCtrls, TRUE);
			CMenuBarChildDlg::CtrlEnableDisable(m_ThickCtrls, TRUE);
		}
		else
		{
			CMenuBarChildDlg::CtrlShowHide(m_ThickCtrls, FALSE);
			CMenuBarChildDlg::CtrlShowHide(m_SectCtrls, TRUE);
			CMenuBarChildDlg::CtrlEnableDisable(m_SectCtrls, FALSE);
		}
	}

	if (pDoc->m_pAttrCtrl->IsPlate(nElemType) ||
		pDoc->m_pAttrCtrl->IsPlstrs(nElemType)/* ||
		pDoc->m_pAttrCtrl->IsPlstrn(nElemType) ||
		pDoc->m_pAttrCtrl->IsAxisym(nElemType)*/)
	{
		CMenuBarChildDlg::CtrlEnableDisable(m_KNodeCtrls, TRUE);

		if (m_nBetaAngleOrRefPoint == 1) m_nBetaAngleOrRefPoint = 0;
		GetDlgItem(IDC_TM_REF_POINT_RADIO)->EnableWindow(FALSE);		
	}

	if (pDoc->m_pAttrCtrl->IsSolid(nElemType))
	{
		GetDlgItem(IDC_TM_ORI_STATIC1)->EnableWindow(TRUE);	
	}

	KNodeEnableDisable();

	/*
	if(pDoc->m_pAttrCtrl->IsNeedKnode(nElemType))//knode가 필요한 요소종류인가?
	{
		CMenuBarChildDlg::CtrlEnableDisable(m_KNodeCtrls,TRUE);
		KNodeEnableDisable();
	}
	else
	{
		CMenuBarChildDlg::CtrlEnableDisable(m_KNodeCtrls,FALSE);
	}

	if(pDoc->m_pAttrCtrl->IsNeedSect (nElemType))//section이	필요한 요소종류인가?
	{
		CMenuBarChildDlg::CtrlShowHide(m_SectCtrls,TRUE);
		CMenuBarChildDlg::CtrlEnableDisable(m_SectCtrls,TRUE);
	}
	else
	{
		if(pDoc->m_pAttrCtrl->IsSolidType(nElemType))
			CMenuBarChildDlg::CtrlEnableDisable(m_SectCtrls,FALSE);
		else
			CMenuBarChildDlg::CtrlShowHide(m_SectCtrls,FALSE);
	}

	if(pDoc->m_pAttrCtrl->IsNeedThik (nElemType))//thickness가 필요한 요소종류인가?
	{
		CMenuBarChildDlg::CtrlShowHide(m_ThickCtrls,TRUE);
		CMenuBarChildDlg::CtrlEnableDisable(m_ThickCtrls,TRUE);
	}
	else
	{
		if(pDoc->m_pAttrCtrl->IsSolidType(nElemType))
			CMenuBarChildDlg::CtrlEnableDisable(m_ThickCtrls,FALSE);
		else
			CMenuBarChildDlg::CtrlShowHide(m_ThickCtrls,FALSE);
	}
	*/
}



/*********************************************************************
	 *
	 *
 *****    CONTROL IDS...
	***
	 */
	 /*
	 {
			 IDC_TM_ELTYPE_STATIC
			 IDC_TM_ELEM_TYPE_COMBO

			 IDC_TM_MAT_STATIC1
			 IDC_TM_MAT_NO_EDIT
			 IDC_TM_MAT_STATIC2
			 IDC_TM_MAT_STATIC3
			 IDC_TM_MAT_NAME_COMBO
			 IDC_TM_MAT_BUTTON

			 IDC_TM_SEC_STATIC1
			 IDC_TM_SEC_NO_EDIT
			 IDC_TM_SEC_STATIC2
			 IDC_TM_SEC_STATIC3
			 IDC_TM_SEC_NAME_COMBO
			 IDC_TM_SEC_BUTTON

			 IDC_TM_ORI_STATIC1
			 IDC_TM_BETA_ANGLE_RADIO
			 IDC_TM_BANGLE_EDIT
			 IDC_TM_REF_POINT_RADIO
			 IDC_TM_REF_POINT_EDIT
			 IDC_TM_DIST_UNIT
			 IDC_TM_ANG_UNIT

			 IDC_TM_NODES_STATIC
			 IDC_TM_NODES_EDIT

			 IDC_TM_THICK_STATIC1
			 IDC_TM_THICK_NO_EDIT
			 IDC_TM_THICK_STATIC2
			 IDC_TM_THICK_STATIC3
			 IDC_TM_THICK_NAME_COMBO
			 IDC_TM_THICK_BUTTON
	 }

		 CDBDoc * pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
		 (pDoc->m_pDataCtrl)->AddNode(0,NodePos,CS.NCopyNum,CS.Dx,CS.Dy,CS.Dz,m_bIsCreateSamePos);


		 int	ChangeEltyp(int	eltyp);				//요소종류번호 변경(bn1	용)
		 BOOL IsValidEltyp(int eltyp);			//요소종류번호 검증
		 int	GetMaxNumNode(int eltyp);			//요소종류별 최대허용 절점수
		 int	GetNumNode(int eltyp,int* elnod);	//요소종류별 사용절점수
		 CString	GetEltypName(int eltyp);		//요소종류이름
		 BOOL IsFrameType(int eltyp);			//선 종류인가?
		 BOOL IsPlaneType(int eltyp);			//판 종류인가?
		 BOOL IsSolidType(int eltyp);			//솔리드 종류인가?
		 BOOL IsTruss(int eltyp);				//truss	인가?
		 int	GetTrussID();						//truss	요소종류번호 얻기
		 BOOL IsBeam(int	eltyp);					//beam 인가?
		 int	GetBeamID();						//beam 요소종류번호	얻기
		 BOOL IsPlstrs(int eltyp);				//plane	stress 인가?
		 int	GetPlstrsID();						//plane	stress 요소종류번호	얻기
		 BOOL IsPlate(int eltyp);				//plate	인가?
		 int	GetPlateID();						//plate	요소종류번호 얻기
		 BOOL IsWall(int	eltyp);					//wall 인가?
		 int	GetWallID();						//wall 요소종류번호	얻기
		 BOOL IsPlstrn(int eltyp);				//plane	strain 인가?
		 int	GetPlstrnID();						//plane	strain 요소종류번호	얻기
		 BOOL IsAxisym(int eltyp);				//axisymmetric 인가?
		 int	GetAxisymID();						//axisymmetric 요소종류번호	얻기
		 BOOL IsSolid(int eltyp);				//solid	인가?
		 int	GetSolidID();						//solid	요소종류번호 얻기
		 BOOL IsPanel(int eltyp);				//panel	인가?
		 int	GetPanelID();						//panel	요소종류번호 얻기
		 BOOL IsBDSSlab(int eltyp);				//bds slab 인가?
		 int	GetBDSSlabID();						//bds slab 요소종류번호	얻기
		 BOOL IsSDSSlab(int eltyp);				//sds slab 인가?
		 int	GetSDSSlabID();						//sds slab 요소종류번호	얻기
		 BOOL IsTensTruss(int eltyp);			//tension only truss 인가?
		 int	GetTensTrussID();					//tension only truss 요소종류번호 얻기
		 BOOL IsCompTruss(int eltyp);			//compression only truss 인가?
		 int	GetCompTrussID();					//compression only truss 요소종류번호 얻기
		 BOOL IsNeedKnode(int eltyp);			//knode가 필요한 요소종류인가?
		 BOOL IsNeedProp(int	eltyp);				//property가 필요한	요소종류인가?
		 BOOL IsNeedSect(int	eltyp);				//section이	필요한 요소종류인가?
		 BOOL IsNeedThik(int	eltyp);				//thickness가 필요한 요소종류인가?
		 BOOL IsVerticalFrame(const T_NODE_D& Node_I, const T_NODE_D& Node_J);				//수직부재인가 1/100 오차허용?
		 BOOL IsVerticalFrame(const double Coor_i[3], const double Coor_j[3]);
		 int	GetEltypeCount(int eltyp);			//요소종류별 요소갯수
	 */


void CElemCreateDlg::OnSelchangeTmElemTypeCombo()
{
	m_nCurETypeIndex = m_ctrlCobxEType.GetCurSel();
	int nElemType = (int)m_ctrlCobxEType.GetItemData(m_nCurETypeIndex);

	AlignControls();
	ChildLayOut(nElemType);
	//  UpdateNodeNumCtrls();

	// pig-2k/9/17
	//  EnableDisableIntersectCtrl(nElemType);
	//  ShowHideElemTypeCtrl(nElemType);
	//  UpdateNodeNumCtrls();
	ShowHideElemTypeCtrl(nElemType);
	UpdateNodeNumCtrls();
	EnableDisableIntersectCtrl(nElemType);

	I_GENModelBase::SetModelingModeST(IG_STATE_RESET);
	m_wndNodeListEdit.SetWindowText(_T(""));
	SetTempElemDataByCurSetting();
	ChangeBitmap();
	ChangeText();
}

void CElemCreateDlg::OnTmMatButton()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_PROP_MATERIAL, 0));
}

void CElemCreateDlg::OnTmSecButton()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_PROP_SECTION, 0));

}

void CElemCreateDlg::OnTmThickButton()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_PROP_THICKNESS, 0));
}

void CElemCreateDlg::OnTmBetaAngleOrRefPointRadio()
{
	CWnd* pWnd = GetDlgItem(IDC_TM_BETA_ANGLE_RADIO);

	UpdateData(TRUE);
	KNodeEnableDisable();
}

void CElemCreateDlg::OrientationEnableDisable(int nElemType)
{
	CMenuBarChildDlg::CtrlShowHide(m_aOrientation, FALSE);
#ifdef _MEC
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc->m_pAttrCtrl->IsPlstrn(nElemType) || pDoc->m_pAttrCtrl->IsAxisym(nElemType) || pDoc->m_pAttrCtrl->IsSolid(nElemType))
	{
		CMenuBarChildDlg::CtrlShowHide(m_KNodeCtrls, FALSE);
		CMenuBarChildDlg::CtrlShowHide(m_aOrientation, TRUE);
		GetDlgItem(IDC_TM_ORI_STATIC1)->ShowWindow(TRUE);

		if (pDoc->m_pAttrCtrl->IsPlstrn(nElemType) || pDoc->m_pAttrCtrl->IsAxisym(nElemType))
		{

			CMenuBarChildDlg::CtrlShowHide(m_aRefAngle, TRUE);
			CMenuBarChildDlg::CtrlShowHide(m_aRefAxis, FALSE);
			GetDlgItem(IDC_TM_REF_BUTTON)->ShowWindow(FALSE);

			if (pDoc->m_pAttrCtrl->IsAxisym(nElemType))
			{
				CMenuBarChildDlg::CtrlEnableDisable(m_aRefAngle, FALSE);
				CMenuBarChildDlg::CtrlEnableDisable(m_aOrientation, FALSE);
			}
			else
			{
				CMenuBarChildDlg::CtrlEnableDisable(m_aOrientation, TRUE);
				CMenuBarChildDlg::CtrlEnableDisable(m_aRefAngle, m_nRefAngle == 1);
			}
		}
		if (pDoc->m_pAttrCtrl->IsSolid(nElemType))
		{
			CMenuBarChildDlg::CtrlEnableDisable(m_aOrientation, TRUE);
			CMenuBarChildDlg::CtrlEnableDisable(m_aRefAxis, m_nRefAngle == 1);
			GetDlgItem(IDC_TM_REF_BUTTON)->EnableWindow(m_nRefAngle == 1);


			CMenuBarChildDlg::CtrlShowHide(m_aRefAxis, TRUE);
			GetDlgItem(IDC_TM_REF_BUTTON)->ShowWindow(TRUE);

			CMenuBarChildDlg::CtrlShowHide(m_aRefAngle, FALSE);
		}
	}
#endif
}

void CElemCreateDlg::KNodeEnableDisable()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	int nCurETypeIndex = m_ctrlCobxEType.GetCurSel();
	int nElemType = (int)m_ctrlCobxEType.GetItemData(nCurETypeIndex);

#ifdef _MEC
	CMenuBarChildDlg::CtrlShowHide(m_KNodeCtrls, TRUE);
#endif

	if (pDoc->m_pAttrCtrl->IsFrameType(nElemType))
	{
		//    CMenuBarChildDlg::CtrlEnableDisable(m_BAngleCtrls,m_nBetaAngleOrRefPoint == 0);
		//    CMenuBarChildDlg::CtrlEnableDisable(m_RefPointCtrls,m_nBetaAngleOrRefPoint == 1);

		if (m_nBetaAngleOrRefPoint == 0)
		{
			CMenuBarChildDlg::CtrlShowHide(m_BAngleCtrls, TRUE);
			CMenuBarChildDlg::CtrlShowHide(m_RefPointCtrls, FALSE);
			CMenuBarChildDlg::CtrlShowHide(m_DirVectorCtrls, FALSE);
		}
		else if (m_nBetaAngleOrRefPoint == 1)
		{
			CMenuBarChildDlg::CtrlShowHide(m_BAngleCtrls, FALSE);
			CMenuBarChildDlg::CtrlShowHide(m_RefPointCtrls, TRUE);
			CMenuBarChildDlg::CtrlShowHide(m_DirVectorCtrls, FALSE);
		}
		else
		{
			CMenuBarChildDlg::CtrlShowHide(m_BAngleCtrls, FALSE);
			CMenuBarChildDlg::CtrlShowHide(m_RefPointCtrls, FALSE);
			CMenuBarChildDlg::CtrlShowHide(m_DirVectorCtrls, TRUE);
		}
	}
	else
	{
		CMenuBarChildDlg::CtrlEnableDisable(m_BAngleCtrls, FALSE);
		CMenuBarChildDlg::CtrlEnableDisable(m_RefPointCtrls, FALSE);
		CMenuBarChildDlg::CtrlEnableDisable(m_DirVectorCtrls, FALSE);
	}

	if (pDoc->m_pAttrCtrl->IsPlate(nElemType) || pDoc->m_pAttrCtrl->IsPlstrs(nElemType))
	{
		CMenuBarChildDlg::CtrlEnableDisable(m_BAngleCtrls, TRUE);
		CMenuBarChildDlg::CtrlEnableDisable(m_RefPointCtrls, TRUE);
		CMenuBarChildDlg::CtrlEnableDisable(m_DirVectorCtrls, TRUE);

		if (m_nBetaAngleOrRefPoint == 0)
		{
			CMenuBarChildDlg::CtrlShowHide(m_BAngleCtrls, TRUE);
			CMenuBarChildDlg::CtrlShowHide(m_RefPointCtrls, FALSE);
			CMenuBarChildDlg::CtrlShowHide(m_DirVectorCtrls, FALSE);
		}
		else if (m_nBetaAngleOrRefPoint == 2)
		{
			CMenuBarChildDlg::CtrlShowHide(m_BAngleCtrls, FALSE);
			CMenuBarChildDlg::CtrlShowHide(m_RefPointCtrls, FALSE);
			CMenuBarChildDlg::CtrlShowHide(m_DirVectorCtrls, TRUE);
		}
	}

	OrientationEnableDisable(nElemType);
}

void CElemCreateDlg::AngleSpinHandler(CWnd* pWnd, CString& str, NMHDR* pNMHDR, LRESULT* pResult)
{
	double Delta;

	pWnd->GetWindowText(str);

	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	if (!GSimpleFloatParse(str, Delta))
	{
		MyParser Parser;
		if (Parser.ParsingNumberArg(str))
			Parser.GetNumberData(Delta);
		else
			Delta = 0.0;
	}

	if (pNMUpDown->iDelta < 0)
		Delta += 1;
	else
		Delta -= 1;

	str.Format(_LS(IDS_WG_TREEMENU___2f), Delta);

	pWnd->SetWindowText(str);

	*pResult = 0;
}

void CElemCreateDlg::UpdateNodeNumCtrls()
{
	int EType = m_ElemTypeIDs[m_nCurETypeIndex];
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();


	int NumNode = pDoc->m_pAttrCtrl->GetMaxNumNode(EType);

	if (pDoc->m_pAttrCtrl->IsFrameType(EType))
	{
		CMenuBarChildDlg::CtrlShowHide(m_SolidNodesCtrls, FALSE);
		CMenuBarChildDlg::CtrlShowHide(m_PlateNodesCtrls, FALSE);
	}
	else
	{
		CMenuBarChildDlg::CtrlEnableDisable(m_PlateNodesCtrls, TRUE);
		CMenuBarChildDlg::CtrlEnableDisable(m_SolidNodesCtrls, TRUE);

		if (pDoc->m_pAttrCtrl->IsPlaneType(EType))
		{
			CMenuBarChildDlg::CtrlShowHide(m_SolidNodesCtrls, FALSE);
			if (pDoc->m_pAttrCtrl->IsWall(EType)) // Wall : allow only 4 node
				CMenuBarChildDlg::CtrlShowHide(m_PlateNodesCtrls, FALSE);
			else CMenuBarChildDlg::CtrlShowHide(m_PlateNodesCtrls, TRUE);

			CWnd* pWnd = GetDlgItem(IDC_TM_3NODE_RADIO);
			CRect rect;
			pWnd->GetWindowRect(&rect);
			ScreenToClient(&rect);
			if (m_bPlanePos)
			{
				rect.OffsetRect(DISTNODE, 0);
				pWnd->MoveWindow(rect);
				m_bPlanePos = !m_bPlanePos;
			}

			pWnd = GetDlgItem(IDC_TM_4NODE_RADIO);
			pWnd->SetWindowPos(NULL, rect.right + 10, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}

		if (pDoc->m_pAttrCtrl->IsSolidType(EType))
		{
			CMenuBarChildDlg::CtrlShowHide(m_PlateNodesCtrls, FALSE);
			CMenuBarChildDlg::CtrlShowHide(m_SolidNodesCtrls, TRUE);
			CWnd* pWnd = GetDlgItem(IDC_TM_3NODE_RADIO);
			CRect rect;
			pWnd->GetWindowRect(&rect);
			ScreenToClient(&rect);
			if (!m_bPlanePos)
			{
				rect.OffsetRect(-DISTNODE, 0);
				pWnd->MoveWindow(rect);
				m_bPlanePos = !m_bPlanePos;
			}

			pWnd = GetDlgItem(IDC_TM_4NODE_RADIO);
			pWnd->SetWindowPos(NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			pWnd->GetWindowRect(&rect);
			ScreenToClient(&rect);

			pWnd = GetDlgItem(IDC_TM_6NODE_RADIO);
			pWnd->SetWindowPos(NULL, rect.right, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			pWnd->GetWindowRect(&rect);
			ScreenToClient(&rect);

			pWnd = GetDlgItem(IDC_TM_8NODE_RADIO);
			pWnd->SetWindowPos(NULL, rect.right, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}

		if (pDoc->m_pAttrCtrl->IsPlaneType(EType))
		{
			m_nNodeNumIndex = 1;
			UpdateData(FALSE);
		}

		if (pDoc->m_pAttrCtrl->IsSolidType(EType))
		{
			m_nNodeNumIndex = 3;
			UpdateData(FALSE);
		}
	}

	if (pDoc->m_pAttrCtrl->IsAxisym(EType))
	{
		m_nRefAngle = 0;
		UpdateData(FALSE);
		OnTmOrientationRdo();
	}
}

/*********************************************************************
	 *
	 *
 *****   ELEMENT CREATION PARAMETER 설정 OPERATION
	***
	 */
int  CElemCreateDlg::GetCurElemTypeID()
{
	m_nCurETypeIndex = m_ctrlCobxEType.GetCurSel();
	return m_ElemTypeIDs[m_nCurETypeIndex];
}

// Element Type에 따른 Data설정이 이루어 지고 있음...
BOOL CElemCreateDlg::GetPropIDS(int nElemType, int& nMatID, int& nSecID, int& nThkID)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	CString strT, strS, strM;
	m_ctrlEditThik.GetWindowText(strT);
	m_ctrlEditSect.GetWindowText(strS);
	m_ctrlEditMatl.GetWindowText(strM);

	if (pDoc->m_pAttrCtrl->IsNeedMatl(nElemType))
		GetINumber(strM, nMatID);
	else nMatID = 0;
	if (pDoc->m_pAttrCtrl->IsNeedSect(nElemType))
		GetINumber(strS, nSecID);
	else nSecID = 0;
	if (pDoc->m_pAttrCtrl->IsNeedThik(nElemType))
		GetINumber(strT, nThkID);
	else nThkID = 0;

	return TRUE;
}
// Element Type에 따른 Data설정이 이루어 지고 있음...
BOOL CElemCreateDlg::GetKNodeDatas(int nElemType, CArray<unsigned int, unsigned int>& NodeKeyList, int& nBAngleOrRefP,
	int& nRefAngle, double& BAngle, double& RPWx, double& RPWy, double& RPWz, unsigned int& NucsK)
{
	CString BAStr, RefPStr, VecStr, RefAStr;
	double  RPUx, RPUy, RPUz;
	m_wndRefPointEdit.GetWindowText(RefPStr);
	m_wndBAngleEdit.GetWindowText(BAStr);
	m_wndDirVectorEdit.GetWindowText(VecStr);
	m_edtRefAngle.GetWindowText(RefAStr);

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();

	if (pDoc->m_pAttrCtrl->IsNeedKnode(nElemType))//knode가 필요한 요소종류인가?
	{
		nBAngleOrRefP = m_nBetaAngleOrRefPoint;
		if (nBAngleOrRefP == 0) // BetaAngle Need
		{
			GetFloatNumber(BAStr, BAngle);
			RPWx = RPWy = RPWz = 0.0;
		}
		else if (nBAngleOrRefP == 1)// Reference Point Need
		{
			BAngle = 0.0;
			GetPosition(RefPStr, RPWx, RPWy, RPWz);
			pIGM->GetWcsPos(RPWx, RPWy, RPWz, RPWx, RPWy, RPWz); // UCS Data를 WCS Data로 변환 한다.
		}
		else   // Vector
		{
			m_wndDirVectorEdit.GetWindowText(VecStr);
			if (TRUE == m_wndDirVectorEdit.IsKeyPressed())
			{
				if (GetDirectionVector(VecStr, RPUx, RPUy, RPUz))
				{
					pIGM->GetWcsVector(RPUx, RPUy, RPUz, RPWx, RPWy, RPWz);
				}
				else return FALSE;
			}
			else
			{
				m_wndDirVectorEdit.GetCurMousePosDist(RPWx, RPWy, RPWz, RPUx, RPUy, RPUz);
			}

			if (NodeKeyList.GetSize() != 2) return FALSE;
			T_NODE_D NodeI;
			if (!pDoc->m_pAttrCtrl->GetNode(NodeKeyList.GetAt(0), NodeI)) return FALSE;
			BAngle = 0.0;
			RPWx = RPWx + NodeI.x;
			RPWy = RPWy + NodeI.y;
			RPWz = RPWz + NodeI.z;
			pIGM->GetWcsPos(RPWx, RPWy, RPWz, RPWx, RPWy, RPWz); // UCS Data를 WCS Data로 변환 한다.      
		}
	}
	else
	{
		nBAngleOrRefP = -1;
		BAngle = 0.0;
		RPWx = RPWy = RPWz = 0.0;
	}

	if (pDoc->m_pAttrCtrl->IsPlate(nElemType) ||
		pDoc->m_pAttrCtrl->IsPlstrs(nElemType) /*||
		pDoc->m_pAttrCtrl->IsPlstrn(nElemType) ||
		pDoc->m_pAttrCtrl->IsAxisym(nElemType)*/)
	{
		nBAngleOrRefP = m_nBetaAngleOrRefPoint;
		if (nBAngleOrRefP == 0) // BetaAngle Need
		{
			GetFloatNumber(BAStr, BAngle);
			RPWx = RPWy = RPWz = 0.0;
		}
		else if (nBAngleOrRefP == 2) // Vector
		{
			m_wndDirVectorEdit.GetWindowText(VecStr);
			GetDirectionVector(VecStr, RPUx, RPUy, RPUz);
			RPWx = RPUx;
			RPWy = RPUy;
			RPWz = RPUz;
		}
		else
		{
			// Nothing
		}
	}

	if (pDoc->m_pAttrCtrl->IsPlstrn(nElemType) || pDoc->m_pAttrCtrl->IsAxisym(nElemType))
	{
		nRefAngle = m_nRefAngle;

		if (nRefAngle == 1)
		{
			GetFloatNumber(RefAStr, BAngle);
		}
	}

	if (pDoc->m_pAttrCtrl->IsSolid(nElemType))
	{
		NucsK = 0;

		nRefAngle = m_nRefAngle;

		if (nRefAngle == 1)
		{
			if (m_cmbRefAxis.GetCount() > 0)
			{
				NucsK = m_cmbRefAxis.GetItemData(m_cmbRefAxis.GetCurSel());
			}
			else
			{
				GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Ref_Axis));
				return FALSE;
			}
		}
	}
	return TRUE;
}
// Element Type에 따른 Data설정이 이루어 지고 있음...
BOOL CElemCreateDlg::GetNodeListAndNodeNum(int nElemType,
	CArray<unsigned int, unsigned int>& _NodeList,
	int& nMaxNodeNum)
{
	CString strNodes;
	m_wndNodeListEdit.GetWindowText(strNodes);

	//if(!GetNodeList(strNodes,_NodeList)) return FALSE;

	//if(!_NodeList.GetSize()) return FALSE;
	GetNodeList(strNodes, _NodeList);

	if (m_bVerticalType && IsVerticalElemType())
	{
		if (!MakeVerticalNode(_NodeList)) return FALSE;
	}

	//Frame Type  2개로 고정...
	//Plane Type  3,4개     : m_nNodeNumIndex 0 , 1
	//Solid Type  4,6,8개   : m_nNodeNumIndex 1 , 2, 3

	// Element Type과 m_nNodeNumIndex간 matching 관계 검사 할것.  !!!
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if (pDoc->m_pAttrCtrl->IsFrameType(nElemType))
	{
		nMaxNodeNum = 2;
	}
	else
	{
		switch (m_nNodeNumIndex)  // 0 -> 3 Nodes , 1 -> 4 Nodes , 2 -> 6 Nodes , 3 -> 8 Nodes
		{
		case 0:
			nMaxNodeNum = 3;
			break;
		case 1:
			nMaxNodeNum = 4;
			break;
		case 2:
			nMaxNodeNum = 6;
			break;
		case 3:
			nMaxNodeNum = 8;
			break;
		default:
			//ASSERT(0);
			nMaxNodeNum = 0;  // Error !!!
			break;
		}
	}

	// Element Type 별 Node Number와 현재 설정된 Node Number 개수에서 
	// 오류 조건 검사및 처리 ... 
	// 설정된 Node 개수가 MaxNodeNumber보다 작을때 또는 클때...적절한 처리를 한다. 

	return TRUE;
}

BOOL CElemCreateDlg::MakeVerticalNode(CArray<unsigned int, unsigned int>& _NodeList)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();

	// height 0, 음수 허용
	double dHeight = 0.0;
	if (!GetHeightVal(dHeight)) { ASSERT(0); return FALSE; }

	CArray<T_NODE_K, T_NODE_K> aVertNewNode;
	for (int i = _NodeList.GetSize() - 1; i > -1; i--)
	{
		T_NODE_D NodeD;
		if (!pDoc->m_pAttrCtrl->GetNode(_NodeList[i], NodeD)) return FALSE;

		NodeD.z = IsUserDefinedStory() ? NodeD.z + dHeight : dHeight;

		T_NODE_K ExistKey = 0;
		if (!pDoc->m_pAttrCtrl->ExistNode(NodeD.x, NodeD.y, NodeD.z, ExistKey))
		{
			ExistKey = pDoc->m_pDataCtrl->GetNodeKey();
			if (!pDoc->m_pDataCtrl->AddNode(ExistKey, NodeD)) { ASSERT(0); return FALSE; }
		}
		aVertNewNode.Add(ExistKey);
	}
	_NodeList.Append(aVertNewNode);
	return TRUE;
}

BOOL CElemCreateDlg::GetElemTypeAndMaxNodeNum(int& nElemType, int& nMaxNodeNum)
{
	UpdateData(TRUE);
	nElemType = GetCurElemTypeID();

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();

	if (2 == pDoc->m_pAttrCtrl->GetMaxNumNode(nElemType))
	{
		nMaxNodeNum = 2;
	}
	else
	{
		switch (m_nNodeNumIndex)  // 0 -> 3 Nodes , 1 -> 4 Nodes , 2 -> 6 Nodes , 3 -> 8 Nodes
		{
		case 0:
			nMaxNodeNum = 3;
			break;
		case 1:
			nMaxNodeNum = 4;
			break;
		case 2:
			nMaxNodeNum = 6;
			break;
		case 3:
			nMaxNodeNum = 8;
			break;
		default:
			ASSERT(0);
			nMaxNodeNum = 0;  // Error !!!
			break;
		}
	}
	return TRUE;
}

BOOL CElemCreateDlg::GetParameter(_ElemCreate& EC)
{
	UpdateData(TRUE);

	EC.m_nElemType = GetCurElemTypeID();

	BOOL RetVal = TRUE;

	///////////////////////////////////////////////////////////////////////////
	// DB에서 Element Type과 부합되는 Data를 요구하므로 
	// Element Type과 연관된 Data 이외의 Data는 무효한 Data를 설정해야 한다. 
	// 현재 해당 질의 함수에서 이러한 처리를 수행하고 있다. 
	if (!GetNodeListAndNodeNum(EC.m_nElemType, EC.m_NodeKeyList, EC.m_nMaxNodeNum))
		RetVal = FALSE;

	if (!GetKNodeDatas(EC.m_nElemType, EC.m_NodeKeyList, EC.m_nBAngleOrRefPoint,
		EC.m_nRefAngle, EC.m_fBAngle, EC.m_RPWx, EC.m_RPWy, EC.m_RPWz, EC.m_NucsK))
		RetVal = FALSE;
	if (!GetPropIDS(EC.m_nElemType, EC.m_nMatID, EC.m_nSecID, EC.m_nThkID))
		RetVal = FALSE;

	EC.m_bIntersectElem = m_bIntersectElem;
	EC.m_bIntersectNode = m_bIntersectNode;
	EC.m_bCreateInnerPoint = m_chkCreateInnerPoint.GetCheck();

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if (pDoc->m_pAttrCtrl->IsTensTruss(EC.m_nElemType))
	{
		int nType;
		CDlgUtil::CtrlRadioGetCheck(this, m_TensTrussCtrls, nType);
		EC.m_nSubType = nType + 1;
		if (nType == 0) // Truss
		{
			EC.m_dTensionForce = m_wndTTAlCompEdit.GetEditValue();
			int nCheck = ((CButton*)GetDlgItem(IDC_TM_TENS_LMT_CHK))->GetCheck();
			EC.m_bUseLimit = (nCheck == 1);
			if (EC.m_bUseLimit)
				EC.m_dLimit = m_edtTensLimit.GetEditValue();
		}
		else if (nType == 1) // Hook
		{
			if (!GetDistance(&m_wndTTDistEdit, EC.m_dNonlLength))
			{
				GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_distance_));
				return FALSE;
			}
		}
		else if (nType == 2) // Cable
		{
			// *^^*
			EC.m_nTensCableKind = GetTensCableKind();
			if (EC.m_nTensCableKind == 3)   // Lu
			{
				EC.m_dNonlLength = m_wndTTLengthEdit.GetEditValue();
			}
			else if (EC.m_nTensCableKind == 1 || EC.m_nTensCableKind == 2)  // Pretension, Horizontal
				EC.m_dTensionForce = m_wndTTForceEdit.GetEditValue();
			else ASSERT(0);
		}

	}
	else if (pDoc->m_pAttrCtrl->IsCompTruss(EC.m_nElemType))
	{
		int nType;
		CDlgUtil::CtrlRadioGetCheck(this, m_CompTrussCtrls, nType);
		EC.m_nSubType = nType + 1;
		if (nType == 0) // Truss
		{
			EC.m_dTensionForce = m_wndCTAlTensEdit.GetEditValue();
			int nCheck = ((CButton*)GetDlgItem(IDC_TM_COMP_LMT_CHK))->GetCheck();
			EC.m_bUseLimit = (nCheck == 1);
			if (EC.m_bUseLimit)
				EC.m_dLimit = m_edtCompLimit.GetEditValue();
		}
		else if (nType == 1) // Gap
		{
			if (!GetDistance(&m_wndCTDistEdit, EC.m_dNonlLength))
			{
				GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_distance_));
				return FALSE;
			}
		}
	}
	else if (pDoc->m_pAttrCtrl->IsPlate(EC.m_nElemType))
	{
		if (this->IsDlgButtonChecked(IDC_TM_ELEM_WITH_DRILL_CHK))
		{
			if (this->IsDlgButtonChecked(IDC_TM_ELEM_TYPE_THIN))
				EC.m_nSubType = 3;
			else
				EC.m_nSubType = 2;
		}
		else
		{
			if (this->IsDlgButtonChecked(IDC_TM_ELEM_TYPE_THIN))
				EC.m_nSubType = 1;
			else
				EC.m_nSubType = 0;
		}
	}
	else if (pDoc->m_pAttrCtrl->IsWall(EC.m_nElemType))
	{
		int nType;
		CDlgUtil::CtrlRadioGetCheck(this, m_WallCtrls, nType);
		EC.m_nSubType = nType + 1;

		int nNodeConnectivity;
		CDlgUtil::CtrlRadioGetCheck(this, m_WallNodeConnectRdo, nNodeConnectivity);
		EC.m_nNodeConnectivity = nNodeConnectivity;

		int nWallType;
		CDlgUtil::CtrlRadioGetCheck(this, m_WallTypeRdo, nWallType);
		EC.m_nWallType = nWallType;


		CString strWallID = _T("");
		m_editWall.GetWindowText(strWallID);
		EC.m_nWallID = _ttoi(strWallID);
	}
	else if (pDoc->m_pAttrCtrl->IsPlstrs(EC.m_nElemType))
	{
		if (this->IsDlgButtonChecked(IDC_TM_ELEM_WITH_DRILL_CHK))
		{
			EC.m_nSubType = 1;
		}
		else
		{
			EC.m_nSubType = 0;
		}
	}

	return RetVal;
}

BOOL CElemCreateDlg::GetDistance(CMouseEdit* pDistEdit, double& dblDist)
{
	CString strDist;
	pDistEdit->GetWindowText(strDist);
	return GetFloatNumber(strDist, dblDist);
}

/*
***
*****
	*
	*
**********************************************************************/
void CElemCreateDlg::OnSetfocusTmNodesEdit()
{
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	//pIGM->SetTempElemErrorFlag(TRUE);
	//pIGM->CursorClearHide();
	//pIGM->ResetTempElem();
	//pIGM->SetTempElemErrorFlag(FALSE);
	if (pIGM)
	{
		SetTempElemDataByCurSetting();

		pIGM->SetTempElemFirstDrawFlag();
		//pIGM->SetTempElemErrorFlag(TRUE);
		//pIGM->SetTempElemErrorFlag(FALSE);

		CArray<unsigned int, unsigned int> NList;
		pIGM->GetTempElemNodeList(NList);
		if (NList.GetSize() != 0)
		{
			m_wndNodeListEdit.SetWindowText(_T(""));
			pIGM->ResetTempElem();
			I_GENModelBase::SetModelingModeST(IG_STATE_ELEM_ADD);
		}
	}

}

void CElemCreateDlg::SetTempElemDataByCurSetting()
{
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if (!pIGM) return;
	_ElemCreate EC;

	if (GetParameter(EC))
	{
		pIGM->SetTempElemData(EC.m_nElemType, EC.m_nMaxNodeNum);
		pIGM->SetTempElemNodeList(EC.m_NodeKeyList, EC.m_nElemType, EC.m_nMaxNodeNum, pIGM);
	}
}


void CElemCreateDlg::_CreateElement_MEC(_ElemCreate& EC, T_ELEM_D& EData)
{
	BOOL bBetaAngleError = FALSE;

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if (pDoc->m_pAttrCtrl->IsPlstrn(EData.eltyp) || pDoc->m_pAttrCtrl->IsAxisym(EData.eltyp))
	{
		if (EC.m_nRefAngle == 0)
		{
			if (!pDoc->m_pAttrCtrl->CalcElemBetaAngle(EData, 0, 0, 0, 0, 0)) bBetaAngleError = TRUE; // TODO: mec
		}
		else
		{
			if (!pDoc->m_pAttrCtrl->CalcElemBetaAngle(EData, FALSE, EC.m_fBAngle)) bBetaAngleError = TRUE;
		}
	}
	else if (pDoc->m_pAttrCtrl->IsSolid(EData.eltyp))
	{
		if (EC.m_nRefAngle == 0)
		{
			CString strGCS = _T("GCS");

			T_NUCS_K NucsK;
			NucsK = pDoc->m_pAttrCtrl->GetNucsKey(strGCS);

			if (NucsK == 0)
			{
				T_NUCS_D NucsD; NucsD.Initialize();
				NucsD.UcsName = strGCS;
				NucsD.dOrg[0] = 0.0;
				NucsD.dOrg[1] = 0.0;
				NucsD.dOrg[2] = 0.0;
				NucsD.dVx[0] = 1.0;
				NucsD.dVx[1] = 0.0;
				NucsD.dVx[2] = 0.0;
				NucsD.dVy[0] = 0.0;
				NucsD.dVy[1] = 1.0;
				NucsD.dVy[2] = 0.0;

				if (!pDoc->m_pDataCtrl->AddNucs(NucsD)) bBetaAngleError = TRUE;

				NucsK = pDoc->m_pAttrCtrl->GetNucsKey(strGCS);
			}

			EData.NucsK = NucsK;
		}
		else
		{
			EData.NucsK = EC.m_NucsK;
		}
	}

	if (bBetaAngleError)
	{
		I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
		pIGM->SetTempElemErrorFlag(TRUE);
		pIGM->CursorClearHide();
		pIGM->CancelAllTempElem();
	}
	return;
}

void CElemCreateDlg::_CreateElement()
{
	_ElemCreate EC;
	
	if (GetParameter(EC))
	{
		T_ELEM_D EData;
		EData.Initialize();

		EData.eltyp = EC.m_nElemType;
		EData.angle = EC.m_fBAngle;
		EData.elmat = EC.m_nMatID;
		if (EC.m_nSecID != 0)
			EData.elpro = EC.m_nSecID;
		if (EC.m_nThkID != 0)
			EData.elpro = EC.m_nThkID;

		EData.dNonlLength = EC.m_dNonlLength;
		EData.dTensionForce = EC.m_dTensionForce;
		EData.dLimit = EC.m_dLimit; // MNET:2774 20070507 mylee
		EData.bUseLimit = EC.m_bUseLimit;
		EData.dCableLengthRatio = EC.m_dLnLRatio;
		EData.nCableOpt = EC.m_nTensCableKind;
		EData.nSubType = EC.m_nSubType;
		EData.nWallId = EC.m_nWallID;
		EData.nNodeConnectivity = EC.m_nNodeConnectivity;
		EData.nWallType = EC.m_nWallType;

		if (EC.m_nMaxNodeNum == EC.m_NodeKeyList.GetSize())
		{
			for (int i = 0; i < EC.m_nMaxNodeNum; i++)
				EData.elnod[i] = EC.m_NodeKeyList[i];
		}
		else
		{
			GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Invalid_Node_Number____));
		}

#ifdef _MEC
		_CreateElement_MEC(EC, EData);
#endif

		CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
		//    if(!pDoc->m_pDataCtrl->AddElem(0,EData,EC.m_bIntersectNode,EC.m_bIntersectElem))
		if (!pDoc->m_pDataCtrl->AddElem(0, EData, EC.m_bIntersectNode, EC.m_bIntersectElem, EC.m_nBAngleOrRefPoint, EC.m_RPWx, EC.m_RPWy, EC.m_RPWz, EC.m_bCreateInnerPoint))
		{
			I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
			pIGM->SetTempElemErrorFlag(TRUE);
			pIGM->CursorClearHide();
			pIGM->CancelAllTempElem();
			//pIGM->ResetTempElem();

			// 존재 하는 모든 Temp Element 를 Clear한다. 

			//      pIGM->CursorCurPointDraw();
			//pIGM->SetTempElemErrorFlag(FALSE);
		}
		else
		{
			// Add by Water Tree 2000.7.11
			// Wall ID Auto Increment 설정
			CString strTemp;
			m_ctrlCobxEType.GetLBText(m_ctrlCobxEType.GetCurSel(), strTemp);

			// *^^*
			if (strTemp == _LS(IDS_WG_TREEMENU_Wall))
			{
				int nMode = GetWallMode();
				if (nMode == 1)
				{
					CString strText = _T("");
					m_editWall.GetWindowText(strText);
					strText.Format(_T("%d"), _ttoi(strText) + 1);
					m_editWall.SetWindowText(strText);
				}
				else if (nMode == 2)
					m_editWall.UpdateEditBoxText();
			}
		}
	}
	else
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Element_Creation_Arguement_));
	}
}



LRESULT CElemCreateDlg::OnNodeListEnter(WPARAM wParam, LPARAM lParam)
{
	if (m_wndNodeListEdit.GetSafeHwnd() == (HWND)lParam)
	{
		_CreateElement();
		m_wndNodeListEdit.SetWindowText(_T(""));
	}
	return 0L;
}

LRESULT CElemCreateDlg::OnNodeListEscape(WPARAM wParam, LPARAM lParam)
{
	if (m_wndNodeListEdit.GetSafeHwnd() == (HWND)lParam)
	{

		//SetTempElemDataByCurSetting();
		I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
		if (!pIGM) return 0L;

		m_wndNodeListEdit.SetWindowText(_T(""));
		//pIGM->ResetTempElem();
		pIGM->SetModelingModeST(IG_STATE_INITALL);
		pIGM->SetModelingModeST(IG_STATE_ELEM_ADD);

		//SetTempElemDataByCurSetting();
		//pIGM->HideTempElem
		//pIGM->EndTempElem(
		//I_GENModelBase::SetModelingModeST(IG_STATE_INITALL); 
	}
	return 0L;
}

/*********************************************************************
	 *
	 *
 *****  Model View에서의 Mouse Event Notify를 받는다.
	***
	 */
LRESULT CElemCreateDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();

	if (m_wndNodeListEdit.GetSafeHwnd() == (HWND)lParam &&
		pIGM->GetModelingModeST() == IG_STATE_ELEM_ADD)
	{
		CArray<unsigned int, unsigned int> NList;
		pIGM->GetTempElemNodeList(NList);

		CString str = _T("");
		CString Tstr;

		int nCount = NList.GetSize();
		if (nCount == 1)
			m_wndNodeListEdit.SetWindowText(_T(""));

		if (nCount)
		{
			if (nCount == 1)
			{
				str.Format(_LS(IDS_WG_TREEMENU__d), NList[0]);
			}
			else
			{
				str.Format(_LS(IDS_WG_TREEMENU__d), NList[0]);
				for (int i = 1; i < nCount; i++)
				{
					Tstr.Format(_LS(IDS_WG_TREEMENU____d), NList[i]);
					str = str + Tstr;
				}
			}
			m_wndNodeListEdit.SetWindowText(str);


			if (pIGM->IsTempNodeComplete() || (m_bVerticalType && IsVerticalElemType() && pIGM->IsTempNodeHalf()))
			{
				//Element Creation !!!
				_CreateElement();
				//I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
				//pIGM->SetTempElemData(nElemType,nMaxNodeNum);

				//m_wndNodeListEdit.SetFocus();
				pIGM->ResetTempElem();
				pIGM->SetModelingModeST(IG_STATE_INITALL);
				pIGM->SetModelingModeST(IG_STATE_ELEM_ADD);
				m_wndNodeListEdit.ImAssocWindowST();
				//m_wndNodeListEdit.SetFocus();
			}
		}
	}
	//I_GENModelBase::SetModelingModeST(IG_STATE_ELEM_ADD);
	return 0L;
}


BOOL CElemCreateDlg::IsVerticalElemType()
{
	int EType = m_ElemTypeIDs[m_nCurETypeIndex];
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	BOOL bVerticalElemType = pDoc->m_pAttrCtrl->IsFrameType(EType) || pDoc->m_pAttrCtrl->IsWall(EType);
	return bVerticalElemType;
}

void CElemCreateDlg::OnTmIntsectElemCheck()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CElemCreateDlg::OnTmIntsectNodeCheck()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (m_bIntersectNode && (m_nCurETypeIndex > 3))
		m_chkCreateInnerPoint.EnableWindow(TRUE);
	else
		m_chkCreateInnerPoint.EnableWindow(FALSE);
}

void CElemCreateDlg::OnTmClickNodeRadio()
{
	// TODO: Add your control notification handler code here
	I_GENModelBase::SetModelingModeST(IG_STATE_RESET);
	m_wndNodeListEdit.SetWindowText(_T(""));
	SetTempElemDataByCurSetting();

	int nEType;
	int nIndex = m_ctrlCobxEType.GetCurSel();
	if (nIndex != CB_ERR)
	{
		nEType = (int)m_ctrlCobxEType.GetItemData(nIndex);
		EnableDisableIntersectCtrl(nEType);
	}
	ChangeBitmap();
}

void CElemCreateDlg::OnClickTensTrussRadio()
{
	/*
	// TODO: Add your control notification handler code here
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_TensTrussCtrls, nCheck);
	CtrlShowHide(m_TensTruss0Ctrls, nCheck == 0);
	CtrlShowHide(m_TensTruss1Ctrls, nCheck == 1);
	//CtrlShowHide(m_TensTruss2Ctrls, nCheck == 2);
	CtrlShowHide(m_TensTruss2Ctrls, FALSE);
	*/
	TrussCtrlMan();
	ChangeBitmap();
}

void CElemCreateDlg::OnClickCompTrussRadio()
{
	// TODO: Add your control notification handler code here
	/*
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_CompTrussCtrls, nCheck);
	CtrlShowHide(m_CompTruss0Ctrls, nCheck == 0);
	CtrlShowHide(m_CompTruss1Ctrls, nCheck == 1);
	*/
	TrussCtrlMan();
	ChangeBitmap();
}

void CElemCreateDlg::TrussCtrlMan()
{
	int nElemType = GetCurElemTypeID();
	switch (nElemType)
	{
	case TRTENS_EL:
	{
		int nCheck;
		CDlgUtil::CtrlRadioGetCheck(this, m_TensTrussCtrls, nCheck);
		CtrlShowHide(m_TensTruss0Ctrls, nCheck == 0);
		CtrlShowHide(m_TensTruss1Ctrls, nCheck == 1);
		CtrlShowHide(m_TensTruss2Ctrls, nCheck == 2);
		CtrlShowHide(m_TensTruss3Ctrls, nCheck == 2);

		if (nCheck == 2) ChangeTensCableCombo();
	}
	break;
	case TRCOMP_EL:
	{
		int nCheck;
		CDlgUtil::CtrlRadioGetCheck(this, m_CompTrussCtrls, nCheck);
		CtrlShowHide(m_CompTruss0Ctrls, nCheck == 0);
		CtrlShowHide(m_CompTruss1Ctrls, nCheck == 1);
	}
	break;
	default:
		break;
	}
}

void CElemCreateDlg::OnTmIntstTol()
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_INTERSECTTOL, 0));
}

void CElemCreateDlg::OnTmOrthoCheck()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_nCheckOrtho)
		I_GENModelBase::GetCurMySelfST()->SetTemp_E_XYZOrthoMode();
	else
		I_GENModelBase::GetCurMySelfST()->SetTemp_E_FreeMode();
}

void CElemCreateDlg::OnSelchangeTmNodesCobx()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);                  // 0 ABS 1 REL 2 DIST_ANG
	m_wndNodeUserEdit.SetPreParsingMode(m_nPreParseType);
}

void CElemCreateDlg::OnTmNodesEnterBtn()
{
	// TODO: Add your control notification handler code here
	m_wndNodeUserEdit.EnterPressOP();
}

// *^^*
void CElemCreateDlg::OnSelchangeTmWIdCombo()
{
	int nMode = GetWallMode();
	if (nMode >= 0) m_editWall.SetMode(nMode);
	if (nMode == 2) m_editWall.UpdateEditBoxText();
}


///////////////////////////////////////////////////////////////////////////////
//
// DB Update

void CElemCreateDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch (lHint)
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
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CElemCreateDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bChangeStor = FALSE;
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_ELEM_ADD):
		case(UR_ELEM_DEL):
		{
			T_ELEM_K key;
			T_ELEM_D data;
			pViewBuff->GetElem(nKey, key, data);
			if (data.eltyp != WALL_EL) return;

			int nMode = GetWallMode();
			if (nMode == 2)  m_editWall.UpdateEditBoxText();
		}
		break;
		case(UR_STOR_ADD):
		case(UR_STOR_DEL):
		{
			bChangeStor = TRUE;
			InitComboNucs();
		}
		break;
		case(UR_NUCS_ADD):
		case(UR_NUCS_DEL):
		{
			InitComboNucs();
		}
		break;
		default:
			break;
		}
	} // end of while

	if (bChangeStor) InitHeightCombo();
}


//////////////////////////// 요기서 부터 새로 추가

void CElemCreateDlg::InitTensCableCombo()
{
	m_cobxTensCable.ResetContent();
	m_cobxTensCable.AddString(TM_TENS_CABLE_LU);        // Lu
	m_cobxTensCable.AddString(TM_TENS_CABLE_PRETENS);    // Pretension
	m_cobxTensCable.AddString(TM_TENS_CABLE_HORIZ); // Horizontal
	m_cobxTensCable.SetCurSel(0);
}

void CElemCreateDlg::InitHeightCombo()
{
	if (!::IsWindow(m_cbxHeight.m_hWnd)) return;

	m_cbxHeight.ResetContent();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CArray<T_STOR_K, T_STOR_K> aStorK;
	pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);

	T_STOR_D StorD;
	CString strName;
	int nCount = aStorK.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		if (!pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD))   continue;
		strName.Format(_T("%s %s"), StorD.StoryName, _T("FL"));

		m_cbxHeight.SetItemData(m_cbxHeight.AddString(strName), aStorK[i]);
	}

	m_cbxHeight.SetItemData(m_cbxHeight.AddString(_LS(IDS_TMWK__MSG119)), D_LEVEL_USER_DEFINED);
	m_cbxHeight.SetCurSel(0);
	OnSelHeightCmb();
}

int CElemCreateDlg::GetTensCableKind()
{
	CString csKind;
	int nIndex = m_cobxTensCable.GetCurSel();
	if (nIndex == CB_ERR) { ASSERT(0); return -1; }
	m_cobxTensCable.GetLBText(nIndex, csKind);

	if (csKind == TM_TENS_CABLE_LU) return 3;
	if (csKind == TM_TENS_CABLE_PRETENS) return 1;
	if (csKind == TM_TENS_CABLE_HORIZ) return 2;
	return -1;
}

void CElemCreateDlg::OnTmClickTensLimit()
{
	int nCheck = 0;
	nCheck = ((CButton*)GetDlgItem(IDC_TM_TENS_LMT_CHK))->GetCheck();
	if (nCheck == 0) m_edtTensLimit.SetEditUnit(0);
	GetDlgItem(IDC_TM_TENS_LMT_EDT)->EnableWindow(nCheck == 1);
}

void CElemCreateDlg::OnTmClickCompLimit()
{
	int nCheck = 0;
	nCheck = ((CButton*)GetDlgItem(IDC_TM_COMP_LMT_CHK))->GetCheck();
	if (nCheck == 0) m_edtCompLimit.SetEditUnit(0);
	GetDlgItem(IDC_TM_COMP_LMT_EDT)->EnableWindow(nCheck == 1);
}

void CElemCreateDlg::OnTmClickWallType()
{
	int nWallType;
	CDlgUtil::CtrlRadioGetCheck(this, m_WallTypeRdo, nWallType);

	// Plate Base일 때 Node Connect 컨트롤 비활성화(141217, 김종민 부장님 요청)
	CArray<UINT, UINT> aWallNCCtrls;
	aWallNCCtrls.Add(IDC_TM_W_NC_TITLE);
	aWallNCCtrls.Add(IDC_TM_W_RADIO_PINED);
	aWallNCCtrls.Add(IDC_TM_W_RADIO_FIXED);
	CDlgUtil::CtrlEnableDisable(this, aWallNCCtrls, nWallType != 0);
}
void CElemCreateDlg::OnSelchangeTensCableCombo()
{
	// TODO: Add your control notification handler code here
	ChangeTensCableCombo();
}

void CElemCreateDlg::ChangeTensCableCombo()
{
	CWnd* pWnd = NULL;
	pWnd = GetDlgItem(IDC_TM_TT_LENGTH_EDIT);
	if (pWnd == NULL) { ASSERT(0); return; }

	int nKind = GetTensCableKind();
	ASSERT(nKind == 3 || nKind == 1 || nKind == 2);

	pWnd->ShowWindow(nKind == 3);
	pWnd = GetDlgItem(IDC_TM_TT_LENGTH_UNIT);
	if (pWnd == NULL) { ASSERT(0); return; }
	pWnd->ShowWindow(nKind == 3);

	CDlgUtil::CtrlShowHide(this, m_TensTruss3Ctrls, nKind == 1 || nKind == 2);

}
void CElemCreateDlg::OnTmClickVertiType()
{
	UpdateData(TRUE);
	if (m_bVerticalType)
	{
		m_cbxHeight.EnableWindow(TRUE);
		m_untHeigth.EnableWindow(TRUE);

		int count = m_cbxHeight.GetCount();
		int sel = m_cbxHeight.GetCurSel();
		if (count > 0 && count == sel + 1)
		{
			m_edtHeight.EnableWindow(TRUE);
		}
		else
		{
			m_edtHeight.EnableWindow(FALSE);
		}
	}
	else
	{
		CDlgUtil::CtrlEnableDisable(this, m_HeightCtrls, FALSE);
	}
}

void CElemCreateDlg::OnSelHeightCmb()
{
	T_STOR_K StorK = NULL;
	if (!GetSelectedStor(StorK))   return;

	if (StorK == -1)
	{
		m_edtHeight.EnableWindow(TRUE);
		m_untHeigth.EnableWindow(TRUE);
		//     T_STOR_K CurStorK = pStorC->GetCurStor();
		//     double dStoryHeight = pStorC->GetStoryHeight(CurStorK);
		//     m_edtHeight.SetEditUnit(dStoryHeight);
		m_edtHeight.SetFocus();
		m_edtHeight.SetSel(0, -1);
	}
	else
	{
		m_edtHeight.EnableWindow(FALSE);
		m_untHeigth.EnableWindow(FALSE);
		//m_edtHeight.Clear();
	}
	// 
	// 	CCommandHelper::GetInstance()->OnChangeVerticalMembHeight(StorK == D_LEVEL_SELECTED_MEMB);
}

BOOL CElemCreateDlg::GetSelectedStor(T_STOR_K& rKey)
{
	rKey = 0;
	int nSelect = m_cbxHeight.GetCurSel();
	if (nSelect == CB_ERR) return FALSE;

	rKey = m_cbxHeight.GetItemData(nSelect);
	return TRUE;
}

BOOL CElemCreateDlg::GetHeightVal(double& rdHeight)
{
	T_STOR_K StorK = NULL;
	if (!GetSelectedStor(StorK))   return FALSE;

	if (StorK == D_LEVEL_USER_DEFINED)
	{
		rdHeight = m_edtHeight.GetEditValue();
	}
	else
	{
		CDBDoc* pDoc = CDBDoc::GetDocPoint();

		CArray<T_STOR_K, T_STOR_K> aStorK;
		pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);

		rdHeight = pDoc->m_pAttrCtrl->GetStorLevel(StorK);
	}
	return TRUE;
}

bool CElemCreateDlg::IsUserDefinedStory()
{
	T_STOR_K storKey = 0;
	GetSelectedStor(storKey);
	return storKey == D_LEVEL_USER_DEFINED;
}

void CElemCreateDlg::OnTmOrientationRdo()
{
	UpdateData(TRUE);
	KNodeEnableDisable();
}

void CElemCreateDlg::OnTmRefDirCmb()
{
	UpdateData(TRUE);
	KNodeEnableDisable();
}

void CElemCreateDlg::OnTmRefButton()
{
	GRenderView* pRenderView = (GRenderView*)(I_GENModelBase::GetCurMySelfST()->GetOwnerWnd()); ASSERT(pRenderView);

	pRenderView->CreateAndActivateUcsDlg(7);
}

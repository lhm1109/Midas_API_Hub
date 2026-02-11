// NodeProjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "NodeProjectDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_dbRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNodeProjectDlg dialog


CNodeProjectDlg::CNodeProjectDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CNodeProjectDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_pBitmap = 0;
	//{{AFX_DATA_INIT(CNodeProjectDlg)
	m_nCopyOrMove = 0;
	m_nDirection = 0;
	m_bMergeNode = TRUE;
	m_bCopyAttr = FALSE;
	m_bIntersect = FALSE;
	m_nDivideType = 0;
	//}}AFX_DATA_INIT
	m_aLineCtrl.Add(IDC_TM_LINE_PT1_TITLE);
	m_aLineCtrl.Add(IDC_TM_LINE_PT1_EDIT);
	m_aLineCtrl.Add(IDC_TM_LINE_PT1_UNIT);
	m_aLineCtrl.Add(IDC_TM_LINE_PT2_TITLE);
	m_aLineCtrl.Add(IDC_TM_LINE_PT2_EDIT);
	m_aLineCtrl.Add(IDC_TM_LINE_PT2_UNIT);

	m_aPlaneCtrl.Add(IDC_TM_PLANE_PT1_TITLE);
	m_aPlaneCtrl.Add(IDC_TM_PLANE_PT1_EDIT);
	m_aPlaneCtrl.Add(IDC_TM_PLANE_PT1_UNIT);
	m_aPlaneCtrl.Add(IDC_TM_PLANE_PT2_TITLE);
	m_aPlaneCtrl.Add(IDC_TM_PLANE_PT2_EDIT);
	m_aPlaneCtrl.Add(IDC_TM_PLANE_PT2_UNIT);
	m_aPlaneCtrl.Add(IDC_TM_PLANE_PT3_TITLE);
	m_aPlaneCtrl.Add(IDC_TM_PLANE_PT3_EDIT);
	m_aPlaneCtrl.Add(IDC_TM_PLANE_PT3_UNIT);

	m_aCylinderCtrl.Add(IDC_TM_CYL_PT1_TITLE);
	m_aCylinderCtrl.Add(IDC_TM_CYL_PT1_EDIT);
	m_aCylinderCtrl.Add(IDC_TM_CYL_PT1_UNIT);
	m_aCylinderCtrl.Add(IDC_TM_CYL_PT2_TITLE);
	m_aCylinderCtrl.Add(IDC_TM_CYL_PT2_EDIT);
	m_aCylinderCtrl.Add(IDC_TM_CYL_PT2_UNIT);
	m_aCylinderCtrl.Add(IDC_TM_CYL_RAD_TITLE);
	m_aCylinderCtrl.Add(IDC_TM_CYL_RAD_EDIT);
	m_aCylinderCtrl.Add(IDC_TM_CYL_RAD_UNIT);

	m_aConeCtrl.Add(IDC_TM_CONE_PT1_TITLE);
	m_aConeCtrl.Add(IDC_TM_CONE_PT1_EDIT);
	m_aConeCtrl.Add(IDC_TM_CONE_PT1_UNIT);
	m_aConeCtrl.Add(IDC_TM_CONE_RAD1_TITLE);
	m_aConeCtrl.Add(IDC_TM_CONE_RAD1_EDIT);
	m_aConeCtrl.Add(IDC_TM_CONE_RAD1_UNIT);
	m_aConeCtrl.Add(IDC_TM_CONE_PT2_TITLE);
	m_aConeCtrl.Add(IDC_TM_CONE_PT2_EDIT);
	m_aConeCtrl.Add(IDC_TM_CONE_PT2_UNIT);
	m_aConeCtrl.Add(IDC_TM_CONE_RAD2_TITLE);
	m_aConeCtrl.Add(IDC_TM_CONE_RAD2_EDIT);
	m_aConeCtrl.Add(IDC_TM_CONE_RAD2_UNIT);

	m_aSphereCtrl.Add(IDC_TM_SPHR_ORG_TITLE);
	m_aSphereCtrl.Add(IDC_TM_SPHR_ORG_EDIT);
	m_aSphereCtrl.Add(IDC_TM_SPHR_ORG_UNIT);
	m_aSphereCtrl.Add(IDC_TM_SPHR_RAD_TITLE);
	m_aSphereCtrl.Add(IDC_TM_SPHR_RAD_EDIT);
	m_aSphereCtrl.Add(IDC_TM_SPHR_RAD_UNIT);

	m_aEllipsoidCtrl.Add(IDC_TM_ELIP_ORG_TITLE);
	m_aEllipsoidCtrl.Add(IDC_TM_ELIP_ORG_EDIT);
	m_aEllipsoidCtrl.Add(IDC_TM_ELIP_ORG_UNIT);
	m_aEllipsoidCtrl.Add(IDC_TM_ELIP_RAD_TITLE);
	m_aEllipsoidCtrl.Add(IDC_TM_ELIP_RAD_EDIT);
	m_aEllipsoidCtrl.Add(IDC_TM_ELIP_RAD_UNIT);
	m_aEllipsoidCtrl.Add(IDC_TM_ELIP_PT1_TITLE);
	m_aEllipsoidCtrl.Add(IDC_TM_ELIP_PT1_EDIT);
	m_aEllipsoidCtrl.Add(IDC_TM_ELIP_PT1_UNIT);

	m_aParaPlnCtrl.Add(IDC_TM_PARA_PLN_PT1_TITLE);
	m_aParaPlnCtrl.Add(IDC_TM_PARA_PLN_PT1_EDIT);
	m_aParaPlnCtrl.Add(IDC_TM_PARA_PLN_PT1_UNIT);
	m_aParaPlnCtrl.Add(IDC_TM_PARA_PLN_PT2_TITLE);
	m_aParaPlnCtrl.Add(IDC_TM_PARA_PLN_PT2_EDIT);
	m_aParaPlnCtrl.Add(IDC_TM_PARA_PLN_PT2_UNIT);
	m_aParaPlnCtrl.Add(IDC_TM_PARA_PLN_PT3_TITLE);
	m_aParaPlnCtrl.Add(IDC_TM_PARA_PLN_PT3_EDIT);
	m_aParaPlnCtrl.Add(IDC_TM_PARA_PLN_PT3_UNIT);

	m_aElementCtrl.Add(IDC_TM_ELEM_TITLE);
	m_aElementCtrl.Add(IDC_TM_ELEM_EDIT);

	m_aDirPt2Ctrl.Add(IDC_TM_DIR_PT1_EDIT);
	m_aDirPt2Ctrl.Add(IDC_TM_DIR_PT1_UNIT);

	m_aDisableOnMoveCtrl.Add(IDC_TM_MERGE_DUP_NODE_CHECK);
	m_aDisableOnMoveCtrl.Add(IDC_TM_TOLERANCE);
	m_aDisableOnMoveCtrl.Add(IDC_TM_COPY_NODEATTR_CHECK);
	m_aDisableOnMoveCtrl.Add(IDC_TM_COPYNODEDATA);

	// Divide Control
	m_aDivideCtrl.Add(IDC_TM_RDO_EQUAL);
	m_aDivideCtrl.Add(IDC_TM_RDO_UNEQUAL);
	m_aDivideCtrl.Add(IDC_TM_STATIC_NUM_DIVISION);
	m_aDivideCtrl.Add(IDC_TM_EDT_NUM_DIVISION);
	m_aDivideCtrl.Add(IDC_TM_SPIN_NUM_DIVISION);
	m_aDivideCtrl.Add(IDC_TM_STATIC_RATIO_DIVISION);
	m_aDivideCtrl.Add(IDC_TM_EDT_RATIO_DIVISION);
	
	m_aDivideECtrl.Add(IDC_TM_STATIC_NUM_DIVISION);
	m_aDivideECtrl.Add(IDC_TM_EDT_NUM_DIVISION);
	m_aDivideECtrl.Add(IDC_TM_SPIN_NUM_DIVISION);

	m_aDivideUECtrl.Add(IDC_TM_STATIC_RATIO_DIVISION);
	m_aDivideUECtrl.Add(IDC_TM_EDT_RATIO_DIVISION);
}

CNodeProjectDlg::~CNodeProjectDlg()
{
	if (m_pBitmap) delete m_pBitmap;
}

void CNodeProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNodeProjectDlg)
	DDX_Control(pDX, IDC_TM_CHK_DIVIDE, m_chkDivide);
	DDX_Control(pDX, IDC_TM_EDT_RATIO_DIVISION, m_edtRatioDivide);
	DDX_Control(pDX, IDC_TM_EDT_NUM_DIVISION, m_edtNumDivide);
	DDX_Control(pDX, IDC_TM_DIR_PT1_UNIT, m_wndDirPt1Unit);
	DDX_Control(pDX, IDC_TM_DIR_PT1_EDIT, m_wndDirPt1);
	DDX_Control(pDX, IDC_TM_ELEM_EDIT, m_wndElemNo);
	DDX_Control(pDX, IDC_TM_PARA_PLN_PT3_UNIT, m_wndParaPlnPt3Unit);
	DDX_Control(pDX, IDC_TM_PARA_PLN_PT3_EDIT, m_wndParaPlnPt3);
	DDX_Control(pDX, IDC_TM_PARA_PLN_PT2_UNIT, m_wndParaPlnPt2Unit);
	DDX_Control(pDX, IDC_TM_PARA_PLN_PT2_EDIT, m_wndParaPlnPt2);
	DDX_Control(pDX, IDC_TM_PARA_PLN_PT1_UNIT, m_wndParaPlnPt1Unit);
	DDX_Control(pDX, IDC_TM_PARA_PLN_PT1_EDIT, m_wndParaPlnPt1);
	DDX_Control(pDX, IDC_TM_ELIP_PT1_UNIT, m_wndElipPt1Unit);
	DDX_Control(pDX, IDC_TM_ELIP_PT1_EDIT, m_wndElipPt1);
	DDX_Control(pDX, IDC_TM_ELIP_RAD_UNIT, m_wndElipRadUnit);
	DDX_Control(pDX, IDC_TM_ELIP_RAD_EDIT, m_wndElipRad);
	DDX_Control(pDX, IDC_TM_ELIP_ORG_UNIT, m_wndElipOrgUnit);
	DDX_Control(pDX, IDC_TM_ELIP_ORG_EDIT, m_wndElipOrg);
	DDX_Control(pDX, IDC_TM_SPHR_RAD_UNIT, m_wndSphrRadUnit);
	DDX_Control(pDX, IDC_TM_SPHR_RAD_EDIT, m_wndSphrRad);
	DDX_Control(pDX, IDC_TM_SPHR_ORG_UNIT, m_wndSphrOrgUnit);
	DDX_Control(pDX, IDC_TM_SPHR_ORG_EDIT, m_wndSphrOrg);
	DDX_Control(pDX, IDC_TM_CONE_RAD2_UNIT, m_wndConeRad2Unit);
	DDX_Control(pDX, IDC_TM_CONE_RAD2_EDIT, m_wndConeRad2);
	DDX_Control(pDX, IDC_TM_CONE_RAD1_UNIT, m_wndConeRad1Unit);
	DDX_Control(pDX, IDC_TM_CONE_RAD1_EDIT, m_wndConeRad1);
	DDX_Control(pDX, IDC_TM_CONE_PT2_UNIT, m_wndConePt2Unit);
	DDX_Control(pDX, IDC_TM_CONE_PT2_EDIT, m_wndConePt2);
	DDX_Control(pDX, IDC_TM_CONE_PT1_UNIT, m_wndConePt1Unit);
	DDX_Control(pDX, IDC_TM_CONE_PT1_EDIT, m_wndConePt1);
	DDX_Control(pDX, IDC_TM_CYL_RAD_UNIT, m_wndCylRadUnit);
	DDX_Control(pDX, IDC_TM_CYL_RAD_EDIT, m_wndCylRad);
	DDX_Control(pDX, IDC_TM_CYL_PT2_UNIT, m_wndCylPt2Unit);
	DDX_Control(pDX, IDC_TM_CYL_PT2_EDIT, m_wndCylPt2);
	DDX_Control(pDX, IDC_TM_CYL_PT1_UNIT, m_wndCylPt1Unit);
	DDX_Control(pDX, IDC_TM_CYL_PT1_EDIT, m_wndCylPt1);
	DDX_Control(pDX, IDC_TM_PLANE_PT3_UNIT, m_wndPlanePt3Unit);
	DDX_Control(pDX, IDC_TM_PLANE_PT3_EDIT, m_wndPlanePt3);
	DDX_Control(pDX, IDC_TM_PLANE_PT2_UNIT, m_wndPlanePt2Unit);
	DDX_Control(pDX, IDC_TM_PLANE_PT2_EDIT, m_wndPlanePt2);
	DDX_Control(pDX, IDC_TM_PLANE_PT1_UNIT, m_wndPlanePt1Unit);
	DDX_Control(pDX, IDC_TM_PLANE_PT1_EDIT, m_wndPlanePt1);
	DDX_Control(pDX, IDC_TM_LINE_PT2_UNIT, m_wndLinePt2Unit);
	DDX_Control(pDX, IDC_TM_LINE_PT2_EDIT, m_wndLinePt2);
	DDX_Control(pDX, IDC_TM_LINE_PT1_UNIT, m_wndLinePt1Unit);
	DDX_Control(pDX, IDC_TM_LINE_PT1_EDIT, m_wndLinePt1);
	DDX_Control(pDX, IDC_TM_PT_COMBO, m_wndProjectType);
	DDX_Radio(pDX, IDC_TM_COPY_RADIO, m_nCopyOrMove);
	DDX_Radio(pDX, IDC_TM_DIR_NORMAL, m_nDirection);
	DDX_Check(pDX, IDC_TM_MERGE_DUP_NODE_CHECK, m_bMergeNode);
	DDX_Check(pDX, IDC_TM_COPY_NODEATTR_CHECK, m_bCopyAttr);
	DDX_Check(pDX, IDC_TM_INTERSECT_CHECK, m_bIntersect);
	DDX_Radio(pDX, IDC_TM_RDO_EQUAL, m_nDivideType);
	DDX_Control(pDX, IDC_TM_PICTURE, m_wndPicture);
	//}}AFX_DATA_MAP
}

void CNodeProjectDlg::ChangeBitmap(int nBitmap)
{
	CString aBitmapPath[] = {
		_T("SVG\\Illustration\\Dialog\\tm_nprj1.svg"),
		_T("SVG\\Illustration\\Dialog\\tm_nprj2.svg"),
		_T("SVG\\Illustration\\Dialog\\tm_nprj3.svg"),
		_T("SVG\\Illustration\\Dialog\\tm_nprj4.svg"),
		_T("SVG\\Illustration\\Dialog\\tm_nprj5.svg"),
		_T("SVG\\Illustration\\Dialog\\tm_nprj6.svg"),
		_T("SVG\\Illustration\\Dialog\\tm_nprj8.svg"),
		_T("SVG\\Illustration\\Dialog\\tm_nprj7.svg")
	};

	m_wndPicture.SetImage(aBitmapPath[nBitmap - 1]);
}

void CNodeProjectDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;

	UINT aFirstCtrlID[] = {
		IDC_TM_PLANE_PT1_TITLE, IDC_TM_CYL_PT1_TITLE, IDC_TM_CONE_PT1_TITLE,
		IDC_TM_SPHR_ORG_TITLE, IDC_TM_ELIP_ORG_TITLE, IDC_TM_PARA_PLN_PT1_TITLE,
		IDC_TM_ELEM_TITLE
	};
	CArray<UINT, UINT> *aCtrlSet[] = { 
		&m_aPlaneCtrl, &m_aCylinderCtrl, &m_aConeCtrl,
		&m_aSphereCtrl, &m_aEllipsoidCtrl, &m_aParaPlnCtrl,
		&m_aElementCtrl,
	};

	GetDlgItem(IDC_TM_LINE_PT1_TITLE)->GetWindowRect(rRef);
	for (int i = 0; i < sizeof(aFirstCtrlID)/sizeof(UINT); i++)
	{
		GetDlgItem(aFirstCtrlID[i])->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CtrlMoveDistY(*aCtrlSet[i], nDistY);
	}

	// Divide
	GetDlgItem(m_aDivideECtrl.GetAt(0))->GetWindowRect(rRef);
	GetDlgItem(m_aDivideUECtrl.GetAt(0))->GetWindowRect(rToMove);
	CtrlMoveDistY(m_aDivideUECtrl, rRef.top-rToMove.top);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_TM_BTN_INTERSECT);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(5);
	CWnd* pParent = GetParent();
	pParent->ScreenToClient(r);
	MoveWindow(r);
}

void CNodeProjectDlg::ChangeProjectionType(int nProjType)
{
	CtrlShowHide(m_aLineCtrl, nProjType == 0);
	CtrlShowHide(m_aPlaneCtrl, nProjType == 1);
	CtrlShowHide(m_aCylinderCtrl, nProjType == 2);
	CtrlShowHide(m_aConeCtrl, nProjType == 3);
	CtrlShowHide(m_aSphereCtrl, nProjType == 4);
	CtrlShowHide(m_aEllipsoidCtrl, nProjType == 5);
	CtrlShowHide(m_aParaPlnCtrl, nProjType == 6);
	CtrlShowHide(m_aElementCtrl, nProjType == 7);
	ChangeBitmap(nProjType+1);

	CString aTitle[] = {
		_LS(IDS_WG_TREEMENU_Base_Line_Definition), _LS(IDS_WG_TREEMENU_Base_Plane_Definition),
		_LS(IDS_WG_TREEMENU_Base_Cylinder_Definition), _LS(IDS_WG_TREEMENU_Base_Cone_Definition),
		_LS(IDS_WG_TREEMENU_Base_Sphere_Definition), _LS(IDS_WG_TREEMENU_Base_Ellipsoid_Definition),
		_LS(IDS_WG_TREEMENU_Base_Parabolic_Plane_Definition),
		_LS(IDS_WG_TREEMENU_Base_Element)
	};
	CBCGPStatic* pFrame = (CBCGPStatic*)GetDlgItem(IDC_TM_FRAME);
	pFrame->SetWindowText(aTitle[nProjType]);
}

BOOL CNodeProjectDlg::Get3Point(CMouseEdit& pt, double &Px, double &Py, double &Pz)
{
	if(pt.IsKeyPressed())
	{
		CString strPos;
		pt.GetWindowText(strPos);
		return GetPosition(strPos,Px,Py,Pz);
	}
	else
	{
		double wx, wy, wz;
		pt.GetCurMousePosReal(wx,wy,wz,Px,Py,Pz);
	}
	return TRUE;
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CNodeProjectDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CNodeProjectDlg)
	ON_CBN_SELCHANGE(IDC_TM_PT_COMBO, OnChangeProjectType)
	ON_BN_CLICKED(IDC_TM_DIR_NORMAL, OnChangeDirection)
	ON_BN_CLICKED(IDC_TM_COPY_RADIO, OnChangeCopyMove)
	ON_BN_CLICKED(IDC_TM_TOLERANCE, OnTmTolerance)
	ON_BN_CLICKED(IDC_TM_COPYNODEDATA, OnTmCopynodedata)
	ON_BN_CLICKED(IDC_TM_RDO_EQUAL, OnChangeDivideType)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TM_SPIN_NUM_DIVISION, OnDeltaposTmSpinNumDivision)
	ON_BN_CLICKED(IDC_TM_DIR_2PT_AXIS, OnChangeDirection)
	ON_BN_CLICKED(IDC_TM_MOVE_RADIO, OnChangeCopyMove)
	ON_BN_CLICKED(IDC_TM_RDO_UNEQUAL, OnChangeDivideType)
	ON_BN_CLICKED(IDC_TM_CHK_DIVIDE, OnTmChkDivide)
	ON_BN_CLICKED(IDC_TM_BTN_INTERSECT, OnTmBtnIntersect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNodeProjectDlg message handlers

BOOL CNodeProjectDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	AlignControl();

	m_wndProjectType.AddString(_LS(IDS_WG_TREEMENU_Project_nodes_on_a_line));
	m_wndProjectType.AddString(_LS(IDS_WG_TREEMENU_Project_nodes_on_a_plane));
	m_wndProjectType.AddString(_LS(IDS_WG_TREEMENU_Project_nodes_on_a_cylinder));
	m_wndProjectType.AddString(_LS(IDS_WG_TREEMENU_Project_nodes_on_a_cone));
	m_wndProjectType.AddString(_LS(IDS_WG_TREEMENU_Project_nodes_on_a_sphere));
	m_wndProjectType.AddString(_LS(IDS_WG_TREEMENU_Project_nodes_on_an_ellipsoid));
	m_wndProjectType.AddString(_LS(IDS_WG_TREEMENU_Project_nodes_on_a_parabolic_plane));
	m_wndProjectType.AddString(_LS(IDS_WG_TREEMENU_Project_nodes_on_an_element));

	// line
	m_wndLinePt1.SetAttUcsPos();
	m_wndLinePt1.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndLinePt2.SetAttUcsPos();
	m_wndLinePt2.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndLinePt1Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndLinePt2Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndLinePt1.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wndLinePt2.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wndLinePt1.SetNextLink(&m_wndLinePt2);
	m_wndLinePt2.SetNextLink(NULL);
	// plane
	m_wndPlanePt1.SetAttUcsPos();
	m_wndPlanePt1.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndPlanePt2.SetAttUcsPos();
	m_wndPlanePt2.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndPlanePt3.SetAttUcsPos();
	m_wndPlanePt3.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndPlanePt1Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndPlanePt2Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndPlanePt3Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndPlanePt1.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wndPlanePt2.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wndPlanePt3.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wndPlanePt1.SetNextLink(&m_wndPlanePt2);
	m_wndPlanePt2.SetNextLink(&m_wndPlanePt3);
	m_wndPlanePt3.SetNextLink(NULL);
	// cylinder
	m_wndCylPt1.SetAttUcsPos();
	m_wndCylPt1.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndCylPt2.SetAttUcsPos();
	m_wndCylPt2.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndCylRad.SetAttUcsDistance();
	m_wndCylRad.SetModeToUse(MOUSEEDIT_USE_NODE_SET_DIST);
	m_wndCylRad.SetDistValueMode(TRUE);
	m_wndCylPt1Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndCylPt1Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndCylRadUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndCylPt1.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wndCylPt2.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wndCylRad.SetWindowText(_T("0"));
	m_wndCylPt1.SetNextLink(&m_wndCylPt2);
	m_wndCylPt2.SetNextLink(&m_wndCylRad);
	m_wndCylRad.SetNextLink(NULL);
	// cone
	m_wndConePt1.SetAttUcsPos();
	m_wndConePt1.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndConePt2.SetAttUcsPos();
	m_wndConePt2.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndConeRad1.SetAttUcsDistance();
	m_wndConeRad1.SetModeToUse(MOUSEEDIT_USE_NODE_SET_DIST);
	m_wndConeRad1.SetDistValueMode(TRUE);
	m_wndConeRad2.SetAttUcsDistance();
	m_wndConeRad2.SetModeToUse(MOUSEEDIT_USE_NODE_SET_DIST);
	m_wndConeRad2.SetDistValueMode(TRUE);
	m_wndConePt1Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndConePt2Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndConeRad1Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndConeRad2Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndConePt1.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wndConePt2.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wndConeRad1.SetWindowText(_T("0"));
	m_wndConeRad2.SetWindowText(_T("0"));
	m_wndConePt1.SetNextLink(&m_wndConeRad1);
	m_wndConePt2.SetNextLink(&m_wndConeRad2);
	m_wndConeRad1.SetNextLink(&m_wndConePt2);
	m_wndConeRad2.SetNextLink(NULL);
	// sphere
	m_wndSphrOrg.SetAttUcsPos();
	m_wndSphrOrg.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndSphrRad.SetAttUcsDistance();
	m_wndSphrRad.SetModeToUse(MOUSEEDIT_USE_NODE_SET_DIST);
	m_wndSphrRad.SetDistValueMode(TRUE);
	m_wndSphrOrgUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndSphrRadUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndSphrOrg.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wndSphrRad.SetWindowText(_T("0"));
	m_wndSphrOrg.SetNextLink(&m_wndSphrRad);
	m_wndSphrRad.SetNextLink(NULL);
	// ellipsoid
	m_wndElipOrg.SetAttUcsPos();
	m_wndElipOrg.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndElipRad.SetAttUcsDistance();
	m_wndElipRad.SetModeToUse(MOUSEEDIT_USE_NODE_SET_DIST);
	m_wndElipRad.SetDistValueMode(TRUE);
	m_wndElipPt1.SetAttUcsPos();
	m_wndElipPt1.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndElipOrgUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndElipRadUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndElipPt1Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndElipOrg.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wndElipRad.SetWindowText(_T("0"));
	m_wndElipPt1.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wndElipOrg.SetNextLink(&m_wndElipRad);
	m_wndElipRad.SetNextLink(&m_wndElipPt1);
	m_wndElipPt1.SetNextLink(NULL);
	// parabolic plane
	m_wndParaPlnPt1.SetAttUcsPos();
	m_wndParaPlnPt1.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndParaPlnPt2.SetAttUcsPos();
	m_wndParaPlnPt2.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndParaPlnPt3.SetAttUcsPos();
	m_wndParaPlnPt3.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndParaPlnPt1Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndParaPlnPt2Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndParaPlnPt3Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndParaPlnPt1.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wndParaPlnPt2.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wndParaPlnPt3.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wndParaPlnPt1.SetNextLink(&m_wndParaPlnPt2);
	m_wndParaPlnPt2.SetNextLink(&m_wndParaPlnPt3);
	m_wndParaPlnPt3.SetNextLink(NULL);
	// element
	m_wndElemNo.SetAttElemList();
	m_wndElemNo.SetLButtonDownNotifyWindow(this);
	m_wndElemNo.SetEnterNotifyWindow(this);
	m_wndElemNo.SetModeToUse(MOUSEEDIT_USE_GET_ELEM_LIST);
	m_wndElemNo.SetMaxElemKeyNum(1);

	// Direction
	m_wndDirPt1.SetAttUcsDistance();
	m_wndDirPt1.SetModeToUse(MOUSEEDIT_USE_NODE_SET_DIST);
	m_wndDirPt1Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDirPt1.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));

	// Divide
	m_edtNumDivide.SetRange(1, SHRT_MAX);
	m_edtNumDivide.SetValue(2);
	m_edtNumDivide.SetInteger(TRUE);
	m_edtRatioDivide.SetWindowText(_T(""));
	OnTmChkDivide();

	// Default
	m_wndProjectType.SetCurSel(0);

	int nProjType = m_wndProjectType.GetCurSel();
	ChangeProjectionType(nProjType);

	CtrlEnableDisable(m_aDirPt2Ctrl, m_nDirection == 1);
	CtrlEnableDisable(m_aDisableOnMoveCtrl, m_nCopyOrMove == 0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CNodeProjectDlg::GetParameter(_NodeProject& NP)
{
	if (!UpdateData(TRUE)) return FALSE;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	
	// Get Parameter
	NP.m_bCopyOrMove = (m_nCopyOrMove == 0) ? TRUE : FALSE;
	NP.m_nProjectionType = m_wndProjectType.GetCurSel()+1;

	// Divide
	if(m_chkDivide.GetCheck())
	{
		NP.m_nDivideType = m_nDivideType+1;

		CString strTemp;
		switch(NP.m_nDivideType)
		{
			case 1:  // Equal
				m_edtNumDivide.GetWindowText(strTemp);
				if(!GetINumber(strTemp, NP.m_nDivide)) return FALSE;
				--NP.m_nDivide;
				break;
			case 2:  // Parametric Unequal
				m_edtRatioDivide.GetWindowText(strTemp);
				if(!GetUEDistance(strTemp, NP.m_arDistance)) return FALSE;
				break;
		}	
	}
	else {NP.m_nDivideType = 0; NP.m_nDivide=1;	NP.m_arDistance.RemoveAll();}

	double ux1, uy1, uz1;
	CString csValue;
	switch(NP.m_nProjectionType)
	{
	case 1: // line
		if(!Get3Point(m_wndLinePt1, ux1, uy1, uz1)) return FALSE;
		pIGM->GetWcsPos(ux1, uy1, uz1,
										NP.m_nLinePt1WX, NP.m_nLinePt1WY, NP.m_nLinePt1WZ);
		if(!Get3Point(m_wndLinePt2, ux1, uy1, uz1)) return FALSE;
		pIGM->GetWcsPos(ux1, uy1, uz1,
										NP.m_nLinePt2WX, NP.m_nLinePt2WY, NP.m_nLinePt2WZ);
		break;
	case 2: // plane
		if(!Get3Point(m_wndPlanePt1, ux1, uy1, uz1)) return FALSE;
		pIGM->GetWcsPos(ux1, uy1, uz1,
										NP.m_nPlanePt1WX, NP.m_nPlanePt1WY, NP.m_nPlanePt1WZ);
		if(!Get3Point(m_wndPlanePt2, ux1, uy1, uz1)) return FALSE;
		pIGM->GetWcsPos(ux1, uy1, uz1,
										NP.m_nPlanePt2WX, NP.m_nPlanePt2WY, NP.m_nPlanePt2WZ);
		if(!Get3Point(m_wndPlanePt3, ux1, uy1, uz1)) return FALSE;
		pIGM->GetWcsPos(ux1, uy1, uz1,
										NP.m_nPlanePt3WX, NP.m_nPlanePt3WY, NP.m_nPlanePt3WZ);
		break;
	case 3: // cylinder
		if(!Get3Point(m_wndCylPt1, ux1, uy1, uz1)) return FALSE;
		pIGM->GetWcsPos(ux1, uy1, uz1,
										NP.m_nCylPt1WX, NP.m_nCylPt1WY, NP.m_nCylPt1WZ);
		if(!Get3Point(m_wndCylPt2, ux1, uy1, uz1)) return FALSE;
		pIGM->GetWcsPos(ux1, uy1, uz1,
										NP.m_nCylPt2WX, NP.m_nCylPt2WY, NP.m_nCylPt2WZ);
		m_wndCylRad.GetWindowText(csValue);
		if(!GetFloatNumber(csValue, NP.m_nCylRadius)) return FALSE;
		break;
	case 4: // cone
		if(!Get3Point(m_wndConePt1, ux1, uy1, uz1)) return FALSE;
		pIGM->GetWcsPos(ux1, uy1, uz1,
										NP.m_nConePt1WX, NP.m_nConePt1WY, NP.m_nConePt1WZ);
		if(!Get3Point(m_wndConePt2, ux1, uy1, uz1)) return FALSE;
		pIGM->GetWcsPos(ux1, uy1, uz1,
										NP.m_nConePt2WX, NP.m_nConePt2WY, NP.m_nConePt2WZ);
		m_wndConeRad1.GetWindowText(csValue);
		if(!GetFloatNumber(csValue, NP.m_nConeRadius1)) return FALSE;
		m_wndConeRad2.GetWindowText(csValue);
		if(!GetFloatNumber(csValue, NP.m_nConeRadius2)) return FALSE;
		break;
	case 5: // sphere
		if(!Get3Point(m_wndSphrOrg, ux1, uy1, uz1)) return FALSE;
		pIGM->GetWcsPos(ux1, uy1, uz1,
										NP.m_nSphrOrgWX, NP.m_nSphrOrgWY, NP.m_nSphrOrgWZ);
		m_wndSphrRad.GetWindowText(csValue);
		if(!GetFloatNumber(csValue, NP.m_nSphrRadius)) return FALSE;
		break;
	case 6: // ellipsoid
		if(!Get3Point(m_wndElipOrg, ux1, uy1, uz1)) return FALSE;
		pIGM->GetWcsPos(ux1, uy1, uz1,
										NP.m_nElipOrgWX, NP.m_nElipOrgWY, NP.m_nElipOrgWZ);
		if(!Get3Point(m_wndElipPt1, ux1, uy1, uz1)) return FALSE;
		pIGM->GetWcsPos(ux1, uy1, uz1,
										NP.m_nElipPt1WX, NP.m_nElipPt1WY, NP.m_nElipPt1WZ);
		m_wndElipRad.GetWindowText(csValue);
		if(!GetFloatNumber(csValue, NP.m_nElipRadius)) return FALSE;
		break;
	case 7: // parabolic plane
		if(!Get3Point(m_wndParaPlnPt1, ux1, uy1, uz1)) return FALSE;
		pIGM->GetWcsPos(ux1, uy1, uz1,
										NP.m_nParaPlnPt1WX, NP.m_nParaPlnPt1WY, NP.m_nParaPlnPt1WZ);
		if(!Get3Point(m_wndParaPlnPt2, ux1, uy1, uz1)) return FALSE;
		pIGM->GetWcsPos(ux1, uy1, uz1,
										NP.m_nParaPlnPt2WX, NP.m_nParaPlnPt2WY, NP.m_nParaPlnPt2WZ);
		if(!Get3Point(m_wndParaPlnPt3, ux1, uy1, uz1)) return FALSE;
		pIGM->GetWcsPos(ux1, uy1, uz1,
										NP.m_nParaPlnPt3WX, NP.m_nParaPlnPt3WY, NP.m_nParaPlnPt3WZ);
		break;
	case 8: // element
		{
			CArray<UINT, UINT> aElemKey;
			CString csElemKey;
			m_wndElemNo.GetWindowText(csElemKey);
			// GetNodeList는 Elem List 구할 때도 사용가능하다.
			if (!GetNodeList(csElemKey, aElemKey)) return FALSE;
			if (aElemKey.GetSize() != 1)
			{
				GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Select_1_element_and_try_ag));
				return FALSE;
			}
			NP.m_nElemKey = aElemKey.GetAt(0);
		}
		break;

	default : ASSERT(0);
	}

	NP.m_nDirection = m_nDirection+1;

	if (m_nDirection == 1)  // 2point
	{
		CString strDirVector;
		double wx, wy, wz;

		m_wndDirPt1.GetWindowText(strDirVector);
		if(TRUE == m_wndDirPt1.IsKeyPressed())
		{
			if(GetDirectionVector(strDirVector, ux1, uy1, uz1))
			{
				 pIGM->GetWcsVector(ux1, uy1, uz1, wx, wy, wz);
																														
			}
			else return FALSE;
		}
		else
		{
			m_wndDirPt1.GetCurMousePosDist(wx, wy, wz, ux1, uy1, uz1);
		}
		double VL = wx*wx + wy*wy + wz*wz;
		if(pIGM->IsZero(VL))
		{
			AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___Direction_vector_length_is_));
			return FALSE;
		}
		
		VL = sqrt(VL);
		NP.m_nDirPt1WX = wx/VL;
		NP.m_nDirPt1WY = wy/VL;
		NP.m_nDirPt1WZ = wz/VL;
	}

	pIGM->GetWcsVector(1.0, 0.0, 0.0, NP.v1[0], NP.v1[1], NP.v1[2]);
	pIGM->GetWcsVector(0.0, 1.0, 0.0, NP.v2[0], NP.v2[1], NP.v2[2]);
	pIGM->GetWcsVector(0.0, 0.0, 1.0, NP.v3[0], NP.v3[1], NP.v3[2]);

	NP.m_bMergeNode = m_bMergeNode;
	NP.m_bCopyAttr = m_bCopyAttr;
	NP.m_bIntersect = m_bIntersect;

	return TRUE;
}

BOOL CNodeProjectDlg::ProjectNode(_NodeProject& NP)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<unsigned int, unsigned int> aKey;

	if (!pIGM->GetSelectedNodeKeyList(aKey)) return FALSE;
	if (aKey.GetSize() < 1)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
		return FALSE;
	}

	double p1[3], p2[3], p3[3], r[3];
	switch(NP.m_nProjectionType)
	{
	case 1: 
		p1[0] = NP.m_nLinePt1WX; p1[1] = NP.m_nLinePt1WY; p1[2] = NP.m_nLinePt1WZ;
		p2[0] = NP.m_nLinePt2WX; p2[1] = NP.m_nLinePt2WY; p2[2] = NP.m_nLinePt2WZ;
		break;
	case 2: 
		p1[0] = NP.m_nPlanePt1WX; p1[1] = NP.m_nPlanePt1WY; p1[2] = NP.m_nPlanePt1WZ;
		p2[0] = NP.m_nPlanePt2WX; p2[1] = NP.m_nPlanePt2WY; p2[2] = NP.m_nPlanePt2WZ;
		p3[0] = NP.m_nPlanePt3WX; p3[1] = NP.m_nPlanePt3WY; p3[2] = NP.m_nPlanePt3WZ;
		break;
	case 3:
		p1[0] = NP.m_nCylPt1WX; p1[1] = NP.m_nCylPt1WY; p1[2] = NP.m_nCylPt1WZ;
		p2[0] = NP.m_nCylPt2WX; p2[1] = NP.m_nCylPt2WY; p2[2] = NP.m_nCylPt2WZ;
		r[0] = NP.m_nCylRadius;
		break;
	case 4:
		p1[0] = NP.m_nConePt1WX; p1[1] = NP.m_nConePt1WY; p1[2] = NP.m_nConePt1WZ;
		p2[0] = NP.m_nConePt2WX; p2[1] = NP.m_nConePt2WY; p2[2] = NP.m_nConePt2WZ;
		r[0] = NP.m_nConeRadius1; r[1] = NP.m_nConeRadius2;
		break;
	case 5:
		p1[0] = NP.m_nSphrOrgWX; p1[1] = NP.m_nSphrOrgWY; p1[2] = NP.m_nSphrOrgWZ;
		r[0] = NP.m_nSphrRadius;
		break;
	case 6:
		p1[0] = NP.m_nElipOrgWX; p1[1] = NP.m_nElipOrgWY; p1[2] = NP.m_nElipOrgWZ;
		p2[0] = NP.m_nElipPt1WX; p2[1] = NP.m_nElipPt1WY; p2[2] = NP.m_nElipPt1WZ;
		r[0] = NP.m_nElipRadius;
		break;
	case 7:
		p1[0] = NP.m_nParaPlnPt1WX; p1[1] = NP.m_nParaPlnPt1WY; p1[2] = NP.m_nParaPlnPt1WZ;
		p2[0] = NP.m_nParaPlnPt2WX; p2[1] = NP.m_nParaPlnPt2WY; p2[2] = NP.m_nParaPlnPt2WZ;
		p3[0] = NP.m_nParaPlnPt3WX; p3[1] = NP.m_nParaPlnPt3WY; p3[2] = NP.m_nParaPlnPt3WZ;
		break;
	case 8:
		break;
	}
	BOOL bSuccess = m_pDoc->m_pDataCtrl->ProjectNode(aKey, NP.m_bCopyOrMove, 
				NP.m_nProjectionType, NP.m_nDirection, 
				NP.m_nDirPt1WX, NP.m_nDirPt1WY, NP.m_nDirPt1WZ,
				p1, p2, p3, r, NP.m_nElemKey, NP.m_bMergeNode, NP.m_bCopyAttr, NP.m_bIntersect,
				NP.m_nDivideType, NP.m_nDivide, NP.m_arDistance, NP.v1, NP.v2, NP.v3);

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	return TRUE;
}

void CNodeProjectDlg::Execute()
{
	_NodeProject NP;
	if(GetParameter(NP))
	{
		ProjectNode(NP);
	}
	else
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Parameter_Error____));
}

void CNodeProjectDlg::OnChangeProjectType() 
{
	// TODO: Add your control notification handler code here
	int nProjType = m_wndProjectType.GetCurSel();
	ChangeProjectionType(nProjType);
}

void CNodeProjectDlg::OnChangeDirection() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	switch(nID)
	{
	case IDC_TM_DIR_NORMAL: m_nDirection = 0; break;
	case IDC_TM_DIR_2PT_AXIS: m_nDirection = 1; break;
	}
	CtrlEnableDisable(m_aDirPt2Ctrl, m_nDirection == 1);
	if (m_nDirection == 1) m_wndDirPt1.SetFocus();
}

void CNodeProjectDlg::OnChangeCopyMove() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	UINT nID = LOWORD(pMsg->wParam);
	switch(nID)
	{
	case IDC_TM_COPY_RADIO: m_nCopyOrMove = 0; break;
	case IDC_TM_MOVE_RADIO: m_nCopyOrMove = 1; break;
	}
	CtrlEnableDisable(m_aDisableOnMoveCtrl, m_nCopyOrMove == 0);	
	CtrlEnableDisable(m_aDivideCtrl, m_nCopyOrMove == 0);
	((CButton*)GetDlgItem(IDC_TM_CHK_DIVIDE))->EnableWindow(m_nCopyOrMove == 0);
	if(m_nCopyOrMove == 0) OnTmChkDivide();
}

void CNodeProjectDlg::OnTmCopynodedata() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_COPYATTR_NODE,0));	
}

void CNodeProjectDlg::OnTmTolerance() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_MERGETOL,0));	
}

void CNodeProjectDlg::OnChangeDivideType() 
{
	UpdateData(TRUE);

	CtrlShowHide(m_aDivideECtrl, m_nDivideType==0);
	CtrlShowHide(m_aDivideUECtrl, m_nDivideType==1);
}

void CNodeProjectDlg::OnDeltaposTmSpinNumDivision(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString strTemp;
	m_edtNumDivide.GetWindowText(strTemp);
	
	if(pNMUpDown->iDelta < 0)
	{
		strTemp.Format(_LS(IDS_WG_TREEMENU__d),_ttoi(strTemp) + 1);
		m_edtNumDivide.SetWindowText(strTemp);		
	}
	else
		if(_tstof(strTemp) > 2)
		{
			strTemp.Format(_LS(IDS_WG_TREEMENU__d),_ttoi(strTemp) - 1);
			m_edtNumDivide.SetWindowText(strTemp);		
		}

	*pResult = 0;
}

void CNodeProjectDlg::OnTmChkDivide() 
{
	CtrlEnableDisable(m_aDivideCtrl, m_chkDivide.GetCheck() == 1);	
	OnChangeDivideType();
}

void CNodeProjectDlg::OnTmBtnIntersect()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_INTERSECTTOL,0));
}

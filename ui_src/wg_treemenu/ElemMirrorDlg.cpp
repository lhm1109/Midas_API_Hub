// ElemMirrorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ElemMirrorDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_dbRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElemMirrorDlg dialog


CElemMirrorDlg::CElemMirrorDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CElemMirrorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElemMirrorDlg)
	m_nCopyOrMove = 0;
	m_nWhatPlane =  0;
	m_bIsCopyElemAttr = FALSE;
	m_bIsCopyNodeAttr = FALSE;
	m_bIntstNode = FALSE;
	m_bIntstElem = FALSE;
	m_bBetaAngle = FALSE;
	m_bReverseElem = FALSE;
	//}}AFX_DATA_INIT

	m_MoveDisable.Add(IDC_TM_COPY_NODEATTR_CHECK);
	m_MoveDisable.Add(IDC_TM_COPY_ELEMATTR_CHECK);
	m_MoveDisable.Add(IDC_TM_COPYNODEDATA);
#if !defined(_TOWER)
	m_MoveDisable.Add(IDC_TM_COPYELEMDATA);
#endif
	m_MoveDisable.Add(IDC_TM_MATL_INC_TITLE);
	m_MoveDisable.Add(IDC_TM_MATL_INC_EDIT);
	//m_MoveDisable.Add(IDC_TM_MATL_INC_SPIN);
	m_MoveDisable.Add(IDC_TM_MATL_INC_REP);
	m_MoveDisable.Add(IDC_TM_SECT_INC_TITLE);
	m_MoveDisable.Add(IDC_TM_SECT_INC_EDIT);
	//m_MoveDisable.Add(IDC_TM_SECT_INC_SPIN);
	m_MoveDisable.Add(IDC_TM_SECT_INC_REP);
	m_MoveDisable.Add(IDC_TM_THIK_INC_TITLE);
	m_MoveDisable.Add(IDC_TM_THIK_INC_EDIT);
	//m_MoveDisable.Add(IDC_TM_THIK_INC_SPIN);
	m_MoveDisable.Add(IDC_TM_THIK_INC_REP);
	m_MoveDisable.Add(IDC_TM_INTST_TITLE);
	m_MoveDisable.Add(IDC_TM_INTST_NODE);
	m_MoveDisable.Add(IDC_TM_INTST_ELEM);
	
	m_aThik.Add(IDC_TM_THIK_INC_TITLE);
	m_aThik.Add(IDC_TM_THIK_INC_EDIT);
	//m_aThik.Add(IDC_TM_THIK_INC_SPIN);

	m_PlaneDisable.Add(IDC_TM_XCOORD_EDIT);
	m_PlaneDisable.Add(IDC_TM_ZCOORD_EDIT);
	m_PlaneDisable.Add(IDC_TM_YCOORD_EDIT);
	m_PlaneDisable.Add(IDC_TM_1ST_STATIC);
	m_PlaneDisable.Add(IDC_TM_1ST_EDIT);
	m_PlaneDisable.Add(IDC_TM_2ND_STATIC);
	m_PlaneDisable.Add(IDC_TM_2ND_EDIT);
	m_PlaneDisable.Add(IDC_TM_3RD_STATIC);
	m_PlaneDisable.Add(IDC_TM_3RD_EDIT);
	m_PlaneDisable.Add(IDC_TM_XCOORD_UNIT);
	m_PlaneDisable.Add(IDC_TM_YCOORD_UNIT);
	m_PlaneDisable.Add(IDC_TM_ZCOORD_UNIT);
	m_PlaneDisable.Add(IDC_TM_P1COORD_UNIT);
	m_PlaneDisable.Add(IDC_TM_P2COORD_UNIT);
	m_PlaneDisable.Add(IDC_TM_P3COORD_UNIT);

	m_PlaneYZEnable.Add(IDC_TM_XCOORD_EDIT);
	m_PlaneYZEnable.Add(IDC_TM_XCOORD_UNIT);

	m_PlaneXYEnable.Add(IDC_TM_ZCOORD_EDIT);
	m_PlaneXYEnable.Add(IDC_TM_ZCOORD_UNIT);

	m_PlaneXZEnable.Add(IDC_TM_YCOORD_EDIT);
	m_PlaneXZEnable.Add(IDC_TM_YCOORD_UNIT);

	m_Plane3PEnable.Add(IDC_TM_1ST_STATIC);
	m_Plane3PEnable.Add(IDC_TM_1ST_EDIT);
	m_Plane3PEnable.Add(IDC_TM_2ND_STATIC);
	m_Plane3PEnable.Add(IDC_TM_2ND_EDIT);
	m_Plane3PEnable.Add(IDC_TM_3RD_STATIC);
	m_Plane3PEnable.Add(IDC_TM_3RD_EDIT);
	m_Plane3PEnable.Add(IDC_TM_P1COORD_UNIT);
	m_Plane3PEnable.Add(IDC_TM_P2COORD_UNIT);
	m_Plane3PEnable.Add(IDC_TM_P3COORD_UNIT);
}


void CElemMirrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemMirrorDlg)
	DDX_Control(pDX, IDC_TM_THIK_INC_EDIT, m_wndThikInc);
	DDX_Control(pDX, IDC_TM_SECT_INC_EDIT, m_wndSectInc);
	DDX_Control(pDX, IDC_TM_MATL_INC_EDIT, m_wndMatlInc);
	DDX_Control(pDX, IDC_TM_ZCOORD_EDIT, m_wndZCoord);
	DDX_Control(pDX, IDC_TM_YCOORD_EDIT, m_wndYCoord);
	DDX_Control(pDX, IDC_TM_XCOORD_EDIT, m_wndXCoord);
	DDX_Control(pDX, IDC_TM_3RD_EDIT, m_wnd3rdPoint);
	DDX_Control(pDX, IDC_TM_2ND_EDIT, m_wnd2ndPoint);
	DDX_Control(pDX, IDC_TM_1ST_EDIT, m_wnd1stPoint);
	DDX_Control(pDX, IDC_TM_XCOORD_UNIT, m_wndXCoordUnit);
	DDX_Control(pDX, IDC_TM_YCOORD_UNIT, m_wndYCoordUnit);
	DDX_Control(pDX, IDC_TM_ZCOORD_UNIT, m_wndZCoordUnit);
	DDX_Control(pDX, IDC_TM_P1COORD_UNIT, m_wndP1CoordUnit);
	DDX_Control(pDX, IDC_TM_P2COORD_UNIT, m_wndP2CoordUnit);
	DDX_Control(pDX, IDC_TM_P3COORD_UNIT, m_wndP3CoordUnit);
	DDX_Radio(pDX, IDC_TM_COPY_RADIO, m_nCopyOrMove);
	DDX_Radio(pDX, IDC_TM_YZ_RADIO  , m_nWhatPlane);
	DDX_Check(pDX, IDC_TM_COPY_ELEMATTR_CHECK, m_bIsCopyElemAttr);
	DDX_Check(pDX, IDC_TM_COPY_NODEATTR_CHECK, m_bIsCopyNodeAttr);
	DDX_Check(pDX, IDC_TM_INTST_NODE, m_bIntstNode);
	DDX_Check(pDX, IDC_TM_INTST_ELEM, m_bIntstElem);
	DDX_Check(pDX, IDC_TM_BETA_ANGLE_CHECK, m_bBetaAngle);
	DDX_Check(pDX, IDC_TM_REVERSE_ELEM_CHECK, m_bReverseElem);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CElemMirrorDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CElemMirrorDlg)
	ON_BN_CLICKED(IDC_TM_COPY_RADIO, OnTmCopyMoveRadio)
	ON_BN_CLICKED(IDC_TM_XY_RADIO, OnTmPlaneRadio)
	ON_BN_CLICKED(IDC_TM_COPYNODEDATA, OnTmCopynodedata)
	ON_BN_CLICKED(IDC_TM_COPYELEMDATA, OnTmCopyelemdata)
	ON_BN_CLICKED(IDC_TM_TOLERANCE, OnTmTolerance)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_MATL_INC_SPIN, OnDeltaposTmIncSpin)
	ON_BN_CLICKED(IDC_TM_MOVE_RADIO, OnTmCopyMoveRadio)
	ON_BN_CLICKED(IDC_TM_YZ_RADIO, OnTmPlaneRadio)
	ON_BN_CLICKED(IDC_TM_ZX_RADIO, OnTmPlaneRadio)
	ON_BN_CLICKED(IDC_TM_3POINT_RADIO, OnTmPlaneRadio)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_SECT_INC_SPIN, OnDeltaposTmIncSpin)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_THIK_INC_SPIN, OnDeltaposTmIncSpin)
	ON_BN_CLICKED(IDC_TM_INTST_TOL, OnTmIntstTol)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemMirrorDlg message handlers

BOOL CElemMirrorDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	m_wnd3rdPoint.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0)); 
	m_wnd2ndPoint.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wnd1stPoint.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	
	m_wndZCoord  .SetWindowText(_T("0"));
	m_wndYCoord  .SetWindowText(_T("0"));
	m_wndXCoord  .SetWindowText(_T("0"));

	m_wndMatlInc.SetRange(SHRT_MIN, SHRT_MAX);
	m_wndMatlInc.SetWindowText(_T("0"));
	m_wndSectInc.SetRange(SHRT_MIN, SHRT_MAX);
	m_wndSectInc.SetWindowText(_T("0"));
	m_wndThikInc.SetRange(SHRT_MIN, SHRT_MAX);
	m_wndThikInc.SetWindowText(_T("0"));

	m_wndXCoord.SetAttUcsPos();
	m_wndYCoord.SetAttUcsPos();
	m_wndZCoord.SetAttUcsPos();
	m_wndZCoord.SetModeToUse(MOUSEEDIT_USE_POS_Z_ONLY);
	m_wndYCoord.SetModeToUse(MOUSEEDIT_USE_POS_Y_ONLY);
	m_wndXCoord.SetModeToUse(MOUSEEDIT_USE_POS_X_ONLY);

	m_wnd1stPoint.SetAttUcsPos();
	m_wnd2ndPoint.SetAttUcsPos();
	m_wnd3rdPoint.SetAttUcsPos();
	m_wnd1stPoint.SetModeToUse(MOUSEEDIT_USE_PLANE_P1);
	m_wnd2ndPoint.SetModeToUse(MOUSEEDIT_USE_PLANE_P2);
	m_wnd3rdPoint.SetModeToUse(MOUSEEDIT_USE_PLANE_P3);

	m_wndXCoordUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndYCoordUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndZCoordUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndP1CoordUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndP2CoordUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndP3CoordUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	// TODO: Add extra initialization here
	//CtrlEnableDisable(m_PlaneDisable,FALSE);
	CtrlEnableDisable(m_PlaneYZEnable,m_nWhatPlane == 0);
	CtrlEnableDisable(m_PlaneXYEnable,m_nWhatPlane == 1);
	CtrlEnableDisable(m_PlaneXZEnable,m_nWhatPlane == 2);
	CtrlEnableDisable(m_Plane3PEnable,m_nWhatPlane == 3);
	CtrlEnableDisable(m_MoveDisable,m_nCopyOrMove == 0);

	GotoDlgCtrl(&m_wndXCoord);

//#ifdef _MDEMO
//  m_bBetaAngle = FALSE;
//  CWnd* pWnd = GetDlgItem(IDC_TM_BETA_ANGLE_CHECK);
//  pWnd->EnableWindow(FALSE);
//#endif

#ifdef _MVIL
	CtrlEnableDisable(m_aThik, FALSE);
#endif

	return FALSE;  // return TRUE unless you set the focus to a control
	               // EXCEPTION: OCX Property Pages should return FALSE
}


void CElemMirrorDlg::OnTmCopyMoveRadio() 
{
	UpdateData(TRUE);
	CtrlEnableDisable(m_MoveDisable,m_nCopyOrMove == 0); 
#ifdef _MVIL
	CtrlEnableDisable(m_aThik, FALSE);
#endif
}


void CElemMirrorDlg::OnTmPlaneRadio() 
{
	UpdateData(TRUE);
	//CtrlEnableDisable(m_PlaneDisable,FALSE);
	CtrlEnableDisable(m_PlaneYZEnable,m_nWhatPlane == 0);
	CtrlEnableDisable(m_PlaneXYEnable,m_nWhatPlane == 1);
	CtrlEnableDisable(m_PlaneXZEnable,m_nWhatPlane == 2);
	CtrlEnableDisable(m_Plane3PEnable,m_nWhatPlane == 3);
	switch(m_nWhatPlane)
	{
	case 0:
		GotoDlgCtrl(&m_wndXCoord); break;
	case 1:
		GotoDlgCtrl(&m_wndZCoord); break;
	case 2:
		GotoDlgCtrl(&m_wndYCoord); break;
	case 3:
		GotoDlgCtrl(&m_wnd1stPoint); break;
	}
}

BOOL CElemMirrorDlg::GetPoint(CMouseEdit& m_wndPoint, double &Px, double &Py, double &Pz)
{
	if(m_wndPoint.IsKeyPressed())
	{
		CString strPos;
		m_wndPoint.GetWindowText(strPos);
		return GetPosition(strPos,Px,Py,Pz);
	}
	else
	{
		double wx, wy, wz;
		m_wndPoint.GetCurMousePosReal(wx,wy,wz,Px,Py,Pz);
	}
	return TRUE;
}

///////////////////////
BOOL CElemMirrorDlg::GetParameter(_ElemMirror &EM)
{
	UpdateData(TRUE);
	
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	EM.m_bIsCopy = m_nCopyOrMove == 0;
	EM.m_nWhatPlane = m_nWhatPlane; // 0 : yz, 1 : xy, 2 : zx, 3 : 3Point
	
	// MQC:3124-Seungjun-20100203 mirror 기능을 사용할 경우 오차가 발생.
	double dPosX = 0.0;  double dPosY = 0.0;  double dPosZ = 0.0;

	CString csValue;
	double dblValue;
	switch(m_nWhatPlane)
	{
	case 0: // yz
		// MQC:3124-Seungjun-20100203 mirror 기능을 사용할 경우 오차가 발생.
		// point를 mouse로 picking할 경우 string parsing 하지 않고 실제 picking한 좌표를 가져와서 사용.
		// string parsing을 할 경우 유효숫자 문제(반올림 해버림)로 오차 발생.
		if(!m_wndXCoord.GetCoordData(&dblValue, &dPosY, &dPosZ))
		{
			m_wndXCoord.GetWindowText(csValue);
			GetFloatNumber(csValue, dblValue);
		}
		EM.m_Point1XU = dblValue;
		EM.m_Point1YU = 0.0;
		EM.m_Point1ZU = 0.0;
		EM.m_Point2XU = dblValue;
		EM.m_Point2YU = 1.0;
		EM.m_Point2ZU = 0.0;
		EM.m_Point3XU = dblValue;
		EM.m_Point3YU = 0.0;
		EM.m_Point3ZU = 1.0;
		break;
	case 1: // xy
		// MQC:3124-Seungjun-20100203 mirror 기능을 사용할 경우 오차가 발생.
		if(!m_wndZCoord.GetCoordData(&dPosX, &dPosY, &dblValue))
		{
			m_wndZCoord.GetWindowText(csValue);
			GetFloatNumber(csValue, dblValue);
		}
		EM.m_Point1XU = 0.0;
		EM.m_Point1YU = 0.0;
		EM.m_Point1ZU = dblValue;
		EM.m_Point2XU = 1.0;
		EM.m_Point2YU = 0.0;
		EM.m_Point2ZU = dblValue;
		EM.m_Point3XU = 0.0;
		EM.m_Point3YU = 1.0;
		EM.m_Point3ZU = dblValue;
		break;
	case 2: // zx
		// MQC:3124-Seungjun-20100203 mirror 기능을 사용할 경우 오차가 발생.
		if(!m_wndYCoord.GetCoordData(&dPosX, &dblValue, &dPosZ))
		{
			m_wndYCoord.GetWindowText(csValue);
			GetFloatNumber(csValue, dblValue);
		}
		EM.m_Point1XU = 0.0;
		EM.m_Point1YU = dblValue;
		EM.m_Point1ZU = 0.0;
		EM.m_Point2XU = 1.0;
		EM.m_Point2YU = dblValue;
		EM.m_Point2ZU = 0.0;
		EM.m_Point3XU = 0.0;
		EM.m_Point3YU = dblValue;
		EM.m_Point3ZU = 1.0;
		break;
	case 3: // 3Point
		if (!GetPoint(m_wnd1stPoint, EM.m_Point1XU, EM.m_Point1YU, EM.m_Point1ZU)) return FALSE;
		if (!GetPoint(m_wnd2ndPoint, EM.m_Point2XU, EM.m_Point2YU, EM.m_Point2ZU)) return FALSE;
		if (!GetPoint(m_wnd3rdPoint, EM.m_Point3XU, EM.m_Point3YU, EM.m_Point3ZU)) return FALSE;
		break;
	default:
		break;
	}

	pIGM->GetWcsPos(EM.m_Point1XU, EM.m_Point1YU, EM.m_Point1ZU,
									EM.m_Point1XW, EM.m_Point1YW, EM.m_Point1ZW);
	pIGM->GetWcsPos(EM.m_Point2XU, EM.m_Point2YU, EM.m_Point2ZU,
									EM.m_Point2XW, EM.m_Point2YW, EM.m_Point2ZW);
	pIGM->GetWcsPos(EM.m_Point3XU, EM.m_Point3YU, EM.m_Point3ZU,
									EM.m_Point3XW, EM.m_Point3YW, EM.m_Point3ZW);
	
	EM.m_bAttrCopy        = m_bIsCopyElemAttr;
	EM.m_bNodeAttrCopy    = m_bIsCopyNodeAttr;

	if (!m_wndMatlInc.GetEditValue(EM.m_nMatlInc))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_material_increment_));
		return FALSE;
	}
	if (!m_wndSectInc.GetEditValue(EM.m_nSectInc))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_section_increment_v));
		return FALSE;
	}
	if (!m_wndThikInc.GetEditValue(EM.m_nThikInc))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_thickness_increment));
		return FALSE;
	}
	
	EM.m_bIntstNode = m_bIntstNode;
	EM.m_bIntstElem = m_bIntstElem;

	EM.m_bBetaAngle = m_bBetaAngle;
	EM.m_bReverseElem = m_bReverseElem;         // Reverse Element Data  입력.

	return TRUE;
}

/*
	BOOL MirrorElem_Move(CArray<T_ELEM_K, T_ELEM_K>& rKey, double p1[3], double p2[3], double p3[3]);
	BOOL MirrorElem_Copy(CArray<T_ELEM_K, T_ELEM_K>& rKey, double p1[3], double p2[3], double p3[3], BOOL bCopyElemAttr);
*/
void CElemMirrorDlg::MoveElem(_ElemMirror& EM)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<unsigned int, unsigned int> NKeyArray;
	
	if(pIGM->GetSelectedElemKeyList(NKeyArray))
	{
		double p1[3];
		double p2[3];
		double p3[3];
		
		p1[0] = EM.m_Point1XW;
		p1[1] = EM.m_Point1YW;
		p1[2] = EM.m_Point1ZW;
		p2[0] = EM.m_Point2XW;
		p2[1] = EM.m_Point2YW;
		p2[2] = EM.m_Point2ZW;
		p3[0] = EM.m_Point3XW;
		p3[1] = EM.m_Point3YW;
		p3[2] = EM.m_Point3ZW;
		
		BOOL bSuccess = pDoc->m_pDataCtrl->MirrorElem_Move(NKeyArray, p1, p2, p3, EM.m_bBetaAngle, EM.m_bReverseElem);
		if (bSuccess) pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
	else
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_element_));
}

void CElemMirrorDlg::CopyElem(_ElemMirror& EM)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<unsigned int, unsigned int> NKeyArray;
	
	if(pIGM->GetSelectedElemKeyList(NKeyArray))
	{
		double p1[3];
		double p2[3];
		double p3[3];
		
		p1[0] = EM.m_Point1XW;
		p1[1] = EM.m_Point1YW;
		p1[2] = EM.m_Point1ZW;
		p2[0] = EM.m_Point2XW;
		p2[1] = EM.m_Point2YW;
		p2[2] = EM.m_Point2ZW;
		p3[0] = EM.m_Point3XW;
		p3[1] = EM.m_Point3YW;
		p3[2] = EM.m_Point3ZW;
		
		BOOL bSuccess = pDoc->m_pDataCtrl->MirrorElem_Copy(NKeyArray, p1, p2, p3, 
																		EM.m_bNodeAttrCopy, EM.m_bAttrCopy, EM.m_bBetaAngle,
																		EM.m_nMatlInc, TRUE, 
																		EM.m_nSectInc, TRUE, 
																		EM.m_nThikInc, TRUE,
																		EM.m_bIntstNode, EM.m_bIntstElem, EM.m_bReverseElem);
		pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
	else
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_element_));
	}
}

void CElemMirrorDlg::Execute()
{
	_ElemMirror EM;
	GetParameter(EM);
	if(EM.m_bIsCopy)
	{
		CopyElem(EM);
	}
	else
	{
		MoveElem(EM);
	}
}

void CElemMirrorDlg::OnTmCopynodedata() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_COPYATTR_NODE,0));
}

void CElemMirrorDlg::OnTmCopyelemdata() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_COPYATTR_ELEM,0));
}

void CElemMirrorDlg::OnTmTolerance() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_MERGETOL,0));
}


void CElemMirrorDlg::OnDeltaposTmIncSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	const MSG* pMsg = GetCurrentMessage();
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	int NIter;
	
	CString strIter;

	CFormulaEditSpin* pIterNum;
	UINT nID = LOWORD(pMsg->wParam);
	switch(nID)
	{
	case IDC_TM_MATL_INC_SPIN: pIterNum = &m_wndMatlInc; break;
	case IDC_TM_SECT_INC_SPIN: pIterNum = &m_wndSectInc; break;
	case IDC_TM_THIK_INC_SPIN: pIterNum = &m_wndThikInc; break;
	default: ASSERT(0);
	}

	pIterNum->GetEditValue(NIter);
	NIter -= pNMUpDown->iDelta;
	
	strIter.Format(_LS(IDS_WG_TREEMENU__d),NIter);
	
	pIterNum->SetWindowText(strIter);
	*pResult = 0;
}

void CElemMirrorDlg::OnTmIntstTol() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_INTERSECTTOL,0));		
}

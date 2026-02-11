// NodeMirrorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "NodeMirrorDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_dbRes2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNodeMirrorDlg dialog

CNodeMirrorDlg::CNodeMirrorDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CNodeMirrorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNodeMirrorDlg)
	m_nCopyOrMove = 0;
	m_nWhatPlane =  0;
	m_bIsMergeDupNode = TRUE;
	m_bIsCopyAttr = FALSE;
	m_bIntersect = FALSE;
	//}}AFX_DATA_INIT

	m_MoveDisable.Add(IDC_TM_MERGE_DUP_NODE_CHECK);
	m_MoveDisable.Add(IDC_TM_COPY_NODEATTR_CHECK);
	m_MoveDisable.Add(IDC_TM_TOLERANCE);
	m_MoveDisable.Add(IDC_TM_COPYNODEDATA);
	
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


void CNodeMirrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNodeMirrorDlg)
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
	DDX_Check(pDX, IDC_TM_MERGE_DUP_NODE_CHECK, m_bIsMergeDupNode);
	DDX_Check(pDX, IDC_TM_COPY_NODEATTR_CHECK, m_bIsCopyAttr);
	DDX_Check(pDX, IDC_TM_ITST_CHECK, m_bIntersect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNodeMirrorDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CNodeMirrorDlg)
	ON_BN_CLICKED(IDC_TM_COPY_RADIO, OnTmCopyMoveRadio)
	ON_BN_CLICKED(IDC_TM_XY_RADIO, OnTmPlaneRadio)
	ON_BN_CLICKED(IDC_TM_COPYNODEDATA, OnTmCopynodedata)
	ON_BN_CLICKED(IDC_TM_TOLERANCE, OnTmTolerance)
	ON_BN_CLICKED(IDC_TM_MOVE_RADIO, OnTmCopyMoveRadio)
	ON_BN_CLICKED(IDC_TM_YZ_RADIO, OnTmPlaneRadio)
	ON_BN_CLICKED(IDC_TM_ZX_RADIO, OnTmPlaneRadio)
	ON_BN_CLICKED(IDC_TM_3POINT_RADIO, OnTmPlaneRadio)
	ON_BN_CLICKED(IDC_TM_INTSECT_TOL, OnTmIntsectTol)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNodeMirrorDlg message handlers

BOOL CNodeMirrorDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	m_wnd3rdPoint.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0)); 
	m_wnd2ndPoint.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wnd1stPoint.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	
	m_wndZCoord  .SetWindowText(_T("0"));
	m_wndYCoord  .SetWindowText(_T("0"));
	m_wndXCoord  .SetWindowText(_T("0"));

	m_wndXCoord  .SetAttUcsPos();
	m_wndYCoord  .SetAttUcsPos();
	m_wndZCoord  .SetAttUcsPos();
	m_wndZCoord  .SetModeToUse(MOUSEEDIT_USE_POS_Z_ONLY);
	m_wndYCoord  .SetModeToUse(MOUSEEDIT_USE_POS_Y_ONLY);
	m_wndXCoord  .SetModeToUse(MOUSEEDIT_USE_POS_X_ONLY);

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

	return FALSE;  // return TRUE unless you set the focus to a control
	               // EXCEPTION: OCX Property Pages should return FALSE
}


void CNodeMirrorDlg::OnTmCopyMoveRadio() 
{
	UpdateData(TRUE);
	CtrlEnableDisable(m_MoveDisable,m_nCopyOrMove == 0); 
}


void CNodeMirrorDlg::OnTmPlaneRadio() 
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

BOOL CNodeMirrorDlg::GetPoint(CMouseEdit& m_wndPoint, double &Px, double &Py, double &Pz)
{
	//if(m_wnd1stPoint.IsKeyPressed())
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
BOOL CNodeMirrorDlg::GetParameter(_NodeMirror &NM)
{
	UpdateData(TRUE);
	
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	NM.m_bIsCopy = m_nCopyOrMove == 0;
	NM.m_nWhatPlane = m_nWhatPlane; // 0 : yz, 1 : xy, 2 : zx, 3 : 3Point
	
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
		NM.m_Point1XU = dblValue;
		NM.m_Point1YU = 0.0;
		NM.m_Point1ZU = 0.0;
		NM.m_Point2XU = dblValue;
		NM.m_Point2YU = 1.0;
		NM.m_Point2ZU = 0.0;
		NM.m_Point3XU = dblValue;
		NM.m_Point3YU = 0.0;
		NM.m_Point3ZU = 1.0;
		break;
	case 1: // xy
		// MQC:3124-Seungjun-20100203 mirror 기능을 사용할 경우 오차가 발생.
		if(!m_wndZCoord.GetCoordData(&dPosX, &dPosY, &dblValue))
		{
			m_wndZCoord.GetWindowText(csValue);
			GetFloatNumber(csValue, dblValue);
		}
		NM.m_Point1XU = 0.0;
		NM.m_Point1YU = 0.0;
		NM.m_Point1ZU = dblValue;
		NM.m_Point2XU = 1.0;
		NM.m_Point2YU = 0.0;
		NM.m_Point2ZU = dblValue;
		NM.m_Point3XU = 0.0;
		NM.m_Point3YU = 1.0;
		NM.m_Point3ZU = dblValue;
		break;
	case 2: // zx
		// MQC:3124-Seungjun-20100203 mirror 기능을 사용할 경우 오차가 발생.
		if(!m_wndYCoord.GetCoordData(&dPosX, &dblValue, &dPosZ))
		{
			m_wndYCoord.GetWindowText(csValue);
			GetFloatNumber(csValue, dblValue);
		}
		NM.m_Point1XU = 0.0;
		NM.m_Point1YU = dblValue;
		NM.m_Point1ZU = 0.0;
		NM.m_Point2XU = 1.0;
		NM.m_Point2YU = dblValue;
		NM.m_Point2ZU = 0.0;
		NM.m_Point3XU = 0.0;
		NM.m_Point3YU = dblValue;
		NM.m_Point3ZU = 1.0;
		break;
	case 3: // 3Point
		if (!GetPoint(m_wnd1stPoint, NM.m_Point1XU, NM.m_Point1YU, NM.m_Point1ZU)) return FALSE;
		if (!GetPoint(m_wnd2ndPoint, NM.m_Point2XU, NM.m_Point2YU, NM.m_Point2ZU)) return FALSE;
		if (!GetPoint(m_wnd3rdPoint, NM.m_Point3XU, NM.m_Point3YU, NM.m_Point3ZU)) return FALSE;
		break;
	default:
		break;
	}

	pIGM->GetWcsPos(NM.m_Point1XU, NM.m_Point1YU, NM.m_Point1ZU,
									NM.m_Point1XW, NM.m_Point1YW, NM.m_Point1ZW);
	pIGM->GetWcsPos(NM.m_Point2XU, NM.m_Point2YU, NM.m_Point2ZU,
									NM.m_Point2XW, NM.m_Point2YW, NM.m_Point2ZW);
	pIGM->GetWcsPos(NM.m_Point3XU, NM.m_Point3YU, NM.m_Point3ZU,
									NM.m_Point3XW, NM.m_Point3YW, NM.m_Point3ZW);
	
	NM.m_bMergeDupNode    = m_bIsMergeDupNode;
	NM.m_bAttrCopy        = m_bIsCopyAttr;
	NM.m_bIntersect       = m_bIntersect;

	return TRUE;
}

/*
	BOOL MirrorNode_Move(CArray<T_NODE_K, T_NODE_K>& rKey, double p1[3], double p2[3], double p3[3]);
	BOOL MirrorNode_Copy(CArray<T_NODE_K, T_NODE_K>& rKey, double p1[3], double p2[3], double p3[3], 
												BOOL bMergeNode, BOOL bCopyNodeAttr);
*/
void CNodeMirrorDlg::MoveNode(_NodeMirror& NM)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<unsigned int, unsigned int> NKeyArray;
	
	if(pIGM->GetSelectedNodeKeyList(NKeyArray))
	{
		double p1[3];
		double p2[3];
		double p3[3];
		
		p1[0] = NM.m_Point1XW;
		p1[1] = NM.m_Point1YW;
		p1[2] = NM.m_Point1ZW;
		p2[0] = NM.m_Point2XW;
		p2[1] = NM.m_Point2YW;
		p2[2] = NM.m_Point2ZW;
		p3[0] = NM.m_Point3XW;
		p3[1] = NM.m_Point3YW;
		p3[2] = NM.m_Point3ZW;
		
		BOOL bSuccess = pDoc->m_pDataCtrl->MirrorNode_Move(NKeyArray, p1, p2, p3, 
																											 NM.m_bIntersect);
		if (bSuccess)
			pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
	else
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
	}
}

void CNodeMirrorDlg::CopyNode(_NodeMirror& NM)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<unsigned int, unsigned int> NKeyArray;
	
	if(pIGM->GetSelectedNodeKeyList(NKeyArray))
	{
		double p1[3];
		double p2[3];
		double p3[3];
		
		p1[0] = NM.m_Point1XW;
		p1[1] = NM.m_Point1YW;
		p1[2] = NM.m_Point1ZW;
		p2[0] = NM.m_Point2XW;
		p2[1] = NM.m_Point2YW;
		p2[2] = NM.m_Point2ZW;
		p3[0] = NM.m_Point3XW;
		p3[1] = NM.m_Point3YW;
		p3[2] = NM.m_Point3ZW;
		
		BOOL bSuccess = pDoc->m_pDataCtrl->MirrorNode_Copy(NKeyArray, p1, p2, p3,
																					NM.m_bMergeDupNode ,NM.m_bAttrCopy,
																					NM.m_bIntersect);
		if (bSuccess)
			pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
	else
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
	}
}

void CNodeMirrorDlg::Execute()
{
	_NodeMirror NM;
	GetParameter(NM);
	if(NM.m_bIsCopy)
	{
		CopyNode(NM);
	}
	else
	{
		MoveNode(NM);
	}
}


void CNodeMirrorDlg::OnTmCopynodedata() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_COPYATTR_NODE,0));
}

void CNodeMirrorDlg::OnTmTolerance() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_MERGETOL,0));
}

void CNodeMirrorDlg::OnTmIntsectTol() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_INTERSECTTOL,0));	
}

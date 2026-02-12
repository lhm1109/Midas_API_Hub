// NodeRotateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "NodeRotateDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_dbRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNodeRotateDlg dialog


CNodeRotateDlg::CNodeRotateDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CNodeRotateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNodeRotateDlg)
	m_nCopyOrMove = 0;
	m_nRotAxis = 0;
	m_bCopyNodeAttr = FALSE;
	m_bMergeDupNodeCheck = TRUE;
	m_bIntersect = FALSE;
	m_nAngleType = 0;
	//}}AFX_DATA_INIT
 
	m_MoveDisable.Add(IDC_TM_NUMBEROFTIMES_EDIT);
	m_MoveDisable.Add(IDC_TM_NTIMES_STATIC);
	//m_MoveDisable.Add(IDC_TM_NTIMES_SPIN);
	m_MoveDisable.Add(IDC_TM_COPY_NODEATTR_CHECK);
	m_MoveDisable.Add(IDC_TM_MERGE_DUP_NODE_CHECK);
	m_MoveDisable.Add(IDC_TM_DUPLICATE_TOL);
	m_MoveDisable.Add(IDC_TM_COPYATTR_OPT);
	m_MoveDisable.Add(IDC_TM_NTIME_RDO_UNEQUAL);

	m_2PEnable.Add( IDC_TM_1STP_STATIC     );
	m_2PEnable.Add( IDC_TM_1ST_POINT_EDIT  );
	m_2PEnable.Add( IDC_TM_DIST_STATIC2    );
	m_2PEnable.Add( IDC_TM_2ND_STATIC      );
	m_2PEnable.Add( IDC_TM_2ND_POINT_EDIT  );
	m_2PEnable.Add( IDC_TM_DIST_STATIC3    );

	m_OneAxisDisable.Add( IDC_TM_2ND_STATIC    );
	m_OneAxisDisable.Add( IDC_TM_2ND_POINT_EDIT);
	m_OneAxisDisable.Add( IDC_TM_DIST_STATIC3  );

	m_OneAxisEnable.Add(IDC_TM_1STP_STATIC   );
	m_OneAxisEnable.Add(IDC_TM_1ST_POINT_EDIT);
	m_OneAxisEnable.Add(IDC_TM_DIST_STATIC2  );

	// Angle부분
	m_arEqualCtrl.Add(IDC_TM_NTIME_STATIC);
	m_arEqualCtrl.Add(IDC_TM_NUMBEROFTIMES_EDIT);
	//m_arEqualCtrl.Add(IDC_TM_NTIMES_SPIN);
	m_arEqualCtrl.Add(IDC_TM_ROT_ANGLE_STATIC);
	m_arEqualCtrl.Add(IDC_TM_ROT_ANGLE_EDIT);
	//m_arEqualCtrl.Add(IDC_TM_ROT_ANGLE_SPIN);
	m_arEqualCtrl.Add(IDC_TM_ANGLE_STATIC);

	m_arUnEqualCtrl.Add(IDC_TM_ROT_UNEQ_ANGLE_STATIC);
	m_arUnEqualCtrl.Add(IDC_TM_ROT_UNEQ_ANGLE_EDIT);
	m_arUnEqualCtrl.Add(IDC_TM_ROT_UNEQ_ANGLE_UNIT);
	m_arUnEqualCtrl.Add(IDC_TM_ROT_UNEQ_ANGLE_EXAM_STATIC);
}

void CNodeRotateDlg::AlignControl()
{
	CRect rect,rectRef;

	CWnd* pWnd = GetDlgItem(m_arEqualCtrl.GetAt(0));
	pWnd->GetWindowRect(rectRef);
	pWnd = GetDlgItem(m_arUnEqualCtrl.GetAt(0));
	pWnd->GetWindowRect(rect);

	CDlgUtil::CtrlMoveDistY(this, m_arUnEqualCtrl, rectRef.top - rect.top);

	pWnd = GetDlgItem(IDC_TM_STATIC1);
	pWnd->GetWindowRect(rectRef);
	
	this->GetWindowRect(rect);
	rect.bottom = rectRef.bottom+1;
	CWnd* pParent = GetParent();
	pParent->ScreenToClient(rect);
	this->MoveWindow(rect);
}

void CNodeRotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNodeRotateDlg)
	DDX_Control(pDX, IDC_TM_ROT_UNEQ_ANGLE_UNIT, m_wndUEAngleUnit);
	DDX_Control(pDX, IDC_TM_ROT_UNEQ_ANGLE_EDIT, m_edtUEAngle);
	DDX_Control(pDX, IDC_TM_DIST_AXIAL_UNIT, m_wndDistAxialUnit);
	DDX_Control(pDX, IDC_TM_DIST_AXIAL_EDIT, m_wndDistAxialEdit);
	DDX_Control(pDX, IDC_TM_ANGLE_STATIC  , m_wndAngleStatic);
	DDX_Control(pDX, IDC_TM_DIST_STATIC3  , m_wndDistStatic3);
	DDX_Control(pDX, IDC_TM_DIST_STATIC2  , m_wndDistStatic2);
	DDX_Control(pDX, IDC_TM_DIST_STATIC   , m_wndDistStatic1);
	DDX_Control(pDX, IDC_TM_DISTANCE_EDIT ,     m_wndDist       );
	DDX_Control(pDX, IDC_TM_ROT_ANGLE_EDIT,     m_wndRotAngle   );
	DDX_Control(pDX, IDC_TM_2ND_POINT_EDIT,     m_wnd2ndPoint   );
	DDX_Control(pDX, IDC_TM_1ST_POINT_EDIT,     m_wnd1stPoint   );
	DDX_Control(pDX, IDC_TM_NUMBEROFTIMES_EDIT, m_wndIterNum);
	DDX_Radio(pDX, IDC_TM_COPY_RADIO, m_nCopyOrMove);
	DDX_Radio(pDX, IDC_TM_AXIS_X, m_nRotAxis);
	DDX_Check(pDX, IDC_TM_COPY_NODEATTR_CHECK, m_bCopyNodeAttr);
	DDX_Check(pDX, IDC_TM_MERGE_DUP_NODE_CHECK, m_bMergeDupNodeCheck);
	DDX_Check(pDX, IDC_TM_ITST_CHECK, m_bIntersect);
	DDX_Radio(pDX, IDC_TM_NTIME_RDO_EQUAL, m_nAngleType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNodeRotateDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CNodeRotateDlg)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_NTIMES_SPIN, OnDeltaposTmNtimesSpin)
	ON_BN_CLICKED(IDC_TM_AXIS_X, OnTmAxisRadio)
	ON_BN_CLICKED(IDC_TM_COPY_RADIO, OnTmCopyMoveRadio)
	ON_BN_CLICKED(IDC_TM_COPYATTR_OPT, OnTmCopyattrOpt)
	ON_BN_CLICKED(IDC_TM_DUPLICATE_TOL, OnTmDuplicateTol)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_ROT_ANGLE_SPIN, OnDeltaposTmRotAngleSpin)
	ON_BN_CLICKED(IDC_TM_INTSECT_TOL, OnTmIntsectTol)
	ON_BN_CLICKED(IDC_TM_AXIS_Y, OnTmAxisRadio)
	ON_BN_CLICKED(IDC_TM_AXIS_Z, OnTmAxisRadio)
	ON_BN_CLICKED(IDC_TM_MOVE_RADIO, OnTmCopyMoveRadio)
	ON_BN_CLICKED(IDC_TM_AXIS_2POINT, OnTmAxisRadio)
	ON_BN_CLICKED(IDC_TM_NTIME_RDO_EQUAL, OnAngleType)
	ON_BN_CLICKED(IDC_TM_NTIME_RDO_UNEQUAL, OnAngleType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNodeRotateDlg message handlers

BOOL CNodeRotateDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd* pWnd = GetDlgItem(IDC_TM_NUMBEROFTIMES_EDIT);
	GotoDlgCtrl(pWnd);

	m_wnd1stPoint.SetAttUcsPos();
	m_wnd1stPoint.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wnd2ndPoint.SetAttUcsPos();
	m_wnd2ndPoint.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wnd1stPoint.SetNextLink(&m_wnd2ndPoint);
	m_wnd2ndPoint.SetNextLink(NULL);
	
	m_wndDist.SetAttUcsDistance();
	m_wndDist.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndDist.SetDistValueMode(TRUE);
	m_wndDistAxialEdit.SetAttUcsDistance();
	m_wndDistAxialEdit.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndDistAxialEdit.SetDistValueMode(TRUE);

	CtrlEnableDisable(m_MoveDisable,m_nCopyOrMove == 0);
	
	CtrlEnableDisable(m_2PEnable,m_nRotAxis == 3);

	CtrlEnableDisable(m_OneAxisEnable, TRUE);

	m_wndDist.SetWindowText(_T("0"));     
	m_wndDistAxialEdit.SetWindowText(_T("0"));
	m_wndRotAngle.SetRange(-360, 360);
	m_wndRotAngle.SetValue(0);
	m_wnd2ndPoint.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0)); 
	m_wnd1stPoint.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wndIterNum.SetRange(1, SHRT_MAX);
	m_wndIterNum.SetValue(1);
	m_wndIterNum.SetInteger(TRUE);
	m_wndAngleStatic.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_wndUEAngleUnit.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_wndDistStatic3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDistStatic2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDistStatic1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDistAxialUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	AlignControl();
	OnAngleType(); 

	return FALSE;  // return TRUE unless you set the focus to a control
	               // EXCEPTION: OCX Property Pages should return FALSE
}

void CNodeRotateDlg::OnDeltaposTmNtimesSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString strIter;
	int NIter;
	
	GetNTimes(NIter);
	NIter -= pNMUpDown->iDelta;
	if (NIter < 0) NIter = 0;
	
	strIter.Format(_LS(IDS_WG_TREEMENU__d),NIter);
	
	m_wndIterNum.SetWindowText(strIter);
	*pResult = 0;
}

void CNodeRotateDlg::OnDeltaposTmRotAngleSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString csAngle;
	double dblAngle;
	if (!GetRotAngle(dblAngle)) dblAngle = 0.0;
	CEdit* pwndAngle = (CEdit*)GetDlgItem(IDC_TM_ROT_ANGLE_EDIT);
	/*
	pwndAngle->GetWindowText(csAngle);
	dblAngle = _tstof(csAngle);
	*/

	if(pNMUpDown->iDelta < 0)
		dblAngle -= (double)(pNMUpDown->iDelta);
	else
		dblAngle -= (double)(pNMUpDown->iDelta);
	if (dblAngle > 360.0) dblAngle = 360.0;
	else if (dblAngle < -360.0) dblAngle = -360.0;

	csAngle.Format(_LS(IDS_WG_TREEMENU__g), dblAngle);
	pwndAngle->SetWindowText(csAngle);
	*pResult = 0;
}

BOOL CNodeRotateDlg::GetNTimes(int &NTimes)
{
	 return m_wndIterNum.GetEditValue(NTimes);
}

BOOL CNodeRotateDlg::GetFirstPoint(double &Px, double &Py, double &Pz)
{
	if(m_wnd1stPoint.IsKeyPressed())
	{
		CString strPos;
		m_wnd1stPoint.GetWindowText(strPos);
		return GetPosition(strPos,Px,Py,Pz);
	}
	else
	{
		double wx, wy, wz;
		m_wnd1stPoint.GetCurMousePosReal(wx,wy,wz,Px,Py,Pz);
	}
	return TRUE;
}

BOOL CNodeRotateDlg::GetSecondPoint(double &Px, double &Py, double &Pz)
{
	if(m_wnd2ndPoint.IsKeyPressed())
	{
		CString strPos;
		m_wnd2ndPoint.GetWindowText(strPos);
		return GetPosition(strPos,Px,Py,Pz);
	}
	else
	{
		double wx, wy, wz;
		m_wnd2ndPoint.GetCurMousePosReal(wx,wy,wz,Px,Py,Pz);
	}
	return TRUE;
}

BOOL CNodeRotateDlg::GetRotAngle(double &RotAng)
{
	CString strRot;
	m_wndRotAngle.GetWindowText(strRot);
	return GetFloatNumber(strRot,RotAng);  
}

BOOL CNodeRotateDlg::GetRotAngle(_NodeRotate &NR)
{
	ASSERT(m_nAngleType==0 || m_nAngleType==1);

	CString strRot;
	double RotAng;
	NR.m_arAngle.RemoveAll();

	if(m_nAngleType)  // Unequal Angle Type
	{
		m_edtUEAngle.GetWindowText(strRot);
		if(!GetUEDistance(strRot, NR.m_arAngle)) return FALSE;
	}
	else // Equal Angle Type
	{
	  m_wndRotAngle.GetWindowText(strRot);
		if(!GetFloatNumber(strRot,RotAng)) return FALSE;

	  for(int i=0; i<NR.m_NTimes; i++)
			NR.m_arAngle.Add(RotAng);
	}

	return TRUE;
}

BOOL CNodeRotateDlg::GetDistanceDrDz(double &Dr, double &Dz)
{
	CString strRadial, strAxial;
	m_wndDist.GetWindowText(strRadial);
	m_wndDistAxialEdit.GetWindowText(strAxial);

	BOOL bGet = GetFloatNumber(strRadial, Dr);
	bGet = bGet && GetFloatNumber(strAxial, Dz);
	
	return bGet;
}


BOOL CNodeRotateDlg::GetParameter(_NodeRotate &NR)
{
	UpdateData(TRUE);
	
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	NR.m_bIsCopy = m_nCopyOrMove == 0;
	NR.m_nAxis   = m_nRotAxis;  // 0 : X , 1 : Y , 2 : Z , 3 : 2Point

	if(!GetNTimes(NR.m_NTimes)) return FALSE;

	if(!GetRotAngle(NR.m_AngleOfRotDEG)) return FALSE;

	if(!GetRotAngle(NR)) return FALSE;

	if(!GetDistanceDrDz(NR.m_DistR,NR.m_DistZ)) return FALSE;
	
	if(GetFirstPoint(NR.m_FirstP_UX,NR.m_FirstP_UY,NR.m_FirstP_UZ))
	{
		pIGM->GetWcsPos(NR.m_FirstP_UX,NR.m_FirstP_UY,NR.m_FirstP_UZ,
										NR.m_FirstP_WX,NR.m_FirstP_WY,NR.m_FirstP_WZ);
	}
	else
		return FALSE;
	
	switch(m_nRotAxis)
	{
	case 0:
		NR.m_AxisV_Ux = 1.0;
		NR.m_AxisV_Uy = 0.0;
		NR.m_AxisV_Uz = 0.0;
		pIGM->GetWcsVector(NR.m_AxisV_Ux,NR.m_AxisV_Uy,NR.m_AxisV_Uz,
											 NR.m_AxisV_Wx,NR.m_AxisV_Wy,NR.m_AxisV_Wz);
		break;
	case 1:
		NR.m_AxisV_Ux = 0.0;
		NR.m_AxisV_Uy = 1.0;
		NR.m_AxisV_Uz = 0.0;
		pIGM->GetWcsVector(NR.m_AxisV_Ux,NR.m_AxisV_Uy,NR.m_AxisV_Uz,
											 NR.m_AxisV_Wx,NR.m_AxisV_Wy,NR.m_AxisV_Wz);
		break;
	case 2:
		NR.m_AxisV_Ux = 0.0;
		NR.m_AxisV_Uy = 0.0;
		NR.m_AxisV_Uz = 1.0;
		pIGM->GetWcsVector(NR.m_AxisV_Ux,NR.m_AxisV_Uy,NR.m_AxisV_Uz,
											 NR.m_AxisV_Wx,NR.m_AxisV_Wy,NR.m_AxisV_Wz);
		break;
	case 3:
		{
			if(GetSecondPoint(NR.m_SecondP_UX,NR.m_SecondP_UY,NR.m_SecondP_UZ))
			{
				pIGM->GetWcsPos(NR.m_SecondP_UX,NR.m_SecondP_UY,NR.m_SecondP_UZ,
												NR.m_SecondP_WX,NR.m_SecondP_WY,NR.m_SecondP_WZ);
				
				NR.m_AxisV_Ux = NR.m_SecondP_UX - NR.m_FirstP_UX;
				NR.m_AxisV_Uy = NR.m_SecondP_UY - NR.m_FirstP_UY;
				NR.m_AxisV_Uz = NR.m_SecondP_UZ - NR.m_FirstP_UZ;

				double VL;
				
				VL = NR.m_AxisV_Ux*NR.m_AxisV_Ux +  NR.m_AxisV_Uy*NR.m_AxisV_Uy + 
						 NR.m_AxisV_Uz*NR.m_AxisV_Uz;

				VL = sqrt(VL);

				if(pIGM->IsZero(VL))
				{
					GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Rotation_axis_vector_is_zer));
					return FALSE;
				}

				NR.m_AxisV_Ux = NR.m_AxisV_Ux / VL;
				NR.m_AxisV_Uy = NR.m_AxisV_Uy / VL;
				NR.m_AxisV_Uz = NR.m_AxisV_Uz / VL;

				pIGM->GetWcsVector(NR.m_AxisV_Ux,NR.m_AxisV_Uy,NR.m_AxisV_Uz,
													 NR.m_AxisV_Wx,NR.m_AxisV_Wy,NR.m_AxisV_Wz);


			}
			else
				return FALSE;
		}
		break;
	default:
		break;
	}
	
	NR.m_bMergeDupNode    = m_bMergeDupNodeCheck;
	NR.m_bAttrCopy        = m_bCopyNodeAttr;
	NR.m_bIntersect       = m_bIntersect;

	return TRUE;
}

/*
BOOL CDataCtrl::RotateNode_Move(CArray<T_NODE_K, T_NODE_K>&	rKey, double angle,	double dr, double dz, T_AXIS_VECTOR	Axis)
BOOL CDataCtrl::RotateNode_Copy(CArray<T_NODE_K, T_NODE_K>&	rKey, int nCopy, double	angle, double dr, double dz,
								T_AXIS_VECTOR Axis,	BOOL bAllowDuplicate, BOOL bCopyData)
*/

void CNodeRotateDlg::MoveNode(_NodeRotate& NR)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<unsigned int, unsigned int> NKeyArray;
	
	if(pIGM->GetSelectedNodeKeyList(NKeyArray))
	{
		T_AXIS_VECTOR AxisV;
		
		AxisV.dblOrgX = NR.m_FirstP_WX;
		AxisV.dblOrgY = NR.m_FirstP_WY;
		AxisV.dblOrgZ = NR.m_FirstP_WZ;
		AxisV.dblVecX = NR.m_AxisV_Wx;
		AxisV.dblVecY = NR.m_AxisV_Wy;
		AxisV.dblVecZ = NR.m_AxisV_Wz;
		
		BOOL bSuccess = pDoc->m_pDataCtrl->RotateNode_Move(
																			 NKeyArray,NR.m_AngleOfRotDEG,
																			 NR.m_DistR,NR.m_DistZ,AxisV,NR.m_bIntersect);
		if (bSuccess) pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
	else
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
	}
}

void CNodeRotateDlg::CopyNode(_NodeRotate& NR)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<unsigned int, unsigned int> NKeyArray;
	
	if(pIGM->GetSelectedNodeKeyList(NKeyArray))
	{
		T_AXIS_VECTOR AxisV;
		
		AxisV.dblOrgX = NR.m_FirstP_WX;
		AxisV.dblOrgY = NR.m_FirstP_WY;
		AxisV.dblOrgZ = NR.m_FirstP_WZ;
		AxisV.dblVecX = NR.m_AxisV_Wx;
		AxisV.dblVecY = NR.m_AxisV_Wy;
		AxisV.dblVecZ = NR.m_AxisV_Wz;
		
		BOOL bSuccess = pDoc->m_pDataCtrl->RotateNode_Copy(NKeyArray,NR.m_arAngle,NR.m_DistR, NR.m_DistZ,
								        AxisV,NR.m_bMergeDupNode,NR.m_bAttrCopy,NR.m_bIntersect);
		if (bSuccess) pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
	else
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
	}
}

void CNodeRotateDlg::OnTmAxisRadio() 
{
	UpdateData(TRUE);
	if(m_nRotAxis == 3)
		CtrlEnableDisable(m_2PEnable,TRUE);
	else
	{
		CtrlEnableDisable(m_OneAxisEnable,TRUE);
		CtrlEnableDisable(m_OneAxisDisable,FALSE);
	}
	//GotoDlgCtrl(&m_wnd1stPoint);
}

void CNodeRotateDlg::OnTmCopyMoveRadio() 
{
	UpdateData(TRUE);
	CtrlEnableDisable(m_MoveDisable,m_nCopyOrMove == 0);

	if(m_nAngleType)  // UnEqual Angle일때
	{
		CDlgUtil::CtrlShowHide(this, m_arEqualCtrl, TRUE);	
		CDlgUtil::CtrlShowHide(this, m_arUnEqualCtrl, FALSE);		

		m_nAngleType = 0;
		UpdateData(FALSE);
	}
}

void CNodeRotateDlg::Execute()
{
	_NodeRotate NR;
	GetParameter(NR);
	if(NR.m_bIsCopy)
	{
	 CopyNode(NR);
	}
	else
	{
	 MoveNode(NR);
	}
}


void CNodeRotateDlg::OnTmCopyattrOpt() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_COPYATTR_NODE,0));
}

void CNodeRotateDlg::OnTmDuplicateTol() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_MERGETOL,0));
}

void CNodeRotateDlg::OnTmIntsectTol() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_INTERSECTTOL,0));
}

void CNodeRotateDlg::OnAngleType() 
{
	UpdateData(TRUE);

	CDlgUtil::CtrlShowHide(this, m_arEqualCtrl, m_nAngleType == 0);	
	CDlgUtil::CtrlShowHide(this, m_arUnEqualCtrl, m_nAngleType == 1);
}

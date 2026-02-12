// ElemRotateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ElemRotateDlg.h"

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
// CElemRotateDlg dialog


CElemRotateDlg::CElemRotateDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CElemRotateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElemRotateDlg)
	m_nCopyOrMove = 0;
	m_nRotAxis = 0;
	m_bCopyNodeAttr = FALSE;
	m_bCopyElemAttr = FALSE;
	m_bIntstNode = FALSE;
	m_bIntstElem = FALSE;
	m_bBetaAngle = TRUE;
	m_nAngleType = 0;
	//}}AFX_DATA_INIT
	m_MoveDisable.Add(IDC_TM_NUMBEROFTIMES_EDIT);
	m_MoveDisable.Add(IDC_TM_NTIMES_STATIC);
	//m_MoveDisable.Add(IDC_TM_NTIMES_SPIN);
	m_MoveDisable.Add(IDC_TM_COPY_NODEATTR_CHECK);
	m_MoveDisable.Add(IDC_TM_COPY_ELEMATTR_CHECK);
#if !defined(_TOWER)
	m_MoveDisable.Add(IDC_TM_COPYATTR_OPT);
#endif
	m_MoveDisable.Add(IDC_TM_COPYNODE_OPT);
	m_MoveDisable.Add(IDC_TM_MATL_INC_TITLE);
	m_MoveDisable.Add(IDC_TM_MATL_INC_EDIT);
	//m_MoveDisable.Add(IDC_TM_MATL_INC_SPIN);
	m_MoveDisable.Add(IDC_TM_SECT_INC_TITLE);
	m_MoveDisable.Add(IDC_TM_SECT_INC_EDIT);
	//m_MoveDisable.Add(IDC_TM_SECT_INC_SPIN);
	m_MoveDisable.Add(IDC_TM_THIK_INC_TITLE);
	m_MoveDisable.Add(IDC_TM_THIK_INC_EDIT);
	//m_MoveDisable.Add(IDC_TM_THIK_INC_SPIN);
	m_MoveDisable.Add(IDC_TM_INTST_TITLE);
	m_MoveDisable.Add(IDC_TM_INTST_NODE);
	m_MoveDisable.Add(IDC_TM_INTST_ELEM);
	m_MoveDisable.Add(IDC_TM_NTIME_RDO_UNEQUAL);

	m_aThik.Add(IDC_TM_THIK_INC_TITLE);
	m_aThik.Add(IDC_TM_THIK_INC_EDIT);
	//m_aThik.Add(IDC_TM_THIK_INC_SPIN);

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

void CElemRotateDlg::AlignControl()
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
	rect.bottom = rectRef.bottom + globalUtils.ScaleByDPI(4 + 32);
	CWnd* pParent = GetParent();
	pParent->ScreenToClient(rect);
	this->MoveWindow(rect);
}

void CElemRotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemRotateDlg)
	DDX_Control(pDX, IDC_TM_ROT_UNEQ_ANGLE_UNIT, m_wndUEAngleUnit);
	DDX_Control(pDX, IDC_TM_ROT_UNEQ_ANGLE_EDIT, m_edtUEAngle);
	DDX_Control(pDX, IDC_TM_THIK_INC_REP, m_wndThikIncRep);
	DDX_Control(pDX, IDC_TM_SECT_INC_REP, m_wndSectIncRep);
	DDX_Control(pDX, IDC_TM_MATL_INC_REP, m_wndMatlIncRep);
	DDX_Control(pDX, IDC_TM_DIST_AXIAL_EDIT, m_wndDistAxialEdit);
	DDX_Control(pDX, IDC_TM_DIST_AXIAL_UNIT, m_wndDistAxialUnit);
	DDX_Control(pDX, IDC_TM_THIK_INC_EDIT, m_wndThikInc);
	DDX_Control(pDX, IDC_TM_SECT_INC_EDIT, m_wndSectInc);
	DDX_Control(pDX, IDC_TM_MATL_INC_EDIT, m_wndMatlInc);
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
	DDX_Check(pDX, IDC_TM_COPY_ELEMATTR_CHECK, m_bCopyElemAttr);
	DDX_Check(pDX, IDC_TM_INTST_NODE, m_bIntstNode);
	DDX_Check(pDX, IDC_TM_INTST_ELEM, m_bIntstElem);
	DDX_Check(pDX, IDC_TM_BETA_ANGLE_CHECK, m_bBetaAngle);
	DDX_Radio(pDX, IDC_TM_NTIME_RDO_EQUAL, m_nAngleType);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CElemRotateDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CElemRotateDlg)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_NTIMES_SPIN, OnDeltaposTmNtimesSpin)
	ON_BN_CLICKED(IDC_TM_AXIS_X, OnTmAxisRadio)
	ON_BN_CLICKED(IDC_TM_COPY_RADIO, OnTmCopyMoveRadio)
	ON_BN_CLICKED(IDC_TM_COPYNODE_OPT, OnTmCopynodeOpt)
	ON_BN_CLICKED(IDC_TM_COPYATTR_OPT, OnTmCopyattrOpt)
	ON_BN_CLICKED(IDC_TM_DUPLICATE_TOL, OnTmDuplicateTol)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_ROT_ANGLE_SPIN, OnDeltaposTmRotAngleSpin)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_MATL_INC_SPIN, OnDeltaposTmIncSpin)
	ON_BN_CLICKED(IDC_TM_INTST_TOL, OnTmIntstTol)
	ON_BN_CLICKED(IDC_TM_AXIS_Y, OnTmAxisRadio)
	ON_BN_CLICKED(IDC_TM_AXIS_Z, OnTmAxisRadio)
	ON_BN_CLICKED(IDC_TM_MOVE_RADIO, OnTmCopyMoveRadio)
	ON_BN_CLICKED(IDC_TM_AXIS_2POINT, OnTmAxisRadio)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_SECT_INC_SPIN, OnDeltaposTmIncSpin)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_THIK_INC_SPIN, OnDeltaposTmIncSpin)
	ON_BN_CLICKED(IDC_TM_NTIME_RDO_EQUAL, OnChangeAngleType)
	ON_BN_CLICKED(IDC_TM_NTIME_RDO_UNEQUAL, OnChangeAngleType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemRotateDlg message handlers

BOOL CElemRotateDlg::OnInitDialog() 
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
	m_wndRotAngle.SetWindowText(_T("0")); 
	m_wnd2ndPoint.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0)); 
	m_wnd1stPoint.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wndIterNum.SetRange(0, SHRT_MAX);
	m_wndIterNum.SetWindowText(_T("1"));
	m_wndIterNum.SetInteger(TRUE);
	m_wndMatlInc.SetRange(SHRT_MIN, SHRT_MAX);
	m_wndMatlInc.SetWindowText(_T("0"));
	m_wndSectInc.SetRange(SHRT_MIN, SHRT_MAX);
	m_wndSectInc.SetWindowText(_T("0"));
	m_wndThikInc.SetRange(SHRT_MIN, SHRT_MAX);
	m_wndThikInc.SetWindowText(_T("0"));

	m_wndAngleStatic.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_wndUEAngleUnit.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_wndDistStatic3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDistStatic2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDistStatic1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDistAxialUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

//#ifdef _MDEMO
//  m_bBetaAngle = FALSE;
//  CButton* pTempWnd = (CButton*)GetDlgItem(IDC_TM_BETA_ANGLE_CHECK);
//  pTempWnd->EnableWindow(FALSE);
//  pTempWnd->SetCheck(0);
//#endif

#ifdef _MVIL
	CtrlEnableDisable(m_aThik, FALSE);
#endif

	AlignControl();
	OnChangeAngleType();

	return FALSE;  // return TRUE unless you set the focus to a control
	               // EXCEPTION: OCX Property Pages should return FALSE
}

void CElemRotateDlg::OnDeltaposTmNtimesSpin(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CElemRotateDlg::OnDeltaposTmRotAngleSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString csAngle;
	double dblAngle;
	CFormulaEdit* pwndAngle = (CFormulaEdit*)GetDlgItem(IDC_TM_ROT_ANGLE_EDIT);
	pwndAngle->GetEditValue(dblAngle);

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

void CElemRotateDlg::OnDeltaposTmIncSpin(NMHDR* pNMHDR, LRESULT* pResult) 
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


BOOL CElemRotateDlg::GetNTimes(int &NTimes)
{
	CString strIter;
	m_wndIterNum.GetWindowText(strIter);

	return GetNumberOfTimes(strIter,NTimes);

}

BOOL CElemRotateDlg::GetFirstPoint(double &Px, double &Py, double &Pz)
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

BOOL CElemRotateDlg::GetSecondPoint(double &Px, double &Py, double &Pz)
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


BOOL CElemRotateDlg::GetRotAngle(double &RotAng)
{
	CString strRot;
	m_wndRotAngle.GetWindowText(strRot);
	return GetFloatNumber(strRot,RotAng);
}


BOOL CElemRotateDlg::GetRotAngle(_ElemRotate &ER)
{
	ASSERT(m_nAngleType==0 || m_nAngleType==1);

	CString strRot;
	double RotAng;
	ER.m_arAngle.RemoveAll();

	if(m_nAngleType)  // Unequal Angle Type
	{
		m_edtUEAngle.GetWindowText(strRot);
		if(!GetUEDistance(strRot, ER.m_arAngle)) return FALSE;
	}
	else // Equal Angle Type
	{
	  m_wndRotAngle.GetWindowText(strRot);
		if(!GetFloatNumber(strRot,RotAng)) return FALSE;

	  for(int i=0; i<ER.m_NTimes; i++)
			ER.m_arAngle.Add(RotAng);
	}

	return TRUE;
}

BOOL CElemRotateDlg::GetDistanceDrDz(double &Dr, double &Dz)
{
	CString strRadial, strAxial;
	m_wndDist.GetWindowText(strRadial);
	m_wndDistAxialEdit.GetWindowText(strAxial);

	BOOL bGet = GetFloatNumber(strRadial, Dr);
	bGet = bGet && GetFloatNumber(strAxial, Dz);
	
	return bGet;
}


BOOL CElemRotateDlg::GetParameter(_ElemRotate &ER)
{
	UpdateData(TRUE);
	
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	ER.m_bIsCopy = m_nCopyOrMove == 0;
	ER.m_nAxis   = m_nRotAxis;  // 0 : X , 1 : Y , 2 : Z , 3 : 2Point

	if(!GetNTimes(ER.m_NTimes)) return FALSE;

	if(!GetRotAngle(ER.m_AngleOfRotDEG)) return FALSE;  // Move

	if(!GetRotAngle(ER)) return FALSE;  // Copy
	
	if(!GetDistanceDrDz(ER.m_DistR,ER.m_DistZ)) return FALSE;
	
	if(GetFirstPoint(ER.m_FirstP_UX,ER.m_FirstP_UY,ER.m_FirstP_UZ))
	{
		pIGM->GetWcsPos(ER.m_FirstP_UX,ER.m_FirstP_UY,ER.m_FirstP_UZ,
										ER.m_FirstP_WX,ER.m_FirstP_WY,ER.m_FirstP_WZ);
	}
	else
		return FALSE;
	
	switch(m_nRotAxis)
	{
	case 0:
		ER.m_AxisV_Ux = 1.0;
		ER.m_AxisV_Uy = 0.0;
		ER.m_AxisV_Uz = 0.0;
		pIGM->GetWcsVector(ER.m_AxisV_Ux,ER.m_AxisV_Uy,ER.m_AxisV_Uz,
											 ER.m_AxisV_Wx,ER.m_AxisV_Wy,ER.m_AxisV_Wz);
		break;
	case 1:
		ER.m_AxisV_Ux = 0.0;
		ER.m_AxisV_Uy = 1.0;
		ER.m_AxisV_Uz = 0.0;
		pIGM->GetWcsVector(ER.m_AxisV_Ux,ER.m_AxisV_Uy,ER.m_AxisV_Uz,
											 ER.m_AxisV_Wx,ER.m_AxisV_Wy,ER.m_AxisV_Wz);
		break;
	case 2:
		ER.m_AxisV_Ux = 0.0;
		ER.m_AxisV_Uy = 0.0;
		ER.m_AxisV_Uz = 1.0;
		pIGM->GetWcsVector(ER.m_AxisV_Ux,ER.m_AxisV_Uy,ER.m_AxisV_Uz,
											 ER.m_AxisV_Wx,ER.m_AxisV_Wy,ER.m_AxisV_Wz);
		break;
	case 3:
		{
			if(GetSecondPoint(ER.m_SecondP_UX,ER.m_SecondP_UY,ER.m_SecondP_UZ))
			{
				pIGM->GetWcsPos(ER.m_SecondP_UX,ER.m_SecondP_UY,ER.m_SecondP_UZ,
												ER.m_SecondP_WX,ER.m_SecondP_WY,ER.m_SecondP_WZ);
				
				ER.m_AxisV_Ux = ER.m_SecondP_UX - ER.m_FirstP_UX;
				ER.m_AxisV_Uy = ER.m_SecondP_UY - ER.m_FirstP_UY;
				ER.m_AxisV_Uz = ER.m_SecondP_UZ - ER.m_FirstP_UZ;

				double VL;
				
				VL = ER.m_AxisV_Ux*ER.m_AxisV_Ux +  ER.m_AxisV_Uy*ER.m_AxisV_Uy + 
						 ER.m_AxisV_Uz*ER.m_AxisV_Uz;

				VL = sqrt(VL);

				if(pIGM->IsZero(VL))
				{
					GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Rotation_axis_vector_is_zer));
					return FALSE;
				}

				ER.m_AxisV_Ux = ER.m_AxisV_Ux / VL;
				ER.m_AxisV_Uy = ER.m_AxisV_Uy / VL;
				ER.m_AxisV_Uz = ER.m_AxisV_Uz / VL;

				pIGM->GetWcsVector(ER.m_AxisV_Ux,ER.m_AxisV_Uy,ER.m_AxisV_Uz,
													 ER.m_AxisV_Wx,ER.m_AxisV_Wy,ER.m_AxisV_Wz);
			}
			else
				return FALSE;
		}
		break;
	default:
		break;
	}
	
	ER.m_bCopyNodeAttr        = m_bCopyNodeAttr;
	ER.m_bCopyElemAttr        = m_bCopyElemAttr;

	if (!m_wndMatlInc.GetEditValue(ER.m_nMatlInc))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_material_increment_));
		return FALSE;
	}
	if (!m_wndSectInc.GetEditValue(ER.m_nSectInc))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_section_increment_v));
		return FALSE;
	}
	if (!m_wndThikInc.GetEditValue(ER.m_nThikInc))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_thickness_increment));
		return FALSE;
	}

	ER.m_bMatlIncRep = (m_wndMatlIncRep.GetCheck() == 1) ? TRUE : FALSE;
	ER.m_bSectIncRep = (m_wndSectIncRep.GetCheck() == 1) ? TRUE : FALSE;
	ER.m_bThikIncRep = (m_wndThikIncRep.GetCheck() == 1) ? TRUE : FALSE;

	ER.m_bIntstNode = m_bIntstNode;
	ER.m_bIntstElem = m_bIntstElem;

	ER.m_bBetaAngle = m_bBetaAngle;

	return TRUE;
}

/*
BOOL CDataCtrl::RotateElem_Move(CArray<T_ELEM_K, T_ELEM_K>&	rKey, double angle,	double dr, double dz, T_AXIS_VECTOR	Axis)
BOOL CDataCtrl::RotateElem_Copy(CArray<T_ELEM_K, T_ELEM_K>&	rKey, int nCopy, double	angle, double dr, double dz,
								T_AXIS_VECTOR Axis,	BOOL bCopyData)
*/

void CElemRotateDlg::MoveElem(_ElemRotate& ER)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<unsigned int, unsigned int> NKeyArray;
	
	if(pIGM->GetSelectedElemKeyList(NKeyArray))
	{
		T_AXIS_VECTOR AxisV;
		
		AxisV.dblOrgX = ER.m_FirstP_WX;
		AxisV.dblOrgY = ER.m_FirstP_WY;
		AxisV.dblOrgZ = ER.m_FirstP_WZ;
		AxisV.dblVecX = ER.m_AxisV_Wx;
		AxisV.dblVecY = ER.m_AxisV_Wy;
		AxisV.dblVecZ = ER.m_AxisV_Wz;
		
		BOOL bRotated = pDoc->m_pDataCtrl->RotateElem_Move(NKeyArray,
										ER.m_AngleOfRotDEG, ER.m_DistR,ER.m_DistZ,AxisV,
										ER.m_bBetaAngle);
		if (bRotated) pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
	else
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_element_));
	}
}

void CElemRotateDlg::CopyElem(_ElemRotate& ER)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<unsigned int, unsigned int> NKeyArray;
	
	if(pIGM->GetSelectedElemKeyList(NKeyArray))
	{
		T_AXIS_VECTOR AxisV;
		
		AxisV.dblOrgX = ER.m_FirstP_WX;
		AxisV.dblOrgY = ER.m_FirstP_WY;
		AxisV.dblOrgZ = ER.m_FirstP_WZ;
		AxisV.dblVecX = ER.m_AxisV_Wx;
		AxisV.dblVecY = ER.m_AxisV_Wy;
		AxisV.dblVecZ = ER.m_AxisV_Wz;
		
/*    BOOL bRotated = pDoc->m_pDataCtrl->RotateElem_Copy(NKeyArray, 
										ER.m_NTimes,ER.m_AngleOfRotDEG,ER.m_DistR, ER.m_DistZ,
								    AxisV, ER.m_bCopyNodeAttr, ER.m_bCopyElemAttr,
										ER.m_bBetaAngle, 
										ER.m_nMatlInc, ER.m_bMatlIncRep, 
										ER.m_nSectInc, ER.m_bSectIncRep, 
										ER.m_nThikInc, ER.m_bThikIncRep, 
										ER.m_bIntstNode, ER.m_bIntstElem); */

		BOOL bRotated = pDoc->m_pDataCtrl->RotateElem_Copy(NKeyArray, 
										ER.m_arAngle, ER.m_DistR, ER.m_DistZ,
								    AxisV, ER.m_bCopyNodeAttr, ER.m_bCopyElemAttr,
										ER.m_bBetaAngle, 
										ER.m_nMatlInc, ER.m_bMatlIncRep, 
										ER.m_nSectInc, ER.m_bSectIncRep, 
										ER.m_nThikInc, ER.m_bThikIncRep, 
										ER.m_bIntstNode, ER.m_bIntstElem); 

		if (bRotated) pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
	else
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_element_));
	}
}

void CElemRotateDlg::OnTmAxisRadio() 
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

void CElemRotateDlg::OnTmCopyMoveRadio() 
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

#ifdef _MVIL
	CtrlEnableDisable(m_aThik, FALSE);
#endif
}

void CElemRotateDlg::Execute()
{
	_ElemRotate ER;
	GetParameter(ER);
	if(ER.m_bIsCopy)
	{
	 CopyElem(ER);
	}
	else
	{
	 MoveElem(ER);
	}
}

void CElemRotateDlg::OnTmCopynodeOpt() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_COPYATTR_NODE,0));
}


void CElemRotateDlg::OnTmCopyattrOpt() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_COPYATTR_ELEM,0));
}

void CElemRotateDlg::OnTmDuplicateTol() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_MERGETOL,0));
}

void CElemRotateDlg::OnTmIntstTol() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_INTERSECTTOL,0));	
}

void CElemRotateDlg::OnChangeAngleType() 
{
	UpdateData(TRUE);

	CDlgUtil::CtrlShowHide(this, m_arEqualCtrl, m_nAngleType == 0);	
	CDlgUtil::CtrlShowHide(this, m_arUnEqualCtrl, m_nAngleType == 1);	
}

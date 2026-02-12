// ElemRenumberDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ElemRenumberDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_FlagCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "StructElementDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElemRenumberDlg dialog

CElemRenumberDlg::CElemRenumberDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CElemRenumberDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElemRenumberDlg)
	m_NumTarget = 1;
	m_SelType = 1;
	m_tmStartNode = _T("1");
	m_tmStartElement = _T("1");
	m_nRotAxis = 0;
	m_nPolAxis = 0;
	//}}AFX_DATA_INIT

	m_aCtrlCartesian.Add(IDC_TM_RENUM_CARTESIAN_SORT_PREF_FRM);
	m_aCtrlCartesian.Add(IDC_TM_RENUM_CARTESIAN_SORT_ORDER_TXT);
	m_aCtrlCartesian.Add(IDC_TM_RENUM_CARTESIAN_GLOBAL_AXIS_TXT);
	m_aCtrlCartesian.Add(IDC_TM_RENUM_CARTESIAN_1ST_TXT);
	m_aCtrlCartesian.Add(IDC_TM_RENUM_CARTESIAN_2ND_TXT);
	m_aCtrlCartesian.Add(IDC_TM_RENUM_CARTESIAN_3RD_TXT);
	m_aCtrlCartesian.Add(IDC_TM_RENUM_CARTESIAN_1ST);
	m_aCtrlCartesian.Add(IDC_TM_RENUM_CARTESIAN_2ND);
	m_aCtrlCartesian.Add(IDC_TM_RENUM_CARTESIAN_3RD);
	
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_SORT_PREF_FRM);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_SORT_ORDER_TXT);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_ITEM_TXT);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_1ST_TXT);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_2ND_TXT);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_3RD_TXT);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_1ST);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_2ND);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_3RD);

	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_SORT_REF_AXIS_FRM);  
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_GLOBAL_AXIS_RDO);  
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_TWO_POINTS_RDO);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_REF_AXIS_CMB);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_1ST_POINT_TXT);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_2ND_POINT_TXT);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_1ST_POINT);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_2ND_POINT);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_1ST_POINT_UNIT);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_2ND_POINT_UNIT);

	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_SORT_REF_AXIS_FRM2);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_GLOBAL_AXIS_RDO2);  
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_TWO_POINTS_RDO2);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_REF_AXIS_CMB2);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_1ST_POINT_TXT2);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_2ND_POINT_TXT2);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_1ST_POINT2);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_2ND_POINT2);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_1ST_POINT_UNIT2);
	m_aCtrlCylinder.Add(IDC_TM_RENUM_CYLINDER_2ND_POINT_UNIT2);  
	m_aCtrlCylinder.Add(IDC_TM_PICTURE);

	m_aCtrlRefAxisGlobal.Add(IDC_TM_RENUM_CYLINDER_REF_AXIS_CMB);
	m_aCtrlRefAxisTwoPnt.Add(IDC_TM_RENUM_CYLINDER_1ST_POINT);
	m_aCtrlRefAxisTwoPnt.Add(IDC_TM_RENUM_CYLINDER_2ND_POINT);

	m_aCtrlPolAxisGlobal.Add(IDC_TM_RENUM_CYLINDER_REF_AXIS_CMB2);
	m_aCtrlPolAxisTwoPnt.Add(IDC_TM_RENUM_CYLINDER_1ST_POINT2);
	m_aCtrlPolAxisTwoPnt.Add(IDC_TM_RENUM_CYLINDER_2ND_POINT2);

	m_aCtrlButton.Add(IDC_TM_EXECUTE);
	m_aCtrlButton.Add(IDC_TM_CLOSE);

	m_szCmb[0] = _LSX((+)X);
	m_szCmb[1] = _LSX((-)X);
	m_szCmb[2] = _LSX((+)Y);
	m_szCmb[3] = _LSX((-)Y);
	m_szCmb[4] = _LSX((+)Z);
	m_szCmb[5] = _LSX((-)Z);
}


void CElemRenumberDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemRenumberDlg)
	DDX_Control(pDX, IDC_TM_RENUM_CYLINDER_1ST_POINT_UNIT, m_wnd1stPointUnit);
	DDX_Control(pDX, IDC_TM_RENUM_CYLINDER_2ND_POINT_UNIT, m_wnd2ndPointUnit);
	DDX_Control(pDX, IDC_TM_RENUM_CYLINDER_1ST_POINT_UNIT2, m_wnd1stPointUnit2);
	DDX_Control(pDX, IDC_TM_RENUM_CYLINDER_2ND_POINT_UNIT2, m_wnd2ndPointUnit2);
	DDX_Control(pDX, IDC_TM_RENUM_SORT_METHOD_CMB, m_SortMethodCmb);    
	DDX_Control(pDX, IDC_TM_RENUM_CYLINDER_REF_AXIS_CMB, m_RefAxisCmb); 
	DDX_Control(pDX, IDC_TM_RENUM_CYLINDER_REF_AXIS_CMB2, m_PolAxisCmb);   
	DDX_Control(pDX, IDC_TM_RENUM_CYLINDER_1ST, m_CynOrderCmb1);
	DDX_Control(pDX, IDC_TM_RENUM_CYLINDER_2ND, m_CynOrderCmb2);
	DDX_Control(pDX, IDC_TM_RENUM_CYLINDER_3RD, m_CynOrderCmb3);
	DDX_Control(pDX, IDC_TM_RENUM_CARTESIAN_3RD, m_CarOrderCmb3);
	DDX_Control(pDX, IDC_TM_RENUM_CARTESIAN_2ND, m_CarOrderCmb2);
	DDX_Control(pDX, IDC_TM_RENUM_CARTESIAN_1ST, m_CarOrderCmb1);
 	DDX_Control(pDX, IDC_TM_RENUM_CYLINDER_1ST_POINT, m_wndRot1stPoint);
	DDX_Control(pDX, IDC_TM_RENUM_CYLINDER_2ND_POINT, m_wndRot2ndPoint);
	DDX_Control(pDX, IDC_TM_RENUM_CYLINDER_1ST_POINT2, m_wndPol1stPoint);
	DDX_Control(pDX, IDC_TM_RENUM_CYLINDER_2ND_POINT2, m_wndPol2ndPoint);
	DDX_Radio(pDX, IDC_TM_RENUM_NODE, m_NumTarget);
	DDX_Radio(pDX, IDC_TM_SELECT_ALL, m_SelType);
	DDX_Text(pDX, IDC_TM_START_NODE, m_tmStartNode);
	DDX_Text(pDX, IDC_TM_START_ELEMENT, m_tmStartElement);
	DDX_Radio(pDX, IDC_TM_RENUM_CYLINDER_GLOBAL_AXIS_RDO, m_nRotAxis);
	DDX_Radio(pDX, IDC_TM_RENUM_CYLINDER_GLOBAL_AXIS_RDO2, m_nPolAxis);
	DDX_Control(pDX, IDC_TM_PICTURE, m_wndCylinderCoordPic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CElemRenumberDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CElemRenumberDlg)
	ON_CBN_SELCHANGE(IDC_TM_RENUM_CYLINDER_REF_AXIS_CMB, OnSelchangeCynAxisOrder)	
	ON_CBN_SELCHANGE(IDC_TM_RENUM_CYLINDER_1ST, OnSelchangeCynOrder1st)
	ON_CBN_SELCHANGE(IDC_TM_RENUM_CYLINDER_2ND, OnSelchangeCynOrder2nd)
	ON_CBN_SELCHANGE(IDC_TM_RENUM_CARTESIAN_1ST, OnSelchangeCarOrder1st)
	ON_CBN_SELCHANGE(IDC_TM_RENUM_CARTESIAN_2ND, OnSelchangeCarOrder2nd)
	ON_BN_CLICKED(IDC_TM_RENUM_NODE, OnTmRenumTarget)
	ON_CBN_SELCHANGE(IDC_TM_RENUM_SORT_METHOD_CMB, OnSelchangeSortMethodCmb)
	ON_BN_CLICKED(IDC_TM_RENUM_ELEMENT, OnTmRenumTarget)
	ON_BN_CLICKED(IDC_TM_RENUM_ELEMNODE, OnTmRenumTarget)
	ON_BN_CLICKED(IDC_TM_RENUM_CYLINDER_GLOBAL_AXIS_RDO, OnTmCylinderAxisRdo)
	ON_BN_CLICKED(IDC_TM_RENUM_CYLINDER_GLOBAL_AXIS_RDO2, OnTmCylinderAxisRdo)
	ON_BN_CLICKED(IDC_TM_RENUM_CYLINDER_TWO_POINTS_RDO, OnTmCylinderAxisRdo)
	ON_BN_CLICKED(IDC_TM_RENUM_CYLINDER_TWO_POINTS_RDO2, OnTmCylinderAxisRdo)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemRenumberDlg message handlers

BOOL CElemRenumberDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();
	
	m_NumTarget = 1;
	m_SelType = 1;
	m_tmStartNode = _T("1");
	m_tmStartElement = _T("1");
	m_nRotAxis = 0;
	m_nPolAxis = 0;

	m_wndRot1stPoint.SetAttUcsPos();
	m_wndRot1stPoint.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndRot2ndPoint.SetAttUcsPos();
	m_wndRot2ndPoint.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndRot1stPoint.SetNextLink(&m_wndRot2ndPoint);
	m_wndRot2ndPoint.SetNextLink(NULL);
	m_wndRot1stPoint.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0)); 
	m_wndRot2ndPoint.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));  
	
	m_wndPol1stPoint.SetAttUcsPos();
	m_wndPol1stPoint.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndPol2ndPoint.SetAttUcsPos();
	m_wndPol2ndPoint.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndPol1stPoint.SetNextLink(&m_wndPol2ndPoint);
	m_wndPol2ndPoint.SetNextLink(NULL);
	m_wndPol1stPoint.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0)); 
	m_wndPol2ndPoint.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));  

	m_wnd1stPointUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wnd2ndPointUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wnd1stPointUnit2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wnd2ndPointUnit2.SetUnitType(D_UNITSYS_BASE_LENGTH);

	AlignControl();      
	SetInitCombString();   
	SetEditControl();	

	CString strPicPath = _T("SVG\\illustration\\Dialog\\tm_renum.svg");
	m_wndCylinderCoordPic.SetImage(strPicPath);

	auto BtnEx = GetDlgItem(IDC_TM_EXECUTE);
	if (BtnEx)
		BtnEx->ShowWindow(SW_HIDE);
		
	auto BtnClose = GetDlgItem(IDC_TM_CLOSE);
	if (BtnClose)
		BtnClose->ShowWindow(SW_HIDE);


	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CElemRenumberDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;	
	
	GetDlgItem(IDC_TM_RENUM_CYLINDER_SORT_REF_AXIS_FRM)->GetWindowRect(rToMove);
	GetDlgItem(IDC_TM_RENUM_CARTESIAN_SORT_PREF_FRM)->GetWindowRect(rRef);
	nDistY = rRef.top - rToMove.top;

	CDlgUtil::CtrlMoveDistY(this, m_aCtrlCylinder, nDistY);
	/**
	UINT aFirstCtrlID[] = {IDC_TM_RENUM_CARTESIAN_SORT_PREF_FRM};
	CArray<UINT, UINT> *aCtrlSet[] = {&m_aCtrlCylinder};

	GetDlgItem(IDC_TM_RENUM_CYLINDER_SORT_REF_AXIS_FRM)->GetWindowRect(rRef);
	for (int i = 0; i < sizeof(aFirstCtrlID)/sizeof(UINT); i++)
	{
		GetDlgItem(aFirstCtrlID[i])->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, *aCtrlSet[i], nDistY);
	}
	**/
	
	// resize self size -> Height가 큰 Cylinder상태의 Size로 맞추어 준다.
	//CWnd* pWndLast = GetDlgItem(IDC_TM_RENUM_CYLINDER_OPTION_FRM);
	CWnd* pWndLast = GetDlgItem(IDC_TM_PICTURE);
	CRect rectLast, rectExecute;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	GetDlgItem(IDC_TM_EXECUTE)->GetWindowRect(rectExecute);
	r.bottom = rectLast.bottom + rectExecute.Height();
	MoveWindow(r);
}

void CElemRenumberDlg::SetInitCombString()
{
	m_CarOrderCmb1.AddString(m_szCmb[0]);
	m_CarOrderCmb1.AddString(m_szCmb[1]);
	m_CarOrderCmb1.AddString(m_szCmb[2]);
	m_CarOrderCmb1.AddString(m_szCmb[3]);
	m_CarOrderCmb1.AddString(m_szCmb[4]);
	m_CarOrderCmb1.AddString(m_szCmb[5]);
	m_CarOrderCmb1.SetCurSel(4);

	m_CarOrderCmb2.AddString(m_szCmb[0]);
	m_CarOrderCmb2.AddString(m_szCmb[1]);
	m_CarOrderCmb2.AddString(m_szCmb[2]);
	m_CarOrderCmb2.AddString(m_szCmb[3]);
	m_CarOrderCmb2.SetCurSel(2);

	m_CarOrderCmb3.AddString(m_szCmb[0]);
	m_CarOrderCmb3.AddString(m_szCmb[1]);
	m_CarOrderCmb3.SetCurSel(0);

	m_SortMethodCmb.AddString(_LS(IDS_TM_RENUMBER_CARTESIAN_COORDINATES));
	m_SortMethodCmb.AddString(_LS(IDS_TM_RENUMBER_CYLINDRICAL_COORDINATES));
	m_SortMethodCmb.SetCurSel(0);

	m_RefAxisCmb.AddString(m_szCmb[0]);
	m_RefAxisCmb.AddString(m_szCmb[1]);
	m_RefAxisCmb.AddString(m_szCmb[2]);
	m_RefAxisCmb.AddString(m_szCmb[3]);
	m_RefAxisCmb.AddString(m_szCmb[4]);
	m_RefAxisCmb.AddString(m_szCmb[5]);
	m_RefAxisCmb.SetCurSel(4);

	m_PolAxisCmb.AddString(m_szCmb[0]);
	m_PolAxisCmb.AddString(m_szCmb[1]);
	m_PolAxisCmb.AddString(m_szCmb[2]);  
	m_PolAxisCmb.AddString(m_szCmb[3]);
	m_PolAxisCmb.SetCurSel(0);  

	m_CynOrderCmb1.AddString(_LS(IDS_TM_RENUMBER_RADIUS));
	m_CynOrderCmb1.AddString(_LS(IDS_TM_RENUMBER_ROTATING_ANGLE));
	m_CynOrderCmb1.AddString(_LS(IDS_TM_RENUMBER_HEIGHT));
	m_CynOrderCmb1.SetCurSel(2);

	m_CynOrderCmb2.AddString(_LS(IDS_TM_RENUMBER_RADIUS));
	m_CynOrderCmb2.AddString(_LS(IDS_TM_RENUMBER_ROTATING_ANGLE));  
	m_CynOrderCmb2.SetCurSel(1);

	m_CynOrderCmb3.AddString(_LS(IDS_TM_RENUMBER_RADIUS));
	m_CynOrderCmb3.SetCurSel(0);
}

void CElemRenumberDlg::OnTmRenumTarget() 
{		
	UpdateData(TRUE);
	SetEditControl();	
}

void CElemRenumberDlg::OnSelchangeSortMethodCmb() 
{  
	SetFrameSize();
	SetEditControl();
}

void CElemRenumberDlg::SetFrameSize()
{
	int nSortMethod = m_SortMethodCmb.GetCurSel();

	CRect rectRef, rectToMove, rectParent, rectButton;
	CWnd* pWnd = GetDlgItem(IDC_TM_RENUM_CYLINDER_OPTION_FRM);
	pWnd->GetWindowRect(rectRef);
	if (nSortMethod == 0)
		GetDlgItem(IDC_TM_RENUM_CARTESIAN_SORT_PREF_FRM)->GetWindowRect(rectToMove);
	else
		GetDlgItem(IDC_TM_PICTURE)->GetWindowRect(rectToMove);

	int nHeight = rectToMove.bottom - rectRef.top + globalUtils.ScaleByDPI(5);
	rectRef.bottom = rectRef.top + nHeight;
	pWnd->GetParent()->ScreenToClient(rectRef);   //Client Coordinate
	pWnd->MoveWindow(rectRef);

	//Button변경
	GetDlgItem(IDC_TM_EXECUTE)->GetWindowRect(rectButton);
	GetDlgItem(IDC_TM_RENUM_CYLINDER_OPTION_FRM)->GetWindowRect(rectToMove);
	int nY = rectToMove.bottom + globalUtils.ScaleByDPI(5) - rectButton.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlButton, nY);

	//Dialog Size변경
	GetWindowRect(rectParent);
	int nDlgHeight = rectRef.bottom + globalUtils.ScaleByDPI(5) + rectButton.Height();
	MoveWindow(0, 0, rectParent.Width(), nDlgHeight);
	((CStructElementDlg*)(GetParent()->GetParent()))->SetHolderSize(nDlgHeight);
}

void CElemRenumberDlg::OnTmCylinderAxisRdo() 
{
	UpdateData(TRUE);
	SetEditControl();
}

void CElemRenumberDlg::SetEditControl()
{
	CEdit* pEdit;

	switch(m_NumTarget)
	{
		case 0:
			pEdit = (CEdit *)this->GetDlgItem(IDC_TM_START_NODE);
			pEdit->EnableWindow(TRUE);
			pEdit = (CEdit *)this->GetDlgItem(IDC_TM_START_ELEMENT);
			pEdit->EnableWindow(FALSE);
			break;
		case 1:
			pEdit = (CEdit *)this->GetDlgItem(IDC_TM_START_NODE);
			pEdit->EnableWindow(FALSE);
			pEdit = (CEdit *)this->GetDlgItem(IDC_TM_START_ELEMENT);
			pEdit->EnableWindow(TRUE);
			break;
		case 2:
			pEdit = (CEdit *)this->GetDlgItem(IDC_TM_START_NODE);
			pEdit->EnableWindow(TRUE);
			pEdit = (CEdit *)this->GetDlgItem(IDC_TM_START_ELEMENT);
			pEdit->EnableWindow(TRUE);
			break;
	}

	int nSortMethod = m_SortMethodCmb.GetCurSel();
	CDlgUtil::CtrlShowHide(this, m_aCtrlCartesian, nSortMethod == 0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlCylinder, nSortMethod == 1);  

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlRefAxisGlobal, m_nRotAxis == 0);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlRefAxisTwoPnt, m_nRotAxis == 1);

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlPolAxisGlobal, m_nPolAxis == 0);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlPolAxisTwoPnt, m_nPolAxis == 1);  

	UpdateData(FALSE);
	Invalidate();
}

void CElemRenumberDlg::OnSelchangeCarOrder1st() 
{
	m_CarOrderCmb2.ResetContent();
	int i=0, nCurSel = m_CarOrderCmb1.GetCurSel();
	switch(nCurSel)
	{
	case 0:
	case 1:
		m_CarOrderCmb2.AddString(m_szCmb[2]);
		m_CarOrderCmb2.AddString(m_szCmb[3]);
		m_CarOrderCmb2.AddString(m_szCmb[4]);
		m_CarOrderCmb2.AddString(m_szCmb[5]);
		break;
	case 2:
	case 3:
		m_CarOrderCmb2.AddString(m_szCmb[0]);
		m_CarOrderCmb2.AddString(m_szCmb[1]);
		m_CarOrderCmb2.AddString(m_szCmb[4]);
		m_CarOrderCmb2.AddString(m_szCmb[5]);
		break;
	case 4:
	case 5:
		m_CarOrderCmb2.AddString(m_szCmb[0]);
		m_CarOrderCmb2.AddString(m_szCmb[1]);
		m_CarOrderCmb2.AddString(m_szCmb[2]);
		m_CarOrderCmb2.AddString(m_szCmb[3]);
		break;
	default:  ASSERT(0); return;
	}
	m_CarOrderCmb2.SetCurSel(0);	
	OnSelchangeCarOrder2nd();	
}

void CElemRenumberDlg::OnSelchangeCarOrder2nd() 
{  
	CString strTemp;
	m_CarOrderCmb3.ResetContent();
	
	switch(m_CarOrderCmb2.GetCurSel())
	{
	case 0:
	case 1:
		m_CarOrderCmb2.GetLBText(2, strTemp);
		m_CarOrderCmb3.AddString(strTemp);    
		m_CarOrderCmb2.GetLBText(3, strTemp);
		m_CarOrderCmb3.AddString(strTemp);    
		break;
	case 2:
	case 3:
		m_CarOrderCmb2.GetLBText(0, strTemp);
		m_CarOrderCmb3.AddString(strTemp);    
		m_CarOrderCmb2.GetLBText(1, strTemp);
		m_CarOrderCmb3.AddString(strTemp);    
		break;
	default : ASSERT(0); return;
	}
	m_CarOrderCmb3.SetCurSel(0);	
}

void CElemRenumberDlg::OnSelchangeCynOrder1st()
{
	CString sOrder[3]={_LS(IDS_TM_RENUMBER_RADIUS), 
										 _LS(IDS_TM_RENUMBER_ROTATING_ANGLE),
										 _LS(IDS_TM_RENUMBER_HEIGHT)};	
	m_CynOrderCmb2.ResetContent();
	for(int i = 0; i < 3 ; i++)
	{
	  if(m_CynOrderCmb1.GetCurSel() != i) m_CynOrderCmb2.AddString(sOrder[i]);	  
	}
	m_CynOrderCmb2.SetCurSel(0);	
	OnSelchangeCynOrder2nd();	
}

void CElemRenumberDlg::OnSelchangeCynOrder2nd()
{  
	CString strTemp;
	m_CynOrderCmb3.ResetContent();
	
	switch(m_CynOrderCmb2.GetCurSel())
	{
		case 0:
			m_CynOrderCmb2.GetLBText(1, strTemp);
		  m_CynOrderCmb3.AddString(strTemp);    
			break;
		case 1:
			m_CynOrderCmb2.GetLBText(0, strTemp);
		  m_CynOrderCmb3.AddString(strTemp);
			break;
	}
	m_CynOrderCmb3.SetCurSel(0);	
}

void CElemRenumberDlg::OnSelchangeCynAxisOrder()
{
	m_PolAxisCmb.ResetContent();
	int i=0, nCurSel = m_RefAxisCmb.GetCurSel();
	switch(nCurSel)
	{
	case 0:
	case 1:
		m_PolAxisCmb.AddString(m_szCmb[2]);
		m_PolAxisCmb.AddString(m_szCmb[3]);
		m_PolAxisCmb.AddString(m_szCmb[4]);
		m_PolAxisCmb.AddString(m_szCmb[5]);
		break;
	case 2:
	case 3:
		m_PolAxisCmb.AddString(m_szCmb[0]);
		m_PolAxisCmb.AddString(m_szCmb[1]);
		m_PolAxisCmb.AddString(m_szCmb[4]);
		m_PolAxisCmb.AddString(m_szCmb[5]);
		break;
	case 4:
	case 5:
		m_PolAxisCmb.AddString(m_szCmb[0]);
		m_PolAxisCmb.AddString(m_szCmb[1]);
		m_PolAxisCmb.AddString(m_szCmb[2]);
		m_PolAxisCmb.AddString(m_szCmb[3]);
		break;
	default:  ASSERT(0); return;  
	}
	m_PolAxisCmb.SetCurSel(0);
}


BOOL CElemRenumberDlg::GetRot1stPoint(double &Px, double &Py, double &Pz)
{
	if(m_wndRot1stPoint.IsKeyPressed())
	{
		CString strPos;
		m_wndRot1stPoint.GetWindowText(strPos);
		return GetPosition(strPos,Px,Py,Pz);
	}
	else
	{
		double wx, wy, wz;
		m_wndRot1stPoint.GetCurMousePosReal(wx,wy,wz,Px,Py,Pz);
	}
	return TRUE;
}

BOOL CElemRenumberDlg::GetRot2ndPoint(double &Px, double &Py, double &Pz)
{
	if(m_wndRot2ndPoint.IsKeyPressed())
	{
		CString strPos;
		m_wndRot2ndPoint.GetWindowText(strPos);
		return GetPosition(strPos,Px,Py,Pz);
	}
	else
	{
		double wx, wy, wz;
		m_wndRot2ndPoint.GetCurMousePosReal(wx,wy,wz,Px,Py,Pz);
	}
	return TRUE;
}

BOOL CElemRenumberDlg::GetPol1stPoint(double &Px, double &Py, double &Pz)
{
	if(m_wndPol1stPoint.IsKeyPressed())
	{
		CString strPos;
		m_wndPol1stPoint.GetWindowText(strPos);
		return GetPosition(strPos,Px,Py,Pz);
	}
	else
	{
		double wx, wy, wz;
		m_wndPol1stPoint.GetCurMousePosReal(wx,wy,wz,Px,Py,Pz);
	}
	return TRUE;
}

BOOL CElemRenumberDlg::GetPol2ndPoint(double &Px, double &Py, double &Pz)
{
	if(m_wndPol2ndPoint.IsKeyPressed())
	{
		CString strPos;
		m_wndPol2ndPoint.GetWindowText(strPos);
		return GetPosition(strPos,Px,Py,Pz);
	}
	else
	{
		double wx, wy, wz;
		m_wndPol2ndPoint.GetCurMousePosReal(wx,wy,wz,Px,Py,Pz);
	}
	return TRUE;
}

BOOL CElemRenumberDlg::CheckOrthogonal()
{
	double rot1[3], rot2[3], pol1[3], pol2[3], VecRot[3], nVecRot[3], VecPol[3], nVecPol[3];;
	CString strTemp;
	
	// Reference Rotating Axis
	if(m_nRotAxis == 0)  // Global Axis
	{ 
		rot1[0] = 0, rot1[1] = 0, rot1[2] = 0;
		m_RefAxisCmb.GetLBText(m_RefAxisCmb.GetCurSel(), strTemp);
		if      (strTemp == m_szCmb[0]) { rot2[0] = 1.;  rot2[1] = 0.;  rot2[2] = 0.; }
		else if (strTemp == m_szCmb[1]) { rot2[0] = -1.; rot2[1] = 0.;  rot2[2] = 0.; }
		else if (strTemp == m_szCmb[2]) { rot2[0] = 0.;  rot2[1] = 1.;  rot2[2] = 0.; }
		else if (strTemp == m_szCmb[3]) { rot2[0] = 0.;  rot2[1] = -1.; rot2[2] = 0.; }
		else if (strTemp == m_szCmb[4]) { rot2[0] = 0.;  rot2[1] = 0.;  rot2[2] = 1.; }
		else if (strTemp == m_szCmb[5]) { rot2[0] = 0.;  rot2[1] = 0.;  rot2[2] = -1.;}
		else                            { ASSERT(0); rot2[0] = 0.; rot2[1] = 0.; rot2[2] = 0.;};
	}
	else  // Axis defined by two points
	{
		GetRot1stPoint(rot1[0], rot1[1], rot1[2]);
		GetRot2ndPoint(rot2[0], rot2[1], rot2[2]);    
	}
	VecRot[0] = rot2[0] - rot1[0];
	VecRot[1] = rot2[1] - rot1[1];
	VecRot[2] = rot2[2] - rot1[2];
	if(!CMathFunc::mathNormalize(VecRot, nVecRot)) 
	{    
		GSaveHistoryFormatNF(_LS(IDS_WG_TM_RENUMBER_ROT_TWO_POINTS_CLOSE));  //_T("The two points defining rotating axis are too close!")
		return FALSE;
	}
	// Reference Polar Axis
	if(m_nPolAxis == 0)  // Global Axis
	{ 
		pol1[0] = 0, pol1[1] = 0, pol1[2] = 0; 
		m_PolAxisCmb.GetLBText(m_PolAxisCmb.GetCurSel(), strTemp);
		if      (strTemp == m_szCmb[0]) { pol2[0] = 1.;  pol2[1] = 0.;  pol2[2] = 0.; }
		else if (strTemp == m_szCmb[1]) { pol2[0] = -1.; pol2[1] = 0.;  pol2[2] = 0.; }
		else if (strTemp == m_szCmb[2]) { pol2[0] = 0.;  pol2[1] = 1.;  pol2[2] = 0.; }
		else if (strTemp == m_szCmb[3]) { pol2[0] = 0.;  pol2[1] = -1.; pol2[2] = 0.; }
		else if (strTemp == m_szCmb[4]) { pol2[0] = 0.;  pol2[1] = 0.;  pol2[2] = 1.; }
		else if (strTemp == m_szCmb[5]) { pol2[0] = 0.;  pol2[1] = 0.;  pol2[2] = -1.;}
		else                            { ASSERT(0); pol2[0] = 0.; pol2[1] = 0.; pol2[2] = 0.;};
	}
	else  // Axis defined by two points
	{
		GetPol1stPoint(pol1[0], pol1[1], pol1[2]);
		GetPol2ndPoint(pol2[0], pol2[1], pol2[2]);    
	}
	VecPol[0] = pol2[0] - pol1[0];
	VecPol[1] = pol2[1] - pol1[1];
	VecPol[2] = pol2[2] - pol1[2];
	if(!CMathFunc::mathNormalize(VecPol, nVecPol))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TM_RENUMBER_POL_TWO_POINTS_CLOSE)); //_T("The two points defining polar axis are too close!")
		return FALSE;
	}
	
	double dTolerance= m_pDoc->m_pFlagCtrl->GetGeneralCoordinateToleranceCurrentUnit();
	double dDistance;  
	if(!CMathFunc::mathIntersectLine(rot1, rot2, pol1, pol2, dTolerance, dDistance, m_org)) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TM_RENUMBER_POL_ROL_NOT_INTERSECT)); // _T("The rotating axis and polar axis does not intersect each other!")
		return FALSE;
	}

	double dAngle = CMathFunc::mathCrossAngle(nVecRot, nVecPol);
	double dTol = 0.001;
	if(fabs(dAngle - 90) > dTol) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TM_RENUMBER_POL_ROL_NOT_ORTHOGONAL)); // _T("The rotating axis and polar axis are not orthogonal!")
		return FALSE;
	}
	else 
	{
		for(int i = 0; i < 3; i++)
		{
			m_rot[i] = m_org[i] + nVecRot[i];      
			m_pol[i] = m_org[i] + nVecPol[i];
		}
		return TRUE;
	}
}

void CElemRenumberDlg::Execute()
{
	int nSortMethod = m_SortMethodCmb.GetCurSel();
	switch(nSortMethod)
	{
		case 0: ExecuteCartesian();   break; // Cartesian Coordinates
		case 1: ExecuteCylindrical(); break; // Cylindrical Coordinates  
	}
}

void CElemRenumberDlg::ExecuteCartesian()
{
 	UpdateData(TRUE);
	CArray<T_NODE_K, T_NODE_K> aNodeKey;
	CArray<T_ELEM_K, T_ELEM_K> aElemKey;
	BOOL SelectCheckNode=FALSE;
	BOOL SelectCheckElem=FALSE;
	BOOL SelectCheckLkky=FALSE;

	// SortOrder 를 정한다.
	//CString strTemp1,strTemp2, strTemp3;
	int OrderTemp = GetOrderOpt();
	/*m_CarOrderCmb1.GetLBText(m_CarOrderCmb1.GetCurSel(), strTemp1);
	m_CarOrderCmb2.GetLBText(m_CarOrderCmb2.GetCurSel(), strTemp2);
	m_CarOrderCmb3.GetLBText(m_CarOrderCmb3.GetCurSel(), strTemp3);
	OrderTemp ;
	if((strTemp1 == _T("X")) && (strTemp2 == _T("Y"))) OrderTemp = 1;
	else if((strTemp1 == _T("X")) && (strTemp2 == _T("Z"))) OrderTemp = 2;
	else if((strTemp1 == _T("Y")) && (strTemp2 == _T("X"))) OrderTemp = 3;
	else if((strTemp1 == _T("Y")) && (strTemp2 == _T("Z"))) OrderTemp = 4;
	else if((strTemp1 == _T("Z")) && (strTemp2 == _T("X"))) OrderTemp = 5;
	else OrderTemp = 6;*/

	StartNode = (T_NODE_K)_ttoi(m_tmStartNode);
	StartElem = (T_ELEM_K)_ttoi(m_tmStartElement);
	// Numbering Target 을 체크한다. Node인지 Elem인지 Both인지.
	// 동시에 Select Type을 체크. All 인지 By Select 인지.
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	switch(m_NumTarget)
	{
	  case 0:
		  if(m_SelType == 0)  //노드만 전부 Renumbering
		  {
			  m_pDoc->m_pAttrCtrl->GetNodeKeyList(aNodeKey);
			  m_pDoc->m_pDataCtrl->Renumbering(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp);
		  }
		  else  // 노드만 선택한 부분을 Renumbering
		  {
			  if(!pIGM->GetSelectedNodeKeyList(aNodeKey,TRUE))
			  {
				  GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_)); return; 
			  }
			  if(m_pDoc->m_pDataCtrl->Renumbering(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp))
				  m_pDoc->m_pViewCtrl->UnselectAll(NULL);
		  }
		  break;
	  case 1:
		  if(m_SelType == 0)  //Element만 전부 Renumbering
		  {
			  m_pDoc->m_pAttrCtrl->GetElemKeyList(aElemKey);
			  m_pDoc->m_pDataCtrl->Renumbering(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp);
		  }
		  else  // Element만 선택한 부분을 Renumbering
		  {
			  if(!pIGM->GetSelectedElemKeyList(aElemKey,TRUE))
			  {
				  GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_element_)); return; 
			  }
			  if(m_pDoc->m_pDataCtrl->Renumbering(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp))
				  m_pDoc->m_pViewCtrl->UnselectAll(NULL);
		  }
		  break;
	  case 2:
		  if(m_SelType == 0)  //Element와 Node를 전부 Renumbering
		  {
			  m_pDoc->m_pAttrCtrl->GetNodeKeyList(aNodeKey);
			  m_pDoc->m_pAttrCtrl->GetElemKeyList(aElemKey,FALSE);
			  m_pDoc->m_pDataCtrl->Renumbering(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp);
		  }
		  else  // Element 와 Node의 선택한 부분을 Renumbering
		  {
			  if(!pIGM->GetSelectedNodeKeyList(aNodeKey,TRUE)) SelectCheckNode = TRUE;
			  if(!pIGM->GetSelectedElemKeyList(aElemKey,TRUE)) SelectCheckElem = TRUE;
			  if(SelectCheckNode && SelectCheckElem)
			  {
				  GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_and_elemen)); return; 
			  }
			  if(m_pDoc->m_pDataCtrl->Renumbering(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp))
				  m_pDoc->m_pViewCtrl->UnselectAll(NULL);
		  }
	    break;
	}
}

void CElemRenumberDlg::ExecuteCylindrical()
{
	UpdateData(TRUE);
	CArray<T_NODE_K, T_NODE_K> aNodeKey;
	CArray<T_ELEM_K, T_ELEM_K> aElemKey;
	BOOL SelectCheckNode=FALSE;
	BOOL SelectCheckElem=FALSE;
	BOOL SelectCheckLkky=FALSE;

	if(!CheckOrthogonal()) return;
		
	// SortOrder 를 정한다.
	CString strTemp1,strTemp2;
	int OrderTemp;
	m_CynOrderCmb1.GetLBText(m_CynOrderCmb1.GetCurSel(), strTemp1);
	m_CynOrderCmb2.GetLBText(m_CynOrderCmb2.GetCurSel(), strTemp2);
	if((strTemp1 == _LS(IDS_TM_RENUMBER_HEIGHT)) && (strTemp2 == _LS(IDS_TM_RENUMBER_ROTATING_ANGLE)))      OrderTemp = 6;
	else if((strTemp1 == _LS(IDS_TM_RENUMBER_HEIGHT)) && (strTemp2 == _LS(IDS_TM_RENUMBER_RADIUS)))         OrderTemp = 5;
	else if((strTemp1 == _LS(IDS_TM_RENUMBER_ROTATING_ANGLE)) && (strTemp2 == _LS(IDS_TM_RENUMBER_HEIGHT))) OrderTemp = 4;
	else if((strTemp1 == _LS(IDS_TM_RENUMBER_ROTATING_ANGLE)) && (strTemp2 == _LS(IDS_TM_RENUMBER_RADIUS))) OrderTemp = 3;
	else if((strTemp1 == _LS(IDS_TM_RENUMBER_RADIUS)) && (strTemp2 == _LS(IDS_TM_RENUMBER_HEIGHT)))         OrderTemp = 2;
	else OrderTemp = 1;

	StartNode = (T_NODE_K)_ttoi(m_tmStartNode);
	StartElem = (T_ELEM_K)_ttoi(m_tmStartElement);
	// Numbering Target 을 체크한다. Node인지 Elem인지 Both인지.
	// 동시에 Select Type을 체크. All 인지 By Selection 인지.
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	switch(m_NumTarget)
	{
	  case 0:  // Node
		  if(m_SelType == 0)  // All
		  {
			  m_pDoc->m_pAttrCtrl->GetNodeKeyList(aNodeKey);
			  m_pDoc->m_pDataCtrl->RenumberingCyn(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp,m_org,m_rot,m_pol);
		  }
		  else  // By Selection
		  {
			  if(!pIGM->GetSelectedNodeKeyList(aNodeKey,TRUE))
			  {
				  GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_)); return; 
			  }
			  if(m_pDoc->m_pDataCtrl->RenumberingCyn(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp,m_org,m_rot,m_pol))
				  m_pDoc->m_pViewCtrl->UnselectAll(NULL);
		  }
		  break;
	  case 1:  // Element
		  if(m_SelType == 0)  // All
		  {
			  m_pDoc->m_pAttrCtrl->GetElemKeyList(aElemKey);
			  m_pDoc->m_pDataCtrl->RenumberingCyn(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp,m_org,m_rot,m_pol);
		  }
		  else  // By Selection
		  {
			  if(!pIGM->GetSelectedElemKeyList(aElemKey,TRUE))
			  {
				  GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_element_)); return; 
			  }
			  if(m_pDoc->m_pDataCtrl->RenumberingCyn(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp,m_org,m_rot,m_pol))
				  m_pDoc->m_pViewCtrl->UnselectAll(NULL);
		  }
		  break;
	  case 2:  // Node and Element
		  if(m_SelType == 0)  // All
		  {
			  m_pDoc->m_pAttrCtrl->GetNodeKeyList(aNodeKey);
			  m_pDoc->m_pAttrCtrl->GetElemKeyList(aElemKey);
			  m_pDoc->m_pDataCtrl->RenumberingCyn(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp,m_org,m_rot,m_pol);
		  }
		  else  // By Selection
		  {
			  if(!pIGM->GetSelectedNodeKeyList(aNodeKey,TRUE)) SelectCheckNode = TRUE;
			  if(!pIGM->GetSelectedElemKeyList(aElemKey,TRUE)) SelectCheckElem = TRUE;
			  if(SelectCheckNode && SelectCheckElem)
			  {
				  GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_and_elemen)); return; 
			  }
			  if(m_pDoc->m_pDataCtrl->RenumberingCyn(aNodeKey,aElemKey,StartNode,StartElem,OrderTemp,m_org,m_rot,m_pol))
				  m_pDoc->m_pViewCtrl->UnselectAll(NULL);
		  }
	    break;
	}
}

void CElemRenumberDlg::OnTmExecute()
{
	((CStructElementDlg*)(GetParent()->GetParent()))->OnTmExecAndClose(TRUE);
}

void CElemRenumberDlg::OnTmClose()
{
	((CStructElementDlg*)(GetParent()->GetParent()))->OnTmExecAndClose(FALSE);
}

int CElemRenumberDlg::GetOrderOpt()
{
	int nRet = 0;

	int nCursel1 = m_CarOrderCmb1.GetCurSel();
	int nCursel2 = m_CarOrderCmb2.GetCurSel();
	int nCursel3 = m_CarOrderCmb3.GetCurSel();
	nRet = nCursel1*8 + nCursel2*2 + nCursel3;

	return nRet;
}

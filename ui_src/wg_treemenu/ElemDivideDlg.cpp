// ElemDivideDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ElemDivideDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_dbRes2.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElemDivideDlg dialog


CElemDivideDlg::CElemDivideDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CElemDivideDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElemDivideDlg)
	m_bMergeDupNode = TRUE;
	m_bSubDivideFrame = TRUE;
	m_nOption = 0;
	m_nElemType = 0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_ElemTypeCtrl.Add(IDC_TM_ET_FRAME);
	m_ElemTypeCtrl.Add(IDC_TM_ET_PLANE);
	m_ElemTypeCtrl.Add(IDC_TM_ET_SOLID);
	m_ElemTypeCtrl.Add(IDC_TM_ET_WALL);

	m_EqCtrl.Add(IDC_TM_NUMOFDIV_STATIC_X);
	m_EqCtrl.Add(IDC_TM_NUMOFDIV_X);
	//m_EqCtrl.Add(IDC_TM_NUMOFDIV_SPIN_X);
	m_EqCtrl.Add(IDC_TM_NUMOFDIV_STATIC_Y);
	m_EqCtrl.Add(IDC_TM_NUMOFDIV_Y);
	//m_EqCtrl.Add(IDC_TM_NUMOFDIV_SPIN_Y);
	m_EqCtrl.Add(IDC_TM_NUMOFDIV_STATIC_Z);
	m_EqCtrl.Add(IDC_TM_NUMOFDIV_Z);
	//m_EqCtrl.Add(IDC_TM_NUMOFDIV_SPIN_Z);

	m_EqCtrlY.Add(IDC_TM_NUMOFDIV_STATIC_Y);
	m_EqCtrlY.Add(IDC_TM_NUMOFDIV_Y);
	//m_EqCtrlY.Add(IDC_TM_NUMOFDIV_SPIN_Y);

	m_EqCtrlZ.Add(IDC_TM_NUMOFDIV_STATIC_Z);
	m_EqCtrlZ.Add(IDC_TM_NUMOFDIV_Z);
	//m_EqCtrlZ.Add(IDC_TM_NUMOFDIV_SPIN_Z);

	m_UeqCtrl.Add(IDC_TM_UEQ_STATIC_X);
	m_UeqCtrl.Add(IDC_TM_UEQ_EDIT_X);
	m_UeqCtrl.Add(IDC_TM_UEQ_EDIT_UNIT_X);
	m_UeqCtrl.Add(IDC_TM_UEQ_STATIC_Y);
	m_UeqCtrl.Add(IDC_TM_UEQ_EDIT_Y);
	m_UeqCtrl.Add(IDC_TM_UEQ_EDIT_UNIT_Y);
	m_UeqCtrl.Add(IDC_TM_UEQ_STATIC_Z);
	m_UeqCtrl.Add(IDC_TM_UEQ_EDIT_Z);
	m_UeqCtrl.Add(IDC_TM_UEQ_EDIT_UNIT_Z);
	m_UeqCtrl.Add(IDC_TM_UEQ_EXAM_STATIC);

	m_UeqCtrlY.Add(IDC_TM_UEQ_STATIC_Y);
	m_UeqCtrlY.Add(IDC_TM_UEQ_EDIT_Y);
	m_UeqCtrlY.Add(IDC_TM_UEQ_EDIT_UNIT_Y);

	m_UeqCtrlZ.Add(IDC_TM_UEQ_STATIC_Z);
	m_UeqCtrlZ.Add(IDC_TM_UEQ_EDIT_Z);
	m_UeqCtrlZ.Add(IDC_TM_UEQ_EDIT_UNIT_Z);

	m_ParamCtrl.Add(IDC_TM_RATIO_STATIC_X);
	m_ParamCtrl.Add(IDC_TM_RATIO_EDIT_X);
	m_ParamCtrl.Add(IDC_TM_RATIO_STATIC_Y);
	m_ParamCtrl.Add(IDC_TM_RATIO_EDIT_Y);
	m_ParamCtrl.Add(IDC_TM_RATIO_STATIC_Z);
	m_ParamCtrl.Add(IDC_TM_RATIO_EDIT_Z);
	m_ParamCtrl.Add(IDC_TM_RATIO_EXAM_STATIC);

	m_ParamCtrlY.Add(IDC_TM_RATIO_STATIC_Y);
	m_ParamCtrlY.Add(IDC_TM_RATIO_EDIT_Y);

	m_ParamCtrlZ.Add(IDC_TM_RATIO_STATIC_Z);
	m_ParamCtrlZ.Add(IDC_TM_RATIO_EDIT_Z);

	m_PBraceCtrl.Add(IDC_TM_PB_NOD_STATIC);
	m_PBraceCtrl.Add(IDC_TM_PB_NOD_EDIT);
	//m_PBraceCtrl.Add(IDC_TM_PB_NOD_SPIN);
	m_PBraceCtrl.Add(IDC_TM_PB_EDGE_STATIC);
	m_PBraceCtrl.Add(IDC_TM_PB_EDGE_EDIT);

	m_ByNodeCtrl.Add(IDC_TM_NODE_STATIC1);
	m_ByNodeCtrl.Add(IDC_TM_NODE_STATIC2);
	m_ByNodeCtrl.Add(IDC_TM_NODE_ELEM);
	m_ByNodeCtrl.Add(IDC_TM_NODE_NODE);

	m_PlaneSolidCtrl.Add(IDC_TM_ET_PLANE);
	m_PlaneSolidCtrl.Add(IDC_TM_ET_SOLID);

	m_PatternCtrl.Add(IDC_TM_PATTERN_EDGE_STATIC);  //Added by water tree. 2000.6.30
	m_PatternCtrl.Add(IDC_TM_PATTERN_EDGE_CBO);
	m_PatternCtrl.Add(IDC_TM_PATTERN_POINT_STATIC);
	m_PatternCtrl.Add(IDC_TM_PATTERN_POINT_CBO);     
	m_PatternCtrl.Add(IDC_TM_PATTERN_PICTURE);

	m_PatternEdgeCtrl.Add(IDC_TM_PATTERN_EDGE_STATIC);
	m_PatternEdgeCtrl.Add(IDC_TM_PATTERN_EDGE_CBO);
	m_PatternPointCtrl.Add(IDC_TM_PATTERN_POINT_STATIC);
	m_PatternPointCtrl.Add(IDC_TM_PATTERN_POINT_CBO);
}

void CElemDivideDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemDivideDlg)
	DDX_Control(pDX, IDC_TM_PATTERN_POINT_CBO, m_cboPatternPoint);
	DDX_Control(pDX, IDC_TM_PATTERN_EDGE_CBO, m_cboPatternEdge);
	DDX_Control(pDX, IDC_TM_PATTERN_PICTURE, m_cboPicture);
	DDX_Control(pDX, IDC_TM_NODE_NODE, m_wndByNodeNode);
	DDX_Control(pDX, IDC_TM_NODE_ELEM, m_wndByNodeElem);
	DDX_Control(pDX, IDC_TM_PB_EDGE_EDIT, m_wndEdgeEdit);
	DDX_Control(pDX, IDC_TM_PB_NOD_EDIT, m_wndNumOfDivPB);
	DDX_Control(pDX, IDC_TM_NUMOFDIV_X, m_wndNumOfDivX);
	DDX_Control(pDX, IDC_TM_NUMOFDIV_Y, m_wndNumOfDivY);
	DDX_Control(pDX, IDC_TM_NUMOFDIV_Z, m_wndNumOfDivZ);
	DDX_Control(pDX, IDC_TM_UEQ_EDIT_UNIT_X, m_wndUeqDistUnitX);
	DDX_Control(pDX, IDC_TM_UEQ_EDIT_UNIT_Y, m_wndUeqDistUnitY);
	DDX_Control(pDX, IDC_TM_UEQ_EDIT_UNIT_Z, m_wndUeqDistUnitZ);
	DDX_Control(pDX, IDC_TM_UEQ_EDIT_X, m_wndUeqDistX);
	DDX_Control(pDX, IDC_TM_UEQ_EDIT_Y, m_wndUeqDistY);
	DDX_Control(pDX, IDC_TM_UEQ_EDIT_Z, m_wndUeqDistZ);
	DDX_Control(pDX, IDC_TM_RATIO_EDIT_X, m_wndRatioX);
	DDX_Control(pDX, IDC_TM_RATIO_EDIT_Y, m_wndRatioY);
	DDX_Control(pDX, IDC_TM_RATIO_EDIT_Z, m_wndRatioZ);
	DDX_Check(pDX, IDC_TM_MERGE_DUP_NODE, m_bMergeDupNode);
	DDX_Check(pDX, IDC_TM_SUB_DIVIDE_FRAME, m_bSubDivideFrame);
	DDX_Radio(pDX, IDC_TM_EQUAL_DIST, m_nOption);
	DDX_Radio(pDX, IDC_TM_ET_FRAME, m_nElemType);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CElemDivideDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CElemDivideDlg)
	ON_BN_CLICKED(IDC_TM_EQUAL_DIST, OnTmEqualDist)
	ON_BN_CLICKED(IDC_TM_UNEQUAL_DIST, OnTmUnequalDist)
	ON_BN_CLICKED(IDC_TM_PARAM_DIST, OnTmParamDist)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_NUMOFDIV_SPIN_X, OnDeltaposTmNumofdivSpin)
	ON_BN_CLICKED(IDC_TM_ET_FRAME, OnTmElemType)
	ON_BN_CLICKED(IDC_TM_TOLERANCE, OnTmTolerance)
	ON_BN_CLICKED(IDC_TM_PARALLEL_BRACING, OnTmParallelBracing)
	ON_BN_CLICKED(IDC_TM_BY_NODE, OnTmByNode)
	ON_BN_CLICKED(IDC_TM_BY_PATTERN, OnTmByPattern)
	ON_CBN_SELCHANGE(IDC_TM_PATTERN_PICTURE, OnSelchangeTmPatternPicture)
	ON_CBN_SELCHANGE(IDC_TM_PATTERN_EDGE_CBO, OnSelchangeTmPatternEdgeCbo)
	ON_CBN_SELCHANGE(IDC_TM_PATTERN_POINT_CBO, OnSelchangeTmPatternPointCbo)
	ON_BN_CLICKED(IDC_TM_ET_PLANE, OnTmElemType)
	ON_BN_CLICKED(IDC_TM_ET_SOLID, OnTmElemType)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_NUMOFDIV_SPIN_Y, OnDeltaposTmNumofdivSpin)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_NUMOFDIV_SPIN_Z, OnDeltaposTmNumofdivSpin)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_PB_NOD_SPIN, OnDeltaposTmNumofdivSpin)
	ON_BN_CLICKED(IDC_TM_ET_WALL, OnTmElemType)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
END_MESSAGE_MAP()

void CElemDivideDlg::CtrlEnableDisableEQ(BOOL bEnable)
{
	CtrlShowHide(m_EqCtrl, bEnable);
	CtrlEnableDisable(m_EqCtrl, bEnable);
	CtrlEnableDisable(m_EqCtrlY, bEnable && (m_nElemType==1 || m_nElemType==2));    // Planar(1), Solid(2)
	CtrlEnableDisable(m_EqCtrlZ, bEnable && (m_nElemType==2 || m_nElemType==3));    // Wall(3), Solid(2)
}

void CElemDivideDlg::CtrlEnableDisableUEQ(BOOL bEnable)
{
	CtrlShowHide(m_UeqCtrl, bEnable);
	CtrlEnableDisable(m_UeqCtrl, bEnable);
	CtrlEnableDisable(m_UeqCtrlY, bEnable && (m_nElemType==1 || m_nElemType==2));
	CtrlEnableDisable(m_UeqCtrlZ, bEnable && (m_nElemType==2 || m_nElemType==3));
}

void CElemDivideDlg::CtrlEnableDisablePUEQ(BOOL bEnable)
{
	CtrlShowHide(m_ParamCtrl, bEnable);
	CtrlEnableDisable(m_ParamCtrl, bEnable);
	CtrlEnableDisable(m_ParamCtrlY, bEnable && (m_nElemType==1 || m_nElemType==2));
	CtrlEnableDisable(m_ParamCtrlZ, bEnable && (m_nElemType==2 || m_nElemType==3));
}

void CElemDivideDlg::CtrlEnableDisablePBrace(BOOL bEnable)
{
	CtrlShowHide(m_PBraceCtrl, bEnable);
	CtrlEnableDisable(m_PBraceCtrl, bEnable);
}

void CElemDivideDlg::CtrlEnableDisableByNode(BOOL bEnable)
{
	CtrlShowHide(m_ByNodeCtrl, bEnable);
	CtrlEnableDisable(m_ByNodeCtrl, bEnable);
}

void CElemDivideDlg::CtrlEnableDisableByPattern(BOOL bEnable)
{
	CtrlShowHide(m_PatternCtrl, bEnable);
	CtrlEnableDisable(m_PatternCtrl, bEnable);
}

void CElemDivideDlg::CtrlEnableDisableOption(BOOL bEnable)
{
	CArray<UINT, UINT> aCtrl;
	aCtrl.Add(IDC_TM_PARALLEL_BRACING);
	aCtrl.Add(IDC_TM_BY_NODE);
	CtrlEnableDisable(aCtrl, bEnable);
	
	CButton* tempBtn;
	tempBtn = (CButton*)GetDlgItem(IDC_TM_BY_PATTERN);
	if(m_nElemType == 1)
		tempBtn->EnableWindow(TRUE);
	else
		tempBtn->EnableWindow(FALSE);
}

void CElemDivideDlg::AlignControls()
{
	CArray<UINT, UINT>* arCtrlArray[] = { &m_UeqCtrl, &m_ParamCtrl, &m_PBraceCtrl, &m_ByNodeCtrl, &m_PatternCtrl};
	CRect rRef, rToMove;

#ifdef _CIVIL
	#if defined( _RUS )
		CWnd* pWnd;
		pWnd = GetDlgItem(IDC_TM_ET_WALL); ASSERT(pWnd);
		pWnd->GetWindowRect(rRef);
		pWnd->ShowWindow(FALSE);

		pWnd = GetDlgItem(IDC_TM_ET_PLANE); ASSERT(pWnd);
		pWnd->GetWindowRect(rToMove);
		CSize p(0, rToMove.top - rRef.top);
		this->ScreenToClient(rToMove);
		rToMove -= p;
		pWnd->MoveWindow(rToMove, TRUE);
	#else
		CWnd* pWnd;
		pWnd = GetDlgItem(IDC_TM_ET_WALL); ASSERT(pWnd);
		pWnd->GetWindowRect(rRef);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_TM_ET_PLANE); ASSERT(pWnd);
		pWnd->GetWindowRect(rToMove);
		CSize p(rToMove.left - rRef.left, 0);
		this->ScreenToClient(rToMove);
		rToMove -= p;
		pWnd->MoveWindow(rToMove, TRUE);
	#endif
#endif

	int nDistY;
	GetDlgItem(m_EqCtrl.GetAt(0))->GetWindowRect(rRef);
	for (int i = 0; i < 5; i++)
	{
		GetDlgItem(arCtrlArray[i]->GetAt(0))->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CtrlMoveDistY(*(arCtrlArray[i]), nDistY);
	}

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_TM_LAST_CTRL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+5;
	CWnd* pParent = GetParent();
	pParent->ScreenToClient(r);
	MoveWindow(r);
}

/////////////////////////////////////////////////////////////////////////////
// CElemDivideDlg message handlers
BOOL CElemDivideDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	AlignControls();

	m_wndEdgeEdit.SetAttElemList();
	m_wndEdgeEdit.SetLButtonDownNotifyWindow(this);
	m_wndEdgeEdit.SetEnterNotifyWindow(this);
	m_wndEdgeEdit.SetModeToUse(MOUSEEDIT_USE_GET_ELEM_LIST);
	m_wndEdgeEdit.SetMaxElemKeyNum(2);

	m_wndByNodeElem.SetAttElemList();
	m_wndByNodeElem.SetLButtonDownNotifyWindow(this);
	m_wndByNodeElem.SetEnterNotifyWindow(this);
	m_wndByNodeElem.SetModeToUse(MOUSEEDIT_USE_GET_ELEM_LIST);
	m_wndByNodeElem.SetMaxElemKeyNum(1);

	m_wndByNodeNode.SetAttNodeList();
	m_wndByNodeNode.SetLButtonDownNotifyWindow(this);
	m_wndByNodeNode.SetEnterNotifyWindow(this);
	m_wndByNodeNode.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_wndByNodeNode.SetMaxNodeKeyNum(1);

	m_wndByNodeElem.SetNextLink(&m_wndByNodeNode);
	m_wndByNodeNode.SetNextLink(NULL);

	m_wndNumOfDivX.SetRange(1, SHRT_MAX);
	m_wndNumOfDivX.SetWindowText(_T("2"));
	m_wndNumOfDivX.SetInteger(TRUE);
	m_wndNumOfDivY.SetRange(1, SHRT_MAX);
	m_wndNumOfDivY.SetWindowText(_T("2"));
	m_wndNumOfDivY.SetInteger(TRUE);
	m_wndNumOfDivZ.SetRange(1, SHRT_MAX);
	m_wndNumOfDivZ.SetWindowText(_T("2"));
	m_wndNumOfDivZ.SetInteger(TRUE);
	m_wndNumOfDivPB.SetRange(1, SHRT_MAX);
	m_wndNumOfDivPB.SetWindowText(_T("2"));
	m_wndNumOfDivPB.SetInteger(TRUE);

	m_wndUeqDistUnitX.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndUeqDistUnitY.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndUeqDistUnitZ.SetUnitType(D_UNITSYS_BASE_LENGTH);

	InitValue();

	CtrlEnableDisableEQ(m_nOption == 0);
	CtrlEnableDisableUEQ(m_nOption == 1);
	CtrlEnableDisablePUEQ(m_nOption == 2);
	CtrlEnableDisablePBrace(m_nOption == 3);
	CtrlEnableDisableByNode(m_nOption == 4);
	CtrlEnableDisableByPattern(m_nOption == 5);
	CtrlEnableDisableOption(m_nElemType == 0);

	GotoDlgCtrl(&m_wndNumOfDivX);

#ifdef _MVIL
	CtrlEnableDisable(m_PlaneSolidCtrl, FALSE);
#endif

	// Added by water tree.
	SetupImageList();
	m_cboPicture.SetCurSel(0);

	if(m_nOption == 5)
	{
		m_pDoc->m_pViewCtrl->SetDivPatternMode(TRUE);
		OnSelchangeTmPatternPicture();
//		m_pDoc->m_pViewCtrl->SetDivPatternMode(TRUE);
//		m_pDoc->m_pViewCtrl->SetDivPatternParam(m_cboPicture.GetCurSel(), GetEdgeorPoint());
	}

	OnTmElemType();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CElemDivideDlg::OnTmEqualDist() 
{
	// TODO: Add your control notification handler code here
	CtrlEnableDisableEQ(TRUE);
	CtrlEnableDisableUEQ(FALSE);
	CtrlEnableDisablePUEQ(FALSE);
	CtrlEnableDisablePBrace(FALSE);
	CtrlEnableDisableByNode(FALSE);
	CtrlEnableDisableByPattern(FALSE);

	m_pDoc->m_pViewCtrl->SetDivPatternMode(FALSE);
}

void CElemDivideDlg::OnTmUnequalDist() 
{
	// TODO: Add your control notification handler code here
	CtrlEnableDisableEQ(FALSE);
	CtrlEnableDisableUEQ(TRUE);
	CtrlEnableDisablePUEQ(FALSE);
	CtrlEnableDisablePBrace(FALSE);
	CtrlEnableDisableByNode(FALSE);
	CtrlEnableDisableByPattern(FALSE);

	m_pDoc->m_pViewCtrl->SetDivPatternMode(FALSE);
}

void CElemDivideDlg::OnTmParamDist() 
{
	// TODO: Add your control notification handler code here
	CtrlEnableDisableEQ(FALSE);
	CtrlEnableDisableUEQ(FALSE);
	CtrlEnableDisablePUEQ(TRUE);	
	CtrlEnableDisablePBrace(FALSE);
	CtrlEnableDisableByNode(FALSE);
	CtrlEnableDisableByPattern(FALSE);

	m_pDoc->m_pViewCtrl->SetDivPatternMode(FALSE);
}

void CElemDivideDlg::OnTmParallelBracing() 
{
	// TODO: Add your control notification handler code here
	CtrlEnableDisableEQ(FALSE);
	CtrlEnableDisableUEQ(FALSE);
	CtrlEnableDisablePUEQ(FALSE);	
	CtrlEnableDisablePBrace(TRUE);
	CtrlEnableDisableByNode(FALSE);
	CtrlEnableDisableByPattern(FALSE);

	m_pDoc->m_pViewCtrl->SetDivPatternMode(FALSE);
}

void CElemDivideDlg::OnTmByNode() 
{
	// TODO: Add your control notification handler code here
	CtrlEnableDisableEQ(FALSE);
	CtrlEnableDisableUEQ(FALSE);
	CtrlEnableDisablePUEQ(FALSE);	
	CtrlEnableDisablePBrace(FALSE);
	CtrlEnableDisableByNode(TRUE);	
	CtrlEnableDisableByPattern(FALSE);

	m_pDoc->m_pViewCtrl->SetDivPatternMode(FALSE);
}

void CElemDivideDlg::OnTmByPattern() 
{
	// TODO: Add your control notification handler code here
	CtrlEnableDisableEQ(FALSE);
	CtrlEnableDisableUEQ(FALSE);
	CtrlEnableDisablePUEQ(FALSE);	
	CtrlEnableDisablePBrace(FALSE);
	CtrlEnableDisableByNode(FALSE);	
	CtrlEnableDisableByPattern(TRUE);	
	m_pDoc->m_pViewCtrl->SetDivPatternMode(TRUE);
	OnSelchangeTmPatternPicture();

//	m_pDoc->m_pViewCtrl->SetDivPatternMode(TRUE);
//	m_pDoc->m_pViewCtrl->SetDivPatternParam(m_cboPicture.GetCurSel(), GetEdgeorPoint());
}

void CElemDivideDlg::OnTmElemType()
{
	int oldElemType = m_nElemType;
	
	UpdateData(TRUE); // get m_nElemType, m_nOption
	CDlgUtil::CtrlRadioGetCheck(this, m_ElemTypeCtrl, m_nElemType);

	switch(oldElemType)
	{
		case 0:                   // Frame
			if (m_nElemType == 1 || m_nElemType == 3)
			{
				if (m_nOption == 3 || m_nOption == 4)
				{
					m_nOption = 0;
					UpdateData(FALSE);
				}
			}
			else
			{
				if (m_nOption == 3 || m_nOption == 4 || m_nOption == 5)
				{
					m_nOption = 0;
					UpdateData(FALSE);
				}
			}
			break;
		case 1:                     // Plane
			if (m_nOption == 5)
			{
				m_nOption = 0;
				UpdateData(FALSE);
			}
			break;
	}

	CtrlEnableDisableEQ(m_nOption == 0);
	CtrlEnableDisableUEQ(m_nOption == 1);
	CtrlEnableDisablePUEQ(m_nOption == 2);
	CtrlEnableDisablePBrace(m_nOption == 3);
	CtrlEnableDisableByNode(m_nOption == 4);
	CtrlEnableDisableByPattern(m_nOption == 5);
	CtrlEnableDisableOption(m_nElemType == 0);

	if(m_nOption == 5) 
	{
		m_pDoc->m_pViewCtrl->SetDivPatternMode(TRUE);
		m_pDoc->m_pViewCtrl->SetDivPatternParam(m_cboPicture.GetCurSel(), GetEdgeorPoint());
	}
	else
	{
		m_pDoc->m_pViewCtrl->SetDivPatternMode(FALSE);
	}

	if(m_nElemType == 0) //Frame
		GetDlgItem(IDC_TM_SUB_DIVIDE_FRAME)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_TM_SUB_DIVIDE_FRAME)->EnableWindow(TRUE);
}

void CElemDivideDlg::OnDeltaposTmNumofdivSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	const MSG* pMsg = GetCurrentMessage();
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString csDiv;
	int nDiv;
	
	CFormulaEditSpin* pNumOfDiv;
	switch(LOWORD(pMsg->wParam))
	{
	case IDC_TM_NUMOFDIV_SPIN_X: pNumOfDiv = &m_wndNumOfDivX; break;
	case IDC_TM_NUMOFDIV_SPIN_Y: pNumOfDiv = &m_wndNumOfDivY; break;
	case IDC_TM_NUMOFDIV_SPIN_Z: pNumOfDiv = &m_wndNumOfDivZ; break;
	case IDC_TM_PB_NOD_SPIN    : pNumOfDiv = &m_wndNumOfDivPB; break;
	}

	if (!GetNumOfDiv(pNumOfDiv, nDiv)) { *pResult = 0; return; }
	nDiv -= pNMUpDown->iDelta;
	if (nDiv < 1) nDiv = 1;
	
	csDiv.Format(_LS(IDS_WG_TREEMENU__d),nDiv);
	
	pNumOfDiv->SetWindowText(csDiv);
	
	*pResult = 0;
}

BOOL CElemDivideDlg::GetNumOfDiv(CEdit* pEdit, int &nDiv)
{
	CString csDiv;
	pEdit->GetWindowText(csDiv);
	return GetINumber(csDiv, nDiv);
}

BOOL CElemDivideDlg::GetUeqDistance(CEdit* pEdit, CDblArray &aDistList)
{
	CString csDistList;
	pEdit->GetWindowText(csDistList);
	return GetUEDistance(csDistList, aDistList);
}

BOOL CElemDivideDlg::GetParamRatio(CEdit* pEdit, CDblArray &aRatioList)
{
	CString csRatioList;
	pEdit->GetWindowText(csRatioList);
	return GetUEDistanceABS(csRatioList, aRatioList);
}

BOOL CElemDivideDlg::GetParameter(_ElemDivide& ED)
{
	UpdateData(TRUE);
	CDlgUtil::CtrlRadioGetCheck(this, m_ElemTypeCtrl, m_nElemType);
	
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	ED.m_nElemType = m_nElemType;
	ED.m_nOption = m_nOption;

	// 입력항의 갯수 : frame타입은 1개, plane은 2개, solid는 3개
	int nCount = m_nElemType + 1;
	switch(m_nOption)
	{
	case 0: // Equal Distance
		{
			CEdit* pEdit[] = {&m_wndNumOfDivX, &m_wndNumOfDivY, &m_wndNumOfDivZ};
			if (m_nElemType == 3)   // Wall
			{
				if (!GetNumOfDiv(pEdit[0], ED.m_nDivision[0])) return FALSE;
				ED.m_nDivision[0] -= 1; 
				if (!GetNumOfDiv(pEdit[2], ED.m_nDivision[2])) return FALSE;
				ED.m_nDivision[2] -= 1; 
			}
			else
			{
				for (int i = 0; i < nCount; i++)
				{
					if (!GetNumOfDiv(pEdit[i], ED.m_nDivision[i])) return FALSE;
					ED.m_nDivision[i] -= 1; // division에서 1 뺀다(user의 혼란 제거위해)
				}
			}
		}
		break;
	case 1: // Unequal Distance
		{
			CEdit* pEdit[] = {&m_wndUeqDistX, &m_wndUeqDistY, &m_wndUeqDistZ};
			if (m_nElemType == 3)   // Wall
			{
				if (!GetUeqDistance(pEdit[0], ED.m_List[0])) return FALSE;
				if (!GetUeqDistance(pEdit[2], ED.m_List[2])) return FALSE;
			}
			else
			{
				for (int i = 0; i < nCount; i++)
					if (!GetUeqDistance(pEdit[i], ED.m_List[i])) return FALSE;
			}
		}
		break;
	case 2: // Param Ratio
		{
			CEdit* pEdit[] = {&m_wndRatioX, &m_wndRatioY, &m_wndRatioZ};
			if (m_nElemType == 3)   // Wall
			{
				if (!GetParamRatio(pEdit[0], ED.m_List[0])) return FALSE;
				if (!GetParamRatio(pEdit[2], ED.m_List[2])) return FALSE;
			}
			else
			{
				for (int i = 0; i < nCount; i++)
					if (!GetParamRatio(pEdit[i], ED.m_List[i])) return FALSE;
			}
		}
		break;
	case 3: // Parallel Bracing
		{
			if (!GetNumOfDiv(&m_wndNumOfDivPB, ED.m_nDivision[0])) return FALSE;
			ED.m_nDivision[0] -= 1; // division에서 1 뺀다(user의 혼란 제거위해)
			CString csNodeList;
			m_wndEdgeEdit.GetWindowText(csNodeList);
			if (!GetNodeList(csNodeList, ED.m_aEdgeNode)) return FALSE;
			if (ED.m_aEdgeNode.GetSize() != 2) return FALSE;
		}
		break;
	case 4: // Divide by Node
		{
			CString csElemList, csNodeList;
			m_wndByNodeElem.GetWindowText(csElemList);
			m_wndByNodeNode.GetWindowText(csNodeList);

			CArray<UINT, UINT> aKey;
			if (!GetNodeList(csElemList, aKey)) return FALSE;
			if (aKey.GetSize() != 1) return FALSE;
			ED.m_nElemNo = aKey[0];

			aKey.RemoveAll();
			if (!GetNodeList(csNodeList, aKey)) return FALSE;
			if (aKey.GetSize() != 1) return FALSE;
			ED.m_nNodeNo = aKey[0];
		}
		break;
	case 5: // Divide by Pattern
		{
			ED.nPatternType = m_cboPicture.GetCurSel()+1;
			ED.nBaseEdge = m_cboPatternEdge.GetCurSel()+1;
			ED.nBasePoint = m_cboPatternPoint.GetCurSel()+1;
		}
		break;
	default: ASSERT(0);
	}

	if (m_nOption == 1 || m_nOption == 2)
	{
		int nSum=0;
		for (int i = 0; i < nCount; i++)
			nSum += ED.m_List[i].GetSize();
		if (nSum == 0) return FALSE;
	}

	ED.m_bMergeDupNode = m_bMergeDupNode;
	ED.m_bSubDivideFrame = m_bSubDivideFrame;
 
	return TRUE;
}

// Z -> X, X -> Y, Y -> Z 
void CElemDivideDlg::MakeWallDataByLocalAxis(_ElemDivide& ED)
{
	if (ED.m_nElemType != 3) return;

	int nTemp = ED.m_nDivision[0]; 
	ED.m_nDivision[0] = ED.m_nDivision[2];
	ED.m_nDivision[2] = ED.m_nDivision[1];
	ED.m_nDivision[1] = nTemp;

	CDblArray aTemp;
	
	aTemp.RemoveAll();
	for(int i=0; i<ED.m_List[0].GetSize(); i++)
		aTemp.Add(ED.m_List[0].GetAt(i));

	ED.m_List[0].RemoveAll();
	for(int i=0; i<ED.m_List[2].GetSize(); i++)
		ED.m_List[0].Add(ED.m_List[2].GetAt(i));
	 
	ED.m_List[2].RemoveAll();
	for(int i=0; i<ED.m_List[1].GetSize(); i++)
		ED.m_List[2].Add(ED.m_List[1].GetAt(i));

	ED.m_List[1].RemoveAll();
	for(int i=0; i<aTemp.GetSize(); i++)
		ED.m_List[1].Add(aTemp.GetAt(i));
}

//BOOL DivideElem_Equal(CArray<T_ELEM_K, T_ELEM_K>& rKey, int nElType, int nDivide[3], BOOL bMergeNode);  // nElType =1:Frame =2:Plane =3:Solid
//BOOL DivideElem_Unequal(CArray<T_ELEM_K, T_ELEM_K>& rKey, int nElType, int nDivide[3], CArray<double, double> arDistance[3], BOOL bParametric, BOOL bMergeNode);  // nElType =1:Frame =2:Plane =3:Solid
//BOOL DivideElem_Brace(T_ELEM_K Key1, T_ELEM_K Key2, int nDivide, BOOL bMergeNode);
void CElemDivideDlg::DivideElem(_ElemDivide& ED)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<unsigned int, unsigned int> KeyList;
	
	BOOL bSuccess = FALSE;
	BOOL bError = FALSE;
	CString strError;
	T_ELEM_D ElemD;

	pIGM->GetSelectedElemKeyList(KeyList);

	if (ED.m_nElemType == 1)
	{
		for(int i=0; i<KeyList.GetSize(); i++)
		{
			if(!pDoc->m_pAttrCtrl->GetElem(KeyList[i], ElemD)) continue;
			if(!pDoc->m_pAttrCtrl->IsPlate(ElemD.eltyp) && !pDoc->m_pAttrCtrl->IsPlstrs(ElemD.eltyp)) continue;

			if(ElemD.angle != 0.0)
			{
				strError.Format(_LS(IDS_TM_ELEM_DIVIDE_BETA_ANGLE_ERROR), KeyList[i], ElemD.angle);
				GSaveHistoryNF(strError); 
				bError = TRUE;
			}
		}

		if(bError) return;
	}

	if (ED.m_nOption == 0) // Equal Distance 
	{
		if (ED.m_nElemType == 3) MakeWallDataByLocalAxis(ED);
		
		bSuccess = pDoc->m_pDataCtrl->DivideElem_Equal(
							 KeyList, ED.m_nElemType+1, ED.m_nDivision, ED.m_bMergeDupNode, ED.m_bSubDivideFrame);
	}
	else if (ED.m_nOption == 1 || ED.m_nOption == 2) // Unequal Distance (or Parametric Unequal Distance)
	{
		BOOL bParametric = (ED.m_nOption == 1) ? FALSE : TRUE;
		for (int i = 0; i < 3; i++) ED.m_nDivision[i] = ED.m_List[i].GetSize();
		if (ED.m_nElemType == 3) MakeWallDataByLocalAxis(ED);
		
		bSuccess = pDoc->m_pDataCtrl->DivideElem_Unequal(
							 KeyList, ED.m_nElemType+1, ED.m_nDivision, ED.m_List, 
							 bParametric, ED.m_bMergeDupNode, ED.m_bSubDivideFrame);
	}
	else if (ED.m_nOption == 3) // parallel bracing
	{
		bSuccess = pDoc->m_pDataCtrl->DivideElem_Brace(
							 ED.m_aEdgeNode[0], ED.m_aEdgeNode[1],
							 ED.m_nDivision[0], ED.m_bMergeDupNode);
	}
	else if (ED.m_nOption == 4)  // Divide By Node
	{
		bSuccess = pDoc->m_pDataCtrl->DivideElem_Node(ED.m_nElemNo, ED.m_nNodeNo);
	}
	else if (ED.m_nOption == 5)  // Divide By Pattern
	{
		if(KeyList.GetSize() == 0)
		{
			GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_element_));
			return;
		}
		
		bSuccess = pDoc->m_pDataCtrl->DivideElem_Pattern(KeyList, 
																										 ED.nPatternType, 
																										 ED.nBaseEdge, 
																										 ED.nBasePoint, 
																										 ED.m_bMergeDupNode,
																										 ED.m_bSubDivideFrame);
	}

	if (bSuccess) pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CElemDivideDlg::Execute()
{
	_ElemDivide ED;

	if(GetParameter(ED))
	{
		//CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
		//I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
		DivideElem(ED);
	}
	else
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Parameter_Error____));
	}
}

void CElemDivideDlg::OnTmTolerance() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_MERGETOL,0));
}

LRESULT CElemDivideDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	// GetCurrentMessage에서 ID 구분을 할 수 없어 포커스 자동 이동을 할 수 없다.
	return 0L;
}

void CElemDivideDlg::OnSelchangeTmPatternPicture() 
{
	// TODO: Add your control notification handler code here
	m_cboPatternEdge.ResetContent();
	m_cboPatternPoint.ResetContent();

	switch(m_cboPicture.GetCurSel())
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			m_cboPatternEdge.AddString(_LS(IDS_WG_TREEMENU_Edge__1));
			m_cboPatternEdge.AddString(_LS(IDS_WG_TREEMENU_Edge__2));
			m_cboPatternEdge.AddString(_LS(IDS_WG_TREEMENU_Edge__3));
			m_cboPatternEdge.AddString(_LS(IDS_WG_TREEMENU_Edge__4));
			m_cboPatternEdge.SetCurSel(0);
			EnableDisablePatternCtrl(TRUE,FALSE);	
			break;
		case 6:
		case 7:
			m_cboPatternPoint.AddString(_LS(IDS_WG_TREEMENU_Point__1));
			m_cboPatternPoint.AddString(_LS(IDS_WG_TREEMENU_Point__2));
			m_cboPatternPoint.AddString(_LS(IDS_WG_TREEMENU_Point__3));
			m_cboPatternPoint.AddString(_LS(IDS_WG_TREEMENU_Point__4));
			m_cboPatternPoint.SetCurSel(0);
			EnableDisablePatternCtrl(FALSE,TRUE);	
			break;	
		case 8:
			EnableDisablePatternCtrl(FALSE,FALSE);				
			break;
		case 9:
		case 10:
			m_cboPatternEdge.AddString(_LS(IDS_WG_TREEMENU_Edge__1));
			m_cboPatternEdge.AddString(_LS(IDS_WG_TREEMENU_Edge__2));
			m_cboPatternEdge.AddString(_LS(IDS_WG_TREEMENU_Edge__3));
			m_cboPatternEdge.SetCurSel(0);
			EnableDisablePatternCtrl(TRUE,FALSE);	
			break;
		case 11:
		case 12:
			EnableDisablePatternCtrl(FALSE,FALSE);				
			break;
	}
	
//	m_pDoc->m_pViewCtrl->SetDivPatternMode(TRUE);
	m_pDoc->m_pViewCtrl->SetDivPatternParam(m_cboPicture.GetCurSel(), GetEdgeorPoint());
}

int CElemDivideDlg::GetEdgeorPoint() 
{
	switch(m_cboPicture.GetCurSel())
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			return m_cboPatternEdge.GetCurSel();
			break;
		case 6:
		case 7:
			return m_cboPatternPoint.GetCurSel();
			break;	
		case 8:
			return -1;
			break;
		case 9:
		case 10:
			return m_cboPatternEdge.GetCurSel();
			break;
		case 11:
		case 12:
			return -1;
			break;
	}
	return -1;
}


void CElemDivideDlg::EnableDisablePatternCtrl(BOOL flag1, BOOL flag2) //flag1 for Edge, flag2 for Point
{
	CtrlEnableDisable(m_PatternEdgeCtrl, flag1);
	CtrlEnableDisable(m_PatternPointCtrl, flag2);
}

void CElemDivideDlg::SetupImageList()
{
	UINT m_ImageList[] = { IDB_TM_DIVIDE1, IDB_TM_DIVIDE2, IDB_TM_DIVIDE3, IDB_TM_DIVIDE4, 
												 IDB_TM_DIVIDE5, IDB_TM_DIVIDE6, IDB_TM_DIVIDE7, IDB_TM_DIVIDE8,  	
												 IDB_TM_DIVIDE9, IDB_TM_DIVIDE10, IDB_TM_DIVIDE11, IDB_TM_DIVIDE12,
	                       IDB_TM_DIVIDE13 }; 	

	m_pImageList = new CImageList;
	m_pImageList->Create(41,33,ILC_COLOR8,13,0);
	CBitmap* pBitmap;
	
	for(int i=0; i<13; i++)
	{
		pBitmap = new CBitmap;
		pBitmap->LoadBitmap(m_ImageList[i]);
		m_pImageList->Add(pBitmap, RGB(0,0,0));
		delete pBitmap;
	}

	m_cboPicture.SetImageList(m_pImageList);

	COMBOBOXEXITEM cbi;
	CString str;
	int nItem;

	for(int i=0; i<13; i++)
	{
//		cbi.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_OVERLAY | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
		cbi.mask = CBEIF_IMAGE | CBEIF_OVERLAY | CBEIF_SELECTEDIMAGE;
		cbi.iItem = i;
		str.Format(_T(""));
		cbi.pszText = (LPTSTR)(LPCTSTR)str;
		cbi.cchTextMax = 0;
		cbi.iImage = i;
		cbi.iSelectedImage = i;
		cbi.iOverlay = i;
		cbi.iIndent = 0;

		nItem = m_cboPicture.InsertItem(&cbi);
	}
}

void CElemDivideDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	delete m_pImageList;
	m_pImageList = NULL;
	
	m_pDoc->m_pViewCtrl->SetDivPatternMode(FALSE);
	
	CDialog::PostNcDestroy();
}

void CElemDivideDlg::OnSelchangeTmPatternEdgeCbo() 
{
	// TODO: Add your control notification handler code here
	m_pDoc->m_pViewCtrl->SetDivPatternParam(m_cboPicture.GetCurSel(), GetEdgeorPoint());	
}

void CElemDivideDlg::OnSelchangeTmPatternPointCbo() 
{
	// TODO: Add your control notification handler code here
	m_pDoc->m_pViewCtrl->SetDivPatternParam(m_cboPicture.GetCurSel(), GetEdgeorPoint());	
}

void CElemDivideDlg::CheckMode(BOOL bCheck)
{
	m_pDoc = CDBDoc::GetDocPoint();
	InitValue();
	if((bCheck) && (m_nOption == 5))
	{
		if(m_pDoc)
		{
			m_pDoc->m_pViewCtrl->SetDivPatternMode(TRUE);
		  m_pDoc->m_pViewCtrl->SetDivPatternParam(m_cboPicture.GetCurSel(), GetEdgeorPoint());	
		}
	}
	else
	{
		if(m_pDoc)
			m_pDoc->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
}

void CElemDivideDlg::InitValue()
{
	UINT uIntType[] = { IDC_TM_ET_FRAME , IDC_TM_ET_PLANE , IDC_TM_ET_SOLID };
	UINT uIntKind[] = { IDC_TM_EQUAL_DIST, IDC_TM_UNEQUAL_DIST, IDC_TM_PARAM_DIST, 
											IDC_TM_PARALLEL_BRACING, IDC_TM_BY_NODE, IDC_TM_BY_PATTERN };

	for(int i=0 ; i < 3; i++)
	{
		if(((CButton *)GetDlgItem(uIntType[i]))->GetCheck())
			m_nElemType = i;
	}
	for(int i=0 ; i < 6; i++)
	{
		if(((CButton *)GetDlgItem(uIntKind[i]))->GetCheck())
			m_nOption = i;
	}
}

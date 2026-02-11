// NodeDivideDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "NodeDivideDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_dbRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNodeDivideDlg dialog


CNodeDivideDlg::CNodeDivideDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CNodeDivideDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNodeDivideDlg)
	m_bMergeDupNode = TRUE;
	m_nOption = 0;
	m_bIntersect = FALSE;
	//}}AFX_DATA_INIT
	m_EqCtrl.Add(IDC_TM_NUMOFDIV_STATIC);
	m_EqCtrl.Add(IDC_TM_NUMOFDIV);
	m_EqCtrl.Add(IDC_TM_NUMOFDIV_SPIN);

	m_UeqCtrl.Add(IDC_TM_UEQ_EDIT);
	m_UeqCtrl.Add(IDC_TM_UEQ_EDIT_UNIT);
	m_UeqCtrl.Add(IDC_TM_UEQ_EXAM_STATIC);

	m_ParamCtrl.Add(IDC_TM_RATIO_STATIC);
	m_ParamCtrl.Add(IDC_TM_RATIO_EDIT);
}


void CNodeDivideDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNodeDivideDlg)
	DDX_Control(pDX, IDC_TM_NUMOFDIV, m_wndNumOfDiv);
	DDX_Control(pDX, IDC_TM_UEQ_EDIT_UNIT, m_wndUeqDistUnit);
	DDX_Control(pDX, IDC_TM_UEQ_EDIT, m_wndUeqDist);
	DDX_Control(pDX, IDC_TM_RATIO_EDIT, m_wndRatio);
	DDX_Control(pDX, IDC_TM_NODE4DIVIDE, m_wndNode4Divide);
	DDX_Check(pDX, IDC_TM_MERGE_DUP_NODE, m_bMergeDupNode);
	DDX_Radio(pDX, IDC_TM_EQUAL_DIST, m_nOption);
	DDX_Check(pDX, IDC_TM_ITST_CHECK, m_bIntersect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNodeDivideDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CNodeDivideDlg)
	ON_BN_CLICKED(IDC_TM_EQUAL_DIST, OnTmEqualDist)
	ON_BN_CLICKED(IDC_TM_UNEQUAL_DIST, OnTmUnequalDist)
	ON_BN_CLICKED(IDC_TM_PARAM_DIST, OnTmParamDist)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TM_NUMOFDIV_SPIN, OnDeltaposTmNumofdivSpin)
	ON_BN_CLICKED(IDC_TM_TOLERANCE, OnTmTolerance)
	ON_BN_CLICKED(IDC_TM_INTSECT_TOL, OnTmIntsectTol)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNodeDivideDlg message handlers

BOOL CNodeDivideDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndNode4Divide.SetAttNodeList();
	m_wndNode4Divide.SetLButtonDownNotifyWindow(this);
	m_wndNode4Divide.SetEnterNotifyWindow(this);
	m_wndNode4Divide.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_wndNode4Divide.SetMaxNodeKeyNum(2);

	m_wndNumOfDiv.SetRange(1, SHRT_MAX);
	m_wndNumOfDiv.SetValue(2);
	m_wndNumOfDiv.SetInteger(TRUE);
	m_wndUeqDistUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	CtrlEnableDisable(m_EqCtrl, m_nOption == 0);
	CtrlEnableDisable(m_UeqCtrl, m_nOption == 1);
	CtrlEnableDisable(m_ParamCtrl, m_nOption == 2);

	GotoDlgCtrl(&m_wndNumOfDiv);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNodeDivideDlg::OnTmEqualDist() 
{
	// TODO: Add your control notification handler code here
	CtrlEnableDisable(m_EqCtrl, TRUE);
	CtrlEnableDisable(m_UeqCtrl, FALSE);
	CtrlEnableDisable(m_ParamCtrl, FALSE);
}

void CNodeDivideDlg::OnTmUnequalDist() 
{
	// TODO: Add your control notification handler code here
	CtrlEnableDisable(m_EqCtrl, FALSE);
	CtrlEnableDisable(m_UeqCtrl, TRUE);
	CtrlEnableDisable(m_ParamCtrl, FALSE);
}

void CNodeDivideDlg::OnTmParamDist() 
{
	// TODO: Add your control notification handler code here
	CtrlEnableDisable(m_EqCtrl, FALSE);
	CtrlEnableDisable(m_UeqCtrl, FALSE);
	CtrlEnableDisable(m_ParamCtrl, TRUE);	
}

void CNodeDivideDlg::OnDeltaposTmNumofdivSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString csDiv;
	int nDiv;
	
	if (!GetNumOfDiv(nDiv)) { *pResult = 0; return; }
	nDiv -= pNMUpDown->iDelta;
	if (nDiv < 1) nDiv = 1;
	csDiv.Format(_LS(IDS_WG_TREEMENU__d),nDiv);
	
	m_wndNumOfDiv.SetWindowText(csDiv);
	
	*pResult = 0;
}

BOOL CNodeDivideDlg::GetNumOfDiv(int &nDiv)
{
	return m_wndNumOfDiv.GetEditValue(nDiv);
}

BOOL CNodeDivideDlg::GetUeqDistance(CArray<double, double>& aDistList)
{
	CString csDistList;
	m_wndUeqDist.GetWindowText(csDistList);
	return GetUEDistance(csDistList, aDistList);
}

BOOL CNodeDivideDlg::GetParamRatio(CArray<double, double>& aRatioList)
{
	CString csRatioList;
	m_wndRatio.GetWindowText(csRatioList);
	return GetUEDistanceABS(csRatioList, aRatioList);
}

BOOL CNodeDivideDlg::GetNode4Div(int &nNode1, int &nNode2)
{
	CString csNodeList;
	m_wndNode4Divide.GetWindowText(csNodeList);

	CArray<int,int> NodeKeyList;
	
	BOOL bSuccess = GetNodeList(csNodeList,NodeKeyList);
	if (!bSuccess) return FALSE;
	if (NodeKeyList.GetSize() != 2) return FALSE;
	nNode1 = NodeKeyList[0];
	nNode2 = NodeKeyList[1];
	return TRUE;
}

BOOL CNodeDivideDlg::GetParameter(_NodeDivide& ND)
{
	UpdateData(TRUE);
	
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	if (!GetNumOfDiv(ND.m_nDivision)) return FALSE;
	ND.m_nDivision -= 1;  // division에서 1 뺀다(user의 혼란 제거위해)
	ND.m_nOption = m_nOption;
	
	if (m_nOption == 1) // Unequal Distance
		GetUeqDistance(ND.m_List);
	else if (m_nOption == 2)  // Parametric Unequal Distance
		GetParamRatio(ND.m_List);

	if (m_nOption != 0 && ND.m_List.GetSize() <= 0) return FALSE;

	if (!GetNode4Div(ND.m_nNode1, ND.m_nNode2)) return FALSE;

	ND.m_bMergeDupNode = m_bMergeDupNode;
	ND.m_bIntersect    = m_bIntersect;
 
	return TRUE;
}

//	BOOL DivideNode_Equal(T_NODE_K Node1, T_NODE_K Node2, int nDivide, BOOL bMergeNode);
//	BOOL DivideNode_Unequal(T_NODE_K Node1, T_NODE_K Node2, int nDivide, CArray<double, double>& arDistance, BOOL bParametric, BOOL bMergeNode);
void CNodeDivideDlg::DivideNode(_NodeDivide& ND)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<unsigned int, unsigned int> NKeyArray;
	
	if(ND.m_nOption == 0) // Equal Distance 
	{
		if((pDoc->m_pDataCtrl)->DivideNode_Equal(ND.m_nNode1, ND.m_nNode2,
																						 ND.m_nDivision, ND.m_bMergeDupNode,
																						 ND.m_bIntersect))
		{
			pDoc->m_pViewCtrl->UnselectAll(NULL);
		}
	}
	else // Unequal Distance (or Parametric Unequal Distance)
	{
		BOOL bParametric = (ND.m_nOption == 1) ? FALSE : TRUE;
		int nDivision = ND.m_List.GetSize();
		if((pDoc->m_pDataCtrl)->DivideNode_Unequal(ND.m_nNode1, ND.m_nNode2,
																							 nDivision, ND.m_List,
																							 bParametric, ND.m_bMergeDupNode,
																							 ND.m_bIntersect))
		{
			pDoc->m_pViewCtrl->UnselectAll(NULL);
		}
	}
}

void CNodeDivideDlg::Execute()
{
	_NodeDivide ND;

	if(GetParameter(ND))
	{
		//CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
		//I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
		DivideNode(ND);
	}
	else
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Parameter_Error____));
	}
}

void CNodeDivideDlg::OnTmTolerance() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_MERGETOL,0));
}

LRESULT CNodeDivideDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if (m_wndNode4Divide.IsKeyPressed()) return 0L;

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if(m_wndNode4Divide.GetSafeHwnd() == (HWND)lParam)
	{
		if(pIGM->GetModelingModeST() == IG_STATE_GET_NODE_LIST)
		{
			_NodeDivide ND;
			if(GetParameter(ND))
			{
				CButton* pExecButton = GetExecButton();
				if(pExecButton)
				{
				// Execute Button의 Push 효과를 낸다. 
				}
				DivideNode(ND);
				
				//m_wndNode4Divide.ClearNodeKeyList();
				//m_wndNode4Divide.SetFocus();
			}
		}
	}
	return 0L;
}

void CNodeDivideDlg::OnTmIntsectTol() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_INTERSECTTOL,0));	
}

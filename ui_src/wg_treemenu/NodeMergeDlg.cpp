// NodeMergeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "NodeMergeDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_FlagCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNodeMergeDlg dialog


CNodeMergeDlg::CNodeMergeDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CNodeMergeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNodeMergeDlg)
	m_nOption = 0;
	m_bRemoveMergedNodes = TRUE;
	m_nStartNum = 1;
	m_nEndNum = 1;
	//}}AFX_DATA_INIT
	m_NumberCtrl.Add(IDC_TM_START_NUM_STATIC);
	m_NumberCtrl.Add(IDC_TM_START_NUM_EDIT);
	m_NumberCtrl.Add(IDC_TM_END_NUM_STATIC);
	m_NumberCtrl.Add(IDC_TM_END_NUM_EDIT);
}

void CNodeMergeDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNodeMergeDlg)
	DDX_Control(pDX, IDC_TM_TOLERANCE_UNIT, m_wndToleranceUnit);
	DDX_Control(pDX, IDC_TM_TOLERANCE_EDIT, m_wndToleranceEdit);
	DDX_Radio(pDX, IDC_TM_OPTION_ALL, m_nOption);
	DDX_Check(pDX, IDC_TM_REMOVE_CHECK, m_bRemoveMergedNodes);
	DDX_Text(pDX, IDC_TM_START_NUM_EDIT, m_nStartNum);
	DDX_Text(pDX, IDC_TM_END_NUM_EDIT, m_nEndNum);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CNodeMergeDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CNodeMergeDlg)
	ON_BN_CLICKED(IDC_TM_OPTION_ALL, OnTmOptionAll)
	ON_BN_CLICKED(IDC_TM_OPTION_NUMBER, OnTmOptionNumber)
	ON_BN_CLICKED(IDC_TM_OPTION_SELECTION, OnTmOptionSelection)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNodeMergeDlg message handlers
BOOL CNodeMergeDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	m_nEndNum = pDoc->m_pAttrCtrl->GetLastNumNode();
	m_wndToleranceUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndToleranceEdit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndToleranceEdit.SetEditUnit(pDoc->m_pFlagCtrl->GetMergeToleranceCurrentUnit());

//#ifdef _MDEMO
//  m_bRemoveMergedNodes = TRUE;
//  CWnd* pWnd = GetDlgItem(IDC_TM_REMOVE_CHECK);
//  pWnd->EnableWindow(FALSE);
//#endif
	
	UpdateData(FALSE);

	CtrlEnableDisable(m_NumberCtrl, m_nOption == 2);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNodeMergeDlg::OnTmOptionAll() 
{
	// TODO: Add your control notification handler code here
	CtrlEnableDisable(m_NumberCtrl, FALSE);
}

void CNodeMergeDlg::OnTmOptionSelection() 
{
	// TODO: Add your control notification handler code here
	CtrlEnableDisable(m_NumberCtrl, FALSE);
}

void CNodeMergeDlg::OnTmOptionNumber() 
{
	// TODO: Add your control notification handler code here
	CtrlEnableDisable(m_NumberCtrl, TRUE);
}

BOOL CNodeMergeDlg::GetParameter(_NodeMerge &NM)
{
	if (!UpdateData(TRUE)) return FALSE;

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	NM.m_nOption = m_nOption;
	switch(m_nOption)
	{
	case 0:
		NM.m_nStartNum = 1;
		NM.m_nEndNum = pDoc->m_pAttrCtrl->GetLastNumNode();
		break;
	case 1:
		pIGM->GetSelectedNodeKeyList(NM.m_KeyList);
		break;
	case 2:
		NM.m_nStartNum = m_nStartNum;
		NM.m_nEndNum = m_nEndNum;
		break;
	default: ASSERT(0);
	}
	NM.m_nTolerance = m_wndToleranceEdit.GetEditValue();
	NM.m_bRemoveMergedNodes = m_bRemoveMergedNodes;

	return TRUE;
}

BOOL CNodeMergeDlg::MergeNode(_NodeMerge &NM)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	BOOL bResult = FALSE;
	switch(NM.m_nOption)
	{
	case 0: // merge all
		bResult = pDoc->m_pDataCtrl->MergeNode_Number(NM.m_nStartNum, NM.m_nEndNum,
																									NM.m_nTolerance, 
																									NM.m_bRemoveMergedNodes);
		break;
	case 1: // merge selection
		bResult = pDoc->m_pDataCtrl->MergeNode_Select(NM.m_KeyList,
																									NM.m_nTolerance, 
																									NM.m_bRemoveMergedNodes);
		break;
	case 2: // merge by number
		bResult  = pDoc->m_pDataCtrl->MergeNode_Number(NM.m_nStartNum, NM.m_nEndNum,
																									 NM.m_nTolerance, 
																									 NM.m_bRemoveMergedNodes);
		break;
	default: ASSERT(0);
	}

	if (bResult) pDoc->m_pViewCtrl->UnselectAll(NULL);
	return bResult;
}

void CNodeMergeDlg::Execute()
{
	_NodeMerge NM;
	if (GetParameter(NM))
	{
		MergeNode(NM);
	}
	else
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Parameter_Error___));
	}
}

// ElemIntersectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ElemIntersectDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_FlagCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElemIntersectDlg dialog


CElemIntersectDlg::CElemIntersectDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CElemIntersectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElemIntersectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CElemIntersectDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemIntersectDlg)
	DDX_Control(pDX, IDC_TM_TOLERANCE_UNIT, m_wndTolUnit);
	DDX_Control(pDX, IDC_TM_TOLERANCE_EDIT, m_wndTolEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CElemIntersectDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CElemIntersectDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemIntersectDlg message handlers

BOOL CElemIntersectDlg::OnInitDialog() 
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndTolEdit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndTolUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndTolEdit.SetEditUnit(pDoc->m_pFlagCtrl->GetMergeToleranceCurrentUnit()); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CElemIntersectDlg::Execute()
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	CArray<T_ELEM_K, T_ELEM_K> aElemKey;
	pIGM->GetSelectedElemKeyList(aElemKey);
	if (aElemKey.GetSize() <= 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_element_));
		return;
	}

	double dblTol;
	dblTol = this->m_wndTolEdit.GetEditValue();

	BOOL bSuccess = pDoc->m_pDataCtrl->IntersectElem(aElemKey);
	if (bSuccess) pDoc->m_pViewCtrl->UnselectAll(NULL);
}

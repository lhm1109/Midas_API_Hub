// MeshMapKEDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "MeshMapKEDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_cmd\wg_cmdAll.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeshMapKEDlg dialog


CMeshMapKEDlg::CMeshMapKEDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CMeshMapKEDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMeshMapKEDlg)
	//}}AFX_DATA_INIT
}

void CMeshMapKEDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeshMapKEDlg)
	//}}AFX_DATA_MAP	
}

void CMeshMapKEDlg::Execute()
{

}

BEGIN_MESSAGE_MAP(CMeshMapKEDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CMeshMapKEDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeshMapKEDlg message handlers

BOOL CMeshMapKEDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMeshMapKEDlg::OnTmExecute() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
}

void CMeshMapKEDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

BOOL CMeshMapKEDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;  
	return TRUE;
}
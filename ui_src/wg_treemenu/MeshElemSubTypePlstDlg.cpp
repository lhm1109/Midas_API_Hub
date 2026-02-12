// MeshElemSubTypePlstDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "MeshElemSubTypePlstDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeshElemSubTypePlstDlg dialog


CMeshElemSubTypePlstDlg::CMeshElemSubTypePlstDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CMeshElemSubTypePlstDlg::IDD, pParent)
{
	m_bDOF = FALSE;
	//{{AFX_DATA_INIT(CMeshElemSubTypePlstDlg)	
	//}}AFX_DATA_INIT
}


void CMeshElemSubTypePlstDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeshElemSubTypePlstDlg)
	DDX_Control(pDX, IDC_TM_MESH_ELEM_WITH_DRILL_CHK, m_chkDOF);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMeshElemSubTypePlstDlg, CDialogMove)
	//{{AFX_MSG_MAP(CMeshElemSubTypePlstDlg)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeshElemSubTypePlstDlg message handlers

void CMeshElemSubTypePlstDlg::OnOK() 
{
	m_bDOF = m_chkDOF.GetCheck();

	CDialogMove::OnOK();
}

BOOL CMeshElemSubTypePlstDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	m_chkDOF.SetCheck(m_bDOF);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMeshElemSubTypePlstDlg::SetData(BOOL bDOF)
{
	m_bDOF = bDOF;
}

void CMeshElemSubTypePlstDlg::GetData(BOOL& bDOF)
{
	bDOF = m_bDOF;
}
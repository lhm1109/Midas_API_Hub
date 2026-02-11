// MeshElemSubTypePlateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "MeshElemSubTypePlateDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeshElemSubTypePlateDlg dialog


CMeshElemSubTypePlateDlg::CMeshElemSubTypePlateDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CMeshElemSubTypePlateDlg::IDD, pParent)
{
	m_nThickThin = 0;
	m_bDOF = FALSE;
	//{{AFX_DATA_INIT(CMeshElemSubTypePlateDlg)	
	//}}AFX_DATA_INIT
}


void CMeshElemSubTypePlateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeshElemSubTypePlateDlg)
	DDX_Control(pDX, IDC_TM_MESH_ELEM_TYPE_THICK, m_radioThick);
	DDX_Control(pDX, IDC_TM_MESH_ELEM_TYPE_THIN, m_radioThin);
	DDX_Control(pDX, IDC_TM_MESH_ELEM_WITH_DRILL_CHK, m_chkDOF);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMeshElemSubTypePlateDlg, CDialogMove)
	//{{AFX_MSG_MAP(CMeshElemSubTypePlateDlg)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeshElemSubTypePlateDlg message handlers

void CMeshElemSubTypePlateDlg::OnOK() 
{
	if(m_radioThick.GetCheck())
		m_nThickThin = 0;
	else
		m_nThickThin = 1;

	m_bDOF = m_chkDOF.GetCheck();

	CDialogMove::OnOK();
}

BOOL CMeshElemSubTypePlateDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	if(m_nThickThin == 0)
	{
		m_radioThick.SetCheck(TRUE);
		m_radioThin.SetCheck(FALSE);
	}
	else
	{
		m_radioThick.SetCheck(FALSE);
		m_radioThin.SetCheck(TRUE);
	}
	
	m_chkDOF.SetCheck(m_bDOF);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMeshElemSubTypePlateDlg::SetData(int nThickThin, BOOL bDOF)
{
	m_nThickThin = nThickThin;
	m_bDOF = bDOF;
}

void CMeshElemSubTypePlateDlg::GetData(int& nThickThin, BOOL& bDOF)
{
	nThickThin = m_nThickThin;
	bDOF = m_bDOF;
}
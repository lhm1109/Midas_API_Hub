// MeshElemSubTypeWallDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "MeshElemSubTypeWallDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define STR_WALL_FIXED  _LS(IDS_TM_WALL_ID_FIXED)
#define STR_WALL_INCR1  _LS(IDS_TM_WALL_ID_AUTO) 
#define STR_WALL_LAST1  _LS(IDS_TM_WALL_ID_LAST1)

/////////////////////////////////////////////////////////////////////////////
// CMeshElemSubTypeWallDlg dialog


CMeshElemSubTypeWallDlg::CMeshElemSubTypeWallDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CMeshElemSubTypeWallDlg::IDD, pParent)
{
	m_nMembPlate = 0;
	m_nWallIDType = 1;
	m_nWallID = 1;
	//{{AFX_DATA_INIT(CMeshElemSubTypeWallDlg)	
	//}}AFX_DATA_INIT
}


void CMeshElemSubTypeWallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeshElemSubTypeWallDlg)
	DDX_Control(pDX, IDC_TM_W_RADIO_MEMBR, m_radioMembrane);	
	DDX_Control(pDX, IDC_TM_W_RADIO_PLATE, m_radioPlate);	
	DDX_Control(pDX, IDC_TM_W_ID_COMBO, m_cboWall);	
	DDX_Control(pDX, IDC_TM_W_ID_EDIT, m_editWall);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMeshElemSubTypeWallDlg, CDialogMove)
	//{{AFX_MSG_MAP(CMeshElemSubTypeWallDlg)
	ON_CBN_SELCHANGE(IDC_TM_W_ID_COMBO, OnSelchangeTmWIdCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeshElemSubTypeWallDlg message handlers

void CMeshElemSubTypeWallDlg::OnOK() 
{
	if(m_radioMembrane.GetCheck())
	  m_nMembPlate = 0;
	else
		m_nMembPlate = 1;

	m_nWallIDType = m_cboWall.GetCurSel();

	CString strWallID = _T("");
	m_editWall.GetWindowText(strWallID);
	m_nWallID = _ttoi(strWallID); 

	CDialogMove::OnOK();
}

BOOL CMeshElemSubTypeWallDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	if(m_nMembPlate == 0)
	{
		m_radioMembrane.SetCheck(TRUE);
		m_radioPlate.SetCheck(FALSE);
	}
	else
	{
		m_radioMembrane.SetCheck(FALSE);
		m_radioPlate.SetCheck(TRUE);
	}

	m_cboWall.ResetContent();
	m_cboWall.AddString(STR_WALL_FIXED);
	m_cboWall.AddString(STR_WALL_INCR1);
	m_cboWall.AddString(STR_WALL_LAST1);  
	m_cboWall.SetCurSel(m_nWallIDType);
	
	if(m_nWallIDType>=0) m_editWall.SetMode(m_nWallIDType);
	m_editWall.SetEditBoxText(m_nWallIDType); 

	CString strText = _T("");  
	strText.Format(_T("%d"), m_nWallID);
	m_editWall.SetWindowText(strText);

	if(m_nWallIDType >= 0) m_editWall.SetMode(m_nWallIDType);
	if(m_nWallIDType == 2) m_editWall.UpdateEditBoxText();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMeshElemSubTypeWallDlg::SetData(int nMembPlate, int nWallIDType, int nWallID)
{
	m_nMembPlate = nMembPlate;
	m_nWallIDType = nWallIDType;
	m_nWallID = nWallID;
}

void CMeshElemSubTypeWallDlg::GetData(int& nMembPlate, int& nWallIDType, int& nWallID)
{
	nMembPlate = m_nMembPlate;
	nWallIDType = m_nWallIDType;
	nWallID = m_nWallID;
}

void CMeshElemSubTypeWallDlg::OnSelchangeTmWIdCombo() 
{ 
	int nMode = GetWallMode();
	if(nMode >= 0) m_editWall.SetMode(nMode);
	if(nMode == 2) m_editWall.UpdateEditBoxText();
}

int CMeshElemSubTypeWallDlg::GetWallMode()
{
	CString strSel = _T("");
	int nIndex = m_cboWall.GetCurSel();
	if(nIndex<0) return -1;
	m_cboWall.GetLBText(nIndex, strSel);

	if(strSel == STR_WALL_FIXED) return 0;
	else if(strSel == STR_WALL_INCR1) return 1;
	else if(strSel == STR_WALL_LAST1) return 2;

	return -1;
} 
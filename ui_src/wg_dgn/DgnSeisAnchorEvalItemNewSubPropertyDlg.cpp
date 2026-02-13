// CDgnSeisAnchorEvalItemNewSubPropertyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorEvalItemNewSubPropertyDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItemNewSubPropertyDlg dialog

CDgnSeisAnchorEvalItemNewSubPropertyDlg::CDgnSeisAnchorEvalItemNewSubPropertyDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnSeisAnchorEvalItemNewSubPropertyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorEvalItemNewSubPropertyDlg)	
	//}}AFX_DATA_INIT
	m_pParent = (CDgnSeisAnchorEvalItemNewDlg*)pParent;

	m_pGrid = new CDgnSeisAnchorPropertyGrid(this);
}

void CDgnSeisAnchorEvalItemNewSubPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchorEvalItemNewSubPropertyDlg)
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_PROPERTY_GRID, *m_pGrid);
	//}}AFX_DATA_MAP
}

void CDgnSeisAnchorEvalItemNewSubPropertyDlg::SetPropertyData(T_ANEV_D* pData)
{
	m_pData = pData;
}

void CDgnSeisAnchorEvalItemNewSubPropertyDlg::Data2Dlg() 
{ 
	m_pGrid->SetData2Grid(m_pData->BearingProp);

	UpdateData(FALSE);
}

BOOL CDgnSeisAnchorEvalItemNewSubPropertyDlg::Dlg2Data() 
{
	UpdateData(TRUE);

	m_pData->BearingProp.Initialize();
	m_pGrid->GetGrid2Data(m_pData->BearingProp);

	return TRUE;
}

void CDgnSeisAnchorEvalItemNewSubPropertyDlg::SetViewData()
{
	Dlg2Data();
	m_pParent->SetViewData();
}

BEGIN_MESSAGE_MAP(CDgnSeisAnchorEvalItemNewSubPropertyDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItemNewSubPropertyDlg message handlers

BOOL CDgnSeisAnchorEvalItemNewSubPropertyDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	m_pGrid->Initialize();

	Data2Dlg();
			
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


// CDgnSeisAnchorEvalItem2NewSubPropertyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorEvalItem2NewSubPropertyDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItem2NewSubPropertyDlg dialog

CDgnSeisAnchorEvalItem2NewSubPropertyDlg::CDgnSeisAnchorEvalItem2NewSubPropertyDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnSeisAnchorEvalItem2NewSubPropertyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorEvalItem2NewSubPropertyDlg)	
	//}}AFX_DATA_INIT
	m_pParent = (CDgnSeisAnchorEvalItem2NewDlg*)pParent;

	m_pGrid = new CDgnSeisAnchor2PropertyGrid(this);
}

void CDgnSeisAnchorEvalItem2NewSubPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchorEvalItem2NewSubPropertyDlg)
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_PROPERTY2_GRID, *m_pGrid);
	//}}AFX_DATA_MAP
}

void CDgnSeisAnchorEvalItem2NewSubPropertyDlg::SetPropertyData(T_ANEV_D* pData)
{
	m_pData = pData;
}

void CDgnSeisAnchorEvalItem2NewSubPropertyDlg::Data2Dlg() 
{ 
	m_pGrid->SetData2Grid(m_pData->BearingProp);

	UpdateData(FALSE);
}

BOOL CDgnSeisAnchorEvalItem2NewSubPropertyDlg::Dlg2Data() 
{
	UpdateData(TRUE);

	m_pData->BearingProp.Initialize();
	m_pGrid->GetGrid2Data(m_pData->BearingProp);

	return TRUE;
}

void CDgnSeisAnchorEvalItem2NewSubPropertyDlg::SetViewData()
{
	Dlg2Data();
	m_pParent->SetViewData();
}

BEGIN_MESSAGE_MAP(CDgnSeisAnchorEvalItem2NewSubPropertyDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItem2NewSubPropertyDlg message handlers

BOOL CDgnSeisAnchorEvalItem2NewSubPropertyDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	m_pGrid->Initialize();

	Data2Dlg();
			
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


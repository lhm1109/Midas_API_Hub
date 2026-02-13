// CDgnSeisAnchorEvalItem2NewSubTensionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorEvalItem2NewSubTensionDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItem2NewSubTensionDlg dialog

CDgnSeisAnchorEvalItem2NewSubTensionDlg::CDgnSeisAnchorEvalItem2NewSubTensionDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnSeisAnchorEvalItem2NewSubTensionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorEvalItem2NewSubTensionDlg)	
	//}}AFX_DATA_INIT
	m_pParent = (CDgnSeisAnchorEvalItem2NewDlg*)pParent;

	m_pGrid = new CDgnSeisAnchor2TensionGrid();
}

void CDgnSeisAnchorEvalItem2NewSubTensionDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchorEvalItem2NewSubTensionDlg)
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_TENSION2_GRID, *m_pGrid);
	//}}AFX_DATA_MAP
}

void CDgnSeisAnchorEvalItem2NewSubTensionDlg::SetTensionData(T_ANEV_D* pData)
{
	m_pData = pData;
}

void CDgnSeisAnchorEvalItem2NewSubTensionDlg::Data2Dlg() 
{ 
	m_pGrid->SetData2Grid(*m_pData);

	UpdateData(FALSE);
}

BOOL CDgnSeisAnchorEvalItem2NewSubTensionDlg::Dlg2Data() 
{
	UpdateData(TRUE);

	m_pData->TensChk.Initialize();
	m_pGrid->GetGrid2Data(*m_pData);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CDgnSeisAnchorEvalItem2NewSubTensionDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItem2NewSubTensionDlg message handlers

BOOL CDgnSeisAnchorEvalItem2NewSubTensionDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	m_pGrid->Initialize();

	Data2Dlg();
			
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

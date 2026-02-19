// CDgnSeisAnchorEvalItemNewSubTensionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorEvalItemNewSubTensionDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItemNewSubTensionDlg dialog

CDgnSeisAnchorEvalItemNewSubTensionDlg::CDgnSeisAnchorEvalItemNewSubTensionDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnSeisAnchorEvalItemNewSubTensionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorEvalItemNewSubTensionDlg)	
	//}}AFX_DATA_INIT

	m_pGrid = new CDgnSeisAnchorTensionGrid();
}

void CDgnSeisAnchorEvalItemNewSubTensionDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchorEvalItemNewSubTensionDlg)
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_TENSION_GRID, *m_pGrid);
	//}}AFX_DATA_MAP
}

void CDgnSeisAnchorEvalItemNewSubTensionDlg::SetTensionData(T_ANEV_D* pData)
{
	m_pData = pData;
}

void CDgnSeisAnchorEvalItemNewSubTensionDlg::Data2Dlg() 
{ 
	m_pGrid->SetData2Grid(*m_pData);

	UpdateData(FALSE);
}

BOOL CDgnSeisAnchorEvalItemNewSubTensionDlg::Dlg2Data() 
{
	UpdateData(TRUE);

	m_pData->TensChk.Initialize();
	m_pGrid->GetGrid2Data(*m_pData);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CDgnSeisAnchorEvalItemNewSubTensionDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItemNewSubTensionDlg message handlers

BOOL CDgnSeisAnchorEvalItemNewSubTensionDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	m_pGrid->Initialize();

	Data2Dlg();
			
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

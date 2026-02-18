// CDgnSeisAnchorEvalItemNewSubSupportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorEvalItemNewSubSupportDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItemNewSubSupportDlg dialog

CDgnSeisAnchorEvalItemNewSubSupportDlg::CDgnSeisAnchorEvalItemNewSubSupportDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnSeisAnchorEvalItemNewSubSupportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorEvalItemNewSubSupportDlg)	
	//}}AFX_DATA_INIT
	m_pParent = (CDgnSeisAnchorEvalItemNewDlg*)pParent;

	m_pGrid = new CDgnSeisAnchorSupportGrid(this);
}

void CDgnSeisAnchorEvalItemNewSubSupportDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchorEvalItemNewSubSupportDlg)
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_SUPPORT_GRID, *m_pGrid);
	//}}AFX_DATA_MAP
}


void CDgnSeisAnchorEvalItemNewSubSupportDlg::SetSupportData(T_ANEV_D* pData)
{
	m_pData = pData;
}

void CDgnSeisAnchorEvalItemNewSubSupportDlg::Data2Dlg() 
{ 
	m_pGrid->SetData2Grid(m_pData->SuppType);

	UpdateData(FALSE);
}

BOOL CDgnSeisAnchorEvalItemNewSubSupportDlg::Dlg2Data() 
{
	UpdateData(TRUE);

	m_pData->SuppType.Initialize();
	m_pGrid->GetGrid2Data(m_pData->SuppType);

	return TRUE;
}

void CDgnSeisAnchorEvalItemNewSubSupportDlg::SetViewData()
{
	Dlg2Data();
	m_pParent->SetViewData();
}

BEGIN_MESSAGE_MAP(CDgnSeisAnchorEvalItemNewSubSupportDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItemNewSubSupportDlg message handlers

BOOL CDgnSeisAnchorEvalItemNewSubSupportDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	m_pGrid->Initialize();

	Data2Dlg();
			
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


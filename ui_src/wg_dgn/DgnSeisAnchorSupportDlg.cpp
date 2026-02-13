// CDgnSeisAnchorSupportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorSupportDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorSupportDlg dialog

CDgnSeisAnchorSupportDlg::CDgnSeisAnchorSupportDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisAnchorSupportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorSupportDlg)	
	//}}AFX_DATA_INIT
	m_pGrid = NULL;
}

void CDgnSeisAnchorSupportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchorSupportDlg)
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_SUPPORT_GRID, *m_pGrid);
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_SUPPORT_BMP, m_wndPicture);
	//}}AFX_DATA_MAP
}

void CDgnSeisAnchorSupportDlg::Data2Dlg() 
{ 
	m_pGrid->SetData2Grid(m_Data);

	UpdateData(FALSE);
}

BOOL CDgnSeisAnchorSupportDlg::Dlg2Data() 
{
	UpdateData(TRUE);

	m_Data.Initialize();
	m_pGrid->GetGrid2Data(m_Data);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CDgnSeisAnchorSupportDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_SUPPORT_OK_BTN,     OnDgnSeisAnchorSupportOKBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_SUPPORT_CANCEL_BTN, OnDgnSeisAnchorSupportCancelBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorSupportDlg message handlers

BOOL CDgnSeisAnchorSupportDlg::OnInitDialog() 
{
	if (m_pGrid == nullptr)
		m_pGrid = new CDgnSeisAnchorSupportGrid();

	CDialogMove::OnInitDialog();

	m_pGrid->Initialize();
	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\Dgn_Seis_Anchor_Support.svg"));

	Data2Dlg();
			
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisAnchorSupportDlg::OnDgnSeisAnchorSupportOKBtn()
{
	if(!Dlg2Data()) return;

	if(m_pGrid)
	{
		delete m_pGrid;
		m_pGrid = NULL;
	}

	CDialogMove::OnOK();
}

void CDgnSeisAnchorSupportDlg::OnDgnSeisAnchorSupportCancelBtn()
{
	if(m_pGrid)
	{
		delete m_pGrid;
		m_pGrid = NULL;
	}
	
	CDialogMove::OnCancel();
}
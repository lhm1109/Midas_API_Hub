// DgnRebarDlg.cpp : implementation file
//
// 2006. 11. 23
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"

#include "..\wg_db\wg_db_AttrCtrl.h"

#include "DgnRebarDlg.h"
#include "DgnRebarPscDlg.h"
#include "DgnConColmReinforceDlgCIVIL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnRebarDlg dialog


CDgnRebarDlg::CDgnRebarDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnRebarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnRebarPscDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pBeamDlg = NULL;  
	m_pColumnDlg = NULL;
}

/*
void CDgnRebarDlg::SetParamDataPsc(T_RPSC_K RpscK)
{
	m_KeyPsc = RpscK;
}*/

void CDgnRebarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRebarDlg)
	DDX_Control(pDX, IDC_DGN_TAB, m_ctrlTab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnRebarDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnRebarDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnRebarDlg message handlers

BOOL CDgnRebarDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->LayoutChildDlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CDgnRebarDlg::LayoutChildDlg()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);
	
	int nInitShow = 0;

	m_pBeamDlg = new CDgnRebarPscDlg(this);
	m_pColumnDlg = new CDgnConColmReinforceDlgCIVIL(this);
	
	T_CHRP_D chrpData;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(pDoc->m_pAttrCtrl->GetChrp(chrpData))
	{
		BOOL bSecondType = (chrpData.iDgnCode == RC_JTG_B02_01_2008||chrpData.iDgnCode == RC_CJJ_166_2011);
		m_pColumnDlg->SetTieSecondType(bSecondType);
	}
	
		
	/*
	if (m_KeyPsc != 0) 
	{
		m_pPscDlg->SetParamData(m_KeyPsc);
	}*/

	m_ctrlTab.AddTab(m_pBeamDlg, _LS(IDS_DGN_BEAM), CDgnRebarPscDlg::IDD, TRUE);  
	m_ctrlTab.AddTab(m_pColumnDlg, _LS(IDS_DGN_COLUMN), CDgnConColmReinforceDlgCIVIL::IDD, TRUE);

	m_ctrlTab.ShowTab(nInitShow);
}

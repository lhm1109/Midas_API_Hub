// DgnConColmReinforceRebarDlgCIVIL.cpp : implementation file
//
// 2006. 11. 23
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"


#include "DgnConColmReinforceRebarDlgCIVIL.h"
#include "DgnConColmReinforceDlgCIVIL.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConColmReinforceRebarDlgCIVIL dialog


CDgnConColmReinforceRebarDlgCIVIL::CDgnConColmReinforceRebarDlgCIVIL(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConColmReinforceRebarDlgCIVIL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMRebarPscDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pColumnDlg = NULL;
}

void CDgnConColmReinforceRebarDlgCIVIL::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConColmReinforceRebarDlgCIVIL)
	DDX_Control(pDX, IDC_DGN_TAB, m_ctrlTab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConColmReinforceRebarDlgCIVIL, CDialogMove)
	//{{AFX_MSG_MAP(CDgnConColmReinforceRebarDlgCIVIL)
	ON_BN_CLICKED(IDC_DGN_ADD_BTN, OnDgnAdd)
	ON_BN_CLICKED(IDC_DGN_DELETE, OnDgnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConColmReinforceRebarDlgCIVIL message handlers

BOOL CDgnConColmReinforceRebarDlgCIVIL::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
		
	// TODO: Add extra initialization here
	this->LayoutChildDlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConColmReinforceRebarDlgCIVIL::OnDgnAdd()
{
	switch (m_ctrlTab.GetCurSel())
	{
		case 0:
		m_pColumnDlg->AddDgn();
		break;
	default:
		break;
	}
}

void CDgnConColmReinforceRebarDlgCIVIL::OnDgnDelete()
{
	switch (m_ctrlTab.GetCurSel())
	{
	case 0:
		m_pColumnDlg->DeleteDgn();
		break;
	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CDgnConColmReinforceRebarDlgCIVIL::LayoutChildDlg()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);
	
	int nInitShow = 0;

	m_pColumnDlg = new CDgnConColmReinforceDlgCIVIL(this);
	m_ctrlTab.AddTab(m_pColumnDlg, _LS(IDS_DGN_COLUMN), CDgnConColmReinforceDlgCIVIL::IDD, TRUE);
	
	m_ctrlTab.ShowTab(nInitShow);
}

void CDgnConColmReinforceRebarDlgCIVIL::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CDgnConColmReinforceRebarDlgCIVIL::SetStatusAddBtn(int nStatus)
{
	switch (nStatus)
	{
	case CDgnConColmReinforceRebarDlgCIVIL::DGNCON_BTN_HIDE:
		GetDlgItem(IDC_DGN_ADD_BTN)->ShowWindow(SW_HIDE);
		break;
	case CDgnConColmReinforceRebarDlgCIVIL::DGNCON_BTN_SHOW:
		GetDlgItem(IDC_DGN_ADD_BTN)->ShowWindow(SW_SHOW);
		break;
	case CDgnConColmReinforceRebarDlgCIVIL::DGNCON_BTN_DISABLE:
		GetDlgItem(IDC_DGN_ADD_BTN)->EnableWindow(FALSE);
		break;
	case CDgnConColmReinforceRebarDlgCIVIL::DGNCON_BTN_ENABLE:
		GetDlgItem(IDC_DGN_ADD_BTN)->EnableWindow(TRUE);
		break;
	default:
		break;
	}	
}

void CDgnConColmReinforceRebarDlgCIVIL::SetStatusDeleteBtn(int nStatus)
{
	switch (nStatus)
	{
	case CDgnConColmReinforceRebarDlgCIVIL::DGNCON_BTN_HIDE:
		GetDlgItem(IDC_DGN_DELETE)->ShowWindow(SW_HIDE);
		break;
	case CDgnConColmReinforceRebarDlgCIVIL::DGNCON_BTN_SHOW:
		GetDlgItem(IDC_DGN_DELETE)->ShowWindow(SW_SHOW);
		break;
	case CDgnConColmReinforceRebarDlgCIVIL::DGNCON_BTN_DISABLE:
		GetDlgItem(IDC_DGN_DELETE)->EnableWindow(FALSE);
		break;
	case CDgnConColmReinforceRebarDlgCIVIL::DGNCON_BTN_ENABLE:
		GetDlgItem(IDC_DGN_DELETE)->EnableWindow(TRUE);
		break;
	default:
		break;
	}
}

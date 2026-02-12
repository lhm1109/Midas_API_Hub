// CmdAutoLoadCombSelCvlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombSelCvlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombSelCvlDlg dialog
CCmdAutoLoadCombSelCvlDlg::CCmdAutoLoadCombSelCvlDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdAutoLoadCombSelCvlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdAutoLoadCombSelCvlDlg)
	//}}AFX_DATA_INIT

	m_bUlti1 = FALSE;
	m_bUlti2 = FALSE;
	m_bUlti3 = FALSE;
	m_bUlti4 = FALSE;
	m_bUlti5 = FALSE;
	m_bExtr1 = FALSE;
	m_bExtr2 = FALSE;
	m_bServ1 = FALSE;
	m_bServ2 = FALSE;
	m_bServ3 = FALSE;
	m_bServ4 = FALSE;
	m_bServ5 = FALSE;
	m_bFati1 = FALSE;
}

void CCmdAutoLoadCombSelCvlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdAutoLoadCombSelCvlDlg)  
	DDX_Check(pDX,  IDC_CMD_AUTO_LCOM_ULTI1_CHK,  m_bUlti1);
	DDX_Check(pDX,  IDC_CMD_AUTO_LCOM_ULTI2_CHK,  m_bUlti2);
	DDX_Check(pDX,  IDC_CMD_AUTO_LCOM_ULTI3_CHK,  m_bUlti3);
	DDX_Check(pDX,  IDC_CMD_AUTO_LCOM_ULTI4_CHK,  m_bUlti4);
	DDX_Check(pDX,  IDC_CMD_AUTO_LCOM_ULTI5_CHK,  m_bUlti5);
	DDX_Check(pDX,  IDC_CMD_AUTO_LCOM_EXTR1_CHK,  m_bExtr1);
	DDX_Check(pDX,  IDC_CMD_AUTO_LCOM_EXTR2_CHK,  m_bExtr2);
	DDX_Check(pDX,  IDC_CMD_AUTO_LCOM_SERV1_CHK,  m_bServ1);
	DDX_Check(pDX,  IDC_CMD_AUTO_LCOM_SERV2_CHK,  m_bServ2);
	DDX_Check(pDX,  IDC_CMD_AUTO_LCOM_SERV3_CHK,  m_bServ3);
	DDX_Check(pDX,  IDC_CMD_AUTO_LCOM_SERV4_CHK,  m_bServ4);
	DDX_Check(pDX,  IDC_CMD_AUTO_LCOM_SERV5_CHK,  m_bServ5);
	DDX_Check(pDX,  IDC_CMD_AUTO_LCOM_FATI1_CHK,  m_bFati1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombSelCvlDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCmdAutoLoadCombSelCvlDlg)
	ON_BN_CLICKED(IDC_CMD_AUTO_LCOM_SEL_ALL_BTN,   OnCmdAutoCombSelAll)
	ON_BN_CLICKED(IDC_CMD_AUTO_LCOM_UNSEL_ALL_BTN, OnCmdAutoCombUnSelAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCmdAutoLoadCombSelCvlDlg::SetDefaultValue()
{
// 	if(m_nDesignType == 0) // Steel
// 	{
// 		m_bUlti1 = TRUE;
// 		m_bUlti2 = TRUE;
// 		m_bUlti3 = TRUE;
// 		m_bUlti4 = TRUE;
// 		m_bUlti5 = TRUE;
// 		m_bExtr1 = FALSE;
// 		m_bExtr2 = FALSE;
// 		m_bServ1 = FALSE;
// 		m_bServ2 = TRUE;
// 		m_bServ3 = FALSE;
// 		m_bServ4 = FALSE;
// 		m_bFati1 = TRUE;
// 	}
// 	else if(m_nDesignType == 1) // Concrete
// 	{
// 		m_bUlti1 = TRUE;
// 		m_bUlti2 = TRUE;
// 		m_bUlti3 = TRUE;
// 		m_bUlti4 = TRUE;
// 		m_bUlti5 = TRUE;
// 		m_bExtr1 = TRUE;
// 		m_bExtr2 = FALSE;
// 		m_bServ1 = TRUE;
// 		m_bServ2 = FALSE;
// 		m_bServ3 = FALSE;
// 		m_bServ4 = FALSE;
// 		m_bFati1 = FALSE;
// 	}
// 	else 
// 	{
// 		ASSERT(0);
// 	}

	UpdateData(FALSE);
}

void CCmdAutoLoadCombSelCvlDlg::EnableDisableControls()
{
	if(m_nDesignType == 0 || m_nDesignType == 3) // Steel, Steel Composite
	{
		GetDlgItem(IDC_CMD_AUTO_LCOM_EXTR_GRP)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_AUTO_LCOM_EXTR1_CHK)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_AUTO_LCOM_EXTR2_CHK)->EnableWindow(FALSE);
	}
	else if(m_nDesignType == 1) // Concrete
	{
		GetDlgItem(IDC_CMD_AUTO_LCOM_FATI_GRP)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_AUTO_LCOM_FATI1_CHK)->EnableWindow(FALSE);
	}

	if (m_nDesignType == 1) //Concrete
	{
		GetDlgItem(IDC_CMD_AUTO_LCOM_SERV5_CHK)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CMD_AUTO_LCOM_SERV5_CHK)->EnableWindow(FALSE);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombSelCvlDlg message handlers

BOOL CCmdAutoLoadCombSelCvlDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	SetDefaultValue();
	EnableDisableControls();

	return TRUE;
}

void CCmdAutoLoadCombSelCvlDlg::OnOK()
{
	UpdateData(TRUE);

	CDialogMove::OnOK();
}

void CCmdAutoLoadCombSelCvlDlg::OnCmdAutoCombSelAll()
{
	if(m_nDesignType == 0 || m_nDesignType == 3) // Steel, Steel Composite
	{
		m_bUlti1 = TRUE;
		m_bUlti2 = TRUE;
		m_bUlti3 = TRUE;
		m_bUlti4 = TRUE;
		m_bUlti5 = TRUE;
		m_bExtr1 = FALSE;
		m_bExtr2 = FALSE;
		m_bServ1 = TRUE;
		m_bServ2 = TRUE;
		m_bServ3 = TRUE;
		m_bServ4 = TRUE;
		m_bServ5 = FALSE;
		m_bFati1 = TRUE;
	}
	else if(m_nDesignType == 1) // Concrete
	{
		m_bUlti1 = TRUE;
		m_bUlti2 = TRUE;
		m_bUlti3 = TRUE;
		m_bUlti4 = TRUE;
		m_bUlti5 = TRUE;
		m_bExtr1 = TRUE;
		m_bExtr2 = TRUE;
		m_bServ1 = TRUE;
		m_bServ2 = TRUE;
		m_bServ3 = TRUE;
		m_bServ4 = TRUE;
		m_bServ5 = TRUE;
		m_bFati1 = FALSE;
	}
	else 
	{
		ASSERT(0);
	}
	
	UpdateData(FALSE);
}

void CCmdAutoLoadCombSelCvlDlg::OnCmdAutoCombUnSelAll()
{
	m_bUlti1 = FALSE;
	m_bUlti2 = FALSE;
	m_bUlti3 = FALSE;
	m_bUlti4 = FALSE;
	m_bUlti5 = FALSE;
	m_bExtr1 = FALSE;
	m_bExtr2 = FALSE;
	m_bServ1 = FALSE;
	m_bServ2 = FALSE;
	m_bServ3 = FALSE;
	m_bServ4 = FALSE;
	m_bServ5 = FALSE;
	m_bFati1 = FALSE;

	UpdateData(FALSE);
}

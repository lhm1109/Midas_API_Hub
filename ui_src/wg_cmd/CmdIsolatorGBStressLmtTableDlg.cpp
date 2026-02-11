// NLLinkPrtListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdIsolatorGBCoefCalcDlg.h"
#include "CmdIsolatorGBStressLmtTableDlg.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdIsolatorGBStressLmtTableDlg dialog


CCmdIsolatorGBStressLmtTableDlg::CCmdIsolatorGBStressLmtTableDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdIsolatorGBStressLmtTableDlg::IDD, pParent),m_wndGrid()
{
	//{{AFX_DATA_INIT(CCmdIsolatorGBStressLmtTableDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCmdIsolatorGBStressLmtTableDlg:: ~CCmdIsolatorGBStressLmtTableDlg()
{

}

void CCmdIsolatorGBStressLmtTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdIsolatorGBStressLmtTableDlg)
	DDX_Control(pDX, IDD_CMD_LOAD_ISOLATOR_GB_LMT_GRID, m_wndGrid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCmdIsolatorGBStressLmtTableDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCmdIsolatorGBStressLmtTableDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_OK, OnCmdBtnOK)
	ON_BN_CLICKED(IDC_CMD_BTN_CANCEL, OnCmdBtnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL CCmdIsolatorGBStressLmtTableDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	
	m_wndGrid.Initialize();
	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	m_wndGrid.MakeItemEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCmdIsolatorGBStressLmtTableDlg::OnCmdBtnOK() 
{
// 	T_ISGB_D data;	data.Initialize();
// 	m_pDoc->m_pUnitCtrl->ConvertUnitIsolationOut(data);
	m_wndGrid.Grid2Data();
	//m_pData = m_wndGrid.pIsgbD;
	CDialogMove::OnOK();
}

void CCmdIsolatorGBStressLmtTableDlg::OnCmdBtnCancel() 
{
	//T_ISGB_D data;	data.Initialize();
	//m_wndGrid.Grid2Data(data);
	CDialogMove::OnCancel();
}

BOOL CCmdIsolatorGBStressLmtTableDlg::SetParamData(T_ISGB_D *pParamData)
{
	m_wndGrid.SetData(pParamData);
	return TRUE;
}

BOOL CCmdIsolatorGBStressLmtTableDlg::GetParamData(T_ISGB_D *pParamData)
{
	pParamData = m_pData;
	return TRUE;
}

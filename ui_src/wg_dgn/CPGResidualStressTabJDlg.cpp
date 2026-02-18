// CPGResidualStressTabJDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGResidualStressTabJDlg.h"
#include "CPGResidualStressDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCPGResidualStressTabJDlg dialog


CCPGResidualStressTabJDlg::CCPGResidualStressTabJDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCPGResidualStressTabJDlg::IDD, pParent)
{

	m_nType = 2;

	ASSERT(pParent);
	m_pParent = (CCPGResidualStressDlg*)pParent;

	m_Data.Initialize();
}


void CCPGResidualStressTabJDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_DGN_CPG_RESIDUAl_STRESS_TYPE_SR_RDO, m_nType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPGResidualStressTabJDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCPGResidualStressTabJDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCPGResidualStressTabJDlg message handlers


BOOL CCPGResidualStressTabJDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	InitialDlgData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCPGResidualStressTabJDlg::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_I_Data = m_Data;

	return CChildDialog::DestroyWindow();
}


void CCPGResidualStressTabJDlg::InitialDlgData()
{
	m_Data.Initialize();

	Data2Dlg();
}

BOOL CCPGResidualStressTabJDlg::Data2Dlg()
{
	m_nType = m_Data.nType;

	UpdateData(FALSE);

	return TRUE;
}

BOOL CCPGResidualStressTabJDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.nType = m_nType;

	return TRUE;
}

void CCPGResidualStressTabJDlg::SetAllControlEnable(bool enable)
{
	GetDlgItem(IDC_DGN_CPG_RESIDUAl_STRESS_TYPE_SR_RDO)->EnableWindow(enable);
	GetDlgItem(IDC_DGN_CPG_RESIDUAl_STRESS_TYPE_HR_RDO)->EnableWindow(enable);
	GetDlgItem(IDC_DGN_CPG_RESIDUAl_STRESS_TYPE_LW_RDO)->EnableWindow(enable);
	GetDlgItem(IDC_DGN_CPG_RESIDUAl_STRESS_TYPE_HW_RDO)->EnableWindow(enable);
}

void CCPGResidualStressTabJDlg::GetCgscData()
{
	Dlg2Data();

	m_pParent->m_I_Data = m_Data;
}

void CCPGResidualStressTabJDlg::SetCgscData()
{
	Data2Dlg();
}


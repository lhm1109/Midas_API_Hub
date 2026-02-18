// CPGUnbraLengTabJDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGUnbraLengTabJDlg.h"
#include "CPGUnbraLengDlg.h"
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
// CCPGUnbraLengTabJDlg dialog


CCPGUnbraLengTabJDlg::CCPGUnbraLengTabJDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCPGUnbraLengTabJDlg::IDD, pParent)
{
	//전역변수 초기화
	

	//{{AFX_DATA_INIT(CCPGUnbraLengTabJDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CCPGUnbraLengDlg*)pParent;

	m_Data.Initialize();
}


void CCPGUnbraLengTabJDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPGUnbraLengTabJDlg)
	DDX_Control(pDX, IDC_DGN_CPG_CPUL_TOP_EDIT,    m_dFlangeTop);
	DDX_Control(pDX, IDC_DGN_CPG_CPUL_BOT_EDIT,    m_dFlangeBot);
	DDX_Control(pDX, IDC_DGN_CPG_CPUL_TOP_UNIT,    m_dFlangeTopU);
	DDX_Control(pDX, IDC_DGN_CPG_CPUL_BOT_UNIT,    m_dFlangeBotU);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPGUnbraLengTabJDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCPGUnbraLengTabJDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCPGUnbraLengTabJDlg message handlers


BOOL CCPGUnbraLengTabJDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	SetInitUnit();

	InitialDlgData();
	SetAllControlEnable(m_pParent->m_bJTabEnable);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCPGUnbraLengTabJDlg::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_J_Data = m_Data;

	return CChildDialog::DestroyWindow();
}


void CCPGUnbraLengTabJDlg::SetInitUnit()
{
	m_dFlangeTopU.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dFlangeBotU.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCPGUnbraLengTabJDlg::InitialDlgData()
{
	//m_Data.Initialize();

	Data2Dlg();
}

BOOL CCPGUnbraLengTabJDlg::Data2Dlg()
{
	m_dFlangeBot.SetEditUnit(m_Data.dLengthBot);
	m_dFlangeTop.SetEditUnit(m_Data.dLengthTop);
	
	UpdateData(FALSE);
	return TRUE;
}

BOOL CCPGUnbraLengTabJDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.dLengthBot  = m_dFlangeBot.GetEditValue();
	m_Data.dLengthTop  = m_dFlangeTop.GetEditValue();	

	return TRUE;
}

void CCPGUnbraLengTabJDlg::GetCpulData()
{
	Dlg2Data();

	m_pParent->m_J_Data = m_Data;
}

void CCPGUnbraLengTabJDlg::SetCpulData()
{
	Data2Dlg();
}

void CCPGUnbraLengTabJDlg::SetAllControlEnable(BOOL bEnable)
{
// 	bool enable = true;
// 	//if(m_pParent->m_bBothIJ)
// 	//	enable = false;
// 
// 	if(m_pParent->m_Tab.GetCurSel() == 0)
// 		enable = true;

	GetDlgItem(IDC_DGN_CPG_CPUL_TOP_EDIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPUL_BOT_EDIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPUL_TOP_UNIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_CPUL_BOT_UNIT)->EnableWindow(bEnable);
}
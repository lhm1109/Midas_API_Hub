// CPGUnbraLengTabIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGUnbraLengTabIDlg.h"
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
// CCPGUnbraLengTabIDlg dialog


CCPGUnbraLengTabIDlg::CCPGUnbraLengTabIDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCPGUnbraLengTabIDlg::IDD, pParent)
{
	//전역변수 초기화
	

	//{{AFX_DATA_INIT(CCPGUnbraLengTabIDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CCPGUnbraLengDlg*)pParent;

	m_Data.Initialize();
}


void CCPGUnbraLengTabIDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPGUnbraLengTabIDlg)
	DDX_Control(pDX, IDC_DGN_CPG_CPUL_TOP_EDIT,    m_dFlangeTop);
	DDX_Control(pDX, IDC_DGN_CPG_CPUL_BOT_EDIT,    m_dFlangeBot);
	DDX_Control(pDX, IDC_DGN_CPG_CPUL_TOP_UNIT,    m_dFlangeTopU);
	DDX_Control(pDX, IDC_DGN_CPG_CPUL_BOT_UNIT,    m_dFlangeBotU);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPGUnbraLengTabIDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCPGUnbraLengTabIDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCPGUnbraLengTabIDlg message handlers


BOOL CCPGUnbraLengTabIDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	SetInitUnit();

	InitialDlgData();
	SetAllControlEnable();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCPGUnbraLengTabIDlg::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_I_Data = m_Data;

	return CChildDialog::DestroyWindow();
}


void CCPGUnbraLengTabIDlg::SetInitUnit()
{
	m_dFlangeTopU.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dFlangeBotU.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCPGUnbraLengTabIDlg::InitialDlgData()
{
	//m_Data.Initialize();

	Data2Dlg();
}

BOOL CCPGUnbraLengTabIDlg::Data2Dlg()
{
	m_dFlangeBot.SetEditUnit(m_Data.dLengthBot);
	m_dFlangeTop.SetEditUnit(m_Data.dLengthTop);
	
	UpdateData(FALSE);
	return TRUE;
}

BOOL CCPGUnbraLengTabIDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.dLengthBot  = m_dFlangeBot.GetEditValue();
	m_Data.dLengthTop  = m_dFlangeTop.GetEditValue();	

	return TRUE;
}

void CCPGUnbraLengTabIDlg::GetCpulData()
{
	Dlg2Data();

	m_pParent->m_I_Data = m_Data;
}

void CCPGUnbraLengTabIDlg::SetCpulData()
{
	Data2Dlg();
}

void CCPGUnbraLengTabIDlg::SetAllControlEnable()
{
	bool enable = true;
	//if(m_pParent->m_bBothIJ)
	//	enable = false;

	if(m_pParent->m_Tab.GetCurSel() == 0)
		enable = true;

	GetDlgItem(IDC_DGN_CPG_CPUL_TOP_EDIT)->EnableWindow(enable);
	GetDlgItem(IDC_DGN_CPG_CPUL_BOT_EDIT)->EnableWindow(enable);
	GetDlgItem(IDC_DGN_CPG_CPUL_TOP_UNIT)->EnableWindow(enable);
	GetDlgItem(IDC_DGN_CPG_CPUL_BOT_UNIT)->EnableWindow(enable);
}
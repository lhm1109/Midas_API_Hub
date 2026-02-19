// CRCDesignOptTabIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CRCDesignOptTabIDlg.h"
#include "CRCDgnOption.h"
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
// CCRCDesignOptTabIDlg dialog


CCRCDesignOptTabIDlg::CCRCDesignOptTabIDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCRCDesignOptTabIDlg::IDD, pParent)
{
	//전역변수 초기화
	

	//{{AFX_DATA_INIT(CCRCDesignOptTabIDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CCRCDgnOption*)pParent;

	m_Data.Initialize();
	m_Data.dLengthTop = 45.;
	m_Data.dLengthBot = 45.;
}


void CCRCDesignOptTabIDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCRCDesignOptTabIDlg)
	DDX_Control(pDX, IDC_DGN_CPG_CPUL_TOP_EDIT,    m_dFlangeTop);
	DDX_Control(pDX, IDC_DGN_CPG_CPUL_BOT_EDIT,    m_dFlangeBot);
	DDX_Control(pDX, IDC_DGN_CPG_CPUL_TOP_UNIT,    m_dFlangeTopU);
	DDX_Control(pDX, IDC_DGN_CPG_CPUL_BOT_UNIT,    m_dFlangeBotU);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCRCDesignOptTabIDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCRCDesignOptTabIDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCRCDesignOptTabIDlg message handlers


BOOL CCRCDesignOptTabIDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	SetInitUnit();

	InitialDlgData();
	SetAllControlEnable();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCRCDesignOptTabIDlg::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_I_Data = m_Data;

	return CChildDialog::DestroyWindow();
}


void CCRCDesignOptTabIDlg::SetInitUnit()
{
	m_dFlangeTopU.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_dFlangeBotU.SetUnitType(D_UNITSYS_BASE_DEGREE);
}

void CCRCDesignOptTabIDlg::InitialDlgData()
{
	//m_Data.Initialize();
	
	Data2Dlg();
}

BOOL CCRCDesignOptTabIDlg::Data2Dlg()
{
	m_dFlangeBot.SetEditUnit(m_Data.dLengthBot);
	m_dFlangeTop.SetEditUnit(m_Data.dLengthTop);


	
	UpdateData(FALSE);
	return TRUE;
}

BOOL CCRCDesignOptTabIDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.dLengthBot  = m_dFlangeBot.GetEditValue();
	m_Data.dLengthTop  = m_dFlangeTop.GetEditValue();	

	return TRUE;
}

void CCRCDesignOptTabIDlg::GetCpulData()
{
	Dlg2Data();

	m_pParent->m_I_Data = m_Data;
}

void CCRCDesignOptTabIDlg::SetCpulData()
{
	Data2Dlg();
}

void CCRCDesignOptTabIDlg::SetAllControlEnable()
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
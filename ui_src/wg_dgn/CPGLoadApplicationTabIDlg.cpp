// CPGLoadApplicationTabIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGLoadApplicationTabIDlg.h"
#include "CPGLoadApplicationDlg.h"

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
// CCPGLoadApplicationTabIDlg dialog


CCPGLoadApplicationTabIDlg::CCPGLoadApplicationTabIDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCPGLoadApplicationTabIDlg::IDD, pParent)
{
	//전역변수 초기화
	

	//{{AFX_DATA_INIT(CCPGLoadApplicationTabIDlg)
	m_bUseLoad = TRUE;
	m_iType = 0;
	
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CCPGLoadApplicationDlg*)pParent;

	m_Data.Initialize();
}


void CCPGLoadApplicationTabIDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPGLoadApplicationTabIDlg)
	DDX_Check(pDX, IDC_DGN_CPG_LOAD_USED_CHK, m_bUseLoad);

	DDX_Radio(pDX, IDC_DGN_CPG_LOAD_TYPE_A_RADIO, m_iType);

	DDX_Control(pDX, IDC_DGN_CPG_LOAD_A_EDIT, m_dA);
	DDX_Control(pDX, IDC_DGN_CPG_LOAD_S_EDIT, m_dSs);
	DDX_Control(pDX, IDC_DGN_CPG_LOAD_C_EDIT, m_dC);
	
	DDX_Control(pDX, IDC_DGN_CPG_LOAD_A_UNIT, m_dAUnit);
	DDX_Control(pDX, IDC_DGN_CPG_LOAD_S_UNIT, m_dSsUnit);
	DDX_Control(pDX, IDC_DGN_CPG_LOAD_C_UNIT, m_dCUnit);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPGLoadApplicationTabIDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCPGLoadApplicationTabIDlg)
	ON_BN_CLICKED(IDC_DGN_CPG_LOAD_USED_CHK, OnUseLoadChk)
	ON_BN_CLICKED(IDC_DGN_CPG_LOAD_TYPE_A_RADIO, OnTypeRadioClick)	
	ON_BN_CLICKED(IDC_DGN_CPG_LOAD_TYPE_B_RADIO, OnTypeRadioClick)	
	ON_BN_CLICKED(IDC_DGN_CPG_LOAD_TYPE_C_RADIO, OnTypeRadioClick)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCPGLoadApplicationTabIDlg message handlers


BOOL CCPGLoadApplicationTabIDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
		
	SetInitUnit();

	InitialDlgData();
	SetAllControlEnable();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCPGLoadApplicationTabIDlg::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_I_Data = m_Data;

	return CChildDialog::DestroyWindow();
}


void CCPGLoadApplicationTabIDlg::SetInitUnit()
{
	m_dA.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dSs.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dC.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
	m_dAUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dSsUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_dCUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);		
}


void CCPGLoadApplicationTabIDlg::OnTypeRadioClick() 
{
	UpdateData(TRUE);

	if(m_iType==0 || m_iType==1)//Type(a), Type(b) 일 경우
	{
		GetDlgItem(IDC_DGN_CPG_LOAD_A_STATIC)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_LOAD_A_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_LOAD_A_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_LOAD_S_STATIC)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_LOAD_S_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_LOAD_S_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_LOAD_C_STATIC)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_LOAD_C_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_LOAD_C_UNIT)->EnableWindow(false);		
	}
	else
	{
		GetDlgItem(IDC_DGN_CPG_LOAD_A_STATIC)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_LOAD_A_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_LOAD_A_UNIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_LOAD_S_STATIC)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_LOAD_S_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_LOAD_S_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_LOAD_C_STATIC)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_LOAD_C_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_LOAD_C_UNIT)->EnableWindow(true);
	}		
}

void CCPGLoadApplicationTabIDlg::OnUseLoadChk() 
{
	UpdateData(TRUE);

	if(m_bUseLoad)//철근 사용 입력 여부
	{
		GetDlgItem(IDC_DGN_CPG_LOAD_TYPE_A_RADIO)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_LOAD_TYPE_B_RADIO)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_LOAD_TYPE_C_RADIO)->EnableWindow(true);
		OnTypeRadioClick();		
	}
	else
	{		
		GetDlgItem(IDC_DGN_CPG_LOAD_TYPE_A_RADIO)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_LOAD_TYPE_B_RADIO)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_LOAD_TYPE_C_RADIO)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_LOAD_A_STATIC)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_LOAD_A_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_LOAD_A_UNIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_LOAD_S_STATIC)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_LOAD_S_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_LOAD_S_UNIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_LOAD_C_STATIC)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_LOAD_C_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_LOAD_C_UNIT)->EnableWindow(false);
	}	
}



void CCPGLoadApplicationTabIDlg::InitialDlgData()
{
	//m_Data.Initialize();

	Data2Dlg();
}

BOOL CCPGLoadApplicationTabIDlg::Data2Dlg()
{
	m_bUseLoad = m_Data.bUseTfType;
	m_iType = m_Data.iLoadAppType;
	
	m_dA.SetEditUnit(m_Data.da);
	m_dSs.SetEditUnit(m_Data.dSs);
	m_dC.SetEditUnit(m_Data.dC);
	
	UpdateData(FALSE);

	SetAllControlEnable();

	return TRUE;
}

BOOL CCPGLoadApplicationTabIDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.bUseTfType = m_bUseLoad;
	m_Data.iLoadAppType = m_iType;

	m_Data.da = m_dA.GetEditValue();
	m_Data.dSs = m_dSs.GetEditValue();
	m_Data.dC = m_dC.GetEditValue();	

	return TRUE;
}

void CCPGLoadApplicationTabIDlg::GetCptfData()
{
	Dlg2Data();

	m_pParent->m_I_Data = m_Data;
}

void CCPGLoadApplicationTabIDlg::SetCptfData()
{
	Data2Dlg();
}

void CCPGLoadApplicationTabIDlg::SetAllControlEnable()
{
	bool enable = true;
	//if(m_pParent->m_bBothIJ)
	//	enable = false;

	if(m_pParent->m_Tab.GetCurSel() == 0)
		enable = true;

	if(enable)
	{
		GetDlgItem(IDC_DGN_CPG_LOAD_USED_CHK)->EnableWindow(enable);
		OnUseLoadChk();
	}
	else
	{
		GetDlgItem(IDC_DGN_CPG_LOAD_USED_CHK)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_LOAD_TYPE_A_RADIO)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_LOAD_TYPE_B_RADIO)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_LOAD_TYPE_C_RADIO)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_LOAD_A_STATIC)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_LOAD_A_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_LOAD_A_UNIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_LOAD_S_STATIC)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_LOAD_S_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_LOAD_S_UNIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_LOAD_C_STATIC)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_LOAD_C_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_LOAD_C_UNIT)->EnableWindow(enable);		
	}	
}


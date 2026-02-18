// CPGTranStifTabJDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGTranStifTabJDlg.h"
#include "CPGTranStifDlg.h"

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
// CCPGTranStifTabJDlg dialog


CCPGTranStifTabJDlg::CCPGTranStifTabJDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCPGTranStifTabJDlg::IDD, pParent)
{
	//전역변수 초기화
	

	//{{AFX_DATA_INIT(CCPGTranStifTabJDlg)
	m_bUseStif = TRUE;
	m_bInterRigidStif = FALSE;
	m_bInterNonRigidStif = FALSE;
	m_iStifNum = 0;

	//m_dHt = 0.0;
	//m_dt = 0.0;
	//m_dPitch = 0.0;
	//m_dDistRstif = 0.0;
	//m_dDistNRstif = 0.0;
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CCPGTranStifDlg*)pParent;

	m_Data.Initialize();
}

CCPGTranStifTabJDlg::~CCPGTranStifTabJDlg()
{
	
}

void CCPGTranStifTabJDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPGTranStifTabJDlg)
	DDX_Check(pDX, IDC_DGN_CPG_STIF_USED_STIF_CHK, m_bUseStif);
	DDX_Check(pDX, IDC_DGN_CPG_STIF_RIGID_STIF_CHK, m_bInterRigidStif);
	DDX_Check(pDX, IDC_DGN_CPG_STIF_NONRIGID_STIF_CHK, m_bInterNonRigidStif);
	DDX_Radio(pDX, IDC_DGN_CPG_STIF_ONE_RADIO, m_iStifNum);

	DDX_Control(pDX, IDC_DGN_CPG_STIF_HT_EDIT, m_dHt);
	DDX_Control(pDX, IDC_DGN_CPG_STIF_T_EDIT, m_dt);
	DDX_Control(pDX, IDC_DGN_CPG_STIF_PITCH_EDIT, m_dPitch);
	DDX_Control(pDX, IDC_DGN_CPG_STIF_RIGID_EDIT, m_dDistRstif);
	DDX_Control(pDX, IDC_DGN_CPG_STIF_NONRIGID_EDIT, m_dDistNRstif);

	DDX_Control(pDX, IDC_DGN_CPG_STIF_HT_UNIT, m_dHtUnit);
	DDX_Control(pDX, IDC_DGN_CPG_STIF_T_UNIT, m_dtUnit);
	DDX_Control(pDX, IDC_DGN_CPG_STIF_PITCH_UNIT, m_dPitchUnit);
	DDX_Control(pDX, IDC_DGN_CPG_STIF_RIGID_UNIT, m_dDistRstifUnit);
	DDX_Control(pDX, IDC_DGN_CPG_STIF_NONRIGID_UNIT, m_dDistNRstifUnit);

	DDX_Control(pDX,IDC_DGN_CPG_STIF_USED_STIF_CHK, m_Ctrl_UseCheck);
	DDX_Control(pDX,IDC_DGN_CPG_STIF_ONE_RADIO, m_Ctrl_OneRadio);
	DDX_Control(pDX,IDC_DGN_CPG_STIF_TWO_RADIO, m_Ctrl_TwoRadio);	
	DDX_Control(pDX,IDC_DGN_CPG_STIF_RIGID_STIF_CHK, m_Ctrl_RigidCheck);
	DDX_Control(pDX,IDC_DGN_CPG_STIF_NONRIGID_STIF_CHK, m_Ctrl_NonRigidCheck);
	DDX_Control(pDX,IDC_DGN_CPG_STIF_STATIC, m_Ctrl_Static);
	DDX_Control(pDX,IDC_DGN_CPG_STIF_STATIC2, m_Ctrl_Static2);
	DDX_Control(pDX,IDC_DGN_CPG_STIF_STATIC3, m_Ctrl_Static3);
	DDX_Control(pDX,IDC_DGN_CPG_STIF_STATIC4, m_Ctrl_Static4);
	DDX_Control(pDX,IDC_DGN_CPG_STIF_STATIC5, m_Ctrl_Static5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPGTranStifTabJDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCPGTranStifTabJDlg)
	ON_BN_CLICKED(IDC_DGN_CPG_STIF_USED_STIF_CHK, OnUseStifChk)
	ON_BN_CLICKED(IDC_DGN_CPG_STIF_RIGID_STIF_CHK, OnRigidChk)
	ON_BN_CLICKED(IDC_DGN_CPG_STIF_NONRIGID_STIF_CHK, OnNonRigidChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCPGTranStifTabJDlg message handlers


BOOL CCPGTranStifTabJDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
		
	SetInitUnit();

	InitialDlgData();
	SetAllControlEnable(m_pParent->m_bJTabEnable);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCPGTranStifTabJDlg::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_J_Data = m_Data;

	return CChildDialog::DestroyWindow();
}

void CCPGTranStifTabJDlg::SetInitUnit()
{
	m_dHt.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dt.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dPitch.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dDistRstif.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dDistNRstif.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_dHtUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dtUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_dPitchUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_dDistRstifUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_dDistNRstifUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
}


void CCPGTranStifTabJDlg::OnUseStifChk() 
{
	UpdateData(TRUE);

	if(m_bUseStif)//철근 사용 입력 여부
	{
		GetDlgItem(IDC_DGN_CPG_STIF_ONE_RADIO)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_TWO_RADIO)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_STATIC)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_HT_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_HT_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_STATIC2)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_T_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_T_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_STATIC3)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_PITCH_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_PITCH_UNIT)->EnableWindow(true);

		GetDlgItem(IDC_DGN_CPG_STIF_RIGID_STIF_CHK)->EnableWindow(true);
		OnRigidChk();

		GetDlgItem(IDC_DGN_CPG_STIF_NONRIGID_STIF_CHK)->EnableWindow(true);
		OnNonRigidChk();
	}
	else
	{
		GetDlgItem(IDC_DGN_CPG_STIF_ONE_RADIO)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_TWO_RADIO)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_STATIC)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_HT_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_HT_UNIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_STATIC2)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_T_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_T_UNIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_STATIC3)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_PITCH_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_PITCH_UNIT)->EnableWindow(false);

		GetDlgItem(IDC_DGN_CPG_STIF_RIGID_STIF_CHK)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_STATIC4)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_RIGID_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_RIGID_UNIT)->EnableWindow(false);

		GetDlgItem(IDC_DGN_CPG_STIF_NONRIGID_STIF_CHK)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_STATIC5)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_NONRIGID_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_NONRIGID_UNIT)->EnableWindow(false);
	}	
}

void CCPGTranStifTabJDlg::OnRigidChk() 
{
	UpdateData(TRUE);

	if(m_bInterRigidStif)//Intermediate rigid transverse stiffener
	{
		GetDlgItem(IDC_DGN_CPG_STIF_STATIC4)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_RIGID_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_RIGID_UNIT)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_DGN_CPG_STIF_STATIC4)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_RIGID_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_RIGID_UNIT)->EnableWindow(false);
	}		
}

void CCPGTranStifTabJDlg::OnNonRigidChk() 
{
	UpdateData(TRUE);

	if(m_bInterNonRigidStif)//Intermediate non-rigid transverse stiffener
	{
		GetDlgItem(IDC_DGN_CPG_STIF_STATIC5)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_NONRIGID_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STIF_NONRIGID_UNIT)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_DGN_CPG_STIF_STATIC5)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_NONRIGID_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STIF_NONRIGID_UNIT)->EnableWindow(false);
	}		
}


void CCPGTranStifTabJDlg::InitialDlgData()
{
	//m_Data.Initialize();

	Data2Dlg();
}

BOOL CCPGTranStifTabJDlg::Data2Dlg()
{
	m_bUseStif = m_Data.bUseStif;
	m_iStifNum = m_Data.iStifNum;
	m_dHt.SetEditUnit(m_Data.dht);
	m_dt.SetEditUnit(m_Data.dt);
	m_dPitch.SetEditUnit(m_Data.dPitch);	
	m_bInterRigidStif = m_Data.bInterRigidStif;
	m_bInterNonRigidStif = m_Data.bInterNonRigidStif;
	m_dDistRstif.SetEditUnit(m_Data.dDistRstif);	
	m_dDistNRstif.SetEditUnit(m_Data.dDistNRstif);		

	UpdateData(FALSE);

	SetAllControlEnable(m_pParent->m_bJTabEnable);

	return TRUE;
}

BOOL CCPGTranStifTabJDlg::Dlg2Data()
{
	UpdateData(TRUE);

	//m_Data.bUseStif = m_Ctrl_UseCheck.GetCheck();
	//if(m_Ctrl_OneRadio.GetCheck())
	//	m_Data.iStifNum = 0;
	//else
	//	m_Data.iStifNum = 1;
	m_Data.bUseStif = m_bUseStif;
	m_Data.iStifNum = m_iStifNum;
	m_Data.dht = m_dHt.GetEditValue();
	m_Data.dt = m_dt.GetEditValue();
	m_Data.dPitch = m_dPitch.GetEditValue();
	//m_Data.bInterRigidStif = m_Ctrl_RigidCheck.GetCheck();
	//m_Data.bInterNonRigidStif = m_Ctrl_NonRigidCheck.GetCheck();
	m_Data.bInterRigidStif = m_bInterRigidStif;
	m_Data.bInterNonRigidStif = m_bInterNonRigidStif;
	m_Data.dDistRstif = m_dDistRstif.GetEditValue();
	m_Data.dDistNRstif = m_dDistNRstif.GetEditValue();	

	return TRUE;
}

void CCPGTranStifTabJDlg::GetCgtsData()
{
	Dlg2Data();

	m_pParent->m_J_Data = m_Data;
}

void CCPGTranStifTabJDlg::SetCgtsData()
{
	Data2Dlg();
}

void CCPGTranStifTabJDlg::SetAllControlEnable(bool enable)
{
	//bool enable = true;
	//if(m_pParent->m_bBothIJ)
	//	enable = false;

	if(enable)
	{
		m_Ctrl_UseCheck.EnableWindow(true);
		OnUseStifChk();
	}
	else
	{
		m_Ctrl_UseCheck.EnableWindow(false);

		m_dHt.EnableWindow(false);
		m_dt.EnableWindow(false);
		m_dPitch.EnableWindow(false);
		m_dDistRstif.EnableWindow(false);
		m_dDistNRstif.EnableWindow(false);
		m_dHtUnit.EnableWindow(false);
		m_dtUnit.EnableWindow(false);
		m_dPitchUnit.EnableWindow(false);
		m_dDistRstifUnit.EnableWindow(false);
		m_dDistNRstifUnit.EnableWindow(false);

		m_Ctrl_OneRadio.EnableWindow(false);
		m_Ctrl_TwoRadio.EnableWindow(false);
		m_Ctrl_RigidCheck.EnableWindow(false);
		m_Ctrl_NonRigidCheck.EnableWindow(false);
		m_Ctrl_Static.EnableWindow(false);
		m_Ctrl_Static2.EnableWindow(false);
		m_Ctrl_Static3.EnableWindow(false);
		m_Ctrl_Static4.EnableWindow(false);
		m_Ctrl_Static5.EnableWindow(false);

		/*
		GetDlgItem(IDC_DGN_CPG_STIF_USED_STIF_CHK)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STIF_ONE_RADIO)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STIF_TWO_RADIO)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STIF_STATIC)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STIF_HT_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STIF_HT_UNIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STIF_STATIC2)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STIF_T_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STIF_T_UNIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STIF_STATIC3)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STIF_PITCH_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STIF_PITCH_UNIT)->EnableWindow(enable);

		GetDlgItem(IDC_DGN_CPG_STIF_RIGID_STIF_CHK)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STIF_STATIC4)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STIF_RIGID_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STIF_RIGID_UNIT)->EnableWindow(enable);

		GetDlgItem(IDC_DGN_CPG_STIF_NONRIGID_STIF_CHK)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STIF_STATIC5)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STIF_NONRIGID_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STIF_NONRIGID_UNIT)->EnableWindow(enable);
		*/
	}
}


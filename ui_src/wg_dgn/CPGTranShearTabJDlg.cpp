// CPGTranShearTabJDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGTranShearTabJDlg.h"
#include "CPGTranShearDlg.h"

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
// CCPGTranShearTabJDlg dialog


CCPGTranShearTabJDlg::CCPGTranShearTabJDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCPGTranShearTabJDlg::IDD, pParent)
{
	//전역변수 초기화
	

	//{{AFX_DATA_INIT(CCPGTranShearTabJDlg)
	m_bUseShear = TRUE;	
	m_iShearNum = 0;
	
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CCPGTranShearDlg*)pParent;

	m_Data.Initialize();
}


void CCPGTranShearTabJDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPGTranShearTabJDlg)
	DDX_Check(pDX, IDC_DGN_CPG_SHEAR_USED_SHEAR_CHK, m_bUseShear);
	
	DDX_Text(pDX, IDC_DGN_CPG_SHEAR_NUM_EDIT, m_iShearNum);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_FU_EDIT, m_dFu);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_DS_EDIT, m_dDs);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_HSC_EDIT, m_dHsc);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_ST_EDIT, m_dSt);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_SC_EDIT, m_dSc);

	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_FU_UNIT, m_dFuUnit);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_DS_UNIT, m_dDsUnit);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_HSC_UNIT, m_dHscUnit);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_ST_UNIT, m_dStUnit);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_SC_UNIT, m_dScUnit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPGTranShearTabJDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCPGTranShearTabJDlg)
	ON_BN_CLICKED(IDC_DGN_CPG_SHEAR_USED_SHEAR_CHK, OnUseShearChk)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCPGTranShearTabJDlg message handlers


BOOL CCPGTranShearTabJDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
		
	SetInitUnit();

	InitialDlgData();
	SetAllControlEnable(m_pParent->m_bJTabEnable);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCPGTranShearTabJDlg::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_J_Data = m_Data;

	return CChildDialog::DestroyWindow();
}


void CCPGTranShearTabJDlg::SetInitUnit()
{
	m_dFu.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_dDs.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dHsc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dSt.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dSc.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_dFuUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_dDsUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_dHscUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_dStUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_dScUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
}


void CCPGTranShearTabJDlg::OnUseShearChk() 
{
	UpdateData(TRUE);

	if(m_bUseShear)//철근 사용 입력 여부
	{
		GetDlgItem(IDC_DGN_CPG_SHEAR_STATIC)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_STATIC2)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_STATIC3)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_STATIC4)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_STATIC5)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_STATIC6)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_NUM_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_FU_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_DS_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_HSC_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_ST_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_SC_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_FU_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_DS_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_HSC_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_ST_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_SC_UNIT)->EnableWindow(true);
		
	}
	else
	{
		GetDlgItem(IDC_DGN_CPG_SHEAR_STATIC)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_STATIC2)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_STATIC3)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_STATIC4)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_STATIC5)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_STATIC6)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_NUM_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_FU_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_DS_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_HSC_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_ST_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_SC_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_FU_UNIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_DS_UNIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_HSC_UNIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_ST_UNIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_SC_UNIT)->EnableWindow(false);
	}		
}



void CCPGTranShearTabJDlg::InitialDlgData()
{
	//m_Data.Initialize();

	Data2Dlg();
}

BOOL CCPGTranShearTabJDlg::Data2Dlg()
{
	m_bUseShear = m_Data.bUseShear;
	m_iShearNum = m_Data.iNum;
	m_dFu.SetEditUnit(m_Data.dfu);
	m_dDs.SetEditUnit(m_Data.dDiameter);
	m_dHsc.SetEditUnit(m_Data.dhsc);
	m_dSt.SetEditUnit(m_Data.dSt);
	m_dSc.SetEditUnit(m_Data.dSpace);

	UpdateData(FALSE);

	SetAllControlEnable(m_pParent->m_bJTabEnable);

	return TRUE;
}

BOOL CCPGTranShearTabJDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.bUseShear = m_bUseShear;
	m_Data.iNum = m_iShearNum;
	m_Data.dfu = m_dFu.GetEditValue();
	m_Data.dDiameter = m_dDs.GetEditValue();
	m_Data.dhsc = m_dHsc.GetEditValue();
	m_Data.dSt = m_dSt.GetEditValue();
	m_Data.dSpace = m_dSc.GetEditValue();	

	return TRUE;
}

void CCPGTranShearTabJDlg::GetCgscData()
{
	Dlg2Data();

	m_pParent->m_J_Data = m_Data;
}

void CCPGTranShearTabJDlg::SetCgscData()
{
	Data2Dlg();
}

void CCPGTranShearTabJDlg::SetAllControlEnable(bool enable)
{
	//bool enable = true;
	//if(m_pParent->m_bBothIJ)
	//	enable = false;

	if(enable)
	{
		GetDlgItem(IDC_DGN_CPG_SHEAR_USED_SHEAR_CHK)->EnableWindow(enable);
		OnUseShearChk();
	}
	else
	{
	GetDlgItem(IDC_DGN_CPG_SHEAR_USED_SHEAR_CHK)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_STATIC)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_STATIC2)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_STATIC3)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_STATIC4)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_STATIC5)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_STATIC6)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_NUM_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_FU_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_DS_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_HSC_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_ST_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_SC_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_FU_UNIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_DS_UNIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_HSC_UNIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_ST_UNIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_SC_UNIT)->EnableWindow(enable);
	}	
}


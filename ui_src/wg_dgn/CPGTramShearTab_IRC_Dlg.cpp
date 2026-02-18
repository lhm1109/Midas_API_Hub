// CPGTranShearTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGTramShearTab_IRC_Dlg.h"
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
// CCPGTranShearTab_IRC_Dlg dialog


CCPGTranShearTab_IRC_Dlg::CCPGTranShearTab_IRC_Dlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCPGTranShearTab_IRC_Dlg::IDD, pParent)
{
	//전역변수 초기화


	//{{AFX_DATA_INIT(CCPGTranShearTab_IRC_Dlg)
	m_bUseShear = TRUE;	
	m_iShearNum = 0;

	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CCPGTranShearDlg*)pParent;

	m_Data.Initialize();
}


void CCPGTranShearTab_IRC_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPGTranShearTab_IRC_Dlg)
	DDX_Check(pDX, IDC_DGN_CPG_SHEAR_USED_SHEAR_CHK, m_bUseShear);

	DDX_Text(pDX, IDC_DGN_CPG_SHEAR_NUM_EDIT, m_iShearNum);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_FU_EDIT, m_dFu);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_DS_EDIT, m_dDs);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_HSC_EDIT, m_dHsc);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_ST_EDIT, m_dSt);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_SC_EDIT, m_dSc);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_FATIGUE_EDIT , m_dQr);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_TRA_RF_EDIT  , m_dAst);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_PLANE_EDIT   ,    m_dL);

	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_FU_UNIT, m_dFuUnit);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_DS_UNIT, m_dDsUnit);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_HSC_UNIT, m_dHscUnit);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_ST_UNIT, m_dStUnit);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_SC_UNIT, m_dScUnit);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_FATIGUE_UNIT , m_dQrUnit );
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_TRA_RF_UNIT2 , m_dAstUnit);
	DDX_Control(pDX, IDC_DGN_CPG_SHEAR_PLANE_UNIT   ,     m_dLUnit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPGTranShearTab_IRC_Dlg, CChildDialog)
	//{{AFX_MSG_MAP(CCPGTranShearTab_IRC_Dlg)
	ON_BN_CLICKED(IDC_DGN_CPG_SHEAR_USED_SHEAR_CHK, OnUseShearChk)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCPGTranShearTab_IRC_Dlg message handlers


BOOL CCPGTranShearTab_IRC_Dlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();


	SetInitUnit();

	InitialDlgData();
	SetAllControlEnable();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCPGTranShearTab_IRC_Dlg::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_I_Data = m_Data;

	return CChildDialog::DestroyWindow();
}


void CCPGTranShearTab_IRC_Dlg::SetInitUnit()
{
	m_dFu.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_dDs.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dHsc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dSt.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dSc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dQr.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_dAst.SetUnitType(D_UNITSYS_BASE_UNIT_AREA);
	m_dL.SetUnitType(D_UNITSYS_BASE_LENGTH);


	m_dFuUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_dDsUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_dHscUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_dStUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_dScUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_dQrUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_dAstUnit.SetUnitType(D_UNITSYS_BASE_UNIT_AREA);
	m_dLUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
}


void CCPGTranShearTab_IRC_Dlg::OnUseShearChk() 
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
		GetDlgItem(IDC_DGN_CPG_SHEAR_FATIGUE_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_TRA_RF_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_PLANE_EDIT )->EnableWindow(true);

		GetDlgItem(IDC_DGN_CPG_SHEAR_FU_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_DS_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_HSC_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_ST_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_SC_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_FATIGUE_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_TRA_RF_UNIT2)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_SHEAR_PLANE_UNIT)->EnableWindow(true);

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
		GetDlgItem(IDC_DGN_CPG_SHEAR_FATIGUE_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_TRA_RF_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_PLANE_EDIT )->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_FU_UNIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_DS_UNIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_HSC_UNIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_ST_UNIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_SC_UNIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_FATIGUE_UNIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_TRA_RF_UNIT2)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_SHEAR_PLANE_UNIT)->EnableWindow(false);
	}		
}



void CCPGTranShearTab_IRC_Dlg::InitialDlgData()
{
	//m_Data.Initialize();

	Data2Dlg();
}

BOOL CCPGTranShearTab_IRC_Dlg::Data2Dlg()
{
	m_bUseShear = m_Data.bUseShear;
	m_iShearNum = m_Data.iNum;
	m_dFu.SetEditUnit(m_Data.dfu);
	m_dDs.SetEditUnit(m_Data.dDiameter);
	m_dHsc.SetEditUnit(m_Data.dhsc);
	m_dSt.SetEditUnit(m_Data.dSt);
	m_dSc.SetEditUnit(m_Data.dSpace);
	m_dQr.SetEditUnit(m_Data.dQr);
	m_dAst.SetEditUnit(m_Data.dAst);
	m_dL.SetEditUnit(m_Data.dPlaneLen);


	UpdateData(FALSE);

	SetAllControlEnable();

	return TRUE;
}

BOOL CCPGTranShearTab_IRC_Dlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.bUseShear = m_bUseShear;
	m_Data.iNum = m_iShearNum;
	m_Data.dfu = m_dFu.GetEditValue();
	m_Data.dDiameter = m_dDs.GetEditValue();
	m_Data.dhsc = m_dHsc.GetEditValue();
	m_Data.dSt = m_dSt.GetEditValue();
	m_Data.dSpace = m_dSc.GetEditValue();	
	m_Data.dQr = m_dQr.GetEditValue();
	m_Data.dAst =m_dAst.GetEditValue();
	m_Data.dPlaneLen =m_dL.GetEditValue();
	return TRUE;
}

void CCPGTranShearTab_IRC_Dlg::GetCgscData()
{
	Dlg2Data();

	m_pParent->m_I_Data = m_Data;
}

void CCPGTranShearTab_IRC_Dlg::SetCgscData()
{
	Data2Dlg();
}

void CCPGTranShearTab_IRC_Dlg::SetAllControlEnable()
{
	bool enable = true;
	//if(m_pParent->m_bBothIJ)
	//	enable = false;

	if(m_pParent->m_Tab.GetCurSel() == 0)
		enable = true;

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
		GetDlgItem(IDC_DGN_CPG_SHEAR_STATIC7)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_STATIC8)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_STATIC9)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_NUM_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_FU_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_DS_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_HSC_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_ST_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_SC_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_FATIGUE_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_TRA_RF_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_PLANE_EDIT )->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_FU_UNIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_DS_UNIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_HSC_UNIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_ST_UNIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_SC_UNIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_FATIGUE_UNIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_TRA_RF_UNIT2)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_SHEAR_PLANE_UNIT)->EnableWindow(enable);
	}	
}



// CPGLateralTorsionTabJ_IRC_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGLateralTorsionTabJ_IRC_Dlg.h"
#include "CPGLateraTorsion_IRC_Dlg.h"

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
// CCPGLateralTorsionTabJ_IRC_Dlg dialog


CCPGLateralTorsionTabJ_IRC_Dlg::CCPGLateralTorsionTabJ_IRC_Dlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCPGLateralTorsionTabJ_IRC_Dlg::IDD, pParent)
{
	//전역변수 초기화
	

	//{{AFX_DATA_INIT(CCPGLateralTorsionTabJ_IRC_Dlg)
	m_bUse = TRUE;	
	m_iAlpha = 0;
		
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CCPGLateraTorsion_IRC_Dlg*)pParent;

	m_Data.Initialize();

// 	m_aAlphaCtrl.RemoveAll();
// 	m_aAlphaCtrl.Add(IDC_DGN_CPG_STATIC3);
// 	m_aAlphaCtrl.Add(IDC_DGN_CPG_ALPHA_EDIT);
// 
// 	m_aSBPBCtrl.RemoveAll();
// 	m_aSBPBCtrl.Add(IDC_DGN_CPG_STATIC4);
// 	m_aSBPBCtrl.Add(IDC_DGN_CPG_STATIC5);
// 	m_aSBPBCtrl.Add(IDC_DGN_CPG_A_EDIT);
// 	m_aSBPBCtrl.Add(IDC_DGN_CPG_A_UNIT);
}


void CCPGLateralTorsionTabJ_IRC_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPGLateralTorsionTabJ_IRC_Dlg)
	DDX_Check(pDX, IDC_DGN_CPG_USED_CHK, m_bUse);
	
	
	DDX_Control(pDX, IDC_DGN_CPG_I_EDIT, m_dI);

	
	DDX_Control(pDX, IDC_DGN_CPG_I_UNIT, m_dIUnit);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPGLateralTorsionTabJ_IRC_Dlg, CChildDialog)
	//{{AFX_MSG_MAP(CCPGLateralTorsionTabJ_IRC_Dlg)
	ON_BN_CLICKED(IDC_DGN_CPG_USED_CHK, OnUseChk)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCPGLateralTorsionTabJ_IRC_Dlg message handlers


BOOL CCPGLateralTorsionTabJ_IRC_Dlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();	
		
	SetInitUnit();

	InitialDlgData();
	SetAllControlEnable(m_pParent->m_bJTabEnable);

// 	for(int i = 0; i < m_aAlphaCtrl.GetSize(); ++i)
// 	{
// 		GetDlgItem(m_aAlphaCtrl[i])->ShowWindow(FALSE);
// 	}
// 
// 	for(int i = 0; i < m_aSBPBCtrl.GetSize(); ++i)
// 	{
// 		GetDlgItem(m_aSBPBCtrl[i])->ShowWindow(FALSE);
// 	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCPGLateralTorsionTabJ_IRC_Dlg::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_J_Data = m_Data;

	return CChildDialog::DestroyWindow();
}


void CCPGLateralTorsionTabJ_IRC_Dlg::SetInitUnit()
{
	m_dI.SetUnitType(D_UNITSYS_BASE_LENGTH);
	

	m_dIUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
}


void CCPGLateralTorsionTabJ_IRC_Dlg::OnUseChk() 
{
	UpdateData(TRUE);

	if(m_bUse)//철근 사용 입력 여부
	{
		GetDlgItem(IDC_DGN_CPG_STATIC)->EnableWindow(true);
		
		GetDlgItem(IDC_DGN_CPG_I_EDIT)->EnableWindow(true);
		
		GetDlgItem(IDC_DGN_CPG_I_UNIT)->EnableWindow(true);
			
	}
	else
	{
		GetDlgItem(IDC_DGN_CPG_STATIC)->EnableWindow(false);
		
		GetDlgItem(IDC_DGN_CPG_I_EDIT)->EnableWindow(false);
		
		GetDlgItem(IDC_DGN_CPG_I_UNIT)->EnableWindow(false);
		
	}		
}



void CCPGLateralTorsionTabJ_IRC_Dlg::InitialDlgData()
{
	//m_Data.Initialize();

	Data2Dlg();
}

BOOL CCPGLateralTorsionTabJ_IRC_Dlg::Data2Dlg()
{
	m_bUse = m_Data.bUseData;	
	m_dI.SetEditUnit(m_Data.dl);

	UpdateData(FALSE);

	SetAllControlEnable(m_pParent->m_bJTabEnable);

	return TRUE;
}

BOOL CCPGLateralTorsionTabJ_IRC_Dlg::Dlg2Data()
{
	UpdateData(TRUE);

		m_Data.bUseData = m_bUse;
	m_Data.dCd = 1.;
	m_Data.dl = m_dI.GetEditValue();
		m_Data.da = 1.;
		m_Data.iAlpha = 1.;

	return TRUE;
}

void CCPGLateralTorsionTabJ_IRC_Dlg::GetCgltData()
{
	Dlg2Data();

	m_pParent->m_J_Data = m_Data;
}

void CCPGLateralTorsionTabJ_IRC_Dlg::SetCgltData()
{
	Data2Dlg();
}

void CCPGLateralTorsionTabJ_IRC_Dlg::SetAllControlEnable(bool enable)
{
	//bool enable = true;
	//if(m_pParent->m_bBothIJ)
	//	enable = false;

	if(enable)
	{
		GetDlgItem(IDC_DGN_CPG_USED_CHK)->EnableWindow(enable);
		OnUseChk();
	}
	else
	{
		GetDlgItem(IDC_DGN_CPG_USED_CHK)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STATIC)->EnableWindow(enable);
	
		GetDlgItem(IDC_DGN_CPG_I_EDIT)->EnableWindow(enable);
		
		GetDlgItem(IDC_DGN_CPG_I_UNIT)->EnableWindow(enable);
				
	}	
}


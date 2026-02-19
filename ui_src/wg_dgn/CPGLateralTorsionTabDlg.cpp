// CPGLateralTorsionTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGLateralTorsionTabDlg.h"
#include "CPGLateraTorsionDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_DlgUtil.h"

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
// CCPGLateralTorsionTabDlg dialog


CCPGLateralTorsionTabDlg::CCPGLateralTorsionTabDlg(CWnd* pParent /*=NULL*/, int nType/* = 0*/)
	: CChildDialog(CCPGLateralTorsionTabDlg::IDD, pParent)
{
	//전역변수 초기화
	

	//{{AFX_DATA_INIT(CCPGLateralTorsionTabDlg)
	m_nType = nType;
	m_bUse = TRUE;	
	m_iAlpha = 0;
		
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CCPGLateraTorsionDlg*)pParent;

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


void CCPGLateralTorsionTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPGLateralTorsionTabDlg)
	DDX_Check(pDX, IDC_DGN_CPG_USED_CHK, m_bUse);
	
	DDX_Text(pDX, IDC_DGN_CPG_ALPHA_EDIT, m_iAlpha);
	DDX_Control(pDX, IDC_DGN_CPG_I_EDIT, m_dI);
	DDX_Control(pDX, IDC_DGN_CPG_CD_EDIT, m_dCd);	
	DDX_Control(pDX, IDC_DGN_CPG_A_EDIT, m_dA);
	
	DDX_Control(pDX, IDC_DGN_CPG_I_UNIT, m_dIUnit);
	DDX_Control(pDX, IDC_DGN_CPG_CD_UNIT, m_dCdUnit);	
	DDX_Control(pDX, IDC_DGN_CPG_A_UNIT, m_dAUnit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPGLateralTorsionTabDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCPGLateralTorsionTabDlg)
	ON_BN_CLICKED(IDC_DGN_CPG_USED_CHK, OnUseChk)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCPGLateralTorsionTabDlg message handlers


BOOL CCPGLateralTorsionTabDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	InitCtrl();
	SetInitUnit();

	InitialDlgData();
	SetAllControlEnable();

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

BOOL CCPGLateralTorsionTabDlg::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_I_Data = m_Data;

	return CChildDialog::DestroyWindow();
}


void CCPGLateralTorsionTabDlg::InitCtrl()
{
	if (m_nType == 1) // Steel Rating
	{
		CDlgUtil::CtrlShowHideByRect(this, IDC_DGN_CPG_GROUP, FALSE, TRUE);

		CArray<UINT, UINT> aControls;
		aControls.RemoveAll();
		aControls.Add(IDC_DGN_CPG_STATIC4);
		aControls.Add(IDC_DGN_CPG_STATIC5);
		aControls.Add(IDC_DGN_CPG_A_EDIT);
		aControls.Add(IDC_DGN_CPG_A_UNIT);

		CDlgUtil::CtrlShowHide(this, aControls, TRUE);

		CRect rRef;
		CRect rToMove;
		int nDistX, nDistY;

		GetDlgItem(IDC_DGN_CPG_USED_CHK)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CPG_STATIC4)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.top  - rToMove.top+globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistXY(this, aControls, nDistX, nDistY);
	}
}

void CCPGLateralTorsionTabDlg::SetInitUnit()
{
	m_dI.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dCd.SetUnitType(D_UNITSYS_BASE_UNITFORCE);	
	m_dA.SetUnitType(D_UNITSYS_BASE_LENGTH);	

	m_dIUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dCdUnit.SetUnitType(D_UNITSYS_BASE_UNITFORCE);	
	m_dAUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
}


void CCPGLateralTorsionTabDlg::OnUseChk() 
{
	UpdateData(TRUE);

	if(m_bUse)//철근 사용 입력 여부
	{
		GetDlgItem(IDC_DGN_CPG_STATIC)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STATIC2)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STATIC3)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STATIC4)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STATIC5)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_I_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_CD_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_ALPHA_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_A_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_I_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_CD_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_A_UNIT)->EnableWindow(true);		
	}
	else
	{
		GetDlgItem(IDC_DGN_CPG_STATIC)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STATIC2)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STATIC3)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STATIC4)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STATIC5)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_I_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_CD_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_ALPHA_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_A_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_I_UNIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_CD_UNIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_A_UNIT)->EnableWindow(false);		
	}		
}



void CCPGLateralTorsionTabDlg::InitialDlgData()
{
	//m_Data.Initialize();

	Data2Dlg();
}

BOOL CCPGLateralTorsionTabDlg::Data2Dlg()
{
	m_bUse = m_Data.bUseData;

	m_iAlpha = m_Data.iAlpha;
	m_dI.SetEditUnit(m_Data.dl);
	m_dCd.SetEditUnit(m_Data.dCd);
	m_dA.SetEditUnit(m_Data.da);	

	UpdateData(FALSE);

	SetAllControlEnable();

	return TRUE;
}

BOOL CCPGLateralTorsionTabDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.bUseData = m_bUse;

	m_Data.iAlpha = m_iAlpha;
	m_Data.dl = m_dI.GetEditValue();
	m_Data.dCd = m_dCd.GetEditValue();
	m_Data.da = m_dA.GetEditValue();

	return TRUE;
}

void CCPGLateralTorsionTabDlg::GetCgltData()
{
	Dlg2Data();

	m_pParent->m_I_Data = m_Data;
}

void CCPGLateralTorsionTabDlg::SetCgltData()
{
	Data2Dlg();
}

void CCPGLateralTorsionTabDlg::SetAllControlEnable()
{
	bool enable = true;
	//if(m_pParent->m_bBothIJ)
	//	enable = false;

	if(m_pParent->m_Tab.GetCurSel() == 0)
		enable = true;

	if(enable)
	{
		GetDlgItem(IDC_DGN_CPG_USED_CHK)->EnableWindow(enable);
		OnUseChk();
	}
	else
	{
		GetDlgItem(IDC_DGN_CPG_USED_CHK)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STATIC)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STATIC2)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STATIC3)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STATIC4)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_STATIC5)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_I_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_CD_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_ALPHA_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_A_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_I_UNIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_CD_UNIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_A_UNIT)->EnableWindow(enable);			
	}	
}


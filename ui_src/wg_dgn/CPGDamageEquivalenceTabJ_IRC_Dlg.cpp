// CPGDamageEquivalenceTabJDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGDamageEquivalenceTabJ_IRC_Dlg.h"
#include "CPGDamageEquivalence_IRC_Dlg.h"

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
// CCPGDamageEquivalenceTabJ_IRC_Dlg dialog


CCPGDamageEquivalenceTabJ_IRC_Dlg::CCPGDamageEquivalenceTabJ_IRC_Dlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCPGDamageEquivalenceTabJ_IRC_Dlg::IDD, pParent)
{
	//전역변수 초기화
	

	//{{AFX_DATA_INIT(CCPGDamageEquivalenceTabJ_IRC_Dlg)
	m_bUse = TRUE;	
	m_dLamdaV2 = 0.0;
	m_dLamdaV4 = 0.0;
	m_dCorrFact = 0.0;
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CCPGDamageEquivalence_IRC_Dlg*)pParent;

	m_Data.Initialize();
}


void CCPGDamageEquivalenceTabJ_IRC_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPGDamageEquivalenceTabJ_IRC_Dlg)
	DDX_Check(pDX, IDC_DGN_CPG_USED_CHK, m_bUse);
	
	DDX_Text(pDX, IDC_DGN_CPG_LAMDA_V2_EDIT, m_dLamdaV2);
	DDX_Text(pDX, IDC_DGN_CPG_LAMDA_V4_EDIT, m_dLamdaV4);
		DDX_Text(pDX, IDC_DGN_CPG_LAMDA_V4_EDIT2, m_dCorrFact);
		DDX_Control(pDX, IDC_DGN_CPG_DAMAGE_EQUI_FF_UNIT,m_nUnitFf );
		DDX_Control(pDX, IDC_DGN_CPG_DAMAGE_EQUI_TF_UNIT,m_nUnitTf );
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPGDamageEquivalenceTabJ_IRC_Dlg, CChildDialog)
	//{{AFX_MSG_MAP(CCPGDamageEquivalenceTabJ_IRC_Dlg)
	ON_BN_CLICKED(IDC_DGN_CPG_USED_CHK, OnUseChk)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCPGDamageEquivalenceTabJ_IRC_Dlg message handlers


BOOL CCPGDamageEquivalenceTabJ_IRC_Dlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
		
	SetInitUnit();

	InitialDlgData();
	SetAllControlEnable(m_pParent->m_bJTabEnable);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCPGDamageEquivalenceTabJ_IRC_Dlg::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_J_Data = m_Data;

	return CChildDialog::DestroyWindow();
}


void CCPGDamageEquivalenceTabJ_IRC_Dlg::SetInitUnit()
{
	m_nUnitFf.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_nUnitTf.SetUnitType(D_UNITSYS_BASE_STRESS);
}


void CCPGDamageEquivalenceTabJ_IRC_Dlg::OnUseChk() 
{
	UpdateData(TRUE);

	T_CPGD_D CpgdD; CpgdD.Initialize();
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);

	if(CpgdD.iDgnCode == IRC22_08_CSG) 
	{
		GetDlgItem(IDC_DGN_CPG_USED_CHK)->SetWindowText(_LS(IDS_DGN_CPG_FAT_ASSESMENT));
		GetDlgItem(IDC_DGN_CPG_STATIC)->SetWindowText(_LS(IDS_DGN_CPG_FAT_NOR_FAT_STREMGTH));
		GetDlgItem(IDC_DGN_CPG_STATIC2)->SetWindowText(_LS(IDS_DGN_CPG_FAT_SHEAR_FAT_STREMGTH));

	}




	if(m_bUse)//철근 사용 입력 여부
	{
		GetDlgItem(IDC_DGN_CPG_STATIC)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STATIC2)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_LAMDA_V2_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_LAMDA_V4_EDIT)->EnableWindow(true);	
		GetDlgItem(IDC_DGN_CPG_STATIC3)->EnableWindow(true);	
		GetDlgItem(IDC_DGN_CPG_LAMDA_V4_EDIT2)->EnableWindow(true);	
		GetDlgItem(IDC_DGN_CPG_DAMAGE_EQUI_FF_UNIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_DAMAGE_EQUI_TF_UNIT)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_DGN_CPG_STATIC)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STATIC2)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_LAMDA_V2_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_LAMDA_V4_EDIT)->EnableWindow(false);	
		GetDlgItem(IDC_DGN_CPG_STATIC3)->EnableWindow(false);	
		GetDlgItem(IDC_DGN_CPG_LAMDA_V4_EDIT2)->EnableWindow(false);	
		GetDlgItem(IDC_DGN_CPG_DAMAGE_EQUI_FF_UNIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_DAMAGE_EQUI_TF_UNIT)->EnableWindow(false);
	}		
}



void CCPGDamageEquivalenceTabJ_IRC_Dlg::InitialDlgData()
{
	//m_Data.Initialize();

	Data2Dlg();
}

BOOL CCPGDamageEquivalenceTabJ_IRC_Dlg::Data2Dlg()
{
	T_CPGD_D CpgdD; CpgdD.Initialize();
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);
	m_Data.bUseData = CpgdD.bStrength[5];

	m_bUse = m_Data.bUseData;

	m_dLamdaV2 = m_Data.dLamda_v2;
	m_dLamdaV4 = m_Data.dLamda_v4;	
	m_dCorrFact = m_Data.dCorrect_fact;
	UpdateData(FALSE);

	SetAllControlEnable(m_pParent->m_bJTabEnable);

	return TRUE;
}

BOOL CCPGDamageEquivalenceTabJ_IRC_Dlg::Dlg2Data()
{
	UpdateData(TRUE);
	T_CPGD_D CpgdD; CpgdD.Initialize();
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);
	m_bUse = CpgdD.bStrength[5];

	m_Data.bUseData = m_bUse;

	m_Data.dLamda_v2 = m_dLamdaV2;
	m_Data.dLamda_v4 = m_dLamdaV4;
	m_Data.dCorrect_fact = m_dCorrFact;
	return TRUE;
}

void CCPGDamageEquivalenceTabJ_IRC_Dlg::GetCgfrData()
{
	Dlg2Data();

	m_pParent->m_J_Data = m_Data;
}

void CCPGDamageEquivalenceTabJ_IRC_Dlg::SetCgfrData()
{
	Data2Dlg();
}

void CCPGDamageEquivalenceTabJ_IRC_Dlg::SetAllControlEnable(bool enable)
{
	//bool enable = true;
	//if(m_pParent->m_bBothIJ)
	//	enable = false;	

	T_CPGD_D CpgdD; CpgdD.Initialize();
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);

	if(CpgdD.iDgnCode == IRC22_08_CSG) 
	{
		GetDlgItem(IDC_DGN_CPG_USED_CHK)->SetWindowText(_T("Fatigue Assessment"));
		GetDlgItem(IDC_DGN_CPG_STATIC)->SetWindowText(_T("Uncorrected Normal Fatigue Strength(f_f)"));
		GetDlgItem(IDC_DGN_CPG_STATIC2)->SetWindowText(_T("Uncorrected Shear Fatigue Strength(T_f)"));
	}
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
		GetDlgItem(IDC_DGN_CPG_LAMDA_V2_EDIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_LAMDA_V4_EDIT)->EnableWindow(enable);	
		GetDlgItem(IDC_DGN_CPG_STATIC3)->EnableWindow(enable);	
		GetDlgItem(IDC_DGN_CPG_LAMDA_V4_EDIT2)->EnableWindow(enable);	
		GetDlgItem(IDC_DGN_CPG_DAMAGE_EQUI_FF_UNIT)->EnableWindow(enable);
		GetDlgItem(IDC_DGN_CPG_DAMAGE_EQUI_TF_UNIT)->EnableWindow(enable);
	}	
}


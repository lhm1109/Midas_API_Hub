// CPGDamageEquivalenceTabJDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGDamageEquivalenceTabJDlg.h"
#include "CPGDamageEquivalenceDlg.h"

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
// CCPGDamageEquivalenceTabJDlg dialog


CCPGDamageEquivalenceTabJDlg::CCPGDamageEquivalenceTabJDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCPGDamageEquivalenceTabJDlg::IDD, pParent)
{
	//전역변수 초기화
	

	//{{AFX_DATA_INIT(CCPGDamageEquivalenceTabJDlg)
	m_bUse = TRUE;	
	m_dLamdaV2 = 0.0;
	m_dLamdaV4 = 0.0;
		
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CCPGDamageEquivalenceDlg*)pParent;

	m_Data.Initialize();
}


void CCPGDamageEquivalenceTabJDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPGDamageEquivalenceTabJDlg)
	DDX_Check(pDX, IDC_DGN_CPG_USED_CHK, m_bUse);
	
	DDX_Text(pDX, IDC_DGN_CPG_LAMDA_V2_EDIT, m_dLamdaV2);
	DDX_Text(pDX, IDC_DGN_CPG_LAMDA_V4_EDIT, m_dLamdaV4);
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPGDamageEquivalenceTabJDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCPGDamageEquivalenceTabJDlg)
	ON_BN_CLICKED(IDC_DGN_CPG_USED_CHK, OnUseChk)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCPGDamageEquivalenceTabJDlg message handlers


BOOL CCPGDamageEquivalenceTabJDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
		
	SetInitUnit();

	InitialDlgData();
	SetAllControlEnable(m_pParent->m_bJTabEnable);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCPGDamageEquivalenceTabJDlg::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_J_Data = m_Data;

	return CChildDialog::DestroyWindow();
}


void CCPGDamageEquivalenceTabJDlg::SetInitUnit()
{
	
}


void CCPGDamageEquivalenceTabJDlg::OnUseChk() 
{
	UpdateData(TRUE);
	T_CPGD_D CpgdD; CpgdD.Initialize();
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);

	if(CpgdD.iDgnCode == IRC22_08_CSG) 
	{
		GetDlgItem(IDC_DGN_CPG_USED_CHK)->SetWindowText(_T("Fatigue Assessment"));
		GetDlgItem(IDC_DGN_CPG_STATIC)->SetWindowText(_T("Uncorrected Normal Fatigue Strength(f_f)"));
		GetDlgItem(IDC_DGN_CPG_STATIC2)->SetWindowText(_T("Uncorrected Shear Fatigue Strength(T_f)"));
	}
	if(m_bUse)//철근 사용 입력 여부
	{
		GetDlgItem(IDC_DGN_CPG_STATIC)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_STATIC2)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_LAMDA_V2_EDIT)->EnableWindow(true);
		GetDlgItem(IDC_DGN_CPG_LAMDA_V4_EDIT)->EnableWindow(true);		
	}
	else
	{
		GetDlgItem(IDC_DGN_CPG_STATIC)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_STATIC2)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_LAMDA_V2_EDIT)->EnableWindow(false);
		GetDlgItem(IDC_DGN_CPG_LAMDA_V4_EDIT)->EnableWindow(false);	
	}		
}



void CCPGDamageEquivalenceTabJDlg::InitialDlgData()
{
	//m_Data.Initialize();

	Data2Dlg();
}

BOOL CCPGDamageEquivalenceTabJDlg::Data2Dlg()
{
	m_bUse = m_Data.bUseData;

	m_dLamdaV2 = m_Data.dLamda_v2;
	m_dLamdaV4 = m_Data.dLamda_v4;	

	UpdateData(FALSE);

	SetAllControlEnable(m_pParent->m_bJTabEnable);

	return TRUE;
}

BOOL CCPGDamageEquivalenceTabJDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.bUseData = m_bUse;

	m_Data.dLamda_v2 = m_dLamdaV2;
	m_Data.dLamda_v4 = m_dLamdaV4;

	return TRUE;
}

void CCPGDamageEquivalenceTabJDlg::GetCgfrData()
{
	Dlg2Data();

	m_pParent->m_J_Data = m_Data;
}

void CCPGDamageEquivalenceTabJDlg::SetCgfrData()
{
	Data2Dlg();
}

void CCPGDamageEquivalenceTabJDlg::SetAllControlEnable(bool enable)
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
	}	
}


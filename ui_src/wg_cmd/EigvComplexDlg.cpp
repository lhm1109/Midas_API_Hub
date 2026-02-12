// ThisItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "EigvComplexDlg.h"
#include "ThisCalcDamping.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_dbLock\LockOption.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEigvComplexDlg dialog


CEigvComplexDlg::CEigvComplexDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CEigvComplexDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_nFreqPeriodRdoDlg = 0;
	m_dFrequencyDlg = 1.0;
	m_dPeriodDlg = 1.0;
	m_Data.Initialize();
	//{{AFX_DATA_INIT(CEigvComplexDlg)  

	m_nCoefDefineRdo = 0;
	m_nFreqPeriodRdo = 0;

	m_bCancel = FALSE;
	//}}AFX_DATA_INIT
}

CEigvComplexDlg::~CEigvComplexDlg()
{

}

void CEigvComplexDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEigvComplexDlg)  
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_SHOW_DAMPING_BTN, m_wndShowBtn);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_TYPE_MASS_CHK, m_wndMassProp);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_TYPE_STIFF_CHK, m_wndStiffProp); 
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_DIRECT_RM_EDIT, m_wndDirectRm);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_DIRECT_RK_EDIT, m_wndDirectRk);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_CALC_RM_EDIT, m_wndCalcRm);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_CALC_RK_EDIT, m_wndCalcRk);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_F1_EDIT, m_wndF1);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_F2_EDIT, m_wndF2);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_T1_EDIT, m_wndT1);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_T2_EDIT, m_wndT2);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_X1_EDIT, m_wndX1);
	DDX_Control(pDX, IDC_CMD_DAMPING_MASS_X2_EDIT, m_wndX2);
	DDX_Radio(pDX, IDC_CMD_DAMPING_MASS_DEFINE_RDO, m_nCoefDefineRdo);
	DDX_Radio(pDX, IDC_CMD_DAMPING_MASS_FREQ_PERIOD_RDO, m_nFreqPeriodRdo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEigvComplexDlg, CDialogMove)
	//{{AFX_MSG_MAP(CEigvComplexDlg)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MASS_DEFINE_RDO, OnChangeCoefDefine)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MASS_DEFINE_RDO2, OnChangeCoefDefine)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MASS_SHOW_DAMPING_BTN, OnShowDampingBtn)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MASS_TYPE_MASS_CHK, OnMassStiffChk)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MASS_TYPE_STIFF_CHK, OnMassStiffChk)
	ON_EN_CHANGE(IDC_CMD_DAMPING_MASS_F1_EDIT, OnCallAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_DAMPING_MASS_F2_EDIT, OnCallAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_DAMPING_MASS_T1_EDIT, OnCallAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_DAMPING_MASS_T2_EDIT, OnCallAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_DAMPING_MASS_X1_EDIT, OnCallAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_DAMPING_MASS_X2_EDIT, OnCallAutoCalcCoef)	
	ON_BN_CLICKED(IDC_CMD_DAMPING_MASS_FREQ_PERIOD_RDO, OnChangeFreqPeriod)
	ON_BN_CLICKED(IDC_CMD_DAMPING_MASS_FREQ_PERIOD_RDO2, OnChangeFreqPeriod)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////
// Interfaces
//////////////////////////////////////////////////////////////////////////////
void CEigvComplexDlg::SetDlgData(T_EIGV_COMP &data)
{ 
	m_Data = data; 
}

BOOL CEigvComplexDlg::GetDlgData(T_EIGV_COMP &data)
{
	if (m_bCancel)
		return FALSE;
	data.Initialize();
	data = m_Data;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
// Implementations
//////////////////////////////////////////////////////////////////////////////
BOOL CEigvComplexDlg::Dlg2Data()  
{
	// 
	UpdateData(TRUE);
	m_Data.Initialize();
	m_Data.bMassProp = m_wndMassProp.GetCheck();
	m_Data.bStiffProp = m_wndStiffProp.GetCheck();
	m_Data.nCoefDefine = m_nCoefDefineRdo + 1;
	if(m_nCoefDefineRdo == 0) // Direct Specification
	{
		m_Data.dRm = m_wndDirectRm.GetEditValue();
		m_Data.dRk = m_wndDirectRk.GetEditValue();
	}
	else if(m_nCoefDefineRdo == 1) // Calculate from Modal Damping
	{
		m_Data.dRm = m_wndCalcRm.GetEditValue();
		m_Data.dRk = m_wndCalcRk.GetEditValue();
	}
	m_Data.nFreqPeriod = m_nFreqPeriodRdo + 1;
	m_Data.dFreqOne = m_wndF1.GetEditValue();
	m_Data.dFreqTwo = m_wndF2.GetEditValue();
	m_Data.dPeriodOne = m_wndT1.GetEditValue();
	m_Data.dPeriodTwo = m_wndT2.GetEditValue();
	m_Data.dDampingOne = m_wndX1.GetEditValue();
	m_Data.dDampingTwo = m_wndX2.GetEditValue();

	return TRUE;
}

BOOL CEigvComplexDlg::Data2Dlg()
{
	CString csVal;
	
	m_wndMassProp.SetCheck(m_Data.bMassProp);
	m_wndStiffProp.SetCheck(m_Data.bStiffProp);
	m_nCoefDefineRdo = m_Data.nCoefDefine - 1;
	if(m_nCoefDefineRdo == 0) // Direct Specification
	{
		m_wndDirectRm.SetEditUnit(m_Data.dRm);
		m_wndDirectRk.SetEditUnit(m_Data.dRk);
		m_wndCalcRm.SetEditUnit(0.);
		m_wndCalcRk.SetEditUnit(0.);
	}
	else if(m_nCoefDefineRdo == 1) // Calculate from Modal Damping
	{
		m_wndDirectRm.SetEditUnit(0.);
		m_wndDirectRk.SetEditUnit(0.);
		m_wndCalcRm.SetEditUnit(m_Data.dRm);
		m_wndCalcRk.SetEditUnit(m_Data.dRk);
	}
	m_nFreqPeriodRdo = m_Data.nFreqPeriod - 1;
	m_wndF1.SetEditUnit(m_Data.dFreqOne);
	m_wndF2.SetEditUnit(m_Data.dFreqTwo);
	m_wndT1.SetEditUnit(m_Data.dPeriodOne);
	m_wndT2.SetEditUnit(m_Data.dPeriodTwo);
	m_wndX1.SetEditUnit(m_Data.dDampingOne);
	m_wndX2.SetEditUnit(m_Data.dDampingTwo);  

	UpdateData(FALSE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CEigvComplexDlg message handlers

BOOL CEigvComplexDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CDialogMove::OnInitDialog();

	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlAttribute, IDC_CMD_DAMPING_MASS_ATTRIBUTE_FRM, FALSE);

	Data2Dlg();  
	AutoCalcCoef();
	CtrlManager();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEigvComplexDlg::OnCallAutoCalcCoef()
{
	AutoCalcCoef();
}

void CEigvComplexDlg::OnChangeCoefDefine()
{
	CtrlManager();
	AutoCalcCoef();  // Mass and Stiffness Coefficient Calculation
}

void CEigvComplexDlg::OnChangeFreqPeriod()
{
	CtrlManager();
	AutoCalcCoef();  // Mass and Stiffness Coefficient Calculation
}

void CEigvComplexDlg::OnMassStiffChk()
{
	CtrlManager();
	AutoCalcCoef();  // Mass and Stiffness Coefficient Calculation
}

void CEigvComplexDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data()) return;

	CDialogMove::OnOK();
}

void CEigvComplexDlg::OnCancel()
{
	m_bCancel = TRUE;
	CDialogMove::OnCancel();
}

void CEigvComplexDlg::AutoCalcCoef()
{
	UpdateData(TRUE);
	T_THIS_PROPDAMPING rData;
	rData.Initialize();
	//rData.nDampingType = 2;  
	rData.nCoefDefine = m_nCoefDefineRdo + 1;
	rData.nFreqPeriod = m_nFreqPeriodRdo + 1;
	rData.bMassProp = m_wndMassProp.GetCheck();
	rData.bStiffProp = m_wndStiffProp.GetCheck();
	rData.dFreqOne = m_wndF1.GetEditValue();
	rData.dFreqTwo = m_wndF2.GetEditValue();
	rData.dPeriodOne = m_wndT1.GetEditValue();
	rData.dPeriodTwo = m_wndT2.GetEditValue();
	rData.dDampingOne = m_wndX1.GetEditValue();
	rData.dDampingTwo = m_wndX2.GetEditValue();
	
	if (rData.nCoefDefine != 2) return;

	if(m_pDoc->m_pEditData->AutoCalcThisCoef(rData, rData.dRm, rData.dRk))
	{
		m_wndCalcRm.SetEditUnit(rData.dRm);
		m_wndCalcRk.SetEditUnit(rData.dRk);
	}
	else
	{
		if(rData.bMassProp)  m_wndCalcRm.SetWindowText(_T("--"));
		if(rData.bStiffProp) m_wndCalcRk.SetWindowText(_T("--"));
	}
}

void CEigvComplexDlg::OnShowDampingBtn()
{
	UpdateData();
	
	CThisCalcDamping dlg;
	
	CRect rectShowBtn, rMain;  
	m_wndShowBtn.GetWindowRect(rectShowBtn);    
	GetWindowRect(rMain);
	int nDistY = rMain.bottom - rectShowBtn.top;
	dlg.SetInitPos(D_INIT_POS_RB, 0, -nDistY);
		
	double dRm = 0., dRk = 0.;
	switch(m_nCoefDefineRdo)
	{
		case 0 : 
			dRm = m_wndDirectRm.GetEditValue();
			dRk = m_wndDirectRk.GetEditValue();
			break;
		case 1:
		default:
			dRm = m_wndCalcRm.GetEditValue();
			dRk = m_wndCalcRk.GetEditValue();
			break;
	}

	dlg.m_bMass  = m_wndMassProp.GetCheck();
	dlg.m_bStiff = m_wndStiffProp.GetCheck();
	dlg.m_dRm	= dRm;
	dlg.m_dRk	= dRk;
	dlg.m_nFreqPeriodRdo = m_nFreqPeriodRdoDlg;
	dlg.m_dFrequency =  m_dFrequencyDlg;
	dlg.m_dPeriod = m_dPeriodDlg;  
	
	if(dlg.DoModal() == IDOK)
	{
		m_nFreqPeriodRdoDlg = dlg.m_nFreqPeriodRdo;
		m_dFrequencyDlg = dlg.m_dFrequency;
		m_dPeriodDlg = dlg.m_dPeriod;
	}
}

void CEigvComplexDlg::CtrlManager()
{
	UpdateData(TRUE);

	BOOL bMass = m_wndMassProp.GetCheck();
	BOOL bStiff = m_wndStiffProp.GetCheck();

	//GetDlgItem(IDC_CMD_DAMPING_MASS_TYPE_MASS_CHK)->EnableWindow(TRUE);
	//GetDlgItem(IDC_CMD_DAMPING_MASS_TYPE_STIFF_CHK)->EnableWindow(TRUE);
	GetDlgItem(IDC_CMD_DAMPING_MASS_ATTRIBUTE_FRM)->EnableWindow(bMass || bStiff);
	GetDlgItem(IDC_CMD_DAMPING_MASS_DEFINE_RDO)->EnableWindow(bMass || bStiff);
	GetDlgItem(IDC_CMD_DAMPING_MASS_DEFINE_RDO2)->EnableWindow(bMass || bStiff);
	GetDlgItem(IDC_CMD_DAMPING_MASS_DIRECT_RM_EDIT)->EnableWindow(m_nCoefDefineRdo == 0 && bMass);
	GetDlgItem(IDC_CMD_DAMPING_MASS_DIRECT_RK_EDIT)->EnableWindow(m_nCoefDefineRdo == 0 && bStiff);
	GetDlgItem(IDC_CMD_DAMPING_MASS_CALC_RM_EDIT)->EnableWindow( m_nCoefDefineRdo == 1 && bMass);
	GetDlgItem(IDC_CMD_DAMPING_MASS_CALC_RK_EDIT)->EnableWindow(m_nCoefDefineRdo == 1 && bStiff);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlAttribute, m_nCoefDefineRdo == 1 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_F1_EDIT)->EnableWindow(m_nCoefDefineRdo == 1 && m_nFreqPeriodRdo == 0 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_T1_EDIT)->EnableWindow(m_nCoefDefineRdo == 1 && m_nFreqPeriodRdo == 1 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_X1_EDIT)->EnableWindow(m_nCoefDefineRdo == 1 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_F2_EDIT)->EnableWindow(m_nCoefDefineRdo == 1 && m_nFreqPeriodRdo == 0 && (bMass && bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_T2_EDIT)->EnableWindow(m_nCoefDefineRdo == 1 && m_nFreqPeriodRdo == 1 && (bMass && bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_X2_EDIT)->EnableWindow(m_nCoefDefineRdo == 1 && (bMass && bStiff));
	GetDlgItem(IDC_CMD_DAMPING_MASS_SHOW_DAMPING_BTN)->EnableWindow(bMass || bStiff);

	
	UpdateData(FALSE);
}
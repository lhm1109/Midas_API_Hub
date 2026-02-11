// CMWindItemKBC2009.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureNodalDlg_CH2001.h"

#include "StldWindPressureNodalDlg.h"
#include "StldWindPressureAreaDlg_CH2001_PulsMag.h"

//#include "..\wg_cmd\CMWindItemDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

// #include "..\wg_base\wg_base_MsgDll.h"  
// 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureNodalDlg_CH2001 dialog

CStldWindPressureNodalDlg_CH2001::CStldWindPressureNodalDlg_CH2001(CWnd* pParent /*=NULL*/)
: CCMWindPressureBaseChildDlg(CStldWindPressureNodalDlg_CH2001::IDD, pParent)
{
	m_bInitChild = FALSE;
	m_pData = NULL;
}

void CStldWindPressureNodalDlg_CH2001::DoDataExchange(CDataExchange* pDX)
{
	CCMWindPressureBaseChildDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TM_MAGFACTOR_EDIT,    m_edtMagnifyFactor);
	DDX_Control(pDX, IDC_TM_FORCE_COEFF_EDIT,  m_edtCoeff);
	DDX_Control(pDX, IDC_TM_FORCE_COEFF_EDIT2, m_edtLeewardCoeff);
	
//   DDX_Control(pDX, IDC_TM_WIND_VIB_EDT,      m_edtWindPressure);
//   DDX_Control(pDX, IDC_TM_WIND_Damping_EDT, m_edtDampingRatio);
}


BEGIN_MESSAGE_MAP(CStldWindPressureNodalDlg_CH2001, CCMWindPressureBaseChildDlg)
	ON_BN_CLICKED(IDC_TM_PULS_MAG_BTN,      OnBtnMagFactorAutoCalc)
	//ON_BN_CLICKED(IDC_TM_WIND_CFT_CHECK,    OnBtnCheckVibration)
	ON_BN_CLICKED(IDC_TM_AUTO_CALC_RD,      OnRdoChanged)
	ON_BN_CLICKED(IDC_TM_USER_DEF_RD,       OnRdoChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureNodalDlg_CH2001 message handlers

BOOL CStldWindPressureNodalDlg_CH2001::OnInitDialog() 
{
	CCMWindPressureBaseChildDlg::OnInitDialog();

	InitControl();

	if(m_pData)
		Data2Dlg( (void*)m_pData);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CStldWindPressureNodalDlg_CH2001::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
//   if(m_pData)
//   {
//     Dlg2Data((void*)m_pData);
//   }
	
	return CCMWindPressureBaseChildDlg::DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CStldWindPressureNodalDlg_CH2001::InitDefaultData()
{
}

void CStldWindPressureNodalDlg_CH2001::Data2Dlg( void* pData, BOOL bWarning )
{
	m_pData = (T_WDPR_D*)pData;
	
	if(m_pData->CodeParam.CH2001.nPulseMagnifyMethod == 2)
	{
		((CButton*)GetDlgItem(IDC_TM_AUTO_CALC_RD))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_TM_USER_DEF_RD))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_TM_AUTO_CALC_RD))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_TM_USER_DEF_RD))->SetCheck(FALSE);
	}
	OnRdoChanged();

	m_edtMagnifyFactor.SetEditUnit(m_pData->CodeParam.CH2001.dPulseMagnifyFactor);
	m_edtCoeff.SetEditUnit(m_pData->CodeParam.CH2001.dWindwardCoef);
	m_edtLeewardCoeff.SetEditUnit(m_pData->CodeParam.CH2001.dLeewardCoef);
	
	//Confort.. 하지 않도록...
//   ((CButton*)GetDlgItem(IDC_TM_WIND_CFT_CHECK))->SetCheck(m_pData->CH2001.bComfort);
//   m_edtWindPressure.SetEditUnit(m_pData->CH2001.dCftWindPressure);
//   m_edtDampingRatio.SetEditUnit(m_pData->CH2001.dCftDampingRatio);
//   OnBtnCheckVibration();
}

BOOL CStldWindPressureNodalDlg_CH2001::Dlg2Data( void* pData, BOOL bWarning )
{
	m_pData = (T_WDPR_D*)pData;
	
	m_pData->CodeParam.CH2001.nPulseMagnifyMethod = GetType();
	m_pData->CodeParam.CH2001.dPulseMagnifyFactor = m_edtMagnifyFactor.GetEditValue();

	if(m_bInitChild)
	{
		m_pData->CodeParam.CH2001.dDampingRatio = m_dChildDampingRatio;
		m_pData->CodeParam.CH2001.dPeriod = m_dChildPeriod;
	}

	m_pData->CodeParam.CH2001.dWindwardCoef = m_edtCoeff.GetEditValue();
	m_pData->CodeParam.CH2001.dLeewardCoef = m_edtLeewardCoeff.GetEditValue();

//   if(((CButton*)GetDlgItem(IDC_TM_WIND_CFT_CHECK))->GetCheck())
//   {
//     m_pData->CH2001.bComfort = TRUE;
//   }
//   else
//   {
//     m_pData->CH2001.bComfort = FALSE;
//   }
//   m_pData->CH2001.dCftWindPressure = m_edtWindPressure.GetEditValue();
//   m_pData->CH2001.dCftDampingRatio = m_edtDampingRatio.GetEditValue();
	
	return TRUE;
}

void CStldWindPressureNodalDlg_CH2001::OnBtnMagFactorAutoCalc()
{
	m_pParent->Dlg2Data(FALSE);

	T_WVEP_K WvepK;
	if(!m_pParent->GetWvepKey(WvepK))
	{
		AfxMessageBox(_LS(IDS_TM_WINDP_NO_SELECTED_WVEP));
		return;
	}

	CStldWindPressureAreaDlg_CH2001_PulsMag dlg;
	dlg.SetWdpr(m_pData);
	if(dlg.DoModal() == IDOK)
	{
		m_bInitChild = TRUE;
		m_dChildDampingRatio = dlg.GetDampingRatio();
		m_dChildPeriod = dlg.GetPeriod();
		m_edtMagnifyFactor.SetEditUnit(dlg.GetPulsMagnifyFactor());
	}
}

void CStldWindPressureNodalDlg_CH2001::OnRdoChanged()
{
	int nType = GetType();
	if(nType == 1)
	{
		GetDlgItem(IDC_TM_PULS_MAG_BTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_MAGFACTOR_EDIT)->EnableWindow(FALSE);
	}
	else if(nType == 2)
	{
		GetDlgItem(IDC_TM_PULS_MAG_BTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_MAGFACTOR_EDIT)->EnableWindow(TRUE);
	}
}

int CStldWindPressureNodalDlg_CH2001::GetType()
{
	int nType = 1;
	if(((CButton*)GetDlgItem(IDC_TM_AUTO_CALC_RD))->GetCheck())
		nType = 1;
	else if(((CButton*)GetDlgItem(IDC_TM_USER_DEF_RD))->GetCheck())
		nType = 2;
	else 
	{
		nType = 1;
		((CButton*)GetDlgItem(IDC_TM_AUTO_CALC_RD))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_TM_USER_DEF_RD))->SetCheck(FALSE);
	}
	return nType;
}

void CStldWindPressureNodalDlg_CH2001::InitControl()
{
	m_edtMagnifyFactor.SetUnitType(D_UNITSYS_NONE);
	m_edtCoeff.SetUnitType(D_UNITSYS_NONE);
	m_edtLeewardCoeff.SetUnitType(D_UNITSYS_NONE);
//   m_edtWindPressure.SetUnitType(D_UNITSYS_NONE);
//   m_edtDampingRatio.SetUnitType(D_UNITSYS_NONE);
}

// void CStldWindPressureNodalDlg_CH2001::OnBtnCheckVibration()
// {
//   int nCheck = ((CButton*)GetDlgItem(IDC_TM_WIND_CFT_CHECK))->GetCheck();
//   BOOL bEnable;
//   
//   if(nCheck) bEnable = TRUE;
//   else       bEnable = FALSE;
//   
//   m_edtWindPressure.EnableWindow(bEnable);
//   m_edtDampingRatio.EnableWindow(bEnable);
// }

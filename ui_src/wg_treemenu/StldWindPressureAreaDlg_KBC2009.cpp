// CMWindItemKBC2009.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureAreaDlg_KBC2009.h"

#include "StldWindPressureAreaDlg.h"
#include "StldWindPressureAreaDlg_KBC2009_GustF.h"
#include "StldWindPressureAreaDlg_KBC2009_FrcCoefAuto.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_KBC2009 dialog

CStldWindPressureAreaDlg_KBC2009::CStldWindPressureAreaDlg_KBC2009(CWnd* pParent /*=NULL*/)
: CCMWindPressureBaseChildDlg(CStldWindPressureAreaDlg_KBC2009::IDD, pParent)
{
	m_pData = NULL;
	m_bInAutoCalc = FALSE;
}

void CStldWindPressureAreaDlg_KBC2009::DoDataExchange(CDataExchange* pDX)
{
	CCMWindPressureBaseChildDlg::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TM_GUST_FACTOR_EDIT_EX,     m_editGustFactor_Ex);
	DDX_Control(pDX, IDC_TM_GUST_FACTOR_EDIT_IN,     m_editGustFactor_In);
	DDX_Control(pDX, IDC_TM_FORCE_COEFF_EDIT_EX,     m_editCoefFactor_Ex);
	DDX_Control(pDX, IDC_TM_FORCE_COEFF_EDIT_IN,     m_editCoefFactor_In);
}


BEGIN_MESSAGE_MAP(CStldWindPressureAreaDlg_KBC2009, CCMWindPressureBaseChildDlg)
	ON_BN_CLICKED(IDC_TM_GUST_BTN,              OnCmdGustFactor)
	ON_BN_CLICKED(IDC_TM_FORCE_COEF_AUTO_BTN,   OnCmdCoefFactor)
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_FORCECOEF, OnChkAutoCalcForceCoef)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_KBC2009 message handlers

BOOL CStldWindPressureAreaDlg_KBC2009::OnInitDialog() 
{
	CCMWindPressureBaseChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_editGustFactor_Ex.SetUnitType(D_UNITSYS_NONE);	
	m_editGustFactor_In.SetUnitType(D_UNITSYS_NONE);	
	m_editCoefFactor_Ex.SetUnitType(D_UNITSYS_NONE);	
	m_editCoefFactor_In.SetUnitType(D_UNITSYS_NONE);	
	
	if(m_pData)
		Data2Dlg( (void*) m_pData);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CStldWindPressureAreaDlg_KBC2009::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//Dlg2Data((void*)m_pData);
	
	return CCMWindPressureBaseChildDlg::DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CStldWindPressureAreaDlg_KBC2009::InitDefaultData()
{
}

BOOL CStldWindPressureAreaDlg_KBC2009::Dlg2Data( void* pData, BOOL bWarning )
{
	m_pData = (T_WDPR_D*)pData;

	m_pData->CodeParam.KBC2009.dGfExternal = m_editGustFactor_Ex.GetEditValue();
	m_pData->CodeParam.KBC2009.dGfInternal = m_editGustFactor_In.GetEditValue();
	m_pData->CodeParam.KBC2009.dCfExternal = m_editCoefFactor_Ex.GetEditValue();
	m_pData->CodeParam.KBC2009.dCfInternal = m_editCoefFactor_In.GetEditValue();
	
	CButton* pBtn = (CButton*)GetDlgItem(IDC_WINDP_AUTOCALC_FORCECOEF);
	if(pBtn->GetCheck()) m_pData->CodeParam.KBC2009.bAutoForceCoef = TRUE;
	else                 m_pData->CodeParam.KBC2009.bAutoForceCoef = FALSE;

	if(m_pData->CodeParam.KBC2009.bAutoForceCoef)
	{
	  if(m_pData->arKBC2009ForceCoef.GetSize() <= 1 && m_bInAutoCalc == FALSE)
	  {
		  if(bWarning) AfxMessageBox(_LS(IDS_TM_WINDP_KBC2009_NOAUTOCALCINFO));
		  return FALSE;
	  }
	}

	return TRUE;
}

void CStldWindPressureAreaDlg_KBC2009::Data2Dlg( void* pData, BOOL bWarning )
{
	m_pData = (T_WDPR_D*)pData;
	
	m_editGustFactor_Ex.SetEditUnit(m_pData->CodeParam.KBC2009.dGfExternal, 4);
	m_editGustFactor_In.SetEditUnit(m_pData->CodeParam.KBC2009.dGfInternal, 4);
	m_editCoefFactor_Ex.SetEditUnit(m_pData->CodeParam.KBC2009.dCfExternal, 4);
	m_editCoefFactor_In.SetEditUnit(m_pData->CodeParam.KBC2009.dCfInternal, 4);
	
	CButton* pBtn = (CButton*)GetDlgItem(IDC_WINDP_AUTOCALC_FORCECOEF);
	pBtn->SetCheck(m_pData->CodeParam.KBC2009.bAutoForceCoef);

	OnChkAutoCalcForceCoef();
}

void CStldWindPressureAreaDlg_KBC2009::OnCmdGustFactor()
{
	m_bInAutoCalc = TRUE;
	if(!m_pParent->Dlg2Data())
	{
		m_bInAutoCalc = FALSE;
		return;
	}
	m_bInAutoCalc = FALSE;
	
	T_WVEP_K WvepK;
	if(!m_pParent->GetWvepKey(WvepK))
	{
		AfxMessageBox(_LS(IDS_TM_WINDP_NO_SELECTED_WVEP));
		return;
	}
	
	T_WVEP_D WvepD;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(!pDoc->m_pAttrCtrl2->GetWvep(WvepK, WvepD))
	{
		ASSERT(0);
		WvepD.Initialize();
	}
	
	T_WVEP_KBC2009& KBC2009 = WvepD.CodeParam.KBC2009;
	
	CStldWindPressureAreaDlg_KBC2009_GustF dlg;
	dlg.SetWindData(KBC2009, m_pData);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetDlgInfo(m_pData->CodeParam.KBC2009.nRigidity, m_pData->CodeParam.KBC2009.dBreath, m_pData->CodeParam.KBC2009.dNaturalFreq,
			m_pData->CodeParam.KBC2009.dDampingRatio, m_pData->CodeParam.KBC2009.dGfExternal);
		
		Data2Dlg( (void*) m_pData);
	}
}

void CStldWindPressureAreaDlg_KBC2009::OnCmdCoefFactor()
{
	m_bInAutoCalc = TRUE;
	if(!m_pParent->Dlg2Data())
	{
		m_bInAutoCalc = FALSE;
		return;
	}
	m_bInAutoCalc = FALSE;

	T_WVEP_K WvepK;
	if(!m_pParent->GetWvepKey(WvepK))
	{
		AfxMessageBox(_LS(IDS_TM_WINDP_NO_SELECTED_WVEP));
		return;
	}

	CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto dlg;
	dlg.SetData(m_pData);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetData(m_pData);
	}
}

void CStldWindPressureAreaDlg_KBC2009::OnChkAutoCalcForceCoef()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_WINDP_AUTOCALC_FORCECOEF);
	if(pBtn->GetCheck())
	{
		GetDlgItem(IDC_TM_FORCE_COEF_AUTO_BTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_FORCE_COEFF_EDIT_EX)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_FORCE_COEFF_EDIT_IN)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TM_FORCE_COEF_AUTO_BTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_FORCE_COEFF_EDIT_EX)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_FORCE_COEFF_EDIT_IN)->EnableWindow(TRUE);
	}
}

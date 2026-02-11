// CStldWindPressureAreaDlg_IS875_2015.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureNodalDlg_IS875_2015.h"

#include "StldWindPressureNodalDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewBuff.h"
//#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"

//#include "..\wg_base\wg_base_MsgDll.h"
//#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureNodalDlg_IS875_2015 dialog

CStldWindPressureNodalDlg_IS875_2015::CStldWindPressureNodalDlg_IS875_2015(CWnd* pParent /*=NULL*/)
	: CCMWindPressureBaseChildDlg(CStldWindPressureNodalDlg_IS875_2015::IDD, pParent)
{
	m_pData = NULL;
	//m_nCodeType = nCodeType;
}

void CStldWindPressureNodalDlg_IS875_2015::DoDataExchange(CDataExchange* pDX)
{
	CCMWindPressureBaseChildDlg::DoDataExchange(pDX);

	// DataEdit controls
	DDX_Control(pDX, IDC_TM_DIRECT_FACTOR_EDIT, m_editDirectFactor);
	DDX_Control(pDX, IDC_TM_COMB_FACTOR_EDIT, m_editCombFactor);
	DDX_Control(pDX, IDC_TM_PRESS_COEF_EDIT, m_editCpe);
	DDX_Control(pDX, IDC_TM_INT_COEF_EDIT, m_editCpi);
	DDX_Control(pDX, IDC_TM_FORCE_COEF_EDIT, m_editForceCoef);

	// Radio buttons for mode
	DDX_Control(pDX, IDC_TM_PRESS_COEF_RADIO, m_radioPressCoef);
	DDX_Control(pDX, IDC_TM_FORCE_COEF_RADIO, m_radioForceCoef);
}
BEGIN_MESSAGE_MAP(CStldWindPressureNodalDlg_IS875_2015, CCMWindPressureBaseChildDlg)
	ON_BN_CLICKED(IDC_TM_PRESS_COEF_RADIO, OnCoefModeRadioClicked)
	ON_BN_CLICKED(IDC_TM_FORCE_COEF_RADIO, OnCoefModeRadioClicked)
END_MESSAGE_MAP()

BOOL CStldWindPressureNodalDlg_IS875_2015::OnInitDialog()
{
	CCMWindPressureBaseChildDlg::OnInitDialog();
	m_editDirectFactor.SetUnitType(D_UNITSYS_NONE);
	m_editCombFactor.SetUnitType(D_UNITSYS_NONE);
	m_editCpe.SetUnitType(D_UNITSYS_NONE);
	m_editCpi.SetUnitType(D_UNITSYS_NONE);
	m_editForceCoef.SetUnitType(D_UNITSYS_NONE);
	m_radioPressCoef.SetCheck(TRUE);
	m_radioForceCoef.SetCheck(FALSE);

	ShowCoefFields(0); // Show Cpe/Cpi 

	if (m_pData)
	{
		m_pData->nCodeType = m_nCodeType;
		Data2Dlg((void*)m_pData);
	}
	return TRUE;
}

BOOL CStldWindPressureNodalDlg_IS875_2015::DestroyWindow()
{
	return CCMWindPressureBaseChildDlg::DestroyWindow();
}


void CStldWindPressureNodalDlg_IS875_2015::ShowCoefFields(int mode)
{
	if (mode == 0) // Pressure Coefficient selected
	{
		// Show and enable Cpe and Cpi
		GetDlgItem(IDC_TM_PRESS_COEF_EDIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_WG_TREEMENU_STATIC3)->ShowWindow(SW_SHOW); // "Cpe :"
		GetDlgItem(IDC_TM_PRESS_COEF_EDIT)->EnableWindow(TRUE);

		GetDlgItem(IDC_TM_INT_COEF_EDIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TM_INT_COEF_EDIT)->EnableWindow(TRUE);

		// Hide Cf controls
		GetDlgItem(IDC_TM_FORCE_COEF_EDIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_WG_TREEMENU_STATIC4)->ShowWindow(SW_HIDE); // "Cf :"
		GetDlgItem(IDC_TM_FORCE_COEF_EDIT)->EnableWindow(FALSE);
	}
	// Force coefficients mode
	else // Force Coefficient selected
	{
		// Show Cf (enabled) and Cpi (disabled)
		GetDlgItem(IDC_TM_PRESS_COEF_EDIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_WG_TREEMENU_STATIC3)->ShowWindow(SW_HIDE); // "Cpe :"
		GetDlgItem(IDC_TM_PRESS_COEF_EDIT)->EnableWindow(FALSE);

		GetDlgItem(IDC_TM_FORCE_COEF_EDIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_WG_TREEMENU_STATIC4)->ShowWindow(SW_SHOW); // "Cf :"
		GetDlgItem(IDC_TM_FORCE_COEF_EDIT)->EnableWindow(TRUE);

		GetDlgItem(IDC_TM_INT_COEF_EDIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TM_INT_COEF_EDIT)->EnableWindow(FALSE); // disable Cpi in force coeff mode
	}
}

void CStldWindPressureNodalDlg_IS875_2015::OnCoefModeRadioClicked()
{
	int mode = m_radioPressCoef.GetCheck() ? 0 : 1;
	ShowCoefFields(mode);
}
BOOL CStldWindPressureNodalDlg_IS875_2015::Dlg2Data(void* pData, BOOL bWarning)
{
	m_pData = (T_WDPR_D*)pData;
	T_WDPR_IS875_2015* pISCode = m_pData->GetIS875_2015();
	if (pISCode == NULL) { ASSERT(0); return FALSE; }
	pISCode->dkdfact = m_editDirectFactor.GetEditValue();
	pISCode->dkcfact = m_editCombFactor.GetEditValue();
	pISCode->nPressureCoefMode = m_radioPressCoef.GetCheck() ? 0 : 1;

	if (pISCode->nPressureCoefMode == 0) {
		pISCode->dCpe = m_editCpe.GetEditValue();
		pISCode->dCpi = m_editCpi.GetEditValue();
	}
	else {
		pISCode->dForceCoef = m_editForceCoef.GetEditValue();
	}

	return TRUE;
}
void CStldWindPressureNodalDlg_IS875_2015::Data2Dlg(void* pData, BOOL bWarning)
{
	m_pData = (T_WDPR_D*)pData;
	T_WDPR_IS875_2015* pISCode = &(m_pData->CodeParam.IS875_2015);
	if (pISCode == NULL) { ASSERT(0); return; }

	T_WVEP_K WvepK;
	if (m_pParent->GetWvepKey(WvepK))
	{
		T_WVEP_D WvepD;
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		if (!pDoc->m_pAttrCtrl2->GetWvep(WvepK, WvepD))
		{
			ASSERT(0);
			WvepD.Initialize();
		}

		if (WvepD.GetIS875_2015() == NULL) { ASSERT(0); return; }
	}

	m_editDirectFactor.SetEditUnit(pISCode->dkdfact, 4);
	m_editCombFactor.SetEditUnit(pISCode->dkcfact, 4);

	if (pISCode->nPressureCoefMode == 0) {
		m_radioPressCoef.SetCheck(TRUE);
		m_radioForceCoef.SetCheck(FALSE);
		ShowCoefFields(0);
		m_editCpe.SetEditUnit(pISCode->dCpe, 4);
		m_editCpi.SetEditUnit(pISCode->dCpi, 4);
	}
	else {
		m_radioPressCoef.SetCheck(FALSE);
		m_radioForceCoef.SetCheck(TRUE);
		ShowCoefFields(1);
		m_editForceCoef.SetEditUnit(pISCode->dForceCoef, 4);
		m_editCpi.SetEditUnit(pISCode->dCpi, 4);
	}
}


void CStldWindPressureNodalDlg_IS875_2015::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;
	switch (lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}
void CStldWindPressureNodalDlg_IS875_2015::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_WVEP_MFD):
		case(UR_WVEP_LT_ADD):
		case(UR_WVEP_LT_DEL):
		{
			if (m_pData) Data2Dlg((void*)m_pData);
		}
		break;
		default:
			break;
		}
	} // end of while
}
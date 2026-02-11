// CMWindItemKBC2009.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemDlg.h"
#include "CMWindPressureVelo_KBC2009.h"
#include "WindGustKBC2008Dlg.h"
#include "WindAutoCalcDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureVelo_KBC2009 dialog


CCMWindPressureVelo_KBC2009::CCMWindPressureVelo_KBC2009(CWnd* pParent /*=NULL*/)
: CChildDialog(CCMWindPressureVelo_KBC2009::IDD, pParent)
{
	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindPressureVelocityAddMod*)pParent;
}

void CCMWindPressureVelo_KBC2009::SetData2Dlg(T_WVEP_KBC2009& data)
{
	m_Data = data;
	Data2Dlg();
}

BOOL CCMWindPressureVelo_KBC2009::SetDlg2Data(T_WVEP_KBC2009& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CCMWindPressureVelo_KBC2009::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_TOPO_EFFECT_CHECK, m_chkTopoEffect);
	DDX_Control(pDX, IDC_CMD_VKZT_UNIT,         m_unitVKZT);
	DDX_Control(pDX, IDC_CMD_VKZT_EDIT,         m_editVKZT);
// 	DDX_Control(pDX, IDC_CMD_SPEED_UNIT,        m_unitSpeed);
	DDX_Control(pDX, IDC_CMD_SPEED_EDIT,        m_editSpeed);
	DDX_Control(pDX, IDC_CMD_ROOF_HEIGHT_UNIT,  m_unitRoofHeight);
	DDX_Control(pDX, IDC_CMD_ROOF_HEIGHT_EDIT,  m_editRoofHeight);
	DDX_Control(pDX, IDC_CMD_KZT_EDIT,          m_editKZT);
	DDX_Control(pDX, IDC_CMD_CATEGORY_COMBO,    m_cobxCategory);
	DDX_Control(pDX, IDC_CMD_IMPORTANCE_COMBO,  m_cobxImportFactor);
}


BEGIN_MESSAGE_MAP(CCMWindPressureVelo_KBC2009, CChildDialog)
	ON_BN_CLICKED(IDC_CMD_TOPO_EFFECT_CHECK, OnCmdTopoEffectCheck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureVelo_KBC2009 message handlers

BOOL CCMWindPressureVelo_KBC2009::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
// 	m_unitSpeed.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dBasicWindSpeed);
	m_editSpeed.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dBasicWindSpeed);
	
	m_unitRoofHeight.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dRoofHeight);
	m_editRoofHeight.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dRoofHeight);

	m_editKZT.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dKzt);
	m_unitVKZT.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dVzt);
	m_editVKZT.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dVzt);
	
	InitCategoryCombo();
	InitImportFactorCombo();
	OnCmdTopoEffectCheck();
	
	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CCMWindPressureVelo_KBC2009::DestroyWindow() 
{
	return CChildDialog::DestroyWindow();
}

void CCMWindPressureVelo_KBC2009::OnCmdTopoEffectCheck() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkTopoEffect.GetCheck();
	m_editKZT.EnableWindow(nCheck == 1);
	m_editVKZT.EnableWindow(nCheck == 1);
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMWindPressureVelo_KBC2009::InitCategoryCombo()
{
	m_cobxCategory.ResetContent();
	
	int nIndex;
	CString aItem[] = {_T("A"), _T("B"), _T("C"), _T("D")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
	{
		nIndex = m_cobxCategory.AddString(aItem[i]);
		m_cobxCategory.SetItemData(nIndex , i+1);
	}
	m_cobxCategory.SetCurSel(0);
}

void CCMWindPressureVelo_KBC2009::InitImportFactorCombo()
{
	m_cobxImportFactor.ResetContent();
	
	CString aItem[] = {_T("0.9"), _T("0.95"), _T("1.0"), _T("1.1")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxImportFactor.AddString(aItem[i]);
	m_cobxImportFactor.SetCurSel(0); 
}

void CCMWindPressureVelo_KBC2009::Data2Dlg()
{
	CString csVal;
	
	m_cobxCategory.SetCurSel(m_Data.nExposureCategory - 1);
	m_editSpeed.SetEditUnit(m_Data.dBasicWindSpeed);
	
	if (m_Data.dImportanceFactor == 0.9  || m_Data.dImportanceFactor == 0.95 ||
		m_Data.dImportanceFactor == 1.0  || m_Data.dImportanceFactor == 1.1)
	{
		csVal.Format(_T("%g"), m_Data.dImportanceFactor);
		m_cobxImportFactor.SelectString(-1, csVal);
	}
	else
	{
		csVal.Format(_T("%g"), m_Data.dImportanceFactor);
		m_cobxImportFactor.SetWindowText(csVal);
	}
	
	m_editRoofHeight.SetEditUnit(m_Data.dRoofHeight);
	m_chkTopoEffect.SetCheck(m_Data.bTopographicEffect);
	OnCmdTopoEffectCheck();
	m_editKZT.SetEditUnit(m_Data.dKzt);
	m_editVKZT.SetEditUnit(m_Data.dVzt);
}

BOOL CCMWindPressureVelo_KBC2009::Dlg2Data()
{
	CString csVal;

	m_Data.nExposureCategory = m_cobxCategory.GetCurSel() + 1;
	m_Data.dBasicWindSpeed = m_editSpeed.GetEditValue();	
	m_cobxImportFactor.GetWindowText(csVal);
	m_Data.dImportanceFactor = _tstof(csVal);	
	m_Data.dRoofHeight = m_editRoofHeight.GetEditValue();
	m_Data.bTopographicEffect = m_chkTopoEffect.GetCheck();
	if (m_Data.bTopographicEffect)
	{
		m_Data.dKzt = m_editKZT.GetEditValue();
		m_Data.dVzt = m_editVKZT.GetEditValue();
	}
	
	return TRUE;
}

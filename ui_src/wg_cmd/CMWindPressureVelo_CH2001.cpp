// CMWindItemKBC2009.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemDlg.h"
#include "CMWindPressureVelo_CH2001.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureVelo_CH2001 dialog


CCMWindPressureVelo_CH2001::CCMWindPressureVelo_CH2001(CWnd* pParent /*=NULL*/)
: CChildDialog(CCMWindPressureVelo_CH2001::IDD, pParent)
{
	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindPressureVelocityAddMod*)pParent;
}

void CCMWindPressureVelo_CH2001::SetData2Dlg(T_WVEP_CH2001& data)
{
	m_Data = data;
	Data2Dlg();
}

BOOL CCMWindPressureVelo_CH2001::SetDlg2Data(T_WVEP_CH2001& data)
{
	BOOL bRetVal = Dlg2Data();
	data = m_Data;
	return bRetVal;
}

void CCMWindPressureVelo_CH2001::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_CATEGORY_COMBO, m_cobxCategory);
	DDX_Control(pDX, IDC_CMD_SPEED_EDIT,     m_editWindPressure);
	DDX_Control(pDX, IDC_CMD_TOPOCOEF_EDIT,  m_editTopoCoef);
}


BEGIN_MESSAGE_MAP(CCMWindPressureVelo_CH2001, CChildDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureVelo_CH2001 message handlers

BOOL CCMWindPressureVelo_CH2001::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_editWindPressure.SetUnitType(CUnitCtrl::m_WVEP_UNIT.CH2001_dBasicWindPressure);	
	m_editTopoCoef.SetUnitType(CUnitCtrl::m_WVEP_UNIT.CH2001_dTopographyCoef);
	
	//m_editTopoCoef.SetUnitType(CUnitCtrl::m_WIND_UNIT.D_UNITSYS_NONE);
	
	InitCategoryCombo();
	
	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CCMWindPressureVelo_CH2001::DestroyWindow() 
{
	return CChildDialog::DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMWindPressureVelo_CH2001::InitCategoryCombo()
{
	m_cobxCategory.ResetContent();
	
	int nIndex;
	CStringArray strItem;
	strItem.Add(_T("A"));
	strItem.Add(_T("B"));
	strItem.Add(_T("C"));
	strItem.Add(_T("D"));
	int nNum = strItem.GetSize();
	for (int i = 0; i < nNum; i++)
	{
		nIndex = m_cobxCategory.AddString(strItem[i]);
		m_cobxCategory.SetItemData(nIndex , i + 1);
	}
	m_cobxCategory.SetCurSel(0);
}

void CCMWindPressureVelo_CH2001::Data2Dlg()
{
	m_cobxCategory.SetCurSel(m_Data.nRoughCategory-1);
	m_editWindPressure.SetEditUnit(m_Data.dBasicWindPressure);
	m_editTopoCoef.SetEditUnit(m_Data.dTopographyCoef);
}

BOOL CCMWindPressureVelo_CH2001::Dlg2Data()
{
	m_Data.nRoughCategory = m_cobxCategory.GetCurSel() + 1;
	m_Data.dBasicWindPressure = m_editWindPressure.GetEditValue();
	m_Data.dTopographyCoef = m_editTopoCoef.GetEditValue();
	return TRUE;
}

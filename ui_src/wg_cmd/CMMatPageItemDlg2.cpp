// CMMatPageItemDlg2.cpp : implementation file
// Villa용으로 구현된 Code : Type=Steel, Code=KS(S)만 지원한다.

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMatPageItemDlg2.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
//
// Old Version 
// See CMMatPageItemDlg.cpp
//
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_EditData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CCM_TYPE_STEEL      _T("S")
#define CCM_STEEL           _LS(IDS_WG_CMD__ADDD__Steel)
#define CCM_TYPE_STEEL_I    0
#define CCM_STEEL_CODE_KS _T("KS(S)")

/////////////////////////////////////////////////////////////////////////////
// CCMMatPageItemDlg2 dialog


CCMMatPageItemDlg2::CCMMatPageItemDlg2(CDBDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(CCMMatPageItemDlg2::IDD, pParent)
{
	m_pDoc = pDoc;
	m_bModify = FALSE;
	//{{AFX_DATA_INIT(CCMMatPageItemDlg2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMMatPageItemDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMatPageItemDlg2)
	DDX_Control(pDX, IDC_CMD_MP_ITEM_UNIT_THER, m_wndUnitT);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_UNIT_POI, m_wndUnitP);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_UNIT_DEN, m_wndUnitD);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_UNIT_ELAST, m_wndUnitE);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_THER, m_wndSteelT);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_POI, m_wndSteelP);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_DEN, m_wndSteelD);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_ELAST, m_wndSteelE);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_NAME_CBO, m_wndSteelName);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_CODE, m_wndSteelCode);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_NAME, m_wndName);
	DDX_Control(pDX, IDC_CMD_MP_ITEM_ID, m_wndID);
	//}}AFX_DATA_MAP
	m_aCtrlSteelAnal.Add(IDC_CMD_MP_ITEM_ELAST);
	m_aCtrlSteelAnal.Add(IDC_CMD_MP_ITEM_POI);
	m_aCtrlSteelAnal.Add(IDC_CMD_MP_ITEM_THER);
	m_aCtrlSteelAnal.Add(IDC_CMD_MP_ITEM_DEN);
}

/////////////////////////////////////////////////////////////////////////////
// Interface
/////////////////////////////////////////////////////////////////////////////
void CCMMatPageItemDlg2::SetModifyData(T_MATL_K key, T_MATL_D& data)
{
	m_OldKey = key;
	m_Key = key;
	m_Data = data;
	m_bModify = TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// Implementation
/////////////////////////////////////////////////////////////////////////////
void CCMMatPageItemDlg2::InitDlgCtrls()
{
	// change contents of name combo
	m_wndSteelCode.ResetContent();
	m_wndSteelCode.AddString(m_Data.Data1.CodeName);
	m_wndSteelCode.SetCurSel(0);
	SetNameCombo(&m_wndSteelName, m_Data.Type, m_Data.Data1.CodeName);
	SetUnitType();
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlSteelAnal, FALSE);
}

void CCMMatPageItemDlg2::SetUnitType()
{
	m_wndSteelE.SetUnitType(CUnitCtrl::m_MATL_UNIT.Elast);
	m_wndSteelD.SetUnitType(CUnitCtrl::m_MATL_UNIT.Density);
	m_wndSteelP.SetUnitType(CUnitCtrl::m_MATL_UNIT.Poisson);
	m_wndSteelT.SetUnitType(CUnitCtrl::m_MATL_UNIT.Thermal);

	m_wndUnitE.SetUnitType(CUnitCtrl::m_MATL_UNIT.Elast);
	m_wndUnitD.SetUnitType(CUnitCtrl::m_MATL_UNIT.Density);
	m_wndUnitP.SetUnitType(CUnitCtrl::m_MATL_UNIT.Poisson);
	m_wndUnitT.SetUnitType(CUnitCtrl::m_MATL_UNIT.Thermal);
}

void CCMMatPageItemDlg2::ShowDataToDlg()
{
	CString csValue;

	// ID
	csValue.Format(_T("%d"), m_Key);
	m_wndID.SetWindowText(csValue);

	// Name
	m_wndName.SetWindowText(m_Data.Name);

	// Matl Code Name
	CString CodeMatlName;
	CodeMatlName = m_Data.Data1.CodeMatlName;
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(m_Data.Data1.CodeName, CodeMatlName);
#endif
	m_wndSteelName.SelectString(-1, CodeMatlName);

	// Analsis data
	SetAnalData(CCM_TYPE_STEEL_I, m_Data.Data1.Analysis);
}

void CCMMatPageItemDlg2::SetNameCombo(CComboBox* pCombo, CString &csType, CString &csCode)
{
	// reset contents of ComboBox
	pCombo->ResetContent();

	// get material name list by material type
	// first get material type
	CString csMatlType = csType;

	CArray <CString, CString&> aNameList;
	// next get material name list
	if (csMatlType == CCM_TYPE_STEEL)  // steel
		m_pDoc->m_pMatlDB->GetSteelNameList(csCode, aNameList);
	else return;

	int nSize = aNameList.GetSize();
	for (int i = 0; i < nSize; i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(csCode, aNameList[i]);
#endif
		pCombo->AddString(aNameList[i]);
	}
}

void CCMMatPageItemDlg2::GetSteelData(CString csCode, CString csName, T_MATL_ANALYSIS &AnalData)
{
	T_MATL_STEEL SteelData;
	if (m_pDoc->m_pMatlDB->GetSteelData(csCode, csName, SteelData))
	{
		AnalData.Elast = SteelData.Elast;
		AnalData.Poisson = SteelData.Poisson;
		AnalData.Thermal = SteelData.Thermal;
		AnalData.Density = SteelData.Density;
		AnalData.MassDensity = SteelData.MassDensity;
	}
	else AnalData.Initialize();
}

void CCMMatPageItemDlg2::SetAnalData(int nTypeIndex, T_MATL_ANALYSIS &AnalData)
{
	CArray<UINT, UINT> *paCtrlID;
	if (nTypeIndex == CCM_TYPE_STEEL_I)
		paCtrlID = &m_aCtrlSteelAnal;
	else ASSERT(0);

	CString csValue;
	CString aFormat[] = {_T("%.4e"), _T("%g"), _T("%.4e"), _T("%.4g")};
	CEdit* pEdit;
	double* pValue[] = {
		&AnalData.Elast, &AnalData.Poisson, &AnalData.Thermal, &AnalData.Density,
	};

	int nSize = paCtrlID->GetSize();
	for (int i = 0; i < nSize; i++)
	{
		pEdit = (CEdit*)GetDlgItem(paCtrlID->GetAt(i));
		ASSERT(pEdit && pEdit->GetSafeHwnd() && IsWindow(pEdit->GetSafeHwnd()));
		csValue.Format(aFormat[i], *pValue[i]);
		pEdit->SetWindowText(csValue);
	}
}

BOOL CCMMatPageItemDlg2::GetAnalData(int nTypeIndex, T_MATL_ANALYSIS &AnalData)
{
	CArray<UINT, UINT> *paCtrlID;
	if (nTypeIndex == CCM_TYPE_STEEL_I)
		paCtrlID = &m_aCtrlSteelAnal;
	else ASSERT(0);

	CEditUnit* pEdit;
	double* pValue[] = {
		&AnalData.Elast, &AnalData.Poisson, &AnalData.Thermal, &AnalData.Density,
	};

	int nSize = paCtrlID->GetSize();
	for (int i = 0; i < nSize; i++)
	{
		pEdit = (CEditUnit*)GetDlgItem(paCtrlID->GetAt(i));
		ASSERT(pEdit && pEdit->GetSafeHwnd() && IsWindow(pEdit->GetSafeHwnd()));
		*pValue[i] = pEdit->GetEditValue();
	}
	return TRUE;
}

BOOL CCMMatPageItemDlg2::Dlg2Data()
{
	int nID;
	if (!m_wndID.GetEditValue(nID))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_material_number_));
		return FALSE;
	}
	m_Key = (T_MATL_K)nID;
	CString csName;
	m_wndName.GetWindowText(csName);
	csName.TrimLeft(); csName.TrimRight();
	if (csName == _T("")) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Missing_material_name_));
		return FALSE;
	}
	m_Data.Name = csName;
	// Type, Code, Name is update when user changes them.
	// Get Analysis Data
	GetAnalData(CCM_TYPE_STEEL_I, m_Data.Data1.Analysis);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CCMMatPageItemDlg2, CDialog)
	//{{AFX_MSG_MAP(CCMMatPageItemDlg2)
	ON_CBN_SELCHANGE(IDC_CMD_MP_ITEM_NAME_CBO, OnChangeSteelName)
	ON_BN_CLICKED(IDC_CMD_MP_ITEM_BTN_APPLY, OnApply)
	ON_EN_CHANGE(IDC_CMD_MP_ITEM_NAME, OnChangeName)
	ON_EN_CHANGE(IDC_CMD_MP_ITEM_ID, OnChangeID)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMatPageItemDlg message handlers


BOOL CCMMatPageItemDlg2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (!m_bModify) 
	{
		m_Key = m_pDoc->m_pAttrCtrl->GetStartNumMatl();
		m_Data.Initialize();

		m_Data.Type = CCM_TYPE_STEEL;
		m_Data.Data1.CodeName = CCM_STEEL_CODE_KS;
	}
	InitDlgCtrls();
	ShowDataToDlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCMMatPageItemDlg2::OnChangeSteelName() 
{
	// TODO: Add your control notification handler code here
	CString csType, csCode, csName;

	m_wndSteelName.GetWindowText(csName);
	csCode = m_Data.Data1.CodeName;

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(csCode, csName);
#endif

	m_Data.Data1.CodeMatlName = csName;

	// decide code is steel or concrete
	csType = CCM_TYPE_STEEL;

	// get analyis data
	if (csType == CCM_TYPE_STEEL) // steel
		GetSteelData(csCode, csName, m_Data.Data1.Analysis);
	else ASSERT(0);	

	// show analysis data to edit box
	SetAnalData(CCM_TYPE_STEEL_I, m_Data.Data1.Analysis);

	// change material name
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(csCode, csName);
#endif
	m_Data.Name = csName;
	m_wndName.SetWindowText(m_Data.Name);
}

void CCMMatPageItemDlg2::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data()) return;
	if (m_bModify)
	{
		if (!m_pDoc->m_pDataCtrl->ModifyMatl(m_OldKey, m_Key, m_Data)) return;
	}
	else 
	{
		if (!m_pDoc->m_pDataCtrl->AddMatl(m_Key, m_Data)) return;
	}
	CDialog::OnOK();
}

void CCMMatPageItemDlg2::OnApply() 
{
	// TODO: Add your control notification handler code here
	if (!Dlg2Data()) return;

	if (m_bModify)
	{
		if (!m_pDoc->m_pDataCtrl->ModifyMatl(m_OldKey, m_Key, m_Data)) return;
		m_OldKey = m_Key;
	}
	else 
	{
		if (!m_pDoc->m_pDataCtrl->AddMatl(m_Key, m_Data)) return;
		m_Key = m_pDoc->m_pAttrCtrl->GetNextNumMatl(m_Key);
	}

	// Initialize after apply
	if (!m_bModify)
	{
		CString csValue;
		csValue.Format(_T("%d"), m_Key);
		m_wndID.SetWindowText(csValue);
		m_Data.Name = _T("");
		m_wndName.SetWindowText(m_Data.Name);
	}
}


void CCMMatPageItemDlg2::OnChangeName() 
{
	CString csName;
	m_wndName.GetWindowText(csName);
	m_Data.Name = csName;
}

void CCMMatPageItemDlg2::OnChangeID() 
{
	int nID;
	if (m_wndID.GetEditValue(nID))
		m_Key = (T_MATL_K)nID;
}

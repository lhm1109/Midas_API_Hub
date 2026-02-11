// NLLinkPrtGapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "DamperGBItem6_VED.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_InitCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDamperGBItem_VED dialog


CDamperGBItem_VED::CDamperGBItem_VED(CWnd* pParent)
	: CChildDialog(CDamperGBItem_VED::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDamperGBItem_VED)
	m_nDof = 0;
	m_nDampType = 1;
	//}}AFX_DATA_INIT
}
CDamperGBItem_VED::~CDamperGBItem_VED()
{

}

void CDamperGBItem_VED::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDamperGBItem_VED)
	DDX_Radio(pDX, IDC_ETC_DAMPER_DX_RADIO, m_nDof);
	DDX_Radio(pDX, IDC_MAXWELL_MODEL_RADIO, m_nDampType);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_DAMPFORCE_EDIT, m_wndDamperFEdit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_VELEXPONENT_EDIT, m_wndIndexEdit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_BRACSTIFFNESS_EDIT, m_wndSpringStiffEdit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_DAMPSTIFFNESS_EDIT, m_wndDamperStiffEdit);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_DDFORCE_EDIT, m_wndDesignFEdit);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_ACCUSTIFF_EDIT, m_wndAccuStiffEdit);

	DDX_Control(pDX, IDC_ETC_MEC_PROP_DAMPFORCE_UNT, m_wndDamperFUnit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_BRACSTIFFNESS_UNT, m_wndSpringStiffUnit);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_DDFORCE_UNT, m_wndDesignFUnit);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_ACCUSTIFF_UNT, m_wndAccuStiffUnit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDamperGBItem_VED, CChildDialog)
	//{{AFX_MSG_MAP(CDamperGBItem_VED)
	ON_BN_CLICKED(IDC_MAXWELL_MODEL_RADIO, OnCmdDamperTypeRdo)
	ON_BN_CLICKED(IDC_KELVIN_MODEL_RADIO, OnCmdDamperTypeRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDamperGBItem_VED message handlers
BOOL CDamperGBItem_VED::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	//////////////////////////////////////////////
	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl;
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;
	T_UNIT_INDEX CurIndex, CodeIndex;
	pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CodeIndex = CurIndex;
	CodeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;
	pUnitCtrl->SetUnitIndexCurrent(CodeIndex);
	pUnitCtrl->SetUnitIndexCurrent(CurIndex);

	m_wndAccuStiffUnit.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndSpringStiffUnit.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndDamperFUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_wndDesignFUnit.SetUnitType(D_UNITSYS_BASE_FORCE);

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDamperGBItem_VED::Data2Dlg()
{
	m_wndDamperFEdit.SetEditUnit(m_Data.VED.dDamperF);
	m_wndIndexEdit.SetEditUnit(m_Data.VED.dIndex);
	m_wndSpringStiffEdit.SetEditUnit(m_Data.VED.dSpringStiff);
	m_wndDamperStiffEdit.SetEditUnit(m_Data.VED.dDamperStiff);
	m_wndDesignFEdit.SetEditUnit(m_Data.VED.dDesignF);
	m_wndAccuStiffEdit.SetEditUnit(m_Data.VED.dEffectStiff);
	m_nDof = m_Data.VED.nDof;
	m_nDampType = m_Data.VED.nDampType;
	switch (m_nDof)
	{
	case 0: CheckRadioButton(IDC_ETC_DAMPER_DX_RADIO, IDC_ETC_DAMPER_RZ_RADIO, IDC_ETC_DAMPER_DX_RADIO); break;
	case 1: CheckRadioButton(IDC_ETC_DAMPER_DX_RADIO, IDC_ETC_DAMPER_RZ_RADIO, IDC_ETC_DAMPER_DY_RADIO); break;
	case 2: CheckRadioButton(IDC_ETC_DAMPER_DX_RADIO, IDC_ETC_DAMPER_RZ_RADIO, IDC_ETC_DAMPER_DZ_RADIO); break;
	case 3: CheckRadioButton(IDC_ETC_DAMPER_DX_RADIO, IDC_ETC_DAMPER_RZ_RADIO, IDC_ETC_DAMPER_RX_RADIO); break;
	case 4: CheckRadioButton(IDC_ETC_DAMPER_DX_RADIO, IDC_ETC_DAMPER_RZ_RADIO, IDC_ETC_DAMPER_RY_RADIO); break;
	case 5: CheckRadioButton(IDC_ETC_DAMPER_DX_RADIO, IDC_ETC_DAMPER_RZ_RADIO, IDC_ETC_DAMPER_RZ_RADIO); break;
	default:
		break;
	}
	switch (m_nDampType)
	{
	case 0: CheckRadioButton(IDC_MAXWELL_MODEL_RADIO, IDC_KELVIN_MODEL_RADIO, IDC_MAXWELL_MODEL_RADIO); break;
	case 1: CheckRadioButton(IDC_MAXWELL_MODEL_RADIO, IDC_KELVIN_MODEL_RADIO, IDC_KELVIN_MODEL_RADIO); break;
	default:
		break;
	}
	EnableDisableItemCtrls(m_Data.VED.nInputMethod);
}

BOOL CDamperGBItem_VED::Dlg2Data()
{
	UpdateData();
	m_Data.nPropType = 5;
	m_Data.VED.dDamperF = m_wndDamperFEdit.GetEditValue();
	m_Data.VED.dIndex = m_wndIndexEdit.GetEditValue();
	m_Data.VED.dSpringStiff = m_wndSpringStiffEdit.GetEditValue();
	m_Data.VED.dDamperStiff = m_wndDamperStiffEdit.GetEditValue();
	m_Data.VED.dDesignF = m_wndDesignFEdit.GetEditValue();
	m_Data.VED.dEffectStiff = m_wndAccuStiffEdit.GetEditValue();
	int nDof = GetCheckedRadioButton(IDC_ETC_DAMPER_DX_RADIO, IDC_ETC_DAMPER_RZ_RADIO);
	switch (nDof)
	{
	case IDC_ETC_DAMPER_DX_RADIO: m_Data.VED.nDof = 0; break;
	case IDC_ETC_DAMPER_DY_RADIO: m_Data.VED.nDof = 1; break;
	case IDC_ETC_DAMPER_DZ_RADIO: m_Data.VED.nDof = 2; break;
	case IDC_ETC_DAMPER_RX_RADIO: m_Data.VED.nDof = 3; break;
	case IDC_ETC_DAMPER_RY_RADIO: m_Data.VED.nDof = 4; break;
	case IDC_ETC_DAMPER_RZ_RADIO: m_Data.VED.nDof = 5; break;
	default:							  
		break;
	}
	int nDampType = GetCheckedRadioButton(IDC_MAXWELL_MODEL_RADIO, IDC_KELVIN_MODEL_RADIO);
	switch (nDampType)
	{
	case IDC_MAXWELL_MODEL_RADIO: m_Data.VED.nDampType = 0; break;
	case IDC_KELVIN_MODEL_RADIO: m_Data.VED.nDampType = 1; break;
	default:
		break;
	}
	return TRUE;
}

void CDamperGBItem_VED::EnableDisableItemCtrls(int nInputMethod)
{
	if (nInputMethod == 1)
	{
		m_wndDamperFEdit.SetReadOnly(FALSE);
		m_wndIndexEdit.SetReadOnly(TRUE);
		m_wndDesignFEdit.SetReadOnly(TRUE);
		m_wndAccuStiffEdit.SetReadOnly(FALSE);
		if (m_Data.VED.nDampType == 0)
		{
			m_wndSpringStiffEdit.SetReadOnly(FALSE);
			GetDlgItem(IDC_ETC_MEC_PROP_DAMPSTIFFNESS_STATIC)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_ETC_MEC_PROP_DAMPSTIFFNESS_EDIT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_ETC_MEC_PROP_BRACSTIFFNESS_STATIC)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_ETC_MEC_PROP_BRACSTIFFNESS_EDIT)->ShowWindow(SW_SHOW);
		}
		else
		{
			m_wndDamperStiffEdit.SetReadOnly(FALSE);
			GetDlgItem(IDC_ETC_MEC_PROP_BRACSTIFFNESS_STATIC)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_ETC_MEC_PROP_BRACSTIFFNESS_EDIT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_ETC_MEC_PROP_DAMPSTIFFNESS_STATIC)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_ETC_MEC_PROP_DAMPSTIFFNESS_EDIT)->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		m_wndDamperFEdit.SetReadOnly(FALSE);
		m_wndIndexEdit.SetReadOnly(FALSE);
		m_wndDesignFEdit.SetReadOnly(FALSE);
		m_wndAccuStiffEdit.SetReadOnly(FALSE);
		if (m_Data.VED.nDampType == 0)
		{
			m_wndSpringStiffEdit.SetReadOnly(FALSE);
			GetDlgItem(IDC_ETC_MEC_PROP_DAMPSTIFFNESS_STATIC)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_ETC_MEC_PROP_DAMPSTIFFNESS_EDIT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_ETC_MEC_PROP_BRACSTIFFNESS_STATIC)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_ETC_MEC_PROP_BRACSTIFFNESS_EDIT)->ShowWindow(SW_SHOW);
		}
		else
		{
			m_wndDamperStiffEdit.SetReadOnly(FALSE);
			GetDlgItem(IDC_ETC_MEC_PROP_BRACSTIFFNESS_STATIC)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_ETC_MEC_PROP_BRACSTIFFNESS_EDIT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_ETC_MEC_PROP_DAMPSTIFFNESS_STATIC)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_ETC_MEC_PROP_DAMPSTIFFNESS_EDIT)->ShowWindow(SW_SHOW);
		}
	}
}

void CDamperGBItem_VED::OnCmdDamperTypeRdo()
{
	int nSel = GetCheckedRadioButton(IDC_MAXWELL_MODEL_RADIO, IDC_KELVIN_MODEL_RADIO);
	if (nSel == IDC_MAXWELL_MODEL_RADIO)
	{
		GetDlgItem(IDC_ETC_MEC_PROP_DAMPSTIFFNESS_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETC_MEC_PROP_DAMPSTIFFNESS_EDIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETC_MEC_PROP_BRACSTIFFNESS_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ETC_MEC_PROP_BRACSTIFFNESS_EDIT)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_ETC_MEC_PROP_BRACSTIFFNESS_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETC_MEC_PROP_BRACSTIFFNESS_EDIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETC_MEC_PROP_DAMPSTIFFNESS_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ETC_MEC_PROP_DAMPSTIFFNESS_EDIT)->ShowWindow(SW_SHOW);
	}
}

void CDamperGBItem_VED::SetData2Dlg(T_DMGB_D& data)
{
	m_Data.VED.nInputMethod = data.VED.nInputMethod; //
	m_Data.VED.nCode = data.VED.nCode;
	m_Data.VED.nClassify = data.VED.nClassify;
	m_Data.VED.nInputType = data.VED.nInputMethod;
	m_Data.VED.dDamperF = data.VED.dDamperF;
	m_Data.VED.dIndex = data.VED.dIndex;
	m_Data.VED.dSpringStiff = data.VED.dSpringStiff;
	m_Data.VED.dDamperStiff = data.VED.dDamperStiff;
	m_Data.VED.dDesignF = data.VED.dDesignF;
	m_Data.VED.dEffectStiff = data.VED.dEffectStiff;
	m_Data.VED.nDof = data.VED.nDof;
	m_Data.VED.nDampType = data.VED.nDampType;

	Data2Dlg();
}

BOOL CDamperGBItem_VED::SetDlg2Data(T_DMGB_D& data)
{
	BOOL bOK = (Dlg2Data() && CheckData()) ? TRUE : FALSE;
	data = m_Data;
	return bOK;
}
void CDamperGBItem_VED::OnOK()
{
	// TODO: Add extra validation here
	if (!Dlg2Data() || !CheckData())	return;

	CChildDialog::OnOK();
}

BOOL CDamperGBItem_VED::CheckData()
{
	if (m_wndDamperFEdit.GetEditValue() <= 0 || m_wndIndexEdit.GetEditValue() <= 0 || m_wndDesignFEdit.GetEditValue() <= 0
		|| m_wndAccuStiffEdit.GetEditValue() <= 0 || (m_Data.VED.nDampType == 0 && m_wndSpringStiffEdit.GetEditValue() <= 0)
		|| (m_Data.VED.nDampType == 1 && m_wndDamperStiffEdit.GetEditValue() <= 0))
	{
		AfxMessageBox(_LS(IDS_CMD_INPUT_VALUE_ERROR)); 	return FALSE;
	}
	return TRUE;
}

void CDamperGBItem_VED::OnCancel()
{
	// TODO: Add extra cleanup here

	CChildDialog::OnCancel();
}


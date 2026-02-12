// NLLinkPrtGapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "DamperGBItem4_VFD.h"
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
// CDamperGBItem_VFD dialog


CDamperGBItem_VFD::CDamperGBItem_VFD(CWnd* pParent)
	: CChildDialog(CDamperGBItem_VFD::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDamperGBItem_VFD)
	m_nDof = 0;
	m_nDampType = 0;
	//}}AFX_DATA_INIT
}
CDamperGBItem_VFD::~CDamperGBItem_VFD()
{

}

void CDamperGBItem_VFD::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDamperGBItem_VFD)
	DDX_Radio(pDX, IDC_ETC_DAMPER_DX_RADIO, m_nDof);
	DDX_Radio(pDX, IDC_MAXWELL_MODEL_RADIO, m_nDampType);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_DAMPFORCE_EDIT, m_wndDamperFEdit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_VELOEXPONENT_EDIT, m_wndIndexEdit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_BRACSTIFFNESS_EDIT, m_wndSpringStiffEdit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_DAMPSTIFFNESS_EDIT, m_wndDamperStiffEdit);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_DDFORCE_EDIT, m_wndDesignFEdit);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_DESIGNDISP_EDIT, m_wndDispEdit);

	DDX_Control(pDX, IDC_ETC_MEC_PROP_DAMPFORCE_UNT, m_wndDamperFUnit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_BRACSTIFFNESS_UNT, m_wndSpringStiffUnit);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_DDFORCE_UNT, m_wndDesignFUnit);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_DESIGNDISP_UNT, m_wndDispUnit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDamperGBItem_VFD, CChildDialog)
	//{{AFX_MSG_MAP(CDamperGBItem_VFD)
	ON_BN_CLICKED(IDC_MAXWELL_MODEL_RADIO, OnCmdDamperTypeRdo)
	ON_BN_CLICKED(IDC_KELVIN_MODEL_RADIO, OnCmdDamperTypeRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDamperGBItem_VFD message handlers
BOOL CDamperGBItem_VFD::OnInitDialog() 
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

	m_wndDispUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndSpringStiffUnit.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndDamperFUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_wndDesignFUnit.SetUnitType(D_UNITSYS_BASE_FORCE);

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDamperGBItem_VFD::Data2Dlg()
{
	m_wndDamperFEdit.SetEditUnit(m_Data.VFD.dDamperF);
	m_wndIndexEdit.SetEditUnit(m_Data.VFD.dIndex);
	m_wndSpringStiffEdit.SetEditUnit(m_Data.VFD.dSpringStiff);
	m_wndDamperStiffEdit.SetEditUnit(m_Data.VFD.dDamperStiff);
	m_wndDesignFEdit.SetEditUnit(m_Data.VFD.dDesignF);
	m_wndDispEdit.SetEditUnit(m_Data.VFD.dDisp);
	m_nDof = m_Data.VFD.nDof;
	m_nDampType = m_Data.VFD.nDampType;
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
	EnableDisableItemCtrls(m_Data.VFD.nInputMethod);
}

BOOL CDamperGBItem_VFD::Dlg2Data()
{
	UpdateData();
	m_Data.nPropType = 3;
	m_Data.VFD.dDamperF = m_wndDamperFEdit.GetEditValue();
	m_Data.VFD.dIndex = m_wndIndexEdit.GetEditValue();
	m_Data.VFD.dSpringStiff = m_wndSpringStiffEdit.GetEditValue();
	m_Data.VFD.dDamperStiff = m_wndDamperStiffEdit.GetEditValue();
	m_Data.VFD.dDesignF = m_wndDesignFEdit.GetEditValue();
	m_Data.VFD.dDisp = m_wndDispEdit.GetEditValue();
	int nDof = GetCheckedRadioButton(IDC_ETC_DAMPER_DX_RADIO, IDC_ETC_DAMPER_RZ_RADIO);
	switch (nDof)
	{
	case IDC_ETC_DAMPER_DX_RADIO: m_Data.VFD.nDof = 0; break;
	case IDC_ETC_DAMPER_DY_RADIO: m_Data.VFD.nDof = 1; break;
	case IDC_ETC_DAMPER_DZ_RADIO: m_Data.VFD.nDof = 2; break;
	case IDC_ETC_DAMPER_RX_RADIO: m_Data.VFD.nDof = 3; break;
	case IDC_ETC_DAMPER_RY_RADIO: m_Data.VFD.nDof = 4; break;
	case IDC_ETC_DAMPER_RZ_RADIO: m_Data.VFD.nDof = 5; break;
	default:
		break;
	}
	int nDampType = GetCheckedRadioButton(IDC_MAXWELL_MODEL_RADIO, IDC_KELVIN_MODEL_RADIO);
	switch (nDampType)
	{
	case IDC_MAXWELL_MODEL_RADIO: m_Data.VFD.nDampType = 0; break;
	case IDC_KELVIN_MODEL_RADIO: m_Data.VFD.nDampType = 1; break;
	default:
		break;
	}
	return TRUE;
}

void CDamperGBItem_VFD::EnableDisableItemCtrls(int nInputMethod)
{
	if (nInputMethod == 1)
	{
		m_wndDamperFEdit.SetReadOnly(TRUE);
		m_wndIndexEdit.SetReadOnly(TRUE);
		m_wndDesignFEdit.SetReadOnly(TRUE);
		m_wndDispEdit.SetReadOnly(FALSE);
		if (m_Data.VFD.nDampType == 0)
		{
			m_wndSpringStiffEdit.SetReadOnly(TRUE);
			GetDlgItem(IDC_ETC_MEC_PROP_DAMPSTIFFNESS_STATIC)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_ETC_MEC_PROP_DAMPSTIFFNESS_EDIT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_ETC_MEC_PROP_BRACSTIFFNESS_STATIC)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_ETC_MEC_PROP_BRACSTIFFNESS_EDIT)->ShowWindow(SW_SHOW);
		}
		else
		{
			m_wndDamperStiffEdit.SetReadOnly(TRUE);
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
		m_wndDispEdit.SetReadOnly(FALSE);
		if (m_Data.VFD.nDampType == 0)
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

void CDamperGBItem_VFD::OnCmdDamperTypeRdo()
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

void CDamperGBItem_VFD::SetData2Dlg(T_DMGB_D& data)
{
	m_Data.VFD.nInputMethod = data.VFD.nInputMethod; //
	m_Data.VFD.nCode = data.VFD.nCode;
	m_Data.VFD.nClassify = data.VFD.nClassify;
	m_Data.VFD.nInputType = data.VFD.nInputMethod;
	m_Data.VFD.dDamperF = data.VFD.dDamperF;
	m_Data.VFD.dIndex = data.VFD.dIndex;
	m_Data.VFD.dSpringStiff = data.VFD.dSpringStiff; 
	m_Data.VFD.dDamperStiff = data.VFD.dDamperStiff;
	m_Data.VFD.dDesignF = data.VFD.dDesignF;
	m_Data.VFD.dDisp = data.VFD.dDisp;
	m_Data.VFD.nDof = data.VFD.nDof;
	m_Data.VFD.nDampType = data.VFD.nDampType;

	Data2Dlg();
}

BOOL CDamperGBItem_VFD::SetDlg2Data(T_DMGB_D& data)
{
	BOOL bOK = (Dlg2Data() && CheckData()) ? TRUE : FALSE;
	data = m_Data;
	return bOK;
}
void CDamperGBItem_VFD::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data() || !CheckData())	return;

	CChildDialog::OnOK();
}

BOOL CDamperGBItem_VFD::CheckData()
{
	if (m_wndDamperFEdit.GetEditValue() <= 0 || m_wndIndexEdit.GetEditValue() <= 0 || m_wndDesignFEdit.GetEditValue() <= 0
		|| m_wndDispEdit.GetEditValue() <= 0 || (m_Data.VFD.nDampType == 0 && m_wndSpringStiffEdit.GetEditValue() <= 0)
		|| (m_Data.VFD.nDampType == 1 && m_wndDamperStiffEdit.GetEditValue() <= 0))
	{
		AfxMessageBox(_LS(IDS_CMD_INPUT_VALUE_ERROR)); 	return FALSE;
	}
	return TRUE;
}

void CDamperGBItem_VFD::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CChildDialog::OnCancel();
}


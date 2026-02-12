// NLLinkPrtGapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "DamperGBItem5_HDRD.h"
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
// CDamperGBItem_HDRD dialog


CDamperGBItem_HDRD::CDamperGBItem_HDRD(CWnd* pParent)
	: CChildDialog(CDamperGBItem_HDRD::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDamperGBItem_HDRD)
	//}}AFX_DATA_INIT
}
CDamperGBItem_HDRD::~CDamperGBItem_HDRD()
{

}

void CDamperGBItem_HDRD::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDamperGBItem_HDRD)
	DDX_Radio(pDX, IDC_ETC_DAMPER_DX_RADIO, m_nDof);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_INITSTIFFNESS_EDIT, m_wndInitStiffEdit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_YIELDSTRENGTH_EDIT, m_wndFyEdit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_POSTSTIFFNESS_EDIT, m_wndPostStiffEdit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_STIFFRTO_EDIT, m_wndPostStiffRtoEdit);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_EQUISTIFFNESS_EDIT, m_wndEquiStiffEdit);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_EQUIDAMPER_EDIT, m_wndEquiStiffRtoEdit);
	

	DDX_Control(pDX, IDC_ETC_MEC_PROP_INITSTIFF_UNT, m_wndInitStiffUnit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_YIELDSTRENGTH_UNT, m_wndFyUnit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_POSTSTIFFNESS_UNT, m_wndPostStiffUnit);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_EQUISTIFFNESS_UNT, m_wndEquiStiffUnit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDamperGBItem_HDRD, CChildDialog)
	//{{AFX_MSG_MAP(CDamperGBItem_HDRD)
	ON_EN_CHANGE(IDC_ETC_MEC_PROP_INITSTIFFNESS_EDIT, OnUpdatePostStiffnessValue)
	ON_EN_CHANGE(IDC_ETC_MEC_PROP_STIFFRTO_EDIT, OnUpdatePostStiffnessValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDamperGBItem_HDRD message handlers
BOOL CDamperGBItem_HDRD::OnInitDialog() 
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

	m_wndInitStiffUnit.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndFyUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_wndPostStiffUnit.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndEquiStiffUnit.SetUnitType(D_UNITSYS_BASE_SPRING);

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDamperGBItem_HDRD::Data2Dlg()
{
	m_wndInitStiffEdit.SetEditUnit(m_Data.HDRD.dInitStiff);
	m_wndFyEdit.SetEditUnit(m_Data.HDRD.dFy);
	m_wndPostStiffEdit.SetEditUnit(m_Data.HDRD.dYieldStiff);
	m_wndPostStiffRtoEdit.SetEditUnit(m_Data.HDRD.dYieldStiffRot);
	m_wndEquiStiffEdit.SetEditUnit(m_Data.HDRD.dEquiStiff);
	m_wndEquiStiffRtoEdit.SetEditUnit(m_Data.HDRD.dEquiDampRto);
	m_nDof = m_Data.HDRD.nDof;
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
	EnableDisableItemCtrls(m_Data.HDRD.nInputMethod);
}

BOOL CDamperGBItem_HDRD::Dlg2Data()
{
	UpdateData();
	m_Data.nPropType = 4;
	m_Data.HDRD.dInitStiff = m_wndInitStiffEdit.GetEditValue();
	m_Data.HDRD.dFy = m_wndFyEdit.GetEditValue();
	m_Data.HDRD.dYieldStiff = m_wndPostStiffEdit.GetEditValue();
	m_Data.HDRD.dYieldStiffRot = m_wndPostStiffRtoEdit.GetEditValue();
	m_Data.HDRD.dEquiStiff = m_wndEquiStiffEdit.GetEditValue();
	m_Data.HDRD.dEquiDampRto = m_wndEquiStiffRtoEdit.GetEditValue();
	int nDof = GetCheckedRadioButton(IDC_ETC_DAMPER_DX_RADIO, IDC_ETC_DAMPER_RZ_RADIO);
	switch (nDof)
	{
	case IDC_ETC_DAMPER_DX_RADIO: m_Data.HDRD.nDof = 0; break;
	case IDC_ETC_DAMPER_DY_RADIO: m_Data.HDRD.nDof = 1; break;
	case IDC_ETC_DAMPER_DZ_RADIO: m_Data.HDRD.nDof = 2; break;
	case IDC_ETC_DAMPER_RX_RADIO: m_Data.HDRD.nDof = 3; break;
	case IDC_ETC_DAMPER_RY_RADIO: m_Data.HDRD.nDof = 4; break;
	case IDC_ETC_DAMPER_RZ_RADIO: m_Data.HDRD.nDof = 5; break;
	default:
		break;
	}
	return TRUE;
}

void CDamperGBItem_HDRD::EnableDisableItemCtrls(int nInputMethod)
{
	if (nInputMethod == 1)
	{
		m_wndInitStiffEdit.SetReadOnly(FALSE);
		m_wndFyEdit.SetReadOnly(TRUE);
		m_wndPostStiffEdit.SetReadOnly(TRUE);
		m_wndPostStiffRtoEdit.SetReadOnly(TRUE);
		m_wndEquiStiffEdit.SetReadOnly(TRUE);
		m_wndEquiStiffRtoEdit.SetReadOnly(TRUE);
	}
	else
	{
		m_wndInitStiffEdit.SetReadOnly(FALSE);
		m_wndFyEdit.SetReadOnly(FALSE);
		m_wndPostStiffEdit.SetReadOnly(TRUE);
		m_wndPostStiffRtoEdit.SetReadOnly(FALSE);
		m_wndEquiStiffEdit.SetReadOnly(FALSE);
		m_wndEquiStiffRtoEdit.SetReadOnly(FALSE);
	}
}

void CDamperGBItem_HDRD::OnUpdatePostStiffnessValue()
{
	UpdateData(TRUE);
	double dInit = m_wndInitStiffEdit.GetEditValue();
	double dRto = m_wndPostStiffRtoEdit.GetEditValue();
	m_wndPostStiffEdit.SetEditUnit(dInit * dRto);
	UpdateData(FALSE);
}

void CDamperGBItem_HDRD::SetData2Dlg(T_DMGB_D& data)
{
	m_Data.HDRD.nInputMethod = data.HDRD.nInputMethod; //
	m_Data.HDRD.nCode = data.HDRD.nCode;
	m_Data.HDRD.nClassify = data.HDRD.nClassify;
	m_Data.HDRD.nInputType = data.HDRD.nInputMethod;
	m_Data.HDRD.nDof = data.HDRD.nDof;
	m_Data.HDRD.dInitStiff = data.HDRD.dInitStiff;
	m_Data.HDRD.dFy = data.HDRD.dFy;
	m_Data.HDRD.dYieldStiff = data.HDRD.dYieldStiff;
	m_Data.HDRD.dYieldStiffRot = data.HDRD.dYieldStiffRot;
	m_Data.HDRD.dEquiStiff = data.HDRD.dEquiStiff;
	m_Data.HDRD.dEquiDampRto = data.HDRD.dEquiDampRto;
	Data2Dlg();
}

BOOL CDamperGBItem_HDRD::SetDlg2Data(T_DMGB_D& data)
{
	BOOL bOK = (Dlg2Data() && CheckData()) ? TRUE : FALSE;
	data = m_Data;
	return bOK;
}
void CDamperGBItem_HDRD::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data() || !CheckData())	return;

	CChildDialog::OnOK();
}
BOOL CDamperGBItem_HDRD::CheckData()
{
	if (m_wndInitStiffEdit.GetEditValue() <= 0 || m_wndFyEdit.GetEditValue() <= 0 || m_wndPostStiffEdit.GetEditValue() <= 0
		|| m_wndPostStiffRtoEdit.GetEditValue() <= 0 || m_wndEquiStiffEdit.GetEditValue() <= 0 || m_wndEquiStiffRtoEdit.GetEditValue() <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD_INPUT_VALUE_ERROR)); 	return FALSE;
	}
	return TRUE;
}

void CDamperGBItem_HDRD::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CChildDialog::OnCancel();
}


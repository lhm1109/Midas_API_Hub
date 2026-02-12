// NLLinkPrtGapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "DamperGBItem3_FD.h"
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
// CDamperGBItem_FD dialog


CDamperGBItem_FD::CDamperGBItem_FD(CWnd* pParent)
	: CChildDialog(CDamperGBItem_FD::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDamperGBItem_FD)
	//}}AFX_DATA_INIT
}
CDamperGBItem_FD::~CDamperGBItem_FD()
{

}

void CDamperGBItem_FD::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDamperGBItem_FD)
	DDX_Radio(pDX, IDC_ETC_DAMPER_DX_RADIO, m_nDof);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_FY_EDIT, m_wndFyEdit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_DISP_EDIT, m_wndDispEdit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_INITSTIFF_EDIT, m_wndInitStiffEdit);

	DDX_Control(pDX, IDC_ETC_MEC_PROP_FY_UNT, m_wndFyUnit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_DISP_UNT, m_wndDispUnit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_INITSTIFF_UNT, m_wndInitStiffUnit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDamperGBItem_FD, CChildDialog)
	//{{AFX_MSG_MAP(CDamperGBItem_FD)
	ON_EN_CHANGE(IDC_ETC_MEC_PROP_FY_EDIT, OnUpdateDispValue)
	ON_EN_CHANGE(IDC_ETC_MEC_PROP_INITSTIFF_EDIT, OnUpdateDispValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDamperGBItem_FD message handlers
BOOL CDamperGBItem_FD::OnInitDialog() 
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
	m_wndInitStiffUnit.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndFyUnit.SetUnitType(D_UNITSYS_BASE_FORCE);

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDamperGBItem_FD::Data2Dlg()
{
	m_nDof = m_Data.FD.nDof;
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
	m_wndFyEdit.SetEditUnit(m_Data.FD.dForce);
	if (m_Data.MYD.nInputMethod == 0)
	{
		m_wndInitStiffEdit.SetEditUnit(m_Data.FD.dInitStiff, 1);
		double dDisp = m_Data.FD.dForce / m_Data.FD.dInitStiff;
		m_wndDispEdit.SetEditUnit(dDisp);
	}
	else
	{
		m_wndDispEdit.SetEditUnit(m_Data.FD.dDisp);
		double dStiff = m_Data.FD.dForce / m_Data.FD.dDisp;
		m_wndInitStiffEdit.SetEditUnit(dStiff, 1);
	}

	EnableDisableItemCtrls(m_Data.FD.nInputMethod);
}

BOOL CDamperGBItem_FD::Dlg2Data()
{
	UpdateData();
	m_Data.nPropType = 2;
	m_Data.FD.dForce = m_wndFyEdit.GetEditValue();
	m_Data.FD.dDisp = m_wndDispEdit.GetEditValue();
	m_Data.FD.dInitStiff = m_wndInitStiffEdit.GetEditValue();
	int nDof = GetCheckedRadioButton(IDC_ETC_DAMPER_DX_RADIO, IDC_ETC_DAMPER_RZ_RADIO);
	switch (nDof)
	{
	case IDC_ETC_DAMPER_DX_RADIO: m_Data.FD.nDof = 0; break;
	case IDC_ETC_DAMPER_DY_RADIO: m_Data.FD.nDof = 1; break;
	case IDC_ETC_DAMPER_DZ_RADIO: m_Data.FD.nDof = 2; break;
	case IDC_ETC_DAMPER_RX_RADIO: m_Data.FD.nDof = 3; break;
	case IDC_ETC_DAMPER_RY_RADIO: m_Data.FD.nDof = 4; break;
	case IDC_ETC_DAMPER_RZ_RADIO: m_Data.FD.nDof = 5; break;
	default:
		break;
	}
	return TRUE;
}

void CDamperGBItem_FD::EnableDisableItemCtrls(int nInputMethod)
{
	if (nInputMethod == 1)
	{
		m_wndFyEdit.SetReadOnly(TRUE);
		m_wndDispEdit.SetReadOnly(TRUE);
		m_wndInitStiffEdit.SetReadOnly(TRUE);
	}
	else
	{
		m_wndDispEdit.SetReadOnly(TRUE);
		m_wndFyEdit.SetReadOnly(FALSE);
		m_wndInitStiffEdit.SetReadOnly(FALSE);
	}
}

void CDamperGBItem_FD::OnUpdateDispValue()
{
	UpdateData(TRUE);
	double dFy = m_wndFyEdit.GetEditValue();
	double dValue = m_wndInitStiffEdit.GetEditValue();
	m_wndDispEdit.SetEditUnit(dFy / dValue);
	UpdateData(FALSE);
}


void CDamperGBItem_FD::SetData2Dlg(T_DMGB_D& data)
{
	m_Data.FD.nInputMethod = data.FD.nInputMethod; //
	m_Data.FD.nCode = data.FD.nCode;
	m_Data.FD.nClassify = data.FD.nClassify;
	m_Data.FD.nInputType = data.FD.nInputMethod;
	m_Data.FD.nDof = data.FD.nDof;
	m_Data.FD.dForce = data.FD.dForce;
	m_Data.FD.dDisp = data.FD.dDisp; 
	Data2Dlg();
}

BOOL CDamperGBItem_FD::SetDlg2Data(T_DMGB_D& data)
{
	BOOL bOK = (Dlg2Data() && CheckData()) ? TRUE : FALSE;
	data = m_Data;
	return bOK;
}
void CDamperGBItem_FD::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data() || !CheckData())	return;

	CChildDialog::OnOK();
}

BOOL CDamperGBItem_FD::CheckData()
{
	if (m_wndFyEdit.GetEditValue() <= 0 || m_wndDispEdit.GetEditValue() <= 0 || m_wndInitStiffEdit.GetEditValue() <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD_INPUT_VALUE_ERROR)); 	return FALSE;
	}
	return TRUE;
}


void CDamperGBItem_FD::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CChildDialog::OnCancel();
}


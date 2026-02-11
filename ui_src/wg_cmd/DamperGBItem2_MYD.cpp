// NLLinkPrtGapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "DamperGBItem2_MYD.h"
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
// CDamperGBItem_MYD dialog


CDamperGBItem_MYD::CDamperGBItem_MYD(CWnd* pParent)
	: CChildDialog(CDamperGBItem_MYD::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDamperGBItem_MYD)
	//}}AFX_DATA_INIT
}
CDamperGBItem_MYD::~CDamperGBItem_MYD()
{

}

void CDamperGBItem_MYD::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDamperGBItem_MYD)
	DDX_Radio(pDX, IDC_ETC_DAMPER_DX_RADIO, m_nDof);
	DDX_Radio(pDX, IDC_ETC_MEC_PROP_COREMATL_RADIO, m_nCore);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_YIELDSTRENGTH_EDIT, m_wndFyEdit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_INITSTIFFNESS_EDIT, m_wndInitStiffEdit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_YIELDDISP_EDIT, m_wndYieldDispEdit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_STIFFNESSRTO_EDIT, m_wndStiffRotEdit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_YIELDSTRENGTH_UNT, m_wndFyUnit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_INITSTIFFNESS_UNT, m_wndInitStiffUnit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_YIELDDISP_UNT, m_wndYieldDispUnit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDamperGBItem_MYD, CChildDialog)
	//{{AFX_MSG_MAP(CDamperGBItem_MYD)
	ON_BN_CLICKED(IDC_ETC_MEC_PROP_COREMATL_RADIO, OnCmdCoreMaterialRdo)
	ON_BN_CLICKED(IDC_ETC_MEC_PROP_COREMATL_RADIO2, OnCmdCoreMaterialRdo)
	ON_BN_CLICKED(IDC_ETC_MEC_PROP_COREMATL_RADIO3, OnCmdCoreMaterialRdo)
	ON_EN_CHANGE(IDC_ETC_MEC_PROP_YIELDSTRENGTH_EDIT, OnUpdateYieldDispValue)
	ON_EN_CHANGE(IDC_ETC_MEC_PROP_INITSTIFFNESS_EDIT, OnUpdateYieldDispValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDamperGBItem_MYD message handlers
BOOL CDamperGBItem_MYD::OnInitDialog() 
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
	m_wndYieldDispUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndFyUnit.SetUnitType(D_UNITSYS_BASE_FORCE);


	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDamperGBItem_MYD::EnableDisableItemCtrls(int nInputMethod)
{
	if (nInputMethod == 1)
	{
		m_wndFyEdit.SetReadOnly(TRUE);
		m_wndInitStiffEdit.SetReadOnly(TRUE);
		m_wndYieldDispEdit.SetReadOnly(TRUE);
		m_wndStiffRotEdit.SetReadOnly(TRUE);
		GetDlgItem(IDC_ETC_MEC_PROP_COREMATL_STATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_ETC_MEC_PROP_COREMATL_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_ETC_MEC_PROP_COREMATL_RADIO2)->EnableWindow(TRUE);
		GetDlgItem(IDC_ETC_MEC_PROP_COREMATL_RADIO3)->EnableWindow(TRUE);
	}
	else
	{
		m_wndFyEdit.SetReadOnly(FALSE);
		m_wndInitStiffEdit.SetReadOnly(FALSE);
		m_wndYieldDispEdit.SetReadOnly(TRUE);
		m_wndStiffRotEdit.SetReadOnly(FALSE);
		GetDlgItem(IDC_ETC_MEC_PROP_COREMATL_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_ETC_MEC_PROP_COREMATL_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_ETC_MEC_PROP_COREMATL_RADIO2)->EnableWindow(FALSE);
		GetDlgItem(IDC_ETC_MEC_PROP_COREMATL_RADIO3)->EnableWindow(FALSE);
	}
}

void CDamperGBItem_MYD::Data2Dlg()
{
	m_wndFyEdit.SetEditUnit(m_Data.MYD.dFy);
	if (m_Data.MYD.nInputMethod == 0)
	{
		m_wndInitStiffEdit.SetEditUnit(m_Data.MYD.dInitStiff,1);
		double dDisp = m_Data.MYD.dFy / m_Data.MYD.dInitStiff;
		m_wndYieldDispEdit.SetEditUnit(dDisp);
	}
	else
	{
		m_wndYieldDispEdit.SetEditUnit(m_Data.MYD.dDisp);
		double dStiff = m_Data.MYD.dFy / m_Data.MYD.dDisp;
		m_wndInitStiffEdit.SetEditUnit(dStiff,1);
	}

	m_wndStiffRotEdit.SetEditUnit(m_Data.MYD.dStiffRot);
	m_nDof = m_Data.MYD.nDof;
	m_nCore = m_Data.MYD.nCoreMtl;
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
	switch (m_nCore)
	{
	case 0: CheckRadioButton(IDC_ETC_MEC_PROP_COREMATL_RADIO, IDC_ETC_MEC_PROP_COREMATL_RADIO3, IDC_ETC_MEC_PROP_COREMATL_RADIO); break;
	case 1: CheckRadioButton(IDC_ETC_MEC_PROP_COREMATL_RADIO, IDC_ETC_MEC_PROP_COREMATL_RADIO3, IDC_ETC_MEC_PROP_COREMATL_RADIO2); break;
	case 2: CheckRadioButton(IDC_ETC_MEC_PROP_COREMATL_RADIO, IDC_ETC_MEC_PROP_COREMATL_RADIO3, IDC_ETC_MEC_PROP_COREMATL_RADIO3); break;
	default:
		break;
	}
	EnableDisableItemCtrls(m_Data.MYD.nInputMethod);
}

BOOL CDamperGBItem_MYD::Dlg2Data()
{
	UpdateData();
	m_Data.nPropType = 1;
	m_Data.MYD.dFy = m_wndFyEdit.GetEditValue();
	m_Data.MYD.dDisp = m_wndYieldDispEdit.GetEditValue();
	m_Data.MYD.dInitStiff = m_wndInitStiffEdit.GetEditValue();
	m_Data.MYD.dStiffRot = m_wndStiffRotEdit.GetEditValue();
	int nDof = GetCheckedRadioButton(IDC_ETC_DAMPER_DX_RADIO, IDC_ETC_DAMPER_RZ_RADIO);
	switch (nDof)
	{
	case IDC_ETC_DAMPER_DX_RADIO: m_Data.MYD.nDof = 0; break;
	case IDC_ETC_DAMPER_DY_RADIO: m_Data.MYD.nDof = 1; break;
	case IDC_ETC_DAMPER_DZ_RADIO: m_Data.MYD.nDof = 2; break;
	case IDC_ETC_DAMPER_RX_RADIO: m_Data.MYD.nDof = 3; break;
	case IDC_ETC_DAMPER_RY_RADIO: m_Data.MYD.nDof = 4; break;
	case IDC_ETC_DAMPER_RZ_RADIO: m_Data.MYD.nDof = 5; break;
	default:
		break;
	}
	int nCore = GetCheckedRadioButton(IDC_ETC_MEC_PROP_COREMATL_RADIO, IDC_ETC_MEC_PROP_COREMATL_RADIO3);
	switch (nCore)
	{
	case IDC_ETC_MEC_PROP_COREMATL_RADIO: m_Data.MYD.nCoreMtl = 0; break;
	case IDC_ETC_MEC_PROP_COREMATL_RADIO2: m_Data.MYD.nCoreMtl = 1; break;
	case IDC_ETC_MEC_PROP_COREMATL_RADIO3: m_Data.MYD.nCoreMtl = 2; break;
	default:
		break;
	}
	return TRUE;
	
}

void CDamperGBItem_MYD::OnCmdCoreMaterialRdo()
{
	UpdateData();
	int nSel = GetCheckedRadioButton(IDC_ETC_MEC_PROP_COREMATL_RADIO, IDC_ETC_MEC_PROP_COREMATL_RADIO3);
	if (nSel == IDC_ETC_MEC_PROP_COREMATL_RADIO)
	{
		m_wndStiffRotEdit.SetWindowText(_T("0.035"));
	}
	else if (nSel == IDC_ETC_MEC_PROP_COREMATL_RADIO2)
	{
		m_wndStiffRotEdit.SetWindowText(_T("0.025"));
	}
	else
	{
		m_wndStiffRotEdit.SetWindowText(_T("0.05"));
	}
}

void CDamperGBItem_MYD::OnUpdateYieldDispValue()
{
	UpdateData(TRUE);
	double dFy = m_wndFyEdit.GetEditValue();
	double dVal = m_wndInitStiffEdit.GetEditValue();
	double dDisp = dFy / dVal;
	m_wndYieldDispEdit.SetEditUnit(dDisp);
	UpdateData(FALSE);
}

void CDamperGBItem_MYD::SetData2Dlg(T_DMGB_D& data)
{
	m_Data.MYD.nInputMethod = data.MYD.nInputMethod;
	m_Data.MYD.nCode = data.MYD.nCode;
	m_Data.MYD.nClassify = data.MYD.nClassify;
	m_Data.MYD.nInputType = data.MYD.nInputMethod;
	m_Data.MYD.nDof = data.MYD.nDof;
	m_Data.MYD.nCoreMtl = data.MYD.nCoreMtl;
	m_Data.MYD.dFy = data.MYD.dFy;
	m_Data.MYD.dDisp = data.MYD.dDisp;
	m_Data.MYD.dInitStiff = data.MYD.dInitStiff;
	m_Data.MYD.dStiffRot = data.MYD.dStiffRot;
	Data2Dlg();
}

BOOL CDamperGBItem_MYD::SetDlg2Data(T_DMGB_D& data)
{
	BOOL bOK = (Dlg2Data() && CheckData()) ? TRUE : FALSE;
	data = m_Data;
	return bOK;
}

void CDamperGBItem_MYD::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data() || !CheckData())	return;

	CChildDialog::OnOK();
}

BOOL CDamperGBItem_MYD::CheckData()
{
	if (m_wndFyEdit.GetEditValue() <= 0 || m_wndInitStiffEdit.GetEditValue() <= 0
		|| m_wndYieldDispEdit.GetEditValue() <= 0 || m_wndStiffRotEdit.GetEditValue() <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD_INPUT_VALUE_ERROR)); 	return FALSE;
	}

	return TRUE;
}


void CDamperGBItem_MYD::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CChildDialog::OnCancel();
}


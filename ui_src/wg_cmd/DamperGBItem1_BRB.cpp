// NLLinkPrtGapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "DamperGBItem1_BRB.h"
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
// CDamperGBItem_BRB dialog


CDamperGBItem_BRB::CDamperGBItem_BRB(CWnd* pParent)
	: CChildDialog(CDamperGBItem_BRB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDamperGBItem_BRB)
	//}}AFX_DATA_INIT
}
CDamperGBItem_BRB::~CDamperGBItem_BRB()
{

}

void CDamperGBItem_BRB::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDamperGBItem_BRB)
	DDX_Radio(pDX, IDC_ETC_DAMPER_DX_RADIO, m_nDof);
	DDX_Radio(pDX, IDC_ETC_MEC_PROP_COREMATL_RADIO, m_nCore);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_YIELDSTRENGTH_EDIT, m_wndFyEdit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_INITSTIFFNESS_EDIT, m_wndInitStiffEdit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_YIELDDISP_EDIT, m_wndYieldDispEdit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_STIFFNESSRTO_EDIT, m_wndStiffRotEdit);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_LENGTH_EDIT, m_wndLengthEdit);	
	DDX_Control(pDX, IDC_ETC_MEC_PROP_YIELDSTRENGTH_UNT, m_wndFyUnit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_INITSTIFFNESS_UNT, m_wndInitStiffUnit);
	DDX_Control(pDX, IDC_ETC_MEC_PROP_YIELDDISP_UNT, m_wndYieldDispUnit);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_LENGTH_UNT, m_wndLengthUnit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDamperGBItem_BRB, CChildDialog)
	//{{AFX_MSG_MAP(CDamperGBItem_BRB)
	ON_BN_CLICKED(IDC_ETC_MEC_PROP_COREMATL_RADIO, OnCmdCoreMaterialRdo)
	ON_BN_CLICKED(IDC_ETC_MEC_PROP_COREMATL_RADIO2, OnCmdCoreMaterialRdo)
	ON_EN_CHANGE(IDC_ETC_MEC_PROP_YIELDSTRENGTH_EDIT, OnUpdateYieldDispValue)
	ON_EN_CHANGE(IDC_ETC_MEC_PROP_INITSTIFFNESS_EDIT, OnUpdateYieldDispValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDamperGBItem_BRB message handlers
BOOL CDamperGBItem_BRB::OnInitDialog() 
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

	m_wndLengthUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndInitStiffUnit.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndYieldDispUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndFyUnit.SetUnitType(D_UNITSYS_BASE_FORCE);

	GetDlgItem(IDC_ETC_DAMPER_DY_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_ETC_DAMPER_DZ_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_ETC_DAMPER_RX_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_ETC_DAMPER_RY_RADIO)->EnableWindow(FALSE);
	GetDlgItem(IDC_ETC_DAMPER_RZ_RADIO)->EnableWindow(FALSE);

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDamperGBItem_BRB::Data2Dlg()
{
	m_wndFyEdit.SetEditUnit(m_Data.BRB.dFy);
	m_wndYieldDispEdit.SetEditUnit(m_Data.BRB.dDisp);
	if (m_Data.BRB.nInputMethod == 0)
	{
		m_wndInitStiffEdit.SetEditUnit(m_Data.BRB.dInitStiff,1);
		double dDisp = m_Data.BRB.dFy / m_Data.BRB.dInitStiff;
		m_wndYieldDispEdit.SetEditUnit(dDisp);
	}
	else
	{
		m_wndYieldDispEdit.SetEditUnit(m_Data.BRB.dDisp);
		double dStiff = m_Data.BRB.dFy / m_Data.BRB.dDisp;
		m_wndInitStiffEdit.SetEditUnit(dStiff,1);
	}
	
	m_wndStiffRotEdit.SetEditUnit(m_Data.BRB.dStiffRot);
	m_wndLengthEdit.SetEditUnit(m_Data.BRB.dLength);
	m_nDof = m_Data.BRB.nDof;
	m_nCore = m_Data.BRB.nCoreMtl;
	switch (m_nCore)
	{
	case 0: CheckRadioButton(IDC_ETC_MEC_PROP_COREMATL_RADIO, IDC_ETC_MEC_PROP_COREMATL_RADIO2, IDC_ETC_MEC_PROP_COREMATL_RADIO); break;
	case 1: CheckRadioButton(IDC_ETC_MEC_PROP_COREMATL_RADIO, IDC_ETC_MEC_PROP_COREMATL_RADIO2, IDC_ETC_MEC_PROP_COREMATL_RADIO2); break;
	default:
		break;
	}
	EnableDisableItemCtrls(m_Data.BRB.nInputMethod);
}

BOOL CDamperGBItem_BRB::Dlg2Data()
{
	UpdateData();
	m_Data.BRB.dFy = m_wndFyEdit.GetEditValue();
	m_Data.BRB.dDisp = m_wndYieldDispEdit.GetEditValue();
	m_Data.BRB.dInitStiff = m_wndInitStiffEdit.GetEditValue();
	m_Data.BRB.dStiffRot = m_wndStiffRotEdit.GetEditValue();
	m_Data.BRB.dLength = m_wndLengthEdit.GetEditValue();
	m_Data.BRB.nDof = 0;
	int nCore = GetCheckedRadioButton(IDC_ETC_MEC_PROP_COREMATL_RADIO, IDC_ETC_MEC_PROP_COREMATL_RADIO2);
	switch (nCore)
	{
	case IDC_ETC_MEC_PROP_COREMATL_RADIO: m_Data.BRB.nCoreMtl = 0; break;
	case IDC_ETC_MEC_PROP_COREMATL_RADIO2: m_Data.BRB.nCoreMtl = 1; break;
	default:
		break;
	}
	return TRUE;
}

void CDamperGBItem_BRB::EnableDisableItemCtrls(int nInputMethod)
{
	if (nInputMethod == 1)
	{
		m_wndFyEdit.SetReadOnly(TRUE);
		m_wndInitStiffEdit.SetReadOnly(TRUE);
		m_wndYieldDispEdit.SetReadOnly(TRUE);
		m_wndStiffRotEdit.SetReadOnly(TRUE);
		m_wndLengthEdit.SetReadOnly(TRUE);
		GetDlgItem(IDC_ETC_MEC_PROP_COREMATL_STATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_ETC_MEC_PROP_COREMATL_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_ETC_MEC_PROP_COREMATL_RADIO2)->EnableWindow(TRUE);
	}
	else
	{
		m_wndFyEdit.SetReadOnly(FALSE);
		m_wndInitStiffEdit.SetReadOnly(FALSE);
		m_wndYieldDispEdit.SetReadOnly(TRUE);
		m_wndStiffRotEdit.SetReadOnly(FALSE);
		m_wndLengthEdit.SetReadOnly(FALSE);
		GetDlgItem(IDC_ETC_MEC_PROP_COREMATL_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_ETC_MEC_PROP_COREMATL_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_ETC_MEC_PROP_COREMATL_RADIO2)->EnableWindow(FALSE);
	}
}

void CDamperGBItem_BRB::OnCmdCoreMaterialRdo()
{
	UpdateData();
	int nSel = GetCheckedRadioButton(IDC_ETC_MEC_PROP_COREMATL_RADIO, IDC_ETC_MEC_PROP_COREMATL_RADIO2);
	if (nSel == IDC_ETC_MEC_PROP_COREMATL_RADIO)
	{
		m_wndStiffRotEdit.SetWindowText(_T("0.035"));
	}
	else
	{
		m_wndStiffRotEdit.SetWindowText(_T("0.025"));
	}
}

void CDamperGBItem_BRB::OnUpdateYieldDispValue()
{
	UpdateData(TRUE);
	double dFy = m_wndFyEdit.GetEditValue();
	double dVal = m_wndInitStiffEdit.GetEditValue();
	m_wndYieldDispEdit.SetEditUnit(dFy / dVal);
	UpdateData(FALSE);
}

void CDamperGBItem_BRB::SetData2Dlg(T_DMGB_D& data)
{
	m_Data.BRB.nInputMethod = data.BRB.nInputMethod;
	m_Data.BRB.nCode = data.BRB.nCode;
	m_Data.BRB.nClassify = data.BRB.nClassify;
	m_Data.BRB.nInputType = data.BRB.nInputMethod;
	m_Data.BRB.nDof = data.BRB.nDof;
	m_Data.BRB.nCoreMtl = data.BRB.nCoreMtl;
	m_Data.BRB.dFy = data.BRB.dFy;
	m_Data.BRB.dDisp = data.BRB.dDisp;
	m_Data.BRB.dInitStiff = data.BRB.dInitStiff;
	m_Data.BRB.dStiffRot = data.BRB.dStiffRot;
	m_Data.BRB.dLength = data.BRB.dLength;
	m_Data.nPropType = 0;
	Data2Dlg();
}

BOOL CDamperGBItem_BRB::SetDlg2Data(T_DMGB_D& data)
{
	BOOL bOK = (Dlg2Data() && CheckData()) ? TRUE : FALSE;
	data = m_Data;
	return bOK;
}
void CDamperGBItem_BRB::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data() || !CheckData())	return;

	CChildDialog::OnOK();
}

BOOL CDamperGBItem_BRB::CheckData()
{
	if (m_wndFyEdit.GetEditValue() <= 0 || m_wndInitStiffEdit.GetEditValue() <= 0 || m_wndYieldDispEdit.GetEditValue() <= 0
		|| m_wndStiffRotEdit.GetEditValue() <= 0 || m_wndLengthEdit.GetEditValue() <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD_INPUT_VALUE_ERROR)); 	return FALSE;
	}
	return TRUE;
}

void CDamperGBItem_BRB::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CChildDialog::OnCancel();
}


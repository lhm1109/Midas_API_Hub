// NLLinkPrtGapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "AntivibrationSupportGBItem_ESB.h"
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
// CAntivibrationSupportGBItem_ESB dialog


CAntivibrationSupportGBItem_ESB::CAntivibrationSupportGBItem_ESB(CWnd* pParent /*=NULL*/)
	: CChildDialog(CAntivibrationSupportGBItem_ESB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAntivibrationSupportGBItem_ESB)
	//}}AFX_DATA_INIT
}
CAntivibrationSupportGBItem_ESB::~CAntivibrationSupportGBItem_ESB()
{

}

void CAntivibrationSupportGBItem_ESB::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAntivibrationSupportGBItem_ESB)
	DDX_Control(pDX, IDC_CMD_EFFECTIVE_D_EDIT, m_wndEffiDEdit);
	DDX_Control(pDX, IDC_CMD_RUBBER_TR_EDIT, m_wndEffiTrEdit);
	DDX_Control(pDX, IDC_CMD_MECH_KV_ESB_EDIT, m_wndMechKvcEdit_ESB);
	DDX_Control(pDX, IDC_CMD_MECH_KVT_ESB_EDIT, m_wndMechKvtEdit_ESB);
	DDX_Control(pDX, IDC_CMD_MECH_K1_ESB_EDIT, m_wndMechK1Edit_ESB);
	DDX_Control(pDX, IDC_CMD_ADJUST_COEF_TK_EDIT, m_wndTk);
	DDX_Control(pDX, IDC_CMD_ADJUST_COEF_TQ_EDIT, m_wndTq);
	DDX_Control(pDX, IDC_CMD_MECH_U_ESB_COMBO, m_wndMechUComb_ESB);

	DDX_Control(pDX, IDC_CMD_EFFECTIVE_D_UNT, m_wndEffiDUnit);
	DDX_Control(pDX, IDC_CMD_RUBBER_TR_UNT, m_wndEffiTrUnit);
	DDX_Control(pDX, IDC_CMD_MECH_KV_ESB_UNT, m_wndMechKvcUnit_ESB);
	DDX_Control(pDX, IDC_CMD_MECH_KVT_ESB_UNT, m_wndMechKvtUnit_ESB);
	DDX_Control(pDX, IDC_CMD_MECH_K1_ESB_UNT, m_wndMechK1Unit_ESB);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAntivibrationSupportGBItem_ESB, CChildDialog)
	//{{AFX_MSG_MAP(CAntivibrationSupportGBItem_ESB)
	ON_CBN_SELCHANGE(IDC_CMD_MECH_U_ESB_COMBO, OnSelChangeMechUCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAntivibrationSupportGBItem_ESB message handlers
BOOL CAntivibrationSupportGBItem_ESB::OnInitDialog() 
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

	m_wndEffiDUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndEffiTrUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndMechKvcUnit_ESB.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndMechKvtUnit_ESB.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndMechK1Unit_ESB.SetUnitType(D_UNITSYS_BASE_SPRING);

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAntivibrationSupportGBItem_ESB::Data2Dlg()
{
	m_wndEffiDEdit.SetEditUnit(m_Data.ESB.dD);
	m_wndEffiTrEdit.SetEditUnit(m_Data.ESB.dTr,3);
	m_wndMechKvcEdit_ESB.SetEditUnit(m_Data.ESB.dKv);
	m_wndMechKvtEdit_ESB.SetEditUnit(0);
	m_wndMechK1Edit_ESB.SetEditUnit(m_Data.ESB.dK1);
	m_wndTk.SetEditUnit(m_Data.ESB.dTk);
	m_wndTq.SetEditUnit(m_Data.ESB.dTq);
	InitMechUCmb();
	m_wndMechUComb_ESB.SetCurSel(m_Data.ESB.nSel);

	EnableDisableItemCtrls(m_Data.ESB.nInputMethod);
}

BOOL CAntivibrationSupportGBItem_ESB::Dlg2Data()
{
	UpdateData();
	m_Data.ESB.dD = m_wndEffiDEdit.GetEditValue();
	m_Data.ESB.dTr = m_wndEffiTrEdit.GetEditValue();
	m_Data.ESB.dKv = m_wndMechKvcEdit_ESB.GetEditValue();
	m_Data.ESB.dKvt = 0;
	m_Data.ESB.dK1 = m_wndMechK1Edit_ESB.GetEditValue();
	m_Data.ESB.dTk = m_wndTk.GetEditValue();
	m_Data.ESB.dTq = m_wndTq.GetEditValue();
	m_Data.ESB.nSel = m_wndMechUComb_ESB.GetCurSel();
	double dU[4] = { 0.02, 0.03, 0.04, 0.05 };
	m_Data.ESB.dU = dU[m_wndMechUComb_ESB.GetCurSel()];
	return TRUE;
}

void CAntivibrationSupportGBItem_ESB::OnSelChangeMechUCmb()
{
	UpdateData();
}

void CAntivibrationSupportGBItem_ESB::InitMechUCmb()
{
	m_wndMechUComb_ESB.ResetContent();
	double dU[4] = { 0.02, 0.03, 0.04, 0.05 };
	for (int i = 0; i < 4; i++)
	{
		CString strU = _T("");
		strU.Format(_T("%.2f"), dU[i]);
		CDlgUtil::CobxAddItem(m_wndMechUComb_ESB, strU, i);
	}
	m_wndMechUComb_ESB.SetCurSel(0);
}

void CAntivibrationSupportGBItem_ESB::EnableDisableItemCtrls(int nInputMethod)
{
	if (nInputMethod == 1)
	{
		m_wndEffiDEdit.SetReadOnly(TRUE);
		m_wndEffiTrEdit.SetReadOnly(TRUE);
		m_wndMechKvcEdit_ESB.SetReadOnly(TRUE);
		m_wndMechKvtEdit_ESB.SetReadOnly(TRUE);
		m_wndMechK1Edit_ESB.SetReadOnly(TRUE);
		m_wndTk.SetReadOnly(TRUE);
	}
	else
	{
		m_wndEffiDEdit.SetReadOnly(FALSE);
		m_wndEffiTrEdit.SetReadOnly(FALSE);
		m_wndMechKvcEdit_ESB.SetReadOnly(FALSE);
		m_wndMechKvtEdit_ESB.SetReadOnly(TRUE);
		m_wndMechK1Edit_ESB.SetReadOnly(FALSE);
		m_wndTk.SetReadOnly(FALSE);
	}
}

void CAntivibrationSupportGBItem_ESB::SetData2Dlg(T_ASGB_D& data)
{
	m_Data.ESB.nInputMethod = data.ESB.nInputMethod; //0:U
	m_Data.ESB.nCode = data.ESB.nCode;
	m_Data.ESB.nClassify = data.ESB.nClassify;
	m_Data.ESB.nInputType = data.ESB.nInputType;
	m_Data.ESB.dD = data.ESB.dD;
	m_Data.ESB.dTr = data.ESB.dTr;
	m_Data.ESB.dKv = data.ESB.dKv; //Kvc
	m_Data.ESB.dKvt = data.ESB.dKvt;
	m_Data.ESB.dK1 = data.ESB.dK1;
	m_Data.ESB.nSel = data.ESB.nSel;
	m_Data.ESB.dU = data.ESB.dU;
	m_Data.ESB.dTk = data.ESB.dTk;
	m_Data.ESB.dTq = data.ESB.dTq;

	Data2Dlg();
}

BOOL CAntivibrationSupportGBItem_ESB::SetDlg2Data(T_ASGB_D& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CAntivibrationSupportGBItem_ESB::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data())	return;
	
	if(m_wndEffiDEdit.GetEditValue() <= 0 || m_wndEffiTrEdit.GetEditValue() <= 0 || m_wndMechKvcEdit_ESB.GetEditValue() <= 0
		|| m_wndMechK1Edit_ESB.GetEditValue() <= 0 || m_wndTk.GetEditValue()<=0 )
	{
	  AfxMessageBox(_LS(IDS_CMD_INPUT_VALUE_ERROR)); 	return;
	}

	CChildDialog::OnOK();
}

void CAntivibrationSupportGBItem_ESB::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CChildDialog::OnCancel();
}


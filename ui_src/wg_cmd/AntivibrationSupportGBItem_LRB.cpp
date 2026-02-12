// NLLinkPrtGapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "AntivibrationSupportGBItem_LRB.h"
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
// CAntivibrationSupportGBItem_LRB dialog


CAntivibrationSupportGBItem_LRB::CAntivibrationSupportGBItem_LRB(CWnd* pParent)
	: CChildDialog(CAntivibrationSupportGBItem_LRB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAntivibrationSupportGBItem_LRB)
	//}}AFX_DATA_INIT
}
CAntivibrationSupportGBItem_LRB::~CAntivibrationSupportGBItem_LRB()
{

}

void CAntivibrationSupportGBItem_LRB::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAntivibrationSupportGBItem_LRB)
	DDX_Control(pDX, IDC_CMD_EFFECTIVE_D_EDIT, m_wndEffiDEdit);
	DDX_Control(pDX, IDC_CMD_RUBBER_TR_EDIT, m_wndEffiTrEdit);
	DDX_Control(pDX, IDC_CMD_MECH_KV_EDIT, m_wndMechKvcEdit);
	DDX_Control(pDX, IDC_CMD_MECH_KVT_EDIT, m_wndMechKvtEdit);
	DDX_Control(pDX, IDC_CMD_MECH_K1_EDIT, m_wndMechK1Edit);
	DDX_Control(pDX, IDC_CMD_MECH_KD_EDIT, m_wndMechKdEdit);
	DDX_Control(pDX, IDC_CMD_MECH_OD_EDIT, m_wndMechOdEdit);
	DDX_Control(pDX, IDC_CMD_ADJUST_COEF_TK_EDIT, m_wndTk);
	DDX_Control(pDX, IDC_CMD_ADJUST_COEF_TQ_EDIT, m_wndTq);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_EQUI_RTO_EDIT, m_wndEquiRto_LRB);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_H_STIFF_EDIT, m_wndDgnHEquiStiffEdit_LRB);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_S2_EDIT, m_wndS2);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_S1_EDIT, m_wndS1);

	DDX_Control(pDX, IDC_CMD_EFFECTIVE_D_UNT, m_wndEffiDUnit);
	DDX_Control(pDX, IDC_CMD_RUBBER_TR_UNT, m_wndEffiTrUnit);
	DDX_Control(pDX, IDC_CMD_MECH_KV_UNT, m_wndMechKvcUnit);
	DDX_Control(pDX, IDC_CMD_MECH_KVT_UNT, m_wndMechKvtUnit);
	DDX_Control(pDX, IDC_CMD_MECH_K1_UNT, m_wndMechK1Unit);
	DDX_Control(pDX, IDC_CMD_MECH_KD_UNT, m_wndMechKdUnit);
	DDX_Control(pDX, IDC_CMD_MECH_OD_UNT, m_wndMechQdUnit);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_H_STIFF_UNT, m_wndDgnHEquiStiffUnit_LRB);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAntivibrationSupportGBItem_LRB, CChildDialog)
	//{{AFX_MSG_MAP(CAntivibrationSupportGBItem_LRB)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAntivibrationSupportGBItem_LRB message handlers
BOOL CAntivibrationSupportGBItem_LRB::OnInitDialog() 
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
	m_wndMechKvcUnit.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndMechKvtUnit.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndMechK1Unit.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndMechKdUnit.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndMechQdUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
	m_wndDgnHEquiStiffUnit_LRB.SetUnitType(D_UNITSYS_BASE_SPRING);

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAntivibrationSupportGBItem_LRB::Data2Dlg()
{
	m_wndEffiDEdit.SetEditUnit(m_Data.LRB.dD);
	m_wndEffiTrEdit.SetEditUnit(m_Data.LRB.dTr,3);
	m_wndMechKvcEdit.SetEditUnit(m_Data.LRB.dKv);
	m_wndMechKvtEdit.SetEditUnit(m_Data.LRB.dKvt);
	m_wndMechK1Edit.SetEditUnit(m_Data.LRB.dK1);
	m_wndMechKdEdit.SetEditUnit(m_Data.LRB.dKd);
	m_wndMechOdEdit.SetEditUnit(m_Data.LRB.dOd);
	m_wndTk.SetEditUnit(m_Data.LRB.dTk);
	m_wndTq.SetEditUnit(m_Data.LRB.dTq);
	m_wndEquiRto_LRB.SetEditUnit(m_Data.LRB.dEquiRot);
	m_wndDgnHEquiStiffEdit_LRB.SetEditUnit(m_Data.LRB.dHorEquiStiff);
	m_wndS2.SetEditUnit(m_Data.LRB.dS2);
	m_wndS1.SetEditUnit(m_Data.LRB.dS1);
	EnableDisableItemCtrls(m_Data.LRB.nInputMethod);
}

BOOL CAntivibrationSupportGBItem_LRB::Dlg2Data()
{
	UpdateData();
	m_Data.LRB.dD = m_wndEffiDEdit.GetEditValue();
	m_Data.LRB.dTr = m_wndEffiTrEdit.GetEditValue();
	m_Data.LRB.dKv = m_wndMechKvcEdit.GetEditValue();
	m_Data.LRB.dKvt = m_wndMechKvtEdit.GetEditValue();
	m_Data.LRB.dK1 = m_wndMechK1Edit.GetEditValue();
	m_Data.LRB.dKd = m_wndMechKdEdit.GetEditValue();
	m_Data.LRB.dOd = m_wndMechOdEdit.GetEditValue();
	m_Data.LRB.dTk = m_wndTk.GetEditValue();
	m_Data.LRB.dTq = m_wndTq.GetEditValue();
	m_Data.LRB.dHorEquiStiff = m_wndDgnHEquiStiffEdit_LRB.GetEditValue();
	m_Data.LRB.dEquiRot = m_wndEquiRto_LRB.GetEditValue();
	m_Data.LRB.dS2 = m_wndS2.GetEditValue();
	m_Data.LRB.dS1 = m_wndS1.GetEditValue();
	return TRUE;
}

void CAntivibrationSupportGBItem_LRB::EnableDisableItemCtrls(int nInputMethod)
{
	if (nInputMethod == 1)
	{
		m_wndEffiDEdit.SetReadOnly(TRUE);
		m_wndEffiTrEdit.SetReadOnly(TRUE);
		m_wndMechKvcEdit.SetReadOnly(TRUE);
		m_wndMechKvtEdit.SetReadOnly(FALSE);
		m_wndMechK1Edit.SetReadOnly(TRUE);
		m_wndMechKdEdit.SetReadOnly(TRUE);
		m_wndMechOdEdit.SetReadOnly(TRUE);
		m_wndTk.SetReadOnly(TRUE);
		m_wndTq.SetReadOnly(TRUE);
		m_wndS2.SetReadOnly(TRUE);
		m_wndS1.SetReadOnly(TRUE);
		m_wndEquiRto_LRB.SetReadOnly(TRUE);
		m_wndDgnHEquiStiffEdit_LRB.SetReadOnly(TRUE);
	}
	else
	{
		m_wndEffiDEdit.SetReadOnly(FALSE);
		m_wndEffiTrEdit.SetReadOnly(FALSE);
		m_wndMechKvcEdit.SetReadOnly(FALSE);
		m_wndMechKvtEdit.SetReadOnly(FALSE);
		m_wndMechK1Edit.SetReadOnly(FALSE);
		m_wndMechKdEdit.SetReadOnly(FALSE);
		m_wndMechOdEdit.SetReadOnly(FALSE);
		m_wndTk.SetReadOnly(FALSE);
		m_wndTq.SetReadOnly(FALSE);
		m_wndS2.SetReadOnly(FALSE);
		m_wndS1.SetReadOnly(FALSE);
		m_wndEquiRto_LRB.SetReadOnly(FALSE);
		m_wndDgnHEquiStiffEdit_LRB.SetReadOnly(FALSE);
	}
}

void CAntivibrationSupportGBItem_LRB::SetData2Dlg(T_ASGB_D& data)
{
	m_Data.LRB.nInputMethod = data.LRB.nInputMethod; //
	m_Data.LRB.nCode = data.LRB.nCode;
	m_Data.LRB.nClassify = data.LRB.nClassify;
	m_Data.LRB.nInputType = data.LRB.nInputMethod;
	m_Data.LRB.dD = data.LRB.dD;
	m_Data.LRB.dTr = data.LRB.dTr;
	m_Data.LRB.dKv = data.LRB.dKv; //Kvc
	m_Data.LRB.dKvt = data.LRB.dKvt;
	m_Data.LRB.dK1 = data.LRB.dK1;
	m_Data.LRB.dKd = data.LRB.dKd;
	m_Data.LRB.dOd = data.LRB.dOd;//Qd
	m_Data.LRB.dTk = data.LRB.dTk;
	m_Data.LRB.dTq = data.LRB.dTq;
	m_Data.LRB.dHorEquiStiff = data.LRB.dHorEquiStiff;
	m_Data.LRB.dEquiRot = data.LRB.dEquiRot;
	m_Data.LRB.dS2 = data.LRB.dS2;
	m_Data.LRB.dS1 = data.LRB.dS1;
	Data2Dlg();
}

BOOL CAntivibrationSupportGBItem_LRB::SetDlg2Data(T_ASGB_D& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}
void CAntivibrationSupportGBItem_LRB::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data())	return;
	
	if(m_wndEffiDEdit.GetEditValue() <= 0 || m_wndEffiTrEdit.GetEditValue() <= 0 || m_wndMechKvcEdit.GetEditValue() <= 0 
		|| m_wndMechK1Edit.GetEditValue() <= 0 || m_wndMechKdEdit.GetEditValue()<=0 || m_wndMechOdEdit.GetEditValue() <= 0
		|| m_wndTq.GetEditValue() <= 0 || m_wndTk.GetEditValue() <= 0 || m_wndDgnHEquiStiffEdit_LRB.GetEditValue() <= 0
		|| m_wndEquiRto_LRB.GetEditValue() <= 0 || m_wndS2.GetEditValue() <= 0 || m_wndS1.GetEditValue() <= 0 || m_wndMechKvtEdit.GetEditValue() <= 0)
	{
	  AfxMessageBox(_LS(IDS_CMD_INPUT_VALUE_ERROR)); 	return;
	}

	CChildDialog::OnOK();
}

void CAntivibrationSupportGBItem_LRB::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CChildDialog::OnCancel();
}


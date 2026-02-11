// NLLinkPrtGapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "AntivibrationSupportGBItem_HDR.h"
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
// CAntivibrationSupportGBItem_HDR dialog


CAntivibrationSupportGBItem_HDR::CAntivibrationSupportGBItem_HDR(CWnd* pParent /*=NULL*/)
	: CChildDialog(CAntivibrationSupportGBItem_HDR::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAntivibrationSupportGBItem_HDR)
	//}}AFX_DATA_INIT
}
CAntivibrationSupportGBItem_HDR::~CAntivibrationSupportGBItem_HDR()
{

}

void CAntivibrationSupportGBItem_HDR::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAntivibrationSupportGBItem_HDR)
	DDX_Control(pDX, IDC_CMD_EFFECTIVE_D_EDIT, m_wndEffiDEdit);
	DDX_Control(pDX, IDC_CMD_RUBBER_TR_EDIT, m_wndEffiTrEdit);
	DDX_Control(pDX, IDC_CMD_MECH_KV_EDIT, m_wndMechKvcEdit);
	DDX_Control(pDX, IDC_CMD_MECH_KVT_EDIT, m_wndMechKvtEdit);
	DDX_Control(pDX, IDC_CMD_MECH_K1_EDIT, m_wndMechK1Edit);
	DDX_Control(pDX, IDC_CMD_MECH_KD_EDIT, m_wndMechKdEdit);
	DDX_Control(pDX, IDC_CMD_MECH_OD_EDIT, m_wndMechOdEdit);
	DDX_Control(pDX, IDC_CMD_ADJUST_COEF_TK_EDIT, m_wndTk);
	DDX_Control(pDX, IDC_CMD_ADJUST_COEF_TQ_EDIT, m_wndTq);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_EQUI_RTO_EDIT, m_wndEquiRto_HDR);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_H_STIFF_EDIT, m_wndDgnHEquiStiffEdit_HDR);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_S2_EDIT, m_wndS2);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_S1_EDIT, m_wndS1);

	DDX_Control(pDX, IDC_CMD_EFFECTIVE_D_UNT, m_wndEffiDUnit);
	DDX_Control(pDX, IDC_CMD_RUBBER_TR_UNT, m_wndEffiTrUnit);
	DDX_Control(pDX, IDC_CMD_MECH_KV_UNT, m_wndMechKvcUnit);
	DDX_Control(pDX, IDC_CMD_MECH_KVT_UNT, m_wndMechKvtUnit);
	DDX_Control(pDX, IDC_CMD_MECH_K1_UNT, m_wndMechK1Unit);
	DDX_Control(pDX, IDC_CMD_MECH_KD_UNT, m_wndMechKdUnit);
	DDX_Control(pDX, IDC_CMD_MECH_OD_UNT, m_wndMechQdUnit);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_H_STIFF_UNT, m_wndDgnHEquiStiffUnit_HDR);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAntivibrationSupportGBItem_HDR, CChildDialog)
	//{{AFX_MSG_MAP(CAntivibrationSupportGBItem_HDR)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAntivibrationSupportGBItem_HDR message handlers
BOOL CAntivibrationSupportGBItem_HDR::OnInitDialog() 
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
	m_wndDgnHEquiStiffUnit_HDR.SetUnitType(D_UNITSYS_BASE_SPRING);
	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAntivibrationSupportGBItem_HDR::Data2Dlg()
{
	m_wndEffiDEdit.SetEditUnit(m_Data.HDR.dD);
	m_wndEffiTrEdit.SetEditUnit(m_Data.HDR.dTr,3);
	m_wndMechKvcEdit.SetEditUnit(m_Data.HDR.dKv);
	m_wndMechKvtEdit.SetEditUnit(m_Data.HDR.dKvt);
	m_wndMechK1Edit.SetEditUnit(m_Data.HDR.dK1);
	m_wndMechKdEdit.SetEditUnit(m_Data.HDR.dKd);
	m_wndMechOdEdit.SetEditUnit(m_Data.HDR.dOd);
	m_wndTk.SetEditUnit(m_Data.HDR.dTk);
	m_wndTq.SetEditUnit(m_Data.HDR.dTq);
	m_wndEquiRto_HDR.SetEditUnit(m_Data.HDR.dEquiRot);
	m_wndDgnHEquiStiffEdit_HDR.SetEditUnit(m_Data.HDR.dHorEquiStiff);
	//CString strS2, strS1;
	//strS2.Format(_T("%.2f"), m_Data.HDR.dS2);
	//m_wndS2.SetWindowText(strS2);
	//strS1.Format(_T("%.2f"), m_Data.HDR.dS1);
	//m_wndS2.SetWindowText(strS1);
	m_wndS2.SetEditUnit(m_Data.HDR.dS2);
	m_wndS1.SetEditUnit(m_Data.HDR.dS1);

	EnableDisableItemCtrls(m_Data.HDR.nInputMethod);
}

BOOL CAntivibrationSupportGBItem_HDR::Dlg2Data()
{
	UpdateData();
	m_Data.HDR.dD = m_wndEffiDEdit.GetEditValue();
	m_Data.HDR.dTr = m_wndEffiTrEdit.GetEditValue();
	m_Data.HDR.dKv = m_wndMechKvcEdit.GetEditValue();
	m_Data.HDR.dKvt = m_wndMechKvtEdit.GetEditValue();
	m_Data.HDR.dK1 = m_wndMechK1Edit.GetEditValue();
	m_Data.HDR.dKd = m_wndMechKdEdit.GetEditValue();
	m_Data.HDR.dOd = m_wndMechOdEdit.GetEditValue();
	m_Data.HDR.dTk = m_wndTk.GetEditValue();
	m_Data.HDR.dTq = m_wndTq.GetEditValue();
	m_Data.HDR.dHorEquiStiff = m_wndDgnHEquiStiffEdit_HDR.GetEditValue();
	m_Data.HDR.dEquiRot = m_wndEquiRto_HDR.GetEditValue();
	m_Data.HDR.dS2 = m_wndS2.GetEditValue();
	m_Data.HDR.dS1 = m_wndS1.GetEditValue();
	return TRUE;
}

void CAntivibrationSupportGBItem_HDR::EnableDisableItemCtrls(int nInputMethod)
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
		m_wndEquiRto_HDR.SetReadOnly(TRUE);
		m_wndDgnHEquiStiffEdit_HDR.SetReadOnly(TRUE);
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
		m_wndEquiRto_HDR.SetReadOnly(FALSE);
		m_wndDgnHEquiStiffEdit_HDR.SetReadOnly(FALSE);
	}

}

void CAntivibrationSupportGBItem_HDR::SetData2Dlg(T_ASGB_D& data)
{
	m_Data.HDR.nInputMethod = data.HDR.nInputMethod; //
	m_Data.HDR.nCode = data.HDR.nCode;
	m_Data.HDR.nClassify = data.HDR.nClassify;
	m_Data.HDR.nInputType = data.HDR.nInputMethod;
	m_Data.HDR.dD = data.HDR.dD;
	m_Data.HDR.dTr = data.HDR.dTr;
	m_Data.HDR.dKv = data.HDR.dKv; //Kvc
	m_Data.HDR.dKvt = data.HDR.dKvt;
	m_Data.HDR.dK1 = data.HDR.dK1;
	m_Data.HDR.dKd = data.HDR.dKd;
	m_Data.HDR.dOd = data.HDR.dOd;//Qd
	m_Data.HDR.dTk = data.HDR.dTk;
	m_Data.HDR.dTq = data.HDR.dTq;
	m_Data.HDR.dHorEquiStiff = data.HDR.dHorEquiStiff;
	m_Data.HDR.dEquiRot = data.HDR.dEquiRot;
	m_Data.HDR.dS2 = data.HDR.dS2;

	Data2Dlg();
}

BOOL CAntivibrationSupportGBItem_HDR::SetDlg2Data(T_ASGB_D& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CAntivibrationSupportGBItem_HDR::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data())	return;
	
	if(m_wndEffiDEdit.GetEditValue() <= 0 || m_wndEffiTrEdit.GetEditValue() <= 0 || m_wndMechKvcEdit.GetEditValue() <= 0
		|| m_wndMechK1Edit.GetEditValue() <= 0 || m_wndMechKdEdit.GetEditValue()<=0 || m_wndMechOdEdit.GetEditValue() <= 0
		|| m_wndTq.GetEditValue() <= 0 || m_wndTk.GetEditValue() <= 0 || m_wndDgnHEquiStiffEdit_HDR.GetEditValue() <= 0
		|| m_wndEquiRto_HDR.GetEditValue() <= 0 || m_wndS2.GetEditValue()<= 0 || m_wndS1.GetEditValue() <= 0 || m_wndMechKvtEdit.GetEditValue() <= 0)
	{
	  AfxMessageBox(_LS(IDS_CMD_INPUT_VALUE_ERROR)); 	return;
	}

	CChildDialog::OnOK();
}

void CAntivibrationSupportGBItem_HDR::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CChildDialog::OnCancel();
}


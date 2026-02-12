// NLLinkPrtGapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "AntivibrationSupportGBItem_LNR.h"
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
// CAntivibrationSupportGBItem_LNR dialog


CAntivibrationSupportGBItem_LNR::CAntivibrationSupportGBItem_LNR(CWnd* pParent)
	: CChildDialog(CAntivibrationSupportGBItem_LNR::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAntivibrationSupportGBItem_LNR)
	//}}AFX_DATA_INIT
}
CAntivibrationSupportGBItem_LNR::~CAntivibrationSupportGBItem_LNR()
{

}

void CAntivibrationSupportGBItem_LNR::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAntivibrationSupportGBItem_LNR)
	DDX_Control(pDX, IDC_CMD_EFFECTIVE_D_EDIT, m_wndEffiDEdit);
	DDX_Control(pDX, IDC_CMD_RUBBER_TR_EDIT, m_wndEffiTrEdit);
	DDX_Control(pDX, IDC_CMD_MECH_KV_LNR_EDIT, m_wndMechKvcEdit_LNR);
	DDX_Control(pDX, IDC_CMD_MECH_KVT_LNR_EDIT, m_wndMechKvtEdit_LNR);
	DDX_Control(pDX, IDC_CMD_MECH_KH_LNR_EDIT, m_wndMechKhEdit_LNR);
	DDX_Control(pDX, IDC_CMD_ADJUST_COEF_TK_EDIT, m_wndTk);
	DDX_Control(pDX, IDC_CMD_ADJUST_COEF_TQ_EDIT, m_wndTq);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_S2_EDIT, m_wndS2);
	DDX_Control(pDX, IDC_CMD_DESIGN_DATA_S1_EDIT, m_wndS1);

	DDX_Control(pDX, IDC_CMD_EFFECTIVE_D_UNT, m_wndEffiDUnit);
	DDX_Control(pDX, IDC_CMD_RUBBER_TR_UNT, m_wndEffiTrUnit);
	DDX_Control(pDX, IDC_CMD_MECH_KVC_LNR_UNT, m_wndMechKvcUnit_LNR);
	DDX_Control(pDX, IDC_CMD_MECH_KVT_LNR_UNT, m_wndMechKvtUnit_LNR);
	DDX_Control(pDX, IDC_CMD_MECH_KH_LNR_UNT, m_wndMechKhUnit_LNR);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAntivibrationSupportGBItem_LNR, CChildDialog)
	//{{AFX_MSG_MAP(CAntivibrationSupportGBItem_LNR)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAntivibrationSupportGBItem_LNR message handlers
BOOL CAntivibrationSupportGBItem_LNR::OnInitDialog() 
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
 	m_wndMechKvcUnit_LNR.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_wndMechKvtUnit_LNR.SetUnitType(D_UNITSYS_BASE_SPRING);
 	m_wndMechKhUnit_LNR.SetUnitType(D_UNITSYS_BASE_SPRING);


	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAntivibrationSupportGBItem_LNR::EnableDisableItemCtrls(int nInputMethod)
{
	if (nInputMethod == 1)
	{
		m_wndEffiDEdit.SetReadOnly(TRUE);
		m_wndEffiTrEdit.SetReadOnly(TRUE);
		m_wndMechKvcEdit_LNR.SetReadOnly(TRUE);
		m_wndMechKvtEdit_LNR.SetReadOnly(FALSE);
		m_wndMechKhEdit_LNR.SetReadOnly(TRUE);
		m_wndTk.SetReadOnly(TRUE);
		m_wndS2.SetReadOnly(TRUE);
		m_wndS1.SetReadOnly(TRUE);
	}
	else
	{
		m_wndEffiDEdit.SetReadOnly(FALSE);
		m_wndEffiTrEdit.SetReadOnly(FALSE);
		m_wndMechKvcEdit_LNR.SetReadOnly(FALSE);
		m_wndMechKvtEdit_LNR.SetReadOnly(FALSE);
		m_wndMechKhEdit_LNR.SetReadOnly(FALSE);
		m_wndTk.SetReadOnly(FALSE);
		m_wndS2.SetReadOnly(FALSE);
		m_wndS1.SetReadOnly(FALSE);
	}
	m_wndTq.SetReadOnly(TRUE);
}

void CAntivibrationSupportGBItem_LNR::Data2Dlg()
{
	m_wndEffiDEdit.SetEditUnit(m_Data.LNR.dD);
	m_wndEffiTrEdit.SetEditUnit(m_Data.LNR.dTr,3);
	m_wndMechKvcEdit_LNR.SetEditUnit(m_Data.LNR.dKv);
	m_wndMechKvtEdit_LNR.SetEditUnit(m_Data.LNR.dKvt);
	m_wndMechKhEdit_LNR.SetEditUnit(m_Data.LNR.dKh);
	m_wndTk.SetEditUnit(m_Data.LNR.dTk);
	m_wndTq.SetEditUnit(1.0);
	m_wndS2.SetEditUnit(m_Data.LNR.dS2);
	m_wndS1.SetEditUnit(m_Data.LNR.dS1);
	EnableDisableItemCtrls(m_Data.LNR.nInputMethod);
}

BOOL CAntivibrationSupportGBItem_LNR::Dlg2Data()
{
	UpdateData();
	m_Data.LNR.dD = m_wndEffiDEdit.GetEditValue();
	m_Data.LNR.dTr = m_wndEffiTrEdit.GetEditValue();
	m_Data.LNR.dKv  = m_wndMechKvcEdit_LNR.GetEditValue();
	m_Data.LNR.dKvt = m_wndMechKvtEdit_LNR.GetEditValue();
	m_Data.LNR.dKh = m_wndMechKhEdit_LNR.GetEditValue();
	m_Data.LNR.dTk = m_wndTk.GetEditValue();
	m_Data.LNR.dTq = 1;
	m_Data.LNR.dS2 = m_wndS2.GetEditValue();
	m_Data.LNR.dS1 = m_wndS1.GetEditValue();
	return TRUE;
}

void CAntivibrationSupportGBItem_LNR::SetData2Dlg(T_ASGB_D& data)
{
	m_Data.LNR.nInputMethod = data.LNR.nInputMethod;
	m_Data.LNR.nCode = data.LNR.nCode;
	m_Data.LNR.nClassify = data.LNR.nClassify;
	m_Data.LNR.nInputType = data.LNR.nInputType;
	m_Data.LNR.dD = data.LNR.dD;
	m_Data.LNR.dTr = data.LNR.dTr;
	m_Data.LNR.dKv = data.LNR.dKv; //Kvc
	m_Data.LNR.dKvt = data.LNR.dKvt;
	m_Data.LNR.dKh = data.LNR.dKh;
	m_Data.LNR.dTk = data.LNR.dTk;
	m_Data.LNR.dTq = data.LNR.dTq;
	m_Data.LNR.dS2 = data.LNR.dS2;
	m_Data.LNR.dS1 = data.LNR.dS1;
	Data2Dlg();
}

BOOL CAntivibrationSupportGBItem_LNR::SetDlg2Data(T_ASGB_D& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CAntivibrationSupportGBItem_LNR::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data())	return;
	
	if(m_wndEffiDEdit.GetEditValue() <= 0 || m_wndEffiTrEdit.GetEditValue() <= 0 || m_wndMechKvcEdit_LNR.GetEditValue() <= 0
		|| m_wndMechKhEdit_LNR.GetEditValue() <= 0 || m_wndTk.GetEditValue()<=0 || m_wndS2.GetEditValue() <= 0 
		|| m_wndS1.GetEditValue() <= 0 || m_wndMechKvtEdit_LNR.GetEditValue() <= 0)
	{
	  AfxMessageBox(_LS(IDS_CMD_INPUT_VALUE_ERROR)); 	return;
	}

	CChildDialog::OnOK();
}

void CAntivibrationSupportGBItem_LNR::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CChildDialog::OnCancel();
}


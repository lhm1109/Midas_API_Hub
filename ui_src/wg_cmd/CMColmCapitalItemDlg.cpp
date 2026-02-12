// CMColmCapitalItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMColmCapitalItemDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMColmCapitalItemDlg dialog

//BMP_TODO IDC_WG_CMD_STATIC8

CCMColmCapitalItemDlg::CCMColmCapitalItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMColmCapitalItemDlg::IDD, pParent)
{
	//m_pDoc = pDoc;
	m_bModify = FALSE;
	m_Key = 0;
	m_Data.Initialize();
	m_nLocation=0;
	//{{AFX_DATA_INIT(CCMColmCapitalItemDlg)
	//}}AFX_DATA_INIT
}


void CCMColmCapitalItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMColmCapitalItemDlg)
	DDX_Control(pDX, IDC_CMD_COLM_CAPITAL_ITEM_NAME_EDT, m_edtName);
	DDX_Control(pDX, IDC_CMD_COLM_CAPITAL_ITEM_DESC_EDT, m_edtDesc);
	DDX_Control(pDX, IDC_CMD_COLM_CAPITAL_ITEM_B1_EDT, m_edtB1);
	DDX_Control(pDX, IDC_CMD_COLM_CAPITAL_ITEM_B2_EDT, m_edtB2);
	DDX_Control(pDX, IDC_CMD_COLM_CAPITAL_ITEM_H1_EDT, m_edtH1);
	DDX_Control(pDX, IDC_CMD_COLM_CAPITAL_ITEM_H2_EDT, m_edtH2);
	DDX_Control(pDX, IDC_CMD_COLM_CAPITAL_ITEM_ANG_EDT, m_edtAng);
	DDX_Control(pDX, IDC_CMD_COLM_CAPITAL_ITEM_B1_UNT, m_untB1);
	DDX_Control(pDX, IDC_CMD_COLM_CAPITAL_ITEM_B2_UNT, m_untB2);
	DDX_Control(pDX, IDC_CMD_COLM_CAPITAL_ITEM_H1_UNT, m_untH1);
	DDX_Control(pDX, IDC_CMD_COLM_CAPITAL_ITEM_H2_UNT, m_untH2);
	DDX_Control(pDX, IDC_CMD_DROP_COLM_CAPITAL_ANG_UNT, m_untAng);
	DDX_Control(pDX, IDC_CMD_COLM_CAPITAL_ITEM_RIGID_CHK, m_chkAutoRigid);
	DDX_Radio  (pDX, IDC_CMD_COLM_CAPITAL_ITEM_LOC_TOP_RDO, m_nLocation);
	DDX_Control(pDX, IDC_WG_CMD_STATIC8, m_wndDropPanel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMColmCapitalItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMColmCapitalItemDlg)
	ON_BN_CLICKED(IDC_CMD_COLM_CAPITAL_ITEM_APPLY_BTN , OnApply      )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMColmCapitalItemDlg message handlers
BOOL CCMColmCapitalItemDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();

	InitUnit();
	Data2Dlg();

	m_wndDropPanel.SetImage(_T("SVG\\Illustration\\Dialog\\Drop_Panel.svg"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMColmCapitalItemDlg::InitUnit()
{
	m_edtB1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untB1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtB2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untB2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtH1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untH1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtH2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untH2.SetUnitType(D_UNITSYS_BASE_LENGTH);
//   m_edtAng.SetUnitType(D_UNITSYS_BASE_DEGREE);
//   m_untAng.SetUnitType(D_UNITSYS_BASE_DEGREE);
}

void CCMColmCapitalItemDlg::SetData(T_CCPT_K Key, T_CCPT_D &Data)
{
	m_Key = Key;
	m_Data = Data;
}

BOOL CCMColmCapitalItemDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_edtName.GetWindowText(m_Data.strName);
	m_edtDesc.GetWindowText(m_Data.strDecs);

	m_Data.dB1 = m_edtB1.GetEditValue();
	m_Data.dB2 = m_edtB2.GetEditValue();
	m_Data.dH1 = m_edtH1.GetEditValue();
	m_Data.dH2 = m_edtH2.GetEditValue();
//   m_Data.dAngle = m_edtAng.GetEditValue();

	m_Data.bAutoRigid = m_chkAutoRigid.GetCheck() ? TRUE : FALSE;
	m_Data.nLocation = m_nLocation;

	return TRUE;
}
BOOL CCMColmCapitalItemDlg::Data2Dlg()
{
	m_edtName.SetWindowText(m_Data.strName);
	m_edtDesc.SetWindowText(m_Data.strDecs);

	m_edtB1.SetEditUnit(m_Data.dB1);
	m_edtB2.SetEditUnit(m_Data.dB2);
	m_edtH1.SetEditUnit(m_Data.dH1);
	m_edtH2.SetEditUnit(m_Data.dH2);
//   m_edtAng.SetEditUnit(m_Data.dAngle);

	m_chkAutoRigid.SetCheck(m_Data.bAutoRigid);
	m_nLocation = m_Data.nLocation;
	UpdateData(FALSE);

	return TRUE;
}


void CCMColmCapitalItemDlg::OnOK() 
{
	if (!AddToDB())  return;

	CDialogMove::OnOK();  
}

void CCMColmCapitalItemDlg::OnApply()
{
	AddToDB();
}
BOOL CCMColmCapitalItemDlg::AddToDB()
{
	if (!Dlg2Data()) return FALSE;
	// if (!ValidData()) return;

	BOOL bSuccess = FALSE;
	if (m_bModify)  bSuccess = m_pDoc->m_pDataCtrl->ModifyCcpt(m_Key, m_Key, m_Data);  
	else            bSuccess = m_pDoc->m_pDataCtrl->AddCcpt(m_Data);

	return bSuccess;
}
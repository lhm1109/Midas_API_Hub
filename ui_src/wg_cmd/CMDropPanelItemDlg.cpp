// CMDropPanelItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMDropPanelItemDlg.h"

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
// CMDropPanelItemDlg dialog

//BMP_TODO IDC_WG_CMD_STATIC7

CCMDropPanelItemDlg::CCMDropPanelItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMDropPanelItemDlg::IDD, pParent)
{
	//m_pDoc = pDoc;
	m_bModify = FALSE;
	m_Key = 0;
	m_Data.Initialize();
	//{{AFX_DATA_INIT(CCMDropPanelItemDlg)
	m_aCtrlLocation.RemoveAll();
	m_aCtrlLocation.Add(IDC_CMD_DROP_PANEL_ITEM_LOC_TOP_RDO);
	m_aCtrlLocation.Add(IDC_CMD_DROP_PANEL_ITEM_LOC_BOT_RDO);
	m_aCtrlLocation.Add(IDC_CMD_DROP_PANEL_ITEM_LOC_BOTH_RDO);
	//}}AFX_DATA_INIT
}


void CCMDropPanelItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMDropPanelItemDlg)
	DDX_Control(pDX, IDC_CMD_DROP_PANEL_ITEM_NAME_EDT, m_edtName);
	DDX_Control(pDX, IDC_CMD_DROP_PANEL_ITEM_DESC_EDT, m_edtDesc);
	DDX_Control(pDX, IDC_CMD_DROP_PANEL_ITEM_B1_EDT, m_edtB1);
	DDX_Control(pDX, IDC_CMD_DROP_PANEL_ITEM_B2_EDT, m_edtB2);
	DDX_Control(pDX, IDC_CMD_DROP_PANEL_ITEM_H1_EDT, m_edtH1);
	DDX_Control(pDX, IDC_CMD_DROP_PANEL_ITEM_H2_EDT, m_edtH2);
	DDX_Control(pDX, IDC_CMD_DROP_PANEL_ITEM_ANG_EDT, m_edtAngle);
	DDX_Control(pDX, IDC_CMD_DROP_PANEL_ITEM_THIK_EDT, m_edtThik);
	DDX_Control(pDX, IDC_CMD_DROP_PANEL_ITEM_B1_UNT, m_untB1);
	DDX_Control(pDX, IDC_CMD_DROP_PANEL_ITEM_B2_UNT, m_untB2);
	DDX_Control(pDX, IDC_CMD_DROP_PANEL_ITEM_H1_UNT, m_untH1);
	DDX_Control(pDX, IDC_CMD_DROP_PANEL_ITEM_H2_UNT, m_untH2);
	DDX_Control(pDX, IDC_CMD_DROP_PANEL_ITEM_ANG_UNT, m_untAngle);
	DDX_Control(pDX, IDC_CMD_DROP_PANEL_ITEM_THIK_UNT, m_untThik);
	DDX_Control(pDX, IDC_CMD_DROP_PANEL_ITEM_ANG_CHK, m_chkAng);
	DDX_Control(pDX, IDC_CMD_DROP_PANEL_ITEM_THIK_CHK, m_chkThick);
	DDX_Control(pDX, IDC_WG_CMD_STATIC7, m_wndDropPanel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMDropPanelItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMDropPanelItemDlg)
	ON_BN_CLICKED(IDC_CMD_DROP_PANEL_ITEM_APPLY_BTN , OnApply      )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMDropPanelItemDlg message handlers
BOOL CCMDropPanelItemDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();

	InitUnit();
	Data2Dlg();

	m_wndDropPanel.SetImage(_T("SVG\\Illustration\\Dialog\\Drop_Panel.svg"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMDropPanelItemDlg::InitUnit()
{
	m_edtB1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untB1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtB2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untB2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtH1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untH1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtH2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untH2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_untAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_edtThik.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untThik.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMDropPanelItemDlg::SetData(T_DPAN_K Key, T_DPAN_D &Data)
{
	m_Key = Key;
	m_Data = Data;
}

BOOL CCMDropPanelItemDlg::Dlg2Data()
{
	m_edtName.GetWindowText(m_Data.strName);
	m_edtDesc.GetWindowText(m_Data.strDecs);

	m_Data.dB1 = m_edtB1.GetEditValue();
	m_Data.dB2 = m_edtB2.GetEditValue();
	m_Data.dH1 = m_edtH1.GetEditValue();
	m_Data.dH2 = m_edtH2.GetEditValue();
	m_Data.dAngle = m_edtAngle.GetEditValue();
	m_Data.dThikness = m_edtThik.GetEditValue();

	m_Data.bThik = m_chkThick.GetCheck() ? TRUE : FALSE;
	m_Data.bAngle = m_chkAng.GetCheck()? TRUE : FALSE;

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlLocation, m_Data.nLocation);

	return TRUE;
}
BOOL CCMDropPanelItemDlg::Data2Dlg()
{
	m_edtName.SetWindowText(m_Data.strName);
	m_edtDesc.SetWindowText(m_Data.strDecs);

	m_edtB1.SetEditUnit(m_Data.dB1);
	m_edtB2.SetEditUnit(m_Data.dB2);
	m_edtH1.SetEditUnit(m_Data.dH1);
	m_edtH2.SetEditUnit(m_Data.dH2);
	m_edtAngle.SetEditUnit(m_Data.dAngle);
	m_edtThik.SetEditUnit(m_Data.dThikness);

	m_chkThick.SetCheck(m_Data.bThik);
	m_chkAng.SetCheck(m_Data.bAngle);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlLocation, m_Data.nLocation);

	return TRUE;
}

void CCMDropPanelItemDlg::OnOK() 
{
	if (!AddToDB())  return;

	CDialogMove::OnOK();  
}

void CCMDropPanelItemDlg::OnApply()
{
	AddToDB();
}
BOOL CCMDropPanelItemDlg::AddToDB()
{
	if (!Dlg2Data()) return FALSE;
	// if (!ValidData()) return;

	BOOL bSuccess = FALSE;
	if (m_bModify)  bSuccess = m_pDoc->m_pDataCtrl->ModifyDpan(m_Key, m_Key, m_Data);  
	else            bSuccess = m_pDoc->m_pDataCtrl->AddDpan(m_Data);

	return bSuccess;
}

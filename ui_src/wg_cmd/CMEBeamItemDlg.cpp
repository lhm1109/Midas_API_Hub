// CMEBeamItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMEBeamItemDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMEBeamItemDlg dialog


CCMEBeamItemDlg::CCMEBeamItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMEBeamItemDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_bModify = FALSE;
	//{{AFX_DATA_INIT(CCMEBeamItemDlg)
	//}}AFX_DATA_INIT
}


void CCMEBeamItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMEBeamItemDlg)
	DDX_Control(pDX, IDC_CMD_EBEAM_ITEM_NAME_EDT, m_edtName);
	DDX_Control(pDX, IDC_CMD_EBEAM_ITEM_DESC_EDT, m_edtDesc);
	DDX_Control(pDX, IDC_CMD_EBEAM_ITEM_I_WTHL_EDT, m_edtW1);
	DDX_Control(pDX, IDC_CMD_EBEAM_ITEM_I_WTHR_EDT, m_edtW2);
	DDX_Control(pDX, IDC_CMD_EBEAM_ITEM_J_WTHL_EDT, m_edtW3);
	DDX_Control(pDX, IDC_CMD_EBEAM_ITEM_J_WTHR_EDT, m_edtW4);
	DDX_Control(pDX, IDC_CMD_EBEAM_ITEM_I_WTHL_UNT, m_untW1);
	DDX_Control(pDX, IDC_CMD_EBEAM_ITEM_I_WTHR_UNT, m_untW2);
	DDX_Control(pDX, IDC_CMD_EBEAM_ITEM_J_WTHL_UNT, m_untW3);
	DDX_Control(pDX, IDC_CMD_EBEAM_ITEM_J_WTHR_UNT, m_untW4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMEBeamItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMEBeamItemDlg)
	ON_BN_CLICKED(IDC_CMD_EBEAM_ITEM_CALC_I_BTN, OnEBeamCalc_I)
	ON_BN_CLICKED(IDC_CMD_EBEAM_ITEM_CALC_J_BTN, OnEBeamCalc_J)
	ON_BN_CLICKED(IDC_CMD_EBEAM_ITEM_APPLY_BTN , OnApply      )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMEBeamItemDlg message handlers
BOOL CCMEBeamItemDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	InitUnit();
	if(m_bModify) Data2Dlg();
	m_CalcIDlg.SetInitPos(D_INIT_POS_RT, 0, 50);
	m_CalcJDlg.SetInitPos(D_INIT_POS_RT, 0, 50);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMEBeamItemDlg::OnEBeamCalc_I()
{
	if(m_CalcIDlg.DoModal()==IDOK)
	{
		double dWidth = m_CalcIDlg.GetCalculatedWidth();
		m_edtW1.SetEditUnit(dWidth/2.0);
		m_edtW2.SetEditUnit(dWidth/2.0);
	}
}

void CCMEBeamItemDlg::OnEBeamCalc_J()
{
	if(m_CalcJDlg.DoModal()==IDOK)
	{
		double dWidth = m_CalcJDlg.GetCalculatedWidth();
		m_edtW3.SetEditUnit(dWidth/2.0);
		m_edtW4.SetEditUnit(dWidth/2.0);
	}
}

void CCMEBeamItemDlg::OnOK()
{
	if(!Apply()) return;
	CDialogMove::OnOK();
}

void CCMEBeamItemDlg::OnApply()
{
	Apply();
}

BOOL CCMEBeamItemDlg::Apply()
{
	UpdateData(TRUE);
	
	m_Data.Initialize();
	m_edtName.GetWindowText(m_Data.strName);
	m_edtDesc.GetWindowText(m_Data.strDesc);
	m_Data.adWidth[0] = m_edtW1.GetEditValue();
	m_Data.adWidth[1] = m_edtW2.GetEditValue();
	m_Data.adWidth[2] = m_edtW3.GetEditValue();
	m_Data.adWidth[3] = m_edtW4.GetEditValue();

	BOOL bSuccess;
	if(m_bModify)
	{
		bSuccess = m_pDoc->m_pDataCtrl->ModifyPreb(m_Key, m_Key, m_Data);
	}
	else
	{
		bSuccess= m_pDoc->m_pDataCtrl->AddPreb(m_Data);
	}

	return bSuccess;
}

void CCMEBeamItemDlg::InitUnit()
{
	m_edtW1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untW1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtW2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untW2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtW3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untW3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtW4.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untW4.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMEBeamItemDlg::SetModifyData(T_PREB_K Key)
{
	m_bModify = TRUE;
	m_Key = Key;
}

void CCMEBeamItemDlg::Data2Dlg()
{
	m_pDoc->m_pAttrCtrl2->GetPreb(m_Key, m_Data);

	m_edtName.SetWindowText(m_Data.strName);
	m_edtDesc.SetWindowText(m_Data.strDesc);
	m_edtW1.SetEditUnit(m_Data.adWidth[0]);
	m_edtW2.SetEditUnit(m_Data.adWidth[1]);
	m_edtW3.SetEditUnit(m_Data.adWidth[2]);
	m_edtW4.SetEditUnit(m_Data.adWidth[3]);
}
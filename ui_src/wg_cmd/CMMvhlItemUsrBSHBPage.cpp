// CMMvhlItemUsrBSHBPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUsrBSHBPage.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\UnitCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CInternationalDlg
/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrBSHBPage dialog


CCMMvhlItemUsrBSHBPage::CCMMvhlItemUsrBSHBPage(T_MVHL_D* pData, CWnd* pParent /*=NULL*/)
	: CDialog(CCMMvhlItemUsrBSHBPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemUsrBSHBPage)
	//}}AFX_DATA_INIT
	m_pData = NULL;
	m_pData = pData;
}


void CCMMvhlItemUsrBSHBPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemUsrBSHBPage)
	DDX_Control(pDX, IDC_CMD_MVHL_BS_UNIT_NUM_EDIT, m_edtNumUnit);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_PB_EDIT, m_edtBSPb);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D1_EDIT, m_edtBSD1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D2_EDIT, m_edtBSD2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D_EDIT,  m_edtBSD);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D3_EDIT, m_edtBSD3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D4_EDIT, m_edtBSD4);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D5_EDIT, m_edtBSD5);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D6_EDIT, m_edtBSD6);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_PB_UNIT, m_untBSPb);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D1_UNIT, m_untBSD1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D2_UNIT, m_untBSD2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D_UNIT,  m_untBSD);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D3_UNIT, m_untBSD3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D4_UNIT, m_untBSD4);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D5_UNIT, m_untBSD5);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D6_UNIT, m_untBSD6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvhlItemUsrBSHBPage, CDialog)
	//{{AFX_MSG_MAP(CCMMvhlItemUsrBSHBPage)
	ON_EN_CHANGE (IDC_CMD_MVHL_BS_D2_EDIT, OnChangeLoadDistEdit)
	ON_EN_CHANGE (IDC_CMD_MVHL_BS_D_EDIT,  OnChangeLoadDistEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvhlItemUsrBSHBPage::SetData2Dlg()
{
	m_edtBSPb.SetEditUnit(m_pData->dPb);
	m_edtBSD1.SetEditUnit(m_pData->dD1);
	m_edtBSD2.SetEditUnit(m_pData->dD2);
	m_edtBSD .SetEditUnit(m_pData->dDelta);
	m_edtBSD3.SetEditUnit(m_pData->dD2 + m_pData->dDelta);
	m_edtBSD4.SetEditUnit(m_pData->dD2 + m_pData->dDelta * 2.);
	m_edtBSD5.SetEditUnit(m_pData->dD2 + m_pData->dDelta * 3.);
	m_edtBSD6.SetEditUnit(m_pData->dD2 + m_pData->dDelta * 4.);

	m_edtNumUnit.SetEditUnit(m_pData->dUnitNum);

	UpdateData(FALSE);
}

BOOL CCMMvhlItemUsrBSHBPage::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->dPb            = m_edtBSPb.GetEditValue();
	m_pData->dD1            = m_edtBSD1.GetEditValue();
	m_pData->dD2            = m_edtBSD2.GetEditValue();
	m_pData->dDelta         = m_edtBSD.GetEditValue();

	m_pData->dUnitNum = m_edtNumUnit.GetEditValue();

	m_pData->nLoadType = 0;

	return TRUE;
}

void CCMMvhlItemUsrBSHBPage::InitUnit()
{
	m_untBSPb.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPb);
	m_untBSD1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD3.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD4.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD5.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD6.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);

	m_edtNumUnit.SetUnitType(D_UNITSYS_NONE);
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrBSHBPage message handlers
BOOL CCMMvhlItemUsrBSHBPage::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitUnit();

	SetData2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemUsrBSHBPage::OnChangeLoadDistEdit()
{
	UpdateData(TRUE);

	double dBSD2 = m_edtBSD2.GetEditValue();
	double dBSD  = m_edtBSD .GetEditValue();
	
	m_edtBSD3.SetEditUnit(dBSD2 + dBSD);
	m_edtBSD4.SetEditUnit(dBSD2 + dBSD * 2.);
	m_edtBSD5.SetEditUnit(dBSD2 + dBSD * 3.);
	m_edtBSD6.SetEditUnit(dBSD2 + dBSD * 4.);

	UpdateData(FALSE);
}

BOOL CCMMvhlItemUsrBSHBPage::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
		pMsg->wParam=9;
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE)
		pMsg->wParam=VK_TAB;

	return CInternationalDlg::PreTranslateMessage(pMsg);
}

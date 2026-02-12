// CMMvhlItemStdBSBS5400Page.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdBSBS5400Page.h"
#include "CMMvhlItemStdBSDlg.h"
#include "CMMvhlSItemUsrLaneFactor.h"

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
// CCMMvhlItemStdBSBS5400Page dialog

CCMMvhlItemStdBSBS5400Page::CCMMvhlItemStdBSBS5400Page(T_MVHL_D* pData, CWnd* pParent /*=NULL*/)
	: CDialog(CCMMvhlItemStdBSBS5400Page::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemStdBSBS5400Page)
	m_pParent = (CCMMvhlItemStdBSDlg*) pParent;

	m_PatchD.Initialize();

	//}}AFX_DATA_INIT

	m_pData = NULL;
	m_pData = pData;
}

void CCMMvhlItemStdBSBS5400Page::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdBSBS5400Page)
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W1_EDIT, m_edtW1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W2_EDIT, m_edtW2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W2_EXP_STATIC, m_edtW2_exp);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W3_EDIT, m_edtW3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L1_EDIT, m_edtL1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L2_EDIT, m_edtL2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_PA_EDIT, m_edtPa);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_PB_EDIT, m_edtPb);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_UNIT_NUM_EDIT, m_edtNumUnit);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D1_EDIT, m_edtD1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D2_EDIT, m_edtD2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D3_EDIT, m_edtD3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D4_EDIT, m_edtD4);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D5_EDIT, m_edtD5);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D6_EDIT, m_edtD6);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D_EDIT, m_edtd);

	DDX_Check  (pDX, IDC_CMD_MVHL_PATCH_CHK, m_PatchD.bPatchLoad);

	DDX_Control(pDX, IDC_CMD_MVHL_BS_W1_UNIT, m_untW1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W2_UNIT, m_untW2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W3_UNIT, m_untW3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L1_UNIT, m_untL1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L2_UNIT, m_untL2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_PA_UNIT, m_untPa);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_PB_UNIT, m_untPb);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D1_UNIT, m_untD1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D2_UNIT, m_untD2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D3_UNIT, m_untD3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D4_UNIT, m_untD4);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D5_UNIT, m_untD5);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D6_UNIT, m_untD6);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D_UNIT, m_untd);
	//}}AFX_DATA_MAP
}

BOOL CCMMvhlItemStdBSBS5400Page::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == 13)
		pMsg->wParam = 9;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		pMsg->wParam = VK_TAB;

	return CInternationalDlg::PreTranslateMessage(pMsg);
}

void CCMMvhlItemStdBSBS5400Page::SetData2Dlg()
{
	m_edtW1.SetEditUnit(m_pData->dW1);
	m_edtW2.SetEditUnit(m_pData->dW2);
	m_edtW3.SetEditUnit(m_pData->dW3);
	m_edtL1.SetEditUnit(m_pData->dLoadedLength);
	m_edtL2.SetEditUnit(m_pData->dLoadedLength);
	m_edtPa.SetEditUnit(m_pData->dPa);
	m_edtPb.SetEditUnit(m_pData->dPb);
	m_edtNumUnit.SetEditUnit(m_pData->dUnitNum);

	double dD3 = m_pData->dD2 + m_pData->dDelta;
	double dD4 = m_pData->dD2 + m_pData->dDelta * 2.0;
	double dD5 = m_pData->dD2 + m_pData->dDelta * 3.0;
	double dD6 = m_pData->dD2 + m_pData->dDelta * 4.0;

	m_edtD1.SetEditUnit(m_pData->dD1);
	m_edtD2.SetEditUnit(m_pData->dD2);
	m_edtD3.SetEditUnit(dD3);
	m_edtD4.SetEditUnit(dD4);
	m_edtD5.SetEditUnit(dD5);
	m_edtD6.SetEditUnit(dD6);
	m_edtd.SetEditUnit(m_pData->dDelta);

	double dExp2 = 0.475;
	m_edtW2_exp.SetEditUnit(dExp2);

	m_PatchD = m_pData->PatchLoad;

	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdBSBS5400Page::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->dW1 = m_edtW1.GetEditValue();
	m_pData->dW2 = m_edtW2.GetEditValue();
	m_pData->dW3 = m_edtW3.GetEditValue();
	m_pData->dLoadedLength = m_edtL1.GetEditValue();
	m_pData->dLoadedLength2 = m_edtL2.GetEditValue();
	m_pData->dPa = m_edtPa.GetEditValue();
	m_pData->dPb = m_edtPb.GetEditValue();
	m_pData->dUnitNum = m_edtNumUnit.GetEditValue();
	m_pData->dD1 = m_edtD1.GetEditValue();
	m_pData->dD2 = m_edtD2.GetEditValue();
	m_pData->dDelta = m_edtd.GetEditValue();

	m_pData->dExp2 = m_edtW2_exp.GetEditValue();

	if (m_PatchD.bPatchLoad)
	{
		m_pData->PatchLoad = m_PatchD;
	}
	else
	{
		m_pData->PatchLoad.Initialize();
	}

	return TRUE;
}

void CCMMvhlItemStdBSBS5400Page::InitUnit()
{
	m_untW1.SetTextFmt(_LSX(kN / m));
	m_untW1.SetUnitType(D_UNITSYS_NONE);
	m_untW2.SetTextFmt(_LSX(kN / m));
	m_untW2.SetUnitType(D_UNITSYS_NONE);
	m_untW3.SetTextFmt(_LSX(kN / m));
	m_untW3.SetUnitType(D_UNITSYS_NONE);
	m_untL1.SetTextFmt(_LSX(m));
	m_untL1.SetUnitType(D_UNITSYS_NONE);
	m_untL2.SetTextFmt(_LSX(m));
	m_untL2.SetUnitType(D_UNITSYS_NONE);
	
	m_untPa.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPa);
	m_untPb.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPb);

	m_untD1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD1);
	m_untD2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD2);
	m_untD3.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD2);
	m_untD4.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD2);
	m_untD5.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD2);
	m_untD6.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD2);
	m_untd.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);

	m_edtW2_exp.SetUnitType(D_UNITSYS_NONE);
}

BEGIN_MESSAGE_MAP(CCMMvhlItemStdBSBS5400Page, CDialog)
	//{{AFX_MSG_MAP(CCMMvhlItemStdBSBS5400Page)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBSBS5400Page message handlers
BOOL CCMMvhlItemStdBSBS5400Page::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitUnit();

	SetData2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
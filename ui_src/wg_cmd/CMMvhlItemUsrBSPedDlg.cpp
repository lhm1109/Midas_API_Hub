// CMMvhlItemUsrBSPedDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUsrBSPedDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CInternationalDlg
/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrBSPedDlg dialog


CCMMvhlItemUsrBSPedDlg::CCMMvhlItemUsrBSPedDlg(T_MVHL_D* pData, CWnd* pParent /*=NULL*/)
	: CDialog(CCMMvhlItemUsrBSPedDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemUsrBSPedDlg)

	//}}AFX_DATA_INIT

	m_pData = NULL;
	m_pData = pData;
}


void CCMMvhlItemUsrBSPedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemUsrBSPedDlg)
	DDX_Control(pDX, IDC_CMD_MVHL_PED_W1_EDIT, m_edtPedW1);
	DDX_Control(pDX, IDC_CMD_MVHL_PED_W1_UNIT, m_untPedW1);
	DDX_Control(pDX, IDC_CMD_MVHL_PED_W2_EDIT, m_edtPedW2);
	DDX_Control(pDX, IDC_CMD_MVHL_PED_W2_UNIT, m_untPedW2);
	DDX_Control(pDX, IDC_CMD_MVHL_PED_L1_EDIT, m_edtPedL1);
	DDX_Control(pDX, IDC_CMD_MVHL_PED_L1_UNIT, m_untPedL1);
	DDX_Control(pDX, IDC_CMD_MVHL_PED_L2_EDIT, m_edtPedL2);
	DDX_Control(pDX, IDC_CMD_MVHL_PED_L2_UNIT, m_untPedL2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvhlItemUsrBSPedDlg, CDialog)
	//{{AFX_MSG_MAP(CCMMvhlItemUsrBSPedDlg)
	ON_EN_CHANGE(IDC_CMD_MVHL_PED_W1_EDIT, OnChangePedW1Edit)
	ON_EN_CHANGE(IDC_CMD_MVHL_PED_L1_EDIT, OnChangePedL1Edit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvhlItemUsrBSPedDlg::SetData2Dlg()
{
	m_edtPedW1.SetEditUnit(m_pData->dW1);
	CString csVal;
	m_edtPedW1.GetWindowText(csVal);
	csVal = _T("k*")+csVal;
	m_edtPedW2.SetWindowText(csVal);
	m_edtPedL1.SetEditUnit(m_pData->dLoadedLength);
	m_edtPedL1.GetWindowText(csVal);
	m_edtPedL2.SetWindowText(csVal);
}

BOOL CCMMvhlItemUsrBSPedDlg::SetDlg2Data()
{
	m_pData->dW1            = m_edtPedW1.GetEditValue();
	m_pData->dLoadedLength  = m_edtPedL1.GetEditValue();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrBSPedDlg message handlers
BOOL CCMMvhlItemUsrBSPedDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// 단위 변환
	/*
	m_edtPedW1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dW1);
	m_untPedW1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dW1);
	//m_edtPedW2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dW2);
	m_untPedW2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dW2);
	m_edtPedL1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dLoadedLength);
	m_untPedL1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dLoadedLength);
	//m_edtPedL2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dLoadedLength);
	m_untPedL2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dLoadedLength);
	m_edtPedL2.SetReadOnly(TRUE);
	*/

	// 단위 고정한다.
	m_edtPedW1.SetUnitType(D_UNITSYS_NONE);
	m_untPedW1.SetTextFmt(_LSX(kN/m)+CUnitCtrl::m_str2);
	m_untPedW1.SetUnitType(D_UNITSYS_NONE);

	m_untPedW2.SetTextFmt(_LSX(kN/m)+CUnitCtrl::m_str2);
	m_untPedW2.SetUnitType(D_UNITSYS_NONE);

	m_edtPedL1.SetUnitType(D_UNITSYS_NONE);
	m_untPedL1.SetTextFmt(_LSX(m));
	m_untPedL1.SetUnitType(D_UNITSYS_NONE);

	m_untPedL2.SetTextFmt(_LSX(m));
	m_untPedL2.SetUnitType(D_UNITSYS_NONE);
	m_edtPedL2.SetReadOnly(TRUE);

	SetData2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCMMvhlItemUsrBSPedDlg::OnChangePedW1Edit()
{
	CString csVal;
	m_edtPedW1.GetWindowText(csVal);
	csVal = _T("k*")+csVal;
	m_edtPedW2.SetWindowText(csVal);
}

void CCMMvhlItemUsrBSPedDlg::OnChangePedL1Edit()
{
	CString csVal;
	m_edtPedL1.GetWindowText(csVal);
	m_edtPedL2.SetWindowText(csVal);
}

BOOL CCMMvhlItemUsrBSPedDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
		pMsg->wParam=9;
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE)
		pMsg->wParam=VK_TAB;

	return CInternationalDlg::PreTranslateMessage(pMsg);
}

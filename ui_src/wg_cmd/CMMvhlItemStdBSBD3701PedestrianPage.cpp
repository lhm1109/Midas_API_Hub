// CMMvhlItemStdBSBD3701PedestrianPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdBSBD3701PedestrianPage.h"
#include "CMMvhlItemStdBSDlg.h"

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
// CCMMvhlItemStdBSBD3701PedestrianPage dialog

CCMMvhlItemStdBSBD3701PedestrianPage::CCMMvhlItemStdBSBD3701PedestrianPage(T_MVHL_D* pData, CWnd* pParent /*=NULL*/)
	: CDialog(CCMMvhlItemStdBSBD3701PedestrianPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemStdBSBD3701PedestrianPage)
	m_pParent = (CCMMvhlItemStdBSDlg*) pParent;

	//}}AFX_DATA_INIT

	m_pData = NULL;
	m_pData = pData;
}

void CCMMvhlItemStdBSBD3701PedestrianPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdBSBD3701PedestrianPage)
	DDX_Control(pDX, IDC_CMD_MVHL_PED_W1_EDIT, m_edtW1);
	DDX_Control(pDX, IDC_CMD_MVHL_PED_W2_EDIT, m_edtW2);
	DDX_Control(pDX, IDC_CMD_MVHL_PED_L1_EDIT, m_edtL1);
	DDX_Control(pDX, IDC_CMD_MVHL_PED_L2_EDIT, m_edtL2);

	DDX_Control(pDX, IDC_CMD_MVHL_PED_W1_UNIT, m_untW1);
	DDX_Control(pDX, IDC_CMD_MVHL_PED_W2_UNIT, m_untW2);
	DDX_Control(pDX, IDC_CMD_MVHL_PED_L1_UNIT, m_untL1);
	DDX_Control(pDX, IDC_CMD_MVHL_PED_L2_UNIT, m_untL2);
	//}}AFX_DATA_MAP
}

BOOL CCMMvhlItemStdBSBD3701PedestrianPage::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == 13)
		pMsg->wParam = 9;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		pMsg->wParam = VK_TAB;

	return CInternationalDlg::PreTranslateMessage(pMsg);
}

void CCMMvhlItemStdBSBD3701PedestrianPage::SetData2Dlg()
{
	m_edtW1.SetEditUnit(m_pData->dW1);

	CString csVal;
	m_edtW1.GetWindowText(csVal);
	csVal = _T("k*") + csVal;
	m_edtW2.SetWindowText(csVal);

	m_edtL1.SetEditUnit(m_pData->dLoadedLength);
	m_edtL2.SetEditUnit(m_pData->dLoadedLength);

	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdBSBD3701PedestrianPage::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->dW1 = m_edtW1.GetEditValue();
	m_pData->dLoadedLength = m_edtL1.GetEditValue();

	return TRUE;
}

void CCMMvhlItemStdBSBD3701PedestrianPage::InitUnit()
{
	m_untW1.SetTextFmt(_LSX(kN / m));
	m_untW1.SetUnitType(D_UNITSYS_NONE);
	m_untW2.SetTextFmt(_LSX(kN / m));
	m_untW2.SetUnitType(D_UNITSYS_NONE);
	m_untL1.SetTextFmt(_LSX(m));
	m_untL1.SetUnitType(D_UNITSYS_NONE);
	m_untL2.SetTextFmt(_LSX(m));
	m_untL2.SetUnitType(D_UNITSYS_NONE);
}

BEGIN_MESSAGE_MAP(CCMMvhlItemStdBSBD3701PedestrianPage, CDialog)
	//{{AFX_MSG_MAP(CCMMvhlItemStdBSBD3701PedestrianPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBSBD3701PedestrianPage message handlers
BOOL CCMMvhlItemStdBSBD3701PedestrianPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitUnit();

	SetData2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
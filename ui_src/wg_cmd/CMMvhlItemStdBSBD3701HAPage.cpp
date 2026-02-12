// CMMvhlItemStdBSBD3701HAPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdBSBD3701HAPage.h"
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
// CCMMvhlItemStdBSBD3701HAPage dialog

CCMMvhlItemStdBSBD3701HAPage::CCMMvhlItemStdBSBD3701HAPage(T_MVHL_D* pData, CWnd* pParent /*=NULL*/)
	: CDialog(CCMMvhlItemStdBSBD3701HAPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemStdBSBD3701HAPage)
	m_pParent = (CCMMvhlItemStdBSDlg*) pParent;

	m_nLaneFact = 0;
	m_bAddData = FALSE;
	//}}AFX_DATA_INIT

	m_pData = NULL;
	m_pData = pData;
}

void CCMMvhlItemStdBSBD3701HAPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdBSBD3701HAPage)
	DDX_Radio  (pDX, IDC_CMD_MVHL_LANE_FACTOR_RDO1, m_nLaneFact);
	DDX_Control(pDX, IDC_CMD_MVHL_BS37_W1_EDIT, m_edtW1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS37_W1_EXP_STATIC, m_edtW1_exp);
	DDX_Control(pDX, IDC_CMD_MVHL_BS37_W2_EDIT, m_edtW2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS37_W2_EXP_STATIC, m_edtW2_exp);
	DDX_Control(pDX, IDC_CMD_MVHL_BS37_W3_EDIT, m_edtW3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS37_L1_EDIT, m_edtL1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS37_L2_EDIT, m_edtL21);
	DDX_Control(pDX, IDC_CMD_MVHL_BS37_L2_EDIT1, m_edtL22);
	DDX_Control(pDX, IDC_CMD_MVHL_BS37_L3_EDIT, m_edtL3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_PA_EDIT, m_edtPa);

	DDX_Check  (pDX, IDC_CMD_MVHL_BS_ADD_DATA_CHK, m_bAddData);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_ADD_AL_EDT2, m_edtaL);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_ADD_CATEGORY_CBO, m_cmbCategory);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_ADD_LOAD_LEVEL_CBO, m_cmbLoadLevel);

	DDX_Control(pDX, IDC_CMD_MVHL_BS37_W1_UNIT, m_untW1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS37_W2_UNIT, m_untW2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS37_W3_UNIT, m_untW3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS37_L1_UNIT, m_untL1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS37_L2_UNIT, m_untL2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS37_L3_UNIT, m_untL3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_PA_UNIT, m_untPa);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_ADD_AL_UNT, m_untaL);	
	//}}AFX_DATA_MAP
}

BOOL CCMMvhlItemStdBSBD3701HAPage::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == 13)
		pMsg->wParam = 9;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		pMsg->wParam = VK_TAB;

	return CInternationalDlg::PreTranslateMessage(pMsg);
}

void CCMMvhlItemStdBSBD3701HAPage::SetData2Dlg()
{
	m_nLaneFact = m_pData->nLaneFactor;

	m_edtW1.SetEditUnit(m_pData->dW1);
	m_edtW2.SetEditUnit(m_pData->dW2);
	m_edtW3.SetEditUnit(m_pData->dW3);
	m_edtL1.SetEditUnit(m_pData->dLoadedLength);
	m_edtL21.SetEditUnit(m_pData->dLoadedLength);
	m_edtL22.SetEditUnit(m_pData->dLoadedLength2);
	m_edtL3.SetEditUnit(m_pData->dLoadedLength2);
	m_edtPa.SetEditUnit(m_pData->dPa);

	double dExp1 = 0.67;
	double dExp2 = 0.1;

	m_edtW1_exp.SetEditUnit(dExp1);
	m_edtW2_exp.SetEditUnit(dExp2);

	m_bAddData = m_pData->bAddDataBD2101;

	m_edtaL.SetEditUnit(m_pData->dAddDataaL);
	m_cmbCategory.SetCurSel(m_pData->nAddDataCategory);
	m_cmbLoadLevel.SetCurSel(m_pData->nAddDataLoadLevel);

	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdBSBD3701HAPage::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->nLaneFactor = m_nLaneFact;

	m_pData->dW1 = m_edtW1.GetEditValue();
	m_pData->dW2 = m_edtW2.GetEditValue();
	m_pData->dW3 = m_edtW3.GetEditValue();
	m_pData->dLoadedLength = m_edtL1.GetEditValue();
	m_pData->dLoadedLength2 = m_edtL22.GetEditValue();
	m_pData->dPa = m_edtPa.GetEditValue();

	m_pData->dExp = m_edtW1_exp.GetEditValue();
	m_pData->dExp2 = m_edtW2_exp.GetEditValue();

	m_pData->bAddDataBD2101 = m_bAddData;

	m_pData->dAddDataaL = m_edtaL.GetEditValue();
	m_pData->nAddDataCategory = m_cmbCategory.GetCurSel();
	m_pData->nAddDataLoadLevel = m_cmbLoadLevel.GetCurSel();

	return TRUE;
}

void CCMMvhlItemStdBSBD3701HAPage::EnableDisableControls()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_MVHL_BS_HA_LANE_FACT_USER_STC)->EnableWindow(m_nLaneFact == 2);
	GetDlgItem(IDC_CMD_MVHL_BS_HA_LANE_FACT_USER_BTN)->EnableWindow(m_nLaneFact == 2);

	if (m_nLaneFact == 0)
	{
		GetDlgItem(IDC_CMD_MVHL_BS_ADD_DATA_CHK)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CMD_MVHL_BS_ADD_DATA_CHK)->EnableWindow(TRUE);
	}

	GetDlgItem(IDC_CMD_MVHL_BS_ADD_AL_EDT2)->EnableWindow(m_bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_ADD_AL_UNT)->EnableWindow(m_bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_ADD_CATEGORY_CBO)->EnableWindow(m_bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_ADD_LOAD_LEVEL_CBO)->EnableWindow(m_bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_AD_FACTOR_ST)->EnableWindow(m_bAddData);
	GetDlgItem(IDC_CMD_MVHL_AL_ST)->EnableWindow(m_bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_ADD_AL_UNT)->EnableWindow(m_bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_RD_FACTOR_ST)->EnableWindow(m_bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_CATEGORY_ST2)->EnableWindow(m_bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_LOADLEVEL_ST2)->EnableWindow(m_bAddData);
}

void CCMMvhlItemStdBSBD3701HAPage::InitUnit()
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
	m_untL3.SetTextFmt(_LSX(m));
	m_untL3.SetUnitType(D_UNITSYS_NONE);

	m_untPa.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPa);

	m_edtW1_exp.SetUnitType(D_UNITSYS_NONE);
	m_edtW2_exp.SetUnitType(D_UNITSYS_NONE);

	m_edtaL.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untaL.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMMvhlItemStdBSBD3701HAPage::InitCombo()
{
	m_cmbCategory.ResetContent();
	m_cmbCategory.AddString(_T("Hp"));
	m_cmbCategory.AddString(_T("Mp"));
	m_cmbCategory.AddString(_T("Lp"));
	m_cmbCategory.AddString(_T("Hg"));
	m_cmbCategory.AddString(_T("Mg"));
	m_cmbCategory.AddString(_T("Lg"));
	m_cmbCategory.SetCurSel(0);

	m_cmbLoadLevel.ResetContent();
	m_cmbLoadLevel.AddString(_T("40t"));
	m_cmbLoadLevel.AddString(_T("26t"));
	m_cmbLoadLevel.AddString(_T("18t"));
	m_cmbLoadLevel.AddString(_T("7.5t"));
	m_cmbLoadLevel.AddString(_T("G1 FE"));
	m_cmbLoadLevel.AddString(_T("G2 FE"));
	m_cmbLoadLevel.AddString(_T("3t"));
	m_cmbLoadLevel.SetCurSel(0);
}


BEGIN_MESSAGE_MAP(CCMMvhlItemStdBSBD3701HAPage, CDialog)
	//{{AFX_MSG_MAP(CCMMvhlItemStdBSBD3701HAPage)
	ON_BN_CLICKED(IDC_CMD_MVHL_LANE_FACTOR_RDO1, OnLaneFactorRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_HA_LANE_FACT_AUTO_BD21_01_RDO, OnLaneFactorRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_LANE_FACTOR_RDO2, OnLaneFactorRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_HA_LANE_FACT_USER_BTN, OnLaneFactorBtn)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_ADD_DATA_CHK, OnAddDataChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBSBD3701HAPage message handlers
BOOL CCMMvhlItemStdBSBD3701HAPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitUnit();
	InitCombo();

	SetData2Dlg();

	EnableDisableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemStdBSBD3701HAPage::OnLaneFactorRdo()
{
	EnableDisableControls();
}

void CCMMvhlItemStdBSBD3701HAPage::OnLaneFactorBtn()
{
	CCMMvhlSItemUsrLaneFactor dlg;
	dlg.SetData(*m_pData);
	if (dlg.DoModal() == IDOK)
	{
		m_pData->dLaneFactor[0] = dlg.GetData().dLaneFactor[0];
		m_pData->dLaneFactor[1] = dlg.GetData().dLaneFactor[1];
		m_pData->dLaneFactor[2] = dlg.GetData().dLaneFactor[2];
		m_pData->dLaneFactor[3] = dlg.GetData().dLaneFactor[3];
	}
}

void CCMMvhlItemStdBSBD3701HAPage::OnAddDataChk()
{
	EnableDisableControls();
}
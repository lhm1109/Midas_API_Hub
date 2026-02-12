// CMMvhlItemUsrBSHAPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUsrBSHAPage.h"
#include "CMMvhlItemUsrDlg.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\UnitCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_base\TestEnvMgr.h"

#include "CMMvhlSItemUsrLaneFactor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CInternationalDlg
/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrBSHAPage dialog


CCMMvhlItemUsrBSHAPage::CCMMvhlItemUsrBSHAPage(T_MVHL_D* pData, CWnd* pParent /*=NULL*/)
	: CDialog(CCMMvhlItemUsrBSHAPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemUsrBSHAPage)
	m_pParent = (CCMMvhlItemUsrDlg*) pParent;

	m_nLaneFact = 0;
	//}}AFX_DATA_INIT

	m_pData = NULL;
	m_pData = pData;
}


void CCMMvhlItemUsrBSHAPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemUsrBSHAPage)
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W1_EDIT, m_edtBSW1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W2_EDIT, m_edtBSW2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W3_EDIT, m_edtBSW3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L1_EDIT, m_edtBSL1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L2_EDIT2, m_edtBSL21);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L2_EDIT, m_edtBSL22); 
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L3_EDIT, m_edtBSL3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_PA_EDIT, m_edtBSPa);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W1_EXP_STATIC, m_edtBSW1_exp);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W2_EXP_STATIC, m_edtBSW2_exp);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W1_UNIT, m_untBSW1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W2_UNIT, m_untBSW2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W3_UNIT, m_untBSW3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L1_UNIT, m_untBSL1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L2_UNIT, m_untBSL2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L3_UNIT, m_untBSL3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_PA_UNIT, m_untBSPa);
	DDX_Radio  (pDX, IDC_CMD_MVHL_BS_HA_LANE_FACT_AUTO_RDO, m_nLaneFact);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_HA_LANE_FACT_GRP,      m_chkHALaneFactor);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_ADD_AL_EDT, m_edtaL);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_ADD_AL_UNT, m_untaL);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_ADD_CATEGORY_CBO, m_cbxCategory);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_ADD_LOAD_LEVEL_CBO, m_cbxLoadLevel);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_ADD_DATA_CHK, m_chkAddData);  
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvhlItemUsrBSHAPage, CDialog)
	//{{AFX_MSG_MAP(CCMMvhlItemUsrBSHAPage)
	ON_EN_CHANGE (IDC_CMD_MVHL_BS_L1_EDIT, OnChangeLoadLen1Edit)
	ON_EN_CHANGE (IDC_CMD_MVHL_BS_L2_EDIT, OnChangeLoadLen2Edit)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_HA_LANE_FACT_AUTO_RDO, OnChangeLaneFactorRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_HA_LANE_FACT_AUTO_BD21_01_RDO, OnChangeLaneFactorRdo)  
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_HA_LANE_FACT_USER_RDO, OnChangeLaneFactorRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_HA_LANE_FACT_USER_BTN, OnHALaneFactor)  
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_ADD_DATA_CHK, OnChkAddData)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvhlItemUsrBSHAPage::SetData2Dlg()
{
	m_edtBSW1.SetEditUnit(m_pData->dW1);
	m_edtBSW2.SetEditUnit(m_pData->dW2);
	m_edtBSW3.SetEditUnit(m_pData->dW3);
	m_edtBSL1.SetEditUnit(m_pData->dLoadedLength);
	m_edtBSL21.SetEditUnit(m_pData->dLoadedLength);
	m_edtBSL22.SetEditUnit(m_pData->dLoadedLength2);
	m_edtBSL3.SetEditUnit(m_pData->dLoadedLength2);
	m_edtBSPa.SetEditUnit(m_pData->dPa);

	m_edtBSW1_exp.SetEditUnit(m_pData->dExp, 3);
	m_edtBSW2_exp.SetEditUnit(m_pData->dExp2, 3);

	m_nLaneFact = m_pData->nLaneFactor;

	m_chkAddData.SetCheck(m_pData->bAddDataBD2101);

	m_edtaL.SetEditUnit(m_pData->dAddDataaL);
	m_cbxCategory.SetCurSel(m_pData->nAddDataCategory);
	m_cbxLoadLevel.SetCurSel(m_pData->nAddDataLoadLevel);

	UpdateData(FALSE);
}

BOOL CCMMvhlItemUsrBSHAPage::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->dW1            = m_edtBSW1.GetEditValue();
	m_pData->dW2            = m_edtBSW2.GetEditValue();
	m_pData->dW3            = m_edtBSW3.GetEditValue();
	m_pData->dLoadedLength  = m_edtBSL1.GetEditValue();
	m_pData->dLoadedLength2 = m_edtBSL22.GetEditValue();
	m_pData->dPa            = m_edtBSPa.GetEditValue();

	m_pData->dExp           = m_edtBSW1_exp.GetEditValue();
	m_pData->dExp2          = m_edtBSW2_exp.GetEditValue();

	m_pData->nLaneFactor    = m_nLaneFact;
	//m_pData->dLaneFactor[0] = m_edtLaneFact1.GetEditValue(); // CCMMvhlSItemUsrLaneFactor 를 여는 곳에서 세팅 함
	//m_pData->dLaneFactor[1] = m_edtLaneFact2.GetEditValue();
	//m_pData->dLaneFactor[2] = m_edtLaneFact3.GetEditValue();
	//m_pData->dLaneFactor[3] = m_edtLaneFact4.GetEditValue();

	m_pData->bAddDataBD2101 =  m_chkAddData.IsWindowEnabled() ? m_chkAddData.GetCheck() : FALSE;

	m_pData->dAddDataaL = m_edtaL.GetEditValue();
	m_pData->nAddDataCategory = m_cbxCategory.GetCurSel();
	m_pData->nAddDataLoadLevel = m_cbxLoadLevel.GetCurSel();

	m_pData->nLoadType = 0;

	return TRUE;
}

void CCMMvhlItemUsrBSHAPage::InitUnit()
{
	m_untBSW1.SetTextFmt(_LSX(kN/m));
	m_untBSW1.SetUnitType(D_UNITSYS_NONE);
	m_untBSW2.SetTextFmt(_LSX(kN/m));
	m_untBSW2.SetUnitType(D_UNITSYS_NONE);
	m_untBSW3.SetTextFmt(_LSX(kN/m));
	m_untBSW3.SetUnitType(D_UNITSYS_NONE);
	m_untBSL1.SetTextFmt(_LSX(m));
	m_untBSL1.SetUnitType(D_UNITSYS_NONE);
	m_untBSL2.SetTextFmt(_LSX(m));
	m_untBSL2.SetUnitType(D_UNITSYS_NONE);
	m_untBSL3.SetTextFmt(_LSX(m));
	m_untBSL3.SetUnitType(D_UNITSYS_NONE);

	m_untBSPa.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPa);

	m_edtBSW1_exp.SetUnitType(D_UNITSYS_NONE);
	m_edtBSW2_exp.SetUnitType(D_UNITSYS_NONE);
	
	m_edtaL.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untaL.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrBSHAPage message handlers
BOOL CCMMvhlItemUsrBSHAPage::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_chkHALaneFactor.InitControl(this, CCMMvhlItemUsrBSHAPage::IDD, IDC_CMD_MVHL_BS_HA_LANE_FACT_GRP,TRUE,TRUE);

	InitUnit();
	InitCbx();

	SetData2Dlg();

	OnChangeLaneFactorRdo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemUsrBSHAPage::OnChangeLoadLen1Edit()
{
	UpdateData(TRUE);

	double dBSL1 = m_edtBSL1.GetEditValue();
	m_edtBSL21.SetEditUnit(dBSL1);

	UpdateData(FALSE);
}

void CCMMvhlItemUsrBSHAPage::OnChangeLoadLen2Edit()
{
	UpdateData(TRUE);

	double dBSL22 = m_edtBSL22.GetEditValue();

	m_edtBSL3.SetEditUnit(dBSL22);

	UpdateData(FALSE);
}

void CCMMvhlItemUsrBSHAPage::OnChangeLaneFactorRdo()
{
	SetEnable();

}

BOOL CCMMvhlItemUsrBSHAPage::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
		pMsg->wParam=9;
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE)
		pMsg->wParam=VK_TAB;

	return CInternationalDlg::PreTranslateMessage(pMsg);
}

LRESULT CCMMvhlItemUsrBSHAPage::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	m_pParent->ResizeBSDialogByFoldGroupBox();
	
	return 0L;
}

void CCMMvhlItemUsrBSHAPage::InitCbx()
{
	m_cbxCategory.ResetContent();
	m_cbxCategory.AddString(_T("Hp"));
	m_cbxCategory.AddString(_T("Mp"));
	m_cbxCategory.AddString(_T("Lp"));
	m_cbxCategory.AddString(_T("Hg"));
	m_cbxCategory.AddString(_T("Mg"));
	m_cbxCategory.AddString(_T("Lg"));
	m_cbxCategory.SetCurSel(0);
	m_cbxLoadLevel.ResetContent();
	m_cbxLoadLevel.AddString(_T("40t"));
	m_cbxLoadLevel.AddString(_T("26t"));
	m_cbxLoadLevel.AddString(_T("18t"));
	m_cbxLoadLevel.AddString(_T("7.5t"));
	m_cbxLoadLevel.AddString(_T("G1 FE"));
	m_cbxLoadLevel.AddString(_T("G2 FE"));
	m_cbxLoadLevel.AddString(_T("3t"));
	m_cbxLoadLevel.SetCurSel(0);
}

void CCMMvhlItemUsrBSHAPage::SetEnable()
{
	UpdateData(TRUE);
	
	GetDlgItem(IDC_CMD_MVHL_BS_HA_LANE_FACT_USER_STC)->EnableWindow(m_nLaneFact == 2);
	GetDlgItem(IDC_CMD_MVHL_BS_HA_LANE_FACT_USER_BTN)->EnableWindow(m_nLaneFact == 2);

	if(m_nLaneFact == 0)
	{
		m_chkAddData.EnableWindow(FALSE);
	}
	else
	{
		m_chkAddData.EnableWindow(TRUE);
	}

	BOOL bAddData = m_chkAddData.GetCheck() && m_chkAddData.IsWindowEnabled();
	GetDlgItem(IDC_CMD_MVHL_BS_ADD_AL_EDT)->EnableWindow(bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_ADD_AL_UNT)->EnableWindow(bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_ADD_CATEGORY_CBO)->EnableWindow(bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_ADD_LOAD_LEVEL_CBO)->EnableWindow(bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_AD_FACTOR_ST)->EnableWindow(bAddData);
	GetDlgItem(IDC_CMD_MVHL_AL_ST)->EnableWindow(bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_ADD_AL_UNT)->EnableWindow(bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_RD_FACTOR_ST)->EnableWindow(bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_CATEGORY_ST)->EnableWindow(bAddData);
	GetDlgItem(IDC_CMD_MVHL_BS_LOADLEVEL_ST)->EnableWindow(bAddData);
}

void CCMMvhlItemUsrBSHAPage::OnHALaneFactor()
{
	CCMMvhlSItemUsrLaneFactor dlg;
	dlg.SetData(*m_pData);
	if(dlg.DoModal() == IDOK)
	{
		m_pData->dLaneFactor[0] = dlg.GetData().dLaneFactor[0];
		m_pData->dLaneFactor[1] = dlg.GetData().dLaneFactor[1];
		m_pData->dLaneFactor[2] = dlg.GetData().dLaneFactor[2];
		m_pData->dLaneFactor[3] = dlg.GetData().dLaneFactor[3];
	}
}

void CCMMvhlItemUsrBSHAPage::OnChkAddData()
{
	SetEnable();
}

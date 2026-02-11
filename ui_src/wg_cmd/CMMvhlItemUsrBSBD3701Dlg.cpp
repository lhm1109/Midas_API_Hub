// CMMvhlItemUsrBSBD3701Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUsrBSBD3701Dlg.h"
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
// CCMMvhlItemUsrBSBD3701Dlg dialog


CCMMvhlItemUsrBSBD3701Dlg::CCMMvhlItemUsrBSBD3701Dlg(T_MVHL_D* pData, CWnd* pParent /*=NULL*/)
	: CDialog(CCMMvhlItemUsrBSBD3701Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemUsrBSBD3701Dlg)
// 	m_dBSW1 = 0.0;
//  m_dBSW2 = 0.0;
//  m_dBSW3 = 0.0;
//  m_dBSL1 = 0.0;
//  m_dBSL21 = 0.0;
//  m_dBSL22 = 0.0;
//  m_dBSL3 = 0.0;
//  m_dBSPa = 0.0;
//  m_dBSPb = 0.0;
// 	m_dBSD1 = 0.0;
// 	m_dBSD2 = 0.0;
// 	m_dBSD  = 0.0;
//  m_dBSD3 = 0.0;
// 	m_dBSD4 = 0.0;
// 	m_dBSD5 = 0.0;
// 	m_dBSD6 = 0.0;
//  m_strBSW1_exp = _T("");
//  m_strBSW2_exp = _T("");
	m_pParent = (CCMMvhlItemUsrDlg*) pParent;

	m_nLaneFact = 0;
	//}}AFX_DATA_INIT

	m_pData = NULL;
	m_pData = pData;
}


void CCMMvhlItemUsrBSBD3701Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemUsrBSBD3701Dlg)
	DDX_Control(pDX, IDC_CMD_MVHL_BS_UNIT_NUM_EDIT, m_edtNumUnit);
// 	DDX_Text   (pDX, IDC_CMD_MVHL_BS_W1_EDIT, m_dBSW1);
//  DDX_Text   (pDX, IDC_CMD_MVHL_BS_W2_EDIT, m_dBSW2);
//  DDX_Text   (pDX, IDC_CMD_MVHL_BS_W3_EDIT, m_dBSW3);
//  DDX_Text   (pDX, IDC_CMD_MVHL_BS_L1_EDIT, m_dBSL1);
//  DDX_Text   (pDX, IDC_CMD_MVHL_BS_L2_EDIT2, m_dBSL21);
//  DDX_Text   (pDX, IDC_CMD_MVHL_BS_L2_EDIT, m_dBSL22);
//  DDX_Text   (pDX, IDC_CMD_MVHL_BS_L3_EDIT, m_dBSL3);
//  DDX_Text   (pDX, IDC_CMD_MVHL_BS_PA_EDIT, m_dBSPa);
//  DDX_Text   (pDX, IDC_CMD_MVHL_BS_PB_EDIT, m_dBSPb);
//	DDX_Text   (pDX, IDC_CMD_MVHL_BS_D1_EDIT, m_dBSD1);
// 	DDX_Text   (pDX, IDC_CMD_MVHL_BS_D2_EDIT, m_dBSD2);
// 	DDX_Text   (pDX, IDC_CMD_MVHL_BS_D_EDIT,  m_dBSD);
//  DDX_Text   (pDX, IDC_CMD_MVHL_BS_D3_EDIT, m_dBSD3);
// 	DDX_Text   (pDX, IDC_CMD_MVHL_BS_D4_EDIT, m_dBSD4);
// 	DDX_Text   (pDX, IDC_CMD_MVHL_BS_D5_EDIT, m_dBSD5);
// 	DDX_Text   (pDX, IDC_CMD_MVHL_BS_D6_EDIT, m_dBSD6);
	
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W1_EDIT, m_edtBSW1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W2_EDIT, m_edtBSW2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W3_EDIT, m_edtBSW3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L1_EDIT, m_edtBSL1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L2_EDIT2, m_edtBSL21);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L2_EDIT, m_edtBSL22); 
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L3_EDIT, m_edtBSL3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_PA_EDIT, m_edtBSPa);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_PB_EDIT, m_edtBSPb);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D1_EDIT, m_edtBSD1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D2_EDIT, m_edtBSD2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D_EDIT,  m_edtBSD);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D3_EDIT, m_edtBSD3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D4_EDIT, m_edtBSD4);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D5_EDIT, m_edtBSD5);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D6_EDIT, m_edtBSD6);
//   DDX_Text   (pDX, IDC_CMD_MVHL_BS_W1_EXP_STATIC, m_strBSW1_exp);
//   DDX_Text   (pDX, IDC_CMD_MVHL_BS_W2_EXP_STATIC, m_strBSW2_exp);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W1_EXP_STATIC, m_edtBSW1_exp);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W2_EXP_STATIC, m_edtBSW2_exp);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W1_UNIT, m_untBSW1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W2_UNIT, m_untBSW2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_W3_UNIT, m_untBSW3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L1_UNIT, m_untBSL1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L2_UNIT, m_untBSL2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_L3_UNIT, m_untBSL3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_PA_UNIT, m_untBSPa);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_PB_UNIT, m_untBSPb);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D1_UNIT, m_untBSD1);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D2_UNIT, m_untBSD2);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D_UNIT,  m_untBSD);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D3_UNIT, m_untBSD3);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D4_UNIT, m_untBSD4);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D5_UNIT, m_untBSD5);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_D6_UNIT, m_untBSD6);
	DDX_Radio  (pDX, IDC_CMD_MVHL_BS_HA_LANE_FACT_AUTO_RDO, m_nLaneFact);
	
	DDX_Control(pDX, IDC_CMD_MVHL_BS_HA_LANE_FACT_GRP,      m_chkHALaneFactor);

	DDX_Control(pDX, IDC_CMD_MVHL_BS_ADD_AL_EDT, m_edtaL);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_ADD_AL_UNT, m_untaL);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_ADD_CATEGORY_CBO, m_cbxCategory);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_ADD_LOAD_LEVEL_CBO, m_cbxLoadLevel);
	DDX_Control(pDX, IDC_CMD_MVHL_BS_ADD_DATA_CHK, m_chkAddData);  
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvhlItemUsrBSBD3701Dlg, CDialog)
	//{{AFX_MSG_MAP(CCMMvhlItemUsrBSBD3701Dlg)
	ON_EN_CHANGE (IDC_CMD_MVHL_BS_L1_EDIT, OnChangeLoadLen1Edit)
	ON_EN_CHANGE (IDC_CMD_MVHL_BS_L2_EDIT, OnChangeLoadLen2Edit)
	ON_EN_CHANGE (IDC_CMD_MVHL_BS_D2_EDIT, OnChangeLoadDistEdit)
	ON_EN_CHANGE (IDC_CMD_MVHL_BS_D_EDIT,  OnChangeLoadDistEdit)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_HA_LANE_FACT_AUTO_RDO, OnChangeLaneFactorRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_HA_LANE_FACT_AUTO_BD21_01_RDO, OnChangeLaneFactorRdo)  
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_HA_LANE_FACT_USER_RDO, OnChangeLaneFactorRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_HA_LANE_FACT_USER_BTN, OnHALaneFactor)  
	ON_BN_CLICKED(IDC_CMD_MVHL_BS_ADD_DATA_CHK, OnChkAddData)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvhlItemUsrBSBD3701Dlg::SetData2Dlg()
{
//  m_dBSW1  = m_pData->dW1;
//  m_dBSW2  = m_pData->dW2;
//  m_dBSW3  = m_pData->dW3;
//  m_dBSL1  = m_pData->dLoadedLength;
//  m_dBSL21 = m_pData->dLoadedLength;
//  m_dBSL22 = m_pData->dLoadedLength2;
//  m_dBSL3  = m_pData->dLoadedLength2;
//  m_dBSPa  = m_pData->dPa;
//  m_dBSPb  = m_pData->dPb;
// 	m_dBSD1  = m_pData->dD1;
// 	m_dBSD2  = m_pData->dD2;
// 	m_dBSD   = m_pData->dDelta;
//  m_dBSD3  = m_dBSD2 + m_dBSD;
// 	m_dBSD4  = m_dBSD2 + m_dBSD * 2.;
// 	m_dBSD5  = m_dBSD2 + m_dBSD * 3.;
// 	m_dBSD6  = m_dBSD2 + m_dBSD * 4.;

	m_edtBSW1.SetEditUnit(m_pData->dW1);
	m_edtBSW2.SetEditUnit(m_pData->dW2);
	m_edtBSW3.SetEditUnit(m_pData->dW3);
	m_edtBSL1.SetEditUnit(m_pData->dLoadedLength);
	m_edtBSL21.SetEditUnit(m_pData->dLoadedLength);
	m_edtBSL22.SetEditUnit(m_pData->dLoadedLength2);
	m_edtBSL3.SetEditUnit(m_pData->dLoadedLength2);
	m_edtBSPa.SetEditUnit(m_pData->dPa);
	m_edtBSPb.SetEditUnit(m_pData->dPb);
	m_edtBSD1.SetEditUnit(m_pData->dD1);
	m_edtBSD2.SetEditUnit(m_pData->dD2);
	m_edtBSD .SetEditUnit(m_pData->dDelta);
	m_edtBSD3.SetEditUnit(m_pData->dD2 + m_pData->dDelta);
	m_edtBSD4.SetEditUnit(m_pData->dD2 + m_pData->dDelta * 2.);
	m_edtBSD5.SetEditUnit(m_pData->dD2 + m_pData->dDelta * 3.);
	m_edtBSD6.SetEditUnit(m_pData->dD2 + m_pData->dDelta * 4.);

	//m_strBSW1_exp.Format(_T("%.3g"), m_pData->dExp);
	//m_strBSW2_exp.Format(_T("%.3g"),GetExpValue(m_dBSW1, m_dBSW2, m_dBSL1));
	//m_strBSW2_exp.Format(_T("%.3g"),GetExpValue(m_pData->dW1, m_pData->dW2, m_pData->dLoadedLength));
	//m_strBSW2_exp.Format(_T("%.3g"), m_pData->dExp2);

	m_edtBSW1_exp.SetEditUnit(m_pData->dExp, 3);
	m_edtBSW2_exp.SetEditUnit(m_pData->dExp2, 3);

	m_edtNumUnit.SetEditUnit(m_pData->dUnitNum);

	m_nLaneFact = m_pData->nLaneFactor;
	
	m_chkAddData.SetCheck(m_pData->bAddDataBD2101);

	m_edtaL.SetEditUnit(m_pData->dAddDataaL);
	m_cbxCategory.SetCurSel(m_pData->nAddDataCategory);
	m_cbxLoadLevel.SetCurSel(m_pData->nAddDataLoadLevel);

	UpdateData(FALSE);
}

BOOL CCMMvhlItemUsrBSBD3701Dlg::SetDlg2Data()
{
	UpdateData(TRUE);

//  m_pData->dW1            = m_dBSW1;
//  m_pData->dW2            = m_dBSW2;
//  m_pData->dW3            = m_dBSW3;
//  m_pData->dLoadedLength  = m_dBSL1;
//  m_pData->dLoadedLength2 = m_dBSL22;
//  m_pData->dPa            = m_dBSPa;
//  m_pData->dPb            = m_dBSPb;
// 	m_pData->dD1            = m_dBSD1;
// 	m_pData->dD2            = m_dBSD2;
// 	m_pData->dDelta         = m_dBSD;

	m_pData->dW1            = m_edtBSW1.GetEditValue();
	m_pData->dW2            = m_edtBSW2.GetEditValue();
	m_pData->dW3            = m_edtBSW3.GetEditValue();
	m_pData->dLoadedLength  = m_edtBSL1.GetEditValue();
	m_pData->dLoadedLength2 = m_edtBSL22.GetEditValue();
	m_pData->dPa            = m_edtBSPa.GetEditValue();
	m_pData->dPb            = m_edtBSPb.GetEditValue();
	m_pData->dD1            = m_edtBSD1.GetEditValue();
	m_pData->dD2            = m_edtBSD2.GetEditValue();
	m_pData->dDelta         = m_edtBSD.GetEditValue();

//  m_pData->dExp           = _tstof(m_strBSW1_exp);
// 	m_pData->dExp2          = _tstof(m_strBSW2_exp);
	m_pData->dExp           = m_edtBSW1_exp.GetEditValue();
	m_pData->dExp2          = m_edtBSW2_exp.GetEditValue();

	m_pData->dUnitNum = m_edtNumUnit.GetEditValue();

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

void CCMMvhlItemUsrBSBD3701Dlg::InitUnit()
{
	// 단위 변환
	/*
	m_untBSW1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dW3);
	m_untBSW2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dW3);
	m_untBSW3.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dW3);
	m_untBSL1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dLoadedLength);
	m_untBSL2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dLoadedLength);
	m_untBSL3.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dLoadedLength);
	*/
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
	m_untBSPb.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPb);
	m_untBSD1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD3.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD4.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD5.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);
	m_untBSD6.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDelta);

	m_edtBSW1_exp.SetUnitType(D_UNITSYS_NONE);
	m_edtBSW2_exp.SetUnitType(D_UNITSYS_NONE);
	m_edtNumUnit.SetUnitType(D_UNITSYS_NONE);


	m_edtaL.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untaL.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrBSBD3701Dlg message handlers
BOOL CCMMvhlItemUsrBSBD3701Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_chkHALaneFactor.InitControl(this, CCMMvhlItemUsrBSBD3701Dlg::IDD, IDC_CMD_MVHL_BS_HA_LANE_FACT_GRP,TRUE,TRUE);

	InitUnit();
	InitCbx();

	SetData2Dlg();

	OnChangeLaneFactorRdo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemUsrBSBD3701Dlg::OnChangeLoadLen1Edit()
{
	UpdateData(TRUE);

	double dBSL1 = m_edtBSL1.GetEditValue();
	m_edtBSL21.SetEditUnit(dBSL1);

	UpdateData(FALSE);
}

void CCMMvhlItemUsrBSBD3701Dlg::OnChangeLoadLen2Edit()
{
	UpdateData(TRUE);

	double dBSL22 = m_edtBSL22.GetEditValue();

	m_edtBSL3.SetEditUnit(dBSL22);

	UpdateData(FALSE);
}

void CCMMvhlItemUsrBSBD3701Dlg::OnChangeLoadDistEdit()
{
	UpdateData(TRUE);

//  m_dBSD3 = m_dBSD2 + m_dBSD;
// 	m_dBSD4 = m_dBSD2 + m_dBSD * 2.;
// 	m_dBSD5 = m_dBSD2 + m_dBSD * 3.;
// 	m_dBSD6 = m_dBSD2 + m_dBSD * 4.;

	double dBSD2 = m_edtBSD2.GetEditValue();
	double dBSD  = m_edtBSD .GetEditValue();
	
	m_edtBSD3.SetEditUnit(dBSD2 + dBSD);
	m_edtBSD4.SetEditUnit(dBSD2 + dBSD * 2.);
	m_edtBSD5.SetEditUnit(dBSD2 + dBSD * 3.);
	m_edtBSD6.SetEditUnit(dBSD2 + dBSD * 4.);

	UpdateData(FALSE);
}

void CCMMvhlItemUsrBSBD3701Dlg::SetEnable()
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

void CCMMvhlItemUsrBSBD3701Dlg::OnChangeLaneFactorRdo()
{
	SetEnable();
}

void CCMMvhlItemUsrBSBD3701Dlg::OnHALaneFactor()
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

void CCMMvhlItemUsrBSBD3701Dlg::OnChkAddData()
{
	SetEnable();
}

double CCMMvhlItemUsrBSBD3701Dlg::GetExpValue(double a, double b, double l)
{
	double dExp=0.;

	BOOL bCalc = TRUE;
	if(a <= 0. || b <= 0.) bCalc = FALSE;
	if(l <= 0. || l == 1.) bCalc = FALSE; 
	if(bCalc)
	{
		//double dExp1 = _tstof(m_strBSW1_exp);
		double dExp1 = m_edtBSW1_exp.GetEditValue();
		a *= pow((1.0/l),dExp1);
		dExp = log10(b/a) / log10(l);
	}
	else dExp = 0.1;

	return dExp;
}

BOOL CCMMvhlItemUsrBSBD3701Dlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
		pMsg->wParam=9;
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE)
		pMsg->wParam=VK_TAB;

	return CInternationalDlg::PreTranslateMessage(pMsg);
}

LRESULT CCMMvhlItemUsrBSBD3701Dlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	m_pParent->ResizeBSDialogByFoldGroupBox();
	
	return 0L;
}

void CCMMvhlItemUsrBSBD3701Dlg::InitCbx()
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

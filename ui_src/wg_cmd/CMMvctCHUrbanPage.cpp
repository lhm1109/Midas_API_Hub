// CMMvctCHUrbanPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvctCHUrbanPage.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMMvctCHUrbanPage dialog

CCMMvctCHUrbanPage::CCMMvctCHUrbanPage(T_BRDG_URBAN* pData, CWnd* pParent /*=NULL*/)
	: CInternationalDlg(CCMMvctCHUrbanPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvctCHUrbanPage)
	//}}AFX_DATA_INIT

	m_pData = NULL;
	m_pData = pData;

	m_bImpactFactor = TRUE;
}

void CCMMvctCHUrbanPage::DoDataExchange(CDataExchange* pDX)
{
	CInternationalDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvctCHUrbanPage)	
	DDX_Control(pDX, IDC_CMD_MVCT_CASE1_L1_EDT,          m_edtCase1L_a);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE1_F1_EDT,          m_edtCase1Fact_a);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE1_L2_EDT,          m_edtCase1L_b);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE1_F2_EDT,          m_edtCase1Fact_b);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE1_L_U_EDT,         m_edtCase1L_U);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE1_L_L_EDT,         m_edtCase1L_L);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE1_F_U_EDT,         m_edtCase1F_U);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE1_F_L_EDT,         m_edtCase1F_L);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE2_COEF_F1_EDT,     m_edtCase2FactCoef1);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE2_COEF_F2_EDT,     m_edtCase2FactCoef2);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE2_LIMIT_F_EDT,     m_edtCase2FactLimit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvctCHUrbanPage, CInternationalDlg)
	//{{AFX_MSG_MAP(CCMMvctCHUrbanPage)
	ON_EN_CHANGE(IDC_CMD_MVCT_CASE1_L1_EDT, OnCmdMvchCHUrbanCase1LEdt)
	ON_EN_CHANGE(IDC_CMD_MVCT_CASE1_L2_EDT, OnCmdMvchCHUrbanCase1LEdt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvctCHUrbanPage::SetData2Dlg()
{
	m_edtCase1L_a.SetEditUnit(m_pData->dCase1L_a);
	m_edtCase1Fact_a.SetEditUnit(m_pData->dCase1Fact_a);
	m_edtCase1L_b.SetEditUnit(m_pData->dCase1L_b);
	m_edtCase1Fact_b.SetEditUnit(m_pData->dCase1Fact_b);
	m_edtCase1L_U.SetEditUnit(m_pData->dCase1L_a); // L 과 동일한 값
	m_edtCase1L_L.SetEditUnit(m_pData->dCase1L_b); // L 과 동일한 값
	m_edtCase1F_U.SetEditUnit(m_pData->dCase1_U);
	m_edtCase1F_L.SetEditUnit(m_pData->dCase1_L);
	m_edtCase2FactCoef1.SetEditUnit(m_pData->dCase2FactCoef1);
	m_edtCase2FactCoef2.SetEditUnit(m_pData->dCase2FactCoef2);
	m_edtCase2FactLimit.SetEditUnit(m_pData->dCase2FactLimit);

	UpdateData(FALSE);
}

BOOL CCMMvctCHUrbanPage::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->dCase1L_a = m_edtCase1L_a.GetEditValue();
	m_pData->dCase1Fact_a = m_edtCase1Fact_a.GetEditValue();
	m_pData->dCase1L_b = m_edtCase1L_b.GetEditValue();
	m_pData->dCase1Fact_b = m_edtCase1Fact_b.GetEditValue();
	m_pData->dCase1_U = m_edtCase1F_U.GetEditValue();
	m_pData->dCase1_L = m_edtCase1F_L.GetEditValue();
	m_pData->dCase2FactCoef1 = m_edtCase2FactCoef1.GetEditValue();
	m_pData->dCase2FactCoef2 = m_edtCase2FactCoef2.GetEditValue();
	m_pData->dCase2FactLimit = m_edtCase2FactLimit.GetEditValue();
		
	return TRUE;
}

void CCMMvctCHUrbanPage::EnableDisableControls(BOOL bImpactFactor)
{
	m_bImpactFactor = bImpactFactor;

	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	CDlgUtil::GetCtrlAllIDbyDlg(this, aControls);
	CDlgUtil::CtrlEnableDisable(this, aControls, m_bImpactFactor);
}

void CCMMvctCHUrbanPage::InitUnit()
{
	m_edtCase1L_a.SetUnitType(D_UNITSYS_NONE);
	m_edtCase1Fact_a.SetUnitType(D_UNITSYS_NONE);
	m_edtCase1L_b.SetUnitType(D_UNITSYS_NONE);
	m_edtCase1Fact_b.SetUnitType(D_UNITSYS_NONE);
	m_edtCase1L_U.SetUnitType(D_UNITSYS_NONE);
	m_edtCase1L_L.SetUnitType(D_UNITSYS_NONE);
	m_edtCase1F_U.SetUnitType(D_UNITSYS_NONE);
	m_edtCase1F_L.SetUnitType(D_UNITSYS_NONE);
	m_edtCase2FactCoef1.SetUnitType(D_UNITSYS_NONE);
	m_edtCase2FactCoef2.SetUnitType(D_UNITSYS_NONE);
	m_edtCase2FactLimit.SetUnitType(D_UNITSYS_NONE);
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHUrbanPage message handlers
BOOL CCMMvctCHUrbanPage::OnInitDialog() 
{
	CInternationalDlg::OnInitDialog();
	
	InitUnit();
	SetData2Dlg();

	EnableDisableControls(m_bImpactFactor);

	OnCmdMvchCHUrbanCase1LEdt();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvctCHUrbanPage::OnCmdMvchCHUrbanCase1LEdt()
{
	UpdateData(TRUE);

	double dL1 = m_edtCase1L_a.GetEditValue();;
	double dL2 = m_edtCase1L_b.GetEditValue();;

	m_edtCase1L_U.SetEditUnit(dL1);
	m_edtCase1L_L.SetEditUnit(dL2);

	UpdateData(FALSE);
}
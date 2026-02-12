// CMMvctCHRCPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvctCHRCPage.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMMvctCHRCPage dialog

CCMMvctCHRCPage::CCMMvctCHRCPage(T_BRDG_RC* pData, CWnd* pParent /*=NULL*/)
	: CInternationalDlg(CCMMvctCHRCPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvctCHRCPage)
	//}}AFX_DATA_INIT

	m_pData = NULL;
	m_pData = pData;

	m_bImpactFactor = TRUE;

	m_bCase2 = FALSE;

	m_pDoc = CDBDoc::GetDocPoint();
}

void CCMMvctCHRCPage::DoDataExchange(CDataExchange* pDX)
{
	CInternationalDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvctCHRCPage)
	DDX_Control(pDX, IDC_CMD_MVCT_CASE1_L1_EDT,    m_edtCase1L1);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE1_F1_EDT,    m_edtCase1F1);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE1_L2_EDT,    m_edtCase1L2);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE1_F2_EDT,    m_edtCase1F2);
	DDX_Check  (pDX, IDC_CMD_MVCT_CASE2_L1_CHK,    m_bCase2);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE2_L1_EDT,    m_edtCase2L1);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE2_F1_EDT,    m_edtCase2F1);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE2_L2_EDT,    m_edtCase2L2);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE2_F2_EDT,    m_edtCase2F2);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE2_GRUP_CMB,  m_cmbGrup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvctCHRCPage, CInternationalDlg)
	//{{AFX_MSG_MAP(CCMMvctCHRCPage)
	ON_BN_CLICKED(IDC_CMD_MVCT_CASE2_L1_CHK, OnCmdMvchCHRCCase2Chk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvctCHRCPage::SetData2Dlg()
{
	m_edtCase1L1.SetEditUnit(m_pData->dCase1L_a);
	m_edtCase1F1.SetEditUnit(m_pData->dCase1Fact_a);
	m_edtCase1L2.SetEditUnit(m_pData->dCase1L_b);
	m_edtCase1F2.SetEditUnit(m_pData->dCase1Fact_b);
	m_bCase2 = m_pData->bCase2;
	m_edtCase2L1.SetEditUnit(m_pData->dCase2L_a);
	m_edtCase2F1.SetEditUnit(m_pData->dCase2Fact_a);
	m_edtCase2L2.SetEditUnit(m_pData->dCase2L_b);
	m_edtCase2F2.SetEditUnit(m_pData->dCase2Fact_b);

	T_GRUP_D GrupD; GrupD.Initialize();
	m_pDoc->m_pAttrCtrl->GetGrup(m_pData->GrupK, GrupD);
	m_cmbGrup.SelectString(0, GrupD.GroupName);

	UpdateData(FALSE);
}

BOOL CCMMvctCHRCPage::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->dCase1L_a    = m_edtCase1L1.GetEditValue();
	m_pData->dCase1Fact_a = m_edtCase1F1.GetEditValue();
	m_pData->dCase1L_b    = m_edtCase1L2.GetEditValue();
	m_pData->dCase1Fact_b = m_edtCase1F2.GetEditValue();
	m_pData->bCase2 = m_bCase2;
	m_pData->dCase2L_a    = m_edtCase2L1.GetEditValue();
	m_pData->dCase2Fact_a = m_edtCase2F1.GetEditValue();
	m_pData->dCase2L_b    = m_edtCase2L2.GetEditValue();
	m_pData->dCase2Fact_b = m_edtCase2F2.GetEditValue();

	if(m_bCase2 && m_cmbGrup.GetCurSel() != -1)
	{
		CString strGrup;
		m_cmbGrup.GetLBText(m_cmbGrup.GetCurSel(), strGrup);
		m_pData->GrupK = m_pDoc->m_pAttrCtrl->GetGrupKey(strGrup);
	}
		
	return TRUE;
}

void CCMMvctCHRCPage::EnableDisableControls(BOOL bImpactFactor)
{
	UpdateData(TRUE);

	m_bImpactFactor = bImpactFactor;

	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	CDlgUtil::GetCtrlAllIDbyDlg(this, aControls);
	CDlgUtil::CtrlEnableDisable(this, aControls, m_bImpactFactor);

	GetDlgItem(IDC_CMD_MVCT_CASE2_L1_EDT  )->EnableWindow(m_bImpactFactor && m_bCase2);
	GetDlgItem(IDC_CMD_MVCT_CASE2_F1_EDT  )->EnableWindow(m_bImpactFactor && m_bCase2);
	GetDlgItem(IDC_CMD_MVCT_CASE2_L2_EDT  )->EnableWindow(m_bImpactFactor && m_bCase2);
	GetDlgItem(IDC_CMD_MVCT_CASE2_F2_EDT  )->EnableWindow(m_bImpactFactor && m_bCase2);
	GetDlgItem(IDC_CMD_MVCT_CASE2_GRUP_CMB)->EnableWindow(m_bImpactFactor && m_bCase2);
}

void CCMMvctCHRCPage::InitUnit()
{
	m_edtCase1L1.SetUnitType(D_UNITSYS_NONE);
	m_edtCase1F1.SetUnitType(D_UNITSYS_NONE);
	m_edtCase1L2.SetUnitType(D_UNITSYS_NONE);
	m_edtCase1F2.SetUnitType(D_UNITSYS_NONE);
	m_edtCase2L1.SetUnitType(D_UNITSYS_NONE);
	m_edtCase2F1.SetUnitType(D_UNITSYS_NONE);
	m_edtCase2L2.SetUnitType(D_UNITSYS_NONE);
	m_edtCase2F2.SetUnitType(D_UNITSYS_NONE);
}

void CCMMvctCHRCPage::InitCombo()
{
	CArray<T_GRUP_K, T_GRUP_K> aGrupK;
	T_GRUP_D GrupD;;	

	m_pDoc->m_pAttrCtrl->GetGrupKeyList(aGrupK);

	for(int i=0; i<aGrupK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetGrup(aGrupK[i], GrupD)) continue;

		m_cmbGrup.AddString(GrupD.GroupName);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHRCPage message handlers
BOOL CCMMvctCHRCPage::OnInitDialog() 
{
	CInternationalDlg::OnInitDialog();
	
	InitUnit();
	InitCombo();

	SetData2Dlg();

	EnableDisableControls(m_bImpactFactor);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvctCHRCPage::OnCmdMvchCHRCCase2Chk()
{
	EnableDisableControls(m_bImpactFactor);
}
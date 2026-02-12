// CMMvctCHStlPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvctCHStlPage.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMMvctCHStlPage dialog

CCMMvctCHStlPage::CCMMvctCHStlPage(T_BRDG_STL* pData, CWnd* pParent /*=NULL*/)
	: CInternationalDlg(CCMMvctCHStlPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvctCHStlPage)
	//}}AFX_DATA_INIT

	m_pData = NULL;
	m_pData = pData;

	m_bImpactFactor = TRUE;

	m_bCase2 = FALSE;

	m_pDoc = CDBDoc::GetDocPoint();
}

void CCMMvctCHStlPage::DoDataExchange(CDataExchange* pDX)
{
	CInternationalDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvctCHStlPage)	
	DDX_Control(pDX, IDC_CMD_MVCT_CASE1_U_EDT,      m_edtCase1_U);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE1_L_EDT,      m_edtCase1_L);
	DDX_Check  (pDX, IDC_CMD_MVCT_CASE2_CHK,        m_bCase2);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE2_U_EDT,      m_edtCase2_U);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE2_L_EDT,      m_edtCase2_L);
	DDX_Control(pDX, IDC_CMD_MVCT_CASE2_GRUP_CMB,   m_cmbGrup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvctCHStlPage, CInternationalDlg)
	//{{AFX_MSG_MAP(CCMMvctCHStlPage)
	ON_BN_CLICKED(IDC_CMD_MVCT_CASE2_CHK, OnCmdMvchCHStlCase2Chk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvctCHStlPage::SetData2Dlg()
{
	m_edtCase1_U.SetEditUnit(m_pData->dCase1_U);
	m_edtCase1_L.SetEditUnit(m_pData->dCase1_L);
	m_bCase2 = m_pData->bCase2;
	m_edtCase2_U.SetEditUnit(m_pData->dCase2_U);
	m_edtCase2_L.SetEditUnit(m_pData->dCase2_L);

	T_GRUP_D GrupD; GrupD.Initialize();
	m_pDoc->m_pAttrCtrl->GetGrup(m_pData->GrupK, GrupD);
	m_cmbGrup.SelectString(0, GrupD.GroupName);

	UpdateData(FALSE);
}

BOOL CCMMvctCHStlPage::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->dCase1_U = m_edtCase1_U.GetEditValue();
	m_pData->dCase1_L = m_edtCase1_L.GetEditValue();
	m_pData->bCase2 = m_bCase2;
	m_pData->dCase2_U = m_edtCase2_U.GetEditValue();
	m_pData->dCase2_L = m_edtCase2_L.GetEditValue();

	if(m_bCase2 && m_cmbGrup.GetCurSel() != -1)
	{
		CString strGrup;
		m_cmbGrup.GetLBText(m_cmbGrup.GetCurSel(), strGrup);
		m_pData->GrupK = m_pDoc->m_pAttrCtrl->GetGrupKey(strGrup);
	}

	return TRUE;
}

void CCMMvctCHStlPage::EnableDisableControls(BOOL bImpactFactor)
{
	UpdateData(TRUE);

	m_bImpactFactor = bImpactFactor;

	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	CDlgUtil::GetCtrlAllIDbyDlg(this, aControls);
	CDlgUtil::CtrlEnableDisable(this, aControls, m_bImpactFactor);

	GetDlgItem(IDC_CMD_MVCT_CASE2_U_EDT   )->EnableWindow(m_bImpactFactor && m_bCase2);
	GetDlgItem(IDC_CMD_MVCT_CASE2_L_EDT   )->EnableWindow(m_bImpactFactor && m_bCase2);
	GetDlgItem(IDC_CMD_MVCT_CASE2_GRUP_CMB)->EnableWindow(m_bImpactFactor && m_bCase2);	
}

void CCMMvctCHStlPage::InitUnit()
{
	m_edtCase1_U.SetUnitType(D_UNITSYS_NONE);
	m_edtCase1_L.SetUnitType(D_UNITSYS_NONE);
	m_edtCase2_U.SetUnitType(D_UNITSYS_NONE);
	m_edtCase2_L.SetUnitType(D_UNITSYS_NONE);
}

void CCMMvctCHStlPage::InitCombo()
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
// CCMMvctCHStlPage message handlers
BOOL CCMMvctCHStlPage::OnInitDialog() 
{
	CInternationalDlg::OnInitDialog();
	
	InitUnit();
	InitCombo();

	SetData2Dlg();

	EnableDisableControls(m_bImpactFactor);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvctCHStlPage::OnCmdMvchCHStlCase2Chk()
{
	EnableDisableControls(m_bImpactFactor);
}

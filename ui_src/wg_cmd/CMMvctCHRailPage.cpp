// CMMvctCHRailPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvctCHRailPage.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMMvctCHRailPage dialog

CCMMvctCHRailPage::CCMMvctCHRailPage(T_BRDG_RAIL* pData, int nBrgType, CWnd* pParent /*=NULL*/)
	: CInternationalDlg(CCMMvctCHRailPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvctCHRailPage)
	m_bLfai = FALSE;
	m_bLength = FALSE;
	//}}AFX_DATA_INIT

	m_pData = NULL;
	m_pData = pData;

	m_nBrgType = nBrgType;
	m_nSpanType = 0;
	m_bImpactFactor = TRUE;

	m_pDoc = CDBDoc::GetDocPoint();

	m_aMuReduc.RemoveAll();
	m_aMuReduc.Add(IDC_CMD_MVCT_MUR_STC1);
	m_aMuReduc.Add(IDC_CMD_MVCT_MUR_STC2);
	m_aMuReduc.Add(IDC_CMD_MVCT_MUR_EDT1);
	m_aMuReduc.Add(IDC_CMD_MVCT_MUR_STC3);
	m_aMuReduc.Add(IDC_CMD_MVCT_MUR_EDT2);
	m_aMuReduc.Add(IDC_CMD_MVCT_MUR_STC4);
	m_aMuReduc.Add(IDC_CMD_MVCT_MUR_STC5);
	m_aMuReduc.Add(IDC_CMD_MVCT_MUR_EDT3);
	m_aMuReduc.Add(IDC_CMD_MVCT_HC_STC);
	m_aMuReduc.Add(IDC_CMD_MVCT_HC_EDT);
	m_aMuReduc.Add(IDC_CMD_MVCT_HC_UNT);
}

void CCMMvctCHRailPage::DoDataExchange(CDataExchange* pDX)
{
	CInternationalDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvctCHRailPage)
	DDX_Control(pDX,  IDC_CMD_MVCT_MU_EDT1,         m_edtMu1);
	DDX_Control(pDX,  IDC_CMD_MVCT_MU_EDT2,         m_edtMu2);
	DDX_Control(pDX,  IDC_CMD_MVCT_MU_EDT3,         m_edtMu3);
	DDX_Check  (pDX,  IDC_CMD_MVCT_LFAI_CHK,        m_bLfai);
	DDX_Control(pDX,  IDC_CMD_MVCT_LFAI_EDT,        m_edtLfai);
	DDX_Check  (pDX,  IDC_CMD_MVCT_LENGTH_CHK,      m_bLength);
	DDX_Control(pDX,  IDC_CMD_MVCT_LENGTH_GRP_CMB,  m_cmbLengthGrup);
	DDX_Control(pDX,  IDC_CMD_MVCT_MUR_EDT1,        m_edtMuR1);
	DDX_Control(pDX,  IDC_CMD_MVCT_MUR_EDT2,        m_edtMuR2);
	DDX_Control(pDX,  IDC_CMD_MVCT_MUR_EDT3,        m_edtMuR3);
	DDX_Control(pDX,  IDC_CMD_MVCT_HC_EDT,          m_edthc);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvctCHRailPage, CInternationalDlg)
	//{{AFX_MSG_MAP(CCMMvctCHRailPage)
	ON_BN_CLICKED(IDC_CMD_MVCT_LFAI_CHK, OnCmdMvchCHRailLfaiChk)
	ON_BN_CLICKED(IDC_CMD_MVCT_LENGTH_CHK, OnCmdMvchCHRailLengthChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvctCHRailPage::SetData2Dlg()
{
	m_edtMu1.SetEditUnit(m_pData->dMu1);
	m_edtMu2.SetEditUnit(m_pData->dMu2);
	m_edtMu3.SetEditUnit(m_pData->dMu3);
	m_bLfai = m_pData->bLfai;
	m_edtLfai.SetEditUnit(m_pData->dLfai);
	m_bLength = m_pData->bLength;
	
	T_GRUP_D GrupD; GrupD.Initialize();
	m_pDoc->m_pAttrCtrl->GetGrup(m_pData->LengthGrupK, GrupD);
	m_cmbLengthGrup.SelectString(0, GrupD.GroupName);
	
	m_edtMuR1.SetEditUnit(m_pData->dMuR1);
	m_edtMuR2.SetEditUnit(m_pData->dMuR2);
	m_edtMuR3.SetEditUnit(m_pData->dMuR3);
	m_edthc.SetEditUnit(m_pData->dhc);
		
	UpdateData(FALSE);
}

BOOL CCMMvctCHRailPage::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->dMu1 = m_edtMu1.GetEditValue();
	m_pData->dMu2 = m_edtMu2.GetEditValue();
	m_pData->dMu3 = m_edtMu3.GetEditValue();
	m_pData->bLfai = m_bLfai;
	m_pData->dLfai = m_edtLfai.GetEditValue();
	m_pData->bLength = m_bLength;

	if(m_bLength && m_cmbLengthGrup.GetCurSel() != -1)
	{
		CString strGrup;
		m_cmbLengthGrup.GetLBText(m_cmbLengthGrup.GetCurSel(), strGrup);
		m_pData->LengthGrupK = m_pDoc->m_pAttrCtrl->GetGrupKey(strGrup);
	}
	
	m_pData->dMuR1 = m_edtMuR1.GetEditValue();
	m_pData->dMuR2 = m_edtMuR2.GetEditValue();
	m_pData->dMuR3 = m_edtMuR3.GetEditValue();
	m_pData->dhc = m_edthc.GetEditValue();
		
	return TRUE;
}

void CCMMvctCHRailPage::EnableDisableControls(BOOL bImpactFactor, int nSpanType)
{
	UpdateData(TRUE);

	m_bImpactFactor = bImpactFactor;
	m_nSpanType = nSpanType;

	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	CDlgUtil::GetCtrlAllIDbyDlg(this, aControls);
	CDlgUtil::CtrlEnableDisable(this, aControls, m_bImpactFactor);

	GetDlgItem(IDC_CMD_MVCT_LFAI_STC)->EnableWindow(m_bImpactFactor && !m_bLfai);
	GetDlgItem(IDC_CMD_MVCT_LFAI_EDT)->EnableWindow(m_bImpactFactor && !m_bLfai);
	GetDlgItem(IDC_CMD_MVCT_LFAI_UNT)->EnableWindow(m_bImpactFactor && !m_bLfai);

	GetDlgItem(IDC_CMD_MVCT_LENGTH_CHK    )->EnableWindow(m_bImpactFactor && nSpanType == 0 && m_bLfai);	
	GetDlgItem(IDC_CMD_MVCT_LENGTH_GRP_STC)->EnableWindow(m_bImpactFactor && nSpanType == 0 && m_bLfai && m_bLength);
	GetDlgItem(IDC_CMD_MVCT_LENGTH_GRP_CMB)->EnableWindow(m_bImpactFactor && nSpanType == 0 && m_bLfai && m_bLength);
}

void CCMMvctCHRailPage::InitUnit()
{
	m_edtMu1.SetUnitType(D_UNITSYS_NONE);
	m_edtMu2.SetUnitType(D_UNITSYS_NONE);
	m_edtMu3.SetUnitType(D_UNITSYS_NONE);
	m_edtLfai.SetUnitType(D_UNITSYS_NONE);
	m_edtMuR1.SetUnitType(D_UNITSYS_NONE);
	m_edtMuR2.SetUnitType(D_UNITSYS_NONE);
	m_edtMuR3.SetUnitType(D_UNITSYS_NONE);
	m_edthc.SetUnitType(D_UNITSYS_NONE);
}

void CCMMvctCHRailPage::InitCombo()
{
	CArray<T_GRUP_K, T_GRUP_K> aGrupK;
	T_GRUP_D GrupD;;	

	m_pDoc->m_pAttrCtrl->GetGrupKeyList(aGrupK);

	for(int i=0; i<aGrupK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetGrup(aGrupK[i], GrupD)) continue;

		m_cmbLengthGrup.AddString(GrupD.GroupName);
	}
}

void CCMMvctCHRailPage::ControlsShowHide()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlShowHide(this, m_aMuReduc, m_nBrgType==2);
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHRailPage message handlers
BOOL CCMMvctCHRailPage::OnInitDialog() 
{
	CInternationalDlg::OnInitDialog();
	
	InitUnit();
	InitCombo();

	SetData2Dlg();

	ControlsShowHide();
	EnableDisableControls(m_bImpactFactor, m_nSpanType);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvctCHRailPage::OnCmdMvchCHRailLfaiChk()
{
	EnableDisableControls(m_bImpactFactor, m_nSpanType);
}

void CCMMvctCHRailPage::OnCmdMvchCHRailLengthChk()
{
	EnableDisableControls(m_bImpactFactor, m_nSpanType);
}

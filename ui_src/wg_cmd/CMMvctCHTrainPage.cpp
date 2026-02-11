// CMMvctCHTrainPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvctCHTrainPage.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMMvctCHTrainPage dialog

CCMMvctCHTrainPage::CCMMvctCHTrainPage(T_BRDG_TRAIN* pData, int nCodeType, CWnd* pParent /*=NULL*/)
	: CInternationalDlg(CCMMvctCHTrainPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvctCHTrainPage)
	m_nMethod = 0;
	m_bConcLoadedLength = FALSE;
	m_bArchLoadedLength = FALSE;
	//}}AFX_DATA_INIT

	m_pData = NULL;
	m_pData = pData;
	
	m_nCodeType = nCodeType;

	if(m_nCodeType == 2)
	{
		if(m_pData->dConc_h < 0.5) m_pData->dConc_h = 0.5;
	}

	m_nSpanType = 0;
	m_bImpactFactor = TRUE;

	m_pDoc = CDBDoc::GetDocPoint();

	m_aSimple.RemoveAll();
	m_aSimple.Add(IDC_CMD_MVCT_SIMPLE_U_EDT);
	m_aSimple.Add(IDC_CMD_MVCT_SIMPLE_STC1);
	m_aSimple.Add(IDC_CMD_MVCT_SIMPLE_L_EDT);
	m_aSimple.Add(IDC_CMD_MVCT_SIMPLE_STC2);

	m_aCompo.RemoveAll();
	m_aCompo.Add(IDC_CMD_MVCT_COMPO_U_EDT);
	m_aCompo.Add(IDC_CMD_MVCT_COMPO_STC1);
	m_aCompo.Add(IDC_CMD_MVCT_COMPO_L_EDT);
	m_aCompo.Add(IDC_CMD_MVCT_COMPO_STC2);

	m_aConc.RemoveAll();
	m_aConc.Add(IDC_CMD_MVCT_CONC_STC1);
	m_aConc.Add(IDC_CMD_MVCT_CONC_U_EDT);
	m_aConc.Add(IDC_CMD_MVCT_CONC_STC2);
	m_aConc.Add(IDC_CMD_MVCT_CONC_L_EDT);
	m_aConc.Add(IDC_CMD_MVCT_CONC_STC3);
	m_aConc.Add(IDC_CMD_MVCT_CONC_H_STC);
	m_aConc.Add(IDC_CMD_MVCT_CONC_H_EDT);
	m_aConc.Add(IDC_CMD_MVCT_CONC_H_UNT);
	m_aConc.Add(IDC_CMD_MVCT_CONC_L_CHK);
	m_aConc.Add(IDC_CMD_MVCT_CONC_GRUP_STC);
	m_aConc.Add(IDC_CMD_MVCT_CONC_GRUP_CMB);

	m_aArch.RemoveAll();
	m_aArch.Add(IDC_CMD_MVCT_ARCH_U_EDT);
	m_aArch.Add(IDC_CMD_MVCT_ARCH_STC1);
	m_aArch.Add(IDC_CMD_MVCT_ARCH_STC2);
	m_aArch.Add(IDC_CMD_MVCT_ARCH_STC3);
	m_aArch.Add(IDC_CMD_MVCT_ARCH_L_EDT);
	m_aArch.Add(IDC_CMD_MVCT_ARCH_STC4);
	m_aArch.Add(IDC_CMD_MVCT_ARCH_LAMBDA_STC);
	m_aArch.Add(IDC_CMD_MVCT_ARCH_LAMBDA_EDT);
	m_aArch.Add(IDC_CMD_MVCT_ARCH_F_STC);
	m_aArch.Add(IDC_CMD_MVCT_ARCH_F_EDT);
	m_aArch.Add(IDC_CMD_MVCT_ARCH_L_CHK);
	m_aArch.Add(IDC_CMD_MVCT_ARCH_GRUP_STC);
	m_aArch.Add(IDC_CMD_MVCT_ARCH_GRUP_CMB);
}

void CCMMvctCHTrainPage::DoDataExchange(CDataExchange* pDX)
{
	CInternationalDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvctCHTrainPage)
	DDX_Radio  (pDX, IDC_CMD_MVCT_METHOD_RDO1,      m_nMethod);
	DDX_Control(pDX, IDC_CMD_MVCT_SIMPLE_U_EDT,     m_edtSimple_U);
	DDX_Control(pDX, IDC_CMD_MVCT_SIMPLE_L_EDT,     m_edtSimple_L);
	DDX_Control(pDX, IDC_CMD_MVCT_COMPO_U_EDT,      m_edtCompo_U);
	DDX_Control(pDX, IDC_CMD_MVCT_COMPO_L_EDT,      m_edtCompo_L);
	DDX_Control(pDX, IDC_CMD_MVCT_CONC_U_EDT,       m_edtConc_U);
	DDX_Control(pDX, IDC_CMD_MVCT_CONC_L_EDT,       m_edtConc_L);
	DDX_Control(pDX, IDC_CMD_MVCT_CONC_H_EDT,       m_edtConc_h);
	DDX_Check  (pDX, IDC_CMD_MVCT_CONC_L_CHK,       m_bConcLoadedLength);
	DDX_Control(pDX, IDC_CMD_MVCT_CONC_GRUP_CMB,    m_cmbConcGrup);
	DDX_Control(pDX, IDC_CMD_MVCT_ARCH_U_EDT,       m_edtArch_U);
	DDX_Control(pDX, IDC_CMD_MVCT_ARCH_L_EDT,       m_edtArch_L);
	DDX_Control(pDX, IDC_CMD_MVCT_ARCH_LAMBDA_EDT,  m_edtArchLambda);
	DDX_Control(pDX, IDC_CMD_MVCT_ARCH_F_EDT,       m_edtArch_f);
	DDX_Check  (pDX, IDC_CMD_MVCT_ARCH_L_CHK,       m_bArchLoadedLength);
	DDX_Control(pDX, IDC_CMD_MVCT_ARCH_GRUP_CMB,    m_cmbArchGrup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvctCHTrainPage, CInternationalDlg)
	//{{AFX_MSG_MAP(CCMMvctCHTrainPage)
	ON_BN_CLICKED(IDC_CMD_MVCT_METHOD_RDO1, OnCmdMvchCHTrainMethodRdo)
	ON_BN_CLICKED(IDC_CMD_MVCT_METHOD_RDO2, OnCmdMvchCHTrainMethodRdo)
	ON_BN_CLICKED(IDC_CMD_MVCT_METHOD_RDO3, OnCmdMvchCHTrainMethodRdo)
	ON_BN_CLICKED(IDC_CMD_MVCT_METHOD_RDO4, OnCmdMvchCHTrainMethodRdo)
	ON_BN_CLICKED(IDC_CMD_MVCT_CONC_L_CHK,  OnCmdMvchCHTrainConcLChk)
	ON_BN_CLICKED(IDC_CMD_MVCT_ARCH_L_CHK,  OnCmdMvchCHTrainArchLChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvctCHTrainPage::SetData2Dlg()
{
	m_nMethod = m_pData->nMethod;
	m_edtSimple_U.SetEditUnit(m_pData->dSimple_U);
	m_edtSimple_L.SetEditUnit(m_pData->dSimple_L);
	m_edtCompo_U.SetEditUnit(m_pData->dCompo_U);
	m_edtCompo_L.SetEditUnit(m_pData->dCompo_L);
	m_edtConc_U.SetEditUnit(m_pData->dConc_U);
	m_edtConc_L.SetEditUnit(m_pData->dConc_L);
	m_edtConc_h.SetEditUnit(m_pData->dConc_h);
	m_bConcLoadedLength = m_pData->bConcLoadedLength;
	
	T_GRUP_D GrupD; GrupD.Initialize();
	m_pDoc->m_pAttrCtrl->GetGrup(m_pData->ConcGrupK, GrupD);
	m_cmbConcGrup.SelectString(0, GrupD.GroupName);

	m_edtArch_U.SetEditUnit(m_pData->dArch_U);
	m_edtArch_L.SetEditUnit(m_pData->dArch_L);
	m_edtArchLambda.SetEditUnit(m_pData->dArchLambda);
	m_edtArch_f.SetEditUnit(m_pData->dArch_f);	
	m_bArchLoadedLength = m_pData->bArchLoadedLength;
	
	GrupD.Initialize();
	m_pDoc->m_pAttrCtrl->GetGrup(m_pData->ArchGrupK, GrupD);
	m_cmbArchGrup.SelectString(0, GrupD.GroupName);

	UpdateData(FALSE);
}

BOOL CCMMvctCHTrainPage::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->nMethod = m_nMethod;
	m_pData->dSimple_U = m_edtSimple_U.GetEditValue();
	m_pData->dSimple_L = m_edtSimple_L.GetEditValue();
	m_pData->dCompo_U = m_edtCompo_U.GetEditValue();
	m_pData->dCompo_L = m_edtCompo_L.GetEditValue();
	m_pData->dConc_U = m_edtConc_U.GetEditValue();
	m_pData->dConc_L = m_edtConc_L.GetEditValue();
	m_pData->dConc_h = m_edtConc_h.GetEditValue();
	m_pData->bConcLoadedLength = m_bConcLoadedLength;

	CString strGrup;
	if(m_bConcLoadedLength && m_cmbConcGrup.GetCurSel() != -1)
	{
		m_cmbConcGrup.GetLBText(m_cmbConcGrup.GetCurSel(), strGrup);
		m_pData->ConcGrupK = m_pDoc->m_pAttrCtrl->GetGrupKey(strGrup);
	}
	
	m_pData->dArch_U = m_edtArch_U.GetEditValue();
	m_pData->dArch_L = m_edtArch_L.GetEditValue();
	m_pData->dArchLambda = m_edtArchLambda.GetEditValue();
	m_pData->dArch_f = m_edtArch_f.GetEditValue();
	m_pData->bArchLoadedLength = m_bArchLoadedLength;

	if(m_bArchLoadedLength && m_cmbArchGrup.GetCurSel() != -1)
	{
		m_cmbArchGrup.GetLBText(m_cmbArchGrup.GetCurSel(), strGrup);
		m_pData->ArchGrupK = m_pDoc->m_pAttrCtrl->GetGrupKey(strGrup);
	}
	
	return TRUE;
}

void CCMMvctCHTrainPage::EnableDisableControls(BOOL bImpactFactor, int nSpanType)
{
	UpdateData(TRUE);

	m_bImpactFactor = bImpactFactor;
	m_nSpanType = nSpanType;

	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	CDlgUtil::GetCtrlAllIDbyDlg(this, aControls);
	CDlgUtil::CtrlEnableDisable(this, aControls, m_bImpactFactor);

	GetDlgItem(IDC_CMD_MVCT_CONC_L_CHK   )->EnableWindow(m_bImpactFactor && nSpanType == 0);
	GetDlgItem(IDC_CMD_MVCT_CONC_GRUP_STC)->EnableWindow(m_bImpactFactor && nSpanType == 0 && m_bConcLoadedLength);
	GetDlgItem(IDC_CMD_MVCT_CONC_GRUP_CMB)->EnableWindow(m_bImpactFactor && nSpanType == 0 && m_bConcLoadedLength);

	GetDlgItem(IDC_CMD_MVCT_ARCH_L_CHK   )->EnableWindow(m_bImpactFactor && nSpanType == 0);
	GetDlgItem(IDC_CMD_MVCT_ARCH_GRUP_STC)->EnableWindow(m_bImpactFactor && nSpanType == 0 && m_bArchLoadedLength);
	GetDlgItem(IDC_CMD_MVCT_ARCH_GRUP_CMB)->EnableWindow(m_bImpactFactor && nSpanType == 0 && m_bArchLoadedLength);
}

void CCMMvctCHTrainPage::InitUnit()
{
	m_edtSimple_U.SetUnitType(D_UNITSYS_NONE);
	m_edtSimple_L.SetUnitType(D_UNITSYS_NONE);
	m_edtCompo_U.SetUnitType(D_UNITSYS_NONE);
	m_edtCompo_L.SetUnitType(D_UNITSYS_NONE);
	m_edtConc_U.SetUnitType(D_UNITSYS_NONE);
	m_edtConc_L.SetUnitType(D_UNITSYS_NONE);
	m_edtConc_h.SetUnitType(D_UNITSYS_NONE);
	m_edtArch_U.SetUnitType(D_UNITSYS_NONE);
	m_edtArch_L.SetUnitType(D_UNITSYS_NONE);
	m_edtArchLambda.SetUnitType(D_UNITSYS_NONE);
	m_edtArch_f.SetUnitType(D_UNITSYS_NONE);
}

void CCMMvctCHTrainPage::InitCombo()
{
	CArray<T_GRUP_K, T_GRUP_K> aGrupK;
	T_GRUP_D GrupD;;	

	m_pDoc->m_pAttrCtrl->GetGrupKeyList(aGrupK);

	for(int i=0; i<aGrupK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetGrup(aGrupK[i], GrupD)) continue;

		m_cmbConcGrup.AddString(GrupD.GroupName);
		m_cmbArchGrup.AddString(GrupD.GroupName);
	}
}

void CCMMvctCHTrainPage::AlignControl()
{
	CRect rTo, rMove;
	int nDistY;

	GetDlgItem(IDC_CMD_MVCT_ARCH_U_EDT)->GetWindowRect(rTo);
	GetDlgItem(IDC_CMD_MVCT_SIMPLE_U_EDT)->GetWindowRect(rMove);
	nDistY = rTo.top - rMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aSimple, nDistY);

	GetDlgItem(IDC_CMD_MVCT_COMPO_U_EDT)->GetWindowRect(rMove);
	nDistY = rTo.top - rMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCompo, nDistY);

	GetDlgItem(IDC_CMD_MVCT_CONC_U_EDT)->GetWindowRect(rMove);
	nDistY = rTo.top - rMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aConc, nDistY);
}

void CCMMvctCHTrainPage::ControlsShowHide()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlShowHide(this, m_aSimple, m_nMethod==0);
	CDlgUtil::CtrlShowHide(this, m_aCompo,  m_nMethod==1);
	CDlgUtil::CtrlShowHide(this, m_aConc,   m_nMethod==2);
	CDlgUtil::CtrlShowHide(this, m_aArch,   m_nMethod==3);
}

void CCMMvctCHTrainPage::ChangeText()
{
	if(m_nCodeType == 1)
	{
		GetDlgItem(IDC_CMD_MVCT_CONC_STC3)->SetWindowText(_T("+ L ),     a = 4 * ( 1 - h ) <= 2"));
	}
	else
	{
		GetDlgItem(IDC_CMD_MVCT_CONC_STC3)->SetWindowText(_T("+ L ),     a = 0.32 * ( 3 - h )^2"));
	}	
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHTrainPage message handlers
BOOL CCMMvctCHTrainPage::OnInitDialog() 
{
	CInternationalDlg::OnInitDialog();
	
	InitUnit();
	InitCombo();

	SetData2Dlg();

	AlignControl();
	ControlsShowHide();
	EnableDisableControls(m_bImpactFactor, m_nSpanType);

	ChangeText();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvctCHTrainPage::OnCmdMvchCHTrainMethodRdo()
{
	ControlsShowHide();
	EnableDisableControls(m_bImpactFactor, m_nSpanType);
}

void CCMMvctCHTrainPage::OnCmdMvchCHTrainConcLChk()
{
	EnableDisableControls(m_bImpactFactor, m_nSpanType);
}

void CCMMvctCHTrainPage::OnCmdMvchCHTrainArchLChk()
{
	EnableDisableControls(m_bImpactFactor, m_nSpanType);
}
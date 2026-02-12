// ETC_GDS_KS_BRG.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_KS_BRG.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "SpfcKSBRGUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KS_BRG dialog


CETC_GDS_KS_BRG::CETC_GDS_KS_BRG(T_SPFC_D* pParamData, BOOL bUseTs, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_KS_BRG::IDD, pParent, std::make_shared<CSpfcKSBRGUtil>())
{
	//{{AFX_DATA_INIT(CETC_GDS_KS_BRG)
	m_sIF = _T("1.0");
	m_sRMF = _T("1.0");
	m_nSoil = 0;
	m_nEA = 0;
	m_dMaxPeriod = PRD_ED;
	//}}AFX_DATA_INIT

	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcKSBRGUtil>(m_pSpfcBaseUtil);
	m_pParamData = pParamData;
	m_bUseTs = bUseTs;
	SetValue();
}


void CETC_GDS_KS_BRG::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_KS_BRG)
	DDX_Control(pDX, IDC_ETC_GDS_CMB_RMC, m_ctlRMF);
	DDX_Control(pDX, IDC_ETC_GDS_KSB_CMB_IF, m_ctlIF);
	DDX_CBString(pDX, IDC_ETC_GDS_KSB_CMB_IF, m_sIF);
	DDX_CBString(pDX, IDC_ETC_GDS_CMB_RMC, m_sRMF);
	DDX_Radio(pDX, IDC_ETC_GDS_KSB_RDO_SOIL1, m_nSoil);
	DDX_Radio(pDX, IDC_ETC_GDS_KSB_RDO_Area1, m_nEA);
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);	
	DDX_Control(pDX, IDC_CMD_TS_EDT,           m_edtTs);
	DDX_Control(pDX, IDC_CMD_TS_UNT,           m_untTs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_KS_BRG, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_KS_BRG)
	ON_BN_CLICKED(IDC_ETC_GDS_KSB_RDO_SOIL1, OnCmdKSBridgeSoilTypeRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_KSB_RDO_SOIL2, OnCmdKSBridgeSoilTypeRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_KSB_RDO_SOIL3, OnCmdKSBridgeSoilTypeRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_KSB_RDO_SOIL4, OnCmdKSBridgeSoilTypeRdo)

		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KS_BRG message handlers

BOOL CETC_GDS_KS_BRG::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();

	InitUnit();
	
	AlignControls();
	ShowHideControls();

	OnCmdKSBridgeSoilTypeRdo();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_KS_BRG::End()
{
	UpdateData(TRUE);
	SetValue();
	CMyChildDialog::End();
}

//-----------------------------------------------------------------------------------------------------
// INIT : 2003.09.20. by KYE-HONG
// FUNC : 계산알고리즘에 사용되는 Default값을 Setting함. 
//-----------------------------------------------------------------------------------------------------
void CETC_GDS_KS_BRG::SetValue()
{
	m_pSpfcUtil->SetOIF(_tstof(m_sIF));
	m_pSpfcUtil->SetRMC(_tstof(m_sRMF));

	if (m_nSoil == 0)      m_pSpfcUtil->SetSoil(1.0);
	else if (m_nSoil == 1) m_pSpfcUtil->SetSoil(1.2);
	else if (m_nSoil == 2) m_pSpfcUtil->SetSoil(1.5);
	else if (m_nSoil == 3) m_pSpfcUtil->SetSoil(2.0);

	if (m_nEA == 0)      m_pSpfcUtil->SetEPA(0.11);
	else if (m_nEA == 1) m_pSpfcUtil->SetEPA(0.07);

	if (m_nSoil == 0)      m_pSpfcUtil->SetTs(0.33);
	else if (m_nSoil == 1) m_pSpfcUtil->SetTs(0.44);
	else if (m_nSoil == 2) m_pSpfcUtil->SetTs(0.61);
	else if (m_nSoil == 3) m_pSpfcUtil->SetTs(0.94);

	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);
}

BOOL CETC_GDS_KS_BRG::ISVALID()
{ 
	if(!IsValidCmb_d(&m_ctlIF,0,10,1.0)) return FALSE;
	if(!IsValidCmb_d(&m_ctlRMF,0,50,1.0)) return FALSE;
	if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  
	return TRUE;  
}

void CETC_GDS_KS_BRG::InitUnit()
{
	m_edtTs.SetUnitType(D_UNITSYS_BASE_TIME);
	m_untTs.SetUnitType(D_UNITSYS_BASE_TIME);
}

void CETC_GDS_KS_BRG::AlignControls()
{
	CRect rTo, rMove;
	int nDistX = 0;
	int nDistY = 0;

	CArray<UINT, UINT> aPeriod; aPeriod.RemoveAll();
	CArray<UINT, UINT> aTs; aTs.RemoveAll();

	aPeriod.Add(IDC_CMD_PERIOD_TEXT);
	aPeriod.Add(IDC_CMD_PERIOD_EDIT);
	aPeriod.Add(IDC_CMD_PERIOD_UNIT);

	aTs.Add(IDC_CMD_TS_STC);
	aTs.Add(IDC_CMD_TS_EDT);
	aTs.Add(IDC_CMD_TS_UNT);

	GetDlgItem(IDC_CMD_PERIOD_EDIT)->GetWindowRect(rTo);
	GetDlgItem(IDC_CMD_TS_EDT     )->GetWindowRect(rMove);
	nDistX = rTo.left-rMove.left;
	nDistY = rTo.top-rMove.top;
	CDlgUtil::CtrlMoveDistXY(this, aTs, nDistX, nDistY);
}

void CETC_GDS_KS_BRG::ShowHideControls()
{
	CArray<UINT, UINT> aPeriod; aPeriod.RemoveAll();
	CArray<UINT, UINT> aTs; aTs.RemoveAll();

	aPeriod.Add(IDC_CMD_PERIOD_TEXT);
	aPeriod.Add(IDC_CMD_PERIOD_EDIT);
	aPeriod.Add(IDC_CMD_PERIOD_UNIT);

	aTs.Add(IDC_CMD_TS_STC);
	aTs.Add(IDC_CMD_TS_EDT);
	aTs.Add(IDC_CMD_TS_UNT);

 	CDlgUtil::CtrlShowHide(this, aPeriod, !m_bUseTs);
 	CDlgUtil::CtrlShowHide(this, aTs, m_bUseTs);
}

double CETC_GDS_KS_BRG::GetTs() const
{
	return m_pSpfcUtil->GetTs();
}

BOOL CETC_GDS_KS_BRG::Dlg2Data()
{
	m_pParamData->CodeParam.KSBRG.dSoil = m_pSpfcUtil->GetSoil();
	m_pParamData->CodeParam.KSBRG.dEPA  = m_pSpfcUtil->GetEPA();
	m_pParamData->CodeParam.KSBRG.dIe   = m_pSpfcUtil->GetOIF();
	m_pParamData->CodeParam.KSBRG.dCoef = m_pSpfcUtil->GetRMC();
	m_pParamData->CodeParam.KSBRG.dMaxPeriod = m_dMaxPeriod;

	m_pParamData->dResModCoef = m_pSpfcUtil->GetRMC();
	return TRUE;
}
void CETC_GDS_KS_BRG::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	m_pSpfcUtil->SetSoil(m_pParamData->CodeParam.KSBRG.dSoil);
	m_pSpfcUtil->SetEPA(m_pParamData->CodeParam.KSBRG.dEPA);
	m_pSpfcUtil->SetOIF(m_pParamData->CodeParam.KSBRG.dIe);
	m_pSpfcUtil->SetRMC(m_pParamData->CodeParam.KSBRG.dCoef);
	m_dMaxPeriod = m_pParamData->CodeParam.KSBRG.dMaxPeriod;

	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);

	if     (m_pSpfcUtil->GetSoil() == 1.0) m_nSoil = 0;
	else if (m_pSpfcUtil->GetSoil() == 1.2) m_nSoil = 1;
	else if (m_pSpfcUtil->GetSoil() == 1.5) m_nSoil = 2;
	else if (m_pSpfcUtil->GetSoil() == 2.0) m_nSoil = 3;
	else { ASSERT(0); m_nSoil = 0; }

	if     (m_pSpfcUtil->GetEPA() == 0.11) m_nEA = 0;
	else if (m_pSpfcUtil->GetEPA() == 0.07) m_nEA = 1;
	else { ASSERT(0); m_nEA = 0; }

	if(bInit)
	{
		UpdateData(FALSE);

		CString strI, strR;
		strI.Format(_T("%g"), m_pSpfcUtil->GetOIF());
		strR.Format(_T("%g"), m_pSpfcUtil->GetRMC());
		m_ctlIF.SetWindowText(strI);    
		m_ctlRMF.SetWindowText(strR);
		m_ctlRMF.EnableWindow(FALSE);
	}
}

void CETC_GDS_KS_BRG::OnCmdKSBridgeSoilTypeRdo()
{
	UpdateData(TRUE);

	if     (m_nSoil == 0) m_pSpfcUtil->SetTs(0.33);
	else if (m_nSoil == 1) m_pSpfcUtil->SetTs(0.44);
	else if (m_nSoil == 2) m_pSpfcUtil->SetTs(0.61);
	else if (m_nSoil == 3) m_pSpfcUtil->SetTs(0.94);

	m_edtTs.SetEditUnit(m_pSpfcUtil->GetTs());
}

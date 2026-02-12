// ETC_GDS_P100_2013.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_P100_2013.h"

#include "SpfcPO2013Util.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_LateralLoad.h"
#include "..\wg_base\DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_P100_2013 dialog


CETC_GDS_P100_2013::CETC_GDS_P100_2013(T_SPFC_D* pParamData/*=NULL*/, CWnd* pParent /*=NULL*/, int nCase/*=0*/)
	: CMyChildDialog(CETC_GDS_P100_2013::IDD, pParent, std::make_shared<CSpfcPO2013Util>())
{
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcPO2013Util>(m_pSpfcBaseUtil);

	//{{AFX_DATA_INIT(CETC_GDS_P100_2013)
	m_nParaTypeRdo = D_SPFC_P100_2013_TC07S;
	m_dMaxPeriod = PRD_ED;
	//}}AFX_DATA_INIT
	//m_aCtrlElastic.Add(IDC_ETC_GDS_P100_2013_ACC_TEXT);
	m_nCase = nCase;

}


void CETC_GDS_P100_2013::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_P100_2013)
	DDX_Control(pDX, IDC_ETC_GDS_P100_2013_TD_EDIT, m_wndTd);
	DDX_Control(pDX, IDC_ETC_GDS_P100_2013_TC_EDIT, m_wndTc);
	DDX_Control(pDX, IDC_ETC_GDS_P100_2013_TB_EDIT, m_wndTb);
	DDX_Control(pDX, IDC_ETC_GDS_P100_2013_SPEC_TYPE_CMB, m_SpecTypeCmb);
	DDX_Control(pDX, IDC_ETC_GDS_P100_2013_IMPORTANCE_CMB, m_cmbImportance);
	DDX_Control(pDX, IDC_ETC_GDS_P100_2013_DAMPING_EDIT, m_wndDamping);
	DDX_Control(pDX, IDC_ETC_GDS_P100_2013_BEHAVIOR_EDIT, m_wndQ);
	DDX_Control(pDX, IDC_ETC_GDS_P100_2013_AG_EDIT, m_wndAg);
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);	
	DDX_Radio(pDX, IDC_ETC_GDS_P100_2013_PARATYPE_RDO, m_nParaTypeRdo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_P100_2013, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_P100_2013)
	ON_CBN_SELCHANGE(IDC_ETC_GDS_P100_2013_SPEC_TYPE_CMB, OnSelchangeEtcGdsP100_2013SpecTypeCmb)
	ON_BN_CLICKED(IDC_ETC_GDS_P100_2013_PARATYPE_RDO, OnEtcGdsP100_2013ParatypeRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_P100_2013_PARATYPE_RDO2, OnEtcGdsP100_2013ParatypeRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_P100_2013_PARATYPE_RDO3, OnEtcGdsP100_2013ParatypeRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_P100_2013_PARATYPE_RDO4, OnEtcGdsP100_2013ParatypeRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_P100_2013 message handlers

BOOL CETC_GDS_P100_2013::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();

	m_aCtrlElastic.RemoveAll();
	m_aCtrlElastic.Add(IDC_ETC_GDS_P100_2013_DAMPING_TEXT);
	m_aCtrlElastic.Add(IDC_ETC_GDS_P100_2013_DAMPING_EDIT);
	m_aCtrlElastic.Add(IDC_ETC_GDS_P100_2013_DAMPING_UNIT);

	m_aCtrlDesign.RemoveAll();
	m_aCtrlDesign.Add(IDC_ETC_GDS_P100_2013_BEHAVIOR_TEXT);
	m_aCtrlDesign.Add(IDC_ETC_GDS_P100_2013_BEHAVIOR_EDIT);

	m_wndTb.SetEditUnit(m_pSpfcUtil->GetTb());
	m_wndTc.SetEditUnit(m_pSpfcUtil->GetTc());
	m_wndTd.SetEditUnit(m_pSpfcUtil->GetTd());

	m_wndAg.SetEditUnit(m_pSpfcUtil->GetAg());
	m_wndDamping.SetEditUnit(m_pSpfcUtil->GetDamping());
	m_wndQ.SetEditUnit(m_pSpfcUtil->GetQ());

	InitCtrl();
	AlignControl();  
	CtrlManager();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_P100_2013::InitSpectrumTypeCombo()
{
	m_SpecTypeCmb.ResetContent();
	if(m_nCase != 1)
	{
		CDlgUtil::CobxAddItem(m_SpecTypeCmb, _LS(IDS_CMD_EURO2003_H_ELASTIC_SPECTRUM), D_SPFC_P100_2013_SP_HE);
		CDlgUtil::CobxAddItem(m_SpecTypeCmb, _LS(IDS_CMD_EURO2003_V_ELASTIC_SPECTRUM), D_SPFC_P100_2013_SP_VE);
	}
	if(m_nCase !=2 )
	{
		CDlgUtil::CobxAddItem(m_SpecTypeCmb, _LS(IDS_CMD_EURO2003_H_DESIGN_SPECTRUM), D_SPFC_P100_2013_SP_HD);
		CDlgUtil::CobxAddItem(m_SpecTypeCmb, _LS(IDS_CMD_EURO2003_V_DESIGN_SPECTRUM), D_SPFC_P100_2013_SP_VD);
	}
	m_SpecTypeCmb.SetCurSel(0);
	CDlgUtil::CobxAdjustListBoxWidth(m_SpecTypeCmb);
}

BOOL CETC_GDS_P100_2013::Dlg2Data()
{
	if(m_pParamData == NULL)
		return FALSE; 

	// UpdateData(TRUE); // CETC_DESIGN_SPECT에서 Dlg2Data()호출 전에 이미 호출되었다.
	m_pParamData->CodeParam.P100_2013.Initialize();
	m_pParamData->nSpecCodeType = D_SPFC_CODE_P100_2013;
	m_pParamData->CodeParam.P100_2013.nSpectrumType  = m_pSpfcUtil->GetSpecType();

	// Input Data
	m_pParamData->CodeParam.P100_2013.nParameterType = m_nParaTypeRdo;
	m_pParamData->CodeParam.P100_2013.dTb            = m_pSpfcUtil->GetTb();
	m_pParamData->CodeParam.P100_2013.dTc            = m_pSpfcUtil->GetTc();  
	m_pParamData->CodeParam.P100_2013.dTd            = m_pSpfcUtil->GetTd();
	m_pParamData->CodeParam.P100_2013.dAgr           = m_pSpfcUtil->GetAg();
	m_pParamData->CodeParam.P100_2013.dI             = m_pSpfcUtil->GetImportance();
	m_pParamData->CodeParam.P100_2013.dXi            = m_pSpfcUtil->GetDamping();
	m_pParamData->CodeParam.P100_2013.dQ             = m_pSpfcUtil->GetQ();
	m_pParamData->CodeParam.P100_2013.dMaxPeriod     = m_dMaxPeriod;  

	return TRUE;
}

void CETC_GDS_P100_2013::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	if(m_pParamData == NULL)
		return;

	if(m_pParamData->nSpecCodeType!=D_SPFC_CODE_P100_2013)
	{
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_P100_2013);
	}

	// Input Data
	m_pSpfcUtil->SetSpecType(m_pParamData->CodeParam.P100_2013.nSpectrumType);
	m_nParaTypeRdo = m_pParamData->CodeParam.P100_2013.nParameterType;
	m_pSpfcUtil->SetTb(m_pParamData->CodeParam.P100_2013.dTb);
	m_pSpfcUtil->SetTc(m_pParamData->CodeParam.P100_2013.dTc);
	m_pSpfcUtil->SetTd(m_pParamData->CodeParam.P100_2013.dTd);
	m_pSpfcUtil->SetAg(m_pParamData->CodeParam.P100_2013.dAgr);
	m_pSpfcUtil->SetImportance(m_pParamData->CodeParam.P100_2013.dI);
	m_pSpfcUtil->SetDamping(m_pParamData->CodeParam.P100_2013.dXi);
	m_pSpfcUtil->SetQ(m_pParamData->CodeParam.P100_2013.dQ);
	m_dMaxPeriod = m_pParamData->CodeParam.P100_2013.dMaxPeriod;

	if(bInit)
	{
		//   m_SpecTypeCmb.SetCurSel(m_nSpecType);
		auto nSpecType = m_pSpfcUtil->GetSpecType();
		if (CB_ERR == CDlgUtil::CobxSetCurSelItemData(m_SpecTypeCmb, nSpecType))
			m_SpecTypeCmb.SetCurSel(nSpecType);


		m_wndTb.SetEditUnit(m_pSpfcUtil->GetTb());
		m_wndTc.SetEditUnit(m_pSpfcUtil->GetTc());
		m_wndTd.SetEditUnit(m_pSpfcUtil->GetTd());

		m_wndAg.SetEditUnit(m_pSpfcUtil->GetAg());
		m_wndDamping.SetEditUnit(m_pSpfcUtil->GetDamping());
		m_wndQ.SetEditUnit(m_pSpfcUtil->GetQ());

		CString str;
		str.Format(_T("%g"), m_pSpfcUtil->GetImportance());
		m_cmbImportance.SetWindowText(str);

		UpdateData(FALSE);  
	}

	Dlg2UtilData();
}

void CETC_GDS_P100_2013::Dlg2UtilData()
{
	DWORD nSpecType = 0;
	if (CDlgUtil::CobxGetItemDataByCurSel(m_SpecTypeCmb, nSpecType) == FALSE)
		ASSERT(false);
	//   m_nSpecType = m_SpecTypeCmb.GetCurSel();
	
	CString str;
	m_cmbImportance.GetWindowText(str);

	m_pSpfcUtil->SetSpecType(nSpecType);
	m_pSpfcUtil->SetParaTypeRdo(m_nParaTypeRdo);
	m_pSpfcUtil->SetTb(m_wndTb.GetEditValue());
	m_pSpfcUtil->SetTc(m_wndTc.GetEditValue());
	m_pSpfcUtil->SetTd(m_wndTd.GetEditValue());
	m_pSpfcUtil->SetAg(m_wndAg.GetEditValue());
	m_pSpfcUtil->SetImportance(_tstof(str));
	m_pSpfcUtil->SetDamping(m_wndDamping.GetEditValue());
	m_pSpfcUtil->SetQ(m_wndQ.GetEditValue());
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);
}

void CETC_GDS_P100_2013::Data2Dlg()
{
	if(m_pParamData == NULL)
		return;

	if(m_pParamData->nSpecCodeType!=D_SPFC_CODE_P100_2013)
	{
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_P100_2013);
	}
	
	// Input Data
	m_pSpfcUtil->SetSpecType(m_pParamData->CodeParam.P100_2013.nSpectrumType);
	m_nParaTypeRdo= m_pParamData->CodeParam.P100_2013.nParameterType;
	m_pSpfcUtil->SetTb(m_pParamData->CodeParam.P100_2013.dTb);
	m_pSpfcUtil->SetTc(m_pParamData->CodeParam.P100_2013.dTc);
	m_pSpfcUtil->SetTd(m_pParamData->CodeParam.P100_2013.dTd);
	m_pSpfcUtil->SetAg(m_pParamData->CodeParam.P100_2013.dAgr);
	m_pSpfcUtil->SetImportance(m_pParamData->CodeParam.P100_2013.dI);
	m_pSpfcUtil->SetDamping(m_pParamData->CodeParam.P100_2013.dXi);
	m_pSpfcUtil->SetQ(m_pParamData->CodeParam.P100_2013.dQ);
	m_dMaxPeriod  = m_pParamData->CodeParam.P100_2013.dMaxPeriod;

//   m_SpecTypeCmb.SetCurSel(m_nSpecType);

	auto nSpecType = m_pSpfcUtil->GetSpecType();
	if(CB_ERR==CDlgUtil::CobxSetCurSelItemData(m_SpecTypeCmb, nSpecType))
		m_SpecTypeCmb.SetCurSel(nSpecType);

	m_wndTb.SetEditUnit(m_pSpfcUtil->GetTb());
	m_wndTc.SetEditUnit(m_pSpfcUtil->GetTc());
	m_wndTd.SetEditUnit(m_pSpfcUtil->GetTd());

	m_wndAg.SetEditUnit(m_pSpfcUtil->GetAg());
	m_wndDamping.SetEditUnit(m_pSpfcUtil->GetDamping());
	m_wndQ.SetEditUnit(m_pSpfcUtil->GetQ());

	CString str;
	str.Format(_T("%g"), m_pSpfcUtil->GetImportance());
	m_cmbImportance.SetWindowText(str);  

	UpdateData(FALSE);
	Dlg2UtilData();
}

void CETC_GDS_P100_2013::ShowDlgData() 
{
	if(m_pParamData == NULL)
		return;

	if(m_pParamData->nSpecCodeType == D_SPFC_CODE_P100_2013)
		m_CodeD.P100_2013 = m_pParamData->CodeParam.P100_2013;
	else
		m_CodeD.P100_2013.Initialize();

	m_pSpfcUtil->SetSpecType(m_CodeD.P100_2013.nSpectrumType);
	m_nParaTypeRdo= m_CodeD.P100_2013.nParameterType;
	m_pSpfcUtil->SetTb(m_CodeD.P100_2013.dTb);
	m_pSpfcUtil->SetTc(m_CodeD.P100_2013.dTc);
	m_pSpfcUtil->SetTd(m_CodeD.P100_2013.dTd);
	m_pSpfcUtil->SetAg(m_CodeD.P100_2013.dAgr);
	m_pSpfcUtil->SetImportance(m_CodeD.P100_2013.dI);
	m_pSpfcUtil->SetDamping(m_CodeD.P100_2013.dXi);
	m_pSpfcUtil->SetQ(m_CodeD.P100_2013.dQ);
	m_dMaxPeriod  = m_CodeD.P100_2013.dMaxPeriod;  

//   m_SpecTypeCmb.SetCurSel(m_nSpecType);
	auto nSpecType = m_pSpfcUtil->GetSpecType();
	if(CB_ERR==CDlgUtil::CobxSetCurSelItemData(m_SpecTypeCmb, nSpecType))
		m_SpecTypeCmb.SetCurSel(nSpecType);

	m_wndTb.SetEditUnit(m_pSpfcUtil->GetTb());
	m_wndTc.SetEditUnit(m_pSpfcUtil->GetTc());
	m_wndTd.SetEditUnit(m_pSpfcUtil->GetTd());

	m_wndAg.SetEditUnit(m_pSpfcUtil->GetAg());
	m_wndDamping.SetEditUnit(m_pSpfcUtil->GetDamping());
	m_wndQ.SetEditUnit(m_pSpfcUtil->GetQ());

	UpdateData(FALSE);

	CtrlManager();

	CString str;
	str.Format(_T("%g"), m_pSpfcUtil->GetImportance());
	m_cmbImportance.SetWindowText(str);  

	UpdateData(FALSE);
	Dlg2UtilData();
}

void CETC_GDS_P100_2013::Initialize() 
{
	if(m_pParamData == NULL)
		return;

	m_pParamData->CodeParam.Initialize(D_SPFC_CODE_P100_2013);
}

void CETC_GDS_P100_2013::InitIFactorCombo()
{
	m_cmbImportance.ResetContent();
	CString aItem[4] = {_T("0.8"), _T("1.0"), _T("1.2"), _T("1.4")};
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbImportance.AddString(aItem[i]);
	m_cmbImportance.SetCurSel(1);
}

void CETC_GDS_P100_2013::CtrlManager()
{
	UpdateData(TRUE);

	BOOL bElastic = FALSE;  
	BOOL bVertical = FALSE;  
	BOOL bAutomatic = TRUE;

	DWORD nSpecType = 0;
	CDlgUtil::CobxGetItemDataByCurSel(m_SpecTypeCmb, nSpecType);
	if(nSpecType == D_SPFC_P100_2013_SP_HE || nSpecType == D_SPFC_P100_2013_SP_VE) bElastic = TRUE;  
	if(nSpecType == D_SPFC_P100_2013_SP_VE || nSpecType == D_SPFC_P100_2013_SP_VD) bVertical = TRUE;    
	if(m_nParaTypeRdo == D_SPFC_P100_2013_USERD) bAutomatic = FALSE;

	CDlgUtil::CtrlShowHide(this, m_aCtrlElastic, bElastic);
	CDlgUtil::CtrlShowHide(this, m_aCtrlDesign, !bElastic);

	// Tb, Tc, Td는 Spectrum Parameter Type이 User Defined인 경우 사용자 입력만 가능
	m_wndTb.SetReadOnly(bAutomatic);
	m_wndTc.SetReadOnly(bAutomatic);
	m_wndTd.SetReadOnly(bAutomatic);
	
	if(bAutomatic)
	{
		CLateralLoad load;

		double dTb = m_pSpfcUtil->GetTb();
		double dTc = m_pSpfcUtil->GetTc();
		double dTd = m_pSpfcUtil->GetTd();

		load.Get_P100_2013_Spectrum_Parameter(bVertical, m_nParaTypeRdo, dTb, dTc, dTd);

		m_wndTb.SetEditUnit(dTb);
		m_wndTc.SetEditUnit(dTc);
		m_wndTd.SetEditUnit(dTd);

		m_pSpfcUtil->SetTb(dTb);
		m_pSpfcUtil->SetTc(dTc);
		m_pSpfcUtil->SetTd(dTd);
	}
}

void CETC_GDS_P100_2013::AlignControl() 
{
	CWnd* pWnd;
	CRect FromRect, ToRect;
	pWnd = GetDlgItem(IDC_ETC_GDS_P100_2013_DAMPING_EDIT); ASSERT(pWnd);
	pWnd->GetWindowRect(&ToRect);
	pWnd = GetDlgItem(IDC_ETC_GDS_P100_2013_BEHAVIOR_EDIT); ASSERT(pWnd);
	pWnd->GetWindowRect(&FromRect);
	int nDistY = ToRect.top - FromRect.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlDesign, nDistY);  
}

void CETC_GDS_P100_2013::UpdateDampingWnd(CWnd* pDampingWnd)
{
	CString csDamping;
	csDamping.Format(_T("%g"), m_pSpfcUtil->GetDamping()/100.0);
	pDampingWnd->SetWindowText(csDamping);
}

void CETC_GDS_P100_2013::End()
{
	UpdateData(TRUE);
	Dlg2UtilData();

	CMyChildDialog::End();
}

void CETC_GDS_P100_2013::EndNotClose()
{
	UpdateData(TRUE);
	Dlg2UtilData();
}

BOOL CETC_GDS_P100_2013::ISVALID()
{   
	DWORD nSpecType = 0;
	CDlgUtil::CobxGetItemDataByCurSel(m_SpecTypeCmb, nSpecType);

	m_pSpfcUtil->SetSpecType(nSpecType);

	Dlg2UtilData();
	return static_cast<BOOL>(m_pSpfcUtil->CheckValid());
	
}

void CETC_GDS_P100_2013::OnSelchangeEtcGdsP100_2013SpecTypeCmb() 
{
	CtrlManager();	

	// PMS 5324-4 : VD일 때 q초기값 1.5로
	//int nSpecType = m_SpecTypeCmb.GetCurSel();
	DWORD nSpecType = 0;
	CDlgUtil::CobxGetItemDataByCurSel(m_SpecTypeCmb, nSpecType);

	if(nSpecType == D_SPFC_P100_2013_SP_HD)
	{
		m_pSpfcUtil->SetQ(4.0);
		m_wndQ.SetEditUnit(m_pSpfcUtil->GetQ());
	}
	else if(nSpecType == D_SPFC_P100_2013_SP_VD)
	{
		m_pSpfcUtil->SetQ(1.5);
		m_wndQ.SetEditUnit(m_pSpfcUtil->GetQ());
	}
}

void CETC_GDS_P100_2013::OnEtcGdsP100_2013ParatypeRdo() 
{
	CtrlManager();	
}

void CETC_GDS_P100_2013::InitCtrl()
{
	InitSpectrumTypeCombo();
	InitIFactorCombo();
	GetDlgItem(IDC_ETC_GDS_P100_2013_IMPORTANCE_TEXT)->SetWindowText(_LS(IDS_GDS_P100_2013_IMPORTANCE_FACTOR_TEXT)); // 특수문자가 있어서..
}

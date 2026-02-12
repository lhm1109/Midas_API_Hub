// ETC_GDS_EURO2003.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_EURO2003.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\Seismic_EN1998_1_2004.h"

#include "..\wg_base\DlgUtil.h"
#include "SpfcEURO2004Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_EURO2003 dialog


CETC_GDS_EURO2003::CETC_GDS_EURO2003(T_SPFC_D* pParamData/*=NULL*/, CWnd* pParent /*=NULL*/, int nCase/*=0*/)
	: CMyChildDialog(CETC_GDS_EURO2003::IDD, pParent, std::make_shared<CSpfcEURO2004Util>())
{
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcEURO2004Util>(m_pSpfcBaseUtil);

	//{{AFX_DATA_INIT(CETC_GDS_EURO2003)
	m_nParaTypeRdo = -1;
	m_dMaxPeriod = PRD_ED;
	//}}AFX_DATA_INIT
	//m_aCtrlElastic.Add(IDC_ETC_GDS_EURO2003_ACC_TEXT);
	m_nCase = nCase;

	m_nNAIndex = EN_SPFC_RECOMMENDED;

	SetDefaultValue();
}


void CETC_GDS_EURO2003::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_EURO2003)
	DDX_Control(pDX, IDC_ETC_GDS_EURO2003_SPEC_TYPE_CMB,      m_SpecTypeCmb);
	DDX_Control(pDX, IDC_ETC_GDS_EURO2003_GROUND_CMB,         m_GroundTypeCmb);
	DDX_Control(pDX, IDC_ETC_GDS_EURO2003_AG_EDIT,            m_wndAg);
	DDX_Radio(pDX,   IDC_ETC_GDS_EURO2003_PARATYPE_RDO,       m_nParaTypeRdo);
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT,                        m_dMaxPeriod);	
	//
	DDX_Control(pDX, IDC_ETC_GDS_EURO2003_TD_EDIT,            m_wndTd);
	DDX_Control(pDX, IDC_ETC_GDS_EURO2003_TC_EDIT,            m_wndTc);
	DDX_Control(pDX, IDC_ETC_GDS_EURO2003_TB_EDIT,            m_wndTb);
	DDX_Control(pDX, IDC_ETC_GDS_EURO2003_S_EDIT,             m_wndS);
	DDX_Control(pDX, IDC_ETC_GDS_EURO2003_IMPORTANCE_CMB,     m_cmbImportance);
	DDX_Control(pDX, IDC_ETC_GDS_EURO2003_DAMPING_EDIT,       m_wndDamping);
	DDX_Control(pDX, IDC_ETC_GDS_EURO2003_BETA_EDIT,          m_wndBeta);
	DDX_Control(pDX, IDC_ETC_GDS_EURO2003_BEHAVIOR_EDIT,      m_wndQ);
	//
	DDX_Control(pDX, IDC_ETC_GDS_EURO2003_REGION_Mal_CMB,     m_RegionMalCmb);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_EURO2003, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_EURO2003)
	ON_CBN_SELCHANGE(IDC_ETC_GDS_EURO2003_SPEC_TYPE_CMB, OnSelchangeEtcGdsEuro2003SpecTypeCmb)
	ON_BN_CLICKED(IDC_ETC_GDS_EURO2003_PARATYPE_RDO,     OnEtcGdsEuro2003ParatypeRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_EURO2003_PARATYPE_RDO2,    OnEtcGdsEuro2003ParatypeRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_EURO2003_PARATYPE_RDO3,    OnEtcGdsEuro2003ParatypeRdo)
	//ON_EN_CHANGE(IDC_ETC_GDS_EURO2003_PERIOD_TS_Mal_EDIT,  OnChangeTsEdit)
	
	ON_CBN_SELCHANGE(IDC_ETC_GDS_EURO2003_GROUND_CMB,    OnSelchangeEtcGdsEuro2003GroundCmb)
	ON_CBN_SELCHANGE(IDC_ETC_GDS_EURO2003_REGION_Mal_CMB,OnSelchangeEtcGdsEuro2003RegionCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_EURO2003 message handlers

BOOL CETC_GDS_EURO2003::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	if(m_pParamData!=NULL)
	{
		m_nNAIndex = m_pParamData->CodeParam.EURO2004.nNationalAnnex;
	}
	SetDefaultValue();

	InitCtrlArray();

	InitSpectrumTypeCombo();
	InitGroundTypeCombo();
	InitRegionCombo();

	m_wndAg.SetEditUnit(m_pSpfcUtil->GetAg());
	// Euro
	m_wndS.SetEditUnit(m_pSpfcUtil->GetS());
	m_wndTb.SetEditUnit(m_pSpfcUtil->GetTb());
	m_wndTc.SetEditUnit(m_pSpfcUtil->GetTc());
	m_wndTd.SetEditUnit(m_pSpfcUtil->GetTd());
	m_wndDamping.SetEditUnit(m_pSpfcUtil->GetDamping());
	m_wndQ.SetEditUnit(m_pSpfcUtil->GetQ());
	m_wndBeta.SetEditUnit(m_pSpfcUtil->GetBeta());
	
	AlignControl();
	InitIFactorCombo(TRUE);
	CtrlManager();
	AlignControlByNA();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_EURO2003::InitSpectrumTypeCombo()
{
	// 0: 일반, 1: Elastic Sepctrum 삭제, 2: Design Spectrum 삭제
	m_SpecTypeCmb.ResetContent();
	if(m_nNAIndex == EN_SPFC_SINGAPORE)  //(m_bSingapore)
	{
		if(m_nCase !=1 ) CDlgUtil::CobxAddItem(m_SpecTypeCmb, _LS(IDS_CMD_EURO2003_H_ELASTIC_SPECTRUM), EN_HORI_ELASTIC_SIN);
		if(m_nCase !=2 ) CDlgUtil::CobxAddItem(m_SpecTypeCmb, _LS(IDS_CMD_EURO2003_H_DESIGN_SPECTRUM) , EN_HORI_DESIGN_SIN );
	}
	else
	{
		if(m_nCase !=1 )
		{
			CDlgUtil::CobxAddItem(m_SpecTypeCmb, _LS(IDS_CMD_EURO2003_H_ELASTIC_SPECTRUM), EN_HORI_ELASTIC);
			CDlgUtil::CobxAddItem(m_SpecTypeCmb, _LS(IDS_CMD_EURO2003_V_ELASTIC_SPECTRUM), EN_VERT_ELASTIC);
		}
		if(m_nCase !=2 )
		{
			CDlgUtil::CobxAddItem(m_SpecTypeCmb, _LS(IDS_CMD_EURO2003_H_DESIGN_SPECTRUM), EN_HORI_DESIGN);
			CDlgUtil::CobxAddItem(m_SpecTypeCmb, _LS(IDS_CMD_EURO2003_V_DESIGN_SPECTRUM), EN_VERT_DESIGN);
		}
	}
	m_SpecTypeCmb.SetCurSel(0);
	CDlgUtil::CobxAdjustListBoxWidth(m_SpecTypeCmb);
}

void CETC_GDS_EURO2003::InitGroundTypeCombo()
{
	m_GroundTypeCmb.ResetContent();
	if(m_nNAIndex == EN_SPFC_SINGAPORE)//if(m_bSingapore)
	{
		CDlgUtil::CobxAddItem(m_GroundTypeCmb, _T("C"), EN_SPFC_GROUND_C);
		CDlgUtil::CobxAddItem(m_GroundTypeCmb, _T("D"), EN_SPFC_GROUND_D);
		CDlgUtil::CobxAddItem(m_GroundTypeCmb, _T("S1"), EN_SPFC_GROUND_S1);
		m_GroundTypeCmb.SetCurSel(0);
	}
	else
	{
		CDlgUtil::CobxAddItem(m_GroundTypeCmb, _T("A"), EN_SPFC_GROUND_A);
		CDlgUtil::CobxAddItem(m_GroundTypeCmb, _T("B"), EN_SPFC_GROUND_B);
		CDlgUtil::CobxAddItem(m_GroundTypeCmb, _T("C"), EN_SPFC_GROUND_C);
		CDlgUtil::CobxAddItem(m_GroundTypeCmb, _T("D"), EN_SPFC_GROUND_D);
		CDlgUtil::CobxAddItem(m_GroundTypeCmb, _T("E"), EN_SPFC_GROUND_E);
		if (m_nNAIndex != EN_SPFC_MALESIA)
		{
			CDlgUtil::CobxAddItem(m_GroundTypeCmb, _T("S1"), EN_SPFC_GROUND_S1);
			CDlgUtil::CobxAddItem(m_GroundTypeCmb, _T("S2"), EN_SPFC_GROUND_S2);
		}
		m_GroundTypeCmb.SetCurSel(1);
	}
	CDlgUtil::CobxAdjustListBoxWidth(m_GroundTypeCmb);
}

void CETC_GDS_EURO2003::InitRegionCombo()
{
	m_RegionMalCmb.ResetContent();
	CDlgUtil::CobxAddItem(m_RegionMalCmb, _LS(IDS_ETC_GDS_EURO2003_Malaysia_Region_Pen), EN_SPFC_REGION_PENINSULAR);
	CDlgUtil::CobxAddItem(m_RegionMalCmb, _LS(IDS_ETC_GDS_EURO2003_Malaysia_Region_Sab), EN_SPFC_REGION_SABAH);
	CDlgUtil::CobxAddItem(m_RegionMalCmb, _LS(IDS_ETC_GDS_EURO2003_Malaysia_Region_Sar), EN_SPFC_REGION_SARAWAK);
	m_RegionMalCmb.SetCurSel(0);

	CDlgUtil::CobxAdjustListBoxWidth(m_RegionMalCmb);
}

BOOL CETC_GDS_EURO2003::Dlg2Data()
{
	if(m_pParamData == NULL)
		return FALSE; 

	// UpdateData(TRUE); // CETC_DESIGN_SPECT에서 Dlg2Data()호출 전에 이미 호출되었다.
	m_pParamData->nSpecCodeType = D_SPFC_CODE_EURO2004;	

	Dlg2Data(m_pParamData->CodeParam.EURO2004);
		
	return TRUE;
}

void CETC_GDS_EURO2003::Dlg2Data(T_SPFC_EURO2004& EURO2004)
{
	EURO2004.Initialize();

	EURO2004.nSpectrumType	= m_pSpfcUtil->GetSpecType();
	EURO2004.nGroundType	= m_pSpfcUtil->GetGroundType();
	EURO2004.nNationalAnnex = (EN_SPFC_NA) m_nNAIndex;

	EURO2004.nParameterType = m_nParaTypeRdo;
	EURO2004.dSoilFactor    = m_pSpfcUtil->GetS();
	EURO2004.dTb            = m_pSpfcUtil->GetTb();
	EURO2004.dTc            = m_pSpfcUtil->GetTc();  
	EURO2004.dTd            = m_pSpfcUtil->GetTd();
	EURO2004.dAgr           = m_pSpfcUtil->GetAg();
	EURO2004.dI             = m_pSpfcUtil->GetImportance();
	EURO2004.dXi            = m_pSpfcUtil->GetDamping();
	EURO2004.dQ             = m_pSpfcUtil->GetQ();
	EURO2004.dB             = m_pSpfcUtil->GetBeta();
	EURO2004.dMaxPeriod     = m_dMaxPeriod; 
	EURO2004.dTs            = 0.5;
	EURO2004.nResionType    = m_pSpfcUtil->GetRegion();

}

void CETC_GDS_EURO2003::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	if(m_pParamData == NULL)
		return;

	if(m_nNAIndex == EN_SPFC_MALESIA)
	{
		SetPushOverDataMal(bInit/*=TRUE*/);
		return;
	}

	if(m_pParamData->nSpecCodeType!=D_SPFC_CODE_EURO2004)
	{
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_EURO2004);
	}

	m_pSpfcUtil->SetGroundType(m_pParamData->CodeParam.EURO2004.nGroundType);
	m_pSpfcUtil->SetSpecType(m_pParamData->CodeParam.EURO2004.nSpectrumType);

	// Input Data
	m_nParaTypeRdo= m_pParamData->CodeParam.EURO2004.nParameterType;
	m_pSpfcUtil->SetS(m_pParamData->CodeParam.EURO2004.dSoilFactor);
	m_pSpfcUtil->SetTb(m_pParamData->CodeParam.EURO2004.dTb);
	m_pSpfcUtil->SetTc(m_pParamData->CodeParam.EURO2004.dTc);
	m_pSpfcUtil->SetTd(m_pParamData->CodeParam.EURO2004.dTd);
	m_pSpfcUtil->SetAg(m_pParamData->CodeParam.EURO2004.dAgr);
	m_pSpfcUtil->SetImportance(m_pParamData->CodeParam.EURO2004.dI);
	m_pSpfcUtil->SetDamping(m_pParamData->CodeParam.EURO2004.dXi);
	m_pSpfcUtil->SetQ(m_pParamData->CodeParam.EURO2004.dQ);
	m_pSpfcUtil->SetBeta(m_pParamData->CodeParam.EURO2004.dB);
	m_dMaxPeriod  = m_pParamData->CodeParam.EURO2004.dMaxPeriod;

	if(bInit)
	{
		if(CB_ERR==CDlgUtil::CobxSetCurSelItemData(m_SpecTypeCmb, m_pSpfcUtil->GetSpecType()))
			m_SpecTypeCmb.SetCurSel(m_pSpfcUtil->GetSpecType());
		if (CB_ERR == CDlgUtil::CobxSetCurSelItemData(m_GroundTypeCmb, m_pSpfcUtil->GetGroundType()))
			m_GroundTypeCmb.SetCurSel(m_pSpfcUtil->GetGroundType());

		m_wndS.SetEditUnit(m_pSpfcUtil->GetS());
		m_wndTb.SetEditUnit(m_pSpfcUtil->GetTb());
		m_wndTc.SetEditUnit(m_pSpfcUtil->GetTc());
		m_wndTd.SetEditUnit(m_pSpfcUtil->GetTd());

		m_wndAg.SetEditUnit(m_pSpfcUtil->GetAg());
		m_wndDamping.SetEditUnit(m_pSpfcUtil->GetDamping());
		m_wndQ.SetEditUnit(m_pSpfcUtil->GetQ());
		m_wndBeta.SetEditUnit(m_pSpfcUtil->GetBeta());

		CString str;
		str.Format(_T("%g"), m_pSpfcUtil->GetImportance());
		m_cmbImportance.SetWindowText(str);  

		UpdateData(FALSE);  
	}

	Dlg2UtilData();
}
void CETC_GDS_EURO2003::SetPushOverDataMal(BOOL bInit/*=TRUE*/)
{
	if(m_pParamData == NULL)
		return;

// 	if(m_pParamData->CodeParam.EURO2004.nGroundType<13)
// 		return;

	if(m_pParamData->nSpecCodeType!=D_SPFC_CODE_EURO2004)
	{
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_EURO2004);
	}

	m_nNAIndex    = m_pParamData->CodeParam.EURO2004.nNationalAnnex;
	m_pSpfcUtil->SetGroundType(m_pParamData->CodeParam.EURO2004.nGroundType);
	m_pSpfcUtil->SetSpecType(m_pParamData->CodeParam.EURO2004.nSpectrumType);
	m_pSpfcUtil->SetRegion(m_pParamData->CodeParam.EURO2004.nResionType);

	// Input Data
// 	m_dS_Mal          = m_pParamData->CodeParam.EURO2004.dSoilFactor;
// 	m_dTb_Mal         = m_pParamData->CodeParam.EURO2004.dTb;
// 	m_dTc_Mal         = m_pParamData->CodeParam.EURO2004.dTc;
// 	m_dTd_Mal         = m_pParamData->CodeParam.EURO2004.dTd;
// 	m_dTs_Mal         = m_pParamData->CodeParam.EURO2004.dTs;
// 	m_dImportance_Mal = m_pParamData->CodeParam.EURO2004.dI;
// 	m_dDamping_Mal    = m_pParamData->CodeParam.EURO2004.dXi;
// 	m_dQ_Mal          = m_pParamData->CodeParam.EURO2004.dQ;
// 	m_dBeta_Mal       = m_pParamData->CodeParam.EURO2004.dB;

	m_nParaTypeRdo= m_pParamData->CodeParam.EURO2004.nParameterType;
	m_pSpfcUtil->SetAg(m_pParamData->CodeParam.EURO2004.dAgr);
	m_dMaxPeriod  = m_pParamData->CodeParam.EURO2004.dMaxPeriod;

	if(bInit)
	{
		if(CB_ERR==CDlgUtil::CobxSetCurSelItemData(m_SpecTypeCmb, m_pSpfcUtil->GetSpecType()))
			m_SpecTypeCmb.SetCurSel(m_pSpfcUtil->GetSpecType());
		if (CB_ERR == CDlgUtil::CobxSetCurSelItemData(m_GroundTypeCmb, m_pSpfcUtil->GetGroundType()))
			m_GroundTypeCmb.SetCurSel(m_pSpfcUtil->GetGroundType());
		
		m_RegionMalCmb.SetCurSel(m_pSpfcUtil->GetRegion());

// 		m_wndS_Mal.SetEditUnit(m_dS_Mal);
// 		m_wndTb_Mal.SetEditUnit(m_dTb_Mal);
// 		m_wndTc_Mal.SetEditUnit(m_dTc_Mal);
// 		m_wndTd_Mal.SetEditUnit(m_dTd_Mal);
// 		m_wndTs_Mal.SetEditUnit(m_dTs_Mal);
// 		m_wndQ_Mal.SetEditUnit(m_dQ_Mal);
// 		m_wndBeta_Mal.SetEditUnit(m_dBeta_Mal);  
// 		m_wndDamping_Mal.SetEditUnit(m_dDamping_Mal);
		
		m_wndAg.SetEditUnit(m_pSpfcUtil->GetAg());
// 
// 		CString str;
// 		str.Format(_T("%g"), m_dImportance_Mal);
// 		m_cmbImportance_Mal.SetWindowText(str);  

		Dlg2UtilData();
		UpdateData(FALSE);  
	}
}
void CETC_GDS_EURO2003::Dlg2UtilData()
{

	DWORD nSpecType = 0;
	if (CDlgUtil::CobxGetItemDataByCurSel(m_SpecTypeCmb, nSpecType))  m_pSpfcUtil->SetSpecType(nSpecType);
	else { ASSERT(0); m_pSpfcUtil->SetSpecType(0); }

	DWORD nGroundType = 0;
	if (CDlgUtil::CobxGetItemDataByCurSel(m_GroundTypeCmb, nGroundType))  m_pSpfcUtil->SetGroundType(nGroundType);
	else { ASSERT(0); m_pSpfcUtil->SetGroundType(0); }

	DWORD nRegionType = 0;
	if (CDlgUtil::CobxGetItemDataByCurSel(m_RegionMalCmb, nRegionType))  m_pSpfcUtil->SetRegion(nRegionType);
	else { ASSERT(0); m_pSpfcUtil->SetRegion(0); }

	m_pSpfcUtil->SetParaTypeRdo(m_nParaTypeRdo);
	m_pSpfcUtil->SetS(m_wndS.GetEditValue());
	m_pSpfcUtil->SetTb(m_wndTb.GetEditValue());
	m_pSpfcUtil->SetTc(m_wndTc.GetEditValue());
	m_pSpfcUtil->SetTd(m_wndTd.GetEditValue());
	m_pSpfcUtil->SetAg(m_wndAg.GetEditValue());

	CString str;
	m_cmbImportance.GetWindowText(str);
	m_pSpfcUtil->SetImportance(_tstof(str));

	m_pSpfcUtil->SetDamping(m_wndDamping.GetEditValue());
	m_pSpfcUtil->SetQ(m_wndQ.GetEditValue());
	m_pSpfcUtil->SetBeta(m_wndBeta.GetEditValue());
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);
	m_pSpfcUtil->SetNAIndex(static_cast<EN_SPFC_NA>(m_nNAIndex));
}

void CETC_GDS_EURO2003::Data2Dlg()
{
	if(m_pParamData == NULL)
		return;

	if(m_pParamData->nSpecCodeType!=D_SPFC_CODE_EURO2004)
	{
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_EURO2004);
	}

	if(m_nNAIndex == EN_SPFC_SINGAPORE)
	{
		m_pSpfcUtil->SetGroundType(m_pParamData->CodeParam.EURO2004.nGroundType);
		m_pSpfcUtil->SetSpecType(m_pParamData->CodeParam.EURO2004.nSpectrumType)/*-10*/;
	}
	else
	{
		m_pSpfcUtil->SetGroundType(m_pParamData->CodeParam.EURO2004.nGroundType);
		m_pSpfcUtil->SetSpecType(m_pParamData->CodeParam.EURO2004.nSpectrumType);
	}

	// Input Data
	m_nParaTypeRdo= m_pParamData->CodeParam.EURO2004.nParameterType;
	m_pSpfcUtil->SetS(m_pParamData->CodeParam.EURO2004.dSoilFactor);
	m_pSpfcUtil->SetTb(m_pParamData->CodeParam.EURO2004.dTb);
	m_pSpfcUtil->SetTc(m_pParamData->CodeParam.EURO2004.dTc);
	m_pSpfcUtil->SetTd(m_pParamData->CodeParam.EURO2004.dTd);
	m_pSpfcUtil->SetAg(m_pParamData->CodeParam.EURO2004.dAgr);
	m_pSpfcUtil->SetImportance(m_pParamData->CodeParam.EURO2004.dI);
	m_pSpfcUtil->SetDamping(m_pParamData->CodeParam.EURO2004.dXi);
	m_pSpfcUtil->SetQ(m_pParamData->CodeParam.EURO2004.dQ);
	m_pSpfcUtil->SetBeta(m_pParamData->CodeParam.EURO2004.dB);
	m_dMaxPeriod  = m_pParamData->CodeParam.EURO2004.dMaxPeriod;

	if(CB_ERR==CDlgUtil::CobxSetCurSelItemData(m_SpecTypeCmb, m_pSpfcUtil->GetSpecType()))
		m_SpecTypeCmb.SetCurSel(m_pSpfcUtil->GetSpecType());
	if (CB_ERR == CDlgUtil::CobxSetCurSelItemData(m_GroundTypeCmb, m_pSpfcUtil->GetGroundType()))
		m_GroundTypeCmb.SetCurSel(m_pSpfcUtil->GetGroundType());
	
	m_wndS.SetEditUnit(m_pSpfcUtil->GetS());
	m_wndTb.SetEditUnit(m_pSpfcUtil->GetTb());
	m_wndTc.SetEditUnit(m_pSpfcUtil->GetTc());
	m_wndTd.SetEditUnit(m_pSpfcUtil->GetTd());

	m_wndAg.SetEditUnit(m_pSpfcUtil->GetAg());
	m_wndDamping.SetEditUnit(m_pSpfcUtil->GetDamping());
	m_wndQ.SetEditUnit(m_pSpfcUtil->GetQ());
	m_wndBeta.SetEditUnit(m_pSpfcUtil->GetBeta());

	CString str;
	str.Format(_T("%g"), m_pSpfcUtil->GetImportance());
	m_cmbImportance.SetWindowText(str);  

	Dlg2UtilData();
	UpdateData(FALSE);  
}

void CETC_GDS_EURO2003::SetNationalAnnex(int nNAIndex)
{
	if(m_nNAIndex==nNAIndex) return;
	m_nNAIndex=nNAIndex;
	
	//BOOL bSingapore = (nNAIndex==1)? TRUE : FALSE;
	//if(m_bSingapore==bSingapore) return;
	//m_bSingapore = bSingapore;
	
	InitSpectrumTypeCombo();
	InitGroundTypeCombo();
	InitRegionCombo();
	CtrlManager();
	AlignControlByNA();

	if(m_nNAIndex == EN_SPFC_MALESIA)//
	{
		m_nParaTypeRdo=0;
		m_pParamData->CodeParam.EURO2004.nGroundType = EN_SPFC_GROUND_B;
		if(m_pParamData->CodeParam.EURO2004.nParameterType==1) m_pParamData->CodeParam.EURO2004.nParameterType=0;
		//m_pParamData->CodeParam.EURO2004.dAgr = M_InitValueCurUnit(0.0178, N, M, D_UNITSYS_BASE_LENGTH);
		m_pParamData->CodeParam.EURO2004.dI = 1.0;
	}
	else if(m_nNAIndex == EN_SPFC_SINGAPORE)//if(m_bSingapore)
	{
		if(m_nParaTypeRdo==1) m_nParaTypeRdo=0;
		m_pParamData->CodeParam.EURO2004.nGroundType = EN_SPFC_GROUND_C;
		if(m_pParamData->CodeParam.EURO2004.nParameterType==1) m_pParamData->CodeParam.EURO2004.nParameterType=0;
		m_pParamData->CodeParam.EURO2004.dAgr = M_InitValueCurUnit(0.0178, N, M, D_UNITSYS_BASE_LENGTH);
		m_pParamData->CodeParam.EURO2004.dI = 1.4;
	}
	else
	{
		m_pParamData->CodeParam.EURO2004.nGroundType = EN_SPFC_GROUND_B;
		m_pParamData->CodeParam.EURO2004.dAgr = M_InitValueCurUnit(0.08, N, M, D_UNITSYS_BASE_LENGTH);
		m_pParamData->CodeParam.EURO2004.dI = 1.0;
	}
	m_pParamData->CodeParam.EURO2004.nNationalAnnex = (EN_SPFC_NA)m_nNAIndex;
	CDlgUtil::CobxSetCurSelItemData(m_GroundTypeCmb, m_pParamData->CodeParam.EURO2004.nGroundType);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlParaType, m_pParamData->CodeParam.EURO2004.nParameterType);
	m_wndAg.SetEditUnit(m_pParamData->CodeParam.EURO2004.dAgr);
	InitIFactorCombo();
	
	OnEtcGdsEuro2003ParatypeRdo();
}

void CETC_GDS_EURO2003::ShowDlgData() 
{
	if(m_pParamData == NULL)
		return;

	if(m_pParamData->nSpecCodeType == D_SPFC_CODE_EURO2004)
		m_CodeD.EURO2004 = m_pParamData->CodeParam.EURO2004;
	else
		m_CodeD.EURO2004.Initialize();

	m_nNAIndex = m_CodeD.EURO2004.nNationalAnnex;

	if(m_nCase != 0)
	{
		if     (m_nCase==1)  m_CodeD.EURO2004.nSpectrumType = m_nNAIndex == EN_SPFC_SINGAPORE ? EN_HORI_DESIGN_SIN : EN_HORI_DESIGN;
		else if(m_nCase==2)  m_CodeD.EURO2004.nSpectrumType = m_nNAIndex == EN_SPFC_SINGAPORE ? EN_HORI_ELASTIC_SIN : EN_HORI_ELASTIC;
	}

	if(m_nNAIndex == EN_SPFC_SINGAPORE)
	{
		InitSpectrumTypeCombo();
		InitGroundTypeCombo();
	}

	m_pSpfcUtil->SetSpecType(m_CodeD.EURO2004.nSpectrumType);
	m_pSpfcUtil->SetGroundType(m_CodeD.EURO2004.nGroundType);
	m_pSpfcUtil->SetRegion(m_CodeD.EURO2004.nResionType);
	m_nParaTypeRdo= m_CodeD.EURO2004.nParameterType;
	m_pSpfcUtil->SetS(m_CodeD.EURO2004.dSoilFactor);
	m_pSpfcUtil->SetTb(m_CodeD.EURO2004.dTb);
	m_pSpfcUtil->SetTc(m_CodeD.EURO2004.dTc);
	m_pSpfcUtil->SetTd(m_CodeD.EURO2004.dTd);
	m_pSpfcUtil->SetAg(m_CodeD.EURO2004.dAgr);
	m_pSpfcUtil->SetImportance(m_CodeD.EURO2004.dI);
	m_pSpfcUtil->SetDamping(m_CodeD.EURO2004.dXi);
	m_pSpfcUtil->SetQ(m_CodeD.EURO2004.dQ);
	m_pSpfcUtil->SetBeta(m_CodeD.EURO2004.dB);
	m_dMaxPeriod  = m_CodeD.EURO2004.dMaxPeriod;  

	//m_SpecTypeCmb.SetCurSel(m_nSpecType);
	CDlgUtil::CobxSetCurSelItemData(m_SpecTypeCmb, m_pSpfcUtil->GetSpecType());
	CDlgUtil::CobxSetCurSelItemData(m_GroundTypeCmb, m_pSpfcUtil->GetGroundType());
	CDlgUtil::CobxSetCurSelItemData(m_RegionMalCmb, m_pSpfcUtil->GetRegion());
	
	m_wndS.SetEditUnit(m_pSpfcUtil->GetS());
	m_wndTb.SetEditUnit(m_pSpfcUtil->GetTb());
	m_wndTc.SetEditUnit(m_pSpfcUtil->GetTc());
	m_wndTd.SetEditUnit(m_pSpfcUtil->GetTd());

	m_wndAg.SetEditUnit(m_pSpfcUtil->GetAg());
	m_wndDamping.SetEditUnit(m_pSpfcUtil->GetDamping());
	m_wndQ.SetEditUnit(m_pSpfcUtil->GetQ());
	m_wndBeta.SetEditUnit(m_pSpfcUtil->GetBeta());
	
	UpdateData(FALSE);

	InitIFactorCombo(TRUE);
	CtrlManager();

	CString str;
	str.Format(_T("%g"), m_pSpfcUtil->GetImportance());
	m_cmbImportance.SetWindowText(str);

	Dlg2UtilData();
	UpdateData(FALSE);

}

void CETC_GDS_EURO2003::Initialize() 
{
	if(m_pParamData == NULL)
		return;

	m_pParamData->CodeParam.Initialize(D_SPFC_CODE_EURO2004);

	if(m_nNAIndex == EN_SPFC_SINGAPORE)//if(m_bSingapore)
	{
		m_pParamData->CodeParam.EURO2004.nGroundType = EN_SPFC_GROUND_C;
		m_pParamData->CodeParam.EURO2004.dAgr = M_InitValueCurUnit(0.0178, N, M, D_UNITSYS_BASE_LENGTH);
		m_pParamData->CodeParam.EURO2004.dI = 1.4;
	}
}

void CETC_GDS_EURO2003::SetDefaultValue()
{
	m_nParaTypeRdo = 0;
}

void CETC_GDS_EURO2003::InitIFactorCombo(BOOL bInit/*=FALSE*/)
{
	m_cmbImportance.ResetContent();
	if(m_nNAIndex == EN_SPFC_SINGAPORE)//if(m_bSingapore)
	{
		if(bInit)
		{
			if(m_pParamData->CodeParam.EURO2004.nGroundType==10)
			{
				CString strFactor = _T("1.4");
				m_cmbImportance.AddString(strFactor);
			}
			else
			{
				CString aItem[2] = {_T("1.0"), _T("1.4")};
				for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
					m_cmbImportance.AddString(aItem[i]);
			}
		}
		else
		{
			if(m_GroundTypeCmb.GetCurSel()==0)
			{
				CString strFactor = _T("1.4");
				m_cmbImportance.AddString(strFactor);
			}
			else
			{
				CString aItem[2] = {_T("1.0"), _T("1.4")};
				for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
					m_cmbImportance.AddString(aItem[i]);
			}
		}
		m_cmbImportance.SetCurSel(0);
	}
	else
	{
		CString aItem[4] = {_T("0.8"), _T("1.0"), _T("1.2"), _T("1.4")};
		CString aItem_ma[4] = { _T("0.8"), _T("1.0"), _T("1.2"), _T("1.5") };
		CString* selectedItems = (m_nNAIndex == EN_SPFC_MALESIA) ? aItem_ma : aItem;
		
		for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
			m_cmbImportance.AddString(selectedItems[i]);
		
		m_cmbImportance.SetCurSel(1);
	}
}

void CETC_GDS_EURO2003::CtrlManager()
{
	UpdateData(TRUE);
	
	CDlgUtil::CtrlShowHide(this, m_aCtrlRegionType, m_nNAIndex == EN_SPFC_MALESIA);

	BOOL bElastic = FALSE;  
	BOOL bVertical = FALSE;  
	BOOL bSpecialGroundType = FALSE;
	BOOL bUserDefined = FALSE;
	BOOL bAutomatic = TRUE;

	DWORD nSpecType = 0;
	CDlgUtil::CobxGetItemDataByCurSel(m_SpecTypeCmb, nSpecType);

	DWORD nGroundType = 0;
	CDlgUtil::CobxGetItemDataByCurSel(m_GroundTypeCmb, nGroundType);

	DWORD nRegionType = 0;
	CDlgUtil::CobxGetItemDataByCurSel(m_RegionMalCmb, nRegionType);

	//int nSpecType = m_SpecTypeCmb.GetCurSel();
	//int nGroundType = m_GroundTypeCmb.GetCurSel();
	if(m_nNAIndex == EN_SPFC_SINGAPORE)//if(m_bSingapore)
	{
		//nSpecType+=10;
		//nGroundType+=10;
		
		GetDlgItem(IDC_ETC_GDS_EURO2003_PARATYPE_RDO)->SetWindowText(_LS(IDS_CMD_Automatic));
		GetDlgItem(IDC_ETC_GDS_EURO2003_PARATYPE_RDO2)->ShowWindow(SW_HIDE);
	}
	else if (m_nNAIndex == EN_SPFC_MALESIA)
	{
		GetDlgItem(IDC_ETC_GDS_EURO2003_PARATYPE_RDO)->SetWindowText(_LS(IDS_CMD_MALESIA_TYPE1));
		GetDlgItem(IDC_ETC_GDS_EURO2003_PARATYPE_RDO2)->SetWindowText(_LS(IDS_CMD_MALESIA_TYPE2));
		GetDlgItem(IDC_ETC_GDS_EURO2003_PARATYPE_RDO2)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_ETC_GDS_EURO2003_PARATYPE_RDO)->SetWindowText(_LS(IDS_CMD_Type1));
		GetDlgItem(IDC_ETC_GDS_EURO2003_PARATYPE_RDO2)->SetWindowText(_LS(IDS_CMD_Type2));
		GetDlgItem(IDC_ETC_GDS_EURO2003_PARATYPE_RDO2)->ShowWindow(SW_SHOW);
	}

	if(nSpecType == EN_HORI_ELASTIC || nSpecType == EN_VERT_ELASTIC || nSpecType==EN_HORI_ELASTIC_SIN) bElastic = TRUE;  
	if(nSpecType == EN_VERT_ELASTIC || nSpecType == EN_VERT_DESIGN) bVertical = TRUE;    

	if (m_nNAIndex == EN_SPFC_RECOMMENDED)
	{
		if (nGroundType == EN_SPFC_GROUND_S1 || nGroundType == EN_SPFC_GROUND_S2) bSpecialGroundType = TRUE;  // S1, S2
	}
	
	// Special Ground Type(S1, S2)인 경우 Spectra Type은 User Defined로 강제 설정
	if(bSpecialGroundType) m_nParaTypeRdo = 2;  
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlParaType, !bSpecialGroundType);
	
	if(m_nParaTypeRdo == 2) bUserDefined = TRUE;
	
	CDlgUtil::CtrlShowHide(this, m_aCtrlElastic, bElastic);
	CDlgUtil::CtrlShowHide(this, m_aCtrlDesign, !bElastic);

	if(bVertical) GetDlgItem(IDC_ETC_GDS_EURO2003_ACC_TEXT)->SetWindowText(_LS(IDS_CMD_ETC_GDS_EURO2003_AgR)); // Ref. Peak Ground Acc. (AvgR)
	else          GetDlgItem(IDC_ETC_GDS_EURO2003_ACC_TEXT)->SetWindowText(_LS(IDS_CMD_ETC_GDS_EURO2003_AvgR));  // Ref. Peak Ground Acc. (AgR)

	if(bSpecialGroundType || bUserDefined) bAutomatic = FALSE;

	// Soil Factor(S)는 Vertical Spectrum인 경우   : Ground Type과 Spectrum Parameter Type의 모든 경우에 대해 항상 1.0으로 고정
	// Soil Factor(S)는 Horizontal Spectrum인 경우 : Ground Type이 S1, S2 이거나, Spectrum Parameter Type이 User Defined인 경우 사용자 입력만 가능
	// Tb, Tc, Td는 Ground Type이 S1, S2 이거나, Spectrum Parameter Type이 User Defined인 경우 사용자 입력만 가능
	m_wndS.SetReadOnly((!bSpecialGroundType && !bUserDefined) || bVertical);
	m_wndTb.SetReadOnly(!bSpecialGroundType && !bUserDefined);
	m_wndTc.SetReadOnly(!bSpecialGroundType && !bUserDefined);
	m_wndTd.SetReadOnly(!bSpecialGroundType && !bUserDefined);

	// Ground Type과 Spectrum Parameter Type에 따른 S, Tb, Tc, Td의 자동설정
	if(bAutomatic)
	{
		CSeismic_EN1998_1_2004::T_EN1998_SEIS_INFO SeisInfo;
		CSeismic_EN1998_1_2004 SeisCalc;
		SeisCalc.GetSeisInfoForSpfc(m_nNAIndex, nGroundType, m_nParaTypeRdo, nRegionType, nSpecType, SeisInfo);

		double dS = m_pSpfcUtil->GetS();
		double dTb = m_pSpfcUtil->GetTb();
		double dTc = m_pSpfcUtil->GetTc();
		double dTd = m_pSpfcUtil->GetTd();

		SeisCalc.GetSpectrumParameter(SeisInfo, dS, dTb, dTc, dTd);

		m_pSpfcUtil->SetS(dS);
		m_pSpfcUtil->SetTb(dTb);
		m_pSpfcUtil->SetTc(dTc);
		m_pSpfcUtil->SetTd(dTd);

		m_wndS.SetEditUnit(dS);
		m_wndTb.SetEditUnit(dTb);
		m_wndTc.SetEditUnit(dTc);
		m_wndTd.SetEditUnit(dTd);
	}

	if(bVertical) 
	{
		m_pSpfcUtil->SetS(1.0);   // Vertical Spectrum인 경우에는 Soil Factor(S)는 강제로 1로 설정
		m_wndS.SetEditUnit(m_pSpfcUtil->GetS());
	}
		
	UpdateData(FALSE);
}



void CETC_GDS_EURO2003::InitCtrlArray()
{
	m_aCtrlElastic.RemoveAll();
	m_aCtrlElastic.Add(IDC_ETC_GDS_EURO2003_DAMPING_TEXT);
	m_aCtrlElastic.Add(IDC_ETC_GDS_EURO2003_DAMPING_EDIT);
	m_aCtrlElastic.Add(IDC_ETC_GDS_EURO2003_DAMPING_UNIT);

	m_aCtrlDesign.RemoveAll();
	m_aCtrlDesign.Add(IDC_ETC_GDS_EURO2003_BEHAVIOR_TEXT);
	m_aCtrlDesign.Add(IDC_ETC_GDS_EURO2003_BEHAVIOR_EDIT);
	m_aCtrlDesign.Add(IDC_ETC_GDS_EURO2003_BETA_TEXT);
	m_aCtrlDesign.Add(IDC_ETC_GDS_EURO2003_BETA_EDIT);
	//m_aCtrlDesign.Add(IDC_ETC_GDS_EURO2003_IMPORTANCE_TEXT);
	//m_aCtrlDesign.Add(IDC_ETC_GDS_EURO2003_IMPORTANCE_CMB);

	m_aCtrlParaType.RemoveAll();
	m_aCtrlParaType.Add(IDC_ETC_GDS_EURO2003_PARATYPE_RDO);
	m_aCtrlParaType.Add(IDC_ETC_GDS_EURO2003_PARATYPE_RDO2);

// 	m_aCtrlEuro.RemoveAll();
// 	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlEuro, IDC_ETC_GDS_EURO2003_GRP,  FALSE);
// 	m_aCtrlMalaysia.RemoveAll();
// 	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlMalaysia, IDC_ETC_GDS_EURO2003_Mal_GRP,  FALSE);

	m_aCtrlGround.RemoveAll();
	m_aCtrlGround.Add(IDC_WG_CMD_STATIC1);
	m_aCtrlGround.Add(IDC_ETC_GDS_EURO2003_GROUND_CMB);

	m_aCtrlRegionType.RemoveAll();
	m_aCtrlRegionType.Add(IDC_ETC_GDS_EURO2003_REGION_Mal_TEXT);
	m_aCtrlRegionType.Add(IDC_ETC_GDS_EURO2003_REGION_Mal_CMB);

	m_aCtrlMove.RemoveAll();
	m_aCtrlMove.Add(IDC_WG_CMD_STATIC0);
	m_aCtrlMove.Add(IDC_ETC_GDS_EURO2003_PARATYPE_RDO);
	m_aCtrlMove.Add(IDC_ETC_GDS_EURO2003_PARATYPE_RDO2);
	m_aCtrlMove.Add(IDC_ETC_GDS_EURO2003_PARATYPE_RDO3);
	m_aCtrlMove.Add(IDC_WG_CMD_STATIC2);
	m_aCtrlMove.Add(IDC_WG_CMD_STATIC3);
	m_aCtrlMove.Add(IDC_WG_CMD_STATIC4);
	m_aCtrlMove.Add(IDC_WG_CMD_STATIC5);
	m_aCtrlMove.Add(IDC_ETC_GDS_EURO2003_S_EDIT);
	m_aCtrlMove.Add(IDC_ETC_GDS_EURO2003_TB_EDIT);
	m_aCtrlMove.Add(IDC_ETC_GDS_EURO2003_TC_EDIT);
	m_aCtrlMove.Add(IDC_ETC_GDS_EURO2003_TD_EDIT);
	m_aCtrlMove.Add(IDC_ETC_GDS_EURO2003_ACC_TEXT);
	m_aCtrlMove.Add(IDC_ETC_GDS_EURO2003_AG_EDIT);
	m_aCtrlMove.Add(IDC_WG_CMD_STATIC6);
	m_aCtrlMove.Add(IDC_ETC_GDS_EURO2003_IMPORTANCE_TEXT);
	m_aCtrlMove.Add(IDC_ETC_GDS_EURO2003_IMPORTANCE_CMB);
	m_aCtrlMove.Add(IDC_ETC_GDS_EURO2003_DAMPING_TEXT);
	m_aCtrlMove.Add(IDC_ETC_GDS_EURO2003_DAMPING_EDIT);
	m_aCtrlMove.Add(IDC_ETC_GDS_EURO2003_DAMPING_UNIT);
	m_aCtrlMove.Add(IDC_ETC_GDS_EURO2003_BEHAVIOR_TEXT);
	m_aCtrlMove.Add(IDC_ETC_GDS_EURO2003_BEHAVIOR_EDIT);
	m_aCtrlMove.Add(IDC_ETC_GDS_EURO2003_BETA_TEXT);
	m_aCtrlMove.Add(IDC_ETC_GDS_EURO2003_BETA_EDIT);
}

void CETC_GDS_EURO2003::AlignControl() 
{
	CWnd* pWnd;
	CRect FromRect, ToRect;
	pWnd = GetDlgItem(IDC_ETC_GDS_EURO2003_DAMPING_EDIT); ASSERT(pWnd);
	pWnd->GetWindowRect(&ToRect);
	pWnd = GetDlgItem(IDC_ETC_GDS_EURO2003_BEHAVIOR_EDIT); ASSERT(pWnd);
	pWnd->GetWindowRect(&FromRect);
	int nDistY = ToRect.top - FromRect.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlDesign, nDistY);  
}

void CETC_GDS_EURO2003::AlignControlByNA()
{
	CWnd* pWnd;
	CRect FromRect, ToRect;
	pWnd = GetDlgItem(IDC_ETC_GDS_EURO2003_REGION_Mal_TEXT); ASSERT(pWnd);
	pWnd->GetWindowRect(&ToRect);
	pWnd = GetDlgItem(IDC_WG_CMD_STATIC0); ASSERT(pWnd);
	pWnd->GetWindowRect(&FromRect);
	int nDistY = ToRect.top - FromRect.top;
	if (m_nNAIndex == EN_SPFC_MALESIA)
		nDistY = ToRect.bottom - FromRect.top + 5;

	CDlgUtil::CtrlMoveDistY(this, m_aCtrlMove, nDistY);
	this->Invalidate();
}

void CETC_GDS_EURO2003::UpdateDampingWnd(CWnd* pDampingWnd)
{
	CString csDamping;
	double dampingValue = m_pSpfcUtil->GetDamping();
	csDamping.Format(_T("%g"), dampingValue / 100.0);

	pDampingWnd->SetWindowText(csDamping);
}

void CETC_GDS_EURO2003::End()
{
	UpdateData(TRUE);
	Dlg2UtilData();
	UpdateData(FALSE);

	CMyChildDialog::End();
}

void CETC_GDS_EURO2003::EndNotClose()
{
	UpdateData(TRUE);
	Dlg2UtilData();
	UpdateData(FALSE);
}
BOOL CETC_GDS_EURO2003::ISVALID()
{   

	BOOL bElastic = FALSE;    
	DWORD nSpecType = 0;
	CDlgUtil::CobxGetItemDataByCurSel(m_SpecTypeCmb, nSpecType);

//   if(m_bSingapore) nSpecType+=10;
	if(nSpecType == EN_HORI_ELASTIC || nSpecType == EN_VERT_ELASTIC || nSpecType == EN_HORI_ELASTIC_SIN) bElastic = TRUE;  
	
	Dlg2UtilData();

	const double dS = m_pSpfcUtil->GetS();
	const double dTb = m_pSpfcUtil->GetTb();
	const double dTc = m_pSpfcUtil->GetTc();
	const double dTd = m_pSpfcUtil->GetTd();
	const double dAg = m_pSpfcUtil->GetAg();
	const double dDamping = m_pSpfcUtil->GetDamping();
	const double dQ = m_pSpfcUtil->GetQ();
	const double dBeta = m_pSpfcUtil->GetBeta();
	const double dImportance = m_pSpfcUtil->GetImportance();

	if(dS <= 0)      {AfxMessageBox(_T("S > 0")); return FALSE;}
	if(dTb <= 0)     {AfxMessageBox(_T("Tb > 0")); return FALSE;}
	if(m_nNAIndex == EN_SPFC_SINGAPORE) //if(m_bSingapore)
	{
		if(dTb > dTc) {AfxMessageBox(_T("Tc >= Tb")); return FALSE;}
		if(dTc > dTd) {AfxMessageBox(_T("Td >= Tc")); return FALSE;}
	}
	else
	{
		if(dTb >= dTc) {AfxMessageBox(_T("Tc > Tb")); return FALSE;}
		if(dTc >= dTd) {AfxMessageBox(_T("Td > Tc")); return FALSE;}
	}
	if(dAg <= 0)     {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__GROUND_ACC)); return FALSE;}
	if(bElastic)
	{
		if(m_nNAIndex == EN_SPFC_SINGAPORE) //if(m_bSingapore)
		{
			if(dTd > 10.4)     {AfxMessageBox(_T("Td <= 10.4")); return FALSE;}
		}
		else
		{
			if(dTd >= 4)     {AfxMessageBox(_T("Td < 4")); return FALSE;}
		}
		
		if(dDamping < 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__DAMPING_VALUE)); return FALSE;}    
	}
	else
	{
		if(dQ <= 0)    {AfxMessageBox(_T("q > 0")); return FALSE;}
		if(dBeta <= 0) {AfxMessageBox(_T("Beta > 0")); return FALSE;}
		if(dImportance <= 0) {AfxMessageBox(_T("I > 0")); return FALSE;}
	}
	if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  
	return TRUE;  
}

void CETC_GDS_EURO2003::OnSelchangeEtcGdsEuro2003SpecTypeCmb() 
{
	CtrlManager();	
}

void CETC_GDS_EURO2003::OnEtcGdsEuro2003ParatypeRdo() 
{
	CtrlManager();	
}

// void CETC_GDS_EURO2003::OnChangeTsEdit()
// {
// // 	m_dTs_Mal = m_wndTs_Mal.GetEditValue();
// 	CtrlManager();	
// }

void CETC_GDS_EURO2003::OnSelchangeEtcGdsEuro2003GroundCmb() 
{
	//if(m_bSingapore) InitIFactorCombo();
	if(m_nNAIndex == EN_SPFC_SINGAPORE || m_nNAIndex == EN_SPFC_MALESIA) InitIFactorCombo();
	CtrlManager();	
}

void CETC_GDS_EURO2003::OnSelchangeEtcGdsEuro2003RegionCmb()
{
	CtrlManager();
}
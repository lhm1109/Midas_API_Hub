// ETC_GDS_SANS.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_SANS.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\Seismic_SANS2010.h"

#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_SANS dialog


CETC_GDS_SANS::CETC_GDS_SANS(T_SPFC_D* pParamData/*=NULL*/, CWnd* pParent /*=NULL*/, int nCase/*=0*/)
	: CMyChildDialog(CETC_GDS_SANS::IDD, pParent)
{
	m_pParamData = pParamData;

	//{{AFX_DATA_INIT(CETC_GDS_SANS)
	m_nParaTypeRdo = -1;
	m_dMaxPeriod = PRD_ED;
	//}}AFX_DATA_INIT
	//m_aCtrlElastic.Add(IDC_ETC_GDS_SANS_ACC_TEXT);
	m_nCase = nCase;

	m_nNAIndex = EN_SPFC_SINGAPORE;
	m_nRegion = EN_SPFC_REGION_PENINSULAR;

	SetDefaultValue();
}


void CETC_GDS_SANS::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_SANS)
	DDX_Control(pDX, IDC_ETC_GDS_SANS_SPEC_TYPE_CMB,      m_SpecTypeCmb);
	DDX_Control(pDX, IDC_ETC_GDS_SANS_GROUND_CMB,         m_GroundTypeCmb);
	DDX_Control(pDX, IDC_ETC_GDS_SANS_AG_EDIT,            m_wndAg);
	DDX_Radio(pDX,   IDC_ETC_GDS_SANS_PARATYPE_RDO,       m_nParaTypeRdo);
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT,                        m_dMaxPeriod);	
	//
	DDX_Control(pDX, IDC_ETC_GDS_SANS_TD_EDIT,            m_wndTd);
	DDX_Control(pDX, IDC_ETC_GDS_SANS_TC_EDIT,            m_wndTc);
	DDX_Control(pDX, IDC_ETC_GDS_SANS_TB_EDIT,            m_wndTb);
	DDX_Control(pDX, IDC_ETC_GDS_SANS_S_EDIT,             m_wndS);
	DDX_Control(pDX, IDC_ETC_GDS_SANS_IMPORTANCE_CMB,     m_cmbImportance);
	DDX_Control(pDX, IDC_ETC_GDS_SANS_DAMPING_EDIT,       m_wndDamping);
	DDX_Control(pDX, IDC_ETC_GDS_SANS_BETA_EDIT,          m_wndBeta);
	DDX_Control(pDX, IDC_ETC_GDS_SANS_BEHAVIOR_EDIT,      m_wndQ);
	//
	DDX_Control(pDX, IDC_ETC_GDS_SANS_REGION_Mal_CMB,     m_RegionMalCmb);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_SANS, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_SANS)
	ON_CBN_SELCHANGE(IDC_ETC_GDS_SANS_SPEC_TYPE_CMB, OnSelchangeEtcGdsSANSSpecTypeCmb)
	ON_BN_CLICKED(IDC_ETC_GDS_SANS_PARATYPE_RDO,     OnEtcGdsSANSParatypeRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_SANS_PARATYPE_RDO2,    OnEtcGdsSANSParatypeRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_SANS_PARATYPE_RDO3,    OnEtcGdsSANSParatypeRdo)
	ON_EN_CHANGE(IDC_ETC_GDS_EURO2003_PERIOD_TS_Mal_EDIT,  OnChangeTsEdit)
	
	ON_CBN_SELCHANGE(IDC_ETC_GDS_SANS_GROUND_CMB,    OnSelchangeEtcGdsSANSGroundCmb)
	ON_CBN_SELCHANGE(IDC_ETC_GDS_SANS_REGION_Mal_CMB,OnSelchangeEtcGdsSANSRegionCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_SANS message handlers

BOOL CETC_GDS_SANS::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	if(m_pParamData!=NULL)
	{
		m_nNAIndex = EN_SPFC_SINGAPORE;// m_pParamData->CodeParam.SANS2010.nNationalAnnex;
	}
	SetDefaultValue();

	InitCtrlArray();

	InitSpectrumTypeCombo();
	InitGroundTypeCombo();
	InitRegionCombo();

	m_wndAg.SetEditUnit(m_dAg);
	// Euro
	m_wndS.SetEditUnit(m_dS);
	m_wndTb.SetEditUnit(m_dTb);
	m_wndTc.SetEditUnit(m_dTc);
	m_wndTd.SetEditUnit(m_dTd);
	m_wndDamping.SetEditUnit(m_dDamping);
	m_wndQ.SetEditUnit(m_dQ);
	m_wndBeta.SetEditUnit(m_dBeta);
	
	AlignControl();
	InitIFactorCombo(TRUE);
	CtrlManager();
	AlignControlByNA();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_SANS::InitSpectrumTypeCombo()
{
	// 0: 일반, 1: Elastic Sepctrum 삭제, 2: Design Spectrum 삭제
	m_SpecTypeCmb.ResetContent();
	if (m_nNAIndex == EN_SPFC_SINGAPORE)  //(m_bSingapore)
	{
		/*if (m_nCase != 1) CDlgUtil::CobxAddItem(m_SpecTypeCmb, _LS(IDS_CMD_EURO2003_H_ELASTIC_SPECTRUM), EN_HORI_ELASTIC_SIN);
		if (m_nCase != 2)*/ 
		CDlgUtil::CobxAddItem(m_SpecTypeCmb, _LS(IDS_CMD_EURO2003_H_DESIGN_SPECTRUM), EN_HORI_DESIGN_SIN);
	}
	else
	{
		if (m_nCase != 1)
		{
			CDlgUtil::CobxAddItem(m_SpecTypeCmb, _LS(IDS_CMD_EURO2003_H_ELASTIC_SPECTRUM), EN_HORI_ELASTIC);
			CDlgUtil::CobxAddItem(m_SpecTypeCmb, _LS(IDS_CMD_EURO2003_V_ELASTIC_SPECTRUM), EN_VERT_ELASTIC);
		}
		if (m_nCase != 2)
		{
			CDlgUtil::CobxAddItem(m_SpecTypeCmb, _LS(IDS_CMD_EURO2003_H_DESIGN_SPECTRUM), EN_HORI_DESIGN);
			CDlgUtil::CobxAddItem(m_SpecTypeCmb, _LS(IDS_CMD_EURO2003_V_DESIGN_SPECTRUM), EN_VERT_DESIGN);
		}
	}
	m_SpecTypeCmb.SetCurSel(0);
	CDlgUtil::CobxAdjustListBoxWidth(m_SpecTypeCmb);
}

void CETC_GDS_SANS::InitGroundTypeCombo()
{
	m_GroundTypeCmb.ResetContent();
	if (m_nNAIndex == EN_SPFC_SINGAPORE)//if(m_bSingapore)
	{
		CDlgUtil::CobxAddItem(m_GroundTypeCmb, _T("1"), EN_SPFC_GROUND_A);
		CDlgUtil::CobxAddItem(m_GroundTypeCmb, _T("2"), EN_SPFC_GROUND_B);
		CDlgUtil::CobxAddItem(m_GroundTypeCmb, _T("3"), EN_SPFC_GROUND_C);
		CDlgUtil::CobxAddItem(m_GroundTypeCmb, _T("4"), EN_SPFC_GROUND_D);
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
		m_GroundTypeCmb.SetCurSel(0);
	}
	CDlgUtil::CobxAdjustListBoxWidth(m_GroundTypeCmb);
}

void CETC_GDS_SANS::InitRegionCombo()
{
	m_RegionMalCmb.ResetContent();
	CDlgUtil::CobxAddItem(m_RegionMalCmb, _LS(IDC_ETC_GDS_SANS_Malaysia_Region_Pen), EN_SPFC_REGION_PENINSULAR);
	CDlgUtil::CobxAddItem(m_RegionMalCmb, _LS(IDC_ETC_GDS_SANS_Malaysia_Region_Sab), EN_SPFC_REGION_SABAH);
	CDlgUtil::CobxAddItem(m_RegionMalCmb, _LS(IDC_ETC_GDS_SANS_Malaysia_Region_Sar), EN_SPFC_REGION_SARAWAK);
	m_RegionMalCmb.SetCurSel(0);

	CDlgUtil::CobxAdjustListBoxWidth(m_RegionMalCmb);
}

BOOL CETC_GDS_SANS::Dlg2Data()
{
	if(m_pParamData == NULL)
		return FALSE; 

	// UpdateData(TRUE); // CETC_DESIGN_SPECT에서 Dlg2Data()호출 전에 이미 호출되었다.
	m_pParamData->nSpecCodeType = D_SPFC_CODE_SANS;	

	Dlg2Data(m_pParamData->CodeParam.SANS2010);
		
	return TRUE;
}

void CETC_GDS_SANS::Dlg2Data(T_SPFC_SANS2010& SANS2010)
{
	SANS2010.Initialize();

	SANS2010.nSpectrumType	= m_nSpecType;
	SANS2010.nGroundType	= m_nGroundType;
	SANS2010.nNationalAnnex = (EN_SPFC_NA) m_nNAIndex;

	SANS2010.nParameterType = m_nParaTypeRdo;
	SANS2010.dSoilFactor    = m_dS;
	SANS2010.dTb            = m_dTb;
	SANS2010.dTc            = m_dTc;  
	SANS2010.dTd            = m_dTd;
	SANS2010.dAgr           = m_dAg;
	SANS2010.dI             = m_dImportance;
	SANS2010.dXi            = m_dDamping;
	SANS2010.dQ             = m_dQ;
	SANS2010.dB             = m_dBeta;
	SANS2010.dMaxPeriod     = m_dMaxPeriod; 
	SANS2010.dTs            = 0.1;
	SANS2010.nResionType    = m_nRegion;

}

void CETC_GDS_SANS::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	if(m_pParamData == NULL)
		return;

	if(m_nNAIndex == EN_SPFC_MALESIA)
	{
		SetPushOverDataMal(bInit/*=TRUE*/);
		return;
	}

	if(m_pParamData->nSpecCodeType!= D_SPFC_CODE_SANS)
	{
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_SANS);
	}

	m_nGroundType = m_pParamData->CodeParam.SANS2010.nGroundType;
	m_nSpecType = m_pParamData->CodeParam.SANS2010.nSpectrumType;

	// Input Data
	m_nParaTypeRdo= m_pParamData->CodeParam.SANS2010.nParameterType;
	m_dS          = m_pParamData->CodeParam.SANS2010.dSoilFactor;
	m_dTb         = m_pParamData->CodeParam.SANS2010.dTb;
	m_dTc         = m_pParamData->CodeParam.SANS2010.dTc;
	m_dTd         = m_pParamData->CodeParam.SANS2010.dTd;
	m_dAg         = m_pParamData->CodeParam.SANS2010.dAgr;
	m_dImportance = m_pParamData->CodeParam.SANS2010.dI;
	m_dDamping    = m_pParamData->CodeParam.SANS2010.dXi;
	m_dQ          = m_pParamData->CodeParam.SANS2010.dQ;
	m_dBeta       = m_pParamData->CodeParam.SANS2010.dB;
	m_dMaxPeriod  = m_pParamData->CodeParam.SANS2010.dMaxPeriod;

	if(bInit)
	{
		if(CB_ERR==CDlgUtil::CobxSetCurSelItemData(m_SpecTypeCmb, m_nSpecType))
			m_SpecTypeCmb.SetCurSel(m_nSpecType);
		if (CB_ERR == CDlgUtil::CobxSetCurSelItemData(m_GroundTypeCmb, m_nGroundType))
			m_GroundTypeCmb.SetCurSel(m_nGroundType);

		m_wndS.SetEditUnit(m_dS);
		m_wndTb.SetEditUnit(m_dTb);
		m_wndTc.SetEditUnit(m_dTc);
		m_wndTd.SetEditUnit(m_dTd);

		m_wndAg.SetEditUnit(m_dAg);
		m_wndDamping.SetEditUnit(m_dDamping);
		m_wndQ.SetEditUnit(m_dQ);
		m_wndBeta.SetEditUnit(m_dBeta);  

		CString str;
		str.Format(_T("%g"), m_dImportance);
		m_cmbImportance.SetWindowText(str);  

		UpdateData(FALSE);  
	}
}
void CETC_GDS_SANS::SetPushOverDataMal(BOOL bInit/*=TRUE*/)
{
	if(m_pParamData == NULL)
		return;

// 	if(m_pParamData->CodeParam.SANS2010.nGroundType<13)
// 		return;

	if(m_pParamData->nSpecCodeType!= D_SPFC_CODE_SANS)
	{
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_SANS);
	}

	m_nNAIndex    = m_pParamData->CodeParam.SANS2010.nNationalAnnex;
	m_nGroundType = m_pParamData->CodeParam.SANS2010.nGroundType;
	m_nSpecType   = m_pParamData->CodeParam.SANS2010.nSpectrumType;
	m_nRegion     = m_pParamData->CodeParam.SANS2010.nResionType;

	// Input Data
// 	m_dS_Mal          = m_pParamData->CodeParam.SANS2010.dSoilFactor;
// 	m_dTb_Mal         = m_pParamData->CodeParam.SANS2010.dTb;
// 	m_dTc_Mal         = m_pParamData->CodeParam.SANS2010.dTc;
// 	m_dTd_Mal         = m_pParamData->CodeParam.SANS2010.dTd;
// 	m_dTs_Mal         = m_pParamData->CodeParam.SANS2010.dTs;
// 	m_dImportance_Mal = m_pParamData->CodeParam.SANS2010.dI;
// 	m_dDamping_Mal    = m_pParamData->CodeParam.SANS2010.dXi;
// 	m_dQ_Mal          = m_pParamData->CodeParam.SANS2010.dQ;
// 	m_dBeta_Mal       = m_pParamData->CodeParam.SANS2010.dB;

	m_nParaTypeRdo= m_pParamData->CodeParam.SANS2010.nParameterType;
	m_dAg         = m_pParamData->CodeParam.SANS2010.dAgr;
	m_dMaxPeriod  = m_pParamData->CodeParam.SANS2010.dMaxPeriod;

	if(bInit)
	{
		if(CB_ERR==CDlgUtil::CobxSetCurSelItemData(m_SpecTypeCmb, m_nSpecType))
			m_SpecTypeCmb.SetCurSel(m_nSpecType);
		if (CB_ERR == CDlgUtil::CobxSetCurSelItemData(m_GroundTypeCmb, m_nGroundType))
			m_GroundTypeCmb.SetCurSel(m_nGroundType);
		
		m_RegionMalCmb.SetCurSel(m_nRegion);

// 		m_wndS_Mal.SetEditUnit(m_dS_Mal);
// 		m_wndTb_Mal.SetEditUnit(m_dTb_Mal);
// 		m_wndTc_Mal.SetEditUnit(m_dTc_Mal);
// 		m_wndTd_Mal.SetEditUnit(m_dTd_Mal);
// 		m_wndTs_Mal.SetEditUnit(m_dTs_Mal);
// 		m_wndQ_Mal.SetEditUnit(m_dQ_Mal);
// 		m_wndBeta_Mal.SetEditUnit(m_dBeta_Mal);  
// 		m_wndDamping_Mal.SetEditUnit(m_dDamping_Mal);
		
		m_wndAg.SetEditUnit(m_dAg);
// 
// 		CString str;
// 		str.Format(_T("%g"), m_dImportance_Mal);
// 		m_cmbImportance_Mal.SetWindowText(str);  

		UpdateData(FALSE);  
	}
}
void CETC_GDS_SANS::Data2Dlg()
{
	if(m_pParamData == NULL)
		return;

	if(m_pParamData->nSpecCodeType!= D_SPFC_CODE_SANS)
	{
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_SANS);
	}

	if(m_nNAIndex == EN_SPFC_SINGAPORE)
	{
		m_nGroundType = m_pParamData->CodeParam.SANS2010.nGroundType;
		m_nSpecType   = m_pParamData->CodeParam.SANS2010.nSpectrumType/*-10*/;
	}
	else
	{
		m_nGroundType = m_pParamData->CodeParam.SANS2010.nGroundType;
		m_nSpecType   = m_pParamData->CodeParam.SANS2010.nSpectrumType;
	}

	// Input Data
	m_nParaTypeRdo= m_pParamData->CodeParam.SANS2010.nParameterType;
	m_dS          = m_pParamData->CodeParam.SANS2010.dSoilFactor;
	m_dTb         = m_pParamData->CodeParam.SANS2010.dTb;
	m_dTc         = m_pParamData->CodeParam.SANS2010.dTc;
	m_dTd         = m_pParamData->CodeParam.SANS2010.dTd;
	m_dAg         = m_pParamData->CodeParam.SANS2010.dAgr;
	m_dImportance = m_pParamData->CodeParam.SANS2010.dI;
	m_dDamping    = m_pParamData->CodeParam.SANS2010.dXi;
	m_dQ          = m_pParamData->CodeParam.SANS2010.dQ;
	m_dBeta       = m_pParamData->CodeParam.SANS2010.dB;
	m_dMaxPeriod  = m_pParamData->CodeParam.SANS2010.dMaxPeriod;

	if(CB_ERR==CDlgUtil::CobxSetCurSelItemData(m_SpecTypeCmb, m_nSpecType))
		m_SpecTypeCmb.SetCurSel(m_nSpecType);
	if (CB_ERR == CDlgUtil::CobxSetCurSelItemData(m_GroundTypeCmb, m_nGroundType))
		m_GroundTypeCmb.SetCurSel(m_nGroundType);
	
	m_wndS.SetEditUnit(m_dS);
	m_wndTb.SetEditUnit(m_dTb);
	m_wndTc.SetEditUnit(m_dTc);
	m_wndTd.SetEditUnit(m_dTd);
	
	m_wndAg.SetEditUnit(m_dAg);
	m_wndDamping.SetEditUnit(m_dDamping);
	m_wndQ.SetEditUnit(m_dQ);
	m_wndBeta.SetEditUnit(m_dBeta);  

	CString str;
	str.Format(_T("%g"), m_dImportance);
	m_cmbImportance.SetWindowText(str);  

	UpdateData(FALSE);  
}

void CETC_GDS_SANS::SetNationalAnnex(int nNAIndex)
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
		m_pParamData->CodeParam.SANS2010.nGroundType = EN_SPFC_GROUND_B;
		if(m_pParamData->CodeParam.SANS2010.nParameterType==1) m_pParamData->CodeParam.SANS2010.nParameterType=0;
		//m_pParamData->CodeParam.SANS2010.dAgr = M_InitValueCurUnit(0.0178, N, M, D_UNITSYS_BASE_LENGTH);
		m_pParamData->CodeParam.SANS2010.dI = 1.0;
	}
	else if(m_nNAIndex == EN_SPFC_SINGAPORE)//if(m_bSingapore)
	{
		if(m_nParaTypeRdo==1) m_nParaTypeRdo=0;
		m_pParamData->CodeParam.SANS2010.nGroundType = EN_SPFC_GROUND_C;
		if(m_pParamData->CodeParam.SANS2010.nParameterType==1) m_pParamData->CodeParam.SANS2010.nParameterType=0;
		m_pParamData->CodeParam.SANS2010.dAgr = M_InitValueCurUnit(0.0178, N, M, D_UNITSYS_BASE_LENGTH);
		m_pParamData->CodeParam.SANS2010.dI = 1.0;
	}
	else
	{
		m_pParamData->CodeParam.SANS2010.nGroundType = EN_SPFC_GROUND_B;
		m_pParamData->CodeParam.SANS2010.dAgr = M_InitValueCurUnit(0.08, N, M, D_UNITSYS_BASE_LENGTH);
		m_pParamData->CodeParam.SANS2010.dI = 1.0;
	}
	m_pParamData->CodeParam.SANS2010.nNationalAnnex = (EN_SPFC_NA)m_nNAIndex;
	CDlgUtil::CobxSetCurSelItemData(m_GroundTypeCmb, m_pParamData->CodeParam.SANS2010.nGroundType);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlParaType, m_pParamData->CodeParam.SANS2010.nParameterType);
	m_wndAg.SetEditUnit(m_pParamData->CodeParam.SANS2010.dAgr);
	InitIFactorCombo();
	
	OnEtcGdsSANSParatypeRdo();
}

void CETC_GDS_SANS::ShowDlgData() 
{
	if(m_pParamData == NULL)
		return;

	if(m_pParamData->nSpecCodeType == D_SPFC_CODE_SANS)
		m_CodeD.SANS2010 = m_pParamData->CodeParam.SANS2010;
	else
		m_CodeD.SANS2010.Initialize();

	m_nNAIndex = m_CodeD.SANS2010.nNationalAnnex;

	if(m_nCase != 0)
	{
		if     (m_nCase==1)  m_CodeD.SANS2010.nSpectrumType = m_nNAIndex == EN_SPFC_SINGAPORE ? EN_HORI_DESIGN_SIN : EN_HORI_DESIGN;
		else if(m_nCase==2)  m_CodeD.SANS2010.nSpectrumType = m_nNAIndex == EN_SPFC_SINGAPORE ? EN_HORI_ELASTIC_SIN : EN_HORI_ELASTIC;
	}

	if(m_nNAIndex == EN_SPFC_SINGAPORE)
	{
		InitSpectrumTypeCombo();
		InitGroundTypeCombo();
	}

	m_nSpecType   = m_CodeD.SANS2010.nSpectrumType;
	m_nGroundType = m_CodeD.SANS2010.nGroundType;
	m_nRegion	  = m_CodeD.SANS2010.nResionType;
	m_nParaTypeRdo= m_CodeD.SANS2010.nParameterType;
	m_dS          = m_CodeD.SANS2010.dSoilFactor;
	m_dTb         = m_CodeD.SANS2010.dTb;
	m_dTc         = m_CodeD.SANS2010.dTc;
	m_dTd         = m_CodeD.SANS2010.dTd;
	m_dAg         = m_CodeD.SANS2010.dAgr;
	m_dImportance = m_CodeD.SANS2010.dI;
	m_dDamping    = m_CodeD.SANS2010.dXi;
	m_dQ          = m_CodeD.SANS2010.dQ;
	m_dBeta       = m_CodeD.SANS2010.dB;
	m_dMaxPeriod  = m_CodeD.SANS2010.dMaxPeriod;  

	//m_SpecTypeCmb.SetCurSel(m_nSpecType);
	CDlgUtil::CobxSetCurSelItemData(m_SpecTypeCmb, m_nSpecType);
	CDlgUtil::CobxSetCurSelItemData(m_GroundTypeCmb, m_nGroundType);
	CDlgUtil::CobxSetCurSelItemData(m_RegionMalCmb, m_nRegion);
	
	m_wndS.SetEditUnit(m_dS);
	m_wndTb.SetEditUnit(m_dTb);
	m_wndTc.SetEditUnit(m_dTc);
	m_wndTd.SetEditUnit(m_dTd);
	
	m_wndAg.SetEditUnit(m_dAg);
	m_wndDamping.SetEditUnit(m_dDamping);
	m_wndQ.SetEditUnit(m_dQ);
	m_wndBeta.SetEditUnit(m_dBeta);  
	
	UpdateData(FALSE);

	InitIFactorCombo(TRUE);
	CtrlManager();

	CString str;
	str.Format(_T("%g"), m_dImportance);
	m_cmbImportance.SetWindowText(str);  

	UpdateData(FALSE);   
}

void CETC_GDS_SANS::Initialize() 
{
	if(m_pParamData == NULL)
		return;

	m_pParamData->CodeParam.Initialize(D_SPFC_CODE_SANS);

	if(m_nNAIndex == EN_SPFC_SINGAPORE)//if(m_bSingapore)
	{
		m_pParamData->CodeParam.SANS2010.nGroundType = EN_SPFC_GROUND_C;
		m_pParamData->CodeParam.SANS2010.dAgr = M_InitValueCurUnit(0.0178, N, M, D_UNITSYS_BASE_LENGTH);
		m_pParamData->CodeParam.SANS2010.dI = 1.0;
	}
}

void CETC_GDS_SANS::SetDefaultValue()
{
	m_nParaTypeRdo = 0;
	m_dAg= 0.1;
	
	m_dS = 1.0;
	m_dTb= 0.15;
	m_dTc= 0.4;
	m_dTd= 2.0;
	m_dDamping = 5.0;
	m_dQ = 1.0;
	m_dBeta= 0.2;  
	m_dImportance = 1.0;
}

void CETC_GDS_SANS::InitIFactorCombo(BOOL bInit/*=FALSE*/)
{
	m_cmbImportance.ResetContent();
	if(m_nNAIndex == EN_SPFC_SINGAPORE)//if(m_bSingapore)
	{
		if(bInit)
		{
			if(m_pParamData->CodeParam.SANS2010.nGroundType==10)
			{
				CString strFactor = _T("1.0");
				m_cmbImportance.AddString(strFactor);
			}
			else
			{
				CString strFactor = _T("1.0");
				m_cmbImportance.AddString(strFactor);;
			}
		}
		else
		{
			if(m_GroundTypeCmb.GetCurSel()==0)
			{
				CString strFactor = _T("1.0");
				m_cmbImportance.AddString(strFactor);
			}
			else
			{
				CString strFactor = _T("1.0");
				m_cmbImportance.AddString(strFactor);
			}
		}
		m_cmbImportance.SetCurSel(0);
	}
	else
	{
		CString strFactor = _T("1.0");
		m_cmbImportance.AddString(strFactor);
		
		m_cmbImportance.SetCurSel(0);
	}
}

void CETC_GDS_SANS::CtrlManager()
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
		
		GetDlgItem(IDC_ETC_GDS_SANS_PARATYPE_RDO)->SetWindowText(_T("Code Based"));
		GetDlgItem(IDC_ETC_GDS_SANS_PARATYPE_RDO2)->ShowWindow(SW_HIDE);
	}
	else if (m_nNAIndex == EN_SPFC_MALESIA)
	{
		GetDlgItem(IDC_ETC_GDS_SANS_PARATYPE_RDO)->SetWindowText(_LS(IDS_CMD_MALESIA_TYPE1));
		GetDlgItem(IDC_ETC_GDS_SANS_PARATYPE_RDO2)->SetWindowText(_LS(IDS_CMD_MALESIA_TYPE2));
		GetDlgItem(IDC_ETC_GDS_SANS_PARATYPE_RDO2)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_ETC_GDS_SANS_PARATYPE_RDO)->SetWindowText(_LS(IDS_CMD_Type1));
		GetDlgItem(IDC_ETC_GDS_SANS_PARATYPE_RDO2)->SetWindowText(_LS(IDS_CMD_Type2));
		GetDlgItem(IDC_ETC_GDS_SANS_PARATYPE_RDO2)->ShowWindow(SW_SHOW);
	}

	if(nSpecType == EN_HORI_ELASTIC || nSpecType == EN_VERT_ELASTIC || nSpecType==EN_HORI_ELASTIC_SIN) bElastic = TRUE;  
	if(nSpecType == EN_VERT_ELASTIC || nSpecType == EN_VERT_DESIGN) bVertical = TRUE;    
	if(nGroundType == EN_SPFC_GROUND_S1 || nGroundType == EN_SPFC_GROUND_S2) bSpecialGroundType = TRUE;  // S1, S2
	
	// Special Ground Type(S1, S2)인 경우 Spectra Type은 User Defined로 강제 설정
	if(bSpecialGroundType) m_nParaTypeRdo = 2;  
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlParaType, !bSpecialGroundType);
	
	if(m_nParaTypeRdo == 2) bUserDefined = TRUE;
	
	CDlgUtil::CtrlShowHide(this, m_aCtrlElastic, bElastic);
	CDlgUtil::CtrlShowHide(this, m_aCtrlDesign, !bElastic);

	if(bVertical) GetDlgItem(IDC_ETC_GDS_SANS_ACC_TEXT)->SetWindowText(_LS(IDS_CMD_ETC_GDS_EURO2003_AgR)); // Ref. Peak Ground Acc. (AvgR)
	else          GetDlgItem(IDC_ETC_GDS_SANS_ACC_TEXT)->SetWindowText(_LS(IDS_CMD_ETC_GDS_EURO2003_AvgR));  // Ref. Peak Ground Acc. (AgR)

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
		CSeismic_SANS2010::T_SANS2010_SEIS_INFO SeisInfo;
		CSeismic_SANS2010 SeisCalc;
		SeisCalc.GetSeisInfoForSpfc(m_nNAIndex, nGroundType, m_nParaTypeRdo, nRegionType, nSpecType, SeisInfo);
		SeisCalc.GetSpectrumParameter(SeisInfo, m_dS, m_dTb, m_dTc, m_dTd);

		m_wndS.SetEditUnit(m_dS);
		m_wndTb.SetEditUnit(m_dTb);
		m_wndTc.SetEditUnit(m_dTc);
		m_wndTd.SetEditUnit(m_dTd);  
	}

	if(bVertical) 
	{
		m_dS = 1.0;   // Vertical Spectrum인 경우에는 Soil Factor(S)는 강제로 1로 설정
		m_wndS.SetEditUnit(m_dS);
	}
		
	UpdateData(FALSE);
}



void CETC_GDS_SANS::InitCtrlArray()
{
	m_aCtrlElastic.RemoveAll();
	m_aCtrlElastic.Add(IDC_ETC_GDS_SANS_DAMPING_TEXT);
	m_aCtrlElastic.Add(IDC_ETC_GDS_SANS_DAMPING_EDIT);
	m_aCtrlElastic.Add(IDC_ETC_GDS_SANS_DAMPING_UNIT);

	m_aCtrlDesign.RemoveAll();
	m_aCtrlDesign.Add(IDC_ETC_GDS_SANS_BEHAVIOR_TEXT);
	m_aCtrlDesign.Add(IDC_ETC_GDS_SANS_BEHAVIOR_EDIT);
	m_aCtrlDesign.Add(IDC_ETC_GDS_SANS_BETA_TEXT);
	m_aCtrlDesign.Add(IDC_ETC_GDS_SANS_BETA_EDIT);
	//m_aCtrlDesign.Add(IDC_ETC_GDS_SANS_IMPORTANCE_TEXT);
	//m_aCtrlDesign.Add(IDC_ETC_GDS_SANS_IMPORTANCE_CMB);

	m_aCtrlParaType.RemoveAll();
	m_aCtrlParaType.Add(IDC_ETC_GDS_SANS_PARATYPE_RDO);
	m_aCtrlParaType.Add(IDC_ETC_GDS_SANS_PARATYPE_RDO2);

// 	m_aCtrlEuro.RemoveAll();
// 	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlEuro, IDC_ETC_GDS_SANS_GRP,  FALSE);
// 	m_aCtrlMalaysia.RemoveAll();
// 	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlMalaysia, IDC_ETC_GDS_SANS_Mal_GRP,  FALSE);

	m_aCtrlGround.RemoveAll();
	m_aCtrlGround.Add(IDC_WG_CMD_STATIC1);
	m_aCtrlGround.Add(IDC_ETC_GDS_SANS_GROUND_CMB);

	m_aCtrlRegionType.RemoveAll();
	m_aCtrlRegionType.Add(IDC_ETC_GDS_SANS_REGION_Mal_TEXT);
	m_aCtrlRegionType.Add(IDC_ETC_GDS_SANS_REGION_Mal_CMB);

	m_aCtrlMove.RemoveAll();
	m_aCtrlMove.Add(IDC_WG_CMD_STATIC0);
	m_aCtrlMove.Add(IDC_ETC_GDS_SANS_PARATYPE_RDO);
	m_aCtrlMove.Add(IDC_ETC_GDS_SANS_PARATYPE_RDO2);
	m_aCtrlMove.Add(IDC_ETC_GDS_SANS_PARATYPE_RDO3);
	m_aCtrlMove.Add(IDC_WG_CMD_STATIC2);
	m_aCtrlMove.Add(IDC_WG_CMD_STATIC3);
	m_aCtrlMove.Add(IDC_WG_CMD_STATIC4);
	m_aCtrlMove.Add(IDC_WG_CMD_STATIC5);
	m_aCtrlMove.Add(IDC_ETC_GDS_SANS_S_EDIT);
	m_aCtrlMove.Add(IDC_ETC_GDS_SANS_TB_EDIT);
	m_aCtrlMove.Add(IDC_ETC_GDS_SANS_TC_EDIT);
	m_aCtrlMove.Add(IDC_ETC_GDS_SANS_TD_EDIT);
	m_aCtrlMove.Add(IDC_ETC_GDS_SANS_ACC_TEXT);
	m_aCtrlMove.Add(IDC_ETC_GDS_SANS_AG_EDIT);
	m_aCtrlMove.Add(IDC_WG_CMD_STATIC6);
	m_aCtrlMove.Add(IDC_ETC_GDS_SANS_IMPORTANCE_TEXT);
	m_aCtrlMove.Add(IDC_ETC_GDS_SANS_IMPORTANCE_CMB);
	m_aCtrlMove.Add(IDC_ETC_GDS_SANS_DAMPING_TEXT);
	m_aCtrlMove.Add(IDC_ETC_GDS_SANS_DAMPING_EDIT);
	m_aCtrlMove.Add(IDC_ETC_GDS_SANS_DAMPING_UNIT);
	m_aCtrlMove.Add(IDC_ETC_GDS_SANS_BEHAVIOR_TEXT);
	m_aCtrlMove.Add(IDC_ETC_GDS_SANS_BEHAVIOR_EDIT);
	m_aCtrlMove.Add(IDC_ETC_GDS_SANS_BETA_TEXT);
	m_aCtrlMove.Add(IDC_ETC_GDS_SANS_BETA_EDIT);
}

void CETC_GDS_SANS::AlignControl() 
{
	CWnd* pWnd;
	CRect FromRect, ToRect;
	pWnd = GetDlgItem(IDC_ETC_GDS_SANS_DAMPING_EDIT); ASSERT(pWnd);
	pWnd->GetWindowRect(&ToRect);
	pWnd = GetDlgItem(IDC_ETC_GDS_SANS_BEHAVIOR_EDIT); ASSERT(pWnd);
	pWnd->GetWindowRect(&FromRect);
	int nDistY = ToRect.top - FromRect.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlDesign, nDistY);  
}

void CETC_GDS_SANS::AlignControlByNA()
{
	CWnd* pWnd;
	CRect FromRect, ToRect;
	pWnd = GetDlgItem(IDC_ETC_GDS_SANS_REGION_Mal_TEXT); ASSERT(pWnd);
	pWnd->GetWindowRect(&ToRect);
	pWnd = GetDlgItem(IDC_WG_CMD_STATIC0); ASSERT(pWnd);
	pWnd->GetWindowRect(&FromRect);
	int nDistY = ToRect.top - FromRect.top;
	if (m_nNAIndex == EN_SPFC_MALESIA)
		nDistY = ToRect.bottom - FromRect.top + 5;

	CDlgUtil::CtrlMoveDistY(this, m_aCtrlMove, nDistY);
	this->Invalidate();
}

void CETC_GDS_SANS::UpdateDampingWnd(CWnd* pDampingWnd)
{
	CString csDamping;
	double dampingValue = m_dDamping;
	csDamping.Format(_T("%g"), dampingValue / 100.0);

	pDampingWnd->SetWindowText(csDamping);
}

void CETC_GDS_SANS::MakeSpectrumData(BOOL bOnlyCalc/*=FALSE*/)
{
	T_SPFC_SANS2010 SANS2010;
	Dlg2Data(SANS2010);

	BOOL bPushover = FALSE;
	if(bOnlyCalc)
	{
		bPushover = TRUE;
	}
	
	double SRa = 1.0;
	double SRv = 1.0;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetSpectralData_SANS2010(bPushover, SRa, SRv, &SANS2010, m_parPeriod, m_parAccel);
	
	if(bOnlyCalc) return;

	if(m_nNAIndex == EN_SPFC_SINGAPORE) //if(m_bSingapore)
	{
		CString strSpecType[2] = {_T("SANS 10160-4:2010 H-ELASTIC"), _T("SANS 10160-4:2010 H-DESIGN")};
		CString strGroundType[4] = {_T("1"), _T("2"), _T("3"), _T("4") };
		
		int nGroundIx = EN_SPFC_GROUND_C;
		switch (m_nGroundType)
		{
		case EN_SPFC_GROUND_A: nGroundIx = 0; break;
		case EN_SPFC_GROUND_B: nGroundIx = 1; break;
		case EN_SPFC_GROUND_C: nGroundIx = 2; break;
		case EN_SPFC_GROUND_D: nGroundIx = 3; break;
		default:
			ASSERT(0);
			nGroundIx = 0;
			break;
		}

		if(m_nSpecType == EN_HORI_ELASTIC_SIN)
			m_sDescript.Format(_LS(IDS_CMD_SANS_H_ELA_PROFILE), strGroundType[nGroundIx], m_dS, m_dTb, m_dTc, m_dTd, m_dAg, m_dImportance, m_dDamping);
		else if(m_nSpecType == EN_HORI_DESIGN_SIN)
			m_sDescript.Format(_LS(IDS_CMD_SANS_H_DGN_PROFILE), strGroundType[nGroundIx], m_dS, m_dTb, m_dTc, m_dTd, m_dAg, m_dQ, m_dBeta);
		
		m_strFuncName = _T("SANS 10160-4:2010 H-DESIGN");// strSpecType[m_nSpecType - 10];
	}
	else
	{
		CString strSpecType[4] = {_T("SANS 10160-4:2010 H-ELASTIC"), _T("SANS 10160-4:2010 V-ELASTIC"), _T("SANS 10160-4:2010 H-DESIGN"), _T("SANS 10160-4:2010 V-DESIGN")};
		CString strGroundType[7] = {_T("A"), _T("B"), _T("C"), _T("D"), _T("E"), _T("S1"), _T("S2")};
		
		if(m_nSpecType == 0)
			m_sDescript.Format(_LS(IDS_CMD_SANS_H_ELA_PROFILE), strGroundType[m_nGroundType], m_dS, m_dTb, m_dTc, m_dTd, m_dAg, m_dImportance, m_dDamping);
		else if(m_nSpecType == 1)
			m_sDescript.Format(_LS(IDS_CMD_SANS_V_ELA_PROFILE), strGroundType[m_nGroundType], m_dS, m_dTb, m_dTc, m_dTd, m_dAg, m_dImportance, m_dDamping);
		else if(m_nSpecType == 2)
			m_sDescript.Format(_LS(IDS_CMD_SANS_H_DGN_PROFILE), strGroundType[m_nGroundType], m_dS, m_dTb, m_dTc, m_dTd, m_dAg, m_dQ, m_dBeta);
		else if(m_nSpecType == 3)
			m_sDescript.Format(_LS(IDS_CMD_SANS_V_DGN_PROFILE), strGroundType[m_nGroundType], m_dS, m_dTb, m_dTc, m_dTd, m_dAg, m_dImportance, m_dQ, m_dBeta);
		
		m_strFuncName = strSpecType[m_nSpecType];
	}
	
}


void CETC_GDS_SANS::MakePOSpectrumData(double dDamping,double SRa,double SRv,CArray <double,double>& aPeriod,CArray <double,double>& aAccel)// for Pushover Curve
{
	T_SPFC_SANS2010 SANS2010;
	Dlg2Data(SANS2010);

	aPeriod.RemoveAll();
	aAccel.RemoveAll();
 
	if(dDamping >= 0)
	{
		SRa = (3.21-0.68*log(dDamping))/2.12;
		SRv = (2.31-0.41*log(dDamping))/1.65;
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetSpectralData_SANS2010(TRUE, SRa, SRv, &SANS2010, &aPeriod, &aAccel);
}
	
void CETC_GDS_SANS::End()
{

	UpdateData(TRUE);
		
	DWORD nSpecType = 0;
	if(CDlgUtil::CobxGetItemDataByCurSel(m_SpecTypeCmb, nSpecType))  m_nSpecType = nSpecType;
	else { ASSERT(0); m_nSpecType = 0; }

	DWORD nGroundType = 0;
	if (CDlgUtil::CobxGetItemDataByCurSel(m_GroundTypeCmb, nGroundType))  m_nGroundType = nGroundType;
	else { ASSERT(0); m_nGroundType = 0; }

	DWORD nRegionType = 0;
	if (CDlgUtil::CobxGetItemDataByCurSel(m_RegionMalCmb, nRegionType))  m_nRegion = nRegionType;
	else { ASSERT(0); m_nRegion = 0; }

/*	m_nRegion = m_RegionMalCmb.GetCurSel();*/
//   m_nSpecType = m_SpecTypeCmb.GetCurSel();
//	m_nGroundType = m_GroundTypeCmb.GetCurSel();

	m_dS = m_wndS.GetEditValue();
	m_dTb = m_wndTb.GetEditValue();
	m_dTc = m_wndTc.GetEditValue();
	m_dTd = m_wndTd.GetEditValue();
	m_dAg = m_wndAg.GetEditValue();
	m_dDamping = m_wndDamping.GetEditValue();
	m_dQ = m_wndQ.GetEditValue();
	m_dBeta = m_wndBeta.GetEditValue();
	
	CString str;
	m_cmbImportance.GetWindowText(str);  
	m_dImportance = _tstof(str);  

	CMyChildDialog::End();
}

void CETC_GDS_SANS::EndNotClose()
{
	UpdateData(TRUE);
	
	DWORD nSpecType = 0;
	if(CDlgUtil::CobxGetItemDataByCurSel(m_SpecTypeCmb, nSpecType))  m_nSpecType = nSpecType;
	else { ASSERT(0); m_nSpecType = 0; }

	DWORD nGroundType = 0;
	if (CDlgUtil::CobxGetItemDataByCurSel(m_GroundTypeCmb, nGroundType))  m_nGroundType = nGroundType;
	else { ASSERT(0); m_nGroundType = 0; }

	DWORD nRegionType = 0;
	if (CDlgUtil::CobxGetItemDataByCurSel(m_RegionMalCmb, nRegionType))  m_nRegion = nRegionType;
	else { ASSERT(0); m_nRegion = 0; }
// 	m_nGroundType = m_GroundTypeCmb.GetCurSel();
	
	m_dS = m_wndS.GetEditValue();
	m_dTb = m_wndTb.GetEditValue();
	m_dTc = m_wndTc.GetEditValue();
	m_dTd = m_wndTd.GetEditValue();
	m_dAg = m_wndAg.GetEditValue();
	m_dDamping = m_wndDamping.GetEditValue();
	m_dQ = m_wndQ.GetEditValue();
	m_dBeta = m_wndBeta.GetEditValue();
	
	CString str;
	m_cmbImportance.GetWindowText(str);  
	m_dImportance = _tstof(str);
}
BOOL CETC_GDS_SANS::ISVALID()
{   

	BOOL bElastic = FALSE;    
	DWORD nSpecType = 0;
	CDlgUtil::CobxGetItemDataByCurSel(m_SpecTypeCmb, nSpecType);

//   if(m_bSingapore) nSpecType+=10;
	if(nSpecType == EN_HORI_ELASTIC || nSpecType == EN_VERT_ELASTIC || nSpecType == EN_HORI_ELASTIC_SIN) bElastic = TRUE;  
	
	m_dS = m_wndS.GetEditValue();
	m_dTb = m_wndTb.GetEditValue();
	m_dTc = m_wndTc.GetEditValue();
	m_dTd = m_wndTd.GetEditValue();
	m_dAg = m_wndAg.GetEditValue();
	m_dDamping = m_wndDamping.GetEditValue();
	m_dQ = m_wndQ.GetEditValue();
	m_dBeta = m_wndBeta.GetEditValue();

	CString str;
	m_cmbImportance.GetWindowText(str);  
	m_dImportance = _tstof(str);  

	if(m_dS <= 0)      {AfxMessageBox(_T("S > 0")); return FALSE;}
	if(m_dTb <= 0)     {AfxMessageBox(_T("Tb > 0")); return FALSE;}
	if(m_nNAIndex == EN_SPFC_SINGAPORE) //if(m_bSingapore)
	{
		if(m_dTb > m_dTc) {AfxMessageBox(_T("Tc >= Tb")); return FALSE;}
		if(m_dTc > m_dTd) {AfxMessageBox(_T("Td >= Tc")); return FALSE;}
	}
	else
	{
		if(m_dTb >= m_dTc) {AfxMessageBox(_T("Tc > Tb")); return FALSE;}
		if(m_dTc >= m_dTd) {AfxMessageBox(_T("Td > Tc")); return FALSE;}
	}
	if(m_dAg <= 0)     {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__GROUND_ACC)); return FALSE;}
	if(bElastic)
	{
		if(m_nNAIndex == EN_SPFC_SINGAPORE) //if(m_bSingapore)
		{
			if(m_dTd > 10.4)     {AfxMessageBox(_T("Td <= 10.4")); return FALSE;}
		}
		else
		{
			if(m_dTd >= 4)     {AfxMessageBox(_T("Td < 4")); return FALSE;}
		}
		
		if(m_dDamping < 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__DAMPING_VALUE)); return FALSE;}    
	}
	else
	{
		if(m_dQ <= 0)    {AfxMessageBox(_T("q > 0")); return FALSE;}
		if(m_dBeta <= 0) {AfxMessageBox(_T("Beta > 0")); return FALSE;}
		if(m_dImportance <= 0) {AfxMessageBox(_T("I > 0")); return FALSE;}
	}
	if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  
	return TRUE;  
}

void CETC_GDS_SANS::OnSelchangeEtcGdsSANSSpecTypeCmb() 
{
	CtrlManager();	
}

void CETC_GDS_SANS::OnEtcGdsSANSParatypeRdo() 
{
	CtrlManager();	
}

void CETC_GDS_SANS::OnChangeTsEdit()
{
// 	m_dTs_Mal = m_wndTs_Mal.GetEditValue();
	CtrlManager();	
}

void CETC_GDS_SANS::OnSelchangeEtcGdsSANSGroundCmb() 
{
	//if(m_bSingapore) InitIFactorCombo();
	if(m_nNAIndex == EN_SPFC_SINGAPORE || m_nNAIndex == EN_SPFC_MALESIA) InitIFactorCombo();
	CtrlManager();	
}

void CETC_GDS_SANS::OnSelchangeEtcGdsSANSRegionCmb()
{
	CtrlManager();
}
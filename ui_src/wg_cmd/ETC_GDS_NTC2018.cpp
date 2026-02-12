// ETC_GDS_NTC2018.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_NTC2018.h"
#include "SpfcNTC2018Util.h"


#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_base\DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_NTC2018 dialog


CETC_GDS_NTC2018::CETC_GDS_NTC2018(T_SPFC_D* pParamData/*=NULL*/, CWnd* pParent /*=NULL*/, int nCase/*=0*/)
	: CMyChildDialog(CETC_GDS_NTC2018::IDD, pParent, std::make_shared<CSpfcNTC2018Util>(nCase))
{
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcNTC2018Util>(m_pSpfcBaseUtil);

	//{{AFX_DATA_INIT(CETC_GDS_NTC2018)
	m_nParaTypeRdo = -1;
	m_dMaxPeriod = PRD_ED;
	//}}AFX_DATA_INIT
	//m_aCtrlElastic.Add(IDC_ETC_GDS_NTC2018_ACC_TEXT);
	m_nCase = nCase;
	
	SetDefaultValue();
}


void CETC_GDS_NTC2018::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_NTC2018)
	DDX_Control(pDX, IDC_ETC_GDS_NTC2018_TD_EDIT, m_wndTd);
	DDX_Control(pDX, IDC_ETC_GDS_NTC2018_TC_EDIT, m_wndTc);
	DDX_Control(pDX, IDC_ETC_GDS_NTC2018_TB_EDIT, m_wndTb);
	DDX_Control(pDX, IDC_ETC_GDS_NTC2018_S_EDIT, m_wndS);
	DDX_Control(pDX, IDC_ETC_GDS_NTC2018_SPEC_TYPE_CMB, m_SpecTypeCmb);
	DDX_Control(pDX, IDC_ETC_GDS_NTC2018_GROUND_CMB, m_GroundTypeCmb);
	DDX_Control(pDX, IDC_ETC_GDS_NTC2018_AG_EDIT, m_wndAg);
	DDX_Control(pDX, IDC_ETC_GDS_NTC2018_AMP_EDIT, m_wndFo);
	DDX_Control(pDX, IDC_ETC_GDS_NTC2018_TCSTAR_EDIT, m_wndTcStar);
	DDX_Control(pDX, IDC_ETC_GDS_NTC2018_DAMPING_EDIT, m_wndDamping);
	DDX_Control(pDX, IDC_ETC_GDS_NTC2018_BEHAVIOR_EDIT, m_wndQ);
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);	
	DDX_Radio(pDX, IDC_ETC_GDS_NTC2018_PARATYPE_RDO, m_nParaTypeRdo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_NTC2018, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_NTC2018)
	ON_CBN_SELCHANGE(IDC_ETC_GDS_NTC2018_SPEC_TYPE_CMB, OnSelchangeEtcGdsNTC2018SpecTypeCmb)
	ON_BN_CLICKED(IDC_ETC_GDS_NTC2018_PARATYPE_RDO,		OnEtcGdsNTC2018ParatypeRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_NTC2018_PARATYPE_RDO2,	OnEtcGdsNTC2018ParatypeRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_NTC2018_PARATYPE_RDO3,	OnEtcGdsNTC2018ParatypeRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_NTC2018_PARATYPE_RDO4,	OnEtcGdsNTC2018ParatypeRdo)
	ON_BN_CLICKED(IDC_ETC_GDS_NTC2018_PARATYPE_RDO5,	OnEtcGdsNTC2018ParatypeRdo)
	ON_CBN_SELCHANGE(IDC_ETC_GDS_NTC2018_GROUND_CMB,	OnSelchangeEtcGdsNTC2018GroundCmb)
	ON_EN_CHANGE(IDC_ETC_GDS_NTC2018_AG_EDIT,			OnChangeCmdAgEdit)
	ON_EN_CHANGE(IDC_ETC_GDS_NTC2018_AMP_EDIT,			OnChangeCmdAmpEdit)
	ON_EN_CHANGE(IDC_ETC_GDS_NTC2018_TCSTAR_EDIT,		OnChangeCmdTcStarEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_NTC2018 message handlers

BOOL CETC_GDS_NTC2018::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	SetDefaultValue();

	m_aCtrlElastic.RemoveAll();
	m_aCtrlElastic.Add(IDC_ETC_GDS_NTC2018_DAMPING_TEXT);
	m_aCtrlElastic.Add(IDC_ETC_GDS_NTC2018_DAMPING_EDIT);
	m_aCtrlElastic.Add(IDC_ETC_GDS_NTC2018_DAMPING_UNIT);
		
	m_aCtrlDesign.RemoveAll();
	m_aCtrlDesign.Add(IDC_ETC_GDS_NTC2018_BEHAVIOR_TEXT);
	m_aCtrlDesign.Add(IDC_ETC_GDS_NTC2018_BEHAVIOR_EDIT); 

	InitSpectrumTypeCombo();
	InitGroundTypeCombo();
	
	m_wndS.SetEditUnit(m_pSpfcUtil->GetS());
	m_wndTb.SetEditUnit(m_pSpfcUtil->GetTb());
	m_wndTc.SetEditUnit(m_pSpfcUtil->GetTc());
	m_wndTd.SetEditUnit(m_pSpfcUtil->GetTd());

	m_wndAg.SetEditUnit(m_pSpfcUtil->GetAg());
	m_wndFo.SetEditUnit(m_pSpfcUtil->GetFo());
	m_wndTcStar.SetEditUnit(m_pSpfcUtil->GetTcStar());
	m_wndDamping.SetEditUnit(m_pSpfcUtil->GetDamping());
	m_wndQ.SetEditUnit(m_pSpfcUtil->GetQ());
	
	AlignControl();
	CtrlManager();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_NTC2018::InitSpectrumTypeCombo()
{
	// 0: 일반, 1: Elastic Sepctrum 삭제, 2: Design Spectrum 삭제
	m_SpecTypeCmb.ResetContent();
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
	m_SpecTypeCmb.SetCurSel(0);
	CDlgUtil::CobxAdjustListBoxWidth(m_SpecTypeCmb);
}

void CETC_GDS_NTC2018::InitGroundTypeCombo()
{
	m_GroundTypeCmb.ResetContent();
	m_GroundTypeCmb.AddString(_T("A"));
	m_GroundTypeCmb.AddString(_T("B"));
	m_GroundTypeCmb.AddString(_T("C"));
	m_GroundTypeCmb.AddString(_T("D"));
	m_GroundTypeCmb.AddString(_T("E"));
	m_GroundTypeCmb.SetCurSel(1);
}

BOOL CETC_GDS_NTC2018::Dlg2Data()
{
	if(m_pParamData == NULL)
		return FALSE;

	// UpdateData(TRUE); // CETC_DESIGN_SPECT에서 Dlg2Data()호출 전에 이미 호출되었다.
	m_pParamData->nSpecCodeType = D_SPFC_CODE_NTC2018;

	Dlg2Data(m_pParamData->CodeParam.NTC2018);

	return TRUE;
}

void CETC_GDS_NTC2018::Dlg2Data(T_SPFC_NTC2018& NTC2018)
{
	NTC2018.Initialize();	
	
	NTC2018.nSpectrumType  = m_pSpfcUtil->GetSpecType();
	NTC2018.nGroundType    = m_pSpfcUtil->GetGroundType();

	// Input Data
	NTC2018.nParameterType	= m_nParaTypeRdo;
	NTC2018.dSoilFactor		= m_pSpfcUtil->GetS();
	NTC2018.dTb				= m_pSpfcUtil->GetTb();
	NTC2018.dTc				= m_pSpfcUtil->GetTc();
	NTC2018.dTd				= m_pSpfcUtil->GetTd();
	NTC2018.dAg				= m_pSpfcUtil->GetAg();
	NTC2018.dFo				= m_pSpfcUtil->GetFo();
	NTC2018.dTcStar			= m_pSpfcUtil->GetTcStar();
	NTC2018.dXi				= m_pSpfcUtil->GetDamping();
	NTC2018.dQ				= m_pSpfcUtil->GetQ();
	NTC2018.dMaxPeriod		= m_dMaxPeriod;
}

void CETC_GDS_NTC2018::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	if(m_pParamData == NULL)
		return;

	if(m_pParamData->nSpecCodeType!=D_SPFC_CODE_NTC2018)
	{
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_NTC2018);
	}

	m_pSpfcUtil->SetGroundType(m_pParamData->CodeParam.NTC2018.nGroundType);
	m_pSpfcUtil->SetSpecType(m_pParamData->CodeParam.NTC2018.nSpectrumType);

	// Input Data
	m_nParaTypeRdo= m_pParamData->CodeParam.NTC2018.nParameterType;
	m_pSpfcUtil->SetS(m_pParamData->CodeParam.NTC2018.dSoilFactor);
	m_pSpfcUtil->SetTb(m_pParamData->CodeParam.NTC2018.dTb);
	m_pSpfcUtil->SetTc(m_pParamData->CodeParam.NTC2018.dTc);
	m_pSpfcUtil->SetTd(m_pParamData->CodeParam.NTC2018.dTd);
	m_pSpfcUtil->SetAg(m_pParamData->CodeParam.NTC2018.dAg);
	m_pSpfcUtil->SetFo(m_pParamData->CodeParam.NTC2018.dFo);
	m_pSpfcUtil->SetTcStar(m_pParamData->CodeParam.NTC2018.dTcStar);
	m_pSpfcUtil->SetDamping(m_pParamData->CodeParam.NTC2018.dXi);
	m_pSpfcUtil->SetQ(m_pParamData->CodeParam.NTC2018.dQ);
	m_dMaxPeriod  = m_pParamData->CodeParam.NTC2018.dMaxPeriod;

	m_pSpfcUtil->SetParaTypeRdo(m_nParaTypeRdo);
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);


	if(bInit)
	{
		if (CB_ERR == CDlgUtil::CobxSetCurSelItemData(m_SpecTypeCmb, m_pSpfcUtil->GetSpecType()))
			m_SpecTypeCmb.SetCurSel(m_pSpfcUtil->GetSpecType());
		m_GroundTypeCmb.SetCurSel(m_pSpfcUtil->GetGroundType());

		m_wndS.SetEditUnit(m_pSpfcUtil->GetS());
		m_wndTb.SetEditUnit(m_pSpfcUtil->GetTb());
		m_wndTc.SetEditUnit(m_pSpfcUtil->GetTc());
		m_wndTd.SetEditUnit(m_pSpfcUtil->GetTd());

		m_wndAg.SetEditUnit(m_pSpfcUtil->GetAg());
		m_wndFo.SetEditUnit(m_pSpfcUtil->GetFo());
		m_wndTcStar.SetEditUnit(m_pSpfcUtil->GetTcStar());
		m_wndDamping.SetEditUnit(m_pSpfcUtil->GetDamping());
		m_wndQ.SetEditUnit(m_pSpfcUtil->GetQ());

		UpdateData(FALSE);  
	}
}
void CETC_GDS_NTC2018::Data2Dlg()
{
	if(m_pParamData == NULL)
		return;

	if(m_pParamData->nSpecCodeType!=D_SPFC_CODE_NTC2018)
	{
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_NTC2018);
	}

	m_pSpfcUtil->SetGroundType(m_pParamData->CodeParam.NTC2018.nGroundType);
	m_pSpfcUtil->SetSpecType(m_pParamData->CodeParam.NTC2018.nSpectrumType);

	// Input Data
	m_nParaTypeRdo = m_pParamData->CodeParam.NTC2018.nParameterType;
	m_pSpfcUtil->SetS(m_pParamData->CodeParam.NTC2018.dSoilFactor);
	m_pSpfcUtil->SetTb(m_pParamData->CodeParam.NTC2018.dTb);
	m_pSpfcUtil->SetTc(m_pParamData->CodeParam.NTC2018.dTc);
	m_pSpfcUtil->SetTd(m_pParamData->CodeParam.NTC2018.dTd);
	m_pSpfcUtil->SetAg(m_pParamData->CodeParam.NTC2018.dAg);
	m_pSpfcUtil->SetFo(m_pParamData->CodeParam.NTC2018.dFo);
	m_pSpfcUtil->SetTcStar(m_pParamData->CodeParam.NTC2018.dTcStar);
	m_pSpfcUtil->SetDamping(m_pParamData->CodeParam.NTC2018.dXi);
	m_pSpfcUtil->SetQ(m_pParamData->CodeParam.NTC2018.dQ);
	m_dMaxPeriod = m_pParamData->CodeParam.NTC2018.dMaxPeriod;

	m_pSpfcUtil->SetParaTypeRdo(m_nParaTypeRdo);
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);

	if (CB_ERR == CDlgUtil::CobxSetCurSelItemData(m_SpecTypeCmb, m_pSpfcUtil->GetSpecType()))
		m_SpecTypeCmb.SetCurSel(m_pSpfcUtil->GetSpecType());
	m_GroundTypeCmb.SetCurSel(m_pSpfcUtil->GetGroundType());

	m_wndS.SetEditUnit(m_pSpfcUtil->GetS());
	m_wndTb.SetEditUnit(m_pSpfcUtil->GetTb());
	m_wndTc.SetEditUnit(m_pSpfcUtil->GetTc());
	m_wndTd.SetEditUnit(m_pSpfcUtil->GetTd());

	m_wndAg.SetEditUnit(m_pSpfcUtil->GetAg());
	m_wndFo.SetEditUnit(m_pSpfcUtil->GetFo());
	m_wndTcStar.SetEditUnit(m_pSpfcUtil->GetTcStar());
	m_wndDamping.SetEditUnit(m_pSpfcUtil->GetDamping());
	m_wndQ.SetEditUnit(m_pSpfcUtil->GetQ());

	UpdateData(FALSE);  
}

void CETC_GDS_NTC2018::ShowDlgData() 
{
	if(m_pParamData == NULL)
		return;

	if(m_pParamData->nSpecCodeType == D_SPFC_CODE_NTC2018)
		m_CodeD.NTC2018 = m_pParamData->CodeParam.NTC2018;
	else
		m_CodeD.NTC2018.Initialize();

	m_pSpfcUtil->SetGroundType(m_CodeD.NTC2018.nGroundType);
	m_pSpfcUtil->SetSpecType(m_CodeD.NTC2018.nSpectrumType);

	// Input Data
	m_nParaTypeRdo = m_CodeD.NTC2018.nParameterType;
	m_pSpfcUtil->SetS(m_CodeD.NTC2018.dSoilFactor);
	m_pSpfcUtil->SetTb(m_CodeD.NTC2018.dTb);
	m_pSpfcUtil->SetTc(m_CodeD.NTC2018.dTc);
	m_pSpfcUtil->SetTd(m_CodeD.NTC2018.dTd);
	m_pSpfcUtil->SetAg(m_CodeD.NTC2018.dAg);
	m_pSpfcUtil->SetFo(m_CodeD.NTC2018.dFo);
	m_pSpfcUtil->SetTcStar(m_CodeD.NTC2018.dTcStar);
	m_pSpfcUtil->SetDamping(m_CodeD.NTC2018.dXi);
	m_pSpfcUtil->SetQ(m_CodeD.NTC2018.dQ);
	m_dMaxPeriod = m_CodeD.NTC2018.dMaxPeriod;

	m_pSpfcUtil->SetParaTypeRdo(m_nParaTypeRdo);
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);
	
	CDlgUtil::CobxSetCurSelItemData(m_SpecTypeCmb, m_pSpfcUtil->GetSpecType());
	m_GroundTypeCmb.SetCurSel(m_pSpfcUtil->GetGroundType());

	m_wndS.SetEditUnit(m_pSpfcUtil->GetS());
	m_wndTb.SetEditUnit(m_pSpfcUtil->GetTb());
	m_wndTc.SetEditUnit(m_pSpfcUtil->GetTc());
	m_wndTd.SetEditUnit(m_pSpfcUtil->GetTd());

	m_wndAg.SetEditUnit(m_pSpfcUtil->GetAg());
	m_wndFo.SetEditUnit(m_pSpfcUtil->GetFo());
	m_wndTcStar.SetEditUnit(m_pSpfcUtil->GetTcStar());
	m_wndDamping.SetEditUnit(m_pSpfcUtil->GetDamping());
	m_wndQ.SetEditUnit(m_pSpfcUtil->GetQ());
	
	UpdateData(FALSE);
	
	CtrlManager();

	UpdateData(FALSE);   
}

void CETC_GDS_NTC2018::Initialize() 
{
	if(m_pParamData == NULL)
		return;

	m_pParamData->CodeParam.Initialize(D_SPFC_CODE_NTC2018);
}

void CETC_GDS_NTC2018::SetDefaultValue()
{
	m_nParaTypeRdo = 0;
	
	//m_dS = 1.2;
	//m_dTb= 0.15;
	//m_dTc= 0.5;
	//m_dTd= 2.0;
	//m_dAg= 0.08;
	//m_dFo= 2.5;
	//m_dTcStar= 0.3;
	//m_dDamping = 5.0;
	//m_dQ = 1.5;
}

void CETC_GDS_NTC2018::CtrlManager()
{
	UpdateData(TRUE);
	
	BOOL bElastic = FALSE;  
	BOOL bVertical = FALSE;
	BOOL bUserDefined = FALSE;
	BOOL bAutomatic = TRUE;

	DWORD nSpecType = 0;
	CDlgUtil::CobxGetItemDataByCurSel(m_SpecTypeCmb, nSpecType);

	if(nSpecType == EN_HORI_ELASTIC || nSpecType == EN_VERT_ELASTIC) bElastic = TRUE;  
	if(nSpecType == EN_VERT_ELASTIC || nSpecType == EN_VERT_DESIGN) bVertical = TRUE;
	
	if(m_nParaTypeRdo == 4) bUserDefined = TRUE;
	
	CDlgUtil::CtrlShowHide(this, m_aCtrlElastic, bElastic);
	CDlgUtil::CtrlShowHide(this, m_aCtrlDesign, !bElastic);

	if(bUserDefined) bAutomatic = FALSE;
	
	// Soil Factor(S)는 Horizontal Spectrum인 경우 : Spectrum Parameter Type이 User Defined인 경우 사용자 입력만 가능
	// Tb, Tc, Td는 Spectrum Parameter Type이 User Defined인 경우 사용자 입력만 가능
	m_wndS.SetReadOnly((!bUserDefined) || bVertical);
	m_wndTb.SetReadOnly(!bUserDefined);
	m_wndTc.SetReadOnly(!bUserDefined);
	m_wndTd.SetReadOnly(!bUserDefined);

	m_pSpfcUtil->SetSpecType(nSpecType);
	m_pSpfcUtil->SetParaTypeRdo(m_nParaTypeRdo);
	m_pSpfcUtil->SetGroundType(m_GroundTypeCmb.GetCurSel());

	m_pSpfcUtil->CtrlManager();

	if(bAutomatic)
	{
		m_wndS.SetEditUnit(m_pSpfcUtil->GetS());
		m_wndTb.SetEditUnit(m_pSpfcUtil->GetTb());
		m_wndTc.SetEditUnit(m_pSpfcUtil->GetTc());
		m_wndTd.SetEditUnit(m_pSpfcUtil->GetTd());  
	}
	else
	{
		if (bVertical)
		{
			m_wndS.SetEditUnit(m_pSpfcUtil->GetS());
		}
	}
		
	UpdateData(FALSE);
}

void CETC_GDS_NTC2018::AlignControl() 
{
	CWnd* pWnd;
	CRect FromRect, ToRect;
	pWnd = GetDlgItem(IDC_ETC_GDS_NTC2018_DAMPING_EDIT); ASSERT(pWnd);
	pWnd->GetWindowRect(&ToRect);
	pWnd = GetDlgItem(IDC_ETC_GDS_NTC2018_BEHAVIOR_EDIT); ASSERT(pWnd);
	pWnd->GetWindowRect(&FromRect);
	int nDistY = ToRect.top - FromRect.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlDesign, nDistY);  
}

void CETC_GDS_NTC2018::UpdateDampingWnd(CWnd* pDampingWnd)
{
	CString csDamping;
	csDamping.Format(_T("%g"), m_pSpfcUtil->GetDamping()/100.0);
	pDampingWnd->SetWindowText(csDamping);
}
	
void CETC_GDS_NTC2018::End()
{
	UpdateData(TRUE);
		
	DWORD nSpecType = 0;
	if(CDlgUtil::CobxGetItemDataByCurSel(m_SpecTypeCmb, nSpecType)) 
		m_pSpfcUtil->SetSpecType(nSpecType);
	else { ASSERT(0); m_pSpfcUtil->SetSpecType(0); }

	m_pSpfcUtil->SetGroundType(m_GroundTypeCmb.GetCurSel());

	m_pSpfcUtil->SetS(m_wndS.GetEditValue());
	m_pSpfcUtil->SetTb(m_wndTb.GetEditValue());
	m_pSpfcUtil->SetTc(m_wndTc.GetEditValue());
	m_pSpfcUtil->SetTd(m_wndTd.GetEditValue());
	m_pSpfcUtil->SetAg(m_wndAg.GetEditValue());
	m_pSpfcUtil->SetFo(m_wndFo.GetEditValue());
	m_pSpfcUtil->SetTcStar(m_wndTcStar.GetEditValue());
	m_pSpfcUtil->SetDamping(m_wndDamping.GetEditValue());
	m_pSpfcUtil->SetQ(m_wndQ.GetEditValue());

	m_pSpfcUtil->SetParaTypeRdo(m_nParaTypeRdo);
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);

	CMyChildDialog::End();
}

void CETC_GDS_NTC2018::EndNotClose()
{
	UpdateData(TRUE);
	
	DWORD nSpecType = 0;
	if(CDlgUtil::CobxGetItemDataByCurSel(m_SpecTypeCmb, nSpecType))
		m_pSpfcUtil->SetSpecType(nSpecType);
	else { ASSERT(0); m_pSpfcUtil->SetSpecType(0); }

	m_pSpfcUtil->SetGroundType(m_GroundTypeCmb.GetCurSel());

	m_pSpfcUtil->SetS(m_wndS.GetEditValue());
	m_pSpfcUtil->SetTb(m_wndTb.GetEditValue());
	m_pSpfcUtil->SetTc(m_wndTc.GetEditValue());
	m_pSpfcUtil->SetTd(m_wndTd.GetEditValue());
	m_pSpfcUtil->SetAg(m_wndAg.GetEditValue());
	m_pSpfcUtil->SetFo(m_wndFo.GetEditValue());
	m_pSpfcUtil->SetTcStar(m_wndTcStar.GetEditValue());
	m_pSpfcUtil->SetDamping(m_wndDamping.GetEditValue());
	m_pSpfcUtil->SetQ(m_wndQ.GetEditValue());

	m_pSpfcUtil->SetParaTypeRdo(m_nParaTypeRdo);
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);
}

BOOL CETC_GDS_NTC2018::ISVALID()
{   
	BOOL bElastic = FALSE;    
	DWORD nSpecType = 0;
	CDlgUtil::CobxGetItemDataByCurSel(m_SpecTypeCmb, nSpecType);

	if(nSpecType == EN_HORI_ELASTIC || nSpecType == EN_VERT_ELASTIC ) bElastic = TRUE;  
	
	m_pSpfcUtil->SetS(m_wndS.GetEditValue());
	m_pSpfcUtil->SetTb(m_wndTb.GetEditValue());
	m_pSpfcUtil->SetTc(m_wndTc.GetEditValue());
	m_pSpfcUtil->SetTd(m_wndTd.GetEditValue());
	m_pSpfcUtil->SetAg(m_wndAg.GetEditValue());
	m_pSpfcUtil->SetFo(m_wndFo.GetEditValue());
	m_pSpfcUtil->SetTcStar(m_wndTcStar.GetEditValue());
	m_pSpfcUtil->SetDamping(m_wndDamping.GetEditValue());
	m_pSpfcUtil->SetQ(m_wndQ.GetEditValue());

	return m_pSpfcUtil->CheckValid();
}

void CETC_GDS_NTC2018::OnSelchangeEtcGdsNTC2018SpecTypeCmb() 
{
	CtrlManager();	
}

void CETC_GDS_NTC2018::OnEtcGdsNTC2018ParatypeRdo() 
{
	CtrlManager();	
}

void CETC_GDS_NTC2018::OnSelchangeEtcGdsNTC2018GroundCmb() 
{
	CtrlManager();	
}

void CETC_GDS_NTC2018::OnChangeCmdAgEdit()
{
	m_pSpfcUtil->SetAg(m_wndAg.GetEditValue());
	if(m_nParaTypeRdo == 4) return;
	CtrlManager();
}

void CETC_GDS_NTC2018::OnChangeCmdAmpEdit()
{
	m_pSpfcUtil->SetFo(m_wndFo.GetEditValue());
	if(m_nParaTypeRdo == 4) return;
	CtrlManager();
}

void CETC_GDS_NTC2018::OnChangeCmdTcStarEdit()
{
	m_pSpfcUtil->SetTcStar(m_wndTcStar.GetEditValue());
	if(m_nParaTypeRdo == 4) return;
	if(fabs(m_pSpfcUtil->GetTcStar()) < 1.0e-10 ) return;
	CtrlManager();
}



// ETC_GDS_IBC2012.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_IBC2012.h"

#include "SpfcIBC2012Util.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\Seismic_NSCP2024.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_IBC2012 dialog


CETC_GDS_IBC2012::CETC_GDS_IBC2012(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/, int codeType)
	: CMyChildDialog(CETC_GDS_IBC2012::IDD, pParent, std::make_shared<CSpfcIBC2012Util>(codeType))
{
	//{{AFX_DATA_INIT(CETC_GDS_IBC2012)
	//}}AFX_DATA_INIT  
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcIBC2012Util>(m_pSpfcBaseUtil);

	SetInitValueforPOSpectrum();
}

// Pushover Spectrum은 초기값 설정전에 호출되기 때문에 생성자에서 아래 함수를 반드시 먼저 불러야 한다.
void CETC_GDS_IBC2012::SetInitValueforPOSpectrum()
{
	//m_dSs  = 0.75;
	//m_dS1  = 0.3;
	//m_dFa  = 1.2;
	//m_dFv  = 1.8;
	//m_dSds = 0.6;
	//m_dSd1 = 0.36;
	//m_dTs = m_dSd1 / m_dSds;
	//m_dR = 4.0;
	//m_dI = 1.0;
	//m_dMaxPeriod = PRD_ED;
	//m_dTL = 4.0;
}

void CETC_GDS_IBC2012::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_IBC2012)
	DDX_Control(pDX, IDC_CMD_SS_CMB, m_cmbSs);
	DDX_Control(pDX, IDC_CMD_S1_CMB, m_cmbS1);
	DDX_Control(pDX, IDC_CMD_SITE_CMB, m_cmbSiteClass);
	DDX_Control(pDX, IDC_CMD_FA_EDIT, m_editFa);
	DDX_Control(pDX, IDC_CMD_FV_EDIT, m_editFv);
	DDX_Control(pDX, IDC_CMD_SDS_EDIT, m_editSds);
	DDX_Control(pDX, IDC_CMD_SD1_EDIT, m_editSd1);
	DDX_Control(pDX, IDC_CMD_R_CMB, m_cmbR);	
	DDX_Control(pDX, IDC_CMD_IMPORTANCE_CMB, m_cmbImportance);	
 	DDX_Control(pDX, IDC_CMD_PERIOD_EDIT, m_editMaxPeriod);	
	DDX_Control(pDX, IDC_CMD_TL_EDIT, m_editTL);	  
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_IBC2012, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_IBC2012)
	ON_CBN_SELCHANGE(IDC_CMD_SS_CMB, OnSelchangeCmdSsCmb)
	ON_CBN_SELCHANGE(IDC_CMD_S1_CMB, OnSelchangeCmdS1Cmb)
	ON_CBN_SELCHANGE(IDC_CMD_SITE_CMB, OnSelchangeCmdSiteCmb)
	ON_CBN_EDITCHANGE(IDC_CMD_SS_CMB, OnChangeCmdSs)
	ON_CBN_EDITCHANGE(IDC_CMD_S1_CMB, OnChangeCmdS1)
	ON_EN_CHANGE(IDC_CMD_FA_EDIT,      OnChangeCmdFaEdit)
	ON_EN_CHANGE(IDC_CMD_FV_EDIT,      OnChangeCmdFvEdit)  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_IBC2012 message handlers

BOOL CETC_GDS_IBC2012::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	InitSsCombo();
	InitS1Combo();
	InitSiteCombo();
	InitIFactorCombo();  
	InitRCombo();

	SetFaFvSdsSd1();

	m_editMaxPeriod.SetEditUnit(PRD_ED);
	m_editTL.SetEditUnit(m_pSpfcUtil->GetTL());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_IBC2012::SetFaFvSdsSd1()
{
	CString str; 
	m_cmbSs.GetWindowText(str);
	double dSs = _tstof(str);
	m_cmbS1.GetWindowText(str);
	double dS1 = _tstof(str);	
	int nSiteClass = m_cmbSiteClass.GetCurSel();

	double dFa = 0.0;
	double dFv = 0.0;
	double dSds = 0.0;
	double dSd1 = 0.0;	
	switch (m_pParamData->nSpecFuncType)
	{
	case D_SPFC_CODE_IBC2009:
	case D_SPFC_CODE_IBC2012:
		{
			CDBDoc* pDoc = CDBDoc::GetDocPoint();
			if (!pDoc->m_pAttrCtrl2->CalcIBC2012Fa(nSiteClass, dSs, dFa)) return;
			if (!pDoc->m_pAttrCtrl2->CalcIBC2012Fv(nSiteClass, dS1, dFv)) return;
			pDoc->m_pAttrCtrl2->CalcIBC2012Sds(dSs, dFa, dSds);
			pDoc->m_pAttrCtrl2->CalcIBC2012Sd1(dS1, dFv, dSd1);
		}
		break;
	case D_SPFC_CODE_NSCP_2024:
		{
			CSeismic_NSCP2024 CodeCalc;
			dFa = CodeCalc.CalcFa(nSiteClass, dSs);
			dFv = CodeCalc.CalcFv(nSiteClass, dS1);
			dSds = CodeCalc.CalcSds(dSs, dFa);
			dSd1 = CodeCalc.CalcSd1(dS1, dFv);
		}
		break;
	default:
		ASSERT(0);
		break;
	}
	
	m_editFa.SetEditUnit(dFa, 5);
	m_editFv.SetEditUnit(dFv, 5);
	m_editSds.SetEditUnit(dSds, 5);
	m_editSd1.SetEditUnit(dSd1, 5);
}

void CETC_GDS_IBC2012::OnSelchangeCmdSsCmb() 
{
	// Edit Box 값을 변경후에 Combo 로 값을 변경하면 바로 Update 안되는 문제...		
	int nTmp = m_cmbSs.GetCurSel();
	const double dSs = [nTmp]()
	{
		switch (nTmp)
		{
		case 0: return 0.25;
		case 1: return 0.5;
		case 2: return 0.75;
		case 3: return 1.0;
		case 4: return 1.25;
		default: return 0.0;			
		}
	}();
	
	CString csValue; 
	csValue.Format(_T("%g"), dSs);
	m_cmbSs.SetWindowText(csValue);

	SetFaFvSdsSd1();
}

void CETC_GDS_IBC2012::OnSelchangeCmdS1Cmb() 
{
	// Edit Box 값을 변경후에 Combo 로 값을 변경하면 바로 Update 안되는 문제...	
	int nTmp = m_cmbS1.GetCurSel();
	const double dS1 = [nTmp]()
	{
		switch (nTmp)
		{
		case 0: return 0.1;
		case 1: return 0.2;
		case 2: return 0.3;
		case 3: return 0.4;
		case 4: return 0.5;
		default: return 0.0;
		}
	}();	
	
	CString csValue; 
	csValue.Format(_T("%g"), dS1);
	m_cmbS1.SetWindowText(csValue);
	
	SetFaFvSdsSd1();
}

void CETC_GDS_IBC2012::OnSelchangeCmdSiteCmb() 
{
	SetFaFvSdsSd1();
}

void CETC_GDS_IBC2012::OnChangeCmdSs() 
{
	SetFaFvSdsSd1();
}

void CETC_GDS_IBC2012::OnChangeCmdS1() 
{
	SetFaFvSdsSd1();
}

void CETC_GDS_IBC2012::OnChangeCmdFaEdit() 
{
	CString str;
	m_cmbSs.GetWindowText(str);
	double dSs  = _tstof(str);
	double dFa  = m_editFa.GetEditValue();
	
	double dSds = 0.0;
	switch (m_pParamData->nSpecCodeType)
	{
	case D_SPFC_CODE_IBC2009:
	case D_SPFC_CODE_IBC2012:
		{
			CDBDoc* pDoc = CDBDoc::GetDocPoint();
			pDoc->m_pAttrCtrl2->CalcIBC2012Sds(dSs, dFa, dSds);
		}
		break;
	case D_SPFC_CODE_NSCP_2024:
		{
			CSeismic_NSCP2024 CodeCalc;
			dSs = CodeCalc.CalcSds(dSs, dFa);
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	m_editSds.SetEditUnit(dSds, 5);
}

void CETC_GDS_IBC2012::OnChangeCmdFvEdit() 
{
	CString str;
	m_cmbS1.GetWindowText(str);
	double dS1  = _tstof(str);
	double dFv  = m_editFv.GetEditValue();
	
	double dSd1 = 0.0;  
	switch (m_pParamData->nSpecCodeType)
	{
	case D_SPFC_CODE_IBC2009:
	case D_SPFC_CODE_IBC2012:
		{
			CDBDoc* pDoc = CDBDoc::GetDocPoint();
			pDoc->m_pAttrCtrl2->CalcIBC2012Sd1(dS1, dFv, dSd1);
		}
		break;
	case D_SPFC_CODE_NSCP_2024:
		{
			CSeismic_NSCP2024 CodeCalc;
			dS1 = CodeCalc.CalcSd1(dS1, dFv);
		}
		break;
	default:
		ASSERT(0);
		break;	
	}	
	
	m_editSd1.SetEditUnit(dSd1, 5);
}

void CETC_GDS_IBC2012::InitSsCombo()
{
	CString aItem[5] = {_T("0.25"), _T("0.5"), _T("0.75"), _T("1.0"), _T("1.25")};

	m_cmbSs.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSs.AddString(aItem[i]);
	m_cmbSs.SetCurSel(2);
}

void CETC_GDS_IBC2012::InitS1Combo()
{
	CString aItem[5] = {_T("0.1"), _T("0.2"), _T("0.3"), _T("0.4"), _T("0.5")};

	m_cmbS1.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbS1.AddString(aItem[i]);
	m_cmbS1.SetCurSel(2);
}

void CETC_GDS_IBC2012::InitSiteCombo()
{
	CString aItem[5] = {_T("A"), _T("B"), _T("C"), _T("D"), _T("E")};

	m_cmbSiteClass.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbSiteClass.AddString(aItem[i]);
	m_cmbSiteClass.SetCurSel(3);
}

void CETC_GDS_IBC2012::InitIFactorCombo()
{
	CString aItem[3] = {_T("1.0"), _T("1.25"), _T("1.5")};

	m_cmbImportance.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbImportance.AddString(aItem[i]);
	m_cmbImportance.SetCurSel(0);
}

void CETC_GDS_IBC2012::InitRCombo()
{
	CString aItem[15] = {_T("1.5"), _T("2"), _T("2.5"), _T("3"), _T("3.25"), _T("3.5"), _T("4"), _T("4.5"), _T("5"), _T("5.5"), _T("6"), _T("6.5"), _T("7"), _T("7.5"), _T("8")};

	m_cmbR.ResetContent();	

	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbR.AddString(aItem[i]);		  
	m_cmbR.SetCurSel(6);	
}

// CETC_DESIGN_SPECT에서 ISVALID()를 호출한 후 End()를 호출한다.
BOOL CETC_GDS_IBC2012::ISVALID()
{ 	
	GetFinalValue();
	return m_pSpfcUtil->CheckValid();
}

void CETC_GDS_IBC2012::End()
{
	CMyChildDialog::End();
}

void CETC_GDS_IBC2012::GetFinalValue()
{
	CString str;
	m_cmbSs.GetWindowText(str);
	m_pSpfcUtil->SetSs(_tstof(str));

	m_cmbS1.GetWindowText(str);
	m_pSpfcUtil->SetS1(_tstof(str));

	m_pSpfcUtil->SetSite(m_cmbSiteClass.GetCurSel());

	m_pSpfcUtil->SetFa(m_editFa.GetEditValue());
	m_pSpfcUtil->SetFv(m_editFv.GetEditValue());
	m_pSpfcUtil->SetSds(m_editSds.GetEditValue());
	m_pSpfcUtil->SetSd1(m_editSd1.GetEditValue());
	m_pSpfcUtil->SetTs(m_pSpfcUtil->GetSd1() / m_pSpfcUtil->GetSds());
	m_pSpfcUtil->SetT0(0.2 * m_pSpfcUtil->GetTs());

	CString strI, strR;
	m_cmbImportance.GetWindowText(strI);
	m_pSpfcUtil->SetI(_tstof(strI));

	m_cmbR.GetWindowText(strR);
	m_pSpfcUtil->SetR(_tstof(strR));

	m_pSpfcUtil->SetMaxPeriod(m_editMaxPeriod.GetEditValue());
	m_pSpfcUtil->SetTL(m_editTL.GetEditValue());
}

T_SPFC_IBC2012* CETC_GDS_IBC2012::GetSpfcCodeParam()
{
	T_SPFC_IBC2012* pIBC2012 = NULL;
	switch (m_pParamData->nSpecCodeType)
	{
	case D_SPFC_CODE_IBC2009: pIBC2012 = &m_pParamData->CodeParam.IBC2009; break;
	case D_SPFC_CODE_IBC2012: pIBC2012 = &m_pParamData->CodeParam.IBC2012; break;
	case D_SPFC_CODE_NSCP_2024: pIBC2012 = &m_pParamData->CodeParam.NSCP2024; break;
	default: ASSERT(0); break;
	}
	return pIBC2012;
}

BOOL CETC_GDS_IBC2012::Dlg2Data()
{
	T_SPFC_IBC2012* pIBC2012 = GetSpfcCodeParam();
	if (pIBC2012 == NULL) return FALSE;
	
	pIBC2012->nSc   = m_pSpfcUtil->GetSite();
	pIBC2012->dSs   = m_pSpfcUtil->GetSs();
	pIBC2012->dS1   = m_pSpfcUtil->GetS1();
	pIBC2012->dFa   = m_pSpfcUtil->GetFa();
	pIBC2012->dFv   = m_pSpfcUtil->GetFv();
	pIBC2012->dSds  = m_pSpfcUtil->GetSds();
	pIBC2012->dSd1  = m_pSpfcUtil->GetSd1();
	pIBC2012->dIe   = m_pSpfcUtil->GetI();
	pIBC2012->dCoef = m_pSpfcUtil->GetR();
	pIBC2012->dTL   = m_pSpfcUtil->GetTL();
	pIBC2012->dMaxPeriod = m_pSpfcUtil->GetMaxPeriod();

	m_pParamData->dResModCoef = m_pSpfcUtil->GetR();
	return TRUE;
}

void CETC_GDS_IBC2012::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	T_SPFC_IBC2012* pIBC2012 = GetSpfcCodeParam();
	if (pIBC2012 == NULL) return;

	m_pSpfcUtil->SetSite(pIBC2012->nSc);
	m_pSpfcUtil->SetSs(pIBC2012->dSs);
	m_pSpfcUtil->SetS1(pIBC2012->dS1);
	m_pSpfcUtil->SetFa(pIBC2012->dFa);
	m_pSpfcUtil->SetFv(pIBC2012->dFv);
	m_pSpfcUtil->SetSds(pIBC2012->dSds);
	m_pSpfcUtil->SetSd1(pIBC2012->dSd1);
	m_pSpfcUtil->SetI(pIBC2012->dIe);
	m_pSpfcUtil->SetR(pIBC2012->dCoef);
	m_pSpfcUtil->SetTL(pIBC2012->dTL);
	m_pSpfcUtil->SetMaxPeriod(pIBC2012->dMaxPeriod);
	m_pSpfcUtil->SetTs(m_pSpfcUtil->GetSd1() / m_pSpfcUtil->GetSds());
	m_pSpfcUtil->SetT0(0.2 * m_pSpfcUtil->GetTs());

	if(bInit)
	{
		// 데이터를 넣는다.
		CString strSs, strS1;
		strSs.Format(_T("%g"), m_pSpfcUtil->GetSs());
		m_cmbSs.SetWindowText(strSs);

		strS1.Format(_T("%g"), m_pSpfcUtil->GetS1());
		m_cmbS1.SetWindowText(strS1);

		m_cmbSiteClass.SetCurSel(m_pSpfcUtil->GetSite());

		m_editFa.SetEditUnit(m_pSpfcUtil->GetFa(), 5);
		m_editFv.SetEditUnit(m_pSpfcUtil->GetFv(), 5);
		m_editSds.SetEditUnit(m_pSpfcUtil->GetSds(), 5);
		m_editSd1.SetEditUnit(m_pSpfcUtil->GetSd1(), 5);

		CString strI, strR;
		strI.Format(_T("%g"), m_pSpfcUtil->GetI());
		strR.Format(_T("%g"), m_pSpfcUtil->GetR());
		m_cmbImportance.SetWindowText(strI);
		m_cmbR.SetWindowText(strR);
		m_editTL.SetEditUnit(m_pSpfcUtil->GetTL());
	}
}
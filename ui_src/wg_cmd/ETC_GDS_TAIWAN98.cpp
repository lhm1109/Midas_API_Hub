// CETC_GDS_TAIWAN98.cpp : implementation file
//
// Taiwan2006
// MNET:1986-GSJI-20060412

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_TAIWAN98.h"


#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_base\SpinBtnExCtrl.h"   //CFormulaEdit 를 사용하기 위함.
#include "SpfcTAIWAN98Util.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CETC_GDS_TAIWAN98::CETC_GDS_TAIWAN98(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_TAIWAN98::IDD, pParent, std::make_shared<CSpfcTAIWAN98Util>())
{
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcTAIWAN98Util>(m_pSpfcBaseUtil);
	//{{AFX_DATA_INIT(CETC_GDS_TAIWAN98)
	//}}AFX_DATA_INIT
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_ACC_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_SDS_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_SD1_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_SDS_CMB);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_SD1_CMB);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_SMS_CMB);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_SM1_CMB);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_MSF_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_SOILTYPE_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_SOILTYPE_CMB);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_SMFD_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_SMFM_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_FMA_EDT);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_FMV_EDT);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_FDA_EDT);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_FDV_EDT);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_FA_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_FV_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_G1_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_G2_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_0D_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_0M_STC);
	
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_RS_NSF_STC);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_RS_NA_STC);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_RS_NV_STC);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_RS_NDA_EDT);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_RS_NDV_EDT);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_RS_NSFD_STC2);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_RS_NSFM_STC2);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_RS_NMV_EDT);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_RS_NMA_EDT);
	
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_SUBZONE_STC);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_SUBZONE_CMB);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_2D_STC);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_2M_STC);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_ACC_STC2);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_SS_STC);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_SDS_EDT2);  
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_SMS_EDT2); 
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_G2_STC2);   
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_T0_STC);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_2D_STC2);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_2M_STC2);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_TD0_EDT);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_TM0_EDT);  
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_TP_STC);  
	
	// MNET:2410-GSJI-20060829
	m_aCtrl2.Add(IDC_CMD_TAIWAN02_RS_SMS_CMB);
	m_aCtrl2.Add(IDC_CMD_TAIWAN02_RS_SM1_CMB);
	m_aCtrl2.Add(IDC_CMD_TAIWAN02_RS_NMA_EDT);
	m_aCtrl2.Add(IDC_CMD_TAIWAN02_RS_NMV_EDT);
	m_aCtrl2.Add(IDC_CMD_TAIWAN02_RS_FMA_EDT);
	m_aCtrl2.Add(IDC_CMD_TAIWAN02_RS_FMV_EDT);
	m_aCtrl2.Add(IDC_CMD_TAIWAN02_RS_SMS_EDT2); 
	m_aCtrl2.Add(IDC_CMD_TAIWAN02_RS_TM0_EDT);

	m_aCtrl1.Add(IDC_CMD_TAIWAN02_RS_SDS_CMB);
	m_aCtrl1.Add(IDC_CMD_TAIWAN02_RS_SD1_CMB);
	m_aCtrl1.Add(IDC_CMD_TAIWAN02_RS_NDA_EDT);
	m_aCtrl1.Add(IDC_CMD_TAIWAN02_RS_NDV_EDT);
	m_aCtrl1.Add(IDC_CMD_TAIWAN02_RS_FDA_EDT);
	m_aCtrl1.Add(IDC_CMD_TAIWAN02_RS_FDV_EDT);
	m_aCtrl1.Add(IDC_CMD_TAIWAN02_RS_SDS_EDT2); 
	m_aCtrl1.Add(IDC_CMD_TAIWAN02_RS_TD0_EDT);

	m_pSpfcUtil->m_nSpecCategory = 0;

	m_pParamData = pParamData;

	SetInitValueforPOSpectrum(); 
}

// Pushover Spectrum은 초기값 설정전에 호출되기 때문에 생성자에서 아래 함수를 반드시 먼저 불러야 한다.
void CETC_GDS_TAIWAN98::SetInitValueforPOSpectrum()
{ 
	//m_pSpfcUtil->m_nSpecCategory = 0;       // MNET:2410-GSJI-20060829

	//m_pSpfcUtil->m_nSpecType = 0;     //Horizontal
	//m_pSpfcUtil->m_nSeisZone = 0;     //General Zone  

	//m_pSpfcUtil->m_dSds = 0.5;         m_pSpfcUtil->m_dSd1 = 0.3;         m_pSpfcUtil->m_dSms = 0.7;         m_pSpfcUtil->m_dSm1 = 0.4;
	//m_pSpfcUtil->m_nSoilType = 0;     //Type1   
	//m_pSpfcUtil->m_dFda = 1.0;         m_pSpfcUtil->m_dFdv = 1.0;         m_pSpfcUtil->m_dFma = 1.0;         m_pSpfcUtil->m_dFmv = 1.0;
	//m_pSpfcUtil->m_dNda = 1.0;         m_pSpfcUtil->m_dNdv = 1.0;         m_pSpfcUtil->m_dNma = 1.0;         m_pSpfcUtil->m_dNmv = 1.0;

	//m_pSpfcUtil->m_dSds_t = 0.6;       m_pSpfcUtil->m_dSms_t = 0.8;       m_pSpfcUtil->m_dTd0_t = 1.6;      m_pSpfcUtil->m_dTm0_t = 1.6;
	//m_pSpfcUtil->m_dR = 1.6;
	//m_pSpfcUtil->m_dAy = 1.2;
	//m_pSpfcUtil->m_dI = 1.0;
	//m_pSpfcUtil->m_dDamping = 5.0;
	//m_pSpfcUtil->m_dMaxPeriod = PRD_ED;
}

void CETC_GDS_TAIWAN98::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_TAIWAN98)
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_CATEGORY_CMB, m_cmbSpecCategory);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_SMS_CMB,      m_cmbSms);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_SM1_CMB,      m_cmbSm1);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_NMV_EDT,      m_edtNmv);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_NMA_EDT,      m_edtNma);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_NDV_EDT,      m_edtNdv);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_NDA_EDT,      m_edtNda);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_FMV_EDT,      m_edtFmv);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_FMA_EDT,      m_edtFma);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_FDV_EDT,      m_edtFdv);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_FDA_EDT,      m_edtFda);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_TYPE_CMB,     m_cmbSpecType);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_ZONE_CMB,     m_cmbSeisZone);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_SDS_CMB,      m_cmbSds);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_SD1_CMB,      m_cmbSd1);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_SOILTYPE_CMB, m_cmbSoilType);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_SUBZONE_CMB,  m_cmbSubZone);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_SDS_EDT2,     m_edtSds_t);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_SMS_EDT2,     m_edtSms_t);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_TM0_EDT,      m_edtTm0);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_TD0_EDT,      m_edtTd0);
	DDX_Control(pDX, IDC_CMD_IMPORTANCE_CMB,           m_cmbI);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_ALPHA_CMB,    m_cmbAlpha);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_R_CMB,        m_cmbR);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_DAMP_EDT,     m_edtDamping);
	DDX_Control(pDX, IDC_CMD_PERIOD_EDIT,              m_edtMaxPeriod);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CETC_GDS_TAIWAN98, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_TAIWAN98)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_RS_TYPE_CMB,     OnSelchangeCmdTaiwan02SpecTypeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_RS_ZONE_CMB,     OnSelchangeCmdTaiwan02SeisZoneCmb)	
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_RS_SOILTYPE_CMB, OnSelchangeCmdTaiwan02SoilTypeCmb)	
	ON_CBN_EDITCHANGE(IDC_CMD_TAIWAN02_RS_SDS_CMB,     OnEditchangeCmdTaiwan02SpecAccCmb)
	ON_CBN_EDITCHANGE(IDC_CMD_TAIWAN02_RS_SD1_CMB,     OnEditchangeCmdTaiwan02SpecAccCmb)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_RS_SDS_CMB,      OnSelchangeCmdTaiwan02SpecAccCmb)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_RS_SD1_CMB,      OnSelchangeCmdTaiwan02SpecAccCmb)
	ON_CBN_EDITCHANGE(IDC_CMD_TAIWAN02_RS_SMS_CMB,     OnEditchangeCmdTaiwan02SpecAccCmb)
	ON_CBN_EDITCHANGE(IDC_CMD_TAIWAN02_RS_SM1_CMB,     OnEditchangeCmdTaiwan02SpecAccCmb)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_RS_SMS_CMB,      OnSelchangeCmdTaiwan02SpecAccCmb)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_RS_SM1_CMB,      OnSelchangeCmdTaiwan02SpecAccCmb)    
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_NDA_EDT,          OnEditchangeCmdTaiwan02NSFactorEdt)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_NDV_EDT,          OnEditchangeCmdTaiwan02NSFactorEdt)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_NMA_EDT,          OnEditchangeCmdTaiwan02NSFactorEdt)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_NMV_EDT,          OnEditchangeCmdTaiwan02NSFactorEdt)	
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_FMV_EDT,          OnEditchangeCmdTaiwan02SMFactorEdt)	
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_FMA_EDT,          OnEditchangeCmdTaiwan02SMFactorEdt)	
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_FDV_EDT,          OnEditchangeCmdTaiwan02SMFactorEdt)	
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_FDA_EDT,          OnEditchangeCmdTaiwan02SMFactorEdt)  
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_SDS_EDT2,         OnEditchangeCmdTaiwan02TaipaiEdt)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_SMS_EDT2,         OnEditchangeCmdTaiwan02TaipaiEdt)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_TM0_EDT,          OnEditchangeCmdTaiwan02TaipaiEdt)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_TD0_EDT,          OnEditchangeCmdTaiwan02TaipaiEdt)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_RS_SUBZONE_CMB,  OnSelchangeCmdTaiwan02SubzoneCmb)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_RS_CATEGORY_CMB, OnSelSpecCategoryCmb) // MNET:2410-GSJI-20060829
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CETC_GDS_TAIWAN98::AlignCtrl()
{
	CRect rRef, rMove;
	GetDlgItem(IDC_CMD_TAIWAN02_RS_ACC_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_TAIWAN02_RS_SUBZONE_STC)->GetWindowRect(rMove);
	int nDistY = rRef.top-rMove.top + globalUtils.ScaleByDPI(10);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlMethod2, nDistY);

	GetDlgItem(IDC_CMD_PERIOD_EDIT)->GetWindowRect(rRef);
	GetWindowRect(rMove);
	rMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(12);
	GetParent()->ScreenToClient(rMove);
	MoveWindow(rMove);
}

void CETC_GDS_TAIWAN98::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	
	m_pSpfcUtil->m_nSpecType = m_pParamData->CodeParam.TAIWAN06.nSpectrumType;
	m_pSpfcUtil->m_nSeisZone = m_pParamData->CodeParam.TAIWAN06.nSeismicZone;
	m_pSpfcUtil->m_nSpecCategory = m_pParamData->CodeParam.TAIWAN06.nSpectrumUsed;
	m_pSpfcUtil->m_dSds = m_pParamData->CodeParam.TAIWAN06.dSds;
	m_pSpfcUtil->m_dSd1 = m_pParamData->CodeParam.TAIWAN06.dSd1;
	m_pSpfcUtil->m_dSms = m_pParamData->CodeParam.TAIWAN06.dSms;
	m_pSpfcUtil->m_dSm1 = m_pParamData->CodeParam.TAIWAN06.dSm1;
	m_pSpfcUtil->m_dNda = m_pParamData->CodeParam.TAIWAN06.dNda;
	m_pSpfcUtil->m_dNdv = m_pParamData->CodeParam.TAIWAN06.dNdv;
	m_pSpfcUtil->m_dNma = m_pParamData->CodeParam.TAIWAN06.dNma;
	m_pSpfcUtil->m_dNmv = m_pParamData->CodeParam.TAIWAN06.dNmv;

	m_pSpfcUtil->m_nSoilType = m_pParamData->CodeParam.TAIWAN06.nSoilType;
	m_pSpfcUtil->m_dFda = m_pParamData->CodeParam.TAIWAN06.dFda;
	m_pSpfcUtil->m_dFdv = m_pParamData->CodeParam.TAIWAN06.dFdv;
	m_pSpfcUtil->m_dFma = m_pParamData->CodeParam.TAIWAN06.dFma;
	m_pSpfcUtil->m_dFmv = m_pParamData->CodeParam.TAIWAN06.dFmv;

	m_pSpfcUtil->m_nSubZone = m_pParamData->CodeParam.TAIWAN06.nSubZone;
	m_pSpfcUtil->m_dSds_t = m_pParamData->CodeParam.TAIWAN06.dSds_t;
	m_pSpfcUtil->m_dSms_t = m_pParamData->CodeParam.TAIWAN06.dSms_t;
	m_pSpfcUtil->m_dTd0_t = m_pParamData->CodeParam.TAIWAN06.dTd0_t;
	m_pSpfcUtil->m_dTm0_t = m_pParamData->CodeParam.TAIWAN06.dTm0_t;

	m_pSpfcUtil->m_dI = m_pParamData->CodeParam.TAIWAN06.dImportanceFactor;
	m_pSpfcUtil->m_dAy = m_pParamData->CodeParam.TAIWAN06.dSeisMagnifyFactor;
	m_pSpfcUtil->m_dR = m_pParamData->CodeParam.TAIWAN06.dResponseModifyFactor;
	m_pSpfcUtil->m_dDamping = m_pParamData->CodeParam.TAIWAN06.dDampingRatio;
	m_pSpfcUtil->m_dMaxPeriod = m_pParamData->CodeParam.TAIWAN06.dMaxPeriod;

	if(bInit)
	{
		// pushover curve 에서 호출될때만 사용 
		HWND hWnd = this->GetSafeHwnd();
		if(!hWnd || !(::IsWindow(hWnd))) return;

		UpdateData(FALSE);
		CString strSds, strSd1, strSms, strSm1;
		strSds.Format(_T("%g"), m_pSpfcUtil->m_dSds);
		strSd1.Format(_T("%g"), m_pSpfcUtil->m_dSd1);
		strSms.Format(_T("%g"), m_pSpfcUtil->m_dSms);
		strSm1.Format(_T("%g"), m_pSpfcUtil->m_dSm1);
		m_cmbSds.SetWindowText(strSds);
		m_cmbSd1.SetWindowText(strSd1);
		m_cmbSms.SetWindowText(strSms);
		m_cmbSm1.SetWindowText(strSm1);

		m_edtNda.SetEditUnit(m_pSpfcUtil->m_dNda);     
		m_edtNdv.SetEditUnit(m_pSpfcUtil->m_dNdv);	
		m_edtNma.SetEditUnit(m_pSpfcUtil->m_dNma);     
		m_edtNmv.SetEditUnit(m_pSpfcUtil->m_dNmv);

		m_cmbSoilType.SetCurSel(m_pSpfcUtil->m_nSoilType);
		m_edtFda.SetEditUnit(m_pSpfcUtil->m_dFda);           
		m_edtFdv.SetEditUnit(m_pSpfcUtil->m_dFdv);
		m_edtFma.SetEditUnit(m_pSpfcUtil->m_dFma);           
		m_edtFmv.SetEditUnit(m_pSpfcUtil->m_dFmv);

		m_cmbSubZone.SetCurSel(m_pSpfcUtil->m_nSubZone);
		m_edtSds_t.SetEditUnit(m_pSpfcUtil->m_dSds_t);   
		m_edtSms_t.SetEditUnit(m_pSpfcUtil->m_dSms_t); 
		m_edtTd0.SetEditUnit(m_pSpfcUtil->m_dTd0_t);   
		m_edtTm0.SetEditUnit(m_pSpfcUtil->m_dTm0_t); 

		CString strIe, strAy, strR;
		strIe.Format(_T("%g"), m_pSpfcUtil->m_dI);
		strAy.Format(_T("%g"), m_pSpfcUtil->m_dAy);
		strR .Format(_T("%g"), m_pSpfcUtil->m_dR);

		m_cmbI.SetWindowText(strIe);
		m_cmbR.SetWindowText(strAy);  
		m_cmbAlpha.SetWindowText(strR);
		m_edtDamping.SetEditUnit(m_pSpfcUtil->m_dDamping);
		m_edtMaxPeriod.SetEditUnit(m_pSpfcUtil->m_dMaxPeriod);

	 // OK 또는 Cancel 한 이후 다시 Dlg를 열었을 때 메뉴 세팅  
		CDlgUtil::CtrlShowHide(this, m_aCtrlMethod0, m_pSpfcUtil->m_nSeisZone != 2);
		CDlgUtil::CtrlShowHide(this, m_aCtrlMethod1, m_pSpfcUtil->m_nSeisZone == 1);
		CDlgUtil::CtrlShowHide(this, m_aCtrlMethod2, m_pSpfcUtil->m_nSeisZone==2);

		CArray<UINT, UINT> aCtrlID;
		aCtrlID.Add(IDC_ETC_GDS_TAIWAN_R_STC);
		aCtrlID.Add(IDC_ETC_GDS_TAIWAN_AY_STC);
		aCtrlID.Add(IDC_CMD_TAIWAN02_RS_R_CMB);
		aCtrlID.Add(IDC_CMD_TAIWAN02_RS_ALPHA_CMB);
		aCtrlID.Add(IDC_ETC_GDS_DAMPING_STATIC);
		aCtrlID.Add(IDC_CMD_TAIWAN02_RS_DAMP_EDT);
		CDlgUtil::CtrlShowHide(this, aCtrlID, FALSE);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_TAIWAN98 message handlers
BOOL CETC_GDS_TAIWAN98::OnInitDialog()
{
	CMyChildDialog::OnInitDialog();

	// MNET:2410-GSJI-20060829
	for(int i=0; i < 3; ++i)
		CDlgUtil::CobxAddItem(m_cmbSpecCategory, m_pSpfcUtil->GetSpecCategoryName(i), i);
	m_cmbSpecCategory.SetCurSel(0);

	m_cmbSpecType.SetItemData(m_cmbSpecType.AddString(_LS(IDS_CMD_EURO2003_H_DESIGN_SPECTRUM)),0);  // _T("Horizontal Design Spectrum")
	m_cmbSpecType.SetItemData(m_cmbSpecType.AddString(_LS(IDS_CMD_EURO2003_V_SUPER_DESIGN_SPECTRUM)),1);  // _T("Vertical Design Spectrum - Super")
	m_cmbSpecType.SetItemData(m_cmbSpecType.AddString(_LS(IDS_CMD_EURO2003_V_SUB_DESIGN_SPECTRUM)),2);  // _T("Vertical Design Spectrum - Sub")
	m_cmbSpecType.SetCurSel(0);

	CDlgUtil::CobxAdjustListBoxWidth(m_cmbSpecType);

	for (int i=0; i < 3; i++)
	 CDlgUtil::CobxAddItem(m_cmbSeisZone, m_pSpfcUtil->GetSeismicZoneName(i), i);
	m_cmbSeisZone.SetCurSel(0);  
	
	AlignCtrl();
	CDlgUtil::CtrlShowHide(this, m_aCtrlMethod0, m_pSpfcUtil->m_nSeisZone != 2);
	CDlgUtil::CtrlShowHide(this, m_aCtrlMethod1, m_pSpfcUtil->m_nSeisZone == 1);
	CDlgUtil::CtrlShowHide(this, m_aCtrlMethod2, m_pSpfcUtil->m_nSeisZone==2);

	InitSpecAcc();
	InitSiteMagnifyFactor();
	InitTaipai();
	InitIFactorCombo();
	InitAyFactorCombo();
	InitRCombo();

	m_edtDamping.SetEditUnit(5.0);      //5% 감쇠 기본
	m_pSpfcUtil->m_dDamping = m_edtDamping.GetEditValue();

	m_edtMaxPeriod.SetEditUnit(PRD_ED);
	m_pSpfcUtil->m_dMaxPeriod = m_edtMaxPeriod.GetEditValue();
	
	GetFinalValue();
	CtrlManager();
	
	return TRUE;
}

void CETC_GDS_TAIWAN98::OnSelchangeCmdTaiwan02SeisZoneCmb() 
{
	m_pSpfcUtil->m_nSeisZone = m_cmbSeisZone.GetItemData(m_cmbSeisZone.GetCurSel());
	CDlgUtil::CtrlShowHide(this, m_aCtrlMethod0, m_pSpfcUtil->m_nSeisZone!=2);
	CDlgUtil::CtrlShowHide(this, m_aCtrlMethod1, m_pSpfcUtil->m_nSeisZone==1);
	CDlgUtil::CtrlShowHide(this, m_aCtrlMethod2, m_pSpfcUtil->m_nSeisZone==2);
	SetSiteMagnifyFactor();
}

void CETC_GDS_TAIWAN98::OnSelchangeCmdTaiwan02SpecTypeCmb() 
{
	m_pSpfcUtil->m_nSpecType = m_cmbSpecType.GetItemData(m_cmbSpecType.GetCurSel());

	if(m_pSpfcUtil->m_nSpecType==1)
	{
		m_pSpfcUtil->m_dR = 2.0;
		m_cmbR.SetCurSel(1);
		m_cmbR.EnableWindow(FALSE);
	}
	else
	{
		m_cmbR.EnableWindow(TRUE);
	}

	if(m_pSpfcUtil->m_nSpecType==1 || m_pSpfcUtil->m_nSpecType==2)
	{
		m_pSpfcUtil->m_nSpecCategory = 0;
		CDlgUtil::CobxSetCurSelItemData(m_cmbSpecCategory, m_pSpfcUtil->m_nSpecCategory);
		m_cmbSpecCategory.ShowWindow(FALSE);

		GetDlgItem(IDC_CMD_TAIWAN02_RS_CATEGORY_EDT)->ShowWindow(TRUE);
		GetDlgItem(IDC_CMD_TAIWAN02_RS_CATEGORY_EDT)->SetWindowText(_LS(IDS_CMD_SPFC_TAIWAN02__Design_Spectrum));
		((CEdit*)GetDlgItem(IDC_CMD_TAIWAN02_RS_CATEGORY_EDT))->SetReadOnly();
	}
	else
	{
		m_cmbSpecCategory.ShowWindow(TRUE);
		
		GetDlgItem(IDC_CMD_TAIWAN02_RS_CATEGORY_EDT)->ShowWindow(FALSE);
	}
}

void CETC_GDS_TAIWAN98::OnEditchangeCmdTaiwan02SpecAccCmb() 
{	
	CtrlManager();            
	SetSpectralData();    
	SetSiteMagnifyFactor();
}

void CETC_GDS_TAIWAN98::OnSelchangeCmdTaiwan02SpecAccCmb() 
{
	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_CMD_TAIWAN02_RS_SDS_CMB, CBN_EDITCHANGE), (LPARAM)m_cmbSds.GetSafeHwnd());
	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_CMD_TAIWAN02_RS_SD1_CMB, CBN_EDITCHANGE), (LPARAM)m_cmbSd1.GetSafeHwnd());
	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_CMD_TAIWAN02_RS_SMS_CMB, CBN_EDITCHANGE), (LPARAM)m_cmbSms.GetSafeHwnd());
	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_CMD_TAIWAN02_RS_SM1_CMB, CBN_EDITCHANGE), (LPARAM)m_cmbSm1.GetSafeHwnd());  
}

void CETC_GDS_TAIWAN98::OnSelchangeCmdTaiwan02SoilTypeCmb()
{  
	CtrlManager();            SetSiteMagnifyFactor();
}

void CETC_GDS_TAIWAN98::OnSelchangeCmdTaiwan02SubzoneCmb() 
{		
	CtrlManager();            SetChangeTaipai();
}

void CETC_GDS_TAIWAN98::OnEditchangeCmdTaiwan02NSFactorEdt()
{
	m_pSpfcUtil->m_dNda = m_edtNda.GetEditValue();
	m_pSpfcUtil->m_dNdv = m_edtNdv.GetEditValue();
	m_pSpfcUtil->m_dNma = m_edtNma.GetEditValue();
	m_pSpfcUtil->m_dNmv = m_edtNmv.GetEditValue();
	SetSiteMagnifyFactor();
}

void CETC_GDS_TAIWAN98::OnEditchangeCmdTaiwan02SMFactorEdt()  
{  
	CFormulaEdit::GetEditValue(&m_edtFdv, m_pSpfcUtil->m_dFdv);       
	CFormulaEdit::GetEditValue(&m_edtFda, m_pSpfcUtil->m_dFda);
	CFormulaEdit::GetEditValue(&m_edtFmv, m_pSpfcUtil->m_dFmv);
	CFormulaEdit::GetEditValue(&m_edtFma, m_pSpfcUtil->m_dFma);
}

void CETC_GDS_TAIWAN98::OnEditchangeCmdTaiwan02TaipaiEdt()    
{
	CFormulaEdit::GetEditValue(&m_edtSds_t, m_pSpfcUtil->m_dSds_t);
	CFormulaEdit::GetEditValue(&m_edtSms_t, m_pSpfcUtil->m_dSms_t);
	CFormulaEdit::GetEditValue(&m_edtTd0, m_pSpfcUtil->m_dTd0_t);
	CFormulaEdit::GetEditValue(&m_edtTm0, m_pSpfcUtil->m_dTm0_t);
}


/////////////////////////////////////////////////////////////////////////////
// User Defined Function 

// CETC_DESIGN_SPECT에서 ISVALID()를 호출한 후 End()를 호출한다.
BOOL CETC_GDS_TAIWAN98::ISVALID()
{
	GetFinalValue();

	DWORD nItem;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbSpecCategory, nItem);
	m_pSpfcUtil->m_nSpecCategory = nItem;
	m_pSpfcUtil->m_nSeisZone = m_cmbSeisZone.GetItemData(m_cmbSeisZone.GetCurSel());

	return static_cast<BOOL>(m_pSpfcUtil->CheckValid());
}

void CETC_GDS_TAIWAN98::End()
{
	CMyChildDialog::End();
}

// ----- Initialize Dialog Control ---------------------------------------------------------------------
void CETC_GDS_TAIWAN98::InitSpecAcc()
{
	m_cmbSds.ResetContent();       m_cmbSd1.ResetContent();
	m_cmbSds.AddString(_T("0.5"));     m_cmbSd1.AddString(_T("0.3"));  
	m_cmbSds.AddString(_T("0.6"));     m_cmbSd1.AddString(_T("0.35")); 
	m_cmbSds.AddString(_T("0.7"));     m_cmbSd1.AddString(_T("0.4"));  
	m_cmbSds.AddString(_T("0.8"));     m_cmbSd1.AddString(_T("0.45")); 
	m_cmbSds.SetCurSel(0);         m_cmbSd1.SetCurSel(0);

	m_cmbSms.ResetContent();       m_cmbSm1.ResetContent();
	m_cmbSms.AddString(_T("0.7"));     m_cmbSm1.AddString(_T("0.4"));
	m_cmbSms.AddString(_T("0.8"));     m_cmbSm1.AddString(_T("0.45"));
	m_cmbSms.AddString(_T("0.9"));     m_cmbSm1.AddString(_T("0.5"));
	m_cmbSms.AddString(_T("1.0"));     m_cmbSm1.AddString(_T("0.55"));
	m_cmbSms.SetCurSel(0);         m_cmbSm1.SetCurSel(0);

	m_edtNda.SetWindowText(_T("1.0"));     m_edtNdv.SetWindowText(_T("1.0"));	
	m_edtNma.SetWindowText(_T("1.0"));     m_edtNmv.SetWindowText(_T("1.0"));
}

void CETC_GDS_TAIWAN98::InitSiteMagnifyFactor()
{  
	for (int i=0; i < 4; i++)
		CDlgUtil::CobxAddItem(m_cmbSoilType, m_pSpfcUtil->GetSoilTypeName(i), i);
	m_cmbSoilType.SetCurSel(0);
	m_edtFda.SetWindowText(_T("1.0"));           m_edtFdv.SetWindowText(_T("1.0"));
	m_edtFma.SetWindowText(_T("1.0"));           m_edtFmv.SetWindowText(_T("1.0"));
}

void CETC_GDS_TAIWAN98::InitTaipai()
{
	for (int i=0; i < 4; i++)
		CDlgUtil::CobxAddItem(m_cmbSubZone, m_pSpfcUtil->GetSubZoneName(i), i);
	m_cmbSubZone.SetCurSel(0);
	
	m_edtSds_t.SetEditUnit(0.6);      m_edtTd0.SetEditUnit(1.6);       
	m_edtSms_t.SetEditUnit(0.8);      m_edtTm0.SetEditUnit(1.6);
	
	m_pSpfcUtil->m_dSds_t =m_edtSds_t.GetEditValue();     m_pSpfcUtil->m_dTd0_t =m_edtTd0.GetEditValue();
	m_pSpfcUtil->m_dSms_t =m_edtSms_t.GetEditValue();     m_pSpfcUtil->m_dTm0_t =m_edtTm0.GetEditValue();
}

void CETC_GDS_TAIWAN98::InitIFactorCombo()
{
	CString aItem[] = {_T("1.0"), _T("1.2")};
	m_cmbI.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbI.AddString(aItem[i]);
	m_cmbI.SetCurSel(0);
}

void CETC_GDS_TAIWAN98::InitAyFactorCombo()
{
	CString aItem[] = {_T("1.65"), _T("1.7")};
	m_cmbAlpha.ResetContent();
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbAlpha.AddString(aItem[i]);
	m_cmbAlpha.SetCurSel(0);
}

void CETC_GDS_TAIWAN98::InitRCombo()
{
	CString aItem[] = {_T("1.6"), _T("2.0"), _T("3.0"), _T("4.2")};
	m_cmbR.ResetContent();	
	for(int i = 0; i < sizeof(aItem) / sizeof(CString); i++)
		m_cmbR.AddString(aItem[i]);
	m_cmbR.SetCurSel(0);
}

void CETC_GDS_TAIWAN98::CtrlManager()
{ 
	// MNET:2410-GSJI-20060908
	CDlgUtil::CtrlEnableDisable(this, m_aCtrl1, m_pSpfcUtil->m_nSpecCategory != 2);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrl2, m_pSpfcUtil->m_nSpecCategory == 2);

	if (m_pSpfcUtil->m_nSeisZone == 1 && m_pSpfcUtil->m_nSpecCategory == 1)
	{
		m_edtNda.SetReadOnly();
		m_edtNdv.SetReadOnly();
		m_pSpfcUtil->m_dNda = 1.0;
		m_edtNda.SetEditUnit(m_pSpfcUtil->m_dNda, 1);
		m_pSpfcUtil->m_dNdv = 1.0;
		m_edtNdv.SetEditUnit(m_pSpfcUtil->m_dNdv, 1);
	}
	else
	{
		m_edtNda.SetReadOnly(FALSE);
		m_edtNdv.SetReadOnly(FALSE);
	}

	m_edtDamping.EnableWindow(FALSE);
	m_edtDamping.SetEditUnit(5.0);      //5% 감쇠 기본
	m_pSpfcUtil->m_dDamping = m_edtDamping.GetEditValue();

	//Site Magnify Factor
	m_pSpfcUtil->m_nSoilType = m_cmbSoilType.GetItemData(m_cmbSoilType.GetCurSel());

	if(m_pSpfcUtil->m_nSpecCategory !=2)
	{
		m_edtFdv.SetReadOnly(m_pSpfcUtil->m_nSoilType != 3);    m_edtFda.SetReadOnly(m_pSpfcUtil->m_nSoilType != 3);
	}
	else
	{
		m_edtFmv.SetReadOnly(m_pSpfcUtil->m_nSoilType != 3);    m_edtFma.SetReadOnly(m_pSpfcUtil->m_nSoilType != 3);
	}
		
	// Taipei Basin only
	m_pSpfcUtil->m_nSubZone = m_cmbSubZone.GetItemData(m_cmbSubZone.GetCurSel());
	if(m_pSpfcUtil->m_nSpecCategory !=2)
	{
		m_edtSds_t.SetReadOnly(m_pSpfcUtil->m_nSubZone != 3);   m_edtTd0.SetReadOnly(m_pSpfcUtil->m_nSubZone != 3);
	}
	else
	{
		m_edtSms_t.SetReadOnly(m_pSpfcUtil->m_nSubZone != 3);   m_edtTm0.SetReadOnly(m_pSpfcUtil->m_nSubZone != 3);
	}
}
// -----------------------------------------------------------------------------------------------------

// ----- Set Member Data -------------------------------------------------------------------------------
void CETC_GDS_TAIWAN98::SetSpectralData()
{
	CFormulaEdit::GetEditValue(&m_cmbSds, m_pSpfcUtil->m_dSds);
	CFormulaEdit::GetEditValue(&m_cmbSd1, m_pSpfcUtil->m_dSd1);
	CFormulaEdit::GetEditValue(&m_cmbSms, m_pSpfcUtil->m_dSms);
	CFormulaEdit::GetEditValue(&m_cmbSm1, m_pSpfcUtil->m_dSm1);
}

void CETC_GDS_TAIWAN98::SetSiteMagnifyFactor()
{   
	if (m_pSpfcUtil->m_nSoilType == 3)      //User
	{
		OnEditchangeCmdTaiwan02SMFactorEdt();
	}
	else                    //Type1 ~ 3
	{
		CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
		if (m_pSpfcUtil->m_nSeisZone == 0 || m_pSpfcUtil->m_nSeisZone == 1)
		{
			if (!pDoc->m_pAttrCtrl2->CalcTaiwan98SiteMagnifyFactor(m_pSpfcUtil->m_nSeisZone, m_pSpfcUtil->m_nSoilType,
				m_pSpfcUtil->m_dSds, m_pSpfcUtil->m_dSd1, m_pSpfcUtil->m_dSms, m_pSpfcUtil->m_dSm1, m_pSpfcUtil->m_dNda, m_pSpfcUtil->m_dNdv, m_pSpfcUtil->m_dNma, m_pSpfcUtil->m_dNmv, m_pSpfcUtil->m_dFda, m_pSpfcUtil->m_dFdv, m_pSpfcUtil->m_dFma, m_pSpfcUtil->m_dFmv))
				return;

			if (m_pSpfcUtil->m_nSeisZone == 1)      // 내부 계산용
			{
				double dNda_1 = 1.0, dNdv_1 = 1.0;
				if (!pDoc->m_pAttrCtrl2->CalcTaiwan98SiteMagnifyFactor(m_pSpfcUtil->m_nSeisZone, m_pSpfcUtil->m_nSoilType,
					m_pSpfcUtil->m_dSds, m_pSpfcUtil->m_dSd1, m_pSpfcUtil->m_dSms, m_pSpfcUtil->m_dSm1, dNda_1, dNdv_1, m_pSpfcUtil->m_dNma, m_pSpfcUtil->m_dNmv, m_pSpfcUtil->m_dFda_1, m_pSpfcUtil->m_dFdv_1, m_pSpfcUtil->m_dFma, m_pSpfcUtil->m_dFmv))
					return;

			}
		}
		if (m_pSpfcUtil->m_nSpecCategory != 2)         // MNET:2410-GSJI-20060829
		{
			m_edtFda.SetEditUnit(m_pSpfcUtil->m_dFda, 1);        m_edtFdv.SetEditUnit(m_pSpfcUtil->m_dFdv, 1);
		}
		else
		{
			m_edtFma.SetEditUnit(m_pSpfcUtil->m_dFma, 1);        m_edtFmv.SetEditUnit(m_pSpfcUtil->m_dFmv, 1);
		}
	}
}

void CETC_GDS_TAIWAN98::SetChangeTaipai()
{    
	m_edtSds_t.SetEditUnit(0.6);            m_edtSms_t.SetEditUnit(0.8); 
	
	m_pSpfcUtil->m_nSubZone = m_cmbSubZone.GetItemData(m_cmbSubZone.GetCurSel());
	if(m_pSpfcUtil->m_nSpecCategory != 2)
	{
		if     (m_pSpfcUtil->m_nSubZone==0)  m_edtTd0.SetEditUnit(1.6);    
		else if(m_pSpfcUtil->m_nSubZone==1)  m_edtTd0.SetEditUnit(1.3);    
		else if(m_pSpfcUtil->m_nSubZone==2)  m_edtTd0.SetEditUnit(1.05);   
		//else if(m_nSubZone==3)  m_edtTd0.SetEditUnit(0.85);   
		else                    m_edtTd0.SetWindowText(_T("1.0"));// User Input
	}
	else
	{    
		if     (m_pSpfcUtil->m_nSubZone==0)  m_edtTm0.SetEditUnit(1.6); 
		else if(m_pSpfcUtil->m_nSubZone==1)  m_edtTm0.SetEditUnit(1.3); 
		else if(m_pSpfcUtil->m_nSubZone==2)  m_edtTm0.SetEditUnit(1.05);
		//else if(m_nSubZone==3)  m_edtTm0.SetEditUnit(0.85);
		else                    m_edtTm0.SetWindowText(_T("1.0")); // User Input
	}
}

void CETC_GDS_TAIWAN98::GetFinalValue()
{
	m_pSpfcUtil->m_nSpecType     = m_cmbSpecType.GetCurSel();
	m_pSpfcUtil->m_nSpecCategory = m_cmbSpecCategory.GetCurSel();
	m_pSpfcUtil->m_nSeisZone     = m_cmbSeisZone.GetCurSel();
	
	CDlgUtil::CobxGetCurSelItemData(m_cmbSpecCategory, m_pSpfcUtil->m_nSpecCategory);
	
	if(m_pSpfcUtil->m_nSeisZone==0 || m_pSpfcUtil->m_nSeisZone==1)
	{
		SetSpectralData();
		SetSiteMagnifyFactor();
		if(m_pSpfcUtil->m_nSeisZone==1)
			OnEditchangeCmdTaiwan02NSFactorEdt();
	}
	else
		OnEditchangeCmdTaiwan02TaipaiEdt();   

	CString strI, strR, strAy, strDamp;
	m_cmbI.GetWindowText(strI);    
	m_cmbAlpha.GetWindowText(strAy);    
	m_cmbR.GetWindowText(strR);
	m_edtDamping.GetWindowText(strDamp);
	m_pSpfcUtil->m_dI = _tstof(strI);
	m_pSpfcUtil->m_dAy= _tstof(strAy);
	m_pSpfcUtil->m_dR = _tstof(strR);
	m_pSpfcUtil->m_dDamping = _tstof(strDamp);
	
	m_pSpfcUtil->m_dMaxPeriod = m_edtMaxPeriod.GetEditValue();
}
// -----------------------------------------------------------------------------------------------------

void CETC_GDS_TAIWAN98::OnSelSpecCategoryCmb()     // MNET:2410-GSJI-20060829
{  
	DWORD nItem;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbSpecCategory, nItem);
	m_pSpfcUtil->m_nSpecCategory = nItem;
	CtrlManager();
	SetSiteMagnifyFactor();
}

BOOL CETC_GDS_TAIWAN98::Dlg2Data()
{
	m_pParamData->CodeParam.TAIWANBRG98.nSpectrumType     = m_pSpfcUtil->m_nSpecType;
	m_pParamData->CodeParam.TAIWANBRG98.nSeismicZone      = m_pSpfcUtil->m_nSeisZone;
	m_pParamData->CodeParam.TAIWANBRG98.nSpectrumUsed     = m_pSpfcUtil->m_nSpecCategory;
	m_pParamData->CodeParam.TAIWANBRG98.dSds              = m_pSpfcUtil->m_dSds;
	m_pParamData->CodeParam.TAIWANBRG98.dSd1              = m_pSpfcUtil->m_dSd1;
	m_pParamData->CodeParam.TAIWANBRG98.dSms              = m_pSpfcUtil->m_dSms;
	m_pParamData->CodeParam.TAIWANBRG98.dSm1              = m_pSpfcUtil->m_dSm1;
	m_pParamData->CodeParam.TAIWANBRG98.dNda              = m_pSpfcUtil->m_dNda;
	m_pParamData->CodeParam.TAIWANBRG98.dNdv              = m_pSpfcUtil->m_dNdv;
	m_pParamData->CodeParam.TAIWANBRG98.dNma              = m_pSpfcUtil->m_dNma;
	m_pParamData->CodeParam.TAIWANBRG98.dNmv              = m_pSpfcUtil->m_dNmv;

	m_pParamData->CodeParam.TAIWANBRG98.nSoilType         = m_pSpfcUtil->m_nSoilType;
	m_pParamData->CodeParam.TAIWANBRG98.dFda              = m_pSpfcUtil->m_dFda;
	m_pParamData->CodeParam.TAIWANBRG98.dFdv              = m_pSpfcUtil->m_dFdv;
	m_pParamData->CodeParam.TAIWANBRG98.dFma              = m_pSpfcUtil->m_dFma;
	m_pParamData->CodeParam.TAIWANBRG98.dFmv              = m_pSpfcUtil->m_dFmv;

	m_pParamData->CodeParam.TAIWANBRG98.nSubZone          = m_pSpfcUtil->m_nSubZone;
	m_pParamData->CodeParam.TAIWANBRG98.dSds_t            = m_pSpfcUtil->m_dSds_t;
	m_pParamData->CodeParam.TAIWANBRG98.dSms_t            = m_pSpfcUtil->m_dSms_t;
	m_pParamData->CodeParam.TAIWANBRG98.dTd0_t            = m_pSpfcUtil->m_dTd0_t;
	m_pParamData->CodeParam.TAIWANBRG98.dTm0_t            = m_pSpfcUtil->m_dTm0_t;

	m_pParamData->CodeParam.TAIWANBRG98.dImportanceFactor     = m_pSpfcUtil->m_dI;
	m_pParamData->CodeParam.TAIWANBRG98.dSeisMagnifyFactor    = m_pSpfcUtil->m_dAy;
	m_pParamData->CodeParam.TAIWANBRG98.dResponseModifyFactor = m_pSpfcUtil->m_dR;
	m_pParamData->CodeParam.TAIWANBRG98.dDampingRatio         = m_pSpfcUtil->m_dDamping;
	m_pParamData->CodeParam.TAIWANBRG98.dMaxPeriod            = m_pSpfcUtil->m_dMaxPeriod;

	m_pParamData->dResModCoef = m_pSpfcUtil->m_dR;
	return TRUE;
}
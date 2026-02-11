// ETC_GDS_UBC97.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_UBC97.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "SpfcUBC97Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_UBC97 dialog


CETC_GDS_UBC97::CETC_GDS_UBC97(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_UBC97::IDD, pParent, std::make_shared<CSpfcUBC97Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_UBC97)
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcUBC97Util>(m_pSpfcBaseUtil);

	m_nOption = 0;
	m_sIF = _T("1.0");
	m_sNC = _T("1.0");			
	m_dMaxPeriod = PRD_ED;

	m_nSoilType = 0;
	m_nSeisZone = 0;
	m_nSourceType = 0;
	m_sDist = _T("10.0");
	//}}AFX_DATA_INIT
}


void CETC_GDS_UBC97::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_UBC97)
	DDX_Radio(pDX, IDC_CMD_UBC97_AUTO_RDO1, m_nOption);	
	DDX_Control(pDX, IDC_CMD_UBC97_CV_EDIT, m_wndCv);
	DDX_Control(pDX, IDC_CMD_UBC97_CA_EDIT, m_wndCa);	
	DDX_Control(pDX, IDC_CMD_UBC97_SEIS_ZONE_CMB, m_SeisZoneCmb);
	DDX_Control(pDX, IDC_CMD_UBC97_SOIL_TYPE_CMB, m_SoilTypeCmb);	
	DDX_Control(pDX, IDC_CMD_UBC97_SRC_TYPE_CMB, m_SourceTypeCmb);	
	DDX_Control(pDX, IDC_ETC_GDS_CMB_RMC, m_cmbNC);
	DDX_Control(pDX, IDC_ETC_GDS_UBC97_CMB_IF, m_cmbIF);
	DDX_Control(pDX, IDC_CMD_UBC97_CLOSE_DIST_EDIT, m_wndCloseDist);	
	DDX_CBString(pDX, IDC_ETC_GDS_UBC97_CMB_IF, m_sIF);
	DDX_CBString(pDX, IDC_ETC_GDS_CMB_RMC, m_sNC);	
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_UBC97, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_UBC97)
	ON_CBN_SELCHANGE(IDC_CMD_UBC97_SOIL_TYPE_CMB, OnSelchangeSoiltype)
	ON_CBN_SELCHANGE(IDC_CMD_UBC97_SEIS_ZONE_CMB, OnSelchangeSeisZone)
	ON_CBN_SELCHANGE(IDC_CMD_UBC97_SRC_TYPE_CMB, OnSelchangeSourceType)
	ON_BN_CLICKED(IDC_CMD_UBC97_AUTO_RDO1, OnUbc97AutoRdo)
	ON_BN_CLICKED(IDC_CMD_UBC97_AUTO_RDO2, OnUbc97AutoRdo)
	ON_EN_CHANGE(IDC_CMD_UBC97_CLOSE_DIST_EDIT, OnChangeDist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_UBC97 message handlers

BOOL CETC_GDS_UBC97::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();

	CString strSoilType[]={_T("Sa"),_T("Sb"),_T("Sc"),_T("Sd"),_T("Se")};
	CString strSZoneFactor[]={_T("Zone1 (0.075)"),_T("Zone2A (0.15)"),_T("Zone2B (0.20)"),_T("Zone3 (0.30)"),_T("Zone4 (0.40)")};		
	CString strSourceType[]={_T("A "),_T("B "),_T("C ")};
	m_wndCloseDist.SetWindowText(m_sDist);
		
	((CButton*)GetDlgItem(IDC_ETC_GDS_UBC97_I_STATIC))->ShowWindow(FALSE);
	((CButton*)GetDlgItem(IDC_ETC_GDS_UBC97_R_STATIC))->ShowWindow(FALSE);
	((CButton*)GetDlgItem(IDC_ETC_GDS_UBC97_CMB_IF))->ShowWindow(FALSE);
	((CButton*)GetDlgItem(IDC_ETC_GDS_CMB_RMC))->ShowWindow(FALSE);

 	for(int i=0;i<5;i++)
	{
		m_SoilTypeCmb.AddString(strSoilType[i]);
		m_SeisZoneCmb.AddString(strSZoneFactor[i]);
	}

	for(int  i=0;i<3;i++)
	{
		m_SourceTypeCmb.AddString(strSourceType[i]);
	}

 	m_SoilTypeCmb.SetCurSel(m_nSoilType);
	m_SeisZoneCmb.SetCurSel(m_nSeisZone);
	m_SourceTypeCmb.SetCurSel(m_nSourceType);
 	CtrlManager(); 
	AutoCalcSeisCoef();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_UBC97::OnUbc97AutoRdo() 
{
	CtrlManager();
	AutoCalcSeisCoef();
}

void CETC_GDS_UBC97::OnSelchangeSoiltype() 
{	
	AutoCalcSeisCoef();
}

void CETC_GDS_UBC97::OnSelchangeSeisZone() 
{	
	CtrlManager();
	AutoCalcSeisCoef();
}

void CETC_GDS_UBC97::OnSelchangeSourceType() 
{	
	AutoCalcSeisCoef();
}

void CETC_GDS_UBC97::OnChangeDist() 
{
	AutoCalcSeisCoef();
}

void CETC_GDS_UBC97::CtrlManager() 
{
	UpdateData(TRUE);  
	int nSeisZone = m_SeisZoneCmb.GetCurSel();

	m_wndCa.SetReadOnly(m_nOption == 0);
	m_wndCv.SetReadOnly(m_nOption == 0);
	
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_UBC97_AUTO_CALC_DATA_FRM, m_nOption == 0, TRUE);
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_CMD_UBC97_NEAR_SRC_FRM, m_nOption == 0 && nSeisZone == 4, TRUE);
	
	UpdateData(TRUE);  
}

void CETC_GDS_UBC97::AutoCalcSeisCoef() 
{
	UpdateData(TRUE);

	int nSoilType  = m_SoilTypeCmb.GetCurSel();
	int nSeisZone  = m_SeisZoneCmb.GetCurSel();	
	int nSourceType= m_SourceTypeCmb.GetCurSel();

	m_pSpfcUtil->SetSoilType(nSoilType);
	m_pSpfcUtil->SetSeisZone(nSeisZone);
	m_pSpfcUtil->SetSourceType(nSourceType);

	CString strDist;
	m_wndCloseDist.GetWindowText(strDist);
	m_pSpfcUtil->SetClosestDist(_tstof(strDist));

	if (nSeisZone == 4)
	{
		CString str, strMsg;
		GetDlgItem(IDC_CMD_UBC97_CLOSE_DIST_STATIC)->GetWindowText(str);
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_NONE_NEGA), str);
		if (m_pSpfcUtil->GetClosestDist() < 0) { AfxMessageBox(strMsg); return; }
	}

	m_pSpfcUtil->AutoCalcSeisCoef();
	
	CString strCa, strCv;
	strCa.Format(_T("%g"), m_pSpfcUtil->GetCa());
	strCv.Format(_T("%g"), m_pSpfcUtil->GetCv());

	m_wndCa.SetWindowText(strCa);
	m_wndCv.SetWindowText(strCv);

	UpdateData(FALSE);

}

void CETC_GDS_UBC97::GetFinalValue()
{
	UpdateData(TRUE);

	m_nSoilType   = m_SoilTypeCmb.GetCurSel();
	m_nSeisZone   = m_SeisZoneCmb.GetCurSel();
	m_nSourceType = m_SourceTypeCmb.GetCurSel();
	m_wndCloseDist.GetWindowText(m_sDist);

	CString strCa, strCv;
	m_wndCa.GetWindowText(strCa);
	m_wndCv.GetWindowText(strCv);

	m_pSpfcUtil->SetCa(_tstof(strCa));
	m_pSpfcUtil->SetCv(_tstof(strCv));

	m_pSpfcUtil->SetIF(_tstof(m_sIF));
	m_pSpfcUtil->SetNC(_tstof(m_sNC));
}

void CETC_GDS_UBC97::End()
{
	CMyChildDialog::End();

	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);
}

BOOL CETC_GDS_UBC97::ISVALID()
{  
	GetFinalValue();

 	CString strCa, strCv;
	m_wndCa.GetWindowText(strCa);
	m_wndCv.GetWindowText(strCv);

	double Ca = _tstof(strCa);
	double Cv = _tstof(strCv);

	CString str, strMsg;
	if(Ca <= 0)
	{
		GetDlgItem(IDC_CMD_UBC97_SEIS_COEF_CA_STATIC)->GetWindowText(str);
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), str);
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if(Cv <= 0)
	{
		GetDlgItem(IDC_CMD_UBC97_SEIS_COEF_CV_STATIC)->GetWindowText(str);
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), str);    
		AfxMessageBox(strMsg);
		return FALSE;
	}

//  if(!IsValidCmb_d(&m_cmbIF,0,10,1.0)) return FALSE;
//  if(!IsValidCmb_d(&m_cmbNC,0,50,1.0)) return FALSE;  
	if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  
	return TRUE;  
}

BOOL CETC_GDS_UBC97::Dlg2Data()
{
	m_pParamData->CodeParam.UBC97.nOption     = m_nOption;
	m_pParamData->CodeParam.UBC97.nSoilType   = m_nSoilType; 
	m_pParamData->CodeParam.UBC97.nSeisZone   = m_nSeisZone;  
	m_pParamData->CodeParam.UBC97.nSourceType = m_nSourceType;

	m_pParamData->CodeParam.UBC97.dClosestDist = _tstof(m_sDist);
	m_pParamData->CodeParam.UBC97.dCa   = m_pSpfcUtil->GetCa();
	m_pParamData->CodeParam.UBC97.dCv   = m_pSpfcUtil->GetCv();
	m_pParamData->CodeParam.UBC97.dIe   = m_pSpfcUtil->GetIF();
	m_pParamData->CodeParam.UBC97.dCoef = m_pSpfcUtil->GetNC();

	m_pParamData->CodeParam.UBC97.dMaxPeriod = m_dMaxPeriod;
	
	return TRUE;
}

void CETC_GDS_UBC97::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	m_nOption     = m_pParamData->CodeParam.UBC97.nOption;
	m_nSoilType   = m_pParamData->CodeParam.UBC97.nSoilType;
	m_nSeisZone   = m_pParamData->CodeParam.UBC97.nSeisZone;
	m_nSourceType = m_pParamData->CodeParam.UBC97.nSourceType;

	m_pSpfcUtil->SetCa(m_pParamData->CodeParam.UBC97.dCa);
	m_pSpfcUtil->SetCv(m_pParamData->CodeParam.UBC97.dCv);
	m_pSpfcUtil->SetIF(m_pParamData->CodeParam.UBC97.dIe);
	m_pSpfcUtil->SetNC(m_pParamData->CodeParam.UBC97.dCoef);

	m_dMaxPeriod = m_pParamData->CodeParam.UBC97.dMaxPeriod;

	m_pSpfcUtil->SetOption(m_nOption);
	m_pSpfcUtil->SetSoilType(m_nSoilType);
	m_pSpfcUtil->SetSeisZone(m_nSeisZone);
	m_pSpfcUtil->SetSourceType(m_nSourceType);
	//m_pSpfcUtil->SetClosestDist(m_pParamData->CodeParam.UBC97.dClosestDist);
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);

	if(bInit)
	{
		UpdateData(FALSE);

		m_SoilTypeCmb.SetCurSel(m_pParamData->CodeParam.UBC97.nSoilType);
		m_SeisZoneCmb.SetCurSel(m_pParamData->CodeParam.UBC97.nSeisZone);
		m_SourceTypeCmb.SetCurSel(m_pParamData->CodeParam.UBC97.nSourceType);

		CString strDist; strDist.Format(_T("%g"), m_pParamData->CodeParam.UBC97.dClosestDist);
		m_wndCloseDist.SetWindowText(strDist);

		CString strCa, strCv;
		strCa.Format(_T("%g"), m_pParamData->CodeParam.UBC97.dCa);
		strCv.Format(_T("%g"), m_pParamData->CodeParam.UBC97.dCv);

		m_wndCa.SetWindowText(strCa);
		m_wndCv.SetWindowText(strCv);

		CString strIF, strNC;
		strIF.Format(_T("%g"), m_pParamData->CodeParam.UBC97.dIe);
		strNC.Format(_T("%g"), m_pParamData->CodeParam.UBC97.dCoef);

		m_cmbIF.SetWindowText(strIF);
		m_cmbNC.SetWindowText(strNC);

		m_sIF  = strIF;
		m_sNC  = strNC;
		m_cmbNC.EnableWindow(FALSE);
		CtrlManager();
		//AutoCalcSeisCoef();
	}
}
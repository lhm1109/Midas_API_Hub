// ETC_GDS_GB50111_2006.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_GB50111_2006.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "SpfcGB50111_2006Util.h"

using namespace mit::lib;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_GB50111_2006 dialog

CETC_GDS_GB50111_2006::CETC_GDS_GB50111_2006(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_GB50111_2006::IDD, pParent, std::make_shared<CSpfcGB50111_2006Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_GB50111_2006)
	m_nEarthEff = -1;
	m_nDivision = -1;
	m_nSFI = -1;
	m_nSiteClass = -1;
	m_dMaxPeriod = PRD_ED;

	//}}AFX_DATA_INIT
	/* // 아래와 같이 주소를 넘겨받으므로 불필요함. 다른 코드도 다 수정하면 지우자.
	m_nEarthEff		= 0;
	m_nDivision	= 0; 
	m_nSFI	= 0;
	m_nSiteClass		= 0;
	m_dXi = 0.05;
	m_dTg = 0.25;
	*/
	m_pParamData = pParamData;    // MNET:2138-GSJI-20060530
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcGB50111_2006Util>(m_pSpfcBaseUtil);

	//SetValue();
}

void CETC_GDS_GB50111_2006::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_GB50111_2006)
	DDX_Radio(pDX, IDD_ETC_GDS_CH2002_RDO_SDC1, m_nDivision);     // division
	DDX_Radio(pDX, IDD_ETC_GDS_CH2002_RDO_Site1, m_nSiteClass);   // site class
	DDX_Radio(pDX, IDD_ETC_GDS_CH2002_RDO_SFI1, m_nSFI);          // seismic fortification intensity
	DDX_Radio(pDX, IDD_ETC_GDS_CH2002_RDO_ER1, m_nEarthEff);      // earthquake effect

	DDX_Control(pDX, IDD_ETC_GDS_CH2002_TG_EDIT, m_edtTg);        // Tg
	DDX_Control(pDX, IDD_ETC_GDS_CH2002_G_EDIT, m_edtG);          // g

	DDX_Control(pDX, IDC_CMD_IMPORTANT_BR, m_chkBridge);          // bridge
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_GB50111_2006, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_GB50111_2006)
	
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_SDC1, OnCalculateTg)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_SDC2, OnCalculateTg)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_SDC3, OnCalculateTg)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_Site1, OnCalculateTg)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_Site2, OnCalculateTg)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_Site3, OnCalculateTg)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_Site4, OnCalculateTg)

	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_SFI1, OnCalculateG)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_SFI2, OnCalculateG)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_SFI3, OnCalculateG)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_SFI4, OnCalculateG)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_SFI5, OnCalculateG)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_SFI6, OnCalculateG)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_ER1, OnCalculateG)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_ER2, OnCalculateG)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_ER3, OnCalculateG)

	ON_BN_CLICKED(IDC_CMD_IMPORTANT_BR, OnClickBtnBR)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_GB50111_2006 message handlers

BOOL CETC_GDS_GB50111_2006::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	//m_edtTg.SetEditUnit(m_dTg);
	//m_edtG.SetEditUnit(m_dG);
	 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CETC_GDS_GB50111_2006::Dlg2Data()
{
	// UpdateData(TRUE); // CETC_DESIGN_SPECT에서 Dlg2Data()호출 전에 이미 호출되었다.
	m_pParamData->CodeParam.GB50111_2006.Initialize();

	m_pParamData->nSpecCodeType = D_SPFC_CODE_GB50111_2006;   //확인사살~  
	m_pParamData->CodeParam.GB50111_2006.nDivision          = m_nDivision;  
	m_pParamData->CodeParam.GB50111_2006.nSiteClass         = m_nSiteClass;   	 
	m_pParamData->CodeParam.GB50111_2006.nSeisFortification = m_nSFI;
	m_pParamData->CodeParam.GB50111_2006.nEarthResponse     = m_nEarthEff;  

	m_pParamData->CodeParam.GB50111_2006.dTg                = m_edtTg.GetEditValue();
	m_pParamData->CodeParam.GB50111_2006.dG                 = m_edtG.GetEditValue();
	m_pParamData->CodeParam.GB50111_2006.bImportantBridge   = m_pSpfcUtil->GetImportantBridge();
	m_pParamData->CodeParam.GB50111_2006.dMaxPeriod = m_dMaxPeriod;

	m_pSpfcUtil->SetTg(m_edtTg.GetEditValue());
	m_pSpfcUtil->SetG(m_edtG.GetEditValue());

	return TRUE;
}

void CETC_GDS_GB50111_2006::Data2Dlg()
{  
	if(m_pParamData->nSpecCodeType!=D_SPFC_CODE_GB50111_2006)
	{
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_GB50111_2006);    
	}
	m_nDivision   = m_pParamData->CodeParam.GB50111_2006.nDivision;
	m_nSiteClass  = m_pParamData->CodeParam.GB50111_2006.nSiteClass;    
	m_nSFI        = m_pParamData->CodeParam.GB50111_2006.nSeisFortification;
	m_nEarthEff   = m_pParamData->CodeParam.GB50111_2006.nEarthResponse;
	m_dMaxPeriod = m_pParamData->CodeParam.GB50111_2006.dMaxPeriod;

	m_chkBridge.SetCheck(m_pParamData->CodeParam.GB50111_2006.bImportantBridge);
	m_edtTg.SetEditUnit(m_pParamData->CodeParam.GB50111_2006.dTg);
	m_edtG.SetEditUnit(m_pParamData->CodeParam.GB50111_2006.dG);

	Dlg2UtilData();
	UpdateData(FALSE);
}

void CETC_GDS_GB50111_2006::Initialize() 
{
	m_pParamData->CodeParam.Initialize(D_SPFC_CODE_GB50111_2006);
}

void CETC_GDS_GB50111_2006::ShowDlgData() 
{
	if(m_pParamData->nSpecCodeType == D_SPFC_CODE_GB50111_2006)
		m_CodeD.GB50111_2006 = m_pParamData->CodeParam.GB50111_2006;
	else
		m_CodeD.GB50111_2006.Initialize();
	
	m_nDivision   = m_CodeD.GB50111_2006.nDivision;
	m_nSiteClass  = m_CodeD.GB50111_2006.nSiteClass;
	m_nSFI        = m_CodeD.GB50111_2006.nSeisFortification;
	m_nEarthEff   = m_CodeD.GB50111_2006.nEarthResponse;

	m_chkBridge.SetCheck(m_pParamData->CodeParam.GB50111_2006.bImportantBridge);
	m_edtTg.SetEditUnit(m_CodeD.GB50111_2006.dTg);
	m_edtG.SetEditUnit(m_CodeD.GB50111_2006.dG);

	m_dMaxPeriod = m_CodeD.GB50111_2006.dMaxPeriod;

	Dlg2UtilData();
	UpdateData(FALSE);
}

void CETC_GDS_GB50111_2006::Dlg2UtilData()
{
	m_pSpfcUtil->SetDivision(m_nDivision);
	m_pSpfcUtil->SetSiteClass(m_nSiteClass);
	m_pSpfcUtil->SetSFI(m_nSFI);
	m_pSpfcUtil->SetEarthEff(m_nEarthEff);
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);

	m_pSpfcUtil->SetTg(m_edtTg.GetEditValue());
	m_pSpfcUtil->SetG(m_edtG.GetEditValue());
	m_pSpfcUtil->SetImportantBridge(m_chkBridge.GetCheck());
}

// void CETC_GDS_GB50111_2006::UpdateDampingWnd(CWnd* pDampingWnd)
// {
//   CString csDamping;
//   csDamping.Format(_T("%g"), m_dXi);
//   pDampingWnd->SetWindowText(csDamping);
// }

void CETC_GDS_GB50111_2006::End()
{
	UpdateData(TRUE);  
	
	//SetValue();

	Dlg2UtilData();
	UpdateData(FALSE);

	CMyChildDialog::End();
}

// void CETC_GDS_GB50111_2006::SetValue()
// {
	// switch(m_nEarthEff)
	// {
		// case 0: 
			// if(m_nSFI == 0) m_dAlphaMax = 0.04;
			// else if(m_nSFI == 1) m_dAlphaMax = 0.08;
			// else if(m_nSFI == 2) m_dAlphaMax = 0.12;
			// else if(m_nSFI == 3) m_dAlphaMax = 0.16;
			// else if(m_nSFI == 4) m_dAlphaMax = 0.24;
			// else if(m_nSFI == 5) m_dAlphaMax = 0.32;				
		// break;
		// case 1:
			// if(m_nSFI == 0) m_dAlphaMax = 0.0;   // 수정 필요
			// else if(m_nSFI == 1) m_dAlphaMax = 0.50;
			// else if(m_nSFI == 2) m_dAlphaMax = 0.72;
			// else if(m_nSFI == 3) m_dAlphaMax = 0.90;
			// else if(m_nSFI == 4) m_dAlphaMax = 1.20;
			// else if(m_nSFI == 5) m_dAlphaMax = 1.40;								
		// break;
	// }
// }

void CETC_GDS_GB50111_2006::OnCalculateTg() 
{
	UpdateData(TRUE);
	double dTg;
	switch(m_nDivision)
	{
		case 0: 
			if(m_nSiteClass == 0) dTg = 0.25;
			else if(m_nSiteClass == 1) dTg = 0.35;
			else if(m_nSiteClass == 2) dTg = 0.45;
			else if(m_nSiteClass == 3) dTg = 0.65;
		break;
		case 1: 
			if(m_nSiteClass == 0) dTg = 0.30;
			else if(m_nSiteClass == 1) dTg = 0.40;
			else if(m_nSiteClass == 2) dTg = 0.55;
			else if(m_nSiteClass == 3) dTg = 0.75;
		break;
		case 2: 
			if(m_nSiteClass == 0) dTg = 0.35;
			else if(m_nSiteClass == 1) dTg = 0.45;
			else if(m_nSiteClass == 2) dTg = 0.65;
			else if(m_nSiteClass == 3) dTg = 0.90;				
		break;
	}

	//if(m_nEarthEff==1 && (m_nSFI==3 || m_nSFI== 4 || m_nSFI==5))
	//	dTg = dTg + 0.05;    // 8,9도 한우지진일때 특정 주기 Tg는 0.05sec 증가	

	m_edtTg.SetEditUnit(dTg);
}

void CETC_GDS_GB50111_2006::OnClickBtnBR()
{
	UpdateData(TRUE);
	m_pSpfcUtil->SetImportantBridge(m_chkBridge.GetCheck());
	if(m_nEarthEff !=0 ) return;

	MyParser parser;
	CString sValue;
	m_edtG.GetWindowText(sValue);
	sValue.TrimLeft();
	if(!parser.ParsingNumberArg(sValue) || sValue == _T("")) return;

	double dCurG = m_edtG.GetEditValue();
	if(m_pSpfcUtil->GetImportantBridge() == TRUE)
		m_edtG.SetEditUnit(dCurG*1.4);
	else
		m_edtG.SetEditUnit(dCurG/1.4);
}

void CETC_GDS_GB50111_2006::OnCalculateG()
{
	UpdateData(TRUE);
	
	m_pSpfcUtil->SetImportantBridge(m_chkBridge.GetCheck()); // 클릭시마다 이 변수에 저장

	double dG;
	switch(m_nSFI)
	{
		case 0:
			if(m_nEarthEff == 0) dG = 0.02;
			else if(m_nEarthEff == 1) dG = 0.05;
			else if(m_nEarthEff == 2) dG = 0.11;
		break;
		case 1:
			if(m_nEarthEff == 0) dG = 0.04;
			else if(m_nEarthEff == 1) dG = 0.10;
			else if(m_nEarthEff == 2) dG = 0.21;
		break;
		case 2:
			if(m_nEarthEff == 0) dG = 0.05;
			else if(m_nEarthEff == 1) dG = 0.15;
			else if(m_nEarthEff == 2) dG = 0.32;
		break;
		case 3:
			if(m_nEarthEff == 0) dG = 0.07;
			else if(m_nEarthEff == 1) dG = 0.20;
			else if(m_nEarthEff == 2) dG = 0.38;
		break;
		case 4:
			if(m_nEarthEff == 0) dG = 0.10;
			else if(m_nEarthEff == 1) dG = 0.30;
			else if(m_nEarthEff == 2) dG = 0.57;
		break;
		case 5:
			if(m_nEarthEff == 0) dG = 0.14;
			else if(m_nEarthEff == 1) dG = 0.40;
			else if(m_nEarthEff == 2) dG = 0.64;
		break;
	}

	BOOL bImpBr = (m_chkBridge.GetCheck()==1); // important 이고 freequent 인 경우 1.4 곱함
	if(m_nEarthEff==0 && (bImpBr))
		dG *= 1.4;

	m_edtG.SetEditUnit(dG);
}

BOOL CETC_GDS_GB50111_2006::ISVALID()
{
	if(!IsValid_d(&m_edtTg)) return FALSE;
	if(!IsValid_d(&m_edtG)) return FALSE;

	if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  
	//if(m_dXi < 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__DAMPING_VALUE)); return FALSE;}
	return TRUE;  
}

BOOL CETC_GDS_GB50111_2006::IsValid_d(CEdit* pEdt)
{
	MyParser parser;
	CString sValue,msg;
	double dValue;
	double dDefValue;
	double dMin;
	if(pEdt==&m_edtTg) {dDefValue=0.25;dMin=0.;}
	if(pEdt==&m_edtG) {dDefValue=0.02;dMin=0.;}
	
	if(IsWindow(pEdt->m_hWnd))
	{
		pEdt->GetWindowText(sValue);
		sValue.TrimLeft();
		if(parser.ParsingNumberArg(sValue) && sValue != _T(""))
		{
			parser.GetNumberData(dValue);
			if(dValue < dMin )
			{
				//sValue.Format(_T("%f"),dMin);
				//pEdt->SetWindowText(sValue);
				((CEditUnit*)pEdt)->SetEditUnit(dDefValue);
				pEdt->SetFocus();
				pEdt->SetSel(0,-1);
				m_pWndError = pEdt;
				msg.Format(_LS(IDS_WG_CMD__ADDD__the_Value_must_be_greater_than__4),dMin);
				MessageBox(msg);
				return FALSE;
			}
			else
				return TRUE;
		}
		else
		{ 
			//sValue.Format(_T("%f"),dDefValue);
			//pEdt->SetWindowText(sValue);
			((CEditUnit*)pEdt)->SetEditUnit(dDefValue);
			pEdt->SetFocus();
			pEdt->SetSel(0,-1);
			m_pWndError = pEdt;
			MessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_Input));
			return FALSE;
		}
	}
	return TRUE;
}
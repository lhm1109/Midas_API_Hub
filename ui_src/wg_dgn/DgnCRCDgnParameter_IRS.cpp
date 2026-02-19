// DgnCRCDgnParameter_IRS.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCRCDgnParameter_IRS.h"
//
#include "DgnCRCAnnexDgnParameter_IRS_Dlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnCRCDgnParameter_IRS dialog


CDgnCRCDgnParameter_IRS::CDgnCRCDgnParameter_IRS(T_PSCD_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CDgnCRCDgnParameter_IRS::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnCRCDgnParameter_IRS)
	//}}AFX_DATA_INIT
	m_nPSType = 0;
	
	m_aSegment.Add(IDC_AASHTO_DESIGN_PARAMETER17);
	m_aSegment.Add(IDC_AASHTO_DESIGN_PARAMETER18);

	m_pParamData = pParamData;
}

CDgnCRCDgnParameter_IRS::~CDgnCRCDgnParameter_IRS()
{
}

void CDgnCRCDgnParameter_IRS::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCRCDgnParameter_IRS)  
	DDX_Control(pDX, IDC_EC2PSC_PRINT_PARAMETER2,   m_ChkOutputOpt_ULS1);
	DDX_Control(pDX, IDC_EC2PSC_PRINT_PARAMETER3,   m_ChkOutputOpt_ULS2);
	DDX_Control(pDX, IDC_EC2PSC_PRINT_PARAMETER4,   m_ChkOutputOpt_ULS3);
	
	DDX_Control(pDX, IDC_EC2PSC_PRINT_PARAMETER6,   m_ChkOutputOpt_SLS1);
	DDX_Control(pDX, IDC_EC2PSC_PRINT_PARAMETER7,   m_ChkOutputOpt_SLS2);
	DDX_Control(pDX, IDC_EC2PSC_PRINT_PARAMETER8,   m_ChkOutputOpt_SLS3);
	DDX_Control(pDX, IDC_EC2PSC_PRINT_PARAMETER9,   m_ChkOutputOpt_SLS4);
	DDX_Control(pDX, IDC_EC2PSC_PRINT_PARAMETER10,  m_ChkOutputOpt_SLS5);
	DDX_Control(pDX, IDC_EC2PSC_PRINT_PARAMETER11,  m_ChkOutputOpt_SLS6);
	//
	//DDX_Control(pDX, IDC_EC2PSC_DESIGN_PARAMETER12, m_CmbExposureClass);
	//DDX_Control(pDX, IDC_EC2PSC_DESIGN_PARAMETER16, m_CmbCementClass);
	//
	DDX_Control(pDX, IDC_EC2PSC_DESIGN_PARAMETER3,  m_RadioTndCalcMethod);
	DDX_Control(pDX, IDC_EC2PSC_DESIGN_PARAMETER4,  m_RadioTndCalcMethod2);
// 	DDX_Control(pDX, IDC_EC2PSC_DESIGN_PARAMETER6,  m_RadioShearMethod);
// 	DDX_Control(pDX, IDC_EC2PSC_DESIGN_PARAMETER7,  m_RadioShearMethod2);
// 	//
// 	DDX_Control(pDX, IDC_EC2PSC_DESIGN_PARAMETER9,  m_dStrutAngle);
	//
	DDX_Control(pDX, IDC_EC2PSC_DESIGN_PARAMETER14, m_ChkUserInputData);
	
	DDX_Radio  (pDX, IDC_EC2PSC_DESIGN_PARAMETER_PS_TYPE1_RDO, m_nPSType);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnCRCDgnParameter_IRS, CMyChildDialog)
	//{{AFX_MSG_MAP(CDgnCRCDgnParameter_IRS)
	ON_BN_CLICKED(IDC_EC2PSC_DESIGN_PARAMETER13,  OnClickModifyDgnParam)
	ON_BN_CLICKED(IDC_EC2PSC_PRINT_PARAMETER12,   OnClickSelectAllOutputOpt)
	ON_BN_CLICKED(IDC_EC2PSC_PRINT_PARAMETER13,   OnClickUnselectAllOutputOpt)
	ON_BN_CLICKED(IDC_EC2PSC_DESIGN_PARAMETER14,  OnCheckUserInputData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//------------------------------------------------------------------
// OnInitDialog
//------------------------------------------------------------------
BOOL CDgnCRCDgnParameter_IRS::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();

	// Set ExposureClass Combo.
	//InitExposureClassComboSet();
	// Set CementClass Combo
	//InitCementClassComboSet();

	// InitSetting!
	Initial_Data();

	// v835 에서 막고 나가자
	//GetDlgItem(IDC_EC2PSC_PRINT_PARAMETER14)->ShowWindow(SW_HIDE);	
		
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnCRCDgnParameter_IRS::Initial_Data()
{
	// Radio1 : ULS
	BOOL bFirst = FALSE;
	bFirst = m_pParamData->iTndnCalcMethod==0? TRUE : FALSE;
	m_RadioTndCalcMethod.SetCheck(bFirst);
	m_RadioTndCalcMethod2.SetCheck(!bFirst);

	// Radio2 : SLS
	bFirst = FALSE; bFirst = m_pParamData->iShearMethod==0? TRUE : FALSE;
// 	m_RadioShearMethod.SetCheck(bFirst);
// 	m_RadioShearMethod2.SetCheck(!bFirst);

	GetDlgItem(IDC_EC2PSC_DESIGN_PARAMETER6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EC2PSC_DESIGN_PARAMETER7)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_EC2PSC_DESIGN_PARAMETER11)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_EC2PSC_DESIGN_PARAMETER12)->ShowWindow(SW_HIDE);
	
	// Beta 계산
	//CDlgUtil::CtrlRadioSetCheck(this, m_aBetaCalType, m_pParamData->nBetaCalType); 

	// Edit : Strut Angle (Deg)
	m_dStrutAngle.SetUnitType(D_UNITSYS_NONE);

	// Default값 SET!!
	double dTheta = 0.0;
	dTheta = m_pParamData->dtheta;
	if(dTheta < 21.8 || dTheta > 45.0)  dTheta = 45.0;
	m_dStrutAngle.SetEditUnit(dTheta);  // Default 21.8 ~ 45.0 (Deg)

	// Combo : Exposure Class
	int iIndex = 0;
	iIndex = m_pParamData->iExposureClass;
	if(iIndex < 0 || iIndex > 18) iIndex = 0;
	//m_CmbExposureClass.SetCurSel(iIndex);

	// Cement Class를 추가하고, 변수는 기존에 있던 변수인 iCorrosiveCondition를 활용함.
	iIndex = 0; iIndex = m_pParamData->iCorrosiveCondition;
	if(iIndex < 0 || iIndex > 2)  iIndex = 0;
	//m_CmbCementClass.SetCurSel(iIndex);

	// CheckBox : Output Opt.
	m_ChkOutputOpt_ULS1.SetCheck(m_pParamData->bUltimateEuro[0]);
	m_ChkOutputOpt_ULS2.SetCheck(m_pParamData->bUltimateEuro[1]);
	m_ChkOutputOpt_ULS3.SetCheck(m_pParamData->bUltimateEuro[2]);
	//m_ChkOutputOpt_ULS4.SetCheck(m_pParamData->bUltimateEuro[3]);

	m_ChkOutputOpt_SLS1.SetCheck(m_pParamData->bServiceabilityEuro[0]);
	m_ChkOutputOpt_SLS2.SetCheck(m_pParamData->bServiceabilityEuro[1]);
	m_ChkOutputOpt_SLS3.SetCheck(m_pParamData->bServiceabilityEuro[2]);
	m_ChkOutputOpt_SLS4.SetCheck(m_pParamData->bServiceabilityEuro[3]);
	m_ChkOutputOpt_SLS5.SetCheck(m_pParamData->bServiceabilityEuro[4]);
	m_ChkOutputOpt_SLS6.SetCheck(m_pParamData->bServiceabilityEuro[5]);
	//
	m_ChkUserInputData.SetCheck(m_pParamData->bUserInputData);
	//
	BOOL bChk = FALSE; bChk = m_pParamData->bUserInputData;
	GetDlgItem(IDC_EC2PSC_DESIGN_PARAMETER13)->EnableWindow(bChk);  // bUserInput인 경우 버튼을 활성화함.

	m_nPSType = m_pParamData->nPreStressStl;

	CDBDoc*	pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	BOOL bPscComposite = pDoc->m_pPostCtrl->IsPSCCompositeSect();	
	/*if(!bPscComposite)
	{
		GetDlgItem(IDC_EC2PSC_BETA_STC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EC2PSC_BETA_PARAMETER1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EC2PSC_BETA_PARAMETER2)->ShowWindow(SW_HIDE);
	}*/
	CDlgUtil::CtrlRadioSetCheck(this, m_aSegment           , m_pParamData->iConstructionType);

	UpdateData(FALSE);
}

BOOL CDgnCRCDgnParameter_IRS::Dlg2Data()
{
	UpdateData(TRUE);

	m_pParamData->iTndnCalcMethod         = m_RadioTndCalcMethod.GetCheck()? 0 : 1;
	//m_pParamData->iShearMethod            = m_RadioShearMethod.GetCheck()? 0 : 1;
	m_pParamData->dtheta                  = m_dStrutAngle.GetEditValue();
	//m_pParamData->iExposureClass          = m_CmbExposureClass.GetCurSel();
	// Cement Class를 추가하고, 변수는 기존에 있던 변수인 iCorrosiveCondition를 활용함.
	//m_pParamData->iCorrosiveCondition     = m_CmbCementClass.GetCurSel();
	m_pParamData->bUltimateEuro[0]        = m_ChkOutputOpt_ULS1.GetCheck();
	m_pParamData->bUltimateEuro[1]        = m_ChkOutputOpt_ULS2.GetCheck();
	m_pParamData->bUltimateEuro[2]        = m_ChkOutputOpt_ULS3.GetCheck();
	//m_pParamData->bUltimateEuro[3]        = m_ChkOutputOpt_ULS4.GetCheck();
	m_pParamData->bServiceabilityEuro[0]  = m_ChkOutputOpt_SLS1.GetCheck();
	m_pParamData->bServiceabilityEuro[1]  = m_ChkOutputOpt_SLS2.GetCheck();
	m_pParamData->bServiceabilityEuro[2]  = m_ChkOutputOpt_SLS3.GetCheck();
	m_pParamData->bServiceabilityEuro[3]  = m_ChkOutputOpt_SLS4.GetCheck();
	m_pParamData->bServiceabilityEuro[4]  = m_ChkOutputOpt_SLS5.GetCheck();
	m_pParamData->bServiceabilityEuro[5]  = m_ChkOutputOpt_SLS6.GetCheck();
	//
	m_pParamData->bUserInputData          = m_ChkUserInputData.GetCheck();

	m_pParamData->nPreStressStl = m_nPSType;

	//CDlgUtil::CtrlRadioGetCheck(this, m_aBetaCalType  , m_pParamData->nBetaCalType); 
	CDlgUtil::CtrlRadioGetCheck(this, m_aSegment      , m_pParamData->iConstructionType);

	return TRUE;
}

// Modify Annex DgnParam.
void CDgnCRCDgnParameter_IRS::OnClickModifyDgnParam()
{
	CDgnCRCAnnexDgnParameter_IRS_Dlg dlg;
	dlg.m_pParamData  = m_pParamData;
	dlg.m_nAnnex      = m_nAnnex;
	if(dlg.DoModal()==IDOK)
	{
		m_pParamData = dlg.m_pParamData;
	}
}

// Select All Output Opt.
void CDgnCRCDgnParameter_IRS::OnClickSelectAllOutputOpt()
{
	UpdateData(TRUE);
	// SET TRUE
	m_ChkOutputOpt_ULS1.SetCheck(TRUE);
	m_ChkOutputOpt_ULS2.SetCheck(TRUE);
	m_ChkOutputOpt_ULS3.SetCheck(TRUE);
	//m_ChkOutputOpt_ULS4.SetCheck(TRUE);
	//
	m_ChkOutputOpt_SLS1.SetCheck(TRUE);
	m_ChkOutputOpt_SLS2.SetCheck(TRUE);
	m_ChkOutputOpt_SLS3.SetCheck(TRUE);
	m_ChkOutputOpt_SLS4.SetCheck(TRUE);
	m_ChkOutputOpt_SLS5.SetCheck(TRUE);
	m_ChkOutputOpt_SLS6.SetCheck(TRUE);

	UpdateData(FALSE);
}

// Unselect All Output Opt.
void CDgnCRCDgnParameter_IRS::OnClickUnselectAllOutputOpt()
{
	UpdateData(TRUE);

	// SET TRUE
	m_ChkOutputOpt_ULS1.SetCheck(FALSE);
	m_ChkOutputOpt_ULS2.SetCheck(FALSE);
	m_ChkOutputOpt_ULS3.SetCheck(FALSE);
	//m_ChkOutputOpt_ULS4.SetCheck(FALSE);
	//
	m_ChkOutputOpt_SLS1.SetCheck(FALSE);
	m_ChkOutputOpt_SLS2.SetCheck(FALSE);
	m_ChkOutputOpt_SLS3.SetCheck(FALSE);
	m_ChkOutputOpt_SLS4.SetCheck(FALSE);
	m_ChkOutputOpt_SLS5.SetCheck(FALSE);
	m_ChkOutputOpt_SLS6.SetCheck(FALSE);
	
	UpdateData(FALSE);
}

void CDgnCRCDgnParameter_IRS::OnCheckUserInputData()
{
	BOOL bChk = FALSE;
	bChk = m_ChkUserInputData.GetCheck();

	GetDlgItem(IDC_EC2PSC_DESIGN_PARAMETER13)->EnableWindow(bChk);
}

//void CDgnCRCDgnParameter_IRS::InitExposureClassComboSet()
//{
//	int iCountCombo = 0;
//	iCountCombo = m_CmbExposureClass.GetCount();
//	if(iCountCombo > 0) m_CmbExposureClass.ResetContent();
//	int iIndex = 0;
//	if (m_pParamData->iDgnCode==IRC_112_2011_PSC)
//	{
//		CString asrtEC2ExpList[4] = {_T("Moderate"),_T("Severe"),_T("Very Severe"),_T("Extreme")};
//		int i=0;
//		for(i=0; i<4; ++i)
//		{
//			m_CmbExposureClass.AddString(asrtEC2ExpList[i]);
//		}
//
//		// Data가 있다면..Data에 의해서 최초 설정되도록 이후 Code추가!!
//		iIndex = m_pParamData->iExposureClass;
//		if(iIndex < 0 || iIndex > 4) iIndex = 0;
//
//	}
//	else
//	{
//		CString asrtEC2ExpList[18] = {_T("X0"),_T("XC1"),_T("XC2"),_T("XC3"),_T("XC4"),_T("XD1"),_T("XD2"),_T("XD3"),_T("XS1"),_T("XS2"),_T("XS3"),
//			_T("XF1"),_T("XF2"),_T("XF3"),_T("XF4"),_T("XA1"),_T("XA2"),_T("XA3")};
//		int i=0;
//		for(i=0; i<18; ++i)
//		{
//			m_CmbExposureClass.AddString(asrtEC2ExpList[i]);
//		}
//
//		// Data가 있다면..Data에 의해서 최초 설정되도록 이후 Code추가!!
//		iIndex = m_pParamData->iExposureClass;
//		if(iIndex < 0 || iIndex > 18) iIndex = 0;
//	}
//
//
//	m_CmbExposureClass.SetCurSel(iIndex);
//}

void CDgnCRCDgnParameter_IRS::InitCementClassComboSet()
{
	/*int iCountCombo = 0;
	iCountCombo = m_CmbCementClass.GetCount();
	if(iCountCombo > 0) m_CmbCementClass.ResetContent();

	CString asrtEC2ExpList[3] = {_T("Class R (s=0.20)"),_T("Class N (s=0.25)"),_T("Class S (s=0.38)")};
	int i=0;
	for(i=0; i<3; ++i)
	{
		m_CmbCementClass.AddString(asrtEC2ExpList[i]);
	}

	 Data가 있다면..Data에 의해서 최초 설정되도록 이후 Code추가!!
	int iIndex = 0; iIndex =m_pParamData->iCorrosiveCondition;
	if(iIndex < 0 || iIndex > 2)  iIndex = 0;
	m_CmbCementClass.SetCurSel(iIndex);*/
}
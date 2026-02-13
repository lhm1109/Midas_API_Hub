// [Check before Release...!!] KSPARK-20130516 : // DgnCRCDgnParameter_LSDPSC.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "DgnCRCDgnParameter_LSDPSC.h"
//
#include "DgnCRCAnnexDgnParameterDlg_LSD.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnCRCDgnParameter_LSDPSC dialog


CDgnCRCDgnParameter_LSDPSC::CDgnCRCDgnParameter_LSDPSC(T_PSCD_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CDgnCRCDgnParameter_LSDPSC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnCRCDgnParameter_LSDPSC)
	//}}AFX_DATA_INIT
	m_pParamData = pParamData;

//   m_aTndCalcMethod.Add(IDC_LSDPSC_DESIGN_PARAMETER3);
//   m_aTndCalcMethod.Add(IDC_LSDPSC_DESIGN_PARAMETER4);

	m_aTndType.Add(IDC_LSDPSC_DESIGN_PARAMETER10);
	m_aTndType.Add(IDC_LSDPSC_DESIGN_PARAMETER11);
	m_aTndType.Add(IDC_LSDPSC_DESIGN_PARAMETER12);
	m_aTndType.Add(IDC_LSDPSC_DESIGN_PARAMETER13);

	m_aBetaCalType.Add(IDC_LSDPSC_BETA_PARAMETER1);
	m_aBetaCalType.Add(IDC_LSDPSC_BETA_PARAMETER2);

	m_aConCalType.Add(IDC_LSDPSC_CONCALC_PARAMETER1);
	m_aConCalType.Add(IDC_LSDPSC_CONCALC_PARAMETER2);

}

CDgnCRCDgnParameter_LSDPSC::~CDgnCRCDgnParameter_LSDPSC()
{
}

void CDgnCRCDgnParameter_LSDPSC::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCRCDgnParameter_LSDPSC)  
	DDX_Control(pDX, IDC_LSDPSC_PRINT_PARAMETER2,   m_ChkOutputOpt_ULS1);
	DDX_Control(pDX, IDC_LSDPSC_PRINT_PARAMETER3,   m_ChkOutputOpt_ULS2);
	DDX_Control(pDX, IDC_LSDPSC_PRINT_PARAMETER4,   m_ChkOutputOpt_ULS3);
	DDX_Control(pDX, IDC_LSDPSC_PRINT_PARAMETER6,   m_ChkOutputOpt_SLS1);
	DDX_Control(pDX, IDC_LSDPSC_PRINT_PARAMETER7,   m_ChkOutputOpt_SLS2);
	DDX_Control(pDX, IDC_LSDPSC_PRINT_PARAMETER8,   m_ChkOutputOpt_SLS3);
	DDX_Control(pDX, IDC_LSDPSC_PRINT_PARAMETER9,   m_ChkOutputOpt_SLS4);
	DDX_Control(pDX, IDC_LSDPSC_PRINT_PARAMETER10,  m_ChkOutputOpt_SLS5);
	DDX_Control(pDX, IDC_LSDPSC_PRINT_PARAMETER11,  m_ChkOutputOpt_SLS6);
	//
	DDX_Control(pDX, IDC_LSDPSC_DESIGN_PARAMETER15, m_CmbCementClass);
	//
	DDX_Control(pDX, IDC_LSDPSC_DESIGN_PARAMETER16, m_ChkUserInputData);
	//
	DDX_Control(pDX, IDC_LSDPSC_DESIGN_PARAMETER7,  m_dStrutAngle);
	//
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnCRCDgnParameter_LSDPSC, CMyChildDialog)
	//{{AFX_MSG_MAP(CDgnCRCDgnParameter_LSDPSC)
	ON_BN_CLICKED(IDC_LSDPSC_DESIGN_PARAMETER16,  OnCheckUserInputData)
	ON_BN_CLICKED(IDC_LSDPSC_DESIGN_PARAMETER17,  OnClickModifyDgnParam)

	ON_BN_CLICKED(IDC_LSDPSC_PRINT_PARAMETER12,   OnClickSelectAllOutputOpt)
	ON_BN_CLICKED(IDC_LSDPSC_PRINT_PARAMETER13,   OnClickUnselectAllOutputOpt)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//------------------------------------------------------------------
// OnInitDialog
//------------------------------------------------------------------
BOOL CDgnCRCDgnParameter_LSDPSC::OnInitDialog() 
{
	frx::MInitCombo initCombo;

	CMyChildDialog::OnInitDialog();

	InitCementClassComboSet();
	Initial_Data();
	return TRUE;  
}

void CDgnCRCDgnParameter_LSDPSC::Initial_Data()
{
	// 휨강도
//   CDlgUtil::CtrlRadioSetCheck(this, m_aTndCalcMethod, m_pParamData->iTndnCalcMethod); 
	// PS 강재종류
	CDlgUtil::CtrlRadioSetCheck(this, m_aTndType, m_pParamData->iTndnKind); 
	
	// Beta 계산
	CDlgUtil::CtrlRadioSetCheck(this, m_aBetaCalType, m_pParamData->nBetaCalType); 

	// Calc Type 계산
	CDlgUtil::CtrlRadioSetCheck(this, m_aConCalType, m_pParamData->nCalcCcType); 

	
	// 이정우 과장 요청 2014.03.12
	// m_pParamData->dtheta = 29.75; // cot theta = 1.75
	// => CMainFrame::AddPSCDesignCode() 에서 수정됨.
 
	// 전단강도 Edit : Strut Angle (Deg)
	m_dStrutAngle.SetUnitType(D_UNITSYS_NONE);  
	double dTheta = m_pParamData->dtheta;
	if(dTheta < 21.8 || dTheta > 45.0)  dTheta = 45.0;
	m_dStrutAngle.SetEditUnit(dTheta);  // Default 21.8 ~ 45.0 (Deg)

	// 시멘트구분 : iCorrosiveCondition를 활용함.
	int iIndex = m_pParamData->iCorrosiveCondition;
	//if(iIndex < 0 || iIndex > 2)  iIndex = 0;
	m_CmbCementClass.SetCurSel(iIndex);

	// CheckBox : Output Opt.
	m_ChkOutputOpt_ULS1.SetCheck(m_pParamData->bUltimateEuro[0]);
	m_ChkOutputOpt_ULS2.SetCheck(m_pParamData->bUltimateEuro[1]);
	m_ChkOutputOpt_ULS3.SetCheck(m_pParamData->bUltimateEuro[2]);
	m_ChkOutputOpt_SLS1.SetCheck(m_pParamData->bServiceabilityEuro[0]);
	m_ChkOutputOpt_SLS2.SetCheck(m_pParamData->bServiceabilityEuro[1]);
	m_ChkOutputOpt_SLS3.SetCheck(m_pParamData->bServiceabilityEuro[2]);
	m_ChkOutputOpt_SLS4.SetCheck(m_pParamData->bServiceabilityEuro[3]);
	m_ChkOutputOpt_SLS5.SetCheck(m_pParamData->bServiceabilityEuro[4]);
	m_ChkOutputOpt_SLS6.SetCheck(m_pParamData->bServiceabilityEuro[5]);
	
	// 사용자 입력
	m_ChkUserInputData.SetCheck(m_pParamData->bUserInputData);
	BOOL bChk = m_pParamData->bUserInputData;
	GetDlgItem(IDC_LSDPSC_DESIGN_PARAMETER17)->EnableWindow(bChk);  // bUserInput인 경우 버튼을 활성화함.

	CDBDoc*	pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	BOOL bPscComposite = pDoc->m_pPostCtrl->IsPSCCompositeSect();	
	if(!bPscComposite)
	{
		GetDlgItem(IDC_LSDPSC_BETA_STC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LSDPSC_BETA_PARAMETER1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LSDPSC_BETA_PARAMETER2)->ShowWindow(SW_HIDE);
	}

}

BOOL CDgnCRCDgnParameter_LSDPSC::Dlg2Data()
{

//   CDlgUtil::CtrlRadioSetCheck(this, m_aTndCalcMethod, m_pParamData->iTndnCalcMethod); 
	CDlgUtil::CtrlRadioGetCheck(this, m_aTndType      , m_pParamData->iTndnKind); 

	CDlgUtil::CtrlRadioGetCheck(this, m_aBetaCalType  , m_pParamData->nBetaCalType); 

	CDlgUtil::CtrlRadioGetCheck(this, m_aConCalType, m_pParamData->nCalcCcType); 


	m_pParamData->dtheta                  = m_dStrutAngle.GetEditValue();

	m_pParamData->iCorrosiveCondition     = m_CmbCementClass.GetCurSel();

	m_pParamData->bUltimateEuro[0]        = m_ChkOutputOpt_ULS1.GetCheck();
	m_pParamData->bUltimateEuro[1]        = m_ChkOutputOpt_ULS2.GetCheck();
	m_pParamData->bUltimateEuro[2]        = m_ChkOutputOpt_ULS3.GetCheck();
	m_pParamData->bServiceabilityEuro[0]  = m_ChkOutputOpt_SLS1.GetCheck();
	m_pParamData->bServiceabilityEuro[1]  = m_ChkOutputOpt_SLS2.GetCheck();
	m_pParamData->bServiceabilityEuro[2]  = m_ChkOutputOpt_SLS3.GetCheck();
	m_pParamData->bServiceabilityEuro[3]  = m_ChkOutputOpt_SLS4.GetCheck();
	m_pParamData->bServiceabilityEuro[4]  = m_ChkOutputOpt_SLS5.GetCheck();
	m_pParamData->bServiceabilityEuro[5]  = m_ChkOutputOpt_SLS6.GetCheck();
	
	m_pParamData->bUserInputData          = m_ChkUserInputData.GetCheck();

	return TRUE;
}

// Modify Annex DgnParam.
void CDgnCRCDgnParameter_LSDPSC::OnClickModifyDgnParam()
{
	CDgnCRCAnnexDgnParameterDlg_LSD dlg;
	dlg.m_pParamData  = m_pParamData;
	if(dlg.DoModal()==IDOK)
	{
		m_pParamData = dlg.m_pParamData;
	}
}

// Select All Output Opt.
void CDgnCRCDgnParameter_LSDPSC::OnClickSelectAllOutputOpt()
{
	UpdateData(TRUE);
	// SET TRUE
	m_ChkOutputOpt_ULS1.SetCheck(TRUE);
	m_ChkOutputOpt_ULS2.SetCheck(TRUE);
	m_ChkOutputOpt_ULS3.SetCheck(TRUE);
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
void CDgnCRCDgnParameter_LSDPSC::OnClickUnselectAllOutputOpt()
{
	UpdateData(TRUE);

	// SET TRUE
	m_ChkOutputOpt_ULS1.SetCheck(FALSE);
	m_ChkOutputOpt_ULS2.SetCheck(FALSE);
	m_ChkOutputOpt_ULS3.SetCheck(FALSE);
	//
	m_ChkOutputOpt_SLS1.SetCheck(FALSE);
	m_ChkOutputOpt_SLS2.SetCheck(FALSE);
	m_ChkOutputOpt_SLS3.SetCheck(FALSE);
	m_ChkOutputOpt_SLS4.SetCheck(FALSE);
	m_ChkOutputOpt_SLS5.SetCheck(FALSE);
	m_ChkOutputOpt_SLS6.SetCheck(FALSE);
	
	UpdateData(FALSE);
}

void CDgnCRCDgnParameter_LSDPSC::OnCheckUserInputData()
{
	BOOL bChk = FALSE;
	bChk = m_ChkUserInputData.GetCheck();

	GetDlgItem(IDC_LSDPSC_DESIGN_PARAMETER17)->EnableWindow(bChk);
}


void CDgnCRCDgnParameter_LSDPSC::InitCementClassComboSet()
{
	int iCountCombo = m_CmbCementClass.GetCount();
	if(iCountCombo > 0) m_CmbCementClass.ResetContent();

	CString asrtEC2ExpList[5] = 
	{ _LS(IDS_DGN_CRC_LSDPSC_CEMENT1),
	_LS(IDS_DGN_CRC_LSDPSC_CEMENT2),
	_LS(IDS_DGN_CRC_LSDPSC_CEMENT3),
	_LS(IDS_DGN_CRC_LSDPSC_CEMENT4),
	_LS(IDS_DGN_CRC_LSDPSC_CEMENT5)
	};
	int i=0;
	for(i=0; i<5; ++i)
	{
		m_CmbCementClass.AddString(asrtEC2ExpList[i]);
	}

	// Data가 있다면..Data에 의해서 최초 설정되도록 이후 Code추가!!
	int iIndex = m_pParamData->iCorrosiveCondition;
	//if(iIndex < 0 || iIndex > 4)  iIndex = 0;
	m_CmbCementClass.SetCurSel(iIndex);
}
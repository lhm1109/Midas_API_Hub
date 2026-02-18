// RatingParamLSD15Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "RatingParamPSCLSD15Dlg.h"
#include "RatingParamPSCLSD15Annex.h"
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\CheckDialogOpen.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_LoadDB.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRatingParamPSCLSD15Dlg dialog
CRatingParamPSCLSD15Dlg::CRatingParamPSCLSD15Dlg(CWnd* pParent )
	: CDialogMove(CRatingParamPSCLSD15Dlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_Data.Initialize();
#ifdef _DEBUG
	T_RACD_D RacdD; RacdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRacd(RacdD);
	if(RacdD.nRatingCode != KSCE_LSD15_RATING)
	{
		ASSERT(0);
	}
#endif

	// KSCE_USD10_RATING와 KSCE_RAIL_USD11_RATING일 경우만 Init을 다르게 하는 처리..
	//ASSERT(m_Data.strDgnLiveLoad == _T("KL"));
	//m_Data.dDgnLiveLoad = 510.0;
	//m_Data.bRefRebar = TRUE;	

	//{{AFX_DATA_INIT(CRatingParamPSCLSD15Dlg)	
	//}}AFX_DATA_INIT
}


CRatingParamPSCLSD15Dlg::~CRatingParamPSCLSD15Dlg()
{

}


void CRatingParamPSCLSD15Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRatingParamPSCLSD15Dlg)
	DDX_Control(pDX, IDC_RDO_DEFLECTION,											m_ratioDeflection);
	DDX_Control(pDX, IDC_RDO_STRAIN,													m_ratioStrain);
	DDX_Control(pDX, IDC_DGN_LIVE_LOAD_EDIT,									m_editLiveLoad);
	DDX_Control(pDX, IDC_DGN_LIVE_LOAD_LVAL_EDIT,							m_editLiveLoadLVal);
	DDX_Control(pDX, IDC_DGN_LSDPSC_RATING_PS_TYPE_RADIO1,		m_ratioPSType1);
	DDX_Control(pDX, IDC_DGN_LSDPSC_RATING_PS_TYPE_RADIO2,		m_ratioPSType2);
	DDX_Control(pDX, IDC_DGN_LSDPSC_RATING_PS_TYPE_RADIO3,		m_ratioPSType3);	
	DDX_Control(pDX, IDC_DGN_LSDPSC_RATING_PS_TYPE_RADIO4,		m_ratioPSType4);
	DDX_Control(pDX, IDC_DGN_LSDPSC_RATING_CONCALC_RADIO1,		m_ratioFlexureType1);
	DDX_Control(pDX, IDC_DGN_LSDPSC_RATING_CONCALC_RADIO2,		m_ratioFlexureType2);
	DDX_Control(pDX, IDC_DGN_LSDPSC_RATING_DESIGN_USER_CHECK,	m_chkUserInput);
	DDX_Control(pDX, IDC_DGN_METHOD_CHECK1,										m_chkDgnOutput1);
	DDX_Control(pDX, IDC_DGN_METHOD_CHECK2,										m_chkDgnOutput2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRatingParamPSCLSD15Dlg, CDialogMove)
	//{{AFX_MSG_MAP(CRatingParamPSCLSD15Dlg)
	ON_BN_CLICKED(IDC_DGN_LSDPSC_RATING_DESIGN_USER_CHECK,  OnCheckUserInputData)
	ON_BN_CLICKED(IDC_DGNLSDPSC_RATING_DESIGN_MDP_BTN, OnClickModDgnParam)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_SEL_ALL_BTN, OnDgnSelectAll)
	ON_BN_CLICKED(IDC_UNSEL_ALL_BTN, OnDgnUnSelectAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CRatingParamPSCLSD15Dlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	CCheckDialogOpen::SetSoftwareRendering();

	InitUnit();

	// 사용자 입력
	//m_chkUserInput.SetCheck(m_Data.bUserInputData);
	//BOOL bChk = m_Data.bUserInputData;
	//GetDlgItem(IDC_DGNLSDPSC_RATING_DESIGN_MDP_BTN)->EnableWindow(bChk);  // bUserInput인 경우 버튼을 활성화함.
	OnCheckUserInputData();

	if(m_pDoc->m_pAttrCtrl2->ExistRakr())
	{
		m_pDoc->m_pAttrCtrl2->GetRakr(m_Data);
	}


	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CRatingParamPSCLSD15Dlg::InitUnit()
{
	m_editLiveLoad.SetUnitType(D_UNITSYS_NONE);
	m_Data.strDgnLiveLoad = _T("KL");
	m_Data.dDgnLiveLoad = 510.0;
}

void CRatingParamPSCLSD15Dlg::Data2Dlg()
{
	if(m_Data.nSurveyMeth == 0)//처짐
	{
		m_ratioDeflection.SetCheck(TRUE);
		m_ratioStrain.SetCheck(FALSE);
	}
	else if(m_Data.nSurveyMeth == 1)//변형률
	{
		m_ratioDeflection.SetCheck(FALSE);
		m_ratioStrain.SetCheck(TRUE);
	}
	else
		ASSERT(0);

	if(m_Data.iTndnKindLSD == 0)//원형강봉과 강선
	{
		m_ratioPSType1.SetCheck(TRUE);
		m_ratioPSType2.SetCheck(FALSE);
		m_ratioPSType3.SetCheck(FALSE);
		m_ratioPSType4.SetCheck(FALSE);
	}
	else if(m_Data.iTndnKindLSD == 1)//강연선
	{
		m_ratioPSType1.SetCheck(FALSE);
		m_ratioPSType2.SetCheck(TRUE);
		m_ratioPSType3.SetCheck(FALSE);
		m_ratioPSType4.SetCheck(FALSE);
	}
	else if(m_Data.iTndnKindLSD == 2)//이형 강선
	{
		m_ratioPSType1.SetCheck(FALSE);
		m_ratioPSType2.SetCheck(FALSE);
		m_ratioPSType3.SetCheck(TRUE);
		m_ratioPSType4.SetCheck(FALSE);
	}
	else if(m_Data.iTndnKindLSD == 3)//이형 강봉
	{
		m_ratioPSType1.SetCheck(FALSE);
		m_ratioPSType2.SetCheck(FALSE);
		m_ratioPSType3.SetCheck(FALSE);
		m_ratioPSType4.SetCheck(TRUE);
	}
	else
		ASSERT(0);

	if(m_Data.nCalcCcType == 0)//응력블럭
	{
		m_ratioFlexureType1.SetCheck(TRUE);
		m_ratioFlexureType2.SetCheck(FALSE);
	}
	else if(m_Data.nCalcCcType == 1)//Strain Parameter
	{
		m_ratioFlexureType1.SetCheck(FALSE);
		m_ratioFlexureType2.SetCheck(TRUE);
	}
	else
		ASSERT(0);

	//설계활하중

	m_editLiveLoad.SetEditUnit(m_Data.dDgnLiveLoad);
	m_editLiveLoadLVal.SetWindowText(m_Data.strDgnLiveLoad);

	if(m_Data.bUserInputData)//사용자 입력
	{
		m_chkUserInput.SetCheck(TRUE);
	}
	else
	{
		m_chkUserInput.SetCheck(FALSE);
		m_Data.Set_PSC_Rating_AnnexData_LSD();
	}
	GetDlgItem(IDC_DGNLSDPSC_RATING_DESIGN_MDP_BTN)->EnableWindow(m_Data.bUserInputData);

	if(m_Data.bPrintParm[0])//단면력 집계
		m_chkDgnOutput1.SetCheck(TRUE);
	else
		m_chkDgnOutput1.SetCheck(FALSE);

	if(m_Data.bPrintParm[1])//휨강도 검토
		m_chkDgnOutput2.SetCheck(TRUE);
	else
		m_chkDgnOutput2.SetCheck(FALSE);
}

void CRatingParamPSCLSD15Dlg::Dlg2Data()
{

	if(m_ratioDeflection.GetCheck())//처짐
		m_Data.nSurveyMeth = 0;
	else if(m_ratioStrain.GetCheck())//변형률
		m_Data.nSurveyMeth = 1;
	else
		ASSERT(0);

	if(m_ratioPSType1.GetCheck())			//원형강봉과 강선
		m_Data.iTndnKindLSD = 0;
	else if(m_ratioPSType2.GetCheck())//강연선
		m_Data.iTndnKindLSD = 1;
	else if(m_ratioPSType3.GetCheck())//이형 강선
		m_Data.iTndnKindLSD = 2;
	else if(m_ratioPSType4.GetCheck())//이형 강봉
		m_Data.iTndnKindLSD = 3;
	else
		ASSERT(0);

	if(m_ratioFlexureType1.GetCheck())//설계기준
		m_Data.nCalcCcType = 0;
	else if(m_ratioFlexureType2.GetCheck())//상세해석
		m_Data.nCalcCcType = 1;
	else
		ASSERT(0);

	////인장철근고려
	//m_Data.bRefRebar = m_chkTensionBar.GetCheck();

	//설계활하중
	m_Data.dDgnLiveLoad = m_editLiveLoad.GetEditValue();

	m_editLiveLoadLVal.GetWindowText(m_Data.strDgnLiveLoad);

	if(m_chkUserInput.GetCheck())//사용자
		m_Data.bUserInputData = TRUE;
	else if(!m_chkUserInput.GetCheck())//사용자
	{
		m_Data.bUserInputData = FALSE;
		m_Data.Set_PSC_Rating_AnnexData_LSD();
	}
	else
		ASSERT(0);

	m_Data.bPrintParm[0] = m_chkDgnOutput1.GetCheck();//단면력집계
	m_Data.bPrintParm[1] = m_chkDgnOutput2.GetCheck();//휨강도검토
}

void CRatingParamPSCLSD15Dlg::OnCheckUserInputData()
{
	BOOL bChk = FALSE;
	bChk = m_chkUserInput.GetCheck();

	GetDlgItem(IDC_DGNLSDPSC_RATING_DESIGN_MDP_BTN)->EnableWindow(bChk);

	m_Data.bUserInputData = bChk;
}

// Modify Annex DgnParam.
void CRatingParamPSCLSD15Dlg::OnClickModDgnParam()
{
	//T_PSCD_D pParamData;
	//pParamData.Initialize();
	CRatingParamPSCLSDAnnex dlg;
	dlg.m_Data  = m_Data;
	if(dlg.DoModal()==IDOK)
	{
		m_Data = dlg.m_Data;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CRatingParamPSCLSD15Dlg message handlers


void CRatingParamPSCLSD15Dlg::OnOk() 
{
	Dlg2Data();

	if(!m_pDoc->m_pDataCtrl->AddRakr(m_Data))
		return;

	CDialogMove::OnOK();		
}

void CRatingParamPSCLSD15Dlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	CDialogMove::OnCancel();
}

void CRatingParamPSCLSD15Dlg::OnDgnSelectAll() 
{
	m_chkDgnOutput1.SetCheck(TRUE);
	m_chkDgnOutput2.SetCheck(TRUE);
}

void CRatingParamPSCLSD15Dlg::OnDgnUnSelectAll() 
{
	m_chkDgnOutput1.SetCheck(FALSE);
	m_chkDgnOutput2.SetCheck(FALSE);
}
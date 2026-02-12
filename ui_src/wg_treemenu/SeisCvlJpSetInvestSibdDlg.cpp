// CSeisCvlJpSetInvestSibdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCvlJpSetInvestSibdDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COMBO_MOMENT_TYPE_My _LS(IDS_IDD_TM_SIBD_IDC_TM_SIBD_STRENGTH_SURVEY_CMB_My)
#define COMBO_MOMENT_TYPE_Mu _LS(IDS_IDD_TM_SIBD_IDC_TM_SIBD_STRENGTH_SURVEY_CMB_Mu)
#define COMBO_MOMENT_TYPE_Myd _LS(IDS_IDD_TM_SIBD_IDC_TM_SIBD_STRENGTH_SURVEY_CMB_Myd)
#define COMBO_MOMENT_TYPE_Mud _LS(IDS_IDD_TM_SIBD_IDC_TM_SIBD_STRENGTH_SURVEY_CMB_Mud)

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestSibdDlg dialog

CSeisCvlJpSetInvestSibdDlg::CSeisCvlJpSetInvestSibdDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisCvlJpSetInvestSibdDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nOption = 0;
	m_surveyType = 0;
	m_cvsv_surveyType = 0;
	m_allowCurvFormula = 0;

	m_aStaticCtrl.RemoveAll();
	m_aStaticCtrl.Add(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC1_STC);
	m_aStaticCtrl.Add(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC2_STC);
}

CSeisCvlJpSetInvestSibdDlg::~CSeisCvlJpSetInvestSibdDlg()
{
	m_pDoc = NULL;
}

void CSeisCvlJpSetInvestSibdDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisCvlEvalGrupDlg)
	DDX_Radio(pDX, IDC_TM_SIBD_ADD_RDO, m_nOption);
	DDX_Control(pDX, IDC_TM_SIBD_SIPA_CMB, m_caseName);
	DDX_Control(pDX, IDC_TM_SIBD_SIGR_CMB, m_groupName);
	DDX_Radio(pDX, IDC_TM_SIBD_STRENGTH_SURVEY_RDO, m_surveyType);
	DDX_Control(pDX, IDC_TM_SIBD_STRENGTH_SURVEY_CMB, m_momentType);
	DDX_Radio(pDX, IDC_TM_SIBD_SURRENDER_SURVEY_RDO, m_cvsv_surveyType);
	DDX_Control(pDX, IDC_TM_SIBD_SURRENDER_SURVEY_ALPHA_EDT, m_surrenderAlpha);
	DDX_Radio(pDX, IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC1_RDO, m_allowCurvFormula);
	DDX_Control(pDX, IDC_TM_SIBD_LIMITSTATE_ALPHA_EDT, m_limitStateAlpha);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSeisCvlJpSetInvestSibdDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CSeisCvlJpSetInvestSibdDlg)
	ON_CBN_SELCHANGE(IDC_TM_SIBD_SIPA_CMB, OnChangedCaseCmb)
	ON_CBN_SELCHANGE(IDC_TM_SIBD_SIGR_CMB, OnChangedGroupCmb)
	ON_BN_CLICKED(IDC_TM_SIBD_STRENGTH_SURVEY_RDO, OnChangedSurveyTypeRdo)
	ON_BN_CLICKED(IDC_TM_SIBD_CURVATURE_SURVEY_RDO, OnChangedSurveyTypeRdo)
	ON_BN_CLICKED(IDC_TM_SIBD_SURRENDER_SURVEY_RDO, OnChangedCvsvSurveyTypeRdo)
	ON_BN_CLICKED(IDC_TM_SIBD_LIMITSTATE_SURVEY_RDO, OnChangedCvsvSurveyTypeRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSeisCvlJpSetInvestSibdDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	MoveStaticCtrl();
	InitControl();
	InitCaseNameCtrl();
	InitGroupNameCtrl();
	UpdateSurveyTypeCtrl();
	UpdateCvsvSurveyType();
	UpdateRcMemeberCtrl();

	return TRUE;// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisCvlJpSetInvestSibdDlg::OnChangedCaseCmb()
{
	UpdateSurveyTypeCtrl();
	UpdateCvsvSurveyType();
	UpdateRcMemeberCtrl();
}

void CSeisCvlJpSetInvestSibdDlg::OnChangedGroupCmb()
{
	UpdateAllButtonCtrl();
}

void CSeisCvlJpSetInvestSibdDlg::OnChangedSurveyTypeRdo()
{
	UpdateAllButtonCtrl();
}

void CSeisCvlJpSetInvestSibdDlg::OnChangedCvsvSurveyTypeRdo()
{
	CString strGroup;
	m_groupName.GetWindowText(strGroup);

	T_SIGR_D dSigr;
	T_SIGR_K kSigr = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strGroup);

	BOOL bRC = FALSE;

	T_SIGR_LOWER_D dLower;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigr, dLower) == TRUE) {
		bRC = dLower.nMaterialType == T_SIGR_LOWER_D::kRC;
	}

	CButton* pBtn = ((CButton*)GetDlgItem(IDC_TM_SIBD_SURRENDER_SURVEY_RDO));
	BOOL bSurrenderSurvey = pBtn->GetCheck();

	pBtn = ((CButton*)GetDlgItem(IDC_TM_SIBD_LIMITSTATE_SURVEY_RDO));
	BOOL bLimitStateSurvey = pBtn->GetCheck();

	GetDlgItem(IDC_TM_SIBD_SURRENDER_SURVEY_ALPHA_EDT)->EnableWindow(bSurrenderSurvey);
	GetDlgItem(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC1_RDO)->EnableWindow(bLimitStateSurvey && bRC);
	GetDlgItem(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC2_RDO)->EnableWindow(bLimitStateSurvey && bRC);
	GetDlgItem(IDC_TM_SIBD_LIMITSTATE_ALPHA_EDT)->EnableWindow(bLimitStateSurvey && bRC);

	UpdateSurveyTypeCtrl();
	UpdateRcMemeberCtrl();
}

void CSeisCvlJpSetInvestSibdDlg::Execute()
{
	UpdateData(TRUE);

	if (m_nOption == 0)
		AddOrModData();
	else
		DelData();
}

void CSeisCvlJpSetInvestSibdDlg::MoveStaticCtrl()
{
	int nDistX = 0;
	CRect rTarget, rSource;

	GetDlgItem(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC1_RDO)->GetWindowRect(rSource);
	GetDlgItem(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC1_STC)->GetWindowRect(rTarget);

	nDistX = rSource.left - rTarget.left/* + globalUtils.ScaleByDPI(8)*/;
	CDlgUtil::CtrlMoveDistX(this, m_aStaticCtrl, nDistX);
}

void CSeisCvlJpSetInvestSibdDlg::InitControl()
{
	T_SIDC_D sidcD; sidcD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetQSidc()->Get(sidcD);

	CButton* pBtn = NULL;
	//옵션 추가/변경 삭제 라디오 버튼
	pBtn = (CButton*)GetDlgItem(IDC_TM_SIBD_ADD_RDO);
	pBtn->SetCheck(1);
	m_nOption = 0;
	//항복조사 허용값α
	m_surrenderAlpha.SetWindowText(_T("1.0"));
	//H14 일때 RC부재의 허용곡률 라디오 버튼 보이게
	//H29 일때 φls2d 계산식 보이게
	int nCmdShow = sidcD.nDesignCode != BrdgSE_CodeJP::JROAD5_H29 ? SW_SHOW : SW_HIDE;
	GetDlgItem(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC1_RDO)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC2_RDO)->ShowWindow(nCmdShow);
	nCmdShow = sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H29 ? SW_SHOW : SW_HIDE;
	GetDlgItem(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC1_STC)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC2_STC)->ShowWindow(nCmdShow);

	CString str = sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H29 ?
		_LS(IDS_IDD_TM_SIBD_IDC_TM_SIBD_LIMITSTATE_SURVEY_FOMULA_STC) :
		_LS(IDS_IDD_TM_SIBD_IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC_STC);
	GetDlgItem(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC_STC)->SetWindowText(str);
}

void CSeisCvlJpSetInvestSibdDlg::InitCaseNameCtrl()
{
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->GetCount() < 1)
		return;

	T_SIDC_D sidcD; sidcD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetQSidc()->Get(sidcD);

	m_caseName.ResetContent();

	CArray<T_SIPA_K, T_SIPA_K> keys;
	m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKeyList(keys);
	for (int i = 0; i < keys.GetCount(); i++)
	{
		T_SIPA_K& key = keys[i];
		T_SIPA_D data;
		m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(key, data);

		if (sidcD.nDesignCode != data.nDesignCode)
			continue;

		m_caseName.AddString(data.strCaseName);
	}
	m_caseName.SetCurSel(0);
}

void CSeisCvlJpSetInvestSibdDlg::InitGroupNameCtrl()
{
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->GetCount() < 1)
		return;

	m_groupName.ResetContent();

	CArray<T_SIGR_K, T_SIGR_K> keys;
	m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKeyList(keys);
	for (int i = 0; i < keys.GetCount(); i++)
	{
		T_SIGR_K& key = keys[i];
		T_SIGR_D data;
		m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(key, data);
		m_groupName.AddString(data.strName);
	}

	m_groupName.SetCurSel(0);
}

void CSeisCvlJpSetInvestSibdDlg::UpdateAllButtonCtrl()
{
	CButton* pBtn = ((CButton*)GetDlgItem(IDC_TM_SIBD_CURVATURE_SURVEY_RDO));
	BOOL bCurvatureSurvey = pBtn->GetCheck();
	GetDlgItem(IDC_TM_SIBD_SURRENDER_SURVEY_RDO)->EnableWindow(bCurvatureSurvey);
	GetDlgItem(IDC_TM_SIBD_SURRENDER_SURVEY_ALPHA_EDT)->EnableWindow(bCurvatureSurvey);
	GetDlgItem(IDC_TM_SIBD_LIMITSTATE_SURVEY_RDO)->EnableWindow(bCurvatureSurvey);
	GetDlgItem(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC1_RDO)->EnableWindow(bCurvatureSurvey);
	GetDlgItem(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC2_RDO)->EnableWindow(bCurvatureSurvey);
	GetDlgItem(IDC_TM_SIBD_LIMITSTATE_ALPHA_EDT)->EnableWindow(bCurvatureSurvey);
	GetDlgItem(IDC_TM_SIBD_STRENGTH_SURVEY_CMB)->EnableWindow(!bCurvatureSurvey);

	if (bCurvatureSurvey) {
		OnChangedCvsvSurveyTypeRdo();
	}
}

void CSeisCvlJpSetInvestSibdDlg::UpdateSurveyTypeCtrl()
{
	CButton* pBtn = ((CButton*)GetDlgItem(IDC_TM_SIBD_STRENGTH_SURVEY_RDO));
	BOOL bStrengthSurvey = pBtn->GetCheck();

	CString str;
	m_groupName.GetWindowText(str);

	T_SIGR_D sigrD;
	T_SIGR_K kSigr = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(str);
	m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(kSigr, sigrD);
	//내력조사 - 상부구조, 사용자 그룹인 경우만 활성
	m_momentType.ResetContent();
	m_momentType.EnableWindow(sigrD.nSubType != T_SIGR_D::kLower && bStrengthSurvey);
	GetDlgItem(IDC_TM_SIBD_STRENGTH_SURVEY_RDO)->EnableWindow(sigrD.nSubType != T_SIGR_D::kLower);

	if (sigrD.nSubType == T_SIGR_D::kLower) {
		((CButton*)GetDlgItem(IDC_TM_SIBD_STRENGTH_SURVEY_RDO))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_TM_SIBD_CURVATURE_SURVEY_RDO))->SetCheck(TRUE);
		m_surveyType = 0; // 1번은 곡률조사
		return;
	}

	//규준에 따른 내력조사 콤보박스 설정
	m_caseName.GetWindowText(str);

	T_SIPA_D sipaD;
	T_SIPA_K kSipa = m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKey(str);
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(kSipa, sipaD) == FALSE)
		return;

	T_SIDC_D sidcD; sidcD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetQSidc()->Get(sidcD);

	if (sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H14 || sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H24) {
		m_momentType.AddString(COMBO_MOMENT_TYPE_My);
		m_momentType.AddString(COMBO_MOMENT_TYPE_Mu);
		m_momentType.SetCurSel(0); //Default=My
	}
	else if (sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H29) {
		if (sipaD.nDesignCode != BrdgSE_CodeJP::JROAD5_H29)
			return;

		if (sipaD.nLimitType == T_SIPA_D::kLimitState1 || sipaD.nLimitType == T_SIPA_D::kLimitState2) {
			m_momentType.AddString(COMBO_MOMENT_TYPE_Myd);
		}
		else if (sipaD.nLimitType == T_SIPA_D::kLimitState3) {
			m_momentType.AddString(COMBO_MOMENT_TYPE_Myd);
			m_momentType.AddString(COMBO_MOMENT_TYPE_Mud);
		}

		m_momentType.SetCurSel(0); //Default=Myd
	}
}

void CSeisCvlJpSetInvestSibdDlg::UpdateCvsvSurveyType()
{
	CString str;
	m_caseName.GetWindowText(str);

	T_SIPA_D sipaD;
	T_SIPA_K kSipa = m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKey(str);
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(kSipa, sipaD) == FALSE)
		return;

	if (sipaD.nDesignCode != BrdgSE_CodeJP::JROAD5_H29)
		return;

	GetDlgItem(IDC_TM_SIBD_LIMITSTATE_SURVEY_RDO)->EnableWindow(sipaD.nLimitType != T_SIPA_D::kLimitState1);
	GetDlgItem(IDC_TM_SIBD_LIMITSTATE_ALPHA_EDT)->EnableWindow(sipaD.nLimitType != T_SIPA_D::kLimitState1);

	((CButton*)GetDlgItem(IDC_TM_SIBD_LIMITSTATE_SURVEY_RDO))->SetCheck(sipaD.nLimitType != T_SIPA_D::kLimitState1);
	((CButton*)GetDlgItem(IDC_TM_SIBD_SURRENDER_SURVEY_RDO))->SetCheck(sipaD.nLimitType == T_SIPA_D::kLimitState1);

	m_cvsv_surveyType = sipaD.nLimitType == T_SIPA_D::kLimitState1 ? 1 : 0;
}

void CSeisCvlJpSetInvestSibdDlg::UpdateRcMemeberCtrl()
{
	T_SIDC_D sidcD; sidcD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetQSidc()->Get(sidcD);

	CString str;
	m_groupName.GetWindowText(str);

	T_SIGR_D sigrD;
	T_SIGR_K kSigr = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(str);

	T_SIGR_LOWER_D sigrLowerD;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigr, sigrLowerD) == FALSE) {
		//상부구조 그룹인 경우
		((CButton*)GetDlgItem(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC1_RDO))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC2_RDO))->EnableWindow(FALSE);
		
		CButton* pBtn = ((CButton*)GetDlgItem(IDC_TM_SIBD_LIMITSTATE_SURVEY_RDO));
		BOOL bLimitStateSurvey = pBtn->GetCheck();
		m_limitStateAlpha.EnableWindow(bLimitStateSurvey);
		return;
	}

	if (sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H14 || sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H24)
		UpdateRcMemeberCtrl_H14_H24(sigrLowerD);
	else if (sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H29)
		UpdateRcMemeberCtrl_H29(sigrLowerD);
}

void CSeisCvlJpSetInvestSibdDlg::UpdateRcMemeberCtrl_H14_H24(const T_SIGR_LOWER_D& sigrLowerD)
{
	SetEnableRcMemeberCtrl(sigrLowerD.nMaterialType == T_SIGR_LOWER_D::kRC);

	if (sigrLowerD.nMaterialType != T_SIGR_LOWER_D::kRC)
		return;

	T_SIDC_D sidcD; sidcD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetQSidc()->Get(sidcD);

	UpdateRcMemeberRadioBtn(sidcD);

	CButton* pBtn = ((CButton*)GetDlgItem(IDC_TM_SIBD_LIMITSTATE_SURVEY_RDO));
	BOOL bLimitStateSurvey = pBtn->GetCheck();

	m_limitStateAlpha.EnableWindow(bLimitStateSurvey);
	if (sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H14)
		UpdateLimitStateAlphaEdt_H14(sigrLowerD);
	else if (sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H24)
		UpdateLimitStateAlphaEdt_H24(sigrLowerD);
}

void CSeisCvlJpSetInvestSibdDlg::UpdateRcMemeberCtrl_H29(const T_SIGR_LOWER_D& sigrLowerD)
{
	GetDlgItem(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC1_STC)->EnableWindow(sigrLowerD.nMaterialType != T_SIGR_LOWER_D::kSteel);
	GetDlgItem(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC2_STC)->EnableWindow(sigrLowerD.nMaterialType != T_SIGR_LOWER_D::kSteel);

	UpdateLimitStateAlphaEdt_H29(sigrLowerD);
}

void CSeisCvlJpSetInvestSibdDlg::SetEnableRcMemeberCtrl(const BOOL& bEnable)
{
	CButton* pBtn = NULL;

	pBtn = (CButton*)GetDlgItem(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC1_RDO);
	pBtn->EnableWindow(bEnable);

	pBtn = (CButton*)GetDlgItem(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC2_RDO);
	pBtn->EnableWindow(bEnable);

	m_limitStateAlpha.EnableWindow(bEnable);
}

void CSeisCvlJpSetInvestSibdDlg::UpdateRcMemeberRadioBtn(const T_SIDC_D& sidcD)
{
	CButton* pBtn = ((CButton*)GetDlgItem(IDC_TM_SIBD_LIMITSTATE_SURVEY_RDO));
	BOOL bLimitStateSurvey = pBtn->GetCheck();

	if (sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H14) {
		pBtn = (CButton*)GetDlgItem(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC1_RDO);
		pBtn->EnableWindow(bLimitStateSurvey);
		pBtn->SetCheck(1);
		m_allowCurvFormula = 0;

		pBtn = (CButton*)GetDlgItem(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC2_RDO);
		pBtn->EnableWindow(FALSE);
		pBtn->SetCheck(0);
	}
	else if (sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H24) {
		pBtn = (CButton*)GetDlgItem(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC1_RDO);
		pBtn->EnableWindow(bLimitStateSurvey);
		pBtn->SetCheck(0);

		pBtn = (CButton*)GetDlgItem(IDC_TM_SIBD_ALLOWABLE_CURVATURE_OF_RC2_RDO);
		pBtn->EnableWindow(bLimitStateSurvey);
		pBtn->SetCheck(1);
		m_allowCurvFormula = 1;
	}
}

void CSeisCvlJpSetInvestSibdDlg::UpdateLimitStateAlphaEdt_H14(const T_SIGR_LOWER_D& groupD)
{
	CString strCase;
	m_caseName.GetWindowText(strCase);

	T_SIPA_K kSipa = m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKey(strCase);

	T_SIPA_D sipaD;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(kSipa, sipaD) == FALSE)
		return;

	double dAlpha = 0;
	if (sipaD.nSesmicPerformanceType == T_SIPA_D::kLevel2) {
		if (sipaD.nSeismicForceType == T_SIPA_D::kType1)
			dAlpha = 3.0;
		else if (sipaD.nSeismicForceType == T_SIPA_D::kType2)
			dAlpha = 1.5;
	}
	else if (sipaD.nSesmicPerformanceType == T_SIPA_D::kLevel3) {
		if (sipaD.nSeismicForceType == T_SIPA_D::kType1)
			dAlpha = 2.4;
		else if (sipaD.nSeismicForceType == T_SIPA_D::kType2)
			dAlpha = 1.2;
	}

	BOOL bSeismicIsolation = FALSE;
	for (int i = 0; i < groupD.BEAR_D.GetCount(); i++)
	{
		if (groupD.BEAR_D[i].nType == T_SIGR_LOWER_BEAR_D::kSeismicIsolation) {
			bSeismicIsolation = TRUE;
			break;
		}
	}

	if (bSeismicIsolation == TRUE)
		dAlpha *= 2;

	CString strDef; strDef.Format(_T("%g"), dAlpha);

	if (bSeismicIsolation == TRUE) {
		GetDlgItem(IDC_TM_SIBD_LIMITSTATE_ALPHA_STC)->SetWindowText(_LS(IDS_IDD_TM_SIBD_IDC_TM_SIBD_LIMITSTATE_ALPHA_STC_AM));
		m_limitStateAlpha.SetWindowText(strDef);
	}
	else {
		GetDlgItem(IDC_TM_SIBD_LIMITSTATE_ALPHA_STC)->SetWindowText(_LS(IDS_IDD_TM_SIBD_IDC_TM_SIBD_LIMITSTATE_ALPHA_STC));
		m_limitStateAlpha.SetWindowText(strDef);
	}
}

void CSeisCvlJpSetInvestSibdDlg::UpdateLimitStateAlphaEdt_H24(const T_SIGR_LOWER_D& groupD)
{
	BOOL bSeismicIsolation = FALSE;
	for (int i = 0; i < groupD.BEAR_D.GetCount(); i++)
	{
		if (groupD.BEAR_D[i].nType == T_SIGR_LOWER_BEAR_D::kSeismicIsolation) {
			bSeismicIsolation = TRUE;
			break;
		}
	}

	if (bSeismicIsolation == TRUE) {
		GetDlgItem(IDC_TM_SIBD_LIMITSTATE_ALPHA_STC)->SetWindowText(_LS(IDS_IDD_TM_SIBD_IDC_TM_SIBD_LIMITSTATE_ALPHA_STC_AM));
		m_limitStateAlpha.SetWindowText(_T("2.4"));
	}
	else {
		GetDlgItem(IDC_TM_SIBD_LIMITSTATE_ALPHA_STC)->SetWindowText(_LS(IDS_IDD_TM_SIBD_IDC_TM_SIBD_LIMITSTATE_ALPHA_STC));
		m_limitStateAlpha.SetWindowText(_T("1.2"));
	}
}

void CSeisCvlJpSetInvestSibdDlg::UpdateLimitStateAlphaEdt_H29(const T_SIGR_LOWER_D& groupD)
{
	BOOL bSeismicIsolation = FALSE;
	for (int i = 0; i < groupD.BEAR_D.GetCount(); i++)
	{
		if (groupD.BEAR_D[i].nType == T_SIGR_LOWER_BEAR_D::kSeismicIsolation) {
			bSeismicIsolation = TRUE;
			break;
		}
	}

	m_limitStateAlpha.EnableWindow(m_cvsv_surveyType == 0 && bSeismicIsolation == TRUE);
	if (bSeismicIsolation == TRUE)
		m_limitStateAlpha.SetWindowText(_T("2.0"));
	else
		m_limitStateAlpha.SetWindowText(_T(""));
}

BOOL CSeisCvlJpSetInvestSibdDlg::AddOrModData()
{
	T_SIBD_D data;
	DlgToData(data);

	//조사 케이스 + 조사 대상 조합으로 기존에 등록되어 있는게 있는지 체크
	BOOL bResult = FALSE;
	T_SIBD_K key = GetExistingDataKey(data);
	if (key == 0) { // Add
		key = m_pDoc->m_pAttrCtrl2->GetQSibd()->GetStartNum();
		bResult = m_pDoc->m_pDataCtrl->AddSibd(key, data);
	}
	else { //Mod
		bResult = m_pDoc->m_pDataCtrl->ModifySibd(key, data);
	}

	ASSERT(bResult);
	return bResult;
}

BOOL CSeisCvlJpSetInvestSibdDlg::DelData()
{
	T_SIBD_D delData;
	DlgToData(delData);

	T_SIBD_K key = 0;
	T_SIBD_D data;

	POSITION pos = m_pDoc->m_pAttrCtrl2->GetQSibd()->GetStart();
	while (pos)
	{
		m_pDoc->m_pAttrCtrl2->GetQSibd()->GetNext(pos, key, data);
		if (data.kSipa == delData.kSipa && data.kSigr == delData.kSigr) {
			return m_pDoc->m_pDataCtrl->DelSibd(key);
		}
	}

	AfxMessageBox(_LS(IDS_IDD_TM_SIBD_MSG_NOT_EXIST_EQUAL_SIPA_SIGR));
	return FALSE;
}

void CSeisCvlJpSetInvestSibdDlg::DlgToData(OUT T_SIBD_D& rData)
{
	T_SIDC_D sidcD; sidcD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetQSidc()->Get(sidcD);

	CString str;
	//조사 케이스
	m_caseName.GetWindowText(str);
	rData.kSipa = m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKey(str);
	//조사 대상
	m_groupName.GetWindowText(str);
	T_KEY key = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(str);
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Exist(key) == FALSE) {
		ASSERT(FALSE);
		return;
	}
	rData.kSigr = key;
	
	rData.nSibdType = m_surveyType == 1 ? T_SIBD_D::kCurvatureSurvey : T_SIBD_D::kStrengthSurvey;
	//내력조사 콤보박스
	if (rData.nSibdType == T_SIBD_D::kStrengthSurvey) {
		m_momentType.GetWindowText(str);
		rData.SRSV_D.nMomentType = GetMomentTypeEnum(str);
	}
	//곡률조사 조건
	else if (rData.nSibdType == T_SIBD_D::kCurvatureSurvey) {
		rData.CVSV_D.nCvsvType = m_cvsv_surveyType == 1 ? T_SIBD_CVSV_D::kLimitStateSurvey : T_SIBD_CVSV_D::kSurrenderSurvey;
		//CVSV_D.nCvsvType 가 항복조사 일 때
		if (rData.CVSV_D.nCvsvType == T_SIBD_CVSV_D::kSurrenderSurvey) {
			m_surrenderAlpha.GetWindowText(str);
			rData.CVSV_D.dSurrenderAlpha = _wtof(str);
		}
		//nCvsvType 가 한계상태조사 일 때
		else if (rData.CVSV_D.nCvsvType == T_SIBD_CVSV_D::kLimitStateSurvey) {
			//RC부재의 허용곡률 계산식
			if (sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H29) {
				rData.CVSV_D.nAllowCurvFormula = T_SIBD_CVSV_D::kPhils2d_Phils3d_Ellipsis;
			}
			else if (sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H14) {
				rData.CVSV_D.nAllowCurvFormula = T_SIBD_CVSV_D::kPhiA_Phiy0_Ellipsis;
			}
			else if (sidcD.nDesignCode == BrdgSE_CodeJP::JROAD5_H24) {
				rData.CVSV_D.nAllowCurvFormula = m_allowCurvFormula == 0 ? T_SIBD_CVSV_D::kPhiA_Phiy0_Ellipsis : T_SIBD_CVSV_D::kPhiA_Phiyls_Ellipsis;
			}
			
			m_limitStateAlpha.GetWindowText(str);
			rData.CVSV_D.dLimitStateAlpha = _wtof(str);
		}
	}
}

T_SIBD_SRSV_D::eMomentType CSeisCvlJpSetInvestSibdDlg::GetMomentTypeEnum(const CString& str)
{
	if (str == COMBO_MOMENT_TYPE_My || str == COMBO_MOMENT_TYPE_Myd)
		return T_SIBD_SRSV_D::kMy;
	else if (str == COMBO_MOMENT_TYPE_Mu || str == COMBO_MOMENT_TYPE_Mud)
		return T_SIBD_SRSV_D::kMu;

	return T_SIBD_SRSV_D::kUnKnown;
}

T_SIBD_K CSeisCvlJpSetInvestSibdDlg::GetExistingDataKey(const T_SIBD_D& newData)
{
	CString str;
	//조사 케이스 + 조사 대상 조합으로 기존에 등록되어 있는게 있는지 체크
	T_SIBD_K key;
	T_SIBD_D data;

	POSITION pos = m_pDoc->m_pAttrCtrl2->GetQSibd()->GetStart();
	while (pos)
	{
		m_pDoc->m_pAttrCtrl2->GetQSibd()->GetNext(pos, key, data);
		if (data.kSipa == newData.kSipa && data.kSigr == newData.kSigr) {
			return key;
		}
	}

	return 0;
}
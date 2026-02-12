// CSeisCvlJpSetInvestSimdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCvlJpSetInvestSimdDlg.h"
#include "SeisCvlJpSetInvestMethodTimeGrid.h"

#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestSimdDlg dialog


CSeisCvlJpSetInvestSimdDlg::CSeisCvlJpSetInvestSimdDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisCvlJpSetInvestSimdDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nOption = 0;
	m_nGroupType = T_SIGR_D::kUnKnown;
	m_nMatlType_plus = T_SIGR_LOWER_D::kUnKnown;
	m_nMatlType_minus = T_SIGR_LOWER_D::kUnKnown;
	m_considerAlpha = FALSE;
	m_bMethodTime = FALSE;
	m_pGrid = NULL;

	T_SIDC_D sidcD; sidcD.Initialize();
	if (m_pDoc->m_pAttrCtrl2->GetQSidc()->Get(sidcD) == TRUE)
		m_nDesignCode = sidcD.nDesignCode;
}

CSeisCvlJpSetInvestSimdDlg::~CSeisCvlJpSetInvestSimdDlg()
{
	m_pDoc = NULL;
	if (m_pGrid != NULL)
	{
		delete m_pGrid;
		m_pGrid = NULL;
	}
}

void CSeisCvlJpSetInvestSimdDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisCvlEvalGrupDlg)
	DDX_Radio(pDX, IDC_TM_SIMD_ADD_RDO, m_nOption);
	DDX_Control(pDX, IDC_TM_SIMD_SIPA_CMB, m_caseName);
	DDX_Control(pDX, IDC_TM_SIMD_SIGR_CMB, m_groupName);
	DDX_Control(pDX, IDC_TM_SIMD_DIR_CMB, m_direction);
	DDX_Control(pDX, IDC_TM_SIMD_DIR_EDT, m_angle);
	DDX_Control(pDX, IDC_TM_SIMD_CALC_TYPE_CMB, m_calcType);
	DDX_Control(pDX, IDC_TM_SIMD_CALC_TYPE_CASE_CMB, m_this_plus);
	DDX_Control(pDX, IDC_TM_SIMD_SIGR_LOWER_CMB, m_groupName_plus);
	DDX_Control(pDX, IDC_TM_SIMD_CALC_TYPE_GROUP_CMB, m_colmGroup_plus);
	DDX_Control(pDX, IDC_TM_SIMD_CALC_TYPE_STEP_DY_CMB, m_stepDeltaY_plus);
	DDX_Control(pDX, IDC_TM_SIMD_CALC_TYPE_STEP_DLS_CMB, m_stepDeltaLs_plus);
	DDX_Control(pDX, IDC_TM_SIMD_CALC_TYPE_UNIT_DA_EDT, m_lengthDeltaA_plus);
	DDX_Control(pDX, IDC_TM_SIMD_CALC_TYPE_CASE_CMB2, m_this_minus);
	DDX_Control(pDX, IDC_TM_SIMD_SIGR_LOWER_CMB2, m_groupName_minus);
	DDX_Control(pDX, IDC_TM_SIMD_CALC_TYPE_GROUP_CMB2, m_colmGroup_minus);
	DDX_Control(pDX, IDC_TM_SIMD_CALC_TYPE_STEP_DY_CMB2, m_stepDeltaY_minus);
	DDX_Control(pDX, IDC_TM_SIMD_CALC_TYPE_STEP_DLS_CMB2, m_stepDeltaLs_minus);
	DDX_Control(pDX, IDC_TM_SIMD_CALC_TYPE_UNIT_DA_EDT2, m_lengthDeltaA_minus);
	DDX_Control(pDX, IDC_TM_SIMD_SAFETY_COEFFICIENT_EDT, m_alpha);
	DDX_Check(pDX, IDC_TM_SIMD_SAFETY_COEFFICIENT_CHK, m_considerAlpha);
	DDX_Control(pDX, IDC_TM_SIMD_CALC_METHOD_CMB, m_calcMethod);
	DDX_Check(pDX, IDC_TM_SIMD_CALC_METHOD_TIME_CHK, m_bMethodTime);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSeisCvlJpSetInvestSimdDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CSeisCvlJpSetInvestSimdDlg)
	ON_CBN_SELCHANGE(IDC_TM_SIMD_SIPA_CMB,	OnChangedCaseCmb)
	ON_CBN_SELCHANGE(IDC_TM_SIMD_SIGR_CMB,	OnChangedGroupCmb)
	ON_CBN_SELCHANGE(IDC_TM_SIMD_DIR_CMB,	OnChangedDirectionCmb)
	ON_CBN_SELCHANGE(IDC_TM_SIMD_CALC_TYPE_CMB, OnChangedCalcTypeCmb)
	ON_BN_CLICKED(IDC_TM_SIMD_CALC_AUTO_BTN, OnCalcAutoBtn)
	ON_CBN_SELCHANGE(IDC_TM_SIMD_CALC_TYPE_CASE_CMB, OnChangedCasePulsCmb)
	ON_CBN_SELCHANGE(IDC_TM_SIMD_CALC_TYPE_CASE_CMB2, OnChangedCaseMinusCmb)
	ON_CBN_SELCHANGE(IDC_TM_SIMD_SIGR_LOWER_CMB, OnChangedGroupPulsCmb)
	ON_CBN_SELCHANGE(IDC_TM_SIMD_SIGR_LOWER_CMB2, OnChangedGroupMinusCmb)
	ON_CBN_SELCHANGE(IDC_TM_SIMD_CALC_TYPE_GROUP_CMB, OnChangedColumnPulsCmb)
	ON_BN_CLICKED(IDC_TM_SIMD_SAFETY_COEFFICIENT_CHK, OnCheckedConsiderAlphaChk)
	ON_BN_CLICKED(IDC_TM_SIMD_CALC_METHOD_TIME_CHK, OnClickMethodTime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSeisCvlJpSetInvestSimdDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	switch (m_nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		OnInitDialog_H14();
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
		OnInitDialog_H24();
		break;
	case BrdgSE_CodeJP::JROAD5_H29:
		OnInitDialog_H29();
		break;
	}

	InitGrid();
	OnClickMethodTime();

	return TRUE;// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisCvlJpSetInvestSimdDlg::InitGrid()
{
	if (m_pGrid != NULL)
	{
		delete m_pGrid;
		m_pGrid = NULL;
	}
	m_pGrid = new CSeisCvlJpSetInvestMethodTimeGrid();

	CRect rectDummy;
	CWnd* pStatic = GetDlgItem(IDC_TM_SIMD_CALC_METHOD_TIME_GRID);
	pStatic->GetClientRect(&rectDummy);
	pStatic->MapWindowPoints(this, rectDummy);
	pStatic->ShowWindow(FALSE);
	if (!m_pGrid->Create(WS_CHILD | WS_VISIBLE, rectDummy, this, IDC_TM_SIMD_CALC_METHOD_TIME_GRID))
	{
		//ASSERT(FALSE);
		return;
	}
	m_pGrid->Initialize();
	SetGrid();
}

void CSeisCvlJpSetInvestSimdDlg::SetGrid()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CString strCase; m_caseName.GetWindowText(strCase);
	T_SIPA_K kSipa = pDoc->m_pAttrCtrl2->GetQSipa()->GetKey(strCase);
	T_SIPA_D tSipa;
	if (pDoc->m_pAttrCtrl2->GetQSipa()->Get(kSipa, tSipa) == FALSE) { ASSERT(FALSE); return; }
	if (tSipa.nSeismicForceDirection != T_SIPA_D::kAxis_Dir && tSipa.nSeismicForceDirection != T_SIPA_D::kPerp_Dir) {
		ASSERT(FALSE); return;
	}

	CArray<CString, CString> aThisName;
	for(int i =0; i< tSipa.aSeisThisK.GetSize(); i++)
	{
		T_THIS_K& kThis = tSipa.aSeisThisK[i];
		T_THIS_D tThis;
		if (pDoc->m_pAttrCtrl-> GetThis(kThis, tThis))
			aThisName.Add(tThis.LoadCaseName);
	}

	m_pGrid->SetRow(aThisName);
}

void CSeisCvlJpSetInvestSimdDlg::GetGird(OUT T_SIMD_D& rData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CArray<CString, CString> aThisName;
	CArray<double, double> aStart;
	CArray<double, double> aEnd;
	m_pGrid->GetRow(aThisName, aStart, aEnd);

	for (int i = 0; i < aThisName.GetSize(); i++)
	{
		T_SIMD_TIME_RANGE_D timeRangeD;
		T_THIS_K kThis = pDoc->m_pAttrCtrl->GetThisKey(aThisName[i]);
		if(kThis == 0) continue;
		timeRangeD.kThis = kThis;
		timeRangeD.dTimeStt = aStart[i];
		timeRangeD.dTimeEnd = aEnd[i];
		rData.aTimeRange.Add(timeRangeD);
	}
}

void CSeisCvlJpSetInvestSimdDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch (lHint)
	{
	case D_UPDATE_UNIT:
		InitStepDeltaACtrl();
		break;
	default:
		break;
	}
}

void CSeisCvlJpSetInvestSimdDlg::OnChangedCaseCmb()
{
	InitStepDeltaLsCtrl();
	InitStepDeltaACtrl();
	UpdateConsiderAlpha();
	SetGrid();
}

void CSeisCvlJpSetInvestSimdDlg::OnChangedGroupCmb()
{
	CString strGroupName;
	m_groupName.GetWindowText(strGroupName);
	T_SIGR_K kSigr = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strGroupName);
	T_SIGR_D sigrD;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(kSigr, sigrD) == TRUE) {
		m_nGroupType = sigrD.nSubType;
	}

	UpdateDirectionCmb();
	InitCalcMethodCtrl();
	UpdateCalcTypeCtrl();
	UpdateAnalysisCaseCtrl();
	UpdateGroupLowNameCtrl(m_groupName_plus);
	UpdateGroupLowNameCtrl(m_groupName_minus, TRUE);
	InitColmGroupCtrl(m_groupName_plus, m_colmGroup_plus);
	InitColmGroupCtrl(m_groupName_minus, m_colmGroup_minus);
	UpdateColmGroupGroupCtrl();
	UpdateStepDeltaYCtrl();
	UpdateStepDeltaLsCtrl();
	UpdateStepDeltaACtrl();
	UpdateAlphaCtrl();
	UpdateConsiderAlpha();
}

void CSeisCvlJpSetInvestSimdDlg::OnChangedDirectionCmb()
{
	UpdateAngleCtrl();
}

void CSeisCvlJpSetInvestSimdDlg::OnChangedCalcTypeCmb()
{
	UpdateDirectionCmb();
	UpdateAnalysisCaseCtrl();
	UpdateCalcAutoBtn();
	UpdateGroupLowNameCtrl(m_groupName_plus);
	UpdateGroupLowNameCtrl(m_groupName_minus, TRUE);
	UpdateColmGroupGroupCtrl();
	UpdateStepDeltaYCtrl();
	UpdateStepDeltaLsCtrl();
	UpdateStepDeltaACtrl();
	UpdateAlphaCtrl();
	UpdateConsiderAlpha();
}

void CSeisCvlJpSetInvestSimdDlg::OnCalcAutoBtn()
{
	UddateValueByCalcTypeAuto();
}

void CSeisCvlJpSetInvestSimdDlg::OnChangedCasePulsCmb()
{
	switch (m_nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		OnChangedCasePulsCmb_H14();
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
		OnChangedCasePulsCmb_H24();
		break;
	case BrdgSE_CodeJP::JROAD5_H29:
		OnChangedCasePulsCmb_H29();
		break;
	}
}

void CSeisCvlJpSetInvestSimdDlg::OnChangedCaseMinusCmb()
{
	switch (m_nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		OnChangedCaseMinusCmb_H14();
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
		OnChangedCaseMinusCmb_H24();
		break;
	case BrdgSE_CodeJP::JROAD5_H29:
		OnChangedCaseMinusCmb_H29();
		break;
	}
}

void CSeisCvlJpSetInvestSimdDlg::OnChangedGroupPulsCmb()
{
	CString strName; m_groupName_plus.GetWindowText(strName);

	//변경된 + 방향 값을 - 방향에도 동일하게 변경해 준다.
	int iIndex = m_groupName_plus.FindStringExact(-1, strName);
	if (iIndex >= 0) m_groupName_minus.SetCurSel(iIndex);
	else			 m_groupName_minus.SetCurSel(0);

	InitColmGroupCtrl(m_groupName_plus, m_colmGroup_plus);
	InitColmGroupCtrl(m_groupName_minus, m_colmGroup_minus);
	
	T_SIGR_K kSigr = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strName);

	T_SIGR_LOWER_D sigrLowerD;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigr, sigrLowerD) == TRUE) {
		m_nMatlType_plus = sigrLowerD.nMaterialType;
		m_nMatlType_minus = sigrLowerD.nMaterialType;
	}
}

void CSeisCvlJpSetInvestSimdDlg::OnChangedGroupMinusCmb()
{
	InitColmGroupCtrl(m_groupName_minus, m_colmGroup_minus);

	CString strName; m_groupName_minus.GetWindowText(strName);
	T_SIGR_K kSigr = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strName);

	T_SIGR_LOWER_D sigrLowerD;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigr, sigrLowerD) == TRUE)
		m_nMatlType_minus = sigrLowerD.nMaterialType;
}

void CSeisCvlJpSetInvestSimdDlg::OnChangedColumnPulsCmb()
{
	CString strName; m_colmGroup_plus.GetWindowText(strName);

	//변경된 + 방향 값을 - 방향에도 동일하게 변경해 준다.
	int iIndex = m_colmGroup_minus.FindStringExact(-1, strName);
	if (iIndex >= 0) m_colmGroup_minus.SetCurSel(iIndex);
	else			 m_colmGroup_minus.SetCurSel(0);
}

void CSeisCvlJpSetInvestSimdDlg::OnCheckedConsiderAlphaChk()
{
	UpdateColmGroupGroupCtrl();
}

void CSeisCvlJpSetInvestSimdDlg::OnClickMethodTime()
{
	UpdateData(TRUE);

	m_pGrid->EnableWindow(m_bMethodTime);
	m_pGrid->EnableControl(m_bMethodTime);
}

void CSeisCvlJpSetInvestSimdDlg::Execute()
{
	UpdateData(TRUE);

	if (m_nOption == 0)
		AddOrModData();
	else
		DelData();
}

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestSimdDlg diagnostics
// 
void CSeisCvlJpSetInvestSimdDlg::InitControl()
{
	CButton* pBtn = NULL;
	//옵션 추가/변경 삭제 라디오 버튼
	pBtn = (CButton*)GetDlgItem(IDC_TM_SIMD_ADD_RDO);
	pBtn->SetCheck(1);
	m_nOption = 0;
}

void CSeisCvlJpSetInvestSimdDlg::InitCaseNameCtrl()
{
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->GetCount() < 1)
		return;

	m_caseName.ResetContent();

	CArray<T_SIPA_K, T_SIPA_K> keys;
	m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKeyList(keys);
	for (int i = 0; i < keys.GetCount(); i++)
	{
		T_SIPA_K& key = keys[i];
		T_SIPA_D data;
		m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(key, data);

		if (m_nDesignCode != data.nDesignCode)
			continue;

		m_caseName.AddString(data.strCaseName);
	}
	m_caseName.SetCurSel(0);
}

void CSeisCvlJpSetInvestSimdDlg::InitGroupNameCtrl()
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
		if (data.nSubType == T_SIGR_D::kUser)
			continue;

		m_groupName.AddString(data.strName);
	}
	m_groupName.SetCurSel(0);

	CString strGroupName;
	m_groupName.GetWindowText(strGroupName);
	T_SIGR_K kSigr = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strGroupName);
	T_SIGR_D sigrD;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(kSigr, sigrD) == TRUE) {
		m_nGroupType = sigrD.nSubType;
	}
	InitCalcMethodCtrl();
}

void CSeisCvlJpSetInvestSimdDlg::InitDirectionCtrl()
{
	m_direction.AddString(COMBO_DIRECTION_SEISMIC);
	m_direction.AddString(COMBO_DIRECTION_ELEM_COORDINATE);
	m_direction.AddString(COMBO_DIRECTION_USER_DEFINE);
	m_direction.SetCurSel(0);
}

void CSeisCvlJpSetInvestSimdDlg::InitCalcMethodCtrl()
{
	m_calcMethod.ResetContent();
	int index = m_calcMethod.InsertString(0, COMBO_CALC_METHOD_NEXCO);
	m_calcMethod.SetItemData(index, seis_jp::kNEXCO);
	if(m_nGroupType == T_SIGR_D::kLower) 
	{
		index = m_calcMethod.InsertString(1, COMBO_CALC_METHOD_JBEC);
		m_calcMethod.SetItemData(index, seis_jp::kJBEC);
	}
	m_calcMethod.SetCurSel(0);

	if (m_nGroupType == T_SIGR_D::kUpper)m_calcMethod.EnableWindow(FALSE);
	else m_calcMethod.EnableWindow(TRUE);
}

void CSeisCvlJpSetInvestSimdDlg::UpdateCalcTypeCtrl()
{
	m_calcType.ResetContent();

	if (m_nGroupType == T_SIGR_D::eSubType::kUpper) {
		m_calcType.AddString(COMBO_CALC_TYPE_FROM_RESULT);
		m_calcType.AddString(COMBO_CALC_TYPE_INPUT_VALUE);
	}
	else if (m_nGroupType == T_SIGR_D::eSubType::kLower) {
		m_calcType.AddString(COMBO_CALC_TYPE_FROM_RESULT);
		m_calcType.AddString(COMBO_CALC_TYPE_FROM_MPHI_Dy);
		if (CTestEnvMgr::GetTestEnvST(_T("DeltaPy_from_MPhi")) == _T("yes")) {
			if (m_nMatlType_plus != T_SIGR_LOWER_D::kSteel && m_nMatlType_minus != T_SIGR_LOWER_D::kSteel)
				m_calcType.AddString(COMBO_CALC_TYPE_FROM_MPHI_Dpy);
		}
		m_calcType.AddString(COMBO_CALC_TYPE_INPUT_VALUE);
	}
	else {
		ASSERT(FALSE);
	}

	m_calcType.SetCurSel(0);
}

void CSeisCvlJpSetInvestSimdDlg::InitAnalysisCaseCtrl(MComboBox& rComboBox)
{
	CArray<T_THIS_K, T_THIS_K> keys;
	m_pDoc->m_pAttrCtrl->GetThisKeyList(keys);
	for (int i = 0; i < keys.GetCount(); i++)
	{
		T_THIS_K& key = keys[i];

		if (m_pDoc->IsDummyInitLoadThis(key) == TRUE)
			continue;

		T_THIS_D data;
		m_pDoc->m_pAttrCtrl->GetThis(key, data);
		if (data.nAnalMethod != D_THIS_ANALMETHOD::D_THIS_ANALMETHOD_STATIC) //정적 해석 케이스만 표시
			continue;

		rComboBox.AddString(data.LoadCaseName);
	}
	rComboBox.SetCurSel(0);
}

void CSeisCvlJpSetInvestSimdDlg::InitGroupLowNameCtrl(MComboBox& rComboBox, OUT int& rMatlType)
{
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetCount() < 1)
		return;

	rComboBox.ResetContent();
	rComboBox.AddString(COMBO_DEFAULT);

	CStringArray arName;
	CArray<T_SIGR_LOWER_K, T_SIGR_LOWER_K> keys;
	m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetKeyList(keys);
	if (keys.GetCount() < 1) return;

	for (int i = 0; i < keys.GetCount(); i++)
	{
		T_SIGR_LOWER_K& key = keys[i];
		T_SIGR_LOWER_D data;
		m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(key, data);
		CString str = m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetSigrName(key);
		arName.Add(str);
		rComboBox.AddString(str);
	}
	rComboBox.SetCurSel(0);

	T_SIGR_K kSigr = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(arName[0]);

	T_SIGR_LOWER_D sigrLowerD;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigr, sigrLowerD) == TRUE)
		rMatlType = sigrLowerD.nMaterialType;
}

void CSeisCvlJpSetInvestSimdDlg::UpdateGroupLowNameCtrl(MComboBox& rComboBox, BOOL bReadOnly/*=FALSE*/)
{
	switch (m_nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		UpdateGroupLowNameCtrl_H14(rComboBox, bReadOnly);
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
		UpdateGroupLowNameCtrl_H24(rComboBox, bReadOnly);
		break;
	case BrdgSE_CodeJP::JROAD5_H29:
		UpdateGroupLowNameCtrl_H29(rComboBox, bReadOnly);
		break;
	}
}

void CSeisCvlJpSetInvestSimdDlg::UpdateAnalysisCaseCtrl()
{
	CString str;
	m_calcType.GetWindowText(str);

	m_this_plus.EnableWindow(str == COMBO_CALC_TYPE_FROM_RESULT);
	m_this_minus.EnableWindow(str == COMBO_CALC_TYPE_FROM_RESULT);
}

void CSeisCvlJpSetInvestSimdDlg::UpdateCalcAutoBtn()
{
	CString str;
	m_calcType.GetWindowText(str);

	GetDlgItem(IDC_TM_SIMD_CALC_AUTO_BTN)->EnableWindow(str == COMBO_CALC_TYPE_FROM_RESULT);
}

void CSeisCvlJpSetInvestSimdDlg::InitColmGroupCtrl(const MComboBox& groupNameCmb, MComboBox& rComboBox)
{
	switch (m_nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		InitColmGroupCtrl_H14(groupNameCmb, rComboBox);
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
		InitColmGroupCtrl_H24(groupNameCmb, rComboBox);
		break;
	case BrdgSE_CodeJP::JROAD5_H29:
		InitColmGroupCtrl_H29(groupNameCmb, rComboBox);
		break;
	}
}

void CSeisCvlJpSetInvestSimdDlg::UpdateColmGroupGroupCtrl()
{
	switch (m_nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		UpdateColmGroupGroupCtrl_H14();
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
		UpdateColmGroupGroupCtrl_H24();
		break;
	case BrdgSE_CodeJP::JROAD5_H29:
		UpdateColmGroupGroupCtrl_H29();
		break;
	}
}

void CSeisCvlJpSetInvestSimdDlg::UpdateStepDeltaYCtrl()
{
	switch (m_nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		UpdateStepDeltaYCtrl_H14();
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
		UpdateStepDeltaYCtrl_H24();
		break;
	case BrdgSE_CodeJP::JROAD5_H29:
		UpdateStepDeltaYCtrl_H29();
		break;
	}
}

void CSeisCvlJpSetInvestSimdDlg::InitStepDeltaLsCtrl()
{
	switch (m_nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		InitStepDeltaLsCtrl_H14();
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
		InitStepDeltaLsCtrl_H24();
		break;
	case BrdgSE_CodeJP::JROAD5_H29:
		InitStepDeltaLsCtrl_H29();
		break;
	}
}

void CSeisCvlJpSetInvestSimdDlg::UpdateStepDeltaLsCtrl()
{
	CString str;
	m_calcType.GetWindowText(str);

	BOOL bEnable = FALSE;
	if (str == COMBO_CALC_TYPE_FROM_RESULT)	bEnable = TRUE;
	else if (str == COMBO_CALC_TYPE_FROM_MPHI_Dy || str == COMBO_CALC_TYPE_FROM_MPHI_Dpy) bEnable = FALSE;
	else if (str == COMBO_CALC_TYPE_INPUT_VALUE)	bEnable = FALSE;

	m_stepDeltaLs_plus.EnableWindow(bEnable);
	m_stepDeltaLs_minus.EnableWindow(bEnable);
}

void CSeisCvlJpSetInvestSimdDlg::InitStepDeltaACtrl()
{
	switch (m_nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		InitStepDeltaACtrl_H14();
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
		InitStepDeltaACtrl_H24();
		break;
	case BrdgSE_CodeJP::JROAD5_H29:
		InitStepDeltaACtrl_H29();
		break;
	}
}

void CSeisCvlJpSetInvestSimdDlg::UpdateStepDeltaACtrl()
{
	CString str;
	m_calcType.GetWindowText(str);

	BOOL bEnable = FALSE;
	if (str == COMBO_CALC_TYPE_FROM_RESULT)	bEnable = FALSE;
	else if (str == COMBO_CALC_TYPE_FROM_MPHI_Dy || str == COMBO_CALC_TYPE_FROM_MPHI_Dpy) bEnable = FALSE;
	else if (str == COMBO_CALC_TYPE_INPUT_VALUE)	bEnable = TRUE;

	m_lengthDeltaA_plus.EnableWindow(bEnable);
	m_lengthDeltaA_minus.EnableWindow(bEnable);
}

void CSeisCvlJpSetInvestSimdDlg::UpdateLoadCaseStepCtrl(const MComboBox& loadCase, MComboBox& rComboBox)
{
	CString str;
	loadCase.GetWindowText(str);

	T_THIS_D data;
	T_THIS_K key = m_pDoc->m_pAttrCtrl->GetThisKey(str);
	if (m_pDoc->m_pAttrCtrl->GetThis(key, data) == FALSE)
		return;

	rComboBox.ResetContent();
	rComboBox.AddString(COMBO_DEFAULT);

	int nStep = data.nOut;
	while (nStep <= data.nIncrementStep)
	{
		str.Format(_T("%d"), nStep);
		rComboBox.AddString(str);

		nStep += data.nOut;
	}
	
	rComboBox.SetCurSel(0);
}

void CSeisCvlJpSetInvestSimdDlg::UpdateAlphaCtrl()
{
	CString str;
	m_calcType.GetWindowText(str);
	if (str == COMBO_CALC_TYPE_INPUT_VALUE) {
		m_alpha.SetWindowText(_T(""));
		m_alpha.EnableWindow(FALSE);
		return;
	}

	CString strGroupPlus; m_groupName_plus.GetWindowText(strGroupPlus);
	CString strGroupMinus; m_groupName_plus.GetWindowText(strGroupMinus);
	T_SIGR_LOWER_D::eMaterialType typePlus = m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetMaterialType(strGroupPlus);
	T_SIGR_LOWER_D::eMaterialType typeMinus = m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetMaterialType(strGroupMinus);

	if (typePlus == T_SIGR_LOWER_D::kSteel && typeMinus == T_SIGR_LOWER_D::kSteel) {
		m_alpha.SetWindowText(_T(""));
		m_alpha.EnableWindow(FALSE);
		return;
	}

	switch (m_nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		UpdateAlphaCtrl_H14();
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
		UpdateAlphaCtrl_H24();
		break;
	case BrdgSE_CodeJP::JROAD5_H29:
		UpdateAlphaCtrl_H29();
		break;
	}
}

void CSeisCvlJpSetInvestSimdDlg::GetBearingArray(CDBDoc* pDoc, const CString& strGroup, CArray<T_SIGR_LOWER_BEAR_D, T_SIGR_LOWER_BEAR_D&>& bearArray)
{
	T_SIGR_LOWER_D dLower;
	T_SIGR_K kSigr = pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strGroup);
	if (pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigr, dLower) == FALSE)
		return;

	bearArray.Append(dLower.BEAR_D);
}

void CSeisCvlJpSetInvestSimdDlg::UpdateConsiderAlpha()
{
	switch (m_nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		UpdateConsiderAlpha_H14();
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
		UpdateConsiderAlpha_H24();
		break;
	case BrdgSE_CodeJP::JROAD5_H29:
		UpdateConsiderAlpha_H29();
		break;
	}
}

void CSeisCvlJpSetInvestSimdDlg::UpdateDirectionCmb()
{
	CString strCalcType, strDir;
	m_calcType.GetWindowText(strCalcType);
	m_direction.GetWindowText(strDir);

	m_direction.EnableWindow(strCalcType != COMBO_CALC_TYPE_INPUT_VALUE);
	m_angle.EnableWindow(strCalcType != COMBO_CALC_TYPE_INPUT_VALUE && strDir == COMBO_DIRECTION_USER_DEFINE);
}

void CSeisCvlJpSetInvestSimdDlg::UpdateAngleCtrl()
{
	CString str;
	m_direction.GetWindowText(str);

	m_angle.EnableWindow(str == COMBO_DIRECTION_USER_DEFINE);
}

void CSeisCvlJpSetInvestSimdDlg::UddateValueByCalcTypeAuto(const UINT/*MPhiProp::eDir*/& dir/*=MPhiProp::kDirMax*/)
{
	/*QIehp::eCom*/UINT comp = QIehp::kUnknown;
	GetComponent(m_caseName, m_groupName, comp);

	if (dir == MPhiProp::kPlus) {
		if (UddateControlByCalcTypeAuto(comp, dir, m_this_plus, m_groupName_plus, m_colmGroup_plus, m_stepDeltaY_plus, m_stepDeltaLs_plus) == FALSE)
			AfxMessageBox(_LS(IDS_IDD_TM_SIMD_MSG_NOT_EXIST_YIELD_STEP_PLUS));
	}
	else if (dir == MPhiProp::kMinus) {
		if (UddateControlByCalcTypeAuto(comp, dir, m_this_minus, m_groupName_minus, m_colmGroup_minus, m_stepDeltaY_minus, m_stepDeltaLs_minus) == FALSE)
			AfxMessageBox(_LS(IDS_IDD_TM_SIMD_MSG_NOT_EXIST_YIELD_STEP_MINUS));
	}
	else {
		UddateControlByYZDir();
	}

	// 교각그룹의 +, - 방향 값은 항상 같은 값으로 유지해 준다.
	CString strGroupPlus, strGroupMinus;
	m_groupName_plus.GetWindowText(strGroupPlus);
	m_groupName_minus.GetWindowText(strGroupMinus);
	if (strGroupPlus != strGroupMinus) {
		int iIndex = m_groupName_minus.FindStringExact(-1, strGroupPlus);
		if (iIndex >= 0) {
			m_groupName_minus.SetCurSel(iIndex);
		}
		else {
			m_groupName_minus.SetCurSel(0);
		}
	}

	// 기둥그룹의 +, - 방향 값은 항상 같은 값으로 유지해 준다.
	CString strColumnPlus, strColumnMinus;
	m_colmGroup_plus.GetWindowText(strColumnPlus);
	m_colmGroup_minus.GetWindowText(strColumnMinus);
	if (strColumnPlus != strColumnMinus) {
		int iIndex = m_colmGroup_minus.FindStringExact(-1, strGroupPlus);
		if (iIndex >= 0) {
			m_colmGroup_minus.SetCurSel(iIndex);
		}
		else {
			m_colmGroup_minus.SetCurSel(0);
		}
	}
}

void CSeisCvlJpSetInvestSimdDlg::GetComponent(const MComboBox& caseNameCmb, const MComboBox& groupNameCmb, OUT UINT& rComp)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CString strCase; caseNameCmb.GetWindowText(strCase);
	T_SIPA_K kSipa = pDoc->m_pAttrCtrl2->GetQSipa()->GetKey(strCase);
	T_SIPA_D tSipa;
	if (pDoc->m_pAttrCtrl2->GetQSipa()->Get(kSipa, tSipa) == FALSE) { ASSERT(FALSE); return; }
	if (tSipa.nSeismicForceDirection != T_SIPA_D::kAxis_Dir && tSipa.nSeismicForceDirection != T_SIPA_D::kPerp_Dir) {
		ASSERT(FALSE); return;
	}
	
	CString str;
	groupNameCmb.GetWindowText(str);

	T_SIGR_D tSigr;
	T_SIGR_K kSigr = pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(str);
	if (pDoc->m_pAttrCtrl2->GetQSigr()->Get(kSigr, tSigr) == FALSE) { ASSERT(FALSE); return; }

	if (tSigr.nSubType == T_SIGR_D::kLower) {
		if (GetComponentByColumn(tSipa.nSeismicForceDirection, kSigr, rComp) == TRUE)
			return;
	}

	T_KEY_LIST keyList;
	pDoc->m_pAttrCtrl2->GetQSigrLower()->GetKeyList(keyList);

	for (int i = 0; i < keyList.GetCount(); i++)
	{
		const T_SIGR_LOWER_K& kSigrLower = keyList[i];
		// 임의의 기둥의 교축방향 전단력 방향을 참고하여 Component 를 반환한다.
		if (GetComponentByColumn(tSipa.nSeismicForceDirection, kSigrLower, rComp) == TRUE)
			break;
	}
}

BOOL CSeisCvlJpSetInvestSimdDlg::GetComponentByColumn(	const UINT/*T_SIPA_D::eSeismicForceDirection*/& nSeismicForceDir, 
														const T_SIGR_LOWER_K& kSigrLower, 
														OUT UINT/*QIehp::eCom*/& rComponent)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_SIGR_LOWER_D tSigrLower;
	if (pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigrLower, tSigrLower) == FALSE) { ASSERT(FALSE); return FALSE; }
	if (tSigrLower.COLM_D.GetCount() < 1) { ASSERT(FALSE); return FALSE; }

	const T_SIGR_LOWER_COLM_D& column = tSigrLower.COLM_D[0];
	if (column.nDirShear != seis_jp::kY_Dir && column.nDirShear != seis_jp::kZ_Dir)
		return FALSE;

	if (nSeismicForceDir == T_SIPA_D::kAxis_Dir) {
		if (column.nDirShear == seis_jp::kY_Dir)
			rComponent = QIehp::kMz;
		else if (column.nDirShear == seis_jp::kZ_Dir)
			rComponent = QIehp::kMy;
	}
	else if (nSeismicForceDir == T_SIPA_D::kPerp_Dir) {
		if (column.nDirShear == seis_jp::kY_Dir)
			rComponent = QIehp::kMy;
		else if (column.nDirShear == seis_jp::kZ_Dir)
			rComponent = QIehp::kMz;
	}

	return (rComponent == QIehp::kMy || rComponent == QIehp::kMz);
}

void CSeisCvlJpSetInvestSimdDlg::UddateControlByYZDir()
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP();
	if (pResult == NULL) return;

	CString strGroupName;
	m_groupName.GetWindowText(strGroupName);
	T_SIGR_K kSigr = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strGroupName);

	CString strThis_plus; m_this_plus.GetWindowText(strThis_plus);
	T_THIS_K kThis_plus = m_pDoc->m_pAttrCtrl->GetThisKey(strThis_plus);

	T_YIELD_STEP stepMy_plus, stepMz_plus;
	if (pResult->GetUltimateStepForSeismic(kSigr, kThis_plus, QIehp::kMy, MPhiProp::kPlus, stepMy_plus) == FALSE)
		stepMy_plus.nUltimateStep = UINT_MAX;
	if (pResult->GetUltimateStepForSeismic(kSigr, kThis_plus, QIehp::kMz, MPhiProp::kPlus, stepMz_plus) == FALSE)
		stepMz_plus.nUltimateStep = UINT_MAX;

	UINT compPlus = (stepMy_plus.nUltimateStep < stepMz_plus.nUltimateStep) ? QIehp::kMy : QIehp::kMz;
	if (UddateControlByCalcTypeAuto(compPlus, MPhiProp::kPlus, m_this_plus, m_groupName_plus, m_colmGroup_plus, m_stepDeltaY_plus, m_stepDeltaLs_plus) == FALSE)
		AfxMessageBox(_LS(IDS_IDD_TM_SIMD_MSG_NOT_EXIST_YIELD_STEP_PLUS));
	

	CString strThis_minus; m_this_minus.GetWindowText(strThis_minus);
	T_THIS_K kThis_minus = m_pDoc->m_pAttrCtrl->GetThisKey(strThis_minus);

	T_YIELD_STEP stepMy_minus, stepMz_minus;
	if(pResult->GetUltimateStepForSeismic(kSigr, kThis_minus, QIehp::kMy, MPhiProp::kMinus, stepMy_minus) == FALSE)
		stepMy_minus.nUltimateStep = UINT_MAX;
	if (pResult->GetUltimateStepForSeismic(kSigr, kThis_minus, QIehp::kMz, MPhiProp::kMinus, stepMz_minus) == FALSE)
		stepMz_minus.nUltimateStep = UINT_MAX;

	UINT compMinus = (stepMy_minus.nUltimateStep < stepMz_minus.nUltimateStep) ? QIehp::kMy : QIehp::kMz;
	if (UddateControlByCalcTypeAuto(compMinus, MPhiProp::kMinus, m_this_minus, m_groupName_minus, m_colmGroup_minus, m_stepDeltaY_minus, m_stepDeltaLs_minus) == FALSE)
		AfxMessageBox(_LS(IDS_IDD_TM_SIMD_MSG_NOT_EXIST_YIELD_STEP_MINUS));
}

BOOL CSeisCvlJpSetInvestSimdDlg::UddateControlByCalcTypeAuto(const UINT/*QIehp::eCom*/& comp,
															 const UINT/*MPhiProp::eDir*/& dir,
															 const MComboBox& thisCmb,
															 OUT MComboBox& groupNameCmb,
															 OUT MComboBox& colmGroupCmb,
															 OUT MComboBox& stepDeltaYCmb,
															 OUT MComboBox& stepDeltaLsCmb)
{
	switch (m_nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		return UddateControlByCalcTypeAuto_H14(comp, dir, thisCmb, groupNameCmb, colmGroupCmb, stepDeltaYCmb, stepDeltaLsCmb);
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
		return UddateControlByCalcTypeAuto_H24(comp, dir, thisCmb, groupNameCmb, colmGroupCmb, stepDeltaYCmb, stepDeltaLsCmb);
		break;
	case BrdgSE_CodeJP::JROAD5_H29:
		return UddateControlByCalcTypeAuto_H29(comp, dir, thisCmb, groupNameCmb, colmGroupCmb, stepDeltaYCmb, stepDeltaLsCmb);
		break;
	}
	return FALSE;
}

BOOL CSeisCvlJpSetInvestSimdDlg::AddOrModData()
{
	T_SIMD_D data;
	if (DlgToData(data) == FALSE) {
		CString str = _LS(IDS_IDD_TM_SIMD_MSG_INVALID_VALUE);
		AfxMessageBox(str);
		return FALSE;
	}

	if (CheckData(data) == FALSE)
		return FALSE;

	//조사 케이스 + 조사 대상 조합으로 기존에 등록되어 있는게 있는지 체크
	BOOL bResult = FALSE;
	T_SIMD_K key = GetExistingDataKey(data);
	if (key == 0) { // Add
		key = m_pDoc->m_pAttrCtrl2->GetQSimd()->GetStartNum();
		bResult = m_pDoc->m_pDataCtrl->AddSimd(key, data);
	}
	else { //Mod
		bResult = m_pDoc->m_pDataCtrl->ModifySimd(key, data);
	}

	ASSERT(bResult);
	return bResult;
}

BOOL CSeisCvlJpSetInvestSimdDlg::DelData()
{
	T_SIMD_D delData;
	DlgToData(delData);

	T_SIMD_K key = 0;
	T_SIMD_D data;

	POSITION pos = m_pDoc->m_pAttrCtrl2->GetQSimd()->GetStart();
	while (pos)
	{
		m_pDoc->m_pAttrCtrl2->GetQSimd()->GetNext(pos, key, data);
		if (data.kSipa == delData.kSipa && data.kSigr == delData.kSigr) {
			return m_pDoc->m_pDataCtrl->DelSimd(key);
		}
	}

	AfxMessageBox(_LS(IDS_IDD_TM_SIMD_MSG_NOT_EXIST_EQUAL_SIPA_SIGR));
	return FALSE;
}

BOOL CSeisCvlJpSetInvestSimdDlg::DlgToData(OUT T_SIMD_D& rData)
{
	// 시간범위 설정방법
	rData.bTimeStepRange = m_bMethodTime;
	GetGird(rData);

	switch (m_nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		return DlgToData_H14(rData);
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
		return DlgToData_H24(rData);
		break;
	case BrdgSE_CodeJP::JROAD5_H29:
		return DlgToData_H29(rData);
		break;
	}
	return FALSE;
}

BOOL CSeisCvlJpSetInvestSimdDlg::CheckData(const T_SIMD_D& data)
{
	CString str;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Exist(data.kSipa) == FALSE) {
		str = _LS(IDS_IDD_TM_SIMD_MSG_NOT_EXIST_SIPA_VALUE);
		AfxMessageBox(str);
		return FALSE;
	}

	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Exist(data.kSigr) == FALSE) {
		str = _LS(IDS_IDD_TM_SIMD_MSG_NOT_EXIST_SIGR_VALUE);
		AfxMessageBox(str);
		return FALSE;
	}

	if (data.nDirDisp == seis_jp::kUser_Dir) {
		if (data.dAngle <= 0 || data.dAngle >= 360) {
			str = _LS(IDS_IDD_TM_SIMD_MSG_INVALID_VALUE_ANGLE);
			AfxMessageBox(str);
			return FALSE;
		}
	}

	if (data.bTimeStepRange)
	{
		for (int i = 0; i < data.aTimeRange.GetSize(); i++)
		{
			T_SIMD_TIME_RANGE_D timeD = data.aTimeRange[i];
			if (timeD.dTimeStt < 0 || timeD.dTimeStt >= timeD.dTimeEnd)
			{
				str = _LS(IDS_METHOD_TIME_ERROR);
				AfxMessageBox(str);
				return FALSE;
		}
	}
	}

	return TRUE;
}

BOOL ExistNodeKeyInSubNode(const T_KEY_LIST& aSubNode, const CArray<T_NODE_K, T_NODE_K>& nodeKeys)
{
	if (aSubNode.GetCount() < 1) return FALSE;

	for (int j = 0; j < nodeKeys.GetCount(); j++)
	{
		T_NODE_K kNode = nodeKeys[j];
		const auto itr = std::find(&aSubNode[0], &aSubNode[0] + aSubNode.GetCount(), kNode);
		if (itr != &aSubNode[0] + aSubNode.GetCount()) {
			return TRUE;
		}
	}

	return FALSE;
}

T_SIMD_K CSeisCvlJpSetInvestSimdDlg::GetExistingDataKey(const T_SIMD_D& newData)
{
	CString str;
	//조사 케이스 + 조사 대상 조합으로 기존에 등록되어 있는게 있는지 체크
	T_SIMD_K key;
	T_SIMD_D data;

	POSITION pos = m_pDoc->m_pAttrCtrl2->GetQSimd()->GetStart();
	while (pos)
	{
		m_pDoc->m_pAttrCtrl2->GetQSimd()->GetNext(pos, key, data);
		if (data.kSipa == newData.kSipa && data.kSigr == newData.kSigr) {
			return key;
		}
	}

	return 0;
}
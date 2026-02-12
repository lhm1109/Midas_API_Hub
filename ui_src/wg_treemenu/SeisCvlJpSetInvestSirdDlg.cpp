// CSeisCvlJpSetInvestSirdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCvlJpSetInvestSirdDlg.h"
#include "SeisCvlJpSetInvestMethodTimeGrid.h"

#include "..\wg_base\TestEnvMgr.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"

#include "SeisCvlJpSetInvestSimdDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COMBO_CALC_TYPE_FROM_RESULT		_LS(IDS_IDD_TM_SIRD_IDC_TM_SIRD_CALC_TYPE_CMB_FROM_RESULT)
#define COMBO_CALC_TYPE_FROM_MPHI_Dy	_LS(IDS_IDD_TM_SIRD_IDC_TM_SIRD_CALC_TYPE_CMB_FROM_MPhi_Dy)
#define COMBO_CALC_TYPE_FROM_MPHI_Dpy	_LS(IDS_IDD_TM_SIRD_IDC_TM_SIRD_CALC_TYPE_CMB_FROM_MPhi_Dpy)
#define COMBO_CALC_TYPE_INPUT_VALUE		_LS(IDS_IDD_TM_SIRD_IDC_TM_SIRD_CALC_TYPE_CMB_INPUT_VALUE)

#define COMBO_DIRECTION_SEISMIC			_LS(IDS_IDD_TM_SIRD_IDC_TM_SIRD_DIR_SEISMIC)
#define COMBO_DIRECTION_ELEM_COORDINATE	_LS(IDS_IDD_TM_SIRD_IDC_TM_SIRD_DIR_ELEM_COORDINATE)
#define COMBO_DIRECTION_USER_DEFINE		_LS(IDS_IDD_TM_SIRD_IDC_TM_SIRD_DIR_USER_DEFINE)

#define COMBO_DEFAULT _T("")

#define COMBO_CALC_METHOD_NEXCO			_LS(IDS_IDC_TM_SIRD_CALC_METHOD_NEXCO)
#define COMBO_CALC_METHOD_JBEC			_LS(IDS_IDC_TM_SIRD_CALC_METHOD_JBEC)

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestSirdDlg dialog

CSeisCvlJpSetInvestSirdDlg::CSeisCvlJpSetInvestSirdDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisCvlJpSetInvestSirdDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nOption = 0;
	m_bMethodTime = FALSE;
	m_pGrid = NULL;
}

CSeisCvlJpSetInvestSirdDlg::~CSeisCvlJpSetInvestSirdDlg()
{
	m_pDoc = NULL;
	if (m_pGrid != NULL)
	{
		delete m_pGrid;
		m_pGrid = NULL;
	}
}

void CSeisCvlJpSetInvestSirdDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisCvlEvalGrupDlg)
	DDX_Radio(pDX, IDC_TM_SIRD_ADD_RDO, m_nOption);
	DDX_Control(pDX, IDC_TM_SIRD_SIPA_CMB, m_caseName);
	DDX_Control(pDX, IDC_TM_SIRD_SIGR_CMB, m_groupName);
	DDX_Control(pDX, IDC_TM_SIRD_DIR_CMB, m_direction);
	DDX_Control(pDX, IDC_TM_SIRD_DIR_EDT, m_angle);
	DDX_Control(pDX, IDC_TM_SIRD_CALC_TYPE_CMB, m_calcType);
	DDX_Control(pDX, IDC_TM_SIRD_CALC_TYPE_CASE_CMB, m_this_plus);
	DDX_Control(pDX, IDC_TM_SIRD_SIGR_LOWER_CMB, m_groupName_plus);
	DDX_Control(pDX, IDC_TM_SIRD_CALC_TYPE_GROUP_CMB, m_colmGroup_plus);
	DDX_Control(pDX, IDC_TM_SIRD_CALC_TYPE_STEP_DY_CMB, m_stepDeltaY_plus);
	DDX_Control(pDX, IDC_TM_SIRD_CALC_TYPE_UNIT_DA_EDT, m_length_plus);
	DDX_Control(pDX, IDC_TM_SIRD_CALC_TYPE_CASE_CMB2, m_this_minus);
	DDX_Control(pDX, IDC_TM_SIRD_SIGR_LOWER_CMB2, m_groupName_minus);
	DDX_Control(pDX, IDC_TM_SIRD_CALC_TYPE_GROUP_CMB2, m_colmGroup_minus);
	DDX_Control(pDX, IDC_TM_SIRD_CALC_TYPE_STEP_DY_CMB2, m_stepDeltaY_minus);
	DDX_Control(pDX, IDC_TM_SIRD_CALC_TYPE_UNIT_DA_EDT2, m_length_minus);
	DDX_Control(pDX, IDC_TM_SIRD_CALC_METHOD_CMB, m_calcMethod);
	DDX_Check(pDX, IDC_TM_SIRD_CALC_METHOD_TIME_CHK, m_bMethodTime);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSeisCvlJpSetInvestSirdDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CSeisCvlJpSetInvestSirdDlg)
	ON_CBN_SELCHANGE(IDC_TM_SIRD_SIPA_CMB, OnChangedCaseCmb)
	ON_CBN_SELCHANGE(IDC_TM_SIRD_SIGR_CMB, OnChangedGroupCmb)
	ON_CBN_SELCHANGE(IDC_TM_SIRD_DIR_CMB, OnChangedDirectionCmb)
	ON_CBN_SELCHANGE(IDC_TM_SIRD_CALC_TYPE_CMB, OnChangedCalcTypeCmb)
	ON_BN_CLICKED(IDC_TM_SIRD_CALC_AUTO_BTN, OnCalcAutoBtn)
	ON_CBN_SELCHANGE(IDC_TM_SIRD_CALC_TYPE_CASE_CMB, OnChangedCasePulsCmb)
	ON_CBN_SELCHANGE(IDC_TM_SIRD_CALC_TYPE_CASE_CMB2, OnChangedCaseMinusCmb)
	ON_CBN_SELCHANGE(IDC_TM_SIRD_SIGR_LOWER_CMB, OnChangedGroupPulsCmb)
	ON_CBN_SELCHANGE(IDC_TM_SIRD_SIGR_LOWER_CMB2, OnChangedGroupMinusCmb)
	ON_CBN_SELCHANGE(IDC_TM_SIRD_CALC_TYPE_GROUP_CMB, OnChangedColumnPulsCmb)
	ON_BN_CLICKED(IDC_TM_SIRD_CALC_METHOD_TIME_CHK, OnClickMethodTime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSeisCvlJpSetInvestSirdDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	InitControl();
	InitCaseNameCtrl();
	InitGroupNameCtrl();
	InitDirectionCtrl();
	UpdateAngleCtrl();
	UpdateCalcTypeCtrl();
	InitAnalysisCaseCtrl(m_this_plus);
	InitAnalysisCaseCtrl(m_this_minus);
	InitGroupLowNameCtrl(m_groupName_plus);
	InitGroupLowNameCtrl(m_groupName_minus);
	UpdateAnalysisCaseCtrl();
	InitColmGroupCtrl(m_groupName_plus, m_colmGroup_plus);
	InitColmGroupCtrl(m_groupName_minus, m_colmGroup_minus);
	UpdateGroupLowNameCtrl(m_groupName_plus);
	UpdateGroupLowNameCtrl(m_groupName_minus, TRUE);
	UpdateColmGroupGroupCtrl();
	UpdateStepDeltaYCtrl();
	UpdateLoadCaseStepCtrl(m_this_plus, m_stepDeltaY_plus);
	UpdateLoadCaseStepCtrl(m_this_minus, m_stepDeltaY_minus);
	InitDeltaY_LengthCtrl();
	UpdateDeltaY_LengthCtrl();
	InitGrid();
	OnClickMethodTime();

	return TRUE;// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisCvlJpSetInvestSirdDlg::InitGrid()
{
	if (m_pGrid != NULL)
	{
		delete m_pGrid;
		m_pGrid = NULL;
	}
	m_pGrid = new CSeisCvlJpSetInvestMethodTimeGrid();

	CRect rectDummy;
	CWnd* pStatic = GetDlgItem(IDC_TM_SIRD_CALC_METHOD_TIME_GRID);
	pStatic->GetClientRect(&rectDummy);
	pStatic->MapWindowPoints(this, rectDummy);
	pStatic->ShowWindow(FALSE);
	if (!m_pGrid->Create(WS_CHILD | WS_VISIBLE, rectDummy, this, IDC_TM_SIRD_CALC_METHOD_TIME_GRID))
	{
		//ASSERT(FALSE);
		return;
	}
	m_pGrid->Initialize();
	SetGrid();
}

void CSeisCvlJpSetInvestSirdDlg::SetGrid()
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
	for (int i = 0; i < tSipa.aSeisThisK.GetSize(); i++)
	{
		T_THIS_K& kThis = tSipa.aSeisThisK[i];
		T_THIS_D tThis;
		if (pDoc->m_pAttrCtrl->GetThis(kThis, tThis))
			aThisName.Add(tThis.LoadCaseName);
	}

	m_pGrid->SetRow(aThisName);
}

void CSeisCvlJpSetInvestSirdDlg::GetGird(OUT T_SIRD_D& rData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CArray<CString, CString> aThisName;
	CArray<double, double> aStart;
	CArray<double, double> aEnd;
	m_pGrid->GetRow(aThisName, aStart, aEnd);

	for (int i = 0; i < aThisName.GetSize(); i++)
	{
		T_SIRD_TIME_RANGE_D timeRangeD;
		T_THIS_K kThis = pDoc->m_pAttrCtrl->GetThisKey(aThisName[i]);
		if (kThis == 0) continue;
		timeRangeD.kThis = kThis;
		timeRangeD.dTimeStt = aStart[i];
		timeRangeD.dTimeEnd = aEnd[i];
		rData.aTimeRange.Add(timeRangeD);
	}
}

void CSeisCvlJpSetInvestSirdDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch (lHint)
	{
	case D_UPDATE_UNIT:
		InitDeltaY_LengthCtrl();
		break;
	default:
		break;
	}
}

void CSeisCvlJpSetInvestSirdDlg::OnChangedCaseCmb()
{
}

void CSeisCvlJpSetInvestSirdDlg::OnChangedGroupCmb()
{
	InitCalcMethodCtrl();
	UpdateCalcTypeCtrl();
	UpdateAnalysisCaseCtrl();
	UpdateGroupLowNameCtrl(m_groupName_plus);
	UpdateGroupLowNameCtrl(m_groupName_minus, TRUE);
	InitColmGroupCtrl(m_groupName_plus, m_colmGroup_plus);
	InitColmGroupCtrl(m_groupName_minus, m_colmGroup_minus);
	UpdateStepDeltaYCtrl();
	UpdateDeltaY_LengthCtrl();
}

void CSeisCvlJpSetInvestSirdDlg::OnChangedDirectionCmb()
{
	UpdateAngleCtrl();
}

void CSeisCvlJpSetInvestSirdDlg::OnChangedCalcTypeCmb()
{
	UpdateDirectionCmb();
	UpdateAnalysisCaseCtrl();
	UpdateCalcAutoBtn();
	UpdateColmGroupGroupCtrl();
	UpdateGroupLowNameCtrl(m_groupName_plus);
	UpdateGroupLowNameCtrl(m_groupName_minus, TRUE);
	UpdateStepDeltaYCtrl();
	UpdateDeltaY_LengthCtrl();
}

void CSeisCvlJpSetInvestSirdDlg::OnCalcAutoBtn()
{
	UddateValueByCalcTypeAuto();
}

void CSeisCvlJpSetInvestSirdDlg::OnChangedCasePulsCmb()
{
	UpdateLoadCaseStepCtrl(m_this_plus, m_stepDeltaY_plus);
}

void CSeisCvlJpSetInvestSirdDlg::OnChangedCaseMinusCmb()
{
	UpdateLoadCaseStepCtrl(m_this_minus, m_stepDeltaY_minus);
}

void CSeisCvlJpSetInvestSirdDlg::OnChangedGroupPulsCmb()
{
	CString strName; m_groupName_plus.GetWindowText(strName);

	//변경된 + 방향 값을 - 방향에도 동일하게 변경해 준다.
	int iIndex = m_groupName_plus.FindStringExact(-1, strName);
	if (iIndex >= 0) m_groupName_minus.SetCurSel(iIndex);
	else			 m_groupName_minus.SetCurSel(0);

	InitColmGroupCtrl(m_groupName_plus, m_colmGroup_plus);
	InitColmGroupCtrl(m_groupName_minus, m_colmGroup_minus);
}

void CSeisCvlJpSetInvestSirdDlg::OnChangedGroupMinusCmb()
{
	InitColmGroupCtrl(m_groupName_minus, m_colmGroup_minus);
}

void CSeisCvlJpSetInvestSirdDlg::OnChangedColumnPulsCmb()
{
	CString strName; m_colmGroup_plus.GetWindowText(strName);

	//변경된 + 방향 값을 - 방향에도 동일하게 변경해 준다.
	int iIndex = m_colmGroup_minus.FindStringExact(-1, strName);
	if (iIndex >= 0) m_colmGroup_minus.SetCurSel(iIndex);
	else			 m_colmGroup_minus.SetCurSel(0);
}

void CSeisCvlJpSetInvestSirdDlg::OnClickMethodTime()
{
	UpdateData(TRUE);

	m_pGrid->EnableWindow(m_bMethodTime);
	m_pGrid->EnableControl(m_bMethodTime);
}

void CSeisCvlJpSetInvestSirdDlg::Execute()
{
	UpdateData(TRUE);

	if (m_nOption == 0)
		AddOrModData();
	else
		DelData();
}

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestSirdDlg diagnostics
// 
void CSeisCvlJpSetInvestSirdDlg::InitControl()
{
	CButton* pBtn = NULL;
	//옵션 추가/변경 삭제 라디오 버튼
	pBtn = (CButton*)GetDlgItem(IDC_TM_SIRD_ADD_RDO);
	pBtn->SetCheck(1);
	m_nOption = 0;
}

void CSeisCvlJpSetInvestSirdDlg::InitCaseNameCtrl()
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

		if (data.nDesignCode == BrdgSE_CodeJP::JROAD5_H14 ||
			data.nDesignCode == BrdgSE_CodeJP::JROAD5_H24) {
			if (data.nSesmicPerformanceType != T_SIPA_D::kLevel2)
				continue;
		}

		if (data.nDesignCode == BrdgSE_CodeJP::JROAD5_H29) {
			if(data.nLimitType != T_SIPA_D::kLimitState2)
				continue;
		}

		m_caseName.AddString(data.strCaseName);
	}
	m_caseName.SetCurSel(0);
}

void CSeisCvlJpSetInvestSirdDlg::InitGroupNameCtrl()
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
	InitCalcMethodCtrl();
}

void CSeisCvlJpSetInvestSirdDlg::InitDirectionCtrl()
{
	m_direction.AddString(COMBO_DIRECTION_SEISMIC);
	m_direction.AddString(COMBO_DIRECTION_ELEM_COORDINATE);
	m_direction.AddString(COMBO_DIRECTION_USER_DEFINE);
	m_direction.SetCurSel(0);
}

void CSeisCvlJpSetInvestSirdDlg::InitCalcMethodCtrl()
{
	CString strGroupName;
	m_groupName.GetWindowText(strGroupName);
	T_SIGR_K kSigr = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strGroupName);
	T_SIGR_D sigrD;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(kSigr, sigrD) == FALSE) return;

	m_calcMethod.ResetContent();
	int index = m_calcMethod.InsertString(0, COMBO_CALC_METHOD_NEXCO);
	m_calcMethod.SetItemData(index, seis_jp::kNEXCO);
	if (sigrD.nSubType == T_SIGR_D::kLower)
	{
		index = m_calcMethod.InsertString(1, COMBO_CALC_METHOD_JBEC);
		m_calcMethod.SetItemData(index, seis_jp::kJBEC);
	}
	m_calcMethod.SetCurSel(0);
}

void CSeisCvlJpSetInvestSirdDlg::UpdateDirectionCmb()
{
	CString strCalcType, strDir;
	m_calcType.GetWindowText(strCalcType);
	m_direction.GetWindowText(strDir);

	m_direction.EnableWindow(strCalcType != COMBO_CALC_TYPE_INPUT_VALUE);
	m_angle.EnableWindow(strCalcType != COMBO_CALC_TYPE_INPUT_VALUE && strDir == COMBO_DIRECTION_USER_DEFINE);
}

void CSeisCvlJpSetInvestSirdDlg::UpdateAngleCtrl()
{
	CString str;
	m_direction.GetWindowText(str);

	m_angle.EnableWindow(str == COMBO_DIRECTION_USER_DEFINE);
}

void CSeisCvlJpSetInvestSirdDlg::UpdateCalcTypeCtrl()
{
	m_calcType.ResetContent();

	CString str;
	m_groupName.GetWindowText(str);

	T_SIGR_D data;
	T_SIGR_K key = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(str);
	m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(key, data);

	if (data.nSubType == T_SIGR_D::kUpper) {
		m_calcType.AddString(COMBO_CALC_TYPE_FROM_RESULT);
		m_calcType.AddString(COMBO_CALC_TYPE_INPUT_VALUE);
	}
	else if (data.nSubType == T_SIGR_D::kLower) {
		m_calcType.AddString(COMBO_CALC_TYPE_FROM_RESULT);
		m_calcType.AddString(COMBO_CALC_TYPE_FROM_MPHI_Dy);
		if (CTestEnvMgr::GetTestEnvST(_T("DeltaPy_from_MPhi")) == _T("yes")) {
			m_calcType.AddString(COMBO_CALC_TYPE_FROM_MPHI_Dpy);
		}
		m_calcType.AddString(COMBO_CALC_TYPE_INPUT_VALUE);
	}
	else {
		ASSERT(FALSE);
	}

	m_calcType.SetCurSel(0);
}

void CSeisCvlJpSetInvestSirdDlg::InitAnalysisCaseCtrl(MComboBox& rComboBox)
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

void CSeisCvlJpSetInvestSirdDlg::InitGroupLowNameCtrl(MComboBox& rComboBox)
{
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetCount() < 1)
		return;

	rComboBox.ResetContent();
	rComboBox.AddString(COMBO_DEFAULT);

	CArray<T_SIGR_LOWER_K, T_SIGR_LOWER_K> keys;
	m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetKeyList(keys);
	for (int i = 0; i < keys.GetCount(); i++)
	{
		T_SIGR_LOWER_K& key = keys[i];
		T_SIGR_LOWER_D data;
		m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(key, data);

		CString str = m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetSigrName(key);
		rComboBox.AddString(str);
	}
	rComboBox.SetCurSel(0);
}

void CSeisCvlJpSetInvestSirdDlg::UpdateGroupLowNameCtrl(MComboBox& rComboBox, BOOL bReadOnly/*=FALSE*/)
{
	CString strGroup, strCalcType;
	m_groupName.GetWindowText(strGroup);
	m_calcType.GetWindowText(strCalcType);

	T_SIGR_D dSigr;
	T_SIGR_K kSigr = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strGroup);
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(kSigr, dSigr) == FALSE)
		return;

	if (dSigr.nSubType == T_SIGR_D::kLower) {
		int iIndex = rComboBox.FindStringExact(-1, strGroup);
		if (iIndex >= 0) {
			rComboBox.SetCurSel(iIndex);
		}
	}

	BOOL bEnable = FALSE;
	if (bReadOnly == FALSE) {
		bEnable = dSigr.nSubType == T_SIGR_D::kUpper &&
			(strCalcType == COMBO_CALC_TYPE_FROM_RESULT ||
				strCalcType == COMBO_CALC_TYPE_INPUT_VALUE);
	}

	rComboBox.EnableWindow(bEnable);
}

void CSeisCvlJpSetInvestSirdDlg::UpdateAnalysisCaseCtrl()
{
	CString str;
	m_calcType.GetWindowText(str);

	m_this_plus.EnableWindow(str == COMBO_CALC_TYPE_FROM_RESULT);
	m_this_minus.EnableWindow(str == COMBO_CALC_TYPE_FROM_RESULT);
}

void CSeisCvlJpSetInvestSirdDlg::UpdateCalcAutoBtn()
{
	CString str;
	m_calcType.GetWindowText(str);

	GetDlgItem(IDC_TM_SIRD_CALC_AUTO_BTN)->EnableWindow(str == COMBO_CALC_TYPE_FROM_RESULT);
}

void CSeisCvlJpSetInvestSirdDlg::InitColmGroupCtrl(const MComboBox& groupNameCmb, MComboBox& rComboBox)
{
	CString strGroup;
	m_groupName.GetWindowText(strGroup);

	T_SIGR_D dSigr;
	T_SIGR_K kSigr = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strGroup);
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(kSigr, dSigr) == FALSE)
		return;

	T_SIGR_LOWER_D dLower;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigr, dLower) == FALSE) {
		groupNameCmb.GetWindowText(strGroup);
		kSigr = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strGroup);
		if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigr, dLower) == FALSE)
			return;
	}

	rComboBox.ResetContent();
	rComboBox.AddString(COMBO_DEFAULT);

	for (int i = 0; i < dLower.COLM_D.GetCount(); i++)
	{
		T_SIGR_LOWER_COLM_D data = dLower.COLM_D[i];
		rComboBox.AddString(data.strName);
	}
	rComboBox.SetCurSel(0);
}

void CSeisCvlJpSetInvestSirdDlg::UpdateColmGroupGroupCtrl()
{
	CString strGroup;
	m_groupName.GetWindowText(strGroup);

	T_SIGR_D dSigr;
	T_SIGR_K kSigr = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strGroup);
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(kSigr, dSigr) == FALSE)
		return;

	//기둥하단으로 부터 관성력 작용위치까지의 거리 h를 구하기 위해 상부그룹이 선택되더라도 활성화 함.
	//if (dSigr.nSubType != T_SIGR_D::kLower) {
	//	m_colmGroup_plus.EnableWindow(FALSE);
	//	m_colmGroup_minus.EnableWindow(FALSE);
	//	return;
	//}

	//CString str;
	//m_calcType.GetWindowText(str);
	//하부공 단독으로 검토할 경우, M-φ관계로 산정과 직접입력의 경우에도 동해석은 돌려야 한다고 함. δMax 를 알기위해
	m_colmGroup_plus.EnableWindow(TRUE);//(str == COMBO_CALC_TYPE_FROM_RESULT);
	m_colmGroup_minus.EnableWindow(FALSE);// - 방향은 항상 비활성 + 방향 값을 따라감
}

void CSeisCvlJpSetInvestSirdDlg::UpdateStepDeltaYCtrl()
{
	CString str;
	m_calcType.GetWindowText(str);

	BOOL bEnable = FALSE;
	if (str == COMBO_CALC_TYPE_FROM_RESULT)	bEnable = TRUE;
	else if (str == COMBO_CALC_TYPE_FROM_MPHI_Dy || str == COMBO_CALC_TYPE_FROM_MPHI_Dpy)		bEnable = FALSE;
	else if (str == COMBO_CALC_TYPE_INPUT_VALUE)	bEnable = FALSE;

	m_stepDeltaY_plus.EnableWindow(bEnable);
	m_stepDeltaY_minus.EnableWindow(bEnable);
}

void CSeisCvlJpSetInvestSirdDlg::UpdateLoadCaseStepCtrl(const MComboBox& loadCase, MComboBox& rComboBox)
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

void CSeisCvlJpSetInvestSirdDlg::InitDeltaY_LengthCtrl()
{
	T_SIRD_UNIT* pUnit = &(CUnitCtrl::m_SIRD_UNIT);

	CString strUnit;
	CUnitCtrl::GetUnitSystem(pUnit->Length, strUnit);

	CString strMsg1; strMsg1.Format(_LS(IDS_IDD_TM_SIRD_IDC_TM_SIRD_CALC_TYPE_UNIT_DA_STC_y), strUnit);
	CString strMsg2; strMsg2.Format(_LS(IDS_IDD_TM_SIRD_IDC_TM_SIRD_CALC_TYPE_UNIT_DA_STC2_y), strUnit);

	GetDlgItem(IDC_TM_SIRD_CALC_TYPE_UNIT_DA_STC)->SetWindowText(strMsg1);
	GetDlgItem(IDC_TM_SIRD_CALC_TYPE_UNIT_DA_STC2)->SetWindowText(strMsg2);
}

void CSeisCvlJpSetInvestSirdDlg::UpdateDeltaY_LengthCtrl()
{
	CString str;
	m_calcType.GetWindowText(str);

	BOOL bEnable = FALSE;
	if (str == COMBO_CALC_TYPE_FROM_RESULT)	bEnable = FALSE;
	else if (str == COMBO_CALC_TYPE_FROM_MPHI_Dy || str == COMBO_CALC_TYPE_FROM_MPHI_Dpy)		bEnable = FALSE;
	else if (str == COMBO_CALC_TYPE_INPUT_VALUE)	bEnable = TRUE;

	m_length_plus.EnableWindow(bEnable);
	m_length_minus.EnableWindow(bEnable);
}

void CSeisCvlJpSetInvestSirdDlg::UddateValueByCalcTypeAuto(const UINT/*MPhiProp::eDir*/& dir/*=MPhiProp::kDirMax*/)
{
	/*QIehp::eCom*/UINT comp = QIehp::kUnknown;
	CSeisCvlJpSetInvestSimdDlg::GetComponent(m_caseName, m_groupName, comp);

	if (dir == MPhiProp::kPlus) {
		if(UddateControlByCalcTypeAuto(comp, dir, m_this_plus, m_groupName_plus, m_colmGroup_plus, m_stepDeltaY_plus) == FALSE)
			AfxMessageBox(_LS(IDS_IDD_TM_SIRD_MSG_NOT_EXIST_YIELD_STEP_PLUS));
	}
	else if (dir == MPhiProp::kMinus) {
		if(UddateControlByCalcTypeAuto(comp, dir, m_this_minus, m_groupName_minus, m_colmGroup_minus, m_stepDeltaY_minus) == FALSE)
			AfxMessageBox(_LS(IDS_IDD_TM_SIRD_MSG_NOT_EXIST_YIELD_STEP_MINUS));
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

void CSeisCvlJpSetInvestSirdDlg::UddateControlByYZDir()
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP();
	if (pResult == NULL) return;

	CString strGroupName;
	m_groupName.GetWindowText(strGroupName);
	T_SIGR_K kSigr = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strGroupName);

	CString strThis_plus; m_this_plus.GetWindowText(strThis_plus);
	T_THIS_K kThis_plus = m_pDoc->m_pAttrCtrl->GetThisKey(strThis_plus);

	T_YIELD_STEP stepMy_plus, stepMz_plus;
	if (pResult->GetYieldStepForSeismic(kSigr, kThis_plus, QIehp::kMy, MPhiProp::kPlus, stepMy_plus) == FALSE)
		stepMy_plus.nYieldStep = UINT_MAX;
	if (pResult->GetYieldStepForSeismic(kSigr, kThis_plus, QIehp::kMz, MPhiProp::kPlus, stepMz_plus) == FALSE)
		stepMz_plus.nYieldStep = UINT_MAX;

	UINT compPlus = (stepMy_plus.nYieldStep < stepMz_plus.nYieldStep) ? QIehp::kMy : QIehp::kMz;
	if (UddateControlByCalcTypeAuto(compPlus, MPhiProp::kPlus, m_this_plus, m_groupName_plus, m_colmGroup_plus, m_stepDeltaY_plus) == FALSE)
		AfxMessageBox(_LS(IDS_IDD_TM_SIRD_MSG_NOT_EXIST_YIELD_STEP_PLUS));

	CString strThis_minus; m_this_minus.GetWindowText(strThis_minus);
	T_THIS_K kThis_minus = m_pDoc->m_pAttrCtrl->GetThisKey(strThis_minus);

	T_YIELD_STEP stepMy_minus, stepMz_minus;
	if (pResult->GetYieldStepForSeismic(kSigr, kThis_minus, QIehp::kMy, MPhiProp::kMinus, stepMy_minus) == FALSE)
		stepMy_minus.nYieldStep = UINT_MAX;
	if (pResult->GetYieldStepForSeismic(kSigr, kThis_minus, QIehp::kMz, MPhiProp::kMinus, stepMz_minus) == FALSE)
		stepMz_minus.nYieldStep = UINT_MAX;

	UINT compMinus = (stepMy_minus.nYieldStep < stepMz_minus.nYieldStep) ? QIehp::kMy : QIehp::kMz;
	if (UddateControlByCalcTypeAuto(compMinus, MPhiProp::kMinus, m_this_minus, m_groupName_minus, m_colmGroup_minus, m_stepDeltaY_minus) == FALSE)
		AfxMessageBox(_LS(IDS_IDD_TM_SIRD_MSG_NOT_EXIST_YIELD_STEP_MINUS));
}

BOOL CSeisCvlJpSetInvestSirdDlg::UddateControlByCalcTypeAuto(const UINT/*QIehp::eCom*/& comp,
															 const UINT/*MPhiProp::eDir*/& dir,
															 const MComboBox& thisCmb,
															 OUT MComboBox& groupNameCmb,
															 OUT MComboBox& colmGroupCmb,
															 OUT MComboBox& stepDeltaYCmb)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP();
	if (pResult == NULL) return FALSE;

	CString strGroupName;
	m_groupName.GetWindowText(strGroupName);
	T_SIGR_K kSigr = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strGroupName);
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Exist(kSigr) == FALSE) { ASSERT(FALSE); return FALSE; }

	CString strThis; thisCmb.GetWindowText(strThis);
	T_THIS_K kThis = m_pDoc->m_pAttrCtrl->GetThisKey(strThis);
	if (m_pDoc->m_pAttrCtrl->ExistThis(kThis) == FALSE) { ASSERT(FALSE); return FALSE; }

	T_YIELD_STEP data;
	if (pResult->GetYieldStepForSeismic(kSigr, kThis, comp, dir, data) == FALSE) {
		groupNameCmb.SetCurSel(0);
		colmGroupCmb.SetCurSel(0);
		stepDeltaYCmb.SetCurSel(0);
		return FALSE;
	}

	int iIndex = 0;
	//대상교량
	T_SIGR_D tSisr;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(data.kSigrLower, tSisr) == FALSE) { ASSERT(FALSE); return FALSE; }
	iIndex = groupNameCmb.FindStringExact(-1, tSisr.strName);
	if (iIndex >= 0) groupNameCmb.SetCurSel(iIndex);
	else			 groupNameCmb.SetCurSel(0);

	InitColmGroupCtrl(groupNameCmb, colmGroupCmb);

	//대상교각
	iIndex = colmGroupCmb.FindStringExact(-1, data.strColumnName);
	if (iIndex >= 0) {
		colmGroupCmb.SetCurSel(iIndex);
	}
	else {
		ASSERT(FALSE);
		colmGroupCmb.SetCurSel(0);
	}

	//δy 스텝
	CString strYStep; strYStep.Format(_T("%d"), data.nYieldStep);
	iIndex = stepDeltaYCmb.FindStringExact(-1, strYStep);
	if (iIndex >= 0) {
		stepDeltaYCmb.SetCurSel(iIndex);
	}
	else {
		ASSERT(FALSE);
		stepDeltaYCmb.SetCurSel(0);
	}

	return TRUE;
}

BOOL CSeisCvlJpSetInvestSirdDlg::AddOrModData()
{
	T_SIRD_D data;
	if (DlgToData(data) == FALSE) {
		CString str = _LS(IDS_IDD_TM_SIRD_MSG_INVALID_VALUE);
		AfxMessageBox(str);
		return FALSE;
	}

	if (CheckData(data) == FALSE)
		return FALSE;

	//조사 케이스 + 조사 대상 조합으로 기존에 등록되어 있는게 있는지 체크
	BOOL bResult = FALSE;
	T_SIRD_K key = GetExistingDataKey(data);
	if (key == 0) { // Add
		key = m_pDoc->m_pAttrCtrl2->GetQSird()->GetStartNum();
		bResult = m_pDoc->m_pDataCtrl->AddSird(key, data);
	}
	else { //Mod
		bResult = m_pDoc->m_pDataCtrl->ModifySird(key, data);
	}

	ASSERT(bResult);
	return bResult;
}

BOOL CSeisCvlJpSetInvestSirdDlg::DelData()
{
	T_SIRD_D delData;
	DlgToData(delData);

	T_SIRD_K key = 0;
	T_SIRD_D data;

	POSITION pos = m_pDoc->m_pAttrCtrl2->GetQSird()->GetStart();
	while (pos)
	{
		m_pDoc->m_pAttrCtrl2->GetQSird()->GetNext(pos, key, data);
		if (data.kSipa == delData.kSipa && data.kSigr == delData.kSigr) {
			return m_pDoc->m_pDataCtrl->DelSird(key);
		}
	}

	AfxMessageBox(_LS(IDS_IDD_TM_SIRD_MSG_NOT_EXIST_EQUAL_SIPA_SIGR));
	return FALSE;
}

BOOL CSeisCvlJpSetInvestSirdDlg::DlgToData(OUT T_SIRD_D& rData)
{
	CString str;
	//조사 케이스
	m_caseName.GetWindowText(str);
	rData.kSipa = m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKey(str);
	//조사 대상
	m_groupName.GetWindowText(str);
	T_KEY key = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(str);
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Exist(key) == FALSE) {
		ASSERT(FALSE);
		return FALSE;
	}
	rData.kSigr = key;
	//변위 방향
	m_direction.GetWindowText(str);
	if (str == COMBO_DIRECTION_SEISMIC)
		rData.nDirDisp = seis_jp::kSeismicForce_Dir;
	else if (str == COMBO_DIRECTION_ELEM_COORDINATE)
		rData.nDirDisp = seis_jp::kLocal_Dir;
	else if (str == COMBO_DIRECTION_USER_DEFINE)
		rData.nDirDisp = seis_jp::kUser_Dir;
	//x축과의 변위 각도
	if (m_angle.IsWindowEnabled() == TRUE) {
		m_angle.GetWindowText(str);
		rData.dAngle = _wtof(str);
	}
	else {
		rData.dAngle = -1;
	}

	// 허용변위계산 방법
	rData.nRspCalcType = m_calcMethod.GetItemData(m_calcMethod.GetCurSel());
	if (rData.nRspCalcType <= 0) return FALSE;

	// 시간범위 설정방법
	rData.bTimeStepRange = m_bMethodTime;
	GetGird(rData);

	//잔류변위 계산방식
	m_calcType.GetWindowText(str);
	if (str == COMBO_CALC_TYPE_FROM_RESULT)
		rData.nAlwCalcType = seis_jp::kAnalysisResult;
	else if (str == COMBO_CALC_TYPE_FROM_MPHI_Dy)
		rData.nAlwCalcType = seis_jp::kMphi_Dy;
	else if (str == COMBO_CALC_TYPE_FROM_MPHI_Dpy)
		rData.nAlwCalcType = seis_jp::kMphi_Dpy;
	else if (str == COMBO_CALC_TYPE_INPUT_VALUE)
		rData.nAlwCalcType = seis_jp::kUserInput;
	//////////////// (+) 방향 잔류변위 계산조건 ////////////////
	rData.PLUS_D.nDirAnal = seis_jp::kPlus;
	//해석 케이스
	if (rData.nAlwCalcType == seis_jp::kAnalysisResult) {
		m_this_plus.GetWindowText(str);
		key = m_pDoc->m_pAttrCtrl->GetThisKey(str);
		if (m_pDoc->m_pAttrCtrl->ExistThis(key) == TRUE) rData.PLUS_D.kThis = key;
	}
	//허용잔류변위 산출 교각
	if (rData.nAlwCalcType != seis_jp::kUserInput) {
		m_groupName_plus.GetWindowText(str);
		key = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(str);
		if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Exist(key) == FALSE) {
			return FALSE;
		}
		rData.PLUS_D.kSigrLower = key;
	}
	//기둥그룹
	if (rData.nAlwCalcType != seis_jp::kUserInput)
	{
		m_colmGroup_plus.GetWindowText(rData.PLUS_D.strNameCOLM);
		if (rData.PLUS_D.strNameCOLM == COMBO_DEFAULT)
			return FALSE;
	}
	//발생 스텝(δy)
	if (rData.nAlwCalcType == seis_jp::kAnalysisResult)
	{
		m_stepDeltaY_plus.GetWindowText(str);
		if (str == COMBO_DEFAULT)
			return FALSE;
		rData.PLUS_D.nStepDeltaY = _wtof(str);
	}
	//길이
	if (m_length_plus.IsWindowEnabled() == TRUE) {
		m_length_plus.GetWindowText(str);
		rData.PLUS_D.dLengthDeltaVar = _wtof(str);
	}
	//////////////// (-) 방향 잔류변위 계산조건 ////////////////
	rData.MINUS_D.nDirAnal = seis_jp::kMinus;
	//해석 케이스
	if (rData.nAlwCalcType == seis_jp::kAnalysisResult) {
		m_this_minus.GetWindowText(str);
		key = m_pDoc->m_pAttrCtrl->GetThisKey(str);
		if (m_pDoc->m_pAttrCtrl->ExistThis(key) == TRUE) rData.MINUS_D.kThis = key;
	}
	//허용잔류변위 산출 교각
	if (rData.nAlwCalcType != seis_jp::kUserInput) {
		m_groupName_minus.GetWindowText(str);
		key = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(str);
		if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Exist(key) == FALSE) {
			return FALSE;
		}
		rData.MINUS_D.kSigrLower = key;
	}
	//기둥그룹
	if (rData.nAlwCalcType != seis_jp::kUserInput)
	{
		m_colmGroup_minus.GetWindowText(rData.MINUS_D.strNameCOLM);
		if (rData.MINUS_D.strNameCOLM == COMBO_DEFAULT)
			return FALSE;
	}
	//발생 스텝(δy)
	if (rData.nAlwCalcType == seis_jp::kAnalysisResult)
	{
		m_stepDeltaY_minus.GetWindowText(str);
		if (str == COMBO_DEFAULT)
			return FALSE;
		rData.MINUS_D.nStepDeltaY = _wtof(str);
	}
	//길이
	if (m_length_minus.IsWindowEnabled() == TRUE) {
		m_length_minus.GetWindowText(str);
		rData.MINUS_D.dLengthDeltaVar = _wtof(str);
	}

	return TRUE;
}

BOOL CSeisCvlJpSetInvestSirdDlg::CheckData(const T_SIRD_D& data)
{
	CString str;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Exist(data.kSipa) == FALSE) {
		str = _LS(IDS_IDD_TM_SIRD_MSG_NOT_EXIST_SIPA_VALUE);
		return FALSE;
	}
	
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Exist(data.kSigr) == FALSE) {
		str = _LS(IDS_IDD_TM_SIRD_MSG_NOT_EXIST_SIGR_VALUE);
		return FALSE;
	}

	if (data.nDirDisp == seis_jp::kUser_Dir) {
		if (data.dAngle <= 0 || data.dAngle >= 360) {
			AfxMessageBox(_LS(IDS_IDD_TM_SIRD_MSG_INVALID_VALUE_ANGLE));
			return FALSE;
		}
	}

	if (data.bTimeStepRange)
	{
		for (int i = 0; i < data.aTimeRange.GetSize(); i++)
		{
			T_SIRD_TIME_RANGE_D timeD = data.aTimeRange[i];
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

T_SIMD_K CSeisCvlJpSetInvestSirdDlg::GetExistingDataKey(const T_SIRD_D& newData)
{
	CString str;
	//조사 케이스 + 조사 대상 조합으로 기존에 등록되어 있는게 있는지 체크
	T_SIRD_K key;
	T_SIRD_D data;

	POSITION pos = m_pDoc->m_pAttrCtrl2->GetQSird()->GetStart();
	while (pos)
	{
		m_pDoc->m_pAttrCtrl2->GetQSird()->GetNext(pos, key, data);
		if (data.kSipa == newData.kSipa && data.kSigr == newData.kSigr) {
			return key;
		}
	}

	return 0;
}

T_THCR_K CSeisCvlJpSetInvestSirdDlg::GetThcrKey(const CString& str)
{
	T_THCR_K key;
	T_THCR_D data;

	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartThcr();
	while (pos)
	{
		m_pDoc->m_pAttrCtrl->GetNextThcr(pos, key, data);
		if (data.GroupName == str)
			return key;
	}

	return 0;
}
// CSeisCvlJpSetInvestSimdDlg_H29.cpp : implementation file
//
#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCvlJpSetInvestSimdDlg.h"

#include "..\wg_base\TestEnvMgr.h"

#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"

void CSeisCvlJpSetInvestSimdDlg::OnInitDialog_H29()
{
	InitControl();
	InitCaseNameCtrl();
	InitGroupNameCtrl();
	InitDirectionCtrl();
	UpdateAngleCtrl();
	UpdateCalcTypeCtrl();
	InitAnalysisCaseCtrl(m_this_plus);
	InitAnalysisCaseCtrl(m_this_minus);
	InitGroupLowNameCtrl(m_groupName_plus, m_nMatlType_plus);
	InitGroupLowNameCtrl(m_groupName_minus, m_nMatlType_minus);
	UpdateAnalysisCaseCtrl();
	InitColmGroupCtrl(m_groupName_plus, m_colmGroup_plus);
	InitColmGroupCtrl(m_groupName_minus, m_colmGroup_minus);
	UpdateGroupLowNameCtrl(m_groupName_plus);
	UpdateGroupLowNameCtrl(m_groupName_minus);
	UpdateColmGroupGroupCtrl();
	UpdateStepDeltaYCtrl();
	InitStepDeltaLsCtrl();
	UpdateStepDeltaLsCtrl();
	InitStepDeltaACtrl();
	UpdateStepDeltaACtrl();
	UpdateLoadCaseStepCtrl(m_this_plus, m_stepDeltaLs_plus);
	UpdateLoadCaseStepCtrl(m_this_minus, m_stepDeltaLs_minus);
	UpdateConsiderAlpha();
	UpdateAlphaCtrl();
	// 기둥그룹등의 활성비활성 상태가 위 Init, Update 함수 등에서 설정된 
	// 활성, 비활성 상태대로 설정이 되지 않아 OnChangedCalcTypeCmb() 함수를 한번 호출해 주어 해결함.
	OnChangedCalcTypeCmb();
}

void CSeisCvlJpSetInvestSimdDlg::OnChangedCasePulsCmb_H29()
{
	UpdateLoadCaseStepCtrl(m_this_plus, m_stepDeltaLs_plus);
}

void CSeisCvlJpSetInvestSimdDlg::OnChangedCaseMinusCmb_H29()
{
	UpdateLoadCaseStepCtrl(m_this_minus, m_stepDeltaLs_minus);
}

void CSeisCvlJpSetInvestSimdDlg::InitColmGroupCtrl_H29(const MComboBox& groupNameCmb, MComboBox& rComboBox)
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

void CSeisCvlJpSetInvestSimdDlg::UpdateGroupLowNameCtrl_H29(MComboBox& rComboBox, BOOL bReadOnly/*=FALSE*/)
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
		bEnable = dSigr.nSubType == T_SIGR_D::kUpper;
	}
	
	rComboBox.EnableWindow(bEnable);
}

void CSeisCvlJpSetInvestSimdDlg::UpdateColmGroupGroupCtrl_H29()
{
	CString strCalcType;
	m_calcType.GetWindowText(strCalcType);
	if (strCalcType == COMBO_CALC_TYPE_INPUT_VALUE) {
		//δMax 를 알기위해 기둥하단으로 부터 관성력 작용위치까지의 거리 h를 구하기 위해 유저입력이 선택되더라도 활성화 함.
		m_colmGroup_plus.EnableWindow(TRUE);
		m_colmGroup_minus.EnableWindow(FALSE);// - 방향은 항상 비활성 + 방향 값을 따라감
		return;
	}

	m_colmGroup_plus.EnableWindow(TRUE);
	m_colmGroup_minus.EnableWindow(FALSE);// - 방향은 항상 비활성 + 방향 값을 따라감
}

void CSeisCvlJpSetInvestSimdDlg::UpdateStepDeltaYCtrl_H29()
{
	CString str;
	m_calcType.GetWindowText(str);

	BOOL bEnable = FALSE;
	if (str == COMBO_CALC_TYPE_FROM_RESULT)	bEnable = FALSE;
	else if (str == COMBO_CALC_TYPE_FROM_MPHI_Dy || str == COMBO_CALC_TYPE_FROM_MPHI_Dpy) bEnable = FALSE;
	else if (str == COMBO_CALC_TYPE_INPUT_VALUE)	bEnable = FALSE;

	m_stepDeltaY_plus.EnableWindow(bEnable);
	m_stepDeltaY_minus.EnableWindow(bEnable);
}

void CSeisCvlJpSetInvestSimdDlg::InitStepDeltaLsCtrl_H29()
{
	CString strGroup;
	m_groupName.GetWindowText(strGroup);

	T_SIGR_D dSigr;
	T_SIGR_K kSigr = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strGroup);
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(kSigr, dSigr) == FALSE)
		return;

	CString strCase;
	m_caseName.GetWindowText(strCase);

	T_SIPA_D sipaD;
	T_SIPA_K kSipa = m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKey(strCase);
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(kSipa, sipaD) == FALSE)
		return;

	if (sipaD.nLimitType == T_SIPA_D::kLimitState1 && dSigr.nSubType == T_SIGR_D::kLower) {
		GetDlgItem(IDC_TM_SIMD_CALC_TYPE_STEP_DLS_STC)->SetWindowText(_LS(IDS_IDD_TM_SIMD_IDC_TM_SIMD_CALC_TYPE_STEP_DLS_STC_yE));
		GetDlgItem(IDC_TM_SIMD_CALC_TYPE_STEP_DLS_STC2)->SetWindowText(_LS(IDS_IDD_TM_SIMD_IDC_TM_SIMD_CALC_TYPE_STEP_DLS_STC_yE));
	}
	else {
		GetDlgItem(IDC_TM_SIMD_CALC_TYPE_STEP_DLS_STC)->SetWindowText(_LS(IDS_IDD_TM_SIMD_IDC_TM_SIMD_CALC_TYPE_STEP_DLS_STC_ls));
		GetDlgItem(IDC_TM_SIMD_CALC_TYPE_STEP_DLS_STC2)->SetWindowText(_LS(IDS_IDD_TM_SIMD_IDC_TM_SIMD_CALC_TYPE_STEP_DLS_STC_ls));
	}
}

void CSeisCvlJpSetInvestSimdDlg::InitStepDeltaACtrl_H29()
{
	T_SIMD_UNIT* pUnit = &(CUnitCtrl::m_SIMD_UNIT);

	CString strUnit;
	CUnitCtrl::GetUnitSystem(pUnit->Length, strUnit);

	CString strMsg;
	strMsg.Format(_LS(IDS_IDD_TM_SIMD_IDC_TM_SIMD_CALC_TYPE_UNIT_DA_STC_lsd), strUnit);

	GetDlgItem(IDC_TM_SIMD_CALC_TYPE_UNIT_DA_STC)->SetWindowText(strMsg);
	GetDlgItem(IDC_TM_SIMD_CALC_TYPE_UNIT_DA_STC2)->SetWindowText(strMsg);
}

void CSeisCvlJpSetInvestSimdDlg::UpdateAlphaCtrl_H29()
{
	m_alpha.EnableWindow(FALSE);

	CString strCase;
	m_caseName.GetWindowText(strCase);

	T_SIPA_K kSipa = m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKey(strCase);

	T_SIPA_D sipaD;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(kSipa, sipaD) == FALSE)
		return;

	if (sipaD.nLimitType != T_SIPA_D::kLimitState2 && sipaD.nLimitType != T_SIPA_D::kLimitState3)
		return;

	CString strGroup;
	m_groupName.GetWindowText(strGroup);

	T_SIGR_D dSigr;
	T_SIGR_K kSigr = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(strGroup);
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(kSigr, dSigr) == FALSE)
		return;

	CArray<T_SIGR_LOWER_BEAR_D, T_SIGR_LOWER_BEAR_D&> bearArray;

	if (dSigr.nSubType == T_SIGR_D::kLower) {
		GetBearingArray(m_pDoc, dSigr.strName, bearArray);
	}
	else {
		CString strGroup_plus, strGroup_minus;
		if (m_groupName_plus.IsWindowEnabled() == TRUE)
			m_groupName_plus.GetWindowText(strGroup_plus);
		if (m_groupName_minus.IsWindowEnabled() == TRUE)
			m_groupName_minus.GetWindowText(strGroup_minus);

		if (strGroup_plus.IsEmpty() == false)
			GetBearingArray(m_pDoc, strGroup_plus, bearArray);

		if (strGroup_minus.IsEmpty() == false) {
			if (strGroup_plus.IsEmpty() == true)
				GetBearingArray(m_pDoc, strGroup_minus, bearArray);
			else if (strGroup_plus != strGroup_minus)
				GetBearingArray(m_pDoc, strGroup_minus, bearArray);
		}
	}

	BOOL bSeismicIsolation = FALSE;
	for (int i = 0; i < bearArray.GetCount(); i++)
	{
		if (bearArray[i].nType == T_SIGR_LOWER_BEAR_D::kSeismicIsolation) {
			bSeismicIsolation = TRUE;
			break;
		}
	}

	m_alpha.EnableWindow(bSeismicIsolation);

	if (bSeismicIsolation == TRUE) {
		GetDlgItem(IDC_TM_SIMD_SAFETY_COEFFICIENT_STC)->SetWindowText(_LS(IDS_IDD_TM_SIDP_IDC_TM_SIDP_ALLO_DISP_PLAS_RATE_ALPHA_M_STC));
		m_alpha.SetWindowText(_T("2.0"));
	}
	else {
		GetDlgItem(IDC_TM_SIMD_SAFETY_COEFFICIENT_STC)->SetWindowText(_LS(IDS_IDD_TM_SIDP_IDC_TM_SIDP_ALLO_DISP_PLAS_RATE_ALPHA_STC));
		m_alpha.SetWindowText(_T(""));
	}
}

void CSeisCvlJpSetInvestSimdDlg::UpdateConsiderAlpha_H29()
{
	CString strCalcType;
	m_calcType.GetWindowText(strCalcType);

	CButton* pCheckBtn = (CButton*)GetDlgItem(IDC_TM_SIMD_SAFETY_COEFFICIENT_CHK);
	if (strCalcType != COMBO_CALC_TYPE_FROM_RESULT) {
		pCheckBtn->SetCheck(FALSE);
		pCheckBtn->EnableWindow(FALSE);
		return;
	}

	pCheckBtn->SetWindowText(_LS(IDS_IDD_TM_SIMD_IDC_TM_SIMD_SAFETY_COEFFICIENT_CHK_H29));

	CString strCase;
	m_caseName.GetWindowText(strCase);

	T_SIPA_K kSipa = m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKey(strCase);

	T_SIPA_D sipaD;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(kSipa, sipaD) == FALSE) {
		pCheckBtn->SetCheck(FALSE);
		pCheckBtn->EnableWindow(FALSE);
		return;
	}

	if (sipaD.nLimitType != T_SIPA_D::kLimitState2 && sipaD.nLimitType != T_SIPA_D::kLimitState3) {
		pCheckBtn->SetCheck(FALSE);
		pCheckBtn->EnableWindow(FALSE);
		return;
	}

	if (strCalcType != COMBO_CALC_TYPE_FROM_RESULT)
		return;

	pCheckBtn->EnableWindow(TRUE);
}

BOOL CSeisCvlJpSetInvestSimdDlg::UddateControlByCalcTypeAuto_H29(const UINT/*QIehp::eCom*/& comp,
																const UINT/*MPhiProp::eDir*/& dir,
																const MComboBox& thisCmb,
																OUT MComboBox& groupNameCmb,
																OUT MComboBox& colmGroupCmb,
																OUT MComboBox& stepDeltaYCmb,
																OUT MComboBox& stepDeltaLsCmb)
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
	if (pResult->GetUltimateStepForSeismic(kSigr, kThis, comp, dir, data) == FALSE) {
		groupNameCmb.SetCurSel(0);
		colmGroupCmb.SetCurSel(0);
		stepDeltaYCmb.SetCurSel(0);
		stepDeltaLsCmb.SetCurSel(0);
		return FALSE;
	}

	T_SIDC_D sidcD; sidcD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetQSidc()->Get(sidcD);

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
	stepDeltaYCmb.SetWindowText(_T(""));

	//δls 스텝
	CString strLsStep; strLsStep.Format(_T("%d"), data.nUltimateStep);
	iIndex = stepDeltaLsCmb.FindStringExact(-1, strLsStep);
	if (iIndex >= 0) {
		stepDeltaLsCmb.SetCurSel(iIndex);
	}
	else {
		ASSERT(FALSE);
		stepDeltaLsCmb.SetCurSel(0);
	}

	return TRUE;
}

BOOL CSeisCvlJpSetInvestSimdDlg::DlgToData_H29(OUT T_SIMD_D& rData)
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

	//내진성능 한계변위 계산방식
	m_calcType.GetWindowText(str);
	if (str == COMBO_CALC_TYPE_FROM_RESULT)
		rData.nAlwCalcType = seis_jp::kAnalysisResult;
	else if (str == COMBO_CALC_TYPE_FROM_MPHI_Dy)
		rData.nAlwCalcType = seis_jp::kMphi_Dy;
	else if (str == COMBO_CALC_TYPE_FROM_MPHI_Dpy)
		rData.nAlwCalcType = seis_jp::kMphi_Dpy;
	else if (str == COMBO_CALC_TYPE_INPUT_VALUE)
		rData.nAlwCalcType = seis_jp::kUserInput;

	//////////////// (+) 방향 내진성능 한계 변위 조건 ////////////////
	rData.PLUS_D.nDirAnal = seis_jp::kPlus;
	//해석 케이스
	if (rData.nAlwCalcType == seis_jp::kAnalysisResult) {
		m_this_plus.GetWindowText(str);
		key = m_pDoc->m_pAttrCtrl->GetThisKey(str);
		if (m_pDoc->m_pAttrCtrl->ExistThis(key) == TRUE) rData.PLUS_D.kThis = key;
	}
	//한계상태 시의 교각
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
	//if (rData.nAlwCalcType == seis_jp::kAnalysisResult)
	//{
		//m_stepDeltaY_plus.GetWindowText(str);
		//if (str == COMBO_DEFAULT)
		//	return FALSE;
		//rData.PLUS_D.nStepDeltaY = _wtof(str);
	//}
	//발생 스텝(δu, δls, δyE)
	if (rData.nAlwCalcType == seis_jp::kAnalysisResult)
	{
		m_stepDeltaLs_plus.GetWindowText(str);
		if (str == COMBO_DEFAULT)
			return FALSE;
		rData.PLUS_D.nStepDeltaVar = _wtof(str);
	}
	//직접입력 길이(δa, δyEd, δlsd)
	if (rData.nAlwCalcType == seis_jp::kUserInput) {
		m_lengthDeltaA_plus.GetWindowText(str);
		rData.PLUS_D.dLengthDeltaVar = _wtof(str);
	}
	else {
		rData.PLUS_D.dLengthDeltaVar = -1;
	}
	//////////////// (-) 방향 내진성능 한계 변위 조건 ////////////////
	rData.MINUS_D.nDirAnal = seis_jp::kMinus;
	//해석 케이스
	if (rData.nAlwCalcType == seis_jp::kAnalysisResult) {
		m_this_minus.GetWindowText(str);
		key = m_pDoc->m_pAttrCtrl->GetThisKey(str);
		if (m_pDoc->m_pAttrCtrl->ExistThis(key) == TRUE) rData.MINUS_D.kThis = key;
	}
	//한계상태 시의 교각
	if (rData.nAlwCalcType != seis_jp::kUserInput) {
		m_groupName_minus.GetWindowText(str);
		key = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(str);
		if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Exist(key) == FALSE)
			return FALSE;
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
	//if (rData.nAlwCalcType == seis_jp::kAnalysisResult) {
		//m_stepDeltaY_minus.GetWindowText(str);
		//if (str == COMBO_DEFAULT)
		//	return FALSE;
		//rData.MINUS_D.nStepDeltaY = _wtof(str);
	//}
	//발생 스텝(δu, δls, δyE)
	if (rData.nAlwCalcType == seis_jp::kAnalysisResult) {
		m_stepDeltaLs_minus.GetWindowText(str);
		if (str == COMBO_DEFAULT)
			return FALSE;
		rData.MINUS_D.nStepDeltaVar = _wtof(str);
	}

	//직접입력 길이(δa, δyEd, δlsd)
	if (rData.nAlwCalcType == seis_jp::kUserInput) {
		m_lengthDeltaA_minus.GetWindowText(str);
		rData.MINUS_D.dLengthDeltaVar = _wtof(str);
	}
	else {
		rData.MINUS_D.dLengthDeltaVar = -1;
	}

	//안전 계수 α
	if (m_alpha.IsWindowEnabled() == TRUE) {
		m_alpha.GetWindowText(str);
		rData.dAlpha = _wtof(str);
	}
	else {
		rData.dAlpha = -1;
	}
	//기초변위 영향분에 대해 안전계수 고려
	if (GetDlgItem(IDC_TM_SIMD_SAFETY_COEFFICIENT_CHK)->IsWindowEnabled() == TRUE)
		rData.bConsiderAlpha = m_considerAlpha;
	else
		rData.bConsiderAlpha = FALSE;

	return TRUE;
}
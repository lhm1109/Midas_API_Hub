#include "stdafx.h"
#include "SeismicDesignSisrBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ThisInfo.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_MATL.h"
#include "..\wg_db\DB_QUERY_SECT.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"

#include "BrdgSeisEval_JP.h"

P_INPUT::P_INPUT(const T_SISR_K& sisrKey)
{
	bValidValue = FALSE;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == nullptr) { ASSERT(pDoc); return; }

	if (pDoc->m_pAttrCtrl2->GetQSisr()->Get(sisrKey, tSisr) == TRUE) { kSisr = sisrKey; } else { return; };
	if (pDoc->m_pAttrCtrl2->GetQSipa()->Get(tSisr.kSipa, tSipa) == TRUE) { kSipa = tSisr.kSipa; } else { return; };
	if (pDoc->m_pAttrCtrl2->GetQSigr()->Get(tSisr.kSigr, tSigr) == TRUE) { kSigr = tSisr.kSigr; } else { return; };

	switch (tSigr.nSubType)
	{
	case T_SIGR_D::kUser:
		if (!pDoc->m_pAttrCtrl2->GetQSigrUser()->Get(tSisr.kSigr,  tUser)) { ASSERT(FALSE); };
		break;
	case T_SIGR_D::kUpper:
		if (!pDoc->m_pAttrCtrl2->GetQSigrUpper()->Get(tSisr.kSigr, tUpper)) { ASSERT(FALSE); };
	break;
	case T_SIGR_D::kLower:
		if (!pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(tSisr.kSigr, tLower)) { ASSERT(FALSE); };
	break;
	default:
		ASSERT(FALSE);
		return;
	}
	bValidValue = TRUE;
}

P_INPUT::~P_INPUT()
{
}

CSeismicDesignSisrBase::CSeismicDesignSisrBase() : CSeismicDesignBase()
{
}

CSeismicDesignSisrBase::~CSeismicDesignSisrBase()
{
}

void CSeismicDesignSisrBase::CalcForce(const T_ELEM_K& kElem, const T_THIS_K_LIST& aThisK, const int nDirShear, OUT T_SISR_EVAL_LOWER& rEval)
{
	const int nCompIdxShear = [nDirShear]()
	{
		switch (nDirShear)
		{
		case seis_jp::kY_Dir: return STRB_SHEAR_Y_I;
		case seis_jp::kZ_Dir: return STRB_SHEAR_Z_I;
		default: ASSERT(0); return 2;
		}
	}();

	const int nCompIdxMoment = [nDirShear]()
	{
		switch (nDirShear)
		{
		case seis_jp::kY_Dir: return STRB_MOMENT_Z_I;
		case seis_jp::kZ_Dir: return STRB_MOMENT_Y_I;
		default: ASSERT(0); return 4;
		}
	}();

	auto L_GetTimeIndex = [](const CArray<double, double>& arTimeStep, const double& dTimeStep, OUT int& nTimeIndex)
	{
		int nMostSimilarIdx = 0;
		double dMostSimilarVal = DBL_MAX;
		for (int i = 0; i < arTimeStep.GetCount(); i++)
		{
			if (dgn::EQ(arTimeStep[i], dTimeStep) == true) {
				nTimeIndex = i;
				return;
			}

			double dValDiff = abs(dTimeStep - arTimeStep[i]);
			if (dValDiff < dMostSimilarVal) {
				nMostSimilarIdx = i;
				dMostSimilarVal = dValDiff;
			}
		}
		nTimeIndex = nMostSimilarIdx;
	};

	auto L_CalcShearAvg = [](T_SISR_EVAL_LOWER_BASE& rEvalBase)
	{
		double dSum = 0.0;
		int nSize = rEvalBase.adShear.GetSize();
		for (int i = 0; i < nSize; ++i)
			dSum += rEvalBase.adShear[i];
		rEvalBase.dShear = dgn::SafeDiv(dSum, (double)nSize);

		dSum = 0.0;
		nSize = rEvalBase.adMoment.GetSize();
		for (int i = 0; i < nSize; ++i)
			dSum += rEvalBase.adMoment[i];
		rEvalBase.dMoment = dgn::SafeDiv(dSum, (double)nSize);
	};

	T_ELEM_D ElemD;
	if (!m_pDoc->m_pAttrCtrl->GetElem(kElem, ElemD)) { ASSERT(0); return; }

	rEval.PosI.kNode = ElemD.elnod[0];
	rEval.PosJ.kNode = ElemD.elnod[1];

	//요소 1개당 3개의 하중 케이스에 대한 최대, 최소 전단력 값을 가져오고 평균을 구한다.
	rEval.PosI.RsltMax.adShear.RemoveAll();
	rEval.PosI.RsltMin.adShear.RemoveAll();
	rEval.PosJ.RsltMax.adShear.RemoveAll();
	rEval.PosJ.RsltMin.adShear.RemoveAll();
	T_STRB_D max[2];
	T_STRB_D min[2];
	const int nSize = aThisK.GetSize();
	for (int i = 0; i < nSize; ++i)
	{
		const T_THIS_K kThis = aThisK[i];
		m_pDoc->m_pPostCtrl->SelectThis(kThis);

		CArray<double, double> arTimeStep;
		m_pDoc->m_pAttrCtrl->GetTimeStep(kThis, arTimeStep);

		max[0].Initialize();
		max[1].Initialize();
		min[0].Initialize();
		min[1].Initialize();

		m_pDoc->m_pPostCtrl->GetThisInfo()->GetStrbWithTime(kElem, &max[0], &min[0], 0, &max[1], &min[1], 0);

		rEval.PosI.RsltMax.adShear.Add(max[0].dblForce[STRB_POSI_I][nCompIdxShear]);
		rEval.PosI.RsltMin.adShear.Add(min[0].dblForce[STRB_POSI_I][nCompIdxShear]);
		rEval.PosJ.RsltMax.adShear.Add(max[0].dblForce[STRB_POSI_J][6 + nCompIdxShear]);
		rEval.PosJ.RsltMin.adShear.Add(min[0].dblForce[STRB_POSI_J][6 + nCompIdxShear]);

		double dTimeStepMax_I = max[1].dblForce[STRB_POSI_I][nCompIdxShear];
		double dTimeStepMin_I = min[1].dblForce[STRB_POSI_I][nCompIdxShear];
		double dTimeStepMax_J = max[1].dblForce[STRB_POSI_J][6 + nCompIdxShear];
		double dTimeStepMin_J = min[1].dblForce[STRB_POSI_J][6 + nCompIdxShear];

		int nTimeIdxMax_I=0, nTimeIdxMin_I=0, nTimeIdxMax_J=0, nTimeIdxMin_J=0;
		L_GetTimeIndex(arTimeStep, dTimeStepMax_I, nTimeIdxMax_I);
		L_GetTimeIndex(arTimeStep, dTimeStepMin_I, nTimeIdxMin_I);
		L_GetTimeIndex(arTimeStep, dTimeStepMax_J, nTimeIdxMax_J);
		L_GetTimeIndex(arTimeStep, dTimeStepMin_J, nTimeIdxMin_J);

		T_STRB_D max_i, min_i, max_j, min_j;
		if (m_pDoc->m_pPostCtrl->GetStrbHistory(nTimeIdxMax_I, kElem, max_i) == TRUE)
			rEval.PosI.RsltMax.adMoment.Add(max_i.dblForce[STRB_POSI_I][nCompIdxMoment]);
		if (m_pDoc->m_pPostCtrl->GetStrbHistory(nTimeIdxMin_I, kElem, min_i) == TRUE)
			rEval.PosI.RsltMin.adMoment.Add(min_i.dblForce[STRB_POSI_I][nCompIdxMoment]);
		if (m_pDoc->m_pPostCtrl->GetStrbHistory(nTimeIdxMax_J, kElem, max_j) == TRUE)
			rEval.PosJ.RsltMax.adMoment.Add(max_j.dblForce[STRB_POSI_J][6 + nCompIdxMoment]);
		if (m_pDoc->m_pPostCtrl->GetStrbHistory(nTimeIdxMin_J, kElem, min_j) == TRUE)
			rEval.PosJ.RsltMin.adMoment.Add(min_j.dblForce[STRB_POSI_J][6 + nCompIdxMoment]);
	}

	L_CalcShearAvg(rEval.PosI.RsltMax);
	L_CalcShearAvg(rEval.PosI.RsltMin);
	L_CalcShearAvg(rEval.PosJ.RsltMax);
	L_CalcShearAvg(rEval.PosJ.RsltMin);
}

void CSeismicDesignSisrBase::CalcForce(const T_ELEM_K& kElem, const T_THIS_K_LIST& aThisK, const int nDirShear, OUT T_SISR_EVAL_ROAD3& rEval)
{
	const int nCompIdxShear = [nDirShear]()
	{
		switch (nDirShear)
		{
		case seis_jp::kY_Dir: return STRB_SHEAR_Y_I;
		case seis_jp::kZ_Dir: return STRB_SHEAR_Z_I;
		default: ASSERT(0); return 2;
		}
	}();

	const int nCompIdxMoment = [nDirShear]()
	{
		switch (nDirShear)
		{
		case seis_jp::kY_Dir: return STRB_MOMENT_Z_I;
		case seis_jp::kZ_Dir: return STRB_MOMENT_Y_I;
		default: ASSERT(0); return 4;
		}
	}();

	auto L_CalcShearAvg = [](T_SISR_EVAL_ROAD3_BASE& rEvalBase)
	{
		double dSum = 0.0;
		int nSize = rEvalBase.Forc.adShear.GetSize();
		for (int i = 0; i < nSize; ++i)
			dSum += abs(rEvalBase.Forc.adShear[i]);

		rEvalBase.Forc.dShear = dgn::SafeDiv(dSum, (double)nSize);
		rEvalBase.Forc.adShearh.Copy(rEvalBase.Forc.adShear); /// taper가 없으므로, 같은 값 할당.

		dSum = 0.0;
		nSize = rEvalBase.Forc.adMoment.GetSize();
		for (int i = 0; i < nSize; ++i)
			dSum += rEvalBase.Forc.adMoment[i];

		rEvalBase.Forc.dMoment = dgn::SafeDiv(dSum, (double)nSize);
		rEvalBase.Forc.adMomenth.Copy(rEvalBase.Forc.adMoment); /// taper가 없으므로, 같은 값 할당.
	};

	T_ELEM_D ElemD;
	if (!m_pDoc->m_pAttrCtrl->GetElem(kElem, ElemD)) { ASSERT(0); return; }

	rEval.PosI.NodeK = ElemD.elnod[0];
	rEval.PosJ.NodeK = ElemD.elnod[1];

	//요소 1개당 3개의 하중 케이스에 대한 최대, 최소 전단력 값을 가져오고 평균을 구한다.
	rEval.PosI.RsltMax.Forc.Initialize();
	rEval.PosI.RsltMin.Forc.Initialize();
	rEval.PosJ.RsltMax.Forc.Initialize();
	rEval.PosJ.RsltMin.Forc.Initialize();
	T_STRB_D StrbMax;
	T_STRB_D StrbMin;
	const int nSize = aThisK.GetSize();
	for (int i = 0; i < nSize; ++i)
	{
		const T_THIS_K kThis = aThisK[i];

		T_LCOM_D LcomD;
		if (!m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_HISTORY, kThis, LcomD)) { ASSERT(0); continue; }
		m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);

		StrbMax.Initialize();
		StrbMin.Initialize();
		m_pDoc->m_pPostCtrl->GetStrbNew(kElem, &StrbMax, &StrbMin, NULL, TRUE);

		rEval.PosI.RsltMax.Forc.adShear.Add(StrbMax.dblForce[STRB_POSI_I][nCompIdxShear]);
		rEval.PosI.RsltMin.Forc.adShear.Add(StrbMin.dblForce[STRB_POSI_I][nCompIdxShear]);
		rEval.PosI.RsltMax.Forc.adMoment.Add(StrbMax.dblForce[STRB_POSI_I][nCompIdxMoment]);
		rEval.PosI.RsltMin.Forc.adMoment.Add(StrbMin.dblForce[STRB_POSI_I][nCompIdxMoment]);

		rEval.PosJ.RsltMax.Forc.adShear.Add(StrbMax.dblForce[STRB_POSI_J][nCompIdxShear + 6]);
		rEval.PosJ.RsltMin.Forc.adShear.Add(StrbMin.dblForce[STRB_POSI_J][nCompIdxShear + 6]);
		rEval.PosJ.RsltMax.Forc.adMoment.Add(StrbMax.dblForce[STRB_POSI_J][nCompIdxMoment + 6]);
		rEval.PosJ.RsltMin.Forc.adMoment.Add(StrbMin.dblForce[STRB_POSI_J][nCompIdxMoment + 6]);
	}

	L_CalcShearAvg(rEval.PosI.RsltMax);
	L_CalcShearAvg(rEval.PosI.RsltMin);
	L_CalcShearAvg(rEval.PosJ.RsltMax);
	L_CalcShearAvg(rEval.PosJ.RsltMin);
}

const BOOL CSeismicDesignSisrBase::GetSisrParamBase(const T_SISR_PARAM_POS& SisrPos, const int nSeismicForceDirection, seis_jp::eDirShear nDirShear, int nPos, double dMoment, T_SISR_PARAM_BASE& rData)
{
	const bool bForcePlus = dgn::ME0(dMoment);
	switch (nDirShear)
	{
	case seis_jp::kY_Dir://Y방향
	{
		if (nPos == k_I) {//I단
			rData = bForcePlus ? SisrPos.yTensBot : SisrPos.yTensTop;
		}
		else {//J단
			rData = bForcePlus ? SisrPos.yTensBot : SisrPos.yTensTop;
		}
	}
	break;
	case seis_jp::kZ_Dir://Z방향
	{
		if (nPos == k_I) {
			rData = bForcePlus ? SisrPos.zTensBot : SisrPos.zTensTop;
		}
		else {
			rData = bForcePlus ? SisrPos.zTensBot : SisrPos.zTensTop;
		}
	}
	break;
	default:
		{
			ASSERT(0);
			return FALSE;
		}
		break;
	}
	
	return TRUE;
}

double CSeismicDesignSisrBase::GetSigmaCk(const T_ELEM_K& kElem, const T_SISR_PARAM_BASE& param, const int& nPos)
{
	double dSigma_ck = 0.0;

	T_ELEM_D ElemD; T_MATD_D MatdD;
	if (!m_pDoc->m_pAttrCtrl->GetElem(kElem, ElemD)) { return dSigma_ck; }
	if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatdD)) { return dSigma_ck; }

	dSigma_ck = MatdD.Data1.Design.C_fc; // 콘크리트의 설계기준강도 (N/mm2)

	// 보강단면이 있는지 확인
	T_SECT_SSM_D tSectSsm;
	if (!m_pDoc->m_pAttrCtrl2->GetQSectSsm()->Get(kElem, tSectSsm)) { return dSigma_ck; }

	T_SECT_D SectD;
	if (!m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro, SectD)) { return dSigma_ck; }

	if (SectD.nStype != D_SECT_TYPE_REGULAR && SectD.nStype != D_SECT_TYPE_TAPERED) return dSigma_ck;
	if (SectD.nStype != D_SECT_TYPE_TAPERED)
		SectD.SectBefore.SectJ = SectD.SectBefore.SectI;

	// 보강단면이 없다면, 기설단면의 콘크리트 강도를 사용	
	T_MATD_REIN_D MatdReinD;
	if (!m_pDoc->m_pAttrCtrl2->GetQMatdRein()->Get(ElemD.elmat, MatdReinD)) { return dSigma_ck; }

	double dAreaSect = param.dAreaSect;
	double dAreaRein = param.dAreaRein;
// 	char chIJ = (nPos == k_I) ? 'I' : 'J';
// 	m_pDoc->m_pAttrCtrl2->GetQReinSsm()->GetArea(ElemD.elpro, tSectSsm.kReinSsm, dAreaSect, dAreaRein, chIJ);

	const double dCk1 = dSigma_ck; // 콘크리트의 설계기준강도 (N/mm2)
	const double dCk2 = MatdReinD.CoverRC.dC_Fc; // 보강단면의 콘크리트 강도 (N/mm2)

	const double dAs1 = param.dAs_sect;// 기설부 철근 단면적
	const double dAs2 = param.dAs_rein;// 보강부 철근 단면적
	const double dAc1 = dAreaSect - dAs1;// 순수 기설부 콘크리트 면적
	const double dAc2 = dAreaRein - dAs2;// 순수 보강부 콘크리트 면적

	//합성 설계강도 = (σck1*Ac1 + σck2*Ac2) / (Ac1+Ac2)
	double dNumerator = dCk1*dAc1 + dCk2*dAc2;
	double dDenominator = dAc1 + dAc2;
	dSigma_ck = dgn::SafeDiv(dNumerator, dDenominator);
	return dSigma_ck;
}

double CSeismicDesignSisrBase::GetSigmaSy(const T_ELEM_K& kElem)
{
	//횡방향 철근 강도 σsy
	double dSigma_sy = 0.0;

	T_ELEM_D ElemD; T_MATD_D MatdD;
	if (!m_pDoc->m_pAttrCtrl->GetElem(kElem, ElemD)) { return dSigma_sy; }
	if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatdD)) { return dSigma_sy; }

	dSigma_sy = min(MatdD.SubRebarData.B_fy, 345.0);// σsy : 기설 띠철근(전단보강철근) 항복강도(N/mm2), 상한치 345N/mm2

	// 보강단면이 없다면, 기설단면의 콘크리트 강도를 사용
	T_MATD_REIN_D MatdReinD;
	if (!m_pDoc->m_pAttrCtrl2->GetQMatdRein()->Get(ElemD.elmat, MatdReinD)) { return dSigma_sy; }

	T_SECT_SSM_D SectSsmD;
	T_REIN_SSM_D ReinSsmD;
	if (!m_pDoc->m_pAttrCtrl2->GetQSectSsm()->Get(kElem, SectSsmD)) { return dSigma_sy; }
	if (!m_pDoc->m_pAttrCtrl2->GetQReinSsm()->Get(SectSsmD.kReinSsm, ReinSsmD)) { return dSigma_sy; }
	if (ReinSsmD.ReinSectRC.bUse == TRUE)
		dSigma_sy = min(dSigma_sy, MatdReinD.CoverRC.dSR_Fy);
	if (ReinSsmD.ReinSectSP.bUse == TRUE)
		dSigma_sy = min(dSigma_sy, MatdReinD.CoverSP.dFy);
	
	return dSigma_sy;
}

double CSeismicDesignSisrBase::GetCc(int nSeisForcType)
{
	switch (nSeisForcType)
	{
	case T_SIPA_D::eSeismicForceType::kType1:
		return 0.6;
	case T_SIPA_D::eSeismicForceType::kType2:
		return 0.8;
	default:
		ASSERT(0);
		return 1.0;
	}
}

double CSeismicDesignSisrBase::CalcCdc(double dadprime)
{
	if (dgn::LE(dadprime, 0.5)) return 6.4;
	if (dgn::ME(dadprime, 2.5)) return 1.0;
		
	std::vector<std::pair<double, double>> vdCdc;
	vdCdc.emplace_back(std::make_pair(0.5, 6.4));
	vdCdc.emplace_back(std::make_pair(1.0, 4.0));
	vdCdc.emplace_back(std::make_pair(1.5, 2.5));
	vdCdc.emplace_back(std::make_pair(2.0, 1.6));
	vdCdc.emplace_back(std::make_pair(2.5, 1.0));

	return CMathFunc::mathInterpolateLinear(vdCdc, dadprime);
}

double CSeismicDesignSisrBase::CalcCds(double dadprime)
{
	if (dgn::MT(dadprime, 2.5)) return 1.0;
	return 1.0 * dadprime / 2.5;
}

double CSeismicDesignSisrBase::CalcSf(const int& nPos, const int& nSeisDir, const seis_jp::eDirShear& nShearDir, const T_ELEM_K& kElem, const double& dd)
{
#pragma region >> Get Data
	T_ELEM_D ElemD;	if (!m_pDoc->m_pAttrCtrl->GetElem(kElem, ElemD)) { ASSERT(FALSE); return 0.0; }
	T_SECT_D SectD; if (!m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro, SectD)) { ASSERT(FALSE); return 0.0; }
	T_MATD_REIN_D MatdReinD; if (!m_pDoc->m_pAttrCtrl2->GetQMatdRein()->Get(ElemD.elmat, MatdReinD)) { ASSERT(FALSE); return 0.0; }
	T_SECT_SSM_D SectSsmD; if (!m_pDoc->m_pAttrCtrl2->GetQSectSsm()->Get(kElem, SectSsmD)) { ASSERT(FALSE); return 0.0; }
	T_REIN_SSM_D ReinSsmD; if (!m_pDoc->m_pAttrCtrl2->GetQReinSsm()->Get(SectSsmD.kReinSsm, ReinSsmD)) { ASSERT(FALSE); return 0.0; }
	if (ReinSsmD.ReinSectCF.bUse == FALSE) { ASSERT(FALSE); return 0.0; }

	const CString& strShape = SectD.SectBefore.Shape;
	//아래 단면의 경우 보강부 단면의 두께는 LocalY 에만 값이 있으므로 LocalZ 에도 계산편의를 위해 같은 값을 넣으줌
	if (strShape == D_SECT_SHAPE_REG_SR ||
		strShape == D_SECT_SHAPE_REG_P ||
		strShape == D_SECT_SHAPE_REG_SOCT ||
		strShape == D_SECT_SHAPE_REG_OCT ||
		strShape == D_SECT_SHAPE_REG_STRK ||
		strShape == D_SECT_SHAPE_REG_TRK)
	{
		ReinSsmD.ReinSectCF.LocalZ = ReinSsmD.ReinSectCF.LocalY;
	}
#pragma endregion <<

	//σF：섬유 시트의 설계용 인장강도(N / mm2)
	auto L_GetSigma_F = [&]()
	{
		double dSigma_F = 0.0;
		if(ReinSsmD.ReinSectCF.nFormulaType == T_REIN_SSM_CF::kCalcuFormula2)			//계산 2의 경우
			dSigma_F = MatdReinD.CoverCF.dFu * 0.6;										//인장강도의 60%
		else if (ReinSsmD.ReinSectCF.nFormulaType == T_REIN_SSM_CF::kCalcuFormula1) {	//계산 1의 경우
			if (MatdReinD.CoverCF.nMainType == T_MATD_REIN_CF::eMainType::kCarbonFiber)
				dSigma_F = (MatdReinD.CoverCF.dFu * 2 / 3);								//탄소섬유 시트는 인장 강도의 2/3
			else if (MatdReinD.CoverCF.nMainType == T_MATD_REIN_CF::eMainType::kAFRP)
				dSigma_F = MatdReinD.CoverCF.dFu * 0.6;									//AFRP시트의 경우, 인장강도의 60%
			else if (MatdReinD.CoverCF.nMainType == T_MATD_REIN_CF::eMainType::kUserInput)
				dSigma_F = MatdReinD.CoverCF.dFu;
		}
		return dSigma_F;
	};
	double dSigma_F = L_GetSigma_F();
	
	//TF：섬유 시트의 두께(mm), TF=nF×tF
	// nF : 섬유 시트의 매수
	// tF : 섬유 시트의 두께(mm)
	auto L_Calc_TF = [&]()
	{
		UINT nF = 0; 
		if (nShearDir == seis_jp::kY_Dir) nF = ReinSsmD.ReinSectCF.LocalZ.nShearNum;
		else if (nShearDir == seis_jp::kZ_Dir) nF = ReinSsmD.ReinSectCF.LocalY.nShearNum;
			
		double tF = MatdReinD.CoverCF.dThickness;
		return nF * tF;
	};
	double TF = L_Calc_TF();

	//aF：섬유 시트의 폭(mm) = 기설 띠철근 간격
	auto L_Calc_aF = [&]()
	{
		T_RRSC_SSM_D sectRebar; sectRebar.Initialize();
		T_RRSC_SSM_D reinRebar; reinRebar.Initialize();
		m_pDoc->m_pAttrCtrl2->GetQRrscSsm()->GetRebarFromElem(kElem, sectRebar, reinRebar);

		if (sectRebar.nSameRebarPos == T_RRSC_SSM_D::kUnkowun) return 0.0;

		T_RBAR_STIRRUP stirrup;
		if (nPos == k_I) {
			if (sectRebar.SbarI.GetCount() < 1) return 0.0;
			stirrup = sectRebar.SbarI[0];
		}
		else if(nPos == k_J) {
			if (sectRebar.SbarJ.GetCount() < 1) return 0.0;
			stirrup = sectRebar.SbarJ[0];
		}
		else {
			return 0.0;
		}

		double aF = 0.0;
		if (nShearDir == seis_jp::kY_Dir) {
			aF = stirrup.StirrupZ.dSpacing;
		}
		else if (nShearDir == seis_jp::kZ_Dir) {
			aF = stirrup.StirrupY.dSpacing;
		}
		else {
			ASSERT(0);
			return 0.0;
		}

		return aF;
	};
	double aF = L_Calc_aF();

	//AF：섬유 시트의 단면적(mm2), aF×TF
	double AF = (aF * TF)*2;

	//Θ：전단철근 및 섬유 시트가 부재 연직 축과 이루는 각도(°), 90도로 고정
	const double dTheta = CMathFunc::ToRadian(90);
	double dSinCos = CMathFunc::mathSin(dTheta) + CMathFunc::mathCos(dTheta);

	// SF : 섬유 시트가 부담하는 전단내력(N)
	// SF = AF x σF x d(sinθ+cosθ) / 1.15aF
	double dNumerator = AF * dSigma_F * dd * dSinCos;
	double dDenominator = 1.15 * aF;
	double dSf = dgn::SafeDiv(dNumerator, dDenominator);
	return dSf;
}

void CSeismicDesignSisrBase::MakeSisrUserMap(T_SISR_K SisrK)
{
	m_mSisrUser.clear();

	CArray<T_SISR_USER_K, T_SISR_USER_K> aSisrUserK;
	m_pDoc->m_pAttrCtrl2->GetQSisrUser()->GetKeysEqualSisrKey(SisrK, aSisrUserK);

	T_SISR_USER_D SisrUserD;
	const int nUser = aSisrUserK.GetSize();
	for (int i = 0; i < nUser; ++i)
	{
		if (!m_pDoc->m_pAttrCtrl2->GetQSisrUser()->Get(aSisrUserK[i], SisrUserD)) continue;

		const T_ELEM_K ElemK = SisrUserD.kElem;
		
		m_mSisrUser.insert(std::make_pair(std::make_tuple(ElemK, 0, true), SisrUserD.I_PLUS_D));
		m_mSisrUser.insert(std::make_pair(std::make_tuple(ElemK, 0, false), SisrUserD.I_MINUS_D));
		m_mSisrUser.insert(std::make_pair(std::make_tuple(ElemK, 1, true), SisrUserD.J_PLUS_D));
		m_mSisrUser.insert(std::make_pair(std::make_tuple(ElemK, 1, false), SisrUserD.J_MINUS_D));
	}
}

bool CSeismicDesignSisrBase::GetSisrUserShfo(T_ELEM_K ElemK, int nPos, bool bForcePlus, OUT T_SISR_USER_SHFO_D& rShfoD)
{
	rShfoD.Initialize();

	auto SisrUserKey = std::make_tuple(ElemK, nPos, bForcePlus);
	if (m_mSisrUser.find(SisrUserKey) == m_mSisrUser.end())
	{
		return false;
	}

	rShfoD = m_mSisrUser[SisrUserKey];
	return true;
}

void CSeismicDesignSisrBase::MakeSisrLowerElemParamMap(T_SISR_K SisrK)
{
	m_vSisrCc10.clear();
	m_mSisrColmHeight.clear();
	m_mSisrShearSpan.clear();	

	auto L_SetCc = [this](T_SISR_K SisrK, const CArray<T_ELEM_K, T_ELEM_K>& aElemK)
	{
		const int nElem = aElemK.GetSize();
		for (int i = 0; i < nElem; ++i)
		{
			m_vSisrCc10.emplace_back(aElemK[i]);
		}
	};
	
	T_SISR_D SisrD;
	if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD)) return;

	T_SIGR_LOWER_D SigrLowerD;
	if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SisrD.kSigr, SigrLowerD)) return;

	T_SIPA_D SipaD;
	if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SisrD.kSipa, SipaD)) return;
		
	std::map<int, double> mBotLevel;

	const int nGrup = SigrLowerD.COLM_D.GetSize();
	for (int i = 0; i < nGrup; ++i)
	{
		double dBotLevel = 0.0;
		double dTopLevel = 0.0;
		GetRangeColmGroup(SigrLowerD.COLM_D[i].aInvestigatedElem, dBotLevel, dTopLevel);
		
		mBotLevel.insert(std::make_pair(i, dBotLevel));

		const int nColm = SigrLowerD.COLM_D[i].aInvestigatedElem.GetSize();
		for (int j = 0; j < nColm; ++j)
		{
			T_ELEM_K ElemK = SigrLowerD.COLM_D[i].aInvestigatedElem[j];
			m_mSisrColmHeight.insert(std::make_pair(ElemK, dTopLevel - dBotLevel));
		}
	}

	const T_SISR_AUTO_D& AutoD = SisrD.AUTO_D;
	if (AutoD.bExtraByShearSpan)
	{
		switch (AutoD.nShearSpanType)
		{
		case T_SISR_AUTO_D::kColumnLength:
			{
				m_mSisrShearSpan = m_mSisrColmHeight;				
			}			
			break;
		case T_SISR_AUTO_D::kLocInertialForce:
			{
				const T_NODE_K TopNodeK = SipaD.nSeismicForceDirection == T_SIPA_D::kAxis_Dir ? SigrLowerD.kNodeAxis : SigrLowerD.kNodePerp;
				T_NODE_D NodeD;
				double dTopLevel = m_pDoc->m_pAttrCtrl->GetNode(TopNodeK, NodeD) ? NodeD.z : 0.0;
				double dBotLevel = 0.0;
				for (int i = 0; i < nGrup; ++i)
				{
					dBotLevel = mBotLevel.find(i) == mBotLevel.end() ? 0.0 : mBotLevel[i];

					const int nColm = SigrLowerD.COLM_D[i].aInvestigatedElem.GetSize();
					for (int j = 0; j < nColm; ++j)
					{
						T_ELEM_K ElemK = SigrLowerD.COLM_D[i].aInvestigatedElem[j];
						m_mSisrShearSpan.insert(std::make_pair(ElemK, dTopLevel - dBotLevel));
					}
				}
			}			
			break;
		case T_SISR_AUTO_D::kUserInput:
			{
				for (int i = 0; i < nGrup; ++i)
				{					
					const int nColm = SigrLowerD.COLM_D[i].aInvestigatedElem.GetSize();
					for (int j = 0; j < nColm; ++j)
					{
						T_ELEM_K ElemK = SigrLowerD.COLM_D[i].aInvestigatedElem[j];
						m_mSisrShearSpan.insert(std::make_pair(ElemK, AutoD.dUserLength));
					}
				}
			}			
			break;
		default:
			ASSERT(0);
			break;
		}
	}

	if (AutoD.bApplyCc)
	{
		switch (AutoD.nTargetElements)
		{
		case T_SISR_AUTO_D::kWholeGroup:
			{
				const int nBeam = SigrLowerD.BEAM_D.GetSize();
				for (int j = 0; j < nBeam; ++j)
				{
					L_SetCc(SisrK, SigrLowerD.BEAM_D[j].aInvestigatedElem);
				}

				const int nColm = SigrLowerD.COLM_D.GetSize();
				for (int j = 0; j < nColm; ++j)
				{
					L_SetCc(SisrK, SigrLowerD.COLM_D[j].aInvestigatedElem);
				}
			}
			break;
		case T_SISR_AUTO_D::kEachElement:
			{
				L_SetCc(SisrK, AutoD.aCcElements);
			}
			break;
		default:
			{
				ASSERT(0);
			}
			break;
		}
	}
}

void CSeismicDesignSisrBase::GetRangeColmGroup(const T_ELEM_K_LIST& aElemK, OUT double& rdBotLevel, OUT double& rdTopLevel)
{
	rdBotLevel = 0.0;
	rdTopLevel = 0.0;

	const int nElem = aElemK.GetSize();
	if (nElem < 1) { return; }

	double dMax = -1.0e+28;
	double dMin =  1.0e+28;

	T_ELEM_D ElemD;
	T_NODE_D NodeD;	
	for (int i = 0; i < nElem; ++i)
	{
		if (!m_pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD)) continue;

		for (int j = 0; j < D_ELEM_MAXNOD; ++j)
		{
			if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[j], NodeD)) continue;

			dMax = max(dMax, NodeD.z);
			dMin = min(dMin, NodeD.z);
		}	
	}

	rdBotLevel = dMin;
	rdTopLevel = dMax;
}

bool CSeismicDesignSisrBase::IsCcUse10(T_ELEM_K ElemK)
{
	return find(m_vSisrCc10.begin(), m_vSisrCc10.end(), ElemK) != m_vSisrCc10.end() ? true : false;
}

double CSeismicDesignSisrBase::GetColmHeight(T_ELEM_K ElemK)
{
	//전단스팬비에 할증 옵션이 켜져 있으면, 전단스팬비를 반환
	if(m_mSisrShearSpan.find(ElemK) != m_mSisrShearSpan.end())
		return m_mSisrShearSpan[ElemK];

	return m_mSisrColmHeight.find(ElemK) == m_mSisrColmHeight.end() ? 0.0 : m_mSisrColmHeight[ElemK];
}

double CSeismicDesignSisrBase::GetShearSpan(T_ELEM_K ElemK)
{
	return m_mSisrShearSpan.find(ElemK) == m_mSisrShearSpan.end() ? 0.0 : m_mSisrShearSpan[ElemK];
}

BOOL CSeismicDesignSisrBase::SetResultRebarCut(const T_SISR_K& kSisr)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP();
	if (!pResult) { ASSERT(0); return FALSE; }

	P_INPUT input(kSisr); if (input.IsValidValue() == FALSE) { ASSERT(FALSE); return FALSE; }

	auto L_GetCurInfo = [](int nForcDirection, const T_SIGR_LOWER_COLM_D& LowerColm, OUT int& rnCutPos, OUT int& rCutElemK)
	{
		switch (nForcDirection)
		{
		case T_SIPA_D::kAxis_Dir:
			{
				rnCutPos = LowerColm.nAxialLoc;
				rCutElemK = LowerColm.kAxialElem;
				return true;
			}
		case T_SIPA_D::kPerp_Dir:
			{
				rnCutPos = LowerColm.nAxisPerpLoc;
				rCutElemK = LowerColm.kAxisPerpElem;
				return true;
			}
		default:
			{
				rnCutPos = 0;
				rCutElemK = 0;
				ASSERT(0);
				return false;
			}
		}
	};

	/// 앞서 계산한 전단내력 조사결과를 단락부 결과에 저장 (할당만 할거라 progress 반영하지 않음)
	T_SIGR_D dSigr;
	if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(input.kSigr, dSigr)) { ASSERT(0); return FALSE; }

	const int nColm = input.tLower.COLM_D.GetSize();
	for (int i = 0; i < nColm; ++i)
	{
		const T_SIGR_LOWER_COLM_D& Cur = input.tLower.COLM_D[i];
		if (!Cur.bRebarCut) continue;

		int nCutPos = 0;
		int CutElemK = 0;
		if (!L_GetCurInfo(input.tSipa.nSeismicForceDirection, Cur, nCutPos, CutElemK)) { continue; }
		if (m_mMemberMap.find(CutElemK) == m_mMemberMap.end()) { ASSERT(FALSE); continue; }

		if (Cur.nDirShear == seis_jp::kYZ_Dir) {
			RunEvalRebarCut(&input, nCutPos, CutElemK, Cur.strName, seis_jp::kY_Dir);
			RunEvalRebarCut(&input, nCutPos, CutElemK, Cur.strName, seis_jp::kZ_Dir);
		}
		else {
			seis_jp::eDirShear nDirShear = m_mMemberMap[CutElemK].nDirShear;
			if (input.tSipa.nSeismicForceDirection == T_SIPA_D::kPerp_Dir) {
				nDirShear = (nDirShear == seis_jp::kY_Dir) ? seis_jp::kZ_Dir : seis_jp::kY_Dir;
			}

			RunEvalRebarCut(&input, nCutPos, CutElemK, Cur.strName, nDirShear);
		}
	}

	return TRUE;
}

void CSeismicDesignSisrBase::RunEvalRebarCut(const P_INPUT* pInput, const int& nCutPos, const int& kElem, const CString& strName, const int& nExamShearDir)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP();
	if (!pResult) { ASSERT(0); return; }

	auto L_SetMaxResult = [](const T_SISR_EVAL_LOWER_POS& evalPos, OUT T_SISR_EVAL_REBAR_CUT& rEvalCut)
	{
		const T_SISR_EVAL_LOWER_BASE& evalBase = (evalPos.RsltMax.dRatio > evalPos.RsltMin.dRatio) ? evalPos.RsltMax : evalPos.RsltMin;
		
		rEvalCut.RsltMax.dSmax = evalPos.RsltMax.dShear;
		rEvalCut.RsltMax.dPs = evalPos.RsltMax.dPs;
		rEvalCut.RsltMax.dRatio = dgn::SafeDivAbs(rEvalCut.RsltMax.dSmax, rEvalCut.RsltMax.dPs);
		rEvalCut.RsltMax.bCheckResult = dgn::MT(rEvalCut.RsltMax.dRatio, 1.0) ? FALSE : TRUE;

		rEvalCut.RsltMin.dSmax = evalPos.RsltMin.dShear;
		rEvalCut.RsltMin.dPs = evalPos.RsltMin.dPs;
		rEvalCut.RsltMin.dRatio = dgn::SafeDivAbs(rEvalCut.RsltMin.dSmax, rEvalCut.RsltMin.dPs);
		rEvalCut.RsltMin.bCheckResult = dgn::MT(rEvalCut.RsltMin.dRatio, 1.0) ? FALSE : TRUE;
	};

	T_SISR_EVAL_LOWER evalLower;
	evalLower.kSipa = pInput->kSipa;
	evalLower.kSigr = pInput->kSigr;
	evalLower.kElem = kElem;
	evalLower.nExamShearDir = nExamShearDir;
	if (!pResult->GetResultSisrLower(evalLower)) return;

	T_SISR_EVAL_REBAR_CUT evalCut;
	evalCut.kSipa = pInput->kSipa;
	evalCut.kSigr = pInput->kSigr;
	evalCut.kElem = kElem;
	evalCut.nRebarCutPos = nCutPos;
	evalCut.strSigrName = pInput->tSigr.strName;
	evalCut.strColmName = strName;
	evalCut.nSeismicForceDirection = pInput->tSipa.nSeismicForceDirection;
	evalCut.nSeismicForceType = pInput->tSipa.nSeismicForceType;
	evalCut.nExamShearDir = nExamShearDir;

	switch (nCutPos)
	{
	case T_SIGR_LOWER_COLM_D::kI:
	{
		L_SetMaxResult(evalLower.PosI, evalCut);
	}
	break;
	case T_SIGR_LOWER_COLM_D::kJ:
	{
		L_SetMaxResult(evalLower.PosJ, evalCut);
	}
	break;
	default:
		ASSERT(0);
		return;
	}

	pResult->SetResultSisrRebarCut(evalCut);
}
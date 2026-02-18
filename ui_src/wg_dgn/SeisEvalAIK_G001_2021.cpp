#include "stdafx.h"

#include <algorithm>

#include "SeisEvalAIK_G001_2021.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_MembCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\DbToolPmcv.h"
#include "..\wg_db\SectUtil.h"
#include "..\wg_db\SeisEvalResult.h"
#include "..\wg_db\SeisEvalDataTool.h"
#include "..\wg_db\PoverInfo.h"
#include "HingeResultTool.h"

#include "..\..\dgnengine\idesign\DGN_lib\DGNCompare.h"

using namespace dgn;

CSeisEvalAIK_G001_2021::CSeisEvalAIK_G001_2021(void) : CSeisEval()
{
	m_CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
	m_CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
    m_bIgnoreMomentHingeProp = FALSE;
}

CSeisEvalAIK_G001_2021::~CSeisEvalAIK_G001_2021(void)
{
}

void CSeisEvalAIK_G001_2021::SetInitPhgtPropComponentCon(UINT unDOF, BOOL bDOF, int nLoc, int nSkcvType, BOOL bSymIJ, int nInitStiff, OUT T_PHGT_D& rPhgtD)
{
	const double dCBRatio = m_HingeCalcParam.dCBRatio;
	rPhgtD.bDOF[unDOF] = bDOF;
	/// [GEN-6988] 자유도가 OFF 되어 있어도 FEAM 형태의 힌지로 설정해 달라는 요청으로 자유도에 관계없이 요 항목들은 같이 생성.
	rPhgtD.nLoc[unDOF] = nLoc;
	rPhgtD.nSkcvType[unDOF] = nSkcvType;
	rPhgtD.Skcv[unDOF].bSymmetricIJ = bSymIJ;
	rPhgtD.Skcv[unDOF].nInitStiffMethod = nInitStiff;
	
	if ( bDOF )
	{
		GetInitialHingePropCon(unDOF, dCBRatio, rPhgtD.Skcv[unDOF].FEMA);
	}
}

void CSeisEvalAIK_G001_2021::GetInitialHingePropCon(UINT unDOF, double dC, OUT T_PHGT_FEMA& rPhgtFEMA)
{
	rPhgtFEMA.bSymmetric = TRUE;
	rPhgtFEMA.bUserDefineRatio = TRUE;

	auto L_SetVal = [] (int nIdxCurv, double dFFY, double dDDY, OUT double dForceMomentRatio[2][9])
	{
		dForceMomentRatio[IDX_FFY][nIdxCurv] = dFFY;
		dForceMomentRatio[IDX_DDY][nIdxCurv] = dDDY;
	};

	L_SetVal(IDX_CURV_ME, -0.2, -1000.0, rPhgtFEMA.dForceMomentRatio);
	L_SetVal(IDX_CURV_MD, -0.2,  -999.0, rPhgtFEMA.dForceMomentRatio);
	L_SetVal(IDX_CURV_MC,  -dC,  -999.0, rPhgtFEMA.dForceMomentRatio);
	L_SetVal(IDX_CURV_MB, -1.0,    -1.0, rPhgtFEMA.dForceMomentRatio);	
	L_SetVal(IDX_CURV_A,   0.0,     0.0, rPhgtFEMA.dForceMomentRatio);
	L_SetVal(IDX_CURV_B,   1.0,     1.0, rPhgtFEMA.dForceMomentRatio);
	L_SetVal(IDX_CURV_C,    dC,   999.0, rPhgtFEMA.dForceMomentRatio);
	L_SetVal(IDX_CURV_D,   0.2,   999.0, rPhgtFEMA.dForceMomentRatio);
	L_SetVal(IDX_CURV_E,   0.2,  1000.0, rPhgtFEMA.dForceMomentRatio);
	
	switch ( unDOF )
	{
	case DOF_FX:
	case DOF_FY:
	case DOF_FZ:
		{
			rPhgtFEMA.dYieldForce[0] = 1.0;
			rPhgtFEMA.dYieldForce[1] = 1.0;
		}
		break;
	case DOF_MX:
	case DOF_MY:
	case DOF_MZ:
		{
			rPhgtFEMA.dYieldMoment[0] = 1.0;
			rPhgtFEMA.dYieldMoment[1] = 1.0;
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	rPhgtFEMA.dIO[0] = rPhgtFEMA.dIO[1] = 2.0;
	rPhgtFEMA.dLS[0] = rPhgtFEMA.dLS[1] = 4.0;
	rPhgtFEMA.dCP[0] = rPhgtFEMA.dCP[1] = 6.0;
}

BOOL CSeisEvalAIK_G001_2021::CalcFzHingePropConBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, double dRatioMuMy,
	OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ)
{
	T_BEAM_SHR_PARAM aShr[EN_BEAM_POS_NUM];
	if ( !MakeBeamShearParam(ElemK, enNLType, aShr) ) { ASSERT(0); return FALSE; }

	double adV[2]  = { 0.0, };  // 대상 step에서의 전단력.
	double adM[2]  = { 0.0, };  // 대상 step에서의 모멘트.
	double adDP[2] = { 0.0, };  // 정모멘트에 대한 항복회전각.
	double adDN[2] = { 0.0, };  // 부모멘트에 대한 항복회전각.
	GetPORsltConBeamHingeMy(PORslt, adV, adM, adDP, adDN);

	T_PDHG_CONBEAM_INFO aPdhgInfo[EN_MEMBEND_NUM];
	double adRatioMuMy[2] = { dRatioMuMy, dRatioMuMy };
	MakeBeamInfo(ElemK, enNLType, adRatioMuMy, adV, adM, aPdhgInfo);

	for ( int i=0; i<EN_MEMBEND_NUM; ++i )
	{
		const EN_BEAM_POS& enPos = GetBeamPos(static_cast<EN_MEMBEND>(i));	

		T_PHGT_FEMA& CurPhgt = i==0 ? rPhgtFemaI : rPhgtFemaJ;

		CurPhgt.bSymmetric = FALSE;
		CurPhgt.bUserDefineRatio = TRUE;
		CurPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
		SetCurveForcCtrlShear(CurPhgt.dForceMomentRatio);
		SetAccetanceCriteriaForcCtrl(CurPhgt);

		/// AIK의 전단힌지는 (+)(-)가 구분되어 계산이 되어야 하므로, 아래와 같이 고려한다.
		/// 계산에서 설계모멘트의 크기는 고려할 필요가 없으므로 부호만 부여합니다.
		const T_CON_SHR_PARAM& CurShrPos = aShr[enPos].GetParam(TRUE, 1.0);
		aPdhgInfo[i].dMu = 1.0;
		CurPhgt.dYieldForce[IDX_PLS] = CalcVnConBeam(aPdhgInfo[i], CurShrPos, aShr[enPos].nShrMethod);
		
		const T_CON_SHR_PARAM& CurShrNeg = aShr[enPos].GetParam(TRUE, -1.0);
		aPdhgInfo[i].dMu = -1.0;
		CurPhgt.dYieldForce[IDX_MNS] = CalcVnConBeam(aPdhgInfo[i], CurShrNeg, aShr[enPos].nShrMethod);
	}
	return TRUE;
}

BOOL CSeisEvalAIK_G001_2021::CalcMyHingePropConBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, double dRatioMuMy,
	OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
	double adV[2]  = { 0.0, };  // 대상 step에서의 전단력.
	double adM[2]  = { 0.0, };  // 대상 step에서의 모멘트.
	double adDP[2] = { 0.0, };  // 정모멘트에 대한 항복회전각.
	double adDN[2] = { 0.0, };  // 부모멘트에 대한 항복회전각.
	GetPORsltConBeamHingeMy(PORslt, adV, adM, adDP, adDN);

	double adRatioMuMy[2] = { dRatioMuMy, dRatioMuMy };
	T_PDHG_CONBEAM_INFO aPdhgInfo[EN_MEMBEND_NUM];
	MakeBeamInfo(ElemK, enNLType, adRatioMuMy, adV, adM, aPdhgInfo);

	aPdhgInfo[EN_MEMBEND_I].dMu = adM[0];
	aPdhgInfo[EN_MEMBEND_I].dVu = adV[0];	
	aPdhgInfo[EN_MEMBEND_J].dMu = adM[1];
	aPdhgInfo[EN_MEMBEND_J].dVu = adV[1];
	
	if ( PORslt.unAnalRsltType == EN_ANAL_ELASTIC )
	{
		auto L_GetThetay = [] (const T_PDHG_CONBEAM_INFO& Cur, double dMn)
		{
			return SafeDiv(dMn*Cur.dShearSpan, 3.0*Cur.dEc*Cur.dIe);
		};

		for ( int i=0; i<EN_MEMBEND_NUM; ++i )
		{
			T_PDHG_CONBEAM_INFO& rCurInfo = aPdhgInfo[i];
			rCurInfo.dThetayP = L_GetThetay(rCurInfo, rCurInfo.FlexBot.dMy);
			rCurInfo.dThetayN = L_GetThetay(rCurInfo, rCurInfo.FlexTop.dMy);
		}
	}
	else
	{
		aPdhgInfo[EN_MEMBEND_I].dThetayP = adDP[0];
		aPdhgInfo[EN_MEMBEND_I].dThetayN = adDN[0];
		aPdhgInfo[EN_MEMBEND_J].dThetayP = adDP[1];
		aPdhgInfo[EN_MEMBEND_J].dThetayN = adDN[1];
	}

	const double dCBRatio = m_HingeCalcParam.dCBRatio;
	const double dc = m_HingeCalcParam.dc;
	
	for ( int i=0; i<EN_MEMBEND_NUM; ++i )
	{
		T_PHGT_FEMA& rCurPhgt = (i == EN_MEMBEND_I) ? rPhgtI : rPhgtJ;

		CalcMyHingePropConBeamPos(aPdhgInfo[i], dCBRatio, dc, rCurPhgt);

		rCurPhgt.nUnloadStiffOption = D_PHGT_FEMA_UNLD_OPTION_ENRG_DISSIPATION;
		const double dDissipation = CalcEnergyDissipationFactorConBeam(aPdhgInfo[i]);
		SetPhgtEnergyDissipationOption(dDissipation, rCurPhgt.enrgDissipation);
	}

	return TRUE;
}

void CSeisEvalAIK_G001_2021::SetAccetanceCriteriaForcCtrl(OUT T_PHGT_FEMA& rPhgtD)
{
	SetAccetanceCriteria(1.0, 1.0, 1.0, rPhgtD);
}

void CSeisEvalAIK_G001_2021::SetAccetanceCriteriaConAxial(OUT T_PHGT_FEMA& rPhgtD)
{
	SetAccetanceCriteria(2.0, 4.0, 6.0, 1.0, 1.0, 1.0, rPhgtD);
}

BOOL CSeisEvalAIK_G001_2021::CalcSeisEval2ndRsltConBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_BEAM& raEval2nd)
{
	CCurUnitSaver UnitSave(TRUE);
	SetCodeUnit();

	raEval2nd.Initialize();

	T_SECT_D SectD;
	T_MATD_D MatdD;
	T_PDCM_D PdcmD;
	if ( !GetElemMatlSectCon(ElemK, SectD, MatdD, PdcmD) ) { ASSERT(0); return FALSE; }
	if ( PdcmD.bUseDefaultVal ) { return FALSE; }
		
	double adForc[2][6] = { { 0.0 }, };
	double adM1y[2] = { 0.0, };
	double adM1z[2] = { 0.0, };
	GetPORsltForc(ElemK, PolcK, nStep, nAddStepIdx, adForc, adM1y, adM1z);

	if ( !GetSeisEval2ndRsltSubConBeam(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd) ) { return FALSE; }

	raEval2nd.bEvaluate = TRUE;

	double adV[2] = { adForc[0][2], adForc[1][2] };
	double adM[2] = { adForc[0][4], adForc[1][4] };
	double adRatioMuMy[2] = { 1.0, };
	GetRatioMuMyPhgt(ElemK, adM, adRatioMuMy);

	T_PDHG_CONBEAM_INFO aInfo[EN_MEMBEND_NUM];
	if ( !MakeBeamInfo(ElemK, EN_NLANALTYPE::PO, adRatioMuMy, adV, adM, aInfo) ) { return FALSE; }

	for ( int i=0; i<EN_MEMBEND_NUM; ++i )
	{
		const T_PDHG_CONBEAM_INFO& Info = aInfo[i];
		T_EVAL2ND_BEAM_POS& rCurPos = i==EN_MEMBEND_I ? raEval2nd.EndI : raEval2nd.EndJ;

		T_PDHG_CONBEAM_PARAM ParamP;
		CalcPlasticDeformationFactors(Info, true, ParamP);

		T_PDHG_CONBEAM_PARAM ParamN;
		CalcPlasticDeformationFactors(Info, false, ParamN);

		const T_PDHG_CONBEAM_PARAM& CurParam = Info.dMu < 0.0 ? ParamN : ParamP;

		rCurPos.dVc = Info.dVc;
		rCurPos.dVs = Info.dVs;
		rCurPos.dVp = Info.dVp;
		rCurPos.dSpace = Info.dS;
		rCurPos.dd     = min(Info.FlexBot.dd, Info.FlexTop.dd);
		rCurPos.dVu    = Info.dVu;
		rCurPos.enCtrlType = CalcBeamCtrlType(Info.dVp, Info.dVc + Info.dVs);
		rCurPos.unDetail = Info.unDetail;
		rCurPos.bSeisConform = CurParam.bSeisDetail;
		rCurPos.bSeisDetail_1 = IsSeismicDetail_1(rCurPos.dSpace, rCurPos.dd);
		rCurPos.bSeisDetail_2 = IsSeismicDetail_2(rCurPos.dVs, rCurPos.dVu);
		rCurPos.dEspUL = CurParam.dEspUL;
		rCurPos.dEspCF = CurParam.dEspCF;
		rCurPos.dEspBL = CurParam.dEspBL;
		rCurPos.dEspSC = CurParam.dEspSC;
		rCurPos.dCu   = CurParam.dcu;
		rCurPos.dPhiy = CurParam.dPhiy;
		rCurPos.dLh   = CurParam.dlh;		
	}

	return TRUE;
}

BOOL CSeisEvalAIK_G001_2021::CalcSeisEval2ndRsltConBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_BEAM& raEval2nd)
{
	raEval2nd.Initialize();
	int nAddStepIdx = -1;
	return CalcSeisEval2ndRsltConBeam(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd);
}

BOOL CSeisEvalAIK_G001_2021::CalcSeisEval2ndRsltConColmAgbp(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd)
{
	SetCodeUnit();

	raEval2nd.Initialize();

	T_SECT_D SectD;
	T_MATD_D MatdD;
	T_PDCM_D PdcmD;
	if ( !GetElemMatlSectCon(ElemK, SectD, MatdD, PdcmD) ) { return FALSE; }
	if ( PdcmD.bUseDefaultVal ) return FALSE;

	T_REBC_D RebcD;
	if ( !m_pDoc->m_pAttrCtrl->GetRebcByElemK(ElemK, RebcD) ) { return FALSE; }
 
	double adForc[2][6] = { 0.0, };
	double adM1y[2] = { 0.0, };
	double adM1z[2] = { 0.0, };
	GetPORsltForc(ElemK, PolcK, nStep, nAddStepIdx, adForc, adM1y, adM1z);

	T_PHGT_D PhgtD;
	if ( !m_pDoc->m_pAttrCtrl->GetPhgtByElemK(ElemK, PhgtD) ) { return FALSE; }
	const bool bFiber = PhgtD.nDefinition == D_PHGT_DEFINITION_MPHI_DIST ? true : false;
 
  	if ( !GetSeisEval2ndRsltSubConColm(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd) )  return FALSE;
 
	raEval2nd.bEvaluate = TRUE;

	T_COLM_SHR_PARAM aShry[EN_MEMBEND_NUM];
	if ( !MakeColmShearParamPdgn(ElemK, EN_NLANALTYPE::PO, EN_DIR_Y, EN_EVALMATL_AVG, aShry) ) { ASSERT(0); return FALSE; }

	T_COLM_SHR_PARAM aShrz[EN_MEMBEND_NUM];
	if ( !MakeColmShearParamPdgn(ElemK, EN_NLANALTYPE::PO, EN_DIR_Z, EN_EVALMATL_AVG, aShrz) ) { ASSERT(0); return FALSE; }

	CSeisEvalDataTool Tool;
	double dFck = Tool.GetFc(EN_EVALMATL_AVG, PdcmD);

	T_PDHG_CONCOLM_INFO PdhgInfo;
	PdhgInfo.dfck = dFck;
    PdhgInfo.dEc  = GetEc(MatdD.Data1.Analysis.Elast, dFck);

	PdhgInfo.dfy  = Tool.GetFym(EN_EVALMATL_AVG, PdcmD);
	PdhgInfo.dfys = Tool.GetFys(EN_EVALMATL_AVG, PdcmD);
	PdhgInfo.dEs  = CalcEs();

	PdhgInfo.bCircle = [] (const CString& strShape)
	{
		if ( strShape == D_SECT_SHAPE_REG_SR ) { return true; }
		if ( strShape == D_SECT_SHAPE_REG_P ) { return true; }
		return false;
	}(SectD.SectBefore.Shape);

	PdhgInfo.dAg  = SectD.SectBefore.SectI.Stiffness.Area;
	PdhgInfo.dAst = GetRebarAs(RebcD.MainRebarName) * RebcD.nQrb;
	PdhgInfo.ddo  = RebcD.dConcCenter;
	PdhgInfo.unDetail = GetSeisDetailConColm(ElemK);

	double dShearSpany[EN_MEMBEND_NUM] = { 0.0, };
	GetShearSpanConColm(ElemK, DOF_FY, dShearSpany);
	double dShearSpanz[EN_MEMBEND_NUM] = { 0.0, };
	GetShearSpanConColm(ElemK, DOF_FZ, dShearSpanz);

	double dB = 0.0;
	double dH = 0.0;
	GetSectDim(SectD, dB, dH);

	double adDy[2] = { raEval2nd.PosI.PhneD[DOF_MY].GetD1(), raEval2nd.PosJ.PhneD[DOF_MY].GetD1() };
	double adDz[2] = { raEval2nd.PosI.PhneD[DOF_MZ].GetD1(), raEval2nd.PosJ.PhneD[DOF_MZ].GetD1() };

	_DB_PMCV_DATA Pmcvy;
	_DB_PMCV_DATA Pmcvz;
	if ( !GetPmcvData(ElemK, EN_EVALSTEP_2ND, EN_EVALMATL_AVG, Pmcvy, Pmcvz) ) { return FALSE; }

	double adThetamaxy[2] = { raEval2nd.PosI.PhneD[DOF_MY].dDisplacement, raEval2nd.PosJ.PhneD[DOF_MY].dDisplacement };
	double adThetamaxz[2] = { raEval2nd.PosI.PhneD[DOF_MZ].dDisplacement, raEval2nd.PosJ.PhneD[DOF_MZ].dDisplacement };

	const UINT unThetayType = GetPOThetayConColmAnalType();

	if ( bFiber )
	{
		CHingeResultTool Tool;
		Tool.GetPOHingeThetamaxByPos(ElemK, DOF_MY, PolcK, nStep, nAddStepIdx, adThetamaxy);
		Tool.GetPOHingeThetamaxByPos(ElemK, DOF_MZ, PolcK, nStep, nAddStepIdx, adThetamaxz);
		
		adThetamaxy[0] *= dShearSpanz[0];
		adThetamaxy[1] *= dShearSpanz[1];
		adThetamaxz[0] *= dShearSpany[0];
		adThetamaxz[1] *= dShearSpany[1];
;	}
	
	UINT unMaxLevel = EN_LEVEL_IO;
	for ( int i=0; i<EN_MEMBEND_NUM; ++i )
	{
		T_EVAL2ND_COLM_POS& rCurPos = i==EN_MEMBEND_I ? raEval2nd.PosI : raEval2nd.PosJ;
		T_COLM_SHR_PARAM CurShry = aShry[i];
		T_COLM_SHR_PARAM CurShrz = aShrz[i];

		T_PDHG_CONCOLM_DIR PdhgDiry;		
		PdhgDiry.dPu = adForc[i][DOF_FX]*(-1.0);
		PdhgDiry.dVu = adForc[i][DOF_FZ];
		PdhgDiry.dMu = adForc[i][DOF_MY];
		PdhgDiry.dB  = dB;
		PdhgDiry.dH  = dH;
		PdhgDiry.dIg = SectD.SectBefore.SectI.Stiffness.Ryy;
		PdhgDiry.dIe = PdhgDiry.dIg * GetFactorIeConColm(ElemK, EN_DIR_Y);
		PdhgDiry.dBv = CurShrz.Comm.dBv;
		PdhgDiry.ddv = CurShrz.Comm.dDv;
		PdhgDiry.dAv = CurShrz.Comm.dAsv;
		PdhgDiry.dShearSpan = dShearSpanz[i];
		PdhgDiry.dMn = bFiber ? CalcMnByAxial(Pmcvy, PdhgDiry.dPu) : fabs(rCurPos.PhneD[DOF_MY].GetP1());
		PdhgDiry.dVy = SafeDiv(PdhgDiry.dMn, PdhgDiry.dShearSpan);
		PdhgDiry.dVc = CalcVcConColm(CurShrz, PdhgDiry.dPu);
		PdhgDiry.dVs = CalcVsFram(CurShrz.Comm);
		PdhgDiry.dVn = PdhgDiry.dVc + PdhgDiry.dVs;
		
		T_PDHG_CONCOLM_DIR PdhgDirz;
		PdhgDirz.dPu = adForc[i][DOF_FX]*(-1.0);
		PdhgDirz.dVu = adForc[i][DOF_FY];;
		PdhgDirz.dMu = adForc[i][DOF_MZ];
		PdhgDirz.dB  = dH;
		PdhgDirz.dH  = dB;
		PdhgDirz.dIg = SectD.SectBefore.SectI.Stiffness.Rzz;
		PdhgDirz.dIe = PdhgDirz.dIg * GetFactorIeConColm(ElemK, EN_DIR_Z);
		PdhgDirz.dBv = CurShry.Comm.dBv;
		PdhgDirz.ddv = CurShry.Comm.dDv;
		PdhgDirz.dAv = CurShry.Comm.dAsv;
		PdhgDirz.dShearSpan = dShearSpany[i];
		PdhgDirz.dMn = bFiber ? CalcMnByAxial(Pmcvz, PdhgDirz.dPu) : fabs(rCurPos.PhneD[DOF_MZ].GetP1());
		PdhgDirz.dVy = SafeDiv(PdhgDirz.dMn, PdhgDirz.dShearSpan);
		PdhgDirz.dVc = CalcVcConColm(CurShry, PdhgDirz.dPu);
		PdhgDirz.dVs = CalcVsFram(CurShry.Comm);
		PdhgDirz.dVn = PdhgDirz.dVc + PdhgDirz.dVs;
		
		rCurPos.Diry.enCtrlType = PdhgDiry.IsDeformationControlled() ? EN_EVALCTRL_FLEX : EN_EVALCTRL_SHER;
		rCurPos.Dirz.enCtrlType = PdhgDirz.IsDeformationControlled() ? EN_EVALCTRL_FLEX : EN_EVALCTRL_SHER;

		CalcSeisEval2ndRsltConColmDir(unThetayType, PdhgInfo, CurShrz, bFiber, adDy[i], adThetamaxy[i], PdhgDiry, rCurPos.Diry, rCurPos.PhneD[DOF_MY]);
		unMaxLevel = max(unMaxLevel, rCurPos.Diry.Moment.enLevel);

		CalcSeisEval2ndRsltConColmDir(unThetayType, PdhgInfo, CurShry, bFiber, adDz[i], adThetamaxz[i], PdhgDirz, rCurPos.Dirz, rCurPos.PhneD[DOF_MZ]);
		unMaxLevel = max(unMaxLevel, rCurPos.Dirz.Moment.enLevel);	
	}

	if (bFiber)
	{
		raEval2nd.PosI.bPhne[DOF_FY] = TRUE;
		raEval2nd.PosJ.bPhne[DOF_FY] = TRUE;
		raEval2nd.PosI.bPhne[DOF_FZ] = TRUE;
		raEval2nd.PosJ.bPhne[DOF_FZ] = TRUE;

		GetSeisEval2ndRsltSubConColmPOFiberShear(ElemK, PolcK, nStep, nAddStepIdx, DOF_FY, raEval2nd.PosI.PhneD[DOF_FY], raEval2nd.PosJ.PhneD[DOF_FY]);
		GetSeisEval2ndRsltSubConColmPOFiberShear(ElemK, PolcK, nStep, nAddStepIdx, DOF_FZ, raEval2nd.PosI.PhneD[DOF_FZ], raEval2nd.PosJ.PhneD[DOF_FZ]);

		// Shear Force
		raEval2nd.PosI.Diry.Moment.enLevel_Force = raEval2nd.PosI.PhneD[DOF_FY].enLevel_Force;
		raEval2nd.PosI.Dirz.Moment.enLevel_Force = raEval2nd.PosI.PhneD[DOF_FZ].enLevel_Force;
		raEval2nd.PosJ.Diry.Moment.enLevel_Force = raEval2nd.PosJ.PhneD[DOF_FY].enLevel_Force;
		raEval2nd.PosJ.Dirz.Moment.enLevel_Force = raEval2nd.PosJ.PhneD[DOF_FZ].enLevel_Force;
	}	

	raEval2nd.enLevel = (EN_LEVEL)unMaxLevel;

	return TRUE;
}

BOOL CSeisEvalAIK_G001_2021::CalcSeisEval2ndRsltConColmAgbp(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd)
{
	raEval2nd.Initialize();
	int nAddStepIdx = -1;
	return CalcSeisEval2ndRsltConColmAgbp(ElemK, PolcK, nStep, nAddStepIdx, nSerialAgxp, raEval2nd);
}

BOOL CSeisEvalAIK_G001_2021::CalcSeisEval2ndRsltConWallAgwp(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd)
{
	CCurUnitSaver UnitSave(TRUE);
	SetCodeUnit();

	raEval2nd.Initialize();

	T_WALL_KEY WallKey;
	T_WALL_D WallD;  WallD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetWallDataFromElemK(ElemK, WallD, &WallKey) ) { return FALSE; }
	if ( !WallD.bLineWall ) { return FALSE; }

	raEval2nd.unWallKey = WallKey;

	T_THIK_D ThikD;
	T_MATD_D MatdD;
	T_PDCM_D PdcmD;
	if ( !GetElemMatlThik(ElemK, ThikD, MatdD, PdcmD) ) { return FALSE; }
	if ( PdcmD.bUseDefaultVal ) return FALSE;
	
	double adForc[2][6] = { { 0.0 }, };
	double adM1y[2] = { 0.0, };
	double adM1z[2] = { 0.0, };
	GetPORsltForc(ElemK, PolcK, nStep, nAddStepIdx, adForc, adM1y, adM1z);

	T_PHGT_D PhgtD;
	if ( !m_pDoc->m_pAttrCtrl->GetPhgtByElemK(ElemK, PhgtD) ) { return FALSE; }
	const bool bFiber = PhgtD.nDefinition == D_PHGT_DEFINITION_MPHI_DIST ? true : false;

	if ( !GetSeisEval2ndRsltSubConWall(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd) )  return FALSE;

	raEval2nd.bEvaluate = TRUE;

	T_WALL_SHR_PARAM aShrAvg[EN_MEMBEND_NUM];
	T_WALL_SHR_PARAM aShrNom[EN_MEMBEND_NUM];
	if ( !MakeWallShearParam(ElemK, EN_DIR_Z, aShrAvg) ) { return FALSE; }
	if ( !MakeWallShearParam(ElemK, EN_DIR_Z, aShrNom) ) { return FALSE; }
	SetWallForcShearParam(adForc, EN_DIR_Z, aShrAvg, aShrNom);

	_DB_PMCV_DATA Pmcvy;
	_DB_PMCV_DATA Pmcvz;
	if ( !GetPmcvData(ElemK, EN_EVALSTEP_2ND, EN_EVALMATL_AVG, Pmcvy, Pmcvz) ) { return FALSE; }

	CSeisEvalDataTool Tool;
	const double dFck = Tool.GetFc(EN_EVALMATL_NOM, PdcmD);
	const double dTw  = WallD.dThickness;
	const double dLw  = WallD.dLength;
	const double dHw  = WallD.dHeight;
	const double dAg  = dTw*dLw;
	const double dIe  = dTw * pow(dLw, 3.0) / 12.0 * GetFactorIeConWall(ElemK);
	const double dLpRatio = GetLpRatioConWall(ElemK);
	const double dLp = min(dLw * dLpRatio, dHw);
	const bool bIgnoreThetay = IsIgnoreThetayFiberWall();

	double adThetamaxy[2] = { 0.0, };
	double adThetamaxz[2] = { 0.0, };
	if ( bFiber )
	{
		CHingeResultTool Tool;
		const double dThetamaxy = Tool.GetPOHingeThetamaxAbsMax(ElemK, DOF_MY, PolcK, nStep, nAddStepIdx);
		const double dThetamaxz = Tool.GetPOHingeThetamaxAbsMax(ElemK, DOF_MZ, PolcK, nStep, nAddStepIdx);

		adThetamaxy[0] = dThetamaxy * dLp;
		adThetamaxy[1] = dThetamaxy * dLp;
		adThetamaxz[0] = dThetamaxz * dLp;
		adThetamaxz[1] = dThetamaxz * dLp;
	}
	else
	{
		T_POST_AGBP_D AgwpD;
		if ( !m_pDoc->m_pPostCtrl->GetAgwp(PolcK, nStep, nSerialAgxp, AgwpD) )
		{
			AgwpD.Initialize();
		}

		adThetamaxy[0] = fabs(AgwpD.dVal_Ry_I);
		adThetamaxy[1] = fabs(AgwpD.dVal_Ry_J);
		
		adThetamaxz[0] = fabs(AgwpD.dVal_Rz_I);
		adThetamaxz[1] = fabs(AgwpD.dVal_Rz_J);	
	}
	
	UINT unMaxLevel = EN_LEVEL_IO;
	for ( int i=0; i<EN_MEMBEND_NUM; ++i )
	{
		T_EVAL2ND_WALL_POS& rCur = i==EN_MEMBEND_I ? raEval2nd.PosI : raEval2nd.PosJ;
		EN_WALL_FORC_POS ForcPos = GetWallForcPos(static_cast<EN_MEMBEND>(i));

		const T_WALL_SHR_PARAM CurShr = aShrAvg[i];
		double dPu = adForc[ForcPos][DOF_FX]*(-1.0);

		rCur.unSeisDetail = GetSeisDetailConWall(ElemK);
		rCur.dAxialIndex  = CalcAxialIndex(dPu, dFck, dAg);
		rCur.dShearIndex  = CalcShearIndexWall(CurShr.dV, CurShr);

		const double dThetamax = fabs(adThetamaxy[i]);
		const double dEc = GetEc(MatdD.Data1.Analysis.Elast, dFck);
		const double dMn = CalcMnByAxial(Pmcvy, dPu);
		const double dThetay = bIgnoreThetay ? 0.0 : SafeDivAbs(dMn, dEc*dIe) * dLp;
		const double dThetap = CalcThetapConColm(dThetamax, dThetay);

		CalcAcceptablePlasticHingeRotationWall(rCur.unSeisDetail, rCur.dAxialIndex, rCur.dShearIndex, 
			rCur.PhneD[DOF_MY].dIO, rCur.PhneD[DOF_MY].dLS, rCur.PhneD[DOF_MY].dCP);
		
		const UINT unCurLevel = GetPerformanceLevel(dThetap, rCur.PhneD[DOF_MY].dIO, rCur.PhneD[DOF_MY].dLS, rCur.PhneD[DOF_MY].dCP);		
				
		rCur.dLp = dLp;
		rCur.dThetamax = dThetamax;
		rCur.dThetay = dThetay;
		rCur.dThetap = dThetap;
				
		rCur.PhneD[DOF_MY].enLevel = unCurLevel;		
		unMaxLevel = max(unCurLevel, unMaxLevel);
	}

	if (bFiber)
	{	
		raEval2nd.PosI.bPhne[DOF_FZ] = TRUE;
		raEval2nd.PosJ.bPhne[DOF_FZ] = TRUE;
				
		GetSeisEval2ndRsltSubConWallPOFiberShear(ElemK, PolcK, nStep, nAddStepIdx, DOF_FZ, raEval2nd.PosI, raEval2nd.PosJ);		
	}

	raEval2nd.enLevel = (EN_LEVEL)unMaxLevel;

	return TRUE;
}

BOOL CSeisEvalAIK_G001_2021::CalcSeisEval2ndRsltConWallAgwp(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd)
{
	raEval2nd.Initialize();
	int nAddStepIdx = -1;
	return CalcSeisEval2ndRsltConWallAgwp(ElemK, PolcK, nStep, nAddStepIdx, nSerialAgxp, raEval2nd);
}

BOOL CSeisEvalAIK_G001_2021::CalcSeisEval2ndRsltConColmFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd)
{
	SetCodeUnit();

	raEval2nd.Initialize();

	T_SECT_D SectD;
	T_MATD_D MatdD;
	T_PDCM_D PdcmD;
	if ( !GetElemMatlSectCon(ElemK, SectD, MatdD, PdcmD) ) { return FALSE; }
	if ( PdcmD.bUseDefaultVal ) return FALSE;
	
	T_REBC_D RebcD;
	if ( !m_pDoc->m_pAttrCtrl->GetRebcByElemK(ElemK, RebcD) ) { return FALSE; }
	
	double adForc[2][6] = { 0.0, };
	double adM1y[2] = { 0.0, };
	double adM1z[2] = { 0.0, };
	GetPORsltForc(ElemK, PolcK, nStep, nAddStepIdx, adForc, adM1y, adM1z);

	T_PHGT_D PhgtD;
	if ( !m_pDoc->m_pAttrCtrl->GetPhgtByElemK(ElemK, PhgtD) ) { return FALSE; }
	const bool bFiber = PhgtD.nDefinition == D_PHGT_DEFINITION_MPHI_DIST ? true : false;
	
	if ( !GetSeisEval2ndRsltSubConColm(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd) )  return FALSE;
	
	raEval2nd.bEvaluate = TRUE;
	
	CSeisEvalDataTool Tool;
	
	T_COLM_SHR_PARAM aShry[EN_MEMBEND_NUM];
	if ( !MakeColmShearParamPdgn(ElemK, EN_NLANALTYPE::PO, EN_DIR_Y, EN_EVALMATL_AVG, aShry) ) { ASSERT(0); return FALSE; }
	
	T_COLM_SHR_PARAM aShrz[EN_MEMBEND_NUM];
	if ( !MakeColmShearParamPdgn(ElemK, EN_NLANALTYPE::PO, EN_DIR_Z, EN_EVALMATL_AVG, aShrz) ) { ASSERT(0); return FALSE; }
		
	double dFck = Tool.GetFc(EN_EVALMATL_AVG, PdcmD);
	
	T_PDHG_CONCOLM_INFO PdhgInfo;
	PdhgInfo.dfck = dFck;
    PdhgInfo.dEc  = GetEc(MatdD.Data1.Analysis.Elast, dFck);

	PdhgInfo.dfy  = Tool.GetFym(EN_EVALMATL_AVG, PdcmD);
	PdhgInfo.dfys = Tool.GetFys(EN_EVALMATL_AVG, PdcmD);
	PdhgInfo.dEs  = CalcEs();

	PdhgInfo.bCircle = [] (const CString& strShape)
	{
		if ( strShape == D_SECT_SHAPE_REG_SR ) { return true; }
		if ( strShape == D_SECT_SHAPE_REG_P ) { return true; }
		return false;
	}(SectD.SectBefore.Shape);

	PdhgInfo.dAg  = SectD.SectBefore.SectI.Stiffness.Area;
	PdhgInfo.dAst = GetRebarAs(RebcD.MainRebarName) * RebcD.nQrb;
	PdhgInfo.ddo  = RebcD.dConcCenter;
	PdhgInfo.unDetail = GetSeisDetailConColm(ElemK);

	double dShearSpany[EN_MEMBEND_NUM] = { 0.0, };
	GetShearSpanConColm(ElemK, DOF_FY, dShearSpany);
	double dShearSpanz[EN_MEMBEND_NUM] = { 0.0, };
	GetShearSpanConColm(ElemK, DOF_FZ, dShearSpanz);

	double dB = 0.0;
	double dH = 0.0;
	GetSectDim(SectD, dB, dH);
	
	double adDy[2] = { raEval2nd.PosI.PhneD[DOF_MY].GetD1(), raEval2nd.PosJ.PhneD[DOF_MY].GetD1() };
	double adDz[2] = { raEval2nd.PosI.PhneD[DOF_MZ].GetD1(), raEval2nd.PosJ.PhneD[DOF_MZ].GetD1() };

	_DB_PMCV_DATA Pmcvy;
	_DB_PMCV_DATA Pmcvz;
	if ( !GetPmcvData(ElemK, EN_EVALSTEP_2ND, EN_EVALMATL_AVG, Pmcvy, Pmcvz) ) { return FALSE; }

	double adThetamaxy[2] = { 0.0, };
	double adThetamaxz[2] = { 0.0, };

	CHingeResultTool HingeTool;
	HingeTool.GetPOHingeThetamaxByPos(ElemK, DOF_MY, PolcK, nStep, nAddStepIdx, adThetamaxy);
	HingeTool.GetPOHingeThetamaxByPos(ElemK, DOF_MZ, PolcK, nStep, nAddStepIdx, adThetamaxz);
	
	adThetamaxy[0] *= dShearSpanz[0];
	adThetamaxy[1] *= dShearSpanz[1];
	adThetamaxz[0] *= dShearSpany[0];
	adThetamaxz[1] *= dShearSpany[1];

	const UINT unThetayType = GetPOThetayConColmAnalType();
	
	UINT unMaxLevel = EN_LEVEL_IO;
	for ( int i=0; i<EN_MEMBEND_NUM; ++i )
	{
		T_EVAL2ND_COLM_POS& rCurPos = i==EN_MEMBEND_I ? raEval2nd.PosI : raEval2nd.PosJ;
		T_COLM_SHR_PARAM CurShry = aShry[i];
		T_COLM_SHR_PARAM CurShrz = aShrz[i];
		
		T_PDHG_CONCOLM_DIR PdhgDiry;
		PdhgDiry.dPu = adForc[i][DOF_FX]*(-1.0);
		PdhgDiry.dVu = adForc[i][DOF_FZ];;
		PdhgDiry.dMu = adForc[i][DOF_MY];
		PdhgDiry.dB  = dB;
		PdhgDiry.dH  = dH;
		PdhgDiry.dIg = SectD.SectBefore.SectI.Stiffness.Ryy;
		PdhgDiry.dIe = PdhgDiry.dIg * GetFactorIeConColm(ElemK, EN_DIR_Y);		
		PdhgDiry.dBv = CurShrz.Comm.dBv;
		PdhgDiry.ddv = CurShrz.Comm.dDv;
		PdhgDiry.dAv = CurShrz.Comm.dAsv;
		PdhgDiry.dShearSpan = dShearSpanz[i];
		PdhgDiry.dMn = bFiber ? CalcMnByAxial(Pmcvy, PdhgDiry.dPu) : fabs(rCurPos.PhneD[DOF_MY].GetP1());
		PdhgDiry.dVy = SafeDiv(PdhgDiry.dMn, PdhgDiry.dShearSpan);
		PdhgDiry.dVc = CalcVcConColm(CurShrz, PdhgDiry.dPu);
		PdhgDiry.dVs = CalcVsFram(CurShrz.Comm);
		PdhgDiry.dVn = PdhgDiry.dVc + PdhgDiry.dVs;
				
		T_PDHG_CONCOLM_DIR PdhgDirz;
		PdhgDirz.dPu = adForc[i][DOF_FX]*(-1.0);
		PdhgDirz.dVu = adForc[i][DOF_FY];;
		PdhgDirz.dMu = adForc[i][DOF_MZ];
		PdhgDirz.dB  = dH;
		PdhgDirz.dH  = dB;
		PdhgDirz.dIg = SectD.SectBefore.SectI.Stiffness.Rzz;
		PdhgDirz.dIe = PdhgDirz.dIg * GetFactorIeConColm(ElemK, EN_DIR_Z);
		PdhgDirz.dBv = CurShry.Comm.dBv;
		PdhgDirz.ddv = CurShry.Comm.dDv;
		PdhgDirz.dAv = CurShry.Comm.dAsv;
		PdhgDirz.dShearSpan = dShearSpany[i];
		PdhgDirz.dMn = bFiber ? CalcMnByAxial(Pmcvz, PdhgDirz.dPu) : fabs(rCurPos.PhneD[DOF_MZ].GetP1());
		PdhgDirz.dVy = SafeDiv(PdhgDirz.dMn, PdhgDirz.dShearSpan);
		PdhgDirz.dVc = CalcVcConColm(CurShry, PdhgDirz.dPu);
		PdhgDirz.dVs = CalcVsFram(CurShry.Comm);
		PdhgDirz.dVn = PdhgDirz.dVc + PdhgDirz.dVs;
		
		CalcSeisEval2ndRsltConColmDir(unThetayType, PdhgInfo, CurShrz, bFiber, adDy[i], adThetamaxy[i], PdhgDiry, rCurPos.Diry, rCurPos.PhneD[DOF_MY]);
		unMaxLevel = max(unMaxLevel, rCurPos.Diry.Moment.enLevel);

		CalcSeisEval2ndRsltConColmDir(unThetayType, PdhgInfo, CurShry, bFiber, adDz[i], adThetamaxz[i], PdhgDirz, rCurPos.Dirz, rCurPos.PhneD[DOF_MZ]);
		unMaxLevel = max(unMaxLevel, rCurPos.Dirz.Moment.enLevel);
	}
	
	raEval2nd.enLevel = (EN_LEVEL)unMaxLevel;

	return TRUE;
}

BOOL CSeisEvalAIK_G001_2021::CalcSeisEval2ndRsltConColmFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd)
{
	raEval2nd.Initialize();
	int nAddStepIdx = -1;
	return CalcSeisEval2ndRsltConColmFiber(ElemK, PolcK, nStep, nAddStepIdx, nSerialAgxp, raEval2nd);
}

BOOL CSeisEvalAIK_G001_2021::CalcSeisEval2ndRsltConWallFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd)
{
	CCurUnitSaver UnitSave(TRUE);
	SetCodeUnit();

	raEval2nd.Initialize();

	T_WALL_KEY WallKey;
	T_WALL_D WallD;  WallD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetWallDataFromElemK(ElemK, WallD, &WallKey) ) { return FALSE; }
	if ( !WallD.bLineWall ) { return FALSE; }

	raEval2nd.unWallKey = WallKey;

	T_THIK_D ThikD;
	T_MATD_D MatdD;
	T_PDCM_D PdcmD;
	if ( !GetElemMatlThik(ElemK, ThikD, MatdD, PdcmD) ) { return FALSE; }
	if ( PdcmD.bUseDefaultVal ) return FALSE;
		
	double adForc[2][6] = { { 0.0 }, };
	double adM1y[2] = { 0.0, };
	double adM1z[2] = { 0.0, };
	GetPORsltForc(ElemK, PolcK, nStep, nAddStepIdx, adForc, adM1y, adM1z);

	_DB_PMCV_DATA Pmcvy;
	_DB_PMCV_DATA Pmcvz;
	if ( !GetPmcvData(ElemK, EN_EVALSTEP_2ND, EN_EVALMATL_AVG, Pmcvy, Pmcvz) ) { return FALSE; }

	if ( !GetSeisEval2ndRsltSubConWallFiber(ElemK, PolcK, nStep, nAddStepIdx, nSerialAgxp, raEval2nd) )  return FALSE;

	raEval2nd.bEvaluate = TRUE;

	T_WALL_SHR_PARAM aShrAvg[EN_MEMBEND_NUM];
	T_WALL_SHR_PARAM aShrNom[EN_MEMBEND_NUM];
	if ( !MakeWallShearParam(ElemK, EN_DIR_Z, aShrAvg) ) { return FALSE; }
	if ( !MakeWallShearParam(ElemK, EN_DIR_Z, aShrNom) ) { return FALSE; }
	SetWallForcShearParam(adForc, EN_DIR_Z, aShrAvg, aShrNom);

	CSeisEvalDataTool Tool;
	const double dFck = Tool.GetFc(EN_EVALMATL_NOM, PdcmD);
	const double dTw  = WallD.dThickness;
	const double dLw  = WallD.dLength;
	const double dHw  = WallD.dHeight;
	const double dAg  = dTw*dLw;
	const double dIe  = dTw * pow(dLw, 3.0) / 12.0 * GetFactorIeConWall(ElemK);
	const double dLpRatio = GetLpRatioConWall(ElemK);
	const double dLp = dLw * dLpRatio;
	const bool bIgnoreThetay = IsIgnoreThetayFiberWall();

	CHingeResultTool HingeTool;
	const double dThetamax = HingeTool.GetPOHingeThetamaxAbsMax(ElemK, DOF_MY, PolcK, nStep, nAddStepIdx) * dLp;

	UINT enMaxLevel = EN_LEVEL_IO;		
	for ( int i=0; i<EN_MEMBEND_NUM; ++i )
	{
		T_EVAL2ND_WALL_POS& rCur = i==EN_MEMBEND_I ? raEval2nd.PosI : raEval2nd.PosJ;
		EN_WALL_FORC_POS ForcPos = GetWallForcPos(static_cast<EN_MEMBEND>(i));

		const T_WALL_SHR_PARAM CurShr = aShrAvg[i];
		double dPu = adForc[ForcPos][DOF_FX]*(-1.0);
		
		rCur.unSeisDetail = GetSeisDetailConWall(ElemK);
		rCur.dAxialIndex  = CalcAxialIndex(dPu, dFck, dAg);
		rCur.dShearIndex  = CalcShearIndexWall(CurShr.dV, CurShr);
				
		const double dEc = GetEc(MatdD.Data1.Analysis.Elast, dFck);
		const double dMn = CalcMnByAxial(Pmcvy, dPu);
		const double dThetay = bIgnoreThetay ? 0.0 : SafeDivAbs(dMn, dEc*dIe) * dLp;
		const double dThetap = CalcThetapConColm(dThetamax, dThetay);
				
		CalcAcceptablePlasticHingeRotationWall(rCur.unSeisDetail, rCur.dAxialIndex, rCur.dShearIndex, 
			rCur.PhneD[DOF_MY].dIO, rCur.PhneD[DOF_MY].dLS, rCur.PhneD[DOF_MY].dCP);

		UINT enCurLevel = GetPerformanceLevel(dThetap, rCur.PhneD[DOF_MY].dIO, rCur.PhneD[DOF_MY].dLS, rCur.PhneD[DOF_MY].dCP);
				
		rCur.dLp = dLp;
		rCur.dThetamax = dThetamax;
		rCur.dThetay = dThetay;
		rCur.dThetap = dThetap;

		rCur.PhneD[DOF_MY].enLevel = enCurLevel;
		enCurLevel = max(enCurLevel, rCur.PhneD[DOF_MY].enLevel_Force);
		enMaxLevel = max(enCurLevel, enMaxLevel);
	}

	raEval2nd.PosI.bPhne[DOF_FZ] = TRUE;
	raEval2nd.PosJ.bPhne[DOF_FZ] = TRUE;

	GetSeisEval2ndRsltSubConWallPOFiberShear(ElemK, PolcK, nStep, nAddStepIdx, DOF_FZ, raEval2nd.PosI, raEval2nd.PosJ);

	raEval2nd.enLevel = static_cast<EN_LEVEL>(enMaxLevel);

	return TRUE;
}

BOOL CSeisEvalAIK_G001_2021::CalcSeisEval2ndRsltConWallFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd)
{
	raEval2nd.Initialize();
	int nAddStepIdx = -1;
	return CalcSeisEval2ndRsltConWallFiber(ElemK, PolcK, nStep, nAddStepIdx, nSerialAgxp, raEval2nd);
}

BOOL CSeisEvalAIK_G001_2021::CalcSeisEval2ndRsltConBeamTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nHingeTimeIndex, OUT T_EVAL2ND_BEAM& raEval2nd)
{
	CCurUnitSaver UnitSave(TRUE);
	SetCodeUnit();

	raEval2nd.Initialize();

	T_SECT_D SectD;
	T_MATD_D MatdD;
	T_PDCM_D PdcmD;
	if ( !GetElemMatlSectCon(ElemK, SectD, MatdD, PdcmD) ) { ASSERT(0); return FALSE; }
	if ( PdcmD.bUseDefaultVal ) { return FALSE; }

	/// [GEN-7144] TH Beam의 부재력도 초기하중에 의한 부재력으로.
	const T_THIS_K InitThisK = GetInitialLoadThisK(ThisK);
	const int nInitHingeTimeIndex = 0;
	double adInitForc[2][6] = { { 0.0 }, };	
	GetTHRsltForc(ElemK, InitThisK, nInitHingeTimeIndex, adInitForc);

	if ( !GetSeisEval2ndRsltSubConBeamTH(ElemK, ThisK, nHingeTimeIndex, 1, raEval2nd) ) { return FALSE; }

	raEval2nd.bEvaluate = TRUE;

	double adV[2] = { adInitForc[0][2], adInitForc[1][2] };
	double adM[2] = { adInitForc[0][4], adInitForc[1][4] };
	double adRatioMuMy[2] = { 1.0, };
	GetRatioMuMyIehp(ElemK, adM, adRatioMuMy);
	
	T_PDHG_CONBEAM_INFO aInfo[EN_MEMBEND_NUM];
	if ( !MakeBeamInfo(ElemK, EN_NLANALTYPE::TH, adRatioMuMy, adV, adM, aInfo) ) { return FALSE; }

	auto L_GetThetay = [] (const T_PDHG_CONBEAM_INFO& Cur, double dMn)
	{
		return SafeDiv(dMn*Cur.dShearSpan, 3.0*Cur.dEc*Cur.dIe);
	};

	for ( int i=0; i<EN_MEMBEND_NUM; ++i )
	{
		T_EVAL2ND_BEAM_POS& rCurPos = i==EN_MEMBEND_I ? raEval2nd.EndI : raEval2nd.EndJ;

		const T_PDHG_CONBEAM_INFO& Info = aInfo[i];
		const bool bPositive = Info.dMu < 0.0 ? false : true;
		T_PDHG_CONBEAM_PARAM Param;
		const auto pdFactor = CalcPlasticDeformationFactors(Info, bPositive, Param);
		
		rCurPos.dVc = Info.dVc;
		rCurPos.dVs = Info.dVs;
		rCurPos.dVp = Info.dVp;
		rCurPos.dSpace = Info.dS;
		rCurPos.dd     = min(Info.FlexBot.dd, Info.FlexTop.dd);
		rCurPos.dVu    = Info.dVu;
		rCurPos.enCtrlType = CalcBeamCtrlType(Info.dVp, Info.dVc + Info.dVs);
		rCurPos.unDetail   = Info.unDetail;
		rCurPos.bSeisConform = Param.bSeisDetail;
		rCurPos.bSeisDetail_1 = IsSeismicDetail_1(rCurPos.dSpace, rCurPos.dd);
		rCurPos.bSeisDetail_2 = IsSeismicDetail_2(rCurPos.dVs, rCurPos.dVu);
		rCurPos.dEspUL = Param.dEspUL;
		rCurPos.dEspCF = Param.dEspCF;
		rCurPos.dEspBL = Param.dEspBL;
		rCurPos.dEspSC = Param.dEspSC;
		rCurPos.dCu   = Param.dcu;
		rCurPos.dPhiy = Param.dPhiy;
		rCurPos.dLh   = Param.dlh;
	}

	return TRUE;
}

BOOL CSeisEvalAIK_G001_2021::CalcSeisEval2ndRsltConColmTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nHingeTimeIndex, int nSerialAgxt, OUT T_EVAL2ND_COLM& raEval2nd)
{
	CCurUnitSaver UnitSave(TRUE);
	SetCodeUnit();

	raEval2nd.Initialize();

	T_SECT_D SectD;
	T_MATD_D MatdD;
	T_PDCM_D PdcmD;
	if ( !GetElemMatlSectCon(ElemK, SectD, MatdD, PdcmD) ) { ASSERT(0); return FALSE; }
	if ( PdcmD.bUseDefaultVal ) { return FALSE; }

	T_REBC_D RebcD;
	if ( !m_pDoc->m_pAttrCtrl->GetRebcByElemK(ElemK, RebcD) ) { return FALSE; }
	
	/// TH Lumped Hinge 평가할 때는 초기하중에 의한 축력으로 Mn, Vc 등을 계산합니다.
	const T_THIS_K InitThisK = GetInitialLoadThisK(ThisK);
	const int nInitHingeTimeIndex = 0;
	double adInitForc[2][6] = { { 0.0 }, };	
	GetTHRsltForc(ElemK, InitThisK, nInitHingeTimeIndex, adInitForc);
		
	if ( !GetSeisEval2ndRsltSubConColmTH(ElemK, ThisK, nHingeTimeIndex, 1, raEval2nd) ) { return FALSE; }

	raEval2nd.bEvaluate = TRUE;
		
	T_COLM_SHR_PARAM aShry[EN_MEMBEND_NUM];
	if ( !MakeColmShearParamPdgn(ElemK, EN_NLANALTYPE::TH, EN_DIR_Y, EN_EVALMATL_AVG, aShry) ) { ASSERT(0); return FALSE; }

	T_COLM_SHR_PARAM aShrz[EN_MEMBEND_NUM];
	if ( !MakeColmShearParamPdgn(ElemK, EN_NLANALTYPE::TH, EN_DIR_Z, EN_EVALMATL_AVG, aShrz) ) { ASSERT(0); return FALSE; }

	CSeisEvalDataTool Tool;
	double dFck = Tool.GetFc(EN_EVALMATL_AVG, PdcmD);

	raEval2nd.bEvaluate = TRUE;

	T_PDHG_CONCOLM_INFO PdhgInfo;
	PdhgInfo.dfck = dFck;
    PdhgInfo.dEc  = GetEc(MatdD.Data1.Analysis.Elast, dFck);

	PdhgInfo.dfy  = Tool.GetFym(EN_EVALMATL_AVG, PdcmD);
	PdhgInfo.dfys = Tool.GetFys(EN_EVALMATL_AVG, PdcmD);
	PdhgInfo.dEs  = CalcEs();

	PdhgInfo.bCircle = [] (const CString& strShape)
	{
		if ( strShape == D_SECT_SHAPE_REG_SR ) { return true; }
		if ( strShape == D_SECT_SHAPE_REG_P ) { return true; }
		return false;
	}(SectD.SectBefore.Shape);

	PdhgInfo.dAg  = SectD.SectBefore.SectI.Stiffness.Area;
	PdhgInfo.dAst = GetRebarAs(RebcD.MainRebarName) * RebcD.nQrb;
	PdhgInfo.ddo  = RebcD.dConcCenter;
	PdhgInfo.unDetail = GetSeisDetailConColm(ElemK);

	const bool bFiber = false; //// 이 함수는 lumped hinge 전용.

	_DB_PMCV_DATA Pmcvy;
	_DB_PMCV_DATA Pmcvz;
	if ( !GetPmcvData(ElemK, EN_EVALSTEP_2ND, EN_EVALMATL_AVG, Pmcvy, Pmcvz) ) { return FALSE; }

	double dShearSpany[EN_MEMBEND_NUM] = { 0.0, };
	GetShearSpanConColm(ElemK, DOF_FY, dShearSpany);
	double dShearSpanz[EN_MEMBEND_NUM] = { 0.0, };
	GetShearSpanConColm(ElemK, DOF_FZ, dShearSpanz);

	double dB = 0.0;
	double dH = 0.0;
	GetSectDim(SectD, dB, dH);

	double adDy[2] = { raEval2nd.PosI.PhneD[DOF_MY].GetD1(), raEval2nd.PosJ.PhneD[DOF_MY].GetD1() };
	double adDz[2] = { raEval2nd.PosI.PhneD[DOF_MZ].GetD1(), raEval2nd.PosJ.PhneD[DOF_MZ].GetD1() };

	m_pDoc->m_pPostCtrl->SelectThis(ThisK);

	double adThetamaxy[2] = { raEval2nd.PosI.PhneD[DOF_MY].dDisplacement, raEval2nd.PosJ.PhneD[DOF_MY].dDisplacement };
	double adThetamaxz[2] = { raEval2nd.PosI.PhneD[DOF_MZ].dDisplacement, raEval2nd.PosJ.PhneD[DOF_MZ].dDisplacement };
	
	const UINT unThetayType = GetTHThetayConColmAnalType();
	
	UINT unMaxLevel = EN_LEVEL_IO;
	for ( int i=0; i<EN_MEMBEND_NUM; ++i )
	{
		T_EVAL2ND_COLM_POS& rCurPos = i==EN_MEMBEND_I ? raEval2nd.PosI : raEval2nd.PosJ;
		T_COLM_SHR_PARAM CurShry = aShry[i];
		T_COLM_SHR_PARAM CurShrz = aShrz[i];

		T_PDHG_CONCOLM_DIR PdhgDiry;
		PdhgDiry.dPu = adInitForc[i][DOF_FX]*(-1.0);
		PdhgDiry.dVu = adInitForc[i][DOF_FZ];;
		PdhgDiry.dMu = adInitForc[i][DOF_MY];
		PdhgDiry.dB  = dB;
		PdhgDiry.dH  = dH;
		PdhgDiry.dIg = SectD.SectBefore.SectI.Stiffness.Ryy;
		PdhgDiry.dIe = PdhgDiry.dIg * GetFactorIeConColm(ElemK, EN_DIR_Y);
		PdhgDiry.dBv = CurShrz.Comm.dBv;
		PdhgDiry.ddv = CurShrz.Comm.dDv;
		PdhgDiry.dAv = CurShrz.Comm.dAsv;
		PdhgDiry.dShearSpan = dShearSpanz[i];
		PdhgDiry.dMn = CalcMnByAxial(Pmcvy, PdhgDiry.dPu);
		PdhgDiry.dVy = SafeDiv(PdhgDiry.dMn, PdhgDiry.dShearSpan);
		PdhgDiry.dVc = CalcVcConColm(CurShrz, PdhgDiry.dPu);
		PdhgDiry.dVs = CalcVsFram(CurShrz.Comm);
		PdhgDiry.dVn = PdhgDiry.dVc + PdhgDiry.dVs;

		T_PDHG_CONCOLM_DIR PdhgDirz;
		PdhgDirz.dPu = adInitForc[i][DOF_FX]*(-1.0);
		PdhgDirz.dVu = adInitForc[i][DOF_FY];;
		PdhgDirz.dMu = adInitForc[i][DOF_MZ];
		PdhgDirz.dB  = dH;
		PdhgDirz.dH  = dB;
		PdhgDirz.dIg = SectD.SectBefore.SectI.Stiffness.Rzz;
		PdhgDirz.dIe = PdhgDirz.dIg * GetFactorIeConColm(ElemK, EN_DIR_Z);
		PdhgDirz.dBv = CurShry.Comm.dBv;
		PdhgDirz.ddv = CurShry.Comm.dDv;
		PdhgDirz.dAv = CurShry.Comm.dAsv;
		PdhgDirz.dShearSpan = dShearSpany[i];
		PdhgDirz.dMn = CalcMnByAxial(Pmcvz, PdhgDirz.dPu);
		PdhgDirz.dVy = SafeDiv(PdhgDirz.dMn, PdhgDirz.dShearSpan);
		PdhgDirz.dVc = CalcVcConColm(CurShry, PdhgDirz.dPu);
		PdhgDirz.dVs = CalcVsFram(CurShry.Comm);
		PdhgDirz.dVn = PdhgDirz.dVc + PdhgDirz.dVs;

		CalcSeisEval2ndRsltConColmDir(unThetayType, PdhgInfo, CurShrz, bFiber, adDy[i], adThetamaxy[i], PdhgDiry, rCurPos.Diry, rCurPos.PhneD[DOF_MY]);
		unMaxLevel = max(unMaxLevel, rCurPos.Diry.Moment.enLevel);

		CalcSeisEval2ndRsltConColmDir(unThetayType, PdhgInfo, CurShry, bFiber, adDz[i], adThetamaxz[i], PdhgDirz, rCurPos.Dirz, rCurPos.PhneD[DOF_MZ]);
		unMaxLevel = max(unMaxLevel, rCurPos.Dirz.Moment.enLevel);						
	}

	return TRUE;
}

BOOL CSeisEvalAIK_G001_2021::CalcSeisEval2ndRsltConColmFiberTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nHingeTimeIndex, int nSerialAgxt, OUT T_EVAL2ND_COLM& raEval2nd)
{
	SetCodeUnit();

	raEval2nd.Initialize();

	T_SECT_D SectD;
	T_MATD_D MatdD;
	T_PDCM_D PdcmD;
	if ( !GetElemMatlSectCon(ElemK, SectD, MatdD, PdcmD) ) { return FALSE; }
	if ( PdcmD.bUseDefaultVal ) return FALSE;

	T_REBC_D RebcD;
	if ( !m_pDoc->m_pAttrCtrl->GetRebcByElemK(ElemK, RebcD) ) { return FALSE; }
		
	/// TH Fiber Hinge 평가할 때는 초기하중에 의한 축력으로 Mn, Vc 등을 계산합니다. [GEN-7133]
	/// -> [GEN-7268] 해당 T_THIS_K의 부재력으로 Mn, Vc 등을 계산	
	double adForc[2][6] = { { 0.0 }, };
	GetTHRsltForc(ElemK, ThisK, nHingeTimeIndex, adForc);

	if ( !GetSeisEval2ndRsltSubConColmFiberTH(ElemK, ThisK, nHingeTimeIndex, 1, nSerialAgxt, raEval2nd) )  return FALSE;
	
	T_COLM_SHR_PARAM aShry[EN_MEMBEND_NUM];
	if ( !MakeColmShearParamPdgn(ElemK, EN_NLANALTYPE::TH, EN_DIR_Y, EN_EVALMATL_AVG, aShry) ) { ASSERT(0); return FALSE; }

	T_COLM_SHR_PARAM aShrz[EN_MEMBEND_NUM];
	if ( !MakeColmShearParamPdgn(ElemK, EN_NLANALTYPE::TH, EN_DIR_Z, EN_EVALMATL_AVG, aShrz) ) { ASSERT(0); return FALSE; }

	CSeisEvalDataTool Tool;
	double dFck = Tool.GetFc(EN_EVALMATL_AVG, PdcmD);

	raEval2nd.bEvaluate = TRUE;

	T_PDHG_CONCOLM_INFO PdhgInfo;
	PdhgInfo.dfck = dFck;
    PdhgInfo.dEc  = GetEc(MatdD.Data1.Analysis.Elast, dFck);

	PdhgInfo.dfy  = Tool.GetFym(EN_EVALMATL_AVG, PdcmD);
	PdhgInfo.dfys = Tool.GetFys(EN_EVALMATL_AVG, PdcmD);
	PdhgInfo.dEs  = CalcEs();

	PdhgInfo.bCircle = [] (const CString& strShape)
	{
		if ( strShape == D_SECT_SHAPE_REG_SR ) { return true; }
		if ( strShape == D_SECT_SHAPE_REG_P ) { return true; }
		return false;
	}(SectD.SectBefore.Shape);

	PdhgInfo.dAg  = SectD.SectBefore.SectI.Stiffness.Area;
	PdhgInfo.dAst = GetRebarAs(RebcD.MainRebarName) * RebcD.nQrb;
	PdhgInfo.ddo  = RebcD.dConcCenter;
	PdhgInfo.unDetail = GetSeisDetailConColm(ElemK);

	const bool bFiber = true;

	_DB_PMCV_DATA Pmcvy;
	_DB_PMCV_DATA Pmcvz;
	if ( !GetPmcvData(ElemK, EN_EVALSTEP_2ND, EN_EVALMATL_AVG, Pmcvy, Pmcvz) ) { return FALSE; }

	double dShearSpany[EN_MEMBEND_NUM] = { 0.0, };
	GetShearSpanConColm(ElemK, DOF_FY, dShearSpany);
	double dShearSpanz[EN_MEMBEND_NUM] = { 0.0, };
	GetShearSpanConColm(ElemK, DOF_FZ, dShearSpanz);

	double dB = 0.0;
	double dH = 0.0;
	GetSectDim(SectD, dB, dH);	
		
	m_pDoc->m_pPostCtrl->SelectThis(ThisK);

	double adDy[2] = { raEval2nd.PosI.PhneD[DOF_MY].GetD1(), raEval2nd.PosJ.PhneD[DOF_MY].GetD1() };
	double adDz[2] = { raEval2nd.PosI.PhneD[DOF_MZ].GetD1(), raEval2nd.PosJ.PhneD[DOF_MZ].GetD1() };
	
	double adThetamaxy[2] = { 0.0, };
	double adThetamaxz[2] = { 0.0, };

	CHingeResultTool HgTool;
	HgTool.GetTHHingeThetamaxByPos(ElemK, DOF_MY, ThisK, nHingeTimeIndex, adThetamaxy);
	HgTool.GetTHHingeThetamaxByPos(ElemK, DOF_MZ, ThisK, nHingeTimeIndex, adThetamaxz);

	adThetamaxy[0] *= dShearSpanz[0];
	adThetamaxy[1] *= dShearSpanz[1];
	adThetamaxz[0] *= dShearSpany[0];
	adThetamaxz[1] *= dShearSpany[1];

	const UINT unThetayType = GetTHThetayConColmAnalType();

	UINT unMaxLevel = EN_LEVEL_IO;
	for ( int i=0; i<EN_MEMBEND_NUM; ++i )
	{
		T_EVAL2ND_COLM_POS& rCurPos = i==EN_MEMBEND_I ? raEval2nd.PosI : raEval2nd.PosJ;
		T_COLM_SHR_PARAM CurShry = aShry[i];
		T_COLM_SHR_PARAM CurShrz = aShrz[i];
		
		T_PDHG_CONCOLM_DIR PdhgDiry;
		PdhgDiry.dPu = adForc[i][DOF_FX]*(-1.0);
		PdhgDiry.dVu = adForc[i][DOF_FZ];;
		PdhgDiry.dMu = adForc[i][DOF_MY];
		PdhgDiry.dB  = dB;
		PdhgDiry.dH  = dH;
		PdhgDiry.dIg = SectD.SectBefore.SectI.Stiffness.Ryy;
		PdhgDiry.dIe = PdhgDiry.dIg * GetFactorIeConColm(ElemK, EN_DIR_Y);
		PdhgDiry.dBv = CurShrz.Comm.dBv;
		PdhgDiry.ddv = CurShrz.Comm.dDv;
		PdhgDiry.dAv = CurShrz.Comm.dAsv;
		PdhgDiry.dShearSpan = dShearSpanz[i];
		PdhgDiry.dMn = CalcMnByAxial(Pmcvy, PdhgDiry.dPu);
		PdhgDiry.dVy = SafeDiv(PdhgDiry.dMn, PdhgDiry.dShearSpan);
		PdhgDiry.dVc = CalcVcConColm(CurShrz, PdhgDiry.dPu);
		PdhgDiry.dVs = CalcVsFram(CurShrz.Comm);
		PdhgDiry.dVn = PdhgDiry.dVc + PdhgDiry.dVs;

		T_PDHG_CONCOLM_DIR PdhgDirz;
		PdhgDirz.dPu = adForc[i][DOF_FX]*(-1.0);
		PdhgDirz.dVu = adForc[i][DOF_FY];;
		PdhgDirz.dMu = adForc[i][DOF_MZ];
		PdhgDirz.dB  = dH;
		PdhgDirz.dH  = dB;
		PdhgDirz.dIg = SectD.SectBefore.SectI.Stiffness.Rzz;
		PdhgDirz.dIe = PdhgDirz.dIg * GetFactorIeConColm(ElemK, EN_DIR_Z);
		PdhgDirz.dBv = CurShry.Comm.dBv;
		PdhgDirz.ddv = CurShry.Comm.dDv;
		PdhgDirz.dAv = CurShry.Comm.dAsv;
		PdhgDirz.dShearSpan = dShearSpany[i];
		PdhgDirz.dMn = CalcMnByAxial(Pmcvz, PdhgDirz.dPu);
		PdhgDirz.dVy = SafeDiv(PdhgDirz.dMn, PdhgDirz.dShearSpan);
		PdhgDirz.dVc = CalcVcConColm(CurShry, PdhgDirz.dPu);
		PdhgDirz.dVs = CalcVsFram(CurShry.Comm);
		PdhgDirz.dVn = PdhgDirz.dVc + PdhgDirz.dVs;

		CalcSeisEval2ndRsltConColmDir(unThetayType, PdhgInfo, CurShrz, bFiber, adDy[i], adThetamaxy[i], PdhgDiry, rCurPos.Diry, rCurPos.PhneD[DOF_MY]);
		unMaxLevel = max(unMaxLevel, rCurPos.Diry.Moment.enLevel);

		CalcSeisEval2ndRsltConColmDir(unThetayType, PdhgInfo, CurShry, bFiber, adDz[i], adThetamaxz[i], PdhgDirz, rCurPos.Dirz, rCurPos.PhneD[DOF_MZ]);
		unMaxLevel = max(unMaxLevel, rCurPos.Dirz.Moment.enLevel);

		// Shear Force
		rCurPos.Diry.Moment.enLevel_Force = rCurPos.PhneD[DOF_FY].enLevel_Force;
		rCurPos.Dirz.Moment.enLevel_Force = rCurPos.PhneD[DOF_FZ].enLevel_Force;
	}

	raEval2nd.enLevel = (EN_LEVEL)unMaxLevel;

	return TRUE;
}

bool CSeisEvalAIK_G001_2021::ReCalcSeisEval2ndRsltConBeamLumpedTH(const T_EVAL2ND_BEAM_POS& InData, OUT T_EVAL2ND_BEAM_POS& rData)
{
	constexpr double eps = std::numeric_limits<double>::epsilon();

	rData.PhneD[DOF_MY].dIO = InData.PhneD[DOF_MY].dIO;
	rData.PhneD[DOF_MY].dLS = InData.PhneD[DOF_MY].dLS;
	rData.PhneD[DOF_MY].dCP = InData.PhneD[DOF_MY].dCP;

	if (fabs(InData.PhneD[DOF_MY].GetD1()) < eps)
	{
		rData.PhneD[DOF_MY].dDuctility = fabs(InData.PhneD[DOF_MY].dDisplacement / eps);
	}
	else
	{
		rData.PhneD[DOF_MY].dDuctility = SafeDivAbs(InData.PhneD[DOF_MY].dDisplacement, InData.PhneD[DOF_MY].GetD1(), eps);
	}

	if (std::isnan(rData.PhneD[DOF_MY].dDuctility) || std::isinf(rData.PhneD[DOF_MY].dDuctility))
	{
		rData.PhneD[DOF_MY].dDuctility = (std::numeric_limits<double>::max)();
	}
	rData.PhneD[DOF_MY].enLevel_Deform = GetPerformanceLevel(rData.PhneD[DOF_MY].dDuctility, rData.PhneD[DOF_MY].dIO, rData.PhneD[DOF_MY].dLS, rData.PhneD[DOF_MY].dCP);

	return true;
}

bool CSeisEvalAIK_G001_2021::ReCalcSeisEval2ndRsltConColmLumpedTH(const int nDOF, const T_EVAL2ND_COLM_POS& InData, OUT T_EVAL2ND_COLM_POS& rData)
{
	if ( nDOF < DOF_MY || nDOF > DOF_MZ )
	{
		ASSERT(0);
	}
	const T_EVAL2ND_COLM_DIR& CurDir = nDOF == DOF_MY ? InData.Diry : InData.Dirz;

	constexpr double eps = std::numeric_limits<double>::epsilon();

	rData.PhneD[nDOF].dIO = InData.PhneD[nDOF].dIO;
	rData.PhneD[nDOF].dLS = InData.PhneD[nDOF].dLS;
	rData.PhneD[nDOF].dCP = InData.PhneD[nDOF].dCP;
	if (fabs(CurDir.dThetay) < eps)
	{
		rData.PhneD[nDOF].dDuctility = fabs(InData.PhneD[nDOF].dDisplacement / eps);
	}
	else
	{
		rData.PhneD[nDOF].dDuctility = SafeDivAbs(InData.PhneD[nDOF].dDisplacement, CurDir.dThetay, eps);
	}

	if (std::isnan(rData.PhneD[nDOF].dDuctility) || std::isinf(rData.PhneD[nDOF].dDuctility))
	{
		rData.PhneD[nDOF].dDuctility = (std::numeric_limits<double>::max)();
	}

	rData.PhneD[nDOF].enLevel_Deform = GetPerformanceLevel(abs(rData.PhneD[nDOF].dDuctility), rData.PhneD[nDOF].dIO, rData.PhneD[nDOF].dLS, rData.PhneD[nDOF].dCP);

	return true;
}

bool CSeisEvalAIK_G001_2021::ReCalcSeisEval2ndRsltConColmFiberTHRAngle(const T_EVAL2ND_COLM_POS& InData, OUT T_EVAL2ND_COLM_POS& rData)
{
	// GEN-7145 재평가 기능 막음
	auto _l_ReCalc4Dir = [this](const T_EVAL2ND_COLM_DIR& DirData, OUT T_EVAL2ND_COLM_DIR& rData, T_EVAL2ND_PHNE& rPhne)
	{
		UINT nSeisDetail = DirData.bSeisConform ? EN_SECO_DETL_SEIS : EN_SECO_DETL_NONE;
		const double dmu = CalcmuConCom(nSeisDetail, DirData.dVc, DirData.dVs, DirData.dVp);
		const double dThetay = DirData.dThetay;
		const double dThetaa = max(DirData.dThetaa, 2.0 * dThetay);
		const double dThetau = CalcThetauConColm(dmu, dThetay, dThetaa);
		const double da = dThetau - dThetay;
		const double db = dThetaa - dThetay;
		const double dThetamax = DirData.dThetamax;
		constexpr double eps = std::numeric_limits<double>::epsilon();
		rData.dThetap = CalcThetapConColm(dThetamax, dThetay);
		rData.dmu = dmu;
		rData.dThetaa = dThetaa;
		rData.dThetau = dThetau;

		CalcAcceptablePlasticHingeRotationColmFiber(da, db, dThetay, rPhne.dIO, rPhne.dLS, rPhne.dCP);
		if (fabs(dThetay) < eps)
		{
			rPhne.dDuctility = fabs(rPhne.dDisplacement / eps);
		}
		else
		{
			rPhne.dDuctility = SafeDiv(rPhne.dDisplacement, dThetay, eps);
		}
		if (std::isnan(rPhne.dDuctility) || std::isinf(rPhne.dDuctility))
		{
			rPhne.dDuctility = (std::numeric_limits<double>::max)();
		}
		rPhne.enLevel_Deform = GetPerformanceLevel(rData.dThetap, rPhne.dIO, rPhne.dLS, rPhne.dCP);

		// ADD GEN-6679 Controlled Action Re Calc
		rData.dVc = DirData.dVc;
		rData.dVp = DirData.dVp;
		const double dVn = DirData.dVc + DirData.dVs;

		rData.enCtrlType = (dVn < DirData.dVp) ? EN_EVALCTRL_SHER : EN_EVALCTRL_FLEX;
	};
	_l_ReCalc4Dir(InData.Diry, rData.Diry, rData.PhneD[DOF_MY]);
	_l_ReCalc4Dir(InData.Dirz, rData.Dirz, rData.PhneD[DOF_MZ]);
	   
	return true;
}

bool CSeisEvalAIK_G001_2021::ReCalcSeisEval2ndRsltConColmFiberTHShear(const T_EVAL2ND_COLM_POS& InData, OUT T_EVAL2ND_COLM_POS& rData)
{
	auto _L_ReCalc4Dir = [this](const T_EVAL2ND_PHNE& InData, OUT T_EVAL2ND_PHNE& rData)
	{
			const auto dFs = InData.dQUF;
			const auto dFns = InData.dQG;
			const auto dFNomi = InData.dQCN;

			const auto dF1dot2 = dFns + 1.2 * (dFs - dFns);
			const auto dReCalcRatio = dgn::SafeDivAbs(dF1dot2, dFNomi);
			const auto enCurLevel = GetPerformanceLevel(dReCalcRatio);

			rData.dQUFapp = dF1dot2;
			rData.dRatio = dReCalcRatio;
			rData.enLevel_Force = enCurLevel;
	};

	_L_ReCalc4Dir(InData.PhneD[DOF_FY], rData.PhneD[DOF_FY]);
	_L_ReCalc4Dir(InData.PhneD[DOF_FZ], rData.PhneD[DOF_FZ]);

	return true;
}

BOOL CSeisEvalAIK_G001_2021::CalcSeisEval2ndRsltConWallFiberTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nHingeTimeIndex, int nSerialAgxt, OUT T_EVAL2ND_WALL& raEval2nd)
{
	CCurUnitSaver UnitSave(TRUE);
	SetCodeUnit();

	raEval2nd.Initialize();

	T_WALL_KEY WallKey;
	T_WALL_D WallD;  WallD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetWallDataFromElemK(ElemK, WallD, &WallKey) ) { return FALSE; }
	if ( !WallD.bLineWall ) { return FALSE; }

	if ( !GetSeisEval2ndRsltSubConWallFiberTH(ElemK, ThisK, nHingeTimeIndex, 1, nSerialAgxt, raEval2nd) )  return FALSE;

	raEval2nd.unWallKey = WallKey;
	
	/// TH Fiber Hinge 평가할 때는 초기하중에 의한 축력으로 Mn, Vc 등을 계산합니다. [GEN-7133]
	/// -> [GEN-7269] 해당 T_THIS_K의 부재력으로 Mn, Vc 등을 계산	
	double adForc[2][6] = { { 0.0 }, };
	GetTHRsltForc(ElemK, ThisK, nHingeTimeIndex, adForc);

	T_THIK_D ThikD;
	T_MATD_D MatdD;
	T_PDCM_D PdcmD;
	if ( !GetElemMatlThik(ElemK, ThikD, MatdD, PdcmD) ) { return FALSE; }
	if ( PdcmD.bUseDefaultVal ) return FALSE;
		
	_DB_PMCV_DATA Pmcvy;
	_DB_PMCV_DATA Pmcvz;
	if ( !GetPmcvData(ElemK, EN_EVALSTEP_2ND, EN_EVALMATL_AVG, Pmcvy, Pmcvz) ) { return FALSE; }
	
	T_WALL_SHR_PARAM aShrAvg[EN_MEMBEND_NUM];
	T_WALL_SHR_PARAM aShrNom[EN_MEMBEND_NUM];
	if ( !MakeWallShearParam(ElemK, EN_DIR_Z, aShrAvg) ) { return FALSE; }
	if ( !MakeWallShearParam(ElemK, EN_DIR_Z, aShrNom) ) { return FALSE; }
	SetWallForcShearParam(adForc, EN_DIR_Z, aShrAvg, aShrNom);

	m_pDoc->m_pPostCtrl->SelectThis(ThisK);

	raEval2nd.bEvaluate = TRUE;

	CSeisEvalDataTool Tool;
	const double dFck = Tool.GetFc(EN_EVALMATL_NOM, PdcmD);
	const double dTw  = WallD.dThickness;
	const double dLw  = WallD.dLength;
	const double dHw  = WallD.dHeight;
	const double dAg  = dTw*dLw;
	const double dIe  = dTw * pow(dLw, 3.0) / 12.0 * GetFactorIeConWall(ElemK);
	const double dLpRatio = GetLpRatioConWall(ElemK);
	const double dLp = dLw * dLpRatio;
	const bool bIgnoreThetay = IsIgnoreThetayFiberWall();

	CHingeResultTool HgTool;
	double dThetamax = HgTool.GetTHHingeThetamaxAbsMax(ElemK, DOF_MY, ThisK, nHingeTimeIndex) * dLp;

	UINT enMaxLevel = EN_LEVEL_IO;
	for ( int i=0; i<EN_MEMBEND_NUM; ++i )
	{
		T_EVAL2ND_WALL_POS& rCur = i==EN_MEMBEND_I ? raEval2nd.PosI : raEval2nd.PosJ;
		EN_WALL_FORC_POS ForcPos = GetWallForcPos(static_cast<EN_MEMBEND>(i));

		const T_WALL_SHR_PARAM CurShr = aShrAvg[i];		
		double dPu = adForc[ForcPos][DOF_FX]*(-1.0);

		rCur.unSeisDetail = GetSeisDetailConWall(ElemK);
		rCur.dAxialIndex  = CalcAxialIndex(dPu, dFck, dAg);
		rCur.dShearIndex  = CalcShearIndexWall(CurShr.dV, CurShr);

		const double dMn = CalcMnByAxial(Pmcvy, dPu);
		const double dEc = GetEc(MatdD.Data1.Analysis.Elast, dFck);
		const double dThetay = bIgnoreThetay ? 0.0 : SafeDivAbs(dMn, dEc*dIe) * dLp;
		const double dThetap = CalcThetapConColm(dThetamax, dThetay);

		CalcAcceptablePlasticHingeRotationWall(rCur.unSeisDetail, rCur.dAxialIndex, rCur.dShearIndex, 
			rCur.PhneD[DOF_MY].dIO, rCur.PhneD[DOF_MY].dLS, rCur.PhneD[DOF_MY].dCP);
				
		rCur.dLp = dLp;
		rCur.dThetamax = dThetamax;
		rCur.dThetay   = dThetay;
		rCur.dThetap   = dThetap;

		UINT enCurLevel = GetPerformanceLevel(dThetap, rCur.PhneD[DOF_MY].dIO, rCur.PhneD[DOF_MY].dLS, rCur.PhneD[DOF_MY].dCP);
		
		rCur.PhneD[DOF_MY].enLevel = enCurLevel;
		enCurLevel = max(enCurLevel, rCur.PhneD[DOF_MY].enLevel_Force);
		enMaxLevel = max(enCurLevel, enMaxLevel);
	}

	raEval2nd.enLevel = static_cast<EN_LEVEL>(enMaxLevel);

	return TRUE;
}

bool CSeisEvalAIK_G001_2021::ReCalcSeisEval2ndRsltConWallFiberTHRAngle(const T_EVAL2ND_WALL_POS& InData, OUT T_EVAL2ND_WALL_POS& rData)
{
	const double dThetamax = InData.dThetamax;
	const double dThetay = InData.dThetay;
	const double dThetap = CalcThetapConColm(dThetamax, dThetay);

	// GEN-7145 재평가 기능 막음
	CalcAcceptablePlasticHingeRotationWall(InData.unSeisDetail, InData.dAxialIndex, InData.dShearIndex,
		rData.PhneD[DOF_MY].dIO, rData.PhneD[DOF_MY].dLS, rData.PhneD[DOF_MY].dCP);

	UINT enCurLevel = GetPerformanceLevel(dThetap, InData.PhneD[DOF_MY].dIO, InData.PhneD[DOF_MY].dLS, InData.PhneD[DOF_MY].dCP);

	rData.dThetap = dThetap;
	rData.PhneD[DOF_MY].enLevel = enCurLevel;

	return true;
}

bool CSeisEvalAIK_G001_2021::ReCalcSeisEval2ndRsltConWallFiberTHShear(const T_EVAL2ND_WALL_POS& InData, OUT T_EVAL2ND_WALL_POS& rData)
{
	const auto dFs = InData.PhneD[DOF_FZ].dQUF;
	const auto dFns = InData.PhneD[DOF_FZ].dQG;
	const auto dFNomi = InData.PhneD[DOF_FZ].dQCN;
	
	const auto dF1dot2 = dFns + 1.2 * (dFs - dFns);
	const auto dReCalcRatio = dgn::SafeDivAbs(dF1dot2, dFNomi);
	const auto enCurLevel = GetPerformanceLevel(dReCalcRatio);

	rData.PhneD[DOF_FZ].dQUFapp = dF1dot2;
	rData.PhneD[DOF_FZ].dRatio	= dReCalcRatio;
	rData.PhneD[DOF_FZ].enLevel_Force = enCurLevel;

	return true;
}

BOOL CSeisEvalAIK_G001_2021::GetLevelSeisEval2ndConFramTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nHingeTimeIndex, int nDof, OUT UINT& runLevelI, OUT UINT& runLevelJ)
{
	runLevelI = EN_LEVEL_INVALID;
	runLevelJ = EN_LEVEL_INVALID;
	
	T_EVAL2ND_PHNE PhneDI;
	T_EVAL2ND_PHNE PhneDJ;
	if ( !GetSeisEval2ndRsltSubConFramTH(ElemK, ThisK, nHingeTimeIndex, 1, nDof, PhneDI, PhneDJ) ) { return FALSE; }
			
	runLevelI = PhneDI.enLevel;
	runLevelJ = PhneDJ.enLevel;

	return TRUE;
}

BOOL CSeisEvalAIK_G001_2021::IsIgnoreMomentHingeProp()
{
    return GetIgnoreMomentHingeProp();
}

BOOL CSeisEvalAIK_G001_2021::GetSeisEval2ndRsltSubConFramTHFiberShear(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nHingeType, int nDof, OUT T_EVAL2ND_PHNE& rPhneDI, OUT T_EVAL2ND_PHNE& rPhneDJ)
{
	rPhneDI.Initialize();
	rPhneDJ.Initialize();		
				
	T_THGC_D ThgcD;
	if (m_pDoc->m_pAttrCtrl2->GetThgc(ThgcD) && ThgcD.nInitLoadMethod == 0 && ThgcD.ThisK_DummyInitLoad != 0)
	{	
		const T_THIS_K InitThisK = ThgcD.ThisK_DummyInitLoad;
		const int nInitHingeTimeIndex = InitThisK == ThisK ? nTimeIndex : 0;
		double adInitForc[2][6] = { { 0.0 }, };
		GetTHRsltForc(ElemK, InitThisK, nInitHingeTimeIndex, adInitForc);

		double adThisForc[2][6] = { { 0.0 }, };
		GetTHRsltForc(ElemK, ThisK, nTimeIndex, adThisForc);

		/// concrete 부재는 축력 부호 처리 필요. compression = (+), tension = (-).
		adThisForc[0][0] *= -1.0;
		adThisForc[1][0] *= -1.0;
		
		const EN_DIR& enDir = nDof == D_DOF_FY ? EN_DIR_Y : EN_DIR_Z;
		T_COLM_SHR_PARAM aShr[EN_MEMBEND_NUM];
		if (!MakeColmShearParamPdgn(ElemK, EN_NLANALTYPE::TH, enDir, EN_EVALMATL_NOM, aShr)) { ASSERT(0); return FALSE; }

		const int nIndexDOF = nDof == D_DOF_FY ? 1 : 2;
		const double dQUFI = adThisForc[0][nIndexDOF];
		const double dQUFJ = adThisForc[1][nIndexDOF];
		const double dQGI  = adInitForc[0][nIndexDOF];
		const double dQGJ  = adInitForc[1][nIndexDOF];
		const double dQCNI = CalcVnConColm(aShr[0], adThisForc[0][0]);
		const double dQCNJ = CalcVnConColm(aShr[1], adThisForc[1][0]);
						
		CSeisEvalDataTool Tool;
		Tool.CalcEval2ndForcCtrlAction(ElemK, dQUFI, dQGI, dQCNI, rPhneDI);
		rPhneDI.enLevel = rPhneDI.enLevel_Force;
		Tool.CalcEval2ndForcCtrlAction(ElemK, dQUFJ, dQGJ, dQCNJ, rPhneDJ);
		rPhneDJ.enLevel = rPhneDJ.enLevel_Force;
	}

	return TRUE;
}

BOOL CSeisEvalAIK_G001_2021::GetSeisEval2ndRsltSubConWallTHFiberShear(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nHingeType, int nDof, OUT T_EVAL2ND_WALL_POS& rPosI, OUT T_EVAL2ND_WALL_POS& rPosJ)
{	
	if (!m_pDoc->m_pAttrCtrl->IsFiberHingeIE(ElemK)) { return FALSE; }

	T_THIK_D ThikD;
	T_MATD_D MatdD;
	T_PDCM_D PdcmD;
	if (!GetElemMatlThik(ElemK, ThikD, MatdD, PdcmD)) { ASSERT(0); return FALSE; }

	T_WALL_KEY WallKey;
	T_WALL_D WallD;  WallD.Initialize();
	if (!m_pDoc->m_pPostCtrl->GetWallDataFromElemK(ElemK, WallD, &WallKey)) { return FALSE; }
	const double dAg = WallD.dThickness * WallD.dLength;

	CSeisEvalDataTool Tool;
	const double dfck = Tool.GetFc(EN_EVALMATL_AVG, PdcmD);
	double adThisForc[2][6] = { { 0.0 }, };
	GetTHRsltForc(ElemK, ThisK, nTimeIndex, adThisForc);

	const double dPuI = adThisForc[0][0] * (-1.0);
	const double dPuJ = adThisForc[1][0] * (-1.0);
		
	rPosI.dAxialIndex = dPuI < 0.0 ? 0.0 : SafeDiv(dPuI, dAg * dfck);
	rPosJ.dAxialIndex = dPuJ < 0.0 ? 0.0 : SafeDiv(dPuJ, dAg * dfck);

	T_EVAL2ND_PHNE& rPhneDI = rPosI.PhneD[nDof];
	T_EVAL2ND_PHNE& rPhneDJ = rPosJ.PhneD[nDof];

	rPhneDI.Initialize();
	rPhneDJ.Initialize();
	
	T_THGC_D ThgcD;
	if (m_pDoc->m_pAttrCtrl2->GetThgc(ThgcD) && ThgcD.nInitLoadMethod == 0 && ThgcD.ThisK_DummyInitLoad != 0)
	{
		const T_THIS_K InitThisK = ThgcD.ThisK_DummyInitLoad;
		const int nInitHingeTimeIndex = InitThisK == ThisK ? nTimeIndex : 0;
		double adInitForc[2][6] = { { 0.0 }, };
		GetTHRsltForc(ElemK, InitThisK, nInitHingeTimeIndex, adInitForc);

		T_WALL_SHR_PARAM aShrAvg[EN_MEMBEND_NUM];
		T_WALL_SHR_PARAM aShrNom[EN_MEMBEND_NUM];
		if (!MakeWallShearParam(ElemK, EN_DIR_Z, aShrAvg)) { return FALSE; }
		if (!MakeWallShearParam(ElemK, EN_DIR_Z, aShrNom)) { return FALSE; }
		SetWallForcShearParam(adThisForc, EN_DIR_Z, aShrAvg, aShrNom);
		
		const int nIndexDOF = nDof == D_DOF_FY ? 1 : 2;
		const double dQUFI = adThisForc[0][nIndexDOF];
		const double dQUFJ = adThisForc[1][nIndexDOF];
		const double dQGI = adInitForc[0][nIndexDOF];
		const double dQGJ = adInitForc[1][nIndexDOF];
		const double dQCNI = CalcVnConWall(aShrNom[0]);
		const double dQCNJ = CalcVnConWall(aShrNom[1]);
				
		Tool.CalcEval2ndForcCtrlAction(ElemK, dQUFI, dQGI, dQCNI, rPhneDI);
		rPhneDI.enLevel = rPhneDI.enLevel_Force;
		Tool.CalcEval2ndForcCtrlAction(ElemK, dQUFJ, dQGJ, dQCNJ, rPhneDJ);
		rPhneDJ.enLevel = rPhneDJ.enLevel_Force;
	}

	return TRUE;
}

double CSeisEvalAIK_G001_2021::CalcVsmaxFram(const T_CON_SHR_PARAM& Shr)
{	
	return 0.2 * (1.0 - Shr.dFck / 250.0) * Shr.dFck * Shr.dBv * Shr.dDv;	
}

BOOL CSeisEvalAIK_G001_2021::CalcFxHingePropConColm(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
	T_SECT_D SectD;
	T_MATD_D MatdD;
	T_PDCM_D PdcmD;
	if ( !GetElemMatlSectCon(ElemK, SectD, MatdD, PdcmD) ) { ASSERT(FALSE); return FALSE; }

	T_REBC_D RebcD;
	if ( !m_pDoc->m_pAttrCtrl->GetRebcByElemK(ElemK, RebcD) ) { ASSERT(FALSE); return FALSE; }

	CSeisEvalDataTool Tool;
	const double dFck = Tool.GetFc(EN_EVALMATL_NOM, PdcmD);
	const double dFym = Tool.GetFym(EN_EVALMATL_NOM, PdcmD);
	const double dAs  = GetRebarAs(RebcD.MainRebarName) * RebcD.nQrb;
	const double dAg  = SectD.SectBefore.SectI.Stiffness.Area;
		
	for ( int i=0; i<EN_MEMBEND_NUM; ++i )
	{
		T_PHGT_FEMA& rCur = i==EN_MEMBEND_I ? rPhgtI : rPhgtJ;

		rCur.bSymmetric = FALSE;
		rCur.bUserDefineRatio = TRUE;
		rCur.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
		SetCurveForcCtrlShear(rCur.dForceMomentRatio);
		SetAccetanceCriteriaForcCtrl(rCur);

		rCur.dYieldForce[IDX_PLS] = CalcPnTensCon(dFym, dAs);
		rCur.dYieldForce[IDX_MNS] = CalcPnCompCon(dFck, dAg, dFym, dAs, RebcD.nHoopType);
	}

	return TRUE;
}

BOOL CSeisEvalAIK_G001_2021::CalcFyzHingePropConColm(T_ELEM_K ElemK, EN_DIR enDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
	double adP[2]  = { 0.0, };
	double adV[2]  = { 0.0, };
	double adM[2]  = { 0.0, };
	double admu[2] = { 0.0, };
	GetPORsltConColmHingeFyz(enDir, PORslt, adP, adV, adM, admu);

	T_COLM_SHR_PARAM aShr[EN_MEMBEND_NUM];
	if ( !MakeColmShearParamPdgn(ElemK, enNLType, enDir, EN_EVALMATL_NOM, aShr) ) { ASSERT(0); return FALSE; }

	for ( int i=0; i<EN_MEMBEND_NUM; ++i )
	{
		T_PHGT_FEMA& rCurPhgt = (i == EN_MEMBEND_I) ? rPhgtI : rPhgtJ;

		rCurPhgt.bSymmetric = FALSE;
		rCurPhgt.bUserDefineRatio = TRUE;
		rCurPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
		SetCurveForcCtrlShear(rCurPhgt.dForceMomentRatio);
		SetAccetanceCriteriaForcCtrl(rCurPhgt);

		double dVn = CalcVnConColm(aShr[i], adP[i]);
		dVn = GetUpdateHingeVnConColm(dVn);

		rCurPhgt.dYieldForce[IDX_PLS] = dVn;
		rCurPhgt.dYieldForce[IDX_MNS] = dVn;
	}

	return TRUE;
}

BOOL CSeisEvalAIK_G001_2021::CalcMyzHingePropConColm(T_ELEM_K ElemK, EN_DIR enDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
	UINT unP = DOF_FX;
	UINT unV = (enDir==EN_DIR_Y) ? DOF_FZ : DOF_FY;
	UINT unM = (enDir==EN_DIR_Y) ? DOF_MY : DOF_MZ;

	double adP[2]  = { 0.0, };
	double adV[2]  = { 0.0, };
	double adM[2]  = { 0.0, };
	double adDy[2] = { 0.0, };
	double adM1y[2] = { 0.0, };
	double adM1z[2] = { 0.0, };
	GetPORsltConColmHingeMyz(enDir, PORslt, adP, adV, adM, adDy, adM1y, adM1z);

	T_SECT_D SectD;
	T_MATD_D MatdD;
	T_PDCM_D PdcmD;
	if ( !GetElemMatlSectCon(ElemK, SectD, MatdD, PdcmD) ) { ASSERT(0); return FALSE; }

	const EN_DIR enDirV = GetPairDirV(enDir);
	T_COLM_SHR_PARAM aShr[EN_MEMBEND_NUM];
	if ( !MakeColmShearParamPdgn(ElemK, enNLType, enDirV, EN_EVALMATL_AVG, aShr) ) { return FALSE; }

	_DB_PMCV_DATA Pmcvy;
	_DB_PMCV_DATA Pmcvz;
	if ( !GetPmcvData(ElemK, EN_EVALSTEP_2ND, EN_EVALMATL_AVG, Pmcvy, Pmcvz) ) { return FALSE; }
	const _DB_PMCV_DATA& CurPmcv = (enDir == EN_DIR_Y) ? Pmcvy : Pmcvz;
	
	T_REBC_D RebcD;
	if ( !m_pDoc->m_pAttrCtrl->GetRebcByElemK(ElemK, RebcD) ) { ASSERT(0); return FALSE; }

	CSeisEvalDataTool Tool;
	const double dfck = Tool.GetFc(EN_EVALMATL_AVG, PdcmD);
	const T_SECT_STIFFNESS& SectStiff = SectD.SectBefore.SectI.Stiffness;

	T_PDHG_CONCOLM_INFO PdhgInfo;
	PdhgInfo.dfck = dfck;
    PdhgInfo.dEc  = GetEc(MatdD.Data1.Analysis.Elast, dfck);

	PdhgInfo.dfy  = Tool.GetFym(EN_EVALMATL_AVG, PdcmD);
	PdhgInfo.dfys = Tool.GetFys(EN_EVALMATL_AVG, PdcmD);
	PdhgInfo.dEs  = CalcEs();
	
	PdhgInfo.bCircle = [] (const CString& strShape)
	{
		if ( strShape == D_SECT_SHAPE_REG_SR ) { return true; }
		if ( strShape == D_SECT_SHAPE_REG_P ) { return true; }
		return false;
	}(SectD.SectBefore.Shape);
	
	PdhgInfo.dAg  = SectStiff.Area;
	PdhgInfo.dAst = GetRebarAs(RebcD.MainRebarName) * RebcD.nQrb;
	PdhgInfo.ddo  = RebcD.dConcCenter;
	PdhgInfo.unDetail = GetSeisDetailConColm(ElemK);
	
	double dShearSpan[EN_MEMBEND_NUM] = { 0.0, };
	GetShearSpanConColm(ElemK, unV, dShearSpan);
	
	T_PDHG_CONCOLM_DIR PdhgDir;	
	PdhgDir.dIg = (enDir == EN_DIR_Y) ? SectStiff.Ryy : SectStiff.Rzz;
	PdhgDir.dIe = PdhgDir.dIg * GetFactorIeConColm(ElemK, enDir);
		
	double dB = 0.0;
	double dH = 0.0;
	GetSectDim(SectD, dB, dH);
	PdhgDir.dB  = (enDir == EN_DIR_Y) ? dB : dH;
	PdhgDir.dH  = (enDir == EN_DIR_Y) ? dH : dB;

	double adM1[2] = { 0.0, };
	memcpy(adM1, (enDir == EN_DIR_Y ? adM1y : adM1z), sizeof(double)*2);
	
	const double dCBRatio = m_HingeCalcParam.dCBRatio;
	const double dc = m_HingeCalcParam.dc;
	const bool bElasticAnal = PORslt.unAnalRsltType == EN_ANAL_ELASTIC ? true : false;

	for ( int i=0; i<EN_MEMBEND_NUM; ++i )
	{
		const T_CON_SHR_PARAM& CurShr = aShr[i].Comm;
		const double dPu = adP[i];
		const double dVu = adV[i];
		const double dMu = adM[i];

		PdhgInfo.dS = CurShr.dSv;

		bool bEndI = (i == EN_MEMBEND_I) ? true : false;
		T_PHGT_FEMA& rCurPhgt = bEndI ? rPhgtI : rPhgtJ;
		
		PdhgDir.dPu = dPu;
		PdhgDir.dVu = dVu;
		PdhgDir.dMu = dMu;

		PdhgDir.dBv = CurShr.dBv;
		PdhgDir.ddv = CurShr.dDv;
		PdhgDir.dAv = CurShr.dAsv;
		PdhgDir.dShearSpan = dShearSpan[i];

		PdhgDir.dMn0 = CalcMnByAxial(CurPmcv, 0.0);
		PdhgDir.dMn = bElasticAnal ? CalcMnByAxial(CurPmcv, dPu) : fabs(adM1[i]); /// [GEN-7027] 해석결과 P1으로 연결.
		PdhgDir.dVy = SafeDiv(PdhgDir.dMn, PdhgDir.dShearSpan);
		PdhgDir.dVc = CalcVcConColm(aShr[i], dPu);
		PdhgDir.dVs = CalcVsFram(CurShr);
		PdhgDir.dVn = PdhgDir.dVc + PdhgDir.dVs;
		PdhgDir.dmu = CalcmuConCom(PdhgInfo.unDetail, PdhgDir.dVc, PdhgDir.dVs, PdhgDir.dVy);
		
		PdhgDir.dThetay = CalcThetayConColm(PORslt.unAnalRsltType, adDy[i], PdhgDir.dMn, PdhgDir.dShearSpan, PdhgInfo.dEc, PdhgDir.dIe);
		PdhgDir.dThetaa = CalcThetaaConColm(PdhgDir.dPu, PdhgDir.dVs, PdhgDir.dThetay);
		PdhgDir.dThetau = CalcThetauConColm(PdhgDir.dmu, PdhgDir.dThetay, PdhgDir.dThetaa);

 		CalcMyzHingePropConColmPos(PdhgInfo, PdhgDir, dCBRatio, dc, rCurPhgt);

		rCurPhgt.nUnloadStiffOption = D_PHGT_FEMA_UNLD_OPTION_ENRG_DISSIPATION;
		const double dDissipation = CalcEnergyDissipationFactorConColm(PdhgInfo, PdhgDir);
		SetPhgtEnergyDissipationOption(dDissipation, rCurPhgt.enrgDissipation);
	}

    if ( !PdhgDir.IsDeformationControlled() )
    {
        SetIgnoreMomentHingeProp(TRUE);
    }

	return TRUE;
}

double CSeisEvalAIK_G001_2021::GetEc(double dEcAnal, double dfck)
{
    const auto enElastType = EN_ELASTIC_MODULUS_TYPE::ANAL; /// 탄성계수 옵션 추가되면 데이터에 추가 (하고 매개변수로 넘겨받아서 ㄱㄱ)
    const auto dEc = (enElastType == EN_ELASTIC_MODULUS_TYPE::ANAL) ? dEcAnal : 8500.0 * pow(dfck, 1.0/3.0);

    return dEc;
}

double CSeisEvalAIK_G001_2021::CalcEs()
{
	return 200000.0; /// MPa.
}

double CSeisEvalAIK_G001_2021::Calcks(double dd) const
{
    /// see [AIK-G-001-2021, EQ(6-2)]
    const auto dd_Meter = dd * 0.001;
    const auto dCal = pow(dgn::SafeDiv(0.3, dd_Meter), 0.25);
    if ( dCal < 0.75 )
        return 0.75;
    if ( dCal > 1.1 )
        return 1.1;

    return dCal;
}

double CSeisEvalAIK_G001_2021::Calccu(const T_PDHG_CONBEAM_INFO& Info, bool bPositive) const
{
    const auto nIterLimit = 25;

    T_CU stCu(Info.dH);
    for ( int nIter = 0; nIter < nIterLimit; ++nIter )
    {
        const auto bOK = Findcu(Info, bPositive, stCu);

        if ( bOK ) break;
    }

    return stCu.dcu;
}

bool CSeisEvalAIK_G001_2021::Findcu(const T_PDHG_CONBEAM_INFO& Info, bool bPositive, T_CU& stCu) const
{
    const auto dTolLimit = 0.001;

    const auto dcu = stCu.dcu;
    T_PDHG_CONBEAM_FLEX FlexTens = bPositive ? Info.FlexBot : Info.FlexTop;
    T_PDHG_CONBEAM_FLEX FlexComp = bPositive ? Info.FlexTop : Info.FlexBot;
    const auto dAs = FlexTens.dAs;
    const auto dfy = Info.dfy;
    const auto dAsc = FlexComp.dAs;
    const auto ddt = FlexTens.dd;
    const auto ddc = Info.dH - FlexComp.dd;
    const auto dEs = Info.dEs;
    const auto dfck = Info.dfck;
    const auto dEspf = 0.05;
    const auto dEspul = SafeDiv(1.0, 3.0 * (dfck + 30.0));
    const auto dEspcf = SafeDiv(dcu * dEspf, ddt - dcu);
    const auto dBeta1 = GetBeta1(dfck);
    const auto dbw = Info.dB;

    const auto dA = dcu * 0.85 * dfck * dBeta1 * dbw;
    const auto dB = dAs * dfy - dAsc * min(dfy, SafeDiv(dcu - ddc, dcu) * dEs * min(dEspul, dEspcf));
    const auto dRatioAB = (EQ0(dB)) ? fabs(dA / D_ZERO_LIMIT) : fabs(dA / dB);
    const auto dTolForce = dRatioAB - 1.0;
    if ( LE(fabs(dTolForce), dTolLimit) )
    {
        return true;
    }

    stCu.dcu = CalcNextcu(dA, dB, dcu, stCu.dcu1, stCu.dcu2);
    return false;
}

double CSeisEvalAIK_G001_2021::CalcNextcu(double dA, double dB, double dcu, double& dcu1, double& dcu2) const
{
    if ( MT(dA, dB) )
    {
        dcu1 = dcu;
    }
    else
    {
        dcu2 = dcu;
    }

    return (dcu1 + dcu2) * 0.5;
}

double CSeisEvalAIK_G001_2021::GetBeta1(double dfck) const
{
    // KDS 14 20 20 : 2021 4.1.1.(8)
    if ( LE(dfck, 40.0) )
    {
        return 0.80;
    }

    if ( MT(dfck, 90.0) )
    {
        return 0.70;
    }

    std::vector<std::pair<double, double>> vBeta1;
    vBeta1.emplace_back(std::make_pair(40.0, 0.80));
    vBeta1.emplace_back(std::make_pair(50.0, 0.80));
    vBeta1.emplace_back(std::make_pair(60.0, 0.76));
    vBeta1.emplace_back(std::make_pair(70.0, 0.74));
    vBeta1.emplace_back(std::make_pair(80.0, 0.72));
    vBeta1.emplace_back(std::make_pair(90.0, 0.70));

    return dgn::math::InterpolationLinear(vBeta1, dfck);
}

std::pair<double, double> CSeisEvalAIK_G001_2021::CalcPlasticDeformationFactors(const T_PDHG_CONBEAM_INFO& Info, bool bPositive, OUT T_PDHG_CONBEAM_PARAM& rParam)
{
    T_PDHG_CONBEAM_FLEX Flex = bPositive ? Info.FlexBot : Info.FlexTop;
    const double dfck = Info.dfck;
    const double dBeta1 = GetBeta1(dfck);
    const double dEc = Info.dEc;
    const double dIe = Info.dIe;
    const double dd = Flex.dd;
    const double dcu = Calccu(Info, bPositive);
    const double djdu = dd - (dBeta1 * dcu) * 0.5;
    const double ddt = Info.dH - Flex.dd1;
    const double ds = Info.dS;
    const double das = Info.dShearSpan;
    const double drhot = Flex.dRho;
    const double dfyt = Info.dfy;
    const double dphiy = SafeDiv(Flex.dMy, dEc * dIe);
    const double dlh = min(dd, max(0.75, SafeDiv(0.5 * das * dd, Info.dH)));
    const double dks = Calcks(ddt);
    const double dft = 0.2 * sqrt(dfck);
    const double drhov = SafeDiv(Info.dAsv, ds * Info.dB);
    const double dfyv = Info.dfys;
    const double dEspf = 0.05;
    const double dEspo = 0.0022;
    const double dEspulNC = 0.0035;
    const double dEspul = SafeDiv(1.0, 3.0 * (dfck + 30.0));
    const double dEspcf = SafeDiv(dcu * dEspf, ddt - dcu);
    const double dEspbl = 3.0 * pow(SafeDiv(ds, Flex.dbi), -2.5);
    const double dEspsc = SafeDiv(dks * sqrt(dft * (dft + 2.0/3.0 * dfck)) * dcu * dEspo,
        (SafeDiv(drhot * dfyt * djdu, das) - drhov * dfyv) * dd);

    const bool bSeisDetail = IsSeisDetailConBeam(Info);
	auto da = 0.02;
	auto db = 0.03;

	const auto enCtrlType = CalcBeamCtrlType(Info.dVp, Info.dVc + Info.dVs);
	switch ( enCtrlType )
	{
	case EN_EVALCTRL_FLEX: 
		{
			const bool bConsiderEspsc = LE(SafeDiv(drhov, drhot), SafeDiv(dfyt * djdu, dfyv * das));
			const auto dMinSeis = bConsiderEspsc ? min(min(min(dEspul, dEspbl), dEspcf), dEspsc) : min(min(dEspul, dEspbl), dEspcf);
			const bool bEspcfGovern = EQ(dEspcf, dMinSeis);
			
			if ( bSeisDetail )
			{
				const auto dphip = SafeDiv(dMinSeis, dcu);
				da = (dphip - dphiy) * dlh;
				db = bEspcfGovern ? da : min((SafeDiv(dEspcf, dcu) - dphiy) * dlh, 2.0 * da);
			}
			else
			{
				const auto dMinNonSeis = bConsiderEspsc ? min(min(min(dEspulNC, dEspbl), dEspcf), dEspsc) : min(min(dEspulNC, dEspbl), dEspcf);
				const auto dphip = SafeDiv(dMinNonSeis, dcu);
				da = min((dphip - dphiy) * dlh, 0.02);
				db = bEspcfGovern ? min(da, 0.03) : min(min((SafeDiv(dEspcf, dcu) - dphiy) * dlh, 2.0 * da), 0.03);
			}
		}
		break;
	case EN_EVALCTRL_SHER:
		{
			da = 0.003;
			db = MT(ds, 0.5*dd)  ? 0.006 : 0.01;			
		}
		break;
	default:
		{
			ASSERT(0);
		}
		break;
	}
    
	rParam.enCtrlType = enCtrlType;
	rParam.bSeisDetail = bSeisDetail;
	rParam.dEspUL = bSeisDetail ? dEspul: dEspulNC;
	rParam.dEspCF = dEspcf;
	rParam.dEspBL = dEspbl;
	rParam.dEspSC = dEspsc;
	rParam.dcu = dcu;
	rParam.dPhiy = dphiy;
    rParam.dlh = dlh;
	
    return std::make_pair(da, db);
}

UINT CSeisEvalAIK_G001_2021::GetSeisDetailConBeam(T_ELEM_K ElemK)
{
	T_SEBE_D SebeD;
	if ( m_pDoc->m_pAttrCtrl->GetSebe(ElemK, SebeD) )
	{
		return SebeD.nDetail;
	}

	T_SEME_D SemeD;
	if ( m_pDoc->m_pAttrCtrl2->GetSeme(SemeD) )
	{
		return SemeD.data.design.nSeismicDetailBeam;
	}

	ASSERT(0);
	return EN_SEBE_DETAIL::EN_SEBE_DETL_NONE;
}

UINT CSeisEvalAIK_G001_2021::GetSeisDetailConColm(T_ELEM_K ElemK)
{
	T_SECO_D SecoD;
	if ( m_pDoc->m_pAttrCtrl->GetSeco(ElemK, SecoD) )
	{
		return SecoD.nDetail;
	}

	T_SEME_D SemeD;
	if ( m_pDoc->m_pAttrCtrl2->GetSeme(SemeD) )
	{
		return SemeD.data.design.nSeismicDetailColm;
	}

	ASSERT(0);
	return EN_SECO_DETAIL::EN_SECO_DETL_NONE;
}

UINT CSeisEvalAIK_G001_2021::GetSeisDetailConWall(T_ELEM_K ElemK)
{
	T_SEWA_D SewaD;
	if ( m_pDoc->m_pAttrCtrl->GetSewa(ElemK, SewaD) )
	{
		return SewaD.nDetail;
	}

	T_SEME_D SemeD;
	if ( m_pDoc->m_pAttrCtrl2->GetSeme(SemeD) )
	{
		return SemeD.data.design.nSeismicDetailWall;
	}

	ASSERT(0);
	return EN_SEWA_BNDR_WALL::EN_SEWA_BNDR_NONE;
}

double CSeisEvalAIK_G001_2021::GetLpRatioConWall(T_ELEM_K ElemK)
{
	T_HLFW_D HlfwD;
	if ( m_pDoc->m_pAttrCtrl->GetHlfw(ElemK, HlfwD) )
	{
		return HlfwD.dLp;
	}

	T_SEME_D SemeD;
	if ( m_pDoc->m_pAttrCtrl2->GetSeme(SemeD) )
	{
		return SemeD.data.design.dPlasticHingeLengthFactor;
	}
	ASSERT(0);
	return 0.5;
}

bool CSeisEvalAIK_G001_2021::IsSeisDetailConBeam(const T_PDHG_CONBEAM_INFO& PdhgInfo)
{
	switch ( PdhgInfo.unDetail )
	{
	case EN_SEBE_DETL_NONE:
		{
			return false;
		}
	case EN_SEBE_DETL_SEIS:
		{
			return true;
		}
	case EN_SEBE_DETL_AUTO:
		{
			const double dd = min(PdhgInfo.FlexBot.dd, PdhgInfo.FlexTop.dd); /// 하부철근 d,상부철근 d값 중 최소값 적용.
			if ( !IsSeismicDetail_1(PdhgInfo.dS, dd) ) { return false; }
			if ( !IsSeismicDetail_2(PdhgInfo.dVs, PdhgInfo.dVu) ) { return false; }
			return true;
		}
	default:
		{
			ASSERT(0);
			return false;
		}
	}
}

void CSeisEvalAIK_G001_2021::GetShearSpanConBeam(T_ELEM_K ElemK, OUT double radLs[EN_MEMBEND_NUM])
{
	memset(radLs, 0.0, EN_MEMBEND_NUM);

	T_SESP_D SespD;
	if ( !m_pDoc->m_pAttrCtrl->GetSesp(ElemK, SespD) )
	{
		T_SEME_D SemeD;
		if ( m_pDoc->m_pAttrCtrl2->GetSeme(SemeD) )
		{
			const T_SEME_PFRM& Pfrm = SemeD.data.design;
			SespD.bReal = FALSE; /// Global에서는 부재별 길이를 입력할 수 없으므로, relative length 로만 처리됩니다.
			SespD.dLszi = Pfrm.dShearSpanLs_z_Beam_i;
			SespD.dLszj = Pfrm.dShearSpanLs_z_Beam_j;
		}
	}

	if ( SespD.bReal )
	{
		radLs[EN_MEMBEND_I] = SespD.dLszi;
		radLs[EN_MEMBEND_J] = SespD.dLszj;
	}
	else
	{
		CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();
		const double dL = pMembCtrl->GetMembLengByIncludeElem(ElemK);

		radLs[EN_MEMBEND_I] = SespD.dLszi * dL;
		radLs[EN_MEMBEND_J] = SespD.dLszj * dL;
	}
}

void CSeisEvalAIK_G001_2021::GetShearSpanConColm(T_ELEM_K ElemK, int nDof, OUT double radLs[EN_MEMBEND_NUM])
{
	memset(radLs, 0.0, EN_MEMBEND_NUM);

	T_SESP_D SespD;
	if ( !m_pDoc->m_pAttrCtrl->GetSesp(ElemK, SespD) )
	{
		T_SEME_D SemeD;
		if ( m_pDoc->m_pAttrCtrl2->GetSeme(SemeD) )
		{
			const T_SEME_PFRM& Pfrm = SemeD.data.design;
			SespD.bReal = FALSE; /// Global에서는 부재별 길이를 입력할 수 없으므로, relative length 로만 처리됩니다.
			SespD.dLsyi = Pfrm.dShearSpanLs_y_Colm_i;
			SespD.dLsyj = Pfrm.dShearSpanLs_y_Colm_j;
			SespD.dLszi = Pfrm.dShearSpanLs_z_Colm_i;
			SespD.dLszj = Pfrm.dShearSpanLs_z_Colm_j;
		}
	}

	const bool bDirVy = nDof == D_DOF_FY ? true : false;
	const double dValI = bDirVy ? SespD.dLsyi : SespD.dLszi;
	const double dValJ = bDirVy ? SespD.dLsyj : SespD.dLszj;

	if ( SespD.bReal )
	{
		radLs[EN_MEMBEND_I] = dValI;
		radLs[EN_MEMBEND_J] = dValJ;
	}
	else
	{
		double dL = 0.0;
		T_SECH_D SechD;		
		if ( m_pDoc->m_pAttrCtrl->GetSech(ElemK, SechD) )
		{
			dL = bDirVy ? SechD.dh0y : SechD.dh0z;
		}
		else
		{
			CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();
			dL = pMembCtrl->GetMembLengByIncludeElem(ElemK);			
		}

		radLs[EN_MEMBEND_I] = dValI * dL;
		radLs[EN_MEMBEND_J] = dValJ * dL;		
	}
}

double CSeisEvalAIK_G001_2021::GetAppSectionStiffnessFactor(T_ELEM_K ElemK, int nDOF)
{
	T_ESSF_D EssfD;
	if ( m_pDoc->m_pAttrCtrl->GetEssfElem(ElemK, EssfD) )
	{
		return EssfD.dScaleFactor[nDOF];
	}

	T_ELEM_D ElemD;
	if ( !m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD) ) { ASSERT(0); return 1.0; }

	T_SECF_D SecfD;
	if ( m_pDoc->m_pAttrCtrl->GetSecfSect(ElemD.elpro, SecfD) )
	{
		return SecfD.dScaleFactor[nDOF];
	}

	return 1.0;
}

double CSeisEvalAIK_G001_2021::GetFactorIeConBeam(T_ELEM_K ElemK)
{
	return GetAppSectionStiffnessFactor(ElemK, 4);
}

double CSeisEvalAIK_G001_2021::GetFactorIeConColm(T_ELEM_K ElemK, EN_DIR enDir)
{
	return GetAppSectionStiffnessFactor(ElemK, (enDir == EN_DIR_Y ? 4 : 5));	
}

double CSeisEvalAIK_G001_2021::GetFactorIeConWall(T_ELEM_K ElemK)
{
	T_WSSF_D WssfD;
	if ( m_pDoc->m_pAttrCtrl->GetWssfElem(ElemK, WssfD) )
	{
		return WssfD.dBending;
	}

	return 1.0;
}

bool CSeisEvalAIK_G001_2021::MakeBeamInfo(T_ELEM_K ElemK, const EN_NLANALTYPE& enAnalType, double adRatioMuMy[EN_MEMBEND_NUM], double adV[EN_MEMBEND_NUM], double adM[EN_MEMBEND_NUM], OUT T_PDHG_CONBEAM_INFO raInfo[EN_MEMBEND_NUM])
{
	for ( int i=0; i<EN_MEMBEND_NUM; ++i )
	{
		raInfo[i].Initialize();
	}

	T_SECT_D SectD;
	T_MATD_D MatdD;
	T_PDCM_D PdcmD;
	if ( !GetElemMatlSectCon(ElemK, SectD, MatdD, PdcmD) ) { ASSERT(0); return false; }
		
	double dB = 0.0;
	double dH = 0.0;
	if ( !GetSectDim(SectD, dB, dH) ) { ASSERT(0); return false; }

	T_REBB_D RebbD;
	if ( !m_pDoc->m_pAttrCtrl->GetRebbByElemK(ElemK, RebbD) ) { ASSERT(0); return false; }

	CSeisEvalDataTool Tool;	
	const double dFce  = Tool.GetFc(EN_EVALMATL_AVG, PdcmD);
	const double dFyme = Tool.GetFym(EN_EVALMATL_AVG, PdcmD);
	const double dFyse = Tool.GetFys(EN_EVALMATL_AVG, PdcmD);
	const double dL    = CMembCtrl::GetMembCtrl()->GetMembLengByIncludeElem(ElemK);

	double adLs[EN_MEMBEND_NUM] = { 0.0, };
	GetShearSpanConBeam(ElemK, adLs);
		
	T_BEAM_SHR_PARAM aShr[EN_BEAM_POS_NUM];
	MakeBeamShearParam(ElemK, enAnalType, aShr);
	
	auto L_CalcConBeamFlex = [this](double dB, double dH, double dfce, double dfye, const T_REBB_BASE& RebbBase, double dDc, OUT T_PDHG_CONBEAM_FLEX& rFlex)
	{
		const double dd  = CalcDeffTensBarCentroid(dH, RebbBase, dDc);
		const double dAs = CalcAs(RebbBase);
		const double da  = CalcStressBlockHeightConBeam(dfce, dfye, dAs, dB);

		const double dDistLayer = GetDistLayerConBeam();
		rFlex.dbi  = GetRebarDia(RebbBase.RebarName);
		rFlex.dd1  = dDc;
		rFlex.dd2  = dDc + dDistLayer + GetRebarDia(RebbBase.RebarName)/2.0 + GetRebarDia(RebbBase.RebarName2nd)/2.0;
		rFlex.dAs1 = RebbBase.nRebar1 * GetRebarAs(RebbBase.RebarName);
		rFlex.dAs2 = RebbBase.nRebar2 * GetRebarAs(RebbBase.RebarName2nd);		
		rFlex.dd   = dd;
		rFlex.dAs  = dAs;
		rFlex.dRho = fabs(dB*dd) < cEvalZero ? 0.0 : CalcAs(RebbBase)/(dB*dd);
		rFlex.da   = da;
        rFlex.dMe  = fabs(dfce*dB) < cEvalZero ? 0.0 : dAs*dfye*(dd-da/2.0);  /// see [MOE2018, (5-1)]
	};
	
	L_CalcConBeamFlex(dB, dH, dFce, dFyme, RebbD.Top_I, RebbD.dT, raInfo[EN_MEMBEND_I].FlexTop);
	L_CalcConBeamFlex(dB, dH, dFce, dFyme, RebbD.Bot_I, RebbD.dB, raInfo[EN_MEMBEND_I].FlexBot);
	L_CalcConBeamFlex(dB, dH, dFce, dFyme, RebbD.Top_J, RebbD.dT, raInfo[EN_MEMBEND_J].FlexTop);
	L_CalcConBeamFlex(dB, dH, dFce, dFyme, RebbD.Bot_J, RebbD.dB, raInfo[EN_MEMBEND_J].FlexBot);

	auto L_CalcMy = [this] (const T_PDHG_CONBEAM_INFO& Info, bool bPositive) -> double
	{
		T_PDHG_CONBEAM_FLEX Flex = bPositive ? Info.FlexBot : Info.FlexTop;
		const double dcu = Calccu(Info, bPositive);
		const double dBeta1 = GetBeta1(Info.dfck);
		const double dd = Flex.dd;
		const double djdu = dd - (dBeta1 * dcu) * 0.5;
		const double ddt = Info.dH - Flex.dd1;
		const double ds = Info.dS;
		const double das = Info.dShearSpan;
		const double drhot = Flex.dRho;
		const double dfyt = Info.dfy;

		return drhot * dfyt * Info.dB * dd * djdu;
	};
	
	const double dFactIe = GetFactorIeConBeam(ElemK);	
	for ( int i=0; i<EN_MEMBEND_NUM; ++i )
	{
		const EN_BEAM_POS& enPos = GetBeamPos(static_cast<EN_MEMBEND>(i));
		const T_CON_SHR_PARAM& CurShr = aShr[enPos].GetParam(TRUE, adM[i]);

		T_PDHG_CONBEAM_INFO& Info = raInfo[i];		
		Info.dRatioMuMy = adRatioMuMy[i];
		Info.dB = dB;
		Info.dH = dH;
		Info.dIe  = SectD.SectBefore.SectI.Stiffness.Ryy * dFactIe;

		Info.dfck = dFce;
        Info.dEc  = GetEc(MatdD.Data1.Analysis.Elast, dFce);

		Info.dfy  = dFyme;
		Info.dfys = dFyse;
		Info.dEs  = CalcEs();

		Info.dMu = adM[i];
		Info.dVu = adV[i];		
				
		Info.unDetail = GetSeisDetailConBeam(ElemK);		
		Info.dShearSpan = adLs[i];
		Info.dAsv = CurShr.dAsv;
		Info.dVs = CalcVsFram(CurShr);	
		Info.dS = CurShr.dSv;		

		Info.FlexTop.dMy = L_CalcMy(Info, false);
		Info.FlexBot.dMy = L_CalcMy(Info, true);
		Info.dVc = CalcVcConBeam(Info, CurShr, aShr[enPos].nShrMethod);
	}
	
	double dMeiP = raInfo[EN_MEMBEND_I].FlexBot.dMy;
	double dMeiN = raInfo[EN_MEMBEND_I].FlexTop.dMy;
	double dMejP = raInfo[EN_MEMBEND_J].FlexBot.dMy;
	double dMejN = raInfo[EN_MEMBEND_J].FlexTop.dMy;

	for (int i = 0; i < EN_MEMBEND_NUM; ++i)
	{		
		raInfo[i].dVp = CalcVpConBeam(dMeiP, dMeiN, dMejP, dMejN, adLs[i]);
	}
	    
	return true;
}

bool CSeisEvalAIK_G001_2021::MakeBeamShearParam(T_ELEM_K ElemK, const EN_NLANALTYPE& enAnalType, OUT T_BEAM_SHR_PARAM raShr[EN_BEAM_POS_NUM])
{
	for ( int i=0; i<EN_BEAM_POS_NUM; ++i )
	{
		raShr[i].Initialize();
	}

	///[GEN-5947] DATA SET.
	T_SECT_D SectD;
	T_MATD_D MatdD;
	T_PDCM_D PdcmD;
	if ( !GetElemMatlSectCon(ElemK, SectD, MatdD, PdcmD) ) { ASSERT(0); return false; }
	if ( PdcmD.bUseDefaultVal ) { return false; }

	double dB = 0.0;
	double dH = 0.0;
	if ( !GetSectDim(SectD, dB, dH) ) { ASSERT(0); return false; }

	T_REBB_D RebbD;
	if ( !m_pDoc->m_pAttrCtrl->GetRebbByElemK(ElemK, RebbD) ) { ASSERT(0); return false; }

	CSeisEvalDataTool Tool;
	const int    nDeffType = Tool.GetDeffTypeConBeam(enAnalType);
	const double dAsv1 = GetRebarAs(RebbD.SubRebarName);

	for ( int i=0; i<EN_BEAM_POS_NUM; ++i )
	{
		T_BEAM_SHR_PARAM& rCur = raShr[i];
		rCur.ParamPos.dFck = Tool.GetFc(EN_EVALMATL_NOM, PdcmD);
		rCur.ParamPos.dFys = Tool.GetFys(EN_EVALMATL_NOM, PdcmD);

		T_REBB_BASE CurTop;
		T_REBB_BASE CurBot;
		GetRebbBase(RebbD, static_cast<EN_BEAM_POS>(i), CurTop, CurBot);

		rCur.nShrMethod    = GetShearStrengthMethodConBeam(enAnalType);
		rCur.ParamPos.dBv  = dB;
		rCur.ParamPos.dDv  = CalcDeffShear(nDeffType, dH, CurBot, RebbD.dB);
		/// RC 보의 전단철근 정보는 Top에 저장되어 있습니다.
		rCur.ParamPos.dAsv = dAsv1 * CurTop.iSubRebarNum;
		rCur.ParamPos.dSv  = CurTop.dSubRebarSpace;

		rCur.ParamNeg = rCur.ParamPos;
		rCur.ParamNeg.dDv  = CalcDeffShear(nDeffType, dH, CurTop, RebbD.dT);
	}

	return true;
}

bool CSeisEvalAIK_G001_2021::MakeWallShearParam(T_ELEM_K ElemK, EN_DIR enDir, OUT T_WALL_SHR_PARAM raShr[EN_MEMBEND_NUM])
{
	for ( int i=0; i<EN_MEMBEND_NUM; ++i )
	{
		raShr[i].Initialize();
	}

	T_WALL_KEY WallKey;
	T_WALL_D WallD;  WallD.Initialize();
	if ( !m_pDoc->m_pPostCtrl->GetWallDataFromElemK(ElemK, WallD, &WallKey) ) { ASSERT(0); return false; }
	if ( !WallD.bLineWall ) { return false; }

	T_THIK_D ThikD;
	T_MATD_D MatdD;
	T_PDCM_D PdcmD;
	if ( !GetElemMatlThik(ElemK, ThikD, MatdD, PdcmD) ) { ASSERT(0); return false; }

	T_REBW_K RebwK;
	RebwK.keymap = WallKey;
	T_REBW_D RebwD;
	if ( !m_pDoc->m_pAttrCtrl->GetRebw(RebwK, RebwD) ) { ASSERT(0); return false; }

	double dTw = WallD.dThickness;
	double dLw = WallD.dLength;

	T_WALL_SHR_PARAM Shr;
	GetWallMatlShear(PdcmD, Shr.Comm);

	Shr.Comm.dLambda = GetConLambda(MatdD);
	Shr.Comm.dAsv = 2.0*GetRebarAs(RebwD.HorizonRebarName);
	Shr.Comm.dSv  = RebwD.dHorizonRebarSpace;

	CSeisEvalDataTool Tool;
	Shr.bVnmaxEQ = Tool.IsSpecialShearWallCondition();
	Shr.dLw = dLw;
	Shr.dTotalHw = GetContinuousWallTotalHw(ElemK, WallD.dHeight);
	
	UINT unDofV = 0;
	UINT unDofM = 0;
	switch ( enDir )
	{
	case EN_DIR_Y:
		{
			unDofV = DOF_FY;
			unDofM = DOF_MZ;

			int nDeffType = Tool.GetDeffTypeConWall(EN_NLANALTYPE::PO);
			Shr.Comm.dBv = dLw;
			Shr.Comm.dDv = CalcDeffShear(nDeffType, dTw, RebwD.dDw);
			Shr.dLw = dTw;
		}
		break;
	case EN_DIR_Z:
		{
			unDofV = DOF_FZ;
			unDofM = DOF_MY;

			int nDeffType = Tool.GetDeffTypeConWall(EN_NLANALTYPE::PO);
			Shr.Comm.dBv = dTw;
			Shr.Comm.dDv = CalcDeffShear(nDeffType, dLw, RebwD.dDe);
			Shr.dLw = dLw;
		}
		break;
	default: ASSERT(0);
		break;
	}

	for ( int i=0; i<EN_MEMBEND_NUM; ++i )
	{
		raShr[i] = Shr;
	}

	return true;
}

double CSeisEvalAIK_G001_2021::CalcPnTensCon(double dfy, double dAs)
{
	return dfy*dAs;
}

double CSeisEvalAIK_G001_2021::CalcPnCompCon(double dfck, double dAg, double dfy, double dAs, int nHoopType)
{
	return (nHoopType == 1 ? 0.8 : 0.85)*(0.85*dfck*(dAg - dAs) + dfy*dAs);
}

int CSeisEvalAIK_G001_2021::GetShearStrengthMethodConBeam(const EN_NLANALTYPE& enAnalType)
{
	T_SEME_D SemeD;
	if ( !m_pDoc->m_pAttrCtrl2->GetSeme(SemeD) )
	{
		SemeD.Initialize();		
	}

	/// [GEN-7231] 옵션이 분리되기 전까지는 기둥과 같은 옵션 사용.
	const T_SEME_PFRM& Pfrm = SemeD.data.design;
	switch ( enAnalType )
	{
	case EN_NLANALTYPE::PO: return Pfrm.nShearStrengthColm_PO;
	case EN_NLANALTYPE::TH: return Pfrm.nShearStrengthColm_TH;		
	default: ASSERT(0); return Pfrm.nShearStrengthColm_PO;
	}
}

int CSeisEvalAIK_G001_2021::GetMomentMethodVcConBeam()
{
	T_SEME_D SemeD;
	if ( m_pDoc->m_pAttrCtrl2->GetSeme(SemeD) )
	{
		return SemeD.data.design.nMomentVcBeamHinge;
	}

	ASSERT(0);
	return D_SEME_MU;
}

double CSeisEvalAIK_G001_2021::CalcSigmact(double dMu, double dbw, double dcu, double djdu, double dfck)
{
	/// see [AIK-G-001-2021, EQ(6-5)]
	double dSigct = dgn::SafeDivAbs(dMu, dbw*dcu*djdu);
	return min(dSigct, 2.0/3.0*dfck);
}

double CSeisEvalAIK_G001_2021::CalcVcConBeam(const T_PDHG_CONBEAM_INFO& Info, const T_CON_SHR_PARAM& Shr, int nShrMethod)
{
	if ( nShrMethod == D_SEME_SHRSTR_KDS_2022 )
	{
		return 1.0/6.0 * sqrt(Shr.dFck) * Shr.dBv * Shr.dDv;
	}
	
	const double dM_sigct = [Info] (int nMethod)
	{
		const double dMy = Info.GetFlexByMu().dMy;		
		switch ( nMethod )
		{
		case D_SEME_MU:
			return dMy * max(1.0, Info.dRatioMuMy);
		case D_SEME_MY:
			return dMy;
		default:
			ASSERT(0);
			return dMy;
		}
	}(GetMomentMethodVcConBeam());

	const bool bPositive = Info.dMu < 0.0 ? false : true;
	const T_PDHG_CONBEAM_FLEX& Flex = bPositive ? Info.FlexBot : Info.FlexTop;
	const double dks = Calcks(Shr.dDv);
	const double dbw = Shr.dBv;
    const double dfck = Shr.dFck;
    const double dBeta1 = GetBeta1(dfck);
    const double dft = 0.2*sqrt(dfck); /// see [AIK-G-001-2021, EQ(6-3)]	
    const double dRhot = Flex.dRho;
    const double dfy = Info.dfy;
    const double das = Info.dShearSpan;	
    const double dRhov = SafeDiv(Info.dAsv, Info.dB*Info.dS);
    const double dfyv = Shr.dFys;
	const double dd = Flex.dd;	
	const double dcu = Calccu(Info, bPositive);
    const double djdu = dd - (dBeta1 * dcu) * 0.5;
	const double dSigct = CalcSigmact(dM_sigct, dbw, dcu, djdu, Shr.dFck);
	const double dCotTheta = sqrt(1.0 + SafeDiv(dSigct, dft));

    const double dNumerator = (SafeDiv(dRhot * dfy * djdu, das) - dRhov * dfyv) * dd;
    const double dDenominator = dks * sqrt(dft * (dft + 2.0/3.0 * dfck)) * dcu;
    const double dEspoEspsc = SafeDiv(dNumerator, dDenominator);
	/// see [AIK-G-001-2021, EQ(6-1)]
    const bool bIgnoreEsp = LE0(dEspoEspsc) || AbsME(dEspoEspsc, 1.0);
    const double dVc = bIgnoreEsp ? dks * dft * dbw * dcu * dCotTheta
        : dks * dft * dbw * dcu * dCotTheta * dEspoEspsc;

    return dVc;
}

double CSeisEvalAIK_G001_2021::CalcVnConBeam(const T_PDHG_CONBEAM_INFO& Info, const T_CON_SHR_PARAM& Shr, int nShrMethod)
{
	const double dVc = CalcVcConBeam(Info, Shr, nShrMethod);
	const double dVs = CalcVsFram(Shr);
	return dVc + dVs;
}

double CSeisEvalAIK_G001_2021::CalcVcConColm(const T_COLM_SHR_PARAM& Shr, double dPu)
{
	const double db = Shr.Comm.dBv;
	const double dd = Shr.Comm.dDv;
	const double dS = Shr.Comm.dSv;
	const double dfck = Shr.Comm.dFck;
	const double dAg  = Shr.dAg;
	const double dLambda = Shr.Comm.dLambda;

	switch ( Shr.nShrMethod )
	{
	case D_SEME_SHRSTR_AIK_2021: /// see [AIK-G-001-2021, EQ(6-15)]
		{
			return 1.0/6.0*dLambda*(1.0 + dgn::SafeDiv(dPu, 14.0*dAg))*sqrt(dfck)*db*dd;
		}
	case D_SEME_SHRSTR_KDS_2022:
		{
			if ( EQ0(dPu) )
			{
				return 1.0/6.0*dLambda*sqrt(dfck)*db*dd; /// [KDS 14 20 22 :2022, (4.2-1)]
			}
			else if ( dPu > 0.0 )
			{
				return 1.0/6.0*dLambda*(1.0 + SafeDiv(dPu, 14.0*dAg))*sqrt(dfck)*db*dd; /// [KDS 14 20 22 :2022, (4.2-2)]
			}
			else
			{
				return 1.0/6.0*dLambda*(1.0 + SafeDiv(dPu, 3.5*dAg))*sqrt(dfck)*db*dd; /// [KDS 14 20 22 :2022, (4.2-6)]
			}
		}
	default:
		{
			ASSERT(0);
			return 0.0;
		}		
	}
}

double CSeisEvalAIK_G001_2021::CalcVnConColm(const T_COLM_SHR_PARAM& Shr, double dPu)
{
	return CalcVcConColm(Shr, dPu) + CalcVsFram(Shr.Comm);
}

double CSeisEvalAIK_G001_2021::CalcVnConWall(const T_WALL_SHR_PARAM& Shr)
{
	const double dfck = Shr.Comm.dFck;
	const double dd = Shr.Comm.dDv;
	const double dTw = Shr.Comm.dBv;
	const double dLw = Shr.dLw;
	const double dPu = Shr.dP;
	const double dVu = Shr.dV;
	const double dMu = Shr.dM;

	double dVc = fabs(dLw) < cEvalZero ? 0.0 : 0.28 * sqrt(dfck) * dTw * dd + dPu * dd / (4.0 * dLw); // dPu : (+) compression, (-) tension.
	double dMV = fabs(dVu) < cEvalZero ? 0.0 : fabs(dMu / dVu);
	if (dMV - dLw / 2.0 > 0.0)
	{
		double dVc2 = (0.05 * sqrt(dfck) + dLw * (0.1 * sqrt(dfck) + 0.2 * dPu / (dLw * dTw)) / (dMV - dLw / 2.0)) * dTw * dd;
		dVc = min(dVc, dVc2);
	}
	if (dVc < 0.0) dVc = 0.0;

	double dVs = CalcVsWall(Shr, dVc);
	return dVc + dVs;
}

double CSeisEvalAIK_G001_2021::CalcmuConCom(UINT nSeisDetail, double dVc, double dVs, double dVy)
{
	/// see [Table 6-5]
	switch ( nSeisDetail )
	{
	case EN_SECO_DETL_NONE:
		{
            return (dVs < dVy) ? 4.0 - 2.0*SafeDiv(dVy - dVs, dVc) : 4.0;
		}
	case EN_SECO_DETL_SEIS:		
		{
            return (dVs < dVy) ? 5.0 - 3.0*SafeDiv(dVy - dVs, dVc) : 5.0;
        }
	default:
		{
			ASSERT(0);
			return 4.0;
		}
	}
}

BOOL CSeisEvalAIK_G001_2021::CalcMyHingePropConBeamPos(const T_PDHG_CONBEAM_INFO& Info, double dCBRatio, double dc, OUT T_PHGT_FEMA& rPhgt)
{
	rPhgt.bSymmetric = FALSE;
	rPhgt.bUserDefineRatio = TRUE;

	T_PDHG_CONBEAM_PARAM ParamP;
    const auto pdFactorP = CalcPlasticDeformationFactors(Info, true, ParamP);
	T_PDHG_CONBEAM_PARAM ParamN;
    const auto pdFactorN = CalcPlasticDeformationFactors(Info, false, ParamN);

    const auto daP = pdFactorP.first;
    const auto dbP = pdFactorP.second;
    const auto daN = pdFactorN.first;
    const auto dbN = pdFactorN.second;

	rPhgt.dYieldMoment[IDX_PLS] = Info.FlexBot.dMy;
	rPhgt.dYieldMoment[IDX_MNS] = Info.FlexTop.dMy;

	SetCurveFromTableConBeam(daN, dbN, Info.dThetayN, daP, dbP, Info.dThetayP, dCBRatio, dc, rPhgt.dForceMomentRatio);

	const auto bsGTd2P = Info.dS > 0.5*Info.FlexBot.dd ? true : false;
	const auto bsGTd2N = Info.dS > 0.5*Info.FlexTop.dd ? true : false;

	const double dIOmin = IsSeisDetailConBeam(Info) ? 0.0035 : 0.0017;
	CalcAcceptablePlasticHingeRotationBeam(ParamP.enCtrlType, bsGTd2P, daP, dbP, Info.dThetayP, dIOmin, rPhgt.dIO[IDX_PLS], rPhgt.dLS[IDX_PLS], rPhgt.dCP[IDX_PLS]); /// for 정모멘트 : 하부철근.	
	CalcAcceptablePlasticHingeRotationBeam(ParamN.enCtrlType, bsGTd2N, daN, dbN, Info.dThetayN, dIOmin, rPhgt.dIO[IDX_MNS], rPhgt.dLS[IDX_MNS], rPhgt.dCP[IDX_MNS]); /// for 부모멘트 : 상부철근.
	
	return TRUE;
}

BOOL CSeisEvalAIK_G001_2021::CalcMyzHingePropConColmPos(const T_PDHG_CONCOLM_INFO& PdhgInfo, const T_PDHG_CONCOLM_DIR& PdhgDir,
	double dCBRatio, double dc, OUT T_PHGT_FEMA& rPhgt)
{
	rPhgt.bSymmetric = FALSE;
	rPhgt.bUserDefineRatio = TRUE;

	double da = 0.0;
	double db = 0.0;
	if ( PdhgDir.IsDeformationControlled() )
	{
		da = PdhgDir.dThetau - PdhgDir.dThetay;
		db = PdhgDir.dThetaa - PdhgDir.dThetay;

		SetCurveFromTableConColm(da, db, PdhgDir.dThetay, dCBRatio, dc, rPhgt.dForceMomentRatio);

        CalcAcceptablePlasticHingeRotationColm(da, db, PdhgDir.dThetay, rPhgt.dIO[IDX_PLS], rPhgt.dLS[IDX_PLS], rPhgt.dCP[IDX_PLS]);
        rPhgt.dIO[IDX_MNS] = rPhgt.dIO[IDX_PLS];
        rPhgt.dLS[IDX_MNS] = rPhgt.dLS[IDX_PLS];
        rPhgt.dCP[IDX_MNS] = rPhgt.dCP[IDX_PLS];
	}
	else
	{
		/// 힘 지배 거동은 취성힌지를 만듭니다.
		rPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
		SetCurveForcCtrlShear(rPhgt.dForceMomentRatio);
		SetAccetanceCriteriaForcCtrl(rPhgt);				
	}

	return TRUE;
}

UINT CSeisEvalAIK_G001_2021::GetPOThetayConColmAnalType()
{
	return CProduct::GetTestEnvValue(_ULS(SPD_THETAY_PO_PMM)) == _ULS(no) ? EN_ANAL_ELASTIC : EN_ANAL_NONLINEAR_PO;
}

UINT CSeisEvalAIK_G001_2021::GetTHThetayConColmAnalType()
{
	return CProduct::GetTestEnvValue(_ULS(SPD_THETAY_PO_PMM)) == _ULS(no) ? EN_ANAL_ELASTIC : EN_ANAL_NONLINEAR_TH;
}

double CSeisEvalAIK_G001_2021::CalcThetayConColm(UINT unAnalRsltType, double dAnalDy, double dMn, double dls, double dE, double dIe)
{
	/// [Table 6-4]
	switch ( unAnalRsltType )
	{
	case EN_ANAL_NONLINEAR_PO:
	case EN_ANAL_NONLINEAR_TH:
		return dAnalDy;
	case EN_ANAL_ELASTIC: 
		return SafeDiv(dMn*dls, 3.0*dE*dIe);
	default: 
		ASSERT(0);
		return 0.0;
	}
}

double CSeisEvalAIK_G001_2021::CalcThetaaConColm(double dNu, double dVs, double dThetay)
{
	/// [Table 6-4]
	const double dVal = SafeDiv(0.48, 4.6 + SafeDiv(dNu, dVs));
	return max(dVal, 2.0*dThetay);
}

double CSeisEvalAIK_G001_2021::CalcThetauConColm(double dmu, double dThetay, double dThetaa)
{
	/// [Table 6-4]
	return min(dmu*dThetay, 0.8*dThetaa);
}

double CSeisEvalAIK_G001_2021::CalcThetapConColm(double dThetamax, double dThetay)
{
	return max(dThetamax - dThetay, 0.0);
}

void CSeisEvalAIK_G001_2021::CalcSeisEval2ndRsltConColmDir(UINT unAnalRsltType, const T_PDHG_CONCOLM_INFO& PdhgInfo, const T_COLM_SHR_PARAM& ShrParam, bool bFiber,
	double dAnalThetay, double dThetamax, const T_PDHG_CONCOLM_DIR& PdhgDir, OUT T_EVAL2ND_COLM_DIR& rDir, OUT T_EVAL2ND_PHNE& rPhneD)
{
	const double dPu = PdhgDir.dPu;	
	const double dMn = PdhgDir.dMn;
	const double dVy = SafeDiv(PdhgDir.dMn, PdhgDir.dShearSpan);
	const double dVc = CalcVcConColm(ShrParam, dPu);
	const double dVs = CalcVsFram(ShrParam.Comm);
	const double dVn = dVc + dVs;
	const double dmu = CalcmuConCom(PdhgInfo.unDetail, dVc, dVs, dVy);

	const double dThetay = CalcThetayConColm(unAnalRsltType, dAnalThetay, dMn, PdhgDir.dShearSpan, PdhgInfo.dEc, PdhgDir.dIe);
	const double dThetaa = CalcThetaaConColm(dPu, dVs, dThetay);
	const double dThetau = CalcThetauConColm(dmu, dThetay, dThetaa);	
	const double da = dThetau - dThetay;
	const double db = dThetaa - dThetay;

	constexpr double eps = std::numeric_limits<double>::epsilon();

	rDir.dVc = dVc;
	rDir.dVs = dVs;
	rDir.dVp = dVy;
	rDir.bSeisConform = PdhgInfo.unDetail == EN_SECO_DETL_SEIS ? true : false;
	rDir.dmu = dmu;
	rDir.dThetau = dThetau;
	rDir.dThetaa = dThetaa;
	rDir.dThetamax = dThetamax;
	rDir.dThetay   = dThetay;
	rDir.dThetap   = CalcThetapConColm(dThetamax, dThetay);
	rDir.enCtrlType = PdhgDir.IsDeformationControlled() ? EN_EVALCTRL_FLEX : EN_EVALCTRL_SHER;
	
	double dIndex;
	if (fabs(dThetay) < eps)
	{
		dIndex = fabs(dThetay / eps);
	}
	else
	{
		dIndex = SafeDivAbs(dThetamax, dThetay, eps);
	}

	if (std::isnan(dIndex) || std::isinf(dIndex))
	{
		dIndex = (std::numeric_limits<double>::max)();
	}
	UINT enCurLevel = GetPerformanceLevel(dIndex, rPhneD.dIO, rPhneD.dLS, rPhneD.dCP);

	if ( bFiber )
	{
		CalcAcceptablePlasticHingeRotationColmFiber(da, db, rDir.dThetay, rDir.Moment.dIO, rDir.Moment.dLS, rDir.Moment.dCP);
		rPhneD.dIO = rDir.Moment.dIO;
		rPhneD.dLS = rDir.Moment.dLS;
		rPhneD.dCP = rDir.Moment.dCP;

		enCurLevel = GetPerformanceLevel(rDir.dThetap, rPhneD.dIO, rPhneD.dLS, rPhneD.dCP);
	}

	rDir.Moment.enLevel = enCurLevel;
	
	rPhneD.dDuctility = dIndex;
	rPhneD.enLevel_Deform = enCurLevel;
	rPhneD.enLevel = enCurLevel;
}

double CSeisEvalAIK_G001_2021::CalcAcceptableCriteriaCP(double dLS, double dCP)
{
	T_SEME_D SemeD;
	if ( !m_pDoc->m_pAttrCtrl2->GetSeme(SemeD) )
	{
		return dCP;
	}

	switch ( SemeD.data.design.nCPLevelOption )
	{
	case D_SEME_USERINPUT:
		{
			const double dUserCP = SafeDiv(dCP, SemeD.data.design.dCPLevelLimit);
			return max(dLS, dUserCP);
		}	
	case D_SEME_INTERMEDIATE:		
		{
			return 0.5*(dLS + dCP);
		}
	default:
		{
			ASSERT(0);
			return dCP;
		}
	}
}

void CSeisEvalAIK_G001_2021::CalcAcceptablePlasticHingeRotationBeam(UINT unCtrlType, bool bsGTd2, double da, double db, double dThetay, double dIOmin, OUT double& rdIO, OUT double& rdLS, OUT double& rdCP)
{
	switch ( unCtrlType )
	{
	case EN_EVALCTRL_FLEX:
		{
			/// [GEN-6326] theta,p / theta.y -> theta,max / theta.y로 수정되어서 계산한 값에 1.0을 더합니다.
			rdIO = 1.0 + SafeDiv(max(da/3.0, dIOmin), dThetay);
			rdLS = 1.0 + SafeDiv(da, dThetay);
			rdCP = 1.0 + SafeDiv(db, dThetay);
		}
		break;
	case EN_EVALCTRL_SHER:
		{
			/// [GEN-6326] theta,p / theta.y -> theta,max / theta.y로 수정되어서 계산한 값에 1.0을 더합니다.		
			rdIO = 1.0 + SafeDiv(0.0015, dThetay);
			rdLS = 1.0 + SafeDiv(bsGTd2 ? 0.005 : 0.01, dThetay);
			rdCP = 1.0 + SafeDiv(bsGTd2 ? 0.010 : 0.02, dThetay);
		}
		break;
	default:
		ASSERT(0);
		break;
	}	

	rdCP = CalcAcceptableCriteriaCP(rdLS, rdCP);
}

void CSeisEvalAIK_G001_2021::CalcAcceptablePlasticHingeRotationColm(double da, double db, double dThetay, OUT double& rdIO, OUT double& rdLS, OUT double& rdCP)
{
	/// [GEN-6326] theta,p / theta.y -> theta,max / theta.y로 수정되어서 계산한 값에 1.0을 더합니다.
    rdIO = 1.0 + 0.5;
    rdLS = 1.0 + SafeDiv(da, dThetay);
    rdCP = 1.0 + 0.5 * SafeDiv(da + db, dThetay);

	rdCP = CalcAcceptableCriteriaCP(rdLS, rdCP);
}

void CSeisEvalAIK_G001_2021::CalcAcceptablePlasticHingeRotationColmFiber(double da, double db, double dThetay, OUT double& rdIO, OUT double& rdLS, OUT double& rdCP)
{
	rdIO = 0.5*dThetay;
	rdLS = da;
	rdCP = 0.5*(da+db);

	rdCP = CalcAcceptableCriteriaCP(rdLS, rdCP);
}

void CSeisEvalAIK_G001_2021::CalcAcceptablePlasticHingeRotationWall(UINT unSeisDetail, double dAxialRatio, double dShearRatio, OUT double& dIO, OUT double& dLS, OUT double& dCP)
{
    dIO = 0.0;
    dLS = 0.0;
    dCP = 0.0;

    switch ( unSeisDetail )
    {
    case EN_SEWA_BNDR_SEIS:
        {
            if ( LE(dAxialRatio, 0.1) )
            {
                if ( LE(dShearRatio, 0.33) )
                {
                    dIO = 0.005;
                    dLS = 0.015;
                    dCP = 0.02;
                }
                else if ( ME(dShearRatio, 0.5) )
                {
                    dIO = 0.004;
                    dLS = 0.01;
                    dCP = 0.015;
                }
                else
                {
                    dIO = dgn::math::CalcLinearFunction(0.33, 0.005, 0.5, 0.004, dShearRatio);
                    dLS = dgn::math::CalcLinearFunction(0.33, 0.015, 0.5, 0.01,  dShearRatio);
                    dCP = dgn::math::CalcLinearFunction(0.33, 0.02,  0.5, 0.015, dShearRatio);
                }
            }
            else if ( ME(dAxialRatio, 0.25) )
            {
                if ( LE(dShearRatio, 0.33) )
                {
                    dIO = 0.003;
                    dLS = 0.009;
                    dCP = 0.012;
                }
                else if ( ME(dShearRatio, 0.5) )
                {
                    dIO = 0.0015;
                    dLS = 0.005;
                    dCP = 0.01;
                }
                else
                {
                    dIO = dgn::math::CalcLinearFunction(0.33, 0.003, 0.5, 0.0015, dShearRatio);
                    dLS = dgn::math::CalcLinearFunction(0.33, 0.009, 0.5, 0.005,  dShearRatio);
                    dCP = dgn::math::CalcLinearFunction(0.33, 0.012, 0.5, 0.01,   dShearRatio);
                }
            }
            else
            {
                if ( LE(dShearRatio, 0.33) )
                {
                    dIO = dgn::math::CalcLinearFunction(0.1, 0.005, 0.25, 0.003, dAxialRatio);
                    dLS = dgn::math::CalcLinearFunction(0.1, 0.015, 0.25, 0.009, dAxialRatio);
                    dCP = dgn::math::CalcLinearFunction(0.1, 0.02,  0.25, 0.012, dAxialRatio);
                }
                else if ( ME(dShearRatio, 0.5) )
                {
                    dIO = dgn::math::CalcLinearFunction(0.1, 0.004, 0.25, 0.0015, dAxialRatio);
                    dLS = dgn::math::CalcLinearFunction(0.1, 0.01,  0.25, 0.005,  dAxialRatio);
                    dCP = dgn::math::CalcLinearFunction(0.1, 0.015, 0.25, 0.01,   dAxialRatio);
                }
                else
                {
                    const auto dIO1 = dgn::math::CalcLinearFunction(0.33, 0.005, 0.5, 0.004, dShearRatio);
                    const auto dLS1 = dgn::math::CalcLinearFunction(0.33, 0.015, 0.5, 0.01,  dShearRatio);
                    const auto dCP1 = dgn::math::CalcLinearFunction(0.33, 0.02,  0.5, 0.015, dShearRatio);

                    const auto dIO2 = dgn::math::CalcLinearFunction(0.33, 0.003, 0.5, 0.0015, dShearRatio);
                    const auto dLS2 = dgn::math::CalcLinearFunction(0.33, 0.009, 0.5, 0.005,  dShearRatio);
                    const auto dCP2 = dgn::math::CalcLinearFunction(0.33, 0.012, 0.5, 0.01,   dShearRatio);

                    dIO = dgn::math::CalcLinearFunction(0.1, dIO1, 0.25, dIO2, dAxialRatio);
                    dLS = dgn::math::CalcLinearFunction(0.1, dLS1, 0.25, dLS2, dAxialRatio);
                    dCP = dgn::math::CalcLinearFunction(0.1, dCP1, 0.25, dCP2, dAxialRatio);
                }
            }
        }
        break;
    case EN_SEWA_BNDR_MITI:
        {
            if ( LE(dAxialRatio, 0.1) )
            {
                if ( LE(dShearRatio, 0.33) )
                {
                    dIO = 0.004;
                    dLS = 0.012;
                    dCP = 0.016;
                }
                else if ( ME(dShearRatio, 0.5) )
                {
                    dIO = 0.0032;
                    dLS = 0.008;
                    dCP = 0.012;
                }
                else
                {
                    dIO = dgn::math::CalcLinearFunction(0.33, 0.004, 0.5, 0.0032, dShearRatio);
                    dLS = dgn::math::CalcLinearFunction(0.33, 0.012, 0.5, 0.008,  dShearRatio);
                    dCP = dgn::math::CalcLinearFunction(0.33, 0.016, 0.5, 0.012,  dShearRatio);
                }
            }
            else if ( ME(dAxialRatio, 0.25) )
            {
                if ( LE(dShearRatio, 0.33) )
                {
                    dIO = 0.0024;
                    dLS = 0.0072;
                    dCP = 0.0096;
                }
                else if ( ME(dShearRatio, 0.5) )
                {
                    dIO = 0.0012;
                    dLS = 0.004;
                    dCP = 0.008;
                }
                else
                {
                    dIO = dgn::math::CalcLinearFunction(0.33, 0.0024, 0.5, 0.0012, dShearRatio);
                    dLS = dgn::math::CalcLinearFunction(0.33, 0.0072, 0.5, 0.004,  dShearRatio);
                    dCP = dgn::math::CalcLinearFunction(0.33, 0.0096, 0.5, 0.008,  dShearRatio);
                }
            }
            else
            {
                if ( LE(dShearRatio, 0.33) )
                {
                    dIO = dgn::math::CalcLinearFunction(0.1, 0.004, 0.25, 0.0024, dAxialRatio);
                    dLS = dgn::math::CalcLinearFunction(0.1, 0.012, 0.25, 0.0072, dAxialRatio);
                    dCP = dgn::math::CalcLinearFunction(0.1, 0.016, 0.25, 0.0096, dAxialRatio);
                }
                else if ( ME(dShearRatio, 0.5) )
                {
                    dIO = dgn::math::CalcLinearFunction(0.1, 0.0032, 0.25, 0.0012, dAxialRatio);
                    dLS = dgn::math::CalcLinearFunction(0.1, 0.008,  0.25, 0.004,  dAxialRatio);
                    dCP = dgn::math::CalcLinearFunction(0.1, 0.012,  0.25, 0.008,  dAxialRatio);
                }
                else
                {
                    const auto dIO1 = dgn::math::CalcLinearFunction(0.33, 0.004, 0.5, 0.0032, dShearRatio);
                    const auto dLS1 = dgn::math::CalcLinearFunction(0.33, 0.012, 0.5, 0.008,  dShearRatio);
                    const auto dCP1 = dgn::math::CalcLinearFunction(0.33, 0.016, 0.5, 0.012,  dShearRatio);

                    const auto dIO2 = dgn::math::CalcLinearFunction(0.33, 0.0024, 0.5, 0.0012, dShearRatio);
                    const auto dLS2 = dgn::math::CalcLinearFunction(0.33, 0.0072, 0.5, 0.004,  dShearRatio);
                    const auto dCP2 = dgn::math::CalcLinearFunction(0.33, 0.0096, 0.5, 0.008,  dShearRatio);

                    dIO = dgn::math::CalcLinearFunction(0.1, dIO1, 0.25, dIO2, dAxialRatio);
                    dLS = dgn::math::CalcLinearFunction(0.1, dLS1, 0.25, dLS2, dAxialRatio);
                    dCP = dgn::math::CalcLinearFunction(0.1, dCP1, 0.25, dCP2, dAxialRatio);
                }
            }
        }
        break;
    case EN_SEWA_BNDR_NONE:
        {
            if ( LE(dAxialRatio, 0.1) )
            {
                if ( LE(dShearRatio, 0.33) )
                {
                    dIO = 0.002;
                    dLS = 0.008;
                    dCP = 0.015;
                }
                else if ( ME(dShearRatio, 0.5) )
                {
                    dIO = 0.002;
                    dLS = 0.006;
                    dCP = 0.01;
                }
                else
                {
                    dIO = 0.002;
                    dLS = dgn::math::CalcLinearFunction(0.33, 0.008, 0.5, 0.006, dShearRatio);
                    dCP = dgn::math::CalcLinearFunction(0.33, 0.015, 0.5, 0.01,  dShearRatio);
                }
            }
            else if ( ME(dAxialRatio, 0.25) )
            {
                if ( LE(dShearRatio, 0.33) )
                {
                    dIO = 0.001;
                    dLS = 0.003;
                    dCP = 0.005;
                }
                else if ( ME(dShearRatio, 0.5) )
                {
                    dIO = 0.001;
                    dLS = 0.002;
                    dCP = 0.004;
                }
                else
                {
                    dIO = 0.001;
                    dLS = dgn::math::CalcLinearFunction(0.33, 0.003, 0.5, 0.002, dShearRatio);
                    dCP = dgn::math::CalcLinearFunction(0.33, 0.005, 0.5, 0.004, dShearRatio);
                }
            }
            else
            {
                if ( LE(dShearRatio, 0.33) )
                {
                    dIO = dgn::math::CalcLinearFunction(0.1, 0.002, 0.25, 0.001, dAxialRatio);
                    dLS = dgn::math::CalcLinearFunction(0.1, 0.008, 0.25, 0.003, dAxialRatio);
                    dCP = dgn::math::CalcLinearFunction(0.1, 0.015, 0.25, 0.005, dAxialRatio);
                }
                else if ( ME(dShearRatio, 0.5) )
                {
                    dIO = dgn::math::CalcLinearFunction(0.1, 0.002, 0.25, 0.001, dAxialRatio);
                    dLS = dgn::math::CalcLinearFunction(0.1, 0.006, 0.25, 0.002, dAxialRatio);
                    dCP = dgn::math::CalcLinearFunction(0.1, 0.01,  0.25, 0.004, dAxialRatio);
                }
                else
                {
                    const auto dIO1 = 0.002;
                    const auto dLS1 = dgn::math::CalcLinearFunction(0.33, 0.008, 0.5, 0.006, dShearRatio);
                    const auto dCP1 = dgn::math::CalcLinearFunction(0.33, 0.015, 0.5, 0.01,  dShearRatio);

                    const auto dIO2 = 0.001;
                    const auto dLS2 = dgn::math::CalcLinearFunction(0.33, 0.003, 0.5, 0.002, dShearRatio);
                    const auto dCP2 = dgn::math::CalcLinearFunction(0.33, 0.005, 0.5, 0.004, dShearRatio);

                    dIO = dgn::math::CalcLinearFunction(0.1, dIO1, 0.25, dIO2, dAxialRatio);
                    dLS = dgn::math::CalcLinearFunction(0.1, dLS1, 0.25, dLS2, dAxialRatio);
                    dCP = dgn::math::CalcLinearFunction(0.1, dCP1, 0.25, dCP2, dAxialRatio);
                }
            }
        }
        break;
    default: ASSERT(0); return;
    }

	dCP = CalcAcceptableCriteriaCP(dLS, dCP);
}

UINT CSeisEvalAIK_G001_2021::GetPerformanceLevel(double dIndex, double dIO, double dLS, double dCP)
{
	if ( dIndex < dIO ) return (UINT)EN_LEVEL_IO;
	if ( dIndex < dLS ) return (UINT)EN_LEVEL_LS;
	if ( dIndex < dCP ) return (UINT)EN_LEVEL_CP;
	return (UINT)EN_LEVEL_CL;
}

UINT CSeisEvalAIK_G001_2021::GetPerformanceLevel(double dIndex)
{
	if (dgn::LT(dIndex, 1.0)) return (UINT)EN_LEVEL_IO;
	return (UINT)EN_LEVEL_CL;
}

void CSeisEvalAIK_G001_2021::SetPhgtEnergyDissipationOption(double dFactor, OUT T_PHGT_FEMA_ENRG_DISSIPATION& rPhgtEnrg)
{
	rPhgtEnrg.bCyclicDegration = TRUE;
	rPhgtEnrg.dAB_p = dFactor;
	rPhgtEnrg.dC_p  = dFactor;
	rPhgtEnrg.dD_p  = dFactor;
	rPhgtEnrg.dE_p  = dFactor;
	rPhgtEnrg.dAB_m = dFactor;
	rPhgtEnrg.dC_m  = dFactor;
	rPhgtEnrg.dD_m  = dFactor;
	rPhgtEnrg.dE_m  = dFactor;
	rPhgtEnrg.bUnloadingStiffFactor = TRUE;
	rPhgtEnrg.dUnloadingStiffFactor = 1.0;	
}

void CSeisEvalAIK_G001_2021::SetCurveFromTableConBeam(double daN, double dbN, double dThetayN,
	double daP, double dbP, double dThetayP, double dCBRatio, double dc, OUT double radCurv[2][9])
{
	radCurv[IDX_FFY][IDX_CURV_ME] = -1.0*dc;
	radCurv[IDX_FFY][IDX_CURV_MD] = -1.0*dc;
	radCurv[IDX_FFY][IDX_CURV_MC] = -1.0*dCBRatio;
	radCurv[IDX_FFY][IDX_CURV_MB] = -1.0;
	radCurv[IDX_FFY][IDX_CURV_A] =  0.0;
	radCurv[IDX_FFY][IDX_CURV_B] =  1.0;
	radCurv[IDX_FFY][IDX_CURV_C] =  dCBRatio;
	radCurv[IDX_FFY][IDX_CURV_D] =  dc;
	radCurv[IDX_FFY][IDX_CURV_E] =  dc;

	auto L_GetParam = [](double dThetay, double dVal)
	{
		return SafeDivAbs(dThetay + dVal, dThetay);
	};

	const double dDN = L_GetParam(dThetayN, dbN);
	const double dDP = L_GetParam(dThetayP, dbP);
		
	radCurv[IDX_DDY][IDX_CURV_ME] = -1.2*dDN;
	radCurv[IDX_DDY][IDX_CURV_MD] = -1.0*dDN;
	radCurv[IDX_DDY][IDX_CURV_MC] = -1.0*L_GetParam(dThetayN, daN);
	radCurv[IDX_DDY][IDX_CURV_MB] = -1.0;
	radCurv[IDX_DDY][IDX_CURV_A] =  0.0;
	radCurv[IDX_DDY][IDX_CURV_B] =  1.0;
	radCurv[IDX_DDY][IDX_CURV_C] = L_GetParam(dThetayP, daP);
	radCurv[IDX_DDY][IDX_CURV_D] = dDP;
	radCurv[IDX_DDY][IDX_CURV_E] = 1.2*dDP;
}

void CSeisEvalAIK_G001_2021::SetCurveFromTableConColm(double da, double db, double dThetay, 
	double dCBRatio, double dc, OUT double radCurv[2][9])
{
	radCurv[IDX_FFY][IDX_CURV_ME] = -1.0*dc;
	radCurv[IDX_FFY][IDX_CURV_MD] = -1.0*dc;
	radCurv[IDX_FFY][IDX_CURV_MC] = -1.0*dCBRatio;
	radCurv[IDX_FFY][IDX_CURV_MB] = -1.0;
	radCurv[IDX_FFY][IDX_CURV_A] =  0.0;
	radCurv[IDX_FFY][IDX_CURV_B] =  1.0;
	radCurv[IDX_FFY][IDX_CURV_C] =  dCBRatio;
	radCurv[IDX_FFY][IDX_CURV_D] =  dc;
	radCurv[IDX_FFY][IDX_CURV_E] =  dc;

	const double dTol = 1.0e-25;
	const double dC = SafeDivAbs(dThetay + da, dThetay, dTol);
	const double dD = SafeDivAbs(dThetay + db, dThetay, dTol);
	
	radCurv[IDX_DDY][IDX_CURV_ME] = -1.2*dD;
	radCurv[IDX_DDY][IDX_CURV_MD] = -1.0*dD;
	radCurv[IDX_DDY][IDX_CURV_MC] = -1.0*dC;
	radCurv[IDX_DDY][IDX_CURV_MB] = -1.0;
	radCurv[IDX_DDY][IDX_CURV_A] =  0.0;
	radCurv[IDX_DDY][IDX_CURV_B] =  1.0;
	radCurv[IDX_DDY][IDX_CURV_C] = dC;
	radCurv[IDX_DDY][IDX_CURV_D] = dD;
	radCurv[IDX_DDY][IDX_CURV_E] = 1.2*dD;
}

double CSeisEvalAIK_G001_2021::CalcEnergyDissipationFactorConBeam(const T_PDHG_CONBEAM_INFO& Info)
{
    /// [Table 6-15]
    const double dfy = Info.dfy;
    const double dAs2 = min(Info.FlexTop.dAs, Info.FlexBot.dAs);
	const double dhs = Info.FlexTop.dd + Info.FlexBot.dd - Info.dH;
    const double dMP = Info.FlexBot.dMy;
    const double dMN = Info.FlexTop.dMy;
    const double dls = Info.dShearSpan ;
    const double dh  = Info.dH;
    const double dLambda = min(1.0, SafeDiv(dls, 5.0 * dh));

    const double dNumerator = 3.0 * dfy * dAs2 * dhs * dLambda;
    const double dDenominator = 2.0 * (dMP + dMN);
    const double dk = max(0.15, SafeDiv(dNumerator, dDenominator));

	return min(dk, 1.0);
}

double CSeisEvalAIK_G001_2021::CalcEnergyDissipationFactorConColm(const T_PDHG_CONCOLM_INFO& Info, const T_PDHG_CONCOLM_DIR& Dir)
{
	/// [Table 6-15]
    const bool bCircular = Info.bCircle;
    const double dfy = Info.dfy;
    const double dRho = SafeDiv(Info.dAst, Info.dAg);
    const double dMP = Dir.dMn0;
    const double dMN = Dir.dMn0;
	const double dls = Dir.dShearSpan;
    const double dh  = Dir.dH;
    const double dLambda = min(1.0, SafeDiv(dls, 3.0 * dh));

    auto dk = 0.0;
    if ( bCircular )
    {
        const double dDs = Dir.dH - Info.ddo;
        const double dD = Dir.dH;
        const double dNumerator = 3.0 * dfy * dRho * dDs * dD * dD;
        const double dDenominator = 8.0 * (dMP + dMN);
        dk = max(0.15, SafeDiv(dNumerator, dDenominator));
    }
    else
    {
        const double db = Dir.dB;
        const double dNumerator = 3.0 * dfy * dRho * db * dh * dh * dLambda;
        const double dDenominator = 8.0 * (dMP + dMN);
        dk = max(0.15, SafeDiv(dNumerator, dDenominator));
    }

	return min(dk, 1.0);
}

bool CSeisEvalAIK_G001_2021::IsIgnoreThetayFiberWall()
{
	T_SEME_D SemeD;
	if ( !m_pDoc->m_pAttrCtrl2->GetSeme(SemeD) )
	{
		SemeD.Initialize(D_SEME_METHOD_AIK_G_001_2021);
	}
	return SemeD.data.design.bIgnoreYeildRotaFWall;
}

bool CSeisEvalAIK_G001_2021::IsSeismicDetail_1(const double dSpace, const double dd) const
{
	return ( dSpace > dd / 3.0 ) ? false : true;
}

bool CSeisEvalAIK_G001_2021::IsSeismicDetail_2(const double dVs, const double dVu) const
{
	return ( dVs < fabs(dVu) * 0.75 ) ? false : true;
}

T_THIS_K CSeisEvalAIK_G001_2021::GetInitialLoadThisK(T_THIS_K ThisK)
{
	T_THGC_D ThgcD;
	m_pDoc->m_pAttrCtrl2->GetThgc(ThgcD);
	return m_pDoc->m_pAttrCtrl2->HasThgcDummyInitLoad() ? ThgcD.ThisK_DummyInitLoad : ThisK;
}

double CSeisEvalAIK_G001_2021::GetRatioMuMy(double dM, const double adForceMomentRatio[2][9])
{
	const int nIdx = dM < 0.0 ? IDX_CURV_MC : IDX_CURV_C;
	return adForceMomentRatio[IDX_FFY][nIdx];
}

bool CSeisEvalAIK_G001_2021::GetRatioMuMyPhgt(T_ELEM_K ElemK, const double adM[2], OUT double radRatioMuMy[2])
{
	memset(radRatioMuMy, 1.0, sizeof(double)*2);

	T_PHGT_D PhgtD;
	if ( !m_pDoc->m_pAttrCtrl->GetPhgtByElemK(ElemK, PhgtD) ) { return false; }

	const int nDOF = DOF_MY;
	if ( PhgtD.nSkcvType[nDOF] != D_SKCV_FEMA ) { return false; }
	
	radRatioMuMy[0] = GetRatioMuMy(adM[0], PhgtD.Skcv[nDOF].FEMA.dForceMomentRatio);
	radRatioMuMy[1] = PhgtD.Skcv[nDOF].bSymmetricIJ ? radRatioMuMy[0] : GetRatioMuMy(adM[1], PhgtD.SkcvSub[nDOF].FEMA.dForceMomentRatio);

	return true;
}

bool CSeisEvalAIK_G001_2021::GetRatioMuMyIehp(T_ELEM_K ElemK, const double adM[2], OUT double radRatioMuMy[2])
{
	memset(radRatioMuMy, 1.0, sizeof(double)*2);

	T_IEHG_D IehgD;
	if ( !m_pDoc->m_pAttrCtrl->GetIehgByElemK(ElemK, IehgD) ) { return false; }
	
	T_IEHP_D IehpD;
	if ( !m_pDoc->m_pAttrCtrl->GetIehp(IehgD.IehpKey, IehpD) ) { return false; }

	const int nDOF = DOF_MY;
	radRatioMuMy[0] = GetRatioMuMy(adM[0], IehpD.AllProp.PROP[nDOF].FEMA.dForceMomentRatio);
	radRatioMuMy[1] = !IehpD.bExistIJProp ? radRatioMuMy[0] : GetRatioMuMy(adM[1], IehpD.AllSubProp.PROP[nDOF].FEMA.dForceMomentRatio);

	return true;
}

BOOL CSeisEvalAIK_G001_2021::GetSeisEval2ndRsltSubConColmPOFiberShear(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nDof, OUT T_EVAL2ND_PHNE& rPhneDI, OUT T_EVAL2ND_PHNE& rPhneDJ)
{
	rPhneDI.Initialize();
	rPhneDJ.Initialize();

	T_POGD_D PogdD;
	if (m_pDoc->m_pAttrCtrl->GetPogd(PogdD) && PogdD.nInitLoadMethod == 0)
	{	
		double adInitForc[2][6] = { { 0.0 }, };
		GetPORsltForcByInitialLoad(ElemK, adInitForc);

		double adPolcForc[2][6] = { { 0.0 }, };
		GetPORsltForc(ElemK, PolcK, nStep, nAddStepIdx, adPolcForc);

		/// concrete 부재는 축력 부호 처리 필요. compression = (+), tension = (-).
		adPolcForc[0][0] *= -1.0;
		adPolcForc[1][0] *= -1.0;

		const EN_DIR& enDir = nDof == D_DOF_FY ? EN_DIR_Y : EN_DIR_Z;
		T_COLM_SHR_PARAM aShr[EN_MEMBEND_NUM];
		if (!MakeColmShearParamPdgn(ElemK, EN_NLANALTYPE::TH, enDir, EN_EVALMATL_NOM, aShr)) { ASSERT(0); return FALSE; }

		const int nIndexDOF = nDof == D_DOF_FY ? 1 : 2;
		const double dQUFI = adPolcForc[0][nIndexDOF];
		const double dQUFJ = adPolcForc[1][nIndexDOF];
		const double dQGI = adInitForc[0][nIndexDOF];
		const double dQGJ = adInitForc[1][nIndexDOF];
		const double dQCNI = CalcVnConColm(aShr[0], adPolcForc[0][0]);
		const double dQCNJ = CalcVnConColm(aShr[1], adPolcForc[1][0]);

		CSeisEvalDataTool Tool;
		Tool.CalcEval2ndForcCtrlAction(ElemK, dQUFI, dQGI, dQCNI, rPhneDI);
		rPhneDI.enLevel = rPhneDI.enLevel_Force;
		Tool.CalcEval2ndForcCtrlAction(ElemK, dQUFJ, dQGJ, dQCNJ, rPhneDJ);
		rPhneDJ.enLevel = rPhneDJ.enLevel_Force;
	}

	return TRUE;
}

BOOL CSeisEvalAIK_G001_2021::GetSeisEval2ndRsltSubConWallPOFiberShear(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nDof, OUT T_EVAL2ND_WALL_POS& rPosI, OUT T_EVAL2ND_WALL_POS& rPosJ)
{
	T_THIK_D ThikD;
	T_MATD_D MatdD;
	T_PDCM_D PdcmD;
	if (!GetElemMatlThik(ElemK, ThikD, MatdD, PdcmD)) { ASSERT(0); return FALSE; }

	T_WALL_KEY WallKey;
	T_WALL_D WallD;  WallD.Initialize();
	if (!m_pDoc->m_pPostCtrl->GetWallDataFromElemK(ElemK, WallD, &WallKey)) { return FALSE; }
	const double dAg = WallD.dThickness * WallD.dLength;

	CSeisEvalDataTool Tool;
	const double dfck = Tool.GetFc(EN_EVALMATL_AVG, PdcmD);
	double adPolcForc[2][6] = { { 0.0 }, };
	GetPORsltForc(ElemK, PolcK, nStep, nAddStepIdx, adPolcForc);

	const double dPuI = adPolcForc[0][0] * (-1.0);
	const double dPuJ = adPolcForc[1][0] * (-1.0);

	rPosI.dAxialIndex = dPuI < 0.0 ? 0.0 : SafeDiv(dPuI, dAg * dfck);
	rPosJ.dAxialIndex = dPuJ < 0.0 ? 0.0 : SafeDiv(dPuJ, dAg * dfck);

	T_EVAL2ND_PHNE& rPhneDI = rPosI.PhneD[nDof];
	T_EVAL2ND_PHNE& rPhneDJ = rPosJ.PhneD[nDof];

	rPhneDI.Initialize();
	rPhneDJ.Initialize();

	T_POGD_D PogdD;
	if (m_pDoc->m_pAttrCtrl->GetPogd(PogdD) && PogdD.nInitLoadMethod == 0)
	{
		double adInitForc[2][6] = { { 0.0 }, };
		GetPORsltForcByInitialLoad(ElemK, adInitForc);

		T_WALL_SHR_PARAM aShrAvg[EN_MEMBEND_NUM];
		T_WALL_SHR_PARAM aShrNom[EN_MEMBEND_NUM];
		if (!MakeWallShearParam(ElemK, EN_DIR_Z, aShrAvg)) { return FALSE; }
		if (!MakeWallShearParam(ElemK, EN_DIR_Z, aShrNom)) { return FALSE; }
		SetWallForcShearParam(adPolcForc, EN_DIR_Z, aShrAvg, aShrNom);

		const int nIndexDOF = nDof == D_DOF_FY ? 1 : 2;
		const double dQUFI = adPolcForc[0][nIndexDOF];
		const double dQUFJ = adPolcForc[1][nIndexDOF];
		const double dQGI = adInitForc[0][nIndexDOF];
		const double dQGJ = adInitForc[1][nIndexDOF];
		const double dQCNI = CalcVnConWall(aShrNom[0]);
		const double dQCNJ = CalcVnConWall(aShrNom[1]);

		Tool.CalcEval2ndForcCtrlAction(ElemK, dQUFI, dQGI, dQCNI, rPhneDI);
		rPhneDI.enLevel = rPhneDI.enLevel_Force;
		Tool.CalcEval2ndForcCtrlAction(ElemK, dQUFJ, dQGJ, dQCNJ, rPhneDJ);
		rPhneDJ.enLevel = rPhneDJ.enLevel_Force;
	}

	return TRUE;
}
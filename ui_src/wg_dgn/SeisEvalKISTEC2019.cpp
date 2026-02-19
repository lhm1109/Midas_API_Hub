#include "stdafx.h"
#include "SeisEvalKISTEC2019.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_MembCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\ColumnWallFinderByInfillStrut.h"
#include "..\wg_db\SeisEvalResult.h"
#include "..\wg_db\SeisEvalDataTool.h"

#include "..\..\dgnengine\idesign\DGN_lib\DGNCompare.h"

CSeisEvalKISTEC2019::CSeisEvalKISTEC2019(void) : CSeisEvalKISTEC2013()
{
}


CSeisEvalKISTEC2019::~CSeisEvalKISTEC2019(void)
{
}

BOOL CSeisEvalKISTEC2019::InitializeHingeInfill(T_ELEM_K ElemK, const BOOL abDof[6], OUT T_PHGT_D& rPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rPhgtD.Initialize();
    if (!abDof[DOF_FX]) { return TRUE; }

    T_MSIS_D MsisD;
    if ( !GetInfillParam(ElemK, MsisD) ) { ASSERT(0); return FALSE; }

    T_MSIS_INPUT MsisInp;
    T_MSIS_PROP  MsisPro;
    if ( !m_pDoc->m_pAttrCtrl2->CalcInfillStrutPropByElem(ElemK, MsisInp, MsisPro) ) { return FALSE; } 

    switch (MsisD.nStrutType)
    {
    case EN_MSIS_PARTIAL_HEIGHT:
        {
            SetPhgtProp(D_PHGT_ELEM_TRUSS, D_PHGT_MATL_CON, D_PHGT_DEFINITION_MTHETA_LUMP, INTER_NONE, rPhgtD);
            SetInitPhgtPropComponentInfill(DOF_FX, abDof[DOF_FX], D_PHGT_FEMA_IFST_WAIST, rPhgtD);
        }
        break;
    case EN_MSIS_FULL_HEIGHT:
        {
            SetPhgtProp(D_PHGT_ELEM_TRUSS, D_PHGT_MATL_CON, D_PHGT_DEFINITION_MTHETA_LUMP, INTER_NONE, rPhgtD);
            SetInitPhgtPropComponentInfill(DOF_FX, abDof[DOF_FX], D_PHGT_FEMA_IFST_INFILL, rPhgtD);
        }
        break;
    default: return FALSE; 
        break;
    }

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SEMM_D SemmD;
    if (!GetElemMatlSectMas(ElemK, SectD, MatdD, SemmD)) { ASSERT(FALSE); return FALSE; } 
    if (SemmD.bUseDefaultVal) { return FALSE; }

    CSeisEvalDataTool Tool;
    /// [JIRA, GEN-646] 2단계 평가시, 평균강도로 적용.
    double dFmC = Tool.GetFmcMas(EN_EVALSTEP_2ND, EN_EVALMATL_AVG, SemmD);
    double dVm  = Tool.GetFvmMas(EN_EVALSTEP_2ND, EN_EVALMATL_AVG, SemmD);
    double dPhi = 1.0; /// 설계가 아니므로, 고려하지 않음.

    T_MISA_D MisaD;
    if (!m_pDoc->m_pAttrCtrl2->GetMisa(ElemK, MisaD)) { ASSERT(FALSE); return FALSE; }

    BOOL bConsiderVbjs = IsConsiderVbjs(ElemK);
    T_EVALLIN_INFILL_CAPA Capa;
    CalcInfillStrn(dFmC, dVm, dPhi, EN_DIAGONAL, MsisD, bConsiderVbjs, Capa);  /// [MQC 26497] 힌지 항복강도 계산시에는 Bracing Type과 관계없이 제 강도 사용.

    rPhgtD.Skcv[DOF_FX].nCalcMethod = D_PHGT_CALC_USER;

    T_PHGT_FEMA& FemaFX = rPhgtD.Skcv[DOF_FX].FEMA;

    FemaFX.nInfillStiff     = 0;
    FemaFX.dYieldForce[0]   = Capa.dVn;
    FemaFX.dInfillStiff_KmiRop = MsisPro.dKmi * MsisPro.dRop;
    FemaFX.dInfillStiff_KmsRop = MsisPro.dKms * MsisPro.dRop;
    FemaFX.dInfillStiff_Hm  = MsisInp.dHm;
    FemaFX.dInfillStiff_ThetaM = MsisPro.dThetaM;

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcUpdateHingeInfill(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_MAP& mPORslt, OUT T_PHGT_D& rPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    if (abDof[DOF_FX])
    {
        rPhgtD.Skcv[DOF_FX].nCalcMethod  = D_PHGT_CALC_USER;
        rPhgtD.Skcv[DOF_FX].bSymmetricIJ = FALSE;
        rPhgtD.Skcv[DOF_FX].nInitStiffMethod = 0;
        rPhgtD.SkcvSub[DOF_FX] = rPhgtD.Skcv[DOF_FX];

        CalcFxHingePropInfill(ElemK, mPORslt, rPhgtD.Skcv[DOF_FX].FEMA, rPhgtD.SkcvSub[DOF_FX].FEMA);		
    }

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcMyHingePropConBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, double dRatioMuMy,
	OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
    double adV[2]  = { 0.0, };  // 대상 step에서의 전단력.
    double adM[2]  = { 0.0, };  // 대상 step에서의 모멘트.
    double adDP[2] = { 0.0, };  // 정모멘트에 대한 항복회전각.
    double adDN[2] = { 0.0, };  // 부모멘트에 대한 항복회전각.
    GetPORsltConBeamHingeMy( PORslt, adV, adM, adDP, adDN );

    T_UPHG_CONBEAM Uphg;
    Uphg.ElemK = ElemK;
    if ( !GetSEResult()->GetUphgConBeam(ElemK, Uphg) ) { Uphg.Initialize(); }

    T_CONBEAM_INFO aInfo[EN_MEMBEND_NUM];
    if ( !MakeBeamInfo(ElemK, EN_EVALSTEP_2ND, adV, aInfo) ) { return FALSE; }

    CSeisEvalDataTool Tool;
    int nSeisCompType = Tool.GetSeisComponentType(ElemK);

    for ( int i=0; i<EN_MEMBEND_NUM; ++i )
    {
        bool bEndI = ( i == EN_MEMBEND_I ) ? true : false;

        T_UPHG_CONBEAM_POS& CurUphg = bEndI ? Uphg.EndI : Uphg.EndJ;        
        CurUphg.Info  = aInfo[i];        
        CurUphg.PolcK = PORslt.PolcK;
        CurUphg.nStep = PORslt.nStep;
        CurUphg.dMu   = adM[i];
        CurUphg.dVu   = adV[i];
        CurUphg.ParamP.dThetay = adDP[i];
        CurUphg.ParamN.dThetay = adDN[i];

        T_PHGT_FEMA& CurPhgt = bEndI ? rPhgtI : rPhgtJ;
        CalcMyHingePropConBeamPos(CurUphg, CurPhgt);
    }

    GetSEResult()->SetUphgConBeam(ElemK, Uphg);

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcFxHingePropConColm(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    if ( !GetElemMatlSectCon(ElemK, SectD, MatdD, SecmD) ) { return FALSE; }

    T_REBC_D RebcD;
    if ( !m_pDoc->m_pAttrCtrl->GetRebcByElemK(ElemK, RebcD) ) { return FALSE; }
    
    CSeisEvalDataTool Tool;
    const double dFck = Tool.GetFc(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);
    const double dFym = Tool.GetFym(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);
    const double dAs  = GetRebarAs(RebcD.MainRebarName) * RebcD.nQrb;
    const double dAg  = SectD.SectBefore.SectI.Stiffness.Area;
    const double dFactorPnmax = GetFactorPnmaxColm(EN_EVALSTEP_2ND, RebcD.nHoopType);
    const bool bInfill = IsConColmConnectedInfill(ElemK);
    
    CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();
    const double dL = pMembCtrl->GetMembLengByIncludeElem(ElemK);

    T_UPHG_CONCOLM Uphg;
    Uphg.ElemK = ElemK;

    if ( !GetSEResult()->GetUphgConColm(ElemK, Uphg) ) { Uphg.Initialize(); }

    for ( int i=0; i<EN_MEMBEND_NUM; ++i )
    {
        const bool bEndI = ( i == EN_MEMBEND_I ) ? true : false;
        T_PHGT_FEMA& rCur = bEndI ? rPhgtI : rPhgtJ;
        T_UPHG_CONCOLM_POS& rUphgPos = bEndI ? Uphg.EndI : Uphg.EndJ;

        rCur.bSymmetric = FALSE;
        rCur.bUserDefineRatio = TRUE;		        
        rCur.dYieldForce[IDX_PLS] = CalcPnTensCon(dFym, dAs);
        rCur.dYieldForce[IDX_MNS] = CalcPnCompCon(dFck, dAg, dFym, dAs, dFactorPnmax);

        if ( bInfill )
        {
            rCur.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;

            const double dThetayComp = fabs(PORslt.dD1Neg[i][DOF_FX])/dL;
            double adTableComp[6] = { 0.0, };
            GetParamTable_ConColmInfilled_Comp(adTableComp);
            double adParamComp[6] = { 0.0, };
            CalcHingeParamConFromDE(dThetayComp, adTableComp, adParamComp);

            rUphgPos.ParamComp.dThetay = dThetayComp;
            memcpy(rUphgPos.ParamComp.adTable, adTableComp, sizeof(double)*6);
            memcpy(rUphgPos.ParamComp.adParam, adParamComp, sizeof(double)*6);

            const double dThetayTens = fabs(PORslt.dD1Pos[i][DOF_FX])/dL;
            double adTableTens[6] = { 0.0, };
            GetParamTable_ConColmInfilled_Tens(adTableTens);
            double adParamTens[6] = { 0.0, };
            CalcHingeParamConFromDE(dThetayTens, adTableTens, adParamTens);

            rUphgPos.ParamTens.dThetay = dThetayTens;
            memcpy(rUphgPos.ParamTens.adTable, adTableTens, sizeof(double)*6);
            memcpy(rUphgPos.ParamTens.adParam, adParamTens, sizeof(double)*6);

            SetCurveDispCtrl(adParamTens, adParamComp, rCur.dForceMomentRatio);
            SetAccetanceCriteria(adParamTens, adParamComp, rCur);            
        }
        else
        {
            rCur.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;

            SetCurveForcCtrlAxial(rCur.dForceMomentRatio);
            SetAccetanceCriteriaConAxial(rCur);
        }       
    }

    GetSEResult()->SetUphgConColm(ElemK, Uphg);

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcFyzHingePropConColm(T_ELEM_K ElemK, EN_DIR enDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType,
                                                  OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
    double adP[2]  = { 0.0, };
    double adV[2]  = { 0.0, };
    double adM[2]  = { 0.0, };
    double admu[2] = { 0.0, };
    GetPORsltConColmHingeFyz(enDir, PORslt, adP, adV, adM, admu);

    T_COLM_SHR_PARAM aShr[EN_MEMBEND_NUM];
    if (!MakeColmShearParamEval(ElemK, EN_EVALSTEP_2ND, enDir, aShr)) { ASSERT(0); return FALSE; }

    for (int i=0; i<EN_MEMBEND_NUM; ++i)
    {
        T_PHGT_FEMA& rCurPhgt = i==EN_MEMBEND_I ? rPhgtI : rPhgtJ;
        const T_COLM_SHR_PARAM& CurShr = aShr[i];

        rCurPhgt.bSymmetric = FALSE;
        rCurPhgt.bUserDefineRatio = TRUE;	
        rCurPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
        SetCurveForcCtrlShear(rCurPhgt.dForceMomentRatio);
        /// [JIRA,GEN-714] 하중지배 거동 성분의 Acceptance Criteria 변경 (0.5, 0.75, 1.0 -> 1.0, 1.0, 1.0)
        SetAccetanceCriteria(1.0, 1.0, 1.0, rCurPhgt);

        double dVn = CalcVnConColm(CurShr, adP[i], adV[i], adM[i]);
        dVn = GetUpdateHingeVnConColm(dVn);
        
        rCurPhgt.dYieldForce[IDX_PLS] = dVn;
        rCurPhgt.dYieldForce[IDX_MNS] = dVn;
    }

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcMyzHingePropConColm(T_ELEM_K ElemK, EN_DIR enDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType,
                                                  OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
    double adP[2]  = { 0.0, };
    double adV[2]  = { 0.0, };
    double adM[2]  = { 0.0, };
    double adDy[2] = { 0.0, };
    double adM1y[2] = { 0.0, };
    double adM1z[2] = { 0.0, };
    GetPORsltConColmHingeMyz(enDir, PORslt, adP, adV, adM, adDy, adM1y, adM1z);

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    if (!GetElemMatlSectCon(ElemK, SectD, MatdD, SecmD)) { ASSERT(0); return FALSE; }
           
    const EN_DIR enDirV = GetPairDirV(enDir);

    T_COLM_SHR_PARAM aShr[EN_MEMBEND_NUM];
    MakeColmShearParamEval(ElemK, EN_EVALSTEP_2ND, enDirV, aShr);

    CSeisEvalDataTool Tool;
    int nSeisCompType = Tool.GetSeisComponentType(ElemK);
    if (nSeisCompType == D_SCMT_TYPE_NONE) { return FALSE; }

    UINT unSeisDetail = Tool.GetSeisDetailType(ElemK);
            
    _DB_PMCV_DATA Pmcvy;
    _DB_PMCV_DATA Pmcvz;
    if (!GetPmcvData(ElemK, EN_EVALSTEP_2ND, EN_EVALMATL_AVG, Pmcvy, Pmcvz)) { return FALSE; }

    double dMeyI = abs(adM1y[0]);
    double dMezI = abs(adM1z[0]);
    double dMeyJ = abs(adM1y[1]);
    double dMezJ = abs(adM1z[1]);

	double adL[2] = { 0.0, };
	CalConColmLengthConsiderInfillStrutPosition(ElemK, adL);
	const double dL = enDir == EN_DIR_Y ? adL[1] : adL[0];

    const double dAg = SectD.SectBefore.SectI.Stiffness.Area;
    const double dVp  = fabs(dL) < cEvalZero ? 0.0 : (enDirV == EN_DIR_Y ? fabs(dMezI+dMezJ)/dL : fabs(dMeyI+dMeyJ)/dL); 
    const double dFck = Tool.GetFc(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);

    T_UPHG_CONCOLM Uphg;
    Uphg.ElemK = ElemK;
    
    if ( !GetSEResult()->GetUphgConColm(ElemK, Uphg) ) { Uphg.Initialize(); }
    
    for ( int i=0; i<EN_MEMBEND_NUM; ++i )
    {        
        const double dMey = abs(adM1y[i]);
        const double dMez = abs(adM1z[i]);

        const T_COLM_SHR_PARAM& CurShr = aShr[i];
        const double dP  = adP[i];
        const double dV  = adV[i];
        const double dM  = adM[i];
        const double dDy = adDy[i];
        const double dVn = CalcVnConColm(CurShr, dP, dV, dM); /// [JIRA, GEN-572] 파괴모드를 계산할 때도 K1 고려. Vo = Vn.
        
        BOOL bConsiderGroup1 = IsConditionGroup1(CurShr.Comm);
        T_CONCOLM_INFO Info;
        Info.dfck = dFck;
        Info.dAg  = dAg;        
        Info.dAxlIdx = CalcAxialIndex(dP, dFck, dAg);
        Info.dShrIdx = CalcShearIndex(dV, CurShr.Comm);
        Info.dRhoIdx = CurShr.Comm.CalcRhoIndex();
        Info.dS   = CurShr.Comm.dSv;
        Info.db   = CurShr.Comm.dBv;
        Info.dd   = CurShr.Comm.dDv;
        Info.dVp  = dVp;
        Info.dVn  = dVn;
        Info.dMei = enDir==EN_DIR_Y ? dMeyI : dMezI;
        Info.dMej = enDir==EN_DIR_Y ? dMeyJ : dMezJ;
        Info.dL   = dL;
        Info.bConsiderGroup1 = bConsiderGroup1;
        Info.unSeisDetail = unSeisDetail;                
        Info.unGroup = GetGroupConColm(unSeisDetail, dVp, dVn, bConsiderGroup1);
        Info.dPu  = dP;
        Info.dVu  = dV;

        const bool bEndI = ( i == EN_MEMBEND_I ) ? true : false;
        T_PHGT_FEMA& rCurPhgt = bEndI ? rPhgtI : rPhgtJ;
        T_UPHG_CONCOLM_POS& rUphgPos = bEndI ? Uphg.EndI : Uphg.EndJ;

        T_UPHG_CONCOLM_DIR& UphgDir = enDir==EN_DIR_Y ? rUphgPos.Diry : rUphgPos.Dirz;        
        UphgDir.PolcK = PORslt.PolcK;
        UphgDir.nStep = PORslt.nStep;
        UphgDir.dPu   = dP;
        UphgDir.dMu   = dM;
        UphgDir.dVu   = dV;
        UphgDir.Info  = Info;
        UphgDir.Param.dThetay = dDy;
        
        /// [JIRA,GEN-1174] Column(M)의 휨성분은 일반 Column과 동일하게.
        CalcMyzHingePropConColmPos(UphgDir, rCurPhgt);        
    }

    GetSEResult()->SetUphgConColm(ElemK, Uphg);
        
    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcFzHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
    T_WALL_KEY WallKey;
    T_WALL_D WallD;  WallD.Initialize();
    if (!m_pDoc->m_pPostCtrl->GetWallDataFromElemK(ElemK, WallD, &WallKey))  {  return FALSE; }
    if ( !WallD.bLineWall ) { return FALSE; }

    double dTw = WallD.dThickness;
    double dLw = WallD.dLength;
    double dHw = WallD.dHeight;

    T_THIK_D ThikD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    if (!GetElemMatlThik(ElemK, ThikD, MatdD, SecmD)) { ASSERT(FALSE); return FALSE; }

    CSeisEvalDataTool Tool;    
    const bool bBE = Tool.IsConsiderBoundaryElement(ElemK);

    T_WALL_SHR_PARAM aShrAvg[EN_MEMBEND_NUM];
    T_WALL_SHR_PARAM aShrNom[EN_MEMBEND_NUM];
    if ( !MakeWallShearParam(ElemK, EN_EVALSTEP_2ND, EN_DIR_Z, aShrAvg, aShrNom) ) { ASSERT(0); return FALSE; }
    SetWallForcShearParam(PORslt.dForce, EN_DIR_Z, aShrAvg, aShrNom);

    // [GEN-1589] 휨강도는 하중지배이더라도 평균강도에 의한 값을 사용.    
    _DB_PMCV_DATA Pmcvy;
    _DB_PMCV_DATA Pmcvz;
    if ( !GetPmcvData(ElemK, EN_EVALSTEP_2ND, EN_EVALMATL_AVG, Pmcvy, Pmcvz) ) { return FALSE; }

    T_UPHG_CONWALL Uphg;
    Uphg.ElemK = ElemK;
    if ( !GetSEResult()->GetUphgConWall(ElemK, Uphg) ) { Uphg.Initialize(); }

    const double dFck = Tool.GetFc(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);
    const double dAg  = dTw*dLw;
    const int nPMMethod = Tool.GetPMCalcMethodWall(ElemK, EN_EVALSTEP_2ND);

    for ( int i = 0; i < EN_MEMBEND_NUM; ++i )
    {
        bool bEndI = (i == EN_MEMBEND_I) ? true : false;

        const EN_WALL_FORC_POS& CurPos = GetWallForcPos(static_cast<EN_MEMBEND>(i));

        const double dPu = PORslt.dForce[CurPos][DOF_FX] * (-1.0);
        const double dVu = PORslt.dForce[CurPos][DOF_FZ];
        const double dMu = PORslt.dForce[CurPos][DOF_MY];

        const T_WALL_SHR_PARAM& CurShrAvg = aShrAvg[i];
        const T_WALL_SHR_PARAM& CurShrNom = aShrNom[i];

        const double dMe = CalcMn(nPMMethod, Pmcvy, dPu, dMu);
        const double dVe = CalcVnConWall(CurShrAvg);
        const double dVn = CalcVnConWall(CurShrNom);

        T_UPHG_CONWALL_POS& UphgPos = bEndI ? Uphg.EndI : Uphg.EndJ;
        T_CONWALL_INFO& Info = UphgPos.Info;
        Info.dHw = dHw;
        Info.dLw = dLw;
        Info.bBE = bBE;
        Info.dVu = dVu;
        Info.dMe = dMe;
        Info.dVe = dVe;
        Info.dVn = dVn;
        Info.dVp = CalcVpConWall(dMe, dHw, CurShrAvg);
        Info.enCtrlType = CalcWallCtrlType(dMe, dVe, dHw, dLw, CurShrAvg); // 휨-전단지배 판단은 변형지배로 가정한 전단강도 Vn을 사용.
        Info.dAxlIdx    = CalcAxialIndex(dPu, dFck, dAg);
        Info.dShrIdx    = CalcShearIndexWall(dVu, CurShrNom);  // 단면정보와 fck값만 사용하므로 Avg, Nom 둘다 사용 가능.
        Info.bForceCtrl = (Info.enCtrlType == EN_EVALCTRL_SHER && Info.dAxlIdx > 0.15) ? true : false;

        UphgPos.ParamV.dThetay = GetThetay(PORslt, CurPos, DOF_FZ);
        UphgPos.PolcK = PORslt.PolcK;
        UphgPos.nStep = PORslt.nStep;
        UphgPos.dPu = dPu;
        UphgPos.dVu = dVu;
        UphgPos.dMu = dMu;

        T_PHGT_FEMA& rCurPhgt = bEndI ? rPhgtI : rPhgtJ;
        CalcFzHingePropConWallPos(UphgPos, rCurPhgt);

        rCurPhgt.dYieldForce[IDX_PLS] = dVe;
        rCurPhgt.dYieldForce[IDX_MNS] = dVe;
    }

    GetSEResult()->SetUphgConWall(ElemK, Uphg);

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcMyHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
    T_WALL_KEY WallKey;
    T_WALL_D WallD;  WallD.Initialize();
    if (!m_pDoc->m_pPostCtrl->GetWallDataFromElemK(ElemK, WallD, &WallKey))  {  return FALSE; }
    if ( !WallD.bLineWall ) { return FALSE; }

    T_THIK_D ThikD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    if (!GetElemMatlThik(ElemK, ThikD, MatdD, SecmD)) { ASSERT(0); return FALSE; }

    const double dTw = WallD.dThickness;
    const double dLw = WallD.dLength;
    const double dHw = WallD.dHeight;
        
    CSeisEvalDataTool Tool;
    const bool bBE = Tool.IsConsiderBoundaryElement(ElemK);
    const double dFck = Tool.GetFc(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);    
    const double dAg  = dTw*dLw;
    const int nPMMethod = Tool.GetPMCalcMethodWall(ElemK, EN_EVALSTEP_2ND);

    T_WALL_SHR_PARAM aShrAvg[EN_MEMBEND_NUM];
    T_WALL_SHR_PARAM aShrNom[EN_MEMBEND_NUM];
    if ( !MakeWallShearParam(ElemK, EN_EVALSTEP_2ND, EN_DIR_Z, aShrAvg, aShrNom) ) { ASSERT(0); return FALSE; }

    _DB_PMCV_DATA Pmcvy;
    _DB_PMCV_DATA Pmcvz;
    if (!GetPmcvData(ElemK, EN_EVALSTEP_2ND, EN_EVALMATL_AVG, Pmcvy, Pmcvz)) { return FALSE; }

    T_UPHG_CONWALL Uphg;
    Uphg.ElemK = ElemK;
    if ( !GetSEResult()->GetUphgConWall(ElemK, Uphg) ) { Uphg.Initialize(); }

    for ( int i = 0 ; i < EN_MEMBEND_NUM; ++i )
    {
        bool bEndI = ( i == EN_MEMBEND_I ) ? true : false;

        const EN_WALL_FORC_POS& enPos = GetWallForcPos(static_cast<EN_MEMBEND>(i));

        const double dPu = PORslt.dForce[enPos][DOF_FX]*(-1.0);
        const double dVu = PORslt.dForce[enPos][DOF_FZ];
        const double dMu = PORslt.dForce[enPos][DOF_MY];

        T_WALL_SHR_PARAM& CurShrAvg = aShrAvg[i];
        CurShrAvg.SetForc(dPu, dVu, dMu);
        T_WALL_SHR_PARAM& CurShrNom = aShrNom[i];
        CurShrAvg.SetForc(dPu, dVu, dMu);

        const double dMe = CalcMn(nPMMethod, Pmcvy, dPu, fabs(dMu));
        const double dVe = CalcVnConWall(CurShrAvg);
        const double dVn = CalcVnConWall(CurShrNom);

        T_CONWALL_INFO Info;
        Info.dHw = dHw;
        Info.dLw = dLw;
        Info.bBE = bBE;
        Info.dMe = dMe;
        Info.dMu = dMu;
        Info.dVu = dVu;
        Info.dVe = dVe;
        Info.dVn = dVn;
        Info.dVp = CalcVpConWall(dMe, dHw, CurShrAvg);
        Info.enCtrlType = CalcWallCtrlType(dMe, dVe, dHw, dLw, CurShrAvg); // 휨-전단지배 판단은 변형지배로 가정한 전단강도 Vn을 사용.
        Info.dAxlIdx = CalcAxialIndex(dPu, dFck, dAg);
        Info.dShrIdx = CalcShearIndexWall(dVu, CurShrNom); // 단면정보와 fck값만 사용하므로 Avg, Nom 둘다 사용 가능.
        Info.bForceCtrl = (Info.enCtrlType == EN_EVALCTRL_SHER && Info.dAxlIdx > 0.15) ? true : false;

        T_UPHG_CONWALL_POS& UphgPos = bEndI ? Uphg.EndI : Uphg.EndJ;
        UphgPos.Info = Info;
        UphgPos.ParamM.dThetay = GetThetay(PORslt, enPos, DOF_MY);
        UphgPos.PolcK = PORslt.PolcK;
        UphgPos.nStep = PORslt.nStep;
        UphgPos.dPu = dPu;
        UphgPos.dVu = dVu;
        UphgPos.dMu = dMu;

        T_PHGT_FEMA& rCurPhgt = bEndI ? rPhgtI : rPhgtJ;
        CalcMyHingePropConWallPos(UphgPos, rCurPhgt);
    }

    GetSEResult()->SetUphgConWall(ElemK, Uphg);

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcFxHingePropInfill(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_MAP& mPORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
    rPhgtI.bSymmetric  = FALSE;
    rPhgtI.bUserDefineRatio = TRUE;
    rPhgtI.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED; 

    T_ELEM_D ElemD;
    if ( !m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD) ) { ASSERT(0); return FALSE; }

    T_MSIS_D MsisD;
    if ( !GetInfillParam(ElemK, MsisD) ) { ASSERT(0); return FALSE; }

    T_MSIS_INPUT MsisInp;
    T_MSIS_PROP  MsisPro;
    if ( !m_pDoc->m_pAttrCtrl2->CalcInfillStrutPropByElem(ElemK, MsisInp, MsisPro) ) { return FALSE; } 

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SEMM_D SemmD;
    if (!GetElemMatlSectMas(ElemK, SectD, MatdD, SemmD)) { ASSERT(FALSE); return FALSE; } 
    if (SemmD.bUseDefaultVal) { return FALSE; }

    CSeisEvalDataTool Tool;
    /// [JIRA, GEN-646] 2단계 평가시, 평균강도로 적용.
    double dFmC = Tool.GetFmcMas(EN_EVALSTEP_2ND, EN_EVALMATL_AVG, SemmD);
    double dVm  = Tool.GetFvmMas(EN_EVALSTEP_2ND, EN_EVALMATL_AVG, SemmD);
    double dPhi = 1.0; /// 설계가 아니므로, 고려하지 않음.

    T_MISA_D MisaD;
    if (!m_pDoc->m_pAttrCtrl2->GetMisa(ElemK, MisaD)) { ASSERT(FALSE); return FALSE; }

    BOOL bConsiderVbjs = IsConsiderVbjs(ElemK);
    T_EVALLIN_INFILL_CAPA Capa;
    CalcInfillStrn(dFmC, dVm, dPhi, EN_DIAGONAL, MsisD, bConsiderVbjs, Capa); /// [MQC 26497] 힌지 항복강도 계산시에는 Bracing Type과 관계없이 제 강도 사용.

    rPhgtI.nInfillStiff         = 0;
    rPhgtI.dYieldForce[IDX_MNS] = Capa.dVn;
    rPhgtI.dYieldForce[IDX_PLS] = Capa.dVn;
    rPhgtI.dInfillStiff_KmiRop  = MsisPro.dKmi * MsisPro.dRop;
    rPhgtI.dInfillStiff_KmsRop  = MsisPro.dKms * MsisPro.dRop;
    rPhgtI.dInfillStiff_Hm      = MsisInp.dHm;
    rPhgtI.dInfillStiff_ThetaM  = MsisPro.dThetaM;

    double dBeta = 1.0;
    T_TopCWStrut_D TopCW;
    if (!m_pDoc->m_pAttrCtrl2->GetTopColumnOnStrut(ElemK, TopCW))
    {
        dBeta = 0.0;
    }
    else
    {
        T_VFRAME_LIST aVframe;
        T_VFRAME Vframe;
        if (MakeVframe2ndColm(TopCW.ElemKColumn_I, TopCW.bColumnReversed_I, mPORslt, Vframe)) { aVframe.Add(Vframe); }
        if (MakeVframe2ndColm(TopCW.ElemKColumn_J, TopCW.bColumnReversed_J, mPORslt, Vframe)) { aVframe.Add(Vframe); }

        T_ELEM_K_LIST aWallK;
        aWallK.Append(TopCW.aElemKWall_I);
        aWallK.Append(TopCW.aElemKWall_J);
        int nWall = static_cast<int>(aWallK.GetSize());
        for (int m=0; m<nWall; ++m)
        {
            if (MakeVframe2ndWall(aWallK[m], mPORslt, Vframe)) { aVframe.Add(Vframe); }
        }

        double dVm = Capa.dVn; /// [JIRA,GEN-822] 힌지 항복강도 계산시에는 Bracing Type과 관계없이 제 강도 사용.
        dBeta = CalcBeta(ElemK, dVm, aVframe);
    }


    double dPm = Capa.dPhiPn;
    CalcParamInfill(dBeta, MsisInp.dLm, MsisInp.dHm, MsisPro.dKmi, MsisPro.dKms, MsisPro.dThetaM, Capa.dVn, rPhgtI);

    rPhgtJ = rPhgtI;

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcMyHingePropStlBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
    double adDy[2] = { 0.0 };
    for (int i=0; i<2; ++i)
    {
        adDy[i] = GetThetay(PORslt, i, DOF_MY);
    }

    T_MATD_D MatdD;
    T_SECT_D SectD;
    T_SESM_D SesmD;
    if (!GetElemMatlSectStl(ElemK, SectD, MatdD, SesmD)) { ASSERT(FALSE); return FALSE; }

    CSeisEvalDataTool Tool;
    double dFye = Tool.GetFyStl(EN_EVALSTEP_2ND, EN_EVALMATL_AVG, SesmD); //### 두께별 강도를 고려할 필요는 없는가?

    double adBTR[2] = { 0.0, };
    double adDTR[2] = { 0.0, };	
    CalcWTRLimitStlBeam(SectD.SectBefore.Shape, MatdD.Data1.Analysis.Elast, dFye, adBTR, adDTR);

    auto l_CalcHingeParam = [&](const T_SECT_SECTBASE_D& SectBaseD, double dThetayPos, OUT T_PHGT_FEMA& rPhgtPos)
    {
        rPhgtPos.bSymmetric = FALSE;
        rPhgtPos.bUserDefineRatio = TRUE;
        rPhgtPos.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_NORMAL_BILINEAR;

        double dBTR = 0.0;
        double dDTR = 0.0;
        CalcWTR(SectD.SectBefore.Shape, SectBaseD, dBTR, dDTR);

        double adParam[6] = { 0.0, };
        CalcParamStlBeam(dBTR, dDTR, adBTR, adDTR, dThetayPos, adParam);
                
        rPhgtPos.dYieldMoment[0] = dFye*SectBaseD.Design.Zyy;
        rPhgtPos.dYieldMoment[1] = rPhgtPos.dYieldMoment[0];

        SetCurveDispCtrlStl(adParam, rPhgtPos.dForceMomentRatio);
        SetAccetanceCriteria(adParam[HG_IO], adParam[HG_LS], adParam[HG_CP], rPhgtPos);
    };

    l_CalcHingeParam(GetSectBaseEndI(SectD), adDy[0], rPhgtI);
    l_CalcHingeParam(GetSectBaseEndJ(SectD), adDy[1], rPhgtJ);

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcFxHingePropStlColm(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
    T_MATD_D MatdD;
    T_SECT_D SectD;
    T_SESM_D SesmD;
    if (!GetElemMatlSectStl(ElemK, SectD, MatdD, SesmD)) { ASSERT(FALSE); return FALSE; }

    CSeisEvalDataTool Tool;
    double dE   = MatdD.Data1.Analysis.Elast;
    double dFy  = Tool.GetFyStl(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SesmD);
    double dFye = Tool.GetFyStl(EN_EVALSTEP_2ND, EN_EVALMATL_AVG, SesmD);

    const T_SECT_SECTBASE_D& SectBaseI = SectD.SectBefore.SectI;
    const T_SECT_SECTBASE_D& SectBaseJ = SectD.nStype == D_SECT_TYPE_TAPERED ? SectD.SectBefore.SectJ : SectBaseI;

    double dSlenI = CalcStlSlenderness(ElemK, 0, SectD.SectBefore.SectI);
    double dSlenJ = CalcStlSlenderness(ElemK, 4, SectBaseJ);	

    auto l_CalcHinge = [&](const T_SECT_SECTBASE_D& SectBase, double dSlen, OUT T_PHGT_FEMA& rCurPhgt)
    {
        rCurPhgt.bSymmetric = FALSE;
        rCurPhgt.bUserDefineRatio = TRUE;
        rCurPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED; 

        SetCurveForcCtrlAxial(rCurPhgt.dForceMomentRatio);

        double dAg = SectBase.Stiffness.Area;
        rCurPhgt.dYieldForce[IDX_PLS] = CalcPnTensStl(dAg, dFye);
        rCurPhgt.dYieldForce[IDX_MNS] = CalcPnCompStl(dAg, dFy, dE, dSlen);

        /// 인장에 대한 허용기준은 <순수인장력을 받는 기둥>을 따라갑니다.
        rCurPhgt.dIO[IDX_PLS] = 1.25; /// 1.0+0.25
        rCurPhgt.dLS[IDX_PLS] = 4.0;  /// 1.0+3.0
        rCurPhgt.dCP[IDX_PLS] = 6.0;  /// 1.0+5.0
        /// 압축에 대한 허용기준은 휨지배거동으로 합니다.		
        /// [JIRA,GEN-714] 하중지배 거동 성분의 Acceptance Criteria 변경 (0.5, 0.75, 1.0 -> 1.0, 1.0, 1.0)
        rCurPhgt.dIO[IDX_MNS] = 1.0;
        rCurPhgt.dLS[IDX_MNS] = 1.0;
        rCurPhgt.dCP[IDX_MNS] = 1.0;
    };

    l_CalcHinge(SectBaseI, dSlenI, rPhgtI);
    l_CalcHinge(SectBaseJ, dSlenJ, rPhgtJ);

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcMyzHingePropStlColm(T_ELEM_K ElemK, UINT unDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
    double adP[2]  = { 0.0 };
    double adDy[2] = { 0.0 };

    UINT unDOF = unDir==EN_DIR_Y ? DOF_MY : DOF_MZ;
    for (int i=0; i<2; ++i)
    {
        adP[i]  = PORslt.dForce[i][DOF_FX];
        adDy[i] = GetThetay(PORslt, i, unDOF);
    }

    T_MATD_D MatdD;
    T_SECT_D SectD;
    T_SESM_D SesmD;

    if (!GetElemMatlSectStl(ElemK, SectD, MatdD, SesmD)) { ASSERT(FALSE); return FALSE; } 

    CSeisEvalDataTool Tool;
    double dE   = MatdD.Data1.Analysis.Elast;
    double dFy  = Tool.GetFyStl(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SesmD); //## 두께별 강도를 고려하고 있지 않다.
    double dFye = Tool.GetFyStl(EN_EVALSTEP_2ND, EN_EVALMATL_AVG, SesmD);

    int nSeisCompType = Tool.GetSeisComponentType(ElemK);

    const T_SECT_SECTBASE_D& SectBaseI = GetSectBaseEndI(SectD);
    const T_SECT_SECTBASE_D& SectBaseJ = GetSectBaseEndJ(SectD);

    auto l_CalcHinge = [&](const T_SECT_SECTBASE_D& SectBase, double dSlen, double dP, double dThetay, OUT T_PHGT_FEMA& rCurPhgt)
    {
        rCurPhgt.bSymmetric = FALSE;
        rCurPhgt.bUserDefineRatio = TRUE;

        double dPy = CalcPnCompStl(SectBase.Stiffness.Area, dFy, dE, dSlen);
        EN_STLCOLM enColmType = GetStlColmForcType(dP, dPy);
        double dkp = CalcKp(dP, dPy);

        double dBTR = 0.0;
        double dDTR = 0.0;
        CalcWTR(SectD.SectBefore.Shape, SectBase, dBTR, dDTR);

        double adBTR[2] = { 0.0, };
        double adDTR[2] = { 0.0, };
        CalcWTRLimitStlColm(SectD.SectBefore.Shape, enColmType, dE, dFye, adBTR, adDTR);

        double adParam[6] = { 0.0, };
        if (enColmType == EN_STLCOLM_PGT05PY)
        {
            rCurPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
            SetCurveForcCtrlShear(rCurPhgt.dForceMomentRatio);
            /// [JIRA,GEN-714] 하중지배 거동 성분의 Acceptance Criteria 변경 (0.5, 0.75, 1.0 -> 1.0, 1.0, 1.0)
            adParam[HG_IO] = 1.0;
            adParam[HG_LS] = 1.0;
            adParam[HG_CP] = 1.0;		
        }
        else
        {
            rCurPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_NORMAL_BILINEAR;
            CalcParamStlColm(enColmType, dkp, dBTR, dDTR, adBTR, adDTR, dThetay, adParam);
            SetCurveDispCtrlStl(adParam, rCurPhgt.dForceMomentRatio);			
        }

        const double dZ = unDir==EN_DIR_Y ? SectBase.Design.Zyy : SectBase.Design.Zzz;
        rCurPhgt.dYieldMoment[0] = dFye*dZ;
        rCurPhgt.dYieldMoment[1] = rCurPhgt.dYieldMoment[0];

        SetAccetanceCriteria(adParam[HG_IO], adParam[HG_LS], adParam[HG_CP], rCurPhgt);
    };

    double dSlenI = CalcStlSlenderness(ElemK, 0, SectBaseI);
    double dSlenJ = CalcStlSlenderness(ElemK, 4, SectBaseJ);

    l_CalcHinge(SectBaseI, dSlenI, adP[0], adDy[0], rPhgtI);
    l_CalcHinge(SectBaseJ, dSlenJ, adP[1], adDy[1], rPhgtJ);

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcFxHingePropStlBrac(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
    T_MATD_D MatdD;
    T_SECT_D SectD;
    T_SESM_D SesmD;
    if (!GetElemMatlSectStl(ElemK, SectD, MatdD, SesmD)) { ASSERT(0); return FALSE; }

    if (!IsSectShapeStlBrac(SectD.SectBefore.Shape) ) 
    {
        GSaveHistoryFormat(_LS(IDS_DGN_SE_ERROR_NOT_UPDATE_ELEM_BY_SECT_INFO), ElemK);
        return FALSE; 
    }

    CSeisEvalDataTool Tool;
    double dE   = MatdD.Data1.Analysis.Elast;
    double dFy  = Tool.GetFyStl(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SesmD); //## 두께별 강도를 고려하고 있지 않다.
    double dFye = Tool.GetFyStl(EN_EVALSTEP_2ND, EN_EVALMATL_AVG, SesmD);

    int nSeisCompType = Tool.GetSeisComponentType(ElemK);

    auto l_CalcHinge = [&](UINT unBuck, const CString& strShape, const T_SECT_SECTBASE_D& SectBase, double dSlen, OUT T_PHGT_FEMA& rCurPhgt)
    {
        rCurPhgt.bSymmetric = FALSE;
        rCurPhgt.bUserDefineRatio = TRUE;
        rCurPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;

        double dAg  = SectBase.Stiffness.Area;		
        double dPnC = CalcPnCompStl(dAg, dFy, dE, dSlen);
        double dPnT = CalcPnTensStl(dAg, dFye);

        double dBTR = 0.0;
        double dDTR = 0.0;
        CalcWTR(strShape, SectBase, dBTR, dDTR);

        double adBTR[2] = { 0.0, };
        double adDTR[2] = { 0.0, };
        CalcWTRLimitStlColm(strShape, EN_STLCOLM_PLT02PY, dE, dFye, adBTR, adDTR); // Brace에서는 BOx과 Pipe형인 경우에만 판-폭두께비의 의미가 있음.

        double adParamC[6] = { 0.0, };	
        CalcParamStlBracComp(unBuck, strShape, dSlen, dFye, dE, dBTR, dDTR, adBTR, adDTR, adParamC);

        double adParamT[6] = { 0.0, };	
        CalcParamStlBracTens(strShape, adParamT);

        SetCurveDispCtrlStl(adParamT, adParamC, rCurPhgt.dForceMomentRatio);
        SetAccetanceCriteria(adParamT[HG_IO], adParamT[HG_LS], adParamT[HG_CP], adParamC[HG_IO], adParamC[HG_LS], adParamC[HG_CP], rCurPhgt);

        rCurPhgt.dYieldForce[IDX_PLS] = dPnT;
        rCurPhgt.dYieldForce[IDX_MNS] = dPnC;
    };

    const T_SECT_SECTBASE_D& SectBaseI = GetSectBaseEndI(SectD);
    const T_SECT_SECTBASE_D& SectBaseJ = GetSectBaseEndJ(SectD);

    const double dSlenI = CalcStlSlenderness(ElemK, 0, SectBaseI);
    const double dSlenJ = CalcStlSlenderness(ElemK, 4, SectBaseJ);

    l_CalcHinge(EN_BRACBUCK_OUT, SectD.SectBefore.Shape, SectBaseI, dSlenI, rPhgtI);
    l_CalcHinge(EN_BRACBUCK_OUT, SectD.SectBefore.Shape, SectBaseJ, dSlenJ, rPhgtJ);

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcSeisEval1stRsltConColm(T_ELEM_K ElemK, OUT T_EVAL1ST_COLM& raEval1st)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    raEval1st.Initialize();
    raEval1st.ElemK = ElemK;

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    if (!GetElemMatlSectCon(ElemK, SectD, MatdD, SecmD)) { return FALSE; } 
    if (SecmD.bUseDefaultVal) { return FALSE; }

    CSeisEvalDataTool Tool;
    int nSeisCompType = Tool.GetSeisComponentType(ElemK);
    if (nSeisCompType == D_SCMT_TYPE_NONE) { return FALSE; }

    T_REBC_D RebcD;
    if (!m_pDoc->m_pAttrCtrl->GetRebcByElemK(ElemK, RebcD)) { return FALSE; }

    raEval1st.bEvaluate = TRUE;
        
    const double dAlpha = GetAlphaConColm(SectD);
    if (fabs(dAlpha) < cEvalZero) { ASSERT(0); return FALSE; }

    T_EVAL1ST_COLM_LCOM EvalLcom;
    T_LCOM_K_LIST aLcomK;    
    int nSize = GetLcomKeyList1stEval(ElemK, aLcomK);
    
    raEval1st.aEvalLcom.SetSize(nSize);
    if (nSize < 1) { return FALSE; }

    T_COLM_SHR_PARAM aShry[EN_MEMBEND_NUM];
    if ( !MakeColmShearParamEval(ElemK, EN_EVALSTEP_1ST, EN_DIR_Y, aShry) ) { ASSERT(0); return FALSE; }

    T_COLM_SHR_PARAM aShrz[EN_MEMBEND_NUM];
    if ( !MakeColmShearParamEval(ElemK, EN_EVALSTEP_1ST, EN_DIR_Z, aShrz) ) { ASSERT(0); return FALSE; }

    const double dAg  = SectD.SectBefore.SectI.Stiffness.Area;	    
    const double dFactorPnmax = GetFactorPnmaxColm(EN_EVALSTEP_1ST, RebcD.nHoopType);
    
    const double dFck  = Tool.GetFc(EN_EVALSTEP_1ST, EN_EVALMATL_NOM, SecmD);
    const double dFym  = Tool.GetFym(EN_EVALSTEP_1ST, EN_EVALMATL_NOM, SecmD);
    const double dFce  = Tool.GetFc(EN_EVALSTEP_1ST, EN_EVALMATL_AVG, SecmD);
    const double dFyme = Tool.GetFym(EN_EVALSTEP_1ST, EN_EVALMATL_AVG, SecmD);

    const double dAs  = GetRebarAs(RebcD.MainRebarName) * RebcD.nQrb;
    /// nominal axial strength.
    const double dPnC = CalcPnCompCon(dFck, dAg, dFym, dAs, dFactorPnmax);
    const double dPnT = CalcPnTensCon(dFym, dAs);
    /// expect axial strength.
    const double dPeC = CalcPnCompCon(dFce, dAg, dFyme, dAs, dFactorPnmax);
    const double dPeT = CalcPnTensCon(dFyme, dAs);
            
    _DB_PMCV_DATA Pmcvy, Pmcvz;
    if (!GetPmcvData(ElemK, EN_EVALSTEP_1ST, EN_EVALMATL_AVG, Pmcvy, Pmcvz)) { return FALSE; }

    ConvertPmcvData(Pmcvy, raEval1st.Pmcvy);
    ConvertPmcvData(Pmcvz, raEval1st.Pmcvz);

    CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();
    CalcForc1stLcom(ElemK, pMembCtrl);

    const double dL = pMembCtrl->GetMembLengByIncludeElem(ElemK);

    const bool bInfilled = IsConColmConnectedInfill(ElemK);
    const UINT unSeisDetail = Tool.GetSeisDetailType(ElemK);
    
    T_MFAC_D MfacD;
    const BOOL bUserMfac = m_pDoc->m_pAttrCtrl->GetMfac(ElemK, MfacD);
    const int nPMMethod = Tool.GetPMCalcMethodColm(ElemK, EN_EVALSTEP_1ST);

    T_STRB_D StrbDisp, StrbForc;
    double dMaxDCR = -cEvalUpOn;
    for ( int i=0; i<nSize; ++i )
    {		
        T_LCOM_K LcomK = aLcomK[i];
        if (!GetEvalStrb1st(pMembCtrl, ElemK, LcomK, StrbDisp, StrbForc)) { ASSERT(0);	continue; }

        EvalLcom.Initialize();
        EvalLcom.LcomK = LcomK;	
        SetEvalConColm1stForc(StrbDisp, StrbForc, EvalLcom.PosI, EvalLcom.PosJ);

        double adMey[2] = { 0.0, };
        GetMn(nPMMethod, EN_DIR_Y, Pmcvy, EvalLcom, adMey);
        double adMez[2] = { 0.0, };
        GetMn(nPMMethod, EN_DIR_Z, Pmcvz, EvalLcom, adMez);
        
        for ( int m=0; m<EN_MEMBEND_NUM; ++m )
        {
            T_EVAL1ST_COLM_POS& CurPos = (m==EN_MEMBEND_I) ? EvalLcom.PosI : EvalLcom.PosJ;

            const T_COLM_SHR_PARAM& ShrY = aShry[m];
            const T_COLM_SHR_PARAM& ShrZ = aShrz[m];

            const double dPu  = CurPos.dPUF;
            const double dVuy = CurPos.dVUFy;
            const double dVuz = CurPos.dVUFz;
            const double dMuy = CurPos.dMUDy;
            const double dMuz = CurPos.dMUDz;
            const double dMey = adMey[m];
            const double dMez = adMez[m];
            const double dVny = CalcVnConColm(ShrY, dPu, dVuy, dMuz);
            const double dVnz = CalcVnConColm(ShrZ, dPu, dVuz, dMuy);
            SetEvalConColm1stStrn(dPnC, dPnT, dPeC, dPeT, dMey, dMez, dVny, dVnz, CurPos);

            auto l_SetInfo = [this](double dVu, const T_COLM_SHR_PARAM& CurShr, OUT T_CONCOLM_INFO& CurInfo)
            {
                BOOL bGroup1 = IsConditionGroup1(CurShr.Comm);               
                CurInfo.dRhoIdx = CurShr.Comm.CalcRhoIndex();
                CurInfo.dShrIdx = CalcShearIndex(dVu, CurShr.Comm);  
                CurInfo.bConsiderGroup1 = bGroup1;
                CurInfo.unGroup = GetGroupConColm(CurInfo.unSeisDetail, CurInfo.dVp, CurInfo.dVn, bGroup1);
                CurInfo.dS = CurShr.Comm.dSv;
                CurInfo.db = CurShr.Comm.dBv;
                CurInfo.dd = CurShr.Comm.dDv;
            };

            /// local y (My-Vz)            
            T_CONCOLM_INFO& rInfoY = CurPos.Diry.Info;
            rInfoY.unSeisDetail = unSeisDetail;
            rInfoY.bInfilled    = bInfilled;
            rInfoY.dfck = dFck;
            rInfoY.dAg  = dAg;
            rInfoY.dAxlIdx = CalcAxialIndex(dPu, dFck, dAg);            
            rInfoY.dPu = dPu;
            rInfoY.dVu = dVuz;
            rInfoY.dVn = dVnz;
            SetVpColmInfo(adMey, adMey[m], dL, rInfoY);            
            l_SetInfo(dVuz, ShrZ, rInfoY);

            /// local z (Mz-Vy)
            T_CONCOLM_INFO& rInfoZ = CurPos.Dirz.Info;
            rInfoZ.unSeisDetail = unSeisDetail;
            rInfoZ.bInfilled    = bInfilled;
            rInfoZ.dfck = dFck;
            rInfoZ.dAg  = dAg;
            rInfoZ.dAxlIdx = CalcAxialIndex(dPu, dFck, dAg);            
            rInfoZ.dPu = dPu;
            rInfoZ.dVu = dVuy;
            rInfoZ.dVn = dVny;
            SetVpColmInfo(adMez, adMez[m], dL, rInfoZ);            
            l_SetInfo(dVuy, ShrY, rInfoZ);
                        
            const bool bTensPUD = ( CurPos.dPUD < 0.0 ) ? true : false;
            if (bUserMfac)
            {
                if ( bInfilled )
                {
                    CurPos.MfacP.Set(bUserMfac, ( bTensPUD ? MfacD.dMFactor1 : MfacD.dMFactor2 ));
                    CurPos.Diry.MfacM.Set(bUserMfac, MfacD.dMFactor3);
                    CurPos.Dirz.MfacM.Set(bUserMfac, MfacD.dMFactor4);
                }
                else
                {
                    CurPos.Diry.MfacM.Set(bUserMfac, MfacD.dMFactor1);
                    CurPos.Dirz.MfacM.Set(bUserMfac, MfacD.dMFactor2);
                }
            }

            CurPos.bForcCtrlP = true;
            if ( bInfilled )
            {
                CurPos.bForcCtrlP = false;       
                if ( bTensPUD ) 
                {
                    GetMFactTable_ConColmInfilled_Tens(nSeisCompType, CurPos.MfacP.admFac);
                }
                else
                {
                    GetMFactTable_ConColmInfilled_Comp(nSeisCompType, CurPos.MfacP.admFac);
                }                
            }
            
            CalcMFactConColmDir(nSeisCompType, CurPos.Diry);
            CalcMFactConColmDir(nSeisCompType, CurPos.Dirz);
            CalcEval1stConColmPos(dAlpha, CurPos);
        }
        
        EvalLcom.enLevel = max(EvalLcom.PosI.enLevel, EvalLcom.PosJ.enLevel);
        raEval1st.aEvalLcom.SetAt(i, EvalLcom);

        if ( raEval1st.enLevel <= EvalLcom.enLevel )
        {
            const double dCurDCR = EvalLcom.GetDCR(EvalLcom.enLevel);
            if ( dgn::ME(dCurDCR, dMaxDCR) )
            {
                dMaxDCR = dCurDCR;
                raEval1st.RepLcomK = LcomK;
            }
        }
        raEval1st.enLevel = max(raEval1st.enLevel, EvalLcom.enLevel);		
    }

    GetSEResult()->SetResultSE1stConColm(ElemK, raEval1st);

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcSeisEval1stRsltConWall(T_ELEM_K ElemK, OUT T_EVAL1ST_WALL& raEval1st)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    raEval1st.Initialize();

    T_WALL_KEY WallKey;
    T_WALL_D WallD;  WallD.Initialize();
    if (!m_pDoc->m_pPostCtrl->GetWallDataFromElemK(ElemK, WallD, &WallKey))  {  return FALSE; }
    if ( !WallD.bLineWall ) { return FALSE; }

    raEval1st.RepElemK  = ElemK;
    raEval1st.unWallKey = WallKey;

    T_THIK_D ThikD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    if (!GetElemMatlThik(ElemK, ThikD, MatdD, SecmD)) { return FALSE; }
    if (SecmD.bUseDefaultVal) { return FALSE; }

    CSeisEvalDataTool Tool;
    int nSeisCompType = Tool.GetSeisComponentType(ElemK);
    if (nSeisCompType == D_SCMT_TYPE_NONE) { return FALSE; }

    double dTw = WallD.dThickness;
    double dLw = WallD.dLength;
    double dHw = WallD.dHeight;
    double dAg = dTw*dLw;

    T_REBW_K RebwK;
    RebwK.keymap = WallKey;
    T_REBW_D RebwD;
    if (!m_pDoc->m_pAttrCtrl->GetRebw(RebwK, RebwD)) { return FALSE; }

    raEval1st.bEvaluate = TRUE;
        
    const double dFck = Tool.GetFc(EN_EVALSTEP_1ST, EN_EVALMATL_NOM, SecmD);
    const bool   bBE  = Tool.IsConsiderBoundaryElement(RebwD);
    raEval1st.bBE = bBE;

    T_WALL_SHR_PARAM aShrAvg[EN_MEMBEND_NUM];
    T_WALL_SHR_PARAM aShrNom[EN_MEMBEND_NUM];
    MakeWallShearParam(ElemK, EN_EVALSTEP_1ST, EN_DIR_Z, aShrAvg, aShrNom);

    T_MFAC_D MfacD;
    BOOL bUserMfac = m_pDoc->m_pAttrCtrl->GetMfac(ElemK, MfacD);

    _DB_PMCV_DATA PmcvyAvg;
    _DB_PMCV_DATA PmcvzAvg;
    if ( !GetPmcvData(ElemK, EN_EVALSTEP_1ST, EN_EVALMATL_AVG, PmcvyAvg, PmcvzAvg) ) { return FALSE; }

    ConvertPmcvData(PmcvyAvg, raEval1st.PmcvyAvg);
    ConvertPmcvData(PmcvzAvg, raEval1st.PmcvzAvg);

    _DB_PMCV_DATA PmcvyNom;
    _DB_PMCV_DATA PmcvzNom;
    if ( !GetPmcvData(ElemK, EN_EVALSTEP_1ST, EN_EVALMATL_NOM, PmcvyNom, PmcvzNom) ) { return FALSE; }

    ConvertPmcvData(PmcvyNom, raEval1st.PmcvyNom);
    ConvertPmcvData(PmcvzNom, raEval1st.PmcvzNom);

    CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();
    CalcForc1stLcom(ElemK, pMembCtrl);

    UINT unInternalWallID = 0;
    if (!m_pDoc->m_pPostCtrl->GetInternalWallId(ElemK, unInternalWallID)) { ASSERT(0); return FALSE; }
    
    T_EVAL1ST_WALL_LCOM EvalLcom;
    T_LCOM_K_LIST aLcomK;
    int nSize = GetLcomKeyList1stEval(ElemK, aLcomK);	
    raEval1st.aEvalLcom.SetSize(nSize);

    double dMaxDCR = -cEvalUpOn;
    const int nPMMethod = Tool.GetPMCalcMethodWall(ElemK, EN_EVALSTEP_1ST);
    for (int i=0; i<nSize; ++i)
    {
        const T_LCOM_K LcomK = aLcomK[i];

        T_STRW_D StrwDisp, StrwForc;
        if (!GetEvalStrw1st(pMembCtrl, unInternalWallID, LcomK, StrwDisp, StrwForc)) { ASSERT(0); continue; } 

        EvalLcom.Initialize();
        EvalLcom.LcomK = LcomK;
        SetEvalConWall1stForc(StrwDisp, StrwForc, EvalLcom.PosI, EvalLcom.PosJ);

        for ( int m=0; m<EN_MEMBEND_NUM; ++m )
        {
            T_EVAL1ST_WALL_POS& rCurLcomPos = (m==EN_MEMBEND_I) ? EvalLcom.PosI : EvalLcom.PosJ;        

            const double dPUF = rCurLcomPos.dPUF;
            const double dVUF = rCurLcomPos.dVUF;
            const double dMUF = rCurLcomPos.dMUF;
            const double dVUD = rCurLcomPos.dVUD;
            const double dMUD = rCurLcomPos.dMUD;

            T_WALL_SHR_PARAM& CurShrAvg = aShrAvg[m];
            CurShrAvg.SetForc(dPUF, dVUD, dMUD);

            T_WALL_SHR_PARAM& CurShrNom = aShrNom[m];
            CurShrNom.SetForc(dPUF, dVUF, dMUF);

            const double dMe = CalcMn(nPMMethod, PmcvyAvg, dPUF, fabs(dMUD));
            const double dMn = CalcMn(nPMMethod, PmcvyNom, dPUF, fabs(dMUF));
            const double dVe = CalcVnConWall(CurShrAvg);
            const double dVn = CalcVnConWall(CurShrNom);

            if ( bUserMfac )
            {
                rCurLcomPos.MfacMV.Set(bUserMfac, MfacD.dMFactor1);
            }

            T_CONWALL_INFO& Info = rCurLcomPos.Info;
            Info.dHw = dHw;
            Info.dLw = dLw;
            Info.bBE = bBE;
            Info.dMe = dMe;
            Info.dMn = dMn;
            Info.dMu = dMUD;
            Info.dVu = dVUD;            
            Info.dVe = dVe;
            Info.dVn = dVn;
            Info.dVp = CalcVpConWall(dMe, dHw, CurShrAvg);
            Info.dAxlIdx = CalcAxialIndex(dPUF, dFck, dAg);
            Info.dShrIdx = CalcShearIndexWall(dVUD, CurShrAvg); // 단면정보와 fck값만 사용하므로 Avg, Nom 둘다 사용 가능.
            Info.enCtrlType = CalcWallCtrlType(dMe, dVe, dHw, dLw, CurShrAvg); // 휨-전단지배 판단은 변형지배로 가정한 전단강도 Vn을 사용.
            Info.bForceCtrl = (Info.enCtrlType == EN_EVALCTRL_SHER && Info.dAxlIdx > 0.15) ? true : false;

            CalcMFactConWall(nSeisCompType, Info, rCurLcomPos);
            CalcEval1stConWallPos(rCurLcomPos);
        }

        EvalLcom.enLevel = max(EvalLcom.PosI.enLevel, EvalLcom.PosJ.enLevel);
        
        if ( raEval1st.enLevel <= EvalLcom.enLevel )
        {
            const double dCurDCR = EvalLcom.GetDCR(EvalLcom.enLevel);
            if ( dgn::ME(dCurDCR, dMaxDCR) )
            {
                dMaxDCR = dCurDCR;
                raEval1st.RepLcomK = LcomK;
            }
        }
        raEval1st.enLevel = max(raEval1st.enLevel, EvalLcom.enLevel);
        raEval1st.aEvalLcom.SetAt(i, EvalLcom);
    }

    GetSEResult()->SetResultSE1stConWall(ElemK, raEval1st);

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcSeisEval1stRsltInfill(T_ELEM_K ElemK, OUT T_EVAL1ST_INFILL& raEval1st)
{
    SetCodeUnit();

    raEval1st.Initialize();
    raEval1st.ElemK = ElemK;

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SEMM_D SemmD;
    if (!GetElemMatlSectMas(ElemK, SectD, MatdD, SemmD)) { ASSERT(0); return FALSE; } 
    if (SemmD.bUseDefaultVal) { return FALSE; }

    T_ELEM_D ElemD;
    if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(FALSE); return FALSE; }

    CSeisEvalDataTool Tool;
    int nSeisCompType = Tool.GetSeisComponentType(ElemK);
    if (nSeisCompType == D_SCMT_TYPE_NONE) { return FALSE; }

    T_MSIS_D MsisD;
    if (!GetInfillParam(ElemK, MsisD)) { ASSERT(FALSE); return FALSE; }

    /// [JIRA, GEN-646] 1단계 평가시, 평균강도로 적용.
    double dFmC = Tool.GetFmcMas(EN_EVALSTEP_1ST, EN_EVALMATL_AVG, SemmD);
    double dVm  = Tool.GetFvmMas(EN_EVALSTEP_1ST, EN_EVALMATL_AVG, SemmD);
    double dPhi = 1.0; /// 1단계 상세평가에서는 강도감소계수를 고려하지 않습니다.

    T_MISA_D MisaD;
    if (!m_pDoc->m_pAttrCtrl2->GetMisa(ElemK, MisaD)) { ASSERT(0); return FALSE; }

    BOOL bConsiderVbjs = IsConsiderVbjs(ElemK);
    T_EVALLIN_INFILL_CAPA Capa;
    CalcInfillStrn(dFmC, dVm, dPhi, MisaD.nBracingType, MsisD, bConsiderVbjs, Capa);
    double dLmHm = fabs(MsisD.dHm) < cEvalZero ? 0.0 : MsisD.dLm/MsisD.dHm;

    raEval1st.bEvaluate = TRUE;

    T_EVAL1ST_INFILL_LCOM EvalLcom;
    T_LCOM_K_LIST aLcomK;
    const int nSize = GetLcomKeyList1stEval(ElemK, aLcomK);

    raEval1st.aEvalLcom.SetSize(nSize);
    if (nSize < 1) { return FALSE; }

    T_MFAC_D MfacD;
    BOOL bUserMfac = m_pDoc->m_pAttrCtrl->GetMfac(ElemK, MfacD);

    CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();
    CalcForc1stLcom(ElemK, pMembCtrl);

    /// [GEN-1147] 스트럿에 연결된 RC 기둥의 전단강도 계산 속도 개선.
    bool bExistTopCW = false;        
    CMapEx<T_LCOM_K, T_LCOM_K, T_STRB_D, T_STRB_D&> mStrbColmI;
    CMapEx<T_LCOM_K, T_LCOM_K, T_STRB_D, T_STRB_D&> mStrbColmJ;
    T_ELEM_K_LIST aWallK;
    CMapEx<T_LCOM_K, T_LCOM_K, T_STRW_D, T_STRW_D&>* pmStrw = nullptr;

    T_TopCWStrut_D TopCW;
    if ( m_pDoc->m_pAttrCtrl2->GetTopColumnOnStrut(ElemK, TopCW) )
    {
        bExistTopCW = true;
        GetEval1stConColmForcMap(pMembCtrl, TopCW.ElemKColumn_I, aLcomK, mStrbColmI);
        GetEval1stConColmForcMap(pMembCtrl, TopCW.ElemKColumn_J, aLcomK, mStrbColmJ);

        aWallK.Append(TopCW.aElemKWall_I);
        aWallK.Append(TopCW.aElemKWall_J);
        INT_PTR nWall = aWallK.GetSize();
        pmStrw = new CMapEx<T_LCOM_K, T_LCOM_K, T_STRW_D, T_STRW_D&>[nWall];

        for ( INT_PTR i = 0 ; i < nWall ; ++i )
        {
            GetEval1stConWallForcMap(pMembCtrl, aWallK[i], aLcomK, pmStrw[i]);
        }
    }

    T_STOR_K StorK;
    m_pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ElemK);
        
    T_STRT_D StrtDisp, StrtForc;
    for ( INT_PTR i = 0 ; i < nSize ; ++i )
    {
        T_LCOM_K LcomK = aLcomK[i];
        if (!GetEvalStrt1st(pMembCtrl, ElemK, LcomK, StrtDisp, StrtForc)) { ASSERT(FALSE);	continue; }

        EvalLcom.Initialize();
        EvalLcom.LcomK = LcomK;	
        EvalLcom.EndI.dPd = StrtDisp.dblForce[0];
        EvalLcom.EndI.dPc = Capa.dPhiPn;

        EvalLcom.EndJ.dPd = StrtDisp.dblForce[1];		
        EvalLcom.EndJ.dPc = Capa.dPhiPn;

        double dBeta = 1.0;
        if (bUserMfac) 
        {
            EvalLcom.EndI.SetUserMFac(bUserMfac, MfacD.dMFactor1);
            EvalLcom.EndJ.SetUserMFac(bUserMfac, MfacD.dMFactor1);
        }
        else
        {
            if ( !bExistTopCW )
            {
                dBeta = 0.0;
            }
            else
            {
                T_VFRAME_LIST aVframe;
                T_VFRAME Vframe;

                T_STRB_D StrbI;
                if ( mStrbColmI.Lookup(LcomK, StrbI) )
                {
                    if ( MakeVframe1stColm(TopCW.ElemKColumn_I, TopCW.bColumnReversed_I, StrbI, Vframe) ) { aVframe.Add(Vframe); }
                }

                T_STRB_D StrbJ;
                if ( mStrbColmJ.Lookup(LcomK, StrbJ) )
                {
                    if ( MakeVframe1stColm(TopCW.ElemKColumn_J, TopCW.bColumnReversed_J, StrbJ, Vframe) ) { aVframe.Add(Vframe); }
                }

                INT_PTR nWall = aWallK.GetSize();
                for ( INT_PTR m = 0 ; m < nWall ; ++m )
                {
                    T_STRW_D StrwD;
                    if ( pmStrw[m].Lookup(LcomK, StrwD) )
                    {
                        if ( MakeVframe1stWall(aWallK[m], StrwD, Vframe)) { aVframe.Add(Vframe); }
                    }                    
                }

                double dVm = (MisaD.nBracingType == EN_X ? 2 : 1) * Capa.dVn;
                dBeta = CalcBeta(ElemK, dVm, aVframe);
            }
        }        

        CalcEval1stInfillPos(ElemK, dBeta, dLmHm, EvalLcom.EndI);
        CalcEval1stInfillPos(ElemK, dBeta, dLmHm, EvalLcom.EndJ);

        EvalLcom.enLevel  = max(EvalLcom.EndI.DCR.enLevel, EvalLcom.EndJ.DCR.enLevel);
        raEval1st.enLevel = max(raEval1st.enLevel, EvalLcom.enLevel);

        raEval1st.aEvalLcom.SetAt(i, EvalLcom);
    }

    if ( pmStrw != nullptr ) { delete[] pmStrw; pmStrw = nullptr; }

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcSeisEval1stRsltStlBeam(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBEAM& raEval1st)
{
    SetCodeUnit();

    raEval1st.Initialize();
    raEval1st.ElemK = ElemK;

    CSeisEvalDataTool Tool;
    int nSeisCompType = Tool.GetSeisComponentType(ElemK);
    if (nSeisCompType == D_SCMT_TYPE_NONE) { return FALSE; }

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SESM_D SesmD;
    if (!GetElemMatlSectStl(ElemK, SectD, MatdD, SesmD)) { ASSERT(FALSE); return FALSE; } 
    if (SesmD.bUseDefaultVal) { return FALSE; }

    T_EVAL1ST_STLRSLT StlRslt;
    if (!CheckStlSectFor1stEval(ElemK, StlRslt)) { return FALSE; }

    raEval1st.bEvaluate = TRUE;

    int nSize = static_cast<int>(StlRslt.aRsltLcom.GetSize());
    raEval1st.aEvalLcom.SetSize(nSize);
    if (nSize < 1) { return FALSE; }

    double dFye = Tool.GetFyStl(EN_EVALSTEP_1ST, EN_EVALMATL_AVG, SesmD);
    double adBTR[2] = { 0.0, };
    double adDTR[2] = { 0.0, };	
    CalcWTRLimitStlBeam(SectD.SectBefore.Shape, MatdD.Data1.Analysis.Elast, dFye, adBTR, adDTR);

    T_MFAC_D MfacD;
    BOOL bUserMfac = m_pDoc->m_pAttrCtrl->GetMfac(ElemK, MfacD);

    const T_SECT_SECTBASE_D& SectBaseI = GetSectBaseEndI(SectD);	
    double dBTRI = 0.0;
    double dDTRI = 0.0;
    CalcWTR(SectD.SectBefore.Shape, SectBaseI, dBTRI, dDTRI);

    const T_SECT_SECTBASE_D& SectBaseJ = GetSectBaseEndJ(SectD);
    double dBTRJ = 0.0;
    double dDTRJ = 0.0;
    CalcWTR(SectD.SectBefore.Shape, SectBaseJ, dBTRJ, dDTRJ);

    T_LCOM_D LcomD;
    for (int i=0; i<nSize; ++i)
    {
        const T_EVAL1ST_STLRSLT_LCOM& RsltLcom = StlRslt.aRsltLcom[i];
        T_EVAL1ST_STLBEAM_LCOM& EvalLcom = raEval1st.aEvalLcom[i];
        EvalLcom.Initialize();

        if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, RsltLcom.LcomK, LcomD)) { ASSERT(FALSE);  continue; }

        EvalLcom.LcomK = RsltLcom.LcomK;

        if(bUserMfac) EvalLcom.EndI.SetUserMfac(bUserMfac, MfacD.dMFactor1);
        CalcEval1stStlBeamPos(nSeisCompType, RsltLcom.EndI, dBTRI, dDTRI, adBTR, adDTR, EvalLcom.EndI);
        if(bUserMfac) EvalLcom.EndJ.SetUserMfac(bUserMfac, MfacD.dMFactor1);
        CalcEval1stStlBeamPos(nSeisCompType, RsltLcom.EndJ, dBTRJ, dDTRJ, adBTR, adDTR, EvalLcom.EndJ);

        EvalLcom.enLevel  = max(EvalLcom.EndI.enLevel, EvalLcom.EndJ.enLevel);
        raEval1st.enLevel = max(raEval1st.enLevel, EvalLcom.enLevel);
    }

    return TRUE;	
}

BOOL CSeisEvalKISTEC2019::CalcSeisEval1stRsltStlColm(T_ELEM_K ElemK, OUT T_EVAL1ST_STLCOLM& raEval1st)
{
    SetCodeUnit();

    raEval1st.Initialize();
    raEval1st.ElemK = ElemK;

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SESM_D SesmD;
    if (!GetElemMatlSectStl(ElemK, SectD, MatdD, SesmD)) { ASSERT(FALSE); return FALSE; } 

    CSeisEvalDataTool Tool;
    int nSeisCompType = Tool.GetSeisComponentType(ElemK);
    if (nSeisCompType == D_SCMT_TYPE_NONE) { return FALSE; }

    if (SesmD.bUseDefaultVal) { return FALSE; }

    T_EVAL1ST_STLRSLT StlRslt;
    if (!CheckStlSectFor1stEval(ElemK, StlRslt)) { return FALSE; }

    raEval1st.bEvaluate = TRUE;

    int nSize = static_cast<int>(StlRslt.aRsltLcom.GetSize());
    raEval1st.aEvalLcom.SetSize(nSize);
    if (nSize < 1) { return FALSE; }

    const CString& strSectShape = SectD.SectBefore.Shape;

    double dE   = MatdD.Data1.Analysis.Elast;
    double dFy  = Tool.GetFyStl(EN_EVALSTEP_1ST, EN_EVALMATL_NOM, SesmD);
    double dFye = Tool.GetFyStl(EN_EVALSTEP_1ST, EN_EVALMATL_AVG, SesmD);

    T_MFAC_D MfacD;
    BOOL bUserMfac = m_pDoc->m_pAttrCtrl->GetMfac(ElemK, MfacD);

    const T_SECT_SECTBASE_D& SectBaseI = GetSectBaseEndI(SectD);
    const T_SECT_SECTBASE_D& SectBaseJ = GetSectBaseEndJ(SectD);

    double dSlenI = CalcStlSlenderness(ElemK, 0, SectBaseI);
    double dSlenJ = CalcStlSlenderness(ElemK, 4, SectBaseJ);

    double dPyI = CalcPnCompStl(SectBaseI.Stiffness.Area, dFy, dE, dSlenI);
    double dPyJ = CalcPnCompStl(SectBaseJ.Stiffness.Area, dFy, dE, dSlenJ);

    T_LCOM_D LcomD;
    for (int i=0; i<nSize; ++i)
    {
        const T_EVAL1ST_STLRSLT_LCOM& RsltLcom = StlRslt.aRsltLcom[i];
        T_EVAL1ST_STLCOLM_LCOM& EvalLcom = raEval1st.aEvalLcom[i];
        EvalLcom.Initialize();

        if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, RsltLcom.LcomK, LcomD)) { ASSERT(FALSE);  continue; }

        EvalLcom.LcomK = RsltLcom.LcomK;

        if (bUserMfac) { EvalLcom.PosI.SetUserMfac(TRUE, MfacD.dMFactor1, MfacD.dMFactor2, MfacD.dMFactor3); }
        CalcEval1stStlColmPos(nSeisCompType, strSectShape, SectBaseI, dE, dFye, dPyI, RsltLcom.EndI, EvalLcom.PosI);

        if (bUserMfac) { EvalLcom.PosJ.SetUserMfac(TRUE, MfacD.dMFactor1, MfacD.dMFactor2, MfacD.dMFactor3); }
        CalcEval1stStlColmPos(nSeisCompType, strSectShape, SectBaseJ, dE, dFye, dPyJ, RsltLcom.EndJ, EvalLcom.PosJ);				

        EvalLcom.enLevel = max(EvalLcom.PosI.DCR.enLevel, EvalLcom.PosJ.DCR.enLevel);
        raEval1st.enLevel = max(raEval1st.enLevel, EvalLcom.enLevel);
    }

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcSeisEval1stRsltStlBrac(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBRAC& raEval1st)
{
    SetCodeUnit();

    raEval1st.Initialize();
    raEval1st.ElemK = ElemK;

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SESM_D SesmD;
    if (!GetElemMatlSectStl(ElemK, SectD, MatdD, SesmD)) { ASSERT(FALSE); return FALSE; } 
    if (SesmD.bUseDefaultVal) { return FALSE; }

    CSeisEvalDataTool Tool;
    int nSeisCompType = Tool.GetSeisComponentType(ElemK);
    if (nSeisCompType == D_SCMT_TYPE_NONE) { return FALSE; }

    T_EVAL1ST_STLRSLT StlRslt;
    if (!CheckStlSectFor1stEval(ElemK, StlRslt)) { return FALSE; }

    raEval1st.bEvaluate = TRUE;

    int nSize = static_cast<int>(StlRslt.aRsltLcom.GetSize());
    raEval1st.aEvalLcom.SetSize(nSize);
    if (nSize < 1) { return FALSE; } 

    double dFye = Tool.GetFyStl(EN_EVALSTEP_1ST, EN_EVALMATL_AVG, SesmD);
    const CString& strSectShape = SectD.SectBefore.Shape;

    const T_SECT_SECTBASE_D& SectBaseI = GetSectBaseEndI(SectD);
    const T_SECT_SECTBASE_D& SectBaseJ = GetSectBaseEndJ(SectD);

    double dSlenI = CalcStlSlenderness(ElemK, 0, SectBaseI);
    double dSlenJ = CalcStlSlenderness(ElemK, 4, SectBaseJ);

    UINT unBuckling = BUCK_IN;  //###

    T_MFAC_D MfacD;
    BOOL bUserMfac = m_pDoc->m_pAttrCtrl->GetMfac(ElemK, MfacD);

    double dE   = MatdD.Data1.Analysis.Elast;
    double adBTR[2] = { 0.0, };
    double adDTR[2] = { 0.0, };
    CalcWTRLimitStlColm(strSectShape, EN_STLCOLM_PLT02PY, dE, dFye, adBTR, adDTR); // Brace에서는 BOx과 Pipe형인 경우에만 판-폭두께비의 의미가 있음.

    T_LCOM_D LcomD;
    for (int i=0; i<nSize; ++i)
    {
        const T_EVAL1ST_STLRSLT_LCOM& RsltLcom = StlRslt.aRsltLcom[i];
        T_EVAL1ST_STLBRAC_LCOM& EvalLcom = raEval1st.aEvalLcom[i];
        EvalLcom.Initialize();

        if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, RsltLcom.LcomK, LcomD)) { ASSERT(FALSE);  continue; }

        EvalLcom.LcomK = RsltLcom.LcomK;

        if (bUserMfac) { EvalLcom.PosI.SetUserMFac(TRUE, (RsltLcom.EndI.dPUD < 0.0 ? TRUE : FALSE), MfacD.dMFactor1, MfacD.dMFactor2); }
        CalcEval1stStlBracPos(nSeisCompType, strSectShape, SectBaseI, dFye, dE, dSlenI, adBTR, adDTR, unBuckling, RsltLcom.EndI, EvalLcom.PosI);
        if (bUserMfac) { EvalLcom.PosJ.SetUserMFac(TRUE, (RsltLcom.EndJ.dPUD < 0.0 ? TRUE : FALSE), MfacD.dMFactor1, MfacD.dMFactor2); }
        CalcEval1stStlBracPos(nSeisCompType, strSectShape, SectBaseJ, dFye, dE, dSlenJ, adBTR, adDTR, unBuckling, RsltLcom.EndJ, EvalLcom.PosJ);	

        EvalLcom.enLevel  = max(EvalLcom.PosI.DCR.enLevel, EvalLcom.PosJ.DCR.enLevel);
        raEval1st.enLevel = max(raEval1st.enLevel, EvalLcom.enLevel);
    }

    return TRUE;
}   


BOOL CSeisEvalKISTEC2019::CalcEvalPreInfWallSimple(T_ELEM_K ElemK, OUT T_EVALPRE_INFL& rEvalPre)
{
    T_SEME_D SemeD;
    if ( !m_pDoc->m_pAttrCtrl2->GetSeme(SemeD) ) { SemeD.Initialize(); }
    
    rEvalPre.Initialize();

    BOOL bConsiderInfill = SemeD.data.eval.bEffectMasonry;
    if ( !bConsiderInfill ) { return TRUE; }

    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    CSeisEvalDataTool Tool;
    if ( !Tool.IsConsiderPreEval(ElemK) ) { return FALSE; }

    /// 벽체와 같이 Global X,Y 방향에 나란한 경우에만 고려합니다.
    T_ELEM_D ElemD;
    if ( !m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD) ) { ASSERT(0); return FALSE; }
    
    T_NODE_D stNodeD;
    if ( !m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], stNodeD) ) { ASSERT(0); return FALSE; }
    T_NODE_D edNodeD;
    if ( !m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], edNodeD) ) { ASSERT(0); return FALSE; } 
    
    double adVecXYorg[2] = { edNodeD.x - stNodeD.x, edNodeD.y - stNodeD.y };
    double adVecXYnor[2] = { 0.0, 0.0 };
    CMathFunc::mathNormalize2D(adVecXYorg, adVecXYnor);

    const double dAngle = CMathFunc::mathCrossAngle2D(adVecXYnor[0], adVecXYnor[1], 1.0, 0.0) * CMathFunc::m_trrad;
    
    rEvalPre.ElemK = ElemK;
    if ( CMathFunc::mathCos(dAngle) < cEvalZero ) /// cos(angle) = 0 -> Global-Y 
    {
        CalcCsPreInfill(ElemK, rEvalPre.DirY);
    }
    if ( CMathFunc::mathSin(dAngle) < cEvalZero ) /// sin(angle) = 0 -> Global-X 
    {
        CalcCsPreInfill(ElemK, rEvalPre.DirX);
    }

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcSeisEval2ndRsltConColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_COLM& raEval2nd)
{
    SetCodeUnit();

    raEval2nd.Initialize();

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    if (!GetElemMatlSectCon(ElemK, SectD, MatdD, SecmD)) { return FALSE; } 
    if (SecmD.bUseDefaultVal) return FALSE;

    CSeisEvalDataTool Tool;
    if ( Tool.GetSeisComponentType(ElemK) == D_SCMT_TYPE_NONE) return FALSE;

    T_REBC_D RebcD;
    if (!m_pDoc->m_pAttrCtrl->GetRebcByElemK(ElemK, RebcD)) { return FALSE; }

    double adForc[2][6] = { 0.0, };
    double adM1y[2] = { 0.0, };
    double adM1z[2] = { 0.0, };
    GetPORsltForc(ElemK, PolcK, nStep, nAddStepIdx, adForc, adM1y, adM1z);

    if (!GetSeisEval2ndRsltSubConColm(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd))  return FALSE;

    raEval2nd.bEvaluate = TRUE;

    double dFck = Tool.GetFc(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);
    double dFym = Tool.GetFym(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);
    double dFys = Tool.GetFys(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);

    double dB = 0.0;
    double dH = 0.0;
    if (!GetSectDim(SectD, dB, dH)) { ASSERT(FALSE); return FALSE; }
    double dAg  = SectD.SectBefore.SectI.Stiffness.Area;

    T_COLM_SHR_PARAM aShry[EN_MEMBEND_NUM];
    if (!MakeColmShearParamEval(ElemK, EN_EVALSTEP_2ND, EN_DIR_Y, aShry)) { ASSERT(0); return FALSE; }

    T_COLM_SHR_PARAM aShrz[EN_MEMBEND_NUM];
    if (!MakeColmShearParamEval(ElemK, EN_EVALSTEP_2ND, EN_DIR_Z, aShrz)) { ASSERT(0); return FALSE; }

    const double dFactorPnmax = GetFactorPnmaxColm(EN_EVALSTEP_2ND, RebcD.nHoopType);
    double dAs  = GetRebarAs(RebcD.MainRebarName) * RebcD.nQrb;
    double dPnC = CalcPnCompCon(dFck, dAg, dFym, dAs, dFactorPnmax);
    double dPnT = CalcPnTensCon(dFym, dAs);

	double adL[2] = { 0.0, };
	CalConColmLengthConsiderInfillStrutPosition(ElemK, adL);
	const double dLy = adL[1];
	const double dLz = adL[0];

    UINT unSeisDetail = Tool.GetSeisDetailType(ElemK);

    double dMeyi = fabs(adM1y[0]);
    double dMezi = fabs(adM1z[0]);
    double dMeyj = fabs(adM1y[1]);
    double dMezj = fabs(adM1z[1]);

    for (int i=0; i<EN_MEMBEND_NUM; ++i)
    {
        T_EVAL2ND_COLM_POS& rCurPos = i==EN_MEMBEND_I ? raEval2nd.PosI : raEval2nd.PosJ;
        T_COLM_SHR_PARAM CurShry = aShry[i];
        T_COLM_SHR_PARAM CurShrz = aShrz[i];

        double dPu  = adForc[i][DOF_FX]*(-1.0);
        double dVuy = adForc[i][DOF_FY];
        double dVuz = adForc[i][DOF_FZ];
        double dMuy = adForc[i][DOF_MY];
        double dMuz = adForc[i][DOF_MZ];       

        double dVny = CalcVnConColm(CurShry, dPu, dVuy, dMuz);
        double dVnz = CalcVnConColm(CurShrz, dPu, dVuz, dMuy);
        double dVsy = CalcVsFram(CurShry.Comm);
        double dVsz = CalcVsFram(CurShrz.Comm);

        BOOL bConsiderGroup1y = IsConditionGroup1(CurShry.Comm);
        BOOL bConsiderGroup1z = IsConditionGroup1(CurShrz.Comm);

        rCurPos.dAxialIndex = CalcAxialIndex(dPu, dFck, dAg);
        rCurPos.Diry.dRhoIndex = CurShrz.Comm.CalcRhoIndex();
        rCurPos.Dirz.dRhoIndex = CurShry.Comm.CalcRhoIndex();
        CalcSeisEval2ndRsltConColmDir(unSeisDetail, bConsiderGroup1z, dVuz, dVsz, dVnz, CurShrz.Comm, dLy, dMeyi, dMeyi, dMeyj, rCurPos.Diry);
        CalcSeisEval2ndRsltConColmDir(unSeisDetail, bConsiderGroup1y, dVuy, dVsy, dVny, CurShry.Comm, dLz, dMezi, dMezi, dMezj, rCurPos.Dirz);		        
    }

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcSeisEval2ndRsltConWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_WALL& raEval2nd)
{
    SetCodeUnit();

    raEval2nd.Initialize();

    T_WALL_KEY WallKey;
    T_WALL_D WallD;  WallD.Initialize();
    if (!m_pDoc->m_pPostCtrl->GetWallDataFromElemK(ElemK, WallD, &WallKey))  {  return FALSE; }
    if ( !WallD.bLineWall ) { return FALSE; }

    raEval2nd.unWallKey = WallKey;

    T_THIK_D ThikD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    if (!GetElemMatlThik(ElemK, ThikD, MatdD, SecmD)) { return FALSE; }
    if (SecmD.bUseDefaultVal) return FALSE;

    CSeisEvalDataTool Tool;
    if ( Tool.GetSeisComponentType(ElemK) == D_SCMT_TYPE_NONE) return FALSE;

    double dTw = WallD.dThickness;
    double dLw = WallD.dLength;
    double dHw = WallD.dHeight;
    double dAg = dTw*dLw;

    T_REBW_K RebwK;
    RebwK.keymap = WallKey;
    T_REBW_D RebwD;
    if (!m_pDoc->m_pAttrCtrl->GetRebw(RebwK, RebwD)) { return FALSE; }

    double adForc[2][6] = { { 0.0 }, };
    double adM1y[2] = { 0.0, };
    double adM1z[2] = { 0.0, };
    GetPORsltForc(ElemK, PolcK, nStep, nAddStepIdx, adForc, adM1y, adM1z);

    if (!GetSeisEval2ndRsltSubConWall(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd))  return FALSE;

    raEval2nd.bEvaluate = TRUE;

    const double dFck = Tool.GetFc(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);

    T_WALL_SHR_PARAM aShrAvg[EN_MEMBEND_NUM];
    T_WALL_SHR_PARAM aShrNom[EN_MEMBEND_NUM];
    if ( !MakeWallShearParam(ElemK, EN_EVALSTEP_2ND, EN_DIR_Z, aShrAvg, aShrNom) ) { ASSERT(0); return FALSE; }

    for ( int i=0; i<EN_MEMBEND_NUM; ++i )
    {
        const EN_WALL_FORC_POS& enPos = GetWallForcPos(static_cast<EN_MEMBEND>(i));
        double dPu = adForc[enPos][0]*(-1.0);
        double dVu = adForc[enPos][2];
        double dMu = adForc[enPos][4];
        double dMe = adM1y[enPos];

        T_WALL_SHR_PARAM& CurShrAvg = aShrAvg[i];
        CurShrAvg.SetForc(dPu, dVu, dMu);
        T_WALL_SHR_PARAM& CurShrNom = aShrNom[i];
        CurShrNom.SetForc(dPu, dVu, dMu);

        const double dVe = CalcVnConWall(CurShrAvg);

        T_EVAL2ND_WALL_POS& rCurPos = i==EN_MEMBEND_I ? raEval2nd.PosI : raEval2nd.PosJ;

        /// [JIRA,GEN-631] 전단지배이고 벽체 축력비 > 0.15인 경우라도 변형지배로 고려. (해석 수렴성 향상을 위함)
        rCurPos.dAxialIndex = CalcAxialIndex(dPu, dFck, dAg);
        rCurPos.dShearIndex = CalcShearIndexWall(dVu, CurShrNom);
        rCurPos.enCtrlType  = CalcWallCtrlType(dMe, dVe, dHw, dLw, CurShrAvg);
        rCurPos.bForceCtrl  = FALSE; //(raEval2nd.PosI.enCtrlType == EN_EVALCTRL_SHER && raEval2nd.PosI.dAxialIndex > 0.15) ? TRUE : FALSE;
    }

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcSeisEval2ndRsltInfWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_INFILL& raEval2nd)
{
    raEval2nd.Initialize();
    int nAddStepIdx = -1;
    return CalcSeisEval2ndRsltInfWall(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd);	
}

BOOL CSeisEvalKISTEC2019::CalcSeisEval2ndRsltInfWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_INFILL& raEval2nd)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    raEval2nd.Initialize();

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SEMM_D SemmD;
    if (!GetElemMatlSectMas(ElemK, SectD, MatdD, SemmD)) { ASSERT(FALSE); return FALSE; } 
    if (SemmD.bUseDefaultVal) { return FALSE; }

    CSeisEvalDataTool Tool;
    if ( Tool.GetSeisComponentType(ElemK) == D_SCMT_TYPE_NONE ) return FALSE;

    if (!GetSeisEval2ndRsltSubInfWall(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd))  return FALSE;

    raEval2nd.bEvaluate = TRUE;

    return TRUE;
}

double CSeisEvalKISTEC2019::GetAlphaConColm(const T_SECT_D& SectD)
{
    const CString& strShape = SectD.SectBefore.Shape;    
    if (strShape == D_SECT_SHAPE_REG_SB)
    {
        double dH = SectD.SectBefore.SectI.Size[0];
        double dB = SectD.SectBefore.SectI.Size[1];
        return fabs(dH-dB) < cEvalZero ? 1.75 : 1.5;
    }
    if (strShape == D_SECT_SHAPE_REG_SR)
    {
        return 1.75;		
    }
    ASSERT(0);
    return 0.0;
}

void CSeisEvalKISTEC2019::SetAccetanceCriteriaForcCtrl(OUT T_PHGT_FEMA& rPhgtD)
{
    SetAccetanceCriteria(1.0, 1.0, 1.0, rPhgtD);
}

void CSeisEvalKISTEC2019::SetAccetanceCriteriaConAxial(OUT T_PHGT_FEMA& rPhgtD)
{
    SetAccetanceCriteria(2.0, 4.0, 6.0, 1.0, 1.0, 1.0, rPhgtD);
}

EN_LEVEL CSeisEvalKISTEC2019::GetPerformLevelForcCtrl(const double dDCR[3])
{
    /// 하중지배 거동인 경우, DCR=1.0을 기준으로 IO / COLLAPSE 로 판단합니다.
    if (dDCR[EN_LEVEL_CP] > 1.0) { return EN_LEVEL_CL; }
    return EN_LEVEL_IO;
}

double CSeisEvalKISTEC2019::GetMFactForc(const EN_LEVEL enLevel)
{
    switch (enLevel)
    {
    case EN_LEVEL_IO: return 1.0;
    case EN_LEVEL_LS: return 1.0;
    case EN_LEVEL_CP: return 1.0;
    case EN_LEVEL_CL: return 1.0;       
    default: ASSERT(0); return 1.0;
    }
}

double CSeisEvalKISTEC2019::GetPhiInfill()
{
    T_SEME_D SemeD;
    if (!m_pDoc->m_pAttrCtrl2->GetSeme(SemeD)) { SemeD.Initialize(); }
    return SemeD.data.eval.dPhi_v;
}

double CSeisEvalKISTEC2019::CalcK1(const T_COLM_SHR_PARAM& ColmShr)
{
    double dK1 = 0.0;
    switch ( ColmShr.nShrMethod )
    {
    case D_SEME_SHRSTR_MOE2018:
        dK1 = CalcK1_EVAL(ColmShr.Comm);
        break;
    case D_SEME_SHRSTR_STANDARD:
        dK1 = ColmShr.bAutoK1 ? CalcK1_KBC(ColmShr.Comm) : ColmShr.dUserK1;
        break;
    default: ASSERT(0); 
        break;
    }
    return dK1;
}

double CSeisEvalKISTEC2019::CalcVsmaxFram(const T_CON_SHR_PARAM& Shr)
{
    return 2.0 * sqrt(Shr.dFck) / 3.0 * Shr.dBv * Shr.dDv;
}

BOOL CSeisEvalKISTEC2019::CalcMyHingePropConBeamPos(OUT T_UPHG_CONBEAM_POS& rUphg, OUT T_PHGT_FEMA& rPhgt)
{
    rPhgt.bSymmetric = FALSE;
    rPhgt.bUserDefineRatio = TRUE;

    const T_CONBEAM_INFO& Info = rUphg.Info;
    const double dThetayBot = rUphg.ParamP.dThetay;
    const double dThetayTop = rUphg.ParamN.dThetay;

    double adParamP[6] = { 0.0, };
    double adParamN[6] = { 0.0, };

    switch ( Info.enCtrlType )
    {
    case EN_EVALCTRL_FLEX:
        {
            T_MEMBPARAM_TABLE_2 Table2;
            GetParamTable_ConBeam_CtrlFlex(Info.bSeisConform, Table2);

            double adTableP[6] = { 0.0 };
            CalcParamTable2(Table2, Info.dRhoIdxBot, Info.dShrIdx, adTableP);
            CalcHingeParamConFromAB(dThetayBot, adTableP, adParamP);
            SetUphgParam(adTableP, adParamP, rUphg.ParamP);

            double adTableN[6] = { 0.0 };
            CalcParamTable2(Table2, Info.dRhoIdxTop, Info.dShrIdx, adTableN);
            CalcHingeParamConFromAB(dThetayTop, adTableN, adParamN);
            SetUphgParam(adTableN, adParamN, rUphg.ParamN);

            rPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_STIFF_DEGRADE;
        }
        break;
    case EN_EVALCTRL_SHER:
        {
            double adTable[6] = { 0.0, };
            GetParamTable_ConBeam_CtrlSher(Info.IsSpaceGTd2(), adTable);

            CalcHingeParamConFromAB(dThetayBot, adTable, adParamP);
            SetUphgParam(adTable, adParamP, rUphg.ParamP);

            CalcHingeParamConFromAB(dThetayTop, adTable, adParamN);
            SetUphgParam(adTable, adParamN, rUphg.ParamN);

            rPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
        }
        break;
    default: ASSERT(0);
        break;
    }

    rPhgt.dYieldMoment[IDX_PLS] = 0.9 * Info.FlexBot.dMe; // MY(+)
    rPhgt.dYieldMoment[IDX_MNS] = 0.9 * Info.FlexTop.dMe;

    SetCurveDispCtrl(adParamP, adParamN, rPhgt.dForceMomentRatio);
    SetAccetanceCriteria(adParamP, adParamN, rPhgt);

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcMyzHingePropConColmPos(OUT T_UPHG_CONCOLM_DIR& rUphg, OUT T_PHGT_FEMA& rPhgt)
{
    rPhgt.bSymmetric = FALSE;
    rPhgt.bUserDefineRatio = TRUE;	

    UINT unGroup = rUphg.Info.unGroup;
    /// group 3이면 group 2의 형태로 힌지를 만든다.
    if (unGroup == EN_CONCOLM_GROUP3) { unGroup = EN_CONCOLM_GROUP2; }

    const double dAxl = rUphg.Info.dAxlIdx;
    const double dRho = rUphg.Info.dRhoIdx;
    const double dShr = rUphg.Info.dShrIdx;
    const double dThy = rUphg.Param.dThetay;

    double adTable[6] = { 0.0, };    
    switch (unGroup)
    {
    case EN_CONCOLM_GROUP1:
        {
            T_MEMBPARAM_TABLE_2 Table2;
            GetParamTable_ConColm_Group1(Table2);
            CalcParamTable2(Table2, dAxl, dRho, adTable);
        }
        break;
    case EN_CONCOLM_GROUP2:
        {
            T_MEMBPARAM_TABLE_3 Table3;
            GetParamTable_ConColm_Group2(Table3);
            CalcParamTable3(Table3, dAxl, dRho, dShr, adTable);
        }
        break;
    case EN_CONCOLM_GROUP3:
        {
            /// 지금은 group 3으로 들어오는 일이 없음..
            T_MEMBPARAM_TABLE_2 Table2;
            GetParamTable_ConColm_Group3(Table2);            
            CalcParamTable2(Table2, dAxl, dRho, adTable);            
        }
        break;
    default: ASSERT(FALSE); break;
    }

    double adParam[6] = { 0.0, };
    CalcHingeParamConFromAB(dThy, adTable, adParam);

    switch ( unGroup )
    {
    case EN_CONCOLM_GROUP1:
    case EN_CONCOLM_GROUP2:
        {
            rPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_STIFF_DEGRADE;
            SetCurveDispCtrl(adParam, rPhgt.dForceMomentRatio);
        }
        break;
    case EN_CONCOLM_GROUP3:
        {
            rPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
            SetCurveForcCtrlShear(rPhgt.dForceMomentRatio); /// 4.1.7[해설]에 따라서, Group iii은 하중지배로 고려.

            /// <표 5-11>의 group3 > b값을 고려해서 수렴성을 높이는 방향으로 반영. (group3으로 들어오면 다시 확인)
            rPhgt.dForceMomentRatio[IDX_DDY][IDX_CURV_ME] = -1.0*adParam[HG_B];
            rPhgt.dForceMomentRatio[IDX_DDY][IDX_CURV_E ] =      adParam[HG_B];
        }
        break;        
    default: ASSERT(0);
        break;
    }

    SetAccetanceCriteria(adParam, rPhgt);

    memcpy(rUphg.Param.adTable, adTable, sizeof(double)*6);
    memcpy(rUphg.Param.adParam, adParam, sizeof(double)*6);    

    return TRUE;
}


BOOL CSeisEvalKISTEC2019::CalcFzHingePropConWallPos(OUT T_UPHG_CONWALL_POS& rUphg, OUT T_PHGT_FEMA& rPhgt)
{
    rPhgt.bSymmetric  = FALSE;
    rPhgt.bUserDefineRatio = TRUE;        

    /// [JIRA,GEN-631] 전단지배이고 벽체 축력비 > 0.15인 경우라도 변형지배로 고려. (해석 수렴성 향상을 위함)
    // 	if (enCtrlType == EN_EVALCTRL_SHER && dAxialIndex > 0.15)
    // 	{
    // 		adParam[HG_IO] = 0.50;
    // 		adParam[HG_LS] = 0.75;
    // 		adParam[HG_CP] = 1.00;
    // 		enCurvType = EN_EVALCURV_FORC;
    //      rPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_NORMAL_BILINEAR; /// [PMS 5695-136] 전단지배는 Unloading Stiffness Type을 Bilinear로. 
    //      SetCurveForcCtrlShear(rPhgt.dForceMomentRatio);
    // 	}
    const double dAxlIdx = rUphg.Info.dAxlIdx;
    const double dThetay = rUphg.ParamV.dThetay;

    double adTable[6] = { 0.0, };
    GetParamTable_ConWall_CtrlSher(dAxlIdx, adTable);
    double adParam[6] = { 0.0, };
    CalcHingeParamConFromDE(dThetay, adTable, adParam);        

    rPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
    SetCurveDispCtrl(adParam, rPhgt.dForceMomentRatio);
    SetAccetanceCriteria(adParam, rPhgt);

    SetUphgParam(adTable, adParam, rUphg.ParamV);

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcMyHingePropConWallPos(OUT T_UPHG_CONWALL_POS& rUphg, OUT T_PHGT_FEMA& rPhgt)
{
    rPhgt.bSymmetric = FALSE;
    rPhgt.bUserDefineRatio = TRUE;

    const T_CONWALL_INFO& Info = rUphg.Info;

    EN_EVALCURV enCurvType = EN_EVALCURV_DISP;
    const double dAxlIdx = Info.dAxlIdx;
    const double dThetay = rUphg.ParamM.dThetay;

    double adTable[6] = { 0.0, };
    double adParam[6] = { 0.0, };
    switch ( rUphg.Info.enCtrlType )
    {
    case EN_EVALCTRL_SHER:
        {
            /// [JIRA,GEN-631] 전단지배이고 벽체 축력비 > 0.15인 경우라도 변형지배로 고려. (해석 수렴성 향상을 위함)
            // 		if (dAxialIndex > 0.15)
            // 		{
            // 			adParam[HG_IO] = 0.50;
            // 			adParam[HG_LS] = 0.75;
            // 			adParam[HG_CP] = 1.00;
            // 			enCurvType = EN_EVALCURV_FORC;
            // 		}
            // 		else
            // 		{

            GetParamTable_ConWall_CtrlSher(dAxlIdx, adTable);
            CalcHingeParamConFromDE(dThetay, adTable, adParam);

            enCurvType = EN_EVALCURV_DISPRATIO;
        }
        break;
    case EN_EVALCTRL_FLEX:
        {
            const double dShrIdx = Info.dShrIdx;
            const BOOL bBE = Info.bBE;

            T_MEMBPARAM_TABLE_2 Table2;
            GetParamTable_ConWall_CtrlFlex(bBE, Table2);
            CalcParamTable2(Table2, dAxlIdx, dShrIdx, adTable);
            CalcHingeParamConFromAB(dThetay, adTable, adParam);

            enCurvType = EN_EVALCURV_DISP;
        }
        break;
    default: ASSERT(0);
        break;
    }

    switch (enCurvType)
    {		
    case EN_EVALCURV_DISP:
    case EN_EVALCURV_DISPRATIO:
        {
            rPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_STIFF_DEGRADE;
            SetCurveDispCtrl(adParam, rPhgt.dForceMomentRatio);
        }
        break;
    case EN_EVALCURV_FORC:
        {
            rPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_NORMAL_BILINEAR; /// [PMS 5695-136] 전단지배는 Unloading Stiffness Type을 Bilinear로. 
            SetCurveForcCtrlShear(rPhgt.dForceMomentRatio);
        }		
        break;
    default: ASSERT(FALSE);
        break;
    }

    SetAccetanceCriteria(adParam, rPhgt);

    SetUphgParam(adTable, adParam, rUphg.ParamM);

    return TRUE;
}

void CSeisEvalKISTEC2019::CalcParamInfill(double dBeta, double dLm, double dHm, double dKmi, double dKms, double dThetam, double dQy, OUT T_PHGT_FEMA& rPhgt)
{
    double dLmHm  = fabs(dHm) < cEvalZero ? 0.0 : dLm/dHm;

    auto L_GetParam =[](double dLmHmRat, double dD05, double dD10, double dD20)
    {
        if (dLmHmRat < 0.5) { return dD05; }
        if (dLmHmRat < 1.0) { return CMathFunc::mathInterpolate(dD05, dD10, (dLmHmRat-0.5)/0.5); }
        if (dLmHmRat < 2.0) { return CMathFunc::mathInterpolate(dD10, dD20, dLmHmRat-1.0); }
        return dD20;
    };

    double dD  = 0.0;
    double dLS = 0.0;
    if (dBeta < 0.7)
    {
        dD  = L_GetParam(dLmHm, 0.005, 0.004, 0.003);
        dLS = L_GetParam(dLmHm, 0.004, 0.003, 0.002);		
    }
    else if (dBeta < 1.3)
    {
        dD  = L_GetParam(dLmHm, 0.010, 0.008, 0.006);
        dLS = L_GetParam(dLmHm, 0.008, 0.006, 0.004);
    }
    else
    {
        dD  = L_GetParam(dLmHm, 0.015, 0.012, 0.009);
        dLS = L_GetParam(dLmHm, 0.011, 0.009, 0.007);
    }

    double dCosTm = CMathFunc::mathCos(dThetam*CMathFunc::m_trrad);
    double dB = fabs(dKmi*dHm) < cEvalZero ?               0.0 : 0.7*dQy/(dKmi*dHm);
    double dC = fabs(dKms*dCosTm*dCosTm*dHm) < cEvalZero ? 0.0 : dQy/(dKms*dCosTm*dCosTm*dHm);

    if (dC > dD) { dC = dD; }  /// 조적조 압축 곡선 개선.

    rPhgt.dForceMomentRatio[IDX_DDY][IDX_CURV_ME] = -dD;
    rPhgt.dForceMomentRatio[IDX_DDY][IDX_CURV_MD] = -dD;
    rPhgt.dForceMomentRatio[IDX_DDY][IDX_CURV_MC] = -dC;
    rPhgt.dForceMomentRatio[IDX_DDY][IDX_CURV_MB] = -dB;
    rPhgt.dForceMomentRatio[IDX_DDY][IDX_CURV_A ] =  0.0;

    rPhgt.dForceMomentRatio[IDX_DDY][IDX_CURV_E ] = dD;
    rPhgt.dForceMomentRatio[IDX_DDY][IDX_CURV_D ] = dD;
    rPhgt.dForceMomentRatio[IDX_DDY][IDX_CURV_C ] = dC;
    rPhgt.dForceMomentRatio[IDX_DDY][IDX_CURV_B ] = dB;

    rPhgt.dForceMomentRatio[IDX_FFY][IDX_CURV_ME] =  0.0;
    rPhgt.dForceMomentRatio[IDX_FFY][IDX_CURV_MD] = rPhgt.nInfillStrutType==EN_MSIS_PARTIAL_HEIGHT ? -1.0 : 0.0;
    rPhgt.dForceMomentRatio[IDX_FFY][IDX_CURV_MC] = -1.0;
    rPhgt.dForceMomentRatio[IDX_FFY][IDX_CURV_MB] = -0.7;
    rPhgt.dForceMomentRatio[IDX_FFY][IDX_CURV_A ] =  0.0;

    rPhgt.dForceMomentRatio[IDX_FFY][IDX_CURV_E ] = -1.0*rPhgt.dForceMomentRatio[IDX_FFY][IDX_CURV_ME];
    rPhgt.dForceMomentRatio[IDX_FFY][IDX_CURV_D ] = -1.0*rPhgt.dForceMomentRatio[IDX_FFY][IDX_CURV_MD];
    rPhgt.dForceMomentRatio[IDX_FFY][IDX_CURV_C ] = -1.0*rPhgt.dForceMomentRatio[IDX_FFY][IDX_CURV_MC];
    rPhgt.dForceMomentRatio[IDX_FFY][IDX_CURV_B ] = -1.0*rPhgt.dForceMomentRatio[IDX_FFY][IDX_CURV_MB];

    rPhgt.dIO[IDX_PLS] = rPhgt.dIO[IDX_MNS] = dLS;  ///  원래는 n.a. 이나 입력창에서 보여지므로 LS와 같은 값으로 채움.
    rPhgt.dLS[IDX_PLS] = rPhgt.dLS[IDX_MNS] = dLS;
    rPhgt.dCP[IDX_PLS] = rPhgt.dCP[IDX_MNS] = dLS;  ///  원래는 n.a. 이나 입력창에서 보여지므로 LS와 같은 값으로 채움.
}

void CSeisEvalKISTEC2019::CalcMFactConColmDir(int nSeisCompType, OUT T_EVAL1ST_COLM_DIR& rEval)
{
    const T_CONCOLM_INFO& Info = rEval.Info;
    if ( !rEval.MfacM.bUserMfac )
    {
        /// [JIRA,GEN-1174] Column(M)의 휨성분은 일반 Column과 동일하게.
        const double dAxl = Info.dAxlIdx;
        const double dRho = Info.dRhoIdx;
        const double dShr = Info.dShrIdx;
        double admFac[3] = { 0.0, };
        switch (Info.unGroup)
        {
        case EN_CONCOLM_GROUP1: 
            {
                T_MEMBMFACT_TABLE_2 Table2;
                GetMFactTable_ConColm_Group1(nSeisCompType, Table2);
                CalcMFactTable2(Table2, dAxl, dRho, admFac);
            }                
            break;
        case EN_CONCOLM_GROUP2: 
            {
                T_MEMBMFACT_TABLE_3 Table3;
                GetMFactTable_ConColm_Group2(nSeisCompType, Table3);
                CalcMFactTable3(Table3, dAxl, dRho, dShr, admFac);
            }                
            break;
        case EN_CONCOLM_GROUP3: 
            {
                T_MEMBMFACT_TABLE_2 Table2;
                GetMFactTable_ConColm_Group3(nSeisCompType, Table2);
                CalcMFactTable2(Table2, dAxl, dRho, admFac);
            }                
            break;
        default: ASSERT(FALSE); break;
        }

        memcpy(rEval.MfacM.admFac, admFac, sizeof(double)*3);

        double dScaleFact = GetScaleUpFactor4MFact();
        MultiplyScaleFactorMFact(dScaleFact, rEval.MfacM.admFac);		
    }

    //SetFinalMe(Info.dMe, Info.CalcMeByShear(), rEval);
}

void CSeisEvalKISTEC2019::CalcMFactConWall(int nSeisCompType, const T_CONWALL_INFO& Info, OUT T_EVAL1ST_WALL_POS& rEval)
{    
    if ( Info.bForceCtrl ) 
    {
        for ( int i = EN_LEVEL_IO ; i <= EN_LEVEL_CP ; ++i )
        {
            rEval.MfacMV.admFac[i] = 1.0;
        }
        return; 
    }
    
    if ( !rEval.MfacMV.bUserMfac )
    {
        double admfac[3] = { 0.0, };
        if ( Info.enCtrlType == EN_EVALCTRL_SHER )
        {
            GetMFactTable_ConWall_CtrlSher(nSeisCompType, Info.dAxlIdx, admfac);
        }
        else
        {
            T_MEMBMFACT_TABLE_2 Table2;
            GetMFactTable_ConWall_CtrlFlex(nSeisCompType, Info.bBE, Table2);
            CalcMFactTable2(Table2, Info.dAxlIdx, Info.dShrIdx, admfac);
        }

        for ( int i = EN_LEVEL_IO ; i <= EN_LEVEL_CP ; ++i )
        {
            rEval.MfacMV.admFac[i] = admfac[i];
        }
    }

    double dScaleFact = GetScaleUpFactor4MFact();
    MultiplyScaleFactorMFact(dScaleFact, rEval.MfacMV.admFac);
}

EN_EVALCTRL CSeisEvalKISTEC2019::CalcWallCtrlType(double dMp, double dVn, double dHw, double dLw, const T_WALL_SHR_PARAM& Shr)
{
    /// see [ KISTEC 2019, 5.5.4(1) ]
    double dRatHwLw = fabs(dLw) < cEvalZero ? 3.0 + cEvalZero : (dHw / dLw);
    if ( dRatHwLw > 3.0 ) { return EN_EVALCTRL_FLEX; }
    if ( dRatHwLw < 1.5 ) { return EN_EVALCTRL_SHER; }
    
    return CSeisEval::CalcWallCtrlType(dMp, dVn, dHw, Shr);
}

#pragma region /// INFILL-STRUT

BOOL CSeisEvalKISTEC2019::CalcCsPreInfill(T_ELEM_K ElemK, OUT T_EVALPRE_INFL_DIR& rEval)
{
    rEval.Initialize();

    T_MSIS_D MsisD;
    if (!GetInfillParam(ElemK, MsisD)) { ASSERT(0); return FALSE; }

    T_MISA_D MisaD;
    if (!m_pDoc->m_pAttrCtrl2->GetMisa(ElemK, MisaD)) { ASSERT(0); return FALSE; }

    T_ELEM_D ElemD;
    if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); return FALSE; }
    T_SEMM_D SemmD;
    if (!m_pDoc->m_pAttrCtrl2->GetSemm_Calulated(ElemD.elmat, SemmD)) { ASSERT(0); return FALSE; }

    CSeisEvalDataTool Tool;
    const double dAopen = MsisD.dOpeningArea;
    const double dvsm = Tool.GetFvmMas(EN_EVALSTEP_PRE, EN_EVALMATL_NOM, SemmD);
    double dAsm = MsisD.dTm * MsisD.dLm;    
    if ( MsisD.nStrutType == EN_MSIS_FULL_HEIGHT && dAopen > 0.0 )
    {
        double dAtotl = MsisD.dHm * MsisD.dLm;        
        dAsm *= ( dAtotl - dAopen ) / dAtotl;
    }

    const double dFactor = ( MisaD.nBracingType == EN_DIAGONAL ) ? 1.0 : 0.5;

    rEval.dAsm = dAsm;
    rEval.dvsm = dvsm;
    rEval.dCs  = dvsm * dAsm * dFactor;

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::IsConsiderVbjs(T_ELEM_K ElemK)
{
    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SEMM_D SemmD;
    if ( !GetElemMatlSectMas(ElemK, SectD, MatdD, SemmD) ) { ASSERT(0); return FALSE; } 
    if ( SemmD.bUseDefaultVal) { return FALSE; }

    if ( SemmD.nMasonryCondition != D_SEMM_JOINT_GOOD) { return TRUE; }
    if ( SemmD.nMasonryJoint     != D_SEMM_JOINT_GOOD) { return TRUE; }

    T_MSIS_D MsisD;
    if (!GetInfillParam(ElemK, MsisD)) { ASSERT(FALSE); return TRUE; }

    if (MsisD.dOpeningArea > 0.0) { return TRUE; }

    /// 면외강도 전도에 안전.
    if (!IsOkOutPlane(MsisD, SemmD)) { return TRUE; }    

    return FALSE;
}

void CSeisEvalKISTEC2019::CalcInfillStrn(double dFmC, double dVm, double dPhi, int nBracingType, const T_MSIS_D& MsisD, BOOL bConsiderVbjs, OUT T_EVALLIN_INFILL_CAPA& rInfillCapa)
{
    rInfillCapa.Initialize();

    double dCI = fabs(MsisD.dHm) < cEvalZero ? cEvalZero : 1.925*MsisD.dLm/MsisD.dHm;

    rInfillCapa.dAn    = MsisD.dLm*MsisD.dTm;;  /// 채움벽 수평방향의 모르타르 접합 또는 그라우트 접합면의 순면적. 여기에서는 (채움벽체 길이)X(두께)로 계산합니다.
    rInfillCapa.dfvi   = dVm;
    rInfillCapa.dCI    = dCI;
    rInfillCapa.dfdt   = 0.125*sqrt(dFmC);      /// 이 아이는 N/mm2으로 계산해야 하니, 단위계 확인해 주세요.

    rInfillCapa.dVbjs  = rInfillCapa.dAn * rInfillCapa.dfvi;
    rInfillCapa.dVdc   = 0.818*MsisD.dLm*MsisD.dTm*rInfillCapa.dfdt/dCI*(1.0 + sqrt(dCI*dCI + 1.0));
    rInfillCapa.dVbm   = dFmC*MsisD.dHm*MsisD.dTm/3.0;
    rInfillCapa.dVn    = rInfillCapa.CalcVn(bConsiderVbjs);		
    rInfillCapa.dPhi   = dPhi;

    double dFact = 0.0;
    switch (nBracingType)
    {
    case EN_DIAGONAL: dFact = 1.0; break;
    case EN_X:        dFact = 0.5; break;
    default: ASSERT(FALSE);	break;
    }

    double dCosThetam  = MsisD.dLm/sqrt(MsisD.dHm*MsisD.dHm + MsisD.dLm*MsisD.dLm);
    rInfillCapa.dPhiPn = dFact*dPhi*rInfillCapa.dVn/dCosThetam;	
}

BOOL CSeisEvalKISTEC2019::IsOkOutPlane(const T_MSIS_D& MsisD, const T_SEMM_D& SemmD)
{
    T_SEME_D SemeD;
    if (!m_pDoc->m_pAttrCtrl2->GetSeme(SemeD))  { SemeD.Initialize(); }
    int nPerformLevel = SemeD.data.eval.nPerformLevel;

    int nMasonryJoint = SemmD.nMasonryJoint;
    double dhm = MsisD.dHm;
    double dtm = MsisD.dTm;
    double dhmtm    = fabs(dtm) < cEvalZero ? 0.0 : dhm/dtm;
    double dLambda2 = CalcLambda2(dhm, dtm);

    CSeisEvalDataTool Tool;
    /// 면외 방향 전도는 예비평가 데이터로 고려합니다.
    double dfmc = Tool.GetFmcMas(EN_EVALSTEP_PRE, EN_EVALMATL_NOM, SemmD);
    double dfmt = Tool.GetFmtMas(EN_EVALSTEP_PRE, EN_EVALMATL_NOM, SemmD);

    double dQtn = 0.0;    
    if ( IsArchingAction(nMasonryJoint, dhmtm) )
    {
        dQtn = CalcQtnArchingAction(dhm, dtm, dfmc);
    }
    else
    {
        dQtn = CalcQtnCantilever(dhm, dtm, dfmt);
    }

    CArray<T_SEIS_ITEM, T_SEIS_ITEM&> aSeisItem;
    T_SEIS_ITEM CurSeisItem;

    T_KEY_LIST aSeisK;
    m_pDoc->m_pAttrCtrl->GetSeisKeyList(aSeisK);

    T_SEIS_D SeisD;
    int nSeis = static_cast<int>(aSeisK.GetSize());
    for (int i=0; i<nSeis; ++i)
    {
        if (!m_pDoc->m_pAttrCtrl->GetSeis(aSeisK[i], SeisD)) continue;                
        if (!GetSeisParam(SeisD, CurSeisItem)) continue;
        aSeisItem.Add(CurSeisItem);
    }

    T_KEY_LIST aSplcK;
    m_pDoc->m_pAttrCtrl->GetSplcKeyList(aSplcK);

    T_SPLC_D SplcD;
    T_SPFC_D SpfcD;
    int nSplc = static_cast<int>(aSplcK.GetSize());
    for (int i=0; i<nSplc; ++i)
    {
        if (!m_pDoc->m_pAttrCtrl->GetSplc(aSplcK[i], SplcD)) continue;

        int nSpfc = static_cast<int>(SplcD.aSpfcKey.GetSize());
        for (int m=0; m<nSpfc; ++m)
        {
            if (!m_pDoc->m_pAttrCtrl->GetSpfc(SplcD.aSpfcKey[m], SpfcD)) continue;
            if (!GetSpfcParam(SpfcD, CurSeisItem)) continue;
            aSeisItem.Add(CurSeisItem);
        }
    }

    int nItem = static_cast<int>(aSeisItem.GetSize());
    if(nItem<0) return FALSE;
    for (int i=0; i<nItem; ++i)
    {
        double dSDS = aSeisItem[i].dSDS;
        double dSD1 = aSeisItem[i].dSD1;
        double dIe  = aSeisItem[i].dIe;
        if (!IsNeedCheckOutPlane(nMasonryJoint, dhm, dtm, nPerformLevel, dSDS, dSD1)) continue;

        double dFP = CalcFp(nPerformLevel, dtm, dSDS, dIe);
        if (dFP > dQtn) { return FALSE; }
    }

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::IsNeedCheckOutPlane(int nMasonryJoint, double dhm, double dtm, int nPerformLevel, double dSDS, double dSD1)
{
    if (nMasonryJoint != D_SEMM_JOINT_GOOD) { return TRUE; }
    if (dhm > 3300.0) { return TRUE; }
    if (dtm >  190.0) { return TRUE; }

    double dhmtm = fabs(dtm) < cEvalZero ? 0.0 : dhm/dtm;
    if ( !IsArchingAction(nMasonryJoint, dhmtm) ) { return TRUE; }

    double dhmtmLimit1 = CalcHmTmLimitTable6_1_7(nPerformLevel, dSDS, 0.330, 0.50);
    double dhmtmLimit2 = CalcHmTmLimitTable6_1_7(nPerformLevel, dSD1, 0.133, 0.20);
    double dhmtmLimit  = min(dhmtmLimit1, dhmtmLimit2);

    if (dhmtm > dhmtmLimit || fabs(dhmtm - dhmtmLimit) > cEvalZero) { return TRUE; }

    return FALSE;
}

BOOL CSeisEvalKISTEC2019::IsArchingAction(int nMasonryJoint, double dhmtm)
{
    if (nMasonryJoint != D_SEMM_JOINT_GOOD) { return FALSE; }
    if (dhmtm > 25.0) { return FALSE; }    
    return TRUE;
}

double CSeisEvalKISTEC2019::CalcHmTmLimitTable6_1_7(int nPerformLevel, double dVal, double dMin, double dMax)
{
    double dLimit = 0.0;
    if (dVal < dMin)
    {
        switch (nPerformLevel)
        {
        case D_SEME_PERFORM_IO: dLimit = 14.0; break;
        case D_SEME_PERFORM_LS: dLimit = 15.0; break;
        case D_SEME_PERFORM_CP: dLimit = 16.0; break;
        default: ASSERT(FALSE); break;
        }
    }
    else if (dVal < dMax)
    {
        switch (nPerformLevel)
        {
        case D_SEME_PERFORM_IO: dLimit = 13.0; break;
        case D_SEME_PERFORM_LS: dLimit = 14.0; break;
        case D_SEME_PERFORM_CP: dLimit = 15.0; break;
        default: ASSERT(FALSE); break;
        }
    }
    else
    {
        switch (nPerformLevel)
        {
        case D_SEME_PERFORM_IO: dLimit =  8.0; break;
        case D_SEME_PERFORM_LS: dLimit =  9.0; break;
        case D_SEME_PERFORM_CP: dLimit = 10.0; break;
        default: ASSERT(FALSE); break;
        }
    }
    return dLimit;
}

double CSeisEvalKISTEC2019::CalcQtnArchingAction(double dhm, double dtm, double dfmc)
{
    double dhmtm = fabs(dtm) < cEvalZero ? 0.0 : dhm/dtm;
    double dLambda2 = CalcLambda2(dhm, dtm);
    return fabs(dhmtm) < cEvalZero ? 0.0 : 0.7*dfmc*dLambda2/dhmtm;
}

double CSeisEvalKISTEC2019::CalcQtnCantilever(double dhm, double dtm, double dfmt)
{
    return dfmt*dtm*dtm/6.0*(dhm*dhm/2.0);
}

double CSeisEvalKISTEC2019::CalcFp(double nPerformLevel, double dtm, double dSDS, double dIe)
{
    double dParam = max(0.4*dSDS*dIe, 0.1);
    double dXi    = GetXi(nPerformLevel);
    double dWp    = m_pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_DENSITY, 20.0);
    return dParam*dXi*dWp;        
}

double CSeisEvalKISTEC2019::CalcLambda2(double dhm, double dtm)
{
    double dhmtm = fabs(dtm) < cEvalZero ? 0.0 : dhm/dtm;

    if (dhmtm <  5.0+cEvalZero) { return 0.129; }
    if (dhmtm > 25.0+cEvalZero) { return 0.013; }

    double adhmtm[4] = { 5.0,  10.0,  15.0,  25.0 };
    double adLam2[4] = { 0.129, 0.060, 0.034, 0.013};

    double dLambda2 = 0.0;
    for (int i=0; i<3; ++i)
    {
        if (dhmtm < adhmtm[i+1]+cEvalZero)
        {
            dLambda2 = CMathFunc::mathInterpolate(adLam2[i], adLam2[i+1], dhmtm-adhmtm[i]);
        }
    }    
    return dLambda2;
}

double CSeisEvalKISTEC2019::GetXi(int nPerformLevel)
{
    double dXi = 0.0;
    switch (nPerformLevel)
    {
    case D_SEME_PERFORM_IO: dXi = 1.0; break;
    case D_SEME_PERFORM_LS: dXi = 1.3; break;
    case D_SEME_PERFORM_CP: dXi = 2.0; break;
    default: ASSERT(FALSE); break;
    }
    return dXi;
}


BOOL CSeisEvalKISTEC2019::MakeVframe1stColm(T_ELEM_K ElemK, BOOL bNodeITop, const T_STRB_D& StrbD, OUT T_VFRAME& rVframe)
{
    rVframe.Initialize();
    rVframe.ElemK = ElemK;

    const int nPos = !bNodeITop ? 0 : 3;
    const int nDof = !bNodeITop ? 0 : 6;

    const double dPu  = StrbD.dblForce[nPos][nDof] * (-1.0);
    const double dVuy = StrbD.dblForce[nPos][nDof+1];
    const double dVuz = StrbD.dblForce[nPos][nDof+2];
    const double dMuy = StrbD.dblForce[nPos][nDof+4];
    const double dMuz = StrbD.dblForce[nPos][nDof+5];

    T_COLM_SHR_PARAM aShry[EN_MEMBEND_NUM];
    if (!MakeColmShearParamEval(ElemK, EN_EVALSTEP_1ST, EN_DIR_Y, aShry)) { ASSERT(0); return FALSE; }
    T_COLM_SHR_PARAM aShrz[EN_MEMBEND_NUM];
    if (!MakeColmShearParamEval(ElemK, EN_EVALSTEP_1ST, EN_DIR_Z, aShrz)) { ASSERT(0); return FALSE; }

    const EN_MEMBEND enCurEnd = bNodeITop ? EN_MEMBEND_I : EN_MEMBEND_J;  /// Top의 부재력으로 계산한 강도가 필요합니다.
    rVframe.dVny = CalcVnConColm(aShry[enCurEnd], dPu, dVuy, dMuz);
    rVframe.dVnz = CalcVnConColm(aShrz[enCurEnd], dPu, dVuz, dMuy);

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::MakeVframe2ndColm(T_ELEM_K ElemK, BOOL bNodeITop, const T_SEIS_PO_ANAL_MAP& mPORslt, OUT T_VFRAME& rVframe)
{
    rVframe.Initialize();
    if (!m_pDoc->m_pAttrCtrl->ExistElem(ElemK)) { return FALSE; }

    rVframe.ElemK = ElemK;

    T_SEIS_PO_ANAL_RESULT_D PORslt;
    if (!mPORslt.Lookup(ElemK, PORslt)) { ASSERT(0); return FALSE; }

    const int nPos = !bNodeITop ? 0 : 1;    
    const double dPu  = PORslt.dForce[nPos][DOF_FX] * (-1.0);
    const double dVuy = PORslt.dForce[nPos][DOF_FY];
    const double dVuz = PORslt.dForce[nPos][DOF_FZ];
    const double dMuy = PORslt.dForce[nPos][DOF_MY];
    const double dMuz = PORslt.dForce[nPos][DOF_MZ];

    T_COLM_SHR_PARAM aShry[EN_MEMBEND_NUM];
    if (!MakeColmShearParamEval(ElemK, EN_EVALSTEP_2ND, EN_DIR_Y, aShry)) { ASSERT(0); return FALSE; }
    T_COLM_SHR_PARAM aShrz[EN_MEMBEND_NUM];
    if (!MakeColmShearParamEval(ElemK, EN_EVALSTEP_2ND, EN_DIR_Z, aShrz)) { ASSERT(0); return FALSE; }

    const EN_MEMBEND enCurEnd = bNodeITop ? EN_MEMBEND_I : EN_MEMBEND_J;  /// Top의 부재력으로 계산한 강도가 필요합니다.
    rVframe.dVny = CalcVnConColm(aShry[enCurEnd], dPu, dVuy, dMuz);
    rVframe.dVnz = CalcVnConColm(aShrz[enCurEnd], dPu, dVuz, dMuy);

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::MakeVframe1stWall(T_ELEM_K ElemK, const T_STRW_D& StrwD, OUT T_VFRAME& rVframe)
{
    rVframe.Initialize();
        
    T_WALL_SHR_PARAM aShrAvg[EN_MEMBEND_NUM];
    T_WALL_SHR_PARAM aShrNom[EN_MEMBEND_NUM];
    MakeWallShearParam(ElemK, EN_EVALSTEP_1ST, EN_DIR_Z, aShrAvg, aShrNom);

    SetWallForcShearParam(StrwD.dblForce, EN_DIR_Z, aShrAvg, aShrNom);

    // 현재 반영되어 있는 것과 같이 변형지배 거동인 경우의 전단강도 적용.
    rVframe.ElemK = ElemK;
    rVframe.dVny = 0.0;  //// 벽체의 약축 전단은 계산하지 않습니다.
    rVframe.dVnz = CalcVnConWall(aShrAvg[EN_MEMBEND_J]); /// Top의 부재력으로 계산한 강도가 필요합니다.

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::MakeVframe2ndWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_MAP& mPORslt, OUT T_VFRAME& rVframe)
{
    rVframe.Initialize();
    if (!m_pDoc->m_pAttrCtrl->ExistElem(ElemK)) { return FALSE; }

    T_SEIS_PO_ANAL_RESULT_D PORslt;
    if (!mPORslt.Lookup(ElemK, PORslt)) { ASSERT(0); return FALSE; }

    T_WALL_SHR_PARAM aShrAvg[EN_MEMBEND_NUM];
    T_WALL_SHR_PARAM aShrNom[EN_MEMBEND_NUM];
    MakeWallShearParam(ElemK, EN_EVALSTEP_2ND, EN_DIR_Z, aShrAvg, aShrNom);
    SetWallForcShearParam(PORslt.dForce, EN_DIR_Z, aShrAvg, aShrNom);

    // 현재 반영되어 있는 것과 같이 변형지배 거동인 경우의 전단강도 적용.
    rVframe.ElemK = ElemK;
    rVframe.dVny = 0.0;  //// 벽체의 약축 전단은 계산하지 않습니다.
    rVframe.dVnz = CalcVnConWall(aShrAvg[EN_MEMBEND_J]); /// Top의 부재력으로 계산한 강도가 필요합니다.

    return TRUE;
}

void CSeisEvalKISTEC2019::CalcEval1stInfillPos(T_ELEM_K InfillK, double dBeta, double dLmHm, OUT T_EVAL1ST_INFILL_POS& rEvalPos)
{
    if (!rEvalPos.bUserMfac)
    {		
        CalcMFactInfill(dBeta, dLmHm, rEvalPos.dmFac);
        double dScaleFact = GetScaleUpFactor4MFact();
        MultiplyScaleFactorMFact(dScaleFact, rEvalPos.dmFac);		
    }

    for ( int i=EN_LEVEL_IO; i<=EN_LEVEL_CP; ++i )
    {
        rEvalPos.DCR.dDCR[i] = CalcDCRDisp(rEvalPos.dPd, rEvalPos.dPc, rEvalPos.dmFac[i]);
    }

    SetPerformLevel(rEvalPos.DCR);
}

void CSeisEvalKISTEC2019::CalcMFactInfill(double dBeta, double dLmHm, OUT double radMFac[3])
{
    memset(radMFac, 1.0, sizeof(double)*3);

    if (dBeta < 0.7)
    {
        double ad05TBL[2] = { 1.0, 4.0 };
        double ad10TBL[2] = { 1.0, 3.5 };
        double ad50TBL[2] = { 1.0, 3.0 };
        CalcMFactInfill(dLmHm, ad05TBL, ad10TBL, ad50TBL, radMFac);
    }
    else if (dBeta < 1.3)
    {
        double ad05TBL[2] = { 1.5, 6.0 };
        double ad10TBL[2] = { 1.2, 5.2 };
        double ad50TBL[2] = { 1.0, 4.5 };
        CalcMFactInfill(dLmHm, ad05TBL, ad10TBL, ad50TBL, radMFac);
    }
    else
    {
        double ad05TBL[2] = { 1.5, 8.0 };
        double ad10TBL[2] = { 1.2, 7.0 };
        double ad50TBL[2] = { 1.0, 6.0 };
        CalcMFactInfill(dLmHm, ad05TBL, ad10TBL, ad50TBL, radMFac);
    }
}

void CSeisEvalKISTEC2019::CalcMFactInfill(double dLmHm, double ad05TBL[2], double ad10TBL[2], double ad20TBL[2], OUT double radMFac[3])
{
    memset(radMFac, 1.0, sizeof(double)*3);

    if (dLmHm < 0.5)
    {
        radMFac[EN_LEVEL_IO] = ad05TBL[EN_LEVEL_IO];
        radMFac[EN_LEVEL_LS] = ad05TBL[EN_LEVEL_LS];
    }
    else if (dLmHm < 1.0)
    {
        radMFac[EN_LEVEL_IO] = CMathFunc::mathInterpolate(ad05TBL[EN_LEVEL_IO], ad10TBL[EN_LEVEL_IO], (dLmHm-0.5)/0.5);
        radMFac[EN_LEVEL_LS] = CMathFunc::mathInterpolate(ad05TBL[EN_LEVEL_LS], ad10TBL[EN_LEVEL_LS], (dLmHm-0.5)/0.5);
    }
    else if (dLmHm < 2.0)
    {
        radMFac[EN_LEVEL_IO] = CMathFunc::mathInterpolate(ad10TBL[EN_LEVEL_IO], ad20TBL[EN_LEVEL_IO], dLmHm-1.0);
        radMFac[EN_LEVEL_LS] = CMathFunc::mathInterpolate(ad10TBL[EN_LEVEL_LS], ad20TBL[EN_LEVEL_LS], dLmHm-1.0);
    }
    else
    {
        radMFac[EN_LEVEL_IO] = ad20TBL[EN_LEVEL_IO];
        radMFac[EN_LEVEL_LS] = ad20TBL[EN_LEVEL_LS];
    }
    radMFac[EN_LEVEL_CP] = radMFac[EN_LEVEL_LS]; // N.A. 우선 LS와 같은 값으로 부여.
}

#pragma endregion

double CSeisEvalKISTEC2019::CalcVnConColm(const T_COLM_SHR_PARAM& Shr, double dPu, double dVu, double dMu)
{
    const double dfck    = Shr.Comm.dFck;
    const double dbw     = Shr.Comm.dBv;
    const double dd      = Shr.Comm.dDv;
    const double dLambda = Shr.Comm.dLambda;
    const double dAg     = Shr.dAg;

    double dVc = 0.0;
    switch (Shr.nShrMethod)
    {
    case D_SEME_SHRSTR_MOE2018:
        {
            double dMuVud = CalcMuPerVud(dMu, dVu, dd);
            double dAppPu = dPu < 0.0 ? 0.0 : fabs(dPu); /// 인장력이 작용하는 경우에는 0.0으로 본다.

            dVc = dLambda*(0.5*sqrt(dfck)/(dMuVud)*sqrt(1.0 + dAppPu/(0.5*sqrt(dfck)*dAg)))*0.8*dAg;  /// (5-5)
        }
        break;
    case D_SEME_SHRSTR_STANDARD:
        {
            if (fabs(dPu) < cEvalZero)
            {
                dVc = 1.0/6.0*dLambda*sqrt(dfck)*dbw*dd;
            }
            else if (dPu > 0.0)  /// in case of compression.
            {
                dVc = 1.0/6.0*(1.0 + dPu/(14.0*dAg))*dLambda*sqrt(dfck)*dbw*dd;
            }
            else /// in case of tension.
            {
                dVc = 1.0/6.0*(1.0 + dPu/(3.5*dAg))*dLambda*sqrt(dfck)*dbw*dd;
                if (dVc < 0.0) { dVc = 0.0; }
            } 
        }
        break;
    default: ASSERT(FALSE);
        break;
    }
    
    double dVs = CalcVsFram(Shr.Comm);
    double dK1 = CalcK1(Shr);
    return dVc + dK1*dVs;
}

double CSeisEvalKISTEC2019::CalcMuPerVud(double dMu, double dVu, double dd)
{
    double dMuPerVud = fabs(dVu*dd) < cEvalZero? 3.0 : fabs(dMu/(dVu*dd)); /// 강도계산의 간편성을 위하여 M/(Vd)=3을 사용할 수 있음.
    return min(max(2.0, dMuPerVud), 4.0);  /// 5.4.3.2
}

UINT CSeisEvalKISTEC2019::GetGroupConColm(UINT unSeisDetail, double dVp, double dVn, BOOL bConditionGroup1)
{
    double dRatVpVn = fabs(dVn) < cEvalZero ? 1.0 + cEvalZero : fabs(dVp/dVn);

    UINT unGroup = EN_CONCOLM_GROUP3;
    switch (unSeisDetail)
    {
    case D_SEME_REINF_135:
        {
            if      (dRatVpVn > 1.0) { unGroup = EN_CONCOLM_GROUP3; }
            else if (dRatVpVn > 0.6) { unGroup = EN_CONCOLM_GROUP2; }
            else                     { unGroup = bConditionGroup1 ? EN_CONCOLM_GROUP1 : EN_CONCOLM_GROUP2; }
        }
        break;
    case D_SEME_REINF_90:
        {
            unGroup = dRatVpVn > 1.0 ? EN_CONCOLM_GROUP3 : EN_CONCOLM_GROUP2;
        }
        break;
    case D_SEME_REINF_OTHER:
        {
            unGroup = dRatVpVn > 0.6 ? EN_CONCOLM_GROUP3 : EN_CONCOLM_GROUP2;
        }
        break; 
    default: ASSERT(FALSE);
        break;
    }
    return unGroup;
}

BOOL CSeisEvalKISTEC2019::IsConditionGroup1(const T_CON_SHR_PARAM& Shr)
{
    const double dBv = Shr.dBv;
    const double dSv = Shr.dSv;
    const double ddv = Shr.dDv;
    const double dAsv = Shr.dAsv;

    if (dBv*dSv*ddv == 0.0)     { return FALSE; }
    if (dAsv/(dBv*dSv) < 0.002) { return FALSE; }
    if (dSv/ddv > 0.5)          { return FALSE; }
    return TRUE;
}

void CSeisEvalKISTEC2019::GetParamTable_ConBeam_CtrlFlex(BOOL bSeisConform, OUT T_MEMBPARAM_TABLE_2& rTable)
{
    rTable.Initialize();

    /// rho index;
    rTable.dA1 = 0.0;
    rTable.dA2 = 0.5;
    /// shear index;
    rTable.dB1 = 0.25;
    rTable.dB2 = 0.5;

    /// see [KISTEC 2019, table 5.4.6], { a, b, c, IO, LS, CP }
    const double aA1B1[2][6] = { { 0.025, 0.05, 0.2, 0.010, 0.025, 0.05 }, { 0.02 , 0.03 , 0.2, 0.005 , 0.02 , 0.03  } };
    const double aA1B2[2][6] = { { 0.02 , 0.04, 0.2, 0.005, 0.02 , 0.04 }, { 0.01 , 0.015, 0.2, 0.0015, 0.01,  0.015 } };
    const double aA2B1[2][6] = { { 0.02 , 0.03, 0.2, 0.005, 0.02 , 0.03 }, { 0.01 , 0.015, 0.2, 0.005 , 0.01 , 0.015 } };
    const double aA2B2[2][6] = { { 0.015, 0.02, 0.2, 0.005, 0.015, 0.02 }, { 0.005, 0.01 , 0.2, 0.0015, 0.005, 0.01  } };

    int nIdx1 = bSeisConform ? 0 : 1;
    for ( int i=0; i<6; ++i )
    {
        rTable.adTable_A1_B1[i] = aA1B1[nIdx1][i];
        rTable.adTable_A1_B2[i] = aA1B2[nIdx1][i];
        rTable.adTable_A2_B1[i] = aA2B1[nIdx1][i];
        rTable.adTable_A2_B2[i] = aA2B2[nIdx1][i];
    }
}

void CSeisEvalKISTEC2019::GetMFactTable_ConBeam_CtrlFlex(int nSeisCompType, BOOL bSeisConform, OUT T_MEMBMFACT_TABLE_2& rTable)
{
    rTable.Initialize();

    /// rho index;
    rTable.dA1 = 0.0;
    rTable.dA2 = 0.5;
    /// shear index;
    rTable.dB1 = 0.25;
    rTable.dB2 = 0.5;

    /// see [KISTEC 2019, table 5.4.3], { m-IO, m-LS(1st), m-CP(1st), m-LS(2nd), m-CP(2nd) } 
    const double aA1B1[2][5] = { { 3.0, 6.0, 7.0, 6.0, 10.0 }, { 2.0 , 3.0, 4.0, 3.0, 5.0 } };
    const double aA1B2[2][5] = { { 2.0, 3.0, 4.0, 3.0,  5.0 }, { 1.25, 2.0, 3.0, 2.0, 4.0 } };
    const double aA2B1[2][5] = { { 2.0, 3.0, 4.0, 3.0,  5.0 }, { 2.0 , 3.0, 3.0, 3.0, 4.0 } };
    const double aA2B2[2][5] = { { 2.0, 2.0, 3.0, 2.0,  4.0 }, { 1.25, 2.0, 2.0, 2.0, 3.0 } };

    int nIdx1 = bSeisConform ? 0 : 1;
    for ( int i=0; i<3; ++i )
    {
        int nIdx2 = GetColIndexMFactTable(nSeisCompType, i);

        rTable.adTable_A1_B1[i] = aA1B1[nIdx1][nIdx2];
        rTable.adTable_A1_B2[i] = aA1B2[nIdx1][nIdx2];
        rTable.adTable_A2_B1[i] = aA2B1[nIdx1][nIdx2];
        rTable.adTable_A2_B2[i] = aA2B2[nIdx1][nIdx2];
    }
}

void CSeisEvalKISTEC2019::GetParamTable_ConBeam_CtrlSher(BOOL bSpaceGTd2, OUT double radParam[6])
{
    memset(radParam, 0.0, sizeof(double)*6);

    const double adParamLEd2[6] = { 0.003, 0.02, 0.2, 0.0015, 0.01 , 0.02 };
    const double adParamGTd2[6] = { 0.003, 0.01, 0.2, 0.0015, 0.005, 0.01 };

    memcpy(radParam, ( bSpaceGTd2 ? adParamGTd2 : adParamLEd2 ), sizeof(double)*6);    
}

void CSeisEvalKISTEC2019::GetMFactTable_ConBeam_CtrlSher(int nSeisCompType, BOOL bSpaceGTd2, OUT double raMFact[3])
{
    memset(raMFact, 0.0, sizeof(double)*3);

    /// see [KISTEC 2019, table 5.4.3], { m-IO, m-LS(1st), m-CP(1st), m-LS(2nd), m-CP(2nd) } 
    const double adMFact[2][5] = { { 1.25, 1.5, 1.75, 3.0, 4.0 }, { 1.25, 1.5, 1.75, 2.0, 3.0 } };

    int nIdx1 = bSpaceGTd2 ? 1 : 0;
    for ( int i=0; i<3; ++i )
    {
        int nIdx2 = GetColIndexMFactTable(nSeisCompType, i);
        raMFact[i] = adMFact[nIdx1][nIdx2];
    } 
}

void CSeisEvalKISTEC2019::GetParamTable_ConColm_Group1(OUT T_MEMBPARAM_TABLE_2& rTable) /// group i : 휨 파괴.
{
    rTable.Initialize();

    /// axial index = P / (Ag*fck)
    rTable.dA1 = 0.1;
    rTable.dA2 = 0.6;
    /// rho index = Av / (bw*s)
    rTable.dB1 = 0.002;
    rTable.dB2 = 0.006;

    /// see [KISTEC 2019, table 5.4.7],  { a, b, c, IO, LS, CP }
    const double aA1B2[6] = { 0.035, 0.060, 0.2, 0.005, 0.045, 0.060 }; /// P < 0.1, Rho > 0.006
    const double aA2B2[6] = { 0.010, 0.010, 0.0, 0.003, 0.009, 0.010 }; /// P > 0.6, Rho > 0.006
    const double aA1B1[6] = { 0.027, 0.034, 0.2, 0.005, 0.027, 0.034 }; /// P < 0.1, Rho = 0.002
    const double aA2B1[6] = { 0.005, 0.005, 0.0, 0.002, 0.004, 0.005 }; /// P > 0.6, Rho = 0.002
        
    for ( int i=0; i<6; ++i )
    {
        rTable.adTable_A1_B1[i] = aA1B1[i];
        rTable.adTable_A1_B2[i] = aA1B2[i];
        rTable.adTable_A2_B1[i] = aA2B1[i];
        rTable.adTable_A2_B2[i] = aA2B2[i];
    }
}

void CSeisEvalKISTEC2019::GetMFactTable_ConColm_Group1(int nSeisCompType, OUT T_MEMBMFACT_TABLE_2& rTable)
{
    rTable.Initialize();

    /// axial index = P / (Ag*fck)
    rTable.dA1 = 0.1;
    rTable.dA2 = 0.6;
    /// rho index = Av / (bw*s)
    rTable.dB1 = 0.002;
    rTable.dB2 = 0.006;

    /// see [KISTEC 2019, table 5.4.4],  { m-IO, m-LS(1st), m-CP(1st), m-LS(2nd), m-CP(2nd) }
    const double aA1B2[5] = { 2.0 , 2.5, 3.0, 4.0, 5.0 }; /// P < 0.1, Rho > 0.006
    const double aA2B2[5] = { 1.25, 1.8, 1.9, 1.9, 2.0 }; /// P > 0.6, Rho > 0.006
    const double aA1B1[5] = { 2.0 , 2.0, 2.6, 2.6, 3.0 }; /// P < 0.1, Rho < 0.002
    const double aA2B1[5] = { 1.1 , 1.1, 1.2, 1.2, 1.4 }; /// P > 0.6, Rho < 0.002

    for ( int i=0; i<3; ++i )
    {
        int nIdx = GetColIndexMFactTable(nSeisCompType, i);
        rTable.adTable_A1_B1[i] = aA1B1[nIdx];
        rTable.adTable_A1_B2[i] = aA1B2[nIdx];
        rTable.adTable_A2_B1[i] = aA2B1[nIdx];
        rTable.adTable_A2_B2[i] = aA2B2[nIdx];
    }
}

void CSeisEvalKISTEC2019::GetParamTable_ConColm_Group2(OUT T_MEMBPARAM_TABLE_3& rTable)  /// group ii : 휨 항복 후, 전단 파괴.
{
    rTable.Initialize();

    /// axial index = P / (Ag*fck)
    rTable.dA1 = 0.1;
    rTable.dA2 = 0.6;
    /// rho index = Av / (bw*s)
    rTable.dB1 = 0.0005;
    rTable.dB2 = 0.006;
    /// shear index = V / (bw*d*sqrt(fck))
    rTable.dC1 = 0.25;
    rTable.dC2 = 0.5;

    /// see [KISTEC 2019, table 5.4.7],  { a, b, c, IO, LS, CP }
    const double aA1B2C1[6] = { 0.032, 0.060, 0.2, 0.005, 0.045, 0.060 }; /// P < 0.1, Rho > 0.006, V < 0.25
    const double aA1B2C2[6] = { 0.025, 0.060, 0.2, 0.005, 0.045, 0.060 }; /// P < 0.1, Rho > 0.006, V > 0.5
    const double aA2B2C1[6] = { 0.010, 0.010, 0.0, 0.003, 0.009, 0.010 }; /// P > 0.6, Rho > 0.006, V < 0.25
    const double aA2B2C2[6] = { 0.008, 0.008, 0.0, 0.003, 0.007, 0.008 }; /// P > 0.6, Rho > 0.006, V > 0.5
    const double aA1B1C1[6] = { 0.012, 0.012, 0.2, 0.005, 0.010, 0.012 }; /// P < 0.1, Rho < 0.0005, V < 0.25
    const double aA1B1C2[6] = { 0.006, 0.006, 0.2, 0.004, 0.005, 0.006 }; /// P < 0.1, Rho < 0.0005, V > 0.5
    const double aA2B1C1[6] = { 0.004, 0.004, 0.0, 0.002, 0.003, 0.004 }; /// P > 0.6, Rho < 0.0005, V < 0.25
    const double aA2B1C2[6] = { 0.0  , 0.0  , 0.0, 0.0  , 0.0  , 0.0   }; /// P > 0.6, Rho < 0.0005, V > 0.5

    for ( int i=0; i<6; ++i )
    {
        rTable.adTable_A1_B1_C1[i] = aA1B1C1[i];
        rTable.adTable_A1_B1_C2[i] = aA1B1C2[i];
        rTable.adTable_A1_B2_C1[i] = aA1B2C1[i];
        rTable.adTable_A1_B2_C2[i] = aA1B2C2[i];
        rTable.adTable_A2_B1_C1[i] = aA2B1C1[i];
        rTable.adTable_A2_B1_C2[i] = aA2B1C2[i];
        rTable.adTable_A2_B2_C1[i] = aA2B2C1[i];
        rTable.adTable_A2_B2_C2[i] = aA2B2C2[i];
    }
}

void CSeisEvalKISTEC2019::GetMFactTable_ConColm_Group2(int nSeisCompType, OUT T_MEMBMFACT_TABLE_3& rTable)
{
    rTable.Initialize();

    /// axial index = P / (Ag*fck)
    rTable.dA1 = 0.1;
    rTable.dA2 = 0.6;
    /// rho index = Av / (bw*s)
    rTable.dB1 = 0.0005;
    rTable.dB2 = 0.006;
    /// shear index = V / (bw*d*sqrt(fck))
    rTable.dC1 = 0.25;
    rTable.dC2 = 0.5;

    /// see [KISTEC 2019, table 5.4.4],  { m-IO, m-LS(1st), m-CP(1st), m-LS(2nd), m-CP(2nd) }
    const double aA1B2C1[5] = { 2.0 , 2.5, 3.0, 4.0, 5.0 }; /// P < 0.1, Rho > 0.006
    const double aA1B2C2[5] = { 2.0 , 2.0, 2.5, 4.0, 5.0 }; /// P < 0.1, Rho > 0.006
    const double aA2B2C1[5] = { 1.25, 1.8, 1.9, 1.9, 2.0 }; /// P > 0.6, Rho > 0.006
    const double aA2B2C2[5] = { 1.25, 1.5, 1.6, 1.6, 1.8 }; /// P > 0.6, Rho > 0.006
    const double aA1B1C1[5] = { 1.2 , 1.3, 1.4, 1.4, 1.6 }; /// P < 0.1, Rho = 0.0005
    const double aA1B1C2[5] = { 1.0 , 1.0, 1.1, 1.1, 1.2 }; /// P < 0.1, Rho = 0.0005
    const double aA2B1C1[5] = { 1.0 , 1.0, 1.1, 1.1, 1.2 }; /// P > 0.6, Rho = 0.0005
    const double aA2B1C2[5] = { 1.0 , 1.0, 1.0, 1.0, 1.0 }; /// P > 0.6, Rho = 0.0005

    for ( int i=0; i<3; ++i )
    {
        int nIdx = GetColIndexMFactTable(nSeisCompType, i);
        rTable.adTable_A1_B1_C1[i] = aA1B1C1[nIdx];
        rTable.adTable_A1_B1_C2[i] = aA1B1C2[nIdx];
        rTable.adTable_A1_B2_C1[i] = aA1B2C1[nIdx];
        rTable.adTable_A1_B2_C2[i] = aA1B2C2[nIdx];
        rTable.adTable_A2_B1_C1[i] = aA2B1C1[nIdx];
        rTable.adTable_A2_B1_C2[i] = aA2B1C2[nIdx];
        rTable.adTable_A2_B2_C1[i] = aA2B2C1[nIdx];
        rTable.adTable_A2_B2_C2[i] = aA2B2C2[nIdx];
    }
}

void CSeisEvalKISTEC2019::GetParamTable_ConColm_Group3(OUT T_MEMBPARAM_TABLE_2& rTable)  /// group iii : 전단 파괴.
{
    rTable.Initialize();

    /// axial index = P / (Ag*fck)
    rTable.dA1 = 0.1;
    rTable.dA2 = 0.6;
    /// rho index = Av / (bw*s)
    rTable.dB1 = 0.0005;
    rTable.dB2 = 0.006;

    /// see [KISTEC 2019, table 5.4.7],  { a, b, c, IO, LS, CP }
    const double aA1B2[6] = { 0.0, 0.060, 0.0, 0.0, 0.045, 0.060 }; /// P < 0.1, Rho > 0.006
    const double aA2B2[6] = { 0.0, 0.008, 0.0, 0.0, 0.007, 0.008 }; /// P > 0.6, Rho > 0.006
    const double aA1B1[6] = { 0.0, 0.006, 0.0, 0.0, 0.005, 0.006 }; /// P < 0.1, Rho = 0.002
    const double aA2B1[6] = { 0.0, 0.0  , 0.0, 0.0, 0.0  , 0.000 }; /// P > 0.6, Rho = 0.002

    for ( int i=0; i<6; ++i )
    {
        rTable.adTable_A1_B1[i] = aA1B1[i];
        rTable.adTable_A1_B2[i] = aA1B2[i];
        rTable.adTable_A2_B1[i] = aA2B1[i];
        rTable.adTable_A2_B2[i] = aA2B2[i];
    }
}

void CSeisEvalKISTEC2019::GetMFactTable_ConColm_Group3(int nSeisCompType, OUT T_MEMBMFACT_TABLE_2& rTable)
{
    rTable.Initialize();

    /// axial index = P / (Ag*fck)
    rTable.dA1 = 0.1;
    rTable.dA2 = 0.6;
    /// rho index = Av / (bw*s)
    rTable.dB1 = 0.002;
    rTable.dB2 = 0.006;

    /// see [KISTEC 2019, table 5.4.4],  { m-IO, m-LS(1st), m-CP(1st), m-LS(2nd), m-CP(2nd) }
    const double aA1B2[5] = { 1.0, 1.0, 1.0, 4.0, 5.0 }; /// P < 0.1, Rho > 0.006
    const double aA2B2[5] = { 1.0, 1.0, 1.0, 1.6, 1.8 }; /// P > 0.6, Rho > 0.006
    const double aA1B1[5] = { 1.0, 1.0, 1.0, 1.1, 1.2 }; /// P < 0.1, Rho < 0.002
    const double aA2B1[5] = { 1.0, 1.0, 1.0, 1.0, 1.0 }; /// P > 0.6, Rho < 0.002

    for ( int i=0; i<3; ++i )
    {
        int nIdx = GetColIndexMFactTable(nSeisCompType, i);
        rTable.adTable_A1_B1[i] = aA1B1[nIdx];
        rTable.adTable_A1_B2[i] = aA1B2[nIdx];
        rTable.adTable_A2_B1[i] = aA2B1[nIdx];
        rTable.adTable_A2_B2[i] = aA2B2[nIdx];
    }
}

void CSeisEvalKISTEC2019::GetParamTable_ConColmInfilled_Comp(OUT double radTable[6])
{
    /// see [KISTEC 2019, table 6.1.6] parameter d(%), e(%), c, IO(%), LS(%), CP(%).
    /// 보수적인 값을 사용 (그 밖의 경우)
    double adTable[6] = { 0.003, 0.01, 0.2, 0.002, 0.002, 0.003 }; 
    memcpy(radTable, adTable, sizeof(double)*6);    
}

void CSeisEvalKISTEC2019::GetParamTable_ConColmInfilled_Tens(OUT double radTable[6])
{
    /// see [KISTEC 2019, table 6.1.6] 그 밖의 경우에 값이 명시되지 않아 _T("주근 이음부가 충분히 구속된 경우~")의 값을 사용.
    double adTable[6] = { 0.05, 0.05, 0.0, 0.01, 0.03, 0.04 };
    memcpy(radTable, adTable, sizeof(double)*6);
}

void CSeisEvalKISTEC2019::GetMFactTable_ConColmInfilled_Comp(int nSeisCompType, OUT double radMFact[3])
{
    memset(radMFact, 0.0, sizeof(double)*3);
    /// see [KISTEC 2019, table 6.1.4], { m-IO, m-LS(1st), m-CP(1st), m-LS(2nd), m-CP(2nd) }
    /// 보수적인 값을 사용 (그 밖의 경우)
    double adMFactComp[5] = { 1.0, 1.0, 1.0, 1.0, 1.0 };        
    for ( int i = 0 ; i < 3 ; ++i )
    {
        int nIdx = GetColIndexMFactTable(nSeisCompType, i);
        radMFact[i] = adMFactComp[nIdx];
    }
}

void CSeisEvalKISTEC2019::GetMFactTable_ConColmInfilled_Tens(int nSeisCompType, OUT double radMFact[3])
{
    memset(radMFact, 0.0, sizeof(double)*3);
    /// see [KISTEC 2019, table 6.1.4], { m-IO, m-LS(1st), m-CP(1st), m-LS(2nd), m-CP(2nd) }    
    /// 보수적인 값을 사용 (그 밖의 경우)
    double adMFactTens[5] = { 1.0, 2.0, 2.0, 3.0, 4.0 };
        
    for ( int i = 0;  i < 3 ; ++i )
    {
        int nIdx = GetColIndexMFactTable(nSeisCompType, i);
        radMFact[i] = adMFactTens[nIdx];
    }
}

void CSeisEvalKISTEC2019::GetParamTable_ConWall_CtrlFlex(BOOL bBE, OUT T_MEMBPARAM_TABLE_2& rTable)
{
    rTable.Initialize();

    /// axial index = P / (Ag*fck)
    rTable.dA1 = 0.1;
    rTable.dA2 = 0.25;
    /// shear index = V / (sqrt(fck)*tw*lw)
    rTable.dB1 = 0.33;
    rTable.dB2 = 0.5;

    /// see [KISTEC 2019, table 5.5.4],  { a, b, c, IO, LS, CP }
    const double aA1B1[2][6] = { { 0.015, 0.020, 0.75, 0.005 , 0.015, 0.020 }, { 0.008, 0.015, 0.60, 0.002, 0.008, 0.015 } }; 
    const double aA1B2[2][6] = { { 0.010, 0.015, 0.40, 0.004 , 0.01 , 0.015 }, { 0.006, 0.010, 0.30, 0.002, 0.006, 0.01  } }; 
    const double aA2B1[2][6] = { { 0.009, 0.012, 0.60, 0.003 , 0.009, 0.012 }, { 0.003, 0.005, 0.25, 0.001, 0.003, 0.005 } }; 
    const double aA2B2[2][6] = { { 0.005, 0.010, 0.30, 0.0015, 0.005, 0.01  }, { 0.002, 0.004, 0.20, 0.001, 0.002, 0.004 } }; 

    int nIdx = bBE ? 0 : 1;
    for ( int i=0; i<6; ++i )
    {
        rTable.adTable_A1_B1[i] = aA1B1[nIdx][i];
        rTable.adTable_A1_B2[i] = aA1B2[nIdx][i];
        rTable.adTable_A2_B1[i] = aA2B1[nIdx][i];
        rTable.adTable_A2_B2[i] = aA2B2[nIdx][i];
    }
}

void CSeisEvalKISTEC2019::GetMFactTable_ConWall_CtrlFlex(int nSeisCompType, BOOL bBE, OUT T_MEMBMFACT_TABLE_2& rTable)
{
    rTable.Initialize();

    /// axial index = P / (Ag*fck)
    rTable.dA1 = 0.1;
    rTable.dA2 = 0.25;
    /// shear index = V / (sqrt(fck)*tw*lw)
    rTable.dB1 = 0.33;
    rTable.dB2 = 0.5;

    /// see [KISTEC 2019, table 5.5.2],  { m-IO, m-LS(1st), m-CP(1st), m-LS(2nd), m-CP(2nd) }
    const double aA1B1[2][5] = { { 2.0 , 4.0, 6.0, 6.0, 8.0 }, { 2.0 , 2.5, 4.0 , 4.0 , 6.0 } };
    const double aA1B2[2][5] = { { 2.0 , 3.0, 4.0, 4.0, 6.0 }, { 1.5 , 2.0, 2.5 , 2.5 , 4.0 } };
    const double aA2B1[2][5] = { { 1.5 , 3.0, 4.0, 4.0, 6.0 }, { 1.25, 1.5, 2.0 , 2.0 , 3.0 } };
    const double aA2B2[2][5] = { { 1.25, 2.0, 2.5, 2.5, 4.0 }, { 1.25, 1.5, 1.75, 1.75, 2.0 } };

    int nIdx1 = bBE ? 0 : 1;
    for ( int i=0; i<3; ++i )
    {
        int nIdx2 = GetColIndexMFactTable(nSeisCompType, i);
        rTable.adTable_A1_B1[i] = aA1B1[nIdx1][nIdx2];
        rTable.adTable_A1_B2[i] = aA1B2[nIdx1][nIdx2];
        rTable.adTable_A2_B1[i] = aA2B1[nIdx1][nIdx2];
        rTable.adTable_A2_B2[i] = aA2B2[nIdx1][nIdx2];
    }
}

void CSeisEvalKISTEC2019::GetParamTable_ConWall_CtrlSher(double dAxlIdx, OUT double radTable[6])
{
    memset(radTable, 0.0, sizeof(double)*6);

    /// see [KISTEC 2019, table 5.5.5] parameter d(%), e(%), c, IO(%), LS(%), CP(%).
    double adTable[2][6] = { { 0.01, 0.02, 0.2, 0.004, 0.015, 0.02 }, { 0.0075, 0.01, 0.0, 0.004, 0.0075, 0.01 } };
    int nIdx = dAxlIdx > 0.05 ? 1 : 0;
    for ( int i=0; i<6; ++i )
    {
        radTable[i] = adTable[nIdx][i];
    } 
}

void CSeisEvalKISTEC2019::GetMFactTable_ConWall_CtrlSher(int nSeisCompType, double dAxlIdx, OUT double radMFact[3])
{
    memset(radMFact, 0.0, sizeof(double)*3);

    if (dAxlIdx > 0.15)
    {
        memset(radMFact, 1.0, sizeof(double)*3);  /// 축력이 0.15*Ag*fck인 경우, 하중지배 부재로 간주한다. 
        return;
    }

    /// see [KISTEC 2019, table 5.5.3], { m-IO, m-LS(1st), m-CP(1st), m-LS(2nd), m-CP(2nd) }
    double adMFact[2][5] = { { 2.0, 2.5, 3.0, 4.5, 6.0 } , { 1.5, 2.0, 3.0, 3.0, 4.0 } };
    int nIdx1 = dAxlIdx > 0.05 ? 1 : 0;
    for ( int i=0; i<3; ++i )
    {
        int nIdx2 = GetColIndexMFactTable(nSeisCompType, i);
        radMFact[i] = adMFact[nIdx1][nIdx2];
    }
}

BOOL CSeisEvalKISTEC2019::CalcParamStlBeam(double dBTR, double dDTR, double adBTR[2], double adDTR[2], double dThetay, OUT double radParam[6])
{
    memset(radParam, 0.0, sizeof(double)*6);

    /// <표7-4> 보-휨.
    radParam[HG_A ] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR,  9.0, 4.0 );
    radParam[HG_B ] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, 11.0, 6.0 );
    radParam[HG_C ] =       CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR,  0.6, 0.2 );
    radParam[HG_IO] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR,  1.0, 0.25);
    radParam[HG_LS] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR,  9.0, 3.0 );
    radParam[HG_CP] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, 11.0, 4.0 );	

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcWTRLimitStlBeam(const CString& strSectShape, double dE, double dFye, OUT double radBTR[2], OUT double radDTR[2])
{
    /// <표 7-4>
    if (fabs(dFye) < cEvalZero) { ASSERT(FALSE); return FALSE; }

    radBTR[WTR_SC] = 0.30*sqrt(dE/dFye);
    radBTR[WTR_NC] = 0.38*sqrt(dE/dFye);

    radDTR[WTR_SC] = 2.45*sqrt(dE/dFye);
    radDTR[WTR_NC] = 3.76*sqrt(dE/dFye);

    return TRUE;
}

double CSeisEvalKISTEC2019::CalcPnCompStl(double dAg, double dFy, double dE, double dSlen)
{
    double dFe  = CMathFunc::m_pi*CMathFunc::m_pi*dE/(dSlen*dSlen);
    double dFcr = 0.0;
    double dSlenLimit = 4.71*sqrt(dE/dFy);
    if (dSlen > dSlenLimit)
    {
        dFcr = 0.877*dFe;
    }
    else
    {
        dFcr = pow(0.658, dFy/dFe)*dFy;
    }

    return dAg*dFcr;
}

double CSeisEvalKISTEC2019::CalcPnTensStl(double dAg, double dFye)
{
    return dAg*dFye;
}

EN_STLCOLM CSeisEvalKISTEC2019::GetStlColmForcType(double dP, double dPCL)
{
    /// (+)는 인장, (-)는 압축.
    if (dP > 0.0)            { return EN_STLCOLM_PLT02PY; } /// 인장인 경우에는 P<0.2Py로 고려한다.	
    if (fabs(dP) < 0.2*dPCL) { return EN_STLCOLM_PLT02PY; }
    if (fabs(dP) < 0.5*dPCL) { return EN_STLCOLM_PLT05PY; }

    return EN_STLCOLM_PGT05PY;
}

double CSeisEvalKISTEC2019::CalcKp(double dP, double dPy)
{
    return 1.0 - 5.0/3.0*fabs(dP)/dPy;
}

BOOL CSeisEvalKISTEC2019::CalcWTRLimitStlColm(const CString& strSectShape, EN_STLCOLM enType, double dE, double dFye, OUT double radBTR[2], OUT double radDTR[2])
{
    memset(radBTR, 0.0, sizeof(double)*2);
    memset(radDTR, 0.0, sizeof(double)*2);

    if (fabs(dFye) < cEvalZero) { ASSERT(FALSE); return FALSE; }

    if (strSectShape == D_SECT_SHAPE_REG_B)
    {
        radBTR[WTR_SC] = radDTR[WTR_SC] = 0.64*sqrt(dE/dFye);
        radBTR[WTR_NC] = radDTR[WTR_NC] = 1.12*sqrt(dE/dFye);		 
    }
    else
    {
        switch (enType)
        {
        case EN_STLCOLM_PLT02PY:
            {
                radBTR[WTR_SC] = 0.30*sqrt(dE/dFye);
                radBTR[WTR_NC] = 0.38*sqrt(dE/dFye);
                radDTR[WTR_SC] = 1.76*sqrt(dE/dFye);
                radDTR[WTR_NC] = 2.70*sqrt(dE/dFye);
            }
            break;
        case EN_STLCOLM_PLT05PY:
            {
                radBTR[WTR_SC] = 0.30*sqrt(dE/dFye);
                radBTR[WTR_NC] = 0.38*sqrt(dE/dFye);
                radDTR[WTR_SC] = 1.53*sqrt(dE/dFye);
                radDTR[WTR_NC] = 2.35*sqrt(dE/dFye);
            }
            break;
        case EN_STLCOLM_PGT05PY: break;
        default: ASSERT(FALSE); break;
        }
    }

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcParamStlColm(EN_STLCOLM enForcType, double dkp, double dBTR, double dDTR, double adBTR[2], double adDTR[2], double dThetay, OUT double radParam[6])
{
    memset(radParam, 0.0, sizeof(double)*6);

    double adTableSC[6] = { 0.0, };
    double adTableNC[6] = { 0.0, };
    switch (enForcType)
    {
    case EN_STLCOLM_PLT02PY:
        {
            adTableSC[HG_A ] =  9.0;  adTableNC[HG_A ] = 4.0;
            adTableSC[HG_B ] = 11.0;  adTableNC[HG_B ] = 6.0;
            adTableSC[HG_C ] =  0.6;  adTableNC[HG_C ] = 0.2;
            adTableSC[HG_IO] =  1.0;  adTableNC[HG_IO] = 0.25;
            adTableSC[HG_LS] =  9.0;  adTableNC[HG_LS] = 3.0;
            adTableSC[HG_CP] = 11.0;  adTableNC[HG_CP] = 4.0;
        }
        break;
    case EN_STLCOLM_PLT05PY:
        {			
            adTableSC[HG_A ] = 11.0*dkp;  adTableNC[HG_A ] = 1.0;
            adTableSC[HG_B ] = 17.0*dkp;  adTableNC[HG_B ] = 1.5;
            adTableSC[HG_C ] =  0.2;      adTableNC[HG_C ] = 0.2;
            adTableSC[HG_IO] =  0.25;     adTableNC[HG_IO] = 0.25;
            adTableSC[HG_LS] =  8.0*dkp;  adTableNC[HG_LS] = 1.2;
            adTableSC[HG_CP] = 11.0*dkp;  adTableNC[HG_CP] = 1.2;
        }
        break;
    }


    radParam[HG_A ] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, adTableSC[HG_A ], adTableNC[HG_A ]);
    radParam[HG_B ] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, adTableSC[HG_B ], adTableNC[HG_B ]);
    radParam[HG_C ] =       CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, adTableSC[HG_C ], adTableNC[HG_C ]);
    radParam[HG_IO] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, adTableSC[HG_IO], adTableNC[HG_IO]);
    radParam[HG_LS] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, adTableSC[HG_LS], adTableNC[HG_LS]);
    radParam[HG_CP] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, adTableSC[HG_CP], adTableNC[HG_CP]);		

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcTableStlBracCompSlender(UINT unBuck, const CString& strShape, OUT double radTable[6])
{
    memset(radTable, 0.0, sizeof(double)*6);

    auto l_SetTable = [&](double dA, double dB, double dC, double dIO, double dLS, double dCP, OUT double adTable[6])
    {
        adTable[HG_A ] = dA;
        adTable[HG_B ] = dB;
        adTable[HG_C ] = dC;
        adTable[HG_IO] = dIO;
        adTable[HG_LS] = dLS;
        adTable[HG_CP] = dCP;			
    };

    if (strShape == D_SECT_SHAPE_REG_H || (unBuck == EN_BRACBUCK_IN && (strShape == D_SECT_SHAPE_REG_2L || strShape == D_SECT_SHAPE_REG_2C)))
    {
        l_SetTable(0.5, 10.0, 0.3, 0.5, 8.0, 10.0, radTable);
    }
    else if (unBuck == EN_BRACBUCK_OUT && (strShape == D_SECT_SHAPE_REG_2L || strShape == D_SECT_SHAPE_REG_2C))
    {
        l_SetTable(0.5, 9.0, 0.3, 0.5, 7.0, 9.0, radTable);
    }
    else if (strShape == D_SECT_SHAPE_REG_B || strShape == D_SECT_SHAPE_REG_P)
    {
        l_SetTable(0.5, 9.0, 0.3, 0.5, 7.0, 9.0, radTable);
    }
    else if (strShape == D_SECT_SHAPE_REG_L)
    {
        l_SetTable(0.5, 12.0, 0.3, 0.5, 9.0, 12.0, radTable);
    }
    else 
    {
        ASSERT(FALSE); return FALSE;
    }
    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcTableStlBracCompStocky(UINT unBuck, const CString& strShape, double dBTR, double dDTR, double adBTR[2], double adDTR[2], OUT double radTable[6])
{
    memset(radTable, 0.0, sizeof(double)*6);

    auto l_CalcTable = [&](double dA, double dB, double dC, double dIO, double dLS, double dCP, OUT double radCalc[6])
    {
        radCalc[HG_A ] = CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dA, dA);
        radCalc[HG_B ] = CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dB, dB);
        radCalc[HG_C ] = CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dC, dC);
        radCalc[HG_IO] = CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dIO, dIO*0.5);
        radCalc[HG_LS] = CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dLS, dLS*0.5);
        radCalc[HG_CP] = CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dCP, dCP*0.5);			
    };

    /// <표7-6> 가새단면이 내진조밀단면이나 콘크리트충전단면인 경우에 이 모델링 변수와 허용기준을 적용한다.
    /// 가새단면이 비조밀인 경우, 허용기준에 0.5를 곱한다. 
    /// 내진조밀단면과 비조밀단면 사이의 단면을 가진 가새의 경우,직선보간을 통해 결정한 값을 허용기준에 곱한다.
    if (strShape == D_SECT_SHAPE_REG_H || (unBuck == EN_BRACBUCK_IN && (strShape == D_SECT_SHAPE_REG_2L || strShape == D_SECT_SHAPE_REG_2C)))
    {
        l_CalcTable(1.0, 8.0, 0.5, 0.5, 7.0, 8.0, radTable);
    }
    else if (unBuck == EN_BRACBUCK_OUT && (strShape == D_SECT_SHAPE_REG_2L || strShape == D_SECT_SHAPE_REG_2C))
    {
        l_CalcTable(1.0, 7.0, 0.5, 0.5, 6.0, 7.0, radTable);
    }
    else if (strShape == D_SECT_SHAPE_REG_B || strShape == D_SECT_SHAPE_REG_P)
    {
        l_CalcTable(1.0, 7.0, 0.5, 0.5, 6.0, 7.0, radTable);
    }
    else
    {
        ASSERT(FALSE); return FALSE; 
    }

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcParamStlBracComp(UINT unBuck, const CString& strShape, double dSlen, double dFy, double dE, double dBTR, double dDTR, double adBTR[2], double adDTR[2], OUT double radParam[6])
{
    double adTableSLD[6] = { 0.0, };
    CalcTableStlBracCompSlender(unBuck, strShape, adTableSLD);
    double adTableSKT[6] = { 0.0, };
    CalcTableStlBracCompStocky(unBuck, strShape, dBTR, dDTR, adBTR, adDTR, adTableSKT);

    double dSqrtEFy = fabs(dFy) < cEvalZero ? 0.0 : sqrt(dE/dFy);
    double dRatio   = CalcDistRatio(dSlen, 2.1*dSqrtEFy, 4.2*dSqrtEFy);

    radParam[HG_A ] = 1.0 + CMathFunc::mathInterpolate(adTableSKT[HG_A ], adTableSLD[HG_A ], dRatio);
    radParam[HG_B ] = 1.0 + CMathFunc::mathInterpolate(adTableSKT[HG_B ], adTableSLD[HG_B ], dRatio);
    radParam[HG_C ] =       CMathFunc::mathInterpolate(adTableSKT[HG_C ], adTableSLD[HG_C ], dRatio);
    radParam[HG_IO] = 1.0 + CMathFunc::mathInterpolate(adTableSKT[HG_IO], adTableSLD[HG_IO], dRatio);
    radParam[HG_LS] = 1.0 + CMathFunc::mathInterpolate(adTableSKT[HG_LS], adTableSLD[HG_LS], dRatio);
    radParam[HG_CP] = 1.0 + CMathFunc::mathInterpolate(adTableSKT[HG_CP], adTableSLD[HG_CP], dRatio);

    return TRUE;
}

BOOL CSeisEvalKISTEC2019::CalcParamStlBracTens(const CString& strShape, OUT double radParam[6])
{
    memset(radParam, 0.0, sizeof(double)*6);

    if      (strShape == D_SECT_SHAPE_REG_H ) { GetHingeParamStl(10.0, 13.0, 0.6, 0.5, 10.0, 13.0, radParam); }
    else if (strShape == D_SECT_SHAPE_REG_2L) { GetHingeParamStl( 9.0, 12.0, 0.6, 0.5,  9.0, 12.0, radParam); }
    else if (strShape == D_SECT_SHAPE_REG_B ) { GetHingeParamStl( 9.0, 11.0, 0.6, 0.5,  8.0, 11.0, radParam); }
    else if (strShape == D_SECT_SHAPE_REG_P ) { GetHingeParamStl( 8.0,  9.0, 0.6, 0.5,  7.0,  9.0, radParam); }
    else if (strShape == D_SECT_SHAPE_REG_L ) { GetHingeParamStl(10.0, 11.0, 0.6, 0.5,  8.0, 10.0, radParam); }
    else  { ASSERT(FALSE); return FALSE; }

    return TRUE;
}

void CSeisEvalKISTEC2019::CalcEval1stStlBeamPos(int nSeisCompType, const T_EVAL1ST_STLRSLT_POS& RsltPos, double dBTR, double dDTR, double adBTR[2], double adDTR[2], OUT T_EVAL1ST_STLBEAM_POS& rEvalPos)
{
    rEvalPos.dMu = RsltPos.dMUDy;
    rEvalPos.dMe = RsltPos.dMnEy;

    double admFacSC[3] = { 2.00, 6.0, 8.0 };
    double admFacNC[3] = { 1.25, 2.0, 3.0 };
    if (nSeisCompType == D_SCMT_TYPE_SECONDARY)
    {
        admFacSC[EN_LEVEL_LS] = 10.0; admFacSC[EN_LEVEL_CP] = 12.0;
        admFacNC[EN_LEVEL_LS] =  3.0; admFacNC[EN_LEVEL_CP] =  4.0;
    }

    const double dScaleFact = GetScaleUpFactor4MFact();
    for (int i=EN_LEVEL_IO; i<=EN_LEVEL_CP; ++i)
    {
        if (!rEvalPos.bUserMfac)
        {
            double dmFact = CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, admFacSC[i], admFacNC[i]);
            rEvalPos.dmFac[i] = CalcMFactModifiedByStrnTerm(dmFact, RsltPos.dRatMpMnToMpMr);
            rEvalPos.dmFac[i] *= dScaleFact;
        }		
        rEvalPos.DCR.dDCR[i] = CalcDCRDisp(RsltPos.dMUDy, RsltPos.dMnEy, rEvalPos.dmFac[i]);
    }	

    rEvalPos.DCR.enLevel = GetPerformLevel(rEvalPos.DCR.dDCR);
    rEvalPos.enLevel     = rEvalPos.DCR.enLevel;
}


void CSeisEvalKISTEC2019::CalcEval1stStlColmPos(int nSeisCompType, const CString& strShape, const T_SECT_SECTBASE_D& SectBaseD, double dE, double dFye, double dPy,
                                             const T_EVAL1ST_STLRSLT_POS& RsltPos, OUT T_EVAL1ST_STLCOLM_POS& rEvalPos)
{
    rEvalPos.SetForcFromRslt(RsltPos);

    double dP = RsltPos.dPUF;
    EN_STLCOLM enColmType = GetStlColmForcType(dP, dPy);
    double dkp = CalcKp(dP, dPy);

    double adBTR[2] = { 0.0, };
    double adDTR[2] = { 0.0, };
    CalcWTRLimitStlColm(strShape, enColmType, dE, dFye, adBTR, adDTR);

    if (!rEvalPos.bUserMfac)
    {
        double dBTR = 0.0;
        double dDTR = 0.0;
        CalcWTR(strShape, SectBaseD, dBTR, dDTR);

        CalcMFactStlColm(enColmType, dkp, nSeisCompType, dBTR, dDTR, adBTR, adDTR, rEvalPos.dmFacy);
        CalcMFactStlColm(enColmType, dkp, nSeisCompType, dBTR, dDTR, adBTR, adDTR, rEvalPos.dmFacz);		
        CalcMFactStlColmTens(nSeisCompType, rEvalPos.dmFacT);

        if ( enColmType != EN_STLCOLM_PGT05PY )
        {
            const double dScaleFact = GetScaleUpFactor4MFact();
            MultiplyScaleFactorMFact(dScaleFact, rEvalPos.dmFacy);
            MultiplyScaleFactorMFact(dScaleFact, rEvalPos.dmFacz);
            MultiplyScaleFactorMFact(dScaleFact, rEvalPos.dmFacT);
        }

        rEvalPos.dBTR = dBTR;
        rEvalPos.dDTR = dDTR;
        rEvalPos.dBTRsc = adBTR[0]; 
        rEvalPos.dDTRsc = adDTR[0];
        rEvalPos.dBTRc  = adBTR[1];
        rEvalPos.dDTRc  = adDTR[1];
    }

    rEvalPos.enForcType = enColmType;
    for ( int i = EN_LEVEL_IO ; i <= EN_LEVEL_CP ; ++i )
    {
        rEvalPos.DCR.dDCR[i] = CalcDCRStlColm(static_cast<EN_LEVEL>(i), rEvalPos);
    }

    rEvalPos.DCR.enLevel = GetPerformLevel(rEvalPos.DCR.dDCR);
}

void CSeisEvalKISTEC2019::CalcMFactStlColm(EN_STLCOLM enForcType, double dkp, int nSeisCompType, double dBTR, double dDTR, double adBTR[2], double adDTR[2], OUT double radMFac[3])
{
    memset(radMFac, 1.0, sizeof(double)*3);

    double adTableSC[3] = { 0.0, };
    double adTableNC[3] = { 0.0, };
    switch (enForcType)
    {
    case EN_STLCOLM_PLT02PY:
        {
            adTableSC[EN_LEVEL_IO] = 2.0;  adTableNC[EN_LEVEL_IO] = 1.25;
            adTableSC[EN_LEVEL_LS] = 6.0;  adTableNC[EN_LEVEL_LS] = 1.25;
            adTableSC[EN_LEVEL_CP] = 8.0;  adTableNC[EN_LEVEL_CP] = 2.0;	
            if (nSeisCompType == D_SCMT_TYPE_SECONDARY)
            {
                adTableSC[EN_LEVEL_LS] = 10.0;  adTableNC[EN_LEVEL_LS] = 2.0;
                adTableSC[EN_LEVEL_CP] = 12.0;  adTableNC[EN_LEVEL_CP] = 3.0;
            }
        }
        break;
    case EN_STLCOLM_PLT05PY:
        {			
            adTableSC[EN_LEVEL_IO] =  1.25;     adTableNC[EN_LEVEL_IO] = 1.25;
            adTableSC[EN_LEVEL_LS] =  9.0*dkp;  adTableNC[EN_LEVEL_LS] = 1.25;
            adTableSC[EN_LEVEL_CP] = 12.0*dkp;  adTableNC[EN_LEVEL_CP] = 1.5;
            if (nSeisCompType == D_SCMT_TYPE_SECONDARY)
            {
                adTableSC[EN_LEVEL_LS] = 15.0*dkp;  adTableNC[EN_LEVEL_LS] = 2.0;
                adTableSC[EN_LEVEL_CP] = 18.0*dkp;  adTableNC[EN_LEVEL_CP] = 2.0;
            }
        }
        break;
    case EN_STLCOLM_PGT05PY:
        break;
    default: ASSERT(FALSE); break;
    }

    if ( enForcType == EN_STLCOLM_PGT05PY )
    {
        for (int i=EN_LEVEL_IO; i<=EN_LEVEL_CP; ++i)
        {
            radMFac[i] = 1.0;
        }
    }
    else
    {
        for (int i=EN_LEVEL_IO; i<=EN_LEVEL_CP; ++i)
        {
            radMFac[i] = CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, adTableSC[i], adTableNC[i]);
        }    
    }	
}

void CSeisEvalKISTEC2019::CalcMFactStlColmTens(int nSeisCompType, OUT double radMFac[3])
{
    memset(radMFac, 1.0, sizeof(double)*3);

    radMFac[EN_LEVEL_IO] = 1.25;
    switch (nSeisCompType)
    {
    case D_SCMT_TYPE_PRIMARY:   { radMFac[EN_LEVEL_LS] = 3.0;  radMFac[EN_LEVEL_CP] = 5.0; } break;
    case D_SCMT_TYPE_SECONDARY: { radMFac[EN_LEVEL_LS] = 6.0;  radMFac[EN_LEVEL_CP] = 7.0; } break;
    default: ASSERT(FALSE); break;
    }
}


void CSeisEvalKISTEC2019::CalcEval1stStlBracPos(int nSeisCompType, const CString& strShape, const T_SECT_SECTBASE_D& SectBaseD, double dFye, double dE, double dSlen,
                                             double adBTR[2], double adDTR[2], UINT unBuck, const T_EVAL1ST_STLRSLT_POS& RsltPos, OUT T_EVAL1ST_STLBRAC_POS& rEvalPos)
{
    rEvalPos.dPUD = RsltPos.dPUD;
    rEvalPos.dPnC = RsltPos.dPnEC;
    rEvalPos.dPnT = RsltPos.dPnET;
    rEvalPos.dKLr = dSlen;

    if (!rEvalPos.bUserMfac)
    {
        if (RsltPos.dPUD < 0.0) /// compression
        {
            double dBTR = 0.0;
            double dDTR = 0.0;
            CalcWTR(strShape, SectBaseD, dBTR, dDTR);
            CalcMFactStlBracComp(nSeisCompType, unBuck, strShape, dSlen, dFye, dE, dBTR, dDTR, adBTR, adDTR, rEvalPos.unSlenType, rEvalPos.dmFac);
        }
        else
        {
            CalcMFactStlBracTens(nSeisCompType, strShape, rEvalPos.dmFac);
        }

        const double dScaleFact = GetScaleUpFactor4MFact();
        MultiplyScaleFactorMFact(dScaleFact, rEvalPos.dmFac);
    }

    const double dQCE = rEvalPos.dPUD < 0.0 ? rEvalPos.dPnC : rEvalPos.dPnT;
    for ( int i = EN_LEVEL_IO ; i <= EN_LEVEL_CP ; ++i )
    {
        rEvalPos.DCR.dDCR[i] = CalcDCRDisp(RsltPos.dPUD, dQCE, rEvalPos.dmFac[i]);
    }

    rEvalPos.DCR.enLevel = GetPerformLevel(rEvalPos.DCR.dDCR);	
}

void CSeisEvalKISTEC2019::GetMFactTableStlBracCompSlender(int nSeisCompType, UINT unBuck, const CString& strShape, OUT double radTable[3])
{
    memset(radTable, 1.0, sizeof(double)*3);

    auto l_SetTable = [](int nSeisType, double dIO, double dLS1, double dCP1, double dLS2, double dCP2, OUT double adTable[6])
    {
        adTable[EN_LEVEL_IO] = dIO;
        adTable[EN_LEVEL_LS] = nSeisType==D_SCMT_TYPE_PRIMARY ? dLS1 : dLS2;
        adTable[EN_LEVEL_CP] = nSeisType==D_SCMT_TYPE_PRIMARY ? dCP1 : dCP2;	
    };

    if (strShape == D_SECT_SHAPE_REG_H || (unBuck == EN_BRACBUCK_IN && (strShape == D_SECT_SHAPE_REG_2L || strShape == D_SECT_SHAPE_REG_2C)))
    {
        l_SetTable(nSeisCompType, 1.25, 6.0, 8.0, 7.0, 9.0, radTable);
    }
    else if (unBuck == EN_BRACBUCK_OUT && (strShape == D_SECT_SHAPE_REG_2L || strShape == D_SECT_SHAPE_REG_2C))
    {
        l_SetTable(nSeisCompType, 1.25, 5.0, 7.0, 6.0, 8.0, radTable);
    }
    else if (strShape == D_SECT_SHAPE_REG_B || strShape == D_SECT_SHAPE_REG_P || strShape == D_SECT_SHAPE_REG_L)
    {
        l_SetTable(nSeisCompType, 1.25, 5.0, 7.0, 6.0, 8.0, radTable);
    }	
    else 
    {
        l_SetTable(nSeisCompType, 1.25, 5.0, 7.0, 6.0, 8.0, radTable); /// [GEN-2993] Table에 없는 단면은 최소값 적용.
    }	
}

void CSeisEvalKISTEC2019::GetMFactTableStlBracCompStocky(int nSeisCompType, UINT unBuck, const CString& strShape, double dBTR, double dDTR, double adBTR[2], double adDTR[2], OUT double radTable[3])
{
    memset(radTable, 1.0, sizeof(double)*3);

    auto l_CalcTable = [&](int nSeisType, double dIO, double dLS1, double dCP1, double dLS2, double dCP2, OUT double radCalc[3])
    {
        double dLS = nSeisType==D_SCMT_TYPE_PRIMARY ? dLS1 : dLS2;
        double dCP = nSeisType==D_SCMT_TYPE_PRIMARY ? dCP1 : dCP2;

        radCalc[EN_LEVEL_IO] = CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dIO, dIO*0.5);
        radCalc[EN_LEVEL_LS] = CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dLS, dLS*0.5);
        radCalc[EN_LEVEL_CP] = CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dCP, dCP*0.5);		
    };

    /// <표7-6> 가새단면이 내진조밀단면이나 콘크리트충전단면인 경우에 이 모델링 변수와 허용기준을 적용한다.
    /// 가새단면이 비조밀인 경우, 허용기준에 0.5를 곱한다. 
    /// 내진조밀단면과 비조밀단면 사이의 단면을 가진 가새의 경우,직선보간을 통해 결정한 값을 허용기준에 곱한다.
    if (strShape == D_SECT_SHAPE_REG_H || (unBuck == EN_BRACBUCK_IN && (strShape == D_SECT_SHAPE_REG_2L || strShape == D_SECT_SHAPE_REG_2C)))
    {
        l_CalcTable(nSeisCompType, 1.25, 5.0, 7.0, 6.0, 8.0, radTable);
    }
    else if (unBuck == EN_BRACBUCK_OUT && (strShape == D_SECT_SHAPE_REG_2L || strShape == D_SECT_SHAPE_REG_2C))
    {
        l_CalcTable(nSeisCompType, 1.25, 4.0, 6.0, 5.0, 7.0, radTable);
    }
    else if (strShape == D_SECT_SHAPE_REG_B || strShape == D_SECT_SHAPE_REG_P)
    {
        l_CalcTable(nSeisCompType, 1.25, 4.0, 6.0, 5.0, 7.0, radTable);
    }
    else
    {
        l_CalcTable(nSeisCompType, 1.25, 4.0, 6.0, 5.0, 7.0, radTable); /// [GEN-2993] Table에 없는 단면은 최소값 적용.
    }
}

void CSeisEvalKISTEC2019::CalcMFactStlBracComp(int nSeisCompType, UINT unBuck, const CString& strShape, double dSlen, double dFy, double dE, double dBTR, double dDTR,
                                            double adBTR[2], double adDTR[2], OUT UINT& runSlenType, OUT double radMFac[3])
{
    double adTableSLD[6] = { 0.0, };
    GetMFactTableStlBracCompSlender(nSeisCompType, unBuck, strShape, adTableSLD);
    double adTableSKT[6] = { 0.0, };
    GetMFactTableStlBracCompStocky(nSeisCompType, unBuck, strShape, dBTR, dDTR, adBTR, adDTR, adTableSKT);

    double dSqrtEFy = fabs(dFy) < cEvalZero ? 0.0 : sqrt(dE/dFy);
    double dRatio   = CalcDistRatio(dSlen, 2.1*dSqrtEFy, 4.2*dSqrtEFy);

    runSlenType = dSlen > 2.1*dSqrtEFy ? (dSlen < 4.2*dSqrtEFy ? EN_STLBRAC_NORMAL : EN_STLBRAC_SLENDER) : EN_STLBRAC_STOCKY;

    radMFac[EN_LEVEL_IO] = CMathFunc::mathInterpolate(adTableSKT[HG_A], adTableSLD[HG_A], dRatio);
    radMFac[EN_LEVEL_LS] = CMathFunc::mathInterpolate(adTableSKT[HG_B], adTableSLD[HG_B], dRatio);
    radMFac[EN_LEVEL_CP] = CMathFunc::mathInterpolate(adTableSKT[HG_C], adTableSLD[HG_C], dRatio);
}

void CSeisEvalKISTEC2019::CalcMFactStlBracTens(int nSeisCompType, const CString& strShape, OUT double radMFac[3])
{
    memset(radMFac, 1.0, sizeof(double)*3);

    double dRedu1st = 1.0;
    double dRedu2nd = 1.0;
    if (strShape == D_SECT_SHAPE_REG_2L || strShape == D_SECT_SHAPE_REG_B || strShape == D_SECT_SHAPE_REG_P || strShape == D_SECT_SHAPE_REG_L)
    {
        dRedu1st = 0.8;
        dRedu2nd = 0.7;
    }

    radMFac[EN_LEVEL_IO] = 1.25;
    switch (nSeisCompType)
    { 
    case D_SCMT_TYPE_PRIMARY:   { radMFac[EN_LEVEL_LS] = 5.0*dRedu1st;  radMFac[EN_LEVEL_CP] =  7.0*dRedu1st; } break;
    case D_SCMT_TYPE_SECONDARY: { radMFac[EN_LEVEL_LS] = 8.0*dRedu2nd;  radMFac[EN_LEVEL_CP] = 10.0*dRedu2nd; } break;
    default: ASSERT(FALSE); break;
    }
}

void CSeisEvalKISTEC2019::GetHingeParamStl(double dA, double dB, double dC, double dIO, double dLS, double dCP, OUT double radParam[6])
{
    radParam[HG_A ] = 1.0 + dA;
    radParam[HG_B ] = 1.0 + dB;
    radParam[HG_C ] =       dC;
    radParam[HG_IO] = 1.0 + dIO;
    radParam[HG_LS] = 1.0 + dLS;
    radParam[HG_CP] = 1.0 + dCP;
}

void CSeisEvalKISTEC2019::CalcSeisEval2ndRsltConColmDir(UINT unSeisDetail, BOOL bConsiderGroup1, double dVu, double dVs, double dVo, const T_CON_SHR_PARAM& Shr,
                                                     double dL, double dMecur, double dMei, double dMej, OUT T_EVAL2ND_COLM_DIR& rEvalDir)
{	
    double dVp = fabs(dL) < cEvalZero ? 0.0 : (dMei + dMej)/dL;

    rEvalDir.dShearIndex  = CalcShearIndex(dVu, Shr);
    rEvalDir.enCtrlType   = CalcColmCtrlType(dMei, dMej, dL, dVo);	    
    rEvalDir.unGroup      = GetGroupConColm(unSeisDetail, dVp, dVo, bConsiderGroup1);
}

bool CSeisEvalKISTEC2019::IsSectShapeStlBrac(const CString& strShape)
{
    if ( strShape == D_SECT_SHAPE_REG_H  ) { return true; }
    if ( strShape == D_SECT_SHAPE_REG_2L ) { return true; }
    if ( strShape == D_SECT_SHAPE_REG_2C ) { return true; }
    if ( strShape == D_SECT_SHAPE_REG_B  ) { return true; }
    if ( strShape == D_SECT_SHAPE_REG_P  ) { return true; }
    if ( strShape == D_SECT_SHAPE_REG_L  ) { return true; }
    return false;
}
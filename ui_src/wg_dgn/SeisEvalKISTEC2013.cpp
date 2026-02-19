#include "stdafx.h"
#include "SeisEvalKISTEC2013.h"

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

#include "..\..\dgnengine\idesign\DGN_lib\DGNCompare.h"


CSeisEvalKISTEC2013::CSeisEvalKISTEC2013(void) : CSeisEval()
{
    m_CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
    m_CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
}

CSeisEvalKISTEC2013::~CSeisEvalKISTEC2013(void)
{
}

BOOL CSeisEvalKISTEC2013::CalcMyHingePropConBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, double dRationMuMy,
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
        CalcMyHingePropConBeamPos(nSeisCompType, CurUphg, CurPhgt);
    }
    
    GetSEResult()->SetUphgConBeam(ElemK, Uphg);

    return TRUE;
}

double CSeisEvalKISTEC2013::CalcBeta1(double dfck)
{
    return dfck > 28.0 ? max(0.85 - 0.007*(dfck-28.0)/1.0, 0.65) : 0.85;
}

BOOL CSeisEvalKISTEC2013::CalcFxHingePropConColm(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    if (!GetElemMatlSectCon(ElemK, SectD, MatdD, SecmD)) { ASSERT(FALSE); return FALSE; }

    T_REBC_D RebcD;
    if (!m_pDoc->m_pAttrCtrl->GetRebcByElemK(ElemK, RebcD)) { ASSERT(FALSE); return FALSE; }

    CSeisEvalDataTool Tool;
    const double dFck = Tool.GetFc(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);
    const double dFym = Tool.GetFym(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);
    const double dAs  = GetRebarAs(RebcD.MainRebarName) * RebcD.nQrb;
    const double dAg  = SectD.SectBefore.SectI.Stiffness.Area;
    const double dFactorPnmax = GetFactorPnmaxColm(EN_EVALSTEP_2ND, RebcD.nHoopType);

    for ( int i=0; i<EN_MEMBEND_NUM; ++i )
    {
        T_PHGT_FEMA& rCur = i==EN_MEMBEND_I ? rPhgtI : rPhgtJ;

        rCur.bSymmetric = FALSE;
        rCur.bUserDefineRatio = TRUE;		
        rCur.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
        rCur.dYieldForce[IDX_PLS] = CalcPnTensCon(dFym, dAs);
        rCur.dYieldForce[IDX_MNS] = CalcPnCompCon(dFck, dAg, dFym, dAs, dFactorPnmax);

        SetCurveForcCtrlAxial(rCur.dForceMomentRatio);
        SetAccetanceCriteriaConAxial(rCur);
    }

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcFyzHingePropConColm(T_ELEM_K ElemK, EN_DIR enDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType,
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
        T_PHGT_FEMA& rCurPhgt = ( i == EN_MEMBEND_I ) ? rPhgtI : rPhgtJ;

        rCurPhgt.bSymmetric = FALSE;
        rCurPhgt.bUserDefineRatio = TRUE;	
        rCurPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
        SetCurveForcCtrlShear(rCurPhgt.dForceMomentRatio);
        SetAccetanceCriteriaForcCtrl(rCurPhgt);

        double dVn = CalcVnConColm(aShr[i], adP[i], adV[i], adM[i], admu[i]);
        dVn = GetUpdateHingeVnConColm(dVn);

        rCurPhgt.dYieldForce[IDX_PLS] = dVn;
        rCurPhgt.dYieldForce[IDX_MNS] = dVn;
    }

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcMyzHingePropConColm(T_ELEM_K ElemK, EN_DIR enDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType,
                                                  OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
    UINT unP = DOF_FX;
    UINT unV = enDir==EN_DIR_Y? DOF_FZ : DOF_FY;
    UINT unM = enDir==EN_DIR_Y? DOF_MY : DOF_MZ;	

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
    if (!MakeColmShearParamEval(ElemK, EN_EVALSTEP_2ND, enDirV, aShr)) { return FALSE; }
        
    _DB_PMCV_DATA Pmcvy, Pmcvz;    
    if (!GetPmcvData(ElemK, EN_EVALSTEP_2ND, EN_EVALMATL_AVG, Pmcvy, Pmcvz)) { return FALSE; }
    
    CSeisEvalDataTool Tool;
    const double dFck = Tool.GetFc(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);
    const double dAg  = SectD.SectBefore.SectI.Stiffness.Area;
        
    const bool bUseHook = Tool.IsUseSeisHook(ElemK);
    const int nSeisCompType = Tool.GetSeisComponentType(ElemK);

    T_UPHG_CONCOLM Uphg;
    Uphg.ElemK = ElemK;
    if ( !GetSEResult()->GetUphgConColm(ElemK, Uphg) ) { Uphg.Initialize(); }
    
    for ( int i=0; i<EN_MEMBEND_NUM; ++i)
    {
        const double dMey = abs(adM1y[i]);
        const double dMez = abs(adM1z[i]);

        const T_CON_SHR_PARAM& CurShr = aShr[i].Comm;
        const double dP   = adP[i];
        const double dV   = adV[i];        
        
        bool bEndI = ( i == EN_MEMBEND_I ) ? true : false;
        T_PHGT_FEMA& rCurPhgt = bEndI ? rPhgtI : rPhgtJ;
        T_UPHG_CONCOLM_POS& rUphgPos = bEndI ? Uphg.EndI : Uphg.EndJ;

        T_UPHG_CONCOLM_DIR& UphgDir = enDir==EN_DIR_Y ? rUphgPos.Diry : rUphgPos.Dirz;
        UphgDir.Param.dThetay = adDy[i];
        /// 휨 힌지는 전단지배로 판단되더라도 변형지배의 힌지곡선을 사용. 일반적인 상황에서는 전단이 먼저 항복하는 개념이므로, 휨 힌지를 하중지배로 모델링할 필요가 없음.  
        UphgDir.Info.enCtrlType   = EN_EVALCTRL_FLEX;
        UphgDir.Info.bSeisConform = IsTransReinfConforming(bUseHook, dV, CurShr);
        UphgDir.Info.dAxlIdx      = CalcAxialIndex(dP, dFck, dAg);
        UphgDir.Info.dShrIdx      = CalcShearIndex(dV, CurShr);	
        
        CalcMyzHingePropConColmPos(nSeisCompType, UphgDir, rCurPhgt);        
    }

    GetSEResult()->SetUphgConColm(ElemK, Uphg);

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcFxHingePropConWall(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
    T_WALL_KEY WallKey;
    T_WALL_D WallD;  WallD.Initialize();
    if (!m_pDoc->m_pPostCtrl->GetWallDataFromElemK(ElemK, WallD, &WallKey)) { return FALSE; }
    if ( !WallD.bLineWall ) { return FALSE; }

    T_THIK_D ThikD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    if (!GetElemMatlThik(ElemK, ThikD, MatdD, SecmD)) { return FALSE; }

    T_REBW_K RebwK;
    RebwK.keymap = WallKey;
    T_REBW_D RebwD;
    if (!m_pDoc->m_pAttrCtrl->GetRebw(RebwK, RebwD)) { return FALSE; }

    const double dTw = WallD.dThickness;
    const double dLw = WallD.dLength;
    const double dAs = CalcAsWall(WallD, RebwD);

    CSeisEvalDataTool Tool;
    const double dFck = Tool.GetFc(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);
    const double dFym = Tool.GetFym(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);        
    const double dFactorPnmax = GetFactorPnmaxWall(EN_EVALSTEP_2ND);

    for ( int i=0; i<EN_MEMBEND_NUM; ++i )
    {
        T_PHGT_FEMA& rCurPhgt = (i==EN_MEMBEND_I) ? rPhgtI : rPhgtJ;

        rCurPhgt.bSymmetric = FALSE;
        rCurPhgt.bUserDefineRatio = TRUE;	
        rCurPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
        rCurPhgt.dYieldForce[IDX_PLS] = CalcPnTensCon(dFym, dAs);
        rCurPhgt.dYieldForce[IDX_MNS] = CalcPnCompCon(dFck, dTw*dLw, dFym, dAs, dFactorPnmax);

        SetCurveForcCtrlAxial(rCurPhgt.dForceMomentRatio);
        SetAccetanceCriteriaConAxial(rCurPhgt);
    }
    
    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcFyHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt,
                                                 OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
    /// 약축 전단은 무조건 하중지배로 고려합니다. (/w 장주현 DR)    
    T_WALL_SHR_PARAM aShrAvg[EN_MEMBEND_NUM];
    T_WALL_SHR_PARAM aShrNom[EN_MEMBEND_NUM];
    if ( !MakeWallShearParam(ElemK, EN_EVALSTEP_2ND, EN_DIR_Y, aShrAvg, aShrNom) ) { return FALSE; }
    SetWallForcShearParam(PORslt.dForce, EN_DIR_Y, aShrAvg, aShrNom);

    for (int i=0; i<EN_MEMBEND_NUM; ++i)
    {
        T_PHGT_FEMA& rCurPhgt = (i==EN_MEMBEND_I) ? rPhgtI : rPhgtJ;

        rCurPhgt.bSymmetric  = FALSE;
        rCurPhgt.bUserDefineRatio = TRUE;
        rCurPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
        SetCurveForcCtrlShear(rCurPhgt.dForceMomentRatio);
        SetAccetanceCriteriaForcCtrl(rCurPhgt);
                
        double dVn = CalcVnConWall(aShrNom[i]);

        rCurPhgt.dYieldForce[IDX_PLS] = dVn;
        rCurPhgt.dYieldForce[IDX_MNS] = dVn;
    }

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcFzHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, 
                                                 OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
    T_WALL_KEY WallKey;
    T_WALL_D WallD;  WallD.Initialize();
    if (!m_pDoc->m_pPostCtrl->GetWallDataFromElemK(ElemK, WallD, &WallKey))  { return FALSE; }
    if ( !WallD.bLineWall ) { return FALSE; }

    T_THIK_D ThikD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    if (!GetElemMatlThik(ElemK, ThikD, MatdD, SecmD)) { ASSERT(0); return FALSE; }

    T_WALL_SHR_PARAM aShrAvg[EN_MEMBEND_NUM];
    T_WALL_SHR_PARAM aShrNom[EN_MEMBEND_NUM];
    if ( !MakeWallShearParam(ElemK, EN_EVALSTEP_2ND, EN_DIR_Z, aShrAvg, aShrNom) ) { return FALSE; }
    SetWallForcShearParam(PORslt.dForce, EN_DIR_Z, aShrAvg, aShrNom);

    // [GEN-1589] 휨강도는 하중지배이더라도 평균강도에 의한 값을 사용.    
    _DB_PMCV_DATA Pmcvy;
    _DB_PMCV_DATA Pmcvz;
    if ( !GetPmcvData(ElemK, EN_EVALSTEP_2ND, EN_EVALMATL_AVG, Pmcvy, Pmcvz) ) { return FALSE; }

    T_UPHG_CONWALL Uphg;
    Uphg.ElemK = ElemK;
    if ( !GetSEResult()->GetUphgConWall(ElemK, Uphg) ) { Uphg.Initialize(); }
    
    CSeisEvalDataTool Tool;
    const double dFck  = Tool.GetFc(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);
    const double dAg   = WallD.dThickness * WallD.dLength;
    const double dLw  = WallD.dLength;
    const double dHw  = WallD.dHeight;
    const double dKeff = 0.4*MatdD.Data1.Analysis.Elast*dAg;
    const int nPMMethod = Tool.GetPMCalcMethodWall(ElemK, EN_EVALSTEP_2ND);

    int nSeisCompType = Tool.GetSeisComponentType(ElemK);       
    for ( int i = 0 ; i < EN_MEMBEND_NUM ; ++i )
    {
        const EN_WALL_FORC_POS& enCurPos = GetWallForcPos(static_cast<EN_MEMBEND>(i));
        const T_WALL_SHR_PARAM& CurShrAvg = aShrAvg[i];
        const T_WALL_SHR_PARAM& CurShrNom = aShrNom[i];

        bool bEndI = (i == EN_MEMBEND_I) ?  true : false;

        T_UPHG_CONWALL_POS& CurUphg = bEndI ? Uphg.EndI : Uphg.EndJ;

        const double dPu     = PORslt.dForce[enCurPos][DOF_FX]*(-1.0);
        const double dMu     = PORslt.dForce[enCurPos][DOF_MY];

        const double dAxlIdx = CalcAxialIndex(dPu, dFck, dAg);        
        const double dVe     = CalcVnConWall(CurShrAvg);
        const double dMe = CalcMn(nPMMethod, Pmcvy, dPu, dMu);
        const double dThetay = fabs(dVe*dKeff) == 0.0 ? cThetaLimit : fabs(dVe/dKeff);

        const EN_EVALCTRL enCtrlType = CalcWallCtrlType(dMe, dVe, dHw, CurShrAvg); // 휨-전단지배 판단은 변형지배로 가정한 전단강도 Vn을 사용.
        const bool bForceCtrl = (enCtrlType == EN_EVALCTRL_SHER && dAxlIdx > 0.15) ? true : false;

        T_PHGT_FEMA& rCurPhgt = i==EN_MEMBEND_I ? rPhgtI : rPhgtJ;
        CalcFzHingePropConWallPos(nSeisCompType, dAxlIdx, dThetay, rCurPhgt);

        rCurPhgt.dYieldForce[IDX_PLS] = dVe;
        rCurPhgt.dYieldForce[IDX_MNS] = dVe;
    }

    GetSEResult()->SetUphgConWall(ElemK, Uphg);

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcMyHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
    T_WALL_KEY WallKey;
    T_WALL_D WallD;  WallD.Initialize();
    if (!m_pDoc->m_pPostCtrl->GetWallDataFromElemK(ElemK, WallD, &WallKey))  {  return FALSE; }
    if ( !WallD.bLineWall ) { return FALSE; }

    T_THIK_D ThikD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    if (!GetElemMatlThik(ElemK, ThikD, MatdD, SecmD)) { ASSERT(0); return FALSE; }

    T_REBW_K RebwK;
    RebwK.keymap = WallKey;
    T_REBW_D RebwD;
    if (!m_pDoc->m_pAttrCtrl->GetRebw(RebwK, RebwD)) { return FALSE; }
    
    const double dTw = WallD.dThickness;
    const double dLw = WallD.dLength;
    const double dHw = WallD.dHeight;

    CSeisEvalDataTool Tool;
    const bool   bBE  = Tool.IsConsiderBoundaryElement(RebwD);
    const double dFck = Tool.GetFc(EN_EVALSTEP_2ND,  EN_EVALMATL_NOM, SecmD);
    const double dAg  = dTw*dLw;

    T_WALL_SHR_PARAM aShrAvg[EN_MEMBEND_NUM];
    T_WALL_SHR_PARAM aShrNom[EN_MEMBEND_NUM];
    if ( !MakeWallShearParam(ElemK, EN_EVALSTEP_2ND, EN_DIR_Z, aShrAvg, aShrNom) ) { return FALSE; }
    SetWallForcShearParam(PORslt.dForce, EN_DIR_Z, aShrAvg, aShrNom);

    _DB_PMCV_DATA Pmcvy;
    _DB_PMCV_DATA Pmcvz;
    if ( !GetPmcvData(ElemK, EN_EVALSTEP_2ND, EN_EVALMATL_AVG, Pmcvy, Pmcvz) ) { return FALSE; }

    const BOOL bUseHook = GetUseSeisHookWall(ElemK);
    const int nSeisCompType = Tool.GetSeisComponentType(ElemK);
    const int nPMMethod = Tool.GetPMCalcMethodWall(ElemK, EN_EVALSTEP_2ND);

    for ( int i=0; i<EN_MEMBEND_NUM; ++i )
    {
        const EN_WALL_FORC_POS& CurPos = GetWallForcPos(static_cast<EN_MEMBEND>(i));
        const T_WALL_SHR_PARAM& CurShrAvg = aShrAvg[i];
        
        const double dPu = PORslt.dForce[CurPos][DOF_FX]*(-1.0);
        const double dVu = PORslt.dForce[CurPos][DOF_FZ];
        const double dMu = PORslt.dForce[CurPos][DOF_MY];

        const double dMe = CalcMn(nPMMethod, Pmcvy, dPu, dMu);
        const double dAxlIdx = CalcAxialIndex(dPu, dFck, dAg);
        const double dShrIdx = CalcShearIndexWall(dVu, CurShrAvg);
        const double dVe     = CalcVnConWall(CurShrAvg);
        EN_EVALCTRL enCtrlType = CalcWallCtrlType(dMe, dVe, dHw, CurShrAvg);

        T_PHGT_FEMA& rCurPhgt = i==EN_MEMBEND_I ? rPhgtI : rPhgtJ;
        double dDy = GetThetay(PORslt, CurPos, DOF_MY);
        CalcMyHingePropConWallPos(nSeisCompType, enCtrlType, bBE, dAxlIdx, dShrIdx, dDy, rCurPhgt);
    }

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcMzHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ)
{
    T_WALL_KEY WallKey;
    T_WALL_D WallD;  WallD.Initialize();
    if (!m_pDoc->m_pPostCtrl->GetWallDataFromElemK(ElemK, WallD, &WallKey))  {  return FALSE; }
    if ( !WallD.bLineWall ) { return FALSE; }

    T_THIK_D ThikD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    if (!GetElemMatlThik(ElemK, ThikD, MatdD, SecmD)) { ASSERT(FALSE); return FALSE; }

    double dTw = WallD.dThickness;
    double dLw = WallD.dLength;
    double dHw = WallD.dHeight;

    CSeisEvalDataTool Tool;
    double dFck = Tool.GetFc(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);
    double dAg  = dTw*dLw;

    /// 벽체 약축 휨은 휨지배로 가정 (// 기둥도 모멘트 성분은 휨지배로만 고려.)
    T_WALL_SHR_PARAM aShrAvg[EN_MEMBEND_NUM];
    T_WALL_SHR_PARAM aShrNom[EN_MEMBEND_NUM];
    if ( !MakeWallShearParam(ElemK, EN_EVALSTEP_2ND, EN_DIR_Y, aShrAvg, aShrNom) ) { return FALSE; }
    SetWallForcShearParam(PORslt.dForce, EN_DIR_Y, aShrAvg, aShrNom);

    BOOL bUseHook = GetUseSeisHookWall(ElemK);
    int nSeisCompType = Tool.GetSeisComponentType(ElemK);

    _DB_PMCV_DATA Pmcvy;
    _DB_PMCV_DATA Pmcvz;
    if (!GetPmcvData(ElemK, EN_EVALSTEP_2ND, EN_EVALMATL_AVG, Pmcvy, Pmcvz)) { return FALSE; }
    
    for (int i=0; i<EN_MEMBEND_NUM; ++i)
    {
        const EN_WALL_FORC_POS& enPos = GetWallForcPos(static_cast<EN_MEMBEND>(i));
        const T_WALL_SHR_PARAM& CurShr = aShrAvg[i];

        const double dPu = PORslt.dForce[enPos][DOF_FX]*(-1.0);
        const double dVu = PORslt.dForce[enPos][DOF_FY];
        const double dMu = PORslt.dForce[enPos][DOF_MZ];

        T_CONCOLM_INFO Info;
        Info.bSeisConform = IsTransReinfConforming(bUseHook, dVu, CurShr.Comm);
        Info.dAxlIdx = CalcAxialIndex(dPu, dFck, dAg);
        Info.dShrIdx = CalcShearIndex(dVu, CurShr.Comm);
        Info.enCtrlType = EN_EVALCTRL_FLEX;  // 기둥도 모멘트 성분은 휨지배로만 고려.
        //double dVn = CalcVnConWall(CurShr);
        //EN_EVALCTRL enCtrl = CalcColmCtrlType(dMezI, dMezJ, dHw, dVn);

        T_UPHG_CONCOLM_DIR Uphg;
        Uphg.Param.dThetay = GetThetay(PORslt, enPos, DOF_MZ);
        Uphg.Info = Info;

        T_PHGT_FEMA& rCurPhgt = ( i == EN_MEMBEND_I ) ? rPhgtI : rPhgtJ;

        rCurPhgt.bSymmetric  = FALSE;
        rCurPhgt.bUserDefineRatio = TRUE;	
        rCurPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_STIFF_DEGRADE;
        /// 벽체 약축의 힌지 modeling parameter 는 기둥을 따르도록 합니다. (/w 이상현 professor)
        CalcMyzHingePropConColmPos(nSeisCompType, Uphg, rCurPhgt);
    }

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcMyHingePropStlBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ)
{
    double adDy[2] = { 0.0 };
    for (int i=0; i<2; ++i)
    {
        adDy[i] = GetThetay(PORslt, i, DOF_MY);
    }

    T_MATD_D MatdD;
    T_SECT_D SectD;
    T_SESM_D SesmD;

    if (!GetElemMatlSectStl(ElemK, SectD, MatdD, SesmD)) { ASSERT(0); return FALSE; }

    CSeisEvalDataTool Tool;
    int nSeisCompType = Tool.GetSeisComponentType(ElemK);
    const double dFye = Tool.GetFyStl(EN_EVALSTEP_2ND, EN_EVALMATL_AVG, SesmD); //### 두께별 강도를 고려할 필요는 없는가?

    double adBTR[2] = { 0.0, };
    double adDTR[2] = { 0.0, };	
    CalcWTRLimitStlBeam(SectD.SectBefore.Shape, MatdD.Data1.Analysis.Elast, dFye, adBTR, adDTR);

    auto l_CalcHingeParam = [&](int nCompType, const T_SECT_SECTBASE_D& SectBaseD, double dThetayPos, OUT T_PHGT_FEMA& rPhgtPos)
    {
        rPhgtPos.bSymmetric = FALSE;
        rPhgtPos.bUserDefineRatio = TRUE;
        rPhgtPos.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_NORMAL_BILINEAR;

        double dBTR = 0.0;
        double dDTR = 0.0;
        CalcWTR(SectD.SectBefore.Shape, SectBaseD, dBTR, dDTR);

        double adParam[6] = { 0.0, };
        CalcParamStlBeam(nCompType, dBTR, dDTR, adBTR, adDTR, dThetayPos, adParam);

        rPhgtPos.dYieldMoment[0] = dFye*SectBaseD.Design.Zyy;
        rPhgtPos.dYieldMoment[1] = rPhgtPos.dYieldMoment[0];

        SetCurveDispCtrlStl(adParam, rPhgtPos.dForceMomentRatio);
        SetAccetanceCriteria(adParam[HG_IO], adParam[HG_LS], adParam[HG_CP], rPhgtPos);
    };


    l_CalcHingeParam(nSeisCompType, GetSectBaseEndI(SectD), adDy[0], rPhgtFemaI);
    l_CalcHingeParam(nSeisCompType, GetSectBaseEndJ(SectD), adDy[1], rPhgtFemaJ);
    
    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcFxHingePropStlColm(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ)
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
        rCurPhgt.dIO[IDX_MNS] = 0.5;
        rCurPhgt.dLS[IDX_PLS] = 4.0;  /// 1.0+3.0
        rCurPhgt.dLS[IDX_MNS] = 0.75;
        rCurPhgt.dCP[IDX_PLS] = 6.0;  /// 1.0+5.0
        rCurPhgt.dCP[IDX_MNS] = 1.0;
    };

    l_CalcHinge(SectBaseI, dSlenI, rPhgtFemaI);
    l_CalcHinge(SectBaseJ, dSlenJ, rPhgtFemaJ);

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcMyzHingePropStlColm(T_ELEM_K ElemK, UINT unDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ)
{
    // PO 해석결과는 현재 단위계이므로 코드 단위계로 변환해 주어야 한다.
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
        rCurPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_NORMAL_BILINEAR;

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
            rCurPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_NORMAL_BILINEAR; /// [PMS 5695-136] 전단지배는 Unloading Stiffness Type을 Bilinear로. 
            SetCurveForcCtrlShear(rCurPhgt.dForceMomentRatio);
            adParam[HG_IO] = 0.50;
            adParam[HG_LS] = 0.75;
            adParam[HG_CP] = 1.00;		
        }
        else
        {
            CalcParamStlColm(nSeisCompType, enColmType, dkp, dBTR, dDTR, adBTR, adDTR, dThetay, adParam);
            SetCurveDispCtrlStl(adParam, rCurPhgt.dForceMomentRatio);			
        }

        rCurPhgt.dYieldMoment[0] = dFye * (unDir == EN_DIR_Y ? SectBase.Design.Zyy : SectBase.Design.Zzz);
        rCurPhgt.dYieldMoment[1] = rCurPhgt.dYieldMoment[0];

        SetAccetanceCriteria(adParam[HG_IO], adParam[HG_LS], adParam[HG_CP], rCurPhgt);
    };

    double dSlenI = CalcStlSlenderness(ElemK, 0, SectBaseI);
    double dSlenJ = CalcStlSlenderness(ElemK, 4, SectBaseJ);

    l_CalcHinge(SectBaseI, dSlenI, adP[0], adDy[0], rPhgtFemaI);
    l_CalcHinge(SectBaseJ, dSlenJ, adP[1], adDy[1], rPhgtFemaJ);

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcFxHingePropStlBrac(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ)
{
    T_MATD_D MatdD;
    T_SECT_D SectD;
    T_SESM_D SesmD;

    if (!GetElemMatlSectStl(ElemK, SectD, MatdD, SesmD)) { ASSERT(0); return FALSE; }

    if ( !IsSectShapeStlBrac(SectD.SectBefore.Shape) )
    {
        GSaveHistoryFormat(_LS(IDS_DGN_SE_ERROR_NOT_UPDATE_ELEM_BY_SECT_INFO), ElemK);
        return FALSE;
    }

    CSeisEvalDataTool Tool;
    double dE   = MatdD.Data1.Analysis.Elast;
    double dFy  = Tool.GetFyStl(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SesmD); //## 두께별 강도를 고려하고 있지 않다.
    double dFye = Tool.GetFyStl(EN_EVALSTEP_2ND, EN_EVALMATL_AVG, SesmD);

    int nSeisCompType = Tool.GetSeisComponentType(ElemK);

    const T_SECT_SECTBASE_D& SectBaseI = GetSectBaseEndI(SectD);
    const T_SECT_SECTBASE_D& SectBaseJ = GetSectBaseEndJ(SectD);

    double dSlenI = CalcStlSlenderness(ElemK, 0, SectBaseI);
    double dSlenJ = CalcStlSlenderness(ElemK, 4, SectBaseJ);	

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
        CalcParamStlBracComp(nSeisCompType, unBuck, strShape, dBTR, dDTR, adBTR, adDTR, adParamC);

        double adParamT[6] = { 0.0, };	
        CalcParamStlBracTens(nSeisCompType, adParamT);

        SetCurveDispCtrlStl(adParamT, adParamC, rCurPhgt.dForceMomentRatio);
        SetAccetanceCriteria(adParamT[HG_IO], adParamT[HG_LS], adParamT[HG_CP], adParamC[HG_IO], adParamC[HG_LS], adParamC[HG_CP], rCurPhgt);

        rCurPhgt.dYieldForce[IDX_PLS] = dPnT;
        rCurPhgt.dYieldForce[IDX_MNS] = dPnC;
    };

    l_CalcHinge(EN_BRACBUCK_OUT, SectD.SectBefore.Shape, SectBaseI, dSlenI, rPhgtFemaI);
    l_CalcHinge(EN_BRACBUCK_OUT, SectD.SectBefore.Shape, SectBaseJ, dSlenJ, rPhgtFemaJ);

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcMyHingePropSrcBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ)
{
    // PO 해석결과는 현재 단위계이므로 코드 단위계로 변환해 주어야 한다.
    double adDy[2] = { 0.0 };
    for (int i=0; i<2; ++i)
    {
        adDy[i] = GetThetay(PORslt, i, DOF_MY);
    }

    T_MATD_D MatdD;
    T_SECT_D SectD;
    T_SESM_D SesmD;
    T_SECM_D SecmD;
    if (!GetElemMatlSectSrc(ElemK, SectD, MatdD, SesmD, SecmD)) { ASSERT(FALSE); return FALSE; }

    CSeisEvalDataTool Tool;
    const double dFye = Tool.GetFyStl(EN_EVALSTEP_2ND, EN_EVALMATL_AVG, SesmD); //### 두께별 강도를 고려할 필요는 없는가?
    
    rPhgtFemaI.bSymmetric = FALSE;
    rPhgtFemaI.bUserDefineRatio = TRUE;

    int nMaterial = 0;
    GetBeamColumnHingeMatlType(ElemK, nMaterial);
    const BOOL bRC = nMaterial==0;
    rPhgtFemaI.nUnloadStiffnessType = bRC ? D_PHGT_FEMA_UNLD_STIFF_DEGRADE : D_PHGT_FEMA_UNLD_NORMAL_BILINEAR;
        
    double adParam[6] = { 0.0, };
    CalcParamSrcBeam(adDy[0], adParam);
    SetCurveDispCtrlStl(adParam, rPhgtFemaI.dForceMomentRatio);

    rPhgtFemaI.dYieldMoment[0] = dFye*SectD.SectBefore.SectI.Design.Zyy;
    rPhgtFemaI.dYieldMoment[1] = rPhgtFemaI.dYieldMoment[0];

    SetAccetanceCriteria(adParam[HG_IO], adParam[HG_LS], adParam[HG_CP], rPhgtFemaI);
    rPhgtFemaJ = rPhgtFemaI;

    CalcParamSrcBeam(adDy[1], adParam);
    SetCurveDispCtrlStl(adParam, rPhgtFemaJ.dForceMomentRatio);

    if (SectD.SectBefore.nStype == D_SECT_TYPE_TAPERED)
    {
        rPhgtFemaJ.dYieldMoment[0] = dFye*SectD.SectBefore.SectJ.Design.Zyy;
        rPhgtFemaJ.dYieldMoment[1] = rPhgtFemaJ.dYieldMoment[0];
    }
    else
    {
        rPhgtFemaJ.dYieldMoment[0] = rPhgtFemaI.dYieldMoment[0];
        rPhgtFemaJ.dYieldMoment[1] = rPhgtFemaI.dYieldMoment[1];
    }

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcFxHingePropSrcColm(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ)
{
    T_ELEM_D ElemD;
    if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(FALSE); return FALSE; }

    T_MATD_D MatdD;
    T_SECT_D SectD;
    T_SESM_D SesmD;
    T_SECM_D SecmD;	
    if (!GetElemMatlSectSrc(ElemK, SectD, MatdD, SesmD, SecmD)) { ASSERT(FALSE); return FALSE; }

    CSeisEvalDataTool Tool;
    double dFck  = Tool.GetFc(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);
    double dFy   = Tool.GetFyStl(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SesmD);
    double dFyr  = Tool.GetFym(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);

    double dFye  = Tool.GetFyStl(EN_EVALSTEP_2ND, EN_EVALMATL_AVG, SesmD);
    double dFyre = Tool.GetFym(EN_EVALSTEP_2ND, EN_EVALMATL_AVG, SecmD);

    T_REBS_D RebsD;
    if (!m_pDoc->m_pAttrCtrl->GetRebs(ElemD.elpro, RebsD)) { return FALSE; }
           
    const T_SECT_SECTBASE_D& SectBaseStl = SectD.SectBefore.SectI;
    const T_SECT_SECTBASE_D& SectBaseCon = SectD.SectBefore.SectJ;
    double dAsr = GetRebarAs(RebsD.MainRebarName) * RebsD.nQrb;

    auto l_CalcHinge = [&](const T_SECT_SECTBASE_D& SectBaseStl, const T_SECT_SECTBASE_D& SectBaseCon, OUT T_PHGT_FEMA& rCurPhgt)
    {
        rCurPhgt.bSymmetric = FALSE;
        rCurPhgt.bUserDefineRatio = TRUE;
        rCurPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;

        SetCurveForcCtrlAxial(rCurPhgt.dForceMomentRatio);

        double dAs = SectBaseStl.Stiffness.Area;		
        double dAc = SectBaseCon.Stiffness.Area - dAs - dAsr;

        rCurPhgt.dYieldForce[IDX_PLS] = CalcPnTensSrc(dFye, dAs, dFyre, dAsr);
        rCurPhgt.dYieldForce[IDX_MNS] = CalcPnCompSrc(dFck, dAc, dFy, dAs, dFyr, dAsr);

        /// 인장에 대한 허용기준은 <순수인장력을 받는 기둥>을 따라갑니다.
        rCurPhgt.dIO[IDX_PLS] = 1.25; /// 1.0+0.25
        rCurPhgt.dIO[IDX_MNS] = 0.5;
        rCurPhgt.dLS[IDX_PLS] = 4.0;  /// 1.0+3.0
        rCurPhgt.dLS[IDX_MNS] = 0.75;
        rCurPhgt.dCP[IDX_PLS] = 6.0;  /// 1.0+5.0
        rCurPhgt.dCP[IDX_MNS] = 1.0;
    };

    l_CalcHinge(SectBaseStl, SectBaseCon, rPhgtFemaI);

    rPhgtFemaJ = rPhgtFemaI;

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcMyzHingePropSrcColm(T_ELEM_K ElemK, UINT unDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ)
{
    // PO 해석결과는 현재 단위계이므로 코드 단위계로 변환해 주어야 한다.
    double adP[2]  = { 0.0, };
    double adM[2]  = { 0.0, };
    double adDy[2] = { 0.0, };

    UINT unDOF = unDir==EN_DIR_Y ? DOF_MY : DOF_MZ;
    for (int i=0; i<2; ++i)
    {
        adP[i]  = PORslt.dForce[i][DOF_FX];
        adM[i]  = PORslt.dForce[i][unDOF];
        adDy[i] = GetThetay(PORslt, i, unDOF);
    }

    T_ELEM_D ElemD;
    if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(FALSE); return FALSE; }

    T_MATD_D MatdD;
    T_SECT_D SectD;
    T_SESM_D SesmD;
    T_SECM_D SecmD;
    if (!GetElemMatlSectSrc(ElemK, SectD, MatdD, SesmD, SecmD)) { ASSERT(FALSE); return FALSE; } 

    CSeisEvalDataTool Tool;
    double dFck  = Tool.GetFc(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);
    double dFy   = Tool.GetFyStl(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SesmD); //## 두께별 강도를 고려하고 있지 않다.
    double dFyr  = Tool.GetFym(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);
    
    double dFye  = Tool.GetFyStl(EN_EVALSTEP_2ND, EN_EVALMATL_AVG, SesmD);    
    double dFyre = Tool.GetFym(EN_EVALSTEP_2ND, EN_EVALMATL_AVG, SecmD);

    double dFyInput  = MatdD.Data1.Design.S_Fy1;
    double dFckInput = MatdD.Data2.Design.C_fc;
    double dFymInput = GetRebarFy(MatdD.MainRebarData.B_fy);

    T_REBS_D RebsD;
    if (!m_pDoc->m_pAttrCtrl->GetRebs(ElemD.elpro, RebsD)) { return FALSE; }

    const T_SECT_SECTBASE_D& SectBaseStl = SectD.SectBefore.SectI;
    const T_SECT_SECTBASE_D& SectBaseCon = SectD.SectBefore.SectJ;

    double dAsr = GetRebarAs(RebsD.MainRebarName) * RebsD.nQrb;

    _DB_PMCV_DATA Pmcvy;
    _DB_PMCV_DATA Pmcvz;
    if (!GetPmcvData(ElemK, EN_EVALSTEP_2ND, EN_EVALMATL_AVG, Pmcvy, Pmcvz)) { return FALSE; }

    int nPMMethod = D_SEME_PMCM_P; ///## src는 어떻게 하나요?
   
    double dMeI = CalcMn(nPMMethod, Pmcvy, adP[0], adM[0]);
    double dMeJ = CalcMn(nPMMethod, Pmcvy, adP[1], adM[1]);

    auto l_CalcHinge = [&](const T_SECT_SECTBASE_D& SectBaseStl, const T_SECT_SECTBASE_D& SectBaseCon, double dP, double dThetay, OUT T_PHGT_FEMA& rCurPhgt)
    {
        rCurPhgt.bSymmetric = FALSE;
        rCurPhgt.bUserDefineRatio = TRUE;

        double dAs = SectBaseStl.Stiffness.Area;
        double dAc = SectBaseCon.Stiffness.Area - dAs - dAsr;

        double dPy = CalcPnCompSrc(dFck, dAc, dFy, dAs, dFyr, dAsr);
        EN_STLCOLM enColmType = GetStlColmForcType(dP, dPy);
        double dkp = CalcKp(dP, dPy);

        double adParam[6] = { 0.0, };
        if (enColmType == EN_STLCOLM_PGT05PY)
        {
            rCurPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
            SetCurveForcCtrlShear(rCurPhgt.dForceMomentRatio);
        }
        else
        {
            T_PHGE_K PhgeK;
            PhgeK.key.objK = ElemK;
            PhgeK.key.type = 1;
            int nMaterial = 0;
            BOOL bRC = GetHingeMatlType(PhgeK, nMaterial) == 0;
            if(bRC)
                rCurPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_STIFF_DEGRADE;
            else
                rCurPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_NORMAL_BILINEAR;

            CalcParamSrcColm(enColmType, dkp, dThetay, adParam);
            SetCurveDispCtrlStl(adParam, rCurPhgt.dForceMomentRatio);
        }

        SetAccetanceCriteria(adParam[HG_IO], adParam[HG_LS], adParam[HG_CP], rCurPhgt);
    };

    l_CalcHinge(SectBaseStl, SectBaseCon, adP[0], adDy[0], rPhgtFemaI);
    l_CalcHinge(SectBaseStl, SectBaseCon, adP[1], adDy[1], rPhgtFemaJ);

    rPhgtFemaI.dYieldMoment[IDX_PLS] = dMeI;
    rPhgtFemaI.dYieldMoment[IDX_MNS] = dMeI;
    rPhgtFemaJ.dYieldMoment[IDX_PLS] = dMeJ;
    rPhgtFemaJ.dYieldMoment[IDX_MNS] = dMeJ;
    
    return TRUE;	
}

BOOL CSeisEvalKISTEC2013::CalcEvalPreConColmSimple(T_ELEM_K ElemK, OUT T_EVALPRE_COLM& rEval)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rEval.Initialize();

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    if ( !GetElemMatlSectCon(ElemK, SectD, MatdD, SecmD) ) { ASSERT(0); return FALSE; }
    
    T_SECT_STIFFNESS* pStiff = CSectUtil::GetProperStiffPosition(SectD);
    if ( pStiff == nullptr ) { ASSERT(0); return FALSE; }
    if ( SectD.nStype==D_SECT_TYPE_TAPERED )
    {
        // 변단면일 경우 I,J 중 크기가 작은 단면의 값 사용
        T_SECT_STIFFNESS* pStiff_J = CSectUtil::GetProperStiffPosition(SectD, FALSE, 0, 'J');
        if ( pStiff->Area > pStiff_J->Area )
        {
            pStiff = pStiff_J;
        }
    }

    auto L_GetTimeFrame = [](T_SECM_D& SecmD, int nMethod) -> int 
    { 
        return SecmD.nStrengthMode==D_SECM_STRMODE_AUTO ? SecmD.method.GetDefault(nMethod)->nTimeFrame : SecmD.nTimeFrame; 
    }; 
    
    EN_COLM_CATEGORY enCateX = EN_COLM_CATEGORY::SHORT;
    EN_COLM_CATEGORY enCateY = EN_COLM_CATEGORY::SHORT;
    if ( !CalcColmCategory(ElemK, enCateX, enCateY) )  { ASSERT(0); return FALSE; }

    auto L_CalcVn =[]( EN_COLM_CATEGORY enCate, int nTime, double dAsc, OUT T_EVALPRE_COLM_DIR& rDir )
    {
        /// [KISTEC2013, table 3.3.2, Unit : MPa], [KISTEC2019, table 3.3.2]
        const double adAvgSS[3][4] = 
        {
            1.17, 1.23, 1.30, 1.41 ,
            0.71, 0.74, 0.79, 0.86 ,
            0.46, 0.47, 0.48, 0.53 
        };

        UINT unCate = static_cast<UINT>(enCate);
        rDir.dAsc = dAsc;
        rDir.dvsc = adAvgSS[unCate][nTime];
        rDir.CalcVn(enCate);
    };

    int nMethod = m_pDoc->m_pAttrCtrl2->GetSemeMethod();
    int nTimeFrame = L_GetTimeFrame(SecmD, nMethod);
	if(nTimeFrame < 0) nTimeFrame = 0;

    rEval.ElemK = ElemK;
    L_CalcVn(enCateX, nTimeFrame, pStiff->Area, rEval.DirX);
    L_CalcVn(enCateY, nTimeFrame, pStiff->Area, rEval.DirY);
    
    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcEvalPreConWallSimple(T_ELEM_K ElemK, OUT T_EVALPRE_WALL& rEval)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rEval.Initialize();

    T_ELEM_K_LIST aElemK_WallID;
    m_pDoc->m_pAttrCtrl->GetEqualWallIdElemList(aElemK_WallID, ElemK);

    double dTwmin = DBL_MAX;  /// 같은 wall id에 속하는 요소들 중, 가장 작은 두께를 사용합니다.
    double dLwX = 0.0;
    double dLwY = 0.0;
    int nSize = static_cast<int>(aElemK_WallID.GetSize());
    for ( int j=0; j<nSize; ++j )
    {
        T_ELEM_D ElemD_WallID;
        m_pDoc->m_pAttrCtrl->GetElem(aElemK_WallID[j], ElemD_WallID);

        T_NODE_D NodeD_I, NodeD_J;
        m_pDoc->m_pAttrCtrl->GetNode(ElemD_WallID.elnod[0], NodeD_I);
        m_pDoc->m_pAttrCtrl->GetNode(ElemD_WallID.elnod[1], NodeD_J);
        dLwX += fabs(NodeD_J.x - NodeD_I.x);
        dLwY += fabs(NodeD_J.y - NodeD_I.y);

        T_THIK_D ThikD;
        m_pDoc->m_pAttrCtrl->GetThik(ElemD_WallID.elpro, ThikD);
        if ( dTwmin > ThikD.Value.ThickIn ) 
        {
            dTwmin = ThikD.Value.ThickIn; 
        }
    }

    T_ELEM_K_LIST aElemK_Column;
    int nCol = m_pDoc->m_pAttrCtrl->GetWallInternalColumnKeyList(aElemK_WallID, aElemK_Column);
    ASSERT( nCol>=0 && nCol<=2 );

    auto L_CalcVn = []( double dLeng, double dThk, int nCol, OUT T_EVALPRE_WALL_DIR& rDir )
    {
        /// [KISTEC2013, table 3.3.3 Unit : MPa], [KISTEC2019, table 3.3.3]
        double dAvgSS = 0.0;
        switch( nCol )
        {
        case 2: dAvgSS = 3.0; break;
        case 1: dAvgSS = 2.0; break;
        case 0: dAvgSS = 1.0; break;
        }
        
        rDir.dLw  = dLeng;        
        rDir.dAsw = dThk * dLeng;
        rDir.dvsw = dAvgSS;
        rDir.CalcVn();
    };
    
    rEval.ElemK = ElemK;
    rEval.dTw   = dTwmin;
    rEval.nCol  = nCol;
    L_CalcVn( dLwX, dTwmin, nCol, rEval.DirX );
    L_CalcVn( dLwY, dTwmin, nCol, rEval.DirY );

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcEvalPreConColmGeneral(T_ELEM_K ElemK, OUT T_EVALPRE_COLM& rEval)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    CSeisEvalDataTool Tool;
    if ( !Tool.IsConsiderPreEval(ElemK) ) { return FALSE; }

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    if (!GetElemMatlSectCon(ElemK, SectD, MatdD, SecmD)) { ASSERT(0); return FALSE; }

    if (SecmD.bUseDefaultVal) return FALSE;

    _DB_PMCV_DATA Pmcvy;
    _DB_PMCV_DATA Pmcvz;
    if (!GetPmcvData(ElemK, EN_EVALSTEP_PRE, EN_EVALMATL_AVG, Pmcvy, Pmcvz)) { return FALSE; }

    /// 예비평가에서는 순수휨강도로 고려. (축력=0)
    int nPMMethod = D_SEME_PMCM_P;
    double dMey = CalcMnByAxial(Pmcvy, 0.0);
    double dMez = CalcMnByAxial(Pmcvz, 0.0);

    double dh0y = 0.0;
    double dh0z = 0.0;
    GetColmClearHeight(ElemK, dh0y, dh0z);
    double dVuyFlex = 2.0*dMez/dh0z;
    double dVuzFlex = 2.0*dMey/dh0y;

    T_COLM_SHR_PARAM aShry[EN_MEMBEND_NUM];
    MakeColmShearParamEval(ElemK, EN_EVALSTEP_PRE, EN_DIR_Y, aShry);

    T_COLM_SHR_PARAM aShrz[EN_MEMBEND_NUM];
    MakeColmShearParamEval(ElemK, EN_EVALSTEP_PRE, EN_DIR_Z, aShrz);

    /// member assign 및 부재력을 고려하지 않으므로, I단 정보만 사용해도 됩니다.
    const double dVny  = CalcVnConColm(aShry[EN_MEMBEND_I], 0.0, 0.0, 0.0, 0.0);
    const double dVnz  = CalcVnConColm(aShrz[EN_MEMBEND_I], 0.0, 0.0, 0.0, 0.0);

    const EN_EVALCTRL enCtrly = CalcColmCtrlType(dMey, dMey, dh0y, dVnz);
    const EN_EVALCTRL enCtrlz = CalcColmCtrlType(dMez, dMez, dh0z, dVny);

    /// 부재 local에 대해 전단강도를 계산하고, 전체좌표계로 변환 (mathUCS2GCSAsABS) = CStoryDisplDrift class와 동일하게 계산.
    T_ELEM_D ElemD;
    if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); return FALSE; }
    double dLocalVec[3][3] = { 0.0, };
    if (!m_pDoc->calcElemLocalVector(ElemD, dLocalVec)) { ASSERT(0); return FALSE; }

    double dPUX = 0.0, dPUY = dVuyFlex, dPUZ = dVuzFlex;
    mathUCS2GCSAsABS(dPUX, dPUY, dPUZ, dLocalVec);

    double dPNX = 0.0, dPNY = dVny, dPNZ = dVnz;
    mathUCS2GCSAsABS(dPNX, dPNY, dPNZ, dLocalVec);

    UINT unAxisType = GetColmLocalAxisType(ElemK);
    switch (unAxisType)
    {
    case EN_MEMBAXIS_LYGX: 
        rEval.DirX.unAct = enCtrlz;
        rEval.DirY.unAct = enCtrly;
        break;
    case EN_MEMBAXIS_LYGY: 
        rEval.DirX.unAct = enCtrly; 
        rEval.DirY.unAct = enCtrlz;
        break;
    case EN_MEMBAXIS_LYNONE:
        rEval.DirX.unAct = rEval.DirY.unAct = max(enCtrly, enCtrlz); break; /// local과 global이 평행하지 않으면, 불리한 action으로 부여.
    default: ASSERT(FALSE); break;
    }

    rEval.ElemK = ElemK;

    rEval.DirX.dVp = dPUX;
    rEval.DirX.dVn = dPNX;
    rEval.DirX.SetCfCs();

    rEval.DirY.dVp = dPUY;
    rEval.DirY.dVn = dPNY;
    rEval.DirY.SetCfCs();

    return TRUE;
}

 BOOL CSeisEvalKISTEC2013::CalcEvalPreConWallGeneral(T_ELEM_K ElemK, OUT T_EVALPRE_WALL& rEval)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    CSeisEvalDataTool Tool;
    if ( !Tool.IsConsiderPreEval(ElemK) ) return FALSE;

    T_WALL_SHR_PARAM aShrAvg[EN_MEMBEND_NUM];
    T_WALL_SHR_PARAM aShrShr[EN_MEMBEND_NUM];
    if ( !MakeWallShearParam(ElemK, EN_EVALSTEP_PRE, EN_DIR_Z, aShrAvg, aShrShr) ) { return FALSE; }

    double dVny = 0.0;
    double dVnz = CalcVnConWall(aShrAvg[EN_MEMBEND_I]);

    /// 부재 local에 대해 전단강도를 계산하고, 전체좌표계로 변환 (mathUCS2GCSAsABS) = CStoryDisplDrift class와 동일하게 계산.
    T_ELEM_D ElemD;
    if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); return FALSE; }
    double dLocalVec[3][3] = { 0.0, };
    if (!m_pDoc->calcElemLocalVector(ElemD, dLocalVec)) { ASSERT(0); return FALSE; }

    double dLocalVecWall[3][3] = { 0.0, }; // wall일때 local vector 변경 (see CStoryDisplDrift 참고)
    for(int i=0; i<3; ++i) 
    {
        for(int j=0; j<3; ++j) 
        {
            dLocalVecWall[i][j] = dLocalVec[i][j];
        }
    }

    dLocalVec[0][0] = dLocalVecWall[1][0];
    dLocalVec[0][1] = dLocalVecWall[1][1];
    dLocalVec[0][2] = dLocalVecWall[1][2];
    dLocalVec[1][0] = dLocalVecWall[2][0];
    dLocalVec[1][1] = dLocalVecWall[2][1];
    dLocalVec[1][2] = dLocalVecWall[2][2];
    dLocalVec[2][0] = dLocalVecWall[0][0];
    dLocalVec[2][1] = dLocalVecWall[0][1];
    dLocalVec[2][2] = dLocalVecWall[0][2];

    double dPNX = 0.0, dPNY = dVny, dPNZ = dVnz;
    mathUCS2GCSAsABS(dPNX, dPNY, dPNZ, dLocalVec);

    rEval.ElemK = ElemK;
    rEval.DirX.dVn = rEval.DirX.dCs = dPNX;
    rEval.DirY.dVn = rEval.DirY.dCs = dPNY;

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcSeisEval1stRsltConBeam(T_ELEM_K ElemK, OUT T_EVAL1ST_BEAM& raEval1st)
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
        
    raEval1st.bEvaluate = TRUE;
        
    T_BEAM_SHR_PARAM aShr[EN_BEAM_POS_NUM];
    MakeBeamShearParam(ElemK, EN_EVALSTEP_1ST, aShr);

    const bool bUseHook = Tool.IsUseSeisHook(ElemK);

    CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();    
    const double dL = pMembCtrl->GetMembLengByIncludeElem(ElemK);
    CalcForc1stLcom(ElemK, pMembCtrl);

    T_MFAC_D MfacD;
    BOOL bUserMfac = m_pDoc->m_pAttrCtrl->GetMfac(ElemK, MfacD);

    T_STRB_D StrbDisp, StrbForc;
    double dMaxDCR = -cEvalUpOn;

    T_EVAL1ST_BEAM_LCOM EvalLcom;    
    T_LCOM_K_LIST aLcomK;
    int nSize = GetLcomKeyList1stEval(ElemK, aLcomK);
    raEval1st.aEvalLcom.SetSize(nSize);
            
    for ( int i = 0 ; i < nSize ; ++i )
    {
        T_LCOM_K LcomK = aLcomK[i];		
        if (!GetEvalStrb1st(pMembCtrl, ElemK, LcomK, StrbDisp, StrbForc)) { continue; }

        double adVuD[2] = { StrbDisp.dblForce[0][2], StrbDisp.dblForce[3][ 8] };
        double adMuD[2] = { StrbDisp.dblForce[0][4], StrbDisp.dblForce[3][10] };	
        double adVuF[2] = { StrbForc.dblForce[0][2], StrbForc.dblForce[3][ 8] };
        double adMuF[2] = { StrbForc.dblForce[0][4], StrbForc.dblForce[3][10] };	

        T_CONBEAM_INFO aInfo[EN_MEMBEND_NUM];
        MakeBeamInfo(ElemK, EN_EVALSTEP_1ST, adVuF, aInfo);

        EvalLcom.Initialize();
        EvalLcom.LcomK = LcomK;	

        for ( int m = 0 ; m < EN_MEMBEND_NUM ; ++m )
        {
            const EN_BEAM_POS& enPos = GetBeamPos(static_cast<EN_MEMBEND>(m));
            T_EVAL1ST_BEAM_POS& rCurPos = m==EN_MEMBEND_I ? EvalLcom.EndI : EvalLcom.EndJ;

            const T_CONBEAM_INFO& Info = aInfo[m];
            const T_CON_SHR_PARAM& CurShr = aShr[enPos].GetParam(FALSE);        
            
            rCurPos.dMUD = adMuD[m];
            rCurPos.dVUD = adVuD[m];
            rCurPos.dMUF = adMuF[m];
            rCurPos.dVUF = adVuF[m];
            rCurPos.dMeP = Info.FlexBot.dMe;
            rCurPos.dMeN = Info.FlexTop.dMe;
            rCurPos.dVn  = CalcVnConBeam(CurShr);
            rCurPos.Info = aInfo[m];

            if (bUserMfac)
            {
                rCurPos.MfacM.Set(bUserMfac, MfacD.dMFactor1);
            }

            rCurPos.Info.GetRhoIdx(adMuD[m] < 0.0 ? false : true);

            CalcEval1stRsltConBeamPos(nSeisCompType, CurShr.IsSpaceGT05Dv(), rCurPos);
        }      

        EvalLcom.enLevel = max(EvalLcom.EndI.enLevel, EvalLcom.EndJ.enLevel);

        if ( raEval1st.enLevel <= EvalLcom.enLevel )
        { 
            const double dCurDCR = EvalLcom.GetDCR(EvalLcom.enLevel);
            if ( dgn::ME(dCurDCR, dMaxDCR) )
            {
                dMaxDCR = dCurDCR;
                raEval1st.RepLcomK = LcomK;
            }            
        }

        raEval1st.aEvalLcom.SetAt(i, EvalLcom);
        raEval1st.enLevel = max(raEval1st.enLevel, EvalLcom.enLevel);        
    }

    GetSEResult()->SetResultSE1stConBeam(ElemK, raEval1st);

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcSeisEval1stRsltConColm(T_ELEM_K ElemK, OUT T_EVAL1ST_COLM& raEval1st)
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

    T_LCOM_K_LIST aLcomK;
    int nSize = GetLcomKeyList1stEval(ElemK, aLcomK);	
    raEval1st.aEvalLcom.SetSize(nSize);
    if ( nSize < 1 ) { return FALSE; }

    const double dFck = Tool.GetFc(EN_EVALSTEP_1ST, EN_EVALMATL_NOM, SecmD);
    const double dFym = Tool.GetFym(EN_EVALSTEP_1ST, EN_EVALMATL_NOM, SecmD);
            
    double dAlpha = GetAlphaConColm(SectD);

    T_COLM_SHR_PARAM aShry[EN_MEMBEND_NUM];
    MakeColmShearParamEval(ElemK, EN_EVALSTEP_1ST, EN_DIR_Y, aShry);

    T_COLM_SHR_PARAM aShrz[EN_MEMBEND_NUM];
    MakeColmShearParamEval(ElemK, EN_EVALSTEP_1ST, EN_DIR_Z, aShrz);

    const double dAg = SectD.SectBefore.SectI.Stiffness.Area;
    const double dFactorPnmax = GetFactorPnmaxColm(EN_EVALSTEP_1ST, RebcD.nHoopType);
    const double dAs  = GetRebarAs(RebcD.MainRebarName) * RebcD.nQrb;
    const double dPnC = CalcPnCompCon(dFck, dAg, dFym, dAs, dFactorPnmax);
    const double dPnT = CalcPnTensCon(dFym, dAs);
    
    _DB_PMCV_DATA Pmcvy;
    _DB_PMCV_DATA Pmcvz;
    if (!GetPmcvData(ElemK, EN_EVALSTEP_1ST, EN_EVALMATL_AVG, Pmcvy, Pmcvz)) { return FALSE; }

    CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();
    CalcForc1stLcom(ElemK, pMembCtrl);
    
    const double dL = pMembCtrl->GetMembLengByIncludeElem(ElemK);
    const bool bUseHook = Tool.IsUseSeisHook(ElemK);

    T_MFAC_D MfacD;
    BOOL bUserMfac = m_pDoc->m_pAttrCtrl->GetMfac(ElemK, MfacD);
    const int nPMMethod = Tool.GetPMCalcMethodColm(ElemK, EN_EVALSTEP_1ST);
        
    T_LCOM_D LcomD;
    T_STRB_D StrbDisp, StrbForc;    
    double dMaxDCR = -cEvalUpOn;
    T_EVAL1ST_COLM_LCOM EvalLcom;

    for (int i=0; i<nSize; ++i)
    {
        T_LCOM_K LcomK = aLcomK[i];		
        if ( !GetEvalStrb1st(pMembCtrl, ElemK, LcomK, StrbDisp, StrbForc) ) { continue; }

        EvalLcom.Initialize();
        EvalLcom.LcomK = LcomK;	

        SetEvalConColm1stForc(StrbDisp, StrbForc, EvalLcom.PosI, EvalLcom.PosJ);

        double dMeyi = CalcMn(nPMMethod, Pmcvy, EvalLcom.PosI.dPUF, EvalLcom.PosI.dMUDy);
        double dMezi = CalcMn(nPMMethod, Pmcvz, EvalLcom.PosI.dPUF, EvalLcom.PosI.dMUDz); 
        double dMeyj = CalcMn(nPMMethod, Pmcvy, EvalLcom.PosJ.dPUF, EvalLcom.PosJ.dMUDy);
        double dMezj = CalcMn(nPMMethod, Pmcvz, EvalLcom.PosJ.dPUF, EvalLcom.PosJ.dMUDz); 

        for ( int m=0; m<EN_MEMBEND_NUM; ++m)
        {
            T_EVAL1ST_COLM_POS& rCurPos = ( m == EN_MEMBEND_I ) ? EvalLcom.PosI : EvalLcom.PosJ;
            if ( bUserMfac )
            {
                rCurPos.Diry.MfacM.Set(bUserMfac, MfacD.dMFactor1);
                rCurPos.Dirz.MfacM.Set(bUserMfac, MfacD.dMFactor2);
            }

            const double dPu  = rCurPos.dPUF;
            const double dVuy = rCurPos.dVUFy;
            const double dVuz = rCurPos.dVUFz;
            const double dMuy = rCurPos.dMUDy;
            const double dMuz = rCurPos.dMUDz;

            const T_COLM_SHR_PARAM& CurShry = aShry[m];
            const T_COLM_SHR_PARAM& CurShrz = aShrz[m];

            const double dMey = m==EN_MEMBEND_I ? dMeyi : dMeyj;
            const double dMez = m==EN_MEMBEND_I ? dMezi : dMezj;
            const double dmuy = CalcDutilityFactor(dMuy, dMey);
            const double dmuz = CalcDutilityFactor(dMuz, dMez);
            const double dVny = CalcVnConColm(CurShry, dPu, dVuy, dMuz, dmuz);
            const double dVnz = CalcVnConColm(CurShrz, dPu, dVuz, dMuy, dmuy);
            const double dAxialIndex = CalcAxialIndex(dPu, dFck, dAg);

            rCurPos.bForcCtrlP = true;  /// KISTEC 2013 의 기둥 축력은 무조건 하중지배.
            rCurPos.dPnC = dPnC;
            rCurPos.dPnT = dPnT;
            rCurPos.dMey = dMey;
            rCurPos.dMez = dMez;
            rCurPos.dVny = dVny;
            rCurPos.dVnz = dVnz;

            T_CONCOLM_INFO& rInfoY = rCurPos.Diry.Info; /// MY-VZ
            rInfoY.bSeisConform = IsTransReinfConforming(bUseHook, dVuz, CurShrz.Comm);
            rInfoY.dAxlIdx      = dAxialIndex;
            rInfoY.dShrIdx      = CalcShearIndex(dVuz, CurShrz.Comm);            
            rInfoY.enCtrlType   = CalcColmCtrlType(dMeyi, dMeyj, dL, dVnz);
            rInfoY.dMei = dMeyi;
            rInfoY.dMej = dMeyj;
            rInfoY.dMe  = dMey;
            rInfoY.dL   = dL;
            rInfoY.dVp  = fabs(dL) < cEvalZero ? 0.0 : (dMeyi + dMeyj)/dL;
            rInfoY.dVn  = dVnz;
            rInfoY.dS   = CurShrz.Comm.dSv;
            rInfoY.dd   = CurShrz.Comm.dDv;
            CalcEval1stRsltConColmDir(nSeisCompType, rCurPos.Diry);

            T_CONCOLM_INFO& rInfoZ = rCurPos.Dirz.Info;  /// MZ-VY
            rInfoZ.bSeisConform = IsTransReinfConforming(bUseHook, dVuy, CurShry.Comm);
            rInfoZ.dAxlIdx      = dAxialIndex;
            rInfoZ.dShrIdx      = CalcShearIndex(dVuy, CurShry.Comm);            
            rInfoZ.enCtrlType   = CalcColmCtrlType(dMezi, dMezj, dL, dVny);
            rInfoZ.dMei = dMezi;
            rInfoZ.dMej = dMezj;
            rInfoZ.dMe  = dMez;
            rInfoZ.dL   = dL;
            rInfoZ.dVp  = fabs(dL) < cEvalZero ? 0.0 : (dMezi + dMezj)/dL;
            rInfoZ.dVn  = dVny;
            rInfoZ.dS   = CurShry.Comm.dSv;
            rInfoZ.dd   = CurShry.Comm.dDv;
            CalcEval1stRsltConColmDir(nSeisCompType, rCurPos.Dirz);

            CalcEval1stConColmPos(dAlpha, rCurPos);            
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

        raEval1st.aEvalLcom.SetAt(i, EvalLcom);		
        raEval1st.enLevel = max(raEval1st.enLevel, EvalLcom.enLevel);		
    }

    GetSEResult()->SetResultSE1stConColm(ElemK, raEval1st);

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcSeisEval1stRsltConWall(T_ELEM_K ElemK, OUT T_EVAL1ST_WALL& raEval1st)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    raEval1st.Initialize();

    T_WALL_KEY WallKey;
    T_WALL_D WallD;  WallD.Initialize();
    if (!m_pDoc->m_pPostCtrl->GetWallDataFromElemK(ElemK, WallD, &WallKey))  {  return FALSE; }
    if (!WallD.bLineWall ) { return FALSE; }

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
        
    T_REBW_K RebwK;
    RebwK.keymap = WallKey;
    T_REBW_D RebwD;
    if (!m_pDoc->m_pAttrCtrl->GetRebw(RebwK, RebwD)) { return FALSE; }

    raEval1st.bEvaluate = TRUE;

    const double dTw = WallD.dThickness;
    const double dLw = WallD.dLength;
    const double dHw = WallD.dHeight;
    const double dAg = dTw*dLw;

    const double dFck = Tool.GetFc(EN_EVALSTEP_1ST, EN_EVALMATL_NOM, SecmD);
    const bool   bBE  = Tool.IsConsiderBoundaryElement(RebwD);

    T_WALL_SHR_PARAM aShrAvg[EN_MEMBEND_NUM];
    T_WALL_SHR_PARAM aShrNom[EN_MEMBEND_NUM];
    MakeWallShearParam(ElemK, EN_EVALSTEP_1ST, EN_DIR_Z, aShrAvg, aShrNom);

    T_MFAC_D MfacD;
    BOOL bUserMfac = m_pDoc->m_pAttrCtrl->GetMfac(ElemK, MfacD);

    raEval1st.bBE = bBE;

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
    if ( !m_pDoc->m_pPostCtrl->GetInternalWallId(ElemK, unInternalWallID) ) { ASSERT(FALSE); return FALSE; }

    T_LCOM_D LcomD;
    T_STRW_D StrwDisp, StrwForc;
    double dMaxDCR = -cEvalUpOn;

    T_EVAL1ST_WALL_LCOM EvalLcom;
    T_LCOM_K_LIST aLcomK;
    int nSize = GetLcomKeyList1stEval(ElemK, aLcomK);
    raEval1st.aEvalLcom.SetSize(nSize);

    const int nPMMethod = Tool.GetPMCalcMethodWall(ElemK, EN_EVALSTEP_1ST);

    for ( int i=0; i<nSize; ++i )
    {
        T_LCOM_K LcomK = aLcomK[i];

        if ( !GetEvalStrw1st(pMembCtrl, unInternalWallID, LcomK, StrwDisp, StrwForc) ) { ASSERT(FALSE); continue; }

        EvalLcom.Initialize();
        EvalLcom.LcomK = LcomK;
        SetEvalConWall1stForc(StrwDisp, StrwForc, EvalLcom.PosI, EvalLcom.PosJ);

        for ( int m=0; m<EN_MEMBEND_NUM; ++m )
        {
            const EN_WALL_FORC_POS& enPos = GetWallForcPos(static_cast<EN_MEMBEND>(m));

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
            Info.dShrIdx = CalcShearIndexWall(dVUD, CurShrAvg);
            Info.enCtrlType = CalcWallCtrlType(dMe, dVe, dHw, CurShrAvg);
            Info.bForceCtrl = (Info.enCtrlType == EN_EVALCTRL_SHER && Info.dAxlIdx > 0.15) ? true : false;

            CalcMFactConWall(nSeisCompType, Info, rCurLcomPos);
            CalcEval1stConWallPos(rCurLcomPos);
        }

        EvalLcom.enLevel  = max(EvalLcom.PosI.enLevel, EvalLcom.PosJ.enLevel);

        if ( raEval1st.enLevel < EvalLcom.enLevel )
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

BOOL CSeisEvalKISTEC2013::CalcSeisEval1stRsltStlBeam(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBEAM& raEval1st)
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

    const double dFye = Tool.GetFyStl(EN_EVALSTEP_1ST, EN_EVALMATL_AVG, SesmD);
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
        CalcEval1stRsltStlBeamPos(nSeisCompType, RsltLcom.EndI, dBTRI, dDTRI, adBTR, adDTR, EvalLcom.EndI);
        if(bUserMfac) EvalLcom.EndJ.SetUserMfac(bUserMfac, MfacD.dMFactor1);
        CalcEval1stRsltStlBeamPos(nSeisCompType, RsltLcom.EndJ, dBTRJ, dDTRJ, adBTR, adDTR, EvalLcom.EndJ);

        EvalLcom.enLevel  = max(EvalLcom.EndI.enLevel, EvalLcom.EndJ.enLevel);
        raEval1st.enLevel = max(raEval1st.enLevel, EvalLcom.enLevel);
    }

    return TRUE;	
}

BOOL CSeisEvalKISTEC2013::CalcSeisEval1stRsltStlColm(T_ELEM_K ElemK, OUT T_EVAL1ST_STLCOLM& raEval1st)
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

    const CString& strSectShape = SectD.SectBefore.Shape;

    const double dE   = MatdD.Data1.Analysis.Elast;
    const double dFy  = Tool.GetFyStl(EN_EVALSTEP_1ST, EN_EVALMATL_NOM, SesmD);
    const double dFye = Tool.GetFyStl(EN_EVALSTEP_1ST, EN_EVALMATL_AVG, SesmD);

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

BOOL CSeisEvalKISTEC2013::CalcSeisEval1stRsltStlBrac(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBRAC& raEval1st)
{
    SetCodeUnit();

    raEval1st.Initialize();
    raEval1st.ElemK = ElemK;

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SESM_D SesmD;
    if (!GetElemMatlSectStl(ElemK, SectD, MatdD, SesmD)) return FALSE;
    if (SesmD.bUseDefaultVal) { return FALSE; }

    CSeisEvalDataTool Tool;
    int nSeisCompType = Tool.GetSeisComponentType(ElemK);
    if (nSeisCompType == D_SCMT_TYPE_NONE) { return FALSE; }

    T_EVAL1ST_STLRSLT StlRslt;
    if (!CheckStlSectFor1stEval(ElemK, StlRslt)) { return FALSE; }

    raEval1st.bEvaluate = TRUE;

    int nSize = static_cast<int>(StlRslt.aRsltLcom.GetSize());
    raEval1st.aEvalLcom.SetSize(nSize);

    double dE   = MatdD.Data1.Analysis.Elast;
    double dFye = Tool.GetFyStl(EN_EVALSTEP_1ST, EN_EVALMATL_AVG, SesmD);
    const CString& strSectShape = SectD.SectBefore.Shape;

    const T_SECT_SECTBASE_D& SectBaseI = GetSectBaseEndI(SectD);
    const T_SECT_SECTBASE_D& SectBaseJ = GetSectBaseEndJ(SectD);
    UINT unBuckling = BUCK_IN;  //###

    T_MFAC_D MfacD;
    BOOL bUserMfac = m_pDoc->m_pAttrCtrl->GetMfac(ElemK, MfacD);

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
        CalcEval1stStlBracPos(nSeisCompType, strSectShape, SectBaseI, dFye, adBTR, adDTR, unBuckling, RsltLcom.EndI, EvalLcom.PosI);
        if (bUserMfac) { EvalLcom.PosJ.SetUserMFac(TRUE, (RsltLcom.EndJ.dPUD < 0.0 ? TRUE : FALSE), MfacD.dMFactor1, MfacD.dMFactor2); }
        CalcEval1stStlBracPos(nSeisCompType, strSectShape, SectBaseJ, dFye, adBTR, adDTR, unBuckling, RsltLcom.EndJ, EvalLcom.PosJ);	

        EvalLcom.enLevel  = max(EvalLcom.PosI.DCR.enLevel, EvalLcom.PosJ.DCR.enLevel);
        raEval1st.enLevel = max(raEval1st.enLevel, EvalLcom.enLevel);
    }

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcSeisEval1stRsltSrcBeam(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBEAM& raEval1st)
{
    SetCodeUnit();

    raEval1st.Initialize();
    raEval1st.ElemK = ElemK;

    T_MATD_D MatdD;
    T_SECT_D SectD;
    T_SESM_D SesmD;
    T_SECM_D SecmD;
    if (!GetElemMatlSectSrc(ElemK, SectD, MatdD, SesmD, SecmD)) { ASSERT(FALSE); return FALSE; }
    if (SesmD.bUseDefaultVal || SecmD.bUseDefaultVal) { return FALSE; }

    CSeisEvalDataTool Tool;
    int nSeisCompType = Tool.GetSeisComponentType(ElemK);
    if (nSeisCompType == D_SCMT_TYPE_NONE) { return FALSE; }

    raEval1st.bEvaluate = TRUE;

    _DB_PMCV_DATA Pmcvy;
    _DB_PMCV_DATA Pmcvz;
    if (!GetPmcvData(ElemK, EN_EVALSTEP_2ND, EN_EVALMATL_AVG, Pmcvy, Pmcvz)) { return FALSE; }

    /// SRC 보 : 순수 휨강도 계산.
    double dMey = CalcMnByAxial(Pmcvy, 0.0);
    
    CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();
    CalcForc1stLcom(ElemK, pMembCtrl);

    T_STRB_D StrbDisp, StrbForc;

    T_EVAL1ST_STLBEAM_LCOM EvalLcom;
    T_LCOM_K_LIST aLcomK;
    int nSize = GetLcomKeyList1stEval(ElemK, aLcomK);	
    raEval1st.aEvalLcom.SetSize(nSize);

    for (int i=0; i<nSize; ++i)
    {
        T_LCOM_K LcomK = aLcomK[i];		
        if (!GetEvalStrb1st(pMembCtrl, ElemK, LcomK, StrbDisp, StrbForc)) { ASSERT(0); continue; }

        EvalLcom.Initialize();
        EvalLcom.LcomK = LcomK;	

        EvalLcom.EndI.dMu = StrbDisp.dblForce[0][4];
        EvalLcom.EndI.dMe = dMey;
        CalcEval1stRsltSrcBeamPos(nSeisCompType, EvalLcom.EndI);

        EvalLcom.EndJ.dMu = StrbDisp.dblForce[3][10];
        EvalLcom.EndJ.dMe = dMey;
        CalcEval1stRsltSrcBeamPos(nSeisCompType, EvalLcom.EndJ);

        EvalLcom.enLevel = max(EvalLcom.EndI.enLevel, EvalLcom.EndJ.enLevel);

        raEval1st.aEvalLcom.SetAt(i, EvalLcom);
        raEval1st.enLevel = max(raEval1st.enLevel, EvalLcom.enLevel);		
    }

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcSeisEval1stRsltSrcColm(T_ELEM_K ElemK, OUT T_EVAL1ST_STLCOLM& raEval1st)	
{
    SetCodeUnit();

    raEval1st.Initialize();
    raEval1st.ElemK = ElemK;

    T_MATD_D MatdD;
    T_SECT_D SectD;
    T_SESM_D SesmD;
    T_SECM_D SecmD;
    if (!GetElemMatlSectSrc(ElemK, SectD, MatdD, SesmD, SecmD)) { ASSERT(0); return FALSE; }
    if (SesmD.bUseDefaultVal || SecmD.bUseDefaultVal) { return FALSE; }
        
    CSeisEvalDataTool Tool;
    int nSeisCompType = Tool.GetSeisComponentType(ElemK);
    if (nSeisCompType == D_SCMT_TYPE_NONE) { return FALSE; }

    raEval1st.bEvaluate = TRUE;
    
    _DB_PMCV_DATA PmcvyAvg;
    _DB_PMCV_DATA PmcvzAvg;
    if (!GetPmcvData(ElemK, EN_EVALSTEP_1ST, EN_EVALMATL_AVG, PmcvyAvg, PmcvzAvg)) { return FALSE; }

    double dMey = CalcMnByAxial(PmcvyAvg, 0.0);
    double dMez = CalcMnByAxial(PmcvzAvg, 0.0);
    
    _DB_PMCV_DATA PmcvyNom;
    _DB_PMCV_DATA PmcvzNom;
    if (!GetPmcvData(ElemK, EN_EVALSTEP_1ST, EN_EVALMATL_NOM, PmcvyNom, PmcvzNom)) { return FALSE; }
       
    double dMny = CalcMnByAxial(PmcvyNom, 0.0);
    double dMnz = CalcMnByAxial(PmcvzNom, 0.0);
    
    double dFck  = Tool.GetFc(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);
    double dFy   = Tool.GetFyStl(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SesmD);
    double dFyr  = Tool.GetFym(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);

    double dFye  = Tool.GetFyStl(EN_EVALSTEP_2ND, EN_EVALMATL_AVG, SesmD);
    double dFyre = Tool.GetFym(EN_EVALSTEP_2ND, EN_EVALMATL_AVG, SecmD);

    T_ELEM_D ElemD;
    if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); return FALSE; }

    T_REBS_D RebsD;
    if (!m_pDoc->m_pAttrCtrl->GetRebs(ElemD.elpro, RebsD)) { return FALSE; }

    const T_SECT_SECTBASE_D& SectBaseStl = SectD.SectBefore.SectI;
    const T_SECT_SECTBASE_D& SectBaseCon = SectD.SectBefore.SectJ;
    double dAsr = GetRebarAs(RebsD.MainRebarName) * RebsD.nQrb;

    const double dAs = SectBaseStl.Stiffness.Area;		
    const double dAc = SectBaseCon.Stiffness.Area - dAs - dAsr;

    CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();
    CalcForc1stLcom(ElemK, pMembCtrl);

    T_STRB_D StrbDisp, StrbForc;

    T_EVAL1ST_STLCOLM_LCOM EvalLcom;
    T_LCOM_K_LIST aLcomK;
    int nSize = GetLcomKeyList1stEval(ElemK, aLcomK);
    raEval1st.aEvalLcom.SetSize(nSize);

    for (int i=0; i<nSize; ++i)
    {
        T_LCOM_K LcomK = aLcomK[i];		
        if (!GetEvalStrb1st(pMembCtrl, ElemK, LcomK, StrbDisp, StrbForc)) { ASSERT(FALSE); continue; }

        EvalLcom.Initialize();
        EvalLcom.LcomK = LcomK;	

        const double dPnC = CalcPnCompSrc(dFck, dAc, dFy, dAs, dFyr, dAsr);
        const double dPnT = CalcPnTensSrc(dFye, dAs, dFyre, dAsr);

        SetStlForc(TRUE, StrbDisp, StrbForc, EvalLcom.PosI);
        EvalLcom.PosI.SetStrn(dPnC, dMny, dMnz, dPnT, dMey, dMez);
        CalcEval1stSrcColmPos(nSeisCompType, EvalLcom.PosI.dPUF, EvalLcom.PosI.dPCL, EvalLcom.PosI);

        SetStlForc(FALSE, StrbDisp, StrbForc, EvalLcom.PosJ);
        EvalLcom.PosJ.SetStrn(dPnC, dMny, dMnz, dPnT, dMey, dMez);
        CalcEval1stSrcColmPos(nSeisCompType, EvalLcom.PosJ.dPUF, EvalLcom.PosJ.dPCL, EvalLcom.PosJ);

        EvalLcom.enLevel = max(EvalLcom.PosI.DCR.enLevel, EvalLcom.PosJ.DCR.enLevel);

        raEval1st.aEvalLcom.SetAt(i, EvalLcom);
        raEval1st.enLevel = max(raEval1st.enLevel, EvalLcom.enLevel);		
    }

    return TRUE;
}

double CSeisEvalKISTEC2013::GetAlphaConColm(const T_SECT_D& SectD)
{
    return 2.0; 
}

void CSeisEvalKISTEC2013::SetAccetanceCriteriaForcCtrl(OUT T_PHGT_FEMA& rPhgtD)
{
    SetAccetanceCriteria(0.5, 0.75, 1.0, rPhgtD);
}

void CSeisEvalKISTEC2013::SetAccetanceCriteriaConAxial(OUT T_PHGT_FEMA& rPhgtD)
{
    SetAccetanceCriteria(2.0, 4.0, 6.0, 0.5, 0.75, 1.0, rPhgtD);
}

double CSeisEvalKISTEC2013::CalcK1(const T_COLM_SHR_PARAM& ColmShr)
{
    return CalcK1_EVAL(ColmShr.Comm);    
}

double CSeisEvalKISTEC2013::CalcVsmaxFram(const T_CON_SHR_PARAM& Shr)
{    
    return 2.0 * sqrt(Shr.dFck) / 3.0 * Shr.dBv * Shr.dDv;
}

double CSeisEvalKISTEC2013::GetMFactForc(const EN_LEVEL enLevel)
{
    switch (enLevel)
    {
    case EN_LEVEL_IO: return 0.5;
    case EN_LEVEL_LS: return 0.75;
    case EN_LEVEL_CP: return 1.0;
    case EN_LEVEL_CL: return 1.0;         
    default: ASSERT(0); return 1.0;
    }
}

EN_LEVEL CSeisEvalKISTEC2013::GetPerformLevelForcCtrl(const double dDCR[3])
{
    if (dDCR[EN_LEVEL_CP] > 1.0) { return EN_LEVEL_CL; }
    if (dDCR[EN_LEVEL_LS] > 1.0) { return EN_LEVEL_CP; }
    if (dDCR[EN_LEVEL_IO] > 1.0) { return EN_LEVEL_LS; }
    return EN_LEVEL_IO;
}

BOOL CSeisEvalKISTEC2013::CalcMyzHingePropConColmPos(int nSeisCompType, OUT T_UPHG_CONCOLM_DIR& rUphg, OUT T_PHGT_FEMA& rPhgt)
{
    rPhgt.bSymmetric = FALSE;
    rPhgt.bUserDefineRatio = TRUE;	

    const T_CONCOLM_INFO& Info = rUphg.Info;

    double adTable[6] = { 0.0, };
    double adParam[6] = { 0.0, };

    switch ( Info.enCtrlType )
    {
    case EN_EVALCTRL_FLEX:
        {
            T_MEMBPARAM_TABLE_2 Table2;
            GetParamTable_ConColm_CtrlFlex(nSeisCompType, Info.bSeisConform, Table2);

            CalcParamTable2(Table2, Info.dAxlIdx, Info.dShrIdx, adTable);
            CalcHingeParamConFromAB(rUphg.Param.dThetay, adTable, adParam);

            rPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_STIFF_DEGRADE;
            SetCurveDispCtrl(adParam, rPhgt.dForceMomentRatio);
        }
        break;
    case EN_EVALCTRL_SHER:
        {
            adParam[HG_IO] = 0.50;
            adParam[HG_LS] = 0.75;
            adParam[HG_CP] = 1.0;

            rPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
            SetCurveForcCtrlShear(rPhgt.dForceMomentRatio);
        }
        break;    
    default: ASSERT(0);
        break;
    }

    SetAccetanceCriteria(adParam, rPhgt);
    SetUphgParam(adTable, adParam, rUphg.Param);

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcColmCategory(T_ELEM_K ElemK, OUT EN_COLM_CATEGORY& renCateX, OUT EN_COLM_CATEGORY& renCateY)
{
    /// 계산 흐름 정리를 위해서 CTBSeisEvaluationDataMgr 에서 이동.
    // 단면 X,Y 방향 폭    
    const double dVecGlobalX[3] = { 0.0, 1.0, 0.0 };
    const double dVecGlobalY[3] = { 1.0, 0.0, 0.0 };
    double dPos[1] = {0.0}, dWidthX[1], dWidthY[1];
    CSectUtil::GetBeamElemSectWidth(ElemK, dVecGlobalX, 1, dPos, dWidthX);
    double dDX = dWidthX[0];
    CSectUtil::GetBeamElemSectWidth(ElemK, dVecGlobalY, 1, dPos, dWidthY);
    double dDY = dWidthY[0];
    
    double dH0X = 0.0;
    double dH0Y = 0.0;
    CalcColmLHeightForGlobalAxis(ElemK, dH0X, dH0Y);

    auto l_GetCatetory =[](double dD, double dh0) -> EN_COLM_CATEGORY
    {
        double dVal = fabs(dD) < cEvalZero ? 0.0 : dh0/dD;
        if (dVal < 2.0) { return EN_COLM_CATEGORY::SHORT;  }
        if (dVal < 6.0) { return EN_COLM_CATEGORY::NORMAL; }        
        return EN_COLM_CATEGORY::LONG;
    };
    
    renCateX = l_GetCatetory(dDX, dH0X);
    renCateY = l_GetCatetory(dDY, dH0Y);

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcColmLHeightForGlobalAxis(T_ELEM_K ElemK, OUT double& rdH0X, OUT double& rdH0Y)
{
    rdH0X = 0.0;
    rdH0Y = 0.0;

    T_ELEM_D ElemD;
    if ( !m_pDoc->m_pAttrCtrl->GetElem(ElemK,ElemD) ) { ASSERT(0); return FALSE; }
    T_SECH_D SechD;
    if ( m_pDoc->m_pAttrCtrl->GetSech(ElemK,SechD) )
    {
        // PMS 5531 : 내진성능평가 부재별 Parameter 입력 > 기둥 순높이
        // - global 축과 기둥 부재의 local축이 나란한 경우에는 해당 방향의 길이를 적용하여 계산.																			
        // - global 축과 기둥 부재의 local축이 나란하지 않은 경우에는 																			
        //   (a) 대상 global 축에 각도가 가까운 방향의 길이를 적용한다.																			
        //   (b) (a)에서 각도가 동일한 경우, h0y, h0z 중 짧은 길이를 적용한다.																			
        double dAngleNormal = fmod(fabs(ElemD.angle), 180.0);
        if ( fabs(dAngleNormal - 45.0) < 1.e-8 || fabs(dAngleNormal - 135.0) < 1.e-8 )
        {
            rdH0X = rdH0Y = min(SechD.dh0y, SechD.dh0z);
        }
        else if(dAngleNormal < 45.0 || dAngleNormal > 135.0)
        {
            rdH0X = SechD.dh0z;
            rdH0Y = SechD.dh0y;
        }
        else
        {
            rdH0X = SechD.dh0y;
            rdH0Y = SechD.dh0z;
        }
    }
    else
    {
        CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();
        rdH0X = rdH0Y = pMembCtrl->GetMembLengByIncludeElem(ElemK);
    }
    return TRUE;
}

BOOL CSeisEvalKISTEC2013::IsTransReinfConforming(BOOL bUseSeisHook, double dV, const T_CON_SHR_PARAM& Shr)
{
    if ( !bUseSeisHook )
    { 
        return FALSE; 
    }

    if ( Shr.dSv > Shr.dDv/3.0 )
    {
        return FALSE; 
    }

    const double dVs = CalcVsFram(Shr);
    return ( dVs < fabs(dV)*3.0/4.0 ) ? FALSE : TRUE;    
}

BOOL CSeisEvalKISTEC2013::CalcMyHingePropConBeamPos(int nSeisCompType, OUT T_UPHG_CONBEAM_POS& rUphg, OUT T_PHGT_FEMA& rPhgt)
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
            GetParamTable_ConBeam_CtrlFlex(nSeisCompType, Info.bSeisConform, Table2);

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
            GetParamTable_ConBeam_CtrlSher(nSeisCompType, Info.IsSpaceGTd2(), adTable);

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


double CSeisEvalKISTEC2013::CalcPnTensCon(double dfy, double dAs)
{
    return dfy*dAs;
}

double CSeisEvalKISTEC2013::CalcPnCompCon(double dfck, double dAg, double dfy, double dAs, double dFactorPnmax)
{
    return dFactorPnmax*(0.85*dfck*(dAg - dAs) + dfy*dAs);
}

double CSeisEvalKISTEC2013::CalcVnConColm(const T_COLM_SHR_PARAM& Shr, double dPu, double dVu, double dMu, double dmu)
{
    const double dd = Shr.Comm.dDv;
    const double dS = Shr.Comm.dSv;
    const double dfck = Shr.Comm.dFck;
    const double dAg  = Shr.dAg;
    const double dLambda = Shr.Comm.dLambda;

    double dK1 = CalcK1(Shr);
    double dK2 = CalcK2(dmu);
    double dMuVud = CalcMuPerVud(dMu, dVu, dd);
    double dAppPu = dPu < 0.0 ? 0.0 : fabs(dPu); /// 인장력이 작용하는 경우에는 0.0으로 본다.

    double dVs = dK1 * CalcVsFram(Shr.Comm);
    double dVc = dLambda*dK2*(0.5*sqrt(dfck)/(dMuVud)*sqrt(1.0 + dAppPu/(0.5*sqrt(dfck)*dAg)))*0.8*dAg;
    return dVs + dVc;
}

double CSeisEvalKISTEC2013::CalcK2(double dmu)
{
    if (dmu < 2.0) return 1.0;
    if (dmu > 4.0) return 0.7;
    return -0.15*dmu + 1.3;
}

double CSeisEvalKISTEC2013::CalcMuPerVud(double dMu, double dVu, double dd)
{
    double dMuPerVud = fabs(dVu*dd) < cEvalZero? 3.0 : fabs(dMu/(dVu*dd));
    return min(max(2.0, dMuPerVud), 3.0);
}

double CSeisEvalKISTEC2013::CalcVnConWall(const T_WALL_SHR_PARAM& Shr)
{
    const double dfck = Shr.Comm.dFck;
    const double dd   = Shr.Comm.dDv;
    const double dTw  = Shr.Comm.dBv;
    const double dLw  = Shr.dLw;
    const double dPu  = Shr.dP;
    const double dVu  = Shr.dV;
    const double dMu  = Shr.dM;

    double dVc = fabs(dLw) < cEvalZero ? 0.0 : 0.28*sqrt(dfck)*dTw*dd + dPu*dd/(4.0*dLw); // dPu : (+) compression, (-) tension.
    double dMV = fabs(dVu) < cEvalZero ? 0.0 : fabs(dMu/dVu);
    if (dMV - dLw/2.0 > 0.0)
    {
        double dVc2 = (0.05*sqrt(dfck) + dLw*(0.1*sqrt(dfck) + 0.2*dPu/(dLw*dTw))/(dMV - dLw/2.0))*dTw*dd;
        dVc = min(dVc, dVc2);
    }	
    if (dVc < 0.0) dVc = 0.0;

    double dVs = CalcVsWall(Shr, dVc);
    return dVc + dVs;
}

bool CSeisEvalKISTEC2013::MakeBeamInfo(T_ELEM_K ElemK, EN_EVALSTEP enStep, double adV[2], OUT T_CONBEAM_INFO raInfo[EN_MEMBEND_NUM])
{
    for (int i=0; i<EN_MEMBEND_NUM; ++i)
    {
        raInfo[i].Initialize();
    }

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    if (!GetElemMatlSectCon(ElemK, SectD, MatdD, SecmD)) { ASSERT(0); return false; } 
    if (SecmD.bUseDefaultVal) { return false; }

    double dB = 0.0;
    double dH = 0.0;
    if ( !GetSectDim(SectD, dB, dH) ) { ASSERT(0); return false; }

    T_REBB_D RebbD;
    if (!m_pDoc->m_pAttrCtrl->GetRebbByElemK(ElemK, RebbD)) { ASSERT(0); return false; }

    CSeisEvalDataTool Tool;
    const double dFck  = Tool.GetFc(enStep, EN_EVALMATL_NOM, SecmD);
    const double dFce  = Tool.GetFc(enStep, EN_EVALMATL_AVG, SecmD);
    const double dFyme = Tool.GetFym(enStep, EN_EVALMATL_AVG, SecmD);
    const double dL    = CMembCtrl::GetMembCtrl()->GetMembLengByIncludeElem(ElemK);

    const bool bUseHook = Tool.IsUseSeisHook(ElemK);
    int nSeisCompType = Tool.GetSeisComponentType(ElemK);

    T_BEAM_SHR_PARAM aShr[EN_BEAM_POS_NUM];
    MakeBeamShearParam(ElemK, enStep, aShr);

    CalcConBeamFlex(dB, dH, dFce, dFyme, RebbD.Top_I, RebbD.dT, raInfo[EN_MEMBEND_I].FlexTop);
    CalcConBeamFlex(dB, dH, dFce, dFyme, RebbD.Bot_I, RebbD.dB, raInfo[EN_MEMBEND_I].FlexBot);
    CalcConBeamFlex(dB, dH, dFce, dFyme, RebbD.Top_J, RebbD.dT, raInfo[EN_MEMBEND_J].FlexTop);
    CalcConBeamFlex(dB, dH, dFce, dFyme, RebbD.Bot_J, RebbD.dB, raInfo[EN_MEMBEND_J].FlexBot);

    for (int i=0; i<EN_MEMBEND_NUM; ++i)
    {
        const EN_BEAM_POS& enPos = GetBeamPos(static_cast<EN_MEMBEND>(i));
        const T_CON_SHR_PARAM& CurShr = aShr[enPos].GetParam(FALSE);

        T_REBB_BASE RebbTop;
        T_REBB_BASE RebbBot;
        if ( !GetRebbBase(RebbD, enPos, RebbTop, RebbBot) ) { continue; }

        const double dCurV = adV[i];
        T_CONBEAM_INFO& Info = raInfo[i];

        CalcConBeamRho(dB, dH, RebbBot, RebbD.dB, Info.FlexBot);
        CalcConBeamRho(dB, dH, RebbTop, RebbD.dT, Info.FlexTop);

        Info.bSeisConform = IsTransReinfConforming(bUseHook, dCurV, CurShr);
        Info.dRhoBal = CalcConBeamRhoBal(dFck, dFyme);        
        Info.dShrIdx = CalcShearIndex(dCurV, CurShr);
        Info.CalcRhoIdx();

        Info.dL = dL;
        Info.adMeTop[EN_MEMBEND_I] = raInfo[EN_MEMBEND_I].FlexTop.dMe;
        Info.adMeTop[EN_MEMBEND_J] = raInfo[EN_MEMBEND_J].FlexTop.dMe;
        Info.adMeBot[EN_MEMBEND_I] = raInfo[EN_MEMBEND_I].FlexBot.dMe;
        Info.adMeBot[EN_MEMBEND_J] = raInfo[EN_MEMBEND_J].FlexBot.dMe;

        Info.dVp = CalcVpConBeam(Info.adMeTop, Info.adMeBot, dL);
        Info.dVc = CalcVcConBeam(CurShr);
        Info.dVs = CalcVsFram(CurShr);
        Info.dVn = CalcVnConBeam(CurShr);

        Info.enCtrlType = CalcBeamCtrlType(Info.dVp, Info.dVn);
        Info.dSv = CurShr.dSv;
        Info.ddv = CurShr.dDv;
    }
    return true;
}

BOOL CSeisEvalKISTEC2013::CalcMyHingePropConWallPos(int nSeisCompType, EN_EVALCTRL enCtrlType, BOOL bExistBE, double dAxialIndex, double dShearIndex, double dThetay, OUT T_PHGT_FEMA& rPhgt)
{	
    rPhgt.bSymmetric = FALSE;
    rPhgt.bUserDefineRatio = TRUE;

    EN_EVALCURV enCurvType = EN_EVALCURV_DISP;

    double adTable[6] = { 0.0, };
    double adParam[6] = { 0.0, };
    if (enCtrlType == EN_EVALCTRL_SHER)
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
        double adTable[6] = { 0.0, };
        GetParamTable_ConWall_CtrlSher(nSeisCompType, dAxialIndex, adTable);
        CalcHingeParamConFromDE(dThetay, adTable, adParam);
        
        enCurvType = EN_EVALCURV_DISPRATIO;
        //}
    }
    else
    {
        T_MEMBPARAM_TABLE_2 Table2;
        GetParamTable_ConWall_CtrlFlex(nSeisCompType, bExistBE, Table2);
        CalcParamTable2(Table2, dAxialIndex, dShearIndex, adTable);
        CalcHingeParamConFromAB(dThetay, adTable, adParam);
        
        enCurvType = EN_EVALCURV_DISP;
    }
    rPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_STIFF_DEGRADE;

    switch (enCurvType)
    {		
    case EN_EVALCURV_DISP:
    case EN_EVALCURV_DISPRATIO:
        {
            SetCurveDispCtrl(adParam, rPhgt.dForceMomentRatio);
        }
        break;
    case EN_EVALCURV_FORC:
        {
            //rPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_NORMAL_BILINEAR; /// [PMS 5695-136] 전단지배는 Unloading Stiffness Type을 Bilinear로. 
            SetCurveForcCtrlShear(rPhgt.dForceMomentRatio);
        }		
        break;
    default: ASSERT(FALSE);
        break;
    }

    SetAccetanceCriteria(adParam[HG_IO], adParam[HG_LS], adParam[HG_CP], rPhgt);

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcFzHingePropConWallPos(int nSeisCompType, double dAxialIndex, double dThetay, OUT T_PHGT_FEMA& rPhgt)
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
    // 	else
    // 	{	
    double adTable[6] = { 0.0, };
    GetParamTable_ConWall_CtrlSher(nSeisCompType, dAxialIndex, adTable);
    double adParam[6] = { 0.0, };
    CalcHingeParamConFromDE(dThetay, adTable, adParam);        
    //}

    rPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
    SetCurveDispCtrl(adParam, rPhgt.dForceMomentRatio);
    SetAccetanceCriteria(adParam[HG_IO], adParam[HG_LS], adParam[HG_CP], rPhgt);

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcParamStlBeam(int nSeisCompType, double dBTR, double dDTR, double adBTR[2], double adDTR[2], double dThetay, double radParam[6])
{
    memset(radParam, 0.0, sizeof(double)*6);

    double adParamSC[6] = { 9.0, 11.0, 0.6, 1.00, 6.0, 8.0 };
    double adParamNC[6] = { 4.0,  6.0, 0.2, 0.25, 2.0, 3.0 };
    if (nSeisCompType == D_SCMT_TYPE_SECONDARY)
    {
        adParamSC[HG_LS] = 9.0; adParamSC[HG_CP] = 11.0;
        adParamNC[HG_LS] = 3.0; adParamNC[HG_CP] =  4.0;
    }

    radParam[HG_A ] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, adParamSC[HG_A ], adParamNC[HG_A ]);
    radParam[HG_B ] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, adParamSC[HG_B ], adParamNC[HG_B ]);
    radParam[HG_C ] =       CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, adParamSC[HG_C ], adParamNC[HG_C ]);
    radParam[HG_IO] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, adParamSC[HG_IO], adParamNC[HG_IO]);
    radParam[HG_LS] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, adParamSC[HG_LS], adParamNC[HG_LS]);
    radParam[HG_CP] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, adParamSC[HG_CP], adParamNC[HG_CP]);	

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcWTRLimitStlBeam(const CString& strSectShape, double dE, double dFye, OUT double radBTR[2], OUT double radDTR[2])
{
    if (fabs(dFye) < cEvalZero) { ASSERT(FALSE); return FALSE; }

    if (strSectShape == D_SECT_SHAPE_REG_B)
    {
        CalcWTRLimitShapeB(dE, dFye, radBTR);
        memcpy(radDTR, radBTR, 2);
    }
    else if (strSectShape == D_SECT_SHAPE_REG_P)
    {
        CalcWTRLimitShapeP(dFye, radBTR);
        memcpy(radDTR, radBTR, 2);
    }
    else
    {
        radBTR[WTR_SC] = 0.30*sqrt(dE/dFye);
        radBTR[WTR_NC] = 0.38*sqrt(dE/dFye);

        radDTR[WTR_SC] = 2.45*sqrt(dE/dFye);
        radDTR[WTR_NC] = 3.76*sqrt(dE/dFye);
    }
    return TRUE;
}

void CSeisEvalKISTEC2013::CalcWTRLimitShapeB(double dE, double dFye, OUT double radWTR[])
{
    radWTR[WTR_SC] = 0.53*sqrt(dE/dFye);
    radWTR[WTR_NC] = 1.12*sqrt(dE/dFye);		
}

void CSeisEvalKISTEC2013::CalcWTRLimitShapeP(double dFye, OUT double radWTR[])
{
    radWTR[WTR_SC] = 10345.0/dFye;
    radWTR[WTR_NC] = 41380.0/dFye;		
}

double CSeisEvalKISTEC2013::CalcPnCompStl(double dAg, double dFy, double dE, double dSlen)
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

double CSeisEvalKISTEC2013::CalcPnTensStl(double dAg, double dFye)
{
    return dAg*dFye;
}

EN_STLCOLM CSeisEvalKISTEC2013::GetStlColmForcType(double dP, double dPy)
{
    /// (+)는 인장, (-)는 압축.
    if (dP > 0.0)           { return EN_STLCOLM_PLT02PY; } /// 인장인 경우에는 P<0.2Py로 고려한다.	
    if (fabs(dP) < 0.2*dPy) { return EN_STLCOLM_PLT02PY; }
    if (fabs(dP) < 0.5*dPy) { return EN_STLCOLM_PLT05PY; }

    return EN_STLCOLM_PGT05PY;	
}

double CSeisEvalKISTEC2013::CalcKp(double dP, double dPy)
{
    return 1.0 - 1.7*fabs(dP)/dPy;
}

BOOL CSeisEvalKISTEC2013::CalcWTRLimitStlColm(const CString& strSectShape, EN_STLCOLM enType, double dE, double dFye, OUT double radBTR[2], OUT double radDTR[2])
{
    memset(radBTR, 0.0, sizeof(double)*2);
    memset(radDTR, 0.0, sizeof(double)*2);

    if (fabs(dFye) < cEvalZero) { ASSERT(FALSE); return FALSE; }

    if (strSectShape == D_SECT_SHAPE_REG_B)
    {
        CalcWTRLimitShapeB(dE, dFye, radBTR);
        memcpy(radDTR, radBTR, 2);
    }
    else if (strSectShape == D_SECT_SHAPE_REG_P)
    {
        CalcWTRLimitShapeP(dFye, radBTR);
        memcpy(radDTR, radBTR, 2);
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

void CSeisEvalKISTEC2013::SetStlForc(BOOL bEndI, const T_STRB_D& StrbDisp, const T_STRB_D& StrbForc, OUT T_EVAL1ST_STLCOLM_POS& rColmPos)
{
    int nPart = bEndI ? 0 : 3;
    int nComp = bEndI ? 0 : 6;

    rColmPos.dPUD  = StrbDisp.dblForce[nPart][nComp];
    rColmPos.dMUDy = StrbDisp.dblForce[nPart][nComp + 4];
    rColmPos.dMUDz = StrbDisp.dblForce[nPart][nComp + 5];			

    rColmPos.dPUF  = StrbForc.dblForce[nPart][nComp];
    rColmPos.dMUFy = StrbForc.dblForce[nPart][nComp + 4];
    rColmPos.dMUFz = StrbForc.dblForce[nPart][nComp + 5];		
}

void CSeisEvalKISTEC2013::GetMFactStlColm(EN_STLCOLM enColmForcType, double dkp, int nSeisCompType, double dBTR, double dDTR,
                                          double adBTR[2], double adDTR[2], OUT double radmFac[3])
{	
	double adNC[3][2] = { 0.0, };
	double adSC[3][2] = { 0.0, };	
	GetMFactTableValueStlColm(enColmForcType, dkp, adSC, adNC);

    if ( enColmForcType == EN_STLCOLM_PGT05PY )
    {
        for ( int i = EN_LEVEL_IO ; i <= EN_LEVEL_CP ; ++i )
        {
            radmFac[i] = 1.0;
        }
    }
    else
    {
        for ( int i = EN_LEVEL_IO ; i <= EN_LEVEL_CP ; ++i )
        {
            radmFac[i] = CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, adSC[i][nSeisCompType], adNC[i][nSeisCompType]);            
        }
    }	
}

void CSeisEvalKISTEC2013::GetMFactStlColmTens(int nSeisCompType, OUT double radmFac[3])
{
    double adLS[2] = { 3.0, 6.0 };
    double adCP[2] = { 5.0, 7.0 };

    radmFac[EN_LEVEL_IO] = 1.25;
    radmFac[EN_LEVEL_LS] = adLS[nSeisCompType];
    radmFac[EN_LEVEL_CP] = adCP[nSeisCompType];
}

void CSeisEvalKISTEC2013::GetMFactTableValueStlColm(EN_STLCOLM enColmForcType, double dkp, OUT double ramFacSC[3][2], OUT double ramFacNC[3][2])
{
    for (int i=0; i<3; ++i)
    {
        for (int j=0; j<2; ++j)
        {
            ramFacSC[i][j] = 1.0;
            ramFacNC[i][j] = 1.0;
        }
    }

    switch (enColmForcType)
    {
    case EN_STLCOLM_PLT02PY:
        {
            ramFacSC[EN_LEVEL_IO][0] =  2.0; ramFacNC[EN_LEVEL_IO][0] = 1.25;		
            ramFacSC[EN_LEVEL_LS][0] =  6.0; ramFacNC[EN_LEVEL_LS][0] = 1.25;
            ramFacSC[EN_LEVEL_CP][0] =  8.0; ramFacNC[EN_LEVEL_CP][0] = 2.0;

            ramFacSC[EN_LEVEL_IO][1] =  2.0; ramFacNC[EN_LEVEL_IO][1] = 1.25;
            ramFacSC[EN_LEVEL_LS][1] = 10.0; ramFacNC[EN_LEVEL_LS][1] = 2.0;			
            ramFacSC[EN_LEVEL_CP][1] = 12.0; ramFacNC[EN_LEVEL_CP][1] = 3.0;
        }
        break;
    case EN_STLCOLM_PLT05PY:
        {
            ramFacSC[EN_LEVEL_IO][0] = 1.25;     ramFacNC[EN_LEVEL_IO][0] = 1.25;		
            ramFacSC[EN_LEVEL_LS][0] =  9.0*dkp; ramFacNC[EN_LEVEL_LS][0] = 1.25;
            ramFacSC[EN_LEVEL_CP][0] = 12.0*dkp; ramFacNC[EN_LEVEL_CP][0] = 1.5;

            ramFacSC[EN_LEVEL_IO][1] = 1.25;     ramFacNC[EN_LEVEL_IO][1] = 1.25;
            ramFacSC[EN_LEVEL_LS][1] = 15.0*dkp; ramFacNC[EN_LEVEL_LS][1] = 2.0;			
            ramFacSC[EN_LEVEL_CP][1] = 18.0*dkp; ramFacNC[EN_LEVEL_CP][1] = 2.0;
        }
        break;
    case EN_STLCOLM_PGT05PY:
        break;
    default: ASSERT(FALSE);
        break;
    }
}

BOOL CSeisEvalKISTEC2013::CalcParamStlColm(int nSeisCompType, EN_STLCOLM enColmForcType, double dkp, double dBTR, double dDTR, double adBTR[2], double adDTR[2], double dThetay, OUT double radParam[6])																		 
{
    memset(radParam, 0.0, sizeof(double)*6);

    double dParamSC[6] = { 0.0, };
    double dParamNC[6] = { 0.0, };
    GetTableValueStlColmParam(nSeisCompType, enColmForcType, dkp, dParamSC, dParamNC);

    radParam[HG_A ] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dParamSC[HG_A], dParamNC[HG_A]);
    radParam[HG_B ] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dParamSC[HG_B], dParamNC[HG_B]);
    radParam[HG_C ] = CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dParamSC[HG_C ], dParamNC[HG_C ]);
    radParam[HG_IO] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dParamSC[HG_IO], dParamNC[HG_IO]);
    radParam[HG_LS] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dParamSC[HG_LS], dParamNC[HG_LS]);
    radParam[HG_CP] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dParamSC[HG_CP], dParamNC[HG_CP]);		

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::GetTableValueStlColmParam(int nSeisCompType, EN_STLCOLM enColmForcType, double dkp, double dParamSC[6], double dParamNC[6])
{	
    BOOL bDispCtrl = TRUE;	
    switch (enColmForcType)
    {
    case EN_STLCOLM_PLT02PY:
        {
            dParamSC[HG_A ] =  9.0;  dParamNC[HG_A ] = 4.0;
            dParamSC[HG_B ] = 11.0;  dParamNC[HG_B ] = 6.0;
            dParamSC[HG_C ] =  0.6;  dParamNC[HG_C ] = 0.2;
            dParamSC[HG_IO] =  1.0;  dParamNC[HG_IO] = 0.25;

            if (nSeisCompType == D_SCMT_TYPE_PRIMARY)
            {
                dParamSC[HG_LS] =  6.0;  dParamNC[HG_LS] = 2.0;
                dParamSC[HG_CP] =  8.0;  dParamNC[HG_CP] = 3.0;
            }
            else
            {
                dParamSC[HG_LS] =  9.0;  dParamNC[HG_LS] = 3.0;
                dParamSC[HG_CP] = 11.0;  dParamNC[HG_CP] = 4.0;
            }
        }
        break;
    case EN_STLCOLM_PLT05PY:
        {			
            dParamSC[HG_A ] = 11.0*dkp;  dParamNC[HG_A ] = 1.0;
            dParamSC[HG_B ] = 17.0*dkp;  dParamNC[HG_B ] = 1.5;
            dParamSC[HG_C ] =  0.2;      dParamNC[HG_C ] = 0.2;
            dParamSC[HG_IO] =  0.25;     dParamNC[HG_IO] = 0.25;

            if (nSeisCompType == D_SCMT_TYPE_PRIMARY)
            {
                dParamSC[HG_LS] = 14.0*dkp;  dParamNC[HG_LS] = 1.2;
                dParamSC[HG_CP] = 17.0*dkp;  dParamNC[HG_CP] = 1.2;
            }
            else
            {
                dParamSC[HG_LS] =  8.0*dkp;  dParamNC[HG_LS] = 0.5;
                dParamSC[HG_CP] = 11.0*dkp;  dParamNC[HG_CP] = 0.8;
            }
        }
        break;
    default: 
        {			
            ASSERT(FALSE);
            bDispCtrl = FALSE;
        }		
        break;
    }
    return bDispCtrl;
}

BOOL CSeisEvalKISTEC2013::CalcParamStlBracComp(int nSeisCompType, UINT unBuck, const CString& strShape, double dBTR, double dDTR, double adBTR[2], double adDTR[2], OUT double radParam[6])
{
    memset(radParam, 0.0, sizeof(double)*6);

    if (strShape == D_SECT_SHAPE_REG_B || strShape == D_SECT_SHAPE_REG_SB)
    {
        return CalcParamStlBracCompShapeB(nSeisCompType, dBTR, dDTR, adBTR, adDTR, radParam);
    }
    if (strShape == D_SECT_SHAPE_REG_P || strShape == D_SECT_SHAPE_REG_SR)
    {
        return CalcParamStlBracCompShapeP(nSeisCompType, dBTR, dDTR, adBTR, adDTR, radParam);
    }
    if (strShape == D_SECT_SHAPE_REG_2L || strShape == D_SECT_SHAPE_REG_2C || strShape == D_SECT_SHAPE_REG_L || strShape == D_SECT_SHAPE_REG_C || strShape == D_SECT_SHAPE_REG_T || strShape == D_SECT_SHAPE_REG_UDT)
    {
        return CalcParamStlBracCompShape2L2C(nSeisCompType, unBuck, radParam);
    }
    if (strShape == D_SECT_SHAPE_REG_H)
    {
        return CalcParamStlBracCompShapeH(nSeisCompType, radParam);
    }	

    ASSERT(FALSE);
    return FALSE;
}

BOOL CSeisEvalKISTEC2013::CalcParamStlBracTens(int nSeisCompType, OUT double radParam[6])
{
    radParam[HG_A ] = 12.0;  // 1 + 11
    radParam[HG_B ] = 15.0;  // 1 + 14
    radParam[HG_C ] =  0.8;
    radParam[HG_IO] = 1.25;

    if (nSeisCompType == D_SCMT_TYPE_PRIMARY)
    {
        radParam[HG_LS] =  8.0;
        radParam[HG_CP] = 10.0;
    }
    else
    {
        radParam[HG_LS] = 12.0;
        radParam[HG_CP] = 14.0;
    }

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcParamStlBracCompShapeB(int nSeicCompType, double dBTR, double dDTR, double adBTR[2], double adDTR[2], OUT double radParam[6])
{
    memset(radParam, 0.0, sizeof(double)*6);

    double dParamSC[6] = { 0.5, 7.0, 0.4, 0.25, 4.0, 6.0 };
    double dParamNC[6] = { 0.5, 3.0, 0.2, 0.25, 1.0, 2.0 };

    if (nSeicCompType == D_SCMT_TYPE_SECONDARY)
    {
        dParamSC[HG_LS] = 6.0; dParamSC[HG_CP] = 7.0; 
        dParamNC[HG_LS] = 2.0; dParamNC[HG_CP] = 3.0; 
    }

    radParam[HG_A ] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dParamSC[HG_A], dParamNC[HG_A]);
    radParam[HG_B ] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dParamSC[HG_B], dParamNC[HG_B]);
    radParam[HG_C ] = CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dParamSC[HG_C ], dParamNC[HG_C ]);
    radParam[HG_IO] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dParamSC[HG_IO], dParamNC[HG_IO]);
    radParam[HG_LS] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dParamSC[HG_LS], dParamNC[HG_LS]);
    radParam[HG_CP] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dParamSC[HG_CP], dParamNC[HG_CP]);		

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcParamStlBracCompShapeP(int nSeicCompType, double dBTR, double dDTR, double adBTR[2], double adDTR[2], OUT double radParam[6])
{
    memset(radParam, 0.0, sizeof(double)*6);

    double dParamSC[6] = { 0.5, 9.0, 0.4, 0.25, 4.0, 6.0 };
    double dParamNC[6] = { 0.5, 3.0, 0.2, 0.25, 1.0, 2.0 };

    if (nSeicCompType == D_SCMT_TYPE_SECONDARY)
    {
        dParamSC[HG_LS] = 5.0; dParamSC[HG_CP] = 8.0; 
        dParamNC[HG_LS] = 2.0; dParamNC[HG_CP] = 3.0; 
    }

    radParam[HG_A ] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dParamSC[HG_A], dParamNC[HG_A]);
    radParam[HG_B ] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dParamSC[HG_B], dParamNC[HG_B]);
    radParam[HG_C ] = CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dParamSC[HG_C ], dParamNC[HG_C ]);
    radParam[HG_IO] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dParamSC[HG_IO], dParamNC[HG_IO]);
    radParam[HG_LS] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dParamSC[HG_LS], dParamNC[HG_LS]);
    radParam[HG_CP] = 1.0 + CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, dParamSC[HG_CP], dParamNC[HG_CP]);		

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcParamStlBracCompShape2L2C(int nSeisCompType, UINT unBuck, OUT double radParam[6])
{
    memset(radParam, 0.0, sizeof(double)*6);

    double adTableIN[6] = { 0.5, 9.0, 0.2, 0.25, 5.0, 7.0 };
    double adTableOT[6] = { 0.5, 8.0, 0.2, 0.25, 4.0, 6.0 };	

    if (nSeisCompType == D_SCMT_TYPE_SECONDARY)
    {
        adTableIN[HG_LS] = 6.0; adTableIN[HG_CP] = 7.0;
        adTableOT[HG_LS] = 7.0; adTableOT[HG_CP] = 8.0;
    }

    BOOL bCalc = TRUE;
    switch (unBuck)
    {
    case EN_BRACBUCK_IN:
        {
            radParam[HG_A ] = 1.0 + adTableIN[HG_A];
            radParam[HG_B ] = 1.0 + adTableIN[HG_B];
            radParam[HG_C ] = adTableIN[HG_C ];
            radParam[HG_IO] = 1.0 + adTableIN[HG_IO];
            radParam[HG_LS] = 1.0 + adTableIN[HG_LS];
            radParam[HG_CP] = 1.0 + adTableIN[HG_CP];		
        }		
        break;
    case EN_BRACBUCK_OUT:
        {
            radParam[HG_A ] = 1.0 + adTableOT[HG_A];
            radParam[HG_B ] = 1.0 + adTableOT[HG_B];
            radParam[HG_C ] = adTableOT[HG_C ];
            radParam[HG_IO] = 1.0 + adTableOT[HG_IO];
            radParam[HG_LS] = 1.0 + adTableOT[HG_LS];
            radParam[HG_CP] = 1.0 + adTableOT[HG_CP];			
        }		
        break;
    default: ASSERT(FALSE); bCalc = FALSE;
        break;
    }

    return bCalc;
}

BOOL CSeisEvalKISTEC2013::CalcParamStlBracCompShapeH(int nSeisCompType, OUT double radParam[6])
{
    double adTable[6] = { 0.5, 8.0, 0.2, 0.25, 5.0, 7.0 };
    if (nSeisCompType == D_SCMT_TYPE_SECONDARY)
    {
        adTable[HG_LS] = 7.0; adTable[HG_CP] = 8.0;
    }

    radParam[HG_A ] = 1.0 + adTable[HG_A];
    radParam[HG_B ] = 1.0 + adTable[HG_B];
    radParam[HG_C ] = adTable[HG_C ];
    radParam[HG_IO] = 1.0 + adTable[HG_IO];
    radParam[HG_LS] = 1.0 + adTable[HG_LS];
    radParam[HG_CP] = 1.0 + adTable[HG_CP];			

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcParamSrcBeam(double dThetay, OUT double radParam[6])
{
    memset(radParam, 0.0, sizeof(double)*6);

    /// SRC 부재는 내진콤팩트 단면으로 가정.
    double dParamSC[6] = { 9.0, 11.0, 0.6, 1.0, 6.0, 8.0 };

    radParam[HG_A ] = 1.0 + dParamSC[HG_A];
    radParam[HG_B ] = 1.0 + dParamSC[HG_B];
    radParam[HG_C ] = dParamSC[HG_C];
    radParam[HG_IO] = 1.0 + dParamSC[HG_IO];
    radParam[HG_LS] = 1.0 + dParamSC[HG_LS];
    radParam[HG_CP] = 1.0 + dParamSC[HG_CP];

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcParamSrcColm(EN_STLCOLM enColmForcType, double dkp, double dThetay, OUT double radParam[6])
{
    memset(radParam, 0.0, sizeof(double)*6);

    double dParamSC[6] = { 0.0, };
    GetTableValueSrcColmParam(enColmForcType, dkp, dParamSC);

    radParam[HG_A ] = 1.0 + dParamSC[HG_A];
    radParam[HG_B ] = 1.0 + dParamSC[HG_B];
    radParam[HG_C ] = dParamSC[HG_C ];
    radParam[HG_IO] = 1.0 + dParamSC[HG_IO];
    radParam[HG_LS] = 1.0 + dParamSC[HG_LS];
    radParam[HG_CP] = 1.0 + dParamSC[HG_CP];		

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::GetTableValueSrcColmParam(EN_STLCOLM enColmForcType, double dkp, double dParamSC[6])
{
    BOOL bDispCtrl = TRUE;	
    switch (enColmForcType)
    {
    case EN_STLCOLM_PLT02PY:
        {
            dParamSC[HG_A ] =  9.0; 
            dParamSC[HG_B ] = 11.0; 
            dParamSC[HG_C ] =  0.6; 
            dParamSC[HG_IO] =  1.0; 
            dParamSC[HG_LS] =  6.0; 
            dParamSC[HG_CP] =  8.0; 
        }
        break;
    case EN_STLCOLM_PLT05PY:
        {			
            dParamSC[HG_A ] = 11.0*dkp; 
            dParamSC[HG_B ] = 17.0*dkp; 
            dParamSC[HG_C ] =  0.2;     
            dParamSC[HG_IO] =  0.25;    
            dParamSC[HG_LS] =  8.0*dkp; 
            dParamSC[HG_CP] = 11.0*dkp; 
        }
        break;
    default: 
        {			
            ASSERT(FALSE);
            bDispCtrl = FALSE;
        }		
        break;
    }
    return bDispCtrl;
}

double CSeisEvalKISTEC2013::CalcPnCompSrc(double dFck, double dAc, double dFy, double dAs, double dFyr, double dAsr)
{
    return dAs*dFy + dAsr*dFyr + 0.85*dAc*dFck;
}

double CSeisEvalKISTEC2013::CalcPnTensSrc(double dFy, double dAs, double dFyr, double dAsr)
{
    return dAs*dFy + dAsr*dFyr;
}

void CSeisEvalKISTEC2013::CalcEval1stRsltConBeamPos(int nSeisCompType, BOOL bSpaceGTd2, OUT T_EVAL1ST_BEAM_POS& rEvalPos)
{
    if ( !rEvalPos.MfacM.bUserMfac )
    {        
        double admfac[3] = { 0.0, };
        const T_CONBEAM_INFO& Info = rEvalPos.Info;
        switch ( Info.enCtrlType )
        {
        case EN_EVALCTRL_FLEX:
            {
                T_MEMBMFACT_TABLE_2 Table2;
                GetMFactTable_ConBeam_CtrlFlex(nSeisCompType, Info.bSeisConform, Table2);
                CalcMFactTable2(Table2, rEvalPos.GetRhoIdx(), Info.dShrIdx, admfac);
            }
            break;
        case EN_EVALCTRL_SHER:
            {
                GetMFactTable_ConBeam_CtrlSher(nSeisCompType, bSpaceGTd2, admfac);
            }
            break;
        default: ASSERT(0);
            break;
        }

        memcpy(rEvalPos.MfacM.admFac, admfac, sizeof(double)*3);
        
        const double dScaleFact = GetScaleUpFactor4MFact();
        MultiplyScaleFactorMFact(dScaleFact, rEvalPos.MfacM.admFac);
    }

    double dMe = rEvalPos.dMUD < 0.0 ? rEvalPos.dMeN : rEvalPos.dMeP;
    for (int i=EN_LEVEL_IO; i<=EN_LEVEL_CP; ++i )
    {
        rEvalPos.DCRM.dDCR[i] = CalcDCRDisp(rEvalPos.dMUD, dMe, rEvalPos.MfacM.admFac[i]);
        rEvalPos.DCRV.dDCR[i] = CalcDCRForc(rEvalPos.dVUF, rEvalPos.dVn*GetMFactForc(static_cast<EN_LEVEL>(i)));        
    }

    rEvalPos.DCRM.enLevel = GetPerformLevel(rEvalPos.DCRM.dDCR);
    rEvalPos.DCRV.enLevel = GetPerformLevel(rEvalPos.DCRV.dDCR);
    
    rEvalPos.enLevel = max(rEvalPos.DCRM.enLevel, rEvalPos.DCRV.enLevel);
}

void CSeisEvalKISTEC2013::CalcEval1stRsltConColmDir(int nSeisCompType, OUT T_EVAL1ST_COLM_DIR& rEvalDir)
{	
    const T_CONCOLM_INFO& Info = rEvalDir.Info;

    if ( !rEvalDir.MfacM.bUserMfac )
    {        
        double admfac[3] = { 0.0, };
        switch ( Info.enCtrlType )
        {
        case EN_EVALCTRL_FLEX: 
            {
                T_MEMBMFACT_TABLE_2 Table2;
                GetMFactTable_ConColm_CtlrFlex(nSeisCompType, Info.bSeisConform, Table2);
                CalcMFactTable2(Table2, Info.dAxlIdx, Info.dShrIdx, admfac);                

                double dScaleFact = GetScaleUpFactor4MFact();
                MultiplyScaleFactorMFact(dScaleFact, admfac);
            }
            break;
        case EN_EVALCTRL_SHER: 
            {
                admfac[EN_LEVEL_IO] = 1.0;
                admfac[EN_LEVEL_LS] = 1.0;
                admfac[EN_LEVEL_CP] = 1.0;
            }
            break;
        default: ASSERT(0);
            break;
        }
        memcpy(rEvalDir.MfacM.admFac, admfac, sizeof(double)*3);
    }

    //SetFinalMe(Info.dMe, Info.CalcMeByShear(), rEvalDir);
}

void CSeisEvalKISTEC2013::CalcMFactConWall(int nSeisCompType, const T_CONWALL_INFO& Info, OUT T_EVAL1ST_WALL_POS& rEval)
{
    if ( Info.bForceCtrl)
    {
        rEval.MfacMV.admFac[EN_LEVEL_IO] = 0.5;
        rEval.MfacMV.admFac[EN_LEVEL_LS] = 0.75;
        rEval.MfacMV.admFac[EN_LEVEL_CP] = 1.0;
        return;
    }
    else
    {
        if ( !rEval.MfacMV.bUserMfac )
        {
            double admfac[3] = { 0.0, };
            switch ( Info.enCtrlType )
            {
            case EN_EVALCTRL_SHER:
                {
                    GetMFactTable_ConWall_CtrlSher(nSeisCompType, Info.dAxlIdx, admfac);
                }                
                break;
            case EN_EVALCTRL_FLEX:
                {
                    T_MEMBMFACT_TABLE_2 Table2;
                    GetMFactTable_ConWall_CtrlFlex(nSeisCompType, Info.bBE, Table2);
                    CalcMFactTable2(Table2, Info.dAxlIdx, Info.dShrIdx, admfac);
                }
                break;
            default: ASSERT(0);
                break;
            }
            memcpy(rEval.MfacMV.admFac, admfac, sizeof(double)*3);
        }

        double dScaleFact = GetScaleUpFactor4MFact();
        MultiplyScaleFactorMFact(dScaleFact, rEval.MfacMV.admFac);
    }
}


void CSeisEvalKISTEC2013::CalcEval1stRsltStlBeamPos(int nSeisCompType, const T_EVAL1ST_STLRSLT_POS& RsltPos, double dBTR, double dDTR, double adBTR[2], double adDTR[2],  OUT T_EVAL1ST_STLBEAM_POS& rEvalPos)
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
            rEvalPos.dmFac[i] = CalcMFactModifiedByLengTerm(dmFact, RsltPos.dCb, RsltPos.dRatLbLpToMrLp);
            rEvalPos.dmFac[i] *= dScaleFact;
        }		
        rEvalPos.DCR.dDCR[i] = CalcDCRDisp(RsltPos.dMUDy, RsltPos.dMnEy, rEvalPos.dmFac[i]);
    }	

    rEvalPos.DCR.enLevel = GetPerformLevel(rEvalPos.DCR.dDCR);
    rEvalPos.enLevel     = rEvalPos.DCR.enLevel;
}

void CSeisEvalKISTEC2013::CalcEval1stStlColmPos(int nSeisCompType, const CString& strSectShape, const T_SECT_SECTBASE_D& SectBaseD, double dE, double dFye, double dPy,
                                                const T_EVAL1ST_STLRSLT_POS& RsltPos, OUT T_EVAL1ST_STLCOLM_POS& rEvalPos)
{
    rEvalPos.SetForcFromRslt(RsltPos);

    double dP = RsltPos.dPUF;
    EN_STLCOLM enColmType = GetStlColmForcType(dP, dPy);
    double dkp = CalcKp(dP, dPy);

    if (!rEvalPos.bUserMfac)
    {
        double dBTR = 0.0;
        double dDTR = 0.0;
        CalcWTR(strSectShape, SectBaseD, dBTR, dDTR);

        double adBTR[2] = { 0.0, };
        double adDTR[2] = { 0.0, };
        CalcWTRLimitStlColm(strSectShape, enColmType, dE, dFye, adBTR, adDTR);

        GetMFactStlColm(enColmType, dkp, nSeisCompType, dBTR, dDTR, adBTR, adDTR, rEvalPos.dmFacy);
        GetMFactStlColm(enColmType, dkp, nSeisCompType, dBTR, dDTR, adBTR, adDTR, rEvalPos.dmFacz);
        GetMFactStlColmTens(nSeisCompType, rEvalPos.dmFacT);

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
    rEvalPos.DCR.dDCR[EN_LEVEL_IO] = CalcDCRStlColm(EN_LEVEL_IO, rEvalPos);
    rEvalPos.DCR.dDCR[EN_LEVEL_LS] = CalcDCRStlColm(EN_LEVEL_LS, rEvalPos);
    rEvalPos.DCR.dDCR[EN_LEVEL_CP] = CalcDCRStlColm(EN_LEVEL_CP, rEvalPos);

    rEvalPos.DCR.enLevel = GetPerformLevel(rEvalPos.DCR.dDCR);
}

void CSeisEvalKISTEC2013::CalcEval1stStlBracPos(int nSeisCompType, const CString& strSectShpe, const T_SECT_SECTBASE_D& SectBaseD, double dFye, 
                                                double adBTR[2], double adDTR[2], UINT unBuckling, const T_EVAL1ST_STLRSLT_POS& RsltPos, OUT T_EVAL1ST_STLBRAC_POS& rEvalPos)
{
    rEvalPos.dPUD = RsltPos.dPUD;
    rEvalPos.dPnC = RsltPos.dPnEC;
    rEvalPos.dPnT = RsltPos.dPnET;

    if (!rEvalPos.bUserMfac)
    {
        GetMFactStlBrac(nSeisCompType, RsltPos.dPUD, strSectShpe, SectBaseD, adBTR, adDTR, unBuckling, rEvalPos.dmFac);
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


void CSeisEvalKISTEC2013::CalcEval1stRsltSrcBeamPos(int nSeisCompType, OUT T_EVAL1ST_STLBEAM_POS& rEvalPos)
{
    /// SRC 부재는 내진 compact 단면으로 고려.	
    double admFacSC[3] = { 2.0, 6.0, 8.0 };
    if (nSeisCompType == D_SCMT_TYPE_SECONDARY)
    {
        admFacSC[EN_LEVEL_LS] = 10.0; 
        admFacSC[EN_LEVEL_CP] = 12.0;
    }

    const double dScaleFact = GetScaleUpFactor4MFact();
    for (int i=EN_LEVEL_IO; i<=EN_LEVEL_CP; ++i)
    {
        if (!rEvalPos.bUserMfac)
        {
            rEvalPos.dmFac[i] = admFacSC[i] * dScaleFact;
        }		
        rEvalPos.DCR.dDCR[i] = CalcDCRDisp(rEvalPos.dMu, rEvalPos.dMe, rEvalPos.dmFac[i]);
    }	

    rEvalPos.DCR.enLevel = GetPerformLevel(rEvalPos.DCR.dDCR);
}

void CSeisEvalKISTEC2013::CalcEval1stSrcColmPos(int nSeisCompType, double dPUF, double dPy, OUT T_EVAL1ST_STLCOLM_POS& rEvalPos)
{
    /// SRC 부재는 내진 compact 단면으로 고려.	

    EN_STLCOLM enColmType = GetStlColmForcType(dPUF, dPy);
    double dkp = CalcKp(dPUF, dPy);

    if (!rEvalPos.bUserMfac)
    {
        GetMFactSrcColm(enColmType, dkp, nSeisCompType, rEvalPos.dmFacy);
        GetMFactSrcColm(enColmType, dkp, nSeisCompType, rEvalPos.dmFacz);
        GetMFactStlColmTens(nSeisCompType, rEvalPos.dmFacT);

        const double dScaleFact = GetScaleUpFactor4MFact();
        MultiplyScaleFactorMFact(dScaleFact, rEvalPos.dmFacy);
        MultiplyScaleFactorMFact(dScaleFact, rEvalPos.dmFacz);
        MultiplyScaleFactorMFact(dScaleFact, rEvalPos.dmFacT);
    }

    rEvalPos.DCR.dDCR[EN_LEVEL_IO] = CalcDCRStlColm(EN_LEVEL_IO, rEvalPos);
    rEvalPos.DCR.dDCR[EN_LEVEL_LS] = CalcDCRStlColm(EN_LEVEL_LS, rEvalPos);
    rEvalPos.DCR.dDCR[EN_LEVEL_CP] = CalcDCRStlColm(EN_LEVEL_CP, rEvalPos);

    rEvalPos.DCR.enLevel = GetPerformLevel(rEvalPos.DCR.dDCR);
}


void CSeisEvalKISTEC2013::GetMFactStlBrac(int nSeisCompType, double dPu, const CString& strSectShape, const T_SECT_SECTBASE_D& SectBaseD, double adBTR[2], double adDTR[2],
                                          UINT unBucking, OUT double radmFac[3])
{
    if (dPu < 0.0) // compression.
    {
        if (strSectShape == D_SECT_SHAPE_REG_B || strSectShape == D_SECT_SHAPE_REG_P)
        {
            double dBTR = 0.0;
            double dDTR = 0.0;
            CalcWTR(strSectShape, SectBaseD, dBTR, dDTR);

            GetMFactStlBracShapeBP(dBTR, dDTR, adBTR, adDTR, radmFac);
            return;			
        }
        else if (strSectShape == D_SECT_SHAPE_REG_2L || strSectShape == D_SECT_SHAPE_REG_2C)
        {
            GetMFactStlBracShape2L2C(nSeisCompType, unBucking, radmFac);
            return;
        }
        else if (strSectShape == D_SECT_SHAPE_REG_H)
        {
            GetMFactStlBracShapeH(radmFac);
            return;
        }
        else
        {
            ///### 우선 H를 따라가보자. -> [GEN-2993] Table에 없는 단면은 최소값 적용.
            radmFac[EN_LEVEL_IO] = 1.25;
            radmFac[EN_LEVEL_LS] = 2.0;
            radmFac[EN_LEVEL_CP] = 3.0;
            return;
        }
    }
    else
    {
        /// 중심가새골조의 가새 (인장을 받을 때) : 인장-압축가새.
        radmFac[EN_LEVEL_IO] = 1.25;
        radmFac[EN_LEVEL_LS] = 6.0;
        radmFac[EN_LEVEL_CP] = 8.0;

        if (nSeisCompType == D_SCMT_TYPE_SECONDARY)
        {
            radmFac[EN_LEVEL_LS] =  8.0;
            radmFac[EN_LEVEL_CP] = 10.0;
        }
    }	
}

void CSeisEvalKISTEC2013::GetMFactStlBracShapeBP(double dBTR, double dDTR, double adBTR[2], double adDTR[2], OUT double radmFac[3])
{
    /// 중심가새골조의 가새 (압축을 받을 때) : 각형 및 원형 강관 (1차/2차 부재가 동일)
    double admFacSC[2] = { 5.0, 7.0 };
    double admFacNC[2] = { 2.0, 3.0 };

    radmFac[EN_LEVEL_IO] = 1.25;
    radmFac[EN_LEVEL_LS] = CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, admFacSC[0], admFacNC[0]);
    radmFac[EN_LEVEL_CP] = CalcParamDDyFactStl(dBTR, dDTR, adBTR, adDTR, admFacSC[1], admFacNC[1]);
}

void CSeisEvalKISTEC2013::GetMFactStlBracShape2L2C(int nSeisCompType, UINT unBucking, OUT double radmFac[3])
{
    /// 중심가새골조의 가새 (압축을 받을 때) : 쌍ㄱ 및 쌍ㄷ 형강. ROW : unBuckling, COL = nSeisCompType.
    double admFacLS[2][2] = { { 6.0, 7.0 }, { 5.0, 6.0 } }; 
    double admFacCP[2][2] = { { 8.0, 9.0 }, { 7.0, 8.0 } };

    radmFac[EN_LEVEL_IO] = 1.25;
    radmFac[EN_LEVEL_LS] = admFacLS[unBucking][nSeisCompType];
    radmFac[EN_LEVEL_CP] = admFacCP[unBucking][nSeisCompType];	
}

void CSeisEvalKISTEC2013::GetMFactStlBracShapeH(OUT double radmFac[3])
{
    /// 중심가새골조의 가새 (압축을 받을 때) : H 및 I형강 => Primary/Secondary 동일.
    radmFac[EN_LEVEL_IO] = 1.25;
    radmFac[EN_LEVEL_LS] = 6.0;
    radmFac[EN_LEVEL_CP] = 8.0;
}

void CSeisEvalKISTEC2013::GetMFactSrcColm(EN_STLCOLM enColmForcType, double dkp, int nSeisCompType, OUT double radmFac[3])
{
    double adNC[3][2] = { 0.0, };
    double adSC[3][2] = { 0.0, };	
    GetMFactTableValueStlColm(enColmForcType, dkp, adSC, adNC);

    radmFac[EN_LEVEL_IO] = adSC[EN_LEVEL_IO][nSeisCompType];
    radmFac[EN_LEVEL_LS] = adSC[EN_LEVEL_LS][nSeisCompType];
    radmFac[EN_LEVEL_CP] = adSC[EN_LEVEL_CP][nSeisCompType];
}

BOOL CSeisEvalKISTEC2013::CalcSeisEval2ndRsltConBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_BEAM& raEval2nd)
{
    raEval2nd.Initialize();
    int nAddStepIdx = -1;
    return CalcSeisEval2ndRsltConBeam(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd);	
}

BOOL CSeisEvalKISTEC2013::CalcSeisEval2ndRsltConBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_BEAM& raEval2nd)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    raEval2nd.Initialize();

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    if (!GetElemMatlSectCon(ElemK, SectD, MatdD, SecmD)) { ASSERT(0); return FALSE; } 
    if (SecmD.bUseDefaultVal) { return FALSE; }

    CSeisEvalDataTool Tool;
    if ( Tool.GetSeisComponentType(ElemK) == D_SCMT_TYPE_NONE) return FALSE;

    double adForc[2][6] = { { 0.0 }, };
    double adM1y[2] = { 0.0, };
    double adM1z[2] = { 0.0, };
    GetPORsltForc(ElemK, PolcK, nStep, nAddStepIdx, adForc, adM1y, adM1z);

    if (!GetSeisEval2ndRsltSubConBeam(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd))  return FALSE;

    raEval2nd.bEvaluate = TRUE;

    double adV[2] = { adForc[0][2], adForc[1][2] };
    double adM[2] = { adForc[0][4], adForc[1][4] };
    T_CONBEAM_INFO aInfo[EN_MEMBEND_NUM];
    if ( !MakeBeamInfo(ElemK, EN_EVALSTEP_2ND, adV, aInfo) ) return FALSE;

    for (int i=0; i<EN_MEMBEND_NUM; ++i)
    {
        const T_CONBEAM_INFO& Info = aInfo[i];
        T_EVAL2ND_BEAM_POS& rCurPos = i==EN_MEMBEND_I ? raEval2nd.EndI : raEval2nd.EndJ;

        rCurPos.enCtrlType   = Info.enCtrlType;
        rCurPos.bSeisConform = Info.bSeisConform;
        rCurPos.dRebarIndex  = adM[i] < 0.0 ? Info.dRhoIdxTop : Info.dRhoIdxBot;
        rCurPos.dShearIndex  = Info.dShrIdx;
    }

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcSeisEval2ndRsltConColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_COLM& raEval2nd)
{
    raEval2nd.Initialize();
    int nAddStepIdx = -1;
    return CalcSeisEval2ndRsltConColm(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd);	
}

BOOL CSeisEvalKISTEC2013::CalcSeisEval2ndRsltConColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_COLM& raEval2nd)
{
    CCurUnitSaver UnitSave(TRUE);
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

    double adForc[2][6] = { { 0.0 }, };
    double adM1y[2] = { 0.0, };
    double adM1z[2] = { 0.0, };
    GetPORsltForc(ElemK, PolcK, nStep, nAddStepIdx, adForc, adM1y, adM1z);

    if (!GetSeisEval2ndRsltSubConColm(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd))  return FALSE;

    raEval2nd.bEvaluate = TRUE;

    T_COLM_SHR_PARAM aShry[EN_MEMBEND_NUM];
    if (!MakeColmShearParamEval(ElemK, EN_EVALSTEP_2ND, EN_DIR_Y, aShry)) { return FALSE; }

    T_COLM_SHR_PARAM aShrz[EN_MEMBEND_NUM];
    if (!MakeColmShearParamEval(ElemK, EN_EVALSTEP_2ND, EN_DIR_Z, aShrz)) { return FALSE; }

    const double dFck = Tool.GetFc(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);
    const double dAg  = SectD.SectBefore.SectI.Stiffness.Area;    
        
    _DB_PMCV_DATA Pmcvy;
    _DB_PMCV_DATA Pmcvz;
    if (!GetPmcvData(ElemK, EN_EVALSTEP_2ND, EN_EVALMATL_AVG, Pmcvy, Pmcvz)) { return FALSE; }

    CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();
    double dL = pMembCtrl->GetMembLengByIncludeElem(ElemK);
    
    const bool bUseHook = Tool.IsUseSeisHook(ElemK);
    const int nPMMethod = D_SEME_PMCM_P;

    const double dPui  = adForc[0][0]*(-1.0);
    const double dMeyi = CalcMnByAxial(Pmcvy, dPui);
    const double dMezi = CalcMnByAxial(Pmcvz, dPui);

    const double dPuj  = adForc[1][0]*(-1.0);
    const double dMeyj = CalcMnByAxial(Pmcvy, dPuj);
    const double dMezj = CalcMnByAxial(Pmcvz, dPuj);

    for ( int i=0; i<EN_MEMBEND_NUM; ++i )
    {
        T_EVAL2ND_COLM_POS& rCurPos = ( i == EN_MEMBEND_I ) ? raEval2nd.PosI : raEval2nd.PosJ;

        const T_COLM_SHR_PARAM& CurShry = aShry[i];
        const T_COLM_SHR_PARAM& CurShrz = aShrz[i];

        const double dPu  = adForc[i][0]*(-1.0);
        rCurPos.dAxialIndex = CalcAxialIndex(dPu, dFck, dAg);

        const double dVuz = adForc[i][2];
        const double dMuy = adForc[i][4];
        const double dMey = CalcMnByAxial(Pmcvy, dPu);        
        const double dmuy = CalcDutilityFactor(dMuy, dMey);        
        const double dVnz = CalcVnConColm(CurShrz, dPu, dVuz, dMuy, dmuy);

        rCurPos.Diry.bSeisConform = IsTransReinfConforming(bUseHook, dVuz, CurShrz.Comm);
        rCurPos.Diry.dShearIndex  = CalcShearIndex(dVuz, CurShrz.Comm);
        rCurPos.Diry.enCtrlType   = CalcColmCtrlType(dMeyi, dMeyj, dL, dVnz);

        const double dVuy = adForc[i][1];
        const double dMuz = adForc[i][5];
        const double dMez = CalcMnByAxial(Pmcvz, dPu);
        const double dmuz = CalcDutilityFactor(dMuz, dMez);
        const double dVny = CalcVnConColm(CurShry, dPu, dVuy, dMuz, dmuz);

        rCurPos.Dirz.bSeisConform = IsTransReinfConforming(bUseHook, dVuy, CurShry.Comm);
        rCurPos.Dirz.dShearIndex  = CalcShearIndex(dVuy, CurShry.Comm);
        rCurPos.Dirz.enCtrlType   = CalcColmCtrlType(dMezi, dMezj, dL, dVny);
    }

    return TRUE;
}

void CSeisEvalKISTEC2013::CalcSeisEval2ndRsltConColmDir(BOOL bUseHook, double dVu, double dVn, const T_CON_SHR_PARAM& Shr,
                                                        double dL, double dMecur, double dMei, double dMej, OUT T_EVAL2ND_COLM_DIR& rEvalDir)
{
    rEvalDir.bSeisConform = IsTransReinfConforming(bUseHook, dVu, Shr);
    rEvalDir.dShearIndex  = CalcShearIndex(dVu, Shr);
    rEvalDir.enCtrlType   = CalcColmCtrlType(dMei, dMej, dL, dVn);
}

BOOL CSeisEvalKISTEC2013::CalcSeisEval2ndRsltConWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_WALL& raEval2nd)
{
    raEval2nd.Initialize();
    int nAddStepIdx = -1;
    return CalcSeisEval2ndRsltConWall(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd);	
}

BOOL CSeisEvalKISTEC2013::CalcSeisEval2ndRsltConWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_WALL& raEval2nd)
{
    CCurUnitSaver UnitSave(TRUE);
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
    if ( Tool.GetSeisComponentType(ElemK) == D_SCMT_TYPE_NONE) { return FALSE; }

    double adForc[2][6] = { { 0.0 }, };
    double adM1y[2] = { 0.0, };
    double adM1z[2] = { 0.0, };
    GetPORsltForc(ElemK, PolcK, nStep, nAddStepIdx, adForc, adM1y, adM1z);

    if (!GetSeisEval2ndRsltSubConWall(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd))  return FALSE;

    raEval2nd.bEvaluate = TRUE;

    T_WALL_SHR_PARAM aShrAvg[EN_MEMBEND_NUM];
    T_WALL_SHR_PARAM aShrNom[EN_MEMBEND_NUM];
    if ( !MakeWallShearParam(ElemK, EN_EVALSTEP_2ND, EN_DIR_Z, aShrAvg, aShrNom) ) { return FALSE; }
    SetWallForcShearParam(adForc, EN_DIR_Z, aShrAvg, aShrNom);

    double dFck = Tool.GetFc(EN_EVALSTEP_2ND, EN_EVALMATL_NOM, SecmD);
    double dTw  = WallD.dThickness;
    double dLw  = WallD.dLength;
    double dHw  = WallD.dHeight;
    double dAg  = dTw*dLw;

    /// [JIRA,GEN-631] 전단지배이고 벽체 축력비 > 0.15인 경우라도 변형지배로 고려. (해석 수렴성 향상을 위함)
    for (int i=0; i<EN_MEMBEND_NUM; ++i)
    {
        T_EVAL2ND_WALL_POS& rCur = i==EN_MEMBEND_I ? raEval2nd.PosI : raEval2nd.PosJ;
        EN_WALL_FORC_POS ForcPos = GetWallForcPos(static_cast<EN_MEMBEND>(i));

        const T_WALL_SHR_PARAM CurShr = aShrAvg[i];
        double dPu = adForc[ForcPos][DOF_FX]*(-1.0);
        double dMe = adM1y[ForcPos];
        double dVe = CalcVnConWall(CurShr);

        rCur.dAxialIndex = CalcAxialIndex(dPu, dFck, dAg);
        rCur.dShearIndex = CalcShearIndexWall(CurShr.dV, CurShr);
        rCur.enCtrlType  = CalcWallCtrlType(dMe, dVe, dHw, CurShr);		
        rCur.bForceCtrl  = FALSE;//(raEval2nd.PosJ.enCtrlType == EN_EVALCTRL_SHER && raEval2nd.PosJ.dAxialIndex > 0.15) ? TRUE : FALSE;
    }    

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcSeisEval2ndRsltStlBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLBEAM& raEval2nd)
{
    raEval2nd.Initialize();
    int nAddStepIdx = -1;
    return CalcSeisEval2ndRsltStlBeam(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd);	
}

BOOL CSeisEvalKISTEC2013::CalcSeisEval2ndRsltStlBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLBEAM& raEval2nd)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    raEval2nd.Initialize();

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SESM_D SesmD;
    if (!GetElemMatlSectStl(ElemK, SectD, MatdD, SesmD)) { ASSERT(FALSE); return FALSE; } 
    if (SesmD.bUseDefaultVal) { return FALSE; }

    CSeisEvalDataTool Tool;
    if ( Tool.GetSeisComponentType(ElemK) == D_SCMT_TYPE_NONE) return FALSE;

    if (!GetSeisEval2ndRsltSubStlBeam(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd))  return FALSE;

    raEval2nd.bEvaluate = TRUE;

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcSeisEval2ndRsltStlColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLCOLM& raEval2nd)
{
    raEval2nd.Initialize();
    int nAddStepIdx = -1;
    return CalcSeisEval2ndRsltStlColm(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd);	
}

BOOL CSeisEvalKISTEC2013::CalcSeisEval2ndRsltStlColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLCOLM& raEval2nd)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    raEval2nd.Initialize();

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SESM_D SesmD;
    if (!GetElemMatlSectStl(ElemK, SectD, MatdD, SesmD)) { ASSERT(FALSE); return FALSE; } 
    if (SesmD.bUseDefaultVal) { return FALSE; }

    CSeisEvalDataTool Tool;
    if ( Tool.GetSeisComponentType(ElemK) == D_SCMT_TYPE_NONE) return FALSE;

    if (!GetSeisEval2ndRsltSubStlColm(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd))  return FALSE;

    raEval2nd.bEvaluate = TRUE;

    return TRUE;
}

BOOL CSeisEvalKISTEC2013::CalcSeisEval2ndRsltStlBrac(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLBRAC& raEval2nd)
{
    raEval2nd.Initialize();
    int nAddStepIdx = -1;
    return CalcSeisEval2ndRsltStlBrac(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd);	
}

BOOL CSeisEvalKISTEC2013::CalcSeisEval2ndRsltStlBrac(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLBRAC& raEval2nd)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    raEval2nd.Initialize();

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SESM_D SesmD;
    if (!GetElemMatlSectStl(ElemK, SectD, MatdD, SesmD)) { ASSERT(FALSE); return FALSE; } 
    if (SesmD.bUseDefaultVal) { return FALSE; }

    CSeisEvalDataTool Tool;
    if ( Tool.GetSeisComponentType(ElemK) == D_SCMT_TYPE_NONE) return FALSE;

    if (!GetSeisEval2ndRsltSubStlBrac(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd))  return FALSE;

    raEval2nd.bEvaluate = TRUE;

    return TRUE;
}

void CSeisEvalKISTEC2013::GetParamTable_ConBeam_CtrlFlex(int nSeisCompType, BOOL bSeisConform, OUT T_MEMBPARAM_TABLE_2& rTable)
{
    rTable.Initialize();

    /// rho index;
    rTable.dA1 = 0.0;
    rTable.dA2 = 0.5;
    /// shear index;
    rTable.dB1 = 0.25;
    rTable.dB2 = 0.5;

    /// see [KISTEC 2013, table 5.3.1]
    double aA1B1[2][8] = { { 0.02 , 0.03 , 0.2, 0.005 , 0.01 , 0.02 , 0.02 , 0.03  }, { 0.025, 0.05, 0.2, 0.010, 0.02 , 0.025, 0.02 , 0.05 } };
    double aA1B2[2][8] = { { 0.01 , 0.015, 0.2, 0.0015, 0.005, 0.01 , 0.01 , 0.015 }, { 0.02 , 0.04, 0.2, 0.005, 0.01 , 0.02 , 0.02 , 0.04 } };
    double aA2B1[2][8] = { { 0.01 , 0.015, 0.2, 0.005 , 0.01 , 0.01 , 0.01 , 0.015 }, { 0.02 , 0.03, 0.2, 0.005, 0.01 , 0.02 , 0.02 , 0.03 } };
    double aA2B2[2][8] = { { 0.005, 0.01 , 0.2, 0.0015, 0.005, 0.005, 0.005, 0.01  }, { 0.015, 0.02, 0.2, 0.005, 0.005, 0.015, 0.015, 0.02 }};

    int nIdx1 = bSeisConform ? 1 : 0;    
    for ( int i=0; i<6; ++i )
    {
        int nIdx2 = GetColIndexParamTable(nSeisCompType, i);

        rTable.adTable_A1_B1[i] = aA1B1[nIdx1][nIdx2];
        rTable.adTable_A1_B2[i] = aA1B2[nIdx1][nIdx2];
        rTable.adTable_A2_B1[i] = aA2B1[nIdx1][nIdx2];
        rTable.adTable_A2_B2[i] = aA2B2[nIdx1][nIdx2];
    }
}

void CSeisEvalKISTEC2013::GetMFactTable_ConBeam_CtrlFlex(int nSeisCompType, BOOL bSeisConform, OUT T_MEMBMFACT_TABLE_2& rTable)
{
    rTable.Initialize();

    /// rho index;
    rTable.dA1 = 0.0;
    rTable.dA2 = 0.5;
    /// shear index;
    rTable.dB1 = 0.25;
    rTable.dB2 = 0.5;

    /// see [KISTEC 2013, table 4.3.3]
    double aA1B1[2][5] = { { 2.0 , 3.0, 4.0, 3.0, 5.0 } , { 3.0, 6.0, 7.0, 6.0, 10.0 } };
    double aA1B2[2][5] = { { 1.25, 2.0, 3.0, 2.0, 4.0 } , { 2.0, 3.0, 4.0, 3.0,  5.0 } };
    double aA2B1[2][5] = { { 2.0 , 3.0, 3.0, 3.0, 4.0 } , { 2.0, 3.0, 4.0, 3.0,  5.0 } };
    double aA2B2[2][5] = { { 1.25, 2.0, 2.0, 2.0, 3.0 } , { 2.0, 2.0, 3.0, 2.0,  4.0 } };

    int nIdx1 = bSeisConform ? 1 : 0;    
    for ( int i=0; i<3; ++i )
    {
        int nIdx2 = GetColIndexMFactTable(nSeisCompType, i);
        
        rTable.adTable_A1_B1[i] = aA1B1[nIdx1][nIdx2];
        rTable.adTable_A1_B2[i] = aA1B2[nIdx1][nIdx2];
        rTable.adTable_A2_B1[i] = aA2B1[nIdx1][nIdx2];
        rTable.adTable_A2_B2[i] = aA2B2[nIdx1][nIdx2];
    }
}

void CSeisEvalKISTEC2013::GetParamTable_ConBeam_CtrlSher(int nSeisCompType, BOOL bSpaceGTd2, OUT double radParam[6])
{
    memset(radParam, 0.0, sizeof(double)*6);

    double adParam[2][8] = { { 0.003, 0.02, 0.2, 0.0015, 0.002, 0.003, 0.01, 0.02 }, { 0.003, 0.01, 0.2, 0.0015, 0.002, 0.003, 0.005, 0.01 } };

    int nIdx1 = bSpaceGTd2 ? 1 : 0;    
    for ( int i=0; i<6; ++i )
    {
        int nIdx2 = GetColIndexParamTable(nSeisCompType, i);
        radParam[i] = adParam[nIdx1][nIdx2];
    }
}

void CSeisEvalKISTEC2013::GetMFactTable_ConBeam_CtrlSher(int nSeisCompType, BOOL bSpaceGTd2, OUT double raMFact[3])
{
    memset(raMFact, 0.0, sizeof(double)*3);

    double adMFact[2][5] = { { 1.25, 1.5, 1.75, 3.0, 4.0 }, { 1.25, 1.5, 1.75, 2.0, 3.0 } };

    int nIdx1 = bSpaceGTd2 ? 1 : 0;
    for ( int i=0; i<3; ++i )
    {
        int nIdx2 = GetColIndexMFactTable(nSeisCompType, i);
        raMFact[i] = adMFact[nIdx1][nIdx2];
    }    
}

void CSeisEvalKISTEC2013::GetParamTable_ConColm_CtrlFlex(int nSeisCompType, BOOL bSeisConform, OUT T_MEMBPARAM_TABLE_2& rTable)
{
    rTable.Initialize();
    /// axial ratio.
    rTable.dA1 = 0.1;
    rTable.dA2 = 0.4;
    /// shear force index.
    rTable.dB1 = 0.25;
    rTable.dB2 = 0.5;

    /// see [KISTEC 2013, table 5.3.2]
    const double aA1B1[2][8] = { { 0.006, 0.015, 0.2, 0.005, 0.005, 0.006, 0.01 , 0.015 } , { 0.02 , 0.03 , 0.2, 0.005, 0.015, 0.02 , 0.02 , 0.03  } };
    const double aA1B2[2][8] = { { 0.005, 0.012, 0.2, 0.005, 0.005, 0.005, 0.008, 0.012 } , { 0.016, 0.024, 0.2, 0.005, 0.012, 0.016, 0.016, 0.024 } };
    const double aA2B1[2][8] = { { 0.003, 0.01 , 0.2, 0.002, 0.002, 0.003, 0.006, 0.01  } , { 0.015, 0.025, 0.2, 0.003, 0.012, 0.015, 0.018, 0.025 } };
    const double aA2B2[2][8] = { { 0.002, 0.008, 0.2, 0.002, 0.002, 0.002, 0.005, 0.008 } , { 0.012, 0.02 , 0.1, 0.003, 0.01 , 0.012, 0.013, 0.02  } };

    const int nIdx1 = bSeisConform ? 1 : 0;    
    for ( int i=0; i<6; ++i )
    {
        const int nIdx2 = GetColIndexParamTable(nSeisCompType, i);        

        rTable.adTable_A1_B1[i] = aA1B1[nIdx1][nIdx2];
        rTable.adTable_A1_B2[i] = aA1B2[nIdx1][nIdx2];
        rTable.adTable_A2_B1[i] = aA2B1[nIdx1][nIdx2];
        rTable.adTable_A2_B2[i] = aA2B2[nIdx1][nIdx2];
    }
}

void CSeisEvalKISTEC2013::GetMFactTable_ConColm_CtlrFlex(int nSeisCompType, BOOL bSeisConform, OUT T_MEMBMFACT_TABLE_2& rTable)
{
    rTable.Initialize();
    /// axial ratio.
    rTable.dA1 = 0.1;
    rTable.dA2 = 0.4;
    /// shear force index.
    rTable.dB1 = 0.25;
    rTable.dB2 = 0.5;

    /// see [KISTEC 2013, table 4.3.4]
    const double aA1B1[2][5] = { { 2.0 , 2.0, 3.0 , 2.0, 3.0 } , { 2.0 , 3.0, 4.0, 4.0, 5.0 } };
    const double aA1B2[2][5] = { { 2.0 , 2.0, 2.4 , 1.6, 2.4 } , { 2.0 , 2.4, 3.2, 3.2, 4.0 } };
    const double aA2B1[2][5] = { { 1.25, 1.5, 2.0 , 1.5, 2.0 } , { 1.25, 2.0, 3.0, 2.0, 3.0 } };
    const double aA2B2[2][5] = { { 1.25, 1.5, 1.75, 1.0, 1.6 } , { 1.25, 1.6, 2.4, 2.4, 3.2 } };

    const int nIdx1 = bSeisConform ? 1 : 0;
    for ( int i=0; i<3; ++i )
    {
        const int nIdx2 = GetColIndexMFactTable(nSeisCompType, i);
        
        rTable.adTable_A1_B1[i] = aA1B1[nIdx1][nIdx2];
        rTable.adTable_A1_B2[i] = aA1B2[nIdx1][nIdx2];
        rTable.adTable_A2_B1[i] = aA2B1[nIdx1][nIdx2];
        rTable.adTable_A2_B2[i] = aA2B2[nIdx1][nIdx2];
    }
}

void CSeisEvalKISTEC2013::GetParamTable_ConWall_CtrlFlex(int nSeisCompType, BOOL bBE, OUT T_MEMBPARAM_TABLE_2& rTable)
{
    rTable.Initialize();
    /// axial ratio.
    rTable.dA1 = 0.1;
    rTable.dA2 = 0.25;
    /// shear force index.
    rTable.dB1 = 0.25;
    rTable.dB2 = 0.5;

    /// see [KISTEC 2013, table 5.3.3]
    const double aA1B1[2][8] = { { 0.015, 0.020, 0.75, 0.005 , 0.010, 0.015, 0.015, 0.020 } , { 0.008, 0.015, 0.6 , 0.002, 0.004, 0.008, 0.008, 0.015 } };
    const double aA1B2[2][8] = { { 0.01 , 0.015, 0.4 , 0.004 , 0.008, 0.01 , 0.01 , 0.015 } , { 0.006, 0.01 , 0.3 , 0.002, 0.004, 0.006, 0.006, 0.01  } };
    const double aA2B1[2][8] = { { 0.009, 0.012, 0.6 , 0.003 , 0.006, 0.009, 0.009, 0.012 } , { 0.003, 0.005, 0.25, 0.001, 0.002, 0.003, 0.003, 0.005 } };
    const double aA2B2[2][8] = { { 0.005, 0.01 , 0.3 , 0.0015, 0.003, 0.005, 0.005, 0.001 } , { 0.002, 0.004, 0.2 , 0.001, 0.001, 0.002, 0.002, 0.004 } };

    const int nIdx1 = bBE ? 0 : 1;  /// 특수경계요소의 유무.
    for ( int i=0; i<6; ++i )
    {
        const int nIdx2 = GetColIndexParamTable(nSeisCompType, i);

        rTable.adTable_A1_B1[i] = aA1B1[nIdx1][nIdx2];
        rTable.adTable_A1_B2[i] = aA1B2[nIdx1][nIdx2];
        rTable.adTable_A2_B1[i] = aA2B1[nIdx1][nIdx2];
        rTable.adTable_A2_B2[i] = aA2B2[nIdx1][nIdx2];
    }
}

void CSeisEvalKISTEC2013::GetMFactTable_ConWall_CtrlFlex(int nSeisCompType, BOOL bBE, OUT T_MEMBMFACT_TABLE_2& rTable)
{
    rTable.Initialize();
    /// axial ratio.
    rTable.dA1 = 0.1;
    rTable.dA2 = 0.25;
    /// shear force index.
    rTable.dB1 = 0.25;
    rTable.dB2 = 0.5;

    /// see [KISTEC 2013, table 4.3.5]
    const double aA1B1[2][5] = { { 2.0 , 4.0, 6.0, 6.0, 8.0 } , { 2.0 , 2.5, 4.0 , 4.0 , 6.0 } };
    const double aA1B2[2][5] = { { 2.0 , 3.0, 4.0, 4.0, 6.0 } , { 1.5 , 2.0, 2.5 , 2.5 , 4.0 } };
    const double aA2B1[2][5] = { { 1.5 , 3.0, 4.0, 4.0, 6.0 } , { 1.25, 1.5, 2.0 , 2.0 , 3.0 } };
    const double aA2B2[2][5] = { { 1.25, 2.0, 2.5, 2.5, 4.0 } , { 1.25, 1.5, 1.75, 1.75, 2.0 } };

    const int nIdx1 = bBE ? 0 : 1;  /// 특수경계요소의 유무.
    for ( int i=0; i<3; ++i )
    {
        const int nIdx2 = GetColIndexMFactTable(nSeisCompType, i);

        rTable.adTable_A1_B1[i] = aA1B1[nIdx1][nIdx2];
        rTable.adTable_A1_B2[i] = aA1B2[nIdx1][nIdx2];
        rTable.adTable_A2_B1[i] = aA2B1[nIdx1][nIdx2];
        rTable.adTable_A2_B2[i] = aA2B2[nIdx1][nIdx2];
    }
}

void CSeisEvalKISTEC2013::GetParamTable_ConWall_CtrlSher(int nSeisCompType, double dAxlIdx, OUT double radTable[6])
{
    memset(radTable, 0.0, sizeof(double)*6);

    /// see [KISTEC 2013, table 5.3.3] parameter d,e, IO, IL, CP (% in Table).
    double adTable[8] = { 0.0075, 0.02, 0.4, 0.004, 0.006, 0.0075, 0.0075, 0.015 };
    for ( int i=0; i<6; ++i )
    {
        int nIdx = GetColIndexParamTable(nSeisCompType, i);
        radTable[i] = adTable[nIdx];
    } 
}

void CSeisEvalKISTEC2013::GetMFactTable_ConWall_CtrlSher(int nSeisCompType, double dAxlIdx, OUT double raMFact[3])
{
    memset(raMFact, 0.0, sizeof(double)*3);

    /// see [KISTEC 2013, Table 4.3.5]
    double adMFact[5] = { 2.0, 2.0, 3.0, 2.0, 3.0 };        
    for ( int i=0; i<3; ++i )
    {
        int nIdx = GetColIndexMFactTable(nSeisCompType, i);
        raMFact[i] = adMFact[nIdx];
    }    
}

bool CSeisEvalKISTEC2013::IsSectShapeStlBrac(const CString& strShape)
{
    if ( strShape == D_SECT_SHAPE_REG_H  ) { return true; }
    if ( strShape == D_SECT_SHAPE_REG_2L ) { return true; }
    if ( strShape == D_SECT_SHAPE_REG_2C ) { return true; }
    if ( strShape == D_SECT_SHAPE_REG_B  ) { return true; }
    if ( strShape == D_SECT_SHAPE_REG_P  ) { return true; }    
    return false;
}

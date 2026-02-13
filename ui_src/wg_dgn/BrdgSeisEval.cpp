#include "stdafx.h"
#include "BrdgSeisEval.h"

#include "..\wg_db\BrdgSeisEvalResult.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\SelfWgt.h"

#include "DgnSeisAnchorShapeMgr.h"

#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_base\wg_base_ArrayUtil.h"

CBrdgSeisEval::CBrdgSeisEval(void)
{
    m_pDoc = CDBDoc::GetDocPoint();
    if (m_pDoc == NULL) { ASSERT(FALSE); return; }	

    m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(m_CurrUnit);
}


CBrdgSeisEval::~CBrdgSeisEval(void)
{
}

int CBrdgSeisEval::Get_MPhiResult(const T_MPCC_K& Key, T_BRDGEVAL_MPHI_RES mphiResD[2], BOOL bRecal)
{
    T_MPCC_D MpccD;
    if(!m_pDoc->m_pAttrCtrl2->GetMpcc(Key, MpccD)) { ASSERT(0); return FALSE; }  

    return Get_MPhiResult(MpccD, mphiResD, bRecal);
}

int CBrdgSeisEval::Get_MPhiResult(const T_MPCC_D& MpccD, T_BRDGEVAL_MPHI_RES mphiResD[2], BOOL bRecal, double dAddAngle/*=0.0*/)
{ 
    mphiResD[0].initialize();
    mphiResD[1].initialize();

    T_MPCC_D Code_MpccD = const_cast<T_MPCC_D&>(MpccD);
    SEIS_RES_K UKey1, UKey2;
    // 교축방향
    UKey1.Key.GrupK  = MpccD.EvgpK;
    UKey1.Key.iColm  = MpccD.nEvgpColm;
    UKey1.Key.enDir  = LONGI;
    UKey1.Key.enPos  = static_cast<EN_POSITION>(MpccD.nEvgpPos);
    // 교축직각방향
    UKey2.Key.GrupK = MpccD.EvgpK;
    UKey2.Key.iColm = MpccD.nEvgpColm;
    UKey2.Key.enDir = TRANS;
    UKey2.Key.enPos = static_cast<EN_POSITION>(MpccD.nEvgpPos);

    int nChk=0;
    CBrdgSeisEvalResult *pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult();
    if(bRecal)
    {
        nChk = CalcMomentCurvature(Code_MpccD, FALSE, dAddAngle);
        if(!pResult->GetMPhiRes(UKey1.KeyMap, mphiResD[0]))  { ASSERT(0); return FALSE; }
        if(!pResult->GetMPhiRes(UKey2.KeyMap, mphiResD[1]))  { ASSERT(0); return FALSE; }
    } 
    else
    {
        if(!pResult->GetMPhiRes(UKey1.KeyMap, mphiResD[0]))
        {
            if(MpccD.bEvgp && MpccD.EvgpK!=0) pResult->ReadSeisEvalResultMPhi(UKey1.Key.GrupK, UKey1.Key.iColm, UKey1.Key.enDir, UKey1.Key.enPos, mphiResD[0]);
        }
        if(!pResult->GetMPhiRes(UKey2.KeyMap, mphiResD[1]))
        {
            if(MpccD.bEvgp && MpccD.EvgpK!=0) pResult->ReadSeisEvalResultMPhi(UKey2.Key.GrupK, UKey2.Key.iColm, UKey2.Key.enDir, UKey2.Key.enPos, mphiResD[1]);
        }
        nChk = max(mphiResD[0].StateD.nChk, mphiResD[1].StateD.nChk);
    }

    return nChk;
}

BOOL CBrdgSeisEval::GetMatlData(const UINT& ElemK, T_MATD_D& MatlD)
{
    MatlD.Initialize();

    T_ELEM_D eData;
    eData.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, eData))	{ ASSERT(0); return FALSE; }
    UINT iMatl = eData.elmat;

    if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(iMatl, MatlD))	{ ASSERT(0); return FALSE; }
    if(!m_pDoc->m_pAttrCtrl->GetDgnConMatd(MatlD))        { ASSERT(0); return FALSE; }

    return TRUE;
}

UINT CBrdgSeisEval::GetSectKey(const UINT& ElemK)
{
    T_ELEM_D eData;
    eData.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, eData))	{ ASSERT(0); return FALSE; }
    UINT iSect = eData.elpro;

    return iSect;
}

BOOL CBrdgSeisEval::GetSectData(const UINT& ElemK, T_SECT_D& SectD)
{
    SectD.Initialize();

    UINT iSect = GetSectKey(ElemK);
    if(!m_pDoc->m_pAttrCtrl->GetSectDesign(iSect, SectD)) { ASSERT(0); return FALSE; }

    return TRUE;
}

int CBrdgSeisEval::CalcMomentCurvature(const T_MPCC_K& MpccK, BOOL bSave)
{
    T_MPCC_D MpccD;
    if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) { ASSERT(0); return -1; }
    return CalcMomentCurvature(MpccD, bSave);
}

void CBrdgSeisEval::CalcMomentCurvature(CArray<T_MPCC_K, T_MPCC_K>& arMpccK, BOOL bSave)
{
    int nMpcc = arMpccK.GetSize();
    for(int i=0; i<nMpcc; ++i)
        CalcMomentCurvature(arMpccK[i], bSave);
}

int CBrdgSeisEval::CalcMomentCurvature(const T_MPCC_D& MpccD, BOOL bSave, double dAddAngle/* = 0.0*/)
{ 
    T_MPCC_D Code_MpccD = const_cast<T_MPCC_D&>(MpccD);
    T_FIMP_K MatlConcK1Trans = Code_MpccD.MatlConcK1Trans;
    T_FIMP_K MatlConcK2Trans = Code_MpccD.MatlConcK2Trans;
    Code_MpccD.MatlConcK1Trans = 0;
    Code_MpccD.MatlConcK2Trans = 0;

    UINT iCol = Code_MpccD.nEvgpColm;
    // Longitudinal
    DGN_CALC_MPHI_RESULT MPhiResD;
    if(Code_MpccD.bEvgp) Code_MpccD.dAxisAngle = 0.0 + dAddAngle;
    int nError1 = m_MCCalc.CalcMomentCurvature(Code_MpccD, MPhiResD, 0);
    SetMPhiRes(Code_MpccD, iCol, LONGI, (Code_MpccD.bIdealize ? Code_MpccD.nIdealOpt : -1), nError1, MPhiResD, bSave);

    // Transverse
    Code_MpccD.dAxisAngle += 90.0;
    Code_MpccD.MatlConcK1 = Code_MpccD.MatlConcK1Trans = MatlConcK1Trans;
    Code_MpccD.MatlConcK2 = Code_MpccD.MatlConcK2Trans = MatlConcK2Trans;
    int nError2 = m_MCCalc.CalcMomentCurvature(Code_MpccD, MPhiResD, 1);
    SetMPhiRes(Code_MpccD, iCol, TRANS, (Code_MpccD.bIdealize ? Code_MpccD.nIdealOpt : -1), nError2, MPhiResD, bSave);

    return max(nError1, nError2);
}

void CBrdgSeisEval::SetMPhiRes(const T_MPCC_D& MpccD, const UINT& iColm, const EN_LOCAL_DIR& enDir, const int& nIdeal, const int& nError, const DGN_CALC_MPHI_RESULT& MPhiResD, BOOL bSave)
{
    CBrdgSeisEvalResult *pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult(); if(!pResult) { ASSERT(0); }
    SEIS_RES_KEY MphiK = pResult->GetKey(MpccD.EvgpK, iColm, enDir, static_cast<EN_POSITION>(MpccD.nEvgpPos));

    T_BRDGEVAL_MPHI_RES EvalMPhiResD;
    EvalMPhiResD.initialize();

    EvalMPhiResD.StateD.nChk = nError;
    EvalMPhiResD.StateD.dLimitAxialForce = MPhiResD.dLimitAxialForce;
    BOOL bIdeal = nIdeal==-1 ? FALSE : TRUE;

    // 0:Crack, 1:Yield(init.), 2:Yield, 3:Ultimate, 4:Yield(ideal), 5:Ultimate(ideal)
    EvalMPhiResD.StateD.dCurvature[0] = MPhiResD.MPhi_c[0].dCurvature;
    EvalMPhiResD.StateD.dMoment[0]    = MPhiResD.MPhi_c[0].dMoment;
    EvalMPhiResD.StateD.dCurvature[1] = MPhiResD.MPhi_y0[0].dCurvature;
    EvalMPhiResD.StateD.dMoment[1]    = MPhiResD.MPhi_y0[0].dMoment;
    EvalMPhiResD.StateD.dCurvature[2] = MPhiResD.MPhi_y[0].dCurvature;
    EvalMPhiResD.StateD.dMoment[2]    = MPhiResD.MPhi_y[0].dMoment;
    EvalMPhiResD.StateD.dCurvature[3] = MPhiResD.MPhi_u[0].dCurvature;
    EvalMPhiResD.StateD.dMoment[3]    = MPhiResD.MPhi_u[0].dMoment;
    if(bIdeal)
    {
        EvalMPhiResD.StateD.dCurvature[4] = MPhiResD.MPhi_yI[0].dCurvature;
        EvalMPhiResD.StateD.dMoment[4]    = MPhiResD.MPhi_yI[0].dMoment;
        EvalMPhiResD.StateD.dCurvature[5] = MPhiResD.MPhi_uI[0].dCurvature;
        EvalMPhiResD.StateD.dMoment[5]    = MPhiResD.MPhi_uI[0].dMoment;
    }
    else
    {
        EvalMPhiResD.StateD.dCurvature[4] = MPhiResD.MPhi_y[0].dCurvature;
        EvalMPhiResD.StateD.dMoment[4]    = MPhiResD.MPhi_y[0].dMoment;
        EvalMPhiResD.StateD.dCurvature[5] = MPhiResD.MPhi_u[0].dCurvature;
        EvalMPhiResD.StateD.dMoment[5]    = max(MPhiResD.MPhi_u[0].dMoment, MPhiResD.MPhi_y[0].dMoment);
    }

    int nPoint = MPhiResD.arCurvePos.GetSize();
    EvalMPhiResD.arMPhiCruve.SetSize(nPoint);
    for(int i=0; i<nPoint; ++i)
    {
        T_BRDGEVAL_MPHI_UNIT UnitD;
        UnitD.initialize();

        UnitD.dMoment     = MPhiResD.arCurvePos[i].dMoment;
        UnitD.dCurvature  = MPhiResD.arCurvePos[i].dCurvature;
        UnitD.dXn         = MPhiResD.arCurvePos[i].dNeutralAxis;
        UnitD.dConcStrain = MPhiResD.arCurvePos[i].dConcStrain[0];         // [0]:압축  [1]:인장
        UnitD.dStlStrain  = (-1.0)*MPhiResD.arCurvePos[i].dRebarStrain[1]; // [0]:압축  [1]:인장
        UnitD.dConcComp   = MPhiResD.arCurvePos[i].dConcComp;    // 압축콘크리트구간의 길이
        UnitD.dStlComp    = MPhiResD.arCurvePos[i].dRebarComp;   // 압축철근구간의 길이
        UnitD.dStlTens    = MPhiResD.arCurvePos[i].dRebarTen;    // 압축철근구간의 길이

        EvalMPhiResD.arMPhiCruve[i] = UnitD;
    }

    pResult->SetMPhiRes(MphiK, EvalMPhiResD);

    if(bSave)
        pResult->WriteSeisEvalResultMPhi(MpccD.EvgpK, iColm, enDir, static_cast<EN_POSITION>(MpccD.nEvgpPos), EvalMPhiResD);
}

void CBrdgSeisEval::GetCalcParam(const UINT& iGrup, const UINT& iColm, const EN_LOCAL_DIR& enDir, const EN_POSITION& enPos, const bool& bPlainConc, T_BRDGEVAL_CALC_PARAM& CalcParamD)
{
    CalcParamD.initialize();
    
    T_EVGP_D EvgpD;
    if(!m_pDoc->m_pAttrCtrl2->GetEvgp(iGrup, EvgpD)) { ASSERT(0); return; }
    T_ELEM_K ElemK = GetEvgpElemK(iGrup, iColm, enPos, EvgpD);

    double dLocalVector[3][3];
    m_pDoc->calcElemLocalVector(ElemK, dLocalVector);
    BOOL bIend = (enPos == BOTTOM) ? (dLocalVector[0][2] > 0.0) : (dLocalVector[0][2] < 0.0); // Global-Z로 판단.

    T_BSED_D BsedD;
    if(!m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD)) ASSERT(0);

    CalcParamD.OptD.iEvalCode = BsedD.nDesignCode;
    CalcParamD.OptD.iElemK = ElemK;
    CalcParamD.OptD.bIend  = bIend;

    // From DB - 유효높이
    T_EVPM_D EvpmD;
    if(!GetEvpm(iGrup, iColm, EvpmD)) EvpmD.Initialize();
    if(enDir==LONGI)
    {
        if(EvpmD.nBehavL == 0)
        {
            CalcParamD.OptD.enBehavior = RAHMEN;
            CalcParamD.OptD.dHe = enPos==BOTTOM ? EvpmD.dHe1L : EvpmD.dHe2L;
        }
        else 
        {
            CalcParamD.OptD.enBehavior = CANTILEVER;
            CalcParamD.OptD.dHe = enPos==BOTTOM ? EvpmD.dHe1L : 0.0;
        }
    }
    else
    {
        if(EvpmD.nBehavT == 0)
        {
            CalcParamD.OptD.enBehavior = RAHMEN;
            CalcParamD.OptD.dHe = enPos==BOTTOM ? EvpmD.dHe1T : EvpmD.dHe2T;
        }
        else 
        {
            CalcParamD.OptD.enBehavior = CANTILEVER;
            CalcParamD.OptD.dHe = enPos==BOTTOM ? EvpmD.dHe1T : 0.0;
        }
    }

    CalcParamD.OptD.iColm     = iColm;
    CalcParamD.OptD.nBHmethod = EvpmD.nBH;
    CalcParamD.OptD.nSplice   = EvpmD.nSplice;
    CalcParamD.OptD.dlsp      = 0.0;//??//EvpmD
    CalcParamD.OptD.dAeRat    = EvpmD.dAeRat;

    // From DB - 주기 및 통제주기
    double dTl=0.0, dTt=0.0;
    m_pDoc->m_pPostCtrl->GetEiPeriodByDirSeisEval(dTl, dTt);
    CalcParamD.OptD.dT = enDir==LONGI ? dTl : dTt;
    CalcParamD.OptD.dTs = EvpmD.dTs;

    // From DB - 유효강성비
    T_SECT_D SectD;
    const T_SECT_K SectK = GetSectKey(ElemK);
    if(!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD)) { ASSERT(0); return; }

    T_MPCC_K MpccK = GetMpccKey(iGrup, iColm, enPos);
    if(MpccK > 0)
    {
        T_MPCC_D MpccD;
        if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD))     { ASSERT(0); return; }
        if(SectD.nStype == D_SECT_TYPE_TAPERED)
        {
            BOOL bOldCurveType = FALSE;
            int nCalcOption = 1;
            if(MpccD.nPosition == 1)
            {
                // SectSrc를 dPositionJ / dElemLen 위치에서 보간한 정보를 SectOut의 I단에 넣음 
                // (주의!!) SectOut의 I단 정보만 의미 있음!! J단정보는 아무 의미 없음!!
                // (주의!!) Size만 유효 하고 그외 값들은 유효 하지 않음
                T_TSGR_D DataTsgr;
                DataTsgr.Initialize();
                DataTsgr.nVariation_y = 0; // linear
                DataTsgr.nVariation_z = 0; // linear

                T_SECT_D SectTmpD;
                SectTmpD = SectD;
                double dTmpElemLen = 10.0;

                if(!m_pDoc->m_pSectDB->CalcTaperedSection(SectTmpD, DataTsgr, dTmpElemLen, dTmpElemLen / 2.0, dTmpElemLen, nCalcOption, SectD, bOldCurveType, 0))
                {
                    ASSERT(0);
                }
                m_pDoc->m_pSectDB->CalcSectData(SectD);
                bIend = TRUE;
            }
        }
        CalcParamD.OptD.MpccK = MpccK;
    }

    if(SectD.SectBefore.Shape == D_SECT_SHAPE_REG_GEN)
    { 
        double dxMax=-DBL_MAX, dxMin=DBL_MAX;
        double dyMax=-DBL_MAX, dyMin=DBL_MAX;    
        int nOut = SectD.SectBefore.SectI.aOuterPolygon.GetSize();
        for(int i=0; i<nOut; ++i)
        {
            int nPnt = SectD.SectBefore.SectI.aOuterPolygon[i].aVertex.GetSize();
            for(int j=0; j<nPnt; ++j)
            {
                dxMax = max(dxMax, SectD.SectBefore.SectI.aOuterPolygon[i].aVertex[j].dX);
                dxMin = min(dxMin, SectD.SectBefore.SectI.aOuterPolygon[i].aVertex[j].dX);
                dyMax = max(dyMax, SectD.SectBefore.SectI.aOuterPolygon[i].aVertex[j].dY);
                dyMin = min(dyMin, SectD.SectBefore.SectI.aOuterPolygon[i].aVertex[j].dY);
            }
        }
        SectD.SectBefore.SectI.Size[0] = SectD.SectBefore.SectJ.Size[0] = dyMax - dyMin;
        SectD.SectBefore.SectI.Size[1] = SectD.SectBefore.SectJ.Size[1] = dxMax - dxMin;
    }
    
    T_SECF_D SecfD;
    if(!m_pDoc->m_pAttrCtrl->GetSecfSect(SectK, SecfD)) CalcParamD.OptD.dEffStiffRat = 1.0;
    else                                                CalcParamD.OptD.dEffStiffRat = enDir==LONGI ? SecfD.dScaleFactor[4] : SecfD.dScaleFactor[5];

    if(SectD.nStype!=D_SECT_TYPE_TAPERED) bIend = TRUE;
    if(enDir == LONGI)
        CalcParamD.OptD.dEffStiff = CalcParamD.OptD.dEffStiffRat * (bIend ? SectD.SectBefore.SectI.Stiffness.Ryy : SectD.SectBefore.SectJ.Stiffness.Ryy);
    else
        CalcParamD.OptD.dEffStiff = CalcParamD.OptD.dEffStiffRat * (bIend ? SectD.SectBefore.SectI.Stiffness.Rzz : SectD.SectBefore.SectJ.Stiffness.Rzz);

    // Section Data
    CalcParamD.SetcD.strShape = SectD.SectBefore.Shape;
    CalcParamD.SetcD.dArea    = bIend ? SectD.SectBefore.SectI.Stiffness.Area : SectD.SectBefore.SectJ.Stiffness.Area;
    CalcParamD.SetcD.dB       = bIend ? SectD.SectBefore.SectI.Size[1] : SectD.SectBefore.SectJ.Size[1];
    CalcParamD.SetcD.dH       = bIend ? SectD.SectBefore.SectI.Size[0] : SectD.SectBefore.SectJ.Size[0];
	CalcParamD.SetcD.dIy      = bIend ? SectD.SectBefore.SectI.Stiffness.Ryy : SectD.SectBefore.SectJ.Stiffness.Ryy;
	CalcParamD.SetcD.dIz      = bIend ? SectD.SectBefore.SectI.Stiffness.Rzz : SectD.SectBefore.SectJ.Stiffness.Rzz;
    CalcParamD.SetcD.dSy      = bIend ? SectD.SectBefore.SectI.Design.Syy : SectD.SectBefore.SectJ.Design.Syy;
    CalcParamD.SetcD.dSz      = bIend ? SectD.SectBefore.SectI.Design.Szz : SectD.SectBefore.SectJ.Design.Szz;
    if(CalcParamD.SetcD.strShape == D_SECT_SHAPE_REG_P || CalcParamD.SetcD.strShape == D_SECT_SHAPE_REG_SR)
        CalcParamD.SetcD.dB = CalcParamD.SetcD.dH;

    // Material Data
    CalcParamD.MatlD.dfck = Get_fck(iGrup, iColm, enPos, enDir);
    CalcParamD.MatlD.dEc  = Get_Ec(iGrup, iColm, enPos, enDir);
    CalcParamD.MatlD.dfyh = CalcParamD.MatlD.dfy = Get_fyh(iGrup, iColm, enPos);

	double dfckS = 0.0;
	double dfctkS = 0.0;
	double dAxialForceS = 0.0;
	Get_fck_ftk_Shear(iGrup, iColm, enPos, enDir, dfckS, dfctkS, dAxialForceS);
	CalcParamD.MatlD.dfck_S = dfckS;
	CalcParamD.MatlD.dfctk_S = dfctkS;
	CalcParamD.MatlD.dAxialForce_S = dAxialForceS;

    T_MATD_D MatdD;
    if(!GetMatlData(ElemK, MatdD)) {  ASSERT(0);  MatdD.Initialize();  }
    if(CalcParamD.MatlD.dfck < cDgn_Zero) CalcParamD.MatlD.dfck = MatdD.Data1.Design.C_fc;    
    if(CalcParamD.MatlD.dEc  < cDgn_Zero) CalcParamD.MatlD.dEc  = MatdD.Data1.Analysis.Elast;
    if(CalcParamD.MatlD.dfy  < cDgn_Zero) CalcParamD.MatlD.dfy  = MatdD.MainRebarData.B_fy;
    if(CalcParamD.MatlD.dfyh < cDgn_Zero) CalcParamD.MatlD.dfyh = MatdD.SubRebarData.B_fy;
	if(CalcParamD.MatlD.dfck_S < cDgn_Zero) CalcParamD.MatlD.dfck_S = CalcParamD.MatlD.dfck;

    if(bPlainConc)
    {
        CalcParamD.BarD.bPlainConc = bPlainConc;
    }
    else
    {
        // Rebar Data
        //Create_RebarData
        m_pDoc->m_pMatlDB->Create_RebarData(m_CodeUnit.nBase_Length, m_CodeUnit.nBase_Force, TRUE);

        double dAst = 0.0;
        if(CalcParamD.SetcD.strShape == D_SECT_SHAPE_REG_GEN &&
            (SectD.nStype == D_SECT_TYPE_USER || (SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.nStype == D_SECT_TYPE_USER)))
        {
            T_REBT_D GenRbarD;  GenRbarD.Initialize(); // 임의단면 기둥
            if(!m_pDoc->m_pAttrCtrl->GetRebt(SectK, GenRbarD)) ASSERT(0);

            int iIdxRbar = (bIend || GenRbarD.bIJBothMain) ? 0 : 2;
            int iIdxSbar = (bIend || GenRbarD.bIJBothShear) ? 0 : 2;
            T_REBT_MABR Mbar = GenRbarD.arMainRebar[iIdxRbar][0];
            T_REBT_SHBR SBar = GenRbarD.ShearRebar[iIdxSbar];

            CalcParamD.BarD.ddbl = m_pDoc->m_pMatlDB->Get_RebarDia(Mbar.strRebarName);
            CalcParamD.BarD.ddbs = m_pDoc->m_pMatlDB->Get_RebarDia(SBar.strSubRebarName);

            int nRebar = GenRbarD.arMainRebar[iIdxRbar].GetSize();
            if(enDir == LONGI)
            {
                double dMax = -DBL_MAX, dMin = DBL_MAX;
                for(int i = 0; i < nRebar; ++i)
                {
                    dMax = max(dMax, GenRbarD.arMainRebar[iIdxRbar][i].dCenterZ);
                    dMin = min(dMin, GenRbarD.arMainRebar[iIdxRbar][i].dCenterZ);

                    dAst += m_pDoc->m_pMatlDB->Get_RebarArea(GenRbarD.arMainRebar[iIdxRbar][i].strRebarName);
                }
                CalcParamD.BarD.ddc = (CalcParamD.SetcD.dH - (dMax - dMin)) / 2.0;
            }
            else
            {
                double dMax = -DBL_MAX, dMin = DBL_MAX;
                for(int i = 0; i < nRebar; ++i)
                {
                    dMax = max(dMax, GenRbarD.arMainRebar[iIdxRbar][i].dCenterY);
                    dMin = min(dMin, GenRbarD.arMainRebar[iIdxRbar][i].dCenterY);

                    dAst += m_pDoc->m_pMatlDB->Get_RebarArea(GenRbarD.arMainRebar[iIdxRbar][i].strRebarName);
                }
                CalcParamD.BarD.ddc = (CalcParamD.SetcD.dB - (dMax - dMin)) / 2.0;
            }

            GetManderShearData(iGrup, iColm, enPos, enDir, CalcParamD.BarD.dAsp, CalcParamD.BarD.dAv, CalcParamD.BarD.ds, CalcParamD.BarD.nlegs, CalcParamD.BarD.drho);
            if(CalcParamD.BarD.ds < cDgn_Zero) CalcParamD.BarD.ds = SBar.dSubRebarSpace;
            if(CalcParamD.BarD.dAsp < cDgn_Zero) CalcParamD.BarD.dAsp = m_pDoc->m_pMatlDB->Get_RebarArea(SBar.strSubRebarName);
            if(CalcParamD.BarD.dAv < cDgn_Zero) CalcParamD.BarD.dAv = SBar.dSubRebarNum[enDir == LONGI ? 1 : 0] * CalcParamD.BarD.dAsp;
            CalcParamD.BarD.drhosh = EvpmD.dLp;
        }
        else
        {
            T_RCHK_D RchkData; RchkData.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetRchk(SectK, RchkData)) ASSERT(0);
            int iIdx = bIend ? 0 : 2;

            CalcParamD.BarD.ddbl = m_pDoc->m_pMatlDB->Get_RebarDia(RchkData.COLM.strBarNa1[iIdx][0][0]);
            CalcParamD.BarD.ddbs = m_pDoc->m_pMatlDB->Get_RebarDia(RchkData.COLM.strSubBarNa[iIdx]);
            CalcParamD.BarD.ddc = RchkData.COLM.dDc[iIdx][0];
            CalcParamD.BarD.drhosh = EvpmD.dLp;

            for(int i = 0; i < 3; ++i)
            {
                for(int j = 0; j < T_RCHK_COLM_LAYER; ++j)
                {
                    if(RchkData.COLM.iBarNum[iIdx][i][j] == 0) continue;
                    double dAs = m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strBarNa1[iIdx][i][j]);
                    if(!RchkData.COLM.strBarNa2[iIdx][i][j].IsEmpty())
                    {
                        dAs += m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strBarNa2[iIdx][i][j]);
                        dAs /= 2.0;
                    }
                    dAst += (dAs * 2.0 * RchkData.COLM.iBarNum[iIdx][i][j]);
                }
            }

            double dSubBarNum = min(RchkData.COLM.dSubBarNum_y[iIdx], RchkData.COLM.dSubBarNum_z[iIdx]);
            GetManderShearData(iGrup, iColm, enPos, enDir, CalcParamD.BarD.dAsp, CalcParamD.BarD.dAv, CalcParamD.BarD.ds, CalcParamD.BarD.nlegs, CalcParamD.BarD.drho);
            if(CalcParamD.BarD.ds < cDgn_Zero) CalcParamD.BarD.ds = RchkData.COLM.dSubBarDist[iIdx];
            if(CalcParamD.BarD.dAsp < cDgn_Zero) CalcParamD.BarD.dAsp = m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strSubBarNa[iIdx]);
            if(CalcParamD.BarD.dAv < cDgn_Zero) CalcParamD.BarD.dAv = dSubBarNum * CalcParamD.BarD.dAsp;
        }
        CalcParamD.BarD.dAst = dAst;
    }
}

void CBrdgSeisEval::SetCalcParam_AxialForce(const double& dPcol, const double& dPcoltop, const double& dPcolbot, T_BRDGEVAL_FORCE& ForceD)
{
    ForceD.dPtop    = dPcol;
    ForceD.dPcoltop = dPcoltop;
    ForceD.dPcolbot = dPcolbot;
}

void CBrdgSeisEval::GetCalcParam_Force(const UINT& EvgpK, const UINT& iColm, const EN_LOCAL_DIR& enDir, const EN_POSITION& enPos, const UINT& LcomK, const int& nMinMax, T_BRDGEVAL_CALC_PARAM& CalcParamD)
{
	CalcParamD.OptD.iLcomK = LcomK;

    if(LcomK == 0)
	{
		UINT LinkK;
		Calc_PierSeisEnvForce(EvgpK, iColm, enPos, 0, CalcParamD.ForceD);
		Calc_PierSeisEnvDisp(EvgpK, iColm, enDir, 0, CalcParamD.ForceD.dDisp, LinkK);
	}
	else
	{
		// Set force
		T_LCOM_D LcomD; LcomD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, LcomK, LcomD)) ASSERT(0);

		int nRs = 0;
		int nTh = 0;
		m_pDoc->m_pAttrCtrl->GetSeizEvalLcomType(LcomD, nRs, nTh);

		m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);
		T_STRB_D StrbMaxData, StrbMinData, StrbAbs;
		StrbMaxData.Initialize();
		StrbMinData.Initialize();
		StrbAbs.Initialize();
		if(!m_pDoc->m_pPostCtrl->GetStrbNew(CalcParamD.OptD.iElemK, &StrbMaxData, &StrbMinData, &StrbAbs, TRUE))	ASSERT(0);
		for(int i = 0; i < 6; ++i)
		{
			if(nMinMax == 0)  CalcParamD.ForceD.dForce[i] = CalcParamD.OptD.bIend ? StrbMaxData.dblForce[0][i] : StrbMaxData.dblForce[3][6 + i];
			else            CalcParamD.ForceD.dForce[i] = CalcParamD.OptD.bIend ? StrbMinData.dblForce[0][i] : StrbMinData.dblForce[3][6 + i];
		}
		CalcParamD.ForceD.dForce[0] *= -1.0; // (+)압축 (-)인장으로 만들어 줌.
		CalcParamD.ForceD.nLoadType = (nTh > 0) ? 1 : 0;

		CalcParamD.ForceD.dDisp = 0.0;
		T_EVGP_D EvgpD;
		if(m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD))
		{
			// 기둥 상부 축력
			T_ELEM_K ElemTopK = GetEvgpElemK(EvgpK, iColm, 0, EvgpD);
			double dLocalVector[3][3];
			m_pDoc->calcElemLocalVector(ElemTopK, dLocalVector);

			// Get Displacement
			int nNode = EvgpD.aLink.GetSize();
			if(nNode > 0)
			{
				m_pDoc->calcElemLocalVector(CalcParamD.OptD.iElemK, dLocalVector);
				for(int i = 0; i < nNode; ++i)
				{
					T_DISP_D DataMax, DataMin, DataAbs;
					m_pDoc->m_pPostCtrl->GetDispNew(EvgpD.aLink[i], &DataMax, &DataMin, &DataAbs);
					double dDisp = 0.0;
					if(nMinMax == 0)
					{
						CMathFunc::mathGCS2UCS(DataMax.dblDisp[0], DataMax.dblDisp[1], DataMax.dblDisp[2], dLocalVector);
						dDisp = enDir == LONGI ? fabs(DataMax.dblDisp[2]) : fabs(DataMax.dblDisp[1]);
					}
					else
					{
						CMathFunc::mathGCS2UCS(DataMin.dblDisp[0], DataMin.dblDisp[1], DataMin.dblDisp[2], dLocalVector);
						dDisp = enDir == LONGI ? fabs(DataMin.dblDisp[2]) : fabs(DataMin.dblDisp[1]);
					}

					CalcParamD.ForceD.dDisp = max(CalcParamD.ForceD.dDisp, dDisp);
				}
			}
		}
	}
}

void CBrdgSeisEval::GetCalcParam_Force(const UINT& MpccK, const UINT& iColm, const EN_LOCAL_DIR& enDir, const UINT& LcomK, const int& nMinMax, T_BRDGEVAL_CALC_PARAM& CalcParamD)
{
    CalcParamD.OptD.iLcomK = LcomK;

    T_MPCC_D MpccD;
    if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) { ASSERT(0); return; }
    T_EVGP_K EvgpK = MpccD.EvgpK;
    if ( MpccD.nEvgpColm != iColm )  { ASSERT(0); return; }

    const EN_POSITION enPos = MpccD.nEvgpPos == 0 ? TOP : BOTTOM;
    GetCalcParam_Force(EvgpK, iColm, enDir, enPos, LcomK, nMinMax, CalcParamD);
}

EN_BEHAVIOR_TYPE CBrdgSeisEval::GetBehaviorType(const UINT& iGrup, const UINT& iColm, const EN_LOCAL_DIR& enDir)
{
    EN_BEHAVIOR_TYPE enBehavior = NONE_TYPE;

    T_EVPM_D EvpmD;
    if(!GetEvpm(iGrup, iColm, EvpmD)) return enBehavior;

    if(enDir == LONGI)  enBehavior = EvpmD.nBehavL==0 ? RAHMEN : CANTILEVER;
    else                enBehavior = EvpmD.nBehavT==0 ? RAHMEN : CANTILEVER;

    return enBehavior;
}

UINT CBrdgSeisEval::GetMpccKey(const UINT& iGrup, const UINT& iColm, const EN_POSITION& enPos)
{
    POSITION pos = m_pDoc->m_pAttrCtrl2->GetStartMpcc();
    while (pos)
    {
        T_MPCC_K MpccK;
        T_MPCC_D MpccD;
        m_pDoc->m_pAttrCtrl2->GetNextMpcc(pos, MpccK, MpccD);

        if ( MpccD.EvgpK != iGrup )         continue;
        if ( MpccD.nEvgpColm != iColm )     continue;
        if ( enPos != static_cast<EN_POSITION>(MpccD.nEvgpPos) ) continue;

        return MpccK;
    }
    
    return 0;
}

BOOL CBrdgSeisEval::Print_MPhiResult(CString strFileName, CArray<T_MPCC_D, T_MPCC_D&>& arMpccD)
{
    return m_MCCalc.Print_EvgpMPhiResult(strFileName, arMpccD);
}

BOOL CBrdgSeisEval::Print_MPhiResult(CString strFileName, CArray<T_MPCC_K, T_MPCC_K>& arMpccK)
{
    int nMpcc = arMpccK.GetSize();
    CArray<T_MPCC_D, T_MPCC_D&> aMpccD;

    for(int i=0; i<nMpcc; ++i)
    {
        T_MPCC_D MpccD;
        if(!m_pDoc->m_pAttrCtrl2->GetMpcc(arMpccK[i], MpccD)) { ASSERT(0); continue; }  
        aMpccD.Add(MpccD);
    }

    return Print_MPhiResult(strFileName, aMpccD);
}

BOOL CBrdgSeisEval::GetFailureData(const UINT& EvgpK, const UINT& iColm, const EN_LOCAL_DIR& enDir, const EN_POSITION& enPos, T_BRDGEVAL_FAILURE_MODE& FModeD, T_BRDGEVAL_CAPACITY& CapaD)
{
    FModeD.initialize();
    CapaD.initialize();

    CBrdgSeisEvalResult *pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult(); if(!pResult) { ASSERT(0); }
    SEIS_RES_KEY Key = pResult->GetKey(EvgpK, iColm, enDir, enPos);

    T_BRDGEVAL_EVALUATION EvalD;
    EvalD.initialize();
    if(!pResult->ReadSeisEvalResultEval(EvgpK, iColm, enDir, enPos, 0, TRUE, EvalD))
        return FALSE;
    else
        FModeD = EvalD.ModeD;

    if(!pResult->ReadSeisEvalResultCapa(EvgpK, iColm, enDir, enPos, 0, TRUE, CapaD))
        return FALSE;

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CBrdgSeisEval::Write_GDisplayAnalString(CString strTxt, BOOL bClearPre/*TRUE*/, BOOL bSpacePost/*TRUE*/)
{
    GActiveAnalysisViewer();
    if(bClearPre==TRUE)		GClearAnalStrings();
    GDisplayAnalString(strTxt);
    if(bSpacePost==TRUE)	GDisplayAnalString(_T(""));
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calculate Force
double CBrdgSeisEval::Calc_PierCapSuppEnvAxialForce(const T_EVGP_K& EvgpK, const int& iColm, CMapEx<T_ELNK_K, T_ELNK_K, double, double&>& mapElnkRect, CMapEx<T_NLNK_K, T_NLNK_K, double, double&>& mapNlnkRect, BOOL bReport) // gravity load only
{
    mapElnkRect.RemoveAll();
    mapNlnkRect.RemoveAll();
    double dAxial = 0.0;
    UINT iColmIdx = iColm>=0 ? iColm : 0;

	T_EVGP_D EvgpD;
	if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD)) return dAxial;

    bool bPlainConc = [&]()
    {
        if(EvgpD.nType == 1) // 1:Abutment, 교대도 무근콘크리트와 동일하게 처리
            return true;

		T_BSED_D BsedD;
		if(!m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD)) ASSERT(0);
        if(BsedD.nDesignCode == KALIS_2023)
        {
            if(bReport || iColm == -1)
                return !ExistRebar(EvgpK, 0, TOP) && !ExistRebar(EvgpK, 0, BOTTOM);
            else
                return !ExistRebar(EvgpK, iColmIdx, TOP) && !ExistRebar(EvgpK, iColmIdx, BOTTOM);
        }
        return false;
    }();

    T_MPCC_K MpccK = 0;
    T_MPCC_D MpccD;
    MpccD.Initialize();
    if(iColm>=0 && !bPlainConc)
    {
        MpccK = Get_GovernMpcc(EvgpK, iColm);
        if(MpccK==0) return dAxial;
        if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) return dAxial;
    }

    BOOL bLink = TRUE;
    if(EvgpD.nType == 0) // Pier
    {
        if(!EvgpD.bLink) bLink = FALSE;
        else
        {
            if(EvgpD.aLink.GetSize() == 0) bLink = FALSE;
        }
    }
    else
    {
        if(EvgpD.nLink == 0) // Link
        {
            if(EvgpD.aLink.GetSize() == 0) bLink = FALSE;
        }
        else // support
        {
            if(EvgpD.aSupport.GetSize() == 0) bLink = FALSE;
        }
    }
    
    double dCol = (bReport || EvgpD.nType == 1) ? 1.0 : static_cast<double>(EvgpD.aEvgpColm.GetSize());
    BOOL bSetLcom = FALSE;
    if(bReport || iColm==-1)
    {
        if(bPlainConc)
        {
			T_LCOM_D LcomD;
            bSetLcom = Get_DeadLoadLcom(LcomD);
			m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);
            iColmIdx = 0;
        }
        else
        {
            int iMinColm = Set_Lcom4Post(EvgpK);
            bSetLcom = iMinColm == -1 ? TRUE : FALSE;
            if(!bSetLcom) // 사용자 정의만 있는 경우 최소값.
            {
                MpccK = Get_GovernMpcc(EvgpK, iMinColm);
                if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) return dAxial;
            }
            if(iColm == -1)
            {
                iColmIdx = iMinColm;
            }
        }
    }
    else
    {
        if(bPlainConc)
        {
			T_LCOM_D LcomD;
            bSetLcom = Get_DeadLoadLcom(LcomD);
			m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);
		}
        else
        {
            bSetLcom = Set_Lcom4Post(EvgpK, iColmIdx);
        }
    }

    if(!bLink || !bSetLcom)
    { 
        if(iColmIdx==-2 && !bPlainConc) return dAxial;

        dAxial = MpccD.dAxialForce;
        // PierCap 자중계산
        CSelfWgt CalcWeight(m_pDoc);
        if(EvgpD.bPierCap)
        {
            int nPierCap = EvgpD.aPierCap.GetSize();
            for(int i=0; i<nPierCap; ++i)
            {
                T_ELEM_WEIGHT WgtD;
                if(!CalcWeight.GetElemWeight_Frame(EvgpD.aPierCap[i], WgtD)) continue;
                if(bPlainConc)  dAxial += (WgtD.dTotalWeight/dCol); // 초기축력이 없음.
                else            dAxial -= (WgtD.dTotalWeight/dCol);
            }
        }
        if(MpccK == GetMpccKey(EvgpK, iColmIdx, BOTTOM) && EvgpD.aEvgpColm.GetSize() > iColmIdx)
        {
            int nColm = EvgpD.aEvgpColm[iColmIdx].aColumn.GetSize();
            for(int i=0; i<nColm; ++i)
            {
                T_ELEM_WEIGHT WgtD;
                if(!CalcWeight.GetElemWeight_Frame(EvgpD.aEvgpColm[iColmIdx].aColumn[i], WgtD)) continue;
                if(bPlainConc)  dAxial += WgtD.dTotalWeight; // 초기축력이 없음.
                else            dAxial -= WgtD.dTotalWeight;
            }
        }
        return dAxial;
    }

    if(EvgpD.nType == 0 || EvgpD.nLink==0) // pier or link
    {
        CArray<T_ELNK_K, T_ELNK_K> aElnkK, aNlnkK;
        int nLink = m_pDoc->m_pAttrCtrl2->GetLinkList(EvgpD.aLink, aElnkK, aNlnkK);
        int nElnk = aElnkK.GetSize();
        for(int i=0; i<nElnk; ++i)
        {
            T_ELNK_K LinkK = aElnkK[i];
            double dFx = GetLinkForce(LinkK, MpccD.MpccLcom.MnMxType, 0); // minmax 타입에 따라서 0:all, 1:max, 2:min
            dAxial += (dFx/dCol);
            mapElnkRect.SetAt(LinkK, dFx);
        }

        int nNlnk = aNlnkK.GetSize();
        for(int i=0; i<nNlnk; ++i)
        {
            T_NLNK_K LinkK = aNlnkK[i];
            double dFx = GetLinkForce(LinkK, MpccD.MpccLcom.MnMxType, 1); // minmax 타입에 따라서 0:all, 1:max, 2:min
            dAxial += (dFx/dCol);
            mapNlnkRect.SetAt(LinkK, dFx);
        }
    }
    else if(EvgpD.nLink==1)
    {
        INT_PTR nNode = EvgpD.aSupport.GetSize();
        for(int i=0; i<nNode; ++i)
        {
            T_REAC_D ReacD; ReacD.Initialize();
            m_pDoc->m_pPostCtrl->GetReacNew(EvgpD.aSupport[i], &ReacD, NULL, NULL);
            dAxial += (ReacD.dblReac[2]/dCol);
            mapElnkRect.SetAt(EvgpD.aSupport[i], ReacD.dblReac[2]);
        }
    }
    else ASSERT(0);

    return dAxial;
}

double CBrdgSeisEval::Calc_PierColmBotEnvAxialForce(const T_EVGP_K& EvgpK, const UINT& iColm) // gravity load only
{ 
	double dAxial = 0.0;
	UINT iColmIdx = iColm >= 0 ? iColm : 0;
	bool bPlainConc = [&]()
	{
		T_BSED_D BsedD;
		if(!m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD)) ASSERT(0);
		if(BsedD.nDesignCode == KALIS_2023)
		{
			if(iColm == -1)
				return !ExistRebar(EvgpK, 0, TOP) && !ExistRebar(EvgpK, 0, BOTTOM);
			else
				return !ExistRebar(EvgpK, iColmIdx, TOP) && !ExistRebar(EvgpK, iColmIdx, BOTTOM);
		}
		return false;
	}();

    T_MPCC_D MpccD;
    MpccD.Initialize();
    if(bPlainConc)
    {
		T_LCOM_D LcomD;
		if(!Get_DeadLoadLcom(LcomD)) return dAxial;
		m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);
    }
    else
    {
        T_MPCC_K MpccK = Get_GovernMpcc(EvgpK, iColm);
        if(MpccK == 0) return dAxial;

        if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) return dAxial;

        if(!Set_Lcom4Post(EvgpK, iColm))
        {
            dAxial = MpccD.dAxialForce;

            return dAxial;
        }
    }

    T_EVGP_D EvgpD;
    if(m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD))
    {
        T_ELEM_K ElemK = GetEvgpElemK(EvgpK, iColm, 1, EvgpD);

        T_STRB_D StrbMaxData, StrbMinData, StrbAbs;
        StrbMaxData.Initialize();
        StrbMinData.Initialize();
        StrbAbs.Initialize();
        if(!m_pDoc->m_pPostCtrl->GetStrbNew(ElemK,&StrbMaxData,&StrbMinData,&StrbAbs,TRUE))	ASSERT(0);

        double dLocalVector[3][3];
        m_pDoc->calcElemLocalVector(ElemK, dLocalVector);

        BOOL bIend = dLocalVector[0][2] > 0.0 ? TRUE : FALSE; // 일단 Global-Z로 판단.
        // minmax 타입에 따라서 0:all, 1:max, 2:min
        switch (MpccD.MpccLcom.MnMxType)
        {
        case 0:
            dAxial = -1.0 * (bIend ? StrbAbs.dblForce[0][0] : StrbAbs.dblForce[3][6]); // compression is positive.
            break;
        case 1:
            dAxial = -1.0 * (bIend ? StrbMaxData.dblForce[0][0] : StrbMaxData.dblForce[3][6]); // compression is positive.
            break;
        case 2:
            dAxial = -1.0 * (bIend ? StrbMinData.dblForce[0][0] : StrbMinData.dblForce[3][6]); // compression is positive.
            break;
        default:
            dAxial = -1.0 * (bIend ? StrbAbs.dblForce[0][0] : StrbAbs.dblForce[3][6]); // compression is positive.
            break;
        }
    }

    return dAxial;
}

double CBrdgSeisEval::Calc_PierColmTopEnvAxialForce(const T_EVGP_K& EvgpK, const UINT& iColm) // gravity load only
{ 
	double dAxial = 0.0;
	UINT iColmIdx = iColm >= 0 ? iColm : 0;
	bool bPlainConc = [&]()
	{
		T_BSED_D BsedD;
		if(!m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD)) ASSERT(0);
		if(BsedD.nDesignCode == KALIS_2023)
		{
			if(iColm == -1)
				return !ExistRebar(EvgpK, 0, TOP) && !ExistRebar(EvgpK, 0, BOTTOM);
			else
				return !ExistRebar(EvgpK, iColmIdx, TOP) && !ExistRebar(EvgpK, iColmIdx, BOTTOM);
		}
		return false;
	}();

	T_MPCC_D MpccD;
	MpccD.Initialize();
	if(bPlainConc)
	{
		T_LCOM_D LcomD;
		if(!Get_DeadLoadLcom(LcomD)) return dAxial;
		m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);
	}
	else
	{
		T_MPCC_K MpccK = Get_GovernMpcc(EvgpK, iColm);
		if(MpccK == 0) return dAxial;

		if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) return dAxial;

		if(!Set_Lcom4Post(EvgpK, iColm))
		{
			dAxial = MpccD.dAxialForce;

			return dAxial;
		}
	}

    T_EVGP_D EvgpD;
    if(m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD))
    {
        T_ELEM_K ElemK = GetEvgpElemK(EvgpK, iColm, 0, EvgpD);

        T_STRB_D StrbMaxData, StrbMinData, StrbAbs;
        StrbMaxData.Initialize();
        StrbMinData.Initialize();
        StrbAbs.Initialize();
        if(!m_pDoc->m_pPostCtrl->GetStrbNew(ElemK,&StrbMaxData,&StrbMinData,&StrbAbs,TRUE))	ASSERT(0);

        double dLocalVector[3][3];
        m_pDoc->calcElemLocalVector(ElemK, dLocalVector);

        BOOL bIend = dLocalVector[0][2] > 0.0 ? FALSE : TRUE; // 일단 Global-Z로 판단.
        // minmax 타입에 따라서 0:all, 1:max, 2:min
        switch (MpccD.MpccLcom.MnMxType)
        {
        case 0:
            dAxial = -1.0 * (bIend ? StrbAbs.dblForce[0][0] : StrbAbs.dblForce[3][6]); // compression is positive.
            break;
        case 1:
            dAxial = -1.0 * (bIend ? StrbMaxData.dblForce[0][0] : StrbMaxData.dblForce[3][6]); // compression is positive.
            break;
        case 2:
            dAxial = -1.0 * (bIend ? StrbMinData.dblForce[0][0] : StrbMinData.dblForce[3][6]); // compression is positive.
            break;    
        default:
            dAxial = -1.0 * (bIend ? StrbAbs.dblForce[0][0] : StrbAbs.dblForce[3][6]); // compression is positive.
            break;
        }
    }

    return dAxial;
}

void CBrdgSeisEval::Calc_PierSeisEnvForce(const T_EVGP_K& EvgpK, const UINT& iColm, const EN_POSITION& enPos, const int& nType, T_BRDGEVAL_FORCE& ForceD) // Evgp위치에 따라 Mpcc가 정의되므로 - 0:evn, 1:Longi, 2:Trans, 3:Vert
{
    // Envelop nType - 0:All, 1:Longitudinal Leading , 2:Transverse Leading , 3:Vertical Leading

    ForceD.initialize();

    T_ELEM_K ElemK = GetEvgpElemK(EvgpK, iColm, enPos);
    double dLocalVector[3][3];
    m_pDoc->calcElemLocalVector(ElemK, dLocalVector);
    BOOL bIend = (enPos==BOTTOM) ? (dLocalVector[0][2] > 0.0) : (dLocalVector[0][2] < 0.0); // Global-Z로 판단.

	int nRs = 0;
	int nTh = 0;

    T_LCOM_K_LIST arLcomK;
    m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_SEISMIC, arLcomK);
    int nLcom = arLcomK.GetSize();
    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD;
        if(!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, arLcomK[i], LcomD)) continue;
		m_pDoc->m_pAttrCtrl->GetSeizEvalLcomType(LcomD, nRs, nTh);

        if(nType==0)
        {
            if(LcomD.nSeisLcomType == 0) continue;
        }
        else
        {
            if(LcomD.nSeisLcomType != nType) continue;
        }

        m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);
        T_STRB_D StrbMaxData, StrbMinData, StrbAbs;
        StrbMaxData.Initialize();
        StrbMinData.Initialize();
        StrbAbs.Initialize();
        if(!m_pDoc->m_pPostCtrl->GetStrbNew(ElemK,&StrbMaxData,&StrbMinData,&StrbAbs,TRUE))	ASSERT(0);

        for(int j=0; j<6; ++j)
        { 
            double dForce = bIend ? StrbAbs.dblForce[0][j] : StrbAbs.dblForce[3][6+j];
            if(j==0) dForce *= -1.0;
            else     dForce  = fabs(dForce);

            if(ForceD.dForce[j] < dForce) ForceD.dForce[j] = dForce;
        }
    }

	ForceD.nLoadType = (nTh > 0) ? 1 : 0;
}

void CBrdgSeisEval::Calc_PierSeisEnvDisp(const T_EVGP_K& EvgpK, const UINT& iColm, const EN_LOCAL_DIR& enDir, const int& nType, double& dMaxDisp, UINT& MaxLinK, BOOL bBearing, double dTheta)
{
    // Envelop nType - 0:All, 1:Longitudinal Leading , 2:Transverse Leading , 3:Vertical Leading

    dMaxDisp = 0.0;
	MaxLinK = 0;

	T_LCOM_D MaxLcomD;
    T_ELNK_D MaxLinkD;

    T_EVGP_D EvgpD;
    if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD)) { ASSERT(0); return; }

    double dLocalVector[3][3] = { {0.0, 0.0, 1.0},
                                  {0.0, 1.0, 0.0},
                                  {1.0, 0.0, 0.0}};
    if(EvgpD.nType==0) // only pier
    {
        T_ELEM_K ElemK = GetEvgpElemK(EvgpK, iColm, 0, EvgpD);
        m_pDoc->calcElemLocalVector(ElemK, dLocalVector);
    }

    T_LCOM_K_LIST arLcomK;
    m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_SEISMIC, arLcomK);
    int nLcom = arLcomK.GetSize();
    for(int nL=0; nL <nLcom; ++nL)
    {
        T_LCOM_D LcomD;
        if(!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, arLcomK[nL], LcomD)) continue;

        if(nType==0)
        {
            if(LcomD.nSeisLcomType == 0) continue;
        }
        else
        {
            if(LcomD.nSeisLcomType != nType) continue;
        }

        m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);

        if(bBearing) // Bearing
        {
			//link의  beta angle : 0 일 때에 elastic link : z축(교축방향)
            //link의  beta angle : 0 일 때에 elastic link : y축(교축직각방향)

            // Get Relative Displacement
            CArray<T_ELNK_K, T_ELNK_K> aElnkK, aNlnkK;
            int nLink = m_pDoc->m_pAttrCtrl2->GetLinkList(EvgpD.aLink, aElnkK, aNlnkK);
            int nElnk = aElnkK.GetSize();
            for(int i=0; i<nElnk; ++i)
            {
                T_ELNK_K LinkK = aElnkK[i];
                T_ELNK_D LinkD;
                if(!m_pDoc->m_pAttrCtrl->GetElnk(LinkK, LinkD)) continue;
                T_DISP_D Node1DsipMax, Node2DsipMax;
				T_DISP_D Node1DsipMin, Node2DsipMin;
				m_pDoc->m_pPostCtrl->GetDispNew(LinkD.Node1, &Node1DsipMax, &Node1DsipMin, NULL);
				m_pDoc->m_pPostCtrl->GetDispNew(LinkD.Node2, &Node2DsipMax, &Node2DsipMin, NULL);

				T_NODE_D Node1D, Node2D;
				if (!m_pDoc->m_pAttrCtrl->GetNode(LinkD.Node1, Node1D)) continue;
				if (!m_pDoc->m_pAttrCtrl->GetNode(LinkD.Node2, Node2D)) continue;
                if (!m_pDoc->calcFrameLocalVector(Node1D, Node2D, LinkD.dBetaAngle, dLocalVector)) continue;

                for(int j=0; j<DISP_SIZE; ++j)
				{
                    Node1DsipMax.dblDisp[j] -= Node2DsipMax.dblDisp[j]; // relative displacement
					Node1DsipMin.dblDisp[j] -= Node2DsipMin.dblDisp[j]; // relative displacement
				}

                CMathFunc::mathGCS2UCS(Node1DsipMax.dblDisp[0], Node1DsipMax.dblDisp[1], Node1DsipMax.dblDisp[2], dLocalVector);
				CMathFunc::mathGCS2UCS(Node1DsipMin.dblDisp[0], Node1DsipMin.dblDisp[1], Node1DsipMin.dblDisp[2], dLocalVector);
                Node1DsipMax.Abs();
				Node1DsipMin.Abs();
                ConvertToBearing(dTheta, Node1DsipMax.dblDisp[1], Node1DsipMax.dblDisp[2]);
				ConvertToBearing(dTheta, Node1DsipMin.dblDisp[1], Node1DsipMin.dblDisp[2]);

                double dDisp1 = enDir==LONGI ? fabs(Node1DsipMax.dblDisp[2]) : fabs(Node1DsipMax.dblDisp[1]);
				double dDisp2 = enDir==LONGI ? fabs(Node1DsipMin.dblDisp[2]) : fabs(Node1DsipMin.dblDisp[1]);
                double dDisp  = max(dDisp1, dDisp2);
				if (dMaxDisp < dDisp)
				{
					dMaxDisp = dDisp;
					MaxLinK = LinkK;

                    MaxLcomD = LcomD;
                    MaxLinkD = LinkD;
				}
            }

            int nNlnk = aNlnkK.GetSize();
            for(int i=0; i<nNlnk; ++i)
            {
                T_NLNK_K LinkK = aNlnkK[i];
                T_NLNK_D LinkD;
                if(!m_pDoc->m_pAttrCtrl->GetNlnk(LinkK, LinkD)) continue;
                T_DISP_D Node1DsipMax, Node2DsipMax;
				T_DISP_D Node1DsipMin, Node2DsipMin;
				m_pDoc->m_pPostCtrl->GetDispNew(LinkD.Node1, &Node1DsipMax, &Node1DsipMin, NULL);
				m_pDoc->m_pPostCtrl->GetDispNew(LinkD.Node2, &Node2DsipMax, &Node2DsipMin, NULL);

				T_NODE_D Node1D, Node2D;
				if (!m_pDoc->m_pAttrCtrl->GetNode(LinkD.Node1, Node1D)) continue;
				if (!m_pDoc->m_pAttrCtrl->GetNode(LinkD.Node2, Node2D)) continue;
				if (!m_pDoc->calcFrameLocalVector(Node1D, Node2D, LinkD.dBetaAngle, dLocalVector)) continue;

				for (int j = 0; j < DISP_SIZE; ++j)
				{
					Node1DsipMax.dblDisp[j] -= Node2DsipMax.dblDisp[j]; // relative displacement
					Node1DsipMin.dblDisp[j] -= Node2DsipMin.dblDisp[j]; // relative displacement
				}

                CMathFunc::mathGCS2UCS(Node1DsipMax.dblDisp[0], Node1DsipMax.dblDisp[1], Node1DsipMax.dblDisp[2], dLocalVector);
				CMathFunc::mathGCS2UCS(Node1DsipMin.dblDisp[0], Node1DsipMin.dblDisp[1], Node1DsipMin.dblDisp[2], dLocalVector);
                ConvertToBearing(dTheta, Node1DsipMax.dblDisp[1], Node1DsipMax.dblDisp[2]);
				ConvertToBearing(dTheta, Node1DsipMin.dblDisp[1], Node1DsipMin.dblDisp[2]);

                double dDisp1 = enDir==LONGI ? fabs(Node1DsipMax.dblDisp[2]) : fabs(Node1DsipMax.dblDisp[1]);
				double dDisp2 = enDir==LONGI ? fabs(Node1DsipMin.dblDisp[2]) : fabs(Node1DsipMin.dblDisp[1]);
				double dDisp  = max(dDisp1, dDisp2);
				if (dMaxDisp < dDisp)
				{
					dMaxDisp = dDisp;
					MaxLinK = LinkK;
				}
            }
        }
        else
        {
            // Get Displacement
            int nNode = EvgpD.aLink.GetSize();
            for(int i=0; i<nNode; ++i)
            {
                T_DISP_D DataMax, DataMin, DataAbs;
                m_pDoc->m_pPostCtrl->GetDispNew(EvgpD.aLink[i], &DataMax, &DataMin, &DataAbs);
                CMathFunc::mathGCS2UCS(DataMax.dblDisp[0], DataMax.dblDisp[1], DataMax.dblDisp[2], dLocalVector);
				CMathFunc::mathGCS2UCS(DataMin.dblDisp[0], DataMin.dblDisp[1], DataMin.dblDisp[2], dLocalVector);

                double dDisp1 = enDir==LONGI ? fabs(DataMax.dblDisp[2]) : fabs(DataMax.dblDisp[1]);
				double dDisp2 = enDir==LONGI ? fabs(DataMin.dblDisp[2]) : fabs(DataMin.dblDisp[1]);
				double dDisp = max(dDisp1, dDisp2);
                dMaxDisp = max(dMaxDisp, dDisp);
            }
        }
    }
}

void CBrdgSeisEval::ConvertToBearing(const double& dDegree, double& dVal1, double& dVal2)
{
	double dRad = 0.0; // dDegree * CMathFunc::m_trrad; // 교축직각방향과 이루는 각 => 상대각도 적용이후 이 부분은 원래값 적용
    double dCos = fabs(cos(dRad)); // CIVIL-1030 강도개념으로 분력 계산
    double dSin = fabs(sin(dRad)); // CIVIL-1030 강도개념으로 분력 계산

    double dValue[2] = {dVal1, dVal2};

    dVal1 = dValue[0]*dCos + dValue[1]*dSin;
    dVal2 = dValue[0]*dSin + dValue[1]*dCos;
}

T_MPCC_K CBrdgSeisEval::Get_GovernMpcc(const T_EVGP_K& EvgpK, const UINT& iColm)
{
    T_MPCC_K MpccK = GetMpccKey(EvgpK, iColm, BOTTOM);
    if(MpccK == 0) MpccK = GetMpccKey(EvgpK, iColm, TOP);

    return MpccK;
}

BOOL CBrdgSeisEval::Get_GravityLoadCase(const T_EVGP_K& EvgpK, const UINT& iColm, int& nLoadCaseType, int& nLoadCaseKey)
{
    // moment curvature 입력창에서 상하부의 load combination을 다르게 입력하는 경우에는 bottom 의 정보로 평가를 진행합니다. (20190128, 곽용기, 이정우)
    nLoadCaseType = D_LOADCASE_COMB_SEISMIC; 
    nLoadCaseKey  = 0;

    T_MPCC_K MpccK = Get_GovernMpcc(EvgpK, iColm);
    if(MpccK == 0) return FALSE;

    T_MPCC_D MpccD;
    if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) { ASSERT(0); return FALSE; }

    if(MpccD.nAxialForce == 1) return FALSE;
    nLoadCaseType = MpccD.MpccLcom.AnalType;
    nLoadCaseKey  = MpccD.MpccLcom.LoadCaseKey;

    if(nLoadCaseKey==0) return FALSE;

    return TRUE;
}

BOOL CBrdgSeisEval::Set_Lcom4Post(const int& nLoadCaseType, const int& nLoadCaseKey)
{
    T_LCOM_D LcomD;
    if(!m_pDoc->m_pPostCtrl->MakeLoadComb(nLoadCaseType, nLoadCaseKey, LcomD)) return FALSE;
    m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);

    return TRUE;
};

BOOL CBrdgSeisEval::Set_Lcom4Post(const T_EVGP_K& EvgpK, const UINT& iColm)
{
    int nLoadCaseType = D_LOADCASE_COMB_SEISMIC;
    int nLoadCaseKey  = 0;
    if(!Get_GravityLoadCase(EvgpK, iColm, nLoadCaseType, nLoadCaseKey)) return FALSE;
    if(!Set_Lcom4Post(nLoadCaseType, nLoadCaseKey)) return FALSE;

    return TRUE;
}

bool CBrdgSeisEval::Get_DeadLoadLcom(T_LCOM_D& LcomD)
{
    LcomD.Initialize();

    T_LCOM_K_LIST aLcomK;
    m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_SEISMIC, aLcomK);
    const int nLcom = aLcomK.GetSize();
    for(int i = 0; i < nLcom; ++i)
    {
        if(!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, aLcomK[i], LcomD)) continue;
        if(LcomD.LoadCombType != 1) continue;
        if(LcomD.Description == _T("Gravity Envelope")) 
            return true; // Gravity Envelope
    }

    // if no exist Gravity Envelope
    LcomD.Initialize();
    T_STLD_K_LIST aStldK;
    m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
    const auto nStld = aStldK.GetSize();
    for(auto i = 0; i < nStld; ++i)
    {
        T_STLD_D StldD;
		if (!m_pDoc->m_pAttrCtrl->GetStld(aStldK[i], StldD)) continue;
        if (!CDBLib::IsStldDL(StldD.LoadCaseType)) continue;
        T_LCOM_BASE LcomBase;
        LcomBase.AnalType = D_LCOM_STATIC;
        LcomBase.LoadCaseKey = aStldK[i];
        LcomBase.Factor = 1.0;
        LcomD.aCombination.Add(LcomBase);
	}

    return LcomD.aCombination.GetSize() > 0 ? true : false;
}

int CBrdgSeisEval::Get_GravityLoadCase(const T_EVGP_K& EvgpK, int& nLoadCaseType, int& nLoadCaseKey)
{
    // 축력이 최소인...
    nLoadCaseType = D_LOADCASE_COMB_SEISMIC; 
    nLoadCaseKey  = 0;

    double dMinAxial = DBL_MAX;
    double dMinAxialUser = DBL_MAX;

    T_EVGP_D EvgpD;
    if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD)) return FALSE;
    INT_PTR nColm = EvgpD.aEvgpColm.GetSize();
    int iColm = -2;
    for(int i=0; i<nColm; ++i)
    {
        T_MPCC_K MpccK = Get_GovernMpcc(EvgpK, i);
        if(MpccK == 0) continue;

        T_MPCC_D MpccD;
        if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) continue;
        if(MpccD.nAxialForce==1)
        {
            // 사용자 정의 최소값
            if(dMinAxialUser > fabs(MpccD.dAxialForce))
            {
                dMinAxialUser = fabs(MpccD.dAxialForce);
                iColm = i;
            }
            continue; // User
        }
        if(dMinAxial > fabs(MpccD.dAxialForce))
        {
            dMinAxial = fabs(MpccD.dAxialForce);
            nLoadCaseType = MpccD.MpccLcom.AnalType;
            nLoadCaseKey  = MpccD.MpccLcom.LoadCaseKey;
        }
    }

    if(nLoadCaseKey==0) return iColm; // 사용자 정의만 있는 경우

    return -1; // 하중조합 or 조건 선택된 경우
}


int CBrdgSeisEval::Set_Lcom4Post(const T_EVGP_K& EvgpK)
{
    int nLoadCaseType = D_LOADCASE_COMB_SEISMIC;
    int nLoadCaseKey  = 0;
    int iColm = Get_GravityLoadCase(EvgpK, nLoadCaseType, nLoadCaseKey);
    if(!Set_Lcom4Post(nLoadCaseType, nLoadCaseKey)) return FALSE;

    return iColm; // 사용자 정의만 있는 경유 iColm>=0
}

int CBrdgSeisEval::Get_RSList(const T_LCOM_K& LcomK, CArray<T_SPLC_K, T_SPLC_K>& raSplcKey)
{
    raSplcKey.RemoveAll();

    T_LCOM_D LcomD;
    if(!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, LcomK, LcomD)) return 0;
    if (LcomD.LoadCombType == 1) return 0; // Envelope

    int nComb = LcomD.aCombination.GetSize();
    for(int i=0; i<nComb; ++i)
    {
        if(LcomD.aCombination[i].AnalType == D_LCOM_SPECTRUM) raSplcKey.Add(LcomD.aCombination[i].LoadCaseKey);
        else if(LcomD.aCombination[i].AnalType == D_LCOM_CB_SEISMIC)
        {
            CArray<T_SPLC_K, T_SPLC_K> aKey;
            int nSplc = Get_RSList(LcomD.aCombination[i].LoadCaseKey, aKey);
            if(nSplc>0) raSplcKey.Append(aKey);
        }
    }

    CArrayUtil::RemoveDup(raSplcKey, TRUE);
    return raSplcKey.GetSize();
}

int CBrdgSeisEval::Get_RSList(CArray<T_SPLC_K, T_SPLC_K>& raSplcKey)
{
    raSplcKey.RemoveAll();

    T_LCOM_K_LIST arLcom;
    m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_SEISMIC, arLcom);
    int nLcom = arLcom.GetSize();

    for(int i=0; i<nLcom; ++i)
    {
        CArray<T_SPLC_K, T_SPLC_K> aKey;
        int nSplc = Get_RSList(arLcom[i], aKey);
        if(nSplc>0) raSplcKey.Append(aKey);
    }

    CArrayUtil::RemoveDup(raSplcKey, TRUE);
    return raSplcKey.GetSize();
}

int CBrdgSeisEval::Get_RSListByDir(CArray<T_SPLC_K, T_SPLC_K>& raXSplcK, CArray<T_SPLC_K, T_SPLC_K>& raYSplcK, CArray<T_SPLC_K, T_SPLC_K>& raZSplcK)
{
    raXSplcK.RemoveAll();
    raYSplcK.RemoveAll();
    raZSplcK.RemoveAll();

    CArray<T_SPLC_K, T_SPLC_K> aSplcK;
    aSplcK.RemoveAll();
    int nSplc = Get_RSList(aSplcK);
    for(int i=0; i<nSplc; ++i)
    {
        int nDir = 0;
        if(m_pDoc->m_pAttrCtrl->GetBridgeDir(aSplcK[i], nDir))
        {
            if(nDir==0)
                raXSplcK.Add(aSplcK[i]);
            else
                raYSplcK.Add(aSplcK[i]);
        }
        else
            raZSplcK.Add(aSplcK[i]);
    }

    return nSplc;
}

void CBrdgSeisEval::Calc_PierSeisRsForce(const T_EVGP_K& EvgpK, const UINT& iColm, const int& nSplcK, T_BRDGEVAL_FORCE ForceD[2])
{
    ForceD[0].initialize(); // bottom
    ForceD[1].initialize(); // top

    T_EVGP_D EvgpD;
    if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD)) { ASSERT(0); return; }

    T_ELEM_K ElemK[2] = {0};
    ElemK[0] = GetEvgpElemK(EvgpK, iColm, 1, EvgpD);
    ElemK[1] = GetEvgpElemK(EvgpK, iColm, 0, EvgpD);

    if(!Set_Lcom4Post(D_LOADCASE_SPECTRUM, nSplcK)) return;
    for(int i=0; i<2; ++i)
    {
        double dLocalVector[3][3];
        m_pDoc->calcElemLocalVector(ElemK[i], dLocalVector);
        BOOL bIend = (i==0) ? (dLocalVector[0][2] > 0.0) : (dLocalVector[0][2] < 0.0); // Global-Z로 판단.

        T_STRB_D StrbMaxData, StrbMinData, StrbAbs;
        StrbMaxData.Initialize();
        StrbMinData.Initialize();
        StrbAbs.Initialize();
        if(!m_pDoc->m_pPostCtrl->GetStrbNew(ElemK[i],&StrbMaxData,&StrbMinData,&StrbAbs,TRUE))	ASSERT(0);
        for(int j=0; j<6; ++j)
            ForceD[i].dForce[j] = bIend ? StrbAbs.dblForce[0][j] : StrbAbs.dblForce[3][6+j];

        if(ElemK[0] == ElemK[1])
        {
            for(int j=0; j<6; ++j)
                ForceD[1].dForce[j] = bIend ? StrbAbs.dblForce[3][6+j] : StrbAbs.dblForce[0][j];

            break;
        }
    }
}

void CBrdgSeisEval::Calc_PierSeisRsDisp(const T_EVGP_K& EvgpK, const UINT& iColm, const int& nSplcK, double& dDispLong, double& dDsipTrans)
{
    dDispLong = dDsipTrans = 0.0;

    T_EVGP_D EvgpD;
    if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD)) { ASSERT(0); return; }

    T_ELEM_K ElemK = GetEvgpElemK(EvgpK, iColm, 0, EvgpD);
    double dLocalVector[3][3];
    m_pDoc->calcElemLocalVector(ElemK, dLocalVector);

    if(!Set_Lcom4Post(D_LOADCASE_SPECTRUM, nSplcK)) return;

    // Get Displacement
    int nNode = EvgpD.aLink.GetSize();
    for(int i=0; i<nNode; ++i)
    {
        T_DISP_D DataMax, DataMin, DataAbs;
        m_pDoc->m_pPostCtrl->GetDispNew(EvgpD.aLink[i], &DataMax, &DataMin, &DataAbs);
        CMathFunc::mathGCS2UCS(DataMax.dblDisp[0], DataMax.dblDisp[1], DataMax.dblDisp[2], dLocalVector);
        dDispLong  = max(dDispLong, fabs(DataMax.dblDisp[2]));
        dDsipTrans = max(dDsipTrans, fabs(DataMax.dblDisp[1]));
    }
}

BOOL CBrdgSeisEval::Check_MPhiRes(const T_EVGP_K& EvgpK, const UINT& iColm, const EN_LOCAL_DIR& enDir, const EN_POSITION& enPos)
{
    CBrdgSeisEvalResult *pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult(); if(!pResult) { ASSERT(0); return FALSE; }
    T_BRDGEVAL_MPHI_STATE MPhiStateD;
    SEIS_RES_KEY SeisResK = pResult->GetKey(EvgpK, iColm, enDir, enPos);
    if(!pResult->GetMPhiState(SeisResK, MPhiStateD)) return FALSE;

    if(fabs(MPhiStateD.dCurvature[4]) < 1.0E-9 || fabs(MPhiStateD.dCurvature[5]) < 1.0E-9) return FALSE;
    if(fabs(MPhiStateD.dMoment[4]   ) < 1.0E-7 || fabs(MPhiStateD.dMoment[5]   ) < 1.0E-7) return FALSE;

    return TRUE;
};

double CBrdgSeisEval::GetLinkForce(const T_ELNK_K& LinkK, const int& nMinMax, int nType) // 0:elastic, 1:general
{ 
    double dFx = 0.0; // Link의 축력 사용

    T_STEK_D StekMax; StekMax.Initialize();
    T_STEK_D StekMin; StekMin.Initialize();
    T_STEK_D StekAbs; StekAbs.Initialize();
    if(nType==0)
        m_pDoc->m_pPostCtrl->GetStekNew(LinkK, &StekMax, &StekMin, &StekAbs); // Load Combi에 대한 Link Force     
    else if(nType==1)
        m_pDoc->m_pPostCtrl->GetNlnkNew(LinkK, &StekMax, &StekMin, &StekAbs); // Load Combi에 대한 Link Force     
    else ASSERT(0);

    switch(nMinMax)
    {
    case 0:
        dFx = -StekAbs.dblForce[0][0]; // compression is plus
        break;
    case 1:
        dFx = -StekMax.dblForce[0][0]; // compression is plus
        break;
    case 2:
        dFx = -StekMin.dblForce[0][0]; // compression is plus
        break;
    default:
        dFx = -StekAbs.dblForce[0][0]; // compression is plus
        break;
    }

    return dFx;
}

BOOL CBrdgSeisEval::IsHorizontalLink(const T_NODE_K& NodeK1, const T_NODE_K& NodeK2)
{
    T_NODE_D NodeD1, NodeD2;
    if(!m_pDoc->m_pAttrCtrl->GetNode(NodeK1, NodeD1)) return TRUE;
    if(!m_pDoc->m_pAttrCtrl->GetNode(NodeK2, NodeD2)) return TRUE;

    return m_pDoc->m_pAttrCtrl->IsHorizontalFrame(NodeD1, NodeD2);
}

BOOL CBrdgSeisEval::IsRAHMEN(const T_EVGP_K& EvgpK, const UINT& iColm)
{
    T_EVPM_D EvpmD;
    if(!GetEvpm(EvgpK, iColm, EvpmD)) return FALSE;

    return (EvpmD.nBehavL == 0 || EvpmD.nBehavT == 0);
};

void CBrdgSeisEval::GetManderShearData(const T_EVGP_K& EvgpK, const UINT& iColm, const EN_POSITION& enPos, const EN_LOCAL_DIR& enDir, double& dAsp, double& dAv, double& ds, int& nRebarnum, double& dLongiRebarRatio)
{
    dAsp = 0.0;
    dAv  = 0.0;
    ds   = 0.0;

    T_EVGP_D EvgpD;
    if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD)) return;
    T_MPCC_K MpccK = GetMpccKey(EvgpK, iColm, enPos);
    T_MPCC_D MpccD;
    if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) return;

    T_FIMP_K FimpConc1K, FimpConc2K;
    if(enDir == LONGI)
    {
        FimpConc1K = MpccD.MatlConcK1;
        FimpConc2K = MpccD.MatlConcK2;
    }
    else
    {
        FimpConc1K = MpccD.MatlConcK1Trans;
        FimpConc2K = MpccD.MatlConcK2Trans;
    }

    T_FIMP_D FimpConc1, FimpConc2;
    if(!m_pDoc->m_pAttrCtrl->GetFimp(FimpConc1K, FimpConc1)) return;
    if(FimpConc1.nMaterialType != 0) return; // 0=Concrete
    if(FimpConc1.nHysModelType != 6) return; // 6:Mander Model
    if(!m_pDoc->m_pAttrCtrl->GetFimp(FimpConc2K, FimpConc2)) return;

    dAsp = FimpConc2.PROP.CON_MANDER.dGeneralRebarArea;
    if(FimpConc2.PROP.CON_MANDER.nSectRebarType == 1 || FimpConc2.PROP.CON_MANDER.nSectRebarType == 3) // circle
        dAv  = 2.0*dAsp;
    else
        dAv  = enDir==LONGI ? FimpConc2.PROP.CON_MANDER.dGeneralAsx : FimpConc2.PROP.CON_MANDER.dGeneralAsy;
    ds   = FimpConc2.PROP.CON_MANDER.dHoopsSpaceS1;

	nRebarnum = enDir == LONGI ? FimpConc2.PROP.CON_MANDER.nGeneralRebarNumX : FimpConc2.PROP.CON_MANDER.nGeneralRebarNumY;

	dLongiRebarRatio = FimpConc2.PROP.CON_MANDER.dLongiRebarRatio;
}

double CBrdgSeisEval::Get_fck(const T_EVGP_K& EvgpK, const UINT& iColm, const EN_POSITION& enPos, const EN_LOCAL_DIR& enDir)
{
    double dfck = 0.0;
    T_EVGP_D EvgpD;
    if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD)) return dfck;
    T_MPCC_K MpccK = GetMpccKey(EvgpK, iColm, enPos);
    T_MPCC_D MpccD;
    if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) return dfck;

    T_FIMP_K FimpConcK = (enDir == LONGI) ? MpccD.MatlConcK1 : MpccD.MatlConcK1Trans;
    dfck = Get_fck(FimpConcK);

    return dfck;
}

void CBrdgSeisEval::Get_fck_ftk_Shear(const T_EVGP_K& EvgpK, const UINT& iColm, const EN_POSITION& enPos, const EN_LOCAL_DIR& enDir, double& dfckS, double& dfctkS, double& dAxialForceS)
{
	dfckS = 0.0; 
	dfctkS = 0.0;
	dAxialForceS = 0.0;

	T_EVGP_D EvgpD;
	if (!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD)) return;
	T_MPCC_K MpccK = GetMpccKey(EvgpK, iColm, enPos);
	T_MPCC_D MpccD;
	if (!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) return;

	T_FIMP_K FimpConc1K, FimpConc2K;
	if (enDir == LONGI)
	{
		FimpConc1K = MpccD.MatlConcK1;
		FimpConc2K = MpccD.MatlConcK2;
	}
	else
	{
		FimpConc1K = MpccD.MatlConcK1Trans;
		FimpConc2K = MpccD.MatlConcK2Trans;
	}

	T_FIMP_D FimpConc1, FimpConc2;
	if (!m_pDoc->m_pAttrCtrl->GetFimp(FimpConc1K, FimpConc1)) return;
	if (FimpConc1.nMaterialType != 0) return; // 0=Concrete
	if (FimpConc1.nHysModelType != 6) return; // 6:Mander Model
	if (!m_pDoc->m_pAttrCtrl->GetFimp(FimpConc2K, FimpConc2)) return;

	dfckS = FimpConc2.PROP.CON_MANDER.dConfinedConcStrengthfcc; // 전단검토용 압축강도
	dfctkS = FimpConc1.PROP.CON_MANDER.dTensConcDataft;         // 전단검토용 인장강도
	dAxialForceS = MpccD.dAxialForce;                           // 전단검토용 축력
}

double CBrdgSeisEval::Get_fck(const T_FIMP_K& FimpConcK)
{
    T_FIMP_D FimpConcD;
    if(!m_pDoc->m_pAttrCtrl->GetFimp(FimpConcK, FimpConcD)) return 0.0;

    return FimpConcD.Get_fc();
}

double CBrdgSeisEval::Get_fyh(const T_EVGP_K& EvgpK, const UINT& iColm, const EN_POSITION& enPos)
{
    T_EVGP_D EvgpD;
    if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD)) return 0.0;
    T_MPCC_K MpccK = GetMpccKey(EvgpK, iColm, enPos);
    T_MPCC_D MpccD;
    if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) return 0.0;

    T_FIMP_K FimpStlK = MpccD.MatlSteelK;
    T_FIMP_D FimpStlD;
    if(!m_pDoc->m_pAttrCtrl->GetFimp(FimpStlK, FimpStlD)) return 0.0;

    return FimpStlD.Get_fy();
}

double CBrdgSeisEval::Get_Ec(const T_EVGP_K& EvgpK, const UINT& iColm, const EN_POSITION& enPos, const EN_LOCAL_DIR& enDir)
{
    T_EVGP_D EvgpD;
    if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD)) return 0.0;
    T_MPCC_K MpccK = GetMpccKey(EvgpK, iColm, enPos);
    T_MPCC_D MpccD;
    if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) return 0.0;

    T_FIMP_K FimpConcK = (enDir == LONGI) ? MpccD.MatlConcK1 : MpccD.MatlConcK1Trans;
    return Get_Ec(FimpConcK);
}

double CBrdgSeisEval::Get_Ec(const T_FIMP_K& FimpConcK)
{
    double dEc = 0.0;
    T_FIMP_D FimpConcD;
    if(!m_pDoc->m_pAttrCtrl->GetFimp(FimpConcK, FimpConcD)) return dEc;
    if(FimpConcD.nMaterialType != 0)  return dEc;

    switch (FimpConcD.nHysModelType)
    {
    case 0: // 0:Kent&Park
        {
            T_FIMP_CON_KENTPK ConcD = FimpConcD.PROP.CON_KENTPK;
            dEc = fabs(ConcD.dE0) < cDGN_Zero ? 0.0 : 2.0*ConcD.dK*ConcD.dFc / ConcD.dE0;
        }
        break;
    case 1: // 1:JP Conc. Standard Spec
        {
            T_FIMP_CON_STANJP ConcD = FimpConcD.PROP.CON_STANJP;
            dEc = fabs(ConcD.dEpeak) < cDGN_Zero ? 0.0 : 2.0*ConcD.dFc / ConcD.dEpeak;
        }
        break;
    case 2: // 2:JP Roadway Spec. H14
        {
            T_FIMP_CON_ROADJP ConcD = FimpConcD.PROP.CON_ROADJP;
            dEc = ConcD.dEc;
        }
        break;
    case 3: // 3:Nagoya
        {
            T_FIMP_CON_NAGOYA ConcD = FimpConcD.PROP.CON_NAGOYA;
            dEc = fabs(ConcD.dEpsiloncc) < cDGN_Zero ? 0.0 : 2.0*ConcD.dSigmack / ConcD.dEpsiloncc;
        }
        break;
    case 4: // 4:Trilinear Concrete Model
        {
            T_FIMP_CON_TRILIN ConcD = FimpConcD.PROP.CON_TRILIN;
            if ( ConcD.nMethod == 0 )
            {// Sigma-Epsilon
                dEc = fabs(ConcD.dEpsilony[0]) < cDGN_Zero ? 0.0 : ConcD.dSigmay[0] / ConcD.dEpsilony[0];
            }
            else
            {// Sigma-Alpha
                dEc = ConcD.dK[0];
            }
        }
        break;
    case 5: // 5:China (GB50010-02)
        {
            T_FIMP_CON_GB1002 ConcD = FimpConcD.PROP.CON_GB1002;
            double dFck = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, ConcD.dFck);
            double dAlpha_d = max(0.00001, 0.157*pow(dFck, 0.785) - 0.905); // 단위계
            dEc = dAlpha_d * ConcD.dFck;
        }
        break;
    case 6: // 6:Mander Model
        {
            T_FIMP_CON_MANDER ConcD = FimpConcD.PROP.CON_MANDER;
            dEc = ConcD.dConcDataEc;
        }
        break;
    case 7: // 7:JP Roadway Spec. H24
        {
            T_FIMP_CON_ROADJP ConcD = FimpConcD.PROP.CON_ROADJP_H24;
            dEc = ConcD.dEc;
        }
        break;
    case 8: // 8:China (GB/T50010-10)
        {
            T_FIMP_CON_GB1010 ConcD = FimpConcD.PROP.CON_GB1010;
            dEc = ConcD.dEc;
        }
        break;
    default:
        ASSERT(0);
        break;
    }
    return dEc;
}

double CBrdgSeisEval::Get_Es(const T_EVGP_K& EvgpK, const UINT& iColm, const EN_POSITION& enPos)
{
    double dEs = 0.0;
    T_EVGP_D EvgpD;
    if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD)) return dEs;
    T_MPCC_K MpccK = GetMpccKey(EvgpK, iColm, enPos);
    T_MPCC_D MpccD;
    if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) return dEs;

    T_FIMP_K FimpStlK = MpccD.MatlSteelK;
    T_FIMP_D FimpStlD;
    if(!m_pDoc->m_pAttrCtrl->GetFimp(FimpStlK, FimpStlD)) return dEs;
    if(FimpStlD.nMaterialType != 1)  return dEs;

    switch (FimpStlD.nHysModelType)
    {
    case 0: // 0:Menegotto
        dEs = FimpStlD.PROP.STL_MENEGO.dE;
        break;
    case 1: // 1:Bilinear
        dEs = FimpStlD.PROP.STL_BILINE.dE1;
        break;
    case 2: // 2:Unsymmetric Bilinear Steel Model
        dEs = FimpStlD.PROP.STL_GENBIL.dE[0];
        break;
    case 3: // 3:Trilinear Steel Model
        if( FimpStlD.PROP.STL_TRILIN.nMethod==0 )
        {// Sigma-Epsilon
            dEs = FimpStlD.PROP.STL_TRILIN.dSigmay[0] / FimpStlD.PROP.STL_TRILIN.dEpsilony[0];
        }
        else
        {// Sigma-Alpha
            dEs = FimpStlD.PROP.STL_TRILIN.dK[0];
        }
        break;
    case 4: // 4:Park Model
        dEs = FimpStlD.PROP.STL_PARK.dEs;
        break;
    case 5: // 5:JP Roadway Spec.
        dEs = FimpStlD.PROP.STL_ROADJP.dEs;
        break;
    case 6: // 6:China (GB50010-2010)
        dEs = FimpStlD.PROP.STL_GB2010.dEs;
        break;
    default:
        ASSERT(0);
        break;
    }

    return dEs;
}

BOOL CBrdgSeisEval::GetEvpm(const T_EVGP_K& EvgpK, const UINT& iCol, T_EVPM_D& rEvpmD)
{
    rEvpmD.Initialize();

    return m_pDoc->m_pAttrCtrl2->GetEvpm(EvgpK, iCol, rEvpmD);
}

T_ELEM_K CBrdgSeisEval::GetEvgpElemK(const T_EVGP_K& EvgpK, const UINT& iCol, const int& nEvgpPos)
{
    T_EVGP_D EvgpD;
    if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD)) { ASSERT(0); return 0; }

    return GetEvgpElemK(EvgpK, iCol, nEvgpPos, EvgpD);
}

T_ELEM_K CBrdgSeisEval::GetEvgpElemK(const T_EVGP_K& EvgpK, const UINT& iCol, const int& nEvgpPos, T_EVGP_D& EvgpD)
{
    // nEvgpPos ; 0:Top, 1:Bot
    T_ELEM_K ElemK=0;
    if(nEvgpPos == 0)
        ElemK = m_pDoc->m_pAttrCtrl2->GetTopElemK(EvgpD, iCol);
    else if(nEvgpPos == 1)
        ElemK = m_pDoc->m_pAttrCtrl2->GetBotElemK(EvgpD, iCol);
    else ASSERT(0);

    return ElemK;
}

void CBrdgSeisEval::Get_ArrangeEdgeDimsConc(const T_BREV_K& BrevK, const EN_LOCAL_DIR& enDir, double& dca1, double& dca1p, double& dca2, double& dca2p)
{
    T_ANEV_D AnevD;
    T_ANEV_K AnevK = GetAnevByBrev(BrevK, AnevD);

    Get_ArrangeEdgeDimsConc(AnevD, enDir, dca1, dca1p, dca2, dca2p);
}

void CBrdgSeisEval::Get_ArrangeEdgeDimsConc(const T_ANEV_D& AnevD, const EN_LOCAL_DIR& enDir, double& dca1, double& dca1p, double& dca2, double& dca2p)
{
    if ( IsTypicalBearingArrange(AnevD) )
    {
        T_BRPR_D BrprD;
        if ( !m_pDoc->m_pAttrCtrl2->GetBrpr(AnevD.BrprK, BrprD) ) BrprD.Initialize();

        if ( enDir==LONGI )
        {
            dca1  = AnevD.dD2   + BrprD.dS2*static_cast<double>(BrprD.nN2-1)/2.0;
            dca1p = AnevD.dD2_q + BrprD.dS2*static_cast<double>(BrprD.nN2-1)/2.0;
            dca2  = AnevD.dD1   - BrprD.dS1*static_cast<double>(BrprD.nN1-1)/2.0;
            dca2p = AnevD.dD1_q - BrprD.dS1*static_cast<double>(BrprD.nN1-1)/2.0;
        }
        else
        {
            dca1  = AnevD.dD1   + BrprD.dS1*static_cast<double>(BrprD.nN1-1)/2.0;
            dca1p = AnevD.dD1_q + BrprD.dS1*static_cast<double>(BrprD.nN1-1)/2.0;
            dca2  = AnevD.dD2   - BrprD.dS2*static_cast<double>(BrprD.nN2-1)/2.0;
            dca2p = AnevD.dD2_q - BrprD.dS2*static_cast<double>(BrprD.nN2-1)/2.0;
        }
    }
}


void CBrdgSeisEval::Get_ArrangeEdgeDimsPryo(const T_BREV_K& BrevK, const EN_LOCAL_DIR& enDir, double& dca1, double& dca1p, double& dca2, double& dca2p)
{
    T_ANEV_D AnevD;
    T_ANEV_K AnevK = GetAnevByBrev(BrevK, AnevD);

    Get_ArrangeEdgeDimsPryo(AnevD, enDir, dca1, dca1p, dca2, dca2p);
}

void CBrdgSeisEval::Get_ArrangeEdgeDimsPryo(const T_ANEV_D& AnevD, const EN_LOCAL_DIR& enDir, double& dca1, double& dca1p, double& dca2, double& dca2p)
{
    if ( IsTypicalBearingArrange(AnevD) )
    {
        T_BRPR_D BrprD;
        if ( !m_pDoc->m_pAttrCtrl2->GetBrpr(AnevD.BrprK, BrprD) ) BrprD.Initialize();

        if ( enDir==LONGI )
        {
            dca1  = AnevD.dD2   - BrprD.dS2*static_cast<double>(BrprD.nN2-1)/2.0;
            dca1p = AnevD.dD2_q - BrprD.dS2*static_cast<double>(BrprD.nN2-1)/2.0;
            dca2  = AnevD.dD1   - BrprD.dS1*static_cast<double>(BrprD.nN1-1)/2.0;
            dca2p = AnevD.dD1_q - BrprD.dS1*static_cast<double>(BrprD.nN1-1)/2.0;
        }
        else
        {
            dca1  = AnevD.dD1   - BrprD.dS1*static_cast<double>(BrprD.nN1-1)/2.0;
            dca1p = AnevD.dD1_q - BrprD.dS1*static_cast<double>(BrprD.nN1-1)/2.0;
            dca2  = AnevD.dD2   - BrprD.dS2*static_cast<double>(BrprD.nN2-1)/2.0;
            dca2p = AnevD.dD2_q - BrprD.dS2*static_cast<double>(BrprD.nN2-1)/2.0;
        }
    }
}


// 앵커배치에 따른 콘크리트 파괴면 투영면적 Avc for 콘크리트 파괴
double CBrdgSeisEval::Get_Avc(const double& dca1, const double& dca2, const double& dca2p, const double& ds1, const double& dsax, const double& nBC, const double& nA, const double& dha,
                              int& nAvcType, double& dLp)
{
    nAvcType = 0;
    dLp = 0.0;

    double d15ca1 = 1.5*dca1;
    BOOL bInter_s1  = fabs(ds1 )<2.0*d15ca1 ? TRUE : FALSE;
    BOOL bInter_sax = fabs(dsax)<2.0*d15ca1 ? TRUE : FALSE;

    // 투영길이 계산
    if(bInter_s1)
    {
        if(bInter_sax)
        {
            nAvcType = 1;
            dLp = min(dca2, d15ca1) + (nBC*(nA-1.0)*ds1 + (nBC-1.0)*dsax) + min(dca2p, d15ca1);
        }
        else
        {
            nAvcType = 2;
            double dLp1 = min(dca2, d15ca1) + (nA-1.0)*ds1 + d15ca1;
            double dLp2 = min(dca2p, d15ca1) + (nA-1.0)*ds1 + d15ca1;
            dLp = min(dLp1, dLp2);
            if(dLp2+cDGN_Zero < dLp1)
            {
                nAvcType = 4;
            }            
        }
    }
    else
    {
        nAvcType = 3;
        double dLp1 = min(dca2, d15ca1) + d15ca1;
        double dLp2 = min(dca2p, d15ca1) + d15ca1;
        dLp = min(dLp1, dLp2);
        if(dLp2+cDGN_Zero < dLp1)
        {
            nAvcType = 5;
        }
    }
    double dAvc = dha * dLp;

    return dAvc;
}

// 앵커배치에 따른 콘크리트 파괴면 투영면적 Anc for 콘크리트 프라이아웃
double CBrdgSeisEval::Get_Anc(const double& dca1, const double& dca1p, const double& dca2, double dca2p, const double& dhef,
                              const double& ds1, const double& ds2, const double& dSax, const double& dSay,
                              const double& nBC, int nBCT, const double& nA, const double& nB,
                              int& nANcType)
{   
    double dModca1p = dca1p;
    if(nBCT > 1 && static_cast<int>(nB)==1) // 가력방향 반대편 앵커열의 앵커가 1개인 경우 제외
    {
        nBCT -= 1;
        dModca1p += dSay;
    }

    double d15hef = 1.5*dhef;
    double dL1 = 0.0;
    double dL2 = 0.0;
    if(ds1 < 2.0*d15hef)
    {   
        if(dSax < 2.0*d15hef)
        {
            dL1 = min(dca2, d15hef) + (nBC*(nA-1)*ds1+(nBC-1)*dSax) + min(dca2p, d15hef);
            if((dSay+(nB-1)*ds2) < 2.0*d15hef)
            {
                nANcType = 1;
                dL2 = min(dca1, d15hef) + ((nB-1)*ds2+dSay)*(nBCT-1.0) + min(((nB-1)*ds2+dModca1p), d15hef);
            }
            else
            {
                nANcType = 2;
                dL2 = min(dca1, d15hef) + d15hef;
            }
        }
        else
        {
            dL1 = min(dca2, d15hef) + (nA-1)*ds1 + d15hef;
            if((dSay+(nB-1)*ds2) < 2.0*d15hef)
            {
                nANcType = 3;
                dL2 = min(dca1, d15hef) + ((nB-1)*ds2+dSay)*(nBCT-1.0) + min(((nB-1)*ds2+dModca1p), d15hef);
            }
            else
            {
                nANcType = 4;
                dL2 = min(dca1, d15hef) + d15hef;
            }
        }
    }
    else
    {
        dL1 = min(dca2, d15hef) + d15hef;
        if((dSay+(nB-1)*ds2) < 2.0*d15hef)
        {
            nANcType = 5;
            dL2 = min(dca1, d15hef) + ((nB-1)*ds2+dSay)*(nBCT-1.0) + min(((nB-1)*ds2+dModca1p), d15hef);
        }
        else
        {
            nANcType = 6;
            dL2 = min(dca1, d15hef) + d15hef;
        }
    }
    double dANc = dL1*dL2;

    return dANc;
}

int CBrdgSeisEval::Get_NumOfResistSupport(const int&iTypex, const int& iTypey, const int& nBCx, const int& nBCy)
{
    int nResisSup = 1;
    switch (iTypex)
    {
    case 1:
    case 2:
        break;
    case 3:
        nResisSup = nBCx;
        break;
    default:
        break;
    }

    switch (iTypey)
    {
    case 1:
    case 2:
        break;
    case 3:
        nResisSup *= nBCy;
        break;
    default:
        break;
    }

    return nResisSup;
}

BOOL CBrdgSeisEval::IsTypicalBearingArrange(const T_BREV_K& BrevK, double dRelAngle)
{
    T_ANEV_D AnevD;
    T_ANEV_K AnevK = GetAnevByBrev(BrevK, AnevD);
    if(AnevK==0) return FALSE;

    return IsTypicalBearingArrange(AnevD, dRelAngle);
}

BOOL CBrdgSeisEval::IsTypicalBearingArrange(const T_ANEV_D& AnevD, double dRelAngle)
{
	return FALSE;

//     // #1. Angle
//     double dTheta1 = AnevD.dTheta1; // Pier cap Left angle
//     double dTheta2 = AnevD.dTheta2; // Pier cap Right angle
//     double dTheta3 = AnevD.dTheta_q1; // Bearing arrangement angle
//     double dTheta4 = AnevD.dTheta_q2; // Bearing arrangement angle
// 
//     if(fabs(dTheta1) > cDgn_Zero) return FALSE;
//     if(fabs(dTheta2) > cDgn_Zero) return FALSE;
//     if(fabs(dTheta3) > cDgn_Zero) return FALSE;
//     if(fabs(dTheta4) > cDgn_Zero) return FALSE;
// 
//     // #2. support.
//     //if ( AnevD.bSuppType )
//     {
//         INT_PTR nLayer = AnevD.SuppType.aLayer.GetSize();
//         for(INT_PTR i=0; i<nLayer; ++i)
//         {
//             INT_PTR nSupp = AnevD.SuppType.aLayer[i].aBrevType.GetSize();
//             for(INT_PTR j=0; j<nSupp; ++j)
//             {
//                 if (AnevD.SuppType.aLayer[i].aBrevType[j] != 0) return FALSE; // not fixed.
//             }
//         }
//     }
// 
// 	// #2. Bearing Property
// 	T_BRPR_D BrprD; BrprD.Initialize();
// 	T_BRPR_K BrprK1 = CDgnSeisAnchorShapeMgr::GetBrprK(AnevD.BearingProp, 0, 0);
// 	if (!m_pDoc->m_pAttrCtrl2->GetBrpr(BrprK1, BrprD)) { ASSERT(0); return FALSE; }
// 	int nN1 = BrprD.nN1;
// 	int nN2 = BrprD.nN2;
// 
// 	for (int i = 0; i < AnevD.BearingProp.aBrprKs.GetSize(); i++)
// 	{
// 		for (int j = 0; j < AnevD.BearingProp.aBrprKs[i].aBrprK.GetSize(); j++)
// 		{
// 			T_BRPR_K BrprK = CDgnSeisAnchorShapeMgr::GetBrprK(AnevD.BearingProp, i, j);
// 			if(BrprK > 0)
// 			{
// 				if (!m_pDoc->m_pAttrCtrl2->GetBrpr(BrprK, BrprD)) BrprD.Initialize();
// 
// 				if (nN1 - BrprD.nN1 > cDgn_Zero) return FALSE;
// 				if (nN2 - BrprD.nN2 > cDgn_Zero) return FALSE;
// 
// 				if (BrprK1 != BrprK)return FALSE;
// 			}
// 			else
// 			{
// 				continue;
// 			}
// 		}
// 	}
// 
//     // #3. spaces
//     INT_PTR nsax = AnevD.aSax.GetSize();
//     if (nsax > 1)
//     {
//         double dsax = AnevD.aSax[0];
//         for (INT_PTR i=1; i<nsax; ++i)
//         {
//             if (fabs(dsax-AnevD.aSax[i]) > cDgn_Zero) return FALSE;
//         }
//     }
// 
//     INT_PTR nsay = AnevD.aSay.GetSize();
//     if (nsay > 1)
//     {
//         double dsay = AnevD.aSay[0];
//         for (INT_PTR i=1; i<nsay; ++i)
//         {
//             if (fabs(dsay-AnevD.aSay[i]) > cDgn_Zero) return FALSE;
//         }
//     }
// 
//     if ( !EQ0(dRelAngle) ) return FALSE;
//     
//     return TRUE;
}

T_ANEV_K CBrdgSeisEval::GetAnevByBrev(const T_BREV_K& BrevK, T_ANEV_D& rData, BOOL bAnchorDirPosi/*=TRUE*/)
{   
    T_ANEV_K AnevK=0;
    POSITION pos = m_pDoc->m_pAttrCtrl2->GetStartAnev();
    while(pos)
    {   
        m_pDoc->m_pAttrCtrl2->GetNextAnev(pos, AnevK, rData);

		//if (CTestEnvMgr::GetTestEnvST(_T("CivilSeismicTest")) == _T("yes"))
		{
			//////////////////////////////////////////////////////////////////////////
			// 비어있는 곳 채움.
			m_pDoc->m_pAttrCtrl2->GetModifiedAnevD(rData);
			//////////////////////////////////////////////////////////////////////////
		}

		if (bAnchorDirPosi)
		{// 원래 데이터 넘김
		}
		else
		{// 역방향
			//ConvertAnevToNegativeDir(rData);
		}

        if(rData.BrevK == BrevK) return AnevK;
    }
    
    AnevK=0;
    rData.Initialize();
    return AnevK;
}

void CBrdgSeisEval::ConvertAnevToNegativeDir(T_ANEV_D& rData)
{
	// 사용안함.
	ASSERT(0);

	T_ANEV_D rConvertData;
	rConvertData = rData;
	
	// Theta
	rConvertData.dTheta1 = rData.dTheta2;
	rConvertData.dTheta2 = rData.dTheta1;

	// dTheta_q1,q2 는 그대로

	//D1, D1_q
	rConvertData.dD1   = rData.dD1_q;
	rConvertData.dD1_q = rData.dD1;

	//D2, D2_q
	rConvertData.dD2   = rData.dD2_q;
	rConvertData.dD2_q = rData.dD2;

	int row = 0;
	int col = 0;
	int nM = -1;
	int nN = -1;

	//////////////////////////////////////////////////////////////////////////
	// 비어있는 곳 채움.
	//CDBDoc* pDoc = CDBDoc::GetDocPoint();
	//pDoc->m_pAttrCtrl2->GetModifiedAnevD(m_Data);
	//////////////////////////////////////////////////////////////////////////

	// Layer
	nM = -1;
	row = rData.aLayer.GetSize();
	for (int i = row - 1; i >= 0; i--) // 세로 크기 - 1부터 역순으로 반복
	{
		nM++;
		nN = -1;

		rConvertData.aLayer[nM].dD1   = rData.aLayer[i].dD1_q;
		rConvertData.aLayer[nM].dD1_q = rData.aLayer[i].dD1;
		rConvertData.aLayer[nM].dD2   = rData.aLayer[i].dD2_q;
		rConvertData.aLayer[nM].dD2_q = rData.aLayer[i].dD2;
		rConvertData.aLayer[nM].dSay  = (-1.0)*rData.aLayer[i].dSay;

		col = rData.aLayer[i].aSax.GetSize();
		for (int j = col - 1; j >= 0; j--)  // 가로 크기 - 1부터 역순으로 반복
		{
			nN++;
			rConvertData.aLayer[nM].aSax[nN] = rData.aLayer[i].aSax[j];
		}
	}

	// BearingProp
	nM = -1;
	row = rData.BearingProp.aBrprKs.GetSize();
	for (int i = row - 1; i >= 0; i--)    
	{
		nM++;
		nN = -1;
		
		col = rData.BearingProp.aBrprKs[i].aBrprK.GetSize();
		for (int j = col - 1; j >= 0; j--)    
		{
			nN++;
			rConvertData.BearingProp.aBrprKs[nM].aBrprK[nN] = rData.BearingProp.aBrprKs[i].aBrprK[j];
		}
	}

	// SuppType
	nM = -1;
	row = rData.SuppType.aLayer.GetSize();
	for (int i = row - 1; i >= 0; i--)
	{
		nM++;
		nN = -1;

		col = rData.SuppType.aLayer[i].aBrevType.GetSize();
		for (int j = col - 1; j >= 0; j--)
		{
			nN++;
			rConvertData.SuppType.aLayer[nM].aBrevType[nN] = rData.SuppType.aLayer[i].aBrevType[j];
		}
	}

	// TensChk
	nM = -1;
	row = rData.TensChk.aElnkNodeKs.GetSize();
	for (int i = row - 1; i >= 0; i--)
	{
		nM++;
		nN = -1;

		col = rData.TensChk.aElnkNodeKs[i].aNodeK1.GetSize();
		for (int j = col - 1; j >= 0; j--)
		{
			nN++;
			rConvertData.TensChk.aElnkNodeKs[nM].aNodeK1[nN] = rData.TensChk.aElnkNodeKs[i].aNodeK1[j];
			rConvertData.TensChk.aElnkNodeKs[nM].aNodeK2[nN] = rData.TensChk.aElnkNodeKs[i].aNodeK2[j];
		}
	}

	rData.Initialize();
	rData = rConvertData;
}

T_SUPL_K CBrdgSeisEval::GetSuplByBrev(const T_BREV_K& BrevK, T_SUPL_D& rData)
{
    rData.Initialize();

    T_ANEV_D AnevD;
    T_ANEV_K AnevK = GetAnevByBrev(BrevK, AnevD);
    if(AnevK==0) return 0;

    T_SUPL_K SuplK = GetSuplByAnev(AnevK, rData);
    return SuplK;
}

T_SUPL_K CBrdgSeisEval::GetSuplByAnev(const T_ANEV_K& AnevK, T_SUPL_D& rData)
{
    T_SUPL_K SuplK=0;
    POSITION pos = m_pDoc->m_pAttrCtrl2->GetStartSupl();
    while(pos)
    {   
        m_pDoc->m_pAttrCtrl2->GetNextSupl(pos, SuplK, rData);
        if(rData.AnevK == AnevK) return SuplK;
    }

    SuplK = 0;
    rData.Initialize();

    return FALSE;
}

bool CBrdgSeisEval::ExistRebar(const UINT& iGrup, const UINT& iColm, const EN_POSITION& enPos)
{
	T_EVGP_D EvgpD;
	if(!m_pDoc->m_pAttrCtrl2->GetEvgp(iGrup, EvgpD)) { ASSERT(0); return false; }
    const T_ELEM_K ElemK = GetEvgpElemK(iGrup, iColm, enPos, EvgpD);

    const T_SECT_K SectK = GetSectKey(ElemK);
	T_SECT_D SectD;
	if(!m_pDoc->m_pAttrCtrl->GetSectDesign(SectK, SectD)) { ASSERT(0); return false; }

    // Rebar Data
	if(SectD.SectBefore.Shape == D_SECT_SHAPE_REG_GEN &&
        (SectD.nStype==D_SECT_TYPE_USER || (SectD.nStype==D_SECT_TYPE_TAPERED && SectD.SectBefore.nStype==D_SECT_TYPE_USER)))
	{
		T_REBT_D GenRbarD;  GenRbarD.Initialize(); // 임의단면 기둥
        if(m_pDoc->m_pAttrCtrl->GetRebt(SectK, GenRbarD)) return true;
	}
	else
	{
		T_RCHK_D RchkData; RchkData.Initialize();
        if(m_pDoc->m_pAttrCtrl->GetRchk(SectK, RchkData)) return true;
	}
    return false;
}
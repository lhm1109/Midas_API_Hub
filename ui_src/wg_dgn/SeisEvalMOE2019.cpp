#include "stdafx.h"
#include "SeisEvalMOE2019.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\SeisEvalDataTool.h"

CSeisEvalMOE2019::CSeisEvalMOE2019(void) : CSeisEvalMOE2018()
{
}


CSeisEvalMOE2019::~CSeisEvalMOE2019(void)
{
}

double CSeisEvalMOE2019::GetAlphaConColm(const T_SECT_D& SectD)
{
    const CString& strShape = SectD.SectBefore.Shape;
    double dAlpha = 0.0;
    if (strShape == D_SECT_SHAPE_REG_SB)
    {
        double dH = SectD.SectBefore.SectI.Size[0];
        double dB = SectD.SectBefore.SectI.Size[1];
        dAlpha = fabs(dH-dB) < cEvalZero ? 1.75 : 1.5;
    }
    else if (strShape == D_SECT_SHAPE_REG_SR)
    {
        dAlpha = 1.75;		
    }
    return dAlpha;
}

double CSeisEvalMOE2019::GetPhiInfill()
{
    T_SEME_D SemeD;
    if (!m_pDoc->m_pAttrCtrl2->GetSeme(SemeD)) { SemeD.Initialize(); }
    return SemeD.data.eval.dPhi_v;
}

double CSeisEvalMOE2019::CalcVnConColm(const T_COLM_SHR_PARAM& Shr, double dPu, double dVu, double dMu)
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

double CSeisEvalMOE2019::CalcK1(const T_COLM_SHR_PARAM& ColmShr)
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

BOOL CSeisEvalMOE2019::IsConsiderVbjs(T_ELEM_K ElemK)
{
    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SEMM_D SemmD;
    if (!GetElemMatlSectMas(ElemK, SectD, MatdD, SemmD)) { ASSERT(FALSE); return FALSE; } 
    if (SemmD.bUseDefaultVal) { return FALSE; }

    if (SemmD.nMasonryCondition != D_SEMM_JOINT_GOOD) { return TRUE; }
    if (SemmD.nMasonryJoint     != D_SEMM_JOINT_GOOD) { return TRUE; }

    T_MSIS_D MsisD;
    if (!GetInfillParam(ElemK, MsisD)) { ASSERT(FALSE); return TRUE; }

    if (MsisD.dOpeningArea > 0.0) { return TRUE; }

    /// 면외강도 전도에 안전.
    if (!IsOkOutPlane(MsisD, SemmD)) { return TRUE; }    

    return FALSE;
}

BOOL CSeisEvalMOE2019::IsOkOutPlane(const T_MSIS_D& MsisD, const T_SEMM_D& SemmD)
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
    double dfmc = Tool.GetFmcMas(EN_EVALSTEP_PRE, EN_EVALMATL_NOM, SemmD);
    double dfmt = Tool.GetFmtMas(EN_EVALSTEP_PRE, EN_EVALMATL_NOM, SemmD);

    double dQtn = 0.0;    
    if (IsArchingAction(nMasonryJoint, dhmtm))
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

BOOL CSeisEvalMOE2019::IsNeedCheckOutPlane(int nMasonryJoint, double dhm, double dtm, int nPerformLevel, double dSDS, double dSD1)
{
    if (nMasonryJoint != D_SEMM_JOINT_GOOD) { return TRUE; }
    if (dhm > 3300.0) { return TRUE; }
    if (dtm >  190.0) { return TRUE; }

    double dhmtm = fabs(dtm) < cEvalZero ? 0.0 : dhm/dtm;
    if (!IsArchingAction(nMasonryJoint, dhmtm)) { return TRUE; }

    double dhmtmLimit1 = CalcHmTmLimitTable6_7(nPerformLevel, dSDS, 0.330, 0.50);
    double dhmtmLimit2 = CalcHmTmLimitTable6_7(nPerformLevel, dSD1, 0.133, 0.20);
    double dhmtmLimit  = min(dhmtmLimit1, dhmtmLimit2);

    if (dhmtm > dhmtmLimit || fabs(dhmtm - dhmtmLimit) > cEvalZero) { return TRUE; }

    return FALSE;
}

BOOL CSeisEvalMOE2019::IsArchingAction(int nMasonryJoint, double dhmtm)
{
    if (nMasonryJoint != D_SEMM_JOINT_GOOD) { return FALSE; }
    if (dhmtm > 25.0) { return FALSE; }    
    return TRUE;
}

double CSeisEvalMOE2019::CalcHmTmLimitTable6_7(int nPerformLevel, double dVal, double dMin, double dMax)
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

double CSeisEvalMOE2019::CalcQtnArchingAction(double dhm, double dtm, double dfmc)
{
    double dhmtm = fabs(dtm) < cEvalZero ? 0.0 : dhm/dtm;
    double dLambda2 = CalcLambda2(dhm, dtm);
    return fabs(dhmtm) < cEvalZero ? 0.0 : 0.7*dfmc*dLambda2/dhmtm;
}

double CSeisEvalMOE2019::CalcQtnCantilever(double dhm, double dtm, double dfmt)
{
    return dfmt*dtm*dtm/6.0*(dhm*dhm/2.0);
}

double CSeisEvalMOE2019::CalcFp(double nPerformLevel, double dtm, double dSDS, double dIe)
{
    double dParam = max(0.4*dSDS*dIe, 0.1);
    double dXi    = GetXi(nPerformLevel);
    double dWp    = m_pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_DENSITY, 20.0);
    return dParam*dXi*dWp;        
}

double CSeisEvalMOE2019::CalcLambda2(double dhm, double dtm)
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

double CSeisEvalMOE2019::GetXi(int nPerformLevel)
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


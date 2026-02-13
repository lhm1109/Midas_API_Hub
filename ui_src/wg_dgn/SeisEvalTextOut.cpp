#include "stdafx.h"
#include "SeisEvalTextOut.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_FileCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\SeisEvalStruct.h"
#include "..\wg_db\SeisEvalDataTool.h"
#include "..\wg_db\SeisEvalResult.h"

#include "..\dgnengine\idesign\DGN_lib\DGNCompare.h"

#include "..\MIT_Lib\rptostream.h"
#include "..\mit_lib\UrlReader.h"

using namespace std;

CSeisEvalTextOut::CSeisEvalTextOut(void) : CDgnTextOut()
{
    
}


CSeisEvalTextOut::~CSeisEvalTextOut(void)
{
}

bool CSeisEvalTextOut::InitPrintSetting(const CString& strIdentifier)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();        
    
    CString strPathName = pDoc->GetPathName();
    CFileCtrl FileCtrl(strPathName);
    CString strFileName = _T(""); 
    if (CDBLib::ActivateGenNXAnal())
        strFileName.Format(_T("%s_%s.xout"), FileCtrl.GetFilePathAndNameWithoutExtension(), strIdentifier);
    else
        strFileName.Format(_T("%s_%s.out"), FileCtrl.GetFilePathAndNameWithoutExtension(), strIdentifier);

    SetFileName(strFileName);
    rptwofstream fout(strFileName, ios::trunc);

    SetLineWidthForOutline1();
    SetUnitFactor();

    return true;
}

bool CSeisEvalTextOut::PrintSeisEval1stCon(const T_ELEM_K_LIST& aElemK, T_LCOM_K LcomK)
{
    CSeisEvalResult* pRsltPtr = GetResultPtr();
    if ( pRsltPtr == nullptr ) { ASSERT(0); return false; }
    
    PrintTitle(_T("Seismic Evaluation : 1st Step"), GetSeisEvalMethod());

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aBeamK;
    INT_PTR nBeam = Tool.GetEvalConBeamKeyList(aElemK, aBeamK);
    for ( INT_PTR i = 0 ; i < nBeam ; ++i )
    {
        const T_ELEM_K ElemK = aBeamK[i];

        T_EVAL1ST_BEAM Eval;
        if ( !pRsltPtr->GetResultSE1stConBeam(ElemK, Eval) ) continue;

        PrintConBeamInfo(ElemK, EN_EVALSTEP_1ST);

        if ( LcomK == 0 ) { LcomK = Eval.RepLcomK; }
        
        INT_PTR nLcom = Eval.aEvalLcom.GetSize();
        for ( INT_PTR  m = 0; m < nLcom ; ++m )
        {
            const T_EVAL1ST_BEAM_LCOM& EvalLcom = Eval.aEvalLcom[m];
            if ( EvalLcom.LcomK == LcomK )
            {
                PrintEval1stConBeamLcom(ElemK, EvalLcom);
                break;
            }            
        }
    }
    
    T_ELEM_K_LIST aColmK;
    INT_PTR nColm = Tool.GetEvalConColmAllKeyList(aElemK, aColmK);
    for ( INT_PTR i = 0 ; i < nColm ; ++i )
    {
        const T_ELEM_K ElemK = aColmK[i];

        T_EVAL1ST_COLM Eval;
        if ( !pRsltPtr->GetResultSE1stConColm(ElemK, Eval) ) continue;

        PrintConColmInfo(ElemK, EN_EVALSTEP_1ST);

        if ( LcomK == 0 ) { LcomK = Eval.RepLcomK; }
        
        INT_PTR nLcom = Eval.aEvalLcom.GetSize();
        for ( INT_PTR  m = 0; m < nLcom ; ++m )
        {
            const T_EVAL1ST_COLM_LCOM& EvalLcom = Eval.aEvalLcom[m];
            if ( EvalLcom.LcomK == LcomK )
            {
                PrintEval1stConColmLcom(ElemK, EvalLcom, Eval.Pmcvy, Eval.Pmcvz);
                break;
            }            
        }
    }

    T_ELEM_K_LIST aWallK;
    INT_PTR nWall = Tool.GetEvalConWallKeyList(aElemK, aWallK);
    for ( INT_PTR i = 0 ; i < nWall ; ++i )
    {
        const T_ELEM_K ElemK = aWallK[i];

        T_EVAL1ST_WALL Eval;
        if ( !pRsltPtr->GetResultSE1stConWall(ElemK, Eval) ) continue;

        PrintConWallInfo(ElemK, EN_EVALSTEP_1ST);

        if ( LcomK == 0 ) { LcomK = Eval.RepLcomK; }
        
        INT_PTR nLcom = Eval.aEvalLcom.GetSize();
        for ( INT_PTR m = 0 ; m < nLcom ; ++m )
        {
            const T_EVAL1ST_WALL_LCOM& EvalLcom = Eval.aEvalLcom[m];
            if ( EvalLcom.LcomK == LcomK )
            {
                PrintEval1stConWallLcom(ElemK, EvalLcom, Eval.PmcvyAvg, Eval.PmcvyNom);
                break;
            }
        }
    }

    return true;
}

bool CSeisEvalTextOut::PrintSeisEval1stStl(const T_ELEM_K_LIST& aElemK, T_LCOM_K LcomK)
{

    return true;
}

bool CSeisEvalTextOut::PrintSeisEval1stMas(const T_ELEM_K_LIST& aElemK, T_LCOM_K LcomK)
{

    return true;
}

bool CSeisEvalTextOut::PrintSeisEvalUphgCon(const T_ELEM_K_LIST& aElemK)
{
    CSeisEvalResult* pRsltPtr = GetResultPtr();
    if ( pRsltPtr == nullptr ) { ASSERT(0); return false; }

    PrintTitle(_T("Update Hinge Parameters"), GetSeisEvalMethod());

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aBeamK;
    INT_PTR nBeam = Tool.GetEvalConBeamKeyList(aElemK, aBeamK);
    for ( INT_PTR i = 0 ; i < nBeam ; ++i )
    {
        const T_ELEM_K ElemK = aBeamK[i];

        T_UPHG_CONBEAM Uphg;
        if ( !pRsltPtr->GetUphgConBeam(ElemK, Uphg) ) continue;

        PrintConBeamInfo(ElemK, EN_EVALSTEP_2ND);
        PrintUphgConBeam(ElemK, Uphg);        
    }

    T_ELEM_K_LIST aColmK;
    INT_PTR nColm = Tool.GetEvalConColmAllKeyList(aElemK, aColmK);
    for ( INT_PTR i = 0 ; i < nColm ; ++i )
    {
        const T_ELEM_K ElemK = aColmK[i];

        T_UPHG_CONCOLM Uphg;
        if ( !pRsltPtr->GetUphgConColm(ElemK, Uphg) ) continue;

        PrintConColmInfo(ElemK, EN_EVALSTEP_2ND);
        PrintUphgConColm(ElemK, Uphg);
    }

    T_ELEM_K_LIST aWallK;
    INT_PTR nWall = Tool.GetEvalConWallKeyList(aElemK, aWallK);
    for ( INT_PTR i = 0 ; i < nWall ; ++i )
    {
        const T_ELEM_K ElemK = aWallK[i];

        T_UPHG_CONWALL Uphg;
        if ( !pRsltPtr->GetUphgConWall(ElemK, Uphg) ) continue;

        PrintConWallInfo(ElemK, EN_EVALSTEP_2ND);
        PrintUphgConWall(ElemK, Uphg);
    }

    return true;
}

bool CSeisEvalTextOut::PrintSeisEvalUphgStl(const T_ELEM_K_LIST& aElemK)
{
    
    return true;
}

bool CSeisEvalTextOut::PrintSeisEvalUphgMas(const T_ELEM_K_LIST& aElemK)
{

    return true;
}

void CSeisEvalTextOut::PrintConBeamInfo(T_ELEM_K ElemK, EN_EVALSTEP enStep)
{
    WriteOutline1(_LS(IDS_DGN_SE_MEMBER_INFORMATION));

    WriteOutline2(_LS(IDS_DGN_SE_BASIC_INFORMATION));
    WriteOutline3Fmt(_T("%s : %d"), _LS(IDS_DGN_SE_ELEM), ElemK);

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    CSeisEvalDataTool Tool;
    if ( !Tool.GetElemMatlSectCon(ElemK, SectD, MatdD, SecmD) ) { ASSERT(0); }
    
    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_SECTION), SectD.SName);

    PrintConMatlInfo(enStep, MatdD.Name, SecmD);
    PrintConBeamReinf(ElemK);
    WriteBlank();
    
    WriteOutline2(_LS(IDS_DGN_SE_EVAL_INFORMATION));
    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_USE_SEIS_HOOK),  GetYesNo( Tool.IsUseSeisHook(ElemK) ));
    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_SEIS_COMPONENT), GetSeisCompType( Tool.GetSeisComponentType(ElemK) ));
    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_EFFDEPTH_SHEAR), GetShearEffDepth( Tool.GetDeffTypeConBeam(enStep) ));    
}

void CSeisEvalTextOut::PrintConBeamReinf(T_ELEM_K ElemK)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    T_REBB_D RebbD;
    if ( !pDoc->m_pAttrCtrl->GetRebbByElemK(ElemK, RebbD)) { return; }

    auto L_GetMbarInfo =[](const T_REBB_BASE& Base) -> CString
    {
        CString strMain = _T("-");
        switch ( Base.GetArrayType() )
        {
        case 1:
            {
                strMain.Format(_T("%d-%s"), Base.nRebar1, Base.RebarName);
            }            
            break;
        case 2:
            {
                if ( Base.RebarName == Base.RebarName2nd )
                {
                    strMain.Format(_T("%d-%s"), Base.nRebar1 + Base.nRebar2, Base.RebarName);
                }
                else
                {
                    strMain.Format(_T("%d-%s/%d-%s"), Base.nRebar1, Base.RebarName, Base.nRebar2, Base.RebarName2nd);
                }
            }
            break;
        default: ASSERT(0);
            break;
        }
        return strMain;
    };

    WriteOutline3(_LS(IDS_DGN_SE_LONG_REINFORCEMENT));
    WriteParaFmt(_T("I-END (TOP) : %s"), L_GetMbarInfo(RebbD.Top_I));
    WriteParaFmt(_T("      (BOT) : %s"), L_GetMbarInfo(RebbD.Bot_I));
    WriteParaFmt(_T("J-END (TOP) : %s"), L_GetMbarInfo(RebbD.Top_J));
    WriteParaFmt(_T("      (BOT) : %s"), L_GetMbarInfo(RebbD.Bot_J));

    auto L_GetSbarInfo =[this](const T_REBB_BASE& Base, const CString& strSBarName) -> CString
    {
        const int nSpace = GetReinfSpace2MM(Base.dSubRebarSpace);
        CString strSBar = _T("-");
        strSBar.Format(_T("%d-%s@%d"), Base.iSubRebarNum, strSBarName, nSpace);
        return strSBar;
    };

    WriteOutline3(_LS(IDS_DGN_SE_TRAN_REINFORCEMENT));
    WriteParaFmt(_T("I-END : %s"), L_GetSbarInfo(RebbD.Top_I, RebbD.SubRebarName));
    WriteParaFmt(_T("J-END : %s"), L_GetSbarInfo(RebbD.Top_J, RebbD.SubRebarName));   
}

void CSeisEvalTextOut::PrintConBeamForce(T_LCOM_K LcomK, const T_EVAL1ST_BEAM_POS& EvalPos)
{
    WriteOutline2(_LS(IDS_DGN_SE_DESIGN_FORCE));
    PrintLcomName1stEval(LcomK);
    WriteParaFmt(_T("V(UF) = %s"), GetCurVal(EvalPos.dVUF, D_UNITSYS_BASE_FORCE));
    WriteParaFmt(_T("M(UD) = %s"), GetCurVal(EvalPos.dMUD, D_UNITSYS_BASE_MOMENT));
}

void CSeisEvalTextOut::PrintConBeamStrengthFlex(const T_CONBEAM_FLEX& Flex)
{
    WriteParaFmt(_T("As  = %s"), GetCurVal(Flex.dAs, D_UNITSYS_BASE_AREA));
    WritePara   (_T("         As * fye"));
    WriteParaFmt(_T("a   = ------------------   = %s"), GetCurVal(Flex.da, D_UNITSYS_BASE_LENGTH));
    WritePara   (_T("       0.85 * fce * b"));
    WriteParaFmt(_T("d   = %s"), GetCurVal(Flex.dd, D_UNITSYS_BASE_LENGTH));
    WriteParaFmt(_T("Me  = As * fye * (d - a/2) = %s"), GetCurVal(Flex.dMe, D_UNITSYS_BASE_MOMENT));
}

void CSeisEvalTextOut::PrintConBeamStrengthShear(const T_CONBEAM_INFO& Info)
{
    WriteParaFmt(_T("d  = %s"), GetCurVal(Info.ddv, D_UNITSYS_BASE_LENGTH));
    WriteParaFmt(_T("Vc = 1/6 * SQRT[fck] * b * d = %s"), GetCurVal(Info.dVc, D_UNITSYS_BASE_FORCE));
    WriteParaFmt(_T("Vs = Asv * fys * d / s       = %s"), GetCurVal(Info.dVs, D_UNITSYS_BASE_FORCE));
    WriteParaFmt(_T("Vn = Vc + Vs                 = %s"), GetCurVal(Info.dVn, D_UNITSYS_BASE_FORCE));
}

void CSeisEvalTextOut::PrintConBeamCtrlAction(const T_CONBEAM_INFO& Info)
{
    WriteOutline3(_LS(IDS_DGN_SE_CHECK_CONTROLLED_ACTION));
    PrintConCtrlAction(Info.dVp, Info.dVn, Info.enCtrlType);
}

void CSeisEvalTextOut::PrintConBeamLongReinf(const T_CONBEAM_INFO& Info, double dMu)
{
    WriteOutline3(_LS(IDS_DGN_SE_CHECK_LONG_REINFORCEMENT));

    bool bPositiveMoment = dMu < 0.0 ? false : true;

    WriteParaFmt(_T("Rho  = As / (b * d ) = %.4f"), bPositiveMoment ? Info.FlexBot.dRho : Info.FlexTop.dRho);
    WriteParaFmt(_T("Rho' = As'/ (b * d ) = %.4f"), bPositiveMoment ? Info.FlexTop.dRho : Info.FlexBot.dRho);
    WriteParaFmt(_T("           0.85 * Beta1 * fck       600"));
    WriteParaFmt(_T("Rho,bal = -------------------- * ---------- = %.4f"), Info.dRhoBal);
    WritePara   (_T("                   fye           600 + fye "));
    WritePara   (_T(" Rho - Rho'"));
    WriteParaFmt(_T("------------ = %.4f"), Info.GetRhoIdx(bPositiveMoment));
    WritePara   (_T("  Rho,bal"));
}

void CSeisEvalTextOut::PrintConBeamTranReinf(const T_CONBEAM_INFO& Info, double dVu, bool bUseSeisHook, const CString& strComment)
{
    WriteOutline3(_LS(IDS_DGN_SE_CHECK_TRAN_REINFORCEMENT));
    WriteComment(strComment);

    auto L_GetInequalSign =[]( const double dValL, const double dValR ) -> CString
    {
        return dValL > dValR ? _T("> ") : _T("<=");
    };

    WriteParaFmt(_T("%s : %s"), _LS(IDS_DGN_SE_USE_SEIS_HOOK), bUseSeisHook ? _LS(IDS_DGN_SE_YES) : _LS(IDS_DGN_SE_NO));    
    WriteParaFmt(_T("Sv = %s %s d/3 = %s"), GetCurVal(Info.dSv, D_UNITSYS_BASE_LENGTH), 
        L_GetInequalSign(Info.dSv, Info.ddv/3.0), GetCurVal(Info.ddv/3.0, D_UNITSYS_BASE_LENGTH));
    WriteParaFmt(_T("Vs = %s %s (3/4)*V = %s"), GetCurVal(Info.dVs, D_UNITSYS_BASE_FORCE), 
        L_GetInequalSign(Info.dVs, 3.0/4.0*dVu), GetCurVal(3.0/4.0*dVu, D_UNITSYS_BASE_FORCE));
    WritePara(GetSeisConform(Info.bSeisConform));
}

void CSeisEvalTextOut::PrintConBeamShearIdx(const T_CONBEAM_INFO& Info, const CString& strComment)
{
    WriteOutline3(_LS(IDS_DGN_SE_CHECK_SHEAR_FORCE));
    WriteComment(strComment);
    WritePara   (_T("        V"));
    WriteParaFmt(_T("------------------ = %.4f"), Info.dShrIdx);
    WritePara   (_T(" b * d * SQRT[fck]"));
}

void CSeisEvalTextOut::PrintConBeamPerformLevel(const T_EVAL1ST_BEAM_POS& Pos)
{
    WriteOutline2(_LS(IDS_DGN_SE_EVALUATE_PERFORM_LEVEL));

    WriteBlank();
    PrintDCRFlex(Pos.DCRM);
    
    WriteBlank();
    PrintDCRShearForcAction(Pos.DCRV);
    
    WriteBlank();    
    PrintFinalPerformLevel(Pos.enLevel);
}

void CSeisEvalTextOut::PrintConBeamForce(const T_UPHG_CONBEAM_POS& Pos)
{
    WriteOutline2(_LS(IDS_DGN_SE_UPDATE_HINGE_CONDITION));
    WriteOutline3(_LS(IDS_DGN_SE_DESIGN_FORCE));
    WriteParaFmt(_T("%s : %s"), _LS(IDS_DGN_SE_PO_LOADCASE), GetPolcName(Pos.PolcK));
    WriteParaFmt(_T("%s : %d"), _LS(IDS_DGN_SE_PO_STEP), Pos.nStep+1);
    WriteParaFmt(_T("V = %s"), GetCurVal(Pos.dVu, D_UNITSYS_BASE_FORCE));
    WriteParaFmt(_T("M = %s"), GetCurVal(Pos.dMu, D_UNITSYS_BASE_MOMENT));

    WriteOutline3(_LS(IDS_DGN_SE_THETA_Y));
    WriteParaFmt(_T("Theta,y(+) = %12.4f"), Pos.ParamP.dThetay);
    WriteParaFmt(_T("Theta,y(-) = %12.4f"), Pos.ParamN.dThetay);     
}

void CSeisEvalTextOut::PrintConColmInfo(T_ELEM_K ElemK, EN_EVALSTEP enStep)
{
    WriteOutline1(_LS(IDS_DGN_SE_MEMBER_INFORMATION));

    WriteOutline2(_LS(IDS_DGN_SE_BASIC_INFORMATION));
    WriteOutline3Fmt(_T("%s : %d"), _LS(IDS_DGN_SE_ELEM), ElemK);

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    CSeisEvalDataTool Tool;
    if ( !Tool.GetElemMatlSectCon(ElemK, SectD, MatdD, SecmD) ) { ASSERT(0); }

    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_SECTION), SectD.SName);
        
    PrintConMatlInfo(enStep, MatdD.Name, SecmD);
    
    const bool bRectShape = ( SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SB ) ? true : false;
    PrintConColmReinf(ElemK, bRectShape);
    WriteBlank();

    WriteOutline2(_LS(IDS_DGN_SE_EVAL_INFORMATION));
    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_COLM_SEIS_DETAIL_TYPE), GetSeisDetailType( Tool.GetSeisDetailType(ElemK) ));
    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_SEIS_COMPONENT), GetSeisCompType( Tool.GetSeisComponentType(ElemK) ));
    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_EFFDEPTH_SHEAR), GetShearEffDepth( Tool.GetDeffTypeConColm(enStep) ));
    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_PM_CURVE_CALC_TYPE), GetPMCalcType( Tool.GetPMCalcMethodColm(ElemK, enStep) ));
    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_COLM_WITH_MASONRY_INFILLS), GetYesNo( Tool.IsConColmConnectedInfill(ElemK) ));
}

void CSeisEvalTextOut::PrintConColmReinf(T_ELEM_K ElemK, bool bRectShape)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    T_REBC_D RebcD;
    if ( !pDoc->m_pAttrCtrl->GetRebcByElemK(ElemK, RebcD)) { return; }

    CString strMbar = _T("");
    CString strSbar = _T("");
    const int nSpace = GetReinfSpace2MM(RebcD.dSubRebarSpace);

    if ( bRectShape )
    {
        if ( RebcD.bUseCornerRebar && !RebcD.bSameRebarEndNCenter)
        {
            strMbar.Format(_T("%d-%d-%s(%s)"), RebcD.nQrb, RebcD.nRow, RebcD.MainRebarName, RebcD.CornerRebarName);
        }
        else
        {
            strMbar.Format(_T("%d-%d-%s"), RebcD.nQrb, RebcD.nRow, RebcD.MainRebarName);
        }
        strSbar.Format(_T("%d|%d-%s@%d"), RebcD.iSubRebarNum[0], RebcD.iSubRebarNum[1], RebcD.SubRebarName, nSpace);
    }
    else
    {
        strMbar.Format(_T("%d-%s"), RebcD.nQrb, RebcD.MainRebarName);        
        strSbar.Format(_T("%d-%s@%d"), max(RebcD.iSubRebarNum[0], RebcD.iSubRebarNum[1]), RebcD.SubRebarName, nSpace);
    }

    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_LONG_REINFORCEMENT), strMbar);
    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_TRAN_REINFORCEMENT), strSbar);
}

void CSeisEvalTextOut::PrintConColmStrengthAxial(const T_EVAL1ST_COLM_POS& EvalPos)
{
    if ( EvalPos.bForcCtrlP )
    {
        WriteParaFmt(_T("Pn,C = %.2f * [0.85 * fck * (Ag - As) + fy * As] = %s"), 0.80, GetCurVal(EvalPos.dPnC, D_UNITSYS_BASE_FORCE));
        WriteParaFmt(_T("Pn,T = fy * As = %s"), GetCurVal(EvalPos.dPnT, D_UNITSYS_BASE_FORCE));  
    }
    else
    {
        WriteParaFmt(_T("Pe,C = %.2f * [0.85 * fce * (Ag - As) + fye * As] = %s"), 0.80, GetCurVal(EvalPos.dPeC, D_UNITSYS_BASE_FORCE));
        WriteParaFmt(_T("Pe,T = fye * As = %s"), GetCurVal(EvalPos.dPeT, D_UNITSYS_BASE_FORCE));
    }
}

void CSeisEvalTextOut::PrintConColmStrengthShear(int nShearMethod, const T_EVAL1ST_COLM_DIR& EvalDir, 
                                                 const CString& strSymDir, const CString& strComment)
{   
    switch ( nShearMethod )
    {
    case D_SEME_SHRSTR_MOE2018:
        {
            PrintConColmStrengthShearMethodEval(strSymDir, strComment);
        }
        break;
    case D_SEME_SHRSTR_STANDARD:
        {
            PrintConColmStrengthShearMethodKBC(EvalDir.Info.dPu, strSymDir, _T("KBC2016, EQ(0507.3.1), EQ(0507.3.2), EQ(0507.3.6"));
        }
        break;
    default: ASSERT(0);
        break;
    }

    WriteParaFmt(_T("Vn%s = Vc%s + Vs%s = %s"), strSymDir, strSymDir, strSymDir, GetCurVal(EvalDir.Info.dVn, D_UNITSYS_BASE_FORCE));
}

void CSeisEvalTextOut::PrintConColmStrengthShearMethodEval(const CString& strSymDir, const CString& strComment)
{
    WriteComment(strComment);
    WritePara   (_T("param1 = 0.5 * SQRT[fck] / [ M / (V*d) ]"));
    WritePara   (_T("param2 = 1 + Nu / [ 0.5 * SQRT[fck] * Ag ]"));
    WriteParaFmt(_T("Vnc%s = lambda * ( param1 * SQRT[ param2 ]) * 0.8 * Ag"), strSymDir);
    WriteParaFmt(_T("Vns%s = k1 * Av * fy * d / s"), strSymDir);       
}

void CSeisEvalTextOut::PrintConColmStrengthShearMethodKBC(double dPu, const CString& strSymDir, const CString& strComment)
{
    WriteComment(strComment);
    
    if ( fabs(dPu) < cEvalZero )
    {
        WriteParaFmt(_T("Vnc%s = 1/6 * lambda * SQRT[fck] * b * d"), strSymDir);
    }
    else if ( dPu > 0.0 )
    {
        WriteParaFmt(_T("Vnc%s = 1/6 * [ 1 + Nu / ( 14 * Ag ) ] * lambda * SQRT[fck] * b * d"), strSymDir);
    }
    else
    {
        WriteParaFmt(_T("Vnc%s = 1/6 * [ 1 + Nu / ( 3.5 * Ag ) ] * lambda * SQRT[fck] * b * d"), strSymDir);
    }

    WriteParaFmt(_T("Vns%s = k1 * Av * fy * d / s"), strSymDir);
}

void CSeisEvalTextOut::PrintConColmForce(T_LCOM_K LcomK, const T_EVAL1ST_COLM_POS& EvalPos)
{
    WriteOutline2(_LS(IDS_DGN_SE_DESIGN_FORCE));
    PrintLcomName1stEval(LcomK);
    WriteParaFmt(_T("P (UF) = %s"), GetCurVal(EvalPos.dPUF, D_UNITSYS_BASE_FORCE));
    WriteParaFmt(_T("Vy(UF) = %s"), GetCurVal(EvalPos.dVUFy, D_UNITSYS_BASE_FORCE));
    WriteParaFmt(_T("Vz(UF) = %s"), GetCurVal(EvalPos.dVUFz, D_UNITSYS_BASE_FORCE));
    WriteParaFmt(_T("My(UD) = %s"), GetCurVal(EvalPos.dMUDy, D_UNITSYS_BASE_MOMENT));
    WriteParaFmt(_T("Mz(UD) = %s"), GetCurVal(EvalPos.dMUDz, D_UNITSYS_BASE_MOMENT));
}

void CSeisEvalTextOut::PrintConColmCalcParam(const T_CONCOLM_INFO& Info)
{
    WriteOutline3(_LS(IDS_DGN_SE_CALC_PARAM));
    WriteParaFmt(_T("Ag = %s"), GetCurVal(Info.dAg, D_UNITSYS_BASE_AREA));
    WriteParaFmt(_T("b  = %s"), GetCurVal(Info.db, D_UNITSYS_BASE_LENGTH));
    WriteParaFmt(_T("d  = %s"), GetCurVal(Info.dd, D_UNITSYS_BASE_LENGTH));    
}

void CSeisEvalTextOut::PrintConColmTranReinfDetail(EN_EVALSTEP enStep, const T_CONCOLM_INFO& Info, const CString& strComment)
{
    WriteOutline3(_LS(IDS_DGN_SE_CHECK_TRAN_REINFORCEMENT_DETAIL));
    WriteComment(strComment);
    
    CSeisEvalDataTool Tool;
    const int nShearMethod = Tool.GetShearMethodConColm(enStep);

    WriteParaFmt(_T("%s : %s"), _LS(IDS_DGN_SE_COLM_SEIS_DETAIL_TYPE), GetSeisDetailType(Info.unSeisDetail));
    WriteParaFmt(_T("%s : %s"), _LS(IDS_DGN_SE_SHEAR_STRENGTH_TYPE), GetShearStrengthMethod(nShearMethod));
}

void CSeisEvalTextOut::PrintConColmFailureMode(const EN_DIR& enDir, const T_CONCOLM_INFO& Info, const CString& strComment)
{
    const CString& strGroup = [] (UINT unGroup) -> CString
    {
        switch ( unGroup )
        {
        case EN_CONCOLM_GROUP1: return _T("i");
        case EN_CONCOLM_GROUP2: return _T("ii");
        case EN_CONCOLM_GROUP3: return _T("iii");
        default: ASSERT(0); return _T("-");
        }
    }(Info.unGroup);

    const double dRatVpVo = dgn::SafeDiv(Info.dVp, Info.dVn);
    CString strCondition = _T("");
    if ( dgn::LE(dRatVpVo, 0.6) && Info.unSeisDetail == D_SEME_REINF_135 )
    {
        strCondition = Info.bConsiderGroup1 ? _T("[ As/(bw*s) >= 0.002 and s/d <= 0.5 ]")
            : _T("[ As/(bw*s) < 0.002 or s/d > 0.5 ]");
    }

	CString strSymDirM = _T("");
	CString strSymDirV = _T("");
	switch ( enDir )
	{
	case EN_DIR_Y: strSymDirM = _LSX(y); strSymDirV = _LSX(z); break;
	case EN_DIR_Z: strSymDirM = _LSX(z); strSymDirV = _LSX(y); break;
	default: ASSERT(0); break;
	}

    WriteOutline3(_LS(IDS_DGN_SE_CHECK_FAILURE_MODE));
    WriteComment(strComment);
    WriteParaFmt(_T("Me%si = %s"), strSymDirM, GetCurVal(Info.dMei, D_UNITSYS_BASE_MOMENT));
    WriteParaFmt(_T("Me%sj = %s"), strSymDirM, GetCurVal(Info.dMej, D_UNITSYS_BASE_MOMENT));
    WriteParaFmt(_T("L   = %s"), GetCurVal(Info.dL, D_UNITSYS_BASE_LENGTH));
    WriteParaFmt(_T("       Me%si + Me%sj"), strSymDirM, strSymDirM);
    WriteParaFmt(_T("Vp%s  = ------------- = %s"), strSymDirV, GetCurVal(Info.dVp, D_UNITSYS_BASE_FORCE));
    WritePara   (_T("            L"));
    WriteParaFmt(_T("Vo%s  = %s"), strSymDirV, GetCurVal(Info.dVn, D_UNITSYS_BASE_FORCE));
    WriteParaFmt(_T("Vp%s / Vo%s = %.3f -> Group %s %s"), strSymDirV, strSymDirV, dRatVpVo, strGroup, strCondition);
}

void CSeisEvalTextOut::PrintConColmAxialIdx(const T_CONCOLM_INFO& Info, const CString& strComment)
{
    WriteOutline3(_LS(IDS_DGN_SE_CHECK_AXIAL_FORCE));
    WriteComment(strComment);    
    WritePara   (_T("    P"));
    WriteParaFmt(_T("---------- = %.4f"), Info.dAxlIdx);
    WritePara   (_T(" Ag * fck"));
}

void CSeisEvalTextOut::PrintConColmTranReinfRatio(const T_CONCOLM_INFO& Info, const CString& strComment)
{
    WriteOutline3(_LS(IDS_DGN_SE_CHECK_TRAN_REINFORCEMENT_RATIO));
    WriteComment(strComment);    
    WritePara   (_T("        Av"));
    WriteParaFmt(_T("Rho = ------- = %.4f"), Info.dRhoIdx);
    WritePara   (_T("       b * s"));
}

void CSeisEvalTextOut::PrintConColmShearIdx(const T_CONCOLM_INFO& Info, const CString& strSymDir, const CString& strComment)
{
    WriteOutline3(_LS(IDS_DGN_SE_CHECK_SHEAR_FORCE));    
    WriteComment(strComment);
    WritePara   (_T("        V") + strSymDir);
    WriteParaFmt(_T("------------------ = %.4f"), Info.dShrIdx);
    WritePara   (_T(" b * d * SQRT[fck]"));
}

void CSeisEvalTextOut::PrintConColmPerformLevel(const T_EVAL1ST_COLM_POS& Pos)
{
    WriteOutline2(_LS(IDS_DGN_SE_EVALUATE_PERFORM_LEVEL));
    
    WriteBlank();
    if ( Pos.bForcCtrlP )
    {
        PrintDCRAxialForcAction(Pos.DCRP);
    }
    else
    {
        PrintDCRAxialDispAction(Pos.DCRP);
    }

    WriteBlank();
    PrintDCRFlexComb(Pos.DCRM);        
    
    WriteBlank();
    PrintDCRShearForcActionDir(Pos.DCRVy, EN_DIR_Y);
    WriteBlank();
    PrintDCRShearForcActionDir(Pos.DCRVz, EN_DIR_Z);
    
    WriteBlank();
    PrintFinalPerformLevel(Pos.enLevel);
}

void CSeisEvalTextOut::PrintConColmForce(const T_UPHG_CONCOLM_DIR& Dir)
{
    WriteOutline2(_LS(IDS_DGN_SE_UPDATE_HINGE_CONDITION));
    WriteOutline3(_LS(IDS_DGN_SE_DESIGN_FORCE));
    WriteParaFmt(_T("%s : %s"), _LS(IDS_DGN_SE_PO_LOADCASE), GetPolcName(Dir.PolcK));
    WriteParaFmt(_T("%s : %d"), _LS(IDS_DGN_SE_PO_STEP), Dir.nStep+1);
    WriteParaFmt(_T("P = %s"), GetCurVal(Dir.dPu, D_UNITSYS_BASE_FORCE));
    WriteParaFmt(_T("V = %s"), GetCurVal(Dir.dVu, D_UNITSYS_BASE_FORCE));
    WriteParaFmt(_T("M = %s"), GetCurVal(Dir.dMu, D_UNITSYS_BASE_MOMENT));

    WriteOutline3(_LS(IDS_DGN_SE_THETA_Y));
    WriteParaFmt(_T("Theta,y = %12.4f"), Dir.Param.dThetay);
}

#pragma region /// print concrete wall

void CSeisEvalTextOut::PrintConWallInfo(T_ELEM_K ElemK, EN_EVALSTEP enStep)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    T_WALL_KEY WallKey;
    T_WALL_D WallD;  WallD.Initialize();
    if ( !pDoc->m_pPostCtrl->GetWallDataFromElemK(ElemK, WallD, &WallKey) ) { ASSERT(0); }
    if ( !WallD.bLineWall ) { return; }

    WriteOutline1(_LS(IDS_DGN_SE_MEMBER_INFORMATION));

    CSeisEvalDataTool Tool;
    const T_KEY unWallID = Tool.GetWallID(ElemK);
    const CString& strStorName = Tool.GetStoryName(ElemK);

    T_THIK_D ThikD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;    
    if ( !Tool.GetElemMatlThik(ElemK, ThikD, MatdD, SecmD) ) { ASSERT(0); }
    
    WriteOutline2(_LS(IDS_DGN_SE_BASIC_INFORMATION));
    WriteOutline3Fmt(_T("%s : %d ( Wall ID = %d, Story = %s)"), _LS(IDS_DGN_SE_ELEM), ElemK, unWallID, strStorName);
    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_SECTION), ThikD.Value.strName);
    
    WriteParaFmt(_T("Tw = %s"), GetCurVal(WallD.dThickness, D_UNITSYS_BASE_LENGTH));
    WriteParaFmt(_T("Lw = %s"), GetCurVal(WallD.dLength, D_UNITSYS_BASE_LENGTH));
    WriteParaFmt(_T("Hw = %s"), GetCurVal(WallD.dHeight, D_UNITSYS_BASE_LENGTH));
        
    PrintConMatlInfo(enStep, MatdD.Name, SecmD);
    PrintConWallReinf(ElemK);
    WriteBlank();

    WriteOutline2(_LS(IDS_DGN_SE_EVAL_INFORMATION));
    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_BOUNDARY_ELEMENT),  GetYesNo( Tool.IsConsiderBoundaryElement(ElemK) ));
    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_SEIS_COMPONENT), GetSeisCompType( Tool.GetSeisComponentType(ElemK) ));
    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_EFFDEPTH_SHEAR), GetShearEffDepth( Tool.GetDeffTypeConWall(enStep) ));
    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_PM_CURVE_CALC_TYPE), GetPMCalcType( Tool.GetPMCalcMethodWall(ElemK, enStep) ));
}

void CSeisEvalTextOut::PrintConWallReinf(T_ELEM_K ElemK)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    
    T_WALL_KEY WallKey;
    T_WALL_D WallD;  WallD.Initialize();
    if ( !pDoc->m_pPostCtrl->GetWallDataFromElemK(ElemK, WallD, &WallKey) ) { ASSERT(0); return; }
    if ( !WallD.bLineWall ) { return; }

    T_REBW_K RebwK;
    RebwK.keymap = WallKey;
    T_REBW_D RebwD;
    if ( !pDoc->m_pAttrCtrl->GetRebw(RebwK, RebwD)) { ASSERT(0); }

    auto L_GetBarInfo =[this](const CString& strBarName, double dSpace) -> CString
    {
        CString strBar = _T("-");      
        const int nSpace = GetReinfSpace2MM(dSpace);
        strBar.Format(_T("%s@%d"), strBarName, nSpace);
        return strBar;
    };

    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_LONG_REINFORCEMENT), L_GetBarInfo(RebwD.VerticalRebarName, RebwD.dVerticalRebarSpace));    
    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_TRAN_REINFORCEMENT), L_GetBarInfo(RebwD.HorizonRebarName,  RebwD.dHorizonRebarSpace));
}

void CSeisEvalTextOut::PrintConWallStrengthShear(const T_EVAL1ST_WALL_POS& EvalPos)
{    
    WriteComment(_T("KBC 2016, (0507.10.1) ~ (0507.10.3)"));
    WritePara   (_T("Vcz1   = 0.28 * lambda * SQRT[fck] * h * d + Nu * d / ( 4 * lw )"));
    WritePara   (_T("Param1 = lw * [ 0.10 * lambda * SQRT[fck] + 0.2 * Nu / (lw * h) ]"));
    WritePara   (_T("Param2 = Mu / Vu - lw / 2"));
    WritePara   (_T("Vcz2   = [ 0.05 * lambda * SQRT[fck] + param1 / param2 ] * h * d"));
    WritePara   (_T("Vcz    = min [ Vcz1, Vcz2 ]"));
    if ( EvalPos.Info.bForceCtrl )
    {
        WritePara   (_T("Vsz    = Avh * fys * d / sh"));
        WriteParaFmt(_T("Vnz    = Vcz + Vsz = %s"), GetCurVal(EvalPos.Info.dVn, D_UNITSYS_BASE_FORCE));
    }
    else
    {
        WritePara   (_T("Vsz    = Avh * fyse * d / sh"));
        WriteParaFmt(_T("Vnz    = Vcz + Vsz = %s"), GetCurVal(EvalPos.Info.dVe, D_UNITSYS_BASE_FORCE));
    }
}

void CSeisEvalTextOut::PrintConWallForce(T_LCOM_K LcomK, const T_EVAL1ST_WALL_POS& EvalPos)
{
    WriteOutline2(_LS(IDS_DGN_SE_DESIGN_FORCE));
    PrintLcomName1stEval(LcomK);
    WriteParaFmt(_T("P(UF) = %s"), GetCurVal(EvalPos.dPUF, D_UNITSYS_BASE_FORCE));
    WriteParaFmt(_T("V(UF) = %s"), GetCurVal(EvalPos.dVUF, D_UNITSYS_BASE_FORCE));
    WriteParaFmt(_T("M(UF) = %s"), GetCurVal(EvalPos.dMUF, D_UNITSYS_BASE_MOMENT));
    WriteParaFmt(_T("V(UD) = %s"), GetCurVal(EvalPos.dVUD, D_UNITSYS_BASE_FORCE));
    WriteParaFmt(_T("M(UD) = %s"), GetCurVal(EvalPos.dMUD, D_UNITSYS_BASE_MOMENT));
}

void CSeisEvalTextOut::PrintConWallCtrlAction(const T_CONWALL_INFO& Info, const CString& strComment)
{
    WriteOutline3(_LS(IDS_DGN_SE_CHECK_CONTROLLED_ACTION));
    WriteComment(strComment);
    PrintConCtrlAction(Info.dVp, Info.dVe, Info.enCtrlType);
}

void CSeisEvalTextOut::PrintConWallAxialIdx(const T_CONWALL_INFO& Info, const CString& strComment)
{
    WriteOutline3(_LS(IDS_DGN_SE_CHECK_AXIAL_REINFORCEMENT));
    WriteComment(strComment);
    WritePara   (_T("(As-As')*fy + P   "));
    WriteParaFmt(_T("---------------- = %.4f"), Info.dAxlIdx);
    WritePara   (_T(" tw * lw * fck"));
}

void CSeisEvalTextOut::PrintConWallShearIdx(const T_CONWALL_INFO& Info, const CString& strComment)
{
    WriteOutline3(_LS(IDS_DGN_SE_CHECK_SHEAR_FORCE));
    WriteComment(strComment);
    WritePara   (_T("          V"));
    WriteParaFmt(_T("-------------------- = %.4f"), Info.dShrIdx);
    WritePara   (_T("SQRT[fck] * tw * lw"));
}

void CSeisEvalTextOut::PrintConWallPerformLevel(const T_EVAL1ST_WALL_POS& Pos)
{
    WriteOutline2(_LS(IDS_DGN_SE_EVALUATE_PERFORM_LEVEL));

    if ( Pos.Info.bForceCtrl )
    {
        WriteBlank();
        PrintDCRFlexForcActionDir(Pos.DCRM, EN_DIR_Y);

        WriteBlank();
        PrintDCRShearForcActionDir(Pos.DCRV, EN_DIR_Z);
    }
    else
    {
        WriteBlank();
        PrintDCRFlexDir(Pos.DCRM, EN_DIR_Y);

        WriteBlank();
        PrintDCRShearDispActionDir(Pos.DCRV, EN_DIR_Z);
    }

    WriteBlank();
    PrintFinalPerformLevel(Pos.enLevel);
}

void CSeisEvalTextOut::PrintConWallForce(const T_UPHG_CONWALL_POS& Pos)
{
    WriteOutline2(_LS(IDS_DGN_SE_UPDATE_HINGE_CONDITION));
    WriteOutline3(_LS(IDS_DGN_SE_DESIGN_FORCE));
    WriteParaFmt(_T("%s : %s"), _LS(IDS_DGN_SE_PO_LOADCASE), GetPolcName(Pos.PolcK));
    WriteParaFmt(_T("%s : %d"), _LS(IDS_DGN_SE_PO_STEP), Pos.nStep+1);
    WriteParaFmt(_T("P = %s"), GetCurVal(Pos.dPu, D_UNITSYS_BASE_FORCE));
    WriteParaFmt(_T("V = %s"), GetCurVal(Pos.dVu, D_UNITSYS_BASE_FORCE));
    WriteParaFmt(_T("M = %s"), GetCurVal(Pos.dMu, D_UNITSYS_BASE_MOMENT));

    WriteOutline3(_LS(IDS_DGN_SE_THETA_Y));
    WriteParaFmt(_T("Theta,y(V) = %12.4f"), Pos.ParamV.dThetay);
    WriteParaFmt(_T("Theta,y(M) = %12.4f"), Pos.ParamM.dThetay);
}

#pragma endregion

void CSeisEvalTextOut::PrintTitle(const CString& strRprtTitle, const CString& strSEMethod)
{
    const CString& strFullNa = D_MIDAS_FULL_NAME;    
    const CString& strProdNa = D_PRODUCT_NAME;
    const CString& strCpRgYr = D_PRODUCT_COPYRIGHT_YEAR;
    const CString& strCoFuNa = D_COMPANY_FULL_NAME;
    const CString& strCoAbNa = D_COMPANY_ABB_NAME;
    const CString& strUrlNa	 = D_MIDAS_URL;
    const CString& strVerStr	= D_PRODUCT_VERSION_STR;
    
    const CString& strHeadLine = _T("+============================================================+");
    WriteTextFmt(0, strHeadLine);
    if (!CProduct::IsRusLocal())
    {
      WriteTextFmt(0, _T("|  %-56s  |"), _T("MIDAS(") + strFullNa + _T(")"));
      WriteTextFmt(0, _T("|  %-56s  |"), _T("MIDAS(") + strProdNa + _T(" - Design & checking system for windows"));
      WriteTextFmt(0, strHeadLine);
      WriteTextFmt(0, _T("|  %-56s  |"), strRprtTitle);
      WriteTextFmt(0, _T("|  %-56s  |"), _T("Based On ") + strSEMethod);
      WriteTextFmt(0, _T("|  %56s  |"), _T("(c)") + strCpRgYr);
      WriteTextFmt(0, strHeadLine);
      WriteTextFmt(0, _T("|  %-40s%16s  |"), strCoFuNa, _T("(") + strCoAbNa + _T(")"));
      WriteTextFmt(0, _T("|  %-56s  |"), _T("HomePage : ") + strUrlNa);
      WriteTextFmt(0, strHeadLine);
      WriteTextFmt(0, _T("|  %-56s  |"), strProdNa + _T("  Version ") + strVerStr);
      WriteTextFmt(0, strHeadLine);
    }
    WriteBlank(2);   
}

void CSeisEvalTextOut::PrintLcomName1stEval(const T_LCOM_K LcomK)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    T_LCOM_D LcomD;
    if ( !pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, LcomK, LcomD) )
    {
        ASSERT(0);
        return;
    }

    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_LOAD_COMBINATION_FOR_EVALUATION), LcomD.LoadCombName);
}

void CSeisEvalTextOut::PrintFlexStrengthByPmcvAvg(const T_PMCV_TABLE& Pmcv, double dMe, const CString& strDir)
{
    PrintPmcvTable(Pmcv);
    WriteParaFmt(_T("Me%s = %s"), strDir, GetCurVal(dMe, D_UNITSYS_BASE_MOMENT));
}

void CSeisEvalTextOut::PrintFlexStrengthByPmcvNom(const T_PMCV_TABLE& Pmcv, double dMn, const CString& strDir)
{
    PrintPmcvTable(Pmcv);
    WriteParaFmt(_T("Mn%s = %s"), strDir, GetCurVal(dMn, D_UNITSYS_BASE_MOMENT));
}

void CSeisEvalTextOut::PrintMFact(const double adMFact[3])
{
    WriteParaFmt(_T("m(IO) = %6.3f"), adMFact[EN_LEVEL_IO]);
    WriteParaFmt(_T("m(LS) = %6.3f"), adMFact[EN_LEVEL_LS]);
    WriteParaFmt(_T("m(CP) = %6.3f"), adMFact[EN_LEVEL_CP]);
}

void CSeisEvalTextOut::PrintMFactCalc(const double adMFact[3], const CString& strComment)
{
    WriteOutline3(_LS(IDS_DGN_SE_CALC_MFACT_INTERPOLATION));
    WriteComment(strComment);

    PrintMFact(adMFact);
}

void CSeisEvalTextOut::PrintMFactUser(const double adMFact[3])
{
    WriteOutline2(_LS(IDS_DGN_SE_MFACT_DEFINDED_USER));
    WriteOutline3(_LS(IDS_DGN_SE_MFACT_DEFINDED_USER));

    PrintMFact(adMFact);
}

void CSeisEvalTextOut::PrintMFactDir(const double adMFact[3], EN_DIR enDir)
{
    const CString& strDir = GetLocalDirSymbol(enDir);
    WriteParaFmt(_T("m%s(IO) = %.3f"), strDir, adMFact[EN_LEVEL_IO]);
    WriteParaFmt(_T("m%s(LS) = %.3f"), strDir, adMFact[EN_LEVEL_LS]);
    WriteParaFmt(_T("m%s(CP) = %.3f"), strDir, adMFact[EN_LEVEL_CP]);
}

void CSeisEvalTextOut::PrintMFactCalcDir(const double adMFact[3], EN_DIR enDir, const CString& strComment)
{
    WriteOutline3Fmt(_T("%s ( %s )"), _LS(IDS_DGN_SE_CALC_MFACT_INTERPOLATION), GetLocalDir(enDir));
    WriteComment(strComment);

    PrintMFactDir(adMFact, enDir);
}

void CSeisEvalTextOut::PrintMFactUserDir(const double adMFact[3], EN_DIR enDir)
{
    WriteOutline2(_LS(IDS_DGN_SE_MFACT_DEFINDED_USER));
    WriteOutline3Fmt(_T("%s ( %s )"), _LS(IDS_DGN_SE_MFACT_DEFINDED_USER), GetLocalDir(enDir));

    PrintMFactDir(adMFact, enDir);
}

void CSeisEvalTextOut::PrintDCRAxialForcAction(const T_EVAL1ST_DCR& Dcr)
{
    WriteOutline3(_LS(IDS_DGN_SE_CALC_DCR_AXIAL));
    WriteComment(_LS(IDS_DGN_SE_FORCE_CTRL_ACTION));

    const double dDCR = Dcr.dDCR[EN_LEVEL_IO];
    WritePara   (_T("          PUF"));
    WriteParaFmt(_T("DCR,P =  ----- = %6.3f %s"), dDCR, GetStringDCR(dDCR) );
    WritePara   (_T("          Pn"));
    PrintPerformLevel(Dcr.enLevel);    
}

void CSeisEvalTextOut::PrintDCRAxialDispAction(const T_EVAL1ST_DCR& Dcr)
{
    WriteOutline3(_LS(IDS_DGN_SE_CALC_DCR_AXIAL));
    WriteComment(_LS(IDS_DGN_SE_DEFORMATION_CTRL_ACTION));

    CSeisEvalDataTool Tool;
    for ( int i = EN_LEVEL_IO ; i <= EN_LEVEL_CP ; ++i )
    {
        const double dDCR = Dcr.dDCR[i];
        const CString& strLevel = Tool.GetPerformLevel(static_cast<EN_LEVEL>(i));
        WritePara   (_T("                PUD"));
        WriteParaFmt(_T("DCR,P (%s) = ---------- = %6.3f %s"), strLevel, dDCR, GetStringDCR(dDCR));
        WriteParaFmt(_T("              m(%s)*Pe"), strLevel);        
    }

    PrintPerformLevel(Dcr.enLevel);
}

void CSeisEvalTextOut::PrintDCRFlex(const T_EVAL1ST_DCR& Dcr)
{
    WriteOutline3(_LS(IDS_DGN_SE_CALC_DCR_FLEXURE));
    WriteComment(_LS(IDS_DGN_SE_DEFORMATION_CTRL_ACTION));

    CSeisEvalDataTool Tool;
    for ( int i = EN_LEVEL_IO ; i <= EN_LEVEL_CP ; ++i )
    {
        const double dDCR = Dcr.dDCR[i];
        const CString& strLevel = Tool.GetPerformLevel(static_cast<EN_LEVEL>(i));
        WritePara   (_T("                MUD"));
        WriteParaFmt(_T("DCR,M (%s) = ---------- = %6.3f %s"), strLevel, dDCR, GetStringDCR(dDCR));
        WriteParaFmt(_T("              m(%s)*Me"), strLevel);        
    }

    PrintPerformLevel(Dcr.enLevel);
}

void CSeisEvalTextOut::PrintDCRFlexDir(const T_EVAL1ST_DCR& Dcr, EN_DIR enDir)
{
    WriteOutline3Fmt(_T("%s ( %s )"), _LS(IDS_DGN_SE_CALC_DCR_FLEXURE), GetLocalDir(enDir));
    WriteComment(_LS(IDS_DGN_SE_DEFORMATION_CTRL_ACTION));

    const CString& strDir = ( enDir == EN_DIR_Y ) ? _LSX(y) : _LSX(z);
        
    CSeisEvalDataTool Tool;
    for ( int i = EN_LEVEL_IO ; i <= EN_LEVEL_CP ; ++i )
    {
        const double dDCR = Dcr.dDCR[i];
        const CString& strLevel = Tool.GetPerformLevel(static_cast<EN_LEVEL>(i));
        WriteParaFmt(_T("                MUD%s"), strDir);
        WriteParaFmt(_T("DCR,M%s (%s) = ---------- = %6.3f %s"), strDir, strLevel, dDCR, GetStringDCR(dDCR));
        WriteParaFmt(_T("              m(%s)*Me%s"), strLevel, strDir);
    }

    PrintPerformLevel(Dcr.enLevel);
}

void CSeisEvalTextOut::PrintDCRFlexForcActionDir(const T_EVAL1ST_DCR& Dcr, EN_DIR enDir)
{
    WriteOutline3Fmt(_T("%s ( %s )"), _LS(IDS_DGN_SE_CALC_DCR_FLEXURE), GetLocalDir(enDir));
    WriteComment(_LS(IDS_DGN_SE_DEFORMATION_CTRL_ACTION));

    const CString& strDir = (enDir == EN_DIR_Y) ? _LSX(y) : _LSX(z);

    const double dDCR = Dcr.dDCR[EN_LEVEL_IO];
    WriteParaFmt(_T("          MUF%s"), strDir);
    WriteParaFmt(_T("DCR,V%s = ------ = %6.3f %s"), strDir, dDCR, GetStringDCR(dDCR));
    WriteParaFmt(_T("           Mn%s"), strDir);

    PrintPerformLevel(Dcr.enLevel);
}

void CSeisEvalTextOut::PrintDCRFlexComb(const T_EVAL1ST_DCR& Dcr)
{
    WriteOutline3(_LS(IDS_DGN_SE_CALC_DCR_FLEXURE));
    WriteComment(_LS(IDS_DGN_SE_DEFORMATION_CTRL_ACTION));

    CSeisEvalDataTool Tool;    
    for ( int i = EN_LEVEL_IO ; i <= EN_LEVEL_CP ; ++i )
    {
        const double dDCR = Dcr.dDCR[i];
        const CString& strLevel = Tool.GetPerformLevel(static_cast<EN_LEVEL>(i));
        WritePara   (_T("                    My                      Mz"));
        WriteParaFmt(_T("DCR,M (%s) = [ ------------ ]^alpha + [ ------------ ]^alpha = %.3f %s"), strLevel, dDCR, GetStringDCR(dDCR) );
        WriteParaFmt(_T("                my(%s)*Mey               mz(%s)*Mez"), strLevel, strLevel);
    }

    PrintPerformLevel(Dcr.enLevel);
}

void CSeisEvalTextOut::PrintDCRShearForcAction(const T_EVAL1ST_DCR& Dcr)
{
    WriteOutline3(_LS(IDS_DGN_SE_CALC_DCR_SHEAR));
    WriteComment(_LS(IDS_DGN_SE_FORCE_CTRL_ACTION));

    const double dDCR = Dcr.dDCR[EN_LEVEL_IO];    
    WritePara   (_T("         VUF"));
    WriteParaFmt(_T("DCR,V = ----- = %6.3f %s"), dDCR, GetStringDCR(dDCR));
    WritePara   (_T("         Vn"));        

    PrintPerformLevel(Dcr.enLevel);
}

void CSeisEvalTextOut::PrintDCRShearForcActionDir(const T_EVAL1ST_DCR& Dcr, EN_DIR enDir)
{
    WriteOutline3Fmt(_T("%s ( %s )"), _LS(IDS_DGN_SE_CALC_DCR_SHEAR), GetLocalDir(enDir));
    WriteComment(_LS(IDS_DGN_SE_FORCE_CTRL_ACTION));

    const CString& strDir = ( enDir == EN_DIR_Y ) ? _LSX(y) : _LSX(z);
    
    const double dDCR = Dcr.dDCR[EN_LEVEL_IO];
    WriteParaFmt(_T("          VUF%s"), strDir);
    WriteParaFmt(_T("DCR,V%s = ------ = %6.3f %s"), strDir, dDCR, GetStringDCR(dDCR));
    WriteParaFmt(_T("           Vn%s"), strDir);        
    
    PrintPerformLevel(Dcr.enLevel);
}

void CSeisEvalTextOut::PrintDCRShearDispActionDir(const T_EVAL1ST_DCR& Dcr, EN_DIR enDir)
{
    WriteOutline3Fmt(_T("%s ( %s )"), _LS(IDS_DGN_SE_CALC_DCR_SHEAR), GetLocalDir(enDir));
    WriteComment(_LS(IDS_DGN_SE_DEFORMATION_CTRL_ACTION));

    const CString& strDir = ( enDir == EN_DIR_Y ) ? _LSX(y) : _LSX(z);

    CSeisEvalDataTool Tool;
    for ( int i = EN_LEVEL_IO ; i <= EN_LEVEL_CP ; ++i )
    {
        const double dDCR = Dcr.dDCR[i];
        const CString& strLevel = Tool.GetPerformLevel(static_cast<EN_LEVEL>(i));
        WriteParaFmt(_T("                VUD%s"), strDir);
        WriteParaFmt(_T("DCR,V%s (%s) = ---------- = %6.3f %s"), strDir, strLevel, dDCR, GetStringDCR(dDCR));
        WriteParaFmt(_T("              m(%s)*Ve%s"), strLevel, strDir); 
    }
    
    PrintPerformLevel(Dcr.enLevel);
}

void CSeisEvalTextOut::PrintConMatlInfo(EN_EVALSTEP enStep, const CString& strMatlName, const T_SECM_D& SecmD)
{
    CSeisEvalDataTool Tool;

    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_MATERIAL), strMatlName);
    WriteParaFmt(_T("fck = %s"), GetCurVal(Tool.GetFc(enStep, EN_EVALMATL_NOM, SecmD), D_UNITSYS_BASE_STRESS));
    WriteParaFmt(_T("fce = %s"), GetCurVal(Tool.GetFc(enStep, EN_EVALMATL_AVG, SecmD), D_UNITSYS_BASE_STRESS));
    WriteParaFmt(_T("fye = %s"), GetCurVal(Tool.GetFym(enStep, EN_EVALMATL_AVG, SecmD), D_UNITSYS_BASE_STRESS));
    WriteParaFmt(_T("fys = %s"), GetCurVal(Tool.GetFys(enStep, EN_EVALMATL_NOM, SecmD), D_UNITSYS_BASE_STRESS));
}

void CSeisEvalTextOut::PrintConHingeParamFromAB(const double adTableP[6], const double adTableN[6])
{
    WriteOutline3(_LS(IDS_DGN_SE_CALC_MODELING_PARAM_LINEAR_INTERPOATION));

    WriteParaFmt(_T("a(%s)\t= %.4f,\t\ta(%s)\t= %.4f"), _LSX(+), adTableP[HG_A], _LSX(-), adTableN[HG_A]);
    WriteParaFmt(_T("b(%s)\t= %.4f,\t\tb(%s)\t= %.4f"), _LSX(+), adTableP[HG_B], _LSX(-), adTableN[HG_B]);
    WriteParaFmt(_T("c(%s)\t= %.4f,\t\tc(%s)\t= %.4f"), _LSX(+), adTableP[HG_C], _LSX(-), adTableN[HG_C]);

    WriteOutline3(_LS(IDS_DGN_SE_CALC_ACCEPTANCE_CRITERIA_LINEAR_INTERPOLATION));

    WriteParaFmt(_T("IO(%s)\t= %.4f,\t\tIO(%s)\t= %.4f"), _LSX(+), adTableP[HG_IO], _LSX(-), adTableN[HG_IO]);
    WriteParaFmt(_T("LS(%s)\t= %.4f,\t\tLS(%s)\t= %.4f"), _LSX(+), adTableP[HG_LS], _LSX(-), adTableN[HG_LS]);
    WriteParaFmt(_T("CP(%s)\t= %.4f,\t\tCP(%s)\t= %.4f"), _LSX(+), adTableP[HG_CP], _LSX(-), adTableN[HG_CP]);
}

void CSeisEvalTextOut::PrintConHingeParamFromDE(const double adTableP[6], const double adTableN[6])
{
    WriteOutline3(_LS(IDS_DGN_SE_CALC_MODELING_PARAM_LINEAR_INTERPOATION));

    WriteParaFmt(_T("d(%s)\t= %.4f,\t\td(%s)\t= %.4f"), _LSX(+), adTableP[HG_A], _LSX(-), adTableN[HG_A]);
    WriteParaFmt(_T("e(%s)\t= %.4f,\t\te(%s)\t= %.4f"), _LSX(+), adTableP[HG_B], _LSX(-), adTableN[HG_B]);
    WriteParaFmt(_T("c(%s)\t= %.4f,\t\tc(%s)\t= %.4f"), _LSX(+), adTableP[HG_C], _LSX(-), adTableN[HG_C]);

    WriteOutline3(_LS(IDS_DGN_SE_CALC_ACCEPTANCE_CRITERIA_LINEAR_INTERPOLATION));

    WriteParaFmt(_T("IO(%s)\t= %.4f,\t\tIO(%s)\t= %.4f"), _LSX(+), adTableP[HG_IO], _LSX(-), adTableN[HG_IO]);
    WriteParaFmt(_T("LS(%s)\t= %.4f,\t\tLS(%s)\t= %.4f"), _LSX(+), adTableP[HG_LS], _LSX(-), adTableN[HG_LS]);
    WriteParaFmt(_T("CP(%s)\t= %.4f,\t\tCP(%s)\t= %.4f"), _LSX(+), adTableP[HG_CP], _LSX(-), adTableN[HG_CP]);
}

void CSeisEvalTextOut::PrintConHingeParamFromAB(const double adTable[6])
{
    WriteOutline3(_LS(IDS_DGN_SE_CALC_MODELING_PARAM_LINEAR_INTERPOATION));

    WriteParaFmt(_T("a = %.4f"), adTable[HG_A]);
    WriteParaFmt(_T("b = %.4f"), adTable[HG_B]);
    WriteParaFmt(_T("c = %.4f"), adTable[HG_C]);

    WriteOutline3(_LS(IDS_DGN_SE_CALC_ACCEPTANCE_CRITERIA_LINEAR_INTERPOLATION));

    WriteParaFmt(_T("IO = %.4f"), adTable[HG_IO]);
    WriteParaFmt(_T("LS = %.4f"), adTable[HG_LS]);
    WriteParaFmt(_T("CP = %.4f"), adTable[HG_CP]);
}

void CSeisEvalTextOut::PrintConHingeParamFromDE(const double adTable[6])
{
    WriteOutline3(_LS(IDS_DGN_SE_CALC_MODELING_PARAM_LINEAR_INTERPOATION));

    WriteParaFmt(_T("d = %.4f"), adTable[HG_A]);
    WriteParaFmt(_T("e = %.4f"), adTable[HG_B]);
    WriteParaFmt(_T("c = %.4f"), adTable[HG_C]);

    WriteOutline3(_LS(IDS_DGN_SE_CALC_ACCEPTANCE_CRITERIA_LINEAR_INTERPOLATION));

    WriteParaFmt(_T("IO = %.4f"), adTable[HG_IO]);
    WriteParaFmt(_T("LS = %.4f"), adTable[HG_LS]);
    WriteParaFmt(_T("CP = %.4f"), adTable[HG_CP]);
}

void CSeisEvalTextOut::PrintConHingeCurveFromAB(const double adParamP[6], const double adParamN[6])
{
    WriteOutline2(_LS(IDS_DGN_SE_CALC_SKELETON_CURVE));

    auto L_WritePnt =[this](double dVal, const CString& strValName, const CString& strPnt, const CString& strSign)
    {
        WriteParaFmt(_T("                    %s(%s)"), strValName, strSign);
        WriteParaFmt(_T("D/DY,%s(%s) = 1 + ----------- = %.4f"), strPnt, strSign, dVal);
        WriteParaFmt(_T("                  Theta,y(%s)"), strSign);
    };
    
    WriteOutline3Fmt(_T("%s ( %s ) "), _T("Hinge curve points"), _LS(IDS_DGN_SE_POSITIVE));
    L_WritePnt(adParamP[HG_A], _LSX(a), _LSX(C), _LSX(+));
    L_WritePnt(adParamP[HG_A], _LSX(a), _LSX(D), _LSX(+));
    L_WritePnt(adParamP[HG_B], _LSX(b), _LSX(E), _LSX(+));

    WriteOutline3Fmt(_T("%s ( %s ) "), _T("Hinge curve points"), _LS(IDS_DGN_SE_NEGATIVE));
    L_WritePnt(adParamN[HG_B], _LSX(b), _LSX(E), _LSX(-));
    L_WritePnt(adParamN[HG_A], _LSX(a), _LSX(D), _LSX(-));
    L_WritePnt(adParamN[HG_A], _LSX(a), _LSX(C), _LSX(-));
}

void CSeisEvalTextOut::PrintConHingeCurveFromDE(const double adParamP[6], const double adParamN[6])
{
    WriteOutline2(_LS(IDS_DGN_SE_CALC_SKELETON_CURVE));

    auto L_WritePnt =[this](double dVal, const CString& strValName, const CString& strPnt, const CString& strSign)
    {
        WriteParaFmt(_T("                %s(%s)"), strValName, strSign);
        WriteParaFmt(_T("D/DY,%s(%s) = ----------- = %.4f"), strPnt, strSign, dVal);
        WriteParaFmt(_T("              Theta,y(%s)"), strSign);
    };

    WriteOutline3Fmt(_T("%s ( %s ) "), _T("Hinge curve points"), _LS(IDS_DGN_SE_POSITIVE));
    L_WritePnt(adParamP[HG_A], _LSX(d), _LSX(C), _LSX(+));
    L_WritePnt(adParamP[HG_A], _LSX(d), _LSX(D), _LSX(+));
    L_WritePnt(adParamP[HG_B], _LSX(e), _LSX(E), _LSX(+));

    WriteOutline3Fmt(_T("%s ( %s ) "), _T("Hinge curve points"), _LS(IDS_DGN_SE_NEGATIVE));
    L_WritePnt(adParamN[HG_B], _LSX(e), _LSX(E), _LSX(-));
    L_WritePnt(adParamN[HG_A], _LSX(d), _LSX(D), _LSX(-));
    L_WritePnt(adParamN[HG_A], _LSX(d), _LSX(C), _LSX(-));    
}

void CSeisEvalTextOut::PrintConHingeCurveFromAB(const double adParam[6], const CString& strComp)
{
    if ( !strComp.IsEmpty() )
    {
        WriteOutline2Fmt(_T("%s ( %s )"), _LS(IDS_DGN_SE_CALC_SKELETON_CURVE), strComp);
    }
    else
    {
        WriteOutline2(_LS(IDS_DGN_SE_CALC_SKELETON_CURVE));
    }    
    
    auto L_WritePnt =[this](double dVal, const CString& strValName, const CString& strPnt)
    {
        WriteParaFmt(_T("                 %s"), strValName);
        WriteParaFmt(_T("D/DY,%s = 1 + --------- = %.4f"), strPnt, dVal);
        WriteParaFmt(_T("              Theta,y"));
    };

    WriteOutline3(_T("Hinge curve points"));
    L_WritePnt(adParam[HG_A], _LSX(a), _LSX(C));
    L_WritePnt(adParam[HG_A], _LSX(a), _LSX(D));
    L_WritePnt(adParam[HG_B], _LSX(b), _LSX(E));
}

void CSeisEvalTextOut::PrintConHingeCurveFromDE(const double adParam[6], const CString& strComp)
{
    if ( !strComp.IsEmpty() )
    {
        WriteOutline2Fmt(_T("%s ( %s )"), _LS(IDS_DGN_SE_CALC_SKELETON_CURVE), strComp);
    }
    else
    {
        WriteOutline2(_LS(IDS_DGN_SE_CALC_SKELETON_CURVE));
    }

    auto L_WritePnt =[this] (double dVal, const CString& strValName, const CString& strPnt)
    {
        WriteParaFmt(_T("             %s"), strValName);
        WriteParaFmt(_T("D/DY,%s = --------- = %.4f"), strPnt, dVal);
        WriteParaFmt(_T("           Theta,y"));
    };

    WriteOutline3(_T("Hinge curve points"));
    L_WritePnt(adParam[HG_A], _LSX(d), _LSX(C));
    L_WritePnt(adParam[HG_A], _LSX(d), _LSX(D));
    L_WritePnt(adParam[HG_B], _LSX(e), _LSX(E));
}

void CSeisEvalTextOut::PrintConHingeAcceptCriteriaFromAB(const double adParam[6], const CString& strComp)
{
    if ( !strComp.IsEmpty() )
    {
        WriteOutline2Fmt(_T("%s ( %s )"), _LS(IDS_DGN_SE_CALC_ACCEPATANCE_CRITERIA), strComp);
    }
    else
    {
        WriteOutline2(_LS(IDS_DGN_SE_CALC_ACCEPATANCE_CRITERIA));
    }    
    
    auto L_WriteAC =[this](double dVal, const CString& strLevel)
    {
        WriteParaFmt(_T("                 %s"), strLevel);
        WriteParaFmt(_T("AC(%s) = 1 + ---------- = %.3f"), strLevel, dVal);
        WriteParaFmt(_T("              Theta,y"));
    };

    CSeisEvalDataTool Tool;
    const CString& strIO = Tool.GetPerformLevel(EN_LEVEL_IO);
    const CString& strLS = Tool.GetPerformLevel(EN_LEVEL_LS);
    const CString& strCP = Tool.GetPerformLevel(EN_LEVEL_CP);

    L_WriteAC(adParam[HG_IO], strIO);
    L_WriteAC(adParam[HG_LS], strLS);
    L_WriteAC(adParam[HG_CP], strCP);   
}

void CSeisEvalTextOut::PrintConHingeAcceptCriteriaFromDE(const double adParam[6], const CString& strComp)
{
    if ( !strComp.IsEmpty() )
    {
        WriteOutline2Fmt(_T("%s ( %s )"), _LS(IDS_DGN_SE_CALC_ACCEPATANCE_CRITERIA), strComp);
    }
    else
    {
        WriteOutline2(_LS(IDS_DGN_SE_CALC_ACCEPATANCE_CRITERIA));
    }

    auto L_WriteAC =[this] (double dVal, const CString& strLevel)
    {
        WriteParaFmt(_T("             %s"), strLevel);
        WriteParaFmt(_T("AC(%s) = ---------- = %.3f"), strLevel, dVal);
        WriteParaFmt(_T("          Theta,y"));
    };

    CSeisEvalDataTool Tool;
    const CString& strIO = Tool.GetPerformLevel(EN_LEVEL_IO);
    const CString& strLS = Tool.GetPerformLevel(EN_LEVEL_LS);
    const CString& strCP = Tool.GetPerformLevel(EN_LEVEL_CP);

    L_WriteAC(adParam[HG_IO], strIO);
    L_WriteAC(adParam[HG_LS], strLS);
    L_WriteAC(adParam[HG_CP], strCP);
}

void CSeisEvalTextOut::PrintConHingeAcceptCriteriaFromAB(const double adParamP[6], const double adParamN[6])
{
    WriteOutline2(_LS(IDS_DGN_SE_CALC_ACCEPATANCE_CRITERIA));

    auto L_WriteAC =[this](double dVal, const CString& strLevel, const CString& strSign)
    {
        WriteParaFmt(_T("                  %s(%s)"), strLevel, strSign);
        WriteParaFmt(_T("AC(%s)(%s) = 1 + ----------- = %.3f"), strLevel, strSign, dVal);
        WriteParaFmt(_T("                Theta,y(%s)"), strSign);
    };

    CSeisEvalDataTool Tool;
    const CString& strIO = Tool.GetPerformLevel(EN_LEVEL_IO);
    const CString& strLS = Tool.GetPerformLevel(EN_LEVEL_LS);
    const CString& strCP = Tool.GetPerformLevel(EN_LEVEL_CP);

    WriteOutline3(_LS(IDS_DGN_SE_POSITIVE));
    L_WriteAC(adParamP[HG_IO], strIO, _LSX(+));
    L_WriteAC(adParamP[HG_LS], strLS, _LSX(+));
    L_WriteAC(adParamP[HG_CP], strCP, _LSX(+));

    WriteOutline3(_LS(IDS_DGN_SE_NEGATIVE));        
    L_WriteAC(adParamN[HG_IO], strIO, _LSX(-));
    L_WriteAC(adParamN[HG_LS], strLS, _LSX(-));
    L_WriteAC(adParamN[HG_CP], strCP, _LSX(-));
}

void CSeisEvalTextOut::PrintConHingeAcceptCriteriaFromDE(const double adParamP[6], const double adParamN[6])
{
    WriteOutline2(_LS(IDS_DGN_SE_CALC_ACCEPATANCE_CRITERIA));

    auto L_WriteAC =[this](double dVal, const CString& strLevel, const CString& strSign)
    {
        WriteParaFmt(_T("              %s(%s)"), strLevel, strSign);
        WriteParaFmt(_T("AC(%s)(%s) = ----------- = %.3f"), strLevel, strSign, dVal);
        WriteParaFmt(_T("            Theta,y(%s)"), strSign);
    };

    CSeisEvalDataTool Tool;
    const CString& strIO = Tool.GetPerformLevel(EN_LEVEL_IO);
    const CString& strLS = Tool.GetPerformLevel(EN_LEVEL_LS);
    const CString& strCP = Tool.GetPerformLevel(EN_LEVEL_CP);

    WriteOutline3(_LS(IDS_DGN_SE_POSITIVE));
    L_WriteAC(adParamP[HG_IO], strIO, _LSX(+));
    L_WriteAC(adParamP[HG_LS], strLS, _LSX(+));
    L_WriteAC(adParamP[HG_CP], strCP, _LSX(+));

    WriteOutline3(_LS(IDS_DGN_SE_NEGATIVE));        
    L_WriteAC(adParamN[HG_IO], strIO, _LSX(-));
    L_WriteAC(adParamN[HG_LS], strLS, _LSX(-));
    L_WriteAC(adParamN[HG_CP], strCP, _LSX(-));
}

void CSeisEvalTextOut::PrintConCtrlAction(double dVp, double dVe, EN_EVALCTRL enCtrlType)
{
    WritePara   (_T("      Me,I + Me,J "));
    WriteParaFmt(_T("Vp = -------------- = %s"), GetCurVal(dVp, D_UNITSYS_BASE_FORCE));
    WritePara   (_T("           L"));
    WriteParaFmt(_T("Ve = %s"), GetCurVal(dVe, D_UNITSYS_BASE_FORCE));

    switch ( enCtrlType )
    {
    case EN_EVALCTRL_FLEX:
        {
            WriteParaFmt(_T("Vp < Ve -> %s"), _LS(IDS_DGN_SE_CONTROLLED_FLEXURE));
        }
        break;
    case EN_EVALCTRL_SHER:
        {
            WriteParaFmt(_T("Vp > Ve -> %s"), _LS(IDS_DGN_SE_CONTROLLED_SHEAR));
        }
        break;
    default: ASSERT(0);
        break;
    }
}

void CSeisEvalTextOut::PrintPerformLevel(EN_LEVEL enLevel)
{
    CSeisEvalDataTool Tool;
    WriteParaFmt(_T("-> %s : %s"), _LS(IDS_DGN_SE_PERFORM_LEVEL), Tool.GetPerformLevel(enLevel));
}

void CSeisEvalTextOut::PrintFinalPerformLevel(EN_LEVEL enLevel)
{
    CSeisEvalDataTool Tool;
    WriteOutline3Fmt(_T("%s : %s"), _LS(IDS_DGN_SE_FINAL_PERFORM_LEVEL), Tool.GetPerformLevel(enLevel));
}

CString CSeisEvalTextOut::GetSeisEvalMethod()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();

    CString strMethod = _T("");
    int nMethod = pDoc->m_pAttrCtrl2->GetSemeMethod();
    switch ( nMethod )
    {
    case D_SEME_METHOD_KISTEC2013:      strMethod = _LSX(KISTEC 2013);      break;
    case D_SEME_METHOD_KISTEC2019:      strMethod = _LSX(KISTEC 2019);      break;
    case D_SEME_METHOD_MOE2018:         strMethod = _LSX(MOE 2018);         break;
    case D_SEME_METHOD_MOE2019:         strMethod = _LSX(MOE 2019);         break;
	case D_SEME_METHOD_AIK_G_001_2021:  strMethod = _LSX(AIK-G-001-2021);   break;
	default: strMethod = _LSX(-); break;
    }
    return strMethod;
}

CString CSeisEvalTextOut::GetSeisConform(BOOL bSeisConform)
{
    return bSeisConform ? _LS(IDS_DGN_SE_CONSIDER_CONFORM) : _LS(IDS_DGN_SE_CONSIDER_NONCONFORM);
}

CString CSeisEvalTextOut::GetSeisDetailType(UINT unSeisDetType)
{
    switch ( unSeisDetType )
    {
    case D_SEME_REINF_135:   return _LS(IDS_DGN_SE_COLM_SEIS_DETAIL_135_HOOK);
    case D_SEME_REINF_90:    return _LS(IDS_DGN_SE_COLM_SEIS_DETAIL_90_HOOK);
    case D_SEME_REINF_OTHER: return _LS(IDS_DGN_SE_COLM_SEIS_DETAIL_OTHER);
    default: ASSERT(0); return _LSX(-);
    }
}

CString CSeisEvalTextOut::GetSeisCompType(D_SCMT_TYPE unScmtType)
{
    switch ( unScmtType )
    {
    case D_SCMT_TYPE_PRIMARY:   return _LS(IDS_DGN_SE_PRIMARY);
    case D_SCMT_TYPE_SECONDARY: return _LS(IDS_DGN_SE_SECONDARY);
    case D_SCMT_TYPE_NONE:      return _LS(IDS_DGN_NONE);
    default: ASSERT(0); return _LSX(-);
    }
}

CString CSeisEvalTextOut::GetShearStrengthMethod(int nShearType)
{
    switch ( nShearType )
    {
    case D_SEME_SHRSTR_MOE2018:  return GetSeisEvalMethod();
    case D_SEME_SHRSTR_STANDARD: return _LSX(KBC 2016);
    default: ASSERT(0); return _LSX(-);
    }
}

CString CSeisEvalTextOut::GetShearEffDepth(int nEffDepth)
{
    switch ( nEffDepth )
    {
    case D_SEME_EFF_DEPTH_08H:  return _ULS('0.8H');
    case D_SEME_EFF_DEPTH_AUTO: return _LS(IDS_DGN_SE_AUTO);
    default: ASSERT(0); return _LSX(-);
    }
}

CString CSeisEvalTextOut::GetPMCalcType(int nPMCalcType)
{
    switch ( nPMCalcType )
    {
    case D_SEME_PMCM_P: return _LS(IDS_DGN_SE_PM_CURVE_CALC_KEEP_P);
    case D_SEME_PMCM_E: return _LS(IDS_DGN_SE_PM_CURVE_CALC_KEEP_E);
    default: ASSERT(0); return _LSX(-);
    }    
}

CString CSeisEvalTextOut::GetLocalDir(EN_DIR enDir)
{
    switch ( enDir )
    {
    case EN_DIR_Y: return _LS(IDS_DGN_SE_LOCAL_Y);
    case EN_DIR_Z: return _LS(IDS_DGN_SE_LOCAL_Z);        
    default: ASSERT(0); return _LSX(-);
    }
}

CString CSeisEvalTextOut::GetLocalDirSymbol(EN_DIR enDir)
{
    switch ( enDir )
    {
    case EN_DIR_Y: return _LSX(y);
    case EN_DIR_Z: return _LSX(z);        
    default: ASSERT(0); return _LSX(-);
    }
}

CString CSeisEvalTextOut::GetPolcName(T_POLC_K PolcK)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    T_POLC_D PolcD;
    if ( !pDoc->m_pAttrCtrl->GetPolc(PolcK, PolcD)) { ASSERT(0); return _T("-"); }

    return PolcD.strName;
}

CString CSeisEvalTextOut::GetYesNo(bool bYes)
{
    return bYes ? _LS(IDS_DGN_SE_YES) : _LS(IDS_DGN_SE_NO);
}

int CSeisEvalTextOut::GetReinfSpace2MM(double dSpace)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    double dSpaceMM = pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, dSpace);
    return static_cast<int>(dSpaceMM);
}

CString CSeisEvalTextOut::GetStringDCR(double dDCR)
{
    return dDCR > 1.0 ? _T(" > 1.0") : _T(" <= 1.0");
}

void CSeisEvalTextOut::PrintPmcvTable(const T_PMCV_TABLE& Pmcv)
{
    const CString& strSeparator = _T("----------------------------------------------");
    WriteTextFmt(0, strSeparator);
    WriteTextFmt(0, _T("%4s %12s(%6s) %12s(%6s)"),
        _LSX(No), _LSX(Pn), GetUnitString(D_UNITSYS_BASE_FORCE), _LSX(Mn), GetUnitString(D_UNITSYS_BASE_MOMENT));
    WriteTextFmt(0, strSeparator);
    INT_PTR nPmcv = Pmcv.aPmcv.GetSize();
    for ( INT_PTR i = 0; i < nPmcv; ++i )
    {
        const T_PMCV_PNT& Cur = Pmcv.aPmcv[i];
        WriteTextFmt(0, _T("%4d         %s         %s"),
            i+1, GetCurVal(Cur.dPn, D_UNITSYS_BASE_FORCE, FALSE), GetCurVal(Cur.dMn, D_UNITSYS_BASE_MOMENT, FALSE));
    }
    WriteTextFmt(0, strSeparator);
}

CSeisEvalResult* CSeisEvalTextOut::GetResultPtr()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc == nullptr ) { ASSERT(0); return nullptr; }

    return pDoc->m_pPostCtrl->GetSeisEvalResult();
}
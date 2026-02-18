#include "stdafx.h"
#include "SeisEvalTextOutMOE2019.h"

#include "..\wg_db\SeisEvalStruct.h"
#include "..\wg_db\SeisEvalDataTool.h"

CSeisEvalTextOutMOE2019::CSeisEvalTextOutMOE2019(void)
{
}

CSeisEvalTextOutMOE2019::~CSeisEvalTextOutMOE2019(void)
{
}

void CSeisEvalTextOutMOE2019::PrintEval1stConBeamLcom(T_ELEM_K ElemK, const T_EVAL1ST_BEAM_LCOM& EvalLcom)
{    
    WriteOutline1(_LS(IDS_DGN_SE_EVALUATION_RESULT_I_END));
    PrintEval1stConBeamLcomPos(ElemK, EvalLcom.LcomK, EvalLcom.EndI);

    WriteOutline1(_LS(IDS_DGN_SE_EVALUATION_RESULT_J_END));
    PrintEval1stConBeamLcomPos(ElemK, EvalLcom.LcomK, EvalLcom.EndJ);
}

void CSeisEvalTextOutMOE2019::PrintEval1stConColmLcom(T_ELEM_K ElemK, const T_EVAL1ST_COLM_LCOM& EvalLcom,
                                                      const T_PMCV_TABLE& Pmcvy, const T_PMCV_TABLE& Pmcvz)
{
    WriteOutline1(_LS(IDS_DGN_SE_EVALUATION_RESULT_I_END));
    PrintEval1stConColmLcomPos(ElemK, EvalLcom.LcomK, EvalLcom.PosI, Pmcvy, Pmcvz);

    WriteOutline1(_LS(IDS_DGN_SE_EVALUATION_RESULT_J_END));
    PrintEval1stConColmLcomPos(ElemK, EvalLcom.LcomK, EvalLcom.PosJ, Pmcvy, Pmcvz);
}

void CSeisEvalTextOutMOE2019::PrintEval1stConWallLcom(T_ELEM_K ElemK, const T_EVAL1ST_WALL_LCOM& EvalLcom,
    const T_PMCV_TABLE& PmcvyAvg, const T_PMCV_TABLE& PmcvyNom)
{
    WriteOutline1(_LS(IDS_DGN_SE_EVALUATION_RESULT_TOP));
    const T_PMCV_TABLE& PmcvJ = EvalLcom.PosJ.Info.bForceCtrl ? PmcvyNom : PmcvyAvg;
    PrintEval1stConWallLcomPos(ElemK, EvalLcom.LcomK, EvalLcom.PosJ, PmcvJ);

    WriteOutline1(_LS(IDS_DGN_SE_EVALUATION_RESULT_BOT));
    const T_PMCV_TABLE& PmcvI = EvalLcom.PosI.Info.bForceCtrl ? PmcvyNom : PmcvyAvg;
    PrintEval1stConWallLcomPos(ElemK, EvalLcom.LcomK, EvalLcom.PosI, PmcvI);
}

void CSeisEvalTextOutMOE2019::PrintUphgConBeam(T_ELEM_K ElemK, const T_UPHG_CONBEAM& Uphg)
{
    WriteOutline1(_LS(IDS_DGN_SE_EVALUATION_RESULT_I_END));
    PrintUphgConBeamPos(ElemK, Uphg.EndI);

    WriteOutline1(_LS(IDS_DGN_SE_EVALUATION_RESULT_J_END));
    PrintUphgConBeamPos(ElemK, Uphg.EndJ);
}

void CSeisEvalTextOutMOE2019::PrintUphgConColm(T_ELEM_K ElemK, const T_UPHG_CONCOLM& Uphg)
{
    auto L_PrintUphgPos =[this, ElemK](const T_UPHG_CONCOLM_POS& Pos, const CString& strPos)
    {
        CSeisEvalDataTool Tool;
        if ( Tool.IsConColmConnectedInfill(ElemK)  )
        {
            WriteOutline1Fmt(_T("%s ( %s )"), strPos, _LS(IDS_DGN_SE_AXIAL));
            PrintUphgConColmAxial(ElemK, Pos.ParamComp, Pos.ParamTens);
        }

        WriteOutline1Fmt(_T("%s ( %s, %s )"), strPos, _LS(IDS_DGN_SE_FLEXURE), _LS(IDS_DGN_SE_LOCAL_Y));
        PrintUphgConColmDir(ElemK, EN_DIR_Y, Pos.Diry);

        WriteOutline1Fmt(_T("%s ( %s, %s )"), strPos, _LS(IDS_DGN_SE_FLEXURE), _LS(IDS_DGN_SE_LOCAL_Z));
        PrintUphgConColmDir(ElemK, EN_DIR_Z, Pos.Dirz);
    };

    L_PrintUphgPos(Uphg.EndI, _LS(IDS_DGN_SE_CALC_MODELING_PARAM_I_END));
    L_PrintUphgPos(Uphg.EndJ, _LS(IDS_DGN_SE_CALC_MODELING_PARAM_J_END));
}

void CSeisEvalTextOutMOE2019::PrintUphgConWall(T_ELEM_K ElemK, const T_UPHG_CONWALL& Uphg)
{
    auto L_PrintUphgPos = [this, ElemK](const T_UPHG_CONWALL_POS& Pos, const CString& strPos)
    {
        WriteOutline1Fmt(_T("%s ( %s )"), strPos, _LS(IDS_DGN_SE_FLEXURE));
        PrintUphgConWallPosFlex(ElemK, Pos);

        WriteOutline1Fmt(_T("%s ( %s )"), strPos, _LS(IDS_DGN_SE_SHEAR));
        PrintUphgConWallPosSher(ElemK, Pos);
    };

    L_PrintUphgPos(Uphg.EndJ, _LS(IDS_DGN_SE_CALC_MODELING_PARAM_TOP)); 
    L_PrintUphgPos(Uphg.EndI, _LS(IDS_DGN_SE_CALC_MODELING_PARAM_BOT));
}

void CSeisEvalTextOutMOE2019::PrintEval1stConBeamLcomPos(T_ELEM_K ElemK, T_LCOM_K LcomK, const T_EVAL1ST_BEAM_POS& EvalPos)
{
    PrintConBeamForce(LcomK, EvalPos);
    PrintConBeamStrength(EvalPos.Info);
    PrintConBeamMFact(ElemK, EvalPos);
    PrintConBeamPerformLevel(EvalPos);
}

void CSeisEvalTextOutMOE2019::PrintEval1stConColmLcomPos(T_ELEM_K ElemK, T_LCOM_K LcomK, const T_EVAL1ST_COLM_POS& EvalPos,
                                                         const T_PMCV_TABLE& Pmcvy, const T_PMCV_TABLE& Pmcvz)
{
    CSeisEvalDataTool Tool;
    const int nShrMethod = Tool.GetShearMethodConColm(EN_EVALSTEP_1ST);

    PrintConColmForce(LcomK, EvalPos);
    PrintConColmStrength(EvalPos, Pmcvy, Pmcvz, nShrMethod);
    PrintConColmMFactAxial(ElemK, EvalPos);
    PrintConColmMFact(ElemK, EN_DIR_Y, EvalPos.Diry);
    PrintConColmMFact(ElemK, EN_DIR_Z, EvalPos.Dirz);
    PrintConColmPerformLevel(EvalPos);
}

void CSeisEvalTextOutMOE2019::PrintEval1stConWallLcomPos(T_ELEM_K ElemK, T_LCOM_K LcomK, const T_EVAL1ST_WALL_POS& EvalPos,
                                                         const T_PMCV_TABLE& Pmcvy)
{
    PrintConWallForce(LcomK, EvalPos);
    PrintConWallStrength(EvalPos, Pmcvy);
    PrintConWallMFact(ElemK, EvalPos);
    PrintConWallPerformLevel(EvalPos);
}

void CSeisEvalTextOutMOE2019::PrintUphgConBeamPos(T_ELEM_K ElemK, const T_UPHG_CONBEAM_POS& UphgPos)
{    
    CSeisEvalDataTool Tool;
    bool bUseSeisHook = Tool.IsUseSeisHook(ElemK);
    const T_CONBEAM_INFO& Info = UphgPos.Info;
    const CString& strComment = _T("MOE 2019, Table 5-9");

    PrintConBeamForce(UphgPos);

    PrintConBeamStrength(Info);

    WriteOutline2(_LS(IDS_DGN_SE_CALC_MODELING_PARAM_ACCEPTANCE_CRITERIA));
    PrintConBeamCtrlAction(Info);
    PrintConBeamLongReinf(Info, UphgPos.dMu);
    PrintConBeamTranReinf(Info, UphgPos.dVu, bUseSeisHook, strComment);
    PrintConBeamShearIdx(Info, strComment);        
    PrintConHingeParamFromAB(UphgPos.ParamP.adTable, UphgPos.ParamN.adTable);    
    PrintConHingeCurveFromAB(UphgPos.ParamP.adParam, UphgPos.ParamN.adParam);
    PrintConHingeAcceptCriteriaFromAB(UphgPos.ParamP.adParam, UphgPos.ParamN.adParam);
}

void CSeisEvalTextOutMOE2019::PrintUphgConColmAxial(T_ELEM_K ElemK, const T_UPHG_PARAM& ParamComp, const T_UPHG_PARAM& ParamTens)
{
    const CString& strComment = _T("MOE 2019, Table 6-6");

    WriteOutline2(_LS(IDS_DGN_SE_UPDATE_HINGE_CONDITION));
    WriteOutline3(_LS(IDS_DGN_SE_THETA_Y));
    WriteParaFmt(_T("Theta,y(+) = %12.4f"), ParamTens.dThetay);
    WriteParaFmt(_T("Theta,y(-) = %12.4f"), ParamComp.dThetay);   

    WriteOutline2(_LS(IDS_DGN_SE_CALC_MODELING_PARAM_ACCEPTANCE_CRITERIA));
    WriteComment(strComment);
    PrintConHingeParamFromDE(ParamTens.adTable, ParamComp.adTable);
    PrintConHingeCurveFromDE(ParamTens.adParam, ParamComp.adParam);
    PrintConHingeAcceptCriteriaFromDE(ParamTens.adParam, ParamComp.adParam);
}

void CSeisEvalTextOutMOE2019::PrintUphgConColmDir(T_ELEM_K ElemK, EN_DIR enDir, const T_UPHG_CONCOLM_DIR& UphgDir)
{
    const T_CONCOLM_INFO& Info = UphgDir.Info;
    const CString& strComment = _T("MOE 2019, Table 5-10");
        
    PrintConColmForce(UphgDir);

	const CString& strSymDirV = (enDir == EN_DIR_Y) ? _LSX(z) : _LSX(y);

    WriteOutline2(_LS(IDS_DGN_SE_CALC_MODELING_PARAM_ACCEPTANCE_CRITERIA));
    PrintConColmCalcParam(Info);
    PrintConColmTranReinfDetail(EN_EVALSTEP_2ND, Info, strComment);
    PrintConColmFailureMode(enDir, Info, _T("MOE 2019, Table 5.4"));
    PrintConColmAxialIdx(Info, strComment);
    PrintConColmTranReinfRatio(Info, strComment);
    PrintConColmShearIdx(Info, strSymDirV, strComment);
    PrintConHingeParamFromAB(UphgDir.Param.adTable);
    PrintConHingeCurveFromAB(UphgDir.Param.adParam);
    PrintConHingeAcceptCriteriaFromAB(UphgDir.Param.adParam);    
}

void CSeisEvalTextOutMOE2019::PrintUphgConWallPosFlex(T_ELEM_K ElemK, const T_UPHG_CONWALL_POS& UphgPos)
{
    const T_CONWALL_INFO& Info = UphgPos.Info;
    const CString& strComment = _T("MOE 2019, Table 5-15, 5-16");

    PrintConWallForce(UphgPos);

    WriteOutline2(_LS(IDS_DGN_SE_CALC_MODELING_PARAM_ACCEPTANCE_CRITERIA));
    PrintConWallCtrlAction(Info, strComment);
    PrintConWallAxialIdx(Info, strComment);
    PrintConWallShearIdx(Info, strComment);   

    const T_UPHG_PARAM& Param = UphgPos.ParamM;
    switch ( Info.enCtrlType )
    {
    case EN_EVALCTRL_FLEX:
        {
            PrintConHingeParamFromAB(Param.adTable);
            PrintConHingeCurveFromAB(Param.adParam);
            PrintConHingeAcceptCriteriaFromAB(Param.adParam);
        }
        break;
    case EN_EVALCTRL_SHER:
        {
            PrintConHingeParamFromDE(Param.adTable);
            PrintConHingeCurveFromDE(Param.adParam);
            PrintConHingeAcceptCriteriaFromDE(Param.adParam);
        }
        break;
    default:
        ASSERT(0);
        break;
    }
}

void CSeisEvalTextOutMOE2019::PrintUphgConWallPosSher(T_ELEM_K ElemK, const T_UPHG_CONWALL_POS& UphgPos)
{
    const T_CONWALL_INFO& Info = UphgPos.Info;
    const CString& strComment = _T("MOE 2019, Table 5-15, 5-16");

    PrintConWallForce(UphgPos);

    WriteOutline2(_LS(IDS_DGN_SE_CALC_MODELING_PARAM_ACCEPTANCE_CRITERIA));
    PrintConWallCtrlAction(Info, strComment);
    PrintConWallAxialIdx(Info, strComment);
    PrintConWallShearIdx(Info, strComment);   

    PrintConHingeParamFromDE(UphgPos.ParamV.adTable);
    PrintConHingeCurveFromDE(UphgPos.ParamV.adParam);
    PrintConHingeAcceptCriteriaFromDE(UphgPos.ParamV.adParam);
}

void CSeisEvalTextOutMOE2019::PrintConBeamMFact(T_ELEM_K ElemK, const T_EVAL1ST_BEAM_POS& Pos)
{
    if ( Pos.MfacM.bUserMfac )
    {
        PrintMFactUser(Pos.MfacM.admFac);
    }
    else
    {
        WriteOutline2(_LS(IDS_DGN_SE_CALC_MFACT));

        CSeisEvalDataTool Tool;
        bool bUseSeisHook = Tool.IsUseSeisHook(ElemK);
        const T_CONBEAM_INFO& Info = Pos.Info;
        const CString& strComment = _T("MOE 2019, Table 5-6");

        PrintConBeamCtrlAction(Info);
        PrintConBeamLongReinf(Info, Pos.dMUD);
        PrintConBeamTranReinf(Info, Pos.dVUF, bUseSeisHook, strComment);
        PrintConBeamShearIdx(Info, strComment);
        PrintMFactCalc(Pos.MfacM.admFac, strComment);
    }    
}

void CSeisEvalTextOutMOE2019::PrintConColmMFactAxial(T_ELEM_K ElemK, const T_EVAL1ST_COLM_POS& Pos)
{
    if ( Pos.bForcCtrlP ) { return; }

    if ( Pos.MfacP.bUserMfac )
    {
        PrintMFactUser(Pos.MfacP.admFac);
    }
    else
    {
        WriteOutline2Fmt(_T("%s (%s)"), _LS(IDS_DGN_SE_CALC_MFACT), _LS(IDS_DGN_SE_AXIAL)); ///## 문자열 등록.
        const CString& strComment = _T("MOE 2019, Table 6-4");

        PrintMFactCalc(Pos.MfacP.admFac, strComment);
    }
}

void CSeisEvalTextOutMOE2019::PrintConColmMFact(T_ELEM_K ElemK, EN_DIR enDir, const T_EVAL1ST_COLM_DIR& EvalDir)
{
    if ( EvalDir.MfacM.bUserMfac )
    {
        PrintMFactUserDir(EvalDir.MfacM.admFac, enDir);
    }
    else
    {
		WriteOutline2Fmt(_T("%s (%s, %s)"), _LS(IDS_DGN_SE_CALC_MFACT), _LS(IDS_DGN_SE_FLEXURE), GetLocalDir(enDir));

        const T_CONCOLM_INFO& Info = EvalDir.Info;
        const CString& strComment = _T("MOE 2019, Table 5-7");
		const CString& strSymDirV = enDir == EN_DIR_Y ? _LSX(z) : _LSX(y); /// DirShear : DirFlexure swap.
		
        PrintConColmCalcParam(Info);
        PrintConColmTranReinfDetail(EN_EVALSTEP_1ST, Info, strComment);
        PrintConColmFailureMode(enDir, Info, _T("MOE 2019, Table 5.4"));
        PrintConColmAxialIdx(Info, strComment);
        PrintConColmTranReinfRatio(Info, strComment);
        PrintConColmShearIdx(Info, strSymDirV, strComment);
        PrintMFactCalcDir(EvalDir.MfacM.admFac, enDir, strComment);
    }
}

void CSeisEvalTextOutMOE2019::PrintConWallMFact(T_ELEM_K ElemK, const T_EVAL1ST_WALL_POS& Pos)
{
    const CString& strComment = _T("MOE 2019, Table 5-13, 5-14");
    if ( Pos.Info.bForceCtrl )
    {
        PrintConWallForcAction(Pos.Info, strComment);
    }
    else
    {
        if ( Pos.MfacMV.bUserMfac )
        {
            PrintMFactUser(Pos.MfacMV.admFac);
        }
        else
        {
            WriteOutline2(_LS(IDS_DGN_SE_CALC_MFACT));

            const T_CONWALL_INFO& Info = Pos.Info;

            PrintConWallCtrlAction(Info, strComment);
            PrintConWallAxialIdx(Info, strComment);
            PrintConWallShearIdx(Info, strComment);
            PrintMFactCalc(Pos.MfacMV.admFac, strComment);
        }
    }
}

void CSeisEvalTextOutMOE2019::PrintConBeamStrength(const T_CONBEAM_INFO& Info)
{
    WriteOutline2(_LS(IDS_DGN_SE_CALC_STRENGTH));

    WriteOutline3Fmt(_T("%s (%s)"), _LS(IDS_DGN_SE_EXPECTED_FLEX_STERNGTH), _LS(IDS_DGN_SE_POSITIVE));
    WriteComment(_T("MOE 2019, 5.4.3.1(1), EQ (5-1)"));
    PrintConBeamStrengthFlex(Info.FlexBot);
    WriteBlank();

    WriteOutline3Fmt(_T("%s (%s)"), _LS(IDS_DGN_SE_EXPECTED_FLEX_STERNGTH), _LS(IDS_DGN_SE_NEGATIVE));
    WriteComment(_T("MOE 2019, 5.4.3.1(1), EQ (5-1)"));
    PrintConBeamStrengthFlex(Info.FlexTop);
    WriteBlank();

    WriteOutline3(_LS(IDS_DGN_SE_SHEAR_STRENGTH));
    WriteComment(_T("MOE 2019, 5.4.3.1(2), EQ (5-2)~(5-4)"));
    PrintConBeamStrengthShear(Info);
    WriteBlank();
}

void CSeisEvalTextOutMOE2019::PrintConColmStrength(const T_EVAL1ST_COLM_POS& Pos, const T_PMCV_TABLE& Pmcvy, 
                                                   const T_PMCV_TABLE& Pmcvz, int nShrMethod)
{
    WriteOutline2(_LS(IDS_DGN_SE_CALC_STRENGTH));

    WriteOutline3(_LS(IDS_DGN_SE_AXIAL_STRENGTH));    
    PrintConColmStrengthAxial(Pos);
    WriteBlank();

    WriteOutline3Fmt(_T("%s (%s)"), _LS(IDS_DGN_SE_EXPECTED_FLEX_STERNGTH), GetLocalDir(EN_DIR_Y));
    PrintFlexStrengthByPmcvAvg(Pmcvy, Pos.dMey, _LSX(y));
    WriteBlank();

    WriteOutline3Fmt(_T("%s (%s)"), _LS(IDS_DGN_SE_EXPECTED_FLEX_STERNGTH), GetLocalDir(EN_DIR_Z));
    PrintFlexStrengthByPmcvAvg(Pmcvz, Pos.dMez, _LSX(z));
    WriteBlank();

    /// dir 구조체는 모멘트 방향에 대응하도록 저장되어 있음.
    WriteOutline3Fmt(_T("%s (%s)"), _LS(IDS_DGN_SE_SHEAR_STRENGTH), GetLocalDir(EN_DIR_Y));
    PrintConColmStrengthShear(nShrMethod, Pos.Dirz, _LSX(y), _T("MOE 2019, (5-5)"));
    WriteBlank();

    WriteOutline3Fmt(_T("%s (%s)"), _LS(IDS_DGN_SE_SHEAR_STRENGTH), GetLocalDir(EN_DIR_Z));
    PrintConColmStrengthShear(nShrMethod, Pos.Diry, _LSX(z), _T("MOE 2019, (5-5)"));
    WriteBlank();
}


void CSeisEvalTextOutMOE2019::PrintConWallStrength(const T_EVAL1ST_WALL_POS& Pos, const T_PMCV_TABLE& Pmcvy)
{
    WriteOutline2(_LS(IDS_DGN_SE_CALC_STRENGTH));

    if ( Pos.Info.bForceCtrl )
    {
        WriteOutline3Fmt(_T("%s (%s)"), _LS(IDS_DGN_SE_NOMINAL_FLEX_STERNGTH), GetLocalDir(EN_DIR_Y));
        PrintFlexStrengthByPmcvNom(Pmcvy, Pos.Info.dMn, _LSX(y));
        WriteBlank();
    }
    else
    {
        WriteOutline3Fmt(_T("%s (%s)"), _LS(IDS_DGN_SE_EXPECTED_FLEX_STERNGTH), GetLocalDir(EN_DIR_Y));
        PrintFlexStrengthByPmcvAvg(Pmcvy, Pos.Info.dMe, _LSX(y));
        WriteBlank();
    }

    WriteOutline3Fmt(_T("%s (%s)"), _LS(IDS_DGN_SE_SHEAR_STRENGTH), GetLocalDir(EN_DIR_Z));
    PrintConWallStrengthShear(Pos);
    WriteBlank();
}

void CSeisEvalTextOutMOE2019::PrintConWallForcAction(const T_CONWALL_INFO& Info, const CString& strComment)
{
    const CString& strCommentAction = _T("MOE 2019, Table 5-13, 5-14");

    WriteOutline2Fmt(_T("%s"), _LS(IDS_DGN_SE_CHECK_CTRL_ACTION));

    PrintConWallCtrlAction(Info, strCommentAction);
    PrintConWallAxialIdx(Info, strComment);

    WriteOutline3(_LS(IDS_DGN_SE_FORCE_CTRL_ACTION));

    WriteBlank();
}
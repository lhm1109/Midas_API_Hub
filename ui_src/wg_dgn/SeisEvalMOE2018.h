#pragma once

#include "seiseval.h"

class CSeisEvalMOE2018 : public CSeisEval
{
public:
    CSeisEvalMOE2018(void);
    virtual ~CSeisEvalMOE2018(void);

    virtual BOOL InitializeHingeInfill(T_ELEM_K ElemK, const BOOL abDof[6], OUT T_PHGT_D& rPhgtD);
    virtual BOOL CalcUpdateHingeInfill(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_MAP& mPORslt, OUT T_PHGT_D& rPhgtD);

protected:    
	virtual BOOL CalcMyHingePropConBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, double dRatioMuMy, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ);
    virtual double CalcBeta1(double dfck);

    virtual BOOL CalcFxHingePropConColm(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhghJ) override;
    virtual BOOL CalcFyzHingePropConColm(T_ELEM_K ElemK, EN_DIR unDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ);
    virtual BOOL CalcMyzHingePropConColm(T_ELEM_K ElemK, EN_DIR unDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override;

    virtual BOOL CalcFxHingePropConWall(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override;
    virtual BOOL CalcFyHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ);
    virtual BOOL CalcFzHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override;
    virtual BOOL CalcMyHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override;
    virtual BOOL CalcMzHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ);

    virtual BOOL CalcMyHingePropStlBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ);

    virtual BOOL CalcFxHingePropStlColm(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ);	
    virtual BOOL CalcMyzHingePropStlColm(T_ELEM_K ElemK, UINT unDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ);

    virtual BOOL CalcFxHingePropStlBrac(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ);

    virtual BOOL CalcMyHingePropSrcBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ);

    virtual BOOL CalcFxHingePropSrcColm(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ);	
    virtual BOOL CalcMyzHingePropSrcColm(T_ELEM_K ElemK, UINT unDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ);

    virtual BOOL CalcFxHingePropInfill(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_MAP& mPORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ);

    virtual BOOL CalcSeisEvalLinRsltConBeam(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_BEAM& raEvalLin); 
    virtual BOOL CalcSeisEvalLinRsltConColm(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_COLM& raEvalLin);
    virtual BOOL CalcSeisEvalLinRsltConWall(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_WALL& raEvalLin);

    virtual BOOL CalcSeisEvalLinRsltStlBeam(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_STLBEAM& raEvalLin);
    virtual BOOL CalcSeisEvalLinRsltStlColm(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_STLCOLM& raEvalLin);
    virtual BOOL CalcSeisEvalLinRsltStlBrac(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_STLBRAC& raEvalLin);

    virtual BOOL CalcSeisEvalLinRsltInfill(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_INFILL& raEvalLin);

    virtual BOOL CalcSeisEval1stRsltConBeam(T_ELEM_K ElemK, OUT T_EVAL1ST_BEAM& raEval1st);
    virtual BOOL CalcSeisEval1stRsltConColm(T_ELEM_K ElemK, OUT T_EVAL1ST_COLM& raEval1st);
    virtual BOOL CalcSeisEval1stRsltConWall(T_ELEM_K ElemK, OUT T_EVAL1ST_WALL& raEval1st);

    virtual BOOL CalcSeisEval1stRsltStlBeam(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBEAM& raEval1st);
    virtual BOOL CalcSeisEval1stRsltStlColm(T_ELEM_K ElemK, OUT T_EVAL1ST_STLCOLM& raEval1st);
    virtual BOOL CalcSeisEval1stRsltStlBrac(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBRAC& raEval1st);

    virtual BOOL CalcSeisEval1stRsltSrcBeam(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBEAM& raEval1st);
    virtual BOOL CalcSeisEval1stRsltSrcColm(T_ELEM_K ElemK, OUT T_EVAL1ST_STLCOLM& raEval1st);

    virtual BOOL CalcSeisEval1stRsltInfill(T_ELEM_K ElemK, OUT T_EVAL1ST_INFILL& raEval1st);

    virtual BOOL CalcSeisEval2ndRsltConBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_BEAM& raEval2nd);
    virtual BOOL CalcSeisEval2ndRsltConBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_BEAM& raEval2nd);

    virtual BOOL CalcSeisEval2ndRsltConColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_COLM& raEval2nd);
    virtual BOOL CalcSeisEval2ndRsltConColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_COLM& raEval2nd);

    virtual BOOL CalcSeisEval2ndRsltConWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_WALL& raEval2nd);
    virtual BOOL CalcSeisEval2ndRsltConWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_WALL& raEval2nd);

    virtual BOOL CalcSeisEval2ndRsltStlBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLBEAM& raEval2nd);
    virtual BOOL CalcSeisEval2ndRsltStlBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLBEAM& raEval2nd);

    virtual BOOL CalcSeisEval2ndRsltStlColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLCOLM& raEval2nd);
    virtual BOOL CalcSeisEval2ndRsltStlColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLCOLM& raEval2nd);

    virtual BOOL CalcSeisEval2ndRsltStlBrac(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLBRAC& raEval2nd);
    virtual BOOL CalcSeisEval2ndRsltStlBrac(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLBRAC& raEval2nd);

    virtual BOOL CalcSeisEval2ndRsltInfWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_INFILL& raEval2nd);
    virtual BOOL CalcSeisEval2ndRsltInfWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_INFILL& raEval2nd);

    virtual double GetAlphaConColm(const T_SECT_D& SectD) override { return 2.0; }

    virtual double GetPhiInfill() { return 0.6; }
    virtual BOOL IsConsiderVbjs(T_ELEM_K ElemK) { return TRUE; }

    virtual double CalcVs(const T_CON_SHR_PARAM& Param);
    virtual double CalcK1(const T_COLM_SHR_PARAM& ColmShr) override;
    virtual double CalcVsmaxFram(const T_CON_SHR_PARAM& Shr) override;    
    virtual double GetMFactForc(const EN_LEVEL enLevel) override;

    virtual double CalcMuPerVud(double dMu, double dVu, double dd);

    virtual double CalcVnConColm(const T_COLM_SHR_PARAM& Shr, double dPu, double dVu, double dMu);
	virtual void SetAccetanceCriteriaForcCtrl(OUT T_PHGT_FEMA& rPhgtD) override;
    virtual void SetAccetanceCriteriaConAxial(OUT T_PHGT_FEMA& rPhgtD) override;

    virtual EN_LEVEL GetPerformLevelForcCtrl(const double dDCR[3]) override;

private:
    void GetTable_ConBeam_CtrlFlex_C(OUT T_MEMBPARAM_TABLE_2& rTable);
    void GetTable_ConBeam_CtrlFlex_NC(OUT T_MEMBPARAM_TABLE_2& rTable);

    void GetTable_ConBeam_CtrlSher(double dS, double dd, OUT double radTable[6]);

    void GetHingeParamStl(double dA, double dB, double dC, double dIO, double dLS, double dCP, OUT double radParam[6]);
    
    BOOL IsTransReinfConforming(BOOL bUseSeisHook, double dV, const T_CON_SHR_PARAM& Shr);
    double CalcMainRebarIndex(double dfck, double dfy, double dB, double dH, double dM, const T_REBB_BASE& RebbTop, const T_REBB_BASE& RebbBot, double dDt, double dDb);
    void CalcMainRebarIndex(double dfck, double dfy, double dB, double dH, const T_REBB_BASE& RebbTop, const T_REBB_BASE& RebbBot, double dDt, double dDb, OUT double& rdIdxP, OUT double& rdIdxN);	

    BOOL CalcMyHingePropConBeamPos(T_UPHG_CONBEAM_POS& rUphg, OUT T_PHGT_FEMA& rPhgt);

    double CalcPnTensCon(double dfy, double dAs);
    double CalcPnCompCon(double dfck, double dAg, double dfy, double dAs, double dFactorPnmax);

    double CalcV0ConColmForFailMode(const T_COLM_SHR_PARAM& Shr);
    double CalcV0ConColmForFailMode(const T_WALL_SHR_PARAM& Shr);
    double CalcVpConColmForFailMode(double dfy, double dAs, double dH, double dLn);

    UINT GetGroupConColm(UINT unSeisDetail, double dVp, double dVn, BOOL bConditionGroup1);    
    BOOL IsConditionGroup1(const T_CON_SHR_PARAM& Param);

    BOOL CalcMyzHingePropConColmPos(OUT T_UPHG_CONCOLM_DIR& rUphg, OUT T_PHGT_FEMA& rPhgt);
    BOOL CalcMyzHingePropConColmPosConnectInfill(BOOL bConfined, double dPu, double dThetay, OUT T_PHGT_FEMA& rPhgtFema);

    void CalcMFactConColmDir(int nSeisCompType, OUT T_EVAL1ST_COLM_DIR& rEval);
    void CalcMFactConWall(int nSeisCompType, const T_CONWALL_INFO& Info, OUT T_EVAL1ST_WALL_POS& rEval);

    double CalcVnConWall(const T_WALL_SHR_PARAM& Shr);

    double InterpolateTableConWallCtrlFlex(double dP10V33, double dP10V50, double dP25V33, double dP25V50, double dPIdx, double dVIdx);

    void CalcParamConWallCtrlFlex(BOOL bExistBE, double dPIdx, double dVIdx, OUT double radParam[6]);
    void CalcParamConWallCtrlSher(double dPIdx, OUT double radParam[6]);

    BOOL CalcFzHingePropConWallPos(OUT T_UPHG_CONWALL_POS& rUphg, OUT T_PHGT_FEMA& rPhgt);
    BOOL CalcMyHingePropConWallPos(OUT T_UPHG_CONWALL_POS& rUphg, OUT T_PHGT_FEMA& rPhgt);

    BOOL CalcParamStlBeam(double dBTR, double dDTR, double adBTR[2], double adDTR[2], double dThetay, OUT double radParam[6]);
    BOOL CalcWTRLimitStlBeam(const CString& strSectShape, double dE, double dFye, OUT double radBTR[2], OUT double radDTR[2]);

    double CalcPnCompStl(double dAg, double dFy, double dE, double dSlen);
    double CalcPnTensStl(double dAg, double dFye);

    EN_STLCOLM GetStlColmForcType(double dP, double dPCL);
    double CalcKp(double dP, double dPy);
    BOOL CalcWTRLimitStlColm(const CString& strSectShape, EN_STLCOLM enType, double dE, double dFye, OUT double radBTR[2], OUT double radDTR[2]);

    BOOL CalcParamStlColm(EN_STLCOLM enForcType, double dkp, double dBTR, double dDTR, double adBTR[2], double adDTR[2], double dThetay, OUT double radParam[6]);

    BOOL CalcTableStlBracCompSlender(UINT unBuck, const CString& strShape, OUT double radTable[6]);
    BOOL CalcTableStlBracCompStocky(UINT unBuck, const CString& strShape, double dBTR, double dDTR, double adBTR[2], double adDTR[2], OUT double radTable[6]);
    BOOL CalcParamStlBracComp(UINT unBuck, const CString& strShape, double dSlen, double dFy, double dE, double dBTR, double dDTR, double adBTR[2], double adDTR[2], OUT double radParam[6]);	
    BOOL CalcParamStlBracTens(const CString& strShape, OUT double radParam[6]);

    BOOL CalcParamSrcBeam(OUT double radParam[6]);

    double CalcPnCompSrc(double dFck, double dAc, double dFy, double dAs, double dFyr, double dAsr);
    double CalcPnTensSrc(double dFy, double dAs, double dFyr, double dAsr);	

    BOOL CalcParamSrcColm(EN_STLCOLM enForcType, double dkp, OUT double radParam[6]);

    void CalcEval1stRsltConBeamPos(int nSeisCompType, BOOL bSpaceGTd2, OUT T_EVAL1ST_BEAM_POS& rEvalPos);
    
    void CalcEval1stStlBeamPos(int nSeisCompType, const T_EVAL1ST_STLRSLT_POS& RsltPos, double dBTR, double dDTR, double adBTR[2], double adDTR[2], OUT T_EVAL1ST_STLBEAM_POS& rEvalPos);

    void CalcEval1stStlColmPos(int nSeisCompType, const CString& strShape, const T_SECT_SECTBASE_D& SectBaseD, double dE, double dFye, double dPy,
        const T_EVAL1ST_STLRSLT_POS& RsltPos, OUT T_EVAL1ST_STLCOLM_POS& rEvalPos);

    void CalcMFactStlColm(EN_STLCOLM enForcType, double dkp, int nSeisCompType, double dBTR, double dDTR, double adBTR[2], double adDTR[2], OUT double radMFac[3]);

    void CalcMFactStlColmTens(int nSeisCompType, OUT double radMFac[3]);

    void CalcEval1stStlBracPos(int nSeisCompType, const CString& strShape, const T_SECT_SECTBASE_D& SectBaseD, double dFye, double dE, double dSlen,
        double adBTR[2], double adDTR[2], UINT unBuck, const T_EVAL1ST_STLRSLT_POS& RsltPos, OUT T_EVAL1ST_STLBRAC_POS& rEvalPos);

    void GetMFactTableStlBracCompSlender(int nSeisCompType, UINT unBuck, const CString& strShape, OUT double radTable[3]);
    void GetMFactTableStlBracCompStocky(int nSeisCompType, UINT unBuck, const CString& strShape, double dBTR, double dDTR, double adBTR[2], double adDTR[2], OUT double radTable[3]);

    void CalcMFactStlBracComp(int nSeisCompType, UINT unBuck, const CString& strShape, double dSlen, double dFy, double dE, double dBTR, double dDTR, double adBTR[2], double adDTR[2], OUT UINT& runSlenType, OUT double radMFac[3]);
    void CalcMFactStlBracTens(int nSeisCompType, const CString& strShape, OUT double radMFac[3]);

    void CalcEval1stRsltSrcBeamPos(int nSeisCompType, OUT T_EVAL1ST_STLBEAM_POS& rEvalPos);
    void CalcEval1stSrcColmPos(int nSeisCompType, double dPUF, double dPy, OUT T_EVAL1ST_STLCOLM_POS& rEvalPos);

    void CalcMFactSrcColm(EN_STLCOLM enColmForcType, double dkp, int nSeisCompType, OUT double radMFac[3]);

    void CalcSeisEval2ndRsltConColmDir(UINT unSeisDetail, BOOL bConditionGroup1, double dVu, double dVs, double dVo, const T_CON_SHR_PARAM& Shr, double dL, 
        double dMecur, double dMei, double dMej, OUT T_EVAL2ND_COLM_DIR& rEvalDir);

    void CalcInfillStrn(double dFmC, double dVm, double dPhi, int nBracingType, const T_MSIS_D& MsisD, BOOL bConsiderVbjs, OUT T_EVALLIN_INFILL_CAPA& rInfillCapa);

    void CalcMFactInfill(double dBeta, double dLmHm, OUT double radMFac[3]);
    void CalcMFactInfill(double dLmHm, double ad05TBL[2], double ad10TBL[2], double ad20TBL[2], OUT double radMFac[3]);

    BOOL MakeVframe1stColm(T_ELEM_K ElemK, BOOL bNodeITop, const T_STRB_D& StrbD, OUT T_VFRAME& rVframe);
    BOOL MakeVframe2ndColm(T_ELEM_K ElemK, BOOL bNodeITop, const T_SEIS_PO_ANAL_MAP& mPORslt, OUT T_VFRAME& rVframe);
    BOOL MakeVframe1stWall(T_ELEM_K ElemK, const T_STRW_D& StrwD, OUT T_VFRAME& rVframe);
    BOOL MakeVframe2ndWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_MAP& mPORslt, OUT T_VFRAME& rVframe);

    void CalcEval1stInfillPos(T_ELEM_K InfillK, double dBeta, double dLmHm, OUT T_EVAL1ST_INFILL_POS& rEvalPos);

    void CalcParamInfill(double dBeta, double dLm, double dHm, double dKmi, double dKms, double dThetam, double dQy, OUT T_PHGT_FEMA& rPhgtFema);
    double CalcParamInfill(double dLmHm, double dD05, double dD10, double dD20);
    void CalcParamInfillPos(T_NODE_K StrutK, double dBeta, double dLmHm, const T_MSIS_INPUT& MsisInp, const T_MSIS_PROP& MsisPro, double dVm, double dPm, OUT T_PHGT_FEMA& rPhgtFema);

    void CalcEvalLinStlMembPos(const T_EVALLIN_STLRSLT_POS& RsltPos, OUT T_EVALLIN_STLMEMB_POS& rEvalPos);
    void CalcEvalLinConBeamPos(const T_EVAL_CONRSLT_POS& RsltPos, double dVn, OUT T_EVALLIN_BEAM_POS& rEvalPos);
    void CalcEvalLinConColmPos(const T_EVAL_CONRSLT_POS& RsltPos, int nPMMethod, double dAlpha, const T_COLM_SHR_PARAM& Shry,
        const T_COLM_SHR_PARAM& Shrz, OUT T_EVALLIN_COLM_POS& rEvalPos);
    void CalcEvalLinConWallPos(const T_EVAL_CONRSLT_POS& RsltPos, int nPMMethod, BOOL bWeakAxis, double dAlpha, 
        const T_WALL_SHR_PARAM& Shry, const T_WALL_SHR_PARAM& Shrz, OUT T_EVALLIN_COLM_POS& rEvalPos);

    BOOL IsExistLinEvalLcom(T_ELEM_K ElemK, UINT unType);
    double GetPhiVForEvalLin();

private:
    bool MakeBeamInfo(T_ELEM_K ElemK, EN_EVALSTEP enStep, double adV[2], OUT T_CONBEAM_INFO raInfo[EN_MEMBEND_NUM]);

    void GetParamTable_ConBeam_CtrlFlex(BOOL bSeisConform, OUT T_MEMBPARAM_TABLE_2& rTable);    
    void GetMFactTable_ConBeam_CtrlFlex(int nSeisCompType, BOOL bSeisConform, OUT T_MEMBMFACT_TABLE_2& rTable);

    void GetParamTable_ConBeam_CtrlSher(BOOL bSpaceGTd2, OUT double radTable[6]);
    void GetMFactTable_ConBeam_CtrlSher(int nSeisCompType, BOOL bSpaceGTd2, OUT double raMFact[3]);

    void GetParamTable_ConColm_Group1(OUT T_MEMBPARAM_TABLE_2& rTable);  /// group i : ÈÚ ÆÄ±«.
    void GetMFactTable_ConColm_Group1(int nSeisCompType, OUT T_MEMBMFACT_TABLE_2& rTable);

    void GetParamTable_ConColm_Group2(OUT T_MEMBPARAM_TABLE_3& rTable);  /// group ii : ÈÚ Ç×º¹ ÈÄ, Àü´Ü ÆÄ±«.
    void GetMFactTable_ConColm_Group2(int nSeisCompType, OUT T_MEMBMFACT_TABLE_3& rTable);

    void GetParamTable_ConColm_Group3(OUT T_MEMBPARAM_TABLE_2& rTable);  /// group iii : Àü´Ü ÆÄ±«.
    void GetMFactTable_ConColm_Group3(int nSeisCompType, OUT T_MEMBMFACT_TABLE_2& rTable);

    void GetParamTable_ConColmInfilled_Comp(OUT double radTable[6]);
    void GetParamTable_ConColmInfilled_Tens(OUT double radTable[6]);
    void GetMFactTable_ConColmInfilled_Comp(int nSeisCompType, OUT double radMFact[3]);
    void GetMFactTable_ConColmInfilled_Tens(int nSeisCompType, OUT double radMFact[3]);

    void GetParamTable_ConWall_CtrlFlex(BOOL bBE, OUT T_MEMBPARAM_TABLE_2& rTable);
    void GetMFactTable_ConWall_CtrlFlex(int nSeisCompType, BOOL bBE, OUT T_MEMBMFACT_TABLE_2& rTable);

    void GetParamTable_ConWall_CtrlSher(double dAxlIdx, OUT double radTable[6]);
    void GetMFactTable_ConWall_CtrlSher(int nSeisCompType, double dAxlIdx, OUT double raMFact[3]);

    bool IsSectShapeStlBrac(const CString& strShape);
};


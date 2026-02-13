#pragma once

#include "SeisEvalKISTEC2013.h"

class CSeisEvalKISTEC2019 : public CSeisEvalKISTEC2013
{
public:
    CSeisEvalKISTEC2019(void);
    virtual ~CSeisEvalKISTEC2019(void) override;

    virtual BOOL InitializeHingeInfill(T_ELEM_K ElemK, const BOOL abDof[6], OUT T_PHGT_D& rPhgtD) override;
    virtual BOOL CalcUpdateHingeInfill(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_MAP& mPORslt, OUT T_PHGT_D& rPhgtD) override;

protected:
    virtual BOOL CalcMyHingePropConBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, double dRatioMuMy, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override final;

    virtual BOOL CalcFxHingePropConColm(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override final;
    virtual BOOL CalcFyzHingePropConColm(T_ELEM_K ElemK, EN_DIR enDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override final;
    virtual BOOL CalcMyzHingePropConColm(T_ELEM_K ElemK, EN_DIR enDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override final;

    virtual BOOL CalcFzHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override final;
    virtual BOOL CalcMyHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override final;
    
    virtual BOOL CalcFxHingePropInfill(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_MAP& mPORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ);

    virtual BOOL CalcMyHingePropStlBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override final;

    virtual BOOL CalcFxHingePropStlColm(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override final;	
    virtual BOOL CalcMyzHingePropStlColm(T_ELEM_K ElemK, UINT unDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override final;

    virtual BOOL CalcFxHingePropStlBrac(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override final;

    virtual BOOL CalcSeisEval1stRsltConColm(T_ELEM_K ElemK, OUT T_EVAL1ST_COLM& raEval1st) override final;
    virtual BOOL CalcSeisEval1stRsltConWall(T_ELEM_K ElemK, OUT T_EVAL1ST_WALL& raEval1st) override final;
    virtual BOOL CalcSeisEval1stRsltInfill(T_ELEM_K ElemK, OUT T_EVAL1ST_INFILL& raEval1st) override final;

    virtual BOOL CalcSeisEval1stRsltStlBeam(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBEAM& raEval1st) override final;
    virtual BOOL CalcSeisEval1stRsltStlColm(T_ELEM_K ElemK, OUT T_EVAL1ST_STLCOLM& raEval1st) override final;
    virtual BOOL CalcSeisEval1stRsltStlBrac(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBRAC& raEval1st) override final;    
    
    virtual BOOL CalcEvalPreInfWallSimple(T_ELEM_K ElemK, OUT T_EVALPRE_INFL& rEvalPre) override final;

    virtual BOOL CalcSeisEval2ndRsltConColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_COLM& raEval2nd) override final;
    virtual BOOL CalcSeisEval2ndRsltConWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_WALL& raEval2nd) override final;

    virtual BOOL CalcSeisEval2ndRsltInfWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_INFILL& raEval2nd) override final;
    virtual BOOL CalcSeisEval2ndRsltInfWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_INFILL& raEval2nd) override final;
    
    virtual double GetAlphaConColm(const T_SECT_D& SectD) override;    
	virtual void SetAccetanceCriteriaForcCtrl(OUT T_PHGT_FEMA& rPhgtD) override;
    virtual void SetAccetanceCriteriaConAxial(OUT T_PHGT_FEMA& rPhgtD) override;
    virtual EN_LEVEL GetPerformLevelForcCtrl(const double dDCR[3]) override; 

    virtual double CalcK1(const T_COLM_SHR_PARAM& ColmShr) override;
    virtual double CalcVsmaxFram(const T_CON_SHR_PARAM& Shr) override;    
    virtual double GetMFactForc(const EN_LEVEL enLevel) override;
            
    virtual double GetPhiInfill();
    
private:    
    BOOL CalcMyHingePropConBeamPos(OUT T_UPHG_CONBEAM_POS& rUphg, OUT T_PHGT_FEMA& rPhgt);
    BOOL CalcMyzHingePropConColmPos(OUT T_UPHG_CONCOLM_DIR& rUphg, OUT T_PHGT_FEMA& rPhgt);

    BOOL CalcFzHingePropConWallPos(OUT T_UPHG_CONWALL_POS& rUphg, OUT T_PHGT_FEMA& rPhgt);
    BOOL CalcMyHingePropConWallPos(OUT T_UPHG_CONWALL_POS& rUphg, OUT T_PHGT_FEMA& rPhgt);

    void CalcParamInfill(double dBeta, double dLm, double dHm, double dKmi, double dKms, double dThetam, double dQy, OUT T_PHGT_FEMA& rPhgt);

    void CalcMFactConColmDir(int nSeisCompType, OUT T_EVAL1ST_COLM_DIR& rEval);
    void CalcMFactConWall(int nSeisCompType, const T_CONWALL_INFO& Info, OUT T_EVAL1ST_WALL_POS& rEval);
    EN_EVALCTRL CalcWallCtrlType(double dMp, double dVn, double dHw, double dLw, const T_WALL_SHR_PARAM& Shr);

#pragma region /// INFILL-STRUT

    BOOL CalcCsPreInfill(T_ELEM_K ElemK, OUT T_EVALPRE_INFL_DIR& rEval);
    BOOL IsConsiderVbjs(T_ELEM_K ElemK);
    void CalcInfillStrn(double dFmC, double dVm, double dPhi, int nBracingType, const T_MSIS_D& MsisD, BOOL bConsiderVbjs, OUT T_EVALLIN_INFILL_CAPA& rInfillCapa);

    BOOL   IsOkOutPlane(const T_MSIS_D& MsisD, const T_SEMM_D& SemmD);
    BOOL   IsNeedCheckOutPlane(int nMasonryJoint, double dhm, double dtm, int nPerformLevel, double dSDS, double dSD1);
    BOOL   IsArchingAction(int nMasonryJoint, double dhmtm);
    double CalcHmTmLimitTable6_1_7(int nPerformLevel, double dVal, double dMin, double dMax);

    double CalcQtnArchingAction(double dhm, double dtm, double dfmc);
    double CalcQtnCantilever(double dhm, double dtm, double dfmt);
    double CalcFp(double nPerformLevel, double dtm, double dSDS, double dIe);

    double CalcLambda2(double dhm, double dtm);
    double GetXi(int nPerformLevel);
               
    BOOL MakeVframe1stColm(T_ELEM_K ElemK, BOOL bNodeITop, const T_STRB_D& StrbD, OUT T_VFRAME& rVframe);
    BOOL MakeVframe2ndColm(T_ELEM_K ElemK, BOOL bNodeITop, const T_SEIS_PO_ANAL_MAP& mPORslt, OUT T_VFRAME& rVframe);
    BOOL MakeVframe1stWall(T_ELEM_K ElemK, const T_STRW_D& StrwD, OUT T_VFRAME& rVframe);
    BOOL MakeVframe2ndWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_MAP& mPORslt, OUT T_VFRAME& rVframe);

    void CalcEval1stInfillPos(T_ELEM_K InfillK, double dBeta, double dLmHm, OUT T_EVAL1ST_INFILL_POS& rEvalPos);
    void CalcMFactInfill(double dBeta, double dLmHm, OUT double radMFac[3]);
    void CalcMFactInfill(double dLmHm, double ad05TBL[2], double ad10TBL[2], double ad20TBL[2], OUT double radMFac[3]);

#pragma endregion
    
    double CalcVnConColm(const T_COLM_SHR_PARAM& Shr, double dPu, double dVu, double dMu);
    double CalcMuPerVud(double dMu, double dVu, double dd);
    UINT GetGroupConColm(UINT unSeisDetail, double dVp, double dVn, BOOL bConditionGroup1);    
    BOOL IsConditionGroup1(const T_CON_SHR_PARAM& Shr);

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

    void GetHingeParamStl(double dA, double dB, double dC, double dIO, double dLS, double dCP, OUT double radParam[6]);

    void CalcSeisEval2ndRsltConColmDir(UINT unSeisDetail, BOOL bConditionGroup1, double dVu, double dVs, double dVo, const T_CON_SHR_PARAM& Shr, double dL, 
        double dMecur, double dMei, double dMej, OUT T_EVAL2ND_COLM_DIR& rEvalDir);

    bool IsSectShapeStlBrac(const CString& strShape);
};

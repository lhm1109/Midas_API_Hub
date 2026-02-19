#pragma once

#include "seiseval.h"

class CSeisEvalKISTEC2013 : public CSeisEval
{
public:
    CSeisEvalKISTEC2013(void);
    virtual ~CSeisEvalKISTEC2013(void) override;

protected:    
    virtual BOOL CalcMyHingePropConBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, double dRatioMuMy, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override;
	virtual double CalcBeta1(double dfck) override final;

    virtual BOOL CalcFxHingePropConColm(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override;
    virtual BOOL CalcFyzHingePropConColm(T_ELEM_K ElemK, EN_DIR enDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override;
    virtual BOOL CalcMyzHingePropConColm(T_ELEM_K ElemK, EN_DIR enDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override;

    virtual BOOL CalcFxHingePropConWall(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override final;
    virtual BOOL CalcFyHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override;
    virtual BOOL CalcFzHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override;
    virtual BOOL CalcMyHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override;
    virtual BOOL CalcMzHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtI, OUT T_PHGT_FEMA& rPhgtJ) override;

    virtual BOOL CalcMyHingePropStlBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ) override;

    virtual BOOL CalcFxHingePropStlColm(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ) override;	
    virtual BOOL CalcMyzHingePropStlColm(T_ELEM_K ElemK, UINT unDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ) override;

    virtual BOOL CalcFxHingePropStlBrac(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ) override;

    virtual BOOL CalcMyHingePropSrcBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ);

    virtual BOOL CalcFxHingePropSrcColm(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ);	
    virtual BOOL CalcMyzHingePropSrcColm(T_ELEM_K ElemK, UINT unDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ);

    virtual BOOL CalcEvalPreConColmSimple(T_ELEM_K ElemK, OUT T_EVALPRE_COLM& rEval) override;
    virtual BOOL CalcEvalPreConWallSimple(T_ELEM_K ElemK, OUT T_EVALPRE_WALL& rEval) override;

    virtual BOOL CalcEvalPreConColmGeneral(T_ELEM_K ElemK, OUT T_EVALPRE_COLM& rEval) override;
    virtual BOOL CalcEvalPreConWallGeneral(T_ELEM_K ElemK, OUT T_EVALPRE_WALL& rEval) override;
        
    virtual BOOL CalcSeisEval1stRsltConBeam(T_ELEM_K ElemK, OUT T_EVAL1ST_BEAM& raEval1st) override;
    virtual BOOL CalcSeisEval1stRsltConColm(T_ELEM_K ElemK, OUT T_EVAL1ST_COLM& raEval1st) override;
    virtual BOOL CalcSeisEval1stRsltConWall(T_ELEM_K ElemK, OUT T_EVAL1ST_WALL& raEval1st) override;

    virtual BOOL CalcSeisEval1stRsltStlBeam(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBEAM& raEval1st) override;
    virtual BOOL CalcSeisEval1stRsltStlColm(T_ELEM_K ElemK, OUT T_EVAL1ST_STLCOLM& raEval1st) override;
    virtual BOOL CalcSeisEval1stRsltStlBrac(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBRAC& raEval1st) override;

    virtual BOOL CalcSeisEval1stRsltSrcBeam(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBEAM& raEval1st) override final;
    virtual BOOL CalcSeisEval1stRsltSrcColm(T_ELEM_K ElemK, OUT T_EVAL1ST_STLCOLM& raEval1st) override final;

    virtual BOOL CalcSeisEval2ndRsltConBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_BEAM& raEval2nd) override final;
    virtual BOOL CalcSeisEval2ndRsltConBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_BEAM& raEval2nd) override final;

    virtual BOOL CalcSeisEval2ndRsltConColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_COLM& raEval2nd) override;
    virtual BOOL CalcSeisEval2ndRsltConColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_COLM& raEval2nd) override final;

    virtual BOOL CalcSeisEval2ndRsltConWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_WALL& raEval2nd) override;
    virtual BOOL CalcSeisEval2ndRsltConWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_WALL& raEval2nd) override final;

    virtual BOOL CalcSeisEval2ndRsltStlBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLBEAM& raEval2nd) override final;
    virtual BOOL CalcSeisEval2ndRsltStlBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLBEAM& raEval2nd) override final;

    virtual BOOL CalcSeisEval2ndRsltStlColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLCOLM& raEval2nd) override final;
    virtual BOOL CalcSeisEval2ndRsltStlColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLCOLM& raEval2nd) override final;

    virtual BOOL CalcSeisEval2ndRsltStlBrac(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLBRAC& raEval2nd);
    virtual BOOL CalcSeisEval2ndRsltStlBrac(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLBRAC& raEval2nd) override final;

    virtual double GetAlphaConColm(const T_SECT_D& SectD) override;    
	virtual void SetAccetanceCriteriaForcCtrl(OUT T_PHGT_FEMA& rPhgtD) override;
    virtual void SetAccetanceCriteriaConAxial(OUT T_PHGT_FEMA& rPhgtD) override;

    virtual double CalcK1(const T_COLM_SHR_PARAM& ColmShr) override;
    virtual double CalcVsmaxFram(const T_CON_SHR_PARAM& Shr) override;    
    virtual double GetMFactForc(const EN_LEVEL enLevel) override;
    virtual EN_LEVEL GetPerformLevelForcCtrl(const double dDCR[3]) override;
    
protected:

    double CalcPnTensCon(double dfy, double dAs);
    double CalcPnCompCon(double dfck, double dAg, double dfy, double dAs, double dFactorPmax);
    double CalcVnConWall(const T_WALL_SHR_PARAM& Shr);

    bool MakeBeamInfo(T_ELEM_K ElemK, EN_EVALSTEP enStep, double adV[2], OUT T_CONBEAM_INFO raInfo[EN_MEMBEND_NUM]);
        
private:
    BOOL CalcColmCategory(T_ELEM_K ElemK, OUT EN_COLM_CATEGORY& renCateX, OUT EN_COLM_CATEGORY& renCateY);
    BOOL CalcColmLHeightForGlobalAxis(T_ELEM_K ElemK, OUT double& rdH0X, OUT double& rdH0Y);

    BOOL IsTransReinfConforming(BOOL bUseSeisHook, double dV, const T_CON_SHR_PARAM& Shr);     

    BOOL CalcMyHingePropConBeamPos(int nSeisCompType, OUT T_UPHG_CONBEAM_POS& rUphg, OUT T_PHGT_FEMA& rPhgtFema);
    BOOL CalcMyzHingePropConColmPos(int nSeisCompType, OUT T_UPHG_CONCOLM_DIR& rUphg, OUT T_PHGT_FEMA& rPhgt);
        
    double CalcVnConColm(const T_COLM_SHR_PARAM& Shr, double dPu, double dVu, double dMu, double dmu);
    double CalcK2(double dmu);
    double CalcMuPerVud(double dMu, double dVu, double dd);        

    BOOL CalcMyHingePropConWallPos(int nSeisCompType, EN_EVALCTRL enCtrlType, BOOL bExistBE, double dAxialIndex, double dShearIndex, double dThetay, OUT T_PHGT_FEMA& rPhgt);        
    BOOL CalcFzHingePropConWallPos(int nSeisCompType, double dAxialIndex, double dThetay, OUT T_PHGT_FEMA& rPhgt);

    void CalcEval1stRsltConColmDir(int nSeisCompType, OUT T_EVAL1ST_COLM_DIR& rEvalDir);

    void CalcMFactConWall(int nSeisCompType, const T_CONWALL_INFO& Info, OUT T_EVAL1ST_WALL_POS& rEval);
        
    BOOL CalcParamStlBeam(int nSeisCompType, double dBTR, double dDTR, double adBTR[2], double adDTR[2], double dThetay, double radParam[6]);
    BOOL CalcWTRLimitStlBeam(const CString& strSectShape, double dE, double dFye, OUT double radBTR[2], OUT double radDTR[2]);
    void CalcWTRLimitShapeB(double dE, double dFye, OUT double radWTR[]);
    void CalcWTRLimitShapeP(double dFye, OUT double radWTR[]);

    double CalcPnCompStl(double dAg, double dFy, double dE, double dSlen);
    double CalcPnTensStl(double dAg, double dFye);	

    EN_STLCOLM GetStlColmForcType(double dP, double dPy);
    double CalcKp(double dP, double dPy);
    BOOL CalcWTRLimitStlColm(const CString& strSectShape, EN_STLCOLM enType, double dE, double dFye, OUT double radBTR[2], OUT double radDTR[2]);

    void SetStlForc(BOOL bEndI, const T_STRB_D& StrbDisp, const T_STRB_D& StrbForc, OUT T_EVAL1ST_STLCOLM_POS& rColmPos);
    void GetMFactStlColm(EN_STLCOLM enColmForcType, double dkp, int nSeisCompType, double dBTR, double dDTR,
        double adBTR[2], double adDTR[2], OUT double radmFac[3]);
    void GetMFactStlColmTens(int nSeisCompType, OUT double radmFac[3]);
    void GetMFactTableValueStlColm(EN_STLCOLM enColmForcType, double dkp, OUT double ramFacSC[3][2], OUT double ramFacNC[3][2]);

    BOOL CalcParamStlColm(int nSeisCompType, EN_STLCOLM enColmForcType, double dkp, double dBTR, double dDTR, double adBTR[2], double adDTR[2], double dThetay, OUT double radParam[6]);
    BOOL GetTableValueStlColmParam(int nSeisCompType, EN_STLCOLM enColmForcType, double dkp, double dParamSC[6], double dParamNC[6]);

    BOOL CalcParamStlBracComp(int nSeisCompType, UINT unBuck, const CString& strShape, double dBTR, double dDTR, double adBTR[2], double adDTR[2], OUT double radParam[6]);
    BOOL CalcParamStlBracTens(int nSeisCompType, OUT double radParam[6]);

    BOOL CalcParamStlBracCompShapeB(int nSeicCompType, double dBTR, double dDTR, double adBTR[2], double adDTR[2], OUT double radParam[6]);
    BOOL CalcParamStlBracCompShapeP(int nSeicCompType, double dBTR, double dDTR, double adBTR[2], double adDTR[2], OUT double radParam[6]);
    BOOL CalcParamStlBracCompShape2L2C(int nSeisCompType, UINT unBuck, OUT double radParam[6]);
    BOOL CalcParamStlBracCompShapeH(int nSeisCompType, OUT double radParam[6]);	

    BOOL CalcParamSrcBeam(double dThetay, OUT double radParam[6]);	
    BOOL CalcParamSrcColm(EN_STLCOLM enColmForcType, double dkp, double dThetay, OUT double radParam[6]);
    BOOL GetTableValueSrcColmParam(EN_STLCOLM enColmForcType, double dkp, double dParamSC[6]);

    double CalcPnCompSrc(double dFck, double dAc, double dFy, double dAs, double dFyr, double dAsr);
    double CalcPnTensSrc(double dFy, double dAs, double dFyr, double dAsr);	

    void CalcEval1stRsltConBeamPos(int nSeisCompType, BOOL bSpaceGTd2, OUT T_EVAL1ST_BEAM_POS& rEvalPos);
        
    void CalcEval1stRsltStlBeamPos(int nSeisCompType, const T_EVAL1ST_STLRSLT_POS& RsltPos, double dBTR, double dDTR, double adBTR[2], double adDTR[2],  OUT T_EVAL1ST_STLBEAM_POS& rEvalPos);

    void CalcEval1stStlColmPos(int nSeisCompType, const CString& strSectShape, const T_SECT_SECTBASE_D& SectBaseD, double dE, double dFye, double dPy,
        const T_EVAL1ST_STLRSLT_POS& RsltPos, OUT T_EVAL1ST_STLCOLM_POS& rEvalPos);

    void CalcEval1stStlBracPos(int nSeisCompType, const CString& strSectShpe, const T_SECT_SECTBASE_D& SectBaseD, double dFye, 
        double adBTR[2], double adDTR[2], UINT unBuckling, const T_EVAL1ST_STLRSLT_POS& RsltPos, OUT T_EVAL1ST_STLBRAC_POS& rEvalPos);

    void CalcEval1stRsltSrcBeamPos(int nSeisCompType, OUT T_EVAL1ST_STLBEAM_POS& rEvalPos);

    void CalcEval1stSrcColmPos(int nSeisCompType, double dPUF, double dPy, OUT T_EVAL1ST_STLCOLM_POS& rEvalPos);

    void GetMFactStlBrac(int nSeisCompType, double dPu, const CString& strSectShape, const T_SECT_SECTBASE_D& SectBaseD, double adBTR[2], double adDTR[2],
        UINT unBucking, OUT double radmFac[3]);
    void GetMFactStlBracShapeBP(double dBTR, double dDTR, double adBTR[2], double addTR[2], OUT double radmFact[3]);
    void GetMFactStlBracShape2L2C(int nSeisCompType, UINT unBucking, OUT double radmFac[3]);
    void GetMFactStlBracShapeH(OUT double radmFac[3]);

    void GetMFactSrcColm(EN_STLCOLM enColmForcType, double dkp, int nSeisCompType, OUT double radmFac[3]);

    void CalcSeisEval2ndRsltConColmDir(BOOL bUseHook, double dVu, double dVn, const T_CON_SHR_PARAM& Shr,
        double dL, double dMecur, double dMei, double dMej, OUT T_EVAL2ND_COLM_DIR& rEvalDir);


#pragma region /// [ m-Factor , modeling parameter table ]

    void GetParamTable_ConBeam_CtrlFlex(int nSeisCompType, BOOL bSeisConform, OUT T_MEMBPARAM_TABLE_2& rTable);    
    void GetMFactTable_ConBeam_CtrlFlex(int nSeisCompType, BOOL bSeisConform, OUT T_MEMBMFACT_TABLE_2& rTable);

    void GetParamTable_ConBeam_CtrlSher(int nSeisCompType, BOOL bSpaceGTd2, OUT double radTable[6]);
    void GetMFactTable_ConBeam_CtrlSher(int nSeisCompType, BOOL bSpaceGTd2, OUT double raMFact[3]);

    void GetParamTable_ConColm_CtrlFlex(int nSeisCompType, BOOL bSeisConform, OUT T_MEMBPARAM_TABLE_2& rTable);
    void GetMFactTable_ConColm_CtlrFlex(int nSeisCompType, BOOL bSeisConform, OUT T_MEMBMFACT_TABLE_2& rTable);

    void GetParamTable_ConWall_CtrlFlex(int nSeisCompType, BOOL bBE, OUT T_MEMBPARAM_TABLE_2& rTable);
    void GetMFactTable_ConWall_CtrlFlex(int nSeisCompType, BOOL bBE, OUT T_MEMBMFACT_TABLE_2& rTable);

    void GetParamTable_ConWall_CtrlSher(int nSeisCompType, double dAxlIdx, OUT double radTable[6]);
    void GetMFactTable_ConWall_CtrlSher(int nSeisCompType, double dAxlIdx, OUT double raMFact[3]);

#pragma endregion
        
    bool IsSectShapeStlBrac(const CString& strShape);     
};


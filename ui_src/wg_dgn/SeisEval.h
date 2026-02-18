#pragma once

#include "..\wg_db\SeisEvalStruct.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\ColumnDividerByInfillStrut.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_db\DbToolStruct.h"

#include "SeisEvalCalcStruct.h"

enum { INITSTIFF_6EIL=0, INITSTIFF_3EIL, INITSTIFF_2EIL, INITSTIFF_USER, INITSTIFF_ELASTIC, INITSTIFF_SKELETON };
enum { WTR_SC=0, WTR_NC };  /// Seismic Compact / Non Compact
enum { BUCK_IN=0, BUCK_OUT };
enum { INTER_NONE=0, INTER_PMM, INTER_PM };
enum { STEP_NORMAL=0, STEP_ALL, STEP_MAX, STEP_MIN };

const double cThetaLimit = 0.00001;
const double cThetaLimit2 = 0.00001*0.00001;
const double cZeroLimit  = 1.0e-7;

struct T_STRN_D
{
	double dStrain;
	double dLimitStrain;
	T_STRN_D() { dStrain = dLimitStrain = 0.0; };
	T_STRN_D(double dVal1, double dVal2) { dStrain = dVal1, dLimitStrain = dVal2; }
};

class CDBDoc;
class CMembCtrl;
class CSeisEvalResult;
class CSeisEval
{
public:
    CSeisEval(void);
    virtual ~CSeisEval(void);

public:
	virtual BOOL InitializeHingeConBeam(const T_HINGE_CALC_PARAM& Param, OUT T_PHGT_D& rPhgtD);
	virtual BOOL InitializeHingeConColm(const T_HINGE_CALC_PARAM& Param, OUT T_PHGT_D& rPhgtD);
	virtual BOOL InitializeHingeConWall(const T_HINGE_CALC_PARAM& Param, OUT T_PHGT_D& rPhgtD);

    BOOL InitializeHingeStlBeam(const BOOL abDof[6], OUT T_PHGT_D& rPhgtD);
    BOOL InitializeHingeStlColm(const BOOL abDof[6], OUT T_PHGT_D& rPhgtD);
    BOOL InitializeHingeStlBrac(const BOOL abDof[6], OUT T_PHGT_D& rPhgtD);
    BOOL InitializeHingeStlTrus(const BOOL abDof[6], OUT T_PHGT_D& rPhgtD);

    BOOL InitializeHingeSrcBeam(const BOOL abDof[6], OUT T_PHGT_D& rEncasePhgtD, OUT T_PHGT_D& rFilledPhgtD);
    BOOL InitializeHingeSrcColm(const BOOL abDof[6], OUT T_PHGT_D& rEncasePhgtD, OUT T_PHGT_D& rFilledPhgtD);

    BOOL InitializeHingeInfill(const BOOL abDof[6], OUT T_PHGT_D& rPartPhgtD, OUT T_PHGT_D& rFullPhgtD);
    virtual BOOL InitializeHingeInfill(T_ELEM_K ElemK, const BOOL abDof[6], OUT T_PHGT_D& rPhgtD) { return FALSE; }

    BOOL CalcUpdateHingeConBeam(T_ELEM_K ElemK, const T_HINGE_CALC_PARAM& Param, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD);
	BOOL CalcUpdateHingeConColm(T_ELEM_K ElemK, const T_HINGE_CALC_PARAM& Param, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD);
    BOOL CalcUpdateHingeConWall(T_ELEM_K ElemK, const T_HINGE_CALC_PARAM& Param, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD);
    void SetHingeParamPMM(T_ELEM_K ElemK, int nUpdateMatl, const EN_NLANALTYPE& enNLType, OUT T_PHGT_D& rPhgtD);

    BOOL CalcUpdateHingeStlBeam(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD);
    BOOL CalcUpdateHingeStlColm(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD);
    BOOL CalcUpdateHingeStlBrac(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD);
    BOOL CalcUpdateHingeStlTrus(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD);

    BOOL CalcUpdateHingeSrcBeam(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD);
    BOOL CalcUpdateHingeSrcColm(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD);

    virtual BOOL CalcUpdateHingeInfill(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_MAP& mPORslt, OUT T_PHGT_D& rPhgtD) { return FALSE; }
    
    virtual BOOL PrintSeisEval1stRsltSrcBeam4Memb();
    virtual BOOL PrintSeisEval1stRsltSrcColm4Memb();	

    BOOL CalcEvalPreConColmAll(T_EVALPRE_COLM_MAP &rmPreConColm);
    BOOL CalcEvalPreConWallAll(T_EVALPRE_WALL_MAP &rmPreConWall);
    
    BOOL CalcEvalLinConBeamAll(CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_BEAM, T_EVALLIN_BEAM&> &rmLinConBeam);
    BOOL CalcEvalLinConColmAll(CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_COLM, T_EVALLIN_COLM&> &rmLinConColm);
    BOOL CalcEvalLinConWallAll(CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_WALL, T_EVALLIN_WALL&> &rmLinConWall);
    BOOL CalcEvalLinStlBeamAll(CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_STLBEAM, T_EVALLIN_STLBEAM&> &rmLinStlBeam);
    BOOL CalcEvalLinStlColmAll(CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_STLCOLM, T_EVALLIN_STLCOLM&> &rmLinStlColm);
    BOOL CalcEvalLinStlBracAll(CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_STLBRAC, T_EVALLIN_STLBRAC&> &rmLinStlBrac);
    BOOL CalcEvalLinInfWallAll(CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_INFILL, T_EVALLIN_INFILL&> &rmLinInfWall);
    BOOL CalcEvalInfWallOutAll(CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL_INFILL_OUT, T_EVAL_INFILL_OUT&> &rmInfWallOut);

    BOOL CalcEval1stConBeamAll(CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_BEAM, T_EVAL1ST_BEAM&> &rm1stConBeam);
    BOOL CalcEval1stConColmAll(CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_COLM, T_EVAL1ST_COLM&> &rm1stConColm);
    BOOL CalcEval1stConWallAll(CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_WALL, T_EVAL1ST_WALL&> &rm1stConWall);
    BOOL CalcEval1stStlBeamAll(CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_STLBEAM, T_EVAL1ST_STLBEAM&> &rm1stStlBeam);
    BOOL CalcEval1stStlColmAll(CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_STLCOLM, T_EVAL1ST_STLCOLM&> &rm1stStlColm);
    BOOL CalcEval1stStlPanzAll(CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_STLPANZ, T_EVAL1ST_STLPANZ&> &rm1stStlPanz);
    BOOL CalcEval1stStlBracAll(CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_STLBRAC, T_EVAL1ST_STLBRAC&> &rm1stStlBrac);
    BOOL CalcEval1stInfWallAll(CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_INFILL, T_EVAL1ST_INFILL&> &rm1stInfWall);

    BOOL CalcEval2ndConBeamAll(CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_BEAM_ALL, T_EVAL2ND_BEAM_ALL&> &rm2ndConBeam);
    BOOL CalcEval2ndConColmAll(CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_COLM_ALL, T_EVAL2ND_COLM_ALL&> &rm2ndConColm);
    BOOL CalcEval2ndConWallAll(CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_WALL_ALL, T_EVAL2ND_WALL_ALL&> &rm2ndConWall);
    BOOL CalcEval2ndStlBeamAll(CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_STLBEAM_ALL, T_EVAL2ND_STLBEAM_ALL&> &rm2ndStlBeam);
    BOOL CalcEval2ndStlColmAll(CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_STLCOLM_ALL, T_EVAL2ND_STLCOLM_ALL&> &rm2ndStlColm);
    BOOL CalcEval2ndStlBracAll(CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_STLBRAC_ALL, T_EVAL2ND_STLBRAC_ALL&> &rm2ndStlBrac);
    BOOL CalcEval2ndInfWallAll(CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_INFILL_ALL, T_EVAL2ND_INFILL_ALL&> &rm2ndInfWall);
        
    virtual BOOL CalcEvalPreConColmSimple(T_ELEM_K ElemK, OUT T_EVALPRE_COLM& rEvalPre) { return FALSE; }
    virtual BOOL CalcEvalPreConWallSimple(T_ELEM_K ElemK, OUT T_EVALPRE_WALL& rEvalPre) { return FALSE; }
    virtual BOOL CalcEvalPreInfWallSimple(T_ELEM_K ElemK, OUT T_EVALPRE_INFL& rEvalPre) { return FALSE; }

    virtual BOOL CalcEvalPreConColmGeneral(T_ELEM_K ElemK, OUT T_EVALPRE_COLM& rEvalPre) { return FALSE; }
    virtual BOOL CalcEvalPreConWallGeneral(T_ELEM_K ElemK, OUT T_EVALPRE_WALL& rEvalPre) { return FALSE; }
        
    virtual BOOL CalcEvalPreStlColm(T_ELEM_K ElemK, OUT T_EVALPRE_STLCOLM& rEvalPre) { return FALSE; }
    virtual BOOL CalcEvalPreStlBrac(T_ELEM_K ElemK, OUT T_EVALPRE_STLBRAC& rEvalPre) { return FALSE; }

    virtual BOOL CalcEvalPreStlColmInfo(T_ELEM_K ElemK, OUT T_EVALPRE_STLCOLMINFO& rEvalInfo) { return FALSE; }
    virtual BOOL CalcEvalPreStlBeamInfo(T_ELEM_K ElemK, OUT T_EVALPRE_STLBEAMINFO& rEvalInfo) { return FALSE; }
    virtual BOOL CalcEvalPreStlPanzInfo(T_ELEM_K ElemK, OUT T_EVALPRE_STLPANZINFO& rEvalInfo) { return FALSE; }
    virtual BOOL CalcEvalPreStlBracInfo(T_ELEM_K ElemK, OUT T_EVALPRE_STLBRACINFO& rEvalInfo) { return FALSE; }

    virtual BOOL CalcSeisEvalLinRsltConBeam(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_BEAM& raEval1st) { return FALSE; }
    virtual BOOL CalcSeisEvalLinRsltConColm(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_COLM& raEval1st) { return FALSE; }
    virtual BOOL CalcSeisEvalLinRsltConWall(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_WALL& raEval1st) { return FALSE; }

    virtual BOOL CalcSeisEvalLinRsltStlBeam(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_STLBEAM& raEval1st) { return FALSE; }
    virtual BOOL CalcSeisEvalLinRsltStlColm(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_STLCOLM& raEval1st) { return FALSE; }
    virtual BOOL CalcSeisEvalLinRsltStlBrac(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_STLBRAC& raEval1st) { return FALSE; }

    virtual BOOL CalcSeisEvalLinRsltInfill(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_INFILL& raEvalLin) { return FALSE; }

    virtual BOOL CalcSeisEval1stRsltConBeam(T_ELEM_K ElemK, OUT T_EVAL1ST_BEAM& raEval1st) { return FALSE; }
    virtual BOOL CalcSeisEval1stRsltConColm(T_ELEM_K ElemK, OUT T_EVAL1ST_COLM& raEval1st) { return FALSE; }
    virtual BOOL CalcSeisEval1stRsltConWall(T_ELEM_K ElemK, OUT T_EVAL1ST_WALL& raEval1st) { return FALSE; }

    virtual BOOL CalcSeisEval1stRsltStlBeam(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBEAM& raEval1st) { return FALSE; }
    virtual BOOL CalcSeisEval1stRsltStlColm(T_ELEM_K ElemK, OUT T_EVAL1ST_STLCOLM& raEval1st) { return FALSE; }
    virtual BOOL CalcSeisEval1stRsltStlPanz(T_ELEM_K ElemK, OUT T_EVAL1ST_STLPANZ& raEval1st) { return FALSE; }
    virtual BOOL CalcSeisEval1stRsltStlBrac(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBRAC& raEval1st) { return FALSE; }

    virtual BOOL CalcSeisEval1stRsltSrcBeam(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBEAM& raEval1st) { return FALSE; }
    virtual BOOL CalcSeisEval1stRsltSrcColm(T_ELEM_K ElemK, OUT T_EVAL1ST_STLCOLM& raEval1st) { return FALSE; }

    virtual BOOL CalcSeisEval1stRsltInfill(T_ELEM_K ElemK, OUT T_EVAL1ST_INFILL& raEval1st) { return FALSE; }

    virtual BOOL CalcSeisEval2ndRsltConBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_BEAM& raEval2nd) { return FALSE; }
    virtual BOOL CalcSeisEval2ndRsltConBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_BEAM& raEval2nd) { return FALSE; }

	virtual BOOL CalcSeisEval2ndRsltConColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_COLM& raEval2nd) { return FALSE; }
	virtual BOOL CalcSeisEval2ndRsltConColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_COLM& raEval2nd) { return FALSE; }

	virtual BOOL CalcSeisEval2ndRsltConWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_WALL& raEval2nd) { return FALSE; }
	virtual BOOL CalcSeisEval2ndRsltConWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_WALL& raEval2nd) { return FALSE; }
	
    virtual BOOL CalcSeisEval2ndRsltConColmAgbp(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd) { return FALSE; }
    virtual BOOL CalcSeisEval2ndRsltConColmAgbp(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd) { return FALSE; }

    virtual BOOL CalcSeisEval2ndRsltConWallAgwp(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd) { return FALSE; }
    virtual BOOL CalcSeisEval2ndRsltConWallAgwp(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd) { return FALSE; }

	virtual BOOL CalcSeisEval2ndRsltConColmFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd) { return FALSE; }
	virtual BOOL CalcSeisEval2ndRsltConColmFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd) { return FALSE; }

	virtual BOOL CalcSeisEval2ndRsltConWallFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd) { return FALSE; }
	virtual BOOL CalcSeisEval2ndRsltConWallFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd) { return FALSE; }

    virtual BOOL CalcSeisEval2ndRsltStlBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLBEAM& raEval2nd) { return FALSE; }
    virtual BOOL CalcSeisEval2ndRsltStlBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLBEAM& raEval2nd) { return FALSE; }

    virtual BOOL CalcSeisEval2ndRsltStlColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLCOLM& raEval2nd) { return FALSE; }
    virtual BOOL CalcSeisEval2ndRsltStlColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLCOLM& raEval2nd) { return FALSE; }

    virtual BOOL CalcSeisEval2ndRsltStlBrac(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLBRAC& raEval2nd) { return FALSE; }
    virtual BOOL CalcSeisEval2ndRsltStlBrac(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLBRAC& raEval2nd) { return FALSE; }

    virtual BOOL CalcSeisEval2ndRsltInfWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_INFILL& raEval2nd) { return FALSE; }
    virtual BOOL CalcSeisEval2ndRsltInfWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_INFILL& raEval2nd) { return FALSE; }

    virtual BOOL CalcSeisEvalInfillOut(T_ELEM_K ElemK, OUT T_EVAL_INFILL_OUT& raEval) { return FALSE; }

	virtual BOOL CalcSeisEval2ndRsltConBeamTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nHingeTimeIndex, OUT T_EVAL2ND_BEAM& raEval2nd) { return FALSE; }
	
	virtual BOOL CalcSeisEval2ndRsltConColmTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nHingeTimeIndex, int nSerialAgxt, OUT T_EVAL2ND_COLM& raEval2nd) { return FALSE; }
	virtual BOOL CalcSeisEval2ndRsltConColmFiberTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nHingeTimeIndex, int nSerialAgxt, OUT T_EVAL2ND_COLM& raEval2nd) { return FALSE; }

    virtual bool ReCalcSeisEval2ndRsltConBeamLumpedTH(const T_EVAL2ND_BEAM_POS& Data, OUT T_EVAL2ND_BEAM_POS& raEval2nd) { return false; }
    virtual bool ReCalcSeisEval2ndRsltConColmLumpedTH(const int nDOF, const T_EVAL2ND_COLM_POS& Data, OUT T_EVAL2ND_COLM_POS& raEval2nd) { return false; }
    virtual bool ReCalcSeisEval2ndRsltConColmFiberTHRAngle(const T_EVAL2ND_COLM_POS& Data, OUT T_EVAL2ND_COLM_POS& raEval2nd) { return false; }
    virtual bool ReCalcSeisEval2ndRsltConColmFiberTHShear(const T_EVAL2ND_COLM_POS& Data, OUT T_EVAL2ND_COLM_POS& raEval2nd) { return false; }

	virtual BOOL CalcSeisEval2ndRsltConWallFiberTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nHingeTimeIndex, int nSerialAgxt, OUT T_EVAL2ND_WALL& raEval2nd) { return FALSE; }

    virtual bool ReCalcSeisEval2ndRsltConWallFiberTHRAngle(const T_EVAL2ND_WALL_POS& Data, OUT T_EVAL2ND_WALL_POS& raEval2nd) { return false; }
    virtual bool ReCalcSeisEval2ndRsltConWallFiberTHShear(const T_EVAL2ND_WALL_POS& Data, OUT T_EVAL2ND_WALL_POS& raEval2nd) { return false; }

	virtual BOOL GetLevelSeisEval2ndConFramTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nHingeTimeIndex, int nDof, OUT UINT& runLevelI, OUT UINT& runLevelJ) { return FALSE; }

    virtual BOOL GetSeisEval2ndRsltSubConFramTHFiberShear(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nHingeType, int nDof, OUT T_EVAL2ND_PHNE& rPhneDI, OUT T_EVAL2ND_PHNE& rPhneDJ) { return FALSE; }
    virtual BOOL GetSeisEval2ndRsltSubConWallTHFiberShear(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nHingeType, int nDof, OUT T_EVAL2ND_WALL_POS& rPosI, OUT T_EVAL2ND_WALL_POS& rPosJ) { return FALSE; }

    virtual EN_LEVEL GetPerformLevelForcCtrl(const double dDCR[3]);
        
    virtual BOOL IsIgnoreMomentHingeProp() { return FALSE; }

    CString GetPerformLevel4TextOut(EN_LEVEL enLevel);    
    EN_LEVEL GetPerformLevel(const double dDCR[3]);    
    void SetPerformLevel(OUT T_EVAL1ST_DCR& rDCR);
    void SetPerformLevelForcCtrl(OUT T_EVAL1ST_DCR& rDCR);

    CString GetValueForm(int nTotalDigit, int nUnderDigit, double dValue);

    double CalcVnmaxWall(const T_WALL_SHR_PARAM& WallShr);
    double CalcVsmaxWall(const T_WALL_SHR_PARAM& WallShr, double dVc);

    T_UNIT_INDEX GetCurrUnit() { return m_CurrUnit; }
    T_UNIT_INDEX GetCodeUnit() { return m_CodeUnit; }

protected:

    void SetCodeUnit();

    CSeisEvalResult* GetSEResult();

    void SetUphgParam(const double adTable[6], const double adParam[6], OUT T_UPHG_PARAM& rParam);

    T_ELEM_K GetElemShowKey(T_ELEM_K ElemK);

    int GetLcomKeyListLinEval(T_ELEM_K ElemK, UINT unType, OUT T_LCOM_K_LIST& raLcomK);
    int GetLcomKeyList1stEval(T_ELEM_K ElemK, OUT T_LCOM_K_LIST& raLcomK);

    BOOL GetSeisParam(const T_SEIS_D& SeisD, OUT T_SEIS_ITEM& rSeisItem);
    BOOL GetSpfcParam(const T_SPFC_D& SpfcD, OUT T_SEIS_ITEM& rSeisItem);

    double GetConLambda(const T_MATD_D& MatdD);
    double GetScaleUpFactor4MFact();    
    BOOL GetUseSeisHookWall(T_ELEM_K ElemK);
    void GetColmClearHeight(T_ELEM_K ElemK, OUT double& rdh0y, OUT double& rdh0z);
        
    EN_STLBEAM_SWAYTYPE GetStlBeamSwayType(T_ELEM_K ElemK);
    
    UINT GetPhgeType(const T_ELEM_K ElemK);
    int GetPhgtPQInteractionType();
    BOOL GetInfillParam(T_ELEM_K ElemK, OUT T_MSIS_D& rMsisD);
        
    bool GetElemMatlSectCon(T_ELEM_K ElemK, OUT T_SECT_D& rSectD, OUT T_MATD_D& rMatdD, OUT T_SECM_D& rSecmD);
    bool GetElemMatlSectStl(T_ELEM_K ElemK, OUT T_SECT_D& rSectD, OUT T_MATD_D& rMatdD, OUT T_SESM_D& rSesmD);
    bool GetElemMatlSectSrc(T_ELEM_K ElemK, OUT T_SECT_D& rSectD, OUT T_MATD_D& rMatdD, OUT T_SESM_D& rSesmD, OUT T_SECM_D& rSecmD);
    bool GetElemMatlSectMas(T_ELEM_K ElemK, OUT T_SECT_D& rSectD, OUT T_MATD_D& rMatdD, OUT T_SEMM_D& rSemmD);
    bool GetElemMatlThik(T_ELEM_K ElemK, OUT T_THIK_D& rThikD, OUT T_MATD_D& rMatdD, OUT T_SECM_D& rSecmD);

	bool GetElemMatlSectCon(T_ELEM_K ElemK, OUT T_SECT_D& rSectD, OUT T_MATD_D& rMatdD, OUT T_PDCM_D& rPdcmD);
	bool GetElemMatlThik(T_ELEM_K ElemK, OUT T_THIK_D& rThikD, OUT T_MATD_D& rMatdD, OUT T_PDCM_D& rPdcmD);

    T_SECT_SECTBASE_D GetSectBaseEndI(const T_SECT_D& SectD);
    T_SECT_SECTBASE_D GetSectBaseEndJ(const T_SECT_D& SectD);

    EN_DIR GetPairDirV(const EN_DIR& enDirM);
    EN_WALL_FORC_POS GetWallForcPos(const EN_MEMBEND& enEnd);
    EN_BEAM_POS GetBeamPos(const EN_MEMBEND& enEnd);

    BOOL GetEvalStrb1st(CMembCtrl* pMembCtrl, T_ELEM_K ElemK, T_LCOM_K LcomK, OUT T_STRB_D& rStrbDisp, OUT T_STRB_D& rStrbForc);
    BOOL GetEvalStrt1st(CMembCtrl* pMembCtrl, T_ELEM_K ElemK, T_LCOM_K LcomK, OUT T_STRT_D& rStrtDisp, OUT T_STRT_D& rStrtForc);
    BOOL GetEvalStrw1st(CMembCtrl* pMembCtrl, UINT unInternalWallID, T_LCOM_K LcomK, OUT T_STRW_D& rStrwDisp, OUT T_STRW_D& rStrwForc);

    BOOL GetEvalStrbLin(CMembCtrl* pMembCtrl, T_ELEM_K ElemK, T_LCOM_K LcomK, OUT T_STRB_D& rStrbD);
    BOOL GetEvalStrtLin(CMembCtrl* pMembCtrl, T_ELEM_K ElemK, T_LCOM_K LcomK, OUT T_STRT_D& rStrtD);
    BOOL GetEvalStrwLin(CMembCtrl* pMembCtrl, UINT unInternalWallID, T_LCOM_K LcomK, OUT T_STRW_D& rStrwD);

    BOOL GetEvalStrb2nd4AddStep(T_ELEM_K ElemK, T_POLC_K PolcK, int nAddStepIdx, T_STRB_D &rStrbD);
    BOOL GetEvalStrw2nd4AddStep(T_ELEM_K ElemK, T_POLC_K PolcK, int nAddStepIdx, T_STRW_D &rStrwD);
    BOOL GetSeisEval2ndRsltSubConBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_BEAM& raEval2nd);
    BOOL GetSeisEval2ndRsltSubConColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_COLM& raEval2nd);
    BOOL GetSeisEval2ndRsltSubConWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_WALL& raEval2nd);
	BOOL GetSeisEval2ndRsltSubConColmFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd);
	BOOL GetSeisEval2ndRsltSubConWallFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd);
	
    BOOL GetSeisEval2ndRsltSubStlBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLBEAM& raEval2nd);
    BOOL GetSeisEval2ndRsltSubStlColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLCOLM& raEval2nd);
    BOOL GetSeisEval2ndRsltSubStlBrac(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLBRAC& raEval2nd);
    BOOL GetSeisEval2ndRsltSubInfWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_INFILL& raEval2nd);

	BOOL CalcSeisEval2ndConColmFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd) { return TRUE; }
	BOOL CalcSeisEval2ndConWallFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd) { return TRUE; }

	BOOL GetSeisEval2ndRsltSubConBeamTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nHingeType, OUT T_EVAL2ND_BEAM& raEval2nd);
	BOOL GetSeisEval2ndRsltSubConColmTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nHingeType, OUT T_EVAL2ND_COLM& raEval2nd);
	BOOL GetSeisEval2ndRsltSubConColmFiberTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nHingeType, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd);
	BOOL GetSeisEval2ndRsltSubConWallFiberTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nHingeType, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd);

	BOOL GetSeisEval2ndRsltSubConFramTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nHingeType, int nDof, OUT T_EVAL2ND_PHNE& rPhneDI, OUT T_EVAL2ND_PHNE& rPhneDJ);
	BOOL GetSeisEval2ndRsltSubConWallTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nHingeType, int nDof, OUT T_EVAL2ND_PHNE& rPhneDI, OUT T_EVAL2ND_PHNE& rPhneDJ);
		
    void SetEvalConColm1stStrn(double dPnC, double dPnT, double dPeC, double dPeT, double dMey, double dMez, double dVny, double dVnz, OUT T_EVAL1ST_COLM_POS& rPos);

    void SetEvalConBeam1stForc(const T_STRB_D& StrbDisp, const T_STRB_D& StrbForc, OUT T_EVAL1ST_BEAM_POS& rPosI, OUT T_EVAL1ST_BEAM_POS& rPosJ);
    void SetEvalConColm1stForc(const T_STRB_D& StrbDisp, const T_STRB_D& StrbForc, OUT T_EVAL1ST_COLM_POS& rPosI, OUT T_EVAL1ST_COLM_POS& rPosJ);
    void SetEvalConWall1stForc(const T_STRW_D& StrwDisp, const T_STRW_D& StrwForc, OUT T_EVAL1ST_WALL_POS& rPosI, OUT T_EVAL1ST_WALL_POS& rPosJ);
    void SetEvalSrcColm1stForc(const T_STRB_D& StrbDisp, const T_STRB_D& StrbForc, OUT T_EVAL1ST_STLCOLM_POS& rPosI, OUT T_EVAL1ST_STLCOLM_POS& rPosJ);

    void SetEvalConBeamLinForc(const T_STRB_D& StrbD, OUT T_EVALLIN_BEAM_POS& rPosI, OUT T_EVALLIN_BEAM_POS& rPosM, OUT T_EVALLIN_BEAM_POS& rPosJ);

    BOOL GetMatlSE(T_ELEM_K ElemK, EN_EVALSTEP enStep, EN_EVALMATL enMatl, OUT _DB_PMCV_USER_MATLSE& rMatlSE);
    BOOL GetPmcvData(T_ELEM_K ElemK, EN_EVALSTEP enStep, EN_EVALMATL enMatl, OUT _DB_PMCV_DATA& rPmcvy, OUT _DB_PMCV_DATA& rPmcvz);
    bool ConvertPmcvData(const _DB_PMCV_DATA& srcPmcv, OUT T_PMCV_TABLE& rtarPmcv);

    BOOL GetPnSrcColm(T_ELEM_K ElemK, EN_EVALSTEP enStep, EN_EVALMATL enMatl, OUT double radPc[2], OUT double radPt[2]);

    BOOL GetSectDim(const T_SECT_D& SectD, OUT double& rdB, OUT double& rdH);

    double GetRebarFy(double dFyByMatd);
    double GetRebarAs(const CString& strBarName);
    double GetRebarDia(const CString& strBarName);

    double CalcConBeamRhoBal(double dfck, double dfy);
    void CalcConBeamRho(double dB, double dH, const T_REBB_BASE& RebbBase, double dDc, OUT T_CONBEAM_FLEX& rFlex);
    void CalcConBeamFlex(double dB, double dH, double dfce, double dfye, const T_REBB_BASE& RebbBase, double dDc, OUT T_CONBEAM_FLEX& rFlex);

    void SetCurveForcCtrlAxial(OUT double adCurv[2][9]);
    void SetCurveForcCtrlShear(OUT double adCurv[2][9]);
    
    void SetCurveDispCtrl(const double adParam[6], OUT double adCurv[2][9]);
    void SetCurveDispCtrl(const double adParamP[6], const double adParamN[6], OUT double adCurv[2][9]);
    
    void SetCurveDispCtrlStl(const double adParam[6], OUT double adCurv[2][9]);
    void SetCurveDispCtrlStl(const double adParamP[6], const double adParamN[6], OUT double adCurv[2][9]);
    
    void SetAccetanceCriteria(const double adParam[6], OUT T_PHGT_FEMA& rPhgt);
    void SetAccetanceCriteria(const double adParamP[6], const double adParamN[6], OUT T_PHGT_FEMA& rPhgt);
	void SetAccetanceCriteria(double dIO, double dLS, double dCP, OUT T_PHGT_FEMA& rPhgt);
    void SetAccetanceCriteria(double dPIO, double dPLS, double dPCP, double dMIO, double dMLS, double dMCP, OUT T_PHGT_FEMA& rPhgt);

    double CalcDDyFromAB(double dValue, double dThetay);
    double CalcDDyFromDE(double dValue, double dThetay);    
    double CalcDistRatio(double dVal, double dValMin, double dValMax);

    double CalcStressBlockHeightConBeam(double dfc, double dfy, double dAs, double dB);
    double CalcMeConBeam(double dfce, double dB, double dH, double dfye, const T_REBB_BASE& RebbBase, double dDc);    
    double CalcVpConBeam(double dMeiP, double dMeiN, double dMejP, double dMejN, double dL);
    double CalcVpConBeam(const double adMeTop[2], const double adMeBot[2], double dL);
    double CalcVnConBeam(const T_CON_SHR_PARAM& Shr);
    double CalcVcConBeam(const T_CON_SHR_PARAM& Shr);
    double CalcVpConWall(double dMp, double dHw, const T_WALL_SHR_PARAM& Shr);

    EN_EVALCTRL CalcColmCtrlType(double dMei, double dMej, double dL, double dVn);
    EN_EVALCTRL CalcBeamCtrlType(double dVp, double dVn);
    EN_EVALCTRL CalcWallCtrlType(double dMp, double dVn, double dHw, const T_WALL_SHR_PARAM& Shr);

    void GetFramMatlShear(EN_EVALSTEP enStep, const T_SECM_D& SecmD, OUT T_CON_SHR_PARAM& rShr);
	void GetFramMatlShear(const T_PDCM_D& PdcmD, const EN_EVALMATL& enMatl, OUT T_CON_SHR_PARAM& rShr);

    double CalcThetay(double dM, double dK);
    double CalcShearIndex(double dV, const T_CON_SHR_PARAM& Shr);
    double CalcShearIndexWall(double dV, const T_WALL_SHR_PARAM& Shr);
    double CalcAxialIndex(double dP, double dfck, double dAg);

    double CalcVsFram(const T_CON_SHR_PARAM& Shr);
    double CalcVsWall(const T_WALL_SHR_PARAM& WallShr, double dVc);

    double CalcK1_EVAL(const T_CON_SHR_PARAM& Shr);
    double CalcK1_KBC(const T_CON_SHR_PARAM& Shr);

    double CalcAsWall(const T_WALL_D& WallD, const T_REBW_D& RebwD);

    bool MakeBeamShearParam(T_ELEM_K ElemK, EN_EVALSTEP enStep, OUT T_BEAM_SHR_PARAM raShrPos[EN_BEAM_POS_NUM]);
    BOOL GetRebbBase(const T_REBB_D& RebbD, EN_BEAM_POS enPos, OUT T_REBB_BASE& rRebbTop, OUT T_REBB_BASE& rRebbBot);
    double CalcDeffTensBarCentroid(double dH, const T_REBB_BASE& RebbBase, double dDc);
    double CalcDeffShear(int nDeffType, double dH, const T_REBB_BASE& RebbBase, double dDc);
    double CalcAs(const T_REBB_BASE& RebbBase);

    void IsUseConColmShearBarSmid(T_ELEM_K ElemK, EN_DIR enDir, OUT BOOL rabUse[EN_MEMBEND_NUM]);
    bool MakeColmShearParamEval(T_ELEM_K ElemK, EN_EVALSTEP enStep, EN_DIR enDir, OUT T_COLM_SHR_PARAM raShr[EN_MEMBEND_NUM]);
	bool MakeColmShearParamPdgn(T_ELEM_K ElemK, EN_NLANALTYPE enAnal, EN_DIR enDir, EN_EVALMATL enMatl, OUT T_COLM_SHR_PARAM raShr[EN_MEMBEND_NUM]);
    double CalcDeffShear(int nDeffType, double dH, double dd0);
    double CalcAsvShapeSB(const T_REBC_D& RebcD, BOOL bUserMid, EN_DIR enDir);
    double CalcAsvShapeSR(const T_REBC_D& RebcD, BOOL bUserMid);

    bool MakeWallShearParam(T_ELEM_K ElemK, EN_EVALSTEP enStep, EN_DIR enDir, OUT T_WALL_SHR_PARAM raShrAvg[EN_MEMBEND_NUM], OUT T_WALL_SHR_PARAM raShrNom[EN_MEMBEND_NUM]);
    bool SetWallForcShearParam(const double adForc[2][6], EN_DIR enDirV, OUT T_WALL_SHR_PARAM raShrAvg[EN_MEMBEND_NUM], OUT T_WALL_SHR_PARAM raShrNom[EN_MEMBEND_NUM]);

    void GetWallMatlShear(EN_EVALSTEP enStep, const T_SECM_D& SecmD, bool bForcCtrl, OUT T_CON_SHR_PARAM& rShr);
	void GetWallMatlShear(const T_PDCM_D& PdcmD, OUT T_CON_SHR_PARAM& rShr);

    double CalcDutilityFactor(double dMu, double dMe);

    double CalcParamDDyFactStl(double dBTR, double dDTR, double adBTR[2], double adDTR[2], double dParamSC, double dParamNC);
    double CalcStlSlenderness(T_ELEM_K ElemK, int nPosi, const T_SECT_SECTBASE_D& SectBaseD);

    BOOL CalcWTR(const CString& strShape, const T_SECT_SECTBASE_D& SectBaseD, OUT double& rdBTR, OUT double& rdDTR);
    BOOL CalcWTRShapeL(const double adSize[], OUT double& rdBTR, OUT double& rdDTR);
    BOOL CalcWTRShapeH(const double adSize[], OUT double& rdBTR, OUT double& rdDTR);
    BOOL CalcWTRShapeT(const double adSize[], OUT double& rdBTR, OUT double& rdDTR);
    BOOL CalcWTRShapeB(const double adSize[], OUT double& rdBTR, OUT double& rdDTR);
    BOOL CalcWTRShapeP(const double adSize[], OUT double& rdBTR, OUT double& rdDTR);
    BOOL CalcWTRShape2C(const double adSize[], OUT double& rdBTR, OUT double& rdDTR);
    BOOL CalcWTRShapeSB(const double adSize[], OUT double& rdBTR, OUT double& rdDTR);
    BOOL CalcWTRShapeSR(const double adSize[], OUT double& rdBTR, OUT double& rdDTR);

    void mathUCS2GCSAsABS(OUT double& rdx, OUT double& rdy, OUT double& rdz, double ucs[3][3]); 
    UINT GetColmLocalAxisType(T_ELEM_K ElemK);
    UINT GetWallLocalAxisType(T_ELEM_K ElemK);

    double CalcDCRDisp(double dQUD, double dQCE, double dm);
    double CalcDCRForc(double dQUF, double dQCN);
    void MultiplyScaleFactorMFact(double dFactor, OUT double radMFact[3]);

    void SetFinalMe(double dMeorg, double dMeV, OUT T_EVAL1ST_COLM_DIR& rEvalDir);
    double CalcDCRM(double dMuy, double dMuz, double dMey, double dmFacy, double dMez, double dmFacz, double dAlpha);
    double CalcDCRStlColm(EN_LEVEL enLevel, const T_EVAL1ST_STLCOLM_POS& EvalPos);

    BOOL CheckConBeamForLinEval(T_ELEM_K ElemK, UINT unType, OUT T_EVAL_CONRSLT& rBeamRslt);
    BOOL CheckConColmForLinEval(T_ELEM_K ElemK, UINT unType, OUT T_EVAL_CONRSLT& rColmRslt);
    BOOL CheckConWallForLinEval(T_ELEM_K ElemK, UINT unType, OUT T_EVAL_CONRSLT& rWallRslt);

    BOOL CheckStlSectFor1stEval(T_ELEM_K ElemK, OUT T_EVAL1ST_STLRSLT& rStlRslt);
    BOOL CheckStlSectForLinEval(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_STLRSLT& rStlRslt);
    double CalcMFactModifiedByLengTerm(double dmFact, double dCb, double dRatLbLpToLrLp);
    double CalcMFactModifiedByStrnTerm(double dmFact, double dRatMpMnToMpMr);

    BOOL GetStrbAddtionStep(const T_STRB_D& RefStrbD, const T_STRB_D& NxtStrbD, double dDistRatio, OUT T_STRB_D& rStrbD);
    BOOL GetStrwAddtionStep(const T_STRW_D& RefStrwD, const T_STRW_D& NxtStrwD, double dDistRatio, OUT T_STRW_D& rStrwD);

    double GetThetay(const T_SEIS_PO_ANAL_RESULT_D& PORslt, int nPOS, int nDOF);

    void GetPORsltConBeamHingeMy(const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT double radV[2], OUT double radM[2], OUT double radDP[2], OUT double radDN[2]);
    void GetPORsltConColmHingeFyz(UINT unDirV, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT double radP[2], OUT double radV[2], OUT double radM[2], OUT double radmu[2]);
    void GetPORsltConColmHingeMyz(UINT unDirM, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT double radP[2], OUT double radV[2], OUT double radM[2], OUT double radDy[2], OUT double radM1y[2], OUT double radM1z[2]);
    void GetPORsltConWallHingeFy(const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT double radP[2], OUT double radV[2], OUT double radM[2]);
    void GetPORsltConWallHingeFz(const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT double radP[2], OUT double radV[2], OUT double radM[2], OUT double radDyV[2]);
    void GetPORsltConWallHingeMyz(UINT unDirM, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT double radP[2], OUT double radV[2], OUT double radM[2], OUT double radDyM[2]);

    void GetPORsltForc(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT double adForc[2][6], OUT double adM1y[2], OUT double adM1z[2]);
    void GetPORsltForc(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT double adForc[2][6]);
    void GetPORsltForcByInitialLoad(T_ELEM_K ElemK, OUT double adForc[2][6]);    
	void GetTHRsltForc(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, OUT double adForc[2][6]);

	BOOL GetNlghTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nDof, OUT T_NLHG_D& rNlhgDI, OUT T_NLHG_D& rNlhgDJ, OUT T_NLHG_D& rTimeDI, OUT T_NLHG_D& rTimeDJ);
	UINT GetLoadMinMaxTypeTH(T_THIS_K ThisK, int nTimeIndex);

    void CalcMFactTable2(const T_MEMBMFACT_TABLE_2& Table, double dA, double dB, OUT double radMFact[3]);
    void CalcMFactTable3(const T_MEMBMFACT_TABLE_3& Table, double dA, double dB, double dC, OUT double radMFact[3]);

    void CalcParamTable2(const T_MEMBPARAM_TABLE_2& Table, double dA, double dB, OUT double radParam[6]);
    void CalcParamTable3(const T_MEMBPARAM_TABLE_3& Table, double dA, double dB, double dC, OUT double radParam[6]);

    void CalcHingeParamConFromAB(double dThetay, double adTable[6], OUT double radParam[6]);
    void CalcHingeParamConFromDE(double dThetay, double adTable[6], OUT double radParam[6]);
    void CalcHingeParamStl(double adTable[6], OUT double radParam[6]);

    int GetColIndexMFactTable(int nSeisCompType, int nLevel);
    int GetColIndexParamTable(int nSeisCompType, int nParam);

    void CalcEval1stConColmPos(double dAlpha, OUT T_EVAL1ST_COLM_POS& rEvalPos);
    void CalcEval1stConWallPos(OUT T_EVAL1ST_WALL_POS& rEvalPos);

    void GetMn(int nPMMethod, EN_DIR enDir, const _DB_PMCV_DATA& Pmcv, const T_EVAL1ST_COLM_LCOM& EvalLcom, OUT double radMn[EN_MEMBEND_NUM]);
    double CalcMn(int nPMMethod, const _DB_PMCV_DATA& Pmcv, double dPu, double dMu);
    double CalcMnByAxial(const _DB_PMCV_DATA& Pmcv, double dPu);
    double CalcMnByEccen(const _DB_PMCV_DATA& Pmcv, double dPu, double dMu);

    void SetVpColmInfo(double dMeEnd[2], double dMe, double dL, OUT T_CONCOLM_INFO& rInfo);

	void CalConColmLengthConsiderInfillStrutPosition(T_ELEM_K ElemK, double adL[2]);

	bool CalcP1FemaTypeIehg(T_ELEM_K ElemK, int nDOF, bool bEndI, OUT double& rdP1Neg, OUT double& rdP1Pos);
		
protected:

	virtual BOOL CalcFzHingePropConBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, double dRatioMuMy, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ);
    virtual double GetDistLayerConBeam() { return 25.0; }  /// 기준에 따라 다르면 해당 기준에서 override.

	virtual BOOL CalcMyHingePropConBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, double dRatioMuMy, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ) = 0;
    virtual double CalcBeta1(double dfck) = 0;
    
    virtual BOOL CalcFxHingePropConColm(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ) = 0;
    virtual BOOL CalcFyzHingePropConColm(T_ELEM_K ElemK, EN_DIR enDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ) = 0;
    virtual BOOL CalcMyzHingePropConColm(T_ELEM_K ElemK, EN_DIR enDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ) = 0;

    virtual BOOL CalcFxHingePropConWall(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ) = 0;
    virtual BOOL CalcFyHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ) = 0;
    virtual BOOL CalcFzHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ) = 0;
    virtual BOOL CalcMyHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ) = 0;
    virtual BOOL CalcMzHingePropConWall(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ) = 0;

    virtual BOOL CalcMyHingePropStlBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ) = 0;

    virtual BOOL CalcFxHingePropStlColm(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ) = 0;	
    virtual BOOL CalcMyzHingePropStlColm(T_ELEM_K ElemK, UINT unDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ) = 0;

    virtual BOOL CalcFxHingePropStlBrac(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ) = 0;

    virtual BOOL CalcMyHingePropSrcBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ) = 0;

    virtual BOOL CalcFxHingePropSrcColm(T_ELEM_K ElemK, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ) = 0;	
    virtual BOOL CalcMyzHingePropSrcColm(T_ELEM_K ElemK, UINT unDir, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhghFemaJ) = 0;

    virtual double GetAlphaConColm(const T_SECT_D& SectD) = 0;    
    
	virtual void SetAccetanceCriteriaForcCtrl(OUT T_PHGT_FEMA& rPhgtD) = 0;
    virtual void SetAccetanceCriteriaConAxial(OUT T_PHGT_FEMA& rPhgtD) = 0;

    virtual double CalcK1(const T_COLM_SHR_PARAM& ColmShr) = 0;

    virtual double CalcVsmaxFram(const T_CON_SHR_PARAM& Shr) = 0;    

    virtual double GetMFactForc(const EN_LEVEL enLevel) = 0;
                        
private:
    bool MakeRebarSizeMap4CodeUnit();

    bool CalcAsymmetricStiffFxHingeConColm(T_ELEM_K ElemK, OUT T_PHGT_SKCV& rSkcvI, OUT T_PHGT_SKCV& rSkcvJ);
    bool CalcAsymmetricStiffFxHingeConWall(T_ELEM_K ElemK, OUT T_PHGT_SKCV& rSkcvI, OUT T_PHGT_SKCV& rSkcvJ);
    void CalcConAxialStiff(double dEs, double dAs, double dEc, double dAc, double dL, OUT T_PHGT_SKCV& rSkcv);

    void SetCurveDispCtrl(double dDispC, double dDispE, double dForcD, OUT double adCurv[2][9]);
    void SetCurveDispCtrl(double dPDispC, double dPDispE, double dPForcD, double dMDispC, double dMDispE, double dMForcD, OUT double adCurv[2][9]);	

    bool MakeWallShearParam(T_ELEM_K ElemK, EN_EVALSTEP enStep, EN_DIR enDir, bool bForcCtrl, OUT T_WALL_SHR_PARAM raShr[EN_MEMBEND_NUM]);

    double CalcVs(const T_CON_SHR_PARAM& Shr);

    EN_PMCV_MODE_SE GetPmcvModeSE(const EN_NLANALTYPE& enNLType);

protected:
    CString GetDetTextFileName();

    void SetPhgtProp(int nElemType, int nMatlType, int nDefinition, int nInterPM, OUT T_PHGT_D& rPhgtD);
    
    virtual void SetInitPhgtPropComponentCon(UINT unDOF, BOOL bDOF, int nLoc, int nSkcvType, BOOL bSymIJ, int nInitStiff, OUT T_PHGT_D& rPhgtD);
    void SetInitPhgtPropComponentStl(UINT unDOF, BOOL bDOF, int nLoc, int nSkcvType, BOOL bSymIJ, int nInitStiff, OUT T_PHGT_D& rPhgtD);
    void SetInitPhgtPropComponentInfill(UINT unDOF, BOOL bDOF, UINT unInfillType, OUT T_PHGT_D& rPhgtD);

    virtual void GetInitialHingePropCon(UINT unDOF, OUT T_PHGT_FEMA& rPhgtFEMA);
    void GetInitialHingePropStl(UINT unDOF, OUT T_PHGT_FEMA& rPhgtFEMA);
    void GetInitialHingePropInfill(UINT unDOF, OUT T_PHGT_FEMA& rPhgtFEMA);	
    void GetInitialHingeYieldSurface(OUT T_PHGT_YLDS& rPhgtYlds);

    BOOL IsBeamElemPosI(T_NODE_K NodeK, T_ELEM_K ElemK);	
    bool IsConColmConnectedInfill(T_ELEM_K ElemK);    
    double GetFactorPnmaxColm(const EN_EVALSTEP enStep, int nHoopType);
    double GetFactorPnmaxWall(const EN_EVALSTEP enStep);

    bool GetEval1stConColmForcMap(CMembCtrl* pMembCtrl, T_ELEM_K ElemK, const T_LCOM_K_LIST& aLcomK,
        OUT CMapEx<T_LCOM_K, T_LCOM_K, T_STRB_D, T_STRB_D&>& rmStrb);
    bool GetEval1stConWallForcMap(CMembCtrl* pMembCtrl, T_ELEM_K ElemK, const T_LCOM_K_LIST& aLcomK,
        OUT CMapEx<T_LCOM_K, T_LCOM_K, T_STRW_D, T_STRW_D&>& rmStrw);

    double CalcBeta(T_ELEM_K InfillK, double dVm, const T_VFRAME_LIST& aVframe);

    void CalcForc1stLcom(T_ELEM_K ElemK, CMembCtrl* pMembCtrl);

    double GetUpdateHingeVnConColm(double dVncal);
    double GetContinuousWallTotalHw(T_ELEM_K ElemK, double dHw);
	
	void SetHingeCalcParam(const T_HINGE_CALC_PARAM& Param) { m_HingeCalcParam = Param; }
    
public:        
    BOOL GetHingeMatlType(T_PHGE_K PhgeK, OUT int& nMaterial);
	BOOL GetBeamColumnHingeMatlType(T_ELEM_K ElemK, OUT int& nMaterial);
	
	BOOL GetColmAxialStrainListTHFiber(const std::set<T_ELEM_K>& stElemK, int nMatlType, T_THIS_K ThisK, int iTimeIdx, int nStepType, BOOL isRatio, OUT std::unordered_map<T_ELEM_K, std::pair<double, double>>& mapValue);
	BOOL GetWallAxialStrainListTHFiber(const std::set<T_ELEM_K>& stElemK, int nMatlType, T_THIS_K ThisK, int iTimeIdx, int nStepType, BOOL isRatio, OUT std::unordered_map<T_ELEM_K, std::pair<double, double>>& mapValue);
	BOOL GetColmAxialStrainListPOFiber(const std::set<T_ELEM_K>& stElemK, int nMatlType, T_POLC_K PolcK, int iStep, BOOL isRatio, OUT std::unordered_map<T_ELEM_K, std::pair<double, double>>& mapValue);
	BOOL GetWallAxialStrainListPOFiber(const std::set<T_ELEM_K>& stElemK, int nMatlType, T_POLC_K PolcK, int iStep, BOOL isRatio, OUT std::unordered_map<T_ELEM_K, std::pair<double, double>>& mapValue);

private:
	BOOL GetMatlLimitStrain(const int nMatl, OUT std::unordered_map<T_FIMP_K, double>& mapLimitStrain);

protected:
    CDBDoc* m_pDoc;

    T_UNIT_INDEX m_CodeUnit;
    T_UNIT_INDEX m_CurrUnit;	

	T_HINGE_CALC_PARAM m_HingeCalcParam;

private:
    CMapEx<CString, LPCTSTR, _DGN_REBAR_DATA, _DGN_REBAR_DATA&> m_mapRebarSize;	
    CColumnDividerByInfillStrut m_ColmDivByInfill;
};
#if !defined(__ISEISEVALINTERFACEIMPL_H__)
#define __ISEISEVALINTERFACEIMPL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\ISeisEvalInterface.h"

class CUnitCtrl;
class CSeisEval;
class CSeisEvalResult;
class ISeisEvalInterfaceImpl : public ISeisEvalInterface
{
public:
	ISeisEvalInterfaceImpl(void);
	virtual ~ISeisEvalInterfaceImpl(void);

public:
	virtual void SetCalcPTR() override final;
	virtual void EndCalcPTR() override final;
    
#pragma region /// INTIALIZE HINGE.

	virtual BOOL InitializeHingeConBeam(const T_HINGE_CALC_PARAM& Param, OUT T_PHGT_D& rPhgtD) override final;
	virtual BOOL InitializeHingeConColm(const T_HINGE_CALC_PARAM& Param, OUT T_PHGT_D& rPhgtD) override final;
	virtual BOOL InitializeHingeConWall(const T_HINGE_CALC_PARAM& Param, OUT T_PHGT_D& rPhgtD) override final;

	virtual BOOL InitializeHingeConBeam(const T_HINGE_CALC_PARAM& Param, OUT T_IEHP_D& rIehpD) override final;
	virtual BOOL InitializeHingeConColm(const T_HINGE_CALC_PARAM& Param, OUT T_IEHP_D& rIehpD) override final;

	virtual BOOL InitializeHingeStlBeam(const BOOL abDof[6], OUT T_PHGT_D& rPhgtD) override final;
	virtual BOOL InitializeHingeStlColm(const BOOL abDof[6], OUT T_PHGT_D& rPhgtD) override final;
	virtual BOOL InitializeHingeStlBrac(const BOOL abDof[6], OUT T_PHGT_D& rPhgtD) override final;
	virtual BOOL InitializeHingeStlTrus(const BOOL abDof[6], OUT T_PHGT_D& rPhgtD) override final;

	virtual BOOL InitializeHingeSrcBeam(const BOOL abDof[6], OUT T_PHGT_D& rEncasePhgtD, OUT T_PHGT_D& rFilledPhgtD) override final;
	virtual BOOL InitializeHingeSrcColm(const BOOL abDof[6], OUT T_PHGT_D& rEncasePhgtD, OUT T_PHGT_D& rFilledPhgtD) override final;

	virtual BOOL InitializeHingeInfill(const BOOL abDof[6], OUT T_PHGT_D& rPartPhgtD, OUT T_PHGT_D& rFullPhgtD) override final;
	virtual BOOL InitializeHingeInfill(T_ELEM_K ElemK, const BOOL abDof[6], OUT T_PHGT_D& rPhgtD) override final;
	
#pragma endregion

#pragma region /// CALCULATE HINGE PROPERTIES TO UPDATE.
	
    virtual BOOL CalcUpdateHingeConBeam(T_ELEM_K ElemK, const T_HINGE_CALC_PARAM& Param, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD) override final;
    virtual BOOL CalcUpdateHingeConColm(T_ELEM_K ElemK, const T_HINGE_CALC_PARAM& Param, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD) override final;
    virtual BOOL CalcUpdateHingeConWall(T_ELEM_K ElemK, const T_HINGE_CALC_PARAM& Param, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD) override final;

    virtual BOOL CalcUpdateHingeStlBeam(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD) override final;
    virtual BOOL CalcUpdateHingeStlColm(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, int nUpdateMatl, OUT T_PHGT_D& rPhgtD) override final;
    virtual BOOL CalcUpdateHingeStlBrac(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, int nUpdateMatl, OUT T_PHGT_D& rPhgtD) override final;
    virtual BOOL CalcUpdateHingeStlTrus(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD) override final;

    virtual BOOL CalcUpdateHingeSrcBeam(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD) override final;
    virtual BOOL CalcUpdateHingeSrcColm(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, int nUpdateMatl, OUT T_PHGT_D& rPhgtD) override final;
    
    virtual BOOL CalcUpdateHingeInfill(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_MAP& mPORslt, OUT T_PHGT_D& rPhgtD) override final;

	virtual BOOL CalcUpdateHingeConBeam(T_ELEM_K ElemK, const T_HINGE_CALC_PARAM& Param, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_IEHP_D& rIehpD) override final;
	virtual BOOL CalcUpdateHingeConColm(T_ELEM_K ElemK, const T_HINGE_CALC_PARAM& Param, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_IEHP_D& rIehpD) override final;

#pragma endregion

#pragma region /// CALCULATE PRE-EVAL RESULT.
    	
    virtual BOOL GetEvalPreConColmSimple(T_ELEM_K ElemK, OUT T_EVALPRE_COLM& rEvalPre) override final;
    virtual BOOL GetEvalPreConWallSimple(T_ELEM_K ElemK, OUT T_EVALPRE_WALL& rEvalPre) override final;    
    virtual BOOL GetEvalPreInfWallSimple(T_ELEM_K ElemK, OUT T_EVALPRE_INFL& rEvalPre) override final;

    virtual BOOL GetEvalPreConColmGeneral(T_ELEM_K ElemK, OUT T_EVALPRE_COLM& rEvalPre) override final;
    virtual BOOL GetEvalPreConWallGeneral(T_ELEM_K ElemK, OUT T_EVALPRE_WALL& rEvalPre) override final;
    
    virtual BOOL GetEvalPreStlColm(T_ELEM_K ElemK, OUT T_EVALPRE_STLCOLM& rEvalPre) override final { ASSERT(0); return FALSE; }
    virtual BOOL GetEvalPreStlBrac(T_ELEM_K ElemK, OUT T_EVALPRE_STLBRAC& rEvalPre) override final { ASSERT(0); return FALSE; }
    	
	virtual BOOL CalcEvalPreStlColmInfo(T_ELEM_K ElemK, OUT T_EVALPRE_STLCOLMINFO& rEvalInfo) override final { ASSERT(0); return FALSE; }
	virtual BOOL CalcEvalPreStlBeamInfo(T_ELEM_K ElemK, OUT T_EVALPRE_STLBEAMINFO& rEvalInfo) override final { ASSERT(0); return FALSE; }
	virtual BOOL CalcEvalPreStlPanzInfo(T_ELEM_K ElemK, OUT T_EVALPRE_STLPANZINFO& rEvalInfo) override final { ASSERT(0); return FALSE; }
    virtual BOOL CalcEvalPreStlBracInfo(T_ELEM_K ElemK, OUT T_EVALPRE_STLBRACINFO& rEvalInfo) override final { ASSERT(0); return FALSE; }

#pragma endregion

#pragma region /// CALCULATE LINEAR-EVAL RESULT.

	virtual BOOL GetSeisEvalLinRsltConBeam(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_BEAM& raEvalLin) override final;
	virtual BOOL GetSeisEvalLinRsltConColm(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_COLM& raEvalLin) override final;
	virtual BOOL GetSeisEvalLinRsltConWall(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_WALL& raEvalLin) override final;
	virtual BOOL GetSeisEvalLinRsltStlBeam(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_STLBEAM& raEvalLin) override final;
	virtual BOOL GetSeisEvalLinRsltStlColm(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_STLCOLM& raEvalLin) override final;
	virtual BOOL GetSeisEvalLinRsltStlBrac(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_STLBRAC& raEvalLin) override final;
	virtual BOOL GetSeisEvalLinRsltInfWall(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_INFILL& rEvalLin) override final;
	virtual BOOL GetSeisEvalRsltInfWallOut(T_ELEM_K ElemK, OUT T_EVAL_INFILL_OUT& raEval) override final;

#pragma endregion


#pragma region /// CALCULATE 1ST-EVAL RESULT.

	virtual BOOL GetSeisEval1stRsltConBeam(T_ELEM_K ElemK, OUT T_EVAL1ST_BEAM& raEval1st) override final;
	virtual BOOL GetSeisEval1stRsltConColm(T_ELEM_K ElemK, OUT T_EVAL1ST_COLM& raEval1st) override final;
	virtual BOOL GetSeisEval1stRsltConWall(T_ELEM_K ElemK, OUT T_EVAL1ST_WALL& raEval1st) override final;
	virtual BOOL GetSeisEval1stRsltStlBeam(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBEAM& raEval1st) override final;
	virtual BOOL GetSeisEval1stRsltStlColm(T_ELEM_K ElemK, OUT T_EVAL1ST_STLCOLM& raEval1st) override final;
	virtual BOOL GetSeisEval1stRsltStlPanz(T_ELEM_K ElemK, OUT T_EVAL1ST_STLPANZ& raEval1st) override final;
	virtual BOOL GetSeisEval1stRsltStlBrac(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBRAC& raEval1st) override final;
	virtual BOOL GetSeisEval1stRsltInfWall(T_ELEM_K ElemK, OUT T_EVAL1ST_INFILL& raEval1st) override final;
    virtual BOOL GetSeisEval1stRsltSrcBeam(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBEAM& raEval1st) override final;
    virtual BOOL GetSeisEval1stRsltSrcColm(T_ELEM_K ElemK, OUT T_EVAL1ST_STLCOLM& raEval1st) override final;

#pragma endregion


#pragma region /// CALCULATE 2ND-EVAL RESULT : PO (Pushover)

	virtual BOOL GetSeisEval2ndRsltConBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_BEAM& raEval2nd) override final;
	virtual BOOL GetSeisEval2ndRsltConColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_COLM& raEval2nd) override final;
	virtual BOOL GetSeisEval2ndRsltConWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_WALL& raEval2nd) override final;
	virtual BOOL GetSeisEval2ndRsltConColmAgbp(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd) override final;
	virtual BOOL GetSeisEval2ndRsltConWallAgwp(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd) override final;
	virtual BOOL GetSeisEval2ndRsltStlBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLBEAM& raEval2nd) override final;
	virtual BOOL GetSeisEval2ndRsltStlColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLCOLM& raEval2nd) override final;
	virtual BOOL GetSeisEval2ndRsltStlBrac(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLBRAC& raEval2nd) override final;
	virtual BOOL GetSeisEval2ndRsltInfWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_INFILL& raEval2nd) override final;

	virtual BOOL GetSeisEval2ndRsltConColmFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd) override final;
	virtual BOOL GetSeisEval2ndRsltConWallFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd) override final;

	virtual BOOL CalcSeisEval2ndRsltConBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_BEAM& raEval2nd) override final;
	virtual BOOL CalcSeisEval2ndRsltConColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_COLM& raEval2nd) override final;
	virtual BOOL CalcSeisEval2ndRsltConWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_WALL& raEval2nd) override final;
	virtual BOOL CalcSeisEval2ndRsltConColmAgbp(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd) override final;
	virtual BOOL CalcSeisEval2ndRsltConWallAgwp(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd) override final;
	virtual BOOL CalcSeisEval2ndRsltStlBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLBEAM& raEval2nd) override final;
	virtual BOOL CalcSeisEval2ndRsltStlColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLCOLM& raEval2nd) override final;
	virtual BOOL CalcSeisEval2ndRsltStlBrac(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLBRAC& raEval2nd) override final;
	virtual BOOL CalcSeisEval2ndRsltInfWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_INFILL& raEval2nd) override final;
	
	virtual BOOL CalcSeisEval2ndRsltConColmFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd) override final;
	virtual BOOL CalcSeisEval2ndRsltConWallFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd) override final;

#pragma endregion

#pragma region /// CALCULATE 2ND-EVAL RESULT : TH (Time History)

	virtual BOOL CalcSeisEval2ndRsltConBeamTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, OUT T_EVAL2ND_BEAM& raEval2nd) override final;

	virtual BOOL CalcSeisEval2ndRsltConColmTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nSerialAgxt, OUT T_EVAL2ND_COLM& raEval2nd) override final;
	virtual BOOL CalcSeisEval2ndRsltConColmFiberTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nSerialAgxt, OUT T_EVAL2ND_COLM& raEval2nd) override final;

	virtual bool ReCalcSeisEval2ndRsltConBeamLumpedTH(const T_EVAL2ND_BEAM_POS& InData, OUT T_EVAL2ND_BEAM_POS& rData) override final;
	virtual bool ReCalcSeisEval2ndRsltConColmLumpedTH(const int nDOF, const T_EVAL2ND_COLM_POS& InData, OUT T_EVAL2ND_COLM_POS& rData) override final;
	virtual bool ReCalcSeisEval2ndRsltConColmFiberTHRAngle(const T_EVAL2ND_COLM_POS& InData, OUT T_EVAL2ND_COLM_POS& rData) override final;
	virtual bool ReCalcSeisEval2ndRsltConColmFiberTHShear(const T_EVAL2ND_COLM_POS& InData, OUT T_EVAL2ND_COLM_POS& rData) override final;

	virtual BOOL CalcSeisEval2ndRsltConWallFiberTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nSerialAgxt, OUT T_EVAL2ND_WALL& raEval2nd) override final;

	virtual bool ReCalcSeisEval2ndRsltConWallFiberTHRAngle(const T_EVAL2ND_WALL_POS& InData, OUT T_EVAL2ND_WALL_POS& rData) override final;
	virtual bool ReCalcSeisEval2ndRsltConWallFiberTHShear(const T_EVAL2ND_WALL_POS& InData, OUT T_EVAL2ND_WALL_POS& rData) override final;

	virtual BOOL GetLevelSeisEval2ndConFramTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nDof, OUT UINT& runLevelI, OUT UINT& runLevelJ) override final;
	virtual BOOL GetColmAxialStrainListTHFiber(const std::set<T_ELEM_K>& stElemK, int nMatlType, T_THIS_K ThisK, int iTimeIdx, int nStepType, BOOL isRatio, OUT std::unordered_map<T_ELEM_K, std::pair<double, double>>& mapValue) override final;
	virtual BOOL GetWallAxialStrainListTHFiber(const std::set<T_ELEM_K>& stElemK, int nMatlType, T_THIS_K ThisK, int iTimeIdx, int nStepType, BOOL isRatio, OUT std::unordered_map<T_ELEM_K, std::pair<double, double>>& mapValue) override final;
	virtual BOOL GetColmAxialStrainListPOFiber(const std::set<T_ELEM_K>& stElemK, int nMatlType, T_POLC_K PolcK, int iStep, BOOL isRatio, OUT std::unordered_map<T_ELEM_K, std::pair<double, double>>& mapValue) override final;
	virtual BOOL GetWallAxialStrainListPOFiber(const std::set<T_ELEM_K>& stElemK, int nMatlType, T_POLC_K PolcK, int iStep, BOOL isRatio, OUT std::unordered_map<T_ELEM_K, std::pair<double, double>>& mapValue) override final;


#pragma endregion
		
#pragma region /// PRINT 1ST-EVAL RESULT.

	virtual BOOL PrintSeisEval1stRsltConBeam() override final;
	virtual BOOL PrintSeisEval1stRsltConColm() override final;
	virtual BOOL PrintSeisEval1stRsltConWall() override final;

	virtual BOOL PrintSeisEval1stRsltStlBeam() override final;
	virtual BOOL PrintSeisEval1stRsltStlColm() override final;
	virtual BOOL PrintSeisEval1stRsltStlPanz() override final;
	virtual BOOL PrintSeisEval1stRsltStlBrac() override final;

	virtual BOOL PrintSeisEval1stRsltSrcBeam() override final;
	virtual BOOL PrintSeisEval1stRsltSrcColm() override final;

	virtual BOOL PrintSeisEval1stRsltSrcBeam4Memb() override final;
	virtual BOOL PrintSeisEval1stRsltSrcColm4Memb() override final;	

#pragma endregion


#pragma region /// PRINT LINEARANAL-EVAL RESULT.

	virtual BOOL PrintSeisEvalLinRsltInfill() override final;

#pragma endregion


private:
	void WriteMessageToAnalMsgWindow(const CString& strTxt);
	double GetUnitFactorCode2Cur(int nUnitType);
	double GetUnitFactorCur2Code(int nUnitType);
	void   ConvertPhgt2CodeUnit(OUT T_PHGT_D& rPhgtD);
	void   ConvertUnitPhgt_Code2Curr(OUT T_PHGT_D& rPhgtD);
	void   ConvertIehp2CodeUnit(OUT T_IEHP_D& rIehpD);
	void   ConvertUnitIehp_Code2Curr(OUT T_IEHP_D& rIehpD);
	T_SEIS_PO_ANAL_RESULT_D ConvertPORslt2CodeUnit(const T_SEIS_PO_ANAL_RESULT_D& PORslt);
    CUnitCtrl* GetUnitCtrl();
    CSeisEvalResult* GetSEResult();
	
private:
	CSeisEval*   m_pCalc;

	UINT         m_unCode;
	T_UNIT_INDEX m_CurrUnit;
};

#endif //__ISEISEVALINTERFACEIMPL_H__
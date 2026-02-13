#pragma once

#include "..\wg_db\SeisEvalStruct.h"
#include "SeisEvalCalcStruct.h"

class CDBDoc;
class CSeisEvalResult
{
public:
	CSeisEvalResult(void);
	virtual ~CSeisEvalResult(void);

public:
// 	void SetCodeUnit(T_UNIT_INDEX CodeUnit) { m_CodeUnit = CodeUnit; }
// 
// 	BOOL IsEnablePreRslt()     { return m_bPreEvalResult; }
// 	BOOL IsEnable1stRslt()     { return m_b1stEvalResult; }
// 	BOOL IsEnable2ndRslt()     { return m_b2ndEvalResult; }
// 	void SetPreRslt(BOOL bSet) { m_bPreEvalResult = bSet; }
// 	void Set1stRslt(BOOL bSet) { m_b1stEvalResult = bSet; }
// 	void Set2ndRslt(BOOL bSet) { m_b2ndEvalResult = bSet; }
// 
// 	void DeleteAllResult();
// 	void DeletePreResult();
// 	void Delete1stResult();
// 	void Delete2ndResult();
// 
// #pragma region /// SET RESULT.
// 	// Pre
// 	void SetPreConColmRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVALPRE_COLM, T_EVALPRE_COLM&> &mapSrc);
// 	void SetPreConWallRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVALPRE_WALL, T_EVALPRE_WALL&> &mapSrc);
// 	void SetPreStlColmRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVALPRE_STLCOLM, T_EVALPRE_STLCOLM&> &mapSrc);
// 	void SetPreStlBracRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVALPRE_STLBRAC, T_EVALPRE_STLBRAC&> &mapSrc);
// 	// Linear
// 	void SetLinConBeamRslt(const CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_BEAM, T_EVALLIN_BEAM&> &mapSrc);
// 	void SetLinConColmRslt(const CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_COLM, T_EVALLIN_COLM&> &mapSrc);
// 	void SetLinConWallRslt(const CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_WALL, T_EVALLIN_WALL&> &mapSrc);
// 	void SetLinStlBeamRslt(const CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_STLBEAM, T_EVALLIN_STLBEAM&> &mapSrc);
// 	void SetLinStlColmRslt(const CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_STLCOLM, T_EVALLIN_STLCOLM&> &mapSrc);
// 	void SetLinStlBracRslt(const CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_STLBRAC, T_EVALLIN_STLBRAC&> &mapSrc);
// 	void SetLinInfWallRslt(const CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_INFILL, T_EVALLIN_INFILL&> &mapSrc);
//     void SetInfWallOutRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL_INFILL_OUT, T_EVAL_INFILL_OUT&> &mapSrc);
// 	// 1st
// 	void Set1stConBeamRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_BEAM, T_EVAL1ST_BEAM&> &mapSrc);
// 	void Set1stConColmRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_COLM, T_EVAL1ST_COLM&> &mapSrc);
// 	void Set1stConWallRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_WALL, T_EVAL1ST_WALL&> &mapSrc);
// 	void Set1stStlBeamRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_STLBEAM, T_EVAL1ST_STLBEAM&> &mapSrc);
// 	void Set1stStlColmRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_STLCOLM, T_EVAL1ST_STLCOLM&> &mapSrc);
// 	void Set1stStlPanzRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_STLPANZ, T_EVAL1ST_STLPANZ&> &mapSrc);
// 	void Set1stStlBracRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_STLBRAC, T_EVAL1ST_STLBRAC&> &mapSrc);
// 	void Set1stInfWallRslt(const CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_INFILL, T_EVAL1ST_INFILL&> &mapSrc);
// 	// 2nd
// 	void Set2ndConBeamRslt(const CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_BEAM_ALL, T_EVAL2ND_BEAM_ALL&> &mapSrc);
// 	void Set2ndConColmRslt(const CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_COLM_ALL, T_EVAL2ND_COLM_ALL&> &mapSrc);
// 	void Set2ndConWallRslt(const CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_WALL_ALL, T_EVAL2ND_WALL_ALL&> &mapSrc);
// 	void Set2ndStlBeamRslt(const CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_STLBEAM_ALL, T_EVAL2ND_STLBEAM_ALL&> &mapSrc);
// 	void Set2ndStlColmRslt(const CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_STLCOLM_ALL, T_EVAL2ND_STLCOLM_ALL&> &mapSrc);
// 	void Set2ndStlBracRslt(const CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_STLBRAC_ALL, T_EVAL2ND_STLBRAC_ALL&> &mapSrc);
// 	void Set2ndInfWallRslt(const CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_INFILL_ALL, T_EVAL2ND_INFILL_ALL&> &mapSrc);
// 
// #pragma endregion
// 
// #pragma region /// GET RESULT.
// 	// Pre
// 	BOOL GetPreConColmRslt(T_ELEM_K ElemK, OUT T_EVALPRE_COLM& rEvalPre);
// 	BOOL GetPreConWallRslt(T_ELEM_K ElemK, OUT T_EVALPRE_WALL& rEvalPre);
// 	BOOL GetPreStlColmRslt(T_ELEM_K ElemK, OUT T_EVALPRE_STLCOLM& rEvalPre);
// 	BOOL GetPreStlBracRslt(T_ELEM_K ElemK, OUT T_EVALPRE_STLBRAC& rEvalPre);
// 	// Linear
// 	BOOL GetLinConBeamRslt(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_BEAM& rEvalLin);
// 	BOOL GetLinConColmRslt(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_COLM& rEvalLin);
// 	BOOL GetLinConWallRslt(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_WALL& rEvalLin);
// 	BOOL GetLinStlBeamRslt(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_STLBEAM& rEvalLin);
// 	BOOL GetLinStlColmRslt(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_STLCOLM& rEvalLin);
// 	BOOL GetLinStlBracRslt(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_STLBRAC& rEvalLin);
// 	BOOL GetLinInfWallRslt(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_INFILL& rEvalLin);
// 	BOOL GetInfWallOutRslt(T_ELEM_K ElemK, OUT T_EVAL_INFILL_OUT& rEval);
// 	// 1st
// 	BOOL Get1stConBeamRslt(T_ELEM_K ElemK, OUT T_EVAL1ST_BEAM& rEval1st);
// 	BOOL Get1stConColmRslt(T_ELEM_K ElemK, OUT T_EVAL1ST_COLM& rEval1st);
// 	BOOL Get1stConWallRslt(T_ELEM_K ElemK, OUT T_EVAL1ST_WALL& rEval1st);
// 	BOOL Get1stStlBeamRslt(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBEAM& rEval1st);
// 	BOOL Get1stStlColmRslt(T_ELEM_K ElemK, OUT T_EVAL1ST_STLCOLM& rEval1st);
// 	BOOL Get1stStlPanzRslt(T_ELEM_K ElemK, OUT T_EVAL1ST_STLPANZ& rEval1st);
// 	BOOL Get1stStlBracRslt(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBRAC& rEval1st);
// 	BOOL Get1stInfWallRslt(T_ELEM_K ElemK, OUT T_EVAL1ST_INFILL& rEval1st);
// 	// 2nd
// 	BOOL Get2ndConBeamRslt(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_BEAM& rEval2nd);
// 	BOOL Get2ndConColmRslt(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_COLM& rEval2nd);
// 	BOOL Get2ndConWallRslt(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_WALL& rEval2nd);
// 	BOOL Get2ndStlBeamRslt(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLBEAM& rEval2nd);
// 	BOOL Get2ndStlColmRslt(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLCOLM& rEval2nd);
// 	BOOL Get2ndStlBracRslt(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLBRAC& rEval2nd);
// 	BOOL Get2ndInfWallRslt(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_INFILL& rEval2nd);
// 
// #pragma endregion
// 
// #pragma region /// PRINT RESULT.
// 
// 	// Lin
// 	BOOL PrintSeisEvalLinRsltInfWall();
// 	// 1st
// 	BOOL PrintSeisEval1stRsltConBeam();
// 	BOOL PrintSeisEval1stRsltConColm();
// 	BOOL PrintSeisEval1stRsltConWall();
// 	BOOL PrintSeisEval1stRsltStlBeam();
// 	BOOL PrintSeisEval1stRsltStlColm();
// 	BOOL PrintSeisEval1stRsltStlPanz();
// 	BOOL PrintSeisEval1stRsltStlBrac();
// 	BOOL PrintSeisEval1stRsltSrcBeam();
// 	BOOL PrintSeisEval1stRsltSrcColm();
	
private:
	BOOL PrintSeisEval1stRsltConColm_KISTEC2013();
	BOOL PrintSeisEval1stRsltConColm_MOE2018();
	
#pragma endregion
	

private:
	void InitResultMap();

	CString GetValueForm(int nTotalDigit, int nUnderDigit, double dValue);
	CString GetPerformLevel4TextOut(EN_LEVEL enLevel);

private:
	//CDBDoc* m_pDoc;
	T_UNIT_INDEX m_CodeUnit;
	
private:
	BOOL m_bPreEvalResult;
	BOOL m_b1stEvalResult;
	BOOL m_b2ndEvalResult;

	// Pre
	CMapEx<T_ELEM_K, T_ELEM_K, T_EVALPRE_COLM, T_EVALPRE_COLM&>               m_mPreConColm;	    //CalcEvalPreConColm()
	CMapEx<T_ELEM_K, T_ELEM_K, T_EVALPRE_WALL, T_EVALPRE_WALL&>               m_mPreConWall;      //CalcEvalPreConWall()
	CMapEx<T_ELEM_K, T_ELEM_K, T_EVALPRE_STLCOLM, T_EVALPRE_STLCOLM&>         m_mPreStlColm;      //CalcEvalPreStlColm()
	CMapEx<T_ELEM_K, T_ELEM_K, T_EVALPRE_STLBRAC, T_EVALPRE_STLBRAC&>         m_mPreStlBrac;      //CalcEvalPreStlBrac()

	// Linear
	CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_BEAM, T_EVALLIN_BEAM&>       m_mLinConBeam; //CalcSeisEvalLinRsltConBeam()
	CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_COLM, T_EVALLIN_COLM&>       m_mLinConColm; //CalcSeisEvalLinRsltConColm()
	CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_WALL, T_EVALLIN_WALL&>       m_mLinConWall; //CalcSeisEvalLinRsltConWall()
	CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_STLBEAM, T_EVALLIN_STLBEAM&> m_mLinStlBeam; //CalcSeisEvalLinRsltStlBeam()
	CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_STLCOLM, T_EVALLIN_STLCOLM&> m_mLinStlColm; //CalcSeisEvalLinRsltStlColm()
	CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_STLBRAC, T_EVALLIN_STLBRAC&> m_mLinStlBrac; //CalcSeisEvalLinRsltStlBrac()
	CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_INFILL, T_EVALLIN_INFILL&>   m_mLinInfWall; //CalcSeisEvalLinRsltInfill()
	CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL_INFILL_OUT, T_EVAL_INFILL_OUT&>                 m_mInfWallOut;

	// 1st
	CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_BEAM, T_EVAL1ST_BEAM&>       m_m1stConBeam; //CalcSeisEval1stRsltConBeam()
	CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_COLM, T_EVAL1ST_COLM&>       m_m1stConColm; //CalcSeisEval1stRsltConColm()
	CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_WALL, T_EVAL1ST_WALL&>       m_m1stConWall; //CalcSeisEval1stRsltConWall()
	CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_STLBEAM, T_EVAL1ST_STLBEAM&> m_m1stStlBeam; //CalcSeisEval1stRsltStlBeam()
	CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_STLCOLM, T_EVAL1ST_STLCOLM&> m_m1stStlColm; //CalcSeisEval1stRsltStlColm()
	CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_STLPANZ, T_EVAL1ST_STLPANZ&> m_m1stStlPanz; //CalcSeisEval1stRsltStlPanz()
	CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_STLBRAC, T_EVAL1ST_STLBRAC&> m_m1stStlBrac; //CalcSeisEval1stRsltStlBrac()
	CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_INFILL, T_EVAL1ST_INFILL&>   m_m1stInfWall; //CalcSeisEval1stRsltInfill()

	// 2nd
	CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_BEAM_ALL, T_EVAL2ND_BEAM_ALL&>       m_m2ndConBeam; //CalcSeisEval2ndRsltConBeam()
	CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_COLM_ALL, T_EVAL2ND_COLM_ALL&>       m_m2ndConColm; //CalcSeisEval2ndRsltConColm()
	CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_WALL_ALL, T_EVAL2ND_WALL_ALL&>       m_m2ndConWall; //CalcSeisEval2ndRsltConWall()
	CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_STLBEAM_ALL, T_EVAL2ND_STLBEAM_ALL&> m_m2ndStlBeam; //CalcSeisEval2ndRsltStlBeam()
	CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_STLCOLM_ALL, T_EVAL2ND_STLCOLM_ALL&> m_m2ndStlColm; //CalcSeisEval2ndRsltStlColm()
	CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_STLBRAC_ALL, T_EVAL2ND_STLBRAC_ALL&> m_m2ndStlBrac; //CalcSeisEval2ndRsltStlBrac()
	CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_INFILL_ALL, T_EVAL2ND_INFILL_ALL&>   m_m2ndInfWall; //CalcSeisEval2ndRsltInfill()

	
	
	
	
};


#pragma once

#include "SeisEvalTextOut.h"

struct T_CONBEAM_INFO;
struct T_EVAL1ST_COLM_DIR;
struct T_EVAL1ST_WALL_POS;
struct T_UPHG_CONBEAM_POS;
struct T_UPHG_CONCOLM_DIR;
struct T_UPHG_PARAM;
struct T_PMCV_TABLE;

class CSeisEvalTextOutKISTEC2019 : public CSeisEvalTextOut
{
public:
    CSeisEvalTextOutKISTEC2019(void);
    virtual ~CSeisEvalTextOutKISTEC2019(void);

protected:
    virtual void PrintEval1stConBeamLcom(T_ELEM_K ElemK, const T_EVAL1ST_BEAM_LCOM& EvalLcom) override final;
    virtual void PrintEval1stConColmLcom(T_ELEM_K ElemK, const T_EVAL1ST_COLM_LCOM& EvalLcom, const T_PMCV_TABLE& Pmcvy, const T_PMCV_TABLE& Pmcvz) override final;
    virtual void PrintEval1stConWallLcom(T_ELEM_K ElemK, const T_EVAL1ST_WALL_LCOM& EvalLcom, const T_PMCV_TABLE& PmcvyAvg, const T_PMCV_TABLE& PmcvyNom) override final;

    virtual void PrintUphgConBeam(T_ELEM_K ElemK, const T_UPHG_CONBEAM& Uphg) override final;
    virtual void PrintUphgConColm(T_ELEM_K ElemK, const T_UPHG_CONCOLM& Uphg) override final;
    virtual void PrintUphgConWall(T_ELEM_K ElemK, const T_UPHG_CONWALL& Uphg) override final;

    virtual void PrintConWallCtrlAction(const T_CONWALL_INFO& Info, const CString& strComment) override;
    
private:
    void PrintEval1stConBeamLcomPos(T_ELEM_K ElemK, T_LCOM_K LcomK, const T_EVAL1ST_BEAM_POS& EvalPos);
    void PrintEval1stConColmLcomPos(T_ELEM_K ElemK, T_LCOM_K LcomK, const T_EVAL1ST_COLM_POS& EvalPos, const T_PMCV_TABLE& Pmcvy, const T_PMCV_TABLE& Pmcvz);
    void PrintEval1stConWallLcomPos(T_ELEM_K ElemK, T_LCOM_K LcomK, const T_EVAL1ST_WALL_POS& EvalPos, const T_PMCV_TABLE& Pmcvy);

    void PrintUphgConBeamPos(T_ELEM_K ElemK, const T_UPHG_CONBEAM_POS& UphgPos);
    void PrintUphgConColmAxial(T_ELEM_K ElemK, const T_UPHG_PARAM& ParamComp, const T_UPHG_PARAM& ParamTens);
    void PrintUphgConColmDir(T_ELEM_K ElemK, EN_DIR enDir, const T_UPHG_CONCOLM_DIR& UphgDir);
    void PrintUphgConWallPosFlex(T_ELEM_K ElemK, const T_UPHG_CONWALL_POS& UphgPos);
    void PrintUphgConWallPosSher(T_ELEM_K ElemK, const T_UPHG_CONWALL_POS& UphgPos);
    
private:
    void PrintConColmMFactAxial(T_ELEM_K ElemK, const T_EVAL1ST_COLM_POS& Pos);
    void PrintConColmMFact(T_ELEM_K ElemK, EN_DIR enDir, const T_EVAL1ST_COLM_DIR& EvalDir);
    void PrintConWallMFact(T_ELEM_K ElemK, const T_EVAL1ST_WALL_POS& Pos);
    void PrintConBeamMFact(T_ELEM_K ElemK, const T_EVAL1ST_BEAM_POS& Pos);    
    
    void PrintConBeamStrength(const T_CONBEAM_INFO& Info);
    void PrintConColmStrength(const T_EVAL1ST_COLM_POS& Pos, const T_PMCV_TABLE& Pmcvy, const T_PMCV_TABLE& Pmcvz, int nShrMethod);
    void PrintConWallStrength(const T_EVAL1ST_WALL_POS& Pos, const T_PMCV_TABLE& Pmcvy);

    void PrintConWallForcAction(const T_CONWALL_INFO& Info, const CString& strComment);
};


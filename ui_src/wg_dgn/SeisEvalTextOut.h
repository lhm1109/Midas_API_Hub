#pragma once

#include "DgnTextOut.h"

struct T_CONBEAM_FLEX;
struct T_CONBEAM_INFO;
struct T_CONCOLM_INFO;
struct T_CONWALL_INFO;
struct T_EVAL1ST_DCR;
struct T_PMCV_TABLE;

struct T_EVAL1ST_BEAM;
struct T_EVAL1ST_BEAM_LCOM;
struct T_EVAL1ST_BEAM_POS;

struct T_EVAL1ST_COLM;
struct T_EVAL1ST_COLM_LCOM;
struct T_EVAL1ST_COLM_POS;
struct T_EVAL1ST_COLM_DIR;

struct T_EVAL1ST_WALL;
struct T_EVAL1ST_WALL_LCOM;
struct T_EVAL1ST_WALL_POS;

struct T_UPHG_CONBEAM;
struct T_UPHG_CONBEAM_POS;

struct T_UPHG_CONCOLM;
struct T_UPHG_CONCOLM_POS;
struct T_UPHG_CONCOLM_DIR;

struct T_UPHG_CONWALL;
struct T_UPHG_CONWALL_POS;

enum EN_DIR;
enum EN_LEVEL;
enum EN_EVALSTEP;
enum EN_EVALCTRL;

class CSeisEvalResult;
class CSeisEvalTextOut : public CDgnTextOut
{
public:
    CSeisEvalTextOut(void);
    virtual ~CSeisEvalTextOut(void);

public:
    bool InitPrintSetting(const CString& strIdentifier);
            
    bool PrintSeisEval1stCon(const T_ELEM_K_LIST& aElemK, T_LCOM_K LcomK);
    bool PrintSeisEval1stStl(const T_ELEM_K_LIST& aElemK, T_LCOM_K LcomK);
    bool PrintSeisEval1stMas(const T_ELEM_K_LIST& aElemK, T_LCOM_K LcomK);

    bool PrintSeisEvalUphgCon(const T_ELEM_K_LIST& aElemK);
    bool PrintSeisEvalUphgStl(const T_ELEM_K_LIST& aElemK);
    bool PrintSeisEvalUphgMas(const T_ELEM_K_LIST& aElemK);
   
protected:    
    virtual void PrintEval1stConBeamLcom(T_ELEM_K ElemK, const T_EVAL1ST_BEAM_LCOM& EvalLcom) = 0;    
    virtual void PrintEval1stConColmLcom(T_ELEM_K ElemK, const T_EVAL1ST_COLM_LCOM& EvalLcom, const T_PMCV_TABLE& Pmcvy, const T_PMCV_TABLE& Pmcvz) = 0;
    virtual void PrintEval1stConWallLcom(T_ELEM_K ElemK, const T_EVAL1ST_WALL_LCOM& EvalLcom, const T_PMCV_TABLE& PmcvyAvg, const T_PMCV_TABLE& PmcvyNom) = 0;

    virtual void PrintUphgConBeam(T_ELEM_K ElemK, const T_UPHG_CONBEAM& Uphg) = 0;
    virtual void PrintUphgConColm(T_ELEM_K ElemK, const T_UPHG_CONCOLM& Uphg) = 0;
    virtual void PrintUphgConWall(T_ELEM_K ElemK, const T_UPHG_CONWALL& Uphg) = 0;

protected:

#pragma region /// print concrete beam

    virtual void PrintConBeamInfo(T_ELEM_K ElemK, EN_EVALSTEP enStep);
    virtual void PrintConBeamReinf(T_ELEM_K ElemK);

    virtual void PrintConBeamForce(T_LCOM_K LcomK, const T_EVAL1ST_BEAM_POS& EvalPos);
    virtual void PrintConBeamStrengthFlex(const T_CONBEAM_FLEX& Flex);
    virtual void PrintConBeamStrengthShear(const T_CONBEAM_INFO& Info);
    virtual void PrintConBeamCtrlAction(const T_CONBEAM_INFO& Info);
    virtual void PrintConBeamLongReinf(const T_CONBEAM_INFO& Info, double dMu);
    virtual void PrintConBeamTranReinf(const T_CONBEAM_INFO& Info, double dVu, bool bUseSeisHook, const CString& strComment);
    virtual void PrintConBeamShearIdx(const T_CONBEAM_INFO& Info, const CString& strComment);
    
    virtual void PrintConBeamPerformLevel(const T_EVAL1ST_BEAM_POS& Pos);

    virtual void PrintConBeamForce(const T_UPHG_CONBEAM_POS& Pos);        

#pragma endregion

#pragma region /// print concrete column

    virtual void PrintConColmInfo(T_ELEM_K ElemK, EN_EVALSTEP enStep);
    virtual void PrintConColmReinf(T_ELEM_K ElemK, bool bRectShape);

    virtual void PrintConColmStrengthAxial(const T_EVAL1ST_COLM_POS& EvalPos);
    virtual void PrintConColmStrengthShear(int nShearMethod, const T_EVAL1ST_COLM_DIR& EvalPos, const CString& strSymDir, const CString& strComment);
    virtual void PrintConColmStrengthShearMethodEval(const CString& strSymDir, const CString& strComment);
    virtual void PrintConColmStrengthShearMethodKBC(double dPu, const CString& strSymDir, const CString& strComment);

    virtual void PrintConColmForce(T_LCOM_K LcomK, const T_EVAL1ST_COLM_POS& EvalPos);
    virtual void PrintConColmCalcParam(const T_CONCOLM_INFO& Info);
    virtual void PrintConColmTranReinfDetail(EN_EVALSTEP enStep, const T_CONCOLM_INFO& Info, const CString& strComment);
    virtual void PrintConColmFailureMode(const EN_DIR& enDir, const T_CONCOLM_INFO& Info, const CString& strComment);
    virtual void PrintConColmAxialIdx(const T_CONCOLM_INFO& Info, const CString& strComment);
    virtual void PrintConColmTranReinfRatio(const T_CONCOLM_INFO& Info, const CString& strComment);
    virtual void PrintConColmShearIdx(const T_CONCOLM_INFO& Info, const CString& strSymDir, const CString& strComment);
        
    virtual void PrintConColmPerformLevel(const T_EVAL1ST_COLM_POS& Pos);

    virtual void PrintConColmForce(const T_UPHG_CONCOLM_DIR& Dir);

#pragma endregion
    
#pragma region /// print concrete wall

    virtual void PrintConWallInfo(T_ELEM_K ElemK, EN_EVALSTEP enStep);
    virtual void PrintConWallReinf(T_ELEM_K ElemK);
        
    virtual void PrintConWallStrengthShear(const T_EVAL1ST_WALL_POS& EvalPos);

    virtual void PrintConWallForce(T_LCOM_K LcomK, const T_EVAL1ST_WALL_POS& EvalPos);
    virtual void PrintConWallCtrlAction(const T_CONWALL_INFO& Info, const CString& strComment);
    virtual void PrintConWallAxialIdx(const T_CONWALL_INFO& Info, const CString& strComment);
    virtual void PrintConWallShearIdx(const T_CONWALL_INFO& Info, const CString& strComment);

    virtual void PrintConWallPerformLevel(const T_EVAL1ST_WALL_POS& Pos);

    virtual void PrintConWallForce(const T_UPHG_CONWALL_POS& Pos);

#pragma endregion

    virtual void PrintTitle(const CString& strRprtTitle, const CString& strSEMethod);

    virtual void PrintLcomName1stEval(const T_LCOM_K LcomK);

    virtual void PrintFlexStrengthByPmcvAvg(const T_PMCV_TABLE& Pmcv, double dMe, const CString& strDir = _T(""));
    virtual void PrintFlexStrengthByPmcvNom(const T_PMCV_TABLE& Pmcv, double dMn, const CString& strDir = _T(""));
    
    virtual void PrintMFact(const double adMFact[3]);
    virtual void PrintMFactCalc(const double adMFact[3], const CString& strComment);
    virtual void PrintMFactUser(const double adMFact[3]);

    virtual void PrintMFactDir(const double adMFact[3], EN_DIR enDir);
    virtual void PrintMFactCalcDir(const double adMFact[3], EN_DIR enDir, const CString& strComment);
    virtual void PrintMFactUserDir(const double adMFact[3], EN_DIR enDir);

    virtual void PrintDCRAxialForcAction(const T_EVAL1ST_DCR& Dcr);
    virtual void PrintDCRAxialDispAction(const T_EVAL1ST_DCR& Dcr);
    
    virtual void PrintDCRFlex(const T_EVAL1ST_DCR& Dcr);
    virtual void PrintDCRFlexDir(const T_EVAL1ST_DCR& Dcr, EN_DIR enDir);
    virtual void PrintDCRFlexForcActionDir(const T_EVAL1ST_DCR& Dcr, EN_DIR enDir);
    virtual void PrintDCRFlexComb(const T_EVAL1ST_DCR& Dcr);

    virtual void PrintDCRShearForcAction(const T_EVAL1ST_DCR& Dcr);
    virtual void PrintDCRShearForcActionDir(const T_EVAL1ST_DCR& Dcr, EN_DIR enDir);
    virtual void PrintDCRShearDispActionDir(const T_EVAL1ST_DCR& Dcr, EN_DIR enDir);
    
    virtual void PrintConMatlInfo(EN_EVALSTEP enStep, const CString& strMatlName, const T_SECM_D& SecmD); 
    virtual void PrintConHingeParamFromAB(const double adTableP[6], const double adTableN[6]);
    virtual void PrintConHingeParamFromDE(const double adTableP[6], const double adTableN[6]);
    virtual void PrintConHingeParamFromAB(const double adTable[6]);    
    virtual void PrintConHingeParamFromDE(const double adTable[6]);
    
    virtual void PrintConHingeCurveFromAB(const double adParamP[6], const double adParamN[6]);
    virtual void PrintConHingeCurveFromDE(const double adParamP[6], const double adParamN[6]);
    virtual void PrintConHingeCurveFromAB(const double adParam[6], const CString& strComp = _T(""));
    virtual void PrintConHingeCurveFromDE(const double adParam[6], const CString& strComp = _T(""));
    
    virtual void PrintConHingeAcceptCriteriaFromAB(const double adParam[6], const CString& strComp = _T(""));
    virtual void PrintConHingeAcceptCriteriaFromDE(const double adParam[6], const CString& strComp = _T(""));
    virtual void PrintConHingeAcceptCriteriaFromAB(const double adParamP[6], const double adParamN[6]);
    virtual void PrintConHingeAcceptCriteriaFromDE(const double adParamP[6], const double adParamN[6]);

	virtual void PrintConCtrlAction(double dVp, double dVn, EN_EVALCTRL enCtrlType);
    virtual void PrintPerformLevel(EN_LEVEL enLevel);
    virtual void PrintFinalPerformLevel(EN_LEVEL enLevel);
            
    virtual CString GetSeisEvalMethod();
    virtual CString GetSeisConform(BOOL bSeisConform);
    virtual CString GetSeisDetailType(UINT unSeisDetType);
    virtual CString GetSeisCompType(D_SCMT_TYPE unScmtType);
    virtual CString GetShearStrengthMethod(int nShearType);
    virtual CString GetShearEffDepth(int nEffDepth);
    virtual CString GetPMCalcType(int nPMCalcType);

    virtual CString GetLocalDir(EN_DIR enDir);
    virtual CString GetLocalDirSymbol(EN_DIR enDir);
    virtual CString GetPolcName(T_POLC_K PolcK);
    virtual CString GetYesNo(bool bYes);
    
    virtual int GetReinfSpace2MM(double dSpace);
                
private:	    
    CString GetStringDCR(double dDCR);

    void PrintPmcvTable(const T_PMCV_TABLE& Pmcv);

protected:
    CSeisEvalResult* GetResultPtr();
};


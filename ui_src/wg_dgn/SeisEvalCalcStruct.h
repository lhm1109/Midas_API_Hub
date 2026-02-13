#if !defined(__SEISEVALCALCSTRUCT_H__)
#define __SEISEVALCALCSTRUCT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\SeisEvalStruct.h"

#define SE_ELEM_TYPE_KEY unsigned int
union SE_ELEM_TYPE_K
{
    SE_ELEM_TYPE_KEY KeyMap;
    struct
    {
        unsigned int ElemK : 24; // Element Key. (0~16,777,216)
        unsigned int TypeK :  8; // Any Type.    (0~255)
    }Key;
};

#define SE_ELEM_LOAD_KEY unsigned int
union SE_ELEM_LOAD_K
{
    SE_ELEM_LOAD_KEY KeyMap;
    struct
    {
        unsigned int ElemK : 24; // Element Key.            (0~16,777,216)
        unsigned int PolcK :  8; // Pushover Load Case Key. (0~255)
    }Key;
};

enum EN_BEAM_POS
{
    EN_BEAM_POS_I = 0,
    EN_BEAM_POS_M, 
    EN_BEAM_POS_J,
    EN_BEAM_POS_NUM,
};

enum class EN_VAL_STATUS
{
    NONE = 0,
    LE,
    GE,
    EQ
};

enum EN_MFACT_IDX
{
    EN_MFAC_IO = 0,
    EN_MFAC_LS_1ST,
    EN_MFAC_CP_1ST,
    EN_MFAC_LS_2ND,
    EN_MFAC_CP_2ND,
};

struct T_SEIS_ITEM
{
    double dSDS;
    double dSD1;
    double dIe; 

    void Initialize()
    {
        dSDS = 0.0;
        dSD1 = 0.0;
        dIe  = 0.0;
    }
};

struct T_VFRAME
{
    T_ELEM_K ElemK;  /// Column or Wall.
    double dVny;
    double dVnz;

    void Initialize();
    T_VFRAME();
    T_VFRAME(T_ELEM_K Key, double dVy, double dVz);
    T_VFRAME(const T_VFRAME& src) { *this = src; }
    T_VFRAME& operator= (const T_VFRAME& src)
    {
        ElemK = src.ElemK;
        dVny  = src.dVny;
        dVnz  = src.dVnz;
        return *this;
    }
};

typedef CArray<T_VFRAME, T_VFRAME&> T_VFRAME_LIST;

struct T_CON_SHR_PARAM
{
    double dFck;
    double dFys;
    double dLambda;
    double dBv;
    double dDv;
    double dAsv;
    double dSv;

    T_CON_SHR_PARAM();
    void Initialize();
    double CalcRhoIndex() const;
    bool IsSpaceGT05Dv() const;
};

struct T_BEAM_SHR_PARAM
{
    T_CON_SHR_PARAM ParamPos;
    T_CON_SHR_PARAM ParamNeg;
	int nShrMethod;

    void Initialize();
    T_CON_SHR_PARAM GetParam(BOOL bConsiderM, double dMu=0.0) const; /// 보의 전단유효깊이는 대상하중조합의 모멘트 부호에 따라 결정 (same to 설계 @선형해석평가)
};

struct T_COLM_SHR_PARAM
{
    T_CON_SHR_PARAM Comm;
    int nShrMethod;
    BOOL bAutoK1;
    double dUserK1;    
    double dAg;
    double dM;
    double dV;
    double dP;

    void Initialize();    
};

struct T_WALL_SHR_PARAM
{
    T_CON_SHR_PARAM Comm;
    bool bVnmaxEQ;
    double dTotalHw;
    double dLw;
    double dP;
    double dV;   
    double dM;

    T_WALL_SHR_PARAM();
    void Initialize();
    void SetForc(double dPu, double dVu, double dMu);
};

struct T_MEMBMFACT_TABLE_2
{
    double dA1;
    double dA2;
    double dB1;
    double dB2;

    double adTable_A1_B1[3];
    double adTable_A1_B2[3];
    double adTable_A2_B1[3];
    double adTable_A2_B2[3];

    void Initialize();
};

struct T_MEMBMFACT_TABLE_3
{
    double dA1;
    double dA2;
    double dB1;
    double dB2;
    double dC1;
    double dC2;

    double adTable_A1_B1_C1[3];
    double adTable_A1_B1_C2[3];
    double adTable_A1_B2_C1[3];
    double adTable_A1_B2_C2[3];
    double adTable_A2_B1_C1[3];
    double adTable_A2_B1_C2[3];
    double adTable_A2_B2_C1[3];
    double adTable_A2_B2_C2[3];

    void Initialize();
};

struct T_MEMBPARAM_TABLE_2
{
    double dA1;
    double dA2;
    double dB1;
    double dB2;

    double adTable_A1_B1[6];
    double adTable_A1_B2[6];
    double adTable_A2_B1[6];
    double adTable_A2_B2[6];

    void Initialize();
};

struct T_MEMBPARAM_TABLE_3
{
    double dA1;
    double dA2;
    double dB1;
    double dB2;
    double dC1;
    double dC2;

    double adTable_A1_B1_C1[6];
    double adTable_A1_B1_C2[6];
    double adTable_A1_B2_C1[6];
    double adTable_A1_B2_C2[6];
    double adTable_A2_B1_C1[6];
    double adTable_A2_B1_C2[6];
    double adTable_A2_B2_C1[6];
    double adTable_A2_B2_C2[6];

    void Initialize();
};

#endif // !defined(__SEISEVALCALCSTRUCT_H__)
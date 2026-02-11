#ifndef __DB_ST_DN_RESULT_CSG_H__
#define __DB_ST_DN_RESULT_CSG_H__

#define MIDAS_SDK

#include "DB_ST_DT_UNIT.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(1)

///////////////////////////////////////////////////////////////
// Composite Steel Girder Design
///////////////////////////////////////////////////////////////

#define D_CSG_KSCE_LSD12_CHECK_ITEM  13
#define D_CSG_FORCE_COMPONENT 9
#define D_CSG_FORCE_CONCURRENT_7DOF  18

#define D_NR_RAIL_ROUTE_GRADE_RA1   1
#define D_NR_RAIL_ROUTE_GRADE_RA2   2
#define D_NR_RAIL_ROUTE_GRADE_RA3   3
#define D_NR_RAIL_ROUTE_GRADE_RA4   4
#define D_NR_RAIL_ROUTE_GRADE_RA5   5
#define D_NR_RAIL_ROUTE_GRADE_RA6   6
#define D_NR_RAIL_ROUTE_GRADE_RA7   7
#define D_NR_RAIL_ROUTE_GRADE_RA8   8
#define D_NR_RAIL_ROUTE_GRADE_RA9   9
#define D_NR_RAIL_ROUTE_GRADE_RA10  10
#define D_NR_RAIL_ROUTE_GRADE_RA11  11
#define D_NR_RAIL_ROUTE_GRADE_RA12  12
#define D_NR_RAIL_ROUTE_GRADE_RA13  13
#define D_NR_RAIL_ROUTE_GRADE_RA14  14
#define D_NR_RAIL_ROUTE_GRADE_RA15  15

#define D_NR_RAIL_ASSESSED_CATEGORY_A1   1
#define D_NR_RAIL_ASSESSED_CATEGORY_A2   2
#define D_NR_RAIL_ASSESSED_CATEGORY_A3   3
#define D_NR_RAIL_ASSESSED_CATEGORY_B    20
#define D_NR_RAIL_ASSESSED_CATEGORY_C    30
#define D_NR_RAIL_ASSESSED_CATEGORY_D    40
#define D_NR_RAIL_ASSESSED_CATEGORY_E    50
#define D_NR_RAIL_ASSESSED_CATEGORY_F    60

enum enNR_TS_Check
{
    EN_NR_TS_None = 0,
	EN_NR_TS_Web_Yielding,
    EN_NR_TS_Yielding,
    EN_NR_TS_Buckling,
};

enum enSupportType
{
	EN_Support_None = 0,
	EN_Support_End,
	EN_Support_Interior,
};


struct T_SGCI_D
{
    BOOL bCsgCheck[D_CSG_KSCE_LSD12_CHECK_ITEM];
    
    void Initialize()
    {
        memset(bCsgCheck,0,sizeof(bCsgCheck));
    }

    T_SGCI_D() { Initialize(); }
};

struct T_CSGD_FORCE
{
    double dForce[D_CSG_FORCE_COMPONENT];

    void Initialize()
    {
        memset(dForce,0,sizeof(dForce));
    }
    T_CSGD_FORCE() {Initialize();}
    void SetForce(double dFx, double dFy, double dFz, double dMx, double dMy, double dMz)
    {
        dForce[0] = dFx;
        dForce[1] = dFy;
        dForce[2] = dFz;
        dForce[3] = dMx;
        dForce[4] = dMy;
        dForce[5] = dMz;
    }

    void SetForce(double dFx, double dFy, double dFz, double dMx, double dMy, double dMz, double dMbi, double dTst, double dTwp)
    {
        SetForce(dFx, dFy, dFz, dMx, dMy, dMz);
        dForce[6] = dMbi;
        dForce[7] = dTst;
        dForce[8] = dTwp;
    }

    double GetFx()  const { return dForce[0]; }
    double GetFy()  const { return dForce[1]; }
    double GetFz()  const { return dForce[2]; }
    double GetMx()  const { return dForce[3]; }
    double GetMy()  const { return dForce[4]; }
    double GetMz()  const { return dForce[5]; }
    double GetMbi() const { return dForce[6]; }
    double GetTst() const { return dForce[7]; }
    double GetTwp() const { return dForce[8]; }
};

struct T_CSGD_UNIT
{
    T_CSGD_UNIT()
    {
        dForce[0] = D_UNITSYS_BASE_FORCE;       
        dForce[1] = D_UNITSYS_BASE_FORCE;   
        dForce[2] = D_UNITSYS_BASE_FORCE;       
        dForce[3] = D_UNITSYS_BASE_MOMENT;   
        dForce[4] = D_UNITSYS_BASE_MOMENT;       
        dForce[5] = D_UNITSYS_BASE_MOMENT;
        dForce[6] = D_UNITSYS_BASE_DISTSTIFF;
        dForce[7] = D_UNITSYS_BASE_DISTSTIFF;
        dForce[8] = D_UNITSYS_BASE_DISTSTIFF;
    }
    int dForce[D_CSG_FORCE_COMPONENT];
};


struct T_CSGD_COMF
{
	double dLFRatio;     //AASHTO 하중유발피로 출력 위한 변수       
    int nFatigueI;       //AASHTO Fatigue 1,2를 저장하기 위한 변수
    T_CSGD_FORCE ForNC;  //비합성
    T_CSGD_FORCE ForLT;  //단기
    T_CSGD_FORCE ForST;  //장기
    T_CSGD_FORCE ForLC;  //sum
    T_CSGD_FORCE ForMV;
    T_CSGD_FORCE ForSV;
    
    void Initialize()
    {
		dLFRatio = 0.0;
        nFatigueI = 0;
        ForNC.Initialize();
        ForLT.Initialize();
        ForST.Initialize();
        ForLC.Initialize();
		ForMV.Initialize();
        ForSV.Initialize();
    }

    T_CSGD_COMF() {Initialize();}  
};

struct T_CSGD_COMF_MV
{
    int nLcomSize;
    T_CSGD_COMF ForMV[D_CSG_FORCE_CONCURRENT_7DOF];

    void Initialize()
    {
        nLcomSize = 1;
        for (int i=0; i<D_CSG_FORCE_CONCURRENT_7DOF; ++i) ForMV[i].Initialize();
    }
    T_CSGD_COMF_MV() {Initialize();}
};

struct T_CSGF_MEMB
{
    T_CSGD_COMF  MembFor[3]; //0=I, 1=M, 2=J
	T_CSGD_COMF  MembFor13[2]; //0=1/4, 1=3/4
    T_CSGD_COMF  ChkFor;
    T_CSGD_COMF  MyMaxFor;
    T_CSGD_FORCE MVFor;     // Special
    T_CSGD_FORCE MVForStd;  // Standard
    T_ELEM_K     ElemK4Mymax;
    T_SECT_K     SectK4Mymax;
    int          nPos4Mymax;

    void Initialize()
    {
        for ( int i=0; i<3; ++i )
        {
            MembFor[i].Initialize();
        }
        for ( int i=0; i<2; ++i )
        {
            MembFor13[i].Initialize();
        }
        ChkFor.Initialize();
        MyMaxFor.Initialize();
        MVFor.Initialize();
        MVForStd.Initialize();
        ElemK4Mymax = 0;
        SectK4Mymax = 0;
        nPos4Mymax  = 0;
    }
    T_CSGF_MEMB() { Initialize(); }

    void SetMyMaxForce(T_ELEM_K ElemK, T_SECT_K SectK, int nPos, const T_CSGD_COMF& MyFor)
    {
		ElemK4Mymax = ElemK;
		SectK4Mymax = SectK;
        nPos4Mymax = nPos;
        MyMaxFor = MyFor;
    }
};

struct T_CSGD_ULMF
{
    T_CSGD_FORCE For[3]; // I,M,J
    T_CSGD_FORCE CbFor[3]; // I,M,J
    T_CSGD_FORCE For13[2]; // 1/4, 3/4
    T_CSGD_FORCE MmaxFor; // 최대모멘트

    void Initialize()
    {
        for (int i=0; i<3; ++i)
        {
            For[i].Initialize();
            CbFor[i].Initialize();
        }
        for (int i=0; i<2; ++i)
        {
            For13[i].Initialize();
        }
        MmaxFor.Initialize();
    }
    T_CSGD_ULMF() {Initialize();}
};

struct T_CSGD_ULMF_MV
{
    int nLcomSize;
    T_CSGD_ULMF UlmfMV[D_CSG_FORCE_CONCURRENT_7DOF];
    void Initialize()
    {
        nLcomSize = 1;
        for (int i=0; i<D_CSG_FORCE_CONCURRENT_7DOF; ++i) UlmfMV[i];
    }

    T_CSGD_ULMF_MV() {Initialize();}
};
//////////////////////////////////////////////////////////////////////////
// 시공단계 

// 시공단계 휨검토
#define T_SCBR_K unsigned int
struct T_SCBR_BASE 
{
    BOOL bChk;
    UINT nStageK;     // Stage              [Unit=None]
    UINT nStepK;      // Step               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]

    T_CSGD_ULMF  UlFor;
    T_CSGD_FORCE CsFor;

    double dfcw;       //  복부판 응력           (fcw < fcw_lim  OK)
    double dfcw_lim;   //  복부판의 공칭 휨저항
    double dfcf;       //  압축플랜지 응력       (fcf < fcf_lim  OK)
    double dfcf_lim;   //  압축플랜지 응력제한
    double dftf;       //  인장플랜지 응력       (ftf < ftf_lim  OK)
    double dftf_lim;   //  인장플랜지 응력제한
    double dfdeck;  // deck응력   
    double dphifr;  // deck응력 제한
	int nSectType;  //0: I형, 1: Box 2:Tub
	double dFactorCS;  //[none]
    double dflc;
    double dflt;

    double dRatio;
    int  nOK;

    void Initialize()
    {
        bChk = FALSE;
        nStageK = 0;
        nStepK  = 0;
        nOrgLcomK = 0;

        UlFor.Initialize();
        CsFor.Initialize();

        dfcw     = 0.0;       
        dfcw_lim = 0.0;   
        dfcf     = 0.0;       
        dfcf_lim = 0.0;   
        dftf     = 0.0;       
        dftf_lim = 0.0;  
        dfdeck   = 0.0;
        dphifr   = 0.0;
		dfdeck  =0;
		dFactorCS = 0.0;
        dflc     = 0.0;
        dflt     = 0.0;

		dRatio = 0.0;
        nOK = 0;
    }

    T_SCBR_BASE() { Initialize(); }
};

struct T_SCBR_ARRAY_D
{

    CArray<T_SCBR_BASE,T_SCBR_BASE&> arScbrB[4];  
};


struct T_SCBR_D
{
    T_SCBR_BASE ScbrB[4];   // 0: I-정, 1: I-부, 2: J-정, 3: J-부, 

    void Initialize()
    {
        for (int i=0; i<4; ++i) ScbrB[i].Initialize();
    }

    T_SCBR_D() { Initialize(); }
};

struct T_SCBR_UNIT
{
    T_SCBR_UNIT()
    {
        dfcw      = D_UNITSYS_BASE_STRESS;       
        dfcw_lim  = D_UNITSYS_BASE_STRESS;   
        dfcf      = D_UNITSYS_BASE_STRESS;       
        dfcf_lim  = D_UNITSYS_BASE_STRESS;   
        dftf      = D_UNITSYS_BASE_STRESS;       
        dftf_lim  = D_UNITSYS_BASE_STRESS;   
        dfdeck    = D_UNITSYS_BASE_STRESS; 
        dphifr    = D_UNITSYS_BASE_STRESS; 
        dflc      = D_UNITSYS_BASE_STRESS;
        dflt      = D_UNITSYS_BASE_STRESS;
    }
    int dfcw;       
    int dfcw_lim;   
    int dfcf;       
    int dfcf_lim;   
    int dftf;       
    int dftf_lim;   
    int dfdeck;  
    int dphifr;  
	int dflc;
	int dflt;
};

// 시공단계 전단검토
#define T_SCVR_K unsigned int
struct T_SCVR_BASE 
{
    BOOL bChk;
    UINT nStageK;     // Stage              [Unit=None]
    UINT nStepK;      // Step               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]

    T_CSGD_ULMF  UlFor;
    T_CSGD_FORCE CsFor;

    double dVu;      // 설계전단력    (Vu < Vr 일 때 OK)
	double dVu4T;    // Torsion에 의한 전단력 
	double dVuApp;   // Vu + Vu4T
	double dVr;      // 설계전단강도

	double dRatio;
    int  nOK;

    void Initialize()
    {
        bChk = FALSE;
        nStageK = 0;
        nStepK  = 0;
        nOrgLcomK = 0;

        UlFor.Initialize();
        CsFor.Initialize();

        dVu      = 0.0;
		dVu4T    = 0.0;
		dVuApp   = 0.0;
        dVr      = 0.0;      

        nOK = 0;
    }

    T_SCVR_BASE() { Initialize(); }
};

struct T_SCVR_D
{
    T_SCVR_BASE ScvrB[2];

    void Initialize()
    {
        for (int i=0; i<2; ++i) ScvrB[i].Initialize();
    }

    T_SCVR_D() { Initialize(); }
};

struct T_SCVR_UNIT
{
    T_SCVR_UNIT()
    {
        dVu      = D_UNITSYS_BASE_FORCE;      
        dVr      = D_UNITSYS_BASE_FORCE;      
    }
    
    int dVu;      
    int dVr;     
};

//////////////////////////////////////////////////////////////////////////
// 완성계 

// 휨강도
#define T_CSBR_K unsigned int
struct T_CSBR_BASE 
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    BOOL bChkUlmf;
    T_CSGD_ULMF  UlForLC;
    T_CSGD_ULMF  UlForNC;
    T_CSGD_ULMF  UlForLT;
    T_CSGD_ULMF  UlForST;

    T_CSGD_COMF  ComFor;

    double dMy;  // 항복모멘트
    double dMp;  // 소성모멘트
    double dMu;  // 설계모멘트
    double dMr;  // 설계휨강도
    double dFu;  // 설계모멘트 응력
    double dFr;  // 설계휨강도 응력

    // AASHTO
    int nCase;    
    // 0 : 정, compact,    기준식
    // 1 : 정, noncompact, 기준식
    // 2 : 부,             기준식
    // 3 : 부,             Appendix A6
    
    double dDp;   // Dp <= 0.42Dt 일 때 OK						
    double d042Dt;
    double dFuc;  // 설계모멘트 응력(압축)
    double dFrc;  // 설계휨강도 응력(압축)
    double dMuc;  // 설계모멘트(압축)
    double dMrc;  // 설계휨강도(압축)

    double dMrat;

    int  nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax = 0;

        bChkUlmf = FALSE;
        UlForLC.Initialize();
        UlForNC.Initialize();
        UlForLT.Initialize();
        UlForST.Initialize();
        ComFor.Initialize();

        dMy = 0.0;  
        dMp = 0.0;
        dMu = 0.0;
        dMr = 0.0;
        dFu = 0.0;
        dFr = 0.0;
		nCase = 0;
        dDp   = 0.0;
        d042Dt= 0.0;
        dFuc = 0.0;
        dFrc = 0.0;
        dMuc = 0.0;
        dMrc = 0.0;
        dMrat=0.0;
        nOK = 0;
    }

    T_CSBR_BASE() { Initialize(); }
};

struct T_CSBR_D
{
    T_CSBR_BASE CsbrB[4];

    void Initialize()
    {
        for (int i=0; i<4; ++i) CsbrB[i].Initialize();
    }

    T_CSBR_D() { Initialize(); }
};

struct T_CSBR_UNIT
{
    T_CSBR_UNIT()
    {
        dMy   = D_UNITSYS_BASE_MOMENT;
        dMp   = D_UNITSYS_BASE_MOMENT;
        dMu   = D_UNITSYS_BASE_MOMENT;
        dMr   = D_UNITSYS_BASE_MOMENT;
        dFu   = D_UNITSYS_BASE_STRESS;
        dFr   = D_UNITSYS_BASE_STRESS;
        dDp      = D_UNITSYS_BASE_LENGTH;
        d042Dt   = D_UNITSYS_BASE_LENGTH;
        dFuc   = D_UNITSYS_BASE_STRESS;
        dFrc   = D_UNITSYS_BASE_STRESS;
        dMuc = D_UNITSYS_BASE_STRESS;
        dMrc = D_UNITSYS_BASE_STRESS;
    }
    int dMy;
    int dMp;
    int dMu;
    int dMr;
    int dFu;
    int dFr;
    int dDp;
    int d042Dt;
    int dFuc;
    int dFrc;
    int dMuc;
    int dMrc;
};


// 전단강도
#define T_CSVR_K unsigned int
struct T_CSVR_BASE 
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    T_CSGD_ULMF  UlFor;
    T_CSGD_COMF  ComFor;

    double dVu;      // 설계전단력    (Vu < Vr 일 때 OK)
	double dVu4T;    // Torsion에 의한 전단력 
	double dVuApp;   // Vu + Vu4T, web 1 개의 전단력 
    double dVr;      // 설계전단강도
    double ddo_lim;  // 수직보강재 간격 제한값 (do_lim > do  일 때  OK)
    double ddo;      // 수직보강재 간격
    double dbt_lim1; // 수직보강재 돌출폭 제한값  = 50.0 + d / 30.0   (bt > bt_lim1 일 때 OK )
    double dbt_lim2; // 수직보강재 돌출폭 제한값  = 16.0 tp          (bt_lim2 > bt > bt_lim3 일 때 OK)
    double dbt_lim3; // 수직보강재 돌출폭 제한값  = 0.25 bf
    double dbt;      // 수직보강재 돌출폭
    double dIt_lim;  // 수직보강재 단면이차모멘트 제한값  (It_lim <  It  일 때  OK)
    double dIt;      // 수직보강재 단면이차모멘트
    double dAs_lim;  // 수직보강재 단면적 제한값          (As_lim <As 일 대 OK)
    double dAs;      // 수직보강재 단면적

    BOOL bChk_do;
    BOOL bChk_bt;
    BOOL bChk_It;
    BOOL bChk_As;

    double dVrat;
    int  nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax = 0;

        UlFor.Initialize();
        ComFor.Initialize();

        dVu      = 0.0;
		dVu4T    = 0.0;
		dVuApp   = 0.0;
        dVr      = 0.0;      
        ddo_lim  = 0.0;  
        ddo      = 0.0;      
        dbt_lim1 = 0.0; 
        dbt_lim2 = 0.0; 
        dbt_lim3 = 0.0; 
        dbt      = 0.0;      
        dIt_lim  = 0.0;  
        dIt      = 0.0;      
        dAs_lim  = 0.0;  
        dAs      = 0.0;    

        bChk_do = FALSE;
        bChk_bt = FALSE;
        bChk_It = FALSE;
        bChk_As = FALSE;

        dVrat    = 0.0;
        nOK = 0;
    }

    T_CSVR_BASE() { Initialize(); }
};

struct T_CSVR_D
{
    T_CSVR_BASE CsvrB[2];

    void Initialize()
    {
        for (int i=0; i<2; ++i) CsvrB[i].Initialize();
    }

    T_CSVR_D() { Initialize(); }
};

struct T_CSVR_UNIT
{
    T_CSVR_UNIT()
    {
        dVu      = D_UNITSYS_BASE_FORCE;      
        dVr      = D_UNITSYS_BASE_FORCE;      
        ddo_lim  = D_UNITSYS_BASE_LENGTH;  
        ddo      = D_UNITSYS_BASE_LENGTH;        
        dbt_lim1 = D_UNITSYS_BASE_LENGTH;   
        dbt_lim2 = D_UNITSYS_BASE_LENGTH;   
        dbt_lim3 = D_UNITSYS_BASE_LENGTH;   
        dbt      = D_UNITSYS_BASE_LENGTH;        
        dIt_lim  = D_UNITSYS_BASE_STIF;    
        dIt      = D_UNITSYS_BASE_STIF;        
        dAs_lim  = D_UNITSYS_BASE_AREA;    
        dAs      = D_UNITSYS_BASE_AREA;       
    }

    int dVu;      
    int dVr;      
    int ddo_lim;  
    int ddo;      
    int dbt_lim1; 
    int dbt_lim2; 
    int dbt_lim3; 
    int dbt;      
    int dIt_lim;  
    int dIt;      
    int dAs_lim;  
    int dAs;      


};



// 사용성 검토
#define T_CSAC_K unsigned int
struct T_CSGS_BASE
{
    double dfcw;     //  복부판 응력
    double dfcf;     //  압축플랜지 응력
    double dftf;     //  인장플랜지 응력
    BOOL   bIsTopC;  //  Top이 압축플랜지?

    void Initialzie()
    {
        dfcw     = 0.0;       
        dfcf     = 0.0;       
        dftf     = 0.0;
        bIsTopC  = TRUE;
    }

    T_CSGS_BASE() {Initialzie();}
};
struct T_CSAC_BASE 
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    T_CSGD_ULMF  UlFor;
    T_CSGD_COMF  ComFor;

    double dfcw;       //  복부판 응력           (fcw < fcw_lim  OK)
    double dfcw_lim;   //  복부판의 공칭 휨저항
    double dfcf;       //  압축플랜지 응력       (fcf < fcf_lim  OK)
    double dfcf_lim;   //  압축플랜지 응력제한
    double dftf;       //  인장플랜지 응력       (ftf < ftf_lim  OK)
    double dftf_lim;   //  인장플랜지 응력제한


    BOOL bChk_6_10_4_4_2;
    BOOL bChk_6_10_4_4_2_4;

    double dfrat;
    int  nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax = 0;

        UlFor.Initialize();
        ComFor.Initialize();

        dfcw     = 0.0;       
        dfcw_lim = 0.0;   
        dfcf     = 0.0;       
        dfcf_lim = 0.0;   
        dftf     = 0.0;       
        dftf_lim = 0.0;   
        bChk_6_10_4_4_2   = TRUE;
        bChk_6_10_4_4_2_4 = TRUE;

        dfrat    = 0.0;
        nOK = 0;
    }

    T_CSAC_BASE() { Initialize(); }
};

struct T_CSAC_D
{
    T_CSAC_BASE CsacB[4];

    void Initialize()
    {
        for (int i=0; i<4; ++i) CsacB[i].Initialize();
    }

    T_CSAC_D() { Initialize(); }
};

struct T_CSAC_UNIT
{
    T_CSAC_UNIT()
    {
        dfcw      = D_UNITSYS_BASE_STRESS;       
        dfcw_lim  = D_UNITSYS_BASE_STRESS;   
        dfcf      = D_UNITSYS_BASE_STRESS;       
        dfcf_lim  = D_UNITSYS_BASE_STRESS;   
        dftf      = D_UNITSYS_BASE_STRESS;       
        dftf_lim  = D_UNITSYS_BASE_STRESS;   
    }
    int dfcw;       
    int dfcw_lim;   
    int dfcf;       
    int dfcf_lim;   
    int dftf;       
    int dftf_lim;   
};


// 피로검토
#define T_CSFC_K unsigned int
struct T_CSFC_LOAD
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nfcMax;           //                     [Unit=None]
    int  nftMax;           //                     [Unit=None]
    
    T_CSGD_COMF_MV  ComForMV; // 0=fc, 1=ft;
    int  nForId[2];            // fc, ft에 해당하는 동시발생 부재력 index
    
    double dfc;
    double dft;
    double dGammaDelF;     // 피로한계상태 응력범위
    double dGammaDelF_top; // 피로한계상태 응력범위 for top flange
    double dGammaDelF_bot; // 피로한계상태 응력범위 for bottom flange
    
    double dMyMax_Pos;
    double dMyMax_Neg;
    double dftop_com_st;
    double dftop_ten_st;
    double dfbot_com_st;
    double dfbot_ten_st;
    
    double dDelF_n;        // 공칭피로 강도
    BOOL bChkLoadInducedFati;         // 하중유발피로검토 여부
    BOOL bChkLoadInducedFati_top;     // 하중유발피로검토 여부 for top flange
    BOOL bChkLoadInducedFati_bot;     // 하중유발피로검토 여부 for bottom flange
    
    BOOL bPrtLoadInducedFati;         // 하중유발피로출력 여부
    BOOL bPrtLoadInducedFati_top;     // 하중유발피로출력 여부 for top flange
    BOOL bPrtLoadInducedFati_bot;     // 하중유발피로출력 여부 for bottom flange
    
    double dRatio;
    int    nOK;
    
    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nfcMax = 0;
        nftMax = 0;
        //for (int i=0; i<2; ++i) ComFor[i].Initialize();
        memset(nForId,0,sizeof(nForId));
        dfc = 0.0;
        dft = 0.0;
        dMyMax_Pos = 0.0;
        dMyMax_Neg = 0.0;
        dftop_com_st = 0.0;
        dftop_ten_st = 0.0;
        dfbot_com_st = 0.0;
        dfbot_ten_st = 0.0;    
        dGammaDelF = 0.0;
        dGammaDelF_top = 0.0;
        dGammaDelF_bot = 0.0;
        dDelF_n = 0.0;
        bChkLoadInducedFati = TRUE;
        bPrtLoadInducedFati = TRUE;
        dRatio = 0.0;
        nOK = 0;
    }
    T_CSFC_LOAD() {Initialize();}
};
struct T_CSFC_LOAD_UNIT
{
    T_CSFC_LOAD_UNIT()
    {
        dfc        = D_UNITSYS_BASE_STRESS;
        dft        = D_UNITSYS_BASE_STRESS;
        dGammaDelF = D_UNITSYS_BASE_STRESS;
        dDelF_n    = D_UNITSYS_BASE_STRESS;
        
        dMyMax_Pos = D_UNITSYS_BASE_MOMENT;
        dMyMax_Neg = D_UNITSYS_BASE_MOMENT;
        dftop_com_st   = D_UNITSYS_BASE_STRESS;
        dftop_ten_st   = D_UNITSYS_BASE_STRESS;
        dfbot_com_st   = D_UNITSYS_BASE_STRESS;
        dfbot_ten_st   = D_UNITSYS_BASE_STRESS;        
        dGammaDelF_top = D_UNITSYS_BASE_STRESS;
        dGammaDelF_bot = D_UNITSYS_BASE_STRESS;
        
    }
    
    int dfc;
    int dft;
    int dGammaDelF;
    int dDelF_n;  
    int dMyMax_Pos;
    int dMyMax_Neg ;
    int dftop_com_st;
    int dftop_ten_st;
    int dfbot_com_st;
    int dfbot_ten_st;    
    int dGammaDelF_top;
    int dGammaDelF_bot;
    
};

struct T_CSFC_FLEX 
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    T_CSGD_COMF  ComFor;

    double dfcf;           // 복부판 최대전단탄성응력
    double dFyw;           // 복부항복강도
	double dRatio;
    int    nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax = 0;
        ComFor.Initialize();
        dfcf=0.0;
        dFyw=0.0;
		dRatio = 0.0;
        nOK=0;
    }
    T_CSFC_FLEX() {Initialize();}

};
struct T_CSFC_FLEX_UNIT
{
    T_CSFC_FLEX_UNIT()
    {
        dfcf       = D_UNITSYS_BASE_STRESS;
        dFyw       = D_UNITSYS_BASE_STRESS;
    }
    
    int dfcf;
    int dFyw;  
};

struct T_CSFC_SHEAR 
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]
    
    T_CSGD_COMF  ComFor;
    
    double dVcf;           // 복부판 최대전단탄성응력  ,dVu
    double dVcf_lim;       // 전단저항값               ,dVcr
    double dRatio;
    int    nOK;
    
    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax =0;
        ComFor.Initialize();
        dVcf=0.0;
        dVcf_lim=0.0;
		dRatio = 0.0;
        nOK=0;
    }
    T_CSFC_SHEAR() {Initialize();}
    
};
struct T_CSFC_SHEAR_UNIT
{
    T_CSFC_SHEAR_UNIT()
    {
        dVcf       = D_UNITSYS_BASE_FORCE;
        dVcf_lim   = D_UNITSYS_BASE_FORCE;
    }
    
    int dVcf;
    int dVcf_lim;  
};


struct T_CSFC_BASE 
{
    BOOL bChk;

    T_CSFC_LOAD  LoadR[2]; // flange Top,Bottom
    T_CSFC_FLEX  FlexR;
    T_CSFC_SHEAR ShearR;

    double dFrat;
    int  nOK;

    void Initialize()
    {
        bChk = FALSE;
        for (int i=0; i<2; ++i) LoadR[i].Initialize();
        FlexR.Initialize();
        ShearR.Initialize();

        dFrat      = 0.0;
        nOK = 0;
    }

    T_CSFC_BASE() { Initialize(); }
};

struct T_CSFC_D
{
    T_CSFC_BASE CsfcB[2]; // I,J

    void Initialize()
    {
        for (int i=0; i<2; ++i) CsfcB[i].Initialize();
    }

    T_CSFC_D() { Initialize(); }
};

// 전단연결재 검토
#define T_CSCR_K unsigned int
struct T_CSCR_BASE 
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    double dVst;  // 전단력 폭
    double df_fat;  // 상부 플랜지 응력 폭
    double df_fat_B;// 하부 플랜지 응력 폭

    double dH_D;
    double dH_D_lim;
    double dp;
    double dp_lim1;
    double dp_lim2;
    double dp_lim3; // p_edge (AASHTO)
    double dN;      // 개수
    double dNreq;
    double dCover;
    double dPenet;
    double dTVs;  // transverse spacing
    double dEdge; // transverse edge spacing
    double dGammaDelF;           // 피로한계상태 응력범위
    double dpDelF_n;             // 설계피로 강도
    BOOL   bIsFatiChkNeed;

    T_CSFC_LOAD LoadR[2];  // shear connector fatigue check


    double dSCrat;
    int  nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax = 0;

        dVst = 0.0;
        df_fat = 0.0;
        df_fat_B = 0.0;

        dH_D     = 0.0;
        dH_D_lim = 0.0;
        dp       = 0.0;
        dp_lim1  = 0.0;
        dp_lim2  = 0.0;
        dp_lim3  = 0.0;
        dN       = 0.0;
        dNreq    = 0.0;
        dCover   = 0.0;
        dPenet   = 0.0;
        dTVs     = 0.0;
        dEdge    = 0.0;
        dGammaDelF = 0.0;
        dpDelF_n   = 0.0;
        bIsFatiChkNeed = TRUE;
        dSCrat   = 0.0;
        nOK = 0;
        LoadR[0].Initialize();
        LoadR[1].Initialize();
    }

    T_CSCR_BASE() { Initialize(); }
};

struct T_CSCR_D
{
    T_CSCR_BASE CscrB[2];
    
    void Initialize()
    {
        for (int i=0; i<2; ++i) CscrB[i].Initialize();
    }

    T_CSCR_D() { Initialize(); }
};

struct T_CSCR_UNIT
{
    T_CSCR_UNIT()
    {
        dVst     = D_UNITSYS_BASE_FORCE;
        df_fat   = D_UNITSYS_BASE_STRESS;
        df_fat_B = D_UNITSYS_BASE_STRESS;
        dp       = D_UNITSYS_BASE_LENGTH;
        dp_lim1  = D_UNITSYS_BASE_LENGTH;
        dp_lim2  = D_UNITSYS_BASE_LENGTH;
        dp_lim3  = D_UNITSYS_BASE_LENGTH;
        dCover   = D_UNITSYS_BASE_LENGTH;
        dPenet   = D_UNITSYS_BASE_LENGTH;
        dTVs     = D_UNITSYS_BASE_LENGTH;
        dEdge    = D_UNITSYS_BASE_LENGTH;
        dGammaDelF = D_UNITSYS_BASE_STRESS;
        dpDelF_n   = D_UNITSYS_BASE_STRESS;  
    }

    int dVst;
    int df_fat;
    int df_fat_B;

    int dp       ;
    int dp_lim1  ;
    int dp_lim2  ;
    int dp_lim3  ;
    int dCover   ;
    int dPenet   ;
    int dTVs     ;
    int dEdge    ;
    int dGammaDelF; 
    int dpDelF_n;   
};


// 수평보강재
struct T_CSSL_BASE_885
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    double dbl;
    double dbl_lim;    
    double dI;
    double dI_lim;   
    double dr;
    double dr_lim;

    double dfs;
    double dphiRhFys;

    double dSFrat;
    int  nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax = 0;

        dbl     = 0.0;
        dbl_lim = 0.0;    
        dI      = 0.0;
        dI_lim  = 0.0;   
        dr      = 0.0;
        dr_lim  = 0.0;
        dfs     = 0.0;
        dphiRhFys  = 0.0;

        dSFrat  = 0.0;
        nOK = 0;
    }

    T_CSSL_BASE_885() { Initialize(); }
};

#define T_CSSL_K unsigned int
struct T_CSSL_BASE
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    T_CSGD_COMF  ComFor;
     
    BOOL   bIsWeb;
    double dbl;
    double dbl_lim;    
    double dI;
    double dI_lim;   
    double dr;
    double dr_lim;

    double dfs;
    double dphiRhFys;

    BOOL   bIsFlg;
    double dbl_f;
    double dbl_lim_f;    
    double dI_f;
    double dI_lim_f;   

    double dSFrat;
    int  nOK;
    
    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax = 0;
                 
        ComFor.Initialize();
        bIsWeb  = FALSE;
        dbl     = 0.0;
        dbl_lim = 0.0;    
        dI      = 0.0;
        dI_lim  = 0.0;   
        dr      = 0.0;
        dr_lim  = 0.0;
        dfs     = 0.0;
        dphiRhFys  = 0.0;
        
        bIsFlg    = FALSE;
        dbl_f     = 0.0;
        dbl_lim_f = 0.0;    
        dI_f      = 0.0;
        dI_lim_f  = 0.0;   

        dSFrat  = 0.0;
        nOK = 0;
    }
    
    T_CSSL_BASE() { Initialize(); }

    void Convert885(const T_CSSL_BASE_885& rData)
    {
        bChk      = rData.bChk;
        nLcomK    = rData.nLcomK   ;
        nOrgLcomK = rData.nOrgLcomK;
        nMax      = rData.nMax     ;
        dbl       = rData.dbl      ;
        dbl_lim   = rData.dbl_lim  ;    
        dI        = rData.dI       ;
        dI_lim    = rData.dI_lim   ;   
        dr        = rData.dr       ;
        dr_lim    = rData.dr_lim   ;
        dfs       = rData.dfs      ;
        dphiRhFys = rData.dphiRhFys;
        dSFrat    = rData.dSFrat   ;
        nOK       = rData.nOK;

    }
};

struct T_CSSL_D_885
{
    T_CSSL_BASE_885 CsslB[2];

    void Initialize()
    {
        for (int i=0; i<2; ++i) CsslB[i].Initialize();
    }

    T_CSSL_D_885() { Initialize(); }
};

struct T_CSSL_D
{
    T_CSSL_BASE CsslB[2];
    
    void Initialize()
    {
        for (int i=0; i<2; ++i) CsslB[i].Initialize();
    }
    
    T_CSSL_D() { Initialize(); }

    void Convert885(const T_CSSL_D_885& rData)
    {
        for (int i=0; i<2; ++i) CsslB[i].Convert885(rData.CsslB[i]);
    }
};

struct T_CSSL_UNIT
{
    T_CSSL_UNIT()
    {
        dbl     = D_UNITSYS_BASE_LENGTH;
        dbl_lim = D_UNITSYS_BASE_LENGTH;    
        dI      = D_UNITSYS_BASE_STIF;
        dI_lim  = D_UNITSYS_BASE_STIF;   
        dr      = D_UNITSYS_BASE_LENGTH;
        dr_lim  = D_UNITSYS_BASE_LENGTH;

        dfs     = D_UNITSYS_BASE_STRESS;
        dphiRhFys= D_UNITSYS_BASE_STRESS;

    }
    int dbl;
    int dbl_lim;    
    int dI;
    int dI_lim;   
    int dr;
    int dr_lim;
    int dfs     ;
    int dphiRhFys;


};

// Bearing Stiffener 검토
struct T_BSTF_BASE 
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]
    
    double dbt;
    double dbt_lim;
    double dR;
    double dBr;
    double dPr;
    
    int  nOK;
    
    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax = 0;
        
        dbt     = 0.0;
        dbt_lim = 0.0;
        dR      = 0.0;
        dBr     = 0.0;
        dPr     = 0.0;
        
        nOK = 0;
    }
    
    T_BSTF_BASE() { Initialize(); }
};

struct T_BSTF_D
{
    T_BSTF_BASE BstfB[2];
    
    void Initialize()
    {
        for (int i=0; i<2; ++i) BstfB[i].Initialize();
    }
    
    T_BSTF_D() { Initialize(); }
};

struct T_BSTF_UNIT
{
    T_BSTF_UNIT()
    {
        dbt     = D_UNITSYS_BASE_LENGTH;
        dbt_lim = D_UNITSYS_BASE_LENGTH;
        dR      = D_UNITSYS_BASE_FORCE;
        dBr     = D_UNITSYS_BASE_FORCE;
        dPr     = D_UNITSYS_BASE_FORCE;
    }
    
    int dbt;
    int dbt_lim;
    int dR;
    int dBr;
    int dPr;
    
};


// 종합 검토.
#define T_CSCH_K unsigned int

struct T_CSCH_BASE
{
    BOOL bChk;
    int  nOK;

    T_CSBR_BASE  CSBRbase;  // 휨
    T_CSVR_BASE  CSVRbase;  // 전단
    T_CSAC_BASE  CSACbase;  // 사용한계
    T_CSFC_BASE  CSFCbase;  // 피로한계
    T_SCBR_BASE  SCBRbase;  // 시공성(휨)
    T_SCVR_BASE  SCVRbase;  // 시공성(전단)
    T_CSCR_BASE  CSCRbase;  // 전단연결재
    T_CSSL_BASE  CSSLbase;  // 수평보강재

    void Initialize()
    { 
        bChk = FALSE;
        nOK =1;
             
        CSBRbase.Initialize();
        CSVRbase.Initialize();
        CSACbase.Initialize();
        CSFCbase.Initialize();
        SCBRbase.Initialize();
        SCVRbase.Initialize();
        CSCRbase.Initialize();
        CSSLbase.Initialize(); 
    }
    T_CSCH_BASE() { Initialize(); }


};

struct T_CSCH_D
{
    T_CSCH_BASE CschB[4];
 
    void Initialize()
    {
        for (int i=0; i<4; ++i) CschB[i].Initialize();
    }

    T_CSCH_D() { Initialize(); }
};


// Span 검토.
struct T_CSCK_B
{
    T_ELEM_K ElemK;
    int      nPos;
    UINT     nBeamType;

    void Initialize()
    {
        ElemK = 0;
        nPos = 0;
        nBeamType = 0;
    }
    T_CSCK_B () {Initialize();}
	ElemPairK GetElemPairK() const
	{
		return ElemPairK(ElemK, nBeamType);
	}
};

struct T_CSPR_D
{
    T_SPAN_K OrgSpanK;
    T_SPAN_K SubSpanK;
    T_CSCK_B Flex[2]; // 0=Positive M, 1=Negative M
    T_CSCK_B Shear;
    T_CSCK_B Serv[2]; // 0=Positive M, 1=Negative M
    T_CSCK_B Fati[2]; // 0=Positive M, 1=Negative M
    T_CSCK_B ConF[2]; // 0=Positive M, 1=Negative M
    T_CSCK_B ConS;
    T_CSCK_B SCon;
    T_CSCK_B LStfn;
    
    void Initialize()
    {
        OrgSpanK=0;
        SubSpanK=0;

        for (int i=0; i<2; ++i)
        {
            Flex[i].Initialize();
            Serv[i].Initialize();
            Fati[i].Initialize();
            ConF[i].Initialize();
        }    
        Shear.Initialize();
        ConS.Initialize();
        SCon.Initialize();
        LStfn.Initialize();
    }
    
    T_CSPR_D() { Initialize(); }
};


// AASHTO Load Rating - Composite Steel Girder

struct T_RFDT_B
{
    double dKa;
    double dKb;
    double dK;
    double dEpsiC;
    double dEpsiT;

    void Initialize()
    {
        dKa=dKb=dK=0.0;
        dEpsiC=dEpsiT=0.0;
    }

    T_RFDT_B() {Initialize();}
};

// Strength Result - Flexural Strength, Shear Strength
struct T_RCST_BASE
{
    BOOL bChk;
    T_ELEM_K ElemK;
    UINT nPosi;
    UINT nMoment;      // 1=Positive, 2=Negative
    T_GRUP_K nGrupKey; // Group Key
    UINT RCaseK;       // Rating Case Key            [Unit=None]
    BOOL bDCmax;       // DC(Max,Min Factor)                     [Unit=None]
    BOOL bDWmax;       // DW(Max,Min Factor)                     [Unit=None]
    BOOL bTEmax;       // Temperature(Max,Min Factor)            [Unit=None]
    UINT nConcurrent;  // Fx_max .... Mz_min(1~12)               [Unit=None] 
    UINT nIsLocation;  // Is Location Data 0=Non, 1=Exist        [Unit=None]
    T_SPAN_K SpanKey;  // Span Key                               [Unit=None]
    UINT nSpanPos;     // Span postion                           [Unit=None]

    double dPhi_CF;    // Condition Factor           [Unit=None]  
    double dPhi_SF;    // System Factor              [Unit=None]
    double dPhi_RE;    // Resistance Factor          [Unit=None]
    double drDC_BE;    // DC Factor Before           [Unit=None]
    double drDC;       // DC Factor                  [Unit=None]
    double drDW;       // DW Factor                  [Unit=None]
    double drTE;       // Temperature Factor         [Unit=None]
    double drTG;       // Temperature Gradient Factor[Unit=None]
    double drP;        // Permanent Factor           [Unit=None]
    double drSE;       // Secondary Factor           [Unit=None]
    double drUS;       // User Factor                [Unit=None]
    double drPR;       // Primary Vehicle Factor     [Unit=None]
    double drAD;       // Adjacent Vehicle Factor    [Unit=None]

    T_CSGD_ULMF  UlFor;
    T_CSGD_COMF  ComFor;  
    T_CSGD_FORCE ForDCBe;     // DC (My) 
    T_CSGD_FORCE ForDC;     // DC (My)              
    T_CSGD_FORCE ForDW;     // DW                   
    T_CSGD_FORCE ForTE;     // Temperature          
    T_CSGD_FORCE ForTG;     // Temperature Gradient 
    T_CSGD_FORCE ForP;      // Permanent             
    T_CSGD_FORCE ForSE;     // Secondary            
    T_CSGD_FORCE ForUS;     // User                 
    T_CSGD_FORCE ForPR;     // Primary Vehicle      
    T_CSGD_FORCE ForAD;     // Adjacent Vehicle     
    T_CSGD_FORCE ForSUM;    // DC+DW+TE+SE+P+US+AD  
    double       dfbu_PR;   // Primary Vehicle Stress

    int    nCaseRes;
    double dMn_CA;     // Bending Capacity           [Unit=Moment]
    double dpMn_CA;    // [Unit=Moment]
    double dFn_CA;     // [Unit=Stress]
    double dpFn_CA;    // [Unit=Stress]
    double dfbu_SUM;

    double dCaDeM;     // Capacity-Demand M [Unit=Moment]
    double dCaDeF;     // Capacity-Demand F [Unit=Stress]
    int    nRFcase;    // 0: 강도로 인한 NG(같은부호), 1: 강도로 인한 NG(다른부호), 2:부호가 같은 경우 , 3: 부호가 다른 경우
    double dCaDeSh;
    int    nRFcasesh;  // 0 : 강도로 인한 NG, 1 : 강도  OK

    double dRF;        // Rating Factor              [Unit=None]
    double dKRF;       // Rating Factor              [Unit=None]
    double dRT;        // Safe Load Capacity(tons)   [Unit=None]
    double dSLCapa;    // Safe Load Capacity(tons)
    double dRel_Locat; // Relative Location          [Unit=None]

    double dVn_CA;     // 공칭전단강도                     [Unit=Force]
    double dVr_CA;     // 설계전단강도                     [Unit=Force]
    double dRFsh;      // Rating Factor - Shear
    double dKRFsh;
    double dSLCapaSh;

    T_RFDT_B DtkF;     // 휨
    T_RFDT_B DtkS;     // 전단 

    void Initialize()
    {
        bChk = 0;
        ElemK = 0;
        nPosi = 0;
        nMoment = 0;
        RCaseK = 0;
        bDCmax = bDWmax = bTEmax = TRUE;
        nConcurrent = 1;
        nIsLocation = 0;
        SpanKey = 0;
        nSpanPos = 0;
        dPhi_CF = dPhi_SF = dPhi_RE = 0.0;
        drDC_BE = drDC = drDW = drTE = drTG = drP = drSE = drUS = drPR = drAD = 0.0;   
        UlFor.Initialize();
        ComFor.Initialize();
        ForDCBe.Initialize();
        ForDC.Initialize();
        ForDW.Initialize();
        ForTE.Initialize();
        ForTG.Initialize();
        ForP.Initialize();
        ForSE.Initialize();
        ForUS.Initialize();
        ForPR.Initialize();
        ForAD.Initialize();
        ForSUM.Initialize();
        dfbu_PR  = 0.0;
        nCaseRes = 0;
        dMn_CA   = 0.0;
        dpMn_CA  = 0.0;
        dFn_CA   = 0.0;
        dpFn_CA  = 0.0;
        dfbu_SUM = 0.0;
        dCaDeM  = 0.0;
        dCaDeF  = 0.0;
        nRFcase = 0;
        dCaDeSh = 0.0;
        nRFcasesh = 0;
        dRF = dRT = 0.0;
        dKRF = 0.0;
        dSLCapa = 0.0;
        dRel_Locat = 0.0;
        dVn_CA = 0.0;
        dVr_CA = 0.0;
        dRFsh  = 0.0;
        dKRFsh = 0.0;
        dSLCapaSh = 0.0;
        DtkF.Initialize();
        DtkS.Initialize();
    }

    T_RCST_BASE() { Initialize(); }
};

// Rating Case
struct T_RCST_CASE
{
    // 0=I-Posi, 1=J-Posi
    T_RCST_BASE RcstB[2];
    
    void Initialize()
    {
        for(int i=0; i<2; ++i) RcstB[i].Initialize();
    }

    T_RCST_CASE() {Initialize();}
};

struct T_RCCR_BASE
{
    T_ELEM_K ElemK;
    UINT     nPos;
    UINT     nBeamType;
    T_GRUP_K nGrupKey;    // Group Key
    UINT     RCaseK;      // Rating Case Key
    BOOL     bDCmax;       // DC(Max,Min Factor)                     [Unit=None]
    BOOL     bDWmax;       // DW(Max,Min Factor)                     [Unit=None]
    BOOL     bTEmax;       // Temperature(Max,Min Factor)            [Unit=None]
    UINT     nConcurrent; // Fx_max .... Mz_min(1~12) 

    void Initialize()
    {
        ElemK       = 0;
        nPos        = 0;
        nBeamType   = 0;
        nGrupKey    = 0;
        RCaseK      = 0;
        bDCmax      = TRUE;
        bDWmax      = TRUE;
        bTEmax      = TRUE;
        nConcurrent = 0;
    }

    T_RCCR_BASE() {Initialize();}
	ElemPairK GetElemPairK() const
	{
		return ElemPairK(ElemK, nBeamType);
	}
};

// Flexural Rating Factor(smallest)
struct T_RCFC_D
{
    // 0=I-Posi, 1=I-Nega, 2=J-Posi, 3=J-Nega.
    T_RCCR_BASE FlexR[4];
    
    void Initialize()
    {
        for(int i=0; i<4; ++i) FlexR[i].Initialize();
    }

    T_RCFC_D() {Initialize();}
};

// Shear Rating Factor(smallest)
struct T_RCSC_D
{
    T_RCCR_BASE ShearR[2];  // 0=I-Pos, 1=I-Pos
    
    void Initialize()
    {
        for(int i=0; i<2; ++i) ShearR[i].Initialize();
    }

    T_RCSC_D() {Initialize();}
};


// Steel Stress
struct T_RCSS_BASE
{
    BOOL bChk;
    T_ELEM_K ElemK;
    UINT nPosi;              // 1=I-End, 2=J-END
    T_GRUP_K nGrupKey;			 // Group Key
    UINT RCaseK;
    BOOL bDCmax;             // DC(Max,Min Factor)                     [Unit=None]
    BOOL bDWmax;             // DW(Max,Min Factor)                     [Unit=None]
    BOOL bTEmax;             // Temperature(Max,Min Factor)            [Unit=None]
    UINT nConcurrent;        // Fx_max .... Mz_min(1~12)               [Unit=None] 
    UINT nPoint[2];					 //	[0]=comp,[1]=tens , 0=Left Top,1=RT,2=RB,3=LB
    UINT nIsLocation;        // Is Location Data 0=Non, 1=Exist        [Unit=None]
    T_SPAN_K SpanKey;        // Span Key                               [Unit=None]
    UINT nSpanPos;           // Span position                          [Unit=None]
    double dPhi_CF;          // Condition Factor                       [Unit=None]
    double dPhi_SF;          // System Factor                          [Unit=None]  
    double dPhi_RE;          // Resistance Factor                      [Unit=None]
    double drDC_BE;          // DC Factor Before                       [Unit=None]
    double drDC;             // DC Factor                              [Unit=None]
    double drDW;             // DW Factor                              [Unit=None]
    double drTE;             // Temperature Factor                     [Unit=None]
    double drTG;             // Temperature Gradient Factor            [Unit=None]
    double drP;              // Permanent Factor                       [Unit=None]
    double drSE;             // Secondary Factor                       [Unit=None]
    double drUS;             // User Factor                            [Unit=None]
    double drPR;             // Primary Vehicle Factor                 [Unit=None]
    double drAD;             // Adjacent Vehicle Factor                [Unit=None]
    
    T_CSGD_ULMF  UlFor;
    T_CSGD_COMF  ComFor;

    T_CSGS_BASE StrDC_Be;
    T_CSGS_BASE StrLT;     // Stress Long Term
    T_CSGS_BASE StrST;     // Stress Short Term
    T_CSGS_BASE StrPR;      // Primary Vehicle
    T_CSGS_BASE StrSUM;    // DC+DW+TE+SE+P+US+AD
    double dStress_AL[3];  //0=복부, 1=압축플랜지, 2=인장플랜지  [Unit=Stress]
    double dpStress_AL[3];  //0=복부, 1=압축플랜지, 2=인장플랜지  [Unit=Stress]

//   T_CSGS_BASE StrDC;    // DC                                    
//   T_CSGS_BASE StrDW;    // DW                                    
//   T_CSGS_BASE StrTE;    // Temperature                           
//   T_CSGS_BASE StrTG;    // Temperature gradient                  
//   T_CSGS_BASE StrP;     // Permanet                              
//   T_CSGS_BASE StrSE;    // Secondary                             
//   T_CSGS_BASE StrUS;    // User                                                         
//   T_CSGS_BASE StrAD;    // Adjacent Vehicle   
    
    double dCaDe[3];     // Capacity-Demand F [Unit=Stress] 0=복부, 1=압축플랜지, 2=인장플랜지
    int    nRFcase[3];   // 0: 강도로 인한 NG(같은부호), 1: 강도로 인한 NG(다른부호), 2:부호가 같은 경우 , 3: 부호가 다른 경우

    double dRF[3];        // Rating Factor, 0=복부, 1=압축플랜지, 2=인장플랜지 [Unit=None]
    double dKRF[3];       // K*Rating Factor, 0=복부, 1=압축플랜지, 2=인장플랜지 [Unit=None]
    double dRT[3];        // Safe Load Capacity(tons)               [Unit=None]
    double dSLCapa[3];
    double dRFmin;
    double dKRFmin;
    int    nCrMinType;    // Critical 0=복부, 1=압축플랜지, 2=인장플랜지
    double dRel_Locat;    // Relative Location                      [Unit=None]
    
    T_RFDT_B DtkS[3];     // 0=복부, 1=압축플랜지, 2=인장플랜지
    void Initialize()
    {
        bChk = 0;
        ElemK =0;
        nPosi = 0;
        nGrupKey = 0;
        RCaseK = 0;
        bDCmax = bDWmax = bTEmax = TRUE;
        nConcurrent = 1;
        nPoint[0] = nPoint[1] = 0;
        nIsLocation = 0;
        SpanKey = 0;
        nSpanPos = 0;
        dPhi_CF = dPhi_SF = dPhi_RE = 0.0;
        drDC_BE = drDC = drDW = drTE = drTG = drP = drSE = drUS = drPR = drAD = 0.0;   

        UlFor.Initialize();
        ComFor.Initialize();
        StrDC_Be.Initialzie();
        StrLT.Initialzie();
        StrST.Initialzie();
        StrPR.Initialzie();
        StrSUM.Initialzie();

        memset(dStress_AL,0,sizeof(dStress_AL));
        memset(dpStress_AL,0,sizeof(dpStress_AL));
        memset(dKRF, 0, sizeof(dKRF));
        memset(dCaDe, 0, sizeof(dCaDe));    
        memset(nRFcase, 0, sizeof(nRFcase));
        memset(dRF,0,sizeof(dRF));
        memset(dRT,0,sizeof(dRT));
        memset(dSLCapa,0,sizeof(dSLCapa));
        dRFmin = 0.0;
        dKRFmin = 0.0;
        nCrMinType = 0;
        dRel_Locat = 0.0;
        for(int i=0; i<3; ++i) DtkS[i].Initialize();
    }

    T_RCSS_BASE() {Initialize();}
};

struct T_RCSS_CASE
{
    T_RCSS_BASE StreB[2]; //0=I, 1=J
    
    void Initialize()
    {
        for(int i=0; i<2; ++i) StreB[i].Initialize();
    }
    
    T_RCSS_CASE() {Initialize();}
};

struct T_RCGS_D
{  
    T_RCCR_BASE StressR[6]; // 0=I-복부, 1=I-압축플랜지, 2=I-인장플랜지
                                                    // 3=J-복부, 4=J-압축플랜지, 5=J-인장플랜지
    int nPartCR[2];         // 0=I, 1=J 0=복부, 1=압축플랜지, 2=인장플랜지
    
    void Initialize()
    {
        for(int i=0; i<6; ++i) StressR[i].Initialize();
		for(int i=0; i<2; ++i) nPartCR[i]=0;
    }
    
    T_RCGS_D() {Initialize();}
};


// 피로검토
struct T_RCFA_LOAD_900
{
    T_CSGD_COMF  ComFor[2]; // 0=fc, 1=ft;

    double dfc;
    double dft;
    double dGammaDelF;           // 피로한계상태 응력범위
    double dDelF_n;              // 공칭피로 강도
    double dpDelF_n;             // 설계피로 강도
    double dfbuSum;              // DC+DW+TE+SE+P+US+AD
    BOOL   bChkLoadInducedFati;  // 하중유발피로검토 여부
    BOOL   bPrtLoadInducedFati;  // 하중유발피로출력 여부
    double dRatio;

    void Initialize()
    {
        for ( int i=0; i<2; ++i ) ComFor[i].Initialize();
        dfc = 0.0;
        dft = 0.0;
        dGammaDelF = 0.0;
        dDelF_n  = 0.0;
        dpDelF_n = 0.0;
        dfbuSum  = 0.0;
        bChkLoadInducedFati = TRUE;
        bPrtLoadInducedFati = TRUE;
        dRatio = 0.0;
    }

    T_RCFA_LOAD_900 () { Initialize(); }
};

struct T_RCFA_LOAD
{
    T_CSGD_COMF  ComFor[2]; // 0=fc, 1=ft;

    double dfc;
    double dft;
    double dGammaDelF;           // 피로한계상태 응력범위
    double dDelF_n;              // 공칭피로 강도
    double dpDelF_n;             // 설계피로 강도
    double dfbuSum;              // DC+DW+TE+SE+P+US+AD
    double dDelFmax;             // Maximum value on effective stress range
    double dDelFeff;             // effective stress range
    BOOL   bChkLoadInducedFati;  // 하중유발피로검토 여부
    BOOL   bPrtLoadInducedFati;  // 하중유발피로출력 여부
    double dRatio;

    void Initialize()
    {
        for (int i=0; i<2; ++i) ComFor[i].Initialize();
        dfc = 0.0;
        dft = 0.0;
        dGammaDelF = 0.0;
        dDelF_n  = 0.0;
        dpDelF_n = 0.0;
        dfbuSum  = 0.0;
        dDelFmax = 0.0;
        dDelFeff = 0.0;
        bChkLoadInducedFati = TRUE;
        bPrtLoadInducedFati = TRUE;
        dRatio = 0.0;
    }
    void Convert900(const T_RCFA_LOAD_900& rData)
    {
        for ( int i=0; i<2; ++i )
        {
            ComFor[i] = rData.ComFor[i];
        }
        dfc         = rData.dfc;
        dft         = rData.dft;
        dGammaDelF  = rData.dGammaDelF;
        dDelF_n     = rData.dDelF_n;
        dpDelF_n    = rData.dpDelF_n;
        dfbuSum     = rData.dfbuSum;
        dDelFmax    = 0.0;
        dDelFeff    = 0.0;
        bChkLoadInducedFati = rData.bChkLoadInducedFati;
        bPrtLoadInducedFati = rData.bPrtLoadInducedFati;
        dRatio = rData.dRatio;
    }
    T_RCFA_LOAD () {Initialize();}
};

struct T_RCFA_STRS
{
    T_CSGD_COMF  ComFor;
    
    double dfcf;           // 복부판 최대탄성응력    [Unit=Stress]
    double dFyw;           // 복부항복강도           [Unit=Stress]
    double dRatioF;

    double dVcf;           // 복부판 최대전단탄성응력  ,dVu [Unit=Stress]
    double dVcf_lim;       // 전단저항값               ,dVcr [Unit=Stress]
    double dRatioV;

    void Initialize()
    {
        dfcf = dFyw = 0.0;
        dRatioF = 0.0;
        dVcf = dVcf_lim = 0.0;
        dRatioV = 0.0;
    }

    T_RCFA_STRS() {Initialize();}
};

// Fatigue Load Case Stress
struct T_RCFA_LSTR_900
{
    double dStrT;
    double dStrB;

    void Initialzie()
    {
        dStrT = 0.0;
        dStrB = 0.0;
    }

    T_RCFA_LSTR_900() { Initialzie(); }
};


struct T_RCFA_LSTR
{
    double dStrT;
    double dStrB;

    double dStrT_Top;
    double dStrT_Bot;

    double dStrB_Top;
    double dStrB_Bot;

//   double dfcw;     //  복부판 응력
//   double dfcf;     //  압축플랜지 응력
//   double dftf;     //  인장플랜지 응력
//   BOOL   bIsTopC;  //  Top이 압축플랜지?
    
    void Initialzie()
    {
        dStrT = 0.0;
        dStrB = 0.0;
        dStrT_Top = 0.0;
        dStrT_Bot = 0.0;
        dStrB_Top = 0.0;
        dStrB_Bot = 0.0;
    }

    void Convert900(const T_RCFA_LSTR& rData)
    {
        dStrT = rData.dStrT;
        dStrB = rData.dStrB;
        dStrT_Top = 0.0;
        dStrT_Bot = 0.0;
        dStrB_Top = 0.0;
        dStrB_Bot = 0.0;
    }

    T_RCFA_LSTR() {Initialzie();}
};

struct T_RCFA_MLST_900
{
    double dStrT[2]; // Comp/Tens
    double dStrB[2]; // Comp/Tens
    double dStrDelT;
    double dStrDelB;

    void Initialzie()
    {
        memset(dStrT, 0, sizeof(dStrT));
        memset(dStrB, 0, sizeof(dStrB));
        dStrDelT = 0.0;
        dStrDelB = 0.0;
    }

    T_RCFA_MLST_900() { Initialzie(); }
};

struct T_RCFA_MLST
{
    double dStrT[2]; // Comp/Tens
    double dStrB[2]; // Comp/Tens
    double dStrDelT;
    double dStrDelB;

    double dStrDelT_Top;
    double dStrDelT_Bot;
    double dStrDelB_Top;
    double dStrDelB_Bot;

    void Initialzie()
    {
        memset(dStrT, 0, sizeof(dStrT));
        memset(dStrB, 0, sizeof(dStrB));
        dStrDelT = 0.0;
        dStrDelB = 0.0;
        dStrDelT_Top = 0.0;
        dStrDelT_Bot = 0.0;
        dStrDelB_Top = 0.0;
        dStrDelB_Bot = 0.0;
    }

    void Convert900(const T_RCFA_MLST_900& rData)
    {
        for (int i=0; i<2; i++ )
        {
            dStrT[i] = rData.dStrT[i];
            dStrB[i] = rData.dStrB[i];
        }
        dStrDelT = rData.dStrDelT;
        dStrDelB = rData.dStrDelB;

        dStrDelT_Top = 0.0;
        dStrDelT_Bot = 0.0;
        dStrDelB_Top = 0.0;
        dStrDelB_Bot = 0.0;
    }

    T_RCFA_MLST() {Initialzie();}
};

struct T_RATING_INF_FATI_LIFE
{
    // 0 : Top of Top Flange,    1 : Top of Bottom Flange, Bottom
    // 2 : Top of Bottom Flange, 3 : Bottom of Bottom Flange,
    BOOL bSkip[4];
    int nRatFactCase[4];  // 0: 강도로 인한 NG, 1 :  강도가 OK인 경우
    T_RCFA_LOAD LoadCapa[4];
    T_RFDT_B DiagnoTest[4];

    void Initialize()
    {
        memset(nRatFactCase, 0, sizeof(nRatFactCase));

        for ( int i=0; i<4; ++i )
        {
            bSkip[i] = FALSE;
            LoadCapa[i].Initialize();
            DiagnoTest[i].Initialize();
        }
    }
};

struct T_RATING_FIN_FATI_LIFE
{
    // 0 : Top of Top Flange,    1 : Top of Bottom Flange, Bottom
    // 2 : Top of Bottom Flange, 3 : Bottom of Bottom Flange,
    BOOL bOnlyInfinite[4];
    BOOL bRemainFatiLife[4];

    int nRatFactCase[4];  // 0: 강도로 인한 NG, 1 :  강도가 OK인 경우

    double dRs;
    double dRr[4];
    double dNav[4];
    double dDel_Fact_f_Tens[4]; //2.2 * f_tension

    int nCategory[4];

    //Fatigue Life
    double dY_Rem[4]; //Remaining Fatigue Life
    double dADTT_SL_Future[4];
    double dY_ADTT_Limit[4];
    double dY_Rem_Mod[4]; //Modified Remaining Fatigue Life
    double dY[4]; //Total Fatigue Life

    //Serviceability index
    double dGreaterY[4]; // Greater Y or 100 years
    double dSerIndex[4]; // Q in Standard

    T_RCFA_LOAD LoadCapa[4];

    void Initialize()
    {
        memset(nRatFactCase, 0, sizeof(nRatFactCase));
        for ( int i=0; i<4; ++i )
        {
            dRs = 0.0;
            bOnlyInfinite[i] = FALSE;
            bRemainFatiLife[i] = FALSE;
            dRr[i] = 0.0;
            dNav[i] = 0.0;
            dDel_Fact_f_Tens[i] = 0.0;
            nCategory[i] = 0;

            dY_Rem[i] = 0.0;
            dADTT_SL_Future[i] = 0.0;
            dY_ADTT_Limit[i] = 0.0;
            dY_Rem_Mod[i] = 0.0;
            dY[i] = 0.0;

            dGreaterY[i] = 0.0;
            dSerIndex[i] = 0.0;

            LoadCapa[i].Initialize();
        }
    }

    T_RATING_FIN_FATI_LIFE() { Initialize(); }
};

struct T_RCFA_BASE
{
    BOOL bChk;
    T_ELEM_K ElemK;
    UINT nPosi;              // 1=I-End, 2=J-END
    T_GRUP_K nGrupKey;			 // Group Key
    UINT RCaseK;
    BOOL bDCmax;             // DC(Max,Min Factor)                     [Unit=None]
    BOOL bDWmax;             // DW(Max,Min Factor)                     [Unit=None]
    BOOL bTEmax;             // Temperature(Max,Min Factor)            [Unit=None]
    UINT nConcurrent;        // Fx_max .... Mz_min(1~12)               [Unit=None] 
    UINT nPoint[2];					 //	[0]=comp,[1]=tens , 0=Left Top,1=RT,2=RB,3=LB
    UINT nIsLocation;        // Is Location Data 0=Non, 1=Exist        [Unit=None]
    T_SPAN_K SpanKey;        // Span Key                               [Unit=None]
    UINT nSpanPos;           // Span position                          [Unit=None]
    double dPhi_CF;          // Condition Factor                       [Unit=None]
    double dPhi_SF;          // System Factor                          [Unit=None]  
    double dPhi_RE;          // Resistance Factor                      [Unit=None]
    double drDC_BE;          // DC Factor Before                       [Unit=None]
    double drDC;             // DC Factor                              [Unit=None]
    double drDW;             // DW Factor                              [Unit=None]
    double drTE;             // Temperature Factor                     [Unit=None]
    double drTG;             // Temperature Gradient Factor            [Unit=None]
    double drP;              // Permanent Factor                       [Unit=None]
    double drSE;             // Secondary Factor                       [Unit=None]
    double drUS;             // User Factor                            [Unit=None]
    double drPR;             // Primary Vehicle Factor                 [Unit=None]
    double drAD;             // Adjacent Vehicle Factor                [Unit=None]

    T_RCFA_LSTR  StrDC_Be;
    T_RCFA_LSTR  StrLT;
    T_RCFA_LSTR  StrST;
    T_RCFA_MLST  StrPR;
    T_RCFA_MLST  StrAD;
    
    T_RCFA_LOAD  LoadR[2];   // Top,Bottom
    T_RCFA_STRS  StrsR;

    double dCaDe[2];     // Capacity-Demand 0 : Top, 1 : Bottom  [Unit=Stress]
    int    nRFcase[2];    // 0: 강도로 인한 NG, 1 :  강도가 OK인 경우

    double dRF_Load[2];      // 0=Top, 1=Bottom [Unit=None]
    double dRF_Strs[2];      // 0=휨응력검토, 1=전단응력 검토
    double dKRF_Load[2];
    double dRT;         // Safe Load Capacity(tons)               [Unit=None]
    double dSLCapa[2];       // 
    double dRFmin;
    double dKRFmin;
    int    nCrMinType;       // 0=Top, 1=Bottom
    double dRel_Locat;       // Relative Location                      [Unit=None]

    T_RFDT_B DtkS[2];        // 0=Top, 1=Bottom

    void Initialize()
    {
        bChk = 0;
        ElemK =0;
        nPosi = 0;
        nGrupKey = 0;
        RCaseK = 0;
        bDCmax = bDWmax = bTEmax = TRUE;
        nConcurrent = 1;
        nPoint[0] = nPoint[1] = 0;
        nIsLocation = 0;
        SpanKey = 0;
        nSpanPos = 0;
        dPhi_CF = dPhi_SF = dPhi_RE = 0.0;
        drDC_BE = drDC = drDW = drTE = drTG = drP = drSE = drUS = drPR = drAD = 0.0;   

        StrDC_Be.Initialzie();
        StrLT.Initialzie();
        StrST.Initialzie();
        StrPR.Initialzie();
        StrAD.Initialzie();
        
        for (int i=0; i<2; ++i) LoadR[i].Initialize();
        StrsR.Initialize();

        memset(dRF_Load,0,sizeof(dRF_Load));
        memset(dRF_Strs,0,sizeof(dRF_Strs));
        memset(dKRF_Load,0,sizeof(dKRF_Load));
        memset(dSLCapa,0,sizeof(dSLCapa));
        memset(dCaDe,0,sizeof(dCaDe));    
        memset(nRFcase,0,sizeof(nRFcase));

        dRT = 0.0;
        dRFmin = 0.0;
        dKRFmin = 0.0;
        dRel_Locat = 0.0;
        for (int i=0; i<2; ++i ) DtkS[i].Initialize();
    }
    
    T_RCFA_BASE() {Initialize();}
};

struct T_RCFA_CASE
{
    T_RCFA_BASE FatiB[2]; //0=I, 1=J
    
    void Initialize()
    {
        for(int i=0; i<2; ++i) FatiB[i].Initialize();
    }
    
    T_RCFA_CASE() {Initialize();}
};

struct T_RCFA_D
{
    T_RCCR_BASE FatiR[4]; // Load, 응력검토중 가장 불린한.. 
	                      // 0:Top-I, 1:Bottom-I
	                      // 2:Top-J, 3:Bottom-J  

    void Initialize()
    {
        for ( int i=0; i<4; ++i )
        {
            FatiR[i].Initialize();
        }
    }
    
    T_RCFA_D() {Initialize();}
};

struct T_RCFA_INF_BASE
{
    //For AASHTO LRFR19
    // 0 : Top of Top Flange,    1 : Top of Bottom Flange, Bottom
    // 2 : Top of Bottom Flange, 3 : Bottom of Bottom Flange,
    BOOL bChkPosi[4];
    BOOL bSkip[4];
    double dADTT_Present;
    double dSpan;
    double dnL;
    double dn;
    double dRs;
    double dRp;
    double dCapaDemand[4];  //[Unit=Stress]
    double dRatFact[4];  //[Unit=None]
    double dKRatFact_Load[4]; //[Unit=None]
    double dSafeLoadCapa[4];  // 

    T_RCFA_BASE DRCFA;
    T_RATING_INF_FATI_LIFE InfLife; //Infinite Fatigue
    int nCrMinTypeDetail; //0 = Top of Top Flange, 1 = Bot of Top Flange, 2 = Top of Bot Flange, 3 = Bot of Bot Flange

    void Initialize()
    {
        memset(dCapaDemand, 0, sizeof(dCapaDemand));
        memset(dRatFact, 0, sizeof(dRatFact));
        memset(dKRatFact_Load, 0, sizeof(dKRatFact_Load));
        memset(dSafeLoadCapa, 0, sizeof(dSafeLoadCapa));

        dADTT_Present = 0.0;
        dSpan = 0.0;
        dnL = 0.0;
        dn = 0.0;
        dRs = 0.0;
        dRp = 0.0;
        DRCFA.Initialize();
        InfLife.Initialize();
        nCrMinTypeDetail = 0;
        for ( int i=0; i<4; i++ )
        {
            bChkPosi[i] = FALSE;
            bSkip[i] = FALSE;
        }
    }

    T_RCFA_INF_BASE() { Initialize(); }
};

struct T_RCFA_FIN_BASE
{
    //For AASHTO LRFR19
    // 0 : Top of Top Flange,    1 : Top of Bottom Flange, Bottom
    // 2 : Top of Bottom Flange, 3 : Bottom of Bottom Flange,
    BOOL bChkPosi[4];
    BOOL bSkip[4];
    double dCur_Age; // 설계기준에서 a로 표시
    double dADTT_SL_0;
    double dADTT_SL_Present;
    double dADTT_SL_Limit;
    double dAnnualGrowth; // 설계기준에서 g로 표시
    double dADTT_Present;
    double dSpan;
    double dnL;
    double dRs;
    double dRp;
    double dN1; //Only For Fatigue Rating
    double dn; //Used in Dgn

    double dG;
    double dR;
    double dI;
    double dA[4];

    double dDelF[4]; //
    double dDelFeff[4]; //Only For Rating Factor;
    double dDelFmax[4]; //Only For Rating Factor;

    double dCapaDemand[4];  //[Unit=Stress]
    double dRatFact[4];  //[Unit=None]

    T_RCFA_BASE DRCFA;
    T_RATING_FIN_FATI_LIFE FinLife[4]; //Finite Fatigue //0 Minimum, 1 Eval1, 2 Eval2, 3 Mean
    int nCrMinTypeDetail; //0 = Top of Top Flange, 1 = Bot of Top Flange, 2 = Top of Bot Flange, 3 = Bot of Bot Flange

    void Initialize()
    {
        memset(dDelF, 0, sizeof(dDelF));
        memset(dDelFeff, 0, sizeof(dDelFeff));
        memset(dDelFmax, 0, sizeof(dDelFmax));
        memset(dCapaDemand, 0, sizeof(dCapaDemand));
        memset(dRatFact, 0, sizeof(dRatFact));
        memset(dA, 0, sizeof(dA));

        dCur_Age = 0.0; // 설계기준에서 a로 표시
        dADTT_SL_0 = 0.0;
        dADTT_SL_Present = 0.0;
        dADTT_SL_Limit = 0.0;
        dAnnualGrowth = 0.0;
        dADTT_Present = 0.0;
        dSpan = 0.0;
        dnL = 0.0;;
        dRs = 0.0;
        dRp = 0.0;
        dN1 = 0.0;
        dn = 0.0;
        dG = 0.0;
        dR = 0.0;
        dI = 0.0;

        DRCFA.Initialize();
        nCrMinTypeDetail = 0;

        for ( int j=0; j<4; ++j )
        {
            bChkPosi[j] = FALSE;
            FinLife[j].Initialize();
            bSkip[j] = FALSE;
        }
    }

    T_RCFA_FIN_BASE() { Initialize(); }
};

struct T_RCFA_INF_LIFE_CASE
{
    T_RCFA_INF_BASE FatiB[2]; //0=I, 1=J

    void Initialize()
    {
        for ( int i=0; i<2; ++i ) FatiB[i].Initialize();
    }

    T_RCFA_INF_LIFE_CASE() { Initialize(); }
};

struct T_RCFA_INF_LIFE_D //Infinite Life
{
    //I: 0,1,2,3 J 4,5,6,7
    T_RCCR_BASE FatiInfR[8]; //Load, 응력검토중 가장 불린한..Top of Top Flg, Bot of Top Flg, Top of Bot Flg, Bot of Bot Flg

    void Initialize()
    {
        for ( int i=0; i<8; ++i )
        {
            FatiInfR[i].Initialize();
        }
    }
};

struct T_RCFA_FIN_LIFE_CASE
{
    T_RCFA_FIN_BASE FatiB[2]; //0=I, 1=J

    void Initialize()
    {
        for ( int i=0; i<2; ++i ) FatiB[i].Initialize();
    }

    T_RCFA_FIN_LIFE_CASE() { Initialize(); }
};

struct T_RCFA_FIN_LIFE_D //Finite Life
{
    //I: 0,1,2,3 J 4,5,6,7
    T_RCCR_BASE FatiFinR[8]; //Load, 응력검토중 가장 불린한..Top of Top Flg, Bot of Top Flg, Top of Bot Flg, Bot of Bot Flg

    void Initialize()
    {
        for ( int i=0; i<4; ++i )
        {
            FatiFinR[i].Initialize();
        }
    }
};

// System Check : 설계 요소중 불리한 요소를 출력
struct T_RSCR_STRN
{
    T_RCCR_BASE FlexR;
    T_RCCR_BASE ShearR;

    void Initialize()
    {
        FlexR.Initialize();
        ShearR.Initialize();
    }

    T_RSCR_STRN() {Initialize();}
};

struct T_RSCR_SERV
{
    T_RCCR_BASE StressR;  
    
    void Initialize()
    {
        StressR.Initialize();    
    }
    
    T_RSCR_SERV() {Initialize();}
};

struct T_RSCR_FATI
{
    T_RCCR_BASE FatiR;  
    
    void Initialize()
    {
        FatiR.Initialize();
    }
    
    T_RSCR_FATI() {Initialize();}
};


struct T_SCGD_INT_STRESS
{
    double dStress_Ax;// Index 0; // 축응력
    double dStress_M_y[9]; // index : 1 ~ 10 //  강축 방향 휨응력
    double dStress_M_z[9]; // index : 11 ~ 19 //  약축 방향 휨응력

    double dStress_Ax_P2;// Index 0; // 축응력
    double dStress_M_y_P2[9]; // index : 1 ~ 10 //  강축 방향 휨응력
    double dStress_M_z_P2[9]; // index : 11 ~ 19 //  약축 방향 휨응력

    void Initialize()
    {
        dStress_Ax = 0.0;
        memset(dStress_M_y,0,sizeof(dStress_M_y));
        memset(dStress_M_z,0,sizeof(dStress_M_z));
        dStress_Ax_P2 = 0.0;
        memset(dStress_M_y_P2,0,sizeof(dStress_M_y_P2));
        memset(dStress_M_z_P2,0,sizeof(dStress_M_z_P2));
    }

    T_SCGD_INT_STRESS() {Initialize();}  
};


struct T_SCGD_DGN_FORCE_UNIT
{
    T_CSGD_FORCE ForBC;  //비합성, Before Composite
    T_CSGD_FORCE ForAC;  //합성, After Composite
    T_CSGD_FORCE ForceAfterDL; // After Only DL
    T_CSGD_FORCE ForceAfer_Crp;  // After  Only Creep
    T_CSGD_FORCE ForceAfer_Shr;  // After  Only Shrinkage
    T_CSGD_FORCE ForceAfer_BST; // After Only Beam Section Temperature

    T_SCGD_INT_STRESS InterStrCrp; // internal Stress of Creep
    T_SCGD_INT_STRESS InterStrShr;  // internal Stress of Shringkage
    T_SCGD_INT_STRESS InterStrBST; // Internal Stress of Beam Sectiion Temperature

    void Initialize()
    {
        ForBC.Initialize();
        ForAC.Initialize();  
        ForceAfterDL.Initialize();
        ForceAfer_Crp.Initialize();
        ForceAfer_Shr.Initialize();
        ForceAfer_BST.Initialize();
        InterStrCrp.Initialize();
        InterStrShr.Initialize();
        InterStrBST.Initialize();
    }

    T_SCGD_DGN_FORCE_UNIT() {Initialize();}  
};

struct T_SCGD_DGN_FORCE
{
    T_SCGD_DGN_FORCE_UNIT Elem[2];

    void Initialize()
    {
        for(int i =0; i <2; ++i) Elem[i].Initialize();
    }

    T_SCGD_DGN_FORCE() {Initialize();}  
};

//////////////////////////////////////////////////////////////////////////
// CSA-S6-14, Steel Composite

struct T_CLASS_WTR
{
    int nTopCL;
    int nBottomCL;
    int nWebCL;
    int nClass;

    void Initialize()
    {
        nTopCL    = 0;
        nBottomCL = 0;
        nWebCL    = 0;
        nClass    = 0;
    }
    T_CLASS_WTR () {Initialize();}
};

// 시공단계 휨검토 CSA CS Bending Resistance
#define T_CCBR_K unsigned int
struct T_CCBR_BASE 
{
    BOOL bChk;
    UINT nStageK;     // Stage              [Unit=None]
    UINT nStepK;      // Step               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]

    T_CSGD_ULMF  UlFor;
    double dMax;         // Member Mmax
    T_CSGD_FORCE CsFor;

    T_CLASS_WTR ClassD;

    double dCr;
    double dMry;
    double dMrz;

    double dComb1;     // (a) 10.9.4 Axial compression and bending
    double dComb2;     // (b)
    double dComb3;     // (c)
    double dRatio;
    int    nOK;

    void Initialize()
    {
        bChk = FALSE;
        nStageK = 0;
        nStepK  = 0;
        nOrgLcomK = 0;

        UlFor.Initialize();
        dMax = 0.0;
        CsFor.Initialize();

        ClassD.Initialize();

        dCr  = 0.0;
        dMry = 0.0;
        dMrz = 0.0;
        dComb1 = 0.0;
        dComb2 = 0.0;
        dComb3 = 0.0;

        dRatio = 0.0;
        nOK = 0;
    }

    T_CCBR_BASE() { Initialize(); }
};

struct T_CCBR_D
{
    T_CCBR_BASE CcbrB[4];   // 0: I-정, 1: I-부, 2: J-정, 3: J-부, 

    void Initialize()
    {
        for (int i=0; i<4; ++i) CcbrB[i].Initialize();
    }

    T_CCBR_D() { Initialize(); }
};

struct T_CCBR_UNIT
{
    T_CCBR_UNIT()
    {
        dCr  = D_UNITSYS_BASE_FORCE;
        dMry = D_UNITSYS_BASE_MOMENT;
        dMrz = D_UNITSYS_BASE_MOMENT;
    }
    int dCr;
    int dMry;
    int dMrz;

};

// 시공단계 전단검토 CSA CS Shear Resistance
#define T_CCVR_K unsigned int
struct T_CCVR_BASE 
{
    BOOL bChk;
    UINT nStageK;     // Stage              [Unit=None]
    UINT nStepK;      // Step               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]

    T_CSGD_ULMF  UlFor;
    double dMax;         // Member Mmax
    T_CSGD_FORCE CsFor;

    T_CLASS_WTR ClassD;

    double dVu;      // 설계전단력    (Vu < Vr 일 때 OK)
    double dVr;      // 설계전단강도
    double dMr;  

    double dRatio_v;  // Vf/Vr
    double dRatio_m;  // Mf/Mr
    double dRatio_c;  // 0.727*Mf/Mr + 0.455*Vf/Vr<1.0
    double dRatio;
    int    nOK;

    void Initialize()
    {
        bChk = FALSE;
        nStageK = 0;
        nStepK  = 0;
        nOrgLcomK = 0;

        UlFor.Initialize();
        dMax = 0.0;
        CsFor.Initialize();

        ClassD.Initialize();

        dVu      = 0.0;      
        dVr      = 0.0;     
        dMr      = 0.0;
        
        dRatio_v = 0.0;
        dRatio_m = 0.0;
        dRatio_c = 0.0;
        dRatio   = 0.0;
        nOK = 0;
    }

    T_CCVR_BASE() { Initialize(); }
};

struct T_CCVR_D
{
    T_CCVR_BASE CcvrB[2];

    void Initialize()
    {
        for (int i=0; i<2; ++i) CcvrB[i].Initialize();
    }

    T_CCVR_D() { Initialize(); }
};

struct T_CCVR_UNIT
{
    T_CCVR_UNIT()
    {
        dVu      = D_UNITSYS_BASE_FORCE;      
        dVr      = D_UNITSYS_BASE_FORCE;      
        dMr      = D_UNITSYS_BASE_MOMENT;
    }

    int dVu;      
    int dVr;   
    int dMr;
};

// 휨강도 CSA 
#define T_CPBR_K unsigned int
struct T_CPBR_BASE 
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    BOOL bChkUlmf;
    double dMax;         // Member Mmax
    T_CSGD_ULMF  UlForLC;
    T_CSGD_ULMF  UlForNC;
    T_CSGD_ULMF  UlForLT;
    T_CSGD_ULMF  UlForST;

    T_CSGD_COMF  ComFor;

    T_CLASS_WTR ClassD;

    double dMy;  // 항복모멘트
    double dMp;  // 소성모멘트
    double dMu;  // 설계모멘트
    double dMr;  // 설계휨강도    
    double dMfw;
    double dMrz;
    //
    double dMu_sta;
    double dMr_sta;
    double dMfw_sta;
    double dMrz_sta;

    double dMrat;
    double dMrat_sta;
    double dMu_Mr_rat;
    double dMfw_Mrz_rat;
    double dMu_Mr_sta_rat;
    double dMfw_Mrz_sta_rat;
    int    nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax = 0;

        bChkUlmf = FALSE;
        dMax = 0.0;
        UlForLC.Initialize();
        UlForNC.Initialize();
        UlForLT.Initialize();
        UlForST.Initialize();
        ComFor.Initialize();

        ClassD.Initialize();

        dMy = 0.0;  
        dMp = 0.0;
        dMu = 0.0;
        dMr = 0.0;
        dMfw = 0.0;
        dMrz  = 0.0;
        dMu_sta = 0.0;
        dMr_sta = 0.0;
        dMfw_sta = 0.0;
        dMrz_sta = 0.0;

        dMrat = 0.0;
        dMrat_sta = 0.0;
        dMu_Mr_rat = 0.0;
        dMfw_Mrz_rat = 0.0;
        dMu_Mr_sta_rat = 0.0;
        dMfw_Mrz_sta_rat = 0.0;
        nOK   = 0;
    }

    T_CPBR_BASE() { Initialize(); }
};

struct T_CPBR_D
{
    T_CPBR_BASE CpbrB[4];

    void Initialize()
    {
        for (int i=0; i<4; ++i) CpbrB[i].Initialize();
    }

    T_CPBR_D() { Initialize(); }
};

struct T_CPBR_UNIT
{
    T_CPBR_UNIT()
    {
        dMy   = D_UNITSYS_BASE_MOMENT;
        dMp   = D_UNITSYS_BASE_MOMENT;
        dMu   = D_UNITSYS_BASE_MOMENT;
        dMr   = D_UNITSYS_BASE_MOMENT;
    }
    int dMy;
    int dMp;
    int dMu;
    int dMr;
};

// 전단강도
#define T_CPVR_K unsigned int
struct T_CPVR_BASE 
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    T_CSGD_ULMF  UlFor;
    double dMax;         // Member Mmax
    T_CSGD_COMF  ComFor;

    T_CLASS_WTR ClassD;

    double dVu;
    double dVr;
    double dMf;
    double dMr;
    double dMfw;
    double dMrz;
    double dMu_sta;
    double dMr_sta;
    double dMfw_sta;
    double dMrz_sta;

    double dRatio_v;  // Vf/Vr
    double dRatio_m;  // Mf/Mr
    double dRatio_c;  // 0.727*Mf/Mr + 0.455*Vf/Vr<1.0
    double dRatio_c_sta;
    double dMfw_Mrz_rat;      // Mfw/Mrz
    double dMu_Mr_sta_rat;    // Mf/Mr stability
    double dMfw_Mrz_sta_rat;  // Mfw/Mrz stability

    double dVrat;
    double dVrat_sta;
    int    nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax = 0;

        UlFor.Initialize();
        dMax = 0.0;
        ComFor.Initialize();

        ClassD.Initialize();

        dVu      = 0.0;      
        dVr      = 0.0;
        dMf      = 0.0;
        dMr      = 0.0;
        dMfw     = 0.0;
        dMrz     = 0.0;
        dMu_sta = 0.0;
        dMr_sta = 0.0;
        dMfw_sta = 0.0;
        dMrz_sta = 0.0;
        dRatio_v = 0.0;
        dRatio_m = 0.0;
        dRatio_c = 0.0;
        dRatio_c_sta = 0.0;
        dMfw_Mrz_rat = 0.0;
        dMu_Mr_sta_rat = 0.0;
        dMfw_Mrz_sta_rat = 0.0;

        dVrat    = 0.0;
        dVrat_sta = 0.0;
        nOK = 0;
    }

    T_CPVR_BASE() { Initialize(); }
};

struct T_CPVR_D
{
    T_CPVR_BASE CpvrB[2];

    void Initialize()
    {
        for (int i=0; i<2; ++i) CpvrB[i].Initialize();
    }

    T_CPVR_D() { Initialize(); }
};

struct T_CPVR_UNIT
{
    T_CPVR_UNIT()
    {
        dVu      = D_UNITSYS_BASE_FORCE;      
        dVr      = D_UNITSYS_BASE_FORCE;      
        dMf      = D_UNITSYS_BASE_MOMENT;
        dMr      = D_UNITSYS_BASE_MOMENT;
    }
    int dVu;      
    int dVr;      
    int dMf;
    int dMr;
};

// 사용성 검토
#define T_CPAC_K unsigned int
struct T_CPGS_BASE
{
    double dfcw;     //  복부판 응력
    double dfcf;     //  압축플랜지 응력
    double dftf;     //  인장플랜지 응력
    BOOL   bIsTopC;  //  Top이 압축플랜지?

    void Initialzie()
    {
        dfcw     = 0.0;       
        dfcf     = 0.0;       
        dftf     = 0.0;
        bIsTopC  = TRUE;
    }

    T_CPGS_BASE() {Initialzie();}
};
struct T_CPAC_BASE 
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    T_CSGD_ULMF  UlFor;
    double dMax;         // Member Mmax
    T_CSGD_COMF  ComFor;

    T_CLASS_WTR ClassD;

    double dStress; // 10.11.4 Control of permanent deflection
    double d09Fy;   //0.9*Fy, 
    double dRat;
    double dS;      // girder
    double dS3n;    // SC long term
    double dSn;     // SC short term
    double dSp;     // SC rebar (negative)

    int    nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax = 0;

        UlFor.Initialize();
        dMax = 0.0;
        ComFor.Initialize();

        ClassD.Initialize();

        dStress = 0.0;
        d09Fy   = 0.0;
        dRat    = 0.0;
        dS      = 0.0;       
        dS3n    = 0.0;   
        dSn     = 0.0;       
        dSp     = 0.0;   
        nOK     = 0;
    }

    T_CPAC_BASE() { Initialize(); }
};

struct T_CPAC_D
{
    T_CPAC_BASE CpacB[4];

    void Initialize()
    {
        for (int i=0; i<4; ++i) CpacB[i].Initialize();
    }

    T_CPAC_D() { Initialize(); }
};

struct T_CPAC_UNIT
{
    T_CPAC_UNIT()
    {
        dStress = D_UNITSYS_BASE_STRESS;       
        d09Fy   = D_UNITSYS_BASE_STRESS;   
        dS      = D_UNITSYS_BASE_VOLUME;       
        dS3n    = D_UNITSYS_BASE_VOLUME;   
        dSn     = D_UNITSYS_BASE_VOLUME;       
        dSp     = D_UNITSYS_BASE_VOLUME;   
    }
    int dStress;       
    int d09Fy;   
    int dS;       
    int dS3n;   
    int dSn;
    int dSp;         
};


// 피로검토
#define T_CPFC_K unsigned int
struct T_CPFC_LOAD
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]

    T_CSGD_COMF_MV  ComForMV;

    T_CLASS_WTR ClassD;

    double dfsr;
    double dStress_fa; // 0.52*C_L*fsr ?
    double dFsr;

    double dRatio;
    int    nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        ComForMV.Initialize();
        ClassD.Initialize();
        dfsr       = 0.0;
        dStress_fa = 0.0;
        dFsr       = 0.0;
        dRatio     = 0.0;
        nOK = 0;
    }
    T_CPFC_LOAD() {Initialize();}
};

//피로검토 StudWeld
struct T_CPFC_STUD 
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    T_CSGD_COMF_MV  ComForMV;
    T_CSGD_COMF  ComFor;

    double dfsr;
    double dStress_fa; // 0.52*C_L*fsr ?
    double dFsr;

    double dfcf;           // 복부판 최대전단탄성응력
    double dFyw;           // 복부항복강도
    double dRatio;
    int    nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax = 0;
        ComFor.Initialize();
        ComForMV.Initialize();
        dfsr       = 0.0;
        dStress_fa = 0.0;
        dFsr       = 0.0;
        dfcf=0.0;
        dFyw=0.0;
        dRatio = 0.0;
        nOK=0;
    }
    T_CPFC_STUD() {Initialize();}

};
struct T_CPFC_FLEX_UNIT
{
    T_CPFC_FLEX_UNIT()
    {
        dfsr       = D_UNITSYS_BASE_STRESS;
        dStress_fa = D_UNITSYS_BASE_STRESS;
        dFsr       = D_UNITSYS_BASE_STRESS;
        dfcf       = D_UNITSYS_BASE_STRESS;
        dFyw       = D_UNITSYS_BASE_STRESS;
    }

    int dfsr;
    int dStress_fa;
    int dFsr;
    int dfcf;
    int dFyw;  
};

struct T_CPFC_SHEAR 
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    T_CSGD_COMF  ComFor;

    double dVcf;           // 복부판 최대전단탄성응력  ,dVu
    double dVcf_lim;       // 전단저항값               ,dVcr
    double dRatio;
    int    nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax =0;
        ComFor.Initialize();
        dVcf=0.0;
        dVcf_lim=0.0;
        dRatio = 0.0;
        nOK=0;
    }
    T_CPFC_SHEAR() {Initialize();}

};
struct T_CPFC_SHEAR_UNIT
{
    T_CPFC_SHEAR_UNIT()
    {
        dVcf       = D_UNITSYS_BASE_FORCE;
        dVcf_lim   = D_UNITSYS_BASE_FORCE;
    }

    int dVcf;
    int dVcf_lim;  
};

struct T_CPFC_SCONN
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    T_CSGD_COMF_MV ComForMV;
    T_CSGD_COMF  FatiForMax;
    T_CSGD_COMF  FatiForMin;

    double dtau_rs;
    double dVsc;
    double dFsr_D;
    double dRatio;
    int    nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax =0;
        ComForMV.Initialize();
        FatiForMax.Initialize();
        FatiForMin.Initialize();
        dtau_rs  = 0.0;
        dVsc     = 0.0;
        dFsr_D   = 0.0;
        dRatio   = 0.0;
        nOK=0;
    }
    T_CPFC_SCONN() { Initialize(); }
};

struct T_CPFC_SCONN_UNIT
{
    T_CPFC_SCONN_UNIT()
    {
        dtau_rs  = D_UNITSYS_BASE_STRESS;
        dVsc     = D_UNITSYS_BASE_FORCE;
        dFsr_D   = D_UNITSYS_BASE_STRESS;
    }
    int dtau_rs;
    int dVsc;  
    int dFsr_D;
};


struct T_CPFC_BASE 
{
    BOOL bChk;

    T_CPFC_LOAD  LoadR[2]; // flange Top,Bottom
    T_CPFC_STUD  StudR;   // top flange top surface - stud welded
    T_CPFC_SHEAR ShearR;
    T_CPFC_SCONN SConnR;

    double dFrat;
    int    nOK;

    void Initialize()
    {
        bChk = FALSE;
        for (int i=0; i<2; ++i) LoadR[i].Initialize();
        StudR.Initialize();
        ShearR.Initialize();
        SConnR.Initialize();
        dFrat      = 0.0;
        nOK = 0;
    }

    T_CPFC_BASE() { Initialize(); }
};

struct T_CPFC_D
{
    T_CPFC_BASE CpfcB[2]; // I,J

    void Initialize()
    {
        for (int i=0; i<2; ++i) CpfcB[i].Initialize();
    }

    T_CPFC_D() { Initialize(); }
};

// 전단연결재 검토
//#define T_CPSC_K unsigned int
struct T_CASC_BASE 
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    double dhdr;    // h/d
    double dqr;
    double dN;
    double dN_req;
    double dSCrat;
    int  nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax = 0;

        dhdr   = 0.0;
        dqr    = 0.0;
        dN     = 0.0;
        dN_req = 0.0;
        dSCrat = 0.0;
        nOK = 0;
    }

    T_CASC_BASE() { Initialize(); }
};

struct T_CASC_D
{
    T_CASC_BASE CascB[2];

    void Initialize()
    {
        for (int i=0; i<2; ++i) CascB[i].Initialize();
    }

    T_CASC_D() { Initialize(); }
};

struct T_CASC_UNIT
{
    T_CASC_UNIT()
    {
        dqr = D_UNITSYS_BASE_FORCE;
    }
    int dqr;

};

// 수평보강재
#define T_CPTS_K unsigned int
struct T_CPLS_BASE
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]
    
    double da;         // transverse stiffener spacing,  10.10.7 Longitudinal web stiffeners
    double dhp;        // Maximum subpanel depth
    double da_1p5hp;   // a/(1.5*hp)
    double dwtr;       // w/t
    double dwtr_lim;   // 200/sqrt(Fy)
    double dhpl;       // the projecting stiffener width
    double d30t;       // the projecting stiffener width is less than or equal to 30t
    double dI_ls;
    double dI_ls_lim;
    double dr;
    double dr_lim;
    double dRat;
    int    nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax = 0;

        da       = 0.0;
        dhp      = 0.0;
        da_1p5hp = 0.0;
        dwtr     = 0.0;
        dwtr_lim = 0.0;
        dhpl     = 0.0;
        d30t     = 0.0;
        dI_ls    = 0.0;
        dI_ls_lim= 0.0;
        dr       = 0.0;
        dr_lim   = 0.0;
        dRat     = 0.0;
        nOK      = 0;
    }
    T_CPLS_BASE() { Initialize(); }

};

struct T_CPLS_D
{
    T_CPLS_BASE CplsB[2];

    void Initialize()
    {
        for (int i=0; i<2; ++i) CplsB[i].Initialize();
    }

    T_CPLS_D() { Initialize(); }
};

struct T_CPLS_UNIT
{
    T_CPLS_UNIT()
    {
        da        = D_UNITSYS_BASE_LENGTH;
        dhp       = D_UNITSYS_BASE_LENGTH;
        dhpl      = D_UNITSYS_BASE_LENGTH;
        d30t      = D_UNITSYS_BASE_LENGTH;
        dI_ls     = D_UNITSYS_BASE_STIF;
        dI_ls_lim = D_UNITSYS_BASE_STIF;
        dr        = D_UNITSYS_BASE_LENGTH;
        dr_lim    = D_UNITSYS_BASE_LENGTH;
    }
    int da;
    int dhp;
    int dhpl;
    int d30t;
    int dI_ls;
    int dI_ls_lim;
    int dr;
    int dr_lim;
};

struct T_CPTS_BASE
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    double dhwr;           // web, h/w
    double dhwr_lim;       // 10.10.6.1 
    double da;
    double da_lim;         // The distance between stiffeners, a, shall not exceed 67 500h/(h/w)2 when h/w is greater than
                                                 // 150 and shall not exceed 3h when h/w is less than or equal to 150.
    double dAs;
    double dht;            // ???
    double dht_lim;        // ??? The width of a plate used as a stiffener shall not be less than 50 mm plus h/30 and shall not be less
                                                 // than one-quarter of the full width of the flange
    double dIt;
    double dIt_lim;
    double dVf;
    double dVr;
    double dwtr;           // The width-to-thickness ratio of intermediate transverse stiffeners
    double dwtr_lim;
    double dwp;             // The projecting stiffener width
    double d30t;
    double dRat;
    int    nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK   = 0; 
        nOrgLcomK= 0;
        nMax     = 0; 
        dhwr     = 0.0;
        dhwr_lim = 0.0;
        da       = 0.0;
        da_lim   = 0.0;
        dAs      = 0.0;
        dht      = 0.0;
        dht_lim  = 0.0;
        dIt      = 0.0;
        dIt_lim  = 0.0;    
        dVf      = 0.0;
        dVr      = 0.0;
        dwtr     = 0.0;
        dwtr_lim = 0.0;
        dwp      = 0.0;
        d30t     = 0.0;
        dRat     = 0.0;
        nOK      = 0;
    }
    T_CPTS_BASE () {Initialize();}
};

struct T_CPTS_D
{
    T_CPTS_BASE CptsB[2]; // 0=I, 1=J

    void Initialize()
    {
        for (int i=0; i<2; ++i) CptsB[i].Initialize();
    }

    T_CPTS_D() { Initialize(); }
};

struct T_CPTS_UNIT
{
    T_CPTS_UNIT()
    {
        da      = D_UNITSYS_BASE_LENGTH;
        da_lim  = D_UNITSYS_BASE_LENGTH;
        dAs     = D_UNITSYS_BASE_AREA;
        dht     = D_UNITSYS_BASE_LENGTH;
        dht_lim = D_UNITSYS_BASE_LENGTH;
        dIt     = D_UNITSYS_BASE_STIF;
        dIt_lim = D_UNITSYS_BASE_STIF;
        dVf     = D_UNITSYS_BASE_FORCE;
        dVr     = D_UNITSYS_BASE_FORCE;
        dwp     = D_UNITSYS_BASE_LENGTH;
        d30t    = D_UNITSYS_BASE_LENGTH;    
    }
    int da;
    int da_lim;
    int dAs;
    int dht;
    int dht_lim;
    int dIt;
    int dIt_lim;
    int dVf;
    int dVr;
    int dwp;
    int d30t;
    
};

// Bearing Stiffener 검토
struct T_CPBS_BASE 
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    double dWTR;
    double dWTRLimit;
    double dReact;
    double dBr;
    double dCr;
    double dRatio;
    int    nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax = 0;

		dWTR = 0.0;
		dWTRLimit = 0.0;
		dReact = 0.0;
		dBr = 0.0;
		dCr = 0.0;
		dRatio = 0.0;
        nOK    = 0;
    }

    T_CPBS_BASE() { Initialize(); }
};

struct T_CPBS_D
{
    T_CPBS_BASE CpbsB[2];

    void Initialize()
    {
        for (int i=0; i<2; ++i) CpbsB[i].Initialize();
    }

    T_CPBS_D() { Initialize(); }
};

struct T_CPBS_UNIT
{
    T_CPBS_UNIT()
    {
        dReact = D_UNITSYS_BASE_FORCE;
        dBr    = D_UNITSYS_BASE_FORCE;
        dCr    = D_UNITSYS_BASE_FORCE;            
    }
    int dReact;
    int dBr;
    int dCr;    
};


struct T_CSAR_BASE
{
    BOOL bChk;
    int  nOK;

    T_CPBR_BASE  CpbrB[2];  // 휨 0=Positive, 1=Negative
    T_CPVR_BASE  CpvrB;     // 전단
    T_CPAC_BASE  CpacB[2];  // 사용한계, 0=Positive, 1=Negative
    T_CPFC_BASE  CpfcB;     // 피로한계
    T_CASC_BASE  CascB;     // 전단연결재
    T_CPLS_BASE  CplsB;     // 수평보강재
    T_CPTS_BASE  CptsB;     // 수직보강재
    T_CPBS_BASE  CpbsB;     // Bearing Stiffener
    T_CCBR_BASE  CcbrB[2];  // 시공단계(휨)  정/부가 필요한가?
    T_CCVR_BASE  CcvrB;     // 시공단계(전단)

    void Initialize()
    { 
        bChk = FALSE;
        nOK =1;

        for (int i=0; i<2; ++i)
        {
            CpbrB[i].Initialize();
            CpacB[i].Initialize();
            CcbrB[i].Initialize();
        }
        CpvrB.Initialize();
        CpfcB.Initialize();
        CascB.Initialize();
        CplsB.Initialize();
        CptsB.Initialize();
        CpbsB.Initialize();
        CcvrB.Initialize(); 
    }
    T_CSAR_BASE() { Initialize(); }


};

// CS 457 R1 (BS5400-5-1979)
// 휨
struct T_BSBR_BASE
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    T_CSGF_MEMB ForD;

    double dMy;  // 항복모멘트
    double dMp;  // 소성모멘트
    double dMu;  // 설계모멘트
    double dMr;  // 설계휨강도

    double dRatio;
    int    nOK;
    
    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax = 0;
        ForD.Initialize();
        dMy = 0.0;
        dMp = 0.0;
        dMu = 0.0;
        dMr = 0.0;

        dRatio = 0.0;
        nOK = 0;
    }
    T_BSBR_BASE() { Initialize(); }
};

struct T_BSBR_D
{
    T_BSBR_BASE BsbrB[4];

    void Initialize()
    {
        for ( int i=0; i<4; ++i ) BsbrB[i].Initialize();
    }

    T_BSBR_D() { Initialize(); }
};

struct T_BSBR_UNIT
{
    T_BSBR_UNIT()
    {
        dMy   = D_UNITSYS_BASE_MOMENT;
        dMp   = D_UNITSYS_BASE_MOMENT;
        dMu   = D_UNITSYS_BASE_MOMENT;
        dMr   = D_UNITSYS_BASE_MOMENT;
    }
    int dMy;
    int dMp;
    int dMu;
    int dMr;
};

// 전단
struct T_BSVR_BASE
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    T_CSGF_MEMB ForD;

    double dVu;  // 설계전단력
    double dVr;  // 설계휨강도

    double dRatio;
    int    nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax = 0;
        ForD.Initialize();
        dVu = 0.0;
        dVr = 0.0;
        dRatio = 0.0;
        nOK = 0;
    }
    T_BSVR_BASE() { Initialize(); }
};

struct T_BSVR_D
{
    T_BSVR_BASE BsvrB[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i ) BsvrB[i].Initialize();
    }

    T_BSVR_D() { Initialize(); }
};

struct T_BSVR_UNIT
{
    T_BSVR_UNIT()
    {
        dVu   = D_UNITSYS_BASE_FORCE;
        dVr   = D_UNITSYS_BASE_FORCE;
    }
    int dVu;
    int dVr;
};

// Torsion
struct T_BSTR_BASE
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    T_CSGD_COMF  MembFor[3]; //0=I, 1=M, 2=J
    T_CSGD_COMF  ChkFor;
    T_CSGD_FORCE MVFor;

    double dTu;  // 설계전단력
    double dTr;  // 설계휨강도
    double dSigma_t;

    double dRatio;
    int    nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax = 0;
        for ( int i=0; i<3; ++i )
        {
            MembFor[i].Initialize();
        }
        ChkFor.Initialize();
        MVFor.Initialize();
        dTu = 0.0;
        dTr = 0.0;
        dSigma_t = 0.0;
        dRatio = 0.0;
        nOK = 0;
    }
    T_BSTR_BASE() { Initialize(); }
};

struct T_BSTR_D
{
    T_BSTR_BASE BstrB[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i ) BstrB[i].Initialize();
    }

    T_BSTR_D() { Initialize(); }
};

struct T_BSTR_UNIT
{
    T_BSTR_UNIT()
    {
        dTu      = D_UNITSYS_BASE_MOMENT;
        dTr      = D_UNITSYS_BASE_MOMENT;
        dSigma_t = D_UNITSYS_BASE_STRESS;
    }
    int dTu;
    int dTr;
    int dSigma_t;
};

// Longitudinal Shear
struct T_BSLS_BASE
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    T_CSGF_MEMB ForD;

    double dQu;  // 설계전단력 ???
    double dQr;  // 설계휨강도
    double dqp;  //
    double dPu;

    double dRatio;
    int    nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax = 0;
        ForD.Initialize();
        dQu = 0.0;
        dQr = 0.0;
        dqp = 0.0;
        dPu = 0.0;
        dRatio = 0.0;
        nOK = 0;
    }
    T_BSLS_BASE() { Initialize(); }
};

struct T_BSLS_D
{
    T_BSLS_BASE BslsB[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i ) BslsB[i].Initialize();
    }

    T_BSLS_D() { Initialize(); }
};

struct T_BSLS_UNIT
{
    T_BSLS_UNIT()
    {
        dQu   = D_UNITSYS_BASE_FORCE;
        dQr   = D_UNITSYS_BASE_FORCE;
        dqp   = D_UNITSYS_BASE_STRESS;
        dPu   = D_UNITSYS_BASE_STRESS;
    }
    int dQu;
    int dQr;
    int dqp;
    int dPu;
};

// SLS
struct T_BSSC_BASE
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    T_CSGF_MEMB ForD;

    double dSigma_w;       //  복부판 응력           
    double dSigma_w_lim;   //  복부판의 공칭 휨저항
    double dSigma_cf;      //  압축플랜지 응력       
    double dSigma_cf_lim;  //  압축플랜지 응력제한
    double dSigma_tf;      //  인장플랜지 응력       
    double dSigma_tf_lim;  //  인장플랜지 응력제한
    double dSigma_ct;      //  Slab
    double dSigma_cb;
    double dSigma_c_lim;   //  Slab 응력제한
    double dSigma_r;       //  철근
    double dSigma_r_lim;   //  철근 응력제한

    double dSigma_w_m;     //  복부판 응력 MV
    double dSigma_cf_m;
    double dSigma_tf_m;
    double dSigma_ct_m;      //  Slab
    double dSigma_cb_m;
    double dSigma_r_m;

    double dRatio;
    int    nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax = 0;
        ForD.Initialize();
        dSigma_w      = 0.0;
        dSigma_w_lim  = 0.0;
        dSigma_cf     = 0.0;
        dSigma_cf_lim = 0.0;
        dSigma_tf     = 0.0;
        dSigma_tf_lim = 0.0;
        dSigma_ct     = 0.0;
        dSigma_cb     = 0.0;
        dSigma_c_lim  = 0.0;
        dSigma_r      = 0.0;
        dSigma_r_lim  = 0.0;

        dSigma_w_m    = 0.0;
        dSigma_cf_m   = 0.0;
        dSigma_tf_m   = 0.0;
        dSigma_ct_m   = 0.0;
        dSigma_cb_m   = 0.0;
        dSigma_r_m    = 0.0;
        dRatio = 0.0;
        nOK = 0;
    }
};

struct T_BSSC_D
{
    T_BSSC_BASE BsscB[4];

    void Initialize()
    {
        for ( int i=0; i<4; ++i ) BsscB[i].Initialize();
    }

    T_BSSC_D() { Initialize(); }
};

struct T_BSSC_UNIT
{
    T_BSSC_UNIT()
    {
        dSigma_w      = D_UNITSYS_BASE_STRESS;
        dSigma_w_lim  = D_UNITSYS_BASE_STRESS;
        dSigma_cf     = D_UNITSYS_BASE_STRESS;
        dSigma_cf_lim = D_UNITSYS_BASE_STRESS;
        dSigma_tf     = D_UNITSYS_BASE_STRESS;
        dSigma_tf_lim = D_UNITSYS_BASE_STRESS;
        dSigma_ct     = D_UNITSYS_BASE_STRESS;
        dSigma_cb     = D_UNITSYS_BASE_STRESS;
        dSigma_c_lim  = D_UNITSYS_BASE_STRESS;
        dSigma_r      = D_UNITSYS_BASE_STRESS;
        dSigma_r_lim  = D_UNITSYS_BASE_STRESS;

        dSigma_w_m    = D_UNITSYS_BASE_STRESS;
        dSigma_cf_m   = D_UNITSYS_BASE_STRESS;
        dSigma_tf_m   = D_UNITSYS_BASE_STRESS;
        dSigma_ct_m   = D_UNITSYS_BASE_STRESS;
        dSigma_cb_m   = D_UNITSYS_BASE_STRESS;
        dSigma_r_m    = D_UNITSYS_BASE_STRESS;
    }
    int dSigma_w;
    int dSigma_w_lim;
    int dSigma_cf;   
    int dSigma_cf_lim;
    int dSigma_tf;    
    int dSigma_tf_lim;
    int dSigma_ct;    
    int dSigma_cb;
    int dSigma_c_lim; 
    int dSigma_r;    
    int dSigma_r_lim;

    int dSigma_w_m;  
    int dSigma_cf_m;
    int dSigma_tf_m;
    int dSigma_ct_m; 
    int dSigma_cb_m;
    int dSigma_r_m;
};

// Crack
struct T_BSCW_BASE
{
    BOOL bChk;
    UINT nLcomK;           // LcomK               [Unit=None]
    UINT nOrgLcomK;        // LcomK               [Unit=None]
    int  nMax;             //                     [Unit=None]

    T_CSGF_MEMB ForD;

    double dw;
    double dwa;
    double dfc_t;
    double dfc_b;
    double dfr;

    double dRatio;
    int    nOK;

    void Initialize()
    {
        bChk = FALSE;
        nLcomK = 0;
        nOrgLcomK = 0;
        nMax = 0;
        ForD.Initialize();
        dw    = 0.0;
        dwa   = 0.0;
        dfc_t = 0.0;
        dfc_b = 0.0;
        dfr   = 0.0;

        dRatio = 0.0;
        nOK = 0;
    }
    T_BSCW_BASE() { Initialize(); }
};

struct T_BSCW_D
{
    T_BSCW_BASE BscwB[4];

    void Initialize()
    {
        for ( int i=0; i<4; ++i ) BscwB[i].Initialize();
    }

    T_BSCW_D() { Initialize(); }
};

struct T_BSCW_UNIT
{
    T_BSCW_UNIT()
    {
        dw    = D_UNITSYS_BASE_LENGTH;
        dwa   = D_UNITSYS_BASE_LENGTH;
        dfc_t = D_UNITSYS_BASE_STRESS;
        dfc_b = D_UNITSYS_BASE_STRESS;
        dfr   = D_UNITSYS_BASE_STRESS;
    }
    int dw;
    int dwa;
    int dfc_t;
    int dfc_b;
    int dfr;
};

// struct T_CSCH_D
// {
//   T_CSCH_BASE CschB[4];
// 
//   void Initialize()
//   {
//     for (int i=0; i<4; ++i) CschB[i].Initialize();
//   }
// 
//   T_CSCH_D() { Initialize(); }
// };


// Span 검토.
struct T_SPCR_D
{
    T_SPAN_K OrgSpanK;
    T_SPAN_K SubSpanK;
    T_CSCK_B Flex[2];     // 0=Positive M, 1=Negative M
    T_CSCK_B Shear;
    T_CSCK_B Serv[2];     // 0=Positive M, 1=Negative M
    T_CSCK_B FatiLoad[2]; // 0=Top, 1=Bottom ???
    T_CSCK_B FatiFlex;
    T_CSCK_B FatiScon;  
    T_CSCK_B SCon;
    T_CSCK_B LStfn;       // longitudinal stiffener
    T_CSCK_B TStfn;       // Transverse stiffener
    T_CSCK_B Bstfn;       // Bearing Stiffener

    void Initialize()
    {
        OrgSpanK=0;
        SubSpanK=0;

        for (int i=0; i<2; ++i)
        {
            Flex[i].Initialize();
            Serv[i].Initialize();
            FatiLoad[i].Initialize();
        }    
        Shear.Initialize();
        FatiFlex.Initialize();
        FatiScon.Initialize();
        SCon.Initialize();
        LStfn.Initialize();
        TStfn.Initialize();
        Bstfn.Initialize();
    }

    T_SPCR_D() { Initialize(); }
};


struct T_CSCH_REF
{
    T_CSBR_BASE  *pCsbrB;  // 휨
    T_CSVR_BASE  *pCsvrB;  // 전단
    T_CSAC_BASE  *pCsacB;  // 사용한계
    T_CSFC_BASE  *pCsfcB;  // 피로한계
    T_SCBR_BASE  *pScbrB;  // 시공성(휨)
    T_SCVR_BASE  *pScvrB;  // 시공성(전단)
    T_CSCR_BASE  *pCscrB;  // 전단연결재
    T_CSSL_BASE  *pCsslB;  // 수평보강재
    T_BSTF_BASE  *pBstfB;
    T_CSFC_LOAD  *pCsfcLoad;
    T_CSFC_FLEX  *pCsfcFlex;
    T_CSFC_SHEAR *pCsfcShear;
    //
    T_CPBR_BASE  *pCpbrB;  // 휨 
    T_CPVR_BASE  *pCpvrB;  // 전단
    T_CPAC_BASE  *pCpacB;  // 사용한계, 
    T_CPFC_BASE  *pCpfcB;  // 피로한계
    T_CASC_BASE  *pCascB;  // 전단연결재
    T_CPLS_BASE  *pCplsB;  // 수평보강재
    T_CPTS_BASE  *pCptsB;  // 수직보강재
    T_CPBS_BASE  *pCpbsB;  // Bearing Stiffener
    T_CCBR_BASE  *pCcbrB;  // 시공단계(휨)  정/부가 필요한가?
    T_CCVR_BASE  *pCcvrB;  // 시공단계(전단)
    T_CPFC_LOAD  *pCpfcLoad;
    T_CPFC_STUD  *pCpfcStud;
    T_CPFC_SCONN *pCpfcScon;
    //
    T_BSBR_BASE *pBsbrB;
    T_BSVR_BASE *pBsvrB;
    T_BSTR_BASE *pBstrB;
    T_BSLS_BASE *pBslsB;
    T_BSSC_BASE *pBsscB;
    T_BSCW_BASE *pBscwB;

    void Initialize()
    {
        pCsbrB     = NULL;
        pCsvrB     = NULL;
        pCsacB     = NULL;
        pCsfcB     = NULL;
        pScbrB     = NULL;
        pScvrB     = NULL;
        pCscrB     = NULL;
        pCsslB     = NULL;
        pBstfB     = NULL;
        pCsfcLoad  = NULL;
        pCsfcFlex  = NULL;
        pCsfcShear = NULL;
        pCpbrB     = NULL;
        pCpvrB     = NULL;
        pCpacB     = NULL;
        pCpfcB     = NULL;
        pCascB     = NULL;
        pCplsB     = NULL;
        pCptsB     = NULL;
        pCpbsB     = NULL;
        pCcbrB     = NULL;
        pCcvrB     = NULL;
        pCpfcLoad  = NULL;
        pCpfcStud  = NULL;
        pCpfcScon  = NULL;
        pBsbrB     = NULL;
        pBsvrB     = NULL;
        pBstrB     = NULL;
        pBslsB     = NULL;
        pBsscB     = NULL;
        pBscwB     = NULL;
    }
    T_CSCH_REF() { Initialize(); }
};

//////////////////////////////////////////////////////////////////////////
// BS code CS454 Assessment Reserve Factor

struct T_ASCD_CSG_BS_BASE // Assessment Common Data BS
{
    // Common Data
    BOOL     bChk;
    T_ELEM_K ElemK;
    UINT     nBeamType;     // 0=Beam, 1=Virtual Beam
    UINT     unPosi;
    UINT     RCaseK;        // Rating Case Key
    UINT     unConcurrent;  // Fx_max .... Mz_min(1~12)               [Unit=None] 
	int      nDgnLcomK;
	T_STAG_K StageK;
	int      nStepK;
    BOOL     bStdMv;
    BOOL     bSpcMv;
    double   dFc;           // Condition Factor                       [Unit=None]  

    T_CSGF_MEMB ForD;

    BOOL     bComposite;
    double   dStr[7]; // FT, FB, FTL, FBL, FTR, FBR, FMAX of girder
    double   dSlStr[7]; // FT, FB, FTL, FBL, FTR, FBR, FMAX of composite slab
    BOOL     bTopFlangeWithLS;
    BOOL     bBottomFlangeWithLS;
    BOOL     bCompFlangeWithLS;
    BOOL     bWebWithLS;
    enSupportType enSupport;

    void Initialize()
    {
        bChk         = FALSE;
        ElemK        = 0;
        nBeamType    = EN_EL_BEAM;
        unPosi       = 0;
        RCaseK       = 0;
        unConcurrent = 1;
		nDgnLcomK    = 0;
		StageK       = 0;
		nStepK       = 0;
        bStdMv       = FALSE;
        bSpcMv       = FALSE;
        dFc          = 0.0;

        ForD.Initialize();

        bComposite   = FALSE;
        memset(dStr, 0, sizeof(dStr));
        memset(dSlStr, 0, sizeof(dSlStr));
        bTopFlangeWithLS = FALSE;
        bBottomFlangeWithLS = FALSE;
        bCompFlangeWithLS = FALSE;
        bWebWithLS = FALSE;
        enSupport = EN_Support_None;
    }

    void Set_Param(BOOL bCheck, T_ELEM_K ElemKey, UINT nBeam, UINT nPosi, UINT RCaseKey, UINT unConcurr, double dFactor)
    {
        bChk         = bCheck;
        ElemK        = ElemKey;
		nBeamType    = nBeam;
        unPosi       = nPosi;
        RCaseK       = RCaseKey;
        unConcurrent = unConcurr;
        dFc          = dFactor;
    }

    void Set_Stresses(const double& dFT, const double& dFB, const double& dFTL, const double& dFBL, const double& dFTR, const double& dFBR, const double& dFMAX)
    {
        dStr[0] = dFT;
        dStr[1] = dFB;
        dStr[2] = dFTL;
        dStr[3] = dFBL;
        dStr[4] = dFTR;
        dStr[5] = dFBR;
        dStr[6] = dFMAX;
    }
    void Set_SlabStresses(const double& dFT, const double& dFB, const double& dFTL, const double& dFBL, const double& dFTR, const double& dFBR, const double& dFMAX)
    {
        bComposite = TRUE;
        dSlStr[0] = dFT;
        dSlStr[1] = dFB;
        dSlStr[2] = dFTL;
        dSlStr[3] = dFBL;
        dSlStr[4] = dFTR;
        dSlStr[5] = dFBR;
        dSlStr[6] = dFMAX;
    }

    void SetStiffener(BOOL bTopFlange, BOOL bBottomFlange, BOOL bCompFlange, BOOL bWeb)
    {
        bTopFlangeWithLS = bTopFlange;
        bBottomFlangeWithLS = bBottomFlange;
        bCompFlangeWithLS = bCompFlange;
        bWebWithLS = bWeb;
    }

    BOOL IsStiffener() const
    {
        return (bTopFlangeWithLS || bBottomFlangeWithLS || bWebWithLS);
    }

    T_ASCD_CSG_BS_BASE() { Initialize(); }
};

struct T_ASCD_CSG_BS_UNIT
{
    T_ASCD_CSG_BS_UNIT()
    {
        dStr   = D_UNITSYS_BASE_STRESS;
        dSlStr = D_UNITSYS_BASE_STRESS;
    }
    int dStr;
    int dSlStr;
};

struct T_ASCA_RAT_RAIL_BS_B
{
    double   dDCR;     // Mu/RA
    double	 dSBSU;    // 20 x (MD - MDL) / MSLL , Static
    double	 dDBSU;    // Dynamic
    double   dSRA;     // Static Rating
    double   dDRA;
    double   dSUforRA; // Static, U for RA8
    double   dDUforRA; // Dynamic, U for RA8
    int      nSAC;     // Static, Assessed Category , #define D_NR_RAIL_ASSESSED_CATEGORY_A1
    int      nDAC;     // Dynamic, Assessed Category

    void Initialize()
    {
        dDCR     = 0.0;
        dSBSU    = 0.0;
        dDBSU    = 0.0;
        dSRA     = 0.0;
        dDRA     = 0.0;
        dSUforRA = 0.0;
        dDUforRA = 0.0;
		nSAC     = 0;
        nDAC     = 0;
    }
    T_ASCA_RAT_RAIL_BS_B() { Initialize(); }
    
	void Set(double dDCR, double dSBSU, double dDBSU, double dSRA, double dDRA, double dSUforRA, double dDUforRA, int nSAC, int nDAC)
	{
		this->dDCR = dDCR;
		this->dSBSU = dSBSU;
		this->dDBSU = dDBSU;
		this->dSRA = dSRA;
		this->dDRA = dDRA;
		this->dSUforRA = dSUforRA;
		this->dDUforRA = dDUforRA;
        this->nSAC = nSAC;
        this->nDAC = nDAC;
	}
};


struct T_ASSM_STRESS_BS_BASE
{
    // Common Data
    T_ASCD_CSG_BS_BASE ComD;

    double   dR;        // Calculated Resistance R*
    double   dRA;       // Assessment Resistance RA*=Fc x R*
    double   dSA;       // Sa* - Assessment Load Effect total Sa* = Sd* + Sha* + Ssv*
    double   dSd;       // Sd* - Assessment Load Effect; Dead and superimposed dead loads
    double   dSst;      // Sha* - Assessment Load Effect; Standard Vehicle
    double   dS;        // S* - Assessment Load Effect; Special Vehicle
    double   dSd_Sst;   // Sd* + Sha*
    double   dA;        // Reserve Factor for only Standard Vehicle
    double   dPsi;      // Ψ - Reserve Factor with Associated type HA Loading for Special Vehicle; BD 86/11
    double   dPsi_ast;  // Ψ* - Reserve Factor without Associated type HA Loading for Special Vehicle; BD 86/11
    double	 dRF;       // Maximan Reserve Factor for Assessment; max(dA, dPsi, dPsi_ast)

    void Initialize()
    {
        ComD.Initialize();
        dR           = 0.0;
        dRA          = 0.0;
        dSA          = 0.0;
        dSd          = 0.0;
        dSst         = 0.0;
        dS           = 0.0;
        dSd_Sst      = 0.0;
        dA           = 0.0;
        dPsi         = 0.0;
        dPsi_ast     = 0.0;
        dRF          = 1.0;
    }

    T_ASSM_STRESS_BS_BASE() { Initialize(); }
};

struct T_ASSM_STRESS_BS_UNIT
{
    T_ASSM_STRESS_BS_UNIT()
    {
        dR   = D_UNITSYS_BASE_STRESS;
        dRA  = D_UNITSYS_BASE_STRESS;
        dSA  = D_UNITSYS_BASE_STRESS;
        dSd  = D_UNITSYS_BASE_STRESS;
        dSst = D_UNITSYS_BASE_STRESS;
        dS   = D_UNITSYS_BASE_STRESS;
    }
    int dR;
    int dRA;
    int dSA;
    int dSd;
    int dSst;
    int dS;
};

struct T_ASSM_UNIT_NONE_BS_BASE
{
    // Common Data
    T_ASCD_CSG_BS_BASE ComD;

    double   dR;        // Calculated Resistance R*
    double   dRA;       // Assessment Resistance RA*=Fc x R*
    double   dSA;       // Sa* - Assessment Load Effect total Sa* = Sd* + Sha* + Ssv*
    double   dSd;       // Sd* - Assessment Load Effect; Dead and superimposed dead loads
    double   dSst;      // Sha* - Assessment Load Effect; Standard Vehicle
    double   dS;        // S* - Assessment Load Effect; Special Vehicle
    double   dSd_Sst;  // Sd* + Sha*
    double   dA;        // Reserve Factor for only Standard Vehicle
    double   dPsi;      // Ψ - Reserve Factor with Associated type HA Loading for Special Vehicle; BD 86/11
    double   dPsi_ast;  // Ψ* - Reserve Factor without Associated type HA Loading for Special Vehicle; BD 86/11
    double	 dRF;       // Maximan Reserve Factor for Assessment; max(dA, dPsi, dPsi_ast)

    void Initialize()
    {
        ComD.Initialize();
        dR           = 0.0;
        dRA          = 0.0;
        dSA          = 0.0;
        dSd          = 0.0;
        dSst         = 0.0;
        dS           = 0.0;
        dSd_Sst      = 0.0;
        dA           = 0.0;
        dPsi         = 0.0;
        dPsi_ast     = 0.0;
        dRF          = 1.0;
    }

    T_ASSM_UNIT_NONE_BS_BASE() { Initialize(); }
};


struct T_ASSM_RF_BASE
{
    double   dR;        // Calculated Resistance R*
    double   dRA;       // Assessment Resistance RA*=Fc x R*
    double   dSA;       // Sa* - Assessment Load Effect total Sa* = Sd* + Sha* + Ssv*
    double   dSd;       // Sd* - Assessment Load Effect; Dead and superimposed dead loads
    double   dSst;      // Sha* - Assessment Load Effect; Standard Vehicle
    double   dS;        // S* - Assessment Load Effect; Special Vehicle
    double   dSd_Sst;   // Sd* + Sha*
    double   dA;        // Reserve Factor for only Standard Vehicle
    double   dPsi;      // Ψ - Reserve Factor with Associated type HA Loading for Special Vehicle; BD 86/11
    double   dPsi_ast;  // Ψ* - Reserve Factor without Associated type HA Loading for Special Vehicle; BD 86/11
    double	 dRF;       // Maximan Reserve Factor for Assessment; max(dA, dPsi, dPsi_ast)

    void Initialize()
    {
        dR           = 0.0;
        dRA          = 0.0;
        dSA          = 0.0;
        dSd          = 0.0;
        dSst         = 0.0;
        dS           = 0.0;
        dSd_Sst      = 0.0;
        dA           = 0.0;
        dPsi         = 0.0;
        dPsi_ast     = 0.0;
        dRF          = 1.0;
    }

    T_ASSM_RF_BASE() { Initialize(); }
};

struct T_ASFC_CSG_BS_BASE // Assessment Flexural Capacity BS
{
    // Common Data
    T_ASCD_CSG_BS_BASE ComD;

    // Flexural Capacity
    double   dR;       // Calculated Resistance R*
    double   dRA;      // Assessment Resistance RA*=Fc x R*
    double   dSA;      // Sa* - Assessment Load Effect total Sa* = Sd* + Sha* + Ssv*
    double   dSd;      // Sd* - Assessment Load Effect; Dead and superimposed dead loads
    double   dSst;     // Sha* - Assessment Load Effect; Standard Vehicle
    double   dS;       // S* - Assessment Load Effect; Special Vehicle
    double   dSd_Sst;  // Sd* + Sha*
    double   dA;       // Reserve Factor for only Standard Vehicle
    double   dPsi;     // Ψ - Reserve Factor with Associated type HA Loading for Special Vehicle; BD 86/11
    double   dPsi_ast; // Ψ* - Reserve Factor without Associated type HA Loading for Special Vehicle; BD 86/11
	double   dfs;
	double   dfc;
	double   dPhify;
	double   dPhifb;
	double   dfcLimit;
    double	 dRF;      // Maximan Reserve Factor for Assessment; max(dA, dPsi, dPsi_ast)
    T_ASCA_RAT_RAIL_BS_B RatPa;
    int      nOK;

    void Initialize()
    {
        ComD.Initialize();
        dR           = 0.0;
        dRA          = 0.0;
        dSA          = 0.0;
        dSd          = 0.0;
        dSst         = 0.0;
        dS           = 0.0;
        dSd_Sst      = 0.0;
        dA           = 0.0;
        dPsi         = 0.0;
        dPsi_ast     = 0.0;
		dPhify       = 0.0;
		dfs          = 0.0;
		dfc          = 0.0;
		dPhifb       = 0.0;
		dfcLimit     = 0.0;
        dRF          = 0.0;
        RatPa.Initialize();
        nOK          = 0;
    }
    T_ASFC_CSG_BS_BASE() { Initialize(); }
    void SetForce(double dSA1, double dSd1, double dSst1, double dS1)
    {
        dSA  = dSA1;
        dSd  = dSd1;
        dSst = dSst1;
        dS   = dS1;
    }
};

struct T_ASFC_CSG_BS_UNIT
{
    T_ASFC_CSG_BS_UNIT()
    {
        dR   = D_UNITSYS_BASE_MOMENT;
        dRA  = D_UNITSYS_BASE_MOMENT;
        dSA  = D_UNITSYS_BASE_MOMENT;
        dSd  = D_UNITSYS_BASE_MOMENT;
        dSst = D_UNITSYS_BASE_MOMENT;
        dS   = D_UNITSYS_BASE_MOMENT;
		dfs  = D_UNITSYS_BASE_STRESS;
		dfc  = D_UNITSYS_BASE_STRESS;
		dPhify   = D_UNITSYS_BASE_STRESS;
		dPhifb   = D_UNITSYS_BASE_STRESS;
		dfcLimit = D_UNITSYS_BASE_STRESS;

    }
    int dR;
    int dRA;
    int dSA;
    int dSd;
    int dSst;
    int dS;
	int dfs;
	int dfc;
	int dPhify;
	int dPhifb;
	int dfcLimit;
};

struct T_ASFC_CSG_BS_CASE
{
    // 0=I-Posi, 1=J-Posi
    T_ASFC_CSG_BS_BASE AsfcB[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i )  AsfcB[i].Initialize();
    }
    T_ASFC_CSG_BS_CASE() { Initialize(); }
};

struct T_ASFC_CSG_BS_D
{
    // 0=I-Posi, 1=I-Nega, 2=J-Posi, 3=J-Nega.
    T_ASFC_CSG_BS_BASE AsfcB[4];

    void Initialize()
    {
        for ( int i=0; i<4; ++i ) AsfcB[i].Initialize();
    }
    T_ASFC_CSG_BS_D() { Initialize(); }
};

struct T_ACSF_CSG_BS_D
{
	// 0=I-Posi, 1=I-Nega, 2=J-Posi, 3=J-Nega.
	T_ASFC_CSG_BS_BASE AcsfB[4]; // 시공단계에서 따로 검토 사항이 있으면 분리 필요

	void Initialize()
	{
		for ( int i = 0; i < 4; ++i ) AcsfB[i].Initialize();
	}
	T_ACSF_CSG_BS_D() { Initialize(); }
};

struct T_ASSC_CSG_BS_BASE // Assessment Shear Capacity BS
{
    // Common Data
    T_ASCD_CSG_BS_BASE ComD;

    // Shear Capacity
    double   dR;        // Calculated Resistance R*
    double   dRA;       // Assessment Resistance RA*=Fc x R*
    double   dSA;       // Sa* - Assessment Load Effect total Sa* = Sd* + Sha* + Ssv*
    double   dSd;       // Sd* - Assessment Load Effect; Dead and superimposed dead loads
    double   dSst;      // Sha* - Assessment Load Effect; Standard Vehicle
    double   dS;        // S* - Assessment Load Effect; Special Vehicle
    double   dSd_Sst;   // Sd* + Sha*
    double   dA;        // Reserve Factor for only Standard Vehicle
    double   dPsi;      // Ψ - Reserve Factor with Associated type HA Loading for Special Vehicle; BD 86/11
    double   dPsi_ast;  // Ψ* - Reserve Factor without Associated type HA Loading for Special Vehicle; BD 86/11
    double	 dRF;       // Maximan Reserve Factor for Assessment; max(dA, dPsi, dPsi_ast)
	double   dSAapp;    // Box, Tub형 Web 1개의 전단력

    T_ASCA_RAT_RAIL_BS_B RatPa;

    BOOL     bLS_chk;
    double   dLS_V1;   // Longitudinal shear force (force/length)
    double   dLS_V1_lim1;
    double   dLS_V1_lim2;
    double   dLS_s;   // Longitudinal shear reinforcement space
    double   dLS_s_lim1;
    double   dLS_s_lim2;
    int      nOK;

    void Initialize()
    {
        ComD.Initialize();
        dR           = 0.0;
        dRA          = 0.0;
        dSA          = 0.0;
        dSd          = 0.0;
        dSst         = 0.0;
        dS           = 0.0;
        dSd_Sst      = 0.0;
        dA           = 0.0;
        dPsi         = 0.0;
        dPsi_ast     = 0.0;
        dRF          = 1.0;
		dSAapp       = 0.0;
        RatPa.Initialize();

        bLS_chk     = FALSE;
        dLS_V1      = 0.0;
        dLS_V1_lim1 = 0.0;
        dLS_V1_lim2 = 0.0;
        dLS_s       = 0.0;
        dLS_s_lim1  = 0.0;
        dLS_s_lim2  = 0.0;
        nOK         = 0;
    }

    T_ASSC_CSG_BS_BASE() { Initialize(); }
    void SetForce(double dSA1, double dSd1, double dSst1, double dS1)
    {
        dSA  = dSA1;
        dSd  = dSd1;
        dSst = dSst1;
        dS   = dS1;
    }
};

struct T_ASSC_CSG_BS_UNIT
{
    T_ASSC_CSG_BS_UNIT()
    {
        dR   = D_UNITSYS_BASE_FORCE;
        dRA  = D_UNITSYS_BASE_FORCE;
        dSA  = D_UNITSYS_BASE_FORCE;
        dSd  = D_UNITSYS_BASE_FORCE;
        dSst = D_UNITSYS_BASE_FORCE;
        dS   = D_UNITSYS_BASE_FORCE;

        dLS_V1      = D_UNITSYS_BASE_UNITFORCE;
        dLS_V1_lim1 = D_UNITSYS_BASE_UNITFORCE;
        dLS_V1_lim2 = D_UNITSYS_BASE_UNITFORCE;
        dLS_s       = D_UNITSYS_BASE_LENGTH;
        dLS_s_lim1  = D_UNITSYS_BASE_LENGTH;
        dLS_s_lim2  = D_UNITSYS_BASE_LENGTH;
    }
    int dR;
    int dRA;
    int dSA;
    int dSd;
    int dSst;
    int dS;

    int dLS_V1;
    int dLS_V1_lim1;
    int dLS_V1_lim2;
    int dLS_s;
    int dLS_s_lim1;
    int dLS_s_lim2;
};

struct T_ASSC_CSG_BS_CASE
{    
    // 0=I-Posi, 1=J-Posi
    T_ASSC_CSG_BS_BASE AsscB[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i )  AsscB[i].Initialize();
    }
    T_ASSC_CSG_BS_CASE() { Initialize(); }
};

struct T_ASSC_CSG_BS_D
{
    T_ASSC_CSG_BS_BASE AsscB[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i ) AsscB[i].Initialize();
    }
    T_ASSC_CSG_BS_D() { Initialize(); }
};

struct T_ACSV_CSG_BS_D
{
	T_ASSC_CSG_BS_BASE AcsvB[2];

	void Initialize()
	{
		for ( int i = 0; i < 2; ++i ) AcsvB[i].Initialize();
	}
	T_ACSV_CSG_BS_D() { Initialize(); }
};


// Combined Bending and Shear Reserve Factors
struct T_ASCB_CSG_BS_BASE
{
    // Common Data
    T_ASCD_CSG_BS_BASE ComD;

    // Flexural Capacity
    T_CSGD_COMF  ForcePanelM;
    T_CSGD_COMF  ForcePanelV;
    double   dR;       // Calculated Resistance R*
    double   dRA;      // Assessment Resistance RA*=Fc x R*
    double   dSA;      // Sa* - Assessment Load Effect total Sa* = Sd* + Sha* + Ssv*
    double   dSd;      // Sd* - Assessment Load Effect; Dead and superimposed dead loads
    double   dSst;     // Sha* - Assessment Load Effect; Standard Vehicle
    double   dSd_Sst;  // Sd* + Sha*
    double   dS;       // S* - Assessment Load Effect; Special Vehicle
    double   dMf;
    double   dM_D;
    double   dV_D;
    double   dV_R;
    double   dM_SV;
    double   dV_SV;
    double   dM_DL;
    double   dV_DL;
    double   dM_ST;
    double   dV_ST;
    double   dA;       // Reserve Factor for only Standard Vehicle
    double   dPsi;     // Ψ - Reserve Factor with Associated type HA Loading for Special Vehicle; BD 86/11
    double   dPsi_ast; // Ψ* - Reserve Factor without Associated type HA Loading for Special Vehicle; BD 86/11
    double	 dRF;      // Maximan Reserve Factor for Assessment; max(dA, dPsi, dPsi_ast)
    int      nOK;

    void Initialize()
    {
        ComD.Initialize();
        ForcePanelM.Initialize();
        ForcePanelV.Initialize();
        dR           = 0.0;
        dRA          = 0.0;
        dSA          = 0.0;
        dSd          = 0.0;
        dSst         = 0.0;
        dSd_Sst      = 0.0;
        dS           = 0.0;
        dMf          = 0.0;
        dM_D         = 0.0;
        dV_D         = 0.0;
        dV_R         = 0.0;
        dM_SV        = 0.0;
        dV_SV        = 0.0;
        dM_DL        = 0.0;
        dV_DL        = 0.0;
        dM_ST        = 0.0;
        dV_ST        = 0.0;
        dA           = 0.0;
        dPsi         = 0.0;
        dPsi_ast     = 0.0;
        dRF          = 0.0;
        nOK          = 0;
    }
    T_ASCB_CSG_BS_BASE() { Initialize(); }
    void SetForce(double dSA1, double dSd1, double dSst1, double dS1)
    {
        dSA  = dSA1;
        dSd  = dSd1;
        dSst = dSst1;
        dS   = dS1;
    }
};

struct T_ASCB_CSG_BS_UNIT
{
    T_ASCB_CSG_BS_UNIT()
    {
        dMf   = D_UNITSYS_BASE_MOMENT;
        dV_R  = D_UNITSYS_BASE_FORCE;
        dM_SV = D_UNITSYS_BASE_MOMENT;
        dV_SV = D_UNITSYS_BASE_FORCE;
    }
    int dMf;
    int dV_R;
    int dM_SV;
    int dV_SV;
};

struct T_ASCB_CSG_BS_CASE
{
    // 0=I-Posi, 1=J-Posi
    T_ASCB_CSG_BS_BASE AscbB[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i )  AscbB[i].Initialize();
    }
    T_ASCB_CSG_BS_CASE() { Initialize(); }
};

struct T_ASCB_CSG_BS_D
{
    // 0=I-Posi, 1=I-Nega, 2=J-Posi, 3=J-Nega.
    T_ASCB_CSG_BS_BASE AscbB[4];

    void Initialize()
    {
        for ( int i=0; i<4; ++i ) AscbB[i].Initialize();
    }
    T_ASCB_CSG_BS_D() { Initialize(); }
};

struct T_ASTC_CSG_BS_BASE // Assessment Torsion Capacity BS
{
    // Common Data
    T_ASCD_CSG_BS_BASE ComD;

    // Torsion Stress
    double   dv;      // shear stress
    double   dvt;      // torsional shear stress
//     double   dvtu1;      // ultimate torsional shear stress, (v+vt) <= vtu1
//     double   dy1;      // larger centre-line dimension of a link
//     double   dvtu2;      // y1>550mm, ultimate torsional shear stress, vt <= vt*y1/550
//     double	 dRF;      // Reserve Factor for Assessment
//     BOOL     bBox;      //
//     double   dT;      // Torque due to Ultimate Load
//     double   dTu;      // Ulimate Torsion
//     double   dvtmin;      // vtmin

    void Initialize()
    {
        ComD.Initialize();
        dv    = 0.0;
        dvt   = 0.0;
//         dvtu1 = 0.0;
//         dy1   = 0.0;
//         dvtu2 = 0.0;
//         dRF   = 0.0;
//         bBox  = FALSE;
//         dT    = 0.0;
//         dTu   = 0.0;
//         dvtmin= 0.0;
    }
    T_ASTC_CSG_BS_BASE() { Initialize(); }
};

struct T_ASTC_CSG_BS_UNIT
{
    T_ASTC_CSG_BS_UNIT()
    {
        dv    = D_UNITSYS_BASE_STRESS;
        dvt   = D_UNITSYS_BASE_STRESS;
//         dvtu1 = D_UNITSYS_BASE_STRESS;
//         dy1   = D_UNITSYS_BASE_LENGTH;
//         dvtu2 = D_UNITSYS_BASE_STRESS;
//         dT    = D_UNITSYS_BASE_MOMENT;
//         dTu   = D_UNITSYS_BASE_MOMENT;
//         dvtmin= D_UNITSYS_BASE_STRESS;
    }
    int dv;
    int dvt;
//     int dvtu1;
//     int dy1;
//     int dvtu2;
//     int dT;
//     int dTu;
//     int dvtmin;
};

struct T_ASTC_CSG_BS_CASE
{
    // 0=I-Posi, 1=J-Posi
    T_ASTC_CSG_BS_BASE AstcB[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i ) AstcB[i].Initialize();
    }
    T_ASTC_CSG_BS_CASE() { Initialize(); }
};

struct T_ASTC_CSG_BS_D
{
    T_ASTC_CSG_BS_BASE AstcB[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i ) AstcB[i].Initialize();
    }
    T_ASTC_CSG_BS_D() { Initialize(); }
};

enum BS_ASSESS_CSG_SLS_CLASS // BS Assessment SLS Stress check Class
{
    BS_ASSESS_CSG_SLS_CLASS1 = 1,
    BS_ASSESS_CSG_SLS_CLASS2,
    BS_ASSESS_CSG_SLS_CLASS3,
};

struct T_ASSL_CSG_BS_BASE // Assessment Service Limit State BS
{
    // Common Data
    T_ASCD_CSG_BS_BASE ComD;

    // Service Limit State
    BS_ASSESS_CSG_SLS_CLASS enClass;      // Class2 or 3
    double   dLoadEff;
    double   dSA;
    double   dsig_top[2]; // 0=left, 1=right
    double   dsig_bot[2];
    double   dsig_t;      // tensile stress class2
    double   dsig_c;      // compressive stress class2
    BOOL     bTopComp;
    BOOL     bBottomComp;
    double   dsig_w;      // web
    double   dsig_t_lim;  // tensile stress limit class2
    double   dsig_c_lim;  // compressive stress limit class2
    double   dsig_w_lim;  // web
    int      nChkGirder;  // 1 == true
    int      nSectClass;  // 0=none, 1=compact, 2=non-compact

    double   dft_s;       // Slab Top
    double   dfb_s;       // Slab Bottom
    double   dft_s_lim;
    double   dfb_s_lim;
    int      nChkSlab; 
    double   dfr;         // Rebar
    double   dfr_lim;
    int      nChkRebar;
    

    double   dw;          // crack width class3
    double   dw_lim;      // crack width limit class3
    double	 dRF;         // Reserve Factor for Assessment
    int      nOK;         

    T_ASSM_RF_BASE RFcom;
    T_ASSM_RF_BASE RFten;
    double   dRA;       // Assessment Resistance RA*=Fc x R*
    double   dSd;       // Sd* - Assessment Load Effect; Dead and superimposed dead loads
    double   dSst;      // Sha* - Assessment Load Effect; Standard Vehicle
    double   dS;        // S* - Assessment Load Effect; Special Vehicle
    double   dSd_Sst;  // Sd* + Sha*
    double   dA;        // Reserve Factor for only Standard Vehicle
    double   dPsi;      // Ψ - Reserve Factor with Associated type HA Loading for Special Vehicle; BD 86/11
    double   dPsi_ast;  // Ψ* - Reserve Factor without Associated type HA Loading for Special Vehicle; BD 86/11

    void Initialize()
    {
        ComD.Initialize();
        enClass      = BS_ASSESS_CSG_SLS_CLASS1;
        dLoadEff     = 0.0;
        dSA          = 0.0;
        for ( int i=0; i<2; ++i )
        {
            dsig_top[i] = 0.0;
            dsig_bot[i] = 0.0;
        }        
        dsig_t       = 0.0;
        dsig_c       = 0.0;
        bTopComp     = TRUE;
        bBottomComp  = TRUE;
        dsig_w       = 0.0;
        dsig_t_lim   = 0.0;
        dsig_c_lim   = 0.0;
        dsig_w_lim   = 0.0;
        nChkGirder   = 0;
        nSectClass   = 0;
        dft_s        = 0.0;
        dfb_s        = 0.0;
        dft_s_lim    = 0.0;
        dfb_s_lim    = 0.0;
        nChkSlab     = 0;
        dfr          = 0.0;
        dfr_lim      = 0.0;
        nChkRebar    = 0;
        dw           = 0.0;
        dw_lim       = 0.0;
        dRF          = 0.0;
        nOK          = 0;
        RFcom.Initialize();
        RFten.Initialize();
        dRA          = 0.0;
        dSd          = 0.0;
        dSst         = 0.0;
        dS           = 0.0;
        dSd_Sst      = 0.0;
        dA           = 0.0;
        dPsi         = 0.0;
        dPsi_ast     = 0.0;
    }
    T_ASSL_CSG_BS_BASE() { Initialize(); }
};

struct T_ASSL_CSG_BS_UNIT
{
    T_ASSL_CSG_BS_UNIT()
    {
        dSA        = D_UNITSYS_BASE_MOMENT;
        dsig_t     = D_UNITSYS_BASE_STRESS;
        dsig_c     = D_UNITSYS_BASE_STRESS;
        dsig_t_lim = D_UNITSYS_BASE_STRESS;
        dsig_c_lim = D_UNITSYS_BASE_STRESS;
        dw         = D_UNITSYS_BASE_LENGTH;
    }
    int dSA;
    int dsig_t;
    int dsig_c;
    int dsig_t_lim;
    int dsig_c_lim;
    int dw;
};

struct T_ASSL_CSG_BS_CASE
{
    // 0=I-Posi, 1=J-Posi
    T_ASSL_CSG_BS_BASE AsslB[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i )
            AsslB[i].Initialize();
    }
    T_ASSL_CSG_BS_CASE() { Initialize(); }
};

struct T_ASSL_CSG_BS_D
{
    T_ASSL_CSG_BS_BASE AsslB[4];

    void Initialize()
    {
        for ( int i=0; i<4; ++i ) AsslB[i].Initialize();
    }
    T_ASSL_CSG_BS_D() { Initialize(); }
};

struct T_ASLS_CSG_BS_BASE // Assessment Longitudinal Shear Verification
{
    // Common Data
    T_ASCD_CSG_BS_BASE ComD;

    double dq;
    double dPim;
    double dPa;
    double dqr;
    double dq_qr;
    double dq_qr_lim;
    int    nOK;
    double dRA;       // Assessment Resistance RA*=Fc x R*
    double dSA;       // Sa* - Assessment Load Effect total Sa* = Sd* + Sha* + Ssv*
    double dSd;       // Sd* - Assessment Load Effect; Dead and superimposed dead loads
    double dSst;      // Sha* - Assessment Load Effect; Standard Vehicle
    double dS;        // S* - Assessment Load Effect; Special Vehicle
    double dSd_Sst;   // Sd* + Sha*
    double dA;
    int    nSectClass;// 0=none, 1=compact, 2=non-compact

    void Initialize()
    {
        ComD.Initialize();
        dq = 0.0;
        dPim = 0.0;
        dPa = 0.0;
        dqr = 0.0;
        dq_qr = 0.0;
        dq_qr_lim = 0.0;
        nOK = 0;
        dRA   = 0.0;
        dSA   = 0.0;
        dSd   = 0.0;
        dSst  = 0.0;
        dS    = 0.0;
        dSd_Sst = 0.0;
        dA    = 0.0;
        nSectClass = 0;
    }
    T_ASLS_CSG_BS_BASE() { Initialize(); }
};

struct T_ASLS_CSG_BS_CASE
{
    // 0=I-Posi, 1=J-Posi
    T_ASLS_CSG_BS_BASE AslsB[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i )
            AslsB[i].Initialize();
    }
    T_ASLS_CSG_BS_CASE() { Initialize(); }
};

struct T_ASLS_CSG_BS_D
{
    T_ASLS_CSG_BS_BASE AslsB[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i ) AslsB[i].Initialize();
    }
    T_ASLS_CSG_BS_D() { Initialize(); }
};

struct T_ASLS_CSG_BS_UNIT
{
    T_ASLS_CSG_BS_UNIT()
    {
        dq   = D_UNITSYS_BASE_UNITFORCE;
        dPim = D_UNITSYS_BASE_FORCE;
        dPa  = D_UNITSYS_BASE_FORCE;
        dqr  = D_UNITSYS_BASE_UNITFORCE;
    }
    int dq;
    int dPim;
    int dPa;
    int dqr;
};


struct T_ASRS_BASE
{
    BOOL   bChk;
    double dRF;

    void Initialize()
    {
        bChk = FALSE;
        dRF  = 0.0;
    }
    T_ASRS_BASE() { Initialize(); }
};

struct T_ASRS_CSG_BS_BASE // Assessment Reserve Factor/Verification Summary
{
    BOOL     bChk;
    T_ELEM_K ElemK;
    UINT     unPosi;

    T_ASRS_BASE PBend;
    T_ASRS_BASE NBend;
    T_ASRS_BASE Shear;
    T_ASRS_BASE Comb;
    T_ASRS_BASE SLS;
    T_ASRS_BASE LShear;

    int nOK;

    void Initialize()
    {
        bChk = FALSE;
        ElemK  = 0;
        unPosi = 0;
        PBend.Initialize();
        NBend.Initialize();
        Shear.Initialize();
        Comb.Initialize();
        SLS.Initialize();
        LShear.Initialize();
        nOK = 0;
    }
    T_ASRS_CSG_BS_BASE() { Initialize(); }
};

struct T_ASRS_CSG_BS_D
{
    T_ASRS_CSG_BS_BASE AsrsB[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i ) AsrsB[i].Initialize();
    }
    T_ASRS_CSG_BS_D() { Initialize(); }
};

// Steel Flange Reserve Factor Table (Beams with L. Stiffeners)
// Flange
struct T_ASFG_CSG_BS_BASE  
{    
    T_ASCD_CSG_BS_BASE ComD; 

    double dSig_yf;
    double dSig_f_SV;
    double dTau_SV;
    double dSig_f_DL;
    double dTau_DL;
    double dSig_f_ST;
    double dTau_ST;
    double dRA;      // Assessment Resistance RA*=Fc x R*
    double dSA;      // Sa* - Assessment Load Effect total Sa* = Sd* + Sha* + Ssv*
    double dSd;      // Sd* - Assessment Load Effect; Dead and superimposed dead loads
    double dSst;     // Sha* - Assessment Load Effect; Standard Vehicle
    double dS;       // S* - Assessment Load Effect; Special Vehicle
    double dSd_Sst;  // Sd* + Sha*
    double dA;       // Reserve Factor for only Standard Vehicle
    double dPsi;     // Ψ - Reserve Factor with Associated type HA Loading for Special Vehicle; BD 86/11
    double dPsi_ast; // Ψ* - Reserve Factor without Associated type HA Loading for Special Vehicle; BD 86/11
    double dRF;      // Maximan Reserve Factor for Assessment; max(dA, dPsi, dPsi_ast)
    int    nOK;

    void Initialize()
    {
        ComD.Initialize();
        dSig_yf   = 0.0;
        dSig_f_SV = 0.0;
        dTau_SV   = 0.0;
        dSig_f_DL = 0.0;
        dTau_DL   = 0.0;
        dSig_f_ST = 0.0;
        dTau_ST   = 0.0;
        dRA       = 0.0;
        dSA       = 0.0;
        dSd       = 0.0;
        dSst      = 0.0;
        dS        = 0.0;
        dSd_Sst   = 0.0;
        dA        = 0.0;
        dPsi      = 0.0;
        dPsi_ast  = 0.0;
        dRF       = 0.0;
        nOK       = 0;
    }
    T_ASFG_CSG_BS_BASE() { Initialize(); }
};

struct T_ASFG_CSG_BS_UNIT
{
    T_ASFG_CSG_BS_UNIT()
    {
        dSig_yf   = D_UNITSYS_BASE_STRESS;
        dSig_f_SV = D_UNITSYS_BASE_STRESS;
        dTau_SV   = D_UNITSYS_BASE_STRESS;
        dSig_f_DL = D_UNITSYS_BASE_STRESS;
        dTau_DL   = D_UNITSYS_BASE_STRESS;
        dSig_f_ST = D_UNITSYS_BASE_STRESS;
        dTau_ST   = D_UNITSYS_BASE_STRESS;
    }
    int dSig_yf;
    int dSig_f_SV;
    int dTau_SV;
    int dSig_f_DL;
    int dTau_DL;
    int dSig_f_ST;
    int dTau_ST;
};

struct T_ASFG_CSG_BS_CASE
{
    T_ASFG_CSG_BS_BASE AsfgB[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i ) AsfgB[i].Initialize();
    }
    T_ASFG_CSG_BS_CASE() { Initialize(); }
};

struct T_ASFG_CSG_BS_D
{
    T_ASFG_CSG_BS_BASE AsfgB[4]; // 0=i-압축, 1=i-인장, 2=j-압축, 3=j-인장

    void Initialize()
    {
        for ( int i=0; i<4; ++i ) AsfgB[i].Initialize();
    }
    T_ASFG_CSG_BS_D() { Initialize(); }
};

// Stiffener
struct T_ASFS_CSG_BS_BASE
{
    T_ASCD_CSG_BS_BASE ComD;

    double dk_l;
    double dSig_y;
    double dSig_a_SV;
    double dTau_1_SV;
    double dSig_a_DL;
    double dTau_1_DL;
    double dSig_a_ST;
    double dTau_1_ST;
    double dk_s;
    double dRA;      // Assessment Resistance RA*=Fc x R*
    double dSA;      // Sa* - Assessment Load Effect total Sa* = Sd* + Sha* + Ssv*
    double dSd;      // Sd* - Assessment Load Effect; Dead and superimposed dead loads
    double dSst;     // Sha* - Assessment Load Effect; Standard Vehicle
    double dS;       // S* - Assessment Load Effect; Special Vehicle
    double dSd_Sst;  // Sd* + Sha*
    double dA;       // Reserve Factor for only Standard Vehicle
    double dPsi;     // Ψ - Reserve Factor with Associated type HA Loading for Special Vehicle; BD 86/11
    double dPsi_ast; // Ψ* - Reserve Factor without Associated type HA Loading for Special Vehicle; BD 86/11
    double dRF;      // Maximan Reserve Factor for Assessment; max(dA, dPsi, dPsi_ast)
    int    nOK;

    void Initialize()
    {
        ComD.Initialize();
        dk_l      = 0.0;
        dSig_y    = 0.0;
        dSig_a_SV = 0.0;
        dTau_1_SV = 0.0;
        dSig_a_DL = 0.0;
        dTau_1_DL = 0.0;
        dSig_a_ST = 0.0;
        dTau_1_ST = 0.0;
        dk_s      = 0.0;
        dRA       = 0.0;
        dSA       = 0.0;
        dSd       = 0.0;
        dSst      = 0.0;
        dS        = 0.0;
        dSd_Sst   = 0.0;
        dA        = 0.0;
        dPsi      = 0.0;
        dPsi_ast  = 0.0;
        dRF       = 0.0;
        nOK       = 0;
    }
    T_ASFS_CSG_BS_BASE() { Initialize(); }
};

struct T_ASFS_CSG_BS_UNIT
{
    T_ASFS_CSG_BS_UNIT()
    {
        dSig_y    = D_UNITSYS_BASE_STRESS;
        dSig_a_SV = D_UNITSYS_BASE_STRESS;
        dTau_1_SV = D_UNITSYS_BASE_STRESS;
        dSig_a_DL = D_UNITSYS_BASE_STRESS;
        dTau_1_DL = D_UNITSYS_BASE_STRESS;
        dSig_a_ST = D_UNITSYS_BASE_STRESS;
        dTau_1_ST = D_UNITSYS_BASE_STRESS;
    }
    int dSig_y;
    int dSig_a_SV;
    int dTau_1_SV;
    int dSig_a_DL;
    int dTau_1_DL;
    int dSig_a_ST;
    int dTau_1_ST;
};


struct T_ASFS_CSG_BS_CASE
{
    T_ASFS_CSG_BS_BASE AsfsB[2];    // Stiffener, 0=i, 1=j
    T_ASFS_CSG_BS_BASE AsfsFlgB[2]; // Flange, 0=i, 1=j

    void Initialize()
    {
        for ( int i=0; i<2; ++i )
        {
            AsfsB[i].Initialize();
            AsfsFlgB[i].Initialize();
        }
    }
    T_ASFS_CSG_BS_CASE() { Initialize(); }
};

struct T_ASFS_CSG_BS_D
{
    T_ASFS_CSG_BS_BASE AsfsB[4];
	T_ASFS_CSG_BS_BASE AsfsFlgB[4];

    void Initialize()
    {
		for ( int i = 0; i < 4; ++i )
		{
			AsfsB[i].Initialize();
			AsfsFlgB[i].Initialize();
		}
    }
    T_ASFS_CSG_BS_D() { Initialize(); }
};


// Web Panel Reserve Factor Table (Beams with L. Stiffeners)
// Web yielding
struct T_ASWY_CSG_BS_BASE
{
    T_ASCD_CSG_BS_BASE ComD;

    T_CSGD_COMF  Panelij[2];    

    double dSig_yw;
    double dSig_1e_SV;
    double dTau_SV;
    double dSig_1e_DL;
    double dTau_DL;
    double dSig_1e_ST;
    double dTau_ST;
    double dRA;
    double dSA;
    double dSd;
    double dSst;
    double dS;
    double dSd_Sst;  // Sd* + Sha*
    double dA;       // Reserve Factor for only Standard Vehicle
    double dPsi;     // Ψ - Reserve Factor with Associated type HA Loading for Special Vehicle; BD 86/11
    double dPsi_ast; // Ψ* - Reserve Factor without Associated type HA Loading for Special Vehicle; BD 86/11
    double dRF;      // Maximan Reserve Factor for Assessment; max(dA, dPsi, dPsi_ast)
    int    nOK;

    void Initialize()
    {
        ComD.Initialize();
        for ( int i=0; i<2; ++i )
        {
            Panelij[i].Initialize();
        }
        dSig_yw   = 0.0;
        dSig_1e_SV = 0.0;
        dTau_SV   = 0.0;
        dSig_1e_DL = 0.0;
        dTau_DL   = 0.0;
        dSig_1e_ST = 0.0;
        dTau_ST   = 0.0;
        dRA       = 0.0;
        dSA       = 0.0;
        dSd       = 0.0;
        dSst      = 0.0;
        dS        = 0.0;
        dSd_Sst   = 0.0;
        dA        = 0.0;
        dPsi      = 0.0;
        dPsi_ast  = 0.0;
        dRF       = 0.0;
        nOK       = 0;
    }
    T_ASWY_CSG_BS_BASE() { Initialize(); }
};

struct T_ASWY_CSG_BS_UNIT
{
    T_ASWY_CSG_BS_UNIT()
    {
        dSig_yw    = D_UNITSYS_BASE_STRESS;
        dSig_1e_SV = D_UNITSYS_BASE_STRESS;
        dTau_SV    = D_UNITSYS_BASE_STRESS;
        dSig_1e_DL = D_UNITSYS_BASE_STRESS;
        dTau_DL    = D_UNITSYS_BASE_STRESS;
        dSig_1e_ST = D_UNITSYS_BASE_STRESS;
        dTau_ST    = D_UNITSYS_BASE_STRESS;
    }
    int dSig_yw;
    int dSig_1e_SV;
    int dTau_SV;
    int dSig_1e_DL;
    int dTau_DL;
    int dSig_1e_ST;
    int dTau_ST;
};


struct T_ASWY_CSG_BS_D
{
    T_ASWY_CSG_BS_BASE AswyB[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i ) AswyB[i].Initialize();
    }
    T_ASWY_CSG_BS_D() { Initialize(); }
};

// Web Buckling
struct T_ASWB_CSG_BS_BASE
{
    T_ASCD_CSG_BS_BASE ComD;

    double dSig_yw;
    double dK1;
    double dKb;
    double dKq;
    double dSig_1_SV;
    double dSig_b_SV;
    double dTau_SV;
    double dSig_1_DL;
    double dSig_b_DL;
    double dTau_DL;
    double dSig_1_ST;
    double dSig_b_ST;
    double dTau_ST;
    double dRA;      // Assessment Resistance RA*=Fc x R*
    double dSA;      // Sa* - Assessment Load Effect total Sa* = Sd* + Sha* + Ssv*
    double dSd;      // Sd* - Assessment Load Effect; Dead and superimposed dead loads
    double dSst;     // Sha* - Assessment Load Effect; Standard Vehicle
    double dS;       // S* - Assessment Load Effect; Special Vehicle
    double dSd_Sst;  // Sd* + Sha*
    double dA;       // Reserve Factor for only Standard Vehicle
    double dPsi;     // Ψ - Reserve Factor with Associated type HA Loading for Special Vehicle; BD 86/11
    double dPsi_ast; // Ψ* - Reserve Factor without Associated type HA Loading for Special Vehicle; BD 86/11
    double dRF;      // Maximan Reserve Factor for Assessment; max(dA, dPsi, dPsi_ast)
    int    nOK;

    void Initialize()
    {
        ComD.Initialize();
        dSig_yw   = 0.0;
        dK1 = 0.0;
        dKb = 0.0;
        dKq = 0.0;
        dSig_1_SV = 0.0;
        dSig_b_SV = 0.0;
        dTau_SV   = 0.0;
        dSig_1_DL = 0.0;
        dSig_b_DL = 0.0;
        dTau_DL   = 0.0;
        dSig_1_ST = 0.0;
        dSig_b_ST = 0.0;
        dTau_ST   = 0.0;
        dRA       = 0.0;
        dSA       = 0.0;
        dSd       = 0.0;
        dSst      = 0.0;
        dS        = 0.0;
        dSd_Sst   = 0.0;
        dA        = 0.0;
        dPsi      = 0.0;
        dPsi_ast  = 0.0;
        dRF       = 0.0;
        nOK       = 0;
    }
    T_ASWB_CSG_BS_BASE() { Initialize(); }
};

struct T_ASWB_CSG_BS_UNIT
{
    T_ASWB_CSG_BS_UNIT()
    {
        dSig_1_SV = D_UNITSYS_BASE_STRESS;
        dSig_b_SV = D_UNITSYS_BASE_STRESS;
        dTau_SV   = D_UNITSYS_BASE_STRESS;
        dSig_1_DL = D_UNITSYS_BASE_STRESS;
        dSig_b_DL = D_UNITSYS_BASE_STRESS;
        dTau_DL   = D_UNITSYS_BASE_STRESS;
        dSig_1_ST = D_UNITSYS_BASE_STRESS;
        dSig_b_ST = D_UNITSYS_BASE_STRESS;
        dTau_ST   = D_UNITSYS_BASE_STRESS;
    }
    int dSig_1_SV;
    int dSig_b_SV;
    int dTau_SV;
    int dSig_1_DL;
    int dSig_b_DL;
    int dTau_DL;
    int dSig_1_ST;
    int dSig_b_ST;
    int dTau_ST;
};

struct T_ASWB_CSG_BS_D
{
    T_ASWB_CSG_BS_BASE AswbB[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i ) AswbB[i].Initialize();
    }
    T_ASWB_CSG_BS_D() { Initialize(); }
};

// Stiffener
struct T_ASWS_CSG_BS_BASE
{
    T_ASCD_CSG_BS_BASE ComD;

    double dSig_ls;
    double dSig_se_SV;
    double dSig_se_DL;
    double dSig_se_ST;
    double dRA;      // Assessment Resistance RA*=Fc x R*
    double dSA;      // Sa* - Assessment Load Effect total Sa* = Sd* + Sha* + Ssv*
    double dSd;      // Sd* - Assessment Load Effect; Dead and superimposed dead loads
    double dSst;     // Sha* - Assessment Load Effect; Standard Vehicle
    double dS;       // S* - Assessment Load Effect; Special Vehicle
    double dSd_Sst;  // Sd* + Sha*
    double dA;       // Reserve Factor for only Standard Vehicle
    double dPsi;     // Ψ - Reserve Factor with Associated type HA Loading for Special Vehicle; BD 86/11
    double dPsi_ast; // Ψ* - Reserve Factor without Associated type HA Loading for Special Vehicle; BD 86/11
    double dRF;      // Maximan Reserve Factor for Assessment; max(dA, dPsi, dPsi_ast)
	double dIs1;
	double dIs1_min;
	double dIs2;
	double dIs2_min;
    int    nOK;

    void Initialize()
    {
        ComD.Initialize();
        dSig_ls    = 0.0;
        dSig_se_SV = 0.0;
        dSig_se_DL = 0.0;
        dSig_se_ST = 0.0;
        dRA        = 0.0;
        dSA        = 0.0;
        dSd        = 0.0;
        dSst       = 0.0;
        dS         = 0.0;
        dSd_Sst    = 0.0;
        dA         = 0.0;
        dPsi       = 0.0;
        dPsi_ast   = 0.0;
        dRF        = 0.0;
		dIs1       = 0.0;
		dIs1_min   = 0.0;
		dIs2       = 0.0;
		dIs2_min   = 0.0;
        nOK        = 0;
    }
    T_ASWS_CSG_BS_BASE() { Initialize(); }
};

struct T_ASWS_CSG_BS_UNIT
{
    T_ASWS_CSG_BS_UNIT()
    {
        dSig_ls    = D_UNITSYS_BASE_STRESS;
        dSig_se_SV = D_UNITSYS_BASE_STRESS;
        dSig_se_DL = D_UNITSYS_BASE_STRESS;
        dSig_se_ST = D_UNITSYS_BASE_STRESS;
		dIs1       = D_UNITSYS_BASE_STIF;
		dIs1_min   = D_UNITSYS_BASE_STIF;
		dIs2       = D_UNITSYS_BASE_STIF;
		dIs2_min   = D_UNITSYS_BASE_STIF;
    }
    int dSig_ls;
    int dSig_se_SV;
    int dSig_se_DL;
    int dSig_se_ST;
	int dIs1;
	int dIs1_min;
	int dIs2;
	int dIs2_min;
};


struct T_ASWS_CSG_BS_D
{
    T_ASWS_CSG_BS_BASE AswsB[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i ) AswsB[i].Initialize();
    }
    T_ASWS_CSG_BS_D() { Initialize(); }
};

// Slab Reserve Factor Table (Beams with L. Stiffeners)
struct T_ASSB_CSG_BS_BASE
{
    T_ASSM_STRESS_BS_BASE RFData;
    BOOL bIsSlabCr;

    void Initialize()
    {
        RFData.Initialize();
        bIsSlabCr = TRUE;
    }
    T_ASSB_CSG_BS_BASE() { Initialize(); }
};

struct T_ASSB_CSG_BS_D
{
    T_ASSB_CSG_BS_BASE AssbB[2]; // 0=i, 1=j

    void Initialize()
    {
        for ( int i=0; i<2; ++i ) AssbB[i].Initialize();
    }
    T_ASSB_CSG_BS_D() { Initialize(); }
};

// Reserve Factor Summary Table (Beams with L. Stiffeners)
struct T_ASRS_LS_CSG_BS_BASE
{
    BOOL     bChk;
    T_ELEM_K ElemK;
    UINT     unPosi;

    T_ASRS_BASE CFlg;
    T_ASRS_BASE TFlg;
    T_ASRS_BASE Web;
    T_ASRS_BASE SLS;
    T_ASRS_BASE LShear;

    int nOK;

    void Initialize()
    {
        bChk = FALSE;
        ElemK  = 0;
        unPosi = 0;
        CFlg.Initialize();
        TFlg.Initialize();
        Web.Initialize();
        SLS.Initialize();
        LShear.Initialize();
        nOK = 0;
    }
    T_ASRS_LS_CSG_BS_BASE() { Initialize(); }
};

struct T_ASRS_LS_CSG_BS_D
{
    T_ASRS_LS_CSG_BS_BASE AsrsB[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i ) AsrsB[i].Initialize();
    }
    T_ASRS_LS_CSG_BS_D() { Initialize(); }
};

// SLS Stress Verification Table (Beams with L. Stiffeners)
// struct T_ASSL_CSG_BS_BASE 사용

// Longitudinal Shear Verification Table (Beams with L. Stiffeners) 
// struct T_ASLS_CSG_BS_BASE 사용

// NR/GN/CIV/025 Rail Bridge
struct T_ASCA_RAT_STR_BS_B
{
    double   dSigmaR;  // Calculated Resistance R*
    double   dSigmaRA; // Assessment Resistance RA*=Fc x R*
    double   dSigmaT;  // total 
    double   dSigmaD;  // Dead and superimposed dead loads
    double   dSigmaSt; // Static
	double   dSigmaDy; // Dynamic

    T_ASCA_RAT_RAIL_BS_B RatPa; // Rating parameter

    int      nOK;

    void Initialize()
    {
        dSigmaR  = 0.0;
        dSigmaRA = 0.0;
        dSigmaT  = 0.0;
        dSigmaD  = 0.0;
        dSigmaSt = 0.0;
        dSigmaDy = 0.0;

		RatPa.Initialize();

        nOK      = 0;
    }
    T_ASCA_RAT_STR_BS_B() { Initialize(); }
};

struct T_ASCA_STR_BS_UNIT
{
    T_ASCA_STR_BS_UNIT()
    {
        dSigmaR  = D_UNITSYS_BASE_STRESS;
        dSigmaRA = D_UNITSYS_BASE_STRESS;
        dSigmaT  = D_UNITSYS_BASE_STRESS;
        dSigmaD  = D_UNITSYS_BASE_STRESS;
        dSigmaSt = D_UNITSYS_BASE_STRESS;
        dSigmaDy  = D_UNITSYS_BASE_STRESS;
    }
    int dSigmaR;
    int dSigmaRA;
    int dSigmaT;
    int dSigmaD;
    int dSigmaSt;
    int dSigmaDy;

};

struct T_ASCA_RAT_NON_BS_B
{
    double   dValR;  // Calculated Resistance R*
    double   dValRA; // Assessment Resistance RA*=Fc x R*
    double   dValT;  // total 
    double   dValD;  // Dead and superimposed dead loads
    double   dValSt; // Static
    double   dValDy;  // Dynamic

    T_ASCA_RAT_RAIL_BS_B RatPa; // Rating parameter

    int      nOK;

    void Initialize()
    {
        dValR  = 0.0;
        dValRA = 0.0;
        dValT  = 0.0;
        dValD  = 0.0;
        dValSt = 0.0;
        dValDy = 0.0;

        RatPa.Initialize();

        nOK = 0;
    }
    T_ASCA_RAT_NON_BS_B() { Initialize(); }
};


struct T_ASBS_SG_BS_BASE // Bearing stiffener 
{
    // Common Data
    T_ASCD_CSG_BS_BASE ComD;

	T_ASCA_RAT_STR_BS_B YieldingWeb;
    T_ASCA_RAT_STR_BS_B YieldingStiffener;
    T_ASCA_RAT_NON_BS_B BucklingEffStiffener;

    int nOK;

    void Initialize()
    {
        YieldingWeb.Initialize();
        YieldingStiffener.Initialize();
        BucklingEffStiffener.Initialize();
        nOK   = 0;
    }
    T_ASBS_SG_BS_BASE() { Initialize(); }
};

struct T_ASBS_SG_BS_UNIT
{
    T_ASBS_SG_BS_UNIT()
    {
        dR    = D_UNITSYS_BASE_MOMENT;
        dRA   = D_UNITSYS_BASE_MOMENT;
        dMu   = D_UNITSYS_BASE_MOMENT;
        dMuD  = D_UNITSYS_BASE_MOMENT;
        dMuSt = D_UNITSYS_BASE_MOMENT;
        dMuS  = D_UNITSYS_BASE_MOMENT;
    }
    int dR;
    int dRA;
    int dMu;
    int dMuD;
    int dMuSt;
    int dMuS;
};

struct T_ASBS_SG_BS_CASE
{
    // 0=I-Posi, 1=J-Posi
    T_ASBS_SG_BS_BASE AsbsB[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i )  AsbsB[i].Initialize();
    }
    T_ASBS_SG_BS_CASE() { Initialize(); }
};


struct T_ASTS_SG_BS_BASE // Transverse web stiffener other than at supports
{
    // Common Data
    T_ASCD_CSG_BS_BASE ComD;

	T_ASCA_RAT_STR_BS_B StrengthWeb;
    T_ASCA_RAT_STR_BS_B YieldingStiffener;
    T_ASCA_RAT_NON_BS_B BucklingStiffener;

    int nOK;

    void Initialize()
    {
        StrengthWeb.Initialize();
        YieldingStiffener.Initialize();
        BucklingStiffener.Initialize();
        nOK   = 0;
    }
    T_ASTS_SG_BS_BASE() { Initialize(); }
};

struct T_ASTS_SG_BS_UNIT
{
    T_ASTS_SG_BS_UNIT()
    {
        dR    = D_UNITSYS_BASE_MOMENT;
        dRA   = D_UNITSYS_BASE_MOMENT;
        dMu   = D_UNITSYS_BASE_MOMENT;
        dMuD  = D_UNITSYS_BASE_MOMENT;
        dMuSt = D_UNITSYS_BASE_MOMENT;
        dMuS  = D_UNITSYS_BASE_MOMENT;
    }
    int dR;
    int dRA;
    int dMu;
    int dMuD;
    int dMuSt;
    int dMuS;
};

struct T_ASTS_SG_BS_CASE
{
    // 0=I-Posi, 1=J-Posi
    T_ASTS_SG_BS_BASE AstsB[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i )  AstsB[i].Initialize();
    }
    T_ASTS_SG_BS_CASE() { Initialize(); }
};

struct T_ASSF_SG_BS_BASE // Longitudinal shear, Strength of Fastener
{
    // Common Data
    T_ASCD_CSG_BS_BASE ComD;

    T_ASCA_RAT_STR_BS_B Fastener;
    double dVh;
    double dTau;
    double dSigma_q;

    int nOK;

    void Initialize()
    {
        ComD.Initialize();
        Fastener.Initialize();
        dVh = 0.0;
        dTau = 0.0;
        dSigma_q = 0.0;
        nOK = 0;
    }
    T_ASSF_SG_BS_BASE() { Initialize(); }
};

struct T_ASSF_SG_BS_CASE
{
    // 0=I-Posi, 1=J-Posi
    T_ASSF_SG_BS_BASE AssfB[2];

    void Initialize()
    {
        for (int i = 0; i < 2; ++i)  AssfB[i].Initialize();
    }
    T_ASSF_SG_BS_CASE() { Initialize(); }
};

struct T_ASSF_SG_BS_UNIT
{
    T_ASSF_SG_BS_UNIT()
    {
        dVh        = D_UNITSYS_BASE_FORCE;
        dTau       = D_UNITSYS_BASE_STRESS;
        dSigma_q   = D_UNITSYS_BASE_STRESS;
    }
    int dVh;
    int dTau;
    int dSigma_q;
};

struct T_ASSM_ULS_CSG_BS_BASE
{
    T_ASFC_CSG_BS_BASE* pAsfcB;
    T_ASSC_CSG_BS_BASE* pAsscB;
    T_ASCB_CSG_BS_BASE* pAscbB;
    T_ASLS_CSG_BS_BASE* pAslsB;
    T_ASFG_CSG_BS_BASE* pAsfgB;
    T_ASFG_CSG_BS_BASE* pAsfgTenB; // 인장
    T_ASFS_CSG_BS_BASE* pAsfsB;
    T_ASFS_CSG_BS_BASE* pAsfsFlgB;
    T_ASWY_CSG_BS_BASE* pAswyB;
    T_ASWB_CSG_BS_BASE* pAswbB;
    T_ASWS_CSG_BS_BASE* pAswsB;
    T_ASSB_CSG_BS_BASE* pAssbB;
	T_ASBS_SG_BS_BASE*  pAsbsB; // Bearing stiffener
	T_ASSF_SG_BS_BASE*  pAssfB; // Longitudinal shear, Strength of Fastener

    void Initialize()
    {
        pAsfcB = nullptr;
        pAsscB = nullptr;
        pAscbB = nullptr;
        pAslsB = nullptr;
        pAsfgB = nullptr;
        pAsfgTenB = nullptr;
        pAsfsB = nullptr;
        pAswyB = nullptr;
        pAswbB = nullptr;
        pAswsB = nullptr;
        pAssbB = nullptr;
        pAsbsB = nullptr;
        pAssfB = nullptr;
    }
    T_ASSM_ULS_CSG_BS_BASE() { Initialize(); }

    void SetPtr(
        T_ASFC_CSG_BS_BASE* pAsfc,
        T_ASSC_CSG_BS_BASE* pAssc,
        T_ASCB_CSG_BS_BASE* pAscb,
        T_ASLS_CSG_BS_BASE* pAsls,
        T_ASFG_CSG_BS_BASE* pAsfg,
        T_ASFG_CSG_BS_BASE* pAsfgTen,
        T_ASFS_CSG_BS_BASE* pAsfs,
        T_ASFS_CSG_BS_BASE* pAsfsFlg,
        T_ASWY_CSG_BS_BASE* pAswy,
        T_ASWB_CSG_BS_BASE* pAswb,
        T_ASWS_CSG_BS_BASE* pAsws,
        T_ASSB_CSG_BS_BASE* pAssb,
        T_ASBS_SG_BS_BASE* pAsbs,
        T_ASSF_SG_BS_BASE* pAssf
        )
    {
        pAsfcB = pAsfc;
        pAsscB = pAssc;
        pAscbB = pAscb;
        pAslsB = pAsls;
        pAsfgB = pAsfg;
        pAsfgTenB = pAsfgTen;
        pAsfsB = pAsfs;
        pAsfsFlgB = pAsfsFlg;
        pAswyB = pAswy;
        pAswbB = pAswb;
        pAswsB = pAsws;
        pAssbB = pAssb;
        pAsbsB = pAsbs;
		pAssfB = pAssf;
    }
};






struct T_RARF_BASE
{
    UINT   RCaseK;        // Rating Case Key
    UINT   unConcurrent;  // Fx_max .... Mz_min(1~12)               [Unit=None] 
    BOOL   bChk;
    double dSRF;          // Static
    double dDRF;          // Dynamic
    double dSRA;          // Static Rating
    double dDRA;

    void Initialize()
    {
        RCaseK = 0;
        unConcurrent = 0;
        bChk = FALSE;
        dSRF = 0.0;
        dDRF = 0.0;
        dSRA = 0.0;
        dDRA = 0.0;
    }
    T_RARF_BASE() { Initialize(); }
};

struct T_RARF_MG_BS_BASE // Main girder, Assessment Rating Factor/Verification Summary
{
    BOOL     bChk;
    T_ELEM_K ElemK;
    UINT     unPosi;

    T_RARF_BASE PBend;
    T_RARF_BASE NBend;
    T_RARF_BASE Shear;
    T_RARF_BASE BearingSt;
    T_RARF_BASE TransSt;

    int nOK;

    void Initialize()
    {
        bChk = FALSE;
        ElemK  = 0;
        unPosi = 0;
        PBend.Initialize();
        NBend.Initialize();
        Shear.Initialize();
        BearingSt.Initialize();
        TransSt.Initialize();
        nOK = 0;
    }
    T_RARF_MG_BS_BASE() { Initialize(); }
};

struct T_ASRF_MG_BS_D
{
    T_RARF_MG_BS_BASE RarfMGB[2];

    void Initialize()
    {
        for (int i = 0; i < 2; ++i) RarfMGB[i].Initialize();
    }
    T_ASRF_MG_BS_D() { Initialize(); }
};


struct T_RARF_CG_BS_BASE // Cross girder, Assessment Rating Factor/Verification Summary
{
    BOOL     bChk;
    T_ELEM_K ElemK;
    UINT     unPosi;

    T_RARF_BASE PBend;
    T_RARF_BASE NBend;
    T_RARF_BASE Shear;
    T_RARF_BASE Bearer;
    T_RARF_BASE TransSt;

    int nOK;

    void Initialize()
    {
        bChk = FALSE;
        ElemK = 0;
        unPosi = 0;
        PBend.Initialize();
        NBend.Initialize();
        Shear.Initialize();
        Bearer.Initialize();
        TransSt.Initialize();
        nOK = 0;
    }
    T_RARF_CG_BS_BASE() { Initialize(); }
};

struct T_ASRF_CG_BS_D
{
    T_RARF_CG_BS_BASE RarfCGB[2];

    void Initialize()
    {
        for (int i = 0; i < 2; ++i) RarfCGB[i].Initialize();
    }
    T_ASRF_CG_BS_D() { Initialize(); }
};



#pragma pack()

#endif
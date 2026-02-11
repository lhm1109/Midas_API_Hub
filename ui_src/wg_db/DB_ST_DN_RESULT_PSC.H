#ifndef __DB_ST_DN_RESULT_PSC_H__
#define __DB_ST_DN_RESULT_PSC_H__

#define MIDAS_SDK

#include "DB_ST_DT_UNIT.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DGN_RAT_NOVAL   99.99
#define DGN_ZERO 1.0E-07
#define DGN_MAX  1.0E+70
#define D_PSC_FORCE_COMPONENT 9

#pragma pack(1)

enum AASHTO_LCOMTYPE
{
    AASHTO_LCOMTYPE_SERVICE0       = 1,
    AASHTO_LCOMTYPE_SERVICE1       = 2,
    AASHTO_LCOMTYPE_SERVICE2       = 3,
    AASHTO_LCOMTYPE_SERVICE3       = 4,
    AASHTO_LCOMTYPE_SERVICE4       = 5,
    AASHTO_LCOMTYPE_FATIGUE        = 6,
    AASHTO_LCOMTYPE_STRENGTH0      = 11,
    AASHTO_LCOMTYPE_STRENGTH1      = 12,
    AASHTO_LCOMTYPE_STRENGTH2      = 13,
    AASHTO_LCOMTYPE_STRENGTH3      = 14,
    AASHTO_LCOMTYPE_STRENGTH4      = 15,
    AASHTO_LCOMTYPE_STRENGTH5      = 16,
    AASHTO_LCOMTYPE_EXTREME_EVENT1 = 17,
    AASHTO_LCOMTYPE_EXTREME_EVENT2 = 18,   
    AASHTO_LCOMTYPE_SERVICE_IN_LIVE  = 19,  
};

struct T_FYPC_KEY
{
    UINT ElemK;
    UINT TdgrK;
    UINT TdnaK;
    int  nPos;  // 0:I, 1:J
    operator unsigned long() 
    { 
        if(TdgrK == 0)
            return (TdnaK<<24) + (ElemK<<1) + nPos; 
        else
            return (TdgrK<<24) + (ElemK<<1) + nPos;  
    }

    BOOL operator==(const T_FYPC_KEY& rKey) const
    {
        if(ElemK == rKey.ElemK && TdgrK == rKey.TdgrK && TdnaK == rKey.TdnaK && nPos == rKey.nPos) 
            return TRUE;
        else 
            return FALSE;
    }
};

struct T_SEQN_D
{
    int DcomNum;
    int ComfBaseNum;
    UINT ElemNum[59];

    void Initialize()
    {
        DcomNum = 0;
        ComfBaseNum = 0;
        for(int i=0; i<59; i++) ElemNum[i];
    }

};

struct T_COND_D
{
    int DcomNum;
    int ComfBaseNum;  

    void Initialize()
    {
        DcomNum = 0;
        ComfBaseNum = 0;    
    }
    T_COND_D () { Initialize();}  
};

struct T_SEQD_D
{
    UINT ElemK;
    UINT BeamType;
    LONGLONG iSeq;

    void Initialize()
    {
        ElemK = 0;
        BeamType = 0;
        iSeq = 0;
    }
    void Set(UINT ElemK, UINT BeamType, LONGLONG iSeq)
    {
        this->ElemK = ElemK;
		this->BeamType = BeamType;
		this->iSeq = iSeq;
    }
    ElemPairK GetEPairK() const
    {
        return ElemPairK(ElemK, BeamType);
	}
};

struct T_SMQD_D
{
    UINT MembK;
    LONGLONG iSeq;

    void Initialize()
    {
        MembK = 0;
        iSeq = 0;
    }
};

struct T_SEQE_D
{
    UINT LCaseNum[8];
    UINT ElemNum[8];

    void Initialize()
    {
        for(int i=0; i<7; i++) 
        {
            LCaseNum[i]=0;
            ElemNum[i]=0;
        }
    }
};

struct T_LCEM_D
{
    UINT ElemNum;
    LONGLONG iSeq;

    void Initialize()
    {
        ElemNum = 0;
        iSeq = 0;
 }
};

struct T_SEQL_D
{
    UINT nLoadCaseK;
    LONGLONG iSeq;

    void Initialize()
    {
        nLoadCaseK = 0;
        iSeq = 0;
    }
};

struct T_SEQK_D
{
    T_FYPC_KEY FypcKey;
    LONGLONG iSeq;
    void Initialize()
    {
        FypcKey.ElemK = 0;
        FypcKey.TdgrK = 0;
        FypcKey.nPos  = 0;
        iSeq = 0;
    }
};

struct T_DCOM_D
{
    UINT DgnLComK;
    UINT OrgLComK;

    void Initialize()
    {
        DgnLComK = 0;
        OrgLComK = 0;
    }
};

// 설계결과 유무 판단용
struct T_DRST_D_895
{
    BOOL bFysg; // 시공단계 응력
    BOOL bFycm; // 완성계 응력
    BOOL bFypc; // Tendon 검토
    BOOL bFpsg; // 시공단계 주응력
    BOOL bFpms; // 완성계 주응력
    BOOL bBdcw; // 균열검토
    BOOL bTbar; // 인장철근 
    BOOL bBdcr; // 휨 강도
    BOOL bShrr; // 전단 강도
    BOOL bTorr; // 비틀림 강도

    void Initialize()
    {
        bFysg = bFycm = bFypc = bFpsg = bFpms = FALSE;
        bBdcw = bTbar = bBdcr = bShrr = bTorr = FALSE;
    }
    T_DRST_D_895() { Initialize(); }
};

struct T_DRST_D
{
    BOOL bFysg; // 시공단계 응력
    BOOL bFycm; // 완성계 응력
    BOOL bFypc; // Tendon 검토
    BOOL bFpsg; // 시공단계 주응력
    BOOL bFpms; // 완성계 주응력
    BOOL bBdcw; // 균열검토
    BOOL bTbar; // 인장철근 
    BOOL bBdcr; // 휨 강도
    BOOL bShrr; // 전단 강도
    BOOL bTorr; // 비틀림 강도
    BOOL bTran; // Transfer 응력
    BOOL bTrcr; // Transfer 균열

    void Initialize()
    {
        bFysg = bFycm = bFypc = bFpsg = bFpms = FALSE;
        bBdcw = bTbar = bBdcr = bShrr = bTorr = FALSE;   
        bTran = bTrcr = FALSE;
    }
    T_DRST_D () {Initialize();}

    void Convert895(const T_DRST_D_895 &rData)
    {
        bFysg = rData.bFysg;
        bFycm = rData.bFycm;
        bFypc = rData.bFypc;
        bFpsg = rData.bFpsg;
        bFpms = rData.bFpms;
        bBdcw = rData.bBdcw;
        bTbar = rData.bTbar;
        bBdcr = rData.bBdcr;
        bShrr = rData.bShrr;
        bTorr = rData.bTorr;
        bTran = FALSE;
        bTrcr = FALSE;
    }
};

// Bridge Load Rating 
struct T_SQRT_D
{
    UINT nElemNum[2]; // 0=설계 요소, 1=Tendon이 존재하는 요소

    void Initialize()
    {
        for(int i=0; i<2; i++) nElemNum[i];
    }
};


struct T_PSCD_FORCE
{
    double dForce[D_PSC_FORCE_COMPONENT];

    void Initialize()
    {
        memset(dForce, 0, sizeof(dForce));
    }
    T_PSCD_FORCE() { Initialize(); }
};

struct T_PSCD_FORCE_UNIT
{
    T_PSCD_FORCE_UNIT()
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
    int dForce[D_PSC_FORCE_COMPONENT];
};

// 1. 가설중 휨 응력도 (FY_SG)
#define T_FYSG_K unsigned int
struct T_FYSG_BASE
{
	BOOL bCHK;		//설계결과유무
    UINT StagK;		//스테이지번호		[Unit=None]
    UINT StepK;   // Step Key.      [Unit=None]
    double dFT;   //상단응력도
    double dFB;   //하단응력도
    double dFTL;	//상단좌단 합성응력도	[Unit=Stress]
    double dFBL;	//하단좌단 합성응력도	[Unit=Stress]
    double dFTR;	//상단우단 합성응력도	[Unit=Stress]
    double dFBR;	//하단우단 합성응력도	[Unit=Stress]
    double dFMAX;	//최대/최소 합성응력도	[Unit=Stress]
    double dALW;	//허용응력도			[Unit=Stress]
    
    double dFT_PS;	//[Unit=Stress] // Stress due to prestress only
    double dFB_PS;	//[Unit=Stress]
    double dFTL_PS;	//[Unit=Stress]
    double dFBL_PS;	//[Unit=Stress]
    double dFTR_PS;	//[Unit=Stress]
    double dFBR_PS;	//[Unit=Stress]
    double dFMAX_PS; //[Unit=Stress]

    double dfci;  // 초기압축강도, t(day)에 대한 압축강도 
    double dTtfc; // 인장영역에서 콘크리트 인장력(Total tensile Force in conrete)
    double dAct;  // 인장영역 면적
    double dTtfs; // 인장영역에서 철근 인장력(Total tensile Force in steel) 
    double dAst;  // 인장영력에서 철근 면적
    double dFs;   // 철근 응력
    double dRatio;
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        StagK = StepK = 0;
        dFT = dFB = 0.0;
        dFTL = dFBL = dFTR = dFBR = dFMAX = dALW = 0.0;

        dFT_PS   = 0.0;   
        dFB_PS   = 0.0;	  
        dFTL_PS  = 0.0;	
        dFBL_PS  = 0.0;	
        dFTR_PS  = 0.0;	
        dFBR_PS  = 0.0;	
        dFMAX_PS = 0.0; 

        dfci = 0.0;
        dTtfc = dAct = dTtfs = dAst = dFs = 0.0;
        dRatio = 0.0;
        bOK = TRUE;
    }

    T_FYSG_BASE()	{ Initialize(); }
	T_FYSG_BASE(const T_FYSG_BASE& rData)	{*this = rData;}
	T_FYSG_BASE& operator = (const T_FYSG_BASE& rData)
	{
        bCHK   = rData.bCHK  ;
        StagK  = rData.StagK ;
        StepK  = rData.StepK ;
        dFT    = rData.dFT   ;
        dFB    = rData.dFB   ;
        dFTL   = rData.dFTL  ;
        dFBL   = rData.dFBL  ;
        dFTR   = rData.dFTR  ;
        dFBR   = rData.dFBR  ;
        dFMAX  = rData.dFMAX ;
        dALW   = rData.dALW  ;

        dFT_PS   = rData.dFT_PS  ;
        dFB_PS   = rData.dFB_PS  ;
        dFTL_PS  = rData.dFTL_PS ;
        dFBL_PS  = rData.dFBL_PS ;
        dFTR_PS  = rData.dFTR_PS ;
        dFBR_PS  = rData.dFBR_PS ;
        dFMAX_PS = rData.dFMAX_PS;
        
        dfci   = rData.dfci  ;
        dTtfc  = rData.dTtfc ;
        dAct   = rData.dAct  ;
        dTtfs  = rData.dTtfs ;
        dAst   = rData.dAst  ;
        dFs    = rData.dFs   ;
        dRatio = rData.dRatio;
        bOK    = rData.bOK   ;
		return *this;
	}
};

struct T_FYSG_D	// ElemK.
{
    T_FYSG_BASE FysgBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; ++i)
            FysgBase[i].Initialize();
    }
    T_FYSG_D()	{ Initialize(); }
	T_FYSG_D(const T_FYSG_D& rData)	{*this = rData;}
	T_FYSG_D& operator = (const T_FYSG_D& rData)
	{
        for(int i=0; i<4; ++i) FysgBase[i] = rData.FysgBase[i];
		return *this;
	}

};

struct T_FYSG_D_COMP	// ElemK.
{
    T_FYSG_BASE FysgGrBase[4];	//(Girder)  0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    T_FYSG_BASE FysgSlBase[4];	//(Slab)    0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; ++i)
        {
            FysgGrBase[i].Initialize();
            FysgSlBase[i].Initialize();
        }
    }
    T_FYSG_D_COMP()	{ Initialize(); }
    T_FYSG_D_COMP(const T_FYSG_D_COMP& rData)	{*this = rData;}
    T_FYSG_D_COMP& operator = (const T_FYSG_D_COMP& rData)
    {
        for(int i=0; i<4; ++i)
        {
            FysgGrBase[i] = rData.FysgGrBase[i];
            FysgSlBase[i] = rData.FysgSlBase[i];
        }
        return *this;
    }
};


struct T_FYSG_UNIT
{
    T_FYSG_UNIT()
    {
        dFT   = D_UNITSYS_BASE_STRESS;
        dFB   = D_UNITSYS_BASE_STRESS;
        dFTL  = D_UNITSYS_BASE_STRESS;
        dFBL  = D_UNITSYS_BASE_STRESS;
        dFTR  = D_UNITSYS_BASE_STRESS;
        dFBR  = D_UNITSYS_BASE_STRESS;
        dFMAX = D_UNITSYS_BASE_STRESS;
        dALW  = D_UNITSYS_BASE_STRESS;
        dFT_PS   = D_UNITSYS_BASE_STRESS;   
        dFB_PS   = D_UNITSYS_BASE_STRESS;	  
        dFTL_PS  = D_UNITSYS_BASE_STRESS;	
        dFBL_PS  = D_UNITSYS_BASE_STRESS;	
        dFTR_PS  = D_UNITSYS_BASE_STRESS;	
        dFBR_PS  = D_UNITSYS_BASE_STRESS;	
        dFMAX_PS = D_UNITSYS_BASE_STRESS; 
        dfci  = D_UNITSYS_BASE_STRESS;
        dTtfc = D_UNITSYS_BASE_FORCE;
        dAct  = D_UNITSYS_BASE_AREA;
        dTtfs = D_UNITSYS_BASE_FORCE;
        dAst  = D_UNITSYS_BASE_AREA;
        dFs   = D_UNITSYS_BASE_STRESS;
    }
    int dFT;
    int dFB;
    int dFTL;
    int dFBL;
    int dFTR;
    int dFBR;
    int dFMAX;
    int dFT_PS  ;   
    int dFB_PS  ;	  
    int dFTL_PS ;	
    int dFBL_PS ;	
    int dFTR_PS ;	
    int dFBR_PS ;	
    int dFMAX_PS; 

    int dALW;
    int dfci;
    int  dTtfc;
    int  dAct ;
    int  dTtfs;
    int  dAst ;
    int  dFs  ;
};
// 2. PC강재 최대응력도 (FY_PC)
#define T_FYPC_K unsigned int
struct T_FYPC_BASE
{
    BOOL bCHK;		//설계결과유무
    UINT DgnTdgrK;		//텐던그룹번호		[Unit=None]
    double dFDL;	//유효 응력도			[Unit=Stress]
    double dFLL;	//활하중시 응력도		[Unit=Stress]
    double dAFDL;	//허용값 유효			[Unit=Stress]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        DgnTdgrK = 0;
        dFDL = dFLL = dAFDL = 0;
        bOK = TRUE;
    }

};

struct T_FYPC_D	// ElemK.
{
    CArray<T_FYPC_BASE,T_FYPC_BASE> FypcBase[2];	// 0=I, 1=J.

    void Initialize()
    {
        for(int i=0; i<2; i++)
        {
            FypcBase[i].RemoveAll();
        }
    }

};
struct T_FYPC_UNIT
{
    T_FYPC_UNIT()
    {
        dFDL = D_UNITSYS_BASE_STRESS;
        dFLL = D_UNITSYS_BASE_STRESS;
        dAFDL = D_UNITSYS_BASE_STRESS;
    }
    int dFDL;
    int dFLL;
    int dAFDL;

};
// 3. 합성응력도 (FY_CM)
#define T_FYCM_K unsigned int
struct T_FYCM_BASE
{
    BOOL bCHK;		//설계결과유무 
    UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iKind;       //(1:전사하중시, 2:설계시, 3:온도시, 4:지진시)
    double dFT;   //상단응력도
    double dFB;   //하단응력도
    double dFTL;	//상단좌단 합성응력도	[Unit=Stress]
    double dFBL;	//하단좌단 합성응력도	[Unit=Stress]
    double dFTR;	//상단우단 합성응력도	[Unit=Stress]
    double dFBR;	//하단우단 합성응력도	[Unit=Stress]
    double dFMAX;	//최대/최소 합성응력도	[Unit=Stress]
    double dALW;	//허용응력도			[Unit=Stress]

    double dFT_PS;	//[Unit=Stress] // Stress due to prestress only
    double dFB_PS;	//[Unit=Stress]
    double dFTL_PS;	//[Unit=Stress]
    double dFBL_PS;	//[Unit=Stress]
    double dFTR_PS;	//[Unit=Stress]
    double dFBR_PS;	//[Unit=Stress]
    double dFMAX_PS; //[Unit=Stress]

    double dRatio;
    BOOL bOK;
	BOOL bLTerm; // Long Term

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        iKind = 0;
        dFT = dFB = 0.0;
        dFTL = dFBL = dFTR = dFBR = dFMAX = dALW = 0;
        dFT_PS   = 0.0;   
        dFB_PS   = 0.0;	  
        dFTL_PS  = 0.0;	
        dFBL_PS  = 0.0;	
        dFTR_PS  = 0.0;	
        dFBR_PS  = 0.0;	
        dFMAX_PS = 0.0; 

        dRatio = 0.0;
        bOK = TRUE;
        bLTerm = FALSE;
    }
};

struct T_FYCM_D	// ElemK.
{
    T_FYCM_BASE FycmBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            FycmBase[i].Initialize();
    }

};

struct T_FYCM_UNIT
{
    T_FYCM_UNIT()
    {
        dFT   = D_UNITSYS_BASE_STRESS;
        dFB   = D_UNITSYS_BASE_STRESS;
        dFTL  = D_UNITSYS_BASE_STRESS;
        dFBL  = D_UNITSYS_BASE_STRESS;
        dFTR  = D_UNITSYS_BASE_STRESS;
        dFBR  = D_UNITSYS_BASE_STRESS;
        dFMAX = D_UNITSYS_BASE_STRESS;
        dALW  = D_UNITSYS_BASE_STRESS;
        dFT_PS   = D_UNITSYS_BASE_STRESS;   
        dFB_PS   = D_UNITSYS_BASE_STRESS;	  
        dFTL_PS  = D_UNITSYS_BASE_STRESS;	
        dFBL_PS  = D_UNITSYS_BASE_STRESS;	
        dFTR_PS  = D_UNITSYS_BASE_STRESS;	
        dFBR_PS  = D_UNITSYS_BASE_STRESS;	
        dFMAX_PS = D_UNITSYS_BASE_STRESS; 

    }
    int dFT;
    int dFB;
    int dFTL ;
    int dFBL ;
    int dFTR ;
    int dFBR ;
    int dFMAX;
    int dALW ;
    int dFT_PS  ;   
    int dFB_PS  ;	  
    int dFTL_PS ;	
    int dFBL_PS ;	
    int dFTR_PS ;	
    int dFBR_PS ;	
    int dFMAX_PS; 

};

// 4. 휨 균열폭 (BND_CW)
#define T_BDCW_K unsigned int
struct T_BDCW_BASE
{
    BOOL bCHK;		//설계결과유무
    UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iKind;       //(1:전사하중시, 2:설계시, 3:온도시, 4:지진시)
    double dFT;		//상단응력도			[Unit=Stress]
    double dFB;		//하단응력도			[Unit=Stress]
    double dFRT;	//인장철근응력도		[Unit=Stress]
    double dWC;	  //균열폭			[Unit=Length]
    double dAWC;	//인장단부허용균열폭	[Unit=Length]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        iKind = 0;
        dFT = dFB = dFRT = dWC = dAWC = 0;
        bOK = TRUE;
    }

};

struct T_BDCW_D	// ElemK.
{
    T_BDCW_BASE BdcwBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            BdcwBase[i].Initialize();
    }

};

struct T_BDCW_UNIT
{
    T_BDCW_UNIT()
    {
        dFT = D_UNITSYS_BASE_STRESS;
        dFB = D_UNITSYS_BASE_STRESS;
        dFRT = D_UNITSYS_BASE_STRESS;
        dWC  = D_UNITSYS_BASE_LENGTH;
        dAWC = D_UNITSYS_BASE_LENGTH;
    }
    int dFT;
    int dFB;
    int dFRT;
    int dWC;
    int dAWC;
};

// 5. 인장철근 (RBAR_T)
#define T_TBAR_K unsigned int
struct T_TBAR_BASE
{
    BOOL bCHK;		//설계결과유무
    UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iKind;       //(1:전사하중시, 2:설계시, 3:온도시, 4:지진시)
    double dREQ;	//필요철근량			[Unit=Area]
    double dMIN;	//최소철근량			[Unit=Area]
    double dUSE;	//사용철근량			[Unit=Area]
    BOOL bOK;
    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        iKind = 0;
        dREQ = dMIN = dUSE = 0;
        bOK = TRUE;
    }

};

struct T_TBAR_D	// ElemK.
{
    T_TBAR_BASE TbarBase[4];	// 0=I-TOP, 1=I-BOT, 2:J-TOP, 3:J-Bot.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            TbarBase[i].Initialize();
    }
};

struct T_TBAR_UNIT
{
    T_TBAR_UNIT()
    {
        dREQ = D_UNITSYS_BASE_AREA;
        dMIN = D_UNITSYS_BASE_AREA;
        dUSE = D_UNITSYS_BASE_AREA;
    }
    int dREQ;
    int dMIN;
    int dUSE;
};

// 6. 휨파괴안전도 (BND_CR)
#define T_BDCR_K unsigned int
struct T_BDCR_BASE
{
    BOOL bCHK;		//설계결과유무
    UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iKind;       //(1:전사하중시, 2:설계시, 3:온도시, 4:지진시)
    double dAX;		//종국축력			[Unit=Force]
    double dMUY;	//면내 종국모멘트		[Unit=Moment]
    double dMRY;	//면내 파괴저항 모멘트	[Unit=Moment]
    double dRATY;	//면내 안전율			[Unit=None]
    double dMUZ;	//면외 종국모멘트		[Unit=Moment]
    double dMRZ;	//면외 파괴저항 모멘트	[Unit=Moment]
    double dRATZ;	//면외 안전율			[Unit=None]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        iKind = 0;
        dAX = dMUY = dMRY = dRATY = dMUZ = dMRZ = dRATZ = 0;
        bOK = TRUE;
    }
};

struct T_BDCR_D	// ElemK.
{
    T_BDCR_BASE BdcrBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            BdcrBase[i].Initialize();
    }
};

struct T_BDCR_UNIT
{
    T_BDCR_UNIT()
    {
        dAX = D_UNITSYS_BASE_FORCE;
        dMUY = D_UNITSYS_BASE_MOMENT;
        dMRY = D_UNITSYS_BASE_MOMENT;
        dRATY = D_UNITSYS_NONE;
        dMUZ = D_UNITSYS_BASE_MOMENT;
        dMRZ = D_UNITSYS_BASE_MOMENT;
        dRATZ = D_UNITSYS_NONE;
    }
    int dAX;
    int dMUY;
    int dMRY;
    int dRATY;
    int dMUZ;
    int dMRZ;
    int dRATZ;
};

// 7. 가설중 경사 인장 응력도 (FST_SG)
#define T_FSSG_K unsigned int
struct T_FSSG_BASE
{
    BOOL bCHK;		//설계결과유무 
    UINT StagK;		  //스테이지번호			[Unit=None]
    double dFS1;	//2-2위치 전단응력도		[Unit=Stress]
    double dFSG	;	//G-G위치 전단응력도		[Unit=Stress]
    double dFS3	;	//3-3위치 전단응력도		[Unit=Stress]
    double dAFS	;	//허용 전단응력도			[Unit=Stress]
    double dFST1;	//2-2위치 전단+비틀림 응력도	[Unit=Stress]
    double dFSTG;	//G-G위치 전단+비틀림 응력도	[Unit=Stress]
    double dFST3;	//3-3위치 전단+비틀림 응력도	[Unit=Stress]
    double dAFST;	//허용 전단+비틀림 응력도	[Unit=Stress]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        StagK = 0;
        dFS1 = dFSG = dFS3 = dAFS = dFST1 = dFSTG = dFST3 = dAFST = 0;
        bOK = TRUE;
    }
};

struct T_FSSG_D	// ElemK.
{
    T_FSSG_BASE FssgBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            FssgBase[i].Initialize();
    }

};

struct T_FSSG_UNIT
{
    T_FSSG_UNIT()
    {
        dFS1  = D_UNITSYS_BASE_STRESS;
        dFSG  = D_UNITSYS_BASE_STRESS;
        dFS3  = D_UNITSYS_BASE_STRESS;
        dAFS  = D_UNITSYS_BASE_STRESS;
        dFST1 = D_UNITSYS_BASE_STRESS;
        dFSTG = D_UNITSYS_BASE_STRESS;
        dFST3 = D_UNITSYS_BASE_STRESS;
        dAFST = D_UNITSYS_BASE_STRESS;
    }
    int dFS1 ;
    int dFSG	;
    int dFS3	;
    int dAFS	;
    int dFST1;
    int dFSTG;
    int dFST3;
    int dAFST;
};
// 8. 설계하중시 경사 인장 응력도 (전단력 최대시) (FST_MS)
#define T_FSMS_K unsigned int
struct T_FSMS_BASE
{
    BOOL bCHK;		//설계결과유무 
    UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iKind;       //(1:전사하중시, 2:설계시, 3:온도시, 4:지진시)
    double dFS1;	//2-2위치 전단응력도		[Unit=Stress]
    double dFSG	;	//G-G위치 전단응력도		[Unit=Stress]
    double dFS3	;	//3-3위치 전단응력도		[Unit=Stress]
    double dAFS	;	//허용 전단응력도			[Unit=Stress]
    double dFST1;	//2-2위치 전단+비틀림 응력도	[Unit=Stress]
    double dFSTG;	//G-G위치 전단+비틀림 응력도	[Unit=Stress]
    double dFST3;	//3-3위치 전단+비틀림 응력도	[Unit=Stress]
    double dAFST;	//허용 전단+비틀림 응력도	[Unit=Stress]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        iKind = 0;
        dFS1 = dFSG = dFS3 = dAFS = dFST1 = dFSTG = dFST3 = dAFST = 0;
        bOK = TRUE;
    }
};

struct T_FSMS_D	// ElemK.
{
    T_FSMS_BASE FsmsBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            FsmsBase[i].Initialize();
    }
};

struct T_FSMS_UNIT
{
    T_FSMS_UNIT()
    {
        dFS1  = D_UNITSYS_BASE_STRESS;
        dFSG  = D_UNITSYS_BASE_STRESS;
        dFS3  = D_UNITSYS_BASE_STRESS;
        dAFS  = D_UNITSYS_BASE_STRESS;
        dFST1 = D_UNITSYS_BASE_STRESS;
        dFSTG = D_UNITSYS_BASE_STRESS;
        dFST3 = D_UNITSYS_BASE_STRESS;
        dAFST = D_UNITSYS_BASE_STRESS;
    }
    int dFS1 ;
    int dFSG	;
    int dFS3	;
    int dAFS	;
    int dFST1;
    int dFSTG;
    int dFST3;
    int dAFST;
};

// 9. 설계하중시 전단균열 (전단력 최대시) (CW_MS)
#define T_CWMS_K unsigned int
struct T_CWMS_BASE
{
    BOOL bCHK;		//설계결과유무 
    UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iKind;       //(1:전사하중시, 2:설계시, 3:온도시, 4:지진시)
    double dFV;   //평균전단응력도 (전단)
    double dFVT;  //평균전단응력도 (전단+비틀림)
    double dFS	;	//가상 경사인장응력도 (전단)				[Unit=Stress]
    double dFST;	//가상 경사인장응력도 (전단+비틀림)			[Unit=Stress]
    double dAFS;	//가상 경사인장응력도 허용값				[Unit=Stress]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        iKind = 0;
        dFV = dFVT = 0.0;
        dFS = dFST = dAFS = 0.0;
        bOK = TRUE;
    }
};

struct T_CWMS_D	// ElemK.
{
    T_CWMS_BASE CwmsBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            CwmsBase[i].Initialize();
    }
};

struct T_CWMS_UNIT
{
    T_CWMS_UNIT()
    {
        dFV  = D_UNITSYS_BASE_STRESS;
        dFVT = D_UNITSYS_BASE_STRESS;
        dFS  = D_UNITSYS_BASE_STRESS;
        dFST = D_UNITSYS_BASE_STRESS;
        dAFS = D_UNITSYS_BASE_STRESS;
    }
    int dFV ;
    int dFVT;
    int dFS ;
    int dFST;
    int dAFS;
};

// 10. 종국하중시 웹 또는 플랜지 압괴내력에 대한 검토 (전단력 최대시) (CRU_MS)
#define T_CUMS_K unsigned int
struct T_CUMS_BASE
{
    BOOL bCHK;		//설계결과유무 
    UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iKind;       //(11:종국시A, 12:종국시B, 13:종국시C)
    double dQU;		//종국시 전단력		[Unit=Force]
    double dQR;		//웹압괴 내력			[Unit=Force]
    double dTU;		//종국시 비틀림		[Unit=Moment]
    double dTR; 	//웹또는 플랜지 압괴내력			[Unit=Moment]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        iKind = 0;
        dQU = dQR = dTU = dTR = 0.0;
        bOK = TRUE;
    }
};

struct T_CUMS_D	// ElemK.
{
    T_CUMS_BASE CumsBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            CumsBase[i].Initialize();
    }
};

struct T_CUMS_UNIT
{
    T_CUMS_UNIT()
    {
        dQU = D_UNITSYS_BASE_FORCE;
        dQR = D_UNITSYS_BASE_FORCE;
        dTU = D_UNITSYS_BASE_MOMENT;
        dTR = D_UNITSYS_BASE_MOMENT;
    }
    int dQU;
    int dQR;
    int dTU;
    int dTR;
};

// 11. 종국하중시 경사 파괴 내력에 대한 검토 (전단력 최대시) (CR_MS)
#define T_CRMS_K unsigned int

struct T_CRMS_BASE
{
    BOOL bCHK;		//설계결과유무 
    UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iKind;       //(11:종국시A, 12:종국시B, 13:종국시C)
    double dQU;		//종국시 전단력		[Unit=Force]
    double dQR;		//전단경사 인장 내력	[Unit=Force]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        iKind = 0;
        dQU = dQR = 0;
        bOK = TRUE;
    }
};

struct T_CRMS_D	// ElemK.
{
    T_CRMS_BASE CrmsBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            CrmsBase[i].Initialize();
    }
};

struct T_CRMS_UNIT
{
    T_CRMS_UNIT()
    {
        dQU = D_UNITSYS_BASE_FORCE;
        dQR = D_UNITSYS_BASE_FORCE;
    }
    int dQU;
    int dQR;
};

// 12. 설계하중시 경사 인장 응력도 (비틀림 최대시) (FST_MT)
#define T_FSMT_K unsigned int

struct T_FSMT_BASE
{
    BOOL bCHK;		//설계결과유무 
    UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iKind;       //(1:전사하중시, 2:설계시, 3:온도시, 4:지진시)
    double dFS1;	//2-2위치 전단응력도		[Unit=Stress]
    double dFSG	;	//G-G위치 전단응력도		[Unit=Stress]
    double dFS3	;	//3-3위치 전단응력도		[Unit=Stress]
    double dAFS	;	//허용 전단응력도			[Unit=Stress]
    double dFST1;	//2-2위치 전단+비틀림 응력도	[Unit=Stress]
    double dFSTG;	//G-G위치 전단+비틀림 응력도	[Unit=Stress]
    double dFST3;	//3-3위치 전단+비틀림 응력도	[Unit=Stress]
    double dAFST;	//허용 전단+비틀림 응력도	[Unit=Stress]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        iKind = 0;
        dFS1 = dFSG = dFS3 = dAFS = dFST1 = dFSTG = dFST3 = dAFST = 0;
        bOK = TRUE;
    }
};

struct T_FSMT_D	// ElemK.
{
    T_FSMT_BASE FsmtBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            FsmtBase[i].Initialize();
    }
};

struct T_FSMT_UNIT
{
    T_FSMT_UNIT()
    {
        dFS1  = D_UNITSYS_BASE_STRESS;
        dFSG	= D_UNITSYS_BASE_STRESS;
        dFS3	= D_UNITSYS_BASE_STRESS;
        dAFS	= D_UNITSYS_BASE_STRESS;
        dFST1 = D_UNITSYS_BASE_STRESS;
        dFSTG = D_UNITSYS_BASE_STRESS;
        dFST3 = D_UNITSYS_BASE_STRESS;
        dAFST = D_UNITSYS_BASE_STRESS;
    }
    int dFS1 ;
    int dFSG	;
    int dFS3	;
    int dAFS	;
    int dFST1;
    int dFSTG;
    int dFST3;
    int dAFST;
};

// 13. 설계하중시 전단균열 (비틀림 최대시) (CW_MT)
#define T_CWMT_K unsigned int

struct T_CWMT_BASE
{
    BOOL bCHK;		//설계결과유무 
    UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iKind;       //(1:전사하중시, 2:설계시, 3:온도시, 4:지진시)
    double dFV	;	//평균전단응력도 (전단)				[Unit=Stress]
    double dFVT;	//평균전단응력도 (전단+비틀림)			[Unit=Stress]
    double dFS	;	//가상 경사인장응력도 (전단)				[Unit=Stress]
    double dFST;	//가상 경사인장응력도 (전단+비틀림)			[Unit=Stress]
    double dAFS;  //허용값			[Unit=Stress]
    BOOL bOK;
    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        iKind = 0;
        dFV = dFVT = dFS = dFST = dAFS = 0.0;
        bOK = TRUE;
    }
};

struct T_CWMT_D	// ElemK.
{
    T_CWMT_BASE CwmtBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            CwmtBase[i].Initialize();
    }
};

struct T_CWMT_UNIT
{
    T_CWMT_UNIT()
    {
        dFV	 = D_UNITSYS_BASE_STRESS;
        dFVT = D_UNITSYS_BASE_STRESS;
        dFS  = D_UNITSYS_BASE_STRESS;
        dFST = D_UNITSYS_BASE_STRESS;
        dAFS = D_UNITSYS_BASE_STRESS;
    }
    int dFV;
    int dFVT;
    int dFS;
    int dFST;
    int dAFS;
};

// 14. 종국하중시 웹 또는 플랜지 압괴내력에 대한 검토 (비틀림 최대시) (CU_MT)
#define T_CUMT_K unsigned int

struct T_CUMT_BASE
{
    BOOL bCHK;		//설계결과유무 
    UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iKind;       //(11:종국시A, 12:종국시B, 13:종국시C)
    double dQU;		//종국시 전단력		[Unit=Force]
    double dQR;		//전단 내력			  [Unit=Force]
    double dTU;		//종국시 비틀림		[Unit=Moment]
    double dTR; 	//비틀림 내력			[Unit=Moment]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        iKind = 0;
        dQU = dQR = dTU = dTR = 0;
        bOK = TRUE;
    }
};

struct T_CUMT_D	// ElemK.
{
    T_CUMT_BASE CumtBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            CumtBase[i].Initialize();
    }
};

struct T_CUMT_UNIT
{
    T_CUMT_UNIT()
    {
        dQU = D_UNITSYS_BASE_FORCE;
        dQR = D_UNITSYS_BASE_MOMENT;
        dTU = D_UNITSYS_BASE_MOMENT;
        dTR = D_UNITSYS_BASE_MOMENT;
    }
    int dQU;
    int dQR;
    int dTU;
    int dTR;
};

// 15. 종국하중시 경사 파괴 내력에 대한 검토 (비틀림 최대시) (CR_MT)
#define T_CRMT_K unsigned int

struct T_CRMT_BASE
{
    BOOL bCHK;		//설계결과유무 
    UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iKind;       //(11:종국시A, 12:종국시B, 13:종국시C)
    double dTU;		//종국시 비틀림		      [Unit=Moment]
    double dTR; 	//비틀림경사 인장 내력	[Unit=Moment]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        iKind = 0;
        iKind = 0;
        dTU = dTR = 0;
        bOK = TRUE;
    }
};

struct T_CRMT_D	// ElemK.
{
    T_CRMT_BASE CrmtBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            CrmtBase[i].Initialize();
    }
};

struct T_CRMT_UNIT
{
    T_CRMT_UNIT()
    {
        dTU = D_UNITSYS_BASE_MOMENT;
        dTR = D_UNITSYS_BASE_MOMENT;
    }
    int dTU;
    int dTR;
};

// 16. 합성응력도 (하중조합별)
#define T_FYLC_K unsigned int
struct T_FYLC_BASE
{
    BOOL bCHK;		//설계결과유무 
    UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    double dFT;   //상단응력도
    double dFB;   //하단응력도
    double dFTL;	//상단좌단 합성응력도	[Unit=Stress]
    double dFBL;	//하단좌단 합성응력도	[Unit=Stress]
    double dFTR;	//상단우단 합성응력도	[Unit=Stress]
    double dFBR;	//하단우단 합성응력도	[Unit=Stress]

    double dFTP;  //TP에의한 상단응력도
    double dFBP;  //TP에의한 하단응력도
    double dFTLP;	//TP에의한 상단좌단 합성응력도	[Unit=Stress]
    double dFBLP;	//TP에의한 하단좌단 합성응력도	[Unit=Stress]
    double dFTRP;	//TP에의한 상단우단 합성응력도	[Unit=Stress]
    double dFBRP;	//TP에의한 하단우단 합성응력도	[Unit=Stress]

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        dFT = dFB = 0.0;
        dFTL = dFBL = dFTR = dFBR = 0.0;
        dFTP = dFBP = 0.0;
        dFTLP = dFBLP = dFTRP = dFBRP = 0.0;
    }
};

struct T_FYLC_D	// ElemK.
{
    //0:전사하중시 1:설계시 2:온도시 3:지진시
    // 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    T_FYLC_BASE FylcBase[4][4][2];	

    void Initialize()
    {
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++) 
            {
                for(int k=0; k<2; k++)FylcBase[i][j][k].Initialize();
            }
        }
    }

};

struct T_FYLC_UNIT
{
    T_FYLC_UNIT()
    {
        dFT   = D_UNITSYS_BASE_STRESS;
        dFB   = D_UNITSYS_BASE_STRESS;
        dFTL  = D_UNITSYS_BASE_STRESS;
        dFBL  = D_UNITSYS_BASE_STRESS;
        dFTR  = D_UNITSYS_BASE_STRESS;
        dFBR  = D_UNITSYS_BASE_STRESS;
        dFTP   = D_UNITSYS_BASE_STRESS;
        dFBP   = D_UNITSYS_BASE_STRESS;
        dFTLP  = D_UNITSYS_BASE_STRESS;
        dFBLP  = D_UNITSYS_BASE_STRESS;
        dFTRP  = D_UNITSYS_BASE_STRESS;
        dFBRP  = D_UNITSYS_BASE_STRESS;
    }
    int dFT;
    int dFB;
    int dFTL;
    int dFBL;
    int dFTR;
    int dFBR;
    int dFTP;
    int dFBP;
    int dFTLP;
    int dFBLP;
    int dFTRP;
    int dFBRP;

};

// 17. 스테이지별 가설중 휨 응력도 (FY_SGT)
struct T_FYST_BASE
{
	BOOL bCHK;		//설계결과유무
    double dFT;		//상단응력도			[Unit=Stress]
    double dFB;		//하단응력도			[Unit=Stress]
    double dFTL;	//상단좌단 합성응력도	[Unit=Stress]
    double dFBL;	//하단좌단 합성응력도	[Unit=Stress]
    double dFTR;	//상단우단 합성응력도	[Unit=Stress]
    double dFBR;	//하단우단 합성응력도	[Unit=Stress]

    void Initialize()
    {
	  bCHK = FALSE;
        dFT = dFB = 0.0;
        dFTL = dFBL = dFTR = dFBR = 0.0;
    }
};

struct T_FYST_D	// ElemK.
{
    //StagK
    CMap<UINT,UINT,T_FYST_BASE,T_FYST_BASE&> FystBaseMap[2]; // 0=I, 1=J

    void Initialize()
    {
        for(int i=0; i<2; i++) FystBaseMap[i].RemoveAll();
    }

};

// 18. 스테이지별 가설중 경사 인장 응력도 (FST_SGT)
struct T_FSST_BASE
{
    BOOL bCHK;		//설계결과유무 
    double dFS1;	//2-2위치 전단응력도		[Unit=Stress]
    double dFSG	;	//G-G위치 전단응력도		[Unit=Stress]
    double dFS3	;	//3-3위치 전단응력도		[Unit=Stress]
    double dFST1;	//2-2위치 전단+비틀림 응력도	[Unit=Stress]
    double dFSTG;	//G-G위치 전단+비틀림 응력도	[Unit=Stress]
    double dFST3;	//3-3위치 전단+비틀림 응력도	[Unit=Stress]
    void Initialize()
    {
	  bCHK = FALSE;
        dFS1 = dFSG = dFS3 = 0.0;
        dFST1 = dFSTG = dFST3 = 0.0;
    }
};

struct T_FSST_D	// ElemK.
{
    //StagK
    CMap<UINT,UINT,T_FSST_BASE,T_FSST_BASE&> FsstBaseMap[2]; // 0=I, 1=J
    void Initialize()
    {
        for(int i=0; i<2; i++) FsstBaseMap[i].RemoveAll();
    }
};

// 19. 스테이지별 가설중 경사 인장 응력도 중간값 (FST_SMT) //////////////////////////////////////
struct T_FSSM_BASE
{
    //응력검토위치 (0=2-2, 1=G-G, 2=3-3)
    BOOL bCHK;		//설계결과유무 
    double dFX[3];		//Local-x 방향 응력의 합		[Unit=Stress]
    double dFZ[3];		//Local-z 방향 응력의 합		[Unit=Stress]
    double dTS[3];		//전단력에 의한 전단응력도	[Unit=Stress]
    double dTT[3];		//비틀림에 의한 전단응력도	[Unit=Stress]
    double dTP[3];		//전단강봉에 의한 전단응력도	[Unit=Stress]
    void Initialize()
    {
	  bCHK = FALSE;
        for(int i=0; i<3; i++)
        {
            dFX[i] = dFZ[i] = 0.0;
            dTS[i] = dTT[i] = dTP[i] = 0.0;
        }
    }
};

struct T_FSSM_D	// ElemK.
{
    //StagK
    CMap<UINT,UINT,T_FSSM_BASE,T_FSSM_BASE&> FssmBaseMap[2]; // 0=i,1=j
    void Initialize()
    {
        for(int i=0; i<2; i++) FssmBaseMap[i].RemoveAll();
    }
};

// 20. 하중조합별 설계하중시 경사 인장 응력도 계산을 위한 중간값 (전단력최대시) (FST_MST) ///////
struct T_FTMS_BASE
{
    //응력검토위치 (0=2-2, 1=G-G, 2=3-3)
    BOOL bCHK;		//설계결과유무 
    UINT LcomK;		//하중조합번호			[Unit=None]
    int iMax;	    //하중조합종류 (타입MIN-MAX타입이 아님=0, FX-MAX=1~MZ-MIN=12) [Unit=None]
    double dFX[3];		//Local-x 방향 응력의 합		[Unit=Stress]
    double dFZ[3];		//Local-z 방향 응력의 합		[Unit=Stress]
    double dTS[3];		//전단력에 의한 전단응력도	[Unit=Stress]
    double dTT[3];		//비틀림에 의한 전단응력도	[Unit=Stress]
    double dTP[3];		//전단강봉에 의한 전단응력도	[Unit=Stress]
    // Add by Jaeoh. ('2005.10.31).
    // Inclined Stress.
    double dSN[3];    // Not consider Torsion.
    double dST[3];    // Consider Torsion.  
    // Add by Jaeoh. ('2005.11.07).
    double dFxx,dFyy,dFzz;
    double dMux,dMuy,dMuz;

    void Initialize()
    {
	  bCHK = FALSE;
        LcomK = 0; iMax = 0;
        for(int i=0; i<3; i++)
        {
            dFX[i] = dFZ[i] = 0.0;
            dTS[i] = dTT[i] = dTP[i] = 0.0;
            dSN[i] = dST[i] = 0.0;
        }
        dFxx = dFyy = dFzz = 0.0;
        dMux = dMuy = dMuz = 0.0;
    }
};

struct T_FTMS_D	// ElemK.
{
    // 1st : 0=전사하중시, I-설계시, 2=온도시, 3=지진시.
    // 2nd : 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    T_FTMS_BASE FtmsBase[4][4];

    void Initialize()
    {
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++) FtmsBase[i][j].Initialize();
        }
    }

};

// 21. 하중조합별 설계하중시 경사 인장 응력도 계산을 위한 중간값 (비틀림최대시) (FST_MTT) ///////
struct T_FTMT_BASE
{
    // 응력검토위치 (0=2-2, 1=G-G, 2=3-3)
    BOOL bCHK;		//설계결과유무 
    UINT LcomK;		//하중조합번호			[Unit=None]
    int iMax;	    //하중조합종류 (타입MIN-MAX타입이 아님=0, FX-MAX=1~MZ-MIN=12) [Unit=None]
    double dFX[3];		//Local-x 방향 응력의 합		[Unit=Stress]
    double dFZ[3];		//Local-z 방향 응력의 합		[Unit=Stress]
    double dTS[3];		//전단력에 의한 전단응력도	[Unit=Stress]
    double dTT[3];		//비틀림에 의한 전단응력도	[Unit=Stress]
    double dTP[3];		//전단강봉에 의한 전단응력도	[Unit=Stress]
    // Add by Jaeoh. ('2005.10.31).
    // Inclined Stress.
    double dSN[3];    // Not consider Torsion.
    double dST[3];    // Consider Torsion.
    // Add by Jaeoh. ('2005.11.07).
    double dFxx,dFyy,dFzz;
    double dMux,dMuy,dMuz;

    void Initialize()
    {
	  bCHK = FALSE;
        LcomK = 0; iMax = 0;
        for(int i=0; i<3; i++)
        {
            dFX[i] = dFZ[i] = 0.0;
            dTS[i] = dTT[i] = dTP[i] = 0.0;
            dSN[i] = dST[i] = 0.0;
        }
        dFxx = dFyy = dFzz = 0.0;
        dMux = dMuy = dMuz = 0.0;
    }
};

struct T_FTMT_D	// ElemK.
{
    // 1st : 0=전사하중시, I-설계시, 2=온도시, 3=지진시.
    // 2nd : 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    T_FTMT_BASE FtmtBase[4][4];
    void Initialize()
    {
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++) FtmtBase[i][j].Initialize();
        }
    }
};

// 22. 설계하중시 최대/최소에 대한 하중조합번호 (LCB_ALW) ///////
struct T_LCBA_BASE
{
	// 0=전사하중시, 1=설계시, 2=온도시, 3=지진시
	BOOL bCHK;	//설계결과유무
	UINT MmaxLcomK[4][2];	//최대 합성응력도 하중조합번호		[Unit=None]
	UINT MminLcomK[4][2];	//최소 합성응력도 하중조합번호		[Unit=None]
	UINT QmaxLcomK[4];		//최대 전단응력도 하중조합번호		[Unit=None]
	UINT QminLcomK[4];		//최소 전단응력도 하중조합번호		[Unit=None]
	UINT TmaxLcomK[4];		//최대 비틀림응력도 하중조합번호	[Unit=None]
	UINT TminLcomK[4];		//최소 비틀림응력도 하중조합번호	[Unit=None]
    void Initialize()
    {
	  bCHK = FALSE;
        for(int i=0; i<4; i++)
        {
            MmaxLcomK[i][0] = MminLcomK[i][0] = 0;
            MmaxLcomK[i][1] = MminLcomK[i][1] = 0;
            QmaxLcomK[i] = QminLcomK[i] = 0;
            TmaxLcomK[i] = TminLcomK[i] = 0;
        }
    }
};

struct T_LCBA_D	// ElemK.
{
	// 0=I, 1=J.
	T_LCBA_BASE LcbaBase[2];
    void Initialize()
    {
		for(int i=0; i<2; i++)	LcbaBase[i].Initialize();
    }
};

// 23. 종국시 최대/최소에 대한 하중조합번호 (LCB_ULT) ///////
struct T_LCBU_BASE
{
	// 0=종국시A, 1=종국시B, 2=종국시C
	BOOL bCHK;	//설계결과유무
	UINT MmaxLcomK[3];	//최대 휨모멘트 하중조합번호		[Unit=None]
	UINT MminLcomK[3];	//최소 휨모멘트 하중조합번호		[Unit=None]
	UINT QmaxLcomK[3];	//최대 전단력 하중조합번호		[Unit=None]
	UINT QminLcomK[3];	//최소 전단력 하중조합번호		[Unit=None]
	UINT TmaxLcomK[3];	//최대 비틀림 하중조합번호		[Unit=None]
	UINT TminLcomK[3];	//최소 비틀림 하중조합번호		[Unit=None]
    void Initialize()
    {
	  bCHK = FALSE;
        for(int i=0; i<3; i++)
        {
            MmaxLcomK[i] = MminLcomK[i] = 0;
            QmaxLcomK[i] = QminLcomK[i] = 0;
            TmaxLcomK[i] = TminLcomK[i] = 0;
        }
    }
};

struct T_LCBU_D	// ElemK.
{
	// 0=I, 1=J.
	T_LCBU_BASE LcbuBase[2];
    void Initialize()
    {
		for(int i=0; i<2; i++)	LcbuBase[i].Initialize();
    }
};

// '05.02.25 Add by Jaeoh
// 24. 설계하중시 평균전단응력도 (전단력 최대시) (BCK_ASS) ////////////////////////////////////
#define T_FASS_K unsigned int
struct T_FASS_CMPW_BASE
{
    BOOL bCHK;		  //설계결과유무 
    UINT LcomK;		  //하중조합번호			[Unit=None]
    int iKind;		  //하중조합종류 (1:전사하중시, 2:설계시, 3:온도시, 4:지진시)	[Unit=None]
    int iMax;	     	//하중형식 (타입MIN-MAX타입이 아님=0, FX-MAX=1~MZ-MIN=12)	[Unit=None]
    double dRTM;	  //파형강판의 평균전단응력도	[Unit=Stress]
    double dARTM;	  //파형강판의 허용전단응력도	[Unit=Stress]
    BOOL bOK;

    void Initialize()
    {
        bCHK  = 0;
        LcomK = 0;
        iMax  = 0;
        iKind = 0;
        dRTM  = 0.0;
        dARTM = 0.0;
        bOK = TRUE;
    }
};

struct T_FASS_CMPW	// ElemK.
{
    T_FASS_CMPW_BASE FassBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    
    void Initialize()
    {
        for(int i=0; i<4; i++)
            FassBase[i].Initialize();
    }
};

struct T_FASS_CMPW_UNIT
{
    T_FASS_CMPW_UNIT()
    {
        dRTM  = D_UNITSYS_BASE_STRESS;
        dARTM = D_UNITSYS_BASE_STRESS;
    }
    int dRTM ;
    int dARTM;
};

// 25. 종국하중시 최대전단응력도 (전단력 최대시) (BCK_MSS) ////////////////////////////////////
#define T_FMSS_K unsigned int
struct T_FMSS_CMPW_BASE
{
    BOOL bCHK;		  //설계결과유무 
    UINT LcomK;		  //하중조합번호			[Unit=None]
    int  iKind;		  //하중조합종류 (11:종국시A, 12:종국시B, 13:종국시C)		[Unit=None]
    int  iMax;	      //하중형식 (타입MIN-MAX타입이 아님=0, FX-MAX=1~MZ-MIN=12)	[Unit=None]
    double dRTM;	  //파형강판의 최대전단응력도	[Unit=Stress]
    double dARTM;	  //파형강판의 허용전단응력도	[Unit=Stress]
    BOOL bOK;

    void Initialize()
    {
        bCHK  = 0;
        LcomK = 0;
        iKind = 0;
        iMax  = 0;
        dRTM  = 0.0;
        dARTM = 0.0;
        bOK = TRUE;
    }
};

struct T_FMSS_CMPW	// ElemK.
{
    T_FMSS_CMPW_BASE FmssBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            FmssBase[i].Initialize();
    }
};

struct T_FMSS_CMPW_UNIT
{
    T_FMSS_CMPW_UNIT()
    {
        dRTM  = D_UNITSYS_BASE_STRESS;
        dARTM = D_UNITSYS_BASE_STRESS;
    }
    int dRTM ;
    int dARTM;
};

// 26. 파형강판 국부좌굴 검토 (BCK_PRT) ////////////////////////////////////
#define T_FPRT_K unsigned int
struct T_FPRT_CMPW_BASE
{
    BOOL bCHK;		    //설계결과유무 
//  UINT LcomK;		    //하중조합번호			[Unit=None]
//  int iKind;		    //하중조합종류 (11:종국시A, 12:종국시B, 13:종국시C)		[Unit=None]
//  int iMax;	      	//하중형식 (타입MIN-MAX타입이 아님=0, FX-MAX=1~MZ-MIN=12)	[Unit=None]
    double dRTL;		  //파형강판의 좌굴강도		    [Unit=Stress]
    double dRTY;		  //파형강판의 전단항복응력도	[Unit=Stress]
    double dRMDS;	    //Lambda_s (전단좌굴계수)	  [Unit=None]
    double dRAMDS;	  //RMDS의 허용값			        [Unit=None]
    BOOL bOK;

    void Initialize()
    {
        bCHK   = 0;
        dRTL	 = 0.0;
        dRTY	 = 0.0;
        dRMDS  = 0.0;
        dRAMDS = 0.0;   
        bOK = TRUE;
    }
};

struct T_FPRT_CMPW	// ElemK.
{
    T_FPRT_CMPW_BASE FprtBase[2];	// 0=I, 1=J.
    
    void Initialize()
    {
        for(int i=0; i<2; i++)
            FprtBase[i].Initialize();
    }
};

struct T_FPRT_CMPW_UNIT
{
    T_FPRT_CMPW_UNIT()
    {
        dRTL = D_UNITSYS_BASE_STRESS;
        dRTY = D_UNITSYS_BASE_STRESS;
    }
    int dRTL;
    int dRTY;
};

// 27. 파형강판 전체좌굴 검토 (BCK_WHL) ////////////////////////////////////
#define T_FWHL_K unsigned int
struct T_FWHL_CMPW_BASE
{
    BOOL bCHK;		    //설계결과유무 
//  UINT LcomK;		    //하중조합번호			[Unit=None]
//  int iKind;		    //하중조합종류 (11:종국시A, 12:종국시B, 13:종국시C)		[Unit=None]
//  int iMax;	      	//하중형식 (타입MIN-MAX타입이 아님=0, FX-MAX=1~MZ-MIN=12)	[Unit=None]
    double dRTL;		    //파형강판의 좌굴강도		    [Unit=Stress]
    double dRTY;		    //파형강판의 전단항복응력도	[Unit=Stress]
    double dRMDS;	    //Lambda_s (전단좌굴계수)	    [Unit=None]
    double dRAMDS;	    //RMDS의 허용값			        [Unit=None]
    BOOL bOK;

    void Initialize()
    {
        bCHK   = 0;
        dRTL	 = 0.0;
        dRTY	 = 0.0;
        dRMDS  = 0.0;
        dRAMDS = 0.0;   
        bOK = TRUE;
    }
};

struct T_FWHL_CMPW	// ElemK.
{
    T_FWHL_CMPW_BASE FwhlBase[2];	// 0=I, 1=J.

    void Initialize()
    {
        for(int i=0; i<2; i++)
            FwhlBase[i].Initialize();
    }
};

struct T_FWHL_CMPW_UNIT
{
    T_FWHL_CMPW_UNIT()
    {
        dRTL = D_UNITSYS_BASE_STRESS;
        dRTY = D_UNITSYS_BASE_STRESS;
    }
    int dRTL;
    int dRTY;
};

struct T_COMF_KEY
{
    UINT ElemK;
    UINT DgnLcomK;
    int  nPos;  // 0:I, 1:J
    operator unsigned long() {return (DgnLcomK<<24) + (ElemK<<1) + nPos; }

    BOOL operator==(const T_COMF_KEY& rKey) const
    {
        if(ElemK == rKey.ElemK && DgnLcomK == rKey.DgnLcomK && nPos == rKey.nPos) 
            return TRUE;
        else 
            return FALSE;
    }
};

#define T_COMF_K unsigned int

struct T_COMF_BASE
{
    UINT OrgLcomK;
    int iKind;
    int iMax;
    double dFxx,dFyy,dFzz;
    double dMux,dMuy,dMuz;

    void Initialize()
    {
        OrgLcomK = 0;
        iKind = 0;
        iMax = 0;
        dFxx = dFyy = dFzz = 0.0;
        dMux = dMuy = dMuz = 0.0;
    }
};

struct T_COMF_D
{
	// 0=I, 1=J.
    CArray<T_COMF_BASE,T_COMF_BASE> ComfBase[2];	// 0=I, 1=J.

    void Initialize()
    {
		for(int i=0; i<2; i++)	ComfBase[i].RemoveAll();
    }
};

struct T_COMF_UNIT
{
    T_COMF_UNIT()
    {
        dFxx = D_UNITSYS_BASE_FORCE;
        dFyy = D_UNITSYS_BASE_FORCE;
        dFzz = D_UNITSYS_BASE_FORCE;
        dMux = D_UNITSYS_BASE_MOMENT;
        dMuy = D_UNITSYS_BASE_MOMENT;
        dMuz = D_UNITSYS_BASE_MOMENT;
    }
    int dFxx;
    int dFyy;
    int dFzz;
    int dMux;
    int dMuy;
    int dMuz;
};

//////////////////////////KSCE//////////////////////////////////////
struct T_FYPC_KSCE	// TdnaK.
{
    BOOL bCHK;		  //설계결과유무
    int nLoadType;  // 0=Pre-tension, 1=Post-tension 
    double dFDL1;	  //가설중 응력도			[Unit=Stress]
                                    //포스트텐션(iLoadType)인 경우=정착구에서의 응력
    double dFDL2;	  //가설중 응력도			[Unit=Stress]
			            //포스트텐션(iLoadType)인 경우=청착장치의 활동에 의한 손실의 끝 부분에서의 응력
			            //프리텐션인(iLoadType) 경우=사용안함.
    double dFLL;	  //사용하중시 응력도		[Unit=Stress]
    double dAFDL1;	//가설중 허용응력도		[Unit=Stress]
    double dAFDL2;	//가설중 허용응력도		[Unit=Stress]
    double dAFLL; 	//사용하중시 허용응력도		[Unit=Stress]
    int nElemFDL1;
    int nElemFDL2;
    int nElemFLL1;
    int nPartFDL1;
    int nPartFDL2;
    int nPartFLL1;
    int nStage;
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        nLoadType = 0;
        dFDL1 = dFDL2 = dFLL = 0.0;
        dAFDL1 = dAFDL2 = 0.0;
        dAFLL = 0.0;
        nElemFDL1 = nElemFDL2 = nElemFLL1 = 0;
        nPartFDL1 = nPartFDL2 = nPartFLL1 = 0;
        nStage    = 0;
        bOK = TRUE;
    }

};

struct T_FYPC_KSCE_UNIT
{
    T_FYPC_KSCE_UNIT()
    {
        dFDL1  = D_UNITSYS_BASE_STRESS;
        dFDL2  = D_UNITSYS_BASE_STRESS;
        dFLL   = D_UNITSYS_BASE_STRESS;
        dAFDL1 = D_UNITSYS_BASE_STRESS;
        dAFDL2 = D_UNITSYS_BASE_STRESS;
        dAFLL  = D_UNITSYS_BASE_STRESS;
 }
    int dFDL1;
    int dFDL2;
    int dFLL;
    int dAFDL1;
    int dAFDL2;
    int dAFLL;
};

#define T_BDCY_K unsigned int
struct T_BDCY_KSCE_BASE
{
    BOOL bCHK;		    //설계결과유무
    UINT LcomK;		    //하중조합번호		[Unit=None]
    int iMax;	        // 	하중형식 (타입MIN-MAX타입이 아님=0, FX-MAX=1~MZ-MIN=12) 	[Unit=None]
    double dMuy;	    //면내 계수모멘트		[Unit=Moment]
    double dfpe;	    //손실이 일어난 후 강재의 유효프리스트레스	[Unit=Stress]
    double dAps;	    //인장측 PS강재의 단면적	[Unit=Area]
    double dAst;	    //인장측 철근의 단면적		[Unit=Area]
    double dpst;	    //인장측 철근비			[Unit=None]
    double dIy;		    //강축 단면 2차모멘트 		[Unit=Length^4]
    double db;		    //단면폭		[Unit=Length]
    double dh;		    //단면춤		[Unit=Length]
    double ddst;	    //콘크리트압축 연단에서 인장측 철근 중심까지의 거리		[Unit=Length]
    double ddps;	    //인장측 PS강재의 유효춤	[Unit=Length]
    double dfps;	    //계수하중 하에서 PS강재 평균응력	[Unit=Stress]
    double da;		    //등가 직사각형 응력블럭의 깊이	[Unit=Length]
    double dpMny;	    //면내 설계휨모멘트	[Unit=Moment]
    double dRatM;	    //면내 휨강도 안전율	[Unit=None]
    double dqp;		    //강재지수			[Unit=None]
    double d036Beta1; //최대 PS강재량 제한값	[Unit=None]
    double d12Mcr;	  //최소 강재 균열모멘트	[Unit=Moment]
    double d43Muy;	    //도.설 2005 (4/3)Mu	[Unit=Moment]
    // 계산시 사용되는 중간값
    double dPe;		    //유효긴장력			[Unit=Force]
    double dAsc;	    //압축측 철근의 단면적	[Unit=Area]
    double ddsc;	    //콘크리트압축 연단에서 압축측 철근 중심까지의 거리		[Unit=Length]
    double dpsc;	    //압축측 철근비		[Unit=None]
    double dyt;		    //총단면적의 중립축에서 인장 연단까지의 거리			[Unit=Length]
    double deps;	    //PS강재의 도심의 편심량 	[Unit=Length]
	double dAc;		    //콘크리트의 전면적     [Unit=Area]
	double dfy;       //  인장철근의 설계 항복강도 	[Unit=Stress]
	double dfck;	    //콘크리트 설계기준강도 		[Unit=Stress]
	double dbeta1;    //0.85fck의 압축응력이 균등하게 분포하는 직사각형 블록의 깊이 a의 비
	double drp; 	    //PS강재의 종류에 따른 계수	[Unit=None]
	double dphi; 	    //강도감도계수			[Unit=None]
    BOOL bOK;
    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        dMuy = 0.0;
        dfpe = 0.0;
        dAps = 0.0;
        dAst = 0.0;
        dpst = 0.0;
        dIy = 0.0;
		db = 0.0;
		dh = 0.0;
        ddst = 0.0;
        ddps = 0.0;
        dfps = 0.0;
        da = 0.0;	
        dpMny = 0.0;
        dRatM = 0.0;
        dqp = 0.0;	
        d036Beta1 = 0.0;
        d12Mcr = 0.0;
        d43Muy = 0.0;
        dPe = 0.0;		  
        dAsc = 0.0;	    
        ddsc = 0.0;	    
        dpsc = 0.0;	    
        dyt = 0.0;		  
        deps = 0.0;	    
	  dAc = 0.0;		  
	  dfy = 0.0;      
	  dfck = 0.0;	    
	  dbeta1 = 0.0;   
	  drp = 0.0; 	    
	  dphi = 0.0; 	  
        bOK = TRUE;
    }

};


struct T_BDCY_KSCE	// ElemK.
{
    T_BDCY_KSCE_BASE BdcyBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
		for(int i=0; i<4; i++)	BdcyBase[i].Initialize();
    }

};

struct T_BDCY_KSCE_UNIT
{
    T_BDCY_KSCE_UNIT()
    {
        dMuy = D_UNITSYS_BASE_MOMENT;	    //면내 계수모멘트		[Unit=Moment]
        dfpe = D_UNITSYS_BASE_STRESS;	    //손실이 일어난 후 강재의 유효프리스트레스	[Unit=Stress]
        dAps = D_UNITSYS_BASE_AREA;	    //인장측 PS강재의 단면적	[Unit=Area]
        dAst = D_UNITSYS_BASE_AREA;	    //인장측 철근의 단면적		[Unit=Area]
        dIy = D_UNITSYS_BASE_STIF;		    //강축 단면 2차모멘트 		[Unit=Length^4]
		db	= D_UNITSYS_BASE_LENGTH;	    //단면폭		[Unit=Length]
		dh	= D_UNITSYS_BASE_LENGTH;	    //단면춤		[Unit=Length]
        ddst = D_UNITSYS_BASE_LENGTH;	    //콘크리트압축 연단에서 인장측 철근 중심까지의 거리		[Unit=Length]
        ddps = D_UNITSYS_BASE_LENGTH;	    //인장측 PS강재의 유효춤	[Unit=Length]
        dfps = D_UNITSYS_BASE_STRESS;	    //계수하중 하에서 PS강재 평균응력	[Unit=Stress]
        da = D_UNITSYS_BASE_LENGTH;		    //등가 직사각형 응력블럭의 깊이	[Unit=Length]
        dpMny = D_UNITSYS_BASE_MOMENT;	    //면내 설계휨모멘트	[Unit=Moment]
        d12Mcr = D_UNITSYS_BASE_MOMENT;	  //최소 강재 균열모멘트	[Unit=Moment]
        d43Muy  = D_UNITSYS_BASE_MOMENT;	  // [Unit=Moment]
        dPe = D_UNITSYS_BASE_FORCE;		    //유효긴장력			[Unit=Force]
        dAsc = D_UNITSYS_BASE_AREA;	    //압축측 철근의 단면적	[Unit=Area]
        ddsc = D_UNITSYS_BASE_LENGTH;	    //콘크리트압축 연단에서 압축측 철근 중심까지의 거리		[Unit=Length]
        dyt = D_UNITSYS_BASE_LENGTH;		    //총단면적의 중립축에서 인장 연단까지의 거리			[Unit=Length]
        deps = D_UNITSYS_BASE_LENGTH;	    //PS강재의 도심의 편심량 	[Unit=Length]
	  dAc = D_UNITSYS_BASE_AREA;		    //콘크리트의 전면적     [Unit=Area]
	  dfy = D_UNITSYS_BASE_STRESS;       //  인장철근의 설계 항복강도 	[Unit=Stress]
	  dfck = D_UNITSYS_BASE_STRESS;	    //콘크리트 설계기준강도 		[Unit=Stress]
    }
    int dMuy;
    int dfpe;
    int dAps;
    int dAst;
    int dIy;
	int db;
	int dh;
    int ddst;
    int ddps;
    int dfps;
    int da;
    int dpMny;
    int d12Mcr;
    int d43Muy;
    int dPe;
    int dAsc;
    int ddsc;
    int dyt;
    int deps;
	int dAc;
	int dfy;
	int dfck;
};

#define T_BDCZ_K unsigned int
struct T_BDCZ_KSCE_BASE
{
    BOOL bCHK;		    //설계결과유무
    UINT LcomK;		    //하중조합번호		[Unit=None]
    int iMax;	        // 	하중형식 (타입MIN-MAX타입이 아님=0, FX-MAX=1~MZ-MIN=12) 	[Unit=None]
    double dMuy;	    //면내 계수모멘트		[Unit=Moment]
    double dfpe;	    //손실이 일어난 후 강재의 유효프리스트레스	[Unit=Stress]
    double dAps;	    //인장측 PS강재의 단면적	[Unit=Area]
    double dAst;	    //인장측 철근의 단면적		[Unit=Area]
    double dpst;	    //인장측 철근비			[Unit=None]
    double dIy;		    //강축 단면 2차모멘트 		[Unit=Length^4]
    double ddst;	    //콘크리트압축 연단에서 인장측 철근 중심까지의 거리		[Unit=Length]
    double ddps;	    //인장측 PS강재의 유효춤	[Unit=Length]
    double dfps;	    //계수하중 하에서 PS강재 평균응력	[Unit=Stress]
    double da;		    //등가 직사각형 응력블럭의 깊이	[Unit=Length]
    double dpMny;	    //면내 설계휨모멘트	[Unit=Moment]
    double dRatM;	    //면내 휨강도 안전율	[Unit=None]
    double dqp;		    //강재지수			[Unit=None]
    double d036Beta1; //최대 PS강재량 제한값	[Unit=None]
    double d12Mcr;	  //최소 강재 균열모멘트	[Unit=Moment]
    // 계산시 사용되는 중간값
    double dPe;		    //유효긴장력			[Unit=Force]
    double dAsc;	    //압축측 철근의 단면적	[Unit=Area]
    double ddsc;	    //콘크리트압축 연단에서 압축측 철근 중심까지의 거리		[Unit=Length]
    double dpsc;	    //압축측 철근비		[Unit=None]
    double dyt;		    //총단면적의 중립축에서 인장 연단까지의 거리			[Unit=Length]
    double deps;	    //PS강재의 도심의 편심량 	[Unit=Length]
	double dAc;		    //콘크리트의 전면적     [Unit=Area]
	double dfy;       //  인장철근의 설계 항복강도 	[Unit=Stress]
	double dfck;	    //콘크리트 설계기준강도 		[Unit=Stress]
	double dbeta1;    //0.85fck의 압축응력이 균등하게 분포하는 직사각형 블록의 깊이 a의 비
	double drp; 	    //PS강재의 종류에 따른 계수	[Unit=None]
	double dphi; 	    //강도감도계수			[Unit=None]
    BOOL bOK;
    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        dMuy = 0.0;
        dfpe = 0.0;
        dAps = 0.0;
        dAst = 0.0;
        dpst = 0.0;
        dIy = 0.0;
        ddst = 0.0;
        ddps = 0.0;
        dfps = 0.0;
        da = 0.0;	
        dpMny = 0.0;
        dRatM = 0.0;
        dqp = 0.0;	
        d036Beta1 = 0.0;
        d12Mcr = 0.0;	  
        dPe = 0.0;		  
        dAsc = 0.0;	    
        ddsc = 0.0;	    
        dpsc = 0.0;	    
        dyt = 0.0;		  
        deps = 0.0;	    
	  dAc = 0.0;		  
	  dfy = 0.0;      
	  dfck = 0.0;	    
	  dbeta1 = 0.0;   
	  drp = 0.0; 	    
	  dphi = 0.0; 	  
        bOK = TRUE;
    }

};


struct T_BDCZ_KSCE	// ElemK.
{
    T_BDCZ_KSCE_BASE BdczBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
		for(int i=0; i<4; i++)	BdczBase[i].Initialize();
    }

};

struct T_BDCZ_KSCE_UNIT
{
    T_BDCZ_KSCE_UNIT()
    {
        dMuy = D_UNITSYS_BASE_MOMENT;	    //면내 계수모멘트		[Unit=Moment]
        dfpe = D_UNITSYS_BASE_STRESS;	    //손실이 일어난 후 강재의 유효프리스트레스	[Unit=Stress]
        dAps = D_UNITSYS_BASE_AREA;	    //인장측 PS강재의 단면적	[Unit=Area]
        dAst = D_UNITSYS_BASE_AREA;	    //인장측 철근의 단면적		[Unit=Area]
        dIy = D_UNITSYS_BASE_STIF;		    //강축 단면 2차모멘트 		[Unit=Length^4]
        ddst = D_UNITSYS_BASE_LENGTH;	    //콘크리트압축 연단에서 인장측 철근 중심까지의 거리		[Unit=Length]
        ddps = D_UNITSYS_BASE_LENGTH;	    //인장측 PS강재의 유효춤	[Unit=Length]
        dfps = D_UNITSYS_BASE_STRESS;	    //계수하중 하에서 PS강재 평균응력	[Unit=Stress]
        da = D_UNITSYS_BASE_LENGTH;		    //등가 직사각형 응력블럭의 깊이	[Unit=Length]
        dpMny = D_UNITSYS_BASE_MOMENT;	    //면내 설계휨모멘트	[Unit=Moment]
        d12Mcr = D_UNITSYS_BASE_MOMENT;	  //최소 강재 균열모멘트	[Unit=Moment]
        dPe = D_UNITSYS_BASE_FORCE;		    //유효긴장력			[Unit=Force]
        dAsc = D_UNITSYS_BASE_AREA;	    //압축측 철근의 단면적	[Unit=Area]
        ddsc = D_UNITSYS_BASE_LENGTH;	    //콘크리트압축 연단에서 압축측 철근 중심까지의 거리		[Unit=Length]
        dyt = D_UNITSYS_BASE_LENGTH;		    //총단면적의 중립축에서 인장 연단까지의 거리			[Unit=Length]
        deps = D_UNITSYS_BASE_LENGTH;	    //PS강재의 도심의 편심량 	[Unit=Length]
	  dAc = D_UNITSYS_BASE_AREA;		    //콘크리트의 전면적     [Unit=Area]
	  dfy = D_UNITSYS_BASE_STRESS;       //  인장철근의 설계 항복강도 	[Unit=Stress]
	  dfck = D_UNITSYS_BASE_STRESS;	    //콘크리트 설계기준강도 		[Unit=Stress]
    }
    int dMuy;
    int dfpe;
    int dAps;
    int dAst;
    int dIy;
    int ddst;
    int ddps;
    int dfps;
    int da;
    int dpMny;
    int d12Mcr;
    int dPe;
    int dAsc;
    int ddsc;
    int dyt;
    int deps;
	int dAc;
	int dfy;
	int dfck;
};

struct T_CUMS_KSCE_BASE
{
    BOOL bCHK;		  //설계결과유무
    UINT LcomK;		  //하중조합번호		[Unit=None]
    int iMax;	      // 	하중형식 (타입MIN-MAX타입이 아님=0, FX-MAX=1~MZ-MIN=12) 	[Unit=None]
    double dVu;	    //면내 계수전단력		[Unit=Force]
    double dVci;    //  전단과 모멘트의 조합에 의해 사인장균열이 발생할 때 콘크리트가 부담하는 공칭 전단강도 [Unit=Force]
    double dVcw;    //  복부의 과도한 주인장응력으로 인해 사인장균열이 발생할 때 콘크리트가 부담하는 공칭 전단강도			[Unit=Force]
    double dVc;	 	  //콘크리트가 부담하는 공칭 전단강도			[Unit= Force]
    double dVs;	 	  //전단철근이 부담하는 공칭 전단강도 			[Unit= Force]
    double dpVn;	  //단면의 공칭전단력		[Unit= Force]
    double dRatV;    //공칭전단력비
    double dAv;		  //간격 s 이내에 있는 전단철근의 단면적	[Unit=Area]
    double dAvmin;	//최소전단철근	[Unit= Area]
    double dAvreq;	//최소전단철근	[Unit= Area]
    // 계산시 사용되는 중간값
    double dbw;		  //복부의 폭	[Unit=Length]
    double dd;		  //콘크리트압축 연단에서 인장측 철근 중심까지의 거리		[Unit=Length]
    double dVi; 	  //dVu-dVi				[Unit=Force]
    double dVd;		  //사용 고정하중에 의해 발생하는 단면의 전단력			[Unit=Force]
    double dMmax;	  //dMu-dMd				[Unit=Moment]
    double dMu;     //전체 계수하중에 의한 계수모멘트      [Unit=Moment]
    double dMd;		  //하중계수를 고려하지 않은 고정하중 모멘트	   	     [Unit=Moment]
    double dMcr;	  //외부 작용하중에 의한 단면의 휨 균열모멘트			[Unit=Moment]
    double dIy; 	  //강축 단면 2차모멘트 		[Unit=Length^4]
    double dh;		  //단면춤		[Unit=Length]
    double dyt;		  //총단면적의 중립축에서 인장 연단까지의 거리			[Unit=Length]
    double dfpe;	  //손실이 일어난 후 강재의 유효프리스트레스			[Unit=Stress]
    double dfd;     // 외부 작용하중에 의해 인장응력이 발생하는 단면의 최외연에서의 사용 고정하중에의한 응력		 [Unit=Stress]
	double dfpc;	  //유효프리스트레스 힘으로 인한 콘크리트 도심에서의 압축응력	 [Unit=Stress]
	double dVp;     //단면의 유효프리스트레스 힘의 수직 분력				[Unit=Force]
	double dfck;	  //콘크리트 설계기준강도 			[Unit=Stress]
	double dsv; 	  //경사인장철근의 간격			[Unit=Length]
	double dThetaV; // 	경사인장철근의 각도			[Unit=Rad]
	double dsp; 	  //전단강봉의 간격				[Unit=Length]
	double dThetap; //전단강봉의 각도			[Unit=Rad]
	double dAp; 	  //전단강봉의 배치량   [Unit= Area]

    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;

        dVu = 0.0;
        dVci = 0.0;
        dVcw = 0.0;
        dVc = 0.0;
        dVs = 0.0;
        dpVn = 0.0;
        dRatV = 0.0;
        dAv = 0.0;
        dAvmin = 0.0;
        dAvreq = 0.0;
        dbw = 0.0;
        dd = 0.0;
        dVi = 0.0;
        dVd = 0.0;
        dMmax = 0.0;
        dMu = 0.0;
        dMd = 0.0;
        dMcr = 0.0;
        dIy = 0.0;
		dh = 0.0;
        dyt = 0.0;
        dfpe = 0.0;
        dfd = 0.0;
	  dfpc = 0.0;
	  dVp = 0.0;
	  dfck = 0.0;
	  dsv = 0.0;
	  dThetaV = 0.0;
	  dsp = 0.0;
	  dThetap = 0.0;
	  dAp = 0.0;

        bOK = TRUE;
    }

};

struct T_CUMS_KSCE	// ElemK.
{
    T_CUMS_KSCE_BASE CumsBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    void Initialize()
    {
		for(int i=0; i<4; i++)	CumsBase[i].Initialize();
    }

};

struct T_CUMS_KSCE_UNIT
{
    T_CUMS_KSCE_UNIT()
    {
        dVu = D_UNITSYS_BASE_FORCE;	    //[Unit=Force]
        dVci = D_UNITSYS_BASE_FORCE;    //[Unit=Force]
        dVcw = D_UNITSYS_BASE_FORCE;    //[Unit=Force]
        dVc = D_UNITSYS_BASE_FORCE;	 	  //[Unit= Force]
        dVs = D_UNITSYS_BASE_FORCE;	 	  //[Unit= Force]
        dpVn = D_UNITSYS_BASE_FORCE;	  //[Unit= Force]
        dAv = D_UNITSYS_BASE_AREA;		  //[Unit=Area]
        dAvmin = D_UNITSYS_BASE_AREA;	//[Unit= Area]
        dAvreq = D_UNITSYS_BASE_AREA;	//[Unit= Area]
        dbw = D_UNITSYS_BASE_LENGTH;		  //[Unit=Length]
        dd = D_UNITSYS_BASE_LENGTH;		  //[Unit=Length]
        dVi = D_UNITSYS_BASE_FORCE; 	  //[Unit=Force]
        dVd = D_UNITSYS_BASE_FORCE;		  //[Unit=Force]
        dMmax = D_UNITSYS_BASE_MOMENT;	  //[Unit= Moment]
        dMu = D_UNITSYS_BASE_MOMENT;		  //[Unit=Moment]
        dMd = D_UNITSYS_BASE_MOMENT;		  //[Unit=Moment]
        dMcr = D_UNITSYS_BASE_MOMENT;	  //[Unit=Moment]
        dIy = D_UNITSYS_BASE_STIF; 	  //[Unit=Length^4]
		dh	= D_UNITSYS_BASE_LENGTH;	    //[Unit=Length]
        dyt = D_UNITSYS_BASE_LENGTH;		  //[Unit=Length]
        dfpe = D_UNITSYS_BASE_STRESS;	  //[Unit=Stress]
        dfd = D_UNITSYS_BASE_STRESS;     //[Unit=Stress]
	  dfpc = D_UNITSYS_BASE_STRESS;	  //[Unit=Stress]
	  dVp = D_UNITSYS_BASE_FORCE;     //[Unit=Force]
	  dfck = D_UNITSYS_BASE_STRESS;	  //[Unit=Stress]
	  dsv = D_UNITSYS_BASE_LENGTH; 	  //[Unit=Length]
	  dThetaV = D_UNITSYS_BASE_RADIAN; //[Unit=Rad]
	  dsp = D_UNITSYS_BASE_LENGTH; 	   //[Unit=Length]
	  dThetap = D_UNITSYS_BASE_RADIAN; //[Unit=Rad]
	  dAp = D_UNITSYS_BASE_AREA; 	  //[Unit=Area]
    }
    int dVu;
    int dVci;
    int dVcw;
    int dVc;
    int dVs;
    int dpVn;
    int dAv;
    int dAvmin;
    int dAvreq;
    int dbw;
    int dd;
    int dVi;
    int dVd;
    int dMmax;
    int dMu;
    int dMd;
    int dMcr;
    int dIy;
	int dh;
    int dyt;
    int dfpe;
    int dfd;
    int dfpc;
    int dVp;
    int dfck;
    int dsv;
    int dThetaV;
    int dsp;
    int dThetap;
    int dAp;
};

struct T_CRMT_KSCE_BASE
{
    BOOL bCHK;		  //설계결과유무
    UINT LcomK;		  //하중조합번호		[Unit=None]
    int iMax;	     	//하중형식 (타입MIN-MAX타입이 아님=0, FX-MAX=1~MZ-MIN=12) 	[Unit=None]
    double dTu;		  //계수 비틀림모멘트					[Unit=Moment]
    double dTcr;    //비틀림 균열 모멘트 					[Unit=Moment]		
    double dAoh;    //비틀림 저항철근의 중심선으로 폐쇄된 면적	[Unit=Area]
    double dVc;	 	  //콘크리트가 부담하는 공칭 전단강도			[Unit= Force]
    double dvVT;	  //전단력과 비틀림에 의한 전단응력 			[Unit= Stress]
    double dvVTmax;	//전단균열을 유발하는 응력과 제한응력의 합	[Unit= Stress]
    double dpTn;	  //공칭비틀림 모멘트 					[Unit=Moment]
    double dRatT;   //비틀림강도 안전률					[Unit=None]
    double dAt;		  //간격 s 내의 비틀림에 저항하는 폐쇄스터럽의 다리 1개의 면적	[Unit=Area]
    double dAtmin;	//횡방향 폐쇄스터럽의 최소 면적			[Unit= Area]
    double dAluse;	//종방향 비틀림 철근의 총면적				[Unit=Area]
    double dAlreq;  //종방향 비틀림 철근의 총면적				[Unit=Area]
    double dAlmin;	//최소 종방향 비틀림 철근량				[Unit= Area]
    double dAcp;	  //콘크리트 단면의 바깥 둘레로 둘러싸인 단면적		[Unit= Area]
    double dPcp;	  //전단면의 둘레의 길이						[Unit=Length]
    double dfpc; 	  //유효프리스트레스 힘으로 인한 콘크리트 도심에서의 압축응력	 [Unit=Stress]
    double dfck;	  //콘크리트 설계기준강도 					[Unit=Stress]
    double dMu;     //전체 계수하중에 의한 계수모멘트      [Unit=Moment]     
    double dVu;		  //단면의 계수 전단력					[Unit= Force]
    double dPh;		  //외곽부 폐쇄 횡방향 비틀림 철근의 중심선의 둘레길이     [Unit= Length]
    double dbw;		  //복부의 폭							[Unit=Length]
    double dt; 		  //벽의 두께 				 			[Unit=Length]
    double dThetaC; //압축경사각 			 			[Unit=Rad]
    double dfyv;	  //폐쇄된 횡비틀림 철근의 항복강도	  		[Unit=Stress]
    double dfyl;    //종방향 비틀림 철근의 항복강도			[Unit=Stress]

    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;

        dTu = 0.0;		
        dTcr = 0.0;   
        dAoh = 0.0;   
        dVc = 0.0;	 	
        dvVT = 0.0;	  
        dvVTmax = 0.0;
        dpTn = 0.0;	  
        dRatT = 0.0;  
        dAt = 0.0;		
        dAtmin = 0.0;	
        dAluse = 0.0;	
        dAlreq = 0.0; 
        dAlmin = 0.0;	
        dAcp = 0.0;	  
        dPcp = 0.0;	  
        dfpc = 0.0; 	
        dfck = 0.0;	 
        dMu = 0.0;
        dVu = 0.0;		
        dPh = 0.0;		
        dbw = 0.0;		
        dt = 0.0; 		
        dThetaC = 0.0;
        dfyv = 0.0;	  
        dfyl = 0.0;   

        bOK = TRUE;
    }

};

struct T_CRMT_KSCE	// ElemK.
{
    T_CRMT_KSCE_BASE CrmtBase[6];	// 0=I-V-MAX, 1=I-V-MIN, 2=I-T-MAX, 3=J-V-MAX, 4=J-V-MIN, 5=J-T-MAX.

    void Initialize()
    {
		for(int i=0; i<6; i++)	CrmtBase[i].Initialize();
    }
};

struct T_CRMT_KSCE_UNIT
{
    T_CRMT_KSCE_UNIT()
    {
        dTu = D_UNITSYS_BASE_MOMENT;
        dTcr = D_UNITSYS_BASE_MOMENT;   
        dAoh = D_UNITSYS_BASE_AREA;     
        dVc = D_UNITSYS_BASE_FORCE;	 	  
        dvVT = D_UNITSYS_BASE_STRESS;	  
        dvVTmax = D_UNITSYS_BASE_STRESS;
        dpTn = D_UNITSYS_BASE_MOMENT;	  
        dAt = D_UNITSYS_BASE_AREA;		  
        dAtmin = D_UNITSYS_BASE_AREA;	
        dAluse = D_UNITSYS_BASE_AREA;	
        dAlreq = D_UNITSYS_BASE_AREA; 
        dAlmin = D_UNITSYS_BASE_AREA;	
        dAcp = D_UNITSYS_BASE_AREA;	  
        dPcp = D_UNITSYS_BASE_LENGTH;	  
        dfpc = D_UNITSYS_BASE_STRESS; 	
        dfck = D_UNITSYS_BASE_STRESS;	
        dMu = D_UNITSYS_BASE_MOMENT;
        dVu = D_UNITSYS_BASE_FORCE;		  
        dPh = D_UNITSYS_BASE_LENGTH;		
        dbw = D_UNITSYS_BASE_LENGTH;		
        dt = D_UNITSYS_BASE_LENGTH; 		
        dThetaC = D_UNITSYS_BASE_RADIAN;
        dfyv = D_UNITSYS_BASE_STRESS;	  
        dfyl = D_UNITSYS_BASE_STRESS;   
    }
    int dTu;		  
    int dTcr;    
    int dAoh;    
    int dVc;	 	  
    int dvVT;	  
    int dvVTmax;	
    int dpTn;	  
    int dAt;		  
    int dAtmin;	
    int dAluse;	
    int dAlreq;  
    int dAlmin;	
    int dAcp;	  
    int dPcp;	  
    int dfpc; 	  
    int dfck;	 
    int dMu;
    int dVu;		  
    int dPh;		  
    int dbw;		  
    int dt; 		  
    int dThetaC; 
    int dfyv;	  
    int dfyl;    
};

// 필요텐던량.
#define T_TNDN_K unsigned int
struct T_TNDN_KSCE_BASE
{
    BOOL bCHK;		//설계결과유무
    UINT StagK;	  //스테이지번호			[Unit=None]
    UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    UINT LcomKSer;
    int iMaxSer;     //MIN-MAX
    
    double dMd;    // 시공단계 Moment                [Unit=Moment]
    double dMa;    // 사용단계 Moment                [Unit=Moment]
    double de;     // 중립축으로부터 텐던의 중심거리 [Unit=Length]
    double dPimin; // 최소필요긴장력	  			  			[Unit=Force]
    double dApmin; // 필요텐던량		    			  			[Unit=Area]
    double dApuse; // 사용텐던량		    			  			[Unit=Area]
    BOOL bOK;
    void Initialize()
    {
        bCHK  = 0;
        StagK = 0;
        LcomK = 0;
        iMax  = 0;
        LcomKSer=0;
        iMaxSer=0;
        dMd = dMa = 0.0;
        de = dPimin = dApmin = dApuse = 0.0;
        bOK = TRUE;
    }

};

struct T_TNDN_KSCE	// ElemK.
{
    T_TNDN_KSCE_BASE TndnBase[4];	// 0=I-TOP, 1=I-BOT, 2:J-TOP, 3:J-Bot.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            TndnBase[i].Initialize();
    }
};

struct T_TNDN_KSCE_UNIT
{
    T_TNDN_KSCE_UNIT()
    {
        dMd    = D_UNITSYS_BASE_MOMENT;
        dMa    = D_UNITSYS_BASE_MOMENT;
        de     = D_UNITSYS_BASE_LENGTH;
        dPimin = D_UNITSYS_BASE_FORCE;
        dApmin = D_UNITSYS_BASE_AREA;
        dApuse = D_UNITSYS_BASE_AREA;
    }
    int dMd;  
    int dMa; 
    int de;   
    int dPimin;
    int dApmin;
    int dApuse;
};

//////////////////////////JTG D62-04//////////////////////////////////////
// 2. 정상사용극한상태 PC강재 최대응력도 (텐던별)
struct T_FYPC_JTG	// TdnaK.
{
    BOOL bCHK;		  //설계결과유무
    double dFDL1;	  //가설중 응력도			[Unit=Stress]
                                    //포스트텐션(iLoadType)인 경우=정착구에서의 응력
    double dFDL2;	  //가설중 응력도			[Unit=Stress]
			            //포스트텐션(iLoadType)인 경우=청착장치의 활동에 의한 손실의 끝 부분에서의 응력
			            //프리텐션인(iLoadType) 경우=사용안함.
    double dFLL;	  //사용하중시 응력도		[Unit=Stress]
    double dAFDL1;	//가설중 허용응력도		[Unit=Stress]
    double dAFDL2;	//가설중 허용응력도		[Unit=Stress]
    double dAFLL; 	//사용하중시 허용응력도		[Unit=Stress]

    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        dFDL1 = dFDL2 = dFLL = 0.0;
        dAFDL1 = dAFDL2 = 0.0;
        dAFLL = 0.0;
        bOK = TRUE;
    }

};

struct T_FYPC_JTG_UNIT
{
    T_FYPC_JTG_UNIT()
    {
        dFDL1  = D_UNITSYS_BASE_STRESS;
        dFDL2  = D_UNITSYS_BASE_STRESS;
        dFLL   = D_UNITSYS_BASE_STRESS;
        dAFDL1 = D_UNITSYS_BASE_STRESS;
        dAFDL2 = D_UNITSYS_BASE_STRESS;
        dAFLL  = D_UNITSYS_BASE_STRESS;
 }
    int dFDL1;
    int dFDL2;
    int dFLL;
    int dAFDL1;
    int dAFDL2;
    int dAFLL;
};

// 3. 합성응력도 (FY_CM)
struct T_FYCM_JTG_BASE
{
    BOOL bCHK;		//설계결과유무 
    UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iLoadTerm;   // 1:단기하중조합, 2:장기하중조합
    double dFT;   //상단응력도
    double dFB;   //하단응력도
    double dFTL;	//상단좌단 합성응력도	[Unit=Stress]
    double dFBL;	//하단좌단 합성응력도	[Unit=Stress]
    double dFTR;	//상단우단 합성응력도	[Unit=Stress]
    double dFBR;	//하단우단 합성응력도	[Unit=Stress]
    double dFMAX;	//최대/최소 합성응력도	[Unit=Stress]
    double dALW;	//허용응력도			[Unit=Stress]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        iLoadTerm = 0;
        dFT = dFB = 0.0;
        dFTL = dFBL = dFTR = dFBR = dFMAX = dALW = 0;
        bOK = TRUE;
    }
};

struct T_FYCM_JTG	// ElemK.
{
    T_FYCM_JTG_BASE FycmBase[6];	
    //    단기 : 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    //    장기 : 4=I-MIN, 5=J-MIN

    void Initialize()
    {
        for(int i=0; i<6; i++)
            FycmBase[i].Initialize();
    }

};

struct T_FYCM_JTG_UNIT
{
    T_FYCM_JTG_UNIT()
    {
        dFT   = D_UNITSYS_BASE_STRESS;
        dFB   = D_UNITSYS_BASE_STRESS;
        dFTL  = D_UNITSYS_BASE_STRESS;
        dFBL  = D_UNITSYS_BASE_STRESS;
        dFTR  = D_UNITSYS_BASE_STRESS;
        dFBR  = D_UNITSYS_BASE_STRESS;
        dFMAX = D_UNITSYS_BASE_STRESS;
        dALW  = D_UNITSYS_BASE_STRESS;
    }
    int dFT;
    int dFB;
    int dFTL ;
    int dFBL ;
    int dFTR ;
    int dFBR ;
    int dFMAX;
    int dALW ;

};

// 4. 정상사용극한상태 주응력도 (전단력 최대시)
#define T_FPMS_K unsigned int
struct T_FPMS_JTG_BASE  
{
    BOOL bCHK;		//설계결과유무 
    UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    double dFP1;	//1위치 주응력도		[Unit=Stress]
    double dFP2;	//2위치 주응력도		[Unit=Stress]
    double dFP3;	//3위치 주응력도		[Unit=Stress]
    double dFP4;	//4위치 주응력도		[Unit=Stress]
    double dFP5;	//5위치 주응력도		[Unit=Stress]
    double dFP6;	//6위치 주응력도		[Unit=Stress]
    double dFP7;	//7위치 주응력도		[Unit=Stress]
    double dFP8;	//8위치 주응력도		[Unit=Stress]
    double dFP9;	//9위치 주응력도		[Unit=Stress]
    double dFP10;	//10위치 주응력도		[Unit=Stress]
    double dFMAX; // MAX/MIN 주응력도	[Unit=Stress]
    double dAFP;  // 허용 주응력도    [Unit=Stress]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;    
        dFP1 = dFP2 = dFP3 = dFP4 = dFP5 = dFP6 = dFP7 = dFP8 = dFP9= dFP10 = dFMAX = dAFP = 0;
        bOK = TRUE;
    }
};

struct T_FPMS_JTG	// ElemK.
{
    T_FPMS_JTG_BASE FpmsBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            FpmsBase[i].Initialize();
    }
};

struct T_FPMS_JTG_UNIT
{
    T_FPMS_JTG_UNIT()
    {
        dFP1 = D_UNITSYS_BASE_STRESS;
        dFP2 = D_UNITSYS_BASE_STRESS;
        dFP3 = D_UNITSYS_BASE_STRESS;
        dFP4 = D_UNITSYS_BASE_STRESS;
        dFP5 = D_UNITSYS_BASE_STRESS;
        dFP6 = D_UNITSYS_BASE_STRESS;
        dFP7 = D_UNITSYS_BASE_STRESS;
        dFP8 = D_UNITSYS_BASE_STRESS;
        dFP9 = D_UNITSYS_BASE_STRESS;
        dFP10= D_UNITSYS_BASE_STRESS;
        dFMAX= D_UNITSYS_BASE_STRESS;
        dAFP = D_UNITSYS_BASE_STRESS; 
    }
    int dFP1;
    int dFP2;
    int dFP3;
    int dFP4;
    int dFP5;
    int dFP6;
    int dFP7;
    int dFP8;
    int dFP9;
    int dFP10;
    int dFMAX;
    int dAFP;  
};

//  5. 정상사용극한상태 주응력도 (비틀림 최대시)
#define T_FPMT_K unsigned int
struct T_FPMT_JTG_BASE  
{
    BOOL bCHK;		//설계결과유무 
    UINT LcomK;
    UINT StepK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    double dFP1;	//1위치 주응력도		[Unit=Stress]
    double dFP2;	//2위치 주응력도		[Unit=Stress]
    double dFP3;	//3위치 주응력도		[Unit=Stress]
    double dFP4;	//4위치 주응력도		[Unit=Stress]
    double dFP5;	//5위치 주응력도		[Unit=Stress]
    double dFP6;	//6위치 주응력도		[Unit=Stress]
    double dFP7;	//7위치 주응력도		[Unit=Stress]
    double dFP8;	//8위치 주응력도		[Unit=Stress]
    double dFP9;	//9위치 주응력도		[Unit=Stress]
    double dFP10;	//10위치 주응력도		[Unit=Stress]
    double dFMAX; // MAX/MIN 주응력도	[Unit=Stress]
    double dAFP;  // 허용 주응력도    [Unit=Stress]
    double dfci;  // Initial compression strength [Unit=Stress]
    double dRatio;
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = StepK = 0;
        iMax = 0;    
        dFP1 = dFP2 = dFP3 = dFP4 = dFP5 = dFP6 = dFP7 = dFP8 = dFP9= dFP10 = dFMAX = dAFP = 0.0;
        dfci = 0.0;
        dRatio = 0.0;
        bOK = TRUE;
    }

    T_FPMT_JTG_BASE()	{ Initialize(); }
	T_FPMT_JTG_BASE(const T_FPMT_JTG_BASE& rData)	{*this = rData;}
	T_FPMT_JTG_BASE& operator = (const T_FPMT_JTG_BASE& rData)
	{
        bCHK   = rData.bCHK  ;
        LcomK  = rData.LcomK ;
        StepK  = rData.StepK ;
        iMax   = rData.iMax ;    
        dFP1   = rData.dFP1 ;
        dFP2   = rData.dFP2 ;
        dFP3   = rData.dFP3 ;
        dFP4   = rData.dFP4 ;
        dFP5   = rData.dFP5 ;
        dFP6   = rData.dFP6 ;
        dFP7   = rData.dFP7 ;
        dFP8   = rData.dFP8 ;
        dFP9   = rData.dFP9 ;
        dFP10  = rData.dFP10;
        dFMAX  = rData.dFMAX;
        dAFP   = rData.dAFP ;
        dfci   = rData.dfci ;
        dRatio = rData.dRatio;
        bOK    = rData.bOK   ;
		return *this;
	}

};

struct T_FPMT_JTG	// ElemK.
{
    T_FPMT_JTG_BASE FpmtBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            FpmtBase[i].Initialize();
    }
    T_FPMT_JTG()	{ Initialize(); }
	T_FPMT_JTG(const T_FPMT_JTG& rData)	{*this = rData;}
	T_FPMT_JTG& operator = (const T_FPMT_JTG& rData)
	{
        for(int i=0; i<4; ++i) FpmtBase[i] = rData.FpmtBase[i];
        return *this;
    }

};

struct T_FPMT_JTG_UNIT
{
    T_FPMT_JTG_UNIT()
    {
        dFP1 = D_UNITSYS_BASE_STRESS;
        dFP2 = D_UNITSYS_BASE_STRESS;
        dFP3 = D_UNITSYS_BASE_STRESS;
        dFP4 = D_UNITSYS_BASE_STRESS;
        dFP5 = D_UNITSYS_BASE_STRESS;
        dFP6 = D_UNITSYS_BASE_STRESS;
        dFP7 = D_UNITSYS_BASE_STRESS;
        dFP8 = D_UNITSYS_BASE_STRESS;
        dFP9 = D_UNITSYS_BASE_STRESS;
        dFP10= D_UNITSYS_BASE_STRESS;
        dFMAX= D_UNITSYS_BASE_STRESS;
        dfci = D_UNITSYS_BASE_STRESS;
        dAFP = D_UNITSYS_BASE_STRESS; 
    }
    int dFP1;
    int dFP2;
    int dFP3;
    int dFP4;
    int dFP5;
    int dFP6;
    int dFP7;
    int dFP8;
    int dFP9;
    int dFP10;
    int dfci;
    int dFMAX;
    int dAFP;  
};

// 7. 정상사용극한상태 인장철근
struct T_TBAR_JTG_BASE
{
    BOOL bCHK;		//설계결과유무
    UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    double dMj;   //                [Unit=Moment]
    double dREQ;	//필요철근량			[Unit=Area]
    double dMIN;	//최소철근량			[Unit=Area]
    double dUSE;	//사용철근량			[Unit=Area]
    BOOL bOK;
    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        dMj = 0.0;
        dREQ = dMIN = dUSE = 0.0;
        bOK = TRUE;
    }

};

struct T_TBAR_JTG	// ElemK.
{
    T_TBAR_JTG_BASE TbarBase[4];	// 0=I-TOP, 1=I-BOT, 2:J-TOP, 3:J-Bot.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            TbarBase[i].Initialize();
    }
};

struct T_TBAR_JTG_UNIT
{
    T_TBAR_JTG_UNIT()
    {
        dMj  = D_UNITSYS_BASE_MOMENT;
        dREQ = D_UNITSYS_BASE_AREA;
        dMIN = D_UNITSYS_BASE_AREA;
        dUSE = D_UNITSYS_BASE_AREA;
    }
    int dMj;
    int dREQ;
    int dMIN;
    int dUSE;
};

// 8. 필요텐던량.
//#define T_TNDN_K unsigned int
struct T_TNDN_JTG_BASE
{
    BOOL bCHK;		//설계결과유무
    UINT StagK;	  //스테이지번호			[Unit=None]
    UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    UINT LcomKSer;
    int iMaxSer;     //MIN-MAX
    
    double dMg1;    // 시공단계 Moment                [Unit=Moment]
    double dMsum;   // 사용단계 Moment                [Unit=Moment]
    double dMj;     //                                [Unit=Moment]  
    double dey;     // 중립축으로부터 텐던의 중심거리 [Unit=Length]
    double dNymin;	// 최소필요긴장력	  			  			[Unit=Force]
    double dAymin;	// 필요텐던량		    			  			[Unit=Area]
    double dAyuse;	// 사용텐던량		    			  			[Unit=Area]
         //add by juhonghua
    UINT LCombKCrack;
    int iMaxCrack;     //MIN-MAX

    double dMcr;    // 시공단계 Moment                [Unit=Moment]
    double dMud;   // 사용단계 Moment                [Unit=Moment]

    BOOL bOK;
    void Initialize()
    {
        bCHK  = 0;
        StagK = 0;
        LcomK = 0;
        iMax  = 0;
        LcomKSer=0;
        iMaxSer=0;
        dMg1 = dMsum = dMj = 0.0;
        dey = dNymin = dAymin = dAyuse = 0.0;
        dMcr = dMud = 0.0;
        LCombKCrack = 0;
        iMaxCrack = 0;
        bOK = TRUE;
    }

};

struct T_TNDN_JTG	// ElemK.
{
    T_TNDN_JTG_BASE TndnBase[4];	// 0=I-TOP, 1=I-BOT, 2:J-TOP, 3:J-Bot.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            TndnBase[i].Initialize();
    }
};

struct T_TNDN_JTG_UNIT
{
    T_TNDN_JTG_UNIT()
    {
        dMg1   = D_UNITSYS_BASE_MOMENT;
        dMsum  = D_UNITSYS_BASE_MOMENT;
        dMj    = D_UNITSYS_BASE_MOMENT;
        dey    = D_UNITSYS_BASE_LENGTH;
        dNymin = D_UNITSYS_BASE_FORCE;
        dAymin = D_UNITSYS_BASE_AREA;
        dAyuse = D_UNITSYS_BASE_AREA;
        dMcr   =D_UNITSYS_BASE_MOMENT;
        dMud   = D_UNITSYS_BASE_MOMENT;

    }
    int dMg1;  
    int dMsum; 
    int dMj;   
    int dey;   
    int dNymin;
    int dAymin;
    int dAyuse;
    int dMcr;
    int dMud;
};

//  9. 재하능력극한상태 휨강도 (강축)
struct T_BDCY_JTG_BASE
{
    BOOL bCHK;		//설계결과유무
    UINT LcomK;		//하중조합번호		[Unit=None]
    int iMax;	    // 	하중형식 (타입MIN-MAX타입이 아님=0, FX-MAX=1~MZ-MIN=12) 	[Unit=None]
    double dMuy;	//면내 계수모멘트		[Unit=Moment]
    double drMuy; // r0*Muy           [Unit=Moment]
    double dMny;  //면내 설계휨모멘트	[Unit=Moment]
    double dRatY;	//면내 휨강도 안전율	[Unit=None]

    double dx;	//  [Unit=Length]//add by juhonghua
    double dEb; //  [Unit=Length]
    double d2a;  //	[Unit=Length]

    double dReBarRatio;    // [Unit=None]
    double dMinReBarRatio;    // [Unit=None]

    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        dMuy = drMuy = dMny = dRatY = 0.0;
        dx = dEb = d2a = 0.0;
        dReBarRatio = dMinReBarRatio = 0.0;
        bOK = TRUE;
    }
};

struct T_BDCY_JTG	// ElemK.
{
    T_BDCY_JTG_BASE BdcyBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            BdcyBase[i].Initialize();
    }
};

struct T_BDCY_JTG_UNIT
{
    T_BDCY_JTG_UNIT()
    {
        dMuy = D_UNITSYS_BASE_MOMENT;	
        drMuy= D_UNITSYS_BASE_MOMENT;	
        dMny = D_UNITSYS_BASE_MOMENT;  
        dRatY= D_UNITSYS_NONE;	
        dx = D_UNITSYS_BASE_LENGTH;	
        dEb= D_UNITSYS_BASE_LENGTH;	
        d2a = D_UNITSYS_BASE_LENGTH;  
    }
 
    int dMuy;
    int drMuy;
    int dMny;
    int dRatY;  
    int dx;//add by juhonghua
    int dEb;
    int d2a;

};

// 10. 재하능력극한상태 전단강도
struct T_CUMS_JTG_BASE
{
    BOOL bCHK;		  //설계결과유무
    UINT LcomK;		  //하중조합번호		[Unit=None]
    int iMax;	      // 	하중형식 (타입MIN-MAX타입이 아님=0, FX-MAX=1~MZ-MIN=12) 	[Unit=None]
    double dVu;	    //면내 계수전단력	  	 [Unit=Force]
    double drVu;    // r0*Vu               [Unit=Force]
    double dMu;     //면내 계수모멘트      [Unit=Moment]
    double dVn;		  //단면의 공칭전단력		 [Unit=Force]
    double dRatV;   //면내 전단강도 안전율 [Unit=None]
    double dShrSect; //휨부재의 전단단면 검토   [Unit=Force]
    double dShrLoad; //전단재하력검토           [Unit=Force]
    double dMu_P;   //철근합력으로 계산한 단면모멘트[Unit=Moment]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        dVu = drVu = dMu = dVn = dRatV = 0.0;
        dShrSect = 0.0;
        dShrLoad = 0.0;
        dMu_P = 0.0;
        bOK = TRUE;
    }
};

struct T_CUMS_JTG	// ElemK.
{
    T_CUMS_JTG_BASE CumsBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            CumsBase[i].Initialize();
    }
};

struct T_CUMS_JTG_UNIT
{
    T_CUMS_JTG_UNIT()
    {
        dVu   =   D_UNITSYS_BASE_FORCE;
        drVu  =   D_UNITSYS_BASE_FORCE;
        dMu   =   D_UNITSYS_BASE_MOMENT;    
        dVn   =   D_UNITSYS_BASE_FORCE;
        dRatV =   D_UNITSYS_NONE; 
        dShrSect = D_UNITSYS_BASE_FORCE;
        dShrLoad = D_UNITSYS_BASE_FORCE;
        dMu_P =   D_UNITSYS_BASE_MOMENT;
    }
    int dVu;
    int drVu;
    int dMu;
    int dVn;
    int dRatV;  
    int dShrSect;
    int dShrLoad;
    int dMu_P;
};

// 11. 재하능력극한상태 비틀림강도  
struct T_CRMT_JTG_BASE
{
    BOOL bCHK;		  //설계결과유무
    UINT LcomK;		  //하중조합번호		[Unit=None]
    int iMax;	     	//하중형식 (타입MIN-MAX타입이 아님=0, FX-MAX=1~MZ-MIN=12)  [Unit=None]
    double dTu;		  //계수 비틀림모멘트		[Unit=Moment]
    double drTu;     //r0*Tu               [Unit=Moment]
    double dTn;    	//공칭비틀림 모멘트 	[Unit=Moment]
    double dMu;     //계수 모멘트         [Unit=Moment]
    double dVu;     //계수 전단력         [Unit=Force]
    double drVu;    //r0*Vu               [Unit=Force]
    double dVn;     //공칭전단강도        [Unit=Force]
    double dRatT;   //비틀림강도 안전율   [Unit=None]
    double dShrTorVal;  //비틀림단면과 재하력 검토 기준값.         [Unit=Stress] JTG D62-04, (5.5.3-1), (5.5.3-2) 참고 
    double dTorSect;    //휨-전단-비틀림을 받는 직사각형 단면검토  [Unit=Stress] 
    double dTorLoad;    //비틀림 재하력검토                        [Unit=Stress] 
    double dMu_P;   //철근합력으로 계산한 단면모멘트[Unit=Moment]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        dTu = drTu = dTn = dMu= dVu = drVu = dVn = dRatT = 0;
        dShrTorVal = 0.0;
        dTorSect   = 0.0;
        dTorLoad   = 0.0;
        dMu_P = 0.0;
        bOK = TRUE;
    }
};

struct T_CRMT_JTG	// ElemK.
{
    T_CRMT_JTG_BASE CrmtBase[6];	// 0=I-V-MAX, 1=I-V-MIN, 2=I-T-MAX, 3=J-V-MAX, 4=J-V-MIN, 5=J-T-MAX.

    void Initialize()
    {
        for(int i=0; i<6; i++)
            CrmtBase[i].Initialize();
    }
};

struct T_CRMT_JTG_UNIT
{
    T_CRMT_JTG_UNIT()
    {
        dTu   = D_UNITSYS_BASE_MOMENT;
        drTu  = D_UNITSYS_BASE_MOMENT;
        dMu   = D_UNITSYS_BASE_MOMENT;
        dVu   = D_UNITSYS_BASE_FORCE;
        drVu  = D_UNITSYS_BASE_FORCE;
        dVn   = D_UNITSYS_BASE_FORCE;
        dTn   = D_UNITSYS_BASE_MOMENT;
        dRatT = D_UNITSYS_NONE; 
        dShrTorVal = D_UNITSYS_BASE_STRESS;
        dTorSect   = D_UNITSYS_BASE_STRESS;
        dTorLoad   = D_UNITSYS_BASE_STRESS;
        dMu_P = D_UNITSYS_BASE_MOMENT;
    }
    int dTu;
    int drTu;
    int dMu;
    int dVu;
    int drVu;
    int dVn;
    int dTn;
    int dRatT;
    int dShrTorVal;
    int dTorSect;
    int dTorLoad;
    int dMu_P;
};

//+++++++++++++++++++++++++++++++++++++++++++++
// Load And Resistance Factor Rating(LRFR)
// Rating Factor Design.
//+++++++++++++++++++++++++++++++++++++++++++++

struct T_RFDL_B
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

    T_RFDL_B() {Initialize();}
};

// Stress.
struct T_RTST_BASE_845
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
    UINT nSpanPos;           // Span postion                           [Unit=None]
    double dPhi_CF;          // Condition Factor                       [Unit=None]
    double dPhi_SF;          // System Factor                          [Unit=None]  
    double dPhi_RE;          // Resistance Factor                      [Unit=None]
    double drDC;             // DC Factor                              [Unit=None]
    double drDW;             // DW Factor                              [Unit=None]
    double drTE;             // Temperature Factor                     [Unit=None]
    double drTG;             // Temperature Gradient Factor            [Unit=None]
    double drP;              // Permanent Factor                       [Unit=None]
    double drSE;             // Secondary Factor                       [Unit=None]
    double drUS;             // User Factor                            [Unit=None]
    double drPR;             // Primary Vehicle Factor                 [Unit=None]
    double drAD;             // Adjacent Vehicle Factor                [Unit=None]
    double dStress_AL[4];    // Alw Stress(0=Left Top,1=RT,2=RB,3=LB)  [Unit=Stress]
    double dStress_DC[4];    // DC                                     [Unit=Stress]
    double dStress_DW[4];    // DW                                     [Unit=Stress]
    double dStress_TE[4];    // Temperature                            [Unit=Stress]
    double dStress_TG[4];    // Temperature gradient                   [Unit=Stress]
    double dStress_P[4];     // Permanet                               [Unit=Stress]
    double dStress_SE[4];    // Secondary                              [Unit=Stress]
    double dStress_US[4];    // User                                   [Unit=Stress]
    double dStress_PR[4];    // Primary Vehicle                        [Unit=Stress]
    double dStress_AD[4];    // Adjacent Vehicle                       [Unit=Stress]
    double dStress_SUM[4];   // DC+DW+TE+SE+P+US+AD                    [Unit=Stress]
    double dRF[2];           // Rating Factor                          [Unit=None]
    double dRT[2];           // Safe Load Capacity(tons)               [Unit=None]
    double dRFmin;
    double dRel_Locat;       // Relative Location                      [Unit=None]

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
        drDC = drDW = drTE = drTG = drP = drSE = drUS = drPR = drAD = 0.0;   
        for(int i=0; i<4; i++)
        {
            dStress_AL[i] = 0.0;    
            dStress_DC[i] = 0.0;    
            dStress_DW[i] = 0.0;    
            dStress_TE[i] = 0.0;
            dStress_TG[i] = 0.0;
            dStress_P[i]  = 0.0;     
            dStress_SE[i] = 0.0;    
            dStress_US[i] = 0.0;    
            dStress_PR[i] = 0.0;    
            dStress_AD[i] = 0.0;    
            dStress_SUM[i]= 0.0;
        }
        dRF[0] = dRF[1] = 0.0;
        dRT[0] = dRT[1] = 0.0;
        dRFmin = 0.0;
        dRel_Locat = 0.0;
    }
};

struct T_RTST_BASE
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
    UINT nSpanPos;           // Span postion                           [Unit=None]
    double dPhi_CF;          // Condition Factor                       [Unit=None]
    double dPhi_SF;          // System Factor                          [Unit=None]  
    double dPhi_RE;          // Resistance Factor                      [Unit=None]
    double drDC;             // DC Factor                              [Unit=None]
    double drDW;             // DW Factor                              [Unit=None]
    double drTE;             // Temperature Factor                     [Unit=None]
    double drTG;             // Temperature Gradient Factor            [Unit=None]
    double drP;              // Permanent Factor                       [Unit=None]
    double drSE;             // Secondary Factor                       [Unit=None]
    double drUS;             // User Factor                            [Unit=None]
    double drPR;             // Primary Vehicle Factor                 [Unit=None]
    double drAD;             // Adjacent Vehicle Factor                [Unit=None]
    double dStress_AL[4];    // Alw Stress(0=Left Top,1=RT,2=RB,3=LB)  [Unit=Stress]
    double dStress_DC[4];    // DC                                     [Unit=Stress]
    double dStress_DW[4];    // DW                                     [Unit=Stress]
    double dStress_TE[4];    // Temperature                            [Unit=Stress]
    double dStress_TG[4];    // Temperature gradient                   [Unit=Stress]
    double dStress_P[4];     // Permanet                               [Unit=Stress]
    double dStress_SE[4];    // Secondary                              [Unit=Stress]
    double dStress_US[4];    // User                                   [Unit=Stress]
    double dStress_PR[4];    // Primary Vehicle                        [Unit=Stress]
    double dStress_AD[4];    // Adjacent Vehicle                       [Unit=Stress]
    double dStress_SUM[4];   // DC+DW+TE+SE+P+US+AD                    [Unit=Stress]
    double dRF[2];           // Rating Factor                          [Unit=None]
    double dRT[2];           // Safe Load Capacity(tons) weight*K*RF   [Unit=None]
    double dRFmin;
    double dRel_Locat;       // Relative Location                      [Unit=None]
    double dKRF[2];          // K*RF
    double dKRFmin;
    double dWeight;

    T_RFDL_B DtkS[2];        //

    double dCompFpb;
    double dTensFpb;
    double dAlwComp;
    double dAlwTens;

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
        drDC = drDW = drTE = drTG = drP = drSE = drUS = drPR = drAD = 0.0;   
        for(int i=0; i<4; i++)
        {
            dStress_AL[i] = 0.0;    
            dStress_DC[i] = 0.0;    
            dStress_DW[i] = 0.0;    
            dStress_TE[i] = 0.0;
            dStress_TG[i] = 0.0;
            dStress_P[i]  = 0.0;     
            dStress_SE[i] = 0.0;    
            dStress_US[i] = 0.0;    
            dStress_PR[i] = 0.0;    
            dStress_AD[i] = 0.0;    
            dStress_SUM[i]= 0.0;
        }
        for (int i=0; i<2; ++i)
        {
            dRF[i] = 0.0;
            dRT[i] = 0.0;
            dKRF[i] = 0.0;
            DtkS[i].Initialize();
        }    
        dRFmin = 0.0;
        dRel_Locat = 0.0;
        dKRFmin = 0.0;
        dWeight = 0.0;    
        dCompFpb = 0.0;
        dTensFpb = 0.0;
        dAlwComp = 0.0;
        dAlwTens = 0.0;
    }

    void Convert845(const T_RTST_BASE_845 &rData)
    {
        bChk        = rData.bChk;
        ElemK       = rData.ElemK;
        nPosi       = rData.nPosi;
        nGrupKey    = rData.nGrupKey;
        RCaseK      = rData.RCaseK;
        bDCmax      = rData.bDCmax;
        bDWmax      = rData.bDWmax;
        bTEmax      = rData.bTEmax;
        nConcurrent = rData.nConcurrent;
        nPoint[0]   = rData.nPoint[0];
        nPoint[1]   = rData.nPoint[1];
        nIsLocation = rData.nIsLocation;
        SpanKey     = rData.SpanKey;
        nSpanPos    = rData.nSpanPos;
        dPhi_CF     = rData.dPhi_CF;
        dPhi_SF     = rData.dPhi_SF;
        dPhi_RE     = rData.dPhi_RE;
        drDC        = rData.drDC;
        drDW        = rData.drDW;
        drTE        = rData.drTE;
        drTG        = rData.drTG;
        drP         = rData.drP ;
        drSE        = rData.drSE;
        drUS        = rData.drUS;
        drPR        = rData.drPR;
        drAD        = rData.drAD;   
        for(int i=0; i<4; i++)
        {
            dStress_AL[i] = rData.dStress_AL[i] ;    
            dStress_DC[i] = rData.dStress_DC[i] ;    
            dStress_DW[i] = rData.dStress_DW[i] ;    
            dStress_TE[i] = rData.dStress_TE[i] ;
            dStress_TG[i] = rData.dStress_TG[i] ;
            dStress_P[i]  = rData.dStress_P[i]  ;     
            dStress_SE[i] = rData.dStress_SE[i] ;    
            dStress_US[i] = rData.dStress_US[i] ;    
            dStress_PR[i] = rData.dStress_PR[i] ;    
            dStress_AD[i] = rData.dStress_AD[i] ;    
            dStress_SUM[i]= rData.dStress_SUM[i];
        }
        for (int i=0; i<2; ++i)
        {
            dRF[i] = rData.dRF[i];
            dRT[i] = rData.dRT[i];
            dKRF[i] = rData.dRF[i]; //      
        }
        dRFmin     = rData.dRFmin;
        dRel_Locat = rData.dRel_Locat;
        dKRFmin = 0.0;
        dWeight = 0.0;
    }
};

// Rating Case
struct T_RTST_CASE_845
{
    // 0=I-Position, 1=J-Position
    T_RTST_BASE_845 RtstBase[2];

    void Initialize()
    {
        for(int i=0; i<2; i++)
            RtstBase[i].Initialize();
    }
};

struct T_RTST_CASE
{
     // 0=I-Position, 1=J-Position
     T_RTST_BASE RtstBase[2];

     void Initialize()
     {
         for(int i=0; i<2; i++)
             RtstBase[i].Initialize();
     }

     void Convert845(const T_RTST_CASE_845 &rData)
     {
         for(int i=0; i<2; i++) RtstBase[i].Convert845(rData.RtstBase[i]);

     }
};

// Rating Factor(smallest)
struct T_RTST_D_845
{
    // 0=I-Compression, 1=I-Tension
    // 2=J-Compression, 3=J-Tension.
    T_RTST_BASE_845 RtstBase[4];

    void Initialize()
    {
        for(int i=0; i<4; i++)
            RtstBase[i].Initialize();
    }
};

struct T_RTST_D
{
     // 0=I-Compression, 1=I-Tension
     // 2=J-Compression, 3=J-Tension.
     T_RTST_BASE RtstBase[4];
     
     void Initialize()
     {
         for(int i=0; i<4; i++)
             RtstBase[i].Initialize();
     }

     void Convert845(const T_RTST_D_845 &rData)
     {
         for(int i=0; i<4; i++) RtstBase[i].Convert845(rData.RtstBase[i]);
     }
};

struct T_RTST_UNIT
{
    T_RTST_UNIT()
    {
        for(int i=0; i<4; i++)
        {
            dStress_AL[i]  = D_UNITSYS_BASE_STRESS;
            dStress_DC[i]  = D_UNITSYS_BASE_STRESS;
            dStress_DW[i]  = D_UNITSYS_BASE_STRESS;
            dStress_TE[i]  = D_UNITSYS_BASE_STRESS;
            dStress_TG[i]  = D_UNITSYS_BASE_STRESS;
            dStress_P[i]   = D_UNITSYS_BASE_STRESS;
            dStress_SE[i]  = D_UNITSYS_BASE_STRESS;
            dStress_US[i]  = D_UNITSYS_BASE_STRESS;
            dStress_PR[i]  = D_UNITSYS_BASE_STRESS;
            dStress_AD[i]  = D_UNITSYS_BASE_STRESS;
            dStress_SUM[i] = D_UNITSYS_BASE_STRESS;
        }
        dStress = D_UNITSYS_BASE_STRESS;
    }  
    int dStress_AL[4];
    int dStress_DC[4];
    int dStress_DW[4];
    int dStress_TE[4];
    int dStress_TG[4];
    int dStress_P[4];  
    int dStress_SE[4];
    int dStress_US[4];
    int dStress_PR[4];
    int dStress_AD[4];
    int dStress_SUM[4];
    int dStress;
};

struct T_RTST_PERMIT_BASE
{
    BOOL bChk;
    BOOL bDCmax;             // DC(Max,Min Factor)                     [Unit=None]
    BOOL bDWmax;             // DW(Max,Min Factor)                     [Unit=None]
    BOOL bTEmax;             // Temperature(Max,Min Factor)            [Unit=None]
    BOOL bCrack;
    T_ELEM_K ElemK;
    T_GRUP_K nGrupKey;			 // Group Key
    UINT nPosi;              // 1=I-End, 2=J-END
    UINT RCaseK;
    UINT nConcurrent;        // Fx_max .... Mz_min(1~12)               [Unit=None] 
    UINT nIsLocation;        // Is Location Data 0=Non, 1=Exist        [Unit=None]
    UINT nSpanPos;     // Span postion                           [Unit=None]
    T_SPAN_K SpanKey;  // Span Key                               [Unit=None]
    
    double dPhi_CF;    // Condition Factor           [Unit=None]  
    double dPhi_SF;    // System Factor              [Unit=None]
    double dPhi_RE;    // Resistance Factor          [Unit=None]
    double dRel_Locat;       // Relative Location                      [Unit=None]

    double dAppliedStress;
    double dAllowStress;
    double dAllowStressTendon;
    double dAllowStressRebar;
    double dStressRatio;
    double dStressRatioMin;

    double dMdnc;
    double dM_Demand;
    double dMcr;
    double dM_Dead;
    double dM_Live;
    double dM_Sum;

    double dEffectStress;
    double dStress_Dead;
    double dStress_Live;
    double dStress_Mcr;
    double dStress_AfterCr;

    double dIcr;
    double dNeu;
    double dElast;

    int  nTendonRebar;           // 1이면 Rebar, 2이면 Tendon, 0이면 "-"처리

    //0 Top, 1 Bot;
    T_PSCD_FORCE ForceGirder;
    T_PSCD_FORCE ForceShort;
    T_PSCD_FORCE ForceLong;
    T_PSCD_FORCE ForceSum; //ShortTerm + LongTerm

    void Initialize()
    {
        bChk = FALSE;
        bDCmax = bDWmax = bTEmax = TRUE;
        bCrack = FALSE;
        ElemK =0;
        nGrupKey = 0;
        nPosi = 0;
        RCaseK = 0;
        nConcurrent = 0;
        nIsLocation = 0;
        SpanKey = 0;
        nSpanPos = 0;

        dPhi_CF = dPhi_SF = dPhi_RE = 0.0;
        dRel_Locat = 0.0;

        dAppliedStress = 0.0;
        dAllowStress = 0.0;
        dAllowStressTendon = 0.0;
        dAllowStressRebar = 0.0;
        dStressRatio = 0.0;
        dStressRatioMin = 0.0;

        dMdnc = 0.0;
        dM_Demand = 0.0;
        dMcr = 0.0;
        dM_Dead = 0.0;
        dM_Live = 0.0;
        dM_Sum = 0.0;

        dEffectStress = 0.0;
        dStress_Dead = 0.0;
        dStress_Live = 0.0;
        dStress_Mcr  = 0.0;
        dStress_AfterCr = 0.0;

        dIcr = 0.0;
        dNeu = 0.0;
        dElast = 0.0;
        nTendonRebar = 0;

        ForceGirder.Initialize();
        ForceShort.Initialize();
        ForceLong.Initialize();
        ForceSum.Initialize();
    }
};

struct T_RTST_PERMIT_CASE
{
    // 0=I-Position, 1=J-Position
    T_RTST_PERMIT_BASE RtstPermitBase[2];

    void Initialize()
    {
        for ( int i=0; i<2; i++ )
            RtstPermitBase[i].Initialize();
    }
};

struct T_RTST_PERMIT_D
{
    //기획자로부터 Tension만 검토한다고 했지만 사용자의 추후 요청을 고려해서 Compression도 검토하게 세팅함
    // 0=I-Tension, 1=J-Tension
    T_RTST_PERMIT_BASE RtstPermitBase[2];

    void Initialize()
    {
        for ( int i=0; i<2; i++ )
            RtstPermitBase[i].Initialize();
    }
};

struct T_RTST_PERMIT_UNIT
{
    T_RTST_PERMIT_UNIT()
    {
        dMdnc = D_UNITSYS_BASE_MOMENT;
        dM_Dead = D_UNITSYS_BASE_MOMENT;
        dM_Live = D_UNITSYS_BASE_MOMENT;
        dM_Demand = D_UNITSYS_BASE_MOMENT;
        dM_Sum = D_UNITSYS_BASE_MOMENT;
        dMcr = D_UNITSYS_BASE_MOMENT;
        dEffectStress = D_UNITSYS_BASE_STRESS;
        dStress_Dead = D_UNITSYS_BASE_STRESS;
        dStress_Live = D_UNITSYS_BASE_STRESS;
        dAppliedStress = D_UNITSYS_BASE_STRESS;
        dAllowStress = D_UNITSYS_BASE_STRESS;
        dAllowStressTendon = D_UNITSYS_BASE_STRESS;
        dAllowStressRebar = D_UNITSYS_BASE_STRESS;
    }
    int dMdnc;
    int dM_Dead;
    int dM_Live;
    int dM_Demand; // dM_Dead + dM_Live
    int dM_Sum;
    int dMcr;
    int dEffectStress;
    int dStress_Dead;
    int dStress_Live;
    int dAppliedStress;
    int dAllowStress;
    int dAllowStressTendon;
    int dAllowStressRebar;
};

// Tendon Stress.
struct T_RTTS_KEY
{
    ElemPairK ElemK;
    UINT TdnaK;
    int  nPosi;  // 0:I, 1:J
    operator unsigned long() 
    {
        //assert(TdnaK < 256);
        //assert(ElemK.first < (1u << 21));
        //assert(ElemK.second < 4);      // 2bit
        //assert(nPosi == 0 || nPosi == 1);

        return  (unsigned long(TdnaK) << 24)
            | (unsigned long(ElemK.first) << 3)
            | (unsigned long(ElemK.second) << 1)
            | (unsigned long(nPosi));        
    }

    BOOL operator==(const T_RTTS_KEY& rKey) const
    {
        if(ElemK == rKey.ElemK && TdnaK == rKey.TdnaK && nPosi == rKey.nPosi) 
            return TRUE;
        else 
            return FALSE;
    }
};

struct T_RTTS_TDNA
{
    UINT ElemK;
    UINT NumTdnaI;
    UINT NumTdnaJ;
    void Initialize()
    {
        ElemK    = 0;
        NumTdnaI = 0;
        NumTdnaJ = 0;
    }
};

struct T_RTTS_BASE_845 // Rating Tendon Stress
{
    BOOL bChk;
    UINT ElemK;
    UINT nPosi;
    UINT TdgrK;
    UINT TdnaK;
    T_GRUP_K nGrupKey;	// Group Key
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
    double drDC;       // DC Factor                  [Unit=None]
    double drDW;       // DW Factor                  [Unit=None]
    double drTE;       // Temperature Factor         [Unit=None]
    double drTG;       // Temperature Gradient       [Unit=None]
    double drP;        // Permanent Factor           [Unit=None]
    double drSE;       // Secondary Factor           [Unit=None]
    double drUS;       // User Factor                [Unit=None]
    double drPR;       // Primary Vehicle Factor     [Unit=None]
    double drAD;       // Adjacent Vehicle Factor    [Unit=None]
    double dPs_AL;     // Tendon Allow Stress        [Unit=Stress]
    double dPs_DC;     // DC (Stress)                [Unit=Stress]
    double dPs_DW;     // DW                         [Unit=Stress]
    double dPs_TE;     // Temperature                [Unit=Stress]
    double dPs_TG;     // Temperature Gradient       [Unit=Stress]
    double dPs_P;      // Permanet                   [Unit=Stress]
    double dPs_SE;     // Secondary                  [Unit=Stress]
    double dPs_US;     // User                       [Unit=Stress]
    double dPs_PR;     // Primary Vehicle            [Unit=Stress]
    double dPs_AD;     // Adjacent Vehicle           [Unit=Stress]
    double dPs_SUM;    // DC+DW+TE+SE+P+US+AD        [Unit=Stress]
    double dRF;        // Rating Factor              [Unit=None]
    double dRT;        // Safe Load Capacity(tons)   [Unit=None]
    double dRel_Locat; // Relative Location          [Unit=None]

    void Initialize()
    {
        bChk = 0;
        ElemK = 0;
        nPosi = 0;
        TdgrK = 0;
        TdnaK = 0;
        nGrupKey = 0;
        RCaseK = 0;
        bDCmax=bDWmax=bTEmax=TRUE;
        nConcurrent = 0;  
        nIsLocation = 0;
        SpanKey = 0;
        nSpanPos = 0;
        dPhi_CF = dPhi_SF = dPhi_RE = 0.0;
        drDC = drDW = drTE = drTG = drP = drSE = drUS = drPR = drAD = 0.0;   
        dPs_AL = dPs_DC = dPs_DW = dPs_TE = dPs_TG = dPs_P = 0.0;
        dPs_SE = dPs_US = dPs_PR = dPs_AD = 0.0; 
        dPs_SUM = 0.0;
        dRF = dRT = 0.0;
        dRel_Locat = 0.0;
    }
};

struct T_RTTS_BASE // Rating Tendon Stress
{
    BOOL bChk;
    UINT ElemK;
    UINT nPosi;
    UINT TdgrK;
    UINT TdnaK;
	T_GRUP_K nGrupKey;	// Group Key
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
    double drDC;       // DC Factor                  [Unit=None]
    double drDW;       // DW Factor                  [Unit=None]
    double drTE;       // Temperature Factor         [Unit=None]
    double drTG;       // Temperature Gradient       [Unit=None]
    double drP;        // Permanent Factor           [Unit=None]
    double drSE;       // Secondary Factor           [Unit=None]
    double drUS;       // User Factor                [Unit=None]
    double drPR;       // Primary Vehicle Factor     [Unit=None]
    double drAD;       // Adjacent Vehicle Factor    [Unit=None]
    double dPs_AL;     // Tendon Allow Stress        [Unit=Stress]
    double dPs_DC;     // DC (Stress)                [Unit=Stress]
    double dPs_DW;     // DW                         [Unit=Stress]
    double dPs_TE;     // Temperature                [Unit=Stress]
    double dPs_TG;     // Temperature Gradient       [Unit=Stress]
    double dPs_P;      // Permanet                   [Unit=Stress]
    double dPs_SE;     // Secondary                  [Unit=Stress]
    double dPs_US;     // User                       [Unit=Stress]
    double dPs_PR;     // Primary Vehicle            [Unit=Stress]
    double dPs_AD;     // Adjacent Vehicle           [Unit=Stress]
    double dPs_SUM;    // DC+DW+TE+SE+P+US+AD        [Unit=Stress]
    double dRF;        // Rating Factor              [Unit=None]
    double dRT;        // Safe Load Capacity(tons)   [Unit=None]
    double dRel_Locat; // Relative Location          [Unit=None]
    double dKRF;
    double dWeight;

    T_RFDL_B DtkT;

    void Initialize()
    {
        bChk = 0;
        ElemK = 0;
        nPosi = 0;
        TdgrK = 0;
        TdnaK = 0;
        nGrupKey = 0;
        RCaseK = 0;
        bDCmax=bDWmax=bTEmax=TRUE;
        nConcurrent = 0;  
        nIsLocation = 0;
        SpanKey = 0;
        nSpanPos = 0;
        dPhi_CF = dPhi_SF = dPhi_RE = 0.0;
        drDC = drDW = drTE = drTG = drP = drSE = drUS = drPR = drAD = 0.0;   
        dPs_AL = dPs_DC = dPs_DW = dPs_TE = dPs_TG = dPs_P = 0.0;
        dPs_SE = dPs_US = dPs_PR = dPs_AD = 0.0; 
        dPs_SUM = 0.0;
        dRF = dRT = 0.0;
        dRel_Locat = 0.0;
        dKRF = 0.0;
        dWeight = 0.0;
        DtkT.Initialize();
    }

    void Convert845(const T_RTTS_BASE_845 &rData)
    {
        bChk        = rData.bChk    ;
        ElemK       = rData.ElemK   ;
        nPosi       = rData.nPosi   ;
        TdgrK       = rData.TdgrK   ;
        TdnaK       = rData.TdnaK   ;
        nGrupKey    = rData.nGrupKey;
        RCaseK      = rData.RCaseK  ;
        bDCmax      = rData.bDCmax  ;
        bDWmax      = rData.bDWmax  ;
        bTEmax      = rData.bTEmax  ;
        nConcurrent = rData.nConcurrent;  
        nIsLocation = rData.nIsLocation;
        SpanKey     = rData.SpanKey;
        nSpanPos    = rData.nSpanPos;
        dPhi_CF     = rData.dPhi_CF;
        dPhi_SF     = rData.dPhi_SF;
        dPhi_RE     = rData.dPhi_RE;
        drDC        = rData.drDC;
        drDW        = rData.drDW;
        drTE        = rData.drTE;
        drTG        = rData.drTG;
        drP         = rData.drP ;
        drSE        = rData.drSE;
        drUS        = rData.drUS;
        drPR        = rData.drPR;
        drAD        = rData.drAD;   
        dPs_AL      = rData.dPs_AL ;
        dPs_DC      = rData.dPs_DC ;
        dPs_DW      = rData.dPs_DW ;
        dPs_TE      = rData.dPs_TE ;
        dPs_TG      = rData.dPs_TG ;
        dPs_P       = rData.dPs_P  ;
        dPs_SE      = rData.dPs_SE ;
        dPs_US      = rData.dPs_US ;
        dPs_PR      = rData.dPs_PR ;
        dPs_AD      = rData.dPs_AD ; 
        dPs_SUM     = rData.dPs_SUM;
        dRF         = rData.dRF    ;
        dRT         = rData.dRT    ;
        dRel_Locat  = rData.dRel_Locat;
        dKRF        = rData.dRF;
        dWeight     = 0.0;

    }
};

// Rating Case
struct T_RTTS_CASE
{
     // 0=I-Posi, 1=J-Posi
     CArray<T_RTTS_BASE,T_RTTS_BASE> arRttsBase[2];  
     void Initialize()
     {
         for(int i=0; i<2; i++)
             arRttsBase[i].RemoveAll();
     }
};

// Rating Factor(smallest)
struct T_RTTS_D_845
{
    // 0=I-Posi, 1=J-Posi.
    T_RTTS_BASE_845 RttsBase[2];
    void Initialize()
    {
        for(int i=0; i<2; i++)
            RttsBase[i].Initialize();
    }
};

struct T_RTTS_D
{
     // 0=I-Posi, 1=J-Posi.
     T_RTTS_BASE RttsBase[2];
     void Initialize()
     {
         for(int i=0; i<2; i++)
             RttsBase[i].Initialize();
     }

     void Convert845(const T_RTTS_D_845 &rData)
     {
         for(int i=0; i<2; i++) RttsBase[i].Convert845(rData.RttsBase[i]);
     }
};

struct T_RTTS_UNIT
{
    T_RTTS_UNIT()
    {
        dPs_AL  = D_UNITSYS_BASE_STRESS;
        dPs_DC  = D_UNITSYS_BASE_STRESS;
        dPs_DW  = D_UNITSYS_BASE_STRESS;
        dPs_TE  = D_UNITSYS_BASE_STRESS;
        dPs_TG  = D_UNITSYS_BASE_STRESS;
        dPs_P   = D_UNITSYS_BASE_STRESS;
        dPs_SE  = D_UNITSYS_BASE_STRESS;
        dPs_US  = D_UNITSYS_BASE_STRESS;
        dPs_PR  = D_UNITSYS_BASE_STRESS;
        dPs_AD  = D_UNITSYS_BASE_STRESS;
        dPs_SUM = D_UNITSYS_BASE_STRESS;
    }  
    int dPs_AL;
    int dPs_DC;
    int dPs_DW;
    int dPs_TE;
    int dPs_TG;
    int dPs_P; 
    int dPs_SE;
    int dPs_US;
    int dPs_PR;
    int dPs_AD;
    int dPs_SUM;
};


// 휨강도
struct T_RTBC_BASE_845
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
    BOOL bBar_rat_max; // 최대철근비                             [Unit=None]
    BOOL bBar_rat_min; // 최소철근비                             [Unit=None]
    double dPhi_CF;    // Condition Factor           [Unit=None]  
    double dPhi_SF;    // System Factor              [Unit=None]
    double dPhi_RE;    // Resistance Factor          [Unit=None]
    double drDC;       // DC Factor                  [Unit=None]
    double drDW;       // DW Factor                  [Unit=None]
    double drTE;       // Temperature Factor         [Unit=None]
    double drTG;       // Temperature Gradient Factor[Unit=None]
    double drP;        // Permanent Factor           [Unit=None]
    double drSE;       // Secondary Factor           [Unit=None]
    double drUS;       // User Factor                [Unit=None]
    double drPR;       // Primary Vehicle Factor     [Unit=None]
    double drAD;       // Adjacent Vehicle Factor    [Unit=None]
    double dMn_CA;     // Bending Capacity           [Unit=Moment]
    double dMu_DC;     // DC (My)                    [Unit=Moment]
    double dMu_DW;     // DW                         [Unit=Moment]
    double dMu_TE;     // Temperature                [Unit=Moment]
    double dMu_TG;     // Temperature Gradient       [Unit=Moment]
    double dMu_P;      // Permanet                   [Unit=Moment]
    double dMu_SE;     // Secondary                  [Unit=Moment]
    double dMu_US;     // User                       [Unit=Moment]
    double dMu_PR;     // Primary Vehicle            [Unit=Moment]
    double dMu_AD;     // Adjacent Vehicle           [Unit=Moment]
    double dMu_SUM;    // DC+DW+TE+SE+P+US+AD        [Unit=Moment]
    double dRF;        // Rating Factor              [Unit=None]
    double dRT;        // Safe Load Capacity(tons)   [Unit=None]
    double dRel_Locat; // Relative Location          [Unit=None] , 750추가
    double dArea_bar;  // 철근의 면적                [Unit=Area]
    double dArea_ten;  // Tendon의 면적              [Unit=Area]

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
        bBar_rat_max = FALSE;
        bBar_rat_min = FALSE;
        dPhi_CF = dPhi_SF = dPhi_RE = 0.0;
        drDC = drDW = drTE = drTG = drP = drSE = drUS = drPR = drAD = 0.0;   
        dMn_CA = dMu_DC = dMu_DW = dMu_TE = dMu_TG = dMu_P = 0.0;
        dMu_SE = dMu_US = dMu_PR = dMu_AD = 0.0; 
        dMu_SUM = 0.0;
        dRF = dRT = 0.0;
        dRel_Locat = 0.0;
        dArea_bar = 0.0;
        dArea_ten = 0.0;
    }
};

struct T_RTBC_BASE
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
    BOOL bBar_rat_max; // 최대철근비                             [Unit=None]
    BOOL bBar_rat_min; // 최소철근비                             [Unit=None]
    double dPhi_CF;    // Condition Factor           [Unit=None]  
    double dPhi_SF;    // System Factor              [Unit=None]
    double dPhi_RE;    // Resistance Factor          [Unit=None]
    double drDC;       // DC Factor                  [Unit=None]
    double drDW;       // DW Factor                  [Unit=None]
    double drTE;       // Temperature Factor         [Unit=None]
    double drTG;       // Temperature Gradient Factor[Unit=None]
    double drP;        // Permanent Factor           [Unit=None]
    double drSE;       // Secondary Factor           [Unit=None]
    double drUS;       // User Factor                [Unit=None]
    double drPR;       // Primary Vehicle Factor     [Unit=None]
    double drAD;       // Adjacent Vehicle Factor    [Unit=None]
    double dMn_CA;     // Bending Capacity           [Unit=Moment]
    double dMu_DC;     // DC (My)                    [Unit=Moment]
    double dMu_DW;     // DW                         [Unit=Moment]
    double dMu_TE;     // Temperature                [Unit=Moment]
    double dMu_TG;     // Temperature Gradient       [Unit=Moment]
    double dMu_P;      // Permanet                   [Unit=Moment]
    double dMu_SE;     // Secondary                  [Unit=Moment]
    double dMu_US;     // User                       [Unit=Moment]
    double dMu_PR;     // Primary Vehicle            [Unit=Moment]
    double dMu_AD;     // Adjacent Vehicle           [Unit=Moment]
    double dMu_SUM;    // DC+DW+TE+SE+P+US+AD        [Unit=Moment]
    double dRF;        // Rating Factor              [Unit=None]
    double dRT;        // Safe Load Capacity(tons)   [Unit=None]
    double dRel_Locat; // Relative Location          [Unit=None] , 750추가
    double dArea_bar;  // 철근의 면적                [Unit=Area]
    double dArea_ten;  // Tendon의 면적              [Unit=Area]
    double dKRF;
    double dWeight;

    T_RFDL_B DtkF;

    void Initialize()
    {
        bChk = 0;
        ElemK = 0;
        nPosi = 0;
        nMoment = 0;
        nGrupKey = 0;
        RCaseK = 0;
        bDCmax = bDWmax = bTEmax = TRUE;
        nConcurrent = 1;
        nIsLocation = 0;
        SpanKey = 0;
        nSpanPos = 0;
        bBar_rat_max = FALSE;
        bBar_rat_min = FALSE;
        dPhi_CF = dPhi_SF = dPhi_RE = 0.0;
        drDC = drDW = drTE = drTG = drP = drSE = drUS = drPR = drAD = 0.0;   
        dMn_CA = dMu_DC = dMu_DW = dMu_TE = dMu_TG = dMu_P = 0.0;
        dMu_SE = dMu_US = dMu_PR = dMu_AD = 0.0; 
        dMu_SUM = 0.0;
        dRF = dRT = 0.0;
        dRel_Locat = 0.0;
        dArea_bar = 0.0;
        dArea_ten = 0.0;
        dKRF = 0.0;
        dWeight = 0.0;
        DtkF.Initialize();
    }

    void Convert845(const T_RTBC_BASE_845 &rData)
    {
        bChk         = rData.bChk;
        ElemK        = rData.ElemK;
        nPosi        = rData.nPosi;
        nMoment      = rData.nMoment;
        nGrupKey     = rData.nGrupKey;
        RCaseK       = rData.RCaseK;
        bDCmax       = rData.bDCmax;
        bDWmax       = rData.bDWmax;
        bTEmax       = rData.bTEmax;
        nConcurrent  = rData.nConcurrent;
        nIsLocation  = rData.nIsLocation;
        SpanKey      = rData.SpanKey;
        nSpanPos     = rData.nSpanPos;
        bBar_rat_max = rData.bBar_rat_max;
        bBar_rat_min = rData.bBar_rat_min;
        dPhi_CF      = rData.dPhi_CF;
        dPhi_SF      = rData.dPhi_SF;
        dPhi_RE      = rData.dPhi_RE;
        drDC         = rData.drDC;
        drDW         = rData.drDW;
        drTE         = rData.drTE;
        drTG         = rData.drTG;
        drP          = rData.drP ;
        drSE         = rData.drSE;
        drUS         = rData.drUS;
        drPR         = rData.drPR;
        drAD         = rData.drAD;   
        dMn_CA       = rData.dMn_CA;
        dMu_DC       = rData.dMu_DC;
        dMu_DW       = rData.dMu_DW;
        dMu_TE       = rData.dMu_TE;
        dMu_TG       = rData.dMu_TG;
        dMu_P        = rData.dMu_P ;
        dMu_SE       = rData.dMu_SE ;
        dMu_US       = rData.dMu_US ;
        dMu_PR       = rData.dMu_PR ;
        dMu_AD       = rData.dMu_AD ; 
        dMu_SUM      = rData.dMu_SUM;
        dRF          = rData.dRF;
        dRT          = rData.dRT;
        dRel_Locat   = rData.dRel_Locat;
        dArea_bar    = rData.dArea_bar;
        dArea_ten    = rData.dArea_ten;
        dKRF         = rData.dRF;
        dWeight      = 0.0;
    }
};

// Rating Case
struct T_RTBC_CASE_845
{
    // 0=I-Posi, 1=J-Posi
    T_RTBC_BASE_845 RtbcBase[2];

    void Initialize()
    {
        for(int i=0; i<2; i++)
            RtbcBase[i].Initialize();
    }
};

struct T_RTBC_CASE
{
     // 0=I-Posi, 1=J-Posi
     T_RTBC_BASE RtbcBase[2];
     
     void Initialize()
     {
         for(int i=0; i<2; i++)
             RtbcBase[i].Initialize();
     }

     void Convert845(const T_RTBC_CASE_845 &rData)
     {
         for(int i=0; i<2; i++) RtbcBase[i].Convert845(rData.RtbcBase[i]);
     }
};

// Rating Factor(smallest)
struct T_RTBC_D_845
{
    // 0=I-Posi, 1=I-Nega, 2=J-Posi, 3=J-Nega.
    T_RTBC_BASE_845 RtbcBase[4];

    void Initialize()
    {
        for(int i=0; i<4; i++) RtbcBase[i].Initialize();
    }
};

struct T_RTBC_D
{
     // 0=I-Posi, 1=I-Nega, 2=J-Posi, 3=J-Nega.
     T_RTBC_BASE RtbcBase[4];

     void Initialize()
     {
         for(int i=0; i<4; i++)
             RtbcBase[i].Initialize();
     }

     void Convert845(const T_RTBC_D_845 &rData)
     {
         for(int i=0; i<2; i++) RtbcBase[i].Convert845(rData.RtbcBase[i]);
     }

};

struct T_RTBC_UNIT
{
    T_RTBC_UNIT()
    {
        dMn_CA  = D_UNITSYS_BASE_MOMENT;
        dMu_DC  = D_UNITSYS_BASE_MOMENT;
        dMu_DW  = D_UNITSYS_BASE_MOMENT;
        dMu_TE  = D_UNITSYS_BASE_MOMENT;
        dMu_TG  = D_UNITSYS_BASE_MOMENT;
        dMu_P   = D_UNITSYS_BASE_MOMENT;
        dMu_SE  = D_UNITSYS_BASE_MOMENT;
        dMu_US  = D_UNITSYS_BASE_MOMENT;
        dMu_PR  = D_UNITSYS_BASE_MOMENT;
        dMu_AD  = D_UNITSYS_BASE_MOMENT;
        dMu_SUM = D_UNITSYS_BASE_MOMENT;
        dArea_bar = D_UNITSYS_BASE_AREA;
        dArea_ten = D_UNITSYS_BASE_AREA;
    }  
    int dMn_CA;
    int dMu_DC;
    int dMu_DW;
    int dMu_TE;
    int dMu_TG;
    int dMu_P; 
    int dMu_SE;
    int dMu_US;
    int dMu_PR;
    int dMu_AD;
    int dMu_SUM;
    int dArea_bar;
    int dArea_ten;
};

// 전단강도
struct T_RTSC_BASE_845
{
    BOOL bChk;
    T_ELEM_K ElemK;
    UINT nPosi;
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
    double drDC;       // DC Factor                  [Unit=None]
    double drDW;       // DW Factor                  [Unit=None]
    double drTE;       // Temperature Factor         [Unit=None]
    double drTG;       // Temperature Gradient Factor[Unit=None]
    double drP;        // Permanent Factor           [Unit=None]
    double drSE;       // Secondary Factor           [Unit=None]
    double drUS;       // User Factor                [Unit=None]
    double drPR;       // Primary Vehicle Factor     [Unit=None]
    double drAD;       // Adjacent Vehicle Factor    [Unit=None]
    double dVn_CA;     // Shear Capacity             [Unit=Force]
    double dVu_DC;     // DC (Vu)                    [Unit=Force]
    double dVu_DW;     // DW                         [Unit=Force]
    double dVu_TE;     // Temperature                [Unit=Force]
    double dVu_TG;     // Temperature Gradient       [Unit=Force]
    double dVu_P;      // Permanet                   [Unit=Force]
    double dVu_SE;     // Secondary                  [Unit=Force]
    double dVu_US;     // User                       [Unit=Force]
    double dVu_PR;     // Primary Vehicle            [Unit=Force]
    double dVu_AD;     // Adjacent Vehicle           [Unit=Force]
    double dVu_SUM;    // DC+DW+TE+SE+P+US+AD        [Unit=Force]
    double dVu_CAL;    // DC+DW+TE+SE+P+US+AD+PR     [Unit=Force]
    double dMu_CAL;    // DC+DW+TE+SE+P+US+AD+PR     [Unit=Moment]
    double dNu_CAL;    // DC+DW+TE+SE+P+US+AD+PR     [Unit=Force]
    double dRF;        // Rating Factor              [Unit=None]
    double dRT;        // Safe Load Capacity(tons)   [Unit=None]
    double dRel_Locat; // Relative Location          [Unit=None]

    void Initialize()
    {
        bChk = 0;
        ElemK = 0;
        nPosi = 0;
        nGrupKey = 0;
        RCaseK = 0;
        bDCmax = bDWmax = bTEmax = TRUE;
        nConcurrent = 1;
        nIsLocation = 0;
        SpanKey = 0;
        nSpanPos = 0;
        dPhi_CF = dPhi_SF = dPhi_RE = 0.0;
        drDC = drDW = drTE = drTG = drP = drSE = drUS = drPR = drAD = 0.0;   
        dVn_CA = dVu_DC = dVu_DW = dVu_TE = dVu_TG = dVu_P = 0.0;
        dVu_SE = dVu_US = dVu_PR = dVu_AD = 0.0; 
        dVu_SUM=0.0;
        dVu_CAL=0.0;
        dMu_CAL=0.0;
        dNu_CAL=0.0;
        dRF = dRT = 0.0;
        dRel_Locat = 0.0;
    }
};

struct T_RTSC_BASE
{
    BOOL bChk;
    T_ELEM_K ElemK;
    UINT nPosi;
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
    double drDC;       // DC Factor                  [Unit=None]
    double drDW;       // DW Factor                  [Unit=None]
    double drTE;       // Temperature Factor         [Unit=None]
    double drTG;       // Temperature Gradient Factor[Unit=None]
    double drP;        // Permanent Factor           [Unit=None]
    double drSE;       // Secondary Factor           [Unit=None]
    double drUS;       // User Factor                [Unit=None]
    double drPR;       // Primary Vehicle Factor     [Unit=None]
    double drAD;       // Adjacent Vehicle Factor    [Unit=None]
    double dVn_CA;     // Shear Capacity             [Unit=Force]
    double dVu_DC;     // DC (Vu)                    [Unit=Force]
    double dVu_DW;     // DW                         [Unit=Force]
    double dVu_TE;     // Temperature                [Unit=Force]
    double dVu_TG;     // Temperature Gradient       [Unit=Force]
    double dVu_P;      // Permanet                   [Unit=Force]
    double dVu_SE;     // Secondary                  [Unit=Force]
    double dVu_US;     // User                       [Unit=Force]
    double dVu_PR;     // Primary Vehicle            [Unit=Force]
    double dVu_AD;     // Adjacent Vehicle           [Unit=Force]
    double dVu_SUM;    // DC+DW+TE+SE+P+US+AD        [Unit=Force]
    double dVu_CAL;    // DC+DW+TE+SE+P+US+AD+PR     [Unit=Force]
    double dMu_CAL;    // DC+DW+TE+SE+P+US+AD+PR     [Unit=Moment]
    double dNu_CAL;    // DC+DW+TE+SE+P+US+AD+PR     [Unit=Force]
    double dRF;        // Rating Factor              [Unit=None]
    double dRT;        // Safe Load Capacity(tons)   [Unit=None]
    double dRel_Locat; // Relative Location          [Unit=None]
    double dKRF;
    double dWeight;

    T_RFDL_B DtkS;

    void Initialize()
    {
        bChk = 0;
        ElemK = 0;
        nPosi = 0;
        nGrupKey = 0;
        RCaseK = 0;
        bDCmax = bDWmax = bTEmax = TRUE;
        nConcurrent = 1;
        nIsLocation = 0;
        SpanKey = 0;
        nSpanPos = 0;
        dPhi_CF = dPhi_SF = dPhi_RE = 0.0;
        drDC = drDW = drTE = drTG = drP = drSE = drUS = drPR = drAD = 0.0;   
        dVn_CA = dVu_DC = dVu_DW = dVu_TE = dVu_TG = dVu_P = 0.0;
        dVu_SE = dVu_US = dVu_PR = dVu_AD = 0.0; 
        dVu_SUM=0.0;
        dVu_CAL=0.0;
        dMu_CAL=0.0;
        dNu_CAL=0.0;
        dRF = dRT = 0.0;
        dRel_Locat = 0.0;
        dKRF = 0.0;
        dWeight = 0.0;
        DtkS.Initialize();
    }

    void Convert845(const T_RTSC_BASE_845 &rData)
    {
        bChk        = rData.bChk  ;
        ElemK       = rData.ElemK ;
        nPosi       = rData.nPosi ;
        nGrupKey    = rData.nGrupKey;
        RCaseK      = rData.RCaseK;
        bDCmax      = rData.bDCmax;
        bDWmax      = rData.bDWmax;
        bTEmax      = rData.bTEmax;
        nConcurrent = rData.nConcurrent;
        nIsLocation = rData.nIsLocation;
        SpanKey     = rData.SpanKey ;
        nSpanPos    = rData.nSpanPos;
        dPhi_CF     = rData.dPhi_CF;
        dPhi_SF     = rData.dPhi_SF;
        dPhi_RE     = rData.dPhi_RE;
        drDC        = rData.drDC   ;
        drDW        = rData.drDW   ;
        drTE        = rData.drTE   ;
        drTG        = rData.drTG   ;
        drP         = rData.drP    ;
        drSE        = rData.drSE   ;
        drUS        = rData.drUS   ;
        drPR        = rData.drPR   ;
        drAD        = rData.drAD   ;   
        dVn_CA      = rData.dVn_CA ;
        dVu_DC      = rData.dVu_DC ;
        dVu_DW      = rData.dVu_DW ;
        dVu_TE      = rData.dVu_TE ;
        dVu_TG      = rData.dVu_TG ;
        dVu_P       = rData.dVu_P  ;
        dVu_SE      = rData.dVu_SE ;
        dVu_US      = rData.dVu_US ;
        dVu_PR      = rData.dVu_PR ;
        dVu_AD      = rData.dVu_AD ; 
        dVu_SUM     = rData.dVu_SUM;
        dVu_CAL     = rData.dVu_CAL;
        dMu_CAL     = rData.dMu_CAL;
        dNu_CAL     = rData.dNu_CAL;
        dRF         = rData.dRF;
        dRT         = rData.dRT;
        dRel_Locat  = rData.dRel_Locat;
        dKRF        = rData.dRF;
        dWeight     = 0.0;
}
};

// Rating Case
struct T_RTSC_CASE_845
{
    // 0=I-Posi, 1=J-Posi
    T_RTSC_BASE_845 RtscBase[2];

    void Initialize()
    {
        for(int i=0; i<2; i++) RtscBase[i].Initialize();
    }
};

struct T_RTSC_CASE
{
     // 0=I-Posi, 1=J-Posi
     T_RTSC_BASE RtscBase[2];
     
     void Initialize()
     {
         for(int i=0; i<2; i++)
             RtscBase[i].Initialize();
     }

     void Convert845(const T_RTSC_CASE_845 &rData)
     {
         for(int i=0; i<2; i++) RtscBase[i].Convert845(rData.RtscBase[i]);
     }
};

// Rating Factor(Smallest)
struct T_RTSC_D_845
{
    // 0=I-Posi, 1=J-Posi
    T_RTSC_BASE_845 RtscBase[2];

    void Initialize()
    {
        for(int i=0; i<2; i++) RtscBase[i].Initialize();
    }
};

struct T_RTSC_D
{
     // 0=I-Posi, 1=J-Posi
     T_RTSC_BASE RtscBase[2];

     void Initialize()
     {
         for(int i=0; i<2; i++)
             RtscBase[i].Initialize();
     }

     void Convert845(const T_RTSC_D_845 &rData)
     {
         for(int i=0; i<2; i++) RtscBase[i].Convert845(rData.RtscBase[i]);
     }
};

struct T_RTSC_UNIT
{
    T_RTSC_UNIT()
    {
        dVn_CA  = D_UNITSYS_BASE_FORCE;
        dVu_DC  = D_UNITSYS_BASE_FORCE;
        dVu_DW  = D_UNITSYS_BASE_FORCE;
        dVu_TE  = D_UNITSYS_BASE_FORCE;
        dVu_TG  = D_UNITSYS_BASE_FORCE;
        dVu_P   = D_UNITSYS_BASE_FORCE;
        dVu_SE  = D_UNITSYS_BASE_FORCE;
        dVu_US  = D_UNITSYS_BASE_FORCE;
        dVu_PR  = D_UNITSYS_BASE_FORCE;
        dVu_AD  = D_UNITSYS_BASE_FORCE;
        dVu_SUM = D_UNITSYS_BASE_FORCE;
        dVu_CAL = D_UNITSYS_BASE_FORCE;
        dMu_CAL = D_UNITSYS_BASE_MOMENT;
        dNu_CAL = D_UNITSYS_BASE_FORCE;

    }  
    int dVn_CA;
    int dVu_DC;
    int dVu_DW;
    int dVu_TE;
    int dVu_TG;
    int dVu_P; 
    int dVu_SE;
    int dVu_US;
    int dVu_PR;
    int dVu_AD;
    int dVu_SUM;
    int dVu_CAL;
    int dMu_CAL;
    int dNu_CAL;
};

// 비틀림강도
struct T_RTTC_BASE
{
    BOOL bChk;
	T_GRUP_K nGrupKey;	// Group Key
    UINT RCaseK;       // Rating Case Key            [Unit=None]
    BOOL bDCmax;       // DC(Max,Min Factor)                     [Unit=None]
    BOOL bDWmax;       // DW(Max,Min Factor)                     [Unit=None]
    BOOL bTEmax;       // Temperature(Max,Min Factor)            [Unit=None]
    UINT nConcurrent;  // Fx_max .... Mz_min(1~12)               [Unit=None] 
    double dPhi_CF;    // Condition Factor           [Unit=None]  
    double dPhi_SF;    // System Factor              [Unit=None]
    double dPhi_RE;    // Resistance Factor          [Unit=None]
    double drDC;       // DC Factor                  [Unit=None]
    double drDW;       // DW Factor                  [Unit=None]
    double drTE;       // Temperature Factor         [Unit=None]
    double drP;        // Permanent Factor           [Unit=None]
    double drSE;       // Secondary Factor           [Unit=None]
    double drUS;       // User Factor                [Unit=None]
    double drPR;       // Primary Vehicle Factor     [Unit=None]
    double drAD;       // Adjacent Vehicle Factor    [Unit=None]
    double dTn_CA;     // Shear Capacity           [Unit=Force]
    double dTu_DC;     // DC (Vu)                    [Unit=Moment]
    double dTu_DW;     // DW                         [Unit=Moment]
    double dTu_TE;     // Temperature                [Unit=Moment]
    double dTu_P;      // Permanet                   [Unit=Moment]
    double dTu_SE;     // Secondary                  [Unit=Moment]
    double dTu_US;     // User                       [Unit=Moment]
    double dTu_PR;     // Primary Vehicle            [Unit=Moment]
    double dTu_AD;     // Adjacent Vehicle           [Unit=Moment]
    double dTu_SUM;    // DC+DW+TE+SE+P+US+AD        [Unit=Moment]
    double dRF;        // Rating Factor              [Unit=None]
    double dRT;        // Safe Load Capacity(tons)   [Unit=None]

    void Initialize()
    {
        bChk = 0;
        RCaseK = 0;
        bDCmax = bDWmax = bTEmax = TRUE;
        nConcurrent = 1;
        dPhi_CF = dPhi_SF = dPhi_RE = 0.0;
        drDC = drDW = drTE = drP = drSE = drUS = drPR = drAD = 0.0;   
        dTn_CA = dTu_DC = dTu_DW = dTu_TE = dTu_P = 0.0;
        dTu_SE = dTu_US = dTu_PR = dTu_AD = 0.0; 
        dTu_SUM = 0.0;
        dRF = dRT = 0.0;
    }
};

// Rating Case
struct T_RTTC_CASE
{
     // 0=I-Posi, 1=J-Posi
     T_RTTC_BASE RttcBase[2];
     
     void Initialize()
     {
         for(int i=0; i<2; i++)
             RttcBase[i].Initialize();
     }
};

// Rating Factor(Smallest)
struct T_RTTC_D
{
     // 0=I-Posi, 1=J-Posi
     T_RTTC_BASE RttcBase[2];

     void Initialize()
     {
         for(int i=0; i<2; i++)
             RttcBase[i].Initialize();
     }
};

struct T_RTTC_UNIT
{
    T_RTTC_UNIT()
    {
        dTn_CA  = D_UNITSYS_BASE_MOMENT;
        dTu_DC  = D_UNITSYS_BASE_MOMENT;
        dTu_DW  = D_UNITSYS_BASE_MOMENT;
        dTu_TE  = D_UNITSYS_BASE_MOMENT;
        dTu_P   = D_UNITSYS_BASE_MOMENT;
        dTu_SE  = D_UNITSYS_BASE_MOMENT;
        dTu_US  = D_UNITSYS_BASE_MOMENT;
        dTu_PR  = D_UNITSYS_BASE_MOMENT;
        dTu_AD  = D_UNITSYS_BASE_MOMENT;
        dTu_SUM = D_UNITSYS_BASE_MOMENT;
    }  
    int dTn_CA;
    int dTu_DC;
    int dTu_DW;
    int dTu_TE;
    int dTu_P; 
    int dTu_SE;
    int dTu_US;
    int dTu_PR;
    int dTu_AD;
    int dTu_SUM;
};

// Rating KSCE-USD05
 // 허용응력 내하력.
struct T_RKST_BASE
{
    BOOL bCheck;
    T_ELEM_K ElemK;
    UINT nSectPosi;           // 1,2,3,4 단면의..           [Unit=None]
    UINT nRCaseK;             // Rating Case Key            [Unit=None]
    UINT nDgnRCaseK;          // Dgn Rating Case Key        [Unit=None]
    UINT nConcurrent;         // Fx_max .... Mz_min(1~12)   [Unit=None]   
    //double dfa_c;             // 콘크리트 허용응력,압축     [Unit=Stress]  
    //double dfa_t;             // 콘크리트 허용응력,인장     [Unit=Stress]  
    //double dfd;               // 고정하중에 의한 해당요소 상,하연에서의 응력 [Unit=Stress]
    //double dfl;               // 활하중에 의한 해당요소 상,하연에서의 응력   [Unit=Stress]
    //double dStressDL[4];      // 고정하중에 의한 응력 1,2,3,4 [Unit=Stress]
    //double dStressLL[4];      // 활하중에 의한 응력 1,2,3,4 [Unit=Stress]  
    double dRealDispZ;        // 실측처짐 
    double dImpFactor;        // 계산충격계수
    double dRealImpFactor;    // 실측충격계수  
    double dAddResponce;      // 추가보정계수
    double dKr;               // 교통상태 보정계수
    double dKt;               // 노면상태 보정계수
    double dCalc_defl;        // 계산처짐            [Unit=Length]
    //double dReal_revi_coeff;  // 실측보정계수        [Unit=None]
    double dDgnLoad;          // 설계하중, ex) DB-24 [Unit=None]
    //
    int iPosiResult;          // 1,2,3,4 단면의..(값은 0.1.2.3)
    double dfa_s[3];
    double dfa_p[4];
    double dfd_p[4];          // 고정하중에 의한 응력 1,2,3,4 [Unit=Stress]
    double dfdm_p[4];         // 고정하중에 의한 응력(수정) 1,2,3,4 [Unit=Stress]
    double dfl_p[4];          // 활하중에 의한 응력 1,2,3,4 [Unit=Stress]
    double dRF_p[4];
    double dP0_p[4];
    double dP0a_p[4];
    double dfd_TB[2];
    double dfl_TB[2];
    double dfa_TB[2];
    double dRF_TB[2];
    double dP0_TB[2];
    double dP0a_TB[2];
    double dKs;
    double dKsKrKt;
    //double dRF;               // Rating Factor       [Unit=None]
    double d1_plus_i_calc;    // 1+i_계산            [Unit=None]
    double di_rat;            // 충격계수비          [Unit=None]
    double dDisp_rat;         // 처짐비              [Unit=None]
    //double dBasic_Capacity;   // 기본내하력          [Unit=None]
    //double dPublic_Capacity;  // 공용내하력          [Unit=None]
    
    BOOL bOK_p[4];
    //BOOL bOK;

    void Initialize()
    {
        bCheck = FALSE;
        ElemK = 0;
        nSectPosi = 0;
        nRCaseK = 0;
        nDgnRCaseK = 0;
        nConcurrent = 0;
        dRealDispZ = 0.0;
        dImpFactor = 0.0;
        dRealImpFactor = 0.0;
        dAddResponce = 1.0;
        dKr = 0.0;              
        dKt = 0.0;              
        dCalc_defl = 0.0;       
        dDgnLoad = 0.0;         
        iPosiResult = 0;      
	for(int i=0; i<3; i++) {dfa_s[i]=0.0;}
        for(int i = 0; i<4; i++)
        {    
            dfa_p[i] = 0.0;
            dfd_p[i] = 0.0;
            dfdm_p[i] = 0.0;
            dfl_p[i] = 0.0;
            dRF_p[i] = 0.0;
            dP0_p[i] = 0.0;
            dP0a_p[i] = 0.0;
        }
        for(int i = 0; i<4; i++)
        {
            dfd_TB[i] = 0.0;
            dfl_TB[i] = 0.0;
            dfa_TB[i] = 0.0;
            dRF_TB[i] = 0.0;
            dP0_TB[i] = 0.0;
            dP0a_TB[i] = 0.0;
        }
        dKs = 0.0;
        dKsKrKt = 0.0;
        d1_plus_i_calc = 0.0;  
        di_rat = 0.0;          
        dDisp_rat = 0.0;       
    
        for(int i = 0; i<4; i++) bOK_p[i]=FALSE;
    }
};

struct T_RKST_UNIT
{
    T_RKST_UNIT()
    {
        dCalc_defl = D_UNITSYS_BASE_LENGTH;
        dRealDispZ = D_UNITSYS_BASE_LENGTH;
        for(int i =0; i<3; i++) dfa_s[i] = D_UNITSYS_BASE_STRESS;
        for(int i = 0; i<4; i++)
        {
            dfa_p[i] = D_UNITSYS_BASE_STRESS;
            dfd_p[i] = D_UNITSYS_BASE_STRESS;
            dfdm_p[i] = D_UNITSYS_BASE_STRESS;
            dfl_p[i] = D_UNITSYS_BASE_STRESS;
        }
        
    }  
    int dCalc_defl;
    int dRealDispZ;
    int dfa_s[3];
    int dfa_p[4];
    int dfd_p[4];
    int dfdm_p[4];
    int dfl_p[4];  
    
};

struct T_RKST_CASE
{
    T_RKST_BASE RkstBase[4]; // 0=I-상연, 1=I-하연, 2=J-상연, 3=J-하연  

    void Initialize()
    {
        for(int i=0; i<4; i++)
        {
            RkstBase[i].Initialize();      
        }
    }
};

struct T_RKST_D
{
    T_RKST_BASE BaseI[4]; // 0=상연-max(인장), 1=상연-min(압축), 2=하연-max(인장), 3=하연-min(압축)
    T_RKST_BASE BaseJ[4]; // 0=상연-max(인장), 1=상연-min(압축), 2=하연-max(인장), 3=하연-min(압축)

    void Initialize()
    {
        for(int i=0; i<4; i++)
        {
            BaseI[i].Initialize();
            BaseJ[i].Initialize();
        }
    }
};

// 강도설계법 
struct T_RKMC_BASE
{
    BOOL bCheck;
	BOOL bCheck2;						  // 결과 Diagram을 그릴 때 Rating Factor와 기본내하력은 이값이 TRUE일때 그림.. (그외의 경우는 bCheck가 false이면 값이 없다고 봄)
    T_ELEM_K ElemK;
    UINT nRCaseK;             // Rating Case Key           [Unit=None]
    UINT nDgnRCaseK;          // Dgn Rating Case Key        [Unit=None]
    UINT nConcurrent;         // Fx_max .... Mz_min(1~12)  [Unit=None]   
	CString strLcomName;      // LcomName + ype;
    double dPhiMn;            // 콘크리트 허용응력   [Unit=Moment]  
    double dMd;               // 고정하중에 의한 해당요소 상,하연에서의 응력 [Unit=Moment]
    double dM_L;              // 활하중에 의한 해당요소 상,하연에서의 응력   [Unit=Moment]
    double dRealDispZ;        // 실측처짐 
    double dCalc_defl;        // 계산처짐            [Unit=Length]
    double dImpFactor;        // 계산충격계수
    double dRealImpFactor;    // 실측충격계수  
    double dAddResponce;      // 추가 보정계수(KR)
    double dKm;               // 공용년수, 활하중 동시재하 확률, 교통량, 차선수 등의 보정계수  
    //double dReal_revi_coeff;  // 실측보정계수        [Unit=None]
    double dDgnLoad;          // 설계하중, ex) DB-24 [Unit=None]
    //
    double dMd_m;             // Md와 연관.
    double dP0;               // 기본내하력          [Unit=None]
    double dP0a;              // 공용내하력          [Unit=None]
    double dKs;               // 실측보정계수
    double dKsPerKm;
    double dRF;               // Rating Factor       [Unit=None]
    double d1_plus_i_calc;    // 1+i_계산            [Unit=None]
    double di_rat;            // 충격계수비          [Unit=None]
    double dDisp_rat;         // 처짐비              [Unit=None]
    //double dBasic_Capacity;   // 기본내하력          [Unit=None]
    //double dPublic_Capacity;  // 공용내하력          [Unit=None]
    double dFl;              // 변형률 계산 위한 활하중에 의한 인장응력[Unit=stress]
	double dEstiEwip;        // 실측변형률 [Unit=None]
    double dCalcEwip;        // 계산변형률 [Unit=None]
	double dEwip_rat;

    double dMdl;     //Md+Ml
    double dRatioMa ;//Mn/(Md+Ml)
    int    nRankType;//등급
	double dPhiMn_Rank;  // 등급 검토용 휨강도

    double dMd_str;           // 등급검토시,고정하중에 의한 해당요소 상,하연에서의 응력 [Unit=Moment]
    double dM_L_str;          // 등급검토시,활하중에 의한 해당요소 상,하연에서의 응력   [Unit=Moment]
    
	RATING_FORCE  Rating_Force;
    RATING_FORCE  Rating_Force_Str;
    BOOL bOK;

    void Initialize()
    {
        bCheck = FALSE;
		bCheck2 = FALSE;
        ElemK = 0;
        nRCaseK = 0;
        nDgnRCaseK = 0;
        nConcurrent = 0;
		strLcomName = "";
        dPhiMn = 0.0;
        dMd = 0.0;
        dM_L = 0.0;
        dRealDispZ = 0.0;
        dCalc_defl = 0.0;      
        dImpFactor = 0.0;
        dRealImpFactor = 0.0;
        dAddResponce = 1.0;
        dKm = 0.0;    
        dDgnLoad = 0.0;   
        dMd_m = 0.0;
        dP0 = 0.0;
        dP0a = 999.0;
        dKs = 0.0;
        dKsPerKm = 0.0;
        dRF = 0.0;             
        d1_plus_i_calc = 0.0;  
        di_rat = 0.0;          
        dDisp_rat = 0.0;       
        
		dFl = 0.0;
		dEstiEwip = 0.0; 
		dCalcEwip = 0.0; 
		dEwip_rat = 0.0;

		dMdl      = 0.0;
		dRatioMa  = 0.0;
		nRankType = 0;
		dPhiMn_Rank = 0.0;
        dMd_str  = 0.0;          
        dM_L_str = 0.0;          
		Rating_Force.Initialize();
        Rating_Force_Str.Initialize();
        bOK = FALSE;
    }
};

struct T_RKMC_UNIT
{
    T_RKMC_UNIT()
    {
        dPhiMn     = D_UNITSYS_BASE_MOMENT;
        dMd        = D_UNITSYS_BASE_MOMENT;
        dMd_m      = D_UNITSYS_BASE_MOMENT;
        dM_L       = D_UNITSYS_BASE_MOMENT;
        dCalc_defl = D_UNITSYS_BASE_LENGTH;
        dRealDispZ = D_UNITSYS_BASE_LENGTH;
		dMdl       = D_UNITSYS_BASE_MOMENT;
		dFl        = D_UNITSYS_BASE_STRESS;
		dPhiMn_Rank= D_UNITSYS_BASE_MOMENT;
        dMd_str    = D_UNITSYS_BASE_MOMENT;          
        dM_L_str   = D_UNITSYS_BASE_MOMENT;          
    }  
    int dPhiMn;
    int dMd;
    int dMd_m;
    int dM_L;
    int dCalc_defl;  
    int dRealDispZ;
	int dMdl;
	int dFl;
	int dPhiMn_Rank;
    int dMd_str;          
    int dM_L_str;          

};

struct T_RKMC_CASE
{
    T_RKMC_BASE RkmcBase[2]; 

    void Initialize()
    {
        for(int i=0; i<2; i++)
        {
            RkmcBase[i].Initialize();      
        }
    }
};

struct T_RKMC_D
{
    T_RKMC_BASE RkmcBase[4]; // 0=I-max(정), 1=I-min(부), 2=J-max(정), 3=J-min(부)  
    
	void Initialize()
    {
        for(int i=0; i<4; i++)
        {
            RkmcBase[i].Initialize();      
        }
    }
};

struct T_RKMC_BASE_CH
{
    BOOL bCheck;
	BOOL bCheck2;
    T_ELEM_K ElemK;
    UINT nRCaseK;             // Rating Case Key           [Unit=None]
    UINT nDgnRCaseK;          // Dgn Rating Case Key        [Unit=None]
    UINT nConcurrent;         // Fx_max .... Mz_min(1~12)  [Unit=None]   
    double dPhiMn;            // 콘크리트 허용응력   [Unit=Moment]  
    double dMd;               // 고정하중에 의한 해당요소 상,하연에서의 응력 [Unit=Moment]
    double dM_L;              // 활하중에 의한 해당요소 상,하연에서의 응력   [Unit=Moment]
    double dRealDispZ;        // 실측처짐 
    double dCalc_defl;        // 계산처짐            [Unit=Length]
    double dImpFactor;        // 계산충격계수
    double dRealImpFactor;    // 실측충격계수  
    double dAddResponce;      // 추가보정계수
    double dKm;               // 공용년수, 활하중 동시재하 확률, 교통량, 차선수 등의 보정계수  
    double dDgnLoad;          // 설계하중, ex) DB-24 [Unit=None]
    //
    double dMd_m;             // Md와 연관.
    double dP0;               // 기본내하력          [Unit=None]
    double dP0a;              // 공용내하력          [Unit=None]
    double dKs;               // 실측보정계수
    double dKsPerKm;
    double dRF;               // Rating Factor       [Unit=None]
    double d1_plus_i_calc;    // 1+i_계산            [Unit=None]
    double di_rat;            // 충격계수비          [Unit=None]
    double dDisp_rat;         // 처짐비              [Unit=None]
	
	double dEstiEwip;        // 실측변형률 [Unit=None]
    double dCalcEwip;        // 계산변형률 [Unit=None]
	double dEwip_rat;
	
	double dFl;
    double dMdl;     //Md+Ml
    double dRatioMa ;//Mn/(Md+Ml)
    int    nRankType;//등급
	double dPhiMn_Rank;
    BOOL bOK;
	
    char strLcomName[16];     // LcomName + ype;
	RATING_FORCE_CH  Rating_Force;
    RATING_FORCE_CH  Rating_Force_Str;
	
    void ConvertToString(T_RKMC_BASE& rData)
    {
		rData.bCheck         =  bCheck         ;
		rData.bCheck2        =  bCheck2;
		rData.ElemK          =  ElemK          ;
		rData.nRCaseK        =  nRCaseK        ;         
		rData.nDgnRCaseK     =  nDgnRCaseK     ;      
		rData.nConcurrent    =  nConcurrent    ;     
		rData.dPhiMn         =  dPhiMn         ;          
		rData.dMd            =  dMd            ;             
		rData.dM_L           =  dM_L           ;            
		rData.dRealDispZ     =  dRealDispZ     ;      
		rData.dCalc_defl     =  dCalc_defl     ;      
		rData.dImpFactor     =  dImpFactor     ;      
		rData.dRealImpFactor =  dRealImpFactor ;  
        rData.dAddResponce   =  dAddResponce;
		rData.dKm            =  dKm            ;             
		rData.dDgnLoad       =  dDgnLoad       ;        
		rData.dMd_m          =  dMd_m          ;           
		rData.dP0            =  dP0            ;             
		rData.dP0a           =  dP0a           ;            
		rData.dKs            =  dKs            ;             
		rData.dKsPerKm       =  dKsPerKm       ;
		rData.dRF            =  dRF            ;             
		rData.d1_plus_i_calc =  d1_plus_i_calc ;  
		rData.di_rat         =  di_rat         ;          
		rData.dDisp_rat      =  dDisp_rat      ;       
		rData.dEstiEwip      =  dEstiEwip      ;       
		rData.dCalcEwip      =  dCalcEwip      ;       
		rData.dEwip_rat      =  dEwip_rat      ;
		rData.dFl            =  dFl            ;
		rData.dMdl           =  dMdl           ;            
		rData.dRatioMa       =  dRatioMa       ;       
		rData.nRankType      =  nRankType      ;  
		rData.dPhiMn_Rank    =  dPhiMn_Rank;
		rData.bOK            =  bOK            ;
		   
		ConvertCharStr(strLcomName, rData.strLcomName, sizeof(strLcomName)); 
		Rating_Force.ConvertToString(rData.Rating_Force);
        Rating_Force_Str.ConvertToString(rData.Rating_Force_Str);
	
    }
    void ConvertToChar(T_RKMC_BASE& rData)
    {
		bCheck         =  rData.bCheck         ;
		bCheck2        =  rData.bCheck2        ;
		ElemK          =  rData.ElemK          ;
		nRCaseK        =  rData.nRCaseK        ;         
		nDgnRCaseK     =  rData.nDgnRCaseK     ;      
		nConcurrent    =  rData.nConcurrent    ;     
		dPhiMn         =  rData.dPhiMn         ;          
		dMd            =  rData.dMd            ;             
		dM_L           =  rData.dM_L           ;            
		dRealDispZ     =  rData.dRealDispZ     ;      
		dCalc_defl     =  rData.dCalc_defl     ;      
		dImpFactor     =  rData.dImpFactor     ;      
		dRealImpFactor =  rData.dRealImpFactor ;  
        dAddResponce   =  rData.dAddResponce;
		dKm            =  rData.dKm            ;             
		dDgnLoad       =  rData.dDgnLoad       ;        
		dMd_m          =  rData.dMd_m          ;           
		dP0            =  rData.dP0            ;             
		dP0a           =  rData.dP0a           ;            
		dKs            =  rData.dKs            ;             
		dKsPerKm       =  rData.dKsPerKm       ;
		dRF            =  rData.dRF            ;             
		d1_plus_i_calc =  rData.d1_plus_i_calc ;  
		di_rat         =  rData.di_rat         ;          
		dDisp_rat      =  rData.dDisp_rat      ;       
		dEstiEwip      =  rData.dEstiEwip      ;       
		dCalcEwip      =  rData.dCalcEwip      ;       
		dEwip_rat      =  rData.dEwip_rat      ;
		dFl            =  rData.dFl            ;
		dMdl           =  rData.dMdl           ;            
		dRatioMa       =  rData.dRatioMa       ;       
		nRankType      =  rData.nRankType      ;  
		dPhiMn_Rank    =  rData.dPhiMn_Rank    ;
		bOK            =  rData.bOK            ;
    
		ConvertStrChar(rData.strLcomName, strLcomName, sizeof(strLcomName)); 
		Rating_Force.ConvertToChar(rData.Rating_Force);
        Rating_Force_Str.ConvertToChar(rData.Rating_Force_Str);

	}

};


struct T_RKMC_CASE_CH
{
    T_RKMC_BASE_CH RkmcBase[2]; // 0=I, 1=J
	
	void ConvertToString(T_RKMC_CASE& rData)
    {
        for(int i=0; i<2; i++)
        {
            RkmcBase[i].ConvertToString(rData.RkmcBase[i]);   
        }
    }
	void ConvertToChar(T_RKMC_CASE& rData)
    {
        for(int i=0; i<2; i++)
        {
            RkmcBase[i].ConvertToChar(rData.RkmcBase[i]);   
        }
    }

};

///////////////////////////////////////////////////////////////
// Composite Plate Girder Design
///////////////////////////////////////////////////////////////

// Composite Plate Girder
struct T_SQPG_D
{
    UINT nElemNum; // 0=설계 요소, 
    UINT nServLcomNum;
    UINT nStreLcomNum;

    void Initialize()
    {
        nElemNum = 0;
        nServLcomNum = 0;
        nStreLcomNum = 0;
    }
};

// Plate Girder design. CHeck(검토항목당 하나이상이 설계 되었는지 여부)
#define D_CPG_CHECK_ITEM  8
struct T_PGCH_D
{
    BOOL bPgdCheck[D_CPG_CHECK_ITEM];
    
    void Initialize()
    {
        for(int i=0; i<D_CPG_CHECK_ITEM; i++)
            bPgdCheck[i] = FALSE;
    }
};

// Classification of cross-section
struct T_CLAS_BASE
{
    int iFlangeClass;
    int iTopFlgClass;
    int iBotFlgClass;
    int iWebClass;
    int iClassSect;
	double dEpsilon;
    //
    int iWebDeter;  // 0=None, 1=Top, 2=Bot  
    int iClass[5];
    double dPsi[5]; // Flange(0~3) 0=TL, 1=TR, 2=BR, 3=BL, 4=Web
    double dk_sig[5];
    double dRho[5];
    double dbeff[5];
    double dbe1[5];
    double dbe2[5];
    double dBTR[5];

    void Initialize()
    {    
        iFlangeClass = 0;
        iTopFlgClass = 0;
        iBotFlgClass = 0;
        iWebClass = 0;
        iClassSect = 0;
        iWebDeter = 0;
		dEpsilon = 0.0;
        for(int i=0; i<5; i++)
        {
            iClass[i] = 0;
            dPsi[i] = 0.0; 
            dk_sig[i] = 0.0;
            dRho[i] = 0.0;
            dbeff[i] = 0.0;
            dbe1[i] = 0.0;
            dbe2[i] = 0.0;
			dBTR[i]=0.0;
        }
    }
};

//IRC22 15 Section Properties
struct T_PG_IRC22_15_ELP
{

    double dArea      ;
    double dIyy     ;
    double dIzz     ;
    double dtop     ;
    double dbot     ;
    double dstop      ;
    double dsbot      ;
    double dzp      ;


    void Initialize()
    {
        dArea    = 0.0;
        dIyy   = 0.0; 
        dIzz   = 0.0; 
        dtop   = 0.0; 
        dbot   = 0.0; 
        dstop  = 0.0; 
        dsbot  = 0.0; 
        dzp    = 0.0;   


    }

    T_PG_IRC22_15_ELP() {Initialize(); }
    T_PG_IRC22_15_ELP(const T_PG_IRC22_15_ELP& rData) { *this = rData; } 
    T_PG_IRC22_15_ELP& operator=(const T_PG_IRC22_15_ELP& rData) 
    {
        dArea   = rData.dArea ;
        dIyy  = rData.dIyy  ;
        dIzz  = rData.dIzz  ;
        dtop  = rData.dtop  ;
        dbot  = rData.dbot  ;
        dstop = rData.dstop ;
        dsbot = rData.dsbot ;
        dzp   = rData.dzp   ;

        return *this;
    }
};

struct T_PG_IRC22_15_FATG_DESC_TOP
{
    CString strMaxLCB     ;
    CString strMinLCB     ;
    double y1              ;
    double z1             ;
    double dSigma1Max     ;
    double dSigma1Min     ;
    double dTau1Max     ;
    double dTau1Min      ;



    void Initialize()
    {
        strMaxLCB   = _T("");
        strMinLCB   = _T("");
        y1          = 0.0;
        z1          = 0.0;
        dSigma1Max  = 0.0;
        dSigma1Min  = 0.0;
        dTau1Max    = 0.0;
        dTau1Min    = 0.0;


    }

    T_PG_IRC22_15_FATG_DESC_TOP() {Initialize(); }
    T_PG_IRC22_15_FATG_DESC_TOP(const T_PG_IRC22_15_FATG_DESC_TOP& rData) { *this = rData; } 
    T_PG_IRC22_15_FATG_DESC_TOP& operator=(const T_PG_IRC22_15_FATG_DESC_TOP& rData) 
    {
     strMaxLCB  = rData.strMaxLCB   ;
     strMinLCB  = rData.strMinLCB   ;
     y1         = rData.y1          ;
     z1         = rData.z1          ;
     dSigma1Max = rData.dSigma1Max  ;
     dSigma1Min = rData.dSigma1Min  ;
     dTau1Max   = rData.dTau1Max    ;
     dTau1Min   = rData.dTau1Min    ;

        return *this;
    }
};

struct T_PG_IRC22_15_FATG_DESC_BOT
{
    CString strMaxLCB     ;
    CString strMinLCB     ;
    double y1              ;
    double z1             ;
    double dSigma2Max     ;
    double dSigma2Min     ;
    double dTau2Max     ;
    double dTau2Min      ;



    void Initialize()
    {
        strMaxLCB   = _T("");
        strMinLCB   = _T("");
        y1          = 0.0;
        z1          = 0.0;
        dSigma2Max  = 0.0;
        dSigma2Min  = 0.0;
        dTau2Max    = 0.0;
        dTau2Min    = 0.0;


    }

    T_PG_IRC22_15_FATG_DESC_BOT() {Initialize(); }
    T_PG_IRC22_15_FATG_DESC_BOT(const T_PG_IRC22_15_FATG_DESC_BOT& rData) { *this = rData; } 
    T_PG_IRC22_15_FATG_DESC_BOT& operator=(const T_PG_IRC22_15_FATG_DESC_BOT& rData) 
    {
        strMaxLCB  = rData.strMaxLCB   ;
        strMinLCB  = rData.strMinLCB   ;
        y1         = rData.y1          ;
        z1         = rData.z1          ;
        dSigma2Max = rData.dSigma2Max  ;
        dSigma2Min = rData.dSigma2Min  ;
        dTau2Max   = rData.dTau2Max    ;
        dTau2Min   = rData.dTau2Min    ;

        return *this;
    }
};

struct T_PG_IRC22_15_FATG_DFS
{
    T_PG_IRC22_15_FATG_DESC_TOP TopSide;
    T_PG_IRC22_15_FATG_DESC_BOT BotSide;

    void Initialize()
    {
        TopSide.Initialize();
        BotSide.Initialize();

    }

    T_PG_IRC22_15_FATG_DFS() {Initialize(); }
    T_PG_IRC22_15_FATG_DFS(const T_PG_IRC22_15_FATG_DFS& rData) { *this = rData; } 
    T_PG_IRC22_15_FATG_DFS& operator=(const T_PG_IRC22_15_FATG_DFS& rData) 
    {
        TopSide  = rData.TopSide ;
        BotSide  = rData.BotSide ;

        return *this;
    }
};

struct T_PG_IRC22_15_FATG_FATG
{
    
    T_PG_IRC22_15_FATG_DFS TopFlange;
    T_PG_IRC22_15_FATG_DFS BotFlange;
    T_PG_IRC22_15_FATG_DFS Web        ;
    //Correction Factor
    double dMewR           ;
    double dNscCorr            ;
    double df              ;          //Low Fatigue Chk
    double d27gammamft       ;
    double dNsc              ;
    double dNscLim            ;
    double dffn               ; // double duncorrected fatigue strength
    double dtfn               ;
    double dff                  ;
    double dtauf              ;
    double dfd                ;
    double dfdlim             ;
    double dtaufd             ;
    double dtaulim                ;
    CString strLCLcomb        ; // Stress Limitation
    double dSigmaEdSer          ;
    double dTauEdser          ;
    double dSigmaEdSerLim       ;
    double dTauEdserLim         ;
    double dTotSqrt             ;
    CString strLCConc           ;
    double dSigmac              ;
    double dk1                  ;
    CString strLimitRF           ;
    double dSigmaRF              ;
    double dk3                  ;
    double dk1fck               ;
    double dk3fyk               ;
    CString strLCDefl           ;
    double  dDeltaDL            ;
    double  dDeltaDLLimit            ;
    CString strLCBTop                   ;
    double dSigmaTop                    ;
    CString strLCBBot                   ;
    double dSigmaBot                    ;
    double dphi                           ;
    double dc                           ;
    double dEcmminusEcm                 ;
    double dSigmaS                      ;
    double dfcteff                        ;
    double dEcm                           ;
    double dEs                           ;
    double dRhopeff                         ;
    double dAs                            ;
    double dAceff                     ;
    double dX                         ;
    double dEta1                      ;
    double deta                           ;
    double dPhis                          ;
    double dPhip                    ;
    double dkt                            ;
    double dslimit                      ;
    double dsabar                   ;
    double dSrmax                     ;
    double dk1spacing              ;
    double dk2spacing                ;
    double dk3spacing              ;
    double dk4spacing                ;
    double dWk                 ;
    double dWkmax                ;
    int    nCrackClass            ;
    int    iDeflCtrlLCNum       ;

    void Initialize()
    {
        TopFlange.Initialize(); 
        BotFlange.Initialize(); 
        Web.Initialize();    

        strLCLcomb    = _T("");
        strLCConc     = _T("");
        strLimitRF    = _T("");                     
        strLCDefl      = _T("");    
        strLCBTop      = _T("");    
        strLCBBot      = _T("");

        dMewR         =    0.;
        dNscCorr          =    0.;
        df            =    0.;
        d27gammamft   =    0.;
        dNsc          =    0.;
        dNscLim       =    0.;
        dffn          =    0.;
        dtfn          =    0.;
        dff           =    0.;
        dtauf         =    0.;
        dfd           =    0.;
        dfdlim        =    0.;
        dtaufd        =    0.;
        dtaulim       =    0.;
        dSigmaEdSer    =   0.;
        dTauEdser      =   0.;
        dSigmaEdSerLim =   0.;
        dTauEdserLim   =   0.;
        dTotSqrt       =   0.;
        dSigmac         =   0.;
        dk1             =   0.;
        dSigmaRF        =   0.;
        dk3             =   0.;
        dDeltaDL        =   0.;
        dDeltaDLLimit   =   0.;
        dSigmaTop       =   0.;
        dSigmaBot       =   0.;
        dphi            =   0.;
        dc              =   0.;
        dEcmminusEcm    =   0.;
        dSigmaS         =   0.;
        dfcteff         =   0.;
        dEcm            =   0.;
        dEs             =   0.;
        dRhopeff        =   0.;
        dAs             =   0.;
        dAceff          =   0.;
        dX              =   0.;
        dEta1           =   0.;
        deta            =   0.;
        dPhis           =   0.;
        dPhip           =   0.;
        dkt             =   0.;
        dslimit         =   0.;
        dsabar          =   0.;
        dSrmax          =   0.;
        dk1spacing      =   0.;
        dk2spacing      =   0.;
        dk3spacing      =   0.;
        dk4spacing      =   0.;
        dWk             =   0.;
        dWkmax          =   0.;
        nCrackClass     =   0 ;
        dk1fck          =   0.;
        dk3fyk          =   0.;
        iDeflCtrlLCNum  =   0;

    } 

    T_PG_IRC22_15_FATG_FATG() {Initialize(); }
    T_PG_IRC22_15_FATG_FATG(const T_PG_IRC22_15_FATG_FATG& rData) { *this = rData; } 
    T_PG_IRC22_15_FATG_FATG& operator=(const T_PG_IRC22_15_FATG_FATG& rData) 
    {
        TopFlange      = rData.TopFlange;
            BotFlange    = rData.BotFlange;
            Web          = rData.Web;

        strLCLcomb    =    rData.strLCLcomb ;   
        strLCConc     =    rData.strLCConc  ;   
        strLimitRF    =    rData.strLimitRF ;           
        strLCDefl      =   rData.strLCDefl  ;   
        strLCBTop      =   rData.strLCBTop  ;   
        strLCBBot      =   rData.strLCBBot  ;   
        dMewR         =    rData.dMewR      ;
        dNscCorr          =    rData.dNscCorr       ;
        df            =    rData.df         ;
        d27gammamft   =    rData.d27gammamft;
        dNsc          =    rData.dNsc       ;
        dNscLim       =    rData.dNscLim    ;
        dffn          =    rData.dffn       ;
        dtfn          =    rData.dtfn       ;
        dff           =    rData.dff        ;
        dtauf         =    rData.dtauf      ;
        dfd           =    rData.dfd        ;
        dfdlim        =    rData.dfdlim     ;
        dtaufd        =    rData.dtaufd     ;
        dtaulim       =    rData.dtaulim    ;
        dSigmaEdSer    =   rData.dSigmaEdSer;
        dTauEdser      =   rData.dTauEdser       ;
        dSigmaEdSerLim =   rData.dSigmaEdSerLim  ;
        dTauEdserLim   =   rData.dTauEdserLim    ;
        dTotSqrt       =   rData.dTotSqrt        ;
        dSigmac         =   rData.dSigmac       ; 
        dk1             =   rData.dk1           ; 
        dSigmaRF        =   rData.dSigmaRF      ; 
        dk3             =   rData.dk3           ; 
        dDeltaDL        =   rData.dDeltaDL      ; 
        dDeltaDLLimit   =   rData.dDeltaDLLimit ; 
        dSigmaTop       =   rData.dSigmaTop     ; 
        dSigmaBot       =   rData.dSigmaBot     ; 
        dphi            =   rData.dphi          ; 
        dc              =   rData.dc            ; 
        dEcmminusEcm    =   rData.dEcmminusEcm  ; 
        dSigmaS         =   rData.dSigmaS       ; 
        dfcteff         =   rData.dfcteff       ; 
        dEcm            =   rData.dEcm          ; 
        dEs             =   rData.dEs          ; 
        dRhopeff        =   rData.dRhopeff      ; 
        dAs             =   rData.dAs           ; 
        dAceff          =   rData.dAceff        ; 
        dX              =   rData.dX            ; 
        dEta1           =   rData.dEta1         ; 
        deta            =   rData.deta          ; 
        dPhis           =   rData.dPhis         ; 
        dPhip           =   rData.dPhip         ; 
        dkt             =   rData.dkt           ; 
        dslimit         =   rData.dslimit       ; 
        dsabar          =   rData.dsabar        ; 
        dSrmax          =   rData.dSrmax        ; 
        dk1spacing      =   rData.dk1spacing    ; 
        dk2spacing      =   rData.dk2spacing    ; 
        dk3spacing      =   rData.dk3spacing    ; 
        dk4spacing      =   rData.dk4spacing    ; 
        dWk             =   rData.dWk           ; 
        dWkmax          =   rData.dWkmax        ; 
        nCrackClass     =   rData.nCrackClass   ;
        dk1fck          =   rData.dk1fck        ;
        dk3fyk          =   rData.dk3fyk        ;
        iDeflCtrlLCNum  =   rData.iDeflCtrlLCNum;

        return *this;
    }
};



struct T_PG_IRC22_15_LOTRSHEAR
{
    CString strLcomName ;
    CString strLcombType;
    CString strVedDlLcombMax;
    CString strVedLlLcombMax;

    double dVedDL       ;
    double dVedLL       ;
    double dSLProv      ;
    double dSLProvLim     ;
    double dhs          ;
    double dVled        ;
    double dSl1req      ;
    double dSl1prov      ;
    double dn           ;
    double dQu          ;
    double dH1          ;
    double dH2          ;
    double dAsl         ;
    double dAecSlab         ;
    double dAecSteel         ;
    double dSl2req      ;
    double dH          ;
    double dL          ;
    double dVR          ;
    double dVr          ;
    double dSrreq         ;
    double dQr           ;
    double dSLreq         ;

    double dVlcase1         ;
    double dVlcase2         ;
    double dVlL           ;
    double dVlAst            ;
    double dVl25         ;

    void Initialize()
    {
        strLcomName  = _T("");
        strLcombType = _T("");
        strVedDlLcombMax = _T("");
        strVedLlLcombMax = _T("");


        dVedDL     = 0.;
        dVedLL     = 0.;
        dSLProv    = 0.;
        dSLProvLim = 0.;
        dhs        = 0.;
        dVled      = 0.;
        dSl1req    = 0.;
        dSl1prov   = 0.;
        dn         = 0.;
        dQu        = 0.;
        dH1        = 0.;
        dH2        = 0.;
        dAsl       = 0.;
        dAecSlab   = 0.;
        dAecSteel  = 0.;
        dSl2req    = 0.;
        dH         = 0.;
        dL         = 0.;
        dVR        = 0.;
        dVr        = 0.;
        dSrreq     = 0.;
        dQr        = 0.;
        dSLreq     = 0.;                 
        dVlcase1   = 0.;
        dVlcase2   = 0.;
        dVlL       = 0.;
        dVlAst     = 0.;
        dVl25      = 0.;    
                             
    }

    T_PG_IRC22_15_LOTRSHEAR() {Initialize(); }
    T_PG_IRC22_15_LOTRSHEAR(const T_PG_IRC22_15_LOTRSHEAR& rData) { *this = rData; } 
    T_PG_IRC22_15_LOTRSHEAR& operator=(const T_PG_IRC22_15_LOTRSHEAR& rData) 
    {
        strLcomName  = rData.strLcomName  ;
        strLcombType = rData.strLcombType ;
        strVedDlLcombMax = rData.strVedDlLcombMax;
        strVedLlLcombMax = rData.strVedLlLcombMax;

        dVedDL     =  rData. dVedDL     ;
        dVedLL     =  rData. dVedLL     ;
        dSLProv    =  rData. dSLProv    ;
        dSLProvLim =  rData. dSLProvLim ;
        dhs        =  rData. dhs        ;
        dVled      =  rData. dVled      ;
        dSl1req    =  rData. dSl1req    ;
        dSl1prov   =  rData. dSl1prov   ;
        dn         =  rData. dn         ;
        dQu        =  rData. dQu        ;
        dH1        =  rData. dH1        ;
        dH2        =  rData. dH2        ;
        dAsl       =  rData. dAsl       ;
        dAecSlab   =  rData.dAecSlab    ;
        dAecSteel  =  rData.dAecSteel   ;
        dSl2req    =  rData. dSl2req    ;
        dH         =  rData. dH         ;
        dL         =  rData. dL         ;
        dVR        =  rData. dVR        ;
        dVr        =  rData. dVr        ;
        dSrreq     =  rData. dSrreq     ;
        dQr        =  rData. dQr        ;
        dSLreq     =  rData. dSLreq     ;
        dVlcase1   =  rData. dVlcase1   ;
        dVlcase2   =  rData. dVlcase2   ;
        dVlL       =  rData. dVlL       ;
        dVlAst     =  rData. dVlAst     ;
        dVl25      =  rData. dVl25      ;


        return *this;
    }
};


struct T_PG_IRC22_15_TRAWEBSTIFF
{
    //Minimum Dimension CHk
    //Intermediate Stiffener
    double      dMinH            ;
    double      dMinHeff         ;
    double      d20tqe           ;
    double      d14tqe           ;
    int         iStiffType       ; // one side or two side
    double      dIys             ;
    double      d075dtw3         ;
    int         iCbyDcond        ;

    double      dFqd             ;//Buckling Chk
    double      dFqdLim          ;
    double      dhdashw          ;
    double      dAeff            ;
    double      dIysBuck             ;
    double      drys             ;
    double      dKL              ;
    double      dAlpha           ;
    double      dFcc             ;
    double      dLambda          ;
    double      dPhi             ;
    double      dX               ;
    double      dfcd             ;
    
    CString     strLcombName    ;//Bearing Stiffener
    double      dReaction       ;
    double      dFcdw           ;
    double      db1             ;
    double      dn1             ;
    double      dA              ;
    double      dBearingIys     ;
    double      dBearingrys      ;
    double      dK              ;
    double      dBearingfcc     ;
    
    double      dCrushing_n2     ;//Local web crushing
    double      dCrushing_A      ;
    double      dFcdwCrushing     ;
    
    double      dFpsd           ;  //Bearing Chk
    double      dBearing_n       ;
    double      dAq             ;
    
    double      dBuck_fcd        ;//Buckling Chk
    double      dBuck_hdasheff   ;
    double      dBuck_hdashw    ;
    double      dBuck_Aeff      ;
    double      dBuck_Iys       ;
    double      dBuck_rys       ;
    double      dBuck_KL        ;
    double      dBuck_alpha     ;
    double      dBuck_fcc       ;
    double      dBuck_lambda    ;
    double      dBuck_phi       ;
    double      dBuck_X         ;
    double      dBuck_fcd_Lim        ;

    int         iTransvNos;       // One Stiffener or Two Stiffener
    int         iBearingStiffNos; // Pair fo stifener 1,2,3,4,5
    int         iStiffCaseNo;
    void Initialize()
    {

        dMinH            = 0. ;
            dMinHeff       = 0. ;
            d20tqe         = 0. ;
            d14tqe         = 0. ;
            iStiffType     = 0 ;
            dIys           = 0. ;
            d075dtw3       = 0. ;  
            iCbyDcond       = 0 ;
            dFqd           = 0. ;
            dFqdLim        = 0. ;
            dhdashw        = 0. ;
            dAeff          = 0. ;
            dIysBuck           = 0. ;
            drys           = 0. ;
            dKL            = 0. ;
            dAlpha         = 0. ;
            dFcc           = 0. ;
            dLambda        = 0. ;
            dPhi           = 0. ;
            dX             = 0. ;
            dfcd           = 0. ;                     
            strLcombName   = _T("") ;
            dReaction      = 0. ;
            dFcdw          = 0. ;
            db1            = 0. ;
            dn1            = 0. ;
            dA             = 0. ;
            dBearingIys    = 0. ;
            dBearingrys    = 0. ;
            dK             = 0. ;
            dBearingfcc    = 0. ;                    
            dCrushing_n2   = 0. ;
            dCrushing_A    = 0. ;   
            dFcdwCrushing  = 0. ;
            dFpsd          = 0. ;
            dBearing_n     = 0. ;
            dAq            = 0. ;                    
            dBuck_fcd      = 0. ;
            dBuck_hdasheff = 0. ;
            dBuck_hdashw   = 0. ;
            dBuck_Aeff     = 0. ;
            dBuck_Iys      = 0. ;
            dBuck_rys      = 0. ;
            dBuck_KL       = 0. ;
            dBuck_alpha    = 0. ;
            dBuck_fcc      = 0. ;
            dBuck_lambda   = 0. ;
            dBuck_phi      = 0. ;
            dBuck_X        = 0. ;  
            dBuck_fcd_Lim  = 0. ;
            iTransvNos     = 0  ;     
            iBearingStiffNos= 0 ;
            iStiffCaseNo     =0;
    }

    T_PG_IRC22_15_TRAWEBSTIFF() {Initialize(); }
    T_PG_IRC22_15_TRAWEBSTIFF(const T_PG_IRC22_15_TRAWEBSTIFF& rData) { *this = rData; } 
    T_PG_IRC22_15_TRAWEBSTIFF& operator=(const T_PG_IRC22_15_TRAWEBSTIFF& rData) 
    {

            dMinH          = rData.dMinH           ;
            dMinHeff       = rData.dMinHeff        ;
            d20tqe         = rData.d20tqe          ;
            d14tqe         = rData.d14tqe          ;
            iStiffType     = rData.iStiffType      ;
            dIys           = rData.dIys            ;
            d075dtw3       = rData.d075dtw3        ;
            iCbyDcond      =  rData.iCbyDcond       ;
            dFqd           = rData.dFqd            ;
            dFqdLim        = rData.dFqdLim         ;
            dhdashw        = rData.dhdashw         ;
            dAeff          = rData.dAeff           ;
            dIysBuck           = rData.dIysBuck            ;
            drys           = rData.drys            ;
            dKL            = rData.dKL             ;
            dAlpha         = rData.dAlpha          ;
            dFcc           = rData.dFcc            ;
            dLambda        = rData.dLambda         ;
            dPhi           = rData.dPhi            ;
            dX             = rData.dX              ;
            dfcd           = rData.dfcd            ;
            strLcombName   = rData.strLcombName    ;
            dReaction      = rData.dReaction       ;
            dFcdw          = rData.dFcdw           ;
            db1            = rData.db1             ;
            dn1            = rData.dn1             ;
            dA             = rData.dA              ;
            dBearingIys    = rData.dBearingIys     ;
            dBearingrys    = rData.dBearingrys     ;
            dK             = rData.dK              ;
            dBearingfcc    = rData.dBearingfcc     ;
            dCrushing_n2   = rData.dCrushing_n2    ;
            dCrushing_A    = rData.dCrushing_A     ;
            dFcdwCrushing  = rData.dFcdwCrushing    ;
            dFpsd          = rData.dFpsd           ;
            dBearing_n     = rData.dBearing_n      ;
            dAq            = rData.dAq             ;
            dBuck_fcd      = rData.dBuck_fcd       ;
            dBuck_hdasheff = rData.dBuck_hdasheff  ;
            dBuck_hdashw   = rData.dBuck_hdashw    ;
            dBuck_Aeff     = rData.dBuck_Aeff      ;
            dBuck_Iys      = rData.dBuck_Iys       ;
            dBuck_rys      = rData.dBuck_rys        ;
            dBuck_KL       = rData.dBuck_KL        ;
            dBuck_alpha    = rData.dBuck_alpha     ;
            dBuck_fcc      = rData.dBuck_fcc       ;
            dBuck_lambda   = rData.dBuck_lambda    ;
            dBuck_phi      = rData.dBuck_phi       ;
            dBuck_X        = rData.dBuck_X         ;
            dBuck_fcd_Lim  = rData.dBuck_fcd_Lim    ;
            iTransvNos     =  rData.iTransvNos  ;     
            iBearingStiffNos= rData.iBearingStiffNos;
            iStiffCaseNo    = rData.iStiffCaseNo;
        return *this;
    }
};


struct T_PG_IRC22_15_SP
{
    double dGamma_cBS;
    double dGamma_c_acci;
    double dGamma_M0YB;
    double dGamma_M1Ult;
    double dGamma_s;
    double dGamma_v;
    double dGamma_fft;
    double dGamma_Mft;
    //Section Properties
    double dBc;
    double dtc;
    double dHh;
    double ddc;
    double dfck;
    double dEc;
    double dfyk;
    //Effective Width of Concrete Slab
    int iEffWidthCase;
    double dEffWidth;
    //RF Details
    double dTopDia;
    double dTopSpaceing;
    double dTopNos;
    double dTopLayers;
    double dTopCover;
    double dTopTotArea;
    double dBotDia;
    double dBotSpaceing;
    double dBotNos;
    double dBotLayers;
    double dBotCover;
    double dBotTotArea;
    //Design Strength
    double dEta;
    double dLambda;
    double dfcds_BS;
    double dfcds_Acci;
    double dfyds_steel;
    //Girder Prop
    double dbbf;
    double dtbf;
    double dhw;
    double dbtf;
    double dttf;
    double dtw;
    CString sTFMat;
    double  dTFthick;
    double  dTFfy;
    double  dTFfu;
    CString sWMat;
    double  dWthick;
    double  dWfy;
    double  dWfu;
    CString sBFMat;
    double  dBFthick;
    double  dBFfy;
    double  dBFfu;
    //Design Strength
    double dfyd;
    double dEs;
    double dGs;
    //Trst Prop
    double dTRSTWebType;
    double dTRSTWebFy;
    double dTRSTWebH;
    double dTRSTWebB;
    double dTRSTWebtw;
    double dTRSTWebtf;
    double dTRSTWebc;

    double dTRSTBearingType;
    double dTRSTBearingFy;
    double dTRSTBearingH;
    double dTRSTBearingB;
    double dTRSTBearingtw;
    double dTRSTBearingtf;
    double dTRSTBearingc;

    //Min Plate thk
    int iType;
    double dthk;

    //Shear Lag Effect
    int    iSLCase;
    double dSLbot;
    double dSLbob;
    double dSLlo;
    double dOutstandCase1;
    double dOutstandCase2;
    //Elastic Section Properties
    T_PG_IRC22_15_ELP SteelSect        ;
    T_PG_IRC22_15_ELP ShortTermCompo  ;
    T_PG_IRC22_15_ELP LongTermCompo   ;
    T_PG_IRC22_15_ELP Cracked         ;

    void Initialize()
    {
        dGamma_cBS       = 0.0;
        dGamma_c_acci    = 0.0;
        dGamma_M0YB      = 0.0;
        dGamma_M1Ult     = 0.0;
        dGamma_s         = 0.0;
        dGamma_v         = 0.0;
        dGamma_fft       = 0.0;
        dGamma_Mft       = 0.0;
        //Section Properties
        dBc               = 0.0;
        dtc               = 0.0;
        dHh               = 0.0;
        ddc               = 0.0;
        dfck              = 0.0;
        dEc               = 0.0;
        dfyk              = 0.0;
        //Effective Width of Concrete Slab
        iEffWidthCase    = 0;
        dEffWidth        = 0.0;
        //RF Details
        dTopDia           = 0.0;
        dTopSpaceing      = 0.0;
        dTopNos           = 0.0;
        dTopLayers        = 0.0;
        dTopCover         = 0.0;
        dTopTotArea       = 0.0;
        dBotDia           = 0.0;
        dBotSpaceing      = 0.0;
        dBotNos           = 0.0;
        dBotLayers        = 0.0;
        dBotCover         = 0.0;
        dBotTotArea       = 0.0;
        //Design Strength
        dEta             = 0.0;
        dLambda          = 0.0;
        dfcds_BS         = 0.0;
        dfcds_Acci       = 0.0;
        dfyds_steel      = 0.0;
        //Girder Prop
        dbbf              = 0.0;
        dtbf              = 0.0;
        dhw               = 0.0;
        dbtf              = 0.0;
        dttf              = 0.0;
        dtw               = 0.0;
        sTFMat           = "";
        dTFthick         = 0.0;
        dTFfy            = 0.0;
        dTFfu            = 0.0;
        sWMat             = "";
        dWthick           = 0.0;
        dWfy              = 0.0;
        dWfu              = 0.0;
        sBFMat            = "";
        dBFthick          = 0.0;
        dBFfy             = 0.0;
        dBFfu             = 0.0;
        //Design Strength
        dfyd             = 0.0;
        dEs              = 0.0;
        dGs              = 0.0;
        //Trst Prop
        dTRSTWebType      = 0.0;
        dTRSTWebFy        = 0.0;
        dTRSTWebH         = 0.0;
        dTRSTWebB         = 0.0;
        dTRSTWebtw        = 0.0;
        dTRSTWebtf        = 0.0;
        dTRSTWebc         = 0.0;

        dTRSTBearingType  = 0.0;
        dTRSTBearingFy    = 0.0;
        dTRSTBearingH     = 0.0;
        dTRSTBearingB     = 0.0;
        dTRSTBearingtw    = 0.0;
        dTRSTBearingtf    = 0.0;
        dTRSTBearingc     = 0.0;

        //Min Plate thk
        iType            = 0;
        dthk             = 0.0;

        //Shear Lag Effect
        iSLCase           = 0;
        dSLbot            = 0.0;
        dSLbob            = 0.0;
        dSLlo             = 0.0;
        dOutstandCase1    = 0.0;
        dOutstandCase2    = 0.0;
        SteelSect.Initialize();    
        ShortTermCompo .Initialize(); 
        LongTermCompo .Initialize();  
        Cracked .Initialize();        
    }

    T_PG_IRC22_15_SP() {Initialize(); }
    T_PG_IRC22_15_SP(const T_PG_IRC22_15_SP& rData) { *this = rData; } 
    T_PG_IRC22_15_SP& operator=(const T_PG_IRC22_15_SP& rData) 
    {

        dGamma_cBS               = rData.  dGamma_cBS     ;  
        dGamma_c_acci          = rData.  dGamma_c_acci  ;  
        dGamma_M0YB            = rData.  dGamma_M0YB    ;  
        dGamma_M1Ult           = rData.  dGamma_M1Ult   ;  
        dGamma_s               = rData.  dGamma_s       ;  
        dGamma_v               = rData.  dGamma_v       ;  
        dGamma_fft             = rData.  dGamma_fft     ;  
        dGamma_Mft             = rData.  dGamma_Mft     ;  
        //Section Properti     = rData. //Section Properti
        dBc                    = rData. dBc  ;             
        dtc                    = rData. dtc  ;             
        dHh                    = rData. dHh  ;             
        ddc                    = rData. ddc  ;             
        dfck                   = rData. dfck ;             
        dEc                    = rData. dEc  ;             
        dfyk                   = rData. dfyk ;             
        //Effective Width      = rData. //Effective Width 
        iEffWidthCase          = rData.  iEffWidthCase;   
        dEffWidth              = rData.  dEffWidth    ;   
        //RF Details           = rData. //RF Details  ;
        dTopDia                = rData. dTopDia       ;   
        dTopSpaceing           = rData. dTopSpaceing  ;   
        dTopNos                = rData. dTopNos       ;   
        dTopLayers             = rData. dTopLayers    ;   
        dTopCover              = rData. dTopCover     ;   
        dTopTotArea            = rData. dTopTotArea   ;   
        dBotDia                = rData. dBotDia       ;   
        dBotSpaceing           = rData. dBotSpaceing  ;   
        dBotNos                = rData. dBotNos       ;   
        dBotLayers             = rData. dBotLayers    ;   
        dBotCover              = rData. dBotCover     ;   
        dBotTotArea            = rData. dBotTotArea   ;   
        //Design Strength      = rData. //Design Strength
        dEta                   = rData.  dEta       ;     
        dLambda                = rData.  dLambda    ;     
        dfcds_BS               = rData.  dfcds_BS   ;     
        dfcds_Acci             = rData.  dfcds_Acci ;     
        dfyds_steel            = rData.  dfyds_steel;     
        //Girder Prop          = rData. //Girder Prop
        dbbf                   = rData. dbbf   ;           
        dtbf                   = rData. dtbf   ;           
        dhw                    = rData. dhw    ;           
        dbtf                   = rData. dbtf        ;      
        dttf                   = rData. dttf        ;      
        dtw                    = rData. dtw         ;      
        sTFMat                 = rData.  sTFMat     ;      
        dTFthick               = rData.  dTFthick   ;      
        dTFfy                  = rData.  dTFfy      ;      
        dTFfu                  = rData.  dTFfu      ;      
        sWMat                  = rData. sWMat       ;      
        dWthick                = rData. dWthick     ;      
        dWfy                   = rData. dWfy        ;      
        dWfu                   = rData. dWfu        ;      
        sBFMat                 = rData. sBFMat      ;      
        dBFthick               = rData. dBFthick    ;      
        dBFfy                  = rData. dBFfy       ;      
        dBFfu                  = rData. dBFfu       ;      
        //Design Strength      = rData. //Design Strength
        dfyd                   = rData.  dfyd ;           
        dEs                    = rData.  dEs  ;           
        dGs                    = rData.  dGs  ;           
        //Trst Prop            = rData. //Trst Prop
        dTRSTWebType           = rData. dTRSTWebType ;     
        dTRSTWebFy             = rData. dTRSTWebFy   ;     
        dTRSTWebH              = rData. dTRSTWebH    ;     
        dTRSTWebB              = rData. dTRSTWebB    ;     
        dTRSTWebtw             = rData. dTRSTWebtw   ;     
        dTRSTWebtf             = rData. dTRSTWebtf   ;     
        dTRSTWebc              = rData. dTRSTWebc    ;     

        dTRSTBearingType       = rData. dTRSTBearingType  ;
        dTRSTBearingFy         = rData. dTRSTBearingFy    ;
        dTRSTBearingH          = rData. dTRSTBearingH     ;
        dTRSTBearingB          = rData. dTRSTBearingB     ;
        dTRSTBearingtw         = rData. dTRSTBearingtw    ;
        dTRSTBearingtf         = rData. dTRSTBearingtf    ;
        dTRSTBearingc          = rData. dTRSTBearingc     ;

        //Min Plate thk        = rData. //Min Plate thk
        iType                  = rData.  iType ;           
        dthk                   = rData.  dthk ;            

        //Shear Lag Effect     = rData. //Shear Lag Effect
        iSLCase                = rData. iSLCase         ;  
        dSLbot                 = rData. dSLbot          ;  
        dSLbob                 = rData. dSLbob          ;  
        dSLlo                  = rData. dSLlo           ;  
        dOutstandCase1         = rData. dOutstandCase1  ;  
        dOutstandCase2         = rData. dOutstandCase2  ;  

        SteelSect = rData.SteelSect;    
        ShortTermCompo = rData .ShortTermCompo;
        LongTermCompo = rData.LongTermCompo;
        Cracked =rData.Cracked;    

        return *this;
    }
};

struct T_PGBR_IRC_BASE
{
    BOOL bChk;
    T_ELEM_K ElemK;      // Element             [Unit=None]
    UINT nPosi;          // Position 1=I, 2=J   [Unit=None]
    UINT LcomK;          // LcomK               [Unit=None]
    UINT OrgLcomK;       // LcomK               [Unit=None]
    int iMax;            //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
    //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)

    T_PG_IRC22_15_SP SectProp;
    T_PG_IRC22_15_TRAWEBSTIFF TraWebStiff;
    T_PG_IRC22_15_LOTRSHEAR LotrShear;
    T_PG_IRC22_15_FATG_FATG Fatg;

    void Initialize()
    {
        bChk = FALSE;
        ElemK = 0;
        nPosi = 0;
        LcomK = 0;
        OrgLcomK = 0;;
        SectProp.Initialize();
        TraWebStiff.Initialize();
        LotrShear.Initialize();
        Fatg.Initialize();
    }
    T_PGBR_IRC_BASE() { Initialize(); }

};

struct T_PGBR_IRC_LCOM
{
    // 0=I-End, 1=J-End, 
    T_PGBR_IRC_BASE BeforePgbr[2];  
    T_PGBR_IRC_BASE PgbrBase[2];

    void Initialize()
    {
        for(int i=0; i<2; i++)
        {
            BeforePgbr[i].Initialize();
            PgbrBase[i].Initialize();
        }
    }
    T_PGBR_IRC_LCOM() { Initialize(); }
};

// Plate Girder Bending Resistance
struct T_PGBR_BASE
{
    BOOL bChk;
    T_ELEM_K ElemK;      // Element             [Unit=None]
    UINT nPosi;          // Position 1=I, 2=J   [Unit=None]
    UINT LcomK;          // LcomK               [Unit=None]
    UINT OrgLcomK;       // LcomK               [Unit=None]
    int iMax;            //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                             //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iTopClass;       //                     [Unit=None] 
    int iBotClass;       //                     [Unit=None]
    int iWebClass;       //                     [Unit=None]
    int iSectClass;      //                     [Unit=None]
    BOOL bEquiClass2;    //                     [Unit=None]
    double dPlasticNA;   //                     [Unit=Length]
    double dPlasticNAEff;//                     [Unit=Length]
    double dEffeLen;     //                     [Unit=Length]
    double dHoleLen;     //                     [Unit=Length]
    double dElasticNA;   //                     [Unit=Length]
    double dElasticNAEff;//                     [Unit=Length]
    double dFx;          // Fx                  [Unit=Force]
    double dMy;          // My                  [Unit=Moment]
    double dMz;          // Mz                  [Unit=Moment]
    double dNc;          // Nc                  [Unit=Force]
    double dNc_el;       // Nc,el               [Unit=Force]
    double dNc_f;        // Nc,f                [Unit=Force]
    double dMa_Ed;       // Ma,Ed               [Unit=Moment]
    double dMc_Ed;       // Ma,Ed               [Unit=Moment]
    double dMpl_Rd;      // Mpl,Rd              [Unit=Moment]
    double dMel_Rd;      // Mel,Rd              [Unit=Moment]
    double dM_Rd;        // M_Rd                [Unit=Moment]
    double dRatio;       // Ratio               [Unit=None]
	T_SCFP_D ScfpForce;
	T_PSCD_FORCE ForceConst;
	BOOL bOK;
	// For shear
    double dMf_Rd;       // Mf,Rd               [Unit=Moment]
    double dMpl_Rd_s;    // Mpl,Rd              [Unit=Moment]  
    // For Detail
    double dh, dXpl;     // h, Xpl,             [Unit=Length]
    double dXplOverh;    // Xpl/h               [Unit=None]
    double dAlpha;       // Alpha               [Unit=None]
    double dNa_top,  dNa_bot, dNa_web;  //      [Unit=Force]
    double dNa_topc, dNa_topt;          //      [Unit=Force]
    double dNa_webc, dNa_webt;          //      [Unit=Force]
    double dNa_botc, dNa_bott;          //      [Unit=Force]
    double dNa_webc1, dNa_webc2;        //      [Unit=Force]
    double dNa_web_Effe;                //      [Unit=Force]
    double dDistT, dDistB, dDistW;      //      [Unit=Length]
    double dDistTc, dDistTt;            //      [Unit=Length]
    double dDistWc, dDistWt;            //      [Unit=Length]
    double dDistBc, dDistBt;            //      [Unit=Length]
    double dDistWc1, dDistWc2;          //      [Unit=Length]
    double dDistW1, dDistW2;            //      [Unit=Length]
    double dCompfsdAsidsi, dTensfsdAsidsi; //   [Unit=Moment]  
    double dBeta;  // [Unit=None] 
     // elastic
    double dStrStlTop, dStrComTop, dStrStlBot, dStrComBot;  // [Unit=Stress]
    double dSIGa_top, dSIGa_bot;  // [Unit=Stress]
    double dk_min;  // [Unit=None] 
	//IRC Report
	double dTopClassRat;       //                     [Unit=None] 
	double dBotClassRat;       //                     [Unit=None]
	double dWebClassRat;       //                     [Unit=None]
	BOOL   bTrSt;							// Transverse Stiffener Provided = TRUE else FALSE
	double dTRSTPitch;				//Pitch distance of transverse stiffeneer
    double dBeffFactor;
	//For minimum Web Thickness
	int nCondStage1,nCondStage2;
	BOOL bCondStage1,bCondStage2;

    void Initialize()
    {
        bChk = FALSE;
        ElemK = 0;
        nPosi = 0;
        LcomK = 0;
        OrgLcomK = 0;
        iMax = 0;	
        iTopClass = 0;
        iBotClass = 0;
        iWebClass = 0;
        iSectClass = 0;
        bEquiClass2 = FALSE;
        dPlasticNA = 0.0;
        dPlasticNAEff = 0.0;
        dEffeLen = 0.0;     
        dHoleLen = 0.0;     
        dElasticNA = 0.0;   
        dElasticNAEff = 0.0;
        dFx = dMy = dMz = 0.0;
        dNc = 0.0;
        dNc_el = 0.0;
        dNc_f = 0.0;
        dMa_Ed = 0.0;
        dMc_Ed = 0.0;
        dMpl_Rd = 0.0;
        dMel_Rd = 0.0;
        dM_Rd = 0.0;
        dRatio = 0.0;
		ScfpForce.Initialize();
		ForceConst.Initialize();
		bOK = FALSE;
		//
        dMf_Rd = 0.0;
        dMpl_Rd_s = 0.0;
        // 
        dh=dXpl=0.0;
        dXplOverh=0.0;
        dAlpha=0.0;
        dNa_top = dNa_bot = dNa_web = 0.0;
        dNa_topc = dNa_topt = 0.0;
        dNa_webc = dNa_webt = 0.0;
        dNa_botc = dNa_bott = 0.0;
        dNa_webc1 = dNa_webc2 = 0.0;
        dNa_web_Effe = 0.0;
        dDistT = dDistB = dDistW = 0.0;
        dDistTc = dDistTt = 0.0;
        dDistWc = dDistWt = 0.0;            
        dDistBc = dDistBt = 0.0;
        dDistWc1 = dDistWc2 = 0.0;
        dDistW1 = dDistW2 = 0.0;
        dCompfsdAsidsi = dTensfsdAsidsi = 0.0;
        dBeta = 0.0;      
        dStrStlTop = dStrComTop = dStrStlBot = dStrComBot = 0.0;  
        dSIGa_top = dSIGa_bot = 0.0;  
        dk_min = 0.0; 

		dTopClassRat	=0.0;
		dBotClassRat=0.0;
		dWebClassRat=0.0;
		bTrSt = FALSE;
		dTRSTPitch=dBeffFactor=0.0;
		nCondStage1=nCondStage2=0;
		bCondStage1=bCondStage2 = FALSE;
    }
    T_PGBR_BASE() { Initialize(); }
};


struct T_PGBR_LCOM
{
    // 0=I-End, 1=J-End, 
    T_PGBR_BASE BeforePgbr[2];  
    T_PGBR_BASE PgbrBase[2];

    void Initialize()
    {
        for(int i=0; i<2; i++)
        {
            BeforePgbr[i].Initialize();
            PgbrBase[i].Initialize();
        }
    }
    T_PGBR_LCOM() { Initialize(); }
};

struct T_PGBR_D
{
     // 0=I-Positive, 1=I-Negative, 2=J-Positive, 3=J-Negative
     T_PGBR_BASE PgbrBase[4];
     
     void Initialize()
     {
         for(int i=0; i<4; i++)
         {
             PgbrBase[i].Initialize();
         }
     }
     T_PGBR_D() { Initialize(); }
};

struct T_PGBR_IRC_D
{
    // 0=I-Positive, 1=I-Negative, 2=J-Positive, 3=J-Negative
    T_PGBR_IRC_BASE PgbrBase[4];

    void Initialize()
    {
        for(int i=0; i<4; i++)
        {
            PgbrBase[i].Initialize();
        }
    }
    T_PGBR_IRC_D() { Initialize(); }
};

struct T_PGBR_UNIT
{
    T_PGBR_UNIT()
    {
        dPlasticNA = D_UNITSYS_BASE_LENGTH;
        dPlasticNAEff = D_UNITSYS_BASE_LENGTH;
        dEffeLen = D_UNITSYS_BASE_LENGTH;     
        dHoleLen = D_UNITSYS_BASE_LENGTH;     
        dElasticNA = D_UNITSYS_BASE_LENGTH;   
        dElasticNAEff = D_UNITSYS_BASE_LENGTH;
        dFx = D_UNITSYS_BASE_FORCE;
        dMy = D_UNITSYS_BASE_MOMENT;
        dMz = D_UNITSYS_BASE_MOMENT;
        dNc = D_UNITSYS_BASE_FORCE;
        dNc_el = D_UNITSYS_BASE_FORCE;
        dNc_f = D_UNITSYS_BASE_FORCE;
        dMa_Ed = D_UNITSYS_BASE_MOMENT;
        dMc_Ed = D_UNITSYS_BASE_MOMENT;
        dMpl_Rd = D_UNITSYS_BASE_MOMENT;
        dMel_Rd = D_UNITSYS_BASE_MOMENT;
        dM_Rd = D_UNITSYS_BASE_MOMENT;    
        //
        dMf_Rd = D_UNITSYS_BASE_MOMENT;
        dMpl_Rd_s = D_UNITSYS_BASE_MOMENT;
        //
        dh = dXpl = D_UNITSYS_BASE_LENGTH;
        dNa_top = dNa_bot = dNa_web = D_UNITSYS_BASE_FORCE;
        dNa_topc = dNa_topt = D_UNITSYS_BASE_FORCE;
        dNa_webc = dNa_webt = D_UNITSYS_BASE_FORCE;
        dNa_botc = dNa_bott = D_UNITSYS_BASE_FORCE;
        dNa_webc1 = dNa_webc2 = D_UNITSYS_BASE_FORCE;
        dNa_web_Effe = D_UNITSYS_BASE_FORCE;
        dDistT = dDistB = dDistW = D_UNITSYS_BASE_LENGTH;
        dDistTc = dDistTt = D_UNITSYS_BASE_LENGTH;
        dDistWc = dDistWt = D_UNITSYS_BASE_LENGTH;            
        dDistBc = dDistBt = D_UNITSYS_BASE_LENGTH;
        dDistWc1 = dDistWc2 = D_UNITSYS_BASE_LENGTH;
        dDistW1 = dDistW2 = D_UNITSYS_BASE_LENGTH;
        dCompfsdAsidsi = dTensfsdAsidsi = D_UNITSYS_BASE_MOMENT;
        dStrStlTop = dStrComTop = dStrStlBot = dStrComBot = D_UNITSYS_BASE_STRESS;  
        dSIGa_top = dSIGa_bot = D_UNITSYS_BASE_STRESS;  

        dA_eff  = D_UNITSYS_BASE_AREA;
        dIy_eff = D_UNITSYS_BASE_STIF; 
        dIz_eff = D_UNITSYS_BASE_STIF; 

    }  
    int dPlasticNA;
    int dPlasticNAEff;
    int dEffeLen;     
    int dHoleLen;     
    int dElasticNA;   
    int dElasticNAEff;
    int dFx;
    int dMy;
    int dMz;
    int dNc;
    int dNc_el;
    int dNc_f;  
    int dMa_Ed;
    int dMc_Ed;
    int dMpl_Rd;
    int dMel_Rd;
    int dM_Rd;
    int dMf_Rd;
    int dMpl_Rd_s;
    //
    int dh, dXpl;    
    int dNa_top,  dNa_bot, dNa_web;
    int dNa_topc, dNa_topt;        
    int dNa_webc, dNa_webt;        
    int dNa_botc, dNa_bott;        
    int dNa_webc1, dNa_webc2;      
    int dNa_web_Effe;              
    int dDistT, dDistB, dDistW;    
    int dDistTc, dDistTt;          
    int dDistWc, dDistWt;          
    int dDistBc, dDistBt;          
    int dDistWc1, dDistWc2;        
    int dDistW1, dDistW2;          
    int dCompfsdAsidsi, dTensfsdAsidsi;      
    int dStrStlTop, dStrComTop, dStrStlBot, dStrComBot;
    int dSIGa_top, dSIGa_bot;

    int dA_eff;
    int dIy_eff; 
    int dIz_eff; 

};

// Plate Girder Bending Resistance - Rho_c
struct T_DGNLINE_LBD_BR
{
    // T_DGNLINE_LBD 중 필요한 것만..
    int iClass;
    double dxi;
    double dRho_c; // Interaction between plate and column buckling

    void Initialize()
    {
        iClass = 0;
        dxi  = 0.0;
        dRho_c= 0.0; 
    }

    T_DGNLINE_LBD_BR() {Initialize(); }
    T_DGNLINE_LBD_BR(const T_DGNLINE_LBD_BR& rData) { *this = rData; } 
    T_DGNLINE_LBD_BR& operator=(const T_DGNLINE_LBD_BR& rData) 
    {
        iClass = rData.iClass;
        dxi    = rData.dxi;
        dRho_c = rData.dRho_c;

        return *this;
    }
};

struct T_RHOC_BASE
{
    BOOL bChk;
    T_ELEM_K ElemK;      // Element             [Unit=None]
    UINT nPosi;          // Position 1=I, 2=J   [Unit=None]
    UINT LcomK;          // LcomK               [Unit=None]
    UINT OrgLcomK;       // LcomK               [Unit=None]
    int iMax;            //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)

    double dAa_eff_Fx; 
    double dIya_eff_Fx; 
    double dIza_eff_Fx; 
    double dAc_eff_Fx; 
    double dIyc_eff_Fx; 
    double dIzc_eff_Fx; 

    double dAa_eff_My; 
    double dIya_eff_My; 
    double dIza_eff_My; 
    double dAc_eff_My; 
    double dIyc_eff_My; 
    double dIzc_eff_My; 

    double dRho_c;  // BF의 Rho_c
    T_DGNLINE_LBD_BR RhocBeforeFx; // [NONE]
    T_DGNLINE_LBD_BR RhocAfterFx;
    T_DGNLINE_LBD_BR RhocBeforeMy;
    T_DGNLINE_LBD_BR RhocAfterMy;

    void Initialize()
    {
        bChk = FALSE;
        ElemK = 0;
        nPosi = 0;
        LcomK = 0;
        OrgLcomK = 0;
        iMax = 0;	

        dAa_eff_Fx  = 0.0; 
        dIya_eff_Fx = 0.0; 
        dIza_eff_Fx = 0.0; 
        dAc_eff_Fx  = 0.0; 
        dIyc_eff_Fx = 0.0; 
        dIzc_eff_Fx = 0.0; 

        dAa_eff_My  = 0.0; 
        dIya_eff_My = 0.0; 
        dIza_eff_My = 0.0; 
        dAc_eff_My  = 0.0; 
        dIyc_eff_My = 0.0; 
        dIzc_eff_My = 0.0; 

        dRho_c = 0.0;
        RhocBeforeFx.Initialize();
        RhocAfterFx.Initialize();
        RhocBeforeMy.Initialize();
        RhocAfterMy.Initialize();
    }

    T_RHOC_BASE() { Initialize(); }

};


struct T_RHOC_LCOM
{
    // 0=I-End, 1=J-End, 
    T_RHOC_BASE Rho_C[2];

    void Initialize()
    {
        for(int i=0; i<2; i++)
        {
            Rho_C[i].Initialize();
        }
    }
    T_RHOC_LCOM() { Initialize(); }

};

struct T_RHOC_D
{
    // 0=I-Positive, 1=I-Negative, 2=J-Positive, 3=J-Negative
    T_RHOC_BASE Rho_C[4];

    void Initialize()
    {
        for(int i=0; i<4; i++)
        {
            Rho_C[i].Initialize();
        }
    }
    T_RHOC_D() { Initialize(); }
};

struct T_RHOC_UNIT
{
    T_RHOC_UNIT()
    {
        dA_eff  = D_UNITSYS_BASE_AREA;
        dIy_eff = D_UNITSYS_BASE_STIF; 
        dIz_eff = D_UNITSYS_BASE_STIF; 

    }  
    int dA_eff;
    int dIy_eff; 
    int dIz_eff; 

};

// Plate girder, Resistance to Vertical Shear

struct T_PRVS_BASE_UNIT // For Left & Right Web's Detail
{
	double dVpl_Rd;      // Vpl,Rd              [Unit=Force]
	double dVbw_Rd;      // Vbw,Rd              [Unit=Force]
	double dVbf_Rd;      // Vbf,Rd              [Unit=Force]

	// For Detail
	double dIsl_org;   //  [Unit=Length^4]
	double dIsl;       //  [Unit=Length^4]  
	double dXw;        //  [Unit=None]
	double dMax_Bar_Lamda_w;  //  [Unit=None]
	double dc;
	double dbf;          //  [Unit=Length]
	double dtf;          //  [Unit=Length]
	double dVb_Rd_org;   //  [Unit=Force]
	double dVb_Rd_Lim;   //  [Unit=Force]

	T_PRVS_BASE_UNIT() { Initialize(); }

	void Initialize()
	{
		dVpl_Rd				=	0.0;
		dVbw_Rd				=	0.0;
		dVbf_Rd				=	0.0;
		dIsl_org			=	0.0;
		dIsl				=	0.0;    
		dXw					=	0.0;     
		dMax_Bar_Lamda_w	=	0.0;
		dc					=	0.0;
		dbf					=	0.0;       
		dtf					=	0.0;       
		dVb_Rd_org			=	0.0;
		dVb_Rd_Lim			=	0.0;
	}

	T_PRVS_BASE_UNIT& operator =(const T_PRVS_BASE_UNIT& rD)
	{
		dVpl_Rd				=	rD.dVpl_Rd			;
		dVbw_Rd				=	rD.dVbw_Rd			;
		dVbf_Rd				=	rD.dVbf_Rd			;
		dIsl_org			=	rD.dIsl_org			;
		dIsl				=	rD.dIsl				;    
		dXw					=	rD.dXw				;     
		dMax_Bar_Lamda_w	=	rD.dMax_Bar_Lamda_w;
		dc					=	rD.dc				;
		dbf					=	rD.dbf				;       
		dtf					=	rD.dtf				;       
		dVb_Rd_org			=	rD.dVb_Rd_org		;
		dVb_Rd_Lim			=	rD.dVb_Rd_Lim		;

		return *this;
	}
};

struct T_PRVS_BASE
{
    BOOL bChk;
    T_ELEM_K ElemK;      // Element             [Unit=None]
    UINT nPosi;          // Position 1=I, 2=J   [Unit=None]
    UINT LcomK;          // LcomK               [Unit=None]
    UINT OrgLcomK;       // LcomK               [Unit=None]
    int iMax;
    
    int iTopClass;       //                     [Unit=None] 
    int iBotClass;       //                     [Unit=None]
    int iWebClass;       //                     [Unit=None]
    int iSectClass;      //                     [Unit=None]
    double dFx;          // Fx                  [Unit=Force]
    double dFz;          // Fz                  [Unit=Force]
    double dMy;          // My                  [Unit=Moment]
    double dMz;          // Mz                  [Unit=Moment]
    double dN_Ed;        // N_Ed                [Unit=Force]
    double dV_Ed;        // V_Ed                [Unit=Force]
    double dM_Ed;        // M_Ed                [Unit=Moment]  
    double dMa_Ed;       // Ma,Ed               [Unit=Moment]
    double dMpl_Rd;      // Mpl,Rd              [Unit=Moment]
    double dMel_Rd;      // Mel,Rd              [Unit=Moment]
    double dM_Rd;        // M_Rd                [Unit=Moment]
    double dVpl_Rd;      // Vpl,Rd              [Unit=Force]
    double dVbw_Rd;      // Vbw,Rd              [Unit=Force]
    double dVbf_Rd;      // Vbf,Rd              [Unit=Force]
    double dVb_Rd;       // Vb,Rd               [Unit=Force]
    double dV_Rd;        // V_Rd                [Unit=Force]
    double dEta1;        // Eta1                [Unit=None]
	double dEta1_lim;    //                     [Unit=None]
    double dEta3;        // Eta3                [Unit=None]
    double dCombinedEffe;//                     [Unit=None]
    BOOL bCombinedEffe;  //                     [Unit=None]
    BOOL bEta3Exceed05;  //                     [Unit=None]
    double dRatio;       // Ratio               [Unit=None]
	T_SCFP_D ScfpForce;
	T_PSCD_FORCE ForceConst;
	BOOL bOK;
    
    // For Detail
    double dAv;        // Av  [Unit=Area]
    double deta;       //  [Unit=None]
    double dIsl_org;   //  [Unit=Length^4]
    double dIsl;       //  [Unit=Length^4]  
    double dXw;        //  [Unit=None]
    double dMax_Bar_Lamda_w;  //  [Unit=None]
    double dc;
    double dMf_Rd;       //  [Unit=Moment]
    double dbf;          //  [Unit=Length]
    double dtf;          //  [Unit=Length]
    double dVb_Rd_org;   //  [Unit=Force]
    double dVb_Rd_Lim;   //  [Unit=Force]
    BOOL bEta1Ratio;      //  

	//For IRC REport
		double m_dMfRd,m_dHighShearCond1,m_dHighDhearCond2,m_dBetaFactHighShear;

    T_PRVS_BASE_UNIT DetailResD[2]; // 0 : Left Web,  1 : Right Web

    void Initialize()
    {
        bChk = FALSE;
        ElemK = 0;
        nPosi = 0;
        LcomK = 0;
        OrgLcomK = 0;
        iMax = 0;	
        iTopClass = 0;     
        iBotClass = 0;    
        iWebClass = 0;    
        iSectClass = 0;   
        dFx = 0.0;          
        dFz = 0.0;          
        dMy = 0.0;          
        dMz = 0.0;      
        dN_Ed = 0.0;
        dV_Ed = 0.0;
        dM_Ed = 0.0;
        dMa_Ed = 0.0;       
        dMpl_Rd = 0.0;      
        dMel_Rd = 0.0;      
        dM_Rd = 0.0;
        dVpl_Rd = 0.0;
        dVbw_Rd = 0.0;
        dVbf_Rd = 0.0;    
        dVb_Rd = 0.0; 
        dV_Rd = 0.0;        
	    dEta1 = 0.0;
	    dEta1_lim = 0.0;
	    dEta3 = 0.0;
	    dCombinedEffe = 0.0;
	    bCombinedEffe= FALSE;
	    bEta3Exceed05= FALSE;
        dRatio = 0.0; 
		ScfpForce.Initialize();
		ForceConst.Initialize();
        bOK = FALSE;
        //
        dAv = 0.0;        
        deta = 0.0;       
        dIsl_org = 0.0;   
        dIsl = 0.0;         
        dXw = 0.0;        
        dMax_Bar_Lamda_w = 0.0;
        dc = 0.0;
        dMf_Rd = 0.0;       //  [Unit=Moment]
        dbf = 0.0;          //  [Unit=Length]
        dtf = 0.0;          //  [Unit=Length]
        dVb_Rd_org = 0.0;   //  [Unit=Force]
        dVb_Rd_Lim = 0.0;   //  [Unit=Force]
        bEta1Ratio = false;      //  
		m_dMfRd=m_dHighShearCond1=m_dHighDhearCond2=m_dBetaFactHighShear = 0.0;
	    DetailResD[0].Initialize();
	    DetailResD[1].Initialize();
    }

    T_PRVS_BASE() { Initialize(); }

    T_PRVS_BASE& operator=(const T_PRVS_BASE& rD)
    {
	  bChk			= rD.bChk			;
	  ElemK			= rD.ElemK			;
	  nPosi			= rD.nPosi			;
	  LcomK			= rD.LcomK			;
	  OrgLcomK		= rD.OrgLcomK		;
	  iMax			= rD.iMax			;
	  iTopClass		= rD.iTopClass		; 
	  iBotClass		= rD.iBotClass		;
	  iWebClass		= rD.iWebClass		;
	  iSectClass	= rD.iSectClass	;
	  dFx			= rD.dFx			;        
	  dFz			= rD.dFz			;        
	  dMy			= rD.dMy			;        
	  dMz			= rD.dMz			;    
	  dN_Ed			= rD.dN_Ed			;
	  dV_Ed			= rD.dV_Ed			;
	  dM_Ed			= rD.dM_Ed			;
	  dMa_Ed		= rD.dMa_Ed		;     
	  dMpl_Rd		= rD.dMpl_Rd		;    
	  dMel_Rd		= rD.dMel_Rd		;    
	  dM_Rd			= rD.dM_Rd			;
	  dVpl_Rd		= rD.dVpl_Rd		;
	  dVbw_Rd		= rD.dVbw_Rd		;
	  dVbf_Rd		= rD.dVbf_Rd		;  
	  dVb_Rd		= rD.dVb_Rd		;
	  dV_Rd			= rD.dV_Rd			;      
	  dEta1			= rD.dEta1			;
	  dEta1_lim		= rD.dEta1_lim		;
	  dEta3			= rD.dEta3			;
	  dCombinedEffe = rD.dCombinedEffe ;
	  bCombinedEffe	= rD.bCombinedEffe	;
	  bEta3Exceed05	= rD.bEta3Exceed05	;
	  dRatio		= rD.dRatio		;
	  ScfpForce     = rD.ScfpForce;
	  ForceConst    = rD.ForceConst;
	  bOK			= rD.bOK			;
	  //
	  dAv			= rD.dAv			;        
	  deta			= rD.deta			;       
	  dIsl_org		= rD.dIsl_org		;   
	  dIsl			= rD.dIsl			;         
	  dXw			= rD.dXw			;        
	  dMax_Bar_Lamda_w = rD.dMax_Bar_Lamda_w;
	  dc			= rD.dc		;
	  dMf_Rd		= rD.dMf_Rd	;       //  [Unit=Moment]
	  dbf			= rD.dbf		;          //  [Unit=Length]
	  dtf			= rD.dtf		;          //  [Unit=Length]
	  dVb_Rd_org	= rD.dVb_Rd_org;   //  [Unit=Force]
	  dVb_Rd_Lim	= rD.dVb_Rd_Lim;   //  [Unit=Force]
	  bEta1Ratio	= rD.bEta1Ratio;      //  

		m_dMfRd							=		 rD.m_dMfRd							;
		m_dHighShearCond1		=		 rD.m_dHighShearCond1		;
		m_dHighDhearCond2			=	 rD.m_dHighDhearCond2		;
		m_dBetaFactHighShear = 	 rD.m_dBetaFactHighShear;

	  DetailResD[0] = rD.DetailResD[0];
	  DetailResD[1] = rD.DetailResD[1];

	  return *this;
    }
};

struct T_PRVS_D
{
    // 0=I-Max, 1=J-Max
    T_PRVS_BASE BeforePrvs[2];
    T_PRVS_BASE PrvsBase[2];

    void Initialize()
    {
        for(int i=0; i<2; i++)
        {
            BeforePrvs[i].Initialize();
            PrvsBase[i].Initialize();
        }
    }
    T_PRVS_D() { Initialize(); }
};

struct T_PRVS_UNIT
{
    T_PRVS_UNIT()
    {
        dFx = D_UNITSYS_BASE_FORCE;          
        dFz = D_UNITSYS_BASE_FORCE;          
        dMy = D_UNITSYS_BASE_MOMENT;          
        dMz = D_UNITSYS_BASE_MOMENT;    
        dN_Ed = D_UNITSYS_BASE_FORCE;
        dV_Ed = D_UNITSYS_BASE_FORCE;
        dM_Ed = D_UNITSYS_BASE_MOMENT;
        dMa_Ed = D_UNITSYS_BASE_MOMENT;       
        dMpl_Rd = D_UNITSYS_BASE_MOMENT;      
        dMel_Rd = D_UNITSYS_BASE_MOMENT;      
        dM_Rd = D_UNITSYS_BASE_MOMENT;
        dVpl_Rd = D_UNITSYS_BASE_FORCE;
        dVbw_Rd = D_UNITSYS_BASE_FORCE;
        dVbf_Rd = D_UNITSYS_BASE_FORCE;    
        dVb_Rd  = D_UNITSYS_BASE_FORCE;   
        dV_Rd   = D_UNITSYS_BASE_FORCE;
        dAv = D_UNITSYS_BASE_AREA;     
        dIsl_org = D_UNITSYS_BASE_STIF;
        dIsl = D_UNITSYS_BASE_STIF;    
        dc = D_UNITSYS_BASE_LENGTH;    
        dMf_Rd = D_UNITSYS_BASE_MOMENT;
        dbf = D_UNITSYS_BASE_LENGTH;   
        dtf = D_UNITSYS_BASE_LENGTH;    
        dVb_Rd_org = D_UNITSYS_BASE_FORCE;   //  
        dVb_Rd_Lim = D_UNITSYS_BASE_FORCE;   //  
    }    
    int dFx;          
    int dFz;          
    int dMy;          
    int dMz;    
    int dN_Ed;
    int dV_Ed;
    int dM_Ed;
    int dMa_Ed;       
    int dMpl_Rd;      
    int dMel_Rd;      
    int dM_Rd;
    int dVpl_Rd;
    int dVbw_Rd;
    int dVbf_Rd;    
    int dVb_Rd; 
    int dV_Rd;        
    int dAv;        
    int dIsl_org;
    int dIsl;    
    int dc;
    int dMf_Rd;       
    int dbf;          
    int dtf;          
    int dVb_Rd_org;   
    int dVb_Rd_Lim;   
};


// Plate girder, resistance to Lateral-Torsional Buckling
struct T_PLTB_BASE
{
    BOOL bChk;
    T_ELEM_K ElemK;       // Element             [Unit=None]
    UINT nPosi;           // Position 1=I, 2=J   [Unit=None]
    UINT LcomK;           // LcomK               [Unit=None]
    UINT OrgLcomK;        // LcomK               [Unit=None]
    int iMax;
    int iSectClass;       // class               [Unit=None]
    double dFx;           // Fx                  [Unit=Force]
    double dMy;           // My                  [Unit=Moment]
    double dMz;           // Mz                  [Unit=Moment]
    double dV1;           // V1                  [Unit=Force]
    double dV2;           // V2                  [Unit=Force]
    double dM1;           // M1                  [Unit=Moment]
    double dM2;           // M2                  [Unit=Moment]
    double dC4;           // C4                  [Unit=None]
    double dN_Ed;         // N_Ed                [Unit=Force]
    double dM_Ed;         // M_Ed                [Unit=Moment]  
    double dMa_Ed;        // Ma,Ed               [Unit=Moment]
    double dMpl_Rd;       // Mpl,Rd              [Unit=Moment]
    double dMel_Rd;       // Mel,Rd              [Unit=Moment]
    double dNb_Rd;        // Nb,Rd               [Unit=Force]
    double dMb_Rd;        // Mb_Rd               [Unit=Moment]
    double dMcr;          // M_cr                [Unit=Moment]
    double dInterRat;     // N_Ed/Nb,Rd+M_Ed/Mb,Rd<=1.0  [Unit=None]  
    double dRatio;       // Ratio               [Unit=None]
    BOOL bOK;
    //
    double dM_Rd;         // M_Rd               [Unit=Moment]
    double dcc;           // Cd/l               [Unit=Stress]
    double dgamma;        //                    [Unit=None]
    double dMu;           // V1/V2 or V1/V2     [Unit=None]
    double dPHI;          //                    [Unit=None]
    double dm1,dm2;       //                    [Unit=None]
    double dm_min, dm;    //                    [Unit=None]
    double dAwc;          //                    [Unit=Area]
    double dAf;           //                    [Unit=Area]
    double dLamda_LT;     //                    [Unit=None]
    double dAlpha_LT;     //                    [Unit=None]
    double dPHI_LT;       //                    [Unit=None]
    double dX_LT_org;     //                    [Unit=None]
    double dX_LT;         //                    [Unit=None]
    double dX_LT_N;       //                    [Unit=None]
    double dk1, dk2, dks; //                    [Unit=None]
    double dI_at;         //                    [Unit=L^4]
    double dI_afz;        //                    [Unit=L^4]
    double dkc;

    void Initialize()
    {
        bChk = FALSE;
        ElemK = 0;
        nPosi = 0;
        LcomK = 0;
        OrgLcomK = 0;
        iMax = 0;	
        iSectClass = 0;      
        dFx = 0.0;          
        dMy = 0.0;          
        dMz = 0.0;      
        dV1 = 0.0;           
        dV2 = 0.0;           
        dM1 = 0.0;           
        dM2 = 0.0;   
        dC4 = 0.0;
        dN_Ed = 0.0;                      
        dM_Ed = 0.0;           
        dMa_Ed = 0.0;        
        dMpl_Rd = 0.0;       
        dMel_Rd = 0.0;   
        dNb_Rd = 0.0;
        dMb_Rd = 0.0;
        dMcr = 0.0;
        dInterRat = 0.0;
        dRatio = 0.0;
        bOK = FALSE;
        //
        dM_Rd = 0.0;        
        dcc = 0.0;          
        dgamma = 0.0;
        dMu = 0.0;          
        dPHI = 0.0;         
        dm1 = dm2 = 0.0;    
        dm_min = dm = 0.0;  
        dAwc = dAf = 0.0;   
        dLamda_LT = 0.0;    
        dAlpha_LT = 0.0;    
        dPHI_LT = 0.0;      
        dX_LT_org = 0.0;    
        dX_LT = 0.0;    
        dX_LT_N = 0.0;
        dk1 = dk2 = dks = 0.0; 
        dI_at = 0.0;         
        dI_afz = 0.0;        
        dkc = 0.0;
    }
    T_PLTB_BASE() { Initialize(); }
};

struct T_PLTB_D
{  
    T_PLTB_BASE BeforePltb[2];
    T_PLTB_BASE PltbBase[2];

    void Initialize()
    {
        for(int i=0; i<2; i++)
        {
            BeforePltb[i].Initialize();
            PltbBase[i].Initialize();
        }
    }
    T_PLTB_D() { Initialize(); }
};

struct T_PLTB_UNIT
{
    T_PLTB_UNIT()
    {
        dFx = D_UNITSYS_BASE_FORCE;           
        dMy = D_UNITSYS_BASE_MOMENT;           
        dMz = D_UNITSYS_BASE_MOMENT;           
        dV1 = D_UNITSYS_BASE_FORCE;           
        dV2 = D_UNITSYS_BASE_FORCE;           
        dM1 = D_UNITSYS_BASE_MOMENT;           
        dM2 = D_UNITSYS_BASE_MOMENT;           
        dN_Ed = D_UNITSYS_BASE_FORCE;         
        dM_Ed = D_UNITSYS_BASE_MOMENT;         
        dMa_Ed = D_UNITSYS_BASE_MOMENT;        
        dMpl_Rd = D_UNITSYS_BASE_MOMENT;       
        dMel_Rd = D_UNITSYS_BASE_MOMENT;       
        dNb_Rd = D_UNITSYS_BASE_FORCE;
        dMb_Rd = D_UNITSYS_BASE_MOMENT;
        dMcr   = D_UNITSYS_BASE_MOMENT;
        dM_Rd = D_UNITSYS_BASE_MOMENT;
        dcc   = D_UNITSYS_BASE_STRESS;           // Cd/l               
        dAwc  = D_UNITSYS_BASE_AREA;     
        dAf   = D_UNITSYS_BASE_AREA;           //                    [Unit=Area]
        dI_at = D_UNITSYS_BASE_STIF;         //                    [Unit=L^4]
        dI_afz = D_UNITSYS_BASE_STIF;        //                    [Unit=L^4]

    }  
    int dFx;          
    int dMy;          
    int dMz;    
    int dV1;           
    int dV2;           
    int dM1;           
    int dM2;           
    int dN_Ed;         
    int dM_Ed;         
    int dMa_Ed;        
    int dMpl_Rd;       
    int dMel_Rd;       
    int dNb_Rd;
    int dMb_Rd;
    int dMcr;
    //  
    int dM_Rd;         // M_Rd               [Unit=Moment]
    int dcc;           // Cd/l               [Unit=Stress]
    int dAwc;          //                    [Unit=Area]
    int dAf;           //                    [Unit=Area]
    int dI_at;         //                    [Unit=L^4]
    int dI_afz;        //                    [Unit=L^4]

};

// Plate girder, Resistance to Transverse Force
struct T_PRTF_BASE
{
    BOOL bChk;
    T_ELEM_K ElemK;       // Element             [Unit=None]
    UINT nPosi;           // Position 1=I, 2=J   [Unit=None]
    UINT LcomK;           // LcomK               [Unit=None]
    UINT OrgLcomK;        // LcomK               [Unit=None]
    int iMax;             //                     [Unit=None]
    int iSectClass;       // class               [Unit=None]
    double dFx;           // Fx                  [Unit=Force]
    double dFz;           // V1                  [Unit=Force]
    double dMy;           // My                  [Unit=Moment]
    double dMz;           // Mz                  [Unit=Moment]
    double dN_Ed;         // N_Ed                [Unit=Force]
    double dMy_Ed;        // My,Ed               [Unit=Moment]  
    double dMz_Ed;        // My,Ed               [Unit=Moment]  
    double dF_Ed;         // F_Ed                [Unit=Force] 
    double dF_Rd;         // F_Rd                [Unit=Force]
    double dEta1;         //                     [Unit=None]
    double dEta2;         //                     [Unit=None]
    double dInterRat;     // Eta2+0.8*Eta1<=1.4  [Unit=None]  
    double dRatio;       // Ratio                [Unit=None]
    BOOL bOK;
    //
    double db1;           // b1                  [Unit=Length] 
    double dey_N;         // ey,N                [Unit=Length]
    double dez_N;         // ez,N                [Unit=Length]
    double dEta1_p[4];    // eta1, 4지점         [Unit=None]
    double dIsl1;         // Isl,1               [Unit=Length^4]
    double dgamma_s;      // gamma,s             [Unit=None]
    double dk_F;          // k,F                 [Unit=None]
    double dm1, dm2;      // m1,m2               [Unit=None]
    double dly1, dly2;    // ly1, ly2            [Unit=Length]
    double dle, dly;      // le, ly              [Unit=Length]
    double dFcr;          // Fcr                 [Unit=Force]
    double dLamda_F;      // Lamda,F             [Unit=None]
    double dX_F;          // X,F                 [Unit=None]
    double dL_eff;        // L,eff               [Unit=Length]  

    void Initialize()
    {
        bChk = FALSE;
        ElemK = 0;
        nPosi = 0;
        LcomK = 0;
        OrgLcomK = 0;
        iMax = 0;	
        iSectClass = 0;    
        dFx = 0.0;           
        dFz = 0.0;           
        dMy = 0.0;           
        dMz = 0.0;           
        dN_Ed = 0.0;         
        dMy_Ed = 0.0;          
        dMz_Ed = 0.0;          
        dF_Ed = 0.0;         
        dF_Rd = 0.0;         
        dEta1 = 0.0;         
        dEta2 = 0.0;         
        dInterRat = 0.0; 
        dRatio = 0.0;
        bOK = FALSE;
        //
        db1 = 0.0;
        dey_N = 0.0;         
        dez_N = 0.0;         
        for(int i=0; i<4; i++)
            dEta1_p[i]= 0.0;  
        dIsl1 = 0.0;        
        dgamma_s = 0.0;     
        dk_F = 0.0;         
        dm1 = 0.0;
        dm2 = 0.0;      
        dly1 = dly2 = 0.0;  
        dle = dly = 0.0;    
        dFcr = 0.0;         
        dLamda_F = 0.0;     
        dX_F = 0.0;         
        dL_eff = 0.0;       

    }
    T_PRTF_BASE() { Initialize(); }
};

struct T_PRTF_D
{
     // 0=I-Max, 1=J-Max
     T_PRTF_BASE PrtfBase[2];

     void Initialize()
     {
         for(int i=0; i<2; i++)
             PrtfBase[i].Initialize();
     }
     T_PRTF_D() { Initialize(); }
};

struct T_PRTF_UNIT
{
    T_PRTF_UNIT()
    {
        dFx = D_UNITSYS_BASE_FORCE;           
        dFz = D_UNITSYS_BASE_FORCE;           
        dMy = D_UNITSYS_BASE_MOMENT;           
        dMz = D_UNITSYS_BASE_MOMENT;           
        dN_Ed = D_UNITSYS_BASE_FORCE;         
        dMy_Ed = D_UNITSYS_BASE_MOMENT;          
        dMz_Ed = D_UNITSYS_BASE_MOMENT;          
        dF_Ed = D_UNITSYS_BASE_FORCE;         
        dF_Rd = D_UNITSYS_BASE_FORCE;
        db1 = D_UNITSYS_BASE_LENGTH;
        dey_N = dez_N = D_UNITSYS_BASE_LENGTH;         
        dIsl1 = D_UNITSYS_BASE_STIF;        
        dly1 = dly2 = D_UNITSYS_BASE_LENGTH;  
        dle = dly = D_UNITSYS_BASE_LENGTH;    
        dFcr = D_UNITSYS_BASE_FORCE;         
        dL_eff = D_UNITSYS_BASE_LENGTH;     
    }  
    int dFx;           
    int dFz;           
    int dMy;           
    int dMz;           
    int dN_Ed;         
    int dMy_Ed;          
    int dMz_Ed;          
    int dF_Ed;         
    int dF_Rd; 
    //
    int db1;
    int dey_N;         // ey,N                [Unit=Length]
    int dez_N;         // ez,N                [Unit=Length]
    int dIsl1;         // Isl,1               [Unit=Length^4]
    int dly1, dly2;    // ly1, ly2            [Unit=Length]
    int dle, dly;      // le, ly              [Unit=Length]
    int dFcr;          // Fcr                 [Unit=Force]
    int dL_eff;        // L,eff               [Unit=Length]  
};

// Plate Girder, resistance to Longitudinal Shear
struct T_PGLS_BASE
{
    BOOL bChk;
    T_ELEM_K ElemK;       // Element             [Unit=None]
    UINT nPosi;           // Position 1=I, 2=J   [Unit=None]
    UINT LcomK;           // LcomK               [Unit=None]
    UINT OrgLcomK;        // LcomK               [Unit=None]
    int iMax;
    int iSectClass;       //                     [Unit=None]
    double dFx;           // Fx                  [Unit=Force]
    double dFz;           // Fz                  [Unit=Force]
    double dMy;           // My                  [Unit=Moment]
    double dMz;           // Mz                  [Unit=Moment]
    double dNc;           // Nc                  [Unit=Force]
    double dNc_el;        // Nc,el               [Unit=Force]
    double dNc_f;         // Nc,f                [Unit=Force]
    double dMa_Ed;        // Ma,Ed               [Unit=Moment]
    double dMc_Ed;        // Ma,Ed               [Unit=Moment]
    double dMpl_Rd;       // Mpl,Rd              [Unit=Moment]
    double dMel_Rd;       // Mel,Rd              [Unit=Moment]
    double dV_L_Ed;       // V_L_Ed              [Unit=Force]
    double dv_L_Ed;       // v_L,Ed(N/mm)        [Unit=F/L]  
    double dv_L_Ed2;      // v_L,Ed(N/mm)        [Unit=F/L]  
    double dP_Rd1;        // P_Rd(6.18)          [Unit=Force]
    double dP_Rd2;        // P_Rd(6.19)          [Unit=Force]  
    double dP_Rd;         // P_Rd                [Unit=Force]
    int iCheck_P_Rd;      // min(dP_Rd1,dP_Rd2)  [Unit=None]
    double dv_L_Rd;       // P_Rd*(Stud개수)/간격[Unit=F/L]
    double dv_Ed;         // v_Ed(Slab의..)      [Unit=Stress]
    double dv_Ed_lim;     // v_Ed_lim,           [Unit=Stress]
    double dRatio;        // Ratio               [Unit=None]
    BOOL bOK;
    //
    double dAlpha;        // Alpha               [Unit=None]
    double dAzI;          // A*z/I               [Unit=Length] 

    void Initialize()
    {
        bChk = FALSE;
        ElemK = 0;
        nPosi = 0;
        LcomK = 0;
        OrgLcomK = 0;
        iMax = 0;	
        dFx = 0.0; 
        dFz = 0.0;
        dMy = 0.0;           
        dMz = 0.0;           
        dNc = 0.0;           
        dNc_el = 0.0;        
        dNc_f = 0.0;         
        dMa_Ed = 0.0;        
        dMc_Ed = 0.0;        
        dMpl_Rd = 0.0;       
        dMel_Rd = 0.0;       
        dV_L_Ed = 0.0;       
        dv_L_Ed = 0.0; 
        dv_L_Ed2 = 0.0;
        dP_Rd1=dP_Rd2=dP_Rd = 0.0;    
        iCheck_P_Rd=0;
        dv_L_Rd = 0.0;       
        dv_Ed = 0.0;
        dv_Ed_lim = 0.0;
        dRatio = 0.0;
        bOK = FALSE;
        //
        dAlpha = 0.0;
        dAzI = 0.0;  
    }
    T_PGLS_BASE() { Initialize(); }
};

struct T_PGLS_D
{
    // 0=I-Max, 1=J-Max
    T_PGLS_BASE BeforePgls[2];
    T_PGLS_BASE PglsBase[2];

    void Initialize()
    {
        for(int i=0; i<2; i++)
        {
            BeforePgls[i].Initialize();
            PglsBase[i].Initialize();
        }
    }
    T_PGLS_D() { Initialize(); }
};

struct T_PGLS_UNIT
{
    T_PGLS_UNIT()
    {
        dFx = D_UNITSYS_BASE_FORCE;           
        dFz = D_UNITSYS_BASE_FORCE;           
        dMy = D_UNITSYS_BASE_MOMENT;           
        dMz = D_UNITSYS_BASE_MOMENT;           
        dNc = D_UNITSYS_BASE_FORCE;           
        dNc_el = D_UNITSYS_BASE_FORCE;        
        dNc_f = D_UNITSYS_BASE_FORCE;         
        dMa_Ed = D_UNITSYS_BASE_MOMENT;        
        dMc_Ed = D_UNITSYS_BASE_MOMENT;        
        dMpl_Rd = D_UNITSYS_BASE_MOMENT;       
        dMel_Rd = D_UNITSYS_BASE_MOMENT;       
        dV_L_Ed = D_UNITSYS_BASE_FORCE;       
        dv_L_Ed = D_UNITSYS_BASE_UNITFORCE;       
        dv_L_Ed2= D_UNITSYS_BASE_UNITFORCE;       
        dP_Rd1 = D_UNITSYS_BASE_FORCE;
        dP_Rd2 = D_UNITSYS_BASE_FORCE;
        dP_Rd = D_UNITSYS_BASE_FORCE;         
        dv_L_Rd = D_UNITSYS_BASE_UNITFORCE;       
        dv_Ed = D_UNITSYS_BASE_STRESS;
        dv_Ed_lim = D_UNITSYS_BASE_STRESS;
        dAzI = D_UNITSYS_BASE_LENGTH;  
    }  
    int dFx; 
    int dFz;
    int dMy;           
    int dMz;           
    int dNc;           
    int dNc_el;        
    int dNc_f;         
    int dMa_Ed;        
    int dMc_Ed;        
    int dMpl_Rd;       
    int dMel_Rd;       
    int dV_L_Ed;       
    int dv_L_Ed;
    int dv_L_Ed2;
    int dP_Rd1;
    int dP_Rd2;
    int dP_Rd;         
    int dv_L_Rd;       
    int dv_Ed;   
    int dv_Ed_lim;
    int dAzI;
};

// Plate Girder, Resistance to Fatigue
struct T_PGRF_BASE
{
    BOOL bChk;
    T_ELEM_K ElemK;       // Element             [Unit=None]
    UINT nPosi;           // Position 1=I, 2=J   [Unit=None]
    UINT LcomK;           // LcomK               [Unit=None]
    UINT OrgLcomK;        // LcomK               [Unit=None]
    int iMax;
    double dFx;           // Fx                  [Unit=Force]
    double dFz;           // Fz                  [Unit=Force]
    double dMy;           // My                  [Unit=Moment]
    double dMz;           // Mz                  [Unit=Moment]
    double dDel_Tau;      // Δτ                [Unit=Stress]
    double dLamda_v;      // λv                 [Unit=None]
    double dLamda_v1;     // λv1                [Unit=None]
    double dLamda_v2;     // λv2                [Unit=None]
    double dLamda_v3;     // λv3                [Unit=None]
    double dLamda_v4;     // λv4                [Unit=None]
    double dDel_Tau_E_2;  // Δτ_E,2            [Unit=Stress]
    double dDel_Tau_c;    // Δτ_c              [Unit=Stress]
    double dGamma_Ff;     //                     [Unit=None]
    double dGamma_Mfs;    //                     [Unit=None]
    double dRatio;        // rFf*Δτ_E,2/(Δτ_c/rMf,s)  [Unit=None]
    BOOL bOK;
    //
    double dv_L_Ed;       // v,L,Ed              [Unit=UnitForce]
    double dFsc;          // Fsc                 [Unit=Force]
    double dAsc;          // Asc                 [Unit=Area]

    void Initialize()
    {
        bChk = FALSE;
        ElemK = 0;
        nPosi = 0;
        LcomK = 0;
        OrgLcomK = 0;
        iMax = 0;	
        dFx = 0.0; 
        dFz = 0.0;
        dMy = 0.0;           
        dMz = 0.0;
        dDel_Tau = 0.0;     
        dLamda_v = 0.0;     
        dLamda_v1 = 0.0;    
        dLamda_v2 = 0.0;    
        dLamda_v3 = 0.0;    
        dLamda_v4 = 0.0;    
        dDel_Tau_E_2 = 0.0; 
        dDel_Tau_c = 0.0;  
        dGamma_Ff = 0.0;
        dGamma_Mfs = 0.0;
        dRatio = 0.0;       
        bOK = FALSE;
        //
        dv_L_Ed = 0.0;
        dFsc = 0.0;          
        dAsc = 0.0;
    }
    T_PGRF_BASE() { Initialize(); }
};

struct T_PGRF_D
{
     // 0=I-Max, 1=J-Max
     T_PGRF_BASE PgrfBase[2];

     void Initialize()
     {
         for(int i=0; i<2; i++)
             PgrfBase[i].Initialize();
     }
     T_PGRF_D() { Initialize(); }
};

struct T_PGRF_UNIT
{
    T_PGRF_UNIT()
    {
        dFx = D_UNITSYS_BASE_FORCE;           
        dFz = D_UNITSYS_BASE_FORCE;           
        dMy = D_UNITSYS_BASE_MOMENT;           
        dMz = D_UNITSYS_BASE_MOMENT;           
        dDel_Tau     = D_UNITSYS_BASE_STRESS;
        dDel_Tau_E_2 = D_UNITSYS_BASE_STRESS;
        dDel_Tau_c   = D_UNITSYS_BASE_STRESS;
        dv_L_Ed = D_UNITSYS_BASE_UNITFORCE;
        dFsc = D_UNITSYS_BASE_FORCE;
        dAsc = D_UNITSYS_BASE_AREA;

    }  
    int dFx; 
    int dFz;
    int dMy;           
    int dMz;           
    int dDel_Tau;     
    int dDel_Tau_E_2; 
    int dDel_Tau_c;   
    int dv_L_Ed;     
    int dFsc;        
    int dAsc;        
};

//Plate Girder, Beam Stress at the serviceability limit state
struct T_PGBS_GR_BASE // girder
{
    UINT LcomK;           // LcomK               [Unit=None]
    UINT OrgLcomK;        // LcomK               [Unit=None]
    UINT LcomType;        // // 0=None, 1=Quasi-permanent, 2=Frequent, 3=Characteristic
    int iMax;
    double aBeforeForce[6];
    double aForce[6];
    double dBeforeStress[4];
    double dStress[4];    // 0=TL, 1=TR, 2=BR, 3=BL [Unit=Stress]
    double dRatio[4];       // Ratio               [Unit=None]
    double dMaxStress;    // Max Stress     [Unit=Stress]
    double dMaxRatio;       // Ratio               [Unit=None]
    int iIndex_cr;
    double dAlwStress;
    BOOL bChk_Stress;
    double dtau_w;        // web shear stress [Unit=Stress]
    double dAlw_tau;
    BOOL bChk_tau;
    double dStress_cb;   // 
    double dAlw_stress_cb;
    BOOL bChk_Stress_cb;
    BOOL bOK; // bChk_Stress, bChk_tau, bChk_Stress_cb

    void Initialize()
    {
        LcomK = 0;
        OrgLcomK = 0;
        LcomType = 0;
        iMax = 0;	
        memset(aBeforeForce,0,sizeof(aBeforeForce));
        memset(aForce,0,sizeof(aForce));
        memset(dBeforeStress,0,sizeof(dBeforeStress));
        memset(dStress,0,sizeof(dStress));
        memset(dRatio,0,sizeof(dRatio));
        dMaxRatio = 0.0;
        iIndex_cr = 0;
        dAlwStress = 0.0;
        bChk_Stress = FALSE;
        dtau_w = 0.0;
        dAlw_tau = 0.0;
        bChk_tau = FALSE;
        dStress_cb = 0.0;   // 
        dAlw_stress_cb = 0.0;
        bChk_Stress_cb = FALSE;
        bOK = FALSE;
    }

    T_PGBS_GR_BASE() { Initialize(); }
};
struct T_PGBS_SB_BASE // Slab
{
    UINT LcomK;           // LcomK               [Unit=None]
    UINT OrgLcomK;        // LcomK               [Unit=None]
    UINT LcomType;        // 0=None, 1=Quasi-permanent, 2=Frequent, 3=Characteristic
    int iMax;
    double aForce[6];
    double dStress_sb;       // Sigma_c  [Unit=Stress]
    double dAlwStress_sb;
    double dMaxRatio_sb;
    BOOL bChk_Stress_sb;

    void Initialize()
    {
        LcomK = 0;
        OrgLcomK = 0;
        LcomType = 0;
        dStress_sb = 0.0;
        dAlwStress_sb = 0.0;
        dMaxRatio_sb=0.0;
        bChk_Stress_sb = FALSE;
    }
    T_PGBS_SB_BASE() { Initialize(); }
};
struct T_PGBS_BAR_BASE // Rebar
{
    UINT LcomK;           // LcomK               [Unit=None]
    UINT OrgLcomK;        // LcomK               [Unit=None]
    UINT LcomType;        // 0=None, 1=Quasi-permanent, 2=Frequent, 3=Characteristic
    int iMax;
    double aForce[6];
    double dStress_bar;       // Sigma_s  [Unit=Stress]
    double dAlwStress_bar;
    double dMaxRatio_bar;
    BOOL bChk_Stress_bar;

    void Initialize()
    {
        LcomK = 0;           
        OrgLcomK = 0;        
        LcomType = 0;        
        dStress_bar = 0.0;       
        dAlwStress_bar = 0.0;
        dMaxRatio_bar=0.0;
        bChk_Stress_bar = FALSE;
    }
    T_PGBS_BAR_BASE() { Initialize(); }

};

struct T_PGBS_BASE
{
    BOOL bChk;
    T_ELEM_K ElemK;       // Element             [Unit=None]
    UINT nPosi;           // Position 1=I, 2=J   [Unit=None]
    T_PGBS_GR_BASE ChkGirder; // girder
    T_PGBS_SB_BASE ChkSlab;  // slab
    T_PGBS_BAR_BASE ChkBar;  // Rebar  
     
    BOOL bOK; // 위의 bChk_ 부분이 다 만족해야 OK

    void Initialize()
    {
        bChk = FALSE;
        ElemK = 0;
        nPosi = 0;
        ChkGirder.Initialize();
        ChkSlab.Initialize();
        ChkBar.Initialize();
        bOK = FALSE;
    }
    T_PGBS_BASE() { Initialize(); }
};

struct T_PGBS_D
{
     // 0=I-Max, 1=J-Max
     T_PGBS_BASE PgbsBase[2];

     void Initialize()
     {
         for(int i=0; i<2; i++)
             PgbsBase[i].Initialize();
     }
     T_PGBS_D() { Initialize(); }
};

struct T_PGBS_GR_UNIT
{
    T_PGBS_GR_UNIT()
    {
        for(int i=0; i<4; i++) 
        {
            dStress[i] = D_UNITSYS_BASE_STRESS;
            dBeforeStress[i] = D_UNITSYS_BASE_STRESS;
        }
        for(int i = 0; i<6; i++) 
        {
            aForce[i] = D_UNITSYS_BASE_FORCE;
            aBeforeForce[i] = D_UNITSYS_BASE_FORCE;
        }
        dMaxStress = D_UNITSYS_BASE_STRESS;    
        dAlwStress = D_UNITSYS_BASE_STRESS;
        dtau_w = D_UNITSYS_BASE_STRESS;        
        dAlw_tau = D_UNITSYS_BASE_STRESS;
        dStress_cb = D_UNITSYS_BASE_STRESS;   
        dAlw_stress_cb = D_UNITSYS_BASE_STRESS;
    }
    int dBeforeStress[4];
    int dStress[4];
    int aForce[6];
    int aBeforeForce[6];
    int dMaxStress;    
    int dAlwStress;
    int dtau_w;        
    int dAlw_tau;
    int dStress_cb;   
    int dAlw_stress_cb;
};
struct T_PGBS_SB_UNIT
{
    T_PGBS_SB_UNIT()
    {
        for(int i=0; i<6; i++) aForce[i] = D_UNITSYS_BASE_FORCE;
        dStress_sb = D_UNITSYS_BASE_STRESS;   
        dAlwStress_sb = D_UNITSYS_BASE_STRESS;
    }
    int aForce[6];
    int dStress_sb;   
    int dAlwStress_sb;
};
struct T_PGBS_BAR_UNIT
{
    T_PGBS_BAR_UNIT()
    {
        for(int i=0; i<6; i++) aForce[i] = D_UNITSYS_BASE_FORCE;
        dStress_bar = D_UNITSYS_BASE_STRESS;  
        dAlwStress_bar = D_UNITSYS_BASE_STRESS;
    }
    int aForce[6];
    int dStress_bar;  
    int dAlwStress_bar;
};
struct T_PGBS_UNIT
{
    T_PGBS_UNIT()
    {    
    }  
    T_PGBS_GR_UNIT GirderUnit;
    T_PGBS_SB_UNIT SlabUnit;
    T_PGBS_BAR_UNIT RbarUnit;
};

// Longitudinal Shear Serviceability
struct T_PLSS_BASE 
{
    BOOL bChk;
    T_ELEM_K ElemK;       // Element             [Unit=None]
    UINT nPosi;           // Position 1=I, 2=J   [Unit=None]
    UINT LcomK;           // LcomK               [Unit=None]
    UINT OrgLcomK;        // LcomK               [Unit=None]
    UINT LcomType;        // 0=None, 1=Quasi-permanent, 2=Frequent, 3=Characteristic
    int iMax;
    int iSectClass;       //                     [Unit=None]
    double aForce[6];     // 
    double dNc;           // Nc                  [Unit=Force]
    double dNc_el;        // Nc,el               [Unit=Force]
    double dNc_f;         // Nc,f                [Unit=Force]
    double dMa_Ed;        // Ma,Ed               [Unit=Moment]
    double dMc_Ed;        // Ma,Ed               [Unit=Moment]
    double dMpl_Rd;       // Mpl,Rd              [Unit=Moment]
    double dMel_Rd;       // Mel,Rd              [Unit=Moment]
    double dV_L_Ed;       // V_L_Ed              [Unit=Force]
    double dv_L_Ed;       // v_L,Ed(N/mm)        [Unit=F/L]  
    double dv_L_Ed2;      // v_L,Ed(N/mm)        [Unit=F/L]  
    double dP_Rd1;        // P_Rd(6.18)          [Unit=Force]
    double dP_Rd2;        // P_Rd(6.19)          [Unit=Force]  
    double dP_Rd;         // P_Rd                [Unit=Force]
    double dP_Rd_SLS;     // ks*P_Rd             [Unit=Force]
    int iCheck_P_Rd;      // min(dP_Rd1,dP_Rd2)  [Unit=None]
    double dv_L_Rd;       // P_Rd*(Stud개수)/간격[Unit=F/L]
    double dv_Ed;         // v_Ed(Slab의..)      [Unit=Stress]
    double dv_Ed_lim;     // v_Ed_lim,           [Unit=Stress]
    double dRatio;        // Ratio               [Unit=None]
    
    BOOL bOK;
    //
    double dAlpha;        // Alpha               [Unit=None]
    double dAzI;          // A*z/I               [Unit=Length] 
    double dmyu_c;        // [Unit=L^3]  is the moment of area of the concrete slab with respect to the centre of gravity of the composite cross-section
    double dNi;
    double dli;

    void Initialize()
    {
        bChk = FALSE;
        ElemK = 0;
        nPosi = 0;
        LcomK = 0;
        OrgLcomK = 0;
        iMax = 0;	
        dNc = 0.0;           
        dNc_el = 0.0;        
        dNc_f = 0.0;         
        dMa_Ed = 0.0;        
        dMc_Ed = 0.0;        
        dMpl_Rd = 0.0;       
        dMel_Rd = 0.0;       
        dV_L_Ed = 0.0;       
        dv_L_Ed = 0.0; 
        dv_L_Ed2 = 0.0;
        dP_Rd1=dP_Rd2=dP_Rd = 0.0;    
        dP_Rd_SLS=0.0;
        iCheck_P_Rd=0;
        dv_L_Rd = 0.0;       
        dv_Ed = 0.0;
        dv_Ed_lim = 0.0;
        dRatio = 0.0;
        bOK = FALSE;
        //
        dAlpha = 0.0;
        dAzI = 0.0;  
        dmyu_c = 0.0;
    }

    T_PLSS_BASE() { Initialize(); }
};
struct T_PLSS_D
{
    // 0=I-Max, 1=J-Max
    T_PLSS_BASE BeforePlss[2];
    T_PLSS_BASE PlssBase[2];

    void Initialize()
    {
        for(int i=0; i<2; i++)
        {
            BeforePlss[i].Initialize();
            PlssBase[i].Initialize();
        }
    }
    T_PLSS_D() { Initialize(); }
};

struct T_PLSS_UNIT
{
    T_PLSS_UNIT()
    {
        for(int i=0; i<6; i++) aForce[i] = D_UNITSYS_BASE_FORCE;
        dNc = D_UNITSYS_BASE_FORCE;           
        dNc_el = D_UNITSYS_BASE_FORCE;        
        dNc_f = D_UNITSYS_BASE_FORCE;         
        dMa_Ed = D_UNITSYS_BASE_MOMENT;        
        dMc_Ed = D_UNITSYS_BASE_MOMENT;        
        dMpl_Rd = D_UNITSYS_BASE_MOMENT;       
        dMel_Rd = D_UNITSYS_BASE_MOMENT;       
        dV_L_Ed = D_UNITSYS_BASE_FORCE;       
        dv_L_Ed = D_UNITSYS_BASE_UNITFORCE;       
        dv_L_Ed2= D_UNITSYS_BASE_UNITFORCE;       
        dP_Rd1 = D_UNITSYS_BASE_FORCE;
        dP_Rd2 = D_UNITSYS_BASE_FORCE;
        dP_Rd = D_UNITSYS_BASE_FORCE;     
        dP_Rd_SLS = D_UNITSYS_BASE_FORCE;
        dv_L_Rd = D_UNITSYS_BASE_UNITFORCE;       
        dv_Ed = D_UNITSYS_BASE_STRESS;
        dv_Ed_lim = D_UNITSYS_BASE_STRESS;
        dAzI = D_UNITSYS_BASE_LENGTH;  
        dmyu_c = D_UNITSYS_BASE_VOLUME;
    }  
    int aForce[6];
    int dFx; 
    int dFz;
    int dMy;           
    int dMz;           
    int dNc;           
    int dNc_el;        
    int dNc_f;         
    int dMa_Ed;        
    int dMc_Ed;        
    int dMpl_Rd;       
    int dMel_Rd;       
    int dV_L_Ed;       
    int dv_L_Ed;
    int dv_L_Ed2;
    int dP_Rd1;
    int dP_Rd2;
    int dP_Rd;    
    int dP_Rd_SLS;
    int dv_L_Rd;       
    int dv_Ed;   
    int dv_Ed_lim;
    int dAzI;
    int dmyu_c;
};

struct T_PGDR_D
{
    T_PGBR_LCOM PgbrD;
    T_PRVS_D PrvsD;
    T_PLTB_D PltbD;
    T_PGLS_D PglsD;
    T_PGRF_D PgrfD;
    T_PGBS_D PgbsD;

    void Initialize()
    {
        PgbrD.Initialize();
        PrvsD.Initialize();
        PltbD.Initialize();
        PglsD.Initialize();
        PgrfD.Initialize();
        PgbsD.Initialize();
    }
    T_PGDR_D () { Initialize();  }  

};



// For Plate girder detail
struct CPG_STRS_POS
{
    double dStr[4];

    void Initialize()
    {
        for (int i = 0; i < 4; ++i)
        {
            dStr[i] = 0.0;
        }
    }
    CPG_STRS_POS() { Initialize(); }
};

struct CPG_STRESS_CONS
{
    CPG_STRS_POS GirB;
    CPG_STRS_POS GirA;
    CPG_STRS_POS Slab;

    void Initialize()
    {
        GirB.Initialize();
        GirA.Initialize();
        Slab.Initialize();
    }
    CPG_STRESS_CONS() { Initialize(); }
};

struct CPG_SECT_EFFE
{	  
	T_SECT_STIFFNESS EffeStlSect; // Girder 유효단면.
    T_SECT_STIFFNESS EffeComSect; // Composite 유효단면.

    void Initialize()
	{
		EffeStlSect.Initialize();
        EffeComSect.Initialize();
	}
	CPG_SECT_EFFE& operator = (const CPG_SECT_EFFE& rData)
	{
		EffeStlSect=rData.EffeStlSect;
        EffeComSect=rData.EffeComSect;
		return *this;
	}
};

struct CPG_SECT_EFFE_UNIT
{
    CPG_SECT_EFFE_UNIT()
    {
        
    }
    T_SECT_STIFFNESS_UNIT EffeStlSect;
    T_SECT_STIFFNESS_UNIT EffeComSect;
};

struct T_STRESS_LINE_INFO
{
    double dy1, dz1;
    double dy2, dz2;
    double dStr1, dStr2;
    
    void Initialize()
    {
        dy1=dz1=dy2=dz2=0.0;
        dStr1=dStr2=0.0;    
    }

    T_STRESS_LINE_INFO() {Initialize(); }
    T_STRESS_LINE_INFO(const T_STRESS_LINE_INFO& rData) { *this = rData; } 
    T_STRESS_LINE_INFO& operator=(const T_STRESS_LINE_INFO& rData) 
    {
        dy1 = rData.dy1;
        dz1 = rData.dz1;
        dy2 = rData.dy2;
        dz2 = rData.dz2;
        dStr1 = rData.dStr1;
        dStr2 = rData.dStr2;
        
        return *this;
    }  
};
struct T_STRESS_INFO_I
{
    T_STRESS_LINE_INFO FlgTL;
    T_STRESS_LINE_INFO FlgTR;
    T_STRESS_LINE_INFO Web;
    T_STRESS_LINE_INFO FlgBL;
    T_STRESS_LINE_INFO FlgBR;
    void Initialize()
    {
        FlgTL.Initialize();
        FlgTR.Initialize();
        Web.Initialize();
        FlgBL.Initialize();
        FlgBR.Initialize();
    }

    T_STRESS_INFO_I() {Initialize(); }
    T_STRESS_INFO_I(const T_STRESS_INFO_I& rData) { *this = rData; } 
    T_STRESS_INFO_I& operator=(const T_STRESS_INFO_I& rData) 
    {
        FlgTL = rData.FlgTL;
        FlgTR = rData.FlgTR;
        Web   = rData.Web  ;
        FlgBL = rData.FlgBL;
        FlgBR = rData.FlgBR;
        
        return *this;
    }  
};


struct T_PGBR_DETAIL
{
    T_STRESS_INFO_I StressInfoHshapeD;

    T_GLINE_DGN_D GLineDgn;
    T_DLINE_CALC_D StressInfo4Class; // 작용부재력에 대한 Stress, Fx+My
    T_DLINE_CALC_D DlineCalc4Fx;
    T_DLINE_CALC_D DlineCalc4My;

    double dN_slab, dNr_top, dNr_bot, dNg_top, dNg_bot;
    double dPlasticNA_NeglWeb;
    double dbeta_Mpl_Rd;
    double dMpl_Rd_cal;

    T_SECT_STIFFNESS EffGirderFx;
    T_SECT_STIFFNESS EffCompositeFx;
     //
    T_SECT_STIFFNESS EffGirder;
    T_SECT_STIFFNESS EffComposite;

    double dDelMa_Ed;
    double dDelMc_Ed;
    double dM_Ed_top; // Shear resistance
    double dM_Ed_bot;
    double dM_Ed;
    int    nKminType;

    double dRho_c;
    T_DGNLINE_LBD_BR RhocBeforeFx;
    T_DGNLINE_LBD_BR RhocAfterFx;
    T_DGNLINE_LBD_BR RhocBeforeMy;
    T_DGNLINE_LBD_BR RhocAfterMy;

    void Initialize()
    {
        StressInfoHshapeD.Initialize();
        GLineDgn.Initialize();
        StressInfo4Class.Initialize();
        DlineCalc4Fx.Initialize();
        DlineCalc4My.Initialize();
        dN_slab=dNr_top=dNr_bot=dNg_top=dNg_bot=0.0;
        dPlasticNA_NeglWeb = 0.0;
        dbeta_Mpl_Rd=0.0;
        dMpl_Rd_cal=0.0;

        EffGirderFx.Initialize();
        EffCompositeFx.Initialize();
        EffGirder.Initialize();
        EffComposite.Initialize();

        dDelMa_Ed=0.0;
        dDelMc_Ed=0.0;
        dM_Ed_top=dM_Ed_bot=dM_Ed=0.0;
        nKminType = 0;

        dRho_c =1.0;
        RhocBeforeFx.Initialize();
        RhocAfterFx.Initialize();
        RhocBeforeMy.Initialize();
        RhocAfterMy.Initialize();
    }

    T_PGBR_DETAIL() {Initialize(); }
    T_PGBR_DETAIL(const T_PGBR_DETAIL& rData) { *this = rData; } 
    T_PGBR_DETAIL& operator=(const T_PGBR_DETAIL& rData) 
    {
        StressInfoHshapeD = rData.StressInfoHshapeD;
        GLineDgn = rData.GLineDgn;
        StressInfo4Class = rData.StressInfo4Class;
        DlineCalc4Fx = rData.DlineCalc4Fx;
        DlineCalc4My = rData.DlineCalc4My;
        dN_slab = rData.dN_slab;
        dNr_top = rData.dNr_top;
        dNr_bot = rData.dNr_bot;
        dNg_top  = rData.dNg_top;
        dNg_bot  = rData.dNg_bot;
        dPlasticNA_NeglWeb = dPlasticNA_NeglWeb;
        dbeta_Mpl_Rd = rData.dbeta_Mpl_Rd;    
        dMpl_Rd_cal = rData.dMpl_Rd_cal;
        EffGirderFx = rData.EffGirderFx;
        EffCompositeFx = rData.EffCompositeFx;
        EffGirder = rData.EffGirder;
        EffComposite = rData.EffComposite;
        dDelMa_Ed = rData.dDelMa_Ed;
        dDelMc_Ed = rData.dDelMc_Ed;
        dM_Ed_top = rData.dM_Ed_top;
        dM_Ed_bot = rData.dM_Ed_bot;
        dM_Ed = rData.dM_Ed;
        nKminType = rData.nKminType;
        
        dRho_c = rData.dRho_c;
        RhocBeforeFx = rData.RhocBeforeFx;
        RhocAfterFx  = rData.RhocAfterFx;
        RhocBeforeMy = rData.RhocBeforeMy;
        RhocAfterMy  = rData.RhocAfterMy;

        return *this;
    }
};

struct T_PRVS_Vbw_Rd_Sub_Panel
{
    int nLongStfnSize; // 
    double dhw, dt;
    double dAlpha;
    double dk_tau;
    double dk_tau_st;
    double dk_tau_st_lt;
    double dIsl;
    double dLamda_w;  
    void Initialize()
    {
        nLongStfnSize=0;
        dhw=dt=0.0;
        dAlpha=0.0;
        dk_tau=0.0;
        dk_tau_st=0.0;
        dk_tau_st_lt=0.0;
        dIsl=0.0;
        dLamda_w=0.0;    
    }
    T_PRVS_Vbw_Rd_Sub_Panel() {Initialize(); }
    T_PRVS_Vbw_Rd_Sub_Panel(const T_PRVS_Vbw_Rd_Sub_Panel& rData) { *this = rData; } 
    T_PRVS_Vbw_Rd_Sub_Panel& operator=(const T_PRVS_Vbw_Rd_Sub_Panel& rData) 
    {
        nLongStfnSize = rData.nLongStfnSize;
        dhw = rData.dhw;
        dt = rData.dt;
        dAlpha = rData.dAlpha;
        dk_tau = rData.dk_tau;
        dk_tau_st = rData.dk_tau_st;
        dk_tau_st_lt = rData.dk_tau_st_lt;
        dIsl = rData.dIsl;
        dLamda_w = rData.dLamda_w;

        return *this;
    }
};
struct T_PRVS_Vbw_Rd_Param
{
    int nPartType;
    CString strGroupName;
    int iStiffSize;
    BOOL bStiffened; // Long Stiffener or Transverse Stiffener가 있을 경우 .
    double dAlpha; //
    double dk_tau;
    double dk_tau_st;
    double dk_tau_st_limit;
    BOOL bIsShearBuck;
    double dIsl;  
    double dhw;
    double dt;
    double dc;
    double dLamda_w;
    double dLamda_w_cr;
    int iLamda_w_Type;
    int iLamda_w_scope; // EN 1993-1-5, Table 5.1    
    double dtau_Ed;
    double dtau_b_Rd;
    double dV_Edi;
    double dQi;
    double dIyyi;
    double dbf;
    double dXw;
    double dVbw_Rd;
    double dVbf_Rd;
    double dVpl_Rd;
    double dV_Rd;
    double dV_Rd_Limit;
    double dV_Ed;
    double deta3;
    T_PRVS_Vbw_Rd_Sub_Panel Lamda_w_SubD;  
    double dRatio_InteractionMV;
    double dtau_Ed_c;
    double dQi_c;
    double dIyyi_c;
    double dtau_Ed_sum;
    double dtau_Ed_ave;
    void Initialize()
    {
        nPartType=0;
        strGroupName.Empty();
        iStiffSize=0;
        bStiffened=FALSE;
        dAlpha=0.0;
        dk_tau=dk_tau_st=dk_tau_st_limit=0.0;
        bIsShearBuck=FALSE;
        dIsl=dhw=dt=dc=0.0;
        dLamda_w=dLamda_w_cr=0.0;
        iLamda_w_Type=iLamda_w_scope=0;
        dtau_Ed=dtau_b_Rd=dV_Edi=dQi=dIyyi=dbf=0.0;
        dXw=dVbw_Rd=dVbf_Rd=dVpl_Rd=dV_Rd=dV_Rd_Limit=dV_Ed=deta3=0.0;
        Lamda_w_SubD.Initialize();
        dRatio_InteractionMV=0.0;
        dtau_Ed_c=dQi_c=dIyyi_c=0.0;
        dtau_Ed_sum=dtau_Ed_ave=0.0;
    }
    T_PRVS_Vbw_Rd_Param() {Initialize(); }
    T_PRVS_Vbw_Rd_Param(const T_PRVS_Vbw_Rd_Param& rData) { *this = rData; } 
    T_PRVS_Vbw_Rd_Param& operator=(const T_PRVS_Vbw_Rd_Param& rData) 
    {
        nPartType = rData.nPartType;
        strGroupName = rData.strGroupName;
        iStiffSize = rData.iStiffSize;
        bStiffened = rData.bStiffened;
        dAlpha=rData.dAlpha;    
        dk_tau = rData.dk_tau;
        dk_tau_st = rData.dk_tau_st;
        dk_tau_st_limit = rData.dk_tau_st_limit;
        bIsShearBuck = rData.bIsShearBuck;
        dIsl = rData.dIsl;
        dhw = rData.dhw;
        dt = rData.dt;
	dc = rData.dc;
        dLamda_w = rData.dLamda_w;
        dLamda_w_cr = rData.dLamda_w_cr;
        iLamda_w_Type = rData.iLamda_w_Type;
        iLamda_w_scope = rData.iLamda_w_scope;    
        dtau_Ed = rData.dtau_Ed;
        dtau_b_Rd = rData.dtau_b_Rd;
        dV_Edi = rData.dV_Edi;
        dQi = rData.dQi;
        dIyyi = rData.dIyyi;
        dbf = rData.dbf;
        dXw = rData.dXw;
        dVbw_Rd = rData.dVbw_Rd;
	dVbf_Rd	= rData.dVbf_Rd;
        dVpl_Rd = rData.dVpl_Rd;
        dV_Rd = rData.dV_Rd;
        dV_Rd_Limit = rData.dV_Rd_Limit;
        dV_Ed = rData.dV_Ed;
        deta3 = rData.deta3;
        Lamda_w_SubD = rData.Lamda_w_SubD;
        dRatio_InteractionMV = rData.dRatio_InteractionMV;
        dtau_Ed_c = rData.dtau_Ed_c;
        dQi_c = rData.dQi_c;
        dIyyi_c = rData.dIyyi_c;
        dtau_Ed_sum = rData.dtau_Ed_sum;
        dtau_Ed_ave = rData.dtau_Ed_ave;

        return *this;
    }

};

struct T_PRVS_DETAIL
{
    T_STRESS_INFO_I StressInfoHshapeD;

    T_GLINE_DGN_D GLineDgn;
    T_DLINE_CALC_D StressInfo4Class; // 작용부재력에 대한 Stress, Fx+My
    T_DLINE_CALC_D DlineCalc4Fx;
    T_DLINE_CALC_D DlineCalc4My;  
    double dPlasticNA;
    double dN_slab, dNg_top, dNg_bot;
    double dNr_top, dNr_bot;
    double dM_Ed_top;
    double dM_Ed_bot;

    CArray<T_PRVS_Vbw_Rd_Param,T_PRVS_Vbw_Rd_Param&> aGenWebVbwRdParam;
    CArray<T_PRVS_Vbw_Rd_Param,T_PRVS_Vbw_Rd_Param&> aGenFlangeVbwRdParam;
    int iEndSupportType;

    double dMf_Rd_cal;
    double dMf_N_Rd;
    double dReduFactor4N_Ed;    
    int nShearBuckCount;

    void Initialize()
    {
        StressInfoHshapeD.Initialize();
        GLineDgn.Initialize();
        StressInfo4Class.Initialize();
        DlineCalc4Fx.Initialize();
        DlineCalc4My.Initialize();
        dPlasticNA=0.0;
        dN_slab=dNg_top=dNg_bot=0.0;
        dNr_top = 0.0;
        dNr_bot = 0.0;
        dM_Ed_top=dM_Ed_bot=0.0;
        aGenWebVbwRdParam.RemoveAll();
        aGenFlangeVbwRdParam.RemoveAll();
        iEndSupportType = 0;   
        dMf_Rd_cal=0.0;
        dMf_N_Rd=0.0;
        dReduFactor4N_Ed=0.0;
        nShearBuckCount=0;
    }

    T_PRVS_DETAIL() {Initialize(); }
    T_PRVS_DETAIL(const T_PRVS_DETAIL& rData) { *this = rData; } 
    T_PRVS_DETAIL& operator=(const T_PRVS_DETAIL& rData) 
    {
        StressInfoHshapeD = rData.StressInfoHshapeD;
        GLineDgn = rData.GLineDgn;
        StressInfo4Class = rData.StressInfo4Class;
        DlineCalc4Fx = rData.DlineCalc4Fx;
        DlineCalc4My = rData.DlineCalc4My;
        dPlasticNA = rData.dPlasticNA;
        dN_slab = rData.dN_slab;
        dNg_top  = rData.dNg_top;
        dNg_bot  = rData.dNg_bot;
        dNr_top = rData.dNr_top;
        dNr_bot = rData.dNr_bot;
        dM_Ed_top= rData.dM_Ed_top;
        dM_Ed_bot= rData.dM_Ed_bot;
        aGenWebVbwRdParam.Copy(rData.aGenWebVbwRdParam);
        aGenFlangeVbwRdParam.Copy(rData.aGenFlangeVbwRdParam);
        iEndSupportType = rData.iEndSupportType;    
        dMf_Rd_cal = rData.dMf_Rd_cal;
        dMf_N_Rd = rData.dMf_N_Rd;
        dReduFactor4N_Ed = rData.dReduFactor4N_Ed;
        nShearBuckCount = rData.nShearBuckCount;
         
        return *this;
    }
};

struct T_PLTB_DETAIL
{
    double dMpl_Rd;
    double dMel_Rd;
    double dL;
    double dhs;
    double da;
    double dEa;
    double dGs;
    double dI2;
    int iAlpha;
    void Initialize()
    {
        dMpl_Rd = dMel_Rd = 0.0;
        dL=0.0;
        dhs=da=dEa=0.0;
        dGs=dI2=0.0;
        iAlpha=0;
    }

    T_PLTB_DETAIL() {Initialize(); }
    T_PLTB_DETAIL(const T_PLTB_DETAIL& rData) { *this = rData; } 
    T_PLTB_DETAIL& operator=(const T_PLTB_DETAIL& rData) 
    {
        dMpl_Rd = rData.dMpl_Rd;
        dMel_Rd = rData.dMel_Rd;
        dL = rData.dL;
        dhs = rData.dhs;
        da = rData.da;
        dEa = rData.dEa;
        dGs = rData.dGs;
        dI2 = rData.dI2;
        iAlpha = rData.iAlpha;
        
        return *this;
    }
};

struct T_PRTF_DETAIL
{
    double iEta1_max_posi;
    double dWy_eff, dWz_eff;
    int iLoadAppType;
    BOOL bIsStiffener;
    int ik_F_calc_type;
    double dGamma_s_limit;
    double dGamma_s_cal;
    double dhw;
    double da;
    double dSs;
    double dC;

    void Initialize()
    {
        iEta1_max_posi = 0;
        dWy_eff = dWz_eff = 0.0;
        iLoadAppType=0;
        bIsStiffener=FALSE;
        ik_F_calc_type=0;
        dGamma_s_limit=0.0;
        dGamma_s_cal  =0.0;
        dhw=0.0;
        da=0.0;
        dSs=dC=0.0;
    }

    T_PRTF_DETAIL() {Initialize(); }
    T_PRTF_DETAIL(const T_PRTF_DETAIL& rData) { *this = rData; } 
    T_PRTF_DETAIL& operator=(const T_PRTF_DETAIL& rData) 
    {
        iEta1_max_posi = rData.iEta1_max_posi;
        dWy_eff = rData.dWy_eff;
        dWz_eff = rData.dWz_eff;
        iLoadAppType = rData.iLoadAppType;
        bIsStiffener = rData.bIsStiffener;
        ik_F_calc_type = rData.ik_F_calc_type;
        dGamma_s_limit = rData.dGamma_s_limit;
        dGamma_s_cal   = rData.dGamma_s_cal;
        dhw = rData.dhw;
        da = rData.da;
        dSs = rData.dSs;
        dC = rData.dC;
        
        return *this;
    }
};

struct T_PGLS_DETAIL
{
    double dfu;
    int iRat_sc_type;
    int iNum;
    double dDsc, dhsc, dSpace;
    double dLv;
    double dmuy_c; // 도심에 대한 Slab 단면1차 모멘트 
    double dP_Rd_SLS;

    void Initialize()
    {
        dfu=0.0;
        iRat_sc_type=0;
        iNum=0;
        dDsc=dhsc=dSpace=0.0;
        dLv=0.0;
        dmuy_c=0.0;
        dP_Rd_SLS=0.0;
    }

    T_PGLS_DETAIL() {Initialize(); }
    T_PGLS_DETAIL(const T_PGLS_DETAIL& rData) { *this = rData; } 
    T_PGLS_DETAIL& operator=(const T_PGLS_DETAIL& rData) 
    {
        dfu = rData.dfu;
        iRat_sc_type = rData.iRat_sc_type;
        iNum = rData.iNum;
        dDsc = rData.dDsc;
        dhsc = rData.dhsc;
        dSpace = rData.dSpace;   
        dLv = rData.dLv;
        dmuy_c = rData.dmuy_c;
        dP_Rd_SLS = rData.dP_Rd_SLS;

        return *this;
    }
};

struct T_PGRF_DETAIL
{

    void Initialize()
    {
    }

    T_PGRF_DETAIL() {Initialize(); }
    T_PGRF_DETAIL(const T_PGRF_DETAIL& rData) { *this = rData; } 
    T_PGRF_DETAIL& operator=(const T_PGRF_DETAIL& rData) 
    {
        
        return *this;
    }
};

struct T_PGBS_DETAIL
{
    CPG_STRESS_CONS StrD;

    void Initialize()
    {
        StrD.Initialize();
    }

    T_PGBS_DETAIL() {Initialize(); }
    T_PGBS_DETAIL(const T_PGBS_DETAIL& rData) { *this = rData; } 
    T_PGBS_DETAIL& operator=(const T_PGBS_DETAIL& rData) 
    {
        StrD = rData.StrD;

        return *this;
    }
};

struct T_PLSS_DETAIL
{
    double dfu;
    int iRat_sc_type;
    int iNum;
    double dDsc, dhsc, dSpace;
    double dLv;
    double dmuy_c; // 도심에 대한 Slab 단면1차 모멘트 

    void Initialize()
    {
        dfu=0.0;
        iRat_sc_type=0;
        iNum=0;
        dDsc=dhsc=dSpace=0.0;
        dLv=0.0;
        dmuy_c=0.0;
    }

    T_PLSS_DETAIL() {Initialize(); }
    T_PLSS_DETAIL(const T_PLSS_DETAIL& rData) { *this = rData; } 
    T_PLSS_DETAIL& operator=(const T_PLSS_DETAIL& rData) 
    {
        dfu = rData.dfu;
        iRat_sc_type = rData.iRat_sc_type;
        iNum = rData.iNum;
        dDsc = rData.dDsc;
        dhsc = rData.dhsc;
        dSpace = rData.dSpace;   
        dLv = rData.dLv;
        dmuy_c = rData.dmuy_c;
        
        return *this;
    }

};

//////////////////////////////////////////////////////////////////////////
//add for CHINA Railway design by maxiao 2007-04-07 (TB 10002.3-2005)
struct PREDEGREE
{
	BOOL bCHK;		//角뤠셕炬돨깃街
	UINT Lcomk;//駱炬코제莉북돨ID
	int itype;//駱炬뵐潼돨잚謹  0~5:Fx-max ~Mz-max. 6~11:Fx-min ~Mz-min   
	double dTL,	dTR, dBR, dBL, dT, dB;//6 position.
	double dLamda; //渡壇제똑
	double dALW;
	BOOL bOK;
	void Initialize()
	{ 
		bCHK = 0;
		Lcomk=0;
		itype=0;  
		dTL=.0; dTR=.0; dBR=.0; dBL=.0; dT=.0; dB=.0;
	  dLamda=.0;
		dALW=0.7;
		bOK = TRUE;
	}
};
struct T_BEAM_PREDEGREE_CH	// ElemK.
{
    PREDEGREE dataBase[2];	// 0=I, 1=J.
    void Initialize()
    {
        for(int i=0; i<2; i++)
            dataBase[i].Initialize();
    }
};
struct T_COLUMN_PREDEGREE_CH    // ElemK
{
    PREDEGREE dataBase[2];        // 0-I,1-J.
    void Initialize()
    {
        for(int i=0; i<2; i++)
            dataBase[i].Initialize();
    }
};
struct T_PREDEGREE_CH_UNIT
{
	T_PREDEGREE_CH_UNIT()
	{
		dTL    =D_UNITSYS_NONE; 
		dTR    =D_UNITSYS_NONE; 
		dBR    =D_UNITSYS_NONE; 
		dBL    =D_UNITSYS_NONE; 
		dT     =D_UNITSYS_NONE; 
		dB     =D_UNITSYS_NONE;
        dLamda =D_UNITSYS_NONE;
	  dALW   =D_UNITSYS_NONE;
    }
	int dTL,	dTR, dBR, dBL, dT, dB;
    int dLamda;
	int dALW;
};


struct BEND
{
    BOOL bCHK;		//角뤠셕炬돨깃街
    UINT Lcomk;//駱炬코제莉북돨ID
    int itype;//駱炬뵐潼돨잚謹 ( FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11 )
    //          ( FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12) 
    double dMy;	 // 琨앤    double dMn;   // 셕炬琨앤令 
    double dMn;   // 셕炬琨앤令 
    double dK;
    double dK_ALW;
    BOOL bOK;
    void Initialize()
    { 
        bCHK = 0;
        Lcomk=0;
        itype=0;
        dMy=0.0;
        dMn=0.0;
        dK=0.0;
        dK_ALW=0.0;
        bOK = TRUE;
    }
};

struct T_BEAM_BEND_CH	// ElemK.
{
    BEND dataBase[4];	  // 0=I-Mymax, 1=I-Mymin, 2=J-Mymax, 3=J-Mymin.
    void Initialize()
    {
        for(int i=0; i<4; i++)
            dataBase[i].Initialize();
    }
};
struct T_BEND_CH_UNIT
{
    T_BEND_CH_UNIT()
    {
        dMy = D_UNITSYS_BASE_MOMENT;
        dMn = D_UNITSYS_BASE_MOMENT;
    }
    int dMy;
    int dMn;
};

struct INCLINE_SHEAR
{  
    BOOL bCHK;		//角뤠셕炬돨깃街
    UINT Lcomk;//駱炬코제莉북돨ID
    int itype;//駱炬뵐潼돨잚謹 ( FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11 )
    //          ( FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12) 
    double dVz;	   //숏제    double dVn;   //셕炬숏제令K*V
    double dVn;   //셕炬숏제令K*V
    double dK;
    double dK_ALW;
    BOOL bOK;
    void Initialize()
    { 
        bCHK = 0;
        Lcomk=0;
        itype=0;
        dVz=0.0;
        dVn=0.0;
        dK = 0.0;
        dK_ALW = 0.0;
        bOK = TRUE;
    }
};
struct T_BEAM_INCLINE_SHEAR_CH	// ElemK.
{
    INCLINE_SHEAR dataBase[4];	// 0=I-Mymax, 1=I-Mymin, 2=J-Mymax, 3=J-Mymin.
    void Initialize()
    {
        for(int i=0; i<4; i++)
            dataBase[i].Initialize();
    }
};
struct T_INCLINE_SHEAR_CH_UNIT
{
    T_INCLINE_SHEAR_CH_UNIT()
    {
        dVz = D_UNITSYS_BASE_FORCE;
        dVn = D_UNITSYS_BASE_FORCE;
    }
    int dVz;
    int	dVn;
};


struct CON_SS
{  
	BOOL bCHK;		//角뤠셕炬돨깃街
	UINT Lcomk;//駱炬코제莉북돨ID   
	int itype;//駱炬뵐潼돨잚謹 ( FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11 )
                                 //          ( FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12) 
    double dSig;	   
    double dSig_ALW; 
    BOOL bOK;
	void Initialize()
	{ 
		bCHK = 0;
		Lcomk=0;
		itype=0;
	  dSig=0;
		dSig_ALW=0;
		bOK = TRUE;
	}
};
struct T_BEAM_CON_SS_CH	// ElemK.
{
    CON_SS dataBase[12];	// 0=I-Mymax, 1=I-Mymin, 2=J-Mymax, 3=J-Mymin.
    void Initialize()
    {
        for(int i=0; i<12; i++)
            dataBase[i].Initialize();
    }
};
struct T_COLUMN_CON_SS_CH     // ElemK
{
    CON_SS dataBase[12];        // 0=I-Fxmax, 1=I-Fxmin, 2=I-Mymax, 3=I-Mymin, 
	                              // 4=J-Fxmax, 5=J-Fxmin, 6=J-Mymax, 7=J-Mymin.
    void Initialize()
    {
        for(int i=0; i<12; i++)
            dataBase[i].Initialize();
    }
};
struct T_CON_SS_CH_UNIT
{
	T_CON_SS_CH_UNIT()
    {
		dSig     = D_UNITSYS_BASE_STRESS;	   
		dSig_ALW = D_UNITSYS_BASE_STRESS;	   
    }
	int dSig;	   
    int dSig_ALW; 
}; 


struct CON_CROSS_FRACTURE
{  
    BOOL bCHK;		//角뤠셕炬돨깃街
    int  iElemk;//elem
    int  ipos; //secton
    int  itype;//駱炬잚謹 (0꼇豚冀놔君윗壇제뭐숭. 1豚冀윗壇제뎃꼇豚冀역죙뭐숭)
    int  iCheckPos;//駱炬貫零 ( TL=0, TR=1, BR=2, BL=3, T=4, B=5 )
    UINT Lcomk;//駱炬코제莉북돨ID
    //	double dKfSig;
    //	double dKfSig_ALW;
    //  double dSigct;	   
    //  double dSig_ALW; 
    double dSig;//몽앴劤깊목狼헹，뫘劤죄杰唐鑒앴써뭐돨鞫刻렘駕2010-4-15
    double dSigc;
    double drfct;
    double dKf;
    double dKf_ALW;
    double dSigct;	   
    double dSigct_ALW;
    BOOL bOK;
    void Initialize()
    { 
        bCHK = 0;
        iElemk = ipos = iCheckPos = 0;
        Lcomk=0;
        itype=0;
        //		dKfSig = dKfSig_ALW = .0;
        //	  dSigct=	dSig_ALW=.0;
        dSig = dSigc = drfct = 0.0;
        dKf = dKf_ALW = 0.0;
        dSigct =  dSigct_ALW = 0.0;
        bOK = TRUE;
    }
};
struct T_BEAM_CON_CROSS_FRACTURE_CH	// ElemK.
{
    CON_CROSS_FRACTURE dataBase[12];	// 0=I-Mymax, 1=I-Mymin, 2=J-Mymax, 3=J-Mymin.
    void Initialize()
    {
        for(int i=0; i<12; i++)
            dataBase[i].Initialize();
    }
};
struct T_COLUMN_CON_CROSS_FRACTURE_CH	// ElemK.
{
    CON_CROSS_FRACTURE dataBase[12];	 // 0=I-Fxmax, 1=I-Fxmin, 2=I-Mymax, 3=I-Mymin, 
	                                 // 4=J-Fxmax, 5=J-Fxmin, 6=J-Mymax, 7=J-Mymin.
    void Initialize()
    {
        for(int i=0; i<12; i++)
            dataBase[i].Initialize();
    }
};
struct T_CON_CROSS_FRACTURE_CH_UNIT
{
    T_CON_CROSS_FRACTURE_CH_UNIT()
    {
        dSig      = D_UNITSYS_BASE_STRESS;
        dSigc     = D_UNITSYS_BASE_STRESS;
        drfct     = D_UNITSYS_BASE_STRESS;
        dSigct    = D_UNITSYS_BASE_STRESS;
        dSigct_ALW= D_UNITSYS_BASE_STRESS;
    }
    int dSig;
    int dSigc;
    int drfct;
    int dSigct;	   
    int dSigct_ALW; 
};


struct CON_INCLINE_FRACTURE
{
    BOOL bCHK;		//설계결과유무 
    UINT LcomK;
    int itype;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    double dSig_P1;	//1위치 주응력도		[Unit=Stress]
    double dSig_P2;	//2위치 주응력도		[Unit=Stress]
    double dSig_P3;	//3위치 주응력도		[Unit=Stress]
    double dSig_P4;	//4위치 주응력도		[Unit=Stress]
    double dSig_P5;	//5위치 주응력도		[Unit=Stress]
    double dSig_P6;	//6위치 주응력도		[Unit=Stress]
    double dSig_P7;	//7위치 주응력도		[Unit=Stress]
    double dSig_P8;	//8위치 주응력도		[Unit=Stress]
    double dSig_P9;	//9위치 주응력도		[Unit=Stress]
    double dSig_P10;	//10위치 주응력도		[Unit=Stress]
    double dSig_MAX; // MAX/MIN 주응력도	[Unit=Stress]
    double dSig_AP;  // 허용 주응력도    [Unit=Stress]
	BOOL bOK;
    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        itype = 0;    
        dSig_P1=0;	//1위치 주응력도		[Unit=Stress]
        dSig_P2=0;	//2위치 주응력도		[Unit=Stress]
        dSig_P3=0;	//3위치 주응력도		[Unit=Stress]
        dSig_P4=0;	//4위치 주응력도		[Unit=Stress]
        dSig_P5=0;	//5위치 주응력도		[Unit=Stress]
        dSig_P6=0;	//6위치 주응력도		[Unit=Stress]
        dSig_P7=0;	//7위치 주응력도		[Unit=Stress]
        dSig_P8=0;	//8위치 주응력도		[Unit=Stress]
        dSig_P9=0;	//9위치 주응력도		[Unit=Stress]
        dSig_P10=0;	//10위치 주응력도		[Unit=Stress]
        dSig_MAX=0; // MAX/MIN 주응력도	[Unit=Stress]
        dSig_AP=0;  // 허용 주응력도    [Unit=Stress]
		bOK = TRUE;
    }
};
struct T_BEAM_CON_INCLINE_FRACTURE_CH	// ElemK.
{
    CON_INCLINE_FRACTURE dataBase[4];	// 0=I-Mymax, 1=I-Mymin, 2=J-Mymax, 3=J-Mymin.
    void Initialize()
    {
        for(int i=0; i<4; i++)
            dataBase[i].Initialize();
    }
};
struct T_CON_INCLINE_FRACTURE_CH_UNIT	
{
	T_CON_INCLINE_FRACTURE_CH_UNIT()
	{
		dSig_P1 = D_UNITSYS_BASE_STRESS; 
        dSig_P2 = D_UNITSYS_BASE_STRESS; 
        dSig_P3 = D_UNITSYS_BASE_STRESS; 
        dSig_P4 = D_UNITSYS_BASE_STRESS; 
        dSig_P5 = D_UNITSYS_BASE_STRESS; 
        dSig_P6 = D_UNITSYS_BASE_STRESS; 
        dSig_P7 = D_UNITSYS_BASE_STRESS; 
        dSig_P8 = D_UNITSYS_BASE_STRESS; 
        dSig_P9 = D_UNITSYS_BASE_STRESS; 
        dSig_P10 = D_UNITSYS_BASE_STRESS; 
        dSig_MAX = D_UNITSYS_BASE_STRESS; 
        dSig_AP = D_UNITSYS_BASE_STRESS; 
	}
	int dSig_P1;
    int dSig_P2;
    int dSig_P3;
    int dSig_P4;
    int dSig_P5;
    int dSig_P6;
    int dSig_P7;
    int dSig_P8;
    int dSig_P9;
    int dSig_P10;
    int dSig_MAX;
    int dSig_AP;
};


struct CON_TP
{
    BOOL bCHK;		//설계결과유무 
    UINT LcomK;
    int itype;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    double dSig_P1;	//1위치 주응력도		[Unit=Stress]
    double dSig_P2;	//2위치 주응력도		[Unit=Stress]
    double dSig_P3;	//3위치 주응력도		[Unit=Stress]
    double dSig_P4;	//4위치 주응력도		[Unit=Stress]
    double dSig_P5;	//5위치 주응력도		[Unit=Stress]
    double dSig_P6;	//6위치 주응력도		[Unit=Stress]
    double dSig_P7;	//7위치 주응력도		[Unit=Stress]
    double dSig_P8;	//8위치 주응력도		[Unit=Stress]
    double dSig_P9;	//9위치 주응력도		[Unit=Stress]
    double dSig_P10;	//10위치 주응력도		[Unit=Stress]
    double dSig_MAX; // MAX/MIN 주응력도	[Unit=Stress]
    double dSig_AP;  // 허용 주응력도    [Unit=Stress]
	BOOL bOK;
    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        itype = 0;    
        dSig_P1=0;	//1위치 주응력도		[Unit=Stress]
        dSig_P2=0;	//2위치 주응력도		[Unit=Stress]
        dSig_P3=0;	//3위치 주응력도		[Unit=Stress]
        dSig_P4=0;	//4위치 주응력도		[Unit=Stress]
        dSig_P5=0;	//5위치 주응력도		[Unit=Stress]
        dSig_P6=0;	//6위치 주응력도		[Unit=Stress]
        dSig_P7=0;	//7위치 주응력도		[Unit=Stress]
        dSig_P8=0;	//8위치 주응력도		[Unit=Stress]
        dSig_P9=0;	//9위치 주응력도		[Unit=Stress]
        dSig_P10=0;	//10위치 주응력도		[Unit=Stress]
        dSig_MAX=0; // MAX/MIN 주응력도	[Unit=Stress]
        dSig_AP=0;  // 허용 주응력도    [Unit=Stress]
		bOK = TRUE;
    }
};
struct T_BEAM_CON_TP_CH	// ElemK.
{
    CON_TP dataBase[4];	// 0=I-Mymax, 1=I-Mymin, 2=J-Mymax, 3=J-Mymin.
    void Initialize()
    {
        for(int i=0; i<4; i++)
            dataBase[i].Initialize();
    }
};
struct T_CON_TP_CH_UNIT		
{
	T_CON_TP_CH_UNIT()
	{
		dSig_P1 = D_UNITSYS_BASE_STRESS; 
        dSig_P2 = D_UNITSYS_BASE_STRESS; 
        dSig_P3 = D_UNITSYS_BASE_STRESS; 
        dSig_P4 = D_UNITSYS_BASE_STRESS; 
        dSig_P5 = D_UNITSYS_BASE_STRESS; 
        dSig_P6 = D_UNITSYS_BASE_STRESS; 
        dSig_P7 = D_UNITSYS_BASE_STRESS; 
        dSig_P8 = D_UNITSYS_BASE_STRESS; 
        dSig_P9 = D_UNITSYS_BASE_STRESS; 
        dSig_P10 = D_UNITSYS_BASE_STRESS; 
        dSig_MAX = D_UNITSYS_BASE_STRESS; 
        dSig_AP = D_UNITSYS_BASE_STRESS; 
	}
	int dSig_P1;
    int dSig_P2;
    int dSig_P3;
    int dSig_P4;
    int dSig_P5;
    int dSig_P6;
    int dSig_P7;
    int dSig_P8;
    int dSig_P9;
    int dSig_P10;
    int dSig_MAX;
    int dSig_AP;
};


struct TENDON
{
	BOOL bCHK;		//角뤠셕炬돨깃街
	UINT Lcomk;   //駱炬코제莉북돨ID  
	int itype;    //駱炬뵐潼돨잚謹 ( FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11 )
                                //               ( FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12) 
	int iElem;
	int iPosi;
	int iTendon;

    double dSigP;	   
    double dSig_ALW;  
	BOOL bOK;
	void Initialize()
	{ 
		bCHK = 0;
		iElem = iPosi = iTendon = 0;
		Lcomk=0;
		itype=0;
	  dSigP=0;
		dSig_ALW=0;
		bOK = TRUE;
	}
};
struct T_BEAM_TENDON_CH	// ElemK.
{
    TENDON dataBase[2];	// 0=I-Mymax, 1=I-Mymin, 2=J-Mymax, 3=J-Mymin.
    void Initialize()
    {
        for(int i=0; i<2; i++)
            dataBase[i].Initialize();
    }
};
struct T_COLUMN_TENDON_CH     // ElemK
{
    TENDON dataBase[2];        // 0=I-Fxmax, 1=I-Fxmin, 2=I-Mymax, 3=I-Mymin, 
	                              // 4=J-Fxmax, 5=J-Fxmin, 6=J-Mymax, 7=J-Mymin.
    void Initialize()
    {
        for(int i=0; i<2; i++)
            dataBase[i].Initialize();
    }
};
struct T_TENDON_CH_UNIT		
{
	T_TENDON_CH_UNIT()
	{
		dSigP    = D_UNITSYS_BASE_STRESS; 
		dSig_ALW = D_UNITSYS_BASE_STRESS; 
	}
	int	dSigP;
	int dSig_ALW;
};


struct PSCCRACK
{
	BOOL bCHK;		//角뤠셕炬돨깃街
	UINT Lcomk;  //駱炬코제莉북돨ID  
	int itype;   //駱炬뵐潼돨잚謹 ( FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11 )
                        //               ( FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12) 	
    double dwfk; 
	double dwfk_ALW; 
	BOOL bOK;
	void Initialize()
	{ 
		bCHK = 0;
		Lcomk=0;
		itype=0;
        dwfk=0; 
	  dwfk_ALW=0;
		bOK = TRUE;
	}
};
struct T_BEAM_PSCCRACK_CH	// ElemK.
{
    PSCCRACK dataBase[4];	// 0=I-Mymax, 1=I-Mymin, 2=J-Mymax, 3=J-Mymin.
    void Initialize()
    {
        for(int i=0; i<4; i++)
            dataBase[i].Initialize();
    }
};
struct T_PSCCRACK_CH_UNIT		
{
	T_PSCCRACK_CH_UNIT()
	{
		dwfk     = D_UNITSYS_BASE_LENGTH;     
		dwfk_ALW = D_UNITSYS_BASE_LENGTH;     
	}
	int dwfk; 
	int dwfk_ALW;
};


struct SHEAR
{
	BOOL bCHK;		//角뤠셕炬돨깃街
	UINT Lcomk;//駱炬코제莉북돨ID  
	int itype;//駱炬뵐潼돨잚謹 ( FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11 )
                        //               ( FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12) 	
	double dshear5;
	double dshear6;
	double dshear7;
	double dshear8;
	double dshear9;
	double dshear10;

	double dshearc;	   
    double dshear_ALW;
	BOOL bOK;
	void Initialize()
	{ 
		bCHK = 0;
		Lcomk=0;
		itype=0;
		dshear5 =dshear6 =dshear7 =dshear8 =dshear9 =dshear10 =.0;
	  dshearc=.0;
		dshear_ALW=.0;
		bOK = TRUE;
	}
};
struct T_BEAM_SHEAR_CH	// ElemK.
{
    SHEAR dataBase[2];	  // 0=I, 1=J.
    void Initialize()
    {
        for(int i=0; i<2; i++)
            dataBase[i].Initialize();
    }
};
struct T_COLUMN_SHEAR_CH     // ElemK
{
    SHEAR dataBase[2];        // 0=I, 1=J.
    void Initialize()
    {
        for(int i=0; i<2; i++)
            dataBase[i].Initialize();
    }
};
struct T_SHEAR_CH_UNIT	 
{
	T_SHEAR_CH_UNIT()
	{
		dshear5    = D_UNITSYS_BASE_STRESS;		   
		dshear6    = D_UNITSYS_BASE_STRESS;		   
		dshear7    = D_UNITSYS_BASE_STRESS;		   
		dshear8    = D_UNITSYS_BASE_STRESS;		   
		dshear9    = D_UNITSYS_BASE_STRESS;		    
		dshear10   = D_UNITSYS_BASE_STRESS;		   
		dshearc    = D_UNITSYS_BASE_STRESS;		   
		dshear_ALW = D_UNITSYS_BASE_STRESS;		   
	}
	int	dshearc;
	int	dshear_ALW;
	int	dshear5,dshear6,dshear7,dshear8,dshear9,dshear10;
};


struct CON_CS
{
	UINT StagK;		  
	BOOL bCHK;
	double dSig_T;   
    double dSig_B;   
    double dSig_TL;	
    double dSig_BL;	
    double dSig_TR;	
    double dSig_BR;	
    double dSig_MAX; 
    double dSig_ALW;
	BOOL bOK;
	void Initialize()
	{ 
        bCHK=0;
		StagK=0;
        dSig_T=0;   
        dSig_B=0;  
        dSig_TL=0;	
        dSig_BL=0;	
        dSig_TR=0;	
        dSig_BR=0;	
        dSig_MAX=0; 
        dSig_ALW=0;
		bOK = TRUE;
	}
};
struct T_CON_CS_CH	  // ElemK.
{
    CON_CS dataBase[4];	    // 0=I-Mymax, 1=I-Mymin, 2=J-Mymax, 3=J-Mymin.
    void Initialize()
    {
        for(int i=0; i<4; i++)
            dataBase[i].Initialize();
    }
};
struct T_CON_CS_CH_UNIT	 
{
	T_CON_CS_CH_UNIT()
	{
        dSig_T 	  = D_UNITSYS_BASE_STRESS; 
        dSig_B 	  = D_UNITSYS_BASE_STRESS; 
        dSig_TL   = D_UNITSYS_BASE_STRESS; 
        dSig_BL   = D_UNITSYS_BASE_STRESS; 
        dSig_TR   = D_UNITSYS_BASE_STRESS; 
        dSig_BR   = D_UNITSYS_BASE_STRESS; 
        dSig_MAX  = D_UNITSYS_BASE_STRESS; 
        dSig_ALW  = D_UNITSYS_BASE_STRESS;     
    }
    int dSig_T;   
    int dSig_B;  
    int dSig_TL;	
    int dSig_BL;	
    int dSig_TR;	
    int dSig_BR;	
    int dSig_MAX; 
    int dSig_ALW;
};


struct COLUMN_STRENGTH
{
	BOOL bCHK;		//角뤠셕炬돨깃街
	UINT Lcomk;//駱炬코제莉북돨ID  
	int itype;//駱炬뵐潼돨잚謹 ( FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11 )
                        //               ( FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12) 	
	double dKN;	   
    double dN;   
	BOOL bOK;
	void Initialize()
	{ 
		bCHK = 0;
		Lcomk=0;
		itype=0;
	  dKN=0;
		dN=0;
		bOK = TRUE;
	}
};
struct T_COLUMN_STRENGTH_CH     // ElemK
{
    COLUMN_STRENGTH dataBase[12];  // 0=I-Fxmax, 1=I-Fxmin, 2=I-Mymax, 3=I-Mymin, 4=I-Mzmax, 5=I-Mzmin,
	                               // 6=J-Fxmax, 7=J-Fxmin, 8=J-Mymax, 9=J-Mymin,10=J-Mzmax,11=J-Mzmin,
    void Initialize()
    {
        for(int i=0; i<12; i++)
            dataBase[i].Initialize();
    }
};
struct T_STRENGTH_CH_UNIT	  
{
	T_STRENGTH_CH_UNIT()
	{
        dKN= D_UNITSYS_BASE_FORCE; 
	  dN = D_UNITSYS_BASE_FORCE; 
	}
    int	dKN;
	int	dN;
};

struct TENDON_CS
{
	BOOL bCHK;		//角뤠셕炬돨깃街
	UINT Lcomk;   //駱炬코제莉북돨ID  
	int itype;    //駱炬뵐潼돨잚謹 ( FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11 )
                                //               ( FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12) 
	int iStage; //쌓뙈
	int iTendon;//먼監뵀
	int iElem;//데禱뵀
	int iPos;//쌔충貫零
    double dSigP;	   
    double dSig_ALW;  
	BOOL bOK;
	void Initialize()
	{ 
		bCHK = 0;
		Lcomk=0;
		itype=0;
	  dSigP=0;
		dSig_ALW=0;
		iStage = iTendon = iElem = iPos = 0;
		bOK = TRUE;
	}
};
struct T_TENDON_CS_CH	// ElemK.
{
    TENDON_CS dataBase[2];	//    0=I  1=J
    void Initialize()
    {
        for(int i=0; i<2; i++)
            dataBase[i].Initialize();
    }
};
struct T_TENDON_CS_CH_UNIT		
{
	T_TENDON_CS_CH_UNIT()
	{
		dSigP    = D_UNITSYS_BASE_STRESS; 
		dSig_ALW = D_UNITSYS_BASE_STRESS; 
	}
	int	dSigP;
	int dSig_ALW;
};

//////////////////////////////////////////////////////////////////////////

//============================================================================
// For AASHTO-LRFD08
//============================================================================

//============================================================================
// 1. Stresses in concrete during construction stage (Before Long Term Losses)
//============================================================================

struct T_FYSG_AASHTO_BASE_856
{
	BOOL bCHK;		// Should perform check? [Unit=None]
	UINT StagK;		// Construction stage number [Unit=None]
	
	// Stresses during construction stage
	double dFT;		//[Unit=Stress]
	double dFB;		//[Unit=Stress]
	double dFTL;	//[Unit=Stress]
	double dFBL;	//[Unit=Stress]
	double dFTR;	//[Unit=Stress]
	double dFBR;	//[Unit=Stress]
	double dFMAX;	//[Unit=Stress]
	
    /*
	// Stress due to prestress only
	double dFT_PS;	//[Unit=Stress]
	double dFB_PS;	//[Unit=Stress]
	double dFTL_PS;	//[Unit=Stress]
	double dFBL_PS;	//[Unit=Stress]
	double dFTR_PS;	//[Unit=Stress]
	double dFBR_PS;	//[Unit=Stress]
	double dFMAX_PS;//[Unit=Stress]

	// Stress due to service load effects 
	double dFT_SL;	//[Unit=Stress]
	double dFB_SL;	//[Unit=Stress]
	double dFTL_SL;	//[Unit=Stress]
	double dFBL_SL;	//[Unit=Stress]
	double dFTR_SL;	//[Unit=Stress]
	double dFBR_SL;	//[Unit=Stress]
	double dFMAX_SL;//[Unit=Stress]
    */
	
	double dALW;	//[Unit=Stress]
    double dTtfc; //[Unit=Force] 인장영역에서 콘크리트 인장력(Total tensile Force in conrete)
    double dAct;  //[Unit=Area] 인장영역 면적
    double dTtfs; //[Unit=Force] 인장영역에서 철근 인장력(Total tensile Force in steel) 
    double dAst;  //[Unit=Area] 인장영력에서 철근 면적
    double dFs;   //[Unit=Stress] 철근 응력

	BOOL bOK;
	
	void Initialize()
	{
		bCHK = 0;
		StagK = 0;
		dFT = dFB = dFTL = dFBL = dFTR = dFBR = dFMAX = 0.0;
        dTtfc = dAct = dTtfs = dAst = dFs = 0.0;
		//dFT_PS = dFB_PS = dFTL_PS = dFBL_PS = dFTR_PS = dFBR_PS = dFMAX_PS = 0;
		//dFT_SL = dFB_SL = dFTL_SL = dFBL_SL = dFTR_SL = dFBR_SL = dFMAX_SL = 0;
		dALW = 0;
		bOK = TRUE;
	}
    T_FYSG_AASHTO_BASE_856() { Initialize(); }
};

struct T_FYSG_AASHTO_BASE
{
	BOOL bCHK;		// Should perform check? [Unit=None]
	UINT StagK;		// Construction stage number [Unit=None]
	
	// Stresses during construction stage
	double dFT;		//[Unit=Stress]
	double dFB;		//[Unit=Stress]
	double dFTL;	//[Unit=Stress]
	double dFBL;	//[Unit=Stress]
	double dFTR;	//[Unit=Stress]
	double dFBR;	//[Unit=Stress]
	double dFMAX;	//[Unit=Stress]
	
    /*
	// Stress due to prestress only
	double dFT_PS;	//[Unit=Stress]
	double dFB_PS;	//[Unit=Stress]
	double dFTL_PS;	//[Unit=Stress]
	double dFBL_PS;	//[Unit=Stress]
	double dFTR_PS;	//[Unit=Stress]
	double dFBR_PS;	//[Unit=Stress]
	double dFMAX_PS;//[Unit=Stress]

	// Stress due to service load effects 
	double dFT_SL;	//[Unit=Stress]
	double dFB_SL;	//[Unit=Stress]
	double dFTL_SL;	//[Unit=Stress]
	double dFBL_SL;	//[Unit=Stress]
	double dFTR_SL;	//[Unit=Stress]
	double dFBR_SL;	//[Unit=Stress]
	double dFMAX_SL;//[Unit=Stress]
    */
	
	double dALW;	//[Unit=Stress]
    double dTtfc; //[Unit=Force] 인장영역에서 콘크리트 인장력(Total tensile Force in conrete)
    double dAct;  //[Unit=Area] 인장영역 면적
    double dTtfs; //[Unit=Force] 인장영역에서 철근 인장력(Total tensile Force in steel) 
    double dAst;  //[Unit=Area] 인장영력에서 철근 면적
    double dFs;   //[Unit=Stress] 철근 응력
    double dfci;  //[Unit=Stress] 초기 압축강도(시공단계 압축강도)

	BOOL bOK;
	
	void Initialize()
	{
		bCHK = 0;
		StagK = 0;
		dFT = dFB = dFTL = dFBL = dFTR = dFBR = dFMAX = 0.0;
        dTtfc = dAct = dTtfs = dAst = dFs = 0.0;
		//dFT_PS = dFB_PS = dFTL_PS = dFBL_PS = dFTR_PS = dFBR_PS = dFMAX_PS = 0;
		//dFT_SL = dFB_SL = dFTL_SL = dFBL_SL = dFTR_SL = dFBR_SL = dFMAX_SL = 0;
		dALW = 0.0;
        dfci = 0.0;
		bOK = TRUE;
	}
    T_FYSG_AASHTO_BASE() { Initialize(); }

    void Convert856(const T_FYSG_AASHTO_BASE_856 &rData)
    {
        bCHK  = rData.bCHK ;
        StagK = rData.StagK;
        dFT   = rData.dFT  ;
        dFB   = rData.dFB  ;
        dFTL  = rData.dFTL ;
        dFBL  = rData.dFBL ;
        dFTR  = rData.dFTR ;
        dFBR  = rData.dFBR ;
        dFMAX = rData.dFMAX;
        dALW  = rData.dALW ;
        dTtfc = rData.dTtfc;
        dAct  = rData.dAct ;
        dTtfs = rData.dTtfs;
        dAst  = rData.dAst ;
        dFs   = rData.dFs  ;
        bOK   = rData.bOK  ;
        dfci  = 0.0;
    }
};

struct T_FYSG_AASHTO_856	// ElemK.
{
    T_FYSG_AASHTO_BASE_856 FysgBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            FysgBase[i].Initialize();
    }
    T_FYSG_AASHTO_856() { Initialize(); }
};

struct T_FYSG_AASHTO	// ElemK.
{
	T_FYSG_AASHTO_BASE FysgBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<4; i++)
			FysgBase[i].Initialize();
	}
    T_FYSG_AASHTO() { Initialize(); }

    void Convert856(const T_FYSG_AASHTO_856 &rData)
    {
        for(int i=0; i<4; i++)
            FysgBase[i].Convert856(rData.FysgBase[i]);
    }
};

struct T_FYSG_AASHTO_UNIT
{
	T_FYSG_AASHTO_UNIT()
	{
		dFT		= D_UNITSYS_BASE_STRESS;
		dFB		= D_UNITSYS_BASE_STRESS;
		dFTL	= D_UNITSYS_BASE_STRESS;
		dFBL	= D_UNITSYS_BASE_STRESS;
		dFTR	= D_UNITSYS_BASE_STRESS;
		dFBR	= D_UNITSYS_BASE_STRESS;
		dFMAX	= D_UNITSYS_BASE_STRESS;

        /*
		dFT_PS	= D_UNITSYS_BASE_STRESS;
		dFB_PS	= D_UNITSYS_BASE_STRESS;
		dFTL_PS = D_UNITSYS_BASE_STRESS;
		dFBL_PS = D_UNITSYS_BASE_STRESS;
		dFTR_PS = D_UNITSYS_BASE_STRESS;
		dFBR_PS = D_UNITSYS_BASE_STRESS;
		dFMAX_PS= D_UNITSYS_BASE_STRESS;

		dFT_SL	= D_UNITSYS_BASE_STRESS;	
		dFB_SL	= D_UNITSYS_BASE_STRESS;	
		dFTL_SL = D_UNITSYS_BASE_STRESS;
		dFBL_SL = D_UNITSYS_BASE_STRESS;
		dFTR_SL = D_UNITSYS_BASE_STRESS;
		dFBR_SL = D_UNITSYS_BASE_STRESS;
		dFMAX_SL= D_UNITSYS_BASE_STRESS;
        */

		dALW	= D_UNITSYS_BASE_STRESS;	

        dALW  = D_UNITSYS_BASE_STRESS;
        dTtfc = D_UNITSYS_BASE_FORCE;
        dAct  = D_UNITSYS_BASE_AREA;
        dTtfs = D_UNITSYS_BASE_FORCE;
        dAst  = D_UNITSYS_BASE_AREA;
        dFs   = D_UNITSYS_BASE_STRESS;
	}
	int dFT;
	int dFB;
	int dFTL;
	int dFBL;
	int dFTR;
	int dFBR;

    /*
	int dFT_PS;
	int dFB_PS;
	int dFTL_PS;
	int dFBL_PS;
	int dFTR_PS;
	int dFBR_PS;
	int dFMAX_PS;

	int dFT_SL;	
	int dFB_SL;	
	int dFTL_SL;
	int dFBL_SL;
	int dFTR_SL;
	int dFBR_SL;
	int dFMAX_SL;
    */

	int dALW;
 	int dFMAX;
    int  dTtfc;
    int  dAct ;
    int  dTtfs;
    int  dAst ;
    int  dFs  ;
 
};

//==================================================================================
// 2. Stresses in concrete during service load combinations (After Long Term Losses)
//==================================================================================

struct T_FYCM_AASHTO_BASE
{  
	BOOL bCHK;		
	UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iKind;       // enum AASHTO_LCOMTYPE
	double dFT;		//[Unit=Stress]
	double dFB;		//[Unit=Stress]
	double dFTL;	//[Unit=Stress]
	double dFBL;	//[Unit=Stress]
	double dFTR;	//[Unit=Stress]
	double dFBR;	//[Unit=Stress]
	double dFMAX;	//[Unit=Stress]
	double dALW;	//[Unit=Stress]
	
	// Stress due to prestress only
	double dFT_PS  ;	//[Unit=Stress]
	double dFB_PS  ;	//[Unit=Stress]
	double dFTL_PS ;	//[Unit=Stress]
	double dFBL_PS ;	//[Unit=Stress]
	double dFTR_PS ;	//[Unit=Stress]
	double dFBR_PS ;	//[Unit=Stress]
	double dFMAX_PS;//[Unit=Stress]

 /* 
	// Stress due to service load effects 
	double dFT_SL;	//[Unit=Stress]
	double dFB_SL;	//[Unit=Stress]
	double dFTL_SL;	//[Unit=Stress]
	double dFBL_SL;	//[Unit=Stress]
	double dFTR_SL;	//[Unit=Stress]
	double dFBR_SL;	//[Unit=Stress]
	double dFMAX_SL;//[Unit=Stress]
    */

    double dRatio;
	BOOL bOK;
    BOOL bLTerm;

	void Initialize()
	{
		bCHK = 0;
		dFT = dFB = 0.0;
		dFTL = dFBL = dFTR = dFBR = dFMAX = dALW = 0;
		dFT_PS = dFB_PS = dFTL_PS = dFBL_PS = dFTR_PS = dFBR_PS = dFMAX_PS = 0;
		//dFT_SL = dFB_SL = dFTL_SL = dFBL_SL = dFTR_SL = dFBR_SL = dFMAX_SL = 0;
        dRatio = 0.0;
		bOK = TRUE;
        bLTerm = FALSE;
	}
    T_FYCM_AASHTO_BASE() { Initialize(); }
};

struct T_FYCM_AASHTO	// ElemK.
{
	T_FYCM_AASHTO_BASE FycmBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<4; i++) FycmBase[i].Initialize();
	}
    T_FYCM_AASHTO() { Initialize(); }	
};

struct T_FYCM_AASHTO_UNIT
{
	T_FYCM_AASHTO_UNIT()
	{
		dFT		= D_UNITSYS_BASE_STRESS;		
		dFB		= D_UNITSYS_BASE_STRESS;		
		dFTL	= D_UNITSYS_BASE_STRESS;
		dFBL	= D_UNITSYS_BASE_STRESS;
		dFTR	= D_UNITSYS_BASE_STRESS;	
		dFBR	= D_UNITSYS_BASE_STRESS;
		dFMAX	= D_UNITSYS_BASE_STRESS;
		dALW	= D_UNITSYS_BASE_STRESS;	

		dFT_PS	= D_UNITSYS_BASE_STRESS;
		dFB_PS	= D_UNITSYS_BASE_STRESS;	
		dFTL_PS	= D_UNITSYS_BASE_STRESS;	
		dFBL_PS	= D_UNITSYS_BASE_STRESS;	
		dFTR_PS	= D_UNITSYS_BASE_STRESS;	
		dFBR_PS	= D_UNITSYS_BASE_STRESS;	
		dFMAX_PS= D_UNITSYS_BASE_STRESS;

        /*
		dFT_SL	= D_UNITSYS_BASE_STRESS;
		dFB_SL	= D_UNITSYS_BASE_STRESS;	
		dFTL_SL	= D_UNITSYS_BASE_STRESS;	
		dFBL_SL	= D_UNITSYS_BASE_STRESS;	
		dFTR_SL	= D_UNITSYS_BASE_STRESS;	
		dFBR_SL	= D_UNITSYS_BASE_STRESS;	
		dFMAX_SL= D_UNITSYS_BASE_STRESS;
        */
	}

	int dFT;		
	int dFB;		
	int dFTL;	
	int dFBL;	
	int dFTR;	
	int dFBR;	
	int dFMAX;	
	int dALW;	

	int dFT_PS;	
	int dFB_PS;	
	int dFTL_PS;	
	int dFBL_PS;	
	int dFTR_PS;	
	int dFBR_PS;	
	int dFMAX_PS;

    /*
	int dFT_SL;	
	int dFB_SL;	
	int dFTL_SL;	
	int dFBL_SL;	
	int dFTR_SL;
	int dFBR_SL;	
	int dFMAX_SL;
    */
};

//==================================================================================
// Tensile stresses in prestressing tendons
//==================================================================================

struct T_FYPC_AASHTO_856	// TdnaK.
{
    BOOL bCHK;	
    int nLoadType;  // 0=Pre-tension, 1=Post-tension 

    double dFDL1;	// Pre-Tensioning: Stress prior to transfer. Jacking stress. [Unit=Stress]
    // Post-Tensioning: Stress prior to seating. Jacking stress. [Unit=Stress]

    double dFDL2;	// Pre-Tensioning: Not used. [Unit=Stress]		
    // Post-Tensioning: Stress at anchorage and couplers immediately after anchor set. [Unit=Stress]

    double dFDL3;	// Pre-Tensioning: Not used [Unit=Stress]
    // Post-Tensioning: Away from Anchorage and couplers immediately after anchor set. [Unit=stress]

    double dFLL1;	// Stress at service limit after all losses. [Unit=Stress]

    double dFLL2;	// Stress at strength or extreme event limit state. [Unit=Stress]

    double dAFDL1;	// Pre-Tensioning: Allowable stress prior to transfer. Allowable jacking stress. [Unit=Stress]
    // Post-Tensioning: Allowable stress prior to seating. Allowable jacking stress. [Unit=Stress]

    double dAFDL2;	// Pre-Tensioning: Not used. [Unit=Stress]		
    // Post-Tensioning: Allowable stress at anchorage and couplers immediately after anchor set. [Unit=Stress]

    double dAFDL3;	// Pre-Tensioning: Not used. [Unit=Stress]		
    // Post-Tensioning: Allowable stress at anchorage and couplers immediately after anchor set. [Unit=Stress]

    double dAFLL1; 	// Stress at service limit after all losses. [Unit=Stress]

    double dAFLL2;  // Stress at strength or extreme event limit state. [Unit=Stress]

    BOOL bOK;

    void Initialize()
    {
        bCHK = FALSE;
        nLoadType = 0; 
        dFDL1 = dFDL2 = dFDL3 = dFLL1 = dFLL2 = 0.0;
        dAFDL1 = dAFDL2 = dAFDL3 = dAFLL1 = dAFLL2 = 0.0;
        bOK = TRUE;
    }
    T_FYPC_AASHTO_856() { Initialize(); }
};

struct T_FYPC_AASHTO	// TdnaK.
{
	BOOL bCHK;	
    int nLoadType;  // 0=Pre-tension, 1=Post-tension 
		  
	double dFDL1;	// Pre-Tensioning: Stress prior to transfer. Jacking stress. [Unit=Stress]
					// Post-Tensioning: Stress prior to seating. Jacking stress. [Unit=Stress]

	double dFDL2;	// Pre-Tensioning: Not used. [Unit=Stress]		
					// Post-Tensioning: Stress at anchorage and couplers immediately after anchor set. [Unit=Stress]

	double dFDL3;	// Pre-Tensioning: Not used [Unit=Stress]
					// Post-Tensioning: Away from Anchorage and couplers immediately after anchor set. [Unit=stress]

	double dFLL1;	// Stress at service limit after all losses. [Unit=Stress]
	
	double dFLL2;	// Stress at strength or extreme event limit state. [Unit=Stress]

	double dAFDL1;	// Pre-Tensioning: Allowable stress prior to transfer. Allowable jacking stress. [Unit=Stress]
					// Post-Tensioning: Allowable stress prior to seating. Allowable jacking stress. [Unit=Stress]

	double dAFDL2;	// Pre-Tensioning: Not used. [Unit=Stress]		
					// Post-Tensioning: Allowable stress at anchorage and couplers immediately after anchor set. [Unit=Stress]

	double dAFDL3;	// Pre-Tensioning: Not used. [Unit=Stress]		
					// Post-Tensioning: Allowable stress at anchorage and couplers immediately after anchor set. [Unit=Stress]

	double dAFLL1; 	// Stress at service limit after all losses. [Unit=Stress]
	
	double dAFLL2;  // Stress at strength or extreme event limit state. [Unit=Stress]

    int nElemFDL1;
    int nElemFDL2;
    int nElemFLL1;
    int nPartFDL1;
    int nPartFDL2;
    int nPartFLL1;
    int nStage;
    BOOL bOK;

	void Initialize()
	{
		bCHK = FALSE;
        nLoadType = 0; 
		dFDL1 = dFDL2 = dFDL3 = dFLL1 = dFLL2 = 0.0;
		dAFDL1 = dAFDL2 = dAFDL3 = dAFLL1 = dAFLL2 = 0.0;
        nElemFDL1 = nElemFDL2 = nElemFLL1 = 0;
        nPartFDL1 = nPartFDL2 = nPartFLL1 = 0;
        nStage    = 0;
		bOK = TRUE;
	}
    T_FYPC_AASHTO() { Initialize(); }

    void Convert856(const T_FYPC_AASHTO_856& crData)
    {
        bCHK	    = crData.bCHK     ;  
        nLoadType = crData.nLoadType ;
        dFDL1     = crData.dFDL1 ;
        dFDL2     = crData.dFDL2 ;
        dFDL3     = crData.dFDL3 ;
        dFLL1     = crData.dFLL1 ;
        dFLL2     = crData.dFLL2 ;
        dAFDL1    = crData.dAFDL1;
        dAFDL2    = crData.dAFDL2;
        dAFDL3    = crData.dAFDL3;
        dAFLL1    = crData.dAFLL1;
        dAFLL2    = crData.dAFLL2;
        bOK       = crData.bOK   ;

        nElemFDL1 = nElemFDL2 = nElemFLL1 = 0;
        nPartFDL1 = nPartFDL2 = nPartFLL1 = 0;
        nStage    = 0;
    }

};

struct T_FYPC_AASHTO_UNIT
{
	T_FYPC_AASHTO_UNIT()
	{
		dFDL1	= D_UNITSYS_BASE_STRESS;
		dFDL2	= D_UNITSYS_BASE_STRESS;
		dFDL3	= D_UNITSYS_BASE_STRESS;
		dFLL1	= D_UNITSYS_BASE_STRESS;
		dFLL2	= D_UNITSYS_BASE_STRESS;
		dAFDL1	= D_UNITSYS_BASE_STRESS;
		dAFDL2	= D_UNITSYS_BASE_STRESS;
		dAFDL3	= D_UNITSYS_BASE_STRESS;
		dAFLL1	= D_UNITSYS_BASE_STRESS;
		dAFLL2	= D_UNITSYS_BASE_STRESS;
	}

	int dFDL1;
	int dFDL2;
	int dFDL3;
	int dFLL1;
	int dFLL2;
	int dAFDL1;
	int dAFDL2;
	int dAFDL3;
	int dAFLL1;
	int dAFLL2;
};

struct T_FPMS_AASHTO_BASE  
{
    BOOL bCHK;		//설계결과유무 
    UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iKind;       // enum AASHTO_LCOMTYPE
    double dFP1;	//1위치 주응력도		[Unit=Stress]
    double dFP2;	//2위치 주응력도		[Unit=Stress]
    double dFP3;	//3위치 주응력도		[Unit=Stress]
    double dFP4;	//4위치 주응력도		[Unit=Stress]
    double dFP5;	//5위치 주응력도		[Unit=Stress]
    double dFP6;	//6위치 주응력도		[Unit=Stress]
    double dFP7;	//7위치 주응력도		[Unit=Stress]
    double dFP8;	//8위치 주응력도		[Unit=Stress]
    double dFP9;	//9위치 주응력도		[Unit=Stress]
    double dFP10;	//10위치 주응력도		[Unit=Stress]
    double dFMAX; // MAX/MIN 주응력도	[Unit=Stress]
    double dAFP;  // 허용 주응력도    [Unit=Stress]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;    
        dFP1 = dFP2 = dFP3 = dFP4 = dFP5 = dFP6 = dFP7 = dFP8 = dFP9= dFP10 = dFMAX = dAFP = 0;
        bOK = TRUE;
    }
    T_FPMS_AASHTO_BASE() { Initialize(); }
};

struct T_FPMS_AASHTO	// ElemK.
{
	T_FPMS_AASHTO_BASE FpmsBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<4; i++)
			FpmsBase[i].Initialize();
	}
    T_FPMS_AASHTO() { Initialize(); }
};

struct T_FPMS_AASHTO_UNIT
{
    T_FPMS_AASHTO_UNIT()
    {
        dFP1 = D_UNITSYS_BASE_STRESS;
        dFP2 = D_UNITSYS_BASE_STRESS;
        dFP3 = D_UNITSYS_BASE_STRESS;
        dFP4 = D_UNITSYS_BASE_STRESS;
        dFP5 = D_UNITSYS_BASE_STRESS;
        dFP6 = D_UNITSYS_BASE_STRESS;
        dFP7 = D_UNITSYS_BASE_STRESS;
        dFP8 = D_UNITSYS_BASE_STRESS;
        dFP9 = D_UNITSYS_BASE_STRESS;
        dFP10= D_UNITSYS_BASE_STRESS;
        dFMAX= D_UNITSYS_BASE_STRESS;
        dAFP = D_UNITSYS_BASE_STRESS; 
    }
    int dFP1;
    int dFP2;
    int dFP3;
    int dFP4;
    int dFP5;
    int dFP6;
    int dFP7;
    int dFP8;
    int dFP9;
    int dFP10;
    int dFMAX;
    int dAFP;  
};

//==================================================================================
// 
//==================================================================================
struct T_BDCY_AASHTO_BASE_856
{
    BOOL bCHK;
    UINT LcomK;
    int iMax;
    int iKind;      // enum AASHTO_LCOMTYPE
    double dForceTP[6];
    double dMuy;	  // [Unit=Moment]
    double drMuy;		// Factored moment of resistance [Unit=Moment]
    double dAps;    // [Unit=Area]
    double ddp;			// [Unit =Length]
    double dAs;			// [Unit=Area]
    double dds;			// [Unit=Length]
    double db;			// [Unit=Length]
    double dc;			// [Unit=Length]
    double dfps; 		// [Unit = stress]
    double da;			// [Unit=Length]
    double dfr;			// [Unit=Stress]
    double dfcpe;		// [Unit=Stress]
    double dMdnc;   // [Unit=Moment]
    double dSc;			// [Unit=Section Modulus] //Length^3
    double dSnc;    // [Unit=Section Modulus]	//Length^3
    double dMny;		// Nominal moment of resistance [Unit=Moment]
    double dMcry;		// Cracking moment [Unit=Moment]
    double dFacM;	  // [Unit=Moment]
    double dFacMcr;	// [Unit=Moment]
    double dpMn_12Mcr_Rat; // [Unit=None]
    double dpMn_133Mcr_Rat; // [Unit=None]

    double dAp_unbonded; // [Unit=Area]
    double dAp_bonded; // [Unit=Area]
    double ddp_unbonded; // [Unit=Length]
    double ddp_bonded; // [Unit=Length]
    double dAsc; // [Unit=Area]
    double ddsc; // [Unit=Length]
    double dbw; // [Unit=Length]
    double dhf; // [Unit=Length]
    double dBeta1; // [Unit= None];
    double dK; // [Unit=None]
    double dfpe; // [Unit=Stress]
    double dle; // [Unit=Length]
    double dPhi; // [Unit=None]

    double dfc; // [Unit=Stress]
    double dfci; // [Unit=Stress]
    double dfy; // [Unit=Stress]
    double dfyc; // [Unit=Stress]
    double dfpu_bonded; // [Unit=Stress]
    double dfpu_unbonded; // [Unit=Stress]
    double dfpy_bonded; // [Unit=Stress]
    double dfpy_unbonded; // [Unit=Stress]

    double dMf_Mr_Rat;     // [Unit=None]
    double dMr_12Mcr_Rat;  // [Unit=None]
    double dc_d_Rat;        // [Unit=None]

    BOOL bOK;
    void Initialize()
    {
        bCHK = 0;
        LcomK=0;
        iMax=iKind=0;
        memset(dForceTP, 0, sizeof(dForceTP));
        dMuy = drMuy = dMny = dMcry = 0.0;
        dFacM = dFacMcr = 0.0;
        dpMn_12Mcr_Rat = 0.0;
        dpMn_133Mcr_Rat = 0.0;		
        dAps = ddp = dAs = dds = db = dc=0.0;
        dfps = da = dfr = dfcpe = dMdnc = dSc = dSnc = 0.0;
        dAp_unbonded = dAp_bonded = ddp_unbonded = ddp_bonded = dAsc = ddsc = dbw = dhf = dBeta1 = dK = dfpe = dle = dPhi = 0.0;
        dfc = dfci = dfy = dfyc = dfpu_bonded = dfpu_unbonded = dfpy_bonded = dfpy_unbonded = 0.0;
        dMf_Mr_Rat = dMr_12Mcr_Rat = dc_d_Rat = 0.0;
        bOK = 0;
    }
    T_BDCY_AASHTO_BASE_856() { Initialize(); }
};

struct T_BDCY_AASHTO_BASE
{
	BOOL bCHK;
    UINT LcomK;
    int iMax;
    int iKind;      // enum AASHTO_LCOMTYPE
    double dForceTP[6];
	double dMuy;	  // [Unit=Moment]
	double drMuy;		// Factored moment of resistance [Unit=Moment]
	double dAps;    // [Unit=Area]
	double ddp;			// [Unit =Length]
	double dAs;			// [Unit=Area]
	double dds;			// [Unit=Length]
	double db;			// [Unit=Length]
	double dc;			// [Unit=Length]
	double dfps; 		// [Unit = stress]
	double da;			// [Unit=Length]
	double dfr;			// [Unit=Stress]
	double dfcpe;		// [Unit=Stress]
	double dMdnc;   // [Unit=Moment]
	double dSc;			// [Unit=Section Modulus] //Length^3
	double dSnc;    // [Unit=Section Modulus]	//Length^3
	double dMny;		// Nominal moment of resistance [Unit=Moment]
	double dMcry;		// Cracking moment [Unit=Moment]
	double dFacM;	  // [Unit=Moment]
	double dFacMcr;	// [Unit=Moment]
    double dpMn_12Mcr_Rat; // [Unit=None]
    double dpMn_133Mcr_Rat; // [Unit=None]
	
	double dAp_unbonded; // [Unit=Area]
	double dAp_bonded; // [Unit=Area]
	double ddp_unbonded; // [Unit=Length]
	double ddp_bonded; // [Unit=Length]
	double dAsc; // [Unit=Area]
	double ddsc; // [Unit=Length]
	double dbw; // [Unit=Length]
	double dhf; // [Unit=Length]
	double dBeta1; // [Unit= None];
	double dK; // [Unit=None]
	double dfpe; // [Unit=Stress]
	double dle; // [Unit=Length]
	double dPhi; // [Unit=None]

	double dfc; // [Unit=Stress]
	double dfci; // [Unit=Stress]
	double dfy; // [Unit=Stress]
	double dfyc; // [Unit=Stress]
	double dfpu_bonded; // [Unit=Stress]
	double dfpu_unbonded; // [Unit=Stress]
	double dfpy_bonded; // [Unit=Stress]
	double dfpy_unbonded; // [Unit=Stress]

    double dMf_Mr_Rat;     // [Unit=None]
    double dMr_12Mcr_Rat;  // [Unit=None]
    double dc_d_Rat;        // [Unit=None]
    double dNeuTendonRat;

	BOOL bOK;
    BOOL bNeuTendonRat;
	void Initialize()
	{
		bCHK = 0;
        LcomK=0;
        iMax=iKind=0;
        memset(dForceTP, 0, sizeof(dForceTP));
		dMuy = drMuy = dMny = dMcry = 0.0;
        dFacM = dFacMcr = 0.0;
        dpMn_12Mcr_Rat = 0.0;
        dpMn_133Mcr_Rat = 0.0;		
        dAps = ddp = dAs = dds = db = dc=0.0;
        dfps = da = dfr = dfcpe = dMdnc = dSc = dSnc = 0.0;
	  dAp_unbonded = dAp_bonded = ddp_unbonded = ddp_bonded = dAsc = ddsc = dbw = dhf = dBeta1 = dK = dfpe = dle = dPhi = 0.0;
	  dfc = dfci = dfy = dfyc = dfpu_bonded = dfpu_unbonded = dfpy_bonded = dfpy_unbonded = 0.0;
        dMf_Mr_Rat = dMr_12Mcr_Rat = dc_d_Rat = 0.0;
        dNeuTendonRat = 0.0;
        bNeuTendonRat = 0;
		bOK = 0;
	}
    T_BDCY_AASHTO_BASE() { Initialize(); }

    void Convert856(const T_BDCY_AASHTO_BASE_856 &rData)
    {
        bCHK    = rData.bCHK;
        LcomK   = rData.LcomK;
        iMax    = rData.iMax;
        iKind   = rData.iKind;
        for(int i=0; i<6; ++i) dForceTP[i] = rData.dForceTP[i];
        dMuy            = rData.dMuy           ;
        drMuy           = rData.drMuy          ;
        dMny            = rData.dMny           ;
        dMcry           = rData.dMcry          ;
        dFacM           = rData.dFacM          ;
        dFacMcr         = rData.dFacMcr        ;
        dpMn_12Mcr_Rat  = rData.dpMn_12Mcr_Rat ;
        dpMn_133Mcr_Rat = rData.dpMn_133Mcr_Rat;		
        dAps            = rData.dAps           ;
        ddp             = rData.ddp            ;
        dAs             = rData.dAs            ;
        dds             = rData.dds            ;
        db              = rData.db             ;
        dc              = rData.dc             ;
        dfps            = rData.dfps           ;
        da              = rData.da             ;
        dfr             = rData.dfr            ;
        dfcpe           = rData.dfcpe          ;
        dMdnc           = rData.dMdnc          ;
        dSc             = rData.dSc            ;
        dSnc            = rData.dSnc           ;
        dAp_unbonded    = rData.dAp_unbonded   ;
        dAp_bonded      = rData.dAp_bonded     ;
        ddp_unbonded    = rData.ddp_unbonded   ;
        ddp_bonded      = rData.ddp_bonded     ;
        dAsc            = rData.dAsc           ;
        ddsc            = rData.ddsc           ;
        dbw             = rData.dbw            ;
        dhf             = rData.dhf            ;
        dBeta1          = rData.dBeta1         ;
        dK              = rData.dK             ;
        dfpe            = rData.dfpe           ;
        dle             = rData.dle            ;
        dPhi            = rData.dPhi           ;
        dfc             = rData.dfc            ;
        dfci            = rData.dfci           ;
        dfy             = rData.dfy            ;
        dfyc            = rData.dfyc           ;
        dfpu_bonded     = rData.dfpu_bonded    ;
        dfpu_unbonded   = rData.dfpu_unbonded  ;
        dfpy_bonded     = rData.dfpy_bonded    ;
        dfpy_unbonded   = rData.dfpy_unbonded  ;
        dMf_Mr_Rat      = rData.dMf_Mr_Rat     ;
        dMr_12Mcr_Rat   = rData.dMr_12Mcr_Rat  ;
        dc_d_Rat        = rData.dc_d_Rat       ;
        bOK             = rData.bOK            ;

        dNeuTendonRat = 0.0;
        bNeuTendonRat = 0;
    }
};

struct T_BDCY_AASHTO_856	// ElemK.
{
    T_BDCY_AASHTO_BASE_856 BdcyBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            BdcyBase[i].Initialize();
    }
    T_BDCY_AASHTO_856() { Initialize(); }
};

struct T_BDCY_AASHTO	// ElemK.
{
	T_BDCY_AASHTO_BASE BdcyBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<4; i++)
			BdcyBase[i].Initialize();
	}
    T_BDCY_AASHTO() { Initialize(); }

    void Convert856(const T_BDCY_AASHTO_856 &rData)
    {
        for (int i=0; i<4; ++i) BdcyBase[i].Convert856(rData.BdcyBase[i]);
    }
};

struct T_BDCY_AASHTO_UNIT
{
	T_BDCY_AASHTO_UNIT()
	{
        dFtp    = D_UNITSYS_BASE_FORCE;
        dMtp    = D_UNITSYS_BASE_MOMENT;
		dMuy	  = D_UNITSYS_BASE_MOMENT;
		drMuy	  = D_UNITSYS_BASE_MOMENT;
	  dAps    = D_UNITSYS_BASE_AREA;  
	  ddp     = D_UNITSYS_BASE_LENGTH;
	  dAs     = D_UNITSYS_BASE_AREA;	
	  dds     = D_UNITSYS_BASE_LENGTH;
	  db      = D_UNITSYS_BASE_LENGTH;
	  dc      = D_UNITSYS_BASE_LENGTH;
	  dfps    = D_UNITSYS_BASE_STRESS;
	  da      = D_UNITSYS_BASE_LENGTH;
	  dfr     = D_UNITSYS_BASE_STRESS;
	  dfcpe   = D_UNITSYS_BASE_STRESS;
	  dMdnc   = D_UNITSYS_BASE_MOMENT;
	  dSc     = D_UNITSYS_BASE_VOLUME;
	  dSnc    = D_UNITSYS_BASE_VOLUME;
		dMny	  = D_UNITSYS_BASE_MOMENT;
		dMcry	  = D_UNITSYS_BASE_MOMENT;
		dFacM	  = D_UNITSYS_BASE_MOMENT;
		dFacMcr	= D_UNITSYS_BASE_MOMENT;

		dAp_unbonded = D_UNITSYS_BASE_AREA;
		dAp_bonded = D_UNITSYS_BASE_AREA;
		ddp_unbonded  = D_UNITSYS_BASE_LENGTH;
		ddp_bonded  = D_UNITSYS_BASE_LENGTH;
		dAsc = D_UNITSYS_BASE_AREA;
		ddsc  = D_UNITSYS_BASE_LENGTH;
		dbw  = D_UNITSYS_BASE_LENGTH;
		dhf  = D_UNITSYS_BASE_LENGTH;
		dBeta1  = D_UNITSYS_NONE;
		dK = D_UNITSYS_NONE;
		dfpe   = D_UNITSYS_BASE_STRESS;
		dle = D_UNITSYS_BASE_LENGTH;
		dPhi = D_UNITSYS_NONE;

		dfc   = D_UNITSYS_BASE_STRESS;
		dfci   = D_UNITSYS_BASE_STRESS;
		dfy   = D_UNITSYS_BASE_STRESS;
		dfyc   = D_UNITSYS_BASE_STRESS;
		dfpu_bonded   = D_UNITSYS_BASE_STRESS;
		dfpu_unbonded   = D_UNITSYS_BASE_STRESS;
		dfpy_bonded   = D_UNITSYS_BASE_STRESS;
		dfpy_unbonded   = D_UNITSYS_BASE_STRESS;



	}

    int dFtp;
    int dMtp;
	int dMuy;
	int drMuy;
	int dAps;   
	int ddp;		
	int dAs;		
	int dds;		
	int db;			
	int dc;			
	int dfps; 	
	int da;			
	int dfr;		
	int dfcpe;	
	int dMdnc;  
	int dSc;		
	int dSnc;   
	int dMny;
	int dMcry;
	int dFacM;
	int dFacMcr;

	int dAp_unbonded;
	int	dAp_bonded;
	int	ddp_unbonded;
	int	ddp_bonded;
	int dAsc;
	int	ddsc;
	int	dbw;
	int	dhf;
	int	dBeta1;
	int	dK;
	int	dfpe;
	int	dle;
	int	dPhi;

	int	dfc;
	int	dfci;
	int	dfy;
	int	dfyc;
	int	dfpu_bonded;
	int	dfpu_unbonded;
	int	dfpy_bonded;
	int	dfpy_unbonded;

};

//==================================================================================
// Crack check
//==================================================================================

struct T_BDCW_AASHTO_BASE_900
{
    BOOL bCHK;		//[Unit=None]

    UINT LcomK;		//[Unit=None] 
    int iKind;    // enum AASHTO_LCOMTYPE
    int iMax;
    double dFT;		//[Unit=Stress]
    double dFB;		//[Unit=Stress]
    double dFSS;	//[Unit=Stress]

    double ds_use;	//[Unit=Length]
    double ds_max;	//[Unit=Length]

    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iKind=iMax=0;
        dFT = dFB = dFSS = 0;
        ds_use = ds_max = 0;
        bOK = TRUE;
    }

};


struct T_BDCW_AASHTO_BASE
{
	BOOL bCHK;		//[Unit=None]
	
	UINT LcomK;		//[Unit=None] 
    int iKind;    // enum AASHTO_LCOMTYPE
    int iMax;
	double dFT;		//[Unit=Stress]
	double dFB;		//[Unit=Stress]
	double dFSS;	//[Unit=Stress]
	
	double ds_use;	//[Unit=Length]
	double ds_max;	//[Unit=Length]

    BOOL bOK;
	
    double dft_DL;  //[Unit=Stress]
    double dfb_DL;  //[Unit=Stress]
    double dft_LL;  //[Unit=Stress]
    double dfb_LL;  //[Unit=Stress]

    double dFT_Slab;    //[Unit=Stress]
    double dFB_Slab;    //[Unit=Stress]

    double dft_DL_Slab; //[Unit=Stress]
    double dfb_DL_Slab; //[Unit=Stress]
    double dft_LL_Slab; //[Unit=Stress]
    double dfb_LL_Slab; //[Unit=Stress]

    double dPu;
    double dMu;

    double dPuShort;
    double dMuShort;

    double dPuLong;
    double dMuLong;

    double dPu_C;
    double dMu_C;

    double dMdnc;

    double dSig_use;	//[Unit=Stress]
    double dSig_max;	//[Unit=Stress]

	void Initialize()
	{
		bCHK = 0;
		LcomK = 0;
        iKind=iMax=0;
		dFT = dFB = dFSS = 0;
		ds_use = ds_max = 0;
        bOK = TRUE;
        dft_DL = 0.0;
        dfb_DL = 0.0;
        dft_LL = 0.0;
        dfb_LL = 0.0;
        dFT_Slab = 0.0;
        dFB_Slab = 0.0;
        dft_DL_Slab = 0.0;
        dfb_DL_Slab = 0.0;
        dft_LL_Slab = 0.0;
        dfb_LL_Slab = 0.0;
        dPu = 0.0;
        dMu = 0.0;
        dPuShort = 0.0;
        dMuShort = 0.0;
        dPuLong = 0.0;
        dMuLong = 0.0;
        dPu_C = 0.0;
        dMu_C = 0.0;
        dMdnc = 0.0;
        dSig_use = 0.0;
        dSig_max = 0.0;
	}
    T_BDCW_AASHTO_BASE() { Initialize(); }	

    void Convert900(const T_BDCW_AASHTO_BASE_900& crData)
    {
        bCHK    = crData.bCHK;
        LcomK   = crData.LcomK;
        iKind   = crData.iKind;
        iMax    = crData.iMax;
        dFT     = crData.dFT;
        dFB     = crData.dFB;
        dFSS    = crData.dFSS;
        ds_use  = crData.ds_use;
        ds_max  = crData.ds_max;
        bOK     = crData.bOK;

        dft_DL = 0.0;
        dfb_DL = 0.0;
        dft_LL = 0.0;
        dfb_LL = 0.0;
        dFT_Slab = 0.0;
        dFB_Slab = 0.0;
        dft_DL_Slab = 0.0;
        dfb_DL_Slab = 0.0;
        dft_LL_Slab = 0.0;
        dfb_LL_Slab = 0.0;
        dPu = 0.0;
        dMu = 0.0;
        dPuShort = 0.0;
        dMuShort = 0.0;
        dPuLong = 0.0;
        dMuLong = 0.0;
        dPu_C = 0.0;
        dMu_C = 0.0;
        dMdnc = 0.0;
        dSig_use = 0.0;
        dSig_max = 0.0;
    }
};

struct T_BDCW_AASHTO_900	// ElemK.
{
    T_BDCW_AASHTO_BASE_900 BdcwBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for ( int i=0; i<4; i++ )
            BdcwBase[i].Initialize();
    }
    T_BDCW_AASHTO_900() { Initialize(); }
};


struct T_BDCW_AASHTO	// ElemK.
{
	T_BDCW_AASHTO_BASE BdcwBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<4; i++)
			BdcwBase[i].Initialize();
	}
    T_BDCW_AASHTO() { Initialize(); }		

    void Convert900(const T_BDCW_AASHTO_900& rData)
    {
        for ( int i=0; i<4; i++ )
            BdcwBase[i].Convert900(rData.BdcwBase[i]);
    }
};

struct T_BDCW_AASHTO_UNIT
{
	T_BDCW_AASHTO_UNIT()
	{
		dFT = D_UNITSYS_BASE_STRESS;	
		dFB = D_UNITSYS_BASE_STRESS;	
		dFSS= D_UNITSYS_BASE_STRESS;

		ds_use = D_UNITSYS_BASE_LENGTH;
		ds_max = D_UNITSYS_BASE_LENGTH;
        dSig_use = D_UNITSYS_BASE_STRESS;
        dSig_max = D_UNITSYS_BASE_STRESS;

        dft_DL      = D_UNITSYS_BASE_STRESS;
        dfb_DL      = D_UNITSYS_BASE_STRESS;
        dft_LL      = D_UNITSYS_BASE_STRESS;
        dfb_LL      = D_UNITSYS_BASE_STRESS;
        dFT_Slab    = D_UNITSYS_BASE_STRESS;
        dFB_Slab    = D_UNITSYS_BASE_STRESS;
        dft_DL_Slab = D_UNITSYS_BASE_STRESS;
        dfb_DL_Slab = D_UNITSYS_BASE_STRESS;
        dft_LL_Slab = D_UNITSYS_BASE_STRESS;
        dfb_LL_Slab = D_UNITSYS_BASE_STRESS;
        dPu         = D_UNITSYS_BASE_FORCE;
        dMu         = D_UNITSYS_BASE_MOMENT;
        dPuShort    = D_UNITSYS_BASE_FORCE;
        dMuShort    = D_UNITSYS_BASE_MOMENT;
        dPuLong     = D_UNITSYS_BASE_FORCE;
        dMuLong     = D_UNITSYS_BASE_MOMENT;
        dPu_C       = D_UNITSYS_BASE_FORCE;
        dMu_C       = D_UNITSYS_BASE_MOMENT;
        dMdnc       = D_UNITSYS_BASE_MOMENT;
	}

	int dFT;	
	int dFB;	
	int dFSS;
	int ds_use;
	int ds_max;
    int dSig_use;
    int dSig_max;

    int dft_DL;
    int dfb_DL;
    int dft_LL;
    int dfb_LL;
    int dFT_Slab;
    int dFB_Slab;
    int dft_DL_Slab;
    int dfb_DL_Slab;
    int dft_LL_Slab;
    int dfb_LL_Slab;
    int dPu;
    int dMu;
    int dPuShort;
    int dMuShort;
    int dPuLong;
    int dMuLong;
    int dPu_C;
    int dMu_C;
    int dMdnc;

};

//==================================================================================
// Tension rebar required
//==================================================================================

struct T_TBAR_AASHTO_BASE
{
	BOOL bCHK;		//[Unit=None]
	
	UINT LcomK;		//[Unit=None]
    int iKind;    // enum AASHTO_LCOMTYPE
    int iMax;
	double dftop;	// [Unit=Stress]
	double dfbot;	// [Unit=Stress]
	double dREQ;	//[Unit=Area]
	double dMIN;	//[Unit=Area]
	double dUSE;	//[Unit=Area]
	BOOL bOK;
	void Initialize()
	{
		bCHK = 0;
		LcomK = 0;
        iKind=iMax=0;
        dftop = dfbot = 0.0;
		dREQ = dMIN = dUSE = 0.0;
		bOK = TRUE;
	}
    T_TBAR_AASHTO_BASE() { Initialize(); }	
};

struct T_TBAR_AASHTO	// ElemK.
{
	T_TBAR_AASHTO_BASE TbarBase[4];	// 0=I-TOP, 1=I-BOT, 2:J-TOP, 3:J-Bot.
	
	void Initialize()
	{
		for(int i=0; i<4; i++)
			TbarBase[i].Initialize();
	}
    T_TBAR_AASHTO() { Initialize(); }	
};

struct T_TBAR_AASHTO_UNIT
{
	T_TBAR_AASHTO_UNIT()
	{
        dftop = D_UNITSYS_BASE_STRESS;
        dfbot = D_UNITSYS_BASE_STRESS;
		dREQ	= D_UNITSYS_BASE_AREA;
		dMIN	= D_UNITSYS_BASE_AREA;
		dUSE	= D_UNITSYS_BASE_AREA;
	}

    int dftop;
    int dfbot;
	int dREQ;
	int dMIN;
	int dUSE;
};

//==================================================================================
// Shear design
//==================================================================================
struct T_CUMS_AASHTO_BASE_856
{
        BOOL bCHK;			//[Unit=None]
        UINT LcomK;			//[Unit=None]
        int iMax;
        int iKind;      // enum AASHTO_LCOMTYPE
        double dForceTP[6];
        double dNu;     //[Unit=Force]
        double dVu;			//[Unit=Force]
        double dMu;			//[Unit=Moment]
        double dVn;			//[Unit=Force]
        double dPhi;		//[Unit=None]
        double dVc;			//[Unit=Force]
        double dVs;			//[Unit=Force]	
        double dVp;			//[Unit=Force]
        double dPhiVn;		//[Unit=Force]
        double dde;			//[Unit=Length]
        double ddv;			//[Unit=Length]
        double dex;			//[Unit=None]
        double dTheta;		//[Unit=Degree]
        double dBeta;		//[Unit=None]
        double dAv_s;		//[Unit=Area]
        double dA_st;		//[Unit=Area]
        double dAl;			//[Unit=Area]
        double dbv;			//[Unit=Length]
        double ds;			//[Unit=Length]
        double dsmax;	//[Unit=Length]

        double dAv_s_min;	//[Unit=Area]
        double dAv_s_req;	//[Unit=Area]
        double dAl_min;		//[Unit=Area]
        double dbv_min;		//[Unit=Length]

        double dfc; // [Unit=Stress]
        double dfy; // [Unit=Stress]
        double dfpu; // [Unit=Stress]
        double dfpy; // [Unit=Stress]

        double dvu; // [Unit=Stress]
        double dvu_fc; // [Unit=None]
        double dK; // [Unit=None]
        double dfpo; // [Unit=Stress]
        double dVn1; //[Unit=Force]
        double dVn2; //[Unit=Force]
        BOOL bSegmentalConstruction;

        double dAlpha;  //[Unit=Angle]
        double dh; //[Unit=Length]

        double dFt ; //[Unit=Force]
        double dFc ; //[Unit=Force]
        double dFlt;//[Unit=Force]
        double dFlc;//[Unit=Force]

        BOOL bOK;

        //Interface Shear
        double dVui_PscsD      ;//[Unit=Force]
        double dVni_PscsD      ;//[Unit=Force]
        BOOL bOK_PscsD         ;

        void Initialize()
        {
            bCHK = 0;
            LcomK = 0;
            iMax=iKind=0;
            memset(dForceTP, 0, sizeof(dForceTP));
            dNu = dVu = dMu = dVn = 0.0;
            dPhi = dPhiVn = 0.0;
            dVc = dVs = dVp = 0.0;
            dde = ddv = dex = 0.0;
            dTheta = dBeta = 0.0;
            dAv_s = 0.0;
            dA_st = dAl = 0.0;
            dbv = 0.0;
            ds = dsmax = 0.0;

            dAv_s_min = 0.0;
            dAv_s_req = 0.0;
            dAl_min = 0.0;
            dbv_min = 0.0;

            dfc = 0.0;
            dfy = 0.0;
            dfpu = 0.0;
            dfpy = 0.0;

            dvu = 0.0;
            dvu_fc = 0.0;
            dK = 0.0;
            dfpo = 0.0;
            dVn1 = 0.0;
            dVn2 =  0.0;
            bSegmentalConstruction = TRUE;

            dAlpha = 0.0;
            dh = 0.0;

            dFt = 0.0; 
            dFc = 0.0; 
            dFlt= 0.0;
            dFlc= 0.0;

            bOK = TRUE;

            dVui_PscsD = 0.0;
            dVni_PscsD = 0.0;
            bOK_PscsD = TRUE;
        }

};

struct T_CUMS_AASHTO_BASE_845
{
    BOOL bCHK;			//[Unit=None]
    UINT LcomK;			//[Unit=None]
    int iMax;
    int iKind;      // enum AASHTO_LCOMTYPE
    double dForceTP[6];
    double dNu;     //[Unit=Force]
    double dVu;			//[Unit=Force]
    double dMu;			//[Unit=Moment]
    double dVn;			//[Unit=Force]
    double dPhi;		//[Unit=None]
    double dVc;			//[Unit=Force]
    double dVs;			//[Unit=Force]	
    double dVp;			//[Unit=Force]
    double dPhiVn;		//[Unit=Force]
    double dde;			//[Unit=Length]
    double ddv;			//[Unit=Length]
    double dex;			//[Unit=None]
    double dTheta;		//[Unit=Degree]
    double dBeta;		//[Unit=None]
    double dAv_s;		//[Unit=Area]
    double dA_st;		//[Unit=Area]
    double dAl;			//[Unit=Area]
    double dbv;			//[Unit=Length]
    double ds;			//[Unit=Length]
    double dsmax;	//[Unit=Length]

    double dAv_s_min;	//[Unit=Area]
    double dAv_s_req;	//[Unit=Area]
    double dAl_min;		//[Unit=Area]
    double dbv_min;		//[Unit=Length]

    double dfc; // [Unit=Stress]
    double dfy; // [Unit=Stress]
    double dfpu; // [Unit=Stress]
    double dfpy; // [Unit=Stress]

    double dvu; // [Unit=Stress]
    double dvu_fc; // [Unit=None]
    double dK; // [Unit=None]
    double dfpo; // [Unit=Stress]
    double dVn1; //[Unit=Force]
    double dVn2; //[Unit=Force]
    BOOL bSegmentalConstruction;

    double dAlpha;  //[Unit=Angle]
    double dh; //[Unit=Length]

    double dFt ; //[Unit=Force]
    double dFc ; //[Unit=Force]
    double dFlt;//[Unit=Force]
    double dFlc;//[Unit=Force]

    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax=iKind=0;
        memset(dForceTP, 0, sizeof(dForceTP));
        dNu = dVu = dMu = dVn = 0.0;
        dPhi = dPhiVn = 0.0;
        dVc = dVs = dVp = 0.0;
        dde = ddv = dex = 0.0;
        dTheta = dBeta = 0.0;
        dAv_s = 0.0;
        dA_st = dAl = 0.0;
        dbv = 0.0;
        ds = dsmax = 0.0;

        dAv_s_min = 0.0;
        dAv_s_req = 0.0;
        dAl_min = 0.0;
        dbv_min = 0.0;

        dfc = 0.0;
        dfy = 0.0;
        dfpu = 0.0;
        dfpy = 0.0;

        dvu = 0.0;
        dvu_fc = 0.0;
        dK = 0.0;
        dfpo = 0.0;
        dVn1 = 0.0;
        dVn2 =  0.0;
        bSegmentalConstruction = TRUE;

        dAlpha = 0.0;
        dh = 0.0;

        dFt = 0.0; 
        dFc = 0.0; 
        dFlt= 0.0;
        dFlc= 0.0;

        bOK = TRUE;


    }
    T_CUMS_AASHTO_BASE_845() { Initialize(); }	
};

struct T_CUMS_AASHTO_BASE
{
	BOOL bCHK;			//[Unit=None]
	UINT LcomK;			//[Unit=None]
    int iMax;
    int iKind;      // enum AASHTO_LCOMTYPE
    double dForceTP[6];
    double dNu;     //[Unit=Force]
	double dVu;			//[Unit=Force]
	double dMu;			//[Unit=Moment]
	double dVn;			//[Unit=Force]
	double dPhi;		//[Unit=None]
	double dVc;			//[Unit=Force]
	double dVs;			//[Unit=Force]	
	double dVp;			//[Unit=Force]
	double dPhiVn;		//[Unit=Force]
	double dde;			//[Unit=Length]
	double ddv;			//[Unit=Length]
	double dex;			//[Unit=None]
	double dTheta;		//[Unit=Degree]
	double dBeta;		//[Unit=None]
	double dAv_s;		//[Unit=Area]
	double dA_st;		//[Unit=Area]
	double dAl;			//[Unit=Area]
	double dbv;			//[Unit=Length]
	double ds;			//[Unit=Length]
	double dsmax;	//[Unit=Length]

	double dAv_s_min;	//[Unit=Area]
	double dAv_s_req;	//[Unit=Area]
	double dAl_min;		//[Unit=Area]
	double dbv_min;		//[Unit=Length]
	
	double dfc; // [Unit=Stress]
	double dfy; // [Unit=Stress]
	double dfpu; // [Unit=Stress]
	double dfpy; // [Unit=Stress]
	
	double dvu; // [Unit=Stress]
	double dvu_fc; // [Unit=None]
	double dK; // [Unit=None]
	double dfpo; // [Unit=Stress]
	double dVn1; //[Unit=Force]
	double dVn2; //[Unit=Force]
	BOOL bSegmentalConstruction;

	double dAlpha;  //[Unit=Angle]
	double dh; //[Unit=Length]

    double dFt ; //[Unit=Force]
    double dFc ; //[Unit=Force]
    double dFlt;//[Unit=Force]
    double dFlc;//[Unit=Force]

	BOOL bOK;

    //Interface Shear
    double dVui_PscsD      ;//[Unit=Force]
    double dVni_PscsD      ;//[Unit=Force]
    BOOL bOK_PscsD         ;

	double dVui_PscsD_S;   //[Unit=Stress]
    double dVni_PscsD_S;   //[Unit=Stress]
    double dVni_PscsD_Rat  ;//[Unit=Stress]
    double dVni_Conc_PscsD ;
    double dVni_Fric_PscsD ;
    double dVni_Conc_Rat_PscsD ;
    double dVni_Fric_Rat_PscsD ;
    BOOL bOK_PscsD_CSA     ;
    BOOL bChk_PscD         ;
    BOOL bOK_Conc_PscsD    ;
    BOOL bOK_Fric_PscsD    ;


	void Initialize()
	{
		bCHK = 0;
		LcomK = 0;
        iMax=iKind=0;
        memset(dForceTP, 0, sizeof(dForceTP));
		dNu = dVu = dMu = dVn = 0.0;
		dPhi = dPhiVn = 0.0;
		dVc = dVs = dVp = 0.0;
		dde = ddv = dex = 0.0;
		dTheta = dBeta = 0.0;
		dAv_s = 0.0;
		dA_st = dAl = 0.0;
		dbv = 0.0;
	    ds = dsmax = 0.0;

		dAv_s_min = 0.0;
		dAv_s_req = 0.0;
		dAl_min = 0.0;
		dbv_min = 0.0;
		
		dfc = 0.0;
		dfy = 0.0;
		dfpu = 0.0;
		dfpy = 0.0;
	
		dvu = 0.0;
		dvu_fc = 0.0;
		dK = 0.0;
		dfpo = 0.0;
		dVn1 = 0.0;
		dVn2 =  0.0;
		bSegmentalConstruction = TRUE;

		dAlpha = 0.0;
		dh = 0.0;

        dFt = 0.0; 
        dFc = 0.0; 
        dFlt= 0.0;
        dFlc= 0.0;

        bOK = TRUE;

        dVui_PscsD = 0.0;
        dVni_PscsD = 0.0;
        bOK_PscsD = TRUE;

		dVui_PscsD_S = 0.0;
		dVni_PscsD_S = 0.0;
        dVni_PscsD_Rat = 0.0;
        dVni_Conc_PscsD = 0.0;
        dVni_Fric_PscsD = 0.0;
        dVni_Conc_Rat_PscsD = 0.0;
        dVni_Fric_Rat_PscsD = 0.0;
        bOK_PscsD_CSA = TRUE;
        bChk_PscD     = FALSE;
        bOK_Conc_PscsD = TRUE;
        bOK_Fric_PscsD = TRUE;

	}
    T_CUMS_AASHTO_BASE() { Initialize(); }	

    void Convert856(const T_CUMS_AASHTO_BASE_856& crData)
    {
        bCHK	                 = crData.bCHK               	   ;              
        LcomK	                 = crData.LcomK	                 ;
        iMax                   = crData.iMax                    ;
        iKind                  = crData.iKind                   ;
        memcpy(dForceTP, crData.dForceTP, sizeof(crData.dForceTP));
        dNu                    = crData.dNu                     ;
        dVu			               = crData.dVu			               ;
        dMu			               = crData.dMu			               ;
        dVn			               = crData.dVn			               ;
        dPhi		               = crData.dPhi		                 ;
        dVc		                 = crData.dVc		                 ;
        dVs		                 = crData.dVs		                 ;
        dVp		                 = crData.dVp		                 ;
        dPhiVn	               = crData.dPhiVn	                 ;
        dde		                 = crData.dde		                 ;
        ddv		                 = crData.ddv		                 ;
        dex		                 = crData.dex		                 ;
        dTheta	               = crData.dTheta	                 ;
        dBeta	                 = crData.dBeta	                 ;
        dAv_s	                 = crData.dAv_s	                 ;
        dA_st	                 = crData.dA_st	                 ;
        dAl		                 = crData.dAl		                 ;
        dbv		                 = crData.dbv		                 ;
        ds			               = crData.ds			                 ;
        dsmax	                 = crData.dsmax	                 ;

        dAv_s_min	             = crData.dAv_s_min	             ;
        dAv_s_req	             = crData.dAv_s_req	             ;
        dAl_min		             = crData.dAl_min		             ;
        dbv_min		             = crData.dbv_min		             ;

        dfc                    = crData.dfc                     ;
        dfy                    = crData.dfy                     ;
        dfpu                   = crData.dfpu                    ;
        dfpy                   = crData.dfpy                    ;

        dvu                    = crData.dvu                     ;
        dvu_fc                 = crData.dvu_fc                  ;
        dK                     = crData.dK                      ;
        dfpo                   = crData.dfpo                    ;
        dVn1                   = crData.dVn1                    ;
        dVn2                   = crData.dVn2                    ;
        bSegmentalConstruction = crData.bSegmentalConstruction  ;

        dAlpha                 = crData.dAlpha                  ;
        dh                     = crData.dh                      ;

        dFt                    = crData.dFt                     ;
        dFc                    = crData.dFc                     ;
        dFlt                   = crData.dFlt                    ;
        dFlc                   = crData.dFlc                    ;

        bOK                    = crData.bOK                     ;

        //Interface Shear     
        dVui_PscsD             = crData.dVui_PscsD ;
        dVni_PscsD             = crData.dVni_PscsD ;
        bOK_PscsD              = crData.bOK_PscsD  ; 

        dVui_PscsD_S           = 0.0              ;
		dVni_PscsD_S           = 0.0              ;
        dVni_PscsD_Rat         = 0.0              ;
        dVni_Conc_Rat_PscsD    = 0.0              ;
        dVni_Fric_Rat_PscsD    = 0.0              ;
        bOK_PscsD_CSA          = TRUE             ; 
        bChk_PscD              = FALSE            ;
        bOK_Conc_PscsD         = FALSE            ;
        bOK_Fric_PscsD         = FALSE            ;
    }

    void Convert845(const T_CUMS_AASHTO_BASE_845& crData)
    {
        bCHK	                 = crData.bCHK               	   ;              
        LcomK	                 = crData.LcomK	                 ;
        iMax                   = crData.iMax                    ;
        iKind                  = crData.iKind                   ;
        memcpy(dForceTP, crData.dForceTP, sizeof(crData.dForceTP));
        dNu                    = crData.dNu                     ;
        dVu			               = crData.dVu			               ;
        dMu			               = crData.dMu			               ;
        dVn			               = crData.dVn			               ;
        dPhi		               = crData.dPhi		                 ;
        dVc		                 = crData.dVc		                 ;
        dVs		                 = crData.dVs		                 ;
        dVp		                 = crData.dVp		                 ;
        dPhiVn	               = crData.dPhiVn	                 ;
        dde		                 = crData.dde		                 ;
        ddv		                 = crData.ddv		                 ;
        dex		                 = crData.dex		                 ;
        dTheta	               = crData.dTheta	                 ;
        dBeta	                 = crData.dBeta	                 ;
        dAv_s	                 = crData.dAv_s	                 ;
        dA_st	                 = crData.dA_st	                 ;
        dAl		                 = crData.dAl		                 ;
        dbv		                 = crData.dbv		                 ;
        ds			               = crData.ds			                 ;
        dsmax	                 = crData.dsmax	                 ;
                                                         
        dAv_s_min	             = crData.dAv_s_min	             ;
        dAv_s_req	             = crData.dAv_s_req	             ;
        dAl_min		             = crData.dAl_min		             ;
        dbv_min		             = crData.dbv_min		             ;
                                                         
        dfc                    = crData.dfc                     ;
        dfy                    = crData.dfy                     ;
        dfpu                   = crData.dfpu                    ;
        dfpy                   = crData.dfpy                    ;
                                                         
        dvu                    = crData.dvu                     ;
        dvu_fc                 = crData.dvu_fc                  ;
        dK                     = crData.dK                      ;
        dfpo                   = crData.dfpo                    ;
        dVn1                   = crData.dVn1                    ;
        dVn2                   = crData.dVn2                    ;
        bSegmentalConstruction = crData.bSegmentalConstruction  ;
                                                         
        dAlpha                 = crData.dAlpha                  ;
        dh                     = crData.dh                      ;
                                                         
        dFt                    = crData.dFt                     ;
        dFc                    = crData.dFc                     ;
        dFlt                   = crData.dFlt                    ;
        dFlc                   = crData.dFlc                    ;
                                                         
        bOK                    = crData.bOK                     ;
        
        //Interface Shear     
        dVui_PscsD             = 0.0              ;
        dVni_PscsD             = 0.0              ;
        bOK_PscsD              = TRUE             ; 

        dVui_PscsD_S	       = 0.0              ;
		dVni_PscsD_S           = 0.0              ;
        dVni_PscsD_Rat         = 0.0              ;
        dVni_Conc_Rat_PscsD    = 0.0              ;
        dVni_Fric_Rat_PscsD    = 0.0              ;
        bOK_PscsD_CSA          = TRUE             ; 
        bChk_PscD              = FALSE            ;
        bOK_Conc_PscsD         = FALSE            ;
        bOK_Fric_PscsD         = FALSE            ;
    }

};

struct T_CUMS_AASHTO_856	// ElemK.
{
    T_CUMS_AASHTO_BASE_856 CumsBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            CumsBase[i].Initialize();
    }
    T_CUMS_AASHTO_856() { Initialize(); }	
};

struct T_CUMS_AASHTO_845	// ElemK.
{
    T_CUMS_AASHTO_BASE_845 CumsBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            CumsBase[i].Initialize();
    }
    T_CUMS_AASHTO_845() { Initialize(); }	
};

struct T_CUMS_AASHTO	// ElemK.
{
	T_CUMS_AASHTO_BASE CumsBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<4; i++)
			CumsBase[i].Initialize();
	}
    T_CUMS_AASHTO() { Initialize(); }	

    void Convert845(const T_CUMS_AASHTO_845& rData)
    {
        for(int i=0; i<4; i++)
            CumsBase[i].Convert845(rData.CumsBase[i]);
    }

    void Convert856(const T_CUMS_AASHTO_856& rData)
    {
        for(int i=0; i<4; i++)
            CumsBase[i].Convert856(rData.CumsBase[i]);
    }
};

struct T_CUMS_AASHTO_UNIT
{
	T_CUMS_AASHTO_UNIT()
	{
        dFtp    = D_UNITSYS_BASE_FORCE;
        dMtp    = D_UNITSYS_BASE_MOMENT;
        dNu   = D_UNITSYS_BASE_FORCE;		
		dVu		= D_UNITSYS_BASE_FORCE;		
		dMu		= D_UNITSYS_BASE_MOMENT;		
		dVn		= D_UNITSYS_BASE_FORCE;		
		dPhi	= D_UNITSYS_NONE;	
		dVc		= D_UNITSYS_BASE_FORCE;		
		dVs		= D_UNITSYS_BASE_FORCE;		
		dVp		= D_UNITSYS_BASE_FORCE;		
		dPhiVn	= D_UNITSYS_BASE_FORCE;	
		dde		= D_UNITSYS_BASE_LENGTH;		
		ddv		= D_UNITSYS_BASE_LENGTH;		
		dex		= D_UNITSYS_NONE;		
		dTheta	= D_UNITSYS_BASE_DEGREE;	
		dBeta	= D_UNITSYS_NONE;	
		dAv_s	= D_UNITSYS_BASE_AREA;	
		dA_st	= D_UNITSYS_BASE_AREA;	
		dAl		= D_UNITSYS_BASE_AREA;		
		dbv		= D_UNITSYS_BASE_LENGTH;		
		ds    = D_UNITSYS_BASE_LENGTH;
		dsmax = D_UNITSYS_BASE_LENGTH;

		dAv_s_min	= D_UNITSYS_BASE_AREA;
		dAv_s_req	= D_UNITSYS_BASE_AREA;
		dAl_min		= D_UNITSYS_BASE_AREA;	
		dbv_min		= D_UNITSYS_BASE_LENGTH;
		
		dfc = D_UNITSYS_BASE_STRESS;
		dfy = D_UNITSYS_BASE_STRESS;
		dfpu = D_UNITSYS_BASE_STRESS;
		dfpy = D_UNITSYS_BASE_STRESS;
	
		dvu = D_UNITSYS_BASE_STRESS;
		dvu_fc = D_UNITSYS_NONE;
		dK = D_UNITSYS_NONE;
		dfpo = D_UNITSYS_BASE_STRESS;
		dVn1 = D_UNITSYS_BASE_FORCE;
		dVn2 = D_UNITSYS_BASE_FORCE;
		bSegmentalConstruction = D_UNITSYS_NONE;

		dAlpha = D_UNITSYS_NONE;  
        dh = D_UNITSYS_BASE_LENGTH;

		dFt = D_UNITSYS_BASE_FORCE;
		dFc = D_UNITSYS_BASE_FORCE;
		dFlt= D_UNITSYS_BASE_FORCE;
		dFlc= D_UNITSYS_BASE_FORCE;

        dVui_PscsD= D_UNITSYS_BASE_FORCE;
        dVni_PscsD= D_UNITSYS_BASE_FORCE;

		dVui_PscsD_S= D_UNITSYS_BASE_STRESS;
		dVni_PscsD_S= D_UNITSYS_BASE_STRESS;
        dVni_PscsD_Rat= D_UNITSYS_BASE_STRESS;
		dVni_Conc_PscsD= D_UNITSYS_BASE_STRESS;
		dVni_Fric_PscsD= D_UNITSYS_BASE_STRESS;
		dVni_Conc_Rat_PscsD= D_UNITSYS_BASE_STRESS;
		dVni_Fric_Rat_PscsD= D_UNITSYS_BASE_STRESS;
	}
		
    int dFtp;
    int dMtp;
    int dNu;
	int dVu;
	int dMu;		
	int dVn;		
	int dPhi;	
	int dVc;		
	int dVs;		
	int dVp;		
	int dPhiVn;	
	int dde;		
	int ddv;		
	int dex;		
	int dTheta;	
	int dBeta;	
	int dAv_s;	
	int dA_st;	
	int dAl;		
	int dbv;
	int ds;
	int dsmax;
	int dAv_s_min;	
	int dAv_s_req;	
	int dAl_min;		
	int dbv_min;

	int	dfc;
	int dfy;
	int dfpu;
	int dfpy;
	
	int dvu;
	int dvu_fc;
	int dK;
	int dfpo;
	int dVn1;
	int dVn2;
	int bSegmentalConstruction;

	int dAlpha;
	int dh;		
                 
	int dFt ; 
	int dFc ; 
	int dFlt;
	int dFlc;

    int dVui_PscsD;
    int dVni_PscsD;
 
	int dVui_PscsD_S;
    int dVni_PscsD_S;
    int dVni_PscsD_Rat;
	int dVni_Conc_PscsD;
	int dVni_Fric_PscsD;
	int dVni_Conc_Rat_PscsD;
	int dVni_Fric_Rat_PscsD;
};

//==================================================================================
// Torsion design
//==================================================================================

struct T_CRMT_AASHTO_BASE
{
	BOOL bCHK;			//[Unit=None]
	UINT LcomK;			//[Unit=None]
    int iMax;
    int iKind;      // enum AASHTO_LCOMTYPE
    double dForceTP[6];
    double dNu;     //[Unit=Force]
	double dVu;			//[Unit=Force]
	double dMu;			//[Unit=Moment]
	double dTu;			//[Unit=Moment]
	double dVn;			//[Unit=Force]
	double dTn;			//[Unit=Moment]
	double dPhi;		//[Unit=None]
	double dPhi_t;		//[Unit=None]
	double dVc;			//[Unit=Force]
	double dVs;			//[Unit=Force]	
	double dVp;			//[Unit=Force]
	double dPhiVn;		//[Unit=Force]
	double dPhi_tTn;	//[Unit=Moment]
	double dde;			//[Unit=Length]
	double ddv;			//[Unit=Length]
	double dex;			//[Unit=None]
	double dTheta;		//[Unit=Degree]
	double dBeta;		//[Unit=None]
	double dAv_s;		//[Unit=Area]
	double dbv;			//[Unit=Length]
	double dA_st;		//[Unit=Area]
	double dAl;			//[Unit=Area]
	double ds;			//[Unit=Length]
	double dsmax;	//[Unit=Length]

	double dAv_s_min;	//[Unit=Area]
	double dAv_s_req;	//[Unit=Area]
	double dAl_min;		//[Unit=Area]
	double dbv_min;		//[Unit=Length]
	double dAt;			   //[Unit=Area]
	double dAt_req;		//[Unit=Area]
	
	double dfc; // [Unit=Stress]
	double dfy; // [Unit=Stress]
	double dfpu; // [Unit=Stress]
	double dfpy; // [Unit=Stress]
	
	double dvu; // [Unit=Stress]
	double dvu_fc; // [Unit=None]
	double dK; // [Unit=None]
	double dfpo; // [Unit=Stress]
	double dVn1; //[Unit=Force]
	double dVn2; //[Unit=Force]
	BOOL bSegmentalConstruction;

	double dAlpha;  //[Unit=Angle]
	double dh; //[Unit=Length]

	double dTcr; //[Unit=Moment]
    double d025Tcr;   //[Unit=Moment]
    double dSig_comb; //[Unit=Stress]
    double d025phiF;  //[Unit=Stress]

	BOOL bOK;
	
	void Initialize()
	{
		bCHK = 0;
		LcomK = 0;
		iMax=iKind=0;
        memset(dForceTP, 0, sizeof(dForceTP));
		dNu = dVu = dMu = dVn = dTn = dTu = 0.0;
		dPhi = dPhiVn = dPhi_t = dPhi_tTn = 0.0;
		dVc = dVs = dVp = 0.0;
		dde = ddv = dex = 0.0;
		dTheta = dBeta = 0.0;
		dAv_s = 0.0;
		dA_st = dAl = 0.0;
		dbv = 0.0;
		ds = dsmax = 0.0;
		
		dAv_s_min = 0.0;
		dAv_s_req = 0.0;
		dAl_min = 0.0;
		dbv_min = 0.0;
		dAt = dAt_req = 0.0;

		dfc = 0.0;
		dfy = 0.0;
		dfpu = 0.0;
		dfpy = 0.0;
	
		dvu = 0.0;
		dvu_fc = 0.0;
		dK = 0.0;
		dfpo = 0.0;
		dVn1 = 0.0;
		dVn2 =  0.0;
		bSegmentalConstruction = TRUE;

		dAlpha = 0.0;
		dh = 0.0;
		dTcr = 0.0;		
        d025Tcr = dSig_comb = d025phiF =0.0;
		bOK = TRUE;
	}
    T_CRMT_AASHTO_BASE() { Initialize(); }	
};

struct T_CRMT_AASHTO	// ElemK.
{
	T_CRMT_AASHTO_BASE CrmtBase[6];	// 0=I-V-MAX, 1=I-V-MIN, 2=I-T-MAX, 3=J-V-MAX, 4=J-V-MIN, 5=J-T-MAX.
	
	void Initialize()
	{
		for(int i=0; i<6; i++)
			CrmtBase[i].Initialize();
	}
    T_CRMT_AASHTO() { Initialize(); }	
};

struct T_CRMT_AASHTO_UNIT
{
	T_CRMT_AASHTO_UNIT()
	{
        dFtp      = D_UNITSYS_BASE_FORCE;
        dMtp      = D_UNITSYS_BASE_MOMENT;
        dNu       = D_UNITSYS_BASE_FORCE;		
		dVu       = D_UNITSYS_BASE_FORCE;		
		dMu	      = D_UNITSYS_BASE_MOMENT;		
		dTu       = D_UNITSYS_BASE_MOMENT;		
		dVn	      = D_UNITSYS_BASE_FORCE;		
		dTn	      = D_UNITSYS_BASE_MOMENT;		
		dPhi      = D_UNITSYS_NONE;	
		dPhi_t    = D_UNITSYS_NONE;	
		dVc       = D_UNITSYS_BASE_FORCE;		
		dVs       = D_UNITSYS_BASE_FORCE;		
		dVp       = D_UNITSYS_BASE_FORCE;		
		dPhiVn    = D_UNITSYS_BASE_FORCE;	
		dPhi_tTn  = D_UNITSYS_BASE_MOMENT;
		dde       = D_UNITSYS_BASE_LENGTH;		
		ddv       = D_UNITSYS_BASE_LENGTH;		
		dex       = D_UNITSYS_NONE;		
		dTheta    = D_UNITSYS_BASE_DEGREE;	
		dBeta     = D_UNITSYS_NONE;	
		dAv_s     = D_UNITSYS_BASE_AREA;	
		dbv	      = D_UNITSYS_BASE_LENGTH;		
		dA_st     = D_UNITSYS_BASE_AREA;	
		dAl       = D_UNITSYS_BASE_AREA;
		 ds        = D_UNITSYS_BASE_LENGTH;
		dsmax     = D_UNITSYS_BASE_LENGTH;
		dAv_s_min = D_UNITSYS_BASE_AREA;
		dAv_s_req = D_UNITSYS_BASE_AREA;
		dAl_min		= D_UNITSYS_BASE_AREA;	
		dbv_min		= D_UNITSYS_BASE_LENGTH;
		dAt       = D_UNITSYS_BASE_AREA;
		dAt_req   = D_UNITSYS_BASE_AREA;

		dfc = D_UNITSYS_BASE_STRESS;
		dfy = D_UNITSYS_BASE_STRESS;
		dfpu = D_UNITSYS_BASE_STRESS;
		dfpy = D_UNITSYS_BASE_STRESS;
	
		dvu = D_UNITSYS_BASE_STRESS;
		dvu_fc = D_UNITSYS_NONE;
		dK = D_UNITSYS_NONE;
		dfpo = D_UNITSYS_BASE_STRESS;
		dVn1 = D_UNITSYS_BASE_FORCE;
		dVn2 = D_UNITSYS_BASE_FORCE;
		bSegmentalConstruction = D_UNITSYS_NONE;

		dAlpha = D_UNITSYS_NONE;
		dh = D_UNITSYS_BASE_LENGTH;
		dTcr = D_UNITSYS_BASE_MOMENT;

        d025Tcr = D_UNITSYS_BASE_MOMENT;  
        dSig_comb = D_UNITSYS_BASE_STRESS; 
        d025phiF  = D_UNITSYS_BASE_STRESS; 
	}

    int dFtp;
    int dMtp;
    int dNu;
	int	dVu;		
	int dMu;		
	int dTu;		
	int dVn;		
	int dTn;		
	int dPhi;	
	int dPhi_t;	
	int dVc;		
	int dVs;		
	int dVp;		
	int dPhiVn;	
	int dPhi_tTn;
	int dde;		
	int ddv;		
	int dex;		
	int dTheta;	
	int dBeta;	
	int dAv_s;	
	int dbv;		
	int dA_st;	
	int dAl;
    int ds;
    int dsmax;

	int dAv_s_min;
	int dAv_s_req;
	int dAl_min;	
	int dbv_min;	
    int dAt;
    int dAt_req;

    int	dfc;
	int dfy;
	int dfpu;
	int dfpy;
	
	int dvu;
	int dvu_fc;
	int dK;
	int dfpo;
	int dVn1;
	int dVn2;
	int bSegmentalConstruction;

	int dAlpha;
	int dh;	
	int dTcr;

    int d025Tcr;   //[Unit=Moment]
    int dSig_comb; //[Unit=Stress]
    int d025phiF;  //[Unit=Stress]

};

// 전체 크기
struct T_PSCD_DGN_TDS
{
    int nTndnSize;
    int nRbarSize;

    void Initialize()
    {
        nTndnSize = 0;
        nRbarSize = 0;
    }
    T_PSCD_DGN_TDS() { Initialize(); }
};

// 요소 - 정/부 - I,J단
struct T_PSCD_DGN_DS_B
{
    int nTndnSize;
    int nRbarSize;

    void Initialize()
    {
        nTndnSize = 0;
        nRbarSize = 0;
    }
    T_PSCD_DGN_DS_B() { Initialize(); }
    T_PSCD_DGN_DS_B(const T_PSCD_DGN_DS_B& rData) { Initialize(); *this = rData; }
    T_PSCD_DGN_DS_B& operator = (const T_PSCD_DGN_DS_B& rData)
    {
        nTndnSize = rData.nTndnSize;
        nRbarSize = rData.nRbarSize;
        return *this;
    }
};

struct T_PSCD_DGN_DS
{
    T_PSCD_DGN_DS_B SB[4]; // 0,1:I-P/N,   2,3:J-P/N
    int nStartID;    

    void Initialize()
    {
        for ( int i=0; i<4; ++i )
        {
            SB[i].Initialize();
        }        
        nStartID = 0;
    }
    T_PSCD_DGN_DS() { Initialize(); }
    T_PSCD_DGN_DS(const T_PSCD_DGN_DS& rData) { Initialize(); *this = rData; }
    T_PSCD_DGN_DS& operator = (const T_PSCD_DGN_DS& rData)
    {
        for (int i = 0; i < 4; i++)
        {
            SB[i] = rData.SB[i];
        }
        nStartID = rData.nStartID;
        return *this;
    }
};


struct T_PSCD_TNDN_B
{
    T_TDNA_K TdnaK;
    double dx;
    double dy;

    void Initialize()
    {
        TdnaK = 0;
        dx = 0.0;
        dy = 0.0;
    }
};

struct T_PSCD_DGN_B
{
    BOOL bCHK;		//[Unit=None]
    UINT LcomK;		//[Unit=None]
    int  nMax;      //
    int  nKind;     //

    T_PSCD_FORCE ForD;
    double dRatio;
    double dMn;
    double dMd;

    CArray<T_PSCD_TNDN_B, T_PSCD_TNDN_B&> arTndn;

    void Initialize()
    {
        bCHK  = FALSE;
        LcomK = 0;
        nMax  = 0;
        nKind = 0;
        ForD.Initialize();
        dRatio = 0.0;
        dMn    = 0.0;
        dMd    = 0.0;
        arTndn.RemoveAll();
    }

    T_PSCD_DGN_B() { Initialize(); }
    T_PSCD_DGN_B(const T_PSCD_DGN_B& rData) { *this = rData; }
    T_PSCD_DGN_B& operator = (const T_PSCD_DGN_B& rData)
    {
        bCHK   = rData.bCHK;
        LcomK  = rData.LcomK;
        nMax   = rData.nMax;
        nKind  = rData.nKind;
        ForD   = rData.ForD;
        dRatio = rData.dRatio;
        dMn    = rData.dMn;
        dMd    = rData.dMd;
        arTndn.Copy(rData.arTndn);

        return *this;
    }
};

struct T_PSCD_DGN_B_RW
{
    BOOL bCHK;
    UINT LcomK;
    int  nMax;
    int  nKind;

    T_PSCD_FORCE ForD;
    double dRatio;
    double dMn;
    double dMd;

    int nTndnSize;

    void Initialize()
    {
        bCHK  = FALSE;
        LcomK = 0;
        nMax  = 0;
        nKind = 0;
        ForD.Initialize();
        dRatio = 0.0;
        dMn    = 0.0;
        dMd    = 0.0;
        nTndnSize = 0;
    }

    T_PSCD_DGN_B_RW() { Initialize(); }
    void GetData(T_PSCD_DGN_B& rData)
    {
        rData.bCHK   = bCHK;
        rData.LcomK  = LcomK;
        rData.nMax   = nMax;
        rData.nKind  = nKind;
        rData.ForD   = ForD;
        rData.dRatio = dRatio;
        rData.dMn    = dMn;
        rData.dMd    = dMd;
        rData.arTndn.SetSize(nTndnSize);
    }

    void SetData(T_PSCD_DGN_B& rData)
    {
        bCHK   = rData.bCHK;
        LcomK  = rData.LcomK;
        nMax   = rData.nMax;
        nKind  = rData.nKind;
        ForD   = rData.ForD;
        dRatio = rData.dRatio;
        dMn    = rData.dMn;
        dMd    = rData.dMd;
        nTndnSize = static_cast<int>(rData.arTndn.GetSize());
    }
};

struct T_PSCD_DGN_ST
{
    BOOL bCHK;		//[Unit=None]
    UINT LcomK;		//[Unit=None]
    int  nMax;      //
    int  nKind;     //

    T_PSCD_FORCE ForD;
    double dVrat;
    double dTrat;
    double dCombRat;
    double dVr;
    double dTr;

    void Initialize()
    {
        bCHK  = FALSE;
        LcomK = 0;
        nMax  = 0;
        nKind = 0;
        ForD.Initialize();
        dVrat = 0.0;
        dTrat = 0.0;
        dCombRat = 0.0;
        dVr   = 0.0;
        dTr   = 0.0;
    }

    T_PSCD_DGN_ST() { Initialize(); }
    T_PSCD_DGN_ST(const T_PSCD_DGN_ST& rData) { *this = rData; }
    T_PSCD_DGN_ST& operator = (const T_PSCD_DGN_ST& rData)
    {
        bCHK   = rData.bCHK;
        LcomK  = rData.LcomK;
        nMax   = rData.nMax;
        nKind  = rData.nKind;
        ForD   = rData.ForD;
        dVrat  = rData.dVrat;
        dTrat  = rData.dTrat;
        dCombRat = rData.dCombRat;        
        dVr    = rData.dVr;
        dTr    = rData.dTr;

        return *this;
    }
};


struct T_PSCD_DGN_INFO
{
    T_PSCD_DGN_B  Bend[4];       // 0=I-P, 1=I-N, 2=J-P, 3=J-N
    T_PSCD_DGN_ST ShearD[2];     // 0=I, 1=J
    T_PSCD_DGN_ST TorsionD[2];
    T_PSCD_DGN_ST STCombD[2];

    void Initialize()
    {
        for ( int i=0; i<4; ++i )
        {
            Bend[i].Initialize();
        }
        for ( int i=0; i<2; ++i )
        {
            ShearD[i].Initialize();
            TorsionD[i].Initialize();
            STCombD[i].Initialize();
        }
    }

    T_PSCD_DGN_INFO() { Initialize(); }
    T_PSCD_DGN_INFO(const T_PSCD_DGN_INFO& rData) { *this = rData; }
    T_PSCD_DGN_INFO& operator = (const T_PSCD_DGN_INFO& rData)
    {
        for ( int i=0; i<4; ++i )
        {
            Bend[i] = rData.Bend[i];
        }
        for ( int i=0; i<2; ++i )
        {
            ShearD[i]   = rData.ShearD[i];
            TorsionD[i] = rData.TorsionD[i];
            STCombD[i]  = rData.STCombD[i];
        }

        return *this;
    }
};

struct T_PSCD_DGN_INFO_RW
{
    T_PSCD_DGN_B_RW Bend[4];
    T_PSCD_DGN_ST ShearD[2];
    T_PSCD_DGN_ST TorsionD[2];
    T_PSCD_DGN_ST STCombD[2];

    void Initialize()
    {
        for ( int i=0; i<4; ++i )
        {
            Bend[i].Initialize();
        }
        for ( int i=0; i<2; ++i )
        {
            ShearD[i].Initialize();
            TorsionD[i].Initialize();
            STCombD[i].Initialize();
        }
    }

    T_PSCD_DGN_INFO_RW() { Initialize(); }
    void GetData(T_PSCD_DGN_INFO& rData)
    {
        for ( int i=0; i<4; ++i )
        {
            Bend[i].GetData(rData.Bend[i]);
        }
        for ( int i=0; i<2; ++i )
        {
            rData.ShearD[i] = ShearD[i];
            rData.TorsionD[i] = TorsionD[i];
            rData.STCombD[i] = STCombD[i];
        }
    }
    void SetData(T_PSCD_DGN_INFO& rData)
    {
        for ( int i=0; i<4; ++i )
        {
            Bend[i].SetData(rData.Bend[i]);
        }
        for ( int i=0; i<2; ++i )
        {
            ShearD[i]   = rData.ShearD[i];
            TorsionD[i] = rData.TorsionD[i];
            STCombD[i]  = rData.STCombD[i];
        }
    }
};


// For PSC Result

//============================================================================
// 1. Stresses in concrete during construction stage
//============================================================================
struct T_FYSG_PSC_BASE
{
	BOOL bCHK;		//설계결과유무
    UINT StageK;	//스테이지번호		[Unit=None]
    UINT StepK;   // Step Key.      [Unit=None]
    double dFT;   //상단응력도
    double dFB;   //하단응력도
    double dFTL;	//상단좌단 합성응력도	[Unit=Stress]
    double dFBL;	//하단좌단 합성응력도	[Unit=Stress]
    double dFTR;	//상단우단 합성응력도	[Unit=Stress]
    double dFBR;	//하단우단 합성응력도	[Unit=Stress]
    double dFMAX;	//최대/최소 합성응력도	[Unit=Stress]
    double dALW;	//허용응력도			[Unit=Stress]
    double dfci;    // 초기압축강도
    double dDuration;
    double dRatio;
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        StageK = StepK=0;
        dFT = dFB = 0.0;
        dFTL = dFBL = dFTR = dFBR = dFMAX = dALW = 0.0;
        dfci = 0.0;
        dDuration = 0.0;
        dRatio = 0.0;
        bOK = TRUE;
    }	
    T_FYSG_PSC_BASE() { Initialize(); }
};
struct T_FYSG_PSC	// ElemK.
{
	T_FYSG_PSC_BASE FysgBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<4; i++)
			FysgBase[i].Initialize();
	}
    T_FYSG_PSC() { Initialize(); }
};

struct T_FYSG_PSC_UNIT
{
	T_FYSG_PSC_UNIT()
	{
		dFT		= D_UNITSYS_BASE_STRESS;
		dFB		= D_UNITSYS_BASE_STRESS;
		dFTL	= D_UNITSYS_BASE_STRESS;
		dFBL	= D_UNITSYS_BASE_STRESS;
		dFTR	= D_UNITSYS_BASE_STRESS;
		dFBR	= D_UNITSYS_BASE_STRESS;
		dFMAX	= D_UNITSYS_BASE_STRESS;
		dALW	= D_UNITSYS_BASE_STRESS;	
	}
	int dFT;
	int dFB;
	int dFTL;
	int dFBL;
	int dFTR;
	int dFBR;
	int dFMAX;
	int dALW;	
};

//==================================================================================
// 2. Stresses in concrete during service load combinations 
//==================================================================================

struct T_FYCM_PSC_BASE
{  
	BOOL bCHK;		
	UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iKind;    // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int iLSTerm;  // Long, Short Term 구별 
	double dFT;		//[Unit=Stress]
	double dFB;		//[Unit=Stress]
	double dFTL;	//[Unit=Stress]
	double dFBL;	//[Unit=Stress]
	double dFTR;	//[Unit=Stress]
	double dFBR;	//[Unit=Stress]
	double dFMAX;	//[Unit=Stress]
	double dALW;	//[Unit=Stress]
	BOOL bOK;

	void Initialize()
	{
		bCHK = 0;
        LcomK = iMax = iKind = iLSTerm = 0;
		dFT = dFB = 0.0;
		dFTL = dFBL = dFTR = dFBR = dFMAX = dALW = 0;
		bOK = TRUE;
	}
    T_FYCM_PSC_BASE() { Initialize(); }
};

struct T_FYCM_PSC	// ElemK.
{
	T_FYCM_PSC_BASE FycmBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<4; i++) FycmBase[i].Initialize();
	}
    T_FYCM_PSC() { Initialize(); }	
};

struct T_FYCM_PSC_UNIT
{
	T_FYCM_PSC_UNIT()
	{
		dFT		= D_UNITSYS_BASE_STRESS;		
		dFB		= D_UNITSYS_BASE_STRESS;		
		dFTL	= D_UNITSYS_BASE_STRESS;
		dFBL	= D_UNITSYS_BASE_STRESS;
		dFTR	= D_UNITSYS_BASE_STRESS;	
		dFBR	= D_UNITSYS_BASE_STRESS;
		dFMAX	= D_UNITSYS_BASE_STRESS;
		dALW	= D_UNITSYS_BASE_STRESS;	
	}

	int dFT;		
	int dFB;		
	int dFTL;	
	int dFBL;	
	int dFTR;	
	int dFBR;	
	int dFMAX;	
	int dALW;	
};

//==================================================================================
// Tensile stresses in prestressing tendons
//==================================================================================

// struct T_FYPC_AASHTO	// TdnaK. 이걸 사용하자. 

// Check Principal stress for stage
struct T_FPSG_PSC_BASE  
{
    BOOL bCHK;		//설계결과유무 
    UINT StageK;
    UINT StepK;
    int iKind;    // 시공단계에서 필요할 수 있기 때문에. 
    double dFP1;	//1위치 주응력		[Unit=Stress]
    double dFP2;	//2위치 주응력		[Unit=Stress]
    double dFP3;	//3위치 주응력		[Unit=Stress]
    double dFP4;	//4위치 주응력		[Unit=Stress]
    double dFP5;	//5위치 주응력		[Unit=Stress]
    double dFP6;	//6위치 주응력		[Unit=Stress]
    double dFP7;	//7위치 주응력		[Unit=Stress]
    double dFP8;	//8위치 주응력		[Unit=Stress]
    double dFP9;	//9위치 주응력		[Unit=Stress]
    double dFP10;	//10위치 주응력		[Unit=Stress]
    double dFMAX; // MAX/MIN 주응력	[Unit=Stress]
    double dAFP;  // 허용 주응력    [Unit=Stress]
    double dfci;    // 초기압축강도
    double dDuration;
    double dRatio;
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        StageK = StepK = 0;
        iKind = 0;  
        iKind = 0;
        dFP1 = dFP2 = dFP3 = dFP4 = dFP5 = dFP6 = dFP7 = dFP8 = dFP9= dFP10 = dFMAX = dAFP = 0.0;
        dfci = 0.0;
        dDuration = 0.0;
        dRatio = 0.0;
        bOK = TRUE;
    }
    T_FPSG_PSC_BASE() { Initialize(); }
};

struct T_FPSG_PSC	// ElemK.
{
	T_FPSG_PSC_BASE FpsgBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<4; i++)
			FpsgBase[i].Initialize();
	}
    T_FPSG_PSC() { Initialize(); }
};

struct T_FPSG_PSC_UNIT
{
    T_FPSG_PSC_UNIT()
    {
        dFP1 = D_UNITSYS_BASE_STRESS;
        dFP2 = D_UNITSYS_BASE_STRESS;
        dFP3 = D_UNITSYS_BASE_STRESS;
        dFP4 = D_UNITSYS_BASE_STRESS;
        dFP5 = D_UNITSYS_BASE_STRESS;
        dFP6 = D_UNITSYS_BASE_STRESS;
        dFP7 = D_UNITSYS_BASE_STRESS;
        dFP8 = D_UNITSYS_BASE_STRESS;
        dFP9 = D_UNITSYS_BASE_STRESS;
        dFP10= D_UNITSYS_BASE_STRESS;
        dFMAX= D_UNITSYS_BASE_STRESS;
        dAFP = D_UNITSYS_BASE_STRESS; 
    }
    int dFP1;
    int dFP2;
    int dFP3;
    int dFP4;
    int dFP5;
    int dFP6;
    int dFP7;
    int dFP8;
    int dFP9;
    int dFP10;
    int dFMAX;
    int dAFP;  
};

// Check Principal stress
struct T_FPMS_PSC_BASE  
{
    BOOL bCHK;		//설계결과유무 
    UINT LcomK;
    int iMax;		  //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iKind;    // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int iLSTerm;  // Long, Short Term 구별 
    double dFP1;	//1위치 주응력		[Unit=Stress]
    double dFP2;	//2위치 주응력		[Unit=Stress]
    double dFP3;	//3위치 주응력		[Unit=Stress]
    double dFP4;	//4위치 주응력		[Unit=Stress]
    double dFP5;	//5위치 주응력		[Unit=Stress]
    double dFP6;	//6위치 주응력		[Unit=Stress]
    double dFP7;	//7위치 주응력		[Unit=Stress]
    double dFP8;	//8위치 주응력		[Unit=Stress]
    double dFP9;	//9위치 주응력		[Unit=Stress]
    double dFP10;	//10위치 주응력		[Unit=Stress]
    double dFMAX; // MAX/MIN 주응력	[Unit=Stress]
    double dAFP;  // 허용 주응력    [Unit=Stress]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;  
        iKind = iLSTerm = 0;
        dFP1 = dFP2 = dFP3 = dFP4 = dFP5 = dFP6 = dFP7 = dFP8 = dFP9= dFP10 = dFMAX = dAFP = 0;
        bOK = TRUE;
    }
    T_FPMS_PSC_BASE() { Initialize(); }
};

struct T_FPMS_PSC	// ElemK.
{
	T_FPMS_PSC_BASE FpmsBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<4; i++)
			FpmsBase[i].Initialize();
	}
    T_FPMS_PSC() { Initialize(); }
};

struct T_FPMS_PSC_UNIT
{
    T_FPMS_PSC_UNIT()
    {
        dFP1 = D_UNITSYS_BASE_STRESS;
        dFP2 = D_UNITSYS_BASE_STRESS;
        dFP3 = D_UNITSYS_BASE_STRESS;
        dFP4 = D_UNITSYS_BASE_STRESS;
        dFP5 = D_UNITSYS_BASE_STRESS;
        dFP6 = D_UNITSYS_BASE_STRESS;
        dFP7 = D_UNITSYS_BASE_STRESS;
        dFP8 = D_UNITSYS_BASE_STRESS;
        dFP9 = D_UNITSYS_BASE_STRESS;
        dFP10= D_UNITSYS_BASE_STRESS;
        dFMAX= D_UNITSYS_BASE_STRESS;
        dAFP = D_UNITSYS_BASE_STRESS; 
    }
    int dFP1;
    int dFP2;
    int dFP3;
    int dFP4;
    int dFP5;
    int dFP6;
    int dFP7;
    int dFP8;
    int dFP9;
    int dFP10;
    int dFMAX;
    int dAFP;  
};

//==================================================================================
// Bending Resistance
//==================================================================================
struct T_BDCR_PSC_BASE
{
	BOOL bCHK;
    UINT LcomK;
    int iMax;
    int iKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int iLSTerm;  // Long, Short Term 구별
    
    double dMu;	      // 계수모멘트, 작용부재력	[Unit=Moment]
    double dMn;       // 공칭강도, 제한값 적용, nominal flexural resistance [Unit=Moment]
    double dMn_cal;   // 공칭강도, 처음 계산, nominal flexural resistance [Unit=Moment]
    double dphi;      // 강도감소계수
    double dphiMn;    // phi*Mn
    double dRatM;     // Mu/(phi*Mn)
    double dc_neu;    // 중립축
    double dalpha;    //  0.85, 
    double dbeta1;    // a = beta1*c
    double da;        // 직사각형 Stress block a의 비
    double dPe;       // 인장측 유효긴장력			[Unit=Force]
    double dPe_t;     // 전체 유효긴장력        [Unit=Force]
    double dAps;	    // 인장측 PS강재의 단면적	[Unit=Area]
    double dAps_t;	  // 전체 PS강재의 단면적	[Unit=Area]
    double dAst;	    // 인장측 철근의 단면적		[Unit=Area]
    double dAsc;      // 압축측 철근의 단면적   [Unit=Area] 
    double dRho_pt;   // 인장측 tendon. [Unit=None]
    double dRho_st;   // 인장측 철근비			
    double dRho_sc;   // 압축측 철근비			
    double dRho_ps;   // 인장측 강재비(tendon+bar), (Ap+As)/(b*d) , q_b 강재지수?
    double ddpt;      // 압축연단에서 인장 Tendon의 도심까지 거리 [Unit=Length]
    double ddst;      // 압축연단에서 인장측 철근 도심까지의 거리		[Unit=Length]
    double ddsc;	    // 압축연단에서 압축측 철근 도심까지의 거리		[Unit=Length]  
    //
    double dphiMcr;   // 균열모멘트 [Unit=Moment]
    double dphiMn_mo; // Mn에 수정해서 사용할때 
    double dRho_p_max; // 최대 강재량 
    double dRho_p_min; // 최소 강재량   
    // KSCE-LSD12
    double dCmax;     // [L] 최대 중립축
    double dAs_max;   // [L^2] 인장철근 상한값
    double dAs_min;   // [L^2] 인장철근 하한값
    
    BOOL bOK;
    
	void Initialize()
	{
		bCHK = 0;
        LcomK=0;
        iMax=iKind=iLSTerm=0;
        dMu = dMn = dMn_cal = dphi = dphiMn = dRatM = dc_neu = dalpha = dbeta1 = da = 0.0;
        dPe = dPe_t = dAps = dAps_t = dAst = dAsc = 0.0;
        dRho_pt = dRho_st = dRho_sc = dRho_ps = 0.0;
        ddpt = ddst = ddsc = 0.0;
        dphiMcr = dphiMn_mo = 0.0;
        dRho_p_max = dRho_p_min = 0.0;
        dCmax = dAs_max = dAs_min = 0.0;
		bOK = 0;
	}
    T_BDCR_PSC_BASE() { Initialize(); }
};

struct T_BDCR_PSC	// ElemK.
{
	T_BDCR_PSC_BASE BdcrBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<4; i++)
			BdcrBase[i].Initialize();
	}
    T_BDCR_PSC() { Initialize(); }
};

struct T_BDCR_PSC_UNIT
{
	T_BDCR_PSC_UNIT()
	{
        dMu       = D_UNITSYS_BASE_MOMENT;
        dMn       = D_UNITSYS_BASE_MOMENT;
        dMn_cal   = D_UNITSYS_BASE_MOMENT;
        dphiMn    = D_UNITSYS_BASE_MOMENT;
        dc_neu    = D_UNITSYS_BASE_LENGTH;
        da        = D_UNITSYS_BASE_LENGTH;
        dPe       = D_UNITSYS_BASE_FORCE; 
        dPe_t     = D_UNITSYS_BASE_FORCE; 
        dAps      = D_UNITSYS_BASE_AREA;	
        dAps_t    = D_UNITSYS_BASE_AREA;	
        dAst      = D_UNITSYS_BASE_AREA;	
        dAsc      = D_UNITSYS_BASE_AREA;  
        ddpt      = D_UNITSYS_BASE_LENGTH;
        ddst      = D_UNITSYS_BASE_LENGTH;
        ddsc      = D_UNITSYS_BASE_LENGTH;
        dphiMcr   = D_UNITSYS_BASE_MOMENT;
        dphiMn_mo = D_UNITSYS_BASE_MOMENT;
        dCmax     = D_UNITSYS_BASE_LENGTH;     
        dAs_max   = D_UNITSYS_BASE_AREA;
        dAs_min   = D_UNITSYS_BASE_AREA;


	}

    int dMu;
    int dMn;
    int dMn_cal;
    int dphiMn;
    int dc_neu;
    int da;
    int dPe; 
    int dPe_t; 
    int dAps;	
    int dAps_t;	
    int dAst;	
    int dAsc;  
    int ddpt;
    int ddst;
    int ddsc;
    int dphiMcr;
    int dphiMn_mo;
    int dCmax;     
    int dAs_max;   
    int dAs_min;   

};

//==================================================================================
// Crack check
//==================================================================================

struct T_BDCW_PSC_BASE
{
	BOOL bCHK;		//[Unit=None]
	
	UINT LcomK;		//[Unit=None] 
    int iKind;    // enum AASHTO_LCOMTYPE
    int iMax;
    int iLSTerm;  // Long, Short Term 구별

    double dPu;        //[Unit=Force]
    double dMu;        //[Unit=Moment]
    double dFT;		     //Top stress for 균열단면[Unit=Stress]
	double dFB;		     //Bottom stress for 균열단면[Unit=Stress]
    double dft;        //Top stress for gross sect. [Unit=Stress]
    double dfb;        //Bottom stress for gross sect.[Unit=Stress] 
	double dFSS;	     //[Unit=Stress]
    double dEpsilon_sm; // 철근 변형률,    [Unit=None]
    double dEpsilon_cm; // 콘크리트 변형률 [Unit=None]
    double dEpsi_sm_Epsi_cm; // ε_sm - ε_cm [Unit=None]
    double dSr_max;  // maximum crack spacing
	double ds_use;	//[Unit=Length]
	double ds_max;	//[Unit=Length]
    double dWk;     //[Unit=Length]
    double dWmax;   //[Unit=Length]
    double dRatio;    //[Unit=None]
    double dCrack_Asmin ; //[L^2]
    double dAst         ; //[L^2]
	
	BOOL bOK;
	
    BOOL bOK_Cr;
    BOOL bOk_Zero;
    
    
	void Initialize()
	{
		bCHK = 0;
		LcomK = 0;
        iKind=iMax=iLSTerm=0;
        dPu = dMu = 0.0;
		dFT = dFB = dFSS = 0.0;
        dft = dfb = 0.0;
        dEpsilon_sm = dEpsilon_cm = 0.0;
        dEpsi_sm_Epsi_cm = 0.0;
        dSr_max = 0.0;
        dft = dfb = 0.0;
		ds_use = ds_max = 0.0;
        dWk=dWmax=0.0;
        dRatio=0.0;
		bOK = TRUE;
        dCrack_Asmin = dAst =0.0;
        bOK_Cr = FALSE;
        bOk_Zero = FALSE;

	}
    T_BDCW_PSC_BASE() { Initialize(); }	
};

struct T_BDCW_PSC_875// ElemK.
{
    T_BDCW_PSC_BASE BdcwBase[4];	    // 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    T_BDCW_PSC_BASE BdcwBaseZero[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
        {
            BdcwBase[i].Initialize();
            BdcwBaseZero[i].Initialize();
        }  
    }
    T_BDCW_PSC_875() { Initialize(); }		
};

struct T_BDCW_PSC// ElemK.
{
	T_BDCW_PSC_BASE BdcwBase[4];	    // 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    T_BDCW_PSC_BASE BdcwBaseZero[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	T_BDCW_PSC_BASE BdcwBaseAsmin[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

	void Initialize()
	{
		for(int i=0; i<4; i++)
        {
			BdcwBase[i].Initialize();
            BdcwBaseZero[i].Initialize();
            BdcwBaseAsmin[i].Initialize();
        }  
	}
    void Convert875(const T_BDCW_PSC_875& rData)
    {
        for(int i=0; i<4; i++)
        {
            BdcwBase[i] = rData.BdcwBase[i];
            BdcwBaseZero[i] = rData.BdcwBaseZero[i];
            BdcwBaseAsmin[i].Initialize();
        }
    }
    T_BDCW_PSC() { Initialize(); }		
};



struct T_BDCW_PSC_UNIT
{
	T_BDCW_PSC_UNIT()
	{
        dPu = D_UNITSYS_BASE_FORCE;
        dMu = D_UNITSYS_BASE_MOMENT;
		dFT = D_UNITSYS_BASE_STRESS;	
		dFB = D_UNITSYS_BASE_STRESS;	
		dft = D_UNITSYS_BASE_STRESS;	
		dfb = D_UNITSYS_BASE_STRESS;	
		dFSS= D_UNITSYS_BASE_STRESS;
        dSr_max = D_UNITSYS_BASE_LENGTH; 

		ds_use = D_UNITSYS_BASE_LENGTH;
		ds_max = D_UNITSYS_BASE_LENGTH;
        dWk    = D_UNITSYS_BASE_LENGTH;
        dWmax  = D_UNITSYS_BASE_LENGTH;

        dCrack_Asmin = D_UNITSYS_BASE_AREA;
        dAst         = D_UNITSYS_BASE_AREA;
	}

    int dPu;
    int dMu;
	int dFT;	
	int dFB;	
    int dft;
    int dfb;
	int dFSS;
    int dSr_max;
	int ds_use;
	int ds_max;
    int dWk;
    int dWmax;
    int dCrack_Asmin ; 
    int dAst         ; 

};

//==================================================================================
// Shear design
//==================================================================================
struct T_CUMS_PSC_BASE
{
	BOOL bCHK;			//[Unit=None]
	UINT LcomK;			//[Unit=None]
    int iMax;
    int iKind;      // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int iLSTerm;    // Long, Short Term 구별

    double dPu;     //[Unit=Force]
	double dVu;			//[Unit=Force]
	double dMu;			//[Unit=Moment]
    double dft;     //[Unit=Stress]
    double dfb;     //[Unit=Stress]
	double dVn;			//[Unit=Force]
	double dPhi;		//[Unit=None]
	double dVc;			//[Unit=Force]
	double dVs;			//[Unit=Force]	
	double dVp;			//[Unit=Force]
	double dPhiVn;		//[Unit=Force]
    double dAv_s;     //[Unit=LENGTH]
	double dAv_s_min;	//[Unit=LENGTH]
	double dAv_s_req;	//[Unit=LENGTH]
    double ds;        // 전단철근간격 [Unit=LENGTH]
    double ds_max;    // 전단철근간격 max [Unit=LENGTH]
    double dVu_pVn_Rat; // Vu/pVn, [Unit=None]
    BOOL bOK;
 
    // Eurocode2-2:05	
    double dV_Rd_max; 

    //KSCE-LSD12
    double dRho_w;      // [None]
    double dRho_w_min;  // [None]
    double ds_l_max;    //[L]

	void Initialize()
	{
		bCHK = 0;
		LcomK = 0;
        iMax=iKind=iLSTerm=0;
		dPu = dVu = dMu = dVn = 0.0;
        dft=dfb=0.0;
		dPhi = dPhiVn = 0.0;
		dVc = dVs = dVp = 0.0;
		dAv_s = dAv_s_min = dAv_s_req = 0.0;
	  ds = ds_max = 0.0;
		bOK = 0;
        dV_Rd_max=0.0;
        dRho_w = dRho_w_min = ds_l_max =0.0;
	}
    T_CUMS_PSC_BASE() { Initialize(); }	
};

struct T_CUMS_PSC	// ElemK.
{
	T_CUMS_PSC_BASE CumsBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<4; i++)
			CumsBase[i].Initialize();
	}
    T_CUMS_PSC() { Initialize(); }	
};

struct T_CUMS_PSC_UNIT
{
	T_CUMS_PSC_UNIT()
	{
        dPu   = D_UNITSYS_BASE_FORCE;		
		dVu		= D_UNITSYS_BASE_FORCE;		
		dMu		= D_UNITSYS_BASE_MOMENT;	
        dft   = D_UNITSYS_BASE_STRESS;
        dfb   = D_UNITSYS_BASE_STRESS;
		dVn		= D_UNITSYS_BASE_FORCE;		
		dVc		= D_UNITSYS_BASE_FORCE;		
		dVs		= D_UNITSYS_BASE_FORCE;		
		dVp		= D_UNITSYS_BASE_FORCE;		
		dPhiVn	= D_UNITSYS_BASE_FORCE;	
		dAv_s	= D_UNITSYS_BASE_LENGTH;	
        dAv_s_min = D_UNITSYS_BASE_LENGTH;
        dAv_s_req = D_UNITSYS_BASE_LENGTH;
		ds    = D_UNITSYS_BASE_LENGTH;
		ds_max = D_UNITSYS_BASE_LENGTH;		
        dV_Rd_max = D_UNITSYS_BASE_FORCE;
        ds_l_max = D_UNITSYS_BASE_LENGTH;
	}
		
    int dPu;
	int dVu;
	int dMu;		
    int dft;
    int dfb;
	int dVn;		
	int dVc;		
	int dVs;		
	int dVp;		
	int dPhiVn;	
	int dAv_s;	
	int dAv_s_min;	
	int dAv_s_req;	
	int ds;
	int ds_max;
    int dV_Rd_max;
    int ds_l_max;
};

//==================================================================================
// Torsion design
//==================================================================================
struct T_CRMT_PSC_BASE_875
{
	BOOL bCHK;				//[Unit=None]
	UINT LcomK;				//[Unit=None]
    int iMax;
    int iKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int iLSTerm;      // Long, Short Term 구별
    double dPu;       //[Unit=Force]
	double dVu;				//[Unit=Force]
	double dMu;				//[Unit=Moment]
	double dTu;				//[Unit=Moment]
    double dft;     //[Unit=Stress]
    double dfb;     //[Unit=Stress]
	double dVn;				//[Unit=Force]
	double dTn;				//[Unit=Moment]
	double dPhi;			//[Unit=None]
	double dPhi_t;		//[Unit=None]
	double dPhiVn;		//[Unit=Force]
	double dPhi_tTn;	//[Unit=Moment]
	double dAl;       //[Unit=Area]  
    double dAl_min;		//[Unit=Area]  
    double dAt;       //[Unit=Area]  
    double dAt_req;   //[Unit=Area]  
    double dTcr;      //[Unit=Moment]

	BOOL bOK;

    double dV_Rd_max; //[Unit=Force]
    double dComb_rat; //[Unit=None]

    //KSCE-LSD12
    double dAl_req;
    double ds_t;
    double dst_max_rep;
    double drho_st;
    double drho_st_min;
        
	void Initialize()
	{
		bCHK = 0;
		LcomK = 0;
		iMax=iKind=iLSTerm=0;
		dPu = dVu = dMu = dVn = dTn = dTu = 0.0;
        dft=dfb=0.0;
        dPhi = dPhiVn = dPhi_t = dPhi_tTn = 0.0;
		dAl = dAl_min = 0.0;		
		dAt = dAt_req = 0.0;
		dTcr = 0.0;		
		bOK = FALSE;
        dV_Rd_max = 0.0;
        dComb_rat = 0.0;
        dAl_req = ds_t = dst_max_rep = 0.0;
        drho_st = drho_st_min = 0.0;
	}
    T_CRMT_PSC_BASE_875() { Initialize(); }	
};

struct T_CRMT_PSC_BASE
{
    BOOL bCHK;				//[Unit=None]
    UINT LcomK;				//[Unit=None]
    int iMax;
    int iKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int iLSTerm;      // Long, Short Term 구별
    double dPu;       //[Unit=Force]
    double dVu;				//[Unit=Force]
    double dMu;				//[Unit=Moment]
    double dTu;				//[Unit=Moment]
    double dft;     //[Unit=Stress]
    double dfb;     //[Unit=Stress]
    double dVn;				//[Unit=Force]
    double dTn;				//[Unit=Moment]
    double dPhi;			//[Unit=None]
    double dPhi_t;		//[Unit=None]
    double dPhiVn;		//[Unit=Force]
    double dPhi_tTn;	//[Unit=Moment]
    double dAl;       //[Unit=Area]  
    double dAl_min;		//[Unit=Area]  
    double dAt;       //[Unit=Area]  
    double dAt_req;   //[Unit=Area]  
    double dTcr;      //[Unit=Moment]
    BOOL bOK;
    double dV_Rd_max; //[Unit=Force]
    double dComb_rat; //[Unit=None]
    //KSCE-LSD12
    double dAl_req;
    double ds_t;
    double dst_max_rep;
    double drho_st;
    double drho_st_min;
    //Eurocode2 PSC Box
    BOOL bPSC_BOX;
    double dT_Rd_c;
    double dV_Rd_c;
    double dComb_Rd_c_rat;
    BOOL bComb_Rd_c_rat_check;
    double dT_Rd_s;
    double dV_Rd_s;
    double dT_Rd_s_rat;
    double dV_Rd_s_rat;
    double dT_Rd_c_rat;  // T_Sd / T_Rd2
    double dV_Rd_c_rat;  // V_Sd / V_Rd2
    double dT_Rd_lr;
    double dT_Rd_lr_rat; // ABNT, T_Rd4, longitudinal reinforcement, ratio

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax=iKind=iLSTerm=0;
        dPu = dVu = dMu = dVn = dTn = dTu = 0.0;
        dft=dfb=0.0;
        dPhi = dPhiVn = dPhi_t = dPhi_tTn = 0.0;
        dAl = dAl_min = 0.0;		
        dAt = dAt_req = 0.0;
        dTcr = 0.0;		
        bOK = FALSE;
        dV_Rd_max = 0.0;
        dComb_rat = 0.0;
        dAl_req = ds_t = dst_max_rep = 0.0;
        drho_st = drho_st_min = 0.0;
        bPSC_BOX = FALSE;
        dT_Rd_c = dV_Rd_c = dComb_Rd_c_rat = 0.0;
        bComb_Rd_c_rat_check = FALSE;
        dT_Rd_s = dV_Rd_s = dT_Rd_s_rat = dV_Rd_s_rat = 0.0;
        dT_Rd_c_rat = 0.0;
        dV_Rd_c_rat = 0.0;
        dT_Rd_lr = 0.0;
        dT_Rd_lr_rat = 0.0;
    }

    void Convert875(const T_CRMT_PSC_BASE_875 &rData)
    {
        bCHK                 = rData.bCHK       ;
        LcomK                = rData.LcomK      ;
        iMax                 = rData.iMax       ;
        iKind                = rData.iKind      ;
        iLSTerm              = rData.iLSTerm    ;
        dPu                  = rData.dPu        ;
        dVu                  = rData.dVu        ;
        dMu                  = rData.dMu        ;
        dTu                  = rData.dTu        ;
        dft                  = rData.dft        ;
        dfb                  = rData.dfb        ;
        dVn                  = rData.dVn        ;
        dTn                  = rData.dTn        ;
        dPhi                 = rData.dPhi       ;
        dPhi_t               = rData.dPhi_t     ;
        dPhiVn               = rData.dPhiVn     ;
        dPhi_tTn             = rData.dPhi_tTn   ;
        dAl                  = rData.dAl        ;
        dAl_min              = rData.dAl_min    ;
        dAt                  = rData.dAt        ;
        dAt_req              = rData.dAt_req    ;
        dTcr                 = rData.dTcr       ;
        bOK                  = rData.bOK        ;
        dV_Rd_max            = rData.dV_Rd_max  ;
        dComb_rat            = rData.dComb_rat  ;
        dAl_req              = rData.dAl_req    ;
        ds_t                 = rData.ds_t       ;
        dst_max_rep          = rData.dst_max_rep;
        drho_st              = rData.drho_st    ;
        drho_st_min          = rData.drho_st_min;
        bPSC_BOX             = FALSE;
        dT_Rd_c              = 0.0;
        dV_Rd_c              = 0.0;
        dComb_Rd_c_rat       = 0.0;
        bComb_Rd_c_rat_check = FALSE;
        dT_Rd_s              = 0.0;
        dV_Rd_s              = 0.0;
        dT_Rd_s_rat          = 0.0;
        dV_Rd_s_rat          = 0.0;
    }
    T_CRMT_PSC_BASE() { Initialize(); }	
};

struct T_CRMT_PSC_875	// ElemK.
{
    T_CRMT_PSC_BASE_875 CrmtBase[6];	// 0=I-V-MAX, 1=I-V-MIN, 2=I-T-MAX, 3=J-V-MAX, 4=J-V-MIN, 5=J-T-MAX.

    void Initialize()
    {
        for(int i=0; i<6; i++)
            CrmtBase[i].Initialize();
    }
    T_CRMT_PSC_875() { Initialize(); }	
};

struct T_CRMT_PSC	// ElemK.
{
	T_CRMT_PSC_BASE CrmtBase[6];	// 0=I-V-MAX, 1=I-V-MIN, 2=I-T-MAX, 3=J-V-MAX, 4=J-V-MIN, 5=J-T-MAX.
	
	void Initialize()
	{
		for(int i=0; i<6; i++)
			CrmtBase[i].Initialize();
	}
    void Convert875(const T_CRMT_PSC_875& rData)
    {
        for(int i=0; i<6; i++)
        {
            CrmtBase[i].Convert875(rData.CrmtBase[i]);
        }
    }
    T_CRMT_PSC() { Initialize(); }	
};

struct T_CRMT_PSC_UNIT
{
	T_CRMT_PSC_UNIT()
	{
        dPu       = D_UNITSYS_BASE_FORCE;		
		dVu       = D_UNITSYS_BASE_FORCE;		
		dMu	      = D_UNITSYS_BASE_MOMENT;		
        dft       = D_UNITSYS_BASE_STRESS;
        dfb       = D_UNITSYS_BASE_STRESS;
		dTu       = D_UNITSYS_BASE_MOMENT;		
		dVn	      = D_UNITSYS_BASE_FORCE;		
		dTn	      = D_UNITSYS_BASE_MOMENT;		
		dPhi      = D_UNITSYS_NONE;	
		dPhi_t    = D_UNITSYS_NONE;	
		dPhiVn    = D_UNITSYS_BASE_FORCE;	
		dPhi_tTn  = D_UNITSYS_BASE_MOMENT;
		dAl       = D_UNITSYS_BASE_AREA;
        dAl_min   = D_UNITSYS_BASE_AREA;
		dAt       = D_UNITSYS_BASE_AREA;
		dAt_req   = D_UNITSYS_BASE_AREA;
		dTcr      = D_UNITSYS_BASE_MOMENT;
        dV_Rd_max = D_UNITSYS_BASE_FORCE;
        dAl_req   = D_UNITSYS_BASE_AREA;
        ds_t      = D_UNITSYS_BASE_LENGTH;
        dst_max_rep = D_UNITSYS_BASE_LENGTH;
        dT_Rd_c = D_UNITSYS_BASE_MOMENT;
        dV_Rd_c = D_UNITSYS_BASE_FORCE;
        dT_Rd_s = D_UNITSYS_BASE_MOMENT;
        dV_Rd_s = D_UNITSYS_BASE_FORCE;
	}

    int dPu;
	int	dVu;		
	int dMu;		
	int dTu;		
    int dft;
    int dfb;
	int dVn;		
	int dTn;		
	int dPhi;	
	int dPhi_t;	
	int dPhiVn;	
	int dPhi_tTn;
	int dAl;
    int dAl_min;
    int dAt;
    int dAt_req;
	int dTcr;
    int dV_Rd_max;
    int dAl_req;
    int ds_t;
    int dst_max_rep;
    int dT_Rd_c;
    int dV_Rd_c;
    int dT_Rd_s;
    int dV_Rd_s;
    
};

//IRC112 INDIA CODE Structure
// For PSC Result

//============================================================================
// 1. Stresses in concrete during construction stage
//============================================================================
struct T_FYSG_PSC_BASE_IRC
{
	BOOL bCHK;		//설계결과유무
	UINT StageK;	//스테이지번호		[Unit=None]
	UINT StepK;   // Step Key.      [Unit=None]
	double dFT;   //상단응력도
	double dFB;   //하단응력도
	double dFTL;	//상단좌단 합성응력도	[Unit=Stress]
	double dFBL;	//하단좌단 합성응력도	[Unit=Stress]
	double dFTR;	//상단우단 합성응력도	[Unit=Stress]
	double dFBR;	//하단우단 합성응력도	[Unit=Stress]
	double dFMAX;	//최대/최소 합성응력도	[Unit=Stress]
	double dALW;	//허용응력도			[Unit=Stress]
	BOOL bOK;

	void Initialize()
	{
		bCHK = 0;
		StageK = StepK=0;
		dFT = dFB = 0.0;
		dFTL = dFBL = dFTR = dFBR = dFMAX = dALW = 0;
		bOK = TRUE;
	}	
	T_FYSG_PSC_BASE_IRC() { Initialize(); }
};
struct T_FYSG_PSC_IRC	// ElemK.
{
	T_FYSG_PSC_BASE_IRC FysgBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

	void Initialize()
	{
		for(int i=0; i<4; i++)
			FysgBase[i].Initialize();
	}
	T_FYSG_PSC_IRC() { Initialize(); }
};

struct T_FYSG_PSC_UNIT_IRC
{
	T_FYSG_PSC_UNIT_IRC()
	{
		dFT		= D_UNITSYS_BASE_STRESS;
		dFB		= D_UNITSYS_BASE_STRESS;
		dFTL	= D_UNITSYS_BASE_STRESS;
		dFBL	= D_UNITSYS_BASE_STRESS;
		dFTR	= D_UNITSYS_BASE_STRESS;
		dFBR	= D_UNITSYS_BASE_STRESS;
		dFMAX	= D_UNITSYS_BASE_STRESS;
		dALW	= D_UNITSYS_BASE_STRESS;	
	}
	int dFT;
	int dFB;
	int dFTL;
	int dFBL;
	int dFTR;
	int dFBR;
	int dFMAX;
	int dALW;	
};

//==================================================================================
// 2. Stresses in concrete during service load combinations 
//==================================================================================

struct T_FYCM_PSC_BASE_IRC
{  
	BOOL bCHK;		
	UINT LcomK;
	int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
	//                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
	int iKind;    // 기준별 제시하는 하중조합 타입을 구별하기 위해.
	int iLSTerm;  // Long, Short Term 구별 
	double dFT;		//[Unit=Stress]
	double dFB;		//[Unit=Stress]
	double dFTL;	//[Unit=Stress]
	double dFBL;	//[Unit=Stress]
	double dFTR;	//[Unit=Stress]
	double dFBR;	//[Unit=Stress]
	double dFMAX;	//[Unit=Stress]
	double dALW;	//[Unit=Stress]
	BOOL bOK;

	void Initialize()
	{
		bCHK = 0;
		LcomK = iMax = iKind = iLSTerm = 0;
		dFT = dFB = 0.0;
		dFTL = dFBL = dFTR = dFBR = dFMAX = dALW = 0;
		bOK = TRUE;
	}
	T_FYCM_PSC_BASE_IRC() { Initialize(); }
};

struct T_FYCM_PSC_IRC	// ElemK.
{
	T_FYCM_PSC_BASE_IRC FycmBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

	void Initialize()
	{
		for(int i=0; i<4; i++) FycmBase[i].Initialize();
	}
	T_FYCM_PSC_IRC() { Initialize(); }	
};

struct T_FYCM_PSC_UNIT_IRC
{
	T_FYCM_PSC_UNIT_IRC()
	{
		dFT		= D_UNITSYS_BASE_STRESS;		
		dFB		= D_UNITSYS_BASE_STRESS;		
		dFTL	= D_UNITSYS_BASE_STRESS;
		dFBL	= D_UNITSYS_BASE_STRESS;
		dFTR	= D_UNITSYS_BASE_STRESS;	
		dFBR	= D_UNITSYS_BASE_STRESS;
		dFMAX	= D_UNITSYS_BASE_STRESS;
		dALW	= D_UNITSYS_BASE_STRESS;	
	}

	int dFT;		
	int dFB;		
	int dFTL;	
	int dFBL;	
	int dFTR;	
	int dFBR;	
	int dFMAX;	
	int dALW;	
};

//==================================================================================
// Tensile stresses in prestressing tendons
//==================================================================================

// struct T_FYPC_AASHTO	// TdnaK. 이걸 사용하자. 

// Check Principal stress for stage
struct T_FPSG_PSC_BASE_IRC 
{
	BOOL bCHK;		//설계결과유무 
	UINT StageK;
	UINT StepK;
	int iKind;    // 시공단계에서 필요할 수 있기 때문에. 
	double dFP1;	//1위치 주응력		[Unit=Stress]
	double dFP2;	//2위치 주응력		[Unit=Stress]
	double dFP3;	//3위치 주응력		[Unit=Stress]
	double dFP4;	//4위치 주응력		[Unit=Stress]
	double dFP5;	//5위치 주응력		[Unit=Stress]
	double dFP6;	//6위치 주응력		[Unit=Stress]
	double dFP7;	//7위치 주응력		[Unit=Stress]
	double dFP8;	//8위치 주응력		[Unit=Stress]
	double dFP9;	//9위치 주응력		[Unit=Stress]
	double dFP10;	//10위치 주응력		[Unit=Stress]
	double dFMAX; // MAX/MIN 주응력	[Unit=Stress]
	double dAFP;  // 허용 주응력    [Unit=Stress]
	BOOL bOK;

	void Initialize()
	{
		bCHK = 0;
		StageK = StepK = 0;
		iKind = 0;  
		iKind = 0;
		dFP1 = dFP2 = dFP3 = dFP4 = dFP5 = dFP6 = dFP7 = dFP8 = dFP9= dFP10 = dFMAX = dAFP = 0;
		bOK = TRUE;
	}
	T_FPSG_PSC_BASE_IRC() { Initialize(); }
};

struct T_FPSG_PSC_IRC	// ElemK.
{
	T_FPSG_PSC_BASE_IRC FpsgBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

	void Initialize()
	{
		for(int i=0; i<4; i++)
			FpsgBase[i].Initialize();
	}
	T_FPSG_PSC_IRC() { Initialize(); }
};

struct T_FPSG_PSC_UNIT_IRC
{
	T_FPSG_PSC_UNIT_IRC()
	{
		dFP1 = D_UNITSYS_BASE_STRESS;
		dFP2 = D_UNITSYS_BASE_STRESS;
		dFP3 = D_UNITSYS_BASE_STRESS;
		dFP4 = D_UNITSYS_BASE_STRESS;
		dFP5 = D_UNITSYS_BASE_STRESS;
		dFP6 = D_UNITSYS_BASE_STRESS;
		dFP7 = D_UNITSYS_BASE_STRESS;
		dFP8 = D_UNITSYS_BASE_STRESS;
		dFP9 = D_UNITSYS_BASE_STRESS;
		dFP10= D_UNITSYS_BASE_STRESS;
		dFMAX= D_UNITSYS_BASE_STRESS;
		dAFP = D_UNITSYS_BASE_STRESS; 
	}
	int dFP1;
	int dFP2;
	int dFP3;
	int dFP4;
	int dFP5;
	int dFP6;
	int dFP7;
	int dFP8;
	int dFP9;
	int dFP10;
	int dFMAX;
	int dAFP;  
};

// Check Principal stress
struct T_FPMS_PSC_BASE_IRC  
{
	BOOL bCHK;		//설계결과유무 
	UINT LcomK;
	int iMax;		  //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
	//                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
	int iKind;    // 기준별 제시하는 하중조합 타입을 구별하기 위해.
	int iLSTerm;  // Long, Short Term 구별 
	double dFP1;	//1위치 주응력		[Unit=Stress]
	double dFP2;	//2위치 주응력		[Unit=Stress]
	double dFP3;	//3위치 주응력		[Unit=Stress]
	double dFP4;	//4위치 주응력		[Unit=Stress]
	double dFP5;	//5위치 주응력		[Unit=Stress]
	double dFP6;	//6위치 주응력		[Unit=Stress]
	double dFP7;	//7위치 주응력		[Unit=Stress]
	double dFP8;	//8위치 주응력		[Unit=Stress]
	double dFP9;	//9위치 주응력		[Unit=Stress]
	double dFP10;	//10위치 주응력		[Unit=Stress]
	double dFMAX; // MAX/MIN 주응력	[Unit=Stress]
	double dAFP;  // 허용 주응력    [Unit=Stress]
	BOOL bOK;

	void Initialize()
	{
		bCHK = 0;
		LcomK = 0;
		iMax = 0;  
		iKind = iLSTerm = 0;
		dFP1 = dFP2 = dFP3 = dFP4 = dFP5 = dFP6 = dFP7 = dFP8 = dFP9= dFP10 = dFMAX = dAFP = 0;
		bOK = TRUE;
	}
	T_FPMS_PSC_BASE_IRC() { Initialize(); }
};

struct T_FPMS_PSC_IRC	// ElemK.
{
	T_FPMS_PSC_BASE_IRC FpmsBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

	void Initialize()
	{
		for(int i=0; i<4; i++)
			FpmsBase[i].Initialize();
	}
	T_FPMS_PSC_IRC() { Initialize(); }
};

struct T_FPMS_PSC_UNIT_IRC
{
	T_FPMS_PSC_UNIT_IRC()
	{
		dFP1 = D_UNITSYS_BASE_STRESS;
		dFP2 = D_UNITSYS_BASE_STRESS;
		dFP3 = D_UNITSYS_BASE_STRESS;
		dFP4 = D_UNITSYS_BASE_STRESS;
		dFP5 = D_UNITSYS_BASE_STRESS;
		dFP6 = D_UNITSYS_BASE_STRESS;
		dFP7 = D_UNITSYS_BASE_STRESS;
		dFP8 = D_UNITSYS_BASE_STRESS;
		dFP9 = D_UNITSYS_BASE_STRESS;
		dFP10= D_UNITSYS_BASE_STRESS;
		dFMAX= D_UNITSYS_BASE_STRESS;
		dAFP = D_UNITSYS_BASE_STRESS; 
	}
	int dFP1;
	int dFP2;
	int dFP3;
	int dFP4;
	int dFP5;
	int dFP6;
	int dFP7;
	int dFP8;
	int dFP9;
	int dFP10;
	int dFMAX;
	int dAFP;  
};

//==================================================================================
// Bending Resistance
//==================================================================================
struct T_BDCR_PSC_BASE_IRC
{
	BOOL bCHK;
	UINT LcomK;
	int iMax;
	int iKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.
	int iLSTerm;  // Long, Short Term 구별

	double dMu;	      // 계수모멘트, 작용부재력	[Unit=Moment]
	double dMn;       // 공칭강도, 제한값 적용, nominal flexural resistance [Unit=Moment]
	double dMn_cal;   // 공칭강도, 처음 계산, nominal flexural resistance [Unit=Moment]
	double dphi;      // 강도감소계수
	double dphiMn;    // phi*Mn
	double dRatM;     // Mu/(phi*Mn)
	double dc_neu;    // 중립축
	double dalpha;    //  0.85, 
	double dbeta1;    // a = beta1*c
	double da;        // 직사각형 Stress block a의 비
	double dPe;       // 인장측 유효긴장력			[Unit=Force]
	double dPe_t;     // 전체 유효긴장력        [Unit=Force]
	double dAps;	    // 인장측 PS강재의 단면적	[Unit=Area]
	double dAps_t;	  // 전체 PS강재의 단면적	[Unit=Area]
	double dAst;	    // 인장측 철근의 단면적		[Unit=Area]
	double dAsc;      // 압축측 철근의 단면적   [Unit=Area] 
	double dRho_pt;   // 인장측 tendon. [Unit=None]
	double dRho_st;   // 인장측 철근비			
	double dRho_sc;   // 압축측 철근비			
	double dRho_ps;   // 인장측 강재비(tendon+bar), (Ap+As)/(b*d) , q_b 강재지수?
	double ddpt;      // 압축연단에서 인장 Tendon의 도심까지 거리 [Unit=Length]
	double ddst;      // 압축연단에서 인장측 철근 도심까지의 거리		[Unit=Length]
	double ddsc;	    // 압축연단에서 압축측 철근 도심까지의 거리		[Unit=Length]  
	//
	double dphiMcr;   // 균열모멘트 [Unit=Moment]
	double dphiMn_mo; // Mn에 수정해서 사용할때 
	double dRho_p_max; // 최대 강재량 
	double dRho_p_min; // 최소 강재량   
	// KSCE-LSD12
	double dCmax;     // [L] 최대 중립축
	double dAs_max;   // [L^2] 인장철근 상한값
	double dAs_min;   // [L^2] 인장철근 하한값

	BOOL bOK;

	void Initialize()
	{
		bCHK = 0;
		LcomK=0;
		iMax=iKind=iLSTerm=0;
		dMu = dMn = dMn_cal = dphi = dphiMn = dRatM = dc_neu = dalpha = dbeta1 = da = 0.0;
		dPe = dPe_t = dAps = dAps_t = dAst = dAsc = 0.0;
		dRho_pt = dRho_st = dRho_sc = dRho_ps = 0.0;
		ddpt = ddst = ddsc = 0.0;
		dphiMcr = dphiMn_mo = 0.0;
		dRho_p_max = dRho_p_min = 0.0;
		dCmax = dAs_max = dAs_min = 0.0;
		bOK = 0;
	}
	T_BDCR_PSC_BASE_IRC() { Initialize(); }
};

struct T_BDCR_PSC_IRC	// ElemK.
{
	T_BDCR_PSC_BASE_IRC BdcrBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

	void Initialize()
	{
		for(int i=0; i<4; i++)
			BdcrBase[i].Initialize();
	}
	T_BDCR_PSC_IRC() { Initialize(); }
};

struct T_BDCR_PSC_UNIT_IRC
{
	T_BDCR_PSC_UNIT_IRC()
	{
		dMu       = D_UNITSYS_BASE_MOMENT;
		dMn       = D_UNITSYS_BASE_MOMENT;
		dMn_cal   = D_UNITSYS_BASE_MOMENT;
		dphiMn    = D_UNITSYS_BASE_MOMENT;
		dc_neu    = D_UNITSYS_BASE_LENGTH;
		da        = D_UNITSYS_BASE_LENGTH;
		dPe       = D_UNITSYS_BASE_FORCE; 
		dPe_t     = D_UNITSYS_BASE_FORCE; 
		dAps      = D_UNITSYS_BASE_AREA;	
		dAps_t    = D_UNITSYS_BASE_AREA;	
		dAst      = D_UNITSYS_BASE_AREA;	
		dAsc      = D_UNITSYS_BASE_AREA;  
		ddpt      = D_UNITSYS_BASE_LENGTH;
		ddst      = D_UNITSYS_BASE_LENGTH;
		ddsc      = D_UNITSYS_BASE_LENGTH;
		dphiMcr   = D_UNITSYS_BASE_MOMENT;
		dphiMn_mo = D_UNITSYS_BASE_MOMENT;
		dCmax     = D_UNITSYS_BASE_LENGTH;     
		dAs_max   = D_UNITSYS_BASE_AREA;
		dAs_min   = D_UNITSYS_BASE_AREA;


	}

	int dMu;
	int dMn;
	int dMn_cal;
	int dphiMn;
	int dc_neu;
	int da;
	int dPe; 
	int dPe_t; 
	int dAps;	
	int dAps_t;	
	int dAst;	
	int dAsc;  
	int ddpt;
	int ddst;
	int ddsc;
	int dphiMcr;
	int dphiMn_mo;
	int dCmax;     
	int dAs_max;   
	int dAs_min;   

};

//==================================================================================
// Crack check
//==================================================================================

struct T_BDCW_PSC_BASE_IRC
{
	BOOL bCHK;		//[Unit=None]

	UINT LcomK;		//[Unit=None] 
	int iKind;    // enum AASHTO_LCOMTYPE
	int iMax;
	int iLSTerm;  // Long, Short Term 구별

	double dPu;        //[Unit=Force]
	double dMu;        //[Unit=Moment]
	double dFT;		     //Top stress for 균열단면[Unit=Stress]
	double dFB;		     //Bottom stress for 균열단면[Unit=Stress]
	double dft;        //Top stress for gross sect. [Unit=Stress]
	double dfb;        //Bottom stress for gross sect.[Unit=Stress] 
	double dFSS;	     //[Unit=Stress]
	double dEpsilon_sm; // 철근 변형률,    [Unit=None]
	double dEpsilon_cm; // 콘크리트 변형률 [Unit=None]
	double dEpsi_sm_Epsi_cm; // ε_sm - ε_cm [Unit=None]
	double dSr_max;  // maximum crack spacing
	double ds_use;	//[Unit=Length]
	double ds_max;	//[Unit=Length]
	double dWk;     //[Unit=Length]
	double dWmax;   //[Unit=Length]
	double dRatio;    //[Unit=None]
	double dCrack_Asmin ; //[L^2]
	double dAst         ; //[L^2]

	BOOL bOK;

	BOOL bOK_Cr;
	BOOL bOk_Zero;


	void Initialize()
	{
		bCHK = 0;
		LcomK = 0;
		iKind=iMax=iLSTerm=0;
		dFT = dFB = dFSS = 0.0;
		dft = dfb = 0.0;
		dEpsilon_sm = dEpsilon_cm = 0.0;
		dEpsi_sm_Epsi_cm = 0.0;
		dSr_max = 0.0;
		dft = dfb = 0.0;
		ds_use = ds_max = 0.0;
		dWk=dWmax=0.0;
		dRatio=0.0;
		bOK = TRUE;
		dCrack_Asmin = dAst =0.0;
		bOK_Cr = FALSE;
		bOk_Zero = FALSE;

	}
	T_BDCW_PSC_BASE_IRC() { Initialize(); }	
};

struct T_BDCW_PSC_IRC_875// ElemK.
{
    T_BDCW_PSC_BASE_IRC BdcwBase[4];	    // 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    T_BDCW_PSC_BASE_IRC BdcwBaseZero[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
        {
            BdcwBase[i].Initialize();
            BdcwBaseZero[i].Initialize();
        }  
    }
    T_BDCW_PSC_IRC_875() { Initialize(); }		
};

struct T_BDCW_PSC_IRC// ElemK.
{
	T_BDCW_PSC_BASE_IRC BdcwBase[4];	    // 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	T_BDCW_PSC_BASE_IRC BdcwBaseZero[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    T_BDCW_PSC_BASE_IRC BdcwBaseAsmin[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

	void Initialize()
	{
		for(int i=0; i<4; i++)
		{
			BdcwBase[i].Initialize();
			BdcwBaseZero[i].Initialize();
            BdcwBaseAsmin[i].Initialize();
		}  
	}
    void Convert875(const T_BDCW_PSC_IRC_875& rData)
    {
        for(int i=0; i<4; i++)
        {
            BdcwBase[i] = rData.BdcwBase[i];
            BdcwBaseZero[i] = rData.BdcwBaseZero[i];
            BdcwBaseAsmin[i].Initialize();
        }
    }
	T_BDCW_PSC_IRC() { Initialize(); }		
};

struct T_BDCW_PSC_UNIT_IRC
{
	T_BDCW_PSC_UNIT_IRC()
	{
		dPu = D_UNITSYS_BASE_FORCE;
		dMu = D_UNITSYS_BASE_MOMENT;
		dFT = D_UNITSYS_BASE_STRESS;	
		dFB = D_UNITSYS_BASE_STRESS;	
		dft = D_UNITSYS_BASE_STRESS;	
		dfb = D_UNITSYS_BASE_STRESS;	
		dFSS= D_UNITSYS_BASE_STRESS;
		dSr_max = D_UNITSYS_BASE_LENGTH; 

		ds_use = D_UNITSYS_BASE_LENGTH;
		ds_max = D_UNITSYS_BASE_LENGTH;
		dWk    = D_UNITSYS_BASE_LENGTH;
		dWmax  = D_UNITSYS_BASE_LENGTH;

		dCrack_Asmin = D_UNITSYS_BASE_AREA;
		dAst         = D_UNITSYS_BASE_AREA;
	}

	int dPu;
	int dMu;
	int dFT;	
	int dFB;	
	int dft;
	int dfb;
	int dFSS;
	int dSr_max;
	int ds_use;
	int ds_max;
	int dWk;
	int dWmax;
	int dCrack_Asmin ; 
	int dAst         ; 

};

//==================================================================================
// Shear design
//==================================================================================
struct T_CUMS_PSC_BASE_IRC
{
	BOOL bCHK;			//[Unit=None]
	UINT LcomK;			//[Unit=None]
	int iMax;
	int iKind;      // 기준별 제시하는 하중조합 타입을 구별하기 위해.
	int iLSTerm;    // Long, Short Term 구별

	double dPu;     //[Unit=Force]
	double dVu;			//[Unit=Force]
	double dMu;			//[Unit=Moment]
	double dft;     //[Unit=Stress]
	double dfb;     //[Unit=Stress]
	double dVn;			//[Unit=Force]
	double dPhi;		//[Unit=None]
	double dVc;			//[Unit=Force]
	double dVs;			//[Unit=Force]	
	double dVp;			//[Unit=Force]
	double dPhiVn;		//[Unit=Force]
	double dAv_s;     //[Unit=LENGTH]
	double dAv_s_min;	//[Unit=LENGTH]
	double dAv_s_req;	//[Unit=LENGTH]
	double ds;        // 전단철근간격 [Unit=LENGTH]
	double ds_max;    // 전단철근간격 max [Unit=LENGTH]
	double dVu_pVn_Rat; // Vu/pVn, [Unit=None]
	BOOL bOK;

	// Eurocode2-2:05	
	double dV_Rd_max; 

	//KSCE-LSD12
	double dRho_w;      // [None]
	double dRho_w_min;  // [None]
	double ds_l_max;    //[L]

	void Initialize()
	{
		bCHK = 0;
		LcomK = 0;
		iMax=iKind=iLSTerm=0;
		dPu = dVu = dMu = dVn = 0.0;
		dft=dfb=0.0;
		dPhi = dPhiVn = 0.0;
		dVc = dVs = dVp = 0.0;
		dAv_s = dAv_s_min = dAv_s_req = 0.0;
		ds = ds_max = 0.0;
		bOK = 0;
		dV_Rd_max=0.0;
		dRho_w = dRho_w_min = ds_l_max =0.0;
	}
	T_CUMS_PSC_BASE_IRC() { Initialize(); }	
};

struct T_CUMS_PSC_IRC	// ElemK.
{
	T_CUMS_PSC_BASE_IRC CumsBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

	void Initialize()
	{
		for(int i=0; i<4; i++)
			CumsBase[i].Initialize();
	}
	T_CUMS_PSC_IRC() { Initialize(); }	
};

struct T_CUMS_PSC_UNIT_IRC
{
	T_CUMS_PSC_UNIT_IRC()
	{
		dPu   = D_UNITSYS_BASE_FORCE;		
		dVu		= D_UNITSYS_BASE_FORCE;		
		dMu		= D_UNITSYS_BASE_MOMENT;	
		dft   = D_UNITSYS_BASE_STRESS;
		dfb   = D_UNITSYS_BASE_STRESS;
		dVn		= D_UNITSYS_BASE_FORCE;		
		dVc		= D_UNITSYS_BASE_FORCE;		
		dVs		= D_UNITSYS_BASE_FORCE;		
		dVp		= D_UNITSYS_BASE_FORCE;		
		dPhiVn	= D_UNITSYS_BASE_FORCE;	
		dAv_s	= D_UNITSYS_BASE_LENGTH;	
		dAv_s_min = D_UNITSYS_BASE_LENGTH;
		dAv_s_req = D_UNITSYS_BASE_LENGTH;
		ds    = D_UNITSYS_BASE_LENGTH;
		ds_max = D_UNITSYS_BASE_LENGTH;		
		dV_Rd_max = D_UNITSYS_BASE_FORCE;
		ds_l_max = D_UNITSYS_BASE_LENGTH;
	}

	int dPu;
	int dVu;
	int dMu;		
	int dft;
	int dfb;
	int dVn;		
	int dVc;		
	int dVs;		
	int dVp;		
	int dPhiVn;	
	int dAv_s;	
	int dAv_s_min;	
	int dAv_s_req;	
	int ds;
	int ds_max;
	int dV_Rd_max;
	int ds_l_max;
};

//==================================================================================
// Torsion design
//==================================================================================
struct T_CRMT_PSC_BASE_IRC_875
{
    BOOL bCHK;				//[Unit=None]
    UINT LcomK;				//[Unit=None]
    int iMax;
    int iKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int iLSTerm;      // Long, Short Term 구별
    double dPu;       //[Unit=Force]
    double dVu;				//[Unit=Force]
    double dMu;				//[Unit=Moment]
    double dTu;				//[Unit=Moment]
    double dft;     //[Unit=Stress]
    double dfb;     //[Unit=Stress]
    double dVn;				//[Unit=Force]
    double dTn;				//[Unit=Moment]
    double dPhi;			//[Unit=None]
    double dPhi_t;		//[Unit=None]
    double dPhiVn;		//[Unit=Force]
    double dPhi_tTn;	//[Unit=Moment]
    double dAl;       //[Unit=Area]  
    double dAl_min;		//[Unit=Area]  
    double dAt;       //[Unit=Area]  
    double dAt_req;   //[Unit=Area]  
    double dTcr;      //[Unit=Moment]

    BOOL bOK;

    double dV_Rd_max; //[Unit=Force]
    double dComb_rat; //[Unit=None]

    //KSCE-LSD12
    double dAl_req;
    double ds_t;
    double dst_max_rep;
    double drho_st;
    double drho_st_min;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax=iKind=iLSTerm=0;
        dPu = dVu = dMu = dVn = dTn = dTu = 0.0;
        dft=dfb=0.0;
        dPhi = dPhiVn = dPhi_t = dPhi_tTn = 0.0;
        dAl = dAl_min = 0.0;		
        dAt = dAt_req = 0.0;
        dTcr = 0.0;		
        bOK = FALSE;
        dV_Rd_max = 0.0;
        dComb_rat = 0.0;
        dAl_req = ds_t = dst_max_rep = 0.0;
        drho_st = drho_st_min = 0.0;
    }
    T_CRMT_PSC_BASE_IRC_875() { Initialize(); }	
};

struct T_CRMT_PSC_BASE_IRC
{
    BOOL bCHK;				//[Unit=None]
    UINT LcomK;				//[Unit=None]
    int iMax;
    int iKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int iLSTerm;      // Long, Short Term 구별
    double dPu;       //[Unit=Force]
    double dVu;				//[Unit=Force]
    double dMu;				//[Unit=Moment]
    double dTu;				//[Unit=Moment]
    double dft;     //[Unit=Stress]
    double dfb;     //[Unit=Stress]
    double dVn;				//[Unit=Force]
    double dTn;				//[Unit=Moment]
    double dPhi;			//[Unit=None]
    double dPhi_t;		//[Unit=None]
    double dPhiVn;		//[Unit=Force]
    double dPhi_tTn;	//[Unit=Moment]
    double dAl;       //[Unit=Area]  
    double dAl_min;		//[Unit=Area]  
    double dAt;       //[Unit=Area]  
    double dAt_req;   //[Unit=Area]  
    double dTcr;      //[Unit=Moment]
    BOOL bOK;
    double dV_Rd_max; //[Unit=Force]
    double dComb_rat; //[Unit=None]
    //KSCE-LSD12
    double dAl_req;
    double ds_t;
    double dst_max_rep;
    double drho_st;
    double drho_st_min;
    //Eurocode2 PSC Box
    BOOL bPSC_BOX;
    double dT_Rd_c;
    double dV_Rd_c;
    double dComb_Rd_c_rat;
    BOOL bComb_Rd_c_rat_check;
    double dT_Rd_s;
    double dV_Rd_s;
    double dT_Rd_s_rat;
    double dV_Rd_s_rat;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax=iKind=iLSTerm=0;
        dPu = dVu = dMu = dVn = dTn = dTu = 0.0;
        dft=dfb=0.0;
        dPhi = dPhiVn = dPhi_t = dPhi_tTn = 0.0;
        dAl = dAl_min = 0.0;		
        dAt = dAt_req = 0.0;
        dTcr = 0.0;		
        bOK = FALSE;
        dV_Rd_max = 0.0;
        dComb_rat = 0.0;
        dAl_req = ds_t = dst_max_rep = 0.0;
        drho_st = drho_st_min = 0.0;
        bPSC_BOX = FALSE;
        dT_Rd_c = dV_Rd_c = dComb_Rd_c_rat = 0.0;
        bComb_Rd_c_rat_check = FALSE;
        dT_Rd_s = dV_Rd_s = dT_Rd_s_rat = dV_Rd_s_rat = 0.0;
    }

    void Convert875(const T_CRMT_PSC_BASE_IRC_875 &rData)
    {
        bCHK                 = rData.bCHK       ;
        LcomK                = rData.LcomK      ;
        iMax                 = rData.iMax       ;
        iKind                = rData.iKind      ;
        iLSTerm              = rData.iLSTerm    ;
        dPu                  = rData.dPu        ;
        dVu                  = rData.dVu        ;
        dMu                  = rData.dMu        ;
        dTu                  = rData.dTu        ;
        dft                  = rData.dft        ;
        dfb                  = rData.dfb        ;
        dVn                  = rData.dVn        ;
        dTn                  = rData.dTn        ;
        dPhi                 = rData.dPhi       ;
        dPhi_t               = rData.dPhi_t     ;
        dPhiVn               = rData.dPhiVn     ;
        dPhi_tTn             = rData.dPhi_tTn   ;
        dAl                  = rData.dAl        ;
        dAl_min              = rData.dAl_min    ;
        dAt                  = rData.dAt        ;
        dAt_req              = rData.dAt_req    ;
        dTcr                 = rData.dTcr       ;
        bOK                  = rData.bOK        ;
        dV_Rd_max            = rData.dV_Rd_max  ;
        dComb_rat            = rData.dComb_rat  ;
        dAl_req              = rData.dAl_req    ;
        ds_t                 = rData.ds_t       ;
        dst_max_rep          = rData.dst_max_rep;
        drho_st              = rData.drho_st    ;
        drho_st_min          = rData.drho_st_min;
        bPSC_BOX             = FALSE;
        dT_Rd_c              = 0.0;
        dV_Rd_c              = 0.0;
        dComb_Rd_c_rat       = 0.0;
        bComb_Rd_c_rat_check = FALSE;
        dT_Rd_s              = 0.0;
        dV_Rd_s              = 0.0;
        dT_Rd_s_rat          = 0.0;
        dV_Rd_s_rat          = 0.0;
    }
    T_CRMT_PSC_BASE_IRC() { Initialize(); }	
};

struct T_CRMT_PSC_IRC_875	// ElemK.
{
    T_CRMT_PSC_BASE_IRC_875 CrmtBase[6];	// 0=I-V-MAX, 1=I-V-MIN, 2=I-T-MAX, 3=J-V-MAX, 4=J-V-MIN, 5=J-T-MAX.

    void Initialize()
    {
        for(int i=0; i<6; i++)
            CrmtBase[i].Initialize();
    }
    T_CRMT_PSC_IRC_875() { Initialize(); }	
};

struct T_CRMT_PSC_IRC	// ElemK.
{
    T_CRMT_PSC_BASE_IRC CrmtBase[6];	// 0=I-V-MAX, 1=I-V-MIN, 2=I-T-MAX, 3=J-V-MAX, 4=J-V-MIN, 5=J-T-MAX.

    void Initialize()
    {
        for(int i=0; i<6; i++)
            CrmtBase[i].Initialize();
    }
    void Convert875(const T_CRMT_PSC_IRC_875& rData)
    {
        for(int i=0; i<6; i++)
        {
            CrmtBase[i].Convert875(rData.CrmtBase[i]);
        }
    }
    T_CRMT_PSC_IRC() { Initialize(); }	
};

struct T_CRMT_PSC_UNIT_IRC
{
    T_CRMT_PSC_UNIT_IRC()
    {
        dPu       = D_UNITSYS_BASE_FORCE;		
        dVu       = D_UNITSYS_BASE_FORCE;		
        dMu	      = D_UNITSYS_BASE_MOMENT;		
        dft       = D_UNITSYS_BASE_STRESS;
        dfb       = D_UNITSYS_BASE_STRESS;
        dTu       = D_UNITSYS_BASE_MOMENT;		
        dVn	      = D_UNITSYS_BASE_FORCE;		
        dTn	      = D_UNITSYS_BASE_MOMENT;		
        dPhi      = D_UNITSYS_NONE;	
        dPhi_t    = D_UNITSYS_NONE;	
        dPhiVn    = D_UNITSYS_BASE_FORCE;	
        dPhi_tTn  = D_UNITSYS_BASE_MOMENT;
        dAl       = D_UNITSYS_BASE_AREA;
        dAl_min   = D_UNITSYS_BASE_AREA;
        dAt       = D_UNITSYS_BASE_AREA;
        dAt_req   = D_UNITSYS_BASE_AREA;
        dTcr      = D_UNITSYS_BASE_MOMENT;
        dV_Rd_max = D_UNITSYS_BASE_FORCE;
        dAl_req   = D_UNITSYS_BASE_AREA;
        ds_t      = D_UNITSYS_BASE_LENGTH;
        dst_max_rep = D_UNITSYS_BASE_LENGTH;
        dT_Rd_c = D_UNITSYS_BASE_MOMENT;
        dV_Rd_c = D_UNITSYS_BASE_FORCE;
        dT_Rd_s = D_UNITSYS_BASE_MOMENT;
        dV_Rd_s = D_UNITSYS_BASE_FORCE;
    }

    int dPu;
    int	dVu;		
    int dMu;		
    int dTu;		
    int dft;
    int dfb;
    int dVn;		
    int dTn;		
    int dPhi;	
    int dPhi_t;	
    int dPhiVn;	
    int dPhi_tTn;
    int dAl;
    int dAl_min;
    int dAt;
    int dAt_req;
    int dTcr;
    int dV_Rd_max;
    int dAl_req;
    int ds_t;
    int dst_max_rep;
    int dT_Rd_c;
    int dV_Rd_c;
    int dT_Rd_s;
    int dV_Rd_s;

};

//==================================================================================
// 9. PSC Composite : Fatigue
//==================================================================================
struct T_FATG_PSC_BASE_IRC
{
	BOOL bCHK;	        	// 설계결과유무
	double dSigLcom_max; // [Unit=Stress] 
	double dSigLcom_min; // [Unit=Stress] 
	double dVuLcom_max ; // [Unit=Force] 
	double dVuLcom_min ; // [Unit=Force] 
	double dVc;

	UINT LcomK1;  
	double dStressRatio;	 // σc,max/fcd,fat	 [Unit=None]
	double dStressLimit01; // [Unit=None]           
	double dStressLimit02; // [Unit=None]           

	UINT LcomK2; 
	double dShearRatio01; // σc,max/νfcd,fat [Unit=None]
	double dShearRatio02; //  VEd,min/VEd,max  [Unit=None]
	double dStressLimit03;//                   [Unit=None]

	double dRatio;
	BOOL bOK;

	void Initialize()
	{
		bCHK = 0;
		dSigLcom_max =0.0;
		dSigLcom_min =0.0;
		dVuLcom_max  =0.0;
		dVuLcom_min  =0.0;
		dVc = 0.0;

		LcomK1 = 0;
		dStressRatio  =0.0;
		dStressLimit01=0.0;
		dStressLimit02=0.0;

		LcomK2 = 0; 
		dShearRatio01 =0.0;
		dShearRatio02 =0.0;
		dStressLimit03=0.0;

		dRatio = 0.0;
		bOK = TRUE;

	}
	T_FATG_PSC_BASE_IRC() { Initialize(); }	
};

struct T_FATG_PSC_IRC// ElemK.
{
	T_FATG_PSC_BASE_IRC FatgBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

	void Initialize()
	{
		for(int i=0; i<4; i++)
			FatgBase[i].Initialize();
	}
	T_FATG_PSC_IRC() { Initialize(); }		
};

struct T_FATG_PSC_UNIT_IRC
{
	T_FATG_PSC_UNIT_IRC()
	{
		dSigLcom_max = D_UNITSYS_BASE_STRESS;
		dSigLcom_min = D_UNITSYS_BASE_STRESS;
		dVuLcom_max  = D_UNITSYS_BASE_FORCE;
		dVuLcom_min  = D_UNITSYS_BASE_FORCE;
		dVc          = D_UNITSYS_BASE_FORCE;
	}

	int	dSigLcom_max;
	int	dSigLcom_min;
	int	dVuLcom_max ;
	int	dVuLcom_min ;
	int dVc;

};


//End IRC


// For PSC Composite Result

//============================================================================
// 1. PSC Composite : Stresses in concrete during construction stage
//============================================================================
struct T_FYSG_PSC_COMP_BASE
{
    BOOL bCHK;		//설계결과유무
    UINT StageK;	//스테이지번호		[Unit=None]
    UINT StepK;   // Step Key.      [Unit=None]
    double dFT;   //상단응력도
    double dFB;   //하단응력도
    double dFTL;	//상단좌단 합성응력도	[Unit=Stress]
    double dFBL;	//하단좌단 합성응력도	[Unit=Stress]
    double dFTR;	//상단우단 합성응력도	[Unit=Stress]
    double dFBR;	//하단우단 합성응력도	[Unit=Stress]
    double dFMAX;	//최대/최소 합성응력도	[Unit=Stress]
    double dALW;	//허용응력도			[Unit=Stress]
    double dfci;    // 초기압축강도
    double dDuration;
    double dRatio;
    BOOL bOK;
    
    void Initialize()
    {
        bCHK = 0;
        StageK = StepK=0;
        dFT = dFB = 0.0;
        dFTL = dFBL = dFTR = dFBR = dFMAX = dALW = 0.0;
        dfci = 0.0;
        dDuration = 0.0;
        dRatio = 0.0;
        bOK = TRUE;
    }	
    T_FYSG_PSC_COMP_BASE() { Initialize(); }
};
struct T_FYSG_PSC_COMP	// ElemK.
{
    T_FYSG_PSC_COMP_BASE FysgGrBase[4];	//(girder) 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    T_FYSG_PSC_COMP_BASE FysgSlBase[4];	//(slab)   0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    
    void Initialize()
    {
        for(int i=0; i<4; i++)
        {
            FysgGrBase[i].Initialize();
            FysgSlBase[i].Initialize();
        }
            
    }
    T_FYSG_PSC_COMP() { Initialize(); }
};

struct T_FYSG_PSC_COMP_UNIT
{
    T_FYSG_PSC_COMP_UNIT()
    {
        dFT		= D_UNITSYS_BASE_STRESS;
        dFB		= D_UNITSYS_BASE_STRESS;
        dFTL	= D_UNITSYS_BASE_STRESS;
        dFBL	= D_UNITSYS_BASE_STRESS;
        dFTR	= D_UNITSYS_BASE_STRESS;
        dFBR	= D_UNITSYS_BASE_STRESS;
        dFMAX	= D_UNITSYS_BASE_STRESS;
        dALW	= D_UNITSYS_BASE_STRESS;	
    }
    int dFT;
    int dFB;
    int dFTL;
    int dFBL;
    int dFTR;
    int dFBR;
    int dFMAX;
    int dALW;	
};


//==================================================================================
// 2. PSC Composite : Stresses in concrete during service load combinations 
//==================================================================================
struct T_FYCM_PSC_COMP_BASE
{  
    BOOL bCHK;		
    UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
    //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iKind;    // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int iLSTerm;  // Long, Short Term 구별 
    double dFT;		//[Unit=Stress]
    double dFB;		//[Unit=Stress]
    double dFTL;	//[Unit=Stress]
    double dFBL;	//[Unit=Stress]
    double dFTR;	//[Unit=Stress]
    double dFBR;	//[Unit=Stress]
    double dFMAX;	//[Unit=Stress]
    double dALW;	//[Unit=Stress]
    BOOL bOK;
    
    void Initialize()
    {
        bCHK = 0;
        LcomK = iMax = iKind = iLSTerm = 0;
        dFT = dFB = 0.0;
        dFTL = dFBL = dFTR = dFBR = dFMAX = dALW = 0;
        bOK = TRUE;
    }
    T_FYCM_PSC_COMP_BASE() { Initialize(); }
};
struct T_FYCM_PSC_COMP	// ElemK.
{
    T_FYCM_PSC_COMP_BASE FycmGrBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    T_FYCM_PSC_COMP_BASE FycmSlBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    
    void Initialize()
    {
        for(int i=0; i<4; i++)
        {
            FycmGrBase[i].Initialize();
            FycmSlBase[i].Initialize();
        }

    }
    T_FYCM_PSC_COMP() { Initialize(); }	
};

struct T_FYCM_PSC_COMP_UNIT
{
    T_FYCM_PSC_COMP_UNIT()
    {
        dFT		= D_UNITSYS_BASE_STRESS;		
        dFB		= D_UNITSYS_BASE_STRESS;		
        dFTL	= D_UNITSYS_BASE_STRESS;
        dFBL	= D_UNITSYS_BASE_STRESS;
        dFTR	= D_UNITSYS_BASE_STRESS;	
        dFBR	= D_UNITSYS_BASE_STRESS;
        dFMAX	= D_UNITSYS_BASE_STRESS;
        dALW	= D_UNITSYS_BASE_STRESS;	
    }
    
    int dFT;		
    int dFB;		
    int dFTL;	
    int dFBL;	
    int dFTR;	
    int dFBR;	
    int dFMAX;	
    int dALW;	
};

//==================================================================================
// 3. PSC Composite :  Check Principal stress for stage
//==================================================================================
struct T_FPSG_PSC_COMP_BASE  
{
    BOOL bCHK;		//설계결과유무 
    UINT StageK;
    UINT StepK;
    int iKind;    // 시공단계에서 필요할 수 있기 때문에. 
    double dFP1;	//1위치 주응력		[Unit=Stress]
    double dFP2;	//2위치 주응력		[Unit=Stress]
    double dFP3;	//3위치 주응력		[Unit=Stress]
    double dFP4;	//4위치 주응력		[Unit=Stress]
    double dFP5;	//5위치 주응력		[Unit=Stress]
    double dFP6;	//6위치 주응력		[Unit=Stress]
    double dFP7;	//7위치 주응력		[Unit=Stress]
    double dFP8;	//8위치 주응력		[Unit=Stress]
    double dFP9;	//9위치 주응력		[Unit=Stress]
    double dFP10;	//10위치 주응력		[Unit=Stress]
    double dFMAX; // MAX/MIN 주응력	[Unit=Stress]
    double dAFP;  // 허용 주응력    [Unit=Stress]
    BOOL bOK;
    
    void Initialize()
    {
        bCHK = 0;
        StageK = StepK = 0;
        iKind = 0;  
        iKind = 0;
        dFP1 = dFP2 = dFP3 = dFP4 = dFP5 = dFP6 = dFP7 = dFP8 = dFP9= dFP10 = dFMAX = dAFP = 0;
        bOK = TRUE;
    }
    T_FPSG_PSC_COMP_BASE() { Initialize(); }
};
struct T_FPSG_PSC_COMP	// ElemK.
{
    T_FPSG_PSC_COMP_BASE FpsgGrBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    T_FPSG_PSC_COMP_BASE FpsgSlBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    
    void Initialize()
    {
        for(int i=0; i<4; i++) 
        {
            FpsgGrBase[i].Initialize();
            FpsgGrBase[i].Initialize();  
        }
    }
    T_FPSG_PSC_COMP() { Initialize(); }
};

struct T_FPSG_PSC_COMP_UNIT
{
    T_FPSG_PSC_COMP_UNIT()
    {
        dFP1 = D_UNITSYS_BASE_STRESS;
        dFP2 = D_UNITSYS_BASE_STRESS;
        dFP3 = D_UNITSYS_BASE_STRESS;
        dFP4 = D_UNITSYS_BASE_STRESS;
        dFP5 = D_UNITSYS_BASE_STRESS;
        dFP6 = D_UNITSYS_BASE_STRESS;
        dFP7 = D_UNITSYS_BASE_STRESS;
        dFP8 = D_UNITSYS_BASE_STRESS;
        dFP9 = D_UNITSYS_BASE_STRESS;
        dFP10= D_UNITSYS_BASE_STRESS;
        dFMAX= D_UNITSYS_BASE_STRESS;
        dAFP = D_UNITSYS_BASE_STRESS; 
    }
    int dFP1;
    int dFP2;
    int dFP3;
    int dFP4;
    int dFP5;
    int dFP6;
    int dFP7;
    int dFP8;
    int dFP9;
    int dFP10;
    int dFMAX;
    int dAFP;  
};

//==================================================================================
// 4. PSC Composite :  Check Principal stress 
//==================================================================================
struct T_FPMS_PSC_COMP_BASE  
{
    BOOL bCHK;		//설계결과유무 
    UINT LcomK;
    int iMax;		  //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
    //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iKind;    // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int iLSTerm;  // Long, Short Term 구별 
    double dFP1;	//1위치 주응력		[Unit=Stress]
    double dFP2;	//2위치 주응력		[Unit=Stress]
    double dFP3;	//3위치 주응력		[Unit=Stress]
    double dFP4;	//4위치 주응력		[Unit=Stress]
    double dFP5;	//5위치 주응력		[Unit=Stress]
    double dFP6;	//6위치 주응력		[Unit=Stress]
    double dFP7;	//7위치 주응력		[Unit=Stress]
    double dFP8;	//8위치 주응력		[Unit=Stress]
    double dFP9;	//9위치 주응력		[Unit=Stress]
    double dFP10;	//10위치 주응력		[Unit=Stress]
    double dFMAX; // MAX/MIN 주응력	[Unit=Stress]
    double dAFP;  // 허용 주응력    [Unit=Stress]
    BOOL bOK;
    
    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;  
        iKind = iLSTerm = 0;
        dFP1 = dFP2 = dFP3 = dFP4 = dFP5 = dFP6 = dFP7 = dFP8 = dFP9= dFP10 = dFMAX = dAFP = 0;
        bOK = TRUE;
    }
    T_FPMS_PSC_COMP_BASE() { Initialize(); }
};

struct T_FPMS_PSC_COMP	// ElemK.
{
    T_FPMS_PSC_COMP_BASE FpmsGrBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    T_FPMS_PSC_COMP_BASE FpmsSlBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    
    void Initialize()
    {
        for(int i=0; i<4; i++)
        {
            FpmsGrBase[i].Initialize();
            FpmsSlBase[i].Initialize();
        }     
    }
    T_FPMS_PSC_COMP() { Initialize(); }
};

struct T_FPMS_PSC_COMP_UNIT
{
    T_FPMS_PSC_COMP_UNIT()
    {
        dFP1 = D_UNITSYS_BASE_STRESS;
        dFP2 = D_UNITSYS_BASE_STRESS;
        dFP3 = D_UNITSYS_BASE_STRESS;
        dFP4 = D_UNITSYS_BASE_STRESS;
        dFP5 = D_UNITSYS_BASE_STRESS;
        dFP6 = D_UNITSYS_BASE_STRESS;
        dFP7 = D_UNITSYS_BASE_STRESS;
        dFP8 = D_UNITSYS_BASE_STRESS;
        dFP9 = D_UNITSYS_BASE_STRESS;
        dFP10= D_UNITSYS_BASE_STRESS;
        dFMAX= D_UNITSYS_BASE_STRESS;
        dAFP = D_UNITSYS_BASE_STRESS; 
    }
    int dFP1;
    int dFP2;
    int dFP3;
    int dFP4;
    int dFP5;
    int dFP6;
    int dFP7;
    int dFP8;
    int dFP9;
    int dFP10;
    int dFMAX;
    int dAFP;  
};

//==================================================================================
// 5. PSC Composite : Bending Resistance
//==================================================================================
struct T_BDCR_PSC_COMP_BASE_856
{
    BOOL bCHK;
    UINT LcomK;
    int iMax;
    int iKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int iLSTerm;  // Long, Short Term 구별

    double dMu;	      // 계수모멘트, 작용부재력	[Unit=Moment]
    double dMn;       // 공칭강도, 제한값 적용, nominal flexural resistance [Unit=Moment]
    double dMn_cal;   // 공칭강도, 처음 계산, nominal flexural resistance [Unit=Moment]
    double dphi;      // 강도감소계수
    double dphiMn;    // phi*Mn
    double dRatM;     // Mu/(phi*Mn)
    double dc_neu;    // 중립축
    double dalpha;    //  0.85, 
    double dbeta1;    // a = beta1*c
    double da;        // 직사각형 Stress block a의 비
    double dPe;       // 인장측 유효긴장력			[Unit=Force]
    double dPe_t;     // 전체 유효긴장력        [Unit=Force]
    double dAps;	    // 인장측 PS강재의 단면적	[Unit=Area]
    double dAps_t;	  // 전체 PS강재의 단면적	[Unit=Area]
    double dAst;	    // 인장측 철근의 단면적		[Unit=Area]
    double dAsc;      // 압축측 철근의 단면적   [Unit=Area] 
    double dRho_pt;   // 인장측 tendon. [Unit=None]
    double dRho_st;   // 인장측 철근비			
    double dRho_sc;   // 압축측 철근비			
    double dRho_ps;   // 인장측 강재비(tendon+bar), (Ap+As)/(b*d) , q_b 강재지수?
    double ddpt;      // 압축연단에서 인장 Tendon의 도심까지 거리 [Unit=Length]
    double ddst;      // 압축연단에서 인장측 철근 도심까지의 거리		[Unit=Length]
    double ddsc;	    // 압축연단에서 압축측 철근 도심까지의 거리		[Unit=Length]  
    //
    double dphiMcr;   // 균열모멘트 [Unit=Moment]
    double dphiMn_mo; // Mn에 수정해서 사용할때 
    double dRho_p_max; // 최대 강재량 
    double dRho_p_min; // 최소 강재량   
    // KSCE-LSD12
    double dCmax;     // [L] 최대 중립축
    double dAs_max;   // [L^2] 인장철근 상한값
    double dAs_min;   // [L^2] 인장철근 하한값
    // Interface Shear
    double dz_sh;   //[L] 팔길이 from 휨검토
    double dCc_gr;    // 거더 압축력
    double dCc_sl;    // 슬래브 압축력

    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK=0;
        iMax=iKind=iLSTerm=0;
        dMu = dMn = dMn_cal = dphi = dphiMn = dRatM = dc_neu = dalpha = dbeta1 = da = 0.0;
        dPe = dPe_t = dAps = dAps_t = dAst = dAsc = 0.0;
        dRho_pt = dRho_st = dRho_sc = dRho_ps = 0.0;
        ddpt = ddst = ddsc = 0.0;
        dphiMcr = dphiMn_mo = 0.0;
        dRho_p_max = dRho_p_min = 0.0;
        dCmax = dAs_max = dAs_min = 0.0;
        dz_sh = 0.0;
        dCc_gr = dCc_sl = 0.0;
        bOK = 0;
    }
    T_BDCR_PSC_COMP_BASE_856() { Initialize(); }

};
struct T_BDCR_PSC_COMP_BASE
{
	BOOL bCHK;
    UINT LcomK;
    int iMax;
    int iKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int iLSTerm;  // Long, Short Term 구별
    
    double dMu;	      // 계수모멘트, 작용부재력	[Unit=Moment]
    double dMn;       // 공칭강도, 제한값 적용, nominal flexural resistance [Unit=Moment]
    double dMn_cal;   // 공칭강도, 처음 계산, nominal flexural resistance [Unit=Moment]
    double dphi;      // 강도감소계수
    double dphiMn;    // phi*Mn
    double dRatM;     // Mu/(phi*Mn)
    double dc_neu;    // 중립축
    double dalpha;    //  0.85, 
    double dbeta1;    // a = beta1*c
    double da;        // 직사각형 Stress block a의 비
    double dPe;       // 인장측 유효긴장력			[Unit=Force]
    double dPe_t;     // 전체 유효긴장력        [Unit=Force]
    double dAps;	    // 인장측 PS강재의 단면적	[Unit=Area]
    double dAps_t;	  // 전체 PS강재의 단면적	[Unit=Area]
    double dAst;	    // 인장측 철근의 단면적		[Unit=Area]
    double dAsc;      // 압축측 철근의 단면적   [Unit=Area] 
    double dRho_pt;   // 인장측 tendon. [Unit=None]
    double dRho_st;   // 인장측 철근비			
    double dRho_sc;   // 압축측 철근비			
    double dRho_ps;   // 인장측 강재비(tendon+bar), (Ap+As)/(b*d) , q_b 강재지수?
    double ddpt;      // 압축연단에서 인장 Tendon의 도심까지 거리 [Unit=Length]
    double ddst;      // 압축연단에서 인장측 철근 도심까지의 거리		[Unit=Length]
    double ddsc;	    // 압축연단에서 압축측 철근 도심까지의 거리		[Unit=Length]  
    //
    double dphiMcr;   // 균열모멘트 [Unit=Moment]
    double dphiMn_mo; // Mn에 수정해서 사용할때 
    double dRho_p_max; // 최대 강재량 
    double dRho_p_min; // 최소 강재량   
    // KSCE-LSD12
    double dCmax;     // [L] 최대 중립축
    double dAs_max;   // [L^2] 인장철근 상한값
    double dAs_min;   // [L^2] 인장철근 하한값
    // Interface Shear
	double dz_sh;   //[L] 팔길이 from 휨검토
    double dCc_gr;    // 거더 압축력
    double dCc_sl;    // 슬래브 압축력
    double dForce_T;  // 전체인장응력(Rebar + Tendon)
    double dTst_sl;   // 슬래브인장응력(Rebar)

    BOOL bOK;
    
	void Initialize()
	{
		bCHK = 0;
        LcomK=0;
        iMax=iKind=iLSTerm=0;
        dMu = dMn = dMn_cal = dphi = dphiMn = dRatM = dc_neu = dalpha = dbeta1 = da = 0.0;
        dPe = dPe_t = dAps = dAps_t = dAst = dAsc = 0.0;
        dRho_pt = dRho_st = dRho_sc = dRho_ps = 0.0;
        ddpt = ddst = ddsc = 0.0;
        dphiMcr = dphiMn_mo = 0.0;
        dRho_p_max = dRho_p_min = 0.0;
        dCmax = dAs_max = dAs_min = 0.0;
		dz_sh = 0.0;
        dCc_gr = dCc_sl = 0.0;
        dForce_T = dTst_sl = 0.0;
        bOK = 0;
	}
    T_BDCR_PSC_COMP_BASE() { Initialize(); }

    void Convert856(const T_BDCR_PSC_COMP_BASE_856& crData)
    {
        bCHK            = crData.bCHK       ;
        LcomK           = crData.LcomK      ;
        iMax            = crData.iMax       ;
        iKind           = crData.iKind      ;
        iLSTerm         = crData.iLSTerm    ;
                                                                                
        dMu	            = crData.dMu	      ;
        dMn             = crData.dMn        ;
        dMn_cal         = crData.dMn_cal    ;
        dphi            = crData.dphi       ;
        dphiMn          = crData.dphiMn     ;
        dRatM           = crData.dRatM      ;
        dc_neu          = crData.dc_neu     ;
        dalpha          = crData.dalpha     ;
        dbeta1          = crData.dbeta1     ;
        da              = crData.da         ;
        dPe             = crData.dPe        ;
        dPe_t           = crData.dPe_t      ;
        dAps	          = crData.dAps	      ;
        dAps_t	        = crData.dAps_t	    ;
        dAst	          = crData.dAst	      ;
        dAsc            = crData.dAsc       ;
        dRho_pt         = crData.dRho_pt    ;
        dRho_st         = crData.dRho_st    ;
        dRho_sc         = crData.dRho_sc    ;
        dRho_ps         = crData.dRho_ps    ;
        ddpt            = crData.ddpt       ;
        ddst            = crData.ddst       ;
        ddsc	          = crData.ddsc	      ;
                                                                                
        dphiMcr         = crData.dphiMcr    ;
        dphiMn_mo       = crData.dphiMn_mo  ;
        dRho_p_max      = crData.dRho_p_max ;
        dRho_p_min      = crData.dRho_p_min ;
                                                                                ;
        dCmax           = crData.dCmax      ;
        dAs_max         = crData.dAs_max    ;
        dAs_min         = crData.dAs_min    ;
                                                                                
        dz_sh           = crData.dz_sh      ;
        dCc_gr          = crData.dCc_gr     ;
        dCc_sl          = crData.dCc_sl     ;
        dForce_T        = 0.0   ;
        dTst_sl         = 0.0   ;
                                                                                
        bOK             = crData.bOK         ;
    }


};

struct T_BDCR_PSC_COMP_856	// ElemK.
{
    T_BDCR_PSC_COMP_BASE_856 BdcrBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            BdcrBase[i].Initialize();
    }
    T_BDCR_PSC_COMP_856() { Initialize(); }
};

struct T_BDCR_PSC_COMP	// ElemK.
{
	T_BDCR_PSC_COMP_BASE BdcrBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<4; i++)
			BdcrBase[i].Initialize();
	}
    T_BDCR_PSC_COMP() { Initialize(); }

    void Convert856(const T_BDCR_PSC_COMP_856& rData)
    {
        for(int i=0; i<4; i++)
            BdcrBase[i].Convert856(rData.BdcrBase[i]);
    }

};

struct T_BDCR_PSC_COMP_UNIT
{
	T_BDCR_PSC_COMP_UNIT()
	{
        dMu       = D_UNITSYS_BASE_MOMENT;
        dMn       = D_UNITSYS_BASE_MOMENT;
        dMn_cal   = D_UNITSYS_BASE_MOMENT;
        dphiMn    = D_UNITSYS_BASE_MOMENT;
        dc_neu    = D_UNITSYS_BASE_LENGTH;
        da        = D_UNITSYS_BASE_LENGTH;
        dPe       = D_UNITSYS_BASE_FORCE; 
        dPe_t     = D_UNITSYS_BASE_FORCE; 
        dAps      = D_UNITSYS_BASE_AREA;	
        dAps_t    = D_UNITSYS_BASE_AREA;	
        dAst      = D_UNITSYS_BASE_AREA;	
        dAsc      = D_UNITSYS_BASE_AREA;  
        ddpt      = D_UNITSYS_BASE_LENGTH;
        ddst      = D_UNITSYS_BASE_LENGTH;
        ddsc      = D_UNITSYS_BASE_LENGTH;
        dphiMcr   = D_UNITSYS_BASE_MOMENT;
        dphiMn_mo = D_UNITSYS_BASE_MOMENT;
        dCmax     = D_UNITSYS_BASE_LENGTH;     
        dAs_max   = D_UNITSYS_BASE_AREA;
        dAs_min   = D_UNITSYS_BASE_AREA;
		dz_sh     = D_UNITSYS_BASE_LENGTH;
        dCc_gr    = D_UNITSYS_BASE_FORCE; 
        dCc_sl    = D_UNITSYS_BASE_FORCE; 
        dForce_T  = D_UNITSYS_BASE_FORCE;
        dTst_sl   = D_UNITSYS_BASE_FORCE;

	}

    int dMu;
    int dMn;
    int dMn_cal;
    int dphiMn;
    int dc_neu;
    int da;
    int dPe; 
    int dPe_t; 
    int dAps;	
    int dAps_t;	
    int dAst;	
    int dAsc;  
    int ddpt;
    int ddst;
    int ddsc;
    int dphiMcr;
    int dphiMn_mo;
    int dCmax;     
    int dAs_max;   
    int dAs_min;  
	int dz_sh;
    int dCc_gr;
    int dCc_sl;
    int dForce_T;
    int dTst_sl;
};

//==================================================================================
// 6. PSC Composite : Crack check
//==================================================================================
struct T_BDCW_PSC_COMP_BASE
{
	BOOL bCHK;		//[Unit=None]
	
	UINT LcomK;		//[Unit=None] 
    int iKind;    // enum AASHTO_LCOMTYPE
    int iMax;
    int iLSTerm;  // Long, Short Term 구별

    double dPu;        //[Unit=Force]
    double dMu;        //[Unit=Moment]
    double dFT;		     //Top stress for 균열단면[Unit=Stress]
	double dFB;		     //Bottom stress for 균열단면[Unit=Stress]
    double dft;        //Top stress for gross sect. [Unit=Stress]
    double dfb;        //Bottom stress for gross sect.[Unit=Stress] 
	double dFSS;	     //[Unit=Stress]
    double dEpsilon_sm; // 철근 변형률,    [Unit=None]
    double dEpsilon_cm; // 콘크리트 변형률 [Unit=None]
    double dEpsi_sm_Epsi_cm; // ε_sm - ε_cm [Unit=None]
    double dSr_max;  // maximum crack spacing
	double ds_use;	//[Unit=Length]
	double ds_max;	//[Unit=Length]
    double dWk;     //[Unit=Length]
    double dWmax;   //[Unit=Length]
    double dRatio;    //[Unit=None]
    double dCrack_Asmin ; //[L^2]
    double dAst         ; //[L^2]
	
	BOOL bOK;
	
    BOOL bOK_Cr;
    BOOL bOk_Zero;

	void Initialize()
	{
		bCHK = 0;
		LcomK = 0;
        iKind=iMax=iLSTerm=0;
		dFT = dFB = dFSS = 0.0;
        dft = dfb = 0.0;
        dEpsilon_sm = dEpsilon_cm = 0.0;
        dEpsi_sm_Epsi_cm = 0.0;
        dSr_max = 0.0;
        dft = dfb = 0.0;
		ds_use = ds_max = 0.0;
        dWk=dWmax=0.0;
        dRatio=0.0;
		bOK = TRUE;
        dCrack_Asmin = dAst =0.0;
        bOK_Cr = FALSE;
        bOk_Zero = FALSE;

	}
    void Convert856(const T_BDCW_PSC_COMP_BASE& crData)
    {
        bCHK              =crData.bCHK             		;
        LcomK             =crData.LcomK               ;
        iKind             =crData.iKind               ;
        iMax              =crData.iMax                ;
        iLSTerm           =crData.iLSTerm             ;
        dPu               =crData.dPu                 ;
        dMu               =crData.dMu                 ;
        dFT		            =crData.dFT		              ;
        dFB		            =crData.dFB		              ;
        dft               =crData.dft                 ;
        dfb               =crData.dfb                 ;
        dFSS	            =crData.dFSS	              ;
        dEpsilon_sm       =crData.dEpsilon_sm         ;
        dEpsilon_cm       =crData.dEpsilon_cm         ;
        dEpsi_sm_Epsi_cm  =crData.dEpsi_sm_Epsi_cm    ;
        dSr_max           =crData.dSr_max             ;
        ds_use	          =crData.ds_use	            ;
        ds_max	          =crData.ds_max	            ;
        dWk               =crData.dWk                 ;
        dWmax             =crData.dWmax               ;
        dRatio            =crData.dRatio              ;
        dCrack_Asmin      =crData.dCrack_Asmin        ;
        dAst              =crData.dAst                ;
        bOK               =crData.bOK                 ;
        bOK_Cr            =crData.bOK_Cr              ;
        bOk_Zero          =crData.bOk_Zero            ;
    }

    T_BDCW_PSC_COMP_BASE() { Initialize(); }	
};

struct T_BDCW_PSC_COMP_856// ElemK.
{
    T_BDCW_PSC_COMP_BASE BdcwBase[4];	    // 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    T_BDCW_PSC_COMP_BASE BdcwBaseZero[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
        {
            BdcwBase[i].Initialize();
            BdcwBaseZero[i].Initialize();
        }
    }
    T_BDCW_PSC_COMP_856() { Initialize(); }		
};

struct T_BDCW_PSC_COMP// ElemK.
{
	T_BDCW_PSC_COMP_BASE BdcwBase[4];	    // 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    T_BDCW_PSC_COMP_BASE BdcwBaseZero[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	T_BDCW_PSC_COMP_BASE BdcwBaseAsmin[4];// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN. (v860 추가)

	void Initialize()
	{
		for(int i=0; i<4; i++)
        {
			BdcwBase[i].Initialize();
            BdcwBaseZero[i].Initialize();
            BdcwBaseAsmin[i].Initialize();
        }
	}

    void Convert856(const T_BDCW_PSC_COMP_856& rData)
    {
        for(int i=0; i<4; i++)
        {
            BdcwBase[i].Convert856(rData.BdcwBase[i]);
            BdcwBaseZero[i].Convert856(rData.BdcwBaseZero[i]);
            BdcwBaseAsmin[i].Initialize();
        }
    }
    T_BDCW_PSC_COMP() { Initialize(); }	
};


struct T_BDCW_PSC_COMP_UNIT
{
	T_BDCW_PSC_COMP_UNIT()
	{
        dPu = D_UNITSYS_BASE_FORCE;
        dMu = D_UNITSYS_BASE_MOMENT;
		dFT = D_UNITSYS_BASE_STRESS;	
		dFB = D_UNITSYS_BASE_STRESS;	
		dft = D_UNITSYS_BASE_STRESS;	
		dfb = D_UNITSYS_BASE_STRESS;	
		dFSS= D_UNITSYS_BASE_STRESS;
        dSr_max = D_UNITSYS_BASE_LENGTH; 

		ds_use = D_UNITSYS_BASE_LENGTH;
		ds_max = D_UNITSYS_BASE_LENGTH;
        dWk    = D_UNITSYS_BASE_LENGTH;
        dWmax  = D_UNITSYS_BASE_LENGTH;

        dCrack_Asmin = D_UNITSYS_BASE_AREA;
        dAst         = D_UNITSYS_BASE_AREA;
	}

    int dPu;
    int dMu;
	int dFT;	
	int dFB;	
    int dft;
    int dfb;
	int dFSS;
    int dSr_max;
	int ds_use;
	int ds_max;
    int dWk;
    int dWmax;
    int dCrack_Asmin ; 
    int dAst         ; 
};

//==================================================================================
// 7. PSC Composite : Shear design
//==================================================================================
struct T_CUMS_PSC_COMP_BASE
{
	BOOL bCHK;			//[Unit=None]
	UINT LcomK;			//[Unit=None]
    int iMax;
    int iKind;      // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int iLSTerm;    // Long, Short Term 구별

    double dPu;     //[Unit=Force]
	double dVu;			//[Unit=Force]
	double dMu;			//[Unit=Moment]
	double dMuMax;	//[Unit=Moment]
    double dft;     //[Unit=Stress]
    double dfb;     //[Unit=Stress]
	double dVn;			//[Unit=Force]
	double dPhi;		//[Unit=None]
	double dVc;			//[Unit=Force]
	double dVs;			//[Unit=Force]	
	double dVp;			//[Unit=Force]
	double dPhiVn;		//[Unit=Force]
    double dAv_s;     //[Unit=LENGTH]
	double dAv_s_min;	//[Unit=LENGTH]
	double dAv_s_req;	//[Unit=LENGTH]
    double ds;        // 전단철근간격 [Unit=LENGTH]
    double ds_max;    // 전단철근간격 max [Unit=LENGTH]
    double dVu_pVn_Rat; // Vu/pVn, [Unit=None]
    BOOL bOK;
 
    // Eurocode2-2:05	
    double dV_Rd_max; 

    //KSCE-LSD12
    double dRho_w;      // [None]
    double dRho_w_min;  // [None]
    double ds_l_max;    //[L]
    double dM_pre;      //[Unit=Moment]
    double dV_pre;      //[Unit=Force]

	void Initialize()
	{
		bCHK = 0;
		LcomK = 0;
        iMax=iKind=iLSTerm=0;
		dPu = dVu = dMu = dVn = 0.0;
		dMuMax = 0.0;
        dft=dfb=0.0;
		dPhi = dPhiVn = 0.0;
		dVc = dVs = dVp = 0.0;
		dAv_s = dAv_s_min = dAv_s_req = 0.0;
	  ds = ds_max = 0.0;
		bOK = 0;
        dV_Rd_max=0.0;
        dRho_w = dRho_w_min = ds_l_max =0.0;
        dM_pre = dV_pre =0.0;
	}
    T_CUMS_PSC_COMP_BASE() { Initialize(); }	
};

struct T_CUMS_PSC_COMP	// ElemK.
{
	T_CUMS_PSC_COMP_BASE CumsBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<4; i++)
			CumsBase[i].Initialize();
	}
    T_CUMS_PSC_COMP() { Initialize(); }	
};

struct T_CUMS_PSC_COMP_UNIT
{
	T_CUMS_PSC_COMP_UNIT()
	{
        dPu   = D_UNITSYS_BASE_FORCE;		
		dVu		= D_UNITSYS_BASE_FORCE;		
		dMu		= D_UNITSYS_BASE_MOMENT;	
		dMuMax= D_UNITSYS_BASE_MOMENT;	
        dft   = D_UNITSYS_BASE_STRESS;
        dfb   = D_UNITSYS_BASE_STRESS;
		dVn		= D_UNITSYS_BASE_FORCE;		
		dVc		= D_UNITSYS_BASE_FORCE;		
		dVs		= D_UNITSYS_BASE_FORCE;		
		dVp		= D_UNITSYS_BASE_FORCE;		
		dPhiVn	= D_UNITSYS_BASE_FORCE;	
		dAv_s	= D_UNITSYS_BASE_LENGTH;	
        dAv_s_min = D_UNITSYS_BASE_LENGTH;
        dAv_s_req = D_UNITSYS_BASE_LENGTH;
		ds    = D_UNITSYS_BASE_LENGTH;
		ds_max = D_UNITSYS_BASE_LENGTH;		
        dV_Rd_max = D_UNITSYS_BASE_FORCE;
        ds_l_max = D_UNITSYS_BASE_LENGTH;
        dM_pre		= D_UNITSYS_BASE_MOMENT;	
        dV_pre		= D_UNITSYS_BASE_FORCE;		   
	}
		
    int dPu;
	int dVu;
	int dMu;	
	int dMuMax;
    int dft;
    int dfb;
	int dVn;		
	int dVc;		
	int dVs;		
	int dVp;		
	int dPhiVn;	
	int dAv_s;	
	int dAv_s_min;	
	int dAv_s_req;	
	int ds;
	int ds_max;
    int dV_Rd_max;
    int ds_l_max;
    int dM_pre;     
    int dV_pre;     
};

//==================================================================================
// 8. PSC Composite : Torsion design
//==================================================================================
struct T_CRMT_PSC_COMP_BASE_875
{
    BOOL bCHK;				//[Unit=None]
    UINT LcomK;				//[Unit=None]
    int iMax;
    int iKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int iLSTerm;      // Long, Short Term 구별
    double dPu;       //[Unit=Force]
    double dVu;				//[Unit=Force]
    double dMu;				//[Unit=Moment]
    double dTu;				//[Unit=Moment]
    double dft;     //[Unit=Stress]
    double dfb;     //[Unit=Stress]
    double dVn;				//[Unit=Force]
    double dTn;				//[Unit=Moment]
    double dPhi;			//[Unit=None]
    double dPhi_t;		//[Unit=None]
    double dPhiVn;		//[Unit=Force]
    double dPhi_tTn;	//[Unit=Moment]
    double dAl;       //[Unit=Area]  
    double dAl_min;		//[Unit=Area]  
    double dAt;       //[Unit=Area]  
    double dAt_req;   //[Unit=Area]  
    double dTcr;      //[Unit=Moment]

    BOOL bOK;

    double dV_Rd_max; //[Unit=Force]
    double dComb_rat; //[Unit=None]

    //KSCE-LSD12
    double dAl_req;
    double ds_t;
    double dst_max_rep;
    double dM_pre;
    double dV_pre;  
    double drho_st;
    double drho_st_min;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax=iKind=iLSTerm=0;
        dPu = dVu = dMu = dVn = dTn = dTu = 0.0;
        dft=dfb=0.0;
        dPhi = dPhiVn = dPhi_t = dPhi_tTn = 0.0;
        dAl = dAl_min = 0.0;		
        dAt = dAt_req = 0.0;
        dTcr = 0.0;		
        bOK = FALSE;
        dV_Rd_max = 0.0;
        dComb_rat = 0.0;
        dAl_req = ds_t = dst_max_rep = 0.0;
        dM_pre = dV_pre = 0.0;  
        drho_st = drho_st_min =0.0;
    }
    T_CRMT_PSC_COMP_BASE_875() { Initialize(); }	
};

struct T_CRMT_PSC_COMP_BASE
{
	BOOL bCHK;				//[Unit=None]
	UINT LcomK;				//[Unit=None]
    int iMax;
    int iKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int iLSTerm;      // Long, Short Term 구별
    double dPu;       //[Unit=Force]
	double dVu;				//[Unit=Force]
	double dMu;				//[Unit=Moment]
	double dTu;				//[Unit=Moment]
    double dft;     //[Unit=Stress]
    double dfb;     //[Unit=Stress]
	double dVn;				//[Unit=Force]
	double dTn;				//[Unit=Moment]
	double dPhi;			//[Unit=None]
	double dPhi_t;		//[Unit=None]
	double dPhiVn;		//[Unit=Force]
	double dPhi_tTn;	//[Unit=Moment]
	double dAl;       //[Unit=Area]  
    double dAl_min;		//[Unit=Area]  
    double dAt;       //[Unit=Area]  
    double dAt_req;   //[Unit=Area]  
    double dTcr;      //[Unit=Moment]

	BOOL bOK;

    double dV_Rd_max; //[Unit=Force]
    double dComb_rat; //[Unit=None]

    //KSCE-LSD12
    double dAl_req;
    double ds_t;
    double dst_max_rep;
    double dM_pre;
    double dV_pre;  
    double drho_st;
    double drho_st_min;

    //Eurocode2 PSC Box
    BOOL bPSC_BOX;
    double dT_Rd_c;
    double dV_Rd_c;
    double dComb_Rd_c_rat;
    BOOL bComb_Rd_c_rat_check;
    double dT_Rd_s;
    double dV_Rd_s;
    double dT_Rd_s_rat;
    double dV_Rd_s_rat;
    double dT_Rd_c_rat;  // T_Sd / T_Rd2
	double dV_Rd_c_rat;  // V_Sd / V_Rd2
    double dT_Rd_lr;
	double dT_Rd_lr_rat; // ABNT, T_Rd4, longitudinal reinforcement, ratio

	void Initialize()
	{
		bCHK = 0;
		LcomK = 0;
		iMax=iKind=iLSTerm=0;
		dPu = dVu = dMu = dVn = dTn = dTu = 0.0;
        dft=dfb=0.0;
        dPhi = dPhiVn = dPhi_t = dPhi_tTn = 0.0;
		dAl = dAl_min = 0.0;		
		dAt = dAt_req = 0.0;
		dTcr = 0.0;		
		bOK = FALSE;
        dV_Rd_max = 0.0;
        dComb_rat = 0.0;
        dAl_req = ds_t = dst_max_rep = 0.0;
        dM_pre = dV_pre = 0.0;  
        drho_st = drho_st_min =0.0;
        bPSC_BOX = FALSE;
        dT_Rd_c = dV_Rd_c = dComb_Rd_c_rat = 0.0;
        bComb_Rd_c_rat_check = FALSE;
        dT_Rd_s = dV_Rd_s = dT_Rd_s_rat = dV_Rd_s_rat = 0.0;
        dT_Rd_c_rat = 0.0;
        dV_Rd_c_rat = 0.0;
        dT_Rd_lr = 0.0;
        dT_Rd_lr_rat = 0.0;
	}

    void Convert875(const T_CRMT_PSC_COMP_BASE_875 &rData)
    {
        bCHK                 = rData.bCHK       ;
        LcomK                = rData.LcomK      ;
        iMax                 = rData.iMax       ;
        iKind                = rData.iKind      ;
        iLSTerm              = rData.iLSTerm    ;
        dPu                  = rData.dPu        ;
        dVu                  = rData.dVu        ;
        dMu                  = rData.dMu        ;
        dTu                  = rData.dTu        ;
        dft                  = rData.dft        ;
        dfb                  = rData.dfb        ;
        dVn                  = rData.dVn        ;
        dTn                  = rData.dTn        ;
        dPhi                 = rData.dPhi       ;
        dPhi_t               = rData.dPhi_t     ;
        dPhiVn               = rData.dPhiVn     ;
        dPhi_tTn             = rData.dPhi_tTn   ;
        dAl                  = rData.dAl        ;
        dAl_min              = rData.dAl_min    ;
        dAt                  = rData.dAt        ;
        dAt_req              = rData.dAt_req    ;
        dTcr                 = rData.dTcr       ;
        bOK                  = rData.bOK        ;
        dV_Rd_max            = rData.dV_Rd_max  ;
        dComb_rat            = rData.dComb_rat  ;
        dAl_req              = rData.dAl_req    ;
        ds_t                 = rData.ds_t       ;
        dst_max_rep          = rData.dst_max_rep;
        drho_st              = rData.drho_st    ;
        drho_st_min          = rData.drho_st_min;
        dM_pre               = rData.dM_pre     ;
        dV_pre               = rData.dV_pre     ;
        bPSC_BOX             = FALSE;
        dT_Rd_c              = 0.0;
        dV_Rd_c              = 0.0;
        dComb_Rd_c_rat       = 0.0;
        bComb_Rd_c_rat_check = FALSE;
        dT_Rd_s              = 0.0;
        dV_Rd_s              = 0.0;
        dT_Rd_s_rat          = 0.0;
        dV_Rd_s_rat          = 0.0;
    }

    T_CRMT_PSC_COMP_BASE() { Initialize(); }	
};

struct T_CRMT_PSC_COMP_875	// ElemK.
{
    T_CRMT_PSC_COMP_BASE_875 CrmtBase[6];	// 0=I-V-MAX, 1=I-V-MIN, 2=I-T-MAX, 3=J-V-MAX, 4=J-V-MIN, 5=J-T-MAX.

    void Initialize()
    {
        for(int i=0; i<6; i++)
            CrmtBase[i].Initialize();
    }
    T_CRMT_PSC_COMP_875() { Initialize(); }	
};

struct T_CRMT_PSC_COMP	// ElemK.
{
	T_CRMT_PSC_COMP_BASE CrmtBase[6];	// 0=I-V-MAX, 1=I-V-MIN, 2=I-T-MAX, 3=J-V-MAX, 4=J-V-MIN, 5=J-T-MAX.
	
	void Initialize()
	{
		for(int i=0; i<6; i++)
			CrmtBase[i].Initialize();
	}
    void Convert875(const T_CRMT_PSC_COMP_875& rData)
    {
        for(int i=0; i<6; i++)
        {
            CrmtBase[i].Convert875(rData.CrmtBase[i]);
        }
    }
    T_CRMT_PSC_COMP() { Initialize(); }	
};

struct T_CRMT_PSC_COMP_UNIT
{
	T_CRMT_PSC_COMP_UNIT()
	{
        dPu       = D_UNITSYS_BASE_FORCE;		
		dVu       = D_UNITSYS_BASE_FORCE;		
		dMu	      = D_UNITSYS_BASE_MOMENT;		
        dft       = D_UNITSYS_BASE_STRESS;
        dfb       = D_UNITSYS_BASE_STRESS;
		dTu       = D_UNITSYS_BASE_MOMENT;		
		dVn	      = D_UNITSYS_BASE_FORCE;		
		dTn	      = D_UNITSYS_BASE_MOMENT;		
		dPhi      = D_UNITSYS_NONE;	
		dPhi_t    = D_UNITSYS_NONE;	
		dPhiVn    = D_UNITSYS_BASE_FORCE;	
		dPhi_tTn  = D_UNITSYS_BASE_MOMENT;
		dAl       = D_UNITSYS_BASE_AREA;
        dAl_min   = D_UNITSYS_BASE_AREA;
		dAt       = D_UNITSYS_BASE_AREA;
		dAt_req   = D_UNITSYS_BASE_AREA;
		dTcr      = D_UNITSYS_BASE_MOMENT;
        dV_Rd_max = D_UNITSYS_BASE_FORCE;
        dAl_req   = D_UNITSYS_BASE_AREA;
        ds_t      = D_UNITSYS_BASE_LENGTH;
        dst_max_rep = D_UNITSYS_BASE_LENGTH;
        dM_pre    = D_UNITSYS_BASE_MOMENT;
        dV_pre    = D_UNITSYS_BASE_FORCE;
        dT_Rd_c   = D_UNITSYS_BASE_MOMENT;
        dV_Rd_c   = D_UNITSYS_BASE_FORCE;
        dT_Rd_s   = D_UNITSYS_BASE_MOMENT;
        dV_Rd_s   = D_UNITSYS_BASE_FORCE;
	}

    int dPu;
	int	dVu;		
	int dMu;		
	int dTu;		
    int dft;
    int dfb;
	int dVn;		
	int dTn;		
	int dPhi;	
	int dPhi_t;	
	int dPhiVn;	
	int dPhi_tTn;
	int dAl;
    int dAl_min;
    int dAt;
    int dAt_req;
	int dTcr;
    int dV_Rd_max;
    int dAl_req;
    int ds_t;
    int dst_max_rep;
    int dM_pre;
    int dV_pre;
    int dT_Rd_c;
    int dV_Rd_c;
    int dT_Rd_s;
    int dV_Rd_s;
};


//==================================================================================
// 9. PSC Composite : Fatigue
//==================================================================================
struct T_FATG_PSC_BASE
{
	BOOL bCHK;	        	// 설계결과유무
	double dSigLcom_max; // [Unit=Stress] 
	double dSigLcom_min; // [Unit=Stress] 
	double dVuLcom_max ; // [Unit=Force] 
	double dVuLcom_min ; // [Unit=Force] 
	double dVc;

    UINT LcomK1;  
    double dStressRatio;	 // σc,max/fcd,fat	 [Unit=None]
    double dStressLimit01; // [Unit=None]           
	double dStressLimit02; // [Unit=None]           

	UINT LcomK2; 
	double dShearRatio01; // σc,max/νfcd,fat [Unit=None]
	double dShearRatio02; //  VEd,min/VEd,max  [Unit=None]
	double dStressLimit03;//                   [Unit=None]
	
	double dRatio;
    BOOL bOK;

	void Initialize()
	{
		bCHK = 0;
		dSigLcom_max =0.0;
		dSigLcom_min =0.0;
		dVuLcom_max  =0.0;
		dVuLcom_min  =0.0;
		dVc = 0.0;

		LcomK1 = 0;
		dStressRatio  =0.0;
		dStressLimit01=0.0;
		dStressLimit02=0.0;
		
		LcomK2 = 0; 
		dShearRatio01 =0.0;
		dShearRatio02 =0.0;
		dStressLimit03=0.0;

		dRatio = 0.0;
		bOK = TRUE;

	}
    T_FATG_PSC_BASE() { Initialize(); }	
};

struct T_FATG_PSC// ElemK.
{
	T_FATG_PSC_BASE FatgBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<4; i++)
			FatgBase[i].Initialize();
	}
    T_FATG_PSC() { Initialize(); }		
};

struct T_FATG_PSC_UNIT
{
	T_FATG_PSC_UNIT()
	{
		dSigLcom_max = D_UNITSYS_BASE_STRESS;
		dSigLcom_min = D_UNITSYS_BASE_STRESS;
		dVuLcom_max  = D_UNITSYS_BASE_FORCE;
		dVuLcom_min  = D_UNITSYS_BASE_FORCE;
		dVc          = D_UNITSYS_BASE_FORCE;
	}

	int	dSigLcom_max;
	int	dSigLcom_min;
	int	dVuLcom_max ;
	int	dVuLcom_min ;
	int dVc;

};



//////////////////////////////////////////////////////////////////////////
// SNiP, SP 35
//////////////////////////////////////////////////////////////////////////

//============================================================================
// 1. Stresses in concrete during construction stage
//============================================================================
struct T_FYSG_SNIP_BASE
{
	BOOL bCHK;		//설계결과유무
    UINT StageK;	//스테이지번호		[Unit=None]
    UINT StepK;   // Step Key.      [Unit=None]
    double dFT;   //상단응력도
    double dFB;   //하단응력도
    double dFTL;	//상단좌단 합성응력도	[Unit=Stress]
    double dFBL;	//하단좌단 합성응력도	[Unit=Stress]
    double dFTR;	//상단우단 합성응력도	[Unit=Stress]
    double dFBR;	//하단우단 합성응력도	[Unit=Stress]
    double dFMAX;	//최대/최소 합성응력도	[Unit=Stress]
    double dALW;	//허용응력도			[Unit=Stress]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        StageK = StepK=0;
        dFT = dFB = 0.0;
        dFTL = dFBL = dFTR = dFBR = dFMAX = dALW = 0;
        bOK = TRUE;
    }	
    T_FYSG_SNIP_BASE() { Initialize(); }
};
struct T_FYSG_SNIP	// ElemK.
{
	T_FYSG_SNIP_BASE FysgBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<4; ++i) FysgBase[i].Initialize();
	}
    T_FYSG_SNIP() { Initialize(); }
};

struct T_FYSG_SNIP_UNIT
{
	T_FYSG_SNIP_UNIT()
	{
		dFT		= D_UNITSYS_BASE_STRESS;
		dFB		= D_UNITSYS_BASE_STRESS;
		dFTL	= D_UNITSYS_BASE_STRESS;
		dFBL	= D_UNITSYS_BASE_STRESS;
		dFTR	= D_UNITSYS_BASE_STRESS;
		dFBR	= D_UNITSYS_BASE_STRESS;
		dFMAX	= D_UNITSYS_BASE_STRESS;
		dALW	= D_UNITSYS_BASE_STRESS;	
	}
	int dFT;
	int dFB;
	int dFTL;
	int dFBL;
	int dFTR;
	int dFBR;
	int dFMAX;
	int dALW;	
};

//==================================================================================
// 2. Stresses in concrete during service load combinations 
//==================================================================================

struct T_FYCM_SNIP_BASE
{  
	BOOL bCHK;		
	UINT LcomK;
    int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iKind;    // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int iLSTerm;  // Long, Short Term 구별 
	double dFT;		//[Unit=Stress]
	double dFB;		//[Unit=Stress]
	double dFTL;	//[Unit=Stress]
	double dFBL;	//[Unit=Stress]
	double dFTR;	//[Unit=Stress]
	double dFBR;	//[Unit=Stress]
	double dFMAX;	//[Unit=Stress]
	double dALW;	//[Unit=Stress]
	BOOL bOK;

	void Initialize()
	{
		bCHK = 0;
        LcomK = iMax = iKind = iLSTerm = 0;
		dFT = dFB = 0.0;
		dFTL = dFBL = dFTR = dFBR = dFMAX = dALW = 0;
		bOK = TRUE;
	}
    T_FYCM_SNIP_BASE() { Initialize(); }
};

struct T_FYCM_SNIP	// ElemK.
{
	T_FYCM_SNIP_BASE FycmBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<4; ++i) FycmBase[i].Initialize();
	}
    T_FYCM_SNIP() { Initialize(); }	
};

struct T_FYCM_SNIP_UNIT
{
	T_FYCM_SNIP_UNIT()
	{
		dFT		= D_UNITSYS_BASE_STRESS;		
		dFB		= D_UNITSYS_BASE_STRESS;		
		dFTL	= D_UNITSYS_BASE_STRESS;
		dFBL	= D_UNITSYS_BASE_STRESS;
		dFTR	= D_UNITSYS_BASE_STRESS;	
		dFBR	= D_UNITSYS_BASE_STRESS;
		dFMAX	= D_UNITSYS_BASE_STRESS;
		dALW	= D_UNITSYS_BASE_STRESS;	
	}

	int dFT;		
	int dFB;		
	int dFTL;	
	int dFBL;	
	int dFTR;	
	int dFBR;	
	int dFMAX;	
	int dALW;	
};

//==================================================================================
// Tensile stresses in prestressing tendons
//==================================================================================

struct T_FYPC_SNIP	// TdnaK.
{
	BOOL bCHK;	
    int nLoadType;  // 0=Pre-tension, 1=Post-tension 
		  
	double dFDL1;	// Pre-Tensioning: Stress prior to transfer. Jacking stress. [Unit=Stress]
					// Post-Tensioning: Stress prior to seating. Jacking stress. [Unit=Stress]

	double dFDL2;	// Pre-Tensioning: Not used. [Unit=Stress]		
					// Post-Tensioning: Stress at anchorage and couplers immediately after anchor set. [Unit=Stress]

	double dFDL3;	// Pre-Tensioning: Not used [Unit=Stress]
					// Post-Tensioning: Away from Anchorage and couplers immediately after anchor set. [Unit=stress]

	double dFLL1;	// Stress at service limit after all losses. [Unit=Stress]
	
	double dFLL2;	// Stress at strength or extreme event limit state. [Unit=Stress]

	double dAFDL1;	// Pre-Tensioning: Allowable stress prior to transfer. Allowable jacking stress. [Unit=Stress]
					// Post-Tensioning: Allowable stress prior to seating. Allowable jacking stress. [Unit=Stress]

	double dAFDL2;	// Pre-Tensioning: Not used. [Unit=Stress]		
					// Post-Tensioning: Allowable stress at anchorage and couplers immediately after anchor set. [Unit=Stress]

	double dAFDL3;	// Pre-Tensioning: Not used. [Unit=Stress]		
					// Post-Tensioning: Allowable stress at anchorage and couplers immediately after anchor set. [Unit=Stress]

	double dAFLL1; 	// Stress at service limit after all losses. [Unit=Stress]
	
	double dAFLL2;  // Stress at strength or extreme event limit state. [Unit=Stress]

	BOOL bOK;

	void Initialize()
	{
		bCHK = FALSE;
        nLoadType = 0; 
		dFDL1 = dFDL2 = dFDL3 = dFLL1 = dFLL2 = 0.0;
		dAFDL1 = dAFDL2 = dAFDL3 = dAFLL1 = dAFLL2 = 0.0;
		bOK = TRUE;
	}
    T_FYPC_SNIP() { Initialize(); }
};

struct T_FYPC_SNIP_UNIT
{
	T_FYPC_SNIP_UNIT()
	{
		dFDL1	= D_UNITSYS_BASE_STRESS;
		dFDL2	= D_UNITSYS_BASE_STRESS;
		dFDL3	= D_UNITSYS_BASE_STRESS;
		dFLL1	= D_UNITSYS_BASE_STRESS;
		dFLL2	= D_UNITSYS_BASE_STRESS;
		dAFDL1	= D_UNITSYS_BASE_STRESS;
		dAFDL2	= D_UNITSYS_BASE_STRESS;
		dAFDL3	= D_UNITSYS_BASE_STRESS;
		dAFLL1	= D_UNITSYS_BASE_STRESS;
		dAFLL2	= D_UNITSYS_BASE_STRESS;
	}

	int dFDL1;
	int dFDL2;
	int dFDL3;
	int dFLL1;
	int dFLL2;
	int dAFDL1;
	int dAFDL2;
	int dAFDL3;
	int dAFLL1;
	int dAFLL2;
};


// Check Principal stress for stage
struct T_FPSG_SNIP_BASE  
{
    BOOL bCHK;		//설계결과유무 
    UINT StageK;
    UINT StepK;
    int iKind;    // 시공단계에서 필요할 수 있기 때문에. 
    double dFP1;	//1위치 주응력		[Unit=Stress]
    double dFP2;	//2위치 주응력		[Unit=Stress]
    double dFP3;	//3위치 주응력		[Unit=Stress]
    double dFP4;	//4위치 주응력		[Unit=Stress]
    double dFP5;	//5위치 주응력		[Unit=Stress]
    double dFP6;	//6위치 주응력		[Unit=Stress]
    double dFP7;	//7위치 주응력		[Unit=Stress]
    double dFP8;	//8위치 주응력		[Unit=Stress]
    double dFP9;	//9위치 주응력		[Unit=Stress]
    double dFP10;	//10위치 주응력		[Unit=Stress]
    double dFMAX; // MAX/MIN 주응력	[Unit=Stress]
    double dAFP;  // 허용 주응력    [Unit=Stress]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        StageK = StepK = 0;
        iKind = 0;  
        iKind = 0;
        dFP1 = dFP2 = dFP3 = dFP4 = dFP5 = dFP6 = dFP7 = dFP8 = dFP9= dFP10 = dFMAX = dAFP = 0;
        bOK = TRUE;
    }
    T_FPSG_SNIP_BASE() { Initialize(); }
};

struct T_FPSG_SNIP	// ElemK.
{
	T_FPSG_SNIP_BASE FpsgBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<4; ++i) FpsgBase[i].Initialize();
	}
    T_FPSG_SNIP() { Initialize(); }
};

struct T_FPSG_SNIP_UNIT
{
    T_FPSG_SNIP_UNIT()
    {
        dFP1 = D_UNITSYS_BASE_STRESS;
        dFP2 = D_UNITSYS_BASE_STRESS;
        dFP3 = D_UNITSYS_BASE_STRESS;
        dFP4 = D_UNITSYS_BASE_STRESS;
        dFP5 = D_UNITSYS_BASE_STRESS;
        dFP6 = D_UNITSYS_BASE_STRESS;
        dFP7 = D_UNITSYS_BASE_STRESS;
        dFP8 = D_UNITSYS_BASE_STRESS;
        dFP9 = D_UNITSYS_BASE_STRESS;
        dFP10= D_UNITSYS_BASE_STRESS;
        dFMAX= D_UNITSYS_BASE_STRESS;
        dAFP = D_UNITSYS_BASE_STRESS; 
    }
    int dFP1;
    int dFP2;
    int dFP3;
    int dFP4;
    int dFP5;
    int dFP6;
    int dFP7;
    int dFP8;
    int dFP9;
    int dFP10;
    int dFMAX;
    int dAFP;  
};

// Check Principal stress
struct T_FPMS_SNIP_BASE  
{
    BOOL bCHK;		//설계결과유무 
    UINT LcomK;
    int iMax;		  //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iKind;    // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int iLSTerm;  // Long, Short Term 구별 
    double dFP1;	//1위치 주응력		[Unit=Stress]
    double dFP2;	//2위치 주응력		[Unit=Stress]
    double dFP3;	//3위치 주응력		[Unit=Stress]
    double dFP4;	//4위치 주응력		[Unit=Stress]
    double dFP5;	//5위치 주응력		[Unit=Stress]
    double dFP6;	//6위치 주응력		[Unit=Stress]
    double dFP7;	//7위치 주응력		[Unit=Stress]
    double dFP8;	//8위치 주응력		[Unit=Stress]
    double dFP9;	//9위치 주응력		[Unit=Stress]
    double dFP10;	//10위치 주응력		[Unit=Stress]
    double dFMAX; // MAX/MIN 주응력	[Unit=Stress]
    double dAFP;  // 허용 주응력    [Unit=Stress]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;  
        iKind = iLSTerm = 0;
        dFP1 = dFP2 = dFP3 = dFP4 = dFP5 = dFP6 = dFP7 = dFP8 = dFP9= dFP10 = dFMAX = dAFP = 0;
        bOK = TRUE;
    }
    T_FPMS_SNIP_BASE() { Initialize(); }
};

struct T_FPMS_SNIP	// ElemK.
{
	T_FPMS_SNIP_BASE FpmsBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<4; ++i) FpmsBase[i].Initialize();
	}
    T_FPMS_SNIP() { Initialize(); }
};

struct T_FPMS_SNIP_UNIT
{
    T_FPMS_SNIP_UNIT()
    {
        dFP1 = D_UNITSYS_BASE_STRESS;
        dFP2 = D_UNITSYS_BASE_STRESS;
        dFP3 = D_UNITSYS_BASE_STRESS;
        dFP4 = D_UNITSYS_BASE_STRESS;
        dFP5 = D_UNITSYS_BASE_STRESS;
        dFP6 = D_UNITSYS_BASE_STRESS;
        dFP7 = D_UNITSYS_BASE_STRESS;
        dFP8 = D_UNITSYS_BASE_STRESS;
        dFP9 = D_UNITSYS_BASE_STRESS;
        dFP10= D_UNITSYS_BASE_STRESS;
        dFMAX= D_UNITSYS_BASE_STRESS;
        dAFP = D_UNITSYS_BASE_STRESS; 
    }
    int dFP1;
    int dFP2;
    int dFP3;
    int dFP4;
    int dFP5;
    int dFP6;
    int dFP7;
    int dFP8;
    int dFP9;
    int dFP10;
    int dFMAX;
    int dAFP;  
};

struct T_BDCR_SNIP_BASE
{
	BOOL bCHK;
    UINT LcomK;
    int iMax;
    int iKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int iLSTerm;  // Long, Short Term 구별
    
    double dMu;	      // 계수모멘트, 작용부재력	[Unit=Moment]
    double dMr;       // nominal flexural resistance [Unit=Moment]
    double dRatM;     // Mu/(phi*Mn)
    double dXi;   // 균열모멘트 [Unit=Moment]
    double dXiy;
    
    BOOL bOK;
    
	void Initialize()
	{
		bCHK = 0;
        LcomK=0;
        iMax=iKind=iLSTerm=0;
        dMu = dMr = dRatM = 0.0;
        dXi = dXiy = 0.0;
		bOK = TRUE;
	}
    T_BDCR_SNIP_BASE() { Initialize(); }
};

struct T_BDCR_SNIP	// ElemK.
{
	T_BDCR_SNIP_BASE BdcrBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<4; ++i) BdcrBase[i].Initialize();
	}
    T_BDCR_SNIP() { Initialize(); }
};

struct T_BDCR_SNIP_UNIT
{
	T_BDCR_SNIP_UNIT()
	{
        dMu       = D_UNITSYS_BASE_MOMENT;
        dMr       = D_UNITSYS_BASE_MOMENT;
        dXi       = D_UNITSYS_NONE;
        dXiy      = D_UNITSYS_NONE;

	}

    int dMu;
    int dMr;
    int dXi;
    int dXiy;

};

//==================================================================================
// Crack check
//==================================================================================

struct T_BDCW_SNIP_BASE
{
	BOOL bCHK;		//[Unit=None]
	
	UINT LcomK;		//[Unit=None] 
    int iKind;    // 
    int iMax;
    int iLSTerm;  // Long, Short Term 구별

    double dPu;
    double dQu;
    double dMuy;
    double dMuz;
    double dTu;  
    double dSigma_top;
    double dSigma_bot;
    double dSigma_bx;          // [Unit=Stress]
    double dSigma_mt;          // [Unit=Stress]
    double dSigma_mc;          // [Unit=Stress]
    double dRb_mc2;            // σ_bca, σ_mca, [Unit=Stress]
    double dSigma_bt;          // [Unit=Stress]
    double dSigma_bt_Alw;      // σ_bta, [Unit=Stress]
    double dSigma_bc_min;      // [Unit=Stress]
    double dSigma_bc_min_Alw;  // [Unit=Stress]
    double dSigma_mt_Alw;      // [Unit=Stress]
    double dAlpha_cr;          // [Unit=Length]
    double dDelta_cr;          // [Unit=Length]
    double dRatW;              // α_cr/Δ_cr
	
	BOOL bOK;
	
	void Initialize()
	{
		bCHK = 0;
		LcomK = 0;
        iKind=iMax=iLSTerm=0;
        dPu=dQu=0.0;
        dMuy=dMuz=dTu=0.0;
        dSigma_top = dSigma_bot = 0.0;
		dSigma_bx = dSigma_mt = dSigma_mc = 0.0;
        dRb_mc2 = 0.0;
        dSigma_bt = dSigma_bt_Alw = 0.0;
        dSigma_bc_min = dSigma_bc_min_Alw = dSigma_mt_Alw = 0.0;
        dAlpha_cr = dDelta_cr = 0.0;
		bOK = TRUE;
	}
    T_BDCW_SNIP_BASE() { Initialize(); }	
};

struct T_BDCW_SNIP// ElemK.
{
	T_BDCW_SNIP_BASE BdcwBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<4; ++i) BdcwBase[i].Initialize();
	}
    T_BDCW_SNIP() { Initialize(); }		
};

struct T_BDCW_SNIP_UNIT
{
	T_BDCW_SNIP_UNIT()
	{
        dPu=dQu=D_UNITSYS_BASE_FORCE;
        dMuy=dMuz=dTu=D_UNITSYS_BASE_MOMENT;
        dSigma_bx  = D_UNITSYS_BASE_STRESS;   
        dSigma_top = D_UNITSYS_BASE_STRESS;
        dSigma_bot = D_UNITSYS_BASE_STRESS;
        dSigma_mt  = D_UNITSYS_BASE_STRESS;          
        dSigma_mc  = D_UNITSYS_BASE_STRESS;          
        dRb_mc2    = D_UNITSYS_BASE_STRESS;            
        dSigma_bt  = D_UNITSYS_BASE_STRESS;          
        dSigma_bt_Alw = D_UNITSYS_BASE_STRESS;      
        dSigma_bc_min = D_UNITSYS_BASE_STRESS;      
        dSigma_bc_min_Alw = D_UNITSYS_BASE_STRESS;  
        dSigma_mt_Alw = D_UNITSYS_BASE_STRESS;      
        dAlpha_cr     = D_UNITSYS_BASE_LENGTH;          
        dDelta_cr     = D_UNITSYS_BASE_LENGTH;          
	}

    int dPu, dQu;
    int dMuy, dMuz, dTu;
    int dSigma_top;
    int dSigma_bot;
    int dSigma_bx;          
    int dSigma_mt;          
    int dSigma_mc;          
    int dRb_mc2;            
    int dSigma_bt;          
    int dSigma_bt_Alw;      
    int dSigma_bc_min;      
    int dSigma_bc_min_Alw;  
    int dSigma_mt_Alw;      
    int dAlpha_cr;          
    int dDelta_cr;          

};

struct T_CUMS_SNIP_BASE
{
	BOOL bCHK;			//[Unit=None]
	UINT LcomK;			//[Unit=None]
    UINT LcomQuK;
    int iMax;
    int iKind;      // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int iLSTerm;    // Long, Short Term 구별
    BOOL bIsSLS;

    double dPu;     //[Unit=Force]
	double dQu;			//[Unit=Force]
    double dQuSLS;
	double dMu;			//[Unit=Moment]
    double dQr;     //[Unit=Force]
    double dMr;     //[Unit=Moment]
    double dRatQ;   //[Unit=None]
    double dRatM;   //[Unit=None]
    
    BOOL bOK; 

	void Initialize()
	{
		bCHK = 0;
		LcomK = 0;
        LcomQuK = 0;
        iMax=iKind=iLSTerm=0;
        bIsSLS = FALSE;
		dPu = dQu = dMu = 0.0;
        dQuSLS = 0.0;
        dQr = dMr = 0.0;
        dRatQ = dRatM = 0.0;
        bOK = TRUE;
	}
    T_CUMS_SNIP_BASE() { Initialize(); }	
};

struct T_CUMS_SNIP	// ElemK.
{
	T_CUMS_SNIP_BASE CumsBase[2];	// 0=I-MAX, 1=J-MAX
	
	void Initialize()
	{
		for(int i=0; i<2; ++i) CumsBase[i].Initialize();
	}
    T_CUMS_SNIP() { Initialize(); }	
};

struct T_CUMS_SNIP_UNIT
{
	T_CUMS_SNIP_UNIT()
	{
        dPu   = D_UNITSYS_BASE_FORCE;		
		dQu		= D_UNITSYS_BASE_FORCE;		
		dMu		= D_UNITSYS_BASE_MOMENT;	
        dQr   = D_UNITSYS_BASE_FORCE;
        dMr   = D_UNITSYS_BASE_MOMENT;
	}
		
    int dPu;
	int dQu;
	int dMu;		
    int dQr;
    int dMr;
};

struct T_CRMT_SNIP_BASE
{
	BOOL bCHK;				//[Unit=None]
	UINT LcomK;				//[Unit=None]
    UINT LcomQuK;
    int iMax;
    int iKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int iLSTerm;      // Long, Short Term 구별
    BOOL bIsSLS;
    double dPu;       //[Unit=Force]
	double dQu;				//[Unit=Force]
    double dQuSLS;
	double dMu;				//[Unit=Moment]
	double dTu;				//[Unit=Moment]
    double dTr;       //[Unit=Moment]
    double dRatT;     //[Unit=None]
    double dIntRatTM; //[Unit=None]
    double dIntRatTQ; //[Unit=None]

	BOOL bOK;
    
	void Initialize()
	{
		bCHK = 0;
		LcomK = 0;
        LcomQuK = 0;
		iMax=iKind=iLSTerm=0;
        bIsSLS = TRUE;
		dPu = dQu = dMu = dTu = dTr = 0.0;
        dQuSLS = 0.0;
        dRatT = dIntRatTM = dIntRatTQ = 0.0;
		bOK = TRUE;
	}
    T_CRMT_SNIP_BASE() { Initialize(); }	
};

struct T_CRMT_SNIP	// ElemK.
{
	T_CRMT_SNIP_BASE CrmtBase[2];	// 0=I-T-MAX, 1=J-T-MAX
	
	void Initialize()
	{
		for(int i=0; i<2; ++i) CrmtBase[i].Initialize();
	}
    T_CRMT_SNIP() { Initialize(); }	
};

struct T_CRMT_SNIP_UNIT
{
	T_CRMT_SNIP_UNIT()
	{
        dPu    = D_UNITSYS_BASE_FORCE;		
		dQu    = D_UNITSYS_BASE_FORCE;		
		dMu	   = D_UNITSYS_BASE_MOMENT;		
		dTu    = D_UNITSYS_BASE_MOMENT;		
        dTr    = D_UNITSYS_BASE_MOMENT;
	}

    int dPu;
	int	dQu;		
	int dMu;		
	int dTu;		
    int dTr;

};

struct T_FATI_SNIP_BASE
{
	BOOL bCHK;				   // [Unit=None]
	UINT LcomK;				   // [Unit=None]
    int  nMax;
    int  nKind;          // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int  nLSTerm;        // Long, Short Term 구별
    double dPu;          //[Unit=Force],  
	double dQu;			     //[Unit=Force],  
	double dMuy;		     //[Unit=Moment], 
    double dMuz;		     //[Unit=Moment], 
    double dTu;			     //[Unit=Moment], 
    double dPu_tp;
    double dMuy_tp;
    double dMuz_tp;
    double dPu_ts;
    double dMuy_ts;
    double dMuz_ts;
    double dPu_per;
    double dMuy_per;
    double dMuz_per;  
    double dSigma_pmax;  // [Unit=Stress]
    double dSigma_pmin;  // [Unit=Stress]
    double dm_ap1_Rp;    // [Unit=Stress]
    double dSigma_bcmax; // [Unit=Stress]
    double dSigma_bcmin; // [Unit=Stress]
    double dm_b1_Rb;     //  3.94, Table 38, [Unit=Stress]
    double dm_b1_Rp;     // [Unit=Stress]
    double dfbc;         // fbc = M/Ireq*x', [Unit=Stress]
    double dffs;         // ffs = n'*M/Ired*(h-x'-a_u), [Unit=Stress]
    double dm_as1_Rs;    // dffsr = mas1*Rs, [Unit=Stress]
    double dfacs;        // dfacs = N/Ared, [Unit=Stress]
    double dRatF;        // [Unit=None]

    BOOL bOK;

    void Initialize()
    {
		bCHK = 0;
		LcomK = 0;
		nMax=nKind=nLSTerm=0;
        dPu=dQu=dMuy=dMuz=dTu=0.0;
        dPu_tp=dMuy_tp=dMuz_tp=0.0;
        dPu_ts=dMuy_ts=dMuz_ts=0.0;
        dPu_per=dMuy_per=dMuz_per=0.0;
        dSigma_pmax = dSigma_pmin = dm_ap1_Rp = 0.0;
        dSigma_bcmax = dSigma_bcmin = dm_b1_Rb = dm_b1_Rp = 0.0;
        dfbc = dffs = dm_as1_Rs = 0.0;
        dfacs = 0.0;
        dRatF = 0.0;
        bOK = TRUE;
    }

    T_FATI_SNIP_BASE() { Initialize(); }
};

struct T_FATI_SNIP	// ElemK.
{
	T_FATI_SNIP_BASE FatiBase[2];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	
	void Initialize()
	{
		for(int i=0; i<2; ++i) FatiBase[i].Initialize();
	}
    T_FATI_SNIP() { Initialize(); }	
};

struct T_FATI_SNIP_UNIT
{
	T_FATI_SNIP_UNIT()
	{
        dPu           = D_UNITSYS_BASE_FORCE;
        dQu           = D_UNITSYS_BASE_FORCE;
        dMuy          = D_UNITSYS_BASE_MOMENT;
        dMuz          = D_UNITSYS_BASE_MOMENT;
        dTu           = D_UNITSYS_BASE_MOMENT;
        dPu_tp        = D_UNITSYS_BASE_FORCE;
        dMuy_tp       = D_UNITSYS_BASE_MOMENT;
        dMuz_tp       = D_UNITSYS_BASE_MOMENT;
        dPu_ts        = D_UNITSYS_BASE_FORCE;
        dMuy_ts       = D_UNITSYS_BASE_MOMENT;
        dMuz_ts       = D_UNITSYS_BASE_MOMENT;
        dPu_per       = D_UNITSYS_BASE_FORCE;
        dMuy_per      = D_UNITSYS_BASE_MOMENT;
        dMuz_per      = D_UNITSYS_BASE_MOMENT;
        dSigma_pmax   = D_UNITSYS_BASE_STRESS; 
        dSigma_pmin   = D_UNITSYS_BASE_STRESS;
        dm_ap1_Rp     = D_UNITSYS_BASE_STRESS;    
        dSigma_bcmax  = D_UNITSYS_BASE_STRESS; 
        dSigma_bcmin  = D_UNITSYS_BASE_STRESS; 
        dm_b1_Rb      = D_UNITSYS_BASE_STRESS;     
        dm_b1_Rp      = D_UNITSYS_BASE_STRESS;     
        dfbc          = D_UNITSYS_BASE_STRESS;         
        dffs          = D_UNITSYS_BASE_STRESS;         
        dm_as1_Rs     = D_UNITSYS_BASE_STRESS;    
        dfacs         = D_UNITSYS_BASE_STRESS;        
    }

    int dPu;  
	int dQu;	 
	int dMuy;	
    int dMuz;	
    int dTu;	 
    int dPu_tp;
    int dMuy_tp;
    int dMuz_tp;
    int dPu_ts;
    int dMuy_ts;
    int dMuz_ts;
    int dPu_per;
    int dMuy_per;
    int dMuz_per;  
    int dSigma_pmax;  
    int dSigma_pmin;  
    int dm_ap1_Rp;    
    int dSigma_bcmax; 
    int dSigma_bcmin; 
    int dm_b1_Rb;     
    int dm_b1_Rp;     
    int dfbc;         
    int dffs;         
    int dm_as1_Rs;    
    int dfacs;        

};


struct T_PGLS_IRC_D
{
    // 0=I-Max, 1=J-Max
    T_PGLS_BASE BeforePgls[2];
    T_PGLS_BASE PglsBase[2];

    void Initialize()
    {
        for(int i=0; i<2; i++)
        {
            BeforePgls[i].Initialize();
            PglsBase[i].Initialize();
        }
    }
    T_PGLS_IRC_D() { Initialize(); }
};

struct T_PGLS_IRC_UNIT
{
    T_PGLS_IRC_UNIT()
    {
        dFx = D_UNITSYS_BASE_FORCE;           
        dFz = D_UNITSYS_BASE_FORCE;           
        dMy = D_UNITSYS_BASE_MOMENT;           
        dMz = D_UNITSYS_BASE_MOMENT;           
        dNc = D_UNITSYS_BASE_FORCE;           
        dNc_el = D_UNITSYS_BASE_FORCE;        
        dNc_f = D_UNITSYS_BASE_FORCE;         
        dMa_Ed = D_UNITSYS_BASE_MOMENT;        
        dMc_Ed = D_UNITSYS_BASE_MOMENT;        
        dMpl_Rd = D_UNITSYS_BASE_MOMENT;       
        dMel_Rd = D_UNITSYS_BASE_MOMENT;       
        dV_L_Ed = D_UNITSYS_BASE_FORCE;       
        dv_L_Ed = D_UNITSYS_BASE_UNITFORCE;       
        dv_L_Ed2= D_UNITSYS_BASE_UNITFORCE;       
        dP_Rd1 = D_UNITSYS_BASE_FORCE;
        dP_Rd2 = D_UNITSYS_BASE_FORCE;
        dP_Rd = D_UNITSYS_BASE_FORCE;         
        dv_L_Rd = D_UNITSYS_BASE_UNITFORCE;       
        dv_Ed = D_UNITSYS_BASE_STRESS;
        dv_Ed_lim = D_UNITSYS_BASE_STRESS;
        dAzI = D_UNITSYS_BASE_LENGTH; 

        dVedDL  = D_UNITSYS_BASE_FORCE;
        dVedLL  = D_UNITSYS_BASE_FORCE;
        dSl1req = D_UNITSYS_BASE_LENGTH;
        dSl2req = D_UNITSYS_BASE_LENGTH;
        dSlprov = D_UNITSYS_BASE_LENGTH;
    }  
    int dFx; 
    int dFz;
    int dMy;           
    int dMz;           
    int dNc;           
    int dNc_el;        
    int dNc_f;         
    int dMa_Ed;        
    int dMc_Ed;        
    int dMpl_Rd;       
    int dMel_Rd;       
    int dV_L_Ed;       
    int dv_L_Ed;
    int dv_L_Ed2;
    int dP_Rd1;
    int dP_Rd2;
    int dP_Rd;         
    int dv_L_Rd;       
    int dv_Ed;   
    int dv_Ed_lim;
    int dAzI;

    int dVedDL;
    int dVedLL;
    int dSl1req;
    int dSl2req;
    int dSlprov;
};

//////////////////////////////////////////////////////////////////////////
// AS 5100.5:2017 - 호주기준

struct T_FYSG_PSC_AS_BASE
{
    BOOL bCHK;		//설계결과유무
    UINT StageK;	//스테이지번호		[Unit=None]
    UINT StepK;   // Step Key.      [Unit=None]
    double dFT;   //상단응력도
    double dFB;   //하단응력도
    double dFTL;	//상단좌단 합성응력도	[Unit=Stress]
    double dFBL;	//하단좌단 합성응력도	[Unit=Stress]
    double dFTR;	//상단우단 합성응력도	[Unit=Stress]
    double dFBR;	//하단우단 합성응력도	[Unit=Stress]
    double dFMAX;	//최대/최소 합성응력도	[Unit=Stress]
    double dALW;	//허용응력도			[Unit=Stress]
    double dfci;
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        StageK = StepK=0;
        dFT = dFB = 0.0;
        dFTL = dFBL = dFTR = dFBR = dFMAX = dALW = 0.0;
        dfci = 0.0;
        bOK = TRUE;
    }	
    T_FYSG_PSC_AS_BASE() { Initialize(); }
};
struct T_FYSG_PSC_AS	// ElemK.
{
    BOOL bComposite;
    T_FYSG_PSC_AS_BASE FysgGrB[4];	// Girder, 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    T_FYSG_PSC_AS_BASE FysgSlB[4];  // Slab

    void Initialize()
    {
        bComposite = FALSE;
        for(int i=0; i<4; ++i) 
        {
            FysgGrB[i].Initialize();
            FysgSlB[i].Initialize();
        }
    }
    T_FYSG_PSC_AS() { Initialize(); }
};

struct T_FYSG_PSC_AS_UNIT
{
    T_FYSG_PSC_AS_UNIT()
    {
        dFT		= D_UNITSYS_BASE_STRESS;
        dFB		= D_UNITSYS_BASE_STRESS;
        dFTL	= D_UNITSYS_BASE_STRESS;
        dFBL	= D_UNITSYS_BASE_STRESS;
        dFTR	= D_UNITSYS_BASE_STRESS;
        dFBR	= D_UNITSYS_BASE_STRESS;
        dFMAX	= D_UNITSYS_BASE_STRESS;
        dALW	= D_UNITSYS_BASE_STRESS;	
    }
    int dFT;
    int dFB;
    int dFTL;
    int dFBL;
    int dFTR;
    int dFBR;
    int dFMAX;
    int dALW;	
};

// Check Compressive Stress at Transfer
struct T_TRAN_PSC_AS_BASE
{
    BOOL bCHK;		//설계결과유무
    UINT StageK;	//스테이지번호		[Unit=None]
    UINT StepK;   // Step Key.      [Unit=None]
    UINT LcomK;

    T_PSCD_FORCE ForD;
    double dft; 
    double dfb;
    double dStress[4]; // Stress point 0=TL, 1=TR, 2=BR, 3=BL...
    double dftc;       // Top 0,1중 압축
    double dfbc;       // Bottom, 2,3중 압축
    double dfcp;
    double dfmax;
    double dAlwStr;    // 0.6fcp

    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        StageK = 0;
        StepK  = 0;
        LcomK  = 0;
        dft = 0.0;
        dfb = 0.0;
        for ( int i=0; i<4; ++i ) { dStress[i] = 0.0; }
        dftc = 0.0;
        dfbc = 0.0;
        dfcp = 0.0;
        dAlwStr = 0.0;
        dfmax = 0.0;
        bOK = FALSE;
    }
    T_TRAN_PSC_AS_BASE() { Initialize(); }
};

struct T_TRAN_PSC_AS
{
    BOOL bComposite;
    T_TRAN_PSC_AS_BASE GrB[4];	// Girder, 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        bComposite = FALSE;
        for ( int i=0; i<4; ++i )
        {
            GrB[i].Initialize();
        }
    }
    T_TRAN_PSC_AS() { Initialize(); }
};

struct T_TRAN_PSC_AS_UNIT
{
    T_TRAN_PSC_AS_UNIT()
    {
        dft     = D_UNITSYS_BASE_STRESS;
        dfb     = D_UNITSYS_BASE_STRESS;
        dStress = D_UNITSYS_BASE_STRESS;
        dfcp    = D_UNITSYS_BASE_STRESS;
        dAlwStr = D_UNITSYS_BASE_STRESS;
    }
    int dft;
    int dfb;
    int dStress;
    int dfcp;
    int dAlwStr;
};

// Stresses in concrete during service load combinations 

struct T_FYCM_PSC_AS_BASE
{  
    BOOL bCHK;		
    UINT LcomK;
    int  nMax;		//MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                //FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int  nKind;   // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int  nLSTerm; // Long, Short Term 구별 
    double dFT;		//[Unit=Stress]
    double dFB;		//[Unit=Stress]
    double dFTL;	//[Unit=Stress]
    double dFBL;	//[Unit=Stress]
    double dFTR;	//[Unit=Stress]
    double dFBR;	//[Unit=Stress]
    double dFMAX;	//[Unit=Stress]
    double dALW;	//[Unit=Stress]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = nMax = nKind = nLSTerm = 0;
        dFT = dFB = 0.0;
        dFTL = dFBL = dFTR = dFBR = dFMAX = dALW = 0;
        bOK = TRUE;
    }
    T_FYCM_PSC_AS_BASE() { Initialize(); }

    double Get_Max()
    {
        double dTopMax = max(dFT, max(dFTL, dFTR));
        double dBotMax = max(dFB, max(dFBL, dFBR));
        return max(dTopMax, dBotMax);
    }
    double Get_Min()
    {
        double dTopMin = min(dFT, min(dFTL, dFTR));
        double dBotMin = min(dFB, min(dFBL, dFBR));
        return min(dTopMin, dBotMin);
    }
};

struct T_FYCM_PSC_AS	// ElemK.
{
    BOOL bComposite;
    T_FYCM_PSC_AS_BASE FycmGrB[4];	// Girder, 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    T_FYCM_PSC_AS_BASE FycmSlB[4];  // Slab

    void Initialize()
    {
        bComposite = FALSE;
        for(int i=0; i<4; ++i)
        {
            FycmGrB[i].Initialize();
            FycmSlB[i].Initialize();
        }
    }
    T_FYCM_PSC_AS() { Initialize(); }	
};

struct T_FYCM_PSC_AS_UNIT
{
    T_FYCM_PSC_AS_UNIT()
    {
        dFT		= D_UNITSYS_BASE_STRESS;		
        dFB		= D_UNITSYS_BASE_STRESS;		
        dFTL	= D_UNITSYS_BASE_STRESS;
        dFBL	= D_UNITSYS_BASE_STRESS;
        dFTR	= D_UNITSYS_BASE_STRESS;	
        dFBR	= D_UNITSYS_BASE_STRESS;
        dFMAX	= D_UNITSYS_BASE_STRESS;
        dALW	= D_UNITSYS_BASE_STRESS;	
    }

    int dFT;		
    int dFB;		
    int dFTL;	
    int dFBL;	
    int dFTR;	
    int dFBR;	
    int dFMAX;	
    int dALW;	
};


// Tensile stresses in prestressing tendons
struct T_FYPC_PSC_AS	// TdnaK.
{
    BOOL bCHK;	
    int nLoadType;  // 0=Pre-tension, 1=Post-tension 

    double dFDL1;	  // Pre-Tensioning: Stress prior to transfer. Jacking stress. [Unit=Stress]
                                    // Post-Tensioning: Stress prior to seating. Jacking stress. [Unit=Stress]
                                    
    double dFDL2;	  // Pre-Tensioning: Not used. [Unit=Stress]		
                                    // Post-Tensioning: Stress at anchorage and couplers immediately after anchor set. [Unit=Stress]
                                    
    double dFDL3;	  // Pre-Tensioning: Not used [Unit=Stress]
                                    // Post-Tensioning: Away from Anchorage and couplers immediately after anchor set. [Unit=stress]

    double dFLL1;	  // Stress at service limit after all losses. [Unit=Stress]

    double dFLL2;	  // Stress at strength or extreme event limit state. [Unit=Stress]

    double dAFDL1;	// Pre-Tensioning: Allowable stress prior to transfer. Allowable jacking stress. [Unit=Stress]
                                    // Post-Tensioning: Allowable stress prior to seating. Allowable jacking stress. [Unit=Stress]

    double dAFDL2;	// Pre-Tensioning: Not used. [Unit=Stress]		
                                    // Post-Tensioning: Allowable stress at anchorage and couplers immediately after anchor set. [Unit=Stress]

    double dAFDL3;	// Pre-Tensioning: Not used. [Unit=Stress]		
                                    // Post-Tensioning: Allowable stress at anchorage and couplers immediately after anchor set. [Unit=Stress]

    double dAFLL1; 	// Stress at service limit after all losses. [Unit=Stress]

    double dAFLL2;  // Stress at strength or extreme event limit state. [Unit=Stress]

    BOOL bOK;

    void Initialize()
    {
        bCHK = FALSE;
        nLoadType = 0; 
        dFDL1 = dFDL2 = dFDL3 = dFLL1 = dFLL2 = 0.0;
        dAFDL1 = dAFDL2 = dAFDL3 = dAFLL1 = dAFLL2 = 0.0;
        bOK = TRUE;
    }
    T_FYPC_PSC_AS() { Initialize(); }
};

struct T_FYPC_PSC_AS_UNIT
{
    T_FYPC_PSC_AS_UNIT()
    {
        dFDL1	  = D_UNITSYS_BASE_STRESS;
        dFDL2	  = D_UNITSYS_BASE_STRESS;
        dFDL3	  = D_UNITSYS_BASE_STRESS;
        dFLL1	  = D_UNITSYS_BASE_STRESS;
        dFLL2	  = D_UNITSYS_BASE_STRESS;
        dAFDL1	= D_UNITSYS_BASE_STRESS;
        dAFDL2	= D_UNITSYS_BASE_STRESS;
        dAFDL3	= D_UNITSYS_BASE_STRESS;
        dAFLL1	= D_UNITSYS_BASE_STRESS;
        dAFLL2	= D_UNITSYS_BASE_STRESS;
    }

    int dFDL1;
    int dFDL2;
    int dFDL3;
    int dFLL1;
    int dFLL2;
    int dAFDL1;
    int dAFDL2;
    int dAFDL3;
    int dAFLL1;
    int dAFLL2;
};

// FPSG 시공단계 주응력, 기존꺼 사용

// FPMS 완성계 주응력, 기존꺼 사용

// Crack
struct T_BDCW_PSC_AS_BASE_895
{
    BOOL bCHK;		//[Unit=None]
    UINT LcomK;		//[Unit=None] 
    int  nMax;    // 
    int  nKind;   // 

    double dMu;   // 
    double dftop; // 위연단 응력 LCB
    double dfbot; // 아래연단 응력
    double dft;   // 위연단 응력  ,DL  [Unit=Stress]
    double dfb;   // 아래연단 응력,DL   [Unit=Stress]
    double dft_LL;
    double dfb_LL;
    double dfs;
    double dfsa;
    double dfcp;  // 최외곽 텐던위치 콘크리트 응력        [Unit=Stress]
    double ds;    // 철근 간격       [Unit=Length]
    double dsa;   // 철근 간격 제안  [Unit=Length]
    BOOL   bExistTndn;
    double dfta;       // 0.25*sqrt(fc')
    double dfta2;      // 0.5*sqrt(fc')
    BOOL   bChk_fta;   // ft<0.25*sqrt(fc')
    BOOL   bChk_8_6_2_3;
    BOOL   bOK;

    T_COMF_BASE ForBe; // 거더
    T_COMF_BASE ForST; // 완성계 단기,
    T_COMF_BASE ForLT; // 완성계 장기,

    void Initialize()
    {
        bCHK  = FALSE;
        LcomK = 0;
        nMax  = 0;
        nKind = 0;
        dMu   = 0.0;
        dftop = 0.0;
        dfbot = 0.0;
        dft   = 0.0;
        dfb   = 0.0;
        dft_LL= 0.0;
        dfb_LL= 0.0;
        dfs   = 0.0;
        dfsa  = 0.0;
        dfcp  = 0.0;
        ds    = 0.0;
        dsa   = 0.0;
        bExistTndn = FALSE;
        dfta  = 0.0;
        dfta2 = 0.0;
        bChk_fta = FALSE;
        bChk_8_6_2_3 = FALSE;
        bOK   = FALSE;
        ForBe.Initialize();
        ForST.Initialize();
        ForLT.Initialize();
    }

    T_BDCW_PSC_AS_BASE_895() { Initialize(); }
};

struct T_BDCW_PSC_AS_BASE
{
    BOOL bCHK;		//[Unit=None]
    UINT LcomK;		//[Unit=None] 
    int  nMax;    // 
    int  nKind;   // 

    double dMu;   // 
    double dftop; // 위연단 응력 LCB
    double dfbot; // 아래연단 응력
    double dft;   // 위연단 응력  ,DL  [Unit=Stress]
    double dfb;   // 아래연단 응력,DL   [Unit=Stress]
    double dft_LL;
    double dfb_LL;
    double dftop_sl;// Slab 위연단 응력
    double dfbot_sl;// Slab 아래연단 응력
    double dft_DL_sl;
    double dfb_DL_sl;
    double dft_LL_sl;
    double dfb_LL_sl;
    double dfs;
    double dfsa;
    double dfcp;  // 최외곽 텐던위치 콘크리트 응력        [Unit=Stress]
    double dfcp_sl;      // Sigma_scr - SLS
    double dfcp_DL_sl;   // Sigma_scr - PL
    double dfs_sl;       // f_scr - SLS
    double dfs_DL_sl;    // f_scr - PL
    BOOL   bIsChkSlabPL;
    double ds;    // 철근 간격       [Unit=Length]
    double dsa;   // 철근 간격 제안  [Unit=Length]
    BOOL   bExistTndn;
    double dfta;       // 0.25*sqrt(fc')
    double dfta2;      // 0.5*sqrt(fc')
    BOOL   bChk_fta;   // ft<0.25*sqrt(fc')
    BOOL   bChk_8_6_2_3;
    BOOL   bOK;

    T_COMF_BASE ForBe; // 거더
    T_COMF_BASE ForST; // 완성계 단기,
    T_COMF_BASE ForLT; // 완성계 장기,
    
    void Initialize()
    {
        bCHK  = FALSE;
        LcomK = 0;
        nMax  = 0;
        nKind = 0;
        dMu   = 0.0;
        dftop = 0.0;
        dfbot = 0.0;
        dft   = 0.0;
        dfb   = 0.0;
        dft_LL= 0.0;
        dfb_LL= 0.0;
        dftop_sl  = 0.0;
        dfbot_sl  = 0.0;
        dft_DL_sl = 0.0;
        dfb_DL_sl = 0.0;
        dft_LL_sl = 0.0;
        dfb_LL_sl = 0.0;
        dfs   = 0.0;
        dfsa  = 0.0;
        dfcp  = 0.0;
        dfcp_sl = 0.0;
        dfcp_DL_sl = 0.0;
        dfs_sl = 0.0;
        dfs_DL_sl = 0.0;
        ds    = 0.0;
        dsa   = 0.0;
        bExistTndn = FALSE;
        dfta  = 0.0;
        dfta2 = 0.0;
        bChk_fta = FALSE;
        bChk_8_6_2_3 = FALSE;
        bOK   = FALSE;
        ForBe.Initialize();
        ForST.Initialize();
        ForLT.Initialize();
    }

    T_BDCW_PSC_AS_BASE() { Initialize(); }

    void Convert895(const T_BDCW_PSC_AS_BASE_895& rData)
    {
        bCHK  = rData.bCHK;
        LcomK = rData.LcomK;
        nMax  = rData.nMax;
        nKind = rData.nKind;
        dMu   = rData.dMu;
        dftop = rData.dftop;
        dfbot = rData.dfbot;
        dft   = rData.dft;
        dfb   = rData.dfb;
        dft_LL= rData.dft_LL;
        dfb_LL= rData.dfb_LL;
        dfs   = rData.dfs;
        dfsa  = rData.dfsa;
        dfcp  = rData.dfcp;
        ds    = rData.ds;
        dsa   = rData.dsa;
        bExistTndn = rData.bExistTndn;
        dfta  = rData.dfta;
        dfta2 = rData.dfta2;
        bChk_fta = rData.bChk_fta;
        bChk_8_6_2_3 = rData.bChk_8_6_2_3;
        bOK   = rData.bOK;
        ForBe = rData.ForBe;
        ForST = rData.ForST;
        ForLT = rData.ForLT;
    }
};

struct T_BDCW_PSC_AS_895
{
    BOOL bComposite;
    T_BDCW_PSC_AS_BASE_895 BdcwB[4];

    void Initialize()
    {
        bComposite = FALSE;
        for ( int i=0; i<4; i++ ) BdcwB[i].Initialize();
    }
    T_BDCW_PSC_AS_895() { Initialize(); }
};

struct T_BDCW_PSC_AS
{
    BOOL bComposite;
    T_BDCW_PSC_AS_BASE BdcwB[4];

    void Initialize()
    {
        bComposite = FALSE;
        for(int i=0; i<4; i++) BdcwB[i].Initialize();
    }
    T_BDCW_PSC_AS() { Initialize(); }

    void Convert895(const T_BDCW_PSC_AS_895& rData)
    {
        bComposite = rData.bComposite;
        for ( int i=0; i<4; i++ ) BdcwB[i].Convert895(rData.BdcwB[i]);
    }
};

struct T_BDCW_PSC_AS_UNIT
{
    T_BDCW_PSC_AS_UNIT()
    {
        dMu    = D_UNITSYS_BASE_MOMENT;
        dftop  = D_UNITSYS_BASE_STRESS;
        dfbot  = D_UNITSYS_BASE_STRESS;
        dft    = D_UNITSYS_BASE_STRESS;
        dfb    = D_UNITSYS_BASE_STRESS;
        dft_LL = D_UNITSYS_BASE_STRESS;
        dfb_LL = D_UNITSYS_BASE_STRESS;
        dfs    = D_UNITSYS_BASE_STRESS;
        dfsa   = D_UNITSYS_BASE_STRESS;
        dfcp   = D_UNITSYS_BASE_STRESS;
        ds     = D_UNITSYS_BASE_LENGTH;
        dsa    = D_UNITSYS_BASE_LENGTH;
        dfta   = D_UNITSYS_BASE_STRESS;
        dfta2  = D_UNITSYS_BASE_STRESS;
    }
    int dMu;
    int dftop;
    int dfbot;
    int dft;
    int dfb;
    int dft_LL;
    int dfb_LL;
    int dfs;
    int dfsa;
    int dfcp;
    int ds;
    int dsa;
    int dfta;
    int dfta2;
};


struct T_BDCR_PSC_AS_BASE
{
    BOOL bCHK;
    UINT LcomK;
    int  nMax;
    int  nKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.

    double dFu;     // Fx [Unit=Force]
    double dMu;     // M* [Unit=Moment]
    double dpMn;    // phi*Mu    [Unit=Moment]
    double dRatM;   // M*/phi*Mu
    double dMn_min; // (Muo)min  [Unit=Moment]
    double dAst;
    double dAst_min;
    BOOL   bOK;


    void Initialize()
    {
        bCHK    = FALSE;
        LcomK   = 0;
        nMax    = 0;
        nKind   = 0;
        dFu     = 0.0;
        dMu     = 0.0;
        dpMn    = 0.0;
        dRatM   = 0.0;
        dMn_min = 0.0;
        dAst    = 0.0;
        dAst_min= 0.0;
        bOK     = FALSE;
    }

    T_BDCR_PSC_AS_BASE () {Initialize();}
};

struct T_BDCR_PSC_AS
{
    BOOL bComposite;
    T_BDCR_PSC_AS_BASE BdcrB[4]; // 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN

    void Initialize()
    {
        bComposite = FALSE;
        for(int i=0; i<4; i++) BdcrB[i].Initialize();
    }
    T_BDCR_PSC_AS() { Initialize(); }
};

struct T_BDCR_PSC_AS_UNIT
{
    T_BDCR_PSC_AS_UNIT()
    {
        dFu     = D_UNITSYS_BASE_FORCE;
        dMu     = D_UNITSYS_BASE_MOMENT;
        dpMn    = D_UNITSYS_BASE_MOMENT;
        dMn_min = D_UNITSYS_BASE_MOMENT;
        dAst    = D_UNITSYS_BASE_AREA;
        dAst_min= D_UNITSYS_BASE_AREA;
    }
    int dFu;
    int dMu;
    int dpMn;
    int dMn_min;
    int dAst;
    int dAst_min;
};

struct T_SHRR_PSC_AS_BASE // Shear resistance
{
    BOOL bCHK;
    UINT LcomK;
    int  nMax;
    int  nKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    double dForceTP[6];
    double dFu;       // Fx, 부재력 [Unit=Force]
    double dMu;       // My, 부재력 [Unit=Moment]
    double dVu;       // Fz, 부재력 [Unit=Force]
    double dTu;       // Mx, 부재력 [Unit=Moment]
    double dVeq;      // 8.2.1.2(4), (5) [Unit=Force]
    double dpVn;      // Phi*Vu   [Unit=Force]
    double dVnc;      // Vuc      [Unit=Force]
    double dVns;      // Vus      [Unit=Force]
    double dRatV;
    double dPv;       // Pv, Tendon 분력 Pz     [Unit=Force]
    double dVu_max;   // Vu.max  [Unit=Force]
    double dVu_min;   // Vu.min  [Unit=Force]
    double dAsv;      // Asv     [Unit=Area]
    double dAsv_min;  // Asv.min [Unit=Area]
    BOOL   bOK;

    void Initialize()
    {
        bCHK     = FALSE;
        LcomK    = 0;
        nMax     = 0;
        nKind    = 0;
        memset(dForceTP, 0, sizeof(dForceTP));
        dFu      = 0.0;
        dMu      = 0.0;
        dVu      = 0.0;
        dTu      = 0.0;
        dVeq     = 0.0;
        dpVn     = 0.0;
        dVnc     = 0.0;
        dVns     = 0.0;
        dRatV    = 0.0;
        dPv      = 0.0;
        dVu_max  = 0.0;
        dVu_min  = 0.0;
        dAsv     = 0.0;
        dAsv_min = 0.0;
        bOK     = FALSE;
    }

    T_SHRR_PSC_AS_BASE () {Initialize();}
};

struct T_SHRR_PSC_AS
{
    BOOL bComposite;
    T_SHRR_PSC_AS_BASE ShrrB[4]; // 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN

    void Initialize()
    {
        bComposite = FALSE;
        for(int i=0; i<4; i++) ShrrB[i].Initialize(); 
    }
    T_SHRR_PSC_AS() { Initialize(); }
};

struct T_SHRR_PSC_AS_UNIT
{
    T_SHRR_PSC_AS_UNIT()
    {
        dFu      = D_UNITSYS_BASE_FORCE;
        dMu      = D_UNITSYS_BASE_MOMENT;
        dVu      = D_UNITSYS_BASE_FORCE;
        dTu      = D_UNITSYS_BASE_MOMENT;
        dVeq     = D_UNITSYS_BASE_FORCE;
        dpVn     = D_UNITSYS_BASE_FORCE;
        dVnc     = D_UNITSYS_BASE_FORCE;
        dVns     = D_UNITSYS_BASE_FORCE;
        dPv      = D_UNITSYS_BASE_FORCE;
        dVu_max  = D_UNITSYS_BASE_FORCE;
        dVu_min  = D_UNITSYS_BASE_FORCE;
        dAsv     = D_UNITSYS_BASE_AREA;
        dAsv_min = D_UNITSYS_BASE_AREA;
    }
    int dFu;
    int dMu;
    int dVu;
    int dTu;
    int dVeq;
    int dpVn;
    int dVnc;
    int dVns;
    int dPv;
    int dVu_max;
    int dVu_min;
    int dAsv;
    int dAsv_min;

};


struct T_TORR_PSC_AS_BASE // Torsional resistance
{
    BOOL bCHK;
    UINT LcomK;
    int  nMax;
    int  nKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    double dForceTP[6];
    double dFu;       // Fx, 부재력 [Unit=Force]
    double dMu;       // My, 부재력 [Unit=Moment]
    double dVu;       // Fz, 부재력 [Unit=Force]
    double dTu;       // Mx, 부재력 [Unit=Moment]
    double dVeq;      //
    double dpVn;      // Phi*Vu   [Unit=Force]
    double dpTn;      // Phi*Tu   [Unit=Moment]
    double d025pTcr;  // 0.25*phi*Tcr [Unit=Moment]
    double dTcr;      // Tcr  [Unit=Moment]
    double dRatT;     // Tu/pTn
    double dRatVT;    // Vu/pVn + Tu/pTn
    double dTus;      // Tus  [Unit=Moment]
    BOOL   bOK;

    void Initialize()
    {
        bCHK     = FALSE;
        LcomK    = 0;
        nMax     = 0;
        nKind    = 0;
        memset(dForceTP, 0, sizeof(dForceTP));
        dFu      = 0.0;
        dMu      = 0.0;
        dVu      = 0.0;
        dTu      = 0.0;
        dVeq     = 0.0;
        dpVn     = 0.0;
        dpTn     = 0.0;
        d025pTcr = 0.0;
        dTcr     = 0.0;
        dRatT    = 0.0;
        dRatVT   = 0.0;
        dTus     = 0.0;
        bOK      = FALSE;
    }

    T_TORR_PSC_AS_BASE () {Initialize();}
};

struct T_TORR_PSC_AS
{
    BOOL bComposite;
    T_TORR_PSC_AS_BASE TorrB[6]; // 0=I-V-MAX, 1=I-V-MIN, 2=I-T-MAX, 3=J-V-MAX, 4=J-V-MIN, 5=J-T-MAX.

    void Initialize()
    {
        bComposite = FALSE;
        for(int i=0; i<6; i++) TorrB[i].Initialize();
    }
    T_TORR_PSC_AS() { Initialize(); }
};

struct T_TORR_PSC_AS_UNIT
{
    T_TORR_PSC_AS_UNIT()
    {
        dFu      = D_UNITSYS_BASE_FORCE;
        dMu      = D_UNITSYS_BASE_MOMENT;
        dVu      = D_UNITSYS_BASE_FORCE;
        dTu      = D_UNITSYS_BASE_MOMENT;
        dVeq     = D_UNITSYS_BASE_FORCE;
        dpVn     = D_UNITSYS_BASE_FORCE;
        dpTn     = D_UNITSYS_BASE_MOMENT;
        d025pTcr = D_UNITSYS_BASE_MOMENT;
        dTcr     = D_UNITSYS_BASE_MOMENT;
        dTus     = D_UNITSYS_BASE_MOMENT;
    }
    int dFu;
    int dMu;
    int dVu;
    int dTu;
    int dVeq;
    int dpVn;
    int dpTn;
    int d025pTcr;
    int dTcr;
    int dTus;
};


// BS 5400:4, BD44/15
// Crack
struct T_BDCW_PSC_BS_BASE
{
    BOOL bCHK;		//[Unit=None]

    UINT LcomK;		//[Unit=None] 
    int  iMax;

    double dPu;        //[Unit=Force]
    double dMu;        //[Unit=Moment]
    double dMg;        //Nmm Moment due to permenent loads
    double dMq;        //Nmm Moment due to live loads
    double dft;		   //Top stress [Unit=Stress]
    double dfb;		   //Bottom stress [Unit=Stress]
    double dEpsilon_s; // 철근 변형률,    [Unit=None]
    double dEpsilon_1; // 콘크리트 변형률 [Unit=None]
    double dEpsilon_m; //Strain Calculate
    double dcnom;      //mm Nominal required cover
    double dc_neu;     //mm neutral axis
    double dh;         //mm section height
    double dbt;        //mm Width of section at centroid of tension steel
    double dap;        //mm distance from compresison face to point of crack
    double dAst;       //mm2 Area of in tension R/F
    double dCW;        //[Unit=Length]
    double dDgnCW;     //[Unit=Length]
    double dRatio;     //[Unit=None]

    BOOL   bCHK_ConMatl;
    int    iConcGrade;

    BOOL bOK;

    void Initialize()
    {
        bCHK  = FALSE;
        LcomK = 0;
        iMax  = 0;
        dPu   = 0.0;
        dMu   = 0.0;
        dMg   = 0.0;
        dMq   = 0.0;
        dfb   = 0.0;
        dft   = 0.0;
        dEpsilon_s = 0.0;
        dEpsilon_1 = 0.0;
        dEpsilon_m = 0.0;
        dcnom = 0.0;
        dc_neu = 0.0;
        dbt   = 0.0;
        dh    = 0.0;
        dbt   = 0.0;
        dap   = 0.0;
        dAst  = 0.0;
        dCW   = 0.0;
        dDgnCW= 0.0;
        bCHK_ConMatl = TRUE;
        iConcGrade   = 0;
        bOK   = FALSE;
    }

    T_BDCW_PSC_BS_BASE() { Initialize(); }
};

struct T_BDCW_PSC_BS
{
    BOOL bComposite;
    T_BDCW_PSC_BS_BASE BdcwB[4];

    void Initialize()
    {
        bComposite = FALSE;
        for(int i=0; i<4; i++) BdcwB[i].Initialize();
    }
    T_BDCW_PSC_BS() { Initialize(); }
};

struct T_BDCW_PSC_BS_UNIT
{
    T_BDCW_PSC_BS_UNIT()
    {
        dPu    = D_UNITSYS_BASE_FORCE;
        dMu    = D_UNITSYS_BASE_MOMENT;
        dMg    = D_UNITSYS_BASE_MOMENT;
        dMq    = D_UNITSYS_BASE_MOMENT;
        dfb    = D_UNITSYS_BASE_STRESS;
        dft    = D_UNITSYS_BASE_STRESS;
        dcnom  = D_UNITSYS_BASE_LENGTH;
        dc_neu = D_UNITSYS_BASE_LENGTH;
        dh     = D_UNITSYS_BASE_LENGTH;
        dbt    = D_UNITSYS_BASE_LENGTH;
        dap    = D_UNITSYS_BASE_LENGTH;
        dAst   = D_UNITSYS_BASE_AREA;
        dCW    = D_UNITSYS_BASE_LENGTH;
        dDgnCW = D_UNITSYS_BASE_LENGTH;
    }
    int dPu;        //[Unit=Force]
    int dMu;        //[Unit=Moment]
    int dMg;        //Nmm Moment due to permenent loads
    int dMq;        //Nmm Moment due to live loads
    int dfb;		//Top stress for 균열단면[Unit=Stress]
    int dft;		//Bottom stress for 균열단면[Unit=Stress]
    int dcnom;      //mm Nominal required cover
    int dc_neu;     //mm neutral axis
    int dh;         //mm section height
    int dbt;        //mm Width of section at centroid of tension steel
    int dap;        //mm distance from compresison face to point of crack
    int dAst;       //mm2 Area of in tension R/F
    int dCW;        //[Unit=Length]
    int dDgnCW;     //[Unit=Length]
};


struct T_BDCR_PSC_BS_BASE // Flexural Resistance
{
    BOOL bCHK;
    UINT LcomK;
    int  nMax;
    int  nKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.

    double dFx;     // Fx [Unit=Force]
    double dMy;     // My [Unit=Moment]
    double dMu;     // ultimate resistance moment    [Unit=Moment]
    double dRatM;   // My/Mu
    double dMu_min; // (Muo)min  [Unit=Moment]
    double dAst;
    double dAst_min;
    BOOL   bOK;


    void Initialize()
    {
        bCHK    = FALSE;
        LcomK   = 0;
        nMax    = 0;
        nKind   = 0;
        dFx     = 0.0;
        dMy     = 0.0;
        dMu     = 0.0;
        dRatM   = 0.0;
        dMu_min = 0.0;
        dAst    = 0.0;
        dAst_min= 0.0;
        bOK     = FALSE;
    }

    T_BDCR_PSC_BS_BASE () {Initialize();}
};

struct T_BDCR_PSC_BS
{
    BOOL bComposite;
    T_BDCR_PSC_BS_BASE BdcrB[4]; // 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN

    void Initialize()
    {
        bComposite = FALSE;
        for(int i=0; i<4; i++) BdcrB[i].Initialize();
    }
    T_BDCR_PSC_BS() { Initialize(); }
};

struct T_BDCR_PSC_BS_UNIT
{
    T_BDCR_PSC_BS_UNIT()
    {
        dFx     = D_UNITSYS_BASE_FORCE;
        dMy     = D_UNITSYS_BASE_MOMENT;
        dMu     = D_UNITSYS_BASE_MOMENT;
        dMu_min = D_UNITSYS_BASE_MOMENT;
        dAst    = D_UNITSYS_BASE_AREA;
        dAst_min= D_UNITSYS_BASE_AREA;
    }
    int dFx;
    int dMy;
    int dMu;
    int dMu_min;
    int dAst;
    int dAst_min;
};

struct T_SHRR_PSC_BS_BASE_895 // Shear resistance
{
	BOOL bCHK;
	UINT LcomK;
	int  nMax;
	int  nKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.

	T_COMF_BASE ForBe; // 거더
	double dForceTP[6];
	double dFx;       // Fx, 부재력 [Unit=Force]
	double dMy;       // My, 부재력 [Unit=Moment]
	double dVz;       // Fz, 부재력 [Unit=Force]
	double dT;        // Mx, 부재력 [Unit=Moment]
	double dVu;       // ultimate shear capacity   [Unit=Force]
	double dVc;       // ultimate shear resistance of concrete      [Unit=Force]
	double dRatV;
	double dPv;       // Pv, Tendon 분력 Pz     [Unit=Force]
	double dVu_max;   // Vu.max  [Unit=Force]
	double dVu_min;   // Vu.min  [Unit=Force]
	double dAsv;      // Asv     [Unit=Area]
	double dAsv_min;  // Asv.min [Unit=Area]
	double dVmax;     // For BS  [Unit=Force]

	double ds;
	double dMcr;
	double dVp;
	double dAv_s;
	double dAv_s_req;
	double dAsv_req;
	BOOL   bUncrack; //True 이면 Uncrack False 이면 Crack
	BOOL   bOK;

	// Longitudinal shear
	bool  bChk_ls;
	bool  bOK_ls;
	double dV1;
	double dV1_lim1;
	double dV1_lim2;
	double drho_ls;
	double drho_ls_lim;
	double ds_ls;
	double ds_ls_lim1;
	double ds_ls_lim2;

	void Initialize()
	{
		bCHK = FALSE;
		LcomK = 0;
		nMax = 0;
		nKind = 0;
		ForBe.Initialize();
		memset(dForceTP, 0, sizeof(dForceTP));
		dFx = 0.0;
		dMy = 0.0;
		dVz = 0.0;
		dT = 0.0;
		dVu = 0.0;
		dVc = 0.0;
		dRatV = 0.0;
		dPv = 0.0;
		dVu_max = 0.0;
		dVu_min = 0.0;
		dAsv = 0.0;
		dAsv_min = 0.0;
		dVmax = 0.0;
		ds = 0.0;
		dMcr = 0.0;
		dVp = 0.0;
		dAv_s = 0.0;
		dAv_s_req = 0.0;
		dAsv_req = 0.0;
		bUncrack = FALSE;
		bOK = FALSE;

		bChk_ls = FALSE;
		bOK_ls = FALSE;
		dV1 = 0.0;
		dV1_lim1 = 0.0;
		dV1_lim2 = 0.0;
		drho_ls = 0.0;
		drho_ls_lim = 0.0;
		ds_ls = 0.0;
		ds_ls_lim1 = 0.0;
		ds_ls_lim2 = 0.0;
	}

	T_SHRR_PSC_BS_BASE_895 () { Initialize(); }
};


struct T_SHRR_PSC_BS_BASE // Shear resistance
{
    BOOL bCHK;
    UINT LcomK;
    int  nMax;
    int  nKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.

    T_COMF_BASE ForBe; // 거더
    double dForceTP[6];
    double dFx;       // Fx, 부재력 [Unit=Force]
    double dMy;       // My, 부재력 [Unit=Moment]
    double dVz;       // Fz, 부재력 [Unit=Force]
    double dT;        // Mx, 부재력 [Unit=Moment]
    double dVu;       // ultimate shear capacity   [Unit=Force]
    double dVc;       // ultimate shear resistance of concrete      [Unit=Force]
    double dRatV;
    double dPv;       // Pv, Tendon 분력 Pz     [Unit=Force]
    double dVu_max;   // Vu.max  [Unit=Force]
    double dVu_min;   // Vu.min  [Unit=Force]
    double dAsv;      // Asv     [Unit=Area]
    double dAsv_min;  // Asv.min [Unit=Area]
    double dVmax;     // For BS  [Unit=Force]

    double ds;
    double dMcr;
    double dVp;
    double dAv_s;
    double dAv_s_req;
    double dAsv_req;
    double dJointVu;
    BOOL   bUncrack; //True 이면 Uncrack False 이면 Crack
	BOOL   bJoint;
	BOOL   bOK;
    
    // Longitudinal shear
    bool  bChk_ls;
    bool  bOK_ls;
    double dV1;
    double dV1_lim1;
    double dV1_lim2;
    double drho_ls;
    double drho_ls_lim;
    double ds_ls;
    double ds_ls_lim1;
    double ds_ls_lim2;

    void Initialize()
    {
        bCHK     = FALSE;
        LcomK    = 0;
        nMax     = 0;
        nKind    = 0;
        ForBe.Initialize();
        memset(dForceTP, 0, sizeof(dForceTP));
        dFx      = 0.0;
        dMy      = 0.0;
        dVz      = 0.0;
        dT       = 0.0;
        dVu      = 0.0;
        dVc      = 0.0;
        dRatV    = 0.0;
        dPv      = 0.0;
        dVu_max  = 0.0;
        dVu_min  = 0.0;
        dAsv     = 0.0;
        dAsv_min = 0.0;
        dVmax    = 0.0;
        ds       = 0.0;
        dMcr     = 0.0;
        dVp      = 0.0;
        dAv_s    = 0.0;
        dAv_s_req= 0.0;
        dAsv_req = 0.0;
        bUncrack = FALSE;
        bOK      = FALSE;

        bChk_ls = FALSE;
        bOK_ls  = FALSE;
        dV1 = 0.0;
        dV1_lim1 = 0.0;
        dV1_lim2 = 0.0;
        drho_ls = 0.0;
        drho_ls_lim = 0.0;
        ds_ls = 0.0;
        ds_ls_lim1 = 0.0;
        ds_ls_lim2 = 0.0;
		bJoint = FALSE;
		dJointVu = 0.0;
    }

	void Convert895(const T_SHRR_PSC_BS_BASE_895 &rData)
	{
		bCHK = rData.bCHK;
		LcomK = rData.LcomK;
		nMax = rData.nMax;
		nKind = rData.nKind;
		ForBe = rData.ForBe;
		for (int i = 0; i < 6; ++i) dForceTP[i] = rData.dForceTP[i];
		dFx = rData.dFx;
		dMy = rData.dMy;
		dVz = rData.dVz;
		dT = rData.dT;
		dVu = rData.dVu;
		dVc = rData.dVc;
		dRatV = rData.dRatV;
		dPv = rData.dPv;
		dVu_max = rData.dVu_max;
		dVu_min = rData.dVu_min;
		dAsv = rData.dAsv;
		dAsv_min = rData.dAsv_min;
		dVmax = rData.dVmax;
		ds = rData.ds;
		dMcr = rData.dMcr;
		dVp = rData.dVp;
		dAv_s = rData.dAv_s;
		dAv_s_req = rData.dAv_s_req;
		dAsv_req = rData.dAv_s_req;
		bUncrack = rData.bUncrack;
		bOK = rData.bOK;

		bChk_ls = rData.bChk_ls;
        bOK_ls = rData.bOK_ls;
		dV1 = rData.dV1;
        dV1_lim1 = rData.dV1_lim1;
		dV1_lim2 = rData.dV1_lim2;
		drho_ls = rData.drho_ls;
		drho_ls_lim = rData.drho_ls_lim;
		ds_ls = rData.ds_ls;
		ds_ls_lim1 = rData.ds_ls_lim1;
		ds_ls_lim2 = rData.ds_ls_lim2;
		bJoint = FALSE;
		dJointVu = 0.0;
	}

    T_SHRR_PSC_BS_BASE () {Initialize();}
};

struct T_SHRR_PSC_BS_895
{
	BOOL bComposite;
	T_SHRR_PSC_BS_BASE_895 ShrrB[2]; // 0=I-Cr.Ratio, 1=J-Cr.Ratio

	void Initialize()
	{
		bComposite = FALSE;
		for (int i = 0; i < 2; i++) ShrrB[i].Initialize();
	}
	T_SHRR_PSC_BS_895() { Initialize(); }
};


struct T_SHRR_PSC_BS
{
    BOOL bComposite;
    T_SHRR_PSC_BS_BASE ShrrB[2]; // 0=I-Cr.Ratio, 1=J-Cr.Ratio

    void Initialize()
    {
        bComposite = FALSE;
		for (int i = 0; i < 2; i++) ShrrB[i].Initialize();
    }
	void Convert895(const T_SHRR_PSC_BS_895& rData)
	{
		bComposite = FALSE;
		for (int i = 0; i < 2; i++) ShrrB[i].Convert895(rData.ShrrB[i]);
	}
    T_SHRR_PSC_BS() { Initialize(); }
};

struct T_SHRR_PSC_BS_UNIT
{
    T_SHRR_PSC_BS_UNIT()
    {
        dFx      = D_UNITSYS_BASE_FORCE;
        dMy      = D_UNITSYS_BASE_MOMENT;
        dVz      = D_UNITSYS_BASE_FORCE;
        dT       = D_UNITSYS_BASE_MOMENT;
        dVu      = D_UNITSYS_BASE_FORCE;
        dVc      = D_UNITSYS_BASE_FORCE;
        dPv      = D_UNITSYS_BASE_FORCE;
        dVu_max  = D_UNITSYS_BASE_FORCE;
        dVu_min  = D_UNITSYS_BASE_FORCE;
        dAsv     = D_UNITSYS_BASE_AREA;
        dAsv_min = D_UNITSYS_BASE_AREA;
        dVmax    = D_UNITSYS_BASE_FORCE;
        ds       = D_UNITSYS_BASE_LENGTH;
        dMcr     = D_UNITSYS_BASE_MOMENT;
        dVp      = D_UNITSYS_BASE_FORCE;
        dAv_s    = D_UNITSYS_BASE_UNIT_AREA;
        dAv_s_req= D_UNITSYS_BASE_UNIT_AREA;
        dAsv_req = D_UNITSYS_BASE_AREA;
        dJointVu = D_UNITSYS_BASE_FORCE;
        dV1      = D_UNITSYS_BASE_UNIT_AREA;
        dV1_lim1 = D_UNITSYS_BASE_UNIT_AREA;
        dV1_lim2 = D_UNITSYS_BASE_UNIT_AREA;
        ds_ls    = D_UNITSYS_BASE_LENGTH;
        ds_ls_lim1 = D_UNITSYS_BASE_LENGTH;
        ds_ls_lim2 = D_UNITSYS_BASE_LENGTH;
    }
    int dFx;
    int dMy;
    int dVz;
    int dT;
    int dVu;
    int dVc;
    int dPv;
    int dVu_max;
    int dVu_min;
    int dAsv;
    int dAsv_min;
    int dVmax;
    int ds;
    int dMcr;
    int dVp;
    int dAv_s;
    int dAv_s_req;
    int dAsv_req;
    int dJointVu;
    int dV1;
    int dV1_lim1;
    int dV1_lim2;
    int ds_ls;
    int ds_ls_lim1;
    int ds_ls_lim2;
};

struct T_TORR_PSC_BS_BASE // Torsional resistance
{
    BOOL bCHK;
    UINT LcomK;
    int  nMax;
    int  nKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    BOOL bBox;

    T_COMF_BASE ForBe; // 거더
    double dForceTP[6];
    double dFx;       // Fx, 부재력 [Unit=Force]
    double dMy;       // My, 부재력 [Unit=Moment]
    double dVz;       // Fz, 부재력 [Unit=Force]
    double dT;        // Mx, 부재력 [Unit=Moment]
    double dVu;       // ultimate shear capacity   [Unit=Force]
    double dTu;       // ???   [Unit=Moment]
    double dv;
    double dv_t;      // torsional shear stress
    double dv_tmin;
    double dv_tu;     // ultimate torsional shear stress
    double dy1;       // y_1 is the larger centre-line dimension of a link
    double dv_tu_550;  // [Unit=Stress]
    double dAst_st;    // [Unit=Unit Area]
    double dAst_st_req;// [Unit=Unit Area]
    double dAsl_sl;    // [Unit=Unit Area]
    double dAsl_sl_req;// [Unit=Unit Area]
    double dRatT;     // Tu/pTn
    double dRatVT;    // Vu/pVn + Tu/pTn
    BOOL   bOK;

    void Initialize()
    {
        bCHK     = FALSE;
        LcomK    = 0;
        nMax     = 0;
        nKind    = 0;
        bBox     = FALSE;
        ForBe.Initialize();
        memset(dForceTP, 0, sizeof(dForceTP));
        dFx      = 0.0;
        dMy      = 0.0;
        dVz      = 0.0;
        dT       = 0.0;
        dVu      = 0.0;
        dTu      = 0.0;
        dv       = 0.0;
        dv_t     = 0.0;
        dv_tmin  = 0.0;
        dv_tu    = 0.0;
        dy1      = 0.0;
        dRatT    = 0.0;
        dRatVT   = 0.0;
        dv_tu_550   = 0.0;
        dAst_st     = 0.0;
        dAst_st_req = 0.0;
        dAsl_sl     = 0.0;
        dAsl_sl_req = 0.0;
        bOK      = FALSE;
    }

    T_TORR_PSC_BS_BASE () {Initialize();}
};

struct T_TORR_PSC_BS
{
    BOOL bComposite;
    T_TORR_PSC_BS_BASE TorrB[2]; // 0=I-Cr.Ratio, 1=J-Cr.Ratio

    void Initialize()
    {
        bComposite = FALSE;
        for(int i=0; i<2; i++) TorrB[i].Initialize();
    }
    T_TORR_PSC_BS() { Initialize(); }
};

struct T_TORR_PSC_BS_UNIT
{
    T_TORR_PSC_BS_UNIT()
    {
        dFx      = D_UNITSYS_BASE_FORCE;
        dMy      = D_UNITSYS_BASE_MOMENT;
        dVz      = D_UNITSYS_BASE_FORCE;
        dT       = D_UNITSYS_BASE_MOMENT;
        dVu      = D_UNITSYS_BASE_FORCE;
        dTu      = D_UNITSYS_BASE_MOMENT;
        dv       = D_UNITSYS_BASE_STRESS;
        dv_t     = D_UNITSYS_BASE_STRESS;
        dv_tmin  = D_UNITSYS_BASE_STRESS;
        dv_tu    = D_UNITSYS_BASE_STRESS;
        dy1      = D_UNITSYS_BASE_LENGTH;
        dv_tu_550   = D_UNITSYS_BASE_STRESS;
        dAst_st     = D_UNITSYS_BASE_UNIT_AREA;
        dAst_st_req = D_UNITSYS_BASE_UNIT_AREA;
        dAsl_sl     = D_UNITSYS_BASE_UNIT_AREA;
        dAsl_sl_req = D_UNITSYS_BASE_UNIT_AREA;
    }
    int dFx;
    int dMy;
    int dVz;
    int dT;
    int dVu;
    int dTu;
    int dv;
    int dv_t;
    int dv_tmin;
    int dv_tu;
    int dy1;
    int dv_tu_550;
    int dAst_st;
    int dAst_st_req;
    int dAsl_sl;
    int dAsl_sl_req;
};

struct T_TORR_PSC_TMH7_UNIT
{
    T_TORR_PSC_TMH7_UNIT()
    {
        dFx = D_UNITSYS_BASE_FORCE;
        dMy = D_UNITSYS_BASE_MOMENT;
        dVz = D_UNITSYS_BASE_FORCE;
        dT = D_UNITSYS_BASE_MOMENT;
        dVu = D_UNITSYS_BASE_FORCE;
        dTu = D_UNITSYS_BASE_MOMENT;
        dv = D_UNITSYS_BASE_STRESS;
        dv_t = D_UNITSYS_BASE_STRESS;
        dv_tmin = D_UNITSYS_BASE_STRESS;
        dv_tu = D_UNITSYS_BASE_STRESS;
        dy1 = D_UNITSYS_BASE_LENGTH;
        dv_tu_550 = D_UNITSYS_BASE_STRESS;
        dAst_st = D_UNITSYS_BASE_UNIT_AREA;
        dAst_st_req = D_UNITSYS_BASE_UNIT_AREA;
        dAsl_sl = D_UNITSYS_BASE_AREA;
        dAsl_sl_req = D_UNITSYS_BASE_AREA;
    }
    int dFx;
    int dMy;
    int dVz;
    int dT;
    int dVu;
    int dTu;
    int dv;
    int dv_t;
    int dv_tmin;
    int dv_tu;
    int dy1;
    int dv_tu_550;
    int dAst_st;
    int dAst_st_req;
    int dAsl_sl;
    int dAsl_sl_req;
};


// Common
struct T_BDCW_PSC_COM
{
    T_BDCW_PSC_AS *pAS;
    T_BDCW_PSC_BS *pBS;

    void Initialize()
    {
        pAS = NULL;
        pBS = NULL;
    }
    T_BDCW_PSC_COM() { Initialize(); }
};

struct T_FYSG_PSC_BS_BASE
{
    BOOL bCHK;		//설계결과유무
    UINT StageK;	//스테이지번호		[Unit=None]
    UINT StepK;   // Step Key.      [Unit=None]
    double dFT;   //상단응력도
    double dFB;   //하단응력도
    double dFTL;	//상단좌단 합성응력도	[Unit=Stress]
    double dFBL;	//하단좌단 합성응력도	[Unit=Stress]
    double dFTR;	//상단우단 합성응력도	[Unit=Stress]
    double dFBR;	//하단우단 합성응력도	[Unit=Stress]
    double dFMAX;	//최대/최소 합성응력도	[Unit=Stress]
    double dALW;	//허용응력도			[Unit=Stress]
    double dfci;
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        StageK = StepK=0;
        dFT = dFB = 0.0;
        dFTL = dFBL = dFTR = dFBR = dFMAX = dALW = 0.0;
        dfci = 0.0;
        bOK = TRUE;
    }
    T_FYSG_PSC_BS_BASE() { Initialize(); }
};
struct T_FYSG_PSC_BS	// ElemK.
{
    BOOL bComposite;
    T_FYSG_PSC_BS_BASE FysgGrB[4];	// Girder, 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    T_FYSG_PSC_BS_BASE FysgSlB[4];  // Slab

    void Initialize()
    {
        bComposite = FALSE;
        for ( int i=0; i<4; ++i )
        {
            FysgGrB[i].Initialize();
            FysgSlB[i].Initialize();
        }
    }
    T_FYSG_PSC_BS() { Initialize(); }
};

struct T_FYSG_PSC_BS_UNIT
{
    T_FYSG_PSC_BS_UNIT()
    {
        dFT		= D_UNITSYS_BASE_STRESS;
        dFB		= D_UNITSYS_BASE_STRESS;
        dFTL	= D_UNITSYS_BASE_STRESS;
        dFBL	= D_UNITSYS_BASE_STRESS;
        dFTR	= D_UNITSYS_BASE_STRESS;
        dFBR	= D_UNITSYS_BASE_STRESS;
        dFMAX	= D_UNITSYS_BASE_STRESS;
        dALW	= D_UNITSYS_BASE_STRESS;
        dfci    = D_UNITSYS_BASE_STRESS;
    }
    int dFT;
    int dFB;
    int dFTL;
    int dFBL;
    int dFTR;
    int dFBR;
    int dFMAX;
    int dALW;
    int dfci;
};

// Stresses in concrete during service load combinations 

struct T_FYCM_PSC_BS_BASE_895
{
    BOOL bCHK;
    UINT LcomK;
    int  nMax;		//MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                //FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int  nKind;   // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int  nLSTerm; // Long, Short Term 구별 
    double dFT;		//[Unit=Stress]
    double dFB;		//[Unit=Stress]
    double dFTL;	//[Unit=Stress]
    double dFBL;	//[Unit=Stress]
    double dFTR;	//[Unit=Stress]
    double dFBR;	//[Unit=Stress]
    double dFMAX;	//[Unit=Stress]
    double dALW;	//[Unit=Stress]
    double dDepthFac;
    double dFlexTensC3; //[Unit=Stress]
    double dAddReinStress; //[Unit=Stress]
    double dTensConcArea; //[Unit=Area]
    double dTensRebarArea; //[Unit=Area]
    BOOL bOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = nMax = nKind = nLSTerm = 0;
        dFT = dFB = 0.0;
        dFTL = dFBL = dFTR = dFBR = dFMAX = dALW = 0;
        dDepthFac = 0.0;
        dFlexTensC3 =0.0;
        dAddReinStress =0.0;
        dTensConcArea = 0.0;
        dTensRebarArea = 0.0;
        bOK = FALSE;
    }

    T_FYCM_PSC_BS_BASE_895 () { Initialize(); }
};


struct T_FYCM_PSC_BS_BASE_955
{
    BOOL bCHK;
    UINT LcomK;
    int  nMax;		//MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
    //FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int  nKind;   // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int  nLSTerm; // Long, Short Term 구별 
    double dFT;		//[Unit=Stress]
    double dFB;		//[Unit=Stress]
    double dFTL;	//[Unit=Stress]
    double dFBL;	//[Unit=Stress]
    double dFTR;	//[Unit=Stress]
    double dFBR;	//[Unit=Stress]
    double dFMAX;	//[Unit=Stress]
    double dALW;	//[Unit=Stress]
    double dDepthFac;
    double dFlexTensC3; //[Unit=Stress]
    double dAddReinStress; //[Unit=Stress]
    double dTensConcArea; //[Unit=Area]
    double dTensRebarArea; //[Unit=Area]
    double dJointStressLimit;
    BOOL bOK;
    BOOL bJointExist;
    BOOL bJointOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = nMax = nKind = nLSTerm = 0;
        dFT = dFB = 0.0;
        dFTL = dFBL = dFTR = dFBR = dFMAX = dALW = 0;
        dDepthFac = 0.0;
        dFlexTensC3 = 0.0;
        dAddReinStress = 0.0;
        dTensConcArea = 0.0;
        dTensRebarArea = 0.0;
        dJointStressLimit = 0.0;
        bOK = TRUE;
        bJointExist = FALSE;
        bJointOK = TRUE;
    }
    T_FYCM_PSC_BS_BASE_955() { Initialize(); }
};

struct T_FYCM_PSC_BS_BASE
{
    BOOL bCHK;
    UINT LcomK;
    int  nMax;		//MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                //FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int  nKind;   // 기준별 제시하는 하중조합 타입을 구별하기 위해.
    int  nLSTerm; // Long, Short Term 구별 
    double dFT;		//[Unit=Stress]
    double dFB;		//[Unit=Stress]
    double dFTL;	//[Unit=Stress]
    double dFBL;	//[Unit=Stress]
    double dFTR;	//[Unit=Stress]
    double dFBR;	//[Unit=Stress]
    double dFMAX;	//[Unit=Stress]
    double dALW;	//[Unit=Stress]
    double dDepthFac;
    double dFlexTensC3; //[Unit=Stress]
    double dAddReinStress; //[Unit=Stress]
    double dTensConcArea; //[Unit=Area]
    double dTensRebarArea; //[Unit=Area]
    double dJointStressLimit;
    double dfs;
    double dfsa;
    BOOL bOK;
    BOOL bJointExist;
    BOOL bJointOK;

    void Initialize()
    {
        bCHK = 0;
        LcomK = nMax = nKind = nLSTerm = 0;
        dFT = dFB = 0.0;
        dFTL = dFBL = dFTR = dFBR = dFMAX = dALW = 0;
        dDepthFac = 0.0;
        dFlexTensC3 = 0.0;
        dAddReinStress = 0.0;
        dTensConcArea = 0.0;
        dTensRebarArea = 0.0;
        dJointStressLimit = 0.0;
        dfs = 0.0;
        dfsa = 0.0;
        bOK = TRUE;
        bJointExist = FALSE;
        bJointOK = TRUE;
    };

    void Convert955(const T_FYCM_PSC_BS_BASE_955& rData)
    {
        Initialize();
        bCHK = rData.bCHK;
        LcomK = rData.LcomK;
        nMax = rData.nMax;
        nKind = rData.nKind;
        nLSTerm = rData.nLSTerm;
        dFT = rData.dFT;
        dFB = rData.dFB;
        dFTL = rData.dFTL;
        dFBL = rData.dFBL;
        dFTR = rData.dFTR;
        dFBR = rData.dFBR;
        dFMAX = rData.dFMAX;
        dALW = rData.dALW;
        dDepthFac = rData.dDepthFac;
        dFlexTensC3 = rData.dFlexTensC3;
        dAddReinStress = rData.dAddReinStress;
        dTensConcArea = rData.dTensConcArea;
        dTensRebarArea = rData.dTensRebarArea;
        bOK = rData.bOK;
        dJointStressLimit = 0.0;
        dfs = 0.0;
        dfsa = 0.0;
        bJointExist = FALSE;
        bJointOK = TRUE;
    }


    void Convert895(const T_FYCM_PSC_BS_BASE_895 &rData)
    {
        Initialize();
        bCHK = rData.bCHK;
        LcomK = rData.LcomK;
        nMax = rData.nMax;
        nKind = rData.nKind;
        nLSTerm = rData.nLSTerm;
        dFT = rData.dFT;
        dFB = rData.dFB;
        dFTL = rData.dFTL;
        dFBL = rData.dFBL;
        dFTR = rData.dFTR;
        dFBR = rData.dFBR;
        dFMAX = rData.dFMAX;
        dALW = rData.dALW;
        dDepthFac = rData.dDepthFac;
        dFlexTensC3 = rData.dFlexTensC3;
        dAddReinStress = rData.dAddReinStress;
        dTensConcArea = rData.dTensConcArea;
        dTensRebarArea = rData.dTensRebarArea;
        bOK = rData.bOK;
        dJointStressLimit = 0.0;
        dfs = 0.0;
        dfsa = 0.0;
        bJointExist = FALSE;
        bJointOK = TRUE;
    }

    T_FYCM_PSC_BS_BASE() { Initialize(); }

    double Get_Max()
    {
        double dTopMax = max(dFT, max(dFTL, dFTR));
        double dBotMax = max(dFB, max(dFBL, dFBR));
        return max(dTopMax, dBotMax);
    }
    double Get_Min()
    {
        double dTopMin = min(dFT, min(dFTL, dFTR));
        double dBotMin = min(dFB, min(dFBL, dFBR));
        return min(dTopMin, dBotMin);
    }
};

struct T_FYCM_PSC_BS_895	// ElemK.
{
    BOOL bComposite;
    T_FYCM_PSC_BS_BASE_895 FycmGrB[4];	// Girder, 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    T_FYCM_PSC_BS_BASE_895 FycmSlB[4];  // Slab
    
    void Initialize()
    {
        bComposite = FALSE;
        for ( int i=0; i<4; ++i )
        {
            FycmGrB[i].Initialize();
            FycmSlB[i].Initialize();
        }
    }
    T_FYCM_PSC_BS_895() { Initialize(); }
};

struct T_FYCM_PSC_BS_955	// ElemK.
{
    BOOL bComposite;
    T_FYCM_PSC_BS_BASE_955 FycmGrB[4];	// Girder, 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    T_FYCM_PSC_BS_BASE_955 FycmSlB[4];  // Slab

    void Initialize()
    {
        bComposite = FALSE;
        for (int i = 0; i < 4; ++i)
        {
            FycmGrB[i].Initialize();
            FycmSlB[i].Initialize();
        }
    }
    T_FYCM_PSC_BS_955() { Initialize(); }
};

struct T_FYCM_PSC_BS	// ElemK.
{
    BOOL bComposite;
    T_FYCM_PSC_BS_BASE FycmGrB[4];	// Girder, 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    T_FYCM_PSC_BS_BASE FycmSlB[4];  // Slab

    void Initialize()
    {
        bComposite = FALSE;
        for ( int i=0; i<4; ++i )
        {
            FycmGrB[i].Initialize();
            FycmSlB[i].Initialize();
        }
    }
    void Convert895(const T_FYCM_PSC_BS_895& rData)
    {
        bComposite = FALSE;
        for ( int i=0; i<4; ++i )
        {
            FycmGrB[i].Convert895(rData.FycmGrB[i]);
            FycmSlB[i].Convert895(rData.FycmGrB[i]);
        }
    }
    void Convert955(const T_FYCM_PSC_BS_955& rData)
    {
        bComposite = FALSE;
        for (int i = 0; i < 4; ++i)
        {
            FycmGrB[i].Convert955(rData.FycmGrB[i]);
            FycmSlB[i].Convert955(rData.FycmGrB[i]);
        }
    }
    T_FYCM_PSC_BS() { Initialize(); }
};

struct T_FYCM_PSC_BS_UNIT
{
    T_FYCM_PSC_BS_UNIT()
    {
        dFT		= D_UNITSYS_BASE_STRESS;
        dFB		= D_UNITSYS_BASE_STRESS;
        dFTL	= D_UNITSYS_BASE_STRESS;
        dFBL	= D_UNITSYS_BASE_STRESS;
        dFTR	= D_UNITSYS_BASE_STRESS;
        dFBR	= D_UNITSYS_BASE_STRESS;
        dFMAX	= D_UNITSYS_BASE_STRESS;
        dALW	= D_UNITSYS_BASE_STRESS;
        dFlexTensC3 = D_UNITSYS_BASE_STRESS;
        dAddReinStress= D_UNITSYS_BASE_STRESS;
        dTensConcArea = D_UNITSYS_BASE_AREA;
        dTensRebarArea = D_UNITSYS_BASE_AREA;
        dJointStressLimit = D_UNITSYS_BASE_STRESS;
    }

    int dFT;
    int dFB;
    int dFTL;
    int dFBL;
    int dFTR;
    int dFBR;
    int dFMAX;
    int dALW;
    int dFlexTensC3;
    int dAddReinStress;
    int dTensConcArea;
    int dTensRebarArea;
    int dJointStressLimit;
};

struct T_FYCM_PSC_TMH7_UNIT
{
    T_FYCM_PSC_TMH7_UNIT()
    {
        dFT = D_UNITSYS_BASE_STRESS;
        dFB = D_UNITSYS_BASE_STRESS;
        dFTL = D_UNITSYS_BASE_STRESS;
        dFBL = D_UNITSYS_BASE_STRESS;
        dFTR = D_UNITSYS_BASE_STRESS;
        dFBR = D_UNITSYS_BASE_STRESS;
        dFMAX = D_UNITSYS_BASE_STRESS;
        dALW = D_UNITSYS_BASE_STRESS;
        dFlexTensC3 = D_UNITSYS_BASE_STRESS;
        dAddReinStress = D_UNITSYS_BASE_STRESS;
        dTensConcArea = D_UNITSYS_BASE_AREA;
        dTensRebarArea = D_UNITSYS_BASE_AREA;
        dJointStressLimit = D_UNITSYS_BASE_STRESS;
        dfs = D_UNITSYS_BASE_STRESS;
        dfsa = D_UNITSYS_BASE_STRESS;
    }

    int dFT;
    int dFB;
    int dFTL;
    int dFBL;
    int dFTR;
    int dFBR;
    int dFMAX;
    int dALW;
    int dFlexTensC3;
    int dAddReinStress;
    int dTensConcArea;
    int dTensRebarArea;
    int dJointStressLimit;
    int  dfs;
    int  dfsa;
};


//==================================================================================
// Tensile stresses in prestressing tendons
//==================================================================================

//TODO BS 5400 Table 추가 여부 확인
struct T_FYPC_PSC_BS	// TdnaK.
{
    BOOL bCHK;
    int nLoadType;  // 0=Pre-tension, 1=Post-tension 

    double dFpu;    // Tensile stress of tendon

    double dFDL1;	// Pre-Tensioning: Stress prior to transfer. Jacking stress. [Unit=Stress]
                    // Post-Tensioning: Stress prior to seating. Jacking stress. [Unit=Stress]

    double dFDL2;	// Pre-Tensioning: Not used. [Unit=Stress]		
                    // Post-Tensioning: Stress at anchorage and couplers immediately after anchor set. [Unit=Stress]

    double dFLL1;	// Stress at service limit after all losses. [Unit=Stress]

    double dAFDL1;	// Pre-Tensioning: Allowable stress prior to transfer. Allowable jacking stress. [Unit=Stress]
                    // Post-Tensioning: Allowable stress prior to seating. Allowable jacking stress. [Unit=Stress]

    BOOL bOK;

    void Initialize()
    {
        bCHK = FALSE;
        nLoadType = 0;
        dFpu = 0.0;
        dFDL1 = dFDL2 = dFLL1 = 0.0;
        dAFDL1 = 0.0;
        bOK = TRUE;
    }
    T_FYPC_PSC_BS() { Initialize(); }
};

struct T_FYPC_PSC_BS_UNIT
{
    T_FYPC_PSC_BS_UNIT()
    {
        dFpu    = D_UNITSYS_BASE_STRESS;
        dFDL1	= D_UNITSYS_BASE_STRESS;
        dFDL2	= D_UNITSYS_BASE_STRESS;
        dFLL1	= D_UNITSYS_BASE_STRESS;
        dAFDL1	= D_UNITSYS_BASE_STRESS;
    }

    int dFpu;
    int dFDL1;
    int dFDL2;
    int dFLL1;
    int dAFDL1;
};

/////////////////IRS-Indian Railway Standards
//============================================================================
// 1. Stresses in concrete during construction stage
//============================================================================
struct T_FYSG_PSC_BASE_IRS
{
	BOOL bCHK;		//설계결과유무
	UINT StageK;	//스테이지번호		[Unit=None]
	UINT StepK;   // Step Key.      [Unit=None]
	double dFT;   //상단응력도
	double dFB;   //하단응력도
	double dFTL;	//상단좌단 합성응력도	[Unit=Stress]
	double dFBL;	//하단좌단 합성응력도	[Unit=Stress]
	double dFTR;	//상단우단 합성응력도	[Unit=Stress]
	double dFBR;	//하단우단 합성응력도	[Unit=Stress]
	double dFMAX;	//최대/최소 합성응력도	[Unit=Stress]
	double dALW;	//허용응력도			[Unit=Stress]
	BOOL bOK;
	void Initialize()
	{
		bCHK = 0;
		StageK = StepK=0;
		dFT = dFB = 0.0;
		dFTL = dFBL = dFTR = dFBR = dFMAX = dALW = 0;
		bOK = TRUE;
	}	
	T_FYSG_PSC_BASE_IRS() { Initialize(); }
};
	
struct T_FYSG_PSC_IRS	// ElemK.
{
	T_FYSG_PSC_BASE_IRS FysgBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

	void Initialize()
	{
		for(int i=0; i<4; i++)
			FysgBase[i].Initialize();
	}
	T_FYSG_PSC_IRS() { Initialize(); }
};

struct T_FYSG_PSC_UNIT_IRS
{
	T_FYSG_PSC_UNIT_IRS()
	{
		dFT		= D_UNITSYS_BASE_STRESS;
		dFB		= D_UNITSYS_BASE_STRESS;
		dFTL	= D_UNITSYS_BASE_STRESS;
		dFBL	= D_UNITSYS_BASE_STRESS;
		dFTR	= D_UNITSYS_BASE_STRESS;
		dFBR	= D_UNITSYS_BASE_STRESS;
		dFMAX	= D_UNITSYS_BASE_STRESS;
		dALW	= D_UNITSYS_BASE_STRESS;	
	}
	int dFT;
	int dFB;
	int dFTL;
	int dFBL;
	int dFTR;
	int dFBR;
	int dFMAX;
	int dALW;	
};

//==================================================================================
// 2. Stresses in concrete during service load combinations 
//==================================================================================

struct T_FYCM_PSC_BASE_IRS
{  
	BOOL bCHK;		
	UINT LcomK;
	int iMax;		     //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
	//                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
	int iKind;    // 기준별 제시하는 하중조합 타입을 구별하기 위해.
	int iLSTerm;  // Long, Short Term 구별 
	double dFT;		//[Unit=Stress]
	double dFB;		//[Unit=Stress]
	double dFTL;	//[Unit=Stress]
	double dFBL;	//[Unit=Stress]
	double dFTR;	//[Unit=Stress]
	double dFBR;	//[Unit=Stress]
	double dFMAX;	//[Unit=Stress]
	double dALW;	//[Unit=Stress]
	BOOL bOK;

	void Initialize()
	{
		bCHK = 0;
		LcomK = iMax = iKind = iLSTerm = 0;
		dFT = dFB = 0.0;
		dFTL = dFBL = dFTR = dFBR = dFMAX = dALW = 0;
		bOK = TRUE;
	}
	T_FYCM_PSC_BASE_IRS() { Initialize(); }
};

struct T_FYCM_PSC_IRS	// ElemK.
{
	T_FYCM_PSC_BASE_IRS FycmBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

	void Initialize()
	{
		for(int i=0; i<4; i++) FycmBase[i].Initialize();
	}
	T_FYCM_PSC_IRS() { Initialize(); }	
};

struct T_FYCM_PSC_UNIT_IRS
{
	T_FYCM_PSC_UNIT_IRS()
	{
		dFT		= D_UNITSYS_BASE_STRESS;		
		dFB		= D_UNITSYS_BASE_STRESS;		
		dFTL	= D_UNITSYS_BASE_STRESS;
		dFBL	= D_UNITSYS_BASE_STRESS;
		dFTR	= D_UNITSYS_BASE_STRESS;	
		dFBR	= D_UNITSYS_BASE_STRESS;
		dFMAX	= D_UNITSYS_BASE_STRESS;
		dALW	= D_UNITSYS_BASE_STRESS;	
	}

	int dFT;		
	int dFB;		
	int dFTL;	
	int dFBL;	
	int dFTR;	
	int dFBR;	
	int dFMAX;	
	int dALW;	
};

//==================================================================================
// Tensile stresses in prestressing tendons
//==================================================================================

// struct T_FYPC_AASHTO	// TdnaK. 이걸 사용하자. 

// Check Principal stress for stage
struct T_FPSG_PSC_BASE_IRS 
{
	BOOL bCHK;		//설계결과유무 
	UINT StageK;
	UINT StepK;
	int iKind;    // 시공단계에서 필요할 수 있기 때문에. 
	double dFP1;	//1위치 주응력		[Unit=Stress]
	double dFP2;	//2위치 주응력		[Unit=Stress]
	double dFP3;	//3위치 주응력		[Unit=Stress]
	double dFP4;	//4위치 주응력		[Unit=Stress]
	double dFP5;	//5위치 주응력		[Unit=Stress]
	double dFP6;	//6위치 주응력		[Unit=Stress]
	double dFP7;	//7위치 주응력		[Unit=Stress]
	double dFP8;	//8위치 주응력		[Unit=Stress]
	double dFP9;	//9위치 주응력		[Unit=Stress]
	double dFP10;	//10위치 주응력		[Unit=Stress]
	double dFMAX; // MAX/MIN 주응력	[Unit=Stress]
	double dAFP;  // 허용 주응력    [Unit=Stress]
	BOOL bOK;

	void Initialize()
	{
		bCHK = 0;
		StageK = StepK = 0;
		iKind = 0;  
		iKind = 0;
		dFP1 = dFP2 = dFP3 = dFP4 = dFP5 = dFP6 = dFP7 = dFP8 = dFP9= dFP10 = dFMAX = dAFP = 0;
		bOK = TRUE;
	}
	T_FPSG_PSC_BASE_IRS() { Initialize(); }
};

struct T_FPSG_PSC_IRS	// ElemK.
{
	T_FPSG_PSC_BASE_IRS FpsgBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

	void Initialize()
	{
		for(int i=0; i<4; i++)
			FpsgBase[i].Initialize();
	}
	T_FPSG_PSC_IRS() { Initialize(); }
};

struct T_FPSG_PSC_UNIT_IRS
{
	T_FPSG_PSC_UNIT_IRS()
	{
		dFP1 = D_UNITSYS_BASE_STRESS;
		dFP2 = D_UNITSYS_BASE_STRESS;
		dFP3 = D_UNITSYS_BASE_STRESS;
		dFP4 = D_UNITSYS_BASE_STRESS;
		dFP5 = D_UNITSYS_BASE_STRESS;
		dFP6 = D_UNITSYS_BASE_STRESS;
		dFP7 = D_UNITSYS_BASE_STRESS;
		dFP8 = D_UNITSYS_BASE_STRESS;
		dFP9 = D_UNITSYS_BASE_STRESS;
		dFP10= D_UNITSYS_BASE_STRESS;
		dFMAX= D_UNITSYS_BASE_STRESS;
		dAFP = D_UNITSYS_BASE_STRESS; 
	}
	int dFP1;
	int dFP2;
	int dFP3;
	int dFP4;
	int dFP5;
	int dFP6;
	int dFP7;
	int dFP8;
	int dFP9;
	int dFP10;
	int dFMAX;
	int dAFP;  
};

// Check Principal stress
struct T_FPMS_PSC_BASE_IRS  
{
	BOOL bCHK;		//설계결과유무 
	UINT LcomK;
	int iMax;		  //MIN-MAX타입이 아님=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
	//                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
	int iKind;    // 기준별 제시하는 하중조합 타입을 구별하기 위해.
	int iLSTerm;  // Long, Short Term 구별 
	double dFP1;	//1위치 주응력		[Unit=Stress]
	double dFP2;	//2위치 주응력		[Unit=Stress]
	double dFP3;	//3위치 주응력		[Unit=Stress]
	double dFP4;	//4위치 주응력		[Unit=Stress]
	double dFP5;	//5위치 주응력		[Unit=Stress]
	double dFP6;	//6위치 주응력		[Unit=Stress]
	double dFP7;	//7위치 주응력		[Unit=Stress]
	double dFP8;	//8위치 주응력		[Unit=Stress]
	double dFP9;	//9위치 주응력		[Unit=Stress]
	double dFP10;	//10위치 주응력		[Unit=Stress]
	double dFMAX; // MAX/MIN 주응력	[Unit=Stress]
	double dAFP;  // 허용 주응력    [Unit=Stress]
	BOOL bOK;

	void Initialize()
	{
		bCHK = 0;
		LcomK = 0;
		iMax = 0;  
		iKind = iLSTerm = 0;
		dFP1 = dFP2 = dFP3 = dFP4 = dFP5 = dFP6 = dFP7 = dFP8 = dFP9= dFP10 = dFMAX = dAFP = 0;
		bOK = TRUE;
	}
	T_FPMS_PSC_BASE_IRS() { Initialize(); }
};

struct T_FPMS_PSC_IRS	// ElemK.
{
	T_FPMS_PSC_BASE_IRS FpmsBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

	void Initialize()
	{
		for(int i=0; i<4; i++)
			FpmsBase[i].Initialize();
	}
	T_FPMS_PSC_IRS() { Initialize(); }
};

struct T_FPMS_PSC_UNIT_IRS
{
	T_FPMS_PSC_UNIT_IRS()
	{
		dFP1 = D_UNITSYS_BASE_STRESS;
		dFP2 = D_UNITSYS_BASE_STRESS;
		dFP3 = D_UNITSYS_BASE_STRESS;
		dFP4 = D_UNITSYS_BASE_STRESS;
		dFP5 = D_UNITSYS_BASE_STRESS;
		dFP6 = D_UNITSYS_BASE_STRESS;
		dFP7 = D_UNITSYS_BASE_STRESS;
		dFP8 = D_UNITSYS_BASE_STRESS;
		dFP9 = D_UNITSYS_BASE_STRESS;
		dFP10= D_UNITSYS_BASE_STRESS;
		dFMAX= D_UNITSYS_BASE_STRESS;
		dAFP = D_UNITSYS_BASE_STRESS; 
	}
	int dFP1;
	int dFP2;
	int dFP3;
	int dFP4;
	int dFP5;
	int dFP6;
	int dFP7;
	int dFP8;
	int dFP9;
	int dFP10;
	int dFMAX;
	int dAFP;  
};

//==================================================================================
// Bending Resistance
//==================================================================================
struct T_BDCR_PSC_BASE_IRS
{
	BOOL bCHK;
	UINT LcomK;
	int iMax;
	int iKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.
	int iLSTerm;  // Long, Short Term 구별

	double dMu;	      // 계수모멘트, 작용부재력	[Unit=Moment]
	double dMn;       // 공칭강도, 제한값 적용, nominal flexural resistance [Unit=Moment]
	double dMn_cal;   // 공칭강도, 처음 계산, nominal flexural resistance [Unit=Moment]
	double dphi;      // 강도감소계수
	double dphiMn;    // phi*Mn
	double dRatM;     // Mu/(phi*Mn)
	double dc_neu;    // 중립축
	double dalpha;    //  0.85, 
	double dbeta1;    // a = beta1*c
	double da;        // 직사각형 Stress block a의 비
	double dPe;       // 인장측 유효긴장력			[Unit=Force]
	double dPe_t;     // 전체 유효긴장력        [Unit=Force]
	double dAps;	    // 인장측 PS강재의 단면적	[Unit=Area]
	double dAps_t;	  // 전체 PS강재의 단면적	[Unit=Area]
	double dAst;	    // 인장측 철근의 단면적		[Unit=Area]
	double dAsc;      // 압축측 철근의 단면적   [Unit=Area] 
	double dRho_pt;   // 인장측 tendon. [Unit=None]
	double dRho_st;   // 인장측 철근비			
	double dRho_sc;   // 압축측 철근비			
	double dRho_ps;   // 인장측 강재비(tendon+bar), (Ap+As)/(b*d) , q_b 강재지수?
	double ddpt;      // 압축연단에서 인장 Tendon의 도심까지 거리 [Unit=Length]
	double ddst;      // 압축연단에서 인장측 철근 도심까지의 거리		[Unit=Length]
	double ddsc;	    // 압축연단에서 압축측 철근 도심까지의 거리		[Unit=Length]  
	//
	double dphiMcr;   // 균열모멘트 [Unit=Moment]
	double dphiMn_mo; // Mn에 수정해서 사용할때 
	double dRho_p_max; // 최대 강재량 
	double dRho_p_min; // 최소 강재량   
	// KSCE-LSD12
	double dCmax;     // [L] 최대 중립축
	double dAs_max;   // [L^2] 인장철근 상한값
	double dAs_min;   // [L^2] 인장철근 하한값

	BOOL bOK;

	void Initialize()
	{
		bCHK = 0;
		LcomK=0;
		iMax=iKind=iLSTerm=0;
		dMu = dMn = dMn_cal = dphi = dphiMn = dRatM = dc_neu = dalpha = dbeta1 = da = 0.0;
		dPe = dPe_t = dAps = dAps_t = dAst = dAsc = 0.0;
		dRho_pt = dRho_st = dRho_sc = dRho_ps = 0.0;
		ddpt = ddst = ddsc = 0.0;
		dphiMcr = dphiMn_mo = 0.0;
		dRho_p_max = dRho_p_min = 0.0;
		dCmax = dAs_max = dAs_min = 0.0;
		bOK = 0;
	}
	T_BDCR_PSC_BASE_IRS() { Initialize(); }
};

struct T_BDCR_PSC_IRS	// ElemK.
{
	T_BDCR_PSC_BASE_IRS BdcrBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

	void Initialize()
	{
		for(int i=0; i<4; i++)
			BdcrBase[i].Initialize();
	}
	T_BDCR_PSC_IRS() { Initialize(); }
};

struct T_BDCR_PSC_UNIT_IRS
{
	T_BDCR_PSC_UNIT_IRS()
	{
		dMu       = D_UNITSYS_BASE_MOMENT;
		dMn       = D_UNITSYS_BASE_MOMENT;
		dMn_cal   = D_UNITSYS_BASE_MOMENT;
		dphiMn    = D_UNITSYS_BASE_MOMENT;
		dc_neu    = D_UNITSYS_BASE_LENGTH;
		da        = D_UNITSYS_BASE_LENGTH;
		dPe       = D_UNITSYS_BASE_FORCE; 
		dPe_t     = D_UNITSYS_BASE_FORCE; 
		dAps      = D_UNITSYS_BASE_AREA;	
		dAps_t    = D_UNITSYS_BASE_AREA;	
		dAst      = D_UNITSYS_BASE_AREA;	
		dAsc      = D_UNITSYS_BASE_AREA;  
		ddpt      = D_UNITSYS_BASE_LENGTH;
		ddst      = D_UNITSYS_BASE_LENGTH;
		ddsc      = D_UNITSYS_BASE_LENGTH;
		dphiMcr   = D_UNITSYS_BASE_MOMENT;
		dphiMn_mo = D_UNITSYS_BASE_MOMENT;
		dCmax     = D_UNITSYS_BASE_LENGTH;     
		dAs_max   = D_UNITSYS_BASE_AREA;
		dAs_min   = D_UNITSYS_BASE_AREA;


	}

	int dMu;
	int dMn;
	int dMn_cal;
	int dphiMn;
	int dc_neu;
	int da;
	int dPe; 
	int dPe_t; 
	int dAps;	
	int dAps_t;	
	int dAst;	
	int dAsc;  
	int ddpt;
	int ddst;
	int ddsc;
	int dphiMcr;
	int dphiMn_mo;
	int dCmax;     
	int dAs_max;   
	int dAs_min;   

};

//==================================================================================
// Crack check
//==================================================================================

struct T_BDCW_PSC_BASE_IRS
{
	BOOL bCHK;		//[Unit=None]

	UINT LcomK;		//[Unit=None] 
	int iKind;    // enum AASHTO_LCOMTYPE
	int iMax;
	int iLSTerm;  // Long, Short Term 구별

	double dPu;        //[Unit=Force]
	double dMu;        //[Unit=Moment]
	double dFT;		     //Top stress for 균열단면[Unit=Stress]
	double dFB;		     //Bottom stress for 균열단면[Unit=Stress]
	double dft;        //Top stress for gross sect. [Unit=Stress]
	double dfb;        //Bottom stress for gross sect.[Unit=Stress] 
	double dFSS;	     //[Unit=Stress]
	double dEpsilon_sm; // 철근 변형률,    [Unit=None]
	double dEpsilon_cm; // 콘크리트 변형률 [Unit=None]
	double dEpsi_sm_Epsi_cm; // ε_sm - ε_cm [Unit=None]
	double dSr_max;  // maximum crack spacing
	double ds_use;	//[Unit=Length]
	double ds_max;	//[Unit=Length]
	double dWk;     //[Unit=Length]
	double dWmax;   //[Unit=Length]
	double dRatio;    //[Unit=None]
	double dCrack_Asmin ; //[L^2]
	double dAst         ; //[L^2]

	BOOL bOK;

	BOOL bOK_Cr;
	BOOL bOk_Zero;

	//IRS 
	double dSL		;//mm Bar Spacing
	double dcnom	;//mm Nominal required cover
	double dDesCW	;//mm Design Crack Width
	double dEtamm	;//Strain Calculate
	double dEta1	;//Strain at level
	double dbt		;//mm Width of section at centroid of tension steel
	double dadash	;//mm distance from compresison face to point of crack
	double dMg		;//Nmm Moment due to permenent loads
	double dMq		;//Nmm Moment due to live loads
	double dEtas	;//Strain in tension R/F
	double dAten	;//mm2 Area of in tension R/F

	void Initialize()
	{
		bCHK = 0;
		LcomK = 0;
		iKind=iMax=iLSTerm=0;
		dFT = dFB = dFSS = 0.0;
		dft = dfb = 0.0;
		dEpsilon_sm = dEpsilon_cm = 0.0;
		dEpsi_sm_Epsi_cm = 0.0;
		dSr_max = 0.0;
		dft = dfb = 0.0;
		ds_use = ds_max = 0.0;
		dWk=dWmax=0.0;
		dRatio=0.0;
		bOK = TRUE;
		dCrack_Asmin = dAst =0.0;
		bOK_Cr = FALSE;
		bOk_Zero = FALSE;

		//IRS 
		dSL		= 0.;//mm Bar Spacing
		dcnom	= 0.;//mm Nominal required cover
		dDesCW	= 0.;//mm Design Crack Width
		dEtamm	= 0.;//Strain Calculate
		dEta1	= 0.;//Strain at level
		dbt		= 0.;//mm Width of section at centroid of tension steel
		dadash	= 0.;//mm distance from compresison face to point of crack
		dMg		= 0.;//Nmm Moment due to permenent loads
		dMq		= 0.;//Nmm Moment due to live loads
		dEtas	= 0.;//Strain in tension R/F
		dAten	= 0.;//mm2 Area of in tension R/F

	}
	T_BDCW_PSC_BASE_IRS() { Initialize(); }	
};

struct T_BDCW_PSC_IRS_875// ElemK.
{
	T_BDCW_PSC_BASE_IRS BdcwBase[4];	    // 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	T_BDCW_PSC_BASE_IRS BdcwBaseZero[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

	void Initialize()
	{
		for(int i=0; i<4; i++)
		{
			BdcwBase[i].Initialize();
			BdcwBaseZero[i].Initialize();
		}  
	}
	T_BDCW_PSC_IRS_875() { Initialize(); }		
};

struct T_BDCW_PSC_IRS// ElemK.
{
	T_BDCW_PSC_BASE_IRS BdcwBase[4];	    // 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	T_BDCW_PSC_BASE_IRS BdcwBaseZero[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
	T_BDCW_PSC_BASE_IRS BdcwBaseAsmin[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

	void Initialize()
	{
		for(int i=0; i<4; i++)
		{
			BdcwBase[i].Initialize();
			BdcwBaseZero[i].Initialize();
			BdcwBaseAsmin[i].Initialize();
		}  
	}
	void Convert875(const T_BDCW_PSC_IRS_875& rData)
	{
		for(int i=0; i<4; i++)
		{
			BdcwBase[i] = rData.BdcwBase[i];
			BdcwBaseZero[i] = rData.BdcwBaseZero[i];
			BdcwBaseAsmin[i].Initialize();
		}
	}
	T_BDCW_PSC_IRS() { Initialize(); }		
};

struct T_BDCW_PSC_UNIT_IRS
{
	T_BDCW_PSC_UNIT_IRS()
	{
		dPu = D_UNITSYS_BASE_FORCE;
		dMu = D_UNITSYS_BASE_MOMENT;
		dFT = D_UNITSYS_BASE_STRESS;	
		dFB = D_UNITSYS_BASE_STRESS;	
		dft = D_UNITSYS_BASE_STRESS;	
		dfb = D_UNITSYS_BASE_STRESS;	
		dFSS= D_UNITSYS_BASE_STRESS;
		dSr_max = D_UNITSYS_BASE_LENGTH; 

		ds_use = D_UNITSYS_BASE_LENGTH;
		ds_max = D_UNITSYS_BASE_LENGTH;
		dWk    = D_UNITSYS_BASE_LENGTH;
		dWmax  = D_UNITSYS_BASE_LENGTH;

		dCrack_Asmin = D_UNITSYS_BASE_AREA;
		dAst         = D_UNITSYS_BASE_AREA;

		//IRS 
		dSL		= D_UNITSYS_BASE_LENGTH;//mm Bar Spacing
		dcnom	= D_UNITSYS_BASE_LENGTH;//mm Nominal required cover
		dDesCW	= D_UNITSYS_BASE_LENGTH;//mm Design Crack Width
		dEtamm	= D_UNITSYS_BASE_STRESS;//Strain Calculate
		dEta1	= D_UNITSYS_BASE_STRESS;//Strain at level
		dbt		= D_UNITSYS_BASE_LENGTH;//mm Width of section at centroid of tension steel
		dadash	= D_UNITSYS_BASE_LENGTH;//mm distance from compresison face to point of crack
		dMg		= D_UNITSYS_BASE_MOMENT;//Nmm Moment due to permenent loads
		dMq		= D_UNITSYS_BASE_MOMENT;//Nmm Moment due to live loads
		dEtas	= D_UNITSYS_BASE_STRESS;//Strain in tension R/F
		dAten	= D_UNITSYS_BASE_AREA;//mm2 Area of in tension R/F
	}

	int dPu;
	int dMu;
	int dFT;	
	int dFB;	
	int dft;
	int dfb;
	int dFSS;
	int dSr_max;
	int ds_use;
	int ds_max;
	int dWk;
	int dWmax;
	int dCrack_Asmin ; 
	int dAst         ; 
	int dSL	;
	int dcnom	 ;
	int dDesCW	 ;
	int dEtamm	 ;
	int dEta1	 ;
	int dbt	 ;
	int dadash	 ;
	int dMg	 ;
	int dMq	 ;
	int dEtas	 ;
	int dAten	 ;

};

//==================================================================================
// Shear design
//==================================================================================
struct T_CUMS_PSC_BASE_IRS
{
	BOOL bCHK;			//[Unit=None]
	UINT LcomK;			//[Unit=None]
	int iMax;
	int iKind;      // 기준별 제시하는 하중조합 타입을 구별하기 위해.
	int iLSTerm;    // Long, Short Term 구별

	double dPu;     //[Unit=Force]
	double dVu;			//[Unit=Force]
	double dMu;			//[Unit=Moment]
	double dft;     //[Unit=Stress]
	double dfb;     //[Unit=Stress]
	double dVn;			//[Unit=Force]
	double dPhi;		//[Unit=None]
	double dVc;			//[Unit=Force]
	double dVs;			//[Unit=Force]	
	double dVp;			//[Unit=Force]
	double dPhiVn;		//[Unit=Force]
	double dAv_s;     //[Unit=LENGTH]
	double dAv_s_min;	//[Unit=LENGTH]
	double dAv_s_req;	//[Unit=LENGTH]
	double ds;        // 전단철근간격 [Unit=LENGTH]
	double ds_max;    // 전단철근간격 max [Unit=LENGTH]
	double dVu_pVn_Rat; // Vu/pVn, [Unit=None]
	BOOL bOK;

	// Eurocode2-2:05	
	double dV_Rd_max; 

	//KSCE-LSD12
	double dRho_w;      // [None]
	double dRho_w_min;  // [None]
	double ds_l_max;    //[L]

	UINT strM1; 
	double N1;//kN
	double V1;//kN
	double M1;//kNm
	double dVcr1;//kN

	UINT strM2; 
	double N2;//kN
	double V2;//kN
	double M2;//kNm
	double dVcr2;//kN

	UINT strM3; 
	double N3;//kN
	double V3;//kN
	double M3;//kNm
	double dVcr3;//kN

	UINT strM4; 
	double N4;//kN
	double V4;//kN
	double M4;//kNm
	double dVcr4;//kN

	double dfcp     ; //MPa	//Compressive stress at centroid due to prestress
	double db		; //mm duct diameter
	double dVp_Irs		; //kN is the vertical component of prestress
	double dd		; //mm  Distance between extreme compression fiber and centroid of tendons
	double dfpt		; //Mpa Stress at extreme tensile fiber due to prestress
	double dy		; //mm Distance between tensile fiber and CG of concrete section

	double dWebPlanThk;//Shear Design - R/F Chk mm2/mm

	void Initialize()
	{
		bCHK = 0;
		LcomK = 0;
		iMax=iKind=iLSTerm=0;
		dPu = dVu = dMu = dVn = 0.0;
		dft=dfb=0.0;
		dPhi = dPhiVn = 0.0;
		dVc = dVs = dVp = 0.0;
		dAv_s = dAv_s_min = dAv_s_req = 0.0;
		ds = ds_max = 0.0;
		bOK = 0;
		dV_Rd_max=0.0;
		dRho_w = dRho_w_min = ds_l_max =0.0;
		strM1=strM2=strM3=strM4=0;
		N1=N2=N3=N4=0.;
		V1=V2=V3=V4=0.;
		M1=M2=M3=M4=0.;
		dfcp=db=dVp=dd=dfpt=dy	=0.;
		dWebPlanThk = 0.;
	}
	T_CUMS_PSC_BASE_IRS() { Initialize(); }	
};

struct T_CUMS_PSC_IRS	// ElemK.
{
	T_CUMS_PSC_BASE_IRS CumsBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

	void Initialize()
	{
		for(int i=0; i<4; i++)
			CumsBase[i].Initialize();
	}
	T_CUMS_PSC_IRS() { Initialize(); }	
};

struct T_CUMS_PSC_UNIT_IRS
{
	T_CUMS_PSC_UNIT_IRS()
	{
		dPu   = D_UNITSYS_BASE_FORCE;		
		dVu		= D_UNITSYS_BASE_FORCE;		
		dMu		= D_UNITSYS_BASE_MOMENT;	
		dft   = D_UNITSYS_BASE_STRESS;
		dfb   = D_UNITSYS_BASE_STRESS;
		dVn		= D_UNITSYS_BASE_FORCE;		
		dVc		= D_UNITSYS_BASE_FORCE;		
		dVs		= D_UNITSYS_BASE_FORCE;		
		dVp		= D_UNITSYS_BASE_FORCE;		
		dPhiVn	= D_UNITSYS_BASE_FORCE;	
		dAv_s	= D_UNITSYS_BASE_LENGTH;	
		dAv_s_min = D_UNITSYS_BASE_LENGTH;
		dAv_s_req = D_UNITSYS_BASE_LENGTH;
		ds    = D_UNITSYS_BASE_LENGTH;
		ds_max = D_UNITSYS_BASE_LENGTH;		
		dV_Rd_max = D_UNITSYS_BASE_FORCE;
		ds_l_max = D_UNITSYS_BASE_LENGTH;
		
		N1=N2=N3=N4= D_UNITSYS_BASE_FORCE;;
		V1=V2=V3=V4= D_UNITSYS_BASE_FORCE;;
		M1=M2=M3=M4=D_UNITSYS_BASE_MOMENT;
		dfcp = dfpt = D_UNITSYS_BASE_STRESS;
		dVp_Irs = D_UNITSYS_BASE_FORCE;
		db=dd=dy = D_UNITSYS_BASE_LENGTH;
		dWebPlanThk = D_UNITSYS_BASE_UNIT_AREA;
	}

	int dPu;
	int dVu;
	int dMu;		
	int dft;
	int dfb;
	int dVn;		
	int dVc;		
	int dVs;		
	int dVp;		
	int dPhiVn;	
	int dAv_s;	
	int dAv_s_min;	
	int dAv_s_req;	
	int ds;
	int ds_max;
	int dV_Rd_max;
	int ds_l_max;

	int N1;int N2;int N3;int N4;	
	int V1;int V2;int V3;int V4;
	int M1;int M2;int M3;int M4;

	int dfcp ;   
	int db	;	
	int dVp_Irs	;	
	int dd	;	
	int dfpt;		
	int dy	;	
	int dWebPlanThk;

};

//==================================================================================
// Torsion design
//==================================================================================
struct T_CRMT_PSC_BASE_IRS_875
{
	BOOL bCHK;				//[Unit=None]
	UINT LcomK;				//[Unit=None]
	int iMax;
	int iKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.
	int iLSTerm;      // Long, Short Term 구별
	double dPu;       //[Unit=Force]
	double dVu;				//[Unit=Force]
	double dMu;				//[Unit=Moment]
	double dTu;				//[Unit=Moment]
	double dft;     //[Unit=Stress]
	double dfb;     //[Unit=Stress]
	double dVn;				//[Unit=Force]
	double dTn;				//[Unit=Moment]
	double dPhi;			//[Unit=None]
	double dPhi_t;		//[Unit=None]
	double dPhiVn;		//[Unit=Force]
	double dPhi_tTn;	//[Unit=Moment]
	double dAl;       //[Unit=Area]  
	double dAl_min;		//[Unit=Area]  
	double dAt;       //[Unit=Area]  
	double dAt_req;   //[Unit=Area]  
	double dTcr;      //[Unit=Moment]

	BOOL bOK;

	double dV_Rd_max; //[Unit=Force]
	double dComb_rat; //[Unit=None]

	//KSCE-LSD12
	double dAl_req;
	double ds_t;
	double dst_max_rep;
	double drho_st;
	double drho_st_min;

	void Initialize()
	{
		bCHK = 0;
		LcomK = 0;
		iMax=iKind=iLSTerm=0;
		dPu = dVu = dMu = dVn = dTn = dTu = 0.0;
		dft=dfb=0.0;
		dPhi = dPhiVn = dPhi_t = dPhi_tTn = 0.0;
		dAl = dAl_min = 0.0;		
		dAt = dAt_req = 0.0;
		dTcr = 0.0;		
		bOK = FALSE;
		dV_Rd_max = 0.0;
		dComb_rat = 0.0;
		dAl_req = ds_t = dst_max_rep = 0.0;
		drho_st = drho_st_min = 0.0;
	}
	T_CRMT_PSC_BASE_IRS_875() { Initialize(); }	
};

struct T_CRMT_PSC_BASE_IRS
{
	BOOL bCHK;				//[Unit=None]
	UINT LcomK;				//[Unit=None]
	int iMax;
	int iKind;        // 기준별 제시하는 하중조합 타입을 구별하기 위해.
	int iLSTerm;      // Long, Short Term 구별
	double dPu;       //[Unit=Force]
	double dVu;				//[Unit=Force]
	double dMu;				//[Unit=Moment]
	double dTu;				//[Unit=Moment]
	double dft;     //[Unit=Stress]
	double dfb;     //[Unit=Stress]
	double dVn;				//[Unit=Force]
	double dTn;				//[Unit=Moment]
	double dPhi;			//[Unit=None]
	double dPhi_t;		//[Unit=None]
	double dPhiVn;		//[Unit=Force]
	double dPhi_tTn;	//[Unit=Moment]
	double dAl;       //[Unit=Area]  
	double dAl_min;		//[Unit=Area]  
	double dAt;       //[Unit=Area]  
	double dAt_req;   //[Unit=Area]  
	double dTcr;      //[Unit=Moment]
	BOOL bOK;
	double dV_Rd_max; //[Unit=Force]
	double dComb_rat; //[Unit=None]
	//KSCE-LSD12
	double dAl_req;
	double ds_t;
	double dst_max_rep;
	double drho_st;
	double drho_st_min;
	//Eurocode2 PSC Box
	BOOL bPSC_BOX;
	double dT_Rd_c;
	double dV_Rd_c;
	double dComb_Rd_c_rat;
	BOOL bComb_Rd_c_rat_check;
	double dT_Rd_s;
	double dV_Rd_s;
	double dT_Rd_s_rat;
	double dV_Rd_s_rat;
	//IRS Special Cases
	double dVed		;//kN
	double dTed		;//kNm
	double dvt		;//MPa
	double dhw0		;//mm
	double dA0		;//mm
	double dvtmin	;//MPa
	double dvts		;//MPa
	double dSumVtsVt;//Mpa
	double dvtmax	;//Mpa

	//Calculate Torsional R/F
	double dAstreq	;//mm2
	double dAstTor	;//mm2
	double dSv		;//mm
	double dfyv		;//MPa
	double dAslreq	;//mm2
	double dAsltor	;//mm2
	double dSl		;//mm
	double dfyl		;//MPa
	double dfpu		;//MPa
	double dfpe		;//MPa

	//Longitudinal Shear
	double dVla1	;//N/mm
	double dVla2	;//N/mm
	double dLs		;//N/mm
	double dAe		;//N/mm



	void Initialize()
	{
		bCHK = 0;
		LcomK = 0;
		iMax=iKind=iLSTerm=0;
		dPu = dVu = dMu = dVn = dTn = dTu = 0.0;
		dft=dfb=0.0;
		dPhi = dPhiVn = dPhi_t = dPhi_tTn = 0.0;
		dAl = dAl_min = 0.0;		
		dAt = dAt_req = 0.0;
		dTcr = 0.0;		
		bOK = FALSE;
		dV_Rd_max = 0.0;
		dComb_rat = 0.0;
		dAl_req = ds_t = dst_max_rep = 0.0;
		drho_st = drho_st_min = 0.0;
		bPSC_BOX = FALSE;
		dT_Rd_c = dV_Rd_c = dComb_Rd_c_rat = 0.0;
		bComb_Rd_c_rat_check = FALSE;
		dT_Rd_s = dV_Rd_s = dT_Rd_s_rat = dV_Rd_s_rat = 0.0;
		dVed		= 0.0;
		dTed		= 0.0;
		dvt			= 0.0;
		dhw0		= 0.0;
		dA0			= 0.0;
		dvtmin		= 0.0;
		dvts		= 0.0;
		dSumVtsVt	= 0.0;
		dvtmax		= 0.0;
		dAstreq		= 0.0;
		dAstTor		= 0.0;
		dSv			= 0.0;
		dfyv		= 0.0;
		dAslreq		= 0.0;
		dAsltor		= 0.0;
		dSl			= 0.0;
		dfyl		= 0.0;
		dfpu		= 0.0;
		dfpe		= 0.0;
		dVla1		= 0.0;
		dVla2		= 0.0;
		dLs			= 0.0;
		dAe			= 0.0;
	}

	void Convert875(const T_CRMT_PSC_BASE_IRS_875 &rData)
	{
		bCHK                 = rData.bCHK       ;
		LcomK                = rData.LcomK      ;
		iMax                 = rData.iMax       ;
		iKind                = rData.iKind      ;
		iLSTerm              = rData.iLSTerm    ;
		dPu                  = rData.dPu        ;
		dVu                  = rData.dVu        ;
		dMu                  = rData.dMu        ;
		dTu                  = rData.dTu        ;
		dft                  = rData.dft        ;
		dfb                  = rData.dfb        ;
		dVn                  = rData.dVn        ;
		dTn                  = rData.dTn        ;
		dPhi                 = rData.dPhi       ;
		dPhi_t               = rData.dPhi_t     ;
		dPhiVn               = rData.dPhiVn     ;
		dPhi_tTn             = rData.dPhi_tTn   ;
		dAl                  = rData.dAl        ;
		dAl_min              = rData.dAl_min    ;
		dAt                  = rData.dAt        ;
		dAt_req              = rData.dAt_req    ;
		dTcr                 = rData.dTcr       ;
		bOK                  = rData.bOK        ;
		dV_Rd_max            = rData.dV_Rd_max  ;
		dComb_rat            = rData.dComb_rat  ;
		dAl_req              = rData.dAl_req    ;
		ds_t                 = rData.ds_t       ;
		dst_max_rep          = rData.dst_max_rep;
		drho_st              = rData.drho_st    ;
		drho_st_min          = rData.drho_st_min;
		bPSC_BOX             = FALSE;
		dT_Rd_c              = 0.0;
		dV_Rd_c              = 0.0;
		dComb_Rd_c_rat       = 0.0;
		bComb_Rd_c_rat_check = FALSE;
		dT_Rd_s              = 0.0;
		dV_Rd_s              = 0.0;
		dT_Rd_s_rat          = 0.0;
		dV_Rd_s_rat          = 0.0;
	}
	T_CRMT_PSC_BASE_IRS() { Initialize(); }	
};

struct T_CRMT_PSC_IRS_875	// ElemK.
{
	T_CRMT_PSC_BASE_IRS_875 CrmtBase[6];	// 0=I-V-MAX, 1=I-V-MIN, 2=I-T-MAX, 3=J-V-MAX, 4=J-V-MIN, 5=J-T-MAX.

	void Initialize()
	{
		for(int i=0; i<6; i++)
			CrmtBase[i].Initialize();
	}
	T_CRMT_PSC_IRS_875() { Initialize(); }	
};

struct T_CRMT_PSC_IRS	// ElemK.
{
	T_CRMT_PSC_BASE_IRS CrmtBase[6];	// 0=I-V-MAX, 1=I-V-MIN, 2=I-T-MAX, 3=J-V-MAX, 4=J-V-MIN, 5=J-T-MAX.

	void Initialize()
	{
		for(int i=0; i<6; i++)
			CrmtBase[i].Initialize();
	}
	void Convert875(const T_CRMT_PSC_IRS_875& rData)
	{
		for(int i=0; i<6; i++)
		{
			CrmtBase[i].Convert875(rData.CrmtBase[i]);
		}
	}
	T_CRMT_PSC_IRS() { Initialize(); }	
};

struct T_CRMT_PSC_UNIT_IRS
{
	T_CRMT_PSC_UNIT_IRS()
	{
		dPu       = D_UNITSYS_BASE_FORCE;		
		dVu       = D_UNITSYS_BASE_FORCE;		
		dMu	      = D_UNITSYS_BASE_MOMENT;		
		dft       = D_UNITSYS_BASE_STRESS;
		dfb       = D_UNITSYS_BASE_STRESS;
		dTu       = D_UNITSYS_BASE_MOMENT;		
		dVn	      = D_UNITSYS_BASE_FORCE;		
		dTn	      = D_UNITSYS_BASE_MOMENT;		
		dPhi      = D_UNITSYS_NONE;	
		dPhi_t    = D_UNITSYS_NONE;	
		dPhiVn    = D_UNITSYS_BASE_FORCE;	
		dPhi_tTn  = D_UNITSYS_BASE_MOMENT;
		dAl       = D_UNITSYS_BASE_AREA;
		dAl_min   = D_UNITSYS_BASE_AREA;
		dAt       = D_UNITSYS_BASE_AREA;
		dAt_req   = D_UNITSYS_BASE_AREA;
		dTcr      = D_UNITSYS_BASE_MOMENT;
		dV_Rd_max = D_UNITSYS_BASE_FORCE;
		dAl_req   = D_UNITSYS_BASE_AREA;
		ds_t      = D_UNITSYS_BASE_LENGTH;
		dst_max_rep = D_UNITSYS_BASE_LENGTH;
		dT_Rd_c = D_UNITSYS_BASE_MOMENT;
		dV_Rd_c = D_UNITSYS_BASE_FORCE;
		dT_Rd_s = D_UNITSYS_BASE_MOMENT;
		dV_Rd_s = D_UNITSYS_BASE_FORCE;

		dVed		= D_UNITSYS_BASE_FORCE;
		dTed		= D_UNITSYS_BASE_MOMENT;
		dvt			= D_UNITSYS_BASE_STRESS;
		dhw0		= D_UNITSYS_BASE_LENGTH;
		dA0			= D_UNITSYS_BASE_AREA;
		dvtmin		= D_UNITSYS_BASE_STRESS;
		dvts		= D_UNITSYS_BASE_STRESS;
		dSumVtsVt	= D_UNITSYS_BASE_STRESS;
		dvtmax		= D_UNITSYS_BASE_STRESS;
		dAstreq		= D_UNITSYS_BASE_AREA;
		dAstTor		= D_UNITSYS_BASE_AREA;
		dSv			= D_UNITSYS_BASE_LENGTH;
		dfyv		= D_UNITSYS_BASE_STRESS;
		dAslreq		= D_UNITSYS_BASE_AREA;
		dAsltor		= D_UNITSYS_BASE_AREA;
		dSl			= D_UNITSYS_BASE_LENGTH;
		dfyl		= D_UNITSYS_BASE_STRESS;
		dfpu		= D_UNITSYS_BASE_STRESS;
		dfpe		= D_UNITSYS_BASE_STRESS;
		dVla1		= D_UNITSYS_BASE_FORCE/D_UNITSYS_BASE_LENGTH;
		dVla2		= D_UNITSYS_BASE_FORCE/D_UNITSYS_BASE_LENGTH;
		dLs			= D_UNITSYS_BASE_LENGTH;
		dAe			= D_UNITSYS_BASE_AREA;
	}

	int dPu;
	int	dVu;		
	int dMu;		
	int dTu;		
	int dft;
	int dfb;
	int dVn;		
	int dTn;		
	int dPhi;	
	int dPhi_t;	
	int dPhiVn;	
	int dPhi_tTn;
	int dAl;
	int dAl_min;
	int dAt;
	int dAt_req;
	int dTcr;
	int dV_Rd_max;
	int dAl_req;
	int ds_t;
	int dst_max_rep;
	int dT_Rd_c;
	int dV_Rd_c;
	int dT_Rd_s;
	int dV_Rd_s;

	int dVed		;//kN
	int dTed		;//kNm
	int dvt		;//MPa
	int dhw0		;//mm
	int dA0		;//mm
	int dvtmin	;//MPa
	int dvts		;//MPa
	int dSumVtsVt;//Mpa
	int dvtmax	;//Mpa

	//Calculate Torsional R/F
	int dAstreq	;//mm2
	int dAstTor	;//mm2
	int dSv		;//mm
	int dfyv		;//MPa
	int dAslreq	;//mm2
	int dAsltor	;//mm2
	int dSl		;//mm
	int dfyl		;//MPa
	int dfpu		;//MPa
	int dfpe		;//MPa

	//Longitudinal Shear
	int dVla1	;//N/mm
	int dVla2	;//N/mm
	int dLs		;//N/mm
	int dAe		;//N/mm

};

//==================================================================================
// 9. PSC Composite : Fatigue
//==================================================================================
struct T_FATG_PSC_BASE_IRS
{
	BOOL bCHK;	        	// 설계결과유무
	double dSigLcom_max; // [Unit=Stress] 
	double dSigLcom_min; // [Unit=Stress] 
	double dVuLcom_max ; // [Unit=Force] 
	double dVuLcom_min ; // [Unit=Force] 
	double dVc;

	UINT LcomK1;  
	double dStressRatio;	 // σc,max/fcd,fat	 [Unit=None]
	double dStressLimit01; // [Unit=None]           
	double dStressLimit02; // [Unit=None]           

	UINT LcomK2; 
	double dShearRatio01; // σc,max/νfcd,fat [Unit=None]
	double dShearRatio02; //  VEd,min/VEd,max  [Unit=None]
	double dStressLimit03;//                   [Unit=None]

	double dRatio;
	BOOL bOK;

	void Initialize()
	{
		bCHK = 0;
		dSigLcom_max =0.0;
		dSigLcom_min =0.0;
		dVuLcom_max  =0.0;
		dVuLcom_min  =0.0;
		dVc = 0.0;

		LcomK1 = 0;
		dStressRatio  =0.0;
		dStressLimit01=0.0;
		dStressLimit02=0.0;

		LcomK2 = 0; 
		dShearRatio01 =0.0;
		dShearRatio02 =0.0;
		dStressLimit03=0.0;

		dRatio = 0.0;
		bOK = TRUE;

	}
	T_FATG_PSC_BASE_IRS() { Initialize(); }	
};

struct T_FATG_PSC_IRS// ElemK.
{
	T_FATG_PSC_BASE_IRS FatgBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

	void Initialize()
	{
		for(int i=0; i<4; i++)
			FatgBase[i].Initialize();
	}
	T_FATG_PSC_IRS() { Initialize(); }		
};

struct T_FATG_PSC_UNIT_IRS
{
	T_FATG_PSC_UNIT_IRS()
	{
		dSigLcom_max = D_UNITSYS_BASE_STRESS;
		dSigLcom_min = D_UNITSYS_BASE_STRESS;
		dVuLcom_max  = D_UNITSYS_BASE_FORCE;
		dVuLcom_min  = D_UNITSYS_BASE_FORCE;
		dVc          = D_UNITSYS_BASE_FORCE;
	}

	int	dSigLcom_max;
	int	dSigLcom_min;
	int	dVuLcom_max ;
	int	dVuLcom_min ;
	int dVc;

};

struct T_FYPC_IRS	// TdnaK.
{
	BOOL bCHK;	
	int nLoadType;  // 0=Pre-tension, 1=Post-tension 

	double dFDL1;	// Pre-Tensioning: Stress prior to transfer. Jacking stress. [Unit=Stress]
	// Post-Tensioning: Stress prior to seating. Jacking stress. [Unit=Stress]

	double dFDL2;	// Pre-Tensioning: Not used. [Unit=Stress]		
	// Post-Tensioning: Stress at anchorage and couplers immediately after anchor set. [Unit=Stress]

	double dFDL3;	// Pre-Tensioning: Not used [Unit=Stress]
	// Post-Tensioning: Away from Anchorage and couplers immediately after anchor set. [Unit=stress]

	double dFLL1;	// Stress at service limit after all losses. [Unit=Stress]

	double dFLL2;	// Stress at strength or extreme event limit state. [Unit=Stress]

	double dAFDL1;	// Pre-Tensioning: Allowable stress prior to transfer. Allowable jacking stress. [Unit=Stress]
	// Post-Tensioning: Allowable stress prior to seating. Allowable jacking stress. [Unit=Stress]

	double dAFDL2;	// Pre-Tensioning: Not used. [Unit=Stress]		
	// Post-Tensioning: Allowable stress at anchorage and couplers immediately after anchor set. [Unit=Stress]

	double dAFDL3;	// Pre-Tensioning: Not used. [Unit=Stress]		
	// Post-Tensioning: Allowable stress at anchorage and couplers immediately after anchor set. [Unit=Stress]

	double dAFLL1; 	// Stress at service limit after all losses. [Unit=Stress]

	double dAFLL2;  // Stress at strength or extreme event limit state. [Unit=Stress]

	BOOL bOK;

	void Initialize()
	{
		bCHK = FALSE;
		nLoadType = 0; 
		dFDL1 = dFDL2 = dFDL3 = dFLL1 = dFLL2 = 0.0;
		dAFDL1 = dAFDL2 = dAFDL3 = dAFLL1 = dAFLL2 = 0.0;
		bOK = TRUE;
	}
	T_FYPC_IRS() { Initialize(); }
};

struct T_FYPC_IRS_UNIT
{
	T_FYPC_IRS_UNIT()
	{
		dFDL1	= D_UNITSYS_BASE_STRESS;
		dFDL2	= D_UNITSYS_BASE_STRESS;
		dFDL3	= D_UNITSYS_BASE_STRESS;
		dFLL1	= D_UNITSYS_BASE_STRESS;
		dFLL2	= D_UNITSYS_BASE_STRESS;
		dAFDL1	= D_UNITSYS_BASE_STRESS;
		dAFDL2	= D_UNITSYS_BASE_STRESS;
		dAFDL3	= D_UNITSYS_BASE_STRESS;
		dAFLL1	= D_UNITSYS_BASE_STRESS;
		dAFLL2	= D_UNITSYS_BASE_STRESS;
	}

	int dFDL1;
	int dFDL2;
	int dFDL3;
	int dFLL1;
	int dFLL2;
	int dAFDL1;
	int dAFDL2;
	int dAFDL3;
	int dAFLL1;
	int dAFLL2;
};

//End IRS




// BS code BD21/11 Assessment Reserve Factor
struct T_ASCD_PSC_BS_BASE // Assessment Common Data BS
{
    // Common Data
    BOOL     bChk;
    T_ELEM_K ElemK;
    UINT     unPosi;
    UINT     RCaseK;        // Rating Case Key
    UINT     unConcurrent;  // Fx_max .... Mz_min(1~12)               [Unit=None] 
    BOOL     bStdMv;
    BOOL     bSpcMv;
    double   dFc;           // Condition Factor                       [Unit=None]  

    double dFxx,dFyy,dFzz;
    double dMux,dMuy,dMuz;
    BOOL     bComposite  ;
    double   dStr[7]     ; // FT, FB, FTL, FBL, FTR, FBR, FMAX of girder
    double   dSlStr[7]   ; // FT, FB, FTL, FBL, FTR, FBR, FMAX of composite slab

    void Initialize()
    {
        bChk         = FALSE;
        ElemK        = 0;
        unPosi       = 0;
        RCaseK       = 0;
        unConcurrent = 1;
        bStdMv       = FALSE;
        bSpcMv       = FALSE;
        dFc          = 0.0;

        dFxx = dFyy = dFzz = 0.0;
        dMux = dMuy = dMuz = 0.0;
        bComposite   = FALSE;
        memset(dStr, 0, sizeof(dStr));
        memset(dSlStr, 0, sizeof(dSlStr));
    }
    void Set_Forces(const double& dFx, const double& dFy, const double& dFz, const double& dMx, const double& dMy, const double& dMz)
    {
        dFxx = dFx;
        dFyy = dFy;
        dFzz = dFz;
        dMux = dMx;
        dMuy = dMy;
        dMuz = dMz;
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
};

struct T_ASCD_PSC_BS_UNIT
{
    T_ASCD_PSC_BS_UNIT()
    {
        dFxx = D_UNITSYS_BASE_FORCE;
        dFyy = D_UNITSYS_BASE_FORCE;
        dFzz = D_UNITSYS_BASE_FORCE;
        dMux = D_UNITSYS_BASE_MOMENT;
        dMuy = D_UNITSYS_BASE_MOMENT;
        dMuz = D_UNITSYS_BASE_MOMENT;
        dStr   = D_UNITSYS_BASE_STRESS;
        dSlStr = D_UNITSYS_BASE_STRESS;
    }  
    int dFxx;
    int dFyy;
    int dFzz;
    int dMux;
    int dMuy;
    int dMuz;
    int dStr;
    int dSlStr;
};

struct T_ASFC_PSC_BS_BASE // Assessment Flexural Capacity BS
{
    // Common Data
    T_ASCD_PSC_BS_BASE ComD;

    // Flexural Capacity
    double   dR      ;      // Calculated Resistance R*
    double   dRA     ;      // Assessment Resistance RA*=Fc x R*
    double   dSA     ;      // Sa* - Assessment Load Effect total Sa* = Sd* + Sha* + Ssv*
    double   dSd     ;      // Sd* - Assessment Load Effect; Dead and superimposed dead loads
    double   dSst    ;      // Sha* - Assessment Load Effect; Standard Vehicle
    double   dS      ;      // S* - Assessment Load Effect; Special Vehicle
    double   dA      ;      // Reserve Factor for only Standard Vehicle
    double   dPsi    ;      // Ψ - Reserve Factor with Associated type HA Loading for Special Vehicle; BD 86/11
    double   dPsi_ast;      // Ψ* - Reserve Factor without Associated type HA Loading for Special Vehicle; BD 86/11
    double	 dRF     ;      // Maximan Reserve Factor for Assessment; max(dA, dPsi, dPsi_ast)

    void Initialize()
    {
        ComD.Initialize();
        dR           = 0.0;
        dRA          = 0.0;
        dSA          = 0.0;
        dSd          = 0.0;
        dSst         = 0.0;
        dS           = 0.0;
        dA           = 0.0;
        dPsi         = 0.0;
        dPsi_ast     = 0.0;
        dRF          = 0.0;
    }
};

struct T_ASFC_PSC_BS_UNIT
{
    T_ASFC_PSC_BS_UNIT()
    {
        dR   = D_UNITSYS_BASE_MOMENT;
        dRA  = D_UNITSYS_BASE_MOMENT;
        dSA  = D_UNITSYS_BASE_MOMENT;
        dSd  = D_UNITSYS_BASE_MOMENT;
        dSst = D_UNITSYS_BASE_MOMENT;
        dS   = D_UNITSYS_BASE_MOMENT;

    }  
    int dR  ;
    int dRA ;
    int dSA ;
    int dSd ;
    int dSst;
    int dS  ;
};

struct T_ASFC_PSC_BS_CASE
{
    // 0=I-Posi, 1=J-Posi
    T_ASFC_PSC_BS_BASE AsfcBase[2];

    void Initialize()
    {
        for(int i=0; i<2; ++i)  AsfcBase[i].Initialize();
    }
};

struct T_ASFC_PSC_BS_D
{
    // 0=I-Posi, 1=I-Nega, 2=J-Posi, 3=J-Nega.
    T_ASFC_PSC_BS_BASE AsfcB[4];

    void Initialize()
    {
        for(int i=0; i<4; ++i) AsfcB[i].Initialize();
    }
    T_ASFC_PSC_BS_D() { Initialize(); }
};

struct T_ASSC_PSC_BS_BASE_891 // Assessment Shear Capacity BS
{
    // Common Data
    T_ASCD_PSC_BS_BASE ComD;

    // Flexural Capacity
    double   dR;      // Calculated Resistance R*
    double   dRA;      // Assessment Resistance RA*=Fc x R*
    double   dSA;      // Sa* - Assessment Load Effect total Sa* = Sd* + Sha* + Ssv*
    double   dSd;      // Sd* - Assessment Load Effect; Dead and superimposed dead loads
    double   dSst;      // Sha* - Assessment Load Effect; Standard Vehicle
    double   dS;      // S* - Assessment Load Effect; Special Vehicle
    double   dA;      // Reserve Factor for only Standard Vehicle
    double   dPsi;      // Ψ - Reserve Factor with Associated type HA Loading for Special Vehicle; BD 86/11
    double   dPsi_ast;      // Ψ* - Reserve Factor without Associated type HA Loading for Special Vehicle; BD 86/11
    double	 dRF;      // Maximan Reserve Factor for Assessment; max(dA, dPsi, dPsi_ast)

    void Initialize()
    {
        ComD.Initialize();
        dR           = 0.0;
        dRA          = 0.0;
        dSA          = 0.0;
        dSd          = 0.0;
        dSst         = 0.0;
        dS           = 0.0;
        dA           = 0.0;
        dPsi         = 0.0;
        dPsi_ast     = 0.0;
        dRF          = 1.0;
    }
};

struct T_ASSC_PSC_BS_BASE // Assessment Shear Capacity BS
{
    // Common Data
    T_ASCD_PSC_BS_BASE ComD;

    // Flexural Capacity
    double   dR      ;      // Calculated Resistance R*
    double   dRA     ;      // Assessment Resistance RA*=Fc x R*
    double   dSA     ;      // Sa* - Assessment Load Effect total Sa* = Sd* + Sha* + Ssv*
    double   dSd     ;      // Sd* - Assessment Load Effect; Dead and superimposed dead loads
    double   dSst    ;      // Sha* - Assessment Load Effect; Standard Vehicle
    double   dS      ;      // S* - Assessment Load Effect; Special Vehicle
    double   dA      ;      // Reserve Factor for only Standard Vehicle
    double   dPsi    ;      // Ψ - Reserve Factor with Associated type HA Loading for Special Vehicle; BD 86/11
    double   dPsi_ast;      // Ψ* - Reserve Factor without Associated type HA Loading for Special Vehicle; BD 86/11
    double	 dRF     ;      // Maximan Reserve Factor for Assessment; max(dA, dPsi, dPsi_ast)

    bool     bLS_chk    ;
    double   dLS_V1     ;   // Longitudinal shear force (force/length)
    double   dLS_V1_lim1;
    double   dLS_V1_lim2;
    double   dLS_s      ;   // Longitudinal shear reinforcement space
    double   dLS_s_lim1 ;
    double   dLS_s_lim2 ;

    void Initialize()
    {
        ComD.Initialize();
        dR           = 0.0;
        dRA          = 0.0;
        dSA          = 0.0;
        dSd          = 0.0;
        dSst         = 0.0;
        dS           = 0.0;
        dA           = 0.0;
        dPsi         = 0.0;
        dPsi_ast     = 0.0;
        dRF          = 1.0;

        bLS_chk     = false;
        dLS_V1      = 0.0;
        dLS_V1_lim1 = 0.0;
        dLS_V1_lim2 = 0.0;
        dLS_s       = 0.0;
        dLS_s_lim1  = 0.0;
        dLS_s_lim2  = 0.0;
    }

    void Convert891(const T_ASSC_PSC_BS_BASE_891 &rData)
    {
        Initialize();
        ComD     = rData.ComD;
        dR       = rData.dR;
        dRA      = rData.dRA;
        dSA      = rData.dSA;
        dSd      = rData.dSd;
        dSst     = rData.dSst;
        dS       = rData.dS;
        dA       = rData.dA;
        dPsi     = rData.dPsi;
        dPsi_ast = rData.dPsi_ast;
        dRF      = rData.dRF;
    }
};

struct T_ASSC_PSC_BS_UNIT
{
    T_ASSC_PSC_BS_UNIT()
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
    int dR  ;
    int dRA ;
    int dSA ;
    int dSd ;
    int dSst;
    int dS  ;

    int dLS_V1;
    int dLS_V1_lim1;
    int dLS_V1_lim2;
    int dLS_s;
    int dLS_s_lim1;
    int dLS_s_lim2;
};

struct T_ASSC_PSC_BS_CASE_891
{
    // 0=I-Posi, 1=J-Posi
    T_ASSC_PSC_BS_BASE_891 AsscBase[2];

    void Initialize()
    {
        for (int i=0; i<2; ++i)  AsscBase[i].Initialize();
    }
};

struct T_ASSC_PSC_BS_CASE
{
    // 0=I-Posi, 1=J-Posi
    T_ASSC_PSC_BS_BASE AsscBase[2];

    void Initialize()
    {
        for(int i=0; i<2; ++i)  AsscBase[i].Initialize();
    }

    void Convert891(const T_ASSC_PSC_BS_CASE_891 &rData)
    {
        AsscBase[0].Convert891(rData.AsscBase[0]);
        AsscBase[1].Convert891(rData.AsscBase[1]);
    }
};

struct T_ASSC_PSC_BS_D
{
    T_ASSC_PSC_BS_BASE AsscB[2];

    void Initialize()
    {
        for(int i=0; i<2; ++i) AsscB[i].Initialize();
    }
    T_ASSC_PSC_BS_D() { Initialize(); }
};

struct T_ASTC_PSC_BS_BASE_891 // Assessment Torsion Capacity BS
{
    // Common Data
    T_ASCD_PSC_BS_BASE ComD;

    // Torsion Stress
    double   dv;      // shear stress
    double   dvt;      // torsional shear stress
    double   dvtu1;      // ultimate torsional shear stress, (v+vt) <= vtu1
    double   dy1;      // larger centre-line dimension of a link
    double   dvtu2;      // y1>550mm, ultimate torsional shear stress, vt <= vt*y1/550
    double	 dRF;      // Reserve Factor for Assessment
    BOOL     bBox;      //

    void Initialize()
    {
        ComD.Initialize();
        dv = 0.0;
        dv    = 0.0;
        dvt   = 0.0;
        dvtu1 = 0.0;
        dy1   = 0.0;
        dvtu2 = 0.0;
        dRF   = 0.0;
        bBox  = FALSE;
    }
};

struct T_ASTC_PSC_BS_BASE // Assessment Torsion Capacity BS
{
    // Common Data
    T_ASCD_PSC_BS_BASE ComD;

    // Torsion Stress
    double   dv    ;      // shear stress
    double   dvt   ;      // torsional shear stress
    double   dvtu1 ;      // ultimate torsional shear stress, (v+vt) <= vtu1
    double   dy1   ;      // larger centre-line dimension of a link
    double   dvtu2 ;      // y1>550mm, ultimate torsional shear stress, vt <= vt*y1/550
    double	 dRF   ;      // Reserve Factor for Assessment
    BOOL     bBox  ;      //
    double   dT    ;      // Torque due to Ultimate Load
    double   dTu   ;      // Ulimate Torsion
    double   dvtmin;      // vtmin

    void Initialize()
    {
        ComD.Initialize();
        dv    = 0.0;
        dvt   = 0.0;
        dvtu1 = 0.0;
        dy1   = 0.0;
        dvtu2 = 0.0;
        dRF   = 0.0;
        bBox  = FALSE;
        dT    = 0.0;
        dTu   = 0.0;
        dvtmin= 0.0;
    }

    void Convert891(const T_ASTC_PSC_BS_BASE_891 &rData)
    {
        Initialize();
        ComD  = rData.ComD;
        dv    = rData.dv;
        dvt   = rData.dvt;
        dvtu1 = rData.dvtu1;
        dy1   = rData.dy1;
        dvtu2 = rData.dvtu2;
        dRF   = rData.dRF;
        bBox  = rData.bBox;
    }
};

struct T_ASTC_PSC_BS_UNIT
{
    T_ASTC_PSC_BS_UNIT()
    {
        dv    = D_UNITSYS_BASE_STRESS;
        dvt   = D_UNITSYS_BASE_STRESS;
        dvtu1 = D_UNITSYS_BASE_STRESS;
        dy1   = D_UNITSYS_BASE_LENGTH;
        dvtu2 = D_UNITSYS_BASE_STRESS;
        dT    = D_UNITSYS_BASE_MOMENT;
        dTu   = D_UNITSYS_BASE_MOMENT;
        dvtmin= D_UNITSYS_BASE_STRESS;
    }  
    int dv   ;
    int dvt  ;
    int dvtu1;
    int dy1  ;
    int dvtu2;
    int dT   ;
    int dTu  ;
    int dvtmin;
};

struct T_ASTC_PSC_BS_CASE_891
{
    // 0=I-Posi, 1=J-Posi
    T_ASTC_PSC_BS_BASE_891 AstcBase[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i ) AstcBase[i].Initialize();
    }
};

struct T_ASTC_PSC_BS_CASE
{
    // 0=I-Posi, 1=J-Posi
    T_ASTC_PSC_BS_BASE AstcBase[2];

    void Initialize()
    {
        for ( int i=0; i<2; ++i ) AstcBase[i].Initialize();
    }
    void Convert891(const T_ASTC_PSC_BS_CASE_891 &rData)
    {
        AstcBase[0].Convert891(rData.AstcBase[0]);
        AstcBase[1].Convert891(rData.AstcBase[1]);
    }

};

struct T_ASTC_PSC_BS_D
{
    T_ASTC_PSC_BS_BASE AstcB[2];

    void Initialize()
    {
        for(int i=0; i<2; ++i) AstcB[i].Initialize();
    }
    T_ASTC_PSC_BS_D() { Initialize(); }
};

enum BS_ASSESS_SLS_CLASS // BS Assessment SLS Stress check Class
{
    BS_ASSESS_SLS_CLASS1 = 1,
    BS_ASSESS_SLS_CLASS2,
    BS_ASSESS_SLS_CLASS3,
};

struct T_ASSL_PSC_BS_BASE // Assessment Service Limit State BS
{
    // Common Data
    T_ASCD_PSC_BS_BASE ComD; 

    // Service Limit State
    BS_ASSESS_SLS_CLASS enClass;      // Class2 or 3
    double   dSA         ;
    double   dsig_t      ;      // tensile stress class2
    double   dsig_c      ;      // compressive stress class2
    double   dsig_t_lim  ;      // tensile stress limit class2
    double   dsig_c_lim  ;      // compressive stress limit class2
    BOOL     bBonded     ;      // Is bonded tendon?
    
    double   dMg         ;
    double   dMq         ;
    double   dft         ;
    double   dfb         ;
    double   dw          ;      // crack width class3 for unbonded tendon 
    double   dw_lim      ;      // crack width limit class3 for unbonded tendon 
    double	 dRF         ;      // Reserve Factor for Assessment

    void Initialize()
    {
        ComD.Initialize();
        enClass      = BS_ASSESS_SLS_CLASS1;
        dSA          = 0.0;
        dsig_t       = 0.0;
        dsig_c       = 0.0;
        dsig_t_lim   = 0.0;
        dsig_c_lim   = 0.0;
        bBonded      = TRUE;
        dMg          = 0.0;
        dMq          = 0.0;
        dft          = 0.0;
        dfb          = 0.0;
        dw           = 0.0;
        dw_lim       = 0.0;
        dRF          = 0.0;        
    }
};

struct T_ASSL_PSC_BS_UNIT
{
    T_ASSL_PSC_BS_UNIT()
    {
        dSA        = D_UNITSYS_BASE_MOMENT;
        dsig_t     = D_UNITSYS_BASE_STRESS;
        dsig_c     = D_UNITSYS_BASE_STRESS;
        dsig_t_lim = D_UNITSYS_BASE_STRESS;
        dsig_c_lim = D_UNITSYS_BASE_STRESS;
        dMg        = D_UNITSYS_BASE_MOMENT;
        dMq        = D_UNITSYS_BASE_MOMENT;
        dft        = D_UNITSYS_BASE_STRESS;
        dfb        = D_UNITSYS_BASE_STRESS;
        dw         = D_UNITSYS_BASE_LENGTH;
        dw_lim     = D_UNITSYS_BASE_LENGTH;        
    }  
    int dSA       ;
    int dsig_t    ;
    int dsig_c    ;
    int dsig_t_lim;
    int dsig_c_lim;
    int dMg;
    int dMq;
    int dft;
    int dfb;
    int dw        ;
    int dw_lim    ;    
};

struct T_ASSL_PSC_BS_CASE
{
    // 0=I-Posi, 1=J-Posi
    T_ASSL_PSC_BS_BASE AsslBase[2];

    void Initialize()
    {
        for(int i=0; i<2; ++i)
            AsslBase[i].Initialize();
    }
};

struct T_ASSL_PSC_BS_D
{
    T_ASSL_PSC_BS_BASE AsslB[4];

    void Initialize()
    {
        for(int i=0; i<4; ++i) AsslB[i].Initialize();
    }
    T_ASSL_PSC_BS_D() { Initialize(); }
};

#pragma pack()

#endif




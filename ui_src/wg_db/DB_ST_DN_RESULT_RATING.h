#ifndef __DB_ST_DN_RESULT_RATING_H__
#define __DB_ST_DN_RESULT_RATING_H__

#define MIDAS_SDK

#include "DB_ST_DT_UNIT.h"
#include "DB_ST_DN_RESULT_CSG.H"
//#include "..\dgnengine\src\DgnCalcBase\DgnCalcBase_CSG_Struct.h"
//#include "..\dgnengine\src\CVL_DgnCalc_KR\DgnCalc_KR_STL_Struct.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(1)

enum
{
	ENUM_KS05 = 0,  // USD-05
	ENUM_KSCE_ASD,  // Rating KSCE-ASD 05 10 RAIL 04 11
};

enum RATING_BRIDGELEVEL
{
	ENUM_LEVEL_A = 0,
	ENUM_LEVEL_B,
	ENUM_LEVEL_C,
	ENUM_LEVEL_D,
	ENUM_LEVEL_E,
};

enum RATING_EVALUATION_TYPE
{
    ENUM_DESIGN = 0,
    ENUM_LEGAL,
    ENUM_PERMIT,
};

enum LOCATION_TENDON_REBAR
{
    LOCATION_NONE   = 0,
    LOCATION_REBAR,
    LOCATION_TENDON,
};

enum LOCATION_FATIGUE
{
    TOPFLANGE_TOP = 0,
    TOPFLANGE_BOT ,
    BOTFLANGE_TOP ,
    BOTFLANGE_BOT,
};

enum EVAL_LIVE_MODEL
{
    LRFD = 0,
    TRUCK_SURVEY,
};

enum FATI_LIFE_CALC_TYPE
{
    INF_FATI = 0,
    FIN_FATI,
};

enum EVALU_METHOD
{
    REFINED = 0,
    SIMPLIFIED,
};

enum FATI_LIFE_TYPE
{
    MINIMUM = 0,
    EVALUATION1,
    EVALUATION2,
    MEAN,
};

enum FATI_CATEGORY
{
    CATE_A = 0,
    CATE_B,
    CATE_B_P,
    CATE_C,
    CATE_C_P,
    CATE_D,
    CATE_E,
    CATE_E_P,
};

struct T_RATI_UNITLCOM
{
	BOOL    bUse;	          // 설계시 검토여부
	int     nType;	        // 하중종류 1:Strn  2:Serv
	int     nOrgLkey;       // 원래 하중 번호
	int     nDgnLkey;       // 설계 하중조합 key
	CString strName;	      // 하중조합이름
	CString strDesc;        // 하중조합을 구성하는 Load Case에 대한 설명
	double  dForce[6];      // 부재력 : 축방향력(Fx, Fy, Fz), 모멘트(Mx, My, Mz)

	void Initialize()
	{
		int i=0;
		bUse          = FALSE;
		nType         = 0;
		nOrgLkey      = 0;
		nDgnLkey      = 0;
		strName.Empty();	 
		strDesc.Empty();
		for(i=0 ; i<6 ; i++)
		{
			dForce[i]       = 0.0; 
		}
	}  
	void Get_Force(double& dFx, double& dFy, double& dFz, double& dMx, double& dMy, double& dMz)
	{
		dFx = dForce[0];
		dFy = dForce[1];
		dFz = dForce[2];
		dMx = dForce[3];
		dMy = dForce[4];
		dMz = dForce[5];
	}
	void Set_Force(double dFx, double dFy, double dFz, double dMx, double dMy, double dMz)
	{
		dForce[0] = dFx;
		dForce[1] = dFy;
		dForce[2] = dFz;
		dForce[3] = dMx;
		dForce[4] = dMy;
		dForce[5] = dMz;
	}

	T_RATI_UNITLCOM() {Initialize();}
	T_RATI_UNITLCOM(const T_RATI_UNITLCOM& rData) { *this = rData; }
	T_RATI_UNITLCOM& operator=(const T_RATI_UNITLCOM& rData)
	{
		Initialize();
		bUse          = rData.bUse     ;
		nType         = rData.nType    ;
		nOrgLkey      = rData.nOrgLkey ;
		nDgnLkey      = rData.nDgnLkey ;
		strName       = rData.strName  ;	
		strDesc       = rData.strDesc  ;
		for(int i=0 ; i<6 ; i++)
		{
			dForce[i]       = rData.dForce[i];  
		}
		return *this;
	}
};

struct T_RATI_UNITLCOM_CH
{
	BOOL    bUse;	          // 설계시 검토여부
	int     nType;	        // 하중종류 1:Strn  2:Serv
	int     nOrgLkey;       // 원래 하중 번호
	char    strName[16];	  // 하중조합이름
	char    strDesc[16];    // 하중조합을 구성하는 Load Case에 대한 설명
	double  dForce[6];      // 부재력 : 축방향력(Fx, Fy, Fz), 모멘트(Mx, My, Mz)

	void Initialize()
	{
		int i=0;
		bUse          = FALSE;
		nType         = 0;
		nOrgLkey      = 0;
		memset(strName, 0, sizeof(strName));
		memset(strDesc, 0, sizeof(strDesc));

		for(i=0 ; i<6 ; i++)
		{
			dForce[i]       = 0.0; 
		}
	}  
	T_RATI_UNITLCOM_CH() {Initialize();}
	T_RATI_UNITLCOM_CH(const T_RATI_UNITLCOM_CH& rData) { *this = rData; }
	T_RATI_UNITLCOM_CH& operator=(const T_RATI_UNITLCOM_CH& rData)
	{
		Initialize();
		int i=0;
		bUse          = rData.bUse     ;
		nType         = rData.nType    ;
		nOrgLkey      = rData.nOrgLkey ;
		for(int i = 0; i<16; ++i)
		{
			strName[i]      = rData.strName[i];	 
			strDesc[i]      = rData.strDesc[i];
		}
		for(i=0 ; i<6 ; i++)
		{
			dForce[i]       = rData.dForce[i];  
		}
		return *this;
	}

	void ConvertToString(T_RATI_UNITLCOM& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDesc, rData.strDesc, sizeof(strDesc));
		rData.bUse          = bUse     ;
		rData.nType         = nType    ;
		rData.nOrgLkey      = nOrgLkey ;

		for(int i=0 ; i<6 ; i++)
		{
			rData.dForce[i]   = dForce[i];  
		}
	}
	void ConvertToChar(T_RATI_UNITLCOM& rData)
	{
		CString stringName(rData.strName);
		CString stringDesc(rData.strDesc);
		ConvertStrChar(stringName, strName, sizeof(strName));
		ConvertStrChar(stringDesc, strDesc, sizeof(strDesc));
		bUse          = rData.bUse     ;
		nType         = rData.nType    ;
		nOrgLkey      = rData.nOrgLkey ;

		for(int i=0 ; i<6 ; i++)
		{
			dForce[i]       = rData.dForce[i];  
		}
	}
};


// Steel Rating KSCE-ASD 05 10 RAIL 04 11
struct T_RFKR_BASE
{
	BOOL bCheck;
	BOOL bCheck2;						  // 결과 Diagram을 그릴 때 Rating Factor와 기본내하력은 이값이 TRUE일때 그림.. (그외의 경우는 bCheck가 false이면 값이 없다고 봄)

	T_ELEM_K ElemK;
	UINT nSectPosi;           // 1,2,3,4 단면의..           [Unit=None]
	UINT nRCaseK;             // Rating Case Key            [Unit=None]
	UINT nDgnRCaseK;          // Dgn Rating Case Key        [Unit=None]
	UINT nConcurrent;         // Fx_max .... Mz_min(1~12)   [Unit=None]   

	double dEstiDisp;        // 실측처짐
	double dCalcDisp;        // 계산처짐
	double dRatioDisp;       // 계산/실측

	double dEstiEwip;        // 실측변형률
	double dCalcEwip;        // 계산변형률
	double dRatioEwip;       // 계산/실측
	
	double dCalcImpFactor;    // 계산충격계수
	double dEstiImpFactor;    // 실측충격계수  
	double dRatioImpFactor;   // 계산/실측
	
	int    nRFca;         // 0=Stress, 1=Force
	double dfa;
	double dfd;               
	double dfl;
	double dMc;
	double dMd;
	double dMl;
	double d1iFactor;
	
	double dRF;      //내하율
	double dPr;      //설계활하중
	double dP0;      //기본내하력
	double dKs;      //응답보정계수
	double dP;       //공용내하력
	int    nOK;      //판정 0 : -,  1 : OK 2 : NG

	double dfdl;     //fd+l
	double dMdl;
	double dRatiodfa;//fa/fd+l
	int    nRankType;//등급
	
	T_RATI_UNITLCOM Load;           // Dgn엔진 구조체 직접 사용하지 않기 위해 STL_KR_UNITLCOM -> T_RATI_UNITLCOM
	T_RATI_UNITLCOM Load2; // 단부 
	CString strLiveLoad;
	
	T_CSGD_ULMF  Ulmf;    //CSG_ULMF_D
	T_CSGD_COMF  ChkFor;  //CSG_FORCE_COMP
	

	void Initialize()
	{
		bCheck       = FALSE;
		bCheck2      = FALSE;
		ElemK        = 0;
		nSectPosi    = 0;
		nRCaseK      = 0;
		nDgnRCaseK   = 0;
		nConcurrent  = 0;
		dEstiDisp    = 0.0;
		dCalcDisp    = 0.0;
		dRatioDisp   = 0.0;
		dEstiEwip    = 0.0;
		dCalcEwip    = 0.0;  
		dRatioEwip   = 0.0; 
		dCalcImpFactor = 0.0;
		dEstiImpFactor = 0.0;
		dRatioImpFactor= 0.0;
		nRFca          = 0;
		dfa            = 0.0;      
		dfd            = 0.0;
		dfl            = 0.0;
		dMc            = 0.0;
		dMd            = 0.0;
		dMl            = 0.0;
		d1iFactor      = 0.0;
		dRF       = 0.0;      
		dPr       = 0.0;      
		dP0       = 0.0;      
		dKs       = 0.0;      
		dP        = 0.0;       
		nOK       = 0;      
		dfdl      = 0.0;
		dMdl      = 0.0;
		dRatiodfa = 0.0;
		nRankType = 0;
		
		Load.Initialize();
		Load2.Initialize();

		strLiveLoad.Empty();

		Ulmf.Initialize();
		ChkFor.Initialize();
	}
};

struct T_RFKR_UNIT
{
	T_RFKR_UNIT()
	{
		dEstiDisp = D_UNITSYS_BASE_LENGTH;
		dCalcDisp = D_UNITSYS_BASE_LENGTH;

		dfa  = D_UNITSYS_BASE_STRESS;
		dfd  = D_UNITSYS_BASE_STRESS;
		dfl  = D_UNITSYS_BASE_STRESS;
		dfdl = D_UNITSYS_BASE_STRESS;
		dMc  = D_UNITSYS_BASE_MOMENT;
		dMd  = D_UNITSYS_BASE_MOMENT;
		dMl  = D_UNITSYS_BASE_MOMENT;
		dMdl = D_UNITSYS_BASE_MOMENT;

		dForce[0]  = D_UNITSYS_BASE_FORCE;
		dForce[1]  = D_UNITSYS_BASE_FORCE;
		dForce[2]  = D_UNITSYS_BASE_FORCE;
		dForce[3]  = D_UNITSYS_BASE_MOMENT;
		dForce[4]  = D_UNITSYS_BASE_MOMENT;
		dForce[5]  = D_UNITSYS_BASE_MOMENT;
	}  
	int dEstiDisp;
	int dCalcDisp;
	int dfa;
	int dfd;
	int dfl;
	int dfdl;
	int dMc;
	int dMd;
	int dMl;
	int dMdl;
	int dForce[6];
	
};

struct RATING_FORCE
{
	T_RATI_UNITLCOM ForceDL;
	T_RATI_UNITLCOM ForceLL;
	T_RATI_UNITLCOM ForceG;

	void Initialize()
	{
		ForceDL.Initialize();
		ForceLL.Initialize();
		ForceG.Initialize();
	}
};

struct RATING_FORCE_CH
{
	T_RATI_UNITLCOM_CH ForceDL;
	T_RATI_UNITLCOM_CH ForceLL;
	T_RATI_UNITLCOM_CH ForceG;
	void ConvertToString(RATING_FORCE& rData)
	{
		ForceDL.ConvertToString(rData.ForceDL);
		ForceLL.ConvertToString(rData.ForceLL);
		ForceG.ConvertToString(rData.ForceG);
	}
	void ConvertToChar(RATING_FORCE& rData)
	{

		ForceDL.ConvertToChar(rData.ForceDL);
		ForceLL.ConvertToChar(rData.ForceLL);
		ForceG.ConvertToChar(rData.ForceG);
	}
};

struct T_RFKR_CASE
{
	T_RFKR_BASE RfkrBase[4]; // 0=I-상연, 1=I-하연, 2=J-상연, 3=J-하연  
	int nForceSize;
	int nResultCheck;  // 파일 쓸 때 유효 요소 표기.(10)
	CArray<RATING_FORCE, RATING_FORCE&> arForceRCaseI;
	CArray<RATING_FORCE, RATING_FORCE&> arForceRCaseJ;

	void Initialize()
	{
		for(int i=0; i<4; i++)
		{
			RfkrBase[i].Initialize();      
		}
		nForceSize = 0;
		nResultCheck = 0;
		arForceRCaseI.RemoveAll();
		arForceRCaseJ.RemoveAll();
	}
};

struct T_RFKR_D
{
	T_RFKR_BASE BaseI[4]; // 0=상연-max(인장), 1=상연-min(압축), 2=하연-max(인장), 3=하연-min(압축)
	T_RFKR_BASE BaseJ[4]; // 0=상연-max(인장), 1=상연-min(압축), 2=하연-max(인장), 3=하연-min(압축)
	CArray<RATING_FORCE, RATING_FORCE&> arForceI;
	CArray<RATING_FORCE, RATING_FORCE&> arForceJ;

	void Initialize()
	{
		for(int i=0; i<4; i++)
		{
			BaseI[i].Initialize();
			BaseJ[i].Initialize();
		}
		arForceI.RemoveAll();
		arForceJ.RemoveAll();
	}
};

//CH
struct T_RFKR_BASE_CH
{
	BOOL bCheck;
	BOOL bCheck2;
	T_ELEM_K ElemK;
	UINT nSectPosi;           // 1,2,3,4 단면의..           [Unit=None]
	UINT nRCaseK;             // Rating Case Key            [Unit=None]
	UINT nDgnRCaseK;          // Dgn Rating Case Key        [Unit=None]
	UINT nConcurrent;         // Fx_max .... Mz_min(1~12)   [Unit=None]   
	
	double dEstiDisp;        // 실측처짐
	double dCalcDisp;        // 계산처짐
	double dRatioDisp;       // 계산/실측
	
	double dEstiEwip;        // 실측변형률
	double dCalcEwip;        // 계산변형률
	double dRatioEwip;       // 계산/실측
	
	double dCalcImpFactor;    // 계산충격계수
	double dEstiImpFactor;    // 실측충격계수  
	double dRatioImpFactor;   // 계산/실측
	
	int    nRFca;         // 0=Stress, 1=Force
	double dfa;
	double dfd;               
	double dfl;               
	double dMc;
	double dMd;
	double dMl;
	double d1iFactor;
	
	double dRF;      //내하율
	double dPr;      //설계활하중
	double dP0;      //기본내하력
	double dKs;      //응답보정계수
	double dP;       //공용내하력
	int    nOK;      //판정 OK NG
	
	double dfdl;     //fd+l
	double dMdl;
	double dRatiodfa;//fa/fd+l

	// 등급 판정용
	double dPhiMn_Rank;
	double dMu_Rank;
	int    nRankType;//등급
	
	T_RATI_UNITLCOM_CH Load;  
	T_RATI_UNITLCOM_CH Load2; // 단부 

	char strLiveLoad[16];

	T_CSGD_ULMF  Ulmf; 
	T_CSGD_COMF  ChkFor;
	
	void ConvertToString(T_RFKR_BASE& rData)
	{
		//memcpy(&rData, this, FIELD_OFFSET(T_RFKR_BASE_CH, Load)); // T_RFKR_BASE_CH와 T_RFKR_BASE의 크기가 달라 이 매크로 사용 중지
		rData.bCheck         =  bCheck         ;  
		rData.bCheck2        =  bCheck2        ;  
		rData.ElemK          =  ElemK          ;  
		rData.nSectPosi      =  nSectPosi      ;     
		rData.nRCaseK        =  nRCaseK        ;       
		rData.nDgnRCaseK     =  nDgnRCaseK     ;    
		rData.nConcurrent    =  nConcurrent    ;   
		rData.dEstiDisp      =  dEstiDisp      ;    
		rData.dCalcDisp      =  dCalcDisp      ;    
		rData.dRatioDisp     =  dRatioDisp     ;   
		rData.dEstiEwip      =  dEstiEwip      ;    
		rData.dCalcEwip      =  dCalcEwip      ;    
		rData.dRatioEwip     =  dRatioEwip     ;   
		rData.dCalcImpFactor =  dCalcImpFactor ;  
		rData.dEstiImpFactor =  dEstiImpFactor ;  
		rData.dRatioImpFactor=  dRatioImpFactor;  
		rData.nRFca          =  nRFca          ;
		rData.dfa            =  dfa            ;  
		rData.dfd            =  dfd            ;          
		rData.dfl            =  dfl            ;
		rData.dMc            =  dMc            ;
		rData.dMd            =  dMd            ;
		rData.dMl            =  dMl            ;
		rData.d1iFactor      =  d1iFactor      ;  
		rData.dRF            =  dRF            ;  
		rData.dPr            =  dPr            ;  
		rData.dP0            =  dP0            ;  
		rData.dKs            =  dKs            ;  
		rData.dP             =  dP             ;   
		rData.nOK            =  nOK            ;  
		rData.dfdl           =  dfdl           ; 
		rData.dMdl           =  dMdl           ;
		rData.dRatiodfa      =  dRatiodfa      ;  
//     rData.dPhiMn_Rank    =  dPhiMn_Rank    ;  
//     rData.dMu_Rank       =  dMu_Rank       ;  
		rData.nRankType      =  nRankType      ;  

		Load.ConvertToString(rData.Load); 
		Load2.ConvertToString(rData.Load2); 
		ConvertCharStr(strLiveLoad, rData.strLiveLoad, sizeof(strLiveLoad));

		rData.Ulmf   = Ulmf;
		rData.ChkFor = ChkFor;
	}

	void ConvertToChar(T_RFKR_BASE& rData)
	{
		//memcpy(this, &rData, FIELD_OFFSET(T_RFKR_BASE, Load));
	  bCheck         = rData.bCheck         ;  
	  bCheck2        = rData.bCheck2        ;  
	  ElemK          = rData.ElemK          ;  
	  nSectPosi      = rData.nSectPosi      ;     
	  nRCaseK        = rData.nRCaseK        ;       
	  nDgnRCaseK     = rData.nDgnRCaseK     ;    
	  nConcurrent    = rData.nConcurrent    ;   
	  dEstiDisp      = rData.dEstiDisp      ;    
	  dCalcDisp      = rData.dCalcDisp      ;    
	  dRatioDisp     = rData.dRatioDisp     ;   
	  dEstiEwip      = rData.dEstiEwip      ;    
	  dCalcEwip      = rData.dCalcEwip      ;    
	  dRatioEwip     = rData.dRatioEwip     ;   
	  dCalcImpFactor = rData.dCalcImpFactor ;  
	  dEstiImpFactor = rData.dEstiImpFactor ;  
	  dRatioImpFactor= rData.dRatioImpFactor;  
		nRFca          = rData.nRFca          ;
	  dfa            = rData.dfa            ;  
	  dfd            = rData.dfd            ;          
	  dfl            = rData.dfl            ;           
		dMc            = rData.dMc            ;
		dMd            = rData.dMd            ;
		dMl            = rData.dMl            ;
	  d1iFactor      = rData.d1iFactor      ;  
	  dRF            = rData.dRF            ;  
	  dPr            = rData.dPr            ;  
	  dP0            = rData.dP0            ;  
	  dKs            = rData.dKs            ;  
	  dP             = rData.dP             ;   
	  nOK            = rData.nOK            ;  
	  dfdl           = rData.dfdl           ;
		dMdl           = rData.dMdl           ;
	  dRatiodfa      = rData.dRatiodfa      ;  
	  //dPhiMn_Rank		=   rData.dPhiMn_Rank;  
	  //dMu_Rank		=	rData.dMu_Rank	;  
	  nRankType		= rData.nRankType;  

	  Load.ConvertToChar(rData.Load); 
	  Load2.ConvertToChar(rData.Load2); 
	  ConvertStrChar(rData.strLiveLoad, strLiveLoad, sizeof(strLiveLoad));

		Ulmf   = rData.Ulmf;
		ChkFor = rData.ChkFor;
}
};

struct T_RFKR_CASE_CH
{
	T_RFKR_BASE_CH RfkrBase[4]; // 0=I-상연, 1=I-하연, 2=J-상연, 3=J-하연  
	int nForceSize;
	int nResultCheck;
	CArray<RATING_FORCE_CH, RATING_FORCE_CH&> arForceRCaseI;
	CArray<RATING_FORCE_CH, RATING_FORCE_CH&> arForceRCaseJ;
	
	void ConvertToString(T_RFKR_CASE& rData)
	{
		for (int i = 0; i<4; ++i)
		{
			RfkrBase[i].ConvertToString(rData.RfkrBase[i]);
		}
		rData.nForceSize = nForceSize;
		rData.nResultCheck = nForceSize;
		rData.arForceRCaseI.SetSize(nForceSize);
		rData.arForceRCaseJ.SetSize(nForceSize);
		for (int i = 0; i<nForceSize; ++i)
		{
			arForceRCaseI[i].ConvertToString(rData.arForceRCaseI[i]);
			arForceRCaseJ[i].ConvertToString(rData.arForceRCaseJ[i]);
		}
	}
	void ConvertToChar(T_RFKR_CASE& rData)
	{
		for (int i = 0; i<4; ++i)
		{
			RfkrBase[i].ConvertToChar(rData.RfkrBase[i]);
		}
		nForceSize = rData.nForceSize;
		nResultCheck = rData.nResultCheck;
		this->arForceRCaseI.SetSize(nForceSize);
		this->arForceRCaseJ.SetSize(nForceSize);
		for (int i = 0; i<nForceSize; ++i)
		{
			arForceRCaseI[i].ConvertToChar(rData.arForceRCaseI[i]);
			arForceRCaseJ[i].ConvertToChar(rData.arForceRCaseJ[i]);
		}
	}
};

// RC Rating
struct T_RFRC_BASE
{
	BOOL bCheck;
	BOOL bCheck2;						  // 결과 Diagram을 그릴 때 Rating Factor와 기본내하력은 이값이 TRUE일때 그림.. (그외의 경우는 bCheck가 false이면 값이 없다고 봄)
	T_ELEM_K ElemK;
	T_NODE_K MinNodeK;        // 가장 불리한 처짐 위치(plate) 
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
	double dAddResponceFactor;    // 추가응답보정계수
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

	// 등급 검토용 변수
	double dPhiMn_Rank;
	double dMu_Rank;
	double dRatioMa ;//Mn/(Md+Ml)
	int    nRankType;//등급

	RATING_FORCE  Rating_Force; // Rating 계산용 하중정보 (Include only checked item)
	RATING_FORCE  Rating_Force_Str; // 강도 검토용 하중 정보(Include all checked item)
	BOOL bOK;

	void Initialize()
	{
		bCheck = FALSE;
		bCheck2 = FALSE;
		ElemK = 0;
		MinNodeK =0;
		nRCaseK = 0;
		nDgnRCaseK = 0;
		nConcurrent = 0;
		strLcomName = _T("");
		dPhiMn = 0.0;
		dMd = 0.0;
		dM_L = 0.0;
		dRealDispZ = 0.0;
		dCalc_defl = 0.0;      
		dImpFactor = 0.0;
		dRealImpFactor = 0.0;
		dAddResponceFactor = 0.0;
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
		dPhiMn_Rank = 0.0;
		dMu_Rank = 0.0;
		dRatioMa  = 0.0;
		nRankType = 0;

		Rating_Force.Initialize();
		Rating_Force_Str.Initialize();

		bOK = FALSE;
	}
};

struct T_RFRC_UNIT
{
	T_RFRC_UNIT()
	{
		dPhiMn     = D_UNITSYS_BASE_MOMENT;
		dMd        = D_UNITSYS_BASE_MOMENT;
		dMd_m      = D_UNITSYS_BASE_MOMENT;
		dM_L       = D_UNITSYS_BASE_MOMENT;
		dCalc_defl = D_UNITSYS_BASE_LENGTH;
		dRealDispZ = D_UNITSYS_BASE_LENGTH;
		dMdl       = D_UNITSYS_BASE_MOMENT;
		dPhiMn_Rank = D_UNITSYS_BASE_MOMENT;
		dMu_Rank = D_UNITSYS_BASE_MOMENT;
		dFl        = D_UNITSYS_BASE_STRESS;
	}  
	int dPhiMn;
	int dMd;
	int dMd_m;
	int dM_L;
	int dCalc_defl;  
	int dRealDispZ;
	int dMdl;
	int dPhiMn_Rank;
	int dMu_Rank;
	int dFl;
};

struct T_RFRC_CASE
{
	T_RFRC_BASE RfrcBase[2]; 

	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			RfrcBase[i].Initialize();      
		}
	}
};

struct T_RFRC_D
{
	T_RFRC_BASE RfrcBase[4]; // 0=I-max(정), 1=I-min(부), 2=J-max(정), 3=J-min(부)  

	void Initialize()
	{
		for(int i=0; i<4; i++)
		{
			RfrcBase[i].Initialize();      
		}
	}
};

struct T_RFRC_BASE_CH
{
	BOOL bCheck;
	BOOL bCheck2;
	T_ELEM_K ElemK;
	T_NODE_K MinNodeK;
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
	double dAddResponceFactor;    // 추가응답보정계수
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

	// 등급 판정용
	double dPhiMn_Rank;
	double dMu_Rank;
	double dRatioMa ;//Mn/(Md+Ml)
	int    nRankType;//등급
	BOOL bOK;

	char strLcomName[16];     // LcomName + ype;
	RATING_FORCE_CH  Rating_Force;
	RATING_FORCE_CH  Rating_Force_Str;

	void ConvertToString(T_RFRC_BASE& rData)
	{
		rData.bCheck         =  bCheck         ;
		rData.bCheck2        =  bCheck2;
		rData.ElemK          =  ElemK          ;
		rData.MinNodeK       =  MinNodeK;
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
		rData.dAddResponceFactor = dAddResponceFactor;
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
		rData.dPhiMn_Rank    =  dPhiMn_Rank;
		rData.dMu_Rank       =  dMu_Rank;
		rData.nRankType      =  nRankType      ;       
		rData.bOK            =  bOK            ;

		ConvertCharStr(strLcomName, rData.strLcomName, sizeof(strLcomName)); 
		Rating_Force.ConvertToString(rData.Rating_Force);
		Rating_Force_Str.ConvertToString(rData.Rating_Force_Str);

	}
	void ConvertToChar(T_RFRC_BASE& rData)
	{
		bCheck         =  rData.bCheck         ;
		bCheck2        =  rData.bCheck2        ;
		ElemK          =  rData.ElemK          ;
		MinNodeK       =  rData.MinNodeK;
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
		dAddResponceFactor = rData.dAddResponceFactor;
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
		dPhiMn_Rank    =  rData.dPhiMn_Rank    ;  
		dMu_Rank       =  rData.dMu_Rank;
		nRankType      =  rData.nRankType      ;       
		bOK            =  rData.bOK            ;

		ConvertStrChar(rData.strLcomName, strLcomName, sizeof(strLcomName)); 
		Rating_Force.ConvertToChar(rData.Rating_Force);
		Rating_Force_Str.ConvertToChar(rData.Rating_Force_Str);
	}

};


struct T_RFRC_CASE_CH
{
	T_RFRC_BASE_CH RfrcBase[2]; // 0=I, 1=J

	void ConvertToString(T_RFRC_CASE& rData)
	{
		for(int i=0; i<2; i++)
		{
			RfrcBase[i].ConvertToString(rData.RfrcBase[i]);   
		}
	}
	void ConvertToChar(T_RFRC_CASE& rData)
	{
		for(int i=0; i<2; i++)
		{
			RfrcBase[i].ConvertToChar(rData.RfrcBase[i]);   
		}
	}

};

// RC Plate Rating
struct T_RFRP_D
{
	T_RFRC_BASE RfrpBase[4]; // 0=I-max(정), 1=I-min(부), 2=J-max(정), 3=J-min(부)  : I/J 구분이 없어 일단 0, 1만 사용.
	
	void Initialize()
	{
		for(int i=0; i<4; i++)
		{
			RfrpBase[i].Initialize();      
		}
	}
};

struct T_RFRP_CASE
{
	T_RFRC_BASE RfrpBase[2]; // 0=I, 1=J

	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			RfrpBase[i].Initialize();      
		}
	}
};


struct T_RFRP_CASE_CH
{
	T_RFRC_BASE_CH RfrpBase[2]; // 0=I, 1=J

	void ConvertToString(T_RFRP_CASE& rData)
	{
		for(int i=0; i<2; i++)
		{
			RfrpBase[i].ConvertToString(rData.RfrpBase[i]);   
		}
	}
	void ConvertToChar(T_RFRP_CASE& rData)
	{
		for(int i=0; i<2; i++)
		{
			RfrpBase[i].ConvertToChar(rData.RfrpBase[i]);   
		}
	}

};


//***********************************
// PSC Rating KSCE-ASD 05 10 RAIL 04 11
struct T_RFPC_BASE
{
	BOOL bCheck;
	BOOL bCheck2;    // RF, 기본내하력 출력용 
	T_ELEM_K ElemK;
	//UINT nSectPosi;           // 1,2,3,4 단면의..           [Unit=None]
	UINT nRCaseK;             // Rating Case Key            [Unit=None]
	UINT nDgnRCaseK;          // Dgn Rating Case Key        [Unit=None]
	UINT nConcurrent;         // Fx_max .... Mz_min(1~12)   [Unit=None]   

	double dMn;						// Mn
	double dMd;						// Md
	double dMl;						// Ml
	double dRF;						// Rating Factor
	double dPr;						// 설계활하중
	double dP0;						// 기본내하력
	double dCalcDisp;     // 계산처짐
	double dCalcEwip;     // 계산변형률
	double dKs;						//응답보정계수
	double dP;						//공용내하력
	int    nOK;						//판정 0 : -,  1 : OK 2 : NG

	void Initialize()
	{
		bCheck = FALSE;
		bCheck2 = FALSE;
		ElemK = 0;  
		nRCaseK = 0;
		nDgnRCaseK = 0;
		nConcurrent = 0;
		dMn = 0;
		dMd = 0;
		dMl = 0;
		dRF = 0;
		dPr = 0;
		dP0 = 0;
		dCalcDisp = 0;
		dCalcEwip = 0;
		dKs = 0;
		dP = 0;
		nOK = 0;
	}
};

struct T_RFPC_UNIT
{
	T_RFPC_UNIT()
	{    
		dMn = D_UNITSYS_BASE_MOMENT;
		dMd = D_UNITSYS_BASE_MOMENT;
		dMl = D_UNITSYS_BASE_MOMENT;

		dCalcDisp = D_UNITSYS_BASE_LENGTH;    
	}  
	double dMn;						// Mn
	double dMd;						// Md
	double dMl;						// Ml
	double dCalcDisp;     // 계산처짐  
	void Initialize()
	{
		double dMn = 0;						// Mn
		double dMd = 0;						// Md
		double dMl = 0;						// Ml
		double dCalcDisp = 0;     // 계산처짐  
	}
};

struct T_RFPC_D
{
	T_RFPC_BASE BaseI[2]; // 0=max(인장), 1=min(압축)
	T_RFPC_BASE BaseJ[2]; // 0=max(인장), 1=min(압축)

	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			BaseI[i].Initialize();
			BaseJ[i].Initialize();
		}
	}
};


struct T_RDPO_LC
{
	int nLcomType; // D_LCOM_STATIC, ...
	int nLcomKey;
	T_RDPO_LC& operator=(const T_RDPO_LC& src)
	{
		nLcomType = src.nLcomType;
		nLcomKey = src.nLcomKey;
		return *this;
	}
	BOOL operator==(const T_RDPO_LC& src) const
	{
		if(nLcomType == src.nLcomType && nLcomKey == src.nLcomKey) return TRUE;
		else return FALSE;
	}
};

/// Rating Design Print Option ///
const int RDPO_STL_MODE=0, RDPO_PSC_MODE=1, RDPO_RC_MODE=2;
#define T_RDPO_K unsigned int
struct T_RDPO_D
{
	// 보고서 출력
	CString   m_strStlTitle;     // 제목
	CString   m_strPscTitle;     // 제목
	
	// 출력 그림 (Color)
	COLORREF  m_colSect    ;  // 모델(단면)
	COLORREF  m_colLine    ;  // 모델(선)
	COLORREF  m_colNode    ;  // 절점
	COLORREF  m_colNodeNum ;  // 절점(번호)
	COLORREF  m_colElemNum ;  // 요소(번호)
	COLORREF  m_colSprnBind;  // 스프링(구속)
	COLORREF  m_colSprnNone;  // 스프링(비구속)
	COLORREF  m_colBeamLoad;  // Beam Load
	COLORREF  m_colTempLoad;  // Temp. Load
	COLORREF  m_colTempGrad;  // Temp. Gradient
	COLORREF  m_colLoadVal ;  // Load Value
	COLORREF  m_colRsltVal ;  // Result Value
	
	// 출력 그림 (Font/Size)
	CString   m_strNodeFont;  // 절점/요소(번호)
	CString   m_strLoadFont;  // Load Value
	CString   m_strRsltFont;  // Result Value
	int       m_nNodeFontSize; // 절점/요소(번호)
	int       m_nLoadFontSize; // Load Value
	int       m_nRsltFontSize; // Result Value
	int       m_nSymbol;       // Label Symbol
	BOOL      m_bFontNode    ; // Font Node Bold Chk
	BOOL      m_bFontValue   ; // Font Value Bold Chk
	BOOL      m_bFontRslt    ; // Font Result Bold Chk
	
	// 단면력도
	int       m_nContour;      // 0:Solid Fill, 1:Line Fill
	double    m_dContourScale; // Scale
	int       m_nResultValue;  // 0:Abs Max, 1:Min & Max 2: All;
	BOOL      m_bAll_I;        // I
	BOOL      m_bAll_C;        // Center
	BOOL      m_bAll_J;        // J
	BOOL      m_bAll_Mid;      // Mid
	
	CArray<T_RDPO_LC, T_RDPO_LC&> m_aSelectedLC; // 사용 안함

	T_RDPO_D() {}
	T_RDPO_D(const T_RDPO_D& src) { *this = src; }
	T_RDPO_D& operator=(const T_RDPO_D& src)
	{
		m_strStlTitle      = src.m_strStlTitle;     // 제목
		m_strPscTitle      = src.m_strPscTitle;     // 제목
		m_colSect       = src.m_colSect;      // 모델(단면)
		m_colLine       = src.m_colLine;      // 모델(선)
		m_colNode       = src.m_colNode;      // 절점
		m_colNodeNum    = src.m_colNodeNum;   // 절점(번호)
		m_colElemNum    = src.m_colElemNum;   // 요소(번호)
		m_colSprnBind   = src.m_colSprnBind;  // 스프링(구속)
		m_colSprnNone   = src.m_colSprnNone;  // 스프링(비구속)
		m_colBeamLoad   = src.m_colBeamLoad;  // Beam Load
		m_colTempLoad   = src.m_colTempLoad;  // Temp. Load
		m_colTempGrad   = src.m_colTempGrad;  // Temp. Gradient
		m_colLoadVal    = src.m_colLoadVal ;  // Load Value
		m_colRsltVal    = src.m_colRsltVal ;  // Result Value
		m_strNodeFont   = src.m_strNodeFont;  // 절점/요소(번호)
		m_strLoadFont   = src.m_strLoadFont;  // Load Value
		m_strRsltFont   = src.m_strRsltFont;  // Result Value
		m_nNodeFontSize = src.m_nNodeFontSize; // 절점/요소(번호)
		m_nLoadFontSize = src.m_nLoadFontSize; // Load Value
		m_nRsltFontSize = src.m_nRsltFontSize; // Result Value
		m_nSymbol       = src.m_nSymbol;       // Label Symbol
		m_bFontNode     = src.m_bFontNode ;    // Font Node Bold Chk
		m_bFontValue    = src.m_bFontValue;    // Font Value Bold Chk
		m_bFontRslt     = src.m_bFontRslt ;    // Font Result Bold Chk
		m_nContour      = src.m_nContour;      // 0:Solid Fill, 1:Line Fill
		m_dContourScale = src.m_dContourScale; // Scale
		m_nResultValue  = src.m_nResultValue;  // 0:Abs Max, 1:Min & Max 2: All;
		m_bAll_I        = src.m_bAll_I;        // I
		m_bAll_C        = src.m_bAll_C;        // Center
		m_bAll_J        = src.m_bAll_J;        // J
		m_bAll_Mid      = src.m_bAll_Mid;      // Mid
		m_aSelectedLC.Copy(src.m_aSelectedLC);
		
		return *this;
	}
//  void Initialize() {} // 대화상자에서 초기화
};
struct T_RDPO_D_CH
{
	// 보고서 출력
	char      m_strStlTitle[80]; // 제목
	char      m_strPscTitle[80]; // 제목
	
	// 출력 그림 (Color)
	COLORREF  m_colSect    ;  // 모델(단면)
	COLORREF  m_colLine    ;  // 모델(선)
	COLORREF  m_colNode    ;  // 절점
	COLORREF  m_colNodeNum ;  // 절점(번호)
	COLORREF  m_colElemNum ;  // 요소(번호)
	COLORREF  m_colSprnBind;  // 스프링(구속)
	COLORREF  m_colSprnNone;  // 스프링(비구속)
	COLORREF  m_colBeamLoad;  // Beam Load
	COLORREF  m_colTempLoad;  // Temp. Load
	COLORREF  m_colTempGrad;  // Temp. Gradient
	COLORREF  m_colLoadVal ;  // Load Value
	COLORREF  m_colRsltVal ;  // Result Value
	
	// 출력 그림 (Font/Size)
	char      m_strNodeFont[40];  // 절점/요소(번호)
	char      m_strLoadFont[40];  // Load Value
	char      m_strRsltFont[40];  // Result Value
	int       m_nNodeFontSize; // 절점/요소(번호)
	int       m_nLoadFontSize; // Load Value
	int       m_nRsltFontSize; // Result Value
	int       m_nSymbol;       // Label Symbol
	BOOL      m_bFontNode    ; // Font Node Bold Chk
	BOOL      m_bFontValue   ; // Font Value Bold Chk
	BOOL      m_bFontRslt    ; // Font Result Bold Chk
	
	// 단면력도
	int       m_nContour     ; // 0:Solid Fill, 1:Line Fill
	double    m_dContourScale; // Scale
	int       m_nResultValue ; // 0:Abs Max, 1:Min & Max 2: All;
	BOOL      m_bAll_I       ; // I
	BOOL      m_bAll_C       ; // Center
	BOOL      m_bAll_J       ; // J
	BOOL      m_bAll_Mid     ; // Mid

	void ConvertToString(T_RDPO_D& rData)
	{
		ConvertCharStr(m_strStlTitle, rData.m_strStlTitle, sizeof(m_strStlTitle));   
		ConvertCharStr(m_strPscTitle, rData.m_strPscTitle, sizeof(m_strPscTitle));   
		ConvertCharStr(m_strNodeFont, rData.m_strNodeFont, sizeof(m_strNodeFont));
		ConvertCharStr(m_strLoadFont, rData.m_strLoadFont, sizeof(m_strLoadFont));
		ConvertCharStr(m_strRsltFont, rData.m_strRsltFont, sizeof(m_strRsltFont));
		rData.m_colSect       = m_colSect       ;
		rData.m_colLine       = m_colLine       ;
		rData.m_colNode       = m_colNode       ;
		rData.m_colNodeNum    = m_colNodeNum    ;
		rData.m_colElemNum    = m_colElemNum    ;
		rData.m_colSprnBind   = m_colSprnBind   ;
		rData.m_colSprnNone   = m_colSprnNone   ;
		rData.m_colBeamLoad   = m_colBeamLoad   ;
		rData.m_colTempLoad   = m_colTempLoad   ;
		rData.m_colTempGrad   = m_colTempGrad   ;
		rData.m_colLoadVal    = m_colLoadVal    ;
		rData.m_colRsltVal    = m_colRsltVal    ;
		rData.m_nNodeFontSize = m_nNodeFontSize ;
		rData.m_nLoadFontSize = m_nLoadFontSize ;
		rData.m_nRsltFontSize = m_nRsltFontSize ;
		rData.m_nSymbol       = m_nSymbol       ;
		rData.m_bFontNode     = m_bFontNode     ;
		rData.m_bFontValue    = m_bFontValue    ;
		rData.m_bFontRslt     = m_bFontRslt     ;
		rData.m_nContour      = m_nContour      ;
		rData.m_dContourScale = m_dContourScale ;
		rData.m_nResultValue  = m_nResultValue  ;
		rData.m_bAll_I        = m_bAll_I        ;
		rData.m_bAll_C        = m_bAll_C        ;
		rData.m_bAll_J        = m_bAll_J        ;
		rData.m_bAll_Mid      = m_bAll_Mid      ;
	}
	void ConvertToChar(T_RDPO_D& rData)
	{
		ConvertStrChar(rData.m_strStlTitle, m_strStlTitle, sizeof(m_strStlTitle)); 
		ConvertStrChar(rData.m_strPscTitle, m_strPscTitle, sizeof(m_strPscTitle)); 
		ConvertStrChar(rData.m_strNodeFont, m_strNodeFont, sizeof(m_strNodeFont));
		ConvertStrChar(rData.m_strLoadFont, m_strLoadFont, sizeof(m_strLoadFont));
		ConvertStrChar(rData.m_strRsltFont, m_strRsltFont, sizeof(m_strRsltFont));
		m_colSect       = rData.m_colSect      ;
		m_colLine       = rData.m_colLine      ;
		m_colNode       = rData.m_colNode      ;
		m_colNodeNum    = rData.m_colNodeNum   ;
		m_colElemNum    = rData.m_colElemNum   ;
		m_colSprnBind   = rData.m_colSprnBind  ;
		m_colSprnNone   = rData.m_colSprnNone  ;
		m_colBeamLoad   = rData.m_colBeamLoad  ;
		m_colTempLoad   = rData.m_colTempLoad  ;
		m_colTempGrad   = rData.m_colTempGrad  ;
		m_colLoadVal    = rData.m_colLoadVal   ;
		m_colRsltVal    = rData.m_colRsltVal   ;
		m_nNodeFontSize = rData.m_nNodeFontSize;
		m_nLoadFontSize = rData.m_nLoadFontSize;
		m_nRsltFontSize = rData.m_nRsltFontSize;
		m_nSymbol       = rData.m_nSymbol      ;
		m_bFontNode     = rData.m_bFontNode    ;
		m_bFontValue    = rData.m_bFontValue   ;
		m_bFontRslt     = rData.m_bFontRslt    ;
		m_nContour      = rData.m_nContour     ;
		m_dContourScale = rData.m_dContourScale;
		m_nResultValue  = rData.m_nResultValue ;
		m_bAll_I        = rData.m_bAll_I       ;
		m_bAll_C        = rData.m_bAll_C       ;
		m_bAll_J        = rData.m_bAll_J       ;
		m_bAll_Mid      = rData.m_bAll_Mid     ;
	}
};
struct T_RDPO_UDRD_D
{
	T_RDPO_D data;
};
struct T_RDPO_UDRD_D_CH
{
	T_RDPO_D_CH data;
};
struct T_RDPO_UDRD_D_CH_RW
{
	char      m_strStlTitle[80]; // 제목
	char      m_strPscTitle[80]; // 제목
	COLORREF  m_colSect    ;  // 모델(단면)
	COLORREF  m_colLine    ;  // 모델(선)
	COLORREF  m_colNode    ;  // 절점
	COLORREF  m_colNodeNum ;  // 절점(번호)
	COLORREF  m_colElemNum ;  // 요소(번호)
	COLORREF  m_colSprnBind;  // 스프링(구속)
	COLORREF  m_colSprnNone;  // 스프링(비구속)
	COLORREF  m_colBeamLoad;  // Beam Load
	COLORREF  m_colTempLoad;  // Temp. Load
	COLORREF  m_colTempGrad;  // Temp. Gradient
	COLORREF  m_colLoadVal ;  // Load Value
	COLORREF  m_colRsltVal ;  // Result Value
	char      m_strNodeFont[40];  // 절점/요소(번호)
	char      m_strLoadFont[40];  // Load Value
	char      m_strRsltFont[40];  // Result Value
	int       m_nNodeFontSize; // 절점/요소(번호)
	int       m_nLoadFontSize; // Load Value
	int       m_nRsltFontSize; // Result Value
	int       m_nSymbol      ; // Label Symbol
	BOOL      m_bFontNode    ; // Font Node Bold Chk
	BOOL      m_bFontValue   ; // Font Value Bold Chk
	BOOL      m_bFontRslt    ; // Font Result Bold Chk
	int       m_nContour     ; // 0:Solid Fill, 1:Line Fill
	double    m_dContourScale; // Scale
	int       m_nResultValue ; // 0:Abs Max, 1:Min & Max 2: All;
	BOOL      m_bAll_I       ; // I
	BOOL      m_bAll_C       ; // Center
	BOOL      m_bAll_J       ; // J
	BOOL      m_bAll_Mid     ; // Mid

	void GetRdpo(T_RDPO_D_CH& rData)
	{
		memcpy(rData.m_strStlTitle, m_strStlTitle, sizeof(m_strStlTitle));
		memcpy(rData.m_strPscTitle, m_strPscTitle, sizeof(m_strPscTitle));
		memcpy(rData.m_strNodeFont, m_strNodeFont, sizeof(m_strNodeFont));
		memcpy(rData.m_strLoadFont, m_strLoadFont, sizeof(m_strLoadFont));
		memcpy(rData.m_strRsltFont, m_strRsltFont, sizeof(m_strRsltFont));
		rData.m_colSect       = m_colSect       ;
		rData.m_colLine       = m_colLine       ;
		rData.m_colNode       = m_colNode       ;
		rData.m_colNodeNum    = m_colNodeNum    ;
		rData.m_colElemNum    = m_colElemNum    ;
		rData.m_colSprnBind   = m_colSprnBind   ;
		rData.m_colSprnNone   = m_colSprnNone   ;
		rData.m_colBeamLoad   = m_colBeamLoad   ;
		rData.m_colTempLoad   = m_colTempLoad   ;
		rData.m_colTempGrad   = m_colTempGrad   ;
		rData.m_colLoadVal    = m_colLoadVal    ;
		rData.m_colRsltVal    = m_colRsltVal    ;
		rData.m_nNodeFontSize = m_nNodeFontSize ;
		rData.m_nLoadFontSize = m_nLoadFontSize ;
		rData.m_nRsltFontSize = m_nRsltFontSize ;
		rData.m_nSymbol       = m_nSymbol       ;
		rData.m_bFontNode     = m_bFontNode     ;
		rData.m_bFontValue    = m_bFontValue    ;
		rData.m_bFontRslt     = m_bFontRslt     ;
		rData.m_nContour      = m_nContour      ;
		rData.m_dContourScale = m_dContourScale ;
		rData.m_nResultValue  = m_nResultValue  ;
		rData.m_bAll_I        = m_bAll_I        ;
		rData.m_bAll_C        = m_bAll_C        ;
		rData.m_bAll_J        = m_bAll_J        ;
		rData.m_bAll_Mid      = m_bAll_Mid      ;
	}
	void SetRdpo(const T_RDPO_D_CH& rData)
	{
		memcpy(m_strStlTitle, rData.m_strStlTitle, sizeof(m_strStlTitle));
		memcpy(m_strPscTitle, rData.m_strPscTitle, sizeof(m_strPscTitle));
		memcpy(m_strNodeFont, rData.m_strNodeFont, sizeof(m_strNodeFont));
		memcpy(m_strLoadFont, rData.m_strLoadFont, sizeof(m_strLoadFont));
		memcpy(m_strRsltFont, rData.m_strRsltFont, sizeof(m_strRsltFont));
		m_colSect       = rData.m_colSect      ;
		m_colLine       = rData.m_colLine      ;
		m_colNode       = rData.m_colNode      ;
		m_colNodeNum    = rData.m_colNodeNum   ;
		m_colElemNum    = rData.m_colElemNum   ;
		m_colSprnBind   = rData.m_colSprnBind  ;
		m_colSprnNone   = rData.m_colSprnNone  ;
		m_colBeamLoad   = rData.m_colBeamLoad  ;
		m_colTempLoad   = rData.m_colTempLoad  ;
		m_colTempGrad   = rData.m_colTempGrad  ;
		m_colLoadVal    = rData.m_colLoadVal   ;
		m_colRsltVal    = rData.m_colRsltVal   ;
		m_nNodeFontSize = rData.m_nNodeFontSize;
		m_nLoadFontSize = rData.m_nLoadFontSize;
		m_nRsltFontSize = rData.m_nRsltFontSize;
		m_nSymbol       = rData.m_nSymbol      ;
		m_bFontNode     = rData.m_bFontNode    ;
		m_bFontValue    = rData.m_bFontValue   ;
		m_bFontRslt     = rData.m_bFontRslt    ;
		m_nContour      = rData.m_nContour     ;
		m_dContourScale = rData.m_dContourScale;
		m_nResultValue  = rData.m_nResultValue ;
		m_bAll_I        = rData.m_bAll_I       ;
		m_bAll_C        = rData.m_bAll_C       ;
		m_bAll_J        = rData.m_bAll_J       ;
		m_bAll_Mid      = rData.m_bAll_Mid     ;
	}
};
struct T_RDPO_UNIT
{
	T_RDPO_UNIT()
	{
		nNoUnit=D_UNITSYS_NONE;    
	}
	int nNoUnit;  
};
#define HASHSIZERDPO 1

#pragma pack()

#endif





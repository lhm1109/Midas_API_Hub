#ifndef __DB_ST_DN_RSLT_PLATE_H__
#define __DB_ST_DN_RSLT_PLATE_H__

#define MIDAS_SDK

#include "DB_ST_DT_UNIT.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../dgnengine/idesign/DGN_def/SectionRebarPlateDef.h"

#pragma pack(1)

const int C_PLATE_POS		=  3;


struct T_PDEL_KEY
{
	BOOL bChk;
	UINT nLcomK;
	UINT nDgnLcomK;
	UINT nConc;       //MIN-MAX타입이 아님=0, FX-MAX=1 ...
	UINT ElemK;
	UINT nPos;        //Plate (center of element + number of node) 5개 Point 부재력 위치

    //for engine
    int nPosiItrKey;

	void Initialize()
	{
		bChk      = FALSE;
		nLcomK    = 0;
		nDgnLcomK = 0;
		nConc     = 0;
		ElemK     = 0;
		nPos      = 0;
        nPosiItrKey = 0;
	}
	T_PDEL_KEY() {Initialize();}
};

struct T_PLTD_FORCE
{
	double dForce[6];

	void Initialize()
	{
		memset(dForce,0,sizeof(dForce));
	}
	T_PLTD_FORCE() {Initialize();}
};

struct T_PLTD_UNIT
{
	T_PLTD_UNIT()
	{
		dForce[0] = D_UNITSYS_BASE_FORCE;       
		dForce[1] = D_UNITSYS_BASE_FORCE;   
		dForce[2] = D_UNITSYS_BASE_FORCE;       
		dForce[3] = D_UNITSYS_BASE_MOMENT;   
		dForce[4] = D_UNITSYS_BASE_MOMENT;       
		dForce[5] = D_UNITSYS_BASE_MOMENT;   
	}
	int dForce[6];
};


struct T_PBRE_STRN_POSI
{
	T_PDEL_KEY   KeyD;
	T_PLTD_FORCE ForD;

	int    nAsRes;
	int    nAsResV;
	int    nAsResT;
	double dpMn;
	double dpVc;
	double dpVs;
	double dpVn;
	double dpTn;
	double dRatM;
	double dRatV;
	double dRatT;

	double dv; //stress
    double dpvc;
    double dpvs;
    double dpvn;

	double dSv_Req;
	double dSubBarDist;
	double dAsApp;      // Applied As with Rebar
	double dAsReq;      // Required As if there are Rebars
	double dRatV_Code;  // Eurocode2-2:05 
	double dV_Rd;  //EC2 Shear Resistance  V_Rd = V_Rdc > V_Ed ? V_Rdc : V_Rds
	BOOL   bOK;

	double dfd;  // Fatigue
	double dfr;
	double dRatF;

    BOOL   bChkLongAsByV; // for AASHTO Shear
    BOOL   bChkLongAsByT; // for AASHTO Torsion

	// neutral axis check.
	double dxd;
	double dxd_limit;

	void Initialize()
	{
		KeyD.Initialize();
		ForD.Initialize();
		nAsRes  = 0;
		nAsResV = 0;
		nAsResT = 0;
		dpMn    = 0.0;
		dpVc    = 0.0;
		dpVs    = 0.0;
		dpVn    = 0.0;
		dpTn    = 0.0;
		dRatM   = 0.0;
		dRatV   = 0.0;
		dRatT   = 0.0;

		dv   = 0.0;
        dpvc = 0.0;
        dpvs = 0.0;
        dpvn = 0.0;

		dAsApp  = 0.0;
		dAsReq  = 0.0;
		bOK     = 0.0;
		dSv_Req = 0.0;
		dRatV_Code = 0.0;
		dSubBarDist = 0.0;
		dpVs = 0.0;
		bOK = FALSE;

		dfd = 0.0;
		dfr = 0.0;
		dRatF = 0.0;

        bChkLongAsByV = TRUE;
        bChkLongAsByT = TRUE;

		dxd = 0.0;
		dxd_limit = 0.0;
	}
	T_PBRE_STRN_POSI() {Initialize();}
};

struct T_PBRE_STRN_POSI_UNIT
{
	T_PBRE_STRN_POSI_UNIT()
	{
		dpMn   = D_UNITSYS_BASE_MOMENT;
		dpVc   = D_UNITSYS_BASE_FORCE;
		dpVs   = D_UNITSYS_BASE_FORCE;
		dpVn   = D_UNITSYS_BASE_FORCE;
		dpTn   = D_UNITSYS_BASE_MOMENT;
		dAsApp = D_UNITSYS_BASE_AREA;
		dAsReq = D_UNITSYS_BASE_AREA;
	}
	int dpMn;
	int dpVc;
	int dpVs;
	int dpVn;
	int dpTn;
	int dAsApp;
	int dAsReq;
};

struct T_PBRE_SERV_POSI
{
	T_PDEL_KEY   KeyD;
	T_PLTD_FORCE ForD;
	//Crack
	double dw;
	double dwa;
	double dfs;
	double ds;
	double dsa;
	double dAs_Top;
	double dAs_Bot;
	double dAs_req;
	//stress
	double dConSs;
	double dConSsa;
	double dStlSs;
	double dStlSsa;
	//deflection
	double dMaxDispZ;
	double dDeflectLim;

	BOOL bOK_Dist; // Shrinkage and Temperature (배력철근)

	void Initialize()
	{
		KeyD.Initialize();
		ForD.Initialize();
		dw  = 0.0;
		dwa = 0.0;
		dfs = 0.0;
		ds  = 0.0;
		dsa = 0.0;
		dAs_Top = 0.0;
		dAs_Bot = 0.0;
		dAs_req = 0.0;
		dConSs  = 0.0;
		dConSsa  = 0.0;
		dStlSs  = 0.0;
		dStlSsa = 0.0;
		dMaxDispZ   = 0.0;
		dDeflectLim = 0.0;
		bOK_Dist= FALSE;
	}
	T_PBRE_SERV_POSI() {Initialize();}
};

struct T_PBRE_SERV_POSI_UNIT
{
	T_PBRE_SERV_POSI_UNIT()
	{
		dw  = D_UNITSYS_BASE_LENGTH;
		dwa = D_UNITSYS_BASE_LENGTH;
		dfs = D_UNITSYS_BASE_STRESS;
		ds  = D_UNITSYS_BASE_LENGTH;
		dsa = D_UNITSYS_BASE_LENGTH;
		dAs     = D_UNITSYS_BASE_AREA;
		dAs_req = D_UNITSYS_BASE_AREA;
		dConSs  = D_UNITSYS_BASE_STRESS;
		dConSsa = D_UNITSYS_BASE_STRESS;
		dStlSs  = D_UNITSYS_BASE_STRESS;
		dStlSsa = D_UNITSYS_BASE_STRESS;
		dMaxDispZ   = D_UNITSYS_BASE_LENGTH;
		dDeflectLim = D_UNITSYS_BASE_LENGTH;
	}
	int dw;
	int dwa;
	int dfs;
	int ds;
	int dsa;
	int dAs;
	int dAs_req;
	int dConSs;
	int dConSsa;
	int dStlSs;
	int dStlSsa;
	int dMaxDispZ;
	int dDeflectLim;
};

// Local direction
struct T_PBRE_STRN_LOCD
{
	T_PBRE_STRN_POSI PosR[C_PLATE_POS];
	void Initialize()
	{
		for(int i=0; i<C_PLATE_POS; ++i)
		{
			PosR[i].Initialize();
		}
	}
	T_PBRE_STRN_LOCD() {Initialize();}
};

// 하중조합별 결과
struct T_PBRE_STRN_LCOM
{
	T_PBRE_STRN_LOCD LDir[2];
	void Initialize()
	{
		for(int i=0; i<2; ++i) LDir[i].Initialize();
	}
	T_PBRE_STRN_LCOM() {Initialize();}
};

struct T_PBRE_SERV_LOCD
{
	T_PBRE_SERV_POSI PosR[C_PLATE_POS];
	void Initialize()
	{
		for(int i=0; i<C_PLATE_POS; ++i) PosR[i].Initialize();
	}
	T_PBRE_SERV_LOCD() {Initialize();}
};

// 하중조합별 결과
struct T_PBRE_SERV_LCOM
{
	T_PBRE_SERV_LOCD LDir[2];
	void Initialize()
	{
		for(int i=0; i<2; ++i) LDir[i].Initialize();
	}
	T_PBRE_SERV_LCOM() {Initialize();}
};

enum 
{
	EN_RCSC_FLEX_POSITIVE = 0,
	EN_RCSC_FLEX_NEGATIVE,
};
// 위치별 Critical 결과
struct T_PBRE_RES_POS
{
	T_PBRE_STRN_POSI FlexR[2]; // 0=Positive, 1=Negative
	T_PBRE_STRN_POSI SheaR;
	T_PBRE_STRN_POSI TorR;
	T_PBRE_SERV_POSI CrackR[2];
	T_PBRE_SERV_POSI DistR[2];   // Shrinkage and Temperature (배력철근)
	T_PBRE_SERV_POSI StressR[2]; // Stress (응력검토), rebar에 대한 응력
	T_PBRE_SERV_POSI StressC[2]; // Stress (응력검토), conc에 대한 응력
	T_PBRE_SERV_POSI DeflecR; // Deflection
	T_PBRE_STRN_POSI FatiR; // fatigue for Russia

	void Initialize()
	{
		for (int i=0; i<2; ++i) 
		{
			FlexR[i].Initialize();
			CrackR[i].Initialize();
			DistR[i].Initialize();
			StressR[i].Initialize();
			StressC[i].Initialize();
		}
		SheaR.Initialize();
		TorR.Initialize();
		DeflecR.Initialize();
		FatiR.Initialize();
	}
	T_PBRE_RES_POS() {Initialize();}
};

struct T_PBRE_RES_DIR
{
	T_PBRE_RES_POS PosR[C_PLATE_POS];
	
	void Initialize()
	{
		for(int i=0; i<C_PLATE_POS; ++i) PosR[i].Initialize();
	}
	T_PBRE_RES_DIR() {Initialize();}
};

struct T_PBRE_RES_SUBD
{
	T_PBRE_RES_DIR DirR[2]; // 0=X-Result, 1=Y-Result

	//통합설계엔진 산정된 배근 정보
	dgn::def::CSectionRebarPlate DgnRebar;

	void Initialize()
	{
		for(int i=0; i<2; ++i) DirR[i].Initialize();
		DgnRebar.Init();
	}
	T_PBRE_RES_SUBD() {Initialize();}
};


//////////////////////////////////////////////////////////////////////////
// Wall plate

struct T_PCRE_STRN_POSI
{
	T_PDEL_KEY   KeyD;
	T_PLTD_FORCE ForD;

	int    nAsRes;
	int    nAsResV;
	double dpPn;
	double dpPnmax;
	double dpMn;
	double dMuyd;
	double dpVc;
	double dpVs;
	double dpVn;
	double dRatM;
	double dRatV;
	double dSv_Req;
	double dSubBarDist;
	double dRatV_Code;

    double dv;
    double dpvc;
    double dpvs;
    double dpvn;
	
	double dRatA;
	double dAsApp;  // Applied As with Rebar
	double dAsReq;  // Required As if there are Rebars
	double dV_Rd;  //EC2 Shear Resistance  V_Rd = V_Rdc > V_Ed ? V_Rdc : V_Rds
	double dPMRat_M;
	double dPMRat_N;
	BOOL bOK;

	double dfd;  // Fatigue
	double dfr;
	double dRatF;

    BOOL   bChkLongAsByV; // for AASHTO Shear
    BOOL   bChkLongAsByT; // for AASHTO Torsion

	// neutral axis check.
	double dxd;
	double dxd_limit;

	void Initialize()
	{
		KeyD.Initialize();
		ForD.Initialize();
		nAsRes  = 0;
		nAsResV = 0;
		dpPn    = 0.0;
		dpPnmax = 0.0;
		dpMn    = 0.0;
		dMuyd   = 0.0;
		dpVc    = 0.0;
		dpVs    = 0.0;
		dpVn    = 0.0;

        dv      = 0.0;
        dpvc    = 0.0;
        dpvs    = 0.0;
        dpvn    = 0.0;

		dRatM   = 0.0;
		dRatV   = 0.0;
		dAsApp  = 0.0;
		dAsReq  = 0.0;
		dSv_Req = 0.0;
		dSubBarDist = 0.0;
		dRatA   = 0.0;
		dRatV_Code = 0.0;
		dV_Rd    = 0.0;
		bOK     = FALSE;
		dPMRat_M  = 0.0;
		dPMRat_N  = 0.0;

		dfd = 0.0;
		dfr = 0.0;
		dRatF = 0.0;

        bChkLongAsByV = TRUE;
        bChkLongAsByT = TRUE;

		dxd = 0.0;
		dxd_limit = 0.0;
	}
	T_PCRE_STRN_POSI() {Initialize();}
};

struct T_PCRE_STRN_POSI_UNIT
{
	T_PCRE_STRN_POSI_UNIT()
	{
		dpPn   = D_UNITSYS_BASE_FORCE;
		dpPnmax= D_UNITSYS_BASE_FORCE;
		dpMn   = D_UNITSYS_BASE_MOMENT;
		dMuyd  = D_UNITSYS_BASE_MOMENT;
		dpVc   = D_UNITSYS_BASE_FORCE;
		dpVs   = D_UNITSYS_BASE_FORCE;
		dpVn   = D_UNITSYS_BASE_FORCE;
		dAsApp = D_UNITSYS_BASE_AREA;
		dAsReq = D_UNITSYS_BASE_AREA;
	}
	int dpPn;
	int dpPnmax;
	int dpMn;
	int dMuyd;
	int dpVc;
	int dpVs;
	int dpVn;
	int dAsApp;
	int dAsReq;
};


struct T_PCRE_SERV_POSI
{
	T_PDEL_KEY   KeyD;
	T_PLTD_FORCE ForD;

	double dw;
	double dwa;
	double dfs;
	double ds;
	double dsa;
	double dAs;
	double dAs_req;
	BOOL bOK_Dist; // Shrinkage and Temperature (배력철근)
	//stress
	double dConSs;
	double dConSsa;
	double dStlSs;
	double dStlSsa;

	void Initialize()
	{
		KeyD.Initialize();
		ForD.Initialize();
		dw      = 0.0;
		dwa     = 0.0;
		dfs     = 0.0;
		ds      = 0.0;
		dsa     = 0.0;
		dAs     = 0.0;
		dAs_req = 0.0;
		bOK_Dist= FALSE;

		dConSs  = 0.0;
		dConSsa = 0.0;
		dStlSs  = 0.0;
		dStlSsa = 0.0;
	}
	T_PCRE_SERV_POSI() {Initialize();}
};

struct T_PCRE_SERV_POSI_UNIT
{
	T_PCRE_SERV_POSI_UNIT()
	{
		dw      = D_UNITSYS_BASE_LENGTH;
		dwa     = D_UNITSYS_BASE_LENGTH;
		dfs     = D_UNITSYS_BASE_STRESS;
		ds      = D_UNITSYS_BASE_LENGTH;
		dsa     = D_UNITSYS_BASE_LENGTH;
		dAs     = D_UNITSYS_BASE_AREA;
		dAs_req = D_UNITSYS_BASE_AREA;
	}
	int dw;
	int dwa;
	int dfs;
	int ds;
	int dsa;
	int dAs;
	int dAs_req;
};

struct T_PCRE_STRN_LOCD
{
	T_PCRE_STRN_POSI PosR[C_PLATE_POS];
	void Initialize()
	{
		for(int i=0; i<C_PLATE_POS; ++i) PosR[i].Initialize();
	}
	T_PCRE_STRN_LOCD() {Initialize();}
};

// 하중조합별 결과
struct T_PCRE_STRN_LCOM
{
	T_PCRE_STRN_LOCD LDir[2];
	void Initialize()
	{
		for(int i=0; i<2; ++i) LDir[i].Initialize();
	}
	T_PCRE_STRN_LCOM() {Initialize();}
};

struct T_PCRE_SERV_LOCD
{
	T_PCRE_SERV_POSI PosR[C_PLATE_POS];
	void Initialize()
	{
		for(int i=0; i<C_PLATE_POS; ++i) PosR[i].Initialize();
	}
	T_PCRE_SERV_LOCD() {Initialize();}
};

// 하중조합별 결과
struct T_PCRE_SERV_LCOM
{
	T_PCRE_SERV_LOCD LDir[2];
	void Initialize()
	{
		for(int i=0; i<2; ++i) LDir[i].Initialize();
	}
	T_PCRE_SERV_LCOM() {Initialize();}
};

// 위치별 Critical 결과
struct T_PCRE_RES_POS
{
	T_PCRE_STRN_POSI AxialR;
	T_PCRE_STRN_POSI FlexR[2]; // 0=Positive, 1=Negative
	T_PCRE_STRN_POSI SheaR;
	T_PCRE_SERV_POSI StressR[2]; //rebar에 대한 stress
	T_PCRE_SERV_POSI StressC[2]; //conc에 대한 stress
	T_PCRE_SERV_POSI CrackR[2];
	T_PCRE_SERV_POSI DistR[2];   // Shrinkage and Temperature (배력철근)
	T_PCRE_STRN_POSI FatiR; // fatigue for Russia

	void Initialize()
	{
		for (int i=0; i<2; ++i) 
		{
			FlexR[i].Initialize();
			StressR[i].Initialize();
			StressC[i].Initialize();
			CrackR[i].Initialize();
			DistR[i].Initialize();
		}
		SheaR.Initialize();
		AxialR.Initialize();
		FatiR.Initialize();
	}
	T_PCRE_RES_POS() {Initialize();}
};

struct T_PCRE_RES_DIR
{
	T_PCRE_RES_POS PosR[C_PLATE_POS];

	void Initialize()
	{
		for(int i=0; i<C_PLATE_POS; ++i) PosR[i].Initialize();
	}
	T_PCRE_RES_DIR() {Initialize();}
};

struct T_PCRE_RES_SUBD
{
	T_PCRE_RES_DIR DirR[2]; // 0=X-Result, 1=Y-Result
	
	//통합설계엔진 산정된 배근정보
	dgn::def::CSectionRebarPlate DgnRebar;

	void Initialize()
	{
		for(int i=0; i<2; ++i) DirR[i].Initialize();
		DgnRebar.Init();
	}
	T_PCRE_RES_SUBD() {Initialize();}
};

enum 
{
	RCSC_SBDO_BEAM_TYPE = 1,
	RCSC_SBDO_COLM_TYPE = 2,
};

#pragma pack()

#endif
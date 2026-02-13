#if !defined(__DGNSTRUCT_H__)
#define __DGNSTRUCT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\DB_ST_DN.h"
#include "..\dgnengine\src\DgnCalcBase\DgnCalcBase_MPhi_Struct.h"

// PMS:4020-Seungjun-20090710 Slab/Wall Design.
const int  LF = 0;
const int  RT = 1;
const int  TP = 2;
const int  BT = 3;

const int CONST_DGN_iPOSNUM	= 5;
const double cDgn_Zero	= 1.0E-07;
const double dDgn_LimF	= 1.0E-04;	// Force Limit.

// Steel.
// Add by GAY. PMS:4035. ('09.06.22). KSSC-LSD09 기준 추가.
// Add by GAY. PMS:4061. ('09.09.01). AISC-LRFD05, AISC-ASD05 기준 추가.
// Add by GAY. PMS:4064. ('09.09.16). TWN-ASD96, TWN-LSD96 기준 추가.
// Add by GAY. PMS:XXXX. ('13.01.15). AISC-LRFD10, AISC-ASD10 기준 추가.
// add by maxiao(2015-9-24)GB50017-15
// Add by tss(2017.8.2)GB50018-02
// Add by xuezc(2018/2/8)GB50017-17
// Add by tss(2021.07.15)JGJ209-2010
enum
{
	AIK_ASD83      = 1,  AISC_ASD89    = 2,   AISC_LRFD93    = 3,  AIK_LSD97   = 4,  KSCE_ASD96      = 5,
	BS5950_90      = 6,  Eurocode3     = 7,   AISI_CFSD86    = 8,  AIK_CFSD98  = 9,  AIJ_ASD02       =10,
	KEPCO97_1111   =11,  AISC_LRFD2K   =12,   GBJ17_88       =13,  CSA_S16_01  =14,  IS800_1984      =15,
	TWN_ASD90      =16,  TWN_LSD90     =17,   GB50017_03     =18,  BS5950_2K   =19,  KSSC_ASD03      =20,
	Eurocode3_05   =21,  IS800_2007    =22,   KSSC_LSD09     =23,  AISC_LRFD05 =24,  AISC_ASD05      =25,
	TWN_ASD96      =26,  TWN_LSD96     =27,   AISC_LRFD10    =28,  AISC_ASD10  =29,  AISI_CFSD08     =30,
	AASHTO_LS02    =51,  JTJ025_86     =52,   AASHTO_LS96    =53,  TWN_BRG_LS90=54,  AASHTO_AS96     =55,
	TWN_BRG_AS90   =56,  KSCE_ASD05    =57,   KSCE_RAIL_ASD04=58,  KR_BRG_LSD15=59,  KSCE_ASD10      =60,
	KSCE_RAIL_ASD11=61,  Eurocode3_2_05=62,   AASHTO_LS12    =63,  GB50017_15  =64,  Eurocode3_1_3_06=65,
	KSSC_LSD16     =66,  IRC24_2010    =67,   AASHTO_LS16    =68,  GB50018_02  =69,  GB50017_17      =70,
	AISC_LRFD16    =71,  AISC_ASD16    =72,   CSA_S6_14_ST =73,    AASHTO_LS17 =74,	 GB51249_2017    =75,
	KDS_41_31_2019 =76,  JGJ209_2010   =77,   KDS_41_30_10_2022=78, KDS_24_14_30_2019=79, NSCP_2015_S_LRFD = 80,
	NSCP_2015_S_ASD=81,  KDS_24_14_31_2018=82,IRS_SBC=83,          AASHTO_LS20 =84,  JROAD_H14       =85,
	JROAD_H24     =86,   CSA_S6_19_ST =87,    SP_16_13330_2017=88, AISC_LRFD22 =89,  AISC_ASD22      =90,
	GB50018_25 = 91,     AASHTO_LS24  =92,    JROAD_H29      =93,  JROAD_R07   =94,
};

// RC.
// Change, Jaeoh (06.12.08) ex) 1 -> 101
// Add by GAY. PMS:4267. ('11.12.09). GB/T50010-10 기준 추가.
// Add by GAY. PMS:4450. ('12.03.12). ACI318-08, 11 기준 추가.
enum
{
	ACI318_89   = 101,  AIK_USD94      = 102,  ACI318_95          = 103,  KSCE_USD96   = 104,  KCI_USD99  = 105,
	ACI318_99   = 106,  AIK_WSD2K      = 107,  AIJ_WSD99          = 108,  BS8110_97    = 109,  Eurocode2  = 110,
	GB50010_02  = 111,  ACI318_02      = 112,  CSA_A233_94        = 113,  IS456_2000   = 114,  TWN_USD92  = 115,
	KCI_USD03	= 116,  KSCE_RC_USD05  = 117,  KSCE_RC_RAIL_USD04 = 118,  Eurocode2_04 = 119,  ACI318_05  = 120,
	KCI_USD07   = 121,  Eurocode2_2_05 = 122,  KCI_USD07_CVL      = 123,  KCI_USD12    = 124,  TWN_USD100 = 125,
	GB50010_10  = 126,  ACI318_08      = 127,  ACI318_11          = 128,  NSR_10       = 129,  ACI318_14  = 130,
	ACI318M_14  = 131,  GB50010_19	   = 132,  KDS_41_30_2018     = 133,  NSCP_2015_C  = 134,  KDS_41_20_2022 = 135,
	ACI318_19   = 136,  ACI318M_19     = 137,  TWN_USD112         = 138,  NTC_DCEC_2017= 139,  SP_63_1330_2018 = 140,
	NTC_DCEC_2023 = 141,
	// Gen은 여기 뒤에 추가!
	// 151부터 Civil!
	AASHTO_LC02 = 151,  JTJ023_85      = 152,  IRC_21_00         = 153,  CSA_S6_00    = 154,  AASHTO_LC96= 155,
	TWN_BRG_LC90= 156,  AASHTO_LC07    = 157,  KR_LRFD11_RC      = 158,  KSCE_RC_USD10= 159,  KSCE_RC_RAIL_USD11 = 160,
	AASHTO_LC12 = 161,  SNiP_20503_84RC= 162,  SP_35_13330_11RC  = 163,  SNiP_20503_84RC_MKS= 164,  SP_35_13330_11RC_MKS= 165,
	AASHTO_LC16 = 166,  AASHTO_LC17    = 167,  KDS_24_14_21_2022 = 168,
	IRC_112_11  = 179,	CSA_S6_14_RC   = 180,
	IRS_RC		= 181,  IRC_112_20     = 182, AASHTO_LC20        = 183, BS5400_4_90 = 184, CSA_S6_19_RC  = 185,
	AREMA23     = 186,  ACI318_25      = 187, ACI318M_25         = 188, TMH07_89    = 189, AS5100_5_2017 = 190,
	KDS_14_20_00_2022 = 191, AASHTO_LC24 = 192,
};

enum
{
	RECOMMENDED = 1001, ITALY       = 1002, UK         = 1003, SWEDEN = 1004, SINGAPORE     = 1005,
	SWEDEN_2019 = 1006, AUSTRIA     = 1007, BELGIUM    = 1008, CYPRUS = 1009, CZECHREPUBLIC = 1010,
	DENMARK     = 1011, NETHERLANDS = 1012, FINLAND    = 1013, FRANCE = 1014, GERMANY       = 1015,
	GREECE      = 1016, IRELAND     = 1017, LUXEMBOURG = 1018, NORWAY = 1019, POLAND        = 1020,
	ROMANIA     = 1021, SLOVAKIA    = 1022, SLOVENIA   = 1023, SPAIN  = 1024, MALAYSIA      = 1025,
};

// SRC.
enum
{
	SSRC79=201, AIK_SRC2K=202, AIJ_SRC01=203, JGJ138_01=204, TWN_SRC92=205, TWN_SRC100=206
};

// PSC See wg_db\DB_ST_DN.h at struct T_PSCD_D.
//enum {JSCE02=1, KSCE_USD03=2, JTG_D62_04=3};

// Coded by Parkhj : 기준별 단위계 구분
enum
{
	CODEUNIT_US=1, CODEUNIT_SI=2, CODEUNIT_MKS=3
};

///////////////////////////////////////////////
// Coded by Seungjun ('20070126) MNet:No.2619 For Taiwan. Design cosidered the compression rebars.
enum
{
	DRBD_None = 0, DRBD_DCRB = 1, DRBD_DCBB = 2
};  // None, Design Creteria for rebars, Design Creteria for rebars by member.

// Coded by Seungjun ('20070807) MNet:No.2921.
enum
{
	RCS_BDGN=11, RCS_BCHK, RCS_BDUC,
	RCS_CDGN=21, RCS_CCHK, RCS_CDUC,
	RCS_RDGN=31, RCS_RCHK,
	RCS_WDGN=41, RCS_WCHK,
	RCS_PDGN=51, RCS_PCHK,
	RCS_SCHK=62,
	RCS_HDGN=71,
	RCS_SDGN=81, RCS_SLCK=82, // Slab Design, Checking
	RCS_SSCK=84, RCS_SSEV=85, RCS_SWAF=89,  // Slab Shear checking, Serviceability checking, wood-armer force.
	RCS_MWDN=91, RCS_MWCK=92, RCS_MWWM=99,  // Meshed wall design, checking, force.
	SRC_BDGN=101, SRC_BCHK,
	SRC_CDGN=111, SRC_CCHK,
	RCS_ISDN=301, RCS_ISCK,// Inclined slab design, checking.
	RCS_JFDD=401, RCS_JFDC,
	RCS_STDN=501, RCS_STCK, RCS_STFO, // Strip Design, Strip Checking, Strip Force
	RCS_SHFD=701, RCS_SHFC, RCS_SHSC = 704, // [SHFD] SHell Flexural Design, [SHFC] SHell Flexural Checking, [SHSC] SHell Shear Checking
	RCS_ABDG=801, RCS_ABCH, // Anchor Bolt
};   // JFD Footing Design, Checking.

// SHJUNG-20100315 (주의 : TBAMRUDTData.cpp에서 RCS_BDGN, RCS_BCHK, RCS_PDGN, RCS_PCHK, RCS_HDGN, SRC_BCHK 이 숫자를 사용 하고 있음)
// 숫자를 수정 하는 경우에 TBAMRUDTData.cpp의 RCS_WDGN 등을 같이 변경 하여 주세요.

enum
{
	STL_CHK_ASD=201, STL_CHK_LRFD=202
};

enum
{
	CFSTL_CHK=301
};

enum
{
	DGN_TABLE=1, DGN_GRAPHIC=2, DGN_DETAIL=3, DGN_SUMMARY=4
};

enum
{
	RC_BEAM =11, RC_COLUMN=12, RC_BRACE=13, RC_WALL=14,
	STEEL   =21,
	SRC_BEAM=31, SRC_COLUMN=32,
	RC_FOOTING=41,
	CFSTEEL =51
};

enum
{
	RCS_BDGN_FLX=11, RCS_BDGN_SHR=12,
	RCS_CDGN_FLX=21, RCS_CDGN_SHR=22,
	RCS_WDGN_FLX=31, RCS_WDGN_SHR=32,
	RCS_PDGN_FLX=41, RCS_PDGN_SHR=42
};

enum
{
	STL_ACS=11
}; // 나중에 더 추가될 수 있음.

enum
{
	CMD_LOADCASE=11, CMD_LOADCOMB=12
};

enum
{
	RCS_SLAB_POS_INTERNAL = 0, // internal
	RCS_SLAB_POS_SIDE_T   = 1, // Side Top
	RCS_SLAB_POS_SIDE_B   = 2, // Side Bottom
	RCS_SLAB_POS_SIDE_L   = 3, // Side Left
	RCS_SLAB_POS_SIDE_R   = 4, // Side Right
	RCS_SLAB_POS_COR_LT   = 5, // Corner Left-Top
	RCS_SLAB_POS_COR_LB   = 6, // Corner Left-Bottom
	RCS_SLAB_POS_COR_RB   = 7, // corner Right-Bottom
	RCS_SLAB_POS_COR_RT   = 8, // Corner Right-Top
};

enum 
{
	EN_DGNAXIS_LY = 1,
	EN_DGNAXIS_LZ
};

enum EN_EC8_SEISCLASS
{
	EN_EC8_NONE,
	EN_EC8_DCH,
	EN_EC8_DCM,
	EN_EC8_DCL
};

enum EN_EC8_STRUCT_TYPE
{
	EN_EC8_FRAME_SYSTEM = 0,
	EN_EC8_DUAL_SYSTEM,
	EN_EC8_COUPLED_WALL_SYSTEM,
	EN_EC8_UNCOUPLED_WALL_SYSTEM,
	EN_EC8_TORSIONAL_FLEX_SYSYEM,
	EN_EC8_INVERTED_PENDULUM_SYSTEM,
};

#define		DGN_PUNCH_SHEAR_DIV			6
/////////////////////////////////////////////////////////////////////////////
// Define Plate Member Force/Moment Components             //////////////////
/////////////////////////////////////////////////////////////////////////////
#define	 PFORC_COMPONENT_FXX  0
#define	 PFORC_COMPONENT_FYY  1
#define	 PFORC_COMPONENT_FXY  2
#define	 PFORC_COMPONENT_FMX  3
#define	 PFORC_COMPONENT_FMN  4
#define	 PFORC_COMPONENT_FVT  5
#define	 PFORC_COMPONENT_MXX  6
#define	 PFORC_COMPONENT_MYY  7
#define	 PFORC_COMPONENT_MXY  8
#define	 PFORC_COMPONENT_MMX  9
#define	 PFORC_COMPONENT_MMN  10
#define	 PFORC_COMPONENT_MVT  11
#define	 PFORC_COMPONENT_VXX  12
#define	 PFORC_COMPONENT_VYY  13
#define  PFORC_COMPONENT_FMX_ABS 14
#define  PFORC_COMPONENT_MMX_ABS 15
#define  PFORC_COMPONENT_VMX_ABS 16
#define  PFORC_COMPONENT_MOMENT_COMB 17 // 20061103 : Moment Combination
#define  PFORC_COMPONENT_SMX_TOP 18  // MAXIMUM PRINCIPAL STRESS IN TOP SIDE.
#define  PFORC_COMPONENT_SMN_TOP 19  // MINIMUM PRINCIPAL STRESS IN TOP SIDE.
#define  PFORC_COMPONENT_SMX_BOT 20  // MAXIMUM PRINCIPAL STRESS IN BOTTOM SIDE.
#define  PFORC_COMPONENT_SMN_BOT 21	 // MINIMUM PRINCIPAL STRESS IN BOTTOM SIDE.

enum EN_PSTRS_POS
{
	EN_PSTRS_TOP = 0,
	EN_PSTRT_BOT
};

enum EN_SHEL_DIR
{
	EN_SHEL_DIR_X = 0,
	EN_SHEL_DIR_Y,
};

enum EN_SHEL_REINF_TYPE
{
	EN_SHEL_REINF_UNCRACK = 0,
	EN_SHEL_REINF_NONE,
	EN_SHEL_REINF_DIRX,
	EN_SHEL_REINF_DIRY,
	EN_SHEL_REINF_BOTH,
};

enum EN_SHEL_CRD_TYPE
{
	EN_SHEL_CRD_FCD,
	EN_SHEL_CRD_SIGCDMAX
};

enum EN_DGNSBDO_TYPE
{
	EN_DGNSBDO_ALL     = 0,
	EN_DGNSBDO_SLAB    = 1,
	EN_DGNSBDO_MAT     = 2,
	EN_DGNSBDO_WALL    = 3,
	EN_DGNSBDO_SLABMAT = 4,
	EN_DGNSBDO_INSLAB  = 5,
	EN_DGNSBDO_SHELL   = 6
};

enum EN_PUNCHING_POS
{
	EN_PUNCHING_INNER = 0,
	EN_PUNCHING_SIDE_T,
	EN_PUNCHING_SIDE_B,
	EN_PUNCHING_SIDE_L,
	EN_PUNCHING_SIDE_R,
	EN_PUNCHING_CORNER_BR,
	EN_PUNCHING_CORNER_TR,
	EN_PUNCHING_CORNER_TL,
	EN_PUNCHING_CORNER_BL
};

enum
{
	EN_SUPPORT_PIN_PIN = 0,
	EN_SUPPORT_FIX_FIX,
	EN_SUPPORT_FIX_PIN,
	EN_SUPPORT_CANTI,
	EN_SUPPORT_NUM,
};


#define   VALUE_OPT_MAX         0
#define   VALUE_OPT_CENTER      1

////////////////////DataCtrl///////////////////
struct _DGN_REDU
{
	int iLcasNo;
	CString sLcasNa;
	CString sLcasType;
	CString sDescript;

	void Initialize()
	{
		iLcasNo=0;
		sLcasNa=_T("");
		sLcasType=_T("");
		sDescript=_T("");
	}
};

struct _DGN_RBAR
{
	double dY;
	double dZ;

	void Initialize()
	{
		dY=0.0; dZ=0.0;
	}
};

struct _DGN_TNDN	// Tendon.
{
	int iTdnaKey;
	int iTndnType;  // 1:Internal, 2:External
	int iBondType;	// 0=Bonded, 1=Unbonded.
	int iLoadType;	// 0=Pre-tension, 1=Post-tension.
	double dEps;
	double dfpu;
	double dfpy;
	double dAps;
	double dDia;
	double dyz[2];
	double dJackForceTran;	// Jacking Force at Transfer Stage.
	double dJackForceServ;	// Jacking Force at Service Load Stage.
	double dRotRad;					// Rotate degree of tendon (radian).
	
	void Initialize()
	{
		iTdnaKey=0;
		iTndnType=0;
		iBondType=0;
		iLoadType=0;
		dEps=0.0;
		dfpu=0.0; 
		dfpy=0.0;
		dAps=0.0;
		dDia=0.0;
		dyz[0]=0.0; 
		dyz[1]=0.0;
		dJackForceTran=0.0;
		dJackForceServ=0.0;
		dRotRad=0.0;
	}
};

struct _DGNC_RBAR
{
	int iSectNo;
	// 3:Site, 5:BarLay
	int iBarLayc;
	int iBarLayt;
	int iBarLayb;
	CString strSubBarNa;
	double dAsc;
	double dAst;
	double dAsb;
	double dDc[5];
	double dDt[5];
	double dDb[5];
	BOOL bCheckCovc[5];
	BOOL bCheckCovt[5];
	BOOL bCheckCovb[5];
	int iBarNum[3][5];
	CString strBarNa1[3][5];
	CString strBarNa2[3][5];
	BOOL bCheckRebar[3][5];
	BOOL bCheckSpace[3][5];

	void Initialize()
	{
		iSectNo=0;
		iBarLayc=0;
		iBarLayt=0;
		iBarLayb=0;
		strSubBarNa=_T("");
		dAsc=0.0;
		dAst=0.0; 
		dAsb=0.0;
		for ( int i=0; i<5; ++i )
		{
			dDc[i]=0.0;
			dDt[i]=0.0;
			dDb[i]=0.0;
			bCheckCovc[i] = FALSE;
			bCheckCovt[i] = FALSE;
			bCheckCovb[i] = FALSE;
			for ( int j=0; j<3; ++j )
			{
				iBarNum[j][i] = 0;
				strBarNa1[j][i] = _T("");
				strBarNa2[j][i] = _T("");
				bCheckRebar[j][i] = FALSE;
				bCheckSpace[j][i] = FALSE;
			}
		}
	}
};

struct _DGNC_CPPL_SINGLE_D  //해당 Plate 설계 영역의 면에 대한 철근정보(상하구분없이 한측면만의 정보) : RC Plate 설계시 계산 및 삽도작업을 위해 가공된 Data
{
	BOOL    bNum;         // 개수 ,CTC default=TRUE
	double  dDt[5];       // 피복두께 (5:Layer)
	double  dBarNum[5];   // 주철근 개수
	double  dBarCTC[5];   // 주철근 간격
	CString strBarNa1[5]; // Top Size1 철근 직경
	CString strBarNa2[5]; // Top Size2 철근 직경
	double  dSubBarNum;   // 전달철근 다리수 (default=2)
	double  dSubBarDist;  // 전단철근 간격
	CString strSubBarNa;  // 전단철근 직경

	void Initialize()
	{
		bNum = TRUE;
		dSubBarNum  = 0.;
		dSubBarDist = 0.;
		strSubBarNa = _T("");
		for ( int i=0; i<5; ++i )
		{
			dDt[i]       = 0.0;
			dBarNum[i]   = 0.0;
			dBarCTC[i]   = 0.0;
			strBarNa1[i] = _T("");
			strBarNa2[i] = _T("");
		}
	}
	_DGNC_CPPL_SINGLE_D() {}
	_DGNC_CPPL_SINGLE_D(const _DGNC_CPPL_SINGLE_D& src)	
	{
		*this = src;
	}
	_DGNC_CPPL_SINGLE_D& operator = (const _DGNC_CPPL_SINGLE_D& src)
	{
		Initialize();
		bNum = src.bNum;        
		dSubBarNum  = src.dSubBarNum ;
		dSubBarDist = src.dSubBarDist;
		strSubBarNa = src.strSubBarNa;
		for ( int i=0; i<5; ++i )
		{
			dDt[i]       = src.dDt[i];
			dBarNum[i]   = src.dBarNum[i];
			dBarCTC[i]   = src.dBarCTC[i];
			strBarNa1[i] = src.strBarNa1[i];
			strBarNa2[i] = src.strBarNa2[i];
		}
		return *this;
	}
};

struct _DGNC_CPPL_D  //해당 Plate 설계 영역의 면에 대한 철근정보 : RC Plate 설계시 계산 및 삽도작업을 위해 가공된 Data
{
	BOOL    bNum;            // 개수 ,CTC default=TRUE
	double  dDt[5];          // 5:Layer
	double  dDb[5];          // Bottom에서 거리
	double  dTopBarNum[5];   // Top 철근 개수
	double  dBotBarNum[5];   // Bottom 철근 개수
	double  dTopBarCTC[5];   //Top철근 간격
	double  dBotBarCTC[5];   //Bottom철근 간격
	CString strTopBarNa1[5]; // Top Size1 철근 직경
	CString strTopBarNa2[5]; // Top Size2 철근 직경
	CString strBotBarNa1[5]; // Bottom Size1 철근 직경
	CString strBotBarNa2[5]; // Bottom Size2 철근 직경
	double  dSubBarNum;      // 전달철근 다리수 (default=2)
	double  dSubBarDist;     // 전단철근 간격
	CString strSubBarNa;     // 전단철근 직경

	void Initialize()
	{
		bNum = TRUE;        
		dSubBarNum  = 0.0;
		dSubBarDist = 0.0;
		strSubBarNa = _T("");
		for ( int i=0; i<5; ++i )
		{
			dDt[i] = 0.0;
			dDb[i] = 0.0;
			dTopBarNum[i]   = 0.0;
			dBotBarNum[i]   = 0.0;
			dTopBarCTC[i]   = 0.0;
			dBotBarCTC[i]   = 0.0;
			strTopBarNa1[i] = _T("");
			strTopBarNa2[i] = _T("");
			strBotBarNa1[i] = _T("");
			strBotBarNa2[i] = _T("");
		}
	}
	_DGNC_CPPL_D() {}
	_DGNC_CPPL_D(const _DGNC_CPPL_D& src) 
	{
		*this = src;
	}
	_DGNC_CPPL_D& operator = (const _DGNC_CPPL_D& src)
	{
		Initialize();
		bNum = src.bNum;
		dSubBarNum  = src.dSubBarNum ;
		dSubBarDist = src.dSubBarDist;
		strSubBarNa = src.strSubBarNa;
		for ( int i=0; i<5; ++i )
		{
			dDt[i]          = src.dDt[i];
			dDb[i]          = src.dDb[i];
			dTopBarNum[i]   = src.dTopBarNum[i];
			dBotBarNum[i]   = src.dBotBarNum[i];
			dTopBarCTC[i]   = src.dTopBarCTC[i];
			dBotBarCTC[i]   = src.dBotBarCTC[i];
			strTopBarNa1[i] = src.strTopBarNa1[i];
			strTopBarNa2[i] = src.strTopBarNa2[i];
			strBotBarNa1[i] = src.strBotBarNa1[i];
			strBotBarNa2[i] = src.strBotBarNa2[i];
		}
		return *this;
	}

	void Set_Single(const _DGNC_CPPL_SINGLE_D& cpplSingleD)
	{
		Initialize();
		bNum=cpplSingleD.bNum;        
		dSubBarNum  =cpplSingleD.dSubBarNum ;
		dSubBarDist =cpplSingleD.dSubBarDist;
		strSubBarNa =cpplSingleD.strSubBarNa;
		for ( int i=0; i<5; ++i )
		{
			dDt[i]         =cpplSingleD.dDt[i]      ;
			dDb[i]         =cpplSingleD.dDt[i]      ;
			dTopBarNum[i]  =cpplSingleD.dBarNum[i]  ;
			dBotBarNum[i]  =cpplSingleD.dBarNum[i]  ;
			dTopBarCTC[i]  =cpplSingleD.dBarCTC[i]  ;
			dBotBarCTC[i]  =cpplSingleD.dBarCTC[i]  ;
			strTopBarNa1[i]=cpplSingleD.strBarNa1[i];
			strTopBarNa2[i]=cpplSingleD.strBarNa2[i];
			strBotBarNa1[i]=cpplSingleD.strBarNa1[i];
			strBotBarNa2[i]=cpplSingleD.strBarNa2[i];
		}
	}
	void Get_Single(BOOL bTop, _DGNC_CPPL_SINGLE_D& cpplSingleD)
	{
		cpplSingleD.Initialize();
		cpplSingleD.bNum=bNum;
		
		cpplSingleD.dSubBarNum  = dSubBarNum ;
		cpplSingleD.dSubBarDist = dSubBarDist;
		cpplSingleD.strSubBarNa = strSubBarNa;
		for ( int i=0; i<5; ++i )
		{
			if(bTop)
			{
				cpplSingleD.dDt[i]       = dDt[i]         ;
				cpplSingleD.dBarNum[i]   = dTopBarNum[i]  ;
				cpplSingleD.dBarCTC[i]   = dTopBarCTC[i]  ;
				cpplSingleD.strBarNa1[i] = strTopBarNa1[i];
				cpplSingleD.strBarNa2[i] = strTopBarNa2[i];
			}
			else
			{
				cpplSingleD.dDt[i]       = dDb[i]         ;
				cpplSingleD.dBarNum[i]   = dBotBarNum[i]  ;
				cpplSingleD.dBarCTC[i]   = dBotBarCTC[i]  ;
				cpplSingleD.strBarNa1[i] = strBotBarNa1[i];
				cpplSingleD.strBarNa2[i] = strBotBarNa2[i];
			}
		}
	}
};

/////////////////////ForceCtrl//////////////////////////

struct _DGN_LCOM
{
	BOOL bMaxLcom;
	int DesignLcomKey;
	CString DesignLcomNa;
	int OriginalLcomKey;
	int OriginalLcomNo;
	int nActive;
	BOOL bLongTerm;
	int iDgnLcomType;
	int iSeismicType;
	double dExtraRat;	// for PSC Design.
	// Add by GAY. MNET:2683. ('07.05.15). Load Component Type. (1~7:FX,FY,FZ,MX,MY,MZ | 0:Not Consider Concurrent Force.)
	int iLcomCompType;
	T_LCOM_D_UL LcomUlData;
	// Add, Jaeoh. 2008.02.19 For KR,Rating
	int iRatingCase;
	int iMoveLcaseType;
	int iServiceLcomType; // Add, Jaeoh. [11/17/2009] For PSC EUROCODE
	int nStrnSubType;     // SNiP, SP35 경우 Strength에 Sub type 추가 이진우 bj님 요청
	int nFatigueType;
	double dFactorDeadCS;
	CArray<BOOL,BOOL&> aRatChkStatic; // Civil Rating 사용
	double dFactorER[3];    // CSA, Before 부재력중 바닥판이 굳기 전에 하중을 적용하기 위해
	int nLcomKind;
	double dAllowableStressFactor;
	
	void Initialize()
	{
		bMaxLcom=FALSE;
		DesignLcomKey=0;
		DesignLcomNa=_T("");
		OriginalLcomKey=0;
		OriginalLcomNo=0;
		nActive=0;
		bLongTerm=FALSE;
		iDgnLcomType=0;
		iSeismicType=0;
		dExtraRat=0.0;
		iLcomCompType=0;
		LcomUlData.Initialize();
		iRatingCase=0;
		iMoveLcaseType=0;
		iServiceLcomType=0;
		nStrnSubType=-1;
		nFatigueType=-1;
		dFactorDeadCS = 1.0;
		aRatChkStatic.RemoveAll();
		for ( int i=0; i<3; ++i )
		{
			dFactorER[i] = 1.0;
		}
		nLcomKind = 0;
		dAllowableStressFactor = 1.0;
	}
	_DGN_LCOM() 
	{
		Initialize(); 
	}
	_DGN_LCOM(const _DGN_LCOM& src)
	{
		*this = src;
	}
	_DGN_LCOM& operator=(const _DGN_LCOM& src)
	{
		bMaxLcom         = src.bMaxLcom;
		DesignLcomKey    = src.DesignLcomKey;
		DesignLcomNa     = src.DesignLcomNa;
		OriginalLcomKey  = src.OriginalLcomKey;
		OriginalLcomNo   = src.OriginalLcomNo;
		nActive          = src.nActive;
		bLongTerm        = src.bLongTerm;
		iDgnLcomType     = src.iDgnLcomType;
		iSeismicType     = src.iSeismicType;
		dExtraRat        = src.dExtraRat;
		iLcomCompType    = src.iLcomCompType;
		LcomUlData       = src.LcomUlData;
		iRatingCase      = src.iRatingCase;
		iMoveLcaseType   = src.iMoveLcaseType;
		iServiceLcomType = src.iServiceLcomType;
		nStrnSubType     = src.nStrnSubType;
		nFatigueType     = src.nFatigueType;
		dFactorDeadCS    = src.dFactorDeadCS;
		aRatChkStatic.Copy(src.aRatChkStatic);
		for ( int i=0; i<3; ++i )
		{
			dFactorER[i] = src.dFactorER[i];
		}
		nLcomKind = src.nLcomKind;
		dAllowableStressFactor = src.dAllowableStressFactor;
		return *this;
	}

	CString Get_DesignLcomNa(BOOL bDetail)
	{
		CString outDesignLcomNa = DesignLcomNa;
		if(bDetail)
		{
			int nSize = outDesignLcomNa.GetLength();
			if(nSize > 0)
			{
				TCHAR endChar = outDesignLcomNa.GetAt(nSize-1);
				if(endChar == '+')
				{
					outDesignLcomNa = outDesignLcomNa.Left(nSize-1) + _T("max");
				}
				else if(endChar == '-')
				{
					outDesignLcomNa = outDesignLcomNa.Left(nSize-1) + _T("min");
				}
			}
		}
		return outDesignLcomNa;
	}
	//(2007.12.13) Add by Unsang :: Excel 출력용 하중조합이름.
	CString Get_LcomName()
	{
		CString strDesc = _T("");
		int nSize = DesignLcomNa.GetLength();
		if(nSize > 0)
		{
			TCHAR endChar = DesignLcomNa.GetAt(nSize-1);
			if(endChar == '+')
			{
				strDesc = _T("(max)");
			}
			else if(endChar == '-')
			{
				strDesc = _T("(min)");
			}
		}
		return LcomUlData.LoadCombName + strDesc;
	}
};

typedef CArray<_DGN_LCOM, _DGN_LCOM&> _DGN_LCOM_LIST;

struct _DGN_LCTB_POS	// Load Contribution.
{
	double dRemnFxx;
	double dRemnFyy;
	double dRemnFzz;
	double dRemnBMx;
	double dRemnBMy;
	double dRemnBMz;
	// Add by GAY. ('13.05.28). PMS:4632. Member force for 7DOF.
	// Bi-Moment, Twisting Moment, Warping Moment.
	double dRemnMbi;
	double dRemnTst;
	double dRemnTwp;
	double dRemnFxxUF;
	double dRemnFyyUF;
	double dRemnFxyUF;
	double dRemnVxxUF;
	double dRemnVyyUF;
	double dRemnMxxUF;
	double dRemnMyyUF;
	double dRemnMxyUF;

	void Initialize()
	{
		dRemnFxx = 0.0;
		dRemnFyy = 0.0;
		dRemnFzz = 0.0;
		dRemnBMx = 0.0;
		dRemnBMy = 0.0;
		dRemnBMz = 0.0;
		dRemnMbi = 0.0;
		dRemnTst = 0.0;
		dRemnTwp = 0.0;
		dRemnFxxUF = 0.0;
		dRemnFyyUF = 0.0;
		dRemnFxyUF = 0.0;
		dRemnVxxUF = 0.0;
		dRemnVyyUF = 0.0;
		dRemnMxxUF = 0.0;
		dRemnMyyUF = 0.0;
		dRemnMxyUF = 0.0;
	}
};

struct _DGN_LCTB_END	// Load Contribution.
{
	double dRemnBMye[2];
	double dRemnBMze[2];

	void Initialize()
	{
		for(int i=0; i<2; ++i)
		{
			dRemnBMye[i]=0.0;
			dRemnBMze[i]=0.0;
		}
	}
};

// Coded by Seungjun MNet:No.2393 ('20060816)
// Serviceability Limit State
struct _DGN_DISP
{
	int LcomKey;
	int RefPointKey;
	double MaxDistX;
	double MaxDispX;
	double MaxDistY;
	double MaxDispY;
	double MaxDistZ;
	double MaxDispZ;

	void Initialize()
	{
		LcomKey = 0;
		RefPointKey = 0;
		MaxDistX = 0.0;
		MaxDistY = 0.0;
		MaxDistZ = 0.0;
		MaxDispX = 0.0;
		MaxDispY = 0.0;
		MaxDispZ = 0.0;
	}
	_DGN_DISP& operator=(const _DGN_DISP& src)
	{
		LcomKey     = src.LcomKey;
		RefPointKey = src.RefPointKey;
		MaxDistX    = src.MaxDistX;
		MaxDistY    = src.MaxDistY;
		MaxDistZ    = src.MaxDistZ;
		MaxDispX    = src.MaxDispX;
		MaxDispY    = src.MaxDispY;
		MaxDispZ    = src.MaxDispZ;
		return *this;
	}
};

// Coded by Seungjun MNet:No.2393 ('20060828)
// Serviceability Limit State - Temp.
struct _DGN_SERV_LIM
{
	int iDeflection;   // All
	double dDefLim;    // All
	int iCrack;        // RC Only
	double dCrackLim;  // RC Only

	void Initialize()
	{
		iDeflection = 0;
		dDefLim     = 0.0;
		iCrack      = 0;
		dCrackLim   = 0.0;
	}
	_DGN_SERV_LIM& operator=(const _DGN_SERV_LIM& src)
	{
		iDeflection = src.iDeflection;
		dDefLim     = src.dDefLim;
		iCrack      = src.iCrack;
		dCrackLim   = src.dCrackLim;
		return *this;
	}
};

#define REBAR_KEY unsigned int

// Bridge Load Rating Design Rating CASE
struct BLRD_RCASE
{
	int DesignBlrcKey;
	CString DesignBlrcNa;
	int OriginalBlrcKey;
	int OriginalBlrcNo;
	BOOL bDgnBlrcType[3]; // DC, DW, T
	int iDgnLcomType;     // 1=FX-MAX,2=FX-MIN,...12=MZ-MIN.

	void Initialize()
	{
		DesignBlrcKey = 0;
		DesignBlrcNa  = _T("");
		OriginalBlrcKey = 0;
		OriginalBlrcNo = 0;
		for ( int i=0; i<3; ++i )
		{
			bDgnBlrcType[i] = TRUE;
		}
		iDgnLcomType = 0;
	}
	BLRD_RCASE& operator=(const BLRD_RCASE& src)
	{
		DesignBlrcKey   = src.DesignBlrcKey;
		DesignBlrcNa    = src.DesignBlrcNa;
		OriginalBlrcKey = src.OriginalBlrcKey;
		OriginalBlrcNo  = src.OriginalBlrcNo;
		for ( int i=0; i<3; i++ )
		{
			bDgnBlrcType[i] = src.bDgnBlrcType[i];
		}
		iDgnLcomType = src.iDgnLcomType;
		return *this;
	}
};

// Add by ZINU('06.09.08). NO:2340. Consider Factor for Lcase(RS) at GB50010-02 (전중비, 0.2Q0, 박약층).
#define _DGN_SHRR_KEY unsigned int

union _DGN_SHRR_K	// Shear Ratio.
{
	_DGN_SHRR_KEY KeyMap;
	struct
	{
		unsigned int LcasK : 12;   // 0-4096
		unsigned int ElemK : 20;   // 0-1048576
	}Key;
};

// Add by ZINU('06.09.11). NO:2340. Consider Factor for Lcase(RS) at GB50010-02 (전중비, 0.2Q0, 박약층).
struct _DGN_SHRR_BASE	// Shear Ratio Data (Only GB50010-02).
{
	double dRatAx;
	double dRatMy;
	double dRatMz;
	double dRatVy;
	double dRatVz;

	void Initialize()
	{
		dRatAx=1.0;
		dRatMy=1.0;
		dRatMz=1.0;
		dRatVy=1.0;
		dRatVz=1.0;
	}
};

struct _DGN_SHRR_D	// Shear Ratio Data (Only GB50010-02).
{
	// 0 = 박약층 & 층전단재하능력.
	// 1 = 전중비 Shear/WeightSum.
	// 2 = (0.2*Q0)/Qi.
	_DGN_SHRR_BASE ShrrBase[3];

	void Initialize()
	{
		for ( int i=0; i<3; i++ )
		{
			ShrrBase[i].Initialize();
		}
	}
};

// Coded by Seungjun MNet:No.2591 ('20061129) Bending moments by Earthquake Load For SCWB Design.
// ULONG64 : unsigned 64bit integer.
// **** Causion ****
// If you used 64bit union, you must make two 16bit unions on the structure.
// If you didn't make two 16bit unions, 64bit union always has wrong value.
// And you can't find this in Debug. In Win32 Debug 64bit union has correct value.
#define _DGN_SCWB_KEY ULONG64
union _DGN_SCWB_K
{
	_DGN_SCWB_KEY KeyMap; // unsigned 64bit int

	struct
	{
		union  // unsigned 16bit int
		{
			unsigned int ElemInfo;

			struct
			{
				unsigned int ElemK    : 26;  // 0~67,108,864
				unsigned int PosiK    :  3;  // 0~7 : 0:I-end, 1:J-end
				unsigned int MembType :  3;  // Member Type
			}ElemD;
		}_ElemInfo;

		union  // unsigned 16bit int
		{
			unsigned int LoadInfo;

			struct
			{
				unsigned int LcaseType : 17;  // 0~131,071
				unsigned int LcaseK    : 15;  // 0~32,768
			}LoadD;
		}_LoadInfo;
	}Key;
};

struct _DGN_SCWB_D
{
	double dPu;
	double dMyEQ;
	double dMzEQ;
	T_NODE_K NodeK;

	void Initialize()
	{
		dPu   = 0.0;
		dMyEQ = 0.0;
		dMzEQ = 0.0;
		NodeK = 0;
	};
};

// Coded by Seungjun ('20070126) MNet:No.2619 For Taiwan. Design cosidered the compression rebars.
struct _DGN_DRBD_D
{
	int iDRBDType;
	double dRatio;

	void Initialize()
	{
		iDRBDType = DRBD_None;
		dRatio    = 0.0;
	};
};

#define _DGN_LCASE_KEY unsigned int
union _DGN_LCASE_K
{
	_DGN_LCASE_KEY KeyMap;
	struct
	{
		unsigned int LcaseKey : 20;
		unsigned int AnalType : 12;
	}Key;
};

struct T_DETAIL_STR
{
	CArray<CStringW,CStringW> arString;
	
	CString CW2A(int nIndex) const
	{
		CString strTemp = ::CW2A(arString.GetAt(nIndex));
		return strTemp;
	}
	void Initialize()
	{
		arString.RemoveAll();
	}
	T_DETAIL_STR()	{}
	T_DETAIL_STR(const T_DETAIL_STR& src)	
	{
		*this = src;
	}
	T_DETAIL_STR& operator = (const T_DETAIL_STR& src)
	{
		arString.Copy(src.arString);
		return *this;
	}
};

// Add by ZINU.('08.09.26). NO:3664, Set Data by Bottom Wall in case of Same Wall-ID.
struct _DGN_BOUND_ELEM
{
	int iWallId;        // Wall-ID.
	CString strStorNa;  // Story Name.
	CString strLcomNa;
	int iPosiNo;        // 0=Bot, 1=Top.
	double d02fck;      // 0.2*fck.
	double dPu;   // Comp(+), Tens(-).
	double dMuy;
	double dLw;   // Length.
	double dTw;   // Thickness.
	double dSigma_x;    // Axial Stress (P/A + My/Sy).
	BOOL bContinue; // Continue to next wall.

	void Initialize()
	{
		iWallId   = 0;
		strStorNa = _T("");
		strLcomNa = _T("");
		iPosiNo   = 0;
		d02fck    = 0.0;
		dPu       = 0.0;
		dMuy      = 0.0;
		dLw       = 0.0;
		dTw       = 0.0;
		dSigma_x  = 0.0;
		bContinue = FALSE;
	}
};

// PMS:3956-Seungjun-20090311 EC8:04 Seismic Design
#define _DGN_WDLF_KEY unsigned int  // Wall design force function for EC2:04(EC8:04)
union _DGN_WDLF_K
{
	_DGN_WDLF_KEY KeyMap;
	struct
	{
		unsigned int wallid : 16;   // 0-65535
		unsigned int Lcomk  : 16;   // 0-65535
	}Key;
};

// Wall design force function for EC2:04(EC8:04)
struct _DGN_WDLF_D
{
	double da1;  // tension shift
	double dF1;  // bottom force (moment or shear)
	double dF2;  // top force
	double dF13;  // third force for shear
	double dTopZ;// Height
	double dBotZ;// Height

	void Initialize()
	{
		da1   = 0.0;
		dF1   = 0.0;
		dF2   = 0.0;
		dF13  = 0.0;
		dTopZ = 0.0;
		dBotZ = 0.0;
	}
};

struct _DGN_WALL_D
{
	UINT InternalWallId;
	CArray<UINT, UINT> arElemList;
	UINT TopNodeK;
	UINT BotNodeK;

	void Initialize()
	{
		InternalWallId = 0;
		arElemList.RemoveAll();
		TopNodeK = 0;
		BotNodeK = 0;
	}
	_DGN_WALL_D& operator = (const _DGN_WALL_D& src)
	{
		Initialize();
		InternalWallId = src.InternalWallId;
		arElemList.Copy(src.arElemList);
		TopNodeK = src.TopNodeK;
		BotNodeK = src.BotNodeK;

		return *this;
	}
};

/////////////////////////////////////////////////

struct T_SLAB_DESIGN_RESULT
{
	double dResultAs[4];
	void initialize()
	{
		for ( int i=0; i<4; ++i )
		{
			dResultAs[i] = 0.0;
		}        
	}
};

struct T_ONEW_SHEAR_POS
{
	//C3DPoint Point;
	UINT     ElemK;
};

struct DGN_CUTLINE_D
{
	T_NODE_D Point1;
	double	dPoint1Val1;
	double	dPoint1Val2;
	double  dDist1;
	T_NODE_D Point2;
	double	dPoint2Val1;
	double	dPoint2Val2;
	double  dDist2;
	T_ELEM_K KeyElem;
	int		 nSide;// 0: Left Side, 1: Right Side of Line Vector
	double  dAsTop; // 단위길이당 철근량
	double  dAsBot; // 단위길이당 철근량

	void initialize()
	{
		Point1.Initialize();
		dPoint1Val1 = 0.0;
		dPoint1Val2 = 0.0;
		dDist1      = 0.0;
		Point2.Initialize();
		dPoint2Val1 = 0.0;
		dPoint2Val2 = 0.0;
		dDist2      = 0.0;
		KeyElem     = 0;
		nSide       = 0;
		dAsTop      = 0.0;
		dAsBot      = 0.0;
	}
};

struct DGN_CUTLINE_AR
{
	CArray<DGN_CUTLINE_D, DGN_CUTLINE_D&> arCutLine;

	void initialize()
	{
		arCutLine.RemoveAll();
	}
	DGN_CUTLINE_AR& operator = (const DGN_CUTLINE_AR& src)
	{
		initialize();
		arCutLine.Copy(src.arCutLine);

		return *this;
	}
};

//===============================================================================
// PMS:xxxx-Seungjun-20090415 Slab Design
// From SDS DgnDataBase.h
enum EN_PHI
{
	EN_PHI_TENS = 0,
	EN_PHI_TRAN,
	EN_PHI_COMP,
};

struct _DGN_PHI
{
	EN_PHI enType;
	double det;
	double dety;
	double detu;
	double dphic;
	double dphit;
	double dphi;

	void Initialize()
	{
		enType = EN_PHI_TENS;
		det    = 0.0;
		dety   = 0.0;
		detu   = 0.0;
		dphic  = 1.0;
		dphit  = 1.0;
		dphi   = 1.0;
	}
	_DGN_PHI()
	{
		Initialize();
	}
	_DGN_PHI(const _DGN_PHI& sData)
	{
		*this = sData;
	}
	_DGN_PHI& operator = (const _DGN_PHI& sData)
	{
		enType = sData.enType;
		det    = sData.det;
		dety   = sData.dety;
		detu   = sData.detu;
		dphic  = sData.dphic;
		dphit  = sData.dphit;
		dphi   = sData.dphi;

		return *this;
	}
};

struct _DGN_SLAB_POS
{
	int iLCBNo;
	_DGN_PHI phi;
	double dN;
	double dMu;
	double dpMn;
	int iServLCBNo;
	double  dNq;
	double  dMq;
	double  dwk;//for BackCalcRebar
	double  dAsReq;
	double  dAsUse;
	double  dRhoReq;
	double  dRhoUse;
	double  dRhoCom;
	double  dRatM;    
	T_SLAB_BAR Rebar;
	T_SLAB_BAR AddRebar;
	CString strBarSize1;   //Add by xuezc 2017/9/30 For Print
	CString strAddBarSize1;//Add by xuezc 2017/9/30 For Print
	double  dSpace1;
	double  dAddSpace1;
	// PMS:DNKIM-20121102 Eurocode2:04의 NTC2012 기준추가시 중립축비(x/d) 검토 항목추가
	double dRatXd; // (x/d)
	double dLimXd; // (x/d)limit
	// for kds21
	double deta;
	double dalpha;
	double dbeta;
	/// aij-wsd99.
	double dMuFactByLcomTerm;
	
	int nLCBNoOpp;
	double dMuOpp;
	double dFactDist;
	double dRhomin1;
	double dRhomin2;
	double dRhocal;

	void Initialize()
	{
		iLCBNo	   = 0;
		dMu		   = 0.0;
		phi.Initialize();
		dpMn	   = 0.0;
		dAsReq	   = 0.0;
		dAsUse	   = 0.0;
		dRhoReq    = 0.0;
		dRhoUse    = 0.0;
		dRhoCom    = 0.0;
		dRatM	   = 0.0;
		Rebar.Initialize();
		AddRebar.Initialize();
		strBarSize1		= _T("");
		strAddBarSize1	= _T("");
		dSpace1 = 0.0;
		dAddSpace1  = 0.0;
		dRatXd      = 0.0;
		dLimXd      = 0.0;
		dNq			= 0.0;
		dMq			= 0.0;
		iServLCBNo  = 0;
		dwk			= 0.0;
		dN = 0;
		deta = 0.0;
		dalpha = 0.0;
		dbeta = 0.0;
		dMuFactByLcomTerm = 1.0;

		nLCBNoOpp = 0;
		dMuOpp    = 0.0;
		dFactDist = 0.0;
		dRhomin1 = 0.0;
		dRhomin2 = 0.0;
		dRhocal  = 0.0;
	}
	_DGN_SLAB_POS()
	{
		Initialize(); 
	}
	_DGN_SLAB_POS(const _DGN_SLAB_POS& sData)
	{
		*this = sData;
	}
	_DGN_SLAB_POS& operator = (const _DGN_SLAB_POS& sData)
	{
		iLCBNo	= sData.iLCBNo;
		dMu		= sData.dMu;
		phi     = sData.phi;
		dpMn	= sData.dpMn;
		dAsReq	= sData.dAsReq;
		dAsUse	= sData.dAsUse;
		dRhoReq	= sData.dRhoReq;
		dRhoUse	= sData.dRhoUse;
		dRhoCom = sData.dRhoCom;
		dRatM	= sData.dRatM;
		Rebar   = sData.Rebar;
		AddRebar = sData.AddRebar;
		strBarSize1	   = sData.strBarSize1;
		strAddBarSize1 = sData.strAddBarSize1;
		dSpace1		   = sData.dSpace1;
		dAddSpace1	   = sData.dAddSpace1;
		dRatXd      = sData.dRatXd;
		dLimXd      = sData.dLimXd;
		dNq			= sData.dNq;
		dMq			= sData.dMq;
		dwk			= sData.dwk;
		iServLCBNo	= sData.iServLCBNo;
		dN          = sData.dN;
		deta   = sData.deta;
		dalpha = sData.dalpha;
		dbeta  = sData.dbeta;
		dMuFactByLcomTerm = sData.dMuFactByLcomTerm;

		nLCBNoOpp = sData.nLCBNoOpp;
		dMuOpp    = sData.dMuOpp;
		dFactDist = sData.dFactDist;
		dRhomin1  = sData.dRhomin1;
		dRhomin2  = sData.dRhomin2;
		dRhocal   = sData.dRhocal;

		return *this;
	}

	double GetMuByLcomTerm() const
	{
		return dMu * dMuFactByLcomTerm;
	}
};

// PMS:4352-Seungjun-20111007 EC Inclined slab.
struct _DGN_ISLAB_POS
{
	int iNodeK;
	int iLCBNo;
	// Type
	int nCase; // 1~4
	// Plate forces
	double dMxx, dMyy, dMxy;
	double dFxx, dFyy, dFxy;
	// Equivalent Forces
	double dnpSdx;	// unit:unitforce
	double dnpSdy;	// unit:unitforce
	double dvpSd;	  // unit:unitforce
	// Resultant Forces
	double dnpx;	// unit:force
	double dnpy;	// unit:force
	double dvpx;	// unit:force
	double dvpy;	// unit:force
	// Stress
	double dsNa1; // Rebar-x	unit:stress
	double dsNa2; // Rebar-y	unit:stress
	double dsNh;  // concrete	unit:stress
	double dAc;   // concrete area	unit:area
	double dfytd;	// resistance	unit:stress
	double dfcd; 	// resistance	unit:stress
	// rebar    
	T_SLAB_BAR Rebar;
	T_SLAB_BAR AddRebar;

	void Initialize()
	{
		iNodeK = 0;
		iLCBNo = 0;
		nCase  = 0;
		dMxx   = 0.0;
		dMyy   = 0.0;
		dMxy   = 0.0;
		dFxx   = 0.0;
		dFyy   = 0.0;
		dFxy   = 0.0;
		dnpSdx = 0.0;
		dnpSdy = 0.0;
		dvpSd  = 0.0;
		dnpx   = 0.0;
		dnpy   = 0.0;
		dvpx   = 0.0;
		dvpy   = 0.0;
		dsNa1  = 0.0;
		dsNa2  = 0.0;
		dsNh   = 0.0;
		dAc    = 0.0;
		dfytd  = 0.0;
		dfcd   = 0.0;
		Rebar.Initialize();
		AddRebar.Initialize();        
	}
	_DGN_ISLAB_POS() 
	{
	}
	_DGN_ISLAB_POS(const _DGN_ISLAB_POS& sData)
	{
		*this = sData;
	}
	_DGN_ISLAB_POS& operator = (const _DGN_ISLAB_POS& sData)
	{
		iNodeK = sData.iNodeK;
		iLCBNo = sData.iLCBNo;
		nCase  = sData.nCase ;
		dMxx   = sData.dMxx  ;
		dMyy   = sData.dMyy  ;
		dMxy   = sData.dMxy  ;
		dFxx   = sData.dFxx  ;
		dFyy   = sData.dFyy  ;
		dFxy   = sData.dFxy  ;
		dnpSdx = sData.dnpSdx;
		dnpSdy = sData.dnpSdy;
		dvpSd  = sData.dvpSd ;
		dnpx   = sData.dnpx  ;
		dnpy   = sData.dnpy  ;
		dvpx   = sData.dvpx  ;
		dvpy   = sData.dvpy  ;
		dsNa1  = sData.dsNa1 ;
		dsNa2  = sData.dsNa2 ;
		dsNh   = sData.dsNh  ;
		dAc    = sData.dAc   ;
		dfytd  = sData.dfytd ;
		dfcd   = sData.dfcd  ;
		Rebar = sData.Rebar;
		AddRebar = sData.AddRebar;
		
		return *this;
	}
};

struct _DGN_ISLAB_MID
{
	int iNodeK;
	int iLCBNo;
	// Shear Force
	double dVx;	// unit:unitforce
	double dVy;	// unit:unitforce
	// Principal Shear
	double dV1;	// unit:unitforce
	// Resistance
	double dVRd1;
	// Tensile Rebar Ratio
	double dRho_x;
	double dRho_y;

	void Initialize()
	{
		iNodeK = 0;
		iLCBNo = 0;
		dVx = 0.0;
		dVy = 0.0;
		dV1 = 0.0;
		dVRd1 = 0.0;
		dRho_x = 0.0;
		dRho_y = 0.0;
	}
	_DGN_ISLAB_MID()
	{
	}
	_DGN_ISLAB_MID(const _DGN_ISLAB_MID& sData)	
	{
		*this = sData;
	}
	_DGN_ISLAB_MID& operator = (const _DGN_ISLAB_MID& sData)
	{
		iNodeK = sData.iNodeK;
		iLCBNo = sData.iLCBNo;
		dVx    = sData.dVx ;
		dVy    = sData.dVy  ;
		dV1    = sData.dV1  ;
		dVRd1  = sData.dVRd1 ;
		dRho_x = sData.dRho_x;
		dRho_y = sData.dRho_y;
		return *this;
	}
};

struct _DGN_SLAB_SERV_POS
{
	int iLCBNo;
	double m_dN;//for Crack China
	double dMu, dMuD, dMuL;
	// Stress
	double dStress[2];  // 0:concrete 1:Steel stress
	double dAStress[2]; // 0:k2*fck 1:k3*fyk - Allowable stess
	double dStrRat[2];
	// Crack
	int     nCheck; // 0:check, 1:200mm미만, 2:하중조합/외기조건 만족하지 않음.
	T_SLAB_BAR Rebar;
	T_SLAB_BAR AddRebar;    
	double  dwk;  // 발생균열
	double  dAwk; // 허용균열
	double  dAsmin;
	double  dASpace;
	double  dARebarDia;
	double  dCrkRat[4];
	// Deflection
	double  dElasticDsp;
	double  dCrackedDsp;
	double  dCreepedDsp;
	double  dAllowDsp;
	double  dRhoComp;
	// Ani-Crack
	int		iLCBNoAniCrack;
	double	dAniCrack;
	//
	int iLCBNo4Def;// MQC.10930 for deflection

	void Initialize()
	{
		iLCBNo = 0;
		iLCBNoAniCrack = 0;
		for(int i=0; i<2; ++i)
		{
			dStress[i]  = 0.0;
			dAStress[i] = 0.0;
			dStrRat[i]  = 0.0;
		}
		nCheck    = 0;
		Rebar.Initialize();
		AddRebar.Initialize();
		dwk       = 0.0;
		dAwk      = 0.0;
		dAsmin    = 0.0;
		dASpace   = 0.0;
		dARebarDia = 0.0;
		for(int i=0; i<4; i++)
		{
			dCrkRat[i] = 0.0;
		}        
		dElasticDsp = 0.0;
		dCrackedDsp = 0.0;
		dCreepedDsp = 0.0;
		dAllowDsp   = 0.0;
		dRhoComp    = 0.0;
		iLCBNo4Def  = 0;
		dAniCrack	= 0.0;
		m_dN		= 0;
	}
	_DGN_SLAB_SERV_POS()
	{
	}
	_DGN_SLAB_SERV_POS(const _DGN_SLAB_SERV_POS& sData)	
	{
		*this = sData;
	}
	_DGN_SLAB_SERV_POS& operator = (const _DGN_SLAB_SERV_POS& sData)
	{
		iLCBNo	   = sData.iLCBNo;
		iLCBNoAniCrack = sData.iLCBNoAniCrack;
		dMu        = sData.dMu;
		dMuD       = sData.dMuD;
		dMuL       = sData.dMuL;
		m_dN	   = sData.m_dN;
		iLCBNo4Def = sData.iLCBNo4Def;

		for(int i=0; i<2; i++)
		{
			dStress[i]  = sData.dStress[i];
			dAStress[i] = sData.dAStress[i];
			dStrRat[i]  = sData.dStrRat[i];
		}
		nCheck     = sData.nCheck;
		Rebar      = sData.Rebar;
		AddRebar   = sData.AddRebar;
		dwk        = sData.dwk;
		dAwk       = sData.dAwk;
		dAsmin     = sData.dAsmin;
		dASpace    = sData.dASpace;
		dARebarDia = sData.dARebarDia;
		for(int i=0; i<4; i++)
			dCrkRat[i]=sData.dCrkRat[i];
		dElasticDsp = sData.dElasticDsp;
		dCrackedDsp = sData.dCrackedDsp;
		dCreepedDsp = sData.dCreepedDsp;
		dAllowDsp   = sData.dAllowDsp;
		dRhoComp    = sData.dRhoComp;
		dAniCrack	= sData.dAniCrack;
		return *this;
	}
};

struct _DGN_SLAB_ND_POS
{
	int nLcomNo;
	double dMEd;
	double dMyd;
	double dRatio;
	double dAsUse;
	double dRhoUse;
	T_SLAB_BAR Rebar;
	T_SLAB_BAR AddRebar;
	DGN_CALC_MPHI_RESULT MphiRes;

	void Initialize()
	{
		nLcomNo = 0;
		dMEd = 0.0;
		dMyd = 0.0;
		dRatio = 0.0;
		dAsUse = 0.0;
		dRhoUse = 0.0;
		Rebar.Initialize();
		AddRebar.Initialize();
		MphiRes.Initialize();
	}
	_DGN_SLAB_ND_POS()
	{
		Initialize();  
	}
	_DGN_SLAB_ND_POS(const _DGN_SLAB_ND_POS& src)
	{
		*this = src;  
	}
	_DGN_SLAB_ND_POS& operator= (const _DGN_SLAB_ND_POS& src)
	{
		nLcomNo = src.nLcomNo;
		dMEd = src.dMEd;
		dMyd = src.dMyd;
		dRatio = src.dRatio;
		dAsUse = src.dAsUse;
		dRhoUse = src.dRhoUse;
		Rebar   = src.Rebar;
		AddRebar = src.AddRebar;
		MphiRes  = src.MphiRes;
		return *this;
	}
	void SetForc(int nLcom, double dM)
	{
		nLcomNo = nLcom;
		dMEd = dM;
	}
};

enum EN_SLAB_SHR_JP_ALPHA_TYPE
{
	EN_SLAB_SHR_JP_ALPHA_NONE = 0,
	EN_SLAB_SHR_JP_ALPHA_AUTO,
	EN_SLAB_SHR_JP_ALPHA_USER
};

struct _DGN_SLAB_SHR_JP_POS
{
	int nLcomNo;    
	double dM;
	bool bTopTens;
	double dJ;
	double dSumPsi;
	double dfs;   
	bool bConsiderRebar;
	EN_SLAB_SHR_JP_ALPHA_TYPE enAlphaType;	
	double dAlpha; /// 전단지간에 대한 할증계수.
	bool bReduConcShortTerm;
	double dpw;    /// 전단철근비.
	double dwft;   /// 전단철근의 전단보강용 허용인장응력도.
	double dQ;
	double dQa;
	double dRatio;
	T_SLAB_BAR TensRebar;
	T_SLAB_BAR TensAddRebar;

	void Initialize()
	{
		nLcomNo = 0;
		dM = 0.0;
		bTopTens = false;
		dJ = 0.0;
		dSumPsi = 0.0;
		dfs = 0.0;
		bConsiderRebar = false;
		enAlphaType = EN_SLAB_SHR_JP_ALPHA_NONE;
		dAlpha = 0.0;
		bReduConcShortTerm = false;
		dpw = 0.0;
		dwft = 0.0;
		dQ = 0.0;
		dQa = 0.0;
		dRatio = 0.0;
		TensRebar.Initialize();
		TensAddRebar.Initialize();
	}
	_DGN_SLAB_SHR_JP_POS()
	{
		Initialize();
	}
	_DGN_SLAB_SHR_JP_POS(const _DGN_SLAB_SHR_JP_POS& src)
	{
		*this = src;
	}
	_DGN_SLAB_SHR_JP_POS& operator= (const _DGN_SLAB_SHR_JP_POS& src)
	{
		nLcomNo  = src.nLcomNo;
		dM       = src.dM;
		bTopTens = src.bTopTens;
		dJ       = src.dJ;
		dSumPsi  = src.dSumPsi;
		dfs      = src.dfs;
		bConsiderRebar = src.bConsiderRebar;
		enAlphaType = src.enAlphaType;
		dAlpha   = src.dAlpha;
		bReduConcShortTerm = src.bReduConcShortTerm;
		dpw      = src.dpw;
		dwft     = src.dwft;
		dQ       = src.dQ;
		dQa      = src.dQa;
		dRatio   = src.dRatio;
		TensRebar    = src.TensRebar;
		TensAddRebar = src.TensAddRebar;
		return *this;
	}
};

struct _DGN_SLAB_COM
{
	int iSlabNo;
	int nNodeNo;
	double dB;
	double dT;
	double dFck;
	double dFc;
	double dFy;
	double dFyk; //Add HJS for GB code
	double dFys; // shear rebar strength.
	double dAlpha1; //for GB50010-02 by Hong,jiseon 2002.8.26
	double dThk;
	double dWidth; // for Strip Design
	
	void Initialize()
	{
		iSlabNo	= 0;
		nNodeNo = 0;
		dB		= 0.0;
		dT		= 0.0;
		dFck	= 0.0;
		dFc		= 0.0;
		dFy     = 0.0;
		dFyk	= 0.0;
		dFys    = 0.0;
		dThk	= 0.0;
		dAlpha1 = 0.0;
		dWidth  = 0.0;
	}
	_DGN_SLAB_COM()
	{
		Initialize(); 
	}
	_DGN_SLAB_COM(const _DGN_SLAB_COM& cData)
	{
		*this = cData;
	}
	_DGN_SLAB_COM& operator = (const _DGN_SLAB_COM& cData)
	{
		iSlabNo	= cData.iSlabNo;
		nNodeNo = cData.nNodeNo;
		dB		= cData.dB;
		dT		= cData.dT;
		dFck	= cData.dFck;
		dFc		= cData.dFc;
		dFy		= cData.dFy;
		dFyk    = cData.dFyk;
		dFys    = cData.dFys;
		dThk	= cData.dThk;
		dAlpha1 = cData.dAlpha1;
		dWidth  = cData.dWidth;
		return *this;
	}
};

struct _DGN_SLAB_RES
{
	int iNodeNum;
	_DGN_SLAB_COM Com[iConstMaxNo];
	_DGN_SLAB_POS Bot[iConstMaxNo];
	_DGN_SLAB_POS Top[iConstMaxNo];
	
	void Initialize()
	{
		iNodeNum = 0;
		for ( int i=0; i<iConstMaxNo; ++i )
		{
			Com[i].Initialize();
			Bot[i].Initialize();
			Top[i].Initialize();
		}
	}
	_DGN_SLAB_RES()
	{
	}
	_DGN_SLAB_RES(const _DGN_SLAB_RES& rData)
	{
		*this = rData;
	}
	_DGN_SLAB_RES& operator = (const _DGN_SLAB_RES& rData)
	{
		iNodeNum = rData.iNodeNum;
		for ( int i=0; i<iConstMaxNo; ++i )
		{
			Com[i] = rData.Com[i];
			Bot[i] = rData.Bot[i];
			Top[i] = rData.Top[i];
		}
		return *this;
	}
};

typedef CMapEx<UINT, UINT&, _DGN_SLAB_RES, _DGN_SLAB_RES&> _DGN_SLAB_RES_MAP;

struct _DGN_STRIP_RES2
{
	_DGN_SLAB_COM Com;
	_DGN_SLAB_POS Pos;

	void Initialize()
	{
		Com.Initialize();
		Pos.Initialize();
	}

	_DGN_STRIP_RES2() 
	{
		Initialize(); 
	}
	_DGN_STRIP_RES2(const _DGN_STRIP_RES2& rData)
	{
		*this = rData; 
	}
	_DGN_STRIP_RES2& operator = (const _DGN_STRIP_RES2& rData)
	{
		Com = rData.Com;
		Pos = rData.Pos;
		return *this;
	}
};

struct _DGN_STRIP_RES1
{
	_DGN_STRIP_RES2 Top;
	_DGN_STRIP_RES2 Bot;

	void Initialize()
	{
		Top.Initialize();
		Bot.Initialize();
	}
	_DGN_STRIP_RES1()
	{
		Initialize(); 
	}
	_DGN_STRIP_RES1(const _DGN_STRIP_RES1& rData)
	{
		*this = rData; 
	}
	_DGN_STRIP_RES1& operator = (const _DGN_STRIP_RES1& rData)
	{
		Top = rData.Top;
		Bot = rData.Bot;
		return *this;
	}
};

struct _DGN_STRIP_RES
{
	_DGN_STRIP_RES1 PosI; // Node i
	_DGN_STRIP_RES1 PosM; // Middle
	_DGN_STRIP_RES1 PosJ; // Node J

	void Initialize()
	{
		PosI.Initialize();
		PosM.Initialize();
		PosJ.Initialize();
	}
	_DGN_STRIP_RES() 
	{
		Initialize(); 
	}
	_DGN_STRIP_RES(const _DGN_STRIP_RES& rData) 
	{
		*this = rData; 
	}
	_DGN_STRIP_RES& operator = (const _DGN_STRIP_RES& rData)
	{
		PosI = rData.PosI;
		PosM = rData.PosM;
		PosJ = rData.PosJ;
		return *this;
	}
};

struct _DGN_ISLAB_RES
{
	int iNodeNum;
	_DGN_SLAB_COM  Com[iConstMaxNo];
	_DGN_ISLAB_POS Bot[iConstMaxNo];
	_DGN_ISLAB_POS Top[iConstMaxNo];
	_DGN_ISLAB_MID Mid[iConstMaxNo];
	
	void Initialize()
	{
		iNodeNum = 0;
		for(int i=0; i<iConstMaxNo; i++)
		{
			Com[i].Initialize();
			Bot[i].Initialize();
			Top[i].Initialize();
			Mid[i].Initialize();
		}
	}
	_DGN_ISLAB_RES()
	{
	}
	_DGN_ISLAB_RES(const _DGN_ISLAB_RES& rData)
	{
		*this = rData;
	}
	_DGN_ISLAB_RES& operator = (const _DGN_ISLAB_RES& rData)
	{
		iNodeNum = rData.iNodeNum;
		for(int i=0; i<iConstMaxNo; i++)
		{
			Com[i] = rData.Com[i];
			Bot[i] = rData.Bot[i];
			Top[i] = rData.Top[i];
			Mid[i] = rData.Mid[i];
		}
		return *this;
	}
};

struct _DGN_SLAB_SERV_RES
{
	int iNodeNum;
	_DGN_SLAB_COM	Com[iConstMaxNo];
	_DGN_SLAB_SERV_POS Bot[iConstMaxNo];
	_DGN_SLAB_SERV_POS Top[iConstMaxNo];
	
	void Initialize()
	{
		iNodeNum = 0;
		for(int i=0; i<iConstMaxNo; i++)
		{
			Com[i].Initialize();
			Bot[i].Initialize();
			Top[i].Initialize();
		}
	}
	_DGN_SLAB_SERV_RES()
	{
	}
	_DGN_SLAB_SERV_RES(const _DGN_SLAB_SERV_RES& rData)
	{
		*this = rData;
	}
	_DGN_SLAB_SERV_RES& operator = (const _DGN_SLAB_SERV_RES& rData)
	{
		iNodeNum = rData.iNodeNum;
		for(int i=0; i<iConstMaxNo; i++)
		{
			Com[i] = rData.Com[i];
			Bot[i] = rData.Bot[i];
			Top[i] = rData.Top[i];
		}
		return *this;
	}
};

typedef CMap<UINT, UINT&, _DGN_SLAB_SERV_RES, _DGN_SLAB_SERV_RES&> _DGN_SLAB_SERV_RES_MAP;

struct _DGN_SLAB_ND_RES
{
	int nNodeNum;
	_DGN_SLAB_COM Com[iConstMaxNo];
	_DGN_SLAB_ND_POS Bot[iConstMaxNo];
	_DGN_SLAB_ND_POS Top[iConstMaxNo];

	void Initialize()
	{
		nNodeNum = 0;
		for (int i = 0; i < iConstMaxNo; ++i)
		{
			Com[i].Initialize();
			Bot[i].Initialize();
			Top[i].Initialize();
		}
	}
	_DGN_SLAB_ND_RES()
	{
		Initialize(); 
	}
	_DGN_SLAB_ND_RES(const _DGN_SLAB_ND_RES& src)
	{
		*this = src;  
	}
	_DGN_SLAB_ND_RES& operator= (const _DGN_SLAB_ND_RES& src)
	{
		nNodeNum = src.nNodeNum;
		for (int i = 0; i < iConstMaxNo; ++i)
		{
			Com[i] = src.Com[i];
			Bot[i] = src.Bot[i];
			Top[i] = src.Top[i];
		}
		return *this;
	}
};

typedef CMapEx<UINT, UINT&, _DGN_SLAB_ND_RES, _DGN_SLAB_ND_RES&> _DGN_SLAB_ND_RES_MAP;

struct _DGN_SLAB_SHR_JP_RES
{
	int nNodeNum;
	_DGN_SLAB_COM Com[iConstMaxNo];
	_DGN_SLAB_SHR_JP_POS Pos[iConstMaxNo]; // shear result.
	
	void Initialize()
	{
		nNodeNum = 0;
		for ( int i = 0; i < iConstMaxNo; ++i )
		{
			Com[i].Initialize();
			Pos[i].Initialize();            
		}
	}
	_DGN_SLAB_SHR_JP_RES()
	{
		Initialize();
	}
	_DGN_SLAB_SHR_JP_RES(const _DGN_SLAB_SHR_JP_RES& src)
	{
		*this = src;
	}
	_DGN_SLAB_SHR_JP_RES& operator= (const _DGN_SLAB_SHR_JP_RES& src)
	{
		nNodeNum = src.nNodeNum;
		for ( int i = 0; i < iConstMaxNo; ++i )
		{
			Com[i] = src.Com[i];
			Pos[i] = src.Pos[i];            
		}
		return *this;
	}
};

typedef CMapEx<UINT, UINT, _DGN_SLAB_SHR_JP_RES, _DGN_SLAB_SHR_JP_RES&> _DGN_SLAB_SHR_JP_RES_MAP;

struct _DGN_PUNCH_INP_KCI12
{
	double dFck;
	double dfys;
	double drho;
	double dalphaS;
	double db0;
	double dd;

	void Initialize()
	{
		dFck    = 0.0;
		dfys    = 0.0;
		drho    = 0.0;
		dalphaS = 0.0;
		db0     = 0.0;
		dd      = 0.0;
	}
	_DGN_PUNCH_INP_KCI12() 
	{
		Initialize(); 
	}
	_DGN_PUNCH_INP_KCI12(const _DGN_PUNCH_INP_KCI12 &rData)
	{
		*this = rData; 
	}
	_DGN_PUNCH_INP_KCI12 &operator = (const _DGN_PUNCH_INP_KCI12 &rData)
	{
		dFck    = rData.dFck;
		dfys    = rData.dfys;
		drho    = rData.drho;
		dalphaS = rData.dalphaS;
		db0     = rData.db0;
		dd      = rData.dd;

		return *this;
	}
};

struct _DGN_PUNCH_RES_KCI12_VS
{
	BOOL   bChk;
	double dvs;
	double dVs;
	double dAv;
	double dd;
	double ds;

	void Initialize()
	{
		bChk = FALSE;
		dvs  = 0.0;
		dVs  = 0.0;
		dAv  = 0.0;
		dd   = 0.0;
		ds   = 0.0;
	}
	_DGN_PUNCH_RES_KCI12_VS()
	{
		Initialize(); 
	}
	_DGN_PUNCH_RES_KCI12_VS(const _DGN_PUNCH_RES_KCI12_VS &rData)
	{
		*this = rData; 
	}
	_DGN_PUNCH_RES_KCI12_VS &operator = (const _DGN_PUNCH_RES_KCI12_VS &rData)
	{
		bChk = rData.bChk;
		dvs  = rData.dvs;
		dVs  = rData.dVs;
		dAv  = rData.dAv;
		dd   = rData.dd;
		ds   = rData.ds;

		return *this;
	}
};

struct _DGN_PUNCH_RES_KCI12
{
	double dvc;
	double dVc;
	double dalphas;
	double dVcmax;
	double dVc1;
	double dks;
	double dkbo;
	double dfte;
	double dfcc;
	double dcot;
	double drho;
	double dcu;
	_DGN_PUNCH_RES_KCI12_VS VS;

	void Initialize()
	{
		dvc     = 0.0;
		dVc     = 0.0;
		dalphas = 0.0;
		dVcmax  = 0.0;
		dVc1    = 0.0;
		dks     = 0.0;
		dkbo    = 0.0;
		dfte    = 0.0;
		dfcc    = 0.0;
		dcot    = 0.0;
		drho    = 0.0;
		dcu     = 0.0;
		VS.Initialize();
	}
	_DGN_PUNCH_RES_KCI12()
	{
		Initialize(); 
	}
	_DGN_PUNCH_RES_KCI12(const _DGN_PUNCH_RES_KCI12 &rData)
	{
		*this = rData; 
	}
	_DGN_PUNCH_RES_KCI12 &operator = (const _DGN_PUNCH_RES_KCI12 &rData)
	{
		dvc     = rData.dvc;
		dVc     = rData.dVc;
		dalphas = rData.dalphas;
		dVcmax  = rData.dVcmax;
		dVc1    = rData.dVc1;
		dks     = rData.dks;
		dkbo    = rData.dkbo;
		dfte    = rData.dfte;
		dfcc    = rData.dfcc;
		dcot    = rData.dcot;
		drho    = rData.drho;
		dcu     = rData.dcu;
		VS      = rData.VS;

		return *this;
	}
	double CalcVn() const
	{
		return VS.bChk ? (dVc + VS.dVs) : dVc;        
	}
	double CalcVnConsiderVnmax() const
	{
		return min(CalcVn(), dVcmax);
	}
};

enum EN_PUNCH_RES_TYPE
{
	EN_VALID = 0,
	EN_NOT_EXIST_PERIMETER, /// [GEN-7484] 위험단면이 sub domain 안에 존재하지 않음.
};

struct _DGN_PUNCH_RES
{
	EN_PUNCH_RES_TYPE enResType;
	int iPileNo;
	int iSlabNo;	// Slab No having Minimum Thickness.
	int iNodeNo;
	int iSubDomainNo;
	int iLCBNo;
	int iShapeType;
	int iPosition;
	double dThk;	// Minimum Thickness.
	double dT, dB;
	double dFck;
	double dMu;
	double dVu;
	double dpVc2, dpVc21, dpVc22, dpVc23, dVrd_max;
	double dVsreq1;
	double dVsreq2;
	double dVnmax;
	double dAsw_s;
	double dRatV;
	double dBeta_c, dAlpha_s;
	double dDeff, db0, du11;
	//Add by Hong,jiseon 2002.8.24 for GB50010-02
	double dEta1, dEta2, dEta;
	double dBeta_h;
	double dFt;
	// Only TwowayCheck.
	double drvx, drvy;	// Gamma_vx, Gamma_vy.
	double dMux, dMuy;
	// For Eurocode2:04
	double dRholy, dRholz, dRhol;
	double dK, du0;
	double dv_ed;
	double dk, dC1, dC2, dW1;
	int    iPeriType;
	double duout_ef;
	double duout_ef_dist;
	double dCRdc;
	double dvmin;
	_DGN_PUNCH_RES_KCI12 KCI12;

	void Initialize()
	{
		enResType = EN_VALID;
		iPileNo=0; iSlabNo=0; iLCBNo=0; iNodeNo=0.0;
		iSubDomainNo = 0;
		iShapeType = 0; iPosition=0;
		dThk=0.0; dT=0.0; dB=0.0;
		dFck=0.0;
		dMu=0.0;
		dVu=0.0;
		dpVc2=0.0; dpVc21=0.0; dpVc22=0.0; dpVc23=0.0, dVrd_max=0.0;
		dVsreq1 = 0.0;
		dVsreq2 = 0.0;
		dVnmax  = 0.0;
		dAsw_s=0.0;
		dRatV=0.0;
		dBeta_c=0.0; dAlpha_s=0.0;
		dDeff=0.0; db0=0.0, du11=0.0;
		dEta1=0.0; dEta2=0.0; dEta=0.0;
		dBeta_h=0.0;
		dFt=0.0;
		drvx=0.0; drvy=0.0;
		dMux=0.0; dMuy=0.0;
		dRholy=0.0; dRholz=0.0; dRhol=0.0;
		dK=0.0; du0=0.0;
		dv_ed = 0.0;
		dk=0.0; dC1=0.0; dC2=0.0; dW1=0.0;
		iPeriType=0;
		duout_ef=0.0;
		duout_ef_dist=0.0;
		dCRdc=0.0;
		dvmin = 0.0;
		KCI12.Initialize();
	}
	_DGN_PUNCH_RES()
	{
		Initialize(); 
	}
	_DGN_PUNCH_RES(const _DGN_PUNCH_RES &rData) 
	{
		*this = rData; 
	}
	_DGN_PUNCH_RES &operator = (const _DGN_PUNCH_RES &rData)
	{
		enResType    = rData.enResType;
		iPileNo      = rData.iPileNo;
		iSlabNo      = rData.iSlabNo;
		iLCBNo       = rData.iLCBNo ;
		iNodeNo		 = rData.iNodeNo;
		iSubDomainNo = rData.iSubDomainNo;
		iShapeType   = rData.iShapeType;
		iPosition	 = rData.iPosition;
		dThk         = rData.dThk;
		dT           = rData.dT;
		dB           = rData.dB;
		dFck         = rData.dFck;
		dMu          = rData.dMu;
		dVu          = rData.dVu;
		dpVc2        = rData.dpVc2;
		dpVc21       = rData.dpVc21;
		dpVc22       = rData.dpVc22;
		dpVc23       = rData.dpVc23;
		dVrd_max     = rData.dVrd_max;
		dVsreq1      = rData.dVsreq1;
		dVsreq2      = rData.dVsreq2;
		dVnmax       = rData.dVnmax;
		dAsw_s       = rData.dAsw_s;
		dRatV        = rData.dRatV;
		dBeta_c      = rData.dBeta_c;
		dAlpha_s     = rData.dAlpha_s;
		dDeff        = rData.dDeff;
		db0          = rData.db0;
		du11         = rData.du11;
		dEta1        = rData.dEta1;
		dEta2        = rData.dEta2;
		dEta         = rData.dEta;
		dBeta_h      = rData.dBeta_h;
		dFt          = rData.dFt;
		drvx         = rData.drvx;
		drvy         = rData.drvy;
		dMux         = rData.dMux;
		dMuy         = rData.dMuy;
		dRholy       = rData.dRholy;
		dRholz       = rData.dRholz;
		dRhol        = rData.dRhol;
		dK           = rData.dK;
		du0          = rData.du0;
		dv_ed        = rData.dv_ed;
		dk           = rData.dk;
		dC1          = rData.dC1;
		dC2          = rData.dC2;
		dW1          = rData.dW1;
		iPeriType    = rData.iPeriType;
		duout_ef     = rData.duout_ef;
		duout_ef_dist= rData.duout_ef_dist;
		dCRdc        = rData.dCRdc;
		dvmin        = rData.dvmin;
		KCI12        = rData.KCI12;

		return *this;
	}
};

typedef CMap<UINT,UINT&,_DGN_PUNCH_RES,_DGN_PUNCH_RES&> _DGN_PUNCH_RES_MAP;

struct _DGN_1WAY_PART
{
	int iSlabNo;
	int iLCBNo;
	double dThk;
	double dT;
	double dB;
	double dFck;
	double dFt;
	double dVu[2];
	double dDist[2];
	double dPoint1[3];
	double dPoint2[3];
	double dDeff;
	double dbw;
	double dBeta_h;
	double dk;
	double dCRdc;
	double dvmin;
	double drhol;
	double dAsTop;
	double dAsBot;

	void Initialize()
	{
		iSlabNo = 0;
		iLCBNo = 0;
		dThk = 0.0;
		dT = 0.0;
		dB = 0.0;
		dFck = 0.0;
		dFt  = 0.0;
		for ( int i=0; i<2; ++i )
		{
			dVu[i] = 0.0;
			dDist[i] = 0.0;
		}
		for ( int i=0; i<3; ++i )
		{
			dPoint1[i] = 0.0;
			dPoint2[i] = 0.0;
		}
		dDeff = 0.0;
		dbw   = 0.0;
		dBeta_h = 0.0;
		dk = 0.0;
		dCRdc = 0.0;
		dvmin = 0.0;
		drhol = 0.0;
		dAsBot = 0.0;
		dAsTop = 0.0;
	}
	_DGN_1WAY_PART()
	{
	}
	_DGN_1WAY_PART(const _DGN_1WAY_PART& rData)
	{
		*this = rData; 
	}
	_DGN_1WAY_PART& operator= (const _DGN_1WAY_PART& rData)
	{
		iSlabNo = rData.iSlabNo;
		iLCBNo  = rData.iLCBNo;
		dThk = rData.dThk;
		dT = rData.dT;
		dB = rData.dB;
		dFck = rData.dFck;
		dFt  = rData.dFt;
		for ( int i = 0; i < 2; ++i )
		{
			dVu[i]   = rData.dVu[i];
			dDist[i] = rData.dDist[i];
		}
		for ( int i = 0; i < 3; ++i )
		{
			dPoint1[i] = rData.dPoint1[i];
			dPoint2[i] = rData.dPoint2[i];
		}
		dDeff   = rData.dDeff;
		dbw     = rData.dbw;
		dBeta_h = rData.dBeta_h;
		dk = rData.dk;
		dCRdc = rData.dCRdc;
		dvmin = rData.dvmin;
		drhol = rData.drhol;
		dAsBot = rData.dAsBot;
		dAsTop = rData.dAsTop;
		return *this;
	}
};

struct _DGN_1WAY_RES
{
	CString strCutLineNa;
	double dMu;
	double dVu;	// Total Force.
	double dpVc;
	double dThkCoef;
	double dFcuCoef;
	double dThkReq;
	double dRatV;
	double dk;
	double dCRdc;
	double dvmin;
	double drhol;
	double deff;
	double dAsw_sr;
	CArray<_DGN_1WAY_PART, _DGN_1WAY_PART&> PartList;

	void Initialize()
	{
		strCutLineNa = _T("");
		dMu     = 0.0;
		dVu			= 0.0;
		dpVc		= 0.0;
		dThkCoef= 0.0;
		dFcuCoef= 0.0;
		dThkReq	= 0.0;
		dRatV		= 0.0;
		dk      = 0.0;
		dCRdc   = 0.0;
		dvmin   = 0.0;
		drhol   = 0.0;
		deff    = 0.0;
		dAsw_sr = 0.0;
		PartList.RemoveAll();
	}
	_DGN_1WAY_RES()
	{
	}
	_DGN_1WAY_RES(const _DGN_1WAY_RES& rData)
	{
		*this = rData;
	}
	_DGN_1WAY_RES& operator = (const _DGN_1WAY_RES& rData)
	{
		strCutLineNa = rData.strCutLineNa;
		dMu     = rData.dMu;
		dVu		= rData.dVu;
		dpVc	= rData.dpVc;
		dThkCoef= rData.dThkCoef;
		dFcuCoef= rData.dFcuCoef;
		dThkReq	= rData.dThkReq;
		dRatV	= rData.dRatV;
		dk      = rData.dk;
		dCRdc   = rData.dCRdc;
		dvmin   = rData.dvmin;
		drhol   = rData.drhol;
		deff    = rData.deff;
		dAsw_sr = rData.dAsw_sr;
		PartList.RemoveAll();
		PartList.Copy(rData.PartList);
		return *this;
	}
};

typedef CMap<UINT, UINT&, _DGN_1WAY_RES, _DGN_1WAY_RES&> _DGN_1WAY_RES_MAP;

struct _DGN_1WAY_SLAB_PART
{
	int iSlabNo;
	int iLCBNo;
	double dThk;
	double dDeff;
	double dAlpha1; //Add by Hong,jiseon 2002.8.26 for GB50010-02
	double dT;
	double dB;
	double dFck;
	double dFc;
	double dFy;
	double dFyk;
	double dMu[2];
	double dDist[2];
	double dPoint1[3];
	double dPoint2[3];

	void Initialize()
	{
		iSlabNo	= 0;
		iLCBNo	= 0;
		dThk	= 0.0;
		dDeff	= 0.0;
		dAlpha1 = 0.0;
		dT		= 0.0;
		dB		= 0.0;
		dFck	= 0.0;
		dFc		= 0.0;
		dFyk	= 0.0;
		dFy		= 0.0;
		for ( int i=0; i<2; ++i )
		{
			dMu[i]	 = 0.0;
			dDist[i] = 0.0;
		}
		for ( int i=0; i<3; ++i )
		{
			dPoint1[i] = 0.0;
			dPoint2[i] = 0.0;
		}
	}
};

struct _DGN_1WAY_SLAB_RES
{
	CString strCutLineNa;
	_DGN_PHI  phi;
	double  dMu;	// Total Moment.
	double  dpMn;
	double  dAsReq;
	double  dAsUse;
	double  dRhoReq;
	double  dRhoUse;
	double  dRatM;
	// PMS:DNKIM-20121102 Eurocode2:04의 NTC2012 기준추가시 중립축비(x/d) 검토 항목추가
	double dRatXd; // (x/d)
	double dLimXd; // (x/d)limit
	T_SLAB_BAR Rebar;
	T_SLAB_BAR AddRebar;
	CArray<_DGN_1WAY_SLAB_PART, _DGN_1WAY_SLAB_PART&> PartList;

	void Initialize()
	{
		strCutLineNa = _T("");
		phi.Initialize();
		dMu		  = 0.0;
		dpMn	  = 0.0;
		dAsReq	  = 0.0;
		dAsUse	  = 0.0;
		dRhoReq	  = 0.0;
		dRhoUse	  = 0.0;
		dRatM	  = 0.0;        
		dRatXd    = 0.0;
		dLimXd    = 0.0;
		Rebar.Initialize();
		AddRebar.Initialize();
		PartList.RemoveAll();
	}
	_DGN_1WAY_SLAB_RES()
	{
	}
	_DGN_1WAY_SLAB_RES(const _DGN_1WAY_SLAB_RES& rData)
	{
		*this = rData;
	}
	_DGN_1WAY_SLAB_RES& operator = (const _DGN_1WAY_SLAB_RES& rData)
	{
		strCutLineNa = rData.strCutLineNa;
		phi      = rData.phi;
		dMu		 = rData.dMu;
		dpMn	 = rData.dpMn;
		dAsReq	 = rData.dAsReq;
		dAsUse	 = rData.dAsUse;
		dRhoReq	 = rData.dRhoReq;
		dRhoUse	 = rData.dRhoUse;
		dRatM	 = rData.dRatM;        
		dRatXd   = rData.dRatXd;
		dLimXd   = rData.dLimXd;
		Rebar    = rData.Rebar;
		AddRebar = rData.AddRebar;
		PartList.RemoveAll();
		PartList.Copy(rData.PartList);
		
		return *this;
	}
};

typedef CMap<UINT, UINT&, _DGN_1WAY_SLAB_RES, _DGN_1WAY_SLAB_RES&> _DGN_1WAY_SLAB_RES_MAP;

struct _DGN_PERI_POS
{
	// To Create Cutting Lines.
	double dCx[2];
	double dCy[2];

	void Initialize()
	{
		for ( int i=0; i<2; ++i )
		{
			dCx[i]=0.0;
			dCy[i]=0.0;
		}
	}
};

struct _DGN_PERI_RES
{
	int iSlabNo;
	int nPos; // 0:Top, 1:Left, 2:Bottom, 3:Right
	double dCx[2];
	double dCy[2];
	CArray<int,int> arLcomNo;
	CArray<double,double> arVu[2];
	CArray<double,double> arMu[2];
	
	void Initialize()
	{
		iSlabNo = 0;
		nPos = 0;
		arLcomNo.RemoveAll();
		for(int i=0; i<2; i++)
		{
			dCx[i]=0.0;
			dCy[i]=0.0;
			arVu[i].RemoveAll();
			arMu[i].RemoveAll();
		}
	}
	_DGN_PERI_RES() {}
	_DGN_PERI_RES(const _DGN_PERI_RES& rData)	{*this = rData;}
	_DGN_PERI_RES& operator = (const _DGN_PERI_RES& rData)
	{
		iSlabNo = rData.iSlabNo;
		nPos = rData.nPos;
		arLcomNo.Copy(rData.arLcomNo);
		for(int i=0; i<2; i++)
		{
			dCx[i] = rData.dCx[i];
			dCy[i] = rData.dCy[i];
			arVu[i].Copy(rData.arVu[i]);
			arMu[i].Copy(rData.arMu[i]);
		}
		return *this;
	}
};

enum PUNCH_SUPP_TYPE
{
	PUNCH_SIZE = 1,
	COLUMN_SUPPORT, // LOWER COLUMN
	PILE_SUPPORT,
	POINT_LOAD // UPPER COLUMN
};

struct _DGN_PERI_PART
{
	int iPileNo;
	UINT iNodeNo;
	int iPunchShp;	// 1:(Rectangle,Squre,H), 2:Round.
	int iSuppType;	// 1:Punching Size, 2:Column Supp, 3:Pile Supp. 4:Point Load. (Gen에는 2,4만 유효)
	int iPosition;  // 0:Inner, 1:Side T/B, 2:Side L/R, 3:Corner.
	double dRotate;
	CArray<_DGN_PERI_POS, _DGN_PERI_POS> aPosList;
	CArray<_DGN_PERI_POS, _DGN_PERI_POS> aInnList;	// Inner Positions.
	CArray<_DGN_PERI_RES, _DGN_PERI_RES> aResList;
	double dPeriUnitX; // PMS4603:DNKIM-20130222
	double dPeriUnitY;
	bool   bPararel_Localy_Top;

	void Initialize()
	{
		iPileNo=0;
		iNodeNo=0;
		iPunchShp=0;
		iSuppType=0;
		iPosition=0;
		dRotate=0.0;
		aPosList.RemoveAll();
		aInnList.RemoveAll();
		aResList.RemoveAll();
		dPeriUnitX = 0.0;
		dPeriUnitY = 0.0;
		bPararel_Localy_Top = false;
	}
	_DGN_PERI_PART() {}
	_DGN_PERI_PART(const _DGN_PERI_PART& rData) { *this = rData; }
	_DGN_PERI_PART& operator = (const _DGN_PERI_PART& rData)
	{
		iPileNo	  = rData.iPileNo;
		iNodeNo   = rData.iNodeNo;
		iPunchShp = rData.iPunchShp;
		iSuppType = rData.iSuppType;
		iPosition = rData.iPosition;
		dRotate	  = rData.dRotate;
		aPosList.Copy(rData.aPosList);
		aInnList.Copy(rData.aInnList);
		aResList.Copy(rData.aResList);
		dPeriUnitX = rData.dPeriUnitX;
		dPeriUnitY = rData.dPeriUnitY;
		bPararel_Localy_Top = rData.bPararel_Localy_Top;

		return *this;
	}

	bool IsUseTopBarCover() const
	{
		switch ( iSuppType )
		{
		case PUNCH_SIZE:
		case POINT_LOAD:
			return false;
		case COLUMN_SUPPORT:
		case PILE_SUPPORT: 
			return true;		
		default: 
			ASSERT(0); 
			return false;
		}
	}
};

typedef CMap<CString,LPCTSTR,_DGN_PERI_PART,_DGN_PERI_PART&> _DGN_PERI_PART_MAP;
//===============================================================================

/////////////////////////////
// 1방향 슬라브/바닥판 설계
// MQC:3161-Seungjun-20100209 요소번호가 65536 이상일 경우 Dir 2. 설계결과 이상.
#define _DGN_SLBF_KEY ULONG64
union _DGN_SLBF_K
{
	_DGN_SLBF_KEY KeyMap; // unsigned 64bit int

	struct
	{
		union  // unsigned 16bit int
		{
			unsigned int ElemInfo;

			struct
			{
				unsigned int ElemK : 32;  // 0~4,294,967,295
			}ElemD;
		}_ElemInfo;

		union  // unsigned 16bit int
		{
			unsigned int LoadInfo;

			struct
			{
				unsigned int LcomK : 32;  // 0~4,294,967,295
			}LoadD;
		}_LoadInfo;
	}Key;
};

struct _DGN_SLAB_FORC
{
	double dMuTop[4];
	double dMuBot[4];

	void Initialize()
	{
		for(int i=0; i<4; i++)
		{
			dMuBot[i] = 0.0;
			dMuTop[i] = 0.0;
		}
	}
};

struct _DGN_SLAB_FORC_MV
{
	double dM[4];
	double dV[4];

	void Initialize()
	{
		for ( int i=0; i<4; ++i )
		{
			dM[i] = 0.0;
			dV[i] = 0.0;
		}
	}
	_DGN_SLAB_FORC_MV()
	{
		Initialize();
	}
	_DGN_SLAB_FORC_MV(const _DGN_SLAB_FORC_MV& src)
	{
		*this = src;
	}
	_DGN_SLAB_FORC_MV& operator= (const _DGN_SLAB_FORC_MV& src)
	{
		for ( int i=0; i<4; ++i )
		{
			dM[i] = src.dM[i];
			dV[i] = src.dV[i];
		}
		return *this;
	}
};

// Meshed wall
struct _DGN_WALL_POS
{
	int iLCBNo;
	double dftd[2]; // x,y
	double dftn[2];
	double dSig_cd; // x,y
	double dNu_fcd;
	double dStress[3];
	double dAsReq[2];
	double dAsUse[2];
	double dRhoReq[2];
	double dRhoUse[2];
	double dRat[3];
	double dSpace[2];
	CString strBarSize[2];
	CString strRebar[2];

	void Initialize()
	{
		iLCBNo = 0;
		for(int i=0; i<2; ++i)
		{
			dftd[i]     = 0.0;
			dftn[i]     = 0.0;
			dAsReq[i]	= 0.0;
			dAsUse[i]	= 0.0;
			dRhoReq[i]	= 0.0;
			dRhoUse[i]	= 0.0;
			dSpace[i]	= 0.0;
			strBarSize[i] = _T("");
			strRebar[i]	  = _T("");
		}

		dSig_cd = 0.0;
		dNu_fcd = 0.0;

		for ( int i=0; i<3; ++i )
		{
			dStress[i] = 0.0;
			dRat[i] = 0.0;
		}
	}
	_DGN_WALL_POS() {}
	_DGN_WALL_POS(const _DGN_WALL_POS& sData)
	{
		*this = sData;
	}
	_DGN_WALL_POS& operator = (const _DGN_WALL_POS& sData)
	{
		iLCBNo	= sData.iLCBNo;
		for(int i=0; i<2; ++i)
		{
			dftd[i] 	 = sData.dftd[i];
			dftn[i] 	 = sData.dftn[i];
			dAsReq[i]	 = sData.dAsReq[i];
			dAsUse[i]	 = sData.dAsUse[i];
			dRhoReq[i]	 = sData.dRhoReq[i];
			dRhoUse[i]	 = sData.dRhoUse[i];
			dSpace[i]	  = sData.dSpace[i];
			strBarSize[i] = sData.strBarSize[i];
			strRebar[i]	  = sData.strRebar[i];
		}

		dSig_cd = sData.dSig_cd;
		dNu_fcd	= sData.dNu_fcd;

		for ( int i=0; i<3; ++i )
		{
			dRat[i] = sData.dRat[i];
			dStress[i] = sData.dStress[i];
		}
		
		return *this;
	}
};

struct _DGN_WALL_COM
{
	int iWallNo;
	double dB;
	double dT;
	double dFck;
	double dFc;
	double dFy;
	double dFyk; //Add HJS for GB code
	double dAlpha1; //for GB50010-02 by Hong,jiseon 2002.8.26
	double dThk;

	void Initialize()
	{
		iWallNo	= 0;
		dB	  = 0.0;
		dT	  = 0.0;
		dFck  = 0.0;
		dFc	  = 0.0;
		dFyk  = 0.0;
		dFy   = 0.0;
		dAlpha1 = 0.0;
		dThk	= 0.0;
	}
	_DGN_WALL_COM()
	{
	}
	_DGN_WALL_COM(const _DGN_WALL_COM& cData)
	{
		*this = cData;
	}
	_DGN_WALL_COM& operator = (const _DGN_WALL_COM& cData)
	{
		iWallNo	= cData.iWallNo;
		dB		= cData.dB;
		dT		= cData.dT;
		dFck	= cData.dFck;
		dFc		= cData.dFc;
		dFyk    = cData.dFyk;
		dFy		= cData.dFy;
		dAlpha1 = cData.dAlpha1;
		dThk	= cData.dThk;
		return *this;
	}
};

struct _DGN_WALL_RES
{
	int iNodeNum;
	_DGN_WALL_COM Com[iConstMaxNo];
	_DGN_WALL_POS Bot[iConstMaxNo];
	_DGN_WALL_POS Top[iConstMaxNo];

	void Initialize()
	{
		iNodeNum = 0;
		for(int i=0; i<iConstMaxNo; ++i)
		{
			Com[i].Initialize();
			Bot[i].Initialize();
			Top[i].Initialize();
		}
	}
	_DGN_WALL_RES()
	{
	}
	_DGN_WALL_RES(const _DGN_WALL_RES& rData)
	{
		*this = rData;
	}
	_DGN_WALL_RES& operator = (const _DGN_WALL_RES& rData)
	{
		iNodeNum = rData.iNodeNum;
		for(int i=0; i<iConstMaxNo; ++i)
		{
			Com[i] = rData.Com[i];
			Bot[i] = rData.Bot[i];
			Top[i] = rData.Top[i];
		}
		return *this;
	}
};

#pragma region /// FOR SHELL DESIGN

struct _DGN_SHEL_FORC
{
	double dMxx[4];
	double dMyy[4];
	double dMxy[4];
	double dFxx[4];
	double dFyy[4];
	double dFxy[4];
	double dVxx[4];
	double dVyy[4];
	double dSmxTop[4]; // [stress] Sigma,max at top.
	double dSmnTop[4]; // [stress] Sigma,min at top.
	double dSmxBot[4]; // [stress] Sigma,max at bottom.
	double dSmnBot[4]; // [stress] Sigma,min at bottom.

	void Initialize()
	{
		for (int i=0; i<4; ++i)
		{
			dMxx[i] = 0.0;
			dMyy[i] = 0.0;
			dMxy[i] = 0.0;
			dFxx[i] = 0.0;
			dFyy[i] = 0.0;
			dFxy[i] = 0.0;
			dVxx[i] = 0.0;
			dVyy[i] = 0.0;
			dSmxTop[i] = 0.0;
			dSmnTop[i] = 0.0;
			dSmxBot[i] = 0.0;
			dSmnBot[i] = 0.0;
		}
	}
	_DGN_SHEL_FORC() 
	{
		Initialize(); 
	}
	_DGN_SHEL_FORC(const _DGN_SHEL_FORC& src)
	{
		*this = src; 
	}
	_DGN_SHEL_FORC& operator= (const _DGN_SHEL_FORC& src)
	{
		for (int i=0; i<4; ++i)
		{
			dMxx[i] = src.dMxx[i];
			dMyy[i] = src.dMyy[i];
			dMxy[i] = src.dMxy[i];
			dFxx[i] = src.dFxx[i];
			dFyy[i] = src.dFyy[i];
			dFxy[i] = src.dFxy[i];
			dVxx[i] = src.dVxx[i];
			dVyy[i] = src.dVyy[i];
			dSmxTop[i] = src.dSmxTop[i];
			dSmnTop[i] = src.dSmnTop[i];
			dSmxBot[i] = src.dSmxBot[i];
			dSmnBot[i] = src.dSmnBot[i];
		}

		return *this;
	}
};

typedef CArray<_DGN_SHEL_FORC, _DGN_SHEL_FORC&> _DGN_SHEL_FORC_LIST;
typedef CMapEx<_DGN_SLBF_KEY, _DGN_SLBF_KEY&, _DGN_SHEL_FORC,_DGN_SHEL_FORC&> _DGN_SHEL_FORC_MAP;

struct _DGN_SHEL_COM
{
	int nElemNo;
	double dBx;  // Dir-X (Dir-1)의 하부 피복두께
	double dTx;  // Dir-X (Dir-1)의 상부 피복두께
	double dBy;  // Dir-X (Dir-1)의 하부 피복두께
	double dTy;  // Dir-X (Dir-1)의 상부 피복두께
	double dFck;
	double dFc;
	double dFy;
	double dThk;

	void Initialize()
	{
		nElemNo = 0;
		dBx  = 0.0;
		dTx  = 0.0;
		dBy  = 0.0;
		dTy  = 0.0;
		dFck = 0.0;
		dFc  = 0.0;
		dFy  = 0.0;
		dThk = 0.0;
	}
	_DGN_SHEL_COM() 
	{
	}
	_DGN_SHEL_COM(const _DGN_SHEL_COM& src) 
	{
		*this = src; 
	}
	_DGN_SHEL_COM& operator= (const _DGN_SHEL_COM& src)
	{
		nElemNo = src.nElemNo;
		dBx  = src.dBx;
		dTx  = src.dTx;
		dBy  = src.dBy;
		dTy  = src.dTy;
		dFck = src.dFck;
		dFc  = src.dFc;
		dFy  = src.dFy;
		dThk = src.dThk;

		return *this;
	}
};

struct _DGN_SHEL_POSDIR
{
	double dftd;
	double dftn;
	double dRatio;
	double dAsreq;
	double dAsuse;
	double dRhoreq;
	double dRhomin;
	double dRhouse;
	BOOL bChkBasicBar;  /// Basic Bar 고려 여부.
	double dBarSpace;
	double dAddBarSpace;
	CString strBarSize;  /// D10
	CString strAddBarSize;
	CString strBarInfo;  /// format D10@200
	CString strAddBarInfo;

	void Initialize()
	{
		dftd = 0.0;
		dftn = 0.0;
		dRatio = 0.0;
		dAsreq = 0.0;
		dAsuse = 0.0;
		dRhoreq = 0.0;
		dRhomin = 0.0;
		dRhouse = 0.0;
		bChkBasicBar = FALSE;
		dBarSpace = 0.0;
		dAddBarSpace = 0.0;
		strBarSize = _T("");
		strAddBarSize = _T("");
		strBarInfo = _T("");
		strAddBarInfo = _T("");
	}
	_DGN_SHEL_POSDIR()
	{
	}
	_DGN_SHEL_POSDIR(const _DGN_SHEL_POSDIR& src)
	{
		*this = src; 
	}
	_DGN_SHEL_POSDIR& operator= (const _DGN_SHEL_POSDIR& src)
	{
		dftd    = src.dftd;
		dftn    = src.dftn;
		dRatio  = src.dRatio;
		dAsreq  = src.dAsreq;
		dAsuse  = src.dAsuse;
		dRhoreq = src.dRhoreq;
		dRhomin = src.dRhomin;
		bChkBasicBar  = src.bChkBasicBar;
		dRhouse = src.dRhouse;
		dBarSpace  = src.dBarSpace;
		dAddBarSpace  = src.dAddBarSpace;
		strBarSize    = src.strBarSize;
		strAddBarSize = src.strAddBarSize;
		strBarInfo    = src.strBarInfo;
		strAddBarInfo = src.strAddBarInfo;

		return *this;
	}
};

struct _DGN_SHEL_CRACK
{
	double dSigmax;
	double dSigmin;
	double dfcm;
	double dAlpha;
	double dBeta;
	double dLambda;
	double dJ2;
	double dJ3;
	double dI1;
	double dPHI;

	void Initialize()
	{
		dSigmax = 0.0;
		dSigmin = 0.0;
		dfcm    = 0.0;
		dAlpha  = 0.0;
		dBeta   = 0.0;
		dLambda = 0.0;
		dJ2     = 0.0;
		dJ3     = 0.0;
		dI1     = 0.0;
		dPHI    = 0.0;
	}
	_DGN_SHEL_CRACK()
	{
		Initialize(); 
	}
	_DGN_SHEL_CRACK(const _DGN_SHEL_CRACK& src)
	{
		*this = src; 
	}
	_DGN_SHEL_CRACK& operator= (const _DGN_SHEL_CRACK& src)
	{
		dSigmax = src.dSigmax;
		dSigmin = src.dSigmin;
		dfcm    = src.dfcm;
		dAlpha  = src.dAlpha;
		dBeta   = src.dBeta;
		dLambda = src.dLambda;
		dJ2     = src.dJ2;
		dJ3     = src.dJ3;
		dI1     = src.dI1;
		dPHI    = src.dPHI;
		return *this;
	}
};

struct _DGN_SHEL_POS
{
	int nElemNo;
	int nNodeNo;
	int nDgnLcomNo;
	int nOrgLcomNo;

	double dLayerThk;
	_DGN_SHEL_CRACK Crack;
	double dSigEdx;
	double dSigEdy;
	double dTauEdxy;  // = TauEdyx.

	EN_SHEL_REINF_TYPE enReinfType;
	_DGN_SHEL_POSDIR BarX;
	_DGN_SHEL_POSDIR BarY;

	EN_SHEL_CRD_TYPE enConRdType;
	double dAlpha;      // ratio between two principal stresses.
	double dSigConcEd;  // concrete design stress.
	double dSigConcRd;  // concrete capacity stress.
	double dRatCon;
	double dRatio;
	double dfcd; // aci계열은 없는 개념이라서 근거 출력용.

	void Initialize()
	{
		nElemNo  = 0;
		nNodeNo  = 0;
		nDgnLcomNo = 0;
		nOrgLcomNo = 0;
		dLayerThk = 0.0;
		Crack.Initialize();
		dSigEdx  = 0.0;
		dSigEdy  = 0.0;
		dTauEdxy = 0.0;

		enReinfType = EN_SHEL_REINF_NONE;
		BarX.Initialize();
		BarY.Initialize();
		enConRdType = EN_SHEL_CRD_FCD;
		dAlpha     = 0.0;
		dSigConcEd = 0.0;
		dSigConcRd = 0.0;
		dRatCon    = 0.0;
		dRatio     = 0.0;
		dfcd       = 0.0;
	}
	_DGN_SHEL_POS()
	{
	}
	_DGN_SHEL_POS(const _DGN_SHEL_POS& src)
	{
		*this = src; 
	}
	_DGN_SHEL_POS& operator= (const _DGN_SHEL_POS& src)
	{
		nElemNo    = src.nElemNo;
		nNodeNo    = src.nNodeNo;
		nDgnLcomNo = src.nDgnLcomNo;
		nOrgLcomNo = src.nOrgLcomNo;
		dLayerThk  = src.dLayerThk;
		Crack      = src.Crack;
		dSigEdx    = src.dSigEdx;
		dSigEdy    = src.dSigEdy;
		dTauEdxy   = src.dTauEdxy;

		enReinfType = src.enReinfType;
		BarX = src.BarX;
		BarY = src.BarY;

		enConRdType = src.enConRdType;
		dAlpha = src.dAlpha;
		dSigConcEd = src.dSigConcEd;
		dSigConcRd = src.dSigConcRd;
		dRatCon    = src.dRatCon;
		dRatio     = src.dRatio;
		dfcd       = src.dfcd;
		return *this;
	}

	void SetInfo(int nCurElem, int nCurNode, int nCurDgnLcom, int nCurOrgLcom)
	{
		nElemNo = nCurElem;
		nNodeNo = nCurNode;
		nDgnLcomNo = nCurDgnLcom;
		nOrgLcomNo = nCurOrgLcom;
	}
};

struct _DGN_SHEL_POSV
{
	int nElemNo;
	int nNodeNo;
	int nDgnLcomNo;
	int nOrgLcomNo;
	double dNEdx;
	double dNEdy;
	double dVEdx;
	double dVEdy;
	double dVEdo;
	double dTanPhio;
	double dRhol;
	double dk;
	double dSigcp;
	double dCRdc;
	double dvmin;
	double dk1;	
	double dVRdc1;
	double dVRdc2;
	double dVRdc;
	double dRatio;
	double dTheta;  // concrete strut angle.
	double dz;
	double dAswSreq;  // required Asw/S
	double dVEdsReq;
	double dd;

	void Initialize()
	{
		nElemNo  = 0;
		nNodeNo  = 0;
		nDgnLcomNo = 0;
		nOrgLcomNo = 0;
		dNEdx    = 0.0;
		dNEdy    = 0.0;
		dVEdx    = 0.0;
		dVEdy    = 0.0;
		dVEdo    = 0.0;
		dTanPhio = 0.0;
		dRhol    = 0.0;
		dk       = 0.0;
		dSigcp   = 0.0;
		dCRdc    = 0.0;
		dvmin    = 0.0;
		dk1      = 0.0;
		dVRdc1   = 0.0;
		dVRdc2   = 0.0;
		dVRdc    = 0.0;
		dRatio   = 0.0;
		dTheta   = 0.0;
		dz       = 0.0;
		dAswSreq = 0.0;
		dVEdsReq = 0.0;
		dd       = 0.0;
	}
	_DGN_SHEL_POSV()
	{
		Initialize(); 
	}
	_DGN_SHEL_POSV(const _DGN_SHEL_POSV& src) 
	{
		*this = src; 
	}
	_DGN_SHEL_POSV& operator= (const _DGN_SHEL_POSV& src)
	{
		nElemNo  = src.nElemNo;
		nNodeNo  = src.nNodeNo;
		nDgnLcomNo = src.nDgnLcomNo;
		nOrgLcomNo = src.nOrgLcomNo;
		dNEdx    = src.dNEdx;
		dNEdy    = src.dNEdy;
		dVEdx    = src.dVEdx;
		dVEdy    = src.dVEdy;
		dVEdo    = src.dVEdo;
		dTanPhio = src.dTanPhio;
		dRhol    = src.dRhol;
		dk       = src.dk;
		dSigcp   = src.dSigcp;
		dCRdc    = src.dCRdc;
		dvmin    = src.dvmin;
		dk1      = src.dk1;
		dVRdc1   = src.dVRdc1;
		dVRdc2   = src.dVRdc2;
		dVRdc    = src.dVRdc;
		dRatio   = src.dRatio;
		dTheta   = src.dTheta;
		dz       = src.dz;
		dAswSreq = src.dAswSreq;
		dVEdsReq = src.dVEdsReq;
		dd       = src.dd;
		return *this;
	}
};

struct _DGN_SHEL_FRES
{
	int nElemNo;
	int nNodeNum;
	_DGN_SHEL_COM Com;
	_DGN_SHEL_POS Bot[iConstMaxNo];
	_DGN_SHEL_POS Top[iConstMaxNo];

	void Initialize()
	{
		nElemNo  = 0;
		nNodeNum = 0;
		Com.Initialize();
		for (int i=0; i<iConstMaxNo; i++)
		{
			Bot[i].Initialize();
			Top[i].Initialize();
		}
	}
	_DGN_SHEL_FRES()
	{
	}
	_DGN_SHEL_FRES(const _DGN_SHEL_FRES& rData)
	{
		*this = rData; 
	}
	_DGN_SHEL_FRES& operator = (const _DGN_SHEL_FRES& rData)
	{
		nElemNo  = rData.nElemNo;
		nNodeNum = rData.nNodeNum;
		for(int i=0; i<iConstMaxNo; i++)
		{
			Com = rData.Com;
			Bot[i] = rData.Bot[i];
			Top[i] = rData.Top[i];
		}
		return *this;
	}
};

typedef CMapEx<UINT, UINT&, _DGN_SHEL_FRES, _DGN_SHEL_FRES&> _DGN_SHEL_FRES_MAP;

struct _DGN_SHEL_VRES
{
	int nElemNo;
	int nNodeNum;
	_DGN_SHEL_COM  Com;
	_DGN_SHEL_POSV Shr[iConstMaxNo];

	void Initialize()
	{
		nElemNo  = 0;
		nNodeNum = 0;
		Com.Initialize();
		for (int i=0; i<iConstMaxNo; ++i)
		{
			Shr[i].Initialize();
		}
	}
	_DGN_SHEL_VRES() 
	{
		Initialize(); 
	}
	_DGN_SHEL_VRES(const _DGN_SHEL_VRES& src)
	{
		*this = src; 
	}
	_DGN_SHEL_VRES& operator= (const _DGN_SHEL_VRES& src)
	{
		nElemNo  = src.nElemNo;
		nNodeNum = src.nNodeNum;
		Com      = src.Com;
		for (int i=0; i<iConstMaxNo; ++i)
		{
			Shr[i] = src.Shr[i];
		}
		return *this;
	}
};

typedef CMapEx<UINT, UINT&, _DGN_SHEL_VRES, _DGN_SHEL_VRES&> _DGN_SHEL_VRES_MAP;

#pragma endregion

//===============================================================================

///////////// 추가 필요 구조체.
#define T_MCKL_K UINT
typedef CArray<T_MCKL_K, T_MCKL_K> T_MCKL_K_LIST;

//(*customize*)
struct T_MCKL_D
{
	CString   strName;              // shear check line name
	double Point1[3];
	double Point2[3];
	
	void Initialize()
	{
		strName.Empty();
		memset(Point1, 0, sizeof(Point1));
		memset(Point2, 0, sizeof(Point2));
	}
	static struct T_MCKL_UNIT
	{
		int strName;
		int Point1;
		int Point2;
	} unit;
};

#define T_SCKL_K UINT
typedef CArray<T_SCKL_K, T_SCKL_K> T_SCKL_K_LIST;

//(*customize*)
struct T_SCKL_D
{
	CString   strName;              // shear check line name
	int       nDir;                 // 0, 1
	double    Point1[3];
	double    Point2[3];
	double    dCheckRatio;
	int       nComp;
	void Initialize()
	{
		strName.Empty();
		nDir = 0;
		memset(Point1, 0, sizeof(Point1));
		memset(Point2, 0, sizeof(Point2));
		dCheckRatio = 0.0;
		nComp = 0;
	}
	static struct T_SCKL_UNIT
	{
		int strName;
		int nDir;
		int Point1;
		int Point2;
		int dCheckRatio;
		int nComp;
	} unit;
};

#define		DGN_SMOO_FORC_NUM  8
struct _DGN_SMOO_FORC
{
	CArray<double, double> aForce[DGN_SMOO_FORC_NUM];

	void Initialize()
	{
		for ( int i=0; i<DGN_SMOO_FORC_NUM; ++i )
		{
			aForce[i].RemoveAll();
		}
	}
	_DGN_SMOO_FORC& operator = (const _DGN_SMOO_FORC& rData)
	{
		for ( int i=0; i<DGN_SMOO_FORC_NUM; ++i )
		{
			aForce[i].Copy(rData.aForce[i]);
		}

		return *this;
	}
};

struct _DGN_CELL_FORC
{
	CMapEx<T_NODE_K, T_NODE_K, T_KEY_LIST_EX, T_KEY_LIST_EX&> arNodeElemList;

	void Initialize()
	{
		arNodeElemList.RemoveAll();
	}
	_DGN_CELL_FORC& operator = (const _DGN_CELL_FORC& rData)
	{
		POSITION pos = rData.arNodeElemList.GetStartPosition();
		while(pos)
		{
			T_NODE_K NextNodeK;
			T_KEY_LIST_EX ElemListK;
			rData.arNodeElemList.GetNextAssoc(pos, NextNodeK, ElemListK);

			arNodeElemList.SetAt(NextNodeK, ElemListK);
		}

		return *this;
	}
};

struct _DGN_ELEM_LIST
{
	CArray<T_ELEM_K, T_ELEM_K> arElemList;

	void Initialize()
	{
		arElemList.RemoveAll();
	}
	_DGN_ELEM_LIST& operator = (const _DGN_ELEM_LIST& rData)
	{
		arElemList.Copy(rData.arElemList);

		return *this;
	}
};

struct _DGN_EXPAND_LOAD_ELEM // 1개의 Elem의 할증정보를 보관
{
	BOOL bIsExpand;
	T_ELEM_K ElemK;
	BOOL   bExPosi[3]; // 할증시킬 위치
	double dFactor[3]; // 할증비 or 하중계수
	int    nExpandType[3];// 하중할증형식 (0:기존 하중계수에 dFactor을 곱함, 1:기존 하중계수를 dFactor로 대체)
	// 축력, 전단력, 토션이 추가로 필요해지면 그때 추가

	void Initialize()
	{
		bIsExpand = FALSE;
		ElemK     = 0;
		for(int i=0 ; i<3 ; i++)
		{
			bExPosi[i] = FALSE;
			dFactor[i] = 1.0;
			nExpandType[i] = 1;
		}
	}
};
struct _DGN_EXPAND_LOAD_LCOM // 해당 Lcom에 대해 할증시킬 Elem정보를 보관
{
	BOOL bIsExpand;
	T_LCOM_K LcomK;
	CMapEx<T_ELEM_K, T_ELEM_K, _DGN_EXPAND_LOAD_ELEM, _DGN_EXPAND_LOAD_ELEM&> arElemList;
	
	void Initialize()
	{
		bIsExpand = FALSE;
		LcomK     = 0;
		arElemList.RemoveAll();
	}
	_DGN_EXPAND_LOAD_LCOM& operator = (const _DGN_EXPAND_LOAD_LCOM& rData)
	{
		bIsExpand = rData.bIsExpand;
		LcomK     = rData.LcomK;
		POSITION pos = rData.arElemList.GetStartPosition();
		while(pos)
		{
			T_ELEM_K ElemK;
			_DGN_EXPAND_LOAD_ELEM ExElemD;
			rData.arElemList.GetNextAssoc(pos, ElemK, ExElemD);

			arElemList.SetAt(ElemK, ExElemD);
		}
		return *this;
	}
	void Add(_DGN_EXPAND_LOAD_ELEM ExElemD, int nType) // nType : 추가형식(0:누적, 1:Max)
	{
		_DGN_EXPAND_LOAD_ELEM ExElemD_Org;
		if(arElemList.Lookup(ExElemD.ElemK, ExElemD_Org))
		{
			if(!ExElemD_Org.bIsExpand)
			{ ExElemD_Org = ExElemD; }
			else
			{
				for(int i=0 ; i<3 ; i++)
				{
					if(!ExElemD_Org.bExPosi[i] && ExElemD.bExPosi[i])
					{
						ExElemD_Org.bExPosi[i] = ExElemD.bExPosi[i];
						ExElemD_Org.dFactor[i] = ExElemD.dFactor[i];
						ExElemD_Org.nExpandType[i] = ExElemD.nExpandType[i];
					}
					else if(ExElemD_Org.bExPosi[i] && ExElemD.bExPosi[i])
					{
						if(nType == 0)
						{
							if(ExElemD_Org.nExpandType[i] != ExElemD.nExpandType[i])
							{
								ExElemD_Org.dFactor[i] = ExElemD_Org.dFactor[i]*ExElemD.dFactor[i];
								ExElemD_Org.nExpandType[i] = 1;
							}
							else if(ExElemD_Org.nExpandType[i] == 0)
							{
								ExElemD_Org.dFactor[i] = ExElemD_Org.dFactor[i]*ExElemD.dFactor[i];
							}
							else if(ExElemD_Org.nExpandType[i] == 1)
							{
								ExElemD_Org.dFactor[i] = max(ExElemD_Org.dFactor[i],ExElemD.dFactor[i]);
							}
						}
						else
						{
							ExElemD_Org.dFactor[i] = max(ExElemD_Org.dFactor[i],ExElemD.dFactor[i]);
							if(ExElemD_Org.nExpandType[i] != ExElemD.nExpandType[i])
								ExElemD_Org.nExpandType[i] = 1;
						}
					}
				}
			}
			arElemList.SetAt(ExElemD.ElemK, ExElemD_Org);
		}
		else
		{
			arElemList.SetAt(ExElemD.ElemK, ExElemD);
		}
	}
};

struct _DGN_EXPAND_LOAD_D
{
	BOOL bIsExpand;
	CMapEx<T_LCOM_K, T_LCOM_K, _DGN_EXPAND_LOAD_LCOM, _DGN_EXPAND_LOAD_LCOM&> arLcomList;
	
	void Initialize()
	{
		bIsExpand = FALSE;
		arLcomList.RemoveAll();
	}
	_DGN_EXPAND_LOAD_D& operator = (const _DGN_EXPAND_LOAD_D& rData)
	{
		bIsExpand = rData.bIsExpand;
		POSITION pos = rData.arLcomList.GetStartPosition();
		while(pos)
		{
			T_LCOM_K LcomK;
			_DGN_EXPAND_LOAD_LCOM ExLcomD;
			rData.arLcomList.GetNextAssoc(pos, LcomK, ExLcomD);

			arLcomList.SetAt(LcomK, ExLcomD);
		}
		return *this;
	}
	void Merge(_DGN_EXPAND_LOAD_D& rData)
	{
		bIsExpand = (bIsExpand || rData.bIsExpand);
		POSITION pos = rData.arLcomList.GetStartPosition();
		while(pos)
		{
			T_LCOM_K LcomK;
			_DGN_EXPAND_LOAD_LCOM ExLcomD_Org, ExLcomD;
			rData.arLcomList.GetNextAssoc(pos, LcomK, ExLcomD);

			if(arLcomList.Lookup(LcomK, ExLcomD_Org))
			{
				if(ExLcomD_Org.bIsExpand)
				{
					POSITION posElem = ExLcomD.arElemList.GetStartPosition();
					while(pos)
					{
						T_ELEM_K ElemK;
						_DGN_EXPAND_LOAD_ELEM ExElemD;
						ExLcomD.arElemList.GetNextAssoc(posElem, ElemK, ExElemD);
						ExLcomD_Org.Add(ExElemD, 0);
					}
					arLcomList.SetAt(LcomK, ExLcomD_Org);
				}
				else
				{ arLcomList.SetAt(LcomK, ExLcomD); }
			}
			else
			{ arLcomList.SetAt(LcomK, ExLcomD); }
		}
		return;
	}
};

// ServiceAgency를 할때 검토할 Elem Key List를 넘겨주기 위한 구조체
struct _DGN_EXPAND_LOAD_D_FOR_SERV_AGEN
{
	BOOL bUserElem;
	int  nLcomType; // D_LCOMTYPE_STEEL, D_LCOMTYPE_CONCRETE...등
	T_ELEM_K_LIST arElemK;
	_DGN_EXPAND_LOAD_D ExpandLoadD;
	void Initialize()
	{
		bUserElem = FALSE;
		nLcomType = 0;
		arElemK.RemoveAll();
		ExpandLoadD.Initialize();
	}
	_DGN_EXPAND_LOAD_D_FOR_SERV_AGEN& operator = (const _DGN_EXPAND_LOAD_D_FOR_SERV_AGEN& rData)
	{
		bUserElem = rData.bUserElem;
		nLcomType = rData.nLcomType;
		arElemK.Copy(rData.arElemK);
		ExpandLoadD = rData.ExpandLoadD;
		return *this;
	}
};

// Lcom별 Story별 Brace분담률 β
struct _DGN_BRACE_BETA_STOR
{
	double dBeta[2];
	void Initialize()
	{
		dBeta[0] = 0.0;
		dBeta[1] = 0.0;
	}
};
struct _DGN_BRACE_BETA_LCOM
{
	int nDir; //0:X방향, 1:Y방향, 2:기타방향
	CMapEx<T_STOR_K, T_STOR_K, _DGN_BRACE_BETA_STOR, _DGN_BRACE_BETA_STOR&> arStorList;
	
	void Initialize() { nDir = 0;  arStorList.RemoveAll(); }
	_DGN_BRACE_BETA_LCOM& operator = (const _DGN_BRACE_BETA_LCOM& rData)
	{
		nDir = rData.nDir;
		POSITION pos = rData.arStorList.GetStartPosition();
		while(pos)
		{
			T_STOR_K StorK;
			_DGN_BRACE_BETA_STOR BetaStorD;
			rData.arStorList.GetNextAssoc(pos, StorK, BetaStorD);
			arStorList.SetAt(StorK, BetaStorD);
		}
		return *this;
	}
};
struct _DGN_BRACE_BETA_D
{
	CMapEx<T_LCOM_K, T_LCOM_K, _DGN_BRACE_BETA_LCOM, _DGN_BRACE_BETA_LCOM&> arLcomList;
	void Initialize() { arLcomList.RemoveAll(); }
	_DGN_BRACE_BETA_D& operator = (const _DGN_BRACE_BETA_D& rData)
	{
		POSITION pos = rData.arLcomList.GetStartPosition();
		while(pos)
		{
			T_LCOM_K LcomK;
			_DGN_BRACE_BETA_LCOM BetaLcomD;
			rData.arLcomList.GetNextAssoc(pos, LcomK, BetaLcomD);
			arLcomList.SetAt(LcomK, BetaLcomD);
		}
		return *this;
	}
};

struct _DGN_BRACE_BETA_D_FOR_SERV_AGEN
{
	int  nLcomType; // D_LCOMTYPE_STEEL, D_LCOMTYPE_CONCRETE...등
	_DGN_BRACE_BETA_D BraceBetaD;
	void Initialize()
	{
		nLcomType = 0;
		BraceBetaD.Initialize();
	}
	_DGN_BRACE_BETA_D_FOR_SERV_AGEN& operator = (const _DGN_BRACE_BETA_D_FOR_SERV_AGEN& rData)
	{
		nLcomType = rData.nLcomType;
		BraceBetaD = rData.BraceBetaD;
		return *this;
	}
};

struct _DGN_PSTR_LCOMRC
{
	// Total
	double FxxUF[2];
	double FyyUF[2];
	double FxyUF[2];
	double VxxUF[2];
	double VyyUF[2];
	double MxxUF[2];
	double MyyUF[2];
	double MxyUF[2];

	// DL
	double FxxUFDL[2];
	double FyyUFDL[2];
	double FxyUFDL[2];
	double VxxUFDL[2];
	double VyyUFDL[2];
	double MxxUFDL[2];
	double MyyUFDL[2];
	double MxyUFDL[2];

	// ML
	double FxxUFML[2];
	double FyyUFML[2];
	double FxyUFML[2];
	double VxxUFML[2];
	double VyyUFML[2];
	double MxxUFML[2];
	double MyyUFML[2];
	double MxyUFML[2];

	int    FxxUFKey[2];
	int    FyyUFKey[2];
	int    FxyUFKey[2];
	int    VxxUFKey[2];
	int    VyyUFKey[2];
	int    MxxUFKey[2];
	int    MyyUFKey[2];
	int    MxyUFKey[2];

	int    FxxUFDLKey[2];
	int    FyyUFDLKey[2];
	int    FxyUFDLKey[2];
	int    VxxUFDLKey[2];
	int    VyyUFDLKey[2];
	int    MxxUFDLKey[2];
	int    MyyUFDLKey[2];
	int    MxyUFDLKey[2];

	int    FxxUFMLKey[2];
	int    FyyUFMLKey[2];
	int    FxyUFMLKey[2];
	int    VxxUFMLKey[2];
	int    VyyUFMLKey[2];
	int    MxxUFMLKey[2];
	int    MyyUFMLKey[2];
	int    MxyUFMLKey[2];

	void Initialize()
	{
		for(int i=0 ; i<2 ; i++)
		{
			FxxUF[i]  =0.0;
			FyyUF[i]  =0.0;
			FxyUF[i]  =0.0;
			VxxUF[i]  =0.0;
			VyyUF[i]  =0.0;
			MxxUF[i]  =0.0;
			MyyUF[i]  =0.0;
			MxyUF[i]  =0.0;
			FxxUFDL[i]  =0.0;
			FyyUFDL[i]  =0.0;
			FxyUFDL[i]  =0.0;
			VxxUFDL[i]  =0.0;
			VyyUFDL[i]  =0.0;
			MxxUFDL[i]  =0.0;
			MyyUFDL[i]  =0.0;
			MxyUFDL[i]  =0.0;
			FxxUFML[i]  =0.0;
			FyyUFML[i]  =0.0;
			FxyUFML[i]  =0.0;
			VxxUFML[i]  =0.0;
			VyyUFML[i]  =0.0;
			MxxUFML[i]=0.0;
			MyyUFML[i]=0.0;
			MxyUFML[i]=0.0;

			FxxUFKey[i]  =0;
			FyyUFKey[i]  =0;
			FxyUFKey[i]  =0;
			VxxUFKey[i]  =0;
			VyyUFKey[i]  =0;
			MxxUFKey[i]  =0;
			MyyUFKey[i]  =0;
			MxyUFKey[i]  =0;
			FxxUFDLKey[i]  =0;
			FyyUFDLKey[i]  =0;
			FxyUFDLKey[i]  =0;
			VxxUFDLKey[i]  =0;
			VyyUFDLKey[i]  =0;
			MxxUFDLKey[i]  =0;
			MyyUFDLKey[i]  =0;
			MxyUFDLKey[i]  =0;

			FxxUFMLKey[i]  =0;
			FyyUFMLKey[i]  =0;
			FxyUFMLKey[i]  =0;
			VxxUFMLKey[i]  =0;
			VyyUFMLKey[i]  =0;
			MxxUFMLKey[i]=0;
			MyyUFMLKey[i]=0;
			MxyUFMLKey[i]=0;
		}
	}
	_DGN_PSTR_LCOMRC()	{}
	_DGN_PSTR_LCOMRC(const _DGN_PSTR_LCOMRC& rData)	{*this = rData;}
	_DGN_PSTR_LCOMRC& operator = (const _DGN_PSTR_LCOMRC& rData)
	{
		for(int i=0 ; i<2 ; i++)
		{
			FxxUF[i]  =rData.FxxUF[i]  ;
			FyyUF[i]  =rData.FyyUF[i]  ;
			FxyUF[i]  =rData.FxyUF[i]  ;
			VxxUF[i]  =rData.VxxUF[i]  ;
			VyyUF[i]  =rData.VyyUF[i]  ;
			MxxUF[i]  =rData.MxxUF[i]  ;
			MyyUF[i]  =rData.MyyUF[i]  ;
			MxyUF[i]  =rData.MxyUF[i]  ;
			FxxUFDL[i]  =rData.FxxUFDL[i]  ;
			FyyUFDL[i]  =rData.FyyUFDL[i]  ;
			FxyUFDL[i]  =rData.FxyUFDL[i]  ;
			VxxUFDL[i]  =rData.VxxUFDL[i]  ;
			VyyUFDL[i]  =rData.VyyUFDL[i]  ;
			MxxUFDL[i]  =rData.MxxUFDL[i]  ;
			MyyUFDL[i]  =rData.MyyUFDL[i]  ;
			MxyUFDL[i]  =rData.MxyUFDL[i]  ;
			FxxUFML[i]  =rData.FxxUFML[i]  ;
			FyyUFML[i]  =rData.FyyUFML[i]  ;
			FxyUFML[i]  =rData.FxyUFML[i]  ;
			VxxUFML[i]  =rData.VxxUFML[i]  ;
			VyyUFML[i]  =rData.VyyUFML[i]  ;
			MxxUFML[i]=rData.MxxUFML[i];
			MyyUFML[i]=rData.MyyUFML[i];
			MxyUFML[i]=rData.MxyUFML[i];

			FxxUFKey[i]  =rData.FxxUFKey[i]  ;
			FyyUFKey[i]  =rData.FyyUFKey[i]  ;
			FxyUFKey[i]  =rData.FxyUFKey[i]  ;
			VxxUFKey[i]  =rData.VxxUFKey[i]  ;
			VyyUFKey[i]  =rData.VyyUFKey[i]  ;
			MxxUFKey[i]  =rData.MxxUFKey[i]  ;
			MyyUFKey[i]  =rData.MyyUFKey[i]  ;
			MxyUFKey[i]  =rData.MxyUFKey[i]  ;
			FxxUFDLKey[i]  =rData.FxxUFDLKey[i]  ;
			FyyUFDLKey[i]  =rData.FyyUFDLKey[i]  ;
			FxyUFDLKey[i]  =rData.FxyUFDLKey[i]  ;
			VxxUFDLKey[i]  =rData.VxxUFDLKey[i]  ;
			VyyUFDLKey[i]  =rData.VyyUFDLKey[i]  ;
			MxxUFDLKey[i]  =rData.MxxUFDLKey[i]  ;
			MyyUFDLKey[i]  =rData.MyyUFDLKey[i]  ;
			MxyUFDLKey[i]  =rData.MxyUFDLKey[i]  ;
			FxxUFMLKey[i]  =rData.FxxUFMLKey[i]  ;
			FyyUFMLKey[i]  =rData.FyyUFMLKey[i]  ;
			FxyUFMLKey[i]  =rData.FxyUFMLKey[i]  ;
			VxxUFMLKey[i]  =rData.VxxUFMLKey[i]  ;
			VyyUFMLKey[i]  =rData.VyyUFMLKey[i]  ;
			MxxUFMLKey[i]=rData.MxxUFMLKey[i];
			MyyUFMLKey[i]=rData.MyyUFMLKey[i];
			MxyUFMLKey[i]=rData.MxyUFMLKey[i];
		}
		return *this;
	}
};

struct PLATE_UNIT_FORCE
{
	double dFxxD, dFxxL, dFxxR, dFxxML, dFxxTP, dFxxTS, dFxxLU;
	double dFyyD, dFyyL, dFyyR, dFyyML, dFyyTP, dFyyTS, dFyyLU;
	double dFxyD, dFxyL, dFxyR, dFxyML, dFxyTP, dFxyTS, dFxyLU;
	double dVxxD, dVxxL, dVxxR, dVxxML, dVxxTP, dVxxTS, dVxxLU;
	double dVyyD, dVyyL, dVyyR, dVyyML, dVyyTP, dVyyTS, dVyyLU;
	double dMxxD, dMxxL, dMxxR, dMxxML, dMxxTP, dMxxTS, dMxxLU;
	double dMyyD, dMyyL, dMyyR, dMyyML, dMyyTP, dMyyTS, dMyyLU;
	double dMxyD, dMxyL, dMxyR, dMxyML, dMxyTP, dMxyTS, dMxyLU;

	void Initialize()
	{
		dFxxD=0.0, dFxxL=0.0, dFxxR=0.0, dFxxML=0.0, dFxxTP=0.0, dFxxTS=0.0, dFxxLU=0.0;
		dFyyD=0.0, dFyyL=0.0, dFyyR=0.0, dFyyML=0.0, dFyyTP=0.0, dFyyTS=0.0, dFyyLU=0.0;
		dFxyD=0.0, dFxyL=0.0, dFxyR=0.0, dFxyML=0.0, dFxyTP=0.0, dFxyTS=0.0, dFxyLU=0.0;
		dVxxD=0.0, dVxxL=0.0, dVxxR=0.0, dVxxML=0.0, dVxxTP=0.0, dVxxTS=0.0, dVxxLU=0.0;
		dVyyD=0.0, dVyyL=0.0, dVyyR=0.0, dVyyML=0.0, dVyyTP=0.0, dVyyTS=0.0, dVyyLU=0.0;
		dMxxD=0.0, dMxxL=0.0, dMxxR=0.0, dMxxML=0.0, dMxxTP=0.0, dMxxTS=0.0, dMxxLU=0.0;
		dMyyD=0.0, dMyyL=0.0, dMyyR=0.0, dMyyML=0.0, dMyyTP=0.0, dMyyTS=0.0, dMyyLU=0.0;
		dMxyD=0.0, dMxyL=0.0, dMxyR=0.0, dMxyML=0.0, dMxyTP=0.0, dMxyTS=0.0, dMxyLU=0.0;
	}
	PLATE_UNIT_FORCE& operator= (const PLATE_UNIT_FORCE& src)
	{
		dFxxD = src.dFxxD, dFxxL = src.dFxxL, dFxxR = src.dFxxR, dFxxML = src.dFxxML, dFxxTP = src.dFxxTP, dFxxTS = src.dFxxTS, dFxxLU = src.dFxxLU;
		dFyyD = src.dFyyD, dFyyL = src.dFyyL, dFyyR = src.dFyyR, dFyyML = src.dFyyML, dFyyTP = src.dFyyTP, dFyyTS = src.dFyyTS, dFyyLU = src.dFyyLU;
		dFxyD = src.dFxyD, dFxyL = src.dFxyL, dFxyR = src.dFxyR, dFxyML = src.dFxyML, dFxyTP = src.dFxyTP, dFxyTS = src.dFxyTS, dFxyLU = src.dFxyLU;
		dVxxD = src.dVxxD, dVxxL = src.dVxxL, dVxxR = src.dVxxR, dVxxML = src.dVxxML, dVxxTP = src.dVxxTP, dVxxTS = src.dVxxTS, dVxxLU = src.dVxxLU;
		dVyyD = src.dVyyD, dVyyL = src.dVyyL, dVyyR = src.dVyyR, dVyyML = src.dVyyML, dVyyTP = src.dVyyTP, dVyyTS = src.dVyyTS, dVyyLU = src.dVyyLU;
		dMxxD = src.dMxxD, dMxxL = src.dMxxL, dMxxR = src.dMxxR, dMxxML = src.dMxxML, dMxxTP = src.dMxxTP, dMxxTS = src.dMxxTS, dMxxLU = src.dMxxLU;
		dMyyD = src.dMyyD, dMyyL = src.dMyyL, dMyyR = src.dMyyR, dMyyML = src.dMyyML, dMyyTP = src.dMyyTP, dMyyTS = src.dMyyTS, dMyyLU = src.dMyyLU;
		dMxyD = src.dMxyD, dMxyL = src.dMxyL, dMxyR = src.dMxyR, dMxyML = src.dMxyML, dMxyTP = src.dMxyTP, dMxyTS = src.dMxyTS, dMxyLU = src.dMxyLU;

		return *this;
	}
};

#endif // !defined(__DGNSTRUCT_H__)

#pragma once

#ifndef _TENDON_AUTO_GEN_DEF_H_
#define _TENDON_AUTO_GEN_DEF_H_

#define TOL_SECT_COMP 0.008
#define TOL_VALU_COMP 1E-5
#define TEMP_ARY_END -1000000.0

//tempInfo, bXY, dt, bSymm, bAdd
#define STRAIGHT_DT(tempInfo, bXY, dt, bSymm, bAdd)	{ \
	double dOffset = m_AutoInfo.GetOffset(bXY, bSymm); \
	if(bXY) { \
	tempInfo.tndnSubXY.aSpanInfo[0].dDt = dt + dOffset; \
	tempInfo.tndnSubXY.aSpanInfo[0].dDt1 = dt + dOffset; \
	tempInfo.tndnSubXY.bXSymmetry = bSymm; } \
	else { \
	tempInfo.tndnSubXZ.aSpanInfo[0].dDt = dt + dOffset; \
	tempInfo.tndnSubXZ.aSpanInfo[0].dDt1 = dt + dOffset; \
	tempInfo.tndnSubXZ.bXSymmetry = bSymm; } \
	if(bAdd) { \
	m_pTemplateCtrl->GetNewName(m_strPrefix, tempInfo.strName); \
	m_pTemplateCtrl->AddTemplateNU(tempInfo); } }

#define SETPOINT_SYMM(vertical, horizon, dStempLen, tempInfo) { dStempLen[0] = horizon;\
	SetLine(vertical, dStempLen, 1, &tempInfo, FALSE);\
	SetLine(vertical, dStempLen, 1, &tempInfo, TRUE);}

#define DEF_DECLARE_FUNCS(funcName) BOOL AutoGenTndn_##funcName## (const T_AUTOTNDN_ARG& tArg);\
	static BOOL AutoGenFuncTndn_##funcName##(const T_AUTOTNDN_ARG& tArg) { return tArg.pCtrl->AutoGenTndn_##funcName##(tArg); }

// 공통으로 처리를 위한 자동 생성 함수 인자
class CDgnTendonAutoGenCtrl;
struct T_AUTOTNDN_ARG
{
	int  nAdditionalNum;
	BOOL bRev;
	int  nType;
	CDgnTendonAutoGenCtrl* pCtrl;

	T_AUTOTNDN_ARG(CDgnTendonAutoGenCtrl* pC)
	{
		pCtrl = pC;
		Init();
	}

	virtual ~T_AUTOTNDN_ARG() {}

	void Init(int nAN = 0, BOOL bR = FALSE, int nT = 0)
	{
		nAdditionalNum = nAN;
		bRev = bR;
		nType = nT;
	}
};
typedef BOOL(*pAutoGenFunc)(const T_AUTOTNDN_ARG&);

//AutoGen지원하는 국가 늘어나면 SIZE 늘리기.
enum IDX_STR_LIST
{
	IDX_STR_LIST_UK,
	IDX_STR_LIST_Italy,
	IDX_STR_LIST_USA,
	IDX_STR_LIST_AS,
	IDX_STR_LIST_NZ,
	IDX_STR_LIST_SIZE,
};

enum EN_SECT_TYPE_STRIDX_UK
{
	EN_SECT_TYPE_STRIDX_UK_M,
	EN_SECT_TYPE_STRIDX_UK_SY,
	EN_SECT_TYPE_STRIDX_UK_U,
	EN_SECT_TYPE_STRIDX_UK_Y,
	EN_SECT_TYPE_STRIDX_UK_YE,
	EN_SECT_TYPE_STRIDX_UK_W,
	EN_SECT_TYPE_STRIDX_UK_UMB,
	EN_SECT_TYPE_STRIDX_UK_MY,
	EN_SECT_TYPE_STRIDX_UK_MYE,
	EN_SECT_TYPE_STRIDX_UK_SD,
	EN_SECT_TYPE_STRIDX_UK_SYE,
	EN_SECT_TYPE_STRIDX_UK_TYE_S,
	EN_SECT_TYPE_STRIDX_UK_TYE_B,
	EN_SECT_TYPE_STRIDX_UK_TY_S,
	EN_SECT_TYPE_STRIDX_UK_TY_B,
	EN_SECT_TYPE_STRIDX_UK_T,
	EN_SECT_TYPE_STRIDX_UK_U_SU,
	EN_SECT_TYPE_STRIDX_UK_NUBMER,
};

enum EN_SECT_TYPE_STRIDX_Italy
{
	EN_SECT_TYPE_STRIDX_Italy_VH,
	EN_SECT_TYPE_STRIDX_Italy_NUMBER,
};

enum EN_SECT_TYPE_STRIDX_USA
{
	EN_SECT_TYPE_STRIDX_USA_AASHTO,
	EN_SECT_TYPE_STRIDX_USA_ILDOT,
	EN_SECT_TYPE_STRIDX_USA_massDOT,
	EN_SECT_TYPE_STRIDX_USA_MoDOT,
	EN_SECT_TYPE_STRIDX_USA_LADOT,
	EN_SECT_TYPE_STRIDX_USA_ODOT,
	EN_SECT_TYPE_STRIDX_USA_RIDOT,
	EN_SECT_TYPE_STRIDX_USA_TXDOT,
	EN_SECT_TYPE_STRIDX_USA_NUBMER,
};

enum EN_SECT_TYPE_STRIDX_AS
{
	EN_SECT_TYPE_STRIDX_AS_PSC_Deck_Unit_TMR_QLD,
	EN_SECT_TYPE_STRIDX_AS_SUPER_T_RMS_2019,
	EN_SECT_TYPE_STRIDX_AS_SUPER_T,
	EN_SECT_TYPE_STRIDX_AS_PLANK_GIRDER,
	EN_SECT_TYPE_STRIDX_AS_NUBMER,
};

enum EN_SECT_TYPE_STRIDX_NZ
{
	EN_SECT_TYPE_STRIDX_NZ_SUPER_T,
	EN_SECT_TYPE_STRIDX_NZ_I_GIRDER,
	EN_SECT_TYPE_STRIDX_NZ_HOLLOW_CORE,
	EN_SECT_TYPE_STRIDX_NZ_SUPERHI,
	EN_SECT_TYPE_STRIDX_NZ_NUBMER,
};

struct T_AUTOTNDN_INFO
{
	//Input
	CString strDBCode;
	CString strDBName;
	CString strDBNameSub;
	double  dOrigin[2]; //Y, Z 좌표.

	double  dOffsetTop;
	double  dOffsetBottom;
	double  dOffsetLeft;
	double  dOffsetRight;

	T_AUTOTNDN_INFO()
	{
		Init();
	}

	void Init()
	{
		strDBCode.Empty();
		strDBName.Empty();
		strDBNameSub.Empty();
		dOrigin[0] = 0.0;
		dOrigin[1] = 0.0;

		dOffsetTop    = 0.0;
		dOffsetBottom = 0.0;
		dOffsetLeft   = 0.0;
		dOffsetRight  = 0.0;
	}

	T_AUTOTNDN_INFO& operator=(const T_AUTOTNDN_INFO& src)
	{
		strDBCode     = src.strDBCode;
		strDBName     = src.strDBName;
		strDBNameSub  = src.strDBNameSub;
		dOrigin[0]    = src.dOrigin[0];
		dOrigin[1]    = src.dOrigin[1];

		dOffsetTop    = src.dOffsetTop;
		dOffsetBottom = src.dOffsetBottom;
		dOffsetLeft   = src.dOffsetLeft;
		dOffsetRight  = src.dOffsetRight;
		return *this;
	}

	double GetOffset(BOOL bXY, BOOL bSymm)
	{
		if(bXY)
		{
			if(bSymm) return dOffsetLeft;
			else      return dOffsetRight;
		}
		else
		{
			if(bSymm) return dOffsetBottom;
			else      return dOffsetTop;
		}
		return 0.0;
	}
};

#endif //_TENDON_AUTO_GEN_DEF_H_
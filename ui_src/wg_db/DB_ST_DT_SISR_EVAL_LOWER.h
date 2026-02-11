#ifndef __DB_ST_DT_SISR_EVAL_LOWERH__
#define __DB_ST_DT_SISR_EVAL_LOWERH__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT_EVAL_KEYS.h"

struct T_SISR_EVAL_LOWER_BASE
{
	bool bAutoCapa;
	double dB;		//교각 본체 단면의 폭 [L]
	double dD;		//교각 본체 단면의 유효높이 [L]
	double dPt;		//인장철근비 [%]
	double dCe;		//교각본체단면의 유효높이d 에 대한 보정계수
	double dCpt;	//축방향 인장철근비 Pt에 관한 보정계수
	double dCdc;	//전단 스팬에 의해 콘크리트가 부담하는 전단내력의 할증계수
	double dCds;	//전단 스팬에 의해 띠철근이 부담하는 전단내력의 저감계수
	double dSc;		//콘크리트가 부담하는 전단내력 [F]
	double dSs;		//철근이 부담하는 전단내력 [F]
	double dPs;		//전단내력 [F]
	double dShear;
	double dMoment;
	double dRatio;
	CArray<double, double> adShear;
	CArray<double, double> adMoment;

	T_SISR_EVAL_LOWER_BASE() { Initialize(); }
	T_SISR_EVAL_LOWER_BASE(const T_SISR_EVAL_LOWER_BASE& src) { *this = src; }
	T_SISR_EVAL_LOWER_BASE& operator= (const T_SISR_EVAL_LOWER_BASE& src)
	{
		if (this != &src)
		{
			bAutoCapa = src.bAutoCapa;
			dB = src.dB;
			dD = src.dD;
			dPt = src.dPt;
			dCe = src.dCe;
			dCpt = src.dCpt;
			dCdc = src.dCdc;
			dCds = src.dCds;
			dSc = src.dSc;
			dSs = src.dSs;
			dPs = src.dPs;
			dShear = src.dShear;
			dMoment = src.dMoment;
			dRatio = src.dRatio;
			adShear.Copy(src.adShear);
			adMoment.Copy(src.adMoment);
		}

		return *this;
	}

	void Initialize()
	{
		bAutoCapa = true;
		dB = 0.0;
		dD = 0.0;
		dPt = 0.0;
		dCe = 0.0;
		dCpt = 0.0;
		dCdc = 0.0;
		dCds = 0.0;
		dSc = 0.0;
		dSs = 0.0;
		dPs = 0.0;
		dShear = 0.0;
		dMoment = 0.0;
		dRatio = 0.0;
		adShear.RemoveAll();
		adMoment.RemoveAll();
	}

	enum eDirection
	{
		kUnKnown,
		kMax,
		kMin,
	};
};

struct T_SISR_EVAL_LOWER_BASE_RW
{
	bool bAutoCapa;
	double dB;		//교각 본체 단면의 폭 [L]
	double dD;		//교각 본체 단면의 유효높이 [L]
	double dPt;		//인장철근비 [%]
	double dCe;		//교각본체단면의 유효높이d 에 대한 보정계수
	double dCpt;	//축방향 인장철근비 Pt에 관한 보정계수
	double dCdc;	//전단 스팬에 의해 콘크리트가 부담하는 전단내력의 할증계수
	double dCds;	//전단 스팬에 의해 띠철근이 부담하는 전단내력의 저감계수
	double dSc;		//콘크리트가 부담하는 전단내력 [F]
	double dSs;		//철근이 부담하는 전단내력 [F]
	double dPs;		//전단내력 [F]
	double dShear;
	double dMoment;
	double dRatio;
	int nCountShear;
	int nCountMoment;

	T_SISR_EVAL_LOWER_BASE_RW() { Initialize(); }
	T_SISR_EVAL_LOWER_BASE_RW(const T_SISR_EVAL_LOWER_BASE_RW& src) { *this = src; }
	T_SISR_EVAL_LOWER_BASE_RW& operator= (const T_SISR_EVAL_LOWER_BASE_RW& src)
	{
		if (this != &src)
		{
			bAutoCapa = src.bAutoCapa;
			dB = src.dB;
			dD = src.dD;
			dPt = src.dPt;
			dCe = src.dCe;
			dCpt = src.dCpt;
			dCdc = src.dCdc;
			dCds = src.dCds;
			dSc = src.dSc;
			dSs = src.dSs;
			dPs = src.dPs;
			dShear = src.dShear;
			dMoment = src.dMoment;
			dRatio = src.dRatio;
			nCountShear = src.nCountShear;
			nCountMoment = src.nCountMoment;
		}

		return *this;
	}

	void Get(T_SISR_EVAL_LOWER_BASE& rTar)
	{
		rTar.bAutoCapa = bAutoCapa;
		rTar.dB = dB;
		rTar.dD = dD;
		rTar.dPt = dPt;
		rTar.dCe = dCe;
		rTar.dCpt = dCpt;
		rTar.dCdc = dCdc;
		rTar.dCds = dCds;
		rTar.dSc = dSc;
		rTar.dSs = dSs;
		rTar.dPs = dPs;
		rTar.dShear = dShear;
		rTar.dMoment = dMoment;
		rTar.dRatio = dRatio;
	}

	void Set(const T_SISR_EVAL_LOWER_BASE& src)
	{
		bAutoCapa = src.bAutoCapa;
		dB = src.dB;
		dD = src.dD;
		dPt = src.dPt;
		dCe = src.dCe;
		dCpt = src.dCpt;
		dCdc = src.dCdc;
		dCds = src.dCds;
		dSc = src.dSc;
		dSs = src.dSs;
		dPs = src.dPs;
		dShear = src.dShear;
		dMoment = src.dMoment;
		dRatio = src.dRatio;
		nCountShear = src.adShear.GetCount();
		nCountMoment = src.adMoment.GetCount();
	}

	void Initialize()
	{
		bAutoCapa = true;
		dB = 0.0;
		dD = 0.0;
		dPt = 0.0;
		dCe = 0.0;
		dCpt = 0.0;
		dCdc = 0.0;
		dCds = 0.0;
		dSc = 0.0;
		dSs = 0.0;
		dPs = 0.0;
		dShear = 0.0;
		dMoment = 0.0;
		dRatio = 0.0;
		nCountShear = 0;
		nCountMoment = 0;
	}
};

struct T_SISR_EVAL_LOWER_POS
{
	T_NODE_K kNode;		
	T_SISR_EVAL_LOWER_BASE RsltMax;
	T_SISR_EVAL_LOWER_BASE RsltMin;
	
	T_SISR_EVAL_LOWER_POS() { Initialize(); }
	T_SISR_EVAL_LOWER_POS(const T_SISR_EVAL_LOWER_POS& src) { *this = src; }
	T_SISR_EVAL_LOWER_POS& operator= (const T_SISR_EVAL_LOWER_POS& src)
	{
		if (this != &src) {
			kNode = src.kNode;			
			RsltMax = src.RsltMax;
			RsltMin = src.RsltMin;
		}
		return *this;
	}

	void Initialize()
	{
		kNode = 0;
		RsltMax.Initialize();
		RsltMin.Initialize();
	}
};

struct T_SISR_EVAL_LOWER_POS_RW
{
	T_NODE_K kNode;	
	T_SISR_EVAL_LOWER_BASE_RW RsltMax;
	T_SISR_EVAL_LOWER_BASE_RW RsltMin;

	T_SISR_EVAL_LOWER_POS_RW() { Initialize(); }
	T_SISR_EVAL_LOWER_POS_RW(const T_SISR_EVAL_LOWER_POS_RW& src) { *this = src; }
	T_SISR_EVAL_LOWER_POS_RW& operator= (const T_SISR_EVAL_LOWER_POS_RW& src)
	{
		if (this != &src) {
			kNode = src.kNode;			
			RsltMax = src.RsltMax;
			RsltMin = src.RsltMin;
		}
		return *this;
	}

	void Get(T_SISR_EVAL_LOWER_POS& rTar)
	{
		rTar.kNode = kNode;
		RsltMax.Get(rTar.RsltMax);
		RsltMin.Get(rTar.RsltMin);
	}
	void Set(const T_SISR_EVAL_LOWER_POS& src)
	{
		kNode = src.kNode;		
		RsltMax.Set(src.RsltMax);
		RsltMin.Set(src.RsltMin);
	}

	void Initialize()
	{
		kNode = 0;		
		RsltMax.Initialize();
		RsltMin.Initialize();
	}
};

//교각에 대한 전단조사 - 하부구조(H14/24 공통)
struct T_SISR_EVAL_LOWER
{
	UINT kSipa;
	UINT kSigr;
	UINT kElem;
	int nMemberType;			//부재 타입 : enum DB_MEMB_TYPE
	int nMaterialType;			//교각의 재료 타입 : enum T_SIGR_LOWER_D::eMaterialType
	int nExamShearDir;			//검토된 기준 전단력 방향 : enum seis_jp::eDirShear
	T_SISR_EVAL_LOWER_POS PosI;
	T_SISR_EVAL_LOWER_POS PosJ;
	
	T_SISR_EVAL_LOWER() { Initialize(); }
	T_SISR_EVAL_LOWER(const T_SISR_EVAL_LOWER& src) { *this = src; }
	T_SISR_EVAL_LOWER& operator= (const T_SISR_EVAL_LOWER& src)
	{
		if (this != &src) {
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			kElem = src.kElem;
			nMemberType = src.nMemberType;
			nMaterialType = src.nMaterialType;
			nExamShearDir = src.nExamShearDir;
			PosI = src.PosI;
			PosJ = src.PosJ;
		}
		return *this;
	}

	void Initialize()
	{
		kSipa = 0;
		kSigr = 0;
		kElem = 0;
		nMemberType = 0;
		nMaterialType = 0;
		nExamShearDir = 0;
		PosI.Initialize();
		PosJ.Initialize();	
	}
};

struct T_SISR_EVAL_LOWER_RW
{
	UINT kSipa;
	UINT kSigr;
	UINT kElem;
	int nMemberType;
	int nMaterialType;
	int nExamShearDir;
	T_SISR_EVAL_LOWER_POS_RW PosI;
	T_SISR_EVAL_LOWER_POS_RW PosJ;

	T_SISR_EVAL_LOWER_RW() { Initialize(); }
	T_SISR_EVAL_LOWER_RW(const T_SISR_EVAL_LOWER_RW& src) { *this = src; }
	T_SISR_EVAL_LOWER_RW& operator= (const T_SISR_EVAL_LOWER_RW& src)
	{
		if (this != &src) {
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			kElem = src.kElem;
			nMemberType = src.nMemberType;
			nMaterialType = src.nMaterialType;
			nExamShearDir = src.nExamShearDir;
			PosI = src.PosI;
			PosJ = src.PosJ;
		}
		return *this;
	}

	void Get(T_SISR_EVAL_LOWER& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		rData.kElem = kElem;
		rData.nMemberType = nMemberType;
		rData.nMaterialType = nMaterialType;
		rData.nExamShearDir = nExamShearDir;
		PosI.Get(rData.PosI);
		PosJ.Get(rData.PosJ);
	}
	void Set(const T_SISR_EVAL_LOWER& data)
	{
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		kElem = data.kElem;
		nMemberType = data.nMemberType;
		nMaterialType = data.nMaterialType;
		nExamShearDir = data.nExamShearDir;
		PosI.Set(data.PosI);
		PosJ.Set(data.PosJ);
	}

	void Initialize()
	{
		kSipa = 0;
		kSigr = 0;
		kElem = 0;
		nMemberType = 0;
		nMaterialType = 0;
		nExamShearDir = 0;
		PosI.Initialize();
		PosJ.Initialize();
	}
};

#pragma region  /// 하부구조 기둥 단락부 전단내력 결과 : 출력방식 통일을 위해 "T_SIBD_EVAL_REBAR_CUT"와 같은 형태로.

struct T_SISR_EVAL_REBAR_CUT_VALUE
{
	double dSmax;
	double dPs;
	double dRatio;
	BOOL bCheckResult;

	T_SISR_EVAL_REBAR_CUT_VALUE() { Initialize(); }
	T_SISR_EVAL_REBAR_CUT_VALUE(const T_SISR_EVAL_REBAR_CUT_VALUE& src) { *this = src; }
	T_SISR_EVAL_REBAR_CUT_VALUE& operator= (const T_SISR_EVAL_REBAR_CUT_VALUE& src)
	{
		if (this != &src)
		{
			dSmax = src.dSmax;
			dPs = src.dPs;
			dRatio = src.dRatio;
			bCheckResult = src.bCheckResult;
		}

		return *this;
	}

	void Initialize()
	{
		dSmax = 0.0;
		dPs = 0.0;
		dRatio = 0.0;
		bCheckResult = FALSE;
	}
};

struct T_SISR_EVAL_REBAR_CUT
{
	UINT kSipa;
	UINT kSigr;
	UINT kElem;
	CString strSigrName;
	CString strColmName;
	int nRebarCutPos;			//철근 단락부 위치 T_SIGR_LOWER_COLM_D::ePosType, CDgnToolMPhi_SeismicDesign_JP::ePos
	int nSeismicForceDirection; //지진력 방향 : enum T_SIPA_D::eSeismicForceDirection
	int nSeismicForceType;		//지진력 타입 : enum T_SIPA_D::eSeismicForceType
	int nExamShearDir;			//검토된 기준 전단력 방향 : enum seis_jp::eDirShear
	T_SISR_EVAL_REBAR_CUT_VALUE RsltMax;
	T_SISR_EVAL_REBAR_CUT_VALUE RsltMin;

	T_SISR_EVAL_REBAR_CUT() { Initialize(); }
	T_SISR_EVAL_REBAR_CUT(const T_SISR_EVAL_REBAR_CUT& src) { *this = src; }
	T_SISR_EVAL_REBAR_CUT& operator= (const T_SISR_EVAL_REBAR_CUT& src)
	{
		if (this != &src) {
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			kElem = src.kElem;
			strSigrName = src.strSigrName;
			strColmName = src.strColmName;
			nRebarCutPos = src.nRebarCutPos;
			nSeismicForceDirection = src.nSeismicForceDirection;
			nSeismicForceType = src.nSeismicForceType;
			nExamShearDir = src.nExamShearDir;
			RsltMax = src.RsltMax;
			RsltMin = src.RsltMin;
		}
		return *this;
	}

	void Initialize()
	{
		kSipa = 0;
		kSigr = 0;
		kElem = 0;
		strSigrName = _T("");
		strColmName = _T("");
		nRebarCutPos = 0;
		nSeismicForceDirection = 0;
		nSeismicForceType = 0;
		nExamShearDir = 0;
		RsltMax.Initialize();
		RsltMin.Initialize();
	}
};

struct T_SISR_EVAL_REBAR_CUT_CH
{
	UINT kSipa;
	UINT kSigr;
	UINT kElem;
	char strSigrName[40];
	char strColmName[60];
	int nRebarCutPos;
	int nSeismicForceDirection;
	int nSeismicForceType;
	int nExamShearDir;
	T_SISR_EVAL_REBAR_CUT_VALUE RsltMax;
	T_SISR_EVAL_REBAR_CUT_VALUE RsltMin;

	void Get(T_SISR_EVAL_REBAR_CUT& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		rData.kElem = kElem;
		ConvertCharStr(strSigrName, rData.strSigrName, sizeof(strSigrName));
		ConvertCharStr(strColmName, rData.strColmName, sizeof(strColmName));
		rData.nRebarCutPos = nRebarCutPos;
		rData.nSeismicForceDirection = nSeismicForceDirection;
		rData.nSeismicForceType = nSeismicForceType;
		rData.nExamShearDir = nExamShearDir;
		rData.RsltMax = RsltMax;
		rData.RsltMin = RsltMin;
	}

	void Set(/*const*/ T_SISR_EVAL_REBAR_CUT& data)
	{
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		kElem = data.kElem;
		ConvertStrChar(data.strSigrName, strSigrName, sizeof(strSigrName));
		ConvertStrChar(data.strColmName, strColmName, sizeof(strColmName));
		nRebarCutPos = data.nRebarCutPos;
		nSeismicForceDirection = data.nSeismicForceDirection;
		nSeismicForceType = data.nSeismicForceType;
		nExamShearDir = data.nExamShearDir;
		RsltMax = data.RsltMax;
		RsltMin = data.RsltMin;
	}
};

#pragma endregion

#endif // !defined(__DB_ST_DT_SISR_EVAL_LOWERH__)
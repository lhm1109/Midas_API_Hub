#ifndef __DB_ST_DT_SISR_EVAL_DIAG__
#define __DB_ST_DT_SISR_EVAL_DIAG__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT_EVAL_KEYS.h"

struct T_SISR_EVAL_ROAD3_CAPA_DIAG
{
	double dB;
	double dD;
	double dM0;
	double dMd;
	double dK;
	double dSc;
	double dSs;
	double dSp;
	double dSus;
	double dRatio;
	
	T_SISR_EVAL_ROAD3_CAPA_DIAG() { Initialize(); }
	T_SISR_EVAL_ROAD3_CAPA_DIAG(const T_SISR_EVAL_ROAD3_CAPA_DIAG& src) { *this = src; }
	T_SISR_EVAL_ROAD3_CAPA_DIAG& operator= (const T_SISR_EVAL_ROAD3_CAPA_DIAG& src)
	{
		if (this != &src) {
			dB = src.dB;
			dD = src.dD;
			dM0 = src.dM0;
			dMd = src.dMd;
			dK = src.dK;
			dSc = src.dSc;
			dSs = src.dSs;
			dSp = src.dSp;
			dSus = src.dSus;			
			dRatio = src.dRatio;
		}
		return *this;
	}

	void Initialize()
	{
		dB = 0.0;
		dD = 0.0;
		dM0 = 0.0;
		dMd = 0.0;
		dK = 0.0;
		dSc = 0.0;
		dSs = 0.0;
		dSp = 0.0;
		dSus = 0.0;		
		dRatio = 0.0;
	}
};

struct T_SISR_EVAL_ROAD3_CAPA_COMP
{
	double dB;
	double dD;
	double dTaumax;
	double dAp;
	double dSigmape;
	double dSucw;
	double dSp;
	double dSuc;
	double dRatio;

	T_SISR_EVAL_ROAD3_CAPA_COMP() { Initialize(); }
	T_SISR_EVAL_ROAD3_CAPA_COMP(const T_SISR_EVAL_ROAD3_CAPA_COMP& src) { *this = src; }
	T_SISR_EVAL_ROAD3_CAPA_COMP& operator= (const T_SISR_EVAL_ROAD3_CAPA_COMP& src)
	{
		if (this != &src) {
			dB = src.dB;
			dD = src.dD;
			dTaumax = src.dTaumax;
			dAp = src.dAp;
			dSigmape = src.dSigmape;
			dSucw = src.dSucw;			
			dSp = src.dSp;
			dSuc = src.dSuc;
			dRatio = src.dRatio;
		}
		return *this;
	}

	void Initialize()
	{
		dB = 0.0;
		dD = 0.0;
		dTaumax = 0.0;
		dAp = 0.0;
		dSigmape = 0.0;
		dSucw = 0.0;
		dSp = 0.0;
		dSuc = 0.0;
		dRatio = 0.0;
	}
};

struct T_SISR_EVAL_ROAD3_FORC
{	
	CArray<double, double> adShear;  /// 시간이력하중조건에 대한 전단력.
	CArray<double, double> adShearh;
	CArray<double, double> adMoment; /// 시간이력하중조건에 대한 모멘트.
	CArray<double, double> adMomenth;
	double dShear;
	double dMoment;

	T_SISR_EVAL_ROAD3_FORC() { Initialize(); }
	T_SISR_EVAL_ROAD3_FORC(const T_SISR_EVAL_ROAD3_FORC& src) { *this = src; }
	T_SISR_EVAL_ROAD3_FORC& operator= (const T_SISR_EVAL_ROAD3_FORC& src)
	{
		if (this != &src) {
			adShear.Copy(src.adShear);
			adShearh.Copy(src.adShearh);
			adMoment.Copy(src.adMoment);
			adMomenth.Copy(src.adMomenth);
			dShear = src.dShear;
			dMoment = src.dMoment;
		}
		return *this;
	}

	void Initialize()
	{		
		adShear.RemoveAll();
		adShearh.RemoveAll();
		adMoment.RemoveAll();
		adMomenth.RemoveAll();
		dShear = 0.0;
		dMoment = 0.0;
	}
};

struct T_SISR_EVAL_ROAD3_FORC_RW
{
	int nCountShear;
	int nCountShearh;
	int nCountMoment;
	int nCountMomenth;
	double dShear;
	double dMoment;

	T_SISR_EVAL_ROAD3_FORC_RW() { Initialize(); }
	T_SISR_EVAL_ROAD3_FORC_RW(const T_SISR_EVAL_ROAD3_FORC_RW& src) { *this = src; }
	T_SISR_EVAL_ROAD3_FORC_RW& operator= (const T_SISR_EVAL_ROAD3_FORC_RW& src)
	{
		if (this != &src) {
			nCountShear = src.nCountShear;
			nCountShearh = src.nCountShearh;
			nCountMoment = src.nCountMoment;
			nCountMomenth = src.nCountMomenth;
			dShear = src.dShear;
			dMoment = src.dMoment;
		}
		return *this;
	}

	void Get(T_SISR_EVAL_ROAD3_FORC& rTar)
	{		
		rTar.dShear = dShear;
		rTar.dMoment = dMoment;
	}

	void Set(const T_SISR_EVAL_ROAD3_FORC& src)
	{
		nCountShear  = src.adShear.GetSize();
		nCountShearh = src.adShearh.GetSize();
		nCountMoment = src.adMoment.GetSize();
		nCountMomenth = src.adMomenth.GetSize();
		dShear = src.dShear;
		dMoment = src.dMoment;
	}

	void Initialize()
	{
		nCountShear = 0;
		nCountShearh = 0;
		nCountMoment = 0;
		nCountMomenth = 0;
		dShear = 0.0;
		dMoment = 0.0;
	}
};

struct T_SISR_EVAL_ROAD3_BASE
{
	bool bAutoCapa;
	T_SISR_EVAL_ROAD3_CAPA_DIAG CapaDiag;
	T_SISR_EVAL_ROAD3_CAPA_COMP CapaComp;
	T_SISR_EVAL_ROAD3_FORC Forc;

	T_SISR_EVAL_ROAD3_BASE() { Initialize(); }
	T_SISR_EVAL_ROAD3_BASE(const T_SISR_EVAL_ROAD3_BASE& src) { *this = src; }
	T_SISR_EVAL_ROAD3_BASE& operator = (const T_SISR_EVAL_ROAD3_BASE& src)
	{
		if (this != &src)
		{
			bAutoCapa = src.bAutoCapa;
			CapaDiag = src.CapaDiag;
			CapaComp = src.CapaComp;
			Forc = src.Forc;
		}
		return *this;
	}
	void Initialize()
	{
		bAutoCapa = true;
		CapaDiag.Initialize();
		CapaComp.Initialize();
		Forc.Initialize();
	}
};

struct T_SISR_EVAL_ROAD3_BASE_RW
{
	bool bAutoCapa;
	T_SISR_EVAL_ROAD3_CAPA_DIAG CapaDiag;
	T_SISR_EVAL_ROAD3_CAPA_COMP CapaComp;
	T_SISR_EVAL_ROAD3_FORC_RW Forc;

	T_SISR_EVAL_ROAD3_BASE_RW() { Initialize(); }
	T_SISR_EVAL_ROAD3_BASE_RW(const T_SISR_EVAL_ROAD3_BASE_RW& src) { *this = src; }
	T_SISR_EVAL_ROAD3_BASE_RW& operator = (const T_SISR_EVAL_ROAD3_BASE_RW& src)
	{
		if (this != &src)
		{
			bAutoCapa = src.bAutoCapa;
			CapaDiag = src.CapaDiag;
			CapaComp = src.CapaComp;
			Forc = src.Forc;
		}
		return *this;
	}

	void Get(T_SISR_EVAL_ROAD3_BASE& rTar)
	{
		rTar.bAutoCapa = bAutoCapa;
		rTar.CapaDiag = CapaDiag;
		rTar.CapaComp = CapaComp;
		Forc.Get(rTar.Forc);		
	}

	void Set(const T_SISR_EVAL_ROAD3_BASE& src)
	{
		bAutoCapa = src.bAutoCapa;
		CapaDiag = src.CapaDiag;
		CapaComp = src.CapaComp;
		Forc.Set(src.Forc);
	}

	void Initialize()
	{
		bAutoCapa = true;
		CapaDiag.Initialize();
		CapaComp.Initialize();
		Forc.Initialize();
	}
};

struct T_SISR_EVAL_ROAD3_POS
{
	T_NODE_K NodeK;
	T_SISR_EVAL_ROAD3_BASE RsltMax;
	T_SISR_EVAL_ROAD3_BASE RsltMin;

	T_SISR_EVAL_ROAD3_POS() { Initialize(); }
	T_SISR_EVAL_ROAD3_POS(const T_SISR_EVAL_ROAD3_POS& src) { *this = src; }
	T_SISR_EVAL_ROAD3_POS& operator= (const T_SISR_EVAL_ROAD3_POS& src)
	{
		if (this != &src)
		{
			NodeK = src.NodeK;
			RsltMax = src.RsltMax;
			RsltMin = src.RsltMin;
		}
		return *this;
	}
	void Initialize()
	{
		NodeK = 0;
		RsltMax.Initialize();
		RsltMin.Initialize();
	}
};

struct T_SISR_EVAL_ROAD3_POS_RW
{
	T_NODE_K NodeK;
	T_SISR_EVAL_ROAD3_BASE_RW RsltMax;
	T_SISR_EVAL_ROAD3_BASE_RW RsltMin;

	T_SISR_EVAL_ROAD3_POS_RW() { Initialize(); }
	T_SISR_EVAL_ROAD3_POS_RW(const T_SISR_EVAL_ROAD3_POS_RW& src) { *this = src; }
	T_SISR_EVAL_ROAD3_POS_RW& operator= (const T_SISR_EVAL_ROAD3_POS_RW& src)
	{
		if (this != &src)
		{
			NodeK = src.NodeK;
			RsltMax = src.RsltMax;
			RsltMin = src.RsltMin;
		}
		return *this;
	}
	void Get(T_SISR_EVAL_ROAD3_POS& rTar)
	{
		rTar.NodeK = NodeK;
		RsltMax.Get(rTar.RsltMax);
		RsltMin.Get(rTar.RsltMin);
	}
	void Set(const T_SISR_EVAL_ROAD3_POS& src)
	{
		NodeK = src.NodeK;
		RsltMax.Set(src.RsltMax);
		RsltMin.Set(src.RsltMin);
	}
	void Initialize()
	{
		NodeK = 0;
		RsltMax.Initialize();
		RsltMin.Initialize();
	}	
};

struct T_SISR_EVAL_ROAD3
{
	UINT kSipa;
	UINT kSigr;
	UINT kElem;
	int nExamShearDir;			//검토된 기준 전단력 방향 : enum seis_jp::eDirShear
	T_SISR_EVAL_ROAD3_POS PosI;
	T_SISR_EVAL_ROAD3_POS PosJ;

	T_SISR_EVAL_ROAD3() { Initialize(); }
	T_SISR_EVAL_ROAD3(const T_SISR_EVAL_ROAD3& src) { *this = src; }
	T_SISR_EVAL_ROAD3& operator= (const T_SISR_EVAL_ROAD3& src)
	{
		if (this != &src)
		{
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			kElem = src.kElem;
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
		nExamShearDir = 0;
		PosI.Initialize();
		PosJ.Initialize();
	}
};

struct T_SISR_EVAL_ROAD3_RW
{
	UINT kSipa;
	UINT kSigr;
	UINT kElem;
	int nExamShearDir;
	T_SISR_EVAL_ROAD3_POS_RW PosI;
	T_SISR_EVAL_ROAD3_POS_RW PosJ;

	T_SISR_EVAL_ROAD3_RW() { Initialize(); }
	T_SISR_EVAL_ROAD3_RW(const T_SISR_EVAL_ROAD3_RW& src) { *this = src; }
	T_SISR_EVAL_ROAD3_RW& operator= (const T_SISR_EVAL_ROAD3_RW& src)
	{
		if (this != &src)
		{
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			kElem = src.kElem;
			nExamShearDir = src.nExamShearDir;
			PosI = src.PosI;
			PosJ = src.PosJ;
		}
		return *this;
	}
	void Get(T_SISR_EVAL_ROAD3& rTar)
	{
		rTar.kSipa = kSipa;
		rTar.kSigr = kSigr;
		rTar.kElem = kElem;
		rTar.nExamShearDir = nExamShearDir;
		PosI.Get(rTar.PosI);
		PosJ.Get(rTar.PosJ);	
	}
	void Set(const T_SISR_EVAL_ROAD3& src)
	{
		kSipa = src.kSipa;
		kSigr = src.kSigr;
		kElem = src.kElem;
		nExamShearDir = src.nExamShearDir;
		PosI.Set(src.PosI);
		PosJ.Set(src.PosJ);
	}
	void Initialize()
	{
		kSipa = 0;
		kSigr = 0;
		kElem = 0;
		nExamShearDir = 0;
		PosI.Initialize();
		PosJ.Initialize();
	}
};

#endif // !defined(__DB_ST_DT_SISR_EVAL_DIAG__)
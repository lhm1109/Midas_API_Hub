#ifndef __DB_ST_DT_SIMD_EVAL__
#define __DB_ST_DT_SIMD_EVAL__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

struct T_SIMD_EVAL_DISP
{
	UINT kThis;
	double dRatio;
	int nDirAnal;		//해석방향 enum seis_jp::eDirAnal
	doubleArray aDeltaA;
	doubleArray aDeltaMax;

	T_SIMD_EVAL_DISP() { Initialize(); }
	T_SIMD_EVAL_DISP(const T_SIMD_EVAL_DISP& src) { *this = src; }
	T_SIMD_EVAL_DISP& operator= (const T_SIMD_EVAL_DISP& src)
	{
		if (this != &src) {
			kThis = src.kThis;
			dRatio = src.dRatio;
			nDirAnal = src.nDirAnal;
			aDeltaA.Copy(src.aDeltaA);
			aDeltaMax.Copy(src.aDeltaMax);
		}
		return *this;
	}

	void Initialize()
	{
		kThis = 0;
		dRatio = 0;
		nDirAnal = 0;
		aDeltaA.RemoveAll();
		aDeltaMax.RemoveAll();
	}

	T_SIMD_EVAL_DISP& operator += (const T_SIMD_EVAL_DISP& src)
	{
		if (this != &src) {
			dRatio += src.dRatio;
			for (int i = 0; i < aDeltaA.GetCount(); i++) { aDeltaA[i] += abs(src.aDeltaA[i]); }
			for (int i = 0; i < aDeltaMax.GetCount(); i++) { aDeltaMax[i] += abs(src.aDeltaMax[i]); }
		}
		return *this;
	}

	T_SIMD_EVAL_DISP& operator /= (const double& den)
	{
		dRatio = dgn::SafeDiv(dRatio, den);
		for (int i = 0; i < aDeltaA.GetCount(); i++) { aDeltaA[i] = dgn::SafeDiv(aDeltaA[i], den); }
		for (int i = 0; i < aDeltaMax.GetCount(); i++) { aDeltaMax[i] = dgn::SafeDiv(aDeltaMax[i], den); }

		return *this;
	}
};

struct T_SIMD_EVAL_DISP_RW
{
	UINT kThis;
	double dRatio;
	int nDirAnal;
	int nSizeDeltaA;
	int nSizeDeltaMax;

	T_SIMD_EVAL_DISP_RW() { Initialize(); }
	T_SIMD_EVAL_DISP_RW(const T_SIMD_EVAL_DISP_RW& src) { *this = src; }
	T_SIMD_EVAL_DISP_RW& operator= (const T_SIMD_EVAL_DISP_RW& src)
	{
		if (this != &src) {
			kThis = src.kThis;
			dRatio = src.dRatio;
			nDirAnal = src.nDirAnal;
			nSizeDeltaA = src.nSizeDeltaA;
			nSizeDeltaMax = src.nSizeDeltaMax;
		}
		return *this;
	}
	void Initialize()
	{
		kThis = 0;
		dRatio = 0;
		nDirAnal = 0;
		nSizeDeltaA = 0;
		nSizeDeltaMax = 0;
	}

	void Get(T_SIMD_EVAL_DISP& rData)
	{
		rData.kThis = kThis;
		rData.dRatio = dRatio;
		rData.nDirAnal = nDirAnal;
		rData.aDeltaA.SetSize(nSizeDeltaA);
		rData.aDeltaMax.SetSize(nSizeDeltaMax);
	}
	void Set(const T_SIMD_EVAL_DISP& data)
	{
		kThis = data.kThis;
		dRatio = data.dRatio;
		nDirAnal = data.nDirAnal;
		nSizeDeltaA = data.aDeltaA.GetCount();
		nSizeDeltaMax = data.aDeltaMax.GetCount();
	}
};

//최대응답변위 조사결과 - 상부구조(H14/24 공통)
struct T_SIMD_EVAL
{
	UINT kSimd;
	UINT kSipa;
	UINT kSigr;
	int nDesignCode;			//내진조사 기준 : enum BrdgSE_CodeJP		
	int nAlwCalcType;			//허용변위 계산 방식 : enum seis_jp::eAlwCalcType
	int nRspCalcType;			//응답변위 계산 방식 : enum seis_jp::eRspCalcType
	int nSeismicForceType;		//지진력 타입 : enum T_SIPA_D::eSeismicForceType
	int nPosType;				//위치 : enum T_SIMD_EVAL::ePosType
	T_NODE_K kGirderNode;		//상부구조 좌,우단 절점
	T_NODE_K kPierTopNode;		//하부구조 관성력 작용위치
	T_SIGR_LOWER_K kSigrLower;	//하부그룹
	CString strColumnName;
	BOOL bCheckResult;
	T_SIMD_EVAL_DISP Averge;
	CArray<T_SIMD_EVAL_DISP, T_SIMD_EVAL_DISP> aDisplacement;

	T_SIMD_EVAL() { Initialize(); }
	T_SIMD_EVAL(const T_SIMD_EVAL& src) { *this = src; }
	T_SIMD_EVAL& operator= (const T_SIMD_EVAL& src)
	{
		if (this != &src) {
			kSimd = src.kSimd;
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			nDesignCode = src.nDesignCode;
			nAlwCalcType = src.nAlwCalcType;
			nRspCalcType = src.nRspCalcType;
			nSeismicForceType = src.nSeismicForceType;
			nPosType = src.nPosType;
			kGirderNode = src.kGirderNode;
			kPierTopNode = src.kPierTopNode;
			kSigrLower = src.kSigrLower;
			strColumnName = src.strColumnName;
			bCheckResult = src.bCheckResult;
			Averge = src.Averge;
			aDisplacement.Copy(src.aDisplacement);
		}
		return *this;
	}

	void Initialize()
	{
		kSimd = 0;
		kSipa = 0;
		kSigr = 0;
		nDesignCode = 0;
		nAlwCalcType = 0;
		nRspCalcType = 0;
		nSeismicForceType = 0;
		nPosType = 0;
		kGirderNode = 0;
		kPierTopNode = 0;
		kSigrLower = 0;
		strColumnName = _T("");
		bCheckResult = FALSE;
		Averge.Initialize();
		aDisplacement.RemoveAll();
	}

	enum ePosType
	{
		kUnKnown,
		kLeft,
		kRight,
		kLower,
	};

	double GetDeltaMax(const doubleArray& aDeltaMax) 
	{
		double dValue = 0;
		switch (this->nRspCalcType)
		{
		case seis_jp::kNEXCO:
			ASSERT(aDeltaMax.GetCount() == seis_jp::kSizeNexco);
			if (aDeltaMax.GetCount() > seis_jp::kDmaxNexco) {
				dValue = aDeltaMax[seis_jp::kDmaxNexco];
			}
			break;

		case seis_jp::kJBEC:
			ASSERT(aDeltaMax.GetCount() == seis_jp::kSizeJbec);
			if (aDeltaMax.GetCount() > seis_jp::kDmaxJbec) {
				dValue = aDeltaMax[seis_jp::kDmaxJbec];
			}
			break;
		}
		return dValue;
	};

	double GetDeltaA(const doubleArray& aDeltaA)
	{
		auto L_GetDeltaA_H14 = [&](OUT double& rValue) {
			switch (this->nAlwCalcType)
			{
			case seis_jp::kAnalysisResult:
				ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_AnalH14);
				if (aDeltaA.GetCount() > seis_jp::kDa_AnalH14) {
					rValue = aDeltaA[seis_jp::kDa_AnalH14];
				}
				break;
			case seis_jp::kMphi_Dy:
			case seis_jp::kMphi_Dpy:
				ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_MPhiH14);
				if (aDeltaA.GetCount() > seis_jp::kDa_MPhiH14) {
					rValue = aDeltaA[seis_jp::kDa_MPhiH14];
				}
				break;
			}
		};

		auto L_GetDeltaA_H24 = [&](OUT double& rValue) {
			switch (this->nAlwCalcType)
			{
			case seis_jp::kAnalysisResult:
				ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_AnalH24);
				if (aDeltaA.GetCount() > seis_jp::kDa_AnalH24) {
					rValue = aDeltaA[seis_jp::kDa_AnalH24];
				}
				break;
			case seis_jp::kMphi_Dy:
			case seis_jp::kMphi_Dpy:
				ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_MPhiH24);
				if (aDeltaA.GetCount() > seis_jp::kDa_MPhiH24) {
					rValue = aDeltaA[seis_jp::kDa_MPhiH24];
				}
				break;
			}
		};

		auto L_GetDeltaA_H29 = [&](OUT double& rValue) {
			switch (this->nAlwCalcType)
			{
			case seis_jp::kAnalysisResult:
				ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_AnalH29);
				if (aDeltaA.GetCount() > seis_jp::kDlsd_AnalH29) {
					rValue = aDeltaA[seis_jp::kDlsd_AnalH29];
				}
				break;
			case seis_jp::kMphi_Dy:
			case seis_jp::kMphi_Dpy:
				ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_MPhiH29);
				if (aDeltaA.GetCount() > seis_jp::kDlsd_MPhiH29) {
					rValue = aDeltaA[seis_jp::kDlsd_MPhiH29];
				}
				break;
			}
		};

		double dValue = 0;
		switch (this->nDesignCode)
		{
		case BrdgSE_CodeJP::JROAD5_H14:
			L_GetDeltaA_H14(dValue);
			break;
		case BrdgSE_CodeJP::JROAD5_H24:
			L_GetDeltaA_H24(dValue);
			break;
		case BrdgSE_CodeJP::JROAD5_H29:
			L_GetDeltaA_H29(dValue);
			break;
		}
		return dValue;
	};
};

struct T_SIMD_EVAL_CH
{
	UINT kSimd;
	UINT kSipa;
	UINT kSigr;
	int nDesignCode;
	int nAlwCalcType;
	int nRspCalcType;
	int nSeismicForceType;
	int nPosType;
	T_NODE_K kGirderNode;
	T_NODE_K kPierTopNode;
	T_SIGR_LOWER_K kSigrLower;
	char strColumnName[60];
	BOOL bCheckResult;
	T_SIMD_EVAL_DISP Averge;
	CArray<T_SIMD_EVAL_DISP, T_SIMD_EVAL_DISP> aDisplacement;

	void Get(T_SIMD_EVAL& rData)
	{
		rData.kSimd = kSimd;
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		rData.nDesignCode = nDesignCode;
		rData.nAlwCalcType = nAlwCalcType;
		rData.nRspCalcType = nRspCalcType;
		rData.nSeismicForceType = nSeismicForceType;
		rData.nPosType = nPosType;
		rData.kGirderNode = kGirderNode;
		rData.kPierTopNode = kPierTopNode;
		rData.kSigrLower = kSigrLower;
		ConvertCharStr(strColumnName, rData.strColumnName, sizeof(strColumnName));
		rData.bCheckResult = bCheckResult;
		rData.Averge = Averge;
		rData.aDisplacement.Copy(aDisplacement);
	}

	void Set(/*const*/T_SIMD_EVAL& data)
	{
		kSimd = data.kSimd;
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		nDesignCode = data.nDesignCode;
		nAlwCalcType = data.nAlwCalcType;
		nRspCalcType = data.nRspCalcType;
		nSeismicForceType = data.nSeismicForceType;
		nPosType = data.nPosType;
		kGirderNode = data.kGirderNode;
		kPierTopNode = data.kPierTopNode;
		kSigrLower = data.kSigrLower;
		ConvertStrChar(data.strColumnName, strColumnName, sizeof(strColumnName));
		bCheckResult = data.bCheckResult;
		Averge = data.Averge;
		aDisplacement.Copy(data.aDisplacement);
	}
};

struct T_SIMD_EVAL_RW
{
	UINT kSimd;
	UINT kSipa;
	UINT kSigr;
	int nDesignCode;
	int nAlwCalcType;
	int nRspCalcType;
	int nSeismicForceType;
	int nPosType;
	T_NODE_K kGirderNode;
	T_NODE_K kPierTopNode;
	T_SIGR_LOWER_K kSigrLower;
	char strColumnName[60];
	BOOL bCheckResult;
	T_SIMD_EVAL_DISP_RW Averge;
	int nDisplacement;

	T_SIMD_EVAL_RW() { Initialize(); }
	T_SIMD_EVAL_RW(const T_SIMD_EVAL_RW& src) { *this = src; }
	T_SIMD_EVAL_RW& operator= (const T_SIMD_EVAL_RW& src)
	{
		if (this != &src) {
			kSimd = src.kSimd;
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			nDesignCode = src.nDesignCode;
			nAlwCalcType = src.nAlwCalcType;
			nRspCalcType = src.nRspCalcType;
			nSeismicForceType = src.nSeismicForceType;
			nPosType = src.nPosType;
			kGirderNode = src.kGirderNode;
			kPierTopNode = src.kPierTopNode;
			kSigrLower = src.kSigrLower;
			memcpy(strColumnName, src.strColumnName, sizeof(strColumnName));
			bCheckResult = src.bCheckResult;
			Averge = src.Averge;
			nDisplacement = src.nDisplacement;
		}
		return *this;
	}

	void Get(T_SIMD_EVAL_CH& rData)
	{
		rData.kSimd = kSimd;
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		rData.nDesignCode = nDesignCode;
		rData.nAlwCalcType = nAlwCalcType;
		rData.nRspCalcType = nRspCalcType;
		rData.nSeismicForceType = nSeismicForceType;
		rData.nPosType = nPosType;
		rData.kGirderNode = kGirderNode;
		rData.kPierTopNode = kPierTopNode;
		rData.kSigrLower = kSigrLower;
		memcpy(rData.strColumnName, strColumnName, sizeof(strColumnName));
		rData.bCheckResult = bCheckResult;
		Averge.Get(rData.Averge);
	}
	void Set(const T_SIMD_EVAL_CH& data)
	{
		kSimd = data.kSimd;
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		nDesignCode = data.nDesignCode;
		nAlwCalcType = data.nAlwCalcType;
		nRspCalcType = data.nRspCalcType;
		nSeismicForceType = data.nSeismicForceType;
		nPosType = data.nPosType;
		kGirderNode = data.kGirderNode;
		kPierTopNode = data.kPierTopNode;
		kSigrLower = data.kSigrLower;
		memcpy(strColumnName, data.strColumnName, sizeof(strColumnName));
		bCheckResult = data.bCheckResult;
		Averge.Set(data.Averge);
		nDisplacement = data.aDisplacement.GetCount();
	}

	void Initialize()
	{
		kSimd = 0;
		kSipa = 0;
		kSigr = 0;
		nDesignCode = 0;
		nAlwCalcType = 0;
		nRspCalcType = 0;
		nSeismicForceType = 0;
		nPosType = 0;
		kGirderNode = 0;
		kPierTopNode = 0;
		kSigrLower = 0;
		Averge.Initialize();
		bCheckResult = FALSE;
		nDisplacement = 0;
	}
};

#endif // !defined(__DB_ST_DT_SIMD_EVAL__)
#pragma once
#include "SPFCBaseUtil.h"

class CSpfcCHCJJ2011Util : public CSpfcBaseUtil
{
public:
	CSpfcCHCJJ2011Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;

	void SetOutputData();
	double CalcTgFromInputData() const;
	double CalcEta1FromInputData() const;
	double CalcEta2FromInputData() const;
	double CalcGammaFromInputData() const;
	double CalcCiFromInputData() const;
	double CalcAFromOutputData(const double dCi) const;
	double CalcSMaxFromOutputData(const double dA) const;

public:
	int GetSeisType() const { return m_nSeisType; }
	int GetZoningMap() const { return m_nZoningMap; }
	int GetSiteType() const { return m_nSiteType; }
	int GetSeismicIntensity() const { return m_nSeismicIntensity; }
	int GetSeismicE() const { return m_nSeismicE; }

	double GetDampingRatio() const { return m_dDampingRatio; }
	BOOL GetE2_005() const { return m_bE2_005; }
	BOOL GetVertAccSpec() const { return m_bVertAccSpec; }

	double GetTg() const { return m_dTg; }
	double GetEta1() const { return m_dEta1; }
	double GetEta2() const { return m_dEta2; }
	double GetGamma() const { return m_dGamma; }
	double GetCi() const { return m_dCi; }
	double GetA() const { return m_dA; }
	double GetSMax() const { return m_dSMax; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }

	void SetSeisType(int nSeisType) { m_nSeisType = nSeisType; }
	void SetZoningMap(int nZoningMap) { m_nZoningMap = nZoningMap; }
	void SetSiteType(int nSiteType) { m_nSiteType = nSiteType; }
	void SetSeismicIntensity(int nSeismicIntensity) { m_nSeismicIntensity = nSeismicIntensity; }
	void SetSeismicE(int nSeismicE) { m_nSeismicE = nSeismicE; }

	void SetDampingRatio(double dDampingRatio) { m_dDampingRatio = dDampingRatio; }
	void SetE2_005(BOOL bE2_005) { m_bE2_005 = bE2_005; }
	void SetVertAccSpec(BOOL bVertAccSpec) { m_bVertAccSpec = bVertAccSpec; }

	void SetTg(double dTg) { m_dTg = dTg; }
	void SetEta1(double dEta1) { m_dEta1 = dEta1; }
	void SetEta2(double dEta2) { m_dEta2 = dEta2; }
	void SetGamma(double dGamma) { m_dGamma = dGamma; }
	void SetCi(double dCi) { m_dCi = dCi; }
	void SetA(double dA) { m_dA = dA; }
	void SetSMax(double dSMax) { m_dSMax = dSMax; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }

private:
	int m_nSeisType;
	int m_nZoningMap;
	int m_nSiteType;
	int m_nSeismicIntensity;
	int m_nSeismicE;

	double m_dDampingRatio;
	BOOL m_bE2_005;
	BOOL m_bVertAccSpec;

	double m_dTg;
	double m_dEta1;
	double m_dEta2;
	double m_dGamma;
	double m_dCi;
	double m_dA;
	double m_dSMax;
	double m_dMaxPeriod;
};

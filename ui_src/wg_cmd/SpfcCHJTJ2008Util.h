#pragma once
#include "SPFCBaseUtil.h"

class CSpfcCHJTJ2008Util : public CSpfcBaseUtil
{
public:
	CSpfcCHJTJ2008Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;

	void SetOutputData();
	double CalcTgFromInputData() const;
	double CalcCiFromInputData() const;
	double CalcCsFromInputData() const;
	double CalcCdFromInputData() const;
	double CalcEPAFromInputData() const;
	double CalcSMaxFromOutputData(const double dCi, const double dCs, const double dCd, const double dEPA) const;

public:
	int GetBridgeType() const { return m_nBridgeType; }
	int GetZoningMap() const { return m_nZoningMap; }
	int GetSiteType() const { return m_nSiteType; }
	int GetSeismicIntensity() const { return m_nSeismicIntensity; }
	int GetSeismicCriterion() const { return m_nSeismicCriterion; }

	double GetDampingRatio() const { return m_dDampingRatio; }

	BOOL GetLargeBridge() const { return m_bLargeBridge; }
	BOOL GetVertAccSpec() const { return m_bVertAccSpec; }
	int GetVertAccSpecType() const { return m_nVertAccSpecType; }

	double GetTg() const { return m_dTg; }
	double GetCi() const { return m_dCi; }
	double GetCs() const { return m_dCs; }
	double GetCd() const { return m_dCd; }
	double GetEPA() const { return m_dEPA; }
	double GetSMax() const { return m_dSMax; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }

	void SetBridgeType(int nBridgeType) { m_nBridgeType = nBridgeType; }
	void SetZoningMap(int nZoningMap) { m_nZoningMap = nZoningMap; }
	void SetSiteType(int nSiteType) { m_nSiteType = nSiteType; }
	void SetSeismicIntensity(int nSeismicIntensity) { m_nSeismicIntensity = nSeismicIntensity; }
	void SetSeismicCriterion(int nSeismicCriterion) { m_nSeismicCriterion = nSeismicCriterion; }

	void SetDampingRatio(double dDampingRatio) { m_dDampingRatio = dDampingRatio; }

	void SetLargeBridge(BOOL bLargeBridge) { m_bLargeBridge = bLargeBridge; }
	void SetVertAccSpec(BOOL bVertAccSpec) { m_bVertAccSpec = bVertAccSpec; }
	void SetVertAccSpecType(int nVertAccSpecType) { m_nVertAccSpecType = nVertAccSpecType; }

	void SetTg(double dTg) { m_dTg = dTg; }
	void SetCi(double dCi) { m_dCi = dCi; }
	void SetCs(double dCs) { m_dCs = dCs; }
	void SetCd(double dCd) { m_dCd = dCd; }
	void SetEPA(double dEPA) { m_dEPA = dEPA; }
	void SetSMax(double dSMax) { m_dSMax = dSMax; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }

private:
	// Input Data
	int m_nBridgeType;
	int m_nZoningMap;
	int m_nSiteType;
	int m_nSeismicIntensity;
	int m_nSeismicCriterion;

	double m_dDampingRatio;

	BOOL m_bLargeBridge;
	BOOL m_bVertAccSpec;
	int  m_nVertAccSpecType;

	//Output Data
	double m_dTg;
	double m_dCi;
	double m_dCs;
	double m_dCd;
	double m_dEPA;
	double m_dSMax;
	double m_dMaxPeriod;
};

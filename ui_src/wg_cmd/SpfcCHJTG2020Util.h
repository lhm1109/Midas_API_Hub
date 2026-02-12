#pragma once
#include "SPFCBaseUtil.h"

class CSpfcCHJTG2020Util : public CSpfcBaseUtil
{
public:
	CSpfcCHJTG2020Util();

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
	double CalcSMaxFromOutputData(const double dCi, const double dCs, const double dCd, const double dA) const;

public:
	int GetBridgeType() const { return m_nBridgeType; }
	int GetZoningMap() const { return m_nZoningMap; }
	int GetSiteType() const { return m_nSiteType; }
	int GetSeismicIntensity() const { return m_nSeismicIntensity; }
	int GetSeismicCriterion() const { return m_nSeismicCriterion; }

	double GetDampingRatio() const { return m_dDampingRatio; }
	BOOL GetLargeBridge() const { return m_bLargeBridge; }
	BOOL GetVertResSpec() const { return m_bVertResSpec; }

	double GetTg() const { return m_dTg; }
	double GetCi() const { return m_dCi; }
	double GetCs() const { return m_dCs; }
	double GetCd() const { return m_dCd; }
	double GetA() const { return m_dA; }
	double GetSMax() const { return m_dSMax; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }

	void SetBridgeType(int nBridgeType) { m_nBridgeType = nBridgeType; }
	void SetZoningMap(int nZoningMap) { m_nZoningMap = nZoningMap; }
	void SetSiteType(int nSiteType) { m_nSiteType = nSiteType; }
	void SetSeismicIntensity(int nSeismicIntensity) { m_nSeismicIntensity = nSeismicIntensity; }
	void SetSeismicCriterion(int nSeismicCriterion) { m_nSeismicCriterion = nSeismicCriterion; }

	void SetDampingRatio(double dDampingRatio) { m_dDampingRatio = dDampingRatio; }
	void SetLargeBridge(BOOL bLargeBridge) { m_bLargeBridge = bLargeBridge; }
	void SetVertResSpec(BOOL bVertResSpec) { m_bVertResSpec = bVertResSpec; }

	void SetTg(double dTg) { m_dTg = dTg; }
	void SetCi(double dCi) { m_dCi = dCi; }
	void SetCs(double dCs) { m_dCs = dCs; }
	void SetCd(double dCd) { m_dCd = dCd; }
	void SetA(double dA) { m_dA = dA; }
	void SetSMax(double dSMax) { m_dSMax = dSMax; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }

private:
	int m_nBridgeType;
	int m_nZoningMap;
	int m_nSiteType;
	int m_nSeismicIntensity;
	int m_nSeismicCriterion;

	double m_dDampingRatio;
	BOOL m_bLargeBridge;
	BOOL m_bVertResSpec;
	 
	double m_dTg;
	double m_dCi;
	double m_dCs;
	double m_dCd;
	double m_dA;
	double m_dSMax;
	double m_dMaxPeriod;
};

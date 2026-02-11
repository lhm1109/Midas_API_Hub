#pragma once
#include "SpfcBaseUtil.h"

class CSpfcIS2025Util : public CSpfcBaseUtil
{
public:
	CSpfcIS2025Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

private:
	double GetDeltaZeta(const double dTn);
	double GetDeltaVert(const double dTn);

public:
	int GetSpecType() const { return m_nSpecType; }
	int GetEqZone() const { return m_nEqZone; }
	int GetRetPeriod() const { return m_nRetPeriod; }
	int GetSiteClass() const { return m_nSiteClass; }
	double GetDamping() const { return m_dDamping; }
	double GetImpFact() const { return m_dImpFact; }
	double GetRespReduFact() const { return m_dRespReduFact; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }
	double GetZoneFact() const { return m_dZoneFactor; }

	void SetSpecType(int nSpecType) { m_nSpecType = nSpecType; }
	void SetEqZone(int nEqZone) { m_nEqZone = nEqZone; }
	void SetRetPeriod(int nRetPeriod) { m_nRetPeriod = nRetPeriod; }
	void SetSiteClass(int nSiteClass) { m_nSiteClass = nSiteClass; }
	void SetDamping(double dDamping) { m_dDamping = dDamping; }
	void SetImpFact(double dImpFact) { m_dImpFact = dImpFact; }
	void SetRespReduFact(double dRespReduFact) { m_dRespReduFact = dRespReduFact; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }
	void SetZoneFact(double dZoneFact) { m_dZoneFactor = dZoneFact; }

private:
	int m_nSpecType;
	int m_nEqZone;
	int m_nRetPeriod;
	double m_dZoneFactor;
	double m_dImpFact;
	double m_dDamping;
	double m_dRespReduFact;
	int m_nSiteClass;
	double m_dMaxPeriod;

public:
	double m_adZoneFacts[EN_IS2025_EQ_ZONE_COUNT][EN_IS2025_RET_COUNT] = {
		{ 0.0375, 0.0500, 0.0600, 0.0750, 0.1000, 0.1125, 0.1500, 0.2000, 0.2700 }, // Zone II
		{ 0.0625, 0.0850, 0.1000, 0.1250, 0.1670, 0.1875, 0.2500, 0.3330, 0.4500 }, // Zone III
		{ 0.1400, 0.1750, 0.2100, 0.2330, 0.2800, 0.2917, 0.3500, 0.4400, 0.5250 }, // Zone IV
		{ 0.2000, 0.2500, 0.3000, 0.3330, 0.4000, 0.4167, 0.5000, 0.6250, 0.7500 }, // Zone V
		{ 0.3000, 0.3750, 0.4500, 0.5000, 0.6000, 0.6250, 0.7500, 0.9400, 1.1250 }  // Zone VI
	};
};
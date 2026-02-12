#pragma once
#include "SpfcBaseUtil.h"

namespace TW99V
{
	constexpr double ZONE[] = { 0.33 * (2. / 3.), 0.23 * (1. / 3.) };
}

class CSpfcTAIWAN99VUtil : public CSpfcBaseUtil
{
public:
	CSpfcTAIWAN99VUtil();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

	double GetTaiwan86CFactor(int nSoil, double dDamping, double dT);
	double GetTaiwan86POCFactor(int nSoil, double dDamping, double dT, double dTg);
	double GetTaiwan86FuFactor(int nSoil, double dR, double dT);
	double GetTaiwan86SpectralAcc(int nSoil, double dZ, double dDamping, double dI, double dR, double dAlpha, double dT);
	double GetTaiwan86ModifiedTg(int nSoil, double dR, double SRa, double SRv);
	double GetTaiwan86POSpectralAcc(int nSoil, double dZ, double dI, double dR, double dAlpha, double dT, double dTg);

public:
	int GetZone() const { return m_nZone; }
	int GetSoil() const { return m_nSoil; }

	double GetI() const { return m_dI; }
	double GetAlpha() const { return m_dAlpha; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }
	double GetR() const { return m_dR; }
	double GetDamping() const { return m_dDamping; }
	double GetZ() const { return m_dZ; }

	void SetZone(int nZone) { m_nZone = nZone; }
	void SetSoil(int nSoil) { m_nSoil = nSoil; }

	void SetI(double dI) { m_dI = dI; }
	void SetAlpha(double dAlpha) { m_dAlpha = dAlpha; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }
	void SetR(double dR) { m_dR = dR; }
	void SetDamping(double dDamping) { m_dDamping = dDamping; }
	void SetZ(double dZ) { m_dZ = dZ; }

public:
	int m_nZone;
	int m_nSoil;

	double m_dI;
	double m_dAlpha;
	double m_dMaxPeriod;
	double m_dR;
	double m_dDamping;

	double m_dZ;
};

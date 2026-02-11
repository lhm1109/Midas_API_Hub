#pragma once
#include "SpfcBaseUtil.h"

namespace IS2002
{
	constexpr double ZONE[] = { 0.10, 0.16, 0.24, 0.36 };
}

class CSpfcIS2002Util : public CSpfcBaseUtil
{
public:
	CSpfcIS2002Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

	double GetDampingFactorIS2002(double dDamping);

public:
	int GetZone() const { return m_nZone; }
	int GetSoil() const { return m_nSoil; }
	double GetDamping() const { return m_dDamping; }
	double GetDampingFactor() const { return m_dDampingFactor; }
	double GetI() const { return m_dI; }
	double GetR() const { return m_dR; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }
	double GetZ() const { return m_dZ; }

	void SetZone(int nZone) { m_nZone = nZone; }
	void SetSoil(int nSoil) { m_nSoil = nSoil; }
	void SetDamping(double dDamping) { m_dDamping = dDamping; }
	void SetDampingFactor(double dDampingFactor) { m_dDampingFactor = dDampingFactor; }
	void SetI(double dI) { m_dI = dI; }
	void SetR(double dR) { m_dR = dR; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }
	void SetZ(double dZ) { m_dZ = dZ; }

private:
	int m_nZone;
	int m_nSoil;
	double m_dDamping;
	double m_dDampingFactor;
	double m_dI;
	double m_dR;
	double m_dMaxPeriod;

	double m_dZ;
};

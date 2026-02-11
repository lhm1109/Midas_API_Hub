#pragma once
#include "SpfcBaseUtil.h"

class CSpfcIRC2018Util : public CSpfcBaseUtil
{
public:
	CSpfcIRC2018Util();

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
	double GetUserDefSeisZoneFact() const { return m_dUserDefSeisZoneFact; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }
	double GetZ() const { return m_dZ; }
	double GetZoneValue(int idx) const { return arrZone[idx]; }

	void SetZone(int nZone) { m_nZone = nZone; }
	void SetSoil(int nSoil) { m_nSoil = nSoil; }
	void SetDamping(double dDamping) { m_dDamping = dDamping; }
	void SetDampingFactor(double dDampingFactor) { m_dDampingFactor = dDampingFactor; }
	void SetI(double dI) { m_dI = dI; }
	void SetR(double dR) { m_dR = dR; }
	void SetUserDefSeisZoneFact(double dUserDefSeisZoneFact)
	{
		m_dUserDefSeisZoneFact = dUserDefSeisZoneFact;
		SetZoneValueFromUser();
	}
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }
	void SetZ(double dZ) { m_dZ = dZ; }

private:
	void SetZoneValueFromUser() { arrZone.back() = m_dUserDefSeisZoneFact; }

private:
	int m_nZone;
	int m_nSoil;
	double m_dDamping;
	double m_dDampingFactor;
	double m_dI;
	double m_dR;
	double m_dUserDefSeisZoneFact;
	double m_dMaxPeriod;

	double m_dZ;
	std::array<double, 5> arrZone;
};

//int nSeisZone;         // Seismic Zone
//int nSoilType;         // Soil Class
//double dDamping;       // Damping(%)
//double dDampingFactor; // Damping Multiplying Factor
//double dIe;            // Importance Factor(I)
//double dCoef;          // Response reduction Factor(R)
//double dUsrDefSeismicZone; // User Defined Seismic Zone for nSeisZone = 4
//double dMaxPeriod;

#pragma once
#include "SPFCBaseUtil.h"

class CSpfcKSCE2019Util : public CSpfcBaseUtil
{
public:
	CSpfcKSCE2019Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

    double CalZonFactor(int nZone, double dI);

public:
    int GetZone() const { return m_nZone; }
    int GetSite() const { return m_nSite; }
    double GetI() const { return m_dI; }
    double GetMaxPeriod() const { return m_dMaxPeriod; }
    double GetFa() const { return m_dFa; }
    double GetFv() const { return m_dFv; }
    double GetZoneFactor() const { return m_dZoneFactor; }
    double GetT0() const { return m_dT0; }
    double GetTs() const { return m_dTs; }
    double GetTl() const { return m_dTl; }

    void SetZone(int nZone) { m_nZone = nZone; }
    void SetSite(int nSite) { m_nSite = nSite; }
    void SetI(double dI) { m_dI = dI; }
    void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }
    void SetFa(double dFa) { m_dFa = dFa; }
    void SetFv(double dFv) { m_dFv = dFv; }
    void SetZoneFactor(double dZoneFactor) { m_dZoneFactor = dZoneFactor; }
    void SetT0(double dT0) { m_dT0 = dT0; }
    void SetTs(double dTs) { m_dTs = dTs; }
    void SetTl(double dTl) { m_dTl = dTl; }

private:
	int    m_nZone;
	int    m_nSite;
	double m_dI;   // Importance Factor, Risk Factor
	double m_dMaxPeriod;
	double m_dFa;
	double m_dFv;

	double m_dZoneFactor;
	double m_dT0;
	double m_dTs;
	double m_dTl;
};

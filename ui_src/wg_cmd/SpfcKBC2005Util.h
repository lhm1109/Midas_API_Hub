#pragma once
#include "SPFCBaseUtil.h"

class CSpfcKBC2005Util : public CSpfcBaseUtil
{
public:
	CSpfcKBC2005Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

public:
	int GetAccMethod() const { return m_nAccMethod; }
	int GetZone() const { return m_nZone; }
	int GetSite() const { return m_nSite; }
	double GetSds() const { return m_dSds; }
	double GetSd1() const { return m_dSd1; }
	double GetR() const { return m_dR; }
	double GetI() const { return m_dI; }
	bool GetRelieve() const { return m_bRelieve; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }
	double GetT0() const { return m_dT0; }
	double GetTs() const { return m_dTs; }

	void SetAccMethod(int nAccMethod) { m_nAccMethod = nAccMethod; }
	void SetZone(int nZone) { m_nZone = nZone; }
	void SetSite(int nSite) { m_nSite = nSite; }
	void SetSds(double dSds) { m_dSds = dSds; }
	void SetSd1(double dSd1) { m_dSd1 = dSd1; }
	void SetR(double dR) { m_dR = dR; }
	void SetI(double dI) { m_dI = dI; }
	void SetRelieve(bool bRelieve) { m_bRelieve = bRelieve; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }
	void SetT0(double dT0) { m_dT0 = dT0; }
	void SetTs(double dTs) { m_dTs = dTs; }

private:
	int m_nAccMethod;
	int m_nZone;
	int m_nSite;
	double m_dSds;
	double m_dSd1;
	double m_dR;
	double m_dI;
	BOOL m_bRelieve;
	double m_dMaxPeriod;

	double m_dT0;
	double m_dTs;
};

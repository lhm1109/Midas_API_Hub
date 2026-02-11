#pragma once
#include "SPFCBaseUtil.h"

class CSpfcKBC2016Util : public CSpfcBaseUtil
{
public:
	CSpfcKBC2016Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

public:
	int GetZone() const { return m_nZone; }
	int GetSite() const { return m_nSite; }
	double GetZoneFactor() const { return m_dZoneFactor; }
	double GetDepthToMR() const { return m_dDepthToMR; }
	double GetFa() const { return m_dFa; }
	double GetFv() const { return m_dFv; }
	double GetSds() const { return m_dSds; }
	double GetSd1() const { return m_dSd1; }
	double GetI() const { return m_dI; }
	double GetR() const { return m_dR; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }
	double GetT0() const { return m_dT0; }
	double GetTs() const { return m_dTs; }

	void SetZone(int nZone) { m_nZone = nZone; }
	void SetSite(int nSite) { m_nSite = nSite; }
	void SetZoneFactor(double dZoneFactor) { m_dZoneFactor = dZoneFactor; }
	void SetDepthToMR(double dDepthToMR) { m_dDepthToMR = dDepthToMR; }
	void SetFa(double dFa) { m_dFa = dFa; }
	void SetFv(double dFv) { m_dFv = dFv; }
	void SetSds(double dSds) { m_dSds = dSds; }
	void SetSd1(double dSd1) { m_dSd1 = dSd1; }
	void SetI(double dI) { m_dI = dI; }
	void SetR(double dR) { m_dR = dR; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }
	void SetT0(double dT0) { m_dT0 = dT0; }
	void SetTs(double dTs) { m_dTs = dTs; }

private:
	int m_nZone;
	int m_nSite;
	double m_dZoneFactor;
	double m_dDepthToMR;
	double m_dFa;
	double m_dFv;
	double m_dSds;
	double m_dSd1;
	double m_dI;
	double m_dR;
	double m_dMaxPeriod;

	double m_dT0;
	double m_dTs;
};


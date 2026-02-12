#pragma once
#include "SpfcBaseUtil.h"

class CSpfcIBC2000Util : public CSpfcBaseUtil
{
public:
	CSpfcIBC2000Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

	void SetValue();

public:
	int GetWndSC() const { return m_wndSC; }
	int GetMaxPeriod() const { return m_dMaxPeriod; }
	double GetSds() const { return m_dSds; }
	double GetSd1() const { return m_dSd1; }
	double GetI() const { return m_dI; }
	double GetSs() const { return m_dSs; }
	double GetS1() const { return m_dS1; }
	double GetT0() const { return m_dT0; }
	double GetTs() const { return m_dTs; }
	double GetR() const { return m_dR; }

	void SetWndSC(int wndSC) { m_wndSC = wndSC; }
	void SetMaxPeriod(int maxPeriod) { m_dMaxPeriod = maxPeriod; }
	void SetSds(double sds) { m_dSds = sds; }
	void SetSd1(double sd1) { m_dSd1 = sd1; }
	void SetI(double i) { m_dI = i; }
	void SetSs(double ss) { m_dSs = ss; }
	void SetS1(double s1) { m_dS1 = s1; }
	void SetT0(double t0) { m_dT0 = t0; }
	void SetTs(double ts) { m_dTs = ts; }
	void SetR(double r) { m_dR = r; }

private:
	int m_wndSC;
	int m_dMaxPeriod;
	double m_dSds;
	double m_dSd1;

	double m_dI;
	double m_dSs;
	double m_dS1;
	double m_dT0;
	double m_dTs;
	double m_dR;
};

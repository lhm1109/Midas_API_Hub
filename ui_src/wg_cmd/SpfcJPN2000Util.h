#pragma once
#include "SpfcBaseUtil.h"

class CSpfcJPN2000Util : public CSpfcBaseUtil
{
public:
	CSpfcJPN2000Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

public:
	int GetWndZ() const { return m_wndZ; }
	int GetWndTc() const { return m_wndTc; }
	double GetC0() const { return m_dC0; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }
	double GetZ() const { return m_dZ; }
	double GetTc() const { return m_dTc; }
	double GetSlope() const { return m_dSlope; }

	void SetWndZ(int nWndZ) { m_wndZ = nWndZ; }
	void SetWndTc(int nWndTc) { m_wndTc = nWndTc; }
	void SetC0(double dC0) { m_dC0 = dC0; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }
	void SetZ(double dZ) { m_dZ = dZ; }
	void SetTc(double dTc) { m_dTc = dTc; }
	void SetSlope(double dSlope) { m_dSlope = dSlope; }

private:
	int	m_wndZ;
	int	m_wndTc;
	double m_dC0;
	double m_dMaxPeriod;

	double m_dZ;
	double m_dTc;
	double m_dSlope;
};

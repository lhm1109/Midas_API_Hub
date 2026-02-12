#pragma once
#include "SpfcBaseUtil.h"

class CSpfcUBC88Util : public CSpfcBaseUtil
{
public:
	CSpfcUBC88Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;

public:
	double GetSoil() const { return m_dSoil; }
	double GetSZF() const { return m_dSZF; }
	double GetOIF() const { return m_dOIF; }
	double GetRw() const { return m_dRw; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }

	void SetSoil(double dSoil) { m_dSoil = dSoil; }
	void SetSZF(double dSZF) { m_dSZF = dSZF; }
	void SetOIF(double dOIF) { m_dOIF = dOIF; }
	void SetRw(double dRw) { m_dRw = dRw; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }

private:
	double m_dSoil;
	double m_dSZF;
	double m_dOIF;
	double m_dRw;
	double m_dMaxPeriod;
};

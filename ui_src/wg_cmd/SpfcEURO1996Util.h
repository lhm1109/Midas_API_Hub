#pragma once
#include "SpfcBaseUtil.h"

class CSpfcEURO1996Util : public CSpfcBaseUtil
{
public:
	CSpfcEURO1996Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

public:
	int GetSc() const { return m_nSc; }
	int GetKd() const { return m_nKd; }
	int GetKr() const { return m_nKr; }
	double GetQ0() const { return m_dQ0; }
	double GetKw() const { return m_dKw; }
	double GetAlpha() const { return m_dAlpha; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }

	void SetSc(int nSc) { m_nSc = nSc; }
	void SetKd(int nKd) { m_nKd = nKd; }
	void SetKr(int nKr) { m_nKr = nKr; }
	void SetQ0(double dQ0) { m_dQ0 = dQ0; }
	void SetKw(double dKw) { m_dKw = dKw; }
	void SetAlpha(double dAlpha) { m_dAlpha = dAlpha; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }

private:
	int m_nSc;
	int m_nKd;
	int m_nKr;
	double m_dQ0;
	double m_dKw;
	double m_dAlpha;
	double m_dMaxPeriod;

	double m_dSc;
	double m_dKd;
	double m_dKr;
	double m_dBeta;
	double m_dKd1;
	double m_dKd2;
	double m_dTb;
	double m_dTc;
	double m_dTd;
	CString m_sSoil;
};

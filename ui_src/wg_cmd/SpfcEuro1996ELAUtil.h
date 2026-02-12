#pragma once
#include "SpfcBaseUtil.h"

class CSpfcEURO1996ELAUtil : public CSpfcBaseUtil
{
public:
	CSpfcEURO1996ELAUtil();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

public:
	int GetSc() const { return m_nSc; }
	double GetAg() const { return m_dAg; }
	double GetDamping() const { return m_dDamping; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }

	void SetSc(int nSc) { m_nSc = nSc; }
	void SetAg(double dAg) { m_dAg = dAg; }
	void SetDamping(double dDamping) { m_dDamping = dDamping; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }

private:
	int m_nSc;
	double m_dAg;
	double m_dDamping;
	double m_dMaxPeriod;

	double m_dSc;
	double m_dBeta;
	double m_dK1;
	double m_dK2;
	double m_dTb;
	double m_dTc;
	double m_dTd;
	CString m_sSoil;
};

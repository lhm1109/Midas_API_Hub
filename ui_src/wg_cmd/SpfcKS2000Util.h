#pragma once
#include "SPFCBaseUtil.h"

class CSpfcKS2000Util : public CSpfcBaseUtil
{
public:
	CSpfcKS2000Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;


public:
	double GetRMC() const;
	double GetOIF() const;
	double GetEPA() const;
	double GetSoil() const;
	double GetMaxPeriod() const;

	void SetRMC(double rmc);
	void SetOIF(double oif);
	void SetEPA(double epa);
	void SetSoil(double soil);
	void SetMaxPeriod(double maxPeriod);


private:
	double m_dRMC;
	double m_dOIF;
	double m_dEPA;
	double m_dSoil;
	double m_dMaxPeriod;
};

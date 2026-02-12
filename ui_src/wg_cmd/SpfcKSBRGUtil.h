#pragma once
#include "SPFCBaseUtil.h"

class CSpfcKSBRGUtil : public CSpfcBaseUtil
{
public:
	CSpfcKSBRGUtil();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

	double GetRMC() const;
	double GetOIF() const;
	double GetEPA() const;
	double GetSoil() const;
	double GetMaxPeriod() const;
	double GetTs() const;

	void SetRMC(double rmc);
	void SetOIF(double oif);
	void SetEPA(double epa);
	void SetSoil(double soil);
	void SetMaxPeriod(double maxPeriod);
	void SetTs(double dTs);

private:
	double m_dRMF;
	double m_dIF;
	double m_dEA;
	double m_dSoil;
	double m_dMaxPeriod;

	double m_dTs;
};

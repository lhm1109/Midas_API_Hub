#pragma once
#include "SPFCBaseUtil.h"

class CSpfcKSUtil : public CSpfcBaseUtil
{
public:
	CSpfcKSUtil();

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
	double m_dMaxPeriod;	// Dlg와 중복이지만, 계산을 위해 이 변수가 필요한 상황
};

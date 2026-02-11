#pragma once
#include "SpfcBaseUtil.h"

class CSpfcSP2016Util : public CSpfcBaseUtil
{
public:
	CSpfcSP2016Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;

public:
	int GetRegion() const { return m_nRegion; }
	int GetSoil() const { return m_nSoil; }
	double GetK1() const { return m_dK1; }
	double GetK2() const { return m_dK2; }
	double GetK3() const { return m_dK3; }
	double GetK4() const { return m_dK4; }
	double GetKPsi() const { return m_dKPsi; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }

	void SetRegion(int nRegion) { m_nRegion = nRegion; }
	void SetSoil(int nSoil) { m_nSoil = nSoil; }
	void SetK1(double dK1) { m_dK1 = dK1; }
	void SetK2(double dK2) { m_dK2 = dK2; }
	void SetK3(double dK3) { m_dK3 = dK3; }
	void SetK4(double dK4) { m_dK4 = dK4; }
	void SetKPsi(double dKPsi) { m_dKPsi = dKPsi; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }

private:

	int m_nRegion;
	int m_nSoil;
	double m_dK1;
	double m_dK2;
	double m_dK3;
	double m_dK4;
	double m_dKPsi;
	double m_dMaxPeriod;
};

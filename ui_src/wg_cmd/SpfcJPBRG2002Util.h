#pragma once
#include "SpfcBaseUtil.h"
class CSpfcJPBRG2002Util : public CSpfcBaseUtil
{
public:
	CSpfcJPBRG2002Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

private:
	void GetSpectrumValue(double* dValue, double* dExp, int nMethod);

public:
	int GetMethod() const { return m_nMethod; }
	int GetSoil() const { return m_nSoil; }
	int GetZone() const { return m_nZone; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }
	double GetCz() const { return m_dCz; }
	double GetDamping() const { return m_dDamping; }
	double GetCd() const { return m_dCd; }

	void SetMethod(int nMethod) { m_nMethod = nMethod; }
	void SetSoil(int nSoil) { m_nSoil = nSoil; }
	void SetZone(int nZone) { m_nZone = nZone; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }
	void SetCz(double dCz) { m_dCz = dCz; }
	void SetDamping(double dDamping) { m_dDamping = dDamping; }
	void SetCd(double dCd) { m_dCd = dCd; }

private:
	int m_nMethod;
	int m_nZone;
	int m_nSoil;
	double m_dCz;
	double m_dDamping;
	double m_dMaxPeriod;

	double m_dCd;
};

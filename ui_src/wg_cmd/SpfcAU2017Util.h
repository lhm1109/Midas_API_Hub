#pragma once
#include "SpfcBaseUtil.h"

class CSpfcAU2017Util : public CSpfcBaseUtil
{
public:
	CSpfcAU2017Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

	double GetChT(double dTn);

public:
	int GetSoilClass() const { return m_nSoilClass; }
	double GetKp() const { return m_dkp; }
	double GetZ() const { return m_dZ; }
	double GetMu() const { return m_dMu; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }
	CString GetSC() const { return m_strSC; }

	void SetSoilClass(int nSoilClass) { m_nSoilClass = nSoilClass; }
	void SetKp(double dkp) { m_dkp = dkp; }
	void SetZ(double dZ) { m_dZ = dZ; }
	void SetMu(double dMu) { m_dMu = dMu; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }
	void SetSC(const CString& strSC) { m_strSC = strSC; }

private:
	int m_nSoilClass;
	double m_dkp;
	double m_dZ;
	double m_dMu;
	double m_dMaxPeriod;

	CString m_strSC;
};

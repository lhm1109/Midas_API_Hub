#pragma once
#include "SpfcBaseUtil.h"

class CSpfcAASHTOLRFD12Util : public CSpfcBaseUtil
{
public:
	CSpfcAASHTOLRFD12Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

	void SetValue();

public:
    double GetSiteClass() const { return m_nSiteClasee; }
    double GetPGA() const { return m_dPGA; }
    double GetSS() const { return m_dSS; }
    double GetS1() const { return m_dS1; }
    double GetRMF() const { return m_dRMF; }
    double GetMaxPeriod() const { return m_dMaxPeriod; }
    CString GetSoil() const { return m_strSoil; }
    double GetFpga() const { return m_dFpga; }
    double GetFa() const { return m_dFa; }
    double GetFv() const { return m_dFv; }

    void SetSiteClass(double nSiteClasee) { m_nSiteClasee = nSiteClasee; }
    void SetPGA(double dPGA) { m_dPGA = dPGA; }
    void SetSS(double dSS) { m_dSS = dSS; }
    void SetS1(double dS1) { m_dS1 = dS1; }
    void SetRMF(double dRMF) { m_dRMF = dRMF; }
    void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }
    void SetSoil(CString strSoil) { m_strSoil = strSoil; }
    void SetFpga(double dFpga) { m_dFpga = dFpga; }
    void SetFa(double dFa) { m_dFa = dFa; }
    void SetFv(double dFv) { m_dFv = dFv; }

private:
	double m_nSiteClasee;
	double m_dPGA;
	double m_dSS;
	double m_dS1;
	double m_dRMF;
	double m_dMaxPeriod;

	CString m_strSoil;
	double m_dFpga;
	double m_dFa;
	double m_dFv;
};

#pragma once
#include "SpfcBaseUtil.h"

class CSpfcPH2013Util : public CSpfcBaseUtil
{
public:
	CSpfcPH2013Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

public:
	int GetGroundType() const { return m_nGroundType; }
	double GetPGA() const { return m_dPGA; }
	double GetSS() const { return m_dSS; }
	double GetS1() const { return m_dS1; }
	double GetRMF() const { return m_dRMF; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }
	CString GetGT() const { return m_strGT; }
	double GetFpga() const { return m_dFpga; }
	double GetFa() const { return m_dFa; }
	double GetFv() const { return m_dFv; }

	void SetGroundType(int nGroundType) { m_nGroundType = nGroundType; }
	void SetPGA(double dPGA) { m_dPGA = dPGA; }
	void SetSS(double dSS) { m_dSS = dSS; }
	void SetS1(double dS1) { m_dS1 = dS1; }
	void SetRMF(double dRMF) { m_dRMF = dRMF; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }
	void SetGT(const CString& strGT) { m_strGT = strGT; }
	void SetFpga(double dFpga) { m_dFpga = dFpga; }
	void SetFa(double dFa) { m_dFa = dFa; }
	void SetFv(double dFv) { m_dFv = dFv; }

private:
	int m_nGroundType;
	double m_dPGA;
	double m_dSS;
	double m_dS1;
	double m_dRMF;
	double m_dMaxPeriod;

	CString m_strGT;
	double m_dFpga;
	double m_dFa;
	double m_dFv;
};

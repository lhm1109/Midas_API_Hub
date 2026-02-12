#pragma once
#include "SPFCBaseUtil.h"

class CSpfcCH2002Util : public CSpfcBaseUtil
{
public:
	CSpfcCH2002Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
    void SetParamFromCalc() override;

public:
    double GetSDC() const { return m_nSDC; }
    double GetSFI() const { return m_nSFI; }
    double GetSC() const { return m_nSC; }
    double GetER() const { return m_nER; }
    double GetTg() const { return m_dTg; }
    double GetXi() const { return m_dXi; }
    double GetMaxEQE() const { return m_dMaxEQE; }
    double GetMaxPeriod() const { return m_dMaxPeriod; }
    double GetAlphaMax() const { return m_dAlphaMax; }
    double GetGamma() const { return m_dGamma; }
    double GetEta1() const { return m_dEta1; }
    double GetEta2() const { return m_dEta2; }

    void SetSDC(double val) { m_nSDC = val; }
    void SetSFI(double val) { m_nSFI = val; }
    void SetSC(double val) { m_nSC = val; }
    void SetER(double val) { m_nER = val; }
    void SetTg(double val) { m_dTg = val; }
    void SetXi(double val) { m_dXi = val; }
    void SetMaxEQE(double val) { m_dMaxEQE = val; }
    void SetMaxPeriod(double val) { m_dMaxPeriod = val; }
    void SetAlphaMax(double val) { m_dAlphaMax = val; }
    void SetGamma(double val) { m_dGamma = val; }
    void SetEta1(double val) { m_dEta1 = val; }
    void SetEta2(double val) { m_dEta2 = val; }

private:
	int m_nSDC;
	int m_nSFI;
	int m_nSC;
	int m_nER;
	double m_dTg;
	double m_dXi;
	double m_dMaxEQE;
	double m_dMaxPeriod;

	double m_dAlphaMax;
	double m_dGamma;
	double m_dEta1;
	double m_dEta2;
};

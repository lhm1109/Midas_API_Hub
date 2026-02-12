#pragma once
#include "SPFCBaseUtil.h"

class CSpfcCHSH2003Util : public CSpfcBaseUtil
{
public:
	CSpfcCHSH2003Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

public:
	int GetSFI() const { return m_nSFI; }
	int GetSC() const { return m_nSC; }
	int GetER() const { return m_nER; }
	double GetTg() const { return m_dTg; }
	double GetXi() const { return m_dXi; }
	double GetMaxEQE() const { return m_dMaxEQE; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }
	double GetGamma() const { return m_dGamma; }
	double GetEta1() const { return m_dEta1; }
	double GetEta2() const { return m_dEta2; }
	double GetAlphaMax() const { return m_dAlphaMax; }

	void SetSFI(int nSFI) { m_nSFI = nSFI; }
	void SetSC(int nSC) { m_nSC = nSC; }
	void SetER(int nER) { m_nER = nER; }
	void SetTg(double dTg) { m_dTg = dTg; }
	void SetXi(double dXi) { m_dXi = dXi; }
	void SetMaxEQE(double dMaxEQE) { m_dMaxEQE = dMaxEQE; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }
	void SetGamma(double dGamma) { m_dGamma = dGamma; }
	void SetEta1(double dEta1) { m_dEta1 = dEta1; }
	void SetEta2(double dEta2) { m_dEta2 = dEta2; }
	void SetAlphaMax(double dAlphaMax) { m_dAlphaMax = dAlphaMax; }

private:
	int m_nSFI;
	int m_nSC;
	int m_nER;
	double m_dTg;
	double m_dXi;
	double m_dMaxEQE;
	double m_dMaxPeriod;

	double m_dGamma;
	double m_dEta1;
	double m_dEta2;
	double m_dAlphaMax;
};

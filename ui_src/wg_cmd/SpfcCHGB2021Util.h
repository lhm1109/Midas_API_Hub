#pragma once
#include "SPFCBaseUtil.h"

class CSpfcCHGB2021Util : public CSpfcBaseUtil
{
public:
	CSpfcCHGB2021Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

public:
	int GetSDC() const { return m_nSDC; }
	int GetSFI() const { return m_nSFI; }
	int GetSC() const { return m_nSC; }
	int GetER() const { return m_nER; }

	double GetTg() const { return m_dTg; }
	double GetXi() const { return m_dXi; }
	int GetLForce() const { return m_nLForce; }
	double GetMaxEQE() const { return m_dMaxEQE; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }

	double GetAlphaMax() const { return m_dAlphaMax; }
	double GetGamma() const { return m_dGamma; }
	double GetEta() const { return m_dEta; }

	void SetSDC(int nSDC) { m_nSDC = nSDC; }
	void SetSFI(int nSFI) { m_nSFI = nSFI; }
	void SetSC(int nSC) { m_nSC = nSC; }
	void SetER(int nER) { m_nER = nER; }

	void SetTg(double dTg) { m_dTg = dTg; }
	void SetXi(double dXi) { m_dXi = dXi; }
	void SetLForce(int nLForce) { m_nLForce = nLForce; }
	void SetMaxEQE(double dMaxEQE) { m_dMaxEQE = dMaxEQE; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }

	void SetAlphaMax(double dAlphaMax) { m_dAlphaMax = dAlphaMax; }
	void SetGamma(double dGamma) { m_dGamma = dGamma; }
	void SetEta(double dEta) { m_dEta = dEta; }

private:
	int m_nSDC;
	int m_nSFI;
	int m_nSC;
	int m_nER;

	double m_dTg;
	double m_dXi;
	int m_nLForce;
	double m_dMaxEQE;
	double m_dMaxPeriod;

	double m_dAlphaMax;
	double m_dGamma;
	double m_dEta;
};

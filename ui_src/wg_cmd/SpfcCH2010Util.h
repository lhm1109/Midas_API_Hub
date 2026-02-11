#pragma once
#include "SPFCBaseUtil.h"

class CSpfcCH2010Util : public CSpfcBaseUtil
{
public:
	CSpfcCH2010Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

	void CalcEarthquakeCoefFromPushOver();

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
	double GetEta1() const { return m_dEta1; }
	double GetEta2() const { return m_dEta2; }

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
	void SetEta1(double dEta1) { m_dEta1 = dEta1; }
	void SetEta2(double dEta2) { m_dEta2 = dEta2; }

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
	double m_dEta1;
	double m_dEta2;
};
//
//int     nSeisDgnCategory; // 0:1, 1:2, 2:3
//int     nSeisFortification; // 0:0.05g, 1:0.10g, 2:0.15g, 3:0.20g, 4:0.30g, 5:0.40g
//int     nSiteClass; // 0:I0, 1:I1, 2:II, 3:III, 4:IV
//int     nEarthResponse; // 0:Frequent, 1:Mid, 2:Scarce
//double  dTg; // Design Characteristic Period of Ground Motion.
//double  dDamping; // Damping Ratio.
//int     nLForce;    // 0:층전단력차, 1:관성력
//double  dMaxEQE; // Maximum Effect Coefficient
//double dMaxPeriod; // 9.5.5
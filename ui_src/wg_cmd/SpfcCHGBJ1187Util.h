#pragma once
#include "SPFCBaseUtil.h"

class CSpfcCHGBJ1187Util : public CSpfcBaseUtil
{
public:
	CSpfcCHGBJ1187Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

public:
    int GetSFI() const { return m_nSFI; }
    double GetKh() const { return m_dKh; }
    int GetSC() const { return m_nSC; }
    double GetTg1() const { return m_dTg1; }
    double GetTg2() const { return m_dTg2; }
    double GetCz() const { return m_dCz; }
    double GetMaxPeriod() const { return m_dMaxPeriod; }
    double GetMu() const { return m_dMu; }

    void SetSFI(int nSFI) { m_nSFI = nSFI; }
    void SetKh(double dKh) { m_dKh = dKh; }
    void SetSC(int nSC) { m_nSC = nSC; }
    void SetTg1(double dTg1) { m_dTg1 = dTg1; }
    void SetTg2(double dTg2) { m_dTg2 = dTg2; }
    void SetCz(double dCz) { m_dCz = dCz; }
    void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }
    void SetMu(double dMu) { m_dMu = dMu; }

private:
	int m_nSFI;
	double m_dKh;   // 수평지진계수
	int m_nSC;
	double m_dTg1;
	double m_dTg2;
	double m_dCz;   // General Effect Factor(종합영향계수)
	double m_dMaxPeriod;

	double m_dMu;

};

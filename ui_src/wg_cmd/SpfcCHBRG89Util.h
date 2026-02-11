#pragma once
#include "SPFCBaseUtil.h"

class CSpfcCHBRG89Util : public CSpfcBaseUtil
{
public:
	CSpfcCHBRG89Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
    void SetParamFromCalc() override;


public:
    int GetSFI() const { return m_nSFI; }
    double GetKh() const { return m_dKh; }
    int GetSC() const { return m_nSC; }
    double GetTg() const { return m_dTg; }
    double GetCi() const { return m_dCi; }
    double GetCz() const { return m_dCz; }
    double GetMaxPeriod() const { return m_dMaxPeriod; }
    double GetMu() const { return m_dMu; }
    double GetK() const { return m_dk; }

    void SetSFI(int nSFI) { m_nSFI = nSFI; }
    void SetKh(double dKh) { m_dKh = dKh; }
    void SetSC(int nSC) { m_nSC = nSC; }
    void SetTg(double dTg) { m_dTg = dTg; }
    void SetCi(double dCi) { m_dCi = dCi; }
    void SetCz(double dCz) { m_dCz = dCz; }
    void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }
    void SetMu(double dMu) { m_dMu = dMu; }
    void SetK(double dk) { m_dk = dk; }

private:
	int m_nSFI;
	double m_dKh;   // 수평지진계수
	int m_nSC;
	double m_dTg;   // 특정주기
	double m_dCi;   // importance factor(중요도 수정계수)
	double m_dCz;   // General Effect Factor(종합영향계수)
	double m_dMaxPeriod;

	double m_dMu;   // Site 평가지수
	double m_dk;    // 동적증대계수 구하는데 쓰이는 변수 k = 0.9 + 0.1 mu

};

#pragma once
#include "SPFCBaseUtil.h"

class CSpfcGB50111_2006Util : public CSpfcBaseUtil
{
public:
	CSpfcGB50111_2006Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping , double SRa , double SRv , CArray <double , double>& aPeriod , CArray <double , double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;

public:
	int GetDivision() const { return m_nDivision; }
	int GetSiteClass() const { return m_nSiteClass; }
	int GetSFI() const { return m_nSFI; }
	int GetEarthEff() const { return m_nEarthEff; }
	double GetTg() const { return m_dTg; }
	double GetG() const { return m_dG; }
	BOOL GetImportantBridge() const { return m_bImportantBridge; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }

	void SetDivision(int nDivision) { m_nDivision = nDivision; }
	void SetSiteClass(int nSiteClass) { m_nSiteClass = nSiteClass; }
	void SetSFI(int nSFI) { m_nSFI = nSFI; }
	void SetEarthEff(int nEarthEff) { m_nEarthEff = nEarthEff; }
	void SetTg(double dTg) { m_dTg = dTg; }
	void SetG(double dG) { m_dG = dG; }
	void SetImportantBridge(BOOL bImportantBridge) { m_bImportantBridge = bImportantBridge; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }

private:
	int m_nDivision;
	int m_nSiteClass;
	int m_nSFI;
	int m_nEarthEff;
	double m_dTg;
	double m_dG;
	BOOL m_bImportantBridge;
	double m_dMaxPeriod;
};

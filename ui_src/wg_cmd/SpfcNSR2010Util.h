#pragma once
#include "SpfcBaseUtil.h"

class CSpfcNSR2010Util : public CSpfcBaseUtil
{
public:
	CSpfcNSR2010Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

public:
	int GetSite() const { return m_nSite; }
	double GetAa() const { return m_dAa; }
	double GetAv() const { return m_dAv; }
	double GetFa() const { return m_dFa; }
	double GetFv() const { return m_dFv; }
	double GetI() const { return m_dI; }
	double GetR() const { return m_dR; }
	double GetPhi() const { return m_dPhi; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }
	double GetT0() const { return m_dT0; }
	double GetTc() const { return m_dTc; }
	double GetTL() const { return m_dTL; }
	double GetSdsUnUsed() const { return m_dSdsUnUsed; }
	double GetSdUnUsed() const { return m_dSdUnUsed; }
	double GetTLUnUsed() const { return m_dTLUnUsed; }

	void SetSite(int nSite) { m_nSite = nSite; }
	void SetAa(double dAa) { m_dAa = dAa; }
	void SetAv(double dAv) { m_dAv = dAv; }
	void SetFa(double dFa) { m_dFa = dFa; }
	void SetFv(double dFv) { m_dFv = dFv; }
	void SetI(double dI) { m_dI = dI; }
	void SetR(double dR) { m_dR = dR; }
	void SetPhi(double dPhi) { m_dPhi = dPhi; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }
	void SetT0(double dT0) { m_dT0 = dT0; }
	void SetTc(double dTc) { m_dTc = dTc; }
	void SetTL(double dTL) { m_dTL = dTL; }
	void SetSdsUnUsed(double dSdsUnUsed) { m_dSdsUnUsed = dSdsUnUsed; }
	void SetSdUnUsed(double dSdUnUsed) { m_dSdUnUsed = dSdUnUsed; }
	void SetTLUnUsed(double dTLUnUsed) { m_dTLUnUsed = dTLUnUsed; }

private:
	int m_nSite;
	double m_dAa;
	double m_dAv;
	double m_dFa;
	double m_dFv;
	double m_dI;
	double m_dR;
	double m_dPhi;
	double m_dMaxPeriod;

	double m_dT0;
	double m_dTc;
	double m_dTL;
	double m_dSdsUnUsed;
	double m_dSdUnUsed;
	double m_dTLUnUsed;

};
//
//int nSiteClass;     // Site Class(A, B, C, D, E)
//double dAa;         // Effective Peak Acc.
//double dAv;         // Effective Peak Vel.
//double dFa;         // Site Coeff. at Short Period(Fa)
//double dFv;         // Site Coeff. at 1sec Period(Fv)
//double dIe;         // Importance Factor (Ie)
//double dCoef;       // Basic Ductility Factor. (R0)
//double dPhi;        // Phi
//double dMaxPeriod;  // Max. Period
#pragma once
#include "SpfcBaseUtil.h"

class CSpfcNBC95Util : public CSpfcBaseUtil
{
public:
	CSpfcNBC95Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

public:
	double GetV() const { return m_dv; }
	double GetZa() const { return m_dZa; }
	double GetZv() const { return m_dZv; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }
	double GetI() const { return m_dI; }
	double GetF() const { return m_dF; }
	double GetR() const { return m_dR; }

	void SetV(double dv) { m_dv = dv; }
	void SetZa(double dZa) { m_dZa = dZa; }
	void SetZv(double dZv) { m_dZv = dZv; }
	void SetMaxPeriod(double maxPeriod) { m_dMaxPeriod = maxPeriod; }
	void SetI(double dI) { m_dI = dI; }
	void SetF(double dF) { m_dF = dF; }
	void SetR(double dR) { m_dR = dR; }

private:
	double m_dv;
	double m_dZa;
	double m_dZv;
	double m_dMaxPeriod;

	double m_dI;
	double m_dF;
	double m_dR;
};

#pragma once
#include "SpfcBaseUtil.h"

class CSpfcPO2013Util : public CSpfcBaseUtil
{
public:
	CSpfcPO2013Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

public:
	int GetSpecType() const { return m_nSpecType; }
	int GetParaTypeRdo() const { return m_nParaTypeRdo; }
	double GetTb() const { return m_dTb; }
	double GetTc() const { return m_dTc; }
	double GetTd() const { return m_dTd; }
	double GetAg() const { return m_dAg; }
	double GetImportance() const { return m_dImportance; }
	double GetDamping() const { return m_dDamping; }
	double GetQ() const { return m_dQ; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }

	void SetSpecType(int nSpecType) { m_nSpecType = nSpecType; }
	void SetParaTypeRdo(int nParaTypeRdo) { m_nParaTypeRdo = nParaTypeRdo; }
	void SetTb(double dTb) { m_dTb = dTb; }
	void SetTc(double dTc) { m_dTc = dTc; }
	void SetTd(double dTd) { m_dTd = dTd; }
	void SetAg(double dAg) { m_dAg = dAg; }
	void SetImportance(double dImportance) { m_dImportance = dImportance; }
	void SetDamping(double dDamping) { m_dDamping = dDamping; }
	void SetQ(double dQ) { m_dQ = dQ; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }

private:
	int m_nSpecType;
	int m_nParaTypeRdo;
	double m_dTb;
	double m_dTc;
	double m_dTd;
	double m_dAg;
	double m_dImportance;
	double m_dDamping;
	double m_dQ;
	double m_dMaxPeriod;
};

//int nSpectrumType;    // D_SPFC_P100_2013_SP
//int nParameterType;   // D_SPFC_P100_2013_TC
//double dTb;           // Tb
//double dTc;           // Tc
//double dTd;           // Td
//double dAgr;          // Design Ground Acc. (AgR)
//double dI;            // Importance Factor(I)
//double dXi;           // Viscous Damping Ratio(Xi)
//double dQ;            // Behavior Factor(q)
//double dMaxPeriod;    // Max. Period


#pragma once
#include "SpfcBaseUtil.h"

class CSpfcEURO2004Util : public CSpfcBaseUtil
{
public:
	CSpfcEURO2004Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

	T_SPFC_CODE GetCodeFromParam() const;

public:
	int GetSpecType() const { return m_nSpecType; }
	int GetGroundType() const { return m_nGroundType; }
	int GetParaTypeRdo() const { return m_nParaTypeRdo; }
	double GetS() const { return m_dS; }
	double GetTb() const { return m_dTb; }
	double GetTc() const { return m_dTc; }
	double GetTd() const { return m_dTd; }
	double GetAg() const { return m_dAg; }
	double GetImportance() const { return m_dImportance; }
	double GetDamping() const { return m_dDamping; }
	double GetQ() const { return m_dQ; }
	double GetBeta() const { return m_dBeta; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }
	double GetTs() const { return m_dTs; }
	EN_SPFC_NA GetNAIndex() const { return m_nNAIndex; }
	int GetRegion() const { return m_nRegion; }

	void SetSpecType(int nSpecType) { m_nSpecType = nSpecType; }
	void SetGroundType(int nGroundType) { m_nGroundType = nGroundType; }
	void SetParaTypeRdo(int nParaTypeRdo) { m_nParaTypeRdo = nParaTypeRdo; }
	void SetS(double dS) { m_dS = dS; }
	void SetTb(double dTb) { m_dTb = dTb; }
	void SetTc(double dTc) { m_dTc = dTc; }
	void SetTd(double dTd) { m_dTd = dTd; }
	void SetAg(double dAg) { m_dAg = dAg; }
	void SetImportance(double dImportance) { m_dImportance = dImportance; }
	void SetDamping(double dDamping) { m_dDamping = dDamping; }
	void SetQ(double dQ) { m_dQ = dQ; }
	void SetBeta(double dBeta) { m_dBeta = dBeta; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }
	void SetTs(double dTs) { m_dTs = dTs; }
	void SetNAIndex(EN_SPFC_NA nNAIndex) { m_nNAIndex = nNAIndex; }
	void SetRegion(int nRegion) { m_nRegion = nRegion; }

private:
	int m_nSpecType;
	int m_nGroundType;

	int m_nParaTypeRdo;
	double m_dS;
	double m_dTb;
	double m_dTc;
	double m_dTd;
	double m_dAg;
	double m_dImportance;
	double m_dDamping;
	double m_dQ;
	double m_dBeta;
	double m_dMaxPeriod;

	double m_dTs;
	EN_SPFC_NA m_nNAIndex;
	int m_nRegion;

};

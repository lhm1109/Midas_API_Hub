#pragma once
#include "SpfcBaseUtil.h"

class CSpfcNTC2018Util : public CSpfcBaseUtil
{
public:
	CSpfcNTC2018Util(int nCase = 0);

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;

	void CtrlManager();

private:
	T_SPFC_NTC2018 GetCodeFromParam() const;

public:
	int GetSpecType() const { return m_nSpecType; }
	int GetGroundType() const { return m_nGroundType; }
	int GetParaTypeRdo() const { return m_nParaTypeRdo; }
	double GetS() const { return m_dS; }
	double GetTb() const { return m_dTb; }
	double GetTc() const { return m_dTc; }
	double GetTd() const { return m_dTd; }
	double GetAg() const { return m_dAg; }
	double GetFo() const { return m_dFo; }
	double GetTcStar() const { return m_dTcStar; }
	double GetDamping() const { return m_dDamping; }
	double GetQ() const { return m_dQ; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }

	void SetSpecType(int nSpecType) { m_nSpecType = nSpecType; }
	void SetGroundType(int nGroundType) { m_nGroundType = nGroundType; }
	void SetParaTypeRdo(int nParaTypeRdo) { m_nParaTypeRdo = nParaTypeRdo; }
	void SetS(double dS) { m_dS = dS; }
	void SetTb(double dTb) { m_dTb = dTb; }
	void SetTc(double dTc) { m_dTc = dTc; }
	void SetTd(double dTd) { m_dTd = dTd; }
	void SetAg(double dAg) { m_dAg = dAg; }
	void SetFo(double dFo) { m_dFo = dFo; }
	void SetTcStar(double dTcStar) { m_dTcStar = dTcStar; }
	void SetDamping(double dDamping) { m_dDamping = dDamping; }
	void SetQ(double dQ) { m_dQ = dQ; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }

private:
	int m_nSpecType;
	int m_nGroundType;
	int m_nParaTypeRdo;		// UI 연결 확인 필요
	double m_dS;
	double m_dTb;
	double m_dTc;
	double m_dTd;
	double m_dAg;
	double m_dFo;
	double m_dTcStar;
	double m_dDamping;
	double m_dQ;
	double m_dMaxPeriod;	// UI 연결 확인 필요

	int m_nCase;

};

//int nSpectrumType;    // 0:Horizontal Elastic, 1:Vertical Elastic, 2:Horizontal Design, 3:Vertical Design(Recommended), 10:Horizontal Elastic, 11:Horizontal Design (Singapore)
//int nGroundType;      // 0:A, 1:B, 2:C, 3:D, 4:E
//int nParameterType;   // 0:T1, 1:T2, 2:T3, 3:T4, 4:User Defined
//double dSoilFactor;   // SoilFactor
//double dTb;           // Tb
//double dTc;           // Tc
//double dTd;           // Td
//double dAg;                     // Maximum Horizontal Acc. (Ag)
//double dFo;           // Amplification Factor (Fo)
//double dTcStar;             // Period of constant Hor. Acc. (Tc*)
//double dXi;           // Viscous Damping Ratio (Xi)
//double dQ;            // Behavior Factor (q)
//double dMaxPeriod;    // Max. Period
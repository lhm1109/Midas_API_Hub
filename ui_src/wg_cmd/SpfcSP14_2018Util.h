#pragma once
#include "SpfcBaseUtil.h"

enum class SPSoil
{
	enS1 = 0,
	enS2,
	enS3,
	enS4,
};

class CSpfcSP14_2018Util : public CSpfcBaseUtil
{
public:
	CSpfcSP14_2018Util();

public:
	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;

public:
	void GetDefault(T_SPFC_RUS_SP14_2018& rData);
	bool CheckData(const T_SPFC_RUS_SP14_2018& cData);
	bool GetSoilData(std::vector<CString>& rvSoil);
	CString GetFuncDesc(const T_SPFC_RUS_SP14_2018& cData);

public:
	int  GetSoil() const { return m_nSoil; }
	bool GetNLDS() const { return m_bNLDS; }
	double GetK0() const { return m_dK0; }
	double GetK1() const { return m_dK1; }
	double GetKPsi() const { return m_dKPsi; }
	double GetAcce() const { return m_dAcce; }
	double GetMaxPeriod() const { return m_dMaxT; }

public:
	void SetSoil(int nSoil) { m_nSoil = nSoil; }
	void SetNLDS(bool bNLDS) { m_bNLDS = bNLDS; }
	void SetK0(double dK0) { m_dK0 = dK0; }
	void SetK1(double dK1) { m_dK1 = dK1; }
	void SetKPsi(double dKPsi) { m_dKPsi = dKPsi; }
	void SetAcce(double dAcce) { m_dAcce = dAcce; }
	void SetMaxPeriod(double dMaxT) { m_dMaxT = dMaxT; }

private:
	void GetSpecificT(SPSoil enSoil, double& rdT0, double& rdT1);
	bool IsTransientT(double dT, double dTs, double dTinc, double dTol);
	void SetSpecInfo();
	void GetData(T_SPFC_RUS_SP14_2018& rData);

	double Getg();
	double CalcSa(SPSoil enSoil, double dT);
	double CalcBeta(SPSoil enSoil, double dT);
	CString GetSoilInfo(int nSoil);
	CString GetFuncName();

private:
	int    m_nSoil;
	bool   m_bNLDS; // Nonlinear deformation of soil
	double m_dK0;
	double m_dK1;
	double m_dKPsi;
	double m_dAcce;
	double m_dMaxT;
};

#pragma once
#include "SpfcBaseUtil.h"

class CSpfcUBC97Util : public CSpfcBaseUtil
{
public:
	CSpfcUBC97Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

	void AutoCalcSeisCoef(bool isApiMode = false);

public:
	int GetOption() const { return m_nOption; }
	int GetSoilType() const { return m_nSoilType; }
	int GetSeisZone() const { return m_nSeisZone; }
	int GetSourceType() const { return m_nSourceType; }
	double GetClosestDist() const { return m_dClosestDist; }
	double GetCa() const { return m_dCa; }
	double GetCv() const { return m_dCv; }
	double GetIF() const { return m_dIF; }
	double GetNC() const { return m_dNC; }
	double GetMaxPeriod() const { return m_dMaxPeriod; }

	void SetOption(int nOption) { m_nOption = nOption; }
	void SetSoilType(int nSoilType) { m_nSoilType = nSoilType; }
	void SetSeisZone(int nSeisZone) { m_nSeisZone = nSeisZone; }
	void SetSourceType(int nSourceType) { m_nSourceType = nSourceType; }
	void SetClosestDist(double dClosestDist) { m_dClosestDist = dClosestDist; }
	void SetCa(double dCa) { m_dCa = dCa; }
	void SetCv(double dCv) { m_dCv = dCv; }
	void SetIF(double dIF) { m_dIF = dIF; }
	void SetNC(double dNC) { m_dNC = dNC; }
	void SetMaxPeriod(double dMaxPeriod) { m_dMaxPeriod = dMaxPeriod; }

private:
	int m_nOption;
	int m_nSoilType;
	int m_nSeisZone;
	int m_nSourceType;
	double m_dClosestDist;
	double m_dCa;
	double m_dCv;
	double m_dIF;
	double m_dNC;
	double m_dMaxPeriod;
};

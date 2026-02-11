#pragma once
#include "SpfcBaseUtil.h"
#include "ETC_GDS_TAIWAN02_CALC.h"

typedef CArray<double, double> SeisDblArray;

enum EN_TAIWAN_SPECTRUM_TYPE : unsigned int
{
	enSpectTypeHOR = 0, // Horizontal
	enSpectTypeVER, // Vertical
};

enum EN_TAIWAN_SEIS_ZONE : unsigned int
{
	enSeisZoneGEN = 0, // General Zone
	enSeisZoneNFZ, // Near Fault Zone
	enSeisZoneTPB, // Taipei Basin
};

enum EN_TAIWAN_SEIS_SUBZONE : unsigned int
{
	enSubZoneTPB1 = 0, // Taipei Basin 1
	enSubZoneTPB2, // Taipei Basin 2
	enSubZoneTPB3, // Taipei Basin 3
	enSubZoneTPB4, // Taipei Basin 4
	enSubZoneUSER, // User Input
};

enum EN_TAIWAN_SPECTRUM_USED : unsigned int
{
	enSpectUsedDGN = 0, // Design
	enSpectUsedMED, // Small Medium
	enSpectUsedMAX, // Maximum
};

enum EN_TAIWAN_SOIL_TYPE : unsigned int
{
	enSoilTypeTYP1 = 0,
	enSoilTypeTYP2,
	enSoilTypeTYP3,
	enSoilTypeUSER,
};

class CSpfcTW2006Util : public CSpfcBaseUtil
{
public:
	CSpfcTW2006Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
	void SetParamFromCalc() override;

public:
	virtual void SetInitValueforPOSpectrum();
	virtual bool IsFundamentalPeriodREQ();
	virtual bool IsValidNearFaultFactor(CString& rcsMsg);

	CString GetSeismicZoneName(int nCode);
	CString GetSoilTypeName(int nCode);
	CString GetSubZoneName(int nCode);
	CString GetSpecCategoryName(int nCode);

	virtual void CalcSiteMagnifyFactor();

protected:
	double GetSaFactor(stETC_GDS_TAIWAN02_CALC& sCalc, const double dT, int nMIN);
	double GetFuFactor(double& dT, const double dT0, int nMIN);
	double GetModifiedSaFu(double dSa, double dFu);

private:
	virtual UINT GetSeismicCode() { return D_SPFC_CODE_TAIWAN06; };
	virtual CString GetFunctionName() { return _T("Taiwan(2006)"); };
	virtual CString GetFunctionDescription();
	virtual double CalcSaEveryZone(stETC_GDS_TAIWAN02_CALC& sCalc, double dTn, double dT1, double dSRa, double dSRv, BOOL bPOcurve);
	virtual double GetInitFundamentalPeriod();

	virtual bool CalcGeneralZone(stETC_GDS_TAIWAN02_CALC& sCalc);
	virtual bool CalcNearFaultZone(stETC_GDS_TAIWAN02_CALC& sCalc);
	virtual bool CalcTaipeiBasin(stETC_GDS_TAIWAN02_CALC& sCalc);

	bool GetBFactorbyDamping(stETC_GDS_TAIWAN02_CALC& rstCalc, const double& dDamping);
	void TransPointEveryZone(stETC_GDS_TAIWAN02_CALC sCalc, double dSRa, double dSRv, double dTn, double dT1);
	void TransPointPOSpectrum(stETC_GDS_TAIWAN02_CALC sCalc, double dSRa, double dSRv, const double& dt, const double& dTol, double& rdSa, double dTx, const double& dTn, const double& dT1, SeisDblArray& radPeriod, SeisDblArray& radAccel, BOOL bPOcurve);

	double LinearInt(double dx1, double dx2, double dy1, double dy2, double dxx);

	void MakePOSpectrumData_sub(double dDamping, double dSRa, double dSRv, SeisDblArray& radPeriod, SeisDblArray& radAccel, BOOL bMakeSpectrum = FALSE, BOOL bOnlyCalc = FALSE);

public:
	int m_nSpecCategory; // 0:Design ,1: Small-Medium, 2:Maximum
	int m_nSeisZone;     // 0:General, 1:Near fault, 2:Taipei Basin
	int m_nSpecType;     // Horizontal, Vertical
	// - General / Near Fault Zone
	double m_dSds;        double m_dSd1;    // d -> design ,       m -> maximum
	double m_dSms;        double m_dSm1;    // s -> short period,  1 -> 1 sec
	int    m_nSoilType;                     // Site Magnify Factor
	double m_dFda;        double m_dFdv;
	double m_dFma;        double m_dFmv;
	// Near Source Factor(Near Fault Zone only)
	double m_dNda;        double m_dNdv;
	double m_dNma;        double m_dNmv;
	double m_dFda_1;      double m_dFdv_1;    //Fa, Fv 산정시 (Na=1.0)*Sds, (Nv=1.0)*Sd1 에 의해 구함
	// Taipei Basin only
	int    m_nSubZone;
	double m_dSds_t;      double m_dSms_t;
	double m_dTd0_t;      double m_dTm0_t;

	double m_dI;
	double m_dAy;
	double m_dR;
	double m_dDamping;
	double m_dMaxPeriod;
	double m_dFundPeriod;

};

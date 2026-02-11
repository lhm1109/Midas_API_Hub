#pragma once
#include "SpfcBaseUtil.h"
#include "ETC_GDS_TAIWAN02_CALC.h"

class CSpfcTAIWAN98Util : public CSpfcBaseUtil
{
public:
	CSpfcTAIWAN98Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
    void SetParamFromCode(const T_SPFC_CODE& codeParam) override;
    void SetParamFromCalc() override;


    CString GetSeismicZoneName(int nCode);
    CString GetSoilTypeName(int nCode);
    CString GetSubZoneName(int nCode);
    CString GetSpecCategoryName(int nCode);

private:

    bool Calc_GeneralZone(stETC_GDS_TAIWAN02_CALC& sCalc);
    bool Calc_NearFaultZone(stETC_GDS_TAIWAN02_CALC& sCalc);
    bool Calc_TaipaiBasin(stETC_GDS_TAIWAN02_CALC& sCalc);

    double GetSaFactor(stETC_GDS_TAIWAN02_CALC& sCalc, const double dT, int nMIN);
    bool   GetFuFactor(double& dT, const double dT0, int nMIN, double& dFu);
    double GetModifiedSaFu(double dSa, double dFu);
    double GetModifiedSaFuAv(double dSa, double dFu, double dAv);
    double GetModified0_4SDSFu(stETC_GDS_TAIWAN02_CALC& sCalc, double dFu, int nMIN);

    bool GetBFactorbyDamping(const double dDamping);
    double LinearInt(double dx1, double dx2, double dy1, double dy2, double dxx); //Linear Interpolation

    double Calc_dSa_EveryZone(stETC_GDS_TAIWAN02_CALC& sCalc, double Tn, double SRa, double SRv, BOOL bPOcurve = FALSE);
    void TransPoint_EveryZone(stETC_GDS_TAIWAN02_CALC sCalc, double SRa, double SRv, double dTn);
    void TransPoint_POSpectrum(stETC_GDS_TAIWAN02_CALC sCalc, double SRa, double SRv, double& dt, double& dTol,
        double& dSa, double dTx, double& Tn, CArray <double, double>& aPeriod, CArray <double, double>& aAccel, BOOL bPOcurve);

public:
    // MNET:2410-GSJI-20060829
    stETC_GDS_TAIWAN02_CALC sCalc;      // Variables
    double m_dFda_1;                    //Fa, Fv 산정시 (Na=1.0)*Sds, (Nv=1.0)*Sd1 에 의해 구함
    double m_dFdv_1;


	int m_nSpecType;                    // Horizontal, Vertical
	int m_nSeisZone;                    // 0:General, 1:Near fault, 2:Taipai Basin
	int m_nSpecCategory;                // 0:Design ,1: Small-Medium, 2:Maximum

    // - General / Near Fault Zone
	double m_dSds;    // d -> design ,       m -> maximum 
	double m_dSd1;    // s -> short period,  1 -> 1 sec
	double m_dSms;
	double m_dSm1;
	int m_nSoilType;                    // Site Magnify Factor
	double m_dFda;
	double m_dFdv;
	double m_dFma;
	double m_dFmv;

    // Near Source Factor(Near Fault Zone only)
    double m_dNda;
    double m_dNdv;
    double m_dNma;
    double m_dNmv;

    // Taipai Basin only
	int m_nSubZone;
	double m_dSds_t;
	double m_dSms_t;
	double m_dTd0_t;
	double m_dTm0_t;

	double m_dI;
	double m_dAy;
	double m_dR;
	double m_dDamping;                  // (%)
	double m_dMaxPeriod;
};

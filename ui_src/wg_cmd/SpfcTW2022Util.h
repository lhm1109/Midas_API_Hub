#pragma once
#include "SpfcTW2006Util.h"

class CSpfcTW2022Util : public CSpfcTW2006Util
{
public:
	void SetParamFromCode(const T_SPFC_CODE& codeParam) final;
	void SetParamFromCalc() final;

	void SetInitValueforPOSpectrum() override final;
	void CalcSiteMagnifyFactor() override final;

private:
	bool IsFundamentalPeriodREQ() override final;
	bool IsValidNearFaultFactor(CString& rcsMsg) override final;

	UINT GetSeismicCode() override final { return D_SPFC_CODE_TAIWAN22; };
	CString GetFunctionName() override final { return _T("Taiwan(2022)"); };
	CString GetFunctionDescription() override final;
	double CalcSaEveryZone(stETC_GDS_TAIWAN02_CALC& sCalc, double dTn, double dT1, double dSRa, double dSRv, BOOL bPOcurve) override final;

	bool CalcGeneralZone(stETC_GDS_TAIWAN02_CALC& sCalc) override final;
	bool CalcNearFaultZone(stETC_GDS_TAIWAN02_CALC& sCalc) override final;
	bool CalcTaipeiBasin(stETC_GDS_TAIWAN02_CALC& sCalc) override final;

};
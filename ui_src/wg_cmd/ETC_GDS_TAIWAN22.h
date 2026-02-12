#pragma once
#include "ETC_GDS_TAIWAN02.h"
#include "ETC_GDS_TAIWAN02_CALC.h"

class CETC_GDS_TAIWAN22 : public CETC_GDS_TAIWAN02
{
public:
	CETC_GDS_TAIWAN22(T_SPFC_D* pParamData, CWnd* pParent = NULL);

private:
	virtual BOOL Dlg2Data() override final;
	virtual void CtrlManager() override final;
	virtual void InitSpecAccNearFaultZone() override final;
	virtual void SetPushOverData(BOOL bInit=TRUE) override final;
	//virtual void SetInitValueforPOSpectrum() override final;
	//virtual bool IsFundamentalPeriodREQ() override final;
	//virtual bool IsValidNearFaultFactor(CString& rcsMsg) override final;

private:
	virtual void ChangeCaption() override final;
	virtual void ChangeCaptionSmallMedium() override final;
	//virtual UINT GetSeismicCode() override final { return D_SPFC_CODE_TAIWAN22; };
	//virtual CString GetFunctionName() override final { return _T("Taiwan(2022)"); };
	//virtual CString GetFunctionDescription() override final;
	//virtual double CalcSaEveryZone(stETC_GDS_TAIWAN02_CALC& sCalc, double dTn, double dT1, double dSRa, double dSRv, BOOL bPOcurve) override final;
	virtual double GetInitFundamentalPeriod() override final;

	//virtual bool CalcGeneralZone(stETC_GDS_TAIWAN02_CALC& sCalc) override final;
	//virtual bool CalcNearFaultZone(stETC_GDS_TAIWAN02_CALC& sCalc) override final;
	//virtual bool CalcTaipeiBasin(stETC_GDS_TAIWAN02_CALC& sCalc) override final;
	//virtual void CalcSiteMagnifyFactor() override final;
	virtual void GetSiteMagnifyFactor() override final;
};

#pragma once
#include "SpfcBaseUtil.h"

class CSpfcDPT2018Util : public CSpfcBaseUtil
{
public:
	CSpfcDPT2018Util();

	bool CheckValid() override;
	void MakeSpectrumData(bool bOnlyCalc = false) override;
	void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) override;
	void SetParamFromCode(const T_SPFC_CODE& codeParam) override;

private:
	CString GetDescription(const T_SPFC_DPT2018& cData);

private:
	T_SPFC_DPT2018 m_Data;
};

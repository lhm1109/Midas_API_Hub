#pragma once

class CSpfcBaseUtil;

class CSpfcMakeSpectrumUtil
{
public:
	bool MakeSpectrumData(const T_SPFC_CODE& codeParam, int nSpecCodeType, CArray<double, double>& arPeriod, CArray<double, double>& arAccel) const;

private:
	std::unique_ptr<CSpfcBaseUtil> GetSpfcUtilFromCode(int nSpecCodeType) const;
	void SetSpfcResult(const CSpfcBaseUtil& spfcUtil, CArray<double, double>& arPeriod, CArray<double, double>& arAccel, CString& strDescription, CString& strFuncName) const;
};

#pragma once

namespace SpfcUtil
{
	constexpr int BUFFER_SIZE = 128;
}

class CSpfcBaseUtil
{
public:
	CSpfcBaseUtil();
	virtual ~CSpfcBaseUtil() = default;

	virtual bool CheckValid() = 0;
	virtual void MakeSpectrumData(bool bOnlyCalc = false) = 0;
	virtual void MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel) = 0;// for Pushover Curve
	virtual void SetParamFromCode(const T_SPFC_CODE& codeParam) = 0;
	virtual void SetParamFromCalc() {}

public:
	bool IsValid(const double& dValue, const double& dMin, const double& dMax = DBL_MAX);

	void InitPeriod();
	void InitAccel();

	std::wstring GetFuncName() const;
	std::wstring GetDescript() const;
	std::vector<double> GetPeriod() const;
	std::vector<double> GetAccel() const;
	int GetStepCoeff() const;
	int GetPeriodCoeff() const;

	void SetFuncName(const std::wstring& sFuncName);
	void SetDescript(const std::wstring& sDescript);
	void SetStepCoeff(int nStepCoeff);
	void SetPeriodCoeff(int nPeriodCoeff);

	void AddPeriod(double period);
	void AddAccel(double accel);

private:
	std::wstring m_strFuncName;		// TODO - change name;
	std::wstring m_sDescript;

	std::vector<double> m_vPeriod;
	std::vector<double> m_vAccel;

	int m_nStepCoeff;
	int m_nPeriodCoeff;

};

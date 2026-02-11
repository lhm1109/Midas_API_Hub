#include "stdafx.h"
#include "SpfcBaseUtil.h"
#include "../wg_db/DBDoc.h"

CSpfcBaseUtil::CSpfcBaseUtil()
{
	m_nStepCoeff = 4;
	m_nPeriodCoeff = 2;
}

bool CSpfcBaseUtil::IsValid(const double& dValue, const double& dMin, const double& dMax)
{
	if (dValue < dMin)
	{
		const auto pDoc = CDBDoc::GetDocPoint();
		pDoc->DisplayHistoryMessage(_LS(IDS_WG_CMD__ADDD__the_Value_must_be_greater_than__4), dMin);
		return false;
	}

	if (dValue > dMax)
	{
		const auto pDoc = CDBDoc::GetDocPoint();
		pDoc->DisplayHistoryMessage(_LS(IDS_WG_CMD__ADDD__the_Value_must_be_less_than__4_1f), dMin);
		return false;
	}

	return true;
}

void CSpfcBaseUtil::InitPeriod()
{
	m_vPeriod.clear();
}

void CSpfcBaseUtil::InitAccel()
{
	m_vAccel.clear();
}

std::wstring CSpfcBaseUtil::GetFuncName() const
{
	return m_strFuncName;
}

std::wstring CSpfcBaseUtil::GetDescript() const
{
	return m_sDescript;
}

std::vector<double> CSpfcBaseUtil::GetPeriod() const
{
	return m_vPeriod;
}

std::vector<double> CSpfcBaseUtil::GetAccel() const
{
	return m_vAccel;
}

int CSpfcBaseUtil::GetStepCoeff() const
{
	return m_nStepCoeff;
}

int CSpfcBaseUtil::GetPeriodCoeff() const
{
	return m_nPeriodCoeff;
}

void CSpfcBaseUtil::SetFuncName(const std::wstring& sFuncName)
{
	m_strFuncName = sFuncName;
}

void CSpfcBaseUtil::SetDescript(const std::wstring& sDescript)
{
	m_sDescript = sDescript;
}

void CSpfcBaseUtil::SetStepCoeff(int nStepCoeff)
{
	m_nStepCoeff = nStepCoeff;
}

void CSpfcBaseUtil::SetPeriodCoeff(int nPeriodCoeff)
{
	m_nPeriodCoeff = nPeriodCoeff;
}

void CSpfcBaseUtil::AddPeriod(double period)
{
	m_vPeriod.emplace_back(period);
}

void CSpfcBaseUtil::AddAccel(double accel)
{
	m_vAccel.emplace_back(accel);
}
#include "stdafx.h"
#include "SpfcSP14_2018Util.h"
#include "../wg_db/UnitCtrl.h"

CSpfcSP14_2018Util::CSpfcSP14_2018Util()
{
	T_SPFC_RUS_SP14_2018 DefaultD;
	GetDefault(DefaultD);

	m_nSoil = DefaultD.nSoil;
	m_dK0   = DefaultD.dK0;
	m_dK1   = DefaultD.dK1;
	m_dKPsi = DefaultD.dKPsi;
	m_dAcce = DefaultD.dAcce;
	m_bNLDS = DefaultD.bNLDS;
	m_dMaxT = DefaultD.dMaxPeriod;
}

void CSpfcSP14_2018Util::GetDefault(T_SPFC_RUS_SP14_2018& rData)
{
	rData.Initialize();
	rData.nSoil = 0;
	rData.dK0   = 1.00;
	rData.dK1   = 0.12;
	rData.dKPsi = 1.30;
	rData.dAcce = M_InitValueCurUnit(1.0, TON, M, CUnitCtrl::m_SPFC_UNIT.Gravity);
	rData.bNLDS = true;
	rData.dMaxPeriod = PRD_ED;
}

bool CSpfcSP14_2018Util::CheckData(const T_SPFC_RUS_SP14_2018& cData)
{
	if ( dgn::LE0(cData.dK0) )
	{
		AfxMessageBox(nsGetStr(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("K0")));
		return false;
	}
	if ( dgn::LE0(cData.dK1) )
	{
		AfxMessageBox(nsGetStr(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("K1")));
		return false;
	}
	if ( dgn::LE0(cData.dKPsi) )
	{
		AfxMessageBox(nsGetStr(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("KPsi")));
		return false;
	}
	if ( dgn::LE0(cData.dAcce) )
	{
		AfxMessageBox(nsGetStr(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Accel.")));
		return false;
	}
	if ( dgn::LE0(cData.dMaxPeriod) )
	{
		AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0));
		return false;
	}
	return true;
}

void CSpfcSP14_2018Util::GetData(T_SPFC_RUS_SP14_2018& rData)
{
	rData.Initialize();
	rData.dK0        = m_dK0;
	rData.dK1        = m_dK1;
	rData.dKPsi      = m_dKPsi;
	rData.dAcce      = m_dAcce;
	rData.dMaxPeriod = m_dMaxT;
}

bool CSpfcSP14_2018Util::CheckValid()
{
	T_SPFC_RUS_SP14_2018 cData;
	GetData(cData);
	return CheckData(cData);
}

void CSpfcSP14_2018Util::MakeSpectrumData(bool bOnlyCalc)
{
	double dTsta = PRD_ST;
	double dTend = m_dMaxT;
	int nStep = PRD_NUM;
	if ( bOnlyCalc )
	{
		dTend *= GetPeriodCoeff();
		nStep *= GetStepCoeff();
	}

	const SPSoil enSoil = static_cast< SPSoil >( m_nSoil );
	double dT0 = 0.0;
	double dT1 = 0.0;
	GetSpecificT(enSoil, dT0, dT1);

	const double dTinc = ( dTend - dTsta ) / static_cast< double >( nStep );
	const double dTol = dTinc / nStep;

	InitPeriod();
	InitAccel();

	for ( int i = 0; i <= nStep; i++ )
	{
		const double dTn = dTsta + dTinc * i;

		if ( IsTransientT(dTn, dT0, dTinc, dTol) )
		{
			const double dSa = CalcSa(enSoil, dT0);
			AddPeriod(dT0);
			AddAccel(dSa);
		}

		if ( IsTransientT(dTn, dT1, dTinc, dTol) )
		{
			const double dSa = CalcSa(enSoil, dT1);
			AddPeriod(dT1);
			AddAccel(dSa);
		}

		if ( true )
		{
			const double dSa = CalcSa(enSoil, dTn);
			AddPeriod(dTn);
			AddAccel(dSa);
		}
	}

	if ( !bOnlyCalc )
	{
		SetSpecInfo();
	}
}

double CSpfcSP14_2018Util::CalcSa(SPSoil enSoil, double dT)
{
	const double dg    = Getg();
	const double dBeta = CalcBeta(enSoil, dT);
	const double dNLDS = m_bNLDS ? 0.70 : 1.00;
	return dgn::SafeDiv(m_dK0 * m_dK1 * m_dKPsi * m_dAcce * dBeta * dNLDS, dg);
}

double CSpfcSP14_2018Util::CalcBeta(SPSoil enSoil, double dT)
{
	double dT0 = 0.0, dT1 = 0.0;
	GetSpecificT(enSoil, dT0, dT1);

	switch ( enSoil )
	{
	case SPSoil::enS1:
	case SPSoil::enS2:
		{
			if ( dgn::LE(dT, dT0) )
			{
				return 1.0 + 15.0 * dT;
			}
			if ( dgn::LE(dT, dT1) )
			{
				return 2.5;
			}
			return 2.5 * pow(0.4 / dT, 0.5);
		}
		break;
	case SPSoil::enS3:
	case SPSoil::enS4:
		{
			if ( dgn::LE(dT, dT0) )
			{
				return 1.0 + 15.0 * dT;
			}
			if ( dgn::LE(dT, dT1) )
			{
				return 2.5;
			}
			return 2.5 * pow(0.8 / dT, 0.5);
		}
		break;
	}
	ASSERT(0); return 0.0;
}

double CSpfcSP14_2018Util::Getg()
{
	return M_InitValueCurUnit(9.806, TON, M, CUnitCtrl::m_SPFC_UNIT.Gravity);
}

void CSpfcSP14_2018Util::SetSpecInfo()
{
	T_SPFC_RUS_SP14_2018 cData;
	GetData(cData);
	const CString csName = GetFuncName();
	const CString csDesc = GetFuncDesc(cData);
	SetDescript(static_cast< std::wstring >( csDesc ));
	SetFuncName(static_cast< std::wstring >( csName ));
}

bool CSpfcSP14_2018Util::GetSoilData(std::vector<CString>& rvSoil)
{
	rvSoil.clear();
	rvSoil.push_back(_T("I"));
	rvSoil.push_back(_T("II"));
	rvSoil.push_back(_T("III"));
	rvSoil.push_back(_T("IV"));
	return true;
}

CString CSpfcSP14_2018Util::GetSoilInfo(int nSoil)
{
	std::vector<CString> vSoil;
	if ( !GetSoilData(vSoil) ) { ASSERT(0); return CString(); }

	if ( nSoil < vSoil.size() )
	{
		return vSoil[nSoil];
	}
	ASSERT(0); return CString();
}

CString CSpfcSP14_2018Util::GetFuncName()
{
	return SPFC_RUS_SP_14_13330;
}

CString CSpfcSP14_2018Util::GetFuncDesc(const T_SPFC_RUS_SP14_2018& cData)
{
	const CString csSoil = GetSoilInfo(cData.nSoil);
	return nsGetStr(_T("Soil=%s, K0=%3.2f, K1=%3.2f, KΨ=%3.2f, Acce=%3.2f"),
		csSoil, cData.dK0, cData.dK1, cData.dKPsi, cData.dAcce);
}

void CSpfcSP14_2018Util::MakePOSpectrumData(double dDamping, double SRa, double SRv,
	CArray<double, double>& radPeriod, CArray<double, double>& radAccel)
{
	radPeriod.RemoveAll();
	radAccel.RemoveAll();
	ASSERT(0);
}

void CSpfcSP14_2018Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nSoil = codeParam.SP14_2018.nSoil;
	m_dK0   = codeParam.SP14_2018.dK0;
	m_dK1   = codeParam.SP14_2018.dK1;
	m_dKPsi = codeParam.SP14_2018.dKPsi;
	m_dAcce = codeParam.SP14_2018.dAcce;
	m_bNLDS = codeParam.SP14_2018.bNLDS;
	m_dMaxT = codeParam.SP14_2018.dMaxPeriod;
}

bool CSpfcSP14_2018Util::IsTransientT(double dT, double dTs, double dTinc, double dTol)
{
	if ( dgn::LE(dT, dTs, dTol) ) return false;
	if ( dgn::ME(dT, dTs + dTinc, dTol) ) return false;
	return true;
}

void CSpfcSP14_2018Util::GetSpecificT(SPSoil enSoil, double& rdT0, double& rdT1)
{
	rdT0 = 0.0;
	rdT1 = 0.0;

	switch ( enSoil )
	{
	case SPSoil::enS1:
	case SPSoil::enS2:
		{
			rdT0 = 0.1;
			rdT1 = 0.4;
		}
		break;
	case SPSoil::enS3:
	case SPSoil::enS4:
		{
			rdT0 = 0.1;
			rdT1 = 0.8;
		}
		break;
	}
}
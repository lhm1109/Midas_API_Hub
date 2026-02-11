#include "stdafx.h"
#include "SpfcDPT2018Util.h"
#include "../wg_db/LateralSeisDPT.h"

CSpfcDPT2018Util::CSpfcDPT2018Util()
{
    CLateralSeisDPT seisDPT;
    auto strFuncName = seisDPT.GetName();
    SetFuncName(std::wstring(strFuncName));
    m_Data.Init();

    m_Data.dFa = 1.38;
    m_Data.dFv = 1.38;
    m_Data.dSds = 0.506;
    m_Data.dSd1 = 0.2024;
    m_Data.dResModFactor = 4.0;
    m_Data.dImportFactor = 1.2;
}

bool CSpfcDPT2018Util::CheckValid()
{
    const double dZero = 1.0e-7;
    if (m_Data.unRegion == EN_DPT_SEIS_REGION_NOT_BANGKOK)
    {
        if (m_Data.dFa < dZero)
        {
            AfxMessageBox(nsGetStr(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Fa")));
            return FALSE;
        }

        if (m_Data.dFv < dZero)
        {
            AfxMessageBox(nsGetStr(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Fv")));
            return FALSE;
        }

        if (m_Data.dSds < dZero)
        {
            AfxMessageBox(nsGetStr(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Sds")));
            return FALSE;
        }

        if (m_Data.dSd1 < dZero)
        {
            AfxMessageBox(nsGetStr(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Sd1")));
            return FALSE;
        }
    }

    if (m_Data.dImportFactor < dZero)
    {
        AfxMessageBox(nsGetStr(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("I")));
        return FALSE;
    }

    if (m_Data.dResModFactor < dZero)
    {
        AfxMessageBox(nsGetStr(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("R")));
        return FALSE;
    }

    if (m_Data.dMaxPeriod < dZero)
    {
        AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0));
        return FALSE;
    }

    return TRUE;
}

void CSpfcDPT2018Util::MakeSpectrumData(bool bOnlyCalc)
{
    CLateralSeisDPT seisDPT;
    MArray<FVal> aSpecRS;
    if (!seisDPT.MakeRSFunction(m_Data, aSpecRS)) { ASSERT(0); return; }

    int nSpecRS = aSpecRS.GetSize();
    if (nSpecRS < 1) { ASSERT(0); return; }

	InitPeriod();
	InitAccel();

    for (int i = 0; i < nSpecRS; ++i)
    {
        const FVal& SpecRS = aSpecRS[i];
        AddPeriod(SpecRS.dVar);
        AddAccel(SpecRS.dValue);
    }

    auto sDescript = GetDescription(m_Data);
    SetDescript(std::wstring(sDescript));
}

void CSpfcDPT2018Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();
	ASSERT(false);
}

void CSpfcDPT2018Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
    m_Data = codeParam.DPT2018;
}

CString CSpfcDPT2018Util::GetDescription(const T_SPFC_DPT2018& m_Data)
{
    CLateralSeisDPT seisDPT;
    return seisDPT.GetDescription(m_Data);
}
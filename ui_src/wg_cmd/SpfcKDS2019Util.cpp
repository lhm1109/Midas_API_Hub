#include "stdafx.h"
#include "SpfcKDS2019Util.h"

CSpfcKDS2019Util::CSpfcKDS2019Util()
{
    m_dFa = 1.38;
    m_dFv = 1.38;
    m_dSds = 0.506;
    m_dSd1 = 0.2024;
    m_dTs = m_dSd1 / m_dSds;
    m_dR = 4.0;
    m_dI = 1.2;
    m_dMaxPeriod = PRD_ED;

	m_nZone = 0;
	m_nSite = 0;
	m_dZoneFactor = 0.14;
	m_dT0 = 0.2 * m_dTs;
}

bool CSpfcKDS2019Util::CheckValid()
{
    CString strMsg;
    if (m_dFa <= 0.)
    {
        strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Fa"));
        AfxMessageBox(strMsg);
        return false;
    }
    if (m_dFv <= 0.)
    {
        strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Fv"));
        AfxMessageBox(strMsg);
        return false;
    }
    if (m_dSds <= 0.)
    {
        strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Sds"));
        AfxMessageBox(strMsg);
        return false;
    }
    if (m_dSd1 <= 0.)
    {
        strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Sd1"));
        AfxMessageBox(strMsg);
        return false;
    }
    if (m_dI <= 0.)
    {
        strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Ie"));
        AfxMessageBox(strMsg);
        return false;
    }
    if (m_dR <= 0.)
    {
        strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("R"));
        AfxMessageBox(strMsg);
        return false;
    }

    if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }
    return true;
}

void CSpfcKDS2019Util::MakeSpectrumData(bool bOnlyCalc)
{
    double st = PRD_ST;
    double ed = m_dMaxPeriod;
    int Step = PRD_NUM;
    // Pushover FEMA440에서 계산할 때(Period 2배, Step 4배)
    if (bOnlyCalc)
    {
        m_dT0 = 0.2 * m_dTs;
        ed *= GetPeriodCoeff();
        Step *= GetStepCoeff();
    }
    double Tn = 0.0;
    double dSa = 0.0;

    double dt = (ed - st) / Step;  // time interval	
    double dTol = dt / Step;  // Tolerance

	InitPeriod();
	InitAccel();

    for (int i = 0; i <= Step; i++)
    {
        Tn = st + dt * i;

        if (Tn >= 0 && Tn <= m_dT0)
        {
            dSa = (0.6 * m_dSds / m_dT0 * Tn + 0.4 * m_dSds) * m_dI / m_dR;
        }
        else if (Tn > m_dT0 && Tn <= m_dTs)
        {
            dSa = m_dSds * m_dI / m_dR;
        }
        else if (Tn > m_dTs && Tn <= 5.0)
        {
            dSa = m_dSd1 / Tn * m_dI / m_dR;
        }
        else if (Tn > 5.0)
        {
            dSa = m_dSd1 * 5.0 / Tn / Tn * m_dI / m_dR;
        }

        if (Tn > m_dT0 + dTol && Tn < m_dT0 + dt - dTol) // transition point		
        {
            AddPeriod(m_dT0);
            AddAccel(m_dSds * m_dI / m_dR);
        }
        if (Tn > m_dTs + dTol && Tn < m_dTs + dt - dTol) // transition point		
        {
            AddPeriod(m_dTs);
            AddAccel(m_dSds * m_dI / m_dR);
        }

        AddPeriod(Tn);
        AddAccel(dSa);
    }

    if (bOnlyCalc) return;
    CString strSite[] = { _T("S1"),_T("S2"),_T("S3"),_T("S4"),_T("S5"),_T("S6") };

    // Zone 은 0 = 1, 1 = 2
    // 수정하려면 CString strSite[]={_T("1"),_T("2")}; 이거 추가 한 후에 Msg 수정
    wchar_t buffer[SpfcUtil::BUFFER_SIZE];
    int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD_KDS2019_PROFILE), m_nZone + 1, m_dZoneFactor, strSite[m_nSite], m_dFa, m_dFv, m_dSds, m_dSd1, m_dI, m_dR);

    if (result >= 0)
    {
        SetDescript(buffer);
    }

    SetFuncName(L"KDS(41-17-00:2019)");
}

void CSpfcKDS2019Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
    aPeriod.RemoveAll();
    aAccel.RemoveAll();

    double st = PRD_ST;
    double ed = m_dMaxPeriod * 2;
    int Step = PRD_NUM * 4;
    double Tn = 0.0;
    double dSa = 0.0;

    if (dDamping >= 0)
    {
        SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
        SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
    }

    double dt = (ed - st) / Step;  // time interval	
    double dTol = dt / Step;  // Tolerance

    double dTs = m_dTs * SRv / SRa;
    double dT0 = 0.2 * dTs;
    double dTl = 5.0 * SRv / SRa; /// 5 (sec) 주기를 scale 처리.

    for (int i = 0; i <= Step; i++)
    {
        Tn = st + dt * i;

        if (Tn >= 0 && Tn <= dT0)
        {
            dSa = (0.4 + (SRa - 0.4) * Tn / dT0) * m_dSds * m_dI / m_dR;
        }
        else if (Tn > dT0 && Tn <= dTs)
        {
            dSa = m_dSds * m_dI / m_dR * SRa;
        }
        else if (Tn > dTs && Tn <= dTl)
        {
            dSa = min((m_dSd1 / Tn) * (m_dI / m_dR) * SRv, (m_dSds * m_dI / m_dR) * SRa);
        }
        else if (Tn > dTl)
        {
            dSa = min(m_dSd1 * dTl / Tn / Tn * m_dI / m_dR * SRv, (m_dSds * m_dI / m_dR) * SRa);
        }

        if (Tn > dT0 + dTol && Tn < dT0 + dt - dTol) // transition point		
        {
            aPeriod.Add(dT0);
            aAccel.Add((m_dSds * m_dI / m_dR) * SRa);
        }
        if (Tn > dTs + dTol && Tn < dTs + dt - dTol) // transition point		
        {
            aPeriod.Add(dTs);
            aAccel.Add((m_dSds * m_dI / m_dR) * SRa);
        }
        if (Tn > dTl + dTol && Tn < dTl + dt - dTol) // transition point		
        {
            aPeriod.Add(dTl);
            aAccel.Add(min((m_dSd1 / Tn) * (m_dI / m_dR) * SRv, (m_dSds * m_dI / m_dR) * SRa));
        }

        aPeriod.Add(Tn);
        aAccel.Add(dSa);
    }
}

void CSpfcKDS2019Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
    m_nZone = codeParam.KDS2019.nSeisZone;
    m_nSite = codeParam.KDS2019.nSiteClass;
    m_dZoneFactor = codeParam.KDS2019.dZoneFactor;
    m_dFa = codeParam.KDS2019.dFa;
    m_dFv = codeParam.KDS2019.dFv;
    m_dSds = codeParam.KDS2019.dSds;
    m_dSd1 = codeParam.KDS2019.dSd1;
    m_dI = codeParam.KDS2019.dIe;
    m_dR = codeParam.KDS2019.dCoef;
    m_dMaxPeriod = codeParam.KDS2019.dMaxPeriod;
}

void CSpfcKDS2019Util::SetParamFromCalc()
{
    m_dTs = m_dSd1 / m_dSds;
    m_dT0 = 0.2 * m_dTs;
}

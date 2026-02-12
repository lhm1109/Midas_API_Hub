#include "stdafx.h"
#include "wg_cmd.h"
#include "LoadCombCtrl.h"

#include "..\wg_base\wg_base_AppBase.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_CompFunc.h"

#include <fstream>       // for ofstream

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#if !defined D_STR
#define D_STR 0
#endif

#if !defined D_SER
#define D_SER 1
#endif

#if !defined D_LOAD_LEVEL_STRN
#define D_LOAD_LEVEL_STRN 0
#endif

#if !defined D_LOAD_LEVEL_SERV
#define D_LOAD_LEVEL_SERV 1
#endif

void CLoadCombCtrl::Make_SubComb_EQ01_KBC2016(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, 
                                              CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
	CString strDesc = _T(""); 
	/// see KBC2015, 0301.5(1). (0301.5.1) = 1.4(D+F) -> [PMS 5279] 토압고려 하중조합 개선.
	strDesc.Format(_T("1.4(DL+EV+FP) + (SFAC)%s"), strPsTs); 
	Set_LCB(nActive, strDesc, 1.4,cDL, 1.4,cCSD, 1.4,cEPV, 1.4,cFP, dPSS,cPsTs);
}

void CLoadCombCtrl::Make_SubComb_EQ02_KBC2016(int nActive,	CombBase* cDL, CombBase* cCSD, CombBase* cEPV,
                                              CombBase* cFP, CombBase* cTL, CombBase* cLL, CombBase* cCSL, CombBase* cIL, 
                                              CombBase* cLR, CombBase* cSL, CombBase* cRL,  CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
	CString strDesc = _T("");
	/// see KBC2015, 0301.5(1). (0301.5.2) = 1.2(D+F+T)+1.6L+0.5(Lr or S or R).	-> [PMS 5279] 토압고려 하중조합 개선.
    if ( !cLR->IsExist() && !cSL->IsExist() && !cRL->IsExist() )
    {
        strDesc.Format(_T("1.2(DL+EV+FP+TL) + 1.6(LL+IL) + (SFAC)%s"), strPsTs);
        Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.2, cFP, 1.2, cTL, 1.6, cLL, 1.6, cCSL, 1.6, cIL, dPSS, cPsTs);
    }
    else
    {
        CString strCaseLR[3] = { _T("LR"), _T("SL"), _T("RL") };
        CombBase* cLoadLR[3] = { cLR, cSL, cRL };

        for ( int i=0; i<3; ++i )
        {
            strDesc.Format(_T("1.2(DL+EV+FP+TL) + 1.6(LL+IL) + 0.5%s+ (SFAC)%s"), strCaseLR[i], strPsTs);
            Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.2, cFP, 1.2, cTL, 1.6, cLL, 1.6, cCSL, 1.6, cIL, 0.5, cLoadLR[i], dPSS, cPsTs);
        }
    }
}

void CLoadCombCtrl::Make_SubComb_EQ03_KBC2016(int nActive, BOOL bExistLL, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, 
                                              CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
                                              CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cWL, CombBase* cPsTs,
                                              const CString& strPsTs, double dPSS)
{
	CString strDesc = _T("");
	/// see KBC2015, 0301.5(1). (0301.5.3) = 1.2D+1.6(Lr or S or R)+(1.0L or 0.65W) -> [PMS 5279] 토압고려 하중조합 개선.
			
	CString strCaseLR[3] = { _T("LR"), _T("SL"), _T("RL") };
	CombBase* cLoadLR[3] = { cLR, cSL, cRL };

	if (!bExistLL && !cWL->IsExist()) //1.2D + 1.6(LR or S or R).
	{
		for(int i=0; i<3; ++i)
		{
			strDesc.Format(_T("1.2(DL+EV+FP) + 1.6%s + (SFAC)%s"), strCaseLR[i], strPsTs); 
			Set_LCB(nActive, strDesc, 1.2,cDL, 1.2,cCSD, 1.2,cEPV, 1.2,cFP, 1.6,cLoadLR[i], dPSS,cPsTs);
		}
	}
	else
	{
		for(int i=0; i<3; ++i)
		{
			strDesc.Format(_T("1.2(DL+EV+FP) + 1.6%s + 1.0(LL+IL) + (SFAC)%s"), strCaseLR[i], strPsTs);
			Set_LCB(nActive, strDesc, 1.2,cDL, 1.2,cCSD, 1.2,cEPV, 1.2,cFP, 1.6,cLoadLR[i], 1.0,cLL, 1.0,cCSL, 1.0,cIL, dPSS,cPsTs);
						
			strDesc.Format(_T("1.2(DL+EV+FP) + 1.6%s + 0.65WL + (SFAC)%s"), strCaseLR[i], strPsTs);
			Set_LCB(nActive, strDesc, 1.2,cDL, 1.2,cCSD, 1.2,cEPV, 1.2,cFP, 1.6,cLoadLR[i], 0.65,cWL, dPSS,cPsTs);
			strDesc.Format(_T("1.2(DL+EV+FP) + 1.6%s - 0.65WL + (SFAC)%s"), strCaseLR[i], strPsTs);
			Set_LCB(nActive, strDesc, 1.2,cDL, 1.2,cCSD, 1.2,cEPV, 1.2,cFP, 1.6,cLoadLR[i],-0.65,cWL, dPSS,cPsTs);
		}
	}
}

void CLoadCombCtrl::Make_SubComb_EQ04_KBC2016(int nActive, BOOL bExistLL, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, 
                                              CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
                                              CombBase* cLR, CombBase* cSL, CombBase* cRL,  CombBase* cWL, CombBase* cPsTs,
                                              const CString& strPsTs, double dPSS)
{
	CString strDesc = _T("");
	/// see KBC2015, 0301.5(1). (0301.5.4) = 1.2D+1.3W+1.0L+0.5(Lr 또는 S 또는 R) -> [PMS 5279] 토압고려 하중조합 개선.
	
	BOOL bExistLRSeries = !cLR->IsExist() && !cSL->IsExist() && !cRL->IsExist()? FALSE : TRUE;
	if (bExistLRSeries)
	{
		CString strCaseLR[3] = { _T("LR"), _T("SL"), _T("RL") };
		CombBase* cLoadLR[3] = { cLR, cSL, cRL };

		if (bExistLL)
		{
			for (int i=0; i<3; ++i)
			{
				strDesc.Format(_T("1.2(DL+EV+FP) + 1.3WL + 1.0(LL+IL) + 0.5%s + (SFAC)%s"), strCaseLR[i], strPsTs); 
				Set_LCB(nActive, strDesc, 1.2,cDL, 1.2,cCSD, 1.2,cEPV, 1.2,cFP, 1.3,cWL, 1.0,cLL, 1.0,cCSL, 1.0,cIL, 0.5,cLoadLR[i], dPSS,cPsTs);

				strDesc.Format(_T("1.2(DL+EV+FP) - 1.3WL + 1.0(LL+IL) + 0.5%s + (SFAC)%s"), strCaseLR[i], strPsTs); 
				Set_LCB(nActive, strDesc, 1.2,cDL, 1.2,cCSD, 1.2,cEPV, 1.2,cFP,-1.3,cWL, 1.0,cLL, 1.0,cCSL, 1.0,cIL, 0.5,cLoadLR[i], dPSS,cPsTs);
			}
		}
		else
		{
			for (int i=0; i<3; ++i)
			{
				strDesc.Format(_T("1.2(DL+EV+FP) + 1.3WL + 0.5%s + (SFAC)%s"), strCaseLR[i], strPsTs);
				Set_LCB(nActive, strDesc, 1.2,cDL, 1.2,cCSD, 1.2,cEPV, 1.2,cFP, 1.3,cWL, 0.5,cLoadLR[i], dPSS,cPsTs);

				strDesc.Format(_T("1.2(DL+EV+FP) - 1.3WL + 0.5%s + (SFAC)%s"), strCaseLR[i], strPsTs);
				Set_LCB(nActive, strDesc, 1.2,cDL, 1.2,cCSD, 1.2,cEPV, 1.2,cFP,-1.3,cWL, 0.5,cLoadLR[i], dPSS,cPsTs);
			}
		}
	}
	else
	{
		if (bExistLL)
		{
			strDesc.Format(_T("1.2(DL+EV+FP) + 1.3WL + 1.0(LL+IL) + (SFAC)%s"), strPsTs);
			Set_LCB(nActive, strDesc, 1.2,cDL, 1.2,cCSD, 1.2,cEPV, 1.2,cFP, 1.3,cWL, 1.0,cLL, 1.0,cCSL, 1.0,cIL, dPSS,cPsTs);
			
			strDesc.Format(_T("1.2(DL+EV+FP) - 1.3WL + 1.0(LL+IL) + (SFAC)%s"), strPsTs);
			Set_LCB(nActive, strDesc, 1.2,cDL, 1.2,cCSD, 1.2,cEPV, 1.2,cFP,-1.3,cWL, 1.0,cLL, 1.0,cCSL, 1.0,cIL, dPSS,cPsTs);
		}
		else
		{
			strDesc.Format(_T("1.2(DL+EV+FP) + 1.3WL + (SFAC)%s"), strPsTs);
			Set_LCB(nActive, strDesc, 1.2,cDL, 1.2,cCSD, 1.2,cEPV, 1.2,cFP, 1.3,cWL, dPSS,cPsTs);

			strDesc.Format(_T("1.2(DL+EV+FP) - 1.3WL + (SFAC)%s"), strPsTs);
			Set_LCB(nActive, strDesc, 1.2,cDL, 1.2,cCSD, 1.2,cEPV, 1.2,cFP,-1.3,cWL, dPSS,cPsTs);
		}    
	}
}

void CLoadCombCtrl::Make_SubComb_EQ05_KBC2016(int nActive, BOOL bExistLL, BOOL bSlcmSpecial, CombBase* cDL, CombBase* cCSD, 
                                              CombBase* cEPV, CombBase* cFP, CombBase* cLL, CombBase* cCSL, 
                                              CombBase* cIL, CombBase* cSL, CombBase* cEL, CombBase* cESP, CombBase* cPsTs, 
                                              const CString& strPsTs, double dPSS)
{
	CString strDesc = _T("");
	/// see KBC2015, 0301.5(1). (0301.5.5) = 1.2D+1.0E+1.0L+0.2S -> [PMS 5279] 토압고려 하중조합 개선.
	
	double dDLp12 = 1.2;	
	CString strAddDLp = _T("");	
	CString strAddEL  = m_bModeUG ? _T("(SUF)") : _T("");
	CString strAddESP = _T("(SUF)");

	if (bSlcmSpecial)
	{
		dDLp12 = 1.2 + m_dSlcmSds * m_dSlcmSpecial; // for 1.2D + [(OMG)E + 0.2SDS*D]		
		strAddDLp = cEPV->IsExist() ? _T(" + (SPC)(SDS)(DL+EV)") : _T(" + (SPC)(SDS)DL");		
		strAddEL  = m_bModeUG ? _T("(SUF)(OMG)") : _T("(OMG)");
		strAddESP = _T("(SUF)(OMG)");
	}

    if ( cSL->IsExist() )
    {
        if ( bExistLL )
        {
            strDesc.Format(_T("1.2(DL+EV+FP) + 1.0%sEL + 1.0(LL+IL) + 0.2SL + (SFAC)%s%s"), strAddEL, strPsTs, strAddDLp);   Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDLp12, cEPV, 1.2, cFP, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 1.0, cIL, 0.2, cSL, dPSS, cPsTs);
            strDesc.Format(_T("1.2(DL+EV+FP) - 1.0%sEL + 1.0(LL+IL) + 0.2SL + (SFAC)%s%s"), strAddEL, strPsTs, strAddDLp);   Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDLp12, cEPV, 1.2, cFP,-1.0, cEL, 1.0, cLL, 1.0, cCSL, 1.0, cIL, 0.2, cSL, dPSS, cPsTs);

            strDesc.Format(_T("1.2(DL+EV+FP) + 1.0%sESP + 1.0(LL+IL) + 0.2SL + (SFAC)%s%s"), strAddESP, strPsTs, strAddDLp); Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDLp12, cEPV, 1.2, cFP, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 1.0, cIL, 0.2, cSL, dPSS, cPsTs);
            strDesc.Format(_T("1.2(DL+EV+FP) - 1.0%sESP + 1.0(LL+IL) + 0.2SL + (SFAC)%s%s"), strAddESP, strPsTs, strAddDLp); Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDLp12, cEPV, 1.2, cFP,-1.0, cESP, 1.0, cLL, 1.0, cCSL, 1.0, cIL, 0.2, cSL, dPSS, cPsTs);
        }
        else
        {
            strDesc.Format(_T("1.2(DL+EV+FP) + 1.0%sEL + 0.2SL + (SFAC)%s%s"), strAddEL, strPsTs, strAddDLp);   Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDLp12, cEPV, 1.2, cFP, 1.0, cEL, 0.2, cSL, dPSS, cPsTs);
            strDesc.Format(_T("1.2(DL+EV+FP) - 1.0%sEL + 0.2SL + (SFAC)%s%s"), strAddEL, strPsTs, strAddDLp);   Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDLp12, cEPV, 1.2, cFP,-1.0, cEL, 0.2, cSL, dPSS, cPsTs);

            strDesc.Format(_T("1.2(DL+EV+FP) + 1.0%sESP + 0.2SL + (SFAC)%s%s"), strAddESP, strPsTs, strAddDLp); Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDLp12, cEPV, 1.2, cFP, 1.0, cESP, 0.2, cSL, dPSS, cPsTs);
            strDesc.Format(_T("1.2(DL+EV+FP) - 1.0%sESP + 0.2SL + (SFAC)%s%s"), strAddESP, strPsTs, strAddDLp); Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDLp12, cEPV, 1.2, cFP,-1.0, cESP, 0.2, cSL, dPSS, cPsTs);
        }
    }
    else
    {
        if ( bExistLL )
        {
            strDesc.Format(_T("1.2(DL+EV+FP) + 1.0%sEL + 1.0(LL+IL) + (SFAC)%s%s"), strAddEL, strPsTs, strAddDLp);   Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDLp12, cEPV, 1.2, cFP, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 1.0, cIL, dPSS, cPsTs);
            strDesc.Format(_T("1.2(DL+EV+FP) - 1.0%sEL + 1.0(LL+IL) + (SFAC)%s%s"), strAddEL, strPsTs, strAddDLp);   Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDLp12, cEPV, 1.2, cFP,-1.0, cEL, 1.0, cLL, 1.0, cCSL, 1.0, cIL, dPSS, cPsTs);

            strDesc.Format(_T("1.2(DL+EV+FP) + 1.0%sESP + 1.0(LL+IL) + (SFAC)%s%s"), strAddESP, strPsTs, strAddDLp); Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDLp12, cEPV, 1.2, cFP, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 1.0, cIL, dPSS, cPsTs);
            strDesc.Format(_T("1.2(DL+EV+FP) - 1.0%sESP + 1.0(LL+IL) + (SFAC)%s%s"), strAddESP, strPsTs, strAddDLp); Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDLp12, cEPV, 1.2, cFP,-1.0, cESP, 1.0, cLL, 1.0, cCSL, 1.0, cIL, dPSS, cPsTs);
        }
        else
        {
            strDesc.Format(_T("1.2(DL+EV+FP) + 1.0%sEL + (SFAC)%s%s"), strAddEL, strPsTs, strAddDLp);   Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDLp12, cEPV, 1.2, cFP, 1.0, cEL, dPSS, cPsTs);
            strDesc.Format(_T("1.2(DL+EV+FP) - 1.0%sEL + (SFAC)%s%s"), strAddEL, strPsTs, strAddDLp);   Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDLp12, cEPV, 1.2, cFP,-1.0, cEL, dPSS, cPsTs);

            strDesc.Format(_T("1.2(DL+EV+FP) + 1.0%sESP + (SFAC)%s%s"), strAddESP, strPsTs, strAddDLp); Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDLp12, cEPV, 1.2, cFP, 1.0, cESP, dPSS, cPsTs);
            strDesc.Format(_T("1.2(DL+EV+FP) - 1.0%sESP + (SFAC)%s%s"), strAddESP, strPsTs, strAddDLp); Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDLp12, cEPV, 1.2, cFP,-1.0, cESP, dPSS, cPsTs);
        }
    }
}

void CLoadCombCtrl::Make_SubComb_EQ06_KBC2016(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV,
                                              CombBase* cWL, CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
	CString strDesc = _T("");
	/// see KBC2015, 0301.5(1). (0301.5.6) = 0.9D+1.3W -> [PMS 5279] 토압고려 하중조합 개선.
    strDesc.Format(_T("0.9(DL+EV) + 1.3WL + (SFAC)%s"), strPsTs); Set_LCB(nActive, strDesc, 0.9, cDL, 0.9, cCSD, 0.9, cEPV, 1.3, cWL, dPSS, cPsTs);
    strDesc.Format(_T("0.9(DL+EV) - 1.3WL + (SFAC)%s"), strPsTs); Set_LCB(nActive, strDesc, 0.9, cDL, 0.9, cCSD, 0.9, cEPV, -1.3, cWL, dPSS, cPsTs);
}

void CLoadCombCtrl::Make_SubComb_EQ07_KBC2016(int nActive, BOOL bSlcmSpecial, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, 
                                              CombBase* cFP, CombBase* cEL, CombBase* cESP, CombBase* cPsTs,
                                              const CString& strPsTs, double dPSS)
{
	double dDLm09 = 0.9;	
	CString strAddDLm = _T("");
	CString strAddEL  = m_bModeUG ? _T("(SUF)") : _T("");
	CString strAddESP = _T("(SUF)");

	if (bSlcmSpecial)
	{
		dDLm09 = 0.9 - m_dSlcmSds * m_dSlcmSpecial; // for 0.9D + [(OMG)E - 0.2SDS*D]		
		strAddDLm = cEPV->IsExist() ? _T(" - (SPC)(SDS)(DL+EV)") : _T(" - (SPC)(SDS)DL");		
		strAddEL  = m_bModeUG ? _T("(SUF)(OMG)") : _T("(OMG)");
		strAddESP = _T("(SUF)(OMG)");
	}

	CString strDesc = _T("");
	/// see KBC2015, 0301.5(1). (0301.5.7) = 0.9D+1.0E -> [PMS 5279] 토압고려 하중조합 개선
    strDesc.Format(_T("0.9(DL+EV) + 1.0%sEL + (SFAC)%s%s"), strAddEL, strPsTs, strAddDLm);   Set_LCB(nActive, strDesc, dDLm09, cDL, dDLm09, cCSD, dDLm09, cEPV, 1.0, cEL, dPSS, cPsTs);
    strDesc.Format(_T("0.9(DL+EV) - 1.0%sEL + (SFAC)%s%s"), strAddEL, strPsTs, strAddDLm);   Set_LCB(nActive, strDesc, dDLm09, cDL, dDLm09, cCSD, dDLm09, cEPV,-1.0, cEL, dPSS, cPsTs);

    strDesc.Format(_T("0.9(DL+EV) + 1.0%sESP + (SFAC)%s%s"), strAddESP, strPsTs, strAddDLm); Set_LCB(nActive, strDesc, dDLm09, cDL, dDLm09, cCSD, dDLm09, cEPV, 1.0, cESP, dPSS, cPsTs);
    strDesc.Format(_T("0.9(DL+EV) - 1.0%sESP + (SFAC)%s%s"), strAddESP, strPsTs, strAddDLm); Set_LCB(nActive, strDesc, dDLm09, cDL, dDLm09, cCSD, dDLm09, cEPV,-1.0, cESP, dPSS, cPsTs);
}


void CLoadCombCtrl::Make_SubComb_EQ08_KBC2016(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP,
                                              CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
	CString strDesc = _T(""); 
	/// see KBC2015, 0301.5(1). (0301.5.8) = D+F -> [PMS 5279] 토압고려 하중조합 개선
	strDesc.Format(_T("(DL+EV+FP) + (SFAC)%s"), strPsTs); 
	Set_LCB(nActive, strDesc, 1.0,cDL, 1.0,cCSD, 1.0,cEPV, 1.0,cFP, dPSS,cPsTs);
}

void CLoadCombCtrl::Make_SubComb_EQ09_KBC2016(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, 
                                              CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL, 
                                              CombBase* cTL,  CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
	CString strDesc = _T("");
	/// see KBC2015, 0301.5(1). (0301.5.9) = D+F+L+T -> [PMS 5279] 토압고려 하중조합 개선	
    strDesc.Format(_T("(DL+EV+FP) + (LL+IL+TL) + (SFAC)%s"), strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cEPV, 1.0, cFP, 1.0, cLL, 1.0, cCSL, 1.0, cIL, 1.0, cTL, dPSS, cPsTs);
}

void CLoadCombCtrl::Make_SubComb_EQ10_KBC2016(int nActive, BOOL bExistLL,  CombBase* cDL, CombBase* cCSD, CombBase* cEPV,
                                              CombBase* cFP, CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cPsTs,
                                              const CString& strPsTs, double dPSS)
{
	CString strDesc = _T("");
	
	/// see KBC2015, 0301.5(1). (0301.5.10) = D+F+(Lr or S or R) -> [PMS 5279] 토압고려 하중조합 개선
	CString strCaseLR[3] = { _T("LR"), _T("SL"), _T("RL") };
	CombBase* cLoadLR[3] = { cLR, cSL, cRL };
	for(int i=0; i<3; ++i)
	{
		strDesc.Format(_T("1.0(DL+EV+FP) + 1.0%s + (SFAC)%s"), strCaseLR[i], strPsTs); 
		Set_LCB(nActive, strDesc, 1.0,cDL, 1.0,cCSD, 1.0,cEPV, 1.0,cFP, 1.0,cLoadLR[i], dPSS,cPsTs);
	}
}

void CLoadCombCtrl::Make_SubComb_EQ11_KBC2016(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, 
                                              CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL, CombBase* cLR, 
                                              CombBase* cSL, CombBase* cRL,  CombBase* cTL,  CombBase* cPsTs, 
                                              const CString& strPsTs, double dPSS)
{
	CString strDesc = _T("");
	
	/// see KBC2015, 0301.5(1). (0301.5.11) = D+F+0.75(L+T) + 0.75(Lr or S or R) -> [PMS 5279] 토압고려 하중조합 개선
	CString strCaseLR[3] = { _T("LR"), _T("SL"), _T("RL") };
	CombBase* cLoadLR[3] = { cLR, cSL, cRL };
    	
    for ( int i=0; i<3; ++i )
    {
        strDesc.Format(_T("1.0(DL+EV+FP) + 0.75(LL+IL+TL) + 0.75%s + (SFAC)%s"), strCaseLR[i], strPsTs);
        Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cEPV, 1.0, cFP, 0.75, cLL, 0.75, cCSL, 0.75, cIL, 0.75, cTL, 0.75, cLoadLR[i], dPSS, cPsTs);
    }
}

void CLoadCombCtrl::Make_SubComb_EQ12_KBC2016(int nActive, BOOL bSlcmSpecial, CombBase* cDL, CombBase* cCSD, CombBase* cEPV,
                                              CombBase* cFP, CombBase* cWL, CombBase* cEL, CombBase* cESP, CombBase* cPsTs,
                                              const CString& strPsTs, double dPSS)
{
	CString strDesc = _T("");
	strDesc.Format(_T("(DL+EV+FP) + 0.85WL + (SFAC)%s"), strPsTs);
	Set_LCB(nActive, strDesc, 1.0,cDL, 1.0,cCSD, 1.0,cEPV, 1.0,cFP, 0.85,cWL, dPSS,cPsTs);
	strDesc.Format(_T("(DL+EV+FP) - 0.85WL + (SFAC)%s"), strPsTs);
	Set_LCB(nActive, strDesc, 1.0,cDL, 1.0,cCSD, 1.0,cEPV, 1.0,cFP,-0.85,cWL, dPSS,cPsTs);
	
	double dDLp10 = 1.0;
	CString strAddDLp = _T("");
	CString strAddEL  = m_bModeUG ? _T("(SUF)") : _T("");
	CString strAddESP = _T("(SUF)");
	if (bSlcmSpecial)
	{
		dDLp10 = 1.0 + 0.7 * m_dSlcmSds * m_dSlcmSpecial; // for 1.0D + [(OMG)E + 0.2SDS*D]
		strAddDLp = cEPV->IsExist() ? _T(" + 0.7(SPC)(SDS)(DL+EV)") : _T(" + 0.7(SPC)(SDS)DL");
		strAddEL  = m_bModeUG ? _T("(SUF)(OMG)") : _T("(OMG)");
		strAddESP = _T("(SUF)(OMG)");
	}

	strDesc.Format(_T("(DL+EV+FP) + 0.7%sEL + (SFAC)%s%s"), strAddEL, strPsTs, strAddDLp);  
    Set_LCB(nActive, strDesc, dDLp10,cDL, dDLp10,cCSD, dDLp10,cEPV, 1.0,cFP, 0.7,cEL,  dPSS,cPsTs);
	
	strDesc.Format(_T("(DL+EV+FP) - 0.7%sEL + (SFAC)%s%s"), strAddEL, strPsTs, strAddDLp);
    Set_LCB(nActive, strDesc, dDLp10,cDL, dDLp10,cCSD, dDLp10,cEPV, 1.0,cFP,-0.7,cEL,  dPSS,cPsTs);

	strDesc.Format(_T("(DL+EV+FP) + 0.7%sESP + (SFAC)%s%s"), strAddESP, strPsTs, strAddDLp); 
    Set_LCB(nActive, strDesc, dDLp10,cDL, dDLp10,cCSD, dDLp10,cEPV, 1.0,cFP, 0.7,cESP, dPSS,cPsTs);

	strDesc.Format(_T("(DL+EV+FP) - 0.7%sESP + (SFAC)%s%s"), strAddESP, strPsTs, strAddDLp);
    Set_LCB(nActive, strDesc, dDLp10,cDL, dDLp10,cCSD, dDLp10,cEPV, 1.0,cFP,-0.7,cESP, dPSS,cPsTs);
}

void CLoadCombCtrl::Make_SubComb_EQ13_KBC2016(int nActive, BOOL bExistLL, BOOL bSlcmSpecial, CombBase* cDL, CombBase* cCSD,
                                              CombBase* cEPV, CombBase* cFP, CombBase* cLL, CombBase* cCSL,
                                              CombBase* cIL, CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cWL, 
                                              CombBase* cEL, CombBase* cESP, CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
	CString strDesc = _T("");

	/// see KBC2015, 0301.5(1). (0301.5.13) = D+F+0.75(0.85W or 0.7E)+0.75L+0.75(Lr or S or R) -> [PMS 5279] 토압고려 하중조합 개선
	BOOL bExistLRSeries = !cLR->IsExist() && !cSL->IsExist() && !cRL->IsExist()? FALSE : TRUE;
	double dWL  = 0.75*0.85;
	double dEL  = 0.75*0.70;
	
	double dDLp10 = 1.0;
	CString strAddDLp = _T("");
	CString strAddEL  = m_bModeUG ? _T("(SUF)") :  _T("");
	CString strAddESP = _T("(SUF)");

	if (bSlcmSpecial)
	{
		dDLp10 = 1.0 + dEL * m_dSlcmSds * m_dSlcmSpecial; // for 1.0D + [(OMG)E + 0.2SDS*D]
		strAddDLp = cEPV->IsExist() ? _T(" + (0.75*0.7)(SPC)(SDS)(DL+EV)") : _T(" + (0.75*0.7)(SPC)(SDS)DL");
		strAddEL  = m_bModeUG ? _T("(SUF)(OMG)") : _T("(OMG)");
		strAddESP = _T("(SUF)(OMG)");
	}

	CString strCaseLR[3] = { _T("LR"), _T("SL"), _T("RL") };
	CombBase* cLoadLR[3] = { cLR, cSL, cRL };
	
    if ( bExistLRSeries )
    {
        if ( bExistLL )
        {
            for ( int i=0; i<3; ++i )
            {
                strDesc.Format(_T("1.0(DL+EV+FP) + (0.75*0.85)WL + 0.75(LL+IL) + 0.75%s + (SFAC)%s"), strCaseLR[i], strPsTs);  Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cEPV, 1.0, cFP, dWL, cWL, 0.75, cLL, 0.75, cCSL, 0.75, cIL, 0.75, cLoadLR[i], dPSS, cPsTs);
                strDesc.Format(_T("1.0(DL+EV+FP) - (0.75*0.85)WL + 0.75(LL+IL) + 0.75%s + (SFAC)%s"), strCaseLR[i], strPsTs);  Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cEPV, 1.0, cFP, -dWL, cWL, 0.75, cLL, 0.75, cCSL, 0.75, cIL, 0.75, cLoadLR[i], dPSS, cPsTs);
            }

            for ( int i=0; i<3; ++i )
            {
                strDesc.Format(_T("1.0(DL+EV+FP) + (0.75*0.70)%sEL + 0.75(LL+IL) + 0.75%s + (SFAC)%s%s"), strAddEL, strCaseLR[i], strPsTs, strAddDLp);    Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, dEL, cEL, 0.75, cLL, 0.75, cCSL, 0.75, cIL, 0.75, cLoadLR[i], dPSS, cPsTs);
                strDesc.Format(_T("1.0(DL+EV+FP) - (0.75*0.70)%sEL + 0.75(LL+IL) + 0.75%s + (SFAC)%s%s"), strAddEL, strCaseLR[i], strPsTs, strAddDLp);    Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, -dEL, cEL, 0.75, cLL, 0.75, cCSL, 0.75, cIL, 0.75, cLoadLR[i], dPSS, cPsTs);

                strDesc.Format(_T("1.0(DL+EV+FP) + (0.75*0.70)%sESP + 0.75(LL+IL) + 0.75%s + (SFAC)%s%s"), strAddESP, strCaseLR[i], strPsTs, strAddDLp);  Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, dEL, cESP, 0.75, cLL, 0.75, cCSL, 0.75, cIL, 0.75, cLoadLR[i], dPSS, cPsTs);
                strDesc.Format(_T("1.0(DL+EV+FP) - (0.75*0.70)%sESP + 0.75(LL+IL) + 0.75%s + (SFAC)%s%s"), strAddESP, strCaseLR[i], strPsTs, strAddDLp);  Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, -dEL, cESP, 0.75, cLL, 0.75, cCSL, 0.75, cIL, 0.75, cLoadLR[i], dPSS, cPsTs);
            }
        }
        else
        {
            for ( int i=0; i<3; ++i )
            {
                strDesc.Format(_T("1.0(DL+EV+FP) + (0.75*0.85)WL + 0.75%s + (SFAC)%s"), strCaseLR[i], strPsTs); Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cEPV, 1.0, cFP, dWL, cWL, 0.75, cLoadLR[i], dPSS, cPsTs);
                strDesc.Format(_T("1.0(DL+EV+FP) - (0.75*0.85)WL + 0.75%s + (SFAC)%s"), strCaseLR[i], strPsTs); Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cEPV, 1.0, cFP, -dWL, cWL, 0.75, cLoadLR[i], dPSS, cPsTs);
            }

            for ( int i=0; i<3; ++i )
            {
                strDesc.Format(_T("1.0(DL+EV+FP) + (0.75*0.70)%sEL + 0.75%s + (SFAC)%s%s"), strAddEL, strCaseLR[i], strPsTs, strAddDLp);   Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, dEL, cEL, 0.75, cLoadLR[i], dPSS, cPsTs);
                strDesc.Format(_T("1.0(DL+EV+FP) - (0.75*0.70)%sEL + 0.75%s + (SFAC)%s%s"), strAddEL, strCaseLR[i], strPsTs, strAddDLp);   Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, -dEL, cEL, 0.75, cLoadLR[i], dPSS, cPsTs);

                strDesc.Format(_T("1.0(DL+EV+FP) + (0.75*0.70)%sESP + 0.75%s + (SFAC)%s%s"), strAddESP, strCaseLR[i], strPsTs, strAddDLp); Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, dEL, cESP, 0.75, cLoadLR[i], dPSS, cPsTs);
                strDesc.Format(_T("1.0(DL+EV+FP) - (0.75*0.70)%sESP + 0.75%s + (SFAC)%s%s"), strAddESP, strCaseLR[i], strPsTs, strAddDLp); Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, -dEL, cESP, 0.75, cLoadLR[i], dPSS, cPsTs);
            }
        }
    }
    else
    {
        if ( bExistLL )
        {
            strDesc.Format(_T("1.0(DL+EV+FP) + (0.75*0.85)WL + 0.75(LL+IL) + (SFAC)%s"), strPsTs); Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cEPV, 1.0, cFP, dWL, cWL, 0.75, cLL, 0.75, cCSL, 0.75, cIL, dPSS, cPsTs);
            strDesc.Format(_T("1.0(DL+EV+FP) - (0.75*0.85)WL + 0.75(LL+IL) + (SFAC)%s"), strPsTs); Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cEPV, 1.0, cFP, -dWL, cWL, 0.75, cLL, 0.75, cCSL, 0.75, cIL, dPSS, cPsTs);

            strDesc.Format(_T("1.0(DL+EV+FP) + (0.75*0.70)%sEL + 0.75(LL+IL) + (SFAC)%s%s"), strAddEL, strPsTs, strAddDLp);   Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, dEL, cEL, 0.75, cLL, 0.75, cCSL, 0.75, cIL, dPSS, cPsTs);
            strDesc.Format(_T("1.0(DL+EV+FP) - (0.75*0.70)%sEL + 0.75(LL+IL) + (SFAC)%s%s"), strAddEL, strPsTs, strAddDLp);   Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, -dEL, cEL, 0.75, cLL, 0.75, cCSL, 0.75, cIL, dPSS, cPsTs);

            strDesc.Format(_T("1.0(DL+EV+FP) + (0.75*0.70)%sESP + 0.75(LL+IL) + (SFAC)%s%s"), strAddESP, strPsTs, strAddDLp); Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, dEL, cESP, 0.75, cLL, 0.75, cCSL, 0.75, cIL, dPSS, cPsTs);
            strDesc.Format(_T("1.0(DL+EV+FP) - (0.75*0.70)%sESP + 0.75(LL+IL) + (SFAC)%s%s"), strAddESP, strPsTs, strAddDLp); Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, -dEL, cESP, 0.75, cLL, 0.75, cCSL, 0.75, cIL, dPSS, cPsTs);
        }
        else
        {
            strDesc.Format(_T("1.0(DL+EV+FP) + (0.75*0.85)WL + (SFAC)%s"), strPsTs); Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cEPV, 1.0, cFP, dWL, cWL, dPSS, cPsTs);
            strDesc.Format(_T("1.0(DL+EV+FP) - (0.75*0.85)WL + (SFAC)%s"), strPsTs); Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cEPV, 1.0, cFP, -dWL, cWL, dPSS, cPsTs);

            strDesc.Format(_T("1.0(DL+EV+FP) + (0.75*0.70)%sEL + (SFAC)%s%s"), strAddEL, strPsTs, strAddDLp);   Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, dEL, cEL, dPSS, cPsTs);
            strDesc.Format(_T("1.0(DL+EV+FP) - (0.75*0.70)%sEL + (SFAC)%s%s"), strAddEL, strPsTs, strAddDLp);   Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, -dEL, cEL, dPSS, cPsTs);

            strDesc.Format(_T("1.0(DL+EV+FP) + (0.75*0.70)%sESP + (SFAC)%s%s"), strAddESP, strPsTs, strAddDLp); Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, dEL, cESP, dPSS, cPsTs);
            strDesc.Format(_T("1.0(DL+EV+FP) - (0.75*0.70)%sESP + (SFAC)%s%s"), strAddESP, strPsTs, strAddDLp); Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, -dEL, cESP, dPSS, cPsTs);
        }
    }    
}

void CLoadCombCtrl::Make_SubComb_EQ14_KBC2016(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, 
                                              CombBase* cWL, CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
	CString strDesc = _T("");
	/// see KBC2015, 0301.5(1). (0301.5.14) = 0.6D+0.85W.
    strDesc.Format(_T("0.6(DL+EV) + 0.85WL + (SFAC)%s"), strPsTs); Set_LCB(nActive, strDesc, 0.6, cDL, 0.6, cCSD, 0.6, cEPV, 0.85, cWL, dPSS, cPsTs);
    strDesc.Format(_T("0.6(DL+EV) - 0.85WL + (SFAC)%s"), strPsTs); Set_LCB(nActive, strDesc, 0.6, cDL, 0.6, cCSD, 0.6, cEPV, -0.85, cWL, dPSS, cPsTs);
}

void CLoadCombCtrl::Make_SubComb_EQ15_KBC2016(int nActive, BOOL bSlcmSpecial, CombBase* cDL, CombBase* cCSD, CombBase* cEPV,
                                              CombBase* cFP, CombBase* cEL, CombBase* cESP, CombBase* cPsTs, 
                                              const CString& strPsTs, double dPSS)
{
	/// see KBC2015, 0301.5(1). (0301.5.15) = 0.6D+0.7E.
	double dDLm06 = 0.6;
	CString strAddDLm = _T("");
	CString strAddEL  = m_bModeUG ? _T("(SUF)") : _T("");
	CString strAddESP = _T("(SUF)");

	if (bSlcmSpecial)
	{
		dDLm06 = 0.6 - 0.7 * m_dSlcmSds * m_dSlcmSpecial; // for 0.6D + [(OMG)E - 0.2SDS*D]
		strAddDLm = cEPV->IsExist() ? _T(" - (0.75*0.7)(SPC)(SDS)(DL+EV)") : _T(" - (0.75*0.7)(SPC)(SDS)DL");
		strAddEL  = m_bModeUG ? _T("(SUF)(OMG)") : _T("(OMG)");
		strAddESP = _T("(SUF)(OMG)");
	}
	
    CString strDesc = _T("");
    strDesc.Format(_T("0.6(DL+EV) + 0.7%sEL + (SFAC)%s%s"), strAddEL, strPsTs, strAddDLm);   Set_LCB(nActive, strDesc, dDLm06, cDL, dDLm06, cCSD, dDLm06, cEPV, 0.7, cEL, dPSS, cPsTs);
    strDesc.Format(_T("0.6(DL+EV) - 0.7%sEL + (SFAC)%s%s"), strAddEL, strPsTs, strAddDLm);   Set_LCB(nActive, strDesc, dDLm06, cDL, dDLm06, cCSD, dDLm06, cEPV, -0.7, cEL, dPSS, cPsTs);

    strDesc.Format(_T("0.6(DL+EV) + 0.7%sESP + (SFAC)%s%s"), strAddESP, strPsTs, strAddDLm); Set_LCB(nActive, strDesc, dDLm06, cDL, dDLm06, cCSD, dDLm06, cEPV, 0.7, cESP, dPSS, cPsTs);
    strDesc.Format(_T("0.6(DL+EV) - 0.7%sESP + (SFAC)%s%s"), strAddESP, strPsTs, strAddDLm); Set_LCB(nActive, strDesc, dDLm06, cDL, dDLm06, cCSD, dDLm06, cEPV, -0.7, cESP, dPSS, cPsTs);
}


void CLoadCombCtrl::Make_SubComb_EQ01_KBC2009(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cPS)
{
    // [0702.2.1] 1.4D + 1.4F.
    if ( cFP->IsExist() )
    {
        Set_LCB( nA, _T("1.4(DL+FP) + PS"), 1.4,cDL,1.4,cCSD, 1.4,cFP, 1.0,cPS );
    }
    else
    {
        Set_LCB( nA, _T("1.4DL + PS"), 1.4,cDL,1.4,cCSD, 1.0,cPS );
    }
}

void CLoadCombCtrl::Make_SubComb_EQ02_KBC2009(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cTL, 
                                              CombBase* cLL, CombBase* cCSL, CombBase* cEP, CombBase* cEPV, CombBase* cEPH, 
                                              CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cPS)
{
    // [0702.2.2] 1.2(D+F+T) +1.6(L+H) + 0.5(LR or S or R)
    if ( !cLR->IsExist() && !cSL->IsExist() && !cRL->IsExist() )
    {	
        Set_LCB( nA, _T("1.2(DL+FP+TL) + 1.6(LL+(EP+EV+EH)) + PS"), 1.2,cDL,1.2,cCSD, 1.2,cFP, 1.2,cTL, 1.6,cLL,1.6,cCSL, 1.6,cEP, 1.6,cEPV, 1.6,cEPH, 1.0,cPS );
    }
    else
    {
        Set_LCB( nA, _T("1.2(DL+FP+TL) + 1.6(LL+(EP+EV+EH)) + 0.5LR + PS"), 1.2,cDL,1.2,cCSD, 1.2,cFP, 1.2,cTL, 1.6,cLL,1.6,cCSL, 1.6,cEP, 1.6,cEPV, 1.6,cEPH, 0.5,cLR, 1.0,cPS );
        Set_LCB( nA, _T("1.2(DL+FP+TL) + 1.6(LL+(EP+EV+EH)) + 0.5SL + PS"), 1.2,cDL,1.2,cCSD, 1.2,cFP, 1.2,cTL, 1.6,cLL,1.6,cCSL, 1.6,cEP, 1.6,cEPV, 1.6,cEPH, 0.5,cSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2(DL+FP+TL) + 1.6(LL+(EP+EV+EH)) + 0.5RL + PS"), 1.2,cDL,1.2,cCSD, 1.2,cFP, 1.2,cTL, 1.6,cLL,1.6,cCSL, 1.6,cEP, 1.6,cEPV, 1.6,cEPH, 0.5,cRL, 1.0,cPS );
    }
}

void CLoadCombCtrl::Make_SubComb_EQ03_KBC2009(int nA, BOOL bExistLL, double dLf1, CombBase* cDL, CombBase* cCSD, 
                                              CombBase* cLL, CombBase* cCSL, CombBase* cWL,
                                              CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cPS)
{
    // [0702.2.3] 1.2D + 1.6(LR or S or R) + (1.0L or 0.65W)
    if ( !bExistLL && !cWL->IsExist() )
    {
        Set_LCB( nA, _T("1.2DL + 1.6LR + PS"), 1.2,cDL,1.2,cCSD, 1.6,cLR, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL + 1.6SL + PS"), 1.2,cDL,1.2,cCSD, 1.6,cSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL + 1.6RL + PS"), 1.2,cDL,1.2,cCSD, 1.6,cRL, 1.0,cPS );
    }
    else
    {
        Set_LCB( nA, _T("1.2DL + 1.6LR + 1.0LL + PS"),  1.2,cDL,1.2,cCSD, 1.6,cLR,  dLf1,cLL,dLf1,cCSL,	1.0,cPS );
        Set_LCB( nA, _T("1.2DL + 1.6LR + 0.65WL + PS"), 1.2,cDL,1.2,cCSD, 1.6,cLR,  0.65,cWL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL + 1.6LR - 0.65WL + PS"), 1.2,cDL,1.2,cCSD, 1.6,cLR, -0.65,cWL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL + 1.6SL + 1.0LL + PS"),	1.2,cDL,1.2,cCSD, 1.6,cSL,  dLf1,cLL,dLf1,cCSL,	1.0,cPS );
        Set_LCB( nA, _T("1.2DL + 1.6SL + 0.65WL + PS"), 1.2,cDL,1.2,cCSD, 1.6,cSL,  0.65,cWL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL + 1.6SL - 0.65WL + PS"), 1.2,cDL,1.2,cCSD, 1.6,cSL, -0.65,cWL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL + 1.6RL + 1.0LL + PS"),	1.2,cDL,1.2,cCSD, 1.6,cRL,  dLf1,cLL,dLf1,cCSL,	1.0,cPS );
        Set_LCB( nA, _T("1.2DL + 1.6RL + 0.65WL + PS"), 1.2,cDL,1.2,cCSD, 1.6,cRL,  0.65,cWL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL + 1.6RL - 0.65WL + PS"), 1.2,cDL,1.2,cCSD, 1.6,cRL, -0.65,cWL, 1.0,cPS );
    }
}

void CLoadCombCtrl::Make_SubComb_EQ04_KBC2009(int nA, BOOL bExistLL, double dLf1, CombBase* cDL, CombBase* cCSD, 
                                              CombBase* cLL, CombBase* cCSL, CombBase* cWL, CombBase* cLR,
                                              CombBase* cSL, CombBase* cRL, CombBase* cPS)
{
    const BOOL bExistLR = cLR->IsExist();
    const BOOL bExistSL = cSL->IsExist();
    const BOOL bExistRL = cRL->IsExist();

    // [0702.2.4] 1.2D + 1.3W + 1.0L +0.5(LR or S or R)    
    if ( !bExistLL && !bExistLR && !bExistSL && !bExistRL )	
    {		
        Set_LCB( nA, _T("1.2DL + 1.3WL + PS"), 1.2,cDL,1.2,cCSD,  1.3,cWL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL - 1.3WL + PS"), 1.2,cDL,1.2,cCSD, -1.3,cWL, 1.0,cPS );
    }
    else if ( !bExistLR && !bExistSL && !bExistRL )	
    {		
        Set_LCB( nA, _T("1.2DL + 1.3WL + 1.0LL + PS"), 1.2,cDL,1.2,cCSD,  1.3,cWL, dLf1,cLL,dLf1,cCSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL - 1.3WL + 1.0LL + PS"), 1.2,cDL,1.2,cCSD, -1.3,cWL, dLf1,cLL,dLf1,cCSL, 1.0,cPS );
    }
    else if ( !bExistLL )
    {		
        Set_LCB( nA, _T("1.2DL + 1.3WL + 0.5LR + PS"), 1.2,cDL,1.2,cCSD,  1.3,cWL, 0.5,cLR, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL - 1.3WL + 0.5LR + PS"), 1.2,cDL,1.2,cCSD, -1.3,cWL, 0.5,cLR, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL + 1.3WL + 0.5SL + PS"), 1.2,cDL,1.2,cCSD,  1.3,cWL, 0.5,cSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL - 1.3WL + 0.5SL + PS"), 1.2,cDL,1.2,cCSD, -1.3,cWL, 0.5,cSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL + 1.3WL + 0.5RL + PS"), 1.2,cDL,1.2,cCSD,  1.3,cWL, 0.5,cRL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL - 1.3WL + 0.5RL + PS"), 1.2,cDL,1.2,cCSD, -1.3,cWL, 0.5,cRL, 1.0,cPS );
    }
    else
    {		
        Set_LCB( nA, _T("1.2DL + 1.3WL + 1.0LL + 0.5LR + PS"), 1.2,cDL,1.2,cCSD,  1.3,cWL, dLf1,cLL,dLf1,cCSL, 0.5,cLR, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL - 1.3WL + 1.0LL + 0.5LR + PS"), 1.2,cDL,1.2,cCSD, -1.3,cWL, dLf1,cLL,dLf1,cCSL, 0.5,cLR, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL + 1.3WL + 1.0LL + 0.5SL + PS"), 1.2,cDL,1.2,cCSD,  1.3,cWL, dLf1,cLL,dLf1,cCSL, 0.5,cSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL - 1.3WL + 1.0LL + 0.5SL + PS"), 1.2,cDL,1.2,cCSD, -1.3,cWL, dLf1,cLL,dLf1,cCSL, 0.5,cSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL + 1.3WL + 1.0LL + 0.5RL + PS"), 1.2,cDL,1.2,cCSD,  1.3,cWL, dLf1,cLL,dLf1,cCSL, 0.5,cRL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL - 1.3WL + 1.0LL + 0.5RL + PS"), 1.2,cDL,1.2,cCSD, -1.3,cWL, dLf1,cLL,dLf1,cCSL, 0.5,cRL, 1.0,cPS );
    }        
}

void CLoadCombCtrl::Make_SubComb_EQ06_KBC2009(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cWL, CombBase* cEP,
                                              CombBase* cEPV, CombBase* cEPH, CombBase* cPS)
{
    // [0702.2.6] 0.9D + 1.3W + 1.6H
    if ( !cEP->IsExist() && !cEPV->IsExist() && !cEPH->IsExist() )
    {
        Set_LCB( nA, _T("0.9DL + 1.3WL + PS"), 0.9,cDL,0.9,cCSD,  1.3,cWL, 1.0,cPS );
        Set_LCB( nA, _T("0.9DL - 1.3WL + PS"), 0.9,cDL,0.9,cCSD, -1.3,cWL, 1.0,cPS );
    }
    else if ( !cWL->IsExist() )
    {
        Set_LCB( nA, _T("0.9DL + 1.6(EP+EV+EH) + PS"), 0.9,cDL,0.9,cCSD, 1.6,cEP, 1.6,cEPV, 1.6,cEPH, 1.0,cPS );
    }
    else
    {
        Set_LCB( nA, _T("0.9DL + 1.3WL + 1.6(EP+EV+EH) + PS"), 0.9,cDL,0.9,cCSD,  1.3,cWL, 1.6,cEP, 1.6,cEPV, 1.6,cEPH, 1.0,cPS );
        Set_LCB( nA, _T("0.9DL - 1.3WL + 1.6(EP+EV+EH) + PS"), 0.9,cDL,0.9,cCSD, -1.3,cWL, 1.6,cEP, 1.6,cEPV, 1.6,cEPH, 1.0,cPS );
    }
}

void CLoadCombCtrl::Make_SubComb_EQ05_KBC2009(int nA, BOOL bExistLL, double dLf1, CombBase* cDL, CombBase* cCSD,
                                              CombBase* cLL, CombBase* cCSL, CombBase* cEL, CombBase* cESP, CombBase* cSL, CombBase* cPS)
{
    const BOOL bExistSL = cSL->IsExist();
    // [0702.2.5] 1.2D + 1.0E + 1.0L + 0.2S		
    if ( !bExistLL && !bExistSL )
    {
        Set_LCB( nA, _T("1.2DL + 1.0EL + PS"),		 1.2,cDL,1.2,cCSD,  1.0, cEL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL - 1.0EL + PS"),		 1.2,cDL,1.2,cCSD, -1.0, cEL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL + 1.0(SUF)ESP + PS"), 1.2,cDL,1.2,cCSD,  1.0,cESP, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL - 1.0(SUF)ESP + PS"), 1.2,cDL,1.2,cCSD, -1.0,cESP, 1.0,cPS );
    }
    else if ( !bExistSL )
    {
        Set_LCB( nA, _T("1.2DL + 1.0EL + 1.0LL + PS"),		 1.2,cDL,1.2,cCSD,  1.0, cEL, dLf1,cLL,dLf1,cCSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL - 1.0EL + 1.0LL + PS"),		 1.2,cDL,1.2,cCSD, -1.0, cEL, dLf1,cLL,dLf1,cCSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL + 1.0(SUF)ESP + 1.0LL + PS"), 1.2,cDL,1.2,cCSD,  1.0,cESP, dLf1,cLL,dLf1,cCSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL - 1.0(SUF)ESP + 1.0LL + PS"), 1.2,cDL,1.2,cCSD, -1.0,cESP, dLf1,cLL,dLf1,cCSL, 1.0,cPS );
    }
    else if ( !bExistLL )
    {
        Set_LCB( nA, _T("1.2DL + 1.0EL + 0.2SL + PS"),		 1.2,cDL,1.2,cCSD,  1.0, cEL, 0.2,cSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL - 1.0EL + 0.2SL + PS"),		 1.2,cDL,1.2,cCSD, -1.0, cEL, 0.2,cSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL + 1.0(SUF)ESP + 0.2SL + PS"), 1.2,cDL,1.2,cCSD,  1.0,cESP, 0.2,cSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL - 1.0(SUF)ESP + 0.2SL + PS"), 1.2,cDL,1.2,cCSD, -1.0,cESP, 0.2,cSL, 1.0,cPS );
    }
    else
    {
        Set_LCB( nA, _T("1.2DL + 1.0EL + 1.0LL + 0.2SL + PS"),		 1.2,cDL,1.2,cCSD,  1.0, cEL, dLf1,cLL,dLf1,cCSL, 0.2,cSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL - 1.0EL + 1.0LL + 0.2SL + PS"),		 1.2,cDL,1.2,cCSD, -1.0, cEL, dLf1,cLL,dLf1,cCSL, 0.2,cSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL + 1.0(SUF)ESP + 1.0LL + 0.2SL + PS"), 1.2,cDL,1.2,cCSD,  1.0,cESP, dLf1,cLL,dLf1,cCSL, 0.2,cSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL - 1.0(SUF)ESP + 1.0LL + 0.2SL + PS"), 1.2,cDL,1.2,cCSD, -1.0,cESP, dLf1,cLL,dLf1,cCSL, 0.2,cSL, 1.0,cPS );
    }
}

void CLoadCombCtrl::Make_SubComb_EQ05_KBC2009_Special(int nA, BOOL bExistLL, double dLf1, CombBase* cDL, CombBase* cCSD,
                                                      CombBase* cLL, CombBase* cCSL, CombBase* cEL, CombBase* cESP, CombBase* cSL, CombBase* cPS)                                       
{
    const double dDLp12 = 1.20 + m_dSlcmSds * m_dSlcmSpecial; // for 1.2D + [(OMG)E + 0.2SDS*D]    
    const BOOL bExistSL = cSL->IsExist();

    // [0702.2.5] 1.2D + 1.0E + 1.0L + 0.2S		
    if( !bExistLL && !bExistSL )
    {
        Set_LCB( nA, _T("1.2DL + 1.0(OMG)EL + PS + (SPC)(SDS)DL"),		 dDLp12,cDL,dDLp12,cCSD,  1.0, cEL, 1.0,cPS	);
        Set_LCB( nA, _T("1.2DL - 1.0(OMG)EL + PS + (SPC)(SDS)DL"),		 dDLp12,cDL,dDLp12,cCSD, -1.0, cEL, 1.0,cPS	);
        Set_LCB( nA, _T("1.2DL + 1.0(SUF)(OMG)ESP + PS + (SPC)(SDS)DL"), dDLp12,cDL,dDLp12,cCSD,  1.0,cESP, 1.0,cPS	);
        Set_LCB( nA, _T("1.2DL - 1.0(SUF)(OMG)ESP + PS + (SPC)(SDS)DL"), dDLp12,cDL,dDLp12,cCSD, -1.0,cESP, 1.0,cPS	);
    }
    else if ( !bExistSL )
    {
        Set_LCB( nA, _T("1.2DL + 1.0(OMG)EL + 1.0LL + PS + (SPC)(SDS)DL"),		 dDLp12,cDL,dDLp12,cCSD,  1.0, cEL, dLf1,cLL,dLf1,cCSL, 1.0,cPS	);
        Set_LCB( nA, _T("1.2DL - 1.0(OMG)EL + 1.0LL + PS + (SPC)(SDS)DL"),		 dDLp12,cDL,dDLp12,cCSD, -1.0, cEL, dLf1,cLL,dLf1,cCSL, 1.0,cPS	);
        Set_LCB( nA, _T("1.2DL + 1.0(SUF)(OMG)ESP + 1.0LL + PS + (SPC)(SDS)DL"), dDLp12,cDL,dDLp12,cCSD,  1.0,cESP, dLf1,cLL,dLf1,cCSL, 1.0,cPS	);
        Set_LCB( nA, _T("1.2DL - 1.0(SUF)(OMG)ESP + 1.0LL + PS + (SPC)(SDS)DL"), dDLp12,cDL,dDLp12,cCSD, -1.0,cESP, dLf1,cLL,dLf1,cCSL, 1.0,cPS	);
    }
    else if ( !bExistLL )
    {
        Set_LCB( nA, _T("1.2DL + 1.0(OMG)EL + 0.2SL + PS + (SPC)(SDS)DL"),		 dDLp12,cDL,dDLp12,cCSD,  1.0, cEL, 0.2,cSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL - 1.0(OMG)EL + 0.2SL + PS + (SPC)(SDS)DL"),		 dDLp12,cDL,dDLp12,cCSD, -1.0, cEL, 0.2,cSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL + 1.0(SUF)(OMG)ESP + 0.2SL + PS + (SPC)(SDS)DL"), dDLp12,cDL,dDLp12,cCSD,  1.0,cESP, 0.2,cSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL - 1.0(SUF)(OMG)ESP + 0.2SL + PS + (SPC)(SDS)DL"), dDLp12,cDL,dDLp12,cCSD, -1.0,cESP, 0.2,cSL, 1.0,cPS );
    }
    else
    {
        Set_LCB( nA, _T("1.2DL + 1.0(OMG)EL + 1.0LL + 0.2SL + PS + (SPC)(SDS)DL"),		 dDLp12,cDL,dDLp12,cCSD,  1.0, cEL, dLf1,cLL, dLf1,cCSL, 0.2,cSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL - 1.0(OMG)EL + 1.0LL + 0.2SL + PS + (SPC)(SDS)DL"),		 dDLp12,cDL,dDLp12,cCSD, -1.0, cEL, dLf1,cLL, dLf1,cCSL, 0.2,cSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL + 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + PS + (SPC)(SDS)DL"), dDLp12,cDL,dDLp12,cCSD,  1.0,cESP, dLf1,cLL, dLf1,cCSL, 0.2,cSL, 1.0,cPS );
        Set_LCB( nA, _T("1.2DL - 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + PS + (SPC)(SDS)DL"), dDLp12,cDL,dDLp12,cCSD, -1.0,cESP, dLf1,cLL, dLf1,cCSL, 0.2,cSL, 1.0,cPS );
    }
}

void CLoadCombCtrl::Make_SubComb_EQ07_KBC2009(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cEL, CombBase* cESP,
                                              CombBase* cEP, CombBase* cEPV, CombBase* cEPH, CombBase* cPS)
{
    // [0702.2.7] 0.9D + 1.0E + 1.6H		
    if ( !cEP->IsExist() && !cEPV->IsExist() && !cEPH->IsExist() )
    {
        Set_LCB( nA, _T("0.9DL + 1.0EL + PS"),		 0.9,cDL,0.9,cCSD,  1.0,cEL,  1.0,cPS );
        Set_LCB( nA, _T("0.9DL - 1.0EL + PS"),		 0.9,cDL,0.9,cCSD, -1.0,cEL,  1.0,cPS );
        Set_LCB( nA, _T("0.9DL + 1.0(SUF)ESP + PS"), 0.9,cDL,0.9,cCSD,  1.0,cESP, 1.0,cPS );
        Set_LCB( nA, _T("0.9DL - 1.0(SUF)ESP + PS"), 0.9,cDL,0.9,cCSD, -1.0,cESP, 1.0,cPS );
    }
    else if ( !cEL->IsExist() && !cESP->IsExist() )
    {
        Set_LCB( nA, _T("0.9DL + 1.6(EP+EV+EH) + PS"),  .9,cDL,0.9,cCSD, 1.6,cEP, 1.6,cEPV, 1.6,cEPH, 1.0,cPS );
    }
    else
    {
        Set_LCB( nA, _T("0.9DL + 1.0EL + 1.6(EP+EV+EH) + PS"),		 0.9,cDL,0.9,cCSD,  1.0,cEL,  1.6,cEP, 1.6,cEPV, 1.6,cEPH, 1.0,cPS );
        Set_LCB( nA, _T("0.9DL - 1.0EL + 1.6(EP+EV+EH) + PS"),		 0.9,cDL,0.9,cCSD, -1.0,cEL,  1.6,cEP, 1.6,cEPV, 1.6,cEPH, 1.0,cPS );
        Set_LCB( nA, _T("0.9DL + 1.0(SUF)ESP + 1.6(EP+EV+EH) + PS"), 0.9,cDL,0.9,cCSD,  1.0,cESP, 1.6,cEP, 1.6,cEPV, 1.6,cEPH, 1.0,cPS );
        Set_LCB( nA, _T("0.9DL - 1.0(SUF)ESP + 1.6(EP+EV+EH) + PS"), 0.9,cDL,0.9,cCSD, -1.0,cESP, 1.6,cEP, 1.6,cEPV, 1.6,cEPH, 1.0,cPS );
    }
}

void CLoadCombCtrl::Make_SubComb_EQ07_KBC2009_Special(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cEL, CombBase* cESP,
                                                      CombBase* cEP, CombBase* cEPV, CombBase* cEPH, CombBase* cPS)
{
    double dDLm09 = 0.90 - m_dSlcmSds * m_dSlcmSpecial; // for 0.9D + [(OMG)E - 0.2SDS*D]
    // [0702.2.7] 0.9D + 1.0E + 1.6H		
    if ( !cEP->IsExist() && !cEPV->IsExist() && !cEPH->IsExist() )
    {
        Set_LCB( nA, _T("0.9DL + 1.0(OMG)EL + PS - (SPC)(SDS)DL"),	      dDLm09,cDL,dDLm09,cCSD,  1.0,cEL,  1.0,cPS );
        Set_LCB( nA, _T("0.9DL - 1.0(OMG)EL + PS - (SPC)(SDS)DL"),	      dDLm09,cDL,dDLm09,cCSD, -1.0,cEL,  1.0,cPS );
        Set_LCB( nA, _T("0.9DL + 1.0(SUF)(OMG)ESP + PS - (SPC)(SDS)DL)"), dDLm09,cDL,dDLm09,cCSD,  1.0,cESP, 1.0,cPS );
        Set_LCB( nA, _T("0.9DL - 1.0(SUF)(OMG)ESP + PS - (SPC)(SDS)DL)"), dDLm09,cDL,dDLm09,cCSD, -1.0,cESP, 1.0,cPS );
    }
    else if ( !cEL->IsExist() && !cESP->IsExist() )
    {
        Set_LCB( nA, _T("0.9DL + 1.6(EP+EV+EH) + PS"), 0.9,cDL,0.9,cCSD, 1.6,cEP, 1.6,cEPV, 1.6,cEPH, 1.0,cPS);
    }
    else
    {
        Set_LCB( nA, _T("0.9DL + 1.0(OMG)EL + 1.6(EP+EV+EH) + PS - (SPC)(SDS)DL"),		 dDLm09,cDL,dDLm09,cCSD,  1.0,cEL,  1.6,cEP, 1.6,cEPV, 1.6,cEPH, 1.0,cPS );
        Set_LCB( nA, _T("0.9DL - 1.0(OMG)EL + 1.6(EP+EV+EH) + PS - (SPC)(SDS)DL"),		 dDLm09,cDL,dDLm09,cCSD, -1.0,cEL,  1.6,cEP, 1.6,cEPV, 1.6,cEPH, 1.0,cPS );
        Set_LCB( nA, _T("0.9DL + 1.0(SUF)(OMG)ESP + 1.6(EP+EV+EH) + PS - (SPC)(SDS)DL"), dDLm09,cDL,dDLm09,cCSD,  1.0,cESP, 1.6,cEP, 1.6,cEPV, 1.6,cEPH, 1.0,cPS );
        Set_LCB( nA, _T("0.9DL - 1.0(SUF)(OMG)ESP + 1.6(EP+EV+EH) + PS - (SPC)(SDS)DL"), dDLm09,cDL,dDLm09,cCSD, -1.0,cESP, 1.6,cEP, 1.6,cEPV, 1.6,cEPH, 1.0,cPS );
    }
}


void CLoadCombCtrl::Make_SubComb_EQ1_KCI_USD12(int nA, BOOL bStageCS, double dPSS, 
                                               CombBase* cDL,  CombBase* cFP, CombBase* cPS, CombBase* cTS,  CombBase* cCSD,
                                               BOOL bSlcmSpecial, BOOL bSlcmVertical)
{
    // (3.3.1) 1.4D + 1.4 F
    int nActive = GetActiveIndex(nA, bSlcmSpecial, bSlcmVertical);

    CString   strPSorTS = bStageCS ? _T("TS") : _T("PS");
    CombBase* cPSorTs   = bStageCS ?    cTS   :    cPS;
    CString   strDesc   = _T("");

    if(cFP->IsExist())
    {	
        strDesc.Format(_T("1.4(DL+FP) + (SFAC)%s"), strPSorTS);	
        Set_LCB(nActive, strDesc, 1.4,cDL,1.4,cCSD,1.4,cFP,dPSS,cPSorTs); 
    }
    else
    { 
        strDesc.Format(_T("1.4DL + (SFAC)%s"), strPSorTS);     	
        Set_LCB(nActive, strDesc,	1.4,cDL,1.4,cCSD,        dPSS,cPSorTs); 
    }
}

void CLoadCombCtrl::Make_SubComb_EQ2_KCI_USD12(int nA, BOOL bStageCS, double dPSS, double dEV,
                                               CombBase* cDL,  CombBase* cLL, CombBase* cLR, CombBase* cSL,  CombBase* cRL,  CombBase* cTL,  CombBase* cSH,  CombBase* cCR, 
                                               CombBase* cFP,  CombBase* cPS, CombBase* cTS, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cEPH, 
                                               CombBase* cEPV,
                                               BOOL bSlcmSpecial, BOOL bSlcmVertical)
{
    // (3.3.2) 1.2(D+F+T) +1.6(L+H) + 0.5(LR or S or R)
    int nActive = 0;
    if     (!bSlcmSpecial && !bSlcmVertical) nActive = nA;
    else if( bSlcmSpecial && !bSlcmVertical) nActive = nA + 3;
    else if(!bSlcmSpecial &&  bSlcmVertical) nActive = nA + 4;
    else ASSERT(0);

    CString   strPSorTS = bStageCS ? _T("TS") : _T("PS");
    CombBase* cPSorTs   = bStageCS ?    cTS   :    cPS;
    CString   strDesc   = _T("");

    if(!cLR->IsExist() && !cSL->IsExist() && !cRL->IsExist())	
    { 
        strDesc.Format(_T("1.2(DL+FP+(TL+CR+SH)) + 1.6(LL+((APH)EV+EH)) + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,1.2,cFP,1.2,cTL,1.2,cCR,1.2,cSH,1.2,cCSC,1.2,cCSS,1.6,cLL,1.6,cCSL,dEV,cEPV,1.6,cEPH,        dPSS,cPSorTs);
    }
    else //(cLR->IsExist()  cSL->IsExist()  cRL->IsExist())	
    {
        strDesc.Format(_T("1.2(DL+FP+(TL+CR+SH)) + 1.6(LL+((APH)EV+EH)) + 0.5LR + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,1.2,cFP,1.2,cTL,1.2,cCR,1.2,cSH,1.2,cCSC,1.2,cCSS,1.6,cLL,1.6,cCSL,dEV,cEPV,1.6,cEPH,0.5,cLR,dPSS,cPSorTs);

        strDesc.Format(_T("1.2(DL+FP+(TL+CR+SH)) + 1.6(LL+((APH)EV+EH)) + 0.5SL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,1.2,cFP,1.2,cTL,1.2,cCR,1.2,cSH,1.2,cCSC,1.2,cCSS,1.6,cLL,1.6,cCSL,dEV,cEPV,1.6,cEPH,0.5,cSL,dPSS,cPSorTs);

        strDesc.Format(_T("1.2(DL+FP+(TL+CR+SH)) + 1.6(LL+((APH)EV+EH)) + 0.5RL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,1.2,cFP,1.2,cTL,1.2,cCR,1.2,cSH,1.2,cCSC,1.2,cCSS,1.6,cLL,1.6,cCSL,dEV,cEPV,1.6,cEPH,0.5,cRL,dPSS,cPSorTs);
    }
}

void CLoadCombCtrl::Make_SubComb_EQ3_KCI_USD12(int nA, BOOL bStageCS, double dPSS, BOOL bExistLL,
                                               CombBase* cDL,  CombBase* cLL, CombBase* cLR, CombBase* cWL,  CombBase* cSL,  CombBase* cRL,  CombBase* cPS,  CombBase* cTS, 
                                               CombBase* cCSD, CombBase* cCSL,
                                               BOOL bSlcmSpecial, BOOL bSlcmVertical)
{
    // MQC:3269-Seungjun-20100330 Live Load 계수 수정.
    // KCI2012 p60. 3.3.2 소요강도 (2) 차고, 공공집회장소 및 L이 5.0kN/m2 이상인 모든 장소 이외에는
    // 식(3.3.3), 식(3.3.4) 및 식(3.3.5)에서 활하중 L에 대한 하중 계수를 0.5로 감소시킬 수 있다.  --> 적용안함.

    // (3.3.3) 1.2D + 1.6(LR or S or R) + (1.0L or 0.65W)
    int nActive = GetActiveIndex(nA, bSlcmSpecial, bSlcmVertical);

    CString   strPSorTS = bStageCS ? _T("TS") : _T("PS");
    CombBase* cPSorTs   = bStageCS ?    cTS   :    cPS;
    CString   strDesc   = _T("");

    if(!bExistLL && !cWL->IsExist())
    {
        strDesc.Format(_T("1.2DL + 1.6LR + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc, 1.2,cDL,1.2,cCSD,1.6,cLR,dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL + 1.6SL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc, 1.2,cDL,1.2,cCSD,1.6,cSL,dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL + 1.6RL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc, 1.2,cDL,1.2,cCSD,1.6,cRL,dPSS,cPSorTs);
    }
    else
    {
        strDesc.Format(_T("1.2DL + 1.6LR + 1.0LL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,1.6,cLR, 1.00,cLL,1.00,cCSL,dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL + 1.6LR + 0.65WL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,1.6,cLR, 0.65,cWL,          dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL + 1.6LR - 0.65WL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,1.6,cLR,-0.65,cWL,          dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL + 1.6SL + 1.0LL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,1.6,cSL, 1.00,cLL,1.00,cCSL,dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL + 1.6SL + 0.65WL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,1.6,cSL, 0.65,cWL,          dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL + 1.6SL - 0.65WL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,1.6,cSL,-0.65,cWL,          dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL + 1.6RL + 1.0LL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,1.6,cRL, 1.00,cLL,1.00,cCSL,dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL + 1.6RL + 0.65WL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,1.6,cRL, 0.65,cWL,          dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL + 1.6RL - 0.65WL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,1.6,cRL,-0.65,cWL,          dPSS,cPSorTs);
    }
}

void CLoadCombCtrl::Make_SubComb_EQ4_KCI_USD12(int nA, BOOL bStageCS, double dPSS, BOOL bExistLL,
                                               CombBase* cDL,  CombBase* cLL, CombBase* cLR, CombBase* cWL,  CombBase* cSL,  CombBase* cRL,  CombBase* cPS,  CombBase* cTS,
                                               CombBase* cCSD, CombBase* cCSL,
                                               BOOL bSlcmSpecial, BOOL bSlcmVertical)
{
    // MQC:3269-Seungjun-20100330 Live Load 계수 수정.
    // KCI2012 p60. 3.3.2 소요강도 (2) 차고, 공공집회장소 및 L이 5.0kN/m2 이상인 모든 장소 이외에는
    // 식(3.3.3), 식(3.3.4) 및 식(3.3.5)에서 활하중 L에 대한 하중 계수를 0.5로 감소시킬 수 있다. --> 적용안함.

    // (3.3.4) 1.2D + 1.3W + 1.0L +0.5(LR or S or R)
    int nActive = GetActiveIndex(nA, bSlcmSpecial, bSlcmVertical);

    CString   strPSorTS = bStageCS ? _T("TS") : _T("PS");
    CombBase* cPSorTs   = bStageCS ?    cTS   :    cPS;
    CString   strDesc   = _T("");

    if(!bExistLL && !cLR->IsExist() && !cSL->IsExist() && !cRL->IsExist())	
    {
        strDesc.Format(_T("1.2DL + 1.3WL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD, 1.3,cWL,dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL - 1.3WL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,-1.3,cWL,dPSS,cPSorTs);
    }
    else if(!cLR->IsExist() && !cSL->IsExist() && !cRL->IsExist() && !cRL->IsExist())	
    {		
        strDesc.Format(_T("1.2DL + 1.3WL + 1.0LL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD, 1.3,cWL,1.0,cLL,1.0,cCSL,dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL - 1.3WL + 1.0LL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,-1.3,cWL,1.0,cLL,1.0,cCSL,dPSS,cPSorTs);
    }
    else if(!bExistLL)
    {		
        strDesc.Format(_T("1.2DL + 1.3WL + 0.5LR + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD, 1.3,cWL,0.5,cLR,dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL - 1.3WL + 0.5LR + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,-1.3,cWL,0.5,cLR,dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL + 1.3WL + 0.5SL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD, 1.3,cWL,0.5,cSL,dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL - 1.3WL + 0.5SL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,-1.3,cWL,0.5,cSL,dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL + 1.3WL + 0.5RL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD, 1.3,cWL,0.5,cRL,dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL - 1.3WL + 0.5RL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,-1.3,cWL,0.5,cRL,dPSS,cPSorTs);
    }
    else
    {		
        strDesc.Format(_T("1.2DL + 1.3WL + 1.0LL + 0.5LR + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD, 1.3,cWL,1.0,cLL,1.0,cCSL,0.5,cLR,dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL - 1.3WL + 1.0LL + 0.5LR + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,-1.3,cWL,1.0,cLL,1.0,cCSL,0.5,cLR,dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL + 1.3WL + 1.0LL + 0.5SL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD, 1.3,cWL,1.0,cLL,1.0,cCSL,0.5,cSL,dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL - 1.3WL + 1.0LL + 0.5SL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,-1.3,cWL,1.0,cLL,1.0,cCSL,0.5,cSL,dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL + 1.3WL + 1.0LL + 0.5RL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD, 1.3,cWL,1.0,cLL,1.0,cCSL,0.5,cRL,dPSS,cPSorTs);

        strDesc.Format(_T("1.2DL - 1.3WL + 1.0LL + 0.5RL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,-1.3,cWL,1.0,cLL,1.0,cCSL,0.5,cRL,dPSS,cPSorTs);
    }
}

void CLoadCombCtrl::Make_SubComb_EQ5_KCI_USD12(int nA, BOOL bStageCS, double dPSS, BOOL bExistLL,
                                               CombBase* cDL,  CombBase* cLL,  CombBase* cEL, CombBase* cSL,  CombBase* cPS,  CombBase* cESP, CombBase* cTS,  CombBase* cCSD, 
                                               CombBase* cCSL, CombBase* cEPH, CombBase* cEPV,
                                               BOOL bSlcmSpecial, BOOL bSlcmVertical)
{
    // MQC:3269-Seungjun-20100330 Live Load 계수 수정.
    // KCI2012 p60. 3.3.2 소요강도 (2) 차고, 공공집회장소 및 L이 5.0kN/m2 이상인 모든 장소 이외에는
    // 식(3.3.3), 식(3.3.4) 및 식(3.3.5)에서 활하중 L에 대한 하중 계수를 0.5로 감소시킬 수 있다.  --> 적용안함.

    // (3.3.5) 1.2(D + Ev) + 1.0E + 1.0L + 0.2S +(1.0Eh or 0.5Eh)		 
    int nActive = GetActiveIndex(nA, bSlcmSpecial, bSlcmVertical);

    CString   csPSorTS = bStageCS ? _T("TS") : _T("PS");
    CombBase* cPSorTs  = bStageCS ?    cTS   :    cPS;

    BOOL bEPv = cEPV->IsExist();
    BOOL bEPh = cEPH->IsExist();
    if      (!bEPv && !bEPh) Make_SubComb_EQ5_KCI_USD12_Case1(nActive, bExistLL, cDL, cLL, cEL, cSL, cESP, cCSD, cCSL, cEPH, cEPV, cPSorTs, csPSorTS, dPSS, bSlcmSpecial);
    else if (!bEPv &&  bEPh) Make_SubComb_EQ5_KCI_USD12_Case2(nActive, bExistLL, cDL, cLL, cEL, cSL, cESP, cCSD, cCSL, cEPH, cEPV, cPSorTs, csPSorTS, dPSS, bSlcmSpecial);
    else if ( bEPv && !bEPh) Make_SubComb_EQ5_KCI_USD12_Case3(nActive, bExistLL, cDL, cLL, cEL, cSL, cESP, cCSD, cCSL, cEPH, cEPV, cPSorTs, csPSorTS, dPSS, bSlcmSpecial);
    else if ( bEPv &&  bEPh) Make_SubComb_EQ5_KCI_USD12_Case4(nActive, bExistLL, cDL, cLL, cEL, cSL, cESP, cCSD, cCSL, cEPH, cEPV, cPSorTs, csPSorTS, dPSS, bSlcmSpecial);
    else ASSERT(0);
}

void CLoadCombCtrl::Make_SubComb_EQ5_KCI_USD12_Case1(int nActive, BOOL bExistLL,
                                                     CombBase* cDL,  CombBase* cLL,  CombBase* cEL, CombBase* cSL,  
                                                     CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cEPH, CombBase* cEPV,
                                                     CombBase* cPSorTs, CString csPSorTS, double dPSS, BOOL bSlcmSpecial)
{
    const double dDLp12 = 1.20 + m_dSlcmSds * m_dSlcmSpecial; // for 1.2D + [(OMG)E + 0.2SDS*D]

    CString csDesc = _T("");
    if(!bExistLL && !cSL->IsExist())
    {
        if(!bSlcmSpecial)
        {
            csDesc.Format(_T("1.2DL + 1.0EL + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD, 1.0,cEL,  dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0EL + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD,-1.0,cEL,  dPSS,cPSorTs);

            csDesc.Format(_T("1.2DL + 1.0(SUF)ESP + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD, 1.0,cESP, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0(SUF)ESP + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD,-1.0,cESP, dPSS,cPSorTs);
        }
        else 
        {
            csDesc.Format(_T("1.2DL + 1.0(OMG)EL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	     Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD, 1.0,cEL,  dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL + 1.0(OMG)EL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	     Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD, 1.0,cEL,  dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0(OMG)EL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	     Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD,-1.0,cEL,  dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0(OMG)EL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	     Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD,-1.0,cEL,  dPSS,cPSorTs);

            csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	 Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD, 1.0,cESP, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	 Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD, 1.0,cESP, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	 Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD,-1.0,cESP, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	 Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD,-1.0,cESP, dPSS,cPSorTs);
        }
    }
    else if(!cSL->IsExist())
    {
        if(!bSlcmSpecial)
        {
            csDesc.Format(_T("1.2DL + 1.0EL + 1.0LL + (SFAC)%s"), csPSorTS);		    Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD, 1.0,cEL,  1.0,cLL,1.0,cCSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0EL + 1.0LL + (SFAC)%s"), csPSorTS);		    Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD,-1.0,cEL,  1.0,cLL,1.0,cCSL, dPSS,cPSorTs);

            csDesc.Format(_T("1.2DL + 1.0(SUF)ESP + 1.0LL + (SFAC)%s"), csPSorTS);		Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD, 1.0,cESP, 1.0,cLL,1.0,cCSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0(SUF)ESP + 1.0LL + (SFAC)%s"), csPSorTS);		Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD,-1.0,cESP, 1.0,cLL,1.0,cCSL, dPSS,cPSorTs);
        }
        else 
        {
            csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 1.0LL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	   Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD, 1.0,cEL,  1.0,cLL,1.0,cCSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 1.0LL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	   Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD, 1.0,cEL,  1.0,cLL,1.0,cCSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 1.0LL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD,-1.0,cEL,  1.0,cLL,1.0,cCSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 1.0LL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD,-1.0,cEL,  1.0,cLL,1.0,cCSL, dPSS,cPSorTs);

            csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 1.0LL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD, 1.0,cESP, 1.0,cLL,1.0,cCSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 1.0LL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD, 1.0,cESP, 1.0,cLL,1.0,cCSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 1.0LL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD,-1.0,cESP, 1.0,cLL,1.0,cCSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 1.0LL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD,-1.0,cESP, 1.0,cLL,1.0,cCSL, dPSS,cPSorTs);
        }
    }
    else if(!bExistLL)
    {
        if(!bSlcmSpecial)
        {
            csDesc.Format(_T("1.2DL + 1.0EL + 0.2SL + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD, 1.0,cEL,  0.2,cSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0EL + 0.2SL + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD,-1.0,cEL,  0.2,cSL, dPSS,cPSorTs);

            csDesc.Format(_T("1.2DL + 1.0(SUF)ESP + 0.2SL + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD, 1.0,cESP, 0.2,cSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0(SUF)ESP + 0.2SL + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD,-1.0,cESP, 0.2,cSL, dPSS,cPSorTs);
        }
        else 
        {	
            csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD, 1.0,cEL,  0.2,cSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD, 1.0,cEL,  0.2,cSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD,-1.0,cEL,  0.2,cSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD,-1.0,cEL,  0.2,cSL, dPSS,cPSorTs);

            csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD, 1.0,cESP, 0.2,cSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD, 1.0,cESP, 0.2,cSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD,-1.0,cESP, 0.2,cSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD,-1.0,cESP, 0.2,cSL, dPSS,cPSorTs);
        }
    }
    else
    {
        if(!bSlcmSpecial)
        {
            csDesc.Format(_T("1.2DL + 1.0EL + 1.0LL + 0.2SL + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD,  1.0,cEL, 1.0,cLL,1.0,cCSL, 0.2,cSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0EL + 1.0LL + 0.2SL + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD, -1.0,cEL, 1.0,cLL,1.0,cCSL, 0.2,cSL, dPSS,cPSorTs);

            csDesc.Format(_T("1.2DL + 1.0(SUF)ESP + 1.0LL + 0.2SL + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD,  1.0,cESP, 1.0,cLL,1.0,cCSL, 0.2,cSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0(SUF)ESP + 1.0LL + 0.2SL + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD, -1.0,cESP, 1.0,cLL,1.0,cCSL, 0.2,cSL, dPSS,cPSorTs);
        }
        else 
        {
            csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 1.0LL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD,  1.0,cEL,  1.0,cLL,1.0,cCSL, 0.2,cSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 1.0LL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD,  1.0,cEL,  1.0,cLL,1.0,cCSL, 0.2,cSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 1.0LL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD, -1.0,cEL,  1.0,cLL,1.0,cCSL, 0.2,cSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 1.0LL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD, -1.0,cEL,  1.0,cLL,1.0,cCSL, 0.2,cSL, dPSS,cPSorTs);

            csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD,  1.0,cESP, 1.0,cLL,1.0,cCSL, 0.2,cSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD,  1.0,cESP, 1.0,cLL,1.0,cCSL, 0.2,cSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD, -1.0,cESP, 1.0,cLL,1.0,cCSL, 0.2,cSL, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc,	dDLp12,cDL,dDLp12,cCSD, -1.0,cESP, 1.0,cLL,1.0,cCSL, 0.2,cSL, dPSS,cPSorTs);
        }
    }
}

void CLoadCombCtrl::Make_SubComb_EQ5_KCI_USD12_Case2(int nActive, BOOL bExistLL,
                                                     CombBase* cDL,  CombBase* cLL,  CombBase* cEL, CombBase* cSL,  
                                                     CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cEPH, CombBase* cEPV,
                                                     CombBase* cPSorTs, CString csPSorTS, double dPSS, BOOL bSlcmSpecial)
{
    const double dDLp12 = 1.20 + m_dSlcmSds * m_dSlcmSpecial; // for 1.2D + [(OMG)E + 0.2SDS*D]

    CString csDesc = _T("");
    if(!bExistLL && !cSL->IsExist())
    {
        if(!bSlcmSpecial)
        {
            csDesc.Format(_T("1.2DL + 1.0EL + 1.0EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD, 1.0,cEL,  1.0,cEPH, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0EL + 1.0EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD,-1.0,cEL,  1.0,cEPH, dPSS,cPSorTs);

            csDesc.Format(_T("1.2DL + 1.0(SUF)ESP + 1.0EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD, 1.0,cESP, 1.0,cEPH, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0(SUF)ESP + 1.0EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD,-1.0,cESP, 1.0,cEPH, dPSS,cPSorTs);

            csDesc.Format(_T("1.2DL + 1.0EL + 0.5EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD, 1.0,cEL,  0.5,cEPH, dPSS,cPSorTs);
            csDesc.Format(_T("1.2DL - 1.0EL + 0.5EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD,-1.0,cEL,  0.5,cEPH, dPSS,cPSorTs);

            csDesc.Format(_T("1.2DL + 1.0(SUF)ESP + 0.5EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD, 1.0,cESP, 0.5,cEPH, dPSS,cPSorTs);		
            csDesc.Format(_T("1.2DL - 1.0(SUF)ESP + 0.5EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc,	1.2,cDL,1.2,cCSD,-1.0,cESP, 0.5,cEPH, dPSS,cPSorTs);
        }
        else 
		{
			csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	    Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cEL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	    Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cEL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	    Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cEL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	    Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cEL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cESP, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cESP, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cESP, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cESP, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	    Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cEL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	    Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cEL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	    Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cEL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	    Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cEL, 0.5, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cESP, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cESP, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cESP, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cESP, 0.5, cEPH, dPSS, cPSorTs);
		}
	}
	else if ( !cSL->IsExist() )
	{
		if ( !bSlcmSpecial )
		{
			csDesc.Format(_T("1.2DL + 1.0EL + 1.0LL + 1.0EH + (SFAC)%s"), csPSorTS);		      Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0EL + 1.0LL + 1.0EH + (SFAC)%s"), csPSorTS);		      Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2DL + 1.0(SUF)ESP + 1.0LL + 1.0EH + (SFAC)%s"), csPSorTS);		Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)ESP + 1.0LL + 1.0EH + (SFAC)%s"), csPSorTS);		Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2DL + 1.0EL + 1.0LL + 0.5EH + (SFAC)%s"), csPSorTS);		      Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0EL + 1.0LL + 0.5EH + (SFAC)%s"), csPSorTS);		      Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2DL + 1.0(SUF)ESP + 1.0LL + 0.5EH + (SFAC)%s"), csPSorTS);		Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)ESP + 1.0LL + 0.5EH + (SFAC)%s"), csPSorTS);		Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);
		}
		else
		{
			csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 1.0LL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	   Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 1.0LL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	   Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 1.0LL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	   Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 1.0LL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	   Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 1.0LL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 1.0LL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 1.0LL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 1.0LL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 1.0LL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 1.0LL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 1.0LL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 1.0LL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 1.0LL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 1.0LL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 1.0LL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 1.0LL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);
		}
	}
	else if ( !bExistLL )
	{
		if ( !bSlcmSpecial )
		{
			csDesc.Format(_T("1.2DL + 1.0EL + 0.2SL + 1.0EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.0, cEL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0EL + 0.2SL + 1.0EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, -1.0, cEL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2DL + 1.0(SUF)ESP + 0.2SL + 1.0EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.0, cESP, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)ESP + 0.2SL + 1.0EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, -1.0, cESP, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2DL + 1.0EL + 0.2SL + 0.5EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.0, cEL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0EL + 0.2SL + 0.5EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, -1.0, cEL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2DL + 1.0(SUF)ESP + 0.2SL + 0.5EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.0, cESP, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)ESP + 0.2SL + 0.5EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, -1.0, cESP, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
		}
		else
		{
			csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cEL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cEL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cEL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cEL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cESP, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cESP, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cESP, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cESP, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cEL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cEL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cEL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cEL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cESP, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cESP, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cESP, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cESP, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
		}
	}
	else
	{
		if ( !bSlcmSpecial )
		{
			csDesc.Format(_T("1.2DL + 1.0EL + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0EL + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2DL + 1.0(SUF)ESP + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)ESP + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2DL + 1.0EL + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0EL + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2DL + 1.0(SUF)ESP + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)ESP + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
		}
		else
		{
			csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL + 1.0(OMG)EL + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(OMG)EL + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL + 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2DL - 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
		}
	}
}

void CLoadCombCtrl::Make_SubComb_EQ5_KCI_USD12_Case3(int nActive, BOOL bExistLL,
                                                     CombBase* cDL,  CombBase* cLL,  CombBase* cEL, CombBase* cSL,  
                                                     CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cEPH, CombBase* cEPV,
                                                     CombBase* cPSorTs, CString csPSorTS, double dPSS, BOOL bSlcmSpecial)
{
    double dDLp12 = 1.20 + m_dSlcmSds * m_dSlcmSpecial; // for 1.2D + [(OMG)E + 0.2SDS*D]

	CString csDesc = _T("");
	if ( !bExistLL && !cSL->IsExist() )
	{
		if ( !bSlcmSpecial )
		{
			csDesc.Format(_T("1.2(DL+EV) + 1.0EL + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cEL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0EL + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cEL, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)ESP + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cESP, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)ESP + (SFAC)%s"), csPSorTS);	Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cESP, dPSS, cPSorTs);
		}
		else
		{
			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	    Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	    Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	    Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	    Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, dPSS, cPSorTs);
		}
	}
	else if ( !cSL->IsExist() )
	{
		if ( !bSlcmSpecial )
		{
			csDesc.Format(_T("1.2(DL+EV) + 1.0EL + 1.0LL + (SFAC)%s"), csPSorTS);	    Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cEL, 1.0, cLL, 1.0, cCSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0EL + 1.0LL + (SFAC)%s"), csPSorTS);	    Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cEL, 1.0, cLL, 1.0, cCSL, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)ESP + 1.0LL + (SFAC)%s"), csPSorTS);	Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cESP, 1.0, cLL, 1.0, cCSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)ESP + 1.0LL + (SFAC)%s"), csPSorTS);	Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cESP, 1.0, cLL, 1.0, cCSL, dPSS, cPSorTs);
		}
		else
		{
			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 1.0LL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);		Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 1.0, cLL, 1.0, cCSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 1.0LL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);		Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 1.0, cLL, 1.0, cCSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 1.0LL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);		Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 1.0, cLL, 1.0, cCSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 1.0LL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);		Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 1.0, cLL, 1.0, cCSL, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 1.0LL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 1.0, cLL, 1.0, cCSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 1.0LL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 1.0, cLL, 1.0, cCSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 1.0LL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 1.0, cLL, 1.0, cCSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 1.0LL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 1.0, cLL, 1.0, cCSL, dPSS, cPSorTs);
		}
	}
	else if ( !bExistLL )
	{
		if ( !bSlcmSpecial )
		{
			csDesc.Format(_T("1.2(DL+EV) + 1.0EL + 0.2SL + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cEL, 0.2, cSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0EL + 0.2SL + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cEL, 0.2, cSL, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)ESP + 0.2SL + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cESP, 0.2, cSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)ESP + 0.2SL + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cESP, 0.2, cSL, dPSS, cPSorTs);
		}
		else
		{
			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 0.2, cSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 0.2, cSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 0.2, cSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 0.2, cSL, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 0.2, cSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 0.2, cSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 0.2, cSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 0.2, cSL, dPSS, cPSorTs);
		}
	}
	else
	{
		if ( !bSlcmSpecial )
		{
			csDesc.Format(_T("1.2(DL+EV) + 1.0EL + 1.0LL + 0.2SL + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0EL + 1.0LL + 0.2SL + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)ESP + 1.0LL + 0.2SL + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)ESP + 1.0LL + 0.2SL + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, dPSS, cPSorTs);
		}
		else
		{
			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 1.0LL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 1.0LL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 1.0LL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 1.0LL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, dPSS, cPSorTs);
		}
	}
}

void CLoadCombCtrl::Make_SubComb_EQ5_KCI_USD12_Case4(int nActive, BOOL bExistLL,
                                                     CombBase* cDL,  CombBase* cLL,  CombBase* cEL, CombBase* cSL,  
                                                     CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cEPH, CombBase* cEPV,
                                                     CombBase* cPSorTs, CString csPSorTS, double dPSS, BOOL bSlcmSpecial)
{
    const double dDLp12 = 1.20 + m_dSlcmSds * m_dSlcmSpecial; // for 1.2D + [(OMG)E + 0.2SDS*D]

	CString csDesc = _T("");
	if ( !bExistLL && !cSL->IsExist() )
	{
		if ( !bSlcmSpecial )
		{
			csDesc.Format(_T("1.2(DL+EV) + 1.0EL + 1.0EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cEL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0EL + 1.0EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cEL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)ESP + 1.0EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cESP, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)ESP + 1.0EH + (SFAC)%s"), csPSorTS);	Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cESP, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0EL + 0.5EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cEL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0EL + 0.5EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cEL, 0.5, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)ESP + 0.5EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cESP, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)ESP + 0.5EH + (SFAC)%s"), csPSorTS);	Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cESP, 0.5, cEPH, dPSS, cPSorTs);
		}
		else
		{
			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	    Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	    Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	    Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	    Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	    Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	    Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	    Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	    Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 0.5, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);	Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 0.5, cEPH, dPSS, cPSorTs);
		}
	}
	else if ( !cSL->IsExist() )
	{
		if ( !bSlcmSpecial )
		{
			csDesc.Format(_T("1.2(DL+EV) + 1.0EL + 1.0LL + 1.0EH + (SFAC)%s"), csPSorTS);		    Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0EL + 1.0LL + 1.0EH + (SFAC)%s"), csPSorTS);		    Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)ESP + 1.0LL + 1.0EH + (SFAC)%s"), csPSorTS);		Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)ESP + 1.0LL + 1.0EH + (SFAC)%s"), csPSorTS);		Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0EL + 1.0LL + 0.5EH + (SFAC)%s"), csPSorTS);		    Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0EL + 1.0LL + 0.5EH + (SFAC)%s"), csPSorTS);		    Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)ESP + 1.0LL + 0.5EH + (SFAC)%s"), csPSorTS);		Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)ESP + 1.0LL + 0.5EH + (SFAC)%s"), csPSorTS);		Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);
		}
		else
		{
			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 1.0LL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);		Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 1.0LL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);		Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 1.0LL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);		Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 1.0LL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);		Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 1.0LL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 1.0LL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 1.0LL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 1.0LL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 1.0LL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 1.0LL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 1.0LL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 1.0LL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 1.0LL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 1.0LL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 1.0LL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 1.0LL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.5, cEPH, dPSS, cPSorTs);
		}
	}
	else if ( !bExistLL )
	{
		if ( !bSlcmSpecial )
		{
			csDesc.Format(_T("1.2(DL+EV) + 1.0EL + 0.2SL + 1.0EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cEL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0EL + 0.2SL + 1.0EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cEL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)ESP + 0.2SL + 1.0EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cESP, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)ESP + 0.2SL + 1.0EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cESP, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0EL + 0.2SL + 0.5EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cEL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0EL + 0.2SL + 0.5EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cEL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)ESP + 0.2SL + 0.5EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cESP, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)ESP + 0.2SL + 0.5EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cESP, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
		}
		else
		{
			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
		}
	}
	else
	{
		if ( !bSlcmSpecial )
		{
			csDesc.Format(_T("1.2(DL+EV) + 1.0EL + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0EL + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)ESP + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)ESP + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0EL + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0EL + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s"), csPSorTS);       Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)ESP + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)ESP + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s"), csPSorTS); Set_LCB(nActive, csDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
		}
		else
		{
			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + 1.0EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 1.0, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(OMG)EL + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(OMG)EL + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS);       Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cEL, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);

			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) + 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, 1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
			csDesc.Format(_T("1.2(DL+EV) - 1.0(SUF)(OMG)ESP + 1.0LL + 0.2SL + 0.5EH + (SFAC)%s + (SPC)(SDS)DL"), csPSorTS); Set_LCB(nActive, csDesc, dDLp12, cDL, dDLp12, cCSD, 1.2, cEPV, -1.0, cESP, 1.0, cLL, 1.0, cCSL, 0.2, cSL, 0.5, cEPH, dPSS, cPSorTs);
		}
	}
}

void CLoadCombCtrl::Make_SubComb_EQ6_KCI_USD12(int nA, BOOL bStageCS, double dPSS, double dEV,
                                               CombBase* cDL,  CombBase* cLL,  CombBase* cLR, CombBase* cSL,  CombBase* cRL,  CombBase* cTL,  CombBase* cSH,  CombBase* cCR, 
                                               CombBase* cFP,  CombBase* cPS,  CombBase* cTS, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cEPH, 
                                               CombBase* cEPV,
                                               BOOL bSlcmSpecial, BOOL bSlcmVertical)
{
    // (3.3.6) 1.2(D+F+T) +1.6(L+Hv) + 0.8Hh + 0.5(LR or S or R)
    int nActive = GetActiveIndex(nA, bSlcmSpecial, bSlcmVertical);

    CString   strPSorTS = bStageCS ? _T("TS") : _T("PS");
    CombBase* cPSorTs   = bStageCS ?    cTS   :    cPS;
    CString   strDesc   = _T("");

    if(cEPH->IsExist())
    {
        strDesc.Format(_T("1.2(DL+FP+(TL+CR+SH)) + 1.6(LL+(APH)EV) + 0.8EH + 0.5LR + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,1.2,cFP,1.2,cTL,1.2,cCR,1.2,cSH,1.2,cCSC,1.2,cCSS,1.6,cLL,1.6,cCSL,dEV,cEPV,0.8,cEPH,0.5,cLR,dPSS,cPSorTs);

        strDesc.Format(_T("1.2(DL+FP+(TL+CR+SH)) + 1.6(LL+(APH)EV) + 0.8EH + 0.5SL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,1.2,cFP,1.2,cTL,1.2,cCR,1.2,cSH,1.2,cCSC,1.2,cCSS,1.6,cLL,1.6,cCSL,dEV,cEPV,0.8,cEPH,0.5,cSL,dPSS,cPSorTs);

        strDesc.Format(_T("1.2(DL+FP+(TL+CR+SH)) + 1.6(LL+(APH)EV) + 0.8EH + 0.5RL + (SFAC)%s"), strPSorTS);
        Set_LCB(nActive, strDesc,	1.2,cDL,1.2,cCSD,1.2,cFP,1.2,cTL,1.2,cCR,1.2,cSH,1.2,cCSC,1.2,cCSS,1.6,cLL,1.6,cCSL,dEV,cEPV,0.8,cEPH,0.5,cRL,dPSS,cPSorTs);
    }
}

void CLoadCombCtrl::Make_SubComb_EQ7_KCI_USD12(int nA, BOOL bStageCS, double dPSS,
                                               CombBase* cDL,  CombBase* cWL, CombBase* cPS,  CombBase* cTS, CombBase* cCSD, CombBase* cEPH, CombBase* cEPV,
                                               BOOL bSlcmSpecial, BOOL bSlcmVertical)
{
    // (3.3.7) 0.9(D + Ev) + 1.3W + (1.6Hh or 0.8Hh)
    int nActive = GetActiveIndex(nA, bSlcmSpecial, bSlcmVertical);

    CString   strPSorTS = bStageCS ? _T("TS") : _T("PS");
    CombBase* cPSorTs   = bStageCS ?    cTS   :    cPS;
    CString   strDesc   = _T("");

    // by GAY. ('13.09.12). MQC:6377. 토압 포함 하중조합 생성방식 변경.
    BOOL bWL = cWL->IsExist();
    BOOL bEH = cEPH->IsExist();

    if (bWL)
    {
        if (bEH)
        {
            strDesc.Format(_T("0.9(DL+EV) + 1.3WL + 1.6EH + (SFAC)%s"), strPSorTS);
            Set_LCB(nActive, strDesc,	0.9,cDL,0.9,cCSD,0.9,cEPV, 1.3,cWL,1.6,cEPH,dPSS,cPSorTs);

            strDesc.Format(_T("0.9(DL+EV) - 1.3WL + 1.6EH + (SFAC)%s"), strPSorTS);
            Set_LCB(nActive, strDesc, 0.9,cDL,0.9,cCSD,0.9,cEPV,-1.3,cWL,1.6,cEPH,dPSS,cPSorTs);

            strDesc.Format(_T("0.9(DL+EV) + 1.3WL + 0.8EH + (SFAC)%s"), strPSorTS);
            Set_LCB(nActive, strDesc,	0.9,cDL,0.9,cCSD,0.9,cEPV, 1.3,cWL,0.8,cEPH,dPSS,cPSorTs);

            strDesc.Format(_T("0.9(DL+EV) - 1.3WL + 0.8EH + (SFAC)%s"), strPSorTS);
            Set_LCB(nActive, strDesc,	0.9,cDL,0.9,cCSD,0.9,cEPV,-1.3,cWL,0.8,cEPH,dPSS,cPSorTs);
        }
        else
        {
            strDesc.Format(_T("0.9(DL+EV) + 1.3WL + (SFAC)%s"), strPSorTS);
            Set_LCB(nActive, strDesc,	0.9,cDL,0.9,cCSD,0.9,cEPV, 1.3,cWL,dPSS,cPSorTs);

            strDesc.Format(_T("0.9(DL+EV) - 1.3WL + (SFAC)%s"), strPSorTS);
            Set_LCB(nActive, strDesc, 0.9,cDL,0.9,cCSD,0.9,cEPV,-1.3,cWL,dPSS,cPSorTs);			
        }
    }
    else
    {
        if (bEH)
        {
            strDesc.Format(_T("0.9(DL+EV) + 1.6EH + (SFAC)%s"), strPSorTS);
            Set_LCB(nActive, strDesc,	0.9,cDL,0.9,cCSD,0.9,cEPV,1.6,cEPH,dPSS,cPSorTs);

            strDesc.Format(_T("0.9(DL+EV) + 0.8EH + (SFAC)%s"), strPSorTS);
            Set_LCB(nActive, strDesc,	0.9,cDL,0.9,cCSD,0.9,cEPV,0.8,cEPH,dPSS,cPSorTs);
        }
        else
        {
            strDesc.Format(_T("0.9(DL+EV) + (SFAC)%s"), strPSorTS);
            Set_LCB(nActive, strDesc,	0.9,cDL,0.9,cCSD,0.9,cEPV, dPSS,cPSorTs);
        }
    }
}

void CLoadCombCtrl::Make_SubComb_EQ8_KCI_USD12(int nA, BOOL bStageCS, double dPSS,
                                               CombBase* cDL,  CombBase* cEL, CombBase* cPS,  CombBase* cESP, CombBase* cTS,  CombBase* cCSD, CombBase* cEPH, CombBase* cEPV,
                                               BOOL bSlcmSpecial, BOOL bSlcmVertical)

{
    // (3.3.8) 0.9(D + Ev) + 1.0E + (1.0Hh or 0.5Hh)
    int nActive = GetActiveIndex(nA, bSlcmSpecial, bSlcmVertical);

    CString   strPSorTS = bStageCS ? _T("TS") : _T("PS");
    CombBase* cPSorTs   = bStageCS ?    cTS   :    cPS;
    CString   strDesc   = _T("");

    const double dDLm9	= 0.90 - m_dSlcmSds * m_dSlcmSpecial; // for 0.9D + [(OMG)E - 0.2SDS*D]

    BOOL bEL = (!cEL->IsExist() && !cESP->IsExist()) ? FALSE : TRUE;
    BOOL bEH = cEPH->IsExist();

    // by GAY. ('13.09.12). MQC:6377. 토압 포함 하중조합 생성방식 변경.
    if (bEL)
    {
        if (bEH)
        {
            if (!bSlcmSpecial)
            {
                strDesc.Format(_T("0.9(DL+EV) + 1.0EL + 1.0EH + (SFAC)%s"), strPSorTS);       Set_LCB(nActive, strDesc, 0.9,cDL,0.9,cCSD,0.9,cEPV, 1.0,cEL,  1.0,cEPH,dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) - 1.0EL + 1.0EH + (SFAC)%s"), strPSorTS);       Set_LCB(nActive, strDesc, 0.9,cDL,0.9,cCSD,0.9,cEPV,-1.0,cEL,  1.0,cEPH,dPSS,cPSorTs);

                strDesc.Format(_T("0.9(DL+EV) + 1.0(SUF)ESP + 1.0EH + (SFAC)%s"), strPSorTS); Set_LCB(nActive, strDesc, 0.9,cDL,0.9,cCSD,0.9,cEPV, 1.0,cESP, 1.0,cEPH,dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) - 1.0(SUF)ESP + 1.0EH + (SFAC)%s"), strPSorTS); Set_LCB(nActive, strDesc, 0.9,cDL,0.9,cCSD,0.9,cEPV,-1.0,cESP, 1.0,cEPH,dPSS,cPSorTs);

                strDesc.Format(_T("0.9(DL+EV) + 1.0EL + 0.5EH + (SFAC)%s"), strPSorTS);       Set_LCB(nActive, strDesc,	0.9,cDL,0.9,cCSD,0.9,cEPV, 1.0,cEL,  0.5,cEPH,dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) - 1.0EL + 0.5EH + (SFAC)%s"), strPSorTS);       Set_LCB(nActive, strDesc,	0.9,cDL,0.9,cCSD,0.9,cEPV,-1.0,cEL,  0.5,cEPH,dPSS,cPSorTs);

                strDesc.Format(_T("0.9(DL+EV) + 1.0(SUF)ESP + 0.5EH + (SFAC)%s"), strPSorTS); Set_LCB(nActive, strDesc, 0.9,cDL,0.9,cCSD,0.9,cEPV, 1.0,cESP, 0.5,cEPH,dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) - 1.0(SUF)ESP + 0.5EH + (SFAC)%s"), strPSorTS); Set_LCB(nActive, strDesc, 0.9,cDL,0.9,cCSD,0.9,cEPV,-1.0,cESP, 0.5,cEPH,dPSS,cPSorTs);
            }
            else
            {
                strDesc.Format(_T("0.9(DL+EV) + 1.0(OMG)EL + 1.0EH + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS);       Set_LCB(nActive, strDesc,	dDLm9,cDL,dDLm9,cCSD,0.9,cEPV, 1.0,cEL,  1.0,cEPH,dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) + 1.0(OMG)EL + 1.0EH + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS);       Set_LCB(nActive, strDesc,	dDLm9,cDL,dDLm9,cCSD,0.9,cEPV, 1.0,cEL,  1.0,cEPH,dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) - 1.0(OMG)EL + 1.0EH + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS);       Set_LCB(nActive, strDesc,	dDLm9,cDL,dDLm9,cCSD,0.9,cEPV,-1.0,cEL,  1.0,cEPH,dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) - 1.0(OMG)EL + 1.0EH + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS);       Set_LCB(nActive, strDesc,	dDLm9,cDL,dDLm9,cCSD,0.9,cEPV,-1.0,cEL,  1.0,cEPH,dPSS,cPSorTs);

                strDesc.Format(_T("0.9(DL+EV) + 1.0(SUF)(OMG)ESP + 1.0EH + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS); Set_LCB(nActive, strDesc, dDLm9,cDL,dDLm9,cCSD,0.9,cEPV, 1.0,cESP, 1.0,cEPH,dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) + 1.0(SUF)(OMG)ESP + 1.0EH + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS); Set_LCB(nActive, strDesc, dDLm9,cDL,dDLm9,cCSD,0.9,cEPV, 1.0,cESP, 1.0,cEPH,dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) - 1.0(SUF)(OMG)ESP + 1.0EH + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS); Set_LCB(nActive, strDesc, dDLm9,cDL,dDLm9,cCSD,0.9,cEPV,-1.0,cESP, 1.0,cEPH,dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) - 1.0(SUF)(OMG)ESP + 1.0EH + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS); Set_LCB(nActive, strDesc, dDLm9,cDL,dDLm9,cCSD,0.9,cEPV,-1.0,cESP, 1.0,cEPH,dPSS,cPSorTs);

                strDesc.Format(_T("0.9(DL+EV) + 1.0(OMG)EL + 0.5EH + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS);       Set_LCB(nActive, strDesc,	dDLm9,cDL,dDLm9,cCSD,0.9,cEPV, 1.0,cEL,  0.5,cEPH,dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) + 1.0(OMG)EL + 0.5EH + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS);       Set_LCB(nActive, strDesc,	dDLm9,cDL,dDLm9,cCSD,0.9,cEPV, 1.0,cEL,  0.5,cEPH,dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) - 1.0(OMG)EL + 0.5EH + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS);       Set_LCB(nActive, strDesc,	dDLm9,cDL,dDLm9,cCSD,0.9,cEPV,-1.0,cEL,  0.5,cEPH,dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) - 1.0(OMG)EL + 0.5EH + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS);       Set_LCB(nActive, strDesc,	dDLm9,cDL,dDLm9,cCSD,0.9,cEPV,-1.0,cEL,  0.5,cEPH,dPSS,cPSorTs);

                strDesc.Format(_T("0.9(DL+EV) + 1.0(SUF)(OMG)ESP + 0.5EH + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS); Set_LCB(nActive, strDesc, dDLm9,cDL,dDLm9,cCSD,0.9,cEPV, 1.0,cESP, 0.5,cEPH,dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) + 1.0(SUF)(OMG)ESP + 0.5EH + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS); Set_LCB(nActive, strDesc, dDLm9,cDL,dDLm9,cCSD,0.9,cEPV, 1.0,cESP, 0.5,cEPH,dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) - 1.0(SUF)(OMG)ESP + 0.5EH + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS); Set_LCB(nActive, strDesc, dDLm9,cDL,dDLm9,cCSD,0.9,cEPV,-1.0,cESP, 0.5,cEPH,dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) - 1.0(SUF)(OMG)ESP + 0.5EH + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS); Set_LCB(nActive, strDesc, dDLm9,cDL,dDLm9,cCSD,0.9,cEPV,-1.0,cESP, 0.5,cEPH,dPSS,cPSorTs);
            }
        }
        else
        {
            if (!bSlcmSpecial)
            {
                strDesc.Format(_T("0.9(DL+EV) + 1.0EL + (SFAC)%s"), strPSorTS);       Set_LCB(nActive, strDesc, 0.9,cDL,0.9,cCSD,0.9,cEPV, 1.0,cEL,  dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) - 1.0EL + (SFAC)%s"), strPSorTS);       Set_LCB(nActive, strDesc, 0.9,cDL,0.9,cCSD,0.9,cEPV,-1.0,cEL,  dPSS,cPSorTs);

                strDesc.Format(_T("0.9(DL+EV) + 1.0(SUF)ESP + (SFAC)%s"), strPSorTS); Set_LCB(nActive, strDesc, 0.9,cDL,0.9,cCSD,0.9,cEPV, 1.0,cESP, dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) - 1.0(SUF)ESP + (SFAC)%s"), strPSorTS); Set_LCB(nActive, strDesc, 0.9,cDL,0.9,cCSD,0.9,cEPV,-1.0,cESP, dPSS,cPSorTs);
            }
            else
            {
                strDesc.Format(_T("0.9(DL+EV) + 1.0(OMG)EL + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS);       Set_LCB(nActive, strDesc,	dDLm9,cDL,dDLm9,cCSD,0.9,cEPV, 1.0,cEL,  dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) + 1.0(OMG)EL + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS);       Set_LCB(nActive, strDesc,	dDLm9,cDL,dDLm9,cCSD,0.9,cEPV, 1.0,cEL,  dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) - 1.0(OMG)EL + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS);       Set_LCB(nActive, strDesc,	dDLm9,cDL,dDLm9,cCSD,0.9,cEPV,-1.0,cEL,  dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) - 1.0(OMG)EL + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS);       Set_LCB(nActive, strDesc,	dDLm9,cDL,dDLm9,cCSD,0.9,cEPV,-1.0,cEL,  dPSS,cPSorTs);

                strDesc.Format(_T("0.9(DL+EV) + 1.0(SUF)(OMG)ESP + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS); Set_LCB(nActive, strDesc, dDLm9,cDL,dDLm9,cCSD,0.9,cEPV, 1.0,cESP, dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) + 1.0(SUF)(OMG)ESP + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS); Set_LCB(nActive, strDesc, dDLm9,cDL,dDLm9,cCSD,0.9,cEPV, 1.0,cESP, dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) - 1.0(SUF)(OMG)ESP + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS); Set_LCB(nActive, strDesc, dDLm9,cDL,dDLm9,cCSD,0.9,cEPV,-1.0,cESP, dPSS,cPSorTs);
                strDesc.Format(_T("0.9(DL+EV) - 1.0(SUF)(OMG)ESP + (SFAC)%s - (SPC)(SDS)DL"), strPSorTS); Set_LCB(nActive, strDesc, dDLm9,cDL,dDLm9,cCSD,0.9,cEPV,-1.0,cESP, dPSS,cPSorTs);
            }
        }
    }
    else
    {
        if (bEH)
        {
            strDesc.Format(_T("0.9(DL+EV) + 1.0EH + (SFAC)%s"), strPSorTS); Set_LCB(nActive, strDesc,	0.9,cDL,0.9,cCSD, 0.9,cEPV, 1.0,cEPH, dPSS,cPSorTs);
            strDesc.Format(_T("0.9(DL+EV) + 0.5EH + (SFAC)%s"), strPSorTS); Set_LCB(nActive, strDesc,	0.9,cDL,0.9,cCSD, 0.9,cEPV, 0.5,cEPH, dPSS,cPSorTs);
        }
        else
        {
            strDesc.Format(_T("0.9(DL+EV) + (SFAC)%s"), strPSorTS); Set_LCB(nActive, strDesc,	0.9,cDL,0.9,cCSD, 0.9,cEPV, dPSS,cPSorTs);
            strDesc.Format(_T("0.9(DL+EV) + (SFAC)%s"), strPSorTS); Set_LCB(nActive, strDesc,	0.9,cDL,0.9,cCSD, 0.9,cEPV, dPSS,cPSorTs);
        }
    }
}

void CLoadCombCtrl::Make_SubComb_KCI_USD12_Serv(int nA, BOOL bExistLL, const CString &strPS, CombBase* cDL, CombBase* cCSD, CombBase* cFP,
                                                CombBase* cLL,  CombBase* cCSL, CombBase* cTL, CombBase* cEPV, CombBase* cEPH,
                                                CombBase* cLR, CombBase* cSL,  CombBase* cRL,
                                                CombBase* cWL, CombBase* cEL,  CombBase* cESP, CombBase* cPSorTS)

{
    CString strDesc = _T("");
    // [3.3.1]	
    strDesc.Format(_T("(DL+FP) + %s"), strPS);
    Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cFP, 1.0,cPSorTS);

    double dAlphaH = m_dAlpha_H;
    BOOL bLRSeries = (!cLR->IsExist() && !cSL->IsExist() && !cRL->IsExist())? FALSE : TRUE;
    CString strCaseLR[3] = { _T("LR"), _T("SL"), _T("RL") };
    CombBase* cLoadLR[3] = { cLR, cSL, cRL };

    // [3.3.2]
    if(bLRSeries)
    {
        for (int i=0; i<3; ++i)
        {
            strDesc.Format(_T("(DL+FP+TL) + (LL+(APH)EV+EH) + %s + %s"), strCaseLR[i], strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cFP, 1.0,cTL, 1.0,cLL,1.0,cCSL, dAlphaH,cEPV, 1.0,cEPH, 1.0,cLoadLR[i], 1.0,cPSorTS);
        }		
    }
    else
    {
        strDesc.Format(_T("(DL+FP+TL) + (LL+(APH)EV+EH) + %s"), strPS);
        Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cFP, 1.0,cTL, 1.0,cLL,1.0,cCSL, dAlphaH,cEPV, 1.0,cEPH, 1.0,cPSorTS);
    }

    // [3.3.3]
    if(!bExistLL && !cWL->IsExist())
    {
        for (int i=0; i<3; ++i)
        {
            strDesc.Format(_T("DL + %s + %s"), strCaseLR[i], strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cLoadLR[i], 1.0,cPSorTS);
        }    
    }
    else
    {
        // [3.3.2]와 같은 조합이 생성되지 않도록 하기 위한 조건 처리.
        BOOL bGenerate = !cFP->IsExist() && !cTL->IsExist() && !cEPV->IsExist() & !cEPH->IsExist() ? FALSE : TRUE;

        for (int i=0; i<3; ++i)
        {
            if (bGenerate)
            {
                strDesc.Format(_T("DL + %s + LL + %s"), strCaseLR[i], strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cLoadLR[i], 1.0,cLL,1.0,cCSL, 1.0,cPSorTS);
            }
            strDesc.Format(_T("DL + %s + WL + %s"), strCaseLR[i], strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cLoadLR[i], 1.0,cWL, 1.0,cPSorTS);
            strDesc.Format(_T("DL + %s - WL + %s"), strCaseLR[i], strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cLoadLR[i],-1.0,cWL, 1.0,cPSorTS);
        }
    }

    // [3.3.4]
    if (bExistLL)
    {
        if (bLRSeries)
        {
            for (int i=0; i<3; ++i)
            {
                strDesc.Format(_T("DL + WL + LL + %s + %s"), strCaseLR[i], strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cWL, 1.0,cLL,1.0,cCSL, 1.0,cLoadLR[i], 1.0,cPSorTS);
                strDesc.Format(_T("DL - WL + LL + %s + %s"), strCaseLR[i], strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD,-1.0,cWL, 1.0,cLL,1.0,cCSL, 1.0,cLoadLR[i], 1.0,cPSorTS);
            }			
        }
        else
        {
            strDesc.Format(_T("DL + WL + LL + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cWL, 1.0,cLL,1.0,cCSL, 1.0,cPSorTS);
            strDesc.Format(_T("DL - WL + LL + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD,-1.0,cWL, 1.0,cLL,1.0,cCSL, 1.0,cPSorTS);			
        }
    }
    else
    {
        if (bLRSeries)
        {
            for (int i=0; i<3; ++i)
            {
                strDesc.Format(_T("DL + WL + %s + %s"), strCaseLR[i], strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cWL, 1.0,cLoadLR[i], 1.0,cPSorTS);
                strDesc.Format(_T("DL - WL + %s + %s"), strCaseLR[i], strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD,-1.0,cWL, 1.0,cLoadLR[i], 1.0,cPSorTS);
            }			
        }
        else
        {
            strDesc.Format(_T("DL + WL + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cWL, 1.0,cPSorTS);
            strDesc.Format(_T("DL - WL + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD,-1.0,cWL, 1.0,cPSorTS);			
        }
    }

    // [3.3.5]
    BOOL bSL = cSL->IsExist();
    BOOL bEH = cEPH->IsExist();
    if (bExistLL)
    {
        if (bEH)
        {
            if (bSL)
            {
                strDesc.Format(_T("(DL+EV) + 0.7EL + LL + SL + EH + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 0.7,cEL, 1.0,cLL,1.0,cCSL, 1.0,cSL, 1.0,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) - 0.7EL + LL + SL + EH + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-0.7,cEL, 1.0,cLL,1.0,cCSL, 1.0,cSL, 1.0,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) + 0.7(SUF)ESP + LL + SL + EH + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 0.7,cESP, 1.0,cLL,1.0,cCSL, 1.0,cSL, 1.0,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) - 0.7(SUF)ESP + LL + SL + EH + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-0.7,cESP, 1.0,cLL,1.0,cCSL, 1.0,cSL, 1.0,cEPH, 1.0,cPSorTS);
            }
            else
            {
                strDesc.Format(_T("(DL+EV) + 0.7EL + LL + EH + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 0.7,cEL, 1.0,cLL,1.0,cCSL, 1.0,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) - 0.7EL + LL + EH + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-0.7,cEL, 1.0,cLL,1.0,cCSL, 1.0,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) + 0.7(SUF)ESP + LL + EH + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 0.7,cESP, 1.0,cLL,1.0,cCSL, 1.0,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) - 0.7(SUF)ESP + LL + EH + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-0.7,cESP, 1.0,cLL,1.0,cCSL, 1.0,cEPH, 1.0,cPSorTS);		
            }
        }
        else
        {
            if (bSL)
            {
                strDesc.Format(_T("(DL+EV) + 0.7EL + LL + SL + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 0.7,cEL, 1.0,cLL,1.0,cCSL, 1.0,cSL, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) - 0.7EL + LL + SL + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-0.7,cEL, 1.0,cLL,1.0,cCSL, 1.0,cSL, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) + 0.7(SUF)ESP + LL + SL + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 0.7,cESP, 1.0,cLL,1.0,cCSL, 1.0,cSL, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) - 0.7(SUF)ESP + LL + SL + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-0.7,cESP, 1.0,cLL,1.0,cCSL, 1.0,cSL, 1.0,cPSorTS);
            }
            else
            {
                strDesc.Format(_T("(DL+EV) + 0.7EL + LL + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 0.7,cEL, 1.0,cLL,1.0,cCSL, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) - 0.7EL + LL + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-0.7,cEL, 1.0,cLL,1.0,cCSL, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) + 0.7(SUF)ESP + LL + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 0.7,cESP, 1.0,cLL,1.0,cCSL, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) - 0.7(SUF)ESP + LL + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-0.7,cESP, 1.0,cLL,1.0,cCSL, 1.0,cPSorTS);		
            }
        }
    }
    else
    {
        if (bEH)
        {
            if (bSL)
            {
                strDesc.Format(_T("(DL+EV) + 0.7EL + SL + EH + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 0.7,cEL, 1.0,cSL, 1.0,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) - 0.7EL + SL + EH + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-0.7,cEL, 1.0,cSL, 1.0,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) + 0.7(SUF)ESP + SL + EH + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 0.7,cESP, 1.0,cSL, 1.0,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) - 0.7(SUF)ESP + SL + EH + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-0.7,cESP, 1.0,cSL, 1.0,cEPH, 1.0,cPSorTS);			
            }
            else
            {
                strDesc.Format(_T("(DL+EV) + 0.7EL + EH + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 0.7,cEL, 1.0,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) - 0.7EL + EH + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-0.7,cEL, 1.0,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) + 0.7(SUF)ESP + EH + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 0.7,cESP, 1.0,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) - 0.7(SUF)ESP + EH + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-0.7,cESP, 1.0,cEPH, 1.0,cPSorTS);			
            }
        }
        else
        {
            if (bSL)
            {
                strDesc.Format(_T("(DL+EV) + 0.7EL + SL + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 0.7,cEL, 1.0,cSL, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) - 0.7EL + SL + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-0.7,cEL, 1.0,cSL, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) + 0.7(SUF)ESP + SL + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 0.7,cESP, 1.0,cSL, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) - 0.7(SUF)ESP + SL + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-0.7,cESP, 1.0,cSL, 1.0,cPSorTS);			
            }
            else
            {
                strDesc.Format(_T("(DL+EV) + 0.7EL + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 0.7,cEL, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) - 0.7EL + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-0.7,cEL, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) + 0.7(SUF)ESP + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 0.7,cESP, 1.0,cPSorTS);
                strDesc.Format(_T("(DL+EV) - 0.7(SUF)ESP + %s"), strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-0.7,cESP, 1.0,cPSorTS);
            }
        }
    }

    // [3.3.6]은 사용성 조합에서는 [3.3.2]와 동일.

    // [3.3.7]
    BOOL bWL = cWL->IsExist();	
    if (bWL)
    {
        if (bEH)
        {
            strDesc.Format(_T("(DL+EV) + WL + EH + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 1.0,cWL, 1.0,cEPH, 1.0,cPSorTS);
            strDesc.Format(_T("(DL+EV) - WL + EH + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-1.0,cWL, 1.0,cEPH, 1.0,cPSorTS);
        }
        else
        {
            strDesc.Format(_T("(DL+EV) + WL + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 1.0,cWL, 1.0,cPSorTS);
            strDesc.Format(_T("(DL+EV) - WL + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-1.0,cWL, 1.0,cPSorTS);
        }
    }
    else
    {
        if (bEH)
        {
            strDesc.Format(_T("(DL+EV) + EH + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 1.0,cEPH, 1.0,cPSorTS);			
        }
        else
        {
            strDesc.Format(_T("(DL+EV) + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 1.0,cPSorTS);						
        }
    }

    // [0503.3.8]
    BOOL bSeis = !cEL->IsExist() && !cESP->IsExist() ? FALSE : TRUE;
    if (bSeis)
    {
        if (bEH)
        {
            strDesc.Format(_T("(DL+EV) + 0.7EL + EH + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 0.7,cEL, 1.0,cEPH, 1.0,cPSorTS);
            strDesc.Format(_T("(DL+EV) - 0.7EL + EH + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-0.7,cEL, 1.0,cEPH, 1.0,cPSorTS);
            strDesc.Format(_T("(DL+EV) + 0.7(SUF)ESP + EH + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 0.7,cESP, 1.0,cEPH, 1.0,cPSorTS);
            strDesc.Format(_T("(DL+EV) - 0.7(SUF)ESP + EH + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-0.7,cESP, 1.0,cEPH, 1.0,cPSorTS);		
        }
        else
        {
            strDesc.Format(_T("(DL+EV) + 0.7EL + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 0.7,cEL, 1.0,cPSorTS);
            strDesc.Format(_T("(DL+EV) - 0.7EL + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-0.7,cEL, 1.0,cPSorTS);
            strDesc.Format(_T("(DL+EV) + 0.7(SUF)ESP + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 0.7,cESP, 1.0,cPSorTS);
            strDesc.Format(_T("(DL+EV) - 0.7(SUF)ESP + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV,-0.7,cESP, 1.0,cPSorTS);
        }
    }
    else
    {
        if (bEH)
        {
            strDesc.Format(_T("(DL+EV) + EH + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 1.0,cEPH, 1.0,cPSorTS);
        }
        else
        {
            strDesc.Format(_T("(DL+EV) + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 1.0,cPSorTS);
        }
    }
}

void CLoadCombCtrl::Make_SubComb_KCI_USD12_Serv_FDN(int nA, BOOL bExistLL, const CString &strPS, CombBase* cDL, CombBase* cCSD, CombBase* cFP,
                                                    CombBase* cLL,  CombBase* cCSL, CombBase* cTL, CombBase* cEPV, CombBase* cEPH,
                                                    CombBase* cLR, CombBase* cSL,  CombBase* cRL,
                                                    CombBase* cWL, CombBase* cEL,  CombBase* cESP, CombBase* cPSorTS)
{
    CString strDesc = _T("");
    // [3.3.1]	
    strDesc.Format(_T("(DL+FP) + %s"), strPS);
    Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cFP, 1.0,cEPV, 1.0,cPSorTS);

    double dAlphaH = m_dAlpha_H;	
    BOOL bLRSeries = (!cLR->IsExist() && !cSL->IsExist() && !cRL->IsExist())? FALSE : TRUE;
    CString strCaseLR[3] = { _T("LR"), _T("SL"), _T("RL") };
    CombBase* cLoadLR[3] = { cLR, cSL, cRL };
    double dSF = 1.0/1.5;

    // [3.3.2]
    if(bLRSeries)
    {
        for (int i=0; i<3; ++i)
        {
            strDesc.Format(_T("(DL+FP+TL) + (LL+(APH)EV+EH) + %s + %s"), strCaseLR[i], strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cFP, 1.0,cTL, 1.0,cLL,1.0,cCSL, dAlphaH,cEPV, 1.0,cEPH, 1.0,cLoadLR[i], 1.0,cPSorTS);
        }
    }
    else
    {
        strDesc.Format(_T("(DL+FP+TL) + (LL+(APH)EV+EH) + %s"), strPS);
        Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cFP, 1.0,cTL, 1.0,cLL,1.0,cCSL, dAlphaH,cEPV, 1.0,cEPH, 1.0,cPSorTS);
    }

    // [3.3.3]
    if(!bExistLL && !cWL->IsExist())
    {
        for (int i=0; i<3; ++i)
        {
            strDesc.Format(_T("DL + %s + %s"), strCaseLR[i], strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cLoadLR[i], 1.0,cPSorTS);
        }
    }
    else
    {
        // [3.3.2]와 같은 조합이 생성되지 않도록 하기 위한 조건 처리.
        BOOL bGenerate = !cFP->IsExist() && !cTL->IsExist() && !cEPV->IsExist() & !cEPH->IsExist() ? FALSE : TRUE;
        for (int i=0; i<3; ++i)
        {
            if (bGenerate)
            {
                strDesc.Format(_T("DL + %s + LL + %s"), strCaseLR[i], strPS);
                Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cLoadLR[i], 1.0,cLL,1.0,cCSL, 1.0,cPSorTS);
            }
            strDesc.Format(_T("(DL + %s + WL)/1.5 + %s"), strCaseLR[i], strPS);
            Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cLoadLR[i], dSF,cWL, 1.0,cPSorTS);
            strDesc.Format(_T("(DL + %s - WL)/1.5 + %s"), strCaseLR[i], strPS);
            Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cLoadLR[i],-dSF,cWL, 1.0,cPSorTS);
        }
    }

    // [3.3.4]
    if (bExistLL)
    {
        if (bLRSeries)
        {
            for (int i=0; i<3; ++i)
            {
                strDesc.Format(_T("(DL + WL + LL + %s)/1.5 + %s"), strCaseLR[i], strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cWL, dSF,cLL,dSF,cCSL, dSF,cLoadLR[i], 1.0,cPSorTS);
                strDesc.Format(_T("(DL - WL + LL + %s)/1.5 + %s"), strCaseLR[i], strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD,-dSF,cWL, dSF,cLL,dSF,cCSL, dSF,cLoadLR[i], 1.0,cPSorTS);
            }			
        }
        else
        {
            strDesc.Format(_T("(DL + WL + LL)/1.5 + %s"), strPS);
            Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cWL, dSF,cLL,dSF,cCSL, 1.0,cPSorTS);
            strDesc.Format(_T("(DL - WL + LL)/1.5 + %s"), strPS);
            Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD,-dSF,cWL, dSF,cLL,dSF,cCSL, 1.0,cPSorTS);			
        }
    }
    else
    {
        if (bLRSeries)
        {
            for (int i=0; i<3; ++i)
            {
                strDesc.Format(_T("(DL + WL + %s)/1.5 + %s"), strCaseLR[i], strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cWL, dSF,cLoadLR[i], 1.0,cPSorTS);
                strDesc.Format(_T("(DL - WL + %s)/1.5 + %s"), strCaseLR[i], strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD,-dSF,cWL, dSF,cLoadLR[i], 1.0,cPSorTS);
            }			
        }
        else
        {
            strDesc.Format(_T("(DL + WL)/1.5 + %s"), strPS);
            Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cWL, 1.0,cPSorTS);
            strDesc.Format(_T("(DL - WL)/1.5 + %s"), strPS);
            Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD,-dSF,cWL, 1.0,cPSorTS);			
        }
    }

    // [3.3.5]
    BOOL bSL = cSL->IsExist();
    BOOL bEH = cEPH->IsExist();
    if (bExistLL)
    {
        if (bEH)
        {
            if (bSL)
            {
                strDesc.Format(_T("((DL+EV) + 0.7EL + LL + SL + EH)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, 0.7*dSF,cEL, dSF,cLL,dSF,cCSL, dSF,cSL, dSF,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) - 0.7EL + LL + SL + EH)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-0.7*dSF,cEL, dSF,cLL,dSF,cCSL, dSF,cSL, dSF,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) + 0.7(SUF)ESP + LL + SL + EH)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, 0.7*dSF,cESP, dSF,cLL,dSF,cCSL, dSF,cSL, dSF,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) - 0.7(SUF)ESP + LL + SL + EH)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-0.7*dSF,cESP, dSF,cLL,dSF,cCSL, dSF,cSL, dSF,cEPH, 1.0,cPSorTS);
            }
            else
            {
                strDesc.Format(_T("((DL+EV) + 0.7EL + LL + EH)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, 0.7*dSF,cEL, dSF,cLL,dSF,cCSL, dSF,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) - 0.7EL + LL + EH)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-0.7*dSF,cEL, dSF,cLL,dSF,cCSL, dSF,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) + 0.7(SUF)ESP + LL + EH)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, 0.7*dSF,cESP, dSF,cLL,dSF,cCSL, dSF,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) - 0.7(SUF)ESP + LL + EH)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-0.7*dSF,cESP, dSF,cLL,dSF,cCSL, dSF,cEPH, 1.0,cPSorTS);		
            }
        }
        else
        {
            if (bSL)
            {
                strDesc.Format(_T("((DL+EV) + 0.7EL + LL + SL)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, 0.7*dSF,cEL, dSF,cLL,dSF,cCSL, dSF,cSL, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) - 0.7EL + LL + SL)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-0.7*dSF,cEL, dSF,cLL,dSF,cCSL, dSF,cSL, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) + 0.7(SUF)ESP + LL + SL)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, 0.7*dSF,cESP, dSF,cLL,dSF,cCSL, dSF,cSL, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) - 0.7(SUF)ESP + LL + SL)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-0.7*dSF,cESP, dSF,cLL,dSF,cCSL, dSF,cSL, 1.0,cPSorTS);
            }
            else
            {
                strDesc.Format(_T("((DL+EV) + 0.7EL + LL)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, 0.7*dSF,cEL, dSF,cLL,dSF,cCSL, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) - 0.7EL + LL)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-0.7*dSF,cEL, dSF,cLL,dSF,cCSL, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) + 0.7(SUF)ESP + LL)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, 0.7*dSF,cESP, dSF,cLL,dSF,cCSL, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) - 0.7(SUF)ESP + LL)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-0.7*dSF,cESP, dSF,cLL,dSF,cCSL, 1.0,cPSorTS);		
            }
        }
    }
    else
    {
        if (bEH)
        {
            if (bSL)
            {
                strDesc.Format(_T("((DL+EV) + 0.7EL + SL + EH)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, 0.7*dSF,cEL, dSF,cSL, dSF,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) - 0.7EL + SL + EH)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-0.7*dSF,cEL, dSF,cSL, dSF,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) + 0.7(SUF)ESP + SL + EH)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, 0.7*dSF,cESP, dSF,cSL, dSF,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) - 0.7(SUF)ESP + SL + EH)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-0.7*dSF,cESP, dSF,cSL, dSF,cEPH, 1.0,cPSorTS);			
            }
            else
            {
                strDesc.Format(_T("((DL+EV) + 0.7EL + EH)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, 0.7*dSF,cEL, dSF,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) - 0.7EL + EH)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-0.7*dSF,cEL, dSF,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) + 0.7(SUF)ESP + EH)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, 0.7*dSF,cESP, dSF,cEPH, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) - 0.7(SUF)ESP + EH)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-0.7*dSF,cESP, dSF,cEPH, 1.0,cPSorTS);			
            }
        }
        else
        {
            if (bSL)
            {
                strDesc.Format(_T("((DL+EV) + 0.7EL + SL)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, 0.7*dSF,cEL, dSF,cSL, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) - 0.7EL + SL)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-0.7*dSF,cEL, dSF,cSL, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) + 0.7(SUF)ESP + SL)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, 0.7*dSF,cESP, dSF,cSL, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) - 0.7(SUF)ESP + SL)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-0.7*dSF,cESP, dSF,cSL, 1.0,cPSorTS);			
            }
            else
            {
                strDesc.Format(_T("((DL+EV) + 0.7EL)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, 0.7*dSF,cEL, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) - 0.7EL)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-0.7*dSF,cEL, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) + 0.7(SUF)ESP)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, 0.7*dSF,cESP, 1.0,cPSorTS);
                strDesc.Format(_T("((DL+EV) - 0.7(SUF)ESP)/1.5 + %s"), strPS);
                Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-0.7*dSF,cESP, 1.0,cPSorTS);
            }
        }
    }

    // [3.3.6]은 사용성 조합에서는 [3.3.2]와 동일.

    // [3.3.7]
    BOOL bWL = cWL->IsExist();	
    if (bWL)
    {
        if (bEH)
        {
            strDesc.Format(_T("((DL+EV) + WL + EH)/1.5 + %s"), strPS);
            Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, dSF,cWL, dSF,cEPH, 1.0,cPSorTS);
            strDesc.Format(_T("((DL+EV) - WL + EH)/1.5 + %s"), strPS);
            Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-dSF,cWL, dSF,cEPH, 1.0,cPSorTS);
        }
        else
        {
            strDesc.Format(_T("((DL+EV) + WL)/1.5 + %s"), strPS);
            Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, dSF,cWL, 1.0,cPSorTS);
            strDesc.Format(_T("((DL+EV) - WL)/1.5 + %s"), strPS);
            Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-dSF,cWL, 1.0,cPSorTS);
        }
    }
    else
    {
        if (bEH)
        {
            strDesc.Format(_T("(DL+EV) + EH + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 1.0,cEPH, 1.0,cPSorTS);			
        }
        else
        {
            strDesc.Format(_T("(DL+EV) + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 1.0,cPSorTS);						
        }
    }

    // [0503.3.8]
    BOOL bSeis = !cEL->IsExist() && !cESP->IsExist() ? FALSE : TRUE;
    if (bSeis)
    {
        if (bEH)
        {
            strDesc.Format(_T("((DL+EV) + 0.7EL + EH)/1.5 + %s"), strPS);
            Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, 0.7*dSF,cEL, dSF,cEPH, 1.0,cPSorTS);
            strDesc.Format(_T("((DL+EV) - 0.7EL + EH)/1.5 + %s"), strPS);
            Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-0.7*dSF,cEL, dSF,cEPH, 1.0,cPSorTS);
            strDesc.Format(_T("((DL+EV) + 0.7(SUF)ESP + EH)/1.5 + %s"), strPS);
            Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, 0.7*dSF,cESP, dSF,cEPH, 1.0,cPSorTS);
            strDesc.Format(_T("((DL+EV) - 0.7(SUF)ESP + EH)/1.5 + %s"), strPS);
            Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-0.7*dSF,cESP, dSF,cEPH, 1.0,cPSorTS);		
        }
        else
        {
            strDesc.Format(_T("((DL+EV) + 0.7EL)/1.5 + %s"), strPS);
            Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, 0.7*dSF,cEL, 1.0,cPSorTS);
            strDesc.Format(_T("((DL+EV) - 0.7EL)/1.5 + %s"), strPS);
            Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-0.7*dSF,cEL, 1.0,cPSorTS);
            strDesc.Format(_T("((DL+EV) + 0.7(SUF)ESP)/1.5 + %s"), strPS);
            Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV, 0.7*dSF,cESP, 1.0,cPSorTS);
            strDesc.Format(_T("((DL+EV) - 0.7(SUF)ESP)/1.5 + %s"), strPS);
            Set_LCB(nA, strDesc, dSF,cDL,dSF,cCSD, dSF,cEPV,-0.7*dSF,cESP, 1.0,cPSorTS);
        }
    }
    else
    {
        if (bEH)
        {
            strDesc.Format(_T("(DL+EV) + EH + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 1.0,cEPH, 1.0,cPSorTS);
        }
        else
        {
            strDesc.Format(_T("(DL+EV) + %s"), strPS);
            Set_LCB(nA, strDesc, 1.0,cDL,1.0,cCSD, 1.0,cEPV, 1.0,cPSorTS);
        }
    }
}

#pragma region /// [GEN-3331] KDS 41 10 15 : 2021 (for USD or LSD)

void CLoadCombCtrl::Make_SubComb_EQ01_KDS41_10_15_2021(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP,
    CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    Make_SubComb_EQ01_KBC2016(nActive, cDL, cCSD, cEPV, cFP, cPsTs, strPsTs, dPSS);
}

void CLoadCombCtrl::Make_SubComb_EQ02_KDS41_10_15_2021(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV,
    CombBase* cFP, CombBase* cTL, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
    CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    Make_SubComb_EQ02_KBC2016(nActive, cDL, cCSD, cEPV, cFP, cTL, cLL, cCSL, cIL, cLR, cSL, cRL, cPsTs, strPsTs, dPSS);
}

void CLoadCombCtrl::Make_SubComb_EQ03_KDS41_10_15_2021(int nActive, BOOL bExistLL, CombBase* cDL, CombBase* cCSD, CombBase* cEPV,
    CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
    CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cWL, CombBase* cPsTs,
    const CString& strPsTs, double dPSS)
{
    CString strDesc = _T(""); 
    CString strCaseLR[3] = { _T("LR"), _T("SL"), _T("RL") };
    CombBase* cLoadLR[3] = { cLR, cSL, cRL };

    if ( !bExistLL && !cWL->IsExist() ) //1.2D + 1.6(LR or S or R).
    {
        for ( int i=0; i<3; ++i )
        {
            strDesc.Format(_T("1.2(DL+EV+FP) + 1.6%s + (SFAC)%s"), strCaseLR[i], strPsTs);
            Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.2, cFP, 1.6, cLoadLR[i], dPSS, cPsTs);
        }
    }
    else
    {
        const double dWL = 0.5;
        const CString& strFactWL = _T("0.5");
        for ( int i=0; i<3; ++i )
        {
            strDesc.Format(_T("1.2(DL+EV+FP) + 1.6%s + 1.0(LL+IL) + (SFAC)%s"), strCaseLR[i], strPsTs);
            Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.2, cFP, 1.6, cLoadLR[i], 1.0, cLL, 1.0, cCSL, 1.0, cIL, dPSS, cPsTs);

            strDesc.Format(_T("1.2(DL+EV+FP) + 1.6%s + %sWL + (SFAC)%s"), strCaseLR[i], strFactWL, strPsTs);
            Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.2, cFP, 1.6, cLoadLR[i], dWL, cWL, dPSS, cPsTs);
            strDesc.Format(_T("1.2(DL+EV+FP) + 1.6%s - %sWL + (SFAC)%s"), strCaseLR[i], strFactWL, strPsTs);
            Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.2, cFP, 1.6, cLoadLR[i], -dWL, cWL, dPSS, cPsTs);
        }
    }
}

void CLoadCombCtrl::Make_SubComb_EQ04_KDS41_10_15_2021(int nActive, BOOL bExistLL, CombBase* cDL, CombBase* cCSD, CombBase* cEPV,
    CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
    CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cWL, CombBase* cPsTs,
    const CString& strPsTs, double dPSS)
{
    CString strDesc = _T("");
    BOOL bExistLRSeries = !cLR->IsExist() && !cSL->IsExist() && !cRL->IsExist()? FALSE : TRUE;
    
    const double dWL = 1.0;
    const CString& strFactWL = _T("1.0");
    if ( bExistLRSeries )
    {
        CString strCaseLR[3] = { _T("LR"), _T("SL"), _T("RL") };
        CombBase* cLoadLR[3] = { cLR, cSL, cRL };

        if ( bExistLL )
        {
            for ( int i=0; i<3; ++i )
            {
                strDesc.Format(_T("1.2(DL+EV+FP) + %sWL + 1.0(LL+IL) + 0.5%s + (SFAC)%s"), strFactWL, strCaseLR[i], strPsTs);
                Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.2, cFP, dWL, cWL, 1.0, cLL, 1.0, cCSL, 1.0, cIL, 0.5, cLoadLR[i], dPSS, cPsTs);

                strDesc.Format(_T("1.2(DL+EV+FP) - %sWL + 1.0(LL+IL) + 0.5%s + (SFAC)%s"), strFactWL, strCaseLR[i], strPsTs);
                Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.2, cFP, -dWL, cWL, 1.0, cLL, 1.0, cCSL, 1.0, cIL, 0.5, cLoadLR[i], dPSS, cPsTs);
            }
        }
        else
        {
            for ( int i=0; i<3; ++i )
            {
                strDesc.Format(_T("1.2(DL+EV+FP) + %sWL + 0.5%s + (SFAC)%s"), strFactWL, strCaseLR[i], strPsTs);
                Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.2, cFP, dWL, cWL, 0.5, cLoadLR[i], dPSS, cPsTs);

                strDesc.Format(_T("1.2(DL+EV+FP) - %sWL + 0.5%s + (SFAC)%s"), strFactWL, strCaseLR[i], strPsTs);
                Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.2, cFP, -dWL, cWL, 0.5, cLoadLR[i], dPSS, cPsTs);
            }
        }
    }
    else
    {
        if ( bExistLL )
        {
            strDesc.Format(_T("1.2(DL+EV+FP) + %sWL + 1.0(LL+IL) + (SFAC)%s"), strFactWL, strPsTs);
            Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.2, cFP, dWL, cWL, 1.0, cLL, 1.0, cCSL, 1.0, cIL, dPSS, cPsTs);

            strDesc.Format(_T("1.2(DL+EV+FP) - %sWL + 1.0(LL+IL) + (SFAC)%s"), strFactWL, strPsTs);
            Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.2, cFP, -dWL, cWL, 1.0, cLL, 1.0, cCSL, 1.0, cIL, dPSS, cPsTs);
        }
        else
        {
            strDesc.Format(_T("1.2(DL+EV+FP) + %sWL + (SFAC)%s"), strFactWL, strPsTs);
            Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.2, cFP, dWL, cWL, dPSS, cPsTs);

            strDesc.Format(_T("1.2(DL+EV+FP) - %sWL + (SFAC)%s"), strFactWL, strPsTs);
            Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.2, cEPV, 1.2, cFP, -dWL, cWL, dPSS, cPsTs);
        }
    }
}

void CLoadCombCtrl::Make_SubComb_EQ05_KDS41_10_15_2021(int nActive, BOOL bExistLL, BOOL bSlcmSpecial, CombBase* cDL, CombBase* cCSD,
    CombBase* cEPV, CombBase* cFP, CombBase* cLL, CombBase* cCSL,
    CombBase* cIL, CombBase* cSL, CombBase* cEL, CombBase* cESP, CombBase* cPsTs,
    const CString& strPsTs, double dPSS)
{
    Make_SubComb_EQ05_KBC2016(nActive, bExistLL, bSlcmSpecial, cDL, cCSD, cEPV, cFP, cLL, cCSL, cIL, cSL, cEL, cESP, cPsTs, strPsTs, dPSS);    
}

void CLoadCombCtrl::Make_SubComb_EQ06_KDS41_10_15_2021(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV,
    CombBase* cWL, CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    const double dWL = 1.0;
    const CString& strFactWL = _T("1.0");
    CString strDesc = _T("");    
    strDesc.Format(_T("0.9(DL+EV) + %sWL + (SFAC)%s"), strFactWL, strPsTs); Set_LCB(nActive, strDesc, 0.9, cDL, 0.9, cCSD, 0.9, cEPV, dWL, cWL, dPSS, cPsTs);
    strDesc.Format(_T("0.9(DL+EV) - %sWL + (SFAC)%s"), strFactWL, strPsTs); Set_LCB(nActive, strDesc, 0.9, cDL, 0.9, cCSD, 0.9, cEPV, -dWL, cWL, dPSS, cPsTs);
}

void CLoadCombCtrl::Make_SubComb_EQ07_KDS41_10_15_2021(int nActive, BOOL bSlcmSpecial, CombBase* cDL, CombBase* cCSD, CombBase* cEPV,
    CombBase* cFP, CombBase* cEL, CombBase* cESP, CombBase* cPsTs,
    const CString& strPsTs, double dPSS)
{
    Make_SubComb_EQ07_KBC2016(nActive, bSlcmSpecial, cDL, cCSD, cEPV, cFP, cEL, cESP, cPsTs, strPsTs, dPSS);   
}

#pragma endregion

#pragma region /// [GEN-3331] KDS 41 10 15 : 2021 (for ASD)
void CLoadCombCtrl::Make_SubComb_EQ08_KDS41_10_15_2021(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP,
    CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    Make_SubComb_EQ08_KBC2016(nActive, cDL, cCSD, cEPV, cFP, cPsTs, strPsTs, dPSS);
}

void CLoadCombCtrl::Make_SubComb_EQ09_KDS41_10_15_2021(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV,
    CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
    CombBase* cTL, CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    Make_SubComb_EQ09_KBC2016(nActive, cDL, cCSD, cEPV, cFP, cLL, cCSL, cIL, cTL, cPsTs, strPsTs, dPSS);
}

void CLoadCombCtrl::Make_SubComb_EQ10_KDS41_10_15_2021(int nActive, BOOL bExistLL, CombBase* cDL, CombBase* cCSD, CombBase* cEPV,
    CombBase* cFP, CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cPsTs,
    const CString& strPsTs, double dPSS)
{
    Make_SubComb_EQ10_KBC2016(nActive, bExistLL, cDL, cCSD, cEPV, cFP, cLR, cSL, cRL, cPsTs, strPsTs, dPSS);
}

void CLoadCombCtrl::Make_SubComb_EQ11_KDS41_10_15_2021(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV,
    CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL, CombBase* cLR,
    CombBase* cSL, CombBase* cRL, CombBase* cTL, CombBase* cPsTs,
    const CString& strPsTs, double dPSS)
{
    Make_SubComb_EQ11_KBC2016(nActive, cDL, cCSD, cEPV, cFP, cLL, cCSL, cIL, cLR, cSL, cRL, cTL, cPsTs, strPsTs, dPSS);    
}

void CLoadCombCtrl::Make_SubComb_EQ12_KDS41_10_15_2021(int nActive, BOOL bSlcmSpecial, CombBase* cDL, CombBase* cCSD, CombBase* cEPV,
    CombBase* cFP, CombBase* cWL, CombBase* cEL, CombBase* cESP, CombBase* cPsTs,
    const CString& strPsTs, double dPSS)
{
    const double dWL = 0.65;
    const CString& strFactWL = _T("0.65");
    CString strDesc = _T("");    
    strDesc.Format(_T("(DL+EV+FP) + %sWL + (SFAC)%s"), strFactWL, strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cEPV, 1.0, cFP, dWL, cWL, dPSS, cPsTs);
    strDesc.Format(_T("(DL+EV+FP) - %sWL + (SFAC)%s"), strFactWL, strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cEPV, 1.0, cFP, -dWL, cWL, dPSS, cPsTs);

    double dDLp10 = 1.0;
    CString strAddDLp = _T("");
    CString strFactEL  = _T("0.7");
    CString strFactESP = _T("0.7(SUF)");
    const CString& strTermDLEV = cEPV->IsExist() ? _T("(DL+EV)") : _T("DL");
    if ( bSlcmSpecial )
    {
        dDLp10 = 1.0 + 0.7 * m_dSlcmSds * m_dSlcmSpecial; // for 1.0D + [(OMG)E + 0.2SDS*D]
        strAddDLp = _T(" + ") + strFactEL + _T("(SPC)(SDS)") + strTermDLEV;
        strFactEL  += _T("(OMG)");
        strFactESP += _T("(OMG)");
    }
    
    const double dEL = 0.7;   

    strDesc.Format(_T("(DL+EV+FP) + %sEL + (SFAC)%s%s"), strFactEL, strPsTs, strAddDLp);
    Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, dEL, cEL, dPSS, cPsTs);

    strDesc.Format(_T("(DL+EV+FP) - %sEL + (SFAC)%s%s"), strFactEL, strPsTs, strAddDLp);
    Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, -dEL, cEL, dPSS, cPsTs);

    strDesc.Format(_T("(DL+EV+FP) + %sESP + (SFAC)%s%s"), strFactESP, strPsTs, strAddDLp);
    Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, dEL, cESP, dPSS, cPsTs);

    strDesc.Format(_T("(DL+EV+FP) - %sESP + (SFAC)%s%s"), strFactESP, strPsTs, strAddDLp);
    Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, -dEL, cESP, dPSS, cPsTs);
}

void CLoadCombCtrl::Make_SubComb_EQ13_KDS41_10_15_2021(int nActive, BOOL bExistLL, BOOL bSlcmSpecial, CombBase* cDL, CombBase* cCSD,
    CombBase* cEPV, CombBase* cFP, CombBase* cLL, CombBase* cCSL,
    CombBase* cIL, CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cWL,
    CombBase* cEL, CombBase* cESP, CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    CString strDesc = _T("");

    /// see KBC2015, 0301.5(1). (0301.5.13) = D+F+0.75(0.85W or 0.7E)+0.75L+0.75(Lr or S or R) -> [PMS 5279] 토압고려 하중조합 개선
    const bool bExistLRSeries = !cLR->IsExist() && !cSL->IsExist() && !cRL->IsExist()? false : true;
    const double dWL  = 0.75*0.65;
    const double dEL  = 0.75*0.70;
    const CString& strFactWL = _T("(0.75*0.65)");
    const CString& strFactEL = _T("(0.75*0.70)");

    double dDLp10 = 1.0;
    CString strAddDLp = _T("");
    CString strAddEL  = _T("");
    CString strAddESP = _T("(SUF)");

    if ( bSlcmSpecial )
    {
        dDLp10 = 1.0 + dEL * m_dSlcmSds * m_dSlcmSpecial; // for 1.0D + [(OMG)E + 0.2SDS*D]
        strAddDLp = cEPV->IsExist() ? _T(" + (0.75*0.7)(SPC)(SDS)(DL+EV)") : _T(" + (0.75*0.7)(SPC)(SDS)DL");
        strAddEL  = _T("(OMG)");
        strAddESP = _T("(SUF)(OMG)");
    }

    CString strCaseLR[3] = { _T("LR"), _T("SL"), _T("RL") };
    CombBase* cLoadLR[3] = { cLR, cSL, cRL };
    if ( bExistLRSeries )
    {
        if ( bExistLL )
        {
            for ( int i=0; i<3; ++i )
            {
                strDesc.Format(_T("1.0(DL+EV+FP) + %sWL + 0.75(LL+IL) + 0.75%s + (SFAC)%s"), strFactWL, strCaseLR[i], strPsTs);  Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cEPV, 1.0, cFP, dWL, cWL, 0.75, cLL, 0.75, cCSL, 0.75, cIL, 0.75, cLoadLR[i], dPSS, cPsTs);
                strDesc.Format(_T("1.0(DL+EV+FP) - %sWL + 0.75(LL+IL) + 0.75%s + (SFAC)%s"), strFactWL, strCaseLR[i], strPsTs);  Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cEPV, 1.0, cFP, -dWL, cWL, 0.75, cLL, 0.75, cCSL, 0.75, cIL, 0.75, cLoadLR[i], dPSS, cPsTs);
            }

            for ( int i=0; i<3; ++i )
            {
                strDesc.Format(_T("1.0(DL+EV+FP) + %s%sEL + 0.75(LL+IL) + 0.75%s + (SFAC)%s%s"), strFactEL, strAddEL, strCaseLR[i], strPsTs, strAddDLp);    Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, dEL, cEL, 0.75, cLL, 0.75, cCSL, 0.75, cIL, 0.75, cLoadLR[i], dPSS, cPsTs);
                strDesc.Format(_T("1.0(DL+EV+FP) - %s%sEL + 0.75(LL+IL) + 0.75%s + (SFAC)%s%s"), strFactEL, strAddEL, strCaseLR[i], strPsTs, strAddDLp);    Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, -dEL, cEL, 0.75, cLL, 0.75, cCSL, 0.75, cIL, 0.75, cLoadLR[i], dPSS, cPsTs);

                strDesc.Format(_T("1.0(DL+EV+FP) + %s%sESP + 0.75(LL+IL) + 0.75%s + (SFAC)%s%s"), strFactEL, strAddESP, strCaseLR[i], strPsTs, strAddDLp);  Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, dEL, cESP, 0.75, cLL, 0.75, cCSL, 0.75, cIL, 0.75, cLoadLR[i], dPSS, cPsTs);
                strDesc.Format(_T("1.0(DL+EV+FP) - %s%sESP + 0.75(LL+IL) + 0.75%s + (SFAC)%s%s"), strFactEL, strAddESP, strCaseLR[i], strPsTs, strAddDLp);  Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, -dEL, cESP, 0.75, cLL, 0.75, cCSL, 0.75, cIL, 0.75, cLoadLR[i], dPSS, cPsTs);
            }
        }
        else
        {
            for ( int i=0; i<3; ++i )
            {
                strDesc.Format(_T("1.0(DL+EV+FP) + %sWL + 0.75%s + (SFAC)%s"), strFactWL, strCaseLR[i], strPsTs); Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cEPV, 1.0, cFP, dWL, cWL, 0.75, cLoadLR[i], dPSS, cPsTs);
                strDesc.Format(_T("1.0(DL+EV+FP) - %sWL + 0.75%s + (SFAC)%s"), strFactWL, strCaseLR[i], strPsTs); Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cEPV, 1.0, cFP, -dWL, cWL, 0.75, cLoadLR[i], dPSS, cPsTs);
            }

            for ( int i=0; i<3; ++i )
            {
                strDesc.Format(_T("1.0(DL+EV+FP) + %s%sEL + 0.75%s + (SFAC)%s%s"), strFactEL, strAddEL, strCaseLR[i], strPsTs, strAddDLp);   Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, dEL, cEL, 0.75, cLoadLR[i], dPSS, cPsTs);
                strDesc.Format(_T("1.0(DL+EV+FP) - %s%sEL + 0.75%s + (SFAC)%s%s"), strFactEL, strAddEL, strCaseLR[i], strPsTs, strAddDLp);   Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, -dEL, cEL, 0.75, cLoadLR[i], dPSS, cPsTs);

                strDesc.Format(_T("1.0(DL+EV+FP) + %s%sESP + 0.75%s + (SFAC)%s%s"), strFactEL, strAddESP, strCaseLR[i], strPsTs, strAddDLp); Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, dEL, cESP, 0.75, cLoadLR[i], dPSS, cPsTs);
                strDesc.Format(_T("1.0(DL+EV+FP) - %s%sESP + 0.75%s + (SFAC)%s%s"), strFactEL, strAddESP, strCaseLR[i], strPsTs, strAddDLp); Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, -dEL, cESP, 0.75, cLoadLR[i], dPSS, cPsTs);
            }
        }
    }
    else
    {
        if ( bExistLL )
        {
            strDesc.Format(_T("1.0(DL+EV+FP) + %sWL + 0.75(LL+IL) + (SFAC)%s"), strFactWL, strPsTs); Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cEPV, 1.0, cFP, dWL, cWL, 0.75, cLL, 0.75, cCSL, 0.75, cIL, dPSS, cPsTs);
            strDesc.Format(_T("1.0(DL+EV+FP) - %sWL + 0.75(LL+IL) + (SFAC)%s"), strFactWL, strPsTs); Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cEPV, 1.0, cFP, -dWL, cWL, 0.75, cLL, 0.75, cCSL, 0.75, cIL, dPSS, cPsTs);

            strDesc.Format(_T("1.0(DL+EV+FP) + %s%sEL + 0.75(LL+IL) + (SFAC)%s%s"), strFactEL, strAddEL, strPsTs, strAddDLp);   Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, dEL, cEL, 0.75, cLL, 0.75, cCSL, 0.75, cIL, dPSS, cPsTs);
            strDesc.Format(_T("1.0(DL+EV+FP) - %s%sEL + 0.75(LL+IL) + (SFAC)%s%s"), strFactEL, strAddEL, strPsTs, strAddDLp);   Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, -dEL, cEL, 0.75, cLL, 0.75, cCSL, 0.75, cIL, dPSS, cPsTs);

            strDesc.Format(_T("1.0(DL+EV+FP) + %s%sESP + 0.75(LL+IL) + (SFAC)%s%s"), strFactEL, strAddESP, strPsTs, strAddDLp); Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, dEL, cESP, 0.75, cLL, 0.75, cCSL, 0.75, cIL, dPSS, cPsTs);
            strDesc.Format(_T("1.0(DL+EV+FP) - %s%sESP + 0.75(LL+IL) + (SFAC)%s%s"), strFactEL, strAddESP, strPsTs, strAddDLp); Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, -dEL, cESP, 0.75, cLL, 0.75, cCSL, 0.75, cIL, dPSS, cPsTs);
        }
        else
        {
            strDesc.Format(_T("1.0(DL+EV+FP) + %sWL + (SFAC)%s"), strFactWL, strPsTs); Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cEPV, 1.0, cFP, dWL, cWL, dPSS, cPsTs);
            strDesc.Format(_T("1.0(DL+EV+FP) - %sWL + (SFAC)%s"), strFactWL, strPsTs); Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cEPV, 1.0, cFP, -dWL, cWL, dPSS, cPsTs);

            strDesc.Format(_T("1.0(DL+EV+FP) + %s%sEL + (SFAC)%s%s"), strFactEL, strAddEL, strPsTs, strAddDLp);   Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, dEL, cEL, dPSS, cPsTs);
            strDesc.Format(_T("1.0(DL+EV+FP) - %s%sEL + (SFAC)%s%s"), strFactEL, strAddEL, strPsTs, strAddDLp);   Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, -dEL, cEL, dPSS, cPsTs);

            strDesc.Format(_T("1.0(DL+EV+FP) + %s%sESP + (SFAC)%s%s"), strFactEL, strAddESP, strPsTs, strAddDLp); Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, dEL, cESP, dPSS, cPsTs);
            strDesc.Format(_T("1.0(DL+EV+FP) - %s%sESP + (SFAC)%s%s"), strFactEL, strAddESP, strPsTs, strAddDLp); Set_LCB(nActive, strDesc, dDLp10, cDL, dDLp10, cCSD, dDLp10, cEPV, 1.0, cFP, -dEL, cESP, dPSS, cPsTs);
        }
    }
}

void CLoadCombCtrl::Make_SubComb_EQ14_KDS41_10_15_2021(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV,
    CombBase* cWL, CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    CString strDesc = _T("");
    /// see KDS 41 10 15 : 2021, 1.5.2(1), (1.5-14)
    strDesc.Format(_T("0.6(DL+EV) + 0.65WL + (SFAC)%s"), strPsTs); Set_LCB(nActive, strDesc, 0.6, cDL, 0.6, cCSD, 0.6, cEPV,  0.65, cWL, dPSS, cPsTs);
    strDesc.Format(_T("0.6(DL+EV) - 0.65WL + (SFAC)%s"), strPsTs); Set_LCB(nActive, strDesc, 0.6, cDL, 0.6, cCSD, 0.6, cEPV, -0.65, cWL, dPSS, cPsTs);
}

void CLoadCombCtrl::Make_SubComb_EQ15_KDS41_10_15_2021(int nActive, BOOL bSlcmSpecial, CombBase* cDL, CombBase* cCSD, CombBase* cEPV,
    CombBase* cFP, CombBase* cEL, CombBase* cESP, CombBase* cPsTs,
    const CString& strPsTs, double dPSS)
{
    Make_SubComb_EQ15_KBC2016(nActive, bSlcmSpecial, cDL, cCSD, cEPV, cFP, cEL, cESP, cPsTs, strPsTs, dPSS);    
}

#pragma endregion

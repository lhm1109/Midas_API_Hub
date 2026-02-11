#include "stdafx.h"
#include "wg_cmd.h"
#include "LoadCombCtrl.h"

#include "..\wg_base\wg_base_AppBase.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_CompFunc.h"

#include <fstream>

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma region /// [GEN-3690] NSCP 2015

void CLoadCombCtrl::MakeComb_NSCP2015(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cRL, CombBase* cTL, 
    CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEPH, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS,
    CombBase* cSUM, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS)
{
    const double dPST = GetPSLossFactorTransfer();
    const double dPSS = GetPSLossFactorService();
    
    int iLcomType = Get_CurrentLoadCombType();
    BOOL bIsFDN = ((iLcomType==D_LCOMTYPE_FDN || m_nDesignType==3) ? TRUE:FALSE);

    int nA = 1 + m_nConstLoad*100;
    BOOL bExistLL = IsExistLL(m_nConstLoad, cLL, cCSL);
    SetCombBaseConsiderCS(m_nConstLoad, &cDL, &cCSD, &cLL, &cCSL, &cCR, &cCSC, &cSH, &cCSS);

    SetScaleFactorLive();
    SetAddSeisLcomParam();

    BOOL bStageCS = IsStageCS();

    if ( m_nLcomType==D_LCOMTYPE_GENERAL )	Make_LCB_OnlyRespEccn(nA, cESP);

    const CString& strPsTs = bStageCS ? _T("TS") : _T("PS");
    CombBase* cPsTs = bStageCS?  cTS : cPS;

    m_iSetLcomForKBC = 0;
    Set_PrevLcomSRSS(FALSE, 1.0, 1.0, cEL, cESP);
        
    const int nA_STRN = nA;
    Make_SubComb_EQ01_NSCP2015(nA_STRN, cDL, cCSD, cFP, cPsTs, strPsTs, dPSS);
    Make_SubComb_EQ02_NSCP2015(nA_STRN, cDL, cCSD, cFP, cTL, cLL, cCSL, cEPH, cWP, cLR, cRL, cPsTs, strPsTs, dPSS);
    Make_SubComb_EQ03_NSCP2015(nA_STRN, cDL, cCSD, cLL, cCSL, cLR, cRL, cWL, cPsTs, strPsTs, dPSS);
    Make_SubComb_EQ04_NSCP2015(nA_STRN, cDL, cCSD, cLL, cCSL, cLR, cRL, cWL, cPsTs, strPsTs, dPSS);
    Make_SubComb_EQ05_NSCP2015(nA_STRN, cDL, cCSD, cLL, cCSL, cEL, cESP, cPsTs, strPsTs, dPSS, FALSE);
    Make_SubComb_EQ06_NSCP2015(nA_STRN, cDL, cCSD, cWL, cEPH, cWP, cPsTs, strPsTs, dPSS);
    Make_SubComb_EQ07_NSCP2015(nA_STRN, cDL, cCSD, cEL, cESP, cEPH, cWP, cPsTs, strPsTs, dPSS, FALSE);
    
    const int nA_SERV = nA + 1;
    Make_SubComb_EQ08_NSCP2015(nA_SERV, cDL, cCSD, cFP, cPsTs, strPsTs, dPSS);
    Make_SubComb_EQ09_NSCP2015(nA_SERV, cDL, cCSD, cEPH, cWP, cFP, cLL, cCSL, cTL, cPsTs, strPsTs, dPSS);
    Make_SubComb_EQ10_NSCP2015(nA_SERV, cDL, cCSD, cEPH, cWP, cFP, cLR, cRL, cPsTs, strPsTs, dPSS);
    Make_SubComb_EQ11_NSCP2015(nA_SERV, cDL, cCSD, cEPH, cWP, cFP, cLL, cCSL, cTL, cLR, cRL, cPsTs, strPsTs, dPSS);
    Make_SubComb_EQ12_NSCP2015(nA_SERV, cDL, cCSD, cEPH, cWP, cFP, cWL, cEL, cESP, cPsTs, strPsTs, dPSS, FALSE);
    
    if ( bStageCS ) // For Construction Stage Analysis, consider Transfer Stage.
    {
        if ( fabs(dPST-1.0) < m_dZeroLim )
        {
            Set_LCB(nA_SERV, _T("(SUM-TP-TS) + (1.0)TS"), 1.0, cSUM, (-1.0), cTP);
        }
        else
        {
            Set_LCB(nA_SERV, _T("(SUM-TP-TS) + (SFAT)TS"), 1.0, cSUM, (-1.0), cTP, dPST-1.0, cTS);
        }
    }

    if ( bIsFDN )
    {
        Make_SubComb_EQ13_NSCP2015(nA_SERV, cDL, cCSD, cEPH, cWP, cFP, cLL, cCSL, cLR, cWL, cEL, cESP, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ14_NSCP2015(nA_SERV, cDL, cCSD, cWL, cEPH, cWP, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ15_NSCP2015(nA_SERV, cDL, cCSD, cEL, cESP, cEPH, cWP, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ16_NSCP2015(nA_SERV, cDL, cCSD, cLL, cCSL, cLR, cRL, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ17_NSCP2015(nA_SERV, cDL, cCSD, cLL, cCSL, cWL, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ18_NSCP2015(nA_SERV, cDL, cCSD, cLL, cCSL, cEL, cESP, cPsTs, strPsTs, dPSS);
    }

    //++++++++++++++++++++++++++++++++++++++++
    // Special
    //++++++++++++++++++++++++++++++++++++++++
    if ( m_bSlcmSpecial )
    {
        m_iSetLcomForKBC = 1;
        Set_PrevLcomSRSS(TRUE, 1.0, 1.0, cEL, cESP);

        const int nA_SPEC = nA + 3;
        Make_SubComb_EQ01_NSCP2015(nA_SPEC, cDL, cCSD, cFP, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ02_NSCP2015(nA_SPEC, cDL, cCSD, cFP, cTL, cLL, cCSL, cEPH, cWP, cLR, cRL, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ03_NSCP2015(nA_SPEC, cDL, cCSD, cLL, cCSL, cLR, cRL, cWL, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ04_NSCP2015(nA_SPEC, cDL, cCSD, cLL, cCSL, cLR, cRL, cWL, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ05_NSCP2015(nA_SPEC, cDL, cCSD, cLL, cCSL, cEL, cESP, cPsTs, strPsTs, dPSS, TRUE);
        Make_SubComb_EQ06_NSCP2015(nA_SPEC, cDL, cCSD, cWL, cEPH, cWP, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ07_NSCP2015(nA_SPEC, cDL, cCSD, cEL, cESP, cEPH, cWP, cPsTs, strPsTs, dPSS, TRUE);
        
        if ( bStageCS ) // For Construction Stage Analysis, consider Transfer Stage.
        {
            if ( fabs(dPST-1.0) < m_dZeroLim )
            {
                Set_LCB(nA_SPEC, _T("(SUM-TP-TS) + (1.0)TS"), 1.0, cSUM, (-1.0), cTP);
            }
            else
            {
                Set_LCB(nA_SPEC, _T("(SUM-TP-TS) + (SFAT)TS"), 1.0, cSUM, (-1.0), cTP, dPST-1.0, cTS);
            }
        }
    }

    if ( m_bSlcmVertical )
    {
        m_iSetLcomForKBC = 2;
        Set_PrevLcomSRSS(FALSE, 1.0, 1.0, cEL, cESP);
        double dDLv = (-1.0)*m_dSlcmVertical;

        const int nA_VERT = nA + 4;
        Make_SubComb_EQ01_NSCP2015(nA_VERT, cDL, cCSD, cFP, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ02_NSCP2015(nA_VERT, cDL, cCSD, cFP, cTL, cLL, cCSL, cEPH, cWP, cLR, cRL, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ03_NSCP2015(nA_VERT, cDL, cCSD, cLL, cCSL, cLR, cRL, cWL, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ04_NSCP2015(nA_VERT, cDL, cCSD, cLL, cCSL, cLR, cRL, cWL, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ05_NSCP2015(nA_VERT, cDL, cCSD, cLL, cCSL, cEL, cESP, cPsTs, strPsTs, dPSS, FALSE);
        Make_SubComb_EQ06_NSCP2015(nA_VERT, cDL, cCSD, cWL, cEPH, cWP, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ07_NSCP2015(nA_VERT, cDL, cCSD, cEL, cESP, cEPH, cWP, cPsTs, strPsTs, dPSS, FALSE);
        
        if ( bStageCS )	// For Construction Stage Analysis.
        {
            Set_LCB(nA_VERT, _T("-(VER)DL + 1.0EL + (SFAC)TS"), dDLv, cDL, dDLv, cCSD, 1.0, cEL, dPSS, cTS);
            Set_LCB(nA_VERT, _T("-(VER)DL - 1.0EL + (SFAC)TS"), dDLv, cDL, dDLv, cCSD, -1.0, cEL, dPSS, cTS);
            Set_LCB(nA_VERT, _T("-(VER)DL + 1.0(SUF)ESP + (SFAC)TS"), dDLv, cDL, dDLv, cCSD, 1.0, cESP, dPSS, cTS);
            Set_LCB(nA_VERT, _T("-(VER)DL - 1.0(SUF)ESP + (SFAC)TS"), dDLv, cDL, dDLv, cCSD, -1.0, cESP, dPSS, cTS);

            // Transfer Stage.
            if ( fabs(dPST-1.0) < m_dZeroLim )
            {
                Set_LCB(nA_VERT, _T("(SUM-TP-TS) + (1.0)TS"), 1.0, cSUM, (-1.0), cTP);
            }
            else
            {
                Set_LCB(nA_VERT, _T("(SUM-TP-TS) + (SFAT)TS"), 1.0, cSUM, (-1.0), cTP, dPST-1.0, cTS);
            }
        }
        else// For NON-Construction Stage Analysis.
        {
            Set_LCB(nA_VERT, _T("-(VER)DL + 1.0EL + (SFAC)PS"), dDLv, cDL, dDLv, cCSD, 1.0, cEL, dPSS, cPS);
            Set_LCB(nA_VERT, _T("-(VER)DL - 1.0EL + (SFAC)PS"), dDLv, cDL, dDLv, cCSD, -1.0, cEL, dPSS, cPS);
            Set_LCB(nA_VERT, _T("-(VER)DL + 1.0(SUF)ESP + (SFAC)PS"), dDLv, cDL, dDLv, cCSD, 1.0, cESP, dPSS, cPS);
            Set_LCB(nA_VERT, _T("-(VER)DL - 1.0(SUF)ESP + (SFAC)PS"), dDLv, cDL, dDLv, cCSD, -1.0, cESP, dPSS, cPS);
        }
    }

    // Add by ZINU.('05.04.19). Consider SRSS.
    if ( m_nOrthoType==1 )	m_iSetLcomForSRSS = 0;
}

void CLoadCombCtrl::MakeComb_NSCP2015_ASD(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cRL, CombBase* cTL,
    CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEPH, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS,
    CombBase* cSUM, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS)
{
    const double dPST = GetPSLossFactorTransfer();
    const double dPSS = GetPSLossFactorService();
    
    int iLcomType = Get_CurrentLoadCombType();
    BOOL bIsFDN = ((iLcomType==D_LCOMTYPE_FDN || m_nDesignType==3) ? TRUE:FALSE);

    int nA = 1 + m_nConstLoad*100;
    BOOL bExistLL = IsExistLL(m_nConstLoad, cLL, cCSL);
    SetCombBaseConsiderCS(m_nConstLoad, &cDL, &cCSD, &cLL, &cCSL, &cCR, &cCSC, &cSH, &cCSS);

    SetScaleFactorLive();
    SetAddSeisLcomParam();

    BOOL bStageCS = IsStageCS();

    if ( m_nLcomType==D_LCOMTYPE_GENERAL )	Make_LCB_OnlyRespEccn(nA, cESP);

    const CString& strPsTs = bStageCS ? _T("TS") : _T("PS");
    CombBase* cPsTs = bStageCS?  cTS : cPS;

    m_iSetLcomForKBC = 0;
    Set_PrevLcomSRSS(FALSE, 1.0, 1.0, cEL, cESP);
        
    const int nA_STRN = nA;
    Make_SubComb_EQ08_NSCP2015(nA_STRN, cDL, cCSD, cFP, cPsTs, strPsTs, dPSS);
    Make_SubComb_EQ09_NSCP2015(nA_STRN, cDL, cCSD, cEPH, cWP, cFP, cLL, cCSL, cTL, cPsTs, strPsTs, dPSS);
    Make_SubComb_EQ10_NSCP2015(nA_STRN, cDL, cCSD, cEPH, cWP, cFP, cLR, cRL, cPsTs, strPsTs, dPSS);
    Make_SubComb_EQ11_NSCP2015(nA_STRN, cDL, cCSD, cEPH, cWP, cFP, cLL, cCSL, cTL, cLR, cRL, cPsTs, strPsTs, dPSS);
    Make_SubComb_EQ12_NSCP2015(nA_STRN, cDL, cCSD, cEPH, cWP, cFP, cWL, cEL, cESP, cPsTs, strPsTs, dPSS, FALSE);
    
    const int nA_SERV = nA + 1;
    Make_SubComb_EQ08_NSCP2015(nA_SERV, cDL, cCSD, cFP, cPsTs, strPsTs, dPSS);
    Make_SubComb_EQ09_NSCP2015(nA_SERV, cDL, cCSD, cEPH, cWP, cFP, cLL, cCSL, cTL, cPsTs, strPsTs, dPSS);
    Make_SubComb_EQ10_NSCP2015(nA_SERV, cDL, cCSD, cEPH, cWP, cFP, cLR, cRL, cPsTs, strPsTs, dPSS);
    Make_SubComb_EQ11_NSCP2015(nA_SERV, cDL, cCSD, cEPH, cWP, cFP, cLL, cCSL, cTL, cLR, cRL, cPsTs, strPsTs, dPSS);
    Make_SubComb_EQ12_NSCP2015(nA_SERV, cDL, cCSD, cEPH, cWP, cFP, cWL, cEL, cESP, cPsTs, strPsTs, dPSS, FALSE);
    
    if ( bStageCS ) // For Construction Stage Analysis, consider Transfer Stage.
    {
        if ( fabs(dPST-1.0) < m_dZeroLim )
        {
            Set_LCB(nA_SERV, _T("(SUM-TP-TS) + (1.0)TS"), 1.0, cSUM, (-1.0), cTP);
        }
        else
        {
            Set_LCB(nA_SERV, _T("(SUM-TP-TS) + (SFAT)TS"), 1.0, cSUM, (-1.0), cTP, dPST-1.0, cTS);
        }
    }

    if ( bIsFDN )
    {
        Make_SubComb_EQ13_NSCP2015(nA_SERV, cDL, cCSD, cEPH, cWP, cFP, cLL, cCSL, cLR, cWL, cEL, cESP, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ14_NSCP2015(nA_SERV, cDL, cCSD, cWL, cEPH, cWP, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ15_NSCP2015(nA_SERV, cDL, cCSD, cEL, cESP, cEPH, cWP, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ16_NSCP2015(nA_SERV, cDL, cCSD, cLL, cCSL, cLR, cRL, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ17_NSCP2015(nA_SERV, cDL, cCSD, cLL, cCSL, cWL, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ18_NSCP2015(nA_SERV, cDL, cCSD, cLL, cCSL, cEL, cESP, cPsTs, strPsTs, dPSS);
    }

    //++++++++++++++++++++++++++++++++++++++++
    // Special
    //++++++++++++++++++++++++++++++++++++++++
    if ( m_bSlcmSpecial )
    {
        m_iSetLcomForKBC = 1;
        Set_PrevLcomSRSS(TRUE, 1.0, 1.0, cEL, cESP);

        const int nA_SPEC = nA + 3;        
        Make_SubComb_EQ08_NSCP2015(nA_SPEC, cDL, cCSD, cFP, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ09_NSCP2015(nA_SPEC, cDL, cCSD, cEPH, cWP, cFP, cLL, cCSL, cTL, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ10_NSCP2015(nA_SPEC, cDL, cCSD, cEPH, cWP, cFP, cLR, cRL, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ11_NSCP2015(nA_SPEC, cDL, cCSD, cEPH, cWP, cFP, cLL, cCSL, cTL, cLR, cRL, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ12_NSCP2015(nA_SPEC, cDL, cCSD, cEPH, cWP, cFP, cWL, cEL, cESP, cPsTs, strPsTs, dPSS, TRUE);
        
        if ( bStageCS ) // For Construction Stage Analysis, consider Transfer Stage.
        {
            if ( fabs(dPST-1.0) < m_dZeroLim )
            {
                Set_LCB(nA_SPEC, _T("(SUM-TP-TS) + (1.0)TS"), 1.0, cSUM, (-1.0), cTP);
            }
            else
            {
                Set_LCB(nA_SPEC, _T("(SUM-TP-TS) + (SFAT)TS"), 1.0, cSUM, (-1.0), cTP, dPST-1.0, cTS);
            }
        }
    }

    if ( m_bSlcmVertical )
    {
        m_iSetLcomForKBC = 2;
        Set_PrevLcomSRSS(FALSE, 1.0, 1.0, cEL, cESP);
        double dDLv = (-1.0)*m_dSlcmVertical;

        const int nA_VERT = nA + 4;
        Make_SubComb_EQ08_NSCP2015(nA_VERT, cDL, cCSD, cFP, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ09_NSCP2015(nA_VERT, cDL, cCSD, cEPH, cWP, cFP, cLL, cCSL, cTL, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ10_NSCP2015(nA_VERT, cDL, cCSD, cEPH, cWP, cFP, cLR, cRL, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ11_NSCP2015(nA_VERT, cDL, cCSD, cEPH, cWP, cFP, cLL, cCSL, cTL, cLR, cRL, cPsTs, strPsTs, dPSS);
        Make_SubComb_EQ12_NSCP2015(nA_VERT, cDL, cCSD, cEPH, cWP, cFP, cWL, cEL, cESP, cPsTs, strPsTs, dPSS, FALSE);
        
        if ( bStageCS )	// For Construction Stage Analysis.
        {
            Set_LCB(nA_VERT, _T("-(VER)DL + 1.0EL + (SFAC)TS"), dDLv, cDL, dDLv, cCSD, 1.0, cEL, dPSS, cTS);
            Set_LCB(nA_VERT, _T("-(VER)DL - 1.0EL + (SFAC)TS"), dDLv, cDL, dDLv, cCSD, -1.0, cEL, dPSS, cTS);
            Set_LCB(nA_VERT, _T("-(VER)DL + 1.0(SUF)ESP + (SFAC)TS"), dDLv, cDL, dDLv, cCSD, 1.0, cESP, dPSS, cTS);
            Set_LCB(nA_VERT, _T("-(VER)DL - 1.0(SUF)ESP + (SFAC)TS"), dDLv, cDL, dDLv, cCSD, -1.0, cESP, dPSS, cTS);

            // Transfer Stage.
            if ( fabs(dPST-1.0) < m_dZeroLim )
            {
                Set_LCB(nA_VERT, _T("(SUM-TP-TS) + (1.0)TS"), 1.0, cSUM, (-1.0), cTP);
            }
            else
            {
                Set_LCB(nA_VERT, _T("(SUM-TP-TS) + (SFAT)TS"), 1.0, cSUM, (-1.0), cTP, dPST-1.0, cTS);
            }
        }
        else// For NON-Construction Stage Analysis.
        {
            Set_LCB(nA_VERT, _T("-(VER)DL + 1.0EL + (SFAC)PS"), dDLv, cDL, dDLv, cCSD, 1.0, cEL, dPSS, cPS);
            Set_LCB(nA_VERT, _T("-(VER)DL - 1.0EL + (SFAC)PS"), dDLv, cDL, dDLv, cCSD, -1.0, cEL, dPSS, cPS);
            Set_LCB(nA_VERT, _T("-(VER)DL + 1.0(SUF)ESP + (SFAC)PS"), dDLv, cDL, dDLv, cCSD, 1.0, cESP, dPSS, cPS);
            Set_LCB(nA_VERT, _T("-(VER)DL - 1.0(SUF)ESP + (SFAC)PS"), dDLv, cDL, dDLv, cCSD, -1.0, cESP, dPSS, cPS);
        }
    }

    // Add by ZINU.('05.04.19). Consider SRSS.
    if ( m_nOrthoType==1 )	m_iSetLcomForSRSS = 0;
}

#pragma region /// [GEN-3690] NSCP 2015 (for USD)

void CLoadCombCtrl::Make_SubComb_EQ01_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    /// (203-1) 1.4(D+F)
    CString strDesc = _T("");    
    strDesc.Format(_T("1.4(DL+FP) + (SFAC)%s"), strPsTs);
    Set_LCB(nActive, strDesc, 1.4, cDL, 1.4, cCSD, 1.4, cFP, dPSS, cPsTs);
}

void  CLoadCombCtrl::Make_SubComb_EQ02_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cTL, CombBase* cLL, CombBase* cCSL,
    CombBase* cEPH, CombBase* cWP, CombBase* cLR, CombBase* cRL, CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    /// (203-2) 1.2(D+F+T)+1.6(L+H)+0.5(Lr or R)
    double dSP = 0.0;
    CString strLSP =  _T("");
    if ( IsConsiderLateralSoil(cEPH, cWP) )
    {
        dSP    = m_dLateralSoilFactor;
        strLSP = _T("+ (LSPF)(EH+WP) ");
    }
    
    CString strDesc = _T("");    
    if ( !cLR->IsExist() && !cRL->IsExist() )
    {
        strDesc.Format(_T("1.2(DL+FP+TL) + 1.6LL %s+ (SFAC)%s"), strLSP, strPsTs);
        Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.2, cFP, 1.2, cTL, 1.6, cLL, 1.6, cCSL, dSP, cEPH, dSP, cWP, dPSS, cPsTs);
    }
    else
    {
        std::vector<CombBase*> vCombBase;
        vCombBase.emplace_back(cLR);
        vCombBase.emplace_back(cRL);
        const auto nSize = vCombBase.size();
        for ( int i=0; i<nSize; ++i )
        {
            CombBase* CurBase = vCombBase.at(i);
            strDesc.Format(_T("1.2(DL+FP+TL) + 1.6LL %s+ 0.5%s+ (SFAC)%s"), strLSP, CurBase->Get_Type(), strPsTs);
            Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.2, cFP, 1.2, cTL, 1.6, cLL, 1.6, cCSL, dSP, cEPH, dSP, cWP, 0.5, CurBase, dPSS, cPsTs);
        }
    }
}

void CLoadCombCtrl::Make_SubComb_EQ03_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, 
    CombBase* cLR, CombBase* cRL, CombBase* cWL, CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    /// (203-3) 1.2D+1.6(Lr or R)+(f1*L or 0.5W)
    std::vector<CombBase*> vCombBase;
    vCombBase.emplace_back(cLR);
    vCombBase.emplace_back(cRL);
    
	const INT_PTR nSize = vCombBase.size();
    CString strDesc = _T("");
    const BOOL bExistLL = IsExistLL(m_nConstLoad, cLL, cCSL);
    if ( !bExistLL && !cWL->IsExist() )
    {
        for ( INT_PTR i=0; i<nSize; ++i )
        {
            CombBase* CurBase = vCombBase.at(i);
            strDesc.Format(_T("1.2DL + 1.6%s + (SFAC)%s"), CurBase->Get_Type(), strPsTs);
            Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.6, CurBase, dPSS, cPsTs);
        }
    }
    else
    {
        for ( INT_PTR i=0; i<nSize; ++i )
        {
            CombBase* CurBase = vCombBase.at(i);
            const CString& strCurBaseType = CurBase->Get_Type();

            strDesc.Format(_T("1.2DL + 1.6%s + 1.0(LF)LL + (SFAC)%s"), strCurBaseType, strPsTs);
            Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.6, CurBase, 1.0, cLL, 1.0, cCSL, dPSS, cPsTs);

            strDesc.Format(_T("1.2DL + 1.6%s + 0.5WL + (SFAC)%s"), strCurBaseType, strPsTs);
            Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.6, CurBase, 0.5, cWL, dPSS, cPsTs);
            strDesc.Format(_T("1.2DL + 1.6%s - 0.5WL + (SFAC)%s"), strCurBaseType, strPsTs);
            Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.6, CurBase, -0.5, cWL, dPSS, cPsTs);
        }
    }
}

void CLoadCombCtrl::Make_SubComb_EQ04_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, 
    CombBase* cLR, CombBase* cRL, CombBase* cWL, CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    CString strDesc = _T("");
    /// (203-4) = 1.2D+1.0W+f1*L+0.5(Lr ¶Ç´Â R)
    const BOOL bExistLL = IsExistLL(m_nConstLoad, cLL, cCSL);
    const bool bExistLRSeries = !cLR->IsExist() && !cRL->IsExist() ? false : true;
    if ( bExistLRSeries )
    {
        std::vector<CombBase*> vCombBase;
        vCombBase.emplace_back(cLR);
        vCombBase.emplace_back(cRL);
		INT_PTR nSize = vCombBase.size();

        if ( bExistLL )
        {
            for ( int i=0; i<nSize; ++i )
            {
                CombBase* CurBase = vCombBase.at(i);
                const CString& strCurBaseType = CurBase->Get_Type();

                strDesc.Format(_T("1.2DL + 1.0WL + 1.0(LF)LL + 0.5%s + (SFAC)%s"), strCurBaseType, strPsTs);
                Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.0, cWL, 1.0, cLL, 1.0, cCSL, 0.5, CurBase, dPSS, cPsTs);

                strDesc.Format(_T("1.2DL - 1.0WL + 1.0(LF)LL + 0.5%s + (SFAC)%s"), strCurBaseType, strPsTs);
                Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD,-1.0, cWL, 1.0, cLL, 1.0, cCSL, 0.5, CurBase, dPSS, cPsTs);
            }
        }
        else
        {
            for ( int i=0; i<nSize; ++i )
            {
                CombBase* CurBase = vCombBase.at(i);
                const CString& strCurBaseType = CurBase->Get_Type();

                strDesc.Format(_T("1.2DL + 1.0WL + 0.5%s + (SFAC)%s"), strCurBaseType, strPsTs);
                Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.0, cWL, 0.5, CurBase, dPSS, cPsTs);

                strDesc.Format(_T("1.2DL - 1.0WL + 0.5%s + (SFAC)%s"), strCurBaseType, strPsTs);
                Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD,-1.0, cWL, 0.5, CurBase, dPSS, cPsTs);
            }
        }
    }
    else
    {
        if ( bExistLL )
        {
            strDesc.Format(_T("1.2DL + 1.0WL + 1.0(LF)LL + (SFAC)%s"), strPsTs);
            Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.0, cWL, 1.0, cLL, 1.0, cCSL, dPSS, cPsTs);

            strDesc.Format(_T("1.2DL - 1.0WL + 1.0(LF)LL + (SFAC)%s"), strPsTs);
            Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD,-1.0, cWL, 1.0, cLL, 1.0, cCSL, dPSS, cPsTs);
        }
        else
        {
            strDesc.Format(_T("1.2DL + 1.0WL + (SFAC)%s"), strPsTs);
            Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD, 1.0, cWL, dPSS, cPsTs);

            strDesc.Format(_T("1.2DL - 1.0WL + (SFAC)%s"), strPsTs);
            Set_LCB(nActive, strDesc, 1.2, cDL, 1.2, cCSD,-1.0, cWL, dPSS, cPsTs);
        }
    }
}

void CLoadCombCtrl::Make_SubComb_EQ05_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL,
    CombBase* cEL, CombBase* cESP, CombBase* cPsTs, const CString& strPsTs, double dPSS, BOOL bSlcmSpecial)
{
    const bool bRedundancyFact = m_bRedundancyFact;
    if ( bSlcmSpecial )
	{
		m_bRedundancyFact = false; 		
	}

    double dDL = 1.2;
    CString strAddEL  = m_bRedundancyFact ? _T("(RF)") : _T("");
    CString strAddESP = _T("(SUF)");
	CString strAddDL  = _T("");
    if ( bSlcmSpecial )
    {
		dDL = 1.2 + m_dSlcmSds * m_dSlcmSpecial; // for 1.2D + (OMG)E + 0.2SDS*D
        strAddEL  += _T("(OMG)");
        strAddESP += _T("(OMG)");
		strAddDL = _T(" + (SPC)(SDS)DL");
    }

    CString strDesc = _T("");
    /// (203-5) = 1.2D+1.0E+f1*L
    const BOOL bExistLL = IsExistLL(m_nConstLoad, cLL, cCSL);        
    if ( bExistLL )
    {
        strDesc.Format(_T("1.2DL + 1.0%sEL + 1.0(LF)LL + (SFAC)%s%s"), strAddEL, strPsTs, strAddDL);
        Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, 1.0, cEL, 1.0, cLL, 1.0, cCSL, dPSS, cPsTs);
        strDesc.Format(_T("1.2DL - 1.0%sEL + 1.0(LF)LL + (SFAC)%s%s"), strAddEL, strPsTs, strAddDL);
        Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD,-1.0, cEL, 1.0, cLL, 1.0, cCSL, dPSS, cPsTs);

        strDesc.Format(_T("1.2DL + 1.0%sESP + 1.0(LF)LL + (SFAC)%s%s"), strAddESP, strPsTs, strAddDL);
        Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, 1.0, cESP, 1.0, cLL, 1.0, cCSL, dPSS, cPsTs);
        strDesc.Format(_T("1.2DL - 1.0%sESP + 1.0(LF)LL + (SFAC)%s%s"), strAddESP, strPsTs, strAddDL);
        Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD,-1.0, cESP, 1.0, cLL, 1.0, cCSL, dPSS, cPsTs);
    }
    else
    {
        strDesc.Format(_T("1.2DL + 1.0%sEL + (SFAC)%s%s"), strAddEL, strPsTs, strAddDL);
        Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, 1.0, cEL, dPSS, cPsTs);
        strDesc.Format(_T("1.2DL - 1.0%sEL + (SFAC)%s%s"), strAddEL, strPsTs, strAddDL);
        Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD,-1.0, cEL, dPSS, cPsTs);

        strDesc.Format(_T("1.2DL + 1.0%sESP + (SFAC)%s%s"), strAddESP, strPsTs, strAddDL);
        Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, 1.0, cESP, dPSS, cPsTs);
        strDesc.Format(_T("1.2DL - 1.0%sESP + (SFAC)%s%s"), strAddESP, strPsTs, strAddDL);
        Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD,-1.0, cESP, dPSS, cPsTs);
    }

    m_bRedundancyFact = bRedundancyFact;
}

void CLoadCombCtrl::Make_SubComb_EQ06_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cWL, CombBase* cEPH,
    CombBase* cWP, CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    /// (203-6) 0.9D+1.0W+1.6H
    double dSP = 0.0;
    CString strLSP  = _T("");
    if ( IsConsiderLateralSoil(cEPH, cWP) )
    {
        dSP = m_dLateralSoilFactor;
        strLSP = _T("+ (LSPF)(EH+WP) ");
    }

    CString strDesc = _T("");
    strDesc.Format(_T("0.9DL + 1.0WL %s+ (SFAC)%s"), strLSP, strPsTs);
    Set_LCB(nActive, strDesc, 0.9, cDL, 0.9, cCSD, 1.0, cWL, dSP, cEPH, dSP, cWP, dPSS, cPsTs);
    strDesc.Format(_T("0.9DL - 1.0WL %s+ (SFAC)%s"), strLSP, strPsTs);
    Set_LCB(nActive, strDesc, 0.9, cDL, 0.9, cCSD,-1.0, cWL, dSP, cEPH, dSP, cWP, dPSS, cPsTs);
}

void CLoadCombCtrl::Make_SubComb_EQ07_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEL, CombBase* cESP,
    CombBase* cEPH, CombBase* cWP, CombBase* cPsTs, const CString& strPsTs, double dPSS, BOOL bSlcmSpecial)
{
    const bool bRedundancyFact = m_bRedundancyFact;
    if ( bSlcmSpecial ) { m_bRedundancyFact = false; }

    /// (203-7) 0.9D+1.0E+1.6H
    double dDL = 0.9;
    CString strAddEL  = m_bRedundancyFact && !bSlcmSpecial ? _T("(RF)") : _T("");
    CString strAddESP = _T("(SUF)");
	CString strAddDL  = _T("");
    if ( bSlcmSpecial )
    {
		dDL = 0.9 - m_dSlcmSds * m_dSlcmSpecial; // for 0.9D + (OMG)E - 0.2SDS*D
        strAddEL  += _T("(OMG)");
        strAddESP += _T("(OMG)");
		strAddDL = _T(" - (SPC)(SDS)DL");
    }

    double dSP = 0.0;
    CString strLSP = _T("");
    if ( IsConsiderLateralSoil(cEPH, cWP))
    {
        dSP = m_dLateralSoilFactor;
        strLSP = _T("+ (LSPF)(EH+WP) ");
    }

    CString strDesc = _T("");
    strDesc.Format(_T("0.9DL + 1.0%sEL %s+ (SFAC)%s%s"), strAddEL, strLSP, strPsTs, strAddDL);
    Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, 1.0, cEL, dSP, cEPH, dSP, cWP, dPSS, cPsTs);
    strDesc.Format(_T("0.9DL - 1.0%sEL %s+ (SFAC)%s%s"), strAddEL, strLSP, strPsTs, strAddDL);
    Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD,-1.0, cEL, dSP, cEPH, dSP, cWP, dPSS, cPsTs);

    strDesc.Format(_T("0.9DL + 1.0%sESP %s+ (SFAC)%s%s"), strAddESP, strLSP, strPsTs, strAddDL);
    Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, 1.0, cESP, dSP, cEPH, dSP, cWP, dPSS, cPsTs);
    strDesc.Format(_T("0.9DL - 1.0%sESP %s+ (SFAC)%s%s"), strAddESP, strLSP, strPsTs, strAddDL);
    Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD,-1.0, cESP, dSP, cEPH, dSP, cWP, dPSS, cPsTs);

    m_bRedundancyFact = bRedundancyFact;
}

#pragma endregion

#pragma region /// [GEN-3690] NSCP 2015 (for ASD)

void CLoadCombCtrl::Make_SubComb_EQ08_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    CString strDesc = _T("");
    /// (203-8) D+F
    strDesc.Format(_T("(DL+FP) + (SFAC)%s"), strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cFP, dPSS, cPsTs);
}

void CLoadCombCtrl::Make_SubComb_EQ09_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPH, CombBase* cWP,
    CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cTL, CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    double dSP = 0.0;
    CString strLSP = _T("");
    if ( IsConsiderLateralSoil(cEPH, cWP) )
    {
        dSP = 1.0;
        strLSP = _T("+ (EH+WP) ");
    }

    CString strDesc = _T("");
    /// (203-9) D+H+F+L+T
    strDesc.Format(_T("(DL+FP) + (LL+TL) %s+ (SFAC)%s"), strLSP, strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cFP, 1.0, cLL, 1.0, cCSL, 1.0, cTL, dSP, cEPH, dSP, cWP, dPSS, cPsTs);
}

void CLoadCombCtrl::Make_SubComb_EQ10_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPH, CombBase* cWP,
    CombBase* cFP, CombBase* cLR, CombBase* cRL, CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    double dSP = 0.0;
    CString strLSP = _T("");
    if ( IsConsiderLateralSoil(cEPH, cWP) )
    {
        dSP = 1.0;
        strLSP = _T("+ (EH+WP) ");
    }

    CString strDesc = _T("");
    /// (203-10) D+H+F+(Lr or R)
    std::vector<CombBase*> vCombBase;
    vCombBase.emplace_back(cLR);
    vCombBase.emplace_back(cRL);
	const INT_PTR nSize = vCombBase.size();
    if ( nSize > 0 )
    {
        for ( int i=0; i<nSize; ++i )
        {
            CombBase* CurBase = vCombBase.at(i);
            strDesc.Format(_T("(DL+FP) %s+ %s + (SFAC)%s"), strLSP, CurBase->Get_Type(), strPsTs);
            Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cFP, dSP, cEPH, dSP, cWP, 1.0, CurBase, dPSS, cPsTs);
        }
    }
    else
    {
        strDesc.Format(_T("(DL+FP) %s+ (SFAC)%s"), strLSP, strPsTs);
        Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cFP, dSP, cEPH, dSP, cWP, dPSS, cPsTs);
    }    
}

void CLoadCombCtrl::Make_SubComb_EQ11_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPH, CombBase* cWP,
    CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cTL, CombBase* cLR, CombBase* cRL, CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    double dSP = 0.0;
    CString strLSP = _T("");
    if ( IsConsiderLateralSoil(cEPH, cWP) )
    {
        dSP = 1.0;
        strLSP = _T("+ (EH+WP) ");
    }

    CString strDesc = _T("");
    /// (203-11) D+H+F+0.75(L+T+(Lr or R)]
    std::vector<CombBase*> vCombBase;
    vCombBase.emplace_back(cLR);
    vCombBase.emplace_back(cRL);
	INT_PTR nSize = vCombBase.size();
    for ( int i=0; i<nSize; ++i )
    {
        CombBase* CurBase = vCombBase.at(i);
        strDesc.Format(_T("(DL+FP) %s+ 0.75(LL+TL+%s) + (SFAC)%s"), strLSP, CurBase->Get_Type(), strPsTs);
        Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cFP, dSP, cEPH, dSP, cWP, 0.75, cLL, 0.75, cCSL, 0.75, cTL, 0.75, CurBase, dPSS, cPsTs);
    }
}

void CLoadCombCtrl::Make_SubComb_EQ12_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPH, CombBase* cWP,
    CombBase* cFP, CombBase* cWL, CombBase* cEL, CombBase* cESP, CombBase* cPsTs, const CString& strPsTs, double dPSS, BOOL bSlcmSpecial)
{
    const bool bRedundancyFact = m_bRedundancyFact;
    if ( bSlcmSpecial )
	{
		m_bRedundancyFact = false; 
	}

    /// (203-7) 0.9D+1.0E+1.6H
    const double dDL = 0.9;
    CString strAddEL  = m_bRedundancyFact && !bSlcmSpecial ? _T("(RF)") : _T("");
    CString strAddESP = _T("(SUF)");
    if ( bSlcmSpecial )
    {
        strAddEL  += _T("(OMG)");
        strAddESP += _T("(OMG)");
    }

    double dSP = 0.0;
    CString strLSP = _T("");
    if ( IsConsiderLateralSoil(cEPH, cWP) )
    {
        dSP = m_dLateralSoilFactor;
        strLSP = _T("+ (LSPF)(EH+WP) ");
    }
        
    CString strDesc = _T("");
    /// (203-12) D+H+F+(0.6W or E/1.4)
    strDesc.Format(_T("(DL+FP) %s+ 0.6WL + (SFAC)%s"), strLSP, strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cFP, dSP, cEPH, dSP, cWP,  0.6, cWL, dPSS, cPsTs);
    strDesc.Format(_T("(DL+FP) %s- 0.6WL + (SFAC)%s"), strLSP, strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cFP, dSP, cEPH, dSP, cWP, -0.6, cWL, dPSS, cPsTs);
        
    const double dFactE = 1.0/1.4;
    strDesc.Format(_T("(DL+FP) %s+ %sEL/1.4 + (SFAC)%s"), strLSP, strAddEL, strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cFP, dSP, cEPH, dSP, cWP, dFactE, cEL, dPSS, cPsTs);

    strDesc.Format(_T("(DL+FP) %s- %sEL/1.4 + (SFAC)%s"), strLSP, strAddEL, strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cFP, dSP, cEPH, dSP, cWP,-dFactE, cEL, dPSS, cPsTs);

    strDesc.Format(_T("(DL+FP) %s+ %sESP/1.4 + (SFAC)%s"), strLSP, strAddESP, strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cFP, dSP, cEPH, dSP, cWP, dFactE, cESP, dPSS, cPsTs);

    strDesc.Format(_T("(DL+FP) %s- %sESP/1.4 + (SFAC)%s"), strLSP, strAddESP, strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cFP, dSP, cEPH, dSP, cWP,-dFactE, cESP, dPSS, cPsTs);

    m_bRedundancyFact = bRedundancyFact;
}

#pragma endregion

#pragma region /// [GEN-3892] NSCP 2015 (for ALTERNATIVE)

void CLoadCombCtrl::Make_SubComb_EQ13_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPH, CombBase* cWP, CombBase* cFP, CombBase* cLL, CombBase* cCSL,
    CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cESP, CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    double dSP = 0.0;
    CString strLSP = _T("");
    if ( IsConsiderLateralSoil(cEPH, cWP) )
    {
        dSP = 1.0;
        strLSP = _T("+ (EH+WP) ");
    }

    const double dWL = 0.45; //0.6*0.75;
    CString strDesc = _T("");
    /// (203-13) D + H + F + 0.75[L + LR + (0.6W or E/1.4)]
    strDesc.Format(_T("(DL+FP) %s+ 0.75LL + 0.75LR + 0.45WL + (SFAC)%s"), strLSP, strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cFP, dSP, cEPH, dSP, cWP, 0.75, cLL, 0.75, cCSL, 0.75, cLR, dWL, cWL, dPSS, cPsTs);
    strDesc.Format(_T("(DL+FP) %s+ 0.75LL + 0.75LR - 0.45WL + (SFAC)%s"), strLSP, strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cFP, dSP, cEPH, dSP, cWP, 0.75, cLL, 0.75, cCSL, 0.75, cLR,-dWL, cWL, dPSS, cPsTs);

    const double dFactE = 0.75/1.4;
    const CString& strAddEL = m_bRedundancyFact ? _T("(RF)") : _T("");

    strDesc.Format(_T("(DL+FP) %s+ 0.75LL + 0.75LR + %sEL/1.4 + (SFAC)%s"), strLSP, strAddEL, strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cFP, dSP, cEPH, dSP, cWP, 0.75, cLL, 0.75, cCSL, 0.75, cLR, dFactE, cEL, dPSS, cPsTs);
    strDesc.Format(_T("(DL+FP) %s+ 0.75LL + 0.75LR - %sEL/1.4 + (SFAC)%s"), strLSP, strAddEL, strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cFP, dSP, cEPH, dSP, cWP, 0.75, cLL, 0.75, cCSL, 0.75, cLR,-dFactE, cEL, dPSS, cPsTs);
    strDesc.Format(_T("(DL+FP) %s+ 0.75LL + 0.75LR + (SUF)ESP/1.4 + (SFAC)%s"), strLSP, strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cFP, dSP, cEPH, dSP, cWP, 0.75, cLL, 0.75, cCSL, 0.75, cLR, dFactE, cESP, dPSS, cPsTs);
    strDesc.Format(_T("(DL+FP) %s+ 0.75LL + 0.75LR - (SUF)ESP/1.4 + (SFAC)%s"), strLSP, strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cFP, dSP, cEPH, dSP, cWP, 0.75, cLL, 0.75, cCSL, 0.75, cLR,-dFactE, cESP, dPSS, cPsTs);
}

void CLoadCombCtrl::Make_SubComb_EQ14_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cWL, CombBase* cEPH, CombBase* cWP,
    CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    double dSP = 0.0;
    CString strLSP = _T("");
    if ( IsConsiderLateralSoil(cEPH, cWP) )
    {
        dSP = 1.0;
        strLSP = _T("+ (EH+WP) ");
    }

    CString strDesc = _T("");
    /// (203-14) 0.6D + 0.6W + H
    strDesc.Format(_T("0.6DL + 0.6WL %s+ (SFAC)%s"), strLSP, strPsTs);
    Set_LCB(nActive, strDesc, 0.6, cDL, 0.6, cCSD, 0.6, cWL, dSP, cEPH, dSP, cWP, dPSS, cPsTs);
    strDesc.Format(_T("0.6DL - 0.6WL %s+ (SFAC)%s"), strLSP, strPsTs);
    Set_LCB(nActive, strDesc, 0.6, cDL, 0.6, cCSD,-0.6, cWL, dSP, cEPH, dSP, cWP, dPSS, cPsTs);
}

void CLoadCombCtrl::Make_SubComb_EQ15_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEL, CombBase* cESP, CombBase* cEPH, CombBase* cWP,
    CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    double dSP = 0.0;
    CString strLSP = _T("");
    if ( IsConsiderLateralSoil(cEPH, cWP) )
    {
        dSP = 1.0;
        strLSP = _T("+ (EH+WP) ");
    }

    CString strDesc = _T("");
    const double dFactD = 0.6;
    const double dFactE = 1.0/1.4;
    const CString& strAddEL = m_bRedundancyFact ? _T("(RF)") : _T("");
    /// (203-15) 0.6D + E/1.4 + H
    strDesc.Format(_T("0.6DL + %sEL/1.4 %s+ (SFAC)%s"), strAddEL, strLSP, strPsTs);
    Set_LCB(nActive, strDesc, dFactD, cDL, dFactD, cCSD, dFactE, cEL, dSP, cEPH, dSP, cWP, dPSS, cPsTs);

    strDesc.Format(_T("0.6DL - %sEL/1.4 %s+ (SFAC)%s"), strAddEL, strLSP, strPsTs);
    Set_LCB(nActive, strDesc, dFactD, cDL, dFactD, cCSD,-dFactE, cEL, dSP, cEPH, dSP, cWP, dPSS, cPsTs);

    strDesc.Format(_T("0.6DL + (SUF)ESP/1.4 %s+ (SFAC)%s"), strLSP, strPsTs);
    Set_LCB(nActive, strDesc, dFactD, cDL, dFactD, cCSD, dFactE, cESP, dSP, cEPH, dSP, cWP, dPSS, cPsTs);

    strDesc.Format(_T("0.6DL - (SUF)ESP/1.4 %s+ (SFAC)%s"), strLSP, strPsTs);
    Set_LCB(nActive, strDesc, dFactD, cDL, dFactD, cCSD,-dFactE, cESP, dSP, cEPH, dSP, cWP, dPSS, cPsTs);
}

void CLoadCombCtrl::Make_SubComb_EQ16_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cLR, CombBase* cRL,
    CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    CString strDesc = _T("");
    /// (203-16) D+L+(Lr or R)
    std::vector<CombBase*> vCombBase;
    vCombBase.emplace_back(cLR);
    vCombBase.emplace_back(cRL);
    const INT_PTR nSize = vCombBase.size();
    for ( INT_PTR i=0; i<nSize; ++i )
    {
        CombBase* CurBase = vCombBase.at(i);
        strDesc.Format(_T("DL + LL + %s + (SFAC)%s"), CurBase->Get_Type(), strPsTs);
        Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cLL, 1.0, cCSL, 1.0, CurBase, dPSS, cPsTs);
    }    
}

void CLoadCombCtrl::Make_SubComb_EQ17_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cWL,
    CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    CString strDesc = _T("");
    /// (203-17) D+L+(0.6W)
    strDesc.Format(_T("DL + LL + 0.6WL + (SFAC)%s"), strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cLL, 1.0, cCSL, 0.6, cWL, dPSS, cPsTs);
    strDesc.Format(_T("DL + LL - 0.6WL + (SFAC)%s"), strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cLL, 1.0, cCSL,-0.6, cWL, dPSS, cPsTs);
}

void CLoadCombCtrl::Make_SubComb_EQ18_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cEL,
    CombBase* cESP, CombBase* cPsTs, const CString& strPsTs, double dPSS)
{
    CString strDesc = _T("");
    const double dFactE = 1.0/1.4;
    const CString& strAddEL = m_bRedundancyFact ? _T("(RF)") : _T("");
    /// (203-18) D+L+E/1.4    
    strDesc.Format(_T("DL + LL + %sEL/1.4 + (SFAC)%s"), strAddEL, strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cLL, 1.0, cCSL, dFactE, cEL, dPSS, cPsTs);

    strDesc.Format(_T("DL + LL - %sEL/1.4 + (SFAC)%s"), strAddEL, strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cLL, 1.0, cCSL,-dFactE, cEL, dPSS, cPsTs);

    strDesc.Format(_T("DL + LL + (SUF)ESP/1.4 + (SFAC)%s"), strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cLL, 1.0, cCSL, dFactE, cESP, dPSS, cPsTs);

    strDesc.Format(_T("DL + LL - (SUF)ESP/1.4 + (SFAC)%s"), strPsTs);
    Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cLL, 1.0, cCSL,-dFactE, cESP, dPSS, cPsTs);
}

#pragma endregion

#pragma endregion   

#pragma region /// [GEN-8768] TWN-USD112

void CLoadCombCtrl::Make_SubComb_TWN_USD112_EQ5_3_1a(int nA, CombBase* cDL, CombBase* cCSD,
    CombBase* cFP, CombBase* cPS, double dPS, const CString& strPS, BOOL bSlcmSpecial)
{
	// [TWN-USD112, 5.3.1a] U = 1.4D.
    const double dDL = 1.4;
	
    const int nActive = GetActiveIndex(nA, bSlcmSpecial, FALSE);	
	CString strDesc = _T("");

	strDesc.Format(_T("1.4(DL+FP) + (SFAC)%s"), strPS);
	Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dPS, cPS);
}

void CLoadCombCtrl::Make_SubComb_TWN_USD112_EQ5_3_1b(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cFP, 
    CombBase* cLL, CombBase* cCSL, CombBase* cIL, CombBase* cLR, CombBase* cSL, CombBase* cRL,
    CombBase* cPS, double dPS, const CString& strPS, BOOL bSlcmSpecial)
{
	// [TWN-USD112, 5.3.1b] U = 1.2D + 1.6L + 0.5(Lr or S or R).
    const double dDL = 1.2;
    const double dLL = 1.6;
    const double dLR = 0.5;

	const int nActive = GetActiveIndex(nA, bSlcmSpecial, FALSE);	
	CString strDesc = _T("");

	// In case of m_bLateralSoil==FALSE, Set m_dLateralSoilFactor=0.0 in Initialize();
	if (!cLR->IsExist() && !cSL->IsExist() && !cRL->IsExist())
	{
		strDesc.Format(_T("1.2(DL+FP) + 1.6(LL+IL) + (SFAC)%s"), strPS);
		Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dLL, cLL, dLL, cCSL, dLL, cIL, dPS, cPS);
	}
	else
	{
		CString strCaseLR[3] = { _T("LR"), _T("SL"), _T("RL") };
		CombBase* cLoadLR[3] = { cLR, cSL, cRL };

		for (int i = 0; i < 3; i++)
		{
			strDesc.Format(_T("1.2(DL+FP) + 1.6(LL+IL) + 0.5%s+ (SFAC)%s"), strCaseLR[i], strPS);
			Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dLL, cLL, dLL, cCSL, dLL, cIL, dLR, cLoadLR[i], dPS, cPS);
		}
	}
}

void CLoadCombCtrl::Make_SubComb_TWN_USD112_EQ5_3_1c(int nA, BOOL bExistLL,
	CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
	CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cWL, CombBase* cPS, double dPS, const CString& strPS,
	BOOL bSlcmSpecial)
{
	// [TWN-USD112, 5.3.1c] U = 1.2D + 1.6(Lr or S or R) + (1.0L or 0.8W)
    const double dDL = 1.2;
    const double dLR = 1.6;
    const double dLL = 1.0;
    const double dWL = 0.8;

	const int nActive = GetActiveIndex(nA, bSlcmSpecial, FALSE);	
	CString strDesc = _T("");
    	
	CString strCaseLR[3] = { _T("LR"), _T("SL"), _T("RL") };
	CombBase* cLoadLR[3] = { cLR, cSL, cRL };    

	if (!bExistLL && !cWL->IsExist()) //1.2D + 1.6(LR or S or R).
	{
		for (int i = 0; i < 3; i++)
		{
			strDesc.Format(_T("1.2(DL+FP) + 1.6%s + (SFAC)%s"), strCaseLR[i], strPS);
			Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dLR, cLoadLR[i], dPS, cPS);
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			strDesc.Format(_T("1.2(DL+FP) + 1.6%s + 1.0(LL+IL) + (SFAC)%s"), strCaseLR[i], strPS);
			Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dLR, cLoadLR[i], dLL, cLL, dLL, cCSL, dLL, cIL, dPS, cPS);

			strDesc.Format(_T("1.2(DL+FP) + 1.6%s + %.1fWL + (SFAC)%s"), strCaseLR[i], dWL, strPS);
			Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dLR, cLoadLR[i], dWL, cWL, dPS, cPS);
			strDesc.Format(_T("1.2(DL+FP) + 1.6%s - %.1fWL + (SFAC)%s"), strCaseLR[i], dWL, strPS);
			Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dLR, cLoadLR[i], -dWL, cWL, dPS, cPS);
		}
	}
}

void CLoadCombCtrl::Make_SubComb_TWN_USD112_EQ5_3_1d(int nA, BOOL bExistLL, 
    CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL, CombBase* cLR, CombBase* cSL,
    CombBase* cRL, CombBase* cWL, CombBase* cPS, double dPS, const CString& strPS, BOOL bSlcmSpecial)
{
	// [TWN-USD112, 5.3.1d] U = 1.2D + 1.6W + 1.0L + 0.5(Lr or S or R).
    const double dDL = 1.2;
    const double dWL = 1.6;
    const double dLL = 1.0;
    const double dLR = 0.5;

	const int nActive = GetActiveIndex(nA, bSlcmSpecial, FALSE);
	CString strDesc = _T("");

	BOOL bExistLRSeries = !cLR->IsExist() && !cSL->IsExist() && !cRL->IsExist() ? FALSE : TRUE;
	if (bExistLRSeries)
	{
		CString strCaseLR[3] = { _T("LR"), _T("SL"), _T("RL") };
		CombBase* cLoadLR[3] = { cLR, cSL, cRL };

		if (bExistLL)
		{
			for (int i = 0; i < 3; ++i)
			{
				strDesc.Format(_T("1.2(DL+FP) + 1.6WL + 1.0(LL+IL) + 0.5%s + (SFAC)%s"), strCaseLR[i], strPS);
				Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dWL, cWL, dLL, cLL, dLL, cCSL, dLL, cIL, dLR, cLoadLR[i], dPS, cPS);

				strDesc.Format(_T("1.2(DL+FP) - 1.6WL + 1.0(LL+IL) + 0.5%s + (SFAC)%s"), strCaseLR[i], strPS);
				Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP,-dWL, cWL, dLL, cLL, dLL, cCSL, dLL, cIL, dLR, cLoadLR[i], dPS, cPS);
			}
		}
		else
		{
			for (int i = 0; i < 3; ++i)
			{
				strDesc.Format(_T("1.2(DL+FP) + 1.6WL + 0.5%s + (SFAC)%s"), strCaseLR[i], strPS);
				Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dWL, cWL, dLR, cLoadLR[i], dPS, cPS);

				strDesc.Format(_T("1.2(DL+FP) - 1.6WL + 0.5%s + (SFAC)%s"), strCaseLR[i], strPS);
				Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP,-dWL, cWL, dLR, cLoadLR[i], dPS, cPS);
			}
		}
	}
	else
	{
		if (bExistLL)
		{
			strDesc.Format(_T("1.2(DL+FP) + 1.6WL + 1.0(LL+IL) + (SFAC)%s"), strPS);
			Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dWL, cWL, dLL, cLL, dLL, cCSL, dLL, cIL, dPS, cPS);
			strDesc.Format(_T("1.2(DL+FP) - 1.6WL + 1.0(LL+IL) + (SFAC)%s"), strPS);
			Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP,-dWL, cWL, dLL, cLL, dLL, cCSL, dLL, cIL, dPS, cPS);
		}
		else
		{
			strDesc.Format(_T("1.2(DL+FP) + 1.6WL + (SFAC)%s"), strPS);
			Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dWL, cWL, dPS, cPS);
			strDesc.Format(_T("1.2(DL+FP) - 1.6WL + (SFAC)%s"), strPS);
			Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP,-dWL, cWL, dPS, cPS);
		}
	}
}

void CLoadCombCtrl::Make_SubComb_TWN_USD112_EQ5_3_1e(int nA, BOOL bExistLL, 
    CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL, CombBase* cSL, 
    CombBase* cEL, CombBase* cESP, CombBase* cPS, double dPS, const CString& strPS, BOOL bSlcmSpecial)
{
	// [TWN-USD112, 5.3.1e] U = 1.2D + 1.0E + 1.0L + 0.2S.
    const double dDL = 1.2;
    const double dEL = 1.0;
    const double dLL = 1.0;
    const double dSL = 0.2;
	const int nActive = GetActiveIndex(nA, bSlcmSpecial, FALSE);
	CString strDesc = _T("");

	const double dDLp12 = 1.20 + m_dSlcmSds * m_dSlcmSpecial; // for 1.2D + [(OMG)E + 0.2SDS*D]

	if (cSL->IsExist())
	{
		if (bExistLL)
		{
			if (!bSlcmSpecial)
			{
				strDesc.Format(_T("1.2(DL+FP) + %.1fEL + 1.0(LL+IL) + 0.2SL + (SFAC)%s"), dEL, strPS);       Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dEL, cEL , dLL, cLL, dLL, cCSL, dLL, cIL, dSL, cSL, dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) - %.1fEL + 1.0(LL+IL) + 0.2SL + (SFAC)%s"), dEL, strPS);       Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP,-dEL, cEL , dLL, cLL, dLL, cCSL, dLL, cIL, dSL, cSL, dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) + %.1f(SUF)ESP + 1.0(LL+IL) + 0.2SL + (SFAC)%s"), dEL, strPS); Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dEL, cESP, dLL, cLL, dLL, cCSL, dLL, cIL, dSL, cSL, dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) - %.1f(SUF)ESP + 1.0(LL+IL) + 0.2SL + (SFAC)%s"), dEL, strPS); Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP,-dEL, cESP, dLL, cLL, dLL, cCSL, dLL, cIL, dSL, cSL, dPS, cPS);
			}
			else
			{
				strDesc.Format(_T("1.2(DL+FP) + %.1f(OMG)EL + 1.0(LL+IL) + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), dEL, strPS);       Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDL, cFP, dEL, cEL , dLL, cLL, dLL, cCSL, dLL, cIL, dSL, cSL, dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) - %.1f(OMG)EL + 1.0(LL+IL) + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), dEL, strPS);       Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDL, cFP,-dEL, cEL , dLL, cLL, dLL, cCSL, dLL, cIL, dSL, cSL, dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) + %.1f(SUF)(OMG)ESP + 1.0(LL+IL) + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), dEL, strPS); Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDL, cFP, dEL, cESP, dLL, cLL, dLL, cCSL, dLL, cIL, dSL, cSL, dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) - %.1f(SUF)(OMG)ESP + 1.0(LL+IL) + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), dEL, strPS); Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDL, cFP,-dEL, cESP, dLL, cLL, dLL, cCSL, dLL, cIL, dSL, cSL, dPS, cPS);
			}
		}
		else
		{
			if (!bSlcmSpecial)
			{
				strDesc.Format(_T("1.2(DL+FP) + %.1fEL + 0.2SL + (SFAC)%s"), dEL, strPS);       Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dEL, cEL , dSL, cSL, dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) - %.1fEL + 0.2SL + (SFAC)%s"), dEL, strPS);       Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP,-dEL, cEL , dSL, cSL, dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) + %.1f(SUF)ESP + 0.2SL + (SFAC)%s"), dEL, strPS); Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dEL, cESP, dSL, cSL, dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) - %.1f(SUF)ESP + 0.2SL + (SFAC)%s"), dEL, strPS); Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP,-dEL, cESP, dSL, cSL, dPS, cPS);
			}
			else
			{
				strDesc.Format(_T("1.2(DL+FP) + %.1f(OMG)EL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), dEL, strPS);       Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDL, cFP, dEL, cEL, dSL, cSL, dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) - %.1f(OMG)EL + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), dEL, strPS);       Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDL, cFP,-dEL, cEL, dSL, cSL, dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) + %.1f(SUF)(OMG)ESP + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), dEL, strPS); Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDL, cFP, dEL, cESP, dSL, cSL, dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) - %.1f(SUF)(OMG)ESP + 0.2SL + (SFAC)%s + (SPC)(SDS)DL"), dEL, strPS); Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDL, cFP,-dEL, cESP, dSL, cSL, dPS, cPS);
			}
		}
	}
	else
	{
		if (bExistLL)
		{
			if (!bSlcmSpecial)
			{
				strDesc.Format(_T("1.2(DL+FP) + %.1fEL + 1.0(LL+IL) + (SFAC)%s"), dEL, strPS);       Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dEL, cEL , dLL, cLL, dLL, cCSL, dLL, cIL, dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) - %.1fEL + 1.0(LL+IL) + (SFAC)%s"), dEL, strPS);       Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP,-dEL, cEL , dLL, cLL, dLL, cCSL, dLL, cIL, dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) + %.1f(SUF)ESP + 1.0(LL+IL) + (SFAC)%s"), dEL, strPS); Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dEL, cESP, dLL, cLL, dLL, cCSL, dLL, cIL, dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) - %.1f(SUF)ESP + 1.0(LL+IL) + (SFAC)%s"), dEL, strPS); Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP,-dEL, cESP, dLL, cLL, dLL, cCSL, dLL, cIL, dPS, cPS);
			}
			else
			{
				strDesc.Format(_T("1.2(DL+FP) + %.1f(OMG)EL + 1.0(LL+IL) + (SFAC)%s + (SPC)(SDS)DL"), dEL, strPS);       Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDL, cFP, dEL, cEL , dLL, cLL, dLL, cCSL, dLL, cIL, dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) - %.1f(OMG)EL + 1.0(LL+IL) + (SFAC)%s + (SPC)(SDS)DL"), dEL, strPS);       Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDL, cFP,-dEL, cEL , dLL, cLL, dLL, cCSL, dLL, cIL, dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) + %.1f(SUF)(OMG)ESP + 1.0(LL+IL) + (SFAC)%s + (SPC)(SDS)DL"), dEL, strPS); Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDL, cFP, dEL, cESP, dLL, cLL, dLL, cCSL, dLL, cIL, dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) - %.1f(SUF)(OMG)ESP + 1.0(LL+IL) + (SFAC)%s + (SPC)(SDS)DL"), dEL, strPS); Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDL, cFP,-dEL, cESP, dLL, cLL, dLL, cCSL, dLL, cIL, dPS, cPS);
			}
		}
		else
		{
			if (!bSlcmSpecial)
			{
				strDesc.Format(_T("1.2(DL+FP) + %.1fEL + (SFAC)%s"), dEL, strPS);       Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dEL, cEL , dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) - %.1fEL + (SFAC)%s"), dEL, strPS);       Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP,-dEL, cEL , dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) + %.1f(SUF)ESP + (SFAC)%s"), dEL, strPS); Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dEL, cESP, dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) - %.1f(SUF)ESP + (SFAC)%s"), dEL, strPS); Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP,-dEL, cESP, dPS, cPS);
			}
			else
			{
				strDesc.Format(_T("1.2(DL+FP) + %.1f(OMG)EL + (SFAC)%s + (SPC)(SDS)DL"), dEL, strPS);       Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDL, cFP, dEL, cEL , dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) - %.1f(OMG)EL + (SFAC)%s + (SPC)(SDS)DL"), dEL, strPS);       Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDL, cFP,-dEL, cEL , dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) + %.1f(SUF)(OMG)ESP + (SFAC)%s + (SPC)(SDS)DL"), dEL, strPS); Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDL, cFP, dEL, cESP, dPS, cPS);
				strDesc.Format(_T("1.2(DL+FP) - %.1f(SUF)(OMG)ESP + (SFAC)%s + (SPC)(SDS)DL"), dEL, strPS); Set_LCB(nActive, strDesc, dDLp12, cDL, dDLp12, cCSD, dDL, cFP,-dEL, cESP, dPS, cPS);
			}
		}
	}
}

void CLoadCombCtrl::Make_SubComb_TWN_USD112_EQ5_3_1f(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cWL, 
    CombBase* cPS, double dPS, const CString& strPS, BOOL bSlcmSpecial)
{
	// [TWN-USD112, 5.3.1f] U = 0.9D + 1.6W.
    const double dDL = 0.9;    
    const double dWL = 1.6;
	const int nActive = GetActiveIndex(nA, bSlcmSpecial, FALSE);
	CString strDesc = _T("");
    	
	if (!cWL->IsExist())
	{
		strDesc.Format(_T("0.9DL + (SFAC)%s"), strPS);
		Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dPS, cPS);
	}
	else
	{
		strDesc.Format(_T("0.9DL + 1.6WL + (SFAC)%s"), strPS);
		Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dWL, cWL, dPS, cPS);
		strDesc.Format(_T("0.9DL - 1.6WL + (SFAC)%s"), strPS);
		Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD,-dWL, cWL, dPS, cPS);
	}
}

void CLoadCombCtrl::Make_SubComb_TWN_USD112_EQ5_3_1g(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cFP, 
    CombBase* cEL, CombBase* cESP, CombBase* cPS, double dPS, const CString& strPS, BOOL bSlcmSpecial)
{
	// [TWN-USD112, 5.3.1g] U = 0.9D+1.0E
    const double dDL = 0.9;
    const double dEL = 1.0;
    const double dDLm9 = 0.9 - m_dSlcmSds * m_dSlcmSpecial; // for 0.9D + [(OMG)E - 0.2SDS*D] 
	const int nActive = GetActiveIndex(nA, bSlcmSpecial, FALSE);    
	CString strDesc = _T("");
    
	if (!cEL->IsExist() && !cESP->IsExist())
	{
		strDesc.Format(_T("0.9(DL+FP) + (SFAC)%s"), strPS);
		Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dPS, cPS);
	}
	else
	{
		if (!bSlcmSpecial)
		{
			strDesc.Format(_T("0.9(DL+FP) + 1.0EL + (SFAC)%s"), strPS);
			Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dEL, cEL, dPS, cPS);

			strDesc.Format(_T("0.9(DL+FP) - 1.0EL + (SFAC)%s"), strPS);
			Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP,-dEL, cEL, dPS, cPS);

			strDesc.Format(_T("0.9(DL+FP) + 1.0(SUF)ESP + (SFAC)%s"), strPS);
			Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP, dEL, cESP, dPS, cPS);

			strDesc.Format(_T("0.9(DL+FP) - 1.0(SUF)ESP + (SFAC)%s"), strPS);
			Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dDL, cFP,-dEL, cESP, dPS, cPS);
		}
		else
		{
			strDesc.Format(_T("0.9(DL+FP) + 1.0(OMG)EL + (SFAC)%s - (SPC)(SDS)DL"), strPS);
			Set_LCB(nActive, strDesc, dDLm9, cDL, dDLm9, cCSD, dDL, cFP, dEL, cEL, dPS, cPS);

			strDesc.Format(_T("0.9(DL+FP) - 1.0(OMG)EL + (SFAC)%s - (SPC)(SDS)DL"), strPS);
			Set_LCB(nActive, strDesc, dDLm9, cDL, dDLm9, cCSD, dDL, cFP,-dEL, cEL, dPS, cPS);

			strDesc.Format(_T("0.9(DL+FP) + 1.0(SUF)(OMG)ESP + (SFAC)%s - (SPC)(SDS)DL"), strPS);
			Set_LCB(nActive, strDesc, dDLm9, cDL, dDLm9, cCSD, dDL, cFP, dEL, cESP, dPS, cPS);

			strDesc.Format(_T("0.9(DL+FP) - 1.0(SUF)(OMG)ESP + (SFAC)%s - (SPC)(SDS)DL"), strPS);
			Set_LCB(nActive, strDesc, dDLm9, cDL, dDLm9, cCSD, dDL, cFP,-dEL, cESP, dPS, cPS);
		}
	}
}

#pragma endregion

#pragma region /// Thailand(2021)
void CLoadCombCtrl::MakeComb_Thailand_2021(CombBase* cDL, CombBase* cLL, CombBase* cWL, CombBase* cEL, CombBase* cTL, CombBase* cWP, CombBase* cEP,
    CombBase* cFP, CombBase* cESP, CombBase* cCSD, CombBase* cCSL)
{
	const int iLcomType = Get_CurrentLoadCombType();
	const BOOL bIsFDN = ((iLcomType == D_LCOMTYPE_FDN || m_nDesignType == 3) ? TRUE : FALSE);

	int nA = 1 + m_nConstLoad * 100;
	BOOL bExistLL = IsExistLL(m_nConstLoad, cLL, cCSL);
	SetCombBaseConsiderCS(m_nConstLoad, &cDL, &cCSD, &cLL, &cCSL);
    	
	SetAddSeisLcomParam();

	const BOOL bStageCS = IsStageCS();

	if (m_nLcomType == D_LCOMTYPE_GENERAL)	Make_LCB_OnlyRespEccn(nA, cESP);
    	
	m_iSetLcomForKBC = 0;
	Set_PrevLcomSRSS(FALSE, 1.0, 1.0, cEL, cESP);
    MakeWindLoadGroupMapKBC2016(m_mWindGroup);
    Set_WindDirCombKBC2016();
    
    /// strength load combinations
	const int nA_STRN = nA;
	Make_SubComb_Thai_Strn_EQ1(nA_STRN, cDL, cCSD, cLL, cCSL);
    Make_SubComb_Thai_Strn_EQ2(nA_STRN, cDL, cCSD, cLL, cCSL, cWL);
    Make_SubComb_Thai_Strn_EQ3(nA_STRN, cDL, cCSD, cLL, cCSL, cEL, cESP, FALSE);
    Make_SubComb_Thai_Strn_EQ4(nA_STRN, cDL, cCSD, cLL, cCSL, cEP, cFP);
    Make_SubComb_Thai_Strn_EQ5(nA_STRN, cDL, cCSD, cLL, cCSL, cTL);
	
    /// serviceability load combinations
	const int nA_SERV = nA + 1;
	Make_SubComb_Thai_Serv_EQ1(nA_SERV, cDL, cCSD, cLL, cCSL);
	Make_SubComb_Thai_Serv_EQ2(nA_SERV, cDL, cCSD, cLL, cCSL, cWL);
	Make_SubComb_Thai_Serv_EQ3(nA_SERV, cDL, cCSD, cLL, cCSL, cEL, cESP, FALSE);
	Make_SubComb_Thai_Serv_EQ4(nA_SERV, cDL, cCSD, cLL, cCSL, cTL, cEP, cFP);
    	
	/// special seismic load combinations
	if (m_bSlcmSpecial)
	{
		m_iSetLcomForKBC = 1;
		Set_PrevLcomSRSS(TRUE, 1.0, 1.0, cEL, cESP);

		const int nA_SPEC = nA + 3;
		Make_SubComb_Thai_Strn_EQ1(nA_SPEC, cDL, cCSD, cLL, cCSL);
		Make_SubComb_Thai_Strn_EQ2(nA_SPEC, cDL, cCSD, cLL, cCSL, cWL);
		Make_SubComb_Thai_Strn_EQ3(nA_SPEC, cDL, cCSD, cLL, cCSL, cEL, cESP, TRUE);
		Make_SubComb_Thai_Strn_EQ4(nA_SPEC, cDL, cCSD, cLL, cCSL, cEP, cFP);
		Make_SubComb_Thai_Strn_EQ5(nA_SPEC, cDL, cCSD, cLL, cCSL, cTL);		
	}

    /// vertical seismi load combinations
	if (m_bSlcmVertical)
	{
		m_iSetLcomForKBC = 2;
		Set_PrevLcomSRSS(FALSE, 1.0, 1.0, cEL, cESP);
		double dDLv = (-1.0) * m_dSlcmVertical;

		const int nA_VERT = nA + 4;
		Make_SubComb_Thai_Strn_EQ1(nA_VERT, cDL, cCSD, cLL, cCSL);
		Make_SubComb_Thai_Strn_EQ2(nA_VERT, cDL, cCSD, cLL, cCSL, cWL);
		Make_SubComb_Thai_Strn_EQ3(nA_VERT, cDL, cCSD, cLL, cCSL, cEL, cESP, FALSE);
		Make_SubComb_Thai_Strn_EQ4(nA_VERT, cDL, cCSD, cLL, cCSL, cEP, cFP);
		Make_SubComb_Thai_Strn_EQ5(nA_VERT, cDL, cCSD, cLL, cCSL, cTL);
        		
		Set_LCB(nA_VERT, _T("-(VER)DL + 1.0EL"), dDLv, cDL, dDLv, cCSD, 1.0, cEL);
		Set_LCB(nA_VERT, _T("-(VER)DL - 1.0EL"), dDLv, cDL, dDLv, cCSD, -1.0, cEL);
		Set_LCB(nA_VERT, _T("-(VER)DL + 1.0(SUF)ESP"), dDLv, cDL, dDLv, cCSD, 1.0, cESP);
		Set_LCB(nA_VERT, _T("-(VER)DL - 1.0(SUF)ESP"), dDLv, cDL, dDLv, cCSD, -1.0, cESP);
	}

	// Add by ZINU.('05.04.19). Consider SRSS.
	if (m_nOrthoType == 1)	m_iSetLcomForSRSS = 0;    
}

#pragma region /// Thailand(2021) - for strength design

void CLoadCombCtrl::Make_SubComb_Thai_Strn_EQ1(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL)
{
	/// only structures case.
	CString strDesc = _T("");
        
	strDesc.Format(_T("1.4DL"));
	Set_LCB(nActive, strDesc, 1.4, cDL, 1.4, cCSD);

	strDesc.Format(_T("1.4DL + 1.7LL"));
	Set_LCB(nActive, strDesc, 1.4, cDL, 1.4, cCSD, 1.7, cLL, 1.7, cCSL);
}

void CLoadCombCtrl::Make_SubComb_Thai_Strn_EQ2(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cWL)
{
	/// wind load case.
	CString strDesc = _T("");
    const double dDL = 0.75 * 1.4;
    const double dLL = 0.75 * 1.7;

	strDesc.Format(_T("0.75(1.4D + 1.7L) + 1.6WL"));
    Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dLL, cLL, dLL, cCSL, 1.6, cWL);

	strDesc.Format(_T("0.75(1.4D + 1.7L) - 1.6WL"));
	Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dLL, cLL, dLL, cCSL, -1.6, cWL);
    
	strDesc.Format(_T("0.9DL + 1.6WL"));
	Set_LCB(nActive, strDesc, 0.9, cDL, 0.9, cCSD, 1.6, cWL);

	strDesc.Format(_T("0.9DL - 1.6WL"));
	Set_LCB(nActive, strDesc, 0.9, cDL, 0.9, cCSD, -1.6, cWL);
}

void CLoadCombCtrl::Make_SubComb_Thai_Strn_EQ3(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cEL, CombBase* cESP, BOOL bSlcmSpecial)
{
    /// earthquake/seismic case.	
	CString strAddEL = _T("");
	CString strAddESP = _T("(SUF)");
	CString strAddDL = _T("");

	double dDL = 0.75 * 1.4;
	double dLL = 0.75 * 1.7;
	if (bSlcmSpecial)
	{
		dDL += m_dSlcmSds * m_dSlcmSpecial; // for (fact)D + (OMG)E + 0.2SDS*D
		strAddEL += _T("(OMG)");
		strAddESP += _T("(OMG)");
		strAddDL = _T(" + (SPC)(SDS)DL");
	}
        
	CString strDesc = _T("");
	
	strDesc.Format(_T("0.75(1.4DL + 1.7LL) + 1.0%sEL%s"), strAddEL, strAddDL);
	Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dLL, cLL, dLL, cCSL, 1.0, cEL);
    strDesc.Format(_T("0.75(1.4DL + 1.7LL) - 1.0%sEL%s"), strAddEL, strAddDL);
    Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dLL, cLL, dLL, cCSL, -1.0, cEL);

	strDesc.Format(_T("0.75(1.4DL + 1.7LL) + 1.0%sESP%s"), strAddESP, strAddDL);
	Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dLL, cLL, dLL, cCSL, 1.0, cESP);
	strDesc.Format(_T("0.75(1.4DL + 1.7LL) - 1.0%sESP%s"), strAddESP, strAddDL);
	Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dLL, cLL, dLL, cCSL, -1.0, cESP);

    dDL = 0.9;
    if (bSlcmSpecial)
    {
        dDL += m_dSlcmSds * m_dSlcmSpecial; // for (fact)D + (OMG)E + 0.2SDS*D
    }    

	strDesc.Format(_T("0.9DL + 1.0%sEL%s"), strAddEL, strAddDL);
	Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, 1.0, cEL);
	strDesc.Format(_T("0.9DL - 1.0%sEL%s"), strAddEL, strAddDL);
	Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, -1.0, cEL);

	strDesc.Format(_T("0.9DL + 1.0%sESP%s"), strAddESP, strAddDL);
	Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, 1.0, cESP);
	strDesc.Format(_T("0.9DL - 1.0%sESP%s"), strAddESP, strAddDL);
	Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, -1.0, cESP);
}

void CLoadCombCtrl::Make_SubComb_Thai_Strn_EQ4(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cEP, CombBase* cFP)
{
    /// earth pressure / vertical water pressure load / fluid pressure.
	CString strDesc = _T("");

	strDesc = _T("1.4DL + 1.7LL + 1.7EP");
	Set_LCB(nActive, strDesc, 1.4, cDL, 1.4, cCSD, 1.7, cLL, 1.7, cCSL, 1.7, cEP);

	strDesc = _T("0.9DL + 1.7EP");
	Set_LCB(nActive, strDesc, 0.9, cDL, 0.9, cCSD, 1.7, cEP);

    strDesc = _T("1.4DL + 1.7LL + 1.4FP");
	Set_LCB(nActive, strDesc, 1.4, cDL, 1.4, cCSD, 1.7, cLL, 1.7, cCSL, 1.4, cFP);

	strDesc = _T("0.9DL + 1.4FP");
	Set_LCB(nActive, strDesc, 0.9, cDL, 0.9, cCSD, 1.4, cFP);
}

void CLoadCombCtrl::Make_SubComb_Thai_Strn_EQ5(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cTL)
{
	/// thermal force case.
	CString strDesc = _T("");

	const double dDL = 0.75 * 1.4;
    const double dLL = 0.75 * 1.7;
	strDesc = _T("0.75(1.4DL + 1.4TL + 1.7LL)");
	Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, dLL, cLL, dLL, cCSL, dDL, cTL);

	strDesc = _T("1.4DL + 1.4TL");
	Set_LCB(nActive, strDesc, 1.4, cDL, 1.4, cCSD, 1.4, cTL);
}

#pragma endregion

#pragma region /// Thailand(2021) - service (ASD or WSD)

void CLoadCombCtrl::Make_SubComb_Thai_Serv_EQ1(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL)
{
	/// only structures case.
	CString strDesc = _T("");

	strDesc.Format(_T("1.0DL"));
	Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD);

	strDesc.Format(_T("1.0DL + 1.0LL"));
	Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 1.0, cLL, 1.0, cCSL);
}

void CLoadCombCtrl::Make_SubComb_Thai_Serv_EQ2(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cWL)
{
	/// wind load case.
	CString strDesc = _T("");
	
	strDesc.Format(_T("1.0DL + 0.75LL + 0.75WL"));
	Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 0.75, cLL, 0.75, cCSL, 0.75, cWL);

	strDesc.Format(_T("1.0DL + 0.75LL - 0.75WL"));
	Set_LCB(nActive, strDesc, 1.0, cDL, 1.0, cCSD, 0.75, cLL, 0.75, cCSL, -0.75, cWL);
    	
	strDesc.Format(_T("0.6DL + 1.0WL"));
	Set_LCB(nActive, strDesc, 0.6, cDL, 0.6, cCSD, 1.0, cWL);

	strDesc.Format(_T("0.6DL - 1.0WL"));
    Set_LCB(nActive, strDesc, 0.6, cDL, 0.6, cCSD, -1.0, cWL);
}

void CLoadCombCtrl::Make_SubComb_Thai_Serv_EQ3(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cEL, CombBase* cESP, BOOL bSlcmSpecial)
{
    /// earthquake/seismic case.	
    CString strAddEL = _T("");
    CString strAddESP = _T("(SUF)");
    CString strAddDL = _T("");

    double dDL = 1.0;
    if (bSlcmSpecial)
    {
        dDL += m_dSlcmSds * m_dSlcmSpecial; // for (fact)D + (OMG)E + 0.2SDS*D
        strAddEL += _T("(OMG)");
        strAddESP += _T("(OMG)");
        strAddDL = _T(" + (SPC)(SDS)DL");
    }

    CString strDesc = _T("");

    strDesc.Format(_T("1.0DL + 0.7%sEL%s"), strAddEL, strAddDL);
    Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, 0.7, cEL);
    strDesc.Format(_T("1.0DL - 0.7%sEL%s"), strAddEL, strAddDL);
    Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, -0.7, cEL);

    strDesc.Format(_T("1.0DL + 0.7%sESP%s"), strAddESP, strAddDL);
    Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, 0.7, cESP);
    strDesc.Format(_T("1.0DL - 0.7%sESP%s"), strAddESP, strAddDL);
    Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, -0.7, cESP);


    strDesc.Format(_T("1.0DL + 0.75LL + 0.525%sEL%s"), strAddEL, strAddDL);
    Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, 0.75, cLL, 0.75, cCSL, 0.525, cEL);
    strDesc.Format(_T("1.0DL + 0.75LL - 0.525%sEL%s"), strAddEL, strAddDL);
    Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, 0.75, cLL, 0.75, cCSL, -0.525, cEL);

    strDesc.Format(_T("1.0DL + 0.75LL + 0.525%sESP%s"), strAddESP, strAddDL);
    Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, 0.75, cLL, 0.75, cCSL, 0.525, cESP);
    strDesc.Format(_T("1.0DL + 0.75LL - 0.525%sESP%s"), strAddESP, strAddDL);
    Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, 0.75, cLL, 0.75, cCSL, -0.525, cESP);

    dDL = 0.6;
    if (bSlcmSpecial)
    {
        dDL += m_dSlcmSds * m_dSlcmSpecial; // for (fact)D + (OMG)E + 0.2SDS*D
    }

    strDesc.Format(_T("0.6DL + 0.7%sEL%s"), strAddEL, strAddDL);
    Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, 0.7, cEL);
    strDesc.Format(_T("0.6DL - 0.7%sEL%s"), strAddEL, strAddDL);
    Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, -0.7, cEL);

    strDesc.Format(_T("0.6DL + 0.7%sESP%s"), strAddESP, strAddDL);
    Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, 0.7, cESP);
    strDesc.Format(_T("0.6DL - 0.7%sESP%s"), strAddESP, strAddDL);
    Set_LCB(nActive, strDesc, dDL, cDL, dDL, cCSD, -0.7, cESP);
}

void CLoadCombCtrl::Make_SubComb_Thai_Serv_EQ4(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cTL, CombBase* cEP, CombBase* cFP)
{
    /// earth pressure / vertical water pressure load / fluid pressure and thermal force case
    CString strDesc = _T("");
    std::vector<std::pair<double, CombBase*>> vCombBase;        

    auto L_SetCombBase = [&](double dFact, CombBase* pLoad)
    {
        if (!pLoad->IsExist()) return;

		vCombBase.emplace_back(dFact, pLoad);
        if (!strDesc.IsEmpty()) { strDesc += _T(" + "); }
        strDesc.Format(_T("%s%.2f%s"), strDesc, dFact, pLoad->Get_Type());
    };

    L_SetCombBase(1.0, cDL);
    L_SetCombBase(1.0, cLL);
    L_SetCombBase(1.0, cEP);
    L_SetCombBase(1.0, cFP);
    L_SetCombBase(1.0, cTL);
	
    Set_LCB(nActive, strDesc, vCombBase);
}

#pragma endregion

#pragma endregion

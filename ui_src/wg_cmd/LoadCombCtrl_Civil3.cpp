#include "stdafx.h"
#include "wg_cmd.h"
#include "LoadCombCtrl.h"

#include "..\\wg_db\\wg_db_AttrCtrl.h"
#include "..\\wg_db\\wg_db_PostCtrl.h"
#include "..\wg_db\mvLoad.h"
#include "..\wg_db\VehlDef.h"
#include <unordered_set>

#if !defined D_STR
#define D_STR 0
#endif

#if !defined D_SER
#define D_SER 1
#endif

// for JSCE17
#if !defined D_PER // Permanent; 지속작용 지배상황
#define D_PER 0
#endif

#if !defined D_VAR // Trasient; 변동작용 지배상황
#define D_VAR 1
#endif

#if !defined D_ACC // Accidental; 우발작용 지배상황
#define D_ACC 2
#endif

using namespace std;

CString SerializeCombination(const T_LCOM_D& lcom)
{
    CString key;
    for (int j = 0; j < lcom.aCombination.GetSize(); ++j)
    {
        // Assuming each combination has LoadCaseKey and Factor as unique identifiers
        key.AppendFormat(_T("%d:%.6f;"), lcom.aCombination[j].LoadCaseKey, lcom.aCombination[j].Factor);
    }
    return key;
}

int CLoadCombCtrl::Get_LoadCount(const EN_LOAD& enLcase)
{
    switch (enLcase)
    {
    case EN_LOAD_D  :        return m_nDCount  ;
    case EN_LOAD_DC :        return m_nDcCount  ;
    case EN_LOAD_DD :        return m_nDdCount ;
    case EN_LOAD_DW :        return m_nDwCount;
    case EN_LOAD_L  :        return m_nLCount ;
    case EN_LOAD_IL :        return m_nIlCount ;
    case EN_LOAD_CRL:        return m_nCrlCount ;
    case EN_LOAD_PS :        return m_nPsCount ;
    case EN_LOAD_CR :        return m_nCrCount ;
    case EN_LOAD_SH :        return m_nShCount  ;
    case EN_LOAD_EP :        return m_nEpCount ;
    case EN_LOAD_EH :        return m_nEhCount ;
    case EN_LOAD_EV :        return m_nEvCount  ;
    case EN_LOAD_ES :        return m_nEsCount;
    case EN_LOAD_B  :        return m_nBCount ;
    case EN_LOAD_T :         return m_nTCount;
    case EN_LOAD_TPG :       return m_nTpgCount ;
    case EN_LOAD_S  :        return m_nSCount  ;
    case EN_LOAD_STL:        return m_nStlCount ;
    case EN_LOAD_SM :        return m_nSmCount;
    case EN_LOAD_M  :        return m_nMCount  ;
    case EN_LOAD_CF :        return m_nCfCount;
    case EN_LOAD_BRK:        return m_nBrkCount ;
    case EN_LOAD_IP :        return m_nIlpCount ;
    case EN_LOAD_FR :        return m_nFrCount  ;
    case EN_LOAD_W  :        return m_nWCount  ;
    case EN_LOAD_WL :        return m_nWlCount ;
    case EN_LOAD_WP :        return m_nWpCount;
    case EN_LOAD_FP :        return m_nFpCount ;
    case EN_LOAD_SF :        return m_nSfCount ;
    case EN_LOAD_WPR:        return m_nWprCount ;
    case EN_LOAD_E  :        return m_nECount ;
    case EN_LOAD_ESP:        return m_nEspCount ;
    case EN_LOAD_CO :        return m_nCoCount  ;
    case EN_LOAD_CT :        return m_nCtCount ;
    case EN_LOAD_CV :        return m_nCvCount ;
    case EN_LOAD_ER :        return m_nErCount  ;

    default:
        ASSERT(0);
        break;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////
////// JSCE02 additional Load combination with Wind
////////////////////////////////////////////////////////////////////////








void CLoadCombCtrl::Set_CvlJSCE_No3_1_2(CString strCode, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, 
                                        ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, 
                                        ArINT& arTgLoad, ArINT& arTlLoad, ArINT& arSLoad, ArINT& arCSLoad, ArINT& arWLoad, 
                                        ArINT& arGridD, ArINT& arGridDe, ArINT& arGridLv, ArINT& arGridCrl, ArINT& arGridLe, 
                                        int nStrType, BOOL bActive)
{
    if((m_nConstLoad==0 && m_nDCount==0 && m_nGDCount==0 && m_nGDeCount==0)) return;
    if(m_nWCount == 0) return;  // Wind Load는 반드시 있어야 함
    int nTgIteration = 1;
    if(nStrType == 2)  // Temperature + Wind
    {
        if(m_nTgCount == 0 && m_nTlCount == 0) return;   // 온도하중은 반드시 있어야 함
    }  

    int LoadCaseNo = 0, n=0;
    T_LCOM_D rData;
    double dFactor=1.0, dTgFactor=1.0, dTlFactor=1.0;
    double dPST = (m_bLossFactor ? m_dLossFactorTransfer : 1.0);
    double dPSS = (m_bLossFactor ? m_dLossFactorService  : 1.0);
    // DL는 항상포함, IL과 CF는 LL가 있는 경우만 포함
    int  nSmCountTemp, nTgCountTemp;   // load의 개수가 0 인 경우 아래 루틴을 사용하기 위해 임시로 설정한 load의 개수
    nSmCountTemp  = max(m_nSmCount, 1);
    nTgCountTemp  = max(m_nTgCount, 1);  
    CString strFixSm, strFixTg, strFixW;

    BOOL bStageCS=FALSE;
    if(m_pDoc->IsPostMode()) bStageCS = m_pDoc->m_pPostCtrl->StageAnalysis();
    else										 bStageCS = m_bCSAnalysis;

    for(int k = 0; k < nSmCountTemp; k++) 
    {
        for(int l = 0; l < nTgCountTemp; l++) 
        {
            for(int w=0; w<m_nWCount; ++w)
            {
                for(int x=0; x<2; ++x)
                {
                    double dWFactor = (x == 0) ? 1.0 : -1.0;

                    rData.Initialize();
                    rData.LoadCombName = _LSX(W);

                    LoadCaseNo = 0;
                    strFixSm.Format(_T("%d"), k+1);
                    strFixTg.Format(_T("%d"), l+1);            
                    strFixW.Format(_T("%d"), x+1);

                    int i=0, nDL=0, nPS=0, nCR=0, nSH=0, nSTL=0, nLL=0, nIL=0, nCRL=0, nTG=0, nTLL=0, nSL=0, nW=0;
                    for(i=0; i<m_nDCount; i++)   nDL += Set_STLCIncludeCS(arDLoad.GetAt(i),  dFactor, LoadCaseNo, rData, D_SGLD_DL_CVL);
                    for(i=0; i<m_nGDCount; i++)         Set_GridCase(arGridD.GetAt(i),  dFactor, LoadCaseNo, rData, TRUE);  
                    for(i=0; i<m_nGDeCount; i++)        Set_GridCase(arGridDe.GetAt(i), dFactor, LoadCaseNo, rData, TRUE);  
                    for(i=0; i<m_nPsCount; i++)  nPS += Set_STLCIncludeCS(arPsLoad.GetAt(i), dFactor, LoadCaseNo, rData, D_SGLD_TS_CVL);
                    for(i=0; i<m_nCrCount; i++)  nCR += Set_STLCIncludeCS(arCrLoad.GetAt(i), dFactor, LoadCaseNo, rData, D_SGLD_CS_CVL);
                    for(i=0; i<m_nShCount; i++)  nSH += Set_STLCIncludeCS(arShLoad.GetAt(i), dFactor, LoadCaseNo, rData, D_SGLD_SS_CVL);
                    for(i=0; i<m_nStlCount; i++) nSTL+= Set_STLCIncludeCS(arStlLoad.GetAt(i),dFactor, LoadCaseNo, rData, 0);
                    if(m_nSmCount > 0)                  Set_SettleCase(arSmLoad.GetAt(k), dFactor, LoadCaseNo, rData, TRUE);
                    if(m_nLCount > 0)
                    {
                        for(i=0; i<m_nLCount; i++)   nLL += Set_STLCIncludeCS(arLLoad.GetAt(i),  dFactor, LoadCaseNo, rData, 0);
                        for(i=0; i<m_nIlCount; i++)  nIL += Set_STLCIncludeCS(arIlLoad.GetAt(i), dFactor, LoadCaseNo, rData, 0);
                        for(i=0; i<m_nCrlCount; i++) nCRL+= Set_STLCIncludeCS(arCrlLoad.GetAt(i),dFactor, LoadCaseNo, rData, 0);              
                    }
                    for(i=0; i<m_nGLvCount; i++)  Set_GridCase(arGridLv.GetAt(i), dFactor, LoadCaseNo, rData, TRUE);
                    for(i=0; i<m_nGCrlCount; i++) Set_GridCase(arGridCrl.GetAt(i),dFactor, LoadCaseNo, rData, TRUE);
                    for(i=0; i<m_nGLeCount; i++)  Set_GridCase(arGridLe.GetAt(i), dFactor, LoadCaseNo, rData, TRUE);

                    if(nStrType == 2)
                    {
                        if(m_nTgCount > 0)          nTG += Set_STLCIncludeCS(arTgLoad.GetAt(l),  dTgFactor, LoadCaseNo, rData, 0);
                        for(i=0; i<m_nTlCount; i++) nTLL+= Set_STLCIncludeCS(arTlLoad.GetAt(i),  dTlFactor, LoadCaseNo, rData, 0);
                    }          
                    for(i=0; i<m_nSCount; i++) nSL += Set_STLCIncludeCS(arSLoad.GetAt(i),  dFactor, LoadCaseNo, rData, 0);

                    // Wind load
                    nW = Set_STLCIncludeCS(arWLoad.GetAt(w),  dWFactor, LoadCaseNo, rData, 0);

                    if(m_nConstLoad!=0)
                    {
                        if(CheckCS_SgldK(D_SGLD_DL_CVL, n)) Set_CStageCase(arCSLoad.GetAt(n), dFactor, LoadCaseNo, rData, TRUE);

                        CString strcEL;
                        int nErection = 0;

                        for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
                        {
                            strcEL = _T(""); nErection = 0;
                            if(CheckCS_SgldK(m_CSEL_SgldK[nErec], n)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

                            if(CheckCSEL(nErection, _T("D")) || CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")) || 
                                CheckCSEL(nErection, _T("L")))
                                Set_CStageCase(m_aCSLoad.GetAt(n), dFactor, LoadCaseNo, rData, TRUE);
                            else if(nStrType==2 && CheckCSEL(nErection, _T("T")))
                                Set_CStageCase(m_aCSLoad.GetAt(n), dTgFactor, LoadCaseNo, rData, TRUE);
                            else if(nStrType==3 && CheckCSEL(nErection, _T("TPG")))
                                Set_CStageCase(m_aCSLoad.GetAt(n), dTlFactor, LoadCaseNo, rData, TRUE);
                            else if(CheckCSEL(nErection, _T("W")))
                                Set_CStageCase(m_aCSLoad.GetAt(n), dWFactor, LoadCaseNo, rData, TRUE);
                        }

                        if(CheckCS_SgldK(D_SGLD_TP_CVL, n)) Set_CStageCase(arCSLoad.GetAt(n), dPST,    LoadCaseNo, rData, TRUE);
                        if(CheckCS_SgldK(D_SGLD_TS_CVL, n)) Set_CStageCase(arCSLoad.GetAt(n), dPSS,    LoadCaseNo, rData, TRUE);
                        if(CheckCS_SgldK(D_SGLD_CS_CVL, n)) Set_CStageCase(arCSLoad.GetAt(n), dFactor, LoadCaseNo, rData, TRUE);
                        if(CheckCS_SgldK(D_SGLD_SS_CVL, n)) Set_CStageCase(arCSLoad.GetAt(n), dFactor, LoadCaseNo, rData, TRUE);          
                    }

                    rData.Description = _T("");    
                    CString csPrefix = _T("");
                    if(nDL         > 0) {rData.Description += csPrefix + _T("D");   csPrefix = _T("+");}
                    if(m_nGDCount  > 0) {rData.Description += csPrefix + _T("GD");  csPrefix = _T("+");}
                    if(m_nGDeCount > 0) {rData.Description += csPrefix + _T("GDE"); csPrefix = _T("+");}
                    if(nPS         > 0) {rData.Description += csPrefix + _T("PS");  csPrefix = _T("+");}
                    if(nCR         > 0) {rData.Description += csPrefix + _T("CR");  csPrefix = _T("+");}
                    if(nSH         > 0) {rData.Description += csPrefix + _T("SH");  csPrefix = _T("+");}
                    if(nSTL        > 0) {rData.Description += csPrefix + _T("STL"); csPrefix = _T("+");}
                    if(m_nSmCount  > 0) {rData.Description += csPrefix + _T("SM[") + strFixSm + _T("]"); csPrefix = _T("+");}
                    if(m_nLCount > 0)
                    {
                        if(nLL       > 0) {rData.Description += csPrefix + _T("L");   csPrefix = _T("+");}
                        if(nIL       > 0) {rData.Description += csPrefix + _T("IL");  csPrefix = _T("+");}
                        if(nCRL      > 0) {rData.Description += csPrefix + _T("CRL"); csPrefix = _T("+");}
                    }
                    if(m_nGLvCount > 0) {rData.Description += csPrefix + _T("GLV"); csPrefix = _T("+");}
                    if(m_nGCrlCount> 0) {rData.Description += csPrefix + _T("GCRL");csPrefix = _T("+");}
                    if(m_nGLeCount > 0) {rData.Description += csPrefix + _T("GLE"); csPrefix = _T("+");}
                    if(nStrType == 2)
                    {
                        if(nTG > 0) {rData.Description += csPrefix + _T("TG[") + strFixTg + _T("]");   csPrefix = _T("+");}
                        if(nTLL > 0)  {rData.Description += csPrefix + _T("TL"); csPrefix = _T("+");}                
                    }
                    if(nSL > 0) {rData.Description += csPrefix + _T("S"); csPrefix = _T("+");}
                    if(nW  > 0) 
                    {
                        if(x == 0)
                        {
                            rData.Description += csPrefix + _T("W[") + strFixW + _T("]");   csPrefix = _T("+");
                        }
                        else
                        {
                            rData.Description += _T("-W[") + strFixW + _T("]");   csPrefix = _T("+");
                        }
                    }

                    if(m_nConstLoad!=0)
                    {
                        if(CheckCS_SgldK(D_SGLD_DL_CVL, n))
                        {rData.Description += csPrefix + _T("(cD)"); csPrefix = _T("+");}

                        CString strcEL;
                        int nErection = 0;

                        for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
                        {
                            strcEL = _T(""); nErection = 0;
                            if(CheckCS_SgldK(m_CSEL_SgldK[nErec], n)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

                            if(CheckCSEL(nErection, _T("D")) || CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")) ||
                                CheckCSEL(nErection, _T("L")))
                            {rData.Description += csPrefix + strcEL; csPrefix = _T("+");}
                            else if(nStrType==2 && CheckCSEL(nErection, _T("T")))
                            {rData.Description += csPrefix + GetFactorString(dTgFactor) + strcEL; csPrefix = _T("+");}
                            else if(nStrType==2 && CheckCSEL(nErection, _T("TPG")))
                            {rData.Description += csPrefix + GetFactorString(dTlFactor) + strcEL; csPrefix = _T("+");}
                            else if(CheckCSEL(nErection, _T("W")))
                            {rData.Description += csPrefix + GetFactorString(dWFactor) + strcEL; csPrefix = _T("+");}
                        }
                        if(CheckCS_SgldK(D_SGLD_TP_CVL, n))
                        {rData.Description += csPrefix + GetFactorString(dPST) + _T("(cTP)"); csPrefix = _T("+");}
                        if(CheckCS_SgldK(D_SGLD_TS_CVL, n))
                        {rData.Description += csPrefix + GetFactorString(dPSS) + _T("(cTS)"); csPrefix = _T("+");}
                        if(CheckCS_SgldK(D_SGLD_CS_CVL, n))
                        {rData.Description += csPrefix + _T("(cCR)"); csPrefix = _T("+");}
                        if(CheckCS_SgldK(D_SGLD_SS_CVL, n))
                        {rData.Description += csPrefix + _T("(cSH)"); csPrefix = _T("+");}
                    }

                    if(LoadCaseNo > 0) 
                    {
                        if(m_nLcomType == D_LCOMTYPE_CONCRETE) rData.nActive = 2;  // Serviceability.  
                        if(!bActive) rData.nActive = 0;   // Inactive
                        Set_LoadComb4CivilName(rData, D_SER);
                    }
                }
            }
        }
    }
}

void CLoadCombCtrl::Set_CvlJSCE_No3_1_2_M(CString strCode, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, 
                                          ArINT& arStlLoad, ArUNT& arSmLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, 
                                          ArINT& arTgLoad, ArINT& arTlLoad, ArINT& arSLoad, ArINT& arCSLoad, ArINT& arWLoad, 
                                          ArINT& arGridD, ArINT& arGridDe, ArINT& arGridLv, ArINT& arGridCrl, ArINT& arGridLe, 
                                          int nStrType, BOOL bActive)                                      
{
    if(m_nMCount==0 || (m_nConstLoad==0 && m_nDCount==0 && m_nGDCount==0 && m_nGDeCount==0)) return;    
    if(m_nWCount==0) return;  // Wind Load는 반드시 있어야 함
    if(nStrType == 2)  // Temperature + Wind
    {
        if(m_nTgCount == 0 && m_nTlCount == 0) return;   // 온도하중은 반드시 있어야 함
    }  

    int LoadCaseNo = 0, n=0;
    T_LCOM_D rData;
    double dFactor=1.0, dTgFactor=1.0, dTlFactor=1.0; 
    double dPST = (m_bLossFactor ? m_dLossFactorTransfer : 1.0);
    double dPSS = (m_bLossFactor ? m_dLossFactorService  : 1.0);
    // DL는 항상포함, IL은 LL가 있는 경우만 포함
    int  nSmCountTemp, nTgCountTemp;   // load의 개수가 0 인 경우 아래 루틴을 사용하기 위해 임시로 설정한 load의 개수
    nSmCountTemp  = max(m_nSmCount,  1);  
    nTgCountTemp  = max(m_nTgCount,   1);  
    CString strFixM, strFixSm, strFixTg, strFixW;

    BOOL bStageCS=FALSE;
    if(m_pDoc->IsPostMode()) bStageCS = m_pDoc->m_pPostCtrl->StageAnalysis();
    else										 bStageCS = m_bCSAnalysis;

    for(int j = 0; j < m_nMCount; j++) // moving
    {
        for(int k = 0; k < nSmCountTemp; k++) // settlement
        {
            for(int l = 0; l < nTgCountTemp; l++) // 온도변화
            {
                for(int w=0; w<m_nWCount; ++w)
                {
                    for(int x=0; x<2; ++x)
                    {
                        double dWFactor = (x == 0) ? 1.0 : -1.0;

                        rData.Initialize();
                        rData.LoadCombName = _LSX(W);

                        LoadCaseNo = 0;
                        strFixM.Format(_T("%d"), j+1);
                        strFixSm.Format(_T("%d"), k+1);
                        strFixTg.Format(_T("%d"), l+1);
                        strFixW.Format(_T("%d"), x+1);

                        int i=0, nDL=0, nLL=0, nIL=0, nPS=0, nCR=0, nSH=0, nSTL=0, nCRL=0, nTG=0, nTL=0, nSL=0, nW=0;
                        for(i=0; i<m_nDCount; i++)   nDL += Set_STLCIncludeCS(arDLoad.GetAt(i), dFactor, LoadCaseNo, rData, D_SGLD_DL_CVL);
                        for(i=0; i<m_nGDCount; i++)  Set_GridCase(arGridD.GetAt(i),  dFactor, LoadCaseNo, rData, TRUE);  
                        for(i=0; i<m_nGDeCount; i++) Set_GridCase(arGridDe.GetAt(i), dFactor, LoadCaseNo, rData, TRUE);  
                        for(i=0; i<m_nPsCount; i++)  nPS += Set_STLCIncludeCS(arPsLoad.GetAt(i), dFactor, LoadCaseNo, rData, D_SGLD_TS_CVL);          
                        for(i=0; i<m_nCrCount; i++)  nCR += Set_STLCIncludeCS(arCrLoad.GetAt(i), dFactor, LoadCaseNo, rData, D_SGLD_CS_CVL);
                        for(i=0; i<m_nShCount; i++)  nSH += Set_STLCIncludeCS(arShLoad.GetAt(i), dFactor, LoadCaseNo, rData, D_SGLD_SS_CVL);
                        for(i=0; i<m_nStlCount; i++) nSTL+= Set_STLCIncludeCS(arStlLoad.GetAt(i),dFactor, LoadCaseNo, rData, 0);
                        if(m_nSmCount > 0)           Set_SettleCase(arSmLoad.GetAt(k), dFactor, LoadCaseNo, rData, TRUE);                                          
                        Set_MovingCase(arMLoad.GetAt(j), dFactor, LoadCaseNo, rData, TRUE);                
                        if(m_nLCount > 0)
                        {
                            for(i=0; i<m_nLCount; i++)   nLL += Set_STLCIncludeCS(arLLoad.GetAt(i),  dFactor, LoadCaseNo, rData, 0);
                            for(i=0; i<m_nIlCount; i++)  nIL += Set_STLCIncludeCS(arIlLoad.GetAt(i), dFactor, LoadCaseNo, rData, 0);
                        }
                        for(i=0; i<m_nCrlCount; i++)  nCRL+= Set_STLCIncludeCS(arCrlLoad.GetAt(i),dFactor, LoadCaseNo, rData, 0);
                        for(i=0; i<m_nGCrlCount; i++) Set_GridCase(arGridCrl.GetAt(i), dFactor, LoadCaseNo, rData, TRUE);
                        if(nStrType == 2)
                        {
                            if(m_nTgCount > 0)           nTG += Set_STLCIncludeCS(arTgLoad.GetAt(l),  dTgFactor, LoadCaseNo, rData, 0);
                            for(i=0; i<m_nTlCount; i++)  nTL += Set_STLCIncludeCS(arTlLoad.GetAt(i),  dTlFactor, LoadCaseNo, rData, 0);
                        }
                        for(i=0; i<m_nSCount; i++)     nSL += Set_STLCIncludeCS(arSLoad.GetAt(i),  dFactor, LoadCaseNo, rData, 0);

                        // Wind load
                        nW = Set_STLCIncludeCS(arWLoad.GetAt(w),  dWFactor, LoadCaseNo, rData, 0);

                        if(m_nConstLoad!=0)
                        {
                            if(CheckCS_SgldK(D_SGLD_DL_CVL, n)) Set_CStageCase(arCSLoad.GetAt(n), dFactor, LoadCaseNo, rData, TRUE);

                            CString strcEL;
                            int nErection = 0;

                            for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
                            {
                                strcEL = _T(""); nErection = 0;
                                if(CheckCS_SgldK(m_CSEL_SgldK[nErec], n)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

                                if(CheckCSEL(nErection, _T("D")) || CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")) || CheckCSEL(nErection, _T("L")))
                                    Set_CStageCase(m_aCSLoad.GetAt(n), dFactor, LoadCaseNo, rData, TRUE);
                                else if(nStrType==2 && CheckCSEL(nErection, _T("T")))
                                    Set_CStageCase(m_aCSLoad.GetAt(n), dTgFactor, LoadCaseNo, rData, TRUE);
                                else if(nStrType==2 && CheckCSEL(nErection, _T("TPG")))
                                    Set_CStageCase(m_aCSLoad.GetAt(n), dTlFactor, LoadCaseNo, rData, TRUE);
                                else if(CheckCSEL(nErection, _T("W")))
                                    Set_CStageCase(m_aCSLoad.GetAt(n), dWFactor, LoadCaseNo, rData, TRUE);
                            }
                            if(CheckCS_SgldK(D_SGLD_TP_CVL, n)) Set_CStageCase(arCSLoad.GetAt(n), dPST,    LoadCaseNo, rData, TRUE);
                            if(CheckCS_SgldK(D_SGLD_TS_CVL, n)) Set_CStageCase(arCSLoad.GetAt(n), dPSS,    LoadCaseNo, rData, TRUE);
                            if(CheckCS_SgldK(D_SGLD_CS_CVL, n)) Set_CStageCase(arCSLoad.GetAt(n), dFactor, LoadCaseNo, rData, TRUE);
                            if(CheckCS_SgldK(D_SGLD_SS_CVL, n)) Set_CStageCase(arCSLoad.GetAt(n), dFactor, LoadCaseNo, rData, TRUE);          
                        }

                        rData.Description = _T("");
                        CString csPrefix = _T("");
                        if(nDL         > 0) {rData.Description += csPrefix + _T("D");   csPrefix = _T("+");}
                        if(m_nGDCount  > 0) {rData.Description += csPrefix + _T("GD");  csPrefix = _T("+");}
                        if(m_nGDeCount > 0) {rData.Description += csPrefix + _T("GDE"); csPrefix = _T("+");}
                        if(nPS         > 0) {rData.Description += csPrefix + _T("PS");  csPrefix = _T("+");}
                        if(nCR         > 0) {rData.Description += csPrefix + _T("CR");  csPrefix = _T("+");}
                        if(nSH         > 0) {rData.Description += csPrefix + _T("SH");  csPrefix = _T("+");}
                        if(nSTL        > 0) {rData.Description += csPrefix + _T("STL"); csPrefix = _T("+");}
                        if(m_nSmCount  > 0) {rData.Description += csPrefix + _T("SM[") + strFixSm + _T("]"); csPrefix = _T("+");}
                        {rData.Description += csPrefix + "M[" + strFixM + _T("]");  csPrefix = _T("+");}     
                        if(nLL         > 0) {rData.Description += csPrefix + _T("L");   csPrefix = _T("+");}
                        if(nIL         > 0) {rData.Description += csPrefix + _T("IL");  csPrefix = _T("+");}
                        if(nCRL        > 0) {rData.Description += csPrefix + _T("CRL");  csPrefix = _T("+");}
                        if(m_nGCrlCount> 0) {rData.Description += csPrefix + _T("GCRL"); csPrefix = _T("+");}
                        if(nStrType == 2)
                        { 
                            if(nTG > 0) {rData.Description += csPrefix + _T("TG[") + strFixTg + _T("]");   csPrefix = _T("+");}
                            if(nTL   > 0) {rData.Description += csPrefix + _T("TL"); csPrefix = _T("+");}                
                        }
                        if(nSL > 0) {rData.Description += csPrefix + _T("S"); csPrefix = _T("+");}
                        if(nW  > 0) 
                        {
                            if(x == 0)
                            {
                                rData.Description += csPrefix + _T("W[") + strFixW + _T("]");   csPrefix = _T("+");
                            }
                            else
                            {
                                rData.Description += _T("-W[") + strFixW + _T("]");   csPrefix = _T("+");
                            }
                        }
                        if(m_nConstLoad!=0)
                        {
                            if(CheckCS_SgldK(D_SGLD_DL_CVL, n))
                            {rData.Description += csPrefix + _T("(cD)"); csPrefix = _T("+");}

                            CString strcEL;
                            int nErection = 0;

                            for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
                            {
                                strcEL = _T(""); nErection = 0;
                                if(CheckCS_SgldK(m_CSEL_SgldK[nErec], n)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

                                if(CheckCSEL(nErection, _T("D")) || CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")) || CheckCSEL(nErection, _T("L")))
                                {rData.Description += csPrefix + strcEL; csPrefix = _T("+");}
                                else if(nStrType==3 && CheckCSEL(nErection, _T("T")))
                                {rData.Description += csPrefix + GetFactorString(dTgFactor) + strcEL; csPrefix = _T("+");}
                                else if(nStrType==3 && CheckCSEL(nErection, _T("TPG")))
                                {rData.Description += csPrefix + GetFactorString(dTlFactor) + strcEL; csPrefix = _T("+");}
                                else if(CheckCSEL(nErection, _T("W")))
                                {rData.Description += csPrefix + GetFactorString(dWFactor) + strcEL; csPrefix = _T("+");}
                            }
                            if(CheckCS_SgldK(D_SGLD_TP_CVL, n))
                            {rData.Description += csPrefix + GetFactorString(dPST) + _T("(cTP)"); csPrefix = _T("+");}
                            if(CheckCS_SgldK(D_SGLD_TS_CVL, n))
                            {rData.Description += csPrefix + GetFactorString(dPSS) + _T("(cTS)"); csPrefix = _T("+");}
                            if(CheckCS_SgldK(D_SGLD_CS_CVL, n))
                            {rData.Description += csPrefix + _T("(cCR)"); csPrefix = _T("+");}
                            if(CheckCS_SgldK(D_SGLD_SS_CVL, n))
                            {rData.Description += csPrefix + _T("(cSH)"); csPrefix = _T("+");}
                        }

                        if(LoadCaseNo > 0) 
                        {
                            if(m_nLcomType == D_LCOMTYPE_CONCRETE) rData.nActive = 2;  // Serviceability.  
                            if(!bActive) rData.nActive = 0;   // Inactive
                            Set_LoadComb4CivilName(rData, D_SER);
                        }
                    }
                }
            }
        }
    }
}

// Wind load Only
void CLoadCombCtrl::Set_CvlJSCE_No3_3(CString strCode, ArINT& arWLoad, ArINT& arCSLoad, BOOL bActive)
{  
    if(m_nWCount==0) return;    
    T_LCOM_D rData;  
    for(int j = 0; j < m_nWCount; ++j)
    {
        CString strFixW = _T("");
        strFixW.Format(_T("%d"), j+1);

        for(int l = 0; l < 2; l++)
        {       
            rData.Initialize();
            double dWFactor = (l == 0) ? 1.0 : -1.0;
            rData.Description  = _T("");
            rData.LoadCombName = _LSX(W);

            CString csPrefix = _T("");
            int nLoadCaseNum = 0;
            if(Set_STLCIncludeCS(arWLoad.GetAt(j),  dWFactor, nLoadCaseNum, rData, 0) > 0)
            {
                if(l == 0)  // + sign
                {
                    rData.Description += _T("W[") + strFixW + _T("]");
                }
                else   // - sign
                {
                    rData.Description += _T("-W[") + strFixW + _T("]");
                }
                csPrefix = _T("+");
            }

            if(m_nConstLoad!=0)
            {   
                for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
                {
                    int n=0, nErection = 0;
                    CString strcW;
                    if(CheckCS_SgldK(m_CSEL_SgldK[nErec], n)) { strcW = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

                    if(CheckCSEL(nErection, _T("W")))
                    {
                        Set_CStageCase(m_aCSLoad.GetAt(n), dWFactor, nLoadCaseNum, rData, TRUE);
                        rData.Description += csPrefix + GetFactorString(dWFactor) + strcW;
                        csPrefix = _T("+");
                    }
                }
            }

            if(nLoadCaseNum > 0) 
            {
                if(m_nLcomType == D_LCOMTYPE_CONCRETE) rData.nActive = 2;  // Serviceability.  
                if(!bActive) rData.nActive = 0;   // Inactive
                Set_LoadComb4CivilName(rData, D_SER);
            }
        }
    }
}




////////////////////////////////////////////////////////////////////////
////// JSCE12
////////////////////////////////////////////////////////////////////////
BOOL CLoadCombCtrl::Get_JSCE17_Factor(const EN_JSCE17_NO& enLcomNo, const UINT& nNum, double* raFactor, CString* rastrFactor)
{
    if(nNum != EN_JSCE17_NUM) { ASSERT(0); return FALSE; }

    double dGamma_p[EN_JSCE17_NO_12][EN_JSCE17_NUM] = 
    {
    //                                                         *           *     *                 *     *                                   *
    //   D     (L    IL    CRL)  (PS   CR    SH )  (EP   WP    B  )  TG    TL    S     (STL  SM )  CF    BRK   W     WL    WPR   E     ESP   CO    ER
        {1.00, 0.00, 0.00, 0.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 0.00, 1.00, 0.00, 1.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 1.00},
        {1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 0.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 1.00},
        {1.00, 0.00, 0.00, 0.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 0.00, 1.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 1.00},
        {1.00, 0.00, 0.00, 0.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 0.75, 1.00, 0.00, 1.00, 1.00, 0.00, 0.00, 0.75, 0.00, 1.00, 0.00, 0.00, 0.00, 1.00},
        {1.00, 0.95, 0.95, 0.95, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 0.75, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 1.00},
        {1.00, 0.95, 0.95, 0.95, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 0.00, 1.00, 0.00, 1.00, 1.00, 1.00, 1.00, 0.50, 0.50, 1.00, 0.00, 0.00, 0.00, 1.00},
        {1.00, 0.95, 0.95, 0.95, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 0.50, 1.00, 0.00, 1.00, 1.00, 1.00, 1.00, 0.50, 0.50, 1.00, 0.00, 0.00, 0.00, 1.00},
        {1.00, 0.00, 0.00, 0.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 0.00, 1.00, 0.00, 1.00, 1.00, 0.00, 0.00, 1.00, 0.00, 1.00, 0.00, 0.00, 0.00, 1.00},
        {1.00, 0.00, 0.00, 0.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 0.50, 1.00, 1.00, 1.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.50, 0.50, 0.00, 1.00},
        {1.00, 0.00, 0.00, 0.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 0.00, 1.00, 0.00, 1.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 1.00, 1.00, 0.00, 1.00},
        {1.00, 0.00, 0.00, 0.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 0.00, 0.00, 0.00, 1.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 1.00, 0.00, 0.00},
        {1.00, 0.00, 0.00, 0.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 0.00, 0.00, 0.00, 1.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00}
    };

    double dGamma_q[EN_JSCE17_NO_12][EN_JSCE17_NUM] = 
    {
    //                                                         *           *     *                 *     *                                   *
    //   D     (L    IL    CRL)  (PS   CR    SH )  (EP   WP    B  )  TG    TL    S     (STL  SM )  CF    BRK   W     WL    WPR   E     ESP   CO    ER
        {1.05, 0.00, 0.00, 0.00, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 0.00, 1.00, 0.00, 1.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 1.05},
        {1.05, 1.25, 1.25, 1.25, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 0.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 1.05},
        {1.05, 0.00, 0.00, 0.00, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 1.00, 1.00, 0.00, 1.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 1.05},
        {1.05, 0.00, 0.00, 0.00, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 1.00, 1.00, 0.00, 1.00, 1.00, 0.00, 0.00, 1.25, 0.00, 1.00, 0.00, 0.00, 0.00, 1.05},
        {1.05, 1.25, 1.25, 1.25, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 1.05},
        {1.05, 1.25, 1.25, 1.25, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 0.00, 1.00, 0.00, 1.00, 1.00, 1.00, 1.00, 1.25, 1.25, 1.00, 0.00, 0.00, 0.00, 1.05},
        {1.05, 1.25, 1.25, 1.25, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 1.00, 1.00, 0.00, 1.00, 1.00, 1.00, 1.00, 1.25, 1.25, 1.00, 0.00, 0.00, 0.00, 1.05},
        {1.05, 0.00, 0.00, 0.00, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 0.00, 1.00, 0.00, 1.00, 1.00, 0.00, 0.00, 1.25, 0.00, 1.00, 0.00, 0.00, 0.00, 1.05},
        {1.05, 0.00, 0.00, 0.00, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 1.00, 1.00, 1.00, 1.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 1.00, 1.00, 0.00, 1.05},
        {1.05, 0.00, 0.00, 0.00, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 0.00, 1.00, 0.00, 1.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 1.00, 1.00, 0.00, 1.05},
        {1.05, 0.00, 0.00, 0.00, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 0.00, 0.00, 0.00, 1.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 1.00, 0.00, 0.00},
        {1.05, 0.00, 0.00, 0.00, 1.05, 1.05, 1.05, 1.05, 1.05, 1.05, 0.00, 0.00, 0.00, 1.00, 1.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00}
    };

    int iIdx = enLcomNo-1;
    for(int i=0; i<nNum; ++i)
    {
        raFactor[i] = dGamma_p[iIdx][i] * dGamma_q[iIdx][i];
        if(raFactor[i] > m_dZeroLim)
            //rastrFactor[i].Format(_T("%.2fx%.2f"), dGamma_p[iIdx][i], dGamma_q[iIdx][i]);
            rastrFactor[i] = GetFactorString(dGamma_p[iIdx][i] * dGamma_q[iIdx][i]);
        else
            rastrFactor[i] = _T("");
    }
    return TRUE;
}

int CLoadCombCtrl::Get_JSCE17_LCOM_List(CArray<JSCE17_LCOM, JSCE17_LCOM&>& raLcomList)
{
    raLcomList.RemoveAll();

    JSCE17_LCOM Jsce17D;
    //          No  Moving Thermal ESP
    // 지속작용 지배상황
    Jsce17D.Set(EN_JSCE17_NO_01, FALSE, FALSE,  FALSE);    raLcomList.Add(Jsce17D);// ① D
    // 변동작용 지배상황
    Jsce17D.Set(EN_JSCE17_NO_02, FALSE, FALSE,  FALSE);    raLcomList.Add(Jsce17D);// ② D+L
    Jsce17D.Set(EN_JSCE17_NO_02, TRUE , FALSE,  FALSE);    raLcomList.Add(Jsce17D);// ② D+M+L
    Jsce17D.Set(EN_JSCE17_NO_03, FALSE, TRUE ,  FALSE);    raLcomList.Add(Jsce17D);// ③ D+TH
    Jsce17D.Set(EN_JSCE17_NO_04, FALSE, FALSE,  FALSE);    raLcomList.Add(Jsce17D);// ④ D+TH+WS
    Jsce17D.Set(EN_JSCE17_NO_05, FALSE, FALSE,  FALSE);    raLcomList.Add(Jsce17D);// ⑤ D+L+TH
    Jsce17D.Set(EN_JSCE17_NO_05, TRUE , FALSE,  FALSE);    raLcomList.Add(Jsce17D);// ⑤ D+M+L+TH
    Jsce17D.Set(EN_JSCE17_NO_06, FALSE, FALSE,  FALSE);    raLcomList.Add(Jsce17D);// ⑥ D+L+WS+WL
    Jsce17D.Set(EN_JSCE17_NO_06, TRUE , FALSE,  FALSE);    raLcomList.Add(Jsce17D);// ⑥ D+M+L+WS+WL
    Jsce17D.Set(EN_JSCE17_NO_07, FALSE, FALSE,  FALSE);    raLcomList.Add(Jsce17D);// ⑦ D+L+TH+WS+WL
    Jsce17D.Set(EN_JSCE17_NO_07, TRUE , FALSE,  FALSE);    raLcomList.Add(Jsce17D);// ⑦ D+M+TH+WS+WL
    Jsce17D.Set(EN_JSCE17_NO_08, FALSE, FALSE,  FALSE);    raLcomList.Add(Jsce17D);// ⑧ D+WS
    Jsce17D.Set(EN_JSCE17_NO_09, FALSE, FALSE,  FALSE);    raLcomList.Add(Jsce17D);// ⑨ D+TH+EQ
    Jsce17D.Set(EN_JSCE17_NO_09, FALSE, FALSE,  TRUE );    raLcomList.Add(Jsce17D);// ⑨ D+TH+ESP
    Jsce17D.Set(EN_JSCE17_NO_10, FALSE, FALSE,  FALSE);    raLcomList.Add(Jsce17D);// ⑩ D+EQ
    Jsce17D.Set(EN_JSCE17_NO_10, FALSE, FALSE,  TRUE );    raLcomList.Add(Jsce17D);// ⑩ D+ESP
    // 우발작용 지배상황
    Jsce17D.Set(EN_JSCE17_NO_11, FALSE, FALSE,  FALSE);    raLcomList.Add(Jsce17D);// ⑪ D+EQ
    Jsce17D.Set(EN_JSCE17_NO_11, FALSE, FALSE,  TRUE );    raLcomList.Add(Jsce17D);// ⑪ D+ESP
    Jsce17D.Set(EN_JSCE17_NO_12, FALSE, FALSE,  FALSE);    raLcomList.Add(Jsce17D);// ⑫ D+CO

    return raLcomList.GetSize();
}

BOOL CLoadCombCtrl::Check_JSCE17_LoadCase(const EN_JSCE17_NO& enLcomNo, BOOL bEsp)
{
    // DL는 항상포함
    if((m_nConstLoad==0 && m_nDCount==0 && m_nGDCount==0 && m_nGDeCount==0)) return FALSE;

    // Load check
    switch (enLcomNo)
    {
    case EN_JSCE17_NO_01: // D
        break;
    case EN_JSCE17_NO_02: // D+L
        if(m_nLCount == 0 && m_nGLvCount == 0 && m_nGCrlCount == 0 && m_nGLeCount == 0) return FALSE; // Live Load
        break;
    case EN_JSCE17_NO_03: // D+TH
        if(m_nTgCount == 0) return FALSE;   // Thermal Global
        break;
    case EN_JSCE17_NO_04: // D+TH+WS
        if(m_nTgCount == 0) return FALSE;  // Thermal Global
        if(m_nWCount  == 0) return FALSE;   // WS - Wind Load
        break;
    case EN_JSCE17_NO_05: // D+L+TH
        if(m_nLCount  == 0 && m_nGLvCount == 0 && m_nGCrlCount == 0 && m_nGLeCount == 0) return FALSE; // Live Load
        if(m_nTgCount == 0) return FALSE;   // Thermal Global
        break;
    case EN_JSCE17_NO_06: // D+L+WS+WL
        if(m_nLCount  == 0 && m_nGLvCount == 0 && m_nGCrlCount == 0 && m_nGLeCount == 0) return FALSE; // Live Load
        if(m_nWCount  == 0) return FALSE;   // WS - Wind Load
        if(m_nWlCount == 0) return FALSE;   // WL - Wind Load
        break;
    case EN_JSCE17_NO_07: // D+L+TH+WS+WL
        if(m_nLCount  == 0 && m_nGLvCount == 0 && m_nGCrlCount == 0 && m_nGLeCount == 0) return FALSE; // Live Load
        if(m_nTgCount == 0) return FALSE;   // Thermal Global
        if(m_nWCount  == 0) return FALSE;   // WS - Wind Load
        if(m_nWlCount == 0) return FALSE;   // WL - Wind Load
        break;
    case EN_JSCE17_NO_08: // D+WS
        if(m_nWCount == 0) return FALSE;   // WS - Wind Load
        break;
    case EN_JSCE17_NO_09: // D+TH+EQ
        if(m_nTgCount == 0) return FALSE;  // Thermal Global
        if(bEsp)
        {
            if(m_nEspCount == 0) return FALSE;  // Response spectrum
        }
        else
        {
            if(m_nECount == 0) return FALSE;  // Earthquake
        }
        break;
    case EN_JSCE17_NO_10: // D+EQ
    case EN_JSCE17_NO_11: // D+EQ
        if(bEsp)
        {
            if(m_nEspCount == 0) return FALSE;  // Response spectrum
        }
        else
        {
            if(m_nECount == 0) return FALSE;  // Earthquake
        }
        break;
    case EN_JSCE17_NO_12: // D+CO
        if(m_nCoCount == 0) return FALSE;  // Thermal Global
        break;
    default:
        ASSERT(0);
        return FALSE;
        break;
    }

    return TRUE;
}

int CLoadCombCtrl::Get_JSCE17_LoadCount(const EN_JSCE17& enLcase)
{
    switch (enLcase)
    {
    case EN_JSCE17_D  :        return m_nDCount  ;
    case EN_JSCE17_L  :        return m_nLCount  ;
    case EN_JSCE17_IL :        return m_nIlCount ;
    case EN_JSCE17_CRL:        return m_nCrlCount;
    case EN_JSCE17_PS :        return m_nPsCount ;
    case EN_JSCE17_CR :        return m_nCrCount ;
    case EN_JSCE17_SH :        return m_nShCount ;
    case EN_JSCE17_EP :        return m_nEpCount ;
    case EN_JSCE17_WP :        return m_nWpCount ;
    case EN_JSCE17_B  :        return m_nBCount  ;
    case EN_JSCE17_TG :        return m_nTgCount ;
    case EN_JSCE17_TL :        return m_nTlCount ;
    case EN_JSCE17_S  :        return m_nSCount  ;
    case EN_JSCE17_STL:        return m_nStlCount;
    case EN_JSCE17_SM :        return m_nSmCount ;
    case EN_JSCE17_CF :        return m_nCfCount ;
    case EN_JSCE17_BRK:        return m_nBrkCount;
    case EN_JSCE17_W  :        return m_nWCount  ;
    case EN_JSCE17_WL :        return m_nWlCount ;
    case EN_JSCE17_WPR:        return m_nWprCount;
    case EN_JSCE17_E  :        return m_nECount  ;
    case EN_JSCE17_ESP:        return m_nEspCount;
    case EN_JSCE17_CO :        return m_nCoCount ;
    case EN_JSCE17_ER :        return m_nErCount ;
    default:
        ASSERT(0);
        break;
    }
    return 0;
}


CString CLoadCombCtrl::Get_JSCE17_LoadChar(const EN_JSCE17& enLcase)
{
    switch (enLcase)
    {
    case EN_JSCE17_D  :        return _LSX(D)  ;
    case EN_JSCE17_L  :        return _LSX(L)  ;
    case EN_JSCE17_IL :        return _LSX(IL) ;
    case EN_JSCE17_CRL:        return _LSX(CRL);
    case EN_JSCE17_PS :        return _LSX(PS) ;
    case EN_JSCE17_CR :        return _LSX(CR) ;
    case EN_JSCE17_SH :        return _LSX(SH) ;
    case EN_JSCE17_EP :        return _LSX(EP) ;
    case EN_JSCE17_WP :        return _LSX(WP) ;
    case EN_JSCE17_B  :        return _LSX(B)  ;
    case EN_JSCE17_TG :        return _LSX(TG) ;
    case EN_JSCE17_TL :        return _LSX(TL) ;
    case EN_JSCE17_S  :        return _LSX(S)  ;
    case EN_JSCE17_STL:        return _LSX(STL);
    case EN_JSCE17_SM :        return _LSX(SM) ;
    case EN_JSCE17_CF :        return _LSX(CF) ;
    case EN_JSCE17_BRK:        return _LSX(BRK);
    case EN_JSCE17_W  :        return _LSX(W)  ;
    case EN_JSCE17_WL :        return _LSX(WL) ;
    case EN_JSCE17_WPR:        return _LSX(WPR);
    case EN_JSCE17_E  :        return _LSX(E)  ;
    case EN_JSCE17_ESP:        return _LSX(ESP);
    case EN_JSCE17_CO :        return _LSX(CO) ;
    case EN_JSCE17_ER :        return _LSX(ER) ;
    default:
        ASSERT(0);
        break;
    }
    return _T("");
}

void CLoadCombCtrl::Set_CvlJSCE17(const EN_JSCE17_NO& enLcomNo, ArINT& arDLoad  , ArINT& arLLoad  , ArINT& arIlLoad , ArINT& arCrlLoad, ArUNT& arMLoad , 
                                  ArINT& arPsLoad , ArINT& arCrLoad , ArINT& arShLoad ,
                                  ArINT& arEpLoad , ArINT& arWpLoad , ArINT& arBLoad  , 
                                  ArINT& arTgLoad , ArINT& arTlLoad , ArINT& arSLoad  ,
                                  ArINT& arStlLoad, ArUNT& arSmLoad , ArINT& arCfLoad , ArINT& arBrkLoad, 
                                  ArINT& arWLoad  , ArINT& arWlLoad , ArINT& arWprLoad, 
                                  ArINT& arELoad  , ArUNT& arEspLoad, ArINT& arCoLoad , ArINT& arErLoad , 
                                  ArINT& arGridD  , ArINT& arGridDe , ArINT& arGridLv , ArINT& arGridCrl, ArINT& arGridLe,
                                  ArINT& arCSLoad , 
                                  BOOL bMoving, BOOL bThermal, BOOL bEsp)
{
    if(!Check_JSCE17_LoadCase(enLcomNo, bEsp)) return;
    
    // Make new load combinations.
    CArray<T_LCOM_D, T_LCOM_D&> aNewLcom;
    aNewLcom.RemoveAll();
    Add_CvlJSCE17_D       (enLcomNo, arDLoad, arGridD, arGridDe, aNewLcom);
    if(bMoving)
        Add_CvlJSCE17_M   (enLcomNo, arMLoad  , aNewLcom);
    Add_CvlJSCE17_L       (enLcomNo, arLLoad  , arIlLoad, arCrlLoad, arGridLv, arGridCrl, arGridLe, aNewLcom);
    Add_CvlJSCE17_PS_CR_SH(enLcomNo, arPsLoad , arCrLoad, arShLoad, aNewLcom);
    Add_CvlJSCE17_EP_WP_B (enLcomNo, arEpLoad , arWpLoad, arBLoad , aNewLcom);
    Add_CvlJSCE17_AddBiDir(enLcomNo, EN_JSCE17_TG, arTgLoad , aNewLcom, bThermal); // Add Temperature(Global)
    Add_CvlJSCE17_AddOne  (enLcomNo, EN_JSCE17_TL, arTlLoad, aNewLcom, TRUE);// Add Temperature(Local)
    Add_CvlJSCE17_AddOne  (enLcomNo, EN_JSCE17_S , arSLoad , aNewLcom, TRUE);// Add Snow Load
    Add_CvlJSCE17_STL_SM  (enLcomNo, arStlLoad, arSmLoad , aNewLcom);
    Add_CvlJSCE17_CF_BRK  (enLcomNo, arCfLoad , arBrkLoad, aNewLcom);
    Add_CvlJSCE17_AddBiDir(enLcomNo, EN_JSCE17_W  , arWLoad  , aNewLcom);// Add Wind Load
    Add_CvlJSCE17_AddBiDir(enLcomNo, EN_JSCE17_WL , arWlLoad , aNewLcom);// Add Wind Load (live)
    Add_CvlJSCE17_AddOne  (enLcomNo, EN_JSCE17_WPR, arWprLoad, aNewLcom);// Add wave load 파동압
    if(bEsp) 
        Add_CvlJSCE17_AddBiDir(enLcomNo, EN_JSCE17_ESP, arEspLoad, aNewLcom); // Add Earthquake Static
    else
        Add_CvlJSCE17_AddBiDir(enLcomNo, EN_JSCE17_E  , arELoad  , aNewLcom); // Add Earthquake Response spectrum
    Add_CvlJSCE17_AddOne  (enLcomNo, EN_JSCE17_CO, arCoLoad, aNewLcom);// Add 충돌하중
    Add_CvlJSCE17_AddOne  (enLcomNo, EN_JSCE17_ER, arErLoad, aNewLcom, TRUE);// Add Erection load
    Add_CvlJSCE17_CS      (enLcomNo, arCSLoad , aNewLcom);;

    // Add new load combinations.
    int nNewLcom = aNewLcom.GetSize();
    for(int i=0; i<nNewLcom; ++i)
    {
        if(aNewLcom[i].aCombination.GetSize() == 0) continue;

        if(m_nLcomType == D_LCOMTYPE_CONCRETE) aNewLcom[i].nActive = 1;
        if(enLcomNo > EN_JSCE17_NO_10)
        {
            aNewLcom[i].LoadCombName = _LSX(ACC);
            Set_LoadComb4CivilNameJSCE17(aNewLcom[i], D_ACC);
        }
        else if(enLcomNo > EN_JSCE17_NO_01)
        {
            aNewLcom[i].LoadCombName = _LSX(VAR);
            Set_LoadComb4CivilNameJSCE17(aNewLcom[i], D_VAR);
         }
        else
        {
            aNewLcom[i].LoadCombName = _LSX(PER);
            Set_LoadComb4CivilNameJSCE17(aNewLcom[i], D_PER);
        }
    }
}

// Add Dead Load
void CLoadCombCtrl::Add_CvlJSCE17_D(const EN_JSCE17_NO& enLcomNo, ArINT& arDLoad, ArINT& arGridD, ArINT& arGridDe, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    double  adFactor[EN_JSCE17_NUM];
    CString asFactor[EN_JSCE17_NUM];
    if(!Get_JSCE17_Factor(enLcomNo, EN_JSCE17_NUM, adFactor, asFactor)) { ASSERT(0); return; }

    if(fabs(adFactor[EN_JSCE17_D]) < m_dZeroLim) return;
    if(m_nConstLoad==0 && m_nDCount==0 && m_nGDCount==0 && m_nGDeCount==0) return;

    int nLcom = raNewLcom.GetSize();
    if(nLcom==0)
    {
        T_LCOM_D NewLcomD;
        NewLcomD.Initialize();
        raNewLcom.Add(NewLcomD);
        nLcom = 1;
    }

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        int nDL=0, nCaseNum=0;
        for(int j=0; j<m_nDCount  ; ++j) nDL += Set_STLCIncludeCS(arDLoad.GetAt(j), adFactor[EN_JSCE17_D], nCaseNum, LcomD, D_SGLD_DL_CVL);
        for(int j=0; j<m_nGDCount ; ++j) Set_GridCase(arGridD.GetAt(j),  adFactor[EN_JSCE17_D], nCaseNum, LcomD, TRUE);  
        for(int j=0; j<m_nGDeCount; ++j) Set_GridCase(arGridDe.GetAt(j), adFactor[EN_JSCE17_D], nCaseNum, LcomD, TRUE);  

        // Description
        if(nDL > 0 || m_nGDCount  > 0 || m_nGDeCount > 0)
        {   
            int nCase = 0;
            CString csPrefix(_T("")), csLcom(_T(""));
            if(nDL         > 0) { csLcom += csPrefix + Get_JSCE17_LoadChar(EN_JSCE17_D);   csPrefix = _T("+"); nCase++; }
            if(m_nGDCount  > 0) { csLcom += csPrefix + _T("GD");  csPrefix = _T("+"); nCase++; }
            if(m_nGDeCount > 0) { csLcom += csPrefix + _T("GDE"); csPrefix = _T("+"); nCase++; }
            if(nCase>1)         { csLcom  = _T("{") + csLcom + _T("}"); }

            csPrefix = (LcomD.Description.IsEmpty() ? _T("") : _T("+"));
            LcomD.Description += csPrefix + asFactor[EN_JSCE17_D] + csLcom;
        }

        if(nCaseNum > 0)    raNewLcom[i] = LcomD;
    }
}

// Add Moving Load
void CLoadCombCtrl::Add_CvlJSCE17_M(const EN_JSCE17_NO& enLcomNo, ArUNT& arMLoad, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    double  adFactor[EN_JSCE17_NUM];
    CString asFactor[EN_JSCE17_NUM];
    if(!Get_JSCE17_Factor(enLcomNo, EN_JSCE17_NUM, adFactor, asFactor)) { ASSERT(0); return; }

    if(fabs(adFactor[EN_JSCE17_L]) < m_dZeroLim) return;
    if(m_nMCount==0) return; // Live Load - Moving

    int nLcom = raNewLcom.GetSize();
    if(nLcom==0)
    {
        T_LCOM_D NewLcomD;
        NewLcomD.Initialize();
        raNewLcom.Add(NewLcomD);
        nLcom = 1;
    }

    CArray<T_LCOM_D, T_LCOM_D&> aNewList;
    aNewList.RemoveAll();
    for(int i=0; i<nLcom; ++i)
    {
        for(int m=0; m<m_nMCount; ++m)  
        {
            T_LCOM_D LcomD = raNewLcom[i];
            
            int nExistCase = LcomD.aCombination.GetSize();
            int nCaseNum=nExistCase;
            Set_MovingCase(arMLoad.GetAt(m), adFactor[EN_JSCE17_L], nCaseNum, LcomD, TRUE);

            // Description
            if((nCaseNum-nExistCase) > 0)
            {
                CString strFixM;
                strFixM.Format(_T("[%d]"), m+1);
                CString csPrefix = LcomD.Description.IsEmpty() ? _T("") : _T("+");
                LcomD.Description += csPrefix + asFactor[EN_JSCE17_L] + _T("M") + strFixM;

                aNewList.Add(LcomD);
            }
        }
    }

    if(aNewList.GetSize() >= aNewList.GetSize())
    {
        raNewLcom.Copy(aNewList);
    }
}

// Add Live Load
void CLoadCombCtrl::Add_CvlJSCE17_L(const EN_JSCE17_NO& enLcomNo, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arGridLv , ArINT& arGridCrl, ArINT& arGridLe, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    double  adFactor[EN_JSCE17_NUM];
    CString asFactor[EN_JSCE17_NUM];
    if(!Get_JSCE17_Factor(enLcomNo, EN_JSCE17_NUM, adFactor, asFactor)) { ASSERT(0); return; }

    if(fabs(adFactor[EN_JSCE17_L]) < m_dZeroLim) return;
    if(m_nLCount == 0 && m_nGLvCount == 0 && m_nGCrlCount == 0 && m_nGLeCount == 0) return; // Live Load

    int nLcom = raNewLcom.GetSize();
    if(nLcom==0)
    {
        T_LCOM_D NewLcomD;
        NewLcomD.Initialize();
        raNewLcom.Add(NewLcomD);
        nLcom = 1;
    }

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();

        int nLL=0, nIL=0, nCRL=0, nCaseNum=nExistCase;
        if(m_nLCount > 0)
        {
            for(int j=0; j<m_nLCount  ; ++j) nLL += Set_STLCIncludeCS(arLLoad.GetAt(j),  adFactor[EN_JSCE17_L  ], nCaseNum, LcomD, 0);
            for(int j=0; j<m_nIlCount ; ++j) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(j), adFactor[EN_JSCE17_IL ], nCaseNum, LcomD, 0);
            for(int j=0; j<m_nCrlCount; ++j) nCRL+= Set_STLCIncludeCS(arCrlLoad.GetAt(j),adFactor[EN_JSCE17_CRL], nCaseNum, LcomD, 0);              
        }
        for(int j=0; j<m_nGLvCount ; ++j)  Set_GridCase(arGridLv.GetAt(j), adFactor[EN_JSCE17_L  ], nCaseNum, LcomD, TRUE);
        for(int j=0; j<m_nGCrlCount; ++j)  Set_GridCase(arGridCrl.GetAt(j),adFactor[EN_JSCE17_CRL], nCaseNum, LcomD, TRUE);
        for(int j=0; j<m_nGLeCount ; ++j)  Set_GridCase(arGridLe.GetAt(j), adFactor[EN_JSCE17_L  ], nCaseNum, LcomD, TRUE);

        // Description
        if(nLL > 0 || nIL > 0 || nCRL > 0 || 
           m_nGLvCount > 0 || m_nGCrlCount > 0 || m_nGLeCount > 0)
        {
            int nCase = 0;
            CString csPrefix(_T("")), csLcom(_T(""));
            if(nLL         > 0) { csLcom += csPrefix + Get_JSCE17_LoadChar(EN_JSCE17_L  ); csPrefix = _T("+"); nCase++; }
            if(nIL         > 0) { csLcom += csPrefix + Get_JSCE17_LoadChar(EN_JSCE17_IL ); csPrefix = _T("+"); nCase++; }
            if(nCRL        > 0) { csLcom += csPrefix + Get_JSCE17_LoadChar(EN_JSCE17_CRL); csPrefix = _T("+"); nCase++; }
            if(m_nGLvCount > 0) { csLcom += csPrefix + _T("GLV"); csPrefix = _T("+"); nCase++; }
            if(m_nGCrlCount> 0) { csLcom += csPrefix + _T("GCRL");csPrefix = _T("+"); nCase++; }
            if(m_nGLeCount > 0) { csLcom += csPrefix + _T("GLE"); csPrefix = _T("+"); nCase++; }
            if(nCase>1)         { csLcom  = _T("{") + csLcom + _T("}");                        }

            csPrefix = (LcomD.Description.IsEmpty() ? _T("") : _T("+"));
            LcomD.Description += csPrefix + asFactor[EN_JSCE17_L] + csLcom;
        }

        if((nCaseNum-nExistCase) > 0)    raNewLcom[i] = LcomD;
    }
}

// Add Prestressed, Creep, Shrinkage  Load
void CLoadCombCtrl::Add_CvlJSCE17_PS_CR_SH(const EN_JSCE17_NO& enLcomNo, ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    double  adFactor[EN_JSCE17_NUM];
    CString asFactor[EN_JSCE17_NUM];
    if(!Get_JSCE17_Factor(enLcomNo, EN_JSCE17_NUM, adFactor, asFactor)) { ASSERT(0); return; }

    if(fabs(adFactor[EN_JSCE17_PS]) < m_dZeroLim) return;
    if( m_nPsCount == 0 && m_nCrCount == 0 && m_nShCount == 0) return;

    int nLcom = raNewLcom.GetSize();
    if(nLcom==0)
    {
        T_LCOM_D NewLcomD;
        NewLcomD.Initialize();
        raNewLcom.Add(NewLcomD);
        nLcom = 1;
    }

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();

        int nPS=0, nCR=0, nSH=0, nCaseNum=nExistCase;
        for(int j=0; j<m_nLCount  ; ++j) nPS += Set_STLCIncludeCS(arPsLoad.GetAt(j), adFactor[EN_JSCE17_PS], nCaseNum, LcomD, D_SGLD_TS_CVL);
        for(int j=0; j<m_nIlCount ; ++j) nCR += Set_STLCIncludeCS(arCrLoad.GetAt(j), adFactor[EN_JSCE17_CR], nCaseNum, LcomD, D_SGLD_CS_CVL);
        for(int j=0; j<m_nCrlCount; ++j) nSH += Set_STLCIncludeCS(arShLoad.GetAt(j), adFactor[EN_JSCE17_SH], nCaseNum, LcomD, D_SGLD_SS_CVL);

        // Description
        if(nPS > 0 || nCR > 0 || nSH > 0)
        {
            int nCase = 0;
            CString csPrefix(_T("")), csLcom(_T(""));
            if(nPS > 0) { csLcom += csPrefix + Get_JSCE17_LoadChar(EN_JSCE17_PS); csPrefix = _T("+");  nCase++; }
            if(nCR > 0) { csLcom += csPrefix + Get_JSCE17_LoadChar(EN_JSCE17_CR); csPrefix = _T("+");  nCase++; }
            if(nSH > 0) { csLcom += csPrefix + Get_JSCE17_LoadChar(EN_JSCE17_SH); csPrefix = _T("+");  nCase++; }
            if(nCase>1) { csLcom  = _T("{") + csLcom + _T("}");                        }

            csPrefix = (LcomD.Description.IsEmpty() ? _T("") : _T("+"));
            LcomD.Description += csPrefix + asFactor[EN_JSCE17_PS] + csLcom;
        }

        if((nCaseNum-nExistCase) > 0)    raNewLcom[i] = LcomD;
    }
}

// Add Earth pressure, Water pressure, 부력 또는 양압력 하중
void CLoadCombCtrl::Add_CvlJSCE17_EP_WP_B(const EN_JSCE17_NO& enLcomNo, ArINT& arEpLoad, ArINT& arWpLoad, ArINT& arBLoad, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    double  adFactor[EN_JSCE17_NUM];
    CString asFactor[EN_JSCE17_NUM];
    if(!Get_JSCE17_Factor(enLcomNo, EN_JSCE17_NUM, adFactor, asFactor)) { ASSERT(0); return; }

    if(fabs(adFactor[EN_JSCE17_EP]) < m_dZeroLim) return;
    if( m_nEpCount == 0 && m_nWpCount == 0 && m_nBCount == 0) return;

    int nLcom = raNewLcom.GetSize();
    if(nLcom==0)
    {
        T_LCOM_D NewLcomD;
        NewLcomD.Initialize();
        raNewLcom.Add(NewLcomD);
        nLcom = 1;
    }

    CArray<T_LCOM_D, T_LCOM_D&> aNewList;
    aNewList.RemoveAll();

    int nEd = m_nBCount>0 ? 2 : 1;
    for(int B=0; B<nEd; ++B)
    {   
        for(int i=0; i<nLcom; ++i)
        {   
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();

            int nEP=0, nWP=0, nB=0, nCaseNum=nExistCase;
            for(int j=0; j<m_nEpCount; ++j) nEP += Set_STLCIncludeCS(arEpLoad.GetAt(j), adFactor[EN_JSCE17_EP], nCaseNum, LcomD, 0);
            for(int j=0; j<m_nWpCount; ++j) nWP += Set_STLCIncludeCS(arWpLoad.GetAt(j), adFactor[EN_JSCE17_WP], nCaseNum, LcomD, 0);
            if(B==1)
            {
                for(int j=0; j<m_nBCount ; ++j) nB += Set_STLCIncludeCS(arBLoad.GetAt(j), adFactor[EN_JSCE17_B], nCaseNum, LcomD, 0);
            }

            // Description
            if(nEP > 0 || nWP > 0 || nB > 0)
            {
                int nCase = 0;
                CString csPrefix(_T("")), csLcom(_T(""));
                if(nEP > 0) { csLcom += csPrefix + Get_JSCE17_LoadChar(EN_JSCE17_EP); csPrefix = _T("+"); nCase++; }
                if(nWP > 0) { csLcom += csPrefix + Get_JSCE17_LoadChar(EN_JSCE17_WP); csPrefix = _T("+"); nCase++; }
                if(nB  > 0) { csLcom += csPrefix + Get_JSCE17_LoadChar(EN_JSCE17_B ); csPrefix = _T("+"); nCase++; }
                if(nCase>1) { csLcom  = _T("{") + csLcom + _T("}");                       }

                csPrefix = (LcomD.Description.IsEmpty() ? _T("") : _T("+"));
                LcomD.Description += csPrefix + asFactor[EN_JSCE17_EP] + csLcom;
            }

            if((nCaseNum-nExistCase) > 0)                    aNewList.Add(LcomD);
            else if(B==0 && LcomD.aCombination.GetSize()!=0) aNewList.Add(LcomD);
        }
    }

    if(aNewList.GetSize() >= aNewList.GetSize())
    {
        raNewLcom.Copy(aNewList);
    }
}

// Add Settlement Load
void CLoadCombCtrl::Add_CvlJSCE17_STL_SM(const EN_JSCE17_NO& enLcomNo, ArINT& arStlLoad, ArUNT& arSmLoad, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    double  adFactor[EN_JSCE17_NUM];
    CString asFactor[EN_JSCE17_NUM];
    if(!Get_JSCE17_Factor(enLcomNo, EN_JSCE17_NUM, adFactor, asFactor)) { ASSERT(0); return; }

    if(fabs(adFactor[EN_JSCE17_STL]) < m_dZeroLim) return;
    if(m_nStlCount==0 && m_nSmCount==0) return;

    int nLcom = raNewLcom.GetSize();
    if(nLcom==0)
    {
        T_LCOM_D NewLcomD;
        NewLcomD.Initialize();
        raNewLcom.Add(NewLcomD);
        nLcom = 1;
    }

    CArray<T_LCOM_D, T_LCOM_D&> aNewList;
    aNewList.RemoveAll();
    for(int i=0; i<nLcom; ++i)
    {
        for(int Sm=0; Sm<m_nSmCount; ++Sm)  // non-static Settlement
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();

            int nSTL=0, nCaseNum=nExistCase;
            for(int j=0; j<m_nStlCount; j++) nSTL+= Set_STLCIncludeCS(arStlLoad.GetAt(j), adFactor[EN_JSCE17_STL], nCaseNum, LcomD, 0);
            if(m_nSmCount > 0)                      Set_SettleCase(arSmLoad.GetAt(Sm), adFactor[EN_JSCE17_SM], nCaseNum, LcomD, TRUE);
            
            // Description
            if((nCaseNum-nExistCase) > 0)
            {
                CString strFixSm;
                strFixSm.Format(_T("[%d]"), Sm+1);
                CString csPrefix = LcomD.Description.IsEmpty() ? _T("") : _T("+");
                if(nSTL       > 0) { LcomD.Description += csPrefix + asFactor[EN_JSCE17_STL] + Get_JSCE17_LoadChar(EN_JSCE17_STL);           csPrefix = _T("+"); }
                if(m_nSmCount > 0) { LcomD.Description += csPrefix + asFactor[EN_JSCE17_STL] + Get_JSCE17_LoadChar(EN_JSCE17_SM) + strFixSm; csPrefix = _T("+"); }

                aNewList.Add(LcomD);
            }
        }
    }

    if(aNewList.GetSize() >= aNewList.GetSize())
    {
        raNewLcom.Copy(aNewList);
    }
}

// Add 원심하중, 제동하중
void CLoadCombCtrl::Add_CvlJSCE17_CF_BRK(const EN_JSCE17_NO& enLcomNo, ArINT& arCfLoad, ArINT& arBrkLoad, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    double  adFactor[EN_JSCE17_NUM];
    CString asFactor[EN_JSCE17_NUM];
    if(!Get_JSCE17_Factor(enLcomNo, EN_JSCE17_NUM, adFactor, asFactor)) { ASSERT(0); return; }

    if(fabs(adFactor[EN_JSCE17_CF]) < m_dZeroLim) return;
    if( m_nCfCount == 0 && m_nBrkCount == 0 ) return;

    int nLcom = raNewLcom.GetSize();
    if(nLcom==0)
    {
        T_LCOM_D NewLcomD;
        NewLcomD.Initialize();
        raNewLcom.Add(NewLcomD);
        nLcom = 1;
    }

    CArray<T_LCOM_D, T_LCOM_D&> aNewList;
    aNewList.RemoveAll();

    for(int i=0; i<nLcom; ++i)
    {   
        for(int Brk=0; Brk<2; ++Brk)
        {
            for(int Cf=0; Cf<2; ++Cf)
            {
                T_LCOM_D LcomD = raNewLcom[i];
                int nExistCase = LcomD.aCombination.GetSize();

                if(Cf== 0 && Brk==0 && nExistCase>0)
                {
                    aNewList.Add(LcomD);
                    continue;
                }

                int nCf=0, nBrk=0, nCaseNum=nExistCase;
                if(Cf==1)
                {
                    for(int j=0; j<m_nCfCount ; ++j) nCf += Set_STLCIncludeCS(arCfLoad.GetAt(j), adFactor[EN_JSCE17_CF], nCaseNum, LcomD, 0);
                }
                if(Brk==1)
                {
                    for(int j=0; j<m_nBrkCount ; ++j) nBrk += Set_STLCIncludeCS(arBrkLoad.GetAt(j), adFactor[EN_JSCE17_BRK], nCaseNum, LcomD, 0);
                }

                // Description
                if(nCf > 0 || nBrk > 0)
                {
                    int nCase = 0;
                    CString csPrefix(_T("")), csLcom(_T(""));
                    if(nCf  > 0) { csLcom += csPrefix + Get_JSCE17_LoadChar(EN_JSCE17_CF );  csPrefix = _T("+"); nCase++; }
                    if(nBrk > 0) { csLcom += csPrefix + Get_JSCE17_LoadChar(EN_JSCE17_BRK);  csPrefix = _T("+"); nCase++; }
                    if(nCase>1)  { csLcom  = _T("{") + csLcom + _T("}");                         }

                    csPrefix = (LcomD.Description.IsEmpty() ? _T("") : _T("+"));
                    LcomD.Description += csPrefix + asFactor[EN_JSCE17_CF] + csLcom;
                }

                if((nCaseNum-nExistCase) > 0)    aNewList.Add(LcomD);
            }
        }
    }

    if(aNewList.GetSize() >= aNewList.GetSize())
    {
        raNewLcom.Copy(aNewList);
    }
}

// Add Construction stage load
void CLoadCombCtrl::Add_CvlJSCE17_CS(const EN_JSCE17_NO& enLcomNo, ArINT& arCSLoad , CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nConstLoad==0 || m_nCSCount==0) return; // Static only or no CS

    double  adFactor[EN_JSCE17_NUM];
    CString asFactor[EN_JSCE17_NUM];
    if(!Get_JSCE17_Factor(enLcomNo, EN_JSCE17_NUM, adFactor, asFactor)) { ASSERT(0); return; }

    double dPST = (m_bLossFactor ? m_dLossFactorTransfer : 1.0);
    double dPSS = (m_bLossFactor ? m_dLossFactorService  : 1.0);

    int nLcom = raNewLcom.GetSize();
    if(nLcom==0)
    {
        T_LCOM_D NewLcomD;
        NewLcomD.Initialize();
        raNewLcom.Add(NewLcomD);
        nLcom = 1;
    }

    auto lambda_SetCSnString1 = [this] (int iCaseKey, int iIdx, double* adFactor, int& iCaseNo, T_LCOM_D& rData, LPCTSTR lpszcEL, CString* astrLcom, int* anCase)
    {
        if(iIdx >= EN_JSCE17_NUM) return;

        if(adFactor[iIdx] > 0.0)
        {
            Set_CStageCase(iCaseKey, adFactor[iIdx], iCaseNo, rData, TRUE);

            CString csPrefix = astrLcom[iIdx].IsEmpty() ? _T("") : _T("+");
            astrLcom[iIdx] += csPrefix + lpszcEL;
            ++anCase[iIdx];
        }
    };
    auto lambda_SetCSnString2 = [this] (int iCaseKey, int iIdx, double dFactor, int& iCaseNo, T_LCOM_D& rData, LPCTSTR lpszcEL, CString* astrLcom, int* anCase)
    {
        if(iIdx >= EN_JSCE17_NUM) return;

        if(dFactor > 0.0)
        {
            Set_CStageCase(iCaseKey, dFactor, iCaseNo, rData, TRUE);

            CString csPrefix = astrLcom[iIdx].IsEmpty() ? _T("") : _T("+");
            astrLcom[iIdx] += csPrefix + lpszcEL;
            ++anCase[iIdx];
        }
    };

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];

        BOOL bE = FALSE, bTl=FALSE, bEr=FALSE;
        BOOL bEp =TRUE, bWp=TRUE, bTgp=TRUE;

        // Check Load combination
        int nExistCase = LcomD.aCombination.GetSize();
        for(int j=0; j<nExistCase; ++j)
        {
            if(LcomD.aCombination[j].AnalType == D_LCOM_STATIC)
            {
                T_STLD_D StlD;
                if(!m_pDoc->m_pAttrCtrl->GetStld(LcomD.aCombination[j].LoadCaseKey, StlD)) continue;
                if(StlD.LoadCaseType == _LSX(TL))
                    bTl = TRUE;
                else if(StlD.LoadCaseType == _LSX(ER))
                    bEr = TRUE;
                else if(StlD.LoadCaseType == _LSX(TG))
                {
                    if(LcomD.aCombination[j].Factor < 0.0) bTgp = FALSE;
                }
                else if(StlD.LoadCaseType == _LSX(W))
                {
                    if(LcomD.aCombination[j].Factor < 0.0) bWp = FALSE;
                }
                else if(StlD.LoadCaseType == _LSX(E))
                {
                    bE = TRUE;
                    if(LcomD.aCombination[j].Factor < 0.0) bEp = FALSE;
                }
            }
        }

        
        CString asCS[EN_JSCE17_NUM];
        int     anCase[EN_JSCE17_NUM];
        memset(anCase, 0, sizeof(anCase));
        int nLcasNum=nExistCase, n=0;
        if(CheckCS_SgldK(D_SGLD_DL_CVL, n))
            lambda_SetCSnString1(arCSLoad.GetAt(n), EN_JSCE17_D, adFactor, nLcasNum, LcomD, _T("(cD)"), asCS, anCase);

        // Erection load
        for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
        {
            CString strcEL;
            int nErection = 0;
            if(CheckCS_SgldK(m_CSEL_SgldK[nErec], n)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }
            else continue;

            if((CheckCSEL(nErection, _T("D")) || CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW"))))
                lambda_SetCSnString1(m_aCSLoad.GetAt(n), EN_JSCE17_D, adFactor, nLcasNum, LcomD, strcEL, asCS, anCase);
            else if(CheckCSEL(nErection, _T("L")))
                lambda_SetCSnString1(m_aCSLoad.GetAt(n), EN_JSCE17_L, adFactor, nLcasNum, LcomD, strcEL, asCS, anCase);
            else if(CheckCSEL(nErection, _T("W")))
                lambda_SetCSnString1(m_aCSLoad.GetAt(n), EN_JSCE17_W, adFactor, nLcasNum, LcomD, strcEL, asCS, anCase);
            else if(bE && CheckCSEL(nErection, "E"))
                lambda_SetCSnString1(m_aCSLoad.GetAt(n), EN_JSCE17_E, adFactor, nLcasNum, LcomD, strcEL, asCS, anCase);
            else if(CheckCSEL(nErection, _T("T")))
                lambda_SetCSnString1(m_aCSLoad.GetAt(n), EN_JSCE17_TG, adFactor, nLcasNum, LcomD, strcEL, asCS, anCase);
            else if(bTl && CheckCSEL(nErection, _T("TPG")))
                lambda_SetCSnString1(m_aCSLoad.GetAt(n), EN_JSCE17_TL, adFactor, nLcasNum, LcomD, strcEL, asCS, anCase);
            else if(bEr && CheckCSEL(nErection, _T("ER")))
                lambda_SetCSnString1(m_aCSLoad.GetAt(n), EN_JSCE17_ER, adFactor, nLcasNum, LcomD, strcEL, asCS, anCase);
        }

        // Tendon Primary, Tendon Secondary, Creep Secondary, Shrinkage Secondary
        if(adFactor[EN_JSCE17_PS] > 0.0)
        {
            if(CheckCS_SgldK(D_SGLD_TP_CVL, n)) // Tendon Primary
                lambda_SetCSnString2(arCSLoad.GetAt(n), EN_JSCE17_PS, dPST, nLcasNum, LcomD, _T("(cTP)"), asCS, anCase);
            if(CheckCS_SgldK(D_SGLD_TS_CVL, n)) // Tendon Secondary
                lambda_SetCSnString2(arCSLoad.GetAt(n), EN_JSCE17_PS, dPSS, nLcasNum, LcomD, _T("(cTS)"), asCS, anCase);
            if(CheckCS_SgldK(D_SGLD_CS_CVL, n)) // Creep Secondary
                lambda_SetCSnString1(arCSLoad.GetAt(n), EN_JSCE17_CR, adFactor, nLcasNum, LcomD, _T("(cCR)"), asCS, anCase);
            if(CheckCS_SgldK(D_SGLD_SS_CVL, n)) // Shrinkage Secondary
                lambda_SetCSnString1(arCSLoad.GetAt(n), EN_JSCE17_CR, adFactor, nLcasNum, LcomD, _T("(cSH)"), asCS, anCase);
        }

        // Description
        for(int j=0; j<EN_JSCE17_NUM; ++j)
        {
            if(asCS[j].IsEmpty()) continue;
            if(anCase[j] > 1)
                asCS[j] = _T("{") + asCS[j] + _T("}");
        }

        CString csPrefix = LcomD.Description.IsEmpty() ? _T("") : _T("+");
        if(!asCS[EN_JSCE17_D].IsEmpty()){
            LcomD.Description += csPrefix + asFactor[EN_JSCE17_D] + asCS[EN_JSCE17_D];  csPrefix = _T("+");
        }
        if(!asCS[EN_JSCE17_L].IsEmpty()){
            LcomD.Description += csPrefix + asFactor[EN_JSCE17_L] + asCS[EN_JSCE17_L];  csPrefix = _T("+");
        }
        if(!asCS[EN_JSCE17_PS].IsEmpty()){
            LcomD.Description += csPrefix + asFactor[EN_JSCE17_PS] + asCS[EN_JSCE17_PS];  csPrefix = _T("+");
        }
        if(!asCS[EN_JSCE17_TG].IsEmpty()){
            if(bTgp)  { LcomD.Description += csPrefix + asFactor[EN_JSCE17_TG] + asCS[EN_JSCE17_TG];  csPrefix = _T("+"); }
            else      { LcomD.Description += _T("-")  + asFactor[EN_JSCE17_TG] + asCS[EN_JSCE17_TG];  csPrefix = _T("+"); }
        }
        if(!asCS[EN_JSCE17_TL].IsEmpty()){
            LcomD.Description += csPrefix + asFactor[EN_JSCE17_TL] + asCS[EN_JSCE17_TL];  csPrefix = _T("+");
        }
        if(!asCS[EN_JSCE17_W].IsEmpty()){
            if(bWp)   { LcomD.Description += csPrefix + asFactor[EN_JSCE17_W] + asCS[EN_JSCE17_W];  csPrefix = _T("+"); }
            else      { LcomD.Description += _T("-")  + asFactor[EN_JSCE17_W] + asCS[EN_JSCE17_W];  csPrefix = _T("+"); }
        }
        if(!asCS[EN_JSCE17_E].IsEmpty()){
            if(bEp==0) { LcomD.Description += csPrefix + asFactor[EN_JSCE17_E] + asCS[EN_JSCE17_E];  csPrefix = _T("+"); }
            else       { LcomD.Description += _T("-")  + asFactor[EN_JSCE17_E] + asCS[EN_JSCE17_E];  csPrefix = _T("+"); }
        }
        if(!asCS[EN_JSCE17_ER].IsEmpty()){
            LcomD.Description += csPrefix + asFactor[EN_JSCE17_ER] + asCS[EN_JSCE17_ER];
        }

        if((nLcasNum-nExistCase) > 0)    raNewLcom[i] = LcomD;
    }
}

// 1개의 loadcase가 없고/있고 2개의 하중조합으로 처리.
void CLoadCombCtrl::Add_CvlJSCE17_AddOne(const EN_JSCE17_NO& enLcomNo, const EN_JSCE17& enLcase, ArINT& arLoad, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom, BOOL bOnebyOne)
{
    double  adFactor[EN_JSCE17_NUM];
    CString asFactor[EN_JSCE17_NUM];
    if(!Get_JSCE17_Factor(enLcomNo, EN_JSCE17_NUM, adFactor, asFactor)) { ASSERT(0); return; }

    if(fabs(adFactor[enLcase]) < m_dZeroLim) return;
    int nLoadCount = Get_JSCE17_LoadCount(enLcase);
    if( nLoadCount == 0) return;

    CArray<T_LCOM_D, T_LCOM_D&> aNewList;
    aNewList.RemoveAll();
    if(bOnebyOne) aNewList.Copy(raNewLcom); // Loadcase 없는 set copy

    int nLcom = raNewLcom.GetSize();
    if(nLcom==0)
    {
        T_LCOM_D NewLcomD;
        NewLcomD.Initialize();
        raNewLcom.Add(NewLcomD);
        nLcom = 1;
    }

    CString strLcase = Get_JSCE17_LoadChar(enLcase);
    for(int i=0; i<nLcom; ++i)
    {   
        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();

        int nER=0, nCaseNum=nExistCase;
        for(int j=0; j<nLoadCount ; ++j) nER += Set_STLCIncludeCS(arLoad.GetAt(j), adFactor[enLcase], nCaseNum, LcomD, 0);

        // Description
        if(nER > 0)
        {
            CString csPrefix = LcomD.Description.IsEmpty() ? _T("") : _T("+");
            LcomD.Description += csPrefix + asFactor[enLcase] + strLcase;
        }

        if((nCaseNum-nExistCase) > 0)    aNewList.Add(LcomD);
    }

    if(aNewList.GetSize() >= aNewList.GetSize())
    {
        raNewLcom.Copy(aNewList);
    }
}

// Add 방향별로
void CLoadCombCtrl::Add_CvlJSCE17_AddBiDir(const EN_JSCE17_NO& enLcomNo, const EN_JSCE17& enLcase, ArUNT& arLoad, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom, BOOL bBiDir)
{   
    int nLoad = arLoad.GetSize();
    if(nLoad==0) return;

    ArINT arInt;
    arInt.SetSize(nLoad);
    for(int i=0; i<nLoad; ++i)  arInt[i] = static_cast<int>(arLoad[i]);
    Add_CvlJSCE17_AddBiDir(enLcomNo, enLcase, arInt, raNewLcom, bBiDir);
}

void CLoadCombCtrl::Add_CvlJSCE17_AddBiDir(const EN_JSCE17_NO& enLcomNo, const EN_JSCE17& enLcase, ArINT& arLoad, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom, BOOL bBiDir)
{
    double  adFactor[EN_JSCE17_NUM];
    CString asFactor[EN_JSCE17_NUM];
    if(!Get_JSCE17_Factor(enLcomNo, EN_JSCE17_NUM, adFactor, asFactor)) { ASSERT(0); return; }

    if(fabs(adFactor[enLcase]) < m_dZeroLim) return;
    int nLoadCount = Get_JSCE17_LoadCount(enLcase);
    if( nLoadCount == 0) return;

    int nLcom = raNewLcom.GetSize();
    if(nLcom==0)
    {
        T_LCOM_D NewLcomD;
        NewLcomD.Initialize();
        raNewLcom.Add(NewLcomD);
        nLcom = 1;
    }

    CArray<T_LCOM_D, T_LCOM_D&> aNewList;
    aNewList.RemoveAll();

    CString strLcase = Get_JSCE17_LoadChar(enLcase);
    int nEd = bBiDir ? 2 : 1;
    for(int i=0; i<nLcom; ++i)
    {
        for(int j=0; j<nLoadCount; ++j)  
        {
            for(int p=0; p<nEd; ++p)
            {
                T_LCOM_D LcomD = raNewLcom[i];
                int nExistCase = LcomD.aCombination.GetSize();

                double dFactor = (p==0 ? 1.0 : -1.0) * adFactor[enLcase];

                int nCaseNum=nExistCase;
                if(enLcase == EN_JSCE17_ESP)
                    Set_SpectrumCase(arLoad.GetAt(j), dFactor, nCaseNum, LcomD, TRUE);
                else 
                    Set_STLCIncludeCS(arLoad.GetAt(j), dFactor, nCaseNum, LcomD, 0);

                // Description
                if((nCaseNum-nExistCase) > 0)
                {
                    CString strFixTg;
                    strFixTg.Format(_T("[%d]"), j+1);
                    CString csPrefix = LcomD.Description.IsEmpty() ? _T("") : _T("+");
                    if(p==0) LcomD.Description += csPrefix + asFactor[enLcase] + strLcase + strFixTg;
                    else     LcomD.Description += _T("-")  + asFactor[enLcase] + strLcase + strFixTg;

                    aNewList.Add(LcomD);
                }
            }
        }
    }

    if(aNewList.GetSize() >= aNewList.GetSize())
    {
        raNewLcom.Copy(aNewList);
    }
}

////////////////////////////////////////////////////////////////////////
////// AS5100 Simple Refactored Version by khj0102 2019/07/25
////////////////////////////////////////////////////////////////////////

void CLoadCombCtrl::GetAS_5100_2017Factor( 
    CString& sD1Fac,  CString& sD2Fac,  CString& sDw1Fac,  CString& sDw2Fac,  CString& sEp1Fac,		 CString& sEp2Fac,
    CString& sWp1Fac, CString& sWp2Fac,     
    double& dD1,  double& dD2,  double& dDw1,  double& dDw2,  double& dEp1,   double& dEp2,   
    double& dWp1, double& dWp2, BOOL b1stGrp)
{
    // b1stGrp = TRUE  : Group 1 일때 
    // b1stGrp = FALSE : Group 1 Fatigue or Group 2 일때 
    if(b1stGrp)
    {
        dD1 = m_dAS5100D[0];   sD1Fac.Format(_T("%.2f"), m_dAS5100D[0]);
        dD2 = m_dAS5100D[1];   sD2Fac.Format(_T("%.2f"), m_dAS5100D[1]);
        dDw1 = m_dAS5100DW[0]; sDw1Fac.Format(_T("%.2f"), m_dAS5100DW[0]);
        dDw2 = m_dAS5100DW[1]; sDw2Fac.Format(_T("%.2f"), m_dAS5100DW[1]);
        dEp1 = m_dAS5100SL[0]; sEp1Fac.Format(_T("%.2f"), m_dAS5100SL[0]);
        dEp2 = m_dAS5100SL[1]; sEp2Fac.Format(_T("%.2f"), m_dAS5100SL[1]);
        dWp1 = m_dAS5100GL[0]; sWp1Fac.Format(_T("%.2f"), m_dAS5100GL[0]);
        dWp2 = m_dAS5100GL[1]; sWp2Fac.Format(_T("%.2f"), m_dAS5100GL[1]);
    }
    else
    {
        dD1 = 1.0;	sD1Fac.Format(_T("%.2f"), 1.0);
        dD2 = 1.0;	sD2Fac.Format(_T("%.2f"), 1.0);
        dDw1 = 1.3; sDw1Fac.Format(_T("%.2f"), 1.3); 
        dDw2 = 1.3; sDw2Fac.Format(_T("%.2f"), 1.3);
        dEp1 = 1.2; sEp1Fac.Format(_T("%.2f"), 1.2);
        dEp2 = 1.2; sEp2Fac.Format(_T("%.2f"), 1.2);
        dWp1 = 1.0; sWp1Fac.Format(_T("%.2f"), 1.0);
        dWp2 = 1.0; sWp2Fac.Format(_T("%.2f"), 1.0);
    }
}

int CLoadCombCtrl::Get_AS5100_LCOM_List(CArray<AS5100_LCOM, AS5100_LCOM&>& raLcomList)
{
    raLcomList.RemoveAll();

    AS5100_LCOM AS5100D;

    //PE(= D + DW + EP + WP + PS + CR + SH + STL + SM + FP + B)

    //ULS C 는 ULS B인 경우에서 Road Type이 Pedestrian인 경우

    AS5100D.Set(EN_AS5100_ULS_01    );  raLcomList.Add(AS5100D);    // ULS A: Minimum Strength and Stability - D + DW + EP + WP
    AS5100D.Set(EN_AS5100_ULS_02_BRK);  raLcomList.Add(AS5100D);    // ULS B: PE + Moving
    AS5100D.Set(EN_AS5100_ULS_02_CF );  raLcomList.Add(AS5100D);    // ULS B: PE + Moving + Temp
    AS5100D.Set(EN_AS5100_ULS_03_BRK);  raLcomList.Add(AS5100D);    // ULS B: PE + Moving + Wind
    AS5100D.Set(EN_AS5100_ULS_03_CF );  raLcomList.Add(AS5100D);    // ULS B: PE + Moving
    AS5100D.Set(EN_AS5100_ULS_04_BRK);  raLcomList.Add(AS5100D);    // ULS B: PE + Moving + Temp
    AS5100D.Set(EN_AS5100_ULS_04_CF );  raLcomList.Add(AS5100D);    // ULS B: PE + Moving + Wind
    AS5100D.Set(EN_AS5100_ULS_05    );  raLcomList.Add(AS5100D);    // ULS E: PE + CO
    AS5100D.Set(EN_AS5100_ULS_06    );  raLcomList.Add(AS5100D);    // ULS E: PE + CO + Moving
    AS5100D.Set(EN_AS5100_ULS_07    );   raLcomList.Add(AS5100D);   // ULS H: PE + Earthquake
    AS5100D.Set(EN_AS5100_ULS_08    );   raLcomList.Add(AS5100D);   // ULS J: PE + Wind
    AS5100D.Set(EN_AS5100_ULS_09    );   raLcomList.Add(AS5100D);   // ULS J: PE + Wind + Temp
    AS5100D.Set(EN_AS5100_ULS_10    );   raLcomList.Add(AS5100D);   // ULS K: PE + Temp
    AS5100D.Set(EN_AS5100_ULS_11    );   raLcomList.Add(AS5100D);   // ULS K: PE + Temp + Moving

    AS5100D.Set(EN_AS5100_SLS_01_BRK);  raLcomList.Add(AS5100D);// SLS : PE + Moving + 0.5(Wind + Earthquake)
    AS5100D.Set(EN_AS5100_SLS_01_CF );  raLcomList.Add(AS5100D);// SLS : PE + Moving + 0.5(Wind + Earthquake)
    AS5100D.Set(EN_AS5100_SLS_02_BRK);  raLcomList.Add(AS5100D);// SLS : PE + Moving + 0.5(Temp + Earthquake)
    AS5100D.Set(EN_AS5100_SLS_02_CF );  raLcomList.Add(AS5100D);// SLS : PE + Moving + 0.5(Temp + Earthquake)
    AS5100D.Set(EN_AS5100_SLS_03_BRK);  raLcomList.Add(AS5100D);// SLS : PE + Moving + 0.5(Wind + Temp)
    AS5100D.Set(EN_AS5100_SLS_03_CF );  raLcomList.Add(AS5100D);// SLS : PE + Moving + 0.5(Wind + Temp)
    AS5100D.Set(EN_AS5100_SLS_01_07_BRK );  raLcomList.Add(AS5100D);// SLS : PE + Moving + 0.7Wind
    AS5100D.Set(EN_AS5100_SLS_01_07_CF  );  raLcomList.Add(AS5100D);// SLS : PE + Moving + 0.7Wind
    AS5100D.Set(EN_AS5100_SLS_02_07_BRK );  raLcomList.Add(AS5100D);// SLS : PE + Moving + 0.7Temp
    AS5100D.Set(EN_AS5100_SLS_02_07_CF  );  raLcomList.Add(AS5100D);// SLS : PE + Moving + 0.7Temp
    AS5100D.Set(EN_AS5100_SLS_03_07_BRK );  raLcomList.Add(AS5100D);// SLS : PE + Moving + 0.7Earthquake
    AS5100D.Set(EN_AS5100_SLS_03_07_CF  );  raLcomList.Add(AS5100D);// SLS : PE + Moving + 0.7Earthquake
    AS5100D.Set(EN_AS5100_SLS_04    );  raLcomList.Add(AS5100D);// SLS : PE + Temp + 0.5(Wind + Earthquake)                                  
    AS5100D.Set(EN_AS5100_SLS_05_BRK);  raLcomList.Add(AS5100D);// SLS : PE + Temp + 0.5(Moving + Wind)
    AS5100D.Set(EN_AS5100_SLS_05_CF );  raLcomList.Add(AS5100D);// SLS : PE + Temp + 0.5(Moving + Wind)
    AS5100D.Set(EN_AS5100_SLS_06_BRK);  raLcomList.Add(AS5100D);// SLS : PE + Temp + 0.5(Earthquake + Moving)
    AS5100D.Set(EN_AS5100_SLS_06_CF );  raLcomList.Add(AS5100D);// SLS : PE + Temp + 0.5(Earthquake + Moving)
    AS5100D.Set(EN_AS5100_SLS_04_07    );  raLcomList.Add(AS5100D);// SLS : PE + Temp + 0.7Wind                             
    AS5100D.Set(EN_AS5100_SLS_05_07_BRK);  raLcomList.Add(AS5100D);// SLS : PE + Temp + 0.7Moving     
    AS5100D.Set(EN_AS5100_SLS_05_07_CF );  raLcomList.Add(AS5100D);// SLS : PE + Temp + 0.7Moving     
    AS5100D.Set(EN_AS5100_SLS_06_07    );  raLcomList.Add(AS5100D);// SLS : PE + Temp + 0.7Earthquake
    AS5100D.Set(EN_AS5100_SLS_07    );  raLcomList.Add(AS5100D);// SLS : PE + Wind + 0.5(Earthquake + Temp)
    AS5100D.Set(EN_AS5100_SLS_08_BRK);  raLcomList.Add(AS5100D);// SLS : PE + Wind + 0.5(Temp + Moving),      
    AS5100D.Set(EN_AS5100_SLS_08_CF );  raLcomList.Add(AS5100D);// SLS : PE + Wind + 0.5(Temp + Moving),      
    AS5100D.Set(EN_AS5100_SLS_09_BRK);  raLcomList.Add(AS5100D);// SLS : PE + Wind + 0.5(Moving + Earthquake)
    AS5100D.Set(EN_AS5100_SLS_09_CF );  raLcomList.Add(AS5100D);// SLS : PE + Wind + 0.5(Moving + Earthquake)
    AS5100D.Set(EN_AS5100_SLS_07_07     );  raLcomList.Add(AS5100D);// SLS : PE + Wind + 0.7Earthquake
    AS5100D.Set(EN_AS5100_SLS_08_07     );  raLcomList.Add(AS5100D);// SLS : PE + Wind + 0.7Temp    
    AS5100D.Set(EN_AS5100_SLS_09_07_BRK );  raLcomList.Add(AS5100D);// SLS : PE + Wind + 0.7Moving 
    AS5100D.Set(EN_AS5100_SLS_09_07_CF  );  raLcomList.Add(AS5100D);// SLS : PE + Wind + 0.7Moving
    AS5100D.Set(EN_AS5100_SLS_10    );  raLcomList.Add(AS5100D);// SLS : PE + Earthquake + 0.5(Wind + Temp)
    AS5100D.Set(EN_AS5100_SLS_11_BRK);  raLcomList.Add(AS5100D);// SLS : PE + Earthquake + 0.5(Temp + Moving)
    AS5100D.Set(EN_AS5100_SLS_11_CF );  raLcomList.Add(AS5100D);// SLS : PE + Earthquake + 0.5(Temp + Moving)    
    AS5100D.Set(EN_AS5100_SLS_12_BRK);  raLcomList.Add(AS5100D);// SLS : PE + Earthquake + 0.5(Moving + Wind)
    AS5100D.Set(EN_AS5100_SLS_12_CF );  raLcomList.Add(AS5100D);// SLS : PE + Earthquake + 0.5(Moving + Wind)
    AS5100D.Set(EN_AS5100_SLS_10_07     );  raLcomList.Add(AS5100D);// SLS : PE + Earthquake + 0.7Wind
    AS5100D.Set(EN_AS5100_SLS_11_07     );  raLcomList.Add(AS5100D);// SLS : PE + Earthquake + 0.7Temp
    AS5100D.Set(EN_AS5100_SLS_12_07_BRK );  raLcomList.Add(AS5100D);// SLS : PE + Earthquake + 0.7Moving  
    AS5100D.Set(EN_AS5100_SLS_12_07_CF  );  raLcomList.Add(AS5100D);// SLS : PE + Earthquake + 0.7Moving

    AS5100D.Set(EN_AS5100_Transfer_01);  raLcomList.Add(AS5100D);

    AS5100D.Set(EN_AS5100_FLS_01    );  raLcomList.Add(AS5100D);// FLS : 0.7 * Moving Load * (1 + alpha)

    return raLcomList.GetSize();
}

BOOL CLoadCombCtrl::Check_AS5100_LoadCase(const EN_AS5100_NO& enLcomNo)
{
    //PE(= D + DW + EP + WP + PS + CR + SH + STL + SM + FP + B) 중 하나라도 없는 경우 return FALSE;

    if (enLcomNo > EN_AS5100_ULS_01)
    {
        if (m_nDCount  == 0 && m_nDwCount == 0 && m_nEpCount  == 0 && m_nWpCount == 0 && m_nShCount == 0 && m_nCrCount == 0 &&
            m_nFpCount == 0 && m_nBCount  == 0 && m_nStlCount == 0 && m_nSmCount == 0 && m_nPsCount == 0 && m_nMCount == 0) return FALSE;
    }

    // Load check
    switch (enLcomNo)
    {
    case EN_AS5100_ULS_01: // ULS A: Minimum Strength and Stability - D + DW + EP + WP
        break;
    case EN_AS5100_ULS_02_BRK: // ULS B: PE + Moving
    case EN_AS5100_ULS_02_CF: // ULS B: PE + Moving
        if(m_nMCount == 0) return FALSE;
        break;
    case EN_AS5100_ULS_03_BRK: // ULS B: PE + Moving + Temp
    case EN_AS5100_ULS_03_CF: // ULS B: PE + Moving + Temp
        if(m_nMCount == 0 && m_nTCount == 0 && m_nTpgCount == 0) return FALSE;
        break;
    case EN_AS5100_ULS_04_BRK: // ULS B: PE + Moving + Wind
    case EN_AS5100_ULS_04_CF: // ULS B: PE + Moving + Wind
        if(m_nMCount == 0 && m_nWCount == 0) return FALSE;
        break;
    case EN_AS5100_ULS_05: // ULS E: PE + CO
        if(m_nCoCount == 0) return FALSE;
        break;
    case EN_AS5100_ULS_06: // ULS E: PE + CO + Moving
        if(m_nCoCount == 0 && m_nMCount == 0) return FALSE;
        break;
    case EN_AS5100_ULS_07: // ULS H: PE + Earthquake
        if(m_nECount == 0 && m_nEspCount == 0 ) return FALSE;
        break;
    case EN_AS5100_ULS_08: // ULS J: PE + Wind
        if(m_nWCount == 0) return FALSE;
        break;
    case EN_AS5100_ULS_09: // ULS J: PE + Wind + Temp
        if(m_nWCount == 0 && m_nTCount == 0 && m_nTpgCount == 0) return FALSE;
        break;
    case EN_AS5100_ULS_10: // ULS K: PE + Temp
        if(m_nTCount == 0 && m_nTpgCount == 0) return FALSE;
        break;
    case EN_AS5100_ULS_11: // ULS K: PE + Temp + Moving
        if(m_nTCount == 0 && m_nTpgCount == 0 && m_nMCount == 0) return FALSE;
        break;

    case EN_AS5100_SLS_01_BRK:// SLS : PE + Moving + 0.5(Wind + Earthquake)
    case EN_AS5100_SLS_01_CF:// SLS : PE + Moving + 0.5(Wind + Earthquake)
        if(m_nMCount == 0 && m_nWCount == 0 && m_nECount == 0) return FALSE;
        break;
    case EN_AS5100_SLS_02_BRK:// SLS : PE + Moving + 0.5(Temp + Earthquake)
    case EN_AS5100_SLS_02_CF:// SLS : PE + Moving + 0.5(Temp + Earthquake)
        if(m_nMCount == 0 && m_nTCount == 0 && m_nTpgCount == 0 && m_nECount == 0) return FALSE;
        break;
    case EN_AS5100_SLS_03_BRK:// SLS : PE + Moving + 0.5(Wind + Temp)
    case EN_AS5100_SLS_03_CF:// SLS : PE + Moving + 0.5(Wind + Temp)
        if(m_nMCount == 0 && m_nTCount == 0 && m_nTpgCount == 0 && m_nWCount == 0) return FALSE;
        break;
    case EN_AS5100_SLS_01_07_BRK:// SLS : PE + Moving + 0.7Wind
    case EN_AS5100_SLS_01_07_CF:// SLS : PE + Moving + 0.7Wind
        if(m_nMCount == 0 && m_nWCount == 0) return FALSE;
        if(m_nMCount == 0) return FALSE;
        break;
    case EN_AS5100_SLS_02_07_BRK:// SLS : PE + Moving + 0.7Temp
    case EN_AS5100_SLS_02_07_CF:// SLS : PE + Moving + 0.7Temp
        if(m_nMCount == 0 && m_nTCount == 0 && m_nTpgCount == 0) return FALSE;
        if(m_nMCount == 0) return FALSE;
        break;
    case EN_AS5100_SLS_03_07_BRK:// SLS : PE + Moving + 0.7Earthquake
    case EN_AS5100_SLS_03_07_CF:// SLS : PE + Moving + 0.7Earthquake
        if(m_nMCount == 0 && m_nECount == 0) return FALSE;
        if(m_nMCount == 0) return FALSE;
        break;

    case EN_AS5100_SLS_04:// SLS : PE + Temp + 0.5(Wind + Earthquake)                
        if(m_nTCount == 0 && m_nTpgCount == 0 && m_nWCount == 0 && m_nECount == 0) return FALSE;
        break;
    case EN_AS5100_SLS_05_BRK:// SLS : PE + Temp + 0.5(Moving + Wind)
    case EN_AS5100_SLS_05_CF:// SLS : PE + Temp + 0.5(Moving + Wind)
        if(m_nTCount == 0 && m_nTpgCount == 0 && m_nMCount == 0 && m_nWCount == 0) return FALSE;
        break;
    case EN_AS5100_SLS_06_BRK:// SLS : PE + Temp + 0.5(Earthquake + Moving)
    case EN_AS5100_SLS_06_CF:// SLS : PE + Temp + 0.5(Earthquake + Moving)
        if(m_nTCount == 0 && m_nTpgCount == 0 && m_nMCount == 0 && m_nECount == 0) return FALSE;
        break;
    case EN_AS5100_SLS_04_07:// SLS : PE + Temp + 0.7Wind      
        if(m_nTCount == 0 && m_nTpgCount == 0 && m_nWCount == 0) return FALSE;
        if(m_nTCount == 0 && m_nTpgCount == 0) return FALSE;
        break;
    case EN_AS5100_SLS_05_07_BRK:// SLS : PE + Temp + 0.7Moving 
    case EN_AS5100_SLS_05_07_CF: // SLS : PE + Temp + 0.7Moving 
        if(m_nTCount == 0 && m_nTpgCount == 0 && m_nMCount == 0) return FALSE;
        if(m_nTCount == 0 && m_nTpgCount == 0) return FALSE;
        break;
    case EN_AS5100_SLS_06_07:// SLS : PE + Temp + 0.7Earthquake
        if(m_nTCount == 0 && m_nTpgCount == 0 && m_nECount == 0) return FALSE;
        if(m_nTCount == 0 && m_nTpgCount == 0) return FALSE;
        break;

    case EN_AS5100_SLS_07:// SLS : PE + Wind + 0.5(Earthquake + Temp)
        if(m_nWCount == 0 && m_nTCount == 0 && m_nTpgCount == 0 && m_nECount == 0) return FALSE;
        break;
    case EN_AS5100_SLS_08_BRK:// SLS : PE + Wind + 0.5(Temp + Moving)
    case EN_AS5100_SLS_08_CF:// SLS : PE + Wind + 0.5(Temp + Moving)
        if(m_nWCount == 0 && m_nTCount == 0 && m_nTpgCount == 0 && m_nMCount == 0) return FALSE;
        break;
    case EN_AS5100_SLS_09_BRK:// SLS : PE + Wind + 0.5(Moving + Earthquake)
    case EN_AS5100_SLS_09_CF:// SLS : PE + Wind + 0.5(Moving + Earthquake)
        if(m_nWCount == 0 && m_nMCount == 0 && m_nECount == 0) return FALSE;
        break;
    case EN_AS5100_SLS_07_07:// SLS : PE + Wind + 0.7Earthquake
        if(m_nWCount == 0 && m_nECount == 0) return FALSE;
        if(m_nWCount == 0) return FALSE;
        break;
    case EN_AS5100_SLS_08_07:// SLS : PE + Wind + 0.7Temp  
        if(m_nWCount == 0 && m_nTCount == 0 && m_nTpgCount == 0) return FALSE;
        if(m_nWCount == 0) return FALSE;
        break;
    case EN_AS5100_SLS_09_07_BRK:   // SLS : PE + Wind + 0.7Moving
    case EN_AS5100_SLS_09_07_CF:    // SLS : PE + Wind + 0.7Moving
        if(m_nWCount == 0 && m_nMCount == 0) return FALSE;
        if(m_nWCount == 0) return FALSE;
        break;

    case EN_AS5100_SLS_10:// SLS : PE + Earthquake + 0.5(Wind + Temp)
        if(m_nECount == 0 && m_nTCount == 0 && m_nTpgCount == 0 && m_nWCount == 0) return FALSE;
        break;
    case EN_AS5100_SLS_11_BRK:// SLS : PE + Earthquake + 0.5(Temp + Moving)
    case EN_AS5100_SLS_11_CF:// SLS : PE + Earthquake + 0.5(Temp + Moving)
        if(m_nECount == 0 && m_nTCount == 0 && m_nTpgCount == 0 && m_nMCount == 0) return FALSE;
        break;
    case EN_AS5100_SLS_12_BRK:// SLS : PE + Earthquake + 0.5(Moving + Wind)
    case EN_AS5100_SLS_12_CF:// SLS : PE + Earthquake + 0.5(Moving + Wind)
        if(m_nECount == 0 && m_nMCount == 0 && m_nWCount == 0) return FALSE;
        break;
    case EN_AS5100_SLS_10_07:// SLS : PE + Earthquake + 0.7Wind
        if(m_nECount == 0 && m_nWCount == 0) return FALSE;
        if(m_nECount == 0) return FALSE;
        break;
    case EN_AS5100_SLS_11_07:// SLS : PE + Earthquake + 0.7Temp
        if(m_nECount == 0 && m_nTCount == 0 && m_nTpgCount == 0) return FALSE;
        if(m_nECount == 0) return FALSE;
        break;
    case EN_AS5100_SLS_12_07_BRK:   // SLS : PE + Earthquake + 0.7Moving
    case EN_AS5100_SLS_12_07_CF:    // SLS : PE + Earthquake + 0.7Moving
        if(m_nECount == 0 && m_nMCount == 0) return FALSE;
        if(m_nECount == 0) return FALSE;
        break;

    case EN_AS5100_Transfer_01:
        if ( m_nDCount == 0 && m_nConstLoad == 0 && m_nPsCount == 0 ) return FALSE;
        break;

    case EN_AS5100_FLS_01:// FLS : 0.7 * Moving Load * (1 + alpha)
        if(m_nMCount == 0) return FALSE;
        break;
    default:
        ASSERT(0);
        return FALSE;
        break;
    }
    return TRUE;
}

void CLoadCombCtrl::Set_CvlAS5100(const EN_AS5100_NO& enLcomNo, ArINT& arDLoad , ArINT& arDwLoad, ArINT& arEpLoad, ArINT& arWpLoad , 
                                  ArINT& arFpLoad, ArINT& arBLoad , ArINT& arCrLoad, ArINT& arShLoad , ArINT& arPsLoad , ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arCoLoad, 
                                  ArINT& arWLoad,  ArINT& arELoad , ArUNT& arEspLoad, ArINT& arTLoad , ArINT& arTpgLoad, ArINT& arBrkLoad, ArINT& arCfLoad , ArUNT& arMLoad , ArINT& arCSLoad)
{
    if(!Check_AS5100_LoadCase(enLcomNo)) return;

    BOOL b1stGrp;

    switch (enLcomNo)
    {
    case EN_AS5100_ULS_01:      // ULS A: Minimum Strength and Stability - D + DW + EP + WP
    case EN_AS5100_ULS_02_BRK:  // ULS B: PE + Moving
    case EN_AS5100_ULS_02_CF:   // ULS B: PE + Moving
    case EN_AS5100_ULS_03_BRK:  // ULS B: PE + Moving + Temp
    case EN_AS5100_ULS_03_CF:   // ULS B: PE + Moving + Temp
    case EN_AS5100_ULS_04_BRK:  // ULS B: PE + Moving + Wind
    case EN_AS5100_ULS_04_CF:   // ULS B: PE + Moving + Wind
    case EN_AS5100_ULS_05:      // ULS E: PE + CO
    case EN_AS5100_ULS_06:      // ULS E: PE + CO + Moving
    case EN_AS5100_ULS_07:      // ULS H: PE + Earthquake
    case EN_AS5100_ULS_08:      // ULS J: PE + Wind
    case EN_AS5100_ULS_09:      // ULS J: PE + Wind + Temp
    case EN_AS5100_ULS_10:      // ULS K: PE + Temp
    case EN_AS5100_ULS_11:      // ULS K: PE + Temp + Moving
        b1stGrp = TRUE;
        Set_CvlAS5100_ULS(enLcomNo, arDLoad, arDwLoad, arEpLoad, arWpLoad, arFpLoad, arBLoad, arCrLoad, arShLoad,arPsLoad, arStlLoad, arSmLoad, arCoLoad, arCSLoad, arMLoad, arBrkLoad , arCfLoad, arWLoad, arTLoad, arTpgLoad, arELoad, arEspLoad, b1stGrp);
        break;
    case EN_AS5100_SLS_01_BRK:      // SLS : PE + Moving + 0.5(Wind + Earthquake)
    case EN_AS5100_SLS_01_CF:       // SLS : PE + Moving + 0.5(Wind + Earthquake)
    case EN_AS5100_SLS_02_BRK:      // SLS : PE + Moving + 0.5(Earthquake + Temp)
    case EN_AS5100_SLS_02_CF:       // SLS : PE + Moving + 0.5(Earthquake + Temp)
    case EN_AS5100_SLS_03_BRK:      // SLS : PE + Moving + 0.5(Temp + Wind)
    case EN_AS5100_SLS_03_CF:       // SLS : PE + Moving + 0.5(Temp + Wind)
    case EN_AS5100_SLS_01_07_BRK:   // SLS : PE + Moving + 0.7Wind
    case EN_AS5100_SLS_01_07_CF :   // SLS : PE + Moving + 0.7Wind
    case EN_AS5100_SLS_02_07_BRK:   // SLS : PE + Moving + 0.7Temp
    case EN_AS5100_SLS_02_07_CF:    // SLS : PE + Moving + 0.7Temp
    case EN_AS5100_SLS_03_07_BRK:   // SLS : PE + Moving + 0.7Earthquake
    case EN_AS5100_SLS_03_07_CF:    // SLS : PE + Moving + 0.7Earthquake
    case EN_AS5100_SLS_04:          // SLS : PE + Temp + 0.5(Wind + Earthquake)
    case EN_AS5100_SLS_05_BRK:      // SLS : PE + Temp + 0.5(Moving + Wind)
    case EN_AS5100_SLS_05_CF:       // SLS : PE + Temp + 0.5(Moving + Wind)
    case EN_AS5100_SLS_06_BRK:      // SLS : PE + Temp + 0.5(Earthquake + Moving)
    case EN_AS5100_SLS_06_CF:       // SLS : PE + Temp + 0.5(Earthquake + Moving)
    case EN_AS5100_SLS_04_07:       // SLS : PE + Temp + 0.7Wind
    case EN_AS5100_SLS_05_07_BRK:   // SLS : PE + Temp + 0.7Moving      
    case EN_AS5100_SLS_05_07_CF:    // SLS : PE + Temp + 0.7Moving      
    case EN_AS5100_SLS_06_07:       // SLS : PE + Temp + 0.7Earthquake
    case EN_AS5100_SLS_07:          // SLS : PE + Wind + 0.5(Earthquake + Temp)
    case EN_AS5100_SLS_08_BRK:      // SLS : PE + Wind + 0.5(Temp + Moving),        
    case EN_AS5100_SLS_08_CF:       // SLS : PE + Wind + 0.5(Temp + Moving),        
    case EN_AS5100_SLS_09_BRK:      // SLS : PE + Wind + 0.5(Moving + Earthquake)
    case EN_AS5100_SLS_09_CF:       // SLS : PE + Wind + 0.5(Moving + Earthquake)
    case EN_AS5100_SLS_07_07:       // SLS : PE + Wind + 0.7Earthquake
    case EN_AS5100_SLS_08_07:       // SLS : PE + Wind + 0.7Temp         
    case EN_AS5100_SLS_09_07_BRK:   // SLS : PE + Wind + 0.7Moving   
    case EN_AS5100_SLS_09_07_CF:    // SLS : PE + Wind + 0.7Moving
    case EN_AS5100_SLS_10:          // SLS : PE + Earthquake + 0.5(Wind + Temp)
    case EN_AS5100_SLS_11_BRK:      // SLS : PE + Earthquake + 0.5(Temp + Moving)
    case EN_AS5100_SLS_11_CF:       // SLS : PE + Earthquake + 0.5(Temp + Moving)
    case EN_AS5100_SLS_12_BRK:      // SLS : PE + Earthquake + 0.5(Moving + Wind)
    case EN_AS5100_SLS_12_CF:       // SLS : PE + Earthquake + 0.5(Moving + Wind)
    case EN_AS5100_SLS_10_07:       // SLS : PE + Earthquake + 0.7Wind
    case EN_AS5100_SLS_11_07:       // SLS : PE + Earthquake + 0.7Temp
    case EN_AS5100_SLS_12_07_BRK:   // SLS : PE + Earthquake + 0.7Moving
    case EN_AS5100_SLS_12_07_CF:    // SLS : PE + Earthquake + 0.7Moving
        b1stGrp = FALSE;
        Set_CvlAS5100_SLS(enLcomNo, arDLoad, arDwLoad, arEpLoad, arWpLoad, arFpLoad, arBLoad, arCrLoad, arShLoad,arPsLoad, arStlLoad, arSmLoad, arCoLoad, arCSLoad, arMLoad, arBrkLoad , arCfLoad, arWLoad, arTLoad, arTpgLoad, arELoad, arEspLoad, b1stGrp);
        break;
    case EN_AS5100_Transfer_01:
        b1stGrp = FALSE;
        Set_CvlAS5100_Transfer(enLcomNo, arCSLoad);
        break;
    case EN_AS5100_FLS_01: // FLS
        b1stGrp = FALSE;
        Set_CvlAS5100_FLS(enLcomNo, arCSLoad, arMLoad);
        break;
    }
}

void CLoadCombCtrl::Set_CvlAS5100_ULS(const EN_AS5100_NO& enLcomNo, ArINT& arDLoad, ArINT& arDwLoad, ArINT& arEpLoad, ArINT& arWpLoad, 
                                      ArINT& arFpLoad, ArINT& arBLoad, ArINT& arCrLoad, ArINT& arShLoad, ArINT& arPsLoad, ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arCoLoad, 
                                      ArINT& arCSLoad, ArUNT& arMLoad, ArINT& arBrkLoad , ArINT& arCfLoad, ArINT& arWLoad, ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arELoad, ArUNT& arEspLoad, BOOL b1stGrp)
{
    // ULS A: Minimum Strength and Stability - D + DW + EP + WP
    // ULS B: PE + Moving
    // ULS B: PE + Moving + Temp
    // ULS B: PE + Moving + Wind
    // ULS E: PE + CO
    // ULS E: PE + CO + Moving
    // ULS H: PE + Earthquake
    // ULS J: PE + Wind
    // ULS J: PE + Wind + Temp
    // ULS K: PE + Temp
    // ULS K: PE + Temp + Moving

    CString strTitle;
    strTitle = Add_CvlAS5100_Title(enLcomNo);

    double dCRSHFactor = 0, dPsFactor = 0, dStlFactor = 0, dWFBFactor = 0;
    double dMFactor = 0, dBCFactor = 0, dTFactor = 0, dEFactor = 0, dWFactor = 0, dCoFactor = 0;

    CString sDFac[2], sDwFac[2], sEpFac[2], sWpFac[2];
    double  dDFac[2], dDwFac[2], dEpFac[2], dWpFac[2];
    GetAS_5100_2017Factor(sDFac[0], sDFac[1], sDwFac[0], sDwFac[1], sEpFac[0], sEpFac[1],  sWpFac[0],  sWpFac[1], 
        dDFac[0], dDFac[1], dDwFac[0], dDwFac[1], dEpFac[0], dEpFac[1], dWpFac[0],  dWpFac[1], b1stGrp); 

    if (enLcomNo == EN_AS5100_ULS_01)
    {
        dDFac[0] = 1.35;
        dDFac[1]  = 0.9;
        dDwFac[0] = 1.35;
        dDwFac[1] = 0.9;
        dEpFac[0] = 1.35;
        dEpFac[1] = 0.9;
        dWpFac[0] = 1.0;
        dWpFac[1] = 1.0;
    }

    Set_CvlAS5100_LoadFactorPerm(enLcomNo, dPsFactor, dCRSHFactor, dStlFactor, dWFBFactor);

    GetAS5100TransientFactor(enLcomNo, dMFactor, dBCFactor, dTFactor, dWFactor, dEFactor, dCoFactor);

    int nMCount = max(m_nMCount, 1);
    int nWCountTemp = (m_nWCount>0) ? m_nWCount*2 : 1;
    int nSmCountTemp   = max(m_nSmCount, 1);
    int nTCountTemp = max(m_nTCount, 1); // J
    int nTpgCountTemp = max(m_nTpgCount, 1); // J
    int nECount = max(max(m_nECount, m_nEspCount), 1); //두게 지진하중 동시에 넣는 사용자가 있으면 그건 사용자 잘못.
    int LoadCaseNo;
    T_LCOM_D rData;

    for(int nD    = 0; nD  < m_nAS5100DNum;  nD++)  
    {
        for(int nDw  = 0; nDw  < m_nAS5100DWNum;  nDw++)  
        {
            for(int nEp   = 0; nEp  < m_nAS5100SLNum; nEp++)  
            {
                for(int nWp   = 0; nWp  < m_nAS5100GLNum; nWp++)  
                {
                    for(int nMove = 0; nMove < nMCount; nMove++) 
                    {
                        for(int nSettle = 0; nSettle < nSmCountTemp; nSettle++) 
                        {
                            for (int nWind = 0; nWind < nWCountTemp; nWind++) 
                            {
                                for(int nTemp = 0; nTemp < nTCountTemp; nTemp++) 
                                {
                                    for(int nTempg = 0; nTempg < nTpgCountTemp; nTempg++)
                                    {
                                        for ( int nEarth = 0; nEarth < nECount; ++nEarth )
                                        {
                                            CString strFixM, strFixW, strFixT, strFixTpg, strFixSm, csPrefix, strFixE  = _T("");

                                            rData.Initialize();
                                            LoadCaseNo = 0;
                                            strFixE.Format(_T("%d"), nEarth+1);
                                            strFixM.Format(_T("%d"), nMove+1);
                                            strFixT.Format(_T("%d"), nTemp+1);
                                            strFixTpg.Format(_T("%d"), nTempg+1);
                                            strFixSm.Format(_T("%d"), nSettle+1);
                                            strFixW.Format(_T("%d"), nWind/2+1);

                                            double dWF = (nWind%2==0) ? dWFactor : -1.*dWFactor;

                                            _LCOM_BOTH BothD; BothD.Initialize();
                                            int i=0, nDL=0, nDwL=0, nEpL=0, nWpL=0;
                                            int nCoL=0, nPsL=0, nCrL=0, nShL=0, nStL=0, nFpL=0, nBL=0, nWL=0, nCfL = 0, nBrkL = 0, nTL=0, nTpgL=0, nEL =0, nEsp=0, nn=0;

                                            SetAS5100IncludeCSBasic(enLcomNo, arDLoad, arDwLoad, arEpLoad, arWpLoad, dDFac[nD], dDwFac[nDw], dEpFac[nEp], dWpFac[nWp],
                                                nD, nDw, nEp, nWp, nDL, nDwL, nEpL, nWpL, BothD.nBoth[LoadCaseNo], LoadCaseNo, rData);

                                            SetAS5100IncludeCSPerm(enLcomNo, arFpLoad, arBLoad, arCrLoad, arShLoad, arPsLoad, arStlLoad, arSmLoad, arCoLoad,
                                                dPsFactor, dCRSHFactor, dStlFactor, dWFBFactor, dCoFactor, nPsL, nCrL, nShL, nStL, nFpL, nBL, nSettle, nCoL, LoadCaseNo, rData);

                                            T_MVLD_D rDataMv;
                                            if(nMove < arMLoad.GetSize())
                                            {
                                                m_pDoc->m_pAttrCtrl->GetMvld(arMLoad.GetAt(nMove), rDataMv);
                                            }
                                            else
                                            {
                                                rDataMv.Initialize();
											}

                                            T_MVCD_D rDataMvCode;
                                            m_pDoc->m_pAttrCtrl->GetMvcd(rDataMvCode);

                                            if ( rDataMvCode.nCodeType == D_MOVE_CODE_AUSTRALIA && rDataMv.nLoadModel == 2 )
                                            {
                                                dMFactor = 1.0;
                                            }


                                            SetAS5100IncludeCSMoving(enLcomNo, nMove, dMFactor, dBCFactor, dCoFactor, arCoLoad, arMLoad, arBrkLoad, arCfLoad, nCoL, nBrkL, nCfL, LoadCaseNo, rData);

                                            SetAS5100IncludeCSTemp(enLcomNo, nTemp, nTempg, dTFactor, arTLoad, arTpgLoad, nTL, nTpgL, LoadCaseNo, rData);

                                            SetAS5100IncludeCSWind(enLcomNo, nWind, dWF, arWLoad, nWL, LoadCaseNo, rData);

                                            SetAS5100IncludeCSEarth(enLcomNo, dEFactor, arELoad, arEspLoad, nEL, nEsp, LoadCaseNo, nEarth, rData);

                                            if ( m_nConstLoad!=0 )
                                            {
                                                if ( CheckCS_SgldK(D_SGLD_DL_CVL, nn) )
                                                {
                                                    Set_CStageCase(arCSLoad.GetAt(nn), dDFac[nD], LoadCaseNo, rData, TRUE);
                                                    (nD==1)    ? BothD.nBoth[LoadCaseNo]=1 : BothD.nBoth[LoadCaseNo] = 0;
                                                }

                                                CString strcEL;
                                                int nErection = 0;

                                                for ( int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec )
                                                {
                                                    strcEL = _T(""); nErection = 0;
                                                    SetAS5100CSStagePerm(enLcomNo, nErec, nErection, dDFac[nD], dDwFac[nDw], nn, nD, nDw, arCSLoad, BothD.nBoth[LoadCaseNo], LoadCaseNo, strcEL, rData);
                                                    SetAS5100CSStageTransient(enLcomNo, arCSLoad, nErection, dTFactor, dWFactor, nn, LoadCaseNo, rData);
                                                }
                                                if ( enLcomNo > EN_AS5100_ULS_01 )
                                                {
                                                    SetAS5100CSStagePSCRSHE(enLcomNo, arCSLoad, dPsFactor, dCRSHFactor, nn, LoadCaseNo, rData);
                                                }
                                            }
                                            rData.Description = strTitle;

                                            GetAS5100FactorStrBasicULS(enLcomNo, nDL, nDwL, nEpL, nWpL, nPsL, nCrL, nShL, nStL, nFpL, nBL, nSettle, dDFac[nD], dDwFac[nDw], dEpFac[nEp], dWpFac[nWp],
                                                dPsFactor, dCRSHFactor, dStlFactor, dWFBFactor, strFixSm, rData.Description, csPrefix);

                                            GetAS5100FactorStrLive(enLcomNo, m_nMCount, nBrkL, nCfL, nTL, nTpgL, nWL, nEL, nEsp, nCoL, dCoFactor, dMFactor, dBCFactor, dTFactor, dWF, dEFactor,
                                                strFixE, strFixM, strFixW, strFixT, strFixTpg, rData.Description, csPrefix);

                                            if ( m_nConstLoad!=0 )
                                            {
                                                if ( CheckCS_SgldK(1, nn) )
                                                {
                                                    rData.Description += csPrefix + GetFactorString(dDFac[nD]) + _T("(cD)"); csPrefix = _T("+");
                                                }

                                                CString strcEL;
                                                int nErection = 0;

                                                for ( int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec )
                                                {
                                                    GetAS5100FactorStrPermErec(nErec, nErection, dDFac[nD], dDwFac[nDw], nn, strcEL, rData.Description, csPrefix); //공통으로 있는 D, DW, 부분을 하나의 함수로 묶음
                                                    GetAS5100FactorStrLiveErec(enLcomNo, nErec, nErection, dWFactor, dTFactor, strcEL, rData.Description, csPrefix);
                                                }
                                                if ( enLcomNo > EN_AS5100_ULS_01 )
                                                {
                                                    GetAS5100FactorStrPSCRSHErec(dPsFactor, dCRSHFactor, b1stGrp, nn, rData.Description, csPrefix); //공통으로 있는 PS, CR, SH 부분을 하나의 함수로 묶음
                                                }
                                            }
                                            Check_LoadCombULS(LoadCaseNo, m_nLcomType, rData, BothD);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


void CLoadCombCtrl::Set_CvlAS5100_SLS(const EN_AS5100_NO& enLcomNo, ArINT& arDLoad, ArINT& arDwLoad, ArINT& arEpLoad, ArINT& arWpLoad, 
                                             ArINT& arFpLoad, ArINT& arBLoad, ArINT& arCrLoad, ArINT& arShLoad, ArINT& arPsLoad, ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arCoLoad, 
                                             ArINT& arCSLoad, ArUNT& arMLoad, ArINT& arBrkLoad , ArINT& arCfLoad, ArINT& arWLoad, ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arELoad, ArUNT& arEspLoad, BOOL b1stGrp)
{
    CString strTitle;
    strTitle = Add_CvlAS5100_Title(enLcomNo);

    double dCRSHFactor = 0, dPsFactor = 0, dStlFactor = 0, dWFBFactor = 0;
    double dMFactor = 0, dBCFactor = 0, dTFactor = 0, dEFactor = 0, dWFactor = 0, dCoFactor = 0;

    CString sDFac[2], sDwFac[2], sEpFac[2], sWpFac[2];
    double  dDFac[2], dDwFac[2], dEpFac[2], dWpFac[2];
    GetAS_5100_2017Factor(sDFac[0], sDFac[1], sDwFac[0], sDwFac[1], sEpFac[0], sEpFac[1],  sWpFac[0],  sWpFac[1], 
        dDFac[0], dDFac[1], dDwFac[0], dDwFac[1], dEpFac[0], dEpFac[1], dWpFac[0],  dWpFac[1], b1stGrp); 

    Set_CvlAS5100_LoadFactorPerm(enLcomNo, dPsFactor, dCRSHFactor, dStlFactor, dWFBFactor);

    GetAS5100TransientFactor(enLcomNo, dMFactor, dBCFactor, dTFactor, dWFactor, dEFactor, dCoFactor);

    double dDFactor     = dDFac[0];  //[0] 이나 [1] 값 같음
    double dDwFactor    = dDwFac[0];
    double dEpFactor    = dEpFac[0];
    double dGwFactor    = dWpFac[0];

    int nMCount = max(m_nMCount, 1);
    int nWCountTemp = (m_nWCount>0) ? m_nWCount*2 : 1;
    int nSmCountTemp   = max(m_nSmCount, 1);
    int nTCountTemp = max(m_nTCount, 1); // J
    int nTpgCountTemp = max(m_nTpgCount, 1); // J
    int nECount = max(max(m_nECount, m_nEspCount), 1); //두게 지진하중 동시에 넣는 사용자가 있으면 그건 사용자 잘못.

    int LoadCaseNo;
    T_LCOM_D rData;

    int nBoth = 0;
    int nD = 0; 
    int nDw= 0; 
    int nEp= 0; 
    int nWp= 0; 

    for(int nMove = 0; nMove < nMCount; nMove++) 
    {
        for(int nSettle = 0; nSettle < nSmCountTemp; nSettle++) 
        {
            for (int nWind = 0; nWind < nWCountTemp; nWind++) 
            {
                for(int nTemp = 0; nTemp < nTCountTemp; nTemp++) 
                {
                    for(int nTempg = 0; nTempg < nTpgCountTemp; nTempg++)
                    {
                        for ( int nEarth = 0; nEarth < nECount; ++nEarth )
                        {
                            CString strFixE, strFixM, strFixW, strFixT, strFixTpg, strFixSm, csPrefix  = _T("");

                            rData.Initialize();
                            LoadCaseNo = 0;
                            strFixM.Format(_T("%d"), nMove+1);
                            strFixE.Format(_T("%d"), nEarth+1);
                            strFixT.Format(_T("%d"), nTemp+1);
                            strFixTpg.Format(_T("%d"), nTempg+1);
                            strFixSm.Format(_T("%d"), nSettle+1);
                            strFixW.Format(_T("%d"), nWind/2+1);

                            _LCOM_BOTH BothD; BothD.Initialize();
                            int i=0, nDL=0, nDwL=0, nEpL=0, nWpL=0;
                            int nCoL=0, nPsL=0, nCrL=0, nShL=0, nStL=0, nFpL=0, nBL=0, nWL=0, nCfL = 0, nBrkL = 0, nTL=0, nTpgL=0, nEL =0, nn=0, nEsp=0;

                            SetAS5100IncludeCSBasic(enLcomNo, arDLoad, arDwLoad, arEpLoad, arWpLoad, dDFactor, dDwFactor, dEpFactor, dGwFactor,
                                nD, nDw, nEp, nWp, nDL, nDwL, nEpL, nWpL, nBoth, LoadCaseNo, rData);

                            SetAS5100IncludeCSPerm(enLcomNo, arFpLoad, arBLoad, arCrLoad, arShLoad, arPsLoad, arStlLoad, arSmLoad, arCoLoad,
                                dPsFactor, dCRSHFactor, dStlFactor, dWFBFactor, dCoFactor, nPsL, nCrL, nShL, nStL, nFpL, nBL, nSettle, nCoL, LoadCaseNo, rData);

                            double dWF = (nWind%2==0) ? dWFactor : -1.*dWFactor;

                            SetAS5100IncludeCSMoving(enLcomNo, nMove, dMFactor, dBCFactor, dCoFactor, arCoLoad, arMLoad, arBrkLoad, arCfLoad, nCoL, nBrkL, nCfL, LoadCaseNo, rData);

                            SetAS5100IncludeCSTemp(enLcomNo, nTemp, nTempg, dTFactor, arTLoad, arTpgLoad, nTL, nTpgL, LoadCaseNo, rData);

                            SetAS5100IncludeCSWind(enLcomNo, nWind, dWF, arWLoad, nWL, LoadCaseNo, rData);

                            SetAS5100IncludeCSEarth(enLcomNo, dEFactor, arELoad, arEspLoad, nEL, nEsp, LoadCaseNo, nEarth, rData);

                            if ( m_nConstLoad!=0 )
                            {
                                if ( CheckCS_SgldK(D_SGLD_DL_CVL, nn) )
                                {
                                    Set_CStageCase(arCSLoad.GetAt(nn), dDFactor, LoadCaseNo, rData, TRUE);
                                }

                                CString strcEL;
                                int nErection = 0;

                                for ( int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec )
                                {
                                    strcEL = _T(""); nErection = 0;
                                    SetAS5100CSStagePerm(enLcomNo, nErec, nErection, dDFactor, dDwFactor, nn, nD, nDw, arCSLoad, BothD.nBoth[LoadCaseNo], LoadCaseNo, strcEL, rData);
                                    SetAS5100CSStageTransient(enLcomNo, arCSLoad, nErection, dTFactor, dWFactor, nn, LoadCaseNo, rData);
                                }
                                SetAS5100CSStagePSCRSHE(enLcomNo, arCSLoad, dPsFactor, dCRSHFactor, nn, LoadCaseNo, rData);
                            }
                            rData.Description = strTitle;

                            if ( nDL>0 || nDwL>0 || nEpL>0 || nWpL>0 || nPsL>0 || nCrL>0 || nShL>0 || nStL>0 || nFpL>0 || nBL>0 )
                            {
                                rData.Description += csPrefix + GetFactorString(dDFactor)      + _T("PE");   csPrefix = _T("+");
                            }

                            GetAS5100FactorStrLive(enLcomNo, m_nMCount, nBrkL, nCfL, nTL, nTpgL, nWL, nEL, nEsp, nCoL, dCoFactor, dMFactor, dBCFactor, dTFactor, dWF, dEFactor,
                                strFixE, strFixM, strFixW, strFixT, strFixTpg, rData.Description, csPrefix);

                            if ( m_nConstLoad!=0 )
                            {
                                CString strcEL;
                                int nErection = 0;

                                for ( int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec )
                                {
                                    GetAS5100FactorStrPermErec(nErec, nErection, dDFactor, dDwFactor, nn, strcEL, rData.Description, csPrefix); //공통으로 있는 D, DW, 부분을 하나의 함수로 묶음
                                    GetAS5100FactorStrLiveErec(enLcomNo, nErec, nErection, dWFactor, dTFactor, strcEL, rData.Description, csPrefix);
                                }
                                GetAS5100FactorStrPSCRSHErec(dPsFactor, dCRSHFactor, b1stGrp, nn, rData.Description, csPrefix); //공통으로 있는 PS, CR, SH 부분을 하나의 함수로 묶음
                            }

                            Check_LoadCombSLS(LoadCaseNo, m_nLcomType, rData, BothD);
                        }
                    }
                }
            }
        }
    }
}

void CLoadCombCtrl::Set_CvlAS5100_Transfer(const EN_AS5100_NO& enLcomNo, ArINT& arCSLoad)
{
    if ( arCSLoad.GetSize() ==  0 ) return;

    CString strTitle = Add_CvlAS5100_Title(enLcomNo);

    T_LCOM_D LcomD;
    _LCOM_BOTH BothD; BothD.Initialize();
    int LoadCaseNo = 0;

    if ( m_nConstLoad!=0 )
    {
        LcomD.Description = strTitle;
        CArray<int, int> arTransferLCase;
        arTransferLCase.Add(D_SGLD_DL_CVL);
		if ( m_nLcomType != D_LCOMTYPE_STLCOMP )
		{
			arTransferLCase.Add(D_SGLD_TP_CVL);
			arTransferLCase.Add(D_SGLD_TS_CVL);
		}
        CStringArray arLcName;
        arLcName.Add(_T("(cDL)"));
        arLcName.Add(_T("(cTP)"));
        arLcName.Add(_T("(cTS)"));

        CString csPrefix = _T("");
        for (int i=0; i<arTransferLCase.GetSize(); ++i )
        {
            int nn = 0;
            double dFactor = 1.0;
            if ( CheckCS_SgldK(arTransferLCase[i], nn) )
            {
                Set_CStageCase(arCSLoad.GetAt(nn), dFactor, LoadCaseNo, LcomD, TRUE);
                
                CString strLcase = csPrefix + GetFactorString(dFactor) + arLcName[i]; csPrefix = _T("+");
                LcomD.Description += strLcase;
            }
        }
    }

    if ( m_nLcomType == D_LCOMTYPE_GENERAL ) LcomD.nActive = 1;
    else                                     LcomD.nActive = 2;

    LcomD.nServLcomType = 2;

    if ( !ExistInNewLcom(LcomD) )
    {
        Set_LoadComb4Civil(LcomD, D_SER);
    }
}

void CLoadCombCtrl::Set_CvlAS5100_FLS(const EN_AS5100_NO& enLcomNo, ArINT& arCSLoad, ArUNT& arMLoad) // Fatigue Limit State Design 0.7 * Moving Load * (1 + alpha)
{
    if(m_nMCount  == 0) return;

    double dMFactor = 1.0;

    CString strTitle;
    strTitle = Add_CvlAS5100_Title(enLcomNo);

    int nMCount = max(m_nMCount, 1);

    int nLcomCount = 0;

    int LoadCaseNo;
    T_LCOM_D rData;

    for(int nMove = 0; nMove < nMCount; nMove++)
    {
        CString strFixM, csPrefix  = _T("");

        rData.Initialize();  
        LoadCaseNo = 0;
        strFixM.Format(_T("%d"), nMove+1);

        _LCOM_BOTH BothD; BothD.Initialize();

        int nn = 0;

        BOOL bMoving = FALSE;
        if(m_nMCount>0) bMoving = Exist_MovingLoadData(arMLoad.GetAt(nMove));
        if(bMoving) Set_MovingCase(arMLoad.GetAt(nMove), dMFactor, LoadCaseNo, rData, TRUE);

        rData.Description = strTitle;
        if(m_nMCount>0) {rData.Description += csPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));}

        if(m_nConstLoad!=0)
        {
            CString strcEL;
            int nErection = 0;

            for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
            {
                strcEL = _T(""); nErection = 0;
                if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }
            }
        }
        Check_LoadCombSLS(LoadCaseNo, m_nLcomType, rData, BothD);
    }
}

CString CLoadCombCtrl::Add_CvlAS5100_Title (const EN_AS5100_NO& enLcomNo) // 하중조합 생성시 Title 더하기
{

    CString csTitle(_T(""));

    switch (enLcomNo)
    {
    case EN_AS5100_ULS_01:
        csTitle = _T("ULS A: ");
        break;
    case EN_AS5100_ULS_02_BRK:
    case EN_AS5100_ULS_02_CF:
    case EN_AS5100_ULS_03_BRK:
    case EN_AS5100_ULS_03_CF:
    case EN_AS5100_ULS_04_BRK:
    case EN_AS5100_ULS_04_CF:
        if (m_nASBridgeType == 0)
        {
            csTitle = _T("ULS B: ");
        }
        else
        {
            csTitle = _T("ULS C: ");
        }
        break;
    case EN_AS5100_ULS_05:      
    case EN_AS5100_ULS_06:      
        csTitle = _T("ULS E: ");
        break;
    case EN_AS5100_ULS_07:      
        csTitle = _T("ULS H: ");
        break;
    case EN_AS5100_ULS_08:      
    case EN_AS5100_ULS_09:      
        csTitle = _T("ULS J: ");
        break;
    case EN_AS5100_ULS_10:      
    case EN_AS5100_ULS_11:      
        csTitle = _T("ULS K: ");
        break;
    case EN_AS5100_SLS_01_BRK:    
    case EN_AS5100_SLS_01_CF:       
    case EN_AS5100_SLS_02_BRK:    
    case EN_AS5100_SLS_02_CF:       
    case EN_AS5100_SLS_03_BRK:      
    case EN_AS5100_SLS_03_CF:    
    case EN_AS5100_SLS_01_07_BRK:   // SLS : PE + Moving + 0.7Wind
    case EN_AS5100_SLS_01_07_CF :   // SLS : PE + Moving + 0.7Wind
    case EN_AS5100_SLS_02_07_BRK:   // SLS : PE + Moving + 0.7Temp
    case EN_AS5100_SLS_02_07_CF:    // SLS : PE + Moving + 0.7Temp
    case EN_AS5100_SLS_03_07_BRK:   // SLS : PE + Moving + 0.7Earthquake
    case EN_AS5100_SLS_03_07_CF:    // SLS : PE + Moving + 0.7Earthquake
        csTitle = _T("SLS Moving: ");
        break;
    case EN_AS5100_SLS_04:      
    case EN_AS5100_SLS_05_BRK:     
    case EN_AS5100_SLS_05_CF:      
    case EN_AS5100_SLS_06_BRK:     
    case EN_AS5100_SLS_06_CF:    
    case EN_AS5100_SLS_04_07:       // SLS : PE + Temp + 0.7Wind
    case EN_AS5100_SLS_05_07_BRK:   // SLS : PE + Temp + 0.7Moving      
    case EN_AS5100_SLS_05_07_CF:    // SLS : PE + Temp + 0.7Moving      
    case EN_AS5100_SLS_06_07:       // SLS : PE + Temp + 0.7Earthquake
        csTitle = _T("SLS Temp: ");
        break;
    case EN_AS5100_SLS_07:      
    case EN_AS5100_SLS_08_BRK:   
    case EN_AS5100_SLS_08_CF:      
    case EN_AS5100_SLS_09_BRK:      
    case EN_AS5100_SLS_09_CF:      
    case EN_AS5100_SLS_07_07:       // SLS : PE + Wind + 0.7Earthquake
    case EN_AS5100_SLS_08_07:       // SLS : PE + Wind + 0.7Temp         
    case EN_AS5100_SLS_09_07_BRK:   // SLS : PE + Wind + 0.7Moving   
    case EN_AS5100_SLS_09_07_CF:    // SLS : PE + Wind + 0.7Moving
        csTitle = _T("SLS Wind: ");
        break;
    case EN_AS5100_SLS_10:      
    case EN_AS5100_SLS_11_BRK:      
    case EN_AS5100_SLS_12_BRK:      
    case EN_AS5100_SLS_11_CF:      
    case EN_AS5100_SLS_12_CF:      
    case EN_AS5100_SLS_10_07:       // SLS : PE + Earthquake + 0.7Wind
    case EN_AS5100_SLS_11_07:       // SLS : PE + Earthquake + 0.7Temp
    case EN_AS5100_SLS_12_07_BRK:   // SLS : PE + Earthquake + 0.7Moving
    case EN_AS5100_SLS_12_07_CF:    // SLS : PE + Earthquake + 0.7Moving
        csTitle = _T("SLS Earthquake: ");
        break;
    case EN_AS5100_Transfer_01:
        csTitle = _T("Transfer : ");
        break;
    case EN_AS5100_FLS_01:      
        csTitle = _T("FLS : ");
        break;
    default:
        ASSERT(0);
        break;
    }
    return csTitle;
}

void CLoadCombCtrl::GetAS5100FactorStrPermErec(int& nErec, int& nErection, double& dDFac, double& dDwFac, int& nn, CString& strcEL, CString& sDescription, CString& sPrefix)
{
    strcEL = _T(""); nErection = 0;
    if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }
    if(CheckCSEL(nErection, _T("D")))
    {sDescription += sPrefix + GetFactorString(dDFac) + strcEL; sPrefix = _T("+");}
    if(CheckCSEL(nErection, _T("DW")))
    {sDescription += sPrefix + GetFactorString(dDwFac) + strcEL; sPrefix = _T("+");}
}

void CLoadCombCtrl::GetAS5100FactorStrPSCRSHErec(double& dPsFactor, double& dCRSHFactor, BOOL& b1stGrp, int& nn, CString& sDescription, CString& sPrefix)
{
    if (!b1stGrp)
    {
		if ( m_nLcomType != D_LCOMTYPE_STLCOMP )
		{
			if ( CheckCS_SgldK(D_SGLD_TP_CVL, nn) )
			{sDescription += sPrefix + GetFactorString(dPsFactor) + _T("(cTP)"); sPrefix = _T("+");}
		}
    }
	if ( CheckCS_SgldK(D_SGLD_TP_CVL, nn) )
	{
		if(CheckCS_SgldK(D_SGLD_TS_CVL, nn))
	    {sDescription += sPrefix + GetFactorString(dPsFactor)   + _T("(cTs)"); sPrefix = _T("+");}
	}
    if(CheckCS_SgldK(D_SGLD_CS_CVL, nn))
    {sDescription += sPrefix + GetFactorString(dCRSHFactor) + _T("(cCR)"); sPrefix = _T("+");}
    if(CheckCS_SgldK(D_SGLD_SS_CVL, nn))
    {sDescription += sPrefix + GetFactorString(dCRSHFactor) + _T("(cSH)"); sPrefix = _T("+");}
}

void CLoadCombCtrl::GetAS5100FactorStrLiveErec(const EN_AS5100_NO& enLcomNo, int& nErec, int& nErection, double& dWFactor, double& dTFactor, CString& strcEL, CString& sDescription, CString& sPrefix)
{
    switch (enLcomNo)
    {
    case EN_AS5100_ULS_01:// ULS A: Minimum Strength and Stability - D + DW + EP + WP
    case EN_AS5100_ULS_02_BRK:  // ULS B: PE + Moving
    case EN_AS5100_ULS_02_CF:   // ULS B: PE + Moving
    case EN_AS5100_ULS_05:      // ULS E: PE + CO
    case EN_AS5100_ULS_06:      // ULS E: PE + CO + Moving
    case EN_AS5100_ULS_07:      // ULS H: PE + Earthquake
        break;
    case EN_AS5100_ULS_03_BRK:  // ULS B: PE + Moving + Temp
    case EN_AS5100_ULS_03_CF:   // ULS B: PE + Moving + Temp
    case EN_AS5100_ULS_10:      // ULS K: PE + Temp
    case EN_AS5100_ULS_11:      // ULS K: PE + Temp + Moving
        if(CheckCSEL(nErection, _T("T")))
        {sDescription += sPrefix + GetFactorString(dTFactor) + strcEL; sPrefix = _T("+");}
        if(CheckCSEL(nErection, _T("TPG")))
        {sDescription += sPrefix + GetFactorString(dTFactor) + strcEL; sPrefix = _T("+");}
        break;
    case EN_AS5100_ULS_04_BRK:  // ULS B: PE + Moving + Wind
    case EN_AS5100_ULS_04_CF:   // ULS B: PE + Moving + Wind
    case EN_AS5100_ULS_08:      // ULS J: PE + Wind
        if(CheckCSEL(nErection, _T("W")))
        {sDescription += sPrefix + GetFactorString(dWFactor) + strcEL; sPrefix = _T("+");}
        break;
    case EN_AS5100_ULS_09:      // ULS J: PE + Wind + Temp
        if(CheckCSEL(nErection, _T("W")))
        {sDescription += sPrefix + GetFactorString(dWFactor) + strcEL; sPrefix = _T("+");}
        if(CheckCSEL(nErection, _T("T")))
        {sDescription += sPrefix + GetFactorString(dTFactor) + strcEL; sPrefix = _T("+");}
        if(CheckCSEL(nErection, _T("TPG")))
        {sDescription += sPrefix + GetFactorString(dTFactor) + strcEL; sPrefix = _T("+");}
        break;

        //SLS
    case EN_AS5100_SLS_03_07_BRK:   // SLS : PE + Moving + 0.7Earthquake
    case EN_AS5100_SLS_03_07_CF:    // SLS : PE + Moving + 0.7Earthquake
    case EN_AS5100_SLS_12_07_BRK:   // SLS : PE + Earthquake + 0.7Moving
    case EN_AS5100_SLS_12_07_CF:    // SLS : PE + Earthquake + 0.7Moving
        break;
    case EN_AS5100_SLS_01_BRK:      // SLS : PE + Moving + 0.5(Wind + Earthquake)
    case EN_AS5100_SLS_01_CF:       // SLS : PE + Moving + 0.5(Wind + Earthquake)
    case EN_AS5100_SLS_01_07_BRK:   // SLS : PE + Moving + 0.7Wind
    case EN_AS5100_SLS_01_07_CF :   // SLS : PE + Moving + 0.7Wind
    case EN_AS5100_SLS_09_BRK:      // SLS : PE + Wind + 0.5(Moving + Earthquake)
    case EN_AS5100_SLS_09_CF:       // SLS : PE + Wind + 0.5(Moving + Earthquake)
    case EN_AS5100_SLS_07_07:       // SLS : PE + Wind + 0.7Earthquake
    case EN_AS5100_SLS_09_07_BRK:   // SLS : PE + Wind + 0.7Moving   
    case EN_AS5100_SLS_09_07_CF:    // SLS : PE + Wind + 0.7Moving
    case EN_AS5100_SLS_12_BRK:      // SLS : PE + Earthquake + 0.5(Moving + Wind)
    case EN_AS5100_SLS_12_CF:       // SLS : PE + Earthquake + 0.5(Moving + Wind)
    case EN_AS5100_SLS_10_07:       // SLS : PE + Earthquake + 0.7Wind
        if(CheckCSEL(nErection, _T("W")))
        {sDescription += sPrefix + GetFactorString(dWFactor) + strcEL; sPrefix = _T("+");}
        break;
    case EN_AS5100_SLS_02_BRK:      // SLS : PE + Moving + 0.5(Earthquake + Temp)
    case EN_AS5100_SLS_02_CF:       // SLS : PE + Moving + 0.5(Earthquake + Temp)
    case EN_AS5100_SLS_02_07_BRK:   // SLS : PE + Moving + 0.7Temp
    case EN_AS5100_SLS_02_07_CF:    // SLS : PE + Moving + 0.7Temp
    case EN_AS5100_SLS_06_BRK:      // SLS : PE + Temp + 0.5(Earthquake + Moving)
    case EN_AS5100_SLS_06_CF:       // SLS : PE + Temp + 0.5(Earthquake + Moving)
    case EN_AS5100_SLS_05_07_BRK:   // SLS : PE + Temp + 0.7Moving      
    case EN_AS5100_SLS_05_07_CF:    // SLS : PE + Temp + 0.7Moving      
    case EN_AS5100_SLS_06_07:       // SLS : PE + Temp + 0.7Earthquake
    case EN_AS5100_SLS_11_BRK:      // SLS : PE + Earthquake + 0.5(Temp + Moving)
    case EN_AS5100_SLS_11_CF:       // SLS : PE + Earthquake + 0.5(Temp + Moving)
    case EN_AS5100_SLS_11_07:       // SLS : PE + Earthquake + 0.7Temp
        if(CheckCSEL(nErection, _T("T")))
        {sDescription += sPrefix + GetFactorString(dTFactor) + strcEL; sPrefix = _T("+");}
        if(CheckCSEL(nErection, _T("TPG")))
        {sDescription += sPrefix + GetFactorString(dTFactor) + strcEL; sPrefix = _T("+");}
        break;
    case EN_AS5100_SLS_03_BRK:  // SLS : PE + Moving + 0.5(Temp + Wind)
    case EN_AS5100_SLS_03_CF:   // SLS : PE + Moving + 0.5(Temp + Wind)
    case EN_AS5100_SLS_04:      // SLS : PE + Temp + 0.5(Wind + Earthquake)
    case EN_AS5100_SLS_05_BRK:  // SLS : PE + Temp + 0.5(Moving + Wind)
    case EN_AS5100_SLS_05_CF:   // SLS : PE + Temp + 0.5(Moving + Wind)
    case EN_AS5100_SLS_04_07:   // SLS : PE + Temp + 0.7Wind
    case EN_AS5100_SLS_07:      // SLS : PE + Wind + 0.5(Earthquake + Temp)
    case EN_AS5100_SLS_08_BRK:  // SLS : PE + Wind + 0.5(Temp + Moving),        
    case EN_AS5100_SLS_08_CF:   // SLS : PE + Wind + 0.5(Temp + Moving),   
    case EN_AS5100_SLS_08_07:   // SLS : PE + Wind + 0.7Temp        
    case EN_AS5100_SLS_10:      // SLS : PE + Earthquake + 0.5(Wind + Temp)
        if(CheckCSEL(nErection, _T("W")))
        {sDescription += sPrefix + GetFactorString(dWFactor) + strcEL; sPrefix = _T("+");}
        if(CheckCSEL(nErection, _T("T")))
        {sDescription += sPrefix + GetFactorString(dTFactor) + strcEL; sPrefix = _T("+");}
        if(CheckCSEL(nErection, _T("TPG")))
        {sDescription += sPrefix + GetFactorString(dTFactor) + strcEL; sPrefix = _T("+");}
        break;
    default:
        ASSERT(0);
        break;
    }
}

void CLoadCombCtrl::GetAS5100FactorStrLive(const EN_AS5100_NO& enLcomNo, int& m_nMCount, int& nBrkL, int& nCfL, int& nTL, int& nTpgL, int& nWL, int& nEL, int& nEsp, int& nCoL,
                                           double& dCoFactor, double& dMFactor, double& dBCFactor, double& dTFactor, double& dWF, double& dEFactor, 
    CString& strFixE, CString& strFixM, CString& strFixW, CString& strFixT, CString& strFixTpg, CString& sDescription, CString& sPrefix)
{
    switch (enLcomNo)
    {
    case EN_AS5100_ULS_01:// ULS A: Minimum Strength and Stability - D + DW + EP + WP
        break;
    case EN_AS5100_ULS_02_BRK:  // ULS B: PE + Moving
        if(m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) 
        {
            if(m_nMCount>0 && nBrkL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("BRK"); sPrefix = _T("+");}
        }
        break;
    case EN_AS5100_ULS_02_CF:   // ULS B: PE + Moving
        if(m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) 
        {
            if(m_nMCount>0 && nCfL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("CF"); sPrefix = _T("+");}
        }
        break;
    case EN_AS5100_ULS_03_BRK:  // ULS B: PE + Moving + Temp
        if(m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0)
        {
            if(m_nMCount>0 && nBrkL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("BRK"); sPrefix = _T("+");}
        }
        if(nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        break;
    case EN_AS5100_ULS_03_CF:   // ULS B: PE + Moving + Temp
        if(m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0)
        {
            if(m_nMCount>0 && nCfL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("CF"); sPrefix = _T("+");}
        }
        if(nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        break;
    case EN_AS5100_ULS_04_BRK:  // ULS B: PE + Moving + Wind
        if(m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) 
        {
            if(m_nMCount>0 && nBrkL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("BRK"); sPrefix = _T("+");}
        }
        if(nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        break;
    case EN_AS5100_ULS_04_CF:   // ULS B: PE + Moving + Wind
        if(m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) 
        {
            if(m_nMCount>0 && nCfL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("CF"); sPrefix = _T("+");}
        }
        if(nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        break;
    case EN_AS5100_ULS_05:      // ULS E: PE + CO
        if(nCoL > 0)    {sDescription += sPrefix + GetFactorString(dCoFactor)		+ _T("CO");  sPrefix = _T("+");} 
        break;
    case EN_AS5100_ULS_06:      // ULS E: PE + CO + Moving
        if(nCoL > 0)		 {sDescription += sPrefix + GetFactorString(dCoFactor)		+ _T("CO");  sPrefix = _T("+");} 
        if(m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        break;
    case EN_AS5100_ULS_07:      // ULS H: PE + Earthquake
        {
            if ( nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
            if ( nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
        }        
        break;
    case EN_AS5100_ULS_08:      // ULS J: PE + Wind
        if(nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        break;
    case EN_AS5100_ULS_09:      // ULS J: PE + Wind + Temp
        if(nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        if(nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        break;
    case EN_AS5100_ULS_10:      // ULS K: PE + Temp
        if(nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        break;
    case EN_AS5100_ULS_11:      // ULS K: PE + Temp + Moving
        if(nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        if(m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        break;
    case EN_AS5100_SLS_01_BRK:  // SLS : PE + Moving + 0.5(Wind + Earthquake)
        if(m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if(m_nMCount>0 && nBrkL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("BRK"); sPrefix = _T("+");}
        }
        if(m_nMCount>0 && nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        if ( m_nMCount>0 && nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
        if ( m_nMCount>0 && nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
        break;
    case EN_AS5100_SLS_01_CF:   // SLS : PE + Moving + 0.5(Wind + Earthquake)
        if(m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if(m_nMCount>0 && nCfL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("CF"); sPrefix = _T("+");}
        }
        if(m_nMCount>0 && nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        if ( m_nMCount>0 && nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
        if ( m_nMCount>0 && nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
        break;
    case EN_AS5100_SLS_02_BRK:  // SLS : PE + Moving + 0.5(Earthquake + Temp)
        if(m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if(m_nMCount>0 && nBrkL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("BRK"); sPrefix = _T("+");}
        }
        if(m_nMCount>0 && nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(m_nMCount>0 && nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        if ( m_nMCount>0 && nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
        if ( m_nMCount>0 && nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
        break;
    case EN_AS5100_SLS_02_CF:   // SLS : PE + Moving + 0.5(Earthquake + Temp)
        if(m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if(m_nMCount>0 && nCfL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("CF"); sPrefix = _T("+");}
        }
        if(m_nMCount>0 && nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(m_nMCount>0 && nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        if ( m_nMCount>0 && nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
        if ( m_nMCount>0 && nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
        break;
    case EN_AS5100_SLS_03_BRK:  // SLS : PE + Moving + 0.5(Temp + Wind)
        if(m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if(m_nMCount>0 && nBrkL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("BRK"); sPrefix = _T("+");}
        }
        if(m_nMCount>0 && nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        if(m_nMCount>0 && nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(m_nMCount>0 && nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        break;
    case EN_AS5100_SLS_03_CF:   // SLS : PE + Moving + 0.5(Temp + Wind)
        if(m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if(m_nMCount>0 && nCfL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("CF"); sPrefix = _T("+");}
        }
        if(m_nMCount>0 && nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        if(m_nMCount>0 && nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(m_nMCount>0 && nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        break;
    case EN_AS5100_SLS_01_07_BRK:   // SLS : PE + Moving + 0.7Wind
        if(m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if(m_nMCount>0 && nBrkL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("BRK"); sPrefix = _T("+");}
        }
        if(m_nMCount>0 && nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        break;
    case EN_AS5100_SLS_01_07_CF :   // SLS : PE + Moving + 0.7Wind
        if(m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if(m_nMCount>0 && nCfL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("CF"); sPrefix = _T("+");}
        }
        if(m_nMCount>0 && nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        break;
    case EN_AS5100_SLS_02_07_BRK:   // SLS : PE + Moving + 0.7Temp
        if(m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if(m_nMCount>0 && nBrkL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("BRK"); sPrefix = _T("+");}
        }
        if(m_nMCount>0 && nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(m_nMCount>0 && nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        break;
    case EN_AS5100_SLS_02_07_CF:    // SLS : PE + Moving + 0.7Temp
        if(m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if(m_nMCount>0 && nCfL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("CF"); sPrefix = _T("+");}
        }
        if(m_nMCount>0 && nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(m_nMCount>0 && nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        break;
    case EN_AS5100_SLS_03_07_BRK:   // SLS : PE + Moving + 0.7Earthquake
        if(m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if(m_nMCount>0 && nBrkL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("BRK"); sPrefix = _T("+");}
        }
        if ( m_nMCount>0 && nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
        if ( m_nMCount>0 && nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
        break;
    case EN_AS5100_SLS_03_07_CF:    // SLS : PE + Moving + 0.7Earthquake
        if(m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if(m_nMCount>0 && nCfL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("CF"); sPrefix = _T("+");}
        }
        if ( m_nMCount>0 && nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
        if ( m_nMCount>0 && nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
        break;

    case EN_AS5100_SLS_04:      // SLS : PE + Temp + 0.5(Wind + Earthquake)
        if(nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        if((nTL  > 0 || nTpgL > 0) && nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        if ( (nTL  > 0 || nTpgL > 0) && nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
        if ( (nTL  > 0 || nTpgL > 0) && nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
        break;
    case EN_AS5100_SLS_05_BRK:  // SLS : PE + Temp + 0.5(Moving + Wind)
        if(nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        if((nTL  > 0 || nTpgL > 0) && m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if((nTL  > 0 || nTpgL > 0) && m_nMCount>0 && nBrkL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("BRK"); sPrefix = _T("+");}
        }
        if((nTL  > 0 || nTpgL > 0) && nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        break;
    case EN_AS5100_SLS_05_CF:   // SLS : PE + Temp + 0.5(Moving + Wind)
        if(nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        if((nTL  > 0 || nTpgL > 0) && m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if((nTL  > 0 || nTpgL > 0) && m_nMCount>0 && nCfL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("CF"); sPrefix = _T("+");}
        }
        if((nTL  > 0 || nTpgL > 0) && nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        break;
    case EN_AS5100_SLS_06_BRK:  // SLS : PE + Temp + 0.5(Earthquake + Moving)
        if(nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        if((nTL  > 0 || nTpgL > 0) && m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if((nTL  > 0 || nTpgL > 0) && m_nMCount>0 && nBrkL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("BRK"); sPrefix = _T("+");}
        }
        if ( (nTL  > 0 || nTpgL > 0) && nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
        if ( (nTL  > 0 || nTpgL > 0) && nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
        break;
    case EN_AS5100_SLS_06_CF:   // SLS : PE + Temp + 0.5(Earthquake + Moving)
        if(nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        if((nTL  > 0 || nTpgL > 0) && m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if((nTL  > 0 || nTpgL > 0) && m_nMCount>0 && nCfL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("CF"); sPrefix = _T("+");}
        }
        if ( (nTL  > 0 || nTpgL > 0) && nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
        if ( (nTL  > 0 || nTpgL > 0) && nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
        break;
    case EN_AS5100_SLS_04_07:       // SLS : PE + Temp + 0.7Wind
        if(nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        if((nTL  > 0 || nTpgL > 0) && nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        break;
    case EN_AS5100_SLS_05_07_BRK:   // SLS : PE + Temp + 0.7Moving   
        if(nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        if((nTL  > 0 || nTpgL > 0) && m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if((nTL  > 0 || nTpgL > 0) && m_nMCount>0 && nBrkL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("BRK"); sPrefix = _T("+");}
        }
        break;
    case EN_AS5100_SLS_05_07_CF:    // SLS : PE + Temp + 0.7Moving     
        if(nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        if((nTL  > 0 || nTpgL > 0) && m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if((nTL  > 0 || nTpgL > 0) && m_nMCount>0 && nCfL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("CF"); sPrefix = _T("+");}
        }
        break;
    case EN_AS5100_SLS_06_07:       // SLS : PE + Temp + 0.7Earthquake
        if(nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        if ( (nTL  > 0 || nTpgL > 0) && nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
        if ( (nTL  > 0 || nTpgL > 0) && nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
        break;

    case EN_AS5100_SLS_07:      // SLS : PE + Wind + 0.5(Earthquake + Temp)
        if(nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        if ( nWL  > 0 && nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
        if ( nWL  > 0 && nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
        if(nWL  > 0 && nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(nWL  > 0 && nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        break;
    case EN_AS5100_SLS_08_BRK:  // SLS : PE + Wind + 0.5(Temp + Moving),    
        if(nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        if(nWL  > 0 && m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if(nWL  > 0 && m_nMCount>0 && nBrkL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("BRK"); sPrefix = _T("+");}
        }
        if(nWL  > 0 && nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(nWL  > 0 && nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        break;
    case EN_AS5100_SLS_08_CF:   // SLS : PE + Wind + 0.5(Temp + Moving),   
        if(nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        if(nWL  > 0 && m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if(nWL  > 0 && m_nMCount>0 && nCfL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("CF"); sPrefix = _T("+");}
        }
        if(nWL  > 0 && nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(nWL  > 0 && nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        break;
    case EN_AS5100_SLS_09_BRK:  // SLS : PE + Wind + 0.5(Moving + Earthquake)
        if(nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        if(nWL  > 0 && m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if(nWL  > 0 && m_nMCount>0 && nBrkL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("BRK"); sPrefix = _T("+");}
        }
        if ( nWL  > 0 && nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
        if ( nWL  > 0 && nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
        break;
    case EN_AS5100_SLS_09_CF:   // SLS : PE + Wind + 0.5(Moving + Earthquake)
        if(nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        if(nWL  > 0 && m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if(nWL  > 0 && m_nMCount>0 && nCfL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("CF"); sPrefix = _T("+");}
        }
        if ( nWL  > 0 && nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
        if ( nWL  > 0 && nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
        break;
    case EN_AS5100_SLS_07_07:       // SLS : PE + Wind + 0.7Earthquake
        if(nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        if ( nWL  > 0 && nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
        if ( nWL  > 0 && nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
        break;
    case EN_AS5100_SLS_08_07:       // SLS : PE + Wind + 0.7Temp         
        if(nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        if(nWL  > 0 && nTL  > 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+");}
        if(nWL  > 0 && nTpgL> 0) {sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+");}
        break;
    case EN_AS5100_SLS_09_07_BRK:   // SLS : PE + Wind + 0.7Moving   
        if(nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        if(nWL  > 0 && m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if(nWL  > 0 && m_nMCount>0 && nBrkL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("BRK"); sPrefix = _T("+");}
        }
        break;
    case EN_AS5100_SLS_09_07_CF:    // SLS : PE + Wind + 0.7Moving
        if(nWL  > 0) {if(dWF <0.0) sPrefix=_T(""); sDescription += sPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); sPrefix = _T("+");}
        if(nWL  > 0 && m_nMCount>0) {sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+");}
        if (m_nASBridgeType == 0) //RoadBridge
        {
            if(nWL  > 0 && m_nMCount>0 && nCfL > 0) {sDescription += sPrefix + GetFactorString(dBCFactor) + _T("CF"); sPrefix = _T("+");}
        }
        break;

    case EN_AS5100_SLS_10:      // SLS : PE + Earthquake + 0.5(Wind + Temp)
        {
            int nEarth = max(nEL, nEsp);
            if ( nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
            if ( nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
            if ( nEarth > 0 && nWL  > 0 ) { sDescription += sPrefix + GetFactorString(dWF) + _T("W");   sPrefix = _T("+"); }
            if ( nEarth > 0 && nTL  > 0 ) { sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+"); }
            if ( nEarth > 0 && nTpgL> 0 ) { sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+"); }
        }
        break;
    case EN_AS5100_SLS_11_BRK:  // SLS : PE + Earthquake + 0.5(Temp + Moving)
        {
            int nEarth = max(nEL, nEsp);
            if ( nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
            if ( nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }

            if ( nEarth  > 0 && m_nMCount>0 ) { sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+"); }
            if ( m_nASBridgeType == 0 ) //RoadBridge
            {
                if ( nEarth  > 0 && m_nMCount>0 && nBrkL > 0 ) { sDescription += sPrefix + GetFactorString(dBCFactor) + _T("BRK"); sPrefix = _T("+"); }
            }
            if ( nEarth  > 0 && nTL  > 0 ) { sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+"); }
            if ( nEarth  > 0 && nTpgL> 0 ) { sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+"); }
        }
        break;
    case EN_AS5100_SLS_11_CF:   // SLS : PE + Earthquake + 0.5(Temp + Moving)
        {
            int nEarth = max(nEL, nEsp);
            if ( nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
            if ( nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }

            if ( nEarth  > 0 && m_nMCount>0 ) { sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+"); }
            if ( m_nASBridgeType == 0 ) //RoadBridge
            {
                if ( nEarth  > 0 && m_nMCount>0 && nCfL > 0 ) { sDescription += sPrefix + GetFactorString(dBCFactor) + _T("CF"); sPrefix = _T("+"); }
            }
            if ( nEarth  > 0 && nTL  > 0 ) { sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+"); }
            if ( nEarth  > 0 && nTpgL> 0 ) { sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+"); }
        }
        break;
    case EN_AS5100_SLS_12_BRK:  // SLS : PE + Earthquake + 0.5(Moving + Wind)
        {
            int nEarth = max(nEL, nEsp);
            if ( nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
            if ( nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
            if ( nEarth  > 0 && m_nMCount>0 ) { sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+"); }
            if ( m_nASBridgeType == 0 ) //RoadBridge
            {
                if ( nEarth  > 0 && m_nMCount>0 && nBrkL > 0 ) { sDescription += sPrefix + GetFactorString(dBCFactor) + _T("BRK"); sPrefix = _T("+"); }
            }
            if ( nEarth  > 0 && nWL > 0 ) { sDescription += sPrefix + GetFactorString(dWF)      + _T("W");   sPrefix = _T("+"); }
        }
        break;
    case EN_AS5100_SLS_12_CF:   // SLS : PE + Earthquake + 0.5(Moving + Wind)
        {
            int nEarth = max(nEL, nEsp);
            if ( nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
            if ( nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
            if ( nEarth  > 0 && m_nMCount>0 ) { sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+"); }
            if ( m_nASBridgeType == 0 ) //RoadBridge
            {
                if ( nEarth  > 0 && m_nMCount>0 && nCfL > 0 ) { sDescription += sPrefix + GetFactorString(dBCFactor) + _T("CF"); sPrefix = _T("+"); }
            }
            if ( nEarth  > 0 && nWL > 0 ) { sDescription += sPrefix + GetFactorString(dWF)      + _T("W");   sPrefix = _T("+"); }
        }
        break;

    case EN_AS5100_SLS_10_07:       // SLS : PE + Earthquake + 0.7Wind
        {
            int nEarth = max(nEL, nEsp);
            if ( nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
            if ( nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
            if ( nEarth > 0 && nWL  > 0 ) { sDescription += sPrefix + GetFactorString(dWF) + _T("W");   sPrefix = _T("+"); }
        }
        break;
    case EN_AS5100_SLS_11_07:       // SLS : PE + Earthquake + 0.7Temp
        {
            int nEarth = max(nEL, nEsp);
            if ( nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
            if ( nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
            if ( nEarth  > 0 && nTL  > 0 ) { sDescription += sPrefix + GetFactorString(dTFactor) + _T("T[" + strFixT + _T("]")); sPrefix = _T("+"); }
            if ( nEarth  > 0 && nTpgL> 0 ) { sDescription += sPrefix + GetFactorString(dTFactor) + _T("TPG[" + strFixTpg + _T("]")); sPrefix = _T("+"); }
        }
        break;
    case EN_AS5100_SLS_12_07_BRK:   // SLS : PE + Earthquake + 0.7Moving
        {
            int nEarth = max(nEL, nEsp);
            if ( nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
            if ( nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
            if ( nEarth  > 0 && m_nMCount>0 ) { sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+"); }
            if ( m_nASBridgeType == 0 ) //RoadBridge
            {
                if ( nEarth  > 0 && m_nMCount>0 && nBrkL > 0 ) { sDescription += sPrefix + GetFactorString(dBCFactor) + _T("BRK"); sPrefix = _T("+"); }
            }
        }
        break;
    case EN_AS5100_SLS_12_07_CF:    // SLS : PE + Earthquake + 0.7Moving
        {
            int nEarth = max(nEL, nEsp);
            if ( nEL > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor)			+ _T("E[" + strFixE + _T("]"));  sPrefix = _T("+"); }
            if ( nEsp > 0 ) { sDescription += sPrefix + GetFactorString(dEFactor) + _T("ESP[" + strFixE + _T("]"));   sPrefix = _T("+"); }
            if ( nEarth  > 0 && m_nMCount>0 ) { sDescription += sPrefix + GetFactorString(dMFactor) + _T("M[" + strFixM + _T("]"));  sPrefix = _T("+"); }
            if ( m_nASBridgeType == 0 ) //RoadBridge
            {
                if ( nEarth  > 0 && m_nMCount>0 && nCfL > 0 ) { sDescription += sPrefix + GetFactorString(dBCFactor) + _T("CF"); sPrefix = _T("+"); }
            }
        }
        break;
    default:
        ASSERT(0);
        break;
    }
}


void CLoadCombCtrl::GetAS5100FactorStrBasicULS(const EN_AS5100_NO& enLcomNo, int& nDL, int& nDwL, int& nEpL, int& nWpL, int& nPsL, int& nCrL, int& nShL, int& nStL, int& nFpL, int& nBL, int& nSettle, 
                                               double& dDFactor, double& dDwFactor, double& dEpFactor, double& dGwFactor,  double& dPsFactor, double& dCRSHFactor, double& dStlFactor, double& dWFBFactor,   
                                               CString& strFixSm,  CString& sDescription, CString& sPrefix)
{
    if(nDL  > 0)     {sDescription += sPrefix + GetFactorString(dDFactor)     + _T("D");   sPrefix = _T("+");}
    if(nDwL > 0)     {sDescription += sPrefix + GetFactorString(dDwFactor)    + _T("DW");  sPrefix = _T("+");}
    if(nEpL > 0)     {sDescription += sPrefix + GetFactorString(dEpFactor)    + _T("EP");  sPrefix = _T("+");}
    if(nWpL > 0)     {sDescription += sPrefix + GetFactorString(dGwFactor)    + _T("WP");  sPrefix = _T("+");}

    if (enLcomNo > EN_AS5100_ULS_01)
    {
        if(nPsL > 0)     {sDescription += sPrefix + GetFactorString(dPsFactor)    + _T("PS"); sPrefix = _T("+");}
        if(nCrL > 0)	 {sDescription += sPrefix + GetFactorString(dCRSHFactor)  + _T("CR"); sPrefix = _T("+");}
        if(nShL > 0)	 {sDescription += sPrefix + GetFactorString(dCRSHFactor)  + _T("SH"); sPrefix = _T("+");}
        if(nStL > 0)	 {sDescription += sPrefix + GetFactorString(dStlFactor)   + _T("STL"); sPrefix = _T("+");}
        if(m_nSmCount>0) {sDescription += sPrefix + GetFactorString(dStlFactor)   + _T("SM[" + strFixSm + _T("]")); sPrefix = _T("+");}
        if(nFpL > 0)	 {sDescription += sPrefix + GetFactorString(dWFBFactor)   + _T("FP");  sPrefix = _T("+");} 
        if(nBL > 0)		 {sDescription += sPrefix + GetFactorString(dWFBFactor)	  + _T("B");   sPrefix = _T("+");} 
    }
}


void CLoadCombCtrl::GetAS5100TransientFactor(const EN_AS5100_NO& enLcomNo, double& dMFactor, double& dBCFactor, double& dTFactor, double& dWFactor, double& dEFactor, double& dCoFactor)
{
    int nEcount = max(m_nECount, m_nEspCount);
    switch (enLcomNo)
    {
    case EN_AS5100_ULS_01:// ULS A: Minimum Strength and Stability - D + DW + EP + WP
        break;
    case EN_AS5100_ULS_02_BRK:  // ULS B: PE + Moving
    case EN_AS5100_ULS_02_CF:   // ULS B: PE + Moving
        if (m_nASBridgeType == 0) //RoadBridge
        {
            dMFactor = 1.8;
            dBCFactor = 1.8;
        }
        else //Pedestrian
        {
            dMFactor = 1.5;
        }
        break;
    case EN_AS5100_ULS_03_BRK:  // ULS B: PE + Moving + Temp
    case EN_AS5100_ULS_03_CF:   // ULS B: PE + Moving + Temp
        dTFactor = 1.0;
        if (m_nASBridgeType == 0) //RoadBridge
        {
            dMFactor = 1.8;
            dBCFactor = 1.8;
        }
        else //Pedestrian
        {
            dMFactor = 1.5;
        }
        break;
    case EN_AS5100_ULS_04_BRK:  // ULS B: PE + Moving + Wind
    case EN_AS5100_ULS_04_CF:   // ULS B: PE + Moving + Wind
        dWFactor = 1.0;
        if (m_nASBridgeType == 0) //RoadBridge
        {
            dMFactor = 1.8;
            dBCFactor = 1.8;
        }
        else //Pedestrian
        {
            dMFactor = 1.5;
        }
        break;
    case EN_AS5100_ULS_05:      // ULS E: PE + CO
    case EN_AS5100_ULS_06:      // ULS E: PE + CO + Moving
        dCoFactor   = 1.0;
        dMFactor    = 1.0;
        break;
    case EN_AS5100_ULS_07:      // ULS H: PE + Earthquake
        dEFactor    = 1.0;
        break;
    case EN_AS5100_ULS_08:      // ULS J: PE + Wind
    case EN_AS5100_ULS_09:      // ULS J: PE + Wind + Temp
        dWFactor    = 1.0;
        dTFactor    = 1.0;
        break;
    case EN_AS5100_ULS_10:      // ULS K: PE + Temp
    case EN_AS5100_ULS_11:      // ULS K: PE + Temp + Moving
        dTFactor    = 1.25;
        dMFactor    = 1.0;
        break;
    case EN_AS5100_SLS_01_BRK: // Wind Earthquake 가 있으면 0.5 둘 중 하나만 있으면 0.7 Earthquake 둘 다 없으면 0
    case EN_AS5100_SLS_01_CF: // Wind Earthquake 가 있으면 0.5 둘 중 하나만 있으면 0.7 Earthquake 둘 다 없으면 0
        dMFactor  = 1.0;
        dBCFactor = 1.0;
        if ( nEcount != 0 && m_nWCount != 0)
        {
            dEFactor = 1.0 * 0.5;
            dWFactor = 1.0 * 0.5;
        }
        else
        {
            dEFactor = 1.0 * 0.7;
            dWFactor = 1.0 * 0.7;
        }
        break;
    case EN_AS5100_SLS_01_07_BRK:   // SLS : PE + Moving + 0.7Wind
    case EN_AS5100_SLS_01_07_CF :   // SLS : PE + Moving + 0.7Wind
        dMFactor  = 1.0;
        dBCFactor = 1.0;
        if (m_nWCount != 0)
        {
            dWFactor = 1.0 * 0.7;
        }
        else
        {
            dWFactor = 0.0;
        }
        break;
    case EN_AS5100_SLS_02_BRK:  // Temp Earthquake 가 있으면 0.5 둘 중 하나만 있으면 0.7 Temp 둘 다 없으면 0
    case EN_AS5100_SLS_02_CF:  // Temp Earthquake 가 있으면 0.5 둘 중 하나만 있으면 0.7 Temp 둘 다 없으면 0
        dMFactor  = 1.0;
        dBCFactor = 1.0;
        if ( nEcount != 0 && (m_nTCount  != 0 || m_nTpgCount != 0))
        {
            dEFactor = 1.0 * 0.5;
            dTFactor = 1.0 * 0.5;
        }
        else
        {
            dEFactor = 1.0 * 0.7;
            dTFactor = 1.0 * 0.7;
        }
        break;
    case EN_AS5100_SLS_02_07_BRK:   // SLS : PE + Moving + 0.7Temp
    case EN_AS5100_SLS_02_07_CF:    // SLS : PE + Moving + 0.7Temp
        dMFactor  = 1.0;
        dBCFactor = 1.0;
        if ((m_nTCount  != 0 || m_nTpgCount != 0))
        {
            dTFactor = 1.0 * 0.7;
        }
        else
        {
            dTFactor = 1.0 * 0.0;
        }
        break;
    case EN_AS5100_SLS_03_BRK: // Temp Wind 가 있으면 0.5 둘 중 하나만 있으면 0.7 Wind 둘 다 없으면 0
    case EN_AS5100_SLS_03_CF: // Temp Wind 가 있으면 0.5 둘 중 하나만 있으면 0.7 Wind 둘 다 없으면 0
        dMFactor  = 1.0;
        dBCFactor = 1.0;
        if ((m_nTCount  != 0 || m_nTpgCount != 0) && m_nWCount != 0)
        {
            dTFactor = 1.0 * 0.5;
            dWFactor = 1.0 * 0.5;
        }
        else
        {
            dTFactor = 1.0 * 0.7;
            dWFactor = 1.0 * 0.7;
        }
        break;
    case EN_AS5100_SLS_03_07_BRK:   // SLS : PE + Moving + 0.7Earthquake
    case EN_AS5100_SLS_03_07_CF:    // SLS : PE + Moving + 0.7Earthquake
        dMFactor  = 1.0;
        dBCFactor = 1.0;

        if ( nEcount != 0)
        {
            dEFactor = 1.0 * 0.7;
        }
        else
        {
            dEFactor = 0.0;
        }
        break;
    case EN_AS5100_SLS_04: // Wind Earthquake 가 있으면 0.5 둘 중 하나만 있으면 0.7 Earthquake 둘 다 없으면 0
        dTFactor = 1.0;
        if ( nEcount != 0 && m_nWCount != 0)
        {
            dEFactor = 1.0 * 0.5;
            dWFactor = 1.0 * 0.5;
        }
        else
        {
            dEFactor = 1.0 * 0.7;
            dWFactor = 1.0 * 0.7;
        }
        break;
    case EN_AS5100_SLS_04_07:       // SLS : PE + Temp + 0.7Wind
        dTFactor = 1.0;
        if (m_nWCount != 0)
        {
            dWFactor = 1.0 * 0.7;
        }
        else
        {
            dWFactor = 0.0;
        }
        break;
    case EN_AS5100_SLS_05_BRK: // Wind Moving 가 있으면 0.5 둘 중 하나만 있으면 0.7 Wind 둘 다 없으면 0
    case EN_AS5100_SLS_05_CF:  // Wind Moving 가 있으면 0.5 둘 중 하나만 있으면 0.7 Wind 둘 다 없으면 0
        dTFactor = 1.0;
        if (m_nWCount != 0 && m_nMCount != 0)
        {
            dWFactor  = 1.0 * 0.5;
            dMFactor  = 1.0 * 0.5;
            dBCFactor = 1.0 * 0.5;
        }
        else
        {
            dWFactor  = 1.0 * 0.7;
            dMFactor  = 1.0 * 0.7;
            dBCFactor = 1.0 * 0.7;
        }
        break;
    case EN_AS5100_SLS_05_07_BRK:   // SLS : PE + Temp + 0.7Moving 
    case EN_AS5100_SLS_05_07_CF:    // SLS : PE + Temp + 0.7Moving      
        dTFactor = 1.0;
        if (m_nMCount != 0)
        {
            dMFactor  = 1.0 * 0.7;
            dBCFactor = 1.0 * 0.7;
        }
        else
        {
            dMFactor  = 0.0;
            dBCFactor = 0.0;
        }
        break;
    case EN_AS5100_SLS_06_BRK: // Moving Earthquake 가 있으면 0.5 둘 중 하나만 있으면 0.7 Moving 둘 다 없으면 0
    case EN_AS5100_SLS_06_CF:  // Moving Earthquake 가 있으면 0.5 둘 중 하나만 있으면 0.7 Moving 둘 다 없으면 0
        dTFactor = 1.0;
        if (m_nMCount != 0 && nEcount != 0)
        {
            dEFactor  = 1.0 * 0.5;
            dMFactor  = 1.0 * 0.5;
            dBCFactor = 1.0 * 0.5;
        }
        else
        {
            dEFactor  = 1.0 * 0.7;
            dMFactor  = 1.0 * 0.7;
            dBCFactor = 1.0 * 0.7;
        }
        break;
    case EN_AS5100_SLS_06_07:       // SLS : PE + Temp + 0.7Earthquake
        dTFactor = 1.0;
        if ( nEcount != 0)
        {
            dEFactor = 1.0 * 0.7;
        }
        else
        {
            dEFactor  = 0.0;
        }
        break;
    case EN_AS5100_SLS_07: // Temp Earthquake 가 있으면 0.5 둘 중 하나만 있으면 0.7 Earthquake 둘 다 없으면 0
        dWFactor  = 1.0;
        if ( nEcount != 0 && (m_nTCount  != 0 || m_nTpgCount != 0))
        {
            dEFactor = 1.0 * 0.5;
            dTFactor = 1.0 * 0.5;
        }
        else
        {
            dEFactor = 1.0 * 0.7;
            dTFactor = 1.0 * 0.7;
        }
        break;
    case EN_AS5100_SLS_07_07:       // SLS : PE + Wind + 0.7Earthquake
        dWFactor  = 1.0;
        if ( nEcount != 0)
        {
            dEFactor = 1.0 * 0.7;
        }
        else
        {
            dEFactor = 0.0;
        }
        break;
    case EN_AS5100_SLS_08_BRK: // Temp Moving 가 있으면 0.5 둘 중 하나만 있으면 0.7 Temp 둘 다 없으면 0
    case EN_AS5100_SLS_08_CF:  // Temp Moving 가 있으면 0.5 둘 중 하나만 있으면 0.7 Temp 둘 다 없으면 0
        dWFactor  = 1.0;
        if ((m_nTCount  != 0 || m_nTpgCount != 0) && m_nMCount != 0)
        {
            dTFactor  = 1.0 * 0.5;
            dMFactor  = 1.0 * 0.5;
            dBCFactor = 1.0 * 0.5;
        }
        else
        {
            dTFactor  = 1.0 * 0.7;
            dMFactor  = 1.0 * 0.7;
            dBCFactor = 1.0 * 0.7;
        }
        break;
    case EN_AS5100_SLS_08_07:       // SLS : PE + Wind + 0.7Temp         
        dWFactor  = 1.0;
        if ((m_nTCount  != 0 || m_nTpgCount != 0))
        {
            dTFactor = 1.0 * 0.7;
        }
        else
        {
            dTFactor  = 0.0;
        }
        break;
    case EN_AS5100_SLS_09_BRK: // Moving Earthquake 가 있으면 0.5 둘 중 하나만 있으면 0.7 Moving 둘 다 없으면 0
    case EN_AS5100_SLS_09_CF:  // Moving Earthquake 가 있으면 0.5 둘 중 하나만 있으면 0.7 Moving 둘 다 없으면 0
        dWFactor  = 1.0;
        if (m_nMCount != 0 && nEcount != 0)
        {
            dEFactor  = 1.0 * 0.5;
            dMFactor  = 1.0 * 0.5;
            dBCFactor = 1.0 * 0.5;
        }
        else
        {
            dEFactor  = 1.0 * 0.7;
            dMFactor  = 1.0 * 0.7;
            dBCFactor = 1.0 * 0.7;
        }
        break;
    case EN_AS5100_SLS_09_07_BRK:   // SLS : PE + Wind + 0.7Moving   
    case EN_AS5100_SLS_09_07_CF:    // SLS : PE + Wind + 0.7Moving
        dWFactor  = 1.0;
        if (m_nMCount != 0)
        {
            dMFactor  = 1.0 * 0.7;
            dBCFactor = 1.0 * 0.7;
        }
        else
        {
            dMFactor  = 0.0;
            dBCFactor = 0.0;
        }
        break;
    case EN_AS5100_SLS_10: // Temp Wind 가 있으면 0.5 둘 중 하나만 있으면 0.7 Wind 둘 다 없으면 0
        dEFactor  = 1.0;
        if (m_nWCount != 0 && (m_nTCount  != 0 || m_nTpgCount != 0))
        {
            dWFactor = 1.0 * 0.5;
            dTFactor = 1.0 * 0.5;
        }
        else
        {
            dWFactor = 1.0 * 0.7;
            dTFactor = 1.0 * 0.7;
        }
        break;
    case EN_AS5100_SLS_10_07:       // SLS : PE + Earthquake + 0.7Wind
        dEFactor  = 1.0;
        if (m_nWCount != 0)
        {
            dWFactor = 1.0 * 0.7;
        }
        else
        {
            dWFactor = 0.0;
        }
        break;
    case EN_AS5100_SLS_11_BRK: // Temp Moving 가 있으면 0.5 둘 중 하나만 있으면 0.7 Temp 둘 다 없으면 0
    case EN_AS5100_SLS_11_CF:  // Temp Moving 가 있으면 0.5 둘 중 하나만 있으면 0.7 Temp 둘 다 없으면 0
        dEFactor  = 1.0;
        if ((m_nTCount  != 0 || m_nTpgCount != 0) && m_nMCount != 0)
        {
            dTFactor  = 1.0 * 0.5;
            dMFactor  = 1.0 * 0.5;
            dBCFactor = 1.0 * 0.5;
        }
        else
        {
            dTFactor  = 1.0 * 0.7;
            dMFactor  = 1.0 * 0.7;
            dBCFactor = 1.0 * 0.7;
        }
        break;
    case EN_AS5100_SLS_11_07:       // SLS : PE + Earthquake + 0.7Temp
        dEFactor  = 1.0;
        if ((m_nTCount  != 0 || m_nTpgCount != 0))
        {
            dTFactor = 1.0 * 0.7;
        }
        else
        {
            dTFactor  = 0.0;
        }
        break;
    case EN_AS5100_SLS_12_BRK: // Moving Wind 가 있으면 0.5 둘 중 하나만 있으면 0.7 Moving 둘 다 없으면 0
    case EN_AS5100_SLS_12_CF:  // Moving Wind 가 있으면 0.5 둘 중 하나만 있으면 0.7 Moving 둘 다 없으면 0
        dEFactor  = 1.0;
        if (m_nMCount != 0 && m_nWCount != 0)
        {
            dWFactor  = 1.0 * 0.5;
            dMFactor  = 1.0 * 0.5;
            dBCFactor = 1.0 * 0.5;
        }
        else
        {
            dWFactor  = 1.0 * 0.7;
            dMFactor  = 1.0 * 0.7;
            dBCFactor = 1.0 * 0.7;
        }
        break;
    case EN_AS5100_SLS_12_07_BRK:   // SLS : PE + Earthquake + 0.7Moving
    case EN_AS5100_SLS_12_07_CF:    // SLS : PE + Earthquake + 0.7Moving
        dEFactor  = 1.0;
        if (m_nMCount != 0)
        {
            dMFactor  = 1.0 * 0.7;
            dBCFactor = 1.0 * 0.7;
        }
        else
        {
            dMFactor  = 0.0;
            dBCFactor = 0.0;
        }
        break;
    default:
        ASSERT(0);
        break;
    }

}

void CLoadCombCtrl::SetAS5100CSStageTransient(const EN_AS5100_NO& enLcomNo, ArINT& arCSLoad, int& nErection, double& dTFactor, double& dWFactor, int& nn, int& nLoadCase, T_LCOM_D& rData)
{
    switch (enLcomNo)
    {
    case EN_AS5100_ULS_01:      // ULS A: Minimum Strength and Stability - D + DW + EP + WP
    case EN_AS5100_ULS_02_BRK:  // ULS B: PE + Moving
    case EN_AS5100_ULS_02_CF:   // ULS B: PE + Moving
    case EN_AS5100_ULS_05:      // ULS E: PE + CO
    case EN_AS5100_ULS_06:      // ULS E: PE + CO + Moving
    case EN_AS5100_ULS_07:      // ULS H: PE + Earthquake
        break;
    case EN_AS5100_ULS_03_BRK:  // ULS B: PE + Moving + Temp
    case EN_AS5100_ULS_03_CF:   // ULS B: PE + Moving + Temp
    case EN_AS5100_ULS_10:      // ULS K: PE + Temp
    case EN_AS5100_ULS_11:      // ULS K: PE + Temp + Moving
        if(CheckCSEL(nErection, _T("T")))
            Set_CStageCase(arCSLoad.GetAt(nn), dTFactor, nLoadCase, rData, TRUE);
        if(CheckCSEL(nErection, _T("TPG")))
            Set_CStageCase(arCSLoad.GetAt(nn), dTFactor, nLoadCase, rData, TRUE);
        break;
    case EN_AS5100_ULS_04_BRK:  // ULS B: PE + Moving + Wind
    case EN_AS5100_ULS_04_CF:   // ULS B: PE + Moving + Wind
    case EN_AS5100_ULS_08:      // ULS J: PE + Wind
        if(CheckCSEL(nErection, _T("W")))
            Set_CStageCase(arCSLoad.GetAt(nn), dWFactor, nLoadCase, rData, TRUE);
        break;
    case EN_AS5100_ULS_09:      // ULS J: PE + Wind + Temp
        if(CheckCSEL(nErection, _T("W")))
            Set_CStageCase(arCSLoad.GetAt(nn), dWFactor, nLoadCase, rData, TRUE);
        if(CheckCSEL(nErection, _T("T")))
            Set_CStageCase(arCSLoad.GetAt(nn), dTFactor, nLoadCase, rData, TRUE);
        if(CheckCSEL(nErection, _T("TPG")))
            Set_CStageCase(arCSLoad.GetAt(nn), dTFactor, nLoadCase, rData, TRUE);
        break;

    case EN_AS5100_SLS_03_07_BRK:   // SLS : PE + Moving + 0.7Earthquake
    case EN_AS5100_SLS_03_07_CF:    // SLS : PE + Moving + 0.7Earthquake
    case EN_AS5100_SLS_12_07_BRK:   // SLS : PE + Earthquake + 0.7Moving
    case EN_AS5100_SLS_12_07_CF:    // SLS : PE + Earthquake + 0.7Moving
        break;
    case EN_AS5100_SLS_01_BRK: 
    case EN_AS5100_SLS_01_CF: 
    case EN_AS5100_SLS_01_07_BRK:   // SLS : PE + Moving + 0.7Wind
    case EN_AS5100_SLS_01_07_CF :   // SLS : PE + Moving + 0.7Wind
    case EN_AS5100_SLS_09_BRK: 
    case EN_AS5100_SLS_09_CF: 
    case EN_AS5100_SLS_07_07:       // SLS : PE + Wind + 0.7Earthquake
    case EN_AS5100_SLS_09_07_BRK:   // SLS : PE + Wind + 0.7Moving   
    case EN_AS5100_SLS_09_07_CF:    // SLS : PE + Wind + 0.7Moving
    case EN_AS5100_SLS_12_BRK: 
    case EN_AS5100_SLS_12_CF: 
    case EN_AS5100_SLS_10_07:       // SLS : PE + Earthquake + 0.7Wind
        if(CheckCSEL(nErection, _T("W")))
            Set_CStageCase(arCSLoad.GetAt(nn), dWFactor, nLoadCase, rData, TRUE);
        break;
    case EN_AS5100_SLS_02_BRK: 
    case EN_AS5100_SLS_02_CF: 
    case EN_AS5100_SLS_02_07_BRK:   // SLS : PE + Moving + 0.7Temp
    case EN_AS5100_SLS_02_07_CF:    // SLS : PE + Moving + 0.7Temp
    case EN_AS5100_SLS_06_BRK: 
    case EN_AS5100_SLS_06_CF: 
    case EN_AS5100_SLS_05_07_BRK:   // SLS : PE + Temp + 0.7Moving      
    case EN_AS5100_SLS_05_07_CF:    // SLS : PE + Temp + 0.7Moving      
    case EN_AS5100_SLS_06_07:       // SLS : PE + Temp + 0.7Earthquake
    case EN_AS5100_SLS_11_BRK: 
    case EN_AS5100_SLS_11_CF:
    case EN_AS5100_SLS_11_07:       // SLS : PE + Earthquake + 0.7Temp
        if(CheckCSEL(nErection, _T("T")))
            Set_CStageCase(arCSLoad.GetAt(nn), dTFactor, nLoadCase, rData, TRUE);
        if(CheckCSEL(nErection, _T("TPG")))
            Set_CStageCase(arCSLoad.GetAt(nn), dTFactor, nLoadCase, rData, TRUE);
        break;
    case EN_AS5100_SLS_03_BRK: 
    case EN_AS5100_SLS_03_CF: 
    case EN_AS5100_SLS_04: 
    case EN_AS5100_SLS_05_BRK: 
    case EN_AS5100_SLS_05_CF:
    case EN_AS5100_SLS_04_07:       // SLS : PE + Temp + 0.7Wind
    case EN_AS5100_SLS_07: 
    case EN_AS5100_SLS_08_BRK: 
    case EN_AS5100_SLS_08_CF:
    case EN_AS5100_SLS_08_07:       // SLS : PE + Wind + 0.7Temp         
    case EN_AS5100_SLS_10: 
        if(CheckCSEL(nErection, _T("W")))
            Set_CStageCase(arCSLoad.GetAt(nn), dWFactor, nLoadCase, rData, TRUE);
        if(CheckCSEL(nErection, _T("T")))
            Set_CStageCase(arCSLoad.GetAt(nn), dTFactor, nLoadCase, rData, TRUE);
        if(CheckCSEL(nErection, _T("TPG")))
            Set_CStageCase(arCSLoad.GetAt(nn), dTFactor, nLoadCase, rData, TRUE);
        break;
    default:
        ASSERT(0);
        break;
    }
}

void CLoadCombCtrl::SetAS5100IncludeCSMoving(const EN_AS5100_NO& enLcomNo, int& nMove, double& dMFactor, double& dBCFactor,double& dCoFactor,
                                             ArINT& arCoLoad, ArUNT& arMLoad, ArINT& arBrkLoad , ArINT& arCfLoad, int& nCoL, int& nBrkL, int& nCfL, int& LoadCaseNo, T_LCOM_D& rData)
{
    switch (enLcomNo)
    {
    case EN_AS5100_ULS_01:// ULS A: Minimum Strength and Stability - D + DW + EP + WP
    case EN_AS5100_ULS_07:      // ULS H: PE + Earthquake
    case EN_AS5100_ULS_08:      // ULS J: PE + Wind
    case EN_AS5100_ULS_09:      // ULS J: PE + Wind + Temp
    case EN_AS5100_ULS_10:      // ULS K: PE + Temp
    case EN_AS5100_SLS_04:      // SLS : PE + Temp + 0.5(Wind + Earthquake)
    case EN_AS5100_SLS_04_07:   // SLS : PE + Temp + 0.7Wind
    case EN_AS5100_SLS_06_07:   // SLS : PE + Temp + 0.7Earthquake
    case EN_AS5100_SLS_07:      // SLS : PE + Wind + 0.5(Earthquake + Temp)
    case EN_AS5100_SLS_07_07:   // SLS : PE + Wind + 0.7Earthquake
    case EN_AS5100_SLS_08_07:   // SLS : PE + Wind + 0.7Temp  
    case EN_AS5100_SLS_10:      // SLS : PE + Earthquake + 0.5(Wind + Temp)
    case EN_AS5100_SLS_10_07:   // SLS : PE + Earthquake + 0.7Wind
    case EN_AS5100_SLS_11_07:   // SLS : PE + Earthquake + 0.7Temp
        break;
    case EN_AS5100_ULS_02_BRK:      // ULS B: PE + Moving
    case EN_AS5100_ULS_03_BRK:      // ULS B: PE + Moving + Temp
    case EN_AS5100_ULS_04_BRK:      // ULS B: PE + Moving + Wind
    case EN_AS5100_SLS_01_BRK:      // SLS : PE + Moving + 0.5(Wind + Earthquake)
    case EN_AS5100_SLS_02_BRK:      // SLS : PE + Moving + 0.5(Temp + Earthquake)
    case EN_AS5100_SLS_03_BRK:      // SLS : PE + Moving + 0.5(Temp + Wind)
    case EN_AS5100_SLS_01_07_BRK:   // SLS : PE + Moving + 0.7Wind
    case EN_AS5100_SLS_02_07_BRK:   // SLS : PE + Moving + 0.7Temp
    case EN_AS5100_SLS_03_07_BRK:   // SLS : PE + Moving + 0.7Earthquake
    case EN_AS5100_SLS_05_BRK:      // SLS : PE + Temp + 0.5(Moving + Wind)
    case EN_AS5100_SLS_06_BRK:      // SLS : PE + Temp + 0.5(Earthquake + Moving)
    case EN_AS5100_SLS_05_07_BRK:   // SLS : PE + Temp + 0.7Moving  
    case EN_AS5100_SLS_08_BRK:      // SLS : PE + Wind + 0.5(Temp + Moving),    
    case EN_AS5100_SLS_09_BRK:  // SLS : PE + Wind + 0.5(Moving + Earthquake)
    case EN_AS5100_SLS_09_07_BRK:   // SLS : PE + Wind + 0.7Moving   
    case EN_AS5100_SLS_11_BRK:  // SLS : PE + Earthquake + 0.5(Temp + Moving)
    case EN_AS5100_SLS_12_BRK:  // SLS : PE + Earthquake + 0.5(Moving + Wind)
    case EN_AS5100_SLS_12_07_BRK:   // SLS : PE + Earthquake + 0.7Moving
        if(m_nMCount>0) Set_MovingCase(arMLoad.GetAt(nMove), dMFactor, LoadCaseNo, rData, TRUE);
        if (m_nASBridgeType == 0) //RoadBridge
        {
            for(int i=0; i<m_nBrkCount;  i++) nBrkL += Set_STLCIncludeCS(arBrkLoad.GetAt(i),  dBCFactor, LoadCaseNo, rData, 0);
        }
        break;
    case EN_AS5100_ULS_02_CF:       // ULS B: PE + Moving
    case EN_AS5100_ULS_03_CF:       // ULS B: PE + Moving + Temp
    case EN_AS5100_ULS_04_CF:       // ULS B: PE + Moving + Wind
    case EN_AS5100_SLS_01_CF:       // SLS : PE + Moving + 0.5(Wind + Earthquake)
    case EN_AS5100_SLS_02_CF:       // SLS : PE + Moving + 0.5(Temp + Earthquake)
    case EN_AS5100_SLS_03_CF:       // SLS : PE + Moving + 0.5(Temp + Wind)
    case EN_AS5100_SLS_01_07_CF :   // SLS : PE + Moving + 0.7Wind
    case EN_AS5100_SLS_02_07_CF:    // SLS : PE + Moving + 0.7Temp
    case EN_AS5100_SLS_03_07_CF:    // SLS : PE + Moving + 0.7Earthquake
    case EN_AS5100_SLS_05_CF:   // SLS : PE + Temp + 0.5(Moving + Wind)
    case EN_AS5100_SLS_06_CF:   // SLS : PE + Temp + 0.5(Earthquake + Moving)
    case EN_AS5100_SLS_05_07_CF:    // SLS : PE + Temp + 0.7Moving  
    case EN_AS5100_SLS_08_CF:   // SLS : PE + Wind + 0.5(Temp + Moving),      
    case EN_AS5100_SLS_09_CF:   // SLS : PE + Wind + 0.5(Moving + Earthquake)
    case EN_AS5100_SLS_09_07_CF:    // SLS : PE + Wind + 0.7Moving
    case EN_AS5100_SLS_11_CF:   // SLS : PE + Earthquake + 0.5(Temp + Moving)
    case EN_AS5100_SLS_12_CF:   // SLS : PE + Earthquake + 0.5(Moving + Wind)
    case EN_AS5100_SLS_12_07_CF:    // SLS : PE + Earthquake + 0.7Moving
        if(m_nMCount>0) Set_MovingCase(arMLoad.GetAt(nMove), dMFactor, LoadCaseNo, rData, TRUE);
        if (m_nASBridgeType == 0) //RoadBridge
        {
            for(int i=0; i<m_nCfCount;  i++) nCfL += Set_STLCIncludeCS(arCfLoad.GetAt(i),  dBCFactor, LoadCaseNo, rData, 0);
        }
        break;

    case EN_AS5100_ULS_05:      // ULS E: PE + CO
        for(int i=0; i<m_nCoCount; i++)		  nCoL += Set_STLCIncludeCS(arCoLoad.GetAt(i), dCoFactor, LoadCaseNo, rData, 0);
        break;
    case EN_AS5100_ULS_06:      // ULS E: PE + CO + Moving
        for(int i=0; i<m_nCoCount; i++)		  nCoL += Set_STLCIncludeCS(arCoLoad.GetAt(i), dCoFactor, LoadCaseNo, rData, 0);
        if(m_nMCount>0) Set_MovingCase(arMLoad.GetAt(nMove), dMFactor, LoadCaseNo, rData, TRUE);
        break;
    case EN_AS5100_ULS_11:      // ULS K: PE + Temp + Moving
        if(m_nMCount>0) Set_MovingCase(arMLoad.GetAt(nMove), dMFactor, LoadCaseNo, rData, TRUE);
        break;

    default:
        ASSERT(0);
        break;
    }
}

void CLoadCombCtrl::SetAS5100IncludeCSTemp(const EN_AS5100_NO& enLcomNo, int& nTemp, int& nTempg, double& dTFactor, ArINT& arTLoad,  ArINT& arTpgLoad, int& nTL, int& nTpgL, int& LoadCaseNo, T_LCOM_D& rData)
{
    switch (enLcomNo)
    {
    case EN_AS5100_ULS_01:          // ULS A: Minimum Strength and Stability - D + DW + EP + WP
    case EN_AS5100_ULS_02_BRK:      // ULS B: PE + Moving
    case EN_AS5100_ULS_02_CF:       // ULS B: PE + Moving
    case EN_AS5100_ULS_04_BRK:      // ULS B: PE + Moving + Wind
    case EN_AS5100_ULS_04_CF:       // ULS B: PE + Moving + Wind
    case EN_AS5100_ULS_05:          // ULS E: PE + CO
    case EN_AS5100_ULS_06:          // ULS E: PE + CO + Moving
    case EN_AS5100_ULS_07:          // ULS H: PE + Earthquake
    case EN_AS5100_ULS_08:          // ULS J: PE + Wind
    case EN_AS5100_SLS_01_BRK:      // SLS : PE + Moving + 0.5(Wind + Earthquake)
    case EN_AS5100_SLS_01_CF:       // SLS : PE + Moving + 0.5(Wind + Earthquake)
    case EN_AS5100_SLS_01_07_BRK:   // SLS : PE + Moving + 0.7Wind
    case EN_AS5100_SLS_01_07_CF :   // SLS : PE + Moving + 0.7Wind
    case EN_AS5100_SLS_03_07_BRK:   // SLS : PE + Moving + 0.7Earthquake
    case EN_AS5100_SLS_03_07_CF:    // SLS : PE + Moving + 0.7Earthquake
    case EN_AS5100_SLS_09_BRK:      // SLS : PE + Wind + 0.5(Moving + Earthquake)
    case EN_AS5100_SLS_09_CF:       // SLS : PE + Wind + 0.5(Moving + Earthquake)
    case EN_AS5100_SLS_07_07:       // SLS : PE + Wind + 0.7Earthquake
    case EN_AS5100_SLS_09_07_BRK:   // SLS : PE + Wind + 0.7Moving   
    case EN_AS5100_SLS_09_07_CF:    // SLS : PE + Wind + 0.7Moving
    case EN_AS5100_SLS_12_BRK:      // SLS : PE + Earthquake + 0.5(Moving + Wind)
    case EN_AS5100_SLS_12_CF:       // SLS : PE + Earthquake + 0.5(Moving + Wind)
    case EN_AS5100_SLS_10_07:       // SLS : PE + Earthquake + 0.7Wind
    case EN_AS5100_SLS_12_07_BRK:   // SLS : PE + Earthquake + 0.7Moving
    case EN_AS5100_SLS_12_07_CF:    // SLS : PE + Earthquake + 0.7Moving
        break;
    case EN_AS5100_ULS_03_BRK:      // ULS B: PE + Moving + Temp
    case EN_AS5100_ULS_03_CF:       // ULS B: PE + Moving + Temp
    case EN_AS5100_ULS_09:          // ULS J: PE + Wind + Temp
    case EN_AS5100_ULS_10:          // ULS K: PE + Temp
    case EN_AS5100_ULS_11:          // ULS K: PE + Temp + Moving
    case EN_AS5100_SLS_02_BRK:      // SLS : PE + Moving + 0.5(Temp + Earthquake)
    case EN_AS5100_SLS_02_CF:       // SLS : PE + Moving + 0.5(Temp + Earthquake)
    case EN_AS5100_SLS_03_BRK:      // SLS : PE + Moving + 0.5(Temp + Wind)
    case EN_AS5100_SLS_03_CF:       // SLS : PE + Moving + 0.5(Temp + Wind)
    case EN_AS5100_SLS_02_07_BRK:   // SLS : PE + Moving + 0.7Temp
    case EN_AS5100_SLS_02_07_CF:    // SLS : PE + Moving + 0.7Temp
    case EN_AS5100_SLS_04:          // SLS : PE + Temp + 0.5(Wind + Earthquake)
    case EN_AS5100_SLS_05_BRK:      // SLS : PE + Temp + 0.5(Moving + Wind)
    case EN_AS5100_SLS_05_CF:       // SLS : PE + Temp + 0.5(Moving + Wind)
    case EN_AS5100_SLS_06_BRK:      // SLS : PE + Temp + 0.5(Earthquake + Moving)
    case EN_AS5100_SLS_06_CF:       // SLS : PE + Temp + 0.5(Earthquake + Moving)
    case EN_AS5100_SLS_04_07:       // SLS : PE + Temp + 0.7Wind
    case EN_AS5100_SLS_05_07_BRK:   // SLS : PE + Temp + 0.7Moving  
    case EN_AS5100_SLS_05_07_CF:    // SLS : PE + Temp + 0.7Moving  
    case EN_AS5100_SLS_06_07:       // SLS : PE + Temp + 0.7Earthquake
    case EN_AS5100_SLS_07:          // SLS : PE + Wind + 0.5(Earthquake + Temp)
    case EN_AS5100_SLS_08_BRK:      // SLS : PE + Wind + 0.5(Temp + Moving),    
    case EN_AS5100_SLS_08_CF:       // SLS : PE + Wind + 0.5(Temp + Moving),      
    case EN_AS5100_SLS_08_07:       // SLS : PE + Wind + 0.7Temp  
    case EN_AS5100_SLS_10:          // SLS : PE + Earthquake + 0.5(Wind + Temp)
    case EN_AS5100_SLS_11_BRK:      // SLS : PE + Earthquake + 0.5(Temp + Moving)
    case EN_AS5100_SLS_11_CF:       // SLS : PE + Earthquake + 0.5(Temp + Moving)
    case EN_AS5100_SLS_11_07:       // SLS : PE + Earthquake + 0.7Temp
        if(m_nTCount > 0)   nTL   += Set_STLCIncludeCS(arTLoad.GetAt(nTemp),   dTFactor, LoadCaseNo, rData, 0);
        if(m_nTpgCount > 0) nTpgL += Set_STLCIncludeCS(arTpgLoad.GetAt(nTempg), dTFactor, LoadCaseNo, rData, 0);
        break;
    default:
        ASSERT(0);
        break;
    }
}

void CLoadCombCtrl::SetAS5100IncludeCSWind(const EN_AS5100_NO& enLcomNo, int& nWind, double& dWF, ArINT& arWLoad, int& nWL, int& LoadCaseNo, T_LCOM_D& rData)
{
    switch (enLcomNo)
    {
    case EN_AS5100_ULS_01:// ULS A: Minimum Strength and Stability - D + DW + EP + WP
    case EN_AS5100_ULS_02_BRK:  // ULS B: PE + Moving
    case EN_AS5100_ULS_02_CF:   // ULS B: PE + Moving
    case EN_AS5100_ULS_03_BRK:  // ULS B: PE + Moving + Temp
    case EN_AS5100_ULS_03_CF:   // ULS B: PE + Moving + Temp
    case EN_AS5100_ULS_05:      // ULS E: PE + CO
    case EN_AS5100_ULS_06:      // ULS E: PE + CO + Moving
    case EN_AS5100_ULS_07:      // ULS H: PE + Earthquake
    case EN_AS5100_ULS_10:      // ULS K: PE + Temp
    case EN_AS5100_ULS_11:      // ULS K: PE + Temp + Moving
    case EN_AS5100_SLS_02_BRK:  // SLS : PE + Moving + 0.5(Temp + Earthquake)
    case EN_AS5100_SLS_02_CF:   // SLS : PE + Moving + 0.5(Temp + Earthquake)
    case EN_AS5100_SLS_02_07_BRK:   // SLS : PE + Moving + 0.7Temp
    case EN_AS5100_SLS_02_07_CF:    // SLS : PE + Moving + 0.7Temp
    case EN_AS5100_SLS_03_07_BRK:   // SLS : PE + Moving + 0.7Earthquake
    case EN_AS5100_SLS_03_07_CF:    // SLS : PE + Moving + 0.7Earthquake
    case EN_AS5100_SLS_06_BRK:  // SLS : PE + Temp + 0.5(Earthquake + Moving)
    case EN_AS5100_SLS_06_CF:   // SLS : PE + Temp + 0.5(Earthquake + Moving)
    case EN_AS5100_SLS_05_07_BRK:   // SLS : PE + Temp + 0.7Moving  
    case EN_AS5100_SLS_05_07_CF:    // SLS : PE + Temp + 0.7Moving  
    case EN_AS5100_SLS_06_07:       // SLS : PE + Temp + 0.7Earthquake
    case EN_AS5100_SLS_11_BRK:  // SLS : PE + Earthquake + 0.5(Temp + Moving)
    case EN_AS5100_SLS_11_CF:   // SLS : PE + Earthquake + 0.5(Temp + Moving)
    case EN_AS5100_SLS_11_07:       // SLS : PE + Earthquake + 0.7Temp
    case EN_AS5100_SLS_12_07_BRK:   // SLS : PE + Earthquake + 0.7Moving
    case EN_AS5100_SLS_12_07_CF:    // SLS : PE + Earthquake + 0.7Moving
        break;
    
    case EN_AS5100_ULS_04_BRK:  // ULS B: PE + Moving + Wind
    case EN_AS5100_ULS_04_CF:   // ULS B: PE + Moving + Wind
    case EN_AS5100_ULS_08:      // ULS J: PE + Wind
    case EN_AS5100_ULS_09:      // ULS J: PE + Wind + Temp
    case EN_AS5100_SLS_01_BRK:  // SLS : PE + Moving + 0.5(Wind + Earthquake)
    case EN_AS5100_SLS_01_CF:   // SLS : PE + Moving + 0.5(Wind + Earthquake)
    case EN_AS5100_SLS_03_BRK:  // SLS : PE + Moving + 0.5(Temp + Wind)
    case EN_AS5100_SLS_03_CF:   // SLS : PE + Moving + 0.5(Temp + Wind)
    case EN_AS5100_SLS_01_07_BRK:   // SLS : PE + Moving + 0.7Wind
    case EN_AS5100_SLS_01_07_CF :   // SLS : PE + Moving + 0.7Wind
    case EN_AS5100_SLS_04:      // SLS : PE + Temp + 0.5(Wind + Earthquake)
    case EN_AS5100_SLS_05_BRK:  // SLS : PE + Temp + 0.5(Moving + Wind)
    case EN_AS5100_SLS_05_CF:   // SLS : PE + Temp + 0.5(Moving + Wind)
    case EN_AS5100_SLS_04_07:       // SLS : PE + Temp + 0.7Wind
    case EN_AS5100_SLS_07:      // SLS : PE + Wind + 0.5(Earthquake + Temp)
    case EN_AS5100_SLS_08_BRK:  // SLS : PE + Wind + 0.5(Temp + Moving),    
    case EN_AS5100_SLS_08_CF:   // SLS : PE + Wind + 0.5(Temp + Moving),      
    case EN_AS5100_SLS_09_BRK:  // SLS : PE + Wind + 0.5(Moving + Earthquake)
    case EN_AS5100_SLS_09_CF:   // SLS : PE + Wind + 0.5(Moving + Earthquake)
    case EN_AS5100_SLS_07_07:       // SLS : PE + Wind + 0.7Earthquake
    case EN_AS5100_SLS_08_07:       // SLS : PE + Wind + 0.7Temp  
    case EN_AS5100_SLS_09_07_BRK:   // SLS : PE + Wind + 0.7Moving   
    case EN_AS5100_SLS_09_07_CF:    // SLS : PE + Wind + 0.7Moving
    case EN_AS5100_SLS_10:      // SLS : PE + Earthquake + 0.5(Wind + Temp)
    case EN_AS5100_SLS_12_BRK:  // SLS : PE + Earthquake + 0.5(Moving + Wind)
    case EN_AS5100_SLS_12_CF:   // SLS : PE + Earthquake + 0.5(Moving + Wind)
    case EN_AS5100_SLS_10_07:       // SLS : PE + Earthquake + 0.7Wind
        if(m_nWCount>0)  nWL  += Set_STLCIncludeCS(arWLoad.GetAt(nWind/2), dWF,  LoadCaseNo, rData, 0);
        break;
    default:
        ASSERT(0);
        break;
    }
}

void CLoadCombCtrl::SetAS5100IncludeCSEarth(const EN_AS5100_NO& enLcomNo, double& dEFactor, ArINT& arELoad, ArUNT& arEspLoad, int& nEL, int& nEsp, int& LoadCaseNo, int nEarthIdx, T_LCOM_D& rData)
{
    switch (enLcomNo)
    {
    case EN_AS5100_ULS_01:          // ULS A: Minimum Strength and Stability - D + DW + EP + WP
    case EN_AS5100_ULS_02_BRK:      // ULS B: PE + Moving
    case EN_AS5100_ULS_02_CF:       // ULS B: PE + Moving
    case EN_AS5100_ULS_03_BRK:      // ULS B: PE + Moving + Temp
    case EN_AS5100_ULS_03_CF:       // ULS B: PE + Moving + Temp
    case EN_AS5100_ULS_04_BRK:      // ULS B: PE + Moving + Wind
    case EN_AS5100_ULS_04_CF:       // ULS B: PE + Moving + Wind
    case EN_AS5100_ULS_05:          // ULS E: PE + CO
    case EN_AS5100_ULS_06:          // ULS E: PE + CO + Moving
    case EN_AS5100_ULS_08:          // ULS J: PE + Wind
    case EN_AS5100_ULS_09:          // ULS J: PE + Wind + Temp
    case EN_AS5100_ULS_10:          // ULS K: PE + Temp
    case EN_AS5100_ULS_11:          // ULS K: PE + Temp + Moving
    case EN_AS5100_SLS_03_BRK:      // SLS : PE + Moving + 0.5(Temp + Wind)
    case EN_AS5100_SLS_03_CF:       // SLS : PE + Moving + 0.5(Temp + Wind)
    case EN_AS5100_SLS_01_07_BRK:   // SLS : PE + Moving + 0.7Wind
    case EN_AS5100_SLS_01_07_CF :   // SLS : PE + Moving + 0.7Wind
    case EN_AS5100_SLS_02_07_BRK:   // SLS : PE + Moving + 0.7Temp
    case EN_AS5100_SLS_02_07_CF:    // SLS : PE + Moving + 0.7Temp
    case EN_AS5100_SLS_05_BRK:      // SLS : PE + Temp + 0.5(Moving + Wind)
    case EN_AS5100_SLS_05_CF:       // SLS : PE + Temp + 0.5(Moving + Wind)
    case EN_AS5100_SLS_04_07:       // SLS : PE + Temp + 0.7Wind
    case EN_AS5100_SLS_05_07_BRK:   // SLS : PE + Temp + 0.7Moving  
    case EN_AS5100_SLS_05_07_CF:    // SLS : PE + Temp + 0.7Moving  
    case EN_AS5100_SLS_08_BRK:      // SLS : PE + Wind + 0.5(Temp + Moving),    
    case EN_AS5100_SLS_08_CF:       // SLS : PE + Wind + 0.5(Temp + Moving),      
    case EN_AS5100_SLS_08_07:       // SLS : PE + Wind + 0.7Temp  
    case EN_AS5100_SLS_09_07_BRK:   // SLS : PE + Wind + 0.7Moving   
    case EN_AS5100_SLS_09_07_CF:    // SLS : PE + Wind + 0.7Moving
        break;
    case EN_AS5100_ULS_07:          // ULS H: PE + Earthquake
        {
            for ( int i=0; i<m_nECount; i++ )
            {
                if ( i == nEarthIdx )
                {
                    nEL  += Set_STLCIncludeCS(arELoad.GetAt(i), dEFactor, LoadCaseNo, rData, 0);
                }
            }
            for ( int i=0; i<m_nEspCount; i++ )
            {
                if ( i == nEarthIdx )
                {
                    nEsp++;
                    Set_SplcCase(_T("ESP"), arEspLoad.GetAt(i), dEFactor, LoadCaseNo, rData, TRUE);
                }
            }
            break;
        }
    case EN_AS5100_SLS_01_BRK:      // SLS : PE + Moving + 0.5(Wind + Earthquake)
    case EN_AS5100_SLS_01_CF:       // SLS : PE + Moving + 0.5(Wind + Earthquake)
    case EN_AS5100_SLS_02_BRK:      // SLS : PE + Moving + 0.5(Temp + Earthquake)
    case EN_AS5100_SLS_02_CF:       // SLS : PE + Moving + 0.5(Temp + Earthquake)
    case EN_AS5100_SLS_03_07_BRK:   // SLS : PE + Moving + 0.7Earthquake
    case EN_AS5100_SLS_03_07_CF:    // SLS : PE + Moving + 0.7Earthquake
    case EN_AS5100_SLS_04:          // SLS : PE + Temp + 0.5(Wind + Earthquake)
    case EN_AS5100_SLS_06_BRK:      // SLS : PE + Temp + 0.5(Earthquake + Moving)
    case EN_AS5100_SLS_06_CF:       // SLS : PE + Temp + 0.5(Earthquake + Moving)
    case EN_AS5100_SLS_06_07:       // SLS : PE + Temp + 0.7Earthquake
    case EN_AS5100_SLS_10:          // SLS : PE + Earthquake + 0.5(Wind + Temp)
    case EN_AS5100_SLS_11_BRK:      // SLS : PE + Earthquake + 0.5(Temp + Moving)
    case EN_AS5100_SLS_11_CF:       // SLS : PE + Earthquake + 0.5(Temp + Moving)
    case EN_AS5100_SLS_12_BRK:      // SLS : PE + Earthquake + 0.5(Moving + Wind)
    case EN_AS5100_SLS_12_CF:       // SLS : PE + Earthquake + 0.5(Moving + Wind)
    case EN_AS5100_SLS_10_07:       // SLS : PE + Earthquake + 0.7Wind
    case EN_AS5100_SLS_11_07:       // SLS : PE + Earthquake + 0.7Temp
    case EN_AS5100_SLS_12_07_BRK:   // SLS : PE + Earthquake + 0.7Moving
    case EN_AS5100_SLS_12_07_CF:    // SLS : PE + Earthquake + 0.7Moving
        {
            for ( int i=0; i<m_nECount; i++ )
            {
                if ( i == nEarthIdx )
                {
                    nEL  += Set_STLCIncludeCS(arELoad.GetAt(i), dEFactor, LoadCaseNo, rData, 0);
                }
            }
            for ( int i=0; i<m_nEspCount; i++ )
            {
                if ( i == nEarthIdx )
                {
                    nEsp++;
                    Set_SplcCase(_T("ESP"), arEspLoad.GetAt(i), dEFactor, LoadCaseNo, rData, TRUE);
                }
            }
        }
        break;
    case EN_AS5100_SLS_07:          // SLS : PE + Wind + 0.5(Earthquake + Temp)
    case EN_AS5100_SLS_09_BRK:      // SLS : PE + Wind + 0.5(Moving + Earthquake)
    case EN_AS5100_SLS_09_CF:       // SLS : PE + Wind + 0.5(Moving + Earthquake)
    case EN_AS5100_SLS_07_07:       // SLS : PE + Wind + 0.7Earthquake
        {
            if ( m_nWCount == 0 )
                break;

            for ( int i=0; i<m_nECount; i++ )
            {
                if ( i == nEarthIdx )
                {
                    nEL  += Set_STLCIncludeCS(arELoad.GetAt(i), dEFactor, LoadCaseNo, rData, 0);
                }
            }
            for ( int i=0; i<m_nEspCount; i++ )
            {
                if ( i == nEarthIdx )
                {
                    nEsp++;
                    Set_SplcCase(_T("ESP"), arEspLoad.GetAt(i), dEFactor, LoadCaseNo, rData, TRUE);
                }
            }
        }
        break;
    default:
        ASSERT(0);
        break;
    }
}


void CLoadCombCtrl::SetAS5100CSStagePerm(const EN_AS5100_NO& enLcomNo, int& nErec, int& nErection, double& dDFactor, double& dDwFactor, int& nn, int& nD, int& nDw, ArINT& arCSLoad, int& nBoth, int& LoadCaseNo, CString& strcEL, T_LCOM_D& rData)
{
    if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

    if (enLcomNo >= EN_AS5100_SLS_01_BRK)
    {
        if(CheckCSEL(nErection, _T("D")))
            Set_CStageCase(arCSLoad.GetAt(nn), dDFactor, LoadCaseNo, rData, TRUE);
        if(CheckCSEL(nErection, _T("DW")))
            Set_CStageCase(arCSLoad.GetAt(nn), dDwFactor, LoadCaseNo, rData, TRUE);
    }
    else
    {
        if(CheckCSEL(nErection, _T("D")))
        {
            Set_CStageCase(arCSLoad.GetAt(nn), dDFactor, LoadCaseNo, rData, TRUE);
            (nD==1)    ? nBoth=1 : nBoth = 0;
        }
        if(CheckCSEL(nErection, _T("DW")))
        {
            Set_CStageCase(arCSLoad.GetAt(nn), dDwFactor, LoadCaseNo, rData, TRUE);
            (nDw==1)  ? nBoth=1 : nBoth = 0;
        }
    }

}

void CLoadCombCtrl::SetAS5100CSStagePSCRSHE(const EN_AS5100_NO& enLcomNo, ArINT& arCSLoad, double& dPsFactor, double& dCRSHFactor, int& nn, int& LoadCaseNo, T_LCOM_D& rData)
{
    if (enLcomNo >= EN_AS5100_SLS_01_BRK)
    {
		if ( m_nLcomType != D_LCOMTYPE_STLCOMP )
		{
			if ( CheckCS_SgldK(D_SGLD_TP_CVL, nn) ) Set_CStageCase(arCSLoad.GetAt(nn), dPsFactor, LoadCaseNo, rData, TRUE);
		}        
    }
	if ( m_nLcomType != D_LCOMTYPE_STLCOMP )
	{
		if ( CheckCS_SgldK(D_SGLD_TS_CVL, nn) ) Set_CStageCase(arCSLoad.GetAt(nn), dPsFactor, LoadCaseNo, rData, TRUE);
	}    
    if(CheckCS_SgldK(D_SGLD_CS_CVL, nn)) Set_CStageCase(arCSLoad.GetAt(nn), dCRSHFactor, LoadCaseNo, rData, TRUE);
    if(CheckCS_SgldK(D_SGLD_SS_CVL, nn)) Set_CStageCase(arCSLoad.GetAt(nn), dCRSHFactor, LoadCaseNo, rData, TRUE);
}


void CLoadCombCtrl::SetAS5100IncludeCSBasic(const EN_AS5100_NO& enLcomNo, ArINT& arDLoad , ArINT& arDwLoad, ArINT& arEpLoad, ArINT& arWpLoad, 
                                            double& dDFactor, double& dDwFactor, double& dEpFactor, double& dGwFactor, int& nD, int& nDw, int& nEp, int& nWp,
                                            int& nDL, int& nDwL, int& nEpL, int& nWpL, int& nBoth, int& LoadCaseNo, T_LCOM_D& rData)
{
    if (enLcomNo < EN_AS5100_SLS_01_BRK)
    {
        for(int i=0; i<m_nDCount;  i++)  { nDL   += Set_STLCIncludeCS(arDLoad.GetAt(i),   dDFactor,   LoadCaseNo, rData, D_SGLD_DL_CVL); (nD==1)    ? nBoth=1 : nBoth = 0; }
        for(int i=0; i<m_nDwCount; i++)  { nDwL  += Set_STLCIncludeCS(arDwLoad.GetAt(i),  dDwFactor,  LoadCaseNo, rData, D_SGLD_DL_CVL); (nDw==1)   ? nBoth=1 : nBoth = 0; }
        for(int i=0; i<m_nEpCount; i++)  { nEpL  += Set_STLCIncludeCS(arEpLoad.GetAt(i),  dEpFactor,  LoadCaseNo, rData, 0); (nEp==1)   ? nBoth=1 : nBoth = 0; }
        for(int i=0; i<m_nWpCount; i++)  { nWpL  += Set_STLCIncludeCS(arWpLoad.GetAt(i),  dGwFactor,  LoadCaseNo, rData, 0); (nWp==1)   ? nBoth=1 : nBoth = 0; }
    }
    else
    {
        for(int i=0; i<m_nDCount;  i++)  { nDL   += Set_STLCIncludeCS(arDLoad.GetAt(i),    dDFactor,     LoadCaseNo, rData, D_SGLD_DL_CVL);}
        for(int i=0; i<m_nDwCount; i++)  { nDwL  += Set_STLCIncludeCS(arDwLoad.GetAt(i),   dDwFactor,    LoadCaseNo, rData, D_SGLD_DL_CVL);}
        for(int i=0; i<m_nEpCount; i++)  { nEpL  += Set_STLCIncludeCS(arEpLoad.GetAt(i),   dEpFactor,    LoadCaseNo, rData, 0); }
        for(int i=0; i<m_nWpCount; i++)  { nWpL  += Set_STLCIncludeCS(arWpLoad.GetAt(i),   dGwFactor,    LoadCaseNo, rData, 0);}
    }
}



void CLoadCombCtrl::SetAS5100IncludeCSPerm(const EN_AS5100_NO& enLcomNo, ArINT& arFpLoad, ArINT& arBLoad , ArINT& arCrLoad, ArINT& arShLoad , ArINT& arPsLoad , ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arCoLoad, 
                                           double& dPsFactor, double& dCRSHFactor, double& dStlFactor, double& dWFBFactor, double& dCoFactor,
                                           int& nPsL, int& nCrL, int& nShL, int& nStL, int& nFpL, int& nBL, int& nSettle, int& nCoL, int& LoadCaseNo, T_LCOM_D& rData)
{
    if (enLcomNo > EN_AS5100_ULS_01)
    {
        for(int i=0; i<m_nPsCount; i++)		nPsL  += Set_STLCIncludeCS(arPsLoad.GetAt(i),  dPsFactor,	LoadCaseNo, rData, D_SGLD_TS_CVL);
        for(int i=0; i<m_nCrCount; i++)		nCrL  += Set_STLCIncludeCS(arCrLoad.GetAt(i),  dCRSHFactor, LoadCaseNo, rData, D_SGLD_CS_CVL);
        for(int i=0; i<m_nShCount; i++)		nShL  += Set_STLCIncludeCS(arShLoad.GetAt(i),  dCRSHFactor, LoadCaseNo, rData, D_SGLD_SS_CVL);
        for(int i=0; i<m_nStlCount;i++)		nStL  += Set_STLCIncludeCS(arStlLoad.GetAt(i), dStlFactor,  LoadCaseNo, rData, 0);
        if(m_nSmCount > 0)                    Set_SettleCase(arSmLoad.GetAt(nSettle),      dStlFactor,      LoadCaseNo, rData, TRUE);
        for(int i=0; i<m_nFpCount; i++)		nFpL  += Set_STLCIncludeCS(arFpLoad.GetAt(i),  dWFBFactor,  LoadCaseNo, rData, 0);
        for(int i=0; i<m_nBCount;  i++)		 nBL  += Set_STLCIncludeCS(arBLoad.GetAt(i),   dWFBFactor,  LoadCaseNo, rData, 0);
    }
}

void CLoadCombCtrl::Set_CvlAS5100_LoadFactorPerm(const EN_AS5100_NO& enLcomNo, double& dPsFactor, double& dCRSHFactor, double& dStlFactor, double& dWFBFactor)
{
    if (enLcomNo < EN_AS5100_SLS_01_BRK) // ULS 인 경우
    {
        dCRSHFactor = 1.2;
        dPsFactor   = 1.0;
        dStlFactor  = 1.5;
        dWFBFactor  = 1.3;
    }
    else // SLS 인 경우
    {
        dCRSHFactor = 1.0;
        dPsFactor   = 1.0;
        dStlFactor  = 1.0;
        dWFBFactor  = 1.0;
    }
}

////////////////////////////////////////////////////////////////////////
////// BD 21  -> BS 5400 으로 이름 수정          /////////
//////////////////////////////////////////////////////////////////////////

void CLoadCombCtrl::GetBD_21Factor( 
    CString& sD1Fac,  CString& sD2Fac, CString& sDw1Fac, CString& sDw2Fac, CString& sDc1Fac,    CString& sDc2Fac,
    CString& sEv1Fac, CString& sEv2Fac,CString& sEh1Fac, CString& sEh2Fac, CString& sGf3ULSFac, CString& sGf3SLSFac,     
    double& dD1,  double& dD2,  double& dDw1,  double& dDw2,  double& dDc1,    double& dDc2,
    double& dEv1, double& dEv2, double& dEh1,  double& dEh2,  double& dGf3ULS, double& dGf3SLS, BOOL b1stGrp)
{
    // b1stGrp = TRUE  : Group 1 일때 
    // b1stGrp = FALSE : Group 1 Fatigue or Group 2 일때 
    if(b1stGrp)
    {
        dD1  = m_dBD21D[0];	 sD1Fac.Format (_T("%.2f"), m_dBD21D[0]);
        dD2  = m_dBD21D[1];	 sD2Fac.Format (_T("%.2f"), m_dBD21D[1]);
        dDw1 = m_dBD21DW[0]; sDw1Fac.Format(_T("%.2f"), m_dBD21DW[0]);
        dDw2 = m_dBD21DW[1]; sDw2Fac.Format(_T("%.2f"), m_dBD21DW[1]);
        dDc1 = m_dBD21DC[0]; sDc1Fac.Format(_T("%.2f"), m_dBD21DC[0]);
        dDc2 = m_dBD21DC[1]; sDc2Fac.Format(_T("%.2f"), m_dBD21DC[1]);
        dEv1 = m_dBD21EV[0]; sEv1Fac.Format(_T("%.2f"), m_dBD21EV[0]);
        dEv2 = m_dBD21EV[1]; sEv2Fac.Format(_T("%.2f"), m_dBD21EV[1]);
        dEh1 = m_dBD21EH[0]; sEh1Fac.Format(_T("%.2f"), m_dBD21EH[0]);
        dEh2 = m_dBD21EH[1]; sEh2Fac.Format(_T("%.2f"), m_dBD21EH[1]);

        dGf3ULS = m_dBD21GF3ULS; sGf3ULSFac.Format(_T("%.2f"), m_dBD21GF3ULS);
        dGf3SLS = m_dBD21GF3SLS; sGf3ULSFac.Format(_T("%.2f"), m_dBD21GF3SLS);
    }
    else
    {
        dD1  = 1.0;	 sD1Fac.Format(_T("%.2f"), 1.0);
        dD2  = 1.0;	 sD2Fac.Format(_T("%.2f"), 1.0);
        dDw1 = 1.2; sDw1Fac.Format(_T("%.2f"), 1.2);
        dDw2 = 1.2; sDw2Fac.Format(_T("%.2f"), 1.2);
        dDc1 = 1.0; sDc1Fac.Format(_T("%.2f"), 1.0);
        dDc2 = 1.0; sDc2Fac.Format(_T("%.2f"), 1.0);
        dEv1 = 1.0; sEv1Fac.Format(_T("%.2f"), 1.0);
        dEv2 = 1.0; sEv2Fac.Format(_T("%.2f"), 1.0);
        dEh1 = 1.0; sEh1Fac.Format(_T("%.2f"), 1.0);
        dEh2 = 1.0; sEh2Fac.Format(_T("%.2f"), 1.0);

        dGf3ULS = m_dBD21GF3ULS; sGf3ULSFac.Format(_T("%.2f"), m_dBD21GF3ULS);
        dGf3SLS = m_dBD21GF3SLS; sGf3ULSFac.Format(_T("%.2f"), m_dBD21GF3SLS);
    }

}

void CLoadCombCtrl::GetBD_21FactoredPerm( 
    double& dD1,  double& dD2,  double& dDw1,  double& dDw2,  double& dDc1,    double& dDc2,
    double& dEv1, double& dEv2, double& dEh1,  double& dEh2,  double& dGf3)
{
    dD1  = dGf3 * dD1;
    dD2  = dGf3 * dD2;
    dDw1 = dGf3 * dDw1;
    dDw2 = dGf3 * dDw2;
    dDc1 = dGf3 * dDc1;
    dDc2 = dGf3 * dDc2;
    dEv1 = dGf3 * dEv1;
    dEv2 = dGf3 * dEv2;
    dEh1 = dGf3 * dEh1;
    dEh2 = dGf3 * dEh2;
}

BOOL CLoadCombCtrl::Check_BS5400_LoadCase(const EN_BS5400_NO& enLcomNo)
{

    if (m_nDCount == 0 && m_nDwCount == 0 && m_nDcCount  == 0 && m_nStlCount == 0 && m_nSmCount == 0 && m_nEvCount == 0 && m_nEhCount == 0 &&
        m_nMCount == 0 && m_nTCount  == 0 && m_nTpgCount == 0 && m_nWCount == 0 && m_nFrCount == 0) return FALSE;

    // Load check
    switch (enLcomNo)
    {
    case EN_BS5400_NO_01   :   // Combination 1 - Basic
        if(m_nMCount == 0) return FALSE;
        break;
    case EN_BS5400_NO_02_01:   // Combination 2 - Wind only Dead Superimposed Dead 
    case EN_BS5400_NO_02_02:   // Combination 2 - Wind Not only Dead Superimposed Dead 
    case EN_BS5400_NO_02_03:   // Combination 2 - Wind Relieving Effect
        if(m_nWCount == 0) return FALSE;
        break;
    case EN_BS5400_NO_03   :   // Combination 3 - Temperature
        if(m_nTCount == 0 && m_nTpgCount == 0) return FALSE;
        break;
    case EN_BS5400_NO_04_01:   // Combination 4 - Collision
        if (m_nCoCount == 0) return FALSE;
        break;
    case EN_BS5400_NO_04_02:   // Combination 4 - Centrifugal
        if (m_nCfCount == 0) return FALSE;
        if (m_nBD21BridgeType != 0) return FALSE;
        break;
    case EN_BS5400_NO_04_03:   // Combination 4 - Braking
        if(m_nBrkCount == 0) return FALSE;
        if (m_nBD21BridgeType != 0) return FALSE;
        break;
    case EN_BS5400_NO_05   :   // Combination 5 - Friction
        if (m_nFrCount == 0) return FALSE;
        break;
    default:
        ASSERT(0);
        return FALSE;
        break;
    }
    return TRUE;
}

int CLoadCombCtrl::Get_BS5400_LCOM_List(CArray<BS5400_LCOM, BS5400_LCOM&>& raLcomList)
{
    raLcomList.RemoveAll();
    //Strength, Moving, Wind
    BS5400_LCOM BS5400D;
    //ULS
    BS5400D.Set(EN_BS5400_NO_01   , TRUE , TRUE,  FALSE);   raLcomList.Add(BS5400D);// Combination 1 - Basic
    BS5400D.Set(EN_BS5400_NO_02_01, TRUE , FALSE, TRUE);    raLcomList.Add(BS5400D);// Combination 2 - Wind only Dead Superimposed Dead 
    BS5400D.Set(EN_BS5400_NO_02_02, TRUE , TRUE,  TRUE);    raLcomList.Add(BS5400D);// Combination 2 - Wind Not only Dead Superimposed Dead 
    BS5400D.Set(EN_BS5400_NO_02_03, TRUE , TRUE,  TRUE);    raLcomList.Add(BS5400D);// Combination 2 - Wind Relieving Effect
    BS5400D.Set(EN_BS5400_NO_03   , TRUE , TRUE,  FALSE);   raLcomList.Add(BS5400D);// Combination 3 - Temperature
    BS5400D.Set(EN_BS5400_NO_04_01, TRUE , FALSE, FALSE);   raLcomList.Add(BS5400D);// Combination 4 - Collision
    BS5400D.Set(EN_BS5400_NO_04_02, TRUE , FALSE, FALSE);   raLcomList.Add(BS5400D);// Combination 4 - Centrifugal
    BS5400D.Set(EN_BS5400_NO_04_03, TRUE , FALSE, FALSE);   raLcomList.Add(BS5400D);// Combination 4 - Braking
    BS5400D.Set(EN_BS5400_NO_05   , TRUE , FALSE, FALSE);   raLcomList.Add(BS5400D);// Combination 5 - Friction
    //SLS                                        
    BS5400D.Set(EN_BS5400_NO_01   , FALSE, TRUE,  FALSE);   raLcomList.Add(BS5400D);// Combination 1 - Basic
    BS5400D.Set(EN_BS5400_NO_02_01, FALSE, FALSE, TRUE);    raLcomList.Add(BS5400D);// Combination 2 - Wind only Dead Superimposed Dead 
    BS5400D.Set(EN_BS5400_NO_02_02, FALSE, TRUE,  TRUE);    raLcomList.Add(BS5400D);// Combination 2 - Wind Not only Dead Superimposed Dead 
    BS5400D.Set(EN_BS5400_NO_02_03, FALSE, TRUE,  TRUE);    raLcomList.Add(BS5400D);// Combination 2 - Wind Relieving Effect
    BS5400D.Set(EN_BS5400_NO_03   , FALSE, TRUE,  FALSE);   raLcomList.Add(BS5400D);// Combination 3 - Temperature
    BS5400D.Set(EN_BS5400_NO_04_01, FALSE, FALSE, FALSE);   raLcomList.Add(BS5400D);// Combination 4 - Collision
    BS5400D.Set(EN_BS5400_NO_04_02, FALSE, FALSE, FALSE);   raLcomList.Add(BS5400D);// Combination 4 - Centrifugal
    BS5400D.Set(EN_BS5400_NO_04_03, FALSE, FALSE, FALSE);   raLcomList.Add(BS5400D);// Combination 4 - Braking
    BS5400D.Set(EN_BS5400_NO_05   , FALSE, FALSE, FALSE);   raLcomList.Add(BS5400D);// Combination 5 - Friction

    return raLcomList.GetSize();
}

CString CLoadCombCtrl::Add_CvlBS5400_Title (const EN_BS5400_NO& enLcomNo, BOOL bStrength) // 하중조합 생성시 Title 더하기
{

    CString csComb(_T(""));
    CString csStrength(_T(""));
    CString csTitle(_T(""));

    if (bStrength)
    {
        csStrength = _T("ULS ");
    }
    else
    {
        csStrength = _T("SLS ");
    }

    switch (enLcomNo)
    {
    case EN_BS5400_NO_01:
        csComb = _T("Comb 1: ");
        break;
    case EN_BS5400_NO_02_01:
    case EN_BS5400_NO_02_02:
    case EN_BS5400_NO_02_03:
        csComb = _T("Comb 2: ");
        break;
    case EN_BS5400_NO_03:      
        csComb = _T("Comb 3: ");
        break;
    case EN_BS5400_NO_04_01:      
    case EN_BS5400_NO_04_02:      
    case EN_BS5400_NO_04_03:      
        csComb = _T("Comb 4: ");
        break;
    case EN_BS5400_NO_05:      
        csComb = _T("Comb 5: ");
        break;
    default:
        ASSERT(0);
        break;
    }

    csTitle = csStrength + csComb;

    return csTitle;
}


void CLoadCombCtrl::Set_CvlBS5400(const EN_BS5400_NO& enLcomNo, 
                                  ArINT& arDLoad , ArINT& arDwLoad, ArINT& arDcLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arEvLoad,  ArINT& arEhLoad, 
                                  ArINT& arCrLoad, ArINT& arShLoad, ArINT& arPsLoad,  ArUNT& arMLoad,   ArINT& arCfLoad, ArINT& arBrkLoad, ArINT& arCoLoad,
                                  ArINT& arErLoad, ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arWLoad,   ArINT& arFrLoad, ArINT& arCSLoad, BOOL bStrength, BOOL bMoving, BOOL bWind)
{
    if(!Check_BS5400_LoadCase(enLcomNo)) return;

    // Make new load combinations.
    CArray<T_LCOM_D, T_LCOM_D&> aNewLcom;
    aNewLcom.RemoveAll();
    
    Add_CvlBS5400_Title(enLcomNo, bStrength);

    CString sDFac[2], sDwFac[2], sDcFac[2], sEvFac[2], sEhFac[2], sGf3ULSFac, sGf3SLSFac;
    double  dDFac[2], dDwFac[2], dDcFac[2], dEvFac[2], dEhFac[2], dGf3ULS, dGf3SLS;
    double  dStlFactor, dMFactor, dBrCfFactor, dCRSHFactor, dPsFactor, dWFactor, dTFactor, dTpgFactor, dCoFactor, dFrFactor, dErFactor;

    GetBD_21Factor(sDFac[0], sDFac[1], sDwFac[0], sDwFac[1], sDcFac[0], sDcFac[1], sEvFac[0], sEvFac[1], sEhFac[0], sEhFac[1], sGf3ULSFac, sGf3SLSFac,
        dDFac[0], dDFac[1], dDwFac[0], dDwFac[1], dDcFac[0], dDcFac[1], dEvFac[0], dEvFac[1], dEhFac[0], dEhFac[1], dGf3ULS, dGf3SLS, bStrength);

    if (bStrength)
    {
        GetBD_21FactoredPerm(dDFac[0], dDFac[1], dDwFac[0], dDwFac[1], dDcFac[0], dDcFac[1], dEvFac[0], dEvFac[1], dEhFac[0], dEhFac[1], dGf3ULS);
        dStlFactor  = dGf3ULS * 1.2;
        dMFactor    = 1.0;
        dBrCfFactor = dGf3ULS * 1.25;
        dCRSHFactor = dGf3ULS * 1.2;
        dPsFactor   = dGf3ULS * 1.0;
        dTFactor    = dGf3ULS * 1.3;
        dTpgFactor  = dGf3ULS * 1.0;
        dCoFactor   = dGf3ULS * 1.5;
        dFrFactor   = dGf3ULS * 1.3;
        dErFactor   = dGf3ULS * 1.15;

        if (enLcomNo == EN_BS5400_NO_02_01)
        {
            dWFactor    = dGf3ULS * 1.4;
        }
        else if (enLcomNo == EN_BS5400_NO_02_02)
        {
            dWFactor    = dGf3ULS * 1.1;
        }
        else
        {
            dWFactor    = dGf3ULS * 1.0;
        }
    }
    else
    {
        GetBD_21FactoredPerm(dDFac[0], dDFac[1], dDwFac[0], dDwFac[1], dDcFac[0], dDcFac[1], dEvFac[0], dEvFac[1], dEhFac[0], dEhFac[1], dGf3SLS);
        dStlFactor  = dGf3SLS * 1.0;
        dMFactor    = 1.0;
        dBrCfFactor = dGf3SLS * 1.0;
        dCRSHFactor = dGf3SLS * 1.0;
        dPsFactor   = dGf3SLS * 1.0;
        dWFactor    = dGf3SLS * 1.0;
        dTFactor    = dGf3SLS * 1.0;
        dTpgFactor  = dGf3SLS * 0.8;
        dCoFactor   = dGf3SLS * 1.0;
        dFrFactor   = dGf3SLS * 1.0;
        dErFactor   = dGf3SLS * 1.0;

    }

    Add_CvlBS5400_Title    (enLcomNo, bStrength, aNewLcom);
    if (m_nConstLoad != 1)
    {
        Add_CvlBS5400_D        (enLcomNo, bStrength, arDLoad ,  dDFac[0],  dDFac[1],   aNewLcom);
        Add_CvlBS5400_DW       (enLcomNo, bStrength, arDwLoad,  dDwFac[0], dDwFac[1],  aNewLcom);
        Add_CvlBS5400_DC       (enLcomNo, bStrength, arDcLoad,  dDcFac[0], dDcFac[1],  aNewLcom);
    }
    Add_CvlBS5400_EV       (enLcomNo, bStrength, arEvLoad,  dEvFac[0], dEvFac[1],  aNewLcom);
    Add_CvlBS5400_EH       (enLcomNo, bStrength, arEhLoad,  dEhFac[0], dEhFac[1],  aNewLcom);
    Add_CvlBS5400_STL_SM   (enLcomNo, bStrength, arStlLoad, arSmLoad,  dStlFactor, aNewLcom);
    
    if (bMoving)
    {
        Add_CvlBS5400_M (enLcomNo, bStrength, arMLoad, dMFactor, dGf3ULS, dGf3SLS, aNewLcom);
    }

    if (bWind)
    {
        Add_CvlBS5400_W (enLcomNo, bStrength, arWLoad, dWFactor, aNewLcom);
    }
      
    Add_CvlBS5400_T_TPG (enLcomNo, bStrength, arTLoad, arTpgLoad, dTFactor, dTpgFactor, aNewLcom);
    Add_CvlBS5400_CO (enLcomNo, bStrength, arCoLoad, dCoFactor, aNewLcom);
    Add_CvlBS5400_CF_BRK (enLcomNo, bStrength, arMLoad, arCfLoad, arBrkLoad, dBrCfFactor, aNewLcom);
    Add_CvlBS5400_FR (enLcomNo, bStrength, arFrLoad, dFrFactor, aNewLcom);
    Add_CvlBS5400_PS_CR_SH (enLcomNo, bStrength, arPsLoad , arCrLoad, arShLoad, dPsFactor, dCRSHFactor, aNewLcom);

    Add_CvlBS5400_CS_D  (enLcomNo, bStrength, arCSLoad, dDFac[0],  dDFac[1],  aNewLcom);
    Add_CvlBS5400_CS_DW (enLcomNo, bStrength, arCSLoad, dDwFac[0], dDwFac[1], aNewLcom);
    Add_CvlBS5400_CS_DC (enLcomNo, bStrength, arCSLoad, dDcFac[0], dDcFac[1], aNewLcom);

    if (bWind)
    {
        Add_CvlBS5400_CS_W (enLcomNo, bStrength, arCSLoad, dErFactor, aNewLcom);
    }
    
    Add_CvlBS5400_CS_T (enLcomNo, bStrength, arCSLoad, dTFactor,  dTpgFactor, dErFactor, aNewLcom);
    Add_CvlBS5400_CS_PSCRSH (enLcomNo, bStrength, arCSLoad, dPsFactor, dCRSHFactor, aNewLcom);

    // Add new load combinations.
    int nNewLcom = aNewLcom.GetSize();
    for(int i=0; i<nNewLcom; ++i)
    {
        if(aNewLcom[i].aCombination.GetSize() == 0) continue;

        if(m_nLcomType == D_LCOMTYPE_CONCRETE) 
        {
            if (bStrength)
            {
                aNewLcom[i].nActive = 1;
            }
            else
            {
                aNewLcom[i].nActive = 2;
            }
        }
        else if (m_nLcomType == D_LCOMTYPE_GENERAL)
        {
            aNewLcom[i].nActive = 1;
        }

        if (!ExistInNewLcom(aNewLcom[i]))
        {
            if (bStrength)
            {
                Set_LoadComb4CivilName_proxy(aNewLcom[i], D_STR);
            }
            else
            {
                switch ( enLcomNo )
                {
                case EN_BS5400_NO_01:
                    aNewLcom[i].nServLcomType = 1;
                    break;
                case EN_BS5400_NO_02_01:
                case EN_BS5400_NO_02_02:
                case EN_BS5400_NO_02_03:
                case EN_BS5400_NO_03:
                case EN_BS5400_NO_04_01:
                case EN_BS5400_NO_04_02:
                case EN_BS5400_NO_04_03:
                case EN_BS5400_NO_05:
                    aNewLcom[i].nServLcomType = 2;
                    break;

                default:
                    break;
                }

                Set_LoadComb4CivilName_proxy(aNewLcom[i], D_SER);
            }
        }
    }
}


// Add Title
void CLoadCombCtrl::Add_CvlBS5400_Title(const EN_BS5400_NO& enLcomNo, BOOL bStrength, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{

    CString strTitle;
    strTitle = Add_CvlBS5400_Title(enLcomNo, bStrength);

    int nLcom = raNewLcom.GetSize();
    if(nLcom==0)
    {
        T_LCOM_D NewLcomD;
        NewLcomD.Initialize();
        raNewLcom.Add(NewLcomD);
        nLcom = 1;
    }

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        // Description

        CString csPrefix(_T("")), csLcom(_T(""));
        LcomD.Description += strTitle; // 맨 처음일 때만 필요
        raTotalLcom.Add(LcomD);

    }
    raNewLcom.Copy(raTotalLcom);
}

// Add D Load
void CLoadCombCtrl::Add_CvlBS5400_D(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arDLoad, double& dD1, double& dD2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nDCount==0) return;
    
    CString strTitle;
    strTitle = Add_CvlBS5400_Title(enLcomNo, bStrength);

    double dDFac[2];
    dDFac[0] = dD1;
    dDFac[1] = dD2;

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nD = 0; nD  < m_nBD21DNum; ++nD)  
        {
            T_LCOM_D LcomD = raNewLcom[i];
            _LCOM_BOTH BothD; BothD.Initialize();
            int nExistCase = LcomD.aCombination.GetSize();
            int nDL=0, nCaseNum=nExistCase;
            for(int j=0; j<m_nDCount; ++j) {nDL  += Set_STLCIncludeCS(arDLoad.GetAt(j),  dDFac[nD], nCaseNum, LcomD, D_SGLD_DL_CVL); (nD==1) ? BothD.nBoth[nCaseNum]=1 : BothD.nBoth[nCaseNum] = 0; }

            // Description
            if(nDL > 0)
            {   
                CString csPrefix(_T("")), csLcom(_T(""));
                csLcom += csPrefix + GetFactorString(dDFac[nD])  + _T("D"); csPrefix = _T("+");
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }

            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }

    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlBS5400_DW(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arDwLoad, double& dDw1, double& dDw2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nDwCount==0) return;

    CString strTitle;
    strTitle = Add_CvlBS5400_Title(enLcomNo, bStrength);

    double dDwFac[2];
    dDwFac[0] = dDw1;
    dDwFac[1] = dDw2;

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nDw  = 0; nDw  < m_nBD21DWNum; ++nDw)  
        {
            T_LCOM_D LcomD = raNewLcom[i];
            _LCOM_BOTH BothD; BothD.Initialize();
            int nExistCase = LcomD.aCombination.GetSize();
            int nDwL = 0, nCaseNum=nExistCase;
            for(int j=0; j<m_nDwCount; ++j) {nDwL += Set_STLCIncludeCS(arDwLoad.GetAt(j), dDwFac[nDw], nCaseNum, LcomD, D_SGLD_DL_CVL); (nDw==1)? BothD.nBoth[nCaseNum]=1 : BothD.nBoth[nCaseNum] = 0; }

            // Description
            if(nDwL  > 0)
            {   
                CString csPrefix(_T("")), csLcom(_T(""));
                csLcom += csPrefix + GetFactorString(dDwFac[nDw])+ _T("DW"); csPrefix = _T("+");
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }

            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlBS5400_DC(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arDcLoad, double& dDc1, double& dDc2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nDcCount==0) return;

    CString strTitle;
    strTitle = Add_CvlBS5400_Title(enLcomNo, bStrength);    

    double dDcFac[2];
    dDcFac[0] = dDc1;
    dDcFac[1] = dDc2;

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nDc   = 0; nDc  < m_nBD21DCNum; ++nDc)  
        {
            T_LCOM_D LcomD = raNewLcom[i];
            _LCOM_BOTH BothD; BothD.Initialize();
            int nExistCase = LcomD.aCombination.GetSize();
            int nDcL = 0, nCaseNum=nExistCase;
            for(int j=0; j<m_nDcCount; ++j) {nDcL += Set_STLCIncludeCS(arDcLoad.GetAt(j), dDcFac[nDc], nCaseNum, LcomD, D_SGLD_DL_CVL); (nDc==1)? BothD.nBoth[nCaseNum]=1 : BothD.nBoth[nCaseNum] = 0; }

            // Description
            if(nDcL > 0)
            {   
                CString csPrefix(_T("")), csLcom(_T(""));
                csLcom += csPrefix + GetFactorString(dDcFac[nDc])+ _T("DC"); csPrefix = _T("+");
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }

            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlBS5400_EV(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arEvLoad, double& dEv1, double& dEv2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if (enLcomNo == EN_BS5400_NO_02_01) return;
    if(m_nEvCount==0) return;

    CString strTitle;
    strTitle = Add_CvlBS5400_Title(enLcomNo, bStrength);

    double dEvFac[2];
    dEvFac[0] = dEv1;
    dEvFac[1] = dEv2;

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nEv   = 0; nEv  < m_nBD21EVNum; ++nEv)  
        {
            T_LCOM_D LcomD = raNewLcom[i];
            _LCOM_BOTH BothD; BothD.Initialize();
            int nExistCase = LcomD.aCombination.GetSize();
            int nEvL=0, nCaseNum=nExistCase;
            for(int j=0; j<m_nEvCount; ++j) {nEvL  += Set_STLCIncludeCS(arEvLoad.GetAt(j), dEvFac[nEv], nCaseNum, LcomD, 0); (nEv==1) ? BothD.nBoth[nCaseNum]=1 : BothD.nBoth[nCaseNum] = 0; }
            // Description
            if(nEvL > 0)
            {   
                CString csPrefix(_T("")), csLcom(_T(""));
                csLcom += csPrefix + GetFactorString(dEvFac[nEv])+ _T("EV"); csPrefix = _T("+");
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }

            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlBS5400_EH(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arEhLoad, double& dEh1, double& dEh2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if (enLcomNo == EN_BS5400_NO_02_01) return;
    if(m_nEhCount==0) return;

    CString strTitle;
    strTitle = Add_CvlBS5400_Title(enLcomNo, bStrength);

    double dEhFac[2];
    dEhFac[0] = dEh1;
    dEhFac[1] = dEh2;

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nEh = 0; nEh < m_nBD21EHNum; ++nEh)  
        {
            T_LCOM_D LcomD = raNewLcom[i];
            _LCOM_BOTH BothD; BothD.Initialize();
            int nExistCase = LcomD.aCombination.GetSize();
            int nEhL=0, nCaseNum=nExistCase;
            for(int j=0; j<m_nEhCount; ++j) {nEhL  += Set_STLCIncludeCS(arEhLoad.GetAt(j), dEhFac[nEh], nCaseNum, LcomD, 0); (nEh==1)? BothD.nBoth[nCaseNum]=1 : BothD.nBoth[nCaseNum] = 0;}
            // Description
            if(nEhL > 0)
            {   
                CString csPrefix(_T("")), csLcom(_T(""));
                csLcom += csPrefix + GetFactorString(dEhFac[nEh])+ _T("EH"); csPrefix = _T("+");
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }

            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlBS5400_STL_SM(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arStlLoad, ArUNT& arSmLoad, double& dStlFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if (enLcomNo == EN_BS5400_NO_02_01) return;
    if(m_nSmCount==0 && m_nStlCount==0) return;

    CString strTitle;
    strTitle = Add_CvlBS5400_Title(enLcomNo, bStrength);

    int nSmCountTemp   = max(m_nSmCount, 1);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nSm = 0; nSm < nSmCountTemp; ++nSm)  
        {
            T_LCOM_D LcomD = raNewLcom[i];
            _LCOM_BOTH BothD; BothD.Initialize();
            int nExistCase = LcomD.aCombination.GetSize();
            int nStL=0, nCaseNum=nExistCase;
            for(int j=0; j<m_nStlCount; ++j) nStL  += Set_STLCIncludeCS(arStlLoad.GetAt(j), dStlFactor, nCaseNum, LcomD, 0);
            if(m_nSmCount > 0)                        Set_SettleCase(arSmLoad.GetAt(nSm),   dStlFactor, nCaseNum, LcomD, TRUE);
            // Description
            if(nStL > 0 || m_nSmCount > 0)
            {
                CString csPrefix(_T("")), csLcom(_T(""));
                CString strFixSm;
                strFixSm.Format(_T("%d"), nSm+1);
                if(nStL       > 0) { csLcom += csPrefix + GetFactorString(dStlFactor) + _T("STL"); csPrefix = _T("+"); }
                if(m_nSmCount > 0) { csLcom += csPrefix + GetFactorString(dStlFactor) + _T("SM[" + strFixSm + _T("]")); csPrefix = _T("+"); }
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }

            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlBS5400_PS_CR_SH(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arPsLoad , ArINT& arCrLoad,  ArINT& arShLoad, double& dPsFactor, double& dCRSHFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nConstLoad==1) return;
    if(m_nPsCount==0 && m_nCrCount==0 && m_nShCount==0) return;

    CString strTitle;
    strTitle = Add_CvlBS5400_Title(enLcomNo, bStrength);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;
    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        _LCOM_BOTH BothD; BothD.Initialize();
        int nExistCase = LcomD.aCombination.GetSize();
        int nPsL=0, nCrL=0, nShL=0, nCaseNum=nExistCase;
        for(int j=0; j<m_nPsCount; ++j) nPsL  += Set_STLCIncludeCS(arPsLoad.GetAt(j), dPsFactor,   nCaseNum, LcomD, 0);
        for(int j=0; j<m_nCrCount; ++j) nCrL  += Set_STLCIncludeCS(arCrLoad.GetAt(j), dCRSHFactor, nCaseNum, LcomD, 0);
        for(int j=0; j<m_nShCount; ++j) nShL  += Set_STLCIncludeCS(arShLoad.GetAt(j), dCRSHFactor, nCaseNum, LcomD, 0);
        // Description
        if(nPsL > 0 || nCrL > 0 || nShL > 0)
        {
            CString csPrefix(_T("")), csLcom(_T(""));
            if(nPsL > 0) { csLcom += csPrefix + GetFactorString(dPsFactor)   + _T("PS"); csPrefix = _T("+");}
            if(nCrL > 0) { csLcom += csPrefix + GetFactorString(dCRSHFactor) + _T("CR"); csPrefix = _T("+");}
            if(nShL > 0) { csLcom += csPrefix + GetFactorString(dCRSHFactor) + _T("SH"); csPrefix = _T("+");}
            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;
        }
        if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlBS5400_M(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArUNT& arMLoad, double& dMFactor, double& dGf3ULS, double& dGf3SLS, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nMCount==0) return;

    CString strTitle;
    strTitle = Add_CvlBS5400_Title(enLcomNo, bStrength);

    int nMCount = max(m_nMCount, 1);

    int nLcom = raNewLcom.GetSize();

    double dGf3 = 0.0;
    double dPede = 0.0;

    if (bStrength)
    {
        dGf3 = dGf3ULS;
        if (enLcomNo == EN_BS5400_NO_01)
        {
            dPede = 1.5; //Pedestrian 인 경우 ULS에서 Moving Factor는 1.5
        }
        else
        {
            dPede = 1.25; //Pedestrian 인 경우 ULS에서 Moving Factor는 1.25
        }
    }
    else
    {
        dGf3 = dGf3SLS;
        dPede = 1.0; //Pedestrian 인 경우 SLS에서 Moving Factor는 1.0
    }

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nMv=0; nMv<nMCount; ++nMv)  
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nCaseNum=nExistCase;
            BOOL bMoving = FALSE;
            if (m_nBD21BridgeType == 0) //RoadBridge
            {                                
                if(m_nMCount>0) bMoving = Exist_MovingLoadData(arMLoad.GetAt(nMv));
                T_MVLDbs_D rDatabs;

                m_pDoc->m_pAttrCtrl->GetMvldbs(arMLoad.GetAt(nMv),rDatabs);

                if (rDatabs.bAutoLiveLoadComb)
                {
                    if (bStrength && rDatabs.nDgnCombFactorType == 1)
                    {
                        bMoving = FALSE;
                    }

                    if (!bStrength && rDatabs.nDgnCombFactorType == 0)
                    {
                        bMoving = FALSE;
                    }
                }

                if(bMoving) dMFactor = Get_BSMovingLoadFactorData(arMLoad.GetAt(nMv), strTitle);
                if(bMoving) dMFactor = dGf3 * dMFactor;
                if(bMoving) Set_MovingCase(arMLoad.GetAt(nMv), dMFactor, nCaseNum, LcomD, TRUE);
            }
            else 
            {
                dMFactor = dGf3 * dPede;
                if(m_nMCount>0) Set_MovingCase(arMLoad.GetAt(nMv), dMFactor, nCaseNum, LcomD, TRUE);
            }
            // Description

            CString csPrefix(_T("")), csLcom(_T(""));
            CString strFixM;
            strFixM.Format(_T("%d"), nMv+1);

            if (m_nBD21BridgeType == 0) //RoadBridge
            {   
                if(bMoving && m_nMCount>0)
                {
                    csLcom += csPrefix + GetFactorString(dMFactor) + _T("M[") + strFixM + _T("]");  csPrefix = _T("+");
                    csPrefix = LcomD.Description.IsEmpty() ? _T("") : _T("+");
                    LcomD.Description += csPrefix + csLcom;
                }
            }
            else //Pedestrian
            {
                if(m_nMCount>0)  
                {
                    csLcom += csPrefix + GetFactorString(dMFactor)  + _T("M[") + strFixM + _T("]");  csPrefix = _T("+");
                    csPrefix = LcomD.Description.IsEmpty() ? _T("") : _T("+");
                    LcomD.Description += csPrefix + csLcom;
                }
            }

            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlBS5400_W(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arWLoad, double& dWFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nConstLoad==0 && m_nWCount==0) return;

    CString strTitle;
    strTitle = Add_CvlBS5400_Title(enLcomNo, bStrength);

    int nLcom = raNewLcom.GetSize();

    int nWCountTemp = (m_nWCount>0) ? m_nWCount*2 : 1;

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nWm = 0; nWm < nWCountTemp; ++nWm)  
        {
            T_LCOM_D LcomD = raNewLcom[i];
            _LCOM_BOTH BothD; BothD.Initialize();
            int nExistCase = LcomD.aCombination.GetSize();
            int nWL=0, nCaseNum=nExistCase;
            double dWF = (nWm%2==0) ? dWFactor : -1.0 * dWFactor;
            if(m_nWCount > 0) nWL += Set_STLCIncludeCS(arWLoad.GetAt(nWm/2), dWF, nCaseNum, LcomD, 0);          
            // Description
            if(nWL > 0)
            {
                CString csPrefix(_T("")), csLcom(_T(""));
                CString strFixW;
                strFixW.Format(_T("%d"), nWm/2+1);
                csLcom += csPrefix + GetFactorString(dWF) + _T("W[") + strFixW + _T("]"); csPrefix = _T("+");
                if (dWF < 0.0)
                {
                    csPrefix = _T("");
                }
               
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }
            if(nCaseNum > 0) raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlBS5400_T_TPG(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arTLoad, ArINT& arTpgLoad, double& dTFactor, double& dTpgFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if (enLcomNo != EN_BS5400_NO_03) return;
    if(m_nConstLoad==0 && m_nTCount==0 && m_nTpgCount==0) return;

    CString strTitle;
    strTitle = Add_CvlBS5400_Title(enLcomNo, bStrength);

    int nLcom = raNewLcom.GetSize();

    int nTCountTemp = max(m_nTCount, 1);
    int nTpgCountTemp = max(m_nTpgCount, 1);

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nTm = 0; nTm < nTCountTemp; ++nTm) 
        {
            for(int nTpgm = 0; nTpgm < nTpgCountTemp; ++nTpgm)
            {
                T_LCOM_D LcomD = raNewLcom[i];
                _LCOM_BOTH BothD; BothD.Initialize();
                int nExistCase = LcomD.aCombination.GetSize();
                int nTL=0, nTpgL=0, nCaseNum=nExistCase;
                if(m_nTCount > 0)   nTL   += Set_STLCIncludeCS(arTLoad.GetAt(nTm),      dTFactor, nCaseNum, LcomD, 0);
                if(m_nTpgCount > 0) nTpgL += Set_STLCIncludeCS(arTpgLoad.GetAt(nTpgm),  dTpgFactor, nCaseNum, LcomD, 0);

                // Description
                if(nTL > 0 || nTpgL > 0)
                {
                    CString csPrefix(_T("")), csLcom(_T(""));
                    CString strFixT, strFixTpg;
                    strFixT.Format(_T("%d"), nTm+1);
                    strFixTpg.Format(_T("%d"), nTpgm+1);
                    if(nTL  > 0) {csLcom += csPrefix + GetFactorString(dTFactor)     + _T("T[") + strFixT + _T("]"); csPrefix = _T("+");}
                    if(nTpgL> 0) {csLcom += csPrefix + GetFactorString(dTpgFactor)   + _T("TPG[") + strFixTpg + _T("]"); csPrefix = _T("+");}
                    if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                    {
                        csPrefix = _T("");
                    }
                    LcomD.Description += csPrefix + csLcom;
                }
                if(nCaseNum > 0) raTotalLcom.Add(LcomD);
            }
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlBS5400_CO(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arCoLoad, double& dCoFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if (enLcomNo != EN_BS5400_NO_04_01) return;
    if(m_nCoCount==0) return;

    CString strTitle;
    strTitle = Add_CvlBS5400_Title(enLcomNo, bStrength);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;
    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        _LCOM_BOTH BothD; BothD.Initialize();
        int nExistCase = LcomD.aCombination.GetSize();
        int nCoL=0, nCaseNum=nExistCase;
        for(int j=0; j<m_nCoCount; ++j)  nCoL += Set_STLCIncludeCS(arCoLoad.GetAt(j), dCoFactor,   nCaseNum, LcomD, 0);

        // Description
        if(nCoL > 0)
        {
            CString csPrefix(_T("")), csLcom(_T(""));
            if(nCoL > 0) { csLcom += csPrefix + GetFactorString(dCoFactor)   + _T("CO"); csPrefix = _T("+");}
            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;
        }
        if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
    }
    raNewLcom.Copy(raTotalLcom);
}

void CLoadCombCtrl::Add_CvlBS5400_CF_BRK(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArUNT& arMLoad, ArINT& arCfLoad, ArINT& arBrkLoad, double& dBrCfFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if (enLcomNo != EN_BS5400_NO_04_02 && enLcomNo != EN_BS5400_NO_04_03) return;
    if(m_nCfCount==0 && m_nBrkCount==0) return;
    
    CString strTitle;
    strTitle = Add_CvlBS5400_Title(enLcomNo, bStrength);

    int nLcom = raNewLcom.GetSize();

    int nMCount = max(m_nMCount, 1);

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;
    for(int i=0; i<nLcom; ++i)
    {
        for(int nMv=0; nMv<nMCount; ++nMv)  
        {
            T_LCOM_D LcomD = raNewLcom[i];
            _LCOM_BOTH BothD; BothD.Initialize();
            int nExistCase = LcomD.aCombination.GetSize();
            int nCfL=0, nBrL=0, nCaseNum=nExistCase;
            BOOL bMoving = FALSE;
            if(m_nMCount>0) bMoving = Exist_MovingLoadData(arMLoad.GetAt(nMv));
            if(bMoving)
            {
                switch (enLcomNo)
                {
                case EN_BS5400_NO_04_02:
                    for(int j=0; j<m_nCfCount; ++j)  nCfL += Set_STLCIncludeCS(arCfLoad.GetAt(j), dBrCfFactor, nCaseNum, LcomD, 0);
                    break;
                case EN_BS5400_NO_04_03:
                    for(int j=0; j<m_nBrkCount; ++j) nBrL += Set_STLCIncludeCS(arBrkLoad.GetAt(j), dBrCfFactor, nCaseNum, LcomD, 0);
                    break;
                default:
                    break;
                }

                // Description
                if(nCfL > 0 || nBrL > 0)
                {
                    CString csPrefix(_T("")), csLcom(_T(""));
                    switch (enLcomNo)
                    {
                    case EN_BS5400_NO_04_02:
                        if(nCfL > 0) { csLcom += csPrefix + GetFactorString(dBrCfFactor) + _T("CF"); csPrefix = _T("+");}
                        break;
                    case EN_BS5400_NO_04_03:
                        if(nBrL > 0) { csLcom += csPrefix + GetFactorString(dBrCfFactor) + _T("BRK"); csPrefix = _T("+");}
                        break;
                    default:
                        break;
                    }

                    if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                    {
                        csPrefix = _T("");
                    }
                    LcomD.Description += csPrefix + csLcom;
                }
            }
            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlBS5400_FR(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arFrLoad, double& dFrFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{    
    if (enLcomNo != EN_BS5400_NO_05) return;
    if(m_nFrCount==0) return;

    CString strTitle;
    strTitle = Add_CvlBS5400_Title(enLcomNo, bStrength);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;
    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        _LCOM_BOTH BothD; BothD.Initialize();
        int nExistCase = LcomD.aCombination.GetSize();
        int nFrL=0, nCaseNum=nExistCase;
        for(int j=0; j<m_nFrCount; ++j)  nFrL += Set_STLCIncludeCS(arFrLoad.GetAt(j), dFrFactor,   nCaseNum, LcomD, 0);

        // Description
        if(nFrL > 0)
        {
            CString csPrefix(_T("")), csLcom(_T(""));
            if(nFrL > 0) { csLcom += csPrefix + GetFactorString(dFrFactor)   + _T("FR"); csPrefix = _T("+");}
            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;
        }
        if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
    }
    raNewLcom.Copy(raTotalLcom);
}

// Add Construction stage load

void CLoadCombCtrl::Add_CvlBS5400_CS_W(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dErFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nConstLoad==0 || m_nCSCount==0) return; // Static only or no CS

    CString strTitle;
    strTitle = Add_CvlBS5400_Title(enLcomNo, bStrength);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();
        int nn=0, nCaseNum=nExistCase;
        double dFactor = 0.0;
        for(int j=0; j<nExistCase; ++j )
        {
            if(LcomD.aCombination[j].AnalType == 1 && LcomD.aCombination[j].LoadCaseKey == 37)
            {
                dFactor = LcomD.aCombination[j].Factor;
                break;
            }
        }

        CString strcEL;
        int nErection = 0;

        for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
        {
            strcEL = _T(""); nErection = 0;
            if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

            if(CheckCSEL(nErection, _T("W")))
                Set_CStageCase(arCSLoad.GetAt(nn), dFactor, nCaseNum, LcomD, TRUE);

            if (enLcomNo != EN_BS5400_NO_02_01)
            {
                if(CheckCSEL(nErection, _T("ER")))
                    Set_CStageCase(arCSLoad.GetAt(nn), dErFactor, nCaseNum, LcomD, TRUE);
            }
        }

        CString csPrefix(_T("")), csLcom(_T(""));

        nErection = 0;

        for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
        {
            strcEL = _T(""); nErection = 0;
            if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

            if(CheckCSEL(nErection, _T("W")))
            {csLcom += csPrefix + GetFactorString(dFactor) + strcEL; csPrefix = _T("+");}

            if (enLcomNo != EN_BS5400_NO_02_01)
            {
                if(CheckCSEL(nErection, _T("ER")))
                {csLcom += csPrefix + GetFactorString(dErFactor) + strcEL; csPrefix = _T("+");}
            }
        }

        if (csLcom.IsEmpty() || LcomD.Description == strTitle)
        {
            csPrefix = _T("");
        }
        LcomD.Description += csPrefix + csLcom;

        if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
    }
    raNewLcom.Copy(raTotalLcom);
}

void CLoadCombCtrl::Add_CvlBS5400_CS_T(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dTFactor, double& dTpgFactor, double& dErFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if (enLcomNo != EN_BS5400_NO_03) return;
    if(m_nConstLoad==0 || m_nCSCount==0) return; // Static only or no CS

    CString strTitle;
    strTitle = Add_CvlBS5400_Title(enLcomNo, bStrength);

    int nLcom = raNewLcom.GetSize();

    int nTCountTemp = max(m_nTCount, 1);
    int nTpgCountTemp = max(m_nTpgCount, 1);

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();
        int nn=0, nCaseNum=nExistCase;


            CString strcEL;
            int nErection = 0;

            for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
            {
                strcEL = _T(""); nErection = 0;
                if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

                if(CheckCSEL(nErection, _T("T")))
                    Set_CStageCase(arCSLoad.GetAt(nn), dTFactor, nCaseNum, LcomD, TRUE);
                if(CheckCSEL(nErection, _T("TPG")))
                    Set_CStageCase(arCSLoad.GetAt(nn), dTpgFactor, nCaseNum, LcomD, TRUE);
                if(CheckCSEL(nErection, _T("ER")))
                    Set_CStageCase(arCSLoad.GetAt(nn), dErFactor, nCaseNum, LcomD, TRUE);
            }
        


            CString csPrefix(_T("")), csLcom(_T(""));

            nErection = 0;

            for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
            {
                strcEL = _T(""); nErection = 0;
                if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

                if(CheckCSEL(nErection, _T("T")))
                {csLcom += csPrefix + GetFactorString(dTFactor) + strcEL; csPrefix = _T("+");}
                if(CheckCSEL(nErection, _T("TPG")))
                {csLcom += csPrefix + GetFactorString(dTFactor) + strcEL; csPrefix = _T("+");}
                if(CheckCSEL(nErection, _T("ER")))
                {csLcom += csPrefix + GetFactorString(dErFactor) + strcEL; csPrefix = _T("+");}
            }
            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;
        
        if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
    }
    raNewLcom.Copy(raTotalLcom);
}

void CLoadCombCtrl::Add_CvlBS5400_CS_PSCRSH(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dPsFactor,  double& dCRSHFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nConstLoad==0 || m_nCSCount==0) return; // Static only or no CS

    CString strTitle;
    strTitle = Add_CvlBS5400_Title(enLcomNo, bStrength);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();
        int nn=0, nCaseNum=nExistCase;

        if(m_nConstLoad!=0)
        {
            CString strcEL;
            int nErection = 0;

            for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
            {
                strcEL = _T(""); nErection = 0;
                if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }
            }

            if ( !bStrength )
            {
                if ( CheckCS_SgldK(D_SGLD_TP_CVL, nn) )  Set_CStageCase(arCSLoad.GetAt(nn), dPsFactor, nCaseNum, LcomD, TRUE);
            }

            if ( CheckCS_SgldK(D_SGLD_TS_CVL, nn) )  Set_CStageCase(arCSLoad.GetAt(nn), dPsFactor, nCaseNum, LcomD, TRUE);
            if ( CheckCS_SgldK(D_SGLD_CS_CVL, nn) )  Set_CStageCase(arCSLoad.GetAt(nn), dCRSHFactor, nCaseNum, LcomD, TRUE);
            if ( CheckCS_SgldK(D_SGLD_SS_CVL, nn) )  Set_CStageCase(arCSLoad.GetAt(nn), dCRSHFactor, nCaseNum, LcomD, TRUE);
        }

        if(m_nConstLoad!=0)
        {
            CString csPrefix(_T("")), csLcom(_T(""));

            if ( !bStrength )
            {
                if ( CheckCS_SgldK(D_SGLD_TP_CVL, nn) )
                {
                    csLcom += csPrefix + GetFactorString(dPsFactor)   + _T("(cTP)"); csPrefix = _T("+");
                }
            }

            if ( CheckCS_SgldK(D_SGLD_TS_CVL, nn) )
            {
                csLcom += csPrefix + GetFactorString(dPsFactor)   + _T("(cTs)"); csPrefix = _T("+");
            }
            if ( CheckCS_SgldK(D_SGLD_CS_CVL, nn) )
            {
                csLcom += csPrefix + GetFactorString(dCRSHFactor) + _T("(cCR)"); csPrefix = _T("+");
            }
            if ( CheckCS_SgldK(D_SGLD_SS_CVL, nn) )
            {
                csLcom += csPrefix + GetFactorString(dCRSHFactor) + _T("(cSH)"); csPrefix = _T("+");
            }

            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;
        }

        if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
    }
    raNewLcom.Copy(raTotalLcom);
}


void CLoadCombCtrl::Add_CvlBS5400_CS_D(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dD1, double& dD2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nCSCount==0) return;

    CString strTitle;
    strTitle = Add_CvlBS5400_Title(enLcomNo, bStrength);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    int nTempDNum = 0;

    if (m_nConstLoad == 1)
    {
        nTempDNum = 1;
    }
    else
    {
        if (m_nBD21DNum == 2)
        {
            nTempDNum = 2;
        }
        else
        {
            nTempDNum = 1;
        }
    }

    for(int nD = 0; nD  < m_nBD21DNum; ++nD)  
    {
        for(int i=0; i<nLcom; ++i)
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nn=0, nCaseNum=nExistCase;

            double dFactor = 0.0;
            if (nD == 0)
            {
                dFactor = dD1;
            }
            else
            {
                dFactor = dD2;
            }

            if (nTempDNum == 2)
            {
                for(int j=0; j<nExistCase; ++j )
                {
                    if(LcomD.aCombination[j].AnalType == 1 && (LcomD.aCombination[j].Factor == dD1 || LcomD.aCombination[j].Factor == dD2))
                    {
                        dFactor = LcomD.aCombination[j].Factor;
                        break;
                    }
                }
            }


            if(CheckCS_SgldK(D_SGLD_DL_CVL, nn))
            {
                Set_CStageCase(arCSLoad.GetAt(nn), dFactor, nCaseNum, LcomD, TRUE);
            }		

            CString strcEL;
            int nErection = 0;

            for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
            {
                strcEL = _T(""); nErection = 0;
                if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

                if(CheckCSEL(nErection, _T("D")))
                {
                    Set_CStageCase(arCSLoad.GetAt(nn), dFactor, nCaseNum, LcomD, TRUE);
                }
            }

            nErection = 0;

            CString csPrefix(_T("")), csLcom(_T(""));

            if(CheckCS_SgldK(1, nn))
            {csLcom += csPrefix + GetFactorString(dFactor) + _T("(cD)"); csPrefix = _T("+");}

            for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
            {
                strcEL = _T(""); nErection = 0;
                if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

                if(CheckCSEL(nErection, _T("D")))
                {csLcom += csPrefix + GetFactorString(dFactor) + strcEL; csPrefix = _T("+");}
            }
            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;

            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    raNewLcom.Copy(raTotalLcom);
}

void CLoadCombCtrl::Add_CvlBS5400_CS_DW(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dDw1,  double& dDw2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nCSCount==0) return; // Static only or no CS

    CString strTitle;
    strTitle = Add_CvlBS5400_Title(enLcomNo, bStrength);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    int nTempDwNum = 0;

    if (m_nConstLoad == 1)
    {
        nTempDwNum = 1;
    }
    else
    {
        if (m_nBD21DWNum == 2)
        {
            nTempDwNum = 2;
        }
        else
        {
            nTempDwNum = 1;
        }
    }


    for(int nDw = 0; nDw  < m_nBD21DWNum; ++nDw)  
    {
        for(int i=0; i<nLcom; ++i)
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nn=0, nCaseNum=nExistCase;
            double dFactor = 0.0;
            if (nDw == 0)
            {
                dFactor = dDw1;
            }
            else
            {
                dFactor = dDw2;
            }

            if (nTempDwNum == 2)
            {
                for(int j=0; j<nExistCase; ++j )
                {
                    if(LcomD.aCombination[j].AnalType == 1 && (LcomD.aCombination[j].Factor == dDw1 || LcomD.aCombination[j].Factor == dDw2))
                    {
                        dFactor = LcomD.aCombination[j].Factor;
                        break;
                    }
                }
            }


            CString strcEL;
            int nErection = 0;

            for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
            {
                strcEL = _T(""); nErection = 0;
                if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

                if(CheckCSEL(nErection, _T("DW")))
                {
                    Set_CStageCase(arCSLoad.GetAt(nn), dFactor, nCaseNum, LcomD, TRUE);
                }

            }

            nErection = 0;

            CString csPrefix(_T("")), csLcom(_T(""));

            for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
            {
                strcEL = _T(""); nErection = 0;
                if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

                if(CheckCSEL(nErection, _T("DW")))
                {csLcom += csPrefix + GetFactorString(dFactor) + strcEL; csPrefix = _T("+");}                            
            }
            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;
            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    raNewLcom.Copy(raTotalLcom);
}

void CLoadCombCtrl::Add_CvlBS5400_CS_DC(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dDc1,  double& dDc2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nCSCount==0) return; // Static only or no CS

    CString strTitle;
    strTitle = Add_CvlBS5400_Title(enLcomNo, bStrength);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    int nTempDcNum = 0;

    if (m_nConstLoad == 1)
    {
        nTempDcNum = 1;
    }
    else
    {
        if (m_nBD21DCNum == 2)
        {
            nTempDcNum = 2;
        }
        else
        {
            nTempDcNum = 1;
        }
    }


    for(int nDc = 0; nDc  < m_nBD21DCNum; ++nDc)  
    {
        for(int i=0; i<nLcom; ++i)
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nn=0, nCaseNum=nExistCase;

            double dFactor = 0.0;
            if (nDc == 0)
            {
                dFactor = dDc1;
            }
            else
            {
                dFactor = dDc2;
            }

            if (nTempDcNum == 2)
            {
                for(int j=0; j<nExistCase; ++j )
                {
                    if(LcomD.aCombination[j].AnalType == 1 && (LcomD.aCombination[j].Factor == dDc1 || LcomD.aCombination[j].Factor == dDc2))
                    {
                        dFactor = LcomD.aCombination[j].Factor;
                        break;
                    }
                }

            }


            CString strcEL;
            int nErection = 0;

            for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
            {
                strcEL = _T(""); nErection = 0;
                if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

                if(CheckCSEL(nErection, _T("DC")))
                {
                    Set_CStageCase(arCSLoad.GetAt(nn), dFactor, nCaseNum, LcomD, TRUE);
                }

            }

            nErection = 0;

            CString csPrefix(_T("")), csLcom(_T(""));

            for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
            {
                strcEL = _T(""); nErection = 0;
                if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

                if(CheckCSEL(nErection, _T("DC")))
                {csLcom += csPrefix + GetFactorString(dFactor) + strcEL; csPrefix = _T("+");}
            }
            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;

            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    raNewLcom.Copy(raTotalLcom);
}

////////////////////////////////////////////////////////////////////////
////// AASHTO-LRFD 12, 16, 18 by khj0102 2019/09/09         /////////
//////////////////////////////////////////////////////////////////////////

int CLoadCombCtrl::Get_AASHTOSince12_LCOM_List(CArray<AASHTOSince12_LCOM, AASHTOSince12_LCOM&>& raLcomList)
{
    raLcomList.RemoveAll();
    //bStrength, bExtreme, bService, bFatigue, bMoving, bWind
    AASHTOSince12_LCOM AASHTOSince12D;
    //Strength
    AASHTOSince12D.Set(EN_AASHTOSince12_NO_STR1,    TRUE,  FALSE, FALSE, FALSE, TRUE , FALSE, FALSE);  raLcomList.Add(AASHTOSince12D);   // Strength 1 - Moving, Big LiveFactor,   Settle
    AASHTOSince12D.Set(EN_AASHTOSince12_NO_STR2,    TRUE,  FALSE, FALSE, FALSE, TRUE , FALSE, FALSE);  raLcomList.Add(AASHTOSince12D);   // Strength 2 - Moving, Small LiveFactor, Settle
    AASHTOSince12D.Set(EN_AASHTOSince12_NO_STR3,    TRUE,  FALSE, FALSE, FALSE, FALSE, TRUE , FALSE);   raLcomList.Add(AASHTOSince12D);   // Strength 3 - No Moving, Wind, Settle
    AASHTOSince12D.Set(EN_AASHTOSince12_NO_STR4,    TRUE,  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE);  raLcomList.Add(AASHTOSince12D);   // Strength 4 - No Moving
    AASHTOSince12D.Set(EN_AASHTOSince12_NO_STR5,    TRUE,  FALSE, FALSE, FALSE, TRUE , TRUE , FALSE);   raLcomList.Add(AASHTOSince12D);   // Strength 5 - Moving, Small LiveFactor, Wind, Settle
                                                                                
    //Extreme Event                                                             
    AASHTOSince12D.Set(EN_AASHTOSince12_NO_ETR1_01, FALSE, TRUE,  FALSE, FALSE, TRUE, FALSE, FALSE);  raLcomList.Add(AASHTOSince12D);   // Extreme 1 - Seismic load 
    AASHTOSince12D.Set(EN_AASHTOSince12_NO_ETR1_02, FALSE, TRUE,  FALSE, FALSE, TRUE, FALSE, FALSE);  raLcomList.Add(AASHTOSince12D);   // Extreme 1 - Response Spectrum Load
    AASHTOSince12D.Set(EN_AASHTOSince12_NO_ETR2_01, FALSE, TRUE,  FALSE, FALSE, TRUE, FALSE, FALSE);  raLcomList.Add(AASHTOSince12D);   // Extreme 2 - China  빙압력
    AASHTOSince12D.Set(EN_AASHTOSince12_NO_ETR2_02, FALSE, TRUE,  FALSE, FALSE, TRUE, FALSE, FALSE);  raLcomList.Add(AASHTOSince12D);   // Extreme 2 - Vehicle collision load(차량충돌하중)
    AASHTOSince12D.Set(EN_AASHTOSince12_NO_ETR2_03, FALSE, TRUE,  FALSE, FALSE, TRUE, FALSE, FALSE);  raLcomList.Add(AASHTOSince12D);   // Extreme 2 - Vessel collision load(선박충돌하중)
    
    //Seismic
    if (m_bSeismic)
    {
        AASHTOSince12D.Set(EN_AASHTOSince12_NO_SEIS_EQ, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, TRUE);  raLcomList.Add(AASHTOSince12D);   // Extreme 1 - Seismic load 
        AASHTOSince12D.Set(EN_AASHTOSince12_NO_SEIS_RS, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, TRUE);  raLcomList.Add(AASHTOSince12D);   // Extreme 1 - Response Spectrum Load
    }

    //Service                                                                   
    AASHTOSince12D.Set(EN_AASHTOSince12_NO_SER1,    FALSE, FALSE, TRUE,  FALSE, TRUE , TRUE , FALSE);   raLcomList.Add(AASHTOSince12D);   // Service 1 - Temperature Gradient, Wind, Settle
    AASHTOSince12D.Set(EN_AASHTOSince12_NO_SER2,    FALSE, FALSE, TRUE,  FALSE, TRUE , FALSE, FALSE);  raLcomList.Add(AASHTOSince12D);   // Service 2 - No Temperature Gradient, No Wind
    AASHTOSince12D.Set(EN_AASHTOSince12_NO_SER3,    FALSE, FALSE, TRUE,  FALSE, TRUE , FALSE, FALSE);  raLcomList.Add(AASHTOSince12D);   // Service 3 - Temperature Gradient, No Wind, Settle
    AASHTOSince12D.Set(EN_AASHTOSince12_NO_SER4,    FALSE, FALSE, TRUE,  FALSE, FALSE, TRUE , FALSE);   raLcomList.Add(AASHTOSince12D);   // Service 4 - No Temperature Gradient, Wind, No Moving
                                                                                
    //Fatigue                                                                   
    AASHTOSince12D.Set(EN_AASHTOSince12_NO_FAT1,    FALSE, FALSE, FALSE, TRUE,  FALSE, FALSE, FALSE);  raLcomList.Add(AASHTOSince12D);   // Fatigue 1 - No Moving, Big LiveFactor,
    AASHTOSince12D.Set(EN_AASHTOSince12_NO_FAT1,    FALSE, FALSE, FALSE, TRUE,  TRUE , FALSE, FALSE);  raLcomList.Add(AASHTOSince12D);   // Fatigue 1 - Moving, Big LiveFactor,
    AASHTOSince12D.Set(EN_AASHTOSince12_NO_FAT2,    FALSE, FALSE, FALSE, TRUE,  FALSE, FALSE, FALSE);  raLcomList.Add(AASHTOSince12D);   // Fatigue 2 - No Moving, Small LiveFactor,
    AASHTOSince12D.Set(EN_AASHTOSince12_NO_FAT2,    FALSE, FALSE, FALSE, TRUE,  TRUE , FALSE, FALSE);  raLcomList.Add(AASHTOSince12D);   // Fatigue 2 - Moving, Small LiveFactor,

    return raLcomList.GetSize();
}

BOOL CLoadCombCtrl::Check_AASHTOSince12_LoadCase(const EN_AASHTOSince12_NO& enLcomNo)
{
    // Load check

    if (m_nDCount ==0 && m_nDcCount==0 && m_nDwCount==0 && m_nDdCount==0 && 
        m_nEhCount==0 && m_nEvCount==0 && m_nLCount==0  && m_nEsCount==0 && 
        m_nCrCount==0 && m_nShCount==0 && m_nPsCount==0 && m_nConstLoad==0) return FALSE;

    bool bDeadLoadNo = m_nDCount ==0 && m_nDcCount==0 && m_nDwCount==0 && m_nDdCount==0;
    //보도교의 이동하중인 경우 moving으로 안넣고 ll로 넣는 경우가 있어서 ll도 mv로 봐야함.
    switch (enLcomNo)
    {
    case EN_AASHTOSince12_NO_STR1:
    case EN_AASHTOSince12_NO_STR2:
        if ( bDeadLoadNo && m_nLCount==0 && m_nCrlCount==0 && m_nLsCount==0  &&
            m_nMCount==0 && m_nTCount==0   && m_nStlCount==0 && m_nSmCount==0 && m_nConstLoad==0) return FALSE;
        break;
    case EN_AASHTOSince12_NO_STR3:
        if ( bDeadLoadNo &&  m_nLCount==0 && m_nCrlCount==0 && m_nLsCount==0 &&
            m_nWCount==0 && m_nTCount==0 && m_nStlCount==0 && m_nSmCount==0 && m_nConstLoad==0) return FALSE;
        break;
    case EN_AASHTOSince12_NO_STR4:
        if ( bDeadLoadNo && m_nLCount==0 && m_nCrlCount==0 && m_nLsCount==0 && m_nTCount==0 && m_nConstLoad==0) return FALSE;
        break;
    case EN_AASHTOSince12_NO_STR5:
        if ( bDeadLoadNo && m_nLCount==0 && m_nCrlCount==0 && m_nLsCount==0 &&
            m_nMCount==0 && m_nWCount  ==0 && m_nWlCount==0 && 
            m_nTCount==0 && m_nStlCount==0 && m_nSmCount==0 && m_nConstLoad==0) return FALSE;
        break;
    case EN_AASHTOSince12_NO_ETR1_01:
        if (m_nMCount ==0 && m_nLCount == 0 && m_nECount == 0) return FALSE;
        break;
    case EN_AASHTOSince12_NO_ETR1_02:
        if (m_nMCount ==0 && m_nLCount == 0 && m_nEspCount == 0) return FALSE;
        break;
    case EN_AASHTOSince12_NO_SEIS_EQ:
        if ( (m_nMCount ==0 && m_nLCount == 0) || m_nECount == 0) return FALSE;
        break;
    case EN_AASHTOSince12_NO_SEIS_RS:
        if ( (m_nMCount ==0 && m_nLCount == 0) || m_nEspCount == 0) return FALSE;
        break;
    case EN_AASHTOSince12_NO_ETR2_01:
        if (m_nMCount ==0 && m_nLCount == 0 && m_nIpCount == 0) return FALSE;
        break;
    case EN_AASHTOSince12_NO_ETR2_02:
        if (m_nMCount ==0 && m_nLCount == 0 && m_nCtCount == 0) return FALSE;
        break;
    case EN_AASHTOSince12_NO_ETR2_03:
        if (m_nMCount ==0 && m_nLCount == 0 && m_nCvCount == 0) return FALSE;
        break;
    case EN_AASHTOSince12_NO_SER1:
        if (m_nMCount ==0 && m_nLCount == 0 && m_nWCount == 0 && m_nWlCount==0 &&
            m_nTCount ==0 && m_nTpgCount == 0 && m_nStlCount ==0 && m_nSmCount ==0) return FALSE;
        break;
    case EN_AASHTOSince12_NO_SER2:
        if ( m_nDCount > 0 || m_nDcCount > 0 || m_nDwCount > 0 || m_nDdCount > 0 ||
            m_nEhCount > 0 || m_nEvCount > 0 || m_nLCount > 0  || m_nEsCount > 0 ||
            m_nCrCount > 0 || m_nShCount > 0 || m_nPsCount > 0 || m_nConstLoad > 0 ||
            m_nTCount > 0) return TRUE;
        break;
    case EN_AASHTOSince12_NO_SER3:
        if (m_nMCount ==0 && m_nLCount == 0 && m_nTCount ==0 && m_nTpgCount ==0 && m_nStlCount ==0 && m_nSmCount ==0) return FALSE;
        break;
    case EN_AASHTOSince12_NO_SER4:
        if (m_nMCount ==0 && m_nLCount == 0 && m_nWCount ==0 && m_nTCount ==0 && m_nStlCount ==0 && m_nSmCount ==0) return FALSE;
        break;
    case EN_AASHTOSince12_NO_FAT1:
    case EN_AASHTOSince12_NO_FAT2:
        if(m_nLCount==0 && m_nMCount==0 && m_nLCount == 0) return FALSE;
        break;
    default:
        ASSERT(0);
        break;
    }
    return TRUE;
}

CString CLoadCombCtrl::Add_CvlAASHTOSince12_Title(const EN_AASHTOSince12_NO& enLcomNo) // 하중조합 생성시 Title 더하기
{

    CString csTitle(_T(""));

    switch (enLcomNo)
    {
    case EN_AASHTOSince12_NO_STR1:
        csTitle = _T("Strength-I:");
        break;
    case EN_AASHTOSince12_NO_STR2:
        csTitle = _T("Strength-II:");
        break;
    case EN_AASHTOSince12_NO_STR3:
        csTitle = _T("Strength-III:");
        break;
    case EN_AASHTOSince12_NO_STR4:
        csTitle = _T("Strength-IV:");
        break;
    case EN_AASHTOSince12_NO_STR5:
        csTitle = _T("Strength-V:");
        break;
    case EN_AASHTOSince12_NO_ETR1_01:
    case EN_AASHTOSince12_NO_ETR1_02:
        csTitle = _T("Extreme-I::");
        break;
    case EN_AASHTOSince12_NO_ETR2_01:
    case EN_AASHTOSince12_NO_ETR2_02:
    case EN_AASHTOSince12_NO_ETR2_03:
        csTitle = _T("Extreme-II:");
        break;
    case EN_AASHTOSince12_NO_SER1:
        csTitle = _T("Service-I:");
        break;
    case EN_AASHTOSince12_NO_SER2:
        csTitle = _T("Service-II:");
        break;
    case EN_AASHTOSince12_NO_SER3:
        csTitle = _T("Service-III:");
        break;
    case EN_AASHTOSince12_NO_SER4:
        csTitle = _T("Service-IV:");
        break;
    case EN_AASHTOSince12_NO_FAT1:
        csTitle = _T("Fatigue-I:");
        break;
    case EN_AASHTOSince12_NO_FAT2:
        csTitle = _T("Fatigue-II:");
        break;
    case EN_AASHTOSince12_NO_SEIS_EQ:
    case EN_AASHTOSince12_NO_SEIS_RS:
        csTitle = _T("Seismic:");
        break;
    default:
        ASSERT(0);
        break;
    }

    return csTitle;
}

void CLoadCombCtrl::GetAASHTOSince12Factor(CString& sDc1Fac, CString& sDc2Fac, CString& sDd1Fac, CString& sDd2Fac, 
                                           CString& sDw1Fac, CString& sDw2Fac, CString& sEh1Fac, CString& sEh2Fac, CString& sEv1Fac, CString& sEv2Fac, 
                                           CString& sEs1Fac, CString& sEs2Fac, CString& sEl1Fac, CString& sEl2Fac, CString& sPs1Fac, CString& sPs2Fac, 
                                           CString& sCr1Fac, CString& sCr2Fac, CString& sSh1Fac, CString& sSh2Fac, 
                                           double& dDc1, double& dDc2, double& dDd1, double& dDd2, double& dDw1, double& dDw2, 
                                           double& dEh1, double& dEh2, double& dEv1, double& dEv2, double& dEs1, double& dEs2, 
                                           double& dEl1, double& dEl2, double& dPs1, double& dPs2, double& dCr1, double& dCr2, 
                                           double& dSh1, double& dSh2, const EN_AASHTOSince12_NO& enLcomNo, BOOL bStrength)
{  
    double dAshLr12Dc[2], dAshLr12Dd[2], dAshLr12Dw[2], dAshLr12Eh[2], dAshLr12Ev[2], dAshLr12Es[2], dAshLr12El[2];
    double dAshLr12Ps[2], dAshLr12Cr[2], dAshLr12Sh[2];
    // Initialize
    for(int i = 0; i < 2; i++)
    {
        dAshLr12Dc[i] = 0.0;
        dAshLr12Dd[i] = 0.0;
        dAshLr12Dw[i] = 0.0;
        dAshLr12Eh[i] = 0.0;
        dAshLr12Ev[i] = 0.0;
        dAshLr12Es[i] = 0.0;
        dAshLr12El[i] = 0.0;

        dAshLr12Ps[i] = 0.0;
        dAshLr12Cr[i] = 0.0;
        dAshLr12Sh[i] = 0.0;
    }
    // Assign global variable to local variable
    const double dModifier = bStrength ? m_dAshLr12Modifier : 1.0;
	double adModifierFactor[2] = { dModifier, fabs(dModifier)<m_dZeroLim ? 1.0 : min(1.0, 1.0/dModifier)};
    for(int i = 0; i < 2; i++)
    {
        // MQC_ab_5526-1 Extreme Event I 하중조합의 load factor 변경 
        // 2015 INTERIM REVISIONS - DC 하중조건의 경우 GAMMA_P 적용않음. 1.00 적용
        double dDc = m_dAshLr12Dc[i];
        double dDd = m_dAshLr12Dd[i];
        double dDw = m_dAshLr12Dw[i];
        double dEh = m_dAshLr12Eh[i];
        double dEv = m_dAshLr12Ev[i];
        double dEs = m_dAshLr12Es[i];
        double dEl = m_dAshLr12El[i];
        double dPs = m_dAshLr12Ps[i];
        double dCr = m_dAshLr12Cr[i];
        double dSh = m_dAshLr12Sh[i];

		if (enLcomNo == EN_AASHTOSince12_NO_STR4)
		{
            /// Table 3.4.1.2
			if (dgn::EQ(dDc, 1.25)) dDc = 1.50;			
		}

        if(m_bAASHTO_LRFD16)
        {
            switch (enLcomNo)
            {
                case EN_AASHTOSince12_NO_ETR1_01:
                case EN_AASHTOSince12_NO_ETR1_02:
                    dDc = 1.0; // Extreme-I
                    break;
                default:
                    break;
            }
        }
		if (m_bAASHTO_LRFD18 || m_bAASHTO_LRFD20 || m_bAASHTO_LRFD24)
		{
            switch (enLcomNo)
            {
                case EN_AASHTOSince12_NO_ETR1_01:
                case EN_AASHTOSince12_NO_ETR1_02:
                case EN_AASHTOSince12_NO_ETR2_01:
                case EN_AASHTOSince12_NO_ETR2_02:
                case EN_AASHTOSince12_NO_ETR2_03:
				case EN_AASHTOSince12_NO_SEIS_EQ:
				case EN_AASHTOSince12_NO_SEIS_RS:
                {
                    dDc = 1.0; // Extreme-I, II
                    dDd = 1.0;
                    dDw = 1.0;
                    dEh = 1.0;
                    dEv = 1.0;
                    dEs = 1.0;
                    dEl = 1.0;
                    dPs = 1.0;
                    dCr = 1.0;
                    dSh = 1.0;
                }
                    break;
                default:
                    break;
            }
		}

        dAshLr12Dc[i] = dDc * adModifierFactor[i];
        dAshLr12Dd[i] = dDd * adModifierFactor[i];
        dAshLr12Dw[i] = dDw * adModifierFactor[i];
        dAshLr12Eh[i] = dEh * adModifierFactor[i];
        dAshLr12Ev[i] = dEv * adModifierFactor[i];
        dAshLr12Es[i] = dEs * adModifierFactor[i];
        dAshLr12El[i] = dEl * adModifierFactor[i];
        dAshLr12Ps[i] = dPs * adModifierFactor[i];
        dAshLr12Cr[i] = dCr * adModifierFactor[i];
        dAshLr12Sh[i] = dSh * adModifierFactor[i];
    }

    dDc1 = dAshLr12Dc[0];  sDc1Fac.Format(_T("%.2f"), dAshLr12Dc[0]);
    dDc2 = dAshLr12Dc[1];  sDc2Fac.Format(_T("%.2f"), dAshLr12Dc[1]);
    dDd1 = dAshLr12Dd[0];  sDd1Fac.Format(_T("%.2f"), dAshLr12Dd[0]);
    dDd2 = dAshLr12Dd[1];  sDd2Fac.Format(_T("%.2f"), dAshLr12Dd[1]);
    dDw1 = dAshLr12Dw[0];  sDw1Fac.Format(_T("%.2f"), dAshLr12Dw[0]);
    dDw2 = dAshLr12Dw[1];  sDw2Fac.Format(_T("%.2f"), dAshLr12Dw[1]);
    dEh1 = dAshLr12Eh[0];  sEh1Fac.Format(_T("%.2f"), dAshLr12Eh[0]);
    dEh2 = dAshLr12Eh[1];  sEh2Fac.Format(_T("%.2f"), dAshLr12Eh[1]);
    dEv1 = dAshLr12Ev[0];  sEv1Fac.Format(_T("%.2f"), dAshLr12Ev[0]);
    dEv2 = dAshLr12Ev[1];  sEv2Fac.Format(_T("%.2f"), dAshLr12Ev[1]);
    dEs1 = dAshLr12Es[0];  sEs1Fac.Format(_T("%.2f"), dAshLr12Es[0]);
    dEs2 = dAshLr12Es[1];  sEs2Fac.Format(_T("%.2f"), dAshLr12Es[1]);  
    dEl1 = dAshLr12El[0];  sEl1Fac.Format(_T("%.2f"), dAshLr12El[0]);
    dEl2 = dAshLr12El[1];  sEl2Fac.Format(_T("%.2f"), dAshLr12El[1]);  
    dPs1 = dAshLr12Ps[0];  sPs1Fac.Format(_T("%.2f"), dAshLr12Ps[0]);
    dPs2 = dAshLr12Ps[1];  sPs2Fac.Format(_T("%.2f"), dAshLr12Ps[1]);
    dCr1 = dAshLr12Cr[0];  sCr1Fac.Format(_T("%.2f"), dAshLr12Cr[0]);
    dCr2 = dAshLr12Cr[1];  sCr2Fac.Format(_T("%.2f"), dAshLr12Cr[1]);
    dSh1 = dAshLr12Sh[0];  sSh1Fac.Format(_T("%.2f"), dAshLr12Sh[0]);
    dSh2 = dAshLr12Sh[1];  sSh2Fac.Format(_T("%.2f"), dAshLr12Sh[1]);	
}

// Add Title
void CLoadCombCtrl::Add_CvlAASHTOSince12_Title(const EN_AASHTOSince12_NO& enLcomNo, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{

    CString strTitle;
    strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();
    if(nLcom==0)
    {
        T_LCOM_D NewLcomD;
        NewLcomD.Initialize();
        raNewLcom.Add(NewLcomD);
        nLcom = 1;
    }

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        // Description

        CString csPrefix(_T("")), csLcom(_T(""));
        LcomD.Description += strTitle; // 맨 처음일 때만 필요
        raTotalLcom.Add(LcomD);

    }
    raNewLcom.Copy(raTotalLcom);
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_Trans_Factor (const EN_AASHTOSince12_NO& enLcomNo, BOOL bStrength, double& dCfFactor, double& dLFactor, double& dWFactor, double& dWpFactor, double& dFrFactor, double& dEFactor)
{

    BOOL bAASHTO16 = Get_Chk_AASHTO16();
    BOOL bAASHTO18 = Get_Chk_AASHTO18();
    BOOL bAASHTO20 = Get_Chk_AASHTO20();

	const double dModifier = bStrength ? m_dAshLr12Modifier : 1.0;

    switch (enLcomNo)
    {
    case EN_AASHTOSince12_NO_STR1:
        dCfFactor = 1.75 * dModifier;
        dLFactor = dCfFactor;
        break;
    case EN_AASHTOSince12_NO_STR2:
        dCfFactor = 1.35 * dModifier;
        dLFactor = dCfFactor;
        break;
    case EN_AASHTOSince12_NO_STR3:
        dLFactor = 0.0;
        if (bAASHTO16 || bAASHTO18 || bAASHTO20 )
        {
            dWFactor = 1.0 * dModifier;
        }
        else
        {
            dWFactor = 1.4 * dModifier;
        }
        break;
    case EN_AASHTOSince12_NO_STR4:
        dLFactor = 0.0;
        break;
    case EN_AASHTOSince12_NO_STR5:
        dCfFactor = 1.35 * dModifier;
        dLFactor = dCfFactor;
        if (bAASHTO16 || bAASHTO18 || bAASHTO20)
        {
            dWFactor = 1.0 * dModifier;
        }
        else
        {
            dWFactor = 0.4 * dModifier;
        }
        break;
    case EN_AASHTOSince12_NO_ETR1_01:
    case EN_AASHTOSince12_NO_ETR1_02:
    case EN_AASHTOSince12_NO_ETR2_01:
    case EN_AASHTOSince12_NO_ETR2_02:
    case EN_AASHTOSince12_NO_ETR2_03:
        dLFactor  = 0.5 * dModifier;
        dCfFactor = 0.5 * dModifier;
        dWpFactor = 1.0 * dModifier;
        dFrFactor = 1.0 * dModifier;
        dEFactor  = 1.0 * dModifier;
        break;
    case EN_AASHTOSince12_NO_SEIS_EQ:
    case EN_AASHTOSince12_NO_SEIS_RS:
        dLFactor  = 1.0 * dModifier;
        dCfFactor = 1.0 * dModifier;
        dWpFactor = 1.0 * dModifier;
        dFrFactor = 1.0 * dModifier;
        dEFactor  = 1.0 * dModifier;
        break;
    case EN_AASHTOSince12_NO_SER1:
        dCfFactor = 1.0 * dModifier;
        dLFactor = dCfFactor;
        if (bAASHTO16 || bAASHTO18 || bAASHTO20)
        {
            dWFactor = 1.0 * dModifier;
        }
        else
        {
            dWFactor = 0.3 * dModifier;
        }
        break;
    case EN_AASHTOSince12_NO_SER2:
        dCfFactor = 1.3 * dModifier;
        dLFactor = dCfFactor;
        break;
    case EN_AASHTOSince12_NO_SER3:
        if (bAASHTO16 || bAASHTO18 || bAASHTO20)
        {
            dCfFactor = m_dGamma_LL * dModifier;
        }
        else
        {
            dCfFactor = 0.8 * dModifier;
        }
        dLFactor = dCfFactor;
        break;
    case EN_AASHTOSince12_NO_SER4:
        dLFactor = 0.0;
        if (bAASHTO16 || bAASHTO18 || bAASHTO20)
        {
            dWFactor = 1.0 * dModifier;
        }
        else
        {
            dWFactor = 0.7 * dModifier;
        }
        break;
    case EN_AASHTOSince12_NO_FAT1:
        if (bAASHTO18 || bAASHTO20)
        {
            dCfFactor  = 1.75 * dModifier;
        }
        else
        {
            dCfFactor  = 1.5 * dModifier;
        }
        dLFactor = dCfFactor;
        break;
    case EN_AASHTOSince12_NO_FAT2:
        if (bAASHTO18 || bAASHTO20)
        {
            dCfFactor  = 0.8 * dModifier;
        }
        else
        {
            dCfFactor  = 0.75 * dModifier;
        }
        dLFactor = dCfFactor;
        break;
    default:
        ASSERT(0);
        break;
    }

    if(m_bAshLr12CulvertChk ) dLFactor = 2.0 * dModifier;  // Metal Box Culvert인 경우
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_Temp_Factor (const EN_AASHTOSince12_NO& enLcomNo, BOOL bStrength, double& dTFactor, double& dTpgFactor)
{
    if (bStrength)
    {
        if(m_nAshLr12Deform == 1)        
        {
            dTFactor = 0.5 * m_dAshLr12Modifier;  // All Other Effects인 경우 
        }
    }
    else
    {
        if(m_nAshLr12Deform == 1)        
        {
            dTFactor = 1.0;  // All Other Effects인 경우 
        }
        if (m_nMCount>0 || m_nLCount>0)
        {
            dTpgFactor=0.5; // Live Load or MVL 하나라도 있으면 0.5
        }
        else
        {
            dTpgFactor=1.0; // Live Load or MVL 한개도 없으면 1.0
        }
    }
}

void CLoadCombCtrl::Set_CvlAASHTOSince12(const EN_AASHTOSince12_NO& enLcomNo, 
                                         ArINT& arDcLoad, ArINT& arDdLoad, ArINT& arDwLoad, ArINT& arEhLoad, ArINT& arEvLoad, 
                                         ArINT& arEsLoad, ArINT& arElLoad, ArINT& arLLoad,  ArINT& arIlLoad, ArINT& arWpLoad, ArINT& arSfLoad, 
                                         ArUNT& arMLoad,  ArINT& arCfLoad, ArINT& arBrkLoad,ArINT& arCrlLoad,ArINT& arLsLoad, ArINT& arWlLoad, 
                                         ArINT& arFpLoad, ArINT& arBLoad,  ArINT& arWprLoad,ArINT& arWLoad,  ArINT& arELoad,  ArUNT& arEspLoad,
                                         ArINT& arIpLoad, ArINT& arCtLoad, ArINT& arCvLoad, ArINT& arFrLoad, ArINT& arTLoad,  ArINT& arTpgLoad,
                                         ArINT& arCrLoad, ArINT& arShLoad, ArINT& arPsLoad, ArINT& arStlLoad,ArUNT& arSmLoad, ArINT& arCSLoad, 
                                         BOOL bStrength,  BOOL bExtreme, BOOL bService, BOOL bFatigue, BOOL bMoving, BOOL bWind, BOOL bSeis)
{
    if(!Check_AASHTOSince12_LoadCase(enLcomNo)) return;
    // Make new load combinations.
    CArray<T_LCOM_D, T_LCOM_D&> aNewLcom;
    aNewLcom.RemoveAll();

    Add_CvlAASHTOSince12_Title(enLcomNo);

    CString sDcFac[2], sDdFac[2], sDwFac[2], sEhFac[2], sEvFac[2], sEsFac[2], sElFac[2], sPsFac[2], sCrFac[2], sShFac[2];
    double dDcFac[2], dDdFac[2], dDwFac[2], dEhFac[2], dEvFac[2], dEsFac[2], dElFac[2], dPsFac[2], dCrFac[2], dShFac[2];
    double dModifier, dLFactor, dCfFactor, dWFactor, dWpFactor, dWlFactor, dFrFactor, dTFactor, dTpgFactor, dStlFactor, dEFactor, dDFactor;

    BOOL bAASHTO16 = Get_Chk_AASHTO16();
    BOOL bAASHTO18 = Get_Chk_AASHTO18();

    dModifier = bStrength ? m_dAshLr12Modifier : 1.0;
    dWpFactor = 1.0 * dModifier;
    dWlFactor = 1.0 * dModifier;
    dFrFactor = 1.0 * dModifier; 
    dTFactor  = 1.2 * dModifier; // deformation check인 경우  
    dDFactor  = 1.0 * dModifier;  // Service 와 Fatigue에서만 사용
    dStlFactor = m_dAshLr12Stl * dModifier;

    GetAASHTOSince12Factor(sDcFac[0], sDcFac[1], sDdFac[0], sDdFac[1], sDwFac[0], sDwFac[1], sEhFac[0], sEhFac[1], 
        sEvFac[0], sEvFac[1], sEsFac[0], sEsFac[1], sElFac[0], sElFac[1], sPsFac[0], sPsFac[1], sCrFac[0], sCrFac[1], 
        sShFac[0], sShFac[1], dDcFac[0], dDcFac[1], dDdFac[0], dDdFac[1], dDwFac[0], dDwFac[1], dEhFac[0], dEhFac[1], 
        dEvFac[0], dEvFac[1], dEsFac[0], dEsFac[1], dElFac[0], dElFac[1], dPsFac[0], dPsFac[1], dCrFac[0], dCrFac[1], 
        dShFac[0], dShFac[1], enLcomNo, bStrength);   

    Add_CvlAASHTOSince12_Trans_Factor (enLcomNo, bStrength, dCfFactor, dLFactor, dWFactor, dWpFactor, dFrFactor, dEFactor);
    Add_CvlAASHTOSince12_Temp_Factor  (enLcomNo, bStrength, dTFactor, dTpgFactor);

    Add_CvlAASHTOSince12_Title      (enLcomNo, aNewLcom);

    CString csPrefix_Tmp(_T(""));
    if(m_nLcomType == D_LCOMTYPE_STLCOMP) csPrefix_Tmp = _T(",");
    else                                  csPrefix_Tmp = _T("+");    


    if ( bStrength || bExtreme )
    {
        Add_CvlAASHTOSince12_DC         (enLcomNo, arDcLoad, dDcFac[0], dDcFac[1], csPrefix_Tmp, aNewLcom);
        Add_CvlAASHTOSince12_DW         (enLcomNo, arDwLoad, dDwFac[0], dDwFac[1], csPrefix_Tmp, aNewLcom);
        Add_CvlAASHTOSince12_DD         (enLcomNo, arDdLoad, dDdFac[0], dDdFac[1], csPrefix_Tmp, aNewLcom);
        Add_CvlAASHTOSince12_EH         (enLcomNo, arEhLoad, dEhFac[0], dEhFac[1], csPrefix_Tmp, aNewLcom);
        Add_CvlAASHTOSince12_EV         (enLcomNo, arEvLoad, dEvFac[0], dEvFac[1], csPrefix_Tmp, aNewLcom);
        Add_CvlAASHTOSince12_ES         (enLcomNo, arEsLoad, dEsFac[0], dEsFac[1], csPrefix_Tmp, aNewLcom);
        Add_CvlAASHTOSince12_EL         (enLcomNo, arElLoad, dElFac[0], dElFac[1], csPrefix_Tmp, aNewLcom);
    }
    else if ( bSeis )
    {
        Add_CvlAASHTOSince12_DC         (enLcomNo, arDcLoad, dDcFac[0], dDcFac[1], csPrefix_Tmp, aNewLcom);
        Add_CvlAASHTOSince12_DW         (enLcomNo, arDwLoad, dDwFac[0], dDwFac[1], csPrefix_Tmp, aNewLcom);
    }
    else if ( bService )
    {
        Add_CvlAASHTOSince12_SLS_DEAD   (enLcomNo, arDdLoad, arDwLoad, arDcLoad, dDFactor, csPrefix_Tmp, aNewLcom);
        Add_CvlAASHTOSince12_SLS_EARTH  (enLcomNo, arEvLoad, arEhLoad, arEsLoad, arElLoad, dDFactor, csPrefix_Tmp, aNewLcom);
    }
    else
    {
        Add_CvlAASHTOSince12_FTG_DEAD(enLcomNo, arDwLoad, arDcLoad, dDFactor, csPrefix_Tmp, aNewLcom);
    }


    if ( bMoving )
    {
        Add_CvlAASHTOSince12_FTG_M      (enLcomNo, arMLoad, arLLoad,  arIlLoad, arCfLoad, dLFactor, dCfFactor, csPrefix_Tmp, aNewLcom);
        Add_CvlAASHTOSince12_M          (enLcomNo, arMLoad, arLLoad, arIlLoad, arCfLoad, arBrkLoad, dLFactor, dCfFactor, csPrefix_Tmp, aNewLcom);
        Add_CvlAASHTOSince12_CRL_LS     (enLcomNo, arCrlLoad, arLsLoad, dCfFactor, csPrefix_Tmp, aNewLcom);
    }
    else
    {
        Add_CvlAASHTOSince12_FTG_NO_M   (enLcomNo, arLLoad, arIlLoad, arCfLoad, dLFactor, dCfFactor, csPrefix_Tmp, aNewLcom);                                   
    }

    if ( bWind )
    {
        Add_CvlAASHTOSince12_W          (enLcomNo, arWLoad,   dWFactor,  csPrefix_Tmp, aNewLcom);
        Add_CvlAASHTOSince12_WL         (enLcomNo, arWlLoad,  dWlFactor, csPrefix_Tmp, aNewLcom);
    }

    if ( !bExtreme && !bSeis )
    {
        Add_CvlAASHTOSince12_STL_SM     (enLcomNo, arStlLoad, arSmLoad,  dStlFactor, csPrefix_Tmp, aNewLcom);
    }

    if ( bStrength || bService )
    {
        Add_CvlAASHTOSince12_T          (enLcomNo, arTLoad,   dTFactor, csPrefix_Tmp, aNewLcom);
    }

    if ( bExtreme )
    {
        Add_CvlAASHTOSince12_EQ         (enLcomNo, arELoad ,  arEspLoad, dEFactor, csPrefix_Tmp, aNewLcom);
        Add_CvlAASHTOSince12_CO         (enLcomNo, arIpLoad,  arCtLoad, arCvLoad, dEFactor, csPrefix_Tmp, aNewLcom);
    }
    
    if( bSeis )
    {
        const BOOL bOrtho = (m_nOrthoType==0 ? TRUE : FALSE);
        const int  nOrtho = m_a100_30_Load.GetSize();
        if ( bOrtho && nOrtho>0 )
        {
            Add_CvlAASHTOSince12_EQ_Ortho   (enLcomNo, arELoad, arEspLoad, dEFactor, csPrefix_Tmp, aNewLcom);
        }
        else
        {
            Add_CvlAASHTOSince12_EQ         (enLcomNo, arELoad, arEspLoad, dEFactor, csPrefix_Tmp, aNewLcom);
        }
    }
    else
    {
        Add_CvlAASHTOSince12_TPG        (enLcomNo, arTpgLoad, dTpgFactor, csPrefix_Tmp, aNewLcom);

        Add_CvlAASHTOSince12_FL         (enLcomNo, arWpLoad, arFpLoad, arBLoad, arSfLoad, arWprLoad, dWpFactor, csPrefix_Tmp, aNewLcom);
        Add_CvlAASHTOSince12_FR         (enLcomNo, arFrLoad, dFrFactor, csPrefix_Tmp, aNewLcom);
    }

    if ( bStrength || bExtreme )
    {
        Add_CvlAASHTOSince12_PS         (enLcomNo, arPsLoad,  dPsFac[0], dPsFac[1],  csPrefix_Tmp, aNewLcom);
        Add_CvlAASHTOSince12_CR         (enLcomNo, arCrLoad,  dCrFac[0], dCrFac[1],  csPrefix_Tmp, aNewLcom);
        Add_CvlAASHTOSince12_SH         (enLcomNo, arShLoad,  dShFac[0], dShFac[1],  csPrefix_Tmp, aNewLcom);
    }

    if ( bService )
    {
        Add_CvlAASHTOSince12_SLS_PSCRSH (enLcomNo, arPsLoad, arCrLoad, arShLoad, dDFactor, csPrefix_Tmp, aNewLcom);
    }


    //Construction 시작
    if (!bSeis)
    {
        Add_CvlAASHTOSince12_CS_DC (enLcomNo, bStrength, bExtreme, bService, bFatigue, arCSLoad, dDcFac[0], dDcFac[1], dDFactor, csPrefix_Tmp, aNewLcom);
        Add_CvlAASHTOSince12_CS_DW (enLcomNo, bStrength, bExtreme, bService, bFatigue, arCSLoad, dDwFac[0], dDwFac[1], dDFactor, csPrefix_Tmp, aNewLcom);

        if (bMoving)
        {
            Add_CvlAASHTOSince12_CS_LIVE  (enLcomNo, bFatigue, arCSLoad, dDFactor, dLFactor, csPrefix_Tmp, aNewLcom);
        }

        if (bWind)
        {
            Add_CvlAASHTOSince12_CS_W  (enLcomNo, bFatigue, arCSLoad, dDFactor, dWFactor, csPrefix_Tmp, aNewLcom);
        }

        Add_CvlAASHTOSince12_CS_T       (enLcomNo, bStrength, bExtreme, bService, bFatigue, arCSLoad, dDFactor, dTFactor, dTpgFactor, csPrefix_Tmp, aNewLcom);
        Add_CvlAASHTOSince12_CS_STL     (enLcomNo, bStrength, bExtreme, bService, bFatigue, arCSLoad, dDFactor, dStlFactor, csPrefix_Tmp, aNewLcom);
        Add_CvlAASHTOSince12_CS_E       (enLcomNo, arCSLoad, dEFactor, csPrefix_Tmp, aNewLcom);

        Add_CvlAASHTOSince12_CS_PS      (enLcomNo, bStrength, bExtreme, bService, bFatigue, arCSLoad, dPsFac[0], dPsFac[1], dDFactor, csPrefix_Tmp, aNewLcom);
        Add_CvlAASHTOSince12_CS_CR_SH   (enLcomNo, bStrength, bExtreme, bService, bFatigue, arCSLoad, dCrFac[0], dCrFac[1], dTFactor, dDFactor, csPrefix_Tmp, aNewLcom);
    }

    // Add new load combinations.
    T_LCOM_D envLcomD; envLcomD.Initialize();
    int nNewLcom = aNewLcom.GetSize();
    for(int i=0; i<nNewLcom; ++i)
    {
        if(aNewLcom[i].aCombination.GetSize() == 0) continue;

        if (bStrength)
        {
            if(m_nLcomType == D_LCOMTYPE_STLCOMP)
            {
                aNewLcom[i].nActive = 1;
                aNewLcom[i].nSubLcomType = 0;
            }
            else if (m_nLcomType == D_LCOMTYPE_CONCRETE || m_nLcomType == D_LCOMTYPE_GENERAL)
            {
                aNewLcom[i].nActive = 1;
            }
        }
        else if (bExtreme || bSeis)
        {
            aNewLcom[i].nActive = 1;
        }
        else if (bService)
        {
            if(m_nLcomType == D_LCOMTYPE_STLCOMP)
            {
                aNewLcom[i].nActive = 2;
                aNewLcom[i].nSubLcomType = 0;
            }
            else
            {
                if(m_nLcomType != D_LCOMTYPE_GENERAL) aNewLcom[i].nActive = 2; 
                // 20120726 - JHYUN Service Lcom 자동 분류를 위해서 추가
                // PSC - Concrete Allowable Stress Load Case 에서 사용
                if     (enLcomNo == EN_AASHTOSince12_NO_SER1) aNewLcom[i].nServLcomType = 1;
                else if(enLcomNo == EN_AASHTOSince12_NO_SER2) aNewLcom[i].nServLcomType = 2;
                else if(enLcomNo == EN_AASHTOSince12_NO_SER3) aNewLcom[i].nServLcomType = 3;
                else if(enLcomNo == EN_AASHTOSince12_NO_SER4) aNewLcom[i].nServLcomType = 4;
                else                                          aNewLcom[i].nServLcomType = 0; 
            }
        }
        else if (bFatigue)
        {
            if(m_nLcomType == D_LCOMTYPE_STLCOMP)
            {
                aNewLcom[i].nActive = 2;
                if     (enLcomNo == EN_AASHTOSince12_NO_FAT1) aNewLcom[i].nSubLcomType = 1;
                else if(enLcomNo == EN_AASHTOSince12_NO_FAT2) aNewLcom[i].nSubLcomType = 2;
            }
            else
            {
                if(m_nLcomType != D_LCOMTYPE_GENERAL) aNewLcom[i].nActive = 2; 
            }
        }

        if (!ExistInNewLcom(aNewLcom[i], TRUE))
        {
            if (bStrength || bExtreme || bSeis)
            {
                Set_LoadComb4Civil(aNewLcom[i], D_STR);
            }
            else if (bService || bFatigue )
            {
                Set_LoadComb4Civil(aNewLcom[i], D_SER);
            }
        }
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_DC(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arDcLoad, double& dDc1, double& dDc2, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nConstLoad==1) return;
    if(m_nDcCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    double dDcFac[2] = { dDc1, dDc2 };
    CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;

    const int nLcom = raNewLcom.GetSize();
    const int nDcNum = m_nAshLr12DcType == 2 ? 2 : 1;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nDc = 0; nDc  < nDcNum; ++nDc)
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nDcL = 0, nCaseNum=nExistCase;
            for(int j=0; j<m_nDcCount; ++j) nDcL += Set_STLCIncludeCS(arDcLoad.GetAt(j), dDcFac[nDc], nCaseNum, LcomD, D_SGLD_DL_CVL);

            // Description
            if(nDcL > 0)
            {   
                CString csPrefix(_T("")), csLcom(_T(""));

                csLcom += csPrefix + GetFactorString(dDcFac[nDc])  + _T("DC");  csPrefix = csPrefix_Tmp; 
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }

            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_DD(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arDdLoad, double& dDd1, double& dDd2, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nDdCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    double dDdFac[2] = { dDd1, dDd2 };    
    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;
    
	const int nLcom = raNewLcom.GetSize();
	const int nDdNum = m_nAshLr12DdType == 2 ? 2 : 1;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nDd = 0; nDd  < nDdNum; ++nDd)
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nDdL = 0, nCaseNum=nExistCase;
            for(int j=0; j<m_nDdCount; ++j) nDdL += Set_STLCIncludeCS(arDdLoad.GetAt(j), dDdFac[nDd], nCaseNum, LcomD, 0);

            // Description
            if(nDdL > 0)
            {   
                CString csPrefix(_T("")), csLcom(_T(""));

                csLcom += csPrefix + GetFactorString(dDdFac[nDd])+ _T("DD"); csPrefix = csPrefix_Tmp;
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }

            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_DW(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arDwLoad, double& dDw1, double& dDw2, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nConstLoad==1) return;
    if(m_nDwCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    double dDwFac[2] = { dDw1, dDw2 };
    CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;
	
    const int nLcom = raNewLcom.GetSize();
    const int nDwNum = m_nAshLr12DwType == 2 ? 2 : 1; 
    
    for(int i=0; i<nLcom; ++i)
    {
        for(int nDw = 0; nDw  < nDwNum; ++nDw)
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nDwL = 0, nCaseNum=nExistCase;
            for(int j=0; j<m_nDwCount; ++j) nDwL += Set_STLCIncludeCS(arDwLoad.GetAt(j), dDwFac[nDw], nCaseNum, LcomD, D_SGLD_DL_CVL);

            // Description
            if(nDwL > 0)
            {   
                CString csPrefix(_T("")), csLcom(_T(""));

                csLcom += csPrefix + GetFactorString(dDwFac[nDw])+ _T("DW"); csPrefix = csPrefix_Tmp;
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }

            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_EH(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arEhLoad, double& dEh1, double& dEh2, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nEhCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    double dEhFac[2] = { dEh1, dEh2 };
    CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;

    const int nLcom = raNewLcom.GetSize();
    const int nEhNum = m_nAshLr12EhType == 2 ? 2 : 1;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nEh = 0; nEh < nEhNum; ++nEh)
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nEhL=0, nCaseNum=nExistCase;
            for(int j=0; j<m_nEhCount; ++j) nEhL  += Set_STLCIncludeCS(arEhLoad.GetAt(j), dEhFac[nEh], nCaseNum, LcomD, 0);
            // Description
            if(nEhL > 0)
            {   
                CString csPrefix(_T("")), csLcom(_T(""));

                csLcom += csPrefix + GetFactorString(dEhFac[nEh])+ _T("EH"); csPrefix = csPrefix_Tmp;
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }

            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_EV(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arEvLoad, double& dEv1, double& dEv2, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nEvCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    double dEvFac[2] = { dEv1, dEv2 };
    CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;
    
    const int nLcom = raNewLcom.GetSize();
    const int nEvNum = m_nAshLr12EvType == 2 ? 2 : 1;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nEv = 0; nEv < nEvNum; ++nEv)  
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nEvL=0, nCaseNum=nExistCase;
            for(int j=0; j<m_nEvCount; ++j) nEvL  += Set_STLCIncludeCS(arEvLoad.GetAt(j), dEvFac[nEv], nCaseNum, LcomD, 0);
            // Description
            if(nEvL > 0)
            {   
                CString csPrefix(_T("")), csLcom(_T(""));

                csLcom += csPrefix + GetFactorString(dEvFac[nEv])+ _T("EV"); csPrefix = csPrefix_Tmp;
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }

            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_ES(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arEsLoad, double& dEs1, double& dEs2, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nEsCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

	double dEsFac[2] = { dEs1, dEs2 };
    CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;

    const int nLcom = raNewLcom.GetSize();
    const int nEsNum = m_nAshLr12EsType == 2 ? 2 : 1;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nEs = 0; nEs < nEsNum; ++nEs)
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nEsL=0, nCaseNum=nExistCase;
            for(int j=0; j<m_nEsCount; ++j) nEsL  += Set_STLCIncludeCS(arEsLoad.GetAt(j), dEsFac[nEs], nCaseNum, LcomD, 0);
            // Description
            if(nEsL > 0)
            {   
                CString csPrefix(_T("")), csLcom(_T(""));

                csLcom += csPrefix + GetFactorString(dEsFac[nEs])+ _T("ES"); csPrefix = csPrefix_Tmp;
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }

            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_EL(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arElLoad, double& dEl1, double& dEl2, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nElCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

	double dElFac[2] = { dEl1, dEl2 };
    CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;

    const int nLcom = raNewLcom.GetSize();
    const int nElNum = m_nAshLr12ElType == 2 ? 2 : 1;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nEl = 0; nEl < nElNum; ++nEl)
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nElL=0, nCaseNum=nExistCase;
            for(int j=0; j<m_nElCount; ++j) nElL  += Set_STLCIncludeCS(arElLoad.GetAt(j), dElFac[nEl], nCaseNum, LcomD, 0);
            // DElcription
            if(nElL > 0)
            {   
                CString csPrefix(_T("")), csLcom(_T(""));

                csLcom += csPrefix + GetFactorString(dElFac[nEl])+ _T("EL"); csPrefix = csPrefix_Tmp;
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }

            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_PS(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arPsLoad, double& dPs1, double& dPs2, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nConstLoad==1) return;
    if(m_nPsCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

	double dPsFac[2] = { dPs1, dPs2 };
    CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;
    
    const int nLcom = raNewLcom.GetSize();    
    const int nPsNum = m_nAshLr12PsType == 2 ? 2 : 1;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nPs = 0; nPs < nPsNum; ++nPs)  
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nPsL=0, nCaseNum=nExistCase;
            for(int j=0; j<m_nPsCount; ++j) nPsL  += Set_STLCIncludeCS(arPsLoad.GetAt(j), dPsFac[nPs],   nCaseNum, LcomD, D_SGLD_TS_CVL);
            // Description
            if(nPsL > 0)
            {
                CString csPrefix(_T("")), csLcom(_T(""));

                if(nPsL > 0) { csLcom += csPrefix + GetFactorString(dPsFac[nPs]) + _T("PS"); csPrefix = csPrefix_Tmp;}
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }
            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_CR(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arCrLoad, double& dCr1, double& dCr2, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nConstLoad==1) return;
    if(m_nCrCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

	double dCrFac[2] = { dCr1, dCr2 };
    CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;

    const int nLcom = raNewLcom.GetSize();    
    const int nCrNum = m_nAshLr12CrType == 2 ? 2 : 1;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nCr = 0; nCr < nCrNum; ++nCr)
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nCrL=0, nCaseNum=nExistCase;
            for(int j=0; j<m_nCrCount; ++j) nCrL  += Set_STLCIncludeCS(arCrLoad.GetAt(j), dCrFac[nCr], nCaseNum, LcomD, D_SGLD_CS_CVL);
            // Description
            if(nCrL > 0)
            {
                CString csPrefix(_T("")), csLcom(_T(""));

                if(nCrL > 0) {csLcom += csPrefix + GetFactorString(dCrFac[nCr]) + _T("CR"); csPrefix = csPrefix_Tmp;}
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }
            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_SH(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arShLoad, double& dSh1, double& dSh2, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nConstLoad==1) return;
    if(m_nShCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

	double dShFac[2] = { dSh1, dSh2 };
    CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;

    const int nLcom = raNewLcom.GetSize();
    const int nShNum = m_nAshLr12ShType == 2 ? 2 : 1;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nSh = 0; nSh < nShNum; ++nSh)
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nPsL=0, nCrL=0, nShL=0, nCaseNum=nExistCase;
            for(int j=0; j<m_nShCount; ++j) nShL  += Set_STLCIncludeCS(arShLoad.GetAt(j), dShFac[nSh], nCaseNum, LcomD, D_SGLD_SS_CVL);
            // Description
            if(nShL > 0)
            {
                CString csPrefix(_T("")), csLcom(_T(""));

                if(nShL > 0) { csLcom += csPrefix + GetFactorString(dShFac[nSh]) + _T("SH"); csPrefix = csPrefix_Tmp;}
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }
            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_W(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arWLoad, double& dWFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if (enLcomNo == EN_AASHTOSince12_NO_FAT1 || enLcomNo == EN_AASHTOSince12_NO_FAT2) return;
    if(m_nWCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();

    int nWCountTemp = (m_nWCount>0) ? m_nWCount*2 : 1;

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nWm = 0; nWm < nWCountTemp; ++nWm)  
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nWL=0, nCaseNum=nExistCase;
            double dWF = (nWm%2==0) ? dWFactor : -1.0 * dWFactor;
            if(m_nWCount > 0) nWL += Set_STLCIncludeCS(arWLoad.GetAt(nWm/2), dWF, nCaseNum, LcomD, 0);          
            // Description
            if(nWL > 0)
            {
                CString csPrefix(_T("")), csLcom(_T(""));

                CString strFixW;
                strFixW.Format(_T("%d"), nWm/2+1);
                csLcom += csPrefix + GetFactorString(dWF) + _T("W[" + strFixW + _T("]")); csPrefix = csPrefix_Tmp;
                if (dWF < 0.0)
                {
                    csPrefix = _T("");
                }

                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }
            if(nCaseNum > 0) raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_WL(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arWlLoad, double& dWlFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(enLcomNo == EN_AASHTOSince12_NO_FAT1 || enLcomNo == EN_AASHTOSince12_NO_FAT2) return;
    if(enLcomNo == EN_AASHTOSince12_NO_STR3) return;
    if(m_nWlCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();

    int nWlCountTemp = (m_nWlCount>0) ? m_nWlCount*2 : 1;

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nWlm = 0; nWlm < nWlCountTemp; ++nWlm)  
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nWL=0, nCaseNum=nExistCase;
            double dWLF = (nWlm%2==0) ? dWlFactor : -1.0 * dWlFactor;
            if(m_nWlCount > 0) nWL += Set_STLCIncludeCS(arWlLoad.GetAt(nWlm/2), dWLF, nCaseNum, LcomD, 0);          
            // Description
            if(nWL > 0)
            {
                CString csPrefix(_T("")), csLcom(_T(""));

                CString strFixWL;
                strFixWL.Format(_T("%d"), nWlm/2+1);
                csLcom += csPrefix + GetFactorString(dWLF) + _T("WL[") + strFixWL + _T("]"); csPrefix = csPrefix_Tmp;
                if (dWLF < 0.0)
                {
                    csPrefix = _T("");
                }

                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }
            if(nCaseNum > 0) raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_STL_SM(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arStlLoad, ArUNT& arSmLoad, double& dStlFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{

    if(enLcomNo != EN_AASHTOSince12_NO_SER1 && enLcomNo != EN_AASHTOSince12_NO_SER3 && enLcomNo == EN_AASHTOSince12_NO_STR4) return;
    if(enLcomNo == EN_AASHTOSince12_NO_FAT1 || enLcomNo == EN_AASHTOSince12_NO_FAT2) return;
    if(m_nSmCount==0 && m_nStlCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nSmCountTemp   = max(m_nSmCount, 1);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nSm = 0; nSm < nSmCountTemp; ++nSm)  
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nStL=0, nCaseNum=nExistCase;
            for(int j=0; j<m_nStlCount; ++j) nStL  += Set_STLCIncludeCS(arStlLoad.GetAt(j), dStlFactor, nCaseNum, LcomD, 0);
            if(m_nSmCount > 0)                        Set_SettleCase(arSmLoad.GetAt(nSm),   dStlFactor, nCaseNum, LcomD, TRUE);
            // Description
            if(nStL > 0 || m_nSmCount > 0)
            {
                CString csPrefix(_T("")), csLcom(_T(""));

                CString strFixSm;
                strFixSm.Format(_T("%d"), nSm+1);
                if(nStL       > 0) { csLcom += csPrefix + GetFactorString(dStlFactor) + _T("STL"); csPrefix = csPrefix_Tmp; }
                if(m_nSmCount > 0) { csLcom += csPrefix + GetFactorString(dStlFactor) + _T("SM[") + strFixSm + _T("]"); csPrefix = csPrefix_Tmp; }
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }

            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_T(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arTLoad, double& dTFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nTCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();

    int nTCountTemp = max(m_nTCount, 1);

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nTm = 0; nTm < nTCountTemp; ++nTm) 
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nTL=0, nCaseNum=nExistCase;
            if(m_nTCount > 0)   nTL   += Set_STLCIncludeCS(arTLoad.GetAt(nTm),      dTFactor, nCaseNum, LcomD, 0);

            // Description
            if(nTL > 0)
            {
                CString csPrefix(_T("")), csLcom(_T(""));

                CString strFixT, strFixTpg;
                strFixT.Format(_T("%d"), nTm+1);
                if(nTL  > 0) {csLcom += csPrefix + GetFactorString(dTFactor)     + _T("T[") + strFixT + _T("]"); csPrefix = csPrefix_Tmp;}
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }
            if(nCaseNum > 0) raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_TPG(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arTpgLoad, double& dTpgFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(enLcomNo != EN_AASHTOSince12_NO_SER1 && enLcomNo != EN_AASHTOSince12_NO_SER3) return;
    if(m_nTpgCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();

    int nTpgCountTemp = max(m_nTpgCount, 1);

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nTpgm = 0; nTpgm < nTpgCountTemp; ++nTpgm)
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nTpgL=0, nCaseNum=nExistCase;
            if(m_nTpgCount > 0) nTpgL += Set_STLCIncludeCS(arTpgLoad.GetAt(nTpgm),  dTpgFactor, nCaseNum, LcomD, 0);

            // Description
            if(nTpgL > 0)
            {
                CString csPrefix(_T("")), csLcom(_T(""));

                CString strFixT, strFixTpg;
                strFixTpg.Format(_T("%d"), nTpgm+1);
                if(nTpgL> 0) {csLcom += csPrefix + GetFactorString(dTpgFactor)   + _T("TPG[") + strFixTpg + _T("]"); csPrefix = csPrefix_Tmp;}
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }
            if(nCaseNum > 0) raTotalLcom.Add(LcomD);
        }

    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_M(const EN_AASHTOSince12_NO& enLcomNo, ArUNT& arMLoad  , ArINT& arLLoad,  ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arBrkLoad, double& dLFactor, double& dBrCfFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if (enLcomNo == EN_AASHTOSince12_NO_FAT1 || enLcomNo == EN_AASHTOSince12_NO_FAT2) return;
    if(m_nMCount==0 && m_nLCount==0 ) return;

    auto lambda_GetSelectEffMVLoad = [this](const EN_AASHTOSince12_NO& enLcomNo, const int& iCaseKey, OUT double& dFactor) -> BOOL
    {
        if (!m_bSeismic) return TRUE;
        if (enLcomNo != EN_AASHTOSince12_NO_SEIS_EQ && enLcomNo != EN_AASHTOSince12_NO_SEIS_RS) return TRUE;

        BOOL bSel = FALSE;
        for (int i = 0; i < m_aEffectiveMVLoad.GetSize(); ++i)
        {
            T_MVLD_K MvldK = m_aEffectiveMVLoad[i].MvldK;
            if (MvldK==iCaseKey)
            {
                bSel = TRUE;
                dFactor = m_aEffectiveMVLoad[i].dFactor;
                break;
            }
        }
        return bSel;
    };

    auto lambda_GetSelectEffLLLoad = [this] (const EN_AASHTOSince12_NO& enLcomNo, const int& iCaseKey, OUT double& dFactor) -> BOOL
    {
        if ( !m_bSeismic ) return TRUE;
        if ( enLcomNo != EN_AASHTOSince12_NO_SEIS_EQ && enLcomNo != EN_AASHTOSince12_NO_SEIS_RS ) return TRUE;

        BOOL bSel = FALSE;
        for ( int i = 0; i < m_aEffectiveLoad.GetSize(); ++i )
        {
            T_STLD_K MvldK = m_aEffectiveLoad[i].StldK;
            if ( MvldK==iCaseKey )
            {
                bSel = TRUE;
                dFactor = m_aEffectiveLoad[i].dFactor;
                break;
            }
        }
        return bSel;
    };

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();
    int nBrkCountTemp = max(m_nBrkCount, 1);
    int nMCount = max(m_nMCount, 1);

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nM = 0; nM < nMCount; ++nM)
        {
            for(int nBrk = 0; nBrk < nBrkCountTemp; ++nBrk)
            {        
                T_LCOM_D LcomD = raNewLcom[i];
                int nExistCase = LcomD.aCombination.GetSize();
                int nLL=0, nIL=0, nCF=0, nBRK=0, nCaseNum=nExistCase;

                double deffLL = 1.0;
                if (m_nMCount>0)
                {   
                    if (lambda_GetSelectEffMVLoad(enLcomNo, arMLoad.GetAt(nM), deffLL))
                    {
                        Set_MovingCase(arMLoad.GetAt(nM), deffLL*dLFactor, nCaseNum, LcomD, TRUE);
                    }
                }
                if(m_nLCount > 0)  
                {
                    for ( int j=0; j<m_nLCount; j++ )
                    {
                        if ( lambda_GetSelectEffLLLoad(enLcomNo, arLLoad.GetAt(j), deffLL) )
                        {
                            nLL += Set_STLCIncludeCS(arLLoad.GetAt(j), deffLL*dLFactor, nCaseNum, LcomD, 0);
                        }
                    }
                    for(int j=0; j<m_nIlCount; j++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(j),  dLFactor, nCaseNum, LcomD, 0);
                } 
                if(m_nMCount>0 || m_nLCount>0)
                {
                    for(int j=0; j<m_nCfCount; j++)  nCF += Set_STLCIncludeCS(arCfLoad.GetAt(j),  dBrCfFactor,     nCaseNum, LcomD, 0);
                    if(m_nBrkCount > 0)          nBRK+= Set_STLCIncludeCS(arBrkLoad.GetAt(nBrk), dBrCfFactor,  nCaseNum, LcomD, 0);
                }

                // Description
                if(m_nMCount>0 || nLL > 0 || nIL > 0 || nCF > 0 || nBRK > 0)
                {
                    CString csPrefix(_T("")), csLcom(_T(""));

                    CString strFixM, strFixBrk;
                    strFixM.Format(_T("%d"), nM+1);
                    strFixBrk.Format(_T("%d"), nBrk+1);
                    if(m_nMCount>0) 
                    {
                        if(m_bSeismic) // AASHTO Seismic
                        {
                            CString strLcas = Get_MovingLoadLoadCaseName(arMLoad.GetAt(nM));
                            csLcom += csPrefix + GetFactorString(deffLL*dLFactor)  + strLcas; 
                        }
                        else
                        {
                            csLcom += csPrefix + GetFactorString(dLFactor)  + _T("M[") + strFixM + _T("]");
                        }
                        csPrefix = csPrefix_Tmp;
                    }              
                    if(m_nLCount> 0)
                    {
                        if(nLL > 0) 
                        {
                            if ( m_bSeismic )
                            {
                                csLcom += csPrefix + GetFactorString(deffLL*dLFactor)  + _T("L");   csPrefix = csPrefix_Tmp;
                            }
                            else
                                csLcom += csPrefix + GetFactorString(dLFactor)  + _T("L");   csPrefix = csPrefix_Tmp;
                        }  
                        if(nIL > 0) {csLcom += csPrefix + GetFactorString(dLFactor)  + _T("IL");  csPrefix = csPrefix_Tmp;}  
                    }
                    if(m_nMCount>0 || m_nLCount>0)
                    {
                        if(nCF > 0) {csLcom += csPrefix + GetFactorString(dBrCfFactor) + _T("CF");  csPrefix = csPrefix_Tmp;}
                        if(nBRK> 0) {csLcom += csPrefix + GetFactorString(dBrCfFactor) + _T("BRK[" + strFixBrk + _T("]")); csPrefix = csPrefix_Tmp;}
                    }
                    if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                    {
                        csPrefix = _T("");
                    }
                    LcomD.Description += csPrefix + csLcom;
                }
                if(nCaseNum > 0) raTotalLcom.Add(LcomD);
            }
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_FL(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arWpLoad,  ArINT& arFpLoad, ArINT& arBLoad, ArINT& arSfLoad, ArINT& arWprLoad, double& dWpFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if (enLcomNo == EN_AASHTOSince12_NO_FAT1 || enLcomNo == EN_AASHTOSince12_NO_FAT2) return;
    if(m_nWpCount==0 && m_nFpCount==0 && m_nBCount==0 && m_nSfCount==0 && m_nWprCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();
        int nWP=0, nFP=0, nBL=0, nSF=0, nWPR=0, nCaseNum=nExistCase;
        for(int j=0; j<m_nWpCount;  j++)  nWP += Set_STLCIncludeCS(arWpLoad.GetAt(j),  dWpFactor,  nCaseNum, LcomD, 0);
        for(int j=0; j<m_nFpCount;  j++)  nFP += Set_STLCIncludeCS(arFpLoad.GetAt(j),  dWpFactor,  nCaseNum, LcomD, 0);
        for(int j=0; j<m_nBCount;   j++)  nBL += Set_STLCIncludeCS(arBLoad.GetAt(j),   dWpFactor,  nCaseNum, LcomD, 0);
        for(int j=0; j<m_nSfCount;  j++)  nSF += Set_STLCIncludeCS(arSfLoad.GetAt(j),  dWpFactor,  nCaseNum, LcomD, 0);
        for(int j=0; j<m_nWprCount; j++)  nWPR+= Set_STLCIncludeCS(arWprLoad.GetAt(j), dWpFactor,  nCaseNum, LcomD, 0);

        // Description
        if(nWP > 0 || nFP > 0 || nBL > 0 || nSF > 0 || nWPR > 0)
        {
            CString csPrefix(_T("")), csLcom(_T(""));

            CString strFixT, strFixTpg;
            if(nWP  > 0) { csLcom += csPrefix + GetFactorString(dWpFactor) + _T("WP");  csPrefix = csPrefix_Tmp;}
            if(nFP  > 0) { csLcom += csPrefix + GetFactorString(dWpFactor) + _T("FP");  csPrefix = csPrefix_Tmp;}
            if(nBL  > 0) { csLcom += csPrefix + GetFactorString(dWpFactor) + _T("B");   csPrefix = csPrefix_Tmp;}
            if(nSF  > 0) { csLcom += csPrefix + GetFactorString(dWpFactor) + _T("SF");  csPrefix = csPrefix_Tmp;}
            if(nWPR > 0) { csLcom += csPrefix + GetFactorString(dWpFactor) + _T("WPR"); csPrefix = csPrefix_Tmp;}

            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;
        }
        if(nCaseNum > 0) raTotalLcom.Add(LcomD);
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_CRL_LS(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arCrlLoad, ArINT& arLsLoad, double& dBrCfFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{   
    if (enLcomNo == EN_AASHTOSince12_NO_FAT1 || enLcomNo == EN_AASHTOSince12_NO_FAT2) return;
    if(m_nCrlCount==0 && m_nLsCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();
        int nCRL=0, nLS=0, nCaseNum=nExistCase;
        for(int j=0; j<m_nCrlCount; j++) nCRL+= Set_STLCIncludeCS(arCrlLoad.GetAt(j), dBrCfFactor,  nCaseNum, LcomD, 0);
        for(int j=0; j<m_nLsCount; j++)  nLS += Set_STLCIncludeCS(arLsLoad.GetAt(j),  dBrCfFactor,  nCaseNum, LcomD, 0);

        // Description
        if(nCRL > 0 || nLS > 0)
        {
            CString csPrefix(_T("")), csLcom(_T(""));

            if(nCRL> 0) {csLcom += csPrefix + GetFactorString(dBrCfFactor) + _T("CRL"); csPrefix = csPrefix_Tmp;}
            if(nLS > 0) {csLcom += csPrefix + GetFactorString(dBrCfFactor) + _T("LS");  csPrefix = csPrefix_Tmp;}
            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;
        }
        if(nCaseNum > 0) raTotalLcom.Add(LcomD);
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_FR(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arFrLoad, double& dFrFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if (enLcomNo == EN_AASHTOSince12_NO_FAT1 || enLcomNo == EN_AASHTOSince12_NO_FAT2) return;
    if(m_nFrCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();
        int nFR=0, nCaseNum=nExistCase;
        for(int j=0; j<m_nFrCount; j++)  nFR += Set_STLCIncludeCS(arFrLoad.GetAt(j),  dFrFactor,  nCaseNum, LcomD, 0);

        // Description
        if(nFR > 0)
        {
            CString csPrefix(_T("")), csLcom(_T(""));

            if(nFR> 0) {csLcom += csPrefix + GetFactorString(dFrFactor) + _T("FR"); csPrefix = csPrefix_Tmp;}
            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;
        }
        if(nCaseNum > 0) raTotalLcom.Add(LcomD);
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_SLS_DEAD(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arDdLoad , ArINT& arDwLoad, ArINT& arDcLoad, double& dDFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nConstLoad==1) return;
    if(m_nDcCount==0 && m_nDdCount==0 && m_nDwCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();
        int nDcL = 0,  nDdL = 0, nDwL = 0, nCaseNum=nExistCase;
        for(int j=0; j<m_nDcCount; j++)  nDcL += Set_STLCIncludeCS(arDcLoad.GetAt(j),  dDFactor, nCaseNum, LcomD, D_SGLD_DL_CVL);
        for(int j=0; j<m_nDdCount; j++)  nDdL += Set_STLCIncludeCS(arDdLoad.GetAt(j),  dDFactor, nCaseNum, LcomD, 0);
        for(int j=0; j<m_nDwCount; j++)  nDwL += Set_STLCIncludeCS(arDwLoad.GetAt(j),  dDFactor, nCaseNum, LcomD, D_SGLD_DL_CVL);

        // Description
        if(nDcL > 0 || nDdL  > 0 || nDwL  > 0)
        {   
            CString csPrefix(_T("")), csLcom(_T(""));

            if(nDcL  > 0) { csLcom += csPrefix + GetFactorString(dDFactor) + _T("DC");  csPrefix = csPrefix_Tmp;}
            if(nDdL  > 0) { csLcom += csPrefix + GetFactorString(dDFactor) + _T("DD");  csPrefix = csPrefix_Tmp;}
            if(nDwL  > 0) { csLcom += csPrefix + GetFactorString(dDFactor) + _T("DW");  csPrefix = csPrefix_Tmp;}
            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;
        }

        if(nCaseNum > 0)    raTotalLcom.Add(LcomD);

    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_SLS_EARTH(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arEvLoad, ArINT& arEhLoad, ArINT& arEsLoad, ArINT& arElLoad, double& dDFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nEvCount==0 && m_nEhCount && m_nEsCount && m_nElCount) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {

        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();
        int nEhL=0, nEvL=0, nEsL=0, nElL=0, nCaseNum=nExistCase;
        for(int j=0; j<m_nEhCount; j++)  nEhL += Set_STLCIncludeCS(arEhLoad.GetAt(j),  dDFactor, nCaseNum, LcomD, 0);
        for(int j=0; j<m_nEvCount; j++)  nEvL += Set_STLCIncludeCS(arEvLoad.GetAt(j),  dDFactor, nCaseNum, LcomD, 0);
        for(int j=0; j<m_nEsCount; j++)  nEsL += Set_STLCIncludeCS(arEsLoad.GetAt(j),  dDFactor, nCaseNum, LcomD, 0);
        for(int j=0; j<m_nElCount; j++)  nElL += Set_STLCIncludeCS(arElLoad.GetAt(j),  dDFactor, nCaseNum, LcomD, 0);

        // Description
        if(nEhL > 0 || nEvL > 0 || nEsL > 0 || nElL > 0)
        {   
            CString csPrefix(_T("")), csLcom(_T(""));

            if(nEhL  > 0) { csLcom += csPrefix + GetFactorString(dDFactor) + _T("EH");  csPrefix = csPrefix_Tmp;}
            if(nEvL  > 0) { csLcom += csPrefix + GetFactorString(dDFactor) + _T("EV");  csPrefix = csPrefix_Tmp;}
            if(nEsL  > 0) { csLcom += csPrefix + GetFactorString(dDFactor) + _T("ES");  csPrefix = csPrefix_Tmp;}
            if(nElL  > 0) { csLcom += csPrefix + GetFactorString(dDFactor) + _T("EL");  csPrefix = csPrefix_Tmp;}

            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;
        }

        if(nCaseNum > 0)    raTotalLcom.Add(LcomD);

    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_SLS_PSCRSH(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arPsLoad , ArINT& arCrLoad , ArINT& arShLoad, double& dDFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nConstLoad==1) return;
    if(m_nPsCount == 0 && m_nCrCount == 0 && m_nShCount == 0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();
        int nPS=0, nCR=0, nSH=0, nElL=0, nCaseNum=nExistCase;
        for(int j=0; j<m_nPsCount; j++)   nPS += Set_STLCIncludeCS(arPsLoad.GetAt(j),  dDFactor, nCaseNum, LcomD, D_SGLD_TS_CVL);
        for(int j=0; j<m_nCrCount; j++)   nCR += Set_STLCIncludeCS(arCrLoad.GetAt(j),  dDFactor, nCaseNum, LcomD, D_SGLD_CS_CVL);
        for(int j=0; j<m_nShCount; j++)   nSH += Set_STLCIncludeCS(arShLoad.GetAt(j),  dDFactor, nCaseNum, LcomD, D_SGLD_SS_CVL);

        // Description
        if(nPS > 0 || nCR > 0 || nSH > 0)
        {   
            CString csPrefix(_T("")), csLcom(_T(""));

            if(nPS  > 0) { csLcom += csPrefix + GetFactorString(dDFactor) + _T("PS");  csPrefix = csPrefix_Tmp;}
            if(nCR  > 0) { csLcom += csPrefix + GetFactorString(dDFactor) + _T("CR");  csPrefix = csPrefix_Tmp;}
            if(nSH  > 0) { csLcom += csPrefix + GetFactorString(dDFactor) + _T("SH");  csPrefix = csPrefix_Tmp;}
            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;
        }

        if(nCaseNum > 0)    raTotalLcom.Add(LcomD);

    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_EQ(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arELoad, ArUNT& arEspLoad, double& dEFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if     (enLcomNo == EN_AASHTOSince12_NO_ETR1_01 && m_nECount  == 0) return;
    else if(enLcomNo == EN_AASHTOSince12_NO_ETR1_02 && m_nEspCount== 0) return;
    else if(enLcomNo == EN_AASHTOSince12_NO_SEIS_EQ && m_nECount  == 0) return;
    else if(enLcomNo == EN_AASHTOSince12_NO_SEIS_RS && m_nEspCount== 0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();
    int nECountTemp = 1;
    if     (enLcomNo == EN_AASHTOSince12_NO_ETR1_01) nECountTemp = (m_nECount>0)   ? m_nECount*2   : 1;
    else if(enLcomNo == EN_AASHTOSince12_NO_ETR1_02) nECountTemp = (m_nEspCount>0) ? m_nEspCount*2 : 1;
    else if(enLcomNo == EN_AASHTOSince12_NO_SEIS_EQ) nECountTemp = (m_nECount>0)   ? m_nECount*2   : 1;
    else if(enLcomNo == EN_AASHTOSince12_NO_SEIS_RS) nECountTemp = (m_nEspCount>0) ? m_nEspCount*2 : 1;
    double dScaleUpFactor=1.0;

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nEqm = 0; nEqm < nECountTemp; ++nEqm)
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nEL=0, nCaseNum=nExistCase;
            double dEF = (nEqm%2==0) ? dEFactor : -1.*dEFactor;
            if (enLcomNo == EN_AASHTOSince12_NO_ETR1_01)
            {
                nEL += Set_STLCIncludeCS(arELoad.GetAt(nEqm/2), dEF, nCaseNum, LcomD, 0);
            }
            else if(enLcomNo == EN_AASHTOSince12_NO_ETR1_02)
            {
                Get_ScaleUpFactor(D_LOADCASE_SPECTRUM, arEspLoad.GetAt(nEqm/2), dScaleUpFactor);
                Set_SpectrumCase(arEspLoad.GetAt(nEqm/2), dEF*dScaleUpFactor, nCaseNum, LcomD, TRUE);
            }
            else if (enLcomNo == EN_AASHTOSince12_NO_SEIS_EQ)
            {
                nEL += Set_STLCIncludeCS(arELoad.GetAt(nEqm/2), dEF, nCaseNum, LcomD, 0);
            }
            else if (enLcomNo == EN_AASHTOSince12_NO_SEIS_RS)
            {
                Get_ScaleUpFactor(D_LOADCASE_SPECTRUM, arEspLoad.GetAt(nEqm/2), dScaleUpFactor);
                Set_SpectrumCase(arEspLoad.GetAt(nEqm/2), dEF*dScaleUpFactor, nCaseNum, LcomD, TRUE);
            }

            // Description

            CString csPrefix(_T("")), csLcom(_T(""));

            CString strFixE;
            strFixE.Format(_T("%d"), nEqm/2+1);

            if(enLcomNo == EN_AASHTOSince12_NO_ETR1_01 && nEL>0)
            {
                csLcom += csPrefix + GetFactorString(dEF) + _T("E[") + strFixE + _T("]"); csPrefix = csPrefix_Tmp;
                if (dEF < 0.0)
                {
                    csPrefix = _T("");
                }
            }
            else if(enLcomNo == EN_AASHTOSince12_NO_ETR1_02)
            {
                csLcom += csPrefix + GetFactorString(dEF * dScaleUpFactor) + _T("ESP[") + strFixE + _T("]"); csPrefix = csPrefix_Tmp;
                if (dEF < 0.0)
                {
                    csPrefix = _T("");
                }
            }
            else if (enLcomNo == EN_AASHTOSince12_NO_SEIS_EQ && nEL>0)
            {
                csLcom += csPrefix + GetFactorString(dEF) + _T("E[") + strFixE + _T("]"); csPrefix = csPrefix_Tmp;
                if (dEF < 0.0)
                {
                    csPrefix = _T("");
                }
            }
            else if (enLcomNo == EN_AASHTOSince12_NO_SEIS_RS)
            {
                csLcom += csPrefix + GetFactorString(dEF * dScaleUpFactor) + _T("ESP[") + strFixE + _T("]"); csPrefix = csPrefix_Tmp;
                if (dEF < 0.0)
                {
                    csPrefix = _T("");
                }
            }

            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;

            if(nCaseNum > 0) raTotalLcom.Add(LcomD);
        }

    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_EQ_Ortho(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arELoad, ArUNT& arEspLoad, double& dEFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    bool bMakeOrtho = true;
    const BOOL bOrtho = (m_nOrthoType==0 ? TRUE : FALSE);
    const int  nOrtho = m_a100_30_Load.GetSize();
    if (!bOrtho) bMakeOrtho = false;
    else if (nOrtho==0) bMakeOrtho = false;
    else if (enLcomNo != EN_AASHTOSince12_NO_SEIS_EQ && enLcomNo != EN_AASHTOSince12_NO_SEIS_RS) bMakeOrtho = false;
    if (bOrtho)
    {
        if (enLcomNo == EN_AASHTOSince12_NO_SEIS_EQ && m_nECount  == 0) bMakeOrtho = false;
        else if (enLcomNo == EN_AASHTOSince12_NO_SEIS_RS && m_nEspCount== 0) bMakeOrtho = false;
    }
    if(!bOrtho)
    {
        Add_CvlAASHTOSince12_EQ(enLcomNo, arELoad, arEspLoad, dEFactor, csPrefix_Tmp, raNewLcom);
        return;
    }

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();
    int nECountTemp = 1;
    if      (enLcomNo == EN_AASHTOSince12_NO_SEIS_EQ) nECountTemp = (m_nECount>0)   ? m_nECount*2   : 1;
    else if (enLcomNo == EN_AASHTOSince12_NO_SEIS_RS) nECountTemp = (m_nEspCount>0) ? m_nEspCount*2 : 1;
    double dScaleUpFactor=1.0;

    auto lambda_GetDefineOrthoLoad = [this] (const int& iLCType, const int& iLCKey1, const int& iLCKey2, const int& iLCKey3,
                                             const int& nSubType, const int& nOrthoCombType,
                                             const double& dEFactor, CString& csPrefix, T_LCOM_D& rData, CArray<T_LCOM_D, T_LCOM_D&>& rAddLcom)
    {
        // Plus sign only (+,+) or Both (+,+)(+,-)(-,-)(-,+)
        const int nSize = (nOrthoCombType==COMBTYPE_BOTH) ? 2 : 1;
        const int nSizeV = iLCKey3>0 ? nSize : 1;
        // 100:30 Factor (Case 3)
        double dLc1Fac[2], dLc2Fac[2], dLc3Fac[2];
        CString sLc1Fac, sLc2Fac, sLc3Fac;
        switch (nSubType)
        {
            case LONGI_TYPE:
                dLc1Fac[0] = 1.0; dLc1Fac[1] = -1.0; // Govern
                dLc2Fac[0] = 0.3; dLc2Fac[1] = -0.3;
                dLc3Fac[0] = 0.3; dLc3Fac[1] = -0.3;
                sLc1Fac = _T("1.0");
                sLc2Fac = _T("0.3");
                sLc3Fac = _T("0.3");
                break;
            case TRANS_TYPE:
                dLc1Fac[0] = 0.3; dLc1Fac[1] = -0.3;
                dLc2Fac[0] = 1.0; dLc2Fac[1] = -1.0; // Govern      
                dLc3Fac[0] = 0.3; dLc3Fac[1] = -0.3;
                sLc1Fac = _T("0.3");
                sLc2Fac = _T("1.0");                
                sLc3Fac = _T("0.3");
                break;
            case VERTI_TYPE:
                dLc1Fac[0] = 0.3; dLc1Fac[1] = -0.3;
                dLc2Fac[0] = 0.3; dLc2Fac[1] = -0.3;
                dLc3Fac[0] = 1.0; dLc3Fac[1] = -1.0; // Govern
                sLc1Fac = _T("0.3");
                sLc2Fac = _T("0.3");
                sLc3Fac = _T("1.0");
                break;
            default:
                break;
        }
        const CString sLcEQ    = iLCType == 0 ? _T("(ST)") : _T("(RS)");
        const CString sLc1Name = iLCType == 0 ? Get_StaticLoadCaseName(iLCKey1) : Get_SpectrumLoadCaseName(iLCKey1);
        const CString sLc2Name = iLCType == 0 ? Get_StaticLoadCaseName(iLCKey2) : Get_SpectrumLoadCaseName(iLCKey2);
        const CString sLc3Name = iLCKey3>0 ? (iLCType==0 ? Get_StaticLoadCaseName(iLCKey3) : Get_SpectrumLoadCaseName(iLCKey3)) : _T("");;
        double dScaleUpFactor1=1.0, dScaleUpFactor2=1.0, dScaleUpFactor3=1.0;
        if (iLCType == 1)
        {
            if (Get_ScaleUpFactor(D_LOADCASE_SPECTRUM, iLCKey1, dScaleUpFactor1))
            {
                sLc1Fac.Format(_T("(1.0*%s)"), GetFactorString(dScaleUpFactor1));
            }
            if (Get_ScaleUpFactor(D_LOADCASE_SPECTRUM, iLCKey2, dScaleUpFactor2))
            {
                sLc2Fac.Format(_T("(0.3*%s)"), GetFactorString(dScaleUpFactor2));
            }
            if (iLCKey3>0 && Get_ScaleUpFactor(D_LOADCASE_SPECTRUM, iLCKey3, dScaleUpFactor3))
            {
                sLc3Fac.Format(_T("(0.3*%s)"), GetFactorString(dScaleUpFactor3));
            }
        }
        for (int nLC1 = 0; nLC1 < nSize; ++nLC1){
            for (int nLC2 = 0; nLC2 < nSize; ++nLC2){
                for (int nLC3 = 0; nLC3 < nSizeV; ++nLC3)
                {
                    T_LCOM_D rData_Temp = rData;
                    int LoadCaseNo_Temp = rData_Temp.aCombination.GetSize();
                    CString Sign1 = nLC1==0 ? _T("") : _T("-");
                    CString Sign2 = nLC2==0 ? _T("+") : _T("-");
                    CString Sign3 = nLC3==0 ? _T("+") : _T("-");
                    if (iLCType == 1)
                    {
                        Set_SpectrumCase(iLCKey1, dEFactor*dScaleUpFactor1*dLc1Fac[nLC1], LoadCaseNo_Temp, rData_Temp, TRUE);
                        Set_SpectrumCase(iLCKey2, dEFactor*dScaleUpFactor2*dLc2Fac[nLC2], LoadCaseNo_Temp, rData_Temp, TRUE);
                        if (iLCKey3>0) Set_SpectrumCase(iLCKey3, dEFactor*dScaleUpFactor3*dLc3Fac[nLC3], LoadCaseNo_Temp, rData_Temp, TRUE);
                    }
                    else
                    {
                        Set_StaticCase(iLCKey1, dEFactor*dLc1Fac[nLC1], LoadCaseNo_Temp, rData_Temp);
                        Set_StaticCase(iLCKey2, dEFactor*dLc2Fac[nLC2], LoadCaseNo_Temp, rData_Temp);
                        if (iLCKey3>0) Set_StaticCase(iLCKey3, dEFactor*dLc3Fac[nLC3], LoadCaseNo_Temp, rData_Temp);
                    }
                    rData_Temp.Description += csPrefix + GetFactorString(dEFactor) + _T("[") + Sign1 + sLc1Fac + sLc1Name + sLcEQ;
                    rData_Temp.Description += Sign2 + sLc2Fac + sLc2Name + sLcEQ;
                    if (iLCKey3>0) rData_Temp.Description += Sign3 + sLc3Fac + sLc3Name + sLcEQ + _T("]");
                    else           rData_Temp.Description += _T("]");
                    // 
                    if (LoadCaseNo_Temp > rData.aCombination.GetSize())
                    {
                        if (m_nLcomType != D_LCOMTYPE_GENERAL) rData_Temp.nActive = 1;
                        rData_Temp.nSeisLcomType = nSubType;
                        rAddLcom.Add(rData_Temp);
                    }
                }
            }
        }
    };

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;
    raTotalLcom.RemoveAll();
    for (int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];

        const int nOrthoCombType = Get_OrthoCombType();

        for (int i = 0; i < nOrtho; ++i)
        {
            const int iLCKey1 = m_a100_30_Load[i].iLCKey1;
            const int iLCKey2 = m_a100_30_Load[i].iLCKey2;
            const int iLCKey3 = m_a100_30_Load[i].bLCase3 ? m_a100_30_Load[i].iLCKey3 : 0;

            lambda_GetDefineOrthoLoad(m_a100_30_Load[i].nLoadCaseType, iLCKey1, iLCKey2, iLCKey3, LONGI_TYPE, nOrthoCombType, dEFactor, csPrefix_Tmp, LcomD, raTotalLcom);
            lambda_GetDefineOrthoLoad(m_a100_30_Load[i].nLoadCaseType, iLCKey1, iLCKey2, iLCKey3, TRANS_TYPE, nOrthoCombType, dEFactor, csPrefix_Tmp, LcomD, raTotalLcom);
            if (iLCKey3>0) lambda_GetDefineOrthoLoad(m_a100_30_Load[i].nLoadCaseType, iLCKey1, iLCKey2, iLCKey3, VERTI_TYPE, nOrthoCombType, dEFactor, csPrefix_Tmp, LcomD, raTotalLcom);
        }
    }
    if (raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_CO(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arIpLoad, ArINT& arCtLoad, ArINT& arCvLoad, double& dEFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
         if(enLcomNo == EN_AASHTOSince12_NO_ETR2_01 && m_nIpCount == 0) return;
    else if(enLcomNo == EN_AASHTOSince12_NO_ETR2_02 && m_nCtCount == 0) return;
    else if(enLcomNo == EN_AASHTOSince12_NO_ETR2_03 && m_nCvCount == 0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();
        int nIP=0, nCT=0, nCV=0, nCaseNum=nExistCase;

        if (enLcomNo == EN_AASHTOSince12_NO_ETR2_01)
        {
            for(int j=0; j<m_nIpCount; j++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(j),  dEFactor,  nCaseNum, LcomD, 0);
        }
        else if (enLcomNo == EN_AASHTOSince12_NO_ETR2_02)
        {
            for(int j=0; j<m_nCtCount; j++) nCT += Set_STLCIncludeCS(arCtLoad.GetAt(j),  dEFactor,  nCaseNum, LcomD, 0);
        }
        else if (enLcomNo == EN_AASHTOSince12_NO_ETR2_03)
        {
            for(int j=0; j<m_nCvCount; j++) nCV += Set_STLCIncludeCS(arCvLoad.GetAt(j),  dEFactor,  nCaseNum, LcomD, 0);
        }

        // Description
        if(nIP > 0 || nCT > 0 || nCV > 0)
        {   
            CString csPrefix(_T("")), csLcom(_T(""));

            if     (enLcomNo == EN_AASHTOSince12_NO_ETR2_01 && nIP>0) {csLcom += csPrefix + GetFactorString(dEFactor) + _T("IP"); csPrefix = csPrefix_Tmp;}
            else if(enLcomNo == EN_AASHTOSince12_NO_ETR2_02 && nCT>0) {csLcom += csPrefix + GetFactorString(dEFactor) + _T("CT"); csPrefix = csPrefix_Tmp;}
            else if(enLcomNo == EN_AASHTOSince12_NO_ETR2_03 && nCV>0) {csLcom += csPrefix + GetFactorString(dEFactor) + _T("CV"); csPrefix = csPrefix_Tmp;}

            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;
        }

        if(nCaseNum > 0)    raTotalLcom.Add(LcomD);

    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_FTG_M(const EN_AASHTOSince12_NO& enLcomNo, ArUNT& arMLoad, ArINT& arLLoad,  ArINT& arIlLoad, ArINT& arCfLoad, double& dLFactor, double& dBrCfFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if (enLcomNo != EN_AASHTOSince12_NO_FAT1 && enLcomNo != EN_AASHTOSince12_NO_FAT2) return;
    if(m_nConstLoad==0 ) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();
    int nMCount = max(m_nMCount, 1);

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        for(int nM = 0; nM < nMCount; ++nM)
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nLL=0, nIL=0, nCF=0, nCaseNum=nExistCase;

            if(m_nMCount>0) Set_MovingCase(arMLoad.GetAt(nM), dLFactor, nCaseNum, LcomD, TRUE);        
            if(m_nLCount > 0)  
            {
                for(int j=0; j<m_nLCount; j++)  nLL += Set_STLCIncludeCS(arLLoad.GetAt(j),   dLFactor, nCaseNum, LcomD, 0);
                for(int j=0; j<m_nIlCount; j++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(j),  dLFactor, nCaseNum, LcomD, 0);
            } 
            for(int j=0; j<m_nCfCount; j++)  nCF += Set_STLCIncludeCS(arCfLoad.GetAt(j),  dBrCfFactor,     nCaseNum, LcomD, 0);
            // Description
            if(nLL > 0 || nIL > 0 || nCF > 0 || m_nMCount > 0)
            {
                CString csPrefix(_T("")), csLcom(_T(""));

                CString strFixM;
                strFixM.Format(_T("%d"), nM+1);
                if(m_nMCount > 0)                  {csLcom += csPrefix + GetFactorString(dLFactor)    + _T("M[") + strFixM + _T("]"); csPrefix = csPrefix_Tmp;}              
                if(m_nLCount > 0)                  {csLcom += csPrefix + GetFactorString(dLFactor)    + _T("L");   csPrefix = csPrefix_Tmp;}  
                if(m_nLCount > 0 && m_nIlCount > 0){csLcom += csPrefix + GetFactorString(dLFactor)    + _T("IL");  csPrefix = csPrefix_Tmp;}  
                if(m_nCfCount> 0)                  {csLcom += csPrefix + GetFactorString(dBrCfFactor) + _T("CF");  csPrefix = csPrefix_Tmp;}
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;

            }
            if(nCaseNum > 0) raTotalLcom.Add(LcomD);
        }
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_FTG_NO_M(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arLLoad,  ArINT& arIlLoad, ArINT& arCfLoad, double& dLFactor, double& dBrCfFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if (enLcomNo != EN_AASHTOSince12_NO_FAT1 && enLcomNo != EN_AASHTOSince12_NO_FAT2) return;
    if(m_nLCount==0 && m_nIlCount==0 && m_nCfCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();
        int nLL=0, nIL=0, nCF=0, nCaseNum=nExistCase;

        for(int j=0; j<m_nLCount;  j++) Set_StaticCase(arLLoad.GetAt(j),   dLFactor,    nCaseNum, LcomD);        
        for(int j=0; j<m_nIlCount; j++) Set_StaticCase(arIlLoad.GetAt(j),  dLFactor,    nCaseNum, LcomD); // Impact Load는 Live Load가 있는 경우만 포함  
        for(int j=0; j<m_nCfCount; j++) Set_StaticCase(arCfLoad.GetAt(j),  dBrCfFactor, nCaseNum, LcomD); // Centrifugal Load는 Live Load가 있는 경우만 포함    

        // Description

        CString csPrefix(_T("")), csLcom(_T(""));

        if(m_nLCount > 0)   {csLcom += csPrefix + GetFactorString(dLFactor)    + _T("L");   csPrefix = csPrefix_Tmp;}  
        if(m_nIlCount> 0)   {csLcom += csPrefix + GetFactorString(dLFactor)    + _T("IL");  csPrefix = csPrefix_Tmp;}  
        if(m_nCfCount> 0)   {csLcom += csPrefix + GetFactorString(dBrCfFactor) + _T("CF");  csPrefix = csPrefix_Tmp;}
        if (csLcom.IsEmpty() || LcomD.Description == strTitle)
        {
            csPrefix = _T("");
        }
        LcomD.Description += csPrefix + csLcom;

        if(nCaseNum > 0) raTotalLcom.Add(LcomD);
    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_FTG_DEAD(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arDwLoad, ArINT& arDcLoad, double& dDFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(m_nLcomType != D_LCOMTYPE_STLCOMP) return; // ST, ST+CS
    if(m_nConstLoad==1) return;
    if(m_nDcCount==0 && m_nDwCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);


    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();
        int nDcL = 0, nDwL = 0, nCaseNum=nExistCase;
        for(int j=0; j<m_nDcCount; j++)  nDcL += Set_STLCIncludeCS(arDcLoad.GetAt(j),  dDFactor, nCaseNum, LcomD, D_SGLD_DL_CVL);
        for(int j=0; j<m_nDwCount; j++)  nDwL += Set_STLCIncludeCS(arDwLoad.GetAt(j),  dDFactor, nCaseNum, LcomD, D_SGLD_DL_CVL);

        // Description
        if(nDcL > 0 || nDwL  > 0)
        {   
            CString csPrefix(_T("")), csLcom(_T(""));

            if(nDcL  > 0) { csLcom += csPrefix + GetFactorString(dDFactor) + _T("DC");  csPrefix = csPrefix_Tmp;}
            if(nDwL  > 0) { csLcom += csPrefix + GetFactorString(dDFactor) + _T("DW");  csPrefix = csPrefix_Tmp;}
            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;
        }

        if(nCaseNum > 0)    raTotalLcom.Add(LcomD);

    }
    if(raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_CS_LIVE(const EN_AASHTOSince12_NO& enLcomNo, BOOL bFatigue, ArINT& arCSLoad, double& dDFactor, double& dLFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom) // D, DC, DW 다 포함
{
    if(bFatigue) // CS, ST+CS Steel Composite인 경우만 CS 함
    {
        if(m_nLcomType != D_LCOMTYPE_STLCOMP) return;
    }

    if(m_nConstLoad==0 || m_nCSCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();
        int nn=0, nCaseNum=nExistCase;

        CString strcEL;
        int nErection = 0;

        if (bFatigue)
        {
            dLFactor = dDFactor;
        }

        for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
        {
            strcEL = _T(""); nErection = 0;
            if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

            if(CheckCSEL(nErection, _T("L")))
            {
                Set_CStageCase(arCSLoad.GetAt(nn), dLFactor, nCaseNum, LcomD, TRUE);
            }
        }

        nErection = 0;

        CString csPrefix(_T("")), csLcom(_T(""));

        for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
        {
            strcEL = _T(""); nErection = 0;
            if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

            if(CheckCSEL(nErection, _T("L")))
            {csLcom += csPrefix + GetFactorString(dLFactor) + strcEL; csPrefix = csPrefix_Tmp;}
        }
        if (csLcom.IsEmpty() || LcomD.Description == strTitle)
        {
            csPrefix = _T("");
        }
        LcomD.Description += csPrefix + csLcom;

        if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
    }
    raNewLcom.Copy(raTotalLcom);
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_CS_W(const EN_AASHTOSince12_NO& enLcomNo, BOOL bFatigue, ArINT& arCSLoad, double& dDFactor, double& dWFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom) // D, DC, DW 다 포함
{
    if(bFatigue) // CS, ST+CS Steel Composite인 경우만 CS 함
    {
        if(m_nLcomType != D_LCOMTYPE_STLCOMP) return;
    }

    if(m_nConstLoad==0 || m_nCSCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();
        int nn=0, nCaseNum=nExistCase;

        CString strcEL;
        int nErection = 0;

        if (bFatigue)
        {
            dWFactor = dDFactor;
        }

        for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
        {
            strcEL = _T(""); nErection = 0;
            if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

            if(CheckCSEL(nErection, _T("W")))
            {
                Set_CStageCase(arCSLoad.GetAt(nn), dWFactor, nCaseNum, LcomD, TRUE);
            }
        }

        nErection = 0;

        CString csPrefix(_T("")), csLcom(_T(""));

        for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
        {
            strcEL = _T(""); nErection = 0;
            if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

            if(CheckCSEL(nErection, _T("W")))
            {csLcom += csPrefix + GetFactorString(dWFactor) + strcEL; csPrefix = csPrefix_Tmp;}
        }
        if (csLcom.IsEmpty() || LcomD.Description == strTitle)
        {
            csPrefix = _T("");
        }
        LcomD.Description += csPrefix + csLcom;

        if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
    }
    raNewLcom.Copy(raTotalLcom);
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_CS_T(const EN_AASHTOSince12_NO& enLcomNo, BOOL bStrength, BOOL bExtreme, BOOL bService, BOOL bFatigue, ArINT& arCSLoad, double& dDFactor, double& dTFactor, double& dTpgFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom) // D, DC, DW 다 포함
{
    if(bFatigue) // CS, ST+CS Steel Composite인 경우만 CS 함
    {
        if(m_nLcomType != D_LCOMTYPE_STLCOMP) return;
    }

    if(m_nConstLoad==0  || m_nCSCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();
        int nn=0, nCaseNum=nExistCase;

        CString strcEL;
        int nErection = 0;

        if (bFatigue)
        {
            dTFactor = dDFactor;
            dTpgFactor = dDFactor;
        }

        for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
        {
            strcEL = _T(""); nErection = 0;
            if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

            if(CheckCSEL(nErection, _T("T")))
            {
                Set_CStageCase(arCSLoad.GetAt(nn), dTFactor, nCaseNum, LcomD, TRUE);
            }

            if (enLcomNo == EN_AASHTOSince12_NO_SER1 || enLcomNo == EN_AASHTOSince12_NO_SER3)
            {
                if(CheckCSEL(nErection, _T("TPG")))
                {
                    Set_CStageCase(arCSLoad.GetAt(nn), dTpgFactor, nCaseNum, LcomD, TRUE);
                }
            }
        }

        nErection = 0;

        CString csPrefix(_T("")), csLcom(_T(""));

        for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
        {
            strcEL = _T(""); nErection = 0;
            if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

            if(CheckCSEL(nErection, _T("T")))
            {csLcom += csPrefix + GetFactorString(dTFactor) + strcEL; csPrefix = csPrefix_Tmp;}
            if (enLcomNo == EN_AASHTOSince12_NO_SER1 || enLcomNo == EN_AASHTOSince12_NO_SER3)
            {
                if(CheckCSEL(nErection, _T("TPG")))
                {csLcom += csPrefix + GetFactorString(dTpgFactor) + strcEL; csPrefix = csPrefix_Tmp;}
            }

        }
        if (csLcom.IsEmpty() || LcomD.Description == strTitle)
        {
            csPrefix = _T("");
        }
        LcomD.Description += csPrefix + csLcom;

        if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
    }
    raNewLcom.Copy(raTotalLcom);
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_CS_STL(const EN_AASHTOSince12_NO& enLcomNo, BOOL bStrength, BOOL bExtreme, BOOL bService, BOOL bFatigue, ArINT& arCSLoad,  double& dDFactor, double& dStlFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom) // D, DC, DW 다 포함
{
    if(enLcomNo == EN_AASHTOSince12_NO_STR4) return;
    if(bExtreme) return;
    if(enLcomNo == EN_AASHTOSince12_NO_SER2 || enLcomNo == EN_AASHTOSince12_NO_SER4) return;
    if(bFatigue) // CS, ST+CS Steel Composite인 경우만 CS 함
    {
        if(m_nLcomType != D_LCOMTYPE_STLCOMP) return;
    }

    if(m_nConstLoad==0  || m_nCSCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();
        int nn=0, nCaseNum=nExistCase;

        CString strcEL;
        int nErection = 0;

        if (bFatigue)
        {
            dStlFactor = dDFactor;
        }

        for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
        {
            strcEL = _T(""); nErection = 0;
            if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

            if(CheckCSEL(nErection, _T("STL")))
            {
                Set_CStageCase(arCSLoad.GetAt(nn), dStlFactor, nCaseNum, LcomD, TRUE);
            }
        }

        nErection = 0;

        CString csPrefix(_T("")), csLcom(_T(""));

        for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
        {
            strcEL = _T(""); nErection = 0;
            if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

            if(CheckCSEL(nErection, _T("STL")))
            {csLcom += csPrefix + GetFactorString(dStlFactor) + strcEL; csPrefix = csPrefix_Tmp;}

        }
        if (csLcom.IsEmpty() || LcomD.Description == strTitle)
        {
            csPrefix = _T("");
        }
        LcomD.Description += csPrefix + csLcom;

        if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
    }
    raNewLcom.Copy(raTotalLcom);
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_CS_E(const EN_AASHTOSince12_NO& enLcomNo, ArINT& arCSLoad, double& dEFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom) // D, DC, DW 다 포함
{
    if(enLcomNo != EN_AASHTOSince12_NO_ETR1_01 && enLcomNo != EN_AASHTOSince12_NO_SEIS_EQ) return;
    if(m_nConstLoad==0  || m_nCSCount==0) return;

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

    for(int i=0; i<nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();
        int nn=0, nCaseNum=nExistCase;

        CString strcEL;
        int nErection = 0;

        for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
        {
            strcEL = _T(""); nErection = 0;
            if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

            if(CheckCSEL(nErection, _T("E")))
            {
                Set_CStageCase(arCSLoad.GetAt(nn), dEFactor, nCaseNum, LcomD, TRUE);
            }
        }

        nErection = 0;

        CString csPrefix(_T("")), csLcom(_T(""));

        for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
        {
            strcEL = _T(""); nErection = 0;
            if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

            if(CheckCSEL(nErection, _T("E")))
            {csLcom += csPrefix + GetFactorString(dEFactor) + strcEL; csPrefix = csPrefix_Tmp;}

        }
        if (csLcom.IsEmpty() || LcomD.Description == strTitle)
        {
            csPrefix = _T("");
        }
        LcomD.Description += csPrefix + csLcom;

        if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
    }
    raNewLcom.Copy(raTotalLcom);
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_CS_DW(const EN_AASHTOSince12_NO& enLcomNo, BOOL bStrength, BOOL bExtreme, BOOL bService, BOOL bFatigue, ArINT& arCSLoad, double& dDw1,  double& dDw2, double& dDFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(bFatigue) // CS, ST+CS Steel Composite인 경우만 CS 함
    {
        if(m_nLcomType != D_LCOMTYPE_STLCOMP) return;
    }

    if(m_nCSCount == 0 || m_nConstLoad == 0) return; // Static only or no CS

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;
    const int nDwNum = m_nAshLr12DwType == 2 ? 2 : 1;

    double dDwFactor = 0.0;
    int nTempDwNum = nDwNum;
    
    if (m_nConstLoad == 1)
    {
        nTempDwNum = 1;
    }
    else
    {
        if (nDwNum == 2)
        {
            nTempDwNum = 2;
        }
        else
        {
            nTempDwNum = 1;
        }
    }

    for(int nDw = 0; nDw  < nDwNum; ++nDw)
    {
        for(int i=0; i<nLcom; ++i)
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nn=0, nCaseNum=nExistCase;

            if (m_nAshLr12DwType == 0 || (m_nAshLr12DwType == 2 && nDw == 0))
            {
                dDwFactor = dDw1;
            }
            else
            {
                dDwFactor = dDw2;
            }

            if (nTempDwNum == 2)
            {
                for(int j=0; j<nExistCase; ++j )
                {
                    if(LcomD.aCombination[j].AnalType == 1 && (LcomD.aCombination[j].Factor == dDw1 || LcomD.aCombination[j].Factor == dDw2))
                    {
                        dDwFactor = LcomD.aCombination[j].Factor;
                        break;
                    }
                }
            }

            if (bService || bFatigue )
            {
                dDwFactor = dDFactor;
            }

            CString strcEL;
            int nErection = 0;

            for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
            {
                strcEL = _T(""); nErection = 0;
                if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

                if(CheckCSEL(nErection, _T("DW")))
                {
                    Set_CStageCase(arCSLoad.GetAt(nn), dDwFactor, nCaseNum, LcomD, TRUE);
                }
            }

            nErection = 0;

            CString csPrefix(_T("")), csLcom(_T(""));

            for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
            {
                strcEL = _T(""); nErection = 0;
                if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

                if(CheckCSEL(nErection, _T("DW")))
                {csLcom += csPrefix + GetFactorString(dDwFactor) + strcEL; csPrefix = csPrefix_Tmp;}
            }
            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;
            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }

    raNewLcom.Copy(raTotalLcom);
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_CS_DC(const EN_AASHTOSince12_NO& enLcomNo, BOOL bStrength, BOOL bExtreme, BOOL bService, BOOL bFatigue, ArINT& arCSLoad, double& dDc1,  double& dDc2, double& dDFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(bFatigue) // CS, ST+CS Steel Composite인 경우만 CS 함
    {
        if(m_nLcomType != D_LCOMTYPE_STLCOMP) return;
    }

    if(m_nCSCount == 0 || m_nConstLoad == 0) return; // Static only or no CS

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;
    const int nDcNum = m_nAshLr12DcType == 2 ? 2 : 1;

    double dDcFactor = 0.0;
    int nTempDcNum = 0;

    if (m_nConstLoad == 1)
    {
        nTempDcNum = 1;
    }
    else
    {
        if (nDcNum == 2)
        {
            nTempDcNum = 2;
        }
        else
        {
            nTempDcNum = 1;
        }
    }

    for(int nDc = 0; nDc  < nDcNum; ++nDc)
    {
        for(int i=0; i<nLcom; ++i)
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nn=0, nCaseNum=nExistCase;

			if (m_nAshLr12DcType == 0 || (m_nAshLr12DcType == 2 && nDc == 0))
            {
                dDcFactor = dDc1;
            }
            else
            {
                dDcFactor = dDc2;
            }

            if (nTempDcNum == 2)
            {
                for(int j=0; j<nExistCase; ++j )
                {
                    if(LcomD.aCombination[j].AnalType == 1 && (LcomD.aCombination[j].Factor == dDc1 || LcomD.aCombination[j].Factor == dDc2))
                    {
                        dDcFactor = LcomD.aCombination[j].Factor;
                        break;
                    }
                }
            }

            if (bService || bFatigue )
            {
                dDcFactor = dDFactor;
            }

            if(CheckCS_SgldK(D_SGLD_DL_CVL, nn)) Set_CStageCase(arCSLoad.GetAt(nn), dDcFactor, nCaseNum, LcomD, TRUE);

            CString strcEL;
            int nErection = 0;

            for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
            {
                strcEL = _T(""); nErection = 0;
                if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

                if(CheckCSEL(nErection, _T("D")) || CheckCSEL(nErection, _T("DC")))
                {
                    Set_CStageCase(arCSLoad.GetAt(nn), dDcFactor, nCaseNum, LcomD, TRUE);
                }
            }

            nErection = 0;

            CString csPrefix(_T("")), csLcom(_T(""));

            if(CheckCS_SgldK(D_SGLD_DL_CVL, nn))
            {csLcom += csPrefix + GetFactorString(dDcFactor) + _T("(cD)"); csPrefix = csPrefix_Tmp;}

            for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
            {
                strcEL = _T(""); nErection = 0;
                if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

                if(CheckCSEL(nErection, _T("D")) || CheckCSEL(nErection, _T("DC")))
                {csLcom += csPrefix + GetFactorString(dDcFactor) + strcEL; csPrefix = csPrefix_Tmp;}
            }
            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;

            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    raNewLcom.Copy(raTotalLcom);
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_CS_PS(const EN_AASHTOSince12_NO& enLcomNo, BOOL bStrength, BOOL bExtreme, BOOL bService, BOOL bFatigue, ArINT& arCSLoad, double& dPs1, double& dPs2, double& dDFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if(bFatigue) return;
    if(m_nConstLoad == 0  || m_nCSCount == 0) return;// Static only or no CS

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;
    const int nPsNum = m_nAshLr12PsType == 2 ? 2 : 1;

    double dPsFactor = 0.0;
    for(int nPs = 0; nPs < nPsNum; ++nPs)
    {
        for(int i=0; i<nLcom; ++i)
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nn=0, nCaseNum=nExistCase;

			if (m_nAshLr12PsType == 0 || (m_nAshLr12PsType == 2 && nPs == 0))
            {
                dPsFactor = dPs1;
            }
            else
            {
                dPsFactor = dPs2;
            }

            CString strcEL;
            int nErection = 0;

            for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
            {
                strcEL = _T(""); nErection = 0;
                if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }
            }

            if (bService)
            {
                if (m_nAshLr12Deform == 0) // Deform Check
                {
                    if(CheckCS_SgldK(D_SGLD_TP_CVL, nn))  Set_CStageCase(arCSLoad.GetAt(nn), dDFactor, nCaseNum, LcomD, TRUE);
                }
                else // All other
                {
                    if(CheckCS_SgldK(D_SGLD_TP_CVL, nn))  Set_CStageCase(arCSLoad.GetAt(nn), dDFactor, nCaseNum, LcomD, TRUE);
                    if(CheckCS_SgldK(D_SGLD_TS_CVL, nn))  Set_CStageCase(arCSLoad.GetAt(nn), dDFactor, nCaseNum, LcomD, TRUE);
                }
            }
            else
            {
                if (m_nAshLr12Deform == 0) // Deform Check
                {
                    if(CheckCS_SgldK(D_SGLD_TP_CVL, nn))  Set_CStageCase(arCSLoad.GetAt(nn), dPsFactor, nCaseNum, LcomD, TRUE);
                }
                else // All other
                {
                    if(CheckCS_SgldK(D_SGLD_TS_CVL, nn))  Set_CStageCase(arCSLoad.GetAt(nn), dPsFactor, nCaseNum, LcomD, TRUE);
                }
            }


            CString csPrefix(_T("")), csLcom(_T(""));

            for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
            {
                strcEL = _T(""); nErection = 0;
                if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

            }

            if (bService)
            {
                if(CheckCS_SgldK(D_SGLD_TP_CVL, nn))
                {csLcom += csPrefix + GetFactorString(dDFactor) + _T("(cTP)"); csPrefix = csPrefix_Tmp;}
            }
            else
            {
                if (m_nAshLr12Deform == 0) // Deform Check
                {
                    if(CheckCS_SgldK(D_SGLD_TP_CVL, nn))
                    {csLcom += csPrefix + GetFactorString(dPsFactor) + _T("(cTP)"); csPrefix = csPrefix_Tmp;}
                }
                else // All other
                {
                    if(CheckCS_SgldK(D_SGLD_TS_CVL, nn))
                    {csLcom += csPrefix + GetFactorString(dPsFactor) + _T("(cTs)"); csPrefix = csPrefix_Tmp;}
                }
            }


            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;

            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    raNewLcom.Copy(raTotalLcom);
}

void CLoadCombCtrl::Add_CvlAASHTOSince12_CS_CR_SH(const EN_AASHTOSince12_NO& enLcomNo, BOOL bStrength, BOOL bExtreme, BOOL bService, BOOL bFatigue, ArINT& arCSLoad, double& dCr1, double& dCr2, double& dTFactor, double& dDFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    // 설계기준 분석을 통해 Creep 이나 Shrinkage는 Factor 값이 항상 같음을 고려
    if(bFatigue) // CS, ST+CS Steel Composite인 경우만 CS 함
    {
        if(m_nLcomType != D_LCOMTYPE_STLCOMP) return;
    }
    if(m_nConstLoad == 0  || m_nCSCount == 0) return;// Static only or no CS

    const CString& strTitle = Add_CvlAASHTOSince12_Title(enLcomNo);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;
    const int nCrNum = m_nAshLr12CrType == 2 ? 2 : 1;

    double dCrFactor = 0.0;
    double dShFactor = 0.0;

    int nTempCrNum = 0;

    if (m_nConstLoad == 1)
    {
        nTempCrNum = 1;
    }
    else
    {
        if (nCrNum == 2)
        {
            nTempCrNum = 2;
        }
        else
        {
            nTempCrNum = 1;
        }
    }

    for(int nCr = 0; nCr < nCrNum; ++nCr)
    {
        for(int i=0; i<nLcom; ++i)
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nn=0, nCaseNum=nExistCase;

            if (m_nAshLr12CrType == 0 || (m_nAshLr12CrType == 2 && nCr == 0))
            {
                dCrFactor = dCr1;
            }
            else
            {
                dCrFactor = dCr2;
            }

            if (nTempCrNum == 2)
            {
                for(int j=0; j<nExistCase; ++j )
                {
                    if(LcomD.aCombination[j].AnalType == 13 && (LcomD.aCombination[j].Factor == dCr1 || LcomD.aCombination[j].Factor == dCr2))
                    {
                        dCrFactor = LcomD.aCombination[j].Factor;
                        break;
                    }
                }
            }

            /// [CIVIL-9263] 관련 규정이 없어서 고려하지 않음.
//             if (enLcomNo == EN_AASHTOSince12_NO_STR3 || enLcomNo == EN_AASHTOSince12_NO_STR4)
//             {
//                 dCrFactor = dTFactor;
//             }

            dShFactor = dCrFactor;

            CString strcEL;
            int nErection = 0;

            for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
            {
                strcEL = _T(""); nErection = 0;
                if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }
            }

            if (bFatigue ) // CS, ST+CS Steel Composite인 경우만 CS 함
            {
                if(CheckCS_SgldK(D_SGLD_CS_CVL, nn))  Set_CStageCase(arCSLoad.GetAt(nn), dDFactor, nCaseNum, LcomD, TRUE);
                if(CheckCS_SgldK(D_SGLD_SS_CVL, nn))  Set_CStageCase(arCSLoad.GetAt(nn), dDFactor, nCaseNum, LcomD, TRUE);
            }
            else if (bService)
            {
                if (m_nAshLr12Deform == 0) // Deform Check
                {
                    if(CheckCS_SgldK(D_SGLD_CP_CVL, nn))  Set_CStageCase(arCSLoad.GetAt(nn), dDFactor, nCaseNum, LcomD, TRUE);
                    if(CheckCS_SgldK(D_SGLD_SP_CVL, nn))  Set_CStageCase(arCSLoad.GetAt(nn), dDFactor, nCaseNum, LcomD, TRUE);
                }
                else // All other
                {
                    if(CheckCS_SgldK(D_SGLD_CS_CVL, nn))  Set_CStageCase(arCSLoad.GetAt(nn), dDFactor, nCaseNum, LcomD, TRUE);
                    if(CheckCS_SgldK(D_SGLD_SS_CVL, nn))  Set_CStageCase(arCSLoad.GetAt(nn), dDFactor, nCaseNum, LcomD, TRUE);
                }
            }
            else
            {
                if (m_nAshLr12Deform == 0) // Deform Check
                {
                    if(CheckCS_SgldK(D_SGLD_CP_CVL, nn))  Set_CStageCase(arCSLoad.GetAt(nn), dCrFactor, nCaseNum, LcomD, TRUE);
                    if(CheckCS_SgldK(D_SGLD_SP_CVL, nn))  Set_CStageCase(arCSLoad.GetAt(nn), dShFactor, nCaseNum, LcomD, TRUE);
                }
                else // All other
                {
                    if(CheckCS_SgldK(D_SGLD_CS_CVL, nn))  Set_CStageCase(arCSLoad.GetAt(nn), dCrFactor, nCaseNum, LcomD, TRUE);
                    if(CheckCS_SgldK(D_SGLD_SS_CVL, nn))  Set_CStageCase(arCSLoad.GetAt(nn), dShFactor, nCaseNum, LcomD, TRUE);
                }
            }

            CString csPrefix(_T("")), csLcom(_T(""));

            for(int nErec=0; nErec<D_STCT_EREC_MAX_CVL; ++nErec)
            {
                strcEL = _T(""); nErection = 0;
                if(CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

            }


            if (bFatigue) // CS, ST+CS Steel Composite인 경우만 CS 함
            {
                if (CheckCS_SgldK(D_SGLD_CS_CVL, nn))
                {
                    csLcom += csPrefix + GetFactorString(dDFactor) + _T("(cCR)"); csPrefix = csPrefix_Tmp;
                }
                if (CheckCS_SgldK(D_SGLD_SS_CVL, nn))
                {
                    csLcom += csPrefix + GetFactorString(dDFactor) + _T("(cSH)"); csPrefix = csPrefix_Tmp;
                }
            }
            else if (bService)
            {
                if (CheckCS_SgldK(D_SGLD_CP_CVL, nn))
                {
                    csLcom += csPrefix + GetFactorString(dDFactor) + _T("(cCR)"); csPrefix = csPrefix_Tmp;
                }
                if (CheckCS_SgldK(D_SGLD_CS_CVL, nn))
                {
                    csLcom += csPrefix + GetFactorString(dDFactor) + _T("(cSH)"); csPrefix = csPrefix_Tmp;
                }
            }
            else
            {
                if (m_nAshLr12Deform == 0) // Deform Check
                {
                    if (CheckCS_SgldK(D_SGLD_CP_CVL, nn))
                    {
                        csLcom += csPrefix + GetFactorString(dCrFactor) + _T("(cCR)"); csPrefix = csPrefix_Tmp;
                    }
                    if (CheckCS_SgldK(D_SGLD_CS_CVL, nn))
                    {
                        csLcom += csPrefix + GetFactorString(dShFactor) + _T("(cSH)"); csPrefix = csPrefix_Tmp;
                    }
                }
                else // All other
                {
                    if (CheckCS_SgldK(D_SGLD_CS_CVL, nn))
                    {
                        csLcom += csPrefix + GetFactorString(dCrFactor) + _T("(cCR)"); csPrefix = csPrefix_Tmp;
                    }
                    if (CheckCS_SgldK(D_SGLD_SS_CVL, nn))
                    {
                        csLcom += csPrefix + GetFactorString(dShFactor) + _T("(cSH)"); csPrefix = csPrefix_Tmp;
                    }
                }
            }


            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;

            if(nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    raNewLcom.Copy(raTotalLcom);
}

////////////////////////////////////////////////// IRS SBC LOAD COMBINATIONS///////////////////////

BOOL CLoadCombCtrl::SetCvlIRSSBC_DW(ArINT &arDLoad, ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arBLLoad, ArUNT &arMLoad)
{
    T_LCOM_D LcomD;

    int nDCount = 1;
    BOOL bExistD = (m_nDCount > 0 || m_nDwCount > 0 || m_nDcCount > 0 );
    if (bExistD ) nDCount = 1;
    int nMLCount = max(m_nMCount, 1);

    double dDFactor = 1.0;

    CString csPrefix(_T("")), strFixM(_T("")), strFixW(_T("")), strFixSm(_T(""));

    int LoadCaseNo = 0, ld = 0, nnn = 0;
    for (int d = 0; d < nDCount; ++d)
    {
        for (int j = 0; j < nMLCount; ++j) {
            int nDL = 0, nDc = 0, nSnow = 0, nDw = 0, nPL = 0, nEP = 0, nEV = 0, nES = 0, nEH = 0;
            int nLL = 0, nIL = 0, nLS = 0, nLP = 0, nILP = 0, nCF = 0, nBRK = 0, nBK = 0, nCRL = 0;
            int nB = 0, nWL = 0, nWLL = 0, nTL = 0, nSL = 0, nER = 0;

            LcomD.Initialize();
            LcomD.nActive = 1;
            LoadCaseNo = 0;

            //Permanent
            for (ld = 0; ld < m_nDCount; ++ld) nDL += Set_STLCIncludeCS(arDLoad[ld], dDFactor, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
            for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);

            //for (ld = 0; ld < m_nDcCount; ++ld) nDc += Set_STLCIncludeCS(arDcLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
            //for (ld = 0; ld < m_nDwCount; ++ld) nDw += Set_STLCIncludeCS(arDwLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
            // for(ld=0; ld<m_nLsCount; ++ld) nLS += Set_STLCIncludeCS(arLsLoad[ld], dLsFactor, LoadCaseNo, LcomD, 0);
                   //if(m_nSmCount > 0) Set_SettleCase(arSmLoad[nSmIndex], dSmFactor, LoadCaseNo, LcomD, TRUE);
            if (m_nMCount > 0)  Set_MovingCase(arMLoad.GetAt(j), 1.0, LoadCaseNo, LcomD, TRUE);

            // description
            LcomD.Description.Empty();
            csPrefix.Empty();
            LcomD.Description += _T("1U:");
            if (nDL > 0)
            {
                LcomD.Description += csPrefix + _T("D");
                csPrefix = _T(":");
            }
            if (nDc > 0)
            {
                LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("DC");
                csPrefix = _T("+");
            }
            if (nDw > 0)
            {
                LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("DW");
                csPrefix = _T("+");
            }
            if (nB > 0)
            {
                LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("BL");
                csPrefix = _T("+");
            }
            if (m_nMCount > 0)
            {
                LcomD.Description += csPrefix + _T("ML"); csPrefix = _T(":");
            }


            if (LoadCaseNo > 0) Set_LoadComb4Civil(LcomD, D_STR);
        }
    }
            
        
    

    return TRUE;
}



BOOL CLoadCombCtrl::SetCvlIRSSBC_ICL(ArINT &arDLoad, ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arBLLoad, ArINT &arLLoad, ArINT &arIlLoad, ArINT & arCfLoad, ArINT &arLFLoad, ArUNT &arMLoad)
{
    T_LCOM_D LcomD;

    int nDCount = 1;
    BOOL bExistD = (m_nDCount > 0 || m_nDwCount > 0 || m_nDcCount > 0);
    if (bExistD) nDCount = 1;
    int nMLCount = max(m_nMCount, 1);

    double dDFactor = 1.0;

    CString csPrefix(_T("")), strFixM(_T("")), strFixW(_T("")), strFixSm(_T(""));

    int LoadCaseNo = 0, ld = 0, nnn = 0;
    for (int d = 0; d < nDCount; ++d)
    {
        for (int j = 0; j < nMLCount; ++j) {
            int nDL = 0, nDc = 0, nSnow = 0, nDw = 0, nPL = 0, nEP = 0, nEV = 0, nES = 0, nEH = 0;
            int nLL = 0, nIL = 0, nLS = 0, nLP = 0, nILP = 0, nCF = 0, nBRK = 0, nBK = 0, nCRL = 0;
            int nB = 0, nWL = 0, nWLL = 0, nTL = 0, nSL = 0, nER = 0, nLF = 0;

            LcomD.Initialize();
            LcomD.nActive = 1;
            LoadCaseNo = 0;

            //Permanent
            for (ld = 0; ld < m_nDCount; ++ld) nDL += Set_STLCIncludeCS(arDLoad[ld], dDFactor, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
            for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);

            for (ld = 0; ld < m_nLCount; ++ld) nLL += Set_STLCIncludeCS(arLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
            for (ld = 0; ld < m_nIlCount; ++ld) nIL += Set_STLCIncludeCS(arIlLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
            for (ld = 0; ld < m_nCfCount; ++ld) nCF += Set_STLCIncludeCS(arCfLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
            for (ld = 0; ld < m_nLFCount; ++ld) nLF += Set_STLCIncludeCS(arLFLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);

            if (m_nMCount > 0)  Set_MovingCase(arMLoad.GetAt(j), 1.0, LoadCaseNo, LcomD, TRUE);
            //for (ld = 0; ld < m_nDcCount; ++ld) nDc += Set_STLCIncludeCS(arDcLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
            //for (ld = 0; ld < m_nDwCount; ++ld) nDw += Set_STLCIncludeCS(arDwLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
            // for(ld=0; ld<m_nLsCount; ++ld) nLS += Set_STLCIncludeCS(arLsLoad[ld], dLsFactor, LoadCaseNo, LcomD, 0);
                   //if(m_nSmCount > 0) Set_SettleCase(arSmLoad[nSmIndex], dSmFactor, LoadCaseNo, LcomD, TRUE);

            // description
            LcomD.Description.Empty();
            csPrefix.Empty();
            LcomD.Description += _T("2U:");
            if (nDL > 0)
            {
                LcomD.Description += csPrefix + _T("D");
                csPrefix = _T(":");
            }
            if (nDc > 0)
            {
                LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("DC");
                csPrefix = _T("+");
            }
            if (nDw > 0)
            {
                LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("DW");
                csPrefix = _T("+");
            }
            if (nB > 0)
            {
                LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("BL");
                csPrefix = _T("+");
            }
            if (nLL > 0)
            {
                LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("LL");
                csPrefix = _T("+");
            }
            if (nIL > 0)
            {
                LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("IL");
                csPrefix = _T("+");
            }
            if (nCF > 0)
            {
                LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("CF");
                csPrefix = _T("+");
            }
            if (nLF > 0)
            {
                LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("LF");
                csPrefix = _T("+");
            }
            if (m_nMCount > 0)
            {
                LcomD.Description += csPrefix + _T("ML"); csPrefix = _T(":");
            }
            if (LoadCaseNo > 0) Set_LoadComb4Civil(LcomD, D_STR);
        }
    }




    return TRUE;
}




BOOL CLoadCombCtrl::SetCvlIRSSBC_ICL_T(ArINT &arDLoad, ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arBLLoad, ArINT &arLLoad, ArINT &arIlLoad, ArINT & arCfLoad, ArINT &arLFLoad,ArINT &arTLoad, ArUNT &arMLoad)
{
    T_LCOM_D LcomD;

    int nDCount = 1;
    BOOL bExistD = (m_nDCount > 0 || m_nDwCount > 0 || m_nDcCount > 0);
    if (bExistD) nDCount = 1;
    int nMLCount = max(m_nMCount, 1);

    double dDFactor = 1.0;
    int nTCOunt = (m_nTCount > 0) ? 2.0*m_nTCount : 1;
    double dTempFactor = 1.0;//Accompanying load
    CString csPrefix(_T("")), strFixM(_T("")), strFixW(_T("")), strFixSm(_T(""));

    int LoadCaseNo = 0, ld = 0, nnn = 0;
    for (int d = 0; d < nDCount; ++d)
    {
        for (int q1 = 0; q1 < nTCOunt; ++q1) {
            for (int j = 0; j < nMLCount; ++j) {
                int nDL = 0, nDc = 0, nSnow = 0, nDw = 0, nPL = 0, nEP = 0, nEV = 0, nES = 0, nEH = 0;
                int nLL = 0, nIL = 0, nLS = 0, nLP = 0, nILP = 0, nCF = 0, nBRK = 0, nBK = 0, nCRL = 0;
                int nB = 0, nWL = 0, nT = 0, nTL = 0, nSL = 0, nER = 0, nLF = 0;

                LcomD.Initialize();
                LcomD.nActive = 1;
                LoadCaseNo = 0;

                //Permanent
                for (ld = 0; ld < m_nDCount; ++ld) nDL += Set_STLCIncludeCS(arDLoad[ld], dDFactor, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
                for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);

                for (ld = 0; ld < m_nLCount; ++ld) nLL += Set_STLCIncludeCS(arLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                for (ld = 0; ld < m_nIlCount; ++ld) nIL += Set_STLCIncludeCS(arIlLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                for (ld = 0; ld < m_nCfCount; ++ld) nCF += Set_STLCIncludeCS(arCfLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                for (ld = 0; ld < m_nLFCount; ++ld) nLF += Set_STLCIncludeCS(arLFLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                dTempFactor = (q1 % 2 == 0) ? 1.0 : -1.0;
                if (m_nTCount > 0) nT += Set_STLCIncludeCS(arTLoad[q1 / nTCOunt], dTempFactor, LoadCaseNo, LcomD, 0);

                if (m_nMCount > 0)  Set_MovingCase(arMLoad.GetAt(j), 1.0, LoadCaseNo, LcomD, TRUE);

                //for (ld = 0; ld < m_nDcCount; ++ld) nDc += Set_STLCIncludeCS(arDcLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                //for (ld = 0; ld < m_nDwCount; ++ld) nDw += Set_STLCIncludeCS(arDwLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                // for(ld=0; ld<m_nLsCount; ++ld) nLS += Set_STLCIncludeCS(arLsLoad[ld], dLsFactor, LoadCaseNo, LcomD, 0);
                       //if(m_nSmCount > 0) Set_SettleCase(arSmLoad[nSmIndex], dSmFactor, LoadCaseNo, LcomD, TRUE);

                // description
                LcomD.Description.Empty();
                csPrefix.Empty();
                LcomD.Description += _T("3U:");
                CString strTemp; strTemp.Format(_T("%d"), q1);
                if (nDL > 0)
                {
                    LcomD.Description += csPrefix + _T("D");
                    csPrefix = _T(":");
                }
                if (nDc > 0)
                {
                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("DC");
                    csPrefix = _T("+");
                }
                if (nDw > 0)
                {
                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("DW");
                    csPrefix = _T("+");
                }
                if (nB > 0)
                {
                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("BL");
                    csPrefix = _T("+");
                }
                if (nLL > 0)
                {
                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("LL");
                    csPrefix = _T("+");
                }
                if (nIL > 0)
                {
                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("IL");
                    csPrefix = _T("+");
                }
                if (nCF > 0)
                {
                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("CF");
                    csPrefix = _T("+");
                }
                if (nLF > 0)
                {
                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("LF");
                    csPrefix = _T("+");
                }
                if (m_nTCount > 0)
                {
                    if (nT > 0) { if (dTempFactor < 0.) csPrefix = _T(""); LcomD.Description += csPrefix + GetFactorString(dTempFactor) + _T("Temp") + strTemp + _T("[") + strFixW + _T("]"); csPrefix = _T("+"); }
                    //if(nWLL > 0) {LcomD.Description += csPrefix + GetFactorString(dTempFactor) + _T("WL"); csPrefix = _T("+");}
                }
                if (m_nMCount > 0)
                {
                    LcomD.Description += csPrefix + _T("ML"); csPrefix = _T(":");
                }
                if (LoadCaseNo > 0) Set_LoadComb4Civil(LcomD, D_STR);

            }
        }
    }



    return TRUE;
}



BOOL CLoadCombCtrl::SetCvlIRSSBC_ICL_Wind(ArINT &arDLoad, ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arBLLoad, ArINT &arLLoad, ArINT &arIlLoad, ArINT & arCfLoad, ArINT &arLFLoad, ArINT &arWLoad, ArUNT &arMLoad)
{
    T_LCOM_D LcomD;

    int nDCount = 1;
    BOOL bExistD = (m_nDCount > 0 || m_nDwCount > 0 || m_nDcCount > 0);
    if (bExistD) nDCount = 1;
    int nMLCount = max(m_nMCount, 1);

    double dDFactor = 1.0;
    int nWCount = (m_nWCount > 0) ? m_nWCount * 2 : 1;

    CString csPrefix(_T("")), strFixM(_T("")), strFixW(_T("")), strFixSm(_T(""));
    double dDwFactor = 1.0;
    double dWFactor = m_nIRC6_LSDLVarTraffType == 0 ? 1.0 : 1.0;
    int LoadCaseNo = 0, ld = 0, nnn = 0;
    for (int d = 0; d < nDCount; ++d)
    {
        if (nWCount == 2) dDwFactor = (d == 0) ? 1.0 : -1.0;
        else           dDwFactor = (m_nEC4Permanent == 0) ? 1.0 : -1.0;

        for (int j = 0; j < nWCount; ++j) {
            for (int k = 0; k < nMLCount; ++k) {
                int nDL = 0, nDc = 0, nSnow = 0, nDw = 0, nPL = 0, nEP = 0, nEV = 0, nES = 0, nEH = 0;
                int nLL = 0, nIL = 0, nLS = 0, nLP = 0, nILP = 0, nCF = 0, nBRK = 0, nBK = 0, nCRL = 0;
                int nB = 0, nWL = 0, nWLL = 0, nTL = 0, nSL = 0, nER = 0, nLF = 0;

                LcomD.Initialize();
                LcomD.nActive = 1;
                LoadCaseNo = 0;

                //Permanent
                for (ld = 0; ld < m_nDCount; ++ld) nDL += Set_STLCIncludeCS(arDLoad[ld], dDFactor, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
                for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);

                for (ld = 0; ld < m_nLCount; ++ld) nLL += Set_STLCIncludeCS(arLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                for (ld = 0; ld < m_nIlCount; ++ld) nIL += Set_STLCIncludeCS(arIlLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                for (ld = 0; ld < m_nCfCount; ++ld) nCF += Set_STLCIncludeCS(arCfLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                for (ld = 0; ld < m_nLFCount; ++ld) nLF += Set_STLCIncludeCS(arLFLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);

                double dWF = 1.0;
                if (m_nWCount > 0)
                {
                    dWF = (j % 2 == 0) ? dWFactor : -1. * dWFactor;
                    nWL += Set_STLCIncludeCS(arWLoad[j / 2], dWF, LoadCaseNo, LcomD, 0);
                }
                if (m_nMCount > 0)  Set_MovingCase(arMLoad.GetAt(k), 1.0, LoadCaseNo, LcomD, TRUE);

                // description
                LcomD.Description.Empty();
                csPrefix.Empty();
                LcomD.Description += _T("4U:");
                strFixW.Format(_T("%d"), j / 2 + 1);
                if (nDL > 0)
                {
                    LcomD.Description += csPrefix + _T("D");
                    csPrefix = _T(":");
                }
                if (nDc > 0)
                {
                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("DC");
                    csPrefix = _T("+");
                }
                if (nDw > 0)
                {
                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("DW");
                    csPrefix = _T("+");
                }
                if (nB > 0)
                {
                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("BL");
                    csPrefix = _T("+");
                }
                if (nLL > 0)
                {
                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("LL");
                    csPrefix = _T("+");
                }
                if (nIL > 0)
                {
                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("IL");
                    csPrefix = _T("+");
                }
                if (nCF > 0)
                {
                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("CF");
                    csPrefix = _T("+");
                }
                if (nLF > 0)
                {
                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("LF");
                    csPrefix = _T("+");
                }

                if (m_nWCount > 0)
                {
                    if (nWL > 0) { if (dWF < 0.) csPrefix = _T(""); LcomD.Description += csPrefix + GetFactorString(dWF) + _T("W[") + strFixW + _T("]"); csPrefix = _T("+"); }
                }
                if (m_nMCount > 0)
                {
                    LcomD.Description += csPrefix + _T("ML"); csPrefix = _T(":");
                }
                if (LoadCaseNo > 0) Set_LoadComb4Civil(LcomD, D_STR);

            }
        }
    }



    return TRUE;
}




BOOL CLoadCombCtrl::SetCvlIRSSBC_DW_Wind(ArINT &arDLoad, ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arBLLoad, ArINT &arWLoad, ArUNT &arMLoad)
{
    T_LCOM_D LcomD;

    int nDCount = 1;
    BOOL bExistD = (m_nDCount > 0 || m_nDwCount > 0 || m_nDcCount > 0);
    if (bExistD) nDCount = 1;
    int nMLCount = max(m_nMCount, 1);

    double dDFactor = 1.0;
    int nWCount = (m_nWCount > 0) ? m_nWCount * 2 : 1;

    CString csPrefix(_T("")), strFixM(_T("")), strFixW(_T("")), strFixSm(_T(""));
    double dDwFactor = 1.0;
    double dWFactor = m_nIRC6_LSDLVarTraffType == 0 ? 1.0 : 1.0;
    int LoadCaseNo = 0, ld = 0, nnn = 0;
    for (int d = 0; d < nDCount; ++d)
    {
        if (nWCount == 2) dDwFactor = (d == 0) ? 1.0 : -1.0;
        else           dDwFactor = (m_nEC4Permanent == 0) ? 1.0 : -1.0;

        for (int j = 0; j < nWCount; ++j) {
            for (int k = 0; k < nMLCount; ++k) {
                int nDL = 0, nDc = 0, nSnow = 0, nDw = 0, nPL = 0, nEP = 0, nEV = 0, nES = 0, nEH = 0;
                int nLL = 0, nIL = 0, nLS = 0, nLP = 0, nILP = 0, nCF = 0, nBRK = 0, nBK = 0, nCRL = 0;
                int nB = 0, nWL = 0, nWLL = 0, nTL = 0, nSL = 0, nER = 0, nLF = 0;

                LcomD.Initialize();
                LcomD.nActive = 1;
                LoadCaseNo = 0;

                //Permanent
                for (ld = 0; ld < m_nDCount; ++ld) nDL += Set_STLCIncludeCS(arDLoad[ld], dDFactor, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
                for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);

                double dWF = 1.0;
                if (m_nWCount > 0)
                {
                    dWF = (j % 2 == 0) ? dWFactor : -1. * dWFactor;
                    nWL += Set_STLCIncludeCS(arWLoad[j / 2], dWF, LoadCaseNo, LcomD, 0);
                }
                if (m_nMCount > 0)  Set_MovingCase(arMLoad.GetAt(k), 1.0, LoadCaseNo, LcomD, TRUE);

                // description
                LcomD.Description.Empty();
                csPrefix.Empty();
                LcomD.Description += _T("4U:");
                strFixW.Format(_T("%d"), j / 2 + 1);
                if (nDL > 0)
                {
                    LcomD.Description += csPrefix + _T("D");
                    csPrefix = _T(":");
                }
                if (nDc > 0)
                {
                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("DC");
                    csPrefix = _T("+");
                }
                if (nDw > 0)
                {
                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("DW");
                    csPrefix = _T("+");
                }
                if (nB > 0)
                {
                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("BL");
                    csPrefix = _T("+");
                }
                if (nLL > 0)
                {
                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("LL");
                    csPrefix = _T("+");
                }
                if (nIL > 0)
                {
                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("IL");
                    csPrefix = _T("+");
                }
                if (nCF > 0)
                {
                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("CF");
                    csPrefix = _T("+");
                }
                if (nLF > 0)
                {
                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("LF");
                    csPrefix = _T("+");
                }

                if (m_nWCount > 0)
                {
                    if (nWL > 0) { if (dWF < 0.) csPrefix = _T(""); LcomD.Description += csPrefix + GetFactorString(dWF) + _T("W[") + strFixW + _T("]"); csPrefix = _T("+"); }
                }
                if (m_nMCount > 0)
                {
                    LcomD.Description += csPrefix + _T("ML"); csPrefix = _T(":");
                }
                if (LoadCaseNo > 0) Set_LoadComb4Civil(LcomD, D_STR);

            }
        }
    }



    return TRUE;
}



BOOL CLoadCombCtrl::SetCvlIRSSBC_EQ(int eql, ArINT &arDLoad, ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arBLLoad,
    ArINT &arLLoad, ArINT &arIlLoad, ArINT & arCfLoad, ArINT &arLFLoad,ArINT &arELoad,
    ArUNT& arEspLoad_X, ArUNT& arEspLoad_Y, ArUNT& arEspLoad_Z, ArUNT &arMLoad)
{
    int nMLCount = max(m_nMCount, 1);
    if (m_nConstLoad != 1)
    {
        T_LCOM_D LcomD;

        int nEsp_x = arEspLoad_X.GetSize();
        int nEsp_y = arEspLoad_Y.GetSize();
        int nEsp_z = arEspLoad_Z.GetSize();
        int nTtlESP = nEsp_x + nEsp_y + nEsp_z;

        int nDCount = 1;
        BOOL bExistD = (m_nDCount > 0 || m_nDwCount > 0 || m_nEpCount > 0 || m_nEsCount > 0 ||
            m_nElCount > 0 || m_nCrCount > 0 || m_nShCount > 0);

        int  nnn = 0;

        BOOL bErec = FALSE;
        for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
        {
            if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
        }

        BOOL bAddCsLoad = bErec;

        int nCSCount = ((m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_TP_CVL, nnn)) || (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_TS_CVL, nnn))) ? 2 : 1;

        if (bExistD && m_nEC4Permanent == 2) nDCount = 2;
        int nWCount = 2.*max(m_nWCount, 1);
        int nEPCount = 2.*max(m_nEpCount, 1);
        int nMLCount = m_nMCount;
        double dEFactor_x = 1., dEFactor_y = 1.*0.3;
        int nSmCount = 1;
        int nSmIndex = 0;
        double dRsFactor = 1.;
        int nESPCount_X = (nEsp_x > 0) ? nEsp_x * 2 : 1;
        int nESPCount_Y = (nEsp_y > 0) ? nEsp_y * 2 : 1;
        int nESPCount_Z = (nEsp_z > 0) ? nEsp_z * 2 : 1;

        double dDFactor = 1.0;
        double dDwFactor = 1.0;
        double dLFactor = 0.5;
        double dLsFactor = 0.5;
        double dEPFactor = 0.5;
        double dEhFact = 1.;
        double dErFact = 1.;
        double dPsFactor = 1.0;
        double dWFactor = m_nIRC6_LSDLVarTraffType == 0 ? 0.9 : 0.9;
        double dPsi1Factor = 1.0;
        double dEFactor = 1.0;
        double dTempFactor = 1.;//Accompanying load
        double dCSFactor = 0;
        double dCpShFact = m_bIRSCons_crp_shr_DT_ULS ? 1. : 0;
        CString csPrefix(_T("")), strFixM(_T("")), strFixW(_T("")), strFixE(_T(""));

        int LoadCaseNo = 0, ld = 0;


        //for (int eql=0;eql<nTtlESP;eql++)
        {
            if (eql == 0)
            {
                ////////////// X Major ////////////////
                for (int d = 0; d < nDCount; ++d)
                {
                    dDFactor = 1.;
                    for (int l = 0; l < nCSCount; ++l) {
                        if (l % 2 == 0) dCSFactor = 1.;
                        else     dCSFactor = 1.;
                        for (int m = 0; m < nESPCount_X; m++) {
                            for (int n = 0; n < nESPCount_Y; n++) {
                                for (int o = 0; o < nESPCount_Z; o++) {
                                    for (int j = 0; j < nMLCount; ++j) {

                                        int nDL = 0, nDc = 0, nSnow = 0, nDw = 0, nPL = 0, nEP = 0, nEV = 0, nES = 0, nEH = 0;
                                        int nLL = 0, nIL = 0, nLS = 0, nLP = 0, nILP = 0, nCF = 0, nBRK = 0, nBK = 0, nCRL = 0;
                                        int nB = 0, nWL = 0, nWLL = 0, nTL = 0, nSL = 0, nER = 0, nLF = 0;
                                        LcomD.Initialize();
                                        LcomD.nActive = 1;
                                        LoadCaseNo = 0;

                                        //Permanent
                                        for (ld = 0; ld < m_nDCount; ++ld) nDL += Set_STLCIncludeCS(arDLoad[ld], dDFactor, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
                                        for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);

                                        for (ld = 0; ld < m_nLCount; ++ld) nLL += Set_STLCIncludeCS(arLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        for (ld = 0; ld < m_nIlCount; ++ld) nIL += Set_STLCIncludeCS(arIlLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        for (ld = 0; ld < m_nCfCount; ++ld) nCF += Set_STLCIncludeCS(arCfLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        for (ld = 0; ld < m_nLFCount; ++ld) nLF += Set_STLCIncludeCS(arLFLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);

                                        if (m_nMCount > 0)  Set_MovingCase(arMLoad.GetAt(j), 0.5, LoadCaseNo, LcomD, TRUE);

                                        if (m_bIRSCons_crp_shr_DT_ULS)
                                        {
                                            //for (ld = 0; ld < m_nShCount; ++ld)  nSH += Set_STLCIncludeCS(arShLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
                                            //for (ld = 0; ld < m_nCrCount; ++ld)  nCR += Set_STLCIncludeCS(arCrLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
                                        }
                                        //if (m_nConstLoad != 0) // ST+CS
                                        //{
                                            //if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
                                                //Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor, LoadCaseNo, LcomD, TRUE);

                                            /*CString strcEL;
                                            int nErection = 0;
                                            for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
                                            {
                                                strcEL = _T(""); nErection = 0;
                                                if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }



                                                if (CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")) || CheckCSEL(nErection, _T("PL")) || CheckCSEL(nErection, _T("BL")))
                                                    Set_CStageCase(arCSLoad.GetAt(nnn), dDwFactor, LoadCaseNo, LcomD, TRUE);
                                                else if (CheckCSEL(nErection, _T("D")))
                                                    Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor, LoadCaseNo, LcomD, TRUE);

                                            }*/
                                            //if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
                                            //    Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
                                            //if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
                                            //    Set_CStageCase(arCSLoad.GetAt(nnn), dPsFactor, LoadCaseNo, LcomD, TRUE);
                                            //if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep Primary
                                            //{
                                            //    Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                            //}
                                            //if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep secondary
                                            //{
                                            //    Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                            //}
                                            //if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage Primary
                                            //{
                                            //    Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                            //}
                                            //if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage secondary
                                            //{
                                            //    Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                            //}

                                        //}

                                        double dRS_X = (m % 2 == 0) ? dRsFactor : -1. * dRsFactor;
                                        double dRS_Y = (n % 2 == 0) ? dRsFactor : -1. * dRsFactor;
                                        double dRS_Z = (o % 2 == 0) ? dRsFactor : -1. * dRsFactor;
                                        if (m % 2 == 0 || m % 2 == 1)
                                        {
                                            dRS_Y = 0.3 * dRS_Y;
                                            dRS_Z = 0.3 * dRS_Z;
                                        }
                                        else if (n % 2 == 0)
                                        {
                                            dRS_X = 0.3 * dRS_X;
                                            dRS_Z = 0.3 * dRS_Z;
                                        }
                                        else if (o % 2 == 0)
                                        {
                                            dRS_X = 0.3 * dRS_X;
                                            dRS_Y = 0.3 * dRS_Y;

                                        }
                                        else if (m % 2 == 1 && n % 2 == 1 && o % 2 == 1)
                                        {
                                            dRS_Y = 0.3 * dRS_Y;
                                            dRS_Z = 0.3 * dRS_Z;
                                        }

                                        for (ld = 0; ld < m_nECount; ++ld)
                                        {
                                            if (m_nECount == 1)
                                            {
                                                nES += Set_STLCIncludeCS(arELoad[ld], dEFactor_x, LoadCaseNo, LcomD, 0);
                                            }
                                            else
                                            {
                                                if (ld == 0)            nES += Set_STLCIncludeCS(arELoad[ld], dRS_X, LoadCaseNo, LcomD, 0);
                                                else if (ld % 2 == 1)     nES += Set_STLCIncludeCS(arELoad[ld], dRS_Y, LoadCaseNo, LcomD, 0);
                                                else if (ld % 2 == 0)     nES += Set_STLCIncludeCS(arELoad[ld], dRS_Z, LoadCaseNo, LcomD, 0);

                                            }

                                        }

                                        /*   if (arEspLoad_X.GetSize() > 0) Set_SpectrumCase(arEspLoad_X[m / 2], dRS_X, LoadCaseNo, LcomD, TRUE);
                                           if (arEspLoad_Y.GetSize() > 0) Set_SpectrumCase(arEspLoad_Y[n / 2], dRS_Y, LoadCaseNo, LcomD, TRUE);
                                           if (arEspLoad_Z.GetSize() > 0) Set_SpectrumCase(arEspLoad_Z[o / 2], dRS_Z, LoadCaseNo, LcomD, TRUE);
       */

       // description
                                        LcomD.Description.Empty();
                                        csPrefix.Empty();

                                        LcomD.Description += _T("5U:");
                                        if (nDL > 0)
                                        {
                                            LcomD.Description += csPrefix + _T("D");
                                            csPrefix = _T(":");
                                        }
                                        if (nDc > 0 || nDw > 0 || nPL > 0 || nB > 0)
                                        {
                                            LcomD.Description += csPrefix + _T("SIDL");
                                            csPrefix = _T(":");
                                        }
                                        if (arEspLoad_X.GetSize() > 0)
                                        {
                                            LcomD.Description += csPrefix + GetFactorString(dRS_X) + _T("EQ(X)");
                                            csPrefix = _T(":");
                                        }
                                        if (arEspLoad_Y.GetSize() > 0)
                                        {
                                            LcomD.Description += csPrefix + GetFactorString(dRS_Y) + _T("EQ(Y)");
                                            csPrefix = _T(":");
                                        }
                                        if (arEspLoad_Z.GetSize() > 0)
                                        {
                                            LcomD.Description += csPrefix + GetFactorString(dRS_Z) + _T("EQ(Z)");
                                            csPrefix = _T(":");
                                        }
                                        if (m_nMCount > 0)
                                        {
                                            LcomD.Description += csPrefix + _T("ML"); csPrefix = _T(":");
                                        }
                                        // 
                                        //if (nPS > 0) { LcomD.Description += csPrefix + GetFactorString(dPsFactor) + _T("PS"); csPrefix = _T("+"); }
                                        if (m_nConstLoad != 0) // ST+CS
                                        {
                                            if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("(cD)"); csPrefix = _T("+");
                                            }

                                            CString strcEL;
                                            int nErection = 0;

                                            for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
                                            {
                                                strcEL = _T(""); nErection = 0;
                                                if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }



                                                if (CheckCSEL(nErection, _T("D")) || CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")))
                                                {
                                                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + strcEL; csPrefix = _T("+");
                                                }

                                                else if (CheckCSEL(nErection, _T("W")))
                                                {
                                                    LcomD.Description += csPrefix + GetFactorString(dWFactor) + strcEL; csPrefix = _T("+");
                                                }
                                            }

                                            if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCSFactor) + _T("(cTP)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dPsFactor) + _T("(cTS)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCP)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCS)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSP)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSS)"); csPrefix = _T("+");
                                            }

                                        }
                                        if (LoadCaseNo > 0) Set_LoadComb4Civil(LcomD, D_STR);

                                    }
                                }
                            }
                        }
                    }
                }

            }
            else if (eql % 2 == 1)
            {
                ///////////////////Y MAJOR//////////////////
                for (int d = 0; d < nDCount; ++d)
                {
                    dDFactor = 1.0;
                    for (int l = 0; l < nCSCount; ++l) {
                        if (l % 2 == 0) dCSFactor = 1.;
                        else     dCSFactor = 1.;
                        for (int m = 0; m < nESPCount_X; m++) {
                            for (int n = 0; n < nESPCount_Y; n++) {
                                for (int o = 0; o < nESPCount_Z; o++) {
                                    for (int j = 0; j < nMLCount; ++j) {

                                        int nDL = 0, nDc = 0, nSnow = 0, nDw = 0, nPL = 0, nEP = 0, nEV = 0, nES = 0, nEH = 0;
                                        int nLL = 0, nIL = 0, nLS = 0, nLP = 0, nILP = 0, nCF = 0, nBRK = 0, nBK = 0, nCRL = 0;
                                        int nB = 0, nWL = 0, nWLL = 0, nTL = 0, nSL = 0, nER = 0, nLF = 0;

                                        LcomD.Initialize();
                                        LcomD.nActive = 1;
                                        LoadCaseNo = 0;

                                        //Permanent
                                        for (ld = 0; ld < m_nDCount; ++ld) nDL += Set_STLCIncludeCS(arDLoad[ld], dDFactor, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
                                        for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);

                                        for (ld = 0; ld < m_nLCount; ++ld) nLL += Set_STLCIncludeCS(arLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        for (ld = 0; ld < m_nIlCount; ++ld) nIL += Set_STLCIncludeCS(arIlLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        for (ld = 0; ld < m_nCfCount; ++ld) nCF += Set_STLCIncludeCS(arCfLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        for (ld = 0; ld < m_nLFCount; ++ld) nLF += Set_STLCIncludeCS(arLFLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        if (m_nMCount > 0)  Set_MovingCase(arMLoad.GetAt(j), 0.5, LoadCaseNo, LcomD, TRUE);

                                        //if (m_bIRSCons_crp_shr_DT_ULS)
                                        //{
                                        //    for (ld = 0; ld < m_nShCount; ++ld)  nSH += Set_STLCIncludeCS(arShLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
                                        //    for (ld = 0; ld < m_nCrCount; ++ld)  nCR += Set_STLCIncludeCS(arCrLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
                                        //}
                                        //if (m_nConstLoad != 0) // ST+CS
                                        //{
                                        //    if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
                                        //        Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor, LoadCaseNo, LcomD, TRUE);

                                        //    CString strcEL;
                                        //    int nErection = 0;
                                        //    for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
                                        //    {
                                        //        strcEL = _T(""); nErection = 0;
                                        //        if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }



                                        //        if (CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")) || CheckCSEL(nErection, _T("PL")) || CheckCSEL(nErection, _T("BL")))
                                        //            Set_CStageCase(arCSLoad.GetAt(nnn), dDwFactor, LoadCaseNo, LcomD, TRUE);
                                        //        else if (CheckCSEL(nErection, _T("D")))
                                        //            Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor, LoadCaseNo, LcomD, TRUE);

                                        //    }
                                        //    if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
                                        //        Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
                                        //    if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
                                        //        Set_CStageCase(arCSLoad.GetAt(nnn), dPsFactor, LoadCaseNo, LcomD, TRUE);
                                        //    if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep Primary
                                        //    {
                                        //        Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                        //    }
                                        //    if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep secondary
                                        //    {
                                        //        Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                        //    }
                                        //    if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage Primary
                                        //    {
                                        //        Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                        //    }
                                        //    if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage secondary
                                        //    {
                                        //        Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                        //    }

                                        //}

                                        double dRS_X = (m % 2 == 0) ? dRsFactor : -1. * dRsFactor;
                                        double dRS_Y = (n % 2 == 0) ? dRsFactor : -1. * dRsFactor;
                                        double dRS_Z = (o % 2 == 0) ? dRsFactor : -1. * dRsFactor;
                                        if (m % 2 == 0 || m % 2 == 1)
                                        {
                                            dRS_Y = 0.3 * dRS_Y;
                                            dRS_Z = 0.3 * dRS_Z;
                                        }
                                        else if (n % 2 == 0)
                                        {
                                            dRS_X = 0.3 * dRS_X;
                                            dRS_Z = 0.3 * dRS_Z;
                                        }
                                        else if (o % 2 == 0)
                                        {
                                            dRS_X = 0.3 * dRS_X;
                                            dRS_Y = 0.3 * dRS_Y;

                                        }
                                        else if (m % 2 == 1 && n % 2 == 1 && o % 2 == 1)
                                        {
                                            dRS_Y = 0.3 * dRS_Y;
                                            dRS_Z = 0.3 * dRS_Z;
                                        }

                                        for (ld = 0; ld < m_nECount; ++ld)
                                        {
                                            if (m_nECount == 1)
                                            {
                                                nES += Set_STLCIncludeCS(arELoad[ld], dEFactor_x, LoadCaseNo, LcomD, 0);
                                            }
                                            else
                                            {
                                                if (ld == 0)            nES += Set_STLCIncludeCS(arELoad[ld], dRS_Y, LoadCaseNo, LcomD, 0);
                                                else if (ld % 2 == 1)     nES += Set_STLCIncludeCS(arELoad[ld], dRS_X, LoadCaseNo, LcomD, 0);
                                                else if (ld % 2 == 0)     nES += Set_STLCIncludeCS(arELoad[ld], dRS_Z, LoadCaseNo, LcomD, 0);

                                            }

                                        }

                                        /*    if (arEspLoad_X.GetSize() > 0) Set_SpectrumCase(arEspLoad_X[n / 2], dRS_Y, LoadCaseNo, LcomD, TRUE);
                                            if (arEspLoad_Y.GetSize() > 0) Set_SpectrumCase(arEspLoad_Y[m / 2], dRS_X, LoadCaseNo, LcomD, TRUE);
                                            if (arEspLoad_Z.GetSize() > 0) Set_SpectrumCase(arEspLoad_Z[o / 2], dRS_Z, LoadCaseNo, LcomD, TRUE);
        */

        // description
                                        LcomD.Description.Empty();
                                        csPrefix.Empty();

                                        LcomD.Description += _T("5U:");
                                        if (nDL > 0)
                                        {
                                            LcomD.Description += csPrefix + _T("D");
                                            csPrefix = _T(":");
                                        }
                                        if (nDc > 0 || nDw > 0 || nPL > 0 || nB > 0)
                                        {
                                            LcomD.Description += csPrefix + _T("SIDL");
                                            csPrefix = _T(":");
                                        }
                                        if (arEspLoad_X.GetSize() > 0)
                                        {
                                            LcomD.Description += csPrefix + GetFactorString(dRS_Y) + _T("EQ(X)");
                                            csPrefix = _T(":");
                                        }
                                        if (arEspLoad_Y.GetSize() > 0)
                                        {
                                            LcomD.Description += csPrefix + GetFactorString(dRS_X) + _T("EQ(Y)");
                                            csPrefix = _T(":");
                                        }
                                        if (arEspLoad_Z.GetSize() > 0)
                                        {
                                            LcomD.Description += csPrefix + GetFactorString(dRS_Z) + _T("EQ(Z)");
                                            csPrefix = _T(":");
                                        }
                                        if (m_nMCount > 0)
                                        {
                                            LcomD.Description += csPrefix + _T("ML"); csPrefix = _T(":");
                                        }
                                        // 
                                        //if (nPS > 0) { LcomD.Description += csPrefix + GetFactorString(dPsFactor) + _T("PS"); csPrefix = _T("+"); }
                                        if (m_nConstLoad != 0) // ST+CS
                                        {
                                            if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("(cD)"); csPrefix = _T("+");
                                            }

                                            CString strcEL;
                                            int nErection = 0;

                                            for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
                                            {
                                                strcEL = _T(""); nErection = 0;
                                                if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }



                                                if (CheckCSEL(nErection, _T("D")) || CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")))
                                                {
                                                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + strcEL; csPrefix = _T("+");
                                                }

                                                else if (CheckCSEL(nErection, _T("W")))
                                                {
                                                    LcomD.Description += csPrefix + GetFactorString(dWFactor) + strcEL; csPrefix = _T("+");
                                                }
                                            }

                                            if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCSFactor) + _T("(cTP)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dPsFactor) + _T("(cTS)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCP)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCS)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSP)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSS)"); csPrefix = _T("+");
                                            }
                                        }
                                        if (LoadCaseNo > 0) Set_LoadComb4Civil(LcomD, D_STR);

                                    }
                                }
                            }
                        }
                    }
                }

            }
            else if (eql % 2 == 0)
            {
                //////////////////Z Major/////////////////
                for (int d = 0; d < nDCount; ++d)
                {
                    dDFactor = 1.0;
                    for (int l = 0; l < nCSCount; ++l) {
                        if (l % 2 == 0) dCSFactor = 1.0;
                        else     dCSFactor = 1.;
                        for (int m = 0; m < nESPCount_X; m++) {
                            for (int n = 0; n < nESPCount_Y; n++) {
                                for (int o = 0; o < nESPCount_Z; o++) {
                                    for (int j = 0; j < nMLCount; ++j) {

                                        int nDL = 0, nDc = 0, nSnow = 0, nDw = 0, nPL = 0, nEP = 0, nEV = 0, nES = 0, nEH = 0;
                                        int nLL = 0, nIL = 0, nLS = 0, nLP = 0, nILP = 0, nCF = 0, nBRK = 0, nBK = 0, nCRL = 0;
                                        int nB = 0, nWL = 0, nWLL = 0, nTL = 0, nSL = 0, nER = 0, nLF = 0;

                                        LcomD.Initialize();
                                        LcomD.nActive = 1;
                                        LoadCaseNo = 0;

                                        //Permanent
                                        for (ld = 0; ld < m_nDCount; ++ld) nDL += Set_STLCIncludeCS(arDLoad[ld], dDFactor, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
                                        for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);

                                        for (ld = 0; ld < m_nLCount; ++ld) nLL += Set_STLCIncludeCS(arLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        for (ld = 0; ld < m_nIlCount; ++ld) nIL += Set_STLCIncludeCS(arIlLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        for (ld = 0; ld < m_nCfCount; ++ld) nCF += Set_STLCIncludeCS(arCfLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        for (ld = 0; ld < m_nLFCount; ++ld) nLF += Set_STLCIncludeCS(arLFLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        if (m_nMCount > 0)  Set_MovingCase(arMLoad.GetAt(j), 0.5, LoadCaseNo, LcomD, TRUE);

                                        if (m_bIRSCons_crp_shr_DT_ULS)
                                        {
                                            //for (ld = 0; ld < m_nShCount; ++ld)  nSH += Set_STLCIncludeCS(arShLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
                                            //for (ld = 0; ld < m_nCrCount; ++ld)  nCR += Set_STLCIncludeCS(arCrLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
                                        }
                                        //if (m_nConstLoad != 0) // ST+CS
                                        //{
                                        //    if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
                                        //        Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor, LoadCaseNo, LcomD, TRUE);

                                        //    CString strcEL;
                                        //    int nErection = 0;
                                        //    for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
                                        //    {
                                        //        strcEL = _T(""); nErection = 0;
                                        //        if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }



                                        //        if (CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")) || CheckCSEL(nErection, _T("PL")) || CheckCSEL(nErection, _T("BL")))
                                        //            Set_CStageCase(arCSLoad.GetAt(nnn), dDwFactor, LoadCaseNo, LcomD, TRUE);
                                        //        else if (CheckCSEL(nErection, _T("D")))
                                        //            Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor, LoadCaseNo, LcomD, TRUE);

                                        //    }
                                        //    if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
                                        //        Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
                                        //    if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
                                        //        Set_CStageCase(arCSLoad.GetAt(nnn), dPsFactor, LoadCaseNo, LcomD, TRUE);
                                        //    if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep Primary
                                        //    {
                                        //        Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                        //    }
                                        //    if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep secondary
                                        //    {
                                        //        Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                        //    }
                                        //    if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage Primary
                                        //    {
                                        //        Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                        //    }
                                        //    if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage secondary
                                        //    {
                                        //        Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                        //    }

                                        //}

                                        double dRS_X = (m % 2 == 0) ? dRsFactor : -1. * dRsFactor;
                                        double dRS_Y = (n % 2 == 0) ? dRsFactor : -1. * dRsFactor;
                                        double dRS_Z = (o % 2 == 0) ? dRsFactor : -1. * dRsFactor;
                                        if (m % 2 == 0 || m % 2 == 1)
                                        {
                                            dRS_Y = 0.3 * dRS_Y;
                                            dRS_Z = 0.3 * dRS_Z;
                                        }
                                        else if (n % 2 == 0)
                                        {
                                            dRS_X = 0.3 * dRS_X;
                                            dRS_Z = 0.3 * dRS_Z;
                                        }
                                        else if (o % 2 == 0)
                                        {
                                            dRS_X = 0.3 * dRS_X;
                                            dRS_Y = 0.3 * dRS_Y;

                                        }
                                        else if (m % 2 == 1 && n % 2 == 1 && o % 2 == 1)
                                        {
                                            dRS_Y = 0.3 * dRS_Y;
                                            dRS_Z = 0.3 * dRS_Z;
                                        }

                                        for (ld = 0; ld < m_nECount; ++ld)
                                        {
                                            if (m_nECount == 1)
                                            {
                                                nES += Set_STLCIncludeCS(arELoad[ld], dEFactor_x, LoadCaseNo, LcomD, 0);
                                            }
                                            else
                                            {
                                                if (ld == 0)            nES += Set_STLCIncludeCS(arELoad[ld], dRS_Z, LoadCaseNo, LcomD, 0);
                                                else if (ld % 2 == 1)     nES += Set_STLCIncludeCS(arELoad[ld], dRS_Y, LoadCaseNo, LcomD, 0);
                                                else if (ld % 2 == 0)     nES += Set_STLCIncludeCS(arELoad[ld], dRS_X, LoadCaseNo, LcomD, 0);

                                            }

                                        }

                                        //if (arEspLoad_X.GetSize() > 0) Set_SpectrumCase(arEspLoad_X[o / 2], dRS_Z, LoadCaseNo, LcomD, TRUE);
                                        //if (arEspLoad_Y.GetSize() > 0) Set_SpectrumCase(arEspLoad_Y[n / 2], dRS_Y, LoadCaseNo, LcomD, TRUE);
                                        //if (arEspLoad_Z.GetSize() > 0) Set_SpectrumCase(arEspLoad_Z[m / 2], dRS_X, LoadCaseNo, LcomD, TRUE);


                                        // description
                                        LcomD.Description.Empty();
                                        csPrefix.Empty();

                                        LcomD.Description += _T("5U:");
                                        if (nDL > 0)
                                        {
                                            LcomD.Description += csPrefix + _T("D");
                                            csPrefix = _T(":");
                                        }
                                        if (nDc > 0 || nDw > 0 || nPL > 0 || nB > 0)
                                        {
                                            LcomD.Description += csPrefix + _T("SIDL");
                                            csPrefix = _T(":");
                                        }
                                        if (arEspLoad_X.GetSize() > 0)
                                        {
                                            LcomD.Description += csPrefix + GetFactorString(dRS_Z) + _T("EQ(X)");
                                            csPrefix = _T(":");
                                        }
                                        if (arEspLoad_Y.GetSize() > 0)
                                        {
                                            LcomD.Description += csPrefix + GetFactorString(dRS_Y) + _T("EQ(Y)");
                                            csPrefix = _T(":");
                                        }
                                        if (arEspLoad_Z.GetSize() > 0)
                                        {
                                            LcomD.Description += csPrefix + GetFactorString(dRS_X) + _T("EQ(Z)");
                                            csPrefix = _T(":");
                                        }
                                        if (m_nMCount > 0)
                                        {
                                            LcomD.Description += csPrefix + _T("ML"); csPrefix = _T(":");
                                        }
                                        // 
                                        //if (nPS > 0) { LcomD.Description += csPrefix + GetFactorString(dPsFactor) + _T("PS"); csPrefix = _T("+"); }
                                        if (m_nConstLoad != 0) // ST+CS
                                        {
                                            if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("(cD)"); csPrefix = _T("+");
                                            }

                                            CString strcEL;
                                            int nErection = 0;

                                            for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
                                            {
                                                strcEL = _T(""); nErection = 0;
                                                if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }



                                                if (CheckCSEL(nErection, _T("D")) || CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")))
                                                {
                                                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + strcEL; csPrefix = _T("+");
                                                }

                                                else if (CheckCSEL(nErection, _T("W")))
                                                {
                                                    LcomD.Description += csPrefix + GetFactorString(dWFactor) + strcEL; csPrefix = _T("+");
                                                }
                                            }

                                            if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCSFactor) + _T("(cTP)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dPsFactor) + _T("(cTS)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCP)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCS)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSP)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSS)"); csPrefix = _T("+");
                                            }
                                        }

                                        if (LoadCaseNo > 0) Set_LoadComb4Civil(LcomD, D_STR);
                                    }
                                }
                            }
                        }
                    }
                }

            }
        }

    }


    return TRUE;
}




BOOL CLoadCombCtrl::SetCvlIRSSBC_EQ1(int eql, ArINT &arDLoad, ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arBLLoad,
    ArINT &arLLoad, ArINT &arIlLoad, ArINT & arCfLoad, ArINT &arLFLoad, ArINT &arELoad,
    ArUNT& arEspLoad_X, ArUNT& arEspLoad_Y, ArUNT& arEspLoad_Z, ArUNT &arMLoad)
{
    int nMLCount = max(m_nMCount, 1);
    if (m_nConstLoad != 1)
    {
        T_LCOM_D LcomD;

        int nEsp_x = arEspLoad_X.GetSize();
        int nEsp_y = arEspLoad_Y.GetSize();
        int nEsp_z = arEspLoad_Z.GetSize();
        int nTtlESP = nEsp_x + nEsp_y + nEsp_z;

        int nDCount = 1;
        BOOL bExistD = (m_nDCount > 0 || m_nDwCount > 0 || m_nEpCount > 0 || m_nEsCount > 0 ||
            m_nElCount > 0 || m_nCrCount > 0 || m_nShCount > 0);

        int  nnn = 0;

        BOOL bErec = FALSE;
        for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
        {
            if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
        }

        BOOL bAddCsLoad = bErec;

        int nCSCount = ((m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_TP_CVL, nnn)) || (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_TS_CVL, nnn))) ? 2 : 1;

        if (bExistD && m_nEC4Permanent == 2) nDCount = 2;
        int nWCount = 2.*max(m_nWCount, 1);
        int nEPCount = 2.*max(m_nEpCount, 1);
        int nMLCount = m_nMCount;
        double dEFactor_x = 1., dEFactor_y = 1.*0.3;
        int nSmCount = 1;
        int nSmIndex = 0;
        double dRsFactor = 1.;
        int nESPCount_X = (nEsp_x > 0) ? nEsp_x * 2 : 1;
        int nESPCount_Y = (nEsp_y > 0) ? nEsp_y * 2 : 1;
        int nESPCount_Z = (nEsp_z > 0) ? nEsp_z * 2 : 1;

        double dDFactor = 1.0;
        double dDwFactor = 1.0;
        double dLFactor = 0.5;
        double dLsFactor = 0.5;
        double dEPFactor = 0.5;
        double dEhFact = 1.;
        double dErFact = 1.;
        double dPsFactor = 1.0;
        double dWFactor = m_nIRC6_LSDLVarTraffType == 0 ? 0.9 : 0.9;
        double dPsi1Factor = 1.0;
        double dEFactor = 1.0;
        double dTempFactor = 1.;//Accompanying load
        double dCSFactor = 0;
        double dCpShFact = m_bIRSCons_crp_shr_DT_ULS ? 1. : 0;
        CString csPrefix(_T("")), strFixM(_T("")), strFixW(_T("")), strFixE(_T(""));

        int LoadCaseNo = 0, ld = 0;


        //for (int eql=0;eql<nTtlESP;eql++)
        {
            if (eql == 0)
            {
                ////////////// X Major ////////////////
                for (int d = 0; d < nDCount; ++d)
                {
                    dDFactor = 1.;
                    for (int l = 0; l < nCSCount; ++l) {
                        if (l % 2 == 0) dCSFactor = 1.;
                        else     dCSFactor = 1.;
                        for (int m = 0; m < nESPCount_X; m++) {
                            for (int n = 0; n < nESPCount_Y; n++) {
                                for (int o = 0; o < nESPCount_Z; o++) {
                                    for (int j = 0; j < nMLCount; ++j) {

                                        int nDL = 0, nDc = 0, nSnow = 0, nDw = 0, nPL = 0, nEP = 0, nEV = 0, nES = 0, nEH = 0;
                                        int nLL = 0, nIL = 0, nLS = 0, nLP = 0, nILP = 0, nCF = 0, nBRK = 0, nBK = 0, nCRL = 0;
                                        int nB = 0, nWL = 0, nWLL = 0, nTL = 0, nSL = 0, nER = 0, nLF = 0;
                                        LcomD.Initialize();
                                        LcomD.nActive = 1;
                                        LoadCaseNo = 0;

                                        //Permanent
                                        for (ld = 0; ld < m_nDCount; ++ld) nDL += Set_STLCIncludeCS(arDLoad[ld], dDFactor, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
                                        for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);

                                        for (ld = 0; ld < m_nLCount; ++ld) nLL += Set_STLCIncludeCS(arLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        for (ld = 0; ld < m_nIlCount; ++ld) nIL += Set_STLCIncludeCS(arIlLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        for (ld = 0; ld < m_nCfCount; ++ld) nCF += Set_STLCIncludeCS(arCfLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        for (ld = 0; ld < m_nLFCount; ++ld) nLF += Set_STLCIncludeCS(arLFLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        if (m_nMCount > 0)  Set_MovingCase(arMLoad.GetAt(j), 0.5, LoadCaseNo, LcomD, TRUE);

                                        if (m_bIRSCons_crp_shr_DT_ULS)
                                        {
                                            //for (ld = 0; ld < m_nShCount; ++ld)  nSH += Set_STLCIncludeCS(arShLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
                                            //for (ld = 0; ld < m_nCrCount; ++ld)  nCR += Set_STLCIncludeCS(arCrLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
                                        }
                                        //if (m_nConstLoad != 0) // ST+CS
                                        //{
                                            //if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
                                                //Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor, LoadCaseNo, LcomD, TRUE);

                                            /*CString strcEL;
                                            int nErection = 0;
                                            for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
                                            {
                                                strcEL = _T(""); nErection = 0;
                                                if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }



                                                if (CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")) || CheckCSEL(nErection, _T("PL")) || CheckCSEL(nErection, _T("BL")))
                                                    Set_CStageCase(arCSLoad.GetAt(nnn), dDwFactor, LoadCaseNo, LcomD, TRUE);
                                                else if (CheckCSEL(nErection, _T("D")))
                                                    Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor, LoadCaseNo, LcomD, TRUE);

                                            }*/
                                            //if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
                                            //    Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
                                            //if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
                                            //    Set_CStageCase(arCSLoad.GetAt(nnn), dPsFactor, LoadCaseNo, LcomD, TRUE);
                                            //if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep Primary
                                            //{
                                            //    Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                            //}
                                            //if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep secondary
                                            //{
                                            //    Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                            //}
                                            //if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage Primary
                                            //{
                                            //    Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                            //}
                                            //if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage secondary
                                            //{
                                            //    Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                            //}

                                        //}

                                        double dRS_X = (m % 2 == 0) ? dRsFactor : -1. * dRsFactor;
                                        double dRS_Y = (n % 2 == 0) ? dRsFactor : -1. * dRsFactor;
                                        double dRS_Z = (o % 2 == 0) ? dRsFactor : -1. * dRsFactor;
                                        if (m % 2 == 0 || m % 2 == 1)
                                        {
                                            dRS_Y = 0.3 * dRS_Y;
                                            dRS_Z = 0.3 * dRS_Z;
                                        }
                                        else if (n % 2 == 0)
                                        {
                                            dRS_X = 0.3 * dRS_X;
                                            dRS_Z = 0.3 * dRS_Z;
                                        }
                                        else if (o % 2 == 0)
                                        {
                                            dRS_X = 0.3 * dRS_X;
                                            dRS_Y = 0.3 * dRS_Y;

                                        }
                                        else if (m % 2 == 1 && n % 2 == 1 && o % 2 == 1)
                                        {
                                            dRS_Y = 0.3 * dRS_Y;
                                            dRS_Z = 0.3 * dRS_Z;
                                        }

                                        for (ld = 0; ld < m_nECount; ++ld)
                                        {
                                            if (m_nECount == 1)
                                            {
                                                //nES += Set_STLCIncludeCS(arELoad[ld], dEFactor_x, LoadCaseNo, LcomD, 0);
                                            }
                                            else
                                            {
                                                /* if (ld == 0)            nES += Set_STLCIncludeCS(arELoad[ld], dRS_X, LoadCaseNo, LcomD, 0);
                                                 else if (ld % 2 == 1)     nES += Set_STLCIncludeCS(arELoad[ld], dRS_Y, LoadCaseNo, LcomD, 0);
                                                 else if (ld % 2 == 0)     nES += Set_STLCIncludeCS(arELoad[ld], dRS_Z, LoadCaseNo, LcomD, 0);*/

                                            }

                                        }

                                        if (arEspLoad_X.GetSize() > 0) Set_SpectrumCase(arEspLoad_X[m / 2], dRS_X, LoadCaseNo, LcomD, TRUE);
                                        if (arEspLoad_Y.GetSize() > 0) Set_SpectrumCase(arEspLoad_Y[n / 2], dRS_Y, LoadCaseNo, LcomD, TRUE);
                                        if (arEspLoad_Z.GetSize() > 0) Set_SpectrumCase(arEspLoad_Z[o / 2], dRS_Z, LoadCaseNo, LcomD, TRUE);


                                        // description
                                        LcomD.Description.Empty();
                                        csPrefix.Empty();

                                        LcomD.Description += _T("6U:");
                                        if (nDL > 0)
                                        {
                                            LcomD.Description += csPrefix + _T("D");
                                            csPrefix = _T(":");
                                        }
                                        if (nDc > 0 || nDw > 0 || nPL > 0 || nB > 0)
                                        {
                                            LcomD.Description += csPrefix + _T("SIDL");
                                            csPrefix = _T(":");
                                        }
                                        if (arEspLoad_X.GetSize() > 0)
                                        {
                                            LcomD.Description += csPrefix + GetFactorString(dRS_X) + _T("RS(X)");
                                            csPrefix = _T(":");
                                        }
                                        if (arEspLoad_Y.GetSize() > 0)
                                        {
                                            LcomD.Description += csPrefix + GetFactorString(dRS_Y) + _T("RS(Y)");
                                            csPrefix = _T(":");
                                        }
                                        if (arEspLoad_Z.GetSize() > 0)
                                        {
                                            LcomD.Description += csPrefix + GetFactorString(dRS_Z) + _T("RS(Z)");
                                            csPrefix = _T(":");
                                        }
                                        if (m_nMCount > 0)
                                        {
                                            LcomD.Description += csPrefix + _T("ML"); csPrefix = _T(":");
                                        }
                                        // 
                                        //if (nPS > 0) { LcomD.Description += csPrefix + GetFactorString(dPsFactor) + _T("PS"); csPrefix = _T("+"); }
                                        if (m_nConstLoad != 0) // ST+CS
                                        {
                                            if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("(cD)"); csPrefix = _T("+");
                                            }

                                            CString strcEL;
                                            int nErection = 0;

                                            for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
                                            {
                                                strcEL = _T(""); nErection = 0;
                                                if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }



                                                if (CheckCSEL(nErection, _T("D")) || CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")))
                                                {
                                                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + strcEL; csPrefix = _T("+");
                                                }

                                                else if (CheckCSEL(nErection, _T("W")))
                                                {
                                                    LcomD.Description += csPrefix + GetFactorString(dWFactor) + strcEL; csPrefix = _T("+");
                                                }
                                            }

                                            if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCSFactor) + _T("(cTP)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dPsFactor) + _T("(cTS)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCP)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCS)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSP)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSS)"); csPrefix = _T("+");
                                            }

                                        }
                                        if (LoadCaseNo > 0) Set_LoadComb4Civil(LcomD, D_STR);

                                    }
                                }
                            }
                        }
                    }
                }

            }
            else if (eql % 2 == 1)
            {
                ///////////////////Y MAJOR//////////////////
                for (int d = 0; d < nDCount; ++d)
                {
                    dDFactor = 1.0;
                    for (int l = 0; l < nCSCount; ++l) {
                        if (l % 2 == 0) dCSFactor = 1.;
                        else     dCSFactor = 1.;
                        for (int m = 0; m < nESPCount_X; m++) {
                            for (int n = 0; n < nESPCount_Y; n++) {
                                for (int o = 0; o < nESPCount_Z; o++) {
                                    for (int j = 0; j < nMLCount; ++j) {

                                        int nDL = 0, nDc = 0, nSnow = 0, nDw = 0, nPL = 0, nEP = 0, nEV = 0, nES = 0, nEH = 0;
                                        int nLL = 0, nIL = 0, nLS = 0, nLP = 0, nILP = 0, nCF = 0, nBRK = 0, nBK = 0, nCRL = 0;
                                        int nB = 0, nWL = 0, nWLL = 0, nTL = 0, nSL = 0, nER = 0, nLF = 0;

                                        LcomD.Initialize();
                                        LcomD.nActive = 1;
                                        LoadCaseNo = 0;

                                        //Permanent
                                        for (ld = 0; ld < m_nDCount; ++ld) nDL += Set_STLCIncludeCS(arDLoad[ld], dDFactor, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
                                        for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);

                                        for (ld = 0; ld < m_nLCount; ++ld) nLL += Set_STLCIncludeCS(arLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        for (ld = 0; ld < m_nIlCount; ++ld) nIL += Set_STLCIncludeCS(arIlLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        for (ld = 0; ld < m_nCfCount; ++ld) nCF += Set_STLCIncludeCS(arCfLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        for (ld = 0; ld < m_nLFCount; ++ld) nLF += Set_STLCIncludeCS(arLFLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        if (m_nMCount > 0)  Set_MovingCase(arMLoad.GetAt(j), 0.5, LoadCaseNo, LcomD, TRUE);

                                        //if (m_bIRSCons_crp_shr_DT_ULS)
                                        //{
                                        //    for (ld = 0; ld < m_nShCount; ++ld)  nSH += Set_STLCIncludeCS(arShLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
                                        //    for (ld = 0; ld < m_nCrCount; ++ld)  nCR += Set_STLCIncludeCS(arCrLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
                                        //}
                                        //if (m_nConstLoad != 0) // ST+CS
                                        //{
                                        //    if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
                                        //        Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor, LoadCaseNo, LcomD, TRUE);

                                        //    CString strcEL;
                                        //    int nErection = 0;
                                        //    for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
                                        //    {
                                        //        strcEL = _T(""); nErection = 0;
                                        //        if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }



                                        //        if (CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")) || CheckCSEL(nErection, _T("PL")) || CheckCSEL(nErection, _T("BL")))
                                        //            Set_CStageCase(arCSLoad.GetAt(nnn), dDwFactor, LoadCaseNo, LcomD, TRUE);
                                        //        else if (CheckCSEL(nErection, _T("D")))
                                        //            Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor, LoadCaseNo, LcomD, TRUE);

                                        //    }
                                        //    if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
                                        //        Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
                                        //    if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
                                        //        Set_CStageCase(arCSLoad.GetAt(nnn), dPsFactor, LoadCaseNo, LcomD, TRUE);
                                        //    if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep Primary
                                        //    {
                                        //        Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                        //    }
                                        //    if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep secondary
                                        //    {
                                        //        Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                        //    }
                                        //    if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage Primary
                                        //    {
                                        //        Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                        //    }
                                        //    if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage secondary
                                        //    {
                                        //        Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                        //    }

                                        //}

                                        double dRS_X = (m % 2 == 0) ? dRsFactor : -1. * dRsFactor;
                                        double dRS_Y = (n % 2 == 0) ? dRsFactor : -1. * dRsFactor;
                                        double dRS_Z = (o % 2 == 0) ? dRsFactor : -1. * dRsFactor;
                                        if (m % 2 == 0 || m % 2 == 1)
                                        {
                                            dRS_Y = 0.3 * dRS_Y;
                                            dRS_Z = 0.3 * dRS_Z;
                                        }
                                        else if (n % 2 == 0)
                                        {
                                            dRS_X = 0.3 * dRS_X;
                                            dRS_Z = 0.3 * dRS_Z;
                                        }
                                        else if (o % 2 == 0)
                                        {
                                            dRS_X = 0.3 * dRS_X;
                                            dRS_Y = 0.3 * dRS_Y;

                                        }
                                        else if (m % 2 == 1 && n % 2 == 1 && o % 2 == 1)
                                        {
                                            dRS_Y = 0.3 * dRS_Y;
                                            dRS_Z = 0.3 * dRS_Z;
                                        }

                                        for (ld = 0; ld < m_nECount; ++ld)
                                        {
                                            if (m_nECount == 1)
                                            {
                                                //nES += Set_STLCIncludeCS(arELoad[ld], dEFactor_x, LoadCaseNo, LcomD, 0);
                                            }
                                            else
                                            {
                                                /*         if (ld == 0)            nES += Set_STLCIncludeCS(arELoad[ld], dRS_Y, LoadCaseNo, LcomD, 0);
                                                         else if (ld % 2 == 1)     nES += Set_STLCIncludeCS(arELoad[ld], dRS_X, LoadCaseNo, LcomD, 0);
                                                         else if (ld % 2 == 0)     nES += Set_STLCIncludeCS(arELoad[ld], dRS_Z, LoadCaseNo, LcomD, 0);*/

                                            }

                                        }

                                        if (arEspLoad_X.GetSize() > 0) Set_SpectrumCase(arEspLoad_X[n / 2], dRS_Y, LoadCaseNo, LcomD, TRUE);
                                        if (arEspLoad_Y.GetSize() > 0) Set_SpectrumCase(arEspLoad_Y[m / 2], dRS_X, LoadCaseNo, LcomD, TRUE);
                                        if (arEspLoad_Z.GetSize() > 0) Set_SpectrumCase(arEspLoad_Z[o / 2], dRS_Z, LoadCaseNo, LcomD, TRUE);


                                        // description
                                        LcomD.Description.Empty();
                                        csPrefix.Empty();

                                        LcomD.Description += _T("6U:");
                                        if (nDL > 0)
                                        {
                                            LcomD.Description += csPrefix + _T("D");
                                            csPrefix = _T(":");
                                        }
                                        if (nDc > 0 || nDw > 0 || nPL > 0 || nB > 0)
                                        {
                                            LcomD.Description += csPrefix + _T("SIDL");
                                            csPrefix = _T(":");
                                        }
                                        if (arEspLoad_X.GetSize() > 0)
                                        {
                                            LcomD.Description += csPrefix + GetFactorString(dRS_Y) + _T("RS(X)");
                                            csPrefix = _T(":");
                                        }
                                        if (arEspLoad_Y.GetSize() > 0)
                                        {
                                            LcomD.Description += csPrefix + GetFactorString(dRS_X) + _T("RS(Y)");
                                            csPrefix = _T(":");
                                        }
                                        if (arEspLoad_Z.GetSize() > 0)
                                        {
                                            LcomD.Description += csPrefix + GetFactorString(dRS_Z) + _T("RS(Z)");
                                            csPrefix = _T(":");
                                        }
                                        if (m_nMCount > 0)
                                        {
                                            LcomD.Description += csPrefix + _T("ML"); csPrefix = _T(":");
                                        }
                                        // 
                                        //if (nPS > 0) { LcomD.Description += csPrefix + GetFactorString(dPsFactor) + _T("PS"); csPrefix = _T("+"); }
                                        if (m_nConstLoad != 0) // ST+CS
                                        {
                                            if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("(cD)"); csPrefix = _T("+");
                                            }

                                            CString strcEL;
                                            int nErection = 0;

                                            for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
                                            {
                                                strcEL = _T(""); nErection = 0;
                                                if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }



                                                if (CheckCSEL(nErection, _T("D")) || CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")))
                                                {
                                                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + strcEL; csPrefix = _T("+");
                                                }

                                                else if (CheckCSEL(nErection, _T("W")))
                                                {
                                                    LcomD.Description += csPrefix + GetFactorString(dWFactor) + strcEL; csPrefix = _T("+");
                                                }
                                            }

                                            if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCSFactor) + _T("(cTP)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dPsFactor) + _T("(cTS)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCP)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCS)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSP)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSS)"); csPrefix = _T("+");
                                            }
                                        }
                                        if (LoadCaseNo > 0) Set_LoadComb4Civil(LcomD, D_STR);

                                    }
                                }
                            }
                        }
                    }
                }

            }
            else if (eql % 2 == 0)
            {
                //////////////////Z Major/////////////////
                for (int d = 0; d < nDCount; ++d)
                {
                    dDFactor = 1.0;
                    for (int l = 0; l < nCSCount; ++l) {
                        if (l % 2 == 0) dCSFactor = 1.0;
                        else     dCSFactor = 1.;
                        for (int m = 0; m < nESPCount_X; m++) {
                            for (int n = 0; n < nESPCount_Y; n++) {
                                for (int o = 0; o < nESPCount_Z; o++) {
                                    for (int j = 0; j < nMLCount; ++j) {

                                        int nDL = 0, nDc = 0, nSnow = 0, nDw = 0, nPL = 0, nEP = 0, nEV = 0, nES = 0, nEH = 0;
                                        int nLL = 0, nIL = 0, nLS = 0, nLP = 0, nILP = 0, nCF = 0, nBRK = 0, nBK = 0, nCRL = 0;
                                        int nB = 0, nWL = 0, nWLL = 0, nTL = 0, nSL = 0, nER = 0, nLF = 0;

                                        LcomD.Initialize();
                                        LcomD.nActive = 1;
                                        LoadCaseNo = 0;

                                        //Permanent
                                        for (ld = 0; ld < m_nDCount; ++ld) nDL += Set_STLCIncludeCS(arDLoad[ld], dDFactor, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
                                        for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);

                                        for (ld = 0; ld < m_nLCount; ++ld) nLL += Set_STLCIncludeCS(arLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        for (ld = 0; ld < m_nIlCount; ++ld) nIL += Set_STLCIncludeCS(arIlLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        for (ld = 0; ld < m_nCfCount; ++ld) nCF += Set_STLCIncludeCS(arCfLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        for (ld = 0; ld < m_nLFCount; ++ld) nLF += Set_STLCIncludeCS(arLFLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
                                        if (m_nMCount > 0)  Set_MovingCase(arMLoad.GetAt(j), 0.5, LoadCaseNo, LcomD, TRUE);

                                        if (m_bIRSCons_crp_shr_DT_ULS)
                                        {
                                            //for (ld = 0; ld < m_nShCount; ++ld)  nSH += Set_STLCIncludeCS(arShLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
                                            //for (ld = 0; ld < m_nCrCount; ++ld)  nCR += Set_STLCIncludeCS(arCrLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
                                        }
                                        //if (m_nConstLoad != 0) // ST+CS
                                        //{
                                        //    if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
                                        //        Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor, LoadCaseNo, LcomD, TRUE);

                                        //    CString strcEL;
                                        //    int nErection = 0;
                                        //    for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
                                        //    {
                                        //        strcEL = _T(""); nErection = 0;
                                        //        if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }



                                        //        if (CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")) || CheckCSEL(nErection, _T("PL")) || CheckCSEL(nErection, _T("BL")))
                                        //            Set_CStageCase(arCSLoad.GetAt(nnn), dDwFactor, LoadCaseNo, LcomD, TRUE);
                                        //        else if (CheckCSEL(nErection, _T("D")))
                                        //            Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor, LoadCaseNo, LcomD, TRUE);

                                        //    }
                                        //    if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
                                        //        Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
                                        //    if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
                                        //        Set_CStageCase(arCSLoad.GetAt(nnn), dPsFactor, LoadCaseNo, LcomD, TRUE);
                                        //    if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep Primary
                                        //    {
                                        //        Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                        //    }
                                        //    if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep secondary
                                        //    {
                                        //        Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                        //    }
                                        //    if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage Primary
                                        //    {
                                        //        Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                        //    }
                                        //    if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage secondary
                                        //    {
                                        //        Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
                                        //    }

                                        //}

                                        double dRS_X = (m % 2 == 0) ? dRsFactor : -1. * dRsFactor;
                                        double dRS_Y = (n % 2 == 0) ? dRsFactor : -1. * dRsFactor;
                                        double dRS_Z = (o % 2 == 0) ? dRsFactor : -1. * dRsFactor;
                                        if (m % 2 == 0 || m % 2 == 1)
                                        {
                                            dRS_Y = 0.3 * dRS_Y;
                                            dRS_Z = 0.3 * dRS_Z;
                                        }
                                        else if (n % 2 == 0)
                                        {
                                            dRS_X = 0.3 * dRS_X;
                                            dRS_Z = 0.3 * dRS_Z;
                                        }
                                        else if (o % 2 == 0)
                                        {
                                            dRS_X = 0.3 * dRS_X;
                                            dRS_Y = 0.3 * dRS_Y;

                                        }
                                        else if (m % 2 == 1 && n % 2 == 1 && o % 2 == 1)
                                        {
                                            dRS_Y = 0.3 * dRS_Y;
                                            dRS_Z = 0.3 * dRS_Z;
                                        }

                                        for (ld = 0; ld < m_nECount; ++ld)
                                        {
                                            if (m_nECount == 1)
                                            {
                                                //nES += Set_STLCIncludeCS(arELoad[ld], dEFactor_x, LoadCaseNo, LcomD, 0);
                                            }
                                            else
                                            {
                                                /* if (ld == 0)            nES += Set_STLCIncludeCS(arELoad[ld], dRS_Z, LoadCaseNo, LcomD, 0);
                                                 else if (ld % 2 == 1)     nES += Set_STLCIncludeCS(arELoad[ld], dRS_Y, LoadCaseNo, LcomD, 0);
                                                 else if (ld % 2 == 0)     nES += Set_STLCIncludeCS(arELoad[ld], dRS_X, LoadCaseNo, LcomD, 0);
     */
                                            }

                                        }

                                        if (arEspLoad_X.GetSize() > 0) Set_SpectrumCase(arEspLoad_X[o / 2], dRS_Z, LoadCaseNo, LcomD, TRUE);
                                        if (arEspLoad_Y.GetSize() > 0) Set_SpectrumCase(arEspLoad_Y[n / 2], dRS_Y, LoadCaseNo, LcomD, TRUE);
                                        if (arEspLoad_Z.GetSize() > 0) Set_SpectrumCase(arEspLoad_Z[m / 2], dRS_X, LoadCaseNo, LcomD, TRUE);


                                        // description
                                        LcomD.Description.Empty();
                                        csPrefix.Empty();

                                        LcomD.Description += _T("6U:");
                                        if (nDL > 0)
                                        {
                                            LcomD.Description += csPrefix + _T("D");
                                            csPrefix = _T(":");
                                        }
                                        if (nDc > 0 || nDw > 0 || nPL > 0 || nB > 0)
                                        {
                                            LcomD.Description += csPrefix + _T("SIDL");
                                            csPrefix = _T(":");
                                        }
                                        if (arEspLoad_X.GetSize() > 0)
                                        {
                                            LcomD.Description += csPrefix + GetFactorString(dRS_Z) + _T("RS(X)");
                                            csPrefix = _T(":");
                                        }
                                        if (arEspLoad_Y.GetSize() > 0)
                                        {
                                            LcomD.Description += csPrefix + GetFactorString(dRS_Y) + _T("RS(Y)");
                                            csPrefix = _T(":");
                                        }
                                        if (arEspLoad_Z.GetSize() > 0)
                                        {
                                            LcomD.Description += csPrefix + GetFactorString(dRS_X) + _T("RS(Z)");
                                            csPrefix = _T(":");
                                        }
                                        if (m_nMCount > 0)
                                        {
                                            LcomD.Description += csPrefix + _T("ML"); csPrefix = _T(":");
                                        }
                                        // 
                                        //if (nPS > 0) { LcomD.Description += csPrefix + GetFactorString(dPsFactor) + _T("PS"); csPrefix = _T("+"); }
                                        if (m_nConstLoad != 0) // ST+CS
                                        {
                                            if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("(cD)"); csPrefix = _T("+");
                                            }

                                            CString strcEL;
                                            int nErection = 0;

                                            for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
                                            {
                                                strcEL = _T(""); nErection = 0;
                                                if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }



                                                if (CheckCSEL(nErection, _T("D")) || CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")))
                                                {
                                                    LcomD.Description += csPrefix + GetFactorString(dDFactor) + strcEL; csPrefix = _T("+");
                                                }

                                                else if (CheckCSEL(nErection, _T("W")))
                                                {
                                                    LcomD.Description += csPrefix + GetFactorString(dWFactor) + strcEL; csPrefix = _T("+");
                                                }
                                            }

                                            if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCSFactor) + _T("(cTP)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dPsFactor) + _T("(cTS)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCP)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCS)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSP)"); csPrefix = _T("+");
                                            }
                                            if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn))
                                            {
                                                LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSS)"); csPrefix = _T("+");
                                            }
                                        }

                                        if (LoadCaseNo > 0) Set_LoadComb4Civil(LcomD, D_STR);
                                    }
                                }
                            }
                        }
                    }
                }

            }
        }

    }


    return TRUE;
}




BOOL CLoadCombCtrl::SetCvlIRSSBC_DRL(ArINT &arDLoad, ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arBLLoad, ArINT &arDrlLoad, ArUNT &arMLoad)
{
    T_LCOM_D LcomD;

    int nDCount = 1;
    BOOL bExistD = (m_nDCount > 0 || m_nDwCount > 0 || m_nDcCount > 0);
    if (bExistD) nDCount = 1;

    int nMLCount = max(m_nMCount, 1);
    double dDFactor = 1.0;

    CString csPrefix(_T("")), strFixM(_T("")), strFixW(_T("")), strFixSm(_T(""));

    int LoadCaseNo = 0, ld = 0, nnn = 0;
    for (int d = 0; d < nDCount; ++d)
    {
        for (int j = 0; j < nMLCount; ++j) {
            int nDL = 0, nDc = 0, nSnow = 0, nDw = 0, nPL = 0, nEP = 0, nEV = 0, nES = 0, nEH = 0;
            int nLL = 0, nIL = 0, nLS = 0, nLP = 0, nILP = 0, nCF = 0, nBRK = 0, nBK = 0, nCRL = 0;
            int nB = 0, nWL = 0, nWLL = 0, nTL = 0, nSL = 0, nER = 0, nDRL = 0;

            LcomD.Initialize();
            LcomD.nActive = 1;
            LoadCaseNo = 0;

            //Permanent
            for (ld = 0; ld < m_nDCount; ++ld) nDL += Set_STLCIncludeCS(arDLoad[ld], dDFactor, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
            for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
            for (ld = 0; ld < m_nDrlCount; ++ld) nDRL += Set_STLCIncludeCS(arDrlLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
            if (m_nMCount > 0)  Set_MovingCase(arMLoad.GetAt(j), 1.0, LoadCaseNo, LcomD, TRUE);

            //for (ld = 0; ld < m_nDcCount; ++ld) nDc += Set_STLCIncludeCS(arDcLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
            //for (ld = 0; ld < m_nDwCount; ++ld) nDw += Set_STLCIncludeCS(arDwLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
            // for(ld=0; ld<m_nLsCount; ++ld) nLS += Set_STLCIncludeCS(arLsLoad[ld], dLsFactor, LoadCaseNo, LcomD, 0);
                   //if(m_nSmCount > 0) Set_SettleCase(arSmLoad[nSmIndex], dSmFactor, LoadCaseNo, LcomD, TRUE);

            // description
            LcomD.Description.Empty();
            csPrefix.Empty();
            LcomD.Description += _T("7U:");
            if (nDL > 0)
            {
                LcomD.Description += csPrefix + _T("D");
                csPrefix = _T(":");
            }
            if (nDc > 0)
            {
                LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("DC");
                csPrefix = _T("+");
            }
            if (nDw > 0)
            {
                LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("DW");
                csPrefix = _T("+");
            }
            if (nB > 0)
            {
                LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("BL");
                csPrefix = _T("+");
            }
            if (nDRL > 0)
            {
                LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("DRL");
                csPrefix = _T("+");
            }

            if (m_nMCount > 0)
            {
                LcomD.Description += csPrefix + _T("ML"); csPrefix = _T(":");
            }

            if (LoadCaseNo > 0) Set_LoadComb4Civil(LcomD, D_STR);

        }
    }




    return TRUE;
}


////////////////////////////////////////////////////////////////////////
////// TMH 07 Pinakin 으로 이름 수정          /////////
//////////////////////////////////////////////////////////////////////////

void CLoadCombCtrl::GetBD_21FactorTMH(
	CString& sD1Fac, CString& sD2Fac, CString& sDw1Fac, CString& sDw2Fac, CString& sDc1Fac, CString& sDc2Fac,
	CString& sEv1Fac, CString& sEv2Fac, CString& sEh1Fac, CString& sEh2Fac, CString& sEs1Fac,CString& sEs2Fac, CString& sCrl1Fac, CString& sCrl2Fac, CString& sGf3ULSFac, CString& sGf3SLSFac,
	double& dD1, double& dD2, double& dDw1, double& dDw2, double& dDc1, double& dDc2,
	double& dEv1, double& dEv2, double& dEh1, double& dEh2, double& dEs1, double& dEs2,double& dB1,double& dB2, double& dCrl1, double& dCrl2, double& dGf3ULS, double& dGf3SLS, BOOL b1stGrp, int& m_nCodeBasedVerticalEarthPressure,int& m_nCodeBasedNonVerticalEarthPressure,int& m_nCodeBasedOrUserDef,double& m_dUserDefDL_Max,double& m_dUserDefDL_Min,double& m_dUserDefSIDL_Max, double& m_dUserDefSIDL_Min,int& m_dUserRdo_Dmax,int& m_dUserRdo_Dmin,int& m_dUserRdo_Dboth, int& m_dUserRdo_SIDLmax, int& m_dUserRdo_SIDLmin, int& m_dUserRdo_SIDLboth, const EN_TMH07_NO& enLcomNo)
{
	// b1stGrp = TRUE  : Group 1 일때 
	// b1stGrp = FALSE : Group 1 Fatigue or Group 2 일때 
	if (b1stGrp)
	{
        if (m_nCodeBasedOrUserDef == 1) {
            if (m_dUserRdo_Dmax == 1) {
                dD1 = m_dUserDefDL_Max;	 sD1Fac.Format(_T("%.2f"), m_dUserDefDL_Max);
                dD2 = m_dUserDefDL_Max;	 sD2Fac.Format(_T("%.2f"), m_dUserDefDL_Max);
            }
            else if (m_dUserRdo_Dmin == 1) {
                dD1 = m_dUserDefDL_Min;	 sD1Fac.Format(_T("%.2f"), m_dUserDefDL_Min);
                dD2 = m_dUserDefDL_Min;	 sD2Fac.Format(_T("%.2f"), m_dUserDefDL_Min);
            }
            else if (m_dUserRdo_Dboth == 1) {
                dD1 = m_dUserDefDL_Max;	 sD1Fac.Format(_T("%.2f"), m_dUserDefDL_Max);
                dD2 = m_dUserDefDL_Min;	 sD2Fac.Format(_T("%.2f"), m_dUserDefDL_Min);
            }
        }
        else
        {
            if (enLcomNo == EN_TMH07_NO_02_01) {
                dD1 = m_dBD21D[1];	 sD1Fac.Format(_T("%.2f"), m_dBD21D[1]);
                dD2 = m_dBD21D[1];	 sD2Fac.Format(_T("%.2f"), m_dBD21D[1]);
            }
            else {
                dD1 = m_dBD21D[0];	 sD1Fac.Format(_T("%.2f"), m_dBD21D[0]);
                dD2 = m_dBD21D[0];	 sD2Fac.Format(_T("%.2f"), m_dBD21D[0]);
            }
        }
        if (m_nCodeBasedOrUserDef == 1) {
          
            if (m_dUserRdo_SIDLmax == 1) {
                dDw1 = m_dUserDefSIDL_Max; sDw1Fac.Format(_T("%.2f"), m_dUserDefSIDL_Max);
                dDw2 = m_dUserDefSIDL_Max; sDw2Fac.Format(_T("%.2f"), m_dUserDefSIDL_Max);
            }
            else if (m_dUserRdo_SIDLmin == 1) {
                dDw1 = m_dUserDefSIDL_Min; sDw1Fac.Format(_T("%.2f"), m_dUserDefSIDL_Min);
                dDw2 = m_dUserDefSIDL_Min; sDw2Fac.Format(_T("%.2f"), m_dUserDefSIDL_Min);
            }
            else if (m_dUserRdo_SIDLboth == 1) {
                dDw1 = m_dUserDefSIDL_Max; sDw1Fac.Format(_T("%.2f"), m_dUserDefSIDL_Max);
                dDw2 = m_dUserDefSIDL_Min; sDw2Fac.Format(_T("%.2f"), m_dUserDefSIDL_Min);
            }
        }
        else {
			
            if (enLcomNo == EN_TMH07_NO_02_01) {
                dDw1 = m_dBD21DW[1]; sDw1Fac.Format(_T("%.2f"), m_dBD21DW[1]);
                dDw2 = m_dBD21DW[1]; sDw2Fac.Format(_T("%.2f"), m_dBD21DW[1]);
            }
            else {
                dDw1 = m_dBD21DW[0]; sDw1Fac.Format(_T("%.2f"), m_dBD21DW[0]);
                dDw2 = m_dBD21DW[0]; sDw2Fac.Format(_T("%.2f"), m_dBD21DW[0]);
            }
        }
        if (m_nCodeBasedOrUserDef == 1) {
            if (m_dUserRdo_SIDLmax == 1) {
                dDc1 = m_dUserDefSIDL_Max; sDc1Fac.Format(_T("%.2f"), m_dUserDefSIDL_Max);
                dDc2 = m_dUserDefSIDL_Max; sDc2Fac.Format(_T("%.2f"), m_dUserDefSIDL_Max);
            }
            else if (m_dUserRdo_SIDLmin == 1) {
                dDc1 = m_dUserDefSIDL_Min; sDc1Fac.Format(_T("%.2f"), m_dUserDefSIDL_Min);
                dDc2 = m_dUserDefSIDL_Min; sDc2Fac.Format(_T("%.2f"), m_dUserDefSIDL_Min);
            }
            else if (m_dUserRdo_SIDLboth == 1) {
                dDc1 = m_dUserDefSIDL_Max; sDc1Fac.Format(_T("%.2f"), m_dUserDefSIDL_Max);
                dDc2 = m_dUserDefSIDL_Min; sDc2Fac.Format(_T("%.2f"), m_dUserDefSIDL_Min);
            }
        }
        else {
            if (enLcomNo == EN_TMH07_NO_02_01) {
                dDc1 = m_dBD21DC[1]; sDc1Fac.Format(_T("%.2f"), m_dBD21DC[1]);
                dDc2 = m_dBD21DC[1]; sDc2Fac.Format(_T("%.2f"), m_dBD21DC[1]);
            }
            else {
                dDc1 = m_dBD21DC[0]; sDc1Fac.Format(_T("%.2f"), m_dBD21DC[0]);
                dDc2 = m_dBD21DC[0]; sDc2Fac.Format(_T("%.2f"), m_dBD21DC[0]);
            }
        }
        if (m_nCodeBasedNonVerticalEarthPressure == 0) {
            dEv1 = 1.5; sEv1Fac.Format(_T("%.2f"), 1.5);
            dEv2 = 1.3; sEv2Fac.Format(_T("%.2f"), 1.3);
        }
        else {
			dEv1 = 1.4; sEv1Fac.Format(_T("%.2f"), 1.4);
			dEv2 = 1.2; sEv2Fac.Format(_T("%.2f"), 1.2);
        }
		dEh1 = m_dBD21EH[0]; sEh1Fac.Format(_T("%.2f"), m_dBD21EH[0]);
		dEh2 = m_dBD21EH[1]; sEh2Fac.Format(_T("%.2f"), m_dBD21EH[1]);
        if (m_nCodeBasedVerticalEarthPressure == 0) {
            dEs1 = 1.5; sEs1Fac.Format(_T("%.2f"), m_dBD21ES[0]);
            dEs2 = 1.3; sEs2Fac.Format(_T("%.2f"), m_dBD21ES[1]);
        } else {
			dEs1 = 1.4; sEs1Fac.Format(_T("%.2f"), m_dBD21ES[0]);
			dEs2 = 1.2; sEs2Fac.Format(_T("%.2f"), m_dBD21ES[1]);
        }
		dB1 = 1.2; sEs1Fac.Format(_T("%.2f"), 1.2);
		dB2 = 1.05; sEs2Fac.Format(_T("%.2f"), 1.05);
		dCrl1 = 1.5; sCrl1Fac.Format(_T("%.2f"), 1.5);
		dCrl2 = 1.3; sCrl2Fac.Format(_T("%.2f"), 1.3);
        

		dGf3ULS = m_dBD21GF3ULS; sGf3ULSFac.Format(_T("%.2f"), m_dBD21GF3ULS);
		dGf3SLS = m_dBD21GF3SLS; sGf3ULSFac.Format(_T("%.2f"), m_dBD21GF3SLS);
	}
	else
	{
		dD1 = 1.0;	 sD1Fac.Format(_T("%.2f"), 1.0);
		dD2 = 1.0;	 sD2Fac.Format(_T("%.2f"), 1.0);
		dDw1 = 1.0; sDw1Fac.Format(_T("%.2f"), 1.0);
		dDw2 = 1.0; sDw2Fac.Format(_T("%.2f"), 1.0);
		dDc1 = 1.0; sDc1Fac.Format(_T("%.2f"), 1.0);
		dDc2 = 1.0; sDc2Fac.Format(_T("%.2f"), 1.0);
        if (m_nCodeBasedNonVerticalEarthPressure == 0) {
            dEv1 = 1.1; sEv1Fac.Format(_T("%.2f"), 1.1);
            dEv2 = 1.1; sEv2Fac.Format(_T("%.2f"), 1.1);
        }
        else {
			dEv1 = 1.0; sEv1Fac.Format(_T("%.2f"), 1.0);
			dEv2 = 1.0; sEv2Fac.Format(_T("%.2f"), 1.0);
        }
		dEh1 = 1.0; sEh1Fac.Format(_T("%.2f"), 1.0);
		dEh2 = 1.0; sEh2Fac.Format(_T("%.2f"), 1.0);
		if (m_nCodeBasedVerticalEarthPressure == 0) {
			dEs1 = 1.1; sEs1Fac.Format(_T("%.2f"), 1.0);
			dEs2 = 1.1; sEs2Fac.Format(_T("%.2f"), 1.0);
		}
		else {
			dEs1 = 1.0; sEs1Fac.Format(_T("%.2f"), 1.0);
			dEs2 = 1.0; sEs2Fac.Format(_T("%.2f"), 1.0);
		}
		dB1 = 1.0; sEs1Fac.Format(_T("%.2f"), 1.0);
		dB2 = 1.0; sEs2Fac.Format(_T("%.2f"), 1.0);
		dCrl1 = 1.0; sCrl1Fac.Format(_T("%.2f"), 1.0);
		dCrl2 = 1.0; sCrl2Fac.Format(_T("%.2f"), 1.0);

		dGf3ULS = m_dBD21GF3ULS; sGf3ULSFac.Format(_T("%.2f"), m_dBD21GF3ULS);
		dGf3SLS = m_dBD21GF3SLS; sGf3ULSFac.Format(_T("%.2f"), m_dBD21GF3SLS);
	}

}

void CLoadCombCtrl::GetBD_21FactoredPermTMH(
	double& dD1, double& dD2, double& dDw1, double& dDw2, double& dDc1, double& dDc2,
	double& dEv1, double& dEv2, double& dEh1, double& dEh2, double& dGf3)
{
	dD1 = dGf3 * dD1;
	dD2 = dGf3 * dD2;
	dDw1 = dGf3 * dDw1;
	dDw2 = dGf3 * dDw2;
	dDc1 = dGf3 * dDc1;
	dDc2 = dGf3 * dDc2;
	dEv1 = dGf3 * dEv1;
	dEv2 = dGf3 * dEv2;
	dEh1 = dGf3 * dEh1;
	dEh2 = dGf3 * dEh2;
}

BOOL CLoadCombCtrl::Check_TMH07_LoadCase(const EN_TMH07_NO& enLcomNo)
{

	if (m_nDCount == 0 && m_nDwCount == 0 && m_nDcCount == 0 && m_nStlCount == 0 && m_nSmCount == 0 && m_nEvCount == 0 && m_nEhCount == 0 &&
		m_nMCount == 0 && m_nTCount == 0 && m_nTpgCount == 0 && m_nWCount == 0 && m_nFrCount == 0) return FALSE;

	// Load check
	switch (enLcomNo)
	{
	case EN_TMH07_NO_01:   // Combination 1 - Basic
		if (m_nMCount == 0) return FALSE;
		break;
	case EN_TMH07_NO_02_01:   // Combination 2 - Wind only Dead Superimposed Dead 
	case EN_TMH07_NO_02_02:   // Combination 2 - Wind Not only Dead Superimposed Dead 
	case EN_TMH07_NO_02_03:   // Combination 2 - Wind Relieving Effect
		/*if (m_nWCount == 0) return FALSE;
		break;*/
	case EN_TMH07_NO_03:   // Combination 3 - Temperature
		if (m_nTCount == 0 && m_nTpgCount == 0) return FALSE;
		break;
	case EN_TMH07_NO_04_01:   // Combination 4 - Collision
		if (m_nCoCount == 0) return FALSE;
		break;
	case EN_TMH07_NO_04_02:   // Combination 4 - Centrifugal
		if (m_nCfCount == 0) return FALSE;
		if (m_nBD21BridgeType != 0) return FALSE;
		break;
	case EN_TMH07_NO_04_03:   // Combination 4 - Braking
		if (m_nBrkCount == 0) return FALSE;
		if (m_nBD21BridgeType != 0) return FALSE;
		break;
	case EN_TMH07_NO_05:   // Combination 5 - Friction
		if (m_nFrCount == 0) return FALSE;
		break;
    case EN_TMH07_NO_EQ_ST:
        if (m_nECount == 0 && m_nEspCount == 0) return TRUE;
        if (m_nECount == 0) return FALSE;
        break;
	case EN_TMH07_NO_EQ_RS:
		if (m_nEspCount == 0) return FALSE;
        break;
	default:
		ASSERT(0);
		return FALSE;
		break;
	}
	return TRUE;
}

int CLoadCombCtrl::Get_TMH07_LCOM_List(CArray<TMH07_LCOM, TMH07_LCOM&>& raLcomList)
{
	raLcomList.RemoveAll();
	//Strength, Moving, Wind
	TMH07_LCOM TMH07D;
	//ULS
	TMH07D.Set(EN_TMH07_NO_01, TRUE, TRUE, FALSE, FALSE);   raLcomList.Add(TMH07D);        // Combination 1 - Basic // Type 1
	TMH07D.Set(EN_TMH07_NO_02_01, TRUE, TRUE, TRUE, FALSE);   raLcomList.Add(TMH07D);    // Combination 2 - Wind only Dead Superimposed Dead // Type 2
	//TMH07D.Set(EN_TMH07_NO_02_02, TRUE, TRUE, TRUE, FALSE);    raLcomList.Add(TMH07D);     // Combination 2 - Wind Not only Dead Superimposed Dead  
	//TMH07D.Set(EN_TMH07_NO_02_03, TRUE, TRUE, TRUE, FALSE);    raLcomList.Add(TMH07D);     // Combination 2 - Wind Relieving Effect
	/*TMH07D.Set(EN_TMH07_NO_03, TRUE, TRUE, FALSE, TRUE);  raLcomList.Add(TMH07D);*/        // Combination 3 - Temperature //Type 3
	//TMH07D.Set(EN_TMH07_NO_04_01, TRUE, FALSE, FALSE, FALSE);   raLcomList.Add(TMH07D);    // Combination 4 - Collision
	//TMH07D.Set(EN_TMH07_NO_04_02, TRUE, FALSE, FALSE, FALSE);   raLcomList.Add(TMH07D);    // Combination 4 - Centrifugal
	//TMH07D.Set(EN_TMH07_NO_04_03, TRUE, FALSE, FALSE, FALSE);   raLcomList.Add(TMH07D);    // Combination 4 - Braking
	//TMH07D.Set(EN_TMH07_NO_05, TRUE, FALSE, FALSE, FALSE);   raLcomList.Add(TMH07D);       // Combination 5 - Friction
	TMH07D.Set(EN_TMH07_NO_EQ_ST, TRUE, FALSE, TRUE, TRUE); raLcomList.Add(TMH07D);       //Combination 6 - Seismic static //Type 3
	TMH07D.Set(EN_TMH07_NO_EQ_RS, TRUE, FALSE, TRUE, TRUE); raLcomList.Add(TMH07D);       //Combination 7 - Seismic RS //Type 3
	//SLS                                                                                  
	TMH07D.Set(EN_TMH07_NO_01, FALSE, TRUE, FALSE, FALSE);   raLcomList.Add(TMH07D);       // Combination 1 - Basic // Type 1
	TMH07D.Set(EN_TMH07_NO_02_01, FALSE, TRUE, TRUE, FALSE);    raLcomList.Add(TMH07D);   // Combination 2 - Wind only Dead Superimposed Dead  // Type 2
	//TMH07D.Set(EN_TMH07_NO_02_02, FALSE, TRUE, TRUE, FALSE);    raLcomList.Add(TMH07D);    // Combination 2 - Wind Not only Dead Superimposed Dead
	//TMH07D.Set(EN_TMH07_NO_02_03, FALSE, TRUE, TRUE, FALSE);    raLcomList.Add(TMH07D);    // Combination 2 - Wind Relieving Effect 
	/*TMH07D.Set(EN_TMH07_NO_03, FALSE, TRUE, FALSE, TRUE);   raLcomList.Add(TMH07D);  */     // Combination 3 - Temperature  //Type 3
	//TMH07D.Set(EN_TMH07_NO_04_01, FALSE, FALSE, FALSE, FALSE);   raLcomList.Add(TMH07D);   // Combination 4 - Collision
	//TMH07D.Set(EN_TMH07_NO_04_02, FALSE, FALSE, FALSE, FALSE);   raLcomList.Add(TMH07D);   // Combination 4 - Centrifugal
	//TMH07D.Set(EN_TMH07_NO_04_03, FALSE, FALSE, FALSE, FALSE);   raLcomList.Add(TMH07D);   // Combination 4 - Braking
	//TMH07D.Set(EN_TMH07_NO_05, FALSE, FALSE, FALSE, FALSE);   raLcomList.Add(TMH07D);      // Combination 5 - Friction
	TMH07D.Set(EN_TMH07_NO_EQ_ST, FALSE, FALSE, TRUE, TRUE); raLcomList.Add(TMH07D);       //Combination 6 - Seismic static //Type 3
	TMH07D.Set(EN_TMH07_NO_EQ_RS, FALSE, FALSE, TRUE, TRUE); raLcomList.Add(TMH07D);  // Type 3
	return raLcomList.GetSize();
}

CString CLoadCombCtrl::Add_CvlTMH07_Title(const EN_TMH07_NO& enLcomNo, BOOL bStrength) // 하중조합 생성시 Title 더하기
{

	CString csComb(_T(""));
	CString csStrength(_T(""));
	CString csTitle(_T(""));

	if (bStrength)
	{
		csStrength = _T("U");
	}
    else
	{
		csStrength = _T("S");
	}

	switch (enLcomNo)
	{
	case EN_TMH07_NO_01:
		csComb = _T("1: ");
		break;
	case EN_TMH07_NO_02_01:
	case EN_TMH07_NO_02_02:
	case EN_TMH07_NO_02_03:
		csComb = _T("2: ");
		break;
	case EN_TMH07_NO_03:
		csComb = _T("3: ");
		break;
	case EN_TMH07_NO_04_01:
	case EN_TMH07_NO_04_02:
	case EN_TMH07_NO_04_03:
		csComb = _T("Comb 4: ");
		break;
	case EN_TMH07_NO_05:
		csComb = _T("Comb 5: ");
		break;
	case EN_TMH07_NO_EQ_ST:
		csComb = _T("3: ");
		break;
	case EN_TMH07_NO_EQ_RS:
		csComb = _T("3: ");
		break;
	default:
		ASSERT(0);
		break;
	}

	csTitle = csStrength + csComb;

	return csTitle;
}


void CLoadCombCtrl::Set_CvlTMH07(const EN_TMH07_NO& enLcomNo,
	ArINT& arDLoad, ArINT& arDwLoad, ArINT& arDcLoad, ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arEvLoad, ArINT& arEhLoad,
	ArINT& arCrLoad, ArINT& arShLoad, ArINT& arPsLoad, ArUNT& arMLoad, ArINT& arCfLoad, ArINT& arBrkLoad, ArINT& arCoLoad,
	ArINT& arErLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arWLoad, ArINT& arFrLoad, ArINT& arCSLoad, BOOL bStrength, BOOL bMoving, BOOL bWind)
{
	if (!Check_TMH07_LoadCase(enLcomNo)) return;

	// Make new load combinations.
	CArray<T_LCOM_D, T_LCOM_D&> aNewLcom;  
	aNewLcom.RemoveAll();

	Add_CvlTMH07_Title(enLcomNo, bStrength);

	CString sDFac[2], sDwFac[2], sDcFac[2], sEvFac[2], sEhFac[2], sGf3ULSFac, sGf3SLSFac;
	double  dDFac[2], dDwFac[2], dDcFac[2], dEvFac[2], dEhFac[2], dGf3ULS, dGf3SLS;
	double  dStlFactor, dMFactor, dBrCfFactor, dCRSHFactor, dPsFactor, dWFactor, dTFactor, dTpgFactor, dCoFactor, dFrFactor, dErFactor;

	GetBD_21Factor(sDFac[0], sDFac[1], sDwFac[0], sDwFac[1], sDcFac[0], sDcFac[1], sEvFac[0], sEvFac[1], sEhFac[0], sEhFac[1], sGf3ULSFac, sGf3SLSFac,
		dDFac[0], dDFac[1], dDwFac[0], dDwFac[1], dDcFac[0], dDcFac[1], dEvFac[0], dEvFac[1], dEhFac[0], dEhFac[1], dGf3ULS, dGf3SLS, bStrength);

	if (bStrength)
	{
		GetBD_21FactoredPerm(dDFac[0], dDFac[1], dDwFac[0], dDwFac[1], dDcFac[0], dDcFac[1], dEvFac[0], dEvFac[1], dEhFac[0], dEhFac[1], dGf3ULS);
		dStlFactor = dGf3ULS * 1.2;
		dMFactor = 1.0;
		dBrCfFactor = dGf3ULS * 1.5;
		dCRSHFactor = dGf3ULS * 1.2;
		dPsFactor = dGf3ULS * 1.0;
		dTFactor = dGf3ULS * 1.3;
		dTpgFactor = dGf3ULS * 1.0;
		dCoFactor = dGf3ULS * 1.5;
		dFrFactor = dGf3ULS * 1.3;
		dErFactor = dGf3ULS * 1.15;

		if (enLcomNo == EN_TMH07_NO_02_01)
		{
			dWFactor = dGf3ULS * 1.1;
		}
		else if (enLcomNo == EN_TMH07_NO_EQ_ST || enLcomNo == EN_TMH07_NO_EQ_RS)
		{
			dWFactor = dGf3ULS * 1.4;
		}
		else
		{
			dWFactor = dGf3ULS * 1.0;
		}
	}
	else
	{
		GetBD_21FactoredPerm(dDFac[0], dDFac[1], dDwFac[0], dDwFac[1], dDcFac[0], dDcFac[1], dEvFac[0], dEvFac[1], dEhFac[0], dEhFac[1], dGf3SLS);
		dStlFactor = dGf3SLS * 1.0;
		dMFactor = 1.0;
		dBrCfFactor = dGf3SLS * 1.0;
		dCRSHFactor = dGf3SLS * 1.0;
		dPsFactor = dGf3SLS * 1.0;
		dWFactor = dGf3SLS * 1.0;
		dTFactor = dGf3SLS * 1.0;
		dTpgFactor = dGf3SLS * 0.8;
		dCoFactor = dGf3SLS * 1.0;
		dFrFactor = dGf3SLS * 1.0;
		dErFactor = dGf3SLS * 1.0;

	}

	Add_CvlTMH07_Title(enLcomNo, bStrength, aNewLcom);
	if (m_nConstLoad != 1)
	{
		Add_CvlTMH07_D(enLcomNo, bStrength, arDLoad, dDFac[0], dDFac[1], aNewLcom);
		Add_CvlTMH07_DW(enLcomNo, bStrength, arDwLoad, dDwFac[0], dDwFac[1], aNewLcom);
		Add_CvlTMH07_DC(enLcomNo, bStrength, arDcLoad, dDcFac[0], dDcFac[1], aNewLcom);
	}
    if (bMoving) {
       /* Add_CvlTMH07_M(enLcomNo, bStrength, arMLoad, dDFac[0], dDFac[1], dDFac[1], aNewLcom);*/
    }
	/*Add_CvlTMH07_EV(enLcomNo, bStrength, arEvLoad, dEvFac[0], dEvFac[1], aNewLcom,m_dEPRelieving);*/
	Add_CvlTMH07_EH(enLcomNo, bStrength, arEhLoad, dEhFac[0], dEhFac[1], aNewLcom);
	Add_CvlTMH07_STL_SM(enLcomNo, bStrength, arStlLoad, arSmLoad, dStlFactor, aNewLcom);

	if (bMoving)
	{
		/*Add_CvlTMH07_M(enLcomNo, bStrength, arMLoad, dMFactor, dGf3ULS, dGf3SLS, aNewLcom);*/
	}

	if (bWind)
	{
		/*Add_CvlTMH07_W(enLcomNo, bStrength, arWLoad, dWFactor, aNewLcom);*/
	}

	Add_CvlTMH07_T_TPG(enLcomNo, bStrength, arTLoad, arTpgLoad, dTFactor, dTpgFactor, aNewLcom);
	Add_CvlTMH07_CO(enLcomNo, bStrength, arCoLoad, dCoFactor, aNewLcom);
	/*Add_CvlTMH07_CF_BRK(enLcomNo, bStrength, arMLoad, arCfLoad, arBrkLoad, dBrCfFactor, aNewLcom);*/
	Add_CvlTMH07_ER(enLcomNo, bStrength, arErLoad, dFrFactor, aNewLcom);
	/*Add_CvlTMH07_PS_CR_SH(enLcomNo, bStrength, arPsLoad, arCrLoad, arShLoad, dPsFactor, dCRSHFactor, aNewLcom, m_dEPRelieving);*/

	Add_CvlTMH07_CS_D(enLcomNo, bStrength, arCSLoad, dDFac[0], dDFac[1], aNewLcom);
	Add_CvlTMH07_CS_DW(enLcomNo, bStrength, arCSLoad, dDwFac[0], dDwFac[1], aNewLcom);
	Add_CvlTMH07_CS_DC(enLcomNo, bStrength, arCSLoad, dDcFac[0], dDcFac[1], aNewLcom);

	if (bWind)
	{
		/*Add_CvlTMH07_CS_W(enLcomNo, bStrength, arCSLoad, dErFactor, aNewLcom);*/
	}

	/*Add_CvlTMH07_CS_T(enLcomNo, bStrength, arCSLoad, dTFactor, dTpgFactor, dErFactor, aNewLcom);*/
	Add_CvlTMH07_CS_PSCRSH(enLcomNo, bStrength, arCSLoad, dPsFactor, dCRSHFactor, aNewLcom);

	// Add new load combinations.
	int nNewLcom = aNewLcom.GetSize();
	for (int i = 0; i < nNewLcom; ++i)
	{
		if (aNewLcom[i].aCombination.GetSize() == 0) continue;

		if (m_nLcomType == D_LCOMTYPE_CONCRETE)
		{
			if (bStrength)
			{
				aNewLcom[i].nActive = 1;
			}
			else
			{
				aNewLcom[i].nActive = 2;
			}
		}
		else if (m_nLcomType == D_LCOMTYPE_GENERAL)
		{
			aNewLcom[i].nActive = 1;
		}

		if (!ExistInNewLcom(aNewLcom[i]))
		{
			if (bStrength)
			{
				Set_LoadComb4CivilName_proxy(aNewLcom[i], D_STR);
			}
			else
			{
				switch (enLcomNo)
				{
				case EN_TMH07_NO_01:
					aNewLcom[i].nServLcomType = 1;
					break;
				case EN_TMH07_NO_02_01:
				case EN_TMH07_NO_02_02:
				case EN_TMH07_NO_02_03:
				case EN_TMH07_NO_03:
				case EN_TMH07_NO_04_01:
				case EN_TMH07_NO_04_02:
				case EN_TMH07_NO_04_03:
				case EN_TMH07_NO_05:
					aNewLcom[i].nServLcomType = 2;
					break;

				default:
					break;
				}

				Set_LoadComb4CivilName_proxy(aNewLcom[i], D_SER);
			}
		}
	}
}

void CLoadCombCtrl::Set_CvlTMH07EP_WP(const EN_TMH07_NO& enLcomNo,int m_nCodeBasedOrUserDef,int m_nCodeBasedVerticalEarthPressure,int m_nCodeBasedNonVerticalEarthPressure,double m_dUserDefDL,double m_dUserDefSIDL,double m_dEPRelieving,
	ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arCSADLoad, ArINT& arCSADcLoad, ArINT& arCSADwLoad, ArINT& arPLLoad, ArINT& arBLLoad, ArINT& arStlLoad,
	ArINT& arEsLoad, ArINT& arEpLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, ArINT& arWpLoad, ArINT& arFpLoad,
	ArINT& arLLoad, ArINT& arLsLoad, ArINT& arCrLoad, ArINT& arErLoad, ArINT& arLIPLoad, ArINT& arCfLoad, ArINT& arBrkLoad,
	ArINT& arBkLoad, ArINT& arCoLoad, ArINT& arCtLoad, ArINT& arWlLoad, ArINT& arWLoad, ArINT& arSfLoad,
	ArINT& arWprLoad, ArINT& arELoad, ArINT& arShLoad, ArINT& arPsLoad, ArINT& arTLoad, ArINT& arTpgLoad,
	ArINT& arFrLoad, ArINT& arCSLoad, ArINT& arCrlLoad,ArUNT& arMLoad,  ArUNT &arSmLoad, BOOL bStrength , BOOL bMoving , BOOL bWind )
{
	T_LCOM_D LcomD;

	int nDCount = 1;
	BOOL bExistD = (m_nDCount > 0 || m_nDwCount > 0 || m_nDcCount > 0);
	if (bExistD) nDCount = m_dUserDefDL>2.20?2: 1;//For Both Case of Dead Load
	int nMLCount = max(m_nMCount, 1);
	int nWPCount = (m_nWpCount>=1)?2* m_nWpCount: 1;//Adding and Relieving Water Pressure
	int nEPCount = (m_nEpCount>=1)?2* m_nEpCount: 1;//Adding and Relieving Earth Pressure
	int nPSCount = (m_nPsCount>=1)?2* m_nPsCount: 1;//Adding and Relieving PreStressing
	m_dUserDefDL = m_nCodeBasedOrUserDef == 0 ? 1.2 : m_dUserDefDL;
	double dDFactor[2] = { 1.2,1.15 };//For BOth Case,
	if (nDCount == 1) { dDFactor[0] = m_dUserDefDL, dDFactor[1] = m_dUserDefDL; }

	double dEsFact = m_nConstLoad==1?0.: 1.5;//m_nConstLoad = 0 - ST,m_nConstLoad = 1 - CS,m_nConstLoad = 2 - ST+CS
	double dWPFactor[2] = { 1.2,0 };//Water Pressure Factor
	double dPSFactor = 0.;//{ 0.,0.87 };//Pre Stress Factor
	double dCpShFact = 1.;//Creep Shrinkage Factor
	double dCSFactor = 1.15;//CS Factor
	int nSmCount = 1;
	CString csPrefix(_T("")), strFixM(_T("")), strFixW(_T("")), strFixSm(_T(""));

	int LoadCaseNo = 0, ld = 0, nnn = 0;
	for (int d = 0; d < nDCount; ++d)
	{
		for (int p = 0; p < nPSCount; ++p) {
			if (p % 2 == 0) dPSFactor =  0. ;
			else if (p % 2 == 1) dPSFactor =  0.87;//PS Relieving

			for (int w = 0; w < nWPCount; ++w) {
				for (int e = 0; e < nEPCount; ++e) {
					if (e % 2 == 0) dEsFact = m_nConstLoad == 1 ? 0. : 1.5;
					else if (e % 2 == 1) dEsFact = m_nConstLoad == 1 ? 0. : 0.85;//EP Relieving

					for (int k = 0; k < nSmCount; ++k) {
						for (int j = 0; j < nMLCount; ++j) {
							int nDL = 0, nDc = 0, nSnow = 0, nDw = 0, nPL = 0, nEP = 0, nEV = 0, nES = 0, nEH = 0;
							int nLL = 0, nIL = 0, nLS = 0, nLP = 0, nILP = 0, nCF = 0, nBRK = 0, nBK = 0, nCRL = 0;
							int nB = 0, nWL = 0, nWLL = 0, nTL = 0, nSL = 0, nER = 0, nWP = 0, nFP = 0, nPS = 0;

							LcomD.Initialize();
							LcomD.nActive = 1;
							LoadCaseNo = 0;

							//Permanent
							for (ld = 0; ld < m_nDCount; ++ld) nDL += Set_STLCIncludeCS(arDLoad[ld], dDFactor[d], LoadCaseNo, LcomD, D_SGLD_DL_CVL);
							for (ld = 0; ld < m_nDcCount; ++ld) nDc += Set_STLCIncludeCS(arDcLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
							for (ld = 0; ld < m_nDwCount; ++ld) nDw += Set_STLCIncludeCS(arDwLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);

							for (ld = 0; ld < m_nCSADCount; ++ld) nDL += Set_STLCIncludeCS(arCSADLoad[ld], dDFactor[d], LoadCaseNo, LcomD, D_SGLD_DL_CVL);
							for (ld = 0; ld < m_nCSADcCount; ++ld) nDc += Set_STLCIncludeCS(arCSADcLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
							for (ld = 0; ld < m_nCSADwCount; ++ld) nDw += Set_STLCIncludeCS(arCSADwLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);

							for (ld = 0; ld < m_nPLCount; ++ld) nPL += Set_STLCIncludeCS(arPLLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
							for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);

							//Earth Pressure
							for (ld = 0; ld < m_nEsCount; ++ld) nES += Set_STLCIncludeCS(arEsLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
							for (ld = 0; ld < m_nEpCount; ++ld) nEP += Set_STLCIncludeCS(arEpLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
							//for (ld = 0; ld < m_nEhCount; ++ld) nEH += Set_STLCIncludeCS(arEhLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
							for (ld = 0; ld < m_nEvCount; ++ld) nEV += Set_STLCIncludeCS(arEvLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);

							// for(ld=0; ld<m_nLsCount; ++ld) nLS += Set_STLCIncludeCS(arLsLoad[ld], dLsFactor, LoadCaseNo, LcomD, 0);
								   //if(m_nSmCount > 0) Set_SettleCase(arSmLoad[nSmIndex], dSmFactor, LoadCaseNo, LcomD, TRUE);
							//Water Pressure
							for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dWPFactor[w], LoadCaseNo, LcomD, 0);
							for (ld = 0; ld < m_nWpCount; ld++) nWP += Set_STLCIncludeCS(arWpLoad[ld], dWPFactor[w], LoadCaseNo, LcomD, 0);
							for (ld = 0; ld < m_nFpCount; ld++) nFP += Set_STLCIncludeCS(arFpLoad[ld], dWPFactor[w], LoadCaseNo, LcomD, 0);

							//Vehicle load and Surcharge
							for (ld = 0; ld < m_nLCount; ++ld) nLL += Set_STLCIncludeCS(arLLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
							for (ld = 0; ld < m_nLsCount; ++ld)  nLS += Set_STLCIncludeCS(arLsLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);

							if (m_nMCount > 0)  Set_MovingCase(arMLoad.GetAt(j), 1.0, LoadCaseNo, LcomD, TRUE);
							for (ld = 0; ld < m_nCrlCount; ++ld) nCRL += Set_STLCIncludeCS(arCrlLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);

							for (ld = 0; ld < m_nPsCount; ++ld) nPS += Set_STLCIncludeCS(arPsLoad[ld], dPSFactor, LoadCaseNo, LcomD, D_SGLD_TS_CVL);
							if (m_nSmCount > 0) Set_SettleCase(arSmLoad.GetAt(k), dCpShFact, LoadCaseNo, LcomD, TRUE);


							if (m_nConstLoad != 0) // ST+CS
							{
								if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
									Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor[d], LoadCaseNo, LcomD, TRUE);

								CString strcEL;
								int nErection = 0;
								for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
								{
									strcEL = _T(""); nErection = 0;
									if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }



									if (CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")) || CheckCSEL(nErection, _T("PL")) || CheckCSEL(nErection, _T("BL")))
										Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor[d], LoadCaseNo, LcomD, TRUE);
									else if (CheckCSEL(nErection, _T("D")))
										Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor[d], LoadCaseNo, LcomD, TRUE);

								}
								if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
									Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
								if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
									Set_CStageCase(arCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
								// MQC:4539-JHYUN-20120316
								if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep Primary
								{
									Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
								}
								if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep secondary
								{
									Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
								}
								if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage Primary
								{
									Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
								}
								if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage secondary
								{
									Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
								}
							}

							// description
							LcomD.Description.Empty();
							csPrefix.Empty();
							LcomD.Description += _T("1U:");
							if (nDL > 0)
							{
								LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("D");
								csPrefix = _T(":");
							}
							if (nDc > 0)
							{
								LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("DC");
								csPrefix = _T("+");
							}
							if (nPS > 0)
							{
								LcomD.Description += csPrefix + GetFactorString(dPSFactor) + _T("PS");
								csPrefix = _T(":");
							}
							if (nDw > 0)
							{
								LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("DW");
								csPrefix = _T("+");
							}
							if (nB > 0)
							{
								LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("BL");
								csPrefix = _T("+");
							}
							if (nES > 0)
							{
								LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("ES");
								csPrefix = _T("+");
							}
							if (nEP > 0)
							{
								LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("EP");
								csPrefix = _T("+");
							}
							if (nEH > 0)
							{
								LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("EH");
								csPrefix = _T("+");
							}
							if (nEV > 0)
							{
								LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("EV");
								csPrefix = _T("+");
							}
							if (m_nMCount > 0)
							{
								LcomD.Description += csPrefix + _T("ML"); csPrefix = _T(":");
							}
							if (nB > 0)
							{
								LcomD.Description += csPrefix + GetFactorString(dWPFactor[w]) + _T("BL");
								csPrefix = _T("+");
							}
							if (nWP > 0)
							{
								LcomD.Description += csPrefix + GetFactorString(dWPFactor[w]) + _T("WP");
								csPrefix = _T("+");
							}
							if (nFP > 0)
							{
								LcomD.Description += csPrefix + GetFactorString(dWPFactor[w]) + _T("FP");
								csPrefix = _T("+");
							}
							if (nLL > 0)
							{
								LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("LL");
								csPrefix = _T("+");
							}
							if (nLS > 0)
							{
								LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("LS");
								csPrefix = _T("+");
							}
							if (LoadCaseNo > 0) Set_LoadComb4Civil(LcomD, D_STR);
							if (nCRL > 0)
							{
								LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("CRL");
								csPrefix = _T("+");
							}
							if (m_nConstLoad != 0) // ST+CS
							{


								CString strcEL;
								int nErection = 0;

								for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
								{
									strcEL = _T(""); nErection = 0;

									if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn))
									{
										LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCP)"); csPrefix = _T("+");
									}
									if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn))
									{
										LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCS)"); csPrefix = _T("+");
									}
									if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn))
									{
										LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSP)"); csPrefix = _T("+");
									}
									if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn))
									{
										LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSS)"); csPrefix = _T("+");
									}
								}

							}
						}
					}
				}
			}
		}
	}




	//return TRUE;
}


void CLoadCombCtrl::Set_CvlTMH07EP_WP_T_TPG(const EN_TMH07_NO& enLcomNo, int m_nCodeBasedOrUserDef, int m_nCodeBasedVerticalEarthPressure, int m_nCodeBasedNonVerticalEarthPressure, double m_dUserDefDL, double m_dUserDefSIDL, double m_dEPRelieving,
	ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arCSADLoad, ArINT& arCSADcLoad, ArINT& arCSADwLoad, ArINT& arPLLoad, ArINT& arBLLoad, ArINT& arStlLoad,
	ArINT& arEsLoad, ArINT& arEpLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, ArINT& arWpLoad, ArINT& arFpLoad,
	ArINT& arLLoad, ArINT& arLsLoad, ArINT& arCrLoad, ArINT& arErLoad, ArINT& arLIPLoad, ArINT& arCfLoad, ArINT& arBrkLoad,
	ArINT& arBkLoad, ArINT& arCoLoad, ArINT& arCtLoad, ArINT& arWlLoad, ArINT& arWLoad, ArINT& arSfLoad,
	ArINT& arWprLoad, ArINT& arELoad, ArINT& arShLoad, ArINT& arPsLoad, ArINT& arTLoad, ArINT& arTpgLoad,
	ArINT& arFrLoad, ArINT& arCSLoad, ArINT& arCrlLoad, ArINT& arASLoad, ArUNT& arMLoad, ArUNT &arSmLoad, BOOL bStrength, BOOL bMoving, BOOL bWind)
{
	T_LCOM_D LcomD;

	int nDCount = 1;
	BOOL bExistD = (m_nDCount > 0 || m_nDwCount > 0 || m_nDcCount > 0);
	if (bExistD) nDCount = m_dUserDefDL > 2.20 ? 2 : 1;//For Both Case of Dead Load
	int nMLCount = max(m_nMCount, 1);
	int nWPCount = (m_nWpCount >= 1) ? 3 * m_nWpCount : 1;//Adding and Relieving Water Pressure
	int nEPCount = (m_nEpCount >= 1) ? 3 * m_nEpCount : 1;//Adding and Relieving Earth Pressure
	int nPSCount = (m_nPsCount >= 1) ? 2 * m_nPsCount : 1;//Adding and Relieving PreStressing
	int nWLCount = (m_nWCount >= 1) ? 3 * m_nWCount : 1;//Adding and Relieving Wind Load
	int nWLLCount = (m_nWlCount >= 1) ? 3 * m_nWlCount : 1;//Adding and Relieving Wind Load on Live Load
	m_dUserDefDL = m_nCodeBasedOrUserDef == 0 ? 1.2 : m_dUserDefDL;
	double dDFactor[2] = { 1.05,1.05 };//For BOth Case,
	//if (nDCount == 1) { dDFactor[0] = m_dUserDefDL, dDFactor[1] = m_dUserDefDL; }

	double dEsFact = m_nConstLoad == 1 ? 0. : 1.5;//m_nConstLoad = 0 - ST,m_nConstLoad = 1 - CS,m_nConstLoad = 2 - ST+CS
	double dWPFactor = 1.05;//Water Pressure Factor
	double dPSFactor = 0.;//{ 0.,0.87 };//Pre Stress Factor
	double dCpShFact = 1.;//Creep Shrinkage Factor
	double dCSFactor = 1.15;//CS Factor
	double dWFactor = 1.1;//Wind Factor 1.1 or 0
	double dTPGFact1 = 0.8;
	double dCFForce = 1.5;
	double dCOForce = 1.25;
	double dSfForce = 1.05;
	int nSmCount = 1;

	

	CString csPrefix(_T("")), strFixM(_T("")), strFixW(_T("")), strFixSm(_T(""));

	int LoadCaseNo = 0, ld = 0, nnn = 0,nCSTPCOunt=1;
	double dCSTPFact = 1.15;
	//CS Tendon Primary has 1.15 and 0
	if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn) > 0) nCSTPCOunt = 3;
	for (int d = 0; d < nDCount; ++d)
	{
		for (int p = 0; p < nPSCount; ++p) {
			if (p % 2 == 0) dPSFactor = 1.15;
			else if (p % 2 == 1) dPSFactor = 0.87;//PS Relieving

			for (int w = 0; w < nWPCount; ++w) {
				if (w % 3 == 0) dWPFactor = m_nConstLoad == 1 ? 0 : 1.05;
				else if (w % 3 == 1) dWPFactor = m_nConstLoad == 1 ? 0.0 : 1.0;
				else if (w % 3 == 2) dWPFactor = m_nConstLoad == 1 ? 0.0 : 0.0;

				for (int wl = 0; wl < nWLCount; ++wl) {
					if (wl % 3 == 0) dWFactor = m_nConstLoad == 1 ? 0 : 1.1;
					else if (wl % 3 == 1) dWFactor = m_nConstLoad == 1 ? 0.0 : 1.0;
					else if (wl % 3 == 2) dWFactor = m_nConstLoad == 1 ? 0.0 : 0.0;
					if (p % 2 == 1) dWFactor = dWFactor * -1.;


					for (int e = 0; e < nEPCount; ++e) {
						if (e % 3 == 0) dEsFact = m_nConstLoad == 1 ? 0. : 1.3;
						else if (e % 3 == 1) dEsFact = m_nConstLoad == 1 ? 0. : 0.85;//EP Relieving
						else if (e % 3 == 2) dEsFact = m_nConstLoad == 1 ? 0. : 0.0;//EP Relieving

						for (int k = 0; k < nSmCount; ++k) {
							for (int j = 0; j < nMLCount; ++j) {
								for (int k1 = 0; k1 < nCSTPCOunt; ++k1) {
									if (k1 % 3 == 0) dCSTPFact = m_nConstLoad == 1 ? 0 : 1.15;
									else if (k1 % 3 == 1) dCSTPFact = m_nConstLoad == 1 ? 0.0 : 0.87;
									else if (k1 % 3 == 2) dCSTPFact = m_nConstLoad == 1 ? 0.0 : 0.0;
									
									int nDL = 0, nDc = 0, nSnow = 0, nDw = 0, nPL = 0, nEP = 0, nEV = 0, nES = 0, nEH = 0;
									int nLL = 0, nIL = 0, nLS = 0, nLP = 0, nILP = 0, nCF = 0, nBRK = 0, nBK = 0, nCRL = 0;
									int nB = 0, nWL = 0, nWLL = 0, nTL = 0, nSL = 0, nER = 0, nWP = 0, nFP = 0, nPS = 0,nAS=0;
									int nW = 0, nWl = 0,nCO=0, nSf =0, nWpr=0, nStl=0, nT=0, nTpg=0;
									LcomD.Initialize();
									LcomD.nActive = 1;
									LoadCaseNo = 0;

									//Permanent
									for (ld = 0; ld < m_nDCount; ++ld) nDL += Set_STLCIncludeCS(arDLoad[ld], dDFactor[d], LoadCaseNo, LcomD, D_SGLD_DL_CVL);
									for (ld = 0; ld < m_nDcCount; ++ld) nDc += Set_STLCIncludeCS(arDcLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
									for (ld = 0; ld < m_nDwCount; ++ld) nDw += Set_STLCIncludeCS(arDwLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);

									for (ld = 0; ld < m_nCSADCount; ++ld) nDL += Set_STLCIncludeCS(arCSADLoad[ld], dDFactor[d], LoadCaseNo, LcomD, D_SGLD_DL_CVL);
									for (ld = 0; ld < m_nCSADcCount; ++ld) nDc += Set_STLCIncludeCS(arCSADcLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
									for (ld = 0; ld < m_nCSADwCount; ++ld) nDw += Set_STLCIncludeCS(arCSADwLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);

									for (ld = 0; ld < m_nPLCount; ++ld) nPL += Set_STLCIncludeCS(arPLLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
									for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
	
									////Earth Pressure
									for (ld = 0; ld < m_nEsCount; ++ld) nES += Set_STLCIncludeCS(arEsLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
									for (ld = 0; ld < m_nEpCount; ++ld) nEP += Set_STLCIncludeCS(arEpLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
									////for (ld = 0; ld < m_nEhCount; ++ld) nEH += Set_STLCIncludeCS(arEhLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
									for (ld = 0; ld < m_nEvCount; ++ld) nEV += Set_STLCIncludeCS(arEvLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);

									// for(ld=0; ld<m_nLsCount; ++ld) nLS += Set_STLCIncludeCS(arLsLoad[ld], dLsFactor, LoadCaseNo, LcomD, 0);
										   //if(m_nSmCount > 0) Set_SettleCase(arSmLoad[nSmIndex], dSmFactor, LoadCaseNo, LcomD, TRUE);
									//Water Pressure
									//for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
									for (ld = 0; ld < m_nWpCount; ld++) nWP += Set_STLCIncludeCS(arWpLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
									for (ld = 0; ld < m_nFpCount; ld++) nFP += Set_STLCIncludeCS(arFpLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
									//Wind Load and Wind On Live
									for (ld = 0; ld < m_nWCount; ++ld) nW += Set_STLCIncludeCS(arWLoad[ld], dWFactor, LoadCaseNo, LcomD, 0);
									for (ld = 0; ld < m_nWlCount; ld++) nWl += Set_STLCIncludeCS(arWlLoad[ld], dWFactor, LoadCaseNo, LcomD, 0);

									////Vehicle load and Surcharge
									for (ld = 0; ld < m_nLCount; ++ld) nLL += Set_STLCIncludeCS(arLLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
									for (ld = 0; ld < m_nLsCount; ++ld)  nLS += Set_STLCIncludeCS(arLsLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);

									//if (m_nMCount > 0)  Set_MovingCase(arMLoad.GetAt(j), 1.0, LoadCaseNo, LcomD, TRUE);
									//for (ld = 0; ld < m_nCrlCount; ++ld) nCRL += Set_STLCIncludeCS(arCrlLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);

									for (ld = 0; ld < m_nPsCount; ++ld) nPS += Set_STLCIncludeCS(arPsLoad[ld], dPSFactor, LoadCaseNo, LcomD, D_SGLD_TS_CVL);
									//if (m_nSmCount > 0) Set_SettleCase(arSmLoad.GetAt(k), dCpShFact, LoadCaseNo, LcomD, TRUE);
									for (ld = 0; ld < m_nCfCount; ++ld) nCF += Set_STLCIncludeCS(arCfLoad[ld], dCFForce, LoadCaseNo, LcomD, 0);
									for (ld = 0; ld < m_nASCount; ++ld) nAS += Set_STLCIncludeCS(arASLoad[ld], dCFForce, LoadCaseNo, LcomD, 0);
									for (ld = 0; ld < m_nCoCount; ++ld) nCO += Set_STLCIncludeCS(arCoLoad[ld], dCOForce, LoadCaseNo, LcomD, 0);
									for (ld = 0; ld < m_nSfCount; ++ld) nSf += Set_STLCIncludeCS(arSfLoad[ld], dSfForce, LoadCaseNo, LcomD, 0);
									for (ld = 0; ld < m_nWprCount; ++ld) nWpr += Set_STLCIncludeCS(arWprLoad[ld], dSfForce, LoadCaseNo, LcomD, 0);
									for (ld = 0; ld < m_nStlCount; ++ld) nStl += Set_STLCIncludeCS(arStlLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
									for (ld = 0; ld < m_nTCount; ++ld) nT += Set_STLCIncludeCS(arTLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
									for (ld = 0; ld < m_nTpgCount; ++ld) nTpg += Set_STLCIncludeCS(arTpgLoad[ld], dTPGFact1, LoadCaseNo, LcomD, 0);
									
									if (m_nConstLoad != 0) // ST+CS
									{
										if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
											Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor[d], LoadCaseNo, LcomD, TRUE);

										CString strcEL;
										int nErection = 0;
										for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
										{
											strcEL = _T(""); nErection = 0;
											if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }



											if (CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")) || CheckCSEL(nErection, _T("PL")) || CheckCSEL(nErection, _T("BL")))
												Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor[d], LoadCaseNo, LcomD, TRUE);
											else if (CheckCSEL(nErection, _T("D")))
												Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor[d], LoadCaseNo, LcomD, TRUE);
											else if (CheckCSEL(nErection, _T("ER")))
												Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
											else if (CheckCSEL(nErection, _T("LIP")))
												Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
											else if (CheckCSEL(nErection, _T("L")))
												Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);

											else if (CheckCSEL(nErection, _T("T")))
												Set_CStageCase(arCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
											else if (CheckCSEL(nErection, _T("TPG")))
												Set_CStageCase(arCSLoad.GetAt(nnn), dTPGFact1, LoadCaseNo, LcomD, TRUE);

										}
										if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
											Set_CStageCase(arCSLoad.GetAt(nnn), dCSTPFact, LoadCaseNo, LcomD, TRUE);
										if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
											Set_CStageCase(arCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);

										// MQC:4539-JHYUN-20120316
										if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep Primary
										{
											Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
										}
										if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep secondary
										{
											Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
										}
										if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage Primary
										{
											Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
										}
										if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage secondary
										{
											Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
										}

									}

									// description
									LcomD.Description.Empty();
									csPrefix.Empty();
									LcomD.Description += _T("2U:");
									if (nDL > 0)
									{
										LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("D");
										csPrefix = _T(":");
									}
									if (nDc > 0)
									{
										LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("DC");
										csPrefix = _T("+");
									}
									if (nPS > 0)
									{
										LcomD.Description += csPrefix + GetFactorString(dPSFactor) + _T("PS");
										csPrefix = _T(":");
									}
									if (nDw > 0)
									{
										LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("DW");
										csPrefix = _T("+");
									}
									if (nW > 0)
									{
										LcomD.Description += csPrefix + GetFactorString(dWFactor) + _T("WL");
										csPrefix = _T("+");
									}
									if (nB > 0)
									{
										LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("BL");
										csPrefix = _T("+");
									}
									if (nES > 0)
									{
										LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("ES");
										csPrefix = _T("+");
									}
									if (nEP > 0)
									{
										LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("EP");
										csPrefix = _T("+");
									}
									if (nEH > 0)
									{
										LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("EH");
										csPrefix = _T("+");
									}
									if (nEV > 0)
									{
										LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("EV");
										csPrefix = _T("+");
									}
									if (m_nMCount > 0)
									{
										LcomD.Description += csPrefix + _T("ML"); csPrefix = _T(":");
									}
									if (nB > 0)
									{
										LcomD.Description += csPrefix + GetFactorString(dWPFactor) + _T("BL");
										csPrefix = _T("+");
									}
									if (nWP > 0)
									{
										LcomD.Description += csPrefix + GetFactorString(dWPFactor) + _T("WP");
										csPrefix = _T("+");
									}
									if (nFP > 0)
									{
										LcomD.Description += csPrefix + GetFactorString(dWPFactor) + _T("FP");
										csPrefix = _T("+");
									}
									if (nLL > 0)
									{
										LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("LL");
										csPrefix = _T("+");
									}
									if (nLS > 0)
									{
										LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("LS");
										csPrefix = _T("+");
									}
									if (LoadCaseNo > 0) Set_LoadComb4Civil(LcomD, D_STR);
									if (nCRL > 0)
									{
										LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("CRL");
										csPrefix = _T("+");
									}
									if (m_nConstLoad != 0) // ST+CS
									{


										CString strcEL;
										int nErection = 0;

										for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
										{
											strcEL = _T(""); nErection = 0;

											if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn))
											{
												LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCP)"); csPrefix = _T("+");
											}
											if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn))
											{
												LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCS)"); csPrefix = _T("+");
											}
											if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn))
											{
												LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSP)"); csPrefix = _T("+");
											}
											if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn))
											{
												LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSS)"); csPrefix = _T("+");
											}
											if (CheckCSEL(nErection, _T("ER")))
											{
												LcomD.Description += csPrefix + GetFactorString(dCSFactor) + +_T("(cER)") + strcEL; csPrefix = _T("+");
											}
											if (CheckCSEL(nErection, _T("LIP")))
											{
												LcomD.Description += csPrefix + GetFactorString(dCSFactor) + _T("(cLIP)") + strcEL; csPrefix = _T("+");
											}
											if (CheckCSEL(nErection, _T("LL")))
											{
												LcomD.Description += csPrefix + GetFactorString(dCSFactor) + _T("(cLL)") + strcEL; csPrefix = _T("+");
											}
										}

									}
								}
							}
						}
					}
				}
			}
		}
	}



	//return TRUE;
}



void CLoadCombCtrl::Set_CvlTMH07_Seis(const EN_TMH07_NO& enLcomNo, int m_nCodeBasedOrUserDef, int m_nCodeBasedVerticalEarthPressure, int m_nCodeBasedNonVerticalEarthPressure, double m_dUserDefDL_Max, double m_dUserDefDL_Min, double m_dUserDefSIDL_Max, double m_dUserDefSIDL_Min, double m_dEPRelieving,int m_dUserRdo_Dmax,int m_dUserRdo_Dmin,int m_dUserRdo_Dboth,int m_dUserRdo_SIDLmax,int m_dUserRdo_SIDLmin,int m_dUserRdo_SIDLboth,
    ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arCSADLoad, ArINT& arCSADcLoad, ArINT& arCSADwLoad, ArINT& arPLLoad, ArINT& arBLLoad, ArINT& arStlLoad,
    ArINT& arEsLoad, ArINT& arEpLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, ArINT& arWpLoad, ArINT& arFpLoad,
    ArINT& arLLoad, ArINT& arLsLoad, ArINT& arCrLoad, ArINT& arErLoad, ArINT& arLIPLoad, ArINT& arCfLoad, ArINT& arBrkLoad,
    ArINT& arBkLoad, ArINT& arCoLoad, ArINT& arCtLoad, ArINT& arWlLoad, ArINT& arWLoad, ArINT& arSfLoad,
    ArINT& arWprLoad, ArINT& arELoad, ArINT& arShLoad, ArINT& arPsLoad, ArINT& arTLoad, ArINT& arTpgLoad,
    ArINT& arFrLoad, ArINT& arCSLoad, ArINT& arCrlLoad, ArINT& arASLoad, ArINT& arRsLoad, ArUNT& arEspLoad,
    ArUNT& arMLoad, ArUNT& arSmLoad, BOOL bStrength, BOOL bMoving, BOOL bWind, BOOL bSeis)
{
    if (!Check_TMH07_LoadCase(enLcomNo)) return;

    CString csPrefix_Tmp(_T("+"));
    CArray<T_LCOM_D, T_LCOM_D&> aNewLcom;
    aNewLcom.RemoveAll();

    //Create combinations
    Add_CvlTMH07_Title(enLcomNo, bStrength);
    CString sDFac[2], sDwFac[2], sDcFac[2], sEvFac[2], sEhFac[2], sEsFac[2], sCrlFac[2], sBFac[2], sGf3ULSFac, sGf3SLSFac;
    double  dDFac[2], dDwFac[2], dDcFac[2], dEvFac[2], dEhFac[2], dBFac[2], dCrlFac[2],dGf3ULS, dGf3SLS,dEsFac[2];
    double  dStlFactor, dMFactor, dBrCfFactor, dCRSHFactor, dPsFactor, dWFactor, dTFactor, dTpgFactor, dCoFactor, dAsFactor, dCfFactor,dSfFactor,dErFactor,dEpFactor,dEqFactor;

    GetBD_21FactorTMH(sDFac[0], sDFac[1], sDwFac[0], sDwFac[1], sDcFac[0], sDcFac[1], sEvFac[0], sEvFac[1], sEhFac[0], sEhFac[1], sEsFac[0], sEsFac[1], sCrlFac[0], sCrlFac[1], sGf3ULSFac, sGf3SLSFac,
        dDFac[0], dDFac[1], dDwFac[0], dDwFac[1], dDcFac[0], dDcFac[1], dEvFac[0], dEvFac[1], dEhFac[0], dEhFac[1],dEsFac[0],dEsFac[1],dBFac[0],dBFac[1], dCrlFac[0], dCrlFac[1],dGf3ULS, dGf3SLS, bStrength,m_nCodeBasedVerticalEarthPressure,m_nCodeBasedNonVerticalEarthPressure, m_nCodeBasedOrUserDef,m_dUserDefDL_Max,m_dUserDefDL_Min,m_dUserDefSIDL_Max,m_dUserDefSIDL_Min, m_dUserRdo_Dmax, m_dUserRdo_Dmin, m_dUserRdo_Dboth,m_dUserRdo_SIDLmax,m_dUserRdo_SIDLmin,m_dUserRdo_SIDLboth, enLcomNo);

    if (bStrength)
    {
        GetBD_21FactoredPermTMH(dDFac[0], dDFac[1], dDwFac[0], dDwFac[1], dDcFac[0], dDcFac[1], dEvFac[0], dEvFac[1], dEhFac[0], dEhFac[1], dGf3ULS);
        dStlFactor = dGf3ULS * 1.0;
        dMFactor = 1.0;
        dBrCfFactor = dGf3ULS * 1.5;
        dCRSHFactor = dGf3ULS * 1.0;
        dPsFactor = dGf3ULS * 1.15;
        if (enLcomNo == EN_TMH07_NO_02_01) {
            dTFactor = dGf3ULS * 1;
        }
        else {
            dTFactor = dGf3ULS * 1.3;
        }
		if (enLcomNo == EN_TMH07_NO_02_01) {
			dTpgFactor = dGf3ULS * 0.8;
		}
		else {
			dTpgFactor = dGf3ULS * 1.0;
		}
        dCoFactor = dGf3ULS * 1.25;
        dAsFactor = dGf3ULS * 1.25;
        dCfFactor = dGf3ULS * 1.5;
        if (enLcomNo == EN_TMH07_NO_02_01) {
            dSfFactor = dGf3ULS * 1.05;
        }
        else if (enLcomNo == EN_TMH07_NO_EQ_ST || enLcomNo == EN_TMH07_NO_EQ_RS) {
            dSfFactor = dGf3ULS * 1.3;
        }
        if (enLcomNo == EN_TMH07_NO_EQ_ST || enLcomNo == EN_TMH07_NO_EQ_RS)
        {
            dWFactor = dGf3ULS * 1.4;
        }
        else if (enLcomNo == EN_TMH07_NO_02_01)
        {
            dWFactor = dGf3ULS * 1.1;
        }
        dErFactor = dGf3ULS * 1.15;
    }
    else
    {
        GetBD_21FactoredPermTMH(dDFac[0], dDFac[1], dDwFac[0], dDwFac[1], dDcFac[0], dDcFac[1], dEvFac[0], dEvFac[1], dEhFac[0], dEhFac[1], dGf3SLS);
        dStlFactor = dGf3SLS * 1.0;
        dMFactor = 1.0;
        dBrCfFactor = dGf3SLS * 1.0;
        dCRSHFactor = dGf3SLS * 1.0;
        dPsFactor = dGf3SLS * 1.0;
        dWFactor = dGf3SLS * 1.0;
		if (enLcomNo == EN_TMH07_NO_02_01) {
			dTFactor = dGf3SLS * 0.7;
		}
		else {
			dTFactor = dGf3SLS * 1.0;
		}
		if (enLcomNo == EN_TMH07_NO_02_01) {
			dTpgFactor = dGf3SLS * 0.6;
		}
		else {
			dTpgFactor = dGf3SLS * 0.8;
		}
        /*dTpgFactor = dGf3SLS * 0.8;*/
        dCoFactor = dGf3SLS * 1.0;
        dAsFactor = dGf3SLS * 1.0;
        dCfFactor = dGf3SLS * 1.0;
		if (enLcomNo == EN_TMH07_NO_02_01) {
			dSfFactor = dGf3SLS * 0.85;
		}
		else if (enLcomNo == EN_TMH07_NO_EQ_ST || enLcomNo ==EN_TMH07_NO_EQ_RS) {
			dSfFactor = dGf3SLS * 1.0;
		}
		dErFactor = dGf3SLS * 1.0;

    }
    Add_CvlTMH07_Title(enLcomNo, bStrength, aNewLcom);

    if (m_nConstLoad != 1)
    {
        Add_CvlTMH07_D(enLcomNo, bStrength, arDLoad, dDFac[0], dDFac[1], aNewLcom);
        Add_CvlTMH07_DW(enLcomNo, bStrength, arDwLoad, dDwFac[0], dDwFac[1], aNewLcom);
        Add_CvlTMH07_DC(enLcomNo, bStrength, arDcLoad, dDcFac[0], dDcFac[1], aNewLcom);
        Add_CvlTMH07_ES(enLcomNo, bStrength, arEsLoad, dEsFac[0], dEsFac[1], aNewLcom, m_nCodeBasedVerticalEarthPressure, dGf3ULS, dGf3SLS);
        Add_CvlTMH07_CRL(enLcomNo, bStrength, arCrlLoad, dCrlFac[0], dCrlFac[1], aNewLcom, dGf3ULS, dGf3SLS);
        Add_CvlTMH07_EV(enLcomNo, bStrength, arEvLoad, dEvFac[0], dEvFac[1], aNewLcom, m_dEPRelieving, dGf3ULS, dGf3SLS);
        Add_CvlTMH07_B(enLcomNo, bStrength, arBLoad, dBFac[0], dBFac[1], aNewLcom, m_dEPRelieving, dGf3ULS, dGf3SLS);
        /*Add_CvlTMH07_EH(enLcomNo, bStrength, arEhLoad, dEhFac[0], dEhFac[1], aNewLcom);*/
        Add_CvlTMH07_STL_SM(enLcomNo, bStrength, arStlLoad, arSmLoad, dStlFactor, aNewLcom);
        /*Add_CvlTMH07_T_TPG(enLcomNo, bStrength, arTLoad, arTpgLoad, dTFactor, dTpgFactor, aNewLcom);*/
        Add_CvlTMH07_T(enLcomNo, bStrength, arTLoad, arTpgLoad, dTFactor, dTpgFactor, aNewLcom);
        Add_CvlTMH07_TPG(enLcomNo, bStrength, arTLoad, arTpgLoad, dTFactor, dTpgFactor, aNewLcom);
        Add_CvlTMH07_FA(enLcomNo, bStrength, arSfLoad, dSfFactor, aNewLcom);
        Add_CvlTMH07_CF(enLcomNo, bStrength, arCfLoad, dCfFactor, aNewLcom);
        Add_CvlTMH07_CO(enLcomNo, bStrength, arCoLoad, dCoFactor, aNewLcom);
        Add_CvlTMH07_AS(enLcomNo, bStrength, arASLoad, dAsFactor, aNewLcom);
        Add_CvlTMH07_BRK(enLcomNo, bStrength, arBrkLoad, dGf3ULS, dGf3SLS, aNewLcom);
        Add_CvlTMH07_ER(enLcomNo, bStrength, arErLoad, dErFactor, aNewLcom);
        Add_CvlTMH07_PS_CR_SH(enLcomNo, bStrength, arPsLoad, arCrLoad, arShLoad, arCSLoad, dPsFactor, dCRSHFactor, aNewLcom, m_dEPRelieving, dGf3ULS, dGf3SLS);
        if (bSeis)
        {
            const BOOL bOrtho = (m_nOrthoType == 0 ? TRUE : FALSE);
            const int  nOrtho = m_a100_30_Load.GetSize();
            if (bOrtho && nOrtho > 0)
            {
                Add_CvlTMH07_EQ_Ortho(enLcomNo, arELoad, arEspLoad, csPrefix_Tmp, aNewLcom, bStrength, dGf3ULS, dGf3SLS);
            }
            else
            {
                Add_CvlTMH07_EQ(enLcomNo, arELoad, arEspLoad, arCSLoad,csPrefix_Tmp, aNewLcom, bStrength, dGf3ULS, dGf3SLS);
            }
        }


        if (bMoving)
        {
                  ArUNT arCombinedLoad;

                  //// Copy contents of arMLoad
                  for (int i = 0; i < arMLoad.GetSize(); ++i)
                      arCombinedLoad.Add(arMLoad[i]); 

                  //// Append contents of arLsLoad
                  for (int i = 0; i < arLLoad.GetSize(); ++i)
                      arCombinedLoad.Add(arLLoad[i]);

            Add_CvlTMH07_M(enLcomNo, bStrength, arMLoad,arLLoad,arCombinedLoad,dMFactor, dGf3ULS, dGf3SLS, aNewLcom);
            Add_CvlTMH07_LS(enLcomNo, bStrength, arLsLoad, arLLoad, arCombinedLoad, dMFactor, dGf3ULS, dGf3SLS, aNewLcom);
        }
        if (bWind)
        {
            Add_CvlTMH07_W(enLcomNo, bStrength, arWLoad, dWFactor, aNewLcom, dGf3ULS, dGf3SLS);
        }
    }
   
    if (m_nConstLoad != 0) {
        Add_CvlTMH07_CS_D(enLcomNo, bStrength, arCSLoad, dDFac[0], dDFac[1], aNewLcom);
        Add_CvlTMH07_CS_DW(enLcomNo, bStrength, arCSLoad, dDwFac[0], dDwFac[1], aNewLcom);
        Add_CvlTMH07_CS_DC(enLcomNo, bStrength, arCSLoad, dDcFac[0], dDcFac[1], aNewLcom);
        /*if (bWind)
        {
            Add_CvlTMH07_CS_W(enLcomNo, bStrength, arCSLoad, dPsFactor, aNewLcom);
        }*/

        Add_CvlTMH07_CS_ER_LL(enLcomNo, bStrength, arCSLoad, dTFactor, dTpgFactor, dErFactor, aNewLcom);
        Add_CvlTMH07_CS_EP(enLcomNo, bStrength, arCSLoad, dEpFactor, aNewLcom, m_dEPRelieving, dGf3ULS, dGf3SLS, m_nCodeBasedNonVerticalEarthPressure);
        Add_CvlTMH07_CS_TPG(enLcomNo, bStrength, arCSLoad, dTpgFactor, aNewLcom);
        Add_CvlTMH07_CS_T(enLcomNo, bStrength, arCSLoad, dTFactor, aNewLcom);
        Add_CvlTMH07_CS_PSCRSH(enLcomNo, bStrength, arCSLoad, dPsFactor, dCRSHFactor, aNewLcom);
        if (m_nConstLoad != 2) {
            Add_CvlTMH07_STL_SM(enLcomNo, bStrength, arStlLoad, arSmLoad, dStlFactor, aNewLcom);
        }
        /* Add_CvlTMH07_CS_TP(enLcomNo, bStrength, arCSLoad, dPsFactor,aNewLcom,m_dEPRelieving,dGf3ULS,dGf3SLS);*/
        if (bSeis) {
            Add_CvlTMH07_CS_EQ(enLcomNo, bStrength, arCSLoad, dEqFactor, aNewLcom, dGf3ULS, dGf3SLS);
        }
        if (bWind) {
            Add_CvlTMH07_CS_W(enLcomNo, bStrength, arCSLoad, dWFactor, dGf3ULS, dGf3SLS, aNewLcom);
        }
        if (m_nConstLoad != 2) {
            ArUNT arCombinedLoad;

            //// Copy contents of arMLoad
            for (int i = 0; i < arMLoad.GetSize(); ++i)
                arCombinedLoad.Add(arMLoad[i]);

            //// Append contents of arLsLoad
            for (int i = 0; i < arLLoad.GetSize(); ++i)
                arCombinedLoad.Add(arLLoad[i]);

            Add_CvlTMH07_M(enLcomNo, bStrength, arMLoad, arLLoad, arCombinedLoad, dMFactor, dGf3ULS, dGf3SLS, aNewLcom);
        }
     /*   if (bSeis && m_nConstLoad != 2)
        {
            const BOOL bOrtho = (m_nOrthoType == 0 ? TRUE : FALSE);
            const int  nOrtho = m_a100_30_Load.GetSize();
            if (bOrtho && nOrtho > 0)
            {
                Add_CvlTMH07_EQ_Ortho(enLcomNo, arELoad, arEspLoad, csPrefix_Tmp, aNewLcom, bStrength, dGf3ULS, dGf3SLS);
            }
            else
            {
                Add_CvlTMH07_EQ(enLcomNo, arELoad, arEspLoad, arCSLoad, csPrefix_Tmp, aNewLcom, bStrength, dGf3ULS, dGf3SLS);
    
            }
        }*/
}

    //Add seismic load case (static and RS) to each load combination
	/*if (bSeis)
	{
		const BOOL bOrtho = (m_nOrthoType == 0 ? TRUE : FALSE);
		const int  nOrtho = m_a100_30_Load.GetSize();
		if (bOrtho && nOrtho > 0)
		{
			Add_CvlTMH07_EQ_Ortho(enLcomNo, arELoad, arEspLoad, csPrefix_Tmp, aNewLcom, bStrength);
		}
		else
		{
			Add_CvlTMH07_EQ(enLcomNo, arELoad, arEspLoad, csPrefix_Tmp, aNewLcom, bStrength);
		}
	}*/

	// Generate new load combinations.
	T_LCOM_D envLcomD; envLcomD.Initialize();
	int nNewLcom = aNewLcom.GetSize();
    std::unordered_set<std::wstring> uniqueCombinations;
	for (int i = 0; i < nNewLcom; ++i)
	{
		if (aNewLcom[i].aCombination.GetSize() == 0) continue;
        CString comboKey = SerializeCombination(aNewLcom[i]);
        std::wstring wComboKey(comboKey.GetString());

        // Fast duplicate check
        if (uniqueCombinations.find(wComboKey) != uniqueCombinations.end())
            continue;
        uniqueCombinations.insert(wComboKey);

		if (bStrength)
		{
            if (m_nLcomType == D_LCOMTYPE_CONCRETE || D_LCOMTYPE_GENERAL)
			{
				aNewLcom[i].nActive = 1;
			}
		}

		else if (bSeis)
		{
			aNewLcom[i].nActive = 1;
		}
        if (!bStrength)
        {
            if (m_nLcomType == D_LCOMTYPE_CONCRETE || D_LCOMTYPE_GENERAL)
            {
                aNewLcom[i].nActive = 2;
            }
        }

	/*	if (!ExistInNewLcom(aNewLcom[i], TRUE))
		{*/
			/*if (bStrength || bSeis)*/
			/*{*/
				/*Set_LoadComb4Civil(aNewLcom[i], D_STR);*/
			/*if (bStrength)
			{*/
				/*Set_LoadComb4Civil(aNewLcom[i], D_STR);*/
			/*}*/
		/*	else
			{*/
			switch (enLcomNo)
			{
			case EN_TMH07_NO_01:
				aNewLcom[i].nServLcomType = 1;
				break;
			case EN_TMH07_NO_02_01:
				aNewLcom[i].nServLcomType = 2;
				break;
			case EN_TMH07_NO_02_02:
			case EN_TMH07_NO_02_03:
			case EN_TMH07_NO_03:
			case EN_TMH07_NO_04_01:
			case EN_TMH07_NO_04_02:
			case EN_TMH07_NO_04_03:
            case EN_TMH07_NO_EQ_RS:
                aNewLcom[i].nServLcomType = 2;
                break;
            case EN_TMH07_NO_EQ_ST:
                aNewLcom[i].nServLcomType = 2;
                break;
			case EN_TMH07_NO_05:
				aNewLcom[i].nServLcomType = 2;
				break;

			default:
				break;
			}

			Set_LoadComb4Civil(aNewLcom[i], D_STR);
		/*	}*/
		/*}*/
	}
//	/////////////////////54 Cases///////////////////////////////////////////////////////////////////////////////////////////////////////////
//		T_LCOM_D LcomD;
//
//	int nEsp_x = arEspLoad_X.GetSize();
//	int nEsp_y = arEspLoad_Y.GetSize();
//	int nEsp_z = arEspLoad_Z.GetSize();
//	int nTtlESP = nEsp_x + nEsp_y + nEsp_z;
//
//	int nDCount = 1;
//	BOOL bExistD = (m_nDCount > 0 || m_nDwCount > 0 || m_nDcCount > 0);
//	if (bExistD) nDCount = m_dUserDefDL > 2.20 ? 2 : 1;//For Both Case of Dead Load
//	int nMLCount = max(m_nMCount, 1);
//	int nWPCount = (m_nWpCount >= 1) ? 3 * m_nWpCount : 1;//Adding and Relieving Water Pressure
//	int nEPCount = (m_nEpCount >= 1) ? 3 * m_nEpCount : 1;//Adding and Relieving Earth Pressure
//	int nPSCount = (m_nPsCount >= 1) ? 2 * m_nPsCount : 1;//Adding and Relieving PreStressing
//	int nWLCount = (m_nWCount >= 1) ? 3 * m_nWCount : 1;//Adding and Relieving Wind Load
//	int nWLLCount = (m_nWlCount >= 1) ? 3 * m_nWlCount : 1;//Adding and Relieving Wind Load on Live Load
//	m_dUserDefDL = m_nCodeBasedOrUserDef == 0 ? 1.2 : m_dUserDefDL;
//	double dDFactor[2] = { 1.2,1.2 };//For BOth Case,
//	//if (nDCount == 1) { dDFactor[0] = m_dUserDefDL, dDFactor[1] = m_dUserDefDL; }
//
//	double dEsFact = m_nConstLoad == 1 ? 0. : 1.5;//m_nConstLoad = 0 - ST,m_nConstLoad = 1 - CS,m_nConstLoad = 2 - ST+CS
//	double dWPFactor = 1.05;//Water Pressure Factor
//	double dPSFactor = 0.;//{ 0.,0.87 };//Pre Stress Factor
//	double dCpShFact = 1.;//Creep Shrinkage Factor
//	double dCSFactor = 1.15;//CS Factor
//	double dWFactor = 1.1;//Wind Factor 1.1 or 0
//	double dTPGFact1 = 0.8;
//	double dCFForce = 1.5;
//	double dCOForce = 1.25;
//	double dSfForce = 1.05;
//	int nSmCount = 1;
//
//	double dRsFactor = -1.3;
//	int nESPCount_X = (nEsp_x > 0) ? nEsp_x * 2 : 1;
//	int nESPCount_Y = (nEsp_y > 0) ? nEsp_y * 2 : 1;
//	int nESPCount_Z = (nEsp_z > 0) ? nEsp_z * 2 : 1;
//	double dEFactor_x = dRsFactor, dEFactor_y = dRsFactor *0.3;
//
//	CString csPrefix(_T("")), strFixM(_T("")), strFixW(_T("")), strFixSm(_T(""));
//
//	int LoadCaseNo = 0, ld = 0, nnn = 0, nCSTPCOunt = 1;
//	double dCSTPFact = 1.15;
//	//CS Tendon Primary has 1.15 and 0
//	if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn) > 0) nCSTPCOunt = 3;
//
//	if (eql == 0)
//	{
//		////////////// X Major //////////////// 
//		for (int d = 0; d < nDCount; ++d)
//		{
//			
//
//			for (int p = 0; p < nPSCount; ++p) {
//				if (p % 2 == 0) dPSFactor = 1.15;
//				else if (p % 2 == 1) dPSFactor = 0.87;//PS Relieving
//
//				for (int w = 0; w < nWPCount; ++w) {
//					if (w % 3 == 0) dWPFactor = m_nConstLoad == 1 ? 0 : 1.05;
//					else if (w % 3 == 1) dWPFactor = m_nConstLoad == 1 ? 0.0 : 1.0;
//					else if (w % 3 == 2) dWPFactor = m_nConstLoad == 1 ? 0.0 : 0.0;
//
//					for (int wl = 0; wl < nWLCount; ++wl) {
//						if (wl % 3 == 0) dWFactor = m_nConstLoad == 1 ? 0 : 1.1;
//						else if (wl % 3 == 1) dWFactor = m_nConstLoad == 1 ? 0.0 : 1.0;
//						else if (wl % 3 == 2) dWFactor = m_nConstLoad == 1 ? 0.0 : 0.0;
//						if (p % 2 == 1) dWFactor = dWFactor * -1.;
//
//
//						for (int e = 0; e < nEPCount; ++e) {
//							if (e % 3 == 0) dEsFact = m_nConstLoad == 1 ? 0. : 1.3;
//							else if (e % 3 == 1) dEsFact = m_nConstLoad == 1 ? 0. : 0.85;//EP Relieving
//							else if (e % 3 == 2) dEsFact = m_nConstLoad == 1 ? 0. : 0.0;//EP Relieving
//
//							for (int k = 0; k < nSmCount; ++k) {
//								for (int j = 0; j < nMLCount; ++j) {
//									for (int k1 = 0; k1 < nCSTPCOunt; ++k1) {
//										if (k1 % 3 == 0) dCSTPFact = m_nConstLoad == 1 ? 0 : 1.15;
//										else if (k1 % 3 == 1) dCSTPFact = m_nConstLoad == 1 ? 0.0 : 0.87;
//										else if (k1 % 3 == 2) dCSTPFact = m_nConstLoad == 1 ? 0.0 : 0.0;
//
//										
//
//										for (int m = 0; m < nESPCount_X; m++) {
//											for (int n = 0; n < nESPCount_Y; n++) {
//												for (int o = 0; o < nESPCount_Z; o++) {
//
//													int nDL = 0, nDc = 0, nSnow = 0, nDw = 0, nPL = 0, nEP = 0, nEV = 0, nES = 0, nEH = 0;
//													int nLL = 0, nIL = 0, nLS = 0, nLP = 0, nILP = 0, nCF = 0, nBRK = 0, nBK = 0, nCRL = 0;
//													int nB = 0, nWL = 0, nWLL = 0, nTL = 0, nSL = 0, nER = 0, nWP = 0, nFP = 0, nPS = 0, nAS = 0;
//													int nW = 0, nWl = 0, nCO = 0, nSf = 0, nWpr = 0, nStl = 0, nT = 0, nTpg = 0;
//													
//													LcomD.Initialize();
//													LcomD.nActive = 1;
//													LoadCaseNo = 0;
//
//													//Permanent
//													for (ld = 0; ld < m_nDCount; ++ld) nDL += Set_STLCIncludeCS(arDLoad[ld], dDFactor[d], LoadCaseNo, LcomD, D_SGLD_DL_CVL);
//													for (ld = 0; ld < m_nDcCount; ++ld) nDc += Set_STLCIncludeCS(arDcLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nDwCount; ++ld) nDw += Set_STLCIncludeCS(arDwLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
//
//													for (ld = 0; ld < m_nCSADCount; ++ld) nDL += Set_STLCIncludeCS(arCSADLoad[ld], dDFactor[d], LoadCaseNo, LcomD, D_SGLD_DL_CVL);
//													for (ld = 0; ld < m_nCSADcCount; ++ld) nDc += Set_STLCIncludeCS(arCSADcLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nCSADwCount; ++ld) nDw += Set_STLCIncludeCS(arCSADwLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
//
//													for (ld = 0; ld < m_nPLCount; ++ld) nPL += Set_STLCIncludeCS(arPLLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
//
//													////Earth Pressure
//													for (ld = 0; ld < m_nEsCount; ++ld) nES += Set_STLCIncludeCS(arEsLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nEpCount; ++ld) nEP += Set_STLCIncludeCS(arEpLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
//													////for (ld = 0; ld < m_nEhCount; ++ld) nEH += Set_STLCIncludeCS(arEhLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nEvCount; ++ld) nEV += Set_STLCIncludeCS(arEvLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
//
//													// for(ld=0; ld<m_nLsCount; ++ld) nLS += Set_STLCIncludeCS(arLsLoad[ld], dLsFactor, LoadCaseNo, LcomD, 0);
//														   //if(m_nSmCount > 0) Set_SettleCase(arSmLoad[nSmIndex], dSmFactor, LoadCaseNo, LcomD, TRUE);
//													//Water Pressure
//													//for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nWpCount; ld++) nWP += Set_STLCIncludeCS(arWpLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nFpCount; ld++) nFP += Set_STLCIncludeCS(arFpLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
//													//Wind Load and Wind On Live
//													for (ld = 0; ld < m_nWCount; ++ld) nW += Set_STLCIncludeCS(arWLoad[ld], dWFactor, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nWlCount; ld++) nWl += Set_STLCIncludeCS(arWlLoad[ld], dWFactor, LoadCaseNo, LcomD, 0);
//
//													////Vehicle load and Surcharge
//													for (ld = 0; ld < m_nLCount; ++ld) nLL += Set_STLCIncludeCS(arLLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nLsCount; ++ld)  nLS += Set_STLCIncludeCS(arLsLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
//
//													//if (m_nMCount > 0)  Set_MovingCase(arMLoad.GetAt(j), 1.0, LoadCaseNo, LcomD, TRUE);
//													//for (ld = 0; ld < m_nCrlCount; ++ld) nCRL += Set_STLCIncludeCS(arCrlLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
//
//													for (ld = 0; ld < m_nPsCount; ++ld) nPS += Set_STLCIncludeCS(arPsLoad[ld], dPSFactor, LoadCaseNo, LcomD, D_SGLD_TS_CVL);
//													//if (m_nSmCount > 0) Set_SettleCase(arSmLoad.GetAt(k), dCpShFact, LoadCaseNo, LcomD, TRUE);
//													for (ld = 0; ld < m_nCfCount; ++ld) nCF += Set_STLCIncludeCS(arCfLoad[ld], dCFForce, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nASCount; ++ld) nAS += Set_STLCIncludeCS(arASLoad[ld], dCFForce, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nCoCount; ++ld) nCO += Set_STLCIncludeCS(arCoLoad[ld], dCOForce, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nSfCount; ++ld) nSf += Set_STLCIncludeCS(arSfLoad[ld], dSfForce, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nWprCount; ++ld) nWpr += Set_STLCIncludeCS(arWprLoad[ld], dSfForce, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nStlCount; ++ld) nStl += Set_STLCIncludeCS(arStlLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nTCount; ++ld) nT += Set_STLCIncludeCS(arTLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nTpgCount; ++ld) nTpg += Set_STLCIncludeCS(arTpgLoad[ld], dTPGFact1, LoadCaseNo, LcomD, 0);
//
//													if (m_nConstLoad != 0) // ST+CS
//													{
//														if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
//															Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor[d], LoadCaseNo, LcomD, TRUE);
//
//														CString strcEL;
//														int nErection = 0;
//														for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
//														{
//															strcEL = _T(""); nErection = 0;
//															if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }
//
//
//
//															if (CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")) || CheckCSEL(nErection, _T("PL")) || CheckCSEL(nErection, _T("BL")))
//																Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor[d], LoadCaseNo, LcomD, TRUE);
//															else if (CheckCSEL(nErection, _T("D")))
//																Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor[d], LoadCaseNo, LcomD, TRUE);
//															else if (CheckCSEL(nErection, _T("ER")))
//																Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
//															else if (CheckCSEL(nErection, _T("LIP")))
//																Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
//															else if (CheckCSEL(nErection, _T("L")))
//																Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
//
//															else if (CheckCSEL(nErection, _T("T")))
//																Set_CStageCase(arCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
//															else if (CheckCSEL(nErection, _T("TPG")))
//																Set_CStageCase(arCSLoad.GetAt(nnn), dTPGFact1, LoadCaseNo, LcomD, TRUE);
//
//														}
//														if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
//															Set_CStageCase(arCSLoad.GetAt(nnn), dCSTPFact, LoadCaseNo, LcomD, TRUE);
//														if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
//															Set_CStageCase(arCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
//
//														// MQC:4539-JHYUN-20120316
//														if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep Primary
//														{
//															Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
//														}
//														if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep secondary
//														{
//															Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
//														}
//														if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage Primary
//														{
//															Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
//														}
//														if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage secondary
//														{
//															Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
//														}
//
//													}
//
//													double dRS_X = (m % 2 == 0) ? dRsFactor : -1. * dRsFactor;
//													double dRS_Y = (n % 2 == 0) ? dRsFactor : -1. * dRsFactor;
//													double dRS_Z = (o % 2 == 0) ? dRsFactor : -1. * dRsFactor;
//													if (m % 2 == 0 || m % 2 == 1)
//													{
//														dRS_Y = 0.3 * dRS_Y;
//														dRS_Z = 0.3 * dRS_Z;
//													}
//													else if (n % 2 == 0)
//													{
//														dRS_X = 0.3 * dRS_X;
//														dRS_Z = 0.3 * dRS_Z;
//													}
//													else if (o % 2 == 0)
//													{
//														dRS_X = 0.3 * dRS_X;
//														dRS_Y = 0.3 * dRS_Y;
//
//													}
//													else if (m % 2 == 1 && n % 2 == 1 && o % 2 == 1)
//													{
//														dRS_Y = 0.3 * dRS_Y;
//														dRS_Z = 0.3 * dRS_Z;
//													}
//
//													for (ld = 0; ld < m_nECount; ++ld)
//													{
//														if (m_nECount == 1)
//														{
//															nES += Set_STLCIncludeCS(arELoad[ld], dEFactor_x, LoadCaseNo, LcomD, 0);
//														}
//														else
//														{
//															if (ld == 0)            nES += Set_STLCIncludeCS(arELoad_x[ld], dRS_X, LoadCaseNo, LcomD, 0);
//															else if (ld % 2 == 1)     nES += Set_STLCIncludeCS(arELoad_y[ld], dRS_Y, LoadCaseNo, LcomD, 0);
//															else if (ld % 2 == 0)     nES += Set_STLCIncludeCS(arELoad_z[ld], dRS_Z, LoadCaseNo, LcomD, 0);
//
//														}
//
//													}
//
//													for (ld = 0; ld < arELoad_x.GetCount(); ++ld)    nES += Set_STLCIncludeCS(arELoad_x[ld], dRS_X, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < arELoad_y.GetCount(); ++ld)        nES += Set_STLCIncludeCS(arELoad_y[ld], dRS_Y, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < arELoad_z.GetCount(); ++ld)        nES += Set_STLCIncludeCS(arELoad_z[ld], dRS_Z, LoadCaseNo, LcomD, 0);
//
//																										
//													if (arEspLoad_X.GetSize() > 0) Set_SpectrumCase(arEspLoad_X[m / 2], dRS_X, LoadCaseNo, LcomD, TRUE);
//													if (arEspLoad_Y.GetSize() > 0) Set_SpectrumCase(arEspLoad_Y[n / 2], dRS_Y, LoadCaseNo, LcomD, TRUE);
//													if (arEspLoad_Z.GetSize() > 0) Set_SpectrumCase(arEspLoad_Z[o / 2], dRS_Z, LoadCaseNo, LcomD, TRUE);
//
//
//													// description
//													LcomD.Description.Empty();
//													csPrefix.Empty();
//													LcomD.Description += _T("3U:");
//													if (nDL > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("D");
//														csPrefix = _T(":");
//													}
//													if (nDc > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("DC");
//														csPrefix = _T("+");
//													}
//													if (nPS > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dPSFactor) + _T("PS");
//														csPrefix = _T(":");
//													}
//													if (nDw > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("DW");
//														csPrefix = _T("+");
//													}
//													if (nW > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dWFactor) + _T("WL");
//														csPrefix = _T("+");
//													}
//													if (nB > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("BL");
//														csPrefix = _T("+");
//													}
//													if (nES > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("ES");
//														csPrefix = _T("+");
//													}
//													if (nEP > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("EP");
//														csPrefix = _T("+");
//													}
//													if (nEH > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("EH");
//														csPrefix = _T("+");
//													}
//													if (nEV > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("EV");
//														csPrefix = _T("+");
//													}
//													if (m_nMCount > 0)
//													{
//														LcomD.Description += csPrefix + _T("ML"); csPrefix = _T(":");
//													}
//													if (nB > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dWPFactor) + _T("BL");
//														csPrefix = _T("+");
//													}
//													if (nWP > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dWPFactor) + _T("WP");
//														csPrefix = _T("+");
//													}
//													if (nFP > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dWPFactor) + _T("FP");
//														csPrefix = _T("+");
//													}
//													if (nLL > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("LL");
//														csPrefix = _T("+");
//													}
//													if (nLS > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("LS");
//														csPrefix = _T("+");
//													}
//													if (LoadCaseNo > 0) Set_LoadComb4Civil(LcomD, D_STR);
//													if (nCRL > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("CRL");
//														csPrefix = _T("+");
//													}
//													if (m_nConstLoad != 0) // ST+CS
//													{
//
//
//														CString strcEL;
//														int nErection = 0;
//
//														for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
//														{
//															strcEL = _T(""); nErection = 0;
//
//															if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn))
//															{
//																LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCP)"); csPrefix = _T("+");
//															}
//															if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn))
//															{
//																LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCS)"); csPrefix = _T("+");
//															}
//															if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn))
//															{
//																LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSP)"); csPrefix = _T("+");
//															}
//															if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn))
//															{
//																LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSS)"); csPrefix = _T("+");
//															}
//															if (CheckCSEL(nErection, _T("ER")))
//															{
//																LcomD.Description += csPrefix + GetFactorString(dCSFactor) + +_T("(cER)") + strcEL; csPrefix = _T("+");
//															}
//															if (CheckCSEL(nErection, _T("LIP")))
//															{
//																LcomD.Description += csPrefix + GetFactorString(dCSFactor) + _T("(cLIP)") + strcEL; csPrefix = _T("+");
//															}
//															if (CheckCSEL(nErection, _T("LL")))
//															{
//																LcomD.Description += csPrefix + GetFactorString(dCSFactor) + _T("(cLL)") + strcEL; csPrefix = _T("+");
//															}
//														}
//													}
//													if (arEspLoad_X.GetSize() > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dRS_X) + _T("RS(X)");
//														csPrefix = _T(":");
//													}
//													if (arEspLoad_Y.GetSize() > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dRS_Y) + _T("RS(Y)");
//														csPrefix = _T(":");
//													}
//													if (arEspLoad_Z.GetSize() > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dRS_Z) + _T("RS(Z)");
//														csPrefix = _T(":");
//													}
//
//												}
//											}
//
//										}
//									}
//								}
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//	else if (eql % 2 == 1)
//	{
//		////////////// Y Major //////////////// 
//		for (int d = 0; d < nDCount; ++d)
//		{
//
//
//			for (int p = 0; p < nPSCount; ++p) {
//				if (p % 2 == 0) dPSFactor = 1.15;
//				else if (p % 2 == 1) dPSFactor = 0.87;//PS Relieving
//
//				for (int w = 0; w < nWPCount; ++w) {
//					if (w % 3 == 0) dWPFactor = m_nConstLoad == 1 ? 0 : 1.05;
//					else if (w % 3 == 1) dWPFactor = m_nConstLoad == 1 ? 0.0 : 1.0;
//					else if (w % 3 == 2) dWPFactor = m_nConstLoad == 1 ? 0.0 : 0.0;
//
//					for (int wl = 0; wl < nWLCount; ++wl) {
//						if (wl % 3 == 0) dWFactor = m_nConstLoad == 1 ? 0 : 1.1;
//						else if (wl % 3 == 1) dWFactor = m_nConstLoad == 1 ? 0.0 : 1.0;
//						else if (wl % 3 == 2) dWFactor = m_nConstLoad == 1 ? 0.0 : 0.0;
//						if (p % 2 == 1) dWFactor = dWFactor * -1.;
//
//
//						for (int e = 0; e < nEPCount; ++e) {
//							if (e % 3 == 0) dEsFact = m_nConstLoad == 1 ? 0. : 1.3;
//							else if (e % 3 == 1) dEsFact = m_nConstLoad == 1 ? 0. : 0.85;//EP Relieving
//							else if (e % 3 == 2) dEsFact = m_nConstLoad == 1 ? 0. : 0.0;//EP Relieving
//
//							for (int k = 0; k < nSmCount; ++k) {
//								for (int j = 0; j < nMLCount; ++j) {
//									for (int k1 = 0; k1 < nCSTPCOunt; ++k1) {
//										if (k1 % 3 == 0) dCSTPFact = m_nConstLoad == 1 ? 0 : 1.15;
//										else if (k1 % 3 == 1) dCSTPFact = m_nConstLoad == 1 ? 0.0 : 0.87;
//										else if (k1 % 3 == 2) dCSTPFact = m_nConstLoad == 1 ? 0.0 : 0.0;
//
//
//
//										for (int m = 0; m < nESPCount_X; m++) {
//											for (int n = 0; n < nESPCount_Y; n++) {
//												for (int o = 0; o < nESPCount_Z; o++) {
//
//													int nDL = 0, nDc = 0, nSnow = 0, nDw = 0, nPL = 0, nEP = 0, nEV = 0, nES = 0, nEH = 0;
//													int nLL = 0, nIL = 0, nLS = 0, nLP = 0, nILP = 0, nCF = 0, nBRK = 0, nBK = 0, nCRL = 0;
//													int nB = 0, nWL = 0, nWLL = 0, nTL = 0, nSL = 0, nER = 0, nWP = 0, nFP = 0, nPS = 0, nAS = 0;
//													int nW = 0, nWl = 0, nCO = 0, nSf = 0, nWpr = 0, nStl = 0, nT = 0, nTpg = 0;
//
//													LcomD.Initialize();
//													LcomD.nActive = 1;
//													LoadCaseNo = 0;
//
//													//Permanent
//													for (ld = 0; ld < m_nDCount; ++ld) nDL += Set_STLCIncludeCS(arDLoad[ld], dDFactor[d], LoadCaseNo, LcomD, D_SGLD_DL_CVL);
//													for (ld = 0; ld < m_nDcCount; ++ld) nDc += Set_STLCIncludeCS(arDcLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nDwCount; ++ld) nDw += Set_STLCIncludeCS(arDwLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
//
//													for (ld = 0; ld < m_nCSADCount; ++ld) nDL += Set_STLCIncludeCS(arCSADLoad[ld], dDFactor[d], LoadCaseNo, LcomD, D_SGLD_DL_CVL);
//													for (ld = 0; ld < m_nCSADcCount; ++ld) nDc += Set_STLCIncludeCS(arCSADcLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nCSADwCount; ++ld) nDw += Set_STLCIncludeCS(arCSADwLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
//
//													for (ld = 0; ld < m_nPLCount; ++ld) nPL += Set_STLCIncludeCS(arPLLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
//
//													////Earth Pressure
//													for (ld = 0; ld < m_nEsCount; ++ld) nES += Set_STLCIncludeCS(arEsLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nEpCount; ++ld) nEP += Set_STLCIncludeCS(arEpLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
//													////for (ld = 0; ld < m_nEhCount; ++ld) nEH += Set_STLCIncludeCS(arEhLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nEvCount; ++ld) nEV += Set_STLCIncludeCS(arEvLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
//
//													// for(ld=0; ld<m_nLsCount; ++ld) nLS += Set_STLCIncludeCS(arLsLoad[ld], dLsFactor, LoadCaseNo, LcomD, 0);
//														   //if(m_nSmCount > 0) Set_SettleCase(arSmLoad[nSmIndex], dSmFactor, LoadCaseNo, LcomD, TRUE);
//													//Water Pressure
//													//for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nWpCount; ld++) nWP += Set_STLCIncludeCS(arWpLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nFpCount; ld++) nFP += Set_STLCIncludeCS(arFpLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
//													//Wind Load and Wind On Live
//													for (ld = 0; ld < m_nWCount; ++ld) nW += Set_STLCIncludeCS(arWLoad[ld], dWFactor, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nWlCount; ld++) nWl += Set_STLCIncludeCS(arWlLoad[ld], dWFactor, LoadCaseNo, LcomD, 0);
//
//													////Vehicle load and Surcharge
//													for (ld = 0; ld < m_nLCount; ++ld) nLL += Set_STLCIncludeCS(arLLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nLsCount; ++ld)  nLS += Set_STLCIncludeCS(arLsLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
//
//													//if (m_nMCount > 0)  Set_MovingCase(arMLoad.GetAt(j), 1.0, LoadCaseNo, LcomD, TRUE);
//													//for (ld = 0; ld < m_nCrlCount; ++ld) nCRL += Set_STLCIncludeCS(arCrlLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
//
//													for (ld = 0; ld < m_nPsCount; ++ld) nPS += Set_STLCIncludeCS(arPsLoad[ld], dPSFactor, LoadCaseNo, LcomD, D_SGLD_TS_CVL);
//													//if (m_nSmCount > 0) Set_SettleCase(arSmLoad.GetAt(k), dCpShFact, LoadCaseNo, LcomD, TRUE);
//													for (ld = 0; ld < m_nCfCount; ++ld) nCF += Set_STLCIncludeCS(arCfLoad[ld], dCFForce, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nASCount; ++ld) nAS += Set_STLCIncludeCS(arASLoad[ld], dCFForce, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nCoCount; ++ld) nCO += Set_STLCIncludeCS(arCoLoad[ld], dCOForce, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nSfCount; ++ld) nSf += Set_STLCIncludeCS(arSfLoad[ld], dSfForce, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nWprCount; ++ld) nWpr += Set_STLCIncludeCS(arWprLoad[ld], dSfForce, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nStlCount; ++ld) nStl += Set_STLCIncludeCS(arStlLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nTCount; ++ld) nT += Set_STLCIncludeCS(arTLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < m_nTpgCount; ++ld) nTpg += Set_STLCIncludeCS(arTpgLoad[ld], dTPGFact1, LoadCaseNo, LcomD, 0);
//
//													if (m_nConstLoad != 0) // ST+CS
//													{
//														if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
//															Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor[d], LoadCaseNo, LcomD, TRUE);
//
//														CString strcEL;
//														int nErection = 0;
//														for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
//														{
//															strcEL = _T(""); nErection = 0;
//															if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }
//
//
//
//															if (CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")) || CheckCSEL(nErection, _T("PL")) || CheckCSEL(nErection, _T("BL")))
//																Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor[d], LoadCaseNo, LcomD, TRUE);
//															else if (CheckCSEL(nErection, _T("D")))
//																Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor[d], LoadCaseNo, LcomD, TRUE);
//															else if (CheckCSEL(nErection, _T("ER")))
//																Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
//															else if (CheckCSEL(nErection, _T("LIP")))
//																Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
//															else if (CheckCSEL(nErection, _T("L")))
//																Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
//
//															else if (CheckCSEL(nErection, _T("T")))
//																Set_CStageCase(arCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
//															else if (CheckCSEL(nErection, _T("TPG")))
//																Set_CStageCase(arCSLoad.GetAt(nnn), dTPGFact1, LoadCaseNo, LcomD, TRUE);
//
//														}
//														if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
//															Set_CStageCase(arCSLoad.GetAt(nnn), dCSTPFact, LoadCaseNo, LcomD, TRUE);
//														if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
//															Set_CStageCase(arCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
//
//														// MQC:4539-JHYUN-20120316
//														if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep Primary
//														{
//															Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
//														}
//														if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep secondary
//														{
//															Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
//														}
//														if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage Primary
//														{
//															Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
//														}
//														if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage secondary
//														{
//															Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
//														}
//
//													}
//
//													double dRS_X = (m % 2 == 0) ? dRsFactor : -1. * dRsFactor;
//													double dRS_Y = (n % 2 == 0) ? dRsFactor : -1. * dRsFactor;
//													double dRS_Z = (o % 2 == 0) ? dRsFactor : -1. * dRsFactor;
//													if (m % 2 == 0 || m % 2 == 1)
//													{
//														dRS_Y = 0.3 * dRS_Y;
//														dRS_Z = 0.3 * dRS_Z;
//													}
//													else if (n % 2 == 0)
//													{
//														dRS_X = 0.3 * dRS_X;
//														dRS_Z = 0.3 * dRS_Z;
//													}
//													else if (o % 2 == 0)
//													{
//														dRS_X = 0.3 * dRS_X;
//														dRS_Y = 0.3 * dRS_Y;
//
//													}
//													else if (m % 2 == 1 && n % 2 == 1 && o % 2 == 1)
//													{
//														dRS_Y = 0.3 * dRS_Y;
//														dRS_Z = 0.3 * dRS_Z;
//													}
//
//													for (ld = 0; ld < m_nECount; ++ld)
//													{
//														if (m_nECount == 1)
//														{
//															nES += Set_STLCIncludeCS(arELoad[ld], dEFactor_x, LoadCaseNo, LcomD, 0);
//														}
//														else
//														{
//															if (ld == 0)            nES += Set_STLCIncludeCS(arELoad_x[ld], dRS_Y, LoadCaseNo, LcomD, 0);
//															else if (ld % 2 == 1)     nES += Set_STLCIncludeCS(arELoad_y[ld], dRS_X, LoadCaseNo, LcomD, 0);
//															else if (ld % 2 == 0)     nES += Set_STLCIncludeCS(arELoad_z[ld], dRS_Z, LoadCaseNo, LcomD, 0);
//
//														}
//
//													}
//
//													for (ld = 0; ld < arELoad_x.GetCount(); ++ld)    nES += Set_STLCIncludeCS(arELoad_x[ld], dRS_Y, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < arELoad_y.GetCount(); ++ld)        nES += Set_STLCIncludeCS(arELoad_y[ld], dRS_X, LoadCaseNo, LcomD, 0);
//													for (ld = 0; ld < arELoad_z.GetCount(); ++ld)        nES += Set_STLCIncludeCS(arELoad_z[ld], dRS_Z, LoadCaseNo, LcomD, 0);
//
//
//													if (arEspLoad_X.GetSize() > 0) Set_SpectrumCase(arEspLoad_X[m / 2], dRS_Y, LoadCaseNo, LcomD, TRUE);
//													if (arEspLoad_Y.GetSize() > 0) Set_SpectrumCase(arEspLoad_Y[n / 2], dRS_X, LoadCaseNo, LcomD, TRUE);
//													if (arEspLoad_Z.GetSize() > 0) Set_SpectrumCase(arEspLoad_Z[o / 2], dRS_Z, LoadCaseNo, LcomD, TRUE);
//
//
//													// description
//													LcomD.Description.Empty();
//													csPrefix.Empty();
//													LcomD.Description += _T("3U:");
//													if (nDL > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("D");
//														csPrefix = _T(":");
//													}
//													if (nDc > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("DC");
//														csPrefix = _T("+");
//													}
//													if (nPS > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dPSFactor) + _T("PS");
//														csPrefix = _T(":");
//													}
//													if (nDw > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("DW");
//														csPrefix = _T("+");
//													}
//													if (nW > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dWFactor) + _T("WL");
//														csPrefix = _T("+");
//													}
//													if (nB > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("BL");
//														csPrefix = _T("+");
//													}
//													if (nES > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("ES");
//														csPrefix = _T("+");
//													}
//													if (nEP > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("EP");
//														csPrefix = _T("+");
//													}
//													if (nEH > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("EH");
//														csPrefix = _T("+");
//													}
//													if (nEV > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("EV");
//														csPrefix = _T("+");
//													}
//													if (m_nMCount > 0)
//													{
//														LcomD.Description += csPrefix + _T("ML"); csPrefix = _T(":");
//													}
//													if (nB > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dWPFactor) + _T("BL");
//														csPrefix = _T("+");
//													}
//													if (nWP > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dWPFactor) + _T("WP");
//														csPrefix = _T("+");
//													}
//													if (nFP > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dWPFactor) + _T("FP");
//														csPrefix = _T("+");
//													}
//													if (nLL > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("LL");
//														csPrefix = _T("+");
//													}
//													if (nLS > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("LS");
//														csPrefix = _T("+");
//													}
//													if (LoadCaseNo > 0) Set_LoadComb4Civil(LcomD, D_STR);
//													if (nCRL > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("CRL");
//														csPrefix = _T("+");
//													}
//													if (m_nConstLoad != 0) // ST+CS
//													{
//
//
//														CString strcEL;
//														int nErection = 0;
//
//														for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
//														{
//															strcEL = _T(""); nErection = 0;
//
//															if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn))
//															{
//																LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCP)"); csPrefix = _T("+");
//															}
//															if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn))
//															{
//																LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCS)"); csPrefix = _T("+");
//															}
//															if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn))
//															{
//																LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSP)"); csPrefix = _T("+");
//															}
//															if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn))
//															{
//																LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSS)"); csPrefix = _T("+");
//															}
//															if (CheckCSEL(nErection, _T("ER")))
//															{
//																LcomD.Description += csPrefix + GetFactorString(dCSFactor) + +_T("(cER)") + strcEL; csPrefix = _T("+");
//															}
//															if (CheckCSEL(nErection, _T("LIP")))
//															{
//																LcomD.Description += csPrefix + GetFactorString(dCSFactor) + _T("(cLIP)") + strcEL; csPrefix = _T("+");
//															}
//															if (CheckCSEL(nErection, _T("LL")))
//															{
//																LcomD.Description += csPrefix + GetFactorString(dCSFactor) + _T("(cLL)") + strcEL; csPrefix = _T("+");
//															}
//														}
//													}
//													if (arEspLoad_X.GetSize() > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dRS_X) + _T("RS(X)");
//														csPrefix = _T(":");
//													}
//													if (arEspLoad_Y.GetSize() > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dRS_Y) + _T("RS(Y)");
//														csPrefix = _T(":");
//													}
//													if (arEspLoad_Z.GetSize() > 0)
//													{
//														LcomD.Description += csPrefix + GetFactorString(dRS_Z) + _T("RS(Z)");
//														csPrefix = _T(":");
//													}
//
//												}
//											}
//
//										}
//									}
//								}
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//	
//	else if (eql % 2 == 0)
//	{
//	////////////// Z Major //////////////// 
//	for (int d = 0; d < nDCount; ++d)
//	{
//
//
//		for (int p = 0; p < nPSCount; ++p) {
//			if (p % 2 == 0) dPSFactor = 1.15;
//			else if (p % 2 == 1) dPSFactor = 0.87;//PS Relieving
//
//			for (int w = 0; w < nWPCount; ++w) {
//				if (w % 3 == 0) dWPFactor = m_nConstLoad == 1 ? 0 : 1.05;
//				else if (w % 3 == 1) dWPFactor = m_nConstLoad == 1 ? 0.0 : 1.0;
//				else if (w % 3 == 2) dWPFactor = m_nConstLoad == 1 ? 0.0 : 0.0;
//
//				for (int wl = 0; wl < nWLCount; ++wl) {
//					if (wl % 3 == 0) dWFactor = m_nConstLoad == 1 ? 0 : 1.1;
//					else if (wl % 3 == 1) dWFactor = m_nConstLoad == 1 ? 0.0 : 1.0;
//					else if (wl % 3 == 2) dWFactor = m_nConstLoad == 1 ? 0.0 : 0.0;
//					if (p % 2 == 1) dWFactor = dWFactor * -1.;
//
//
//					for (int e = 0; e < nEPCount; ++e) {
//						if (e % 3 == 0) dEsFact = m_nConstLoad == 1 ? 0. : 1.3;
//						else if (e % 3 == 1) dEsFact = m_nConstLoad == 1 ? 0. : 0.85;//EP Relieving
//						else if (e % 3 == 2) dEsFact = m_nConstLoad == 1 ? 0. : 0.0;//EP Relieving
//
//						for (int k = 0; k < nSmCount; ++k) {
//							for (int j = 0; j < nMLCount; ++j) {
//								for (int k1 = 0; k1 < nCSTPCOunt; ++k1) {
//									if (k1 % 3 == 0) dCSTPFact = m_nConstLoad == 1 ? 0 : 1.15;
//									else if (k1 % 3 == 1) dCSTPFact = m_nConstLoad == 1 ? 0.0 : 0.87;
//									else if (k1 % 3 == 2) dCSTPFact = m_nConstLoad == 1 ? 0.0 : 0.0;
//
//
//
//									for (int m = 0; m < nESPCount_X; m++) {
//										for (int n = 0; n < nESPCount_Y; n++) {
//											for (int o = 0; o < nESPCount_Z; o++) {
//
//												int nDL = 0, nDc = 0, nSnow = 0, nDw = 0, nPL = 0, nEP = 0, nEV = 0, nES = 0, nEH = 0;
//												int nLL = 0, nIL = 0, nLS = 0, nLP = 0, nILP = 0, nCF = 0, nBRK = 0, nBK = 0, nCRL = 0;
//												int nB = 0, nWL = 0, nWLL = 0, nTL = 0, nSL = 0, nER = 0, nWP = 0, nFP = 0, nPS = 0, nAS = 0;
//												int nW = 0, nWl = 0, nCO = 0, nSf = 0, nWpr = 0, nStl = 0, nT = 0, nTpg = 0;
//
//												LcomD.Initialize();
//												LcomD.nActive = 1;
//												LoadCaseNo = 0;
//
//												//Permanent
//												for (ld = 0; ld < m_nDCount; ++ld) nDL += Set_STLCIncludeCS(arDLoad[ld], dDFactor[d], LoadCaseNo, LcomD, D_SGLD_DL_CVL);
//												for (ld = 0; ld < m_nDcCount; ++ld) nDc += Set_STLCIncludeCS(arDcLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
//												for (ld = 0; ld < m_nDwCount; ++ld) nDw += Set_STLCIncludeCS(arDwLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
//
//												for (ld = 0; ld < m_nCSADCount; ++ld) nDL += Set_STLCIncludeCS(arCSADLoad[ld], dDFactor[d], LoadCaseNo, LcomD, D_SGLD_DL_CVL);
//												for (ld = 0; ld < m_nCSADcCount; ++ld) nDc += Set_STLCIncludeCS(arCSADcLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
//												for (ld = 0; ld < m_nCSADwCount; ++ld) nDw += Set_STLCIncludeCS(arCSADwLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
//
//												for (ld = 0; ld < m_nPLCount; ++ld) nPL += Set_STLCIncludeCS(arPLLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
//												for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
//
//												////Earth Pressure
//												for (ld = 0; ld < m_nEsCount; ++ld) nES += Set_STLCIncludeCS(arEsLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
//												for (ld = 0; ld < m_nEpCount; ++ld) nEP += Set_STLCIncludeCS(arEpLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
//												////for (ld = 0; ld < m_nEhCount; ++ld) nEH += Set_STLCIncludeCS(arEhLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
//												for (ld = 0; ld < m_nEvCount; ++ld) nEV += Set_STLCIncludeCS(arEvLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
//
//												// for(ld=0; ld<m_nLsCount; ++ld) nLS += Set_STLCIncludeCS(arLsLoad[ld], dLsFactor, LoadCaseNo, LcomD, 0);
//													   //if(m_nSmCount > 0) Set_SettleCase(arSmLoad[nSmIndex], dSmFactor, LoadCaseNo, LcomD, TRUE);
//												//Water Pressure
//												//for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
//												for (ld = 0; ld < m_nWpCount; ld++) nWP += Set_STLCIncludeCS(arWpLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
//												for (ld = 0; ld < m_nFpCount; ld++) nFP += Set_STLCIncludeCS(arFpLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
//												//Wind Load and Wind On Live
//												for (ld = 0; ld < m_nWCount; ++ld) nW += Set_STLCIncludeCS(arWLoad[ld], dWFactor, LoadCaseNo, LcomD, 0);
//												for (ld = 0; ld < m_nWlCount; ld++) nWl += Set_STLCIncludeCS(arWlLoad[ld], dWFactor, LoadCaseNo, LcomD, 0);
//
//												////Vehicle load and Surcharge
//												for (ld = 0; ld < m_nLCount; ++ld) nLL += Set_STLCIncludeCS(arLLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
//												for (ld = 0; ld < m_nLsCount; ++ld)  nLS += Set_STLCIncludeCS(arLsLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
//
//												//if (m_nMCount > 0)  Set_MovingCase(arMLoad.GetAt(j), 1.0, LoadCaseNo, LcomD, TRUE);
//												//for (ld = 0; ld < m_nCrlCount; ++ld) nCRL += Set_STLCIncludeCS(arCrlLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
//
//												for (ld = 0; ld < m_nPsCount; ++ld) nPS += Set_STLCIncludeCS(arPsLoad[ld], dPSFactor, LoadCaseNo, LcomD, D_SGLD_TS_CVL);
//												//if (m_nSmCount > 0) Set_SettleCase(arSmLoad.GetAt(k), dCpShFact, LoadCaseNo, LcomD, TRUE);
//												for (ld = 0; ld < m_nCfCount; ++ld) nCF += Set_STLCIncludeCS(arCfLoad[ld], dCFForce, LoadCaseNo, LcomD, 0);
//												for (ld = 0; ld < m_nASCount; ++ld) nAS += Set_STLCIncludeCS(arASLoad[ld], dCFForce, LoadCaseNo, LcomD, 0);
//												for (ld = 0; ld < m_nCoCount; ++ld) nCO += Set_STLCIncludeCS(arCoLoad[ld], dCOForce, LoadCaseNo, LcomD, 0);
//												for (ld = 0; ld < m_nSfCount; ++ld) nSf += Set_STLCIncludeCS(arSfLoad[ld], dSfForce, LoadCaseNo, LcomD, 0);
//												for (ld = 0; ld < m_nWprCount; ++ld) nWpr += Set_STLCIncludeCS(arWprLoad[ld], dSfForce, LoadCaseNo, LcomD, 0);
//												for (ld = 0; ld < m_nStlCount; ++ld) nStl += Set_STLCIncludeCS(arStlLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
//												for (ld = 0; ld < m_nTCount; ++ld) nT += Set_STLCIncludeCS(arTLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
//												for (ld = 0; ld < m_nTpgCount; ++ld) nTpg += Set_STLCIncludeCS(arTpgLoad[ld], dTPGFact1, LoadCaseNo, LcomD, 0);
//
//												if (m_nConstLoad != 0) // ST+CS
//												{
//													if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
//														Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor[d], LoadCaseNo, LcomD, TRUE);
//
//													CString strcEL;
//													int nErection = 0;
//													for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
//													{
//														strcEL = _T(""); nErection = 0;
//														if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }
//
//
//
//														if (CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")) || CheckCSEL(nErection, _T("PL")) || CheckCSEL(nErection, _T("BL")))
//															Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor[d], LoadCaseNo, LcomD, TRUE);
//														else if (CheckCSEL(nErection, _T("D")))
//															Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor[d], LoadCaseNo, LcomD, TRUE);
//														else if (CheckCSEL(nErection, _T("ER")))
//															Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
//														else if (CheckCSEL(nErection, _T("LIP")))
//															Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
//														else if (CheckCSEL(nErection, _T("L")))
//															Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
//
//														else if (CheckCSEL(nErection, _T("T")))
//															Set_CStageCase(arCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
//														else if (CheckCSEL(nErection, _T("TPG")))
//															Set_CStageCase(arCSLoad.GetAt(nnn), dTPGFact1, LoadCaseNo, LcomD, TRUE);
//
//													}
//													if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
//														Set_CStageCase(arCSLoad.GetAt(nnn), dCSTPFact, LoadCaseNo, LcomD, TRUE);
//													if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
//														Set_CStageCase(arCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
//
//													// MQC:4539-JHYUN-20120316
//													if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep Primary
//													{
//														Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
//													}
//													if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep secondary
//													{
//														Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
//													}
//													if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage Primary
//													{
//														Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
//													}
//													if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage secondary
//													{
//														Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
//													}
//
//												}
//
//												double dRS_X = (m % 2 == 0) ? dRsFactor : -1. * dRsFactor;
//												double dRS_Y = (n % 2 == 0) ? dRsFactor : -1. * dRsFactor;
//												double dRS_Z = (o % 2 == 0) ? dRsFactor : -1. * dRsFactor;
//												if (m % 2 == 0 || m % 2 == 1)
//												{
//													dRS_Y = 0.3 * dRS_Y;
//													dRS_Z = 0.3 * dRS_Z;
//												}
//												else if (n % 2 == 0)
//												{
//													dRS_X = 0.3 * dRS_X;
//													dRS_Z = 0.3 * dRS_Z;
//												}
//												else if (o % 2 == 0)
//												{
//													dRS_X = 0.3 * dRS_X;
//													dRS_Y = 0.3 * dRS_Y;
//
//												}
//												else if (m % 2 == 1 && n % 2 == 1 && o % 2 == 1)
//												{
//													dRS_Y = 0.3 * dRS_Y;
//													dRS_Z = 0.3 * dRS_Z;
//												}
//
//												for (ld = 0; ld < m_nECount; ++ld)
//												{
//													if (m_nECount == 1)
//													{
//														nES += Set_STLCIncludeCS(arELoad[ld], dEFactor_x, LoadCaseNo, LcomD, 0);
//													}
//													else
//													{
//														if (ld == 0)            nES += Set_STLCIncludeCS(arELoad_x[ld], dRS_Z, LoadCaseNo, LcomD, 0);
//														else if (ld % 2 == 1)     nES += Set_STLCIncludeCS(arELoad_y[ld], dRS_Y, LoadCaseNo, LcomD, 0);
//														else if (ld % 2 == 0)     nES += Set_STLCIncludeCS(arELoad_z[ld], dRS_Z, LoadCaseNo, LcomD, 0);
//
//													}
//
//												}
//
//												for (ld = 0; ld < arELoad_x.GetCount(); ++ld)    nES += Set_STLCIncludeCS(arELoad_x[ld], dRS_Z, LoadCaseNo, LcomD, 0);
//												for (ld = 0; ld < arELoad_y.GetCount(); ++ld)        nES += Set_STLCIncludeCS(arELoad_y[ld], dRS_Y, LoadCaseNo, LcomD, 0);
//												for (ld = 0; ld < arELoad_z.GetCount(); ++ld)        nES += Set_STLCIncludeCS(arELoad_z[ld], dRS_Z, LoadCaseNo, LcomD, 0);
//
//
//												if (arEspLoad_X.GetSize() > 0) Set_SpectrumCase(arEspLoad_X[m / 2], dRS_Z, LoadCaseNo, LcomD, TRUE);
//												if (arEspLoad_Y.GetSize() > 0) Set_SpectrumCase(arEspLoad_Y[n / 2], dRS_Y, LoadCaseNo, LcomD, TRUE);
//												if (arEspLoad_Z.GetSize() > 0) Set_SpectrumCase(arEspLoad_Z[o / 2], dRS_X, LoadCaseNo, LcomD, TRUE);
//
//
//												// description
//												LcomD.Description.Empty();
//												csPrefix.Empty();
//												LcomD.Description += _T("3U:");
//												if (nDL > 0)
//												{
//													LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("D");
//													csPrefix = _T(":");
//												}
//												if (nDc > 0)
//												{
//													LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("DC");
//													csPrefix = _T("+");
//												}
//												if (nPS > 0)
//												{
//													LcomD.Description += csPrefix + GetFactorString(dPSFactor) + _T("PS");
//													csPrefix = _T(":");
//												}
//												if (nDw > 0)
//												{
//													LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("DW");
//													csPrefix = _T("+");
//												}
//												if (nW > 0)
//												{
//													LcomD.Description += csPrefix + GetFactorString(dWFactor) + _T("WL");
//													csPrefix = _T("+");
//												}
//												if (nB > 0)
//												{
//													LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("BL");
//													csPrefix = _T("+");
//												}
//												if (nES > 0)
//												{
//													LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("ES");
//													csPrefix = _T("+");
//												}
//												if (nEP > 0)
//												{
//													LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("EP");
//													csPrefix = _T("+");
//												}
//												if (nEH > 0)
//												{
//													LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("EH");
//													csPrefix = _T("+");
//												}
//												if (nEV > 0)
//												{
//													LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("EV");
//													csPrefix = _T("+");
//												}
//												if (m_nMCount > 0)
//												{
//													LcomD.Description += csPrefix + _T("ML"); csPrefix = _T(":");
//												}
//												if (nB > 0)
//												{
//													LcomD.Description += csPrefix + GetFactorString(dWPFactor) + _T("BL");
//													csPrefix = _T("+");
//												}
//												if (nWP > 0)
//												{
//													LcomD.Description += csPrefix + GetFactorString(dWPFactor) + _T("WP");
//													csPrefix = _T("+");
//												}
//												if (nFP > 0)
//												{
//													LcomD.Description += csPrefix + GetFactorString(dWPFactor) + _T("FP");
//													csPrefix = _T("+");
//												}
//												if (nLL > 0)
//												{
//													LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("LL");
//													csPrefix = _T("+");
//												}
//												if (nLS > 0)
//												{
//													LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("LS");
//													csPrefix = _T("+");
//												}
//												if (LoadCaseNo > 0) Set_LoadComb4Civil(LcomD, D_STR);
//												if (nCRL > 0)
//												{
//													LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("CRL");
//													csPrefix = _T("+");
//												}
//												if (m_nConstLoad != 0) // ST+CS
//												{
//
//
//													CString strcEL;
//													int nErection = 0;
//
//													for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
//													{
//														strcEL = _T(""); nErection = 0;
//
//														if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn))
//														{
//															LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCP)"); csPrefix = _T("+");
//														}
//														if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn))
//														{
//															LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCS)"); csPrefix = _T("+");
//														}
//														if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn))
//														{
//															LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSP)"); csPrefix = _T("+");
//														}
//														if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn))
//														{
//															LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSS)"); csPrefix = _T("+");
//														}
//														if (CheckCSEL(nErection, _T("ER")))
//														{
//															LcomD.Description += csPrefix + GetFactorString(dCSFactor) + +_T("(cER)") + strcEL; csPrefix = _T("+");
//														}
//														if (CheckCSEL(nErection, _T("LIP")))
//														{
//															LcomD.Description += csPrefix + GetFactorString(dCSFactor) + _T("(cLIP)") + strcEL; csPrefix = _T("+");
//														}
//														if (CheckCSEL(nErection, _T("LL")))
//														{
//															LcomD.Description += csPrefix + GetFactorString(dCSFactor) + _T("(cLL)") + strcEL; csPrefix = _T("+");
//														}
//													}
//												}
//												if (arEspLoad_X.GetSize() > 0)
//												{
//													LcomD.Description += csPrefix + GetFactorString(dRS_X) + _T("RS(X)");
//													csPrefix = _T(":");
//												}
//												if (arEspLoad_Y.GetSize() > 0)
//												{
//													LcomD.Description += csPrefix + GetFactorString(dRS_Y) + _T("RS(Y)");
//													csPrefix = _T(":");
//												}
//												if (arEspLoad_Z.GetSize() > 0)
//												{
//													LcomD.Description += csPrefix + GetFactorString(dRS_Z) + _T("RS(Z)");
//													csPrefix = _T(":");
//												}
//
//											}
//										}
//
//									}
//								}
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//	}
//	//return TRUE;
//////////////////////////////////////////////////////////////////Above
////T_LCOM_D LcomD;
////
////int nDCount = 1;
////BOOL bExistD = (m_nDCount > 0 || m_nDwCount > 0 || m_nDcCount > 0);
////if (bExistD) nDCount = m_dUserDefDL > 2.20 ? 2 : 1;//For Both Case of Dead Load
////int nMLCount = max(m_nMCount, 1);
////int nWPCount = (m_nWpCount >= 1) ? 3 * m_nWpCount : 1;//Adding and Relieving Water Pressure
////int nEPCount = (m_nEpCount >= 1) ? 3 * m_nEpCount : 1;//Adding and Relieving Earth Pressure
////int nPSCount = (m_nPsCount >= 1) ? 2 * m_nPsCount : 1;//Adding and Relieving PreStressing
////int nWLCount = (m_nWCount >= 1) ? 3 * m_nWCount : 1;//Adding and Relieving Wind Load
////int nWLLCount = (m_nWlCount >= 1) ? 3 * m_nWlCount : 1;//Adding and Relieving Wind Load on Live Load
////m_dUserDefDL = m_nCodeBasedOrUserDef == 0 ? 1.2 : m_dUserDefDL;
////double dDFactor[2] = { 1.05,1.05 };//For BOth Case,
//////if (nDCount == 1) { dDFactor[0] = m_dUserDefDL, dDFactor[1] = m_dUserDefDL; }
////
////double dEsFact = m_nConstLoad == 1 ? 0. : 1.5;//m_nConstLoad = 0 - ST,m_nConstLoad = 1 - CS,m_nConstLoad = 2 - ST+CS
////double dWPFactor = 1.05;//Water Pressure Factor
////double dPSFactor = 0.;//{ 0.,0.87 };//Pre Stress Factor
////double dCpShFact = 1.;//Creep Shrinkage Factor
////double dCSFactor = 1.15;//CS Factor
////double dWFactor = 1.1;//Wind Factor 1.1 or 0
////double dTPGFact1 = 0.8;
////double dCFForce = 1.5;
////double dCOForce = 1.25;
////double dSfForce = 1.05;
////int nSmCount = 1;
////
////int nEsp_x = arEspLoad_X.GetSize();
////int nEsp_y = arEspLoad_Y.GetSize();
////int nEsp_z = arEspLoad_Z.GetSize();
////int nTtlESP = nEsp_x + nEsp_y + nEsp_z;
////
////double dRsFactor = 1.3;
////int nESPCount_X = (nEsp_x > 0) ? nEsp_x * 2 : 1;
////int nESPCount_Y = (nEsp_y > 0) ? nEsp_y * 2 : 1;
////int nESPCount_Z = (nEsp_z > 0) ? nEsp_z * 2 : 1;
////double dEFactor_x = dRsFactor, dEFactor_y = dRsFactor *0.3;
////
////CString csPrefix(_T("")), strFixM(_T("")), strFixW(_T("")), strFixSm(_T(""));
////
////int LoadCaseNo = 0, ld = 0, nnn = 0, nCSTPCOunt = 1;
////double dCSTPFact = 1.15;
//////CS Tendon Primary has 1.15 and 0
////if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn) > 0) nCSTPCOunt = 3;
////if (eql == 0)
////{
////	for (int d = 0; d < nDCount; ++d)
////	{
////		for (int p = 0; p < nPSCount; ++p) {
////			if (p % 2 == 0) dPSFactor = 1.15;
////			else if (p % 2 == 1) dPSFactor = 0.87;//PS Relieving
////
////			for (int w = 0; w < nWPCount; ++w) {
////				if (w % 3 == 0) dWPFactor = m_nConstLoad == 1 ? 0 : 1.05;
////				else if (w % 3 == 1) dWPFactor = m_nConstLoad == 1 ? 0.0 : 1.0;
////				else if (w % 3 == 2) dWPFactor = m_nConstLoad == 1 ? 0.0 : 0.0;
////
////				for (int wl = 0; wl < nWLCount; ++wl) {
////					if (wl % 3 == 0) dWFactor = m_nConstLoad == 1 ? 0 : 1.1;
////					else if (wl % 3 == 1) dWFactor = m_nConstLoad == 1 ? 0.0 : 1.0;
////					else if (wl % 3 == 2) dWFactor = m_nConstLoad == 1 ? 0.0 : 0.0;
////					if (p % 2 == 1) dWFactor = dWFactor * -1.;
////
////					for (int m = 0; m < nESPCount_X; m++) {
////						double dRS_X = (m % 2 == 0) ? dRsFactor : -1. * dRsFactor;
////						
////																		
////						for (int e = 0; e < nEPCount; ++e) {
////							if (e % 3 == 0) dEsFact = m_nConstLoad == 1 ? 0. : 1.3;
////							else if (e % 3 == 1) dEsFact = m_nConstLoad == 1 ? 0. : 0.85;//EP Relieving
////							else if (e % 3 == 2) dEsFact = m_nConstLoad == 1 ? 0. : 0.0;//EP Relieving
////
////							for (int k = 0; k < nSmCount; ++k) {
////								for (int j = 0; j < nMLCount; ++j) {
////									for (int k1 = 0; k1 < nCSTPCOunt; ++k1) {
////										if (k1 % 3 == 0) dCSTPFact = m_nConstLoad == 1 ? 0 : 1.15;
////										else if (k1 % 3 == 1) dCSTPFact = m_nConstLoad == 1 ? 0.0 : 0.87;
////										else if (k1 % 3 == 2) dCSTPFact = m_nConstLoad == 1 ? 0.0 : 0.0;
////
////										int nDL = 0, nDc = 0, nSnow = 0, nDw = 0, nPL = 0, nEP = 0, nEV = 0, nES = 0, nEH = 0;
////										int nLL = 0, nIL = 0, nLS = 0, nLP = 0, nILP = 0, nCF = 0, nBRK = 0, nBK = 0, nCRL = 0;
////										int nB = 0, nWL = 0, nWLL = 0, nTL = 0, nSL = 0, nER = 0, nWP = 0, nFP = 0, nPS = 0, nAS = 0;
////										int nW = 0, nWl = 0, nCO = 0, nSf = 0, nWpr = 0, nStl = 0, nT = 0, nTpg = 0;
////										LcomD.Initialize();
////										LcomD.nActive = 1;
////										LoadCaseNo = 0;
////
////										//Permanent
////										for (ld = 0; ld < m_nDCount; ++ld) nDL += Set_STLCIncludeCS(arDLoad[ld], dDFactor[d], LoadCaseNo, LcomD, D_SGLD_DL_CVL);
////										for (ld = 0; ld < m_nDcCount; ++ld) nDc += Set_STLCIncludeCS(arDcLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
////										for (ld = 0; ld < m_nDwCount; ++ld) nDw += Set_STLCIncludeCS(arDwLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
////
////										for (ld = 0; ld < m_nCSADCount; ++ld) nDL += Set_STLCIncludeCS(arCSADLoad[ld], dDFactor[d], LoadCaseNo, LcomD, D_SGLD_DL_CVL);
////										for (ld = 0; ld < m_nCSADcCount; ++ld) nDc += Set_STLCIncludeCS(arCSADcLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
////										for (ld = 0; ld < m_nCSADwCount; ++ld) nDw += Set_STLCIncludeCS(arCSADwLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
////
////										for (ld = 0; ld < m_nPLCount; ++ld) nPL += Set_STLCIncludeCS(arPLLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
////										for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dDFactor[d], LoadCaseNo, LcomD, 0);
////
////										////Earth Pressure
////										for (ld = 0; ld < m_nEsCount; ++ld) nES += Set_STLCIncludeCS(arEsLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
////										for (ld = 0; ld < m_nEpCount; ++ld) nEP += Set_STLCIncludeCS(arEpLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
////										////for (ld = 0; ld < m_nEhCount; ++ld) nEH += Set_STLCIncludeCS(arEhLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
////										for (ld = 0; ld < m_nEvCount; ++ld) nEV += Set_STLCIncludeCS(arEvLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
////
////										// for(ld=0; ld<m_nLsCount; ++ld) nLS += Set_STLCIncludeCS(arLsLoad[ld], dLsFactor, LoadCaseNo, LcomD, 0);
////											   //if(m_nSmCount > 0) Set_SettleCase(arSmLoad[nSmIndex], dSmFactor, LoadCaseNo, LcomD, TRUE);
////										//Water Pressure
////										//for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
////										for (ld = 0; ld < m_nWpCount; ld++) nWP += Set_STLCIncludeCS(arWpLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
////										for (ld = 0; ld < m_nFpCount; ld++) nFP += Set_STLCIncludeCS(arFpLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
////										//Wind Load and Wind On Live
////										for (ld = 0; ld < m_nWCount; ++ld) nW += Set_STLCIncludeCS(arWLoad[ld], dWFactor, LoadCaseNo, LcomD, 0);
////										for (ld = 0; ld < m_nWlCount; ld++) nWl += Set_STLCIncludeCS(arWlLoad[ld], dWFactor, LoadCaseNo, LcomD, 0);
////
////										////Vehicle load and Surcharge
////										for (ld = 0; ld < m_nLCount; ++ld) nLL += Set_STLCIncludeCS(arLLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
////										for (ld = 0; ld < m_nLsCount; ++ld)  nLS += Set_STLCIncludeCS(arLsLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
////
////										//if (m_nMCount > 0)  Set_MovingCase(arMLoad.GetAt(j), 1.0, LoadCaseNo, LcomD, TRUE);
////										//for (ld = 0; ld < m_nCrlCount; ++ld) nCRL += Set_STLCIncludeCS(arCrlLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
////
////										for (ld = 0; ld < m_nPsCount; ++ld) nPS += Set_STLCIncludeCS(arPsLoad[ld], dPSFactor, LoadCaseNo, LcomD, D_SGLD_TS_CVL);
////										//if (m_nSmCount > 0) Set_SettleCase(arSmLoad.GetAt(k), dCpShFact, LoadCaseNo, LcomD, TRUE);
////										for (ld = 0; ld < m_nCfCount; ++ld) nCF += Set_STLCIncludeCS(arCfLoad[ld], dCFForce, LoadCaseNo, LcomD, 0);
////										for (ld = 0; ld < m_nASCount; ++ld) nAS += Set_STLCIncludeCS(arASLoad[ld], dCFForce, LoadCaseNo, LcomD, 0);
////										for (ld = 0; ld < m_nCoCount; ++ld) nCO += Set_STLCIncludeCS(arCoLoad[ld], dCOForce, LoadCaseNo, LcomD, 0);
////										for (ld = 0; ld < m_nSfCount; ++ld) nSf += Set_STLCIncludeCS(arSfLoad[ld], dSfForce, LoadCaseNo, LcomD, 0);
////										for (ld = 0; ld < m_nWprCount; ++ld) nWpr += Set_STLCIncludeCS(arWprLoad[ld], dSfForce, LoadCaseNo, LcomD, 0);
////										for (ld = 0; ld < m_nStlCount; ++ld) nStl += Set_STLCIncludeCS(arStlLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
////										for (ld = 0; ld < m_nTCount; ++ld) nT += Set_STLCIncludeCS(arTLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
////										for (ld = 0; ld < m_nTpgCount; ++ld) nTpg += Set_STLCIncludeCS(arTpgLoad[ld], dTPGFact1, LoadCaseNo, LcomD, 0);
////
////										if (m_nConstLoad != 0) // ST+CS
////										{
////											if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
////												Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor[d], LoadCaseNo, LcomD, TRUE);
////
////											CString strcEL;
////											int nErection = 0;
////											for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
////											{
////												strcEL = _T(""); nErection = 0;
////												if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }
////
////
////
////												if (CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")) || CheckCSEL(nErection, _T("PL")) || CheckCSEL(nErection, _T("BL")))
////													Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor[d], LoadCaseNo, LcomD, TRUE);
////												else if (CheckCSEL(nErection, _T("D")))
////													Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor[d], LoadCaseNo, LcomD, TRUE);
////												else if (CheckCSEL(nErection, _T("ER")))
////													Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
////												else if (CheckCSEL(nErection, _T("LIP")))
////													Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
////												else if (CheckCSEL(nErection, _T("L")))
////													Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
////
////												else if (CheckCSEL(nErection, _T("T")))
////													Set_CStageCase(arCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
////												else if (CheckCSEL(nErection, _T("TPG")))
////													Set_CStageCase(arCSLoad.GetAt(nnn), dTPGFact1, LoadCaseNo, LcomD, TRUE);
////
////											}
////											if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
////												Set_CStageCase(arCSLoad.GetAt(nnn), dCSTPFact, LoadCaseNo, LcomD, TRUE);
////											if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
////												Set_CStageCase(arCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
////
////											// MQC:4539-JHYUN-20120316
////											if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep Primary
////											{
////												Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
////											}
////											if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep secondary
////											{
////												Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
////											}
////											if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage Primary
////											{
////												Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
////											}
////											if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage secondary
////											{
////												Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
////											}
////
////										}
////
////												for (ld = 0; ld < arELoad_x.GetSize(); ++ld)    nES += Set_STLCIncludeCS(arELoad_x[ld], dRS_X, LoadCaseNo, LcomD, 0);
////												//for (ld = 0; ld < arELoad_y.GetCount(); ++ld)        nES += Set_STLCIncludeCS(arELoad_y[ld], dRS_Y, LoadCaseNo, LcomD, 0);
////												//for (ld = 0; ld < arELoad_z.GetCount(); ++ld)        nES += Set_STLCIncludeCS(arELoad_z[ld], dRS_Z, LoadCaseNo, LcomD, 0);
////
////																									
////												if (arEspLoad_X.GetSize() > 0) Set_SpectrumCase(arEspLoad_X[m / 2], dRS_X, LoadCaseNo, LcomD, TRUE);
////												//if (arEspLoad_Y.GetSize() > 0) Set_SpectrumCase(arEspLoad_Y[n / 2], dRS_Y, LoadCaseNo, LcomD, TRUE);
////												//if (arEspLoad_Z.GetSize() > 0) Set_SpectrumCase(arEspLoad_Z[o / 2], dRS_Z, LoadCaseNo, LcomD, TRUE);
////										// description
////										LcomD.Description.Empty();
////										csPrefix.Empty();
////										LcomD.Description += _T("3U:");
////										if (nDL > 0)
////										{
////											LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("D");
////											csPrefix = _T(":");
////										}
////										if (nDc > 0)
////										{
////											LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("DC");
////											csPrefix = _T("+");
////										}
////										if (nPS > 0)
////										{
////											LcomD.Description += csPrefix + GetFactorString(dPSFactor) + _T("PS");
////											csPrefix = _T(":");
////										}
////										if (nDw > 0)
////										{
////											LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("DW");
////											csPrefix = _T("+");
////										}
////										if (nW > 0)
////										{
////											LcomD.Description += csPrefix + GetFactorString(dWFactor) + _T("WL");
////											csPrefix = _T("+");
////										}
////										if (nB > 0)
////										{
////											LcomD.Description += csPrefix + GetFactorString(dDFactor[d]) + _T("BL");
////											csPrefix = _T("+");
////										}
////										if (nES > 0)
////										{
////											LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("ES");
////											csPrefix = _T("+");
////										}
////										if (nEP > 0)
////										{
////											LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("EP");
////											csPrefix = _T("+");
////										}
////										if (nEH > 0)
////										{
////											LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("EH");
////											csPrefix = _T("+");
////										}
////										if (nEV > 0)
////										{
////											LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("EV");
////											csPrefix = _T("+");
////										}
////										if (m_nMCount > 0)
////										{
////											LcomD.Description += csPrefix + _T("ML"); csPrefix = _T(":");
////										}
////										if (nB > 0)
////										{
////											LcomD.Description += csPrefix + GetFactorString(dWPFactor) + _T("BL");
////											csPrefix = _T("+");
////										}
////										if (nWP > 0)
////										{
////											LcomD.Description += csPrefix + GetFactorString(dWPFactor) + _T("WP");
////											csPrefix = _T("+");
////										}
////										if (nFP > 0)
////										{
////											LcomD.Description += csPrefix + GetFactorString(dWPFactor) + _T("FP");
////											csPrefix = _T("+");
////										}
////										if (nLL > 0)
////										{
////											LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("LL");
////											csPrefix = _T("+");
////										}
////										if (nLS > 0)
////										{
////											LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("LS");
////											csPrefix = _T("+");
////										}
////										if (LoadCaseNo > 0) Set_LoadComb4Civil(LcomD, D_STR);
////										if (nCRL > 0)
////										{
////											LcomD.Description += csPrefix + GetFactorString(dEsFact) + _T("CRL");
////											csPrefix = _T("+");
////										}
////										if (m_nConstLoad != 0) // ST+CS
////										{
////
////
////											CString strcEL;
////											int nErection = 0;
////
////											for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
////											{
////												strcEL = _T(""); nErection = 0;
////
////												if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn))
////												{
////													LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCP)"); csPrefix = _T("+");
////												}
////												if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn))
////												{
////													LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCS)"); csPrefix = _T("+");
////												}
////												if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn))
////												{
////													LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSP)"); csPrefix = _T("+");
////												}
////												if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn))
////												{
////													LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSS)"); csPrefix = _T("+");
////												}
////												if (CheckCSEL(nErection, _T("ER")))
////												{
////													LcomD.Description += csPrefix + GetFactorString(dCSFactor) + +_T("(cER)") + strcEL; csPrefix = _T("+");
////												}
////												if (CheckCSEL(nErection, _T("LIP")))
////												{
////													LcomD.Description += csPrefix + GetFactorString(dCSFactor) + _T("(cLIP)") + strcEL; csPrefix = _T("+");
////												}
////												if (CheckCSEL(nErection, _T("LL")))
////												{
////													LcomD.Description += csPrefix + GetFactorString(dCSFactor) + _T("(cLL)") + strcEL; csPrefix = _T("+");
////												}
////												if (arEspLoad_X.GetSize() > 0)
////												{
////													LcomD.Description += csPrefix + GetFactorString(dRS_X) + _T("RS(X)");
////													csPrefix = _T(":");
////												}
////												if (arEspLoad_Y.GetSize() > 0)
////												{
////													//LcomD.Description += csPrefix + GetFactorString(dRS_Y) + _T("RS(Y)");
////													csPrefix = _T(":");
////												}
////												if (arEspLoad_Z.GetSize() > 0)
////												{
////													//LcomD.Description += csPrefix + GetFactorString(dRS_Z) + _T("RS(Z)");
////													csPrefix = _T(":");
////												}
////											}
////
////										}
////									}
////								}
////							}
////						}
////					}
////				}
////			}
////		}
////	}
////}//ENd eql=0
//
//
//////////////////////////////////////////////////////////////////////// 32297 - 16 Cases
////if (m_nConstLoad != 1)
////{
////	T_LCOM_D LcomD;
////
////	int nEsp_x = arEspLoad_X.GetSize();
////	int nEsp_y = arEspLoad_Y.GetSize();
////	int nEsp_z = arEspLoad_Z.GetSize();
////	int nTtlESP = nEsp_x + nEsp_y + nEsp_z;
////
////	int nDCount = 1;
////	BOOL bExistD = (m_nDCount > 0 || m_nDwCount > 0 || m_nDcCount > 0);
////	if (bExistD) nDCount = m_dUserDefDL > 2.20 ? 2 : 1;//For Both Case of Dead Load
////	int nMLCount = max(m_nMCount, 1);
////	int nWPCount = (m_nWpCount >= 1) ? 3 * m_nWpCount : 1;//Adding and Relieving Water Pressure
////	int nEPCount = (m_nEpCount >= 1) ? 3 * m_nEpCount : 1;//Adding and Relieving Earth Pressure
////	int nPSCount = (m_nPsCount >= 1) ? 2 * m_nPsCount : 1;//Adding and Relieving PreStressing
////	int nWLCount = (m_nWCount >= 1) ? 3 * m_nWCount : 1;//Adding and Relieving Wind Load
////	int nWLLCount = (m_nWlCount >= 1) ? 3 * m_nWlCount : 1;//Adding and Relieving Wind Load on Live Load
////	m_dUserDefDL = m_nCodeBasedOrUserDef == 0 ? 1.2 : m_dUserDefDL;
////	double dDFactor = 1.2;//For BOth Case,
////	//if (nDCount == 1) { dDFactor[0] = m_dUserDefDL, dDFactor[1] = m_dUserDefDL; }
////
////	int  nnn = 0;
////
////	double dEsFact = m_nConstLoad == 1 ? 0. : 1.5;//m_nConstLoad = 0 - ST,m_nConstLoad = 1 - CS,m_nConstLoad = 2 - ST+CS
////	double dWPFactor = 1.05;//Water Pressure Factor
////	double dPSFactor = 0.;//{ 0.,0.87 };//Pre Stress Factor
////	double dCpShFact = 1.;//Creep Shrinkage Factor
////	double dCSFactor = 1.15;//CS Factor
////	double dWFactor = 1.1;//Wind Factor 1.1 or 0
////	double dTPGFact1 = 0.8;
////	double dCFForce = 1.5;
////	double dCOForce = 1.25;
////	double dSfForce = 1.05;
////	int nSmCount = 1;
////
////	BOOL bErec = FALSE;
////	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
////	{
////		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
////	}
////
////	BOOL bAddCsLoad = bErec;
////	// 			(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
////	// 			(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
////	// 			(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
////	// 			(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
////	// 			(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
////	// 			(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
////	// 			(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
////	// 			(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
////	// 			(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
////	// 			(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;
////
////	int nCSCount = ((m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_TP_CVL, nnn)) || (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_TS_CVL, nnn))) ? 1 : 1;
////
////	if (bExistD && m_nEC4Permanent == 2) nDCount = 2;
////	int nWCount = max(m_nWCount, 1);
////	
////	
////	double dEFactor_x = 1.25, dEFactor_y = 1.25*0.3;
////	
////	int nSmIndex = 0;
////	double dRsFactor = 1.3;
////	int nESPCount_X = (nEsp_x > 0) ? nEsp_x * 2 : 1;
////	int nESPCount_Y = (nEsp_y > 0) ? nEsp_y * 2 : 1;
////	int nESPCount_Z = (nEsp_z > 0) ? nEsp_z * 2 : 1;
////
////	m_bIRSCons_crp_shr_DT_ULS = TRUE;
////	
////	
////	double dLFactor = 1.1;
////	double dLsFactor = 1.0;
////	double dEPFactor = m_nConstLoad == 1 ? 0. : 1.5;
////	double dEhFact = m_nConstLoad == 1 ? 0. : 1.5;
////	double dErFact = 1.35;
////	double dPsFactor = 1.0;
////	
////	double dPsi1Factor = 1.0;
////	double dSmFactor = m_dIRSServ != 1. ? m_dIRSServ : 1.0;
////	double dTempFactor = 0.9;//Accompanying load
////	double dSmFactor1 = 1.;
////	
////	CString csPrefix(_T("")), strFixM(_T("")), strFixW(_T("")), strFixE(_T(""));
////
////	int LoadCaseNo = 0, ld = 0;
////
////
////	//for (int eql=0;eql<nTtlESP;eql++)
////	{
////		if (eql == 0)
////		{
////			////////////// X Major ////////////////
////			for (int d = 0; d < nDCount; ++d)
////			{
////				dDFactor = 1.2;
////				for (int p = 0; p < nPSCount; ++p) {
////					if (p % 2 == 0) dPSFactor = 1.15;
////					else if (p % 2 == 1) dPSFactor = 0.87;//PS Relieving
////					for (int l = 0; l < nEPCount; ++l) {
////						if (l % 3 == 0) dEPFactor = m_nConstLoad == 1 ? 0. : 1.5;
////						else if (l % 3 == 1) dEPFactor = m_nConstLoad == 1 ? 0. : 0.85;//EP Relieving
////						else if (l % 3 == 2) dEPFactor = m_nConstLoad == 1 ? 0. : 0.0;//EP Relieving
////
////						for (int w = 0; w < nWPCount; ++w) {
////							if (w % 3 == 0) dWPFactor = m_nConstLoad == 1 ? 0 : 1.05;
////							else if (w % 3 == 1) dWPFactor = m_nConstLoad == 1 ? 0.0 : 1.0;
////							else if (w % 3 == 2) dWPFactor = m_nConstLoad == 1 ? 0.0 : 0.0;
////
////							for (int ls = 0; ls < nCSCount; ++ls) {
////								dCSFactor = 1.;
////								for (int m = 0; m < nESPCount_X; m++) {
////									for (int n = 0; n < nESPCount_Y; n++) {
////										for (int o = 0; o < nESPCount_Z; o++) {
////											for (int k = 0; k < nSmCount; ++k) {
////												for (int j = 0; j < m_nMCount; ++j) {
////													int nDL = 0, nB = 0, nDc = 0, nDw = 0, nBL = 0, nEP = 0, nSM = 0, nStl = 0, nEH = 0, nEV = 0, nES = 0;
////													int nWP = 0, nLS = 0, nCL = 0, nLL = 0, nCF = 0, nBRK = 0, nBK = 0, nMVL = 0, nCR = 0, nSH = 0, nGWP = 0;
////													int nPL = 0, nPS = 0, dWL = 0, nWLL = 0, nCRL = 0, nER = 0, nW = 0,nFP=0;
////
////													LcomD.Initialize();
////													LcomD.nActive = 1;
////													LoadCaseNo = 0;
////
////													//Permanent
////													for (ld = 0; ld < m_nDCount; ++ld) nDL += Set_STLCIncludeCS(arDLoad[ld], dDFactor, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
////													for (ld = 0; ld < m_nDcCount; ++ld) nDc += Set_STLCIncludeCS(arDcLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nDwCount; ++ld) nDw += Set_STLCIncludeCS(arDwLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
////
////													for (ld = 0; ld < m_nCSADCount; ++ld) nDL += Set_STLCIncludeCS(arCSADLoad[ld], dDFactor, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
////													for (ld = 0; ld < m_nCSADcCount; ++ld) nDc += Set_STLCIncludeCS(arCSADcLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nCSADwCount; ++ld) nDw += Set_STLCIncludeCS(arCSADwLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
////
////													for (ld = 0; ld < m_nPLCount; ++ld) nPL += Set_STLCIncludeCS(arPLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
////
////													for (ld = 0; ld < m_nStlCount; ++ld)  nStl += Set_STLCIncludeCS(arStlLoad[ld], dSmFactor1, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nCrlCount; ++ld) nCRL += Set_STLCIncludeCS(arCrlLoad[ld], dEhFact, LoadCaseNo, LcomD, 0);
////
////													for (ld = 0; ld < m_nLCount; ++ld) nLL += Set_STLCIncludeCS(arLLoad[ld], dLFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nCfCount; ++ld) nCF += Set_STLCIncludeCS(arCfLoad[ld], dLFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nBrkCount; ++ld) nBRK += Set_STLCIncludeCS(arBrkLoad[ld], dLFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nBkCount; ++ld) nBK += Set_STLCIncludeCS(arBkLoad[ld], dLFactor, LoadCaseNo, LcomD, 0);
////
////													for (ld = 0; ld < m_nEpCount; ++ld) nEP += Set_STLCIncludeCS(arEpLoad[ld], dEPFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nEvCount; ++ld) nEV += Set_STLCIncludeCS(arEvLoad[ld], dEPFactor, LoadCaseNo, LcomD, 0);
////													//for (ld = 0; ld < m_nEhCount; ++ld) nEH += Set_STLCIncludeCS(arEhLoad[ld], dEPFactor, LoadCaseNo, LcomD, 0);
////													//for (ld = 0; ld < m_nEvCount; ++ld) nEV += Set_STLCIncludeCS(arEvLoad[ld], dEPFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nEsCount; ++ld) nES += Set_STLCIncludeCS(arEsLoad[ld], dEPFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nWpCount; ++ld) nGWP += Set_STLCIncludeCS(arWpLoad[ld], dEPFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nLsCount; ++ld) nLS += Set_STLCIncludeCS(arLsLoad[ld], dEPFactor, LoadCaseNo, LcomD, 0);
////
////													if (m_bIRSCons_crp_shr_DT_ULS)
////													{
////														for (ld = 0; ld < m_nShCount; ++ld)  nSH += Set_STLCIncludeCS(arShLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
////														for (ld = 0; ld < m_nCrCount; ++ld)  nCR += Set_STLCIncludeCS(arCrLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
////													}
////													if (m_nSmCount > 0) Set_SettleCase(arSmLoad.GetAt(k), dSmFactor1, LoadCaseNo, LcomD, TRUE);
////													if (m_nMCount > 0)  Set_MovingCase(arMLoad.GetAt(j), dDFactor, LoadCaseNo, LcomD, TRUE);
////													//Water Pressure
////									//for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nFpCount; ld++) nFP += Set_STLCIncludeCS(arFpLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
////
////													if (m_nConstLoad != 0) // ST+CS
////													{
////														if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
////															Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor, LoadCaseNo, LcomD, TRUE);
////
////														CString strcEL;
////														int nErection = 0;
////														for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
////														{
////															strcEL = _T(""); nErection = 0;
////															if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }
////
////
////
////															if (CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")) || CheckCSEL(nErection, _T("PL")) || CheckCSEL(nErection, _T("BL")))
////																Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor, LoadCaseNo, LcomD, TRUE);
////															else if (CheckCSEL(nErection, _T("D")))
////																Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor, LoadCaseNo, LcomD, TRUE);
////
////														}
////														if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
////															Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
////														if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
////															Set_CStageCase(arCSLoad.GetAt(nnn), dPsFactor, LoadCaseNo, LcomD, TRUE);
////														if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep Primary
////														{
////															Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
////														}
////														if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep secondary
////														{
////															Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
////														}
////														if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage Primary
////														{
////															Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
////														}
////														if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage secondary
////														{
////															Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
////														}
////
////													}
////													double dRS_X = (m % 2 == 0) ? dRsFactor : -1. * dRsFactor;
////													double dRS_Y = (n % 2 == 0) ? dRsFactor : -1. * dRsFactor;
////													double dRS_Z = (o % 2 == 0) ? dRsFactor : -1. * dRsFactor;
////													if (m % 2 == 0 || m % 2 == 1)
////													{
////														dRS_Y = 0.3 * dRS_Y;
////														dRS_Z = 0.3 * dRS_Z;
////													}
////													else if (n % 2 == 0)
////													{
////														dRS_X = 0.3 * dRS_X;
////														dRS_Z = 0.3 * dRS_Z;
////													}
////													else if (o % 2 == 0)
////													{
////														dRS_X = 0.3 * dRS_X;
////														dRS_Y = 0.3 * dRS_Y;
////
////													}
////													else if (m % 2 == 1 && n % 2 == 1 && o % 2 == 1)
////													{
////														dRS_Y = 0.3 * dRS_Y;
////														dRS_Z = 0.3 * dRS_Z;
////													}
////
////													for (ld = 0; ld < m_nECount; ++ld)
////													{
////														if (m_nECount == 1)
////														{
////															nES += Set_STLCIncludeCS(arELoad[ld], dEFactor_x, LoadCaseNo, LcomD, 0);
////														}
////														else
////														{
////															if (ld == 0)            nES += Set_STLCIncludeCS(arELoad[ld], dRS_X, LoadCaseNo, LcomD, 0);
////															else if (ld % 2 == 1)     nES += Set_STLCIncludeCS(arELoad[ld], dRS_Y, LoadCaseNo, LcomD, 0);
////															else if (ld % 2 == 0)     nES += Set_STLCIncludeCS(arELoad[ld], dRS_Z, LoadCaseNo, LcomD, 0);
////
////														}
////
////													}
////
////													if (arEspLoad_X.GetSize() > 0) Set_SpectrumCase(arEspLoad_X[m / 2], dRS_X, LoadCaseNo, LcomD, TRUE);
////													if (arEspLoad_Y.GetSize() > 0) Set_SpectrumCase(arEspLoad_Y[n / 2], dRS_Y, LoadCaseNo, LcomD, TRUE);
////													if (arEspLoad_Z.GetSize() > 0) Set_SpectrumCase(arEspLoad_Z[o / 2], dRS_Z, LoadCaseNo, LcomD, TRUE);
////
////
////													// description
////													LcomD.Description.Empty();
////													csPrefix.Empty();
////
////													LcomD.Description += _T("2S:");
////													if (nDL > 0)
////													{
////														LcomD.Description += csPrefix + _T("D");
////														csPrefix = _T(":");
////													}
////													if (nDc > 0 || nDw > 0 || nPL > 0 || nBL > 0)
////													{
////														LcomD.Description += csPrefix + _T("SIDL");
////														csPrefix = _T(":");
////													}
////													if (arEspLoad_X.GetSize() > 0)
////													{
////														LcomD.Description += csPrefix + GetFactorString(dRS_X) + _T("RS(X)");
////														csPrefix = _T(":");
////													}
////													if (arEspLoad_Y.GetSize() > 0)
////													{
////														LcomD.Description += csPrefix + GetFactorString(dRS_Y) + _T("RS(Y)");
////														csPrefix = _T(":");
////													}
////													if (arEspLoad_Z.GetSize() > 0)
////													{
////														LcomD.Description += csPrefix + GetFactorString(dRS_Z) + _T("RS(Z)");
////														csPrefix = _T(":");
////													}
////													if (m_nSmCount > 0 || nStl > 0)
////													{
////														LcomD.Description += csPrefix + _T("SM");
////														csPrefix = _T(":");
////													}
////													if (m_nMCount > 0)
////													{
////														LcomD.Description += csPrefix + _T("ML"); csPrefix = _T(":");
////													}
////													if (nLL > 0 || nCF > 0 || nBRK > 0 || nBK > 0)
////													{
////														LcomD.Description += csPrefix + _T("FPLL");
////														csPrefix = _T(":");
////													}
////													if (nEP > 0 || nEH > 0 || nEV > 0 || nES > 0 || nGWP > 0 || nLS > 0)
////													{
////														LcomD.Description += csPrefix; LcomD.Description += (l % 2 == 0) ? _T("EP") : _T("rEP");
////														csPrefix = _T(":");
////													}
////													if (m_nConstLoad != 0) // ST+CS
////													{
////														if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
////														{
////															LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("(cD)"); csPrefix = _T("+");
////														}
////
////														CString strcEL;
////														int nErection = 0;
////
////														for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
////														{
////															strcEL = _T(""); nErection = 0;
////															if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }
////
////
////
////															if (CheckCSEL(nErection, _T("D")) || CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")))
////															{
////																LcomD.Description += csPrefix + GetFactorString(dDFactor) + strcEL; csPrefix = _T("+");
////															}
////
////															else if (CheckCSEL(nErection, _T("W")))
////															{
////																LcomD.Description += csPrefix + GetFactorString(dWFactor) + strcEL; csPrefix = _T("+");
////															}
////														}
////
////														if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
////														{
////															LcomD.Description += csPrefix + GetFactorString(dCSFactor) + _T("(cTP)"); csPrefix = _T("+");
////														}
////														if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
////														{
////															LcomD.Description += csPrefix + GetFactorString(dPsFactor) + _T("(cTS)"); csPrefix = _T("+");
////														}
////														if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn))
////														{
////															LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCP)"); csPrefix = _T("+");
////														}
////														if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn))
////														{
////															LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCS)"); csPrefix = _T("+");
////														}
////														if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn))
////														{
////															LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSP)"); csPrefix = _T("+");
////														}
////														if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn))
////														{
////															LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSS)"); csPrefix = _T("+");
////														}
////													}
////													if (LoadCaseNo > 0)
////													{
////														if (m_nLcomType != D_LCOMTYPE_GENERAL) LcomD.nActive = 2;
////														Set_LoadComb4Civil(LcomD, D_SER);
////													}
////
////												}
////											}
////										}
////									}
////								}
////							}
////						}
////					}
////				}
////			}
////
////		}
////		else if (eql % 2 == 1)
////		{
////			///////////////////Y MAJOR//////////////////
////			for (int d = 0; d < nDCount; ++d)
////			{
////				dDFactor = 1.0;
////				for (int p = 0; p < nPSCount; ++p) {
////					if (p % 2 == 0) dPSFactor = 1.15;
////					else if (p % 2 == 1) dPSFactor = 0.87;//PS Relieving
////					for (int l = 0; l < nEPCount; ++l) {
////						
////						if (l % 3 == 0) dEPFactor = m_nConstLoad == 1 ? 0. : 1.5;
////						else if (l % 3 == 1) dEPFactor = m_nConstLoad == 1 ? 0. : 0.85;//EP Relieving
////						else if (l % 3 == 2) dEPFactor = m_nConstLoad == 1 ? 0. : 0.0;//EP Relieving
////
////						for (int w = 0; w < nWPCount; ++w) {
////							if (w % 3 == 0) dWPFactor = m_nConstLoad == 1 ? 0 : 1.05;
////							else if (w % 3 == 1) dWPFactor = m_nConstLoad == 1 ? 0.0 : 1.0;
////							else if (w % 3 == 2) dWPFactor = m_nConstLoad == 1 ? 0.0 : 0.0;
////
////							for (int ls = 0; ls < nCSCount; ++ls) {
////								dCSFactor = 1.;
////								for (int m = 0; m < nESPCount_X; m++) {
////									for (int n = 0; n < nESPCount_Y; n++) {
////										for (int o = 0; o < nESPCount_Z; o++) {
////											for (int k = 0; k < nSmCount; ++k) {
////												for (int j = 0; j < m_nMCount; ++j) {
////													int nDL = 0, nB = 0, nDc = 0, nDw = 0, nBL = 0, nEP = 0, nSM = 0, nStl = 0, nEH = 0, nEV = 0, nES = 0;
////													int nWP = 0, nLS = 0, nCL = 0, nLL = 0, nCF = 0, nBRK = 0, nBK = 0, nMVL = 0, nCR = 0, nSH = 0, nGWP = 0;
////													int nPL = 0, nPS = 0, dWL = 0, nWLL = 0, nCRL = 0, nER = 0, nW = 0,nFP=0;
////
////													LcomD.Initialize();
////													LcomD.nActive = 1;
////													LoadCaseNo = 0;
////
////													//Permanent
////													for (ld = 0; ld < m_nDCount; ++ld) nDL += Set_STLCIncludeCS(arDLoad[ld], dDFactor, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
////													for (ld = 0; ld < m_nBCount; ++ld) nB += Set_STLCIncludeCS(arBLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nPsCount; ++ld) nPS += Set_STLCIncludeCS(arPsLoad[ld], dCSFactor, LoadCaseNo, LcomD, D_SGLD_TS_CVL);
////													for (ld = 0; ld < m_nDcCount; ++ld) nDc += Set_STLCIncludeCS(arDcLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nDwCount; ++ld) nDw += Set_STLCIncludeCS(arDwLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nPLCount; ++ld) nPL += Set_STLCIncludeCS(arPLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nBLCount; ++ld) nBL += Set_STLCIncludeCS(arBLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
////
////													for (ld = 0; ld < m_nStlCount; ++ld)  nStl += Set_STLCIncludeCS(arStlLoad[ld], dSmFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nCrlCount; ++ld) nCRL += Set_STLCIncludeCS(arCrlLoad[ld], dEhFact, LoadCaseNo, LcomD, 0);
////
////													for (ld = 0; ld < m_nLCount; ++ld) nLL += Set_STLCIncludeCS(arLLoad[ld], dLFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nCfCount; ++ld) nCF += Set_STLCIncludeCS(arCfLoad[ld], dLFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nBrkCount; ++ld) nBRK += Set_STLCIncludeCS(arBrkLoad[ld], dLFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nBkCount; ++ld) nBK += Set_STLCIncludeCS(arBkLoad[ld], dLFactor, LoadCaseNo, LcomD, 0);
////
////													for (ld = 0; ld < m_nEpCount; ++ld) nEP += Set_STLCIncludeCS(arEpLoad[ld], dEPFactor, LoadCaseNo, LcomD, 0);
////													//for (ld = 0; ld < m_nEhCount; ++ld) nEH += Set_STLCIncludeCS(arEhLoad[ld], dEPFactor, LoadCaseNo, LcomD, 0);
////													//for (ld = 0; ld < m_nEvCount; ++ld) nEV += Set_STLCIncludeCS(arEvLoad[ld], dEPFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nEsCount; ++ld) nES += Set_STLCIncludeCS(arEsLoad[ld], dEPFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nWpCount; ++ld) nGWP += Set_STLCIncludeCS(arWpLoad[ld], dEPFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nLsCount; ++ld) nLS += Set_STLCIncludeCS(arLsLoad[ld], dEPFactor, LoadCaseNo, LcomD, 0);
////													if (m_bIRSCons_crp_shr_DT_ULS)
////													{
////														for (ld = 0; ld < m_nShCount; ++ld)  nSH += Set_STLCIncludeCS(arShLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
////														for (ld = 0; ld < m_nCrCount; ++ld)  nCR += Set_STLCIncludeCS(arCrLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
////													}
////													if (m_nSmCount > 0) Set_SettleCase(arSmLoad.GetAt(k), dSmFactor, LoadCaseNo, LcomD, TRUE);
////													if (m_nMCount > 0)  Set_MovingCase(arMLoad.GetAt(j), dDFactor, LoadCaseNo, LcomD, TRUE);
////													//Water Pressure
////									//for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nWpCount; ld++) nWP += Set_STLCIncludeCS(arWpLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nFpCount; ld++) nFP += Set_STLCIncludeCS(arFpLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
////
////													if (m_nConstLoad != 0) // ST+CS
////													{
////														if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
////															Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor, LoadCaseNo, LcomD, TRUE);
////
////														CString strcEL;
////														int nErection = 0;
////														for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
////														{
////															strcEL = _T(""); nErection = 0;
////															if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }
////
////
////
////															if (CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")) || CheckCSEL(nErection, _T("PL")) || CheckCSEL(nErection, _T("BL")))
////																Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor, LoadCaseNo, LcomD, TRUE);
////															else if (CheckCSEL(nErection, _T("D")))
////																Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor, LoadCaseNo, LcomD, TRUE);
////
////														}
////														if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
////															Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
////														if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
////															Set_CStageCase(arCSLoad.GetAt(nnn), dPsFactor, LoadCaseNo, LcomD, TRUE);
////														if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep Primary
////														{
////															Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
////														}
////														if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep secondary
////														{
////															Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
////														}
////														if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage Primary
////														{
////															Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
////														}
////														if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage secondary
////														{
////															Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
////														}
////
////													}
////													double dRS_X = (m % 2 == 0) ? dRsFactor : -1. * dRsFactor;
////													double dRS_Y = (n % 2 == 0) ? dRsFactor : -1. * dRsFactor;
////													double dRS_Z = (o % 2 == 0) ? dRsFactor : -1. * dRsFactor;
////													if (m % 2 == 0 || m % 2 == 1)
////													{
////														dRS_Y = 0.3 * dRS_Y;
////														dRS_Z = 0.3 * dRS_Z;
////													}
////													else if (n % 2 == 0)
////													{
////														dRS_X = 0.3 * dRS_X;
////														dRS_Z = 0.3 * dRS_Z;
////													}
////													else if (o % 2 == 0)
////													{
////														dRS_X = 0.3 * dRS_X;
////														dRS_Y = 0.3 * dRS_Y;
////
////													}
////													else if (m % 2 == 1 && n % 2 == 1 && o % 2 == 1)
////													{
////														dRS_Y = 0.3 * dRS_Y;
////														dRS_Z = 0.3 * dRS_Z;
////													}
////
////													for (ld = 0; ld < m_nECount; ++ld)
////													{
////														if (m_nECount == 1)
////														{
////															nES += Set_STLCIncludeCS(arELoad[ld], dEFactor_x, LoadCaseNo, LcomD, 0);
////														}
////														else
////														{
////															if (ld == 0)            nES += Set_STLCIncludeCS(arELoad[ld], dRS_Y, LoadCaseNo, LcomD, 0);
////															else if (ld % 2 == 1)     nES += Set_STLCIncludeCS(arELoad[ld], dRS_X, LoadCaseNo, LcomD, 0);
////															else if (ld % 2 == 0)     nES += Set_STLCIncludeCS(arELoad[ld], dRS_Z, LoadCaseNo, LcomD, 0);
////
////														}
////
////													}
////
////													if (arEspLoad_X.GetSize() > 0) Set_SpectrumCase(arEspLoad_X[n / 2], dRS_Y, LoadCaseNo, LcomD, TRUE);
////													if (arEspLoad_Y.GetSize() > 0) Set_SpectrumCase(arEspLoad_Y[m / 2], dRS_X, LoadCaseNo, LcomD, TRUE);
////													if (arEspLoad_Z.GetSize() > 0) Set_SpectrumCase(arEspLoad_Z[o / 2], dRS_Z, LoadCaseNo, LcomD, TRUE);
////
////
////													// description
////													LcomD.Description.Empty();
////													csPrefix.Empty();
////
////													LcomD.Description += _T("2S:");
////													if (nDL > 0)
////													{
////														LcomD.Description += csPrefix + _T("D");
////														csPrefix = _T(":");
////													}
////													if (nDc > 0 || nDw > 0 || nPL > 0 || nBL > 0)
////													{
////														LcomD.Description += csPrefix + _T("SIDL");
////														csPrefix = _T(":");
////													}
////													if (arEspLoad_X.GetSize() > 0)
////													{
////														LcomD.Description += csPrefix + GetFactorString(dRS_Y) + _T("RS(X)");
////														csPrefix = _T(":");
////													}
////													if (arEspLoad_Y.GetSize() > 0)
////													{
////														LcomD.Description += csPrefix + GetFactorString(dRS_X) + _T("RS(Y)");
////														csPrefix = _T(":");
////													}
////													if (arEspLoad_Z.GetSize() > 0)
////													{
////														LcomD.Description += csPrefix + GetFactorString(dRS_Z) + _T("RS(Z)");
////														csPrefix = _T(":");
////													}
////													if (m_nSmCount > 0 || nStl > 0)
////													{
////														LcomD.Description += csPrefix + _T("SM");
////														csPrefix = _T(":");
////													}
////													if (m_nMCount > 0)
////													{
////														LcomD.Description += csPrefix + _T("ML"); csPrefix = _T(":");
////													}
////													if (nLL > 0 || nCF > 0 || nBRK > 0 || nBK > 0)
////													{
////														LcomD.Description += csPrefix + _T("FPLL");
////														csPrefix = _T(":");
////													}
////													if (nEP > 0 || nEH > 0 || nEV > 0 || nES > 0 || nGWP > 0 || nLS > 0)
////													{
////														LcomD.Description += csPrefix; LcomD.Description += (l % 2 == 0) ? _T("EP") : _T("rEP");
////														csPrefix = _T(":");
////													}
////													if (m_nConstLoad != 0) // ST+CS
////													{
////														if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
////														{
////															LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("(cD)"); csPrefix = _T("+");
////														}
////
////														CString strcEL;
////														int nErection = 0;
////
////														for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
////														{
////															strcEL = _T(""); nErection = 0;
////															if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }
////
////
////
////															if (CheckCSEL(nErection, _T("D")) || CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")))
////															{
////																LcomD.Description += csPrefix + GetFactorString(dDFactor) + strcEL; csPrefix = _T("+");
////															}
////
////															else if (CheckCSEL(nErection, _T("W")))
////															{
////																LcomD.Description += csPrefix + GetFactorString(dWFactor) + strcEL; csPrefix = _T("+");
////															}
////														}
////
////														if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
////														{
////															LcomD.Description += csPrefix + GetFactorString(dCSFactor) + _T("(cTP)"); csPrefix = _T("+");
////														}
////														if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
////														{
////															LcomD.Description += csPrefix + GetFactorString(dPsFactor) + _T("(cTS)"); csPrefix = _T("+");
////														}
////														if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn))
////														{
////															LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCP)"); csPrefix = _T("+");
////														}
////														if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn))
////														{
////															LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCS)"); csPrefix = _T("+");
////														}
////														if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn))
////														{
////															LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSP)"); csPrefix = _T("+");
////														}
////														if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn))
////														{
////															LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSS)"); csPrefix = _T("+");
////														}
////													}
////													if (LoadCaseNo > 0)
////													{
////														if (m_nLcomType != D_LCOMTYPE_GENERAL) LcomD.nActive = 2;
////														Set_LoadComb4Civil(LcomD, D_SER);
////													}
////
////												}
////											}
////										}
////									}
////								}
////							}
////						}
////					}
////				}
////			}
////
////
////		}
////		else if (eql % 2 == 0)
////		{
////			//////////////////Z Major/////////////////
////			for (int d = 0; d < nDCount; ++d)
////			{
////				dDFactor = 1.0;
////				for (int p = 0; p < nPSCount; ++p) {
////					if (p % 2 == 0) dPSFactor = 1.15;
////					else if (p % 2 == 1) dPSFactor = 0.87;//PS Relieving
////					for (int l = 0; l < nEPCount; ++l) {
////						
////						if (l % 3 == 0) dEPFactor = m_nConstLoad == 1 ? 0. : 1.5;
////						else if (l % 3 == 1) dEPFactor = m_nConstLoad == 1 ? 0. : 0.85;//EP Relieving
////						else if (l % 3 == 2) dEPFactor = m_nConstLoad == 1 ? 0. : 0.0;//EP Relieving
////
////						for (int w = 0; w < nWPCount; ++w) {
////							if (w % 3 == 0) dWPFactor = m_nConstLoad == 1 ? 0 : 1.05;
////							else if (w % 3 == 1) dWPFactor = m_nConstLoad == 1 ? 0.0 : 1.0;
////							else if (w % 3 == 2) dWPFactor = m_nConstLoad == 1 ? 0.0 : 0.0;
////
////							for (int ls = 0; ls < nCSCount; ++ls) {
////								dCSFactor = 1.;
////								for (int m = 0; m < nESPCount_X; m++) {
////									for (int n = 0; n < nESPCount_Y; n++) {
////										for (int o = 0; o < nESPCount_Z; o++) {
////											for (int k = 0; k < nSmCount; ++k) {
////												for (int j = 0; j < m_nMCount; ++j) {
////													int nDL = 0, nB = 0, nDc = 0, nDw = 0, nBL = 0, nEP = 0, nSM = 0, nStl = 0, nEH = 0, nEV = 0, nES = 0;
////													int nWP = 0, nLS = 0, nCL = 0, nLL = 0, nCF = 0, nBRK = 0, nBK = 0, nMVL = 0, nCR = 0, nSH = 0, nGWP = 0;
////													int nPL = 0, nPS = 0, dWL = 0, nWLL = 0, nCRL = 0, nER = 0, nW = 0, nFP = 0, nAS = 0, nCO = 0, nSf = 0, nWpr=0, nT=0, nTPG=0;
////													LcomD.Initialize();
////													LcomD.nActive = 1;
////													LoadCaseNo = 0;
////
////													//Permanent
////													for (ld = 0; ld < m_nDCount; ++ld) nDL += Set_STLCIncludeCS(arDLoad[ld], dDFactor, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
////													for (ld = 0; ld < m_nBCount; ++ld) nB += Set_STLCIncludeCS(arBLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nPsCount; ++ld) nPS += Set_STLCIncludeCS(arPsLoad[ld], dCSFactor, LoadCaseNo, LcomD, D_SGLD_TS_CVL);
////													for (ld = 0; ld < m_nDcCount; ++ld) nDc += Set_STLCIncludeCS(arDcLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nDwCount; ++ld) nDw += Set_STLCIncludeCS(arDwLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nPLCount; ++ld) nPL += Set_STLCIncludeCS(arPLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nBLCount; ++ld) nBL += Set_STLCIncludeCS(arBLLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
////
////													for (ld = 0; ld < m_nCSADCount; ++ld) nDL += Set_STLCIncludeCS(arCSADLoad[ld], dDFactor, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
////													for (ld = 0; ld < m_nCSADcCount; ++ld) nDc += Set_STLCIncludeCS(arCSADcLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nCSADwCount; ++ld) nDw += Set_STLCIncludeCS(arCSADwLoad[ld], dDFactor, LoadCaseNo, LcomD, 0);
////
////
////													for (ld = 0; ld < m_nStlCount; ++ld)  nStl += Set_STLCIncludeCS(arStlLoad[ld], dSmFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nCrlCount; ++ld) nCRL += Set_STLCIncludeCS(arCrlLoad[ld], dEhFact, LoadCaseNo, LcomD, 0);
////
////													for (ld = 0; ld < m_nLCount; ++ld) nLL += Set_STLCIncludeCS(arLLoad[ld], dLFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nCfCount; ++ld) nCF += Set_STLCIncludeCS(arCfLoad[ld], dLFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nBrkCount; ++ld) nBRK += Set_STLCIncludeCS(arBrkLoad[ld], dLFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nBkCount; ++ld) nBK += Set_STLCIncludeCS(arBkLoad[ld], dLFactor, LoadCaseNo, LcomD, 0);
////
////													for (ld = 0; ld < m_nEpCount; ++ld) nEP += Set_STLCIncludeCS(arEpLoad[ld], dEPFactor, LoadCaseNo, LcomD, 0);
////													//for (ld = 0; ld < m_nEhCount; ++ld) nEH += Set_STLCIncludeCS(arEhLoad[ld], dEPFactor, LoadCaseNo, LcomD, 0);
////													//for (ld = 0; ld < m_nEvCount; ++ld) nEV += Set_STLCIncludeCS(arEvLoad[ld], dEPFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nEsCount; ++ld) nES += Set_STLCIncludeCS(arEsLoad[ld], dEPFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nWpCount; ++ld) nGWP += Set_STLCIncludeCS(arWpLoad[ld], dEPFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nLsCount; ++ld) nLS += Set_STLCIncludeCS(arLsLoad[ld], dEPFactor, LoadCaseNo, LcomD, 0);
////													if (m_bIRSCons_crp_shr_DT_ULS)
////													{
////														for (ld = 0; ld < m_nShCount; ++ld)  nSH += Set_STLCIncludeCS(arShLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
////														for (ld = 0; ld < m_nCrCount; ++ld)  nCR += Set_STLCIncludeCS(arCrLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
////													}
////													
////													if (m_nSmCount > 0) Set_SettleCase(arSmLoad.GetAt(k), dSmFactor, LoadCaseNo, LcomD, TRUE);
////													if (m_nMCount > 0)  Set_MovingCase(arMLoad.GetAt(j), dDFactor, LoadCaseNo, LcomD, TRUE);
////													
////													
////													for (ld = 0; ld < m_nASCount; ++ld) nAS += Set_STLCIncludeCS(arASLoad[ld], dCFForce, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nCoCount; ++ld) nCO += Set_STLCIncludeCS(arCoLoad[ld], dCOForce, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nSfCount; ++ld) nSf += Set_STLCIncludeCS(arSfLoad[ld], dSfForce, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nWprCount; ++ld) nWpr += Set_STLCIncludeCS(arWprLoad[ld], dSfForce, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nTCount; ++ld) nT += Set_STLCIncludeCS(arTLoad[ld], dCpShFact, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nTpgCount; ++ld) nTPG += Set_STLCIncludeCS(arTpgLoad[ld], dTPGFact1, LoadCaseNo, LcomD, 0);
////
////													////Vehicle load and Surcharge
////													for (ld = 0; ld < m_nLCount; ++ld) nLL += Set_STLCIncludeCS(arLLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nLsCount; ++ld)  nLS += Set_STLCIncludeCS(arLsLoad[ld], dEsFact, LoadCaseNo, LcomD, 0);
////
////													//Water Pressure
////													//for (ld = 0; ld < m_nBLCount; ++ld) nB += Set_STLCIncludeCS(arBLLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nWpCount; ld++) nWP += Set_STLCIncludeCS(arWpLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
////													for (ld = 0; ld < m_nFpCount; ld++) nFP += Set_STLCIncludeCS(arFpLoad[ld], dWPFactor, LoadCaseNo, LcomD, 0);
////
////
////													if (m_nConstLoad != 0) // ST+CS
////													{
////														if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
////															Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor, LoadCaseNo, LcomD, TRUE);
////
////														CString strcEL;
////														int nErection = 0;
////														for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
////														{
////															strcEL = _T(""); nErection = 0;
////															if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }
////
////
////
////															if (CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")) || CheckCSEL(nErection, _T("PL")) || CheckCSEL(nErection, _T("BL")))
////																Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor, LoadCaseNo, LcomD, TRUE);
////															else if (CheckCSEL(nErection, _T("D")))
////																Set_CStageCase(arCSLoad.GetAt(nnn), dDFactor, LoadCaseNo, LcomD, TRUE);
////
////														}
////														if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
////															Set_CStageCase(arCSLoad.GetAt(nnn), dCSFactor, LoadCaseNo, LcomD, TRUE);
////														if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
////															Set_CStageCase(arCSLoad.GetAt(nnn), dPsFactor, LoadCaseNo, LcomD, TRUE);
////														if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep Primary
////														{
////															Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
////														}
////														if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Creep secondary
////														{
////															Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
////														}
////														if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage Primary
////														{
////															Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
////														}
////														if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn) && m_bIRSCons_crp_shr_DT_ULS) // Shrinkage secondary
////														{
////															Set_CStageCase(m_aCSLoad.GetAt(nnn), dCpShFact, LoadCaseNo, LcomD, TRUE);
////														}
////
////													}
////													double dRS_X = (m % 2 == 0) ? dRsFactor : -1. * dRsFactor;
////													double dRS_Y = (n % 2 == 0) ? dRsFactor : -1. * dRsFactor;
////													double dRS_Z = (o % 2 == 0) ? dRsFactor : -1. * dRsFactor;
////													if (m % 2 == 0 || m % 2 == 1)
////													{
////														dRS_Y = 0.3 * dRS_Y;
////														dRS_Z = 0.3 * dRS_Z;
////													}
////													else if (n % 2 == 0)
////													{
////														dRS_X = 0.3 * dRS_X;
////														dRS_Z = 0.3 * dRS_Z;
////													}
////													else if (o % 2 == 0)
////													{
////														dRS_X = 0.3 * dRS_X;
////														dRS_Y = 0.3 * dRS_Y;
////
////													}
////													else if (m % 2 == 1 && n % 2 == 1 && o % 2 == 1)
////													{
////														dRS_Y = 0.3 * dRS_Y;
////														dRS_Z = 0.3 * dRS_Z;
////													}
////
////													for (ld = 0; ld < m_nECount; ++ld)
////													{
////														if (m_nECount == 1)
////														{
////															nES += Set_STLCIncludeCS(arELoad[ld], dEFactor_x, LoadCaseNo, LcomD, 0);
////														}
////														else
////														{
////															if (ld == 0)            nES += Set_STLCIncludeCS(arELoad[ld], dRS_Z, LoadCaseNo, LcomD, 0);
////															else if (ld % 2 == 1)     nES += Set_STLCIncludeCS(arELoad[ld], dRS_Y, LoadCaseNo, LcomD, 0);
////															else if (ld % 2 == 0)     nES += Set_STLCIncludeCS(arELoad[ld], dRS_X, LoadCaseNo, LcomD, 0);
////
////														}
////
////													}
////
////													if (arEspLoad_X.GetSize() > 0) Set_SpectrumCase(arEspLoad_X[o / 2], dRS_Z, LoadCaseNo, LcomD, TRUE);
////													if (arEspLoad_Y.GetSize() > 0) Set_SpectrumCase(arEspLoad_Y[n / 2], dRS_Y, LoadCaseNo, LcomD, TRUE);
////													if (arEspLoad_Z.GetSize() > 0) Set_SpectrumCase(arEspLoad_Z[m / 2], dRS_X, LoadCaseNo, LcomD, TRUE);
////
////
////													// description
////													LcomD.Description.Empty();
////													csPrefix.Empty();
////
////													LcomD.Description += _T("2S:");
////													if (nDL > 0)
////													{
////														LcomD.Description += csPrefix + _T("D");
////														csPrefix = _T(":");
////													}
////													if (nDc > 0 || nDw > 0 || nPL > 0 || nBL > 0)
////													{
////														LcomD.Description += csPrefix + _T("SIDL");
////														csPrefix = _T(":");
////													}
////													if (arEspLoad_X.GetSize() > 0)
////													{
////														LcomD.Description += csPrefix + GetFactorString(dRS_Z) + _T("RS(X)");
////														csPrefix = _T(":");
////													}
////													if (arEspLoad_Y.GetSize() > 0)
////													{
////														LcomD.Description += csPrefix + GetFactorString(dRS_Y) + _T("RS(Y)");
////														csPrefix = _T(":");
////													}
////													if (arEspLoad_Z.GetSize() > 0)
////													{
////														LcomD.Description += csPrefix + GetFactorString(dRS_X) + _T("RS(Z)");
////														csPrefix = _T(":");
////													}
////													if (m_nSmCount > 0 || nStl > 0)
////													{
////														LcomD.Description += csPrefix + _T("SM");
////														csPrefix = _T(":");
////													}
////													if (m_nMCount > 0)
////													{
////														LcomD.Description += csPrefix + _T("ML"); csPrefix = _T(":");
////													}
////													if (nLL > 0 || nCF > 0 || nBRK > 0 || nBK > 0)
////													{
////														LcomD.Description += csPrefix + _T("FPLL");
////														csPrefix = _T(":");
////													}
////													if (nEP > 0 || nEH > 0 || nEV > 0 || nES > 0 || nGWP > 0 || nLS > 0)
////													{
////														LcomD.Description += csPrefix; LcomD.Description += (l % 2 == 0) ? _T("EP") : _T("rEP");
////														csPrefix = _T(":");
////													}
////													if (m_nConstLoad != 0) // ST+CS
////													{
////														if (CheckCS_SgldK(D_SGLD_DL_CVL, nnn))
////														{
////															LcomD.Description += csPrefix + GetFactorString(dDFactor) + _T("(cD)"); csPrefix = _T("+");
////														}
////
////														CString strcEL;
////														int nErection = 0;
////
////														for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
////														{
////															strcEL = _T(""); nErection = 0;
////															if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nnn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }
////
////
////
////															if (CheckCSEL(nErection, _T("D")) || CheckCSEL(nErection, _T("DC")) || CheckCSEL(nErection, _T("DW")))
////															{
////																LcomD.Description += csPrefix + GetFactorString(dDFactor) + strcEL; csPrefix = _T("+");
////															}
////
////															else if (CheckCSEL(nErection, _T("W")))
////															{
////																LcomD.Description += csPrefix + GetFactorString(dWFactor) + strcEL; csPrefix = _T("+");
////															}
////														}
////
////														if (CheckCS_SgldK(D_SGLD_TP_CVL, nnn))
////														{
////															LcomD.Description += csPrefix + GetFactorString(dCSFactor) + _T("(cTP)"); csPrefix = _T("+");
////														}
////														if (CheckCS_SgldK(D_SGLD_TS_CVL, nnn))
////														{
////															LcomD.Description += csPrefix + GetFactorString(dPsFactor) + _T("(cTS)"); csPrefix = _T("+");
////														}
////														if (CheckCS_SgldK(D_SGLD_CP_CVL, nnn))
////														{
////															LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCP)"); csPrefix = _T("+");
////														}
////														if (CheckCS_SgldK(D_SGLD_CS_CVL, nnn))
////														{
////															LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cCS)"); csPrefix = _T("+");
////														}
////														if (CheckCS_SgldK(D_SGLD_SP_CVL, nnn))
////														{
////															LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSP)"); csPrefix = _T("+");
////														}
////														if (CheckCS_SgldK(D_SGLD_SS_CVL, nnn))
////														{
////															LcomD.Description += csPrefix + GetFactorString(dCpShFact) + _T("(cSS)"); csPrefix = _T("+");
////														}
////													}
////													if (LoadCaseNo > 0)
////													{
////														if (m_nLcomType != D_LCOMTYPE_GENERAL) LcomD.nActive = 2;
////														Set_LoadComb4Civil(LcomD, D_SER);
////													}
////
////												}
////											}
////										}
////									}
////								}
////							}
////						}
////					}
////				}
////			}
////
////
////		}
////	}
////
////
////}
//
////return TRUE;
//
}

void CLoadCombCtrl::Add_CvlTMH07_EQ(const EN_TMH07_NO& enLcomNo, ArINT& arELoad, ArUNT& arEspLoad, ArINT& arCSLoad, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom, BOOL bStrength,double& dGf3ULS,double& dGf3SLS)
{
	if (enLcomNo == EN_TMH07_NO_EQ_ST && m_nECount == 0) return;
   /* if (enLcomNo == EN_TMH07_NO_EQ_ST && m_nConstLoad == 1) return;*/
	else if (enLcomNo == EN_TMH07_NO_EQ_RS && m_nEspCount == 0) return;

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);
    double dEFactor;
	if (bStrength) {
		dEFactor = 1.3 * dGf3ULS;
	}
	else {
		dEFactor = 1.0 * dGf3SLS;
	}
	int nLcom = raNewLcom.GetSize();
	int nECountTemp = 1;
    if (enLcomNo == EN_TMH07_NO_EQ_ST) nECountTemp = (m_nECount > 0) ? m_nECount * 2 : 1;
	else if (enLcomNo == EN_TMH07_NO_EQ_RS) nECountTemp = (m_nEspCount > 0) ? m_nEspCount * 2 : 1;
	double dScaleUpFactor = 1.0;

	CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;

	for (int i = 0; i < nLcom; ++i)
	{
		for (int nEqm = 0; nEqm < nECountTemp; ++nEqm)
		{
			T_LCOM_D LcomD = raNewLcom[i];
			int nExistCase = LcomD.aCombination.GetSize();
			int nEL = 0, nCaseNum = nExistCase;
			double dEF = (nEqm % 2 == 0) ? dEFactor : -1. * dEFactor;
            if (enLcomNo == EN_TMH07_NO_EQ_ST)
			{
				nEL += Set_STLCIncludeCS(arELoad.GetAt(nEqm / 2), dEF, nCaseNum, LcomD, 0);
			}
			else if (enLcomNo == EN_TMH07_NO_EQ_RS)
			{
				Get_ScaleUpFactor(D_LOADCASE_SPECTRUM, arEspLoad.GetAt(nEqm / 2), dScaleUpFactor);
				Set_SpectrumCase(arEspLoad.GetAt(nEqm / 2), dEF * dScaleUpFactor, nCaseNum, LcomD, TRUE);
			}

			// Description

			CString csPrefix(_T("")), csLcom(_T(""));

			CString strFixE;
			strFixE.Format(_T("%d"), nEqm / 2 + 1);

            if (enLcomNo == EN_TMH07_NO_EQ_ST && nEL > 0)
			{
				csLcom += csPrefix + GetFactorString(dEF) + _T("E[") + strFixE + _T("]"); csPrefix = csPrefix_Tmp;
				if (dEF < 0.0)
				{
					csPrefix = _T("");
				}
			}
			else if (enLcomNo == EN_TMH07_NO_EQ_RS)
			{
				csLcom += csPrefix + GetFactorString(dEF * dScaleUpFactor) + _T("ESP[") + strFixE + _T("]"); csPrefix = csPrefix_Tmp;
				if (dEF < 0.0)
				{
					csPrefix = _T("");
				}
			}

			if (csLcom.IsEmpty() || LcomD.Description == strTitle)
			{
				csPrefix = _T("");
			}
			LcomD.Description += csPrefix + csLcom;

			if (nCaseNum > 0) raTotalLcom.Add(LcomD);
		}

	}
	if (raTotalLcom.GetSize() > 0)
	{
		raNewLcom.Copy(raTotalLcom);
	}
}

void CLoadCombCtrl::Add_CvlTMH07_EQ_Ortho(const EN_TMH07_NO& enLcomNo, ArINT& arELoad, ArUNT& arEspLoad, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom, BOOL bStrength,double& dGf3ULS,double& dGf3SLS)
{
	bool bMakeOrtho = true;
	CArray<T_LCOM_D, T_LCOM_D&> aNewLcom;
	aNewLcom.RemoveAll();
	const BOOL bOrtho = (m_nOrthoType == 0 ? TRUE : FALSE);
	const int  nOrtho = m_a100_30_Load.GetSize();
	if (!bOrtho) bMakeOrtho = false;
	else if (nOrtho == 0) bMakeOrtho = false;
	else if (enLcomNo != EN_TMH07_NO_EQ_ST && enLcomNo != EN_TMH07_NO_EQ_RS) bMakeOrtho = false;
    /*if (enLcomNo == EN_TMH07_NO_EQ_ST && m_nConstLoad == 1) bMakeOrtho = false;*/
    double dEFactor = 1.3;
    if (bStrength) {
        dEFactor = dEFactor * dGf3ULS;
    }
    else {
        dEFactor = dEFactor * dGf3SLS;
    }
	if (bOrtho)
	{
		if (enLcomNo == EN_TMH07_NO_EQ_ST && m_nECount == 0) bMakeOrtho = false;
		else if (enLcomNo == EN_TMH07_NO_EQ_RS && m_nEspCount == 0) bMakeOrtho = false;
	}
	if (!bOrtho)
	{
		/*Add_CvlTMH07_EQ(enLcomNo, arELoad, arEspLoad, csPrefix_Tmp, raNewLcom, bStrength);*/
		return;
	}

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nLcom = raNewLcom.GetSize();
	int nECountTemp = 1;
	if (enLcomNo == EN_TMH07_NO_EQ_ST) nECountTemp = (m_nECount > 0) ? m_nECount * 2 : 1;
	else if (enLcomNo == EN_TMH07_NO_EQ_RS) nECountTemp = (m_nEspCount > 0) ? m_nEspCount * 2 : 1;
	double dScaleUpFactor = 1.0;

	auto lambda_GetDefineOrthoLoad = [this](const int& iLCType, const int& iLCKey1, const int& iLCKey2, const int& iLCKey3,
		const int& nSubType, const int& nOrthoCombType,
		const double& dEFactor, CString& csPrefix, T_LCOM_D& rData, CArray<T_LCOM_D, T_LCOM_D&>& rAddLcom)
	{
		// Plus sign only (+,+) or Both (+,+)(+,-)(-,-)(-,+)
		const int nSize = (nOrthoCombType == COMBTYPE_BOTH) ? 2 : 1;
		const int nSizeV = iLCKey3 > 0 ? nSize : 1;
		// 100:30 Factor (Case 3)
		double dLc1Fac[2], dLc2Fac[2], dLc3Fac[2];
		CString sLc1Fac, sLc2Fac, sLc3Fac;
		switch (nSubType)
		{
		case LONGI_TYPE:
			dLc1Fac[0] = 1.0; dLc1Fac[1] = -1.0; // Govern
			dLc2Fac[0] = 0.3; dLc2Fac[1] = -0.3;
			dLc3Fac[0] = 0.3; dLc3Fac[1] = -0.3;
			sLc1Fac = _T("1.0");
			sLc2Fac = _T("0.3");
			sLc3Fac = _T("0.3");
			break;
		case TRANS_TYPE:
			dLc1Fac[0] = 0.3; dLc1Fac[1] = -0.3;
			dLc2Fac[0] = 1.0; dLc2Fac[1] = -1.0; // Govern      
			dLc3Fac[0] = 0.3; dLc3Fac[1] = -0.3;
			sLc1Fac = _T("0.3");
			sLc2Fac = _T("1.0");
			sLc3Fac = _T("0.3");
			break;
		case VERTI_TYPE:
			dLc1Fac[0] = 0.3; dLc1Fac[1] = -0.3;
			dLc2Fac[0] = 0.3; dLc2Fac[1] = -0.3;
			dLc3Fac[0] = 1.0; dLc3Fac[1] = -1.0; // Govern
			sLc1Fac = _T("0.3");
			sLc2Fac = _T("0.3");
			sLc3Fac = _T("1.0");
			break;
		default:
			break;
		}
		const CString sLcEQ = iLCType == 0 ? _T("(ST)") : _T("(RS)");
		const CString sLc1Name = iLCType == 0 ? Get_StaticLoadCaseName(iLCKey1) : Get_SpectrumLoadCaseName(iLCKey1);
		const CString sLc2Name = iLCType == 0 ? Get_StaticLoadCaseName(iLCKey2) : Get_SpectrumLoadCaseName(iLCKey2);
		const CString sLc3Name = iLCKey3 > 0 ? (iLCType == 0 ? Get_StaticLoadCaseName(iLCKey3) : Get_SpectrumLoadCaseName(iLCKey3)) : _T("");;
		double dScaleUpFactor1 = 1.0, dScaleUpFactor2 = 1.0, dScaleUpFactor3 = 1.0;
		if (iLCType == 1)
		{
			if (Get_ScaleUpFactor(D_LOADCASE_SPECTRUM, iLCKey1, dScaleUpFactor1))
			{
				sLc1Fac.Format(_T("(1.0*%s)"), GetFactorString(dScaleUpFactor1));
			}
			if (Get_ScaleUpFactor(D_LOADCASE_SPECTRUM, iLCKey2, dScaleUpFactor2))
			{
				sLc2Fac.Format(_T("(0.3*%s)"), GetFactorString(dScaleUpFactor2));
			}
			if (iLCKey3 > 0 && Get_ScaleUpFactor(D_LOADCASE_SPECTRUM, iLCKey3, dScaleUpFactor3))
			{
				sLc3Fac.Format(_T("(0.3*%s)"), GetFactorString(dScaleUpFactor3));
			}
		}
		for (int nLC1 = 0; nLC1 < nSize; ++nLC1)
		{
			for (int nLC2 = 0; nLC2 < nSize; ++nLC2)
			{
				for (int nLC3 = 0; nLC3 < nSizeV; ++nLC3)
				{
					T_LCOM_D rData_Temp = rData;
					int LoadCaseNo_Temp = rData_Temp.aCombination.GetSize();
					CString Sign1 = nLC1 == 0 ? _T("") : _T("-");
					CString Sign2 = nLC2 == 0 ? _T("+") : _T("-");
					CString Sign3 = nLC3 == 0 ? _T("+") : _T("-");
					if (iLCType == 1)
					{
						Set_SpectrumCase(iLCKey1, dEFactor * dScaleUpFactor1 * dLc1Fac[nLC1], LoadCaseNo_Temp, rData_Temp, TRUE);
						Set_SpectrumCase(iLCKey2, dEFactor * dScaleUpFactor2 * dLc2Fac[nLC2], LoadCaseNo_Temp, rData_Temp, TRUE);
						if (iLCKey3 > 0) Set_SpectrumCase(iLCKey3, dEFactor * dScaleUpFactor3 * dLc3Fac[nLC3], LoadCaseNo_Temp, rData_Temp, TRUE);
					}
					else
					{
						Set_StaticCase(iLCKey1, dEFactor * dLc1Fac[nLC1], LoadCaseNo_Temp, rData_Temp);
						Set_StaticCase(iLCKey2, dEFactor * dLc2Fac[nLC2], LoadCaseNo_Temp, rData_Temp);
						if (iLCKey3 > 0) Set_StaticCase(iLCKey3, dEFactor * dLc3Fac[nLC3], LoadCaseNo_Temp, rData_Temp);
					}
					rData_Temp.Description += csPrefix + GetFactorString(dEFactor) + _T("[") + Sign1 + sLc1Fac + sLc1Name + sLcEQ;
					rData_Temp.Description += Sign2 + sLc2Fac + sLc2Name + sLcEQ;
					if (iLCKey3 > 0) rData_Temp.Description += Sign3 + sLc3Fac + sLc3Name + sLcEQ + _T("]");
					else           rData_Temp.Description += _T("]");
					// 
					if (LoadCaseNo_Temp > rData.aCombination.GetSize())
					{
						if (m_nLcomType != D_LCOMTYPE_GENERAL) rData_Temp.nActive = 1;
						rData_Temp.nSeisLcomType = nSubType;
						rAddLcom.Add(rData_Temp);
					}
				}
			}
		}
	};

	CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;
	raTotalLcom.RemoveAll();
	for (int i = 0; i < nLcom; ++i)
	{
		T_LCOM_D LcomD = raNewLcom[i];

		const int nOrthoCombType = Get_OrthoCombType();

		for (int i = 0; i < nOrtho; ++i)
		{
			const int iLCKey1 = m_a100_30_Load[i].iLCKey1;
			const int iLCKey2 = m_a100_30_Load[i].iLCKey2;
			const int iLCKey3 = m_a100_30_Load[i].bLCase3 ? m_a100_30_Load[i].iLCKey3 : 0;

			lambda_GetDefineOrthoLoad(m_a100_30_Load[i].nLoadCaseType, iLCKey1, iLCKey2, iLCKey3, LONGI_TYPE, nOrthoCombType, dEFactor, csPrefix_Tmp, LcomD, raTotalLcom);
			lambda_GetDefineOrthoLoad(m_a100_30_Load[i].nLoadCaseType, iLCKey1, iLCKey2, iLCKey3, TRANS_TYPE, nOrthoCombType, dEFactor, csPrefix_Tmp, LcomD, raTotalLcom);
			if (iLCKey3 > 0) lambda_GetDefineOrthoLoad(m_a100_30_Load[i].nLoadCaseType, iLCKey1, iLCKey2, iLCKey3, VERTI_TYPE, nOrthoCombType, dEFactor, csPrefix_Tmp, LcomD, raTotalLcom);
		}
	}
	if (raTotalLcom.GetSize() > 0)
	{
		raNewLcom.Copy(raTotalLcom);
	}
}

// Add Title
void CLoadCombCtrl::Add_CvlTMH07_Title(const EN_TMH07_NO& enLcomNo, BOOL bStrength, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nLcom = raNewLcom.GetSize();
	if (nLcom == 0)
	{
		T_LCOM_D NewLcomD;
		NewLcomD.Initialize();
		raNewLcom.Add(NewLcomD);
		nLcom = 1;
	}

	CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

	for (int i = 0; i < nLcom; ++i)
	{
		T_LCOM_D LcomD = raNewLcom[i];
		// Description

		CString csPrefix(_T("")), csLcom(_T(""));
		LcomD.Description += strTitle; // 맨 처음일 때만 필요
		raTotalLcom.Add(LcomD);

	}
	raNewLcom.Copy(raTotalLcom);
}

// Add D Load
void CLoadCombCtrl::Add_CvlTMH07_D(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arDLoad, double& dD1, double& dD2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
	if (m_nDCount == 0) return;

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	double dDFac[2];
    dDFac[0] = dD1;
    dDFac[1] = dD2;
   /* if (enLcomNo == EN_TMH07_NO_02_01) {
	dDFac[0] = dD2;
	dDFac[1] = dD2;
	}
	else {
		dDFac[0] = dD1;
		dDFac[1] = dD1;
	}*/

	int nLcom = raNewLcom.GetSize();

	CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

	for (int i = 0; i < nLcom; ++i)
	{
  		for (int nD = 0; nD < m_nBD21DNum; ++nD)
		{
			T_LCOM_D LcomD = raNewLcom[i];
			_LCOM_BOTH BothD; BothD.Initialize();
			int nExistCase = LcomD.aCombination.GetSize();
			int nDL = 0, nCaseNum = nExistCase;
			for (int j = 0; j < m_nDCount; ++j) { nDL += Set_STLCIncludeCS(arDLoad.GetAt(j), dDFac[nD], nCaseNum, LcomD, D_SGLD_DL_CVL); (nD == 1) ? BothD.nBoth[nCaseNum] = 1 : BothD.nBoth[nCaseNum] = 0; }

			// Description
			if (nDL > 0)
			{
				CString csPrefix(_T("")), csLcom(_T(""));
				csLcom += csPrefix + GetFactorString(dDFac[nD]) + _T("D"); csPrefix = _T("+");
				if (csLcom.IsEmpty() || LcomD.Description == strTitle)
				{
					csPrefix = _T("");
				}
				LcomD.Description += csPrefix + csLcom;
			}

			if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
		}
	}

	if (raTotalLcom.GetSize() > 0)
	{
		raNewLcom.Copy(raTotalLcom);
	}
}

void CLoadCombCtrl::Add_CvlTMH07_DW(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arDwLoad, double& dDw1, double& dDw2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
	if (m_nDwCount == 0) return;

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	double dDwFac[2];
   /* if (enLcomNo == EN_TMH07_NO_02_01) {
		dDwFac[0] = dDw2;
		dDwFac[1] = dDw2;
    }
    else {
		dDwFac[0] = dDw1;
		dDwFac[1] = dDw1;
    }*/
    dDwFac[0] = dDw1;
    dDwFac[1] = dDw2;

	int nLcom = raNewLcom.GetSize();

	CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

	for (int i = 0; i < nLcom; ++i)
	{
		for (int nDw = 0; nDw < m_nBD21DWNum; ++nDw)
		{
			T_LCOM_D LcomD = raNewLcom[i];
			_LCOM_BOTH BothD; BothD.Initialize();
			int nExistCase = LcomD.aCombination.GetSize();
			int nDwL = 0, nCaseNum = nExistCase;
			for (int j = 0; j < m_nDwCount; ++j) { nDwL += Set_STLCIncludeCS(arDwLoad.GetAt(j), dDwFac[nDw], nCaseNum, LcomD, D_SGLD_DL_CVL); (nDw == 1) ? BothD.nBoth[nCaseNum] = 1 : BothD.nBoth[nCaseNum] = 0; }

			// Description
			if (nDwL > 0)
			{
				CString csPrefix(_T("")), csLcom(_T(""));
				csLcom += csPrefix + GetFactorString(dDwFac[nDw]) + _T("DW"); csPrefix = _T("+");
				if (csLcom.IsEmpty() || LcomD.Description == strTitle)
				{
					csPrefix = _T("");
				}
				LcomD.Description += csPrefix + csLcom;
			}

			if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
		}
	}
	if (raTotalLcom.GetSize() > 0)
	{
		raNewLcom.Copy(raTotalLcom);
	}
}

void CLoadCombCtrl::Add_CvlTMH07_DC(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arDcLoad, double& dDc1, double& dDc2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
	if (m_nDcCount == 0) return;

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	double dDcFac[2];
	/*if (enLcomNo == EN_TMH07_NO_02_01) {
		dDcFac[0] = dDc2;
		dDcFac[1] = dDc2;
	}
	else {
		dDcFac[0] = dDc1;
		dDcFac[1] = dDc1;
	}*/
    dDcFac[0] = dDc1;
    dDcFac[1] = dDc2;
	int nLcom = raNewLcom.GetSize();

	CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

	for (int i = 0; i < nLcom; ++i)
	{
		for (int nDc = 0; nDc < m_nBD21DCNum; ++nDc)
		{
			T_LCOM_D LcomD = raNewLcom[i];
			_LCOM_BOTH BothD; BothD.Initialize();
			int nExistCase = LcomD.aCombination.GetSize();
			int nDcL = 0, nCaseNum = nExistCase;
			for (int j = 0; j < m_nDcCount; ++j) { nDcL += Set_STLCIncludeCS(arDcLoad.GetAt(j), dDcFac[nDc], nCaseNum, LcomD, D_SGLD_DL_CVL); (nDc == 1) ? BothD.nBoth[nCaseNum] = 1 : BothD.nBoth[nCaseNum] = 0; }

			// Description
			if (nDcL > 0)
			{
				CString csPrefix(_T("")), csLcom(_T(""));
				csLcom += csPrefix + GetFactorString(dDcFac[nDc]) + _T("DC"); csPrefix = _T("+");
				if (csLcom.IsEmpty() || LcomD.Description == strTitle)
				{
					csPrefix = _T("");
				}
				LcomD.Description += csPrefix + csLcom;
			}

			if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
		}
	}
	if (raTotalLcom.GetSize() > 0)
	{
		raNewLcom.Copy(raTotalLcom);
	}
}

void CLoadCombCtrl::Add_CvlTMH07_EV(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arEvLoad, double& dEv1, double& dEv2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom, double& m_dEPRelieving,double& dGf3ULS,double& dGf3SLS)
{
	/*if (enLcomNo == EN_TMH07_NO_02_01) return;*/
	if (m_nEvCount == 0) return;

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	double dEvFac[2];
	if (enLcomNo == EN_TMH07_NO_02_01)
	{
		dEvFac[0] = dEv2;
		dEvFac[1] = dEv2;
	}
	else
	{
		dEvFac[0] = dEv1;
		dEvFac[1] = dEv1;
	}
	/*dEvFac[0] = dEv1;
	dEvFac[1] = dEv2;*/

	int nLcom = raNewLcom.GetSize();

	CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;
    for (int j = 0; j < 2; j++) {
        if (j == 1) {
            if (bStrength) {
                dEvFac[0] = m_dEPRelieving * dGf3ULS;
                dEvFac[1] = m_dEPRelieving * dGf3ULS;
            }
            else {
				dEvFac[0] = m_dEPRelieving * dGf3SLS;
				dEvFac[1] = m_dEPRelieving * dGf3SLS;
            }
        }

        for (int i = 0; i < nLcom; ++i)
        {
            for (int nEv = 0; nEv < m_nBD21EVNum; ++nEv)
            {
                T_LCOM_D LcomD = raNewLcom[i];
                _LCOM_BOTH BothD; BothD.Initialize();
                int nExistCase = LcomD.aCombination.GetSize();
                int nEvL = 0, nCaseNum = nExistCase;
                for (int j = 0; j < m_nEvCount; ++j) { nEvL += Set_STLCIncludeCS(arEvLoad.GetAt(j), dEvFac[nEv], nCaseNum, LcomD, 0); (nEv == 1) ? BothD.nBoth[nCaseNum] = 1 : BothD.nBoth[nCaseNum] = 0; }
                // Description
                if (nEvL > 0)
                {
                    CString csPrefix(_T("")), csLcom(_T(""));
                    csLcom += csPrefix + GetFactorString(dEvFac[nEv]) + _T("EV"); csPrefix = _T("+");
                    if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                    {
                        csPrefix = _T("");
                    }
                    LcomD.Description += csPrefix + csLcom;
                }

                if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
                //// Duplicate combination with factor 0.85
             /*   T_LCOM_D LcomD_85 = LcomD;
                for (int j = 0; j < m_nEvCount; ++j) {
                    nEvL += Set_STLCIncludeCS(arEvLoad.GetAt(j), dEvFac[nEv] * 0.85, nCaseNum, LcomD_85, 0);
                    (nEv == 1) ? BothD.nBoth[nCaseNum] = 1 : BothD.nBoth[nCaseNum] = 0;
                }
                if (nEvL > 0)
                {
                    CString csLcom85;
                    csLcom85.Format(_T("+0.85EV"));
                    LcomD_85.Description += csLcom85;
                }

                if (nCaseNum > 0)    raTotalLcom.Add(LcomD_85);*/
            }
        }
    }
	if (raTotalLcom.GetSize() > 0)
	{
		raNewLcom.Copy(raTotalLcom);
	}
}

void CLoadCombCtrl::Add_CvlTMH07_EH(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arEhLoad, double& dEh1, double& dEh2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
	if (enLcomNo == EN_TMH07_NO_02_01) return;
	if (m_nEhCount == 0) return;

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	double dEhFac[2];
	dEhFac[0] = dEh1;
	dEhFac[1] = dEh2;

	int nLcom = raNewLcom.GetSize();

	CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;
    for (int i = 0; i < 2; i++) {
        if (i == 1) {
            dEhFac[0] = 0.85;
            dEhFac[1] = 0.85;
        }
        for (int i = 0; i < nLcom; ++i)
        {
            for (int nEh = 0; nEh < m_nBD21EHNum; ++nEh)
            {
                T_LCOM_D LcomD = raNewLcom[i];
                _LCOM_BOTH BothD; BothD.Initialize();
                int nExistCase = LcomD.aCombination.GetSize();
                int nEhL = 0, nCaseNum = nExistCase;
                for (int j = 0; j < m_nEhCount; ++j) { nEhL += Set_STLCIncludeCS(arEhLoad.GetAt(j), dEhFac[nEh], nCaseNum, LcomD, 0); (nEh == 1) ? BothD.nBoth[nCaseNum] = 1 : BothD.nBoth[nCaseNum] = 0; }
                // Description
                if (nEhL > 0)
                {
                    CString csPrefix(_T("")), csLcom(_T(""));
                    csLcom += csPrefix + GetFactorString(dEhFac[nEh]) + _T("EH"); csPrefix = _T("+");
                    if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                    {
                        csPrefix = _T("");
                    }
                    LcomD.Description += csPrefix + csLcom;
                }

                if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
            }
        }
    }
	if (raTotalLcom.GetSize() > 0)
	{
		raNewLcom.Copy(raTotalLcom);
	}
}
void CLoadCombCtrl::Add_CvlTMH07_B(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arBLoad, double& dB1, double& dB2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom, double& m_dEPRelieving, double& dGf3ULS, double& dGf3SLS)
{
    /*if (enLcomNo == EN_TMH07_NO_02_01) return;*/
    if (m_nBCount == 0) return;

    CString strTitle;
    strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

    double dBFac[2];
	if (enLcomNo == EN_TMH07_NO_02_01)
	{
		dBFac[0] = dB2;
		dBFac[1] = dB2;
	}
	else
	{
		dBFac[0] = dB1;
		dBFac[1] = dB1;
	}
    if (bStrength) {
        dBFac[0] = dBFac[0] * dGf3ULS;
        dBFac[1] = dBFac[1] * dGf3ULS;
    }
    else {
		dBFac[0] = dBFac[0] * dGf3SLS;
		dBFac[1] = dBFac[1] * dGf3SLS;
    }

    int nLcom = raNewLcom.GetSize();
    int r_loop;
    if (bStrength) {
        r_loop = 2;
    }
    else {
        r_loop = 1;
    }
    CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;
    for (int j = 0; j < r_loop; j++) {
        if (j == 1) {
            m_dEPRelieving = 1;
            if (bStrength) {
                dBFac[0] = m_dEPRelieving * dGf3ULS;
                dBFac[1] = m_dEPRelieving * dGf3ULS;
            }
            else {
				dBFac[0] = m_dEPRelieving * dGf3SLS;
				dBFac[1] = m_dEPRelieving * dGf3SLS;
            }
        }

        for (int i = 0; i < nLcom; ++i)
        {
            for (int nB = 0; nB < m_nBD21EVNum; ++nB)
            {
                T_LCOM_D LcomD = raNewLcom[i];
                _LCOM_BOTH BothD; BothD.Initialize();
                int nExistCase = LcomD.aCombination.GetSize();
                int nBL = 0, nCaseNum = nExistCase;
                for (int j = 0; j < m_nBCount; ++j) { nBL += Set_STLCIncludeCS(arBLoad.GetAt(j), dBFac[nB], nCaseNum, LcomD, 0); (nB == 1) ? BothD.nBoth[nCaseNum] = 1 : BothD.nBoth[nCaseNum] = 0; }
                // Description
                if (nBL > 0)
                {
                    CString csPrefix(_T("")), csLcom(_T(""));
                    csLcom += csPrefix + GetFactorString(dBFac[nB]) + _T("WP"); csPrefix = _T("+");
                    if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                    {
                        csPrefix = _T("");
                    }
                    LcomD.Description += csPrefix + csLcom;
                }

                if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
                //// Duplicate combination with factor 0.85
             /*   T_LCOM_D LcomD_85 = LcomD;
                for (int j = 0; j < m_nEvCount; ++j) {
                    nEvL += Set_STLCIncludeCS(arEvLoad.GetAt(j), dEvFac[nEv] * 0.85, nCaseNum, LcomD_85, 0);
                    (nEv == 1) ? BothD.nBoth[nCaseNum] = 1 : BothD.nBoth[nCaseNum] = 0;
                }
                if (nEvL > 0)
                {
                    CString csLcom85;
                    csLcom85.Format(_T("+0.85EV"));
                    LcomD_85.Description += csLcom85;
                }

                if (nCaseNum > 0)    raTotalLcom.Add(LcomD_85);*/
            }
        }
    }
    if (raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}
void CLoadCombCtrl::Add_CvlTMH07_STL_SM(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arStlLoad, ArUNT& arSmLoad, double& dStlFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
	/*if (enLcomNo == EN_TMH07_NO_02_01) return;*/
	if (m_nSmCount == 0 && m_nStlCount == 0) return;

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nSmCountTemp = max(m_nSmCount, 1);

	int nLcom = raNewLcom.GetSize();

	CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

	for (int i = 0; i < nLcom; ++i)
	{
		for (int nSm = 0; nSm < nSmCountTemp; ++nSm)
		{
			T_LCOM_D LcomD = raNewLcom[i];
			_LCOM_BOTH BothD; BothD.Initialize();
			int nExistCase = LcomD.aCombination.GetSize();
			int nStL = 0, nCaseNum = nExistCase;
            if (m_nConstLoad != 1) {
                for (int j = 0; j < m_nStlCount; ++j) nStL += Set_STLCIncludeCS(arStlLoad.GetAt(j), dStlFactor, nCaseNum, LcomD, 0);
            }
			if (m_nSmCount > 0)                        Set_SettleCase(arSmLoad.GetAt(nSm), dStlFactor, nCaseNum, LcomD, TRUE);
			// Description
			if (nStL > 0 || m_nSmCount > 0)
			{
				CString csPrefix(_T("")), csLcom(_T(""));
				CString strFixSm;
				strFixSm.Format(_T("%d"), nSm + 1);
				if (nStL > 0 && m_nConstLoad != 1) { csLcom += csPrefix + GetFactorString(dStlFactor) + _T("STL"); csPrefix = _T("+"); }
				if (m_nSmCount > 0) { csLcom += csPrefix + GetFactorString(dStlFactor) + _T("SM[" + strFixSm + _T("]")); csPrefix = _T("+"); }
				if (csLcom.IsEmpty() || LcomD.Description == strTitle)
				{
					csPrefix = _T("");
				}
				LcomD.Description += csPrefix + csLcom;
			}

			if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
		}
	}
	if (raTotalLcom.GetSize() > 0)
	{
		raNewLcom.Copy(raTotalLcom);
	}
}

void CLoadCombCtrl::Add_CvlTMH07_PS_CR_SH(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, ArINT& arCSLoad, double& dPsFactor, double& dCRSHFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom, double& m_dEPRelieving, double& dGf3ULS, double& dGf3SLS)
{
    if (m_nConstLoad == 1) return;
    if (m_nPsCount == 0 && m_nCrCount == 0 && m_nShCount == 0) return;

    CString strTitle;
    strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

    int nLcom = raNewLcom.GetSize();
    int r_loop;
    if (bStrength) {
        r_loop = 2;
    }
    else {
        r_loop = 1;
    }

    CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;
    for (int j = 0; j < r_loop; ++j) {
    for (int i = 0; i < nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        _LCOM_BOTH BothD; BothD.Initialize();
        int nExistCase = LcomD.aCombination.GetSize();
        int nPsL = 0, nCrL = 0, nShL = 0, nCaseNum = nExistCase;
        int nn = 0;
        if (j == 1) {
            if (bStrength) {
                dPsFactor = 0.87 * dGf3ULS;
            }
            else {
                dPsFactor = 0.87 * dGf3SLS;
            }
        }
        for (int j = 0; j < m_nPsCount; ++j) nPsL += Set_STLCIncludeCS(arPsLoad.GetAt(j), dPsFactor, nCaseNum, LcomD, 0);
        for (int j = 0; j < m_nCrCount; ++j) nCrL += Set_STLCIncludeCS(arCrLoad.GetAt(j), dCRSHFactor, nCaseNum, LcomD, 0);
        for (int j = 0; j < m_nShCount; ++j) nShL += Set_STLCIncludeCS(arShLoad.GetAt(j), dCRSHFactor, nCaseNum, LcomD, 0);
        if (m_nConstLoad != 0)
        {
            // Check for D_SGLD_TP_CVL and add cTP
            if (CheckCS_SgldK(D_SGLD_TP_CVL, nn))
            {
                Set_CStageCase(arCSLoad.GetAt(nn), dPsFactor, nCaseNum, LcomD, TRUE);
            }
        }
        
        // Description
        if (nPsL > 0 || nCrL > 0 || nShL > 0)
        {
            CString csPrefix(_T("")), csLcom(_T(""));
            if (nPsL > 0) { csLcom += csPrefix + GetFactorString(dPsFactor) + _T("PS"); csPrefix = _T("+"); }
            if (nCrL > 0) { csLcom += csPrefix + GetFactorString(dCRSHFactor) + _T("CR"); csPrefix = _T("+"); }
            if (nShL > 0) { csLcom += csPrefix + GetFactorString(dCRSHFactor) + _T("SH"); csPrefix = _T("+"); }
            if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_TP_CVL, nn))
            {
                csLcom += csPrefix + GetFactorString(dPsFactor) + _T("(cTP)");
                csPrefix = _T("+");
            }
            
            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;
        }
        if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
    }
}
	if (raTotalLcom.GetSize() > 0)
	{
		raNewLcom.Copy(raTotalLcom);
	}
    if (bStrength) {
        dPsFactor = 1.15 * dGf3ULS;
    }
    else {
        dPsFactor = 1.0 * dGf3SLS;
    }
}

void CLoadCombCtrl::Add_CvlTMH07_M(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArUNT& arMLoad, ArINT& arLLoad, ArUNT& arCombinedLoad, double& dMFactor, double& dGf3ULS, double& dGf3SLS, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if (enLcomNo == EN_TMH07_NO_EQ_ST || enLcomNo == EN_TMH07_NO_EQ_RS)return;
    int count = 0;
    m_nMCount = arCombinedLoad.GetSize();
	if (m_nMCount == 0) return;

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	/*int nMCount = max(m_nMCount, 1);*/

	int nLcom = raNewLcom.GetSize();

	double dGf3 = 0.0;
	double dPede = 0.0;

	if (bStrength)
	{
		dGf3 = dGf3ULS;
		//if (enLcomNo == EN_TMH07_NO_01)
		//{
		//	dPede = 1.5; //Pedestrian 인 경우 ULS에서 Moving Factor는 1.5
		//}
		//else
		//{
		//	dPede = 1.25; //Pedestrian 인 경우 ULS에서 Moving Factor는 1.25
		//}
	}
	else
	{
		dGf3 = dGf3SLS;
		dPede = 1.0; //Pedestrian 인 경우 SLS에서 Moving Factor는 1.0
	}

	CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

	//for (int i = 0; i < nLcom; ++i)
	//{
	//	for (int nMv = 0; nMv < nMCount; ++nMv)
	//	{
	//		T_LCOM_D LcomD = raNewLcom[i];
 //           _LCOM_BOTH BothD; BothD.Initialize();
	//		int nExistCase = LcomD.aCombination.GetSize();
	//		int nCaseNum = nExistCase;
	//		BOOL bMoving = FALSE;
	//		//if (m_nBD21BridgeType == 0) //RoadBridge
	//		//{
	//			if (m_nMCount > 0) bMoving = Exist_MovingLoadData(arCombinedLoad.GetAt(nMv));
 //               if (!bMoving) {
 //                   continue;
 //               }
	//			T_MVLDbs_D rDatabs;
 //              
	//			m_pDoc->m_pAttrCtrl->GetMvldbs(arCombinedLoad.GetAt(nMv), rDatabs);
 //            
	//			if (rDatabs.bAutoLiveLoadComb)
	//			{
	//				if (bStrength && rDatabs.nDgnCombFactorType == 1)
	//				{
	//					bMoving = FALSE;
	//				}

	//				if (!bStrength && rDatabs.nDgnCombFactorType == 0)
	//				{
	//					bMoving = FALSE;
	//				}
	//			}

	//			if (bMoving) dMFactor = Get_BSMovingLoadFactorData(arCombinedLoad.GetAt(nMv), strTitle);
	//			if (bMoving) dMFactor = dGf3 * dMFactor;
	//			if (bMoving) Set_MovingCase(arCombinedLoad.GetAt(nMv), dMFactor, nCaseNum, LcomD, TRUE);
	//			/*	}
	//				else
	//				{
	//					dMFactor = dGf3 * dPede;
	//					if (m_nMCount > 0) Set_MovingCase(arMLoad.GetAt(nMv), dMFactor, nCaseNum, LcomD, TRUE);
	//				}*/
	//		// Description

	//		CString csPrefix(_T("")), csLcom(_T(""));
	//		CString strFixM;
	//		strFixM.Format(_T("%d"), nMv + 1);

	//		//if (m_nBD21BridgeType == 0) //RoadBridge
	//		//{
	//			if (bMoving && m_nMCount > 0)
	//			{
	//				csLcom += csPrefix + GetFactorString(dMFactor) + _T("M[") + strFixM + _T("]");  csPrefix = _T("+");
	//				csPrefix = LcomD.Description.IsEmpty() ? _T("") : _T("+");
	//				LcomD.Description += csPrefix + csLcom;
	//			}
	//		/*}*/
	//		//else //Pedestrian
	//		//{
	//		//	if (m_nMCount > 0)
	//		//	{
	//		//		csLcom += csPrefix + GetFactorString(dMFactor) + _T("M[") + strFixM + _T("]");  csPrefix = _T("+");
	//		//		csPrefix = LcomD.Description.IsEmpty() ? _T("") : _T("+");
	//		//		LcomD.Description += csPrefix + csLcom;
	//		//	}
	//		//}
	//		if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
	//	}
	//}
	for (int i = 0; i < nLcom; ++i)
	{
		T_LCOM_D LcomD = raNewLcom[i];
		_LCOM_BOTH BothD; BothD.Initialize();
		int nExistCase = LcomD.aCombination.GetSize();
		for (int nMv = 0; nMv < m_nMCount; ++nMv)
		{
			int nCaseNum = nExistCase;
			BOOL bMoving = FALSE;

			if (m_nMCount > 0) bMoving = Exist_MovingLoadData(arCombinedLoad.GetAt(nMv));
			if (!bMoving) {
				continue;
			}

			T_MVLDbs_D rDatabs;

			if (bMoving) dMFactor = Get_TMHMovingLoadFactorData(arCombinedLoad.GetAt(nMv), strTitle);
			if (bMoving) dMFactor = dGf3 * dMFactor;
			
            if (bMoving) {
                // Check if arMload is present inside arCombinedLoad
                bool isMloadPresent = false;
                bool isLsLoadPresent = false;
                /*	for (int idx = 0; idx < arCombinedLoad.GetSize(); ++idx)
                    {*/
                for (int j = 0; j < arMLoad.GetSize(); ++j)
                {
                    if (arCombinedLoad.GetAt(nMv) == arMLoad.GetAt(j))
                    {
                        isMloadPresent = true;
                        break;
                    }
                }
				for (int k = 0; k < arLLoad.GetSize(); ++k)
				{
					if (arCombinedLoad.GetAt(nMv) == arLLoad.GetAt(k))
					{
						isLsLoadPresent = true;
						break;
					}
				}
				/* if (isLsLoadPresent && isMloadPresent) {
					 if (count == 0) {
						 isLsLoadPresent = false;
						 count++;
					 }
					 else {
						 isMloadPresent = false;
					 }
				 }*/

                if (isMloadPresent)
                {
                    m_pDoc->m_pAttrCtrl->GetMvldbs(arCombinedLoad.GetAt(nMv), rDatabs);
                    if (bMoving) Set_MovingCase(arCombinedLoad.GetAt(nMv), dMFactor, nCaseNum, LcomD, TRUE);
                    // Description
                    CString csPrefix(_T("")), csLcom(_T(""));
                    CString strFixM;
                    strFixM.Format(_T("%d"), nMv + 1);

                    if (bMoving && m_nMCount > 0)
                    {
                        csLcom += csPrefix + GetFactorString(dMFactor) + _T("M[") + strFixM + _T("]");  csPrefix = _T("+");
                        csPrefix = LcomD.Description.IsEmpty() ? _T("") : _T("+");
                        LcomD.Description += csPrefix + csLcom;
                    }
                }
                
                    if (isLsLoadPresent && m_nConstLoad != 1)
                    {
                        int nEsL = 0;
                        /*for (int j = 0; j < m_nEsCount; ++j)
                        {*/
                        nEsL += Set_STLCIncludeCS(arCombinedLoad.GetAt(nMv), dMFactor, nCaseNum, LcomD, D_SGLD_DL_CVL);
                        (nMv == 1) ? BothD.nBoth[nCaseNum] = 1 : BothD.nBoth[nCaseNum] = 0;
                        /*}*/

                        // Description   
                        if (nEsL > 0)
                        {
                            CString csPrefix(_T("")), csLcom(_T(""));
                            csLcom += csPrefix + GetFactorString(dMFactor) + _T("L");
                            csPrefix = _T("+");
                            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                            {
                                csPrefix = _T("");
                            }
                            LcomD.Description += csPrefix + csLcom;
                        }
                    }
                
                if (nCaseNum > 0) raTotalLcom.Add(LcomD);
            }
           
		}
	}


	if (raTotalLcom.GetSize() > 0)
	{
		raNewLcom.Copy(raTotalLcom);
	}
}
//void CLoadCombCtrl::Add_CvlTMH07_M(
//    const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArUNT& arMLoad, double& dMFactor,
//    double& dGf3ULS, double& dGf3SLS, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
//{
//    if (enLcomNo == EN_TMH07_NO_EQ_ST || enLcomNo == EN_TMH07_NO_EQ_RS) return;
//    m_nMCount = arMLoad.GetSize();
//    if (m_nMCount == 0) return;
//
//    CString strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);
//    int nLcom = raNewLcom.GetSize();
//
//    double dGf3 = bStrength ? dGf3ULS : dGf3SLS;
//
//    CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;
//
//    for (int i = 0; i < nLcom; ++i)
//    {
//        T_LCOM_D LcomD = raNewLcom[i];
//        int nExistCase = LcomD.aCombination.GetSize();
//        for (int nMv = 0; nMv < m_nMCount; ++nMv)
//        {
//            int nCaseNum = nExistCase;
//            BOOL bMoving = Exist_MovingLoadData(arMLoad.GetAt(nMv));
//            if (!bMoving) continue;
//
//            T_MVLDbs_D rDatabs;
//            dMFactor = Get_BSMovingLoadFactorData(arMLoad.GetAt(nMv), strTitle);
//            dMFactor = dGf3 * dMFactor;
//
//            m_pDoc->m_pAttrCtrl->GetMvldbs(arMLoad.GetAt(nMv), rDatabs);
//            Set_MovingCase(arMLoad.GetAt(nMv), dMFactor, nCaseNum, LcomD, TRUE);
//
//            // Description
//            CString csPrefix(_T("")), csLcom(_T(""));
//            CString strFixM;
//            strFixM.Format(_T("%d"), nMv + 1);
//
//            if (bMoving && m_nMCount > 0)
//            {
//                csLcom += csPrefix + GetFactorString(dMFactor) + _T("M[") + strFixM + _T("]");
//                csPrefix = LcomD.Description.IsEmpty() ? _T("") : _T("+");
//                LcomD.Description += csPrefix + csLcom;
//            }
//
//            if (nCaseNum > 0) raTotalLcom.Add(LcomD);
//        }
//    }
//
//    if (raTotalLcom.GetSize() > 0)
//    {
//        raNewLcom.Copy(raTotalLcom);
//    }
//}

void CLoadCombCtrl::Add_CvlTMH07_LS(
    const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arLsLoad, ArINT& arLLoad, ArUNT& arCombinedLoad, double& dMFactor,
    double& dGf3ULS, double& dGf3SLS, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if (enLcomNo == EN_TMH07_NO_EQ_ST || enLcomNo == EN_TMH07_NO_EQ_RS) return;
    int nLsCount = arLsLoad.GetSize();
    if (nLsCount == 0) return;

    CString strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);
    int nLcom = raNewLcom.GetSize();

    double dGf3 = bStrength ? dGf3ULS : dGf3SLS;

    CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;

    for (int i = 0; i < nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();
        for (int nLs = 0; nLs < nLsCount; ++nLs)
        {
            BOOL bMoving = FALSE;
            int nKey = arLsLoad.GetAt(nLs);
            CString strName;
            strName.Format(_T("%d"), nKey);

            // Check if the key is valid (non-zero, or your own validity check)
         /*   if (nKey == 0)
                continue;*/

          /*  bMoving = Exist_MovingLoadData(arLsLoad.GetAt(nLs));*/
            int nValue = 0;
            bMoving = m_lsdata.Lookup(arLsLoad.GetAt(nLs), nValue);
            if (!bMoving) continue;
            int nCaseNum = nExistCase;
            dMFactor = Get_TMHMovingLoadFactorData(arLsLoad.GetAt(nLs), strTitle);
            dMFactor = dGf3 * dMFactor;
         
            int nEsL = Set_STLCIncludeCS(arLsLoad.GetAt(nLs),dMFactor, nCaseNum, LcomD, D_SGLD_DL_CVL);

            // Description
            if (nEsL > 0)
            {
                CString csPrefix(_T("")), csLcom(_T(""));
                csLcom += csPrefix + GetFactorString(dGf3) + _T("LS");
                csPrefix = _T("+");
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }

            if (nCaseNum > 0) raTotalLcom.Add(LcomD);
        }
    }

    if (raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}
void CLoadCombCtrl::Add_CvlTMH07_W(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arWLoad, double& dWFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom, double& dGf3ULS, double& dGf3SLS)
{
	if (m_nWCount == 0) return;

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nLcom = raNewLcom.GetSize();

	int nWCountTemp = (m_nWCount > 0) ? m_nWCount * 2 : 1;

	CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

	//for (int i = 0; i < nLcom; ++i)
	//{
	//	for (int nWm = 0; nWm < nWCountTemp; ++nWm)
	//	{
	//		T_LCOM_D LcomD = raNewLcom[i];
	//		_LCOM_BOTH BothD; BothD.Initialize();
	//		int nExistCase = LcomD.aCombination.GetSize();
	//		int nWL = 0, nCaseNum = nExistCase;
	//		double dWF = (nWm % 2 == 0) ? dWFactor : -1.0 * dWFactor;
	//		if (m_nWCount > 0) nWL += Set_STLCIncludeCS(arWLoad.GetAt(nWm / 2), dWF, nCaseNum, LcomD, 0);
	//		// Description
	//		if (nWL > 0)
	//		{
	//			CString csPrefix(_T("")), csLcom(_T(""));
	//			CString strFixW;
	//			strFixW.Format(_T("%d"), nWm / 2 + 1);
	//			csLcom += csPrefix + GetFactorString(dWF) + _T("W[") + strFixW + _T("]"); csPrefix = _T("+");
	//			if (dWF < 0.0)
	//			{
	//				csPrefix = _T("");
	//			}

	//			if (csLcom.IsEmpty() || LcomD.Description == strTitle)
	//			{
	//				csPrefix = _T("");
	//			}
	//			LcomD.Description += csPrefix + csLcom;
	//		}
	//		if (nCaseNum > 0) raTotalLcom.Add(LcomD);
	//	}
	//}
    for (int r = 0; r < 2; r++) {
        if (r == 1) {
            if (bStrength) {
                dWFactor = 1.0 * dGf3ULS;
            }
            else {
                dWFactor = 1.0 * dGf3SLS;
            }
        }
        for (int k = 0; k < 2; ++k) {
            for (int i = 0; i < nLcom; ++i)
            {
                if (k == 1 && i == 0) {
                    dWFactor = -1.0 * dWFactor;
                }
                T_LCOM_D LcomD = raNewLcom[i];
                _LCOM_BOTH BothD; BothD.Initialize();
                int nExistCase = LcomD.aCombination.GetSize();

                for (int j = 0; j < m_nWCount; ++j) {
                    int nCoL = 0, nCaseNum = nExistCase;
                    nCoL += Set_STLCIncludeCS(arWLoad.GetAt(j), dWFactor, nCaseNum, LcomD, 0);
                    // Description
                    if (nCoL > 0)
                    {
                        CString csPrefix(_T("")), csLcom(_T(""));
                        if (nCoL > 0) { csLcom += csPrefix + GetFactorString(dWFactor) + _T("W"); csPrefix = _T("+"); }
                        if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                        {
                            csPrefix = _T("");
                        }
                        LcomD.Description += csPrefix + csLcom;
                    }
                    if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
                }
            }
        }
    }
	if (raTotalLcom.GetSize() > 0)
	{
		raNewLcom.Copy(raTotalLcom);
	}
}

void CLoadCombCtrl::Add_CvlTMH07_T_TPG(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arTLoad, ArINT& arTpgLoad, double& dTFactor, double& dTpgFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
	if (enLcomNo != EN_TMH07_NO_EQ_ST) return;
	if (m_nConstLoad == 0 && m_nTCount == 0 && m_nTpgCount == 0) return;

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nLcom = raNewLcom.GetSize();

	int nTCountTemp = max(m_nTCount, 1);
	int nTpgCountTemp = max(m_nTpgCount, 1);

	CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

	for (int i = 0; i < nLcom; ++i)
	{
		for (int nTm = 0; nTm < nTCountTemp; ++nTm)
		{
			for (int nTpgm = 0; nTpgm < nTpgCountTemp; ++nTpgm)
			{
				T_LCOM_D LcomD = raNewLcom[i];
				_LCOM_BOTH BothD; BothD.Initialize();
				int nExistCase = LcomD.aCombination.GetSize();
				int nTL = 0, nTpgL = 0, nCaseNum = nExistCase;
				if (m_nTCount > 0)   nTL += Set_STLCIncludeCS(arTLoad.GetAt(nTm), dTFactor, nCaseNum, LcomD, 0);
				if (m_nTpgCount > 0) nTpgL += Set_STLCIncludeCS(arTpgLoad.GetAt(nTpgm), dTpgFactor, nCaseNum, LcomD, 0);

				// Description
				if (nTL > 0 || nTpgL > 0)
				{
					CString csPrefix(_T("")), csLcom(_T(""));
					CString strFixT, strFixTpg;
					strFixT.Format(_T("%d"), nTm + 1);
					strFixTpg.Format(_T("%d"), nTpgm + 1);
					if (nTL > 0) { csLcom += csPrefix + GetFactorString(dTFactor) + _T("T[") + strFixT + _T("]"); csPrefix = _T("+"); }
					if (nTpgL > 0) { csLcom += csPrefix + GetFactorString(dTpgFactor) + _T("TPG[") + strFixTpg + _T("]"); csPrefix = _T("+"); }
					if (csLcom.IsEmpty() || LcomD.Description == strTitle)
					{
						csPrefix = _T("");
					}
					LcomD.Description += csPrefix + csLcom;
				}
				if (nCaseNum > 0) raTotalLcom.Add(LcomD);
			}
		}
	}
	if (raTotalLcom.GetSize() > 0)
	{
		raNewLcom.Copy(raTotalLcom);
	}
}

void CLoadCombCtrl::Add_CvlTMH07_T(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arTLoad, ArINT& arTpgLoad, double& dTFactor, double& dTpgFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
	if (enLcomNo == EN_TMH07_NO_01) return;
	if (m_nConstLoad == 0 && m_nTCount == 0 && m_nTpgCount == 0) return;

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nLcom = raNewLcom.GetSize();

	int nTCountTemp = max(m_nTCount, 1);
	int nTpgCountTemp = max(m_nTpgCount, 1);

	CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;

	for (int i = 0; i < nLcom; ++i)
	{
		T_LCOM_D LcomD = raNewLcom[i];
		_LCOM_BOTH BothD; BothD.Initialize();
		int nExistCase = LcomD.aCombination.GetSize();

		for (int j = 0; j < m_nTCount; ++j) {
			int nCoL = 0, nCaseNum = nExistCase;
			nCoL += Set_STLCIncludeCS(arTLoad.GetAt(j), dTFactor, nCaseNum, LcomD, 0);
			// Description
			if (nCoL > 0)
			{
				CString csPrefix(_T("")), csLcom(_T(""));
				if (nCoL > 0) { csLcom += csPrefix + GetFactorString(dTFactor) + _T("T"); csPrefix = _T("+"); }
				if (csLcom.IsEmpty() || LcomD.Description == strTitle)
				{
					csPrefix = _T("");
				}
				LcomD.Description += csPrefix + csLcom;
			}
			if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
		}
	}
	if (raTotalLcom.GetSize() > 0) {
		raNewLcom.Copy(raTotalLcom);
	}
}
void CLoadCombCtrl::Add_CvlTMH07_TPG(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arTLoad, ArINT& arTpgLoad, double& dTFactor, double& dTpgFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
	if (enLcomNo == EN_TMH07_NO_01) return;
	if (m_nConstLoad == 0 && m_nTCount == 0 && m_nTpgCount == 0) return;

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nLcom = raNewLcom.GetSize();

	int nTCountTemp = max(m_nTCount, 1);
	int nTpgCountTemp = max(m_nTpgCount, 1);

	CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;

	for (int i = 0; i < nLcom; ++i)
	{
		T_LCOM_D LcomD = raNewLcom[i];
		_LCOM_BOTH BothD; BothD.Initialize();
		int nExistCase = LcomD.aCombination.GetSize();

		for (int j = 0; j < m_nTpgCount; ++j) {
			int nCoL = 0, nCaseNum = nExistCase;
			nCoL += Set_STLCIncludeCS(arTpgLoad.GetAt(j), dTpgFactor, nCaseNum, LcomD, 0);
			// Description
			if (nCoL > 0)
			{
				CString csPrefix(_T("")), csLcom(_T(""));
				if (nCoL > 0) { csLcom += csPrefix + GetFactorString(dTpgFactor) + _T("TPG"); csPrefix = _T("+"); }
				if (csLcom.IsEmpty() || LcomD.Description == strTitle)
				{
					csPrefix = _T("");
				}
				LcomD.Description += csPrefix + csLcom;
			}
			if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
		}
	}
	if (raTotalLcom.GetSize() > 0) {
		raNewLcom.Copy(raTotalLcom);
	}

}
void CLoadCombCtrl::Add_CvlTMH07_FA(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arSfLoad, double& dTFactor,CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
	if (enLcomNo == EN_TMH07_NO_01) return;
    int m_nSrCount = arSfLoad.GetSize();
	if (m_nSrCount == 0) return;

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nLcom = raNewLcom.GetSize();

	/*int nTCountTemp = max(m_nTCount, 1);*/
	/*int nTpgCountTemp = max(m_nSrCount, 1);*/

	CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;

	for (int i = 0; i < nLcom; ++i)
	{
		T_LCOM_D LcomD = raNewLcom[i];
		_LCOM_BOTH BothD; BothD.Initialize();
		int nExistCase = LcomD.aCombination.GetSize();

		for (int j = 0; j < m_nSrCount; ++j) {
			int nCoL = 0, nCaseNum = nExistCase;
			nCoL += Set_STLCIncludeCS(arSfLoad.GetAt(j), dTFactor, nCaseNum, LcomD, 0);
			// Description
			if (nCoL > 0)
			{
				CString csPrefix(_T("")), csLcom(_T(""));
				if (nCoL > 0) { csLcom += csPrefix + GetFactorString(dTFactor) + _T("SF"); csPrefix = _T("+"); }
				if (csLcom.IsEmpty() || LcomD.Description == strTitle)
				{
					csPrefix = _T("");
				}
				LcomD.Description += csPrefix + csLcom;
			}
			if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
		}
	}
	if (raTotalLcom.GetSize() > 0) {
		raNewLcom.Copy(raTotalLcom);
	}

}
void CLoadCombCtrl::Add_CvlTMH07_CF(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCfLoad, double& dTFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
	if (enLcomNo == EN_TMH07_NO_01) return;
    if (enLcomNo == EN_TMH07_NO_EQ_ST || enLcomNo == EN_TMH07_NO_EQ_RS) return;
	int m_nCfCount = arCfLoad.GetSize();
	if (m_nCfCount == 0) return;

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nLcom = raNewLcom.GetSize();

	/*int nTCountTemp = max(m_nTCount, 1);*/
	/*int nTpgCountTemp = max(m_nSrCount, 1);*/

	CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;

	for (int i = 0; i < nLcom; ++i)
	{
		T_LCOM_D LcomD = raNewLcom[i];
		_LCOM_BOTH BothD; BothD.Initialize();
		int nExistCase = LcomD.aCombination.GetSize();

		for (int j = 0; j < m_nCfCount; ++j) {
			int nCoL = 0, nCaseNum = nExistCase;
			nCoL += Set_STLCIncludeCS(arCfLoad.GetAt(j), dTFactor, nCaseNum, LcomD, 0);
			// Description
			if (nCoL > 0)
			{
				CString csPrefix(_T("")), csLcom(_T(""));
				if (nCoL > 0) { csLcom += csPrefix + GetFactorString(dTFactor) + _T("CF"); csPrefix = _T("+"); }
				if (csLcom.IsEmpty() || LcomD.Description == strTitle)
				{
					csPrefix = _T("");
				}
				LcomD.Description += csPrefix + csLcom;
			}
			if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
		}
	}
	if (raTotalLcom.GetSize() > 0) {
		raNewLcom.Copy(raTotalLcom);
	}

}
void CLoadCombCtrl::Add_CvlTMH07_CO(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCoLoad, double& dCoFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
	if (enLcomNo == EN_TMH07_NO_01) return;
	if (m_nCoCount == 0) return;

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nLcom = raNewLcom.GetSize();

	CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;
    for (int i = 0; i < nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        _LCOM_BOTH BothD; BothD.Initialize();
        int nExistCase = LcomD.aCombination.GetSize();

        for (int j = 0; j < m_nCoCount; ++j) {
            int nCoL = 0, nCaseNum = nExistCase;
            nCoL += Set_STLCIncludeCS(arCoLoad.GetAt(j), dCoFactor, nCaseNum, LcomD, 0);
            // Description
            if (nCoL > 0)
            {
                CString csPrefix(_T("")), csLcom(_T(""));
                if (nCoL > 0) { csLcom += csPrefix + GetFactorString(dCoFactor) + _T("CO"); csPrefix = _T("+"); }
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }
            if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
        }
    }
    if (raTotalLcom.GetSize() > 0) {
        raNewLcom.Copy(raTotalLcom);
    }
}
void CLoadCombCtrl::Add_CvlTMH07_AS(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arASLoad, double& dCoFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
	int m_ASCount = arASLoad.GetSize();
	if (enLcomNo == EN_TMH07_NO_01) return;
    if (enLcomNo == EN_TMH07_NO_EQ_RS || enLcomNo == EN_TMH07_NO_EQ_ST) return;
	if (m_nASCount == 0) return;

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nLcom = raNewLcom.GetSize();

	CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;
	for (int i = 0; i < nLcom; ++i)
	{
		T_LCOM_D LcomD = raNewLcom[i];
		_LCOM_BOTH BothD; BothD.Initialize();
		int nExistCase = LcomD.aCombination.GetSize();

		for (int j = 0; j < m_nASCount; ++j) {
			int nCoL = 0, nCaseNum = nExistCase;
			nCoL += Set_STLCIncludeCS(arASLoad.GetAt(j), dCoFactor, nCaseNum, LcomD, 0);
			// Description
			if (nCoL > 0)
			{
				CString csPrefix(_T("")), csLcom(_T(""));
				if (nCoL > 0) { csLcom += csPrefix + GetFactorString(dCoFactor) + _T("CO"); csPrefix = _T("+"); }
				if (csLcom.IsEmpty() || LcomD.Description == strTitle)
				{
					csPrefix = _T("");
				}
				LcomD.Description += csPrefix + csLcom;
			}
			if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
		}
	}
	if (raTotalLcom.GetSize() > 0) {
		raNewLcom.Copy(raTotalLcom);
	}
}

void CLoadCombCtrl::Add_CvlTMH07_BRK(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arBrkLoad,double& dGf3ULS,double& dGf3SLS,CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
	if (enLcomNo == EN_TMH07_NO_01 || enLcomNo == EN_TMH07_NO_EQ_RS || enLcomNo == EN_TMH07_NO_EQ_ST) return;
	if (m_nBrkCount == 0) return;

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nLcom = raNewLcom.GetSize();

	int nMCount = max(m_nMCount, 1);

	CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;
	//for (int i = 0; i < nLcom; ++i)
	//{
	//	for (int nMv = 0; nMv < nMCount; ++nMv)
	//	{
	//		T_LCOM_D LcomD = raNewLcom[i];
	//		_LCOM_BOTH BothD; BothD.Initialize();
	//		int nExistCase = LcomD.aCombination.GetSize();
	//		int nCfL = 0, nBrL = 0, nCaseNum = nExistCase;
	//		BOOL bMoving = FALSE;
	//		if (m_nMCount > 0) bMoving = Exist_MovingLoadData(arMLoad.GetAt(nMv));
	//		if (bMoving)
	//		{
	//			switch (enLcomNo)
	//			{
	//			case EN_TMH07_NO_04_02:
	//				for (int j = 0; j < m_nCfCount; ++j)  nCfL += Set_STLCIncludeCS(arCfLoad.GetAt(j), dBrCfFactor, nCaseNum, LcomD, 0);
	//				break;
	//			case EN_TMH07_NO_04_03:
	//				for (int j = 0; j < m_nBrkCount; ++j) nBrL += Set_STLCIncludeCS(arBrkLoad.GetAt(j), dBrCfFactor, nCaseNum, LcomD, 0);
	//				break;
	//			default:
	//				break;
	//			}

	//			// Description
	//			if (nCfL > 0 || nBrL > 0)
	//			{
	//				CString csPrefix(_T("")), csLcom(_T(""));
	//				switch (enLcomNo)
	//				{
	//				case EN_TMH07_NO_04_02:
	//					if (nCfL > 0) { csLcom += csPrefix + GetFactorString(dBrCfFactor) + _T("CF"); csPrefix = _T("+"); }
	//					break;
	//				case EN_TMH07_NO_04_03:
	//					if (nBrL > 0) { csLcom += csPrefix + GetFactorString(dBrCfFactor) + _T("BRK"); csPrefix = _T("+"); }
	//					break;
	//				default:
	//					break;
	//				}

	//				if (csLcom.IsEmpty() || LcomD.Description == strTitle)
	//				{
	//					csPrefix = _T("");
	//				}
	//				LcomD.Description += csPrefix + csLcom;
	//			}
	//		}
	//		if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
	//	}
	//}
	for (int i = 0; i < nLcom; ++i)
	{
		T_LCOM_D LcomD = raNewLcom[i];
		_LCOM_BOTH BothD; BothD.Initialize();
		int nExistCase = LcomD.aCombination.GetSize();

		for (int j = 0; j < m_nBrkCount; ++j) {
			if (!Exist_LongBrkTrct_Data(arBrkLoad.GetAt(j))) {
				continue; // Skip this iteration if the key is not present
			}
            double dBrCfFactor = lngbrk_factors(arBrkLoad.GetAt(j));
            double dBrFactor;
            if (bStrength) {
                dBrFactor = dBrCfFactor * dGf3ULS;
            }
            else {
                dBrCfFactor = 1;
                dBrFactor = dBrCfFactor * dGf3SLS;
            }
			int nCoL = 0, nCaseNum = nExistCase;
			nCoL += Set_STLCIncludeCS(arBrkLoad.GetAt(j), dBrFactor, nCaseNum, LcomD, 0);
			// Description
			if (nCoL > 0)
			{
				CString csPrefix(_T("")), csLcom(_T(""));
				if (nCoL > 0) { csLcom += csPrefix + GetFactorString(dBrFactor) + _T("BRK"); csPrefix = _T("+"); }
				if (csLcom.IsEmpty() || LcomD.Description == strTitle)
				{
					csPrefix = _T("");
				}
				LcomD.Description += csPrefix + csLcom;
			}
			if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
		}
	}

	if (raTotalLcom.GetSize() > 0)
	{
		raNewLcom.Copy(raTotalLcom);
	}
}

void CLoadCombCtrl::Add_CvlTMH07_ER(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arErLoad, double& dFrFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
	if (enLcomNo != EN_TMH07_NO_02_01) return;
	if (m_nErCount == 0) return;

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nLcom = raNewLcom.GetSize();

	CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;
	for (int i = 0; i < nLcom; ++i)
	{
		T_LCOM_D LcomD = raNewLcom[i];
		_LCOM_BOTH BothD; BothD.Initialize();
		int nExistCase = LcomD.aCombination.GetSize();
		int nErL = 0, nCaseNum = nExistCase;
		for (int j = 0; j < m_nErCount; ++j)  nErL += Set_STLCIncludeCS(arErLoad.GetAt(j), dFrFactor, nCaseNum, LcomD, 0);

		// Description
		if (nErL > 0)
		{
			CString csPrefix(_T("")), csLcom(_T(""));
			if (nErL > 0) { csLcom += csPrefix + GetFactorString(dFrFactor) + _T("ER"); csPrefix = _T("+"); }
			if (csLcom.IsEmpty() || LcomD.Description == strTitle)
			{
				csPrefix = _T("");
			}
			LcomD.Description += csPrefix + csLcom;
		}
		if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
	}
	raNewLcom.Copy(raTotalLcom);
}
void CLoadCombCtrl::Add_CvlTMH07_ES(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arEsLoad, double& dEs1, double& dEs2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom,int& m_nCodeBasedVerticalEarthPressure,double& dGf3ULS,double& dGf3SLS)
{
    if (m_nEsCount == 0) return;

    CString strTitle;
    strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

    double dEsFac[2];
    if (enLcomNo == EN_TMH07_NO_02_01) {
        dEsFac[0] = dEs2;
		dEsFac[1] = dEs2;

    } else {
       dEsFac[0] = dEs1;
	   dEsFac[1] = dEs1;
	}
    if (bStrength) {
        dEsFac[0] = dEsFac[0] * dGf3ULS;
        dEsFac[1] = dEsFac[1] * dGf3ULS;
    }
    else {
		dEsFac[0] = dEsFac[0] * dGf3SLS;
		dEsFac[1] = dEsFac[1] * dGf3SLS;
    }

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;

    for (int i = 0; i < nLcom; ++i)
    {
        for (int nEs = 0; nEs < m_nBD21DNum; ++nEs) // Use the correct count variable for ES loads
        {
            T_LCOM_D LcomD = raNewLcom[i];
            _LCOM_BOTH BothD; BothD.Initialize();
            int nExistCase = LcomD.aCombination.GetSize();
            int nEsL = 0, nCaseNum = nExistCase;

			for (int j = 0; j < m_nEsCount; ++j)
			{
				nEsL += Set_STLCIncludeCS(arEsLoad.GetAt(j), dEsFac[nEs], nCaseNum, LcomD, D_SGLD_DL_CVL);
				(nEs == 1) ? BothD.nBoth[nCaseNum] = 1 : BothD.nBoth[nCaseNum] = 0;
			}

            // Description
            if (nEsL > 0)
            {
                CString csPrefix(_T("")), csLcom(_T(""));
                csLcom += csPrefix + GetFactorString(dEsFac[nEs]) + _T("ES");
                csPrefix = _T("+");
                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }

            if (nCaseNum > 0) raTotalLcom.Add(LcomD);
        }
    }

    if (raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}
//void CLoadCombCtrl::Add_CvlTMH07_LS(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arLsLoad, double& dMFactor, double& dGf3ULS, double& dGf3SLS, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
//{
//	m_nLsCount = arLsLoad.GetSize();
//	if (m_nLsCount == 0) return;
//	BOOL bMoving = FALSE;
//
//	CString strTitle;
//	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);
//
//	int nLsCount = max(m_nLsCount, 1);
//
//	int nLcom = raNewLcom.GetSize();
//
//	double dGf3 = 0.0;
//	double dPede = 0.0;
//
//	if (bStrength)
//	{
//		dGf3 = dGf3ULS;
//		if (enLcomNo == EN_TMH07_NO_01)
//		{
//			dPede = 1.5; //Pedestrian 인 경우 ULS에서 Moving Factor는 1.5
//		}
//		else
//		{
//			dPede = 1.25; //Pedestrian 인 경우 ULS에서 Moving Factor는 1.25
//		}
//	}
//	else
//	{
//		dGf3 = dGf3SLS;
//		dPede = 1.0; //Pedestrian 인 경우 SLS에서 Moving Factor는 1.0
//	}
//
//
//	CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;
//
//	for (int i = 0; i < nLcom; ++i)
//	{
//		for (int nEs = 0; nEs < m_nBD21DNum; ++nEs) // Use the correct count variable for ES loads
//		{
//			if (m_nMCount > 0) bMoving = Exist_MovingLoadData(arLsLoad.GetAt(nEs));
//			if (!bMoving) {
//				continue;
//			}
//			T_LCOM_D LcomD = raNewLcom[i];
//			_LCOM_BOTH BothD; BothD.Initialize();
//			int nExistCase = LcomD.aCombination.GetSize();
//			int nEsL = 0, nCaseNum = nExistCase;
//			if (bMoving) dMFactor = Get_BSMovingLoadFactorData(arLsLoad.GetAt(nEs), strTitle);
//			if (bMoving) dMFactor = dGf3 * dMFactor;
//			for (int j = 0; j < m_nEsCount; ++j)
//			{
//				nEsL += Set_STLCIncludeCS(arLsLoad.GetAt(j), dMFactor, nCaseNum, LcomD, D_SGLD_DL_CVL);
//				(nEs == 1) ? BothD.nBoth[nCaseNum] = 1 : BothD.nBoth[nCaseNum] = 0;
//			}
//
//			// Description
//			if (nEsL > 0)
//			{
//				CString csPrefix(_T("")), csLcom(_T(""));
//				csLcom += csPrefix + GetFactorString(dMFactor) + _T("LS");
//				csPrefix = _T("+");
//				if (csLcom.IsEmpty() || LcomD.Description == strTitle)
//				{
//					csPrefix = _T("");
//				}
//				LcomD.Description += csPrefix + csLcom;
//			}
//
//			if (nCaseNum > 0) raTotalLcom.Add(LcomD);
//		}
//	}
//
//	if (raTotalLcom.GetSize() > 0)
//	{
//		raNewLcom.Copy(raTotalLcom);
//	}
//}
void CLoadCombCtrl::Add_CvlTMH07_CRL(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCrlLoad, double& dCrl1, double& dCrl2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom,double& dGf3ULS,double& dGf3SLS)
{
    if (enLcomNo == EN_TMH07_NO_EQ_RS || enLcomNo == EN_TMH07_NO_EQ_ST)return;
	if (m_nCrlCount == 0) return;

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);  

	double dCrlFac[1];
	if (enLcomNo == EN_TMH07_NO_01)
	{
		dCrlFac[0] = dCrl1;
	}
	else
	{
        dCrlFac[0] = dCrl2;
	}
	/*dCrlFac[0] = dCrl1;
	dCrlFac[1] = dCrl2;*/
    if (bStrength) {
        dCrlFac[0] = dCrlFac[0] * dGf3ULS;
    }
    else {
		dCrlFac[0] = dCrlFac[0] * dGf3SLS;
    }
	int nLcom = raNewLcom.GetSize();

	CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;
 
    for (int i = 0; i < nLcom; ++i)
    {
        //for (int nEs = 0; nEs < m_nBD21DNum; ++nEs) // Use the correct count variable for ES loads
        //{
            if (bStrength) {
                T_LCOM_D LcomD = raNewLcom[i];
                _LCOM_BOTH BothD; BothD.Initialize();
                int nExistCase = LcomD.aCombination.GetSize();
                int nEsL = 0, nCaseNum = nExistCase;

                for (int j = 0; j < m_nCrlCount; ++j)
                {
                    nEsL += Set_STLCIncludeCS(arCrlLoad.GetAt(j), dCrlFac[0], nCaseNum, LcomD, D_SGLD_DL_CVL);
                   /* (nEs == 1) ? BothD.nBoth[nCaseNum] = 1 : BothD.nBoth[nCaseNum] = 0;*/
                }

                // Description
                if (nEsL > 0)
                {
                    CString csPrefix(_T("")), csLcom(_T(""));
                    csLcom += csPrefix + GetFactorString(dCrlFac[0]) + _T("CRL");
                    csPrefix = _T("+");
                    if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                    {
                        csPrefix = _T("");
                    }
                    LcomD.Description += csPrefix + csLcom;
                }

                if (nCaseNum > 0) raTotalLcom.Add(LcomD);
            }
            else {
				T_LCOM_D LcomD = raNewLcom[i];
				_LCOM_BOTH BothD; BothD.Initialize();
                int nExistCase = LcomD.aCombination.GetSize();
                for (int j = 0; j < m_nCrlCount; ++j)
                {  
					
					int nEsL = 0, nCaseNum = nExistCase;
                    nEsL += Set_STLCIncludeCS(arCrlLoad.GetAt(j), dCrlFac[0], nCaseNum, LcomD, D_SGLD_DL_CVL);
                  /*  (nEs == 1) ? BothD.nBoth[nCaseNum] = 1 : BothD.nBoth[nCaseNum] = 0;*/

                    // Description
                    if (nEsL > 0)
                    {
                        CString csPrefix(_T("")), csLcom(_T(""));
                        csLcom += csPrefix + GetFactorString(dCrlFac[0]) + _T("CRL");
                        csPrefix = _T("+");
                        if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                        {
                            csPrefix = _T("");
                        }
                        LcomD.Description += csPrefix + csLcom;
                    }
                    if (nCaseNum > 0) raTotalLcom.Add(LcomD);
                }
            }
       /* }*/
    }
   if (raTotalLcom.GetSize() > 0)
   {
       raNewLcom.Copy(raTotalLcom);
   }
}
// Add Construction stage load

void CLoadCombCtrl::Add_CvlTMH07_CS_W(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dWFactor, double& dGf3ULS,double& dGf3SLS, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    /*if (enLcomNo != EN_TMH07_NO_02_01)return;*/
    if (m_nConstLoad == 0 || m_nCSCount == 0) return; // Static only or no CS

    CString strTitle;
    strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;
    for (int r = 0; r < 2; r++) {
        if (r == 1) {
            if (bStrength) {
                dWFactor = 1.0 * dGf3ULS;
            }
            else {
                dWFactor = 1.0 * dGf3SLS;
            }
        }
    for (int k = 0; k < 2; ++k) {
        for (int i = 0; i < nLcom; ++i)
        {
            if (k == 1 && i == 0) {
                dWFactor = -1.0 * dWFactor;
            }

            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            /*int nn = 0, nCaseNum = nExistCase;

            CString strcEL;
            int nErection = 0;
            strcEL = _T("");
            nErection = 0;*/
            // Process only ER load cases
            for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
            {
                int nn = 0, nCaseNum = nExistCase;

                CString strcEL;
                int nErection = 0;
                strcEL = _T("");
                /*nErection = 0;*/

                if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nn))
                {
                    strcEL = m_CSEL_Sgld_Desc[nErec];
                    nErection = m_CSEL_SgldK[nErec];
                }

                if (CheckCSEL(nErection, _T("W")))
                {
                    Set_CStageCase(arCSLoad.GetAt(nn), dWFactor, nCaseNum, LcomD, TRUE);
                }


                CString csPrefix(_T("")), csLcom(_T(""));

                /* nErection = 0;*/

                 // Build description for ER load cases
                 /*for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
                 {*/
                 /* strcEL = _T("");
                  nErection = 0;*/

                if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nn))
                {
                    strcEL = m_CSEL_Sgld_Desc[nErec];
                    nErection = m_CSEL_SgldK[nErec];
                }

                if (CheckCSEL(nErection, _T("W")))
                {
                    csLcom += csPrefix + GetFactorString(dWFactor) + strcEL;
                    csPrefix = _T("+");
                }
                /*}*/

                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;

                if (nCaseNum > 0 && CheckCSEL(nErection, _T("W")))
                {
                    raTotalLcom.Add(LcomD);
                }
            }
        }
    }
}
    // Only copy if raTotalLcom has elements
    if (raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlTMH07_CS_ER_LL(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dTFactor, double& dTpgFactor, double& dErFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
	if (enLcomNo != EN_TMH07_NO_02_01) return;
	if (m_nConstLoad == 0 || m_nCSCount == 0) return; // Static only or no CS

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nLcom = raNewLcom.GetSize();

	int nTCountTemp = max(m_nTCount, 1);
	int nTpgCountTemp = max(m_nTpgCount, 1);

	CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

	for (int i = 0; i < nLcom; ++i)
	{
		T_LCOM_D LcomD = raNewLcom[i];
		int nExistCase = LcomD.aCombination.GetSize();
		int nn = 0, nCaseNum = nExistCase;


		CString strcEL;
		int nErection = 0;

		for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
		{
			strcEL = _T(""); nErection = 0;
			if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

			/*if (CheckCSEL(nErection, _T("T")))
				Set_CStageCase(arCSLoad.GetAt(nn), dTFactor, nCaseNum, LcomD, TRUE);*/
			/*if (CheckCSEL(nErection, _T("TPG")))
				Set_CStageCase(arCSLoad.GetAt(nn), dTpgFactor, nCaseNum, LcomD, TRUE);*/
            if (CheckCSEL(nErection, _T("ER")))
                Set_CStageCase(arCSLoad.GetAt(nn), dErFactor, nCaseNum, LcomD, TRUE);
			if (CheckCSEL(nErection, _T("L")))
				Set_CStageCase(arCSLoad.GetAt(nn), dErFactor, nCaseNum, LcomD, TRUE);
		}



		CString csPrefix(_T("")), csLcom(_T(""));

		nErection = 0;

		for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
		{
			strcEL = _T(""); nErection = 0;
			if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

			/*if (CheckCSEL(nErection, _T("T")))
			{
				csLcom += csPrefix + GetFactorString(dTFactor) + strcEL; csPrefix = _T("+");
			}*/
			/*if (CheckCSEL(nErection, _T("TPG")))
			{
				csLcom += csPrefix + GetFactorString(dTFactor) + strcEL; csPrefix = _T("+");
			}*/
			if (strcEL == _T("ER"))
			{
				csLcom += csPrefix + GetFactorString(dErFactor) + strcEL; csPrefix = _T("+");
			}
			if (CheckCSEL(nErection, _T("L")))
			{
				csLcom += csPrefix + GetFactorString(dErFactor) + strcEL; csPrefix = _T("+");
			}
		}
		if (csLcom.IsEmpty() || LcomD.Description == strTitle)
		{
			csPrefix = _T("");
		}
		LcomD.Description += csPrefix + csLcom;

		if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
	}
	raNewLcom.Copy(raTotalLcom);
}
void CLoadCombCtrl::Add_CvlTMH07_CS_EP(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dEpFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom, double& m_dEPRelieving, double& dGf3ULS, double& dGf3SLS,int& m_nCodeBasedNonVerticalEarthPressure)
{
    /*if (enLcomNo != EN_TMH07_NO_02_01)return;*/
	if (m_nConstLoad == 0 || m_nCSCount == 0) return; // Static only or no CS
    if (enLcomNo == EN_TMH07_NO_02_01) {
        if (bStrength) {
            if (m_nCodeBasedNonVerticalEarthPressure == 0) {
                dEpFactor = 1.3;
            }
            else {
                dEpFactor = 1.2;
            }
        }
        else {
            if (m_nCodeBasedNonVerticalEarthPressure == 0) {
                dEpFactor = 1.1;
            }
            else {
                dEpFactor = 1.0;
            }
        }
    }
    else {
        if (bStrength) {
            if (m_nCodeBasedNonVerticalEarthPressure == 0) {
                dEpFactor = 1.5;
            }
            else {
                dEpFactor = 1.4;
            }
            
        } else{
			if (m_nCodeBasedNonVerticalEarthPressure == 0) {
				dEpFactor = 1.1;
			}
			else {
				dEpFactor = 1.0;
			}
        }
    }
    if (bStrength) {
        dEpFactor = dEpFactor * dGf3ULS;
    }
    else {
        dEpFactor = dEpFactor * dGf3SLS;
    }
	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nLcom = raNewLcom.GetSize();

	CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;
    for (int j = 0; j < 2; ++j) {
        if (j == 1) {
            if (bStrength) {
                dEpFactor = m_dEPRelieving * dGf3ULS;
            }
            else {
                dEpFactor = m_dEPRelieving * dGf3SLS;
            }
        }
        for (int i = 0; i < nLcom; ++i)
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nn = 0, nCaseNum = nExistCase;

            CString strcEL;
            int nErection = 0;

            // Process only ER load cases
            for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
            {
                strcEL = _T("");
                nErection = 0;

                if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nn))
                {
                    strcEL = m_CSEL_Sgld_Desc[nErec];
                    nErection = m_CSEL_SgldK[nErec];
                }

                if (CheckCSEL(nErection, _T("EP")))
                {
                    Set_CStageCase(arCSLoad.GetAt(nn), dEpFactor, nCaseNum, LcomD, TRUE);
                }
            }

            CString csPrefix(_T("")), csLcom(_T(""));

            nErection = 0;

            // Build description for ER load cases
            for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
            {
                strcEL = _T("");
                nErection = 0;

                if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nn))
                {
                    strcEL = m_CSEL_Sgld_Desc[nErec];
                    nErection = m_CSEL_SgldK[nErec];
                }

                if (CheckCSEL(nErection, _T("EP")))
                {
                    csLcom += csPrefix + GetFactorString(dEpFactor) + strcEL;
                    csPrefix = _T("+");
                }
            }

            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }

            LcomD.Description += csPrefix + csLcom;

            if (nCaseNum > 0)
            {
                raTotalLcom.Add(LcomD);
            }
        }
    }
	raNewLcom.Copy(raTotalLcom);
}
void CLoadCombCtrl::Add_CvlTMH07_CS_TPG(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dTpgFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
	if (enLcomNo == EN_TMH07_NO_01)return;
	if (m_nConstLoad == 0 || m_nCSCount == 0) return; // Static only or no CS

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nLcom = raNewLcom.GetSize();

	CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;

	for (int i = 0; i < nLcom; ++i)
	{
		T_LCOM_D LcomD = raNewLcom[i];
		int nExistCase = LcomD.aCombination.GetSize();
		/*int nn = 0, nCaseNum = nExistCase;

		CString strcEL;
		int nErection = 0;
		strcEL = _T("");
		nErection = 0;*/
		// Process only ER load cases
       for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
        {
		   int nn = 0, nCaseNum = nExistCase;

		   CString strcEL;
		   int nErection = 0;
		   strcEL = _T("");
		   /*nErection = 0;*/

            if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nn))
            {
                strcEL = m_CSEL_Sgld_Desc[nErec];
                nErection = m_CSEL_SgldK[nErec];
            }

            if (CheckCSEL(nErection, _T("TPG")))
            {
                Set_CStageCase(arCSLoad.GetAt(nn), dTpgFactor, nCaseNum, LcomD, TRUE);
            }


           CString csPrefix(_T("")), csLcom(_T(""));

           /* nErection = 0;*/

            // Build description for ER load cases
            /*for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
            {*/
			/* strcEL = _T("");
			 nErection = 0;*/

            if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nn))
            {
                strcEL = m_CSEL_Sgld_Desc[nErec];
                nErection = m_CSEL_SgldK[nErec];
            }

            if (CheckCSEL(nErection, _T("TPG")))
            {
                csLcom += csPrefix + GetFactorString(dTpgFactor) + strcEL;
                csPrefix = _T("+");
            }
           /*}*/

            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;

			if (nCaseNum > 0 && CheckCSEL(nErection, _T("TPG")))
			{
				raTotalLcom.Add(LcomD);
			}
        }
	}

    // Only copy if raTotalLcom has elements
    if (raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlTMH07_CS_T(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dTpgFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
    if (enLcomNo == EN_TMH07_NO_01)return;
    if (m_nConstLoad == 0 || m_nCSCount == 0) return; // Static only or no CS

    CString strTitle;
    strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

    int nLcom = raNewLcom.GetSize();

    CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;

    for (int i = 0; i < nLcom; ++i)
    {
        T_LCOM_D LcomD = raNewLcom[i];
        int nExistCase = LcomD.aCombination.GetSize();
        /*int nn = 0, nCaseNum = nExistCase;

        CString strcEL;
        int nErection = 0;
        strcEL = _T("");
        nErection = 0;*/
        // Process only ER load cases
        for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
        {
            int nn = 0, nCaseNum = nExistCase;

            CString strcEL;
            int nErection = 0;
            strcEL = _T("");
            /*nErection = 0;*/

            if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nn))
            {
                strcEL = m_CSEL_Sgld_Desc[nErec];
                nErection = m_CSEL_SgldK[nErec];
            }

            if (CheckCSEL(nErection, _T("T")))
            {
                Set_CStageCase(arCSLoad.GetAt(nn), dTpgFactor, nCaseNum, LcomD, TRUE);
            }


            CString csPrefix(_T("")), csLcom(_T(""));

            /* nErection = 0;*/

             // Build description for ER load cases
             /*for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
             {*/
             /* strcEL = _T("");
              nErection = 0;*/

            if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nn))
            {
                strcEL = m_CSEL_Sgld_Desc[nErec];
                nErection = m_CSEL_SgldK[nErec];
            }

            if (CheckCSEL(nErection, _T("T")))
            {
                csLcom += csPrefix + GetFactorString(dTpgFactor) + strcEL;
                csPrefix = _T("+");
            }
            /*}*/

            if (csLcom.IsEmpty() || LcomD.Description == strTitle)
            {
                csPrefix = _T("");
            }
            LcomD.Description += csPrefix + csLcom;

            if (nCaseNum > 0 && CheckCSEL(nErection, _T("T")))
            {
                raTotalLcom.Add(LcomD);
            }
        }
    }
    // Only copy if raTotalLcom has elements
    if (raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlTMH07_CS_PSCRSH(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dPsFactor, double& dCRSHFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
	if (m_nConstLoad == 0 || m_nCSCount == 0) return; // Static only or no CS

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nLcom = raNewLcom.GetSize();

	CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

	for (int i = 0; i < nLcom; ++i)
	{
		T_LCOM_D LcomD = raNewLcom[i];
		int nExistCase = LcomD.aCombination.GetSize();
		int nn = 0, nCaseNum = nExistCase;

		if (m_nConstLoad != 0)
		{
			CString strcEL;
			int nErection = 0;

			for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
			{
				strcEL = _T(""); nErection = 0;
				if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }
			}

			/*if (!bStrength)
			{*/
				/*if (CheckCS_SgldK(D_SGLD_TP_CVL, nn))  Set_CStageCase(arCSLoad.GetAt(nn), dPsFactor, nCaseNum, LcomD, TRUE);*/
			/*}*/

			if (CheckCS_SgldK(D_SGLD_TS_CVL, nn))  Set_CStageCase(arCSLoad.GetAt(nn), dCRSHFactor, nCaseNum, LcomD, TRUE);
            if (CheckCS_SgldK(D_SGLD_CP_CVL, nn))  Set_CStageCase(arCSLoad.GetAt(nn), dCRSHFactor, nCaseNum, LcomD, TRUE);
			if (CheckCS_SgldK(D_SGLD_CS_CVL, nn))  Set_CStageCase(arCSLoad.GetAt(nn), dCRSHFactor, nCaseNum, LcomD, TRUE);
			if (CheckCS_SgldK(D_SGLD_SS_CVL, nn))  Set_CStageCase(arCSLoad.GetAt(nn), dCRSHFactor, nCaseNum, LcomD, TRUE);
            if (CheckCS_SgldK(D_SGLD_SP_CVL, nn))  Set_CStageCase(arCSLoad.GetAt(nn), dCRSHFactor, nCaseNum, LcomD, TRUE);
		}

		if (m_nConstLoad != 0)
		{
			CString csPrefix(_T("")), csLcom(_T(""));

			/*	if (!bStrength)
				{*/
				/*if (CheckCS_SgldK(D_SGLD_TP_CVL, nn))
				{
					csLcom += csPrefix + GetFactorString(dPsFactor) + _T("(cTP)"); csPrefix = _T("+");
				}*/
		/*	}*/

			if (CheckCS_SgldK(D_SGLD_TS_CVL, nn))
			{
				csLcom += csPrefix + GetFactorString(dPsFactor) + _T("(cTs)"); csPrefix = _T("+");
			}
			if (CheckCS_SgldK(D_SGLD_CS_CVL, nn))
			{
				csLcom += csPrefix + GetFactorString(dCRSHFactor) + _T("(cCR)"); csPrefix = _T("+");
			}
			if (CheckCS_SgldK(D_SGLD_CP_CVL, nn))
			{
				csLcom += csPrefix + GetFactorString(dCRSHFactor) + _T("(cCR)"); csPrefix = _T("+");
			}
			if (CheckCS_SgldK(D_SGLD_SS_CVL, nn))
			{
				csLcom += csPrefix + GetFactorString(dCRSHFactor) + _T("(cSH)"); csPrefix = _T("+");
			}
			if (CheckCS_SgldK(D_SGLD_SP_CVL, nn))
			{
				csLcom += csPrefix + GetFactorString(dCRSHFactor) + _T("(cSH)"); csPrefix = _T("+");
			}

			if (csLcom.IsEmpty() || LcomD.Description == strTitle)
			{
				csPrefix = _T("");
			}
			LcomD.Description += csPrefix + csLcom;
		}

		if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
	}
    // Only copy if raTotalLcom has elements
    if (raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}
void CLoadCombCtrl::Add_CvlTMH07_CS_TP(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dPsFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom, double& m_dEPRelieving, double& dGf3ULS, double& dGf3SLS)
{
	if (m_nConstLoad == 0 || m_nCSCount == 0) return; // Static only or no CS

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nLcom = raNewLcom.GetSize();

	CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;
    for (int j = 0; j < 2; ++j) {
        if (j == 1) {
            if (bStrength) {
                dPsFactor = 0.87 * dGf3ULS; 
            }
            else {
                dPsFactor = 1.0 * dGf3SLS; 
            }
        }
        for (int i = 0; i < nLcom; ++i)
        {
            T_LCOM_D LcomD = raNewLcom[i];
            int nExistCase = LcomD.aCombination.GetSize();
            int nn = 0, nCaseNum = nExistCase;

            if (m_nConstLoad != 0)
            {
                CString strcEL;
                int nErection = 0;

                for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
                {
                    strcEL = _T("");
                    nErection = 0;
                    if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nn))
                    {
                        strcEL = m_CSEL_Sgld_Desc[nErec];
                        nErection = m_CSEL_SgldK[nErec];
                    }
                }

                // Process only D_SGLD_TP_CVL load case
                if (CheckCS_SgldK(D_SGLD_TP_CVL, nn))
                {
                    Set_CStageCase(arCSLoad.GetAt(nn), dPsFactor, nCaseNum, LcomD, TRUE);
                }
            }

            if (m_nConstLoad != 0)
            {
                CString csPrefix(_T("")), csLcom(_T(""));

                // Build description for D_SGLD_TP_CVL load case
                if (CheckCS_SgldK(D_SGLD_TP_CVL, nn))
                {
                    csLcom += csPrefix + GetFactorString(dPsFactor) + _T("(cTP)");
                    csPrefix = _T("+");
                }

                if (csLcom.IsEmpty() || LcomD.Description == strTitle)
                {
                    csPrefix = _T("");
                }
                LcomD.Description += csPrefix + csLcom;
            }

            if (nCaseNum > 0)
            {
                raTotalLcom.Add(LcomD);
            }
        }
    }
    // Only copy if raTotalLcom has elements
    if (raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}


void CLoadCombCtrl::Add_CvlTMH07_CS_D(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dD1, double& dD2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
	if (m_nCSCount == 0) return;

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nLcom = raNewLcom.GetSize();

	CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

	/*int nTempDNum = 0;

	if (m_nConstLoad == 1)
	{
		nTempDNum = 1;
	}
	else
	{
		if (m_nBD21DNum == 2)
		{
			nTempDNum = 2;
		}
		else
		{
			nTempDNum = 1;
		}
	}*/
	double dDFac[2];
	if (enLcomNo == EN_TMH07_NO_02_01) {
		dDFac[0] = dD2;
		dDFac[1] = dD2;
	}
	else {
		dDFac[0] = dD1;
		dDFac[1] = dD1;
	}

	for (int nD = 0; nD < m_nBD21DNum; ++nD)
	{
		for (int i = 0; i < nLcom; ++i)
		{
			T_LCOM_D LcomD = raNewLcom[i];
			int nExistCase = LcomD.aCombination.GetSize();
			int nn = 0, nCaseNum = nExistCase;

			double dFactor = 0.0;
			if (nD == 0)
			{
				dFactor = dD1;  
			}
			else
			{
				dFactor = dD2;
			}

			/*	if (nTempDNum == 2)
				{*/
				for (int j = 0; j < nExistCase; ++j)
				{
					if (LcomD.aCombination[j].AnalType == 1 && (LcomD.aCombination[j].Factor == dD1 || LcomD.aCombination[j].Factor == dD2))
					{
						dFactor = LcomD.aCombination[j].Factor;
						break;
					}
				}
		/*	}*/


			if (CheckCS_SgldK(D_SGLD_DL_CVL, nn))
			{
				Set_CStageCase(arCSLoad.GetAt(nn), dFactor, nCaseNum, LcomD, TRUE);
			}

			CString strcEL;
			int nErection = 0;

			for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
			{
				strcEL = _T(""); nErection = 0;
				if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

				if (CheckCSEL(nErection, _T("D")))
				{
					Set_CStageCase(arCSLoad.GetAt(nn), dFactor, nCaseNum, LcomD, TRUE);
				}
			}

			nErection = 0;

			CString csPrefix(_T("")), csLcom(_T(""));

			if (CheckCS_SgldK(1, nn))
			{
				csLcom += csPrefix + GetFactorString(dFactor) + _T("(cD)"); csPrefix = _T("+");
			}

			for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
			{
				strcEL = _T(""); nErection = 0;
				if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

				if (CheckCSEL(nErection, _T("D")))
				{
					csLcom += csPrefix + GetFactorString(dFactor) + strcEL; csPrefix = _T("+");
				}
			}
			if (csLcom.IsEmpty() || LcomD.Description == strTitle)
			{
				csPrefix = _T("");
			}
			LcomD.Description += csPrefix + csLcom;

			if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
		}
	}
    if (raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlTMH07_CS_DW(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dDw1, double& dDw2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
	if (m_nCSCount == 0) return; // Static only or no CS

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nLcom = raNewLcom.GetSize();

	CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

	/*int nTempDwNum = 0;

	if (m_nConstLoad == 1)
	{
		nTempDwNum = 1;
	}
	else
	{
		if (m_nBD21DWNum == 2)
		{
			nTempDwNum = 2;
		}
		else
		{
			nTempDwNum = 1;
		}
	}*/
	double dDwFac[2];
	if (enLcomNo == EN_TMH07_NO_02_01) {
		dDwFac[0] = dDw2;
		dDwFac[1] = dDw2;
	}
	else {
		dDwFac[0] = dDw1;
		dDwFac[1] = dDw1;
	}


	for (int nDw = 0; nDw < m_nBD21DWNum; ++nDw)
	{
		for (int i = 0; i < nLcom; ++i)
		{
			T_LCOM_D LcomD = raNewLcom[i];
			int nExistCase = LcomD.aCombination.GetSize();
			int nn = 0, nCaseNum = nExistCase;
			double dFactor = 0.0;
			if (nDw == 0)
			{
                if (enLcomNo != EN_TMH07_NO_02_01) {
                    dFactor = dDw1;
                }
                else {
                    dFactor = dDw2;
                }
			}
			else
			{
				dFactor = dDw2;
			}

			/*if (nTempDwNum == 2)
			{*/
				for (int j = 0; j < nExistCase; ++j)
				{
					if (LcomD.aCombination[j].AnalType == 1 && (LcomD.aCombination[j].Factor == dDw1 || LcomD.aCombination[j].Factor == dDw2))
					{
						dFactor = LcomD.aCombination[j].Factor;
						break;
					}
				}
			/*}*/


			CString strcEL;
			int nErection = 0;

			for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
			{
				strcEL = _T(""); nErection = 0;
				if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

                if (CheckCSEL(nErection, _T("DW")) ||
                    CheckCSEL(nErection, _T("LIP")) ||
                    CheckCSEL(nErection, _T("PL")) ||
                    CheckCSEL(nErection, _T("BL")))
                {
                    Set_CStageCase(arCSLoad.GetAt(nn), dFactor, nCaseNum, LcomD, TRUE);
                }

			}

			nErection = 0;

			CString csPrefix(_T("")), csLcom(_T(""));

			for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
			{
				strcEL = _T(""); nErection = 0;
				if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

                if (CheckCSEL(nErection, _T("DW")) ||
                    CheckCSEL(nErection, _T("LIP")) ||
                    CheckCSEL(nErection, _T("PL")) ||
                    CheckCSEL(nErection, _T("BL")))
                {
                    csLcom += csPrefix + GetFactorString(dFactor) + strcEL; csPrefix = _T("+");
                }
			}
			if (csLcom.IsEmpty() || LcomD.Description == strTitle)
			{
				csPrefix = _T("");
			}
			LcomD.Description += csPrefix + csLcom;
			if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
		}
	}
    if (raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlTMH07_CS_DC(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dDc1, double& dDc2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom)
{
	if (m_nCSCount == 0) return; // Static only or no CS

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nLcom = raNewLcom.GetSize();

	CArray<T_LCOM_D, T_LCOM_D &> raTotalLcom;

	/*int nTempDcNum = 0;

	if (m_nConstLoad == 1)
	{
		nTempDcNum = 1;
	}
	else
	{
		if (m_nBD21DCNum == 2)
		{
			nTempDcNum = 2;
		}
		else
		{
			nTempDcNum = 1;
		}
	}*/
	double dDcFac[2];
	if (enLcomNo == EN_TMH07_NO_02_01) {
		dDcFac[0] = dDc2;
		dDcFac[1] = dDc2;
	}
	else {
		dDcFac[0] = dDc1;
		dDcFac[1] = dDc1;
	}


	for (int nDc = 0; nDc < m_nBD21DCNum; ++nDc)
	{
		for (int i = 0; i < nLcom; ++i)
		{
			T_LCOM_D LcomD = raNewLcom[i];
			int nExistCase = LcomD.aCombination.GetSize();
			int nn = 0, nCaseNum = nExistCase;

			double dFactor = 0.0;
			if (nDc == 0)
			{
				dFactor = dDc1;
			}
			else
			{
				dFactor = dDc2;
			}

			/*if (nTempDcNum == 2)
			{*/
				for (int j = 0; j < nExistCase; ++j)
				{
					if (LcomD.aCombination[j].AnalType == 1 && (LcomD.aCombination[j].Factor == dDc1 || LcomD.aCombination[j].Factor == dDc2))
					{
						dFactor = LcomD.aCombination[j].Factor;
						break;
					}
				}

		/*}*/


			CString strcEL;
			int nErection = 0;

			for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
			{
				strcEL = _T(""); nErection = 0;
				if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

				if (CheckCSEL(nErection, _T("DC")))
				{
					Set_CStageCase(arCSLoad.GetAt(nn), dFactor, nCaseNum, LcomD, TRUE);
				}

			}

			nErection = 0;

			CString csPrefix(_T("")), csLcom(_T(""));

			for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
			{
				strcEL = _T(""); nErection = 0;
				if (CheckCS_SgldK(m_CSEL_SgldK[nErec], nn)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }

				if (CheckCSEL(nErection, _T("DC")))
				{
					csLcom += csPrefix + GetFactorString(dFactor) + strcEL; csPrefix = _T("+");
				}
			}
			if (csLcom.IsEmpty() || LcomD.Description == strTitle)
			{
				csPrefix = _T("");
			}
			LcomD.Description += csPrefix + csLcom;

			if (nCaseNum > 0)    raTotalLcom.Add(LcomD);
		}
	}
    if (raTotalLcom.GetSize() > 0)
    {
        raNewLcom.Copy(raTotalLcom);
    }
}

void CLoadCombCtrl::Add_CvlTMH07_CS_EQ(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dEqFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom,double& dGf3ULS,double& dGf3SLS)
{
	if (m_nConstLoad == 0 || m_nCSCount == 0) return; // Static only or no CS
    if (bStrength) {
        dEqFactor = 1.3 * dGf3ULS;
    }
    else {
        dEqFactor = 1.0 * dGf3SLS;
    }

	CString strTitle;
	strTitle = Add_CvlTMH07_Title(enLcomNo, bStrength);

	int nLcom = raNewLcom.GetSize();
	CArray<T_LCOM_D, T_LCOM_D&> raTotalLcom;

	for (int i = 0; i < nLcom; ++i)
	{
		T_LCOM_D LcomD = raNewLcom[i];
		int nExistCase = LcomD.aCombination.GetSize();

		// Process each construction stage load case with alternating factors
		for (int nEqm = 0; nEqm < m_nCSCount * 2; ++nEqm) // Multiply by 2 for positive and negative factors
		{
			int nn = 0, nCaseNum = nExistCase;
			double dFactor = (nEqm % 2 == 0) ? dEqFactor : -1.0 * dEqFactor; // Alternate between positive and negative factors

			CString strcEL;
			int nErection = 0;
			strcEL = _T("");

			if (CheckCS_SgldK(m_CSEL_SgldK[nEqm / 2], nn)) // Use nEqm / 2 to iterate over load cases
			{
				strcEL = m_CSEL_Sgld_Desc[nEqm / 2];
				nErection = m_CSEL_SgldK[nEqm / 2];
			}

			if (CheckCSEL(nErection, _T("E")))
			{
				Set_CStageCase(arCSLoad.GetAt(nn), dFactor, nCaseNum, LcomD, TRUE);
			}

			CString csPrefix(_T("")), csLcom(_T(""));

			// Build description for EQ load cases
			if (CheckCS_SgldK(m_CSEL_SgldK[nEqm / 2], nn))
			{
				strcEL = m_CSEL_Sgld_Desc[nEqm / 2];
				nErection = m_CSEL_SgldK[nEqm / 2];
			}

			if (CheckCSEL(nErection, _T("E")))
			{
				CString strFixE;
				strFixE.Format(_T("%d"), nEqm / 2 + 1); // Load case index
				csLcom += csPrefix + GetFactorString(dFactor) + _T("E[") + strFixE + _T("]");
				csPrefix = _T("+");
			}

			if (csLcom.IsEmpty() || LcomD.Description == strTitle)
			{
				csPrefix = _T("");
			}
			LcomD.Description += csPrefix + csLcom;

			if (nCaseNum > 0 && CheckCSEL(nErection, _T("E")))
			{
				raTotalLcom.Add(LcomD);
			}
		}
	}

	if (raTotalLcom.GetSize() > 0)
	{
		raNewLcom.Copy(raTotalLcom);
	}
}



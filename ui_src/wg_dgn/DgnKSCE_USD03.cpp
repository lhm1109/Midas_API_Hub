// DgnKSCE_USD03.cpp: implementation of the CDgnKSCE_USD03 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "DgnKSCE_USD03.h"

#include "CRCForceCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_AnalysisResult.h"
#include "..\wg_base\wg_base_I_PolyMaker.h"
#include "..\wg_db\wg_db_MathFunc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define DEGTORAD (3.14159265358979323846/180.0)

CDgnKSCE_USD03::CDgnKSCE_USD03()
{
    m_pDoc = CDBDoc::GetDocPoint();
    ASSERT(m_pDoc);
    m_dZero=1.0E-07;
    m_PSC_iITER = 100;
}

CDgnKSCE_USD03::~CDgnKSCE_USD03()
{
}

///////////////////////////////////////////
// For Connection to CRCDataCtrl.

void CDgnKSCE_USD03::SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl)
{
    CDgnPscCommon::SetDataCtrlPointer(pDataCtrl);
	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	// Set Code Unit.
	T_UNIT_INDEX CodeUnit = CurrUnit;
	CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_CM;
    CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_KG;
    m_pDataCtrl->Set_UnitIndex(CodeUnit);
    //m_pDataCtrl->Get_CurrUnitIndex(CodeUnit);

    m_Print.SetDataCtrlPointer(m_pDataCtrl,m_pForcCtrl);

    Make_PscDgnData();
}

CPrtKSCE_USD03* CDgnKSCE_USD03::Get_PrtKscePointer()	{return &m_Print;}

BOOL CDgnKSCE_USD03::Get_KsceFysg(ElemPairK ElemK, T_FYSG_D& FysgD)
{
    BOOL bCalcOK=TRUE;
    _PSC_MATL_ELEM MatlElem;
    if(!Get_PscMatlElem(ElemK, MatlElem))
    {
        return FALSE;
    }
    _PSC_SECT_ELEM SectElem;
    if(!Get_PscSectElem(FALSE, ElemK, SectElem))
    {
        return FALSE;
    }
    _PSC_RBAR_ELEM RbarElem;
    if(!Get_PscRbarElem(FALSE, ElemK, RbarElem)) bCalcOK=FALSE;
    _PSC_TNDN_ELEM TndnElem;
    if(!Get_PscTndnElem(FALSE, ElemK, TndnElem)) bCalcOK=FALSE;

    int i=0, j=0;
    BOOL bPreTndn[2]={TRUE,TRUE};
    for(i=0 ; i<2 ; i++)  // 0=I, 1=J.
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        // Section
        T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;      
        // RBar
        CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
        arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
        // Tendon
        CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
        arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);

        // Pre, Post 가 같이 있을 경우 Pre, Post 결정은 넓이로 한다. 
        double dApPre =0.0;
        double dApPost=0.0;
        for(j=0; j<arTndnPosi.GetSize(); j++)
        {
            _DGN_TNDN_CRC Tndn = arTndnPosi.GetAt(j);
            if(Tndn.iLoadType==0)      dApPre  += Tndn.dArea;
            else if(Tndn.iLoadType==1) dApPost += Tndn.dArea;
        }

        if(dApPre<dApPost) bPreTndn[i] = FALSE;

        for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
        {
            if(FysgD.FysgBase[i*2+j].bCHK==FALSE)  continue;
            double dalComf, dalTenf;
            if(!Get_BeforeAllowbleStressofConc(bPreTndn[i], MatlElem.MatdD, SpscSect, arRbarPosi, arTndnPosi, FysgD.FysgBase[i*2+j] , dalComf, dalTenf)) continue;

            double dExtraRatio  = 1.0;
            /*
            double dAllowStress = 0.0;     
            if(FysgD.FysgBase[i*2+j].dFMAX>0.0) dAllowStress = dExtraRatio*dalComf;
            else                                dAllowStress = dExtraRatio*dalTenf;      
            // Set Data. CodeUnit -> CurrUnit.
            FysgD.FysgBase[i*2+j].dALW = -1.0*dAllowStress;
            */
            // Set Data. CodeUnit -> CurrUnit.
            FysgD.FysgBase[i*2+j].dALW = (-1)*dExtraRatio*(j==0 ? dalComf : dalTenf);
        }
    }   
    return bCalcOK;
}

// 2. 사용하중시 PC강재 최대응력도 (텐던별)
BOOL CDgnKSCE_USD03::Get_KsceFypc(T_TDNA_K TdnaK, T_FYPC_KSCE& FypcKsce)
{
    Get_AllowbleStressofPC(TdnaK, FypcKsce);
    return TRUE;
}	

//  3. 합성응력(FY_COM)
BOOL CDgnKSCE_USD03::Get_KsceFycm(ElemPairK ElemK, T_FYCM_D& FycmD)
{
    BOOL bCalcOK=TRUE;

    _PSC_MATL_ELEM MatlElem;
    if(!Get_PscMatlElem(ElemK, MatlElem))
    {
        return FALSE;
    }
    _PSC_SECT_ELEM SectElem;
    if(!Get_PscSectElem(FALSE, ElemK, SectElem))
    {
        return FALSE;
    }
    _PSC_RBAR_ELEM RbarElem;
    if(!Get_PscRbarElem(FALSE, ElemK, RbarElem)) bCalcOK=FALSE;
    _PSC_TNDN_ELEM TndnElem;
    if(!Get_PscTndnElem(FALSE, ElemK, TndnElem)) bCalcOK=FALSE;

    int i=0, j=0;
    BOOL bPreTndn[2]={TRUE,TRUE};
    for(i=0 ; i<2 ; i++)  // 0=I, 1=J.
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        // Section
        T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;      
        // RBar
        CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
        arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
        // Tendon
        CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
        arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);

        // Pre, Post 가 같이 있을 경우 Pre, Post 결정은 넓이로 한다. 
        double dApPre =0.0;
        double dApPost=0.0;
        for(j=0; j<arTndnPosi.GetSize(); j++)
        {
            _DGN_TNDN_CRC Tndn = arTndnPosi.GetAt(j);
            if(Tndn.iLoadType==0)      dApPre  += Tndn.dArea;
            else if(Tndn.iLoadType==1) dApPost += Tndn.dArea;
        }

        if(dApPre<dApPost) bPreTndn[i] = FALSE;

        for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
        {
            if(FycmD.FycmBase[i*2+j].bCHK==FALSE)  continue;
            double dalComf, dalTenf;
            if(!Get_AfterAllowbleStressofConc(bPreTndn[i], MatlElem.MatdD, SpscSect, arRbarPosi, arTndnPosi, FycmD.FycmBase[i*2+j], dalComf, dalTenf)) continue;
            double dExtraRatio  = m_pForcCtrl->Get_ExtraRatio(FycmD.FycmBase[i*2+j].LcomK);
            /*
            double dAllowStress = 0.0;     
            if(FycmD.FycmBase[i*2+j].dFMAX>0.0) dAllowStress = dExtraRatio*dalComf;
            else                                dAllowStress = dExtraRatio*dalTenf;            
            // Set Data. CodeUnit -> CurrUnit.
            FycmD.FycmBase[i*2+j].dALW =(-1.0*dAllowStress);
            */
            // Set Data. CodeUnit -> CurrUnit.
            FycmD.FycmBase[i*2+j].dALW = (-1)*dExtraRatio*(j==0 ? dalComf : dalTenf);
            //FycmD.FycmBase[i*2+j].dALWLC = (-1)*(j==0 ? dalComf : dalTenf); // 계산서에서 사용
        }
    }
    return bCalcOK;
}

//  4. 휨 균열폭 (BND_CW)
BOOL CDgnKSCE_USD03::Get_KsceBdcw(ElemPairK ElemK, T_BDCW_D& BdcwD)
{
    BOOL bCalcOK=TRUE;

    int i,j,k;
    // Get Crack Width.
    T_PSCD_D PscdD; PscdD.Initialize();
    m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);

    _PSC_MATL_ELEM MatlElem;
    if(!Get_PscMatlElem(ElemK, MatlElem))
    {
        return FALSE;
    }
    _PSC_SECT_ELEM SectElem;
    if(!Get_PscSectElem(TRUE, ElemK, SectElem))
    {
        return FALSE;
    }
    _PSC_SECG_ELEM SecgElem;
    if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
    _PSC_RBAR_ELEM RbarElem;
    if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
    _PSC_TNDN_ELEM TndnElem;
    if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;

    T_MATD_D MatdD = MatlElem.MatdD;

    for(i=0 ; i<2 ; i++)  // 0=I, 1=J
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        // Section
        T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;
        CArray<T_SPSC_COOR,T_SPSC_COOR> arOuter;
        arOuter.Copy(SectElem.SectPosi[i].arOuter);
        CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*> arpInner;  
        int iInnerCellNum = SectElem.SectPosi[i].iInnerCellNum;
        for(int m=0; m<iInnerCellNum; m++)
        {
            CArray<T_SPSC_COOR,T_SPSC_COOR> arInner;
            arInner.Copy(SectElem.SectPosi[i].arInner[m]);
            arpInner.Add(&arInner);
        }

        // RBar
        CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
        arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
        // Tendon
        CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
        arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);

        for(j=0 ; j<2 ; j++) // 0=Top, 1=Bot
        {
            _DGN_FORC_CRC ForcLcomNTP;
            // FALSE : Except Tendon Primary
            if(!m_pDataCtrl->Get_ConForc(ElemK, FALSE, BdcwD.BdcwBase[i*2+j].LcomK, ForcLcomNTP)) continue;
            double dMu_tp = ForcLcomNTP.dMuy[i];
            _DGN_FORC_CRC ForcLcom;
            if(!m_pDataCtrl->Get_ConForc(ElemK, TRUE, BdcwD.BdcwBase[i*2+j].LcomK, ForcLcom)) continue;
            double dMu = ForcLcom.dMuy[i];
            double dftop = -1.0*(BdcwD.BdcwBase[i*2+j].dFT);
            double dfbot = -1.0*(BdcwD.BdcwBase[i*2+j].dFB);
            double dfs, dw, dwa;
            if(!Get_CrackCheck(PscdD.iCrackWidth[j], MatdD, SpscSect, arOuter, arpInner, arRbarPosi, arTndnPosi, dMu, dftop, dfbot, dMu_tp, dfs, dw, dwa)) continue;
            BdcwD.BdcwBase[i*2+j].dFRT = (dfs);
            BdcwD.BdcwBase[i*2+j].dWC  = dw; 
            BdcwD.BdcwBase[i*2+j].dAWC = dwa; 
        }
        // Delete because of New Operator.
        for(k=0; k<arpInner.GetSize(); k++)
        {
            if(arpInner[k])
            {
                arpInner[k]->RemoveAll();
                delete arpInner[k];
                arpInner[k] = NULL; 
            }
        }
        arpInner.RemoveAll();    
    }

    return bCalcOK;
}

//  5. 인장철근 (RBAR_T)
BOOL CDgnKSCE_USD03::Get_KsceTbar(ElemPairK ElemK, T_TBAR_D& TbarD, BOOL bI/*TRUE*/, BOOL bJ/*TRUE*/, int iUseKind/*0*/)
{
    // iUseKind (0=Not Use Result, 1=Top, 2=Bot).
    BOOL bCalcOK=TRUE;

    int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
    ADGNFORCE aForceLcom;
    ADGNSTRSADD aStressAdd;
    //!/ Tendon 포함이 맞는지 확인
    if(!m_pForcCtrl->Get_LcomDataForStress(ElemK, TRUE, FALSE, aForceLcom, aStressAdd, iLcomNum))	return FALSE;
    ASSERT(aForceLcom.GetSize()==aStressAdd.GetSize());
    if(m_pForcCtrl->Get_LcomNumForStrs()==0)	return FALSE;

    ADGNSTRESS aStressLcom;
    ADGNSTRESS aStressTnpr;
    if(!m_pForcCtrl->Get_LcomDataForStress(ElemK, 0/*nFortype*/,TRUE, FALSE, aStressLcom, aStressTnpr, iLcomNum)) return FALSE;

    // Get Tbar.
    if(iUseKind > 0)
    {
        TbarD.Initialize();
        if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetTbarData(ElemK,TbarD))	return FALSE;
    }

    _PSC_MATL_ELEM MatlElem;
    if(!Get_PscMatlElem(ElemK, MatlElem))
    {
        return FALSE;
    }
    _PSC_SECT_ELEM SectElem;
    if(!Get_PscSectElem(TRUE, ElemK, SectElem))
    {
        return FALSE;
    }
    _PSC_SECG_ELEM SecgElem;
    if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
    _PSC_RBAR_ELEM RbarElem;
    if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
    _PSC_TNDN_ELEM TndnElem;
    if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;

    T_MATD_D MatdD = MatlElem.MatdD;

    int iSta = (bI ? 0 : 1);	// I.
    int iEnd = (bJ ? 2 : 1);	// J.
    for(int i=iSta; i<iEnd; i++)
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        BOOL bPrint = m_Print.Get_PrintFlag();    

        // Section
        T_SPSC_SECT SpscSect; SpscSect = SectElem.SectPosi[i].SpscSect;
        CArray<T_SPSC_COOR,T_SPSC_COOR> arOuter;
        arOuter.Copy(SectElem.SectPosi[i].arOuter);
        CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*> arpInner;  
        int iInnerCellNum = SectElem.SectPosi[i].iInnerCellNum;
        for(int m=0; m<iInnerCellNum; m++)
        {
            CArray<T_SPSC_COOR,T_SPSC_COOR> arInner;
            arInner.Copy(SectElem.SectPosi[i].arInner[m]);
            arpInner.Add(&arInner);
        }
        // Section Gross
        T_SPSC_SECT SpscgSect; SpscgSect = SecgElem.SectPosi[i];

        // RBar
        CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
        arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
        // Tendon
        CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
        arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);

        // bw
        double dShrThk[3];
        dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
        dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
        dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
        double dbw = min(min(dShrThk[0],dShrThk[1]),dShrThk[2]);

        if(iUseKind > 0)	// Use Result.
        {
            // Forc.
            for(int j=0; j<2; j++) // Top, Bot.
            {
                BOOL bTopCalc = (j==0 ? TRUE : FALSE);
                int k = i*2+j;
                if(TbarD.TbarBase[k].bCHK==FALSE) continue;
                if(bPrint)  m_Print.Print_TbarElementIJ(ElemK.first, bPosiI, bTopCalc);      
                int iDgnLcomNo = TbarD.TbarBase[k].LcomK;

                if(iDgnLcomNo==0) continue;
                int m = iDgnLcomNo-1;
                _DGN_STRS_CRC StressCrc = aStressLcom.GetAt(m);
                //_DGN_STRS_ADD StrsAdd = aStressAdd.GetAt(j);
                _DGN_STRS_CRC StrsMax, StrsMin;
                StrsMax.Initialize();
                StrsMin.Initialize();
                if(!m_pForcCtrl->Get_StrsByStrs(ElemK, iDgnLcomNo, StressCrc, StrsMax, StrsMin))	continue;
                double dftop = -StrsMin.dStop[i];
                double dfbot = -StrsMin.dSbot[i];
                double dAreq, dAuse, dAmin;
                if(!Get_TensionRebar(bTopCalc, bPrint, MatdD, dbw, SpscSect, arOuter, arpInner, arRbarPosi, arTndnPosi, dftop, dfbot, dAreq, dAuse, dAmin)) continue;
            }
        }
        else	// Not Use Result.
        {
            UINT TopMaxLcomK=0, BotMaxLcomK=0;
            double dAreqTopMax=0.0, dAminTopMax=0.0, dAuseTopMax=0.0;
            double dAreqBotMax=0.0, dAminBotMax=0.0, dAuseBotMax=0.0;

            // Get Tbar.
            _DGN_STRS_CRC StrsMax, StrsMin;
            for(int j=0; j<2; j++) // Top, Bot.
            {
                BOOL bTopCalc = (j==0 ? TRUE : FALSE);
                int k = i*2+j;
                if(TbarD.TbarBase[k].bCHK==FALSE) continue;
                int iDgnLcomNo = TbarD.TbarBase[k].LcomK;
                int m = iDgnLcomNo-1;
                _DGN_STRS_CRC StressCrc = aStressLcom.GetAt(m);
                //_DGN_STRS_ADD StrsAdd = aStressAdd.GetAt(m);
                StrsMax.Initialize();
                StrsMin.Initialize();
                if(!m_pForcCtrl->Get_StrsByStrs(ElemK, iDgnLcomNo, StressCrc, StrsMax, StrsMin))	continue;
                double dftop = -StrsMin.dStop[i];
                double dfbot = -StrsMin.dSbot[i];
                double dAreq=0.0, dAuse=0.0, dAmin=0.0;
                if(j==0)
                { 
                    if(dftop>0.0) 
                    {
                        if(!Get_TensionRebar(bTopCalc, bPrint, MatdD, dbw, SpscSect, arOuter, arpInner, arRbarPosi, arTndnPosi, dftop, dfbot, dAreq, dAuse, dAmin)) continue;
                        else {TopMaxLcomK=iDgnLcomNo; dAreqTopMax=dAreq; dAminTopMax=dAmin; dAuseTopMax=dAuse;}
                    }
                    else TbarD.TbarBase[k].LcomK = 0;
                }
                else if(j==1)
                {
                    if(dfbot>0.0)
                    {
                        if(!Get_TensionRebar(bTopCalc, bPrint, MatdD, dbw, SpscSect, arOuter, arpInner, arRbarPosi, arTndnPosi, dftop, dfbot, dAreq, dAuse, dAmin)) continue;
                        else {BotMaxLcomK=iDgnLcomNo; dAreqBotMax=dAreq; dAminBotMax=dAmin; dAuseBotMax=dAuse;}
                    }
                    else TbarD.TbarBase[k].LcomK = 0;
                }        
            }
            // 0=I-TOP-MAX, 1=I-BOT-MAX, 2=J-TOP-MAX, 3=J-BOT-MAX.
            if(TopMaxLcomK > 0)
            {
                CString strOrgLcomNaTopMax=_T("");
                int iMaxMinTypeTopMax=0;
                if(!m_pForcCtrl->Get_LcomType(TopMaxLcomK, strOrgLcomNaTopMax, iMaxMinTypeTopMax))	ASSERT(0);
                TbarD.TbarBase[i*2+0].LcomK	= TopMaxLcomK;
                TbarD.TbarBase[i*2+0].iKind	= m_pForcCtrl->Get_LcomKind(TopMaxLcomK);
                TbarD.TbarBase[i*2+0].iMax	= iMaxMinTypeTopMax;
                TbarD.TbarBase[i*2+0].dREQ	= dAreqTopMax;
                TbarD.TbarBase[i*2+0].dMIN	= dAminTopMax;
                TbarD.TbarBase[i*2+0].dUSE	= dAuseTopMax;
            }
            if(BotMaxLcomK > 0)
            {
                CString strOrgLcomNaBotMax=_T("");
                int iMaxMinTypeBotMax=0;
                if(!m_pForcCtrl->Get_LcomType(BotMaxLcomK, strOrgLcomNaBotMax, iMaxMinTypeBotMax))	ASSERT(0);
                TbarD.TbarBase[i*2+1].LcomK	= BotMaxLcomK;
                TbarD.TbarBase[i*2+1].iKind	= m_pForcCtrl->Get_LcomKind(BotMaxLcomK);
                TbarD.TbarBase[i*2+1].iMax	= iMaxMinTypeBotMax;
                TbarD.TbarBase[i*2+1].dREQ	= dAreqBotMax;
                TbarD.TbarBase[i*2+1].dMIN	= dAminBotMax;
                TbarD.TbarBase[i*2+1].dUSE	= dAuseBotMax;
            }
        }

        // Delete because of New Operator.
        for(int k=0; k<arpInner.GetSize(); k++)
        {
            if(arpInner[k])
            {
                arpInner[k]->RemoveAll();
                delete arpInner[k];
                arpInner[k] = NULL; 
            }
        }
        arpInner.RemoveAll();  

    }
    return bCalcOK;
}

// 가설중 경사 인장 응력도 (FST_SG)
BOOL CDgnKSCE_USD03::Get_KsceFssg(ElemPairK ElemK, T_FPMT_JTG& FpmtKsce)
{
    _PSC_MATL_ELEM MatlElem;
    if(!Get_PscMatlElem(ElemK, MatlElem))
    {
        return FALSE;
    }
    T_MATD_D MatdD = MatlElem.MatdD;
    double dAFS=0., dAFST=0.;      
    Get_AllowblePrincipalStressOfConc(MatdD, dAFS, dAFST);
    for(int i=0 ; i<2 ; i++)  // 0=I, 1=J.
    {
        for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
        {
            if(FpmtKsce.FpmtBase[i*2+j].bCHK==FALSE)  continue;

            FpmtKsce.FpmtBase[i*2+j].dAFP = -dAFST;      
        }
    }

    return TRUE;
}

// 설계하중시 경사 인장 응력도 (전단력 최대시) (FST_MS)
BOOL CDgnKSCE_USD03::Get_KsceFsms(ElemPairK ElemK, T_FPMS_JTG& FpmsKsce)
{
    _PSC_MATL_ELEM MatlElem;
    if(!Get_PscMatlElem(ElemK, MatlElem))
    {
        return FALSE;
    }
    T_MATD_D MatdD = MatlElem.MatdD;
    double dAFS=0., dAFST=0.;
    Get_AllowblePrincipalStressOfConc(MatdD, dAFS, dAFST);
    for(int i=0 ; i<2 ; i++)  // 0=I, 1=J.
    {
        for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
        {
            if(FpmsKsce.FpmsBase[i*2+j].bCHK==FALSE)  continue;

            FpmsKsce.FpmsBase[i*2+j].dAFP = -dAFST;
        }
    }

    return TRUE;
}

// 설계하중시 경사 인장 응력도 (비틀림 최대시) (FST_MT)
BOOL CDgnKSCE_USD03::Get_KsceFsmt(ElemPairK ElemK, T_FSMT_D& FsmtD)
{
    _PSC_MATL_ELEM MatlElem;
    if(!Get_PscMatlElem(ElemK, MatlElem))
    {
        return FALSE;
    }
    T_MATD_D MatdD = MatlElem.MatdD;
    double dAFS=0., dAFST=0.;
    Get_AllowblePrincipalStressOfConc(MatdD, dAFS, dAFST);
    for(int i=0 ; i<2 ; i++)  // 0=I, 1=J.
    {
        for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
        {
            if(FsmtD.FsmtBase[i*2+j].bCHK==FALSE)  continue;

            FsmtD.FsmtBase[i*2+j].dAFS  = -dAFS;
            FsmtD.FsmtBase[i*2+j].dAFST = -dAFST;
        }
    }

    return TRUE;
}

//6-1. 계수하중시 휨강도 (강축)
BOOL CDgnKSCE_USD03::Get_KsceBdcy(ElemPairK ElemK, T_BDCY_KSCE& BdcrKsce, BOOL bI/*TRUE*/, BOOL bJ/*TRUE*/)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    ASSERT(pDoc);

    T_PSCD_D PscdD; PscdD.Initialize();
    pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);

    BOOL bIter=FALSE; 
    if(PscdD.ifpsMethod==1) bIter = TRUE; //0=설계기준, 1=Iteration

    BOOL bCompoSect = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);
    if(bCompoSect) return FALSE;

    BOOL bCalcOK=TRUE;
    _PSC_MATL_ELEM MatlElem;
    if(!Get_PscMatlElem(ElemK, MatlElem))
    {
        return FALSE;
    }
    _PSC_SECT_ELEM SectElem;
    if(!Get_PscSectElem(TRUE, ElemK, SectElem))
    {
        return FALSE;
    }
    _PSC_SECG_ELEM SecgElem;
    if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
    _PSC_RBAR_ELEM RbarElem;
    if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
    _PSC_TNDN_ELEM TndnElem;
    if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;

    // rp
    // PS강재의 종류(0=저릴랙세이션(0.28), 1=응력제거강재(0.4), 2=강봉(0.55))
    int iTndnKind = PscdD.iTndnKind;
    double drp = 0.0;    
    Get_TndnKindrp(iTndnKind, drp);

    int iSta = (bI ? 0 : 1);	// I.
    int iEnd = (bJ ? 2 : 1);	// J.
    for(int i=iSta; i<iEnd; i++)
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        BOOL bPrint = m_Print.Get_PrintFlag();
        //if(bPrint)  m_Print.Print_ElementIJ(ElemK, bPosiI, bPositive);      

        // Section g
        T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];
        // Section
        T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;
        CArray<T_SPSC_COOR,T_SPSC_COOR> arOuter;
        arOuter.Copy(SectElem.SectPosi[i].arOuter);
        CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*> arpInner;  
        int iInnerCellNum = SectElem.SectPosi[i].iInnerCellNum;
        for(int m=0; m<iInnerCellNum; m++)
        {
            CArray<T_SPSC_COOR,T_SPSC_COOR> arInner;
            arInner.Copy(SectElem.SectPosi[i].arInner[m]);
            arpInner.Add(&arInner);
        }

        // RBar
        CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
        arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
        if(!PscdD.bRefRebar) arRbarPosi.RemoveAll();
        // Tendon
        CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
        arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);
        BOOL bRebarExist=TRUE;
        BOOL bTndnExist=TRUE;
        if(arRbarPosi.GetSize()==0 && arTndnPosi.GetSize()==0)
        {
            bRebarExist=FALSE;
            bTndnExist=FALSE;
            CString sEndIJ = (bPosiI ? _T("I") : _T("J"));
            CString sErrMsg = _T("");
            sErrMsg.Format(_LS(IDS_DGN_PSC_NO_REBAR_TENDON_Err), ElemK, sEndIJ);
            //GSaveHistoryNF(sErrMsg);    
        }

        // bw
        double dShrThk[3];
        dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
        dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
        dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
        double dbw = min(min(dShrThk[0],dShrThk[1]),dShrThk[2]);
        double dFlgThk[2]={SectElem.SectPosi[i].dpFlgThk[0], SectElem.SectPosi[i].dpFlgThk[1]}; 
        double dpMomPos[2][2];
        dpMomPos[0][0] = SectElem.SectPosi[i].dpMomPos[2][0];
        dpMomPos[0][1] = SectElem.SectPosi[i].dpMomPos[2][1];
        dpMomPos[1][0] = SectElem.SectPosi[i].dpMomPos[3][0];
        dpMomPos[1][1] = SectElem.SectPosi[i].dpMomPos[3][1];    

        //Phi
        double dPhib, dPhiv;
        BOOL bPhi = m_pDataCtrl->Get_KscePhi(dPhib, dPhiv);

        for(int j=0 ; j<2 ; j++) //Max, Min
        {
            int k = i*2+j;
            if(!bRebarExist && !bTndnExist) BdcrKsce.BdcyBase[k].bCHK=FALSE;
            if(BdcrKsce.BdcyBase[k].bCHK==FALSE) continue;
            BOOL bPositive = (j==0 ? TRUE : FALSE);
            if(bPrint)  m_Print.Print_ElementIJ(ElemK.first, bPosiI, bPositive);      
            if(!Get_FlexureStrength(TRUE, bPosiI, bIter, bPositive, dPhib, MatlElem, SpscSect, arOuter, arpInner, SpscgSect, 
                drp, dbw, dFlgThk, dpMomPos, arRbarPosi, arTndnPosi, BdcrKsce.BdcyBase[k])) continue;
        }
        // Delete because of New Operator.
        for(int k=0; k<arpInner.GetSize(); k++)
        {
            if(arpInner[k])
            {
                arpInner[k]->RemoveAll();
                delete arpInner[k];
                arpInner[k] = NULL; 
            }
        }
        arpInner.RemoveAll();  

    }
    return bCalcOK;
}

// 6-2. 계수하중시 휨강도 (약축)
BOOL CDgnKSCE_USD03::Get_KsceBdcz(ElemPairK ElemK, T_BDCZ_KSCE& BdcrKsce)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    ASSERT(pDoc);

    T_PSCD_D PscdD; PscdD.Initialize();
    pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);

    BOOL bCalcOK = TRUE;
    _PSC_MATL_ELEM MatlElem;
    if(!Get_PscMatlElem(ElemK, MatlElem))
    {
        return FALSE;
    }
    _PSC_SECT_ELEM SectElem;
    if(!Get_PscSectElem(TRUE, ElemK, SectElem))
    {
        return FALSE;
    }
    _PSC_SECG_ELEM SecgElem;
    if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
    _PSC_RBAR_ELEM RbarElem;
    if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
    _PSC_TNDN_ELEM TndnElem;
    if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;

    for(int i=0 ; i<2 ; i++)  //I,J
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        // Section g
        T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];
        // Section
        T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;
        CArray<T_SPSC_COOR,T_SPSC_COOR> arOuter;
        arOuter.Copy(SectElem.SectPosi[i].arOuter);
        CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*> arpInner;  
        int iInnerCellNum = SectElem.SectPosi[i].iInnerCellNum;
        for(int m=0; m<iInnerCellNum; m++)
        {
            CArray<T_SPSC_COOR,T_SPSC_COOR> arInner;
            arInner.Copy(SectElem.SectPosi[i].arInner[m]);
            arpInner.Add(&arInner);
        }
        // RBar
        CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
        arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
        if(!PscdD.bRefRebar) arRbarPosi.RemoveAll();
        // Tendon
        CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
        arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);
        BOOL bRebarExist=TRUE;
        BOOL bTndnExist=TRUE;
        if(arRbarPosi.GetSize()==0 && arTndnPosi.GetSize()==0)
        {
            bRebarExist=FALSE;
            bTndnExist=FALSE;
            CString sEndIJ = (bPosiI ? _T("I") : _T("J"));
            CString sErrMsg = _T("");
            sErrMsg.Format(_LS(IDS_DGN_PSC_NO_REBAR_TENDON_Err), ElemK, sEndIJ);
            //GSaveHistoryNF(sErrMsg);    
        }

        // bw
        double dShrThk[3];
        dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
        dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
        dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
        double dbw = min(min(dShrThk[0],dShrThk[1]),dShrThk[2]);
        double dFlgThk[2]={SectElem.SectPosi[i].dpFlgThk[0], SectElem.SectPosi[i].dpFlgThk[1]}; 
        double dpMomPos[2][2];
        dpMomPos[0][0] = SectElem.SectPosi[i].dpMomPos[2][0];
        dpMomPos[0][1] = SectElem.SectPosi[i].dpMomPos[2][1];
        dpMomPos[1][0] = SectElem.SectPosi[i].dpMomPos[3][0];
        dpMomPos[1][1] = SectElem.SectPosi[i].dpMomPos[3][1];    

        //Phi
        double dPhib, dPhiv;
        BOOL bPhi = m_pDataCtrl->Get_KscePhi(dPhib, dPhiv);

        for(int j=0 ; j<2 ; j++) //Max, Min
        {
            int k = i*2+j;
            if(!bRebarExist && !bTndnExist) BdcrKsce.BdczBase[k].bCHK=FALSE;
            if(BdcrKsce.BdczBase[k].bCHK==FALSE) continue;
            BOOL bPositive = (j==0 ? TRUE : FALSE);
            if(!Get_FlexureStrength(TRUE, bPosiI, TRUE, bPositive, dPhib, MatlElem, SpscSect, arOuter, arpInner, SpscgSect, 
                dbw, dFlgThk, dpMomPos, arRbarPosi, arTndnPosi, BdcrKsce.BdczBase[k])) continue;
        }
        // Delete because of New Operator.
        for(int k=0; k<arpInner.GetSize(); k++)
        {
            if(arpInner[k])
            {
                arpInner[k]->RemoveAll();
                delete arpInner[k];
                arpInner[k] = NULL; 
            }
        }
        arpInner.RemoveAll();  
    }
    return bCalcOK;
}


//  7. 가설중 경사 인장 응력도 (FST_SG) - JSCE02와 동일
//  8. 설계하중시 경사 인장 응력도 (전단력 최대시) (FST_MS) - JSCE02와 동일
//  9. 설계하중시 전단균열 (전단력 최대시) (CW_MS) - 제외

// 10,11. 계수하중시 전단설계(전단력 최대시)
BOOL CDgnKSCE_USD03::Get_KsceCums(ElemPairK ElemK, T_CUMS_KSCE& CumsKsce, BOOL bI/*TRUE*/, BOOL bJ/*TRUE*/)
{
    BOOL bCalcOK = TRUE;
    _PSC_MATL_ELEM MatlElem;
    if(!Get_PscMatlElem(ElemK, MatlElem))
    {
        return FALSE;
    }
    _PSC_SECT_ELEM SectElem;
    if(!Get_PscSectElem(TRUE, ElemK, SectElem))
    {
        return FALSE;
    }
    _PSC_SECG_ELEM SecgElem;
    if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
    _PSC_RBAR_ELEM RbarElem;
    if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
    _PSC_TNDN_ELEM TndnElem;
    if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
    _PSC_RSHR_ELEM RshrElem;
    if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;

    T_MATD_D MatdD = MatlElem.MatdD;

    if(!m_pDataCtrl->Get_ConMatd(ElemK, MatdD)) return FALSE;
    int iSta = (bI ? 0 : 1);	// I.
    int iEnd = (bJ ? 2 : 1);	// J.
    for(int i=iSta; i<iEnd; i++)
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        BOOL bPrint = m_Print.Get_PrintFlag();

        // Section g
        T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];
        // Section
        T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;
        // RBar
        CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
        arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
        // Tendon
        CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
        arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);    
        // bw
        double dShrThk[3];
        dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
        dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
        dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
        double dbw = min(min(dShrThk[0],dShrThk[1]),dShrThk[2]);
        double dFlgThk[2]={SectElem.SectPosi[i].dpFlgThk[0], SectElem.SectPosi[i].dpFlgThk[1]}; 
        double dpMomPos[2][2];
        dpMomPos[0][0] = SectElem.SectPosi[i].dpMomPos[2][0];
        dpMomPos[0][1] = SectElem.SectPosi[i].dpMomPos[2][1];
        dpMomPos[1][0] = SectElem.SectPosi[i].dpMomPos[3][0];
        dpMomPos[1][1] = SectElem.SectPosi[i].dpMomPos[3][1];    

        //Shear&Torsion
        T_RPSC_SBAR RpscSbar = RshrElem.RshrPosi[i].RpscSbar;
        double dfdt = RshrElem.RshrPosi[i].dfdt;
        double dfdb = RshrElem.RshrPosi[i].dfdb;
        double dVd  = RshrElem.RshrPosi[i].dVd;
        double dMd  = RshrElem.RshrPosi[i].dMd;    

        //Phi
        double dPhib, dPhiv;
        BOOL bPhi = m_pDataCtrl->Get_KscePhi(dPhib, dPhiv);

        for(int j=0 ; j<2 ; j++) //Max, Min
        {
            int k = i*2+j;
            if(CumsKsce.CumsBase[k].bCHK==FALSE) continue;
            if(bPrint) m_Print.Print_ElementIJ(ElemK.first, bPosiI);
            if(!Get_ShearDesign(bPosiI, dPhiv, MatdD, SpscSect, SpscgSect, dbw, dfdt, dfdb, dVd, dMd, arRbarPosi, arTndnPosi, RpscSbar, CumsKsce.CumsBase[k])) continue;      
        }
    }
    return bCalcOK;
}

// 12. 설계하중시 경사 인장 응력도 (비틀림 최대시) (FST_MT) - JSCE02와 동일
// 13. 설계하중시 전단균열 (비틀림 최대시) (CW_MT) - 제외

// 14,15. 계수하중시 비틀림설계(비틀림 최대시)
BOOL CDgnKSCE_USD03::Get_KsceCrmt(ElemPairK ElemK, T_CRMT_KSCE& CrmtKsce, BOOL bI/*TRUE*/, BOOL bJ/*TRUE*/)
{
    BOOL bCalcOK = TRUE;

    _PSC_MATL_ELEM MatlElem;
    if(!Get_PscMatlElem(ElemK, MatlElem))
    {
        return FALSE;
    }
    _PSC_SECT_ELEM SectElem;
    if(!Get_PscSectElem(TRUE, ElemK, SectElem))
    {
        return FALSE;
    }
    _PSC_SECG_ELEM SecgElem;
    if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
    _PSC_RBAR_ELEM RbarElem;
    if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
    _PSC_TNDN_ELEM TndnElem;
    if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
    _PSC_RSHR_ELEM RshrElem;
    if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;

    T_MATD_D MatdD = MatlElem.MatdD;

    int iSta = (bI ? 0 : 1);	// I.
    int iEnd = (bJ ? 2 : 1);	// J.
    for(int i=iSta; i<iEnd; i++)
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        BOOL bPrint = m_Print.Get_PrintFlag();

        // Section g
        T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];
        // Section
        T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;
        // RBar
        CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
        arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
        // Tendon
        CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
        arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);    
        // bw
        double dShrThk[3];
        dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
        dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
        dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
        double dbw = min(min(dShrThk[0],dShrThk[1]),dShrThk[2]);
        double dFlgThk[2]={SectElem.SectPosi[i].dpFlgThk[0], SectElem.SectPosi[i].dpFlgThk[1]}; 
        double dpMomPos[2][2];
        dpMomPos[0][0] = SectElem.SectPosi[i].dpMomPos[2][0];
        dpMomPos[0][1] = SectElem.SectPosi[i].dpMomPos[2][1];
        dpMomPos[1][0] = SectElem.SectPosi[i].dpMomPos[3][0];
        dpMomPos[1][1] = SectElem.SectPosi[i].dpMomPos[3][1];    

        //Shear&Torsion
        T_RPSC_SBAR RpscSbar = RshrElem.RshrPosi[i].RpscSbar;
        double dfdt = RshrElem.RshrPosi[i].dfdt;
        double dfdb = RshrElem.RshrPosi[i].dfdb;
        double dVd  = RshrElem.RshrPosi[i].dVd;
        double dMd  = RshrElem.RshrPosi[i].dMd;    
        //Phi
        double dPhib, dPhiv;
        BOOL bPhi = m_pDataCtrl->Get_KscePhi(dPhib, dPhiv);

        BOOL bSolidSect;
        if(!SectElem.SectPosi[i].bClosedSect) bSolidSect=TRUE;    // bClosedSect -> Box형
        else                                  bSolidSect=FALSE;
        double dAoh = SectElem.SectPosi[i].dAoh;
        double dPh  = SectElem.SectPosi[i].dPh;
        double dKt  = SectElem.SectPosi[i].dKt;    
        double dTorThk = SectElem.SectPosi[i].dTorThk;
        double dPcp = SectElem.SectPosi[i].dPcp;
        double dAcp = SectElem.SectPosi[i].dAcp;

        for(int j=0 ; j<3 ; j++) //Max, Min
        {
            int k = i*3+j;
            if(CrmtKsce.CrmtBase[k].bCHK==FALSE) continue;
            if(bPrint) m_Print.Print_ElementIJ(ElemK.first, bPosiI);
            if(!Get_TorsionDesign(TRUE, bPosiI, dPhiv, MatdD, SpscSect, SpscgSect, bSolidSect, dAoh, dPh, dbw, dTorThk, dPcp, dAcp, dfdt, dfdb, dVd, dMd, arTndnPosi, arRbarPosi, RpscSbar, CrmtKsce.CrmtBase[k])) continue;
        }
    }
    return bCalcOK;
}

BOOL CDgnKSCE_USD03::Get_KsceTndn(ElemPairK ElemK, T_TNDN_KSCE& TndnKsce)
{
    return TRUE;
}

///////////////////////////////////////////
// For Calculation.

BOOL CDgnKSCE_USD03::Get_BeforeAllowbleStressofConc(BOOL bPre, T_MATD_D& MatdD, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbar, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndn,
                                                    T_FYSG_BASE FysgBase , double& dalComf, double& dalTenf)
{
    /*
    BOOL bTop;
    double dStress[6];
    double dMin = FysgBase.dFT;
    dStress[0] = FysgBase.dFT;
    dStress[1] = FysgBase.dFB;
    dStress[2] = FysgBase.dFTL;
    dStress[3] = FysgBase.dFBL;
    dStress[4] = FysgBase.dFTR;
    dStress[5] = FysgBase.dFBR;
    //dStress[6] = FysgD.FysgBase.dFMAX;
    for(int i=0 ; i<6 ; i++)
    {
    if(dMin>dStress[i]) bTop = (i%2==0 ? TRUE : FALSE);
    dMin = min(dMin,dStress[i]);    
    }
    */
    // Tens=(-), Comp=(+).
    BOOL bTop = (FysgBase.dFT < FysgBase.dFB ? TRUE : FALSE);

    double dfi[3] = {0., 0., 0.,};
    double dfe[3] = {0., 0., 0.,};
    if(!Get_AllowbleStressOfConc(bPre, MatdD, SpscSect, arRbar, arTndn, dfi, dfe)) return FALSE;

    // Compression Stress.
    dalComf = dfi[2];

    // Tension Stress
    if(bTop) dalTenf = dfi[0];
    else     dalTenf = dfi[1];

    return  TRUE;
}

BOOL CDgnKSCE_USD03::Get_AfterAllowbleStressofConc(BOOL bPre, T_MATD_D& MatdD, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbar, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndn,
                                                   T_FYCM_BASE FycmBase, double& dalComf, double& dalTenf)
{
    /*  
    BOOL bTop;
    double dStress[6];
    double dMin=FycmBase.dFT;
    dStress[0] = FycmBase.dFT;
    dStress[1] = FycmBase.dFB;
    dStress[2] = FycmBase.dFTL;
    dStress[3] = FycmBase.dFBL;
    dStress[4] = FycmBase.dFTR;
    dStress[5] = FycmBase.dFBR;
    //dStress[6] = FycmD.FycmBase.dFMAX;
    for(int i=0 ; i<6 ; i++)
    {
    if(dMin>dStress[i]) bTop = (i%2==0 ? TRUE : FALSE);
    dMin = min(dMin,dStress[i]);    
    }
    */
    // Tens=(-), Comp=(+).
    BOOL bTop = (FycmBase.dFT < FycmBase.dFB ? TRUE : FALSE);

    double dfi[3] = {0., 0., 0.,};
    double dfe[3] = {0., 0., 0.,};
    if(!Get_AllowbleStressOfConc(bPre, MatdD, SpscSect, arRbar, arTndn, dfi, dfe)) return FALSE;

    // Compression Stress.
    dalComf = dfe[2];

    // Tension Stress
    if(bTop) dalTenf = dfe[0];
    else     dalTenf = dfe[1];

    return TRUE;
}

BOOL CDgnKSCE_USD03::Get_AllowableStressofLcom(BOOL bStage, ElemPairK ElemK, double* dAlw)
{
    BOOL bCalcOK=TRUE;
    _PSC_MATL_ELEM MatlElem;
    if(!Get_PscMatlElem(ElemK, MatlElem))        return FALSE;
    _PSC_SECT_ELEM SectElem;
    if(!Get_PscSectElem(FALSE, ElemK, SectElem)) return FALSE;
    _PSC_RBAR_ELEM RbarElem;
    if(!Get_PscRbarElem(FALSE, ElemK, RbarElem)) bCalcOK=FALSE;
    _PSC_TNDN_ELEM TndnElem;
    if(!Get_PscTndnElem(FALSE, ElemK, TndnElem)) bCalcOK=FALSE;

    int i=0, j=0;
    BOOL bPreTndn[2]={TRUE,TRUE};
    for(i=0 ; i<2 ; i++)  // 0=I, 1=J.
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        // Section
        T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;      
        // RBar
        CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
        arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
        // Tendon
        CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
        arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);

        // Pre, Post 가 같이 있을 경우 Pre, Post 결정은 넓이로 한다. 
        double dApPre =0.0;
        double dApPost=0.0;
        for(j=0; j<arTndnPosi.GetSize(); j++)
        {
            _DGN_TNDN_CRC Tndn = arTndnPosi.GetAt(j);
            if(Tndn.iLoadType==0)      dApPre  += Tndn.dArea;
            else if(Tndn.iLoadType==1) dApPost += Tndn.dArea;
        }

        if(dApPre<dApPost) bPreTndn[i] = FALSE;
        double dfi[3] = {0., 0., 0.};
        double dfe[3] = {0., 0., 0.};
        if(!Get_AllowbleStressOfConc(bPreTndn[i], MatlElem.MatdD, SpscSect, arRbarPosi, arTndnPosi, dfi, dfe)) continue;
        dAlw[i*3+0] = bStage ? dfi[0] : dfe[0];
        dAlw[i*3+1] = bStage ? dfi[1] : dfe[1];
        dAlw[i*3+2] = bStage ? dfi[2] : dfe[2];
    }

    return TRUE;
}

BOOL CDgnKSCE_USD03::Get_AllowbleStressOfConc(BOOL bPre, T_MATD_D& MatdD, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbar, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndn, double* dfi, double* dfe)
{
    // dfi : 0=fti(Top), 1=fti(Bottom), 2=fci
    // dfe : 0=fte(Top), 1=fte(Bottom), 2=fce

    //Material
    double dfck  = MatdD.Data1.Design.C_fc;
    double df_ci = MatdD.Data1.Design.C_fci;

    //Compression Stress
    if(bPre) dfi[2] = -0.6*df_ci;
    else     dfi[2] = -0.55*df_ci;

    dfe[2] = -0.4*dfck;

    // 미리 압축력을 가한 인장구역
    BOOL bExistTndn;  
    BOOL bBond;
    if(arTndn.GetSize()==0) bExistTndn = FALSE;
    else                    bExistTndn = TRUE;

    int iBondTypeCount=0;
    for(int i=0; i<arTndn.GetSize(); i++)
    {
        _DGN_TNDN_CRC TndnInf = arTndn.GetAt(i);
        if(TndnInf.iBondType == 0) iBondTypeCount++;
    }

    if(iBondTypeCount>arTndn.GetSize()/2.) bBond = TRUE;
    else                                   bBond = FALSE;

    double dAst, dds, dAsc, ddc;
    if(!Get_ReBarInfor(TRUE, 0.0, SpscSect, arRbar, dAst, dds, dAsc, ddc)) return FALSE;

    if(bExistTndn)
    {
        dfi[0] = dfe[0] = ( dAsc>0.0 || bBond ? 1.6*sqrt(dfck) : 0.0);
        dfi[1] = dfe[1] = ( dAst>0.0 || bBond ? 1.6*sqrt(dfck) : 0.0);
    }
    else
    {
        dfi[0] = dfe[0] = ( dAsc>0.0 || bBond ? 1.6*sqrt(df_ci) : min(14, 0.8*sqrt(df_ci)));
        dfi[1] = dfe[1] = ( dAst>0.0 || bBond ? 1.6*sqrt(df_ci) : min(14, 0.8*sqrt(df_ci)));  
    }

    return TRUE;
}

BOOL CDgnKSCE_USD03::Get_AllowbleStressofPC(T_TDNA_K TdnaK, T_FYPC_KSCE& FypcKsce)
{ 
    int iLoadType=0.0, iRelxType=0.0;
    double dfpu=0.0, dfpy=0.0;
    double dTotalTendonLength=0.0;
    int iBoundaryNum=0;
    if(!m_pDataCtrl->Get_ConTndn(TdnaK,iLoadType,iRelxType,dfpu,dfpy, dTotalTendonLength, iBoundaryNum))  return FALSE;

    double dAFDL1=0.0, dAFDL2=0.0, dAFLL=0.0;
    // KSCE-USD03 
    // Transfer Stage.
    if(iLoadType==0)  // 프리텐션
    {
        if(iRelxType==0) dAFDL1 = 0.75*dfpu;
        else             dAFDL1 = 0.70*dfpu;
    }
    else if(iLoadType==1) // 포스트텐션
    {
        dAFDL1 = 0.70*dfpu;
        dAFDL2 = 0.83*dfpy;
    }
    else  ASSERT(0);
    // Service Load Stage.
    dAFLL = 0.8*dfpy;

    FypcKsce.dAFDL1 = dAFDL1;
    FypcKsce.dAFDL2 = dAFDL2;
    FypcKsce.dAFLL  = dAFLL;
    return (iLoadType==1 ? dAFDL1*dAFDL2*dAFLL > 0.0 : dAFDL1*dAFLL > 0.0);
}

BOOL CDgnKSCE_USD03::Get_AllowblePrincipalStressOfConc(T_MATD_D& MatdD, double& dAFS, double& dAFST)
{  
    double dfck  = MatdD.Data1.Design.C_fc;
    dAFS = 1.1*sqrt(dfck); // 도.설 2003, 624page, 
    dAFST= 1.1*sqrt(dfck);

    return TRUE;
}

BOOL CDgnKSCE_USD03::Get_CrackCheck(int iCrackType, T_MATD_D& MatdD, T_SPSC_SECT& SpscSect, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbar,
                                    CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, double dMu, double dftop, double dfbot, double dMu_tp, double& dfs, double& dw, double& dwa)
{
    if(arRbar.GetSize()==0) return FALSE;
    //Material
    //double dEc =  MatdD.Data1.Analysis.Elast;
    double dfck= MatdD.Data1.Design.C_fc;
    double dEc = MatdD.Data1.Analysis.Elast;
    double dfca = 0.4*dfck;  //Concrete 허용응력
    double dfy =  MatdD.MainRebarData.B_fy;
    double dEs =  MatdD.MainRebarData.B_Elast;
    double dfsa= min(dfy/2.0,1800.0);     //Rebar 허용응력, Ref. 도.교(2003) 74page

    //Section
    double dyDim = SpscSect.dyDim;
    double dzDim = SpscSect.dzDim;
    double dyCen = SpscSect.dyCen;
    double dzCen = SpscSect.dzCen;
    double dCpy  = dyDim - dyCen;
    double dCpz  = dzDim - dzCen;
    double dArea = SpscSect.dArea;

    //Required Data
    double dh = SpscSect.dzDim;
    double dyst = 0.0, dyten = 0.0;
    double dR = 0.0;
    double dAsY = 0.0, dAstol= 0.0;
    double dd = 0.0, dmax=0.0, dcmax=0.0;

    double dpos = 0.0;
    double dLen = 0.0;
    double dEffA=0.0 , dQ=0.0;
    int    iNumberofRbar=0;
    double dEffAofRbar=0.0;

    int i=0, j=0;
    if((dftop>=0.0 && dfbot<=0.0) || (dftop<=0.0 && dfbot>=0.0))
    {
        if(dftop>0.0)
        {
            dyten = dh - dzCen;
            for(i=0 ; i<arRbar.GetSize() ; i++)
            {
                _DGN_RBAR_CRC rBar = arRbar.GetAt(i);
                if(rBar.dz>0.0)
                {
                    dAstol += rBar.dArea;
                    dAsY += rBar.dArea*fabs(rBar.dz);
                    iNumberofRbar++;
                    if(dmax<fabs(rBar.dz)+0.5*rBar.dDia) 
                    {
                        dmax = fabs(rBar.dz)+0.5*rBar.dDia;  //dC를 위한 변수(도심 기준)
                        dcmax = fabs(rBar.dz);                     //ddc를 위한 변수(도심 기준)  
                    }
                }
            }
            if(dAstol<=0.0 || iNumberofRbar==0) return FALSE;
            dyst = (dAstol==0.0 ? 0.0 : dAsY/dAstol);
            dd = dzCen + dyst;                       
            //Calculation A.
            dpos = dh-2.0*(dh-dd);
            dLen = SpscSect.dyDim;
            if(!CalcSectPSC_QA(arOuter, arInner, TRUE, TRUE, dpos, dLen, dEffA, dQ)) return FALSE;  //BOOL 방향, 축
            dEffAofRbar = dEffA/iNumberofRbar;
        }
        else if(dfbot>0.0)
        {
            dyten = dzCen;
            for(i=0 ; i<arRbar.GetSize() ; i++)
            {
                _DGN_RBAR_CRC rBar = arRbar.GetAt(i);
                if(rBar.dz<0.0)
                {
                    dAstol += rBar.dArea;
                    dAsY += rBar.dArea*fabs(rBar.dz);
                    iNumberofRbar++;
                    if(dmax<fabs(rBar.dz)+0.5*rBar.dDia) 
                    {
                        dmax = fabs(rBar.dz)+0.5*rBar.dDia;
                        dcmax = fabs(rBar.dz);
                    }
                }
            }
            if(dAstol<=0.0 || iNumberofRbar==0.0) return FALSE;
            dyst = (dAstol==0.0 ? 0.0 : dAsY/dAstol);
            dd = (dh-dzCen) + dyst;
            dpos = 2.0*(dh-dd);
            dLen = dyDim;
            if(!CalcSectPSC_QA(arOuter, arInner, FALSE, TRUE, dpos, dLen, dEffA, dQ)) return FALSE;
            dEffAofRbar = dEffA/iNumberofRbar;
        }
        dR = ( dyst==0.0 ? 0.0 : dyten/dyst );
    }
    else if(dftop<0.0 && dfbot<0.0) return FALSE;
    else if(dftop>0.0 && dfbot>0.0) return FALSE;
    //dc
    double ddc = dyten - dcmax;

    //Calculation fs.
    //Calculation Neutral
    double dgap=0.0;
    double dAptol=0.0, dNtodp=0.0, ddp=0.0;
    double dAsttol=0.0, dNtodst=0.0, ddst=0.0;
    double dAsctol=0.0, dNtodsc=0.0, ddsc=0.0;

    double dfst=0.0, dfsc=0.0;
    double dfc=0.0, dEpsi_c=0.0;
    double dfcx=0.0;

    double daArea=0.0, daQ=0.0, daCent=0.0;
    double da=0.0, dCc=0.0, dTp=0.0, dTst=0.0, dCsc=0.0, dCcDel=0.0;
    double dfps=0.0, dMps=0.0;
    double dMn=0.0, dphiMn=0.0;
    double dEpsi_cp=0.0, dEpsi_st=0.0, dEpsi_sc=0.0;
    double dEpsi_ce=0.0, dEpsi_pe=0.0, dEpsi_ps=0.0;

    BOOL bYaxis = TRUE;
    BOOL bDirec = (dMu>=0.0 ? TRUE : FALSE);
    double dns = (dEc==0.0 ? 0.0 : dEs/dEc);      
    double dIner = SpscSect.dIyy;
    double dLength = dyDim;

    double dCp = (bYaxis==TRUE ? dCpz : dCpy);
    double ddist = (bYaxis==TRUE ? dzDim : dyDim);
    UINT iCount = 1;

    //Assume dkd.
    double dk=0.0;
    double dkd = dh*(2./3.);

    CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnInfo;
    do
    {
        dk = dkd/dd;
        dfc = ((1.0-dk)==0.0 ? 0.0 : dfsa*dk/(1.0-dk)/dns);
        if(dfc>dfca) dfc = dfca;
        dEpsi_c = (dEc==0.0 ? 0.0 : dfc/dEc);

        double dkdTemp = (dMu>=0.0 ? dh-dkd : dkd);
        if(!CalcSectPSC_QA(arOuter, arInner, bDirec, bYaxis, dkdTemp, dLength, daArea, daQ)) ASSERT(0);
        daCent = (daArea==0.0 ? 0.0 : daQ/daArea);
        dfcx = (dkd==0.0 ? 0.0 : daCent/dkd*dfc);
        dCc = dfcx*daArea; 

        //dkd을 기준으로 Tendon dy,dz를 수정.
        arTndnInfo.RemoveAll();
        for(i=0 ; i<arTndnPosi.GetSize() ; i++)
        {
            _DGN_TNDN_CRC rTndn = arTndnPosi.GetAt(i);
            _DGN_TNDN_CRC rTnfrC; rTnfrC.Initialize();
            rTnfrC = rTndn;
            double dyposi = (bYaxis==TRUE ? rTndn.dz : rTndn.dy);
            dgap = (dMu>=0.0 ? dkd-dCp : dCp-dkd); //
            if(bYaxis) rTnfrC.dz = rTndn.dz+dgap;
            else       rTnfrC.dy = rTndn.dy+dgap;
            arTndnInfo.Add(rTnfrC);
        }

        //dkd을 기준으로 Rebar(Ac,d) 계산 
        double dkdtemp = (dMu>=0.0 ? dkd : dh-dkd);
        _DGN_TNDN_RBAR rTdRb;
        rTdRb.Initialize();      
        BOOL bGetOk = Get_TndnReBarInfor(bYaxis, dkdtemp, SpscSect, arRbar, arTndnPosi, rTdRb);
        /*
        dAptol =(dMu>=0.0 ? rTdRb.dApttol : rTdRb.dApctol); 
        ddp    =(dMu>=0.0 ? rTdRb.ddpt    : dh-rTdRb.ddpc);
        dAsttol=(dMu>=0.0 ? rTdRb.dAsttol : rTdRb.dAsctol); 
        ddst   =(dMu>=0.0 ? rTdRb.ddst    : dh-rTdRb.ddsc);
        dAsctol=(dMu>=0.0 ? rTdRb.dAsctol : rTdRb.dAsttol); 
        ddsc   =(dMu>=0.0 ? rTdRb.ddsc    : dh-rTdRb.ddst);
        */
        if(dMu>=0.0)  // 정모멘트 
        {
            dAptol =rTdRb.dApttol; ddp  = (dAptol ==0.0 ? 0.0 : rTdRb.ddpt);
            dAsttol=rTdRb.dAsttol; ddst = (dAsttol==0.0 ? 0.0 : rTdRb.ddst);
            dAsctol=rTdRb.dAsctol; ddsc = (dAsctol==0.0 ? 0.0 : rTdRb.ddsc);
        }
        else
        {
            dAptol =rTdRb.dApctol; ddp  = (dAptol ==0.0 ? 0.0 : dh - rTdRb.ddpc);
            dAsttol=rTdRb.dAsctol; ddst = (dAsttol==0.0 ? 0.0 : dh - rTdRb.ddsc);
            dAsctol=rTdRb.dAsttol; ddsc = (dAsctol==0.0 ? 0.0 : dh - rTdRb.ddst);
        }

        dfps=0.0; dTp=0.0;
        for(i=0 ; i<arTndnInfo.GetSize() ; i++)
        {
            _DGN_TNDN_CRC rTndnp = arTndnPosi.GetAt(i);
            double dposi_N = (bYaxis==TRUE ? rTndnp.dz : rTndnp.dy); 
            _DGN_TNDN_CRC rTnInf = arTndnInfo.GetAt(i);
            double dposi = (bYaxis==TRUE ? rTnInf.dz : rTnInf.dy);
            if(dMu>=0.0 && dposi<0.0) 
            {
                double ddpi = dkd + fabs(dposi);
                double dPe  = rTnInf.dArea * rTnInf.dStress[0];
                dEpsi_cp = (dkd==0.0 ? 0.0 : dEpsi_c*(ddpi-dkd)/dkd);
                dEpsi_pe = (rTnInf.dEps==0.0 ? 0.0 : rTnInf.dStress[0]/rTnInf.dEps);
                dEpsi_ce = dArea==0.0 || dEc==0.0 || dIner==0.0 ? 0.0 : dPe/(dArea*dEc)*(1.0+pow(fabs(dposi_N),2.0)/(dIner/dArea));
                dEpsi_ps = dEpsi_pe+dEpsi_ce+dEpsi_cp;
                if(rTnInf.iBondType==0)   dfps = Get_StressOfTendon(rTnInf.dfpy, rTnInf.dfpu, rTnInf.dEps, dEpsi_ps);
                else if(rTnInf.iBondType==1) 
                {
                    if(rTnInf.dStress[0]>=0.5*rTnInf.dfpu)     dfps = rTnInf.dStress[0]+1050.0; 
                    else if(rTnInf.dStress[0]<0.5*rTnInf.dfpu) dfps = Get_StressOfTendon(rTnInf.dfpy, rTnInf.dfpu, rTnInf.dEps, dEpsi_ps);          
                }
                dTp += dfps*rTnInf.dArea;
            }
            else if(dMu<0.0 && dposi>0.0)
            {
                double ddpi = dkd + fabs(dposi);
                double dPe  = rTnInf.dArea * rTnInf.dStress[0];
                dEpsi_cp = (dkd==0.0 ? 0.0 : dEpsi_c*(ddpi-dkd)/dkd);
                dEpsi_pe = (rTnInf.dEps==0.0 ? 0.0 : rTnInf.dStress[0]/rTnInf.dEps);
                dEpsi_ce = dArea==0.0 || dEc==0.0 || dIner==0.0 ? 0.0 : dPe/(dArea*dEc)*(1.0+pow(fabs(dposi_N),2.0)/(dIner/dArea));
                dEpsi_ps = dEpsi_pe+dEpsi_ce+dEpsi_cp;
                if(rTnInf.iBondType==0)   dfps = Get_StressOfTendon(rTnInf.dfpy, rTnInf.dfpu, rTnInf.dEps, dEpsi_ps);
                else if(rTnInf.iBondType==1) 
                {
                    if(rTnInf.dStress[0]>=0.5*rTnInf.dfpu)     dfps = rTnInf.dStress[0]+1050.0; 
                    else if(rTnInf.dStress[0]<0.5*rTnInf.dfpu) dfps = Get_StressOfTendon(rTnInf.dfpy, rTnInf.dfpu, rTnInf.dEps, dEpsi_ps);                     
                }
                dTp += dfps*rTnInf.dArea;
            }

        }   
        if(dAsttol>0.0) dfst = (dkd==0.0 ? 0.0 : dns*dfc*(dd-dkd)/dkd);
        else dfst=0.0;
        if(dAsctol>0.0) dfsc = (dkd==0.0 ? 0.0 : dns*dfc*(dkd-ddsc)/dkd);
        else dfsc=0.0;
        dfst = min(dfst, dfsa);
        dfsc = min(dfsc, dfsa);   
        dTst = dAsttol*dfst;
        dCsc = dAsctol*dfsc;

        dCcDel = dTp + dTst - (dCc-dAsctol*dfck) - dCsc;
        if(fabs(dCcDel)<0.01) break;
        if(iCount>m_PSC_iITER) break;
        if(dCcDel<0.0) dkd -= ddist*pow(0.5,iCount);
        else           dkd += ddist*pow(0.5,iCount);
        ++iCount;

    }while(fabs(dCcDel)>0.01);
    dfs = fabs(dMu_tp)/(dAsttol*(dd-(dkd-daCent)));
    if(dfs>dfsa) dfs = dfsa;  

    //휨 균열폭 
    dw = 1.08*dR*dfs*pow((ddc*dEffAofRbar),1./3.)*(1.0E-05);

    double dC = (dyten - dmax);  //mm
    switch(iCrackType)
    {
    case 0: dwa = 0.006*dC; break;
    case 1: dwa = 0.005*dC; break;
    case 2: dwa = 0.004*dC; break;
    case 3: dwa = 0.0035*dC; break;
    }
    if(dw<=0.0) return FALSE;

    return TRUE;
}

BOOL CDgnKSCE_USD03::CalctensLength(double dftop, double dfbot, double dh, double& dx)
{
    //(+) : 인장
    double dStrHratio=0.0;
    if(dftop<0.0 && dfbot<0.0) dx=0.0; 
    else if((dftop>=0.0 && dfbot<=0.0) || (dftop<=0.0 && dfbot>=0.0))
    {
        if(dftop>0.0)
        {
            dStrHratio = (dh==0.0 ? 0.0 : (fabs(dfbot)+dftop)/dh);
            dx = (dStrHratio==0.0 ? 0.0 : dftop/dStrHratio);
        }
        else
        {
            dStrHratio = (dh==0.0 ? 0.0 : (fabs(dftop)+dfbot)/dh);
            dx = (dStrHratio==0.0 ? 0.0 : dfbot/dStrHratio);
        }
    }
    else if(dftop>0.0 && dfbot>0.0) dx = dh;

    return TRUE;
}

BOOL CDgnKSCE_USD03::CalcSectPSC_QA(CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, BOOL bDir, BOOL baxis, double dpos, double dlen, double& dCutArea, double& dQ)
{
    // Outer=CCW, Inn=CW.
    dQ = 0.0;
    dCutArea = 0.0;
    //*************************************
    // Get Polygon by Cutting Line.
    I_PolyMaker* pPolyMaker;
    pPolyMaker = I_PolyMakerFactory::GetInstance()->New_PMaker();
    int i=0, j=0, k=0;    
    // Outer.
    CArray<XGL_3dp,XGL_3dp&> arOutCut;
    for(i=0; i<arOuter.GetSize(); i++)
    {
        T_SPSC_COOR SpscCoor = arOuter[i];
        for(j=0; j<SpscCoor.arSpscPont[0].GetSize(); j++)
        {
            // x,y Coordinate (from Left, Bottom).
            double dx = SpscCoor.arSpscPont[0].GetAt(j);
            double dy = SpscCoor.arSpscPont[1].GetAt(j);
            XGL_3dp OutXGL;
            OutXGL.Set(dx,dy,0.0);
            arOutCut.Add(OutXGL);
        }
    }
    pPolyMaker->SetOutterPoly(arOutCut);

    // Inner.
    for(i=0; i<arInner.GetSize(); i++)
    {
        CArray<XGL_3dp,XGL_3dp&> arInnCut;
        T_SPSC_COOR_AR* apSpscCoor = arInner[i];
        for(j=0; j<apSpscCoor->GetSize(); j++)
        {
            arInnCut.RemoveAll();
            T_SPSC_COOR SpscCoor = apSpscCoor->GetAt(j);
            for(k=0; k<SpscCoor.arSpscPont[0].GetSize(); k++)
            {
                // x,y Coordinate (from Left, Bottom).
                double dx = SpscCoor.arSpscPont[0].GetAt(k);
                double dy = SpscCoor.arSpscPont[1].GetAt(k);
                XGL_3dp InnXGL;
                InnXGL.Set(dx,dy,0.0);
                arInnCut.Add(InnXGL);
            }
            pPolyMaker->AddInnerPoly(arInnCut);
        }
    }

    double dZero=1.0E-07;
    // Make Polygon by Cutting Line.
    // Get Left Part of Cutting Line (Sta->End).
    XGL_3dp StaXGL, EndXGL;
    double dxSta=0.0, dxEnd=0.0, dySta=0.0, dyEnd=0.0;
    if(bDir==1)  // 1: Left->Right , Top->Bot  0: Right->Left, Bot->Top
    {
        dxSta = (baxis==1 ? (-1)*dZero : dpos);  //1: y-axis 0: z-axis
        dxEnd = (baxis==1 ? dlen+dZero : dpos);
        dySta = (baxis==1 ? dpos : dlen+dZero);
        dyEnd = (baxis==1 ? dpos : (-1)*dZero);
    }
    else
    {
        dxSta = (baxis==1 ? dlen+dZero : dpos);  //1: y-axis 0: z-axis
        dxEnd = (baxis==1 ? (-1)*dZero : dpos);
        dySta = (baxis==1 ? dpos : (-1)*dZero);
        dyEnd = (baxis==1 ? dpos : dlen+dZero);
    }

    StaXGL.Set(dxSta,dySta,0.0);
    EndXGL.Set(dxEnd,dyEnd,0.0);
    pPolyMaker->SetCuttingLine(StaXGL,EndXGL);
    if(!pPolyMaker->MakeResult())	ASSERT(0);
    // Result Polygon.
    for(j=0; j<pPolyMaker->GetResultPolyCount(); j++)
    {
        CArray<XGL_3dp,XGL_3dp&> arResPoly;
        pPolyMaker->GetResultPoly(j,arResPoly);
        int iResNum = arResPoly.GetSize();
        //---------------------------------
        // Calculate Center(x,y), Area.
        double* dpx = new double[iResNum];
        double* dpy = new double[iResNum];
        for(k=0; k<iResNum; k++)
        {
            dpx[k] = arResPoly[k].x();
            dpy[k] = arResPoly[k].y();
        }
        double dxCen=0.0, dyCen=0.0, dArea=0.0;
        if(!CMathFunc::mathPolyCentroid(iResNum,dpx,dpy,dxCen,dyCen,dArea))	ASSERT(0);
        delete []dpx;
        delete []dpy;
        //---------------------------------
        dCutArea += dArea;
        if(baxis==1)			dQ += dArea*fabs(dyCen-dpos);
        else if(baxis==0)	dQ += dArea*fabs(dxCen-dpos);
        else	ASSERT(0);
    }
    //------------------------------------------------------------------------------
    // Hole 을 얻어 오는것이 별도 함수로 분리 되면서 추가된 부분 
    for(j=0; j<pPolyMaker->GetResultHolePolyCount(); j++)
    {
        CArray<XGL_3dp,XGL_3dp&> arResPoly;
        pPolyMaker->GetResultHolePoly(j,arResPoly);
        int iResNum = arResPoly.GetSize();
        //---------------------------------
        // Calculate Center(x,y), Area.
        double* dpx = new double[iResNum];
        double* dpy = new double[iResNum];
        for(k=0; k<iResNum; k++)
        {
            dpx[k] = arResPoly[k].x();
            dpy[k] = arResPoly[k].y();
        }
        double dxCen=0.0, dyCen=0.0, dArea=0.0;
        if(!CMathFunc::mathPolyCentroid(iResNum,dpx,dpy,dxCen,dyCen,dArea))	ASSERT(0);
        delete []dpx;
        delete []dpy;
        //---------------------------------
        dCutArea -= dArea;
        if(baxis==1)			dQ -= dArea*fabs(dyCen-dpos);
        else if(baxis==0)	dQ -= dArea*fabs(dxCen-dpos);
        else	ASSERT(0);
    }

    I_PolyMakerFactory::GetInstance()->Del_PMaker(pPolyMaker);


    return TRUE;
}

BOOL CDgnKSCE_USD03::Get_TensionRebar(BOOL bTopCalc,  BOOL bPrint, T_MATD_D MatdD, double dbw, T_SPSC_SECT& SpscgSect, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbar, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi,
                                      double dftop, double dfbot, double& dAreq, double& dAuse, double& dAmin)
{
    //Material
    double dfck= MatdD.Data1.Design.C_fc;
    double dfy = MatdD.MainRebarData.B_fy;
    double dfsa= min(dfy/2.0,1800.0);     //Rebar 허용응력, Ref. 도.교(2003) 74page
    double dEs = MatdD.MainRebarData.B_Elast;

    //Section
    double dyDim = SpscgSect.dyDim;
    double dzDim = SpscgSect.dzDim;
    double dyCen = SpscgSect.dyCen;
    double dzCen = SpscgSect.dzCen;
    double dArea = SpscgSect.dArea;

    //Required Data
    double dWidth = 0.0;
    double dh = SpscgSect.dzDim;
    double dyten=0.0, dAstol=0.0, dAsY=0.0, dyst=0.0;
    double dLen=0.0, dd=0.0, dbd=0.0, dx=0.0, ddistb=0.0;
    double dTfor=0.0, dfts=0.0;
    double dAten=0.0, dYcent=0.0, dfcent=0.0;
    double dQ=0.0;
    double dAsover=0.0;

    double dApt=0.0, dApc=0.0, dAst=0.0, dAsc=0.0;
    double ddp=0.0, ddst=0.0, ddsc=0.0;
    double dFS = 1.15;
    dAuse = 0.0;

    BOOL bYaxis = TRUE;

    int i=0, j=0;
    double dCtemp = (!bTopCalc ? dh-dzCen : dzCen);
    _DGN_TNDN_RBAR rTdRb;
    rTdRb.Initialize();      
    BOOL bCalcOK = Get_TndnReBarInfor(bYaxis, dCtemp, SpscgSect, arRbar, arTndnPosi, rTdRb);
    if(!bTopCalc)  // 정모멘트 
    {
        dApt=rTdRb.dApttol; ddp  = (dApt==0.0 ? 0.0 : rTdRb.ddpt);
        dApc=rTdRb.dApctol;
        dAst=rTdRb.dAsttol; ddst = (dAst==0.0 ? 0.0 : rTdRb.ddst);
        dAsc=rTdRb.dAsctol; ddsc = (dAsc==0.0 ? 0.0 : rTdRb.ddsc);
    }
    else
    {
        dApt=rTdRb.dApctol; ddp  = (dApt==0.0 ? 0.0 : dh - rTdRb.ddpc);
        dApc=rTdRb.dApttol; 
        dAst=rTdRb.dAsctol; ddst = (dAst==0.0 ? 0.0 : dh - rTdRb.ddsc);
        dAsc=rTdRb.dAsttol; ddsc = (dAsc==0.0 ? 0.0 : dh - rTdRb.ddst);
    }

    dAuse = dAst; // Top.Bottom모두 dAst를 사용함.
    if(ddst<=0.0) ddst = 0.8*dh;
    dbd = dbw * ddst;  

    //Print..  
    if(bPrint) m_Print.Print_TbarDesign1();
    //Tension(+) Compression
    bCalcOK = TRUE;

    if(bTopCalc) // Top Calculation.
    {
        if(dftop>0.0) 
        {
            dyten = dh-dzCen;
            dLen = dyDim;
            ddistb = dh - dx;
            if(dfbot<0.0)
            {
                if(!CalctensLength(dftop, dfbot, dh, dx)) return FALSE;  //tension length

                //Calcultion Tension force
                if(!CalcSectPSC_QA(arOuter, arInner, TRUE, TRUE, ddistb, dLen, dAten, dQ)) bCalcOK = FALSE;
                dYcent = (dAten==0.0 ? 0.0 : dQ/dAten);
                dfcent = (dx==0.0 ? 0.0 : dYcent*dftop/dx);
                dTfor = dfcent*dAten;

                dAreq = (dfsa==0.0 ? 0.0: dTfor/dfsa);
                dAsover = 4.0/3.0*dAreq;
                //Print.
                if(bPrint) m_Print.Print_TbarDesign2(dftop, dfbot, dh, dx, dYcent, dfcent, dAten, dTfor, dfsa, dFS, dAreq);
            }
            else if(dfbot>0.0)
            {
                double dft=0.0, dfm=0.0;
                dyten = dh;
                if(dftop>dfbot)
                {       
                    //Calcultion Tension force
                    dft    = dftop - dfbot;
                    dfm    = dzCen*dft/dh;
                    dfcent = dfbot+dfm;
                    dTfor  = dfcent*dArea;  //Ag
                }
                else if(dfbot>dftop)
                {
                    //Calcultion Tension force
                    dYcent = dzCen;
                    dft    = dfbot - dftop;
                    dfm    = (dh-dzCen)*dft/dh;
                    dfcent = dftop+dfm;
                    dTfor  = dfcent*dArea;  //Ag
                }
                else if(dfbot==dftop)
                {
                    dfcent = dftop;
                    dTfor  = dfcent*dArea;  //Ag
                }

                dAreq = (dfsa==0.0 ? 0.0: dTfor/dfsa);
                dAsover = 4.0/3.0*dAreq;
                //Print.
                if(bPrint) m_Print.Print_TbarDesign3(dft, dfm, dftop, dfbot, dh, dx, dYcent, dfcent, dTfor, dfsa, dFS, dAreq, dyten, dArea);
            }    
        }
        else ASSERT(0);
    }
    else
    {
        if(dfbot>0.0)
        {
            dyten = dzCen;
            dLen = dyDim;      
            if(dftop<0.0)
            {
                if(!CalctensLength(dftop, dfbot, dh, dx)) return FALSE;  //tension length

                //Calcultion Tension force
                if(!CalcSectPSC_QA(arOuter, arInner, FALSE, TRUE, dx, dLen, dAten, dQ)) bCalcOK = FALSE;
                dYcent = (dAten==0.0 ? 0.0 : dQ/dAten);
                dfcent = (dx==0.0 ? 0.0 : dYcent*dfbot/dx);
                dTfor = dfcent*dAten;

                dAreq = (dfsa==0.0 ? 0.0: dTfor/dfsa);
                dAsover = 4.0/3.0*dAreq;
                //Print.
                if(bPrint) m_Print.Print_TbarDesign2(dftop, dfbot, dh, dx, dYcent, dfcent, dAten, dTfor, dfsa, dFS, dAreq);
            }
            else if(dftop>0.0)
            {
                double dft=0.0, dfm=0.0;
                dyten = dh;
                if(dftop>dfbot)
                {       
                    //Calcultion Tension force
                    dft    = dftop - dfbot;
                    dfm    = dzCen*dft/dh;
                    dfcent = dfbot+dfm;
                    dTfor  = dfcent*dArea;  //Ag
                }
                else if(dfbot>dftop)
                {
                    //Calcultion Tension force
                    dYcent = dzCen;
                    dft    = dfbot - dftop;
                    dfm    = (dh-dzCen)*dft/dh;
                    dfcent = dftop+dfm;
                    dTfor  = dfcent*dArea;  //Ag
                }
                else if(dfbot==dftop)
                {
                    dfcent = dftop;
                    dTfor  = dfcent*dArea;  //Ag
                }

                dAreq = (dfsa==0.0 ? 0.0: dTfor/dfsa);
                dAsover = 4.0/3.0*dAreq;
                //Print.
                if(bPrint) m_Print.Print_TbarDesign3(dft, dfm, dftop, dfbot, dh, dx, dYcent, dfcent, dTfor, dfsa, dFS, dAreq, dyten, dArea);
            }    
        }
        else ASSERT(0);
    }

    //Calculation As,min.
    double dAmin1 = (dfy==0.0 ? 0.0 : 0.25*sqrt(dfck)/dfy*dbd); // 도로교 2005, 248p
    double dAmin2 = (dfy==0.0 ? 0.0 : 1.4/dfy*dbd); 
    dAmin = max(dAmin1, dAmin2);
    dAuse = dAstol;    

    return bCalcOK;
}

//휨강도(Y-axis)
BOOL CDgnKSCE_USD03::Get_FlexureStrength(BOOL bYaxis, BOOL bPosiI, BOOL bIter, BOOL bPositive, double dPhib, _PSC_MATL_ELEM& MatlElem, T_SPSC_SECT& SpscSect, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, T_SPSC_SECT& SpscgSect,
                                         double drp, double dbw, double dFlgThk[2], double dpMomPos[2][2], CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_BDCY_KSCE_BASE& BdcrKsce, BOOL bApwM)

{
    T_MATD_D MatD = MatlElem.MatdD;
    //Material
    double dfck= MatD.Data1.Design.C_fc;
    double dEc = MatD.Data1.Analysis.Elast;
    double dfy = MatD.MainRebarData.B_fy;
    double dEs = MatD.MainRebarData.B_Elast;  
    //Section
    double dyDim = SpscSect.dyDim;
    double dzDim = SpscSect.dzDim;
    double dyCen = SpscSect.dyCen;
    double dzCen = SpscSect.dzCen;
    double dCpy  = dyDim - dyCen;
    double dCpz  = dzDim - dzCen;
    double dArea = SpscSect.dArea;
    double dIyy  = SpscSect.dIyy;
    double dIzz  = SpscSect.dIzz;

    //Section_g
    double dyDim_g = SpscgSect.dyDim;
    double dzDim_g = SpscgSect.dzDim;
    double dyCen_g = SpscgSect.dyCen;
    double dzCen_g = SpscgSect.dzCen;
    double dCpy_g  = dyDim_g - SpscgSect.dyCen;
    double dCpz_g  = dzDim_g - SpscgSect.dzCen;
    double dAg     = SpscgSect.dArea;
    double dIyy_g  = SpscgSect.dIyy;
    double dIzz_g  = SpscgSect.dIzz;
    double dIner_g=(bYaxis==TRUE ? dIyy_g : dIzz_g);

    //Force
    double dMu = BdcrKsce.dMuy;

    double dApcmax=0.0; //최대강재량
    double dalpha1 = 0.85;
    double dphi = dPhib;
    double dBeta1 = Get_Beta1(dfck);
    double d036beta1 = 0.36*dBeta1;

    double dEpsi_cu = 0.003;
    double dEpsi_cp=0.0, dEpsi_st=0.0, dEpsi_sc=0.0;
    double dEpsi_ce=0.0, dEpsi_pe=0.0, dEpsi_ps=0.0;

    int i=0,j=0;
    double dgap=0.0;
    double dAstol=0.0, dAsY=0.0;
    double dAptol=0.0, dNtodp=0.0, ddp=0.0;
    double dAsttol=0.0, dNtodst=0.0, ddst=0.0;
    double dAsctol=0.0, dNtodsc=0.0, ddsc=0.0;
    double dApt=0.0, dApc=0.0, dAst=0.0, dAsc=0.0;

    double dNeuC=0.0;
    double ddist=0.0;
    double dfst=0.0, dfsc=0.0;
    double dfst_cal=0.0, dfsc_cal=0.0;

    double daArea=0.0, daQ=0.0, daCent=0.0;
    double da=0.0, dCc=0.0, dTp=0.0, dTst=0.0, dCsc=0.0, dCcDel=0.0;
    double dfps=0.0, dMps=0.0;
    double dMn=0.0, dphiMn=0.0, dMn_Cal=0.0, dphiMn_Cal=0.0;  
    double dTpTimesep=0.0, dPeTimesep=0.0, dfpsiApsi=0.0;
    double dTOL = 10; //1kgf
    double dTOL2 = 0.001;
    double dToler=0.0;

    //계산시 사용되는 변수..Iteration & 도로교설계기준 공통
    double dApw=0.0, dApf=0.0;
    double dqp=0.0;
    double dfpetotal=0.0, dPetotal=0.0;  
    double dfpu=0.0;
    double dfpsave=0.0;
    double dRhoifpui=0.0;
    double dRhoifpsi=0.0;  //
    double dRhop_tot=0.0, dRhop = 0.0, dRhos=0.0, dRhoc=0.0;
    double dbdp=0.0, dbds=0.0, dQa=0.0;

    // Code의 공식을 사용할 때.
    BOOL bOneType=TRUE;
    BOOL bBondType=TRUE;
    UINT iTenTndnNum=0;
    UINT iBondNum   =0;
    UINT iUnbondNum =0;
    double dhf=0.0;
    double dfpe_unb_cal = 0.0;
    double dfpe_unb = 0.0;
    double d05fpu = 0.0;
    double dMn_ps=0.0;
    double dMn_beta1 = 0.0;
    double dphiMn_beta1=0.0;

    double dIner=(bYaxis==TRUE ? dIyy : dIzz);
    BOOL bDirec = (bPositive ? TRUE : FALSE);
    double dh = (bYaxis==TRUE ? dzDim : dyDim);
    double dLength = (bYaxis==TRUE ? dyDim : dzDim);
    double dbf_top = dyDim;
    double dbf_bot = fabs(dpMomPos[0][0] - dpMomPos[1][0]); // 두점의 길이가 아님 x좌표 의 차이로함.
    double db  = (bPositive ? dbf_top : dbf_bot);    

    //0:Pei 1:Apsi 2:dpi 3:epi 4:Epsi_ps 5:fpsi 6:Apsi·fpsi 7:Tpi·epi 8:Pei·epi
    CArray<CString,CString> strTndn;
    CArray<double,double> arfps[9]; 
    BOOL bPrint = m_Print.Get_PrintFlag();

    //Assume C.
    dNeuC = (bYaxis==TRUE ? dzDim : dyDim) * 0.5;
    double dCp = 0.0;
    if(bPositive) dCp = (bYaxis==TRUE ? dCpz : dCpy);
    else          dCp = (bYaxis==TRUE ? dzCen : dyCen);
    double dCtemp=0.0;
    int NumberofTendon = arTndnPosi.GetSize();
    UINT iCount = 1;
    // Flange Height.
    dhf = (bPositive ? dFlgThk[0] : dFlgThk[1]);

    // 중립축 결정을 위해서. 
    BOOL bPosiForce=TRUE;
    double dNeuC1 = 0.0;
    double dNeuC2 = 0.0;

    CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnInfo;
    if(bIter)
    {    
        CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> rarRbarPosi;
        CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> rarTndnPosi;
        do
        {
            iTenTndnNum=0;
            iBondNum   =0;
            iUnbondNum =0;
            dfpsiApsi  = 0.0;
            if(bPrint)
            {
                strTndn.RemoveAll();
                for( i=0; i<9; i++) arfps[i].RemoveAll();        

                dTpTimesep = 0.0;
            }

            dPetotal=0.0;
            //beta1에 대한 면적은 Iteration으로 계산하지 않음. ->나중에 Iteration 적용.
            da = dBeta1*dNeuC;
            double daTemp = (bPositive ? dh-da : da);
            if(CalcSectPSC_QA(arOuter, arInner, bDirec, bYaxis, daTemp, dLength, daArea, daQ))
            {
                daCent = (daArea==0.0 ? 0.0 : daQ/daArea);
                dCc    = dalpha1*dfck*daArea; //
            }

            //dNeuC을 기준으로 Rebar(Ac,d) 계산 
            dCtemp = (bPositive ? dNeuC : dh-dNeuC);
            _DGN_TNDN_RBAR rTdRb;
            rTdRb.Initialize();      
            BOOL bGetOk = Get_TndnReBarInfor(bYaxis, dCtemp, SpscSect, arRbarPosi, arTndnPosi, rTdRb);
            dAptol =(bPositive ? rTdRb.dApttol : rTdRb.dApctol); 
            ddp    =(bPositive ? rTdRb.ddpt    : dh-rTdRb.ddpc);
            if(dAptol==0.0) ddp = 0.0;
            dAsttol=(bPositive ? rTdRb.dAsttol : rTdRb.dAsctol); 
            ddst   =(bPositive ? rTdRb.ddst    : dh-rTdRb.ddsc);
            if(dAsttol==0.0) ddst = 0.0;
            dAsctol=(bPositive ? rTdRb.dAsctol : rTdRb.dAsttol); 
            ddsc   =(bPositive ? rTdRb.ddsc    : dh-rTdRb.ddst);
            if(dAsctol==0.0) ddsc = 0.0;

            double dbdp_It = 0.0;
            double dbd_It = 0.0;      
            if(dhf>=da)
            {
                dbd_It = db*ddst;
                dRhos  = (dbd_It==0.0 ? 0.0 : dAsttol/dbd_It);
                dRhoc  = (dbd_It==0.0 ? 0.0 : dAsctol/dbd_It);

                dbdp_It = db*ddp;
                dRhop = (dbdp_It==0.0 ? 0.0 : dAptol/dbdp_It);
            }
            else if(dhf<da)
            {
                dbd_It = dbw*ddst;
                dRhos  = (dbd_It==0.0 ? 0.0 : dAsttol/dbd_It);
                dRhoc  = (dbd_It==0.0 ? 0.0 : dAsctol/dbd_It);

                dbdp_It = dbw*ddp;
                dRhop = (dbdp_It==0.0 ? 0.0 : dAptol/dbdp_It);
            }

            Get_TndnReBarTransform(bYaxis, bPositive, dNeuC, dCp, SpscSect, arRbarPosi, arTndnPosi, rarRbarPosi, rarTndnPosi);

            dfps=0.0; dMps=0.0; dTp=0.0;
            double dfpu_min = 1.7E308; 
            // Add by Jaeoh ('06.06.15).
            double dPe_Sum=0.0, dMe_Sum=0.0, dep_Sum=0.0;
            for(i=0 ; i<NumberofTendon ; i++)
            {
                _DGN_TNDN_CRC rTnInf = rarTndnPosi.GetAt(i);
                double dposi   = (bYaxis==TRUE ? rTnInf.dz : rTnInf.dy);
                if(bPositive && dposi<0.0) 
                {
                    dPe_Sum += rTnInf.dArea*rTnInf.dStress[0];
                    dMe_Sum += rTnInf.dArea*rTnInf.dStress[0]*dposi;
                }
                else if(!bPositive && dposi>0.0)
                {
                    dPe_Sum += rTnInf.dArea*rTnInf.dStress[0];
                    dMe_Sum += rTnInf.dArea*rTnInf.dStress[0]*dposi;
                }
                dep_Sum = dPe_Sum==0.0 ? 0.0 : dMe_Sum/dPe_Sum;
            }

            for(i=0 ; i<NumberofTendon ; i++)
            {
                _DGN_TNDN_CRC rTnInf = rarTndnPosi.GetAt(i);
                double dposi   = (bYaxis==TRUE ? rTnInf.dz : rTnInf.dy);

                if(bPositive && dposi<0.0) 
                {
                    double ddpi = dNeuC + fabs(dposi);
                    double dPe  = rTnInf.dArea * rTnInf.dStress[0];
                    dEpsi_cp = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(ddpi-dNeuC)/dNeuC);
                    dEpsi_pe = (rTnInf.dEps==0.0 ? 0.0 : rTnInf.dStress[0]/rTnInf.dEps);
                    dEpsi_ce = dArea==0.0 || dEc==0.0 || dIner==0.0 ? 0.0 : dPe_Sum/(dArea*dEc)*(1.0+(dep_Sum*dposi)/(dIner/dArea));  /**/
                    dEpsi_ps = dEpsi_pe+dEpsi_ce+dEpsi_cp;
                    if(rTnInf.iBondType==0)   dfps = Get_StressOfTendon(rTnInf.dfpy, rTnInf.dfpu, rTnInf.dEps, dEpsi_ps);
                    else if(rTnInf.iBondType==1) 
                    {
                        if(rTnInf.dStress[0]>=0.5*rTnInf.dfpu)     dfps = rTnInf.dStress[0] + 1050.0; 
                        else if(rTnInf.dStress[0]<0.5*rTnInf.dfpu) dfps = 0.5*rTnInf.dfpu   + 1050.0;          
                    }
                    dfps = min(dfps,rTnInf.dfpu);
                    dPetotal += rTnInf.dArea*rTnInf.dStress[0];
                    dTp += dfps*rTnInf.dArea;
                    dMps += dfps*rTnInf.dArea*fabs(dposi); 
                    dfpu_min = min(rTnInf.dfpu,dfpu_min);          
                    dfpsiApsi  += dfps*rTnInf.dArea;

                    if(bPrint)
                    {            
                        strTndn.Add(rTnInf.strTndnGrup);
                        arfps[0].Add(dPe);                // 0:Pei
                        arfps[1].Add(rTnInf.dArea);       // 1:Apsi
                        arfps[2].Add(ddpi);               // 2:dpi
                        arfps[3].Add(fabs(dposi));        // 3:epi
                        arfps[4].Add(dEpsi_ps);             // 4:Epsi_ps

                        double dApsi_fpsi = dfps*rTnInf.dArea;
                        double dTpi_epi = dApsi_fpsi*fabs(dposi);
                        double dPei_epi = (rTnInf.dArea*rTnInf.dStress[0])*fabs(dposi);
                        arfps[5].Add(dfps);       //5:fpsi
                        arfps[6].Add(dApsi_fpsi); //6:Apsi·fpsi
                        arfps[7].Add(dTpi_epi);   //7:Tpi·epi
                        arfps[8].Add(dPei_epi);   //8:Pei·epi
                        dTpTimesep += dTpi_epi;
                    }
                }
                else if(!bPositive && dposi>0.0)
                {
                    double ddpi = dNeuC + fabs(dposi);
                    double dPe  = rTnInf.dArea * rTnInf.dStress[0];
                    dEpsi_cp = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(ddpi-dNeuC)/dNeuC);
                    dEpsi_pe = (rTnInf.dEps==0.0 ? 0.0 : rTnInf.dStress[0]/rTnInf.dEps);
                    dEpsi_ce = dArea==0.0 || dEc==0.0 || dIner==0.0 ? 0.0 : dPe_Sum/(dArea*dEc)*(1.0+(dep_Sum*dposi)/(dIner/dArea));
                    dEpsi_ps = dEpsi_pe+dEpsi_ce+dEpsi_cp;
                    if(rTnInf.iBondType==0)   dfps = Get_StressOfTendon(rTnInf.dfpy, rTnInf.dfpu, rTnInf.dEps, dEpsi_ps);
                    else if(rTnInf.iBondType==1) 
                    {
                        if(rTnInf.dStress[0]>=0.5*rTnInf.dfpu)     dfps = rTnInf.dStress[0] + 1050.0; 
                        else if(rTnInf.dStress[0]<0.5*rTnInf.dfpu) dfps = 0.5*rTnInf.dfpu   + 1050.0;
                    }
                    dfps = min(dfps,rTnInf.dfpu);
                    dPetotal += rTnInf.dArea*rTnInf.dStress[0];
                    dTp += dfps*rTnInf.dArea;
                    dMps += dfps*rTnInf.dArea*fabs(dposi);
                    dfpu_min = min(rTnInf.dfpu,dfpu_min);
                    dfpsiApsi  += dfps*rTnInf.dArea;

                    if(bPrint)
                    {            
                        strTndn.Add(rTnInf.strTndnGrup);
                        arfps[0].Add(dPe);                // 0:Pei
                        arfps[1].Add(rTnInf.dArea);       // 1:Apsi
                        arfps[2].Add(ddpi);               // 2:dpi
                        arfps[3].Add(fabs(dposi));        // 3:epi
                        arfps[4].Add(dEpsi_ps);           // 4:Epsi_ps            

                        double dApsi_fpsi = dfps*rTnInf.dArea;
                        double dTpi_epi = dApsi_fpsi*fabs(dposi);
                        double dPei_epi = (rTnInf.dArea*rTnInf.dStress[0])*fabs(dposi);
                        arfps[5].Add(dfps);       //5:fpsi
                        arfps[6].Add(dApsi_fpsi); //6:Apsi·fpsi
                        arfps[7].Add(dTpi_epi);   //7:Tpi·epi
                        arfps[8].Add(dPei_epi);   //8:Pei·epi
                        dTpTimesep += dTpi_epi;
                    }
                }               
            }
            dfpu = dfpu_min;

            if(dAsttol>0.0) dEpsi_st = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(ddst-dNeuC)/dNeuC);
            else dEpsi_st=0.0;
            if(dAsctol>0.0) dEpsi_sc = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(dNeuC-ddsc)/dNeuC);
            else dEpsi_sc=0.0;

            dfst_cal = dEpsi_st*dEs;
            dfsc_cal = dEpsi_sc*dEs;        
            dfst = min(dfst_cal, dfy);
            dfsc = min(dfsc_cal, dfy);   

            dTst = dAsttol*dfst;
            dCsc = dAsctol*dfsc;
            //dTp = dAptol*dfps;

            double dComp = dCc;
            dCcDel = dTp + dTst - dCc;
            dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);

            if(dCcDel<0.0) bPosiForce = FALSE;

            if(bPosiForce)
            {
                dNeuC += 0.3*dh;
                dNeuC2 = dNeuC;
                dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);
                if(fabs(dToler)<dTOL2) break;
                if(iCount>m_PSC_iITER) break;
            }
            else
            {
                if(iCount==1) dNeuC2 = dNeuC;
                dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);
                if(fabs(dToler)<dTOL2) break;
                if(iCount>m_PSC_iITER) break;

                if(dCcDel<=0.0) dNeuC2 = dNeuC;
                else            dNeuC1 = dNeuC;
                dNeuC = 0.5*(dNeuC1 + dNeuC2);
            }

            ++iCount;

        }while(fabs(dToler)>dTOL2);

        dfpsave = (dAptol==0.0 ? 0.0 : dfpsiApsi/dAptol);

        //dNeuC에서 dMn..
        dMn = dCc*(dNeuC-(da-daCent)) + dMps + dTst*(ddst-dNeuC); 
        dMn_Cal = dMn;
        dphiMn_Cal = dphi * dMn;

        double dAp = dAptol; // 인장측 긴장재만. 
        dApw = 0.0;

        if(dhf>=da) // Rectangle.
        {
            dqp = (dfck==0.0 ? 0.0: dRhop*dfpsave/dfck);  //dfpsave
        }
        else if(dhf<da)
        {
            dApf = (dfpsave==0.0 ? 0.0 : dalpha1*dfck*(db-dbw)*dhf/dfpsave);
            if(dAsttol>0.0)
            {
                dApw = (dfpsave==0.0 ? 0.0 : dAp + (dAsttol*dfy/dfpsave) - dApf);
            }
            else
            {
                dApw = dAp - dApf;
            }
            dqp = (dbw*ddp ==0.0 || dfck==0.0 ? 0.0 : dApw/(dbw*ddp)*dfpsave/dfck);
        }
        else ASSERT(0);

        if(da<=dhf)  // 사각형 단면.
        {
            dqp = dRhop*dfpsave/dfck;
            dMn_beta1 = (0.36*dBeta1-0.08*pow(dBeta1,2))*dfck*db*pow(ddp,2);

            if(dqp>d036beta1)
            {        
                dMn = min(dMn, dMn_beta1);
            }
        }
        else if(da>dhf)  // 플랜지를 갖는 단면.
        {
            dqp = (dbw*ddp*dfck==0.0 ? 0.0 : dApw*dfpsave/(dbw*ddp*dfck));
            dMn_beta1 = (0.36*dBeta1-0.08*pow(dBeta1,2))*dfck*dbw*pow(ddp,2)
                +dalpha1*dfck*(db-dbw)*dhf*(ddp-0.5*dhf);
            if(dqp>d036beta1)
            {        
                dMn = min(dMn, dMn_beta1);
            }

        }

        dphiMn = dphi*dMn;
        dphiMn_beta1 = dphi * dMn_beta1;    

    }
    else
    {
        //도로교설계기준 KSCE-USD03

        // 등가 응력 사각형 깊이 결정
        iTenTndnNum=0;
        iBondNum   =0;
        iUnbondNum =0;
        double dh  = (bYaxis==TRUE ? dzDim : dyDim);

        dNeuC = 0.5*dh;  // 초기가정 
        double dCp = 0.0;
        if(bPositive) dCp = (bYaxis==TRUE ? dCpz : dCpy);
        else          dCp = (bYaxis==TRUE ? dzCen : dyCen);
        double ddist = (bYaxis==TRUE ? dzDim : dyDim);
        double dCtemp=0.0;

        BOOL bCalcOK;    
        double dTps=0.0, dCps=0.0;   

        double dfpu_bond  =0.0;
        double dfpu_unbond=0.0;
        CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> rarRbarPosi;
        CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> rarTndnPosi;

        double dMn_co=0.0;    
        double dMn_st=0.0;
        double dMn_cf=0.0;

        CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnInfo;
        do
        {
            dTst=0.0, dTps=0.0, dCsc=0.0, dCps=0.0;
            da = dBeta1*dNeuC;
            //중립축을 기준으로 Tendon dy,dz를 수정.
            dCtemp = (bPositive ? dNeuC : dh-dNeuC);
            _DGN_TNDN_RBAR rTdRb;
            rTdRb.Initialize();      
            bCalcOK = Get_TndnReBarInfor(bYaxis, dCtemp, SpscSect, arRbarPosi, arTndnPosi, rTdRb);
            if(bPositive)  // 정모멘트 
            {
                dApt=rTdRb.dApttol; ddp  = (dApt==0.0 ? 0.0 : rTdRb.ddpt);
                dApc=rTdRb.dApctol;
                dAst=rTdRb.dAsttol; ddst = (dAst==0.0 ? 0.0 : rTdRb.ddst);
                dAsc=rTdRb.dAsctol; ddsc = (dAsc==0.0 ? 0.0 : rTdRb.ddsc);
            }
            else
            {
                dApt=rTdRb.dApctol; ddp  = (dApt==0.0 ? 0.0 : dh - rTdRb.ddpc);
                dApc=rTdRb.dApttol; 
                dAst=rTdRb.dAsctol; ddst = (dAst==0.0 ? 0.0 : dh - rTdRb.ddsc);
                dAsc=rTdRb.dAsttol; ddsc = (dAsc==0.0 ? 0.0 : dh - rTdRb.ddst);
            }

            double dbdp_It = 0.0;
            double dbd_It = 0.0;

            if(dhf>=da)
            {
                dbd_It = db*ddst;
                dRhos  = (dbd_It==0.0 ? 0.0 : dAst/dbd_It);
                dRhoc  = (dbd_It==0.0 ? 0.0 : dAsc/dbd_It);

                dbdp_It = db*ddp;
                dRhop = (dbdp_It==0.0 ? 0.0 : dApt/dbdp_It);
            }
            else if(dhf<da)
            {
                dbd_It = dbw*ddst;
                dRhos  = (dbd_It==0.0 ? 0.0 : dAst/dbd_It);
                dRhoc  = (dbd_It==0.0 ? 0.0 : dAsc/dbd_It);

                dbdp_It = dbw*ddp;
                dRhop = (dbdp_It==0.0 ? 0.0 : dApt/dbdp_It);
            }

            bCalcOK = Get_TndnReBarTransform(bYaxis, bPositive, dNeuC, dCp, SpscSect, arRbarPosi, arTndnPosi, rarRbarPosi, rarTndnPosi);

            // Tendon.
            dPetotal    = 0.0;
            dfpu_bond   = 1.0/m_dZero;
            dfpu_unbond = 1.0/m_dZero;
            double dfpu_bond_sum   = 0.0;
            double dfpu_unbond_sum = 0.0;
            double dfpu_sum = 0.0;
            double dfpe_sum_un = 0.0;      
            for(i=0; i<rarTndnPosi.GetSize(); i++)
            {
                _DGN_TNDN_CRC Tndn = rarTndnPosi.GetAt(i);
                double dfpui = Tndn.dfpu;
                if(bPositive)
                {
                    // Ap
                    if(Tndn.dz<0.0)
                    {
                        iTenTndnNum++;
                        double dfpui = Tndn.dfpu;
                        if(Tndn.iBondType==0) 
                        {
                            iBondNum++;
                            dfpu_bond = min(dfpu_bond, dfpui);  // 한요소에 Tendon Material 여러가지 이면 최소.
                        }
                        else if(Tndn.iBondType==1) 
                        {
                            iUnbondNum++;
                            dfpu_unbond = min(dfpu_unbond, dfpui);
                            double dfpei = Tndn.dStress[0];
                            dfpe_sum_un += dfpei; //!/ x방향이 아니라 합력으로?.
                        }
                        dPetotal += Tndn.dArea*Tndn.dStress[0];
                    }
                }
                else if(!bPositive)
                {
                    // Ap
                    if(Tndn.dz>0.0)
                    {
                        iTenTndnNum++;
                        double dfpui = Tndn.dfpu;
                        if(Tndn.iBondType==0) 
                        {
                            iBondNum++;
                            dfpu_bond = min(dfpu_bond, dfpui);
                        }
                        else if(Tndn.iBondType==1) 
                        {
                            iUnbondNum++;
                            dfpu_unbond = min(dfpu_unbond, dfpui);
                            double dfpei = Tndn.dStress[0];
                            //dfpei = max(dfpei, 0.5*dfpui);
                            dfpe_sum_un += dfpei;     //!/ 
                        }

                        dPetotal += Tndn.dArea*Tndn.dStress[0];
                    }
                }
            }

            if(iBondNum>0 && iUnbondNum==0)
            {
                bBondType = TRUE;
                bOneType  = TRUE;
            }
            else if(iBondNum==0 && iUnbondNum>0)
            {
                bBondType = FALSE;
                bOneType  = TRUE;
            }
            else if(iBondNum>0 && iUnbondNum>0)
            {
                bBondType = FALSE;
                bOneType  = FALSE;
            }      

            dTps = 0.0;
            if(bOneType)
            {        
                if(bBondType)
                {
                    dfpu = dfpu_bond; 
                    dfps = (ddp==0.0 ? 0.0 : dfpu*(1.0-drp/dBeta1*(dRhop*dfpu/dfck+ddst/ddp*(dRhos*dfy/dfck-dRhoc*dfy/dfck))));
                }
                else
                {
                    dfpu = dfpu_unbond; 
                    dfpe_unb_cal = (iUnbondNum==0 ? 0.0 : dfpe_sum_un/iUnbondNum);          
                    dfpe_unb = max(dfpe_unb_cal, 0.5*dfpu);
                    dfps = dfpe_unb + 1050.0;
                }

                d05fpu = 0.5*dfpu;
                if(dhf>=da)
                {
                    dTps = dApt*dfps;
                }
                else if(dhf<da)
                {
                    dApf = (dfps==0.0 ? 0.0 : dalpha1*dfck*(db-dbw)*dhf/dfps);
                    if(dAst>0.0) { dApw = (dfps==0.0 ? 0.0 : dApt + (dAst*dfy/dfps) - dApf); }
                    else         { dApw = dApt - dApf; }

                    dTps = dApt*dfps;
                    //dTps = dApw*dfps;
                }
                dfps = min(dfps,dfpu);

            }
            else if(!bOneType)
            {
                strTndn.RemoveAll();
                for( i=0; i<9; i++)
                    arfps[i].RemoveAll();

                double dfps_bond = 0.0;
                double dfps_unbond = 0.0;
                double dfps_unbond_sum=0.0;
                dfpu = dfpu_bond;
                dfps_bond = (ddp==0.0 ? 0.0 : dfpu*(1.0-drp/dBeta1*(dRhop*dfpu/dfck+ddst/ddp*(dRhos*dfy/dfck-dRhoc*dfy/dfck))));
                for(i=0; i<rarTndnPosi.GetSize(); i++)
                {
                    _DGN_TNDN_CRC Tndn = rarTndnPosi.GetAt(i);
                    double dfpui = Tndn.dfpu;
                    double dApsi = Tndn.dArea;
                    double ddpi = 0.0;
                    if(bPositive)
                    {            
                        if(Tndn.dz<0.0)
                        {
                            ddpi = dNeuC + fabs(Tndn.dz);
                            strTndn.Add(Tndn.strTndnGrup);
                            arfps[0].Add(dApsi);          //0:Apsi
                            arfps[1].Add(dfpui);          //1:fpui              
                            if(Tndn.iBondType==0) 
                            {
                                dTps += dApsi*dfps_bond;                
                                arfps[2].Add(dfps_bond);      //2:fps_bond
                                arfps[3].Add(ddpi);         //3:dpi
                                arfps[4].Add(0.0);          //4:Type 0.0=bond
                            }
                            else if(Tndn.iBondType==1) 
                            {
                                double dfpe_unbond = Tndn.dStress[0]; //!/
                                dfpe_unbond = max(dfpe_unbond, 0.5*dfpui);
                                double dfpsi = dfpe_unbond + 1050.0;
                                dfpsi = min(dfpsi,dfpui);
                                dTps += dApsi*dfpsi;
                                arfps[2].Add(dfpsi);      //2:fps_bond
                                arfps[3].Add(ddpi);    //3:dpi
                                arfps[4].Add(1.0);        //4:Type 0 보다 크면 unbond
                            }
                        }
                    }
                    else if(!bPositive)
                    {
                        // Ap
                        if(Tndn.dz>0.0)
                        {
                            ddpi = dNeuC + fabs(Tndn.dz);
                            strTndn.Add(Tndn.strTndnGrup);
                            arfps[0].Add(dApsi);          //0:Apsi
                            arfps[1].Add(dfpui);          //1:fpui
                            if(Tndn.iBondType==0) 
                            {
                                dTps += dApsi*dfps_bond;
                                arfps[2].Add(dfps_bond);    //2:fps_bond
                                arfps[3].Add(ddpi);         //3:dpi
                                arfps[4].Add(0.0);          //4:Type 0.0=bond
                            }
                            else if(Tndn.iBondType==1) 
                            {
                                double dfpe_unbond = Tndn.dStress[0]; //!/
                                dfpe_unbond = max(dfpe_unbond, 0.5*dfpui);
                                double dfpsi = dfpe_unbond + 1050.0;
                                dfpsi = min(dfpsi,dfpui);
                                dfps_unbond_sum += dfpsi;
                                dTps += dApsi*dfpsi;
                                arfps[2].Add(dfpsi);      //2:fps_bond
                                arfps[3].Add(ddpi);       //3:dpi
                                arfps[4].Add(1.0);        //4:Type 0 보다 크면 unbond
                            }
                        }
                    }
                } // for
                dfps_unbond = (iUnbondNum==0.0 ? 0.0 : dfps_unbond_sum/iUnbondNum);
                dfps = (dfps_bond + dfps_unbond) / 2.0; // if(!OneType) fps는 평균으로 한다. 
                dTps = dfps*dApt;
            } // !bOneType

            double dCc=0.0;
            if(!bOneType)
            {
                if(da<=dhf)
                    dCc = dalpha1*dfck*db*da;
                else if(da>dhf)
                {     
                    if(da<=dh)     dCc = dalpha1*dfck*(dbw*da + (db-dbw)*dhf);
                    else if(da>dh) dCc = dalpha1*dfck*(dbw*dh + (db-dbw)*dhf);
                }
            }
            else if(bOneType)
            {
                if(da<=dhf)
                    dCc = dalpha1*dfck*db*da;
                else if(da>dhf)
                {     
                    if(da<=dh)     dCc = dalpha1*dfck*(dbw*da + (db-dbw)*dhf);
                    else if(da>dh) dCc = dalpha1*dfck*(dbw*dh + (db-dbw)*dhf);

                    //if(da<=dh)     dCc = dalpha1*dfck*(dbw*da); // + (db-dbw)*dhf);
                    //else if(da>dh) dCc = dalpha1*dfck*(dbw*dh); // + (db-dbw)*dhf);
                }
            }

            dTst = dAst*dfy;
            double dTens = dTst + dTps; //(da<=dhf ? dTst + dTps : dTps);

            double dComp = dCc;
            double dCcDel = dTens - dComp;
            dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);

            if(dCcDel<0.0) bPosiForce = FALSE;

            if(bPosiForce)
            {
                dNeuC += 0.3*dh;
                dNeuC2 = dNeuC;
                dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);
                if(fabs(dToler)<dTOL2) break;
                if(iCount>m_PSC_iITER) break;
            }
            else
            {
                if(iCount==1) dNeuC2 = dNeuC;
                dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);
                if(fabs(dToler)<dTOL2) break;
                if(iCount>m_PSC_iITER) break;

                if(dCcDel<=0.0) dNeuC2 = dNeuC;
                else            dNeuC1 = dNeuC;
                dNeuC = 0.5*(dNeuC1 + dNeuC2);
            }

            ++iCount;

        }while(fabs(dToler)>dTOL2);

        // 휨강도 계산.
        dApf = (dfps==0.0 ? 0.0 : dalpha1*dfck*(db-dbw)*dhf/dfps);
        if(dAst>0.0)
        {
            dApw = (dfps==0.0 ? 0.0 : dApt + (dAst*dfy/dfps) - dApf);
        }
        else
        {
            dApw = dApt - dApf;
        }    

        // Code로 계산할 경우 Apw<0 이면 상세계산으로 한 번 더 계산한다. 
        // 상세로 계산할 경우 Apw<0 이면 다시 계산하지 않음. 
        if(!bIter && iCount>m_PSC_iITER)
        {
            if(!Get_FlexureStrength(bYaxis, bPosiI, TRUE, bPositive, dPhib, MatlElem, SpscSect, arOuter, arInner, SpscgSect, drp, dbw, dFlgThk, dpMomPos, arRbarPosi, arTndnPosi, BdcrKsce, TRUE)) return FALSE;

            return TRUE;
        }

        if(!bOneType) // bOneType = FALSE 일 경우 사용.
        {    
            for(i=0; i<rarTndnPosi.GetSize(); i++)
            {          
                _DGN_TNDN_CRC Tndn = rarTndnPosi.GetAt(i);
                double dfpui = Tndn.dfpu;
                double dApsi = Tndn.dArea;
                double dpi   = 0.0;
                if(bPositive)
                {
                    // Ap
                    if(Tndn.dz<0.0)
                    {
                        iTenTndnNum++;            
                        if(Tndn.iBondType==0) 
                        {
                            dMn_ps += dApsi*dfps*fabs(Tndn.dz);
                        }
                        else if(Tndn.iBondType==1) 
                        {
                            double dfpe_unbond = Tndn.dStress[0]; //!/
                            dfpe_unbond = max(dfpe_unbond, 0.5*dfpui);
                            double dfpsi = dfpe_unbond + 1050.0;
                            dfpsi = min(dfpsi,dfpui);

                            dMn_ps += dApsi*dfpsi*fabs(Tndn.dz);
                        }

                    }
                }
                else if(!bPositive)
                {
                    // Ap
                    if(Tndn.dz>0.0)
                    {
                        iTenTndnNum++;
                        if(Tndn.iBondType==0) 
                        {
                            dMn_ps += dApsi*dfps*fabs(Tndn.dz);
                        }
                        else if(Tndn.iBondType==1) 
                        {
                            double dfpe_unbond = Tndn.dStress[0]; //!/
                            dfpe_unbond = max(dfpe_unbond, 0.5*dfpui);
                            double dfpsi = dfpe_unbond + 1050.0;
                            dfpsi = min(dfpsi,dfpui);

                            dMn_ps += dApsi*dfpsi*fabs(Tndn.dz);
                        }

                    }
                }
            }
        }

        if(da<=dhf)  // 사각형 단면.
        {
            dqp = dRhop*dfps/dfck;
            dMn_beta1 = (0.36*dBeta1-0.08*pow(dBeta1,2))*dfck*db*pow(ddp,2);

            if(bOneType)
            {
                dMn_ps = (dApt==0.0 ? 0.0 : dApt*dfps*ddp*(1.0-0.59*(dRhop*dfps/dfck + ddst/ddp*dRhos*dfy/dfck)));
                dMn_st = (dAst==0.0 ? 0.0 : dAst*dfy*ddst*(1.0-0.59*(ddp/ddst*dRhop*dfps/dfck + dRhos*dfy/dfck)));      
                dMn = dMn_ps + dMn_st;
            }
            else if(!bOneType)
            {
                // 중립축을 기준으로 
                dMn_co = dalpha1*dfck*da*db*(dNeuC-da/2.);
                dMn_st = dAst*dfy*(ddst-dNeuC);
                dMn = dMn_co + dMn_ps + dMn_st;  
            }

            // 철근만 있을 경우.
            if(dApt==0 && dAst>0.0)
            {
                dMn = dalpha1*dfck*da*db*(ddst-da/2.);
            }

        }
        else if(da>dhf)  // 플랜지를 갖는 단면.
        {
            dqp = (dbw*ddp*dfck==0.0 ? 0.0 : dApw*dfps/(dbw*ddp*dfck));
            dMn_beta1 = (0.36*dBeta1-0.08*pow(dBeta1,2))*dfck*dbw*pow(ddp,2)
                +0.85*dfck*(db-dbw)*dhf*(ddp-0.5*dhf);
            if(bOneType)
            {
                dMn_ps = (ddp==0.0 ? 0.0 : dApw*dfps*ddp*(1.0-0.59*(dApw/(dbw*ddp)*dfps/dfck)));
                dMn_st = dAst*dfy*(ddst-ddp);
                dMn_cf = dalpha1*dfck*(db-dbw)*dhf*(ddp-0.5*dhf);
                dMn = dMn_ps + dMn_st + dMn_cf;
            }
            else if(!bOneType)
            {
                dMn_co = dalpha1*dfck*da*dbw*(dNeuC-da/2.) + dalpha1*dfck*(db-dbw)*dhf*(dNeuC-dhf/2.);
                dMn_st = dAst*dfy*(ddst-dNeuC);
                dMn = dMn_co + dMn_ps + dMn_st;
            }      

            // 철근만 있을 경우.
            if(dApt==0 && dAst>0.0)
            {
                dMn = dalpha1*dfck*da*dbw*(ddst-da/2.) + dalpha1*dfck*(db-dbw)*dhf*(ddst-dhf/2.);        
            }

        }

        dMn_Cal = dMn;
        dphiMn_Cal = dphi * dMn;

        if(dqp>d036beta1)
        {
            dMn = min(dMn, dMn_beta1);
        }

        dphiMn_beta1 = dalpha1 * dMn_beta1;
        dphiMn  = dalpha1 * dMn;
        dAptol  = dApt;
        dAsttol = dAst;
        dAsctol = dAsc;
        dfpsave = dfps;

    }  //도로교설계기준 KSCE-USD05

    double dSc=0.0;
    if(bYaxis) 
    {
        if(bPositive) dSc = (dzCen_g==0.0 ? 0.0 : dIyy_g/dzCen_g);
        else          dSc = (dzDim_g-dzCen_g==0.0 ? 0.0 : dIyy_g/(dzDim_g-dzCen_g));    
    }
    else
    {
        if(bPositive) dSc = (dyCen_g==0.0 ? 0.0 : dIzz_g/dyCen_g);
        else          dSc = (dyDim_g-dyCen_g==0.0 ? 0.0 : dIzz_g/(dyDim_g-dyCen_g));    
    }
    double dCp_g = 0.0;
    if(bPositive) dCp_g = (bYaxis==TRUE ? dCpz_g : dCpy_g);
    else          dCp_g = (bYaxis==TRUE ? dzCen_g : dyCen_g);
    double dyt_g = dh-dCp_g;

    double dyt = dh-dCp;

    CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnConCent;
    arTndnConCent.RemoveAll();
    for(i=0 ; i<NumberofTendon ; i++)
    {
        _DGN_TNDN_CRC rTndn = arTndnPosi.GetAt(i);
        _DGN_TNDN_CRC rTnfrC; rTnfrC.Initialize();
        rTnfrC = rTndn;
        double dyposi = (bYaxis==TRUE ? rTndn.dz : rTndn.dy);
        dgap =  (bPositive ? dCp_g-dCp : dCp-dCp_g); 
        if(bYaxis) rTnfrC.dz = rTndn.dz+dgap;
        else       rTnfrC.dy = rTndn.dy+dgap;
        arTndnConCent.Add(rTnfrC);
    }

    // Mcr, fpe Calculation.
    double dAps=0.0, dPeep=0.0, dPe_t=0.0, dVp=0.0, dfpe_ave=0.0;
    Get_fpeData(bYaxis, dMu, dAg, dIner_g, dyt_g, arTndnConCent, dAps, dPeep, dPe_t, dVp, dfpe_ave);

    double dRatM = (dphiMn==0.0 ? 0.0 : fabs(dMu)/dphiMn);
    double dfru=2.0*sqrt(dfck);
    double dMcr=(dfru+dfpe_ave)*dSc;
    double d12Mcr=1.2*dMcr;

    if(ddp==0.0 && ddst==0.0)
    {
        bPrint = FALSE;
        BdcrKsce.bCHK = FALSE; // 텐던 & 철근이 없으면 FALSE
    }
    if(bPrint)
    {
        m_Print.m_BdcyParam.Initialize();

        m_Print.m_BdcyParam.iCount = iCount;
        m_Print.m_BdcyParam.dfck = dfck;
        m_Print.m_BdcyParam.dbw  = dbw;
        m_Print.m_BdcyParam.db   = db;
        m_Print.m_BdcyParam.dAps = dAptol;
        m_Print.m_BdcyParam.dfy  = dfy;
        m_Print.m_BdcyParam.ddp  = ddp;
        m_Print.m_BdcyParam.dAs = dAsttol;
        m_Print.m_BdcyParam.dfpu = dfpu;
        m_Print.m_BdcyParam.dd   = ddst;
        m_Print.m_BdcyParam.dAc  = dAg;
        m_Print.m_BdcyParam.dIy  = dIner_g;
        m_Print.m_BdcyParam.dyb  = dyt_g;
        m_Print.m_BdcyParam.dPe  = dPetotal;
        m_Print.m_BdcyParam.dbeta1 = dBeta1;
        m_Print.m_BdcyParam.dep  = ddp-dCp_g;
        m_Print.m_BdcyParam.dMu  = dMu;
        m_Print.m_BdcyParam.drp  = drp;
        m_Print.m_BdcyParam.dhf  = dhf;

        if(bIter)
        {
            m_Print.Print_BdcyDesign0(bApwM);
            //m_Print.Print_BdcyDesign1(dfck, dyDim_g, dAptol, dfy, ddp, dAsttol); //ddp 전체PS긴장재가 아닌 인장측 PS강재만 적용.?
            //m_Print.Print_BdcyDesign2(dfpu, ddp, dAg , dIner_g, dyt_g, dPetotal);
            //m_Print.Print_BdcyDesign3(dBeta1, ddp-dCp_g, dMu , drp, dhf); //drpmax-> drp는 max?...
            m_Print.Print_BdcyDesign4(dNeuC,  dBeta1,  da , dfck,  daArea,  dCc);
            m_Print.Print_BdcyDesign5(strTndn, arfps, dTp, dTpTimesep, dPeTimesep);  //9 = Column
            m_Print.Print_BdcyDesign6(dEpsi_cu, ddst, dNeuC, dEpsi_st, dfst, dfst_cal, dEs, dfy, dAsttol,  dTst);
            //m_Print.Print_BdcyDesign7(dTp, dTst, dCc, dCcDel, dTOL);
            m_Print.Print_BdcyDesign8(dCc, dNeuC, da, daCent, dMps, dTst, ddst, dMn_Cal, dphiMn_Cal, dMu);
            m_Print.Print_BdcyDesign_Code5(bIter, dphi, dfpu, drp, dBeta1, dRhop, dfck, da, dhf, db, dbw, ddst, ddp, dqp, dRhos, 
                dApw, dAptol, dApf, dAsttol, dfy, dfpsave, dphiMn, dphiMn_beta1, d036beta1, dphiMn_Cal);    
            //if(dhf>=da) { m_Print.Print_BdcyDesign9(da, dhf, dRhop, dfpsave, dfck, dqp, d036beta1); }
            //else        { m_Print.Print_BdcyDesign91(da, dhf, dApw, dAptol, dApf, dAsttol, db, dbw, ddp, dfck, dfpsave, dfy, dqp, d036beta1); } 
            m_Print.Print_BdcyDesign10(bIter, dPe_t, dAg, dPeep, dyt_g, dIner_g, dfpe_ave, dfck, dphiMn, d12Mcr, dMu);
        }
        else
        {
            m_Print.Print_BdcyDesign0(bApwM);

            m_Print.Print_BdcyDesign_Code1(dNeuC, dBeta1, da, dfck);
            if(bOneType)
            {
                if(bBondType) m_Print.Print_BdcyDesign_Code2(dfpu, drp, dBeta1, dRhop, dfck, ddst, ddp, dRhos, dRhoc, dfy, dfps);
                else          m_Print.Print_BdcyDesign_Code3(dfpu, dfpe_unb_cal, dfpe_unb, d05fpu, dfps);
            }
            else
            {
                m_Print.Print_BdcyDesign_Code6(strTndn, arfps);
                m_Print.Print_BdcyDesign_Code7(dNeuC, da, db, dbw, dAst, dhf, dfck, ddst, ddp, dfy, dphi, dMn_ps, dphiMn);
            }

            m_Print.Print_BdcyDesign_Code4(da, db, dbw, dApw, dApf, dAptol, dAst, dhf, dfpu, drp, dBeta1, dRhop, 
                dfck, ddst, ddp, dRhos, dfy, dfps, dphi, dMn_Cal, dphiMn_Cal, dMu);
            m_Print.Print_BdcyDesign_Code5(bIter, dphi, dfpu, drp, dBeta1, dRhop, dfck, da, dhf, db, dbw, ddst, ddp, dqp, dRhos, 
                dApw, dAptol, dApf, dAsttol, dfy, dfps, dphiMn, dphiMn_beta1, d036beta1, dphiMn_Cal);
            m_Print.Print_BdcyDesign10(bIter, dPe_t, dAg, dPeep, dyt_g, dIner_g, dfpe_ave, dfck, dphiMn, d12Mcr, dMu);

        }
    }

    /*
    ////////////////////////////
    TRACE(_T("------------------\n"));
    for(i=0 ; i<arTndnPosi.GetSize() ; i++)
    {
    _DGN_TNDN_CRC rTnInf = arTndnPosi.GetAt(i);
    TRACE(_T("A = %lf, dz = %lf\n"), rTnInf.dArea, rTnInf.dz);
    }
    */

    BdcrKsce.dMuy   = dMu;	          //면내 계수모멘트		
    BdcrKsce.dfpe   = dfpe_ave;	    //손실이 일어난 후 강재의 유효프리스트레스	
    BdcrKsce.dAps   = dAptol;	        //인장측 PS강재의 단면적	
    BdcrKsce.dAst   = dAsttol;	      //인장측 철근의 단면적		
    BdcrKsce.dpst   = dRhos;	        //인장측 철근비			
    BdcrKsce.dIy    = dIner_g;	      //강축 단면 2차모멘트 		
    BdcrKsce.db	    = db;		          //단면폭
    BdcrKsce.dh	    = dzDim;		      //단면춤
    BdcrKsce.ddst   = ddst;	          //콘크리트압축 연단에서 인장측 철근 중심까지의 거리		
    BdcrKsce.ddps   = ddp;	          //인장측 PS강재의 유효춤	
    BdcrKsce.dfps   = (bIter ? dfpsave : dfps); //계수하중 하에서 PS강재 평균응력	
    BdcrKsce.da     = da;		          //등가 직사각형 응력블럭의 깊이	
    BdcrKsce.dpMny  = dphiMn;	        //면내 설계휨모멘트	
    BdcrKsce.dRatM  = dRatM;	        //면내 휨강도 안전율	
    BdcrKsce.dqp    = dqp;		        //강재지수			
    BdcrKsce.d036Beta1 = d036beta1;   //최대 PS강재량 제한값	
    BdcrKsce.d12Mcr = d12Mcr;	        //최소 강재 균열모멘트	
    //계산시 사용되는 중간값
    BdcrKsce.dPe    = dPetotal;		    //유효긴장력			
    BdcrKsce.dAsc   = dAsctol;	      //압축측 철근의 단면적	
    BdcrKsce.ddsc   = ddsc;	          //콘크리트압축 연단에서 압축측 철근 중심까지의 거리		
    BdcrKsce.dpsc   = dRhoc;	        //압축측 철근비		
    BdcrKsce.dyt    = dyt_g;		      //총단면적의 중립축에서 인장 연단까지의 거리	
    BdcrKsce.deps   = ddp-dCp_g;	    //PS강재의 도심의 편심량 
    BdcrKsce.dAc    = dAg; 		      //콘크리트의 전면적     
    BdcrKsce.dfy    = dfy;            //인장철근의 설계 항복강도 	
    BdcrKsce.dfck   = dfck;	          //콘크리트 설계기준강도 		
    BdcrKsce.dbeta1 = dBeta1;         //0.85fck의 압축응력이 균등하게 분포하는 직사각형 블록의 깊이 a의 비                
    BdcrKsce.drp    = drp; 	          //PS강재의 종류에 따른 계수	
    BdcrKsce.dphi   = dphi; 	        //강도감도계수		

    return TRUE;  
}

//휨강도(Z-axis)
BOOL CDgnKSCE_USD03::Get_FlexureStrength(BOOL bYaxis, BOOL bPosiI, BOOL bIter, BOOL bPositive, double dPhib, _PSC_MATL_ELEM& MatlElem, T_SPSC_SECT& SpscSect, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, T_SPSC_SECT& SpscgSect,
                                         double dbw, double dFlgThk[2], double dpMomPos[2][2], CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_BDCZ_KSCE_BASE& BdcrKsce)

{
    T_MATD_D MatD = MatlElem.MatdD;
    //Material
    double dfck= MatD.Data1.Design.C_fc;
    double dEc = MatD.Data1.Analysis.Elast;
    double dfy = MatD.MainRebarData.B_fy;
    double dEs = MatD.MainRebarData.B_Elast;  
    //Section
    double dyDim = SpscSect.dyDim;
    double dzDim = SpscSect.dzDim;
    double dyCen = SpscSect.dyCen;
    double dzCen = SpscSect.dzCen;
    double dCpy  = dyDim - dyCen;
    double dCpz  = dzDim - dzCen;
    double dArea = SpscSect.dArea;
    double dIyy  = SpscSect.dIyy;
    double dIzz  = SpscSect.dIzz;

    //Section_g
    double dyDim_g = SpscgSect.dyDim;
    double dzDim_g = SpscgSect.dzDim;
    double dyCen_g = SpscgSect.dyCen;
    double dzCen_g = SpscgSect.dzCen;
    double dCpy_g  = dyDim_g - SpscgSect.dyCen;
    double dCpz_g  = dzDim_g - SpscgSect.dzCen;
    double dAg     = SpscgSect.dArea;
    double dIyy_g  = SpscgSect.dIyy;
    double dIzz_g  = SpscgSect.dIzz;
    double dIner_g=(bYaxis==TRUE ? dIyy_g : dIzz_g);

    //Force
    double dMu = BdcrKsce.dMuy;

    double dApcmax=0.0; //최대강재량
    double dalpha1 = 0.85;
    double dphi = dPhib;
    double dBeta1 = Get_Beta1(dfck);
    double d036beta1 = 0.36*dBeta1;

    double dEpsi_cu = 0.003;
    double dEpsi_cp=0.0, dEpsi_st=0.0, dEpsi_sc=0.0;
    double dEpsi_ce=0.0, dEpsi_pe=0.0, dEpsi_ps=0.0;

    int i=0,j=0;
    double dgap=0.0;
    double dAstol=0.0, dAsY=0.0;
    double dAptol=0.0, dNtodp=0.0, ddp=0.0;
    double dAsttol=0.0, dNtodst=0.0, ddst=0.0;
    double dAsctol=0.0, dNtodsc=0.0, ddsc=0.0;
    double dApt=0.0, dApc=0.0, dAst=0.0, dAsc=0.0;

    double dNeuC=0.0;
    double ddist=0.0;
    double dfst=0.0, dfsc=0.0;

    double daArea=0.0, daQ=0.0, daCent=0.0;
    double da=0.0, dCc=0.0, dTp=0.0, dTst=0.0, dCsc=0.0, dCcDel=0.0;
    double dfps=0.0, dMps=0.0;
    double dMn=0.0, dphiMn=0.0, dMn_Cal=0.0, dphiMn_Cal=0.0;  
    double dTpTimesep=0.0, dPeTimesep=0.0, dfpsiApsi=0.0;
    double dTOL = 10; //1kgf
    double dTOL2 = 0.001;
    double dToler=0.0;

    //계산시 사용되는 변수..Iteration & 도로교설계기준 공통
    double dApw=0.0, dApf=0.0;
    double dqp=0.0;
    double dfpetotal=0.0, dPetotal=0.0;  
    double dfpu=0.0;
    double dfpsave=0.0;
    double dRhoifpui=0.0;
    double dRhoifpsi=0.0;  //
    double dRhop_tot=0.0, dRhop = 0.0, dRhos=0.0, dRhoc=0.0;
    double dbdp=0.0, dbds=0.0, dQa=0.0;

    // Code의 공식을 사용할 때.
    BOOL bOneType=TRUE;
    BOOL bBondType=TRUE;
    UINT iTenTndnNum=0;
    UINT iBondNum   =0;
    UINT iUnbondNum =0;
    double dhf=0.0;
    double dfpe_unb_cal = 0.0;
    double dfpe_unb = 0.0;
    double d05fpu = 0.0;
    double dMn_ps=0.0;
    double dMn_beta1 = 0.0;
    double dphiMn_beta1=0.0;

    double dIner=(bYaxis==TRUE ? dIyy : dIzz);
    BOOL bDirec = (bPositive ? TRUE : FALSE);
    double dh = (bYaxis==TRUE ? dzDim : dyDim);
    double dLength = (bYaxis==TRUE ? dyDim : dzDim);
    double dbf_top = dyDim;
    double dbf_bot = fabs(dpMomPos[0][0] - dpMomPos[1][0]); // 두점의 길이가 아님 x좌표 의 차이로함.
    double db  = (bPositive ? dbf_top : dbf_bot);    

    //0:Pei 1:Apsi 2:dpi 3:epi 4:Epsi_ps 5:fpsi 6:Apsi·fpsi 7:Tpi·epi 8:Pei·epi
    CArray<CString,CString> strTndn;
    CArray<double,double> arfps[9]; 
    // Z-axis 은 Print 하지 않는다. 
    BOOL bPrint = FALSE; //m_Print.Get_PrintFlag();

    //Assume C.
    dNeuC = (bYaxis==TRUE ? dzDim : dyDim) * 0.5;
    double dCp = 0.0;
    if(bPositive) dCp = (bYaxis==TRUE ? dCpz : dCpy);
    else          dCp = (bYaxis==TRUE ? dzCen : dyCen);
    double dCtemp=0.0;
    int NumberofTendon = arTndnPosi.GetSize();
    UINT iCount = 1;
    // Flange Height.
    dhf = (bPositive ? dFlgThk[0] : dFlgThk[1]);

    // 중립축 결정을 위해서. 
    BOOL bPosiForce=TRUE;
    double dNeuC1 = 0.0;
    double dNeuC2 = 0.0;

    CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnInfo;
    if(bIter)
    {    
        CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> rarRbarPosi;
        CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> rarTndnPosi;
        do
        {
            iTenTndnNum=0;
            iBondNum   =0;
            iUnbondNum =0;
            dfpsiApsi  = 0.0;
            if(bPrint)
            {
                strTndn.RemoveAll();
                for( i=0; i<9; i++) arfps[i].RemoveAll();        

                dTpTimesep = 0.0;
            }

            dPetotal=0.0;
            //beta1에 대한 면적은 Iteration으로 계산하지 않음. ->나중에 Iteration 적용.
            da = dBeta1*dNeuC;
            double daTemp = (bPositive ? dh-da : da);
            if(CalcSectPSC_QA(arOuter, arInner, bDirec, bYaxis, daTemp, dLength, daArea, daQ))
            {
                daCent = (daArea==0.0 ? 0.0 : daQ/daArea);
                dCc    = dalpha1*dfck*daArea; //
            }

            //dNeuC을 기준으로 Rebar(Ac,d) 계산 
            dCtemp = (bPositive ? dNeuC : dh-dNeuC);
            _DGN_TNDN_RBAR rTdRb;
            rTdRb.Initialize();      
            BOOL bGetOk = Get_TndnReBarInfor(bYaxis, dCtemp, SpscSect, arRbarPosi, arTndnPosi, rTdRb);
            dAptol =(bPositive ? rTdRb.dApttol : rTdRb.dApctol); 
            ddp    =(bPositive ? rTdRb.ddpt    : dh-rTdRb.ddpc);
            if(dAptol==0.0) ddp = 0.0;
            dAsttol=(bPositive ? rTdRb.dAsttol : rTdRb.dAsctol); 
            ddst   =(bPositive ? rTdRb.ddst    : dh-rTdRb.ddsc);
            if(dAsttol==0.0) ddst = 0.0;
            dAsctol=(bPositive ? rTdRb.dAsctol : rTdRb.dAsttol); 
            ddsc   =(bPositive ? rTdRb.ddsc    : dh-rTdRb.ddst);
            if(dAsctol==0.0) ddsc = 0.0;

            double dbdp_It = 0.0;
            double dbd_It = 0.0;      
            if(dhf>=da)
            {
                dbd_It = db*ddst;
                dRhos  = (dbd_It==0.0 ? 0.0 : dAsttol/dbd_It);
                dRhoc  = (dbd_It==0.0 ? 0.0 : dAsctol/dbd_It);

                dbdp_It = db*ddp;
                dRhop = (dbdp_It==0.0 ? 0.0 : dAptol/dbdp_It);
            }
            else if(dhf<da)
            {
                dbd_It = dbw*ddst;
                dRhos  = (dbd_It==0.0 ? 0.0 : dAsttol/dbd_It);
                dRhoc  = (dbd_It==0.0 ? 0.0 : dAsctol/dbd_It);

                dbdp_It = dbw*ddp;
                dRhop = (dbdp_It==0.0 ? 0.0 : dAptol/dbdp_It);
            }

            Get_TndnReBarTransform(bYaxis, bPositive, dNeuC, dCp, SpscSect, arRbarPosi, arTndnPosi, rarRbarPosi, rarTndnPosi);

            dfps=0.0; dMps=0.0; dTp=0.0;
            double dfpu_min = 1.7E308; 
            // Add by Jaeoh ('06.06.15).
            double dPe_Sum=0.0, dMe_Sum=0.0, dep_Sum=0.0;
            for(i=0 ; i<NumberofTendon ; i++)
            {
                _DGN_TNDN_CRC rTnInf = rarTndnPosi.GetAt(i);
                double dposi   = (bYaxis==TRUE ? rTnInf.dz : rTnInf.dy);
                if(bPositive && dposi<0.0) 
                {
                    dPe_Sum += rTnInf.dArea*rTnInf.dStress[0];
                    dMe_Sum += rTnInf.dArea*rTnInf.dStress[0]*dposi;
                }
                else if(!bPositive && dposi>0.0)
                {
                    dPe_Sum += rTnInf.dArea*rTnInf.dStress[0];
                    dMe_Sum += rTnInf.dArea*rTnInf.dStress[0]*dposi;
                }
                dep_Sum = dPe_Sum==0.0 ? 0.0 : dMe_Sum/dPe_Sum;
            }

            for(i=0 ; i<NumberofTendon ; i++)
            {
                _DGN_TNDN_CRC rTnInf = rarTndnPosi.GetAt(i);
                double dposi   = (bYaxis==TRUE ? rTnInf.dz : rTnInf.dy);

                if(bPositive && dposi<0.0) 
                {
                    double ddpi = dNeuC + fabs(dposi);
                    double dPe  = rTnInf.dArea * rTnInf.dStress[0];
                    dEpsi_cp = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(ddpi-dNeuC)/dNeuC);
                    dEpsi_pe = (rTnInf.dEps==0.0 ? 0.0 : rTnInf.dStress[0]/rTnInf.dEps);
                    dEpsi_ce = dArea==0.0 || dEc==0.0 || dIner==0.0 ? 0.0 : dPe_Sum/(dArea*dEc)*(1.0+(dep_Sum*dposi)/(dIner/dArea));  /**/
                    dEpsi_ps = dEpsi_pe+dEpsi_ce+dEpsi_cp;
                    if(rTnInf.iBondType==0)   dfps = Get_StressOfTendon(rTnInf.dfpy, rTnInf.dfpu, rTnInf.dEps, dEpsi_ps);
                    else if(rTnInf.iBondType==1) 
                    {
                        if(rTnInf.dStress[0]>=0.5*rTnInf.dfpu)     dfps = rTnInf.dStress[0] + 1050.0; 
                        else if(rTnInf.dStress[0]<0.5*rTnInf.dfpu) dfps = 0.5*rTnInf.dfpu   + 1050.0;          
                    }
                    dfps = min(dfps,rTnInf.dfpu);
                    dPetotal += rTnInf.dArea*rTnInf.dStress[0];
                    dTp += dfps*rTnInf.dArea;
                    dMps += dfps*rTnInf.dArea*fabs(dposi); 
                    dfpu_min = min(rTnInf.dfpu,dfpu_min);          
                    dfpsiApsi  += dfps*rTnInf.dArea;

                    if(bPrint)
                    {            
                        strTndn.Add(rTnInf.strTndnGrup);
                        arfps[0].Add(dPe);                // 0:Pei
                        arfps[1].Add(rTnInf.dArea);       // 1:Apsi
                        arfps[2].Add(ddpi);               // 2:dpi
                        arfps[3].Add(fabs(dposi));        // 3:epi
                        arfps[4].Add(dEpsi_ps);             // 4:Epsi_ps

                        double dApsi_fpsi = dfps*rTnInf.dArea;
                        double dTpi_epi = dApsi_fpsi*fabs(dposi);
                        double dPei_epi = (rTnInf.dArea*rTnInf.dStress[0])*fabs(dposi);
                        arfps[5].Add(dfps);       //5:fpsi
                        arfps[6].Add(dApsi_fpsi); //6:Apsi·fpsi
                        arfps[7].Add(dTpi_epi);   //7:Tpi·epi
                        arfps[8].Add(dPei_epi);   //8:Pei·epi
                        dTpTimesep += dTpi_epi;
                    }
                }
                else if(!bPositive && dposi>0.0)
                {
                    double ddpi = dNeuC + fabs(dposi);
                    double dPe  = rTnInf.dArea * rTnInf.dStress[0];
                    dEpsi_cp = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(ddpi-dNeuC)/dNeuC);
                    dEpsi_pe = (rTnInf.dEps==0.0 ? 0.0 : rTnInf.dStress[0]/rTnInf.dEps);
                    dEpsi_ce = dArea==0.0 || dEc==0.0 || dIner==0.0 ? 0.0 : dPe_Sum/(dArea*dEc)*(1.0+(dep_Sum*dposi)/(dIner/dArea));
                    dEpsi_ps = dEpsi_pe+dEpsi_ce+dEpsi_cp;
                    if(rTnInf.iBondType==0)   dfps = Get_StressOfTendon(rTnInf.dfpy, rTnInf.dfpu, rTnInf.dEps, dEpsi_ps);
                    else if(rTnInf.iBondType==1) 
                    {
                        if(rTnInf.dStress[0]>=0.5*rTnInf.dfpu)     dfps = rTnInf.dStress[0] + 1050.0; 
                        else if(rTnInf.dStress[0]<0.5*rTnInf.dfpu) dfps = 0.5*rTnInf.dfpu   + 1050.0;
                    }
                    dfps = min(dfps,rTnInf.dfpu);
                    dPetotal += rTnInf.dArea*rTnInf.dStress[0];
                    dTp += dfps*rTnInf.dArea;
                    dMps += dfps*rTnInf.dArea*fabs(dposi);
                    dfpu_min = min(rTnInf.dfpu,dfpu_min);
                    dfpsiApsi  += dfps*rTnInf.dArea;

                    if(bPrint)
                    {            
                        strTndn.Add(rTnInf.strTndnGrup);
                        arfps[0].Add(dPe);                // 0:Pei
                        arfps[1].Add(rTnInf.dArea);       // 1:Apsi
                        arfps[2].Add(ddpi);               // 2:dpi
                        arfps[3].Add(fabs(dposi));        // 3:epi
                        arfps[4].Add(dEpsi_ps);           // 4:Epsi_ps            

                        double dApsi_fpsi = dfps*rTnInf.dArea;
                        double dTpi_epi = dApsi_fpsi*fabs(dposi);
                        double dPei_epi = (rTnInf.dArea*rTnInf.dStress[0])*fabs(dposi);
                        arfps[5].Add(dfps);       //5:fpsi
                        arfps[6].Add(dApsi_fpsi); //6:Apsi·fpsi
                        arfps[7].Add(dTpi_epi);   //7:Tpi·epi
                        arfps[8].Add(dPei_epi);   //8:Pei·epi
                        dTpTimesep += dTpi_epi;
                    }
                }               
            }
            dfpu = dfpu_min;

            if(dAsttol>0.0) dEpsi_st = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(ddst-dNeuC)/dNeuC);
            else dEpsi_st=0.0;
            if(dAsctol>0.0) dEpsi_sc = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(dNeuC-ddsc)/dNeuC);
            else dEpsi_sc=0.0;

            dfst = min(dEpsi_st*dEs, dfy);
            dfsc = min(dEpsi_sc*dEs, dfy);   

            dTst = dAsttol*dfst;
            dCsc = dAsctol*dfsc;
            //dTp = dAptol*dfps;

            double dComp = dCc;
            dCcDel = dTp + dTst - dCc;
            dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);

            if(dCcDel<0.0) bPosiForce = FALSE;

            if(bPosiForce)
            {
                dNeuC += 0.3*dh;
                dNeuC2 = dNeuC;
                dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);
                if(fabs(dToler)<dTOL2) break;
                if(iCount>m_PSC_iITER) break;
            }
            else
            {
                if(iCount==1) dNeuC2 = dNeuC;
                dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);
                if(fabs(dToler)<dTOL2) break;
                if(iCount>m_PSC_iITER) break;

                if(dCcDel<=0.0) dNeuC2 = dNeuC;
                else            dNeuC1 = dNeuC;
                dNeuC = 0.5*(dNeuC1 + dNeuC2);
            }

            ++iCount;

        }while(fabs(dToler)>dTOL2);

        dfpsave = (dAptol==0.0 ? 0.0 : dfpsiApsi/dAptol);

        //dNeuC에서 dMn..
        dMn = dCc*(dNeuC-(da-daCent)) + dMps + dTst*(ddst-dNeuC); 
        dMn_Cal = dMn;
        dphiMn_Cal = dphi * dMn;

        double dAp = dAptol; // 인장측 긴장재만. 
        dApw = 0.0;

        if(dhf>=da) // Rectangle.
        {
            dqp = (dfck==0.0 ? 0.0: dRhop*dfpsave/dfck);  //dfpsave
        }
        else if(dhf<da)
        {
            dApf = (dfpsave==0.0 ? 0.0 : dalpha1*dfck*(db-dbw)*dhf/dfpsave);
            if(dAsttol>0.0)
            {
                dApw = (dfpsave==0.0 ? 0.0 : dAp + (dAsttol*dfy/dfpsave) - dApf);
            }
            else
            {
                dApw = dAp - dApf;
            }
            dqp = (dbw*ddp ==0.0 || dfck==0.0 ? 0.0 : dApw/(dbw*ddp)*dfpsave/dfck);
        }
        else ASSERT(0);

        if(da<=dhf)  // 사각형 단면.
        {
            dqp = dRhop*dfpsave/dfck;
            dMn_beta1 = (0.36*dBeta1-0.08*pow(dBeta1,2))*dfck*db*pow(ddp,2);

            if(dqp>d036beta1)
            {        
                dMn = min(dMn, dMn_beta1);
            }
        }
        else if(da>dhf)  // 플랜지를 갖는 단면.
        {
            dqp = (dbw*ddp*dfck==0.0 ? 0.0 : dApw*dfpsave/(dbw*ddp*dfck));
            dMn_beta1 = (0.36*dBeta1-0.08*pow(dBeta1,2))*dfck*dbw*pow(ddp,2)
                +dalpha1*dfck*(db-dbw)*dhf*(ddp-0.5*dhf);
            if(dqp>d036beta1)
            {        
                dMn = min(dMn, dMn_beta1);
            }

        }

        dphiMn = dphi*dMn;
        dphiMn_beta1 = dphi * dMn_beta1;    

    }
    else ASSERT(0);


    double dCp_g = 0.0;
    if(bPositive) dCp_g = (bYaxis==TRUE ? dCpz_g : dCpy_g);
    else          dCp_g = (bYaxis==TRUE ? dzCen_g : dyCen_g);
    double dyt_g = dh-dCp_g;

    double dyt = dh-dCp;

    // Mcr, fpe Calculation.
    double dAps=0.0, dPeep=0.0, dPe_t=0.0, dVp=0.0, dfpe_ave=0.0;
    double dRatM = (dphiMn==0.0 ? 0.0 : fabs(dMu)/dphiMn);
    double dfru=2.0*sqrt(dfck);
    double dMcr=0.0;
    double d12Mcr=0.0;

    if(ddp==0.0 && ddst==0.0)
    {
        bPrint = FALSE;
        BdcrKsce.bCHK = FALSE; // 텐던 & 철근이 없으면 FALSE
    }

    BdcrKsce.dMuy   = dMu;	          //면내 계수모멘트		
    BdcrKsce.dpMny  = dphiMn;	        //면내 설계휨모멘트	
    BdcrKsce.dRatM  = dRatM;	        //면내 휨강도 안전율	

    BdcrKsce.dfpe   = 0.0;	    //손실이 일어난 후 강재의 유효프리스트레스	
    BdcrKsce.dAps   = 0.0;	        //인장측 PS강재의 단면적	
    BdcrKsce.dAst   = 0.0;	      //인장측 철근의 단면적		
    BdcrKsce.dpst   = 0.0;	        //인장측 철근비			
    BdcrKsce.dIy    = 0.0;	      //강축 단면 2차모멘트 		
    //  BdcrKsce.db	    = 0.0;		          //단면폭
    //  BdcrKsce.dh	    = 0.0;		      //단면춤
    BdcrKsce.ddst   = 0.0;	          //콘크리트압축 연단에서 인장측 철근 중심까지의 거리		
    BdcrKsce.ddps   = 0.0;	          //인장측 PS강재의 유효춤	
    BdcrKsce.dfps   = 0.0; //계수하중 하에서 PS강재 평균응력	
    BdcrKsce.da     = 0.0;		          //등가 직사각형 응력블럭의 깊이	
    BdcrKsce.dqp    = 0.0;		        //강재지수			
    BdcrKsce.d036Beta1 = 0.0;   //최대 PS강재량 제한값	
    BdcrKsce.d12Mcr = 0.0;	        //최소 강재 균열모멘트	  
    BdcrKsce.dPe    = 0.0;		    //유효긴장력			
    BdcrKsce.dAsc   = 0.0;	      //압축측 철근의 단면적	
    BdcrKsce.ddsc   = 0.0;	          //콘크리트압축 연단에서 압축측 철근 중심까지의 거리		
    BdcrKsce.dpsc   = 0.0;	        //압축측 철근비		
    BdcrKsce.dyt    = 0.0;		      //총단면적의 중립축에서 인장 연단까지의 거리	
    BdcrKsce.deps   = 0.0;	    //PS강재의 도심의 편심량 
    BdcrKsce.dAc    = 0.0; 		      //콘크리트의 전면적     
    BdcrKsce.dfy    = 0.0;            //인장철근의 설계 항복강도 	
    BdcrKsce.dfck   = 0.0;	          //콘크리트 설계기준강도 		
    BdcrKsce.dbeta1 = 0.0;         //0.85fck의 압축응력이 균등하게 분포하는 직사각형 블록의 깊이 a의 비                
    BdcrKsce.drp    = 0.0; 	          //PS강재의 종류에 따른 계수	
    BdcrKsce.dphi   = 0.0; 	        //강도감도계수		

    return TRUE;  
}

double CDgnKSCE_USD03::Get_Beta1(double dfc)
{
    // Factor for stress block width.
    double dBeta=0.0;

    if(dfc <= 280.0)	dBeta = 0.85;
    else							dBeta = max(0.85-0.007*(dfc-280.0)/10.0, 0.65);

    return dBeta;
}

double CDgnKSCE_USD03::Get_StressOfTendon(double df_py, double df_pu, double dEp, double dEpsi_ps)
{
    double dfp=0.0;
    if(df_pu<=(19000.0-m_dZero)) // SWPC 7A
    {
        if(dEpsi_ps<=(0.0076+m_dZero)) dfp = dEpsi_ps*dEp;
        else                           dfp = (17.5-0.0028/(dEpsi_ps-0.0064))*1000.0;       
    }
    else // SWPC 7B
    {
        if(dEpsi_ps<=(0.0086+m_dZero)) dfp = dEpsi_ps*dEp;
        else                           dfp = (19.0-0.0028/(dEpsi_ps-0.007))*1000.0;
    }

    if(dfp>df_pu) dfp = df_pu;
    return dfp; 
}

BOOL CDgnKSCE_USD03::Get_ReBarInfor(BOOL bYaxis, double dGap, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
                                    double& dAs, double& dds, double& dAsc, double& ddc)
{
    int i=0;
    double ddst=0.0, ddsc=0.0;
    double dAttol=0.0, dActol=0.0;
    double dAtY=0.0, dAcY=0.0;
    for(i=0 ; i<arRbarPosi.GetSize(); i++)
    {
        _DGN_RBAR_CRC rRebar = arRbarPosi.GetAt(i);
        if(bYaxis)
        {
            rRebar.dz = rRebar.dz + dGap;
            if(rRebar.dz<0.0)
            {
                dAttol += rRebar.dArea;
                dAtY += rRebar.dArea*rRebar.dz;
            }
            else
            {
                dActol += rRebar.dArea;
                dAcY += rRebar.dArea*rRebar.dz;
            }
        }
        else
        {
            rRebar.dy = rRebar.dy + dGap;
            if(rRebar.dy<0.0)
            {
                dAttol += rRebar.dArea;
                dAtY += rRebar.dArea*rRebar.dz;
            }
            else
            {
                dActol += rRebar.dArea;
                dAcY += rRebar.dArea*rRebar.dz;
            }
        }
    }
    dAs = dAttol;  dds = (dAttol==0.0 ? 0.0 : dAtY/dAttol);
    dAsc = dActol; ddc = (dActol==0.0 ? 0.0 : dAcY/dActol);

    return TRUE;
}									

BOOL CDgnKSCE_USD03::Get_fpeData(BOOL bYaxis, double dMu, double dAg, double dIy, double dyt, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, double& dAps, double& dPeep, double& dPe, double& dVp, double& dfpe)
{
    dAps=dPe=dPeep=dVp=0.0;
    for(int i=0 ; i<arTndnPosi.GetSize() ; i++) // Gross 기준으로.
    {
        _DGN_TNDN_CRC rTnInf = arTndnPosi.GetAt(i);
        double dposi = (bYaxis==TRUE ? rTnInf.dz : rTnInf.dy);
        double dPei = rTnInf.dArea*rTnInf.dStress[0];      
        double dPei_epi = (rTnInf.dArea*rTnInf.dStress[0])*fabs(dposi);
        dAps += rTnInf.dArea;
        dPe  += rTnInf.dStress[0]*rTnInf.dArea; 
        dVp  += rTnInf.dStress[2]*rTnInf.dArea;  //Total Tendon.
        if(dMu>=0.0)  
        {      
            if(dposi<0.0)       
            {        
                dPeep += dPei_epi;        
            }      
            else if(dposi>=0.0) 
            {        
                dPeep -= dPei_epi;
            }      
        }      
        else if(dMu<0.0)
        {      
            if(dposi<0.0) 
            {        
                dPeep -= dPei_epi;
            }       
            else if(dposi>=0.0)
            {        
                dPeep += dPei_epi;        
            }

        }    
    }  

    double dfpe1 = (dAg==0.0 ? 0.0 : dPe / dAg);
    double dfpe2 = (dIy==0.0 ? 0.0 : dPeep / dIy * dyt);
    dfpe = dfpe1 + dfpe2; 

    return TRUE;
}

BOOL CDgnKSCE_USD03::Get_ShearDesign(BOOL bPosiI, double dPhiv, T_MATD_D& MatD, T_SPSC_SECT& SpscSect, T_SPSC_SECT& SpscgSect, double dShrThk, double dfdt, double dfdb, double dVd, double dMd,
                                     CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_RPSC_SBAR& RpscSbar, T_CUMS_KSCE_BASE& CumsKsce)
{
    int iVciType = 2; // 1=자중고려함, 2=자중고려안함.

    //Material
    double dfck= MatD.Data1.Design.C_fc;
    double dEc = MatD.Data1.Analysis.Elast;
    double dfy = MatD.MainRebarData.B_fy;
    double dEs = MatD.MainRebarData.B_Elast;
    double dfys = MatD.SubRebarData.B_fy;
    double dEsv = MatD.SubRebarData.B_Elast;
    double dfyp = dfys; // Assumed by ZINU.('04.08.17).
    //Section
    double dyDim = SpscSect.dyDim;
    double dzDim = SpscSect.dzDim;
    double dyCen = SpscSect.dyCen;
    double dzCen = SpscSect.dzCen;
    double dCpy  = dyDim - dyCen;
    double dCpz  = dzDim - dzCen;
    double dArea = SpscSect.dArea;
    double dIyy  = SpscSect.dIyy;
    double dIzz  = SpscSect.dIzz;
    //Force
    double dMu = CumsKsce.dMu;
    double dVu = CumsKsce.dVu;

    double dphiv = dPhiv;
    double dfpu=0.0, dfpy=0.0;
    double dAps=0.0;
    double dVn=0.0, dVc=0.0, dVs_req=0.0, dVs=0.0, dVs_min=0.0, dVpc=0.0, dVp=0.0;
    double dVci=0.0, dVci_cal=0.0, dVci_min=0.0, dVcw=0.0;
    double dVi=0.0;  //Vd : 부재의 자중만에 의해 생기는 하중계수를 고려하지 않은 전단력
    double dMmax=0.0;
    double dd=0.0, dbwd=0.0, dbwdp=0.0;
    double dbw=dShrThk;
    double dH = dzDim;
    double dyt = (dMu>=0.0 ? dzCen : dCpz); 
    double dI = dIyy;
    double dd_org=0.0;
    double ddp_org=0.0;

    //Gross Section
    BOOL bYaxis = TRUE;
    double dyDim_g = SpscgSect.dyDim;
    double dzDim_g = SpscgSect.dzDim;
    double dyCen_g = SpscgSect.dyCen;
    double dzCen_g = SpscgSect.dzCen;
    double dCpy_g  = dyDim_g - SpscgSect.dyCen;
    double dCpz_g  = dzDim_g - SpscgSect.dzCen;
    double dAg     = SpscgSect.dArea;
    double dIyy_g  = SpscgSect.dIyy;
    double dIzz_g  = SpscgSect.dIzz;
    double dIner_g=(bYaxis==TRUE ? dIyy_g : dIzz_g);

    double dCp=0.0, dCp_g=0.0;
    if(dMu>=0.0) dCp = (bYaxis==TRUE ? dCpz : dCpy);
    else         dCp = (bYaxis==TRUE ? dzCen : dyCen);
    if(dMu>=0.0) dCp_g = (bYaxis==TRUE ? dCpz_g : dCpy_g);
    else         dCp_g = (bYaxis==TRUE ? dzCen_g : dyCen_g);

    double dSc=0.0;
    if(bYaxis) 
    {
        if(dMu>=0.0) dSc = (dzCen_g==0.0 ? 0.0 : dIyy_g/dzCen_g);
        else         dSc = (dzDim_g-dzCen_g==0.0 ? 0.0 : dIyy_g/(dzDim_g-dzCen_g));    
    }
    else
    {
        if(dMu>=0.0) dSc = (dyCen_g==0.0 ? 0.0 : dIzz_g/dyCen_g);
        else         dSc = (dyDim_g-dyCen_g==0.0 ? 0.0 : dIzz_g/(dyDim_g-dyCen_g));    
    }

    double dyt_g = dH-dCp_g;

    CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnConCent;
    arTndnConCent.RemoveAll();
    double dfpu_min = 1.7E308; 
    double dfpy_min = 1.7E308; 
    for(int i=0 ; i<arTndnPosi.GetSize() ; i++)
    {
        _DGN_TNDN_CRC rTndn = arTndnPosi.GetAt(i);
        _DGN_TNDN_CRC rTnfrC; rTnfrC.Initialize();
        rTnfrC = rTndn;
        double dyposi = (bYaxis==TRUE ? rTndn.dz : rTndn.dy);
        double dgap =  (dMu>=0.0 ? dCp_g-dCp : dCp-dCp_g); 
        if(bYaxis) rTnfrC.dz = rTndn.dz+dgap;
        else       rTnfrC.dy = rTndn.dy+dgap;
        arTndnConCent.Add(rTnfrC);
        dfpu_min = min(rTnfrC.dfpu,dfpu_min);   dfpu = dfpu_min;
        dfpy_min = min(rTnfrC.dfpy,dfpy_min);   dfpy = dfpy_min;
    }

    // fpe Calculation.
    double dfpe_ave=0.0, dPe=0.0, dPeep=0.0;  // dPe는 단면의 전체 텐던.
    Get_fpeData(bYaxis, dMu, dAg, dIner_g, dyt_g, arTndnConCent, dAps, dPeep, dPe, dVp, dfpe_ave);
    dVp = fabs(dVp);

    //Get ds  
    double dAptol=0.0,  ddp=0.0;
    double dAsttol=0.0, ddst=0.0;
    double dAsctol=0.0, ddsc=0.0;
    _DGN_TNDN_RBAR rTdRb;
    rTdRb.Initialize();      
    BOOL bGetOk = Get_TndnReBarInfor(TRUE, dCpz, SpscSect, arRbarPosi, arTndnPosi, rTdRb);  
    dAptol =(dMu>=0.0 ? rTdRb.dApttol : rTdRb.dApctol); 
    ddp    =(dMu>=0.0 ? rTdRb.ddpt    : dH-rTdRb.ddpc); ddp = (dAptol==0.0 ? 0.0 : ddp);    
    dAsttol=(dMu>=0.0 ? rTdRb.dAsttol : rTdRb.dAsctol); 
    ddst   =(dMu>=0.0 ? rTdRb.ddst    : dH-rTdRb.ddsc); ddst = (dAsttol==0.0 ? 0.0 : ddst);
    dAsctol=(dMu>=0.0 ? rTdRb.dAsctol : rTdRb.dAsttol); 
    ddsc   =(dMu>=0.0 ? rTdRb.ddsc    : dH-rTdRb.ddst); ddsc = (dAsctol==0.0 ? 0.0 : ddsc);

    dd = dd_org = ddst;  
    ddp_org = ddp;
    if( dd==0.0 || dd<0.8*dH) dd = 0.8*dH;
    dbwd = dbw*dd;
    if( ddp==0.0 || ddp<0.8*dH) ddp = 0.8*dH;
    dbwdp = dbw*ddp;  

    dVi  = dVu - dVd; 
    dMmax= (dMu>=0.0 ? dMu-dMd : -1.0*(dMu-dMd)); 
    double dMcr=0.0, dfpc=0.0;
    double dfd = (dIyy_g==0.0 ? 0.0 : dMd * dyt_g / dIyy_g);  
    double dVci_incl=0.0;
    //+++++++++++++++++++++++++++++++++++++++++
    if(iVciType==1)      
    {
        dMcr = dSc*(1.6*sqrt(dfck)+dfpe_ave-dfd);   //dfd: 외부 작용하중에 의해 인장응력이 발생하는 단면의 최외연에서의 사용 고정하중에 의한 응력  
        dVci_incl = (dMmax==0.0 ? dVd : dVd + dVi*dMcr/dMmax);
        dVci_cal = (dMmax==0.0 ? 0.16*sqrt(dfck)*dbwd + fabs(dVd) : 0.16*sqrt(dfck)*dbwd + fabs(dVd + fabs(dVi*dMcr/dMmax)) );
    }
    else if(iVciType==2) 
    {
        dMcr = dSc*(1.6*sqrt(dfck)+dfpe_ave);
        dVci_incl = (dMu==0.0 ? 0.0 : dVu*dMcr/dMu);
        dVci_cal = (dMmax==0.0 ? 0.16*sqrt(dfck)*dbwd + fabs(dVd) : 0.16*sqrt(dfck)*dbwd + fabs(dVu*dMcr/dMu));
    }

    dVci_min = 0.45*sqrt(dfck)*dbwd;
    dVci = max(dVci_cal, dVci_min);
    dfpc = (dAg==0.0 ? 0.0 : dPe/dAg);  //유효프리스트레스 힘으로 인한 콘크리트 도심에서의 압축응력..
    dVcw = (0.93*sqrt(dfck)+0.3*dfpc)*dbwd + dVp;
    dVc = min(dVci, dVcw);
    double d05pVc = 0.5*dphiv*dVc;      //계수전단력 Vu가 phiVc의 1/2을 초과하는 경우
    double dpVc = dphiv*dVc;
    //Calculation dVs 
    double dAv=0.0, dsv=0.0, dThetaV=0.0;
    double dVsmax=0.0;
    dsv = RpscSbar.dSbarPitch;  
    dAv = (dsv==0.0 ? 0.0 : RpscSbar.dSbarAw);
    if(bPosiI)  dThetaV = RpscSbar.dSbarTheta;  /**/
    else        dThetaV = RpscSbar.dSbarTheta;
    dVsmax=2.12*sqrt(dfck)*dbwd;
    dVs =  (dsv==0.0 ? 0.0 : dAv*dfys*(sin(dThetaV)+cos(dThetaV))*dd/dsv);
    dVs = fabs(dVs);
    dVs_req = (dphiv==0.0 ? 0.0 : (fabs(dVu)-dphiv*dVc)/dphiv);
    dVs_min = min(dVs, dVsmax);
    //Calculation dVpc
    double dAp=0.0, dsp=0.0, dThetaP=0.0;
    dsp = RpscSbar.dTndnDist;
    dAp = (dsp==0.0 ? 0.0 : RpscSbar.dTndnAp);
    if(bPosiI) dThetaP = RpscSbar.dTndnTheta;
    else       dThetaP = RpscSbar.dTndnTheta; 
    dVpc = (dsp==0.0 ? 0.0 : dAp*dfyp*(sin(dThetaP)+cos(dThetaP))*dd/dsp);
    // Change by Jaeoh. ('06.06.15). dVpc(전단강봉)는 기준에 없는 값이므로 고려하지 않는 것으로 수정
    dVn = dVc + dVs_min; // dVc + dVs_min + dVpc -> dVc + dVs_min
    double dpVn = dphiv*dVn;
    double dRatV= (dpVn==0.0 ? 0.0 : dVu/dpVn);
    //Avreq,Avmin
    double dVu_pVc = fabs(dVu)-dphiv*dVc; //-dphiv*dVpc;
    double d08rfckbwd = dphiv*0.8*sqrt(dfck)*dbwd;
    double dAv_req=0.0;
    if(dVu_pVc>0.0) dAv_req = (dphiv*dfys*(sin(dThetaV)+cos(dThetaV))*dd==0.0 ? 0.0 : dVu_pVc*dsv/(dphiv*dfys*(sin(dThetaV)+cos(dThetaV))*dd));
    else                dAv_req = 0.0;
    double dAvmin = (dfys==0.0 ? 0.0 : 3.5*dbw*dsv/dfys);

    BOOL bPrint = m_Print.Get_PrintFlag();
    if(bPrint)
    {
        m_Print.m_CumsParam.dfck = dfck;
        m_Print.m_CumsParam.dbw  = dbw;
        m_Print.m_CumsParam.ddst = dd_org;
        m_Print.m_CumsParam.dfy  = dfy;
        m_Print.m_CumsParam.dh   = dH;
        m_Print.m_CumsParam.dIy  = dIner_g;
        m_Print.m_CumsParam.dfpu = dfpu;
        m_Print.m_CumsParam.ddp  = ddp_org;
        m_Print.m_CumsParam.dAc  = dAg;
        m_Print.m_CumsParam.dfpy = dfpy;
        m_Print.m_CumsParam.dyt  = dyt_g;
        m_Print.m_CumsParam.dAps = dAps;

        m_Print.Print_CumsDesign0();
        //m_Print.Print_CumsDesign1(dfck, dbw, ddst , dfy, dH, dIner_g);
        //m_Print.Print_CumsDesign2(dfpu, ddp, dAg, dfpy, dyt_g, dAps);

        m_Print.Print_CumsDesign3(dMu, dVu, dMd, dVd, dfd, dPe, dfpe_ave);
        m_Print.Print_CumsDesign4(dH, dd, dd_org, dphiv*dH, ddp, ddp_org, dIner_g, dyt_g, dfck, dfpe_ave, dfd, dMcr);
        m_Print.Print_CumsDesign5(dMu, dMd, dMmax, dVu, dVd, dVi, dVci_cal, dVci, dVci_incl, dfck, dbw, dd, dMcr, dVci_min);
        m_Print.Print_CumsDesign6(dPe, dAg, dfpc, dfck, dbw, dd, dVp, dVcw, dVci, dVc);
        //m_Print.Print_CumsDesign7(dVc, dphiv, d05pVc, dpVc, dVu, dbw, dd, dVp, dVcw, dVci);
        m_Print.Print_CumsDesign8(dVu, dVc, dphiv, dpVc, d05pVc, dpVn, dfck, dbw, dd, dVs_req, dVsmax,
            dVs, dVs_min, dsv, dfy, dThetaV, dAv, dAv_req, dAvmin);

    }

    CumsKsce.dVu     =dVu;	    //면내 계수전단력		[Unit=]
    CumsKsce.dVci    =dVci;     //전단과 모멘트의 조합에 의해 사인장균열이 발생할 때 콘크리트가 부담하는 공칭 전단강도 [Unit=Force]
    CumsKsce.dVcw    =dVcw;     //복부의 과도한 주인장응력으로 인해 사인장균열이 발생할 때 콘크리트가 부담하는 공칭 전단강도			[Unit=Force]
    CumsKsce.dVc     =dVc;	 	  //콘크리트가 부담하는 공칭 전단강도			[Unit= Force]
    CumsKsce.dVs     =dVs_min;	 	  //전단철근이 부담하는 공칭 전단강도 			[Unit= Force]
    CumsKsce.dpVn    =dpVn;	    //단면의 공칭전단력		[Unit= Force]
    CumsKsce.dRatV   =dRatV;    //공칭전단력비
    CumsKsce.dAv     =dAv;		  //간격 s 이내에 있는 전단철근의 단면적	[Unit=Area]
    CumsKsce.dAvmin  =dAvmin;	  //최소전단철근	[Unit= Area]
    CumsKsce.dAvreq	 =dAv_req;	//필요전단철근	[Unit= Area]
    //설계시 사용되는 중간값
    CumsKsce.dbw     =dbw;		  //복부의 폭	[Unit=Length]
    CumsKsce.dd      =dd;		    //콘크리트압축 연단에서 인장측 철근 중심까지의 거리		[Unit=Length]
    CumsKsce.dVi     =dVi; 	    //dVu-dVi				[Unit=Force]
    CumsKsce.dVd     =dVd;		  //사용 고정하중에 의해 발생하는 단면의 전단력			[Unit=Force]
    CumsKsce.dMmax   =dMmax;	  //dMu-dMd				[Unit= Moment]
    CumsKsce.dMd     =dMd;		  //하중계수를 고려하지 않은 고정하중 모멘트	   	     [Unit=Moment]
    CumsKsce.dMcr    =dMcr;	    //외부 작용하중에 의한 단면의 휨 균열모멘트			[Unit=Moment]
    CumsKsce.dIy     =dIner_g; 	//강축 단면 2차모멘트 		[Unit=Length^4]
    CumsKsce.dh			 =dzDim;    //단면춤 		[Unit=Length]
    CumsKsce.dyt     =dyt_g;	  //총단면적의 중립축에서 인장 연단까지의 거리			[Unit=Length]
    CumsKsce.dfpe    =dfpe_ave; //손실이 일어난 후 강재의 유효프리스트레스			[Unit=Stress]
    CumsKsce.dfd     =dfd;      //외부 작용하중에 의해 인장응력이 발생하는 단면의 최외연에서의 사용 고정하중에의한 응력		 [Unit=Stress]
    CumsKsce.dfpc    =dfpc;	    //유효프리스트레스 힘으로 인한 콘크리트 도심에서의 압축응력	 [Unit=Stress]
    CumsKsce.dVp     =dVp;      //단면의 유효프리스트레스 힘의 수직 분력				[Unit=Stress]
    CumsKsce.dfck    =dfck;	    //콘크리트 설계기준강도 			[Unit=Stress]
    CumsKsce.dsv     =dsv; 	    //경사인장철근의 간격			[Unit=Length]
    CumsKsce.dThetaV =dThetaV;  //경사인장철근의 각도			[Unit=Rad]
    CumsKsce.dsp     =dsp; 	    //전단강봉의 간격				[Unit=Length]
    CumsKsce.dThetap =dThetaP;  //전단강봉의 각도			[Unit=Rad]
    CumsKsce.dAp     =dAp; 	    //전단강봉의 배치량   [Unit= Area]

    return TRUE;

}

/*  
double Get_PhiT()
{
return 0.8;
}
*/

BOOL CDgnKSCE_USD03::Get_TorsionDesign(BOOL bYaxis, BOOL bPosiI, double dPhiv, T_MATD_D MatD, T_SPSC_SECT& SpscSect, T_SPSC_SECT& SpscgSect, BOOL bClosedSect, double dAoh, double dPh, double dShrThk, double dTorThk, double dPcp, double dAcp, double dfdt, double dfdb, double dVd, double dMd, 
                                       CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, T_RPSC_SBAR& RpscSbar, T_CRMT_KSCE_BASE& CrmtKsce)
{
    int iVciType = 2; // 1=자중고려함, 2=자중고려안함.

    //Material
    double dfck= MatD.Data1.Design.C_fc;
    double dEc = MatD.Data1.Analysis.Elast;
    double dfy = MatD.MainRebarData.B_fy;
    double dEs = MatD.MainRebarData.B_Elast;
    double dfys = MatD.SubRebarData.B_fy;
    double dEsv = MatD.SubRebarData.B_Elast;
    double dfyv = MatD.SubRebarData.B_fy; //폐쇄된 횡비틀림 철근의 항복강도.
    double dfyl = MatD.MainRebarData.B_fy; //종방향 비틀림 철근의 항복강도.
    //Section
    double dyDim = SpscSect.dyDim;
    double dzDim = SpscSect.dzDim;
    double dyCen = SpscSect.dyCen;
    double dzCen = SpscSect.dzCen;
    double dCpy  = dyDim - dyCen;
    double dCpz  = dzDim - dzCen;
    double dArea = SpscSect.dArea;
    double dIyy  = SpscSect.dIyy;
    double dH = dzDim;
    double dyt = dzCen; 
    double dI = dIyy;
    double dd_org=0.0;
    double ddp_org=0.0;

    //Force
    double dMu = CrmtKsce.dMu;
    double dTu = fabs(CrmtKsce.dTu);
    double drTu = CrmtKsce.dTu;
    double dVu = CrmtKsce.dVu;

    //Gross Section
    double dyDim_g = SpscgSect.dyDim;
    double dzDim_g = SpscgSect.dzDim;
    double dyCen_g = SpscgSect.dyCen;
    double dzCen_g = SpscgSect.dzCen;
    double dCpy_g  = dyDim_g - SpscgSect.dyCen;
    double dCpz_g  = dzDim_g - SpscgSect.dzCen;
    double dAg     = SpscgSect.dArea;
    double dIyy_g  = SpscgSect.dIyy;
    double dIzz_g  = SpscgSect.dIzz;
    double dIner_g=(bYaxis==TRUE ? dIyy_g : dIzz_g);

    double dfpu=0.0, dfpy=0.0;
    double dAps=0.0;
    double dd=0.0, dbwd=0.0, dbwdp=0.0;

    BOOL bTorsionChk = FALSE;
    double dCp=0.0, dCp_g=0.0;
    if(dMu>=0.0) dCp = (bYaxis==TRUE ? dCpz : dCpy);
    else         dCp = (bYaxis==TRUE ? dzCen : dyCen);
    if(dMu>=0.0) dCp_g = (bYaxis==TRUE ? dCpz_g : dCpy_g);
    else         dCp_g = (bYaxis==TRUE ? dzCen_g : dyCen_g);

    double dSc=0.0;
    if(bYaxis) 
    {
        if(dMu>=0.0) dSc = (dzCen_g==0.0 ? 0.0 : dIyy_g/dzCen_g);
        else         dSc = (dzDim_g-dzCen_g==0.0 ? 0.0 : dIyy_g/(dzDim_g-dzCen_g));    
    }
    else
    {
        if(dMu>=0.0) dSc = (dyCen_g==0.0 ? 0.0 : dIzz_g/dyCen_g);
        else         dSc = (dyDim_g-dyCen_g==0.0 ? 0.0 : dIzz_g/(dyDim_g-dyCen_g));    
    }

    double dyt_g = (dMu>=0.0 ? dH-dCp_g : dCp_g);

    CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnConCent;
    arTndnConCent.RemoveAll();
    double dfpu_min = 1.7E308; 
    double dfpy_min = 1.7E308; 
    for(int i=0 ; i<arTndnPosi.GetSize() ; i++)
    {
        _DGN_TNDN_CRC rTndn = arTndnPosi.GetAt(i);
        _DGN_TNDN_CRC rTnfrC; rTnfrC.Initialize();
        rTnfrC = rTndn;
        double dyposi = (bYaxis==TRUE ? rTndn.dz : rTndn.dy);
        double dgap =  (dMu>=0.0 ? dCp_g-dCp : dCp-dCp_g); 
        if(bYaxis) rTnfrC.dz = rTndn.dz+dgap;
        else       rTnfrC.dy = rTndn.dy+dgap;
        arTndnConCent.Add(rTnfrC);
        dfpu_min = min(rTnfrC.dfpu,dfpu_min);   dfpu = dfpu_min;
        dfpy_min = min(rTnfrC.dfpy,dfpy_min);   dfpy = dfpy_min;

    }

    // fpcc Calculation.
    double dfpe_ave=0.0, dPe=0.0, dPeep=0.0, dVp=0.0;  // dPe는 단면의 전체 텐던.
    Get_fpeData(bYaxis, dMu, dAg, dIner_g, dyt_g, arTndnConCent, dAps, dPeep, dPe, dVp, dfpe_ave);
    dVp = fabs(dVp);

    //Get ds  
    double dAptol=0.0,  ddp=0.0;
    double dAsttol=0.0, ddst=0.0;
    double dAsctol=0.0, ddsc=0.0;
    _DGN_TNDN_RBAR rTdRb;
    rTdRb.Initialize();      
    BOOL bGetOk = Get_TndnReBarInfor(TRUE, dCpz, SpscSect, arRbarPosi, arTndnPosi, rTdRb);  
    dAptol =(dMu>=0.0 ? rTdRb.dApttol : rTdRb.dApctol); 
    ddp    =(dMu>=0.0 ? rTdRb.ddpt    : dH-rTdRb.ddpc); ddp = (dAptol==0.0 ? 0.0 : ddp);    
    dAsttol=(dMu>=0.0 ? rTdRb.dAsttol : rTdRb.dAsctol); 
    ddst   =(dMu>=0.0 ? rTdRb.ddst    : dH-rTdRb.ddsc); ddst = (dAsttol==0.0 ? 0.0 : ddst);
    dAsctol=(dMu>=0.0 ? rTdRb.dAsctol : rTdRb.dAsttol); 
    ddsc   =(dMu>=0.0 ? rTdRb.ddsc    : dH-rTdRb.ddst); ddsc = (dAsctol==0.0 ? 0.0 : ddsc);

    double dbw=dShrThk;
    double dAst=dAsttol;

    dd = dd_org = ddst;  
    ddp_org = ddp;
    if( dd==0.0 || dd<0.8*dH) dd = 0.8*dH;
    dbwd = dbw*dd;
    if( ddp==0.0 || ddp<0.8*dH) ddp = 0.8*dH;
    dbwdp = dbw*ddp;      

    double dVi   = dVu - dVd;
    double dMmax = dMu-dMd;
    double dfd = (dIyy_g==0.0 ? 0.0 : dMd * dyt_g / dIyy_g);  
    double dMcr=0., dVci_incl=0., dVci_cal=0.;
    if(iVciType==1)      
    {
        dMcr = dSc*(1.6*sqrt(dfck)+dfpe_ave-dfd);   //dfd: 외부 작용하중에 의해 인장응력이 발생하는 단면의 최외연에서의 사용 고정하중에 의한 응력  
        dVci_incl = (dMmax==0.0 ? dVd : dVd + dVi*dMcr/dMmax);
        dVci_cal = (dMmax==0.0 ? 0.16*sqrt(dfck)*dbwd + fabs(dVd) : 0.16*sqrt(dfck)*dbwd + fabs(dVd + fabs(dVi*dMcr/dMmax)) );
    }
    else if(iVciType==2) 
    {
        dMcr = dSc*(1.6*sqrt(dfck)+dfpe_ave);
        dVci_incl = (dMu==0.0 ? 0.0 : dVu*dMcr/dMu);
        dVci_cal = (dMmax==0.0 ? 0.16*sqrt(dfck)*dbwd + fabs(dVd) : 0.16*sqrt(dfck)*dbwd + fabs(dVu*dMcr/dMu));
    } 

    double dVci_min = 0.45*sqrt(dfck)*dbwd;
    double dVci = max(dVci_cal, dVci_min);
    double dfpc = (dAg==0.0 ? 0.0 : dPe/dAg);  //유효프리스트레스 힘으로 인한 콘크리트 도심에서의 압축응력..
    double dVcw = (0.93*sqrt(dfck)+0.3*dfpc)*dbwd+dVp;
    double dVc = min(dVci, dVcw);

    //dAcp : 콘크리트 단면의 바깥 둘레로 둘러싸인 단면적으로, 속빈 단면에서는 속빈 면적을 포함.  
    double dTcr=0.0;
    double dTorSkipValue=0.0;
    double dphit = dPhiv;
    dTcr = (dPcp==0.0 ? 0.0 : 1.06*sqrt(dfck)*pow(dAcp,2.0)/dPcp*sqrt(1.0+dfpc/(1.06*sqrt(dfck))));
    dTorSkipValue=(dPcp==0.0 ? 0.0 : dphit*0.26*sqrt(dfck)*pow(dAcp,2.0)/dPcp*sqrt(1.0+dfpc/(1.06*sqrt(dfck))));
    // dAoh : 비틀림 저항철근의 중심선으로 폐쇄된 면적
    // dPh  : 외곽부 폐쇄 횡방향 비틀림 철근의 중심선의 둘레길이.
    //전단력과 비틀림에 의한 전단응력
    double dStrST=0.0, dStrST_lim=0.0;
    double dtw=dTorThk; 
    double dA0=0.0;
    double dthetaT=0.0;
    double dAohOverPh=0.0;
    if(bClosedSect) 
    {
        if(dbwd==0.0 && dAoh!=0.0) dStrST = dTu*dPh/(1.7*pow(dAoh,2.0));
        else if(dbwd!=0.0 && dAoh==0.0) dStrST = fabs(dVu)/dbwd;
        else if(dbwd==0.0 && dAoh==0.0) dStrST = 0.0;
        else dStrST = sqrt(pow(dVu/dbwd,2.0)+pow(dTu*dPh/(1.7*pow(dAoh,2.0)),2.0));
    }
    else
    {
        dAohOverPh = (dPh==0.0 ? 0.0 : dAoh/dPh);
        if(dtw < dAohOverPh) 
        {
            if(dbwd==0.0 && dAoh*dtw!=0.0) dStrST = dTu/(1.7*dAoh*dtw);
            else if(dbwd!=0.0 && dAoh*dtw==0.0) dStrST = fabs(dVu)/dbwd;
            else if(dbwd==0.0 && dAoh*dtw==0.0) dStrST = 0.0;
            else dStrST = fabs(dVu)/dbwd + dTu/(1.7*dAoh*dtw);
        }    
        else
        {
            if(dbwd==0.0 && dAoh!=0.0) dStrST = dTu*dPh/(1.7*pow(dAoh,2.0));
            else if(dbwd!=0.0 && dAoh==0.0) dStrST = fabs(dVu)/dbwd;
            else if(dbwd==0.0 && dAoh==0.0) dStrST = 0.0;
            else dStrST = fabs(dVu)/dbwd + dTu*dPh/(1.7*pow(dAoh,2.0));
        }    
    }  
    dStrST_lim = (dbwd==0.0 ? 0.0 : dphit*(dVc/dbwd+2.12*sqrt(dfck))); 

    double dst = RpscSbar.dLbarPitch;    // 횡방향철근의 간격
    double dAt = RpscSbar.dLbarAwt;      // 간격 s내의 비틀림에 저항하는 폐쇄스터럽의 다리 1개의 면적, 혹은 비틀림모멘트에 대한 복부 개당의 횡방향 필요 철근량.
    //압축경사각으로 30~60도 
    double dThetaC=0.0; 
    if(dAst*dfy*0.4>=dPe) dThetaC = 45.0*DEGTORAD;  //fs값 대신fy를 
    else if(dAst*dfy*0.4<dPe) dThetaC = 37.5*DEGTORAD;
    else if(dAst==0.0) dThetaC = 45.0*DEGTORAD;  
    dA0 = 0.85*dAoh;  
    double dTn = (dst==0.0 || sin(dThetaC)==0.0 ? 0.0 : 2.0*dA0*dAt*dfyv*cos(dThetaC)/sin(dThetaC)/dst);
    double dpTn= dphit*dTn;
    double dRatT= (dpTn==0.0 ? 0.0 : dTu/dpTn);
    double dAluse = RpscSbar.dLbarAlt;
    double dAlreq = (dst==0.0 || dfyl==0.0 || sin(dThetaC)==0.0 ? 0.0 : dAt/dst*dPh*(dfyv/dfyl)*pow(cos(dThetaC)/sin(dThetaC),2));
    double dAv = RpscSbar.dSbarAw;
    double dAtmin =0.0;
    if(3.5*dbw*dst/dfyv-dAv>0.0)  dAtmin = (3.5*dbw*dst/dfyv-dAv)/2.0;
    else                          dAtmin = 0.0;
    double dAtst = (dst==0.0 ? 0.0 : dAt/dst);
    double dbwfyv =  (dfyv==0.0 ? 0.0 : 1.75*dbw/dfyv);
    double AtOverS = max(dAtst, dbwfyv);
    double dAlmin = (dfyl==0.0 ? 0.0 : 1.32*sqrt(dfck)*dAcp/dfyl-AtOverS*dPh*dfyv/dfyl);

    if(dTorSkipValue <= dTu) bTorsionChk = TRUE;

    BOOL bPrint = m_Print.Get_PrintFlag();
    if(bPrint)
    {
        m_Print.m_CrmtParam.dfck = dfck;
        m_Print.m_CrmtParam.dbw  = dbw;
        m_Print.m_CrmtParam.dt   = dtw;
        m_Print.m_CrmtParam.dfy  = dfy;
        m_Print.m_CrmtParam.dh   = dH;
        m_Print.m_CrmtParam.dIy  = dIner_g;

        m_Print.m_CrmtParam.dfpu = dfpu;
        m_Print.m_CrmtParam.dd   = dd_org;
        m_Print.m_CrmtParam.ddp  = ddp_org;
        m_Print.m_CrmtParam.dAc  = dAg;
        m_Print.m_CrmtParam.dfpy = dfpy;
        m_Print.m_CrmtParam.dyt  = dyt_g;
        m_Print.m_CrmtParam.dAps = dAps;
        m_Print.m_CrmtParam.dPcp = dPcp;
        m_Print.m_CrmtParam.dAcp = dAcp;

        m_Print.Print_CrmtDesign0();    
        //m_Print.Print_CrmtDesign1(dfck, dbw, dtw , dfy, dH, dIner_g);
        //m_Print.Print_CrmtDesign2(dfpu, dd, ddp, dAg, dfpy, dyt, dAps, dPcp, dAcp);

        m_Print.Print_CrmtDesign3(dMu, drTu, dVu, dMd, dVd, dfd, dPe, dfpe_ave);
        m_Print.Print_CrmtDesign4(dfck, dAcp, dPcp, dfpc, dTcr, drTu, dfpe_ave);

        if(bTorsionChk)
        {
            m_Print.Print_CrmtDesign5();
            //콘크리트가 부담하는 전단강도의 산정은 전단 검토와 같음.
            m_Print.Print_CumsDesign4(dH, dd, dd_org, dPhiv*dH, ddp, ddp_org, dIner_g, dyt_g, dfck, dfpe_ave, dfd, dMcr);
            m_Print.Print_CumsDesign5(dMu, dMd, dMmax, dVu, dVd, dVi, dVci_cal, dVci, dVci_incl, dfck, dbw, dd, dMcr, dVci_min);
            m_Print.Print_CumsDesign6(dPe, dAg, dfpc, dfck, dbw, dd, dVp, dVcw, dVci, dVc);

            m_Print.Print_CrmtDesign6(bClosedSect, dAohOverPh, dVu, dVc, dbw, dd, ddp, drTu, 
                dfck, dTorSkipValue, dPh, dAoh, dtw, dStrST, dStrST_lim);
            m_Print.Print_CrmtDesign7(dAoh, dA0, dAt, dfyv, dst, dThetaC, dpTn, dfyl, dPh, dAlmin);
            m_Print.Print_CrmtDesign8(dbw, dst, dfyv, dAv, dAtmin, dfck, dAcp, dAt, dPh, dfyl, dAlmin, dAtst, dbwfyv, AtOverS);

        }
    }

    CrmtKsce.dTu		 =drTu;		          //계수 비틀림모멘트					[Unit=Moment]
    //CrmtKsce.dTcr    =dTcr;           //비틀림 균열 모멘트 					[Unit=Moment]		
    CrmtKsce.dTcr    =dTorSkipValue;    // 1/4Tcr
    CrmtKsce.dAoh    =dAoh;             //비틀림 저항철근의 중심선으로 폐쇄된 면적	[Unit=Area]
    CrmtKsce.dVc	 	 =dVc;	 	          //콘크리트가 부담하는 공칭 전단강도			[Unit= Force]
    CrmtKsce.dvVT	   =dStrST;	          //전단력과 비틀림에 의한 전단응력 			[Unit= Stress]
    CrmtKsce.dvVTmax =dStrST_lim;	      //전단균열을 유발하는 응력과 제한응력의 합	[Unit= Stress]
    CrmtKsce.dpTn	   =dpTn;	            //공칭비틀림 모멘트 					[Unit=Torsion]
    CrmtKsce.dRatT   =dRatT;            //비틀림강도 안전률					[Unit=None]
    CrmtKsce.dAt		 =dAt;		          //간격 s 내의 비틀림에 저항하는 폐쇄스터럽의 다리 1개의 면적	[Unit=Area]
    CrmtKsce.dAtmin	 =dAtmin;	          //횡방향 폐쇄스터럽의 최소 면적			[Unit= Area]
    CrmtKsce.dAluse	 =dAluse;	          //종방향 비틀림 철근의 총면적				[Unit=Area]
    CrmtKsce.dAlreq  =dAlreq;           //종방향 비틀림 철근의 총면적				[Unit=Area]
    CrmtKsce.dAlmin	 =dAlmin;	          //최소 종방향 비틀림 철근량				[Unit= Area]
    CrmtKsce.dAcp	   =dAcp;	            //콘크리트 단면의 바깥 둘레로 둘러싸인 단면적		[Unit= Area]
    CrmtKsce.dPcp	   =dPcp;	            //전단면의 둘레의 길이						[Unit=Length]
    CrmtKsce.dfpc 	 =dfpc; 	          //유효프리스트레스 힘으로 인한 콘크리트 도심에서의 압축응력	 [Unit=Stress]
    CrmtKsce.dfck	   =dfck;	            //콘크리트 설계기준강도 					[Unit=Stress]
    CrmtKsce.dVu		 =dVu;		          //단면의 계수 전단력					[Unit= Force]
    CrmtKsce.dPh		 =dPh;		          //외곽부 폐쇄 횡방향 비틀림 철근의 중심선의 둘레길이     [Unit= Length]
    CrmtKsce.dbw		 =dbw;		          //복부의 폭							[Unit=Length]
    CrmtKsce.dt 		 =dtw;		          //벽의 두께 				 			[Unit=Length]
    CrmtKsce.dThetaC =dThetaC;          //압축경사각 			 			[Unit=Rad]
    CrmtKsce.dfyv	   =dfyv;	            //폐쇄된 횡비틀림 철근의 항복강도	  		[Unit=Stress]
    CrmtKsce.dfyl    =dfyl;             //종방향 비틀림 철근의 항복강도			[Unit=Stress]
    return TRUE;

}


/*
BOOL CDgnKSCE_USD03::Get_TopBotStress(T_SPSC_SECT& SpscSect)
{
return TRUE;
}
*/
/*
struct T_RPSC_SBAR      // 전단보강 철근
{
BOOL   bSbarExist;    // 경사인장철근 정보 있으면 TRUE
double dSbarPitch;    // 경사인장철근의 간격
double dSbarTheta;    // 경사인장철근의 각도
double dSbarAw;       // 경사인장철근의 배치량   //전체 배치량?
BOOL   bTndnExist;    // 전단강봉 정보 있으면 TRUE
double dTndnDist;     // 전단강봉의 간격
double dTndnTheta;    // 전단강봉의 각도
double dTndnAp;       // 전단강봉의 배치량
double dTndnPs;       // 전단강봉의 유효인장력
BOOL   bLbarExist;    // 횡방향철근 정보 있으면 TRUE
double dLbarPitch;    // 횡방향철근의 간격
double dLbarAwt;      // 횡방향철근의 Pitch당 철근 1개의 단면적
double dLbarAlt;      // 횡뱡향철근의 전단면적
*/




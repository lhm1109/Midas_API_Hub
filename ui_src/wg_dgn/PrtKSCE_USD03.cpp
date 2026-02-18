// PrtKSCE_USD03.cpp: implementation of the CPrtKSCE_USD03 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "PrtKSCE_USD03.h"

#include "CRCDataCtrl.h"
#include "CRCForceCtrl.h"
#include "DgnKSCE_USD03.h"
#include "DgnKSCE_USD05.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\wg_db_AnalysisResult.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrtKSCE_USD03::CPrtKSCE_USD03()
{
    m_pDoc=NULL;
    m_pDataCtrl=NULL;
    m_pForcCtrl=NULL;
    m_bPrintFlag=FALSE;

    m_arText.RemoveAll();
    m_StagTitleMap.RemoveAll();
    m_LcomTitleMap.RemoveAll();
    m_arElemK.RemoveAll();
    m_arStagK.RemoveAll();
    m_arLcomK.RemoveAll();

    m_amForcOrgLcom.RemoveAll();
    m_arElemMy[0].RemoveAll(); m_arElemMy[1].RemoveAll();
    m_arElemVz[0].RemoveAll(); m_arElemVz[1].RemoveAll();
    m_arElemMx[0].RemoveAll(); m_arElemMx[1].RemoveAll();

    m_dZero=1.0E-07;

    m_TotalNum = 6;
    m_decimal = 2;

    m_strLength = _T("cm");
    m_strForce  = _T("tonf");
    m_strArea   = _T("cm ~ ^2");
    m_strAreaM  = _T("m^2");
    m_strStif   = _T("cm ~ ^4");
    m_strStifM  = _T("m^4");
    m_strMoment = _T("tonf·m");
    m_strStress = _T("kgf/cm^2");

    m_sTxArea   = _T("cm²");  
    m_sTxAreaM  = _T("m²");
    m_sTxSecM   = _T("cm³");
    m_sTxSecMM  = _T("m³");
    m_sTxStif   = _T("cm⁴");
    m_sTxStifM  = _T("m⁴");
    m_sTxStress = _T("kgf/cm²");

    m_strDegree = _T("˚");
    m_strRadian = _T("rad");

    m_iVciType = 2; // 1=자중고려함, 2=자중고려안함.
}

CPrtKSCE_USD03::~CPrtKSCE_USD03()
{
}

void CPrtKSCE_USD03::SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl, CCRCForceCtrl* pForcCtrl)
{
    m_pDoc        = CDBDoc::GetDocPoint();
    ASSERT(m_pDoc);
    m_pDataCtrl   = pDataCtrl;
    m_pForcCtrl   = pForcCtrl;
    if(!m_pDoc->IsPostMode())	ASSERT(0);
}

void CPrtKSCE_USD03::Set_InitData()
{
    GClearHistory();
    GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_XL_REPORT_READ_DATA));
    CWaitCursor Wait;

    m_pDataCtrl->Initialize();
    if(!m_pDataCtrl->Cal_LcomDataForDesignAll())	ASSERT(0);
    if(!m_pDataCtrl->Get_CrcElemListForDgn(FALSE,m_arElemK))	ASSERT(0);
    m_pDoc->m_pPostCtrl->GetStageInfo()->GetResultSavedStagKeyList(m_arStagK);
    m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, m_arLcomK);
    if(m_arStagK.GetSize()==0 || m_arLcomK.GetSize()==0)	ASSERT(0);

    m_iDgnCode = m_pDataCtrl->Get_Code();
    CString strStresUnit=_T("");
    if(m_iDgnCode == KSCE_USD03)
    {
        m_strStresUnit  = _T("kgf/cm²");
        m_strForceUnit  = _T("tonf");
        m_strMomentUnit = _T("tonf·m");
    }
    else if(m_iDgnCode == KSCE_USD05    ||  m_iDgnCode == KSCE_USD10    ||
        m_iDgnCode==KSCE_RAIL_USD04 ||  m_iDgnCode==KSCE_RAIL_USD11 ||
        m_iDgnCode == JTG_D62_04    ||  m_iDgnCode == CJJ_11_2011   ) 
    {
        m_strStresUnit  = _T("N/mm²");
        m_strForceUnit  = _T("kN");
        m_strMomentUnit = _T("kN·m");
    }
    else ASSERT(0);

    if(!Get_ForcDataByStag())	ASSERT(0);
    if(!Get_ForcDataByOrgLcom())	ASSERT(0);
}

void CPrtKSCE_USD03::Set_PrintFlag(BOOL bPrintFlag)
{
    m_bPrintFlag = bPrintFlag;
}

BOOL CPrtKSCE_USD03::Get_PrintFlag()
{
    return m_bPrintFlag;
}

BOOL CPrtKSCE_USD03::Get_ForcDataByStag()
{
    m_amForcStag.RemoveAll();
    if(!m_bPrintFlag) return FALSE;

    int i=0, j=0, k=0;
    int iElemNum = m_arElemK.GetSize();
    int iStagNum = m_arStagK.GetSize();

    _PRT_FORC_GRPH_GRUP GrphGrup;
    for(i=0; i<iStagNum; i++)
    {
        T_STAG_K StagK = m_arStagK.GetAt(i);
        int iFinalStepNo=0;
        // Get Last StepNo at Current Stage.
        if(!m_pDataCtrl->Get_LastStepInCurStage(StagK, iFinalStepNo))	return FALSE;
        CString strSgldName = _LS(IDS_DB_SGLD_SUM);	// SUM.
        T_SGLD_K SgldK = m_pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);

        // Get Force Data by Load Case.
        T_LCOM_D LcasD;
        LcasD.Initialize();
        if(!m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_STAGE, SgldK, LcasD))	continue;
        if(iFinalStepNo > 0)	m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcasD,iFinalStepNo,2);	// 0=Nonlinear, 1=Pushover, 2=Consruction.
        else									m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcasD);

        GrphGrup.Initialize();
        T_STRB_D StrMaxData, StrMinData, StrAbsData;
        for(j=0; j<iElemNum; j++)
        {
            auto ElemK = m_arElemK.GetAt(j);

            int nMatlK = CDBLib::GetMaterialKey4ElemPairK(ElemK);

            double dElemLen = CDBLib::GetLengthOfBeam(ElemK);

            T_TMAT_D TmatD; TmatD.Initialize();
            T_TDME_D TdmeD; TdmeD.Initialize();

            BOOL bGetTdme = FALSE;
            if(m_pDoc->m_pPostCtrl->GetTmatPost(nMatlK, TmatD))
                bGetTdme = m_pDoc->m_pAttrCtrl->GetTdme(TmatD.TdMatlElastKey, TdmeD);

            double dfci=0.0;
            if(m_pDoc->m_pPostCtrl->GetStageInfo()->IsActiveElemInTgtStag(StagK, ElemK))
            {
                T_STAG_STEP_INFO StageInfo; StageInfo.Initialize();
                m_pDoc->m_pAttrCtrl->GetStagStepInfo(StagK,StageInfo);
                int iStepSize = StageInfo.aInfo.GetSize();
                int nStepArr = iStepSize==0 ? 1 : iStepSize;
                int iSerialStep=0;
                int nCurStep=0;

                double dFirstStep_Day=0.0;
                for(int jj=0; jj<nStepArr; ++jj)
                {
                    nCurStep=jj+1; 
                    if(iStepSize==0)
                    {
                        m_pDataCtrl->Get_LastStepInCurStage(StagK, iSerialStep);
                    }
                    else
                    {
                        iSerialStep=0;
                        if(!m_pDoc->m_pPostCtrl->GetStageInfo()->GetStepSerialKey(StagK,nCurStep,iSerialStep)) continue;
                    }

                    double dStep_Day=0.0;
                    double dDay=0.0;
                    m_pDoc->m_pPostCtrl->GetStageInfo()->GetStagStepDay(StagK, nCurStep, dStep_Day); // Stage에서 각 step의 날짜

                    if(jj==0) dFirstStep_Day = dStep_Day;

                    T_ELFM_D dDayActive; dDayActive.Initialize();
                    m_pDoc->m_pPostCtrl->GetElfmDgn(StagK, ElemK, dDayActive);  // Stage의 시작과 끝에서 재료 날짜.
                    dDay = dStep_Day + dDayActive.dStartMatAge ;

                    if(bGetTdme) m_pDataCtrl->Get_InitCompressStrength(TmatD.TdMatlElastKey, dDay, dfci);
                }
            }

            double dAlw[6] = {0., 0., 0., 0., 0., 0.}; // I(0~2), J(3~5)
            if(m_iDgnCode==KSCE_USD03)
            {
                m_pDataCtrl->Get_DgnKscePointer()->Get_AllowableStressofLcom(TRUE,ElemK,dAlw); // TRUE=시공(초기)
            }
            else if(m_iDgnCode==KSCE_USD05 || m_iDgnCode==KSCE_RAIL_USD04 || m_iDgnCode==KSCE_USD10 || m_iDgnCode==KSCE_RAIL_USD11)
            {
                m_pDataCtrl->Get_DgnKsce05Pointer()->Get_AllowableStressofLcom(TRUE, ElemK, dfci, dAlw);
            }
            else if(m_iDgnCode==JTG_D62_04 ||m_iDgnCode == CJJ_11_2011)
            {
                //m_pDataCtrl->Get_DgnJtgPointer()->Get_AllowableStressofLcom(ElemK,dAlw);
            }
            else ASSERT(0);

            StrMaxData.Initialize();
            StrMinData.Initialize();
            StrAbsData.Initialize();
            if(!m_pDoc->m_pPostCtrl->GetStrbNew(ElemK.first, &StrMaxData, &StrMinData, &StrAbsData, TRUE, NULL, ElemK.second))	continue;
            // Get Fysg.
            T_FYSG_D FysgD;
            FysgD.Initialize();
            if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFysgData(ElemK,FysgD))	continue;
            // Set Stresses.
            _PRT_FORC_GRPH_ELEM GrphElem;
            GrphElem.Initialize();
            GrphElem.ElemK = ElemK.first;
            GrphElem.dLeng = Cng_LenFix(dElemLen,1);
            for(k=0; k<cPOSI_CRC; k++)
            {
                int iPart	 = (k==0 ? 0 : 3);
                int iKind1 = (k==0 ? 0 : 7);
                int iKind2 = (k==0 ? 0 : 1);
                GrphElem.dStl[k] = Cng_StrFix((-1)*StrMaxData.dblStress[iPart][iKind2+16]);
                GrphElem.dStr[k] = Cng_StrFix((-1)*StrMaxData.dblStress[iPart][iKind2+18]);
                GrphElem.dSbl[k] = Cng_StrFix((-1)*StrMaxData.dblStress[iPart][iKind2+22]);
                GrphElem.dSbr[k] = Cng_StrFix((-1)*StrMaxData.dblStress[iPart][iKind2+20]);
                if(m_iDgnCode==JTG_D62_04 ||m_iDgnCode == CJJ_11_2011)
                {
                    GrphElem.dAlc[k]    = Cng_StrFix(FysgD.FysgBase[k*2+0].dALW);	// MAX.
                    GrphElem.dAlt[k][0] = Cng_StrFix(FysgD.FysgBase[k*2+1].dALW);	// MIN.
                }
                else
                {
                    GrphElem.dAlc[k]    = (-1) * dAlw[k*3+2];
                    GrphElem.dAlt[k][0] = (-1) * dAlw[k*3+0];
                    GrphElem.dAlt[k][1] = (-1) * dAlw[k*3+1];
                }
            }
            GrphGrup.dExtraRat = 1.0;
            GrphGrup.arGrphElemMax.Add(GrphElem);
        }
        m_amForcStag.SetAt(StagK, GrphGrup);
    }
    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_ForcDataByStrs()
{
    m_amForcOrgLcom.RemoveAll();
    if(!m_bPrintFlag) return FALSE;

    int i=0, j=0, k=0, m=0;
    int iElemNum = m_arElemK.GetSize();
    int iLcomNum = m_pForcCtrl->Get_LcomNumForStrs();
    if(iElemNum==0 || iLcomNum==0)  return FALSE;

    //***************************************
    _PRT_FORC_GRPH_ELEM** GrphMax = new _PRT_FORC_GRPH_ELEM* [iElemNum];
    _PRT_FORC_GRPH_ELEM** GrphMin = new _PRT_FORC_GRPH_ELEM* [iElemNum];
    for(i=0; i<iElemNum; i++)
    {
        GrphMax[i] = new _PRT_FORC_GRPH_ELEM [iLcomNum];
        GrphMin[i] = new _PRT_FORC_GRPH_ELEM [iLcomNum];
        for(j=0; j<iLcomNum; j++)
        {
            GrphMax[i][j].Initialize(TRUE);
            GrphMin[i][j].Initialize(FALSE);
        }
    }
    T_LCOM_K* LcomOrgK = new T_LCOM_K [iLcomNum];
    BOOL* bMaxMinLcomOrg = new BOOL [iLcomNum];
    double* dpExtraRat = new double [iLcomNum];
    for(i=0; i<iLcomNum; i++)
    {
        LcomOrgK[i]=0;
        bMaxMinLcomOrg[i]=FALSE;
        dpExtraRat[i]=0.0;
    }
    //***************************************

    double dfci=0.0;
    _DGN_STRS_CRC StrsMax, StrsMin;
    for(i=0; i<iElemNum; i++)
    {
        auto ElemK = m_arElemK.GetAt(i);

        double dElemLen = CDBLib::GetLengthOfBeam(ElemK);

        double dAlw[6] = {0., 0., 0., 0., 0., 0.}; // I(0~2), J(3~4)
        if(m_iDgnCode==KSCE_USD03)
        {
            m_pDataCtrl->Get_DgnKscePointer()->Get_AllowableStressofLcom(FALSE,ElemK,dAlw); // FALSE=최종.
        }
        else if(m_iDgnCode==KSCE_USD05 || m_iDgnCode==KSCE_RAIL_USD04 || m_iDgnCode==KSCE_USD10 || m_iDgnCode==KSCE_RAIL_USD11)
        {
            m_pDataCtrl->Get_DgnKsce05Pointer()->Get_AllowableStressofLcom(FALSE,ElemK,dfci,dAlw);
        }
        else if(m_iDgnCode==JTG_D62_04 ||m_iDgnCode == CJJ_11_2011)
        {
            //m_pDataCtrl->Get_DgnJtgPointer()->Get_AllowableStressofLcom(ElemK,dAlw);
        }
        else ASSERT(0);

        // Change by ZINU.('05.01.04). For Beam Section Temperature.
        // Change by Jaeoh.('05.09.16).
        int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
        ADGNSTRESS aStressLcom;
        ADGNSTRSADD aStressAdd;
        if(!m_pForcCtrl->Get_LcomDataForStress(ElemK, TRUE, FALSE, aStressLcom, aStressAdd, iLcomNum))	continue;  // TRUE=Include Tendon Primary.
        // Get Fycm.
        T_FYCM_D FycmD;
        FycmD.Initialize();
        if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFycmData(ElemK,FycmD))	continue;
        for(j=0; j<aStressLcom.GetSize(); j++)
        {
            _DGN_STRS_CRC StressLcom = aStressLcom.GetAt(j);
            _DGN_STRS_ADD StressAdd  = aStressAdd.GetAt(j);
            int iDgnLcomNo = j+1;
            CString strOrgLcomNa=_T("");
            int iOrgLcomNo=0, iSerLcomCount=0;
            int iMaxMinType=0;
            if(!m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType)) continue;
            /////////////////////////////////////
            // Only 응력검토시.
            if(!m_pForcCtrl->Is_LcomForStrs(iDgnLcomNo))  continue;
            m = iSerLcomCount-1;	// Serial Count by OrgLcomK.
            if(m < 0 || m >= aStressLcom.GetSize())	ASSERT(0);
            LcomOrgK[m] = iOrgLcomNo;
            bMaxMinLcomOrg[m] = (iMaxMinType > 0 ? TRUE : FALSE);
            dpExtraRat[m] = m_pForcCtrl->Get_ExtraRatio(iDgnLcomNo);
            StrsMax.Initialize();
            StrsMin.Initialize();
            //if(!m_pForcCtrl->Get_StrsByForc(ElemK, iDgnLcomNo, ForceLcom, StressAdd, StrsMax, StrsMin))	continue;
            if(!m_pForcCtrl->Get_StrsByStrs(ElemK, iDgnLcomNo, StressLcom, StrsMax, StrsMin)) continue;
            for(k=0; k<cPOSI_CRC; k++)
            {
                // Compare Max Stress.
                if(max(StrsMax.dStl[k],StrsMax.dStr[k]) > max(GrphMax[i][m].dStl[k],GrphMax[i][m].dStr[k]))	// Top.
                {
                    GrphMax[i][m].dStl[k] = StrsMax.dStl[k];
                    GrphMax[i][m].dStr[k] = StrsMax.dStr[k];
                }
                if(max(StrsMax.dSbl[k],StrsMax.dSbr[k]) > max(GrphMax[i][m].dSbl[k],GrphMax[i][m].dSbr[k]))	// Bot.
                {
                    GrphMax[i][m].dSbl[k] = StrsMax.dSbl[k];
                    GrphMax[i][m].dSbr[k] = StrsMax.dSbr[k];
                }
                // 여기서 각 하중조합에 대한 계수를 적용해야 한다.
                double dExtraRatio = m_pForcCtrl->Get_ExtraRatio(iDgnLcomNo);
                if(m_iDgnCode==JTG_D62_04 ||m_iDgnCode == CJJ_11_2011)
                {
                    GrphMax[i][m].dAlc[k]	   = Cng_StrFix(FycmD.FycmBase[k*2+0].dALW);	// MAX.
                    GrphMax[i][m].dAlt[k][0] = Cng_StrFix(FycmD.FycmBase[k*2+1].dALW);	// MIN.
                }
                else
                {
                    GrphMax[i][m].dAlc[k]	   = (-1) * dExtraRatio * dAlw[k*3+2];
                    GrphMax[i][m].dAlt[k][0] = (-1) * dExtraRatio * dAlw[k*3+0];
                    GrphMax[i][m].dAlt[k][1] = (-1) * dExtraRatio * dAlw[k*3+1];
                }				
                GrphMax[i][m].ElemK = ElemK.first;
                GrphMax[i][m].dLeng = dElemLen;
                // Compare Min Stress.
                if(bMaxMinLcomOrg[m])	// Only Max/Min.
                {
                    if(min(StrsMin.dStl[k],StrsMin.dStr[k]) < min(GrphMin[i][m].dStl[k],GrphMin[i][m].dStr[k]))	// Top.
                    {
                        GrphMin[i][m].dStl[k] = StrsMin.dStl[k];
                        GrphMin[i][m].dStr[k] = StrsMin.dStr[k];
                    }
                    if(min(StrsMin.dSbl[k],StrsMin.dSbr[k]) < min(GrphMin[i][m].dSbl[k],GrphMin[i][m].dSbr[k]))	// Bot.
                    {
                        GrphMin[i][m].dSbl[k] = StrsMin.dSbl[k];
                        GrphMin[i][m].dSbr[k] = StrsMin.dSbr[k];
                    }

                    if(m_iDgnCode==JTG_D62_04 ||m_iDgnCode == CJJ_11_2011)
                    {
                        GrphMin[i][m].dAlc[k]	   = Cng_StrFix(FycmD.FycmBase[k*2+0].dALW);	// MAX.
                        GrphMin[i][m].dAlt[k][0] = Cng_StrFix(FycmD.FycmBase[k*2+1].dALW);	// MIN.
                    }
                    else
                    {
                        GrphMin[i][m].dAlc[k]	   = (-1) * dExtraRatio * dAlw[k*3+2];
                        GrphMin[i][m].dAlt[k][0] = (-1) * dExtraRatio * dAlw[k*3+0];
                        GrphMin[i][m].dAlt[k][1] = (-1) * dExtraRatio * dAlw[k*3+1];
                    }				
                    GrphMin[i][m].ElemK = ElemK.first;
                    GrphMin[i][m].dLeng = dElemLen;
                }
            }
        }
    }

    _PRT_FORC_GRPH_ELEM GrphElem;
    _PRT_FORC_GRPH_GRUP GrphGrup;
    for(i=0; i<m+1; i++)
    {
        GrphGrup.Initialize();
        for(j=0; j<iElemNum; j++)
        {
            // Max.
            GrphElem.Initialize();
            GrphElem.ElemK = GrphMax[j][i].ElemK;
            GrphElem.dLeng = Cng_LenFix(GrphMax[j][i].dLeng,1);
            for(k=0; k<cPOSI_CRC; k++)
            {
                GrphElem.dStl[k] = Cng_StrFix(GrphMax[j][i].dStl[k]);
                GrphElem.dStr[k] = Cng_StrFix(GrphMax[j][i].dStr[k]);
                GrphElem.dSbl[k] = Cng_StrFix(GrphMax[j][i].dSbl[k]);
                GrphElem.dSbr[k] = Cng_StrFix(GrphMax[j][i].dSbr[k]);
                GrphElem.dAlc[k] = GrphMax[j][i].dAlc[k];
                GrphElem.dAlt[k][0] = GrphMax[j][i].dAlt[k][0]; // Code Unit
                GrphElem.dAlt[k][1] = GrphMax[j][i].dAlt[k][1];
            }
            GrphGrup.arGrphElemMax.Add(GrphElem);
            // Min.
            if(bMaxMinLcomOrg[i])
            {
                GrphElem.Initialize();
                GrphElem.ElemK = GrphMin[j][i].ElemK;
                GrphElem.dLeng = Cng_LenFix(GrphMin[j][i].dLeng,1);
                for(k=0; k<cPOSI_CRC; k++)
                {
                    GrphElem.dStl[k] = Cng_StrFix(GrphMin[j][i].dStl[k]);
                    GrphElem.dStr[k] = Cng_StrFix(GrphMin[j][i].dStr[k]);
                    GrphElem.dSbl[k] = Cng_StrFix(GrphMin[j][i].dSbl[k]);
                    GrphElem.dSbr[k] = Cng_StrFix(GrphMin[j][i].dSbr[k]);
                    GrphElem.dAlc[k] = GrphMin[j][i].dAlc[k];
                    GrphElem.dAlt[k][0] = GrphMin[j][i].dAlt[k][0]; // Code Unit
                    GrphElem.dAlt[k][1] = GrphMin[j][i].dAlt[k][1];
                }
                GrphGrup.arGrphElemMin.Add(GrphElem);
            }
        }
        int iOrgLcomNo = LcomOrgK[i];
        GrphGrup.dExtraRat = dpExtraRat[i];
        m_amForcOrgLcom.SetAt(iOrgLcomNo,GrphGrup);
    }
    //***************************************
    for(i=0; i<iElemNum; i++)
    {
        delete GrphMax[i];
        delete GrphMin[i];
    }
    delete[] GrphMax;
    delete[] GrphMin;
    delete[] LcomOrgK;
    delete[] bMaxMinLcomOrg;
    delete[] dpExtraRat;
    //***************************************

    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_ForcDataByStrn()
{
    m_arElemMy[0].RemoveAll();
    m_arElemMy[1].RemoveAll();
    m_arElemVz[0].RemoveAll();
    m_arElemVz[1].RemoveAll();
    m_arElemMx[0].RemoveAll();
    m_arElemMx[1].RemoveAll();
    if(!m_bPrintFlag) return FALSE;

    int i=0, j=0, k=0, m=0;
    int iElemNum = m_arElemK.GetSize();
    int iLcomNum = m_pForcCtrl->Get_LcomNumForStrn();
    if(iElemNum==0 || iLcomNum==0)  return FALSE;

    int iLcomNumData = m_pForcCtrl->Get_LcomDataCount();
    _DGN_FORC_CRC ForcMyMax, ForcMyMin, ForcVzMax, ForcVzMin, ForcMxMax, ForcMxMin;
    T_LCOM_K LcomMyMax[2], LcomMyMin[2], LcomVzMax[2], LcomVzMin[2], LcomMxMax[2], LcomMxMin[2];	// 0=I,1=J.
    for(i=0; i<iElemNum; i++)
    {
        ForcMyMax.Initialize(TRUE); ForcMyMin.Initialize(FALSE);
        ForcVzMax.Initialize(TRUE); ForcVzMin.Initialize(FALSE);
        ForcMxMax.Initialize(TRUE); ForcMxMin.Initialize(FALSE);
        LcomMyMax[0]=0; LcomMyMax[1]=0;
        LcomMyMin[0]=0; LcomMyMin[1]=0;
        LcomVzMax[0]=0; LcomVzMax[1]=0;
        LcomVzMin[0]=0; LcomVzMin[1]=0;
        LcomMxMax[0]=0; LcomMxMax[1]=0;
        LcomMxMin[0]=0; LcomMxMin[1]=0;

        auto ElemK = m_arElemK.GetAt(i);

        ADGNFORCE aForceLcom;
        if(!m_pForcCtrl->Get_LcomDataForDesign(ElemK, 0/*nFortype*/,TRUE,FALSE,aForceLcom,iLcomNumData))	continue;	// TRUE=Include Tendon Primary.
        for(j=0; j<aForceLcom.GetSize(); j++)
        {
            _DGN_FORC_CRC &ForceLcom = aForceLcom.GetAt(j);
            int iDgnLcomNo = j+1;
            CString strOrgLcomNa=_T("");
            int iOrgLcomNo=0, iSerLcomCount=0;
            int iMaxMinType=0;
            if(!m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType)) continue;
            /////////////////////////////////////
            // Only 종국시.
            if(!m_pForcCtrl->Is_LcomForStrn(iDgnLcomNo))  continue;
            BOOL bMyMax=FALSE, bMyMin=FALSE, bVzMax=FALSE, bVzMin=FALSE, bMxMax=FALSE, bMxMin=FALSE;
            if(iMaxMinType==0)				{bMyMax=TRUE; bVzMax=TRUE; bMxMax=TRUE; bMyMin=TRUE; bVzMin=TRUE; bMxMin=TRUE;}	// Not Max/Min.
            else if(iMaxMinType==5)		{bVzMax=TRUE;}	// FZ-MAX.
            else if(iMaxMinType==6)		{bVzMin=TRUE;}	// FZ-MIN.
            else if(iMaxMinType==7)		{bMxMax=TRUE;}	// MX-MAX.
            else if(iMaxMinType==8)		{bMxMin=TRUE;}	// MX-MIN.
            else if(iMaxMinType==9)		{bMyMax=TRUE;}	// MY-MAX.
            else if(iMaxMinType==10)	{bMyMin=TRUE;}	// MY-MIN.

            for(k=0; k<cPOSI_CRC; k++)
            {
                if(bMyMax && ForceLcom.dMuy[k] > ForcMyMax.dMuy[k])	{Set_Forc24(k,ForceLcom,ForcMyMax); LcomMyMax[k]=iDgnLcomNo;}	// MY-MAX.
                if(bMyMin && ForceLcom.dMuy[k] < ForcMyMin.dMuy[k])	{Set_Forc24(k,ForceLcom,ForcMyMin); LcomMyMin[k]=iDgnLcomNo;}	// MY-MIN.
                if(bVzMax && ForceLcom.dFzz[k] > ForcVzMax.dFzz[k])	{Set_Forc24(k,ForceLcom,ForcVzMax); LcomVzMax[k]=iDgnLcomNo;}	// FZ-MAX.
                if(bVzMin && ForceLcom.dFzz[k] < ForcVzMin.dFzz[k])	{Set_Forc24(k,ForceLcom,ForcVzMin); LcomVzMin[k]=iDgnLcomNo;}	// FZ-MIN.
                if(bMxMax && ForceLcom.dMux[k] > ForcMxMax.dMux[k])	{Set_Forc24(k,ForceLcom,ForcMxMax); LcomMxMax[k]=iDgnLcomNo;}	// MX-MAX.
                if(bMxMin && ForceLcom.dMux[k] < ForcMxMin.dMux[k])	{Set_Forc24(k,ForceLcom,ForcMxMin); LcomMxMin[k]=iDgnLcomNo;}	// MX-MIN.
            }
        }
        // Set Forces by Envelope (My,Vz,Mx).
        Cng_DgnForcCrc(ForcMyMax);  m_arElemMy[0].Add(ForcMyMax);
        Cng_DgnForcCrc(ForcMyMin);  m_arElemMy[1].Add(ForcMyMin);
        Cng_DgnForcCrc(ForcVzMax);  m_arElemVz[0].Add(ForcVzMax);
        Cng_DgnForcCrc(ForcVzMin);  m_arElemVz[1].Add(ForcVzMin);
        Cng_DgnForcCrc(ForcMxMax);  m_arElemMx[0].Add(ForcMxMax);
        Cng_DgnForcCrc(ForcMxMin);  m_arElemMx[1].Add(ForcMxMin);
        for(k=0; k<cPOSI_CRC; k++)
        {
            m_arElemLcomMy[0][k].Add(LcomMyMax[k]);
            m_arElemLcomMy[1][k].Add(LcomMyMin[k]);
            m_arElemLcomVz[0][k].Add(LcomVzMax[k]);
            m_arElemLcomVz[1][k].Add(LcomVzMin[k]);
            m_arElemLcomMx[0][k].Add(LcomMxMax[k]);
            m_arElemLcomMx[1][k].Add(LcomMxMin[k]);
        }
    }
    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_ForcDataByOrgLcom()
{
    BOOL bStrsOK = Get_ForcDataByStrs();
    BOOL bStrnOK = Get_ForcDataByStrn();
    if(!(bStrsOK || bStrnOK)) return FALSE;
    else                      return TRUE;
}

BOOL CPrtKSCE_USD03::Set_Forc24(int iPosiNo, _DGN_FORC_CRC& ForcCrc2, _DGN_FORC_CRC& ForcCrc4)
{
    // 정해진 위치의 정보만 저장하도록 (2->4)
    if(iPosiNo < 0 || iPosiNo > 1)	{ASSERT(0); return FALSE;}
    ForcCrc4.dFxx[iPosiNo] = ForcCrc2.dFxx[iPosiNo];
    ForcCrc4.dFyy[iPosiNo] = ForcCrc2.dFyy[iPosiNo];
    ForcCrc4.dFzz[iPosiNo] = ForcCrc2.dFzz[iPosiNo];
    ForcCrc4.dMux[iPosiNo] = ForcCrc2.dMux[iPosiNo];
    ForcCrc4.dMuy[iPosiNo] = ForcCrc2.dMuy[iPosiNo];
    ForcCrc4.dMuz[iPosiNo] = ForcCrc2.dMuz[iPosiNo];
    return TRUE;
}

BOOL CPrtKSCE_USD03::Cng_DgnForcCrc(_DGN_FORC_CRC& ForcCrc)
{
    for(int i=0; i<cPOSI_CRC; i++)
    {
        ForcCrc.dFxx[i] = Cng_ForFix(ForcCrc.dFxx[i]);
        ForcCrc.dFyy[i] = Cng_ForFix(ForcCrc.dFyy[i]);
        ForcCrc.dFzz[i] = Cng_ForFix(ForcCrc.dFzz[i]);
        ForcCrc.dMux[i] = Cng_MomFix(ForcCrc.dMux[i]);
        ForcCrc.dMuy[i] = Cng_MomFix(ForcCrc.dMuy[i]);
        ForcCrc.dMuz[i] = Cng_MomFix(ForcCrc.dMuz[i]);
    }
    return TRUE;
}

double CPrtKSCE_USD03::Cng_LenFix(double dLength, int iUp)
{
    // cm^iUp.
    double dCngLength=pow(1.0,iUp);	// 1 cm.
    //dCngLength = m_pDataCtrl->Cng_LengthFromCurrToCode(dCngLength,iUp);	// by cm.
    return (dLength*dCngLength);
}

double CPrtKSCE_USD03::Cng_LenFixM(double dLength, int iUp)
{
    // m^iUp.
    double dCngLength;
    if(m_iDgnCode==KSCE_USD03)
    {
        dCngLength=pow(1.0E-02,iUp);	// 1 cm = 1.0E-02 m.
    }
    else if(m_iDgnCode==KSCE_USD05 || m_iDgnCode==KSCE_RAIL_USD04 || m_iDgnCode==KSCE_USD10 || m_iDgnCode==KSCE_RAIL_USD11 ||
        m_iDgnCode==JTG_D62_04 || m_iDgnCode == CJJ_11_2011)
    {
        dCngLength=pow(1.0E-03,iUp);	// 1 mm = 1.0E-03 m.
    }
    else ASSERT(0);

    //dCngLength = m_pDataCtrl->Cng_LengthFromCurrToCode(dCngLength,iUp);	// by cm.
    return (dLength*dCngLength);
}

double CPrtKSCE_USD03::Cng_ForFix(double dForce)
{
    // tonf.
    double dCngForce=1.0E-03;	// 1 kgf = 1.0E-03 tonf.
    //dCngForce = m_pDataCtrl->Cng_ForceFromCurrToCode(dCngForce);
    return (dForce*dCngForce);
}

double CPrtKSCE_USD03::Cng_MomFix(double dMoment)
{
    double dCngMoment;
    if(m_iDgnCode==KSCE_USD03)
    {
        dCngMoment=1.0E-05;	// 1 kgf*cm = 1.0E-05 tonf*m.
    }
    else if(m_iDgnCode==KSCE_USD05 || m_iDgnCode==KSCE_RAIL_USD04 || m_iDgnCode==KSCE_USD10 || m_iDgnCode==KSCE_RAIL_USD11 ||
        m_iDgnCode==JTG_D62_04 ||m_iDgnCode == CJJ_11_2011)
    {
        dCngMoment=1.0E-06;	// 1 N*mm = 1.0E-06 kN*m.
    }
    else ASSERT(0);

    //dCngMoment = m_pDataCtrl->Cng_MomentFromCurrToCode(dCngMoment);
    return (dMoment*dCngMoment);
}

double CPrtKSCE_USD03::Cng_StrFix(double dStress)
{
    // kgf/cm2.
    double dCngStress=1.0;	// 1 kgf/cm2.
    //dCngStress = m_pDataCtrl->Cng_StressFromCurrToCode(dCngStress);
    return (dStress*dCngStress);
}

double CPrtKSCE_USD03::Cng_DegFix(double dRadian)
{
    // Rad -> Deg.
    double dPi = 4*atan(1.0);
    return (dRadian*(180./dPi));
}

double CPrtKSCE_USD03::Cng_CodeLenFix(double dLength, int iUp)
{
    // cm^iUp.
    double dCngLength=pow(1.0,iUp);	// 1 cm.
    return (dLength*dCngLength);
}

double CPrtKSCE_USD03::Cng_CodeLenFixM(double dLength, int iUp)
{
    // m^iUp.
    double dCngLength;
    if(m_iDgnCode==KSCE_USD03)
    {
        dCngLength=pow(1.0E-02,iUp);	// 1 cm = 1.0E-02 m.
    }
    else if(m_iDgnCode==KSCE_USD05 || m_iDgnCode==KSCE_RAIL_USD04 || m_iDgnCode==KSCE_USD10 || m_iDgnCode==KSCE_RAIL_USD11 ||
        m_iDgnCode==JTG_D62_04 ||m_iDgnCode == CJJ_11_2011)
    {
        dCngLength=pow(1.0E-03,iUp);	// 1 mm = 1.0E-03 m.
    }
    else ASSERT(0);

    return (dLength*dCngLength);
}

double CPrtKSCE_USD03::Cng_CodeForFix(double dForce)
{
    // tonf.
    double dCngForce=1.0E-03;	// 1 kgf = 1.0E-03 tonf.
    return (dForce*dCngForce);
}

double CPrtKSCE_USD03::Cng_CodeMomFix(double dMoment)
{
    // tonf*m.
    double dCngMoment;
    if(m_iDgnCode==KSCE_USD03)
    {
        dCngMoment=1.0E-05;	// 1 kgf*cm = 1.0E-05 tonf*m.
    }
    else if(m_iDgnCode==KSCE_USD05 || m_iDgnCode==KSCE_RAIL_USD04 || m_iDgnCode==KSCE_USD10 || m_iDgnCode==KSCE_RAIL_USD11 ||
        m_iDgnCode==JTG_D62_04 ||m_iDgnCode == CJJ_11_2011)
    {
        dCngMoment=1.0E-06;	// 1 N*mm = 1.0E-06 kN*m.
    }
    else ASSERT(0);

    return (dMoment*dCngMoment);
}

double CPrtKSCE_USD03::Cng_CodeStrFix(double dStress)
{
    // kgf/cm2.
    double dCngStress=1.0;	// 1 kgf/cm2.
    return (dStress*dCngStress);
}

double CPrtKSCE_USD03::Cng_CodeDegFix(double dRadian)
{
    // Rad -> Deg.
    double dPi = 4*atan(1.0);
    return (dRadian*(180./dPi));
}

BOOL CPrtKSCE_USD03::Get_ForcDetlSect(ElemPairK ElemK, BOOL bChangeM, _PRT_FORC_DETL_SECT& DetlSect)
{
    DetlSect.Initialize();
    if(!m_bPrintFlag) return FALSE;
    // Get Elem.
    T_ELEM_D ElemD;
    ElemD.Initialize();
    if (ElemK.second == EN_EL_BEAM)
    {
        if (!m_pDoc->m_pPostCtrl->GetElemPost(ElemK.first, ElemD))	return FALSE;
    }    
    T_SECT_K SectK = ElemD.elpro;

    T_SPSC_SECT SpscSect;
    /////////////////////////////////////////
    // Gross.
    // I.
    SpscSect.Initialize();
    if(!CSectUtil::GetPscSectCG(TRUE, SectK, TRUE , FALSE, SpscSect, ElemK))	return FALSE;
    DetlSect.SectBase[0].dArea[0]	= bChangeM ? Cng_LenFixM(SpscSect.dArea, 2) : Cng_LenFix(SpscSect.dArea, 2);
    DetlSect.SectBase[0].dIy[0]		= bChangeM ? Cng_LenFixM(SpscSect.dIyy , 4) : Cng_LenFix(SpscSect.dIyy , 4);
    DetlSect.SectBase[0].dIz[0]		= bChangeM ? Cng_LenFixM(SpscSect.dIzz , 4) : Cng_LenFix(SpscSect.dIzz , 4);
    DetlSect.SectBase[0].dZyt[0]	= bChangeM ? Cng_LenFixM(min(SpscSect.dSlt,SpscSect.dSrt), 3) : Cng_LenFix(min(SpscSect.dSlt,SpscSect.dSrt), 3);
    DetlSect.SectBase[0].dZyb[0]	= bChangeM ? Cng_LenFixM(min(SpscSect.dSlb,SpscSect.dSrb), 3) : Cng_LenFix(min(SpscSect.dSlb,SpscSect.dSrb), 3);
    // J.
    SpscSect.Initialize();
    if(!CSectUtil::GetPscSectCG(TRUE, SectK, FALSE, FALSE, SpscSect, ElemK))	return FALSE;
    DetlSect.SectBase[0].dArea[1]	= bChangeM ? Cng_LenFixM(SpscSect.dArea, 2) : Cng_LenFix(SpscSect.dArea, 2);
    DetlSect.SectBase[0].dIy[1]		= bChangeM ? Cng_LenFixM(SpscSect.dIyy , 4) : Cng_LenFix(SpscSect.dIyy , 4);
    DetlSect.SectBase[0].dIz[1]		= bChangeM ? Cng_LenFixM(SpscSect.dIzz , 4) : Cng_LenFix(SpscSect.dIzz , 4);
    DetlSect.SectBase[0].dZyt[1]	= bChangeM ? Cng_LenFixM(min(SpscSect.dSlt,SpscSect.dSrt), 3) : Cng_LenFix(min(SpscSect.dSlt,SpscSect.dSrt), 3);
    DetlSect.SectBase[0].dZyb[1]	= bChangeM ? Cng_LenFixM(min(SpscSect.dSlb,SpscSect.dSrb), 3) : Cng_LenFix(min(SpscSect.dSlb,SpscSect.dSrb), 3);
    /////////////////////////////////////////
    // Net.
    // I.
    SpscSect.Initialize();
    if(!CSectUtil::GetPscSectCN(TRUE, ElemK, TRUE , FALSE, SpscSect))	return FALSE;
    DetlSect.SectBase[1].dArea[0]	= bChangeM ? Cng_LenFixM(SpscSect.dArea, 2) : Cng_LenFix(SpscSect.dArea, 2);
    DetlSect.SectBase[1].dIy[0]		= bChangeM ? Cng_LenFixM(SpscSect.dIyy , 4) : Cng_LenFix(SpscSect.dIyy , 4);
    DetlSect.SectBase[1].dIz[0]		= bChangeM ? Cng_LenFixM(SpscSect.dIzz , 4) : Cng_LenFix(SpscSect.dIzz , 4);
    DetlSect.SectBase[1].dZyt[0]	= bChangeM ? Cng_LenFixM(min(SpscSect.dSlt,SpscSect.dSrt), 3) : Cng_LenFix(min(SpscSect.dSlt,SpscSect.dSrt), 3);
    DetlSect.SectBase[1].dZyb[0]	= bChangeM ? Cng_LenFixM(min(SpscSect.dSlb,SpscSect.dSrb), 3) : Cng_LenFix(min(SpscSect.dSlb,SpscSect.dSrb), 3);
    // J.
    SpscSect.Initialize();
    if(!CSectUtil::GetPscSectCN(TRUE, ElemK, FALSE, FALSE, SpscSect))	return FALSE;
    DetlSect.SectBase[1].dArea[1]	= bChangeM ? Cng_LenFixM(SpscSect.dArea, 2) : Cng_LenFix(SpscSect.dArea, 2);
    DetlSect.SectBase[1].dIy[1]		= bChangeM ? Cng_LenFixM(SpscSect.dIyy , 4) : Cng_LenFix(SpscSect.dIyy , 4);
    DetlSect.SectBase[1].dIz[1]		= bChangeM ? Cng_LenFixM(SpscSect.dIzz , 4) : Cng_LenFix(SpscSect.dIzz , 4);
    DetlSect.SectBase[1].dZyt[1]	= bChangeM ? Cng_LenFixM(min(SpscSect.dSlt,SpscSect.dSrt), 3) : Cng_LenFix(min(SpscSect.dSlt,SpscSect.dSrt), 3);
    DetlSect.SectBase[1].dZyb[1]	= bChangeM ? Cng_LenFixM(min(SpscSect.dSlb,SpscSect.dSrb), 3) : Cng_LenFix(min(SpscSect.dSlb,SpscSect.dSrb), 3);
    /////////////////////////////////////////
    // CPR.
    // I.
    SpscSect.Initialize();
    if(!CSectUtil::GetPscSectCPR(TRUE, ElemK, TRUE , FALSE, TRUE, SpscSect))	return FALSE;
    DetlSect.SectBase[2].dArea[0]	= bChangeM ? Cng_LenFixM(SpscSect.dArea, 2) : Cng_LenFix(SpscSect.dArea, 2);
    DetlSect.SectBase[2].dIy[0]		= bChangeM ? Cng_LenFixM(SpscSect.dIyy , 4) : Cng_LenFix(SpscSect.dIyy , 4);
    DetlSect.SectBase[2].dIz[0]		= bChangeM ? Cng_LenFixM(SpscSect.dIzz , 4) : Cng_LenFix(SpscSect.dIzz , 4);
    DetlSect.SectBase[2].dZyt[0]	= bChangeM ? Cng_LenFixM(min(SpscSect.dSlt,SpscSect.dSrt), 3) : Cng_LenFix(min(SpscSect.dSlt,SpscSect.dSrt), 3);
    DetlSect.SectBase[2].dZyb[0]	= bChangeM ? Cng_LenFixM(min(SpscSect.dSlb,SpscSect.dSrb), 3) : Cng_LenFix(min(SpscSect.dSlb,SpscSect.dSrb), 3);
    // J.
    SpscSect.Initialize();
    if(!CSectUtil::GetPscSectCPR(TRUE, ElemK, FALSE, FALSE, TRUE, SpscSect))	return FALSE;
    DetlSect.SectBase[2].dArea[1]	= bChangeM ? Cng_LenFixM(SpscSect.dArea, 2) : Cng_LenFix(SpscSect.dArea, 2);
    DetlSect.SectBase[2].dIy[1]		= bChangeM ? Cng_LenFixM(SpscSect.dIyy , 4) : Cng_LenFix(SpscSect.dIyy , 4);
    DetlSect.SectBase[2].dIz[1]		= bChangeM ? Cng_LenFixM(SpscSect.dIzz , 4) : Cng_LenFix(SpscSect.dIzz , 4);
    DetlSect.SectBase[2].dZyt[1]	= bChangeM ? Cng_LenFixM(min(SpscSect.dSlt,SpscSect.dSrt), 3) : Cng_LenFix(min(SpscSect.dSlt,SpscSect.dSrt), 3);
    DetlSect.SectBase[2].dZyb[1]	= bChangeM ? Cng_LenFixM(min(SpscSect.dSlb,SpscSect.dSrb), 3) : Cng_LenFix(min(SpscSect.dSlb,SpscSect.dSrb), 3);

    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_ForcDetlStag(ElemPairK ElemK, _PRT_FORC_DETL_STAG& DetlStag)
{
    DetlStag.Initialize();
    if(!m_bPrintFlag) return FALSE;

    int i=0, j=0, k=0, m=0;
    if(m_arStagK.GetSize()==0)	return FALSE;
    T_STAG_K StagK = m_arStagK.GetAt(m_arStagK.GetSize()-1);

    int iFinalStepNo=0;
    // Get Last StepNo at Current Stage.
    if(!m_pDataCtrl->Get_LastStepInCurStage(StagK, iFinalStepNo))	return FALSE;

    T_STCT_D StctD; StctD.Initialize();
    m_pDoc->m_pAttrCtrl->GetStct(StctD);

    _PRT_FORC_BASE ForcBase[7];	// 0=DL,1=EL,2=TP,3=TS,4=CSP,5=CSS,6=SUM.
    _DGN_STRS_CRC StrsMax, StrsMin;
    for(i=0; i<9; i++)
    {
        int iKind=0;
        CString strSgldName=_T("");
        if(i==0)	{strSgldName=_LS(IDS_DB_SGLD_DL);  iKind=0;}	// DL
        else if(i==2)	{strSgldName=_LS(IDS_DB_SGLD_TP);  iKind=2;}	// TP
        else if(i==3)	{strSgldName=_LS(IDS_DB_SGLD_TS);  iKind=3;}	// TS
        else if(i==4)	{strSgldName=_LS(IDS_DB_SGLD_CP);  iKind=4;}	// CP
        else if(i==5)	{strSgldName=_LS(IDS_DB_SGLD_CS);  iKind=5;}	// CS
        else if(i==6)	{strSgldName=_LS(IDS_DB_SGLD_SP);  iKind=4;}	// SP
        else if(i==7)	{strSgldName=_LS(IDS_DB_SGLD_SS);  iKind=5;}	// SS
        else if(i==8)	{strSgldName=_LS(IDS_DB_SGLD_SUM); iKind=6;}	// SUM
        //	else if(i==9)	strSgldName=_LS(IDS_DB_SGLD_LL);	// LL
        else	continue;

        ForcBase[iKind].Initialize();
        // Change by ZINU.('05.01.04). Get Stress if Stage. (Calculate -> Get)
        T_SGLD_K SgldK = m_pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);
        T_LCOM_D LcasD;
        LcasD.Initialize();
        if(!m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_STAGE, SgldK, LcasD)) continue;    
        if(iFinalStepNo > 0)	m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcasD,iFinalStepNo,2);	// 0=Nonlinear, 1=Pushover, 2=Construction.
        else					m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcasD);
        // Get Force Data by Load Case.
        T_STRB_D StrMaxData; StrMaxData.Initialize();
        T_STRB_D StrMinData; StrMinData.Initialize();
        T_STRB_D StrAbsData; StrAbsData.Initialize();
        if(!m_pDoc->m_pPostCtrl->GetStrbNew(ElemK.first, &StrMaxData, &StrMinData, &StrAbsData, TRUE, NULL, ElemK.second))
        {
            ForcBase[iKind].bData = FALSE;
            continue;
        }

        for(j=0; j<cPOSI_CRC; j++)
        {
            int iPart	 = (j==0 ? 0 : 3);
            int iKind1 = (j==0 ? 0 : 6);
            int iKind2 = (j==0 ? 0 : 1);
            // Forces.
            ForcBase[iKind].dFxx[j] = StrMaxData.dblForce[iPart][0+iKind1];
            ForcBase[iKind].dMuy[j] = StrMaxData.dblForce[iPart][4+iKind1];
            ForcBase[iKind].dMuz[j] = StrMaxData.dblForce[iPart][5+iKind1];
            // Stresses.
            ForcBase[iKind].dStl[j] = StrMaxData.dblStress[iPart][16+iKind2];
            ForcBase[iKind].dStr[j] = StrMaxData.dblStress[iPart][18+iKind2];
            ForcBase[iKind].dSbr[j] = StrMaxData.dblStress[iPart][20+iKind2];
            ForcBase[iKind].dSbl[j] = StrMaxData.dblStress[iPart][22+iKind2];
        }
    }

    int nErIdx=1;
    ForcBase[nErIdx].Initialize();
    ForcBase[nErIdx].bData = FALSE;
    for (INT_PTR Er=0; Er<StctD.aErection.GetSize(); ++Er)
    {
        if(StctD.aErection[Er].nNumStctCase > 0)
        {
            T_SGLD_K SgldK = m_pDoc->m_pAttrCtrl->GetSgldKey(StctD.aErection[Er].strErection);
            T_LCOM_D LcomT;
            LcomT.Initialize();
            if(!m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_STAGE, SgldK, LcomT)) continue;    
            if(iFinalStepNo > 0)	m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomT,iFinalStepNo,2);	// 0=Nonlinear, 1=Pushover, 2=Construction.
            else					m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomT);
            // Get Force Data by Load Case.
            T_STRB_D StrMaxData; StrMaxData.Initialize();
            if(m_pDoc->m_pPostCtrl->GetStrbNew(ElemK.first, &StrMaxData, NULL, NULL, TRUE, NULL, ElemK.second))
            {
                ForcBase[nErIdx].bData = TRUE;
                for(j=0; j<cPOSI_CRC; j++)
                {
                    int iPart	 = (j==0 ? 0 : 3);
                    int iKind1 = (j==0 ? 0 : 6);
                    int iKind2 = (j==0 ? 0 : 1);
                    // Forces.
                    ForcBase[nErIdx].dFxx[j] += StrMaxData.dblForce[iPart][0+iKind1];
                    ForcBase[nErIdx].dMuy[j] += StrMaxData.dblForce[iPart][4+iKind1];
                    ForcBase[nErIdx].dMuz[j] += StrMaxData.dblForce[iPart][5+iKind1];
                    // Stresses.
                    ForcBase[nErIdx].dStl[j] += StrMaxData.dblStress[iPart][16+iKind2];
                    ForcBase[nErIdx].dStr[j] += StrMaxData.dblStress[iPart][18+iKind2];
                    ForcBase[nErIdx].dSbr[j] += StrMaxData.dblStress[iPart][20+iKind2];
                    ForcBase[nErIdx].dSbl[j] += StrMaxData.dblStress[iPart][22+iKind2];
                }                
            }
        }
    }


    for(j=0; j<7; j++)	// iKind.
    {
        _DGN_FORC_CRC ForcCrc;
        //_DGN_STRS_ADD StrsAdd;
        ForcCrc.Initialize();
        for(k=0; k<cPOSI_CRC; k++)
        {
            ForcBase[j].dFxx[k] = Cng_ForFix(ForcBase[j].dFxx[k]);
            ForcBase[j].dMuy[k] = Cng_MomFix(ForcBase[j].dMuy[k]);
            ForcBase[j].dMuz[k] = Cng_MomFix(ForcBase[j].dMuz[k]);
            ForcBase[j].dStl[k] = Cng_StrFix(ForcBase[j].dStl[k]);
            ForcBase[j].dStr[k] = Cng_StrFix(ForcBase[j].dStr[k]);
            ForcBase[j].dSbl[k] = Cng_StrFix(ForcBase[j].dSbl[k]);
            ForcBase[j].dSbr[k] = Cng_StrFix(ForcBase[j].dSbr[k]);
        }
        if(j==0)	DetlStag.ForcDL	 = ForcBase[j];
        else if(j==1)	DetlStag.ForcEL  = ForcBase[j];
        else if(j==2)	DetlStag.ForcTP  = ForcBase[j];
        else if(j==3)	DetlStag.ForcTS  = ForcBase[j];
        else if(j==4)	DetlStag.ForcCSP = ForcBase[j];
        else if(j==5)	DetlStag.ForcCSS = ForcBase[j];
        else if(j==6)	DetlStag.ForcSum = ForcBase[j];
        else	ASSERT(0);
    }
    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_ForcDetlLcas(ElemPairK ElemK, _PRT_FORC_DETL_LCAS& DetlLcas)
{
    DetlLcas.Initialize();
    if(!m_bPrintFlag) return FALSE;

    int i=0, j=0, k=0;

    // Change by ZINU.('05.01.04). For Beam Section Temperature.
    _DGN_STRS_ADD StrsAdd; StrsAdd.Initialize();
    // Static Load.
    CArray<T_STLD_K,T_STLD_K> StldKList;
    StldKList.RemoveAll();
    m_pDoc->m_pPostCtrl->GetStldKeyList(StldKList);
    int nForType = 0; // 단면 부재력 받아옴.
    BOOL bCalcStss = FALSE;
    for(i=0; i<StldKList.GetSize(); i++)
    {
        T_STLD_K StldK = StldKList.GetAt(i);
        _DGN_FORC_CRC ForceLcase;  ForceLcase.Initialize();
        _DGN_STRS_ADD StressAdd;   StressAdd.Initialize();
        _DGN_STRS_CRC StressLcase; StressLcase.Initialize();
        _DGN_SHRS_ADD StssLcase;   StssLcase.Initialize();

        _DGN_FORC_CRC ForcePart1;
        _DGN_STRS_CRC StressPart1;
        _DGN_FORC_CRC ForcePart2;
        _DGN_STRS_CRC StressPart2;
        if(!m_pForcCtrl->Get_Stld(ElemK, StldK, nForType, bCalcStss, ForceLcase, StressAdd, StressLcase, StssLcase,
            ForcePart1, StressPart1, ForcePart2, StressPart2)) continue;
        _PRT_FORC_BASE ForcBase;
        ForcBase.Initialize();
        for(j=0; j<cPOSI_CRC; j++)	// I,J.
        {
            ForcBase.dFxx[j] = ForceLcase.dFxx[j];
            ForcBase.dMuy[j] = ForceLcase.dMuy[j];
            ForcBase.dMuz[j] = ForceLcase.dMuz[j];
        }
        if(CDBLib::IsStldDL(ForceLcase.strType))	DetlLcas.ForcDL += ForcBase;	// Dead.
        else
        {
            DetlLcas.ForcADDmax += ForcBase;
            DetlLcas.ForcADDmin += ForcBase;
        }
        StrsAdd += StressAdd;
    }

    // Moving Load.
    CArray<UINT, UINT> TotalMvldKList; TotalMvldKList.RemoveAll();

    CArray<T_MVLD_K, T_MVLD_K> MvldKList;
    MvldKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetMvldKeyList(MvldKList);
    TotalMvldKList.Append(MvldKList);

    CArray<T_MVLDbs_K, T_MVLDbs_K> MvldbsKList;
    MvldbsKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetMvldbsKeyList(MvldbsKList);
    TotalMvldKList.Append(MvldbsKList);

	CArray<T_MVLDfr_K, T_MVLDfr_K> MvldfrKList;
	MvldfrKList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetMvldfrKeyList(MvldfrKList);
	TotalMvldKList.Append(MvldfrKList);

    CArray<T_MVLDch_K, T_MVLDch_K> MvldchKList;
    MvldchKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetMvldchKeyList(MvldchKList);
    TotalMvldKList.Append(MvldchKList);

    CArray<T_MVLDid_K, T_MVLDid_K> MvldidKList;
    MvldidKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetMvldidKeyList(MvldidKList);
    TotalMvldKList.Append(MvldidKList);

    CArray<T_MVLDjp_K, T_MVLDjp_K> MvldjpKList;
    MvldjpKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetMvldjpKeyList(MvldjpKList);
    TotalMvldKList.Append(MvldjpKList);

    for(i=0; i<TotalMvldKList.GetSize(); i++)
    {
        UINT MvldK = TotalMvldKList.GetAt(i);
        CString strLcomType = _T("");
        ADGNFORCE   aForceLcase;		
        ADGNSTRESS  aStressLcase;
        ADGNSHRSADD aStssLcase;

        ADGNFORCE   aForceP1;
        ADGNSTRESS  aStressP1;
        ADGNFORCE   aForceP2;
        ADGNSTRESS  aStressP2;
        if(!m_pForcCtrl->Get_Mvld(ElemK, MvldK, nForType, bCalcStss, strLcomType, aForceLcase, aStressLcase, aStssLcase,
            aForceP1, aStressP1, aForceP2, aStressP2)) continue;
        _DGN_FORC_CRC &ForceFxMax = aForceLcase.GetAt(0);	// FX-MAX.
        _DGN_FORC_CRC &ForceFxMin = aForceLcase.GetAt(1);	// FX-MIN.
        _DGN_FORC_CRC &ForceMyMax = aForceLcase.GetAt(8);	// MY-MAX.
        _DGN_FORC_CRC &ForceMyMin = aForceLcase.GetAt(9);	// MY-MIN.
        _DGN_FORC_CRC &ForceMzMax = aForceLcase.GetAt(10);	// MZ-MAX.
        _DGN_FORC_CRC &ForceMzMin = aForceLcase.GetAt(11);	// MZ-MIN.
        for(j=0; j<cPOSI_CRC; j++)	// I,J.
        {
            // Max.
            DetlLcas.ForcLLmax.dFxx[j] += ForceFxMax.dFxx[j];
            DetlLcas.ForcLLmax.dMuy[j] += ForceMyMax.dMuy[j];
            DetlLcas.ForcLLmax.dMuz[j] += ForceMzMax.dMuz[j];
            // Min.
            DetlLcas.ForcLLmin.dFxx[j] += ForceFxMin.dFxx[j];
            DetlLcas.ForcLLmin.dMuy[j] += ForceMyMin.dMuy[j];
            DetlLcas.ForcLLmin.dMuz[j] += ForceMzMin.dMuz[j];
        }
    }

    // Settlement Load.
    CArray<T_SMLC_K, T_SMLC_K> SmlcKList;
    SmlcKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetSmlcKeyList(SmlcKList);

    for(i=0; i<SmlcKList.GetSize(); i++)
    {
        T_SMLC_K SmlcK = SmlcKList.GetAt(i);
        ADGNFORCE   aForceLcase;
        ADGNSTRESS  aStressLcase;
        ADGNSHRSADD aStssLcase;

        ADGNFORCE   aForceP1;
        ADGNSTRESS  aStressP1;
        ADGNFORCE   aForceP2;
        ADGNSTRESS  aStressP2;

        CString strLcomType = _T("");
        if(!m_pForcCtrl->Get_Smlc(ElemK, SmlcK, nForType, bCalcStss, strLcomType, aForceLcase, aStressLcase, aStssLcase,
            aForceP1, aStressP1, aForceP2, aStressP2)) continue;
        _DGN_FORC_CRC &ForceFxMax = aForceLcase.GetAt(0);	// FX-MAX.
        _DGN_FORC_CRC &ForceFxMin = aForceLcase.GetAt(1);	// FX-MIN.
        _DGN_FORC_CRC &ForceMyMax = aForceLcase.GetAt(8);	// MY-MAX.
        _DGN_FORC_CRC &ForceMyMin = aForceLcase.GetAt(9);	// MY-MIN.
        _DGN_FORC_CRC &ForceMzMax = aForceLcase.GetAt(10);	// MZ-MAX.
        _DGN_FORC_CRC &ForceMzMin = aForceLcase.GetAt(11);	// MZ-MIN.
        for(j=0; j<cPOSI_CRC; j++)	// I,J.
        {
            // Max.
            DetlLcas.ForcADDmax.dFxx[j] += ForceFxMax.dFxx[j];
            DetlLcas.ForcADDmax.dMuy[j] += ForceMyMax.dMuy[j];
            DetlLcas.ForcADDmax.dMuz[j] += ForceMzMax.dMuz[j];
            // Min.
            DetlLcas.ForcADDmin.dFxx[j] += ForceFxMin.dFxx[j];
            DetlLcas.ForcADDmin.dMuy[j] += ForceMyMin.dMuy[j];
            DetlLcas.ForcADDmin.dMuz[j] += ForceMzMin.dMuz[j];
        }
    }

    // Response Spectrum.
    CArray<T_SPLC_K, T_SPLC_K> SplcKList;
    SplcKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetSplcKeyList(SplcKList);
    for(i=0; i<SplcKList.GetSize(); i++)
    {
        T_SPLC_K SplcK = SplcKList.GetAt(i);
        _DGN_FORC_CRC ForceLcase;
        _DGN_STRS_CRC StressLcase;
        _DGN_SHRS_ADD StssLcase;

        _DGN_FORC_CRC ForcePart1;
        _DGN_STRS_CRC StressPart1;
        _DGN_FORC_CRC ForcePart2;
        _DGN_STRS_CRC StressPart2;
        if(!m_pForcCtrl->Get_Splc(ElemK, SplcK, nForType, bCalcStss, ForceLcase, StressLcase, StssLcase,
            ForcePart1, StressPart1, ForcePart2, StressPart2)) continue;
        for(j=0; j<cPOSI_CRC; j++)	// I,J.
        {
            // Max.
            DetlLcas.ForcADDmax.dFxx[j] += ForceLcase.dFxx[j];
            DetlLcas.ForcADDmax.dMuy[j] += ForceLcase.dMuy[j];
            DetlLcas.ForcADDmax.dMuz[j] += ForceLcase.dMuz[j];
            // Min.
            DetlLcas.ForcADDmin.dFxx[j] += ForceLcase.dFxx[j];
            DetlLcas.ForcADDmin.dMuy[j] += ForceLcase.dMuy[j];
            DetlLcas.ForcADDmin.dMuz[j] += ForceLcase.dMuz[j];
        }
    }

    // Eccentric Response Spectrum.
    CArray<T_ESPC_K, T_ESPC_K> EspcKList;
    EspcKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetEspcKeyList(EspcKList);
    for(i=0; i<EspcKList.GetSize(); i++)
    {
        T_ESPC_K EspcK = EspcKList.GetAt(i);
        _DGN_FORC_CRC ForceLcase; 
        _DGN_STRS_CRC StressLcase;
        _DGN_SHRS_ADD StssLcase;  

        _DGN_FORC_CRC ForcePart1;
        _DGN_STRS_CRC StressPart1;
        _DGN_FORC_CRC ForcePart2;
        _DGN_STRS_CRC StressPart2;
        if(!m_pForcCtrl->Get_Espc(ElemK, EspcK, nForType, bCalcStss, ForceLcase, StressLcase, StssLcase,
            ForcePart1, StressPart1, ForcePart2, StressPart2)) continue;
        for(j=0; j<cPOSI_CRC; j++)	// I,J.
        {
            // Max.
            DetlLcas.ForcADDmax.dFxx[j] += ForceLcase.dFxx[j];
            DetlLcas.ForcADDmax.dMuy[j] += ForceLcase.dMuy[j];
            DetlLcas.ForcADDmax.dMuz[j] += ForceLcase.dMuz[j];
            // Min.
            DetlLcas.ForcADDmin.dFxx[j] += ForceLcase.dFxx[j];
            DetlLcas.ForcADDmin.dMuy[j] += ForceLcase.dMuy[j];
            DetlLcas.ForcADDmin.dMuz[j] += ForceLcase.dMuz[j];
        }
    }

    // Time History.
    CArray<T_THIS_K, T_THIS_K> ThisKList;
    ThisKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetThisKeyList(ThisKList);

    for(i=0; i<ThisKList.GetSize(); i++)
    {
        T_THIS_K ThisK = ThisKList.GetAt(i);
        _DGN_FORC_CRC ForceLcase;
        _DGN_STRS_CRC StressLcase;
        _DGN_SHRS_ADD StssLcase;

        _DGN_FORC_CRC ForcePart1;
        _DGN_STRS_CRC StressPart1;
        _DGN_FORC_CRC ForcePart2;
        _DGN_STRS_CRC StressPart2;
        if(!m_pForcCtrl->Get_This(ElemK, ThisK, nForType, bCalcStss, ForceLcase, StressLcase, StssLcase,
            ForcePart1, StressPart1, ForcePart2, StressPart2)) continue;
        for(j=0; j<cPOSI_CRC; j++)	// I,J.
        {
            // Max.
            DetlLcas.ForcADDmax.dFxx[j] += ForceLcase.dFxx[j];
            DetlLcas.ForcADDmax.dMuy[j] += ForceLcase.dMuy[j];
            DetlLcas.ForcADDmax.dMuz[j] += ForceLcase.dMuz[j];
            // Min.
            DetlLcas.ForcADDmin.dFxx[j] += ForceLcase.dFxx[j];
            DetlLcas.ForcADDmin.dMuy[j] += ForceLcase.dMuy[j];
            DetlLcas.ForcADDmin.dMuz[j] += ForceLcase.dMuz[j];
        }
    }

    // Grid Model.
    CArray<T_GILC_K, T_GILC_K> GilcKList;
    GilcKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetGilcKeyList(GilcKList);

    for(i=0; i<GilcKList.GetSize(); i++)
    {
        T_GILC_K GilcK = GilcKList.GetAt(i);
        T_GILD_D GildD; GildD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetGild(GilcK, GildD)) continue;

        CString strLcomType = _T("");
        ADGNFORCE aForceLcase;
        ADGNSTRESS aStressLcase;
        ADGNSHRSADD aStssLcase;
        ADGNFORCE   aForceP1;
        ADGNSTRESS  aStressP1;
        ADGNFORCE   aForceP2;
        ADGNSTRESS  aStressP2;
        if(!m_pForcCtrl->Get_Gilc(ElemK, GilcK, nForType, bCalcStss, GildD.nType, strLcomType, aForceLcase, aStressLcase, aStssLcase,
            aForceP1, aStressP1, aForceP2, aStressP2)) continue;
        _DGN_FORC_CRC &ForceFxMax = aForceLcase.GetAt(0);	// FX-MAX.
        _DGN_FORC_CRC &ForceFxMin = aForceLcase.GetAt(1);	// FX-MIN.
        _DGN_FORC_CRC &ForceMyMax = aForceLcase.GetAt(8);	// MY-MAX.
        _DGN_FORC_CRC &ForceMyMin = aForceLcase.GetAt(9);	// MY-MIN.
        _DGN_FORC_CRC &ForceMzMax = aForceLcase.GetAt(10);	// MZ-MAX.
        _DGN_FORC_CRC &ForceMzMin = aForceLcase.GetAt(11);	// MZ-MIN.
        for(j=0; j<cPOSI_CRC; j++)	// I,J.
        {
            // Max.
            DetlLcas.ForcLLmax.dFxx[j] += ForceFxMax.dFxx[j];
            DetlLcas.ForcLLmax.dMuy[j] += ForceMyMax.dMuy[j];
            DetlLcas.ForcLLmax.dMuz[j] += ForceMzMax.dMuz[j];
            // Min.
            DetlLcas.ForcLLmin.dFxx[j] += ForceFxMin.dFxx[j];
            DetlLcas.ForcLLmin.dMuy[j] += ForceMyMin.dMuy[j];
            DetlLcas.ForcLLmin.dMuz[j] += ForceMzMin.dMuz[j];
        }
    }

    _DGN_STRS_CRC StrsMax, StrsMin;
    for(i=0; i<5; i++)	// iKind.
    {
        _PRT_FORC_BASE ForcBase;
        if(i==0)	ForcBase = DetlLcas.ForcDL;
        else if(i==1)	ForcBase = DetlLcas.ForcLLmax;
        else if(i==2)	ForcBase = DetlLcas.ForcLLmin;
        else if(i==3)	ForcBase = DetlLcas.ForcADDmax;
        else if(i==4)	ForcBase = DetlLcas.ForcADDmin;
        else	ASSERT(0);

        _DGN_FORC_CRC ForcCrc;
        ForcCrc.Initialize();
        for(j=0; j<cPOSI_CRC; j++)
        {
            ForcCrc.dFxx[j] = ForcBase.dFxx[j];
            ForcCrc.dMuy[j] = ForcBase.dMuy[j];
            ForcCrc.dMuz[j] = ForcBase.dMuz[j];
        }
        StrsMax.Initialize();
        StrsMin.Initialize();
        if(!m_pForcCtrl->Get_StrsByForc(ElemK, 0, ForcCrc, StrsAdd, StrsMax, StrsMin))	continue;
        for(j=0; j<cPOSI_CRC; j++)
        {
            ForcBase.dFxx[j] = Cng_ForFix(ForcCrc.dFxx[j]);
            ForcBase.dMuy[j] = Cng_MomFix(ForcCrc.dMuy[j]);
            ForcBase.dMuz[j] = Cng_MomFix(ForcCrc.dMuz[j]);
            ForcBase.dStl[j] = Cng_StrFix(StrsMax.dStl[j]);
            ForcBase.dStr[j] = Cng_StrFix(StrsMax.dStr[j]);
            ForcBase.dSbl[j] = Cng_StrFix(StrsMax.dSbl[j]);
            ForcBase.dSbr[j] = Cng_StrFix(StrsMax.dSbr[j]);
        }
        if(i==0)	DetlLcas.ForcDL		  = ForcBase;
        else if(i==1)	DetlLcas.ForcLLmax  = ForcBase;
        else if(i==2)	DetlLcas.ForcLLmin  = ForcBase;
        else if(i==3)	DetlLcas.ForcADDmax = ForcBase;
        else if(i==4)	DetlLcas.ForcADDmin = ForcBase;
        else	ASSERT(0);
    }
    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_ForcDetlLcom(_PRT_FORC_DETL_STAG& DetlStag, _PRT_FORC_DETL_LCAS& DetlLcas, _PRT_FORC_DETL_LCOM& LcomPrev, _PRT_FORC_DETL_LCOM& LcomNext)
{
    /////////////////////////////////////////
    // LcomNext.
    LcomNext.Initialize();
    // + SUM(Stage).
    LcomNext.ForcD			+= DetlStag.ForcSum;
    LcomNext.ForcDLmax	+= DetlStag.ForcSum;
    LcomNext.ForcDLmin	+= DetlStag.ForcSum;
    LcomNext.ForcDLAmax	+= DetlStag.ForcSum;
    LcomNext.ForcDLAmin	+= DetlStag.ForcSum;
    // + DL.
    LcomNext.ForcD			+= DetlLcas.ForcDL;
    LcomNext.ForcDLmax	+= DetlLcas.ForcDL;
    LcomNext.ForcDLmin	+= DetlLcas.ForcDL;
    LcomNext.ForcDLAmax	+= DetlLcas.ForcDL;
    LcomNext.ForcDLAmin	+= DetlLcas.ForcDL;
    // + LLmax.
    LcomNext.ForcDLmax	+= DetlLcas.ForcLLmax;
    LcomNext.ForcDLAmax	+= DetlLcas.ForcLLmax;
    // + LLmin.
    LcomNext.ForcDLmin	+= DetlLcas.ForcLLmin;
    LcomNext.ForcDLAmin	+= DetlLcas.ForcLLmin;
    // + ADDmax.
    LcomNext.ForcDLAmax	+= DetlLcas.ForcADDmax;
    // + ADDmin.
    LcomNext.ForcDLAmin	+= DetlLcas.ForcADDmin;
    /////////////////////////////////////////
    // LcomPrev.
    LcomPrev.Initialize();
    LcomPrev.ForcD			= LcomNext.ForcD;
    LcomPrev.ForcDLmax	= LcomNext.ForcDLmax;
    LcomPrev.ForcDLmin	= LcomNext.ForcDLmin;
    LcomPrev.ForcDLAmax	= LcomNext.ForcDLAmax;
    LcomPrev.ForcDLAmin	= LcomNext.ForcDLAmin;
    // - (Creep+Shrinkage) Primary.
    LcomPrev.ForcD			-= DetlStag.ForcCSP;
    LcomPrev.ForcDLmax	-= DetlStag.ForcCSP;
    LcomPrev.ForcDLmin	-= DetlStag.ForcCSP;
    LcomPrev.ForcDLAmax	-= DetlStag.ForcCSP;
    LcomPrev.ForcDLAmin	-= DetlStag.ForcCSP;
    // - (Creep+Shrinkage) Secondary.
    LcomPrev.ForcD			-= DetlStag.ForcCSS;
    LcomPrev.ForcDLmax	-= DetlStag.ForcCSS;
    LcomPrev.ForcDLmin	-= DetlStag.ForcCSS;
    LcomPrev.ForcDLAmax	-= DetlStag.ForcCSS;
    LcomPrev.ForcDLAmin	-= DetlStag.ForcCSS;

    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_ForcGrphStag(T_STAG_K StagK, CString& strText, double& dExtraRat, CArray<_PRT_FORC_GRPH_ELEM,_PRT_FORC_GRPH_ELEM>& arGrphElem)
{
    strText=_T("");
    dExtraRat=1.0;
    arGrphElem.RemoveAll();
    if(!m_bPrintFlag) return FALSE;

    T_STAG_D StagD;
    StagD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetStag(StagK,StagD))	return FALSE;
    strText = StagD.StageName;

    _PRT_FORC_GRPH_GRUP GrphGrup;
    GrphGrup.Initialize();
    if(!m_amForcStag.Lookup(StagK,GrphGrup))	return FALSE;
    dExtraRat = GrphGrup.dExtraRat;
    arGrphElem.Copy(GrphGrup.arGrphElemMax);	// Only Used Max.

    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_ForcGrphLcom(T_LCOM_K LcomK, CString& strLcomName, CString& strText, double& dExtraRat, CArray<_PRT_FORC_GRPH_ELEM,_PRT_FORC_GRPH_ELEM>& arGrphElemMax, CArray<_PRT_FORC_GRPH_ELEM,_PRT_FORC_GRPH_ELEM>& arGrphElemMin)
{
    // LcomK is OrgLcomK (Not DgnLcomK).
    strText=_T("");
    dExtraRat=1.0;
    arGrphElemMax.RemoveAll();
    arGrphElemMin.RemoveAll();
    if(!m_bPrintFlag) return FALSE;

    T_LCOM_D LcomD;
    LcomD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, LcomK, LcomD))	return FALSE;
    strText = LcomD.Description;
    strLcomName = LcomD.LoadCombName;

    _PRT_FORC_GRPH_GRUP GrphGrup;
    GrphGrup.Initialize();
    if(!m_amForcOrgLcom.Lookup(LcomK,GrphGrup))	return FALSE;
    dExtraRat = GrphGrup.dExtraRat;
    arGrphElemMax.Copy(GrphGrup.arGrphElemMax);
    arGrphElemMin.Copy(GrphGrup.arGrphElemMin);

    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_ForcGrphTotl(CArray<_PRT_FORC_GRPH_TOTL,_PRT_FORC_GRPH_TOTL>& arGrphTotl)
{
    arGrphTotl.RemoveAll();
    if(!m_bPrintFlag) return FALSE;

    int i=0, j=0, k=0, l=0;
    _PRT_FORC_GRPH_TOTL GrphTotl;
    _PRT_FORC_GRPH_GRUP GrphGrup;
    // By Stage.

    for(i=0; i<m_arStagK.GetSize(); i++)
    {
        GrphTotl.Initialize();
        T_STAG_K StagK = m_arStagK.GetAt(i);
        T_STAG_D StagD;
        StagD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetStag(StagK,StagD))	continue;
        GrphTotl.strTitle = StagD.StageName;

        GrphGrup.Initialize();
        if(!m_amForcStag.Lookup(StagK,GrphGrup))	continue;

        double dRtcMax = -1./m_dZero;	double dRttMax = -1./m_dZero;
        double dRbcMax = -1./m_dZero;	double dRbtMax = -1./m_dZero;

        double dAlcMin  = 1./m_dZero;
        double dAltTMax = -1./m_dZero;
        double dAltBMax = -1./m_dZero;

        BOOL bRtcMax = FALSE;
        BOOL bRttMax = FALSE;
        BOOL bRbcMax = FALSE;
        BOOL bRbtMax = FALSE;
        for(j=0; j<GrphGrup.arGrphElemMax.GetSize(); j++)
        {
            _PRT_FORC_GRPH_ELEM GrphElem = GrphGrup.arGrphElemMax.GetAt(j);
            for(k=0; k<cPOSI_CRC; k++)
            {
                ///////////////////////////////////
                // Current.
                double dStc	= max(0.,max(GrphElem.dStl[k],GrphElem.dStr[k]));
                double dStt	= min(0.,min(GrphElem.dStl[k],GrphElem.dStr[k]));
                double dSbc = max(0.,max(GrphElem.dSbl[k],GrphElem.dSbr[k]));
                double dSbt = min(0.,min(GrphElem.dSbl[k],GrphElem.dSbr[k]));
                double dAlc = GrphElem.dAlc[k];
                double dAltT = GrphElem.dAlt[k][0];
                double dAltB = GrphElem.dAlt[k][1];
                // Alw.
                dAlcMin  = (dAlc<dAlcMin ? dAlc : dAlcMin);
                dAltTMax = (dAltT>dAltTMax ? dAltT : dAltTMax);
                dAltBMax = (dAltB>dAltBMax ? dAltB : dAltBMax);
                // Ratio.
                double dRtcCur = (dAlc ==0.0 ? 0.0 : fabs(dStc/dAlc));	 // Top Comp.
                double dRttCur = (dAltT==0.0 ? 0.0 : fabs(dStt/dAltT)); // Top Tens.
                double dRbcCur = (dAlc ==0.0 ? 0.0 : fabs(dSbc/dAlc));	 // Bot Comp.
                double dRbtCur = (dAltB==0.0 ? 0.0 : fabs(dSbt/dAltB)); // Bot Tens.
                ///////////////////////////////////
                if(dRtcCur > dRtcMax)
                {          
                    bRtcMax = TRUE;
                    dRtcMax = dRtcCur; 	// Top Comp.
                    GrphTotl.dStc=dStc; GrphTotl.dAltc=dAlc;
                }
                if(dRttCur > dRttMax)
                {
                    bRttMax = TRUE;
                    dRttMax = dRttCur;	// Top Tens.
                    GrphTotl.dStt=dStt; GrphTotl.dAltt=dAltT;
                }
                if(dRbcCur > dRbcMax)
                {
                    bRbcMax = TRUE;
                    dRbcMax = dRbcCur;	// Bot Comp.
                    GrphTotl.dSbc=dSbc; GrphTotl.dAlbc=dAlc;
                }
                if(dRbtCur > dRbtMax)	
                {
                    bRbtMax = TRUE;
                    dRbtMax = dRbtCur;	// Bot Tens.
                    GrphTotl.dSbt=dSbt; GrphTotl.dAlbt=dAltB;
                }
            }
        }
        if(!bRtcMax) GrphTotl.dAltc = dAlcMin;
        if(!bRttMax) GrphTotl.dAltt = dAltTMax;
        if(!bRbcMax) GrphTotl.dAlbc = dAlcMin;
        if(!bRbtMax) GrphTotl.dAlbt = dAltBMax;

        arGrphTotl.Add(GrphTotl);
    }
    // By Lcom.
    for(i=0; i<m_arLcomK.GetSize(); i++)
    {
        GrphTotl.Initialize();
        T_LCOM_K LcomK = m_arLcomK.GetAt(i);
        T_LCOM_D LcomD;
        LcomD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, LcomK, LcomD))	continue;
        GrphTotl.strTitle = LcomD.LoadCombName;

        GrphGrup.Initialize();
        if(!m_amForcOrgLcom.Lookup(LcomK,GrphGrup))	continue;

        double dRtcMax = -1./m_dZero;	double dRttMax = -1./m_dZero;
        double dRbcMax = -1./m_dZero;	double dRbtMax = -1./m_dZero;

        double dAlcMin  = 1./m_dZero;
        double dAltTMax = -1./m_dZero;
        double dAltBMax = -1./m_dZero;

        BOOL bRtcMax = FALSE;
        BOOL bRttMax = FALSE;
        BOOL bRbcMax = FALSE;
        BOOL bRbtMax = FALSE;

        int iMaxCount;
        BOOL bMaxMinType = (GrphGrup.arGrphElemMin.GetSize()>0 ? TRUE : FALSE);
        iMaxCount = bMaxMinType ? 2 : 1;
        for(l=0; l<iMaxCount; l++)
        {
            int iElemNumber = (l==0 ? GrphGrup.arGrphElemMax.GetSize() :  GrphGrup.arGrphElemMin.GetSize() );

            for(j=0; j<iElemNumber; j++)
            {
                _PRT_FORC_GRPH_ELEM GrphElem = (l==0 ? GrphGrup.arGrphElemMax.GetAt(j) : GrphGrup.arGrphElemMin.GetAt(j));
                for(k=0; k<cPOSI_CRC; k++)
                {
                    ///////////////////////////////////
                    // Current.
                    double dStc	= max(0.,max(GrphElem.dStl[k],GrphElem.dStr[k]));
                    double dStt	= min(0.,min(GrphElem.dStl[k],GrphElem.dStr[k]));
                    double dSbc = max(0.,max(GrphElem.dSbl[k],GrphElem.dSbr[k]));
                    double dSbt = min(0.,min(GrphElem.dSbl[k],GrphElem.dSbr[k]));
                    double dAlc = GrphElem.dAlc[k];
                    double dAltT = GrphElem.dAlt[k][0];
                    double dAltB = GrphElem.dAlt[k][1];
                    // Alw.
                    dAlcMin  = (dAlc<dAlcMin ? dAlc : dAlcMin);
                    dAltTMax = (dAltT>dAltTMax ? dAltT : dAltTMax);
                    dAltBMax = (dAltB>dAltBMax ? dAltB : dAltBMax);
                    // Ratio.
                    double dRtcCur = (dAlc ==0.0 ? 0.0 : fabs(dStc/dAlc));	 // Top Comp.
                    double dRttCur = (dAltT==0.0 ? 0.0 : fabs(dStt/dAltT)); // Top Tens.
                    double dRbcCur = (dAlc ==0.0 ? 0.0 : fabs(dSbc/dAlc));	 // Bot Comp.
                    double dRbtCur = (dAltB==0.0 ? 0.0 : fabs(dSbt/dAltB)); // Bot Tens.
                    ///////////////////////////////////
                    if(dRtcCur > dRtcMax)
                    {          
                        bRtcMax = TRUE;
                        dRtcMax = dRtcCur;	// Top Comp.
                        GrphTotl.dStc=dStc; GrphTotl.dAltc=dAlc;
                    }
                    if(dRttCur > dRttMax)
                    {
                        bRttMax = TRUE;
                        dRttMax = dRttCur;	// Top Tens.
                        GrphTotl.dStt=dStt; GrphTotl.dAltt=dAltT;
                    }
                    if(dRbcCur > dRbcMax)
                    {
                        bRbcMax = TRUE;
                        dRbcMax = dRbcCur;	// Bot Comp.
                        GrphTotl.dSbc=dSbc; GrphTotl.dAlbc=dAlc;
                    }
                    if(dRbtCur > dRbtMax)	
                    {
                        bRbtMax = TRUE;
                        dRbtMax = dRbtCur;	// Bot Tens.
                        GrphTotl.dSbt=dSbt; GrphTotl.dAlbt=dAltB;
                    }
                }
            }
        }
        if(!bRtcMax) GrphTotl.dAltc = dAlcMin;
        if(!bRttMax) GrphTotl.dAltt = dAltTMax;
        if(!bRbcMax) GrphTotl.dAlbc = dAlcMin;
        if(!bRbtMax) GrphTotl.dAlbt = dAltBMax;

        arGrphTotl.Add(GrphTotl);
    }
    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_ForcDiagram(CArray<_PRT_FORC_UENV_ELEM,_PRT_FORC_UENV_ELEM>& arUenvElemMax, CArray<_PRT_FORC_UENV_ELEM,_PRT_FORC_UENV_ELEM>& arUenvElemMin)
{
    arUenvElemMax.RemoveAll();
    arUenvElemMin.RemoveAll();
    if(!m_bPrintFlag) return FALSE;

    if(m_pForcCtrl->Get_LcomNumForStrn()==0) return FALSE;
    int i=0, j=0, k=0;
    for(i=0; i<m_arElemK.GetSize(); i++)
    {
        auto ElemK = m_arElemK.GetAt(i);
        for(j=0; j<2; j++)	// 0=Max, 1=Min.
        {
            _PRT_FORC_UENV_ELEM UenvElem;
            UenvElem.Initialize();
            UenvElem.ElemK = ElemK.first;
            for(k=0; k<cPOSI_CRC; k++)
            {
                UenvElem.LcomKMy[k] = m_arElemLcomMy[j][k].GetAt(i);
                UenvElem.LcomKVz[k] = m_arElemLcomVz[j][k].GetAt(i);
                UenvElem.LcomKMx[k] = m_arElemLcomMx[j][k].GetAt(i);
            }
            UenvElem.ForcMy = m_arElemMy[j].GetAt(i);
            UenvElem.ForcVz = m_arElemVz[j].GetAt(i);
            UenvElem.ForcMx = m_arElemMx[j].GetAt(i);
            if(j==0)			arUenvElemMax.Add(UenvElem);
            else if(j==1)	arUenvElemMin.Add(UenvElem);
            else	ASSERT(0);
        }
    }
    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_ResultBdcyPos(ArrElemPairKey& arElemK, CArray<PRT_BDCY_KSCE,PRT_BDCY_KSCE>& arBdcrKsce)
{
    arElemK.RemoveAll();
    arBdcrKsce.RemoveAll();

    int i=0, j=0;
    for(i=0; i<m_arElemK.GetSize(); i++)
    {
        auto ElemK = m_arElemK.GetAt(i);
        // Get Bdcy.
        T_BDCY_KSCE BdcyKsceD;
        BdcyKsceD.Initialize();
        if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetBdcyKsceData(ElemK,BdcyKsceD))	continue;
        PRT_BDCY_KSCE PrtBdcyD;
        PrtBdcyD.Initialize();
        for(j=0; j<2; j++)	// 0=I, 1=J.
        {			
            PrtBdcyD.BdcyBase[j] = BdcyKsceD.BdcyBase[j*2];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
            PrtBdcyD.BdcyBase[j].dMuy	 = Cng_MomFix(PrtBdcyD.BdcyBase[j].dMuy);
            PrtBdcyD.BdcyBase[j].dfpe	 = Cng_StrFix(PrtBdcyD.BdcyBase[j].dfpe);
            PrtBdcyD.BdcyBase[j].dAps	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].dAps,2);
            PrtBdcyD.BdcyBase[j].dAst	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].dAst,2);
            PrtBdcyD.BdcyBase[j].dIy	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].dIy ,4);
            PrtBdcyD.BdcyBase[j].db		 = Cng_LenFix(PrtBdcyD.BdcyBase[j].db  ,1);
            PrtBdcyD.BdcyBase[j].dh		 = Cng_LenFix(PrtBdcyD.BdcyBase[j].dh  ,1);
            PrtBdcyD.BdcyBase[j].ddst	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].ddst,1);
            PrtBdcyD.BdcyBase[j].ddps	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].ddps,1);
            PrtBdcyD.BdcyBase[j].dfps	 = Cng_StrFix(PrtBdcyD.BdcyBase[j].dfps);
            PrtBdcyD.BdcyBase[j].da		 = Cng_LenFix(PrtBdcyD.BdcyBase[j].da	 ,1);
            PrtBdcyD.BdcyBase[j].dpMny = Cng_MomFix(PrtBdcyD.BdcyBase[j].dpMny);
            PrtBdcyD.BdcyBase[j].d12Mcr= Cng_MomFix(PrtBdcyD.BdcyBase[j].d12Mcr);
            PrtBdcyD.BdcyBase[j].dPe	 = Cng_ForFix(PrtBdcyD.BdcyBase[j].dPe);
            PrtBdcyD.BdcyBase[j].dAsc	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].dAsc,2);
            PrtBdcyD.BdcyBase[j].ddsc	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].ddsc,1);
            PrtBdcyD.BdcyBase[j].dyt	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].dyt ,1);
            PrtBdcyD.BdcyBase[j].deps	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].deps,1);
            PrtBdcyD.BdcyBase[j].dAc	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].dAc ,2);
            PrtBdcyD.BdcyBase[j].dfy	 = Cng_StrFix(PrtBdcyD.BdcyBase[j].dfy);
            PrtBdcyD.BdcyBase[j].dfck	 = Cng_StrFix(PrtBdcyD.BdcyBase[j].dfck);
        }
        arElemK.Add(ElemK);
        arBdcrKsce.Add(PrtBdcyD);
    }
    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_ResultBdcyNeg(ArrElemPairKey& arElemK, CArray<PRT_BDCY_KSCE,PRT_BDCY_KSCE>& arBdcrKsce)
{
    arElemK.RemoveAll();
    arBdcrKsce.RemoveAll();

    int i=0, j=0;
    for(i=0; i<m_arElemK.GetSize(); i++)
    {
        auto ElemK = m_arElemK.GetAt(i);
        // Get Bdcy.
        T_BDCY_KSCE BdcyKsceD;
        BdcyKsceD.Initialize();
        if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetBdcyKsceData(ElemK,BdcyKsceD))	continue;
        PRT_BDCY_KSCE PrtBdcyD;
        PrtBdcyD.Initialize();
        for(j=0; j<2; j++)	// 0=I, 1=J.
        {			
            PrtBdcyD.BdcyBase[j] = BdcyKsceD.BdcyBase[j*2+1];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
            PrtBdcyD.BdcyBase[j].dMuy	 = Cng_MomFix(PrtBdcyD.BdcyBase[j].dMuy);
            PrtBdcyD.BdcyBase[j].dfpe	 = Cng_StrFix(PrtBdcyD.BdcyBase[j].dfpe);
            PrtBdcyD.BdcyBase[j].dAps	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].dAps,2);
            PrtBdcyD.BdcyBase[j].dAst	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].dAst,2);
            PrtBdcyD.BdcyBase[j].dIy	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].dIy ,4);
            PrtBdcyD.BdcyBase[j].db 	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].db  ,1);
            PrtBdcyD.BdcyBase[j].dh 	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].dh  ,1);
            PrtBdcyD.BdcyBase[j].ddst	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].ddst,1);
            PrtBdcyD.BdcyBase[j].ddps	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].ddps,1);
            PrtBdcyD.BdcyBase[j].dfps	 = Cng_StrFix(PrtBdcyD.BdcyBase[j].dfps);
            PrtBdcyD.BdcyBase[j].da		 = Cng_LenFix(PrtBdcyD.BdcyBase[j].da	 ,1);
            PrtBdcyD.BdcyBase[j].dpMny = Cng_MomFix(PrtBdcyD.BdcyBase[j].dpMny);
            PrtBdcyD.BdcyBase[j].d12Mcr= Cng_MomFix(PrtBdcyD.BdcyBase[j].d12Mcr);
            PrtBdcyD.BdcyBase[j].dPe	 = Cng_ForFix(PrtBdcyD.BdcyBase[j].dPe);
            PrtBdcyD.BdcyBase[j].dAsc	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].dAsc,2);
            PrtBdcyD.BdcyBase[j].ddsc	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].ddsc,1);
            PrtBdcyD.BdcyBase[j].dyt	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].dyt ,1);
            PrtBdcyD.BdcyBase[j].deps	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].deps,1);
            PrtBdcyD.BdcyBase[j].dAc	 = Cng_LenFix(PrtBdcyD.BdcyBase[j].dAc ,2);
            PrtBdcyD.BdcyBase[j].dfy	 = Cng_StrFix(PrtBdcyD.BdcyBase[j].dfy);
            PrtBdcyD.BdcyBase[j].dfck	 = Cng_StrFix(PrtBdcyD.BdcyBase[j].dfck);
        }
        arElemK.Add(ElemK);
        arBdcrKsce.Add(PrtBdcyD);
    }
    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_ResultCums(ArrElemPairKey& arElemK, CArray<PRT_CUMS_KSCE,PRT_CUMS_KSCE>& arCumsKsce)
{
    arElemK.RemoveAll();
    arCumsKsce.RemoveAll();

    int i=0, j=0;
    for(i=0; i<m_arElemK.GetSize(); i++)
    {
        auto ElemK = m_arElemK.GetAt(i);
        // Get Cums.
        T_CUMS_KSCE CumsKsceD;
        CumsKsceD.Initialize();
        if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCumsKsceData(ElemK,CumsKsceD))	continue;
        double dRatMaxI = (fabs(CumsKsceD.CumsBase[0].dpVn) < cDgn_Zero ? 0.0 : fabs(CumsKsceD.CumsBase[0].dVu)/fabs(CumsKsceD.CumsBase[0].dpVn));	// I-MAX.
        double dRatMinI = (fabs(CumsKsceD.CumsBase[1].dpVn) < cDgn_Zero ? 0.0 : fabs(CumsKsceD.CumsBase[1].dVu)/fabs(CumsKsceD.CumsBase[1].dpVn));	// I-MIN.
        double dRatMaxJ = (fabs(CumsKsceD.CumsBase[2].dpVn) < cDgn_Zero ? 0.0 : fabs(CumsKsceD.CumsBase[2].dVu)/fabs(CumsKsceD.CumsBase[2].dpVn));	// J-MAX.
        double dRatMinJ = (fabs(CumsKsceD.CumsBase[3].dpVn) < cDgn_Zero ? 0.0 : fabs(CumsKsceD.CumsBase[3].dVu)/fabs(CumsKsceD.CumsBase[3].dpVn));	// J-MIN.
        int iI = (dRatMaxI > dRatMinI ? 0 : 1);
        int iJ = (dRatMaxJ > dRatMinJ ? 2 : 3);
        PRT_CUMS_KSCE PrtCumsD;
        PrtCumsD.Initialize();
        for(j=0; j<2; j++)	// 0=I, 1=J.
        {
            PrtCumsD.CumsBase[j] = CumsKsceD.CumsBase[j==0 ? iI : iJ];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
            PrtCumsD.CumsBase[j].dVu		= Cng_ForFix(PrtCumsD.CumsBase[j].dVu);
            PrtCumsD.CumsBase[j].dVci		= Cng_ForFix(PrtCumsD.CumsBase[j].dVci);
            PrtCumsD.CumsBase[j].dVcw		= Cng_ForFix(PrtCumsD.CumsBase[j].dVcw);
            PrtCumsD.CumsBase[j].dVc		= Cng_ForFix(PrtCumsD.CumsBase[j].dVc);
            PrtCumsD.CumsBase[j].dVs		= Cng_ForFix(PrtCumsD.CumsBase[j].dVs);
            PrtCumsD.CumsBase[j].dpVn		= Cng_ForFix(PrtCumsD.CumsBase[j].dpVn);
            PrtCumsD.CumsBase[j].dAv		= Cng_LenFix(PrtCumsD.CumsBase[j].dAv		 ,2);
            PrtCumsD.CumsBase[j].dAvmin	= Cng_LenFix(PrtCumsD.CumsBase[j].dAvmin ,2);
            PrtCumsD.CumsBase[j].dAvreq	= Cng_LenFix(PrtCumsD.CumsBase[j].dAvreq ,2);
            PrtCumsD.CumsBase[j].dbw		= Cng_LenFix(PrtCumsD.CumsBase[j].dbw		 ,1);
            PrtCumsD.CumsBase[j].dd			= Cng_LenFix(PrtCumsD.CumsBase[j].dd		 ,1);
            PrtCumsD.CumsBase[j].dVi		= Cng_ForFix(PrtCumsD.CumsBase[j].dVi);
            PrtCumsD.CumsBase[j].dVd		= Cng_ForFix(PrtCumsD.CumsBase[j].dVd);
            PrtCumsD.CumsBase[j].dMmax  = Cng_MomFix(PrtCumsD.CumsBase[j].dMmax);
            PrtCumsD.CumsBase[j].dMu		= Cng_MomFix(PrtCumsD.CumsBase[j].dMu);
            PrtCumsD.CumsBase[j].dMd		= Cng_MomFix(PrtCumsD.CumsBase[j].dMd);
            PrtCumsD.CumsBase[j].dMcr		= Cng_MomFix(PrtCumsD.CumsBase[j].dMcr);
            PrtCumsD.CumsBase[j].dIy		= Cng_LenFix(PrtCumsD.CumsBase[j].dIy		 ,4);
            PrtCumsD.CumsBase[j].dh			= Cng_LenFix(PrtCumsD.CumsBase[j].dh		 ,1);
            PrtCumsD.CumsBase[j].dyt		= Cng_LenFix(PrtCumsD.CumsBase[j].dyt		 ,1);
            PrtCumsD.CumsBase[j].dfpe		= Cng_StrFix(PrtCumsD.CumsBase[j].dfpe);
            PrtCumsD.CumsBase[j].dfd		= Cng_StrFix(PrtCumsD.CumsBase[j].dfd);
            PrtCumsD.CumsBase[j].dfpc		= Cng_StrFix(PrtCumsD.CumsBase[j].dfpc);
            PrtCumsD.CumsBase[j].dVp		= Cng_ForFix(PrtCumsD.CumsBase[j].dVp);
            PrtCumsD.CumsBase[j].dfck		= Cng_StrFix(PrtCumsD.CumsBase[j].dfck);
            PrtCumsD.CumsBase[j].dsv		= Cng_LenFix(PrtCumsD.CumsBase[j].dsv		 ,1);
            PrtCumsD.CumsBase[j].dThetaV= Cng_DegFix(PrtCumsD.CumsBase[j].dThetaV);
            PrtCumsD.CumsBase[j].dsp		= Cng_LenFix(PrtCumsD.CumsBase[j].dsp		 ,1);
            PrtCumsD.CumsBase[j].dThetap= Cng_DegFix(PrtCumsD.CumsBase[j].dThetap);
            PrtCumsD.CumsBase[j].dAp		= Cng_LenFix(PrtCumsD.CumsBase[j].dAp		 ,2);
        }
        arElemK.Add(ElemK);
        arCumsKsce.Add(PrtCumsD);
    }
    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_ResultCrmt(ArrElemPairKey& arElemK, CArray<PRT_CRMT_KSCE,PRT_CRMT_KSCE>& arCrmtKsce)
{
    arElemK.RemoveAll();
    arCrmtKsce.RemoveAll();

    int i=0, j=0;
    for(i=0; i<m_arElemK.GetSize(); i++)
    {
        auto ElemK = m_arElemK.GetAt(i);
        // Get Crmt.
        T_CRMT_KSCE CrmtKsceD;
        CrmtKsceD.Initialize();
        if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCrmtKsceData(ElemK,CrmtKsceD))	continue;
        double dRatVMaxI = (fabs(CrmtKsceD.CrmtBase[0].dpTn) < cDgn_Zero ? 0.0 : fabs(CrmtKsceD.CrmtBase[0].dTu)/fabs(CrmtKsceD.CrmtBase[0].dpTn));	// I-V-MAX.
        double dRatVMinI = (fabs(CrmtKsceD.CrmtBase[1].dpTn) < cDgn_Zero ? 0.0 : fabs(CrmtKsceD.CrmtBase[1].dTu)/fabs(CrmtKsceD.CrmtBase[1].dpTn));	// I-V-MIN.
        double dRatTMaxI = (fabs(CrmtKsceD.CrmtBase[2].dpTn) < cDgn_Zero ? 0.0 : fabs(CrmtKsceD.CrmtBase[2].dTu)/fabs(CrmtKsceD.CrmtBase[2].dpTn));	// I-T-MAX.
        double dRatVMaxJ = (fabs(CrmtKsceD.CrmtBase[3].dpTn) < cDgn_Zero ? 0.0 : fabs(CrmtKsceD.CrmtBase[3].dTu)/fabs(CrmtKsceD.CrmtBase[3].dpTn));	// J-V-MAX.
        double dRatVMinJ = (fabs(CrmtKsceD.CrmtBase[4].dpTn) < cDgn_Zero ? 0.0 : fabs(CrmtKsceD.CrmtBase[4].dTu)/fabs(CrmtKsceD.CrmtBase[4].dpTn));	// J-V-MIN.
        double dRatTMaxJ = (fabs(CrmtKsceD.CrmtBase[5].dpTn) < cDgn_Zero ? 0.0 : fabs(CrmtKsceD.CrmtBase[5].dTu)/fabs(CrmtKsceD.CrmtBase[5].dpTn));	// J-T-MAX.
        double dRatMaxI	= max(max(dRatVMaxI,dRatVMinI),dRatTMaxI);
        double dRatMaxJ	= max(max(dRatVMaxJ,dRatVMinJ),dRatTMaxJ);
        int iI=0;
        if(fabs(dRatVMaxI-dRatMaxI) < cDgn_Zero)	iI = 0;
        else if(fabs(dRatVMinI-dRatMaxI) < cDgn_Zero)	iI = 1;
        else if(fabs(dRatTMaxI-dRatMaxI) < cDgn_Zero)	iI = 2;
        int iJ=0;
        if(fabs(dRatVMaxJ-dRatMaxJ) < cDgn_Zero)	iJ = 3;
        else if(fabs(dRatVMinJ-dRatMaxJ) < cDgn_Zero)	iJ = 4;
        else if(fabs(dRatTMaxJ-dRatMaxJ) < cDgn_Zero)	iJ = 5;
        PRT_CRMT_KSCE PrtCrmtD;
        PrtCrmtD.Initialize();
        for(j=0; j<2; j++)	// 0=I, 1=J.
        {
            PrtCrmtD.CrmtBase[j] = CrmtKsceD.CrmtBase[j==0 ? iI : iJ];	// 0=I-V-MAX, 1=I-V-MIN, 2=I-T-MAX, 3=J-V-MAX, 4=J-V-MIN, 5=J-T-MAX.
            PrtCrmtD.CrmtBase[j].dTu		 = Cng_MomFix(PrtCrmtD.CrmtBase[j].dTu);
            PrtCrmtD.CrmtBase[j].dTcr		 = Cng_MomFix(PrtCrmtD.CrmtBase[j].dTcr);
            PrtCrmtD.CrmtBase[j].dAoh		 = Cng_LenFix(PrtCrmtD.CrmtBase[j].dAoh	 ,2);
            PrtCrmtD.CrmtBase[j].dVc		 = Cng_ForFix(PrtCrmtD.CrmtBase[j].dVc);
            PrtCrmtD.CrmtBase[j].dvVT		 = Cng_StrFix(PrtCrmtD.CrmtBase[j].dvVT);
            PrtCrmtD.CrmtBase[j].dvVTmax = Cng_StrFix(PrtCrmtD.CrmtBase[j].dvVTmax);
            PrtCrmtD.CrmtBase[j].dpTn		 = Cng_MomFix(PrtCrmtD.CrmtBase[j].dpTn);
            PrtCrmtD.CrmtBase[j].dAt		 = Cng_LenFix(PrtCrmtD.CrmtBase[j].dAt	 ,2);
            PrtCrmtD.CrmtBase[j].dAtmin	 = Cng_LenFix(PrtCrmtD.CrmtBase[j].dAtmin,2);
            PrtCrmtD.CrmtBase[j].dAluse	 = Cng_LenFix(PrtCrmtD.CrmtBase[j].dAluse,2);
            PrtCrmtD.CrmtBase[j].dAlreq	 = Cng_LenFix(PrtCrmtD.CrmtBase[j].dAlreq,2);
            PrtCrmtD.CrmtBase[j].dAlmin	 = Cng_LenFix(PrtCrmtD.CrmtBase[j].dAlmin,2);
            PrtCrmtD.CrmtBase[j].dAcp		 = Cng_LenFix(PrtCrmtD.CrmtBase[j].dAcp	 ,2);
            PrtCrmtD.CrmtBase[j].dPcp		 = Cng_LenFix(PrtCrmtD.CrmtBase[j].dPcp	 ,1);
            PrtCrmtD.CrmtBase[j].dfpc		 = Cng_StrFix(PrtCrmtD.CrmtBase[j].dfpc);
            PrtCrmtD.CrmtBase[j].dfck		 = Cng_StrFix(PrtCrmtD.CrmtBase[j].dfck);
            PrtCrmtD.CrmtBase[j].dMu		 = Cng_MomFix(PrtCrmtD.CrmtBase[j].dMu);
            PrtCrmtD.CrmtBase[j].dVu		 = Cng_ForFix(PrtCrmtD.CrmtBase[j].dVu);
            PrtCrmtD.CrmtBase[j].dPh		 = Cng_LenFix(PrtCrmtD.CrmtBase[j].dPh   ,1);
            PrtCrmtD.CrmtBase[j].dbw		 = Cng_LenFix(PrtCrmtD.CrmtBase[j].dbw	 ,1);
            PrtCrmtD.CrmtBase[j].dt			 = Cng_LenFix(PrtCrmtD.CrmtBase[j].dt		 ,1);
            PrtCrmtD.CrmtBase[j].dThetaC = Cng_DegFix(PrtCrmtD.CrmtBase[j].dThetaC);
            PrtCrmtD.CrmtBase[j].dfyv		 = Cng_StrFix(PrtCrmtD.CrmtBase[j].dfyv);
            PrtCrmtD.CrmtBase[j].dfyl		 = Cng_StrFix(PrtCrmtD.CrmtBase[j].dfyl);
        }
        arElemK.Add(ElemK);
        arCrmtKsce.Add(PrtCrmtD);
    }
    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_DetailTbarPos(ElemPairK ElemK)
{
    // Get Posx.
    T_POSX_D PosxD;
    PosxD.Initialize();
    if (ElemK.second == EN_EL_BEAM)
    {
        if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD))	return FALSE;
    }
    else if (ElemK.second == EN_EL_BEAM)
    {
        if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD))	return FALSE;
    }
    else ASSERT(0);
    
    BOOL bPrtI=FALSE, bPrtJ=FALSE;
    if(PosxD.iBarChk[0]==0)	return FALSE;											// NONE.
    else if(PosxD.iBarChk[0]==1)	{bPrtI=TRUE ; bPrtJ=FALSE;}	// I.
    else if(PosxD.iBarChk[0]==2)	{bPrtI=FALSE; bPrtJ=TRUE ;}	// J.
    else if(PosxD.iBarChk[0]==3)	{bPrtI=TRUE ; bPrtJ=TRUE ;}	// I&J.
    else	ASSERT(0);
    // Get Tbar.
    T_TBAR_D TbarD;
    TbarD.Initialize();
    if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetTbarData(ElemK,TbarD))	return FALSE;
    TbarD.TbarBase[0].bCHK = (TbarD.TbarBase[0].bCHK && bPrtI ? TRUE : FALSE);	// 0=I-TOP-MAX.
    TbarD.TbarBase[1].bCHK = FALSE;									                          	// 1=I-BOT-MAX.
    TbarD.TbarBase[2].bCHK = (TbarD.TbarBase[2].bCHK && bPrtJ ? TRUE : FALSE);	// 2=J-TOP-MAX.
    TbarD.TbarBase[3].bCHK = FALSE;									                         	  // 3=J-BOT-MAX.

    BOOL bExecutePrt = FALSE;
    for(int i=0; i<4; i++)
    {
        if(TbarD.TbarBase[i].bCHK) bExecutePrt = TRUE;
    }

    if(!bExecutePrt) return FALSE;

    // Print on Calculating.
    return m_pDataCtrl->Get_DgnKscePointer()->Get_KsceTbar(ElemK,TbarD,bPrtI,bPrtJ,2);	// 2=Bot.
}

BOOL CPrtKSCE_USD03::Get_DetailTbarNeg(ElemPairK ElemK)
{
    // Get Posx.
    T_POSX_D PosxD;
    PosxD.Initialize();
    if (ElemK.second == EN_EL_BEAM)
    {
        if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD))	return FALSE;
    }
    else if (ElemK.second == EN_EL_BEAM)
    {
        if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD))	return FALSE;
    }
    else ASSERT(0);

    BOOL bPrtI=FALSE, bPrtJ=FALSE;
    if(PosxD.iBarChk[1]==0)	return FALSE;											// NONE.
    else if(PosxD.iBarChk[1]==1)	{bPrtI=TRUE ; bPrtJ=FALSE;}	// I.
    else if(PosxD.iBarChk[1]==2)	{bPrtI=FALSE; bPrtJ=TRUE ;}	// J.
    else if(PosxD.iBarChk[1]==3)	{bPrtI=TRUE ; bPrtJ=TRUE ;}	// I&J.
    else	ASSERT(0);
    // Get Tbar.
    T_TBAR_D TbarD;
    TbarD.Initialize();
    if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetTbarData(ElemK,TbarD))	return FALSE;
    TbarD.TbarBase[0].bCHK = FALSE;									          	                // 0=I-TOP-MAX.
    TbarD.TbarBase[1].bCHK = (TbarD.TbarBase[1].bCHK && bPrtI ? TRUE : FALSE);	// 1=I-BOT-MAX.
    TbarD.TbarBase[2].bCHK = FALSE;										                          // 2=J-TOP-MAX.
    TbarD.TbarBase[3].bCHK = (TbarD.TbarBase[3].bCHK && bPrtJ ? TRUE : FALSE);	// 3=J-BOT-MAX.

    BOOL bExecutePrt = FALSE;
    for(int i=0; i<4; i++)
    {
        if(TbarD.TbarBase[i].bCHK) bExecutePrt = TRUE;
    }

    if(!bExecutePrt) return FALSE;

    // Print on Calculating.
    return m_pDataCtrl->Get_DgnKscePointer()->Get_KsceTbar(ElemK,TbarD,bPrtI,bPrtJ,1);	// 1=Top.
}

BOOL CPrtKSCE_USD03::Get_DetailBdcyPos(ElemPairK ElemK)
{
    // Get Posx.
    T_POSX_D PosxD;
    PosxD.Initialize();
    if (ElemK.second == EN_EL_BEAM)
    {
        if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD))	return FALSE;
    }
    else if (ElemK.second == EN_EL_BEAM)
    {
        if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD))	return FALSE;
    }
    else ASSERT(0);

    BOOL bPrtI=FALSE, bPrtJ=FALSE;
    if(PosxD.iMomChk[0]==0)	return FALSE;											// NONE.
    else if(PosxD.iMomChk[0]==1)	{bPrtI=TRUE ; bPrtJ=FALSE;}	// I.
    else if(PosxD.iMomChk[0]==2)	{bPrtI=FALSE; bPrtJ=TRUE ;}	// J.
    else if(PosxD.iMomChk[0]==3)	{bPrtI=TRUE ; bPrtJ=TRUE ;}	// I&J.
    else	ASSERT(0);
    // Get Bdcy.
    T_BDCY_KSCE BdcyKsceD;
    BdcyKsceD.Initialize();
    if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetBdcyKsceData(ElemK,BdcyKsceD))	return FALSE;
    BdcyKsceD.BdcyBase[0].bCHK = (BdcyKsceD.BdcyBase[0].bCHK && bPrtI ? TRUE : FALSE);	// 0=I-MAX.
    BdcyKsceD.BdcyBase[1].bCHK = FALSE;									  	                            // 1=I-MIN.
    BdcyKsceD.BdcyBase[2].bCHK = (BdcyKsceD.BdcyBase[2].bCHK && bPrtJ ? TRUE : FALSE);	// 2=J-MAX.
    BdcyKsceD.BdcyBase[3].bCHK = FALSE;										                              // 3=J-MIN.

    BOOL bExecutePrt = FALSE;
    for(int i=0; i<4; i++)
    {
        if(BdcyKsceD.BdcyBase[i].bCHK) bExecutePrt = TRUE;
    }

    if(!bExecutePrt) return FALSE;
    // Print on Calculating.
    return m_pDataCtrl->Get_DgnKscePointer()->Get_KsceBdcy(ElemK,BdcyKsceD,bPrtI,bPrtJ);
}

BOOL CPrtKSCE_USD03::Get_DetailBdcyNeg(ElemPairK ElemK)
{
    // Get Posx.
    T_POSX_D PosxD;
    PosxD.Initialize();
    if (ElemK.second == EN_EL_BEAM)
    {
        if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD))	return FALSE;
    }
    else if (ElemK.second == EN_EL_BEAM)
    {
        if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD))	return FALSE;
    }
    else ASSERT(0);

    BOOL bPrtI=FALSE, bPrtJ=FALSE;
    if(PosxD.iMomChk[1]==0)	return FALSE;											// NONE.
    else if(PosxD.iMomChk[1]==1)	{bPrtI=TRUE ; bPrtJ=FALSE;}	// I.
    else if(PosxD.iMomChk[1]==2)	{bPrtI=FALSE; bPrtJ=TRUE ;}	// J.
    else if(PosxD.iMomChk[1]==3)	{bPrtI=TRUE ; bPrtJ=TRUE ;}	// I&J.
    else	ASSERT(0);
    // Get Bdcy.
    T_BDCY_KSCE BdcyKsceD;
    BdcyKsceD.Initialize();
    if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetBdcyKsceData(ElemK,BdcyKsceD))	return FALSE;
    BdcyKsceD.BdcyBase[0].bCHK = FALSE;								  		                            // 0=I-MAX.
    BdcyKsceD.BdcyBase[1].bCHK = (BdcyKsceD.BdcyBase[1].bCHK && bPrtI ? TRUE : FALSE);	// 1=I-MIN.
    BdcyKsceD.BdcyBase[2].bCHK = FALSE;										                              // 2=J-MAX.
    BdcyKsceD.BdcyBase[3].bCHK = (BdcyKsceD.BdcyBase[3].bCHK && bPrtJ ? TRUE : FALSE);	// 3=J-MIN.

    BOOL bExecutePrt = FALSE;
    for(int i=0; i<4; i++)
    {
        if(BdcyKsceD.BdcyBase[i].bCHK) bExecutePrt = TRUE;
    }

    if(!bExecutePrt) return FALSE;

    // Print on Calculating.
    return m_pDataCtrl->Get_DgnKscePointer()->Get_KsceBdcy(ElemK,BdcyKsceD,bPrtI,bPrtJ);
}

BOOL CPrtKSCE_USD03::Get_DetailCums(ElemPairK ElemK)
{
    // Get Posx.
    T_POSX_D PosxD;
    PosxD.Initialize();
    if (ElemK.second == EN_EL_BEAM)
    {
        if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD))	return FALSE;
    }
    else if (ElemK.second == EN_EL_BEAM)
    {
        if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD))	return FALSE;
    }
    else ASSERT(0);

    BOOL bPrtI=FALSE, bPrtJ=FALSE;
    if(PosxD.iShrChk==0)	return FALSE;										// NONE.
    else if(PosxD.iShrChk==1)	{bPrtI=TRUE ; bPrtJ=FALSE;}	// I.
    else if(PosxD.iShrChk==2)	{bPrtI=FALSE; bPrtJ=TRUE ;}	// J.
    else if(PosxD.iShrChk==3)	{bPrtI=TRUE ; bPrtJ=TRUE ;}	// I&J.
    else	ASSERT(0);
    // Get Cums.
    T_CUMS_KSCE CumsKsceD;
    CumsKsceD.Initialize();
    if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCumsKsceData(ElemK,CumsKsceD))	return FALSE;

    double dRatMaxI = (fabs(CumsKsceD.CumsBase[0].dpVn) < cDgn_Zero ? 0.0 : fabs(CumsKsceD.CumsBase[0].dVu)/fabs(CumsKsceD.CumsBase[0].dpVn));	// I-MAX.
    double dRatMinI = (fabs(CumsKsceD.CumsBase[1].dpVn) < cDgn_Zero ? 0.0 : fabs(CumsKsceD.CumsBase[1].dVu)/fabs(CumsKsceD.CumsBase[1].dpVn));	// I-MIN.
    double dRatMaxJ = (fabs(CumsKsceD.CumsBase[2].dpVn) < cDgn_Zero ? 0.0 : fabs(CumsKsceD.CumsBase[2].dVu)/fabs(CumsKsceD.CumsBase[2].dpVn));	// J-MAX.
    double dRatMinJ = (fabs(CumsKsceD.CumsBase[3].dpVn) < cDgn_Zero ? 0.0 : fabs(CumsKsceD.CumsBase[3].dVu)/fabs(CumsKsceD.CumsBase[3].dpVn));	// J-MIN.
    int iI = (dRatMaxI > dRatMinI ? 0 : 1);
    int iJ = (dRatMaxJ > dRatMinJ ? 2 : 3);

    BOOL bExecutePrt = FALSE;
    for(int i=0; i<2; i++) // I,J
    {
        BOOL bPrtIJ = (i==0 ? bPrtI : bPrtJ);
        for(int j=0; j<2; j++)
        {
            int k = i*2+j;
            BOOL bPrint = CumsKsceD.CumsBase[k].bCHK;
            //CumsKsceD.CumsBase[k].bCHK = (bPrtIJ && bPrint ? TRUE : FALSE);

            if(k==0 || k==1)
            {
                CumsKsceD.CumsBase[k].bCHK = (iI==k && bPrtI && bPrint ? TRUE : FALSE);
            }
            else if(k==2 || k==3)
            {
                CumsKsceD.CumsBase[k].bCHK = (iJ==k && bPrtJ && bPrint ? TRUE : FALSE);
            }

            if(CumsKsceD.CumsBase[k].bCHK) bExecutePrt = TRUE;
        }    
    }

    if(!bExecutePrt) return FALSE;
    // Print on Calculating.
    return m_pDataCtrl->Get_DgnKscePointer()->Get_KsceCums(ElemK,CumsKsceD,bPrtI,bPrtJ);
}

BOOL CPrtKSCE_USD03::Get_DetailCrmt(ElemPairK ElemK)
{
    // Get Posx.
    T_POSX_D PosxD;
    PosxD.Initialize();
    if (ElemK.second == EN_EL_BEAM)
    {
        if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD))	return FALSE;
    }
    else if (ElemK.second == EN_EL_BEAM)
    {
        if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD))	return FALSE;
    }
    else ASSERT(0);

    BOOL bPrtI=FALSE, bPrtJ=FALSE;
    if(PosxD.iTorChk==0)	return FALSE;										// NONE.
    else if(PosxD.iTorChk==1)	{bPrtI=TRUE ; bPrtJ=FALSE;}	// I.
    else if(PosxD.iTorChk==2)	{bPrtI=FALSE; bPrtJ=TRUE ;}	// J.
    else if(PosxD.iTorChk==3)	{bPrtI=TRUE ; bPrtJ=TRUE ;}	// I&J.
    else	ASSERT(0);
    // Get Crmt.
    T_CRMT_KSCE CrmtKsceD;
    CrmtKsceD.Initialize();
    if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCrmtKsceData(ElemK,CrmtKsceD))	return FALSE;

    double dRatVMaxI = (fabs(CrmtKsceD.CrmtBase[0].dpTn) < cDgn_Zero ? 0.0 : fabs(CrmtKsceD.CrmtBase[0].dTu)/fabs(CrmtKsceD.CrmtBase[0].dpTn));	// I-V-MAX.
    double dRatVMinI = (fabs(CrmtKsceD.CrmtBase[1].dpTn) < cDgn_Zero ? 0.0 : fabs(CrmtKsceD.CrmtBase[1].dTu)/fabs(CrmtKsceD.CrmtBase[1].dpTn));	// I-V-MIN.
    double dRatTMaxI = (fabs(CrmtKsceD.CrmtBase[2].dpTn) < cDgn_Zero ? 0.0 : fabs(CrmtKsceD.CrmtBase[2].dTu)/fabs(CrmtKsceD.CrmtBase[2].dpTn));	// I-T-MAX.
    double dRatVMaxJ = (fabs(CrmtKsceD.CrmtBase[3].dpTn) < cDgn_Zero ? 0.0 : fabs(CrmtKsceD.CrmtBase[3].dTu)/fabs(CrmtKsceD.CrmtBase[3].dpTn));	// J-V-MAX.
    double dRatVMinJ = (fabs(CrmtKsceD.CrmtBase[4].dpTn) < cDgn_Zero ? 0.0 : fabs(CrmtKsceD.CrmtBase[4].dTu)/fabs(CrmtKsceD.CrmtBase[4].dpTn));	// J-V-MIN.
    double dRatTMaxJ = (fabs(CrmtKsceD.CrmtBase[5].dpTn) < cDgn_Zero ? 0.0 : fabs(CrmtKsceD.CrmtBase[5].dTu)/fabs(CrmtKsceD.CrmtBase[5].dpTn));	// J-T-MAX.
    double dRatMaxI	= max(max(dRatVMaxI,dRatVMinI),dRatTMaxI);
    double dRatMaxJ	= max(max(dRatVMaxJ,dRatVMinJ),dRatTMaxJ);
    int iI=0;
    if(fabs(dRatVMaxI-dRatMaxI) < cDgn_Zero)	iI = 0;
    else if(fabs(dRatVMinI-dRatMaxI) < cDgn_Zero)	iI = 1;
    else if(fabs(dRatTMaxI-dRatMaxI) < cDgn_Zero)	iI = 2;
    int iJ=0;
    if(fabs(dRatVMaxJ-dRatMaxJ) < cDgn_Zero)	iJ = 3;
    else if(fabs(dRatVMinJ-dRatMaxJ) < cDgn_Zero)	iJ = 4;
    else if(fabs(dRatTMaxJ-dRatMaxJ) < cDgn_Zero)	iJ = 5;

    BOOL bExecutePrt = FALSE;
    for(int i=0; i<2; i++) // I,J
    {
        BOOL bPrtIJ = (i==0 ? bPrtI : bPrtJ);
        for(int j=0; j<3; j++)
        {
            int k = i*3+j;
            BOOL bPrint = CrmtKsceD.CrmtBase[k].bCHK;
            //CrmtKsceD.CrmtBase[k].bCHK = (bPrtIJ && bPrint ? TRUE : FALSE);

            if(k==0 || k==1 || k==2)
            {
                CrmtKsceD.CrmtBase[k].bCHK = (iI==k && bPrtI && bPrint? TRUE : FALSE);
            }
            else if(k==3 || k==4 || k==5)
            {
                CrmtKsceD.CrmtBase[k].bCHK = (iJ==k && bPrtJ && bPrint? TRUE : FALSE);
            }

            if(CrmtKsceD.CrmtBase[k].bCHK) bExecutePrt = TRUE;
        }    
    }

    if(!bExecutePrt) return FALSE;

    // Print on Calculating.
    return m_pDataCtrl->Get_DgnKscePointer()->Get_KsceCrmt(ElemK,CrmtKsceD,bPrtI,bPrtJ);
}

// 인장철근량 계산.
void CPrtKSCE_USD03::Print_TbarDesign1()
{
    CString strTbar=_T("");
    strTbar = _T("Text: 인장응력 검토");                                m_arText.Add(strTbar);
    strTbar = _T("Text: 비례식으로 인장영역의 높이를 계산");            m_arText.Add(strTbar);
    m_arText.Add(_T(""));
}

void CPrtKSCE_USD03::Print_TbarDesign2(double dftop, double dfbot, double dh, double dx, double dycent, double dfcent,
                                       double dAten, double dTfor, double dfsa, double dFS, double dAsreq)
{
    CString strTbar=_T("");
    CString sftop  = Check_ValueForm(6,2,dftop);
    CString sfbot  = Check_ValueForm(6,2,dfbot);
    CString sh     = Check_ValueForm(6,2,dh);
    CString sx     = Check_ValueForm(6,2,dx);
    CString sycent = Check_ValueForm(6,2,dycent);
    CString sfcent = Check_ValueForm(6,2,dfcent);
    CString sAten  = Check_ValueForm(6,2,dAten);
    CString sTfor  = Check_ValueForm(6,2,dTfor);
    CString sfsa   = Check_ValueForm(6,2,dfsa);
    CString sFS    = Check_ValueForm(6,2,dFS);
    CString sAsreq = Check_ValueForm(6,2,dAsreq);

    CString sTop = Check_ValueForm(6,2,fabs(dftop));
    CString sBot = Check_ValueForm(6,2,fabs(dfbot));

    strTbar.Format(_T("~ f_top == %s %s ~ ~ f_bot == %s ~ %s"),sftop,m_strStress, sfbot,m_strStress);   m_arText.Add(strTbar);
    if((dftop>=0.0 && dfbot<=0.0) || (dftop<=0.0 && dfbot>=0.0))
    {
        if(dftop>0.0)
        {
            strTbar.Format(_T("~ a == %s / ( %s + %s ) * %s ~ = %s ~ %s"), sTop, sTop, sBot, sh, sx, m_strLength);  m_arText.Add(strTbar);
            m_arText.Add(_T(""));
            strTbar = _T("Text: 인장력 T");                             m_arText.Add(strTbar);
            strTbar = _T("Text: - 인장영역의 도심위치에서의 응력");     m_arText.Add(strTbar);
            m_arText.Add(_T(""));
            strTbar.Format(_T("~ f_cent == y_cent * f_top / a == %s * %s / %s ~ = %s ~ %s"),
                sycent, sTop , sx, sfcent, m_strStress);    m_arText.Add(strTbar);
            strTbar.Format(_T("~ T == f_cent * A_ten == %s * %s ~ = %s ~ %s"),sfcent, sAten, sTfor, m_strForce); m_arText.Add(strTbar);
            strTbar.Format(_T("~ A_sreq == T / f_sa == %s / %s ~ = %s ~ %s"),
                sTfor, sfsa, sAsreq, m_strArea);   m_arText.Add(strTbar);
        }
        else 
        {
            strTbar.Format(_T("~ a == %s / ( %s + %s ) * %s ~ = %s ~ %s"), sBot, sTop, sBot, sh, sx, m_strLength);  m_arText.Add(strTbar);
            strTbar = _T("Text: 인장력 T");            m_arText.Add(strTbar);
            strTbar = _T("Text: - 인장영역의 도심위치에서의 응력");     m_arText.Add(strTbar);
            m_arText.Add(_T(""));
            strTbar.Format(_T("~ f_cent == y_cent * f_bot / a == %s * %s / %s ~ = %s ~ %s"),
                sycent, sBot , sx, sfcent, m_strStress);    m_arText.Add(strTbar);
            strTbar.Format(_T("~ T == f_cent * A_ten == %s * %s ~ = %s ~ %s"),sfcent, sAten, sTfor, m_strForce); m_arText.Add(strTbar);
            strTbar.Format(_T("~ A_sreq == T / f_sa == %s / %s ~ = %s ~ %s"),
                sTfor, sfsa, sAsreq, m_strStress);   m_arText.Add(strTbar);
        }
    }

    m_arText.Add(_T(""));

}

void CPrtKSCE_USD03::Print_TbarDesign3(double dft, double dfm, double dftop, double dfbot, double dh, double dx, double dycent, double dfcent,
                                       double dTfor, double dfsa, double dFS, double dAsreq, double dyten, double dAc)
{
    CString strTbar=_T("");
    CString sft    = Check_ValueForm(6,2,dft);
    CString sfm    = Check_ValueForm(6,2,dfm);
    CString sftop  = Check_ValueForm(6,2,dftop);
    CString sfbot  = Check_ValueForm(6,2,dfbot);
    CString sh     = Check_ValueForm(6,2,dh);
    CString sx     = Check_ValueForm(6,2,dx);
    CString sycent = Check_ValueForm(6,2,dycent);
    CString sfcent = Check_ValueForm(6,2,dfcent);
    CString sTfor  = Check_ValueForm(6,2,dTfor);
    CString sfsa   = Check_ValueForm(6,2,dfsa);
    CString sFS    = Check_ValueForm(6,2,dFS);
    CString sAsreq = Check_ValueForm(6,2,dAsreq);
    CString syten  = Check_ValueForm(6,2,dyten);
    CString sAc    = Check_ValueForm(6,2,dAc);


    CString sTop = Check_ValueForm(6,2,fabs(dftop));
    CString sBot = Check_ValueForm(6,2,fabs(dfbot));

    if(dftop>0.0 && dfbot>0.0) 
    {
        double dft=0.0, dfm=0.0;
        if(dftop>=dfbot)
        {
            strTbar.Format(_T("a == %s ~ %s"), sx, m_strLength);    m_arText.Add(strTbar);
            m_arText.Add(_T(""));
            strTbar = _T("Text: 인장력 T");            m_arText.Add(strTbar);
            strTbar = _T("Text:  -인장영역의 도심위치에서의 응력");     m_arText.Add(strTbar);
            m_arText.Add(_T(""));
            strTbar.Format(_T("~ f_t == f_top - f_bot == %s - %s ~ = %s ~ %s"),sftop, sfbot, sft, m_strStress); m_arText.Add(strTbar);
            strTbar.Format(_T("~ f_m == y_t * f_t / h == %s * %s / %s ~ = %s ~ %s"),syten, sft, sh, dfm, m_strStress);   m_arText.Add(strTbar);
            strTbar.Format(_T("~ T == ( f_bot + f_m ) * A_c == ( %s + %s ) * %s ~ = %s ~ %s"),sfbot, sfm, sAc, sTfor, m_strForce); m_arText.Add(strTbar);
            strTbar.Format(_T("~ A_sreq == T / f_sa == %s / %s ~ = %s ~ %s"),
                sTfor, sfsa, sAsreq, m_strArea);   m_arText.Add(strTbar);
        }
        else if(dfbot>dftop)
        {
            strTbar.Format(_T("a == %s ~ %s"), sx);   m_arText.Add(strTbar);
            m_arText.Add(_T(""));
            strTbar = _T("Text: 인장력 T");            m_arText.Add(strTbar);
            strTbar = _T("Text:  -인장영역의 도심위치에서의 응력");     m_arText.Add(strTbar);
            m_arText.Add(_T(""));
            strTbar.Format(_T("~ f_t == f_top - f_bot == %s - %s ~ = %s ~ %s"),sftop, sfbot, sft, m_strStress); m_arText.Add(strTbar);
            strTbar.Format(_T("~ f_m == y_t * f_t / h == %s * %s / %s ~ = %s ~ %s"),syten, sft, sh, sfm, m_strStress);   m_arText.Add(strTbar);
            strTbar.Format(_T("~ T == ( f_bot + f_m ) * A_c == ( %s + %s ) * %s ~ = %s ~ %s"),sfbot, sfm, sAc, sTfor, m_strForce); m_arText.Add(strTbar);
            strTbar.Format(_T("~ A_sreq == T / f_sa == %s / ( %s * %s ) ~ = %s ~ %s"),
                sTfor, sfsa, sAsreq, m_strArea);   m_arText.Add(strTbar);
        }
    }

    m_arText.Add(_T(""));
}

void CPrtKSCE_USD03::Print_BdcyTheory_Iter(CArray<CString,CString>& aString)
{
    CString strBdcy = _T("");
    CString strEqua = _T("");
    CString strSpace = _T("");
    for(int i=0; i<30; i++)
        strSpace += _T("~ ");

    strBdcy = _T("Text: ▷ 휨 강도 검토");                                   aString.Add(strBdcy);
    strBdcy = _T(""); aString.Add(strBdcy);
    strBdcy = _T("Text:    1) 중립축의 위치 c를 가정한다.");                 aString.Add(strBdcy);
    strBdcy = _T(""); aString.Add(strBdcy);
    strBdcy = _T("Text:    2) 등가 직사각형 응력블록의 깊이에 대한 콘크리트 압축력을 계산한다.");         aString.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ ~ Cc</Title> <Col>15</Col> </Equation>");
    strBdcy.Format(_T("%s  A_a : 압축연단에서`a까지의`넓이"),strEqua);                aString.Add(strBdcy);
    strBdcy = _T(""); aString.Add(strBdcy);

    strBdcy = _T("~ ~ 3)`PS`긴장재의`변형도 ε_ps 를`계산하고,`PS`긴장재의`응력`-`변형률`곡선으로부터");       aString.Add(strBdcy);
    strBdcy = _T("~ ~ ~ ~ f_ps 를`계산한다.");                             aString.Add(strBdcy);

    strEqua = _T("<Equation> <Title>~ ~ ~ epsi_cp</Title> <Col>10</Col> <Title>~ ~ epsi_pe</Title> <Col>20</Col> <Title>~ epsi_ce</Title> <Col>30</Col> </Equation>");  aString.Add(strEqua); 
    strBdcy = _T(""); aString.Add(strBdcy);
    strBdcy = _T(""); aString.Add(strBdcy);
    strBdcy = _T("~ ~ ~ d_pi : `압축연단에서`긴장재`그룹`도심까지의`거리 ");   aString.Add(strBdcy);
    strBdcy = _T("~ ~ ~ e_pi : `중립축에서`긴장재`그룹`도심까지의`거리");      aString.Add(strBdcy);
    strBdcy = _T(""); aString.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ ~ epsi_ps</Title> <Col>0</Col> </Equation>"); aString.Add(strEqua);
    strBdcy = _T(""); aString.Add(strBdcy);
    strBdcy = _T(""); aString.Add(strBdcy);

    strBdcy = _T("Text:   - PS 긴장재의 응력-변형도 곡선");                     aString.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ ~ epsi_py</Title> <Col>0</Col> </Equation>");   aString.Add(strEqua);
    strEqua = _T("<Equation> <Title>~ ~ ~ epsi_ps2</Title> <Col>10</Col> <Title>~ fps</Title> <Col>20</Col> </Equation>");  aString.Add(strEqua);
    strEqua = _T("<Equation> <Title>~ ~ ~ epsi_ps3</Title> <Col>9</Col> </Equation> Text: 일 때,"); aString.Add(strEqua);
    strBdcy = _T(" "); aString.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ ~ fpu</Title> <Col>20</Col> <Title>~ fps2</Title> <Col>30</Col> </Equation>");  aString.Add(strEqua);
    strBdcy = _T(""); aString.Add(strBdcy);
    strBdcy = _T(""); aString.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ ~ fpu2</Title> <Col>20</Col> <Title>~ fps3</Title> <Col>30</Col> </Equation>");  aString.Add(strEqua);
    strBdcy = _T(""); aString.Add(strBdcy);
    strBdcy = _T(""); aString.Add(strBdcy);

    strBdcy = _T("~ ~ 4)`PS`긴장재의`인장부재력 T_p 를`계산한다.");                           aString.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ ~ T_p</Title> <Col>0</Col> </Equation>");   aString.Add(strEqua);
    strBdcy = _T(""); aString.Add(strBdcy);

    strBdcy = _T("~ ~ 5)`인장보강철근의`응력 f_s 계산후`부재력 T_s 를`계산한다.");            aString.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ ~ epsi_s</Title> <Col>10</Col> <Title>~ fs</Title> <Col>20</Col> <Title>~ Ts</Title> <Col>30</Col> </Equation>");  aString.Add(strEqua); 
    strBdcy = _T(""); aString.Add(strBdcy);
    strBdcy = _T(""); aString.Add(strBdcy);

    strBdcy = _T("Text:    6) 힘의 평형상태를 검토한다");                                          aString.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ ~ Delta</Title> <Col>0</Col> </Equation>");   aString.Add(strEqua);
    strBdcy = _T(""); aString.Add(strBdcy);

    strBdcy = _T("Text:    7) |ΔC|>TOL 이면 중립축의 위치를 수정하고 2)부터 다시 반복한다.");     aString.Add(strBdcy);
    strBdcy = _T(""); aString.Add(strBdcy);

    strBdcy = _T("Text:    8) 평형조건이 만족되면 휨강도를 계산한다. (중립축을 기준으로 계산)");   aString.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ ~ Mn</Title> <Col>0</Col> </Equation>");   aString.Add(strEqua);
    strBdcy = _T("~ ~ ~ a_cent :`a부터`압축연단에서`a까지의`넓이의`도심까지의`거리");         aString.Add(strBdcy);
    strBdcy = _T(""); aString.Add(strBdcy);
    strBdcy = _T(""); aString.Add(strBdcy);

    //2 Page
    strBdcy = _T("Text: ▷ PS 강재의 제한");             aString.Add(strBdcy);
    strBdcy = _T(""); aString.Add(strBdcy);

    strBdcy = _T("Text:   - 최대 PS 강재량");             aString.Add(strBdcy);
    strBdcy = _T("Text:   ㆍ중립축이 플랜지내에 있는 단면");             aString.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ ~ qp_in</Title> <Col>30</Col> </Equation>");   
    strBdcy.Format(_T("%s q_p ~ : 강재지수"),strEqua);             aString.Add(strBdcy);
    strBdcy = strSpace + _T("β_1 :`등가`직사각형`응력분포의`깊이와");             aString.Add(strBdcy);
    strBdcy = strSpace + _T("~ ~ ~ ~ 중립축`거리와의`비");             aString.Add(strBdcy);
    strBdcy = _T(""); aString.Add(strBdcy);

    strBdcy = _T("Text:   ㆍ중립축이 플랜지내에 있지 않는 단면");             aString.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ ~ qp_out</Title> <Col>30</Col> </Equation>");  aString.Add(strEqua);
    strBdcy = _T("");   aString.Add(strBdcy);
    strBdcy = _T("");   aString.Add(strBdcy);


    strBdcy = _T("Text: ▷ 최소 PS 강재량");     aString.Add(strBdcy);
    strBdcy = _T(""); aString.Add(strBdcy);

    strEqua = _T("<Equation> <Title>~ ~ ~ b12Mcr</Title> <Col>30</Col> </Equation>");   aString.Add(strEqua);
    strEqua = _T("<Equation> <Title>~ ~ ~ bMcr</Title> <Col>30</Col> </Equation>"); 
    strBdcy.Format(_T("%s f_pe : ~ 손실이`일어난`후`강재의`유효프리스트레스"),strEqua);             aString.Add(strBdcy);
    //
    strEqua = _T("<Equation> <Title>~ ~ ~ fru_val</Title> <Col>30</Col> </Equation>"); 
    strBdcy.Format(_T("%s f_ru ~ : ~ 콘크리트의`허용 휨강도`(`파괴계수`)"),strEqua);                aString.Add(strBdcy);  
    aString.Add(_T(""));
    //
    strEqua = _T("<Equation> <Title>~ ~ ~ bMcr2</Title> <Col>30</Col> </Equation>"); 
    strBdcy.Format(_T("%s Z_c ~ ~ : ~ 외부`작용하중에`의해`인장응력이`발생하는"),strEqua);  aString.Add(strBdcy);  
    strBdcy.Format(_T("%s ~ ~ ~ ~ ~ ~ 단면의`최외연에`대한`합성단면계수"), strSpace);                   aString.Add(strBdcy);
    strBdcy = _T(""); aString.Add(strBdcy);
    //strBdcy = _T("~ ~ ~ Φ  M_n ≥ 2 M_u 일`경우에는`단면은`최소`강재량을`만족하지`않아도`상관이`없다.");             aString.Add(strBdcy);

}

void CPrtKSCE_USD03::Print_BdcyTheory_Code(CArray<CString,CString>& aString)
{
    CString strBdcy = _T("");
    CString strEqua = _T("");
    CString strSpace = _T("");
    for(int i=0; i<30; i++)
        strSpace += _T("~ ");

    strBdcy = _T("Text: ▷ 휨 강도 검토");                                          aString.Add(strBdcy);
    strBdcy = _T(""); aString.Add(strBdcy);
    strBdcy = _T("Text:  ○ PS 강재의 응력");                                       aString.Add(strBdcy);
    strBdcy = _T("Text:   - 부착된 부재");                                          aString.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ fps_bond1</Title> <Col>15</Col> </Equation>");
    strBdcy.Format(_T("%s  "),strEqua);                aString.Add(strBdcy);
    aString.Add(_T(""));
    aString.Add(_T(""));
    strBdcy = _T("Text:   ㆍ인장측에 철근을 배치하지 않은 경우");                                          aString.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ fps_bond2</Title> <Col>15</Col> </Equation>");
    strBdcy.Format(_T("%s  "),strEqua);                aString.Add(strBdcy);
    aString.Add(_T(""));
    aString.Add(_T(""));
    strBdcy = _T("Text:   - 부착되지 않은 부재");                                          aString.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ fps_unbond</Title> <Col>30</Col> </Equation>");
    strBdcy.Format(_T("%s  f_pe : 손실이`일어난`후`유효`프리스트레스"),strEqua);                aString.Add(strBdcy);
    aString.Add(_T(""));

    strBdcy = _T("Text:  ○ 등가 사각형의 응력블록의 깊이");                                       aString.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ a_block</Title> <Col>0</Col> </Equation>");     aString.Add(strEqua);
    aString.Add(_T(""));

    strBdcy = _T("Text:  ○ 설계 휨강도");                                 aString.Add(strBdcy);
    strBdcy = _T("Text:   - 압축측에 배치되는 철근은 항복여부에 따른 저항력 산정하여 고려할 수 있으나"); aString.Add(strBdcy);
    strBdcy = _T("Text:   - 본 검토에서 무시하기로 한다. 또한 압축측에 배치는 PS 강재도 마찬가지로 항복여부에 따른"); aString.Add(strBdcy);
    strBdcy = _T("Text:   - 저항력 산정 규정이 없어 고려하지 않는다."); aString.Add(strBdcy);
    aString.Add(_T(""));
    strBdcy = _T("Text:   - 사각형 단면");                                 aString.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ pMn_rec</Title> <Col>0</Col> </Equation>");     aString.Add(strEqua);
    aString.Add(_T(""));
    aString.Add(_T(""));
    strBdcy = _T("Text:    ㆍ인장측에 철근을 배치하지 않은 경우");                                          aString.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ pMn_rec_nobar</Title> <Col>0</Col> </Equation>");     aString.Add(strEqua);
    aString.Add(_T(""));
    aString.Add(_T(""));
    strBdcy = _T("Text:   - 플랜지를 갖는 단면");                                 aString.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ pMn_flg</Title> <Col>0</Col> </Equation>");     aString.Add(strEqua);
    aString.Add(_T(""));
    aString.Add(_T(""));  
    strEqua = _T("<Equation> <Title>~ ~ ~ ~ Apw1</Title> <Col>0</Col> </Equation>");     aString.Add(strEqua);
    strEqua = _T("<Equation> <Title>~ ~ ~ ~ Apw2</Title> <Col>0</Col> </Equation>");     aString.Add(strEqua);
    strEqua = _T("<Equation> <Title>~ ~ ~ ~ Apf</Title> <Col>0</Col> </Equation>");     aString.Add(strEqua);
    aString.Add(_T(""));

    //2 Page
    strBdcy = _T("Text: ▷ PS 강재의 제한");             aString.Add(strBdcy);
    aString.Add(_T(""));

    strBdcy = _T("Text:   - 최대 PS 강재량");             aString.Add(strBdcy);
    strBdcy = _T("Text:   ㆍ사각형 단면");             aString.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ ~ qp_in</Title> <Col>30</Col> </Equation>");   
    strBdcy.Format(_T("%s q_p ~ : 강재지수"),strEqua);             aString.Add(strBdcy);
    strBdcy = strSpace + _T("β_1 :`등가`직사각형`응력분포의`깊이와");             aString.Add(strBdcy);
    strBdcy = strSpace + _T("~ ~ ~ ~ 중립축`거리와의`비");             aString.Add(strBdcy);
    aString.Add(_T(""));

    strBdcy = _T("Text:   ㆍ플랜지를 갖는 단면");             aString.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ ~ qp_out</Title> <Col>30</Col> </Equation>");  aString.Add(strEqua);
    aString.Add(_T(""));
    aString.Add(_T(""));
    strBdcy = _T("~ ~ 강재지수가  0.36 β_1  보다`큰`부재에서는`설계`휨강도를`다음`값보다`크지`않게`한다");             aString.Add(strBdcy);
    strBdcy = _T("Text:   ㆍ사각형 단면");             aString.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ ~ pMn_qp_rec</Title> <Col>0</Col> </Equation>");  aString.Add(strEqua);
    aString.Add(_T(""));
    strBdcy = _T("Text:   ㆍ플랜지를 갖는 단면");             aString.Add(strBdcy);  
    strEqua = _T("<Equation> <Title>~ ~ ~ pMn_qp_flg</Title> <Col>0</Col> </Equation>");  aString.Add(strEqua);
    aString.Add(_T(""));

    strBdcy = _T("Text: ▷ 최소 PS 강재량");     aString.Add(strBdcy);
    strBdcy = _T(""); aString.Add(strBdcy);

    strEqua = _T("<Equation> <Title>~ ~ ~ b12Mcr</Title> <Col>30</Col> </Equation>");   aString.Add(strEqua);
    strEqua = _T("<Equation> <Title>~ ~ ~ bMcr</Title> <Col>30</Col> </Equation>"); 
    //strBdcy.Format(_T("%s f_pcc : ~ 손실이`일어난`후`강재의`유효프리스트레스"),strEqua);             aString.Add(strBdcy);
    strBdcy.Format(_T("%s f_pe : ~ 외력에`의해`인장응력이`발생하는"),strEqua);             aString.Add(strBdcy);
    strBdcy.Format(_T("%s ~ ~ ~ ~ ~ 단면의`최연단에서`콘크리트의`압축응력"), strSpace);               aString.Add(strBdcy);
    //
    strEqua = _T("<Equation> <Title>~ ~ ~ fru_val</Title> <Col>30</Col> </Equation>"); 
    strBdcy.Format(_T("%s f_ru : ~ 콘크리트의`허용 휨강도`(`파괴계수`)"),strEqua);                aString.Add(strBdcy);  
    aString.Add(_T(""));
    //
    strEqua = _T("<Equation> <Title>~ ~ ~ bMcr2</Title> <Col>30</Col> </Equation>"); 
    strBdcy.Format(_T("%s Z_c ~ : ~ 외부`작용하중에`의해`인장응력이`발생하는"),strEqua);  aString.Add(strBdcy);  
    strBdcy.Format(_T("%s ~ ~ ~ ~ ~ 단면의`최외연에`대한`합성단면계수"), strSpace);                   aString.Add(strBdcy);
    aString.Add(_T(""));
    strBdcy = _T("~ ~ ~ Φ  M_n ≥  4/3  M_u 일`경우에는`단면은`최소`강재량을`만족하지`않아도`상관이`없다.");             aString.Add(strBdcy);

}

void CPrtKSCE_USD03::Print_ElementIJ(T_ELEM_K ElemK, BOOL bPosiI)
{
    CString strBdcy;
    CString strIJ;
    strIJ = ( bPosiI==TRUE ? _T("I") : _T("J"));
    strBdcy.Format(_T("Element : ~ %d ~ ~ Position : %s"),ElemK, strIJ);  
    m_arText.Add(strBdcy);
}

void CPrtKSCE_USD03::Print_ElementIJ(T_ELEM_K ElemK, BOOL bPosiI, BOOL bPositive)
{
    CString strBdcy;
    CString strIJ;
    strIJ = ( bPosiI==TRUE ? _T("I") : _T("J")); 
    if(bPositive) strBdcy.Format(_T("Element : ~ %d ~ ~ Position : %s ~ ~ (정모멘트)"),ElemK, strIJ);  
    else          strBdcy.Format(_T("Element : ~ %d ~ ~ Position : %s ~ ~ (부모멘트)"),ElemK, strIJ);  
    m_arText.Add(strBdcy);
}

void CPrtKSCE_USD03::Print_TbarElementIJ(T_ELEM_K ElemK, BOOL bPosiI, BOOL bTopBot)
{
    CString strTbar;
    CString strIJ;
    strIJ = ( bPosiI==TRUE ? _T("I") : _T("J")); 
    if(bTopBot) strTbar.Format(_T("Element : ~ %d ~ ~ Position : %s ~ ~ (Top)"),ElemK, strIJ);  
    else        strTbar.Format(_T("Element : ~ %d ~ ~ Position : %s ~ ~ (Bottom)"),ElemK, strIJ);  
    m_arText.Add(strTbar);
}

void CPrtKSCE_USD03::Print_BdcyDesign0(BOOL bApwM)
{
    CString sCount; sCount.Format(_T("%d"), m_BdcyParam.iCount);
    CString sfck = Check_ValueForm(6,1, m_BdcyParam.dfck);
    CString sbw  = Check_ValueForm(7,1, m_BdcyParam.dbw);
    CString sb   = Check_ValueForm(7,1, m_BdcyParam.db);
    CString sAps = Check_ValueForm(7,1, m_BdcyParam.dAps);
    CString sfy  = Check_ValueForm(6,1, m_BdcyParam.dfy);
    CString sdp  = Check_ValueForm(7,1, m_BdcyParam.ddp);
    CString sAst = Check_ValueForm(7,1, m_BdcyParam.dAs);

    CString sfpu = Check_ValueForm(7,1, m_BdcyParam.dfpu);
    CString sd   = Check_ValueForm(7,1, m_BdcyParam.dd);
    CString sAc  = Check_ValueForm(9,3, m_BdcyParam.dAc);
    CString sIy  = Check_ValueForm(9,3, m_BdcyParam.dIy);
    CString syt  = Check_ValueForm(7,1, m_BdcyParam.dyb);
    CString sPe  = Check_ValueForm(9,3, Cng_CodeForFix(m_BdcyParam.dPe));

    CString sbeta1 = Check_ValueForm(6,3, m_BdcyParam.dbeta1);
    CString sep    = Check_ValueForm(7,1, m_BdcyParam.dep);
    CString sMu    = Check_ValueForm(8,1, Cng_CodeMomFix(m_BdcyParam.dMu));
    CString srp    = Check_ValueForm(6,2, m_BdcyParam.drp);
    CString shf    = Check_ValueForm(7,1, m_BdcyParam.dhf);

    CString sSpace4 = _T("~ ~ ~ ~ ");
    CString sSpace5 = _T("~ ~ ~ ~ ~ ");
    CString sSpace6 = _T("~ ~ ~ ~ ~ ~ ");
    CString sSpace7 = _T("~ ~ ~ ~ ~ ~ ~ ");
    CString sSpace8 = _T("~ ~ ~ ~ ~ ~ ~ ~ ");
    CString sSpace9 = _T("~ ~ ~ ~ ~ ~ ~ ~ ~ ");
    CString s1stSpace  = _T("");
    CString s2ndSpace  = _T("");

    int iMaxLength = 0;
    int arLength[3][6];

    int isfck   = arLength[0][0] = sfck.GetLength();
    int isfy    = arLength[0][1] = sfy.GetLength();
    int isfpu   = arLength[0][2] = sfpu.GetLength();
    int isbeta1 = arLength[0][3] = sbeta1.GetLength();
    int irp     = arLength[0][4] = srp.GetLength();
    int isbw    = arLength[0][5] = sbw.GetLength();  

    int isb    = arLength[1][0] = sb.GetLength();
    int isdp   = arLength[1][1] = sdp.GetLength();
    int isd    = arLength[1][2] = sd.GetLength();
    int isyt   = arLength[1][3] = syt.GetLength();
    int isep   = arLength[1][4] = sep.GetLength();
    int ishf   = arLength[1][5] = shf.GetLength();  

    int isAps   = arLength[2][0] = sAps.GetLength();
    int isAst   = arLength[2][1] = sAst.GetLength();
    int isAc    = arLength[2][2] = sAc.GetLength();
    int isIy    = arLength[2][3] = sIy.GetLength();
    int isPe    = arLength[2][4] = sPe.GetLength();
    int isMu    = arLength[2][5] = sMu.GetLength();  

    // 1 Column.    
    for(int i=0; i<5; i++)  
        iMaxLength = max(iMaxLength,arLength[0][i]);  

    CString strfck, strfy, strfpu, strbeta1, strrp, strbw;
    strfck  = Get_TextStringLength(iMaxLength, sfck);
    strfy   = Get_TextStringLength(iMaxLength, sfy);
    strfpu  = Get_TextStringLength(iMaxLength, sfpu);
    strbeta1= Get_TextStringLength(iMaxLength, sbeta1);  strbeta1 += sSpace5;
    strrp   = Get_TextStringLength(iMaxLength, srp);     strrp += sSpace5;
    strbw   = Get_TextStringLength(iMaxLength, sbw);

    if(iMaxLength==10 || iMaxLength==11)    s1stSpace = sSpace4;
    else if(iMaxLength==9 || iMaxLength==8) s1stSpace = sSpace5;
    else if(iMaxLength==7)                  s1stSpace = sSpace6;
    else if(iMaxLength==6 || iMaxLength==5) s1stSpace = sSpace7;
    else if(iMaxLength==4 || iMaxLength==3) s1stSpace = sSpace8;
    else if(iMaxLength==2)                  s1stSpace = sSpace9;
    else ASSERT(0);

    CString s1colLength = _T("");
    for(int i=0; i<Get_XlLength(iMaxLength); i++)
        s1colLength += _T("~ ");

    s1colLength  = s1colLength + sSpace8 + sSpace5 + s1stSpace;

    // 2 Column.
    iMaxLength = 0;
    for(int i=0; i<6; i++)  
        iMaxLength = max(iMaxLength,arLength[1][i]);

    CString strb, strdp, strd, stryt, strep, strhf;
    strb  = Get_TextStringLength(iMaxLength, sb  );
    strdp = Get_TextStringLength(iMaxLength, sdp );
    strd  = Get_TextStringLength(iMaxLength, sd  );
    stryt = Get_TextStringLength(iMaxLength, syt );  
    strep = Get_TextStringLength(iMaxLength, sep );  
    strhf = Get_TextStringLength(iMaxLength, shf );  

    if(iMaxLength==10 || iMaxLength==11)    s2ndSpace = sSpace4;
    else if(iMaxLength==9 || iMaxLength==8) s2ndSpace = sSpace5;
    else if(iMaxLength==7)                  s2ndSpace = sSpace6;
    else if(iMaxLength==6 || iMaxLength==5) s2ndSpace = sSpace7;
    else if(iMaxLength==4 || iMaxLength==3) s2ndSpace = sSpace8;
    else if(iMaxLength==2)                  s2ndSpace = sSpace9;

    // 3 Column.
    iMaxLength = 0;
    for(int i=0; i<6; i++)  
        iMaxLength = max(iMaxLength,arLength[2][i]);

    CString strAps, strAst, strAc, strIy, strPe, strMu;
    strAps = Get_TextStringLength(iMaxLength, sAps  );
    strAst = Get_TextStringLength(iMaxLength, sAst );
    strAc  = Get_TextStringLength(iMaxLength, sAc  );
    strIy  = Get_TextStringLength(iMaxLength, sIy );  
    strPe  = Get_TextStringLength(iMaxLength, sPe );  
    strMu  = Get_TextStringLength(iMaxLength, sMu );  

    if(sIy.GetLength()>=9)  strIy.TrimRight(_T("~ "));
    if(sAc.GetLength()>=9)  strAc.TrimRight(_T("~ "));
    if(sPe.GetLength()>=9)  strPe.TrimRight(_T("~ "));
    if(sMu.GetLength()>=9)  strPe.TrimRight(_T("~ "));

    // N/mm2 = 4col
    CString strBdcy;  // ①
    if(bApwM) 
    {
        strBdcy = _T("Text: Apw<0 으로 계산되어 fps를 상세계산 합니다.");  m_arText.Add(strBdcy);
        m_arText.Add(_T(""));
    }
#if defined(_DEBUG)
    strBdcy.Format(_T("%s   %s"), _T("Text: (1) 설계 변수"), sCount);                                m_arText.Add(strBdcy);
#else
    strBdcy = _T("Text: (1) 설계 변수");                                                         m_arText.Add(strBdcy);
#endif
    strBdcy.Format(_T("~ ~ f_ck : ~ %s %s %s b ~ ~ : ~ %s %s %s A_p ~ : ~ %s%s"),
        strfck, m_sTxStress, s1stSpace , strb, m_strLength, s2ndSpace, strAps, m_sTxArea);                    m_arText.Add(strBdcy);
    strBdcy.Format(_T("~ ~ f_y ~ : ~ %s %s %s d_p ~ : ~ %s %s %s A_s ~ : ~ %s%s"),
        strfy, m_sTxStress, s1stSpace, strdp, m_strLength, s2ndSpace, strAst, m_sTxArea);             m_arText.Add(strBdcy);

    strBdcy.Format(_T("~ ~ f_pu : ~ %s %s %s d ~ ~ : ~ %s %s %s A_c ~ : ~ %s %s"),
        strfpu, m_sTxStress, s1stSpace, strd, m_strLength, s2ndSpace, strAc, m_sTxArea);    m_arText.Add(strBdcy);
    strBdcy.Format(_T("~ ~ β_1 : ~ %s %s y_t ~ : ~ %s %s %s I_y ~ : ~ %s %s"),
        strbeta1, s1stSpace, stryt, m_strLength, s2ndSpace, strIy, m_sTxStif);     m_arText.Add(strBdcy);

    strBdcy.Format(_T("~ ~ γ_p : ~ %s %s e_p ~ : ~ %s %s %s P_e ~ : ~ %s %s"),
        strrp, s1stSpace, strep, m_strLength, s2ndSpace, strPe, m_strForce);    m_arText.Add(strBdcy);
    strBdcy.Format(_T("~ ~ b_0 ~ : ~ %s %s ~ ~ ~ ~ %s t ~ ~ : ~ %s %s %s M_u ~ : ~ %s %s"), 
        strbw, m_strLength, s1stSpace, strhf, m_strLength, s2ndSpace, strMu, m_strMoment);          m_arText.Add(strBdcy);
    strBdcy = _T(""); m_arText.Add(strBdcy);

}

CString CPrtKSCE_USD03::Get_TextStringLength(int iMaxLength, CString strText)
{
    CString str = strText;
    int iMaxStrLength = Get_XlLength(iMaxLength);
    int iLength = strText.GetLength();
    int iStrLength = Get_XlLength(iLength);

    str += _T(" ");
    int iCount = iMaxStrLength - iStrLength;
    for(int i=0; i<iCount; i++)
    {
        str += _T("~ ");
    }

    str += _T("~ ");

    if(strText.GetLength()<=3) str += _T("~ ");

    return str;
}

int CPrtKSCE_USD03::Get_XlLength(int iLength)
{
    int iLen=0;
    if(iLength==10 || iLength==11)    iLen = 7;
    else if(iLength==9 || iLength==8) iLen = 6;
    else if(iLength==7)               iLen = 5;
    else if(iLength==6 || iLength==5) iLen = 4;
    else if(iLength==4 || iLength==3) iLen = 3;
    else if(iLength==2)               iLen = 2;

    return iLen;
}

void CPrtKSCE_USD03::Print_BdcyDesign1(double dfck, double db, double dAps , double dfy, double ddp, double dAst)
{
    CString sfck = Check_ValueForm(6,1, dfck);
    CString sb   = Check_ValueForm(6,1, db);
    CString sAps = Check_ValueForm(6,1, dAps);
    CString sfy  = Check_ValueForm(6,1, dfy);
    CString sdp  = Check_ValueForm(6,1, ddp);
    CString sAst = Check_ValueForm(6,1, dAst);

    CString strBdcy;

    //설계 변수.
    strBdcy = _T("Text: ① 설계 변수");                                                         m_arText.Add(strBdcy);
    strBdcy.Format(_T("~ ~ f_ck : ~ %s ~ ~ ~ %s ~ ~ ~ ~ ~ b ~ : ~ %s ~ %s ~ ~ ~ ~ ~ ~ A_ps : ~ %s ~ ~ ~ %s"),
        sfck, m_sTxStress, sb , m_strLength, sAps, m_sTxArea);                    m_arText.Add(strBdcy);
    strBdcy.Format(_T("~ ~ f_y ~ : ~ %s ~ ~ ~ %s ~ ~ ~ ~ ~ d_p : ~ %s ~ %s ~ ~ ~ ~ ~ ~ A_s ~ : ~ %s ~ ~ ~ %s"),
        sfy, m_sTxStress, sdp, m_strLength, sAst, m_sTxArea);             m_arText.Add(strBdcy);
}

void CPrtKSCE_USD03::Print_BdcyDesign2(double dfpu, double dd, double dAc , double dIy, double dyt, double dPe)
{  
    CString sfpu = Check_ValueForm(7,1, dfpu);
    CString sd   = Check_ValueForm(6,1, dd);
    CString sAc  = Check_ValueForm(6,1, dAc);
    CString sIy  = Check_ValueForm(9,3, dIy);
    CString syt  = Check_ValueForm(7,1, dyt);
    CString sPe  = Check_ValueForm(9,3, Cng_CodeForFix(dPe));

    CString strBdcy;
    strBdcy.Format(_T("~ ~ f_pu : ~ %s ~ ~ %s ~ ~ ~ ~ ~ d ~ : ~ %s ~ %s ~ ~ ~ ~ ~ ~ A_c ~ : ~ %s ~ ~ %s"),
        sfpu, m_sTxStress, sd, m_strLength, sAc, m_sTxArea);    m_arText.Add(strBdcy);
    strBdcy.Format(_T("~ ~ I_y ~ : ~ %s %s ~ ~ ~ ~ ~ ~ ~ ~ y_b : ~ %s ~ %s ~ ~ ~ ~ ~ ~ P_e ~ : ~ %s %s"),
        sIy, m_sTxStif, syt, m_strLength, sPe, m_strForce);     m_arText.Add(strBdcy);
}

void CPrtKSCE_USD03::Print_BdcyDesign3(double dbeta1, double dep, double dMu , double drp, double dhf)
{
    CString sbeta1 = Check_ValueForm(6,3, dbeta1);
    CString sep    = Check_ValueForm(7,1, dep);
    CString sMu    = Check_ValueForm(8,1, Cng_CodeMomFix(dMu));
    CString srp    = Check_ValueForm(6,2, drp);
    CString shf    = Check_ValueForm(7,1, dhf);

    CString strBdcy;
    strBdcy.Format(_T("~ ~ β_1 : ~ %s  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ e_p : ~ %s ~ %s ~ ~ ~ ~ ~ ~ M_u ~ : ~ %s ~ ~ ~ %s"),
        sbeta1, sep, m_strLength, sMu, m_strMoment);    m_arText.Add(strBdcy);
    strBdcy.Format(_T("~ ~ γ_p : ~ %s ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ t ~ : ~ %s ~ %s"), srp, shf, m_strLength);          m_arText.Add(strBdcy);
    strBdcy = _T(""); m_arText.Add(strBdcy);
}

void CPrtKSCE_USD03::Print_BdcyDesign4(double dc, double dbeta1, double da , double dfck, double dAa, double dCc)
{
    CString sc     = Check_ValueForm(6,1, dc);
    CString sbeta1 = Check_ValueForm(6,3, dbeta1);
    CString sa     = Check_ValueForm(6,1, da);
    CString sfck   = Check_ValueForm(6,1, dfck);
    CString sAa    = Check_ValueForm(8,1, dAa);
    CString sCc    = Check_ValueForm(6,1, Cng_CodeForFix(dCc));

    CString strBdcy;
    CString strEqua;
    strBdcy = _T("Text: (2) 힘의 평형 조건으로  중립축 c 결정");       m_arText.Add(strBdcy);
    strBdcy.Format(_T("~ ~ c ~ == ~ %s %s"), sc, m_strLength);                                           m_arText.Add(strBdcy);
    m_arText.Add(_T(""));

    strBdcy = _T("Text: (3) 등가 직사각형 응력블록 깊이");       m_arText.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ Equal_a</Title> <Col>6</Col> </Equation>");
    m_iFormula++;
    strBdcy.Format(_T("%s ~ == ~ %s * %s = %s ~ %s"),strEqua, sbeta1, sc, sa, m_strLength);          m_arText.Add(strBdcy);  
    m_arText.Add(_T(""));

    strBdcy = _T("Text: (4) 등가 직사각형 응력블록 깊이의 압축력");       m_arText.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ Cc</Title> <Col>10</Col> </Equation>");
    m_iFormula++;
    strBdcy.Format(_T("%s ~ == 0.85 * %s * %s / 10^3 ~ = %s ~ %s"),strEqua, sfck, sAa , sCc, m_strForce);   m_arText.Add(strBdcy);
    strBdcy = _T(""); m_arText.Add(strBdcy);

}

void CPrtKSCE_USD03::Print_BdcyDesign5(CArray<CString,CString>& strTndn, CArray<double,double>* aData, double dTp, double dTpTimesep, double dPeTimesep)
{
    int i=0, j=0;
    int TndnNum = aData[0].GetSize();

    int iCol = 8;
    CArray<CString,CString> sData[9]; 
    for(i=0; i<iCol; i++)
        sData[i].RemoveAll();

    for(i=0; i<TndnNum; i++)
    {
        CString strTemp;
        strTemp = Check_ValueForm(6,1, Cng_CodeForFix(aData[0].GetAt(i)) );  sData[0].Add(strTemp); // Pei
        strTemp = Check_ValueForm(6,1, aData[1].GetAt(i));               sData[1].Add(strTemp); // Apsi
        strTemp = Check_ValueForm(6,1, aData[2].GetAt(i));               sData[2].Add(strTemp); // dpi
        strTemp = Check_ValueForm(6,1, aData[3].GetAt(i));               sData[3].Add(strTemp); // epi
        strTemp = Check_ValueForm(6,4, aData[4].GetAt(i));               sData[4].Add(strTemp); // epsi_ps
        strTemp = Check_ValueForm(7,1, aData[5].GetAt(i));               sData[5].Add(strTemp); // fps
        strTemp = Check_ValueForm(6,1, Cng_CodeForFix(aData[6].GetAt(i)) );  sData[6].Add(strTemp); // Tpi
        strTemp = Check_ValueForm(6,1, Cng_CodeMomFix(aData[7].GetAt(i)) );  sData[7].Add(strTemp); // Tpi*epi
        strTemp = Check_ValueForm(6,1, Cng_CodeMomFix(aData[8].GetAt(i)) );  sData[8].Add(strTemp); // Pei*epi        
    }
    CString sTp         = Check_ValueForm(8,1, Cng_CodeForFix(dTp));
    CString sTpTimesep  = Check_ValueForm(8,1, Cng_CodeMomFix(dTpTimesep));
    CString sPeTimesep  = Check_ValueForm(8,1, Cng_CodeMomFix(dPeTimesep));

    CString strBdcy;
    CString strEqua;
    CString str1, str2, str3;

    strBdcy = _T("<Table>(5) PS 강재의 응력");    m_arText.Add(strBdcy);
    str1 = _T("<MH>TnGroup ~ ~ ~ ~ ~ ~</MH>   <MH>Pei ~ ~ ~ ~ ~</MH>   <MH>Apsi ~ ~ ~ ~ ~</MH>   ");
    str2 = _T("<MH>dpi ~ ~ ~ ~ ~</MH>   <MH>epi ~ ~ ~ ~ ~</MH>   <MH>εps ~ ~ ~ ~ ~</MH>   <MH>fpsi ~ ~ ~ ~ ~</MH>   ");
    str3 = _T("<MH>Apsi·fpsi ~ ~ ~ ~ ~</MH>   <MH>Tpi·epi ~ ~ ~ ~ ~</MH>   ");
    strBdcy = str1 + str2 + str3; m_arText.Add(strBdcy);
    if(TndnNum>0)
    {
        for( i=0; i<TndnNum; i++)
        {
            str1.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   "),strTndn.GetAt(i) ,sData[0].GetAt(i),sData[1].GetAt(i) ); 
            str2.Format(_T("<MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   "),
                sData[2].GetAt(i), sData[3].GetAt(i), sData[4].GetAt(i), sData[5].GetAt(i));
            str3.Format(_T("<MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   "),
                sData[6].GetAt(i), sData[7].GetAt(i) ); 
            strBdcy = str1 + str2 + str3;  m_arText.Add(strBdcy);
        }
    }
    else
    {
        str1.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   "),_T("-") ,_T("-"),_T("-") ); 
        str2.Format(_T("<MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   "),
            _T("-"), _T("-"), _T("-"), _T("-"));
        str3.Format(_T("<MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   "),
            _T("-"), _T("-") );
        strBdcy = str1 + str2 + str3;  m_arText.Add(strBdcy);                      
    }

    strBdcy = _T("</Table>");  m_arText.Add(strBdcy);

    strEqua = _T("<Equation> <Title>~ ~ T_p</Title> <Col>10</Col> </Equation>");
    strBdcy.Format(_T("%s ~ == ~ %s ~ %s"),strEqua, sTp, m_strForce);             m_arText.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ Tpep</Title> <Col>10</Col> </Equation>");
    strBdcy.Format(_T("%s ~ == ~ %s ~ %s"),strEqua, sTpTimesep, m_strMoment);      m_arText.Add(strBdcy);
    //strEqua = _T("<Equation> <Title>~ ~ Peep</Title> <Col>10</Col> </Equation>");
    //strBdcy.Format(_T("%s ~ == ~ %s ~ %s"),strEqua, sPeTimesep, m_strMoment);      m_arText.Add(strBdcy);

}

void CPrtKSCE_USD03::Print_BdcyDesign6(double dep_cu, double ddst, double dc , double dep_s, double dfs, double dfs_cal, double dEs, double dfy,
                                       double dAst, double dTs)
{
    CString sep_cu = Check_ValueForm(6,4, dep_cu);
    CString sdst   = Check_ValueForm(6,1, ddst);
    CString sc     = Check_ValueForm(6,1, dc);
    CString sep_s  = Check_ValueForm(6,4, dep_s);
    CString sfs    = Check_ValueForm(6,1, dfs);
    CString sfs_cal= Check_ValueForm(6,1, dfs_cal);
    CString sEs    = Check_ValueForm(6,1, dEs);
    CString sfy    = Check_ValueForm(6,1, dfy);
    CString sAst   = Check_ValueForm(6,1, dAst);
    CString sTs    = Check_ValueForm(6,1, Cng_CodeForFix(dTs));

    CString strBdcy;
    CString strEqua;

    strBdcy = _T("Text: (6) 인장철근의 인장응력");       m_arText.Add(strBdcy);
    if(dAst==0.0)
    {
        strBdcy.Format(_T("~ ~ T_s == A_s ·  f_s == %s * %s / 10^3 = %s ~ %s"), sAst, sfs, sTs, m_strForce);     m_arText.Add(strBdcy);
    }
    else
    {
        strEqua = _T("<Equation> <Title>~ ~ epsi_s</Title> <Col>10</Col> </Equation>");      m_arText.Add(strEqua);
        m_iFormula++;
        strBdcy = _T("");   m_arText.Add(strBdcy);
        strBdcy.Format(_T("~ ~ ~ == %s * ( %s - %s ) / %s = %s"), sep_cu, sdst, sc, sc, sep_s);    m_arText.Add(strBdcy);
        strBdcy = _T("");   m_arText.Add(strBdcy);

        strBdcy = _T("<Equation> <Title>~ ~ fs</Title> <Col>0</Col> </Equation>");    m_arText.Add(strBdcy);
        m_iFormula++;
        strBdcy = Get_fsPrint(dfs_cal, dep_s, dEs, dfy);                          m_arText.Add(strBdcy);
        strBdcy = _T("");   m_arText.Add(strBdcy);

        strBdcy.Format(_T("~ ~ T_s == A_s ·  f_s == %s * %s / 10^3 = %s ~ %s"), sAst, sfs, sTs, m_strForce);     m_arText.Add(strBdcy);
    }
    m_arText.Add(_T(""));

}

void CPrtKSCE_USD03::Print_BdcyDesign7(double dTp, double dTs, double dCc , double dDelC, double dTol)
{
    CString strBdcy;
    CString strEqua;

    strEqua = _T("<Equation> <Title>~ ~ Delta</Title> <Col>0</Col> </Equation>");    m_arText.Add(strEqua);
    m_iFormula++;
    strBdcy = Get_DeltaCPrint(dTp, dTs, dCc, dDelC, dTol);      m_arText.Add(strBdcy);
    strBdcy = _T(""); m_arText.Add(strBdcy);
}

void CPrtKSCE_USD03::Print_BdcyDesign8(double dCc, double dc, double da , double da_cent, double dMps, double dTs, 
                                       double ddst, double dMn, double dpMn, double dMu)
{
    CString sCc     = Check_ValueForm(7,1, Cng_CodeForFix(dCc));
    CString sc      = Check_ValueForm(6,1, dc);
    CString sa      = Check_ValueForm(6,1, da);
    CString sa_cent = Check_ValueForm(6,1, da_cent);
    CString sMps    = Check_ValueForm(7,1, Cng_CodeMomFix(dMps));
    CString sTs     = Check_ValueForm(6,1, Cng_CodeForFix(dTs));
    CString sdst    = Check_ValueForm(6,1, ddst);
    CString sMn    = Check_ValueForm(6,1, Cng_CodeMomFix(dMn));
    CString sMu     = Check_ValueForm(6,1, Cng_CodeMomFix(dMu));

    CString strBdcy;
    CString strEqua;

    strBdcy = _T("Text: (7) 휨강도");         m_arText.Add(strBdcy); //③
    //Insert Eq.
    strEqua = _T("<Equation> <Title>~ Mn</Title> <Col>0</Col> </Equation>");    m_arText.Add(strEqua);
    m_iFormula++;
    strBdcy.Format(_T("~ ~ ~ == %s * ( %s - ( %s - %s ) ) / 100 + %s + %s * ( %s - %s ) / 100 ~ = %s ~ %s"),
        sCc, sc, sa, sa_cent, sMps, sTs, sdst, sc, sMn, m_strMoment);  m_arText.Add(strBdcy);
    strBdcy = _T(""); m_arText.Add(strBdcy);
    strBdcy = Get_pMnPrint(dpMn, dMu);      m_arText.Add(strBdcy);
    strBdcy = _T(""); m_arText.Add(strBdcy);

}

void CPrtKSCE_USD03::Print_BdcyDesign9(double da, double dt, double dRhop, double dfps, double dfck , double dqp, double d036Beta1)
{
    CString sa = Check_ValueForm(6,1, da);
    CString st = Check_ValueForm(6,1, dt);

    CString strBdcy;
    CString strEqua;

    //최대 강재량.
    strBdcy = _T("Text: (8) 최대 PS강재량");         m_arText.Add(strBdcy); //④
    strBdcy.Format(_T("~ a == %s %s ~ < ~ t == %s %s"),sa, m_strLength, st, m_strLength);     m_arText.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ qp_in</Title> <Col>0</Col> </Equation>");               m_arText.Add(strEqua);
    m_iFormula++;
    m_arText.Add(_T(""));
    strBdcy = Get_MaxPsPrint(dRhop, dfps, dfck, dqp, d036Beta1);   m_arText.Add(strBdcy);
    m_arText.Add(_T(""));
}

void CPrtKSCE_USD03::Print_BdcyDesign91(double da, double dt, double dApw, double dAp, double dApf, double dAs, double db, double dbw, double ddp, double dfck, double dfps, double dfy, double dqp, double d036beta1)
{
    CString sa   = Check_ValueForm(7,1, da);
    CString st   = Check_ValueForm(7,1,dt);
    CString sApw = Check_ValueForm(7,1,dApw);
    CString sAp  = Check_ValueForm(7,1,dAp);
    CString sApf = Check_ValueForm(7,1,dApf);
    CString sAs  = Check_ValueForm(7,1,dAs);
    CString sb   = Check_ValueForm(7,1,db);
    CString sbw  = Check_ValueForm(7,1,dbw);
    CString sdp  = Check_ValueForm(7,1,ddp);
    CString sfck = Check_ValueForm(7,1,dfck);
    CString sfps = Check_ValueForm(7,1,dfps);
    CString sfy  = Check_ValueForm(7,1,dfy);
    CString sqp  = Check_ValueForm(7,5,dqp);
    CString s036beta1 = Check_ValueForm(6,4,d036beta1);

    CString strBdcy;
    CString strEqua;

    //최대 강재량.
    strBdcy = _T("Text: (8) 최대 PS강재량");         m_arText.Add(strBdcy); // ④
    strBdcy.Format(_T("~ a == %s %s ~ > ~ t == %s %s"),sa, m_strLength, st, m_strLength);  m_arText.Add(strBdcy);

    strEqua = _T("<Equation> <Title>~ Apf</Title> <Col>0</Col> </Equation>");             m_arText.Add(strEqua);  
    m_iFormula++;
    strBdcy.Format(_T("~ ~ ~ == 0.85 * %s * ( %s - %s ) * %s / %s = %s ~ %s"),
        sfck, sb, sbw, st, sfps, sApf, m_sTxArea);                        m_arText.Add(strBdcy);

    if(dAs>0.0)
    {
        strEqua = _T("<Equation> <Title>~ Apw2</Title> <Col>0</Col> </Equation>");          m_arText.Add(strEqua);
        m_iFormula++;
        strBdcy.Format(_T("~ ~ ~ == %s + ( %s * %s / %s ) - %s = %s ~ %s"),
            sAp, sAs, sfy, sfps, sApf, sApw, m_sTxArea);                     m_arText.Add(strBdcy);
    }
    else if(dAs==0.0)
    {
        strEqua = _T("<Equation> <Title>~ Apw1</Title> <Col>9</Col> </Equation>");       
        m_iFormula++;
        strBdcy.Format(_T("%s ~ == %s - %s = %s ~ %s"), strEqua, sAp, sApf, sApw, m_sTxArea);   m_arText.Add(strBdcy);
    }
    m_arText.Add(_T(""));

    CString sUnequal = _T(""), sJudge = _T("");
    if(dqp <= d036beta1) { sUnequal=_T("≤"); sJudge=_T("O.K"); }
    else                 { sUnequal=_T("＞"); sJudge=_T("N.G"); }
    strEqua = _T("<Equation> <Title>~ qp_out</Title> <Col>0</Col> </Equation>");   m_arText.Add(strEqua);
    m_iFormula++;
    m_arText.Add(_T(""));
    strBdcy.Format(_T("~ ~ ~ == %s / ( %s * %s ) * %s / %s = %s ~ %s  ~ %s ~ ~ ~ ~ ~ ~ ~ %s"),
        sApw, sb, sbw , sfps, sfck, sqp , sUnequal, s036beta1, sJudge);  m_arText.Add(strBdcy);
    m_arText.Add(_T(""));

}

void CPrtKSCE_USD03::Print_BdcyDesign10(BOOL bIter, double dPe, double dAc, double dPeTimesep , double dyt, double dIy, double dfpe, 
                                        double dfck, double dpMn, double d12Mcr, double dMu)
{
    CString sPe;        sPe.Format(_T("%.1lf"),dPe); // Check_ValueForm(9,1, dPe);
    CString sAc;        sAc.Format(_T("%.1lf"),dAc); // Check_ValueForm(9,1, dAc);
    CString sPeTimesep; sPeTimesep.Format(_T("%.1lf"),dPeTimesep); // Check_ValueForm(9,1, dPeTimesep);
    CString syt        = Check_ValueForm(7,1, dyt);
    CString sIy;        sIy.Format(_T("%.1lf"),dIy); // Check_ValueForm(9,3, dIy);
    CString sfpe       = Check_ValueForm(6,1, dfpe);
    CString sfck       = Check_ValueForm(6,1, dfck);
    CString spMn       = Check_ValueForm(6,1, Cng_CodeMomFix(dpMn));
    CString s12Mcr     = Check_ValueForm(6,1, Cng_CodeMomFix(d12Mcr));
    CString sMu        = Check_ValueForm(6,1, Cng_CodeMomFix(dMu));

    CString strBdcy;
    CString strEqua;

    //최소 강재량
    if(bIter)
    {
        strBdcy = _T("Text: (9) 최소 강재량");         m_arText.Add(strBdcy); //⑤
    }
    else 
    {
        strBdcy = _T("Text: (7) 최소 강재량");         m_arText.Add(strBdcy); //⑤
    }

    //Insert Eq.
    //strBdcy = _T("<Equation> <Title>~ qp_out</Title> <Col>0</Col> </Equation>");   m_arText.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ fpe</Title> <Col>0</Col> </Equation>");   m_arText.Add(strEqua);
    m_iFormula++;
    strBdcy = _T(""); m_arText.Add(strBdcy);
    strBdcy.Format(_T("~ ~ ~ == %s / %s + %s * %s / %s = %s ~ %s"),
        sPe, sAc, sPeTimesep, syt, sIy, sfpe, m_sTxStress);  m_arText.Add(strBdcy);
    strBdcy = _T(""); m_arText.Add(strBdcy);

    //Insert Eq.
    strEqua = _T("<Equation> <Title>~ ~ bMcr_eq</Title> <Col>0</Col> </Equation>");   m_arText.Add(strEqua);
    m_iFormula++;
    strBdcy.Format(_T("~ ~ ~ == ( 2 * √  ( %s ) + %s ) * ( %s / %s ) / 10^5 = %s ~ %s"),
        sfck, sfpe, sIy, syt, s12Mcr, m_strMoment);          m_arText.Add(strBdcy);
    m_arText.Add(_T(""));
    strBdcy = Get_MinPsPrint( dpMn, d12Mcr);  m_arText.Add(strBdcy);
    m_arText.Add(_T(""));
    m_arText.Add(_T(""));
}

CString CPrtKSCE_USD03::Get_fsPrint(double dfs, double depsi, double dEs, double dfy)
{
    CString sfs   = Check_ValueForm(6,1, dfs);
    CString sepsi = Check_ValueForm(6,4, depsi);
    CString sEs   = Check_ValueForm(6,1, dEs);
    CString sfy   = Check_ValueForm(6,1, dfy);

    CString strTemp = _T("");
    if(dfs<=dfy)
    {
        strTemp.Format(_T("~ ~ ~ == %s ~ * %s = %s ~ ≤  ~ f_y == %s ~ ~ ~ ~ ∴  ~ f_s == %s ~ %s") ,
            sepsi, sEs, sfs, sfy, sfs, m_strStress);
    }
    else
    {
        strTemp.Format(_T("~ ~ ~ == %s ~ * %s = %s ~ ＞  ~ f_y == %s ~ ~ ~ ~ ∴  ~ f_s == %s ~ %s") ,
            sepsi, sEs, sfs, sfy, sfy, m_strStress);
    }  
    return strTemp;

}

CString CPrtKSCE_USD03::Get_DeltaCPrint(double dTp, double dTs, double dCc, double dDelC, double dTol)
{
    CString sTp   = Check_ValueForm(7,1, Cng_CodeForFix(dTp));
    CString sTs   = Check_ValueForm(7,1, Cng_CodeForFix(dTs));
    CString sCc   = Check_ValueForm(7,1, Cng_CodeForFix(dCc));
    CString sDelC = Check_ValueForm(8,6, Cng_CodeForFix(dDelC));
    CString sTol  = Check_ValueForm(8,6, Cng_CodeForFix(dTol));

    CString strTemp = _T(""); 
    if(dDelC<dTol) 
    {
        strTemp.Format(_T("~ ~ ~ ~ == %s + %s - %s = %s ~ ≤  ~ %f"),sTp, sTs, sCc, sDelC, sTol);   
    }
    else
    {
        strTemp.Format(_T("~ ~ ~ ~ == %s + %s - %s = %s ~ ＞  ~ %f ~ ~ ~ ~ 중립축이  수렴하지  않았습니다."),sTp, sTs, sCc, sDelC, sTol);     
    }

    return strTemp;
}

CString CPrtKSCE_USD03::Get_pMnPrint(double dpMn, double dMu)
{
    CString spMn = Check_ValueForm(8,2, Cng_CodeMomFix(dpMn));
    CString sMu = Check_ValueForm(8,2, Cng_CodeMomFix(dMu));

    CString strTemp = _T("");
    CString sUnequal = _T(""), sJudge = _T("");

    if(dpMn >= fabs(dMu)) { sUnequal=_T("≥"); sJudge=_T("O.K"); }
    else            { sUnequal=_T("＜"); sJudge=_T("N.G"); }

    if(dMu<0.0)  strTemp.Format(_T("~ ~ Φ  M_n == %s %s ~ %s  ~ M_u == | %s | ~ %s ~ ~ ~ ~ ~ ~ ~ %s"),spMn, m_strMoment, sUnequal, sMu, m_strMoment, sJudge);
    else         strTemp.Format(_T("~ ~ Φ  M_n == %s %s ~ %s  ~ M_u == %s ~ %s ~ ~ ~ ~ ~ ~ ~ %s"),spMn, m_strMoment, sUnequal, sMu, m_strMoment, sJudge);

    return strTemp;
}

CString CPrtKSCE_USD03::Get_MaxPsPrint(double drhop, double dfps, double dfck, double dqp, double d036beta1)
{
    CString srhop     = Check_ValueForm(6,5, drhop);
    CString sfps      = Check_ValueForm(8,1, dfps);
    CString sfck      = Check_ValueForm(6,1, dfck);
    CString sqp       = Check_ValueForm(6,5, dqp);
    CString s036beta1 = Check_ValueForm(6,2, d036beta1);

    CString strTemp = _T("");
    CString sUnequal = _T(""), sJudge = _T("");

    if(dqp <= d036beta1) { sUnequal=_T("≤"); sJudge=_T("O.K"); }
    else                 { sUnequal=_T("＞"); sJudge=_T("N.G"); }

    strTemp.Format(_T("~ ~ ~ == %s ~ * ( %s / %s ) = %s ~ %s  ~ %s ~ ~ ~ ~ ~ ~ ~ %s"),
        srhop, sfps, sfck, sqp, sUnequal, s036beta1, sJudge);

    return strTemp;
}

CString CPrtKSCE_USD03::Get_MinPsPrint(double dpMn, double d12Mcr)
{
    CString spMn   = Check_ValueForm(8,2, Cng_CodeMomFix(dpMn));
    CString s12Mcr = Check_ValueForm(8,2, Cng_CodeMomFix(d12Mcr));

    CString strTemp = _T("");
    CString sUnequal = _T(""), sJudge = _T("");

    if(dpMn >= d12Mcr) { sUnequal=_T("≥"); sJudge=_T("O.K"); }
    else            { sUnequal=_T("＜"); sJudge=_T("N.G"); }

    strTemp.Format(_T("~ ~ Φ  M_n == %s %s ~ %s  ~ 1.2 ~ M_cr == %s ~ %s ~ ~ ~ ~ ~ ~ ~ %s"),spMn, m_strStress, sUnequal, s12Mcr, m_strStress, sJudge);

    return strTemp;
}

// Code에 따른 계산서.
void CPrtKSCE_USD03::Print_BdcyDesign_Code1(double dc, double dbeta1, double da , double dfck) 
{
    CString sc     = Check_ValueForm(7,1, dc);
    CString sbeta1 = Check_ValueForm(7,3, dbeta1);
    CString sa     = Check_ValueForm(7,1, da);
    CString sfck   = Check_ValueForm(8,2, dfck);  

    CString strBdcy;
    CString strEqua;
    strBdcy = _T("Text: (2) 힘의 평형 조건으로  중립축 c 결정");       m_arText.Add(strBdcy);
    strBdcy.Format(_T("~ ~ c ~ == ~ %s %s"), sc, m_strLength);                                           m_arText.Add(strBdcy);
    m_arText.Add(_T(""));
    strBdcy = _T("Text: (3) 등가 사각형의 응력블록의 깊이");       m_arText.Add(strBdcy);
    strEqua = _T("<Equation> <Title>~ ~ Equal_a</Title> <Col>6</Col> </Equation>");
    m_iFormula++;
    strBdcy.Format(_T("%s ~ == %s * %s = %s ~ %s"),strEqua, sbeta1, sc, sa, m_strLength);          m_arText.Add(strBdcy);
    m_arText.Add(_T(""));   
}

void CPrtKSCE_USD03::Print_BdcyDesign_Code2(double dfpu, double drp, double dbeta1 , double dRhop, double dfck,
                                            double dd, double ddp, double dRho, double dRhoc, double dfy, double dfps)
{
    CString sfpu   = Check_ValueForm(8,2, dfpu  );
    CString srp    = Check_ValueForm(4,2, drp   );
    CString sbeta1 = Check_ValueForm(8,3, dbeta1);
    CString sRhop  = Check_ValueForm(8,5, dRhop );
    CString sfck   = Check_ValueForm(8,2, dfck  );
    CString sd     = Check_ValueForm(7,1, dd    );
    CString sdp    = Check_ValueForm(7,1, ddp   );
    CString sRho   = Check_ValueForm(8,5, dRho  );
    CString sRhoc  = Check_ValueForm(8,5, dRhoc );
    CString sfy    = Check_ValueForm(8,2, dfy   );
    CString sfps   = Check_ValueForm(8,2, dfps  );  

    CString strBdcy;
    CString strEqua;
    strBdcy = _T("Text: (4) PS강재의 응력");           m_arText.Add(strBdcy); //②
    strBdcy = _T("Text:   - 부착된 부재");          m_arText.Add(strBdcy);
    if(dd>0.0)
    {
        strEqua = _T("<Equation> <Title>~ fps_bond1</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);
        m_iFormula++;
        m_arText.Add(_T(""));
        m_arText.Add(_T(""));
        CString str1;
        str1.Format(_T("( %s * %s / %s + %s / %s * ( %s * %s / %s - %s * %s / %s ) )"), 
            sRhop, sfpu, sfck, sd, sdp, sRho, sfy, sfck, sRhoc, sfy, sfck);
        strBdcy.Format(_T("~ ~ ~ == %s * ( 1 - %s / %s * %s ) = %s ~ %s"), sfpu, srp, sbeta1, str1, sfps, m_sTxStress);        m_arText.Add(strBdcy);                   
        m_arText.Add(_T("")); 
    }
    else if(dd==0.0)
    {
        strEqua = _T("<Equation> <Title>~ fps_bond2</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);
        m_iFormula++;
        m_arText.Add(_T(""));
        m_arText.Add(_T(""));
        strBdcy.Format(_T("~ ~ ~ == %s * ( 1 - %s / %s * ( %s * %s / %s ) ) = %s ~ %s"),
            sfpu, srp, sbeta1, sRhop, sfpu, sfck, sfps, m_sTxStress);              m_arText.Add(strBdcy);
    }

}

void CPrtKSCE_USD03::Print_BdcyDesign_Code3(double dfpu, double dfpe_cal, double dfpe, double d05fpu, double dfps)
{
    CString sfpu     = Check_ValueForm(8,2, dfpu);
    CString sfpe     = Check_ValueForm(8,2, dfpe);
    CString sfpe_cal = Check_ValueForm(8,2, dfpe_cal);
    CString s05fpu   = Check_ValueForm(8,2, d05fpu);
    CString sfps     = Check_ValueForm(8,2, dfps);  

    CString strBdcy;
    CString strEqua;
    strBdcy = _T("Text: (4) PS강재의 응력");           m_arText.Add(strBdcy); //②
    strBdcy = _T("Text:   - 부착되지 않은 부재");                                m_arText.Add(strBdcy);
    if(dfps==0.0)
    {
        strEqua = _T("<Equation> <Title>~ fps_unbond</Title> <Col>0</Col> </Equation>");                 m_arText.Add(strEqua);
        strBdcy.Format(_T("~ ~ == 0.0 ~ %s"), m_sTxStress);          m_arText.Add(strBdcy);
        m_iFormula++;
    }
    if(dfpe_cal<d05fpu)
    {
        strBdcy.Format(_T("~ f_pe == %s ~ < ~ 0.5 f_pu == %s"), sfpe_cal, s05fpu);  m_arText.Add(strBdcy);
        strBdcy.Format(_T("~ ~ ∴ f_pe == %s ~ &s"), sfpe, m_sTxStress);                              m_arText.Add(strBdcy);
    }

    strEqua = _T("<Equation> <Title>~ fps_unbond</Title> <Col>0</Col> </Equation>");                 m_arText.Add(strEqua);
    m_iFormula++;
    strBdcy.Format(_T("~ ~ == %s + 1050 = %s ~ %s"), sfpe, dfps, m_sTxStress);          m_arText.Add(strBdcy);
    m_arText.Add(_T("")); 
}

void CPrtKSCE_USD03::Print_BdcyDesign_Code4(double da, double db, double dbw, double dApw, double dApf,
                                            double dAp, double dAs, double dhf, double dfpu, double drp, double dbeta1,
                                            double dRhop, double dfck, double dd, double ddp, double dRho, 
                                            double dfy, double dfps, double dphi, double dMn, double dpMn, double dMu)
{
    CString sa     = Check_ValueForm(7,1, da    );
    CString sb     = Check_ValueForm(7,1, db    );
    CString sbw    = Check_ValueForm(7,1, dbw   );
    CString sApw   = Check_ValueForm(8,1, dApw  );
    CString sApf   = Check_ValueForm(8,1, dApf  );
    CString sAp    = Check_ValueForm(8,1, dAp   );
    CString sAs    = Check_ValueForm(8,1, dAs   );
    CString shf    = Check_ValueForm(7,1, dhf   );
    CString sfpu   = Check_ValueForm(8,2, dfpu  );
    CString srp    = Check_ValueForm(4,2, drp   );
    CString sbeta1 = Check_ValueForm(8,3, dbeta1);
    CString sRhop  = Check_ValueForm(8,5, dRhop );  
    CString sfck   = Check_ValueForm(8,2, dfck  );
    CString sd     = Check_ValueForm(7,1, dd    );
    CString sdp    = Check_ValueForm(7,1, ddp   );
    CString sRho   = Check_ValueForm(8,5, dRho  );
    CString sfy    = Check_ValueForm(8,2, dfy   );
    CString sfps   = Check_ValueForm(8,2, dfps  );
    CString sphi   = Check_ValueForm(6,2, dphi  );
    CString sMn    = Check_ValueForm(9,2, dMn   );
    CString spMn   = Check_ValueForm(9,2, dpMn  );
    CString sMu    = Check_ValueForm(9,2, dMu   );


    CString strBdcy;
    CString strEqua;
    CString str1, str2, str3;
    strBdcy = _T("Text: (5) 설계 휨강도 ");          m_arText.Add(strBdcy); //③
    if(da<=dhf)
    {
        strBdcy.Format(_T("~ a == %s ~ < ~ t == %s ~ ~ ( 사각형`단면 )"), sa, shf);     m_arText.Add(strBdcy);
        m_arText.Add(_T(""));
        if(dAs>0.0)
        {
            strEqua = _T("<Equation> <Title>~ pMn_rec</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);
            m_iFormula++;
            m_arText.Add(_T(""));
            str1.Format(_T("( 1 - 0.59 * ( %s * %s / %s + %s / %s * ( %s * %s / %s ) ) )"),
                sRhop, sfps, sfck, sd, sdp, sRho, sfy, sfck);
            str2.Format(_T("( 1 - 0.59 * ( %s / %s * ( %s * %s / %s ) + %s * %s / %s ) )"),
                sdp, sd, sRhop, sfps, sfck,  sRho, sfy, sfck);
            strBdcy.Format(_T("~ ~ ~ == %s * ( %s * %s * %s * %s + %s * %s * %s * %s ) / 10^5 ~ = %s ~ %s"),
                sphi, sAp, sfps, sdp, str1, sAs, sfy, sd, str2, spMn, m_strMoment);         m_arText.Add(strBdcy);
        }
        else if(dAs==0.0)
        {
            strEqua = _T("<Equation> <Title>~ pMn_rec_nobar</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);
            m_iFormula++;
            m_arText.Add(_T(""));
            str1.Format(_T("( 1 - 0.59 * %s * %s / %s )"),sRhop, sfps, sfck);                    
            strBdcy.Format(_T("~ ~ ~ == %s * ( %s * %s * %s * %s ) / 10^5 ~ = %s ~ %s"),
                sphi, sAp, sfps, sdp, str1, spMn, m_strMoment);                            m_arText.Add(strBdcy);
        }
    }
    else if(da>dhf)
    {
        strBdcy.Format(_T("~ a == %s ~ > ~ t == %s ~ ~ ( 플랜지를`갖는`단면)"), sa, shf);   m_arText.Add(strBdcy);
        m_arText.Add(_T(""));

        strEqua = _T("<Equation> <Title>~ Apf</Title> <Col>0</Col> </Equation>");             m_arText.Add(strEqua);  
        m_iFormula++;
        strBdcy.Format(_T("~ ~ ~ == 0.85 * %s * ( %s - %s ) * %s / %s = %s ~ %s"),
            sfck, sb, sbw, shf, sfps, sApf, m_sTxArea);                        m_arText.Add(strBdcy);

        if(dAs>0.0)
        {
            strEqua = _T("<Equation> <Title>~ Apw2</Title> <Col>0</Col> </Equation>");          m_arText.Add(strEqua);
            m_iFormula++;
            strBdcy.Format(_T("~ ~ ~ == %s + ( %s * %s / %s ) - %s = %s ~ %s"),
                sAp, sAs, sfy, sfps, sApf, sApw, m_sTxArea);                     m_arText.Add(strBdcy);
        }
        else if(dAs==0.0)
        {
            strEqua = _T("<Equation> <Title>~ Apw1</Title> <Col>15</Col> </Equation>");       
            m_iFormula++;
            strBdcy.Format(_T("%s ~ == %s - %s = %s ~ %s"), strEqua, sAp, sApf, sApw, m_sTxArea);   m_arText.Add(strBdcy);
        }
        m_arText.Add(_T(""));

        strEqua = _T("<Equation> <Title>~ pMn_flg</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);
        m_iFormula++;
        m_arText.Add(_T(""));
        str1.Format(_T("%s * %s * %s * ( 1 - 0.59 * %s * %s / ( %s * %s * %s ) )"),
            sApw, sfps, sdp, sApw, sfps, sbw, sdp, sfck); 
        str2.Format(_T("%s * %s * ( %s - %s )"),sAs, sfy, sd, sdp);
        str3.Format(_T("0.85 * %s * ( %s - %s ) * %s * ( %s - 0.5 * %s )"),
            sfck, sb, sbw, shf, sdp, shf);
        strBdcy.Format(_T("~ ~ ~ == %s * ( %s + %s + %s ) / 10^5 ~ = %s ~ %s"),
            sphi, str1, str2, str3, spMn, m_strMoment);                            m_arText.Add(strBdcy);
        m_arText.Add(_T(""));

    }

    strBdcy = _T(""); m_arText.Add(strBdcy);
    strBdcy = Get_pMnPrint(dpMn, dMu);      m_arText.Add(strBdcy);
    strBdcy = _T(""); m_arText.Add(strBdcy);

}

void CPrtKSCE_USD03::Print_BdcyDesign_Code5(BOOL bIter, double dphi, double dfpu, double drp, double dbeta1 , double dRhop, double dfck, double da, double dhf, double db, double dbw,
                                            double dd, double ddp, double dqp, double dRho, double dApw, double dAp, double dApf, double dAs, double dfy, double dfps, double dpMn, double dpMn_beta, double d036beta1, double dpMn_Cal)

{
    CString sphi   = Check_ValueForm(4,2, dphi  );

    CString sfpu   = Check_ValueForm(8,2, dfpu  );
    CString sbeta1 = Check_ValueForm(8,3, dbeta1);
    CString sRhop  = Check_ValueForm(8,5, dRhop );  
    CString sfck   = Check_ValueForm(8,2, dfck  );
    CString sa     = Check_ValueForm(7,1, da    );  
    CString shf    = Check_ValueForm(7,1, dhf   );  
    CString sb     = Check_ValueForm(7,1, db    );  
    CString sbw    = Check_ValueForm(7,1, dbw   );  
    CString sd     = Check_ValueForm(7,1, dd    );
    CString sdp    = Check_ValueForm(7,1, ddp   );
    CString sqp    = Check_ValueForm(8,3, dqp   );
    CString sRho   = Check_ValueForm(8,5, dRho  );
    CString sApw   = Check_ValueForm(7,1, dApw  );
    CString sAp    = Check_ValueForm(7,1, dAp   );
    CString sApf   = Check_ValueForm(7,1, dApf  );
    CString sAs    = Check_ValueForm(7,1, dAs   );
    CString sfy    = Check_ValueForm(8,2, dfy   );
    CString sfps   = Check_ValueForm(8,2, dfps  );
    CString spMn   = Check_ValueForm(9,2, Cng_CodeMomFix(dpMn) );
    CString spMn_beta = Check_ValueForm(9,2, Cng_CodeMomFix(dpMn_beta) );
    CString s036beta1 = Check_ValueForm(6,3, d036beta1);  
    CString spMn_Cal  = Check_ValueForm(9,2, Cng_CodeMomFix(dpMn_Cal) );

    CString strBdcy;
    CString strEqua;
    // strBdcy = _T("Text:  철근 및 PS강재의 제한(연성 한계)");          m_arText.Add(strBdcy);
    if(bIter)
    {
        strBdcy = _T("Text: (8) 최대 PS강재량");         m_arText.Add(strBdcy); //④
    }
    else
    {
        strBdcy = _T("Text: (6) 최대 PS강재량");                m_arText.Add(strBdcy);  //④
    }
    strEqua = _T("<Equation> <Title>~ d036beta1</Title> <Col>5</Col> </Equation>");        
    strBdcy.Format(_T("%s ~ == 0.36 * %s = %s"), strEqua, sbeta1, s036beta1);                 m_arText.Add(strBdcy);
    m_iFormula++;

    if(ddp==0.0)
    {
        strBdcy = _T("~ q_p ~ = ~ 0.0");   m_arText.Add(strBdcy);
        m_arText.Add(_T(""));
        return;
    }

    if(da <= dhf)
    {    
        strEqua = _T("<Equation> <Title>~ qp_rec</Title> <Col>10</Col> </Equation>");      
        strBdcy.Format(_T("%s ~ == ~ %s * %s / %s = %s"), strEqua, sRhop, sfps, sfck, sqp);                         m_arText.Add(strBdcy);
        m_iFormula++;
        if(dqp <= d036beta1)
        {      
            strBdcy = _T("~ q_p ~ ≤ ~ 0.36 β_1");  m_arText.Add(strBdcy);      
        }
        else if(dqp > d036beta1)
        {
            strBdcy = _T("~ q_p ~ > ~ 0.36 β_1");   m_arText.Add(strBdcy);
            m_arText.Add(_T(""));
            strEqua = _T("<Equation> <Title>~ pMn_qp_rec</Title> <Col>0</Col> </Equation>");     m_arText.Add(strEqua);
            strBdcy.Format(_T("~ ~ == %s * ( ( 0.36 * %s - 0.08 * %s^2 ) * %s * %s * %s^2 ) / 10^5 ~ = %s ~ %s"),
                sphi, sbeta1, sbeta1, sfck, sb, sdp, spMn_beta, m_strMoment);      m_arText.Add(strBdcy);
            m_iFormula++;
            m_arText.Add(_T(""));
            if(dpMn_Cal <= dpMn_beta)
            {
                strBdcy.Format(_T("~ ~ 설계`휨강도 ~ Φ  M_n ~ < ~ %s ~ %s"), spMn_beta, m_strMoment);         m_arText.Add(strBdcy);
                strBdcy.Format(_T("~ ~ ∴  ~ Φ  M_n ~ == ~ %s ~ %s"),spMn, m_strMoment);   m_arText.Add(strBdcy);
            }
            else if(dpMn_Cal > dpMn_beta)
            {
                strBdcy.Format(_T("~ ~ 설계`휨강도 ~ Φ  M_n ~ > ~ %s ~ %s"), spMn_beta, m_strMoment);              m_arText.Add(strBdcy);
                strBdcy.Format(_T("~ ~ ∴  ~ Φ  M_n ~ == ~ %s ~ %s"),spMn, m_strMoment);   m_arText.Add(strBdcy);
            }      
        }    
    }
    else if(da > dhf)
    {    
        strBdcy.Format(_T("~ a == %s %s ~ > ~ t == %s %s"),sa, m_strLength, shf, m_strLength);  m_arText.Add(strBdcy);

        CString sUnequal = _T(""), sJudge = _T("");
        if(dqp <= d036beta1) { sUnequal=_T("≤"); sJudge=_T("O.K"); }
        else                 { sUnequal=_T("＞"); sJudge=_T("N.G"); }

        strEqua = _T("<Equation> <Title>~ Apf</Title> <Col>0</Col> </Equation>");             m_arText.Add(strEqua);  
        strBdcy.Format(_T("~ ~ ~ == 0.85 * %s * ( %s - %s ) * %s / %s = %s ~ %s"),
            sfck, sb, sbw, shf, sfps, sApf, m_sTxArea);                        m_arText.Add(strBdcy);
        m_iFormula++;

        if(dAs>0.0)
        {
            strEqua = _T("<Equation> <Title>~ Apw2</Title> <Col>15</Col> </Equation>");          m_arText.Add(strEqua);
            strBdcy.Format(_T("~ ~ ~ == %s + ( %s * %s / %s ) - %s = %s ~ %s"),
                sAp, sAs, sfy, sfps, sApf, sApw, m_sTxArea);                     m_arText.Add(strBdcy);
            m_iFormula++;
        }
        else if(dAs==0.0)
        {
            strEqua = _T("<Equation> <Title>~ Apw1</Title> <Col>9</Col> </Equation>");       
            strBdcy.Format(_T("%s ~ == %s - %s = %s ~ %s"), strEqua, sAp, sApf, sApw, m_sTxArea);   m_arText.Add(strBdcy);
            m_iFormula++;
        }
        m_arText.Add(_T(""));

        strEqua = _T("<Equation> <Title>~ qp_out</Title> <Col>0</Col> </Equation>");   m_arText.Add(strEqua);
        m_arText.Add(_T(""));
        m_iFormula++;
        strBdcy.Format(_T("~ ~ ~ == %s / ( %s * %s ) * %s / %s = %s ~ %s  ~ %s ~ ~ ~ ~ ~ ~ ~ %s"),
            sApw, sbw, sdp , sfps, sfck, sqp , sUnequal, s036beta1, sJudge);  m_arText.Add(strBdcy);
        m_arText.Add(_T(""));

        if(dqp > d036beta1)
        {
            m_arText.Add(_T(""));
            strEqua = _T("<Equation> <Title>~ pMn_qp_flg</Title> <Col>0</Col> </Equation>");     m_arText.Add(strEqua);
            m_iFormula++;
            CString str1, str2;
            str1.Format(_T("%s * ( 0.36 * %s - 0.08 * %s^2 ) * %s * %s^2"),sfck, sbeta1, sbeta1, sb, sdp);
            str2.Format(_T("0.85 * %s * ( %s - %s ) * %s * ( %s - 0.5 * %s )"), sfck, sb, sbw, shf, sdp, shf);
            strBdcy.Format(_T("~ ~ == %s * ( %s + %s ) / 10^5 ~ = %s ~ %s"),sphi, str1, str2, spMn_beta, m_strMoment);   m_arText.Add(strBdcy);
            m_arText.Add(_T(""));                     
            if(dpMn_Cal <= dpMn_beta)
            {
                strBdcy.Format(_T("~ ~ 설계`휨강도 ~ Φ  M_n ~ < ~ %s ~ %s"), spMn_beta, m_strMoment);   m_arText.Add(strBdcy);
                strBdcy.Format(_T("~ ~ ∴  ~ Φ  M_n ~ == ~ %s ~ %s"),spMn, m_strMoment);   m_arText.Add(strBdcy);
            }
            else if(dpMn_Cal > dpMn_beta)
            {
                strBdcy.Format(_T("~ ~ 설계`휨강도 ~ Φ  M_n ~ > ~ %s ~ %s"), spMn_beta, m_strMoment);   m_arText.Add(strBdcy);
                strBdcy.Format(_T("~ ~ ∴  ~ Φ  M_n ~ == ~ %s ~ %s"),spMn, m_strMoment);   m_arText.Add(strBdcy);
            }      
        }    

    }   
    m_arText.Add(_T(""));
}

// BondType, UnbondTyep이 같이 있을 경우
void CPrtKSCE_USD03::Print_BdcyDesign_Code6(CArray<CString,CString>& strTndn, CArray<double,double>* aData)
{
    int i=0, j=0;
    int TndnNum = aData[0].GetSize();

    int iCol = 5;
    CArray<CString,CString> sData[9]; 
    for(i=0; i<iCol; i++)
        sData[i].RemoveAll();

    for(i=0; i<TndnNum; i++)
    {
        CString strTemp;
        strTemp = Check_ValueForm(7,1, aData[0].GetAt(i));               sData[0].Add(strTemp); // Apsi
        strTemp = Check_ValueForm(8,2, aData[1].GetAt(i));               sData[1].Add(strTemp); // fpui
        strTemp = Check_ValueForm(8,2, aData[2].GetAt(i));               sData[2].Add(strTemp); // fpsi
        strTemp = Check_ValueForm(7,1, aData[3].GetAt(i));               sData[3].Add(strTemp); // dpi    
        strTemp = (aData[4].GetAt(i)==0.0 ? _T("부착") : _T("비부착"));          sData[4].Add(strTemp); // Type    
    }

    CString strBdcy;
    CString strEqua;
    CString str1, str2, str3;

    strBdcy = _T("<Table>(4)PS 강재의 응력");    m_arText.Add(strBdcy);
    str1 = _T("<MH>TnGroup ~ ~ ~ ~ ~ ~</MH>   <MH>Apsi ~ ~ ~ ~ ~</MH>   <MH>fpui ~ ~ ~ ~ ~</MH>   ");
    str2 = _T("<MH>fpsi ~ ~ ~ ~ ~</MH>   <MH>dpi ~ ~ ~ ~ ~</MH>   <MH>Type ~ ~ ~ ~ ~</MH>   ");
    strBdcy = str1 + str2; m_arText.Add(strBdcy);
    for( i=0; i<TndnNum; i++)
    {
        str1.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   "),strTndn.GetAt(i) ,sData[0].GetAt(i),sData[1].GetAt(i) ); 
        str2.Format(_T("<MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   "),
            sData[2].GetAt(i), sData[3].GetAt(i), sData[4].GetAt(i) );
        strBdcy = str1 + str2;  m_arText.Add(strBdcy);                      
    }
    strBdcy = _T("</Table>");  m_arText.Add(strBdcy);

}

void CPrtKSCE_USD03::Print_BdcyDesign_Code7(double dc, double da, double db, double dbw, double dAs, double dhf,
                                            double dfck, double dd, double ddp, double dfy, double dphi, 
                                            double dMn_ps, double dpMn)
{
    CString sc     = Check_ValueForm(7,1, dc    );
    CString sa     = Check_ValueForm(7,1, da    );
    CString sb     = Check_ValueForm(7,1, db    );
    CString sbw    = Check_ValueForm(7,1, dbw   );
    CString sAs    = Check_ValueForm(8,1, dAs   );
    CString shf    = Check_ValueForm(7,1, dhf   );
    CString sfck   = Check_ValueForm(8,2, dfck  );
    CString sd     = Check_ValueForm(7,1, dd    );
    CString sdp    = Check_ValueForm(7,1, ddp   );
    CString sfy    = Check_ValueForm(8,2, dfy   );
    CString sphi   = Check_ValueForm(6,2, dphi   );
    CString sMn_ps = Check_ValueForm(9,2, dMn_ps );   
    CString spMn   = Check_ValueForm(9,2, dpMn   );

    CString strBdcy;
    CString strEqua;
    CString str1, str2, str3;
    strBdcy = _T("Text:  설계 휨강도 ");          m_arText.Add(strBdcy);
    if(da<=dhf)
    {
        strBdcy.Format(_T("~ a == %s ~ < ~ h_f == %s ~ ~ ( 사각형`단면 )"), sa, shf);     m_arText.Add(strBdcy);
        m_arText.Add(_T(""));
        strEqua = _T("<Equation> <Title>~ ~ pMn_2type_rec</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);
        m_iFormula++;
        str1.Format(_T("0.85 * %s * %s * %s * ( %s - %s / 2 )"), sfck, sa, sb, sc, sa);
        str2.Format(_T("%s * %s * ( %s - %s )"), sAs, sfy, sd, sc);
        strBdcy.Format(_T("~ ~ == %s * ( %s + %s + %s ) / 10^5 ~ = %s ~ %s"), sphi, str1, sMn_ps, str2, spMn, m_strMoment);         m_arText.Add(strBdcy);

    }
    else if(da>dhf)
    {
        strBdcy.Format(_T("~ a == %s ~ > ~ h_f == %s ~ ~ ( 플랜지를`갖는`단면 )"), sa, shf);     m_arText.Add(strBdcy);
        m_arText.Add(_T(""));

        strEqua = _T("<Equation> <Title>~ ~ pMn_2type_flg</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);
        m_iFormula++;
        str1.Format(_T("0.85 * %s ( %s * %s * ( %s - %s / 2 ) + ( %s - %s ) * %s * ( %s - %s ) )"),
            sfck, sa, sbw, sc, sa, sb, sbw, shf, sc, shf);
        str2.Format(_T("%s * %s * ( %s - %s )"), sAs, sfy, sd, sc);
        strBdcy.Format(_T("~ ~ == %s * ( %s + %s + %s ) / 10^5 ~ = %s ~ %s"), sphi, str1, sMn_ps, str2, spMn, m_strMoment);         m_arText.Add(strBdcy);
    }

}

//전단강도 검토(Cums) //////////////////////////////////////////////////////////////////////////////////////////////
void CPrtKSCE_USD03::Print_CumsTheory(CArray<CString,CString>& aString)
{
    CString strCums = _T("");
    CString strEqua, strSpace;
    strSpace = _T("~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ "); 
    strCums = _T("Text:  ▷ 전단강도 검토");                              aString.Add(strCums);
    strCums = _T(""); aString.Add(strCums);
    strCums = _T("Text:    - 콘크리트가 부담하는 전단강도(Vc)");           aString.Add(strCums);
    strCums = _T("Text:     : 휨 전단균열 발생시의 전단강도(Vci)와 복부 전단균열 발생시의 전단강도(Vcw) 중 작은 값을 적용"); aString.Add(strCums);
    strCums = _T(""); aString.Add(strCums);

    strCums = _T("Text:    - 휨 전단균열 발생시의 전단강도");             aString.Add(strCums);
    strEqua = _T("<Equation> <Title>~ ~ ~ Vci_b2</Title> <Col>30</Col> </Equation> ");    
    //strCums = strEqua + _T("V_d ~ : ~ 사용고정하중에`의해`발생하는`전단력");   aString.Add(strCums);
    //strCums = strSpace + _T("M_d ~ : ~ 사용고정하중에`의해`발생하는`모멘트");   aString.Add(strCums);
    strCums = _T(""); aString.Add(strCums);

    strEqua = _T("<Equation> <Title>~ ~ ~ Mcr_b2</Title> <Col>30</Col> </Equation> ");    
    strCums = strEqua + _T("M_cr : ~ 균열`모멘트");                          aString.Add(strCums);
    strCums = strSpace + _T("f_pe : ~ 유효프리스트레스에 의한`압축응력");     aString.Add(strCums);
    //strCums = strSpace + _T("f_d ~ : ~ 자중에`의한`휨인장응력");                aString.Add(strCums);
    strCums = _T(""); aString.Add(strCums);

    //strEqua = _T("<Equation> <Title>~ ~ ~ Vi_b</Title> <Col>10</Col>  <Title>~ Mmax_b</Title> <Col>30</Col> </Equation> ");     aString.Add(strEqua);
    //strCums = _T(" "); aString.Add(strCums);

    strCums = _T("Text:    - 복부 전단균열 발생시의 전단강도");           aString.Add(strCums);
    strEqua = _T("<Equation> <Title>~ ~ ~ Vcw_b</Title> <Col>30</Col> </Equation> ");    
    strCums = strEqua + _T("f_pc : ~ 유효프리스트레스에`의한`콘크리트");     aString.Add(strCums);
    strCums = strSpace + _T("~ ~ ~ ~ ~ 도심에서의`압축응력");                        aString.Add(strCums);
    strCums = strSpace + _T("V_p ~ : ~ 유효프리스트레스의`수직분력");                aString.Add(strCums);

    strCums = _T(" "); aString.Add(strCums);
    strCums = _T("Text:    - 전단철근이 부담하는 전단강도( Vs )");           aString.Add(strCums);
    strCums = _T("<Equation> <Title>~ ~ ~ Vs_b</Title> <Col>0</Col> </Equation> ");    aString.Add(strCums);
    strCums = _T(""); aString.Add(strCums);
    strCums = _T(""); aString.Add(strCums);

    strCums = _T("Text:    - 최소전단철근");                               aString.Add(strCums);
    strCums = _T("<Equation> <Title>~ ~ ~ pVc_b</Title> <Col>10</Col>  <Title>~ pVc12_b</Title> <Col>20</Col> </Equation> 일경우");  aString.Add(strCums);  
    strCums = _T("<Equation> <Title>~ ~ ~ Avmin_b</Title> <Col>0</Col> </Equation> ");    aString.Add(strCums);

}

void CPrtKSCE_USD03::Print_CumsDesign0()
{
    CString sfck  = Check_ValueForm(8,2, m_CumsParam.dfck);
    CString sbw   = Check_ValueForm(7,1, m_CumsParam.dbw);
    CString sdd   = Check_ValueForm(7,1, m_CumsParam.ddst);
    CString sfy   = Check_ValueForm(8,2, m_CumsParam.dfy);
    CString sh    = Check_ValueForm(7,1, m_CumsParam.dh);
    CString sIy   = Check_ValueForm(9,3, Cng_CodeLenFix(m_CumsParam.dIy,4));

    CString sfpu = Check_ValueForm(8,2, m_CumsParam.dfpu);
    CString sdp  = Check_ValueForm(7,1, m_CumsParam.ddp);
    CString sAc  = Check_ValueForm(9,3, Cng_CodeLenFix(m_CumsParam.dAc,2));
    CString sfpy = Check_ValueForm(8,2, m_CumsParam.dfpy);
    CString syt  = Check_ValueForm(7,1, m_CumsParam.dyt);
    CString sAps = Check_ValueForm(8,1, m_CumsParam.dAps);

    CString sSpace4 = _T("~ ~ ~ ~ ");
    CString sSpace5 = _T("~ ~ ~ ~ ~ ");
    CString sSpace6 = _T("~ ~ ~ ~ ~ ~ ");
    CString sSpace7 = _T("~ ~ ~ ~ ~ ~ ~ ");
    CString sSpace8 = _T("~ ~ ~ ~ ~ ~ ~ ~ ");
    CString sSpace9 = _T("~ ~ ~ ~ ~ ~ ~ ~ ~ ");
    CString s1stSpace  = _T("");
    CString s2ndSpace  = _T("");

    int iMaxLength = 0;
    int arLength[3][4];

    int isfck = arLength[0][0] = sfck.GetLength();
    int isfy  = arLength[0][1] = sfy.GetLength();
    int isfpu = arLength[0][2] = sfpu.GetLength();
    int isfpy = arLength[0][3] = sfpy.GetLength();

    int isbw  = arLength[1][0] = sbw.GetLength();
    int ish   = arLength[1][1] = sh.GetLength();
    int isdp  = arLength[1][2] = sdp.GetLength();
    int isyt  = arLength[1][3] = syt.GetLength();

    int isdd  = arLength[2][0] = sdd.GetLength();
    int isIy  = arLength[2][1] = sIy.GetLength();
    int isAc  = arLength[2][2] = sAc.GetLength();
    int isAps = arLength[2][3] = sAps.GetLength();

    // 1 Column.    
    for(int i=0; i<4; i++)  
        iMaxLength = max(iMaxLength,arLength[0][i]);  

    CString strfck, strfy, strfpu, strfpy;
    strfck  = Get_TextStringLength(iMaxLength, sfck);
    strfy   = Get_TextStringLength(iMaxLength, sfy);
    strfpu  = Get_TextStringLength(iMaxLength, sfpu);
    strfpy  = Get_TextStringLength(iMaxLength, sfpy);

    if(iMaxLength==10 || iMaxLength==11)    s1stSpace = sSpace4;
    else if(iMaxLength==9 || iMaxLength==8) s1stSpace = sSpace5;
    else if(iMaxLength==7)                  s1stSpace = sSpace6;
    else if(iMaxLength==6 || iMaxLength==5) s1stSpace = sSpace7;
    else if(iMaxLength==4 || iMaxLength==3) s1stSpace = sSpace8;
    else if(iMaxLength==2)                  s1stSpace = sSpace9;
    else ASSERT(0);

    // 2 Column.
    iMaxLength = 0;
    for(int i=0; i<4; i++)  
        iMaxLength = max(iMaxLength,arLength[1][i]);

    CString strbw, strh, strdp, stryt;
    strbw  = Get_TextStringLength(iMaxLength, sbw );
    strh   = Get_TextStringLength(iMaxLength, sh  );
    strdp  = Get_TextStringLength(iMaxLength, sdp );
    stryt  = Get_TextStringLength(iMaxLength, syt );  

    if(iMaxLength==10 || iMaxLength==11)    s2ndSpace = sSpace4;
    else if(iMaxLength==9 || iMaxLength==8) s2ndSpace = sSpace5;
    else if(iMaxLength==7)                  s2ndSpace = sSpace6;
    else if(iMaxLength==6 || iMaxLength==5) s2ndSpace = sSpace7;
    else if(iMaxLength==4 || iMaxLength==3) s2ndSpace = sSpace8;
    else if(iMaxLength==2)                  s2ndSpace = sSpace9;

    // 3 Column.
    iMaxLength = 0;
    for(int i=0; i<4; i++)  
        iMaxLength = max(iMaxLength,arLength[2][i]);

    CString strdd, strIy, strAc, strAps;
    strdd  = Get_TextStringLength(iMaxLength, sdd );
    strIy  = Get_TextStringLength(iMaxLength, sIy );
    strAc  = Get_TextStringLength(iMaxLength, sAc );
    strAps = Get_TextStringLength(iMaxLength, sAps);  

    if(sdd.GetLength()<=3)  strdd += _T("~ ");
    if(sIy.GetLength()>=9)  strIy.TrimRight(_T("~ "));
    if(sAc.GetLength()>=9)  strAc.TrimRight(_T("~ "));


    CString strCums = _T("");
    strCums = _T("Text: (1) 설계 변수");                                                m_arText.Add(strCums);
    strCums.Format(_T("~ ~ f_ck : ~ %s%s %s b_w ~ : ~ %s%s %s d ~ ~ : ~ %s%s"),
        strfck,m_sTxStress, s1stSpace, strbw, m_strLength, s2ndSpace, strdd,m_strLength);    m_arText.Add(strCums);
    strCums.Format(_T("~ ~ f_y ~ : ~ %s%s %s h ~ ~ : ~ %s%s %s I_y ~ : ~ %s %s"),
        strfy,m_sTxStress, s1stSpace, strh, m_strLength, s2ndSpace, strIy, m_sTxStif);    m_arText.Add(strCums);  

    strCums.Format(_T("~ ~ f_pu : ~ %s%s %s d_p ~ : ~ %s%s %s A_c ~ : ~ %s %s"),
        strfpu,m_sTxStress, s1stSpace, strdp,m_strLength, s2ndSpace, strAc,m_sTxArea);    m_arText.Add(strCums);
    strCums.Format(_T("~ ~ f_py : ~ %s%s %s y_t ~ : ~ %s%s %s A_p ~ : ~ %s%s"),
        strfpy,m_sTxStress, s1stSpace, stryt, m_strLength, s2ndSpace, strAps,m_sTxArea);    m_arText.Add(strCums);
    m_arText.Add(_T(""));

}

void CPrtKSCE_USD03::Print_CumsDesign1(double dfck, double dbw, double ddst , double dfy, double dh, double dIy)
{
    CString sfck  = Check_ValueForm(6,1, dfck);
    CString sbw   = Check_ValueForm(6,1, dbw);
    CString sdst  = Check_ValueForm(6,1, ddst);
    CString sfy   = Check_ValueForm(6,1, dfy);
    CString sh    = Check_ValueForm(6,1, dh);
    CString sIy   = Check_ValueForm(6,1, Cng_CodeLenFixM(dIy,4));

    CString strCums = _T("");
    strCums = _T("Text: (1) 설계 변수");                                                m_arText.Add(strCums);
    strCums.Format(_T("~ f_ck : ~ %s ~ ~ %s ~ ~ ~ ~ ~ b_w : ~ %s ~ %s ~ ~ ~ ~ ~ d ~ ~ : ~ %s ~ %s"),
        sfck,m_strStress, sbw,m_strLength, sdst,m_strLength);    m_arText.Add(strCums);
    strCums.Format(_T("~ f_y ~ : ~ %s ~ ~ %s ~ ~ ~ ~ ~ h ~ : ~ %s ~ %s ~ ~ ~ ~ ~ I_y ~ : ~ %s ~ ~ %s"),
        sfy,m_strStress, sh,m_strLength , sIy,m_strStifM);    m_arText.Add(strCums);  
}

void CPrtKSCE_USD03::Print_CumsDesign2(double dfpu, double ddp, double dAc, double dfpy, double dyt, double dAps)
{
    CString sfpu = Check_ValueForm(7,1, dfpu);
    CString sdp  = Check_ValueForm(6,1, ddp);
    CString sAc  = Check_ValueForm(6,1, Cng_CodeLenFixM(dAc,2));
    CString sfpy = Check_ValueForm(7,1, dfpy);
    CString syt  = Check_ValueForm(6,1, dyt);
    CString sAps = Check_ValueForm(6,1, dAps);

    CString strCums = _T("");
    strCums.Format(_T("~ f_pu : ~ %s ~ %s ~ ~ ~ ~ ~ d_p : ~ %s ~ %s ~ ~ ~ ~ ~ A_c ~ : ~ %s ~ ~ %s"),
        sfpu,m_strStress, sdp,m_strLength, sAc,m_strAreaM);    m_arText.Add(strCums);
    strCums.Format(_T("~ f_py : ~ %s ~ %s ~ ~ ~ ~ ~ y_b : ~ %s ~ %s ~ ~ ~ ~ ~ A_ps : ~ %s ~ %s"),
        sfpy,m_strStress, syt,m_strLength , sAps,m_strArea);    m_arText.Add(strCums);
    strCums = _T("");   m_arText.Add(strCums);
}

void CPrtKSCE_USD03::Print_CumsDesign3(double dMu, double dVu, double dMd, double dVd, double dfd, double dPe, double dfpe)
{
    CString sMu  = Check_ValueForm(7,1, Cng_CodeMomFix(dMu));
    CString sVu  = Check_ValueForm(7,1, Cng_CodeForFix(dVu));
    CString sMd  = Check_ValueForm(7,1, Cng_CodeMomFix(dMd));
    CString sVd  = Check_ValueForm(7,1, Cng_CodeForFix(dVd));
    CString sfd  = Check_ValueForm(6,1, dfd);
    CString sPe  = Check_ValueForm(7,1, Cng_CodeForFix(dPe));
    CString sfpe = Check_ValueForm(6,1, dfpe);

    CString strStress=_T("kgf/cm²");

    CString strCums = _T("");
    CString strDum1, strDum2, strDum3;
    strCums = _T("Text: (2) 설계 단면력");                 m_arText.Add(strCums);
    strCums = _T("<Table>설계 단면력");                                                                   m_arText.Add(strCums);
    if(m_iVciType==1)
    {
        strDum1 = _T("<MH>계``수``하``중  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~</MH> ");
        strDum2 = _T("<MH>사``용``하``중  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~</MH> ");
        strDum3 = _T("<MH>유효프리스트레스력  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~</MH> ");
        strCums = strDum1 + strDum2 + strDum3;                                                            m_arText.Add(strCums);


        strDum1 = _T("<MH>Mu ~ ~ ~ ~ ~ ~</MH> <MH>Vu ~ ~ ~ ~ ~ ~</MH> ");
        strDum2 = _T("<MH>Md ~ ~ ~ ~ ~ ~</MH> <MH>Vd ~ ~ ~ ~ ~ ~</MH> <MH>fd ~ ~ ~ ~ ~ ~</MH> ");
        strDum3 = _T("<MH>Pe ~ ~ ~ ~ ~ ~</MH> <MH>fpe ~ ~ ~ ~ ~ ~</MH>");
        strCums = strDum1 + strDum2 + strDum3;                                                            m_arText.Add(strCums);


        strDum1.Format(_T("<MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH> "),m_strMoment,m_strForce);
        strDum2.Format(_T("<MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH> "),
            m_strMoment, m_strForce, strStress);
        strDum3.Format(_T("<MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH>"),m_strForce, strStress);
        strCums = strDum1 + strDum2 + strDum3;                                                            m_arText.Add(strCums);


        strDum1.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH> "),sMu, sVu);
        strDum2.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH> "), sMd, sVd, sfd);
        strDum3.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH>"),sPe, sfpe);
        strCums = strDum1 + strDum2 + strDum3;            m_arText.Add(strCums);
    }
    else if(m_iVciType==2)
    {
        strDum1 = _T("<MH>계``수``하``중  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~</MH> ");    
        strDum3 = _T("<MH>유효프리스트레스력  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~</MH> ");
        strCums = strDum1 + strDum3;                                                            m_arText.Add(strCums);


        strDum1 = _T("<MH>Mu ~ ~ ~ ~ ~ ~</MH> <MH>Vu ~ ~ ~ ~ ~ ~</MH> ");
        strDum3 = _T("<MH>Pe ~ ~ ~ ~ ~ ~</MH> <MH>fpe ~ ~ ~ ~ ~ ~</MH>");
        strCums = strDum1 + strDum3;                                                            m_arText.Add(strCums);


        strDum1.Format(_T("<MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH> "),m_strMoment,m_strForce);
        strDum3.Format(_T("<MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH>"),m_strForce, strStress);
        strCums = strDum1 + strDum3;                                                            m_arText.Add(strCums);


        strDum1.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH> "),sMu, sVu);
        strDum3.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH>"),sPe, sfpe);
        strCums = strDum1 + strDum3;            m_arText.Add(strCums);
    }
    strCums = _T("</Table>");   m_arText.Add(strCums);
    m_arText.Add(_T(""));

    strCums = _T("Text: (3) 콘크리트가 부담하는 전단강도의 산정");    m_arText.Add(strCums);
}

void CPrtKSCE_USD03::Print_CumsDesign4(double dh, double dd, double dd_org, double d08h, double ddp, double ddp_org, double dIy, double dyt, double dfck, double dfpe, 
                                       double dfd, double dMcr)
{
    CString sd      = Check_ValueForm(6,1, dd);
    CString sdp     = Check_ValueForm(6,1, ddp);
    CString sd_org  = Check_ValueForm(6,1, dd_org);
    CString sdp_org = Check_ValueForm(6,1, ddp_org);
    CString sIy;    sIy.Format(_T("%.1f"), dIy); // Check_ValueForm(9,3, Cng_CodeLenFix(dIy,4));
    CString syt     = Check_ValueForm(6,1, dyt);
    CString sfck    = Check_ValueForm(6,1, dfck);
    CString sfpe    = Check_ValueForm(6,1, dfpe);
    CString sfd     = Check_ValueForm(6,1, dfd);
    CString sMcr    = Check_ValueForm(6,1, Cng_CodeMomFix(dMcr));

    CString strCums = _T("");
    CString strEqua = _T("");  
    strCums = Get_dValuePrint(dh, d08h, dd_org, ddp_org);                    m_arText.Add(strCums);
    strCums.Format(_T("~ ~ ∴  ~ d == %s %s"), sd, m_strLength);                 m_arText.Add(strCums);
    strCums = _T("");   m_arText.Add(strCums);

    if(m_iVciType==1)
    {
        strEqua = _T("<Equation> <Title>~ Mcr_b</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);  
        m_iFormula++;
        strCums = _T("");   m_arText.Add(strCums);
        strCums.Format(_T("~ ~ ~ ~ == ( %s / %s ) * ( 1.6 ~ * √  ( %s ) + %s - %s ) / 10^5 = %s ~ %s"),
            sIy, syt, sfck, sfpe, sfd, sMcr, m_strMoment);                  m_arText.Add(strCums); 
    }
    else if(m_iVciType==2)
    {
        strEqua = _T("<Equation> <Title>~ Mcr_b2</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);  
        m_iFormula++;
        strCums = _T("");   m_arText.Add(strCums);
        strCums.Format(_T("~ ~ ~ ~ == ( %s / %s ) * ( 1.6 ~ * √  ( %s ) + %s ) / 10^5 = %s ~ %s"),
            sIy, syt, sfck, sfpe, sMcr, m_strMoment);                  m_arText.Add(strCums); 
    }
    strCums = _T("");   m_arText.Add(strCums);
}

void CPrtKSCE_USD03::Print_CumsDesign5(double dMu, double dMd, double dMmax, double dVu, double dVd, double dVi, double dVci_cal,
                                       double dVci, double dVci_incl, double dfck, double dbw, double dd, double dMcr, double dminVci)
{
    CString sMu    = Check_ValueForm(7,1, Cng_CodeMomFix(dMu));
    CString sMd    = Check_ValueForm(7,1, Cng_CodeMomFix(dMd));
    CString sMmax  = Check_ValueForm(7,1, Cng_CodeMomFix(dMmax));
    CString sVu    = Check_ValueForm(7,1, Cng_CodeForFix(dVu));
    CString sVd    = Check_ValueForm(7,1, Cng_CodeForFix(dVd));
    CString sVi    = Check_ValueForm(7,1, Cng_CodeForFix(dVi));
    CString sVci_cal = Check_ValueForm(9,1, Cng_CodeForFix(dVci_cal));  
    CString sVci   = Check_ValueForm(9,1, Cng_CodeForFix(dVci));
    CString sVci_incl = Check_ValueForm(9,1, Cng_CodeForFix(dVci_incl));
    CString sVci_abs = Check_ValueForm(9,1, Cng_CodeForFix(fabs(dVci_incl)));
    CString sfck   = Check_ValueForm(7,1, dfck);
    CString sbw    = Check_ValueForm(7,1, dbw);
    CString sd     = Check_ValueForm(7,1, dd);
    CString sMcr   = Check_ValueForm(7,1, Cng_CodeMomFix(dMcr));
    CString sminVci= Check_ValueForm(7,1, Cng_CodeForFix(dminVci));

    CString strCums = _T("");
    CString strEqua = _T("");
    if(m_iVciType==1)
    {
        strEqua = _T("<Equation> <Title>~ Mmax_b</Title> <Col>12</Col> </Equation>"); m_arText.Add(strEqua);
        m_iFormula++;
        if(dMu>=0.0) { strCums.Format(_T("~ ~ ~ == %s - %s = %s ~ %s"), sMu, sMd, sMmax, m_strMoment);       m_arText.Add(strCums);  }
        else         { strCums.Format(_T("~ ~ ~ == -1 ~ * ( %s - ( %s ) ) = %s ~ %s"), sMu, sMd, sMmax, m_strMoment);       m_arText.Add(strCums);  }
        strCums = _T("");   m_arText.Add(strCums);

        strEqua = _T("<Equation> <Title>~ Vi_b</Title> <Col>10</Col> </Equation>"); m_arText.Add(strEqua);
        m_iFormula++;
        if(dVu>=0.0) { strCums.Format(_T("~ ~ ~ == %s - %s = %s ~ %s"), sVu, sVd, sVi, m_strForce);           m_arText.Add(strCums); }
        else         { strCums.Format(_T("~ ~ ~ == %s - ( %s ) = %s ~ %s"), sVu, sVd, sVi, m_strForce);           m_arText.Add(strCums); }
        strCums = _T("");   m_arText.Add(strCums);
    }

    if(m_iVciType==1)
    {
        strCums = _T("<Equation> <Title>~ Vci_b</Title> <Col>0</Col> </Equation>");  m_arText.Add(strCums);  
        m_iFormula++;
        strCums = _T("");   m_arText.Add(strCums);
        if(dMmax==0.0)
        {
            // 수식.
            strCums.Format(_T("~ ~ ~ == 0.16 * √  ( %s ) * %s * %s / 10^3 + abs ~ ( %s ) = %s ~ %s"),
                sfck, sbw, sd, sVd, sVci_cal, m_strForce);                           m_arText.Add(strCums);
        }
        else
        {	
            if(dMmax>0.0 && dMcr>=0.0 && dVd>=0.0 && dVi>=0.0)
            {
                strCums.Format(_T("~ ~ ~ == 0.16 * √  ( %s ) * %s * %s / 10^3 + %s + %s * ( %s / %s ) = %s ~ %s"),
                    sfck, sbw, sd, sVd, sVi, sMcr, sMmax, sVci_cal, m_strForce);                           m_arText.Add(strCums);
            }
            else 
            {      
                strCums = _T("<Equation> <Title>~ ~ Vci_bend</Title> <Col>0</Col> </Equation>");  m_arText.Add(strCums);  
                m_iFormula++;
                m_arText.Add(_T(""));
                strCums.Format(_T("~ ~ ~ == %s + %s * ( %s / %s ) = %s"), sVd, sVi, sMcr, sMmax, sVci_incl, sVci_incl, sVci_abs);					m_arText.Add(strCums);
                strCums.Format(_T("~ ~ ~ ∴  | %s | == abs ~ ( %s ) = %s ~ %s"), sVci_incl, sVci_incl, sVci_abs, m_strForce);              m_arText.Add(strCums);
                strCums.Format(_T("~ V_ci == 0.16 * √  ( %s ) * %s * %s / 10^3 + %s = %s ~ %s"),
                    sfck, sbw, sd, sVci_abs, sVci_cal, m_strForce);                           m_arText.Add(strCums);		  
            }    		  
        }
    }
    else if(m_iVciType==2)
    {
        strCums = _T("<Equation> <Title>~ Vci_b2</Title> <Col>0</Col> </Equation>");  m_arText.Add(strCums);  
        m_iFormula++;
        strCums = _T("");   m_arText.Add(strCums);
        if(dMu==0.0)
        {
            // 수식.
            strCums.Format(_T("~ ~ ~ == 0.16 * √  ( %s ) * %s * %s / 10^3 = %s ~ %s"),
                sfck, sbw, sd, sVci_cal, m_strForce);                           m_arText.Add(strCums);
        }
        else
        {	
            if((dMu>0.0 && dVu>=0.0) || (dMu<0.0 && dVu<=0.0))
            {
                strCums.Format(_T("~ ~ ~ == 0.16 * √  ( %s ) * %s * %s / 10^3 + %s * ( %s / %s ) = %s ~ %s"),
                    sfck, sbw, sd, sVu, sMcr, sMu, sVci_cal, m_strForce);                           m_arText.Add(strCums);
            }
            else 
            {      
                strCums.Format(_T("~ ~ ~ == 0.16 * √  ( %s ) * %s * %s / 10^3 + abs ~ ( %s * ( %s / %s ) ) = %s ~ %s"),
                    sfck, sbw, sd, sVu, sMcr, sMu, sVci_cal, m_strForce);                           m_arText.Add(strCums);
            }    		  
        }
    }

    strCums = _T("");   m_arText.Add(strCums);

    strEqua = _T("<Equation> <Title>~ Vcimin</Title> <Col>17</Col> </Equation>");  m_arText.Add(strEqua);  
    m_iFormula++;
    strCums.Format(_T("~ ~ ~ ~ ~ == 0.45 * √  ( %s ) * %s * %s / 10^3 = %s ~ %s"), 
        sfck, sbw, sd, sminVci, m_strForce);    m_arText.Add(strCums);                                 
    strCums = _T("");   m_arText.Add(strCums);

    strCums.Format(_T("~ ~ ∴  ~ V_ci == %s ~ %s"), sVci, m_strForce);    m_arText.Add(strCums);
    strCums = _T("");   m_arText.Add(strCums);
}

void CPrtKSCE_USD03::Print_CumsDesign6(double dPe, double dAc, double dfpc, double dfck, double dbw, 
                                       double dd, double dVp, double dVcw, double dVci, double dVc)
{
    CString sPe; sPe.Format(_T("%.1f"), dPe); // Check_ValueForm(8,2, Cng_CodeForFix(dPe));
    CString sAc; sAc.Format(_T("%.1f"), dAc); // Check_ValueForm(8,1, Cng_CodeLenFix(dAc,2));
    CString sfpc = Check_ValueForm(7,1, dfpc);
    CString sfck = Check_ValueForm(7,1, dfck);
    CString sbw  = Check_ValueForm(7,1, dbw);
    CString sd   = Check_ValueForm(7,1, dd);
    CString sVp  = Check_ValueForm(7,1, Cng_CodeForFix(dVp));
    CString sVcw = Check_ValueForm(7,1, Cng_CodeForFix(dVcw));
    CString sVci = Check_ValueForm(7,1, Cng_CodeForFix(dVci));
    CString sVc  = Check_ValueForm(7,1, Cng_CodeForFix(dVc));

    CString strCums = _T("");
    strCums.Format(_T("~ f_pc == %s / %s = %s ~ %s"),sPe, sAc, sfpc, m_strStress);            m_arText.Add(strCums);
    strCums = _T("");   m_arText.Add(strCums);

    strCums = _T("<Equation> <Title>~ Vcw_b</Title> <Col>15</Col> </Equation>");   m_arText.Add(strCums);
    m_iFormula++;
    strCums.Format(_T("~ ~ ~ == ( 0.93 * √  ( %s ) + 0.3 ~ * %s ) * %s * %s / 10^3 + %s = %s ~ %s"),
        sfck, sfpc, sbw, sd, sVp, sVcw, m_strForce);        m_arText.Add(strCums);
    strCums = _T("");   m_arText.Add(strCums);

    strCums.Format(_T("~ V_c == Minimum ~ ( V_ci , V_cw ) == Min ~ ( %s , %s ) == %s ~ %s"), sVci, sVcw, sVc, m_strForce);     m_arText.Add(strCums);  
    strCums = _T("");   m_arText.Add(strCums);
}

void CPrtKSCE_USD03::Print_CumsDesign7(double dVc, double dphiv, double d05pVc, double dpVc, double dVu, double dbw, 
                                       double dd, double dVp, double dVcw, double dVci)
{
    CString sVc    = Check_ValueForm(7,1, Cng_CodeForFix(dVc));
    CString sphiv  = Check_ValueForm(7,1, dphiv);
    CString s05pVc = Check_ValueForm(7,1, Cng_CodeForFix(d05pVc));
    CString spVc   = Check_ValueForm(7,1, Cng_CodeForFix(dpVc));
    CString sVu    = Check_ValueForm(7,1, Cng_CodeForFix(dVu));
    CString sbw    = Check_ValueForm(7,1, dbw);
    CString sd     = Check_ValueForm(7,1, dd);
    CString sVp    = Check_ValueForm(7,1, Cng_CodeForFix(dVp));
    CString sVcw   = Check_ValueForm(7,1, Cng_CodeForFix(dVcw));
    CString sVci   = Check_ValueForm(7,1, Cng_CodeForFix(dVci));

    CString strCums = _T("");  
    strCums.Format(_T("~ 1/2Φ  V_c == 1 / 2 * %s * %s ~ = %s ~ %s"), sVc, sphiv, s05pVc, m_strForce);                    m_arText.Add(strCums);
    strCums = _T("");   m_arText.Add(strCums);
    strCums.Format(_T("~ Φ  V_c == %s ~ * %s = %s ~ %s"),sphiv, sVc, spVc, m_strForce);      m_arText.Add(strCums);
    strCums = _T("");   m_arText.Add(strCums);
    strCums.Format(_T("~ V_u == %s ~ %s"), sVu, m_strForce);      m_arText.Add(strCums);
    strCums = _T("");   m_arText.Add(strCums);

    strCums = Get_ShearReinforcePrint(dpVc, d05pVc, dVu);      m_arText.Add(strCums);

}

void CPrtKSCE_USD03::Print_CumsDesign8(double dVu, double dVc, double dphiv, double dpVc, double d05pVc, double dpVn, double dfck, double dbw, double dd, double dVs_req, double dVs_max,
                                       double dVs, double dVs_min, double dsv, double dfy, double dThetaV, double dAv_use, double dAv_req, double dAvmin)
{
    CString sVu     = Check_ValueForm(7,1, Cng_CodeForFix(fabs(dVu)));
    CString sVc     = Check_ValueForm(7,1, Cng_CodeForFix(dVc));
    CString sphiv   = Check_ValueForm(7,1, dphiv);
    CString spVc    = Check_ValueForm(7,1, Cng_CodeForFix(dpVc));
    CString s05pVc  = Check_ValueForm(7,1, Cng_CodeForFix(d05pVc));
    CString spVn    = Check_ValueForm(7,1, Cng_CodeForFix(dpVn));
    CString sfck    = Check_ValueForm(7,1, dfck);
    CString sbw     = Check_ValueForm(7,1, dbw);
    CString sd      = Check_ValueForm(7,1, dd);
    CString sVs_req = Check_ValueForm(7,1, Cng_CodeForFix(dVs_req));
    CString sVs_min = Check_ValueForm(7,1, Cng_CodeForFix(dVs_min));
    CString sVs_max = Check_ValueForm(7,1, Cng_CodeForFix(dVs_max));

    CString sVs     = Check_ValueForm(7,1, Cng_CodeForFix(dVs));
    CString ssv     = Check_ValueForm(7,1, dsv);
    CString sfy     = Check_ValueForm(7,1, dfy);
    CString sThetaV = Check_ValueForm(7,3, dThetaV);
    CString sAv_use = Check_ValueForm(7,1, dAv_use);
    CString sAv_req = Check_ValueForm(7,1, dAv_req);
    CString sAvmin  = Check_ValueForm(7,1, dAvmin);

    CString strCums = _T("");
    CString strEqua = _T("");

    strCums = _T("Text: (4) 철근이 부담해야 하는 전단력 산정 및 전단 철근량 산정");       m_arText.Add(strCums);
    strCums = _T("Text:  - 전단철근이 부담하는 전단력");                                m_arText.Add(strCums);
    double dRad_Deg90 = 4.*atan(1.)/180.*90.;
    if(fabs(dThetaV-dRad_Deg90)<m_dZero)
    {
        strCums = _T("<Equation> <Title>~ Vs_rece2</Title> <Col>15</Col> </Equation>");       m_arText.Add(strCums);
        m_iFormula++;
        strCums = _T("");                                                                   m_arText.Add(strCums);
        strCums.Format(_T("~ ~ ~ == %s * %s * %s / %s / 10^3 = %s ~ %s"),
            sAv_use, sfy, sd, ssv, sVs, m_strForce);        m_arText.Add(strCums);
    }
    else
    {
        strCums = _T("<Equation> <Title>~ Vs_rece</Title> <Col>15</Col> </Equation>");       m_arText.Add(strCums);
        m_iFormula++;
        strCums = _T("");                                                                   m_arText.Add(strCums);
        strCums.Format(_T("~ ~ ~ == %s * %s * ( sin ~ ( %s ) + cos ~ ( %s ) ) * %s / %s / 10^3 = %s ~ %s"),
            sAv_use, sfy, sThetaV, sThetaV, sd, ssv, sVs, m_strForce);        m_arText.Add(strCums);
    }
    strCums = _T("");   m_arText.Add(strCums);

    strEqua = _T("<Equation> <Title>~ Vsmax</Title> <Col>15</Col> </Equation>");  m_arText.Add(strEqua);
    m_iFormula++;
    strCums.Format(_T("~ ~ ~ ~ == 2.12 * √  ( %s ) * %s * %s / 10^3 = %s ~ %s"),
        sfck, sbw, sd, sVs_max, m_strForce);                      m_arText.Add(strCums);
    strCums = _T("");   m_arText.Add(strCums);

    strCums.Format(_T("~ V_s == Minimum ~ ( V_s , V_s,max ) == Min ~ ( %s , %s ) == %s ~ %s"), sVs, sVs_max, sVs_min, m_strForce);     m_arText.Add(strCums);  
    m_arText.Add(_T(""));

    strCums = _T("Text:  - 최소 전단 철근량");                                       m_arText.Add(strCums);
    strCums = _T("<Equation> <Title>~ Avmin_b</Title> <Col>15</Col> </Equation>");       m_arText.Add(strCums);
    m_iFormula++;
    strCums = _T("");                                                                   m_arText.Add(strCums);
    strCums.Format(_T("~ ~ ~ == 3.5 * %s * %s / %s = %s ~ %s"), sbw, ssv, sfy, sAvmin, m_strArea);    m_arText.Add(strCums);
    m_arText.Add(_T(""));

    //
    strCums = _T("Text:  - 전단 철근량");                                       m_arText.Add(strCums);
    strCums.Format(_T("~ 1/2Φ  V_c == 1 / 2 * %s * %s ~ = %s ~ %s"), sphiv, sVc, s05pVc, m_strForce);                    m_arText.Add(strCums);
    if(dpVc>fabs(dVu) && d05pVc< fabs(dVu))
    {
        strCums = _T("~ ∴  ~ ~ Φ  V_c ~ > ~ V_u ~ , ~ 1/2Φ  V_c  <  ~ V_u  ~ ~ ~ ~ ~ ~ ~ ~ ⇒  최소 전단철근 보강"); m_arText.Add(strCums);
    }
    else if(d05pVc>=fabs(dVu))
    {
        strCums = _T("~ ∴  ~ 1/2Φ  V_c ~ > ~ V_u  ~ ~ ~ ~ ~ ~ ~ ~ ⇒  전단철근 보강 불필요");     m_arText.Add(strCums);
    }  
    else if(dpVc<fabs(dVu))
    {
        strCums = _T("~ ∴  ~ Φ  V_c ~ < ~ V_u  ~ ~ ~ ~ ~ ~ ~ ~ ⇒  전단철근 보강");     m_arText.Add(strCums);
        m_arText.Add(_T(""));
        strEqua = _T("<Equation> <Title>~ Vs_rbar</Title> <Col>15</Col> </Equation>");  m_arText.Add(strEqua);
        m_iFormula++;
        strCums.Format(_T("~ ~ ~ == ( %s - %s ) / %s ~ = %s ~ %s"),
            sVu, spVc, sphiv, sVs_req, m_strForce);               m_arText.Add(strCums);
        m_arText.Add(_T(""));

        if(fabs(dThetaV-dRad_Deg90)<m_dZero)
        {
            strCums = _T("<Equation> <Title>~ Av_req2</Title> <Col>15</Col> </Equation>");       m_arText.Add(strCums);
            m_iFormula++;
            strCums = _T("");                                                                   m_arText.Add(strCums);
            strCums.Format(_T("~ ~ ~ == ( %s * %s ) / ( %s * %s ) = %s ~ %s"),
                sVs_req, ssv, sfy, sd, sAv_req, m_sTxArea);              m_arText.Add(strCums);
        }
        else
        {
            strCums = _T("<Equation> <Title>~ Av_req</Title> <Col>15</Col> </Equation>");       m_arText.Add(strCums);
            m_iFormula++;
            strCums = _T("");                                                                   m_arText.Add(strCums);
            strCums.Format(_T("~ ~ ~ == ( %s * %s ) / ( %s * ( sin ~ ( %s ) + cos ~ ( %s ) ) * %s ) = %s ~ %s"),
                sVs_req, ssv, sfy, sThetaV, sThetaV, sd, sAv_req, m_sTxArea);              m_arText.Add(strCums);
        }    
    }

    m_arText.Add(_T(""));

    strCums = Get_StrengthChkPrint(dpVn,dVu);       m_arText.Add(strCums);
    m_arText.Add(_T(""));
    m_arText.Add(_T(""));
    //strEqua = _T("<Equation> <Title>~ pVn</Title> <Col>15</Col> </Equation>");  m_arText.Add(strEqua);
    //strCums.Format(_T("~ ~ ~ ~ == %s * ( %s + %s ) = %s ~ %s"),sphiv, sVc, sVs , spVn, m_strForce);  m_arText.Add(strEqua);

}

CString CPrtKSCE_USD03::Get_dValuePrint(double dh, double d08h, double dd, double ddp)
{
    CString sh   = Check_ValueForm(6,1, dh);
    CString s08h = Check_ValueForm(6,1, d08h);
    CString sd   = Check_ValueForm(6,1, dd);
    CString sdp  = Check_ValueForm(6,1, ddp);  

    CString strd=_T("");
    if(d08h<=dd) strd.Format(_T("~ 0.8 ~ * %s = %s   <  ~ d == %s %s"),sh, s08h, sd, m_strLength);
    else         strd.Format(_T("~ 0.8 ~ * %s = %s   >  ~ d == %s %s"),sh, s08h, sd, m_strLength);

    return strd;
}

CString CPrtKSCE_USD03::Get_ShearReinforcePrint(double dpVc, double d05pVc, double dVu)
{
    CString strChk=_T("");
    if(dpVc>fabs(dVu) && d05pVc< fabs(dVu))
    {
        strChk = _T("~ ∴  ~ ~ Φ  V_c ~ > ~ V_u ~, ~ 1/2Φ  V_c  <  ~ V_u  ~ ~ ~ ~ ~ ~ ~ ~ ⇒  최소 전단철근 보강");
    }
    else if(d05pVc>=fabs(dVu))
    {
        strChk = _T("~ ∴  ~ 1/2Φ  V_c ~ > ~ V_u  ~ ~ ~ ~ ~ ~ ~ ~ ⇒  전단철근 보강 불필요");
    }  
    else if(dpVc<fabs(dVu))
    {
        strChk = _T("~ ∴  ~ Φ  V_c ~ < ~ V_u  ~ ~ ~ ~ ~ ~ ~ ~ ⇒  전단철근 보강");
    }

    return strChk;
}

CString CPrtKSCE_USD03::Get_StrengthChkPrint(double dpVn, double dVu)
{
    CString spVn = Check_ValueForm(7,1, Cng_CodeForFix(dpVn));
    CString sVu = Check_ValueForm(7,1, Cng_CodeForFix(dVu));

    CString strChk=_T("");
    if(dpVn>dVu) strChk.Format(_T("~ Φ  V_n == Φ  ( V_c + V_s ) == %s ~ %s ~ >  ~ V_u == %s ~ %s ~ ~ ~ ~ ~ O.K"), spVn, m_strForce, sVu, m_strForce);
    else         strChk.Format(_T("~ Φ  V_n == Φ  ( V_c + V_s ) == %s ~ %s ~ <  ~ V_u == %s ~ %s ~ ~ ~ ~ ~ N.G"), spVn, m_strForce, sVu, m_strForce);

    return strChk;
}


// 비틀림강도 검토(Crmt) /////////////////////////////////////////////////////////////////////////////////////////
void CPrtKSCE_USD03::Print_CrmtTheory(CArray<CString,CString>& aString)
{
    CString strCrmt = _T("");
    CString strEqua = _T("");
    CString strSpace = _T("");
    for(int i=0; i<30; i++)
        strSpace += _T("~ ");

    strCrmt = _T("Text:  ▷ 비틀림 검토");        aString.Add(strCrmt);
    strCrmt = _T("");   aString.Add(strCrmt);

    strCrmt = _T("Text:   - 비틀림 모멘트를 무시할 수 있는 경우");        aString.Add(strCrmt);
    strEqua = _T("<Equation> <Title>~ ~ ~ Tcr</Title> <Col>30</Col> </Equation> "); 
    strCrmt.Format(_T("%s P_cp : ~ 전단면의`둘레길이"),strEqua);                    aString.Add(strCrmt);
    strCrmt = strSpace + _T("A_cp : ~ 콘크리트`단면의`바깥둘레로`둘러싸인");      aString.Add(strCrmt);
    strCrmt = strSpace + _T("~ ~ ~ ~ ~ 폐합`단면적");                               aString.Add(strCrmt);
    strCrmt = _T("");   aString.Add(strCrmt);


    strCrmt = _T("Text:   - 비틀림 모멘트를 무시할 수 없는 경우");                           aString.Add(strCrmt);
    strCrmt = _T("");   aString.Add(strCrmt);

    strCrmt = _T("Text:    ① 비틀림에 대한 수직 철근");                           aString.Add(strCrmt);
    strEqua = _T("<Equation> <Title>~ ~ ~ Tn</Title> <Col>30</Col> </Equation> "); 
    strCrmt.Format(_T("%s A_o ~ : ~ 전단흐름에`따라`폐쇄된`단면적"),strEqua);                           aString.Add(strCrmt);
    strCrmt = strSpace + _T("A_oh : ~ 비틀림`저항철근의`중심선으로");          aString.Add(strCrmt);
    strCrmt = strSpace + _T("~ ~ ~ ~ ~ 폐합된`단면적");                                aString.Add(strCrmt);
    strEqua = _T("<Equation> <Title>~ ~ ~ Ao</Title> <Col>30</Col> </Equation> "); 
    strCrmt.Format(_T("%s A_t ~ : ~ 거리`s`내의`비틀림`저항`폐쇄스터럽의"),strEqua);                           aString.Add(strCrmt);
    strCrmt = strSpace + _T("~ ~ ~ ~ ~ 다리 1개 단면적");                           aString.Add(strCrmt);
    strCrmt = _T("");   aString.Add(strCrmt);

    strCrmt = _T("~ ~ ~ θ  : ~ 압축 경사각");                           aString.Add(strCrmt);
    strCrmt = _T("~ ~ ~ 프리스트레싱`되지`않은`부재 ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ θ == 45 ~ ˚");               aString.Add(strCrmt);
    strCrmt = _T("~ ~ ~ 프리스트레스`힘이`주철근`인장강도의`40%미만 ~ θ == 45 ~ ˚");    aString.Add(strCrmt);
    strCrmt = _T("~ ~ ~ 프리스트레스`힘이`주철근`인장강도의`40%이상 ~ θ == 37.5 ˚");  aString.Add(strCrmt);
    strCrmt = _T("");   aString.Add(strCrmt);

    strCrmt = _T("Text:    ② 비틀림에 대한 종방향 철근");                           aString.Add(strCrmt);
    strEqua = _T("<Equation> <Title>~ ~ ~ Al</Title> <Col>30</Col> </Equation> "); 
    strCrmt.Format(_T("%s A_t / s : ~ 최소비틀림`철근량을`적용하지`않음"),strEqua);               aString.Add(strCrmt);
    strCrmt = strSpace + _T("P_h ~ ~ : ~ 외곽부`폐합`비틀림`철근의`중심선`둘레길이");                           aString.Add(strCrmt);
    strCrmt = _T("");   aString.Add(strCrmt);


    strCrmt = _T("Text:   - 단면치수 제한");                            aString.Add(strCrmt);
    strCrmt = _T("Text:     Solid Section");                            aString.Add(strCrmt);
    strEqua = _T("<Equation> <Title>~ ~ ~ SectSol</Title> <Col>30</Col> </Equation> ");		aString.Add(strEqua); 
    strCrmt = _T("");   aString.Add(strCrmt);
    strCrmt = _T("");   aString.Add(strCrmt);
    strCrmt = _T("");   aString.Add(strCrmt);


    strCrmt = _T("Text:     Hollow Section");                            aString.Add(strCrmt);
    strCrmt = _T("~ ~ ~ ①`벽의`두께가` A_oh / p_h  ~ 보다`큰`경우");                             aString.Add(strCrmt);
    strEqua = _T("<Equation> <Title>~ ~ ~ SectHol1</Title> <Col>30</Col> </Equation> ");
    strCrmt.Format(_T("%s  t : 속 빈 단면의 벽 두께"),strEqua);                           aString.Add(strCrmt);
    strCrmt = _T("");   aString.Add(strCrmt);
    strCrmt = _T("");   aString.Add(strCrmt);

    strCrmt = _T("~ ~ ~ ②`벽의`두께가  A_oh / P_h 보다`작은`경우");                           aString.Add(strCrmt);
    strCrmt = _T("<Equation> <Title>~ ~ ~ SectHol2</Title> <Col>0</Col> </Equation> ");     aString.Add(strCrmt);
    strCrmt = _T("");   aString.Add(strCrmt);
    strCrmt = _T("");   aString.Add(strCrmt);


    strCrmt = _T("Text:   - 최소 비틀림 철근량");                             aString.Add(strCrmt);
    strCrmt = _T("Text:    ① 횡방향 최소 철근량");                           aString.Add(strCrmt);
    strCrmt = _T("<Equation> <Title>~ ~ ~ Atmin</Title> <Col>0</Col> </Equation> ");     aString.Add(strCrmt);
    strCrmt = _T("");   aString.Add(strCrmt);
    strCrmt = _T("");   aString.Add(strCrmt);

    strCrmt = _T("Text:    ② 종방향 최소 철근량");                           aString.Add(strCrmt);
    strEqua = _T("<Equation> <Title>~ ~ ~ Almin</Title> <Col>30</Col> </Equation> ");   
    strCrmt.Format(_T("%s A_t / s : ~ 최소비틀림`철근량을`적용"),strEqua);             aString.Add(strCrmt);
    strCrmt = strSpace + _T("A_t / s ≥ 1.75 * b_w / f_y");                    aString.Add(strCrmt);
    strCrmt = _T("");   aString.Add(strCrmt);
    /*
    strCrmt = _T("Text: ▷ 사인장 철근 배근");                               aString.Add(strCrmt);
    strCrmt = _T("A_s : 1개의 복부에 배치된 철근량");               aString.Add(strCrmt);
    strCrmt = _T("A_s1 : 복부 1개당 필요한 사인장 철근량");         aString.Add(strCrmt);
    strCrmt = _T("A_s2 : 복부에 생기는 휨모멘트에 저항하기");       aString.Add(strCrmt);
    strCrmt = _T("Text: 위해 필요한 철근량");                             aString.Add(strCrmt);
    strCrmt = _T("A_t : 비틀림 모멘트에 대한 복부 1개당");          aString.Add(strCrmt);
    strCrmt = _T("Text: 횡방향 필요 철근량");                             aString.Add(strCrmt);
    */
}

void CPrtKSCE_USD03::Print_CrmtDesign0()
{
    CString sfck = Check_ValueForm(8,2, m_CrmtParam.dfck);
    CString sbw  = Check_ValueForm(7,1, m_CrmtParam.dbw);
    CString st   = Check_ValueForm(7,1, m_CrmtParam.dt);
    CString sfy  = Check_ValueForm(8,2, m_CrmtParam.dfy);
    CString sh   = Check_ValueForm(7,1, m_CrmtParam.dh);
    CString sIy  = Check_ValueForm(9,3, Cng_CodeLenFix(m_CrmtParam.dIy,4));

    CString sfpu = Check_ValueForm(8,2, m_CrmtParam.dfpu);
    CString sd   = Check_ValueForm(7,1, m_CrmtParam.dd);
    CString sdp  = Check_ValueForm(7,1, m_CrmtParam.ddp);
    CString sAc  = Check_ValueForm(9,3, Cng_CodeLenFix(m_CrmtParam.dAc,2));
    CString sfpy = Check_ValueForm(8,2, m_CrmtParam.dfpy);
    CString syt  = Check_ValueForm(7,1, m_CrmtParam.dyt);
    CString sAps = Check_ValueForm(8,1, m_CrmtParam.dAps);
    CString sPcp = Check_ValueForm(9,1, m_CrmtParam.dPcp);
    CString sAcp = Check_ValueForm(9,3, Cng_CodeLenFix(m_CrmtParam.dAcp,2));

    CString sSpace4 = _T("~ ~ ~ ~ ");
    CString sSpace5 = _T("~ ~ ~ ~ ~ ");
    CString sSpace6 = _T("~ ~ ~ ~ ~ ~ ");
    CString sSpace7 = _T("~ ~ ~ ~ ~ ~ ~ ");
    CString sSpace8 = _T("~ ~ ~ ~ ~ ~ ~ ~ ");
    CString sSpace9 = _T("~ ~ ~ ~ ~ ~ ~ ~ ~ ");
    CString s1stSpace  = _T("");
    CString s2ndSpace  = _T("");

    int iMaxLength = 0;
    int arLength[3][5];

    int isfck   = arLength[0][0] = sfck.GetLength();
    int isfy    = arLength[0][1] = sfy.GetLength();
    int isfpu   = arLength[0][2] = sfpu.GetLength();
    int isbeta1 = arLength[0][3] = sfpy.GetLength();

    int isbw  = arLength[1][0] = sbw.GetLength();
    int ish   = arLength[1][1] = sh.GetLength();
    int isdp  = arLength[1][2] = sdp.GetLength();
    int isyt  = arLength[1][3] = syt.GetLength();
    int isPcp = arLength[1][4] = sPcp.GetLength();

    int ist   = arLength[2][0] = st.GetLength();
    int isIy  = arLength[2][1] = sIy.GetLength();
    int isAc  = arLength[2][2] = sAc.GetLength();
    int isAps = arLength[2][3] = sAps.GetLength();
    int isAcp = arLength[2][4] = sAcp.GetLength();

    // 1 Column.    
    for(int i=0; i<4; i++)  
        iMaxLength = max(iMaxLength,arLength[0][i]);  

    CString strfck, strfy, strfpu, strfpy;
    strfck  = Get_TextStringLength(iMaxLength, sfck);
    strfy   = Get_TextStringLength(iMaxLength, sfy);
    strfpu  = Get_TextStringLength(iMaxLength, sfpu);
    strfpy  = Get_TextStringLength(iMaxLength, sfpy);

    if(iMaxLength==10 || iMaxLength==11)    s1stSpace = sSpace4;
    else if(iMaxLength==9 || iMaxLength==8) s1stSpace = sSpace5;
    else if(iMaxLength==7)                  s1stSpace = sSpace6;
    else if(iMaxLength==6 || iMaxLength==5) s1stSpace = sSpace7;
    else if(iMaxLength==4 || iMaxLength==3) s1stSpace = sSpace8;
    else if(iMaxLength==2)                  s1stSpace = sSpace9;
    else ASSERT(0);

    CString s1colLength = _T("");
    for(int i=0; i<Get_XlLength(iMaxLength); i++)
        s1colLength += _T("~ ");

    s1colLength  = s1colLength + sSpace8 + sSpace5 + s1stSpace;

    // 2 Column.
    iMaxLength = 0;
    for(int i=0; i<5; i++)  
        iMaxLength = max(iMaxLength,arLength[1][i]);

    CString strbw, strh, strd, stryt, strPcp;
    strbw  = Get_TextStringLength(iMaxLength, sbw );
    strh   = Get_TextStringLength(iMaxLength, sh  );
    strd   = Get_TextStringLength(iMaxLength, sd  );
    stryt  = Get_TextStringLength(iMaxLength, syt );
    strPcp = Get_TextStringLength(iMaxLength, sPcp);

    if(iMaxLength==10 || iMaxLength==11)    s2ndSpace = sSpace4;
    else if(iMaxLength==9 || iMaxLength==8) s2ndSpace = sSpace5;
    else if(iMaxLength==7)                  s2ndSpace = sSpace6;
    else if(iMaxLength==6 || iMaxLength==5) s2ndSpace = sSpace7;
    else if(iMaxLength==4 || iMaxLength==3) s2ndSpace = sSpace8;
    else if(iMaxLength==2)                  s2ndSpace = sSpace9;

    // 3 Column.
    iMaxLength = 0;
    for(int i=0; i<5; i++)  
        iMaxLength = max(iMaxLength,arLength[2][i]);

    CString strt, strIy, strAc, strAps, strAcp;
    strt   = Get_TextStringLength(iMaxLength, st );
    strIy  = Get_TextStringLength(iMaxLength, sIy );
    strAc  = Get_TextStringLength(iMaxLength, sAc );
    strAps = Get_TextStringLength(iMaxLength, sAps);
    strAcp = Get_TextStringLength(iMaxLength, sAcp);

    if(st.GetLength()<=3)   strt += _T("~ ");
    if(sIy.GetLength()>=9)  strIy.TrimRight(_T("~ "));
    if(sAc.GetLength()>=9)  strAc.TrimRight(_T("~ "));
    if(sAcp.GetLength()>=9) strAcp.TrimRight(_T("~ "));

    CString strCrmt = _T("");
    strCrmt = _T("Text: 비틀림 강도 검토");
    strCrmt = _T("Text: (1) 설계 변수");                                                m_arText.Add(strCrmt);
    strCrmt.Format(_T("~ ~ f_ck : ~ %s%s %s b_w ~ : ~ %s%s %s t ~ ~ : ~ %s%s"),
        strfck, m_sTxStress, s1stSpace, strbw, m_strLength, s2ndSpace, strt, m_strLength);             m_arText.Add(strCrmt);
    strCrmt.Format(_T("~ ~ f_y ~ : ~ %s%s %s h ~ ~ : ~ %s%s %s I_y ~ : ~ %s %s"),
        strfy,m_sTxStress, s1stSpace, strh, m_strLength, s2ndSpace, strIy, m_sTxStif);    m_arText.Add(strCrmt);

    strCrmt.Format(_T("~ ~ f_pu : ~ %s%s %s d ~ ~ : ~ %s%s %s A_c ~ : ~ %s %s"),
        strfpu, m_sTxStress, s1stSpace, strd, m_strLength, s2ndSpace, strAc,m_sTxArea);    m_arText.Add(strCrmt);
    strCrmt.Format(_T("~ ~ f_py : ~ %s%s %s y_t ~ : ~ %s%s %s A_p ~ : ~ %s%s"),
        strfpy, m_sTxStress, s1stSpace, stryt,m_strLength, s2ndSpace, strAps,m_sTxArea);    m_arText.Add(strCrmt);
    strCrmt.Format(_T("%s P_cp : ~ %s%s %s A_cp : ~ %s %s "),
        s1colLength, strPcp, m_strLength, s2ndSpace, strAcp, m_sTxArea);    m_arText.Add(strCrmt);
    strCrmt = _T("");   m_arText.Add(strCrmt);

}

void CPrtKSCE_USD03::Print_CrmtDesign1(double dfck, double dbw, double dt , double dfy, double dh, double dIy)
{
    CString sfck = Check_ValueForm(7,1, dfck);
    CString sbw  = Check_ValueForm(7,1, dbw);
    CString st   = Check_ValueForm(7,1, dt);
    CString sfy  = Check_ValueForm(7,1, dfy);
    CString sh   = Check_ValueForm(7,1, dh);
    CString sIy  = Check_ValueForm(7,1, Cng_CodeLenFixM(dIy,4));

    CString strCrmt = _T("");
    strCrmt = _T("Text: 비틀림 강도 검토");
    strCrmt = _T("Text: (1) 설계 변수");                                                m_arText.Add(strCrmt);
    strCrmt.Format(_T("~ ~ f_ck : ~ %s ~ ~ %s ~ ~ ~ ~ ~ b_w ~ : ~ %s ~ %s ~ ~ ~ ~ ~ t ~ ~ : ~ %s ~ ~ ~ %s"),
        sfck,m_strStress, sbw,m_strLength, st,m_strLength);             m_arText.Add(strCrmt);
    strCrmt.Format(_T("~ ~ f_y ~ : ~ %s ~ ~ %s ~ ~ ~ ~ ~ h ~ ~ : ~ %s ~ %s ~ ~ ~ ~ ~ I_y ~ : ~ %s ~ ~ ~ %s"),
        sfy,m_strStress, sh,m_strLength, sIy,m_strStifM);    m_arText.Add(strCrmt);
}

void CPrtKSCE_USD03::Print_CrmtDesign2(double dfpu, double dd, double ddp, double dAc, double dfpy, double dyt, double dAps, 
                                       double dPcp, double dAcp)
{
    CString sfpu = Check_ValueForm(7,1, dfpu);
    CString sd   = Check_ValueForm(7,1, dd);
    CString sdp  = Check_ValueForm(7,1, ddp);
    CString sAc  = Check_ValueForm(7,1, Cng_CodeLenFixM(dAc,2));
    CString sfpy = Check_ValueForm(7,1, dfpy);
    CString syt  = Check_ValueForm(7,1, dyt);
    CString sAps = Check_ValueForm(7,1, dAps);
    CString sPcp = Check_ValueForm(7,1, dPcp);
    CString sAcp = Check_ValueForm(7,1, Cng_CodeLenFixM(dAcp,2));

    CString strCrmt = _T("");
    strCrmt.Format(_T("~ ~ f_pu : ~ %s ~ %s ~ ~ ~ ~ ~ d ~ ~ : ~ %s ~ %s ~ ~ ~ ~ ~ A_c ~ : ~ %s ~ ~ ~ %s"),
        sfpu,m_strStress, sdp,m_strLength, sAc,m_strAreaM);    m_arText.Add(strCrmt);
    strCrmt.Format(_T("~ ~ f_py : ~ %s ~ %s ~ ~ ~ ~ ~ y_b ~ : ~ %s ~ %s ~ ~ ~ ~ ~ A_ps : ~ %s ~ ~ ~ %s"),
        sfpy,m_strStress, syt,m_strLength , sAps,m_strArea);    m_arText.Add(strCrmt);
    strCrmt.Format(_T("~ ~ P_cp : ~ %s ~ ~ %s ~ ~ ~ ~ ~ ~ ~ ~ ~ A_cp : ~ %s %s "),
        sPcp,m_strLength, sAcp,m_strAreaM);    m_arText.Add(strCrmt);
    strCrmt = _T("");   m_arText.Add(strCrmt);
}

void CPrtKSCE_USD03::Print_CrmtDesign3(double dMu, double dTu, double dVu, double dMd, double dVd, double dfd, double dPe, double dfpe)
{
    CString sMu  = Check_ValueForm(7,1, Cng_CodeMomFix(dMu));
    CString sTu  = Check_ValueForm(7,1, Cng_CodeMomFix(dTu));
    CString sVu  = Check_ValueForm(7,1, Cng_CodeForFix(dVu));
    CString sMd  = Check_ValueForm(7,1, Cng_CodeMomFix(dMd));
    CString sVd  = Check_ValueForm(7,1, Cng_CodeForFix(dVd));
    CString sfd  = Check_ValueForm(7,1, dfd);
    CString sPe  = Check_ValueForm(7,1, Cng_CodeForFix(dPe));
    CString sfpe = Check_ValueForm(7,1, dfpe);

    CString strStress=_T("kgf/cm²");
    CString strCrmt = _T("");
    CString strDum1, strDum2, strDum3;
    strCrmt = _T("Text: (2) 설계 단면력");                 m_arText.Add(strCrmt);
    strCrmt = _T("<Table>설계 단면력");                                                                   m_arText.Add(strCrmt);  
    if(m_iVciType==1)
    {
        strDum1 = _T("<MH>계수하중  ~ ~ ~ ~ ~ ~   ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~</MH> ");
        strDum2 = _T("<MH>사용하중  ~ ~ ~ ~ ~ ~   ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~</MH> ");
        strDum3 = _T("<MH>유효프리스트레스력  ~ ~ ~ ~ ~ ~   ~ ~ ~ ~ ~ ~ ~</MH> ");
        strCrmt = strDum1 + strDum2 + strDum3;                  m_arText.Add(strCrmt);
        strDum1 = _T("<MH>Mu ~ ~ ~ ~ ~ ~</MH> <MH>Tu ~ ~ ~ ~ ~ ~</MH> <MH>Vu ~ ~ ~ ~ ~ ~</MH> ");
        strDum2 = _T("<MH>Md ~ ~ ~ ~ ~ ~</MH> <MH>Vd ~ ~ ~ ~ ~ ~</MH> <MH>fd ~ ~ ~ ~ ~ ~</MH> ");
        strDum3 = _T("<MH>Pe ~ ~ ~ ~ ~ ~</MH> <MH>fpe ~ ~ ~ ~ ~ ~</MH>");
        strCrmt = strDum1 + strDum2 + strDum3;                  m_arText.Add(strCrmt);
        strDum1.Format(_T("<MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH> "),
            m_strMoment, m_strMoment, m_strForce);
        strDum2.Format(_T("<MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH> "),
            m_strMoment, m_strForce, strStress);
        strDum3.Format(_T("<MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH>"),m_strForce, strStress);
        strCrmt = strDum1 + strDum2 + strDum3;                  m_arText.Add(strCrmt);
        strDum1.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH> "),
            sMu, sTu, sVu);
        strDum2.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH> "),
            sMd, sVd, sfd);
        strDum3.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH>"),sPe,sfpe);
        strCrmt = strDum1 + strDum2 + strDum3;                  m_arText.Add(strCrmt);
    }
    else if(m_iVciType==2)
    {
        strDum1 = _T("<MH>계수하중  ~ ~ ~ ~ ~ ~   ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~</MH> ");
        strDum3 = _T("<MH>유효프리스트레스력  ~ ~ ~ ~ ~ ~   ~ ~ ~ ~ ~ ~ ~</MH> ");
        strCrmt = strDum1 + strDum3;                  m_arText.Add(strCrmt);
        strDum1 = _T("<MH>Mu ~ ~ ~ ~ ~ ~</MH> <MH>Tu ~ ~ ~ ~ ~ ~</MH> <MH>Vu ~ ~ ~ ~ ~ ~</MH> ");
        strDum3 = _T("<MH>Pe ~ ~ ~ ~ ~ ~</MH> <MH>fpe ~ ~ ~ ~ ~ ~</MH>");
        strCrmt = strDum1 + strDum3;                  m_arText.Add(strCrmt);
        strDum1.Format(_T("<MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH> "),
            m_strMoment, m_strMoment, m_strForce);
        strDum3.Format(_T("<MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH>"),m_strForce, strStress);
        strCrmt = strDum1 + strDum3;                  m_arText.Add(strCrmt);
        strDum1.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH> "),
            sMu, sTu, sVu);
        strDum3.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH>"),sPe,sfpe);
        strCrmt = strDum1 + strDum3;                  m_arText.Add(strCrmt);
    }
    strCrmt = _T("</Table>");    m_arText.Add(strCrmt);
    strCrmt = _T("");            m_arText.Add(strCrmt); 

}

void CPrtKSCE_USD03::Print_CrmtDesign4(double dfck, double dAcp, double dPcp, double dfpc, double dTcr, double dTu, double dfpe)
{
    CString sfck = Check_ValueForm(7,1, dfck);
    CString sAcp;  sAcp.Format(_T("%.1f"), dAcp); // Check_ValueForm(7,1, dAcp);
    CString sPcp = Check_ValueForm(7,1, dPcp);
    CString sfpc = Check_ValueForm(7,2, dfpc);
    CString sTcr = Check_ValueForm(7,1, Cng_CodeForFix(dTcr));
    CString sTu  = Check_ValueForm(7,1, Cng_CodeForFix(dTu));
    CString sfpe = Check_ValueForm(7,1, dfpe);

    CString strCrmt = _T("");
    CString strEqua = _T("");
    strCrmt = _T("Text: (3) 비틀림을 검토해야 하는 경우");                           m_arText.Add(strCrmt);
    strEqua = _T("<Equation> <Title>~ Tcr</Title> <Col>0</Col> </Equation> ");      m_arText.Add(strEqua);
    m_iFormula++;
    strCrmt = _T("");   m_arText.Add(strCrmt);   m_arText.Add(strCrmt);
    strCrmt.Format(_T("~ ~ == 0.8 ~ * 0.26 * √  ( %s ) * %s^2 / %s * √  ( 1 + %s / ( 1.06 * √  ( %s ) ) ) / 10^5"),
        sfck, sAcp, sPcp, sfpc, sfck);    m_arText.Add(strCrmt);
    strCrmt = Get_TorChkPrint(dTcr, dTu);                     m_arText.Add(strCrmt);
    strCrmt = _T("");            m_arText.Add(strCrmt);
}

void CPrtKSCE_USD03::Print_CrmtDesign5()
{
    CString strCrmt = _T("");
    strCrmt = _T("Text: (4) 콘크리트가 부담하는 전단강도의 산정");       m_arText.Add(strCrmt);
    strCrmt = _T("");            m_arText.Add(strCrmt);
}

void CPrtKSCE_USD03::Print_CrmtDesign6(BOOL bHollow, double dAohOverPh, double dVu, double dVc, double dbw, double dd, double ddp, double dTu,
                                       double dfck, double dTcr, double dPh, double dAoh, double dt, double dvVT, double dvVTmax)
{
    CString sAohOverPh = Check_ValueForm(7,1, dAohOverPh);
    CString sVu  = Check_ValueForm(7,1, Cng_CodeForFix(dVu));
    CString sVc  = Check_ValueForm(7,1, Cng_CodeForFix(dVc));
    CString sbw  = Check_ValueForm(7,1, dbw);
    CString sd   = Check_ValueForm(7,1, dd);
    CString sdp  = Check_ValueForm(7,1, ddp);
    CString sTu  = Check_ValueForm(7,1, Cng_CodeMomFix(dTu));
    CString sfck = Check_ValueForm(7,1, dfck);
    CString sTcr = Check_ValueForm(7,1, Cng_CodeMomFix(dTcr));
    CString sPh  = Check_ValueForm(7,1, dPh);
    CString sAoh; sAoh.Format(_T("%.1f"), dAoh); // Check_ValueForm(7,1, dAoh);
    CString st   = Check_ValueForm(7,1, dt);
    CString svVT = Check_ValueForm(7,1, dvVT);
    CString svVTmax  = Check_ValueForm(7,1, dvVTmax);

    CString strCrmt = _T("");
    CString strEqua = _T("");
    CString str1, str2;
    strCrmt = _T("Text: (5) 단면의 치수 제한");       m_arText.Add(strCrmt);
    if(!bHollow)
    {
        strCrmt = _T("Text: - 속찬 단면");   m_arText.Add(strCrmt);      
        strEqua = _T("<Equation> <Title>~ SectSol</Title> <Col>0</Col> </Equation> ");  m_arText.Add(strEqua);
        m_iFormula++;
        strCrmt = _T("");   m_arText.Add(strCrmt); m_arText.Add(strCrmt);
        str1.Format(_T("( %s / ( %s * %s ) )^2"), sVu, sbw, sd);  
        str2.Format(_T("( %s * %s / ( 1.7 * %s^2 ) )^2"), sTu, sPh, sAoh);
        strCrmt.Format(_T("√  ( %s + %s ) = %s"), str1, str2, svVT);     m_arText.Add(strCrmt);
        strCrmt = _T(""); m_arText.Add(strCrmt);
    }
    else
    {
        strCrmt = _T("Text: - 속빈 단면");     m_arText.Add(strCrmt);
        if(dt >= dAohOverPh)
        {
            strCrmt = _T("~ - 벽의 두께가 A_oh / P_h 보다 큰 경우"); m_arText.Add(strCrmt);
            strEqua = _T("<Equation> <Title>~ SectHol1</Title> <Col>0</Col> </Equation> ");  m_arText.Add(strEqua);
            m_iFormula++;
            strCrmt = _T("");   m_arText.Add(strCrmt); m_arText.Add(strCrmt);
            str1.Format(_T("%s / ( %s * %s )"), sVu, sbw, sd);  
            str2.Format(_T("%s * %s / ( 1.7 * %s^2 )"),sTu, sPh , sAoh);
            strCrmt.Format(_T("%s + %s = %s"), str1, str2, svVT);     m_arText.Add(strCrmt);                
            strCrmt = _T(""); m_arText.Add(strCrmt);
        }
        if(dt< dAohOverPh)
        {
            strCrmt = _T("~ - 벽의 두께가 A_oh / P_h 보다 작은 경우"); m_arText.Add(strCrmt);
            strEqua = _T("<Equation> <Title>~ SectHol2</Title> <Col>0</Col> </Equation> ");  m_arText.Add(strEqua);
            m_iFormula++;
            strCrmt = _T("");   m_arText.Add(strCrmt); m_arText.Add(strCrmt);


            str1.Format(_T("%s / ( %s * %s )"), sVu, sbw, sd);
            str2.Format(_T("%s * %s / ( 1.7 * %s * %s )"),sTu, sPh , sAoh, st);
            strCrmt.Format(_T("%s + %s = %s"), str1, str2, svVT);     m_arText.Add(strCrmt);                
            strCrmt = _T(""); m_arText.Add(strCrmt);
        }

    }
    str1.Format(_T("%s / ( %s * %s )"), sVc, sbw, sd);  
    str2.Format(_T("2.12 * √  ( %s )"), sfck);
    strCrmt.Format(_T("~ %s  ~ 0.8 ~ * ( %s + %s ) = %s ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ %s"), (dvVT<=dvVTmax ? _T("≤") : _T("≥")),
        str1, str2, svVTmax, (dvVT<=dvVTmax ? _T("O.K") : _T("N.G")) ); m_arText.Add(strCrmt);

    strCrmt = _T(""); m_arText.Add(strCrmt);

}

void CPrtKSCE_USD03::Print_CrmtDesign7(double dAoh, double dAo, double dAt, double dfyv, double dst, double dThetaC,
                                       double dpTn, double dfyl, double dPh, double dAlmin)
{
    CString sAoh; sAoh.Format(_T("%.1f"), dAoh); // Check_ValueForm(9,1, dAoh);
    CString sAo;  sAo.Format(_T("%.1f"), dAo); // Check_ValueForm(9,1, dAo);
    CString sAt  = Check_ValueForm(7,1, dAt);
    CString sfyv = Check_ValueForm(7,1, dfyv);
    CString sst  = Check_ValueForm(7,1, dst);
    CString sThetaC = Check_ValueForm(7,1, dThetaC);
    CString spTn = Check_ValueForm(7,1, Cng_CodeMomFix(dpTn));
    CString sfyl = Check_ValueForm(7,1, dfyl);
    CString sPh  = Check_ValueForm(7,1, dPh);
    CString sAlmin  = Check_ValueForm(7,1, dAlmin);

    CString strCrmt = _T("");
    CString strEqua = _T("");
    strCrmt = _T("Text: (6) 비틀림 대한 보강 철근");       m_arText.Add(strCrmt);

    strCrmt = _T("Text:  - 비틀림에 대한 수직 철근");      m_arText.Add(strCrmt);

    strEqua = _T("<Equation> <Title>~ Ao</Title> <Col>9</Col> </Equation> ");  m_arText.Add(strEqua);
    m_iFormula++;
    strCrmt.Format(_T("~ ~ ~ == 0.85 * %s = %s ~ %s"), sAoh, sAo, m_strArea);  m_arText.Add(strCrmt);

    strEqua = _T("<Equation> <Title>~ Tn</Title> <Col>0</Col> </Equation> ");   m_arText.Add(strEqua);
    m_iFormula++;
    strCrmt = _T(""); m_arText.Add(strCrmt);
    strCrmt.Format(_T("~ ~ ~ == 2 * %s * %s * %s / %s * ( 1 / tan ~ ( %s ) ) = %s ~ %s"),
        sAo, sAt, sfyv, sst, sThetaC, spTn, m_strMoment);    m_arText.Add(strCrmt);
    strCrmt = _T("");            m_arText.Add(strCrmt);

    strCrmt = _T("Text:  - 비틀림에 대한 종방향 철근");      m_arText.Add(strCrmt);

    strEqua = _T("<Equation> <Title>~ Al</Title> <Col>0</Col> </Equation> ");   m_arText.Add(strEqua);
    m_iFormula++;
    strCrmt = _T(""); m_arText.Add(strCrmt);
    strCrmt.Format(_T("~ ~ ~ == ( %s / %s ) * %s * ( %s / %s ) * ( 1 / tan ~ ( %s ) )^2 = %s ~ %s"),
        sAt, sst, sPh, sfyv, sfyl, sThetaC, sAlmin, m_strArea);   m_arText.Add(strCrmt);
    strCrmt = _T(""); m_arText.Add(strCrmt);
}

void CPrtKSCE_USD03::Print_CrmtDesign8(double dbw, double dst, double dfyv, double dAv_use, double dAtmin, 
                                       double dfck, double dAcp, double dAt, double dPh, double dfyl, double dAlmin,
                                       double dAts_cal, double dbwfyv, double dAts)
{
    CString sbw     = Check_ValueForm(7,1, dbw);
    CString sst     = Check_ValueForm(7,1, dst);
    CString sfyv    = Check_ValueForm(7,1, dfyv);
    CString sAv_use = Check_ValueForm(7,1, dAv_use);
    CString sAtmin  = Check_ValueForm(7,1, dAtmin);
    CString sfck    = Check_ValueForm(7,1, dfck);
    CString sAcp;   sAcp.Format(_T("%.1f"), dAcp); // Check_ValueForm(7,1, Cng_CodeLenFix(dAcp,2));
    CString sAt     = Check_ValueForm(7,1, dAt);
    CString sPh;    sPh.Format(_T("%.1f"), dPh); // Check_ValueForm(8,1, dPh);
    CString sfyl    = Check_ValueForm(7,1, dfyl);
    CString sAlmin  = Check_ValueForm(7,1, dAlmin);
    CString sAts_cal= Check_ValueForm(9,1, dAts_cal);
    CString sbwfyv  = Check_ValueForm(9,1, dbwfyv);
    CString sAts    = Check_ValueForm(9,1, dAts);


    CString strCrmt = _T("");
    CString strEqua = _T("");
    CString str1, str2;
    strCrmt = _T("Text: (7) 최소 비틀림 철근량");       m_arText.Add(strCrmt);

    strCrmt = _T("Text:  - 횡방향 최소 철근량");       m_arText.Add(strCrmt);

    strEqua = _T("<Equation> <Title>~ Atmin</Title> <Col>0</Col> </Equation> ");   m_arText.Add(strEqua);    
    m_iFormula++;
    strCrmt = _T(""); m_arText.Add(strCrmt);
    strCrmt.Format(_T("~ ~ ~ == ( 3.5 * %s * %s / %s - %s ) / 2 = %s ~ %s"),
        sbw, sst, sfyv, sAv_use, sAtmin, m_strArea);    m_arText.Add(strCrmt);
    strCrmt = _T("");            m_arText.Add(strCrmt);

    strCrmt = _T("Text:  - 종방향 최소 철근량");                                            m_arText.Add(strCrmt);
    strCrmt.Format(_T("~ A_t / s == %s / %s = %s"), sAt, sst, sAts_cal);                    m_arText.Add(strCrmt);
    strCrmt.Format(_T("~ 1.75 b_w / f_yv == 1.75 * %s / %s = %s"), sbw, sfyv, sbwfyv);       m_arText.Add(strCrmt);
    strCrmt.Format(_T("~ A_t / s == Max ( %s , %s ) == %s"), sAts_cal, sbwfyv, sAts);          m_arText.Add(strCrmt);
    m_arText.Add(_T(""));

    strEqua = _T("<Equation> <Title>~ Almin</Title> <Col>0</Col> </Equation> ");   m_arText.Add(strEqua);    
    m_iFormula++;
    strCrmt = _T(""); m_arText.Add(strCrmt);
    str1.Format(_T("~ ~ ~ == 1.32 * √  ( %s ) * %s / %s "), sfck, sAcp, sfyl);
    str2.Format(_T("- ( %s / %s ) * %s * ( %s / %s ) = %s ~ %s"),
        sAt, sst, sPh, sfyv, sfyl, sAlmin, m_strArea);
    strCrmt = str1 + str2; m_arText.Add(strCrmt);
}

CString CPrtKSCE_USD03::Get_TorChkPrint(double dTcr, double dTu)
{
    CString sTcr = Check_ValueForm(9,1,Cng_CodeMomFix(dTcr));
    CString sTu = Check_ValueForm(9,1,Cng_CodeMomFix(dTu));

    CString strCrmt = _T("");
    if(dTcr >= fabs(dTu)) strCrmt.Format(_T("~ ~ == %s ~ %s  ~ ＞ ~ T_u == %s ~ %s ~ ~ ~ ~ ~ ~ ⇒ 비틀림 고려 불필요"),sTcr,m_strMoment, sTu,m_strMoment);
    else                  strCrmt.Format(_T("~ ~ == %s ~ %s  ~ ＜ ~ T_u == %s ~ %s ~ ~ ~ ~ ~ ~ ⇒ 비틀림 고려 필요"),sTcr,m_strMoment, sTu,m_strMoment);

    return strCrmt;
}

CString CPrtKSCE_USD03::Check_ValueForm(int iTotal, int iUnder, double dValue)
{
    double c_Zero	= 1.0E-07;
    CString strValue=_T("");
    BOOL bChange=FALSE;
    if(iUnder==0)
    {
        if(fabs(dValue) >= 1.0*pow(10,iTotal))	bChange = TRUE;
        else if(fabs(dValue) < 1.0*pow(10,0))		bChange = TRUE;
    }
    else
    {
        if(fabs(dValue) >= 1.0*pow(10,iTotal-iUnder-1))	bChange = TRUE;
        else if(fabs(dValue) < 1.0*pow(10,-iUnder))			bChange = TRUE;
    }
    // Add by ZINU.('00.9.5).
    if(fabs(dValue) < pow(c_Zero,3))	bChange = FALSE;

    // 	char chTotal[2]=_T(""), chUnder[2]=_T("");
    // 	_itoa(iTotal, chTotal, 10);
    //  	_itoa(iUnder, chUnder, 10);
    // 	CString strTotal(chTotal);
    // 	CString strUnder(chUnder);

    CString strSymbol=_T("");
    if(iUnder==0 && !bChange)
    {
        //     strSymbol = _T("%") + strTotal + _T("d");
        //   	strValue.Format(strSymbol, int(dValue));
        strSymbol.Format(_T("%%%dd"), iTotal);
        strValue.Format(strSymbol, int(dValue));

    }
    else
    {
        //     if(bChange)	strSymbol = _T("%") + strTotal + _T(".") + strUnder + _T("e");
        // 	  else				strSymbol = _T("%") + strTotal + _T(".") + strUnder + _T("f");
        //   	strValue.Format(strSymbol, dValue);
        if(bChange)	strSymbol.Format(_T("%%%d.%de"), iTotal, iUnder);
        else				strSymbol.Format(_T("%%%d.%df"), iTotal, iUnder);
        strValue.Format(strSymbol, dValue);
    }

    strValue.Replace(_T("e+0"),_T("E+"));
    strValue.Replace(_T("e-0"),_T("E-"));

    strValue.Remove(' ');

    return strValue;
}

BOOL CPrtKSCE_USD03::Get_ChartForceDiagram(CArray<Prt_Graph_Data,Prt_Graph_Data&>& arGraphData)
{
    arGraphData.RemoveAll();
    CArray<CString,CString> strTitle;
    CArray<CString,CString> strCol;
    CArray<double,double> dValue;
    CArray<ChartData,ChartData> arForcDiag;

    CArray<_PRT_FORC_UENV_ELEM,_PRT_FORC_UENV_ELEM> arUenvElemMax;
    CArray<_PRT_FORC_UENV_ELEM,_PRT_FORC_UENV_ELEM> arUenvElemMin;

    BOOL bDiagram=FALSE;
    if(!Get_ForcDiagram(arUenvElemMax,arUenvElemMin)) return FALSE;
    if(arUenvElemMax.GetSize()==0) return FALSE;

    int i=0,j=0,k=0,l=0;
    int TotalElem = arUenvElemMax.GetSize();
    for(l=0; l<3; l++) //BMD, SFD, TMD
    {
        // Initialize.
        strTitle.RemoveAll();
        strCol.RemoveAll();
        dValue.RemoveAll();
        arForcDiag.RemoveAll();

        CString sElem, sNode, sYMax, sYMin;
        CString sChartTitle;
        CString sYaxis;
        double dMinXValue = 0.0; 
        double dMaxXValue = 0.0; 

        int iStaRow = 2;

        CString strTemp;
        switch(l)   //0:BMD, 1:SFD, 2:TMD 
        {
        case 0:
            sElem=_T("M"); sNode=_T("N"); sYMax=_T("O"); sYMin=_T("P");
            sChartTitle=_T("휨 모멘트도");  
            sYaxis.Format(_T("Moment(%s)"), m_strMomentUnit);
            strTemp.Format(_T("%s%d"),sElem,iStaRow);  strTitle.Add(strTemp);      
            strTemp = _T("Element");   strTitle.Add(strTemp);
            strTemp.Format(_T("%s%d"),sNode,iStaRow);  strTitle.Add(strTemp);      
            strTemp = _T("I/J");   strTitle.Add(strTemp);
            strTemp.Format(_T("%s%d"),sYMax,iStaRow);  strTitle.Add(strTemp);
            strTemp = _T("Max");   strTitle.Add(strTemp);
            strTemp.Format(_T("%s%d"),sYMin,iStaRow);  strTitle.Add(strTemp);
            strTemp = _T("Min");   strTitle.Add(strTemp);
            break;
        case 1:
            sElem=_T("R"); sNode=_T("S"); sYMax=_T("T"); sYMin=_T("U");
            sChartTitle=_T("전단력도");
            sYaxis.Format(_T("Shear(%s)"), m_strForceUnit);
            strTemp.Format(_T("%s%d"),sElem,iStaRow);  strTitle.Add(strTemp);      
            strTemp = _T("Element");   strTitle.Add(strTemp);
            strTemp.Format(_T("%s%d"),sNode,iStaRow);  strTitle.Add(strTemp);      
            strTemp = _T("I/J");   strTitle.Add(strTemp);
            strTemp.Format(_T("%s%d"),sYMax,iStaRow);  strTitle.Add(strTemp);
            strTemp = _T("Max");   strTitle.Add(strTemp);
            strTemp.Format(_T("%s%d"),sYMin,iStaRow);  strTitle.Add(strTemp);
            strTemp = _T("Min");   strTitle.Add(strTemp);      
            break;
        case 2:
            sElem=_T("W"); sNode=_T("X"); sYMax=_T("Y"); sYMin=_T("Z");
            sChartTitle=_T("비틀림 모멘트도");
            sYaxis.Format(_T("Torsion(%s)"), m_strMomentUnit);
            strTemp.Format(_T("%s%d"),sElem,iStaRow);  strTitle.Add(strTemp);      
            strTemp = _T("Element");   strTitle.Add(strTemp);
            strTemp.Format(_T("%s%d"),sNode,iStaRow);  strTitle.Add(strTemp);      
            strTemp = _T("I/J");   strTitle.Add(strTemp);
            strTemp.Format(_T("%s%d"),sYMax,iStaRow);  strTitle.Add(strTemp);
            strTemp = _T("Max");   strTitle.Add(strTemp);
            strTemp.Format(_T("%s%d"),sYMin,iStaRow);  strTitle.Add(strTemp);
            strTemp = _T("Min");   strTitle.Add(strTemp);            
            break;
        }

        _DGN_FORC_CRC ForceMax; ForceMax.Initialize();
        _DGN_FORC_CRC ForceMin; ForceMin.Initialize();

        double dNode[2];
        for(i=0; i<TotalElem; i++)
        {
            T_ELEM_K ElemK = arUenvElemMax.GetAt(i).ElemK;      
            T_ELEM_D ElemD;
            if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;
            int nNode= m_pDoc->m_pAttrCtrl->GetNumNode(ElemD.eltyp, ElemD.elnod);
            for(j=0; j<nNode; j++)
            {
                dNode[j] = ElemD.elnod[j];          
            }

            if(i==0)            dMinXValue = dNode[0] - 1;
            if(i==TotalElem-1)  dMaxXValue = dNode[1] + 1;

            double dForcMax[4];
            double dForcMin[4];
            dForcMax[0] = dForcMax[3] = 0.0;  //dummy
            dForcMin[0] = dForcMin[3] = 0.0;  //dummy
            switch(l)   //0:BMD, 1:SFD, 2:TMD 
            {
            case 0:
                ForceMax = arUenvElemMax.GetAt(i).ForcMy; 
                ForceMin = arUenvElemMin.GetAt(i).ForcMy; 
                dForcMax[1] = ForceMax.dMuy[0];  //1:i 
                dForcMax[2] = ForceMax.dMuy[1];  //2:j
                dForcMin[1] = ForceMin.dMuy[0];  //1:i 
                dForcMin[2] = ForceMin.dMuy[1];  //2:j
                break;
            case 1:
                ForceMax = arUenvElemMax.GetAt(i).ForcVz; 
                ForceMin = arUenvElemMin.GetAt(i).ForcVz; 
                dForcMax[1] = ForceMax.dFzz[0];  //1:i 
                dForcMax[2] = ForceMax.dFzz[1];  //2:j
                dForcMin[1] = ForceMin.dFzz[0];  //1:i 
                dForcMin[2] = ForceMin.dFzz[1];  //2:j
                break;
            case 2:
                ForceMax = arUenvElemMax.GetAt(i).ForcMx; 
                ForceMin = arUenvElemMin.GetAt(i).ForcMx; 
                dForcMax[1] = ForceMax.dMux[0];  //1:i 
                dForcMax[2] = ForceMax.dMux[1];  //2:j
                dForcMin[1] = ForceMin.dMux[0];  //1:i 
                dForcMin[2] = ForceMin.dMux[1];  //2:j
                break;
            }

            CString sAcol, sBcol, sCcol, sDcol;
            for(j=0; j<2; j++)
            {
                for(k=0; k<2; k++)
                {
                    sAcol.Format(_T("%s%d"),sElem, iStaRow+(i*4+j*2+k+1));
                    strCol.Add(sAcol);  dValue.Add(ElemK);

                    sBcol.Format(_T("%s%d"),sNode, iStaRow+(i*4+j*2+k+1));
                    strCol.Add(sBcol);  dValue.Add(dNode[j]);

                    sCcol.Format(_T("%s%d"),sYMax, iStaRow+(i*4+j*2+k+1));
                    strCol.Add(sCcol);  dValue.Add(dForcMax[j*2+k]);

                    sDcol.Format(_T("%s%d"),sYMin, iStaRow+(i*4+j*2+k+1));
                    strCol.Add(sDcol);  dValue.Add(dForcMin[j*2+k]);      
                }             
            }
        }

        CString sScope=_T("");
        sScope.Format(_T("%s%d:%s%d"),sNode, iStaRow+1 , sYMin, (iStaRow+TotalElem*4));
        ChartData chForce;
        chForce.SourceData = sScope;
        chForce.bLegend = TRUE;
        chForce.ChartTitle = sChartTitle;
        chForce.ChartType = xlXYScatterLinesNoMarkers; //Chart Type : 꺾은선 xlLine, xlLineMarkers, 분산형 xlXYScatter, xlXYScatterSmooth, xlXYScatterSmoothNoMarkers, xlXYScatterLines, xlXYScatterLinesNoMarkers
        chForce.PlotBy = xlColumns;
        chForce.xTitle = _T("Node");
        chForce.yTitle = sYaxis;
        chForce.dXmax = dMaxXValue;
        chForce.dXmin = dMinXValue;
        chForce.nXunit = 2;
        chForce.aSeries.RemoveAll();
        switch(l)   //0:BMD, 1:SFD, 2:TMD 
        {
        case 0:
            chForce.aSeries.Add(_T("Max Moment"));
            chForce.aSeries.Add(_T("Min Moment"));
            break;
        case 1:
            chForce.aSeries.Add(_T("Max Shear"));
            chForce.aSeries.Add(_T("Min Shear"));
            break;
        case 2:
            chForce.aSeries.Add(_T("Max Torsion"));
            chForce.aSeries.Add(_T("Min Torsion"));
            break;
        }	       
        arForcDiag.Add(chForce);

        Prt_Graph_Data GraphData;
        GraphData.Initialize();
        GraphData.arStrTitle.Copy(strTitle);
        GraphData.arStrCol.Copy(strCol);
        GraphData.arValue.Copy(dValue);
        GraphData.arChartData.Copy(arForcDiag);
        arGraphData.Add(GraphData);
    }
    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_ChartForceStage(CArray<Prt_Graph_Data,Prt_Graph_Data&>& arGraphData)
{
    arGraphData.RemoveAll();
    BOOL bForceStage=TRUE;
    int iTopBot = 2;  

    CArray<CString,CString> strTitle;
    CArray<CString,CString> strCol;
    CArray<double,double> dValue;
    CArray<ChartData,ChartData> StageData;

    CArray<T_STAG_K,T_STAG_K> arStagK;
    if(!Get_StageKeyList(arStagK)) return FALSE;  
    T_STAG_K TotalStagK = arStagK.GetSize();
    arGraphData.SetSize(TotalStagK);

    int i,j,k,l,m;
    int iCount=0;
    int iCurrSheet=1;
    for(m=0; m<TotalStagK; m++)
    {
        // Initialize.
        strTitle.RemoveAll();
        strCol.RemoveAll();
        dValue.RemoveAll();
        StageData.RemoveAll();
        T_STAG_K StagK = arStagK.GetAt(m);    

        double dExtraRat;
        CString strStage;
        CArray<_PRT_FORC_GRPH_ELEM,_PRT_FORC_GRPH_ELEM> arGrphElem;
        if(!Get_ForcGrphStag(StagK, strStage, dExtraRat, arGrphElem)) continue;    
        if(arGrphElem.GetSize()==0) continue;
        int TotalElem = arGrphElem.GetSize();    
        int iOverLine = iCount*(TotalElem*4+1) + 2*(iCount+1) + (TotalElem*4)+2;
        if(iOverLine>30000)
        {
            iCount=0;
            iCurrSheet++;
        }
        int iStaRow = iCount*(TotalElem*4+1) + 2*(iCount+1);
        double dMinXValue = 0.0; 
        double dMaxXValue = 0.0; 

        for(i=0; i<iTopBot; i++)
        {
            CString sElem, sNode, sYMax, sYAlwC, sYAlwT;
            CString sChartTitle;

            switch(i)   //0:Top, 1:Bot
            {
            case 0:
                sElem=_T("M"); sNode=_T("N"); sYMax=_T("O"); sYAlwC=_T("P"); sYAlwT=_T("Q");
                sChartTitle=_T("상연  ")+strStage;
                break;
            case 1:
                sElem=_T("S"); sNode=_T("T"); sYMax=_T("U"); sYAlwC=_T("V"); sYAlwT=_T("W");
                sChartTitle=_T("하연  ")+strStage;
                break;      
            }

            CString strTemp;
            strTemp.Format(_T("%s%d"),sElem,iStaRow-1);  strTitle.Add(strTemp);
            strTemp = sChartTitle;   strTitle.Add(strTemp);
            strTemp.Format(_T("%s%d"),sElem,iStaRow);  strTitle.Add(strTemp);      
            strTemp = _T("Element");   strTitle.Add(strTemp);
            strTemp.Format(_T("%s%d"),sNode,iStaRow);  strTitle.Add(strTemp);      
            strTemp = _T("I/J");   strTitle.Add(strTemp);
            strTemp.Format(_T("%s%d"),sYMax,iStaRow);  strTitle.Add(strTemp);
            strTemp = _T("Stress");   strTitle.Add(strTemp);
            strTemp.Format(_T("%s%d"),sYAlwC,iStaRow);  strTitle.Add(strTemp);
            strTemp = _T("Alw Comp");   strTitle.Add(strTemp);
            strTemp.Format(_T("%s%d"),sYAlwT,iStaRow);  strTitle.Add(strTemp);
            strTemp = _T("Alw Tens");   strTitle.Add(strTemp);        

            double dNode[2];
            for(j=0; j<TotalElem; j++)
            {
                _PRT_FORC_GRPH_ELEM ElemStres = arGrphElem.GetAt(j);
                T_ELEM_K ElemK = ElemStres.ElemK;
                T_ELEM_D ElemD;
                if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;
                int nNode= m_pDoc->m_pAttrCtrl->GetNumNode(ElemD.eltyp, ElemD.elnod);
                for(k=0; k<nNode; k++)
                {
                    dNode[k] = ElemD.elnod[k];          
                }

                if(j==0)            dMinXValue = dNode[0] - 1;
                if(j==TotalElem-1)  dMaxXValue = dNode[1] + 1;

                double dStressMax[4];
                dStressMax[0] = dStressMax[3] = 0.0;  //dummy    

                switch(i)   //0:Top, 1:Bot
                {
                case 0:
                    dStressMax[1] = max(ElemStres.dStl[0],ElemStres.dStr[0]);  //1:i 
                    dStressMax[2] = max(ElemStres.dStl[1],ElemStres.dStr[1]);  //2:j      
                    break;
                case 1:
                    dStressMax[1] = max(ElemStres.dSbl[0], ElemStres.dSbr[0]);  //1:i 
                    dStressMax[2] = max(ElemStres.dSbl[1], ElemStres.dSbr[1]);  //2:j      
                    break;      
                }

                CString sAcol, sBcol, sCcol, sDcol, sEcol;
                for(k=0; k<2; k++)
                {
                    for(l=0; l<2; l++)
                    { 
                        sAcol.Format(_T("%s%d"),sElem,iStaRow+(j*4+k*2+l+1));
                        strCol.Add(sAcol);   dValue.Add(ElemK);

                        sBcol.Format(_T("%s%d"),sNode,iStaRow+(j*4+k*2+l+1));
                        strCol.Add(sBcol);   dValue.Add(dNode[k]);

                        sCcol.Format(_T("%s%d"),sYMax, iStaRow+(j*4+k*2+l+1));
                        strCol.Add(sCcol);   dValue.Add(dStressMax[k*2+l]);

                        sDcol.Format(_T("%s%d"),sYAlwC, iStaRow+(j*4+k*2+l+1));
                        strCol.Add(sDcol);   dValue.Add(ElemStres.dAlc[k]);

                        sEcol.Format(_T("%s%d"),sYAlwT, iStaRow+(j*4+k*2+l+1));
                        strCol.Add(sEcol);   dValue.Add(ElemStres.dAlt[k][i]);          
                    }
                }             
            }

            CString sScope=_T("");
            sScope.Format(_T("%s%d:%s%d"),sNode, iStaRow+1, sYAlwT, (iStaRow + TotalElem*4));
            ChartData ChData;
            ChData.SourceData = sScope;
            ChData.bLegend = TRUE;
            ChData.ChartTitle = sChartTitle;
            ChData.ChartType = xlXYScatterLinesNoMarkers; //Chart Type : 꺾은선 xlLine, xlLineMarkers, 분산형 xlXYScatter, xlXYScatterSmooth, xlXYScatterSmoothNoMarkers, xlXYScatterLines, xlXYScatterLinesNoMarkers
            ChData.PlotBy = xlColumns;
            ChData.xTitle = _T("Node");
            ChData.yTitle.Format(_T("Stress(%s)"),m_strStresUnit);
            ChData.dXmax = dMaxXValue;
            ChData.dXmin = dMinXValue;
            ChData.nXunit = 2;
            ChData.aSeries.RemoveAll();
            ChData.aSeries.Add(_T("Stress"));
            ChData.aSeries.Add(_T("Alw Comp"));
            ChData.aSeries.Add(_T("Alw Tens"));

            //ChData.dYmax = dMaxYValue;
            //double dXmax;//X축 최대값
            //double dYmax;//Y축 최대값
            //long   nXunit;//X축 주눈금
            //long   nYunit;//Y축 주눈금

            StageData.Add(ChData);
        }
        iCount++;

        Prt_Graph_Data GraphData;
        GraphData.Initialize();
        GraphData.iCurrSheet = iCurrSheet;
        GraphData.arStrTitle.Copy(strTitle);
        GraphData.arStrCol.Copy(strCol);
        GraphData.arValue.Copy(dValue);
        GraphData.arChartData.Copy(StageData);
        arGraphData.SetAt(m, GraphData);

    }

    if(dValue.GetSize()==0) return FALSE;

    return TRUE;
}

//사용하중시 하중조합별 응력도..
BOOL CPrtKSCE_USD03::Get_ChartForceLcom(CArray<Prt_Graph_Data,Prt_Graph_Data&>& arGraphData)
{ 
    arGraphData.RemoveAll();
    int iTopBot = 2;  

    CArray<CString,CString> strTitle;
    CArray<CString,CString> strCol;
    CArray<double,double> dValue;
    CArray<ChartData,ChartData> LcomData;

    int i,j,k,l,m;    
    CArray<T_LCOM_K,T_LCOM_K> arLcomK;
    if(!Get_LcomKeyList(arLcomK)) return FALSE;
    T_LCOM_K TotalLcomK = arLcomK.GetSize();

    int iCount = 0;
    int iCurrSheet=1;
    for(m=0; m<TotalLcomK; m++)
    {        
        // Initialize.
        strTitle.RemoveAll();
        strCol.RemoveAll();
        dValue.RemoveAll();
        LcomData.RemoveAll();

        T_LCOM_K LcomK = arLcomK.GetAt(m); 

        CString strLcomDescrip;
        CString strLcomName;
        double dExtraRat;

        CArray<_PRT_FORC_GRPH_ELEM,_PRT_FORC_GRPH_ELEM> arGrphElemMax;
        CArray<_PRT_FORC_GRPH_ELEM,_PRT_FORC_GRPH_ELEM> arGrphElemMin;
        if(!Get_ForcGrphLcom(LcomK, strLcomName, strLcomDescrip, dExtraRat, arGrphElemMax, arGrphElemMin)) continue;    
        if(arGrphElemMax.GetSize()==0) continue;
        int TotalElem = arGrphElemMax.GetSize();
        int iOverLine = iCount*(TotalElem*4+1) + 2*(iCount+1) + (TotalElem*4)+2;
        if(iOverLine>30000)
        {
            iCount=0;
            iCurrSheet++;
        }
        int iStaRow = iCount*(TotalElem*4+1) + 2*(iCount+1);
        double dMinXValue = 0.0; 
        double dMaxXValue = 0.0; 

        BOOL bElemMin = TRUE;
        if(arGrphElemMin.GetSize()==0) bElemMin = FALSE; 

        CString strExtraRat;
        strExtraRat.Format(_T(" (할증 계수 : %.2f)"),dExtraRat);
        CString sText=_T("");

        for(i=0; i<iTopBot; i++)
        {
            CString sElem, sNode, sYMax, sYMin, sYAlwC, sYAlwT;
            CString sChartTitle1, sChartTitle2;
            CString strTemp;

            switch(i)   //0:Top, 1:Bot
            {
            case 0:
                sChartTitle1= _T("상연  ") + strLcomName + _T(" : ") + strLcomDescrip + strExtraRat;
                sChartTitle2= _T("상연  ") + strLcomName;
                if(bElemMin)
                {
                    sElem=_T("M"); sNode=_T("N"); sYMax=_T("O"); sYMin=_T("P"); sYAlwC=_T("Q"); sYAlwT=_T("R");             
                    strTemp.Format(_T("%s%d"),sElem,iStaRow-1);  strTitle.Add(strTemp);
                    strTemp = sChartTitle2;   strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sElem,iStaRow);  strTitle.Add(strTemp);      
                    strTemp = _T("Element");   strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sNode,iStaRow);  strTitle.Add(strTemp);      
                    strTemp = _T("I/J");      strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sYMax,iStaRow);  strTitle.Add(strTemp);
                    strTemp = _T("Max Stress");   strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sYMin,iStaRow);  strTitle.Add(strTemp);
                    strTemp = _T("Min Stress");   strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sYAlwC,iStaRow);  strTitle.Add(strTemp);
                    strTemp = _T("Alw Comp");   strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sYAlwT,iStaRow);  strTitle.Add(strTemp);
                    strTemp = _T("Alw Tens");   strTitle.Add(strTemp);             
                }
                else  
                {
                    sElem=_T("M"); sNode=_T("N"); sYMax=_T("O"); sYAlwC=_T("P"); sYAlwT=_T("Q");
                    strTemp.Format(_T("%s%d"),sElem,iStaRow-1);  strTitle.Add(strTemp);
                    strTemp = sChartTitle2;   strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sElem,iStaRow);  strTitle.Add(strTemp);      
                    strTemp = _T("Element");   strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sNode,iStaRow);  strTitle.Add(strTemp);      
                    strTemp = _T("I/J");   strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sYMax,iStaRow);  strTitle.Add(strTemp);
                    strTemp = _T("Max Stress");   strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sYAlwC,iStaRow);  strTitle.Add(strTemp);
                    strTemp = _T("Alw Comp");   strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sYAlwT,iStaRow);  strTitle.Add(strTemp);
                    strTemp = _T("Alw Tens");   strTitle.Add(strTemp);              
                }          
                break;
            case 1:
                sChartTitle1= _T("하연  ") + strLcomName + _T(" : ") + strLcomDescrip + strExtraRat;
                sChartTitle2= _T("하연  ") + strLcomName;
                if(bElemMin)
                {
                    sElem=_T("T"); sNode=_T("U"); sYMax=_T("V"); sYMin=_T("W"); sYAlwC=_T("X"); sYAlwT=_T("Y"); 
                    strTemp.Format(_T("%s%d"),sElem,iStaRow-1);  strTitle.Add(strTemp);
                    strTemp = sChartTitle2;   strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sElem,iStaRow);  strTitle.Add(strTemp);      
                    strTemp = _T("Element");   strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sNode,iStaRow);  strTitle.Add(strTemp);      
                    strTemp = _T("I/J");   strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sYMax,iStaRow);  strTitle.Add(strTemp);
                    strTemp = _T("Max Stress");   strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sYMin,iStaRow);  strTitle.Add(strTemp);
                    strTemp = _T("Min Stress");   strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sYAlwC,iStaRow);  strTitle.Add(strTemp);
                    strTemp = _T("Alw Comp");   strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sYAlwT,iStaRow);  strTitle.Add(strTemp);
                    strTemp = _T("Alw Tens");   strTitle.Add(strTemp);             
                }
                else
                {
                    sElem=_T("T"); sNode=_T("U"); sYMax=_T("V"); sYAlwC=_T("W"); sYAlwT=_T("X"); 
                    strTemp.Format(_T("%s%d"),sElem,iStaRow-1);  strTitle.Add(strTemp);
                    strTemp = sChartTitle2;   strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sElem,iStaRow);  strTitle.Add(strTemp);      
                    strTemp = _T("Element");   strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sNode,iStaRow);  strTitle.Add(strTemp);      
                    strTemp = _T("I/J");   strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sYMax,iStaRow);  strTitle.Add(strTemp);
                    strTemp = _T("Max Stress");   strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sYAlwC,iStaRow);  strTitle.Add(strTemp);
                    strTemp = _T("Alw Comp");   strTitle.Add(strTemp);
                    strTemp.Format(_T("%s%d"),sYAlwT,iStaRow);  strTitle.Add(strTemp);
                    strTemp = _T("Alw Tens");   strTitle.Add(strTemp);      
                }          
                break;      
            }      

            double dNode[2];
            for(j=0; j<TotalElem; j++)
            {
                _PRT_FORC_GRPH_ELEM ElemMax = arGrphElemMax.GetAt(j);
                _PRT_FORC_GRPH_ELEM ElemMin;
                if(bElemMin)  ElemMin = arGrphElemMin.GetAt(j);

                T_ELEM_K ElemK = ElemMax.ElemK;
                T_ELEM_D ElemD;
                if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;
                int nNode= m_pDoc->m_pAttrCtrl->GetNumNode(ElemD.eltyp, ElemD.elnod);
                for(k=0; k<nNode; k++)
                {
                    dNode[k] = ElemD.elnod[k];          
                }

                if(j==0)            dMinXValue = dNode[0] - 1;
                if(j==TotalElem-1)  dMaxXValue = dNode[1] + 1;

                double dStressMax[4];
                double dStressMin[4];

                dStressMax[0] = dStressMax[3] = 0.0;  //dummy    
                dStressMin[0] = dStressMin[3] = 0.0;

                switch(i)   //0:Top, 1:Bot
                {
                case 0:
                    dStressMax[1] = max(ElemMax.dStl[0], ElemMax.dStr[0]);  //1:i 
                    dStressMax[2] = max(ElemMax.dStl[1], ElemMax.dStr[1]);  //2:j      

                    dStressMin[1] = (bElemMin ? min(ElemMin.dStl[0],ElemMin.dStr[0]) : 0.0);  //1:i 
                    dStressMin[2] = (bElemMin ? min(ElemMin.dStl[1],ElemMin.dStr[1]) : 0.0);  //2:j      
                    break;
                case 1:
                    dStressMax[1] = max(ElemMax.dSbl[0],ElemMax.dSbr[0]);  //1:i 
                    dStressMax[2] = max(ElemMax.dSbl[1],ElemMax.dSbr[1]);  //2:j      

                    dStressMin[1] = (bElemMin ? min(ElemMin.dSbl[0],ElemMin.dSbr[0]) : 0.0);  //1:i 
                    dStressMin[2] = (bElemMin ? min(ElemMin.dSbl[1],ElemMin.dSbr[1]) : 0.0);  //2:j      
                    break;      
                }

                CString sAcol, sBcol, sCcol, sDcol, sEcol, sFcol;
                if(bElemMin)
                {          
                    for(k=0; k<2; k++)
                    {
                        for(l=0; l<2; l++)
                        {
                            sAcol.Format(_T("%s%d"),sElem,iStaRow+(j*4+k*2+l+1) );
                            strCol.Add(sAcol);  dValue.Add(ElemK);

                            sBcol.Format(_T("%s%d"),sNode,iStaRow+(j*4+k*2+l+1) );
                            strCol.Add(sBcol);  dValue.Add(dNode[k]);

                            sCcol.Format(_T("%s%d"),sYMax,iStaRow+(j*4+k*2+l+1) );
                            strCol.Add(sCcol);  dValue.Add(dStressMax[k*2+l]);

                            sDcol.Format(_T("%s%d"),sYMin,iStaRow+(j*4+k*2+l+1) );
                            strCol.Add(sDcol);  dValue.Add(dStressMin[k*2+l]);

                            sEcol.Format(_T("%s%d"),sYAlwC,iStaRow+(j*4+k*2+l+1) );
                            strCol.Add(sEcol);  dValue.Add(ElemMax.dAlc[k]);

                            sFcol.Format(_T("%s%d"),sYAlwT,iStaRow+(j*4+k*2+l+1) );
                            strCol.Add(sFcol);  dValue.Add(ElemMax.dAlt[k][i]);            
                        }
                    }            
                }
                else
                {
                    for(k=0; k<2; k++)
                    {
                        for(l=0; l<2; l++)
                        { 
                            sAcol.Format(_T("%s%d"),sElem,iStaRow+(j*4+k*2+l+1) );
                            strCol.Add(sAcol);  dValue.Add(ElemK);

                            sBcol.Format(_T("%s%d"),sNode,iStaRow+(j*4+k*2+l+1) );
                            strCol.Add(sBcol);  dValue.Add(dNode[k]);

                            sCcol.Format(_T("%s%d"),sYMax, iStaRow+(j*4+k*2+l+1));
                            strCol.Add(sCcol);  dValue.Add(dStressMax[k*2+l]);

                            sDcol.Format(_T("%s%d"),sYAlwC, iStaRow+(j*4+k*2+l+1));
                            strCol.Add(sDcol);  dValue.Add(ElemMax.dAlc[k]);

                            sEcol.Format(_T("%s%d"),sYAlwT,iStaRow+(j*4+k*2+l+1));
                            strCol.Add(sEcol);  dValue.Add(ElemMax.dAlt[k][i]);             
                        }
                    }     
                }        
            }
            CString sScope=_T("");
            sScope.Format(_T("%s%d:%s%d"),sNode, iStaRow+1,  sYAlwT, (iStaRow +TotalElem*4));
            ChartData ChData;
            ChData.SourceData = sScope;
            ChData.bLegend = TRUE;
            ChData.ChartTitle = sChartTitle1;
            ChData.ChartType = xlXYScatterLinesNoMarkers; //Chart Type : 꺾은선 xlLine, xlLineMarkers, 분산형 xlXYScatter, xlXYScatterSmooth, xlXYScatterSmoothNoMarkers, xlXYScatterLines, xlXYScatterLinesNoMarkers
            ChData.PlotBy = xlColumns;
            ChData.xTitle = _T("Node");
            ChData.yTitle.Format(_T("Stress(%s)"),m_strStresUnit);
            ChData.nXunit = 2;
            ChData.dXmax = dMaxXValue;
            ChData.dXmin = dMinXValue;
            if(bElemMin)
            {
                ChData.aSeries.Add(_T("Max Str"));
                ChData.aSeries.Add(_T("Min Str"));
                ChData.aSeries.Add(_T("Alw Comp"));
                ChData.aSeries.Add(_T("Alw Tens"));
            }
            else
            {
                ChData.aSeries.Add(_T("Stress"));
                ChData.aSeries.Add(_T("Alw Comp"));
                ChData.aSeries.Add(_T("Alw Tens"));
            }	      

            LcomData.Add(ChData);
        }
        iCount++;

        Prt_Graph_Data GraphData;
        GraphData.Initialize();
        GraphData.iCurrSheet = iCurrSheet;
        GraphData.arStrTitle.Copy(strTitle);
        GraphData.arStrCol.Copy(strCol);
        GraphData.arValue.Copy(dValue);
        GraphData.arChartData.Copy(LcomData);
        arGraphData.Add(GraphData);
    }

    if(arGraphData.GetSize()==0) return FALSE;

    return TRUE;

}

BOOL CPrtKSCE_USD03::Get_ForceDetlXLData(CString strTitle, CArray<Prt_ArString,Prt_ArString&>& arStrChk)
{
    arStrChk.RemoveAll();    
    int i,j;
    CString sStrChk;
    Prt_ArString PrtArStr;
    CArray<CString,CString> arString;
    if(m_arElemK.GetSize()==0) return FALSE;

    int iTotalElemK = m_arElemK.GetSize(); 
    // SetSize
    arStrChk.SetSize(iTotalElemK);
    for(int k=0; k<m_arElemK.GetSize(); k++)
    {
        auto ElemK = m_arElemK.GetAt(k);
        PrtArStr.Initialize(); 
        arString.RemoveAll();

        // 단면속성.
        _PRT_FORC_DETL_SECT  DetlSect;
        if(!Get_ForcDetlSect(ElemK, TRUE, DetlSect)) continue; // TRUE : 단위 m로 표시 
        // 최종 시공단계 부재력.
        _PRT_FORC_DETL_STAG  DetlStag;
        if(!Get_ForcDetlStag(ElemK, DetlStag)) continue;
        // 사용하중조건종류별 부재력 (시공단계 부재력 제외).
        _PRT_FORC_DETL_LCAS  DetlLcas;
        if(!Get_ForcDetlLcas(ElemK, DetlLcas)) continue;
        // 사용하중조합종류별 부재력.
        _PRT_FORC_DETL_LCOM LcomPrev, LcomNext;
        if(!Get_ForcDetlLcom(DetlStag, DetlLcas, LcomPrev, LcomNext)) continue;

        for(i=0; i<2; i++)  //0:I, 1:J
        {
            CString strIJ = (i==0 ? _T("I") : _T("J"));
            CString sSectTitle[3];
            sSectTitle[0] = _T("~ Gross");
            sSectTitle[1] = _T("~ Net");
            sSectTitle[2] = _T("~ CPR");

            sStrChk = _T("-----------------------------------------------------------------------------------------------------------------------"); arString.Add(sStrChk);
            sStrChk.Format(_T("Stress Analysis Element:  %d  Position:  %s"),ElemK, strIJ);               arString.Add(sStrChk);
            sStrChk = _T("***********************************************************************************************************************"); arString.Add(sStrChk);
            sStrChk = _T("Cross Section Area(m²) Iy(m⁴) Iz(m⁴) Zyt(m³) Zyb(m³)");                    arString.Add(sStrChk);
            for(j=0; j<3; j++)
            {
                _PRT_SECT_BASE SectBase = DetlSect.SectBase[j];
                double dArea, dIy, dIz, dZyt, dZyb;
                CString sArea, sIy, sIz, sZyt, sZyb;

                dArea = SectBase.dArea[i];  sArea.Format(_T("%.1f"), dArea); //Check_ValueForm(6, 4, dArea);
                dIy   = SectBase.dIy[i];    sIy.Format(_T("%.1f"), dIy);     //Check_ValueForm(6, 4, dIy);
                dIz   = SectBase.dIz[i];    sIz.Format(_T("%.1f"), dIz);     //Check_ValueForm(6, 4, dIz);
                dZyt  = SectBase.dZyt[i];   sZyt.Format(_T("%.1f"), dZyt);   //Check_ValueForm(6, 4, dZyt);
                dZyb  = SectBase.dZyb[i];   sZyb.Format(_T("%.1f"), dZyb);   //Check_ValueForm(6, 4, dZyb);

                sStrChk.Format(_T("%s %s %s %s %s %s "), sSectTitle[j], sArea, sIy, sIz, sZyt, sZyb);  arString.Add(sStrChk);
            }
            sStrChk = _T("");           arString.Add(sStrChk);

            // XLData 최종 시공단계 부재력.
            sStrChk = _T("~ ~ Fxx(tonf) Muy(tonf·m) Muz(tonf·m) Stop(kgf/cm²) Sbot(kgf/cm²)");    arString.Add(sStrChk);
            sStrChk = _T("-----------------------------------------------------------------------------------------------------------------------"); arString.Add(sStrChk);
            CArray<_PRT_FORC_BASE,_PRT_FORC_BASE> ForcStage;
            ForcStage.Add(DetlStag.ForcDL);
            ForcStage.Add(DetlStag.ForcEL);
            ForcStage.Add(DetlStag.ForcTP);
            ForcStage.Add(DetlStag.ForcTS);
            ForcStage.Add(DetlStag.ForcCSP);
            ForcStage.Add(DetlStag.ForcCSS);
            ForcStage.Add(DetlStag.ForcSum);

            CString sStageTitle[7];
            sStageTitle[0] = _T("PREV DL");
            sStageTitle[1] = _T("~ EL");
            sStageTitle[2] = _T("~ TP");
            sStageTitle[3] = _T("~ TS");
            sStageTitle[4] = _T("~ CSP");
            sStageTitle[5] = _T("~ CSS");
            sStageTitle[6] = _T("~ Sum");

            for(j=0; j<ForcStage.GetSize(); j++)
            {
                double dFxx, dMuy, dMuz, dStop, dSbot;
                CString sFxx, sMuy, sMuz, sStop, sSbot;
                _PRT_FORC_BASE Force = ForcStage.GetAt(j);
                dFxx  = Force.dFxx[i];   sFxx = Check_ValueForm(6, 2, dFxx);
                dMuy  = Force.dMuy[i];   sMuy = Check_ValueForm(6, 2, dMuy);
                dMuz  = Force.dMuz[i];   sMuz = Check_ValueForm(6, 2, dMuz);
                dStop = Force.dStl[i];   sStop = Check_ValueForm(6, 2, dStop);
                dSbot = Force.dSbl[i];   sSbot = Check_ValueForm(6, 2, dSbot);

                sStrChk.Format(_T("%s %s %s %s %s %s "),sStageTitle[j], sFxx, sMuy, sMuz, sStop, sSbot);  arString.Add(sStrChk);
            }
            sStrChk = _T("");           arString.Add(sStrChk);

            // 사용하중조건종류별 부재력 (시공단계 부재력 제외).
            CArray<_PRT_FORC_BASE,_PRT_FORC_BASE> ForcLcas;
            ForcLcas.Add(DetlLcas.ForcDL);
            ForcLcas.Add(DetlLcas.ForcLLmax);
            ForcLcas.Add(DetlLcas.ForcLLmin);
            ForcLcas.Add(DetlLcas.ForcADDmax);
            ForcLcas.Add(DetlLcas.ForcADDmin);

            CString sLcasTitle[5];
            sLcasTitle[0] = _T("SING ForcDL");
            sLcasTitle[1] = _T("~ LLmax");
            sLcasTitle[2] = _T("~ LLmin");
            sLcasTitle[3] = _T("~ ADDmax");
            sLcasTitle[4] = _T("~ ADDmin");

            for(j=0; j<ForcLcas.GetSize(); j++)
            {
                double dFxx, dMuy, dMuz, dStop, dSbot;
                CString sFxx, sMuy, sMuz, sStop, sSbot;
                _PRT_FORC_BASE Force = ForcLcas.GetAt(j);
                dFxx  = Force.dFxx[i];   sFxx  = Check_ValueForm(6, 2, dFxx);
                dMuy  = Force.dMuy[i];   sMuy  = Check_ValueForm(6, 2, dMuy);
                dMuz  = Force.dMuz[i];   sMuz  = Check_ValueForm(6, 2, dMuz);
                dStop = Force.dStl[i];   sStop = Check_ValueForm(6, 2, dStop);
                dSbot = Force.dSbl[i];   sSbot = Check_ValueForm(6, 2, dSbot);

                sStrChk.Format(_T("%s %s %s %s %s %s "),sLcasTitle[j], sFxx, sMuy, sMuz, sStop, sSbot);  arString.Add(sStrChk);
            }

            //XLData LcomPrev 사용하중조합종류별 부재력 .
            sStrChk = _T("Before-----------------------------------------------------------------------------------------------------------------");  arString.Add(sStrChk);
            CArray<_PRT_FORC_BASE,_PRT_FORC_BASE> ForcLPrev;
            ForcLPrev.Add(LcomPrev.ForcD);
            ForcLPrev.Add(LcomPrev.ForcDLmax);
            ForcLPrev.Add(LcomPrev.ForcDLmin);
            ForcLPrev.Add(LcomPrev.ForcDLAmax);
            ForcLPrev.Add(LcomPrev.ForcDLAmin);

            CString sPrevTitle[5];
            sPrevTitle[0] = _T("~ PS+DL+C+S");
            sPrevTitle[1] = _T("~ PS+DL+C+S+LLmax");
            sPrevTitle[2] = _T("~ PS+DL+C+S+LLmin");
            sPrevTitle[3] = _T("~ PS+DL+C+S+(LL+ST)max+W+T");
            sPrevTitle[4] = _T("~ PS+DL+C+S+(LL+ST)min+W+T");

            for(j=0; j<ForcLPrev.GetSize(); j++)
            {
                double dFxx, dMuy, dMuz, dStop, dSbot;
                CString sFxx, sMuy, sMuz, sStop, sSbot;
                _PRT_FORC_BASE Force = ForcLPrev.GetAt(j);
                dFxx  = Force.dFxx[i];  sFxx  = Check_ValueForm(6, 2, dFxx);
                dMuy  = Force.dMuy[i];  sMuy  = Check_ValueForm(6, 2, dMuy);
                dMuz  = Force.dMuz[i];  sMuz  = Check_ValueForm(6, 2, dMuz);
                dStop = Force.dStl[i];  sStop = Check_ValueForm(6, 2, dStop);
                dSbot = Force.dSbl[i];  sSbot = Check_ValueForm(6, 2, dSbot);

                sStrChk.Format(_T("%s %s %s %s %s %s "),sPrevTitle[j], sFxx, sMuy, sMuz, sStop, sSbot);  arString.Add(sStrChk);
            }

            //XLData LcomNext 사용하중조합종류별 부재력 .
            sStrChk = _T("After------------------------------------------------------------------------------------------------------------------");  arString.Add(sStrChk);
            CArray<_PRT_FORC_BASE,_PRT_FORC_BASE> ForcLNext;
            ForcLNext.Add(LcomNext.ForcD);
            ForcLNext.Add(LcomNext.ForcDLmax);
            ForcLNext.Add(LcomNext.ForcDLmin);
            ForcLNext.Add(LcomNext.ForcDLAmax);
            ForcLNext.Add(LcomNext.ForcDLAmin);

            CString sNextTitle[5];
            sNextTitle[0] = _T("~ PS+DL+C+S");
            sNextTitle[1] = _T("~ PS+DL+C+S+LLmax");
            sNextTitle[2] = _T("~ PS+DL+C+S+LLmin");
            sNextTitle[3] = _T("~ PS+DL+C+S+(LL+ST)max+W+T");
            sNextTitle[4] = _T("~ PS+DL+C+S+(LL+ST)min+W+T");

            for(j=0; j<ForcLNext.GetSize(); j++)
            {
                double dFxx, dMuy, dMuz, dStop, dSbot;
                CString sFxx, sMuy, sMuz, sStop, sSbot;
                _PRT_FORC_BASE Force = ForcLNext.GetAt(j);
                dFxx =  Force.dFxx[i];  sFxx  = Check_ValueForm(6, 2, dFxx);
                dMuy =  Force.dMuy[i];  sMuy  = Check_ValueForm(6, 2, dMuy);
                dMuz =  Force.dMuz[i];  sMuz  = Check_ValueForm(6, 2, dMuz);
                dStop = Force.dStl[i];  sStop = Check_ValueForm(6, 2, dStop);
                dSbot = Force.dSbl[i];  sSbot = Check_ValueForm(6, 2, dSbot);

                sStrChk.Format(_T("%s %s %s %s %s %s "),sNextTitle[j], sFxx, sMuy, sMuz, sStop, sSbot);      arString.Add(sStrChk);
            }

            sStrChk = _T("***********************************************************************************************************************"); arString.Add(sStrChk);
            sStrChk = _T(""); arString.Add(sStrChk);      
        }

        PrtArStr.arString.Copy(arString);
        arStrChk.SetAt(k, PrtArStr);
    }

    if(arStrChk.GetSize()==0) return FALSE;

    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_ForceTotalXLData(CArray<CString,CString>& arStrTotal) 
{  
    CString strTotal;
    strTotal = _T("<Table> <응력 검토 집계 결과>                                                     (단위 : kgf/cm²) ");     arStrTotal.Add(strTotal);
    strTotal = _T("<MV><MH>구``분 ~ ~</MH> <MH>상연 ~  ~</MH>  <MH>하연 ~  ~</MH>");  arStrTotal.Add(strTotal);
    strTotal = _T("</MV><MH>구``분 ~ ~</MH> <MH>발생응력</MH> <MH>허용치</MH> <MH>검토결과</MH>  <MH>발생응력</MH> <MH>허용치</MH> <MH>검토결과</MH>"); arStrTotal.Add(strTotal);
    CArray<_PRT_FORC_GRPH_TOTL,_PRT_FORC_GRPH_TOTL> arGrphTotl;
    if(!Get_ForcGrphTotl(arGrphTotl)) return FALSE;
    if(arGrphTotl.GetSize()==0) return FALSE;

    for(int i=0; i<arGrphTotl.GetSize(); i++)
    {
        CString strTitle;
        _PRT_FORC_GRPH_TOTL GrphTotl = arGrphTotl.GetAt(i);
        strTitle = GrphTotl.strTitle;        

        CString sStc, sAltc;	// Top Comp.
        CString sStt, sAltt;	// Top Tens.
        CString sSbc, sAlbc;	// Bot Comp.
        CString sSbt, sAlbt;	// Bot Tens.
        sStt  = Check_ValueForm(6, 2, GrphTotl.dStt);  sAltt = Check_ValueForm(6, 2, GrphTotl.dAltt);
        CString sSttChk = (GrphTotl.dStt>GrphTotl.dAltt ? _T("O.K") : _T("N.G"));
        sStc  = Check_ValueForm(6, 2, GrphTotl.dStc);  sAltc = Check_ValueForm(6, 2, GrphTotl.dAltc);
        CString sStcChk = (GrphTotl.dStc<GrphTotl.dAltc ? _T("O.K") : _T("N.G"));
        sSbt  = Check_ValueForm(6, 2, GrphTotl.dSbt);  sAlbt = Check_ValueForm(6, 2, GrphTotl.dAlbt);
        CString sSbtChk = (GrphTotl.dSbt>GrphTotl.dAlbt ? _T("O.K") : _T("N.G"));
        sSbc  = Check_ValueForm(6, 2, GrphTotl.dSbc);  sAlbc = Check_ValueForm(6, 2, GrphTotl.dAlbc);
        CString sSbcChk = (GrphTotl.dSbc<GrphTotl.dAlbc ? _T("O.K") : _T("N.G"));

        strTotal.Format(_T("<MV><MH>%s ~</MH> <MH>인장</MH>  <MH>%s</MH> <MH>%s</MH> <MH>%s</MH>  <MH>%s</MH> <MH>%s</MH> <MH>%s</MH>"),
            strTitle, sStt, sAltt, sSttChk, sSbt, sAlbt, sSbtChk);   arStrTotal.Add(strTotal);
        strTotal.Format(_T("</MV><MH>%s ~</MH> <MH>압축</MH>  <MH>%s</MH> <MH>%s</MH> <MH>%s</MH>  <MH>%s</MH> <MH>%s</MH> <MH>%s</MH>"),
            strTitle, sStc, sAltc, sStcChk, sSbc, sAlbc, sSbcChk);   arStrTotal.Add(strTotal);
    }
    strTotal = _T("</Table>");    arStrTotal.Add(strTotal);
    strTotal = _T("");

    if(arStrTotal.GetSize()==0) return FALSE;

    return TRUE;
}

//하중조합별 부재력 집계 
BOOL CPrtKSCE_USD03::Get_UltiLoadXLData(CArray<Prt_ArString,Prt_ArString&>& arStrUltiData)
{
    arStrUltiData.RemoveAll();
    CArray<_PRT_FORC_UENV_ELEM,_PRT_FORC_UENV_ELEM> arUenvElemMax;
    CArray<_PRT_FORC_UENV_ELEM,_PRT_FORC_UENV_ELEM> arUenvElemMin;

    BOOL bDiagram=FALSE;
    if(!Get_ForcDiagram(arUenvElemMax,arUenvElemMin)) return FALSE;
    if(arUenvElemMax.GetSize()==0) return FALSE;

    Prt_ArString PrtArStr; PrtArStr.Initialize();
    CArray<CString,CString> arUltiData;
    int i=0,j=0,k=0,l=0;
    int TotalElem = arUenvElemMax.GetSize();
    CString strUltim = _T("");
    CString strTitle[3] = {_T("Moment(M) Max/Min"), _T("Shear(V) Max/Min"),
        _T("Torsion(T) Max/Min")};
    for(l=0; l<3; l++)
    { 
        arUltiData.RemoveAll();
        strUltim = strTitle[l];      arUltiData.Add(strUltim);
        strUltim.Format(_T("%s %s %s %s %s %s %s %s %s"),_T("Element"), _T("Node"), _T("Max/Min"), _T("Fxx(tonf)"), _T("Fyy(tonf)"), _T("Fzz(tonf)"), _T("Mux(tonf·m)"), _T("Muy(tonf·m)"), _T("Muz(tonf·m)")); arUltiData.Add(strUltim);
        //strUltim.Format(_T("~ ~ ~ (%s) (%s) (%s) (%s) (%s) (%s)"), m_strForce, m_strForce, m_strForce, m_strMoment, m_strMoment, m_strMoment); arUltiData.Add(strUltim);
        for(i=0; i<TotalElem; i++)
        {
            T_ELEM_K ElemK = arUenvElemMax.GetAt(i).ElemK;
            _DGN_FORC_CRC ForceMax, ForceMin;
            switch(l)
            {
            case 0:
                ForceMax = arUenvElemMax.GetAt(i).ForcMy;
                ForceMin = arUenvElemMin.GetAt(i).ForcMy;
                break;
            case 1:
                ForceMax = arUenvElemMax.GetAt(i).ForcVz;
                ForceMin = arUenvElemMin.GetAt(i).ForcVz;
                break;
            case 2:
                ForceMax = arUenvElemMax.GetAt(i).ForcMx;
                ForceMin = arUenvElemMin.GetAt(i).ForcMx;
                break;
            }

            for(j=0; j<2; j++) //i,j
            {
                CString sIJ = (j==0 ? _T("I") : _T("J"));
                for(k=0; k<2; k++) // Max.Min
                {
                    sIJ = (k==0 ? sIJ : _T("~"));
                    CString sMaxMin = (k==0 ? _T("Max") : _T("Min")); 
                    _DGN_FORC_CRC &envForce = (k==0 ? ForceMax : ForceMin);
                    CString sFxx = Check_ValueForm(8, 2, envForce.dFxx[j]); 
                    CString sFyy = Check_ValueForm(8, 2, envForce.dFyy[j]);
                    CString sFzz = Check_ValueForm(8, 2, envForce.dFzz[j]);
                    CString sMux = Check_ValueForm(8, 2, envForce.dMux[j]);
                    CString sMuy = Check_ValueForm(8, 2, envForce.dMuy[j]);
                    CString sMuz = Check_ValueForm(8, 2, envForce.dMuz[j]);

                    strUltim.Format(_T("%d %s %s %s %s %s %s %s %s"),ElemK, sIJ, sMaxMin, sFxx, sFyy, sFzz, sMux, sMuy, sMuz); arUltiData.Add(strUltim);
                }

            }   
        }   
        strUltim = _T("");  arUltiData.Add(strUltim);
        strUltim = _T("");  arUltiData.Add(strUltim);

        PrtArStr.arString.Copy(arUltiData);   
        arStrUltiData.Add(PrtArStr);
    }

    if(arStrUltiData.GetSize()==0) return FALSE;

    return TRUE;  
}

BOOL CPrtKSCE_USD03::Get_StageKeyList(CArray<T_STAG_K,T_STAG_K>& arStagK)
{
    for(int i=0; i<m_arStagK.GetSize(); i++)
    {
        T_STAG_K StagK = m_arStagK.GetAt(i);
        arStagK.Add(StagK);
    }   
    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_LcomKeyList(CArray<T_LCOM_K,T_LCOM_K>& arLcomK)
{
    for(int i=0; i<m_arLcomK.GetSize(); i++)
    {
        T_LCOM_K LcomK = m_arLcomK.GetAt(i);
        arLcomK.Add(LcomK);
    }   
    return TRUE;
}

// 휨강도 검토.
BOOL CPrtKSCE_USD03::Get_FlexStrengthCalc(CArray<CString,CString>& arTheory, CArray<Prt_ArString_Detail,Prt_ArString_Detail>& arStrDetail, CMSExcel* m_pXL)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    ASSERT(pDoc);
    arStrDetail.RemoveAll();

    T_PSCD_D PscdD; PscdD.Initialize();
    pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);

    // PS강재의 응력 계산(0=설계기준, 1=상세한 해석)
    if(PscdD.ifpsMethod==0)
    {
        Print_BdcyTheory_Code(arTheory);  
    }
    else if(PscdD.ifpsMethod==1)
    {
        Print_BdcyTheory_Iter(arTheory);
    }

    if(m_arElemK.GetSize()==0) return FALSE;
    int TotalElem = m_arElemK.GetSize();  
    for(int i=0; i<TotalElem; i++)
    {
        auto ElemK = m_arElemK.GetAt(i);  //수정.
        // Initialize.
        m_arText.RemoveAll();   
        m_iFormula = 0;
        Prt_ArString arString; arString.Initialize();
        Prt_ArString_Detail arDetail; arDetail.Initialize();    
        if(!Get_DetailBdcyPos(ElemK)) continue;
        arDetail.iFormula = m_iFormula;
        arDetail.ArString.arString.Copy(m_arText);
        arStrDetail.Add(arDetail);    

        // Initialize.
        m_arText.RemoveAll();   
        m_iFormula = 0;
        arString.Initialize();
        arDetail.Initialize();    
        if(!Get_DetailBdcyNeg(ElemK)) continue;        
        arDetail.iFormula = m_iFormula;
        arDetail.ArString.arString.Copy(m_arText);
        arStrDetail.Add(arDetail);    
    }   

    if(arStrDetail.GetSize()==0) return FALSE;
    return TRUE;
}




BOOL CPrtKSCE_USD03::Get_FlexStrengthTable_KSCE10(CArray<CString,CString>& arPMomTable, CArray<CString,CString>& arNMomTable, CMSExcel* pXL)
{
    // 도로교10, 철도교11을 위해 임의로 형성

    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_FlexStrengthTable(CArray<CString,CString>& arPMomTable, CArray<CString,CString>& arNMomTable)
{
    //요소 전체..
    ArrElemPairKey arElemK;
    CArray<PRT_BDCY_KSCE,PRT_BDCY_KSCE> arBdcrKsce;

    int i,j,k;
    CString strStress = _T("kgf/cm²");
    CString strArea=_T("cm²");
    CString strAreaM=_T("m²");
    CString strStif=_T("cm⁴");
    CString strStifM=_T("m⁴");
    CString strTable;
    for(i=0; i<2; i++) // 0=P-Moment 1=N-Moment
    {
        CArray<PRT_BDCY_KSCE,PRT_BDCY_KSCE> arBdcrKsce; 
        arBdcrKsce.RemoveAll();
        if(i==0) { if(!Get_ResultBdcyPos(arElemK, arBdcrKsce)) continue; }	// My(+).
        else     { if(!Get_ResultBdcyNeg(arElemK, arBdcrKsce)) continue; }	// My(-).       

        strTable = (i==0 ? _T("<Table>정모멘트에 대한 검토") : _T("<Table>부모멘트에 대한 검토"));    
        if(i==0) { arPMomTable.Add(strTable); }
        else     { arNMomTable.Add(strTable); } 
        strTable = _T("<MV>Elem Position Mu fpe Aps As I h b d dp fps a ΦMn 안전  최대  철근량  1.2Mcr  최소"); 
        if(i==0) { arPMomTable.Add(strTable); }
        else     { arNMomTable.Add(strTable); } 
        //   1   2    3    4    5    6    7    8    9   10    11   12   13   14  15  16   17   18
        strTable.Format(_T("</MV>Elem %s (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s)  %s  %s  %s  (%s)  %s"),
            _T("I/J"), m_strMoment,strStress,strArea,strArea,strStifM,m_strLength,m_strLength,m_strLength,
            m_strLength,strStress,m_strLength,m_strMoment,_T("여부"),_T("강재량"),_T("검토"),m_strMoment,_T("철근비")); 
        if(i==0) { arPMomTable.Add(strTable); }
        else     { arNMomTable.Add(strTable); } 

        if(arElemK.GetSize()==0) continue;
        for(j=0; j<arElemK.GetSize(); j++)
        {
            auto ElemK = arElemK.GetAt(j);
            PRT_BDCY_KSCE BdcyKsceD = arBdcrKsce.GetAt(j);
            for(k=0; k<2; k++)	// 0=I, 1=J
            {
                if(!BdcyKsceD.BdcyBase[k].bCHK) continue;
                CString sIJ     = (k==0 ? _T("I") : _T("J"));
                CString sMuy   = Check_ValueForm(9,2,BdcyKsceD.BdcyBase[k].dMuy);   // 2
                CString sfpe   = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[k].dfpe);   // 3
                CString sAps   = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[k].dAps);   // 4
                CString sAst   = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[k].dAst);   // 5
                CString sIy    = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[k].dIy);    // 6
                CString sh     = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[k].dh); //dh   // 7
                CString sb     = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[k].db);
                CString sdst   = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[k].ddst);   // 9
                CString sdps   = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[k].ddps);   // 10
                CString sfps   = Check_ValueForm(9,2,BdcyKsceD.BdcyBase[k].dfps);   // 11
                CString sa     = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[k].da);     // 12
                CString spMny  = Check_ValueForm(8,2,BdcyKsceD.BdcyBase[k].dpMny);  // 13
                CString sMnyChk = (BdcyKsceD.BdcyBase[k].dMuy<BdcyKsceD.BdcyBase[k].dpMny ? _T("O.K") : _T("N.G"));  // 14
                CString sqp    = Check_ValueForm(6,6,BdcyKsceD.BdcyBase[k].dqp);    // 15
                CString sMaxPsChk = (BdcyKsceD.BdcyBase[k].dqp<BdcyKsceD.BdcyBase[k].d036Beta1 ? _T("O.K") : _T("N.G"));  // 16
                CString s12Mcr = Check_ValueForm(9,2,BdcyKsceD.BdcyBase[k].d12Mcr);   // 17
                CString sMinPsChk = (BdcyKsceD.BdcyBase[k].d12Mcr<BdcyKsceD.BdcyBase[k].dpMny ? _T("O.K") : _T("N.G"));   // 18
                //if((2.0*BdcyKsceD.BdcyBase[k].dMuy)<BdcyKsceD.BdcyBase[k].dpMny) sMinPsChk = _T("O.K*");
                /*
                CString sPe    = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[j].dPe);		
                CString sAsc   = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[j].dAsc);
                CString sdsc   = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[j].ddsc);
                CString syt    = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[j].dyt);
                CString seps   = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[j].deps);
                CString sAc    = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[j].dAc);
                CString sfy    = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[j].dfy);		
                CString sfck   = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[j].dfck);
                */
                // 1  2  3  4  5  6   7  8  9 10 11 12 13 14 15 16 17 18 19    20    21    22    23    24    25
                strTable.Format(_T("%d %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s"), 
                    ElemK, sIJ, sMuy, sfpe, sAps, sAst, sIy, sh, sb, sdst, sdps, sfps, sa, spMny, sMnyChk, sqp, sMaxPsChk, s12Mcr, sMinPsChk);
                if(i==0) { arPMomTable.Add(strTable); }
                else     { arNMomTable.Add(strTable); }
            }
        }
    }
    strTable = _T("</Table>");
    arPMomTable.Add(strTable);
    arNMomTable.Add(strTable);

    return TRUE;    
}

BOOL CPrtKSCE_USD03::Get_PosiNegaBMD(CArray<CString,CString>& strTitle, CArray<CString,CString>& strCol, CArray<double,double>& dValue, CArray<ChartData,ChartData>& arChaBMD)
{
    ArrElemPairKey arElemK;
    CArray<PRT_BDCY_KSCE,PRT_BDCY_KSCE> arBdcrKsce;
    if(!Get_ResultBdcyPos(arElemK, arBdcrKsce)) return FALSE;	// My.
    if(arElemK.GetSize()==0) return FALSE;
    int TotalElem = arElemK.GetSize();
    double dMinXValue = arElemK.GetAt(0).first-1;
    double dMaxXValue = arElemK.GetAt(TotalElem-1).first+1;

    int i,j,k;
    for(i=0; i<2; i++) // 0:P-Moment, 1:N-Moment
    {
        if(i==0) { if(!Get_ResultBdcyPos(arElemK, arBdcrKsce)) continue; }	// My(+).
        else     { if(!Get_ResultBdcyNeg(arElemK, arBdcrKsce)) continue; }	// My(-).       

        CString sElem, sNode, sYMax, sYMin;
        CString sChartTitle;
        CString sYaxis;
        //double dFormOrgToCurr = 0.0;
        int iStaRow = 2;    
        switch(i)   //0:정모멘트, 1:부모멘트
        {
        case 0:
            sElem=_T("A"); sNode=_T("B"); sYMax=_T("C"); sYMin=_T("D");
            sChartTitle=_T("휨강도-정모멘트");  
            sYaxis.Format(_T("Moment(%s)"), m_strMomentUnit);
            break;
        case 1:
            sElem=_T("F"); sNode=_T("G"); sYMax=_T("H"); sYMin=_T("I");
            sChartTitle=_T("휨강도-부모멘트");
            sYaxis.Format(_T("Moment(%s)"), m_strMomentUnit);
            break;
        }

        CString strTemp;
        strTemp.Format(_T("%s%d"),sElem,iStaRow);  strTitle.Add(strTemp);      
        strTemp = _T("Element");   strTitle.Add(strTemp);
        strTemp.Format(_T("%s%d"),sNode,iStaRow);  strTitle.Add(strTemp);      
        strTemp = _T("I/J");   strTitle.Add(strTemp);
        strTemp.Format(_T("%s%d"),sYMax,iStaRow);  strTitle.Add(strTemp);
        strTemp = _T("ΦMn");   strTitle.Add(strTemp);
        strTemp.Format(_T("%s%d"),sYMin,iStaRow);  strTitle.Add(strTemp);
        strTemp = _T("Mu");   strTitle.Add(strTemp);

        int TotalElem = arElemK.GetSize();    
        double dNode[2] = { 0.0, 0.0 };
        for(j=0; j<TotalElem; j++)
        {    
            auto ElemK = arElemK.GetAt(j);
            if (ElemK.second == EN_EL_BEAM)
            {
                T_ELEM_D ElemD;
                if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK.first, ElemD)) continue;
                int nNode = m_pDoc->m_pAttrCtrl->GetNumNode(ElemD.eltyp, ElemD.elnod);
                for (k = 0; k < nNode; k++)
                {
                    dNode[k] = ElemD.elnod[k];
                }
            }
            else if (ElemK.second == EN_EL_VBEAM)
            {
                if (j == 0) 
                    dNode[0] = 1;
                if (j == TotalElem - 1)
                    dNode[1] = j;
            }
            else ASSERT(0);

            if(j==0)            dMinXValue = dNode[0] - 1;
            if(j==TotalElem-1)  dMaxXValue = dNode[1] + 1;

            PRT_BDCY_KSCE PrtBdcyD = arBdcrKsce.GetAt(j);
            // 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

            double dpMn[2], dMu[2];
            CString sAcol, sBcol, sCcol, sDcol;
            for(k=0; k<2; k++)  //0=I, 1=J
            {
                // 0:P-Moment, 1:N-Moment
                double dMuFabs = (i==0 ? max(0.,PrtBdcyD.BdcyBase[k].dMuy) : min(0.,PrtBdcyD.BdcyBase[k].dMuy)); 
                dpMn[k] = PrtBdcyD.BdcyBase[k].dpMny;
                dMu[k]  = fabs(dMuFabs);

                sAcol.Format(_T("%s%d"),sElem, iStaRow+j*2+k+1);
                strCol.Add(sAcol);  dValue.Add(ElemK.first);

                sBcol.Format(_T("%s%d"),sNode, iStaRow+j*2+k+1);
                strCol.Add(sBcol);  dValue.Add(dNode[k]);

                sCcol.Format(_T("%s%d"),sYMax, iStaRow+j*2+k+1);
                strCol.Add(sCcol);  dValue.Add(dpMn[k]);

                sDcol.Format(_T("%s%d"),sYMin, iStaRow+j*2+k+1);
                strCol.Add(sDcol);  dValue.Add(dMu[k]);
            }         
        }
        CString sScope=_T("");
        sScope.Format(_T("%s%d:%s%d"),sNode, iStaRow+1 , sYMin, (iStaRow + TotalElem*2));
        ChartData chMoment;
        chMoment.SourceData = sScope;
        chMoment.bLegend = TRUE;
        chMoment.ChartTitle = sChartTitle;
        chMoment.ChartType = xlXYScatterLinesNoMarkers; //Chart Type : 꺾은선 xlLine, xlLineMarkers, 분산형 xlXYScatter, xlXYScatterSmooth, xlXYScatterSmoothNoMarkers, xlXYScatterLines, xlXYScatterLinesNoMarkers
        chMoment.PlotBy = xlColumns;
        chMoment.xTitle = _T("Node");
        chMoment.yTitle = sYaxis;
        chMoment.dXmax = dMaxXValue;
        chMoment.dXmin = dMinXValue;
        chMoment.aSeries.RemoveAll();
        chMoment.aSeries.Add(_T("ΦMn"));
        chMoment.aSeries.Add(_T("Mu"));
        chMoment.nXunit = 1;

        //ChData.dYmax = dMaxYValue;
        //double dXmax;//X축 최대값
        //double dYmax;//Y축 최대값
        //long   nXunit;//X축 주눈금
        //long   nYunit;//Y축 주눈금
        arChaBMD.Add(chMoment);
    }

    if(dValue.GetSize()==0) return FALSE;

    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_ShearStrengthCalc(CArray<CString,CString>& arTheory, CArray<Prt_ArString_Detail,Prt_ArString_Detail>& arStrDetail)
{
    arStrDetail.RemoveAll();
    Print_CumsTheory(arTheory);  // Print Theory 

    m_arText.RemoveAll();
    m_iFormula = 0;
    int TotalElem = m_arElemK.GetSize();
    for(int i=0; i<TotalElem; i++)
    {
        auto ElemK = m_arElemK.GetAt(i);     
        // Initialize.
        m_arText.RemoveAll();   
        m_iFormula = 0;
        Prt_ArString arString; arString.Initialize();
        Prt_ArString_Detail arDetail; arDetail.Initialize();    
        if(!Get_DetailCums(ElemK)) continue;    
        arDetail.iFormula = m_iFormula;
        arDetail.ArString.arString.Copy(m_arText);
        arStrDetail.Add(arDetail);    
    } 

    if(arStrDetail.GetSize()==0) return FALSE;

    return TRUE;
}



BOOL CPrtKSCE_USD03::Get_ShearStrengthTable_KSCE10(CArray<CString,CString>& arShearTable, CMSExcel* pXL)
{
    // 도로교10, 철도교11을 위해 임의로 형성

    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_ShearStrengthTable(CArray<CString,CString>& arShearTable)
{

    ArrElemPairKey arElemK;
    CArray<PRT_CUMS_KSCE,PRT_CUMS_KSCE> arCumsKsce;
    if(!Get_ResultCums(arElemK, arCumsKsce)) return FALSE;	// My.
    if(arElemK.GetSize()==0) return FALSE;

    int i,j;
    CString strStress=_T("kgf/cm²");
    CString strArea=_T("cm²");
    CString strAreaM=_T("m²");
    CString strStif=_T("cm⁴");
    CString strStifM=_T("m⁴");

    CString strTable=_T("");
    strTable = _T("<Table>전단에 대한 검토");    arShearTable.Add(strTable);
    strTable = _T("<MV>Elem Position bw  d  h  I yt  fpe  Mcr  Vci  fpc  Vp  Vcw  ΦVc  Vu ΦVs  s  Av  As  Av,req  As,use ΦVn  안전"); arShearTable.Add(strTable);
    //   1    2  3    4    5     6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26  27
    strTable.Format(_T("</MV>Elem %s (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s)"),
        _T("I/J"), m_strLength,m_strLength,m_strLength,strStifM,m_strLength,strStress,m_strMoment,m_strForce,
        strStress,m_strForce,m_strForce,m_strForce,m_strForce,m_strForce,m_strLength,strArea,strArea,strArea,
        strArea,m_strForce,_T("여부")); arShearTable.Add(strTable);

    for(i=0; i<arElemK.GetSize(); i++)
    {    
        auto ElemK = arElemK.GetAt(i);
        // 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
        PRT_CUMS_KSCE PrtCumsD= arCumsKsce.GetAt(i);
        for(j=0; j<2; j++)	// 0=I, 1=J
        {      
            if(!PrtCumsD.CumsBase[j].bCHK) continue;
            CString sIJ     = (j==0 ? _T("I") : _T("J"));
            CString sbw     = Check_ValueForm(6,2,PrtCumsD.CumsBase[j].dbw);   
            CString sdst    = Check_ValueForm(6,2,PrtCumsD.CumsBase[j].dd);  
            CString sh      = Check_ValueForm(6,2,PrtCumsD.CumsBase[j].dh);  
            CString sIy     = Check_ValueForm(6,2,PrtCumsD.CumsBase[j].dIy);   
            CString syt     = Check_ValueForm(6,2,PrtCumsD.CumsBase[j].dyt);
            CString sfpe    = Check_ValueForm(6,2,PrtCumsD.CumsBase[j].dfpe);
            CString sMcr    = Check_ValueForm(8,2,PrtCumsD.CumsBase[j].dMcr);
            CString sVi     = Check_ValueForm(8,2,PrtCumsD.CumsBase[j].dVi);
            CString sMmax   = Check_ValueForm(8,2,PrtCumsD.CumsBase[j].dMmax);
            CString sVci    = Check_ValueForm(8,2,PrtCumsD.CumsBase[j].dVci);
            CString sfpc    = Check_ValueForm(6,2,PrtCumsD.CumsBase[j].dfpc);  
            CString sVp     = Check_ValueForm(8,2,PrtCumsD.CumsBase[j].dVp);
            CString sVcw    = Check_ValueForm(8,2,PrtCumsD.CumsBase[j].dVcw);
            CString spVc    = Check_ValueForm(8,2,0.8*PrtCumsD.CumsBase[j].dVc);  
            CString sVu     = Check_ValueForm(8,2,PrtCumsD.CumsBase[j].dVu);   
            CString spVs    = Check_ValueForm(8,2,0.8*PrtCumsD.CumsBase[j].dVs);   
            CString ssv     = Check_ValueForm(6,2,PrtCumsD.CumsBase[j].dsv);  
            CString sAv     = Check_ValueForm(6,2,PrtCumsD.CumsBase[j].dAv);
            CString sAst    = _T("0.0"); // Ast
            CString sAv_req = Check_ValueForm(6,2,PrtCumsD.CumsBase[j].dAvreq);
            CString sAs_use = _T("0.0"); //As_use
            CString spVn    = Check_ValueForm(8,2,PrtCumsD.CumsBase[j].dpVn);   
            CString sChk    = (PrtCumsD.CumsBase[j].dVu<PrtCumsD.CumsBase[j].dpVn ? _T("O.K") : _T("N.G"));   // 18

            // 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
            strTable.Format(_T("%d %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s"), 
                ElemK, sIJ, sbw, sdst, sh, sIy, syt, sfpe, sMcr, sVci, sfpc, sVp, sVcw, spVc, sVu, spVs, ssv, sAv, sAst, 
                sAv_req, sAs_use, spVn, sChk); //25개
            arShearTable.Add(strTable);      	
        }
    }
    strTable = _T("</Table>");    arShearTable.Add(strTable);  

    if(arShearTable.GetSize()==0) return FALSE;

    return TRUE;
}

BOOL CPrtKSCE_USD03::Get_TorsStrengthCalc(CArray<CString,CString>& arTheory, CArray<Prt_ArString_Detail,Prt_ArString_Detail>& arStrDetail)
{
    arStrDetail.RemoveAll();
    Print_CrmtTheory(arTheory);  // Print Theory 
    if(m_arElemK.GetSize()==0) return FALSE;

    int TotalElem = m_arElemK.GetSize();  
    for(int i=0; i<TotalElem; i++)
    {
        auto ElemK = m_arElemK.GetAt(i);    
        m_arText.RemoveAll();   
        m_iFormula = 0;
        Prt_ArString arString; arString.Initialize();
        Prt_ArString_Detail arDetail; arDetail.Initialize();    
        if(!Get_DetailCrmt(ElemK)) continue;    
        arDetail.iFormula = m_iFormula;
        arDetail.ArString.arString.Copy(m_arText);
        arStrDetail.Add(arDetail);    
    } 

    if(arStrDetail.GetSize()==0) return FALSE;

    return TRUE;  
}

BOOL CPrtKSCE_USD03::Get_ReinforceCalc(CArray<Prt_ArString_Detail,Prt_ArString_Detail>& arStrDetail)
{
    arStrDetail.RemoveAll();
    if(m_arElemK.GetSize()==0) return FALSE;
    m_arText.RemoveAll();   
    m_iFormula = 0;
    int TotalElem = m_arElemK.GetSize();
    for(int i=0; i<TotalElem; i++)
    {
        auto ElemK = m_arElemK.GetAt(i);
        // Initialize.
        m_arText.RemoveAll();   
        m_iFormula = 0;
        Prt_ArString arString; arString.Initialize();
        Prt_ArString_Detail arDetail; arDetail.Initialize();    
        if(!Get_DetailTbarPos(ElemK)) continue;
        arDetail.iFormula = m_iFormula;
        arDetail.ArString.arString.Copy(m_arText);
        arStrDetail.Add(arDetail);    

        // Initialize.
        m_arText.RemoveAll();   
        m_iFormula = 0;
        arString.Initialize();
        arDetail.Initialize();    
        if(!Get_DetailTbarNeg(ElemK)) continue;    
        arDetail.iFormula = m_iFormula;
        arDetail.ArString.arString.Copy(m_arText);
        arStrDetail.Add(arDetail);    
    } 

    if(arStrDetail.GetSize()==0) return FALSE;

    return TRUE;    
}

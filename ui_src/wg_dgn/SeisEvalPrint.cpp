#include "stdafx.h"
#include "SeisEvalPrint.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\ISeisEvalInterface.h"
#include "..\wg_db\SeisEvalDataTool.h"

using namespace std;
#include "..\MIT_Lib\rptostream.h"

CSeisEvalPrint::CSeisEvalPrint(void)
{
}


CSeisEvalPrint::~CSeisEvalPrint(void)
{
}


BOOL CSeisEvalPrint::PrintLinRsltInfWall()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc == nullptr ) { ASSERT(0); return FALSE; }

    CSeisEvalDataTool Tool;
    
    T_ELEM_K_LIST aInfiK;
    int nSizeInfi = Tool.GetEvalInfillFullTypeKeyList(aInfiK);
    if (nSizeInfi < 1)
    {
        AfxMessageBox(_STR(_T("Not exist infill element to evaluate.")));
        return FALSE;
    }

    if (!CheckExistLcom(D_LCOMTYPE_SEISMIC)) { return FALSE; }

    const CString& strOutFile = GetOutFileName(_T("InfillLin"));
    rptwofstream fout(strOutFile, ios::trunc);

    fout<<_T("========================================================================================================================================")<<endl
        <<_T("     MASONRY INFILL SEISMIC EVALUATION RESULT (LINEAR ANALSYS EVALUATION STEP)")<<endl
        <<_T("    [UNIT] = ")<<GetCurrentUnitString()<<endl
        <<_T("========================================================================================================================================")<<endl
        <<_T("  STOR   ELEM   LCOM PART           Pu            Vbj          Vdc            Vbm            Vn         PhiPn       CDR")<<endl
        <<_T("----------------------------------------------------------------------------------------------------------------------------------------")<<endl;

    auto l_PrintPos = [this](rptwofstream& out, const T_EVALLIN_INFILL_CAPA& capa, const T_EVALLIN_INFILL_POS& pos)
    {		
        out<<GetValueForm(14, 3, pos.dPu)
            <<GetValueForm(14, 3, capa.dVbjs)
            <<GetValueForm(14, 3, capa.dVdc)
            <<GetValueForm(14, 3, capa.dVbm)
            <<GetValueForm(14, 3, capa.dVn)
            <<GetValueForm(14, 3, capa.dPhiPn)
            <<GetValueForm(10, 3, pos.dCDR)
            <<endl;
    };

    T_STOR_K StorK;
    T_STOR_D StorD;
    T_LCOM_D LcomD;
    CString strElem = _T("");
    CString strStor = _T("");
    CString strLcom = _T("");
    CString strLcomType = _T("");

    CSeisEvalWrapper SeisEval;
    T_EVALLIN_INFILL EvalInfi;
    for (int i=0; i<nSizeInfi; ++i)
    {
        T_ELEM_K ElemK = aInfiK[i];
        if (!SeisEval.GetInstance()->GetSeisEvalLinRsltInfWall(ElemK, EN_EVAL_SEIS, EvalInfi)) { ASSERT(FALSE); continue; }

        pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ElemK);
        if (!pDoc->m_pAttrCtrl->GetStor(StorK, StorD))  { continue; }

        int nSizeLcom = static_cast<int>(EvalInfi.aEvalLcom.GetSize());
        for (int m=0; m<nSizeLcom; ++m)
        {
            const T_EVALLIN_INFILL_LCOM & EvalColmLcom = EvalInfi.aEvalLcom[m];			

            if (!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, EvalColmLcom.LcomK, LcomD)) { continue; } 
            if (LcomD.nActive == EN_EVAL_NONE) { continue; }

            strElem.Format(_T("%6d"), ElemK);
            strStor.Format(_T("%6s"), StorD.StoryName);
            strLcom.Format(_T("%6s"), LcomD.LoadCombName);

            fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   I");
            l_PrintPos(fout, EvalColmLcom.Capa, EvalColmLcom.EndI);

            fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   J");
            l_PrintPos(fout, EvalColmLcom.Capa, EvalColmLcom.EndJ);				
        }		
        fout<<_T("----------------------------------------------------------------------------------------------------------------------------------------")<<endl;
    }

    fout.close();

    CDBLib::RunTextEditor(strOutFile, _STR(_T("MASONRY Infill Seismic Evaluate - LINEAR ANALYSIS STEP")), TRUE); 

    return TRUE;
}

BOOL CSeisEvalPrint::Print1stRsltConBeam()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc == nullptr ) { ASSERT(0); return FALSE; }

    
    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aBeamK;
    int nSizeBeam = Tool.GetEvalConBeamKeyList(aBeamK);
    if ( nSizeBeam < 1 )
    {
        AfxMessageBox(_STR(_T("Not exist beam element to evaluate.")));
        return FALSE;
    }

    if (!CheckExistLcom(D_LCOMTYPE_SEISMIC)) { return FALSE; }
    
    const CString& strOutFile = GetOutFileName(_T("ConBeam1st"));
    rptwofstream fout(strOutFile, ios::trunc);

    fout<<_T("=============================================================================================================================================================================================================================================================================")<<endl
        <<_T("     RC BEAM SEISMIC EVALUATION RESULT (1ST STEP)")<<endl
        <<_T("    [UNIT] = ")<<GetCurrentUnitString()<<endl
        <<_T("=============================================================================================================================================================================================================================================================================")<<endl
        <<_T("  STOR   ELEM   LCOM PART           MuD           VuD           MuF           VuF           MeP           MeN            Vn JUDGE SC      Rebar      Shear       m-IO       m-LS       m-CP    MDCR-IO    MDCR-LS    MDCR-CP    VDCR-IO    VDCR-LS    VDCR-CP  M-PL  V-PL  PL")<<endl
        <<_T("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;
        
    auto l_PrintPos = [this](rptwofstream& out, const T_EVAL1ST_BEAM_POS& pos)
    {		
        const T_CONBEAM_INFO& Info = pos.Info;
        out<<GetValueForm(14, 3, pos.dMUD)
            <<GetValueForm(14, 3, pos.dVUD)
            <<GetValueForm(14, 3, pos.dMUF)
            <<GetValueForm(14, 3, pos.dVUF)
            <<GetValueForm(14, 3, pos.dMeP)
            <<GetValueForm(14, 3, pos.dMeN)
            <<GetValueForm(14, 3, pos.dVn)
            <<_T(" ")<<GetEvalCtrlType(Info.enCtrlType)<<_T(" ")<<GetSeisConfinedType(Info.bSeisConform)<<_T(" ")<<GetValueForm(10, 3, pos.dMUD < 0.0 ? Info.dRhoIdxTop : Info.dRhoIdxBot)<<_T(" ")<<GetValueForm(10, 3, Info.dShrIdx)
            <<_T(" ")<<GetValueForm(10, 3, pos.MfacM.admFac[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.MfacM.admFac[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.MfacM.admFac[EN_LEVEL_CP])			
            <<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_CP])
            <<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_CP])
            <<_T("    ")<<GetPerformLevel(pos.DCRM.enLevel)<<_T("    ")<<GetPerformLevel(pos.DCRV.enLevel)<<_T("  ")<<GetPerformLevel(pos.enLevel)
            <<endl;
    };

    T_STOR_K StorK;
    T_STOR_D StorD;
    T_LCOM_D LcomD;
    CString strElem = _T("");
    CString strStor = _T("");
    CString strLcom = _T("");
            
    CSeisEvalWrapper SeisEval;
    T_EVAL1ST_BEAM EvalBeam;
    for (int i=0; i<nSizeBeam; ++i)
    {
        T_ELEM_K ElemK = aBeamK[i];
        if (!SeisEval.GetInstance()->GetSeisEval1stRsltConBeam(ElemK, EvalBeam)) { continue; }
                
        pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ElemK);
        if (!pDoc->m_pAttrCtrl->GetStor(StorK, StorD))  { continue; }

        int nSizeLcom = EvalBeam.aEvalLcom.GetSize();
        for (int m=0; m<nSizeLcom; ++m)
        {
            const T_EVAL1ST_BEAM_LCOM & CurLcom = EvalBeam.aEvalLcom[m];			

            if (!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, CurLcom.LcomK, LcomD)) { continue; } 
            if (LcomD.nActive == EN_EVAL_NONE) { continue; }

            strElem.Format(_T("%6d"), ElemK);
            strStor.Format(_T("%6s"), StorD.StoryName);
            strLcom.Format(_T("%6s"), LcomD.LoadCombName);

            fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   I");
            l_PrintPos(fout, CurLcom.EndI);

            fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   J");
            l_PrintPos(fout, CurLcom.EndJ);				
        }		
        fout<<_T("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;
    }

    fout.close();

    CDBLib::RunTextEditor(strOutFile, _STR(_T("RC Beam Seismic Evaluate - 1ST STEP")), TRUE); 

    return TRUE;
}

BOOL CSeisEvalPrint::Print1stRsltConColm()
{
    CDBDoc *pDoc = CDBDoc::GetDocPoint();
    T_SEME_D SemeD;
    if (!pDoc->m_pAttrCtrl2->GetSeme(SemeD)) { SemeD.Initialize(); }
    switch (SemeD.nMethod)
    {
    case D_SEME_METHOD_KISTEC2013: return Print1stRsltConColm_KISTEC2013(); break;
    case D_SEME_METHOD_MOE2018:    return Print1stRsltConColm_MOE2018();    break;
    case D_SEME_METHOD_MOE2019:    return Print1stRsltConColm_MOE2018();    break;
    case D_SEME_METHOD_KISTEC2019: return Print1stRsltConColm_MOE2018();    break;  // 검토 방법은 MOE 와 동일.
    default: ASSERT(FALSE);	break;
    }
    return FALSE;    
}

BOOL CSeisEvalPrint::Print1stRsltConWall()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc == nullptr ) { ASSERT(0); return FALSE; }

    CSeisEvalDataTool Tool;
    
    T_ELEM_K_LIST aElemK;
    int nSizeElem = Tool.GetEvalConWallKeyList(aElemK);
    if (nSizeElem < 1)
    {
        AfxMessageBox(_STR(_T("Not exist wall element to evaluate.")));
        return FALSE;
    }

    if (!CheckExistLcom(D_LCOMTYPE_SEISMIC)) { return FALSE; }
        
    const CString& strOutFile = GetOutFileName(_T("ConWall1st"));
    rptwofstream fout(strOutFile, ios::trunc);


    fout<<_T("===========================================================================================================================================================================================================================================================================================")<<endl
        <<_T("     RC WALL SEISMIC EVALUATION RESULT (1ST STEP)")<<endl
        <<_T("    [UNIT] = ")<<GetCurrentUnitString()<<endl
        <<_T("===========================================================================================================================================================================================================================================================================================")<<endl
        <<_T("  STOR WALLID   LCOM PART           PuD           MuD           VuD           PuF           MuF           VuF            Me            Vn  JUDGE      AXIAL      SHEAR       m-IO       m-LS       m-CP    MDCR-IO    MDCR-LS    MDCR-CP    VDCR-IO    VDCR-LS    VDCR-CP  M-PL  V-PL    PL")<<endl
        <<_T("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;

    auto l_PrintPos = [this](rptwofstream& out, const T_EVAL1ST_WALL_POS& pos)
    {        
        out<<GetValueForm(14, 3, pos.dPUD)
            <<GetValueForm(14, 3, pos.dMUD)
            <<GetValueForm(14, 3, pos.dVUD)
            <<GetValueForm(14, 3, pos.dPUF)
            <<GetValueForm(14, 3, pos.dMUF)
            <<GetValueForm(14, 3, pos.dVUF)
            <<GetValueForm(14, 3, pos.Info.dMe)
            <<GetValueForm(14, 3, pos.Info.dVn)
            <<_T("  ")<<GetEvalCtrlType(pos.Info.enCtrlType)
            <<_T(" ")<<GetValueForm(10, 3, pos.Info.dAxlIdx)
            <<_T(" ")<<GetValueForm(10, 3, pos.Info.dShrIdx)
            <<_T(" ")<<GetValueForm(10, 3, pos.MfacMV.admFac[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.MfacMV.admFac[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.MfacMV.admFac[EN_LEVEL_CP])
            <<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_CP])
            <<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_CP])
            <<_T("    ")<<GetPerformLevel(pos.DCRM.enLevel)<<_T("    ")<<GetPerformLevel(pos.DCRV.enLevel)<<_T("    ")<<GetPerformLevel(pos.enLevel)
            <<endl;
    };

    T_STOR_D StorD;
    T_LCOM_D LcomD;
    CString strWallID = _T("");
    CString strStor   = _T("");
    CString strLcom   = _T("");

    CSeisEvalWrapper SeisEval;
    T_EVAL1ST_WALL EvalWall;		
    for (int i=0; i<nSizeElem; ++i)
    {
        T_ELEM_K ElemK = aElemK[i];
        if (!SeisEval.GetInstance()->GetSeisEval1stRsltConWall(ElemK, EvalWall)) { continue; }

        T_WALL_K WallK;
        WallK.keymap = EvalWall.unWallKey;

        int nSizeLcom = EvalWall.aEvalLcom.GetSize();
        for (int m=0; m<nSizeLcom; ++m)
        {
            const T_EVAL1ST_WALL_LCOM & CurLcom = EvalWall.aEvalLcom[m];			

            if (!pDoc->m_pAttrCtrl->GetStor(WallK.key.storid, StorD)) { continue; }
            if (!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, CurLcom.LcomK, LcomD)) { continue; } 
            if (LcomD.nActive == EN_EVAL_NONE) { continue; }

            strWallID.Format(_T("%6d"), WallK.key.wallid);
            strStor.Format(_T("%6s"), StorD.StoryName);
            strLcom.Format(_T("%6s"), LcomD.LoadCombName);

            fout<<strStor<<_T(" ")<<strWallID<<_T(" ")<<strLcom<<_T(" ")<<_T("   I");
            l_PrintPos(fout, CurLcom.PosI);

            fout<<strStor<<_T(" ")<<strWallID<<_T(" ")<<strLcom<<_T(" ")<<_T("   J");
            l_PrintPos(fout, CurLcom.PosJ);				
        }		
        fout<<_T("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;
    }

    fout.close();

    CDBLib::RunTextEditor(strOutFile, _STR(_T("RC Wall Seismic Evaluate - 1ST STEP")), TRUE); 

    return TRUE;
}

BOOL CSeisEvalPrint::Print1stRsltStlBeam()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc == nullptr ) { ASSERT(0); return FALSE; }

    CSeisEvalDataTool Tool;
    
    T_ELEM_K_LIST aBeamK;
    int nSizeBeam = Tool.GetEvalStlBeamKeyList(aBeamK);
    if (nSizeBeam < 1)
    {
        AfxMessageBox(_STR(_T("Not exist beam element to evaluate.")));
        return FALSE;
    }

    if (!CheckExistLcom(D_LCOMTYPE_SEISMIC)) { return FALSE; }

    const CString& strOutFile = GetOutFileName(_T("StlBeam1st"));
    rptwofstream fout(strOutFile, ios::trunc);
        
    fout<<_T("============================================================================================================================")<<endl
        <<_T("     STL BEAM SEISMIC EVALUATION RESULT (1ST STEP)")<<endl
        <<_T("    [UNIT] = ")<<GetCurrentUnitString()<<endl
        <<_T("============================================================================================================================")<<endl
        <<_T("  STOR   ELEM   LCOM PART           MuD            Me        m-IO       m-LS       m-CP     DCR-IO     DCR-LS     DCR-CP  PL")<<endl
        <<_T("----------------------------------------------------------------------------------------------------------------------------")<<endl;


    auto l_PrintPos = [this](rptwofstream& out, const T_EVAL1ST_STLBEAM_POS& pos)
    {		
        out<<GetValueForm(14, 3, pos.dMu)
            <<GetValueForm(14, 3, pos.dMe)			
            <<_T(" ")<<GetValueForm(10, 3, pos.dmFac[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.dmFac[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.dmFac[EN_LEVEL_CP])			
            <<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_CP])
            <<_T("  ")<<GetPerformLevel(pos.enLevel)
            <<endl;
    };

    T_STOR_K StorK;
    T_STOR_D StorD;
    T_LCOM_D LcomD;
    CString strElem = _T("");
    CString strStor = _T("");
    CString strLcom = _T("");
    CString strLcomType = _T("");

    CSeisEvalWrapper SeisEval;
    T_EVAL1ST_STLBEAM EvalBeam;
    for (int i=0; i<nSizeBeam; ++i)
    {
        const T_ELEM_K ElemK = aBeamK[i];
        if (!SeisEval.GetInstance()->GetSeisEval1stRsltStlBeam(ElemK, EvalBeam)) { continue; }

        pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ElemK);
        if (!pDoc->m_pAttrCtrl->GetStor(StorK, StorD))  { continue; }

        int nSizeLcom = EvalBeam.aEvalLcom.GetSize();
        for (int m=0; m<nSizeLcom; ++m)
        {
            const T_EVAL1ST_STLBEAM_LCOM & CurLcom = EvalBeam.aEvalLcom[m];

            if (!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, CurLcom.LcomK, LcomD)) { continue; } 
            if (LcomD.nActive == EN_EVAL_NONE) { continue; }

            strElem.Format(_T("%6d"), ElemK);
            strStor.Format(_T("%6s"), StorD.StoryName);
            strLcom.Format(_T("%6s"), LcomD.LoadCombName);

            fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<strLcomType<<_T(" ")<<_T("   I");
            l_PrintPos(fout, CurLcom.EndI);

            fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<strLcomType<<_T(" ")<<_T("   J");
            l_PrintPos(fout, CurLcom.EndJ);				
        }		
        fout<<_T("----------------------------------------------------------------------------------------------------------------------------")<<endl;
    }

    fout.close();

    CDBLib::RunTextEditor(strOutFile, _STR(_T("STL Beam Seismic Evaluate - 1ST STEP")), TRUE); 

    return TRUE;
}

BOOL CSeisEvalPrint::Print1stRsltStlColm()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc == nullptr ) { ASSERT(0); return FALSE; }

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aColmK;
    int nSizeColm = Tool.GetEvalStlColmKeyList(aColmK);
    if (nSizeColm < 1)
    {
        AfxMessageBox(_STR(_T("Not exist column element to evaluate.")));
        return FALSE;
    }

    if (!CheckExistLcom(D_LCOMTYPE_SEISMIC)) { return FALSE; }

    const CString& strOutFile = GetOutFileName(_T("StlColm1st"));
    rptwofstream fout(strOutFile, ios::trunc);

    fout<<_T("============================================================================================================================================================================================================================================================================")<<endl
        <<_T("     STL COLUMN SEISMIC EVALUATION RESULT (1ST STEP)")<<endl
        <<_T("    [UNIT] = ")<<GetCurrentUnitString()<<endl
        <<_T("============================================================================================================================================================================================================================================================================")<<endl
        <<_T("  STOR   ELEM   LCOM PART           PuD          MuyD          MuzD           PuF          MuyF          MuzF           PCL          MCLy          MCLz           TCE          MCEy          MCEz      m-IOt      m-LSt      m-CPt      m-IOy      m-LSy      m-CPy      m-IOz      m-LSz      m-CPz     DCR-IO     DCR-LS     DCR-CP  PL")<<endl
        <<_T("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;

    auto l_PrintPos = [this](rptwofstream& out, const T_EVAL1ST_STLCOLM_POS& pos)
    {		
        out<<GetValueForm(14, 3, pos.dPUD)
            <<GetValueForm(14, 3, pos.dMUDy)
            <<GetValueForm(14, 3, pos.dMUDz)
            <<GetValueForm(14, 3, pos.dPUF)
            <<GetValueForm(14, 3, pos.dMUFy)
            <<GetValueForm(14, 3, pos.dMUFz)
            <<GetValueForm(14, 3, pos.dPCL)
            <<GetValueForm(14, 3, pos.dMCLy)
            <<GetValueForm(14, 3, pos.dMCLz)
            <<GetValueForm(14, 3, pos.dTCE)
            <<GetValueForm(14, 3, pos.dMCEy)
            <<GetValueForm(14, 3, pos.dMCEz)
            <<_T(" ")<<GetValueForm(10, 3, pos.dmFacT[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.dmFacT[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.dmFacT[EN_LEVEL_CP])						
            <<_T(" ")<<GetValueForm(10, 3, pos.dmFacy[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.dmFacy[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.dmFacy[EN_LEVEL_CP])						
            <<_T(" ")<<GetValueForm(10, 3, pos.dmFacz[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.dmFacz[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.dmFacz[EN_LEVEL_CP])						
            <<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_CP])
            <<_T("  ")<<GetPerformLevel(pos.DCR.enLevel)
            <<endl;
    };

    T_STOR_K StorK;
    T_STOR_D StorD;
    T_LCOM_D LcomD;
    CString strElem = _T("");
    CString strStor = _T("");
    CString strLcom = _T("");
    CString strLcomType = _T("");

    CSeisEvalWrapper SeisEval;
    T_EVAL1ST_STLCOLM EvalColm;
    for (int i=0; i<nSizeColm; ++i)
    {
        const T_ELEM_K ElemK = aColmK[i];
        if (!SeisEval.GetInstance()->GetSeisEval1stRsltStlColm(ElemK, EvalColm)) { continue; }

        pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ElemK);
        if (!pDoc->m_pAttrCtrl->GetStor(StorK, StorD))  { continue; }

        int nSizeLcom = EvalColm.aEvalLcom.GetSize();
        for (int m=0; m<nSizeLcom; ++m)
        {
            const T_EVAL1ST_STLCOLM_LCOM & CurLcom = EvalColm.aEvalLcom[m];			

            if (!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, CurLcom.LcomK, LcomD)) { continue; } 
            if (LcomD.nActive == EN_EVAL_NONE) { continue; }

            strElem.Format(_T("%6d"), ElemK);
            strStor.Format(_T("%6s"), StorD.StoryName);
            strLcom.Format(_T("%6s"), LcomD.LoadCombName);

            fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   I");
            l_PrintPos(fout, CurLcom.PosI);

            fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   J");
            l_PrintPos(fout, CurLcom.PosJ);				
        }		
        fout<<_T("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;
    }

    fout.close();

    CDBLib::RunTextEditor(strOutFile, _STR(_T("STL Column Seismic Evaluate - 1ST STEP")), TRUE); 

    return TRUE;
}

BOOL CSeisEvalPrint::Print1stRsltStlPanz()
{
    ASSERT(0);
    /// 아직 STL PANEL ZONE 검토는 지원하지 않습니다.
    return FALSE;
}

BOOL CSeisEvalPrint::Print1stRsltStlBrac()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc == nullptr ) { ASSERT(0); return FALSE; }

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aBracK;
    int nSizeColm = Tool.GetEvalStlBracKeyList(aBracK);
    if (nSizeColm < 1)
    {
        AfxMessageBox(_STR(_T("Not exist brace element to evaluate.")));
        return FALSE;
    }

    if (!CheckExistLcom(D_LCOMTYPE_SEISMIC)) { return FALSE; }

    const CString& strOutFile = GetOutFileName(_T("ConBrac1st"));
    rptwofstream fout(strOutFile, ios::trunc);

    fout<<_T("========================================================================================================================================")<<endl
        <<_T("     STL BRACE SEISMIC EVALUATION RESULT (1ST STEP)")<<endl
        <<_T("    [UNIT] = ")<<GetCurrentUnitString()<<endl
        <<_T("========================================================================================================================================")<<endl
        <<_T("  STOR   ELEM   LCOM PART           PCE           PnC          PnT       m-IO       m-LS       m-CP     DCR-IO     DCR-LS     DCR-CP  PL")<<endl
        <<_T("----------------------------------------------------------------------------------------------------------------------------------------")<<endl;

    auto l_PrintPos = [this](rptwofstream& out, const T_EVAL1ST_STLBRAC_POS& pos)
    {		
        out<<GetValueForm(14, 3, pos.dPUD)
            <<GetValueForm(14, 3, pos.dPnC)
            <<GetValueForm(14, 3, pos.dPnT)
            <<_T(" ")<<GetValueForm(10, 3, pos.dmFac[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.dmFac[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.dmFac[EN_LEVEL_CP])						
            <<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_CP])
            <<_T("  ")<<GetPerformLevel(pos.DCR.enLevel)
            <<endl;
    };

    T_STOR_K StorK;
    T_STOR_D StorD;
    T_LCOM_D LcomD;
    CString strElem = _T("");
    CString strStor = _T("");
    CString strLcom = _T("");
    CString strLcomType = _T("");

    CSeisEvalWrapper SeisEval;
    T_EVAL1ST_STLBRAC EvalBrac;
    for (int i=0; i<nSizeColm; ++i)
    {
        T_ELEM_K ElemK = aBracK[i];
        if (!SeisEval.GetInstance()->GetSeisEval1stRsltStlBrac(ElemK, EvalBrac)) { continue; }

        pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ElemK);
        if (!pDoc->m_pAttrCtrl->GetStor(StorK, StorD))  { continue; }

        int nSizeLcom = EvalBrac.aEvalLcom.GetSize();
        for (int m=0; m<nSizeLcom; ++m)
        {
            const T_EVAL1ST_STLBRAC_LCOM & CurLcom = EvalBrac.aEvalLcom[m];			

            if (!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, CurLcom.LcomK, LcomD)) { continue; } 
            if (LcomD.nActive == EN_EVAL_NONE) { continue; }

            strElem.Format(_T("%6d"), ElemK);
            strStor.Format(_T("%6s"), StorD.StoryName);
            strLcom.Format(_T("%6s"), LcomD.LoadCombName);

            fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   I");
            l_PrintPos(fout, CurLcom.PosI);

            fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   J");
            l_PrintPos(fout, CurLcom.PosJ);				
        }		
        fout<<_T("----------------------------------------------------------------------------------------------------------------------------------------")<<endl;
    }

    fout.close();

    CDBLib::RunTextEditor(strOutFile, _STR(_T("STL Brace Seismic Evaluate - 1ST STEP")), TRUE); 

    return TRUE;
}

BOOL CSeisEvalPrint::Print1stRsltSrcBeam()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc == nullptr ) { ASSERT(0); return FALSE; }

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aBeamK;
    int nSizeBeam = Tool.GetEvalSrcBeamKeyList(aBeamK);
    if (nSizeBeam < 1)
    {
        AfxMessageBox(_STR(_T("Not exist beam element to evaluate.")));
        return FALSE;
    }

    if (!CheckExistLcom(D_LCOMTYPE_SEISMIC)) { return FALSE; }

    const CString& strOutFile = GetOutFileName(_T("SrcBeam1st"));
    rptwofstream fout(strOutFile, ios::trunc);
    
    fout<<_T("============================================================================================================================")<<endl
        <<_T("     SRC BEAM SEISMIC EVALUATION RESULT (1ST STEP)")<<endl
        <<_T("    [UNIT] = ")<<GetCurrentUnitString()<<endl
        <<_T("============================================================================================================================")<<endl
        <<_T("  STOR   ELEM   LCOM PART           MuD            Me        m-IO       m-LS       m-CP     DCR-IO     DCR-LS     DCR-CP  PL")<<endl
        <<_T("----------------------------------------------------------------------------------------------------------------------------")<<endl;


    auto l_PrintPos = [this](rptwofstream& out, const T_EVAL1ST_STLBEAM_POS& pos)
    {		
        out<<GetValueForm(14, 3, pos.dMu)
            <<GetValueForm(14, 3, pos.dMe)			
            <<_T(" ")<<GetValueForm(10, 3, pos.dmFac[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.dmFac[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.dmFac[EN_LEVEL_CP])			
            <<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_CP])
            <<_T("  ")<<GetPerformLevel(pos.enLevel)
            <<endl;
    };

    T_STOR_K StorK;
    T_STOR_D StorD;
    T_LCOM_D LcomD;
    CString strElem = _T("");
    CString strStor = _T("");
    CString strLcom = _T("");
    CString strLcomType = _T("");

    CSeisEvalWrapper SeisEval;
    T_EVAL1ST_STLBEAM EvalBeam;
    for (int i=0; i<nSizeBeam; ++i)
    {
        const T_ELEM_K ElemK = aBeamK[i];
        if (!SeisEval.GetInstance()->GetSeisEval1stRsltSrcBeam(ElemK, EvalBeam)) { continue; }

        pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ElemK);
        if (!pDoc->m_pAttrCtrl->GetStor(StorK, StorD))  { continue; }

        int nSizeLcom = EvalBeam.aEvalLcom.GetSize();
        for (int m=0; m<nSizeLcom; ++m)
        {
            const T_EVAL1ST_STLBEAM_LCOM & CurLcom = EvalBeam.aEvalLcom[m];			

            if (!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, CurLcom.LcomK, LcomD)) { continue; } 
            if (LcomD.nActive == EN_EVAL_NONE ) { continue; }

            strElem.Format(_T("%6d"), ElemK);
            strStor.Format(_T("%6s"), StorD.StoryName);
            strLcom.Format(_T("%6s"), LcomD.LoadCombName);

            fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<strLcomType<<_T(" ")<<_T("   I");
            l_PrintPos(fout, CurLcom.EndI);

            fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<strLcomType<<_T(" ")<<_T("   J");
            l_PrintPos(fout, CurLcom.EndJ);				
        }		
        fout<<_T("----------------------------------------------------------------------------------------------------------------------------")<<endl;
    }

    fout.close();

    CDBLib::RunTextEditor(strOutFile, _STR(_T("SRC Beam Seismic Evaluate - 1ST STEP")), TRUE); 

    return TRUE;
}

BOOL CSeisEvalPrint::Print1stRsltSrcColm()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc == nullptr ) { ASSERT(0); return FALSE; }

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aColmK;
    int nSizeColm = Tool.GetEvalSrcColmKeyList(aColmK);
    if (nSizeColm < 1)
    {
        AfxMessageBox(_STR(_T("Not exist column element to evaluate.")));
        return FALSE;
    }

    if (!CheckExistLcom(D_LCOMTYPE_SEISMIC)) { return FALSE; }

    const CString& strOutFile = GetOutFileName(_T("SrcColm1st"));
    rptwofstream fout(strOutFile, ios::trunc);
        
    fout<<_T("=========================================================================================================================================================================================================================================================================================================================================")<<endl
        <<_T("     SRC COLUMN SEISMIC EVALUATION RESULT (1ST STEP)")<<endl
        <<_T("    [UNIT] = ")<<GetCurrentUnitString()<<endl
        <<_T("=========================================================================================================================================================================================================================================================================================================================================")<<endl
        <<_T("  STOR   ELEM   LCOM PART           PuD          MuyD          MuzD           PuF          MuyF          MuzF           PCL          MCLy          MCLz           TCE          MCEy          MCEz      m-IOt      m-LSt      m-CPt      m-IOy      m-LSy      m-CPy      m-IOz      m-LSz      m-CPz     DCR-IO     DCR-LS     DCR-CP  PL")<<endl
        <<_T("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;

    auto l_PrintPos = [this](rptwofstream& out, const T_EVAL1ST_STLCOLM_POS& pos)
    {		
        out<<GetValueForm(14, 3, pos.dPUD)
            <<GetValueForm(14, 3, pos.dMUDy)
            <<GetValueForm(14, 3, pos.dMUDz)
            <<GetValueForm(14, 3, pos.dPUF)
            <<GetValueForm(14, 3, pos.dMUFy)
            <<GetValueForm(14, 3, pos.dMUFz)
            <<GetValueForm(14, 3, pos.dPCL)
            <<GetValueForm(14, 3, pos.dMCLy)
            <<GetValueForm(14, 3, pos.dMCLz)
            <<GetValueForm(14, 3, pos.dTCE)
            <<GetValueForm(14, 3, pos.dMCEy)
            <<GetValueForm(14, 3, pos.dMCEz)
            <<_T(" ")<<GetValueForm(10, 3, pos.dmFacT[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.dmFacT[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.dmFacT[EN_LEVEL_CP])						
            <<_T(" ")<<GetValueForm(10, 3, pos.dmFacy[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.dmFacy[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.dmFacy[EN_LEVEL_CP])						
            <<_T(" ")<<GetValueForm(10, 3, pos.dmFacz[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.dmFacz[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.dmFacz[EN_LEVEL_CP])						
            <<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCR.dDCR[EN_LEVEL_CP])
            <<_T("  ")<<GetPerformLevel(pos.DCR.enLevel)
            <<endl;
    };

    T_STOR_K StorK;
    T_STOR_D StorD;
    T_LCOM_D LcomD;
    CString strElem = _T("");
    CString strStor = _T("");
    CString strLcom = _T("");
    CString strLcomType = _T("");

    CSeisEvalWrapper SeisEval;
    T_EVAL1ST_STLCOLM EvalColm;
    for (int i=0; i<nSizeColm; ++i)
    {
        const T_ELEM_K ElemK = aColmK[i];
        if (!SeisEval.GetInstance()->GetSeisEval1stRsltSrcColm(ElemK, EvalColm)) { continue; }

        pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ElemK);
        if (!pDoc->m_pAttrCtrl->GetStor(StorK, StorD))  { continue; }

        int nSizeLcom = EvalColm.aEvalLcom.GetSize();
        for (int m=0; m<nSizeLcom; ++m)
        {
            const T_EVAL1ST_STLCOLM_LCOM& CurLcom = EvalColm.aEvalLcom[m];			

            if (!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, CurLcom.LcomK, LcomD)) { continue; } 
            if (LcomD.nActive == EN_EVAL_NONE ) { continue; }

            strElem.Format(_T("%6d"), ElemK);
            strStor.Format(_T("%6s"), StorD.StoryName);
            strLcom.Format(_T("%6s"), LcomD.LoadCombName);

            fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   I");
            l_PrintPos(fout, CurLcom.PosI);

            fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   J");
            l_PrintPos(fout, CurLcom.PosJ);				
        }		
        fout<<_T("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;
    }

    fout.close();

    CDBLib::RunTextEditor(strOutFile, _STR(_T("SRC Column Seismic Evaluate - 1ST STEP")), TRUE); 

    return TRUE;
}


BOOL CSeisEvalPrint::Print1stRsltSrcBeam4Memb()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc == nullptr ) { ASSERT(0); return FALSE; }

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aBeamK;
    int nSizeBeam = Tool.GetEvalSrcBeamKeyList(aBeamK);
    if (nSizeBeam < 1)
    {
        AfxMessageBox(_STR(_T("Not exist beam element to evaluate.")));
        return FALSE;
    }

    if (!CheckExistLcom(D_LCOMTYPE_SEISMIC)) { return FALSE; }

    const CString& strOutFile = GetOutFileName(_T("SrcBeam1st"));
    rptwofstream fout(strOutFile, ios::trunc);

    fout<<_T("=====================================================")<<endl
        <<_T("     SRC BEAM SEISMIC EVALUATION RESULT (1ST STEP)")<<endl
        <<_T("    [UNIT] = ")<<GetCurrentUnitString()<<endl
        <<_T("=====================================================")<<endl
        <<_T("  STOR       SECT   ELEM   PL")<<endl
        <<_T("-----------------------------------------------------")<<endl;

    T_STOR_K StorK;
    T_STOR_D StorD;
    T_ELEM_D ElemD;
    T_SECT_D SectD;
    T_LCOM_D LcomD;
    CString strElem = _T("");
    CString strStor = _T("");
    CString strSect = _T("");

    CSeisEvalWrapper SeisEval;
    T_EVAL1ST_STLBEAM EvalBeam;
    for (int i=0; i<nSizeBeam; ++i)
    {
        T_ELEM_K ElemK = aBeamK[i];
        if (!SeisEval.GetInstance()->GetSeisEval1stRsltSrcBeam(ElemK, EvalBeam)) { continue; }

        pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ElemK);
        if (!pDoc->m_pAttrCtrl->GetStor(StorK, StorD))  { continue; }

        if (!pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { continue; }
        if (!pDoc->m_pAttrCtrl->GetSectDesign(ElemD.elpro, SectD)) { continue; }

        strElem.Format(_T("%6d"), ElemK);
        strStor.Format(_T("%6s"), StorD.StoryName);
        strSect.Format(_T("%10s"), SectD.SName);

        fout<<strStor<<_T(" ")<<strSect<<_T(" ")<<strElem<<_T("  ")<<GetPerformLevel(EvalBeam.enLevel)<<endl;		
    }

    fout<<_T("-----------------------------------------------------")<<endl;
    fout.close();

    CDBLib::RunTextEditor(strOutFile, _STR(_T("SRC Beam Seismic Evaluate - 1ST STEP")), TRUE); 

    return TRUE;
}

BOOL CSeisEvalPrint::Print1stRsltSrcColm4Memb()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc == nullptr ) { ASSERT(0); return FALSE; }

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aColmK;
    int nSizeColm = Tool.GetEvalSrcColmKeyList(aColmK);
    if (nSizeColm < 1)
    {
        AfxMessageBox(_STR(_T("Not exist column element to evaluate.")));
        return FALSE;
    }

    if (!CheckExistLcom(D_LCOMTYPE_SEISMIC)) { return FALSE; }

    const CString& strOutFile = GetOutFileName(_T("SrcColm1st"));
    rptwofstream fout(strOutFile, ios::trunc);
    
    fout<<_T("=====================================================")<<endl
        <<_T("     SRC COLUMN SEISMIC EVALUATION RESULT (1ST STEP)")<<endl
        <<_T("    [UNIT] = ")<<GetCurrentUnitString()<<endl
        <<_T("=====================================================")<<endl
        <<_T("  STOR       SECT   ELEM   PL")<<endl
        <<_T("-----------------------------------------------------")<<endl;

    T_STOR_K StorK;
    T_STOR_D StorD;
    T_ELEM_D ElemD;
    T_SECT_D SectD;
    T_LCOM_D LcomD;
    CString strElem = _T("");
    CString strStor = _T("");
    CString strSect = _T("");

    CSeisEvalWrapper SeisEval;
    T_EVAL1ST_STLCOLM EvalColm;
    for (int i=0; i<nSizeColm; ++i)
    {
        T_ELEM_K ElemK = aColmK[i];
        if (!SeisEval.GetInstance()->GetSeisEval1stRsltSrcColm(ElemK, EvalColm)) { continue; }

        pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ElemK);
        if (!pDoc->m_pAttrCtrl->GetStor(StorK, StorD))  { continue; }

        if (!pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { continue; }
        if (!pDoc->m_pAttrCtrl->GetSectDesign(ElemD.elpro, SectD)) { continue; }

        strElem.Format(_T("%6d"), ElemK);
        strStor.Format(_T("%6s"), StorD.StoryName);
        strSect.Format(_T("%10s"), SectD.SName);

        fout<<strStor<<_T(" ")<<strSect<<_T(" ")<<strElem<<_T("  ")<<GetPerformLevel(EvalColm.enLevel)<<endl;		
    }

    fout<<_T("-----------------------------------------------------")<<endl;
    fout.close();

    CDBLib::RunTextEditor(strOutFile, _STR(_T("SRC Column Seismic Evaluate - 1ST STEP")), TRUE); 

    return TRUE;
}

BOOL CSeisEvalPrint::Print1stRsltConColm_KISTEC2013()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc == nullptr ) { ASSERT(0); return FALSE; }

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aColmK;
    int nSizeColm = Tool.GetEvalConColmKeyList(aColmK);    
    if (nSizeColm < 1)
    {
        AfxMessageBox(_STR(_T("Not exist RC column result to evaluate.")));
        return FALSE;
    }

    if (!CheckExistLcom(D_LCOMTYPE_SEISMIC)) { return FALSE; }

    const CString& strOutFile = GetOutFileName(_T("ConColm1st"));
    rptwofstream fout(strOutFile, ios::trunc);

    fout<<_T("==================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================")<<endl
        <<_T("     RC COLUMN SEISMIC EVALUATION RESULT (1ST STEP)")<<endl
        <<_T("    [UNIT] = ")<<GetCurrentUnitString()<<endl
        <<_T("==================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================")<<endl
        <<_T("  STOR   ELEM   LCOM PART           PuD          MuyD          MuzD          VuyD          VuzD           PuF          MuyF          MuzF          VuyF          VuzF           PnC           PnT           Mey           Mez           Vny           Vnz JUDGE SC     AXIALy     SHEARy      m-IOy      m-LSy      m-CPy JUDGE SC     AXIALz     SHEARz      m-IOz      m-LSz      m-CPz    PDCR-IO    PDCR-LS    PDCR-CP    MDCR-IO    MDCR-LS    MDCR-CP    VDCR-IO    VDCR-LS    VDCR-CP  P-PL  M-PL  V-PL  PL")<<endl
        <<_T("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;
    
    auto l_PrintPos = [this](rptwofstream& out, const T_EVAL1ST_COLM_POS& pos)
    {		
        const T_CONCOLM_INFO& Infoy = pos.Diry.Info;
        const T_CONCOLM_INFO& Infoz = pos.Dirz.Info;

        out<<GetValueForm(14, 3, pos.dPUD)
            <<GetValueForm(14, 3, pos.dMUDy)
            <<GetValueForm(14, 3, pos.dMUDz)
            <<GetValueForm(14, 3, pos.dVUDy)
            <<GetValueForm(14, 3, pos.dVUDz)
            <<GetValueForm(14, 3, pos.dPUF)
            <<GetValueForm(14, 3, pos.dMUFy)
            <<GetValueForm(14, 3, pos.dMUFz)
            <<GetValueForm(14, 3, pos.dVUFy)
            <<GetValueForm(14, 3, pos.dVUFz)
            <<GetValueForm(14, 3, pos.dPnC)
            <<GetValueForm(14, 3, pos.dPnT)
            <<GetValueForm(14, 3, pos.dMey)
            <<GetValueForm(14, 3, pos.dMez)
            <<GetValueForm(14, 3, pos.dVny)
            <<GetValueForm(14, 3, pos.dVnz)
            <<_T(" ")<<GetEvalCtrlType(Infoy.enCtrlType)<<_T(" ")<<GetSeisConfinedType(Infoy.bSeisConform)<<_T(" ")<<GetValueForm(10, 3, Infoy.dAxlIdx)<<_T(" ")<<GetValueForm(10, 3, Infoy.dShrIdx)
            <<_T(" ")<<GetValueForm(10, 3, pos.Diry.MfacM.admFac[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.Diry.MfacM.admFac[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.Diry.MfacM.admFac[EN_LEVEL_CP])			
            <<_T(" ")<<GetEvalCtrlType(Infoz.enCtrlType)<<_T(" ")<<GetSeisConfinedType(Infoz.bSeisConform)<<_T(" ")<<GetValueForm(10, 3, Infoz.dAxlIdx)<<_T(" ")<<GetValueForm(10, 3, Infoz.dShrIdx)
            <<_T(" ")<<GetValueForm(10, 3, pos.Dirz.MfacM.admFac[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.Dirz.MfacM.admFac[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.Dirz.MfacM.admFac[EN_LEVEL_CP])
            <<_T(" ")<<GetValueForm(10, 3, pos.DCRP.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCRP.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCRP.dDCR[EN_LEVEL_CP])
            <<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_CP])
            <<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_CP])
            <<_T("    ")<<GetPerformLevel(pos.DCRP.enLevel)<<_T("    ")<<GetPerformLevel(pos.DCRM.enLevel)<<_T("    ")<<GetPerformLevel(pos.DCRV.enLevel)<<_T("  ")<<GetPerformLevel(pos.enLevel)
            <<endl;
    };
        
    T_STOR_K StorK;
    T_STOR_D StorD;
    T_LCOM_D LcomD;
    CString strElem = _T("");
    CString strStor = _T("");
    CString strLcom = _T("");

    CSeisEvalWrapper SeisEval;
    T_EVAL1ST_COLM EvalColm;
    for (int i=0; i<nSizeColm; ++i)
    {
        T_ELEM_K ElemK = aColmK[i];
        if (!SeisEval.GetInstance()->GetSeisEval1stRsltConColm(ElemK, EvalColm)) { continue; }

        pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ElemK);
        if (!pDoc->m_pAttrCtrl->GetStor(StorK, StorD))  { continue; }

        int nLcom = static_cast<int>(EvalColm.aEvalLcom.GetSize());
        for (int m=0; m<nLcom; ++m)
        {
            const T_EVAL1ST_COLM_LCOM& CurLcom = EvalColm.aEvalLcom[m];			

            if (!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, CurLcom.LcomK, LcomD)) { continue; } 
            if (LcomD.nActive == EN_EVAL_NONE ) { continue; }

            strElem.Format(_T("%6d"), ElemK);
            strStor.Format(_T("%6s"), StorD.StoryName);
            strLcom.Format(_T("%6s"), LcomD.LoadCombName);

            fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   I");
            l_PrintPos(fout, CurLcom.PosI);

            fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   J");
            l_PrintPos(fout, CurLcom.PosJ);				
        }		
        fout<<_T("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;
    }

    fout.close();

    CDBLib::RunTextEditor(strOutFile, _STR(_T("RC Column Seismic Evaluate - 1ST STEP")), TRUE); 

        return TRUE;
}

BOOL CSeisEvalPrint::Print1stRsltConColm_MOE2018()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc == nullptr ) { ASSERT(0); return FALSE; }

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aColmK;
    int nSizeColm = Tool.GetEvalConColmKeyList(aColmK);
    if (nSizeColm < 1)
    {
        AfxMessageBox(_STR(_T("Not exist RC column result to evaluate.")));
        return FALSE;
    }

    if (!CheckExistLcom(D_LCOMTYPE_SEISMIC)) { return FALSE; }

    const CString& strOutFile = GetOutFileName(_T("ConColm1st"));
    rptwofstream fout(strOutFile, ios::trunc);

    fout<<_T("==========================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================")<<endl
        <<_T("     RC COLUMN SEISMIC EVALUATION RESULT (1ST STEP) by MOE2018")<<endl
        <<_T("    [UNIT] = ")<<GetCurrentUnitString()<<endl
        <<_T("============================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================")<<endl
        <<_T("  STOR   ELEM   LCOM PART           PuD          MuyD          MuzD          VuyD          VuzD           PuF          MuyF          MuzF          VuyF          VuzF           PnC           PnT           Mey           Mez           Vny           Vnz      AXIAL GROUPy      Rhovy     SHEARy GROUPz      Rhovz     SHEARz      m-IOy      m-LSy      m-CPy      m-IOz      m-LSz      m-CPz    PDCR-IO    PDCR-LS    PDCR-CP    MDCR-IO    MDCR-LS    MDCR-CP    VDCR-IO    VDCR-LS    VDCR-CP  P-PL  M-PL  V-PL  PL")<<endl
        <<_T("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;
        
    auto l_PrintPos = [this](rptwofstream& out, const T_EVAL1ST_COLM_POS& pos)
    {		
        const T_CONCOLM_INFO& Infoy = pos.Diry.Info;
        const T_CONCOLM_INFO& Infoz = pos.Dirz.Info;
        out<<GetValueForm(14, 3, pos.dPUD)
            <<GetValueForm(14, 3, pos.dMUDy)
            <<GetValueForm(14, 3, pos.dMUDz)
            <<GetValueForm(14, 3, pos.dVUDy)
            <<GetValueForm(14, 3, pos.dVUDz)
            <<GetValueForm(14, 3, pos.dPUF)
            <<GetValueForm(14, 3, pos.dMUFy)
            <<GetValueForm(14, 3, pos.dMUFz)
            <<GetValueForm(14, 3, pos.dVUFy)
            <<GetValueForm(14, 3, pos.dVUFz)
            <<GetValueForm(14, 3, pos.dPnC)
            <<GetValueForm(14, 3, pos.dPnT)
            <<GetValueForm(14, 3, pos.dMey)
            <<GetValueForm(14, 3, pos.dMez)
            <<GetValueForm(14, 3, pos.dVny)
            <<GetValueForm(14, 3, pos.dVnz)
            <<_T(" ")<<GetValueForm(10, 3, Infoy.dAxlIdx)
            <<_T(" ")<<GetConColmGroup_MOE2018(Infoy.unGroup)<<_T(" ")<<GetValueForm(10, 3, Infoy.dRhoIdx)<<_T(" ")<<GetValueForm(10, 3, Infoy.dShrIdx)
            <<_T(" ")<<GetConColmGroup_MOE2018(Infoz.unGroup)<<_T(" ")<<GetValueForm(10, 3, Infoz.dRhoIdx)<<_T(" ")<<GetValueForm(10, 3, Infoz.dShrIdx)
            <<_T(" ")<<GetValueForm(10, 3, pos.Diry.MfacM.admFac[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.Diry.MfacM.admFac[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.Diry.MfacM.admFac[EN_LEVEL_CP])			
            <<_T(" ")<<GetValueForm(10, 3, pos.Dirz.MfacM.admFac[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.Dirz.MfacM.admFac[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.Dirz.MfacM.admFac[EN_LEVEL_CP])
            <<_T(" ")<<GetValueForm(10, 3, pos.DCRP.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCRP.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCRP.dDCR[EN_LEVEL_CP])
            <<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCRM.dDCR[EN_LEVEL_CP])
            <<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_IO])<<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_LS])<<_T(" ")<<GetValueForm(10, 3, pos.DCRV.dDCR[EN_LEVEL_CP])
            <<_T("    ")<<GetPerformLevel(pos.DCRP.enLevel)<<_T("    ")<<GetPerformLevel(pos.DCRM.enLevel)<<_T("    ")<<GetPerformLevel(pos.DCRV.enLevel)<<_T("  ")<<GetPerformLevel(pos.enLevel)
            <<endl;
    };

    T_STOR_K StorK;
    T_STOR_D StorD;
    T_LCOM_D LcomD;
    CString strElem = _T("");
    CString strStor = _T("");
    CString strLcom = _T("");

    CSeisEvalWrapper SeisEval;
    T_EVAL1ST_COLM EvalColm;
    for (int i=0; i<nSizeColm; ++i)
    {
        T_ELEM_K ElemK = aColmK[i];
        if (!SeisEval.GetInstance()->GetSeisEval1stRsltConColm(ElemK, EvalColm)) { ASSERT(FALSE); continue; }

        pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ElemK);
        if (!pDoc->m_pAttrCtrl->GetStor(StorK, StorD))  { ASSERT(FALSE); continue; }

        int nSizeLcom = static_cast<int>(EvalColm.aEvalLcom.GetSize());
        for (int m=0; m<nSizeLcom; ++m)
        {
            const T_EVAL1ST_COLM_LCOM& CurLcom = EvalColm.aEvalLcom[m];			

            if (!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, CurLcom.LcomK, LcomD)) { continue; } 
            if (LcomD.nActive == EN_EVAL_NONE ) { continue; }

            strElem.Format(_T("%6d"), ElemK);
            strStor.Format(_T("%6s"), StorD.StoryName);
            strLcom.Format(_T("%6s"), LcomD.LoadCombName);

            fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   I");
            l_PrintPos(fout, CurLcom.PosI);

            fout<<strStor<<_T(" ")<<strElem<<_T(" ")<<strLcom<<_T(" ")<<_T("   J");
            l_PrintPos(fout, CurLcom.PosJ);				
        }		
        fout<<_T("---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")<<endl;
    }

    fout.close();

    CDBLib::RunTextEditor(strOutFile, _STR(_T("RC Column Seismic Evaluate - 1ST STEP")), TRUE); 

    return TRUE;
}

CString CSeisEvalPrint::GetValueForm(const int nTotalDigit, const int nUnderDigit, const double dValue)
{
    BOOL bChange = FALSE;
    if ( nUnderDigit==0 )
    {
        if ( fabs(dValue) >= 1.0*pow(10,nTotalDigit) )
        {
            bChange = TRUE;
        }
        else if ( fabs(dValue) <  1.0*pow(10,0) )
        {
            bChange = TRUE;
        }
    }
    else
    {
        if ( fabs(dValue) >= 1.0*pow(10,nTotalDigit-nUnderDigit-1) )
        {
            bChange = TRUE;
        }
        else if (fabs(dValue) < 1.0*pow(10,-nUnderDigit))
        {
            bChange = TRUE;
        }
    }

    // Add by ZINU.('00.9.5).
    // cRCS_Zero 보다 작은 경우 0.00~으로 출력 by eklee
    if ( fabs(dValue) < 1.0e-7 )
    {
        bChange = FALSE;
    }

    CString strFmt = _T("");
    CString strVal = _T("");
    if ( nUnderDigit==0 && !bChange )
    {
        strFmt.Format(_T("%%%dd"), nTotalDigit);
        strVal.Format(strFmt, int(dValue));
    }
    else
    {
        if ( bChange )
        {
            strFmt.Format(_T("%%%d.%de"), nTotalDigit, nUnderDigit);
        }
        else
        {
            strFmt.Format(_T("%%%d.%df"), nTotalDigit, nUnderDigit);
        }
        strVal.Format(strFmt, dValue);
    }
    return strVal;
}

CString CSeisEvalPrint::GetPerformLevel(const EN_LEVEL enLevel)
{
    switch ( enLevel )
    {
    case EN_LEVEL_IO: return _LS(IDS_TB_DGN_IO);
    case EN_LEVEL_LS: return _LS(IDS_TB_DGN_LS);
    case EN_LEVEL_CP: return _LS(IDS_TB_DGN_CP);
    case EN_LEVEL_CL: return _LSX(**);
    case EN_LEVEL_INVALID: return _LSX(NO);
    default: ASSERT(FALSE); break;
    }
    return _T("  ");
}

CString CSeisEvalPrint::GetEvalCtrlType(const EN_EVALCTRL enCtrl)
{
    switch (enCtrl)
    {
    case EN_EVALCTRL_FLEX: return _T(" FLEX");
    case EN_EVALCTRL_SHER: return _T("SHEAR"); 
    default: ASSERT(0); return _T("     -");
    } 
}

CString CSeisEvalPrint::GetSeisConfinedType(BOOL bSeisConform)
{
    return bSeisConform ? _T(" C") : _T("NC");
}

CString CSeisEvalPrint::GetConColmGroup_MOE2018(UINT unGroup)
{
    CString strGroup = _T("     -");
    switch (unGroup)
    {
    case EN_CONCOLM_GROUP1: strGroup = _T("Group1"); break;
    case EN_CONCOLM_GROUP2: strGroup = _T("Group2"); break;
    case EN_CONCOLM_GROUP3: strGroup = _T("Group3"); break;
    default: ASSERT(FALSE); break;
    }
    return strGroup;
}

BOOL CSeisEvalPrint::CheckExistLcom(const int nLcomType)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc == nullptr ) { ASSERT(0); return FALSE; }

    T_LCOM_K_LIST aLcomK;
    pDoc->m_pAttrCtrl->GetLcomKeyList(nLcomType, aLcomK);
    if ( aLcomK.GetSize() < 1 )
    {
        AfxMessageBox(_STR(_T("Not exist load comination to evaluate.")));
        return FALSE;
    }
    return TRUE;
}

CString CSeisEvalPrint::GetCurrentUnitString()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc == nullptr ) { ASSERT(0); return _T(""); }

    T_UNIT_INDEX CurUnit;
    pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurUnit);
    T_UNIT_NAMEFACT CurUnitNa;
    pDoc->m_pUnitCtrl->GetUnitNameCurrent(CurUnitNa);

    CString strUnit = _T("");
    strUnit.Format(_T("%s, %s"), CurUnitNa.strForce[CurUnit.nBase_Force], CurUnitNa.strLength[CurUnit.nBase_Length]);
    return strUnit;
}


CString CSeisEvalPrint::GetOutFileName(const CString& strOutType)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc == nullptr ) { ASSERT(0); return _T(""); }

    CString strPathName = pDoc->GetPathName();
    CFileCtrl FileCtrl(strPathName);
    CString strOutFile = _T("");	
    if (CDBLib::ActivateGenNXAnal())
        strOutFile.Format(_T("%s_%s.xout"), FileCtrl.GetFilePathAndNameWithoutExtension(), strOutType);
    else
        strOutFile.Format(_T("%s_%s.out"), FileCtrl.GetFilePathAndNameWithoutExtension(), strOutType);

    return strOutFile;
}
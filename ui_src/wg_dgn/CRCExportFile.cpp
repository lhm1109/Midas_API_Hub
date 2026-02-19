// RCSC_BeamDesign.cpp: implementation of the CRCSC_BeamDesign class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "CRCExportFile.h"
#include <winbase.h>

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_FileCtrl.h"
#include "..\wg_db\wg_db_MembCtrl.h"
#include "..\wg_db\wg_db_LoadDB.h"
#include "..\wg_db\wg_db_NewSect.h"
#include "..\wg_db\wg_db_GridInfo.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\wg_db_TendonInfo.h"
#include "..\wg_db\ReductionMomentInfo.h"
#include "..\wg_db\DBCodeDef.h"
#include "..\ReleaseInfo.h"


#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\product.h"

#include "CRCDataCtrl.h"
#include "CRCForceCtrl.h"
#include "DgnResultFileCtrl.h"
#include "..\MIT_Lib\rptostream.h" // for rptwofstream


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define DEGTORAD (3.14159265358979323846/180.0)

CCRCExportFile::CCRCExportFile()
{
    m_bDeg = FALSE;
    m_DataNum.Initialize();
    m_bStag = FALSE;
    m_PSCBeamBaseD.Initialize();
    m_PSCBeamPostD.Initialize();
    m_PSCBeamStagDMap.RemoveAll();
    m_ExtElemBaseD.Initialize();
    m_ExtElemPostD.Initialize();
    m_ExtElemStagDMap.RemoveAll();
    m_StagKList.RemoveAll();
    m_iBrgDir = 0;

    // Coded by Seungjun for Test ('20060719).
    CString csDesignOutput = CProduct::GetTestEnvValue(_ULS(Design Output));
    m_bTestMode = (csDesignOutput==_ULS(yes))? TRUE : FALSE;	
}

CCRCExportFile::~CCRCExportFile()
{
    CDBDoc::GetDocPoint()->SetDesignStatus(0);	// Initialize.
}


BOOL CCRCExportFile::Export_File()
{
    // Change, Jaeoh (06.12.06)
    /*
    if (CDBDoc::GetDocPoint()->IsLimitedTrialVersion())
    {
    //AfxMessageBox(_LS(IDS_TRIAL_NO_LICENSE));
    CDBDoc::GetDocPoint()->ShowLimitedTrialVersionErrMsg();
    return FALSE;
    }
    */
    // Add by ZINU.('05.09.05).	PSC-Design/Strs(5), PSC-Design/Strn(6).
    int iDgnStatus = m_pDoc->GetDesignStatus();
    m_pDoc->SetDesignStatus(6);

    // Mgt File로 Export할 때에는 base stage로 전환한다.
    T_STAG_K CurStag = m_pDoc->m_pStagCtrl->GetCurStag();
    int iCurStep = m_pDoc->m_pStagCtrl->GetCurStagStepIndex()+1;

    // KYE-HONG-20051123
    // 모멘트저감 데이터는 Post CS에서 만들어야 한다. 
    // MakeStageData()에서 Base로 만들기 때문에 
    // 함수전에 모멘트저감 데이터를 생성해 둔다 
    m_pDoc->m_pPostCtrl->GetReductionMoment()->MakeReductionLoad();  

    if(!MakeStageData()) //이 함수를 실행하면 현재 모드가 Base모드로 바뀐다. 
    {
        m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);
        return FALSE;
    }

    MakeDataSize();

    CFileException FileException;
    CString strPathName=m_pDoc->GetPathName();
    CFileCtrl FileCtrl(strPathName);
    CString strFilePathName=FileCtrl.GetFilePathAndNameWithoutExtension()+_T(".jpd");
    Set_FileName(strFilePathName);

    TRY
    {
        if (!m_cfile.Open(strFilePathName, CFile::modeCreate | CFile::modeWrite, &FileException))
            AfxThrowFileException(FileException.m_cause, FileException.m_lOsError, FileException.m_strFileName);

        if(!Write_Data()) 
        {
            m_cfile.Close();
            CFileStatus status;
            if(CFile::GetStatus(strFilePathName, status))CFile::Remove(strFilePathName);
            m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);
            // 
            m_DataCtrl.GetDgnResultFileCtrl()->DeleteDesignResultFile(D_DESIGN_FORCE_RESULT,FALSE);    
            return FALSE;
        }

        m_cfile.Close();
        // 
        m_DataCtrl.GetDgnResultFileCtrl()->DeleteDesignResultFile(D_DESIGN_FORCE_RESULT,FALSE);    

        // Add by han.('06.05.18). 상시검증체계를 위하여 Test모드일경우 Jpd데이터를 복사하여 놓는다
        if(m_bTestMode)
        {
            CopyFile(strFilePathName, FileCtrl.GetFilePathAndNameWithoutExtension()+_T("_jpd.txt"), false);
        }
    }
    CATCH(CFileException, e)
    {
        e->ReportError();
        m_cfile.Abort();
        CFileStatus status;
        if(CFile::GetStatus(strFilePathName, status))CFile::Remove(strFilePathName);
        m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);
        // 
        m_DataCtrl.GetDgnResultFileCtrl()->DeleteDesignResultFile(D_DESIGN_FORCE_RESULT,FALSE);    
        return FALSE;
    }
    END_CATCH

        m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);

    //
    //pDataCtrl = &(this->m_DataCtrl);

    return TRUE;
}

BOOL CCRCExportFile::MakeStageData()
{
    T_PSCD_D PscdD;
    PscdD.Initialize();
    m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
    // Add by ZINU.('06.02.16). Save iBrgDir.
    m_iBrgDir = PscdD.iBrgDir;

    CArray<T_STAG_K, T_STAG_K> StagKList; 
    StagKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetStagKeyList(StagKList);

    T_STAG_K StagK;
    T_STAG_D StagD;
    T_SECT_K SectK, rSectK;
    T_SECT_D SectD;
    T_MATL_K MatlK, rMatlK;
    T_MATD_D MatdD;
    T_ELEM_D ElemD; //Attribute Control
    T_ELEM_D PElemD; //Post Control
    CArray<T_ELEM_K, T_ELEM_K> ElemKList;
    CMap<T_SECT_K, T_SECT_K, T_SECT_K, T_SECT_K> PSCSectKMap; PSCSectKMap.RemoveAll();
    CMap<T_MATL_K, T_MATL_K, T_MATL_K, T_MATL_K> PSCMatlKMap; PSCMatlKMap.RemoveAll();
    CMap<T_SECT_K, T_SECT_K, T_SECT_K, T_SECT_K> ExtSectKMap; ExtSectKMap.RemoveAll();
    CMap<T_MATL_K, T_MATL_K, T_MATL_K, T_MATL_K> ExtMatlKMap; ExtMatlKMap.RemoveAll();

    CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> PSCBeamKMap; PSCBeamKMap.RemoveAll();


    T_STAG_K FinalStagK = m_pDoc->m_pAttrCtrl->GetFinalStag();

    //PostCS
    m_pDoc->m_pStagCtrl->SetFinalStagWithoutUpdateNotify();
    PSCSectKMap.RemoveAll();
    PSCMatlKMap.RemoveAll();
    ExtSectKMap.RemoveAll();
    ExtMatlKMap.RemoveAll();
    // Add by ZINU.('04.08.13). Check Elements for PSC Design.
    ArrElemPairKey aElemK;
    if(!m_DataCtrl.Get_CrcElemListForDgn(TRUE,aElemK))	return FALSE;

    m_PSCBeamPostD.Initialize();
    m_ExtElemPostD.Initialize();
    ElemKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetElemKeyList(ElemKList);
    for(int j=0; j<ElemKList.GetSize(); j++)
    {
        ElemD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetElem(ElemKList[j], ElemD)) continue; 
        if(!m_pDoc->m_pAttrCtrl->IsBeam(ElemD.eltyp) && !m_pDoc->m_pAttrCtrl->IsTruss(ElemD.eltyp)) continue; //Beam이 아닌 경우 리턴 

        //MatlK는 Attribute Control에있는 요소의 MatlK를 사용
        MatlK = ElemD.elmat;
        MatdD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(MatlK, MatdD)) continue;
        if(MatdD.Type != _T("C") && MatdD.Type != _T("SRC")) continue;

        //SectK는 Post Control에있는 요소의 SectK를 사용 
        //(변단면 그룹을 사용한경우에는 PostControl을 사용해야함)
        PElemD.Initialize();
        if(!m_pDoc->m_pPostCtrl->GetElemPost(ElemKList[j], PElemD)) continue;
        SectK = PElemD.elpro;
        SectD.Initialize();
        if(!m_pDoc->m_pPostCtrl->GetSectPost(SectK, SectD)) continue;

        //파형강판을 사용한 PSCBeam인 경우만 SRC허용
        if(MatdD.Type == _T("SRC") && SectD.SectBefore.Shape != D_SECT_SHAPE_PSC_CMPWEB) continue;

        BOOL bPSCBeam = TRUE;
        if(m_pDoc->m_pAttrCtrl->IsTruss(ElemD.eltyp)) bPSCBeam = FALSE;
        if(bPSCBeam && m_pDoc->m_pAttrCtrl->GetMemberType(ElemKList[j]) != D_MBTP_BEAM) bPSCBeam = FALSE;
        if(bPSCBeam && !CDBLib::IsPscDgnSect(SectD)) bPSCBeam = FALSE;

        if(bPSCBeam)
        {
            PSCBeamKMap.SetAt(ElemKList[j],ElemKList[j]);

            BOOL bPSCBeamOK=TRUE;
            if(PscdD.iDgnCode==JSCE02)
            {
                if(MatdD.Type == _T("C"))
                {
                    if(MatdD.Data1.CodeName!=MATLCODE_CON_JIS_CIVIL)	bPSCBeamOK = FALSE;
                    if(MatdD.Rebar_CodeName!=MATLCODE_CON_JIS_CIVIL)	bPSCBeamOK = FALSE;
                }
                else if(MatdD.Type == _T("SRC"))
                {
                    if(MatdD.Data1.CodeName!= MATLCODE_STL_JIS_CIVIL)	bPSCBeamOK = FALSE;
                    if(MatdD.Data2.CodeName!=MATLCODE_CON_JIS_CIVIL)	bPSCBeamOK = FALSE;
                    if(MatdD.Rebar_CodeName!=MATLCODE_CON_JIS_CIVIL)	bPSCBeamOK = FALSE;
                }
            }
            else if(PscdD.iDgnCode==KSCE_USD03 || PscdD.iDgnCode==KSCE_USD05 || PscdD.iDgnCode==KSCE_USD10 
                || PscdD.iDgnCode==KSCE_RAIL_USD04 || PscdD.iDgnCode==KSCE_RAIL_USD11
                || PscdD.iDgnCode==JTG_D62_04 || PscdD.iDgnCode==CJJ_11_2011 )
            {
                bPSCBeamOK = TRUE;
            }
            else	ASSERT(0);

            if(bPSCBeamOK)
            {
                m_PSCBeamPostD.PElemKList.Add(ElemKList[j]);
                m_PSCBeamPostD.PSectKMap.SetAt(ElemKList[j],SectK);
                m_PSCBeamPostD.MatlKMap.SetAt(ElemKList[j],MatlK);
                PSCSectKMap.SetAt(SectK, SectK);
                PSCMatlKMap.SetAt(MatlK, MatlK);
            }
        }
        else
        {
            m_ExtElemPostD.PElemKList.Add(ElemKList[j]);
            m_ExtElemPostD.PSectKMap.SetAt(ElemKList[j],SectK);
            m_ExtElemPostD.MatlKMap.SetAt(ElemKList[j],MatlK);
            ExtSectKMap.SetAt(SectK, SectK);
            ExtMatlKMap.SetAt(MatlK, MatlK);
        }

    }

    POSITION Pos = PSCSectKMap.GetStartPosition();
    while(Pos)
    {
        rSectK = 0;
        PSCSectKMap.GetNextAssoc(Pos, SectK, rSectK);
        m_PSCBeamPostD.PSectKList.Add(rSectK);
    } 

    Pos = PSCMatlKMap.GetStartPosition();
    while(Pos)
    {
        rMatlK = 0;
        PSCMatlKMap.GetNextAssoc(Pos, MatlK, rMatlK);
        m_PSCBeamPostD.MatlKList.Add(rMatlK);
    } 

    Pos = ExtSectKMap.GetStartPosition();
    while(Pos)
    {
        rSectK = 0;
        ExtSectKMap.GetNextAssoc(Pos, SectK, rSectK);
        m_ExtElemPostD.PSectKList.Add(rSectK);
    } 

    Pos = ExtMatlKMap.GetStartPosition();
    while(Pos)
    {
        rMatlK = 0;
        ExtMatlKMap.GetNextAssoc(Pos, MatlK, rMatlK);
        m_ExtElemPostD.MatlKList.Add(rMatlK);
    } 

    if(m_PSCBeamPostD.PElemKList.GetSize() < 1) return FALSE;

    //Stage
    _CRC_EXPD PSCBeamStagD;
    _CRC_EXPD ExtElemStagD;
    for(int i=0; i<StagKList.GetSize(); i++)
    {
        PSCBeamStagD.Initialize();
        PSCSectKMap.RemoveAll();
        PSCMatlKMap.RemoveAll();
        ExtElemStagD.Initialize();
        ExtSectKMap.RemoveAll();
        ExtMatlKMap.RemoveAll();

        StagK = StagKList.GetAt(i);
        m_pDoc->m_pStagCtrl->SetCurStagWithoutUpdateNotify(StagK);

        if(!m_pDoc->m_pAttrCtrl->GetStag(StagK, PSCBeamStagD.StagD)) continue;

        ElemKList.RemoveAll();
        m_pDoc->m_pAttrCtrl->GetElemKeyList(ElemKList);
        for(int j=0; j<ElemKList.GetSize(); j++)
        {
            ElemD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetElem(ElemKList[j], ElemD)) continue; 
            if(!m_pDoc->m_pAttrCtrl->IsBeam(ElemD.eltyp) && !m_pDoc->m_pAttrCtrl->IsTruss(ElemD.eltyp)) continue;

            //MatlK는 Attribute Control에있는 요소의 MatlK를 사용
            MatlK = ElemD.elmat;
            MatdD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(MatlK, MatdD)) continue;
            if(MatdD.Type != _T("C") && MatdD.Type != _T("SRC")) continue;

            //SectK는 Post Control에있는 요소의 SectK를 사용 
            //(변단면 그룹을 사용한경우에는 PostControl을 사용해야함)
            PElemD.Initialize();
            if(!m_pDoc->m_pPostCtrl->GetElemPost(ElemKList[j], PElemD)) continue;
            SectK = PElemD.elpro;
            SectD.Initialize();
            if(!m_pDoc->m_pPostCtrl->GetSectPost(SectK, SectD)) continue;

            //파형강판을 사용한 PSCBeam인 경우만 SRC허용
            if(MatdD.Type == _T("SRC") && SectD.SectBefore.Shape != D_SECT_SHAPE_PSC_CMPWEB) continue;

            BOOL bPSCBeam = TRUE;
            if(m_pDoc->m_pAttrCtrl->IsTruss(ElemD.eltyp)) bPSCBeam = FALSE;
            //if(bPSCBeam && m_pDoc->m_pAttrCtrl->GetMemberType(ElemKList[j]) != D_MBTP_BEAM) bPSCBeam = FALSE;
            if(bPSCBeam && !CDBLib::IsPscDgnSect(SectD)) bPSCBeam = FALSE;
            T_ELEM_K rElemK = 0;
            if(!PSCBeamKMap.Lookup(ElemKList[j], rElemK)) bPSCBeam = FALSE;

            if(bPSCBeam)
            {
                // Change by ZINU.('04.05.05). Check PSCBeamOK at Last Stage.
                BOOL bPSCBeamOK=TRUE;
                if(PscdD.iDgnCode==JSCE02)
                {
                    if(MatdD.Type == _T("C"))
                    {
                        if(MatdD.Data1.CodeName!=MATLCODE_CON_JIS_CIVIL)	bPSCBeamOK = FALSE;
                        if(MatdD.Rebar_CodeName!=MATLCODE_CON_JIS_CIVIL)	bPSCBeamOK = FALSE;
                    }
                    else if(MatdD.Type == _T("SRC"))
                    {
                        if(MatdD.Data1.CodeName!=MATLCODE_STL_JIS_CIVIL)	bPSCBeamOK = FALSE;
                        if(MatdD.Data2.CodeName!=MATLCODE_CON_JIS_CIVIL)	bPSCBeamOK = FALSE;
                        if(MatdD.Rebar_CodeName!=MATLCODE_CON_JIS_CIVIL)	bPSCBeamOK = FALSE;
                    }
                }
                else if(PscdD.iDgnCode==KSCE_USD03      || PscdD.iDgnCode==KSCE_USD05      || PscdD.iDgnCode==KSCE_USD10 ||  
                    PscdD.iDgnCode==KSCE_RAIL_USD04 || PscdD.iDgnCode==KSCE_RAIL_USD11 || 
                    PscdD.iDgnCode==JTG_D62_04      || PscdD.iDgnCode==CJJ_11_2011)
                {
                    bPSCBeamOK = TRUE;
                }
                else	ASSERT(0);

                if(bPSCBeamOK)
                {
                    PSCBeamStagD.PElemKList.Add(ElemKList[j]);
                    PSCBeamStagD.PSectKMap.SetAt(ElemKList[j],SectK);
                    PSCBeamStagD.MatlKMap.SetAt(ElemKList[j],MatlK);
                    PSCSectKMap.SetAt(SectK, SectK);
                    PSCMatlKMap.SetAt(MatlK, MatlK);
                }
            }
            else
            {
                ExtElemStagD.PElemKList.Add(ElemKList[j]);
                ExtElemStagD.PSectKMap.SetAt(ElemKList[j],SectK);
                ExtElemStagD.MatlKMap.SetAt(ElemKList[j],MatlK);
                ExtSectKMap.SetAt(SectK, SectK);
                ExtMatlKMap.SetAt(MatlK, MatlK);
            }
        }

        CArray<T_SGLD_K, T_SGLD_K> SgldKList; SgldKList.RemoveAll();
        m_pDoc->m_pPostCtrl->GetStageInfo()->GetSgldKeyListPost(SgldKList);
        for(int j=0; j<SgldKList.GetSize(); j++)
        {
            T_SGLD_D SgldD; SgldD.Initialize();
            if(!m_pDoc->m_pPostCtrl->GetStageInfo()->GetSgldPost(SgldKList[j], SgldD)) continue;
            PSCBeamStagD.SgldKList.Add(SgldKList[j]);
            PSCBeamStagD.SgldDList.Add(SgldD);
        }

        POSITION Pos = PSCSectKMap.GetStartPosition();
        while(Pos)
        {
            rSectK = 0;
            PSCSectKMap.GetNextAssoc(Pos, SectK, rSectK);
            PSCBeamStagD.PSectKList.Add(rSectK);
        } 

        Pos = PSCMatlKMap.GetStartPosition();
        while(Pos)
        {
            rMatlK = 0;
            PSCMatlKMap.GetNextAssoc(Pos, MatlK, rMatlK);
            PSCBeamStagD.MatlKList.Add(rMatlK);
        } 

        Pos = ExtSectKMap.GetStartPosition();
        while(Pos)
        {
            rSectK = 0;
            ExtSectKMap.GetNextAssoc(Pos, SectK, rSectK);
            ExtElemStagD.PSectKList.Add(rSectK);
        } 

        Pos = ExtMatlKMap.GetStartPosition();
        while(Pos)
        {
            rMatlK = 0;
            ExtMatlKMap.GetNextAssoc(Pos, MatlK, rMatlK);
            ExtElemStagD.MatlKList.Add(rMatlK);
        } 

        m_StagKList.Add(StagK);
        m_PSCBeamStagDMap.SetAt(StagK, PSCBeamStagD);
        m_ExtElemStagDMap.SetAt(StagK, ExtElemStagD);

        if(StagK == FinalStagK) break;  
    }

    m_bStag = (m_StagKList.GetSize() > 0 && m_pDoc->m_pPostCtrl->StageAnalysis() ? TRUE : FALSE);
    if(!m_bStag)
    {
        CString strMsg = _T("");
        strMsg.Format(_T("%s%s"), _LS(IDS_RCS_NO_STAG_GOOD), _LS(IDS_RCS_DGN));
        GDisplayAnalString(strMsg);
        return FALSE;
    }

    //Base Mode
    m_pDoc->m_pStagCtrl->SetCurStagWithoutUpdateNotify(0);

    PSCSectKMap.RemoveAll();
    PSCMatlKMap.RemoveAll();
    ExtSectKMap.RemoveAll();
    ExtMatlKMap.RemoveAll();

    m_PSCBeamBaseD.Initialize();
    m_ExtElemBaseD.Initialize();
    ElemKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetElemKeyList(ElemKList);
    for(int j=0; j<ElemKList.GetSize(); j++)
    {
        ElemD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetElem(ElemKList[j], ElemD)) continue; 
        if(!m_pDoc->m_pAttrCtrl->IsBeam(ElemD.eltyp) && !m_pDoc->m_pAttrCtrl->IsTruss(ElemD.eltyp)) continue; //Beam이 아닌 경우 리턴 

        //MatlK는 Attribute Control에있는 요소의 MatlK를 사용
        MatlK = ElemD.elmat;
        MatdD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(MatlK, MatdD)) continue;
        if(MatdD.Type != _T("C") && MatdD.Type != _T("SRC")) continue;

        //SectK는 Post Control에있는 요소의 SectK를 사용 
        //(변단면 그룹을 사용한경우에는 PostControl을 사용해야함)
        PElemD.Initialize();
        if(!m_pDoc->m_pPostCtrl->GetElemPost(ElemKList[j], PElemD)) continue;
        SectK = PElemD.elpro;
        SectD.Initialize();
        if(!m_pDoc->m_pPostCtrl->GetSectPost(SectK, SectD)) continue;

        //파형강판을 사용한 PSCBeam인 경우만 SRC허용
        if(MatdD.Type == _T("SRC") && SectD.SectBefore.Shape != D_SECT_SHAPE_PSC_CMPWEB) continue;

        BOOL bPSCBeam = TRUE;
        if(m_pDoc->m_pAttrCtrl->IsTruss(ElemD.eltyp)) bPSCBeam = FALSE;
        //if(bPSCBeam && m_pDoc->m_pAttrCtrl->GetMemberType(ElemKList[j]) != D_MBTP_BEAM) bPSCBeam = FALSE;
        if(bPSCBeam && !CDBLib::IsPscDgnSect(SectD)) bPSCBeam = FALSE;
        T_ELEM_K rElemK = 0;
        if(!PSCBeamKMap.Lookup(ElemKList[j], rElemK)) bPSCBeam = FALSE;

        if(bPSCBeam)
        {
            BOOL bPSCBeamOK=TRUE;
            if(PscdD.iDgnCode==JSCE02)
            {
                if(MatdD.Type == _T("C"))
                {
                    if(MatdD.Data1.CodeName!=MATLCODE_CON_JIS_CIVIL)	bPSCBeamOK = FALSE;
                    if(MatdD.Rebar_CodeName!=MATLCODE_CON_JIS_CIVIL)	bPSCBeamOK = FALSE;
                }
                else if(MatdD.Type == _T("SRC"))
                {
                    if(MatdD.Data1.CodeName!=MATLCODE_STL_JIS_CIVIL)	bPSCBeamOK = FALSE;
                    if(MatdD.Data2.CodeName!=MATLCODE_CON_JIS_CIVIL)	bPSCBeamOK = FALSE;
                    if(MatdD.Rebar_CodeName!=MATLCODE_CON_JIS_CIVIL)	bPSCBeamOK = FALSE;
                }
            }
            else if(PscdD.iDgnCode==KSCE_USD03 || PscdD.iDgnCode==JTG_D62_04|| PscdD.iDgnCode==CJJ_11_2011 || PscdD.iDgnCode==KSCE_USD03)
            {
                bPSCBeamOK = TRUE;
            }
            else	ASSERT(0);

            if(bPSCBeamOK)
            {
                m_PSCBeamBaseD.PElemKList.Add(ElemKList[j]);
                m_PSCBeamBaseD.PSectKMap.SetAt(ElemKList[j],SectK);
                m_PSCBeamBaseD.MatlKMap.SetAt(ElemKList[j],MatlK);
                PSCSectKMap.SetAt(SectK, SectK);
                PSCMatlKMap.SetAt(MatlK, MatlK);
            }
        }
        else
        {
            m_ExtElemBaseD.PElemKList.Add(ElemKList[j]);
            m_ExtElemBaseD.PSectKMap.SetAt(ElemKList[j],SectK);
            m_ExtElemBaseD.MatlKMap.SetAt(ElemKList[j],MatlK);
            ExtSectKMap.SetAt(SectK, SectK);
            ExtMatlKMap.SetAt(MatlK, MatlK);
        }
    }

    Pos = PSCSectKMap.GetStartPosition();
    while(Pos)
    {
        rSectK = 0;
        PSCSectKMap.GetNextAssoc(Pos, SectK, rSectK);
        m_PSCBeamBaseD.PSectKList.Add(rSectK);
    } 

    Pos = PSCMatlKMap.GetStartPosition();
    while(Pos)
    {
        rMatlK = 0;
        PSCMatlKMap.GetNextAssoc(Pos, MatlK, rMatlK);
        m_PSCBeamBaseD.MatlKList.Add(rMatlK);
    } 

    Pos = ExtSectKMap.GetStartPosition();
    while(Pos)
    {
        rSectK = 0;
        ExtSectKMap.GetNextAssoc(Pos, SectK, rSectK);
        m_ExtElemBaseD.PSectKList.Add(rSectK);
    } 

    Pos = ExtMatlKMap.GetStartPosition();
    while(Pos)
    {
        rMatlK = 0;
        ExtMatlKMap.GetNextAssoc(Pos, MatlK, rMatlK);
        m_ExtElemBaseD.MatlKList.Add(rMatlK);
    } 

    if(m_PSCBeamBaseD.PElemKList.GetSize() < 1) return FALSE;
    // Calculate Tdgr Data.
    if(!m_DataCtrl.Cal_TdgrDataForDesignAll())	return FALSE;
    // Add Jaeoh. ('05.08.25).
    CCRCForceCtrl* pForceCtrl = m_DataCtrl.Get_ForceCtrlPointer();
    //pForceCtrl->Set_LcomDataForDesign();  
    m_DataCtrl.Initialize();
	ArrElemPairKey aElemPairKey;
    m_DataCtrl.ConvertElemKeyToElemPairKey(m_PSCBeamBaseD.PElemKList, aElemPairKey);
    if(!m_DataCtrl.Cal_LcomDataForDesignAll(aElemPairKey)) return FALSE;
    //if(!m_DataCtrl.Cal_LcomDataForDesignAll()) return FALSE;

    return TRUE;
}

void CCRCExportFile::MakeDataSize()
{
    //Change Post CS to Base. rsh 2005.05.17(Request By Kono)
    CArray<T_NODE_K, T_NODE_K> NodeKList; NodeKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetNodeKeyList(NodeKList);
    m_DataNum.iNodeNum = NodeKList.GetSize();

    // Change by Jaeoh. 2005.10.31. (Request By Kono). 
    CArray<T_ELEM_K, T_ELEM_K> ElemKList;
    ElemKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetElemKeyList(ElemKList);
    //m_DataNum.iElemNum = m_PSCBeamBaseD.PElemKList.GetSize()+m_ExtElemBaseD.PElemKList.GetSize();
    m_DataNum.iElemNum = ElemKList.GetSize();  

    /*
    CArray<T_NODE_K, T_NODE_K> NodeKList; NodeKList.RemoveAll();
    m_pDoc->m_pPostCtrl->GetNodeKeyListPost(NodeKList);
    m_DataNum.iNodeNum = NodeKList.GetSize();
    m_DataNum.iElemNum = m_PSCBeamBaseD.PElemKList.GetSize()+m_ExtElemBaseD.PElemKList.GetSize();
    */
    //그룹수
    if(m_bStag) m_DataNum.iStagNum = m_StagKList.GetSize();
    else        m_DataNum.iStagNum = 0;
    Write_GrupData(FALSE);

    //재료 수
    m_DataNum.iMatNum = m_PSCBeamBaseD.MatlKList.GetSize();
    CMap<UINT, UINT, UINT, UINT> PSCMatlKMap; PSCMatlKMap.RemoveAll();
    for(int i=0; i<m_PSCBeamBaseD.MatlKList.GetSize(); i++)
    {
        PSCMatlKMap.SetAt(m_PSCBeamBaseD.MatlKList.GetAt(i),m_PSCBeamBaseD.MatlKList.GetAt(i));
    }

    for(int i=0; i<m_ExtElemBaseD.MatlKList.GetSize(); i++)
    {
        T_MATL_K rMatlK = 0;
        T_MATL_K MatlK=m_ExtElemBaseD.MatlKList.GetAt(i);
        if(PSCMatlKMap.Lookup(MatlK, rMatlK) != 0) continue;
        m_DataNum.iMatNum++;
    }

    for(int i=0; i<m_PSCBeamBaseD.MatlKList.GetSize(); i++)
    {
        T_MATD_D MatdD; MatdD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(m_PSCBeamBaseD.MatlKList[i], MatdD)) continue;

        if(m_DataCtrl.Get_SrcMatd(MatdD)) m_DataNum.iStlMatNum++;
    }

    CArray<T_TDNT_K, T_TDNT_K> aTdntK; aTdntK.RemoveAll();
    T_TDNT_D TdntD;
    m_pDoc->m_pAttrCtrl->GetTdntKeyList(aTdntK);
    m_DataNum.iTndnMatNum = aTdntK.GetSize();

    //국소좌표계 수
    CArray<T_SKEW_K, T_SKEW_K> SkewKList; SkewKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetSkewKeyList(SkewKList);
    m_DataNum.iSkewNum = SkewKList.GetSize();

    m_DataNum.iTdnaNum = m_DataCtrl.m_amTdnaK.GetCount();

    int iMaxTPntNum = 0;
    CArray<T_TDNA_K, T_TDNA_K> TdnaKList; TdnaKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetTdnaKeyList(TdnaKList);
    for(int i=0; i<TdnaKList.GetSize(); i++)
    {
        T_TDNA_K TdnaK = TdnaKList.GetAt(i);
        T_TDNA_D TdnaD; TdnaD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetTdna(TdnaK, TdnaD)) continue;

        if(TdnaD.nInputType==0) //2D
        {
            if(iMaxTPntNum < max(TdnaD.aProfileY.GetSize(), TdnaD.aProfileZ.GetSize()))
                iMaxTPntNum = max(TdnaD.aProfileY.GetSize(), TdnaD.aProfileZ.GetSize());
        }
        else
        {
            if(iMaxTPntNum < TdnaD.aProfile.GetSize()) iMaxTPntNum = TdnaD.aProfile.GetSize();
        }
    }
    m_DataNum.iTPntNum = iMaxTPntNum;


    CArray<T_SPSC_COOR,T_SPSC_COOR> arOuter;
    CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*> arInner;

    CMap<UINT, UINT, UINT, UINT> PSCSectKMap; PSCSectKMap.RemoveAll();
    for(int i=0; i<m_PSCBeamBaseD.PSectKList.GetSize(); i++)
    {
        PSCSectKMap.SetAt(m_PSCBeamBaseD.PSectKList.GetAt(i),m_PSCBeamBaseD.PSectKList.GetAt(i));
    }

    m_DataNum.iSectNum=m_PSCBeamBaseD.PSectKList.GetSize();
    for(int i=0; i<m_ExtElemBaseD.PSectKList.GetSize(); i++)
    {
        T_SECT_K SectK=0;
        if(PSCSectKMap.Lookup(m_ExtElemBaseD.PSectKList.GetAt(i), SectK)!=0) continue;
        m_DataNum.iSectNum++;
    }

    for(int i=0; i<m_PSCBeamBaseD.PSectKList.GetSize(); i++)
    {
        T_SECT_K SectK = m_PSCBeamBaseD.PSectKList[i];
        T_SECT_K OrgSectK = m_pDoc->m_pPostCtrl->GetOrgSectKeyPost(SectK);

        for(int j=0; j<2; j++)
        {
            int iMaxBar = 0;
            int iMaxCell = 0; //외측
            int iOutPontNum = 0;
            int iInPoint = 0;

            BOOL bPosI = (j==0 ? TRUE: FALSE);

            //교축방향 최대철근갯수
            T_RPSC_D RpscD; RpscD.Initialize();
            // Change by Jaeoh. ('06.03.20) TRUE -> FALSE
            m_pDoc->m_pAttrCtrl->GetRpscByConsiderRbar(FALSE,OrgSectK,RpscD);
            iMaxBar = RpscD.Mbar[j].GetSize();

            //최대포인트 갯수, 셀갯수
            BOOL bGetOK = CSectUtil::GetPscCoordList(TRUE, SectK,bPosI,arOuter,arInner);
            if(bGetOK)
            {
                for(int k=0; k<arOuter.GetSize(); k++)
                {
                    iMaxCell++;
                    T_SPSC_COOR OutCell = arOuter.GetAt(k);
                    iOutPontNum += OutCell.arSpscPont[0].GetSize()+1;
                }
                for(int k=0; k<arInner.GetSize(); k++)
                {
                    T_SPSC_COOR_AR* apSpscCoor = arInner[k];
                    for(int l=0; l<apSpscCoor->GetSize(); l++)
                    {
                        iMaxCell++;
                        T_SPSC_COOR InCell = apSpscCoor->GetAt(l);
                        iInPoint += InCell.arSpscPont[0].GetSize()+1;
                    }
                }
            }

            // Delete because of New Operator.
            for(int k=0; k<arInner.GetSize(); k++)
            {
                if(arInner[k])
                {
                    arInner[k]->RemoveAll();
                    delete arInner[k];
                    arInner[k] = NULL; 
                }
            }
            arInner.RemoveAll();

            if(iMaxCell > m_DataNum.iCellNum)       m_DataNum.iCellNum    = iMaxCell;
            if(iOutPontNum > m_DataNum.iOutPontNum) m_DataNum.iOutPontNum = iOutPontNum;
            if(iInPoint > m_DataNum.iInPontNum)     m_DataNum.iInPontNum  = iInPoint;
            if(iMaxBar > m_DataNum.iBarLayNum)      m_DataNum.iBarLayNum  = iMaxBar;
        }
    }

    //격자 데이터
    CGridInfo* pGridInfo = m_pDoc->m_pPostCtrl->GetGridInfo();

    CArray<T_GRIDINFO_GIRDER_D, T_GRIDINFO_GIRDER_D&>* pMainGriderList;
    CArray<T_GRIDINFO_GIRDER_D, T_GRIDINFO_GIRDER_D&>* pCrossBeamList;
    CArray<T_NODE_K, T_NODE_K>* pNodeKList;
    pMainGriderList = pGridInfo->GetMainGirder();
    pCrossBeamList = pGridInfo->GetCrossBeam();
    pNodeKList = pGridInfo->GetInflCrossPoint();

    m_DataNum.iMdgrNum=pMainGriderList->GetSize();
    m_DataNum.iCbemNum=pCrossBeamList->GetSize();
    m_DataNum.iCpotNum=pNodeKList->GetSize();

    CArray<T_LDLN_K, T_LDLN_K> LdlnKList; LdlnKList.GetSize();
    m_pDoc->m_pAttrCtrl->GetLdlnKeyList(LdlnKList);
    m_DataNum.iLdlnNum=LdlnKList.GetSize();

    //하중조건 수
    int iLcaseNum = 0;//(동시성를 고려하지않는 하중조건수)
    int iCLcaseNum = 0;//(동시성를 고려한 하중조건수)

    //Static Force
    CArray<T_STLD_K, T_STLD_K> StldKList;
    StldKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetStldKeyList(StldKList);
    iLcaseNum += StldKList.GetSize();

    //Moving Load Force
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

    iCLcaseNum += TotalMvldKList.GetSize();

    //Settlement Load Force
    CArray<T_SMLC_K, T_SMLC_K> SmlcKList;
    SmlcKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetSmlcKeyList(SmlcKList);
    iCLcaseNum += SmlcKList.GetSize();

    //Response Spectrum
    CArray<T_SPLC_K, T_SPLC_K> SplcKList;
    SplcKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetSplcKeyList(SplcKList);
    iLcaseNum += SplcKList.GetSize();

    //Time History
    CArray<T_THIS_K, T_THIS_K> ThisKList;
    ThisKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetThisKeyList(ThisKList);
    iLcaseNum += ThisKList.GetSize();

    //Grid Model
    CArray<T_GILC_K, T_GILC_K> GilcKList;
    GilcKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetGilcKeyList(GilcKList);

    for(int i=0; i<GilcKList.GetSize(); i++)
    {
        T_GILC_K GilcK = GilcKList.GetAt(i);
        T_GILD_D GildD; GildD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetGild(GilcK, GildD)) continue;

        if(GildD.nType == 0 || GildD.nType == 1)  iLcaseNum++;
        else                                      iCLcaseNum++;
    }

    m_DataNum.iLcaseNum = iLcaseNum;
    m_DataNum.iCLcaseNum = iCLcaseNum;
    m_DataNum.iSLcaseNum = 10;

    // 설계용 하중조합개수
    CCRCForceCtrl ForceCtrl;
    ForceCtrl.Set_DataCtrl(&m_DataCtrl);
    ForceCtrl.Set_LcomDataForDesign();
    m_DataNum.iLcomNumDgn = ForceCtrl.Get_LcomDataCount();

    m_DataNum.iLcomNum = m_pDoc->m_pAttrCtrl->GetCountLcom(D_LCOMTYPE_CONCRETE);

    //영향선 하중데이터 수
    Write_MvldData(FALSE);

    //요소, 경계조건, 하중그룹 전체스테이지내 최대 입력수
    m_DataNum.iStagElemNum = 0;
    m_DataNum.iStagBndrNum = 0;
    m_DataNum.iStagLoadNum = 0;
    for(int i=0; i<m_StagKList.GetSize(); i++)
    {
        T_STAG_K StagK = m_StagKList.GetAt(i);
        T_STAG_D StagD; StagD.Initialize();
        _CRC_EXPD PSCBeamStagD;
        PSCBeamStagD.Initialize();
        if(m_PSCBeamStagDMap.Lookup(StagK, PSCBeamStagD) == 0) continue;

        if(m_DataNum.iStagElemNum < PSCBeamStagD.StagD.aActiveElem.GetSize()) 
            m_DataNum.iStagElemNum = PSCBeamStagD.StagD.aActiveElem.GetSize(); 
        if(m_DataNum.iStagElemNum < PSCBeamStagD.StagD.aDeactiveElem.GetSize()) 
            m_DataNum.iStagElemNum = PSCBeamStagD.StagD.aDeactiveElem.GetSize(); 

        if(m_DataNum.iStagBndrNum < PSCBeamStagD.StagD.aActiveBndr.GetSize()) 
            m_DataNum.iStagBndrNum = PSCBeamStagD.StagD.aActiveBndr.GetSize(); 
        if(m_DataNum.iStagBndrNum < PSCBeamStagD.StagD.aDeactiveBndr.GetSize()) 
            m_DataNum.iStagBndrNum = PSCBeamStagD.StagD.aDeactiveBndr.GetSize(); 

        if(m_DataNum.iStagLoadNum < PSCBeamStagD.StagD.aActiveLoad.GetSize()) 
            m_DataNum.iStagLoadNum = PSCBeamStagD.StagD.aActiveLoad.GetSize(); 
        if(m_DataNum.iStagLoadNum < PSCBeamStagD.StagD.aDeactiveLoad.GetSize()) 
            m_DataNum.iStagLoadNum = PSCBeamStagD.StagD.aDeactiveLoad.GetSize(); 
    }


    //Beam Section Temperature의 1차력의 수
    m_DataNum.iEBtmpNum = 0;
    CMap<T_ELEM_K,T_ELEM_K, T_ELEM_K, T_ELEM_K> PostElemMap; PostElemMap.RemoveAll();
    for(int iElem=0; iElem<m_PSCBeamPostD.PElemKList.GetSize(); iElem++)
    {
        T_ELEM_K ElemK = m_PSCBeamPostD.PElemKList.GetAt(iElem);
        PostElemMap.SetAt(ElemK,ElemK);
    }

    T_LCOM_D LcasD;
    T_BTMP_K BtmpK; T_BTMP_D BtmpD;
    POSITION pos = m_pDoc->m_pAttrCtrl->GetStartBtmp();
    while (pos) 
    {
        BtmpD.Initialize();
        m_pDoc->m_pAttrCtrl->GetNextBtmp(pos, BtmpK, BtmpD);

        UINT ElemK = BtmpK.key.entity;
        UINT rElemK = 0;
        if(!PostElemMap.Lookup(ElemK, rElemK)) continue;
        m_DataNum.iEBtmpNum++;
    }

}

BOOL CCRCExportFile::Write_Data()
{
    if(!Write_Title())      return FALSE;
    if(!Write_Version())    return FALSE;
    if(!Write_Unit())       return FALSE;
    if(!Write_DataDefNum()) return FALSE;
    if(!Write_MatlData())   return FALSE;
    if(!Write_CpShData())   return FALSE;
    if(!Write_TdmeData())   return FALSE;
    if(!Write_SectData())   return FALSE;

    if(!Write_FinalLocalAxis())  return FALSE;
    if(!Write_FinalNodeData())   return FALSE;
    if(!Write_FinalElemData())   return FALSE;

    if(!Write_GrupData(TRUE))   return FALSE;
    if(!Write_StageData())			return FALSE;

    if(!Write_MvldData(TRUE))   return FALSE;
    if(!Write_SseiData())       return FALSE;
    if(!Write_TndnData())       return FALSE;
    if(!Write_ShearData())      return FALSE;
    if(!Write_StgForceData())   return FALSE;
    if(!Write_StgCpShData())    return FALSE;
    if(!Write_FinalStagData())  return FALSE;

    return TRUE;
}

BOOL CCRCExportFile::Write_Title()
{
    T_PJCF_D PjcfD; PjcfD.Initialize();
    if (!m_pDoc->m_pAttrCtrl->GetPjcf(PjcfD)) return TRUE;

    Write_Command(_T("G_TTL"));
    Write_Project(PjcfD.strProject);
    Write_Company(PjcfD.strUser);

    return TRUE;
}

BOOL CCRCExportFile::Write_Version()
{
    int iMaj = ((D_PRODUCT_VERSION_CODE & 0xff000000) >> 24);
    int iMin = ((D_PRODUCT_VERSION_CODE & 0x00ff0000) >> 16);
    int iRev = ((D_PRODUCT_VERSION_CODE & 0x0000ff00) >> 8);
    int iRel = D_RELEASE_NO;

    Write_Command(_T("G_VER"));
    Write_Int(iMaj);
    Write_Int(iMin);
    Write_Int(iRev);
    Write_Int(iRel);

    return TRUE;
}

BOOL CCRCExportFile::Write_Unit()
{
    T_UNIT_INDEX UnitIndex;
    CString aForce[] = { _T("KGF"), _T("TONF"), _T("N"), _T("KN"), _T("LBF"), _T("KIPS") };
    CString aLength[] = { _T("MM"), _T("CM"), _T("M"), _T("IN"), _T("FT") };

    m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
    CString strFor = aForce[UnitIndex.nBase_Force];
    CString strLen = aLength[UnitIndex.nBase_Length];
    CString strDeg = (m_bDeg ? _T("DEG") : _T("RAD"));

    Write_Command(_T("G_UNIT"));
    Write_String(strLen);
    Write_String(strFor);
    Write_String(strDeg);

    return TRUE;
}

BOOL CCRCExportFile::Write_DataDefNum()
{
    Write_Command(_T("G_MAX"));
    Write_Int(m_DataNum.iNodeNum);
    Write_Int(m_DataNum.iElemNum);
    Write_Int(m_DataNum.iStagNum);
    Write_Int(m_DataNum.iMatNum);
    Write_Int(m_DataNum.iTndnMatNum);
    Write_Int(m_DataNum.iSkewNum);

    Write_Int(m_DataNum.iTdnaNum);
    Write_Int(m_DataNum.iTPntNum);
    Write_Int(m_DataNum.iSectNum);
    Write_Int(m_DataNum.iBarLayNum);
    Write_Int(m_DataNum.iCellNum);
    Write_Int(m_DataNum.iOutPontNum);
    Write_Int(m_DataNum.iInPontNum);

    Write_Int(m_DataNum.iMdgrNum);
    Write_Int(m_DataNum.iLdlnNum);
    Write_Int(m_DataNum.iCbemNum);
    Write_Int(m_DataNum.iCpotNum);

    Write_Int(m_DataNum.iLcaseNum);
    Write_Int(m_DataNum.iCLcaseNum);
    Write_Int(m_DataNum.iSLcaseNum);
    Write_Int(m_DataNum.iLcomNumDgn);
    Write_Int(m_DataNum.iLcomNum);

    Write_Int(m_DataNum.iDeadNum);
    Write_Int(m_DataNum.iODeadNum);
    Write_Int(m_DataNum.iVhleNum);
    Write_Int(m_DataNum.iCrwdNum);
    Write_Int(m_DataNum.iLiveNum);
    Write_Int(m_DataNum.iP1ltNum);

    Write_Int(m_DataNum.iStlMatNum);

    Write_Command(_T("GR_MAX"));
    Write_Int(m_DataNum.iNodeGrNum);
    Write_Int(m_DataNum.iElemGrNum);
    Write_Int(m_DataNum.iTsecGrNum);
    Write_Int(m_DataNum.iConsGrNum);
    Write_Int(m_DataNum.iGsprGrNum);
    Write_Int(m_DataNum.iNsprGrNum);
    Write_Int(m_DataNum.iRigdGrNum);
    Write_Int(m_DataNum.iGlnkGrNum);
    Write_Int(m_DataNum.iElnkGrNum);
    Write_Int(m_DataNum.iCndlGrNum);
    Write_Int(m_DataNum.iBmldGrNum);
    Write_Int(m_DataNum.iPrstGrNum);
    Write_Int(m_DataNum.iEtmpGrNum);
    Write_Int(m_DataNum.iGtmpGrNum);
    Write_Int(m_DataNum.iBtmpGrNum);
    Write_Int(m_DataNum.iSdspGrNum);
    Write_Int(m_DataNum.iStagElemNum);
    Write_Int(m_DataNum.iStagBndrNum);
    Write_Int(m_DataNum.iStagLoadNum);

    Write_Int(m_DataNum.iPbldGrNum);
    Write_Int(m_DataNum.iPtnsGrNum);
    Write_Int(m_DataNum.iStmpGrNum);
    Write_Int(m_DataNum.iNtmpGrNum);
    Write_Int(m_DataNum.iSmptGrNum);

    Write_Int(m_DataNum.iEBtmpNum);
    Write_Int(m_DataNum.iSecfGrNum);
    Write_Int(m_DataNum.iCglpGrNum);

    return TRUE;
}

BOOL CCRCExportFile::Write_MatlData()
{
    for(int i=0; i<m_PSCBeamBaseD.MatlKList.GetSize(); i++)
    {
        T_MATL_D MatlD; MatlD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetMatl(m_PSCBeamBaseD.MatlKList[i], MatlD)) continue;

        T_MATD_D MatdD; MatdD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(m_PSCBeamBaseD.MatlKList[i], MatdD)) continue;

        if(MatlD.Type == _T("C") && m_DataCtrl.Get_ConMatd(MatdD))
        {
            Write_Command(_T("M_CON"));
            Write_Int(m_PSCBeamBaseD.MatlKList[i]);
            Write_String(MatlD.Name);
            Write_Double(MatdD.Data1.Design.C_fc);
            Write_Double(MatdD.Data1.Design.C_fci);
            Write_Double(MatlD.Data1.Analysis.Elast);
            double dGc = MatlD.Data1.Analysis.Elast/(2*(1+MatlD.Data1.Analysis.Poisson));
            Write_Double(dGc);
            Write_Double(MatlD.Data1.Analysis.Thermal);
            Write_Double(MatlD.Data1.Analysis.Density);

            if(!m_DataCtrl.Get_RbarMatl(TRUE, MatdD)) continue;
            Write_Command(_T("M_MBAR"));
            Write_Int(m_PSCBeamBaseD.MatlKList[i]);
            Write_String(MatdD.MainRebar_RebarName);
            Write_Double(MatdD.CRC.MbarMatl.dRebar_AFu);
            Write_Double(MatdD.MainRebarData.B_fy);
            Write_Double(MatdD.CRC.MbarMatl.dRebar_AFt_D);
            Write_Double(MatdD.CRC.MbarMatl.dRebar_AFt_T);
            Write_Double(MatdD.CRC.MbarMatl.dRebar_AFt_EQ);
            Write_Double(MatdD.CRC.MbarMatl.dRebar_Es);
            Write_Double(MatdD.CRC.MbarMatl.dRebar_AFv);

            if(!m_DataCtrl.Get_RbarMatl(FALSE, MatdD)) continue;
            Write_Command(_T("M_SBAR"));
            Write_Int(m_PSCBeamBaseD.MatlKList[i]);
            Write_String(MatdD.SubRebar_RebarName);
            Write_Double(MatdD.CRC.SbarMatl.dRebar_AFu);
            Write_Double(MatdD.SubRebarData.B_fy);
            Write_Double(MatdD.CRC.SbarMatl.dRebar_AFt_D);
            Write_Double(MatdD.CRC.SbarMatl.dRebar_AFt_T);
            Write_Double(MatdD.CRC.SbarMatl.dRebar_AFt_EQ);
            Write_Double(MatdD.CRC.SbarMatl.dRebar_Es);
            Write_Double(MatdD.CRC.SbarMatl.dRebar_AFv);
        }
        else if(MatlD.Type == _T("SRC") && m_DataCtrl.Get_SrcMatd(MatdD))
        {
            Write_Command(_T("M_CON"));
            Write_Int(m_PSCBeamBaseD.MatlKList[i]);
            Write_String(MatlD.Name);
            Write_Double(MatdD.Data2.Design.C_fc);
            Write_Double(MatdD.Data2.Design.C_fci);
            Write_Double(MatlD.Data2.Analysis.Elast);
            double dGc = MatlD.Data2.Analysis.Elast/(2*(1+MatlD.Data2.Analysis.Poisson));
            Write_Double(dGc);
            Write_Double(MatlD.Data2.Analysis.Thermal);
            Write_Double(MatlD.Data2.Analysis.Density);

            Write_Command(_T("M_STL"));
            Write_Int(m_PSCBeamBaseD.MatlKList[i]);
            Write_String(MatlD.Name);
            Write_Double(MatdD.Data1.Design.S_Fu);
            Write_Double(MatdD.Data1.Design.S_Aft);
            Write_Double(MatdD.Data1.Design.S_Afv);
            Write_Double(MatlD.Data1.Analysis.Elast); //Es

            if(!m_DataCtrl.Get_RbarMatl(TRUE, MatdD)) continue;
            Write_Command(_T("M_MBAR"));
            Write_Int(m_PSCBeamBaseD.MatlKList[i]);
            Write_String(MatdD.MainRebar_RebarName);
            Write_Double(MatdD.CRC.MbarMatl.dRebar_AFu);
            Write_Double(MatdD.MainRebarData.B_fy);
            Write_Double(MatdD.CRC.MbarMatl.dRebar_AFt_D);
            Write_Double(MatdD.CRC.MbarMatl.dRebar_AFt_T);
            Write_Double(MatdD.CRC.MbarMatl.dRebar_AFt_EQ);
            Write_Double(MatdD.CRC.MbarMatl.dRebar_Es);
            Write_Double(MatdD.CRC.MbarMatl.dRebar_AFv);

            if(!m_DataCtrl.Get_RbarMatl(FALSE, MatdD)) continue;
            Write_Command(_T("M_SBAR"));
            Write_Int(m_PSCBeamBaseD.MatlKList[i]);
            Write_String(MatdD.SubRebar_RebarName);
            Write_Double(MatdD.CRC.SbarMatl.dRebar_AFu);
            Write_Double(MatdD.SubRebarData.B_fy);
            Write_Double(MatdD.CRC.SbarMatl.dRebar_AFt_D);
            Write_Double(MatdD.CRC.SbarMatl.dRebar_AFt_T);
            Write_Double(MatdD.CRC.SbarMatl.dRebar_AFt_EQ);
            Write_Double(MatdD.CRC.SbarMatl.dRebar_Es);
            Write_Double(MatdD.CRC.SbarMatl.dRebar_AFv);
        }
        else continue;
    }

    CMap<UINT, UINT, UINT, UINT> PSCMatlKMap; PSCMatlKMap.RemoveAll();
    for(int i=0; i<m_PSCBeamBaseD.MatlKList.GetSize(); i++)
    {
        PSCMatlKMap.SetAt(m_PSCBeamBaseD.MatlKList.GetAt(i),m_PSCBeamBaseD.MatlKList.GetAt(i));
    }

    for(int i=0; i<m_ExtElemBaseD.MatlKList.GetSize(); i++)
    {
        T_MATL_K MatlK = 0;
        if(PSCMatlKMap.Lookup(m_ExtElemBaseD.MatlKList[i], MatlK) != 0) continue;

        T_MATL_D MatlD; MatlD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetMatl(m_ExtElemBaseD.MatlKList[i], MatlD)) continue;

        T_MATD_D MatdD; MatdD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(m_ExtElemBaseD.MatlKList[i], MatdD)) continue;

        if(!m_DataCtrl.Get_ConMatd(MatdD)) continue;

        Write_Command(_T("M_CON"));
        Write_Int(m_ExtElemBaseD.MatlKList[i]);
        Write_String(MatlD.Name);
        Write_Double(MatdD.Data1.Design.C_fc);
        Write_Double(MatdD.Data1.Design.C_fci);
        Write_Double(MatlD.Data1.Analysis.Elast);
        double dGc = MatlD.Data1.Analysis.Elast/(2*(1+MatlD.Data1.Analysis.Poisson));
        Write_Double(dGc);
        Write_Double(MatlD.Data1.Analysis.Thermal);
        Write_Double(MatlD.Data1.Analysis.Density);
    }

    CArray<T_TDNT_K, T_TDNT_K> aTdntK; aTdntK.RemoveAll();
    T_TDNT_D TdntD;
    m_pDoc->m_pAttrCtrl->GetTdntKeyList(aTdntK);

    for(int i=0; i<aTdntK.GetSize(); i++)
    {
        T_TDNT_D TdntD;
        TdntD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetTdnt(aTdntK[i],TdntD)) continue;
        T_MATL_D MatlD;
        MatlD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetMatl(TdntD.MatlKey,MatlD)) continue;

        double dRelaxCoef = 0.0;
        if(TdntD.nRelaxMethod == 0) //0:Method1(기존방법)
            dRelaxCoef = TdntD.dRelaxCoef;
        else if(TdntD.nRelaxMethod == 1) //1:Method2
            dRelaxCoef = TdntD.dRelaxRatio;
        else ASSERT(0);

        Write_Command(_T("M_TNDN"));
        Write_Int(aTdntK[i]);
        Write_String(TdntD.TendonTypeName);
        Write_Int(TdntD.nTendonType);
        Write_Int(TdntD.nLoadType);
        Write_Int(TdntD.nBondType);
        Write_Double(TdntD.dUltimateStrength);
        Write_Double(TdntD.dYieldStrength);
        Write_Double(TdntD.dTendonArea);
        Write_Double(TdntD.dDuctArea);//Dia
        Write_Int(TdntD.nRelaxMethod+1);
        Write_Double(dRelaxCoef);
        Write_Double(MatlD.Data1.Analysis.Elast);
        Write_Double(TdntD.dWobbleFactor);
        Write_Double(TdntD.dFrictionFactor);
        Write_Double(TdntD.dAnchoSlipBegin);
        Write_Double(TdntD.dAnchoSlipEnd);
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_CpShData()
{
    // Add by ZINU.('06.02.16). Check Bridge Direction.
    if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

    T_STCT_D StctD; StctD.Initialize();
    m_pDoc->m_pAttrCtrl->GetStct(StctD);
    if(!StctD.bIncludeTimeDependentEffect) return TRUE;

    CArray<T_TMAT_K, T_TMAT_K> TmatKList;
    TmatKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetTmatKeyList(TmatKList);

    for(int i=0; i<TmatKList.GetSize(); i++)
    {
        T_TMAT_K TmatK = TmatKList.GetAt(i);
        T_TMAT_D TmatD; TmatD.Initialize();
        if(m_pDoc->m_pAttrCtrl->GetTmat(TmatK, TmatD))
        {
            T_TDMT_D TdmtD; TdmtD.Initialize();
            if(m_pDoc->m_pAttrCtrl->GetTdmt(TmatD.TdMatlTypeKey, TdmtD))
            {
                if(TdmtD.COMMON.nCode == 8) //JAPAN Standard
                {
                    Write_Command(_T("M_JIS"));
                    Write_Int(TmatK);
                    Write_String(TdmtD.strMatlName);
                    Write_String(_T("JIS"));
                    Write_Double(TdmtD.COMMON.dCompStrength);
                    // Change by ZINU.('07.05.16). NO:XXXX, nCementType -> nRelativeHumidity.
                    if(TdmtD.JAPAN.nRelativeHumidity == 1) //Curing Underwater
                        Write_Double(100.0);
                    else if(TdmtD.JAPAN.nRelativeHumidity == 2) //Relative Humidity
                        Write_Double(TdmtD.COMMON.dRelativeHumidity);
                    // Modify, Jaeoh.('08.04.11). MQC-1756, nRelativeHumidity -> nCementType
                    Write_Int(TdmtD.JAPAN.nCementType);
                    Write_Double(TdmtD.COMMON.dConcreteAge);
                    Write_Double(TdmtD.JAPAN.dMemberSize);
                }
                else if(TdmtD.COMMON.nCode == 10) //JSCE
                {
                    Write_Command(_T("M_JSCE"));
                    Write_Int(TmatK);
                    Write_String(TdmtD.strMatlName);
                    Write_String(_T("JSCE"));
                    Write_Double(TdmtD.JPJSCE.dModulusOfElasticity);
                    Write_Double(TdmtD.JPJSCE.dRelativeHumidity);
                    Write_Double(TdmtD.JPJSCE.dVolumeSurfaceRatio);
                    Write_Double(TdmtD.JPJSCE.dCementContent);
                    Write_Double(TdmtD.JPJSCE.dWaterContent);
                    Write_Double(TdmtD.JPJSCE.dConcreteAge);
                }
                else
                {
                    CString strMsg = _T("");
                    strMsg.Format(_LS(IDS_DGN_WARN_TDMT_PCDESIGN),TdmtD.strMatlName);
                    GSaveHistoryFormatNF(strMsg);
                }
            }
        }
    }

    CArray<T_EDMP_K, T_EDMP_K> EdmpKList;
    EdmpKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetEdmpKeyList(EdmpKList);

    for(int i=0; i<EdmpKList.GetSize(); i++)
    {
        T_EDMP_K EdmpK = EdmpKList.GetAt(i); //EdmpK = ElemK
        T_EDMP_D EdmpD; EdmpD.Initialize();
        if(m_pDoc->m_pAttrCtrl->GetEdmp(EdmpK, EdmpD))
        {
            if(EdmpD.nType==0)
            {
                Write_Command(_T("E_JIS"));
                Write_Int(EdmpK);
                Write_Double(EdmpD.h); //Notational Size of Member
            }
            else if(EdmpD.nType==1)
            {
                Write_Command(_T("E_JSCE"));
                Write_Int(EdmpK);
                Write_Double(EdmpD.h); //Volume-Surface Ratio
            }
            else ASSERT(0);
        }
    }
    return TRUE;
}

BOOL CCRCExportFile::Write_TdmeData()
{
    // Add by ZINU.('06.02.16). Check Bridge Direction.
    if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

    T_STCT_D StctD; StctD.Initialize();
    m_pDoc->m_pAttrCtrl->GetStct(StctD);
    if(!StctD.bIncludeTimeDependentEffect) return TRUE;

    CArray<T_TMAT_K, T_TMAT_K> TmatKList;
    TmatKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetTmatKeyList(TmatKList);

    for(int i=0; i<TmatKList.GetSize(); i++)
    {
        T_TMAT_K TmatK = TmatKList.GetAt(i);
        T_TMAT_D TmatD; TmatD.Initialize();
        if(m_pDoc->m_pAttrCtrl->GetTmat(TmatK, TmatD))
        {
            T_TDME_D TdmeD; TdmeD.Initialize();
            if(m_pDoc->m_pAttrCtrl->GetTdme(TmatD.TdMatlElastKey, TdmeD))
            {
                Write_Command(_T("TM_DATA"));
                Write_Int(TmatK);
                Write_String(TdmeD.strCode);
                Write_Double(TdmeD.dCompStrength);
            }
        }
    }
    return TRUE;
}

BOOL CCRCExportFile::Write_SectData()
{
    for(int i=0; i<m_PSCBeamBaseD.PSectKList.GetSize(); i++)
    {
        T_SECT_K SectK = m_PSCBeamBaseD.PSectKList.GetAt(i);
        Write_Command(_T("S_SECK"));
        Write_Int(SectK);
        Write_Int(m_pDoc->m_pPostCtrl->GetOrgSectKeyPost(SectK));
    }

    T_SECT_D SectD;
    for(int i=0; i<m_PSCBeamBaseD.PSectKList.GetSize(); i++)
    {
        T_SECT_K SectK = m_PSCBeamBaseD.PSectKList.GetAt(i);
        SectD.Initialize();
        if(m_pDoc->m_pPostCtrl->GetSectPost(SectK, SectD))
        {
            Write_SectType(SectK, SectD);
            Write_SectSize(SectK, SectD, TRUE);
            Write_SectSize(SectK, SectD, FALSE);
            Write_SectCoord(SectK, SectD, TRUE);
            Write_SectCoord(SectK, SectD, FALSE);
        }
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_StageData()
{
    BOOL bOK = FALSE;
    if(!m_bStag) return TRUE;

    for(int i=0; i<m_StagKList.GetSize(); i++)
    {
        T_STAG_K StagK = m_StagKList.GetAt(i);
        T_STAG_D StagD; StagD.Initialize();
        _CRC_EXPD PSCBeamStagD;
        PSCBeamStagD.Initialize();
        if(m_PSCBeamStagDMap.Lookup(StagK, PSCBeamStagD) == 0) continue;

        // Change by Jaeoh('05.06.21) *.jpd 파일에 ISTG 값을 StageK -> nStageId로 수정 
        int iStageId = PSCBeamStagD.StagD.nStageId;

        bOK = Write_StagInfo(iStageId, PSCBeamStagD.StagD);
        if(bOK) bOK = Write_ActiveElemGrup(iStageId, PSCBeamStagD.StagD);
        if(bOK) bOK = Write_InactiveElemGrup(iStageId, PSCBeamStagD.StagD);
        if(bOK) bOK = Write_ActiveSprtGrup(iStageId, PSCBeamStagD.StagD);
        if(bOK) bOK = Write_InactiveSprtGrup(iStageId, PSCBeamStagD.StagD);
        if(bOK) bOK = Write_ActiveLoadGrup(iStageId, PSCBeamStagD.StagD);
        if(bOK) bOK = Write_InactiveLoadGrup(iStageId, PSCBeamStagD.StagD);

        if(!bOK) break;
    }

    return bOK;
}

BOOL CCRCExportFile::Write_MvldData(BOOL bWrite)
{
    // Add by ZINU.('06.02.16). Check Bridge Direction.
    if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

    CArray<T_GILC_K, T_GILC_K> GilcKList; GilcKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetGilcKeyList(GilcKList);

    //Dead Load
    m_DataNum.iDeadNum = 0;
    for(int i=0; i<GilcKList.GetSize(); i++)
    {
        T_GILD_K GildK = GilcKList.GetAt(i);
        T_GILD_D GildD; GildD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetGild(GildK, GildD)) continue;

        if(GildD.nType == 0)//Dead Load
        {
            for(int j=0; j<GildD.aData.GetSize(); j++)
            {
                T_GILD_BASE GildBase = GildD.aData.GetAt(j);

                //기준라인
                T_LDLN_D CLdlnD; CLdlnD.Initialize();
                if(!m_pDoc->m_pAttrCtrl->GetLdln(GildBase.CenterLineK, CLdlnD)) continue;
                CString strCLine = CLdlnD.Name;

                CString strLLine=_T(""), strRLine=_T("");
                //시작라인
                if(GildBase.bStartGirder)
                {
                    T_MGDR_D MgdrD; MgdrD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetMgdr(GildBase.StartGirderLineK, MgdrD)) continue;
                    strLLine = MgdrD.Name;
                }
                else
                {
                    T_LDLN_D LdlnD; LdlnD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetLdln(GildBase.StartGirderLineK, LdlnD)) continue;
                    strLLine = LdlnD.Name;
                }
                //끝라인
                if(GildBase.bEndGirder)
                {
                    T_MGDR_D MgdrD; MgdrD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetMgdr(GildBase.EndGirderLineK, MgdrD)) continue;
                    strRLine = MgdrD.Name;
                }
                else
                {
                    T_LDLN_D LdlnD; LdlnD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetLdln(GildBase.EndGirderLineK, LdlnD)) continue;
                    strRLine = LdlnD.Name;
                }

                //Cross 
                T_CBEM_D SCBemD; SCBemD.Initialize();
                if(!m_pDoc->m_pAttrCtrl->GetCbem(GildBase.StartCrossK, SCBemD)) continue;
                CString strSCrss=SCBemD.Name;
                T_CBEM_D ECBemD; ECBemD.Initialize();
                if(!m_pDoc->m_pAttrCtrl->GetCbem(GildBase.EndCrossK, ECBemD)) continue;
                CString strECrss=ECBemD.Name;

                if(bWrite)
                {
                    Write_Command(_T("L_DL"));
                    Write_Int(GildBase.nLoadType);// 0:Point, 1:Line, 2:Area
                    Write_String(strCLine);
                    Write_String(strLLine);
                    Write_String(strRLine);
                    Write_String(strSCrss);
                    Write_String(strECrss);
                    Write_Double(GildBase.dStartLoad);
                    Write_Double(GildBase.dEndLoad);
                }
                m_DataNum.iDeadNum++;
            }
        }
    }

    /*
    //Other Dead Load
    m_DataNum.iOdlNum = 0;
    for(i=0; i<GilcKList.GetSize(); i++)
    {
    T_GILD_K GildK = GilcKList.GetAt(i);
    T_GILD_D GildD; GildD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetGild(GildK, GildD)) continue;

    if(GildD.nType == 1 )//Other Dead Load
    {
    for(int j=0; j<GildD.aData.GetSize(); j++)
    {
    T_GILD_BASE GildBase = GildD.aData.GetAt(j);

    //기준라인
    T_LDLN_D CLdlnD; CLdlnD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetLdln(GildBase.CenterLineK, CLdlnD)) continue;
    CString strCLine = CLdlnD.Name;

    CString strLLine=_T(""), strRLine=_T("");
    //시작라인
    if(GildBase.bStartGirder)
    {
    T_MGDR_D MgdrD; MgdrD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetMgdr(GildBase.StartGirderLineK, MgdrD)) continue;
    strLLine = MgdrD.Name;
    }
    else
    {
    T_LDLN_D LdlnD; LdlnD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetLdln(GildBase.StartGirderLineK, LdlnD)) continue;
    strLLine = LdlnD.Name;
    }
    //끝라인
    if(GildBase.bEndGirder)
    {
    T_MGDR_D MgdrD; MgdrD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetMgdr(GildBase.EndGirderLineK, MgdrD)) continue;
    strRLine = MgdrD.Name;
    }
    else
    {
    T_LDLN_D LdlnD; LdlnD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetLdln(GildBase.EndGirderLineK, LdlnD)) continue;
    strRLine = LdlnD.Name;
    }

    //Cross 
    T_CBEM_D SCBemD; SCBemD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetCbem(GildBase.StartCrossK, SCBemD)) continue;
    CString strSCrss=SCBemD.Name;
    T_CBEM_D ECBemD; ECBemD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetCbem(GildBase.EndCrossK, ECBemD)) continue;
    CString strECrss=ECBemD.Name;

    if(bWrite)
    {
    Write_Command(_T("L_ODL"));
    Write_Int(GildBase.nLoadType);// 0:Point, 1:Line, 2:Area
    Write_String(strCLine);
    Write_String(strLLine);
    Write_String(strRLine);
    Write_String(strSCrss);
    Write_String(strECrss);
    Write_Double(GildBase.dStartLoad);
    Write_Double(GildBase.dEndLoad);
    }
    m_DataNum.iOdlNum++;
    }
    }
    }
    */

    //자동차하중
    m_DataNum.iVhleNum = 0;
    for(int i=0; i<GilcKList.GetSize(); i++)
    {
        T_GILD_K GildK = GilcKList.GetAt(i);
        T_GILD_D GildD; GildD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetGild(GildK, GildD)) continue;

        if(GildD.nType == 2)//Vehicle Live Load
        {
            for(int j=0; j<GildD.aData.GetSize(); j++)
            {
                T_GILD_BASE GildBase = GildD.aData.GetAt(j);

                //기준차도라인
                T_LDLN_D CLdlnD; CLdlnD.Initialize();
                if(!m_pDoc->m_pAttrCtrl->GetLdln(GildBase.CenterLineK, CLdlnD)) continue;
                CString strCLine = CLdlnD.Name;

                CString strLLine=_T(""), strRLine=_T("");
                //좌측 차도라인
                if(GildBase.bStartGirder)
                {
                    T_MGDR_D MgdrD; MgdrD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetMgdr(GildBase.StartGirderLineK, MgdrD)) continue;
                    strLLine = MgdrD.Name;
                }
                else
                {
                    T_LDLN_D LdlnD; LdlnD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetLdln(GildBase.StartGirderLineK, LdlnD)) continue;
                    strLLine = LdlnD.Name;
                }
                //우측 차도라인
                if(GildBase.bEndGirder)
                {
                    T_MGDR_D MgdrD; MgdrD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetMgdr(GildBase.EndGirderLineK, MgdrD)) continue;
                    strRLine = MgdrD.Name;
                }
                else
                {
                    T_LDLN_D LdlnD; LdlnD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetLdln(GildBase.EndGirderLineK, LdlnD)) continue;
                    strRLine = LdlnD.Name;
                }

                //중앙분리대
                CString strSMLine=_T(" ");
                T_LDLN_D SLdlnD; SLdlnD.Initialize();
                if(m_pDoc->m_pAttrCtrl->GetLdln(GildBase.StartMedialK, SLdlnD)) strSMLine = SLdlnD.Name;
                CString strEMLine=_T(" ");
                T_LDLN_D ELdlnD; ELdlnD.Initialize();
                if(m_pDoc->m_pAttrCtrl->GetLdln(GildBase.EndMedialK, ELdlnD)) strEMLine = ELdlnD.Name;

                //Cross 
                T_CBEM_D SCBemD; SCBemD.Initialize();
                if(!m_pDoc->m_pAttrCtrl->GetCbem(GildBase.StartCrossK, SCBemD)) continue;
                CString strSCrss=SCBemD.Name;
                T_CBEM_D ECBemD; ECBemD.Initialize();
                if(!m_pDoc->m_pAttrCtrl->GetCbem(GildBase.EndCrossK, ECBemD)) continue;
                CString strECrss=ECBemD.Name;

                if(bWrite)
                {
                    Write_Command(_T("L_VHL"));
                    Write_Int(GildD.nVehicleLoadType);
                    Write_String(strCLine);
                    Write_String(strLLine);
                    Write_String(strRLine);
                    Write_String(strSMLine);
                    Write_String(strEMLine);
                    Write_String(strSCrss);
                    Write_String(strECrss);
                    Write_Double(GildBase.dMainLoadingRange);
                }
                m_DataNum.iVhleNum++;
            }
        }
    }

    //군집하중
    m_DataNum.iCrwdNum = 0;
    for(int i=0; i<GilcKList.GetSize(); i++)
    {
        T_GILD_K GildK = GilcKList.GetAt(i);
        T_GILD_D GildD; GildD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetGild(GildK, GildD)) continue;

        if(GildD.nType == 3)//Crowded Live Load
        {
            for(int j=0; j<GildD.aData.GetSize(); j++)
            {
                T_GILD_BASE GildBase = GildD.aData.GetAt(j);

                //기준차도라인
                T_LDLN_D CLdlnD; CLdlnD.Initialize();
                if(!m_pDoc->m_pAttrCtrl->GetLdln(GildBase.CenterLineK, CLdlnD)) continue;
                CString strCLine = CLdlnD.Name;

                CString strLLine=_T(""), strRLine=_T("");
                //좌측 보도라인
                if(GildBase.bStartGirder)
                {
                    T_MGDR_D MgdrD; MgdrD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetMgdr(GildBase.StartGirderLineK, MgdrD)) continue;
                    strLLine = MgdrD.Name;
                }
                else
                {
                    T_LDLN_D LdlnD; LdlnD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetLdln(GildBase.StartGirderLineK, LdlnD)) continue;
                    strLLine = LdlnD.Name;
                }
                //우측 보도라인
                if(GildBase.bEndGirder)
                {
                    T_MGDR_D MgdrD; MgdrD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetMgdr(GildBase.EndGirderLineK, MgdrD)) continue;
                    strRLine = MgdrD.Name;
                }
                else
                {
                    T_LDLN_D LdlnD; LdlnD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetLdln(GildBase.EndGirderLineK, LdlnD)) continue;
                    strRLine = LdlnD.Name;
                }

                //Cross 
                T_CBEM_D SCBemD; SCBemD.Initialize();
                if(!m_pDoc->m_pAttrCtrl->GetCbem(GildBase.StartCrossK, SCBemD)) continue;
                CString strSCrss=SCBemD.Name;
                T_CBEM_D ECBemD; ECBemD.Initialize();
                if(!m_pDoc->m_pAttrCtrl->GetCbem(GildBase.EndCrossK, ECBemD)) continue;
                CString strECrss=ECBemD.Name;

                if(bWrite)
                {
                    Write_Command(_T("L_CRW"));
                    Write_Int(GildD.nCrowdedLoadType);
                    Write_String(strCLine);
                    Write_String(strLLine);
                    Write_String(strRLine);
                    Write_String(strSCrss);
                    Write_String(strECrss);
                }
                m_DataNum.iCrwdNum++;
            }
        }
    }

    //Other Live Load
    m_DataNum.iLiveNum = 0;
    for(int i=0; i<GilcKList.GetSize(); i++)
    {
        T_GILD_K GildK = GilcKList.GetAt(i);
        T_GILD_D GildD; GildD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetGild(GildK, GildD)) continue;

        if(GildD.nType == 4) //Other Live Load
        {
            for(int j=0; j<GildD.aData.GetSize(); j++)
            {
                T_GILD_BASE GildBase = GildD.aData.GetAt(j);

                //기준차도라인
                T_LDLN_D CLdlnD; CLdlnD.Initialize();
                if(!m_pDoc->m_pAttrCtrl->GetLdln(GildBase.CenterLineK, CLdlnD)) continue;
                CString strCLine = CLdlnD.Name;

                CString strLLine=_T(""), strRLine=_T("");
                //좌측 보도라인
                if(GildBase.bStartGirder)
                {
                    T_MGDR_D MgdrD; MgdrD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetMgdr(GildBase.StartGirderLineK, MgdrD)) continue;
                    strLLine = MgdrD.Name;
                }
                else
                {
                    T_LDLN_D LdlnD; LdlnD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetLdln(GildBase.StartGirderLineK, LdlnD)) continue;
                    strLLine = LdlnD.Name;
                }
                //우측 보도라인
                if(GildBase.bEndGirder)
                {
                    T_MGDR_D MgdrD; MgdrD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetMgdr(GildBase.EndGirderLineK, MgdrD)) continue;
                    strRLine = MgdrD.Name;
                }
                else
                {
                    T_LDLN_D LdlnD; LdlnD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetLdln(GildBase.EndGirderLineK, LdlnD)) continue;
                    strRLine = LdlnD.Name;
                }

                //Cross 
                T_CBEM_D SCBemD; SCBemD.Initialize();
                if(!m_pDoc->m_pAttrCtrl->GetCbem(GildBase.StartCrossK, SCBemD)) continue;
                CString strSCrss=SCBemD.Name;
                T_CBEM_D ECBemD; ECBemD.Initialize();
                if(!m_pDoc->m_pAttrCtrl->GetCbem(GildBase.EndCrossK, ECBemD)) continue;
                CString strECrss=ECBemD.Name;

                if(bWrite)
                {
                    Write_Command(_T("L_OLL"));
                    Write_String(strCLine);
                    Write_String(strLLine);
                    Write_String(strRLine);
                    Write_String(strSCrss);
                    Write_String(strECrss);
                    Write_Double(GildBase.dLoad);
                }
                m_DataNum.iLiveNum++;
            }
        }
    }

    //P1 하중
    m_DataNum.iP1ltNum=0;

    CArray<T_P1LT_K, T_P1LT_K> P1ltKList;
    P1ltKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetP1ltKeyList(P1ltKList);

    T_P1LT_D P1ltD;

    for(int i=0; i<P1ltKList.GetSize(); i++)
    {
        T_P1LT_K P1ltK = P1ltKList.GetAt(i);
        P1ltD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetP1lt(P1ltK, P1ltD)) continue;

        if(bWrite)
        {
            Write_Command(_T("L_P1T"));
            Write_Int(P1ltK);
            Write_String(P1ltD.Name);
            Write_Int(P1ltD.nLoadItem);// 0:Reaction, 1:Displacement, 2:Member Force
            for(int j=0; j<6; j++) // 0:Bending Memoment, 1:Shear Force
            {
                Write_Int(P1ltD.nLoadType[j]+1);
            }
        }
        m_DataNum.iP1ltNum++;
    }

    //P1하중의 Assign
    CArray<T_P1LA_K, T_P1LA_K> P1laKList;
    P1laKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetP1laKeyList(P1laKList);
    T_P1LA_D P1laD;

    for(int i=0; i<P1laKList.GetSize(); i++)
    {
        T_P1LA_K P1laK = P1laKList.GetAt(i);
        P1laD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetP1la(P1laK, P1laD)) continue;

        if(bWrite)
        {
            if(P1laD.nLoadItem == 0 || P1laD.nLoadItem == 1) //NodeK
            {
                Write_Command(_T("L_P1AN"));
                Write_Int(P1laD.NodeElemK);
            }
            else //ElemK
            {
                Write_Command(_T("L_P1AE"));
                Write_Int(P1laD.NodeElemK);
            }
            Write_Int(P1laD.TypeKey);
        }
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_SseiData()
{
    // Add by ZINU.('06.02.16). Check Bridge Direction.
    if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

    // SSEI
    T_SSEI_K SseiK; T_SSEI_D SseiD;
    POSITION pos = m_pDoc->m_pAttrCtrl->GetStartSsei();
    while(pos) 
    {
        SseiD.Initialize();
        m_pDoc->m_pAttrCtrl->GetNextSsei(pos, SseiK, SseiD);

        //SseiK = StldK
        T_STLD_D StldD; StldD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetStld(SseiK, StldD)) continue;

        double dAngle = 0.0;
        if(SseiD.nDirectionMtd == 0) //sPoint
        {
            double dP1x = SseiD.dPoint1[0];
            double dP1y = SseiD.dPoint1[1];
            double dP2x = SseiD.dPoint2[0];
            double dP2y = SseiD.dPoint2[1];
            double dcos = (dP2x-dP1x) / sqrt(pow(dP2x-dP1x,2)+pow(dP2y-dP1y,2));
            dAngle = acos(dcos);
        }
        else //Angle
        {
            dAngle = SseiD.dAngle;
        }
        Write_Command(_T("GR_SSEI"));
        Write_Int(SseiK);
        Write_String(StldD.LoadCaseType);
        Write_String(StldD.LoadCaseName);
        Write_Double(SseiD.dKh);
        Write_Double(dAngle);
        int iUseGrup = (SseiD.bUseGrup ? 1 : 0);
        Write_Int(iUseGrup);
        Write_Int(SseiD.GrupKey);
        for(int k=0; k<SseiD.arKeyElem.GetSize(); k++)
        {
            if(!Write_Int(SseiD.arKeyElem.GetAt(k)))
            {
                Write_Command(_T("GR_SSEI"), TRUE);
                Write_NullInt();
                Write_NullString();
                Write_NullString();
                Write_NullDouble();
                Write_NullDouble();
                Write_NullInt();
                Write_NullInt();
            }
        }
    }

    return TRUE;
}
BOOL CCRCExportFile::Write_TndnData()
{

    if(!Write_TndnGrupData()) return FALSE;

    if(!Write_TndnProfileData()) return FALSE;

    if(!Write_TndnForceData())  return FALSE;

    return TRUE;
}

BOOL CCRCExportFile::Write_FinalStagData()
{
    //완성계 
    m_pDoc->m_pStagCtrl->SetFinalStagWithoutUpdateNotify();

    BOOL bOK = FALSE;
    bOK = Write_FinalData();
    if(bOK) bOK = Write_AvgTndnData();
    if(bOK) bOK = Write_SecPropData();
    if(bOK) bOK = Write_ForceData();
    if(bOK) bOK = Write_StressData();
    if(bOK) bOK = Write_BtssForceData();
    if(bOK) bOK = Write_LcomForceData();
    if(bOK) bOK = Write_Dgn_Data();

    m_pDoc->m_pStagCtrl->SetCurStagWithoutUpdateNotify(0);

    return TRUE;
}

BOOL CCRCExportFile::Write_FinalData()
{
    BOOL bOK = FALSE;
    bOK = Write_FinalInfluenceLine();
    if(bOK) bOK = Write_FinalLoadLine();
    if(bOK) bOK = Write_SpanData();

    return bOK;
}

BOOL CCRCExportFile::Write_AvgTndnData()
{
    T_TDGR_K TdgrK; 
    T_TDGR_D TdgrD;
    _DGN_TDGR_CRC TdgrCrc;
    T_SPSC_TNDN SpscTndn;

    for(int i=0; i<m_PSCBeamPostD.PElemKList.GetSize(); i++)
    {
        T_ELEM_K ElemK = m_PSCBeamPostD.PElemKList.GetAt(i);
        ElemPairK EPairK(ElemK, EN_EL_BEAM);

        for(int j=0; j<2; j++)
        {
            BOOL bPosI = (j==0 ? TRUE : FALSE);
            int iPos = (j==0 ? 1 : 2); 
            POSITION Pos = m_DataCtrl.m_amTdgrK.GetStartPosition();
            while(Pos)
            {
                TdgrK = 0;
                TdgrCrc.Initialize();
                m_DataCtrl.m_amTdgrK.GetNextAssoc(Pos, TdgrK, TdgrCrc);
                if(!m_DataCtrl.Get_CrcSpscTdgr(EPairK,bPosI,0.,0.,TdgrCrc,SpscTndn))	continue;

                Write_Command(_T("T_AVG"));
                Write_Int(ElemK);
                Write_Int(iPos);
                Write_String(TdgrCrc.strTdgrNa);
                Write_Double(SpscTndn.dTotNum);
                Write_Double(SpscTndn.dEffNum);
                Write_Double(SpscTndn.dyz[0]);
                Write_Double(SpscTndn.dyz[1]);
                Write_Double(SpscTndn.dEffStress);
                Write_Double(SpscTndn.dStress[0]);
                Write_Double(SpscTndn.dStress[1]);
                Write_Double(SpscTndn.dStress[2]);
            }
        }
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_SecPropData()
{
    T_SECT_K SectK, rSectK;
    T_SECT_D SectD;

    // Add by ZINU.('04.08.26). Gross Sect applied Sect Stiffness Factor.
    // 총단면 (강성증감계수 적용).
    for(int i=0; i<m_PSCBeamPostD.PSectKList.GetSize(); i++)
    {
        SectK = m_PSCBeamPostD.PSectKList.GetAt(i);
        for(int j=0; j<2; j++)
        {
            BOOL bPosI = (j==0 ? TRUE : FALSE);
            T_SPSC_SECT SpscSect; SpscSect.Initialize();
            if(!CSectUtil::GetPscSectCG(TRUE, SectK, bPosI, TRUE, SpscSect)) continue;
            if(j==0)
            {
                Write_Command(_T("S_GROSF"));
                Write_Int(SectK);
            }
            else
            {
                Write_Command(_T("S_GROSF"), TRUE);
                Write_NullInt();
            }
            Write_Int(j+1);
            Write_Double(SpscSect.dyDim);
            Write_Double(SpscSect.dzDim);
            Write_Double(SpscSect.dyCen);
            Write_Double(SpscSect.dzCen);
            Write_Double(SpscSect.dArea);
            Write_Double(SpscSect.dIxx);
            Write_Double(SpscSect.dIyy);
            Write_Double(SpscSect.dIzz);
            Write_Double(SpscSect.dSlt);
            Write_Double(SpscSect.dSrt);
            Write_Double(SpscSect.dSlb);
            Write_Double(SpscSect.dSrb);
            Write_Double(SpscSect.dAsy); //2005.03.03 추가 By RSH
            Write_Double(SpscSect.dAsz); //2005.03.03 추가 By RSH
        }
    }

    CMap<UINT, UINT, UINT, UINT> PSCSectKMap; PSCSectKMap.RemoveAll();
    for(int i=0; i<m_PSCBeamPostD.PSectKList.GetSize(); i++)
    {
        PSCSectKMap.SetAt(m_PSCBeamPostD.PSectKList.GetAt(i),m_PSCBeamPostD.PSectKList.GetAt(i));
    }

    for(int i=0; i<m_ExtElemPostD.PSectKList.GetSize(); i++)
    {
        SectK = m_ExtElemPostD.PSectKList.GetAt(i);
        if(PSCSectKMap.Lookup(SectK, rSectK) != 0) continue;
        T_SECT_K OrgSectK = m_pDoc->m_pPostCtrl->GetOrgSectKeyPost(SectK);
        for(int j=0; j<2; j++)
        {
            BOOL bPosI = (j==0 ? TRUE : FALSE);

            T_SECT_D SectD; SectD.Initialize();
            if(!m_pDoc->m_pPostCtrl->GetSectPost(SectK, SectD)) continue;

            double dArea=0.0, dIxx=0.0, dIyy=0.0, dIzz=0.0;
            // Change by ZINU.('07.03.28). NO:2712, Separate T_SECF_D J from I for Tapered Section.
            T_SECF_D SecfD; SecfD.Initialize();
            BOOL bGetSecf = m_pDoc->m_pAttrCtrl->GetSecfSect(OrgSectK,SecfD);
            if(j==0 || (j==1 && CDBLib::IsSectI(FALSE, SectD.nStype)))
            {
                dArea = SectD.SectBefore.SectI.Stiffness.Area * (bGetSecf ? SecfD.dScaleFactor[0] : 1.0);
                dIxx  = SectD.SectBefore.SectI.Stiffness.Rxx  * (bGetSecf ? SecfD.dScaleFactor[3] : 1.0);
                dIyy  = SectD.SectBefore.SectI.Stiffness.Ryy  * (bGetSecf ? SecfD.dScaleFactor[4] : 1.0);
                dIzz  = SectD.SectBefore.SectI.Stiffness.Rzz  * (bGetSecf ? SecfD.dScaleFactor[5] : 1.0);
            }
            else
            {
                dArea = SectD.SectBefore.SectJ.Stiffness.Area * (bGetSecf ? SecfD.dScaleFactorJ[0] : 1.0);
                dIxx  = SectD.SectBefore.SectJ.Stiffness.Rxx  * (bGetSecf ? SecfD.dScaleFactorJ[3] : 1.0);
                dIyy  = SectD.SectBefore.SectJ.Stiffness.Ryy  * (bGetSecf ? SecfD.dScaleFactorJ[4] : 1.0);
                dIzz  = SectD.SectBefore.SectJ.Stiffness.Rzz  * (bGetSecf ? SecfD.dScaleFactorJ[5] : 1.0);
            }

            if(j==0)
            {

                Write_Command(_T("S_GROSF"));
                Write_Int(SectK);
            }
            else
            {
                Write_Command(_T("S_GROSF"), TRUE);
                Write_NullInt();
            }
            Write_Int(j+1);
            Write_Double(0.0);
            Write_Double(0.0);
            Write_Double(0.0);
            Write_Double(0.0);
            Write_Double(dArea);
            Write_Double(dIxx);
            Write_Double(dIyy);
            Write_Double(dIzz);
            Write_Double(0.0);
            Write_Double(0.0);
            Write_Double(0.0);
            Write_Double(0.0);
            Write_Double(0.0); //2005.03.03 추가 By RSH
            Write_Double(0.0); //2005.03.03 추가 By RSH
        }
    }

    //총단면
    for(int i=0; i<m_PSCBeamPostD.PSectKList.GetSize(); i++)
    {
        SectK = m_PSCBeamPostD.PSectKList.GetAt(i);
        for(int j=0; j<2; j++)
        {
            BOOL bPosI = (j==0 ? TRUE : FALSE);
            T_SPSC_SECT SpscSect; SpscSect.Initialize();
            if(!CSectUtil::GetPscSectCG(TRUE, SectK, bPosI, FALSE, SpscSect)) continue;
            if(j==0)
            {
                Write_Command(_T("S_GROS"));
                Write_Int(SectK);
            }
            else
            {
                Write_Command(_T("S_GROS"), TRUE);
                Write_NullInt();
            }
            Write_Int(j+1);
            Write_Double(SpscSect.dyDim);
            Write_Double(SpscSect.dzDim);
            Write_Double(SpscSect.dyCen);
            Write_Double(SpscSect.dzCen);
            Write_Double(SpscSect.dArea);
            Write_Double(SpscSect.dIxx);
            Write_Double(SpscSect.dIyy);
            Write_Double(SpscSect.dIzz);
            Write_Double(SpscSect.dSlt);
            Write_Double(SpscSect.dSrt);
            Write_Double(SpscSect.dSlb);
            Write_Double(SpscSect.dSrb);
        }
    }

    for(int i=0; i<m_ExtElemPostD.PSectKList.GetSize(); i++)
    {
        SectK = m_ExtElemPostD.PSectKList.GetAt(i);
        if(PSCSectKMap.Lookup(SectK, rSectK) != 0) continue;
        T_SECT_K OrgSectK = m_pDoc->m_pPostCtrl->GetOrgSectKeyPost(SectK);
        for(int j=0; j<2; j++)
        {
            BOOL bPosI = (j==0 ? TRUE : FALSE);

            T_SECT_D SectD; SectD.Initialize();
            if(!m_pDoc->m_pPostCtrl->GetSectPost(SectK, SectD)) continue;

            double dArea=0.0, dIxx=0.0, dIyy=0.0, dIzz=0.0;
            // Change by ZINU.('07.03.28). NO:2712, Separate T_SECF_D J from I for Tapered Section.
            T_SECF_D SecfD; SecfD.Initialize();
            BOOL bGetSecf = m_pDoc->m_pAttrCtrl->GetSecfSect(OrgSectK,SecfD);
            if(j==0 || (j==1 && CDBLib::IsSectI(FALSE, SectD.nStype)))
            {
                dArea = SectD.SectBefore.SectI.Stiffness.Area * (bGetSecf ? SecfD.dScaleFactor[0] : 1.0);
                dIxx  = SectD.SectBefore.SectI.Stiffness.Rxx  * (bGetSecf ? SecfD.dScaleFactor[3] : 1.0);
                dIyy  = SectD.SectBefore.SectI.Stiffness.Ryy  * (bGetSecf ? SecfD.dScaleFactor[4] : 1.0);
                dIzz  = SectD.SectBefore.SectI.Stiffness.Rzz  * (bGetSecf ? SecfD.dScaleFactor[5] : 1.0);
            }
            else
            {
                dArea = SectD.SectBefore.SectJ.Stiffness.Area * (bGetSecf ? SecfD.dScaleFactorJ[0] : 1.0);
                dIxx  = SectD.SectBefore.SectJ.Stiffness.Rxx  * (bGetSecf ? SecfD.dScaleFactorJ[3] : 1.0);
                dIyy  = SectD.SectBefore.SectJ.Stiffness.Ryy  * (bGetSecf ? SecfD.dScaleFactorJ[4] : 1.0);
                dIzz  = SectD.SectBefore.SectJ.Stiffness.Rzz  * (bGetSecf ? SecfD.dScaleFactorJ[5] : 1.0);
            }

            if(j==0)
            {

                Write_Command(_T("S_GROS"));
                Write_Int(SectK);
            }
            else
            {
                Write_Command(_T("S_GROS"), TRUE);
                Write_NullInt();
            }
            Write_Int(j+1);
            Write_Double(0.0);
            Write_Double(0.0);
            Write_Double(0.0);
            Write_Double(0.0);
            Write_Double(dArea);
            Write_Double(dIxx);
            Write_Double(dIyy);
            Write_Double(dIzz);
            Write_Double(0.0);
            Write_Double(0.0);
            Write_Double(0.0);
            Write_Double(0.0);
        }
    }

    //순단면
    T_ELEM_K ElemK;
    for(int i=0; i<m_PSCBeamPostD.PElemKList.GetSize(); i++)
    {
        ElemK = m_PSCBeamPostD.PElemKList.GetAt(i);
		ElemPairK EPairK(ElemK, EN_EL_BEAM);
        for(int j=0; j<2; j++)
        {
            BOOL bPosI = (j==0 ? TRUE : FALSE);
            T_SPSC_SECT SpscSect; SpscSect.Initialize();
            if(!CSectUtil::GetPscSectCN(TRUE, EPairK, bPosI, FALSE, SpscSect)) continue;
            if(j==0)
            {
                Write_Command(_T("S_NET"));
                Write_Int(ElemK);
            }
            else
            {
                Write_Command(_T("S_NET"), TRUE);
                Write_NullInt();
            }
            Write_Int(j+1);
            Write_Double(SpscSect.dyDim);
            Write_Double(SpscSect.dzDim);
            Write_Double(SpscSect.dyCen);
            Write_Double(SpscSect.dzCen);
            Write_Double(SpscSect.dArea);
            Write_Double(SpscSect.dIyy);
            Write_Double(SpscSect.dIzz);
            Write_Double(SpscSect.dSlt);
            Write_Double(SpscSect.dSrt);
            Write_Double(SpscSect.dSlb);
            Write_Double(SpscSect.dSrb);
        }

    }

    //철근환산단면
    for(int i=0; i<m_PSCBeamPostD.PElemKList.GetSize(); i++)
    {
        ElemK = m_PSCBeamPostD.PElemKList.GetAt(i);
        for(int j=0; j<2; j++)
        {
            BOOL bPosI = (j==0 ? TRUE : FALSE);
            T_SPSC_SECT SpscSect; SpscSect.Initialize();
            // Change by ZINU.('06.07.03). GetPscSectCR() -> GetPscSectCNR().
            if(!CSectUtil::GetPscSectCNR(TRUE, ElemK, bPosI, FALSE, SpscSect)) continue;
            if(j==0)
            {
                Write_Command(_T("S_NETR"));
                Write_Int(ElemK);
            }
            else
            {
                Write_Command(_T("S_NETR"), TRUE);
                Write_NullInt();
            }
            Write_Int(j+1);
            Write_Double(SpscSect.dyDim);
            Write_Double(SpscSect.dzDim);
            Write_Double(SpscSect.dyCen);
            Write_Double(SpscSect.dzCen);
            Write_Double(SpscSect.dArea);
            Write_Double(SpscSect.dIyy);
            Write_Double(SpscSect.dIzz);
            Write_Double(SpscSect.dSlt);
            Write_Double(SpscSect.dSrt);
            Write_Double(SpscSect.dSlb);
            Write_Double(SpscSect.dSrb);
        }
    }

    //철근+PC환산단면
    for(int i=0; i<m_PSCBeamPostD.PElemKList.GetSize(); i++)
    {
        ElemK = m_PSCBeamPostD.PElemKList.GetAt(i);
		ElemPairK EPairK(ElemK, EN_EL_BEAM);
        for(int j=0; j<2; j++)
        {
            BOOL bPosI = (j==0 ? TRUE : FALSE);
            T_SPSC_SECT SpscSect; SpscSect.Initialize();
            if(!CSectUtil::GetPscSectCPR(TRUE, EPairK, bPosI, FALSE, TRUE, SpscSect)) continue;
            if(j==0)
            {
                Write_Command(_T("S_NETPR"));
                Write_Int(ElemK);
            }
            else
            {
                Write_Command(_T("S_NETPR"), TRUE);
                Write_NullInt();
            }
            Write_Int(j+1);
            Write_Double(SpscSect.dyDim);
            Write_Double(SpscSect.dzDim);
            Write_Double(SpscSect.dyCen);
            Write_Double(SpscSect.dzCen);
            Write_Double(SpscSect.dArea);
            Write_Double(SpscSect.dIyy);
            Write_Double(SpscSect.dIzz);
            Write_Double(SpscSect.dSlt);
            Write_Double(SpscSect.dSrt);
            Write_Double(SpscSect.dSlb);
            Write_Double(SpscSect.dSrb);
        }
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_ShearData()
{
    // Add by ZINU.('06.02.16). Check Bridge Direction.
    if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

    T_SECT_K SectK;
    for(int i=0; i<m_PSCBeamPostD.PSectKList.GetSize(); i++)
    {
        SectK = m_PSCBeamPostD.PSectKList.GetAt(i);
        for(int j=0; j<2; j++)
        {
            BOOL bPosI = (j==0 ? TRUE : FALSE);
            T_SPSC_DSHR SpscDshr; SpscDshr.Initialize();
            if(!CSectUtil::GetPscShearPosi(TRUE, SectK, bPosI, SpscDshr, TRUE)) continue;
            if(j==0)
            {
                Write_Command(_T("S_SHR"));
                Write_Int(SectK);
            }
            else
            {
                Write_Command(_T("S_SHR"), TRUE);
                //Write_NullInt();  //By Kono
                Write_Int(SectK);
            }
            Write_Int(j+1);
            Write_Double(SpscDshr.dKt);
            Write_Double(SpscDshr.dKtThik);
            Write_Double(SpscDshr.dyz[1][0]);
            Write_Double(SpscDshr.dQ[0]);
            Write_Double(SpscDshr.db[0]);
            Write_Double(SpscDshr.dyz[1][2]);
            Write_Double(SpscDshr.dQ[1]);
            Write_Double(SpscDshr.db[1]);
            Write_Double(SpscDshr.dyz[1][4]);
            Write_Double(SpscDshr.dQ[2]);
            Write_Double(SpscDshr.db[2]);
        }
    }

#ifdef _DEBUG
    // Add by ZINU.('04.05.05). For Testing.
    for(int i=0; i<m_PSCBeamPostD.PSectKList.GetSize(); i++)
    {
        SectK = m_PSCBeamPostD.PSectKList.GetAt(i);
        T_SPSC_DSHR SpscDshrI; SpscDshrI.Initialize();
        T_SPSC_DSHR SpscDshrJ; SpscDshrJ.Initialize();
        if(!CSectUtil::GetPscShearPosi(TRUE, SectK, TRUE, SpscDshrI, TRUE)) continue;
        if(!CSectUtil::GetPscShearPosi(TRUE, SectK, FALSE, SpscDshrJ, TRUE)) continue;
        Write_Command(_T("S_JT"));
        Write_Int(SectK);
        Write_Double(SpscDshrI.dJt);
        Write_Double(SpscDshrJ.dJt);
    }
#endif

    for(int i=0; i<m_PSCBeamPostD.PSectKList.GetSize(); i++)
    {
        SectK = m_PSCBeamPostD.PSectKList.GetAt(i);
        for(int j=0; j<2; j++)
        {
            BOOL bPosI = (j==0 ? TRUE : FALSE);
            BOOL bClosedSect=FALSE;
            int  nDgnSectType = -1;
            double dKt=0.0, dWt=0.0;
            double aKt[6]={0.0};
            double dPyKt[6], dPzKt[6];
            double dpThkFlange[2]={0.,0.};	// Flange Thickness (0=Top,1=Bot).
            BOOL bDivSect4Tor=FALSE;
            int nDivPart=0;
            double dpCutPos[3]={0.};
			int nWebSize = 1;
            if(!CSectUtil::GetPscKtAmPoint(TRUE, SectK, bPosI, bClosedSect, nDgnSectType, dKt, aKt, dWt, dPyKt, dPzKt, dpThkFlange, bDivSect4Tor, nDivPart, dpCutPos, nWebSize)) continue;
            if(j==0)
            {
                Write_Command(_T("S_KT"));
                Write_Int(SectK);
            }
            else
            {
                Write_Command(_T("S_KT"), TRUE);
                Write_NullInt();
            }
            Write_Int(j+1);
            Write_Double(bClosedSect ? dPyKt[0] : 0.0);
            Write_Double(bClosedSect ? dPzKt[0] : 0.0);
            Write_Double(bClosedSect ? dPyKt[1] : 0.0);
            Write_Double(bClosedSect ? dPzKt[1] : 0.0);
            Write_Double(bClosedSect ? dPyKt[2] : 0.0);
            Write_Double(bClosedSect ? dPzKt[2] : 0.0);
            Write_Double(bClosedSect ? dPyKt[3] : 0.0);
            Write_Double(bClosedSect ? dPzKt[3] : 0.0);
            Write_Double(bClosedSect ? dPyKt[4] : 0.0);
            Write_Double(bClosedSect ? dPzKt[4] : 0.0);
            Write_Double(bClosedSect ? dPyKt[5] : 0.0);
            Write_Double(bClosedSect ? dPzKt[5] : 0.0);
            Write_Double(bClosedSect ? dPyKt[0] : 0.0);
            Write_Double(bClosedSect ? dPzKt[0] : 0.0);
        }
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_StgForceData()
{
    if(!m_bStag) return TRUE;

    CArray<T_STAG_K,T_STAG_K> StagKList;
    m_pDoc->m_pPostCtrl->GetStageInfo()->GetResultSavedStagKeyList(StagKList);
    if(!StagKList.GetSize()) return TRUE;

    CCRCForceCtrl* pForceCtrl = m_DataCtrl.Get_ForceCtrlPointer();

    for(int i=0; i<StagKList.GetSize(); i++)
    {
        T_STAG_K StagK = StagKList.GetAt(i);

        _CRC_EXPD PSCBeamStagData; PSCBeamStagData.Initialize();
        if(m_PSCBeamStagDMap.Lookup(StagK, PSCBeamStagData) == 0) continue;

        int iStageId = PSCBeamStagData.StagD.nStageId;    

        int iFinalStepNo = 0;
        Get_LastStepInCurStage(StagK, iFinalStepNo);

        for(int j=0; j<PSCBeamStagData.PElemKList.GetSize(); j++)
        {
            T_ELEM_K ElemK = PSCBeamStagData.PElemKList.GetAt(j);
            ElemPairK EPairK(ElemK, EN_EL_BEAM);

            for(int k=0; k<PSCBeamStagData.SgldKList.GetSize(); k++)
            {
                T_SGLD_K SgldK = PSCBeamStagData.SgldKList.GetAt(k);
                T_SGLD_D SgldD = PSCBeamStagData.SgldDList.GetAt(k);

                _DGN_FORC_CRC ForceLcase;  ForceLcase.Initialize();
                _DGN_STRS_CRC StressLcase; StressLcase.Initialize();
                _DGN_SHRS_ADD StssLcase;   StssLcase.Initialize();
                if(!pForceCtrl->Get_Sgld_Expo(EPairK, SgldK, SgldD, iFinalStepNo, ForceLcase, StressLcase, StssLcase)) continue;

                Write_Command(_T("SE_FORC"));
                Write_Int(iStageId);
                Write_Int(ElemK);
                Write_String(ForceLcase.strType);
                Write_Int(1);
                Write_Double(ForceLcase.dFxx[0]);
                Write_Double(ForceLcase.dFyy[0]);
                Write_Double(ForceLcase.dFzz[0]);
                Write_Double(ForceLcase.dMux[0]);
                Write_Double(ForceLcase.dMuy[0]);
                Write_Double(ForceLcase.dMuz[0]);

                Write_Command(_T("SE_FORC"), TRUE);
                Write_NullInt();
                Write_NullInt();
                Write_NullString();
                Write_Int(2);
                Write_Double(ForceLcase.dFxx[1]);
                Write_Double(ForceLcase.dFyy[1]);
                Write_Double(ForceLcase.dFzz[1]);
                Write_Double(ForceLcase.dMux[1]);
                Write_Double(ForceLcase.dMuy[1]);
                Write_Double(ForceLcase.dMuz[1]);
            }
        }
    }
    return TRUE;
}

BOOL CCRCExportFile::Write_StgCpShData()
{
    // Add by ZINU.('06.02.16). Check Bridge Direction.
    if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.
    if(!m_bStag) return TRUE;

    T_STCT_D StctD; StctD.Initialize();
    m_pDoc->m_pAttrCtrl->GetStct(StctD);
    if(!StctD.bIncludeTimeDependentEffect) return TRUE;

    int iCount=0;
    for(int i=0; i<m_StagKList.GetSize(); i++)
    {
        T_STAG_K StagK = m_StagKList.GetAt(i);

        _CRC_EXPD PSCBeamStagData; PSCBeamStagData.Initialize();
        if(m_PSCBeamStagDMap.Lookup(StagK, PSCBeamStagData)==0) continue;

        int iStageId = PSCBeamStagData.StagD.nStageId;

        iCount=0;
        for(int j=0; j<PSCBeamStagData.PElemKList.GetSize(); j++)
        {
            T_ELEM_K ElemK = PSCBeamStagData.PElemKList.GetAt(j);
            T_ELFM_D ElfmD; ElfmD.Initialize();
            if(!m_pDoc->m_pPostCtrl->GetElfm(StagK, ElemK, ElfmD)) continue;
            iCount++;
            if(iCount==1)
            {
                Write_Command(_T("SE_CPSH"));
                Write_Int(iStageId);
            }
            else
            {
                Write_Command(_T("SE_CPSH"), TRUE);
                Write_NullInt();
            }
            Write_Int(ElemK);
            Write_Double(ElfmD.dStartMatAge);
            Write_Double(ElfmD.dEndMatAge);
            Write_Double(ElfmD.dStartElast);
            Write_Double(ElfmD.dEndElast);
            Write_Double(ElfmD.dAddedCrSkStrain);
            Write_Double(ElfmD.dCreepStrain);
        }
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_ForceData()
{
    CCRCForceCtrl* pForceCtrl = m_DataCtrl.Get_ForceCtrlPointer();

    CArray<T_STLD_K, T_STLD_K> StldKList;
    StldKList.RemoveAll();
    m_pDoc->m_pPostCtrl->GetStldKeyList(StldKList);

    int nForType = 0; // 단면 부재력 받아옴. 
    BOOL bCalcStss = FALSE;
    T_ELEM_D ElemD;
    for(int i=0; i<m_PSCBeamPostD.PElemKList.GetSize(); i++)
    {
        T_ELEM_K ElemK = m_PSCBeamPostD.PElemKList.GetAt(i);
        ElemPairK EPairK(ElemK, EN_EL_BEAM);

        ElemD.Initialize();
        m_pDoc->m_pPostCtrl->GetElemPost(m_PSCBeamPostD.PElemKList[i], ElemD);

        //Static Force
        for(int j=0; j<StldKList.GetSize(); j++)
        {
            T_STLD_K StldK = StldKList.GetAt(j);

            _DGN_FORC_CRC ForceLcase; 
            _DGN_STRS_ADD StressAdd; 
            _DGN_STRS_CRC StressLcase;
            _DGN_SHRS_ADD StssLcase; 
            _DGN_FORC_CRC ForcePart1;
            _DGN_STRS_CRC StressPart1;
            _DGN_FORC_CRC ForcePart2;
            _DGN_STRS_CRC StressPart2;
            if(!pForceCtrl->Get_Stld(EPairK, StldK, nForType, bCalcStss, ForceLcase, StressAdd, StressLcase, StssLcase,
                ForcePart1, StressPart1, ForcePart2, StressPart2)) continue;

            for(int k=0; k<2 ; k++)
            {
                Write_Command(_T("E_FORC"));
                Write_Int(ElemK);
                Write_Int(k+1);
                Write_String(ForceLcase.strType);
                Write_Int(0);
                Write_Double(ForceLcase.dFxx[k]);
                Write_Double(ForceLcase.dFyy[k]);
                Write_Double(ForceLcase.dFzz[k]);
                Write_Double(ForceLcase.dMux[k]);
                Write_Double(ForceLcase.dMuy[k]);
                Write_Double(ForceLcase.dMuz[k]);
            }
        }

        //Moving Load Force
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

        for(int j=0; j<TotalMvldKList.GetSize(); j++)
        {
            UINT MvldK = TotalMvldKList.GetAt(j);

            CString strLcomType = _T("");
            ADGNFORCE aForceLcase;
            ADGNSTRESS aStressLcase;
            ADGNSHRSADD aStssLcase;
            ADGNFORCE   aForceP1;
            ADGNSTRESS  aStressP1;
            ADGNFORCE   aForceP2;
            ADGNSTRESS  aStressP2;
            if(!pForceCtrl->Get_Mvld(EPairK, MvldK, nForType, bCalcStss, strLcomType, aForceLcase, aStressLcase, aStssLcase,
                aForceP1, aStressP1, aForceP2, aStressP2)) continue;

            for(int k=0; k<aForceLcase.GetSize(); k++)
            {
                _DGN_FORC_CRC &ForceLcase = aForceLcase.GetAt(k);

                for(int l=0; l<2; l++)
                {
                    Write_Command(_T("E_FORC"));
                    Write_Int(ElemK);
                    Write_Int(l+1);
                    Write_String(strLcomType);
                    Write_Int(k+1);
                    Write_Double(ForceLcase.dFxx[l]);
                    Write_Double(ForceLcase.dFyy[l]);
                    Write_Double(ForceLcase.dFzz[l]);
                    Write_Double(ForceLcase.dMux[l]);
                    Write_Double(ForceLcase.dMuy[l]);
                    Write_Double(ForceLcase.dMuz[l]);
                }
            }
        }

        //Settlement Load Force
        CArray<T_SMLC_K, T_SMLC_K> SmlcKList;
        SmlcKList.RemoveAll();
        m_pDoc->m_pAttrCtrl->GetSmlcKeyList(SmlcKList);

        for(int j=0; j<SmlcKList.GetSize(); j++)
        {
            T_SMLC_K SmlcK = SmlcKList.GetAt(j);

            ADGNFORCE aForceLcase;
            ADGNSTRESS aStressLcase;
            ADGNSHRSADD aStssLcase;
            ADGNFORCE   aForceP1;
            ADGNSTRESS  aStressP1;
            ADGNFORCE   aForceP2;
            ADGNSTRESS  aStressP2;

            CString strLcomType = _T("");
            if(!pForceCtrl->Get_Smlc(EPairK, SmlcK, nForType, bCalcStss, strLcomType, aForceLcase, aStressLcase, aStssLcase,
                aForceP1, aStressP1, aForceP2, aStressP2)) continue;

            for(int k=0; k<aForceLcase.GetSize(); k++)
            {
                _DGN_FORC_CRC &ForceLcase = aForceLcase.GetAt(k);

                for(int l=0; l<2 ; l++)
                {
                    Write_Command(_T("E_FORC"));
                    Write_Int(ElemK);
                    Write_Int(l+1);
                    Write_String(strLcomType);
                    Write_Int(k+1);
                    Write_Double(ForceLcase.dFxx[l]);
                    Write_Double(ForceLcase.dFyy[l]);
                    Write_Double(ForceLcase.dFzz[l]);
                    Write_Double(ForceLcase.dMux[l]);
                    Write_Double(ForceLcase.dMuy[l]);
                    Write_Double(ForceLcase.dMuz[l]);
                }
            }
        }

        //Response Spectrum
        CArray<T_SPLC_K, T_SPLC_K> SplcKList;
        SplcKList.RemoveAll();
        m_pDoc->m_pAttrCtrl->GetSplcKeyList(SplcKList);

        for(int j=0; j<SplcKList.GetSize(); j++)
        {
            T_SPLC_K SplcK = SplcKList.GetAt(j);

            _DGN_FORC_CRC ForceLcase;
            _DGN_STRS_CRC StressLcase;
            _DGN_SHRS_ADD StssLcase;
            _DGN_FORC_CRC ForcePart1;
            _DGN_STRS_CRC StressPart1;
            _DGN_FORC_CRC ForcePart2;
            _DGN_STRS_CRC StressPart2;
            if(!pForceCtrl->Get_Splc(EPairK, SplcK, nForType, bCalcStss, ForceLcase, StressLcase, StssLcase,
                ForcePart1, StressPart1, ForcePart2, StressPart2)) continue;

            for(int k=0; k<2 ; k++)
            {
                Write_Command(_T("E_FORC"));
                Write_Int(ElemK);
                Write_Int(k+1);
                Write_String(_T("RSP"));
                Write_Int(0);
                Write_Double(ForceLcase.dFxx[k]);
                Write_Double(ForceLcase.dFyy[k]);
                Write_Double(ForceLcase.dFzz[k]);
                Write_Double(ForceLcase.dMux[k]);
                Write_Double(ForceLcase.dMuy[k]);
                Write_Double(ForceLcase.dMuz[k]);
            }
        }

        //Time History
        CArray<T_THIS_K, T_THIS_K> ThisKList;
        ThisKList.RemoveAll();
        m_pDoc->m_pAttrCtrl->GetThisKeyList(ThisKList);

        for(int j=0; j<ThisKList.GetSize(); j++)
        {
            T_THIS_K ThisK = ThisKList.GetAt(j);

            _DGN_FORC_CRC ForceLcase;
            _DGN_STRS_CRC StressLcase;
            _DGN_SHRS_ADD StssLcase;
            _DGN_FORC_CRC ForcePart1;
            _DGN_STRS_CRC StressPart1;
            _DGN_FORC_CRC ForcePart2;
            _DGN_STRS_CRC StressPart2;
            if(!pForceCtrl->Get_This(EPairK, ThisK, nForType, bCalcStss, ForceLcase, StressLcase, StssLcase,
                ForcePart1, StressPart1, ForcePart2, StressPart2)) continue;

            for(int k=0; k<2 ; k++)
            {
                Write_Command(_T("E_FORC"));
                Write_Int(ElemK);
                Write_Int(k+1);
                Write_String(_T("TH"));
                Write_Int(0);
                Write_Double(ForceLcase.dFxx[k]);
                Write_Double(ForceLcase.dFyy[k]);
                Write_Double(ForceLcase.dFzz[k]);
                Write_Double(ForceLcase.dMux[k]);
                Write_Double(ForceLcase.dMuy[k]);
                Write_Double(ForceLcase.dMuz[k]);
            }
        }

        //Grid Model
        CArray<T_GILC_K, T_GILC_K> GilcKList;
        GilcKList.RemoveAll();
        m_pDoc->m_pAttrCtrl->GetGilcKeyList(GilcKList);

        for(int j=0; j<GilcKList.GetSize(); j++)
        {
            T_GILC_K GilcK = GilcKList.GetAt(j);
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
            if(!pForceCtrl->Get_Gilc(EPairK, GilcK, nForType, bCalcStss, GildD.nType, strLcomType, aForceLcase, aStressLcase, aStssLcase,
                aForceP1, aStressP1, aForceP2, aStressP2)) continue;

            for(int k=0; k<aForceLcase.GetSize(); k++)
            {
                _DGN_FORC_CRC &ForceLcase = aForceLcase.GetAt(k);

                for(int l=0; l<2 ; l++)
                {
                    Write_Command(_T("E_FORC"));
                    Write_Int(ElemK);
                    Write_Int(l+1);
                    Write_String(strLcomType);
                    if(aForceLcase.GetSize() == 1)  Write_Int(0);
                    else                            Write_Int(k+1);
                    Write_Double(ForceLcase.dFxx[l]);
                    Write_Double(ForceLcase.dFyy[l]);
                    Write_Double(ForceLcase.dFzz[l]);
                    Write_Double(ForceLcase.dMux[l]);
                    Write_Double(ForceLcase.dMuy[l]);
                    Write_Double(ForceLcase.dMuz[l]);
                }
            }
        }
    }
    return TRUE;
}

BOOL CCRCExportFile::Write_StressData()
{
    // Add by ZINU.('06.02.16). Write Stress by Loadcase.
    // SE_FORC (DL,EL,TP,TS,CS,SS at Final Stage) + E_FORC.
    CCRCForceCtrl* pForceCtrl = m_DataCtrl.Get_ForceCtrlPointer();

    if(m_bStag)	// Only if Stage Exists.
    {
        CArray<T_STAG_K,T_STAG_K> StagKList;
        m_pDoc->m_pPostCtrl->GetStageInfo()->GetResultSavedStagKeyList(StagKList);
        if(StagKList.GetSize() > 0)
        {
            T_STAG_K StagK = StagKList.GetAt(StagKList.GetSize()-1);

            _CRC_EXPD PSCBeamStagData; PSCBeamStagData.Initialize();
            if(m_PSCBeamStagDMap.Lookup(StagK, PSCBeamStagData)==0) ASSERT(0);

            int iFinalStepNo = 0;
            Get_LastStepInCurStage(StagK, iFinalStepNo);

            for(int i=0; i<PSCBeamStagData.PElemKList.GetSize(); i++)
            {
                T_ELEM_K ElemK = PSCBeamStagData.PElemKList.GetAt(i);
                ElemPairK EPairK(ElemK, EN_EL_BEAM);

                for(int j=0; j<PSCBeamStagData.SgldKList.GetSize(); j++)
                {
                    T_SGLD_K SgldK = PSCBeamStagData.SgldKList.GetAt(j);
                    T_SGLD_D SgldD = PSCBeamStagData.SgldDList.GetAt(j);

                    _DGN_FORC_CRC ForceLcase;  ForceLcase.Initialize();
                    _DGN_STRS_CRC StressLcase; StressLcase.Initialize();
                    _DGN_SHRS_ADD StssLcase;   StssLcase.Initialize();
                    if(!pForceCtrl->Get_Sgld_Expo(EPairK, SgldK, SgldD, iFinalStepNo, ForceLcase, StressLcase, StssLcase)) continue;
                    for(int k=0; k<2; k++)
                    {
                        Write_Command(_T("E_STR"));
                        Write_Int(ElemK);
                        Write_Int(k+1);
                        Write_String(StressLcase.strType);
                        Write_Int(0);
                        Write_Double(StressLcase.dStop[k]);
                        Write_Double(StressLcase.dSbot[k]);
                        Write_Double(StressLcase.dStl[k]);
                        Write_Double(StressLcase.dSbl[k]);
                        Write_Double(StressLcase.dStr[k]);
                        Write_Double(StressLcase.dSbr[k]);
                    }
                }
            }
        }
    }

    CArray<T_STLD_K, T_STLD_K> StldKList;
    StldKList.RemoveAll();
    m_pDoc->m_pPostCtrl->GetStldKeyList(StldKList);

    int nForType = 0;  // 단면부재력 받아옴.
    BOOL bCalcStss = FALSE;
    T_ELEM_D ElemD;
    for(int i=0; i<m_PSCBeamPostD.PElemKList.GetSize(); i++)
    {
        T_ELEM_K ElemK = m_PSCBeamPostD.PElemKList.GetAt(i);
        ElemPairK EPairK(ElemK, EN_EL_BEAM);

        ElemD.Initialize();
        m_pDoc->m_pPostCtrl->GetElemPost(m_PSCBeamPostD.PElemKList[i], ElemD);

        //Static Stress
        for(int j=0; j<StldKList.GetSize(); j++)
        {
            T_STLD_K StldK = StldKList.GetAt(j);

            _DGN_FORC_CRC ForceLcase; ForceLcase.Initialize();
            _DGN_STRS_ADD StressAdd; StressAdd.Initialize();
            _DGN_STRS_CRC StressLcase; StressLcase.Initialize();
            _DGN_SHRS_ADD StssLcase;   StssLcase.Initialize();
            _DGN_FORC_CRC ForcePart1;
            _DGN_STRS_CRC StressPart1;
            _DGN_FORC_CRC ForcePart2;
            _DGN_STRS_CRC StressPart2;
            if(!pForceCtrl->Get_Stld(EPairK, StldK, nForType, bCalcStss, ForceLcase, StressAdd, StressLcase, StssLcase,
                ForcePart1, StressPart1, ForcePart2, StressPart2)) continue;

            for(int k=0; k<2; k++)
            {
                Write_Command(_T("E_STR"));
                Write_Int(ElemK);
                Write_Int(k+1);
                Write_String(StressLcase.strType);
                Write_Int(0);
                Write_Double(StressLcase.dStop[k]);
                Write_Double(StressLcase.dSbot[k]);
                Write_Double(StressLcase.dStl[k]);
                Write_Double(StressLcase.dSbl[k]);
                Write_Double(StressLcase.dStr[k]);
                Write_Double(StressLcase.dSbr[k]);
            }
        }

        //Moving Load Stress
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

        for(int j=0; j<TotalMvldKList.GetSize(); j++)
        {
            UINT MvldK = TotalMvldKList.GetAt(j);

            CString strLcomType = _T("");
            ADGNFORCE aForceLcase;
            ADGNSTRESS aStressLcase;
            ADGNSHRSADD aStssLcase;
            ADGNFORCE   aForceP1;
            ADGNSTRESS  aStressP1;
            ADGNFORCE   aForceP2;
            ADGNSTRESS  aStressP2;
            if(!pForceCtrl->Get_Mvld(EPairK, MvldK, nForType, bCalcStss, strLcomType, aForceLcase, aStressLcase, aStssLcase,
                aForceP1, aStressP1, aForceP2, aStressP2)) continue;

            for(int k=0; k<aStressLcase.GetSize(); k++)
            {
                _DGN_STRS_CRC StressLcase = aStressLcase.GetAt(k);

                for(int l=0; l<2; l++)
                {
                    Write_Command(_T("E_STR"));
                    Write_Int(ElemK);
                    Write_Int(l+1);
                    Write_String(strLcomType);
                    Write_Int(k+1);
                    Write_Double(StressLcase.dStop[l]);
                    Write_Double(StressLcase.dSbot[l]);
                    Write_Double(StressLcase.dStl[l]);
                    Write_Double(StressLcase.dSbl[l]);
                    Write_Double(StressLcase.dStr[l]);
                    Write_Double(StressLcase.dSbr[l]);
                }
            }
        }

        //Settlement Load Stress
        CArray<T_SMLC_K, T_SMLC_K> SmlcKList;
        SmlcKList.RemoveAll();
        m_pDoc->m_pAttrCtrl->GetSmlcKeyList(SmlcKList);

        for(int j=0; j<SmlcKList.GetSize(); j++)
        {
            T_SMLC_K SmlcK = SmlcKList.GetAt(j);

            ADGNFORCE aForceLcase;
            ADGNSTRESS aStressLcase;
            ADGNSHRSADD aStssLcase;
            ADGNFORCE   aForceP1;
            ADGNSTRESS  aStressP1;
            ADGNFORCE   aForceP2;
            ADGNSTRESS  aStressP2;

            CString strLcomType = _T("");
            if(!pForceCtrl->Get_Smlc(EPairK, SmlcK, nForType, bCalcStss, strLcomType, aForceLcase, aStressLcase, aStssLcase,
                aForceP1, aStressP1, aForceP2, aStressP2)) continue;

            for(int k=0; k<aStressLcase.GetSize(); k++)
            {
                _DGN_STRS_CRC &StressLcase = aStressLcase.GetAt(k);

                for(int l=0; l<2; l++)
                {
                    Write_Command(_T("E_STR"));
                    Write_Int(ElemK);
                    Write_Int(l+1);
                    Write_String(strLcomType);
                    Write_Int(k+1);
                    Write_Double(StressLcase.dStop[l]);
                    Write_Double(StressLcase.dSbot[l]);
                    Write_Double(StressLcase.dStl[l]);
                    Write_Double(StressLcase.dSbl[l]);
                    Write_Double(StressLcase.dStr[l]);
                    Write_Double(StressLcase.dSbr[l]);
                }
            }
        }

        //Response Spectrum
        CArray<T_SPLC_K, T_SPLC_K> SplcKList;
        SplcKList.RemoveAll();
        m_pDoc->m_pAttrCtrl->GetSplcKeyList(SplcKList);

        for(int j=0; j<SplcKList.GetSize(); j++)
        {
            T_SPLC_K SplcK = SplcKList.GetAt(j);

            _DGN_FORC_CRC ForceLcase;
            _DGN_STRS_CRC StressLcase;
            _DGN_SHRS_ADD StssLcase;
            _DGN_FORC_CRC ForcePart1;
            _DGN_STRS_CRC StressPart1;
            _DGN_FORC_CRC ForcePart2;
            _DGN_STRS_CRC StressPart2;
            if(!pForceCtrl->Get_Splc(EPairK, SplcK, nForType, bCalcStss, ForceLcase, StressLcase, StssLcase,
                ForcePart1, StressPart1, ForcePart2, StressPart2)) continue;

            for(int k=0; k<2; k++)
            {
                Write_Command(_T("E_STR"));
                Write_Int(ElemK);
                Write_Int(k+1);
                Write_String(_T("RSP"));
                Write_Int(0);
                Write_Double(StressLcase.dStop[k]);
                Write_Double(StressLcase.dSbot[k]);
                Write_Double(StressLcase.dStl[k]);
                Write_Double(StressLcase.dSbl[k]);
                Write_Double(StressLcase.dStr[k]);
                Write_Double(StressLcase.dSbr[k]);
            }
        }

        //Time History
        CArray<T_THIS_K, T_THIS_K> ThisKList;
        ThisKList.RemoveAll();
        m_pDoc->m_pAttrCtrl->GetThisKeyList(ThisKList);

        for(int j=0; j<ThisKList.GetSize(); j++)
        {
            T_THIS_K ThisK = ThisKList.GetAt(j);

            _DGN_FORC_CRC ForceLcase;
            _DGN_STRS_CRC StressLcase;
            _DGN_SHRS_ADD StssLcase;
            _DGN_FORC_CRC ForcePart1;
            _DGN_STRS_CRC StressPart1;
            _DGN_FORC_CRC ForcePart2;
            _DGN_STRS_CRC StressPart2;
            if(!pForceCtrl->Get_This(EPairK, ThisK, nForType, bCalcStss, ForceLcase, StressLcase, StssLcase,
                ForcePart1, StressPart1, ForcePart2, StressPart2)) continue;

            for(int k=0; k<2; k++)
            {
                Write_Command(_T("E_STR"));
                Write_Int(ElemK);
                Write_Int(k+1);
                Write_String(_T("TH"));
                Write_Int(0);
                Write_Double(StressLcase.dStop[k]);
                Write_Double(StressLcase.dSbot[k]);
                Write_Double(StressLcase.dStl[k]);
                Write_Double(StressLcase.dSbl[k]);
                Write_Double(StressLcase.dStr[k]);
                Write_Double(StressLcase.dSbr[k]);
            }
        }

        //Grid Model
        CArray<T_GILC_K, T_GILC_K> GilcKList;
        GilcKList.RemoveAll();
        m_pDoc->m_pAttrCtrl->GetGilcKeyList(GilcKList);

        for(int j=0; j<GilcKList.GetSize(); j++)
        {
            T_GILC_K GilcK = GilcKList.GetAt(j);
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
            if(!pForceCtrl->Get_Gilc(EPairK, GilcK, nForType, bCalcStss, GildD.nType, strLcomType, aForceLcase, aStressLcase, aStssLcase,
                aForceP1, aStressP1, aForceP2, aStressP2)) continue;

            for(int k=0; k<aStressLcase.GetSize(); k++)
            {
                _DGN_STRS_CRC &StressLcase = aStressLcase.GetAt(k);

                for(int l=0; l<2; l++)
                {
                    Write_Command(_T("E_STR"));
                    Write_Int(ElemK);
                    Write_Int(l+1);
                    Write_String(strLcomType);
                    if(aForceLcase.GetSize()==1)  Write_Int(0);
                    else                          Write_Int(k+1);
                    Write_Double(StressLcase.dStop[l]);
                    Write_Double(StressLcase.dSbot[l]);
                    Write_Double(StressLcase.dStl[l]);
                    Write_Double(StressLcase.dSbl[l]);
                    Write_Double(StressLcase.dStr[l]);
                    Write_Double(StressLcase.dSbr[l]);
                }
            }
        }
    }
    return TRUE;
}

BOOL CCRCExportFile::Write_BtssForceData()
{
    // Add by ZINU.('06.02.16). Check Bridge Direction.
    if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

    CMap<T_ELEM_K,T_ELEM_K, T_ELEM_K, T_ELEM_K> PostElemMap; PostElemMap.RemoveAll();
    for(int iElem=0; iElem<m_PSCBeamPostD.PElemKList.GetSize(); iElem++)
    {
        T_ELEM_K ElemK = m_PSCBeamPostD.PElemKList.GetAt(iElem);
        PostElemMap.SetAt(ElemK,ElemK);
    }

    T_LCOM_D LcasD;
    T_BTMP_K BtmpK; T_BTMP_D BtmpD;
    POSITION pos = m_pDoc->m_pAttrCtrl->GetStartBtmp();
    while (pos) 
    {
        BtmpD.Initialize();
        m_pDoc->m_pAttrCtrl->GetNextBtmp(pos, BtmpK, BtmpD);

        UINT ElemK = BtmpK.key.entity;
        UINT rElemK = 0;
        if(!PostElemMap.Lookup(ElemK, rElemK)) continue;

        T_STLD_K StldK = BtmpD.LoadCaseKey;
        T_STLD_D StldD; StldD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetStld(StldK, StldD)) continue;

        LcasD.Initialize();
        if(!m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_STATIC,StldK,LcasD))	continue;
        m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcasD);

        T_BTSS_D BtssD; BtssD.Initialize();
        m_pDoc->m_pPostCtrl->GetBtssNew(ElemK, &BtssD, NULL, NULL);

        Write_Command(_T("E_BTMP"));
        Write_Int(ElemK);
        // Change by Jaeoh. (06.03.14)  LoadCaseName -> LoadCaseType 이중환 CJ님.
        Write_String(StldD.LoadCaseType);
        Write_Double(BtssD.dN);
        Write_Double(BtssD.dMy);
        Write_Double(BtssD.dMz);
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_LcomForceData()
{
    //CCRCForceCtrl ForceCtrl;
    CCRCForceCtrl* pForceCtrl = m_DataCtrl.Get_ForceCtrlPointer();

    int iLcomNum = m_DataCtrl.Get_DgnLcomNum();
    for(int i=0; i<m_PSCBeamPostD.PElemKList.GetSize(); i++)
    {
        T_ELEM_K ElemK = m_PSCBeamPostD.PElemKList.GetAt(i);
        ElemPairK EPairK(ElemK, EN_EL_BEAM);

        ADGNFORCE aForceLcom;
        if(!pForceCtrl->Get_LcomDataForDesign(EPairK, 0, FALSE, TRUE, aForceLcom, iLcomNum)) continue;

        for(int j=0; j<aForceLcom.GetSize(); j++)
        {
            _DGN_FORC_CRC ForceLcom = aForceLcom.GetAt(j);
            int iDgnLcomNo = j+1;
            CString strOrgLcomNa = _T("");
            int iMaxMinType = 0;
            if(!pForceCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iMaxMinType)) continue;
            int iKind = pForceCtrl->Get_LcomKind(iDgnLcomNo);
            for(int k=0; k<2 ; k++)
            {
                Write_Command(_T("E_COMF"));
                Write_Int(ElemK);
                Write_Int(k+1);
                Write_Int(iDgnLcomNo);
                Write_String(strOrgLcomNa);
                Write_Int(iKind);
                Write_Int(iMaxMinType);
                Write_Double(ForceLcom.dFxx[k]);
                Write_Double(ForceLcom.dFyy[k]);
                Write_Double(ForceLcom.dFzz[k]);
                Write_Double(ForceLcom.dMux[k]);
                Write_Double(ForceLcom.dMuy[k]);
                Write_Double(ForceLcom.dMuz[k]);
            }
        }
    }

#ifdef _DEBUG
    // Add by ZINU.('04.05.05). For Testing.
    for(int i=0; i<m_PSCBeamPostD.PElemKList.GetSize(); i++)
    {
        T_ELEM_K ElemK = m_PSCBeamPostD.PElemKList.GetAt(i);
        ElemPairK EPairK(ElemK, EN_EL_BEAM);

        ADGNFORCE aForceLcom;
        if(!pForceCtrl->Get_LcomDataForDesign(EPairK, 0, TRUE, TRUE, aForceLcom, iLcomNum)) continue;	// TRUE=Include Tendon Primary.

        for(int j=0; j<aForceLcom.GetSize(); j++)
        {
            _DGN_FORC_CRC ForceLcom = aForceLcom.GetAt(j);
            int iDgnLcomNo = j+1;
            CString strOrgLcomNa = _T("");
            int iMaxMinType = 0;
            if(!pForceCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iMaxMinType)) continue;
            int iKind = pForceCtrl->Get_LcomKind(iDgnLcomNo);
            for(int k=0; k<2 ; k++)
            {
                Write_Command(_T("E_COMF1"));
                Write_Int(ElemK);
                Write_Int(k+1);
                Write_Int(iDgnLcomNo);
                Write_String(strOrgLcomNa);
                Write_Int(iKind);
                Write_Int(iMaxMinType);
                Write_Double(ForceLcom.dFxx[k]);
                Write_Double(ForceLcom.dFyy[k]);
                Write_Double(ForceLcom.dFzz[k]);
                Write_Double(ForceLcom.dMux[k]);
                Write_Double(ForceLcom.dMuy[k]);
                Write_Double(ForceLcom.dMuz[k]);
            }
        }
    }
#endif

    return TRUE;
}

BOOL CCRCExportFile::Write_SectType(T_SECT_K SectK, T_SECT_D& SectD)
{
    T_SECT_SECTION_D& SectionD = SectD.SectBefore;
    CString strCmd = _T("");
    if(SectionD.Shape==D_SECT_SHAPE_PSC_1CELL)
    {
        strCmd = _T("S_T1CEL");

        T_SECT_D_PSC_1CELL SectI; SectI.Set(SectD, TRUE, TRUE);
        Write_Command(strCmd);
        Write_Int(SectK);
        Write_String(SectD.SName);
        Write_Int(1);
        Write_Int((SectI.JO1 ? 1 : 0)); Write_Int((SectI.JO2 ? 1 : 0)); Write_Int((SectI.JO3 ? 1 : 0));
        Write_Int((SectI.JI1 ? 1 : 0)); Write_Int((SectI.JI2 ? 1 : 0)); Write_Int((SectI.JI3 ? 1 : 0)); Write_Int((SectI.JI4 ? 1 : 0)); Write_Int((SectI.JI5 ? 1 : 0));

        T_SECT_D_PSC_1CELL SectJ;
        if(CDBLib::IsSectI(FALSE, SectD.nStype))	SectJ.Set(SectD, TRUE, TRUE);
        else																			SectJ.Set(SectD, TRUE, FALSE);
        Write_Command(strCmd, TRUE);
        Write_NullInt();
        Write_NullString();
        Write_Int(2);
        Write_Int((SectJ.JO1 ? 1 : 0)); Write_Int((SectJ.JO2 ? 1 : 0)); Write_Int((SectJ.JO3 ? 1 : 0));
        Write_Int((SectJ.JI1 ? 1 : 0)); Write_Int((SectJ.JI2 ? 1 : 0)); Write_Int((SectJ.JI3 ? 1 : 0)); Write_Int((SectJ.JI4 ? 1 : 0)); Write_Int((SectJ.JI5 ? 1 : 0));
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_2CELL)
    {
        strCmd = _T("S_T2CEL");

        T_SECT_D_PSC_2CELL SectI; SectI.Set(SectD, TRUE, TRUE);
        Write_Command(strCmd);
        Write_Int(SectK);
        Write_String(SectD.SName);
        Write_Int(1);
        Write_Int((SectI.JO1 ? 1 : 0)); Write_Int((SectI.JO2 ? 1 : 0)); Write_Int((SectI.JO3 ? 1 : 0));
        Write_Int((SectI.JI1 ? 1 : 0)); Write_Int((SectI.JI2 ? 1 : 0)); Write_Int((SectI.JI3 ? 1 : 0)); Write_Int((SectI.JI4 ? 1 : 0)); Write_Int((SectI.JI5 ? 1 : 0));

        T_SECT_D_PSC_2CELL SectJ; 
        if(CDBLib::IsSectI(FALSE, SectD.nStype))	SectJ.Set(SectD, TRUE, TRUE);
        else																			SectJ.Set(SectD, TRUE, FALSE);
        Write_Command(strCmd, TRUE);
        Write_NullInt();
        Write_NullString();
        Write_Int(2);
        Write_Int((SectJ.JO1 ? 1 : 0)); Write_Int((SectJ.JO2 ? 1 : 0)); Write_Int((SectJ.JO3 ? 1 : 0));
        Write_Int((SectJ.JI1 ? 1 : 0)); Write_Int((SectJ.JI2 ? 1 : 0)); Write_Int((SectJ.JI3 ? 1 : 0)); Write_Int((SectJ.JI4 ? 1 : 0)); Write_Int((SectJ.JI5 ? 1 : 0));
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_3CELL)
    {
        strCmd = _T("S_T3CEL");

        T_SECT_D_PSC_3CELL SectI; SectI.Set(SectD, TRUE, TRUE);
        Write_Command(strCmd);
        Write_Int(SectK);
        Write_String(SectD.SName);
        Write_Int(1);
        Write_Int((SectI.JO1 ? 1 : 0));  Write_Int((SectI.JO2 ? 1 : 0));
        Write_Int((SectI.JI1 ? 1 : 0));  Write_Int((SectI.JI2 ? 1 : 0));  Write_Int((SectI.JI3 ? 1 : 0)); Write_Int((SectI.JI4 ? 1 : 0)); Write_Int((SectI.JI5 ? 1 : 0));
        Write_Int((SectI.JI6 ? 1 : 0));  Write_Int((SectI.JI7 ? 1 : 0));  Write_Int((SectI.JI8 ? 1 : 0)); Write_Int((SectI.JI9 ? 1 : 0)); Write_Int((SectI.JI10 ? 1 : 0));
        Write_Int((SectI.JI11 ? 1 : 0));

        T_SECT_D_PSC_3CELL SectJ; 
        if(CDBLib::IsSectI(FALSE, SectD.nStype))	SectJ.Set(SectD, TRUE, TRUE);
        else																			SectJ.Set(SectD, TRUE, FALSE);
        Write_Command(strCmd, TRUE);
        Write_NullInt();
        Write_NullString();
        Write_Int(2);
        Write_Int((SectJ.JO1 ? 1 : 0));  Write_Int((SectJ.JO2 ? 1 : 0));
        Write_Int((SectJ.JI1 ? 1 : 0));  Write_Int((SectJ.JI2 ? 1 : 0));  Write_Int((SectJ.JI3 ? 1 : 0)); Write_Int((SectJ.JI4 ? 1 : 0)); Write_Int((SectJ.JI5 ? 1 : 0));
        Write_Int((SectJ.JI6 ? 1 : 0));  Write_Int((SectJ.JI7 ? 1 : 0));  Write_Int((SectJ.JI8 ? 1 : 0)); Write_Int((SectJ.JI9 ? 1 : 0)); Write_Int((SectJ.JI10 ? 1 : 0));
        Write_Int((SectJ.JI11 ? 1 : 0));
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_4CELL)
    {
        strCmd = _T("S_TNCEL");

        T_SECT_D_PSC_NCELL SectI; SectI.Set(SectD, TRUE, TRUE);
        Write_Command(strCmd);
        Write_Int(SectK);
        Write_String(SectD.SName);
        Write_Int(1);
        Write_Int(SectI.nType);
        Write_Int(SectI.nNumberOfCells);

        T_SECT_D_PSC_NCELL SectJ; 
        if(CDBLib::IsSectI(FALSE, SectD.nStype))	SectJ.Set(SectD, TRUE, TRUE);
        else																			SectJ.Set(SectD, TRUE, FALSE);
        Write_Command(strCmd, TRUE);
        Write_NullInt();
        Write_NullString();
        Write_Int(2);
        Write_Int(SectJ.nType);
        Write_Int(SectJ.nNumberOfCells); //nCell = nGirder+1
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_HALF)
    {
        strCmd = _T("S_TPSCH");

        T_SECT_D_PSC_HALF SectI; SectI.Set(SectD, TRUE, TRUE);
        Write_Command(strCmd);
        Write_Int(SectK);
        Write_String(SectD.SName);
        Write_Int(1);
        Write_Int(SectI.nCellType);
        Write_Int(SectI.nCellShape);
        Write_Int((SectI.JO1 ? 1 : 0)); Write_Int((SectI.JO2 ? 1 : 0)); Write_Int((SectI.JO3 ? 1 : 0));
        Write_Int((SectI.JI1 ? 1 : 0)); Write_Int((SectI.JI2 ? 1 : 0)); Write_Int((SectI.JI3 ? 1 : 0)); Write_Int((SectI.JI4 ? 1 : 0)); Write_Int((SectI.JI5 ? 1 : 0));

        T_SECT_D_PSC_HALF SectJ; 
        if(CDBLib::IsSectI(FALSE, SectD.nStype))	SectJ.Set(SectD, TRUE, TRUE);
        else																			SectJ.Set(SectD, TRUE, FALSE);
        Write_Command(strCmd, TRUE);
        Write_NullInt();
        Write_NullString();
        Write_Int(2);
        Write_Int(SectJ.nCellType);
        Write_Int(SectJ.nCellShape);
        Write_Int((SectJ.JO1 ? 1 : 0)); Write_Int((SectJ.JO2 ? 1 : 0)); Write_Int((SectJ.JO3 ? 1 : 0));
        Write_Int((SectJ.JI1 ? 1 : 0)); Write_Int((SectJ.JI2 ? 1 : 0)); Write_Int((SectJ.JI3 ? 1 : 0)); Write_Int((SectJ.JI4 ? 1 : 0)); Write_Int((SectJ.JI5 ? 1 : 0));
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_MID)
    {
        strCmd = _T("S_TPSCM");

        T_SECT_D_PSC_MID SectI; SectI.Set(SectD, TRUE, TRUE);
        Write_Command(strCmd);
        Write_Int(SectK);
        Write_String(SectD.SName);
        Write_Int(1);
        Write_Int((SectI.bSymmetric ? 1 : 0));
        Write_Int(SectI.nLeftShape);
        Write_Int(SectI.nRightShape);
        Write_Int((SectI.J1 ? 1 : 0));  
        Write_Int((SectI.JL1 ? 1 : 0));  Write_Int((SectI.JL2 ? 1 : 0));  Write_Int((SectI.JL3 ? 1 : 0));  Write_Int((SectI.JL4 ? 1 : 0));
        Write_Int((SectI.JR1 ? 1 : 0));  Write_Int((SectI.JR2 ? 1 : 0));  Write_Int((SectI.JR3 ? 1 : 0));  Write_Int((SectI.JR4 ? 1 : 0));

        T_SECT_D_PSC_MID SectJ; 
        if(CDBLib::IsSectI(FALSE, SectD.nStype))	SectJ.Set(SectD, TRUE, TRUE);
        else																			SectJ.Set(SectD, TRUE, FALSE);
        Write_Command(strCmd, TRUE);
        Write_NullInt();
        Write_NullString();
        Write_Int(2);
        Write_Int((SectJ.bSymmetric ? 1 : 0));
        Write_Int(SectJ.nLeftShape);
        Write_Int(SectJ.nRightShape);
        Write_Int((SectJ.J1 ? 1 : 0));  
        Write_Int((SectJ.JL1 ? 1 : 0));  Write_Int((SectJ.JL2 ? 1 : 0));  Write_Int((SectJ.JL3 ? 1 : 0));  Write_Int((SectJ.JL4 ? 1 : 0));
        Write_Int((SectJ.JR1 ? 1 : 0));  Write_Int((SectJ.JR2 ? 1 : 0));  Write_Int((SectJ.JR3 ? 1 : 0));  Write_Int((SectJ.JR4 ? 1 : 0));
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_T)
    {
        strCmd = _T("S_TPSCT");

        T_SECT_D_PSC_T SectI; SectI.Set(SectD, TRUE, TRUE);
        Write_Command(strCmd);
        Write_Int(SectK);
        Write_String(SectD.SName);
        Write_Int(1);
        Write_Int((SectI.bSymmetric ? 1 : 0));
        Write_Int((SectI.J1 ? 1 : 0));  
        Write_Int((SectI.JL1 ? 1 : 0));  Write_Int((SectI.JL2 ? 1 : 0));  Write_Int((SectI.JL3 ? 1 : 0));  Write_Int((SectI.JL4 ? 1 : 0));
        Write_Int((SectI.JR1 ? 1 : 0));  Write_Int((SectI.JR2 ? 1 : 0));  Write_Int((SectI.JR3 ? 1 : 0));  Write_Int((SectI.JR4 ? 1 : 0));

        T_SECT_D_PSC_T SectJ; 
        if(CDBLib::IsSectI(FALSE, SectD.nStype))	SectJ.Set(SectD, TRUE, TRUE);
        else																			SectJ.Set(SectD, TRUE, FALSE);
        Write_Command(strCmd, TRUE);
        Write_NullInt();
        Write_NullString();
        Write_Int(2);
        Write_Int((SectJ.bSymmetric ? 1 : 0));
        Write_Int((SectJ.J1 ? 1 : 0));  
        Write_Int((SectJ.JL1 ? 1 : 0));  Write_Int((SectJ.JL2 ? 1 : 0));  Write_Int((SectJ.JL3 ? 1 : 0));  Write_Int((SectJ.JL4 ? 1 : 0));
        Write_Int((SectJ.JR1 ? 1 : 0));  Write_Int((SectJ.JR2 ? 1 : 0));  Write_Int((SectJ.JR3 ? 1 : 0));  Write_Int((SectJ.JR4 ? 1 : 0));
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_BOX)
    {
        strCmd = _T("S_TPSCP");

        T_SECT_D_PSC_PLAT SectI; SectI.Set(SectD, TRUE, TRUE);
        Write_Command(strCmd);
        Write_Int(SectK);
        Write_String(SectD.SName);
        Write_Int(1);
        Write_Int(SectI.nCellType);
        if(SectI.nCellType==0)//Half
        {
            Write_Int(SectI.nCellShape);
            Write_Int(0);
        }
        else if(SectI.nCellType==1)//1Cell
        {
            Write_Int(0);
            Write_Int(SectI.nCellShape);
        }
        else
        {
            Write_Int(0);
            Write_Int(0);
        }
        Write_Int((SectI.bSymmetric ? 1 : 0));
        Write_Int((SectI.JL1 ? 1 : 0));  Write_Int((SectI.JR1 ? 1 : 0));

        T_SECT_D_PSC_PLAT SectJ; 
        if(CDBLib::IsSectI(FALSE, SectD.nStype))	SectJ.Set(SectD, TRUE, TRUE);
        else																			SectJ.Set(SectD, TRUE, FALSE);
        Write_Command(strCmd, TRUE);
        Write_NullInt();
        Write_NullString();
        Write_Int(2);
        Write_Int(SectJ.nCellType);
        if(SectJ.nCellType==0)//Half
        {
            Write_Int(SectJ.nCellShape);
            Write_Int(0);
        }
        else if(SectJ.nCellType==1)//1Cell
        {
            Write_Int(0);
            Write_Int(SectJ.nCellShape);
        }
        else
        {
            Write_Int(0);
            Write_Int(0);
        }
        Write_Int((SectJ.bSymmetric ? 1 : 0));
        Write_Int((SectJ.JL1 ? 1 : 0));  Write_Int((SectJ.JR1 ? 1 : 0));
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_I)
    {
        strCmd = _T("S_TPSCI");

        T_SECT_D_PSC_I SectI; SectI.Set(SectD, TRUE, TRUE);
        Write_Command(strCmd);
        Write_Int(SectK);
        Write_String(SectD.SName);
        Write_Int(1);
        Write_Int((SectI.bSymmetric ? 1 : 0));
        Write_Int((SectI.J1 ? 1 : 0));  
        Write_Int((SectI.JL1 ? 1 : 0));  Write_Int((SectI.JL2 ? 1 : 0));  Write_Int((SectI.JL3 ? 1 : 0));  Write_Int((SectI.JL4 ? 1 : 0));
        Write_Int((SectI.JR1 ? 1 : 0));  Write_Int((SectI.JR2 ? 1 : 0));  Write_Int((SectI.JR3 ? 1 : 0));  Write_Int((SectI.JR4 ? 1 : 0));

        T_SECT_D_PSC_I SectJ; 
        if(CDBLib::IsSectI(FALSE, SectD.nStype))	SectJ.Set(SectD, TRUE, TRUE);
        else																			SectJ.Set(SectD, TRUE, FALSE);
        Write_Command(strCmd, TRUE);
        Write_NullInt();
        Write_NullString();
        Write_Int(2);
        Write_Int((SectJ.bSymmetric ? 1 : 0));
        Write_Int((SectJ.J1 ? 1 : 0));  
        Write_Int((SectJ.JL1 ? 1 : 0));  Write_Int((SectJ.JL2 ? 1 : 0));  Write_Int((SectJ.JL3 ? 1 : 0));  Write_Int((SectJ.JL4 ? 1 : 0));
        Write_Int((SectJ.JR1 ? 1 : 0));  Write_Int((SectJ.JR2 ? 1 : 0));  Write_Int((SectJ.JR3 ? 1 : 0));  Write_Int((SectJ.JR4 ? 1 : 0));
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_NCEL2)
    {
        strCmd = _T("S_TNCE2");

        T_SECT_D_PSC_NCEL2 SectI; SectI.Set(SectD, TRUE, TRUE);
        Write_Command(strCmd);
        Write_Int(SectK);
        Write_String(SectD.SName);
        Write_Int(1);
        Write_Int(SectI.nCellType);
        Write_Int((SectI.bSmallHole ? 1 : 0));
        Write_Int((SectI.bSymmetric ? 1 : 0));
        Write_Int(SectI.nNumberOfCells);
        Write_Double(SectI.dSlabWidth);
        Write_Int((SectI.JO ? 1 : 0));
        Write_Int((SectI.JI ? 1 : 0));

        T_SECT_D_PSC_NCEL2 SectJ; 
        if(CDBLib::IsSectI(FALSE, SectD.nStype))	SectJ.Set(SectD, TRUE, TRUE);
        else																			SectJ.Set(SectD, TRUE, FALSE);
        Write_Command(strCmd, TRUE);
        Write_NullInt();
        Write_NullString();
        Write_Int(2);
        Write_Int(SectJ.nCellType);
        Write_Int((SectJ.bSmallHole ? 1 : 0));
        Write_Int((SectJ.bSymmetric ? 1 : 0));
        Write_Int(SectJ.nNumberOfCells);
        Write_Double(SectJ.dSlabWidth);
        Write_Int((SectJ.JO ? 1 : 0));
        Write_Int((SectJ.JI ? 1 : 0));
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_VALUE)
    {
        strCmd = _T("S_TPSCV");

        int nCell = SectD.SectBefore.SectI.aInnerPolygon.GetSize();

        Write_Command(strCmd);
        Write_Int(SectK);
        Write_String(SectD.SName);
        Write_Int(1);
        Write_Int(nCell);

        if(CDBLib::IsSectI(FALSE, SectD.nStype)) 
            nCell = SectD.SectBefore.SectI.aInnerPolygon.GetSize();
        else
            nCell = SectD.SectBefore.SectJ.aInnerPolygon.GetSize();

        Write_Command(strCmd, TRUE);
        Write_NullInt();
        Write_NullString();
        Write_Int(2);
        Write_Int(nCell);
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_CMPWEB) //2005.03.03 추가 By RSH
    {
        strCmd = _T("S_TPCMP");
        double dEFFH = 0.0, dEFD=0.0, dLRF=0.0;

        T_SECT_D_PSC_CMPWEB SectI; SectI.Set(SectD, TRUE, TRUE);
        CSectUtil::GetPscCmpWebEffH(&(SectD.SectBefore.SectI), SectI.bHunch, dEFFH);
        dEFD = SectionD.SectI.Design.TanA;
        dLRF = SectionD.SectI.Design.Beta;

        Write_Command(strCmd);
        Write_Int(SectK);
        Write_String(SectD.SName);
        Write_Int(1);
        Write_Double(SectI.CMP.a);
        Write_Double(SectI.CMP.b);
        Write_Double(SectI.CMP.h);
        Write_Double(SectI.CMP.t);
        Write_Double(dEFFH);
        Write_Double(dEFD); //재단고정도
        Write_Double(dLRF); //길이감소율
        Write_Int((SectI.bSymmetric ? 1 : 0));
        Write_Int((SectI.bHunch ? 1 : 0));
        Write_Double(SectionD.Matl_Elast);
        Write_Double(SectionD.Matl_Density);
        Write_Double(SectionD.Matl_Poisson_S);
        Write_Double(SectionD.Matl_Poisson_C);

        T_SECT_D_PSC_CMPWEB SectJ;
        if(CDBLib::IsSectI(FALSE, SectD.nStype))	
        {
            SectJ.Set(SectD, TRUE, TRUE);
            CSectUtil::GetPscCmpWebEffH(&(SectD.SectBefore.SectI), SectI.bHunch, dEFFH);
            dEFD = SectionD.SectI.Design.TanA;
            dLRF = SectionD.SectI.Design.Beta;
        }
        else
        {
            SectJ.Set(SectD, TRUE, FALSE);
            CSectUtil::GetPscCmpWebEffH(&(SectD.SectBefore.SectJ), SectJ.bHunch, dEFFH);
            dEFD = SectionD.SectJ.Design.TanA;
            dLRF = SectionD.SectJ.Design.Beta;
        }
        Write_Command(strCmd, TRUE);
        Write_NullInt();
        Write_NullString();
        Write_Int(2);
        Write_Double(SectJ.CMP.a);
        Write_Double(SectJ.CMP.b);
        Write_Double(SectJ.CMP.h);
        Write_Double(SectJ.CMP.t);
        Write_Double(dEFFH);
        Write_Double(dEFD); //재단고정도
        Write_Double(dLRF); //길이감소율
        Write_Int((SectJ.bSymmetric ? 1 : 0));
        Write_Int((SectJ.bHunch ? 1 : 0));
        Write_Double(SectionD.Matl_Elast);
        Write_Double(SectionD.Matl_Density);
        Write_Double(SectionD.Matl_Poisson_S);
        Write_Double(SectionD.Matl_Poisson_C);
    }
    else 
    {
        ASSERT(0);
        return FALSE;
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_SectSize(T_SECT_K SectK, T_SECT_D& SectD, BOOL bPosI)
{
    T_SECT_SECTION_D& SectionD = SectD.SectBefore;

    CString strCmd = _T("");
    CArray<double, double> arDimSize; arDimSize.RemoveAll();
    CArray<double, double> arLDimSize; arLDimSize.RemoveAll();
    CArray<double, double> arMDimSize; arMDimSize.RemoveAll();
    CArray<double, double> arRDimSize; arRDimSize.RemoveAll();

    if(SectionD.Shape==D_SECT_SHAPE_PSC_1CELL)
    {
        strCmd = _T("S_1CEL");

        T_SECT_D_PSC_1CELL PscD; 
        PscD.Set(SectD, TRUE, bPosI);

        if(CDBLib::IsSectI(bPosI, SectD.nStype))	PscD.Set(SectD, TRUE, TRUE);
        else																			PscD.Set(SectD, TRUE, FALSE);

        arDimSize.Add(PscD.HO1);  arDimSize.Add(PscD.HO2); arDimSize.Add(PscD.HO21);  arDimSize.Add(PscD.HO22); arDimSize.Add(PscD.HO3); 
        arDimSize.Add(PscD.HO31); arDimSize.Add(PscD.BO1); arDimSize.Add(PscD.BO11);  arDimSize.Add(PscD.BO12); arDimSize.Add(PscD.BO2); 
        arDimSize.Add(PscD.BO21); arDimSize.Add(PscD.BO3); arDimSize.Add(PscD.HI1);   arDimSize.Add(PscD.HI2);  arDimSize.Add(PscD.HI21); 
        arDimSize.Add(PscD.HI22); arDimSize.Add(PscD.HI3); arDimSize.Add(PscD.HI31);  arDimSize.Add(PscD.HI4);  arDimSize.Add(PscD.HI41);
        arDimSize.Add(PscD.HI42); arDimSize.Add(PscD.HI5); arDimSize.Add(PscD.BI1);   arDimSize.Add(PscD.BI11); arDimSize.Add(PscD.BI12);
        arDimSize.Add(PscD.BI21); arDimSize.Add(PscD.BI3); arDimSize.Add(PscD.BI31);  arDimSize.Add(PscD.BI32); 
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_2CELL)
    {
        strCmd = _T("S_2CEL");

        T_SECT_D_PSC_2CELL PscD; 
        if(CDBLib::IsSectI(bPosI, SectD.nStype))	PscD.Set(SectD, TRUE, TRUE);
        else																			PscD.Set(SectD, TRUE, FALSE);

        arDimSize.Add(PscD.HO1);  arDimSize.Add(PscD.HO2); arDimSize.Add(PscD.HO21);  arDimSize.Add(PscD.HO22); arDimSize.Add(PscD.HO3); 
        arDimSize.Add(PscD.HO31); arDimSize.Add(PscD.BO1); arDimSize.Add(PscD.BO11);  arDimSize.Add(PscD.BO12); arDimSize.Add(PscD.BO2); 
        arDimSize.Add(PscD.BO21); arDimSize.Add(PscD.BO3); arDimSize.Add(PscD.HI1);   arDimSize.Add(PscD.HI2);  arDimSize.Add(PscD.HI21); 
        arDimSize.Add(PscD.HI22); arDimSize.Add(PscD.HI3); arDimSize.Add(PscD.HI31);  arDimSize.Add(PscD.HI4);  arDimSize.Add(PscD.HI41);
        arDimSize.Add(PscD.HI42); arDimSize.Add(PscD.HI5); arDimSize.Add(PscD.BI1);   arDimSize.Add(PscD.BI11); arDimSize.Add(PscD.BI12);
        arDimSize.Add(PscD.BI21); arDimSize.Add(PscD.BI3); arDimSize.Add(PscD.BI31);  arDimSize.Add(PscD.BI32); arDimSize.Add(PscD.BI4);
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_3CELL)
    {
        strCmd = _T("S_3CEL");

        T_SECT_D_PSC_3CELL PscD; 
        if(CDBLib::IsSectI(bPosI, SectD.nStype))	PscD.Set(SectD, TRUE, TRUE);
        else																			PscD.Set(SectD, TRUE, FALSE);

        arDimSize.Add(PscD.HI1);  arDimSize.Add(PscD.HI2); arDimSize.Add(PscD.HI3);  arDimSize.Add(PscD.HI4); arDimSize.Add(PscD.HI5);
        arDimSize.Add(PscD.HI6); arDimSize.Add(PscD.HI7); arDimSize.Add(PscD.HI8);  arDimSize.Add(PscD.HI9); arDimSize.Add(PscD.HI10);
        arDimSize.Add(PscD.BI1); arDimSize.Add(PscD.BI2); arDimSize.Add(PscD.BI3);   arDimSize.Add(PscD.BI4);  arDimSize.Add(PscD.BI5);
        arDimSize.Add(PscD.BI6); arDimSize.Add(PscD.BI7); arDimSize.Add(PscD.HO1);  arDimSize.Add(PscD.HO2);  arDimSize.Add(PscD.BO1);
        arDimSize.Add(PscD.BO2); arDimSize.Add(PscD.BO3); arDimSize.Add(PscD.HO21);   arDimSize.Add(PscD.HO22); arDimSize.Add(PscD.HI21);
        arDimSize.Add(PscD.HI22); arDimSize.Add(PscD.HI41); arDimSize.Add(PscD.HI42);  arDimSize.Add(PscD.HI61); arDimSize.Add(PscD.HI62);
        arDimSize.Add(PscD.HI63); arDimSize.Add(PscD.HI91); arDimSize.Add(PscD.HI92);  arDimSize.Add(PscD.HI81); arDimSize.Add(PscD.HI82);
        arDimSize.Add(PscD.BO21); arDimSize.Add(PscD.BO22); arDimSize.Add(PscD.BI11);  arDimSize.Add(PscD.BI12); arDimSize.Add(PscD.BI41);
        arDimSize.Add(PscD.BI42); arDimSize.Add(PscD.BI31); arDimSize.Add(PscD.BI32);  arDimSize.Add(PscD.BI33); arDimSize.Add(PscD.BI71);
        arDimSize.Add(PscD.BI72); arDimSize.Add(PscD.BI61); arDimSize.Add(PscD.BI62);
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_4CELL)
    {
        strCmd = _T("S_NCEL");

        T_SECT_D_PSC_NCELL PscD; 
        if(CDBLib::IsSectI(bPosI, SectD.nStype))	PscD.Set(SectD, TRUE, TRUE);
        else																			PscD.Set(SectD, TRUE, FALSE);

        arDimSize.Add(PscD.H1);  arDimSize.Add(PscD.H2); arDimSize.Add(PscD.H3);  arDimSize.Add(PscD.H4); arDimSize.Add(PscD.H5);
        arDimSize.Add(PscD.B1);  arDimSize.Add(PscD.B2); arDimSize.Add(PscD.B3);  arDimSize.Add(PscD.B4); arDimSize.Add(PscD.B5);
        arDimSize.Add(PscD.B6);  arDimSize.Add(PscD.B7); arDimSize.Add(PscD.B8);
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_HALF)
    {
        strCmd = _T("S_PSCH");

        T_SECT_D_PSC_HALF PscD; 
        if(CDBLib::IsSectI(bPosI, SectD.nStype))	PscD.Set(SectD, TRUE, TRUE);
        else																			PscD.Set(SectD, TRUE, FALSE);

        arDimSize.Add(PscD.HO1);  arDimSize.Add(PscD.HO2); arDimSize.Add(PscD.HO21);  arDimSize.Add(PscD.HO22); arDimSize.Add(PscD.HO3); 
        arDimSize.Add(PscD.HO31); arDimSize.Add(PscD.BO1); arDimSize.Add(PscD.BO11);  arDimSize.Add(PscD.BO12); arDimSize.Add(PscD.BO2); 
        arDimSize.Add(PscD.BO21); arDimSize.Add(PscD.BO3); arDimSize.Add(PscD.HI1);   arDimSize.Add(PscD.HI2);  arDimSize.Add(PscD.HI21); 
        arDimSize.Add(PscD.HI22); arDimSize.Add(PscD.HI3); arDimSize.Add(PscD.HI31);  arDimSize.Add(PscD.HI4);  arDimSize.Add(PscD.HI41);
        arDimSize.Add(PscD.HI42); arDimSize.Add(PscD.HI5); arDimSize.Add(PscD.BI1);   arDimSize.Add(PscD.BI11); arDimSize.Add(PscD.BI12);
        arDimSize.Add(PscD.BI21); arDimSize.Add(PscD.BI3); arDimSize.Add(PscD.BI31);  arDimSize.Add(PscD.BI32); 
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_MID)
    {
        strCmd = _T("S_PSCM");

        T_SECT_D_PSC_MID PscD; 
        if(CDBLib::IsSectI(bPosI, SectD.nStype))	PscD.Set(SectD, TRUE, TRUE);
        else																			PscD.Set(SectD, TRUE, FALSE);

        arDimSize.Add(PscD.H1 );  arDimSize.Add(PscD.HL1); arDimSize.Add(PscD.HL2);   arDimSize.Add(PscD.HL21); arDimSize.Add(PscD.HL22);
        arDimSize.Add(PscD.HL3);  arDimSize.Add(PscD.HL4); arDimSize.Add(PscD.HL41);  arDimSize.Add(PscD.HL42); arDimSize.Add(PscD.HL5);
        arDimSize.Add(PscD.BL1);  arDimSize.Add(PscD.BL2); arDimSize.Add(PscD.BL21);  arDimSize.Add(PscD.BL22); arDimSize.Add(PscD.BL41);
        arDimSize.Add(PscD.BL42); arDimSize.Add(PscD.HR1); arDimSize.Add(PscD.HR2);   arDimSize.Add(PscD.HR21); arDimSize.Add(PscD.HR22);
        arDimSize.Add(PscD.HR3);  arDimSize.Add(PscD.HR4); arDimSize.Add(PscD.HR41);  arDimSize.Add(PscD.HR42); arDimSize.Add(PscD.HR5);
        arDimSize.Add(PscD.BR1);  arDimSize.Add(PscD.BR2); arDimSize.Add(PscD.BR21);  arDimSize.Add(PscD.BR22); arDimSize.Add(PscD.BR41);
        arDimSize.Add(PscD.BR42);
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_T)
    {
        strCmd = _T("S_PSCT");

        T_SECT_D_PSC_T PscD; 
        if(CDBLib::IsSectI(bPosI, SectD.nStype))	PscD.Set(SectD, TRUE, TRUE);
        else																			PscD.Set(SectD, TRUE, FALSE);

        arDimSize.Add(PscD.H1 );  
        arDimSize.Add(PscD.HL1); arDimSize.Add(PscD.HL2);  arDimSize.Add(PscD.HL3);  
        arDimSize.Add(PscD.BL1); arDimSize.Add(PscD.BL2);  arDimSize.Add(PscD.BL3); arDimSize.Add(PscD.BL4);  
        arDimSize.Add(PscD.HL21); arDimSize.Add(PscD.HL22); arDimSize.Add(PscD.HL31);  arDimSize.Add(PscD.HL32);  
        arDimSize.Add(PscD.BL21); arDimSize.Add(PscD.BL22); arDimSize.Add(PscD.BL31);  arDimSize.Add(PscD.BL32);
        arDimSize.Add(PscD.HR1); arDimSize.Add(PscD.HR2);  arDimSize.Add(PscD.HR3);  
        arDimSize.Add(PscD.BR1); arDimSize.Add(PscD.BR2);  arDimSize.Add(PscD.BR3); arDimSize.Add(PscD.BR4);  
        arDimSize.Add(PscD.HR21); arDimSize.Add(PscD.HR22); arDimSize.Add(PscD.HR31);  arDimSize.Add(PscD.HR32);  
        arDimSize.Add(PscD.BR21); arDimSize.Add(PscD.BR22); arDimSize.Add(PscD.BR31);  arDimSize.Add(PscD.BR32);
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_BOX)
    {
        strCmd = _T("S_PSCP");
        T_SECT_D_PSC_PLAT PscD; 
        if(CDBLib::IsSectI(bPosI, SectD.nStype))	PscD.Set(SectD, TRUE, TRUE);
        else																			PscD.Set(SectD, TRUE, FALSE);

        arDimSize.Add(PscD.H1 ); arDimSize.Add(PscD.H2 ); 
        arDimSize.Add(PscD.HOL1); arDimSize.Add(PscD.HOL2); arDimSize.Add(PscD.HOL3);
        arDimSize.Add(PscD.BOL1); arDimSize.Add(PscD.BOL2); arDimSize.Add(PscD.BOL3);
        arDimSize.Add(PscD.HOL11); arDimSize.Add(PscD.BOL11); 
        arDimSize.Add(PscD.HIL1); arDimSize.Add(PscD.HIL2); 
        arDimSize.Add(PscD.BIL1); arDimSize.Add(PscD.BIL2); arDimSize.Add(PscD.BIL3); arDimSize.Add(PscD.BIL4);
        arDimSize.Add(PscD.HOR1); arDimSize.Add(PscD.HOR2); arDimSize.Add(PscD.HOR3);
        arDimSize.Add(PscD.BOR1); arDimSize.Add(PscD.BOR2); arDimSize.Add(PscD.BOR3);
        arDimSize.Add(PscD.HOR11); arDimSize.Add(PscD.BOR11); 
        arDimSize.Add(PscD.HIR1); arDimSize.Add(PscD.HIR2); 
        arDimSize.Add(PscD.BIR1); arDimSize.Add(PscD.BIR2); arDimSize.Add(PscD.BIR3); 
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_I)
    {
        strCmd = _T("S_PSCI");
        T_SECT_D_PSC_I PscD; 
        if(CDBLib::IsSectI(bPosI, SectD.nStype))	PscD.Set(SectD, TRUE, TRUE);
        else																			PscD.Set(SectD, TRUE, FALSE);

        arDimSize.Add(PscD.H1 );  
        arDimSize.Add(PscD.HL1); arDimSize.Add(PscD.HL2); arDimSize.Add(PscD.HL21); arDimSize.Add(PscD.HL22); 
        arDimSize.Add(PscD.HL3); arDimSize.Add(PscD.HL4); arDimSize.Add(PscD.HL41);  arDimSize.Add(PscD.HL42); arDimSize.Add(PscD.HL5); 
        arDimSize.Add(PscD.BL1); arDimSize.Add(PscD.BL2); arDimSize.Add(PscD.BL21); arDimSize.Add(PscD.BL22); 
        arDimSize.Add(PscD.BL4); arDimSize.Add(PscD.BL41);  arDimSize.Add(PscD.BL42);
        arDimSize.Add(PscD.HR1); arDimSize.Add(PscD.HR2); arDimSize.Add(PscD.HR21); arDimSize.Add(PscD.HR22); 
        arDimSize.Add(PscD.HR3); arDimSize.Add(PscD.HR4); arDimSize.Add(PscD.HR41);  arDimSize.Add(PscD.HR42); arDimSize.Add(PscD.HR5); 
        arDimSize.Add(PscD.BR1); arDimSize.Add(PscD.BR2); arDimSize.Add(PscD.BR21); arDimSize.Add(PscD.BR22); 
        arDimSize.Add(PscD.BR4); arDimSize.Add(PscD.BR41);  arDimSize.Add(PscD.BR42);
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_NCEL2)
    {
        T_SECT_D_PSC_NCEL2 PscD;
        if(CDBLib::IsSectI(bPosI, SectD.nStype))	PscD.Set(SectD, TRUE, TRUE);
        else																			PscD.Set(SectD, TRUE, FALSE);
        //Left
        arLDimSize.Add(PscD.Left.HO1); arLDimSize.Add(PscD.Left.HO2);  arLDimSize.Add(PscD.Left.HO21); arLDimSize.Add(PscD.Left.HO3); arLDimSize.Add(PscD.Left.HO4);
        arLDimSize.Add(PscD.Left.BO1); arLDimSize.Add(PscD.Left.BO11); arLDimSize.Add(PscD.Left.BO2);  arLDimSize.Add(PscD.Left.BO3); arLDimSize.Add(PscD.Left.BO4); arLDimSize.Add(PscD.Left.BO5);
        arLDimSize.Add(PscD.Left.HI1); arLDimSize.Add(PscD.Left.HI2);  arLDimSize.Add(PscD.Left.HI21); arLDimSize.Add(PscD.Left.HI3); arLDimSize.Add(PscD.Left.HI4); arLDimSize.Add(PscD.Left.HI5); arLDimSize.Add(PscD.Left.HI6);
        arLDimSize.Add(PscD.Left.R1);  arLDimSize.Add(PscD.Left.R2);
        arLDimSize.Add(PscD.Left.BI1); arLDimSize.Add(PscD.Left.BI2);  arLDimSize.Add(PscD.Left.BI21); arLDimSize.Add(PscD.Left.BI3); arLDimSize.Add(PscD.Left.BI4); arLDimSize.Add(PscD.Left.BI5); arLDimSize.Add(PscD.Left.BI6);
        //Middle
        arMDimSize.Add(PscD.Mid.BI7);  arMDimSize.Add(PscD.Mid.BI8);
        //Right
        arRDimSize.Add(PscD.Right.HO1); arRDimSize.Add(PscD.Right.HO2);  arRDimSize.Add(PscD.Right.HO21); arRDimSize.Add(PscD.Right.HO3); arRDimSize.Add(PscD.Right.HO4);
        arRDimSize.Add(PscD.Right.BO1); arRDimSize.Add(PscD.Right.BO11); arRDimSize.Add(PscD.Right.BO2);  arRDimSize.Add(PscD.Right.BO3); arRDimSize.Add(PscD.Right.BO4); arRDimSize.Add(PscD.Right.BO5);
        arRDimSize.Add(PscD.Right.HI1); arRDimSize.Add(PscD.Right.HI2);  arRDimSize.Add(PscD.Right.HI21); arRDimSize.Add(PscD.Right.HI3); arRDimSize.Add(PscD.Right.HI4); arRDimSize.Add(PscD.Right.HI5); arRDimSize.Add(PscD.Right.HI6);
        arRDimSize.Add(PscD.Right.R1);  arRDimSize.Add(PscD.Right.R2);
        arRDimSize.Add(PscD.Right.BI1); arRDimSize.Add(PscD.Right.BI2);  arRDimSize.Add(PscD.Right.BI21); arRDimSize.Add(PscD.Right.BI3); arRDimSize.Add(PscD.Right.BI4); arRDimSize.Add(PscD.Right.BI5); arRDimSize.Add(PscD.Right.BI6);
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_VALUE)
    {
        strCmd = _T("S_PSCV");

        if(CDBLib::IsSectI(bPosI, SectD.nStype))
        {
            arDimSize.Add(SectD.SectBefore.SectI.Size[0]);
            arDimSize.Add(SectD.SectBefore.SectI.Size[1]);
            arDimSize.Add(SectD.SectBefore.SectI.Size[2]);
            arDimSize.Add(SectD.SectBefore.SectI.Size[3]);
        }
        else
        {
            arDimSize.Add(SectD.SectBefore.SectJ.Size[0]);
            arDimSize.Add(SectD.SectBefore.SectJ.Size[1]);
            arDimSize.Add(SectD.SectBefore.SectJ.Size[2]);
            arDimSize.Add(SectD.SectBefore.SectJ.Size[3]);
        }
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_CMPWEB) //2005.03.03 추가 By RSH
    {
        T_SECT_D_PSC_CMPWEB PscD; 
        PscD.Set(SectD, TRUE, bPosI);

        if(CDBLib::IsSectI(bPosI, SectD.nStype))	PscD.Set(SectD, TRUE, TRUE);
        else																			PscD.Set(SectD, TRUE, FALSE);

        //Left
        arLDimSize.Add(PscD.Left.HO1);  arLDimSize.Add(PscD.Left.HO2); arLDimSize.Add(PscD.Left.HO3);  arLDimSize.Add(PscD.Left.HO4); arLDimSize.Add(PscD.Left.HO5); 
        arLDimSize.Add(PscD.Left.HO6);  arLDimSize.Add(PscD.Left.HO61); arLDimSize.Add(PscD.Left.BO1); arLDimSize.Add(PscD.Left.BO2);  arLDimSize.Add(PscD.Left.BO3); 
        arLDimSize.Add(PscD.Left.BO4);  arLDimSize.Add(PscD.Left.BO5); arLDimSize.Add(PscD.Left.BO6); arLDimSize.Add(PscD.Left.BO7);   arLDimSize.Add(PscD.Left.BO8);  
        arLDimSize.Add(PscD.Left.BO9);  arLDimSize.Add(PscD.Left.BO10); arLDimSize.Add(PscD.Left.BO11); arLDimSize.Add(PscD.Left.HI1);  arLDimSize.Add(PscD.Left.HI2);  
        arLDimSize.Add(PscD.Left.HI3);  arLDimSize.Add(PscD.Left.HI4); arLDimSize.Add(PscD.Left.HI5); arLDimSize.Add(PscD.Left.HI6);   arLDimSize.Add(PscD.Left.HI7);
        arLDimSize.Add(PscD.Left.HI8);  arLDimSize.Add(PscD.Left.BI1); arLDimSize.Add(PscD.Left.BI2); arLDimSize.Add(PscD.Left.BI3);  arLDimSize.Add(PscD.Left.BI4);  
        arLDimSize.Add(PscD.Left.BI5);

        //Right
        arRDimSize.Add(PscD.Right.HO1);  arRDimSize.Add(PscD.Right.HO2); arRDimSize.Add(PscD.Right.HO3);  arRDimSize.Add(PscD.Right.HO4); arRDimSize.Add(PscD.Right.HO5); 
        arRDimSize.Add(PscD.Right.HO6);  arRDimSize.Add(PscD.Right.HO61); arRDimSize.Add(PscD.Right.BO1); arRDimSize.Add(PscD.Right.BO2);  arRDimSize.Add(PscD.Right.BO3); 
        arRDimSize.Add(PscD.Right.BO4);  arRDimSize.Add(PscD.Right.BO5); arRDimSize.Add(PscD.Right.BO6); arRDimSize.Add(PscD.Right.BO7);  arRDimSize.Add(PscD.Right.BO8);  
        arRDimSize.Add(PscD.Right.BO9); arRDimSize.Add(PscD.Right.BO10); arRDimSize.Add(PscD.Right.BO11);  arRDimSize.Add(PscD.Right.HI1);  arRDimSize.Add(PscD.Right.HI2);  
        arRDimSize.Add(PscD.Right.HI3);  arRDimSize.Add(PscD.Right.HI4); arRDimSize.Add(PscD.Right.HI5); arRDimSize.Add(PscD.Right.HI6);   arRDimSize.Add(PscD.Right.HI7);
        arRDimSize.Add(PscD.Right.HI8);  arRDimSize.Add(PscD.Right.BI1); arRDimSize.Add(PscD.Right.BI2); arRDimSize.Add(PscD.Right.BI3);  arRDimSize.Add(PscD.Right.BI4);  
        arRDimSize.Add(PscD.Right.BI5);
    }
    else
    {
        ASSERT(0);
        return FALSE;
    }

    int iPosi = (bPosI ? 1 : 2);
    if(SectionD.Shape==D_SECT_SHAPE_PSC_NCEL2)
    {
        //Left Dimension
        strCmd = _T("S_NCE2L");
        Write_Command(strCmd);
        Write_Int(SectK);
        Write_Int(iPosi);
        for(int i=0; i<arLDimSize.GetSize(); i++)
        {
            double dDim = arLDimSize.GetAt(i);
            if(!Write_Double(dDim))
            {
                Write_Command(strCmd, TRUE);
                Write_NullInt();
                Write_NullInt();
                Write_Double(dDim);
            }
        }
        //Middle Dimension
        strCmd = _T("S_NCE2M");
        Write_Command(strCmd);
        Write_Int(SectK);
        Write_Int(iPosi);
        for(int i=0; i<arMDimSize.GetSize(); i++)
        {
            double dDim = arMDimSize.GetAt(i);
            if(!Write_Double(dDim))
            {
                Write_Command(strCmd, TRUE);
                Write_NullInt();
                Write_NullInt();
                Write_Double(dDim);
            }
        }
        //Middle Dimension
        strCmd = _T("S_NCE2R");
        Write_Command(strCmd);
        Write_Int(SectK);
        Write_Int(iPosi);
        for(int i=0; i<arRDimSize.GetSize(); i++)
        {
            double dDim = arRDimSize.GetAt(i);
            if(!Write_Double(dDim))
            {
                Write_Command(strCmd, TRUE);
                Write_NullInt();
                Write_NullInt();
                Write_Double(dDim);
            }
        }
    }
    else if(SectionD.Shape==D_SECT_SHAPE_PSC_CMPWEB) //2005.03.03 추가 By RSH
    {
        //Left Dimension
        strCmd = _T("S_CMPL");
        Write_Command(strCmd);
        Write_Int(SectK);
        Write_Int(iPosi);
        for(int i=0; i<arLDimSize.GetSize(); i++)
        {
            double dDim = arLDimSize.GetAt(i);
            if(!Write_Double(dDim))
            {
                Write_Command(strCmd, TRUE);
                Write_NullInt();
                Write_NullInt();
                Write_Double(dDim);
            }
        }
        //Right Dimension
        strCmd = _T("S_CMPR");
        Write_Command(strCmd);
        Write_Int(SectK);
        Write_Int(iPosi);
        for(int i=0; i<arRDimSize.GetSize(); i++)
        {
            double dDim = arRDimSize.GetAt(i);
            if(!Write_Double(dDim))
            {
                Write_Command(strCmd, TRUE);
                Write_NullInt();
                Write_NullInt();
                Write_Double(dDim);
            }
        }
    }
    else
    {
        //Dimension
        Write_Command(strCmd);
        Write_Int(SectK);
        Write_Int(iPosi);

        for(int i=0; i<arDimSize.GetSize(); i++)
        {
            double dDim = arDimSize.GetAt(i);
            if(!Write_Double(dDim))
            {
                Write_Command(strCmd, TRUE);
                Write_NullInt();
                Write_NullInt();
                Write_Double(dDim);
            }
        }
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_SectCoord(T_SECT_K SectK, T_SECT_D& SectD, BOOL bPosI)
{
    //BOOL GetPscCoordList(T_SECT_K SectK, BOOL bPosI, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, BOOL bUseExport=FALSE);
    ////////////////////////////
    // Get Sect Position.
    CArray<T_SPSC_COOR,T_SPSC_COOR> arOuter;
    CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*> arInner;
    BOOL bGetOK = CSectUtil::GetPscCoordList(TRUE,SectK,bPosI,arOuter,arInner);

    if(bGetOK)
    {
        for(int i=0; i<arOuter.GetSize(); i++)
        {
            T_SPSC_COOR OutCell = arOuter.GetAt(i);
            if(OutCell.arSpscPont[0].GetSize() != OutCell.arSpscPont[1].GetSize())
            {
                ASSERT(0); continue;
            }

            if(bPosI) Write_Command(_T("S_OUTI"));
            else      Write_Command(_T("S_OUTJ"));
            Write_Int(SectK);

            for(int j=0; j<=OutCell.arSpscPont[0].GetSize(); j++)
            {
                double dy=0, dz=0;
                if(j == OutCell.arSpscPont[0].GetSize())
                {
                    dy = OutCell.arSpscPont[0].GetAt(0);
                    dz = OutCell.arSpscPont[1].GetAt(0);
                }
                else
                {
                    dy = OutCell.arSpscPont[0].GetAt(j);
                    dz = OutCell.arSpscPont[1].GetAt(j);
                }
                if(Write_Enable(32))
                {
                    Write_Double(dy);
                    Write_Double(dz);
                }
                else
                {
                    if(bPosI) Write_Command(_T("S_OUTI"), TRUE);
                    else      Write_Command(_T("S_OUTJ"), TRUE);
                    Write_NullInt();
                    Write_Double(dy);
                    Write_Double(dz);
                }
            }
        }
        for(int i=0; i<arInner.GetSize(); i++)
        {
            T_SPSC_COOR_AR* apSpscCoor = arInner[i];
            int iCell = 0;
            for(int j=0; j<apSpscCoor->GetSize(); j++)
            {
                T_SPSC_COOR InCell = apSpscCoor->GetAt(j);

                if(InCell.arSpscPont[0].GetSize() != InCell.arSpscPont[1].GetSize())
                {
                    ASSERT(0); continue;
                }

                iCell++;
                if(bPosI) Write_Command(_T("S_INI"));
                else      Write_Command(_T("S_INJ"));
                Write_Int(SectK);
                Write_Int(iCell);

                for(int k=0; k<=InCell.arSpscPont[0].GetSize(); k++)
                {
                    double dy=0.0, dz=0.0;
                    if(k==InCell.arSpscPont[0].GetSize())
                    {
                        dy = InCell.arSpscPont[0].GetAt(0);
                        dz = InCell.arSpscPont[1].GetAt(0);
                    }
                    else
                    {
                        dy = InCell.arSpscPont[0].GetAt(k);
                        dz = InCell.arSpscPont[1].GetAt(k);
                    }
                    if(Write_Enable(32))
                    {
                        Write_Double(dy);
                        Write_Double(dz);
                    }
                    else
                    {
                        if(bPosI) Write_Command(_T("S_INI"), TRUE);
                        else      Write_Command(_T("S_INJ"), TRUE);
                        Write_NullInt();
                        Write_NullInt();
                        Write_Double(dy);
                        Write_Double(dz);
                    }
                }
            }
        }
    }

    // Delete because of New Operator.
    for(int i=0; i<arInner.GetSize(); i++)
    {
        if(arInner[i])
        {
            arInner[i]->RemoveAll();
            delete arInner[i];
            arInner[i] = NULL; 
        }
    }
    arInner.RemoveAll();


    return TRUE;
}

BOOL CCRCExportFile::Write_FinalLocalAxis()
{
    // Add by ZINU.('06.02.16). Check Bridge Direction.
    if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

    CArray<T_SKEW_K, T_SKEW_K> aSkewK; aSkewK.RemoveAll();
    T_SKEW_D SkewD;
    m_pDoc->m_pAttrCtrl->GetSkewKeyList(aSkewK);
    int nSize = aSkewK.GetSize();
    if(nSize == 0) return TRUE;

    for(int i = 0; i < nSize; i++)
    {
        SkewD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetSkew(aSkewK[i], SkewD)) continue;
        Write_Command(_T("A_LOCAL"));
        Write_Int(aSkewK[i]);
        for(int j=0; j<3; j++)
        {
            for(int k=0; k<3; k++)
                Write_Double(SkewD.LocalVector[j][k]);
        }
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_FinalNodeData()
{
    CArray<T_NODE_K, T_NODE_K> NodeKList; NodeKList.RemoveAll();
    T_NODE_D NodeD;
    T_SKEW_D SkewD;
    m_pDoc->m_pAttrCtrl->GetNodeKeyList(NodeKList);
    int nSize = NodeKList.GetSize();
    if(nSize == 0) return TRUE;

    for(int i=0; i<nSize; i++)
    {
        NodeD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetNode(NodeKList[i], NodeD)) continue;
        T_NODE_K LAxisK = 0;
        SkewD.Initialize();
        if(m_pDoc->m_pAttrCtrl->GetSkew(NodeKList[i], SkewD)) LAxisK = NodeKList[i];
        Write_Command(_T("N_DATA"));
        Write_Int(NodeKList[i]);
        Write_Int(LAxisK);
        Write_Double(NodeD.x);
        Write_Double(NodeD.y);
        Write_Double(NodeD.z);
    }

    /*
    CArray<T_NODE_K, T_NODE_K> NodeKList; NodeKList.RemoveAll();
    T_NODE_D NodeD;
    T_SKEW_D SkewD;
    m_pDoc->m_pPostCtrl->GetNodeKeyListPost(NodeKList);
    int nSize = NodeKList.GetSize();
    if(nSize == 0) return TRUE;

    for(int i=0; i<nSize; i++)
    {
    NodeD.Initialize();
    if(!m_pDoc->m_pPostCtrl->GetNodePost(NodeKList[i], NodeD)) continue;
    T_NODE_K LAxisK = 0;
    SkewD.Initialize();
    if(m_pDoc->m_pAttrCtrl->GetSkew(NodeKList[i], SkewD)) LAxisK = NodeKList[i];
    Write_Command(_T("N_DATA"));
    Write_Int(NodeKList[i]);
    Write_Int(LAxisK);
    Write_Double(NodeD.x);
    Write_Double(NodeD.y);
    Write_Double(NodeD.z);
    }
    */
    return TRUE;
}

BOOL CCRCExportFile::Write_FinalElemData()
{
    T_ELEM_K ElemK;
    T_ELEM_D ElemD;
    for (int i=0; i<m_PSCBeamBaseD.PElemKList.GetSize(); i++)
    {
        ElemK = m_PSCBeamBaseD.PElemKList[i];
        ElemD.Initialize();
        //PElemD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;
        T_SECT_K SectK;
        //Section은 반드시 Post Section을 사용(Tapered Section Group인경우 문제발생)
        if(!m_PSCBeamBaseD.PSectKMap.Lookup(ElemK,SectK)) continue; 
        T_MATL_K MatlK;
        if(!m_PSCBeamBaseD.MatlKMap.Lookup(ElemK,MatlK)) continue;

        Write_Command(_T("E_DATA"));
        Write_Int(ElemK);
        Write_Int(ElemD.elnod[0]);
        Write_Int(ElemD.elnod[1]);

        Write_Int(SectK); //실제 사용된 Post SectK
        Write_Int(MatlK);
        double dLen=m_pDoc->calcLAVElem(ElemD);
        Write_Double(dLen);
    }

    for(int i=0; i<m_ExtElemBaseD.PElemKList.GetSize(); i++)
    {
        ElemK = m_ExtElemBaseD.PElemKList[i];
        ElemD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;
        T_SECT_K SectK;
        //Section은 반드시 Post Section을 사용(Tapered Section Group인경우 문제발생)
        if(!m_ExtElemBaseD.PSectKMap.Lookup(ElemK,SectK)) continue;
        T_MATL_K MatlK;
        if(!m_ExtElemBaseD.MatlKMap.Lookup(ElemK,MatlK)) continue;

        Write_Command(_T("E_DATA"));
        Write_Int(ElemK);
        Write_Int(ElemD.elnod[0]);
        Write_Int(ElemD.elnod[1]);

        Write_Int(SectK); //실제 사용된 Post SectK
        Write_Int(MatlK);
        double dLen=m_pDoc->calcLAVElem(ElemD);
        Write_Double(dLen);
    }

    /*
    T_ELEM_K ElemK;
    T_ELEM_D ElemD;
    for (int i=0; i<m_PSCBeamPostD.PElemKList.GetSize(); i++)
    {
    ElemK = m_PSCBeamPostD.PElemKList[i];
    ElemD.Initialize();
    if(!m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD)) continue;
    T_SECT_K SectK;
    if(!m_PSCBeamPostD.PSectKMap.Lookup(ElemK,SectK)) continue;
    T_MATL_K MatlK;
    if(!m_PSCBeamPostD.MatlKMap.Lookup(ElemK,MatlK)) continue;

    Write_Command(_T("E_DATA"));
    Write_Int(ElemK);
    Write_Int(ElemD.elnod[0]);
    Write_Int(ElemD.elnod[1]);

    Write_Int(SectK); //실제 사용된 Post SectK
    Write_Int(MatlK);
    double dLen=m_pDoc->calcLAVElem(ElemD);
    Write_Double(dLen);
    }

    for(i=0; i<m_ExtElemPostD.PElemKList.GetSize(); i++)
    {
    ElemK = m_ExtElemPostD.PElemKList[i];
    ElemD.Initialize();
    if(!m_pDoc->m_pPostCtrl->GetElemPost(ElemK, ElemD)) continue;
    T_SECT_K SectK;
    if(!m_ExtElemPostD.PSectKMap.Lookup(ElemK,SectK)) continue;
    T_MATL_K MatlK;
    if(!m_ExtElemPostD.MatlKMap.Lookup(ElemK,MatlK)) continue;

    Write_Command(_T("E_DATA"));
    Write_Int(ElemK);
    Write_Int(ElemD.elnod[0]);
    Write_Int(ElemD.elnod[1]);

    Write_Int(SectK); //실제 사용된 Post SectK
    Write_Int(MatlK);
    double dLen=m_pDoc->calcLAVElem(ElemD);
    Write_Double(dLen);
    }
    */
    return TRUE;
}

BOOL CCRCExportFile::Write_FinalInfluenceLine()
{
    // Add by ZINU.('06.02.16). Check Bridge Direction.
    if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

    CString strCmd = _T("I_ILN");
    CGridInfo* pGridInfo = m_pDoc->m_pPostCtrl->GetGridInfo();

    CArray<T_GRIDINFO_GIRDER_D, T_GRIDINFO_GIRDER_D&>* pMainGriderList;
    CArray<T_GRIDINFO_GIRDER_D, T_GRIDINFO_GIRDER_D&>* pCrossBeamList;
    CArray<T_NODE_K, T_NODE_K>* pNodeKList;
    pMainGriderList = pGridInfo->GetMainGirder();
    pCrossBeamList = pGridInfo->GetCrossBeam();
    pNodeKList = pGridInfo->GetInflCrossPoint();

    int iNodeCount=0;
    for(int i=0; i<pMainGriderList->GetSize(); i++)
    {
        T_GRIDINFO_GIRDER_D MainGirder = pMainGriderList->GetAt(i);
        Write_Command(strCmd);
        Write_String(MainGirder.sGirderName);
        for(int j=0; j<pCrossBeamList->GetSize(); j++)
        {
            T_GRIDINFO_GIRDER_D CrossBeam = pCrossBeamList->GetAt(j);
            T_NODE_K NodeK = pNodeKList->GetAt(iNodeCount);
            iNodeCount++;

            if(Write_Enable(24))
            {
                Write_String(CrossBeam.sGirderName);
                Write_Int(NodeK);
            }
            else
            {
                Write_Command(strCmd, TRUE);
                Write_NullString();
                Write_String(CrossBeam.sGirderName);
                Write_Int(NodeK);
            }
        }
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_FinalLoadLine()
{
    // Add by ZINU.('06.02.16). Check Bridge Direction.
    if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

    CString strCmd = _T("I_LLN");
    CGridInfo* pGridInfo = m_pDoc->m_pPostCtrl->GetGridInfo();
    CArray<T_GRIDINFO_GIRDER_D, T_GRIDINFO_GIRDER_D&>* pCrossBeamList;
    pCrossBeamList = pGridInfo->GetCrossBeam();

    CArray<T_GRIDINFO_LOADLINE_D, T_GRIDINFO_LOADLINE_D&> LoadLineList;
    LoadLineList.RemoveAll();
    pGridInfo->GetLoadLine(LoadLineList);

    for(int i=0; i<LoadLineList.GetSize(); i++)
    {
        T_GRIDINFO_LOADLINE_D LoadLine = LoadLineList.GetAt(i);
        if(LoadLine.aLoadLine.GetSize() != pCrossBeamList->GetSize()) 
        {
            ASSERT(0);
            continue;
        }

        Write_Command(strCmd);
        Write_String(LoadLine.sLoadLineName);
        for(int j=0; j<LoadLine.aLoadLine.GetSize(); j++)
        {
            T_GRIDINFO_LOADLINE_BASE_D LineBase = LoadLine.aLoadLine.GetAt(j);
            T_MGDR_D MgdrD; MgdrD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetMgdr(LineBase.MgdrK, MgdrD)) continue;

            T_GRIDINFO_GIRDER_D CrossBeam = pCrossBeamList->GetAt(j);
            // Change by Jaeoh ('2005.10.11) 'LineBase.dDistMgdr' -> 'LineBase.dDistMgdr2'      
            if(Write_Enable(48))
            {
                Write_String(CrossBeam.sGirderName);
                Write_String(MgdrD.Name);
                Write_Double(LineBase.dDistMgdr2);
            }
            else
            {
                Write_Command(strCmd, TRUE);
                Write_NullString();
                Write_String(CrossBeam.sGirderName);
                Write_String(MgdrD.Name);
                Write_Double(LineBase.dDistMgdr2);
            }
        }
    }  

    return TRUE;
}

BOOL CCRCExportFile::Write_SpanData()
{
    // Add by ZINU.('06.02.16). Check Bridge Direction.
    if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

    double dFlen=0.0;
    T_UNIT_INDEX CurIndex;
    m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);

    CArray<T_BSPN_K, T_BSPN_K> BspnKList;
    BspnKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetBspnKeyList(BspnKList);

    T_BRGT_D BrgtD; BrgtD.Initialize();
    m_pDoc->m_pAttrCtrl->GetBrgt(BrgtD);
    T_BSPN_D BspnD;
    T_CBEM_D SCbemD, ECbemD;
    for(int i=0; i<BspnKList.GetSize(); i++)
    {
        BspnD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetBspn(BspnKList[i], BspnD)) continue;
        if(!m_pDoc->m_pAttrCtrl->GetCbem(BspnD.StartCrossK, SCbemD)) continue;
        if(!m_pDoc->m_pAttrCtrl->GetCbem(BspnD.EndCrossK, ECbemD)) continue;

        Write_Command(_T("I_SPAN"));
        Write_Int(BspnD.nSeq);
        Write_String(SCbemD.Name);
        Write_String(ECbemD.Name);
        Write_Int(BspnD.nSpanType);// 0:Girder, 1:Rahmen, 2:Cantilever, 3:Hanger Beam
        Write_Int(BspnD.nConnectionType);// 0:Both Fix, 1:Left Pin, 2:Right Pin, 3:Both Pin
        Write_Double(BspnD.dSpanLength);
        Write_Double(BspnD.dLoadingLength);
    }
    /*
    //요소별 충격계수
    CArray<T_GIMP_K, T_GIMP_K> GimpKList;
    GimpKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetGimpKeyList(GimpKList);
    T_GIMP_D GimpD;
    for(i=0; i<GimpKList.GetSize(); i++)
    {
    T_GIMP_K GimpK = GimpKList.GetAt(i);
    GimpD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetGimp(GimpK, GimpD)) continue;

    double dShk = 0.0;
    //Use Span Length of Span Data, Use Span Length 75 % of Span Data,Define Span Length Directory
    if(GimpD.nFactorType == 0 || GimpD.nFactorType == 1 || GimpD.nFactorType == 2)
    {
    double dLength = BspnD.dSpanLength * dFlen; //M Unit
    if(GimpD.nFactorType == 1) dLength = dLength*0.75;
    else(GimpD.nFactorType == 2) dLength = GimpD.dSpanLength;
    double dShk = 0.0;
    if(BrgtD.nKindOfBridge == 0) // 0:Steel, 1:PC, 2:RC
    {
    dShk = 20.0 / (50.+ dLength);
    }
    else if(BrgtD.nKindOfBridge == 1)
    {
    dShk = 10.0 / (25.+ dLength);
    }
    else if(BrgtD.nKindOfBridge == 2)
    {
    dShk = 7.0 / (20.+ dLength);
    }
    else ASSERT(0);
    }
    else if(GimpD.nFactorType == 3)//Define Impact Factor Directly
    {
    dShk = GimpD.dImpactFactor;
    }
    else ASSERT(0);

    if(GimpD.nTgtType == 0)       Write_Command(_T("I_IMPE"));
    else if(GimpD.nTgtType == 1)  Write_Command(_T("I_IMPEL"));
    else if(GimpD.nTgtType == 2)  Write_Command(_T("I_IMPNL"));
    Write_Int(GimpD.nTgtKey);
    Write_double(dShk);

    }
    */
    return TRUE;
}

BOOL CCRCExportFile::Write_GrupData(BOOL bWrite)
{
    Write_NodeElemGrup(bWrite);
    Write_BngrGrup(bWrite);
    Write_LoadGrup(bWrite);

    return TRUE;
}

BOOL CCRCExportFile::Write_NodeElemGrup(BOOL bWrite)
{
    int iRecord = 0;
    CArray<T_GRUP_K, T_GRUP_K> GrupKList; GrupKList.RemoveAll();
    T_GRUP_D GrupD;
    m_pDoc->m_pAttrCtrl->GetGrupKeyList(GrupKList);

    CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> ElemKMap;
    ElemKMap.RemoveAll();
    ElemKMap.InitHashTable(m_PSCBeamBaseD.PElemKList.GetSize()+m_ExtElemBaseD.PElemKList.GetSize());
    for(int i=0; i<m_PSCBeamBaseD.PElemKList.GetSize(); i++)
    {
        T_ELEM_K ElemK = m_PSCBeamBaseD.PElemKList.GetAt(i);
        ElemKMap.SetAt(ElemK,ElemK);
    }
    for(int i=0; i<m_ExtElemBaseD.PElemKList.GetSize(); i++)
    {
        T_ELEM_K ElemK = m_ExtElemBaseD.PElemKList.GetAt(i);
        ElemKMap.SetAt(ElemK,ElemK);
    }


    //Wriet Node and Elem Group
    m_DataNum.iNodeGrNum = 0;
    m_DataNum.iElemGrNum = 0;
    for(int i=0; i<GrupKList.GetSize(); i++)
    {
        GrupD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetGrup(GrupKList[i], GrupD)) continue;

        if(GrupD.arKeyNode.GetSize() > 0)
            qsort((void*)GrupD.arKeyNode.GetData(), GrupD.arKeyNode.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);

        CString strGrupName = GrupD.GroupName;
        if(strGrupName ==_T("")) strGrupName = _T("Default");
        for(int j=0; j<GrupD.arKeyNode.GetSize(); j++)
        {
            if(j==0)
            {
                if(bWrite)
                {
                    Write_Command(_T("GR_NODE"));
                    Write_Int((GrupKList[i]==0 ? 99999999 : GrupKList[i]));
                    Write_String(strGrupName);
                }
                iRecord = 32;
                m_DataNum.iNodeGrNum++;
            }
            if((bWrite && Write_Enable(8)) || (!bWrite && iRecord+8 <= 256)) 
            {
                if(bWrite) Write_Int(GrupD.arKeyNode.GetAt(j));
                iRecord += 8;
            }
            else
            {
                if(bWrite)
                {
                    Write_Command(_T("GR_NODE"), TRUE);
                    Write_NullInt();
                    Write_NullString();
                    Write_Int(GrupD.arKeyNode.GetAt(j));
                }
                iRecord = 40;
                m_DataNum.iNodeGrNum++;
            }
        }

        if (GrupD.arKeyElem.GetSize() > 0)
            qsort((void*)GrupD.arKeyElem.GetData(), GrupD.arKeyElem.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);

        strGrupName = GrupD.GroupName;
        if(strGrupName ==_T("")) strGrupName = _T("Default");
        int iCount = 0;
        for(int j=0; j<GrupD.arKeyElem.GetSize(); j++)
        {
            T_ELEM_K ElemK = GrupD.arKeyElem.GetAt(j);
            T_ELEM_K rElemK = 0;
            if(!ElemKMap.Lookup(ElemK, rElemK)) continue;
            if(iCount==0)
            {
                if(bWrite)
                {
                    Write_Command(_T("GR_ELEM"));
                    Write_Int((GrupKList[i]==0 ? 99999999 : GrupKList[i]));
                    Write_String(strGrupName);
                }
                iRecord = 32;
                m_DataNum.iElemGrNum++;
            }
            if((bWrite && Write_Enable(8)) || (!bWrite && iRecord+8 <= 256))
            {
                if(bWrite) Write_Int(ElemK);
                iRecord += 8;
            }
            else
            {
                if(bWrite)
                {
                    Write_Command(_T("GR_ELEM"), TRUE);
                    Write_NullInt();
                    Write_NullString();
                    Write_Int(ElemK);
                }
                iRecord = 40;
                m_DataNum.iElemGrNum++;
            }
            iCount++;
        }
    }

    iRecord = 0;

    //변단면 그룹
    m_DataNum.iTsecGrNum = 0;
    CArray<T_TSGR_K , T_TSGR_K> TsgrKList;
    TsgrKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetTsgrKeyList(TsgrKList);

    T_TSGR_D TsgrD;
    for(int i=0; i<TsgrKList.GetSize(); i++)
    {
        TsgrD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetTsgr(TsgrKList[i], TsgrD)) continue;

        CString strGrupName = TsgrD.GroupName;
        if(strGrupName ==_T("")) strGrupName = _T("Default");
        for(int j=0; j<TsgrD.aElemList.GetSize(); j++)
        {
            if(j==0)
            {
                if(bWrite)
                {
                    Write_Command(_T("GR_TSEC"));
                    Write_Int((TsgrKList[i]==0 ? 99999999 : TsgrKList[i]));
                    Write_String(strGrupName);
                }
                iRecord = 32;
                m_DataNum.iTsecGrNum++;
            }
            if((bWrite && Write_Enable(8)) || (!bWrite && iRecord+8 <= 256))
            {
                if(bWrite) Write_Int(TsgrD.aElemList.GetAt(j));
                iRecord += 8;
            }
            else
            {
                if(bWrite)
                {
                    Write_Command(_T("GR_TSEC"), TRUE);
                    Write_NullInt();
                    Write_NullString();
                    Write_Int(TsgrD.aElemList.GetAt(j));
                }
                iRecord = 40;
                m_DataNum.iTsecGrNum++;
            }
        }
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_BngrGrup(BOOL bWrite)
{
    //Write Constraint Group
    m_DataNum.iConsGrNum=0;
    T_BNGR_D BngrD;

    CArray<T_CONS_K, T_CONS_K> ConsKList;
    ConsKList.RemoveAll();
    T_CONS_D ConsD;
    m_pDoc->m_pAttrCtrl->GetConsKeyList(ConsKList);
    for(int i=0; i<ConsKList.GetSize(); i++)
    {
        T_CONS_K ConsK = ConsKList.GetAt(i);
        ConsD.Initialize();
        BngrD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetCons(ConsK, ConsD)) continue;
        if (ConsD.GroupKey == 0) 
        {
            ConsD.GroupKey = 99999999;
            BngrD.GroupName = _T("Default");
        }
        else m_pDoc->m_pAttrCtrl->GetBngr(ConsD.GroupKey, BngrD);

        if(bWrite)
        {
            Write_Command(_T("GR_CONS"));
            Write_Int(ConsD.GroupKey);
            Write_String(BngrD.GroupName);
            Write_Int(ConsK.key.entity);
            Write_String8(ConsD.Constraint[0]);
            Write_String8(ConsD.Constraint[1]);
            Write_String8(ConsD.Constraint[2]);
            Write_String8(ConsD.Constraint[3]);
            Write_String8(ConsD.Constraint[4]);
            Write_String8(ConsD.Constraint[5]);
        }
        m_DataNum.iConsGrNum++;
    }

    // Write General Spring Group
    m_DataNum.iGsprGrNum=0;
    CArray<T_GSPR_K, T_GSPR_K> GsprKList;
    GsprKList.RemoveAll();
    T_GSPR_D GsprD;
    T_GSTP_D GstpD;
    m_pDoc->m_pAttrCtrl->GetGsprKeyList(GsprKList);
    for(int i=0; i<GsprKList.GetSize(); i++)
    {
        T_GSPR_K GsprK = GsprKList.GetAt(i);
        GsprD.Initialize();
        GstpD.Initialize();
        BngrD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetGspr(GsprK, GsprD)) continue;
        if(!m_pDoc->m_pAttrCtrl->GetGstp(GsprD.SpringTypeKey, GstpD)) continue;
        if (GsprD.GroupKey == 0) 
        {
            GsprD.GroupKey = 99999999;
            BngrD.GroupName = _T("Default");
        }
        else m_pDoc->m_pAttrCtrl->GetBngr(GsprD.GroupKey, BngrD);

        if(bWrite)
        {
            Write_Command(_T("GR_GSPR"));
            Write_Int(GsprD.GroupKey);
            Write_String(BngrD.GroupName);
            Write_Int(GsprK.key.entity);
            Write_Double(GstpD.Spring[0]); Write_Double(GstpD.Spring[6]); Write_Double(GstpD.Spring[7]); Write_Double(GstpD.Spring[8]); Write_Double(GstpD.Spring[9]); Write_Double(GstpD.Spring[10]);

            Write_Command(_T("GR_GSPR"), TRUE);
            Write_NullInt();
            Write_NullString();
            Write_NullInt();
            Write_NullDouble(); Write_Double(GstpD.Spring[1]); Write_Double(GstpD.Spring[11]); Write_Double(GstpD.Spring[12]); Write_Double(GstpD.Spring[13]); Write_Double(GstpD.Spring[14]);

            Write_Command(_T("GR_GSPR"), TRUE);
            Write_NullInt();
            Write_NullString();
            Write_NullInt();
            Write_NullDouble(); Write_NullDouble(); Write_Double(GstpD.Spring[2]); Write_Double(GstpD.Spring[15]); Write_Double(GstpD.Spring[16]); Write_Double(GstpD.Spring[17]);

            Write_Command(_T("GR_GSPR"), TRUE);
            Write_NullInt();
            Write_NullString();
            Write_NullInt();
            Write_NullDouble(); Write_NullDouble(); Write_NullDouble(); Write_Double(GstpD.Spring[3]); Write_Double(GstpD.Spring[18]); Write_Double(GstpD.Spring[19]);

            Write_Command(_T("GR_GSPR"), TRUE);
            Write_NullInt();
            Write_NullString();
            Write_NullInt();
            Write_NullDouble(); Write_NullDouble(); Write_NullDouble(); Write_NullDouble(); Write_Double(GstpD.Spring[4]); Write_Double(GstpD.Spring[20]);

            Write_Command(_T("GR_GSPR"), TRUE);
            Write_NullInt();
            Write_NullString();
            Write_NullInt();
            Write_NullDouble(); Write_NullDouble(); Write_NullDouble(); Write_NullDouble(); Write_NullDouble(); Write_Double(GstpD.Spring[5]);
        }
        m_DataNum.iGsprGrNum += 6;

    }

    // Write Spring Group
    m_DataNum.iNsprGrNum=0;
    CArray<T_NSPR_K, T_NSPR_K> NsprKList;
    NsprKList.RemoveAll();
    T_NSPR_D NsprD;
    m_pDoc->m_pAttrCtrl->GetNsprKeyList(NsprKList);
    for(int i=0; i<NsprKList.GetSize(); i++)
    {
        T_NSPR_K NsprK = NsprKList.GetAt(i);
        NsprD.Initialize();
        BngrD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetNspr(NsprK, NsprD)) continue;
        if (NsprD.GroupKey == 0) 
        {
            NsprD.GroupKey = 99999999;
            BngrD.GroupName = _T("Default");
        }
        else m_pDoc->m_pAttrCtrl->GetBngr(NsprD.GroupKey, BngrD);

        if(bWrite)
        {
            Write_Command(_T("GR_NSPR"));
            Write_Int(NsprD.GroupKey);
            Write_String(BngrD.GroupName);
            Write_Int(NsprK.key.entity);
            Write_Double(NsprD.Spring[0]);
            Write_Double(NsprD.Spring[1]);
            Write_Double(NsprD.Spring[2]);
            Write_Double(NsprD.Spring[3]);
            Write_Double(NsprD.Spring[4]);
            Write_Double(NsprD.Spring[5]);
        }
        m_DataNum.iNsprGrNum++;
    }

    int iRecord = 0;
    //Write Rigid Link Group
    m_DataNum.iRigdGrNum=0;
    CArray<T_RIGD_K, T_RIGD_K> RigdKList;
    RigdKList.RemoveAll();
    T_RIGD_D RigdD;
    m_pDoc->m_pAttrCtrl->GetRigdKeyList(RigdKList);
    for(int i=0; i<RigdKList.GetSize(); i++)
    {
        T_RIGD_K RigdK = RigdKList.GetAt(i);
        RigdD.Initialize();
        BngrD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetRigd(RigdK, RigdD)) continue;
        if (RigdD.GroupKey == 0) 
        {
            RigdD.GroupKey = 99999999;
            BngrD.GroupName = _T("Default");
        }
        else m_pDoc->m_pAttrCtrl->GetBngr(RigdD.GroupKey, BngrD);

        CString Digit;
        int iSlaveType[6];
        Digit.Format(_T("%6.6d"),RigdD.nSlaveType);
        for(int j=0; j<6; j++)
        {
            if(Digit.GetAt(j) == '1') iSlaveType[j] = 1;
            else                      iSlaveType[j] = 0;
        }

        if(bWrite)
        {
            Write_Command(_T("GR_RIGD"));
            Write_Int(RigdD.GroupKey);
            Write_String(BngrD.GroupName);
            Write_Int(RigdK.key.entity);
            Write_Int(iSlaveType[0]);
            Write_Int(iSlaveType[1]);
            Write_Int(iSlaveType[2]);
            Write_Int(iSlaveType[3]);
            Write_Int(iSlaveType[4]);
            Write_Int(iSlaveType[5]);
        }
        m_DataNum.iRigdGrNum++;
        iRecord = 136;

        for(int j=0; j<RigdD.arSlave.GetSize(); j++)
        {
            if((bWrite && Write_Enable(8)) || (!bWrite && iRecord+8 <= 256))
            {
                if(bWrite) Write_Int(RigdD.arSlave.GetAt(j));
                iRecord += 8;
            }
            else
            {
                if(bWrite)
                {
                    Write_Command(_T("GR_RIGD"), TRUE);
                    Write_NullInt();
                    Write_NullString();
                    Write_NullInt();
                    Write_NullInt();
                    Write_NullInt();
                    Write_NullInt();
                    Write_NullInt();
                    Write_NullInt();
                    Write_NullInt();
                    Write_Int(RigdD.arSlave.GetAt(j));
                }
                iRecord = 144;
                m_DataNum.iRigdGrNum++;
            }
        }
    }

    //Write Elastic Link Group
    m_DataNum.iElnkGrNum=0;
    CArray<T_ELNK_K, T_ELNK_K> ElnkKList;
    ElnkKList.RemoveAll();
    T_ELNK_D ElnkD;
    m_pDoc->m_pAttrCtrl->GetElnkKeyList(ElnkKList);
    for(int i=0; i<ElnkKList.GetSize(); i++)
    {
        T_ELNK_K ElnkK = ElnkKList.GetAt(i);
        ElnkD.Initialize();
        BngrD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetElnk(ElnkK, ElnkD)) continue;
        if (ElnkD.GroupKey == 0) 
        {
            ElnkD.GroupKey = 99999999;
            BngrD.GroupName = _T("Default");
        }
        else m_pDoc->m_pAttrCtrl->GetBngr(ElnkD.GroupKey, BngrD);

        if(bWrite)
        {
            Write_Command(_T("GR_ELNK"));
            Write_Int(ElnkD.GroupKey);
            Write_String(BngrD.GroupName);
            Write_Int(ElnkK);
            Write_Int(ElnkD.Node1);
            Write_Int(ElnkD.Node2);
            Write_Double(ElnkD.dBetaAngle*DEGTORAD);
            Write_Int(ElnkD.nSubOption); // =0 General =1 Rigid =2 Tension Only =3 Compression Only
            Write_Double(ElnkD.Spring[0]);
            Write_Double(ElnkD.Spring[1]);
            Write_Double(ElnkD.Spring[2]);
            Write_Double(ElnkD.Spring[3]);
            Write_Double(ElnkD.Spring[4]);
            Write_Double(ElnkD.Spring[5]);
            Write_Int((ElnkD.bShearSprLoc ? 1 : 0));
            Write_Double(ElnkD.dDistRatioSDy);
            Write_Double(ElnkD.dDistRatioSDz);
        }
        m_DataNum.iElnkGrNum++;
    }

    //Write General Link Property and Group
    m_DataNum.iGlnkGrNum=0;

    CArray<T_NLNK_K, T_NLNK_K> NlnkKList;
    NlnkKList.RemoveAll();
    T_NLLP_D NllpD;
    T_NLNK_D NlnkD;
    m_pDoc->m_pAttrCtrl->GetNlnkKeyList(NlnkKList);
    for(int i=0; i<NlnkKList.GetSize(); i++)
    {
        T_NLNK_K NlnkK = NlnkKList.GetAt(i);
        NlnkD.Initialize();
        BngrD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetNlnk(NlnkK, NlnkD)) continue;

        NllpD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetNllp(NlnkD.PropKey, NllpD)) continue;

        if(NllpD.nApplicationType!=0 || NllpD.nElemType!=0) continue;

        if (NlnkD.GroupKey == 0) 
        {
            NlnkD.GroupKey = 99999999;
            BngrD.GroupName = _T("Default");
        }
        else m_pDoc->m_pAttrCtrl->GetBngr(NlnkD.GroupKey, BngrD);

        if(bWrite)
        {
            Write_Command(_T("GR_GLNK"));
            Write_Int(NlnkD.GroupKey);
            Write_String(BngrD.GroupName);
            Write_Int(NlnkK);
            Write_Int(NlnkD.Node1);
            Write_Int(NlnkD.Node2);
            Write_Int(NlnkD.nRefSystem);
            Write_Int((NllpD.bShearSprLoc ? 1 : 0));

            for(int j=0; j<3; j++)
            {
                for(int k=0; k<3; k++)
                    Write_Double(NlnkD.LocalVector[j][k]);
            }

            Write_Command(_T("GR_GLNK"), TRUE);
            Write_NullInt();
            Write_NullString();
            Write_NullInt();
            Write_NullInt();
            Write_NullInt();
            Write_NullInt();
            Write_NullInt();

            Write_Double(NlnkD.dBetaAngle*DEGTORAD);
            Write_Double(NllpD.LDP_dEffStiff[0]);
            Write_Double(NllpD.LDP_dEffStiff[1]);
            Write_Double(NllpD.LDP_dEffStiff[2]);
            Write_Double(NllpD.LDP_dEffStiff[3]);
            Write_Double(NllpD.LDP_dEffStiff[4]);
            Write_Double(NllpD.LDP_dEffStiff[5]);
            Write_Double(NllpD.dDistRatioU2);
            Write_Double(NllpD.dDistRatioU3);
        }
        m_DataNum.iGlnkGrNum += 2;
    }

    //Write Section Stiffness Scale Factor
    m_DataNum.iSecfGrNum=0;
    CArray<T_SECF_K, T_SECF_K> SecfKList;
    SecfKList.RemoveAll();
    T_SECF_D SecfD;
    m_pDoc->m_pAttrCtrl->GetSecfKeyList(SecfKList);
    for(int i=0; i<SecfKList.GetSize(); i++)
    {
        T_SECF_K SecfK = SecfKList.GetAt(i);
        SecfD.Initialize();
        BngrD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetSecf(SecfK, SecfD)) continue;
        if (SecfD.GroupKey == 0) 
        {
            SecfD.GroupKey = 99999999;
            BngrD.GroupName = _T("Default");
        }
        else m_pDoc->m_pAttrCtrl->GetBngr(SecfD.GroupKey, BngrD);

        if(bWrite)
        {
            Write_Command(_T("GR_SESF"));
            Write_Int(SecfD.GroupKey);
            Write_String(BngrD.GroupName);
            Write_Int(SecfK.key.entity);
            Write_Double(SecfD.dScaleFactor[0]);
            Write_Double(SecfD.dScaleFactor[1]);
            Write_Double(SecfD.dScaleFactor[2]);
            Write_Double(SecfD.dScaleFactor[3]);
            Write_Double(SecfD.dScaleFactor[4]);
            Write_Double(SecfD.dScaleFactor[5]);
            Write_Double(SecfD.dScaleFactor[6]);
            // Add by ZINU.('07.03.28). NO:2712, Separate T_SECF_D J from I for Tapered Section.
            /*
            Write_Double(SecfD.dScaleFactorJ[0]);
            Write_Double(SecfD.dScaleFactorJ[1]);
            Write_Double(SecfD.dScaleFactorJ[2]);
            Write_Double(SecfD.dScaleFactorJ[3]);
            Write_Double(SecfD.dScaleFactorJ[4]);
            Write_Double(SecfD.dScaleFactorJ[5]);
            Write_Double(SecfD.dScaleFactorJ[6]);
            */
        }
        m_DataNum.iSecfGrNum++;
    }

    //Write Change General Link Property
    m_DataNum.iCglpGrNum=0;
    CArray<T_CGLP_K, T_CGLP_K> CglpKList;
    CglpKList.RemoveAll();
    T_CGLP_D CglpD;
    m_pDoc->m_pAttrCtrl->GetCglpKeyList(CglpKList);
    for(int i=0; i<CglpKList.GetSize(); i++)
    {
        T_CGLP_K CglpK = CglpKList.GetAt(i);
        CglpD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetCglp(CglpK, CglpD)) continue;

        NllpD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetNllp(CglpD.GLinkPropK, NllpD)) continue;

        if(NllpD.nApplicationType!=0 || NllpD.nElemType!=0) continue;

        BngrD.Initialize();
        if (CglpD.GroupKey == 0) 
        {
            CglpD.GroupKey = 99999999;
            BngrD.GroupName = _T("Default");
        }
        else m_pDoc->m_pAttrCtrl->GetBngr(CglpD.GroupKey, BngrD);

        if(bWrite)
        {
            Write_Command(_T("GR_CGLP"));
            Write_Int(CglpD.GroupKey);
            Write_String(BngrD.GroupName);
            Write_Int(CglpD.GLinkK);
            Write_Int((NllpD.bShearSprLoc ? 1 : 0));
            Write_Double(NllpD.LDP_dEffStiff[0]);
            Write_Double(NllpD.LDP_dEffStiff[1]);
            Write_Double(NllpD.LDP_dEffStiff[2]);
            Write_Double(NllpD.LDP_dEffStiff[3]);
            Write_Double(NllpD.LDP_dEffStiff[4]);
            Write_Double(NllpD.LDP_dEffStiff[5]);
            Write_Double(NllpD.dDistRatioU2);
            Write_Double(NllpD.dDistRatioU3);
        }
        m_DataNum.iGlnkGrNum ++;
    }

    ////////////////Boundary Change Assignment to Load Case/Analysis///////////////
    T_STLD_D StldD; StldD.Initialize();
    T_BCCT_D BcctD; BcctD.Initialize();
    m_pDoc->m_pAttrCtrl->GetBcct(BcctD);
    for(int i=0; i<BcctD.aBcctBase.GetSize(); i++)
    {
        T_BCCT_BASE BcctBase = BcctD.aBcctBase.GetAt(i);

        if(bWrite)
        {
            Write_Command(_T("G_BCCT"));
            Write_Int(BcctBase.nLoadCaseType);
            if(BcctBase.nLoadCaseType==0) //LoadCase
            {
                StldD.Initialize();
                if(!m_pDoc->m_pAttrCtrl->GetStld(BcctBase.LoadCaseKey, StldD)) continue;
                Write_NullInt();
                Write_String(StldD.LoadCaseName);
            }
            else //AnalType
            {
                Write_Int(BcctBase.LoadCaseKey);
                Write_NullString();
            }

            if(BcctBase.BcctCombIndex==-1)  Write_Int(0); //Unchanged
            else                            
            {
                Write_Int(1); //Changed
                for(int j=0; j<BcctD.aBcctComb[BcctBase.BcctCombIndex].aBngrKey.GetSize(); j++)
                {
                    T_BNGR_D BngrD; BngrD.Initialize();
                    T_BNGR_K BngrK = BcctD.aBcctComb[BcctBase.BcctCombIndex].aBngrKey[j];

                    if(BngrK == 0) BngrK = 99999999;

                    if(Write_Enable(8))
                    {
                        Write_Int(BngrK);
                    }
                    else
                    {
                        Write_Command(_T("G_BCCT"), TRUE);
                        Write_NullInt(); //LoadCaseType
                        Write_NullInt(); //AnalType
                        Write_NullString(); //LoadCase Name
                        Write_NullInt(); //Changed
                        Write_Int(BngrK);
                    }
                }
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////

    return TRUE;
}

BOOL CCRCExportFile::Write_LoadGrup(BOOL bWrite)
{
    // Write Nodal Load Group
    CArray<T_STLD_K, T_STLD_K> StldKList;
    StldKList.RemoveAll();
    T_STLD_D StldD;
    m_pDoc->m_pAttrCtrl->GetStldKeyList(StldKList);
    int nSize = StldKList.GetSize();

    CMap<UINT, UINT, int, int> KeyPosMap; 
    KeyPosMap.RemoveAll();
    for(int i=0; i<nSize; i++) KeyPosMap.SetAt(StldKList[i], i);

    CArray<UINT, UINT>* arKeyList;
    arKeyList = new CArray<UINT, UINT>[nSize];

    //Initial Temperature
    T_STYP_D StypD; StypD.Initialize();
    m_pDoc->m_pAttrCtrl->GetStyp(StypD);


    T_CNLD_K CnldK; T_CNLD_D CnldD;

    // cnld
    m_DataNum.iCndlGrNum=0;
    POSITION pos = m_pDoc->m_pAttrCtrl->GetStartCnld();
    while (pos) 
    {
        int nIX = 0;
        CnldD.Initialize();
        m_pDoc->m_pAttrCtrl->GetNextCnld(pos, CnldK, CnldD);
        BOOL bExist = KeyPosMap.Lookup(CnldD.LoadCaseKey, nIX);
        ASSERT(bExist);
        arKeyList[nIX].Add(CnldK.keymap);
    }

    for(int i=0; i<nSize; i++)
    {
        T_STLD_K StldK = StldKList[i];
        int nCount = arKeyList[i].GetSize();
        if(nCount<1) continue;

        StldD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetStld(StldK, StldD)) continue;

        T_LDGR_D LdgrD;
        qsort((void*)arKeyList[i].GetData(), nCount, sizeof(UINT), CCompFunc::i20x12Asc);
        for (int j=0; j<nCount; j++)
        {
            CnldK.keymap = arKeyList[i].GetAt(j);
            CnldD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetCnld(CnldK, CnldD)) continue;
            LdgrD.Initialize();
            if (CnldD.GroupKey == 0) 
            {
                CnldD.GroupKey = 99999999;
                LdgrD.GroupName = _T("Default");
            }
            else m_pDoc->m_pAttrCtrl->GetLdgr(CnldD.GroupKey, LdgrD);

            if(bWrite)
            {
                Write_Command(_T("GR_CNLD"));
                Write_Int(CnldD.GroupKey);
                Write_String(LdgrD.GroupName);
                Write_String(StldD.LoadCaseType);
                Write_String(StldD.LoadCaseName);
                Write_Int(CnldK.key.entity);
                Write_Double(CnldD.Force[0]);
                Write_Double(CnldD.Force[1]);
                Write_Double(CnldD.Force[2]);
                Write_Double(CnldD.Force[3]);
                Write_Double(CnldD.Force[4]);
                Write_Double(CnldD.Force[5]);
            }
            m_DataNum.iCndlGrNum++;
        }
    }

    // Bmld
    m_DataNum.iBmldGrNum=0;
    for(int i=0; i<nSize; i++) arKeyList[i].RemoveAll();

    T_BMLD_K BmldK; T_BMLD_D BmldD;
    pos = m_pDoc->m_pAttrCtrl->GetStartBmld();
    while (pos) 
    {
        int nIX = 0;
        BmldD.Initialize();
        m_pDoc->m_pAttrCtrl->GetNextBmld(pos, BmldK, BmldD);
        BOOL bExist = KeyPosMap.Lookup(BmldD.LoadCaseKey, nIX);
        ASSERT(bExist);
        arKeyList[nIX].Add(BmldK.keymap);
    }

    for(int i=0; i<nSize; i++)
    {
        T_STLD_K StldK = StldKList[i];
        int nCount = arKeyList[i].GetSize();
        if(nCount<1) continue;

        StldD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetStld(StldK, StldD)) continue;

        T_LDGR_D LdgrD;
        qsort((void*)arKeyList[i].GetData(), nCount, sizeof(UINT), CCompFunc::i20x12Asc);
        for (int j=0; j<nCount; j++)
        {
            BmldK.keymap = arKeyList[i].GetAt(j);
            BmldD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetBmld(BmldK, BmldD)) continue;

            //LoadCommandType : 1=BeamLoad  2=FloorLoad  3=Use Temporary 4=Prestress 5=MovingLoad 6=LineLoad 7=TypicalLoad
            // Change by ZINU.('08.02.29). Change Condition.
            // if(BmldD.LoadCommandType == 1 && (BmldD.LoadType==1 || BmldD.LoadType==3)) -> if(BmldD.LoadCommandType==1).
            if(BmldD.LoadCommandType==1)
            {
                LdgrD.Initialize();
                if (BmldD.GroupKey == 0) 
                {
                    BmldD.GroupKey = 99999999;
                    LdgrD.GroupName = _T("Default");
                }
                else m_pDoc->m_pAttrCtrl->GetLdgr(BmldD.GroupKey, LdgrD);

                CString aDir[] = {_T("LX"), _T("LY"), _T("LZ"), _T("GX"), _T("GY"), _T("GZ"), _T("VECTOR")};
                // Change by ZINU.('08.02.28). Add New Type CM, DM.
                // CL(Concentrated Load), CM(Concentrated Moment), DL(Distributed Load), DM(Distributed Moment).
                CString strLType=_T("");
                if(BmldD.LoadType==1)       strLType = _T("CL");
                else if(BmldD.LoadType==2)  strLType = _T("CM");
                else if(BmldD.LoadType==3)  strLType = _T("DL");
                else if(BmldD.LoadType==4)  strLType = _T("DM");
                else  ASSERT(0);

                if(bWrite)
                {
                    Write_Command(_T("GR_BMLD"));
                    Write_Int(BmldD.GroupKey);
                    Write_String(LdgrD.GroupName);
                    Write_String(StldD.LoadCaseType);
                    Write_String(StldD.LoadCaseName);
                    Write_Int(BmldK.key.entity);
                    Write_String(strLType);
                    Write_String(aDir[BmldD.LoadDirection-1]);
                    Write_Double(BmldD.DistanceRatio[0]);
                    Write_Double(BmldD.Force_Type[BmldD.LoadType-1][0]);
                    Write_Double(BmldD.DistanceRatio[1]);
                    Write_Double(BmldD.Force_Type[BmldD.LoadType-1][1]);
                    Write_Double(BmldD.DistanceRatio[2]);
                    Write_Double(BmldD.Force_Type[BmldD.LoadType-1][2]);
                    Write_Double(BmldD.DistanceRatio[3]);
                    Write_Double(BmldD.Force_Type[BmldD.LoadType-1][3]);
                }
                m_DataNum.iBmldGrNum++;
            }
        }
    }

    // Prestress
    m_DataNum.iPrstGrNum=0;
    for(int i=0; i<nSize; i++) arKeyList[i].RemoveAll();

    T_TDPL_K TdplK; T_TDPL_D TdplD;
    pos = m_pDoc->m_pAttrCtrl->GetStartTdpl();
    while (pos) 
    {
        int nIX = 0;
        TdplD.Initialize();
        m_pDoc->m_pAttrCtrl->GetNextTdpl(pos, TdplK, TdplD);
        BOOL bExist = KeyPosMap.Lookup(TdplD.LoadCaseKey, nIX);
        ASSERT(bExist);
        arKeyList[nIX].Add(TdplK.keymap);
    }

    for(int i=0; i<nSize; i++)
    {
        T_STLD_K StldK = StldKList[i];
        int nCount = arKeyList[i].GetSize();
        if(nCount<1) continue;

        StldD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetStld(StldK, StldD)) continue;

        T_LDGR_D LdgrD;
        qsort((void*)arKeyList[i].GetData(), nCount, sizeof(UINT), CCompFunc::i20x12Asc);
        for (int j=0; j<nCount; j++)
        {
            TdplK.keymap = arKeyList[i].GetAt(j);
            TdplD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetTdpl(TdplK, TdplD)) continue;

            LdgrD.Initialize();
            if (TdplD.GroupKey == 0) 
            {
                TdplD.GroupKey = 99999999;
                LdgrD.GroupName = _T("Default");
            }
            else m_pDoc->m_pAttrCtrl->GetLdgr(TdplD.GroupKey, LdgrD);

            if(bWrite)
            {
                Write_Command(_T("GR_PRST"));
                Write_Int(TdplD.GroupKey);
                Write_String(LdgrD.GroupName);
                Write_String(StldD.LoadCaseType);
                Write_String(StldD.LoadCaseName);
                Write_Int(TdplK.key.entity);
                Write_Int(TdplD.nOrder);
                Write_Int(TdplD.nValueType);
                Write_Double(TdplD.dValueBegin[TdplD.nValueType]);
                Write_Double(TdplD.dValueEnd[TdplD.nValueType]);
                Write_Int(TdplD.nGrouting);
            }
            m_DataNum.iPrstGrNum++;
        }
    }

    // Etmp
    m_DataNum.iEtmpGrNum=0;
    for(int i=0; i<nSize; i++) arKeyList[i].RemoveAll();

    T_ETMP_K EtmpK; T_ETMP_D EtmpD;
    pos = m_pDoc->m_pAttrCtrl->GetStartEtmp();
    while (pos) 
    {
        int nIX = 0;
        EtmpD.Initialize();
        m_pDoc->m_pAttrCtrl->GetNextEtmp(pos, EtmpK, EtmpD);
        BOOL bExist = KeyPosMap.Lookup(EtmpD.LoadCaseKey, nIX);
        ASSERT(bExist);
        arKeyList[nIX].Add(EtmpK.keymap);
    }

    for(int i=0; i<nSize; i++)
    {
        T_STLD_K StldK = StldKList[i];
        int nCount = arKeyList[i].GetSize();
        if(nCount<1) continue;

        StldD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetStld(StldK, StldD)) continue;

        T_LDGR_D LdgrD;
        qsort((void*)arKeyList[i].GetData(), nCount, sizeof(UINT), CCompFunc::i20x12Asc);
        for (int j=0; j<nCount; j++)
        {
            EtmpK.keymap = arKeyList[i].GetAt(j);
            EtmpD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetEtmp(EtmpK, EtmpD)) continue;

            LdgrD.Initialize();
            if (EtmpD.GroupKey == 0) 
            {
                EtmpD.GroupKey = 99999999;
                LdgrD.GroupName = _T("Default");
            }
            else m_pDoc->m_pAttrCtrl->GetLdgr(EtmpD.GroupKey, LdgrD);

            if(bWrite)
            {
                Write_Command(_T("GR_ETMP"));
                Write_Int(EtmpD.GroupKey);
                Write_String(LdgrD.GroupName);
                Write_String(StldD.LoadCaseType);
                Write_String(StldD.LoadCaseName);
                Write_Int(EtmpK.key.entity);
                double dTemp = EtmpD.dblElemTemp - StypD.dblInitTemp;
                Write_Double(dTemp);
            }
            m_DataNum.iEtmpGrNum++;
        }
    }

    // Gtmp
    m_DataNum.iGtmpGrNum=0;
    for(int i=0; i<nSize; i++) arKeyList[i].RemoveAll();

    T_GTMP_K GtmpK; T_GTMP_D GtmpD;
    pos = m_pDoc->m_pAttrCtrl->GetStartGtmp();
    while (pos) 
    {
        int nIX = 0;
        GtmpD.Initialize();
        m_pDoc->m_pAttrCtrl->GetNextGtmp(pos, GtmpK, GtmpD);
        BOOL bExist = KeyPosMap.Lookup(GtmpD.LoadCaseKey, nIX);
        ASSERT(bExist);
        arKeyList[nIX].Add(GtmpK.keymap);
    }

    for(int i=0; i<nSize; i++)
    {
        T_STLD_K StldK = StldKList[i];
        int nCount = arKeyList[i].GetSize();
        if(nCount<1) continue;

        StldD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetStld(StldK, StldD)) continue;

        T_LDGR_D LdgrD;
        qsort((void*)arKeyList[i].GetData(), nCount, sizeof(UINT), CCompFunc::i20x12Asc);
        for (int j=0; j<nCount; j++)
        {
            GtmpK.keymap = arKeyList[i].GetAt(j);
            GtmpD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetGtmp(GtmpK, GtmpD)) continue;

            LdgrD.Initialize();
            if (GtmpD.GroupKey == 0) 
            {
                GtmpD.GroupKey = 99999999;
                LdgrD.GroupName = _T("Default");
            }
            else m_pDoc->m_pAttrCtrl->GetLdgr(GtmpD.GroupKey, LdgrD);

            if(GtmpD.nType == 1) //Beam
            {
                if(bWrite)
                {
                    Write_Command(_T("GR_GTMP"));
                    Write_Int(GtmpD.GroupKey);
                    Write_String(LdgrD.GroupName);
                    Write_String(StldD.LoadCaseType);
                    Write_String(StldD.LoadCaseName);
                    Write_Int(GtmpK.key.entity);
                    Write_Double(GtmpD.dTy);
                    if(GtmpD.bUsePropHy)  Write_Double(0.0);
                    else                  Write_Double(GtmpD.dHy);
                    Write_Double(GtmpD.dTz);
                    if(GtmpD.bUsePropHz)  Write_Double(0.0);
                    else                  Write_Double(GtmpD.dHz);
                }
                m_DataNum.iGtmpGrNum++;
            }
        }
    }

    // Btmp
    m_DataNum.iBtmpGrNum=0;
    for(int i=0; i<nSize; i++) arKeyList[i].RemoveAll();

    T_BTMP_K BtmpK; T_BTMP_D BtmpD;
    pos = m_pDoc->m_pAttrCtrl->GetStartBtmp();
    while (pos) 
    {
        int nIX = 0;
        BtmpD.Initialize();
        m_pDoc->m_pAttrCtrl->GetNextBtmp(pos, BtmpK, BtmpD);
        BOOL bExist = KeyPosMap.Lookup(BtmpD.LoadCaseKey, nIX);
        ASSERT(bExist);
        arKeyList[nIX].Add(BtmpK.keymap);
    }

    for(int i=0; i<nSize; i++)
    {
        T_STLD_K StldK = StldKList[i];
        int nCount = arKeyList[i].GetSize();
        if(nCount<1) continue;

        StldD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetStld(StldK, StldD)) continue;

        T_LDGR_D LdgrD;
        qsort((void*)arKeyList[i].GetData(), nCount, sizeof(UINT), CCompFunc::i20x12Asc);
        for(int j=0; j<nCount; j++)
        {
            BtmpK.keymap = arKeyList[i].GetAt(j);
            BtmpD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetBtmp(BtmpK, BtmpD)) continue;

            LdgrD.Initialize();
            if (BtmpD.GroupKey == 0) 
            {
                BtmpD.GroupKey = 99999999;
                LdgrD.GroupName = _T("Default");
            }
            else m_pDoc->m_pAttrCtrl->GetLdgr(BtmpD.GroupKey, LdgrD);

            T_BTMP_STMP StmpD; 
            for(int k=0; k<5; k++)
            {
                StmpD.Initialize();
                StmpD = BtmpD.SectTmp[k];

                double dElast = StmpD.dElast;
                double dThermal = StmpD.dThermal;
                if(StmpD.nMaterial ==1)// 1:Element
                {
                    T_ELEM_D ElemD; ElemD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetElem(BtmpK.key.entity, ElemD)) continue;
                    T_MATL_D MatlD; MatlD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD)) continue;
                    dElast = MatlD.Data1.Analysis.Elast;
                    dThermal = MatlD.Data1.Analysis.Thermal;
                }

                if(k==0)
                {
                    if(bWrite)
                    {
                        Write_Command(_T("GR_BTMP"));
                        Write_Int(BtmpD.GroupKey);
                        Write_String(LdgrD.GroupName);
                        Write_String(StldD.LoadCaseType);
                        Write_String(StldD.LoadCaseName);
                        Write_Int(BtmpK.key.entity);
                        Write_Int(BtmpD.nDirection); // 0:Local-y, 1:Local-z
                        Write_Int(BtmpD.nRefPosition); //0:Centroid, 1:+End(Top), 2:-End(Bot)
                        Write_Double(StypD.dblInitTemp);
                        Write_Double(dElast);
                        Write_Double(dThermal);
                        Write_Double(StmpD.dB);
                        Write_Double(StmpD.dH1);
                        Write_Double(StmpD.dT1);
                        Write_Double(StmpD.dH2);
                        Write_Double(StmpD.dT2);
                    }
                    m_DataNum.iBtmpGrNum++;
                }
                else
                {
                    if(bWrite)
                    {
                        Write_Command(_T("GR_BTMP"), TRUE);
                        Write_NullInt();
                        Write_NullString();
                        Write_NullString();
                        Write_NullString();
                        Write_NullInt();
                        Write_NullInt(); // 0:Local-y, 1:Local-z
                        Write_NullInt(); //0:Centroid, 1:+End(Top), 2:-End(Bot)
                        Write_NullDouble();
                        Write_Double(dElast);
                        Write_Double(dThermal);
                        Write_Double(StmpD.dB);
                        Write_Double(StmpD.dH1);
                        Write_Double(StmpD.dT1);
                        Write_Double(StmpD.dH2);
                        Write_Double(StmpD.dT2);
                    }
                    m_DataNum.iBtmpGrNum++;
                }

            }

        }
    }

    // SDSP
    m_DataNum.iSdspGrNum=0;
    for(int i=0; i<nSize; i++) arKeyList[i].RemoveAll();

    T_SDSP_K SdspK; T_SDSP_D SdspD;
    pos = m_pDoc->m_pAttrCtrl->GetStartSdsp();
    while (pos) 
    {
        int nIX = 0;
        SdspD.Initialize();
        m_pDoc->m_pAttrCtrl->GetNextSdsp(pos, SdspK, SdspD);
        BOOL bExist = KeyPosMap.Lookup(SdspD.LoadCaseKey, nIX);
        ASSERT(bExist);
        arKeyList[nIX].Add(SdspK.keymap);
    }

    for(int i=0; i<nSize; i++)
    {
        T_STLD_K StldK = StldKList[i];
        int nCount = arKeyList[i].GetSize();
        if(nCount<1) continue;

        StldD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetStld(StldK, StldD)) continue;

        T_LDGR_D LdgrD;
        qsort((void*)arKeyList[i].GetData(), nCount, sizeof(UINT), CCompFunc::i20x12Asc);
        for (int j=0; j<nCount; j++)
        {
            SdspK.keymap = arKeyList[i].GetAt(j);
            SdspD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetSdsp(SdspK, SdspD)) continue;

            LdgrD.Initialize();
            if (SdspD.GroupKey == 0) 
            {
                SdspD.GroupKey = 99999999;
                LdgrD.GroupName = _T("Default");
            }
            else m_pDoc->m_pAttrCtrl->GetLdgr(SdspD.GroupKey, LdgrD);

            if(bWrite)
            {
                Write_Command(_T("GR_SDSP"));
                Write_Int(SdspD.GroupKey);
                Write_String(LdgrD.GroupName);
                Write_String(StldD.LoadCaseType);
                Write_String(StldD.LoadCaseName);
                Write_Int(SdspK.key.entity);
                Write_Double(SdspD.Displacement[0]);
                Write_Double(SdspD.Displacement[1]);
                Write_Double(SdspD.Displacement[2]);
                Write_Double(SdspD.Displacement[3]);
                Write_Double(SdspD.Displacement[4]);
                Write_Double(SdspD.Displacement[5]);
            }
            m_DataNum.iSdspGrNum++;
        }
    }

    // Prst(Prestress Beam Load)
    m_DataNum.iPbldGrNum=0;
    for(int i=0; i<nSize; i++) arKeyList[i].RemoveAll();

    T_PRST_K PrstK; T_PRST_D PrstD;
    pos = m_pDoc->m_pAttrCtrl->GetStartPrst();
    while (pos) 
    {
        int nIX = 0;
        PrstD.Initialize();
        m_pDoc->m_pAttrCtrl->GetNextPrst(pos, PrstK, PrstD);
        BOOL bExist = KeyPosMap.Lookup(PrstD.LoadCaseKey, nIX);
        ASSERT(bExist);
        arKeyList[nIX].Add(PrstK.keymap);
    }

    for(int i=0; i<nSize; i++)
    {
        T_STLD_K StldK = StldKList[i];
        int nCount = arKeyList[i].GetSize();
        if(nCount<1) continue;

        StldD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetStld(StldK, StldD)) continue;

        T_LDGR_D LdgrD;
        qsort((void*)arKeyList[i].GetData(), nCount, sizeof(UINT), CCompFunc::i20x12Asc);
        for (int j=0; j<nCount; j++)
        {
            PrstK.keymap = arKeyList[i].GetAt(j);
            PrstD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetPrst(PrstK, PrstD)) continue;

            LdgrD.Initialize();
            if (PrstD.GroupKey == 0) 
            {
                PrstD.GroupKey = 99999999;
                LdgrD.GroupName = _T("Default");
            }
            else m_pDoc->m_pAttrCtrl->GetLdgr(PrstD.GroupKey, LdgrD);

            if(bWrite)
            {
                Write_Command(_T("GR_PBLD"));
                Write_Int(PrstD.GroupKey);
                Write_String(LdgrD.GroupName);
                Write_String(StldD.LoadCaseType);
                Write_String(StldD.LoadCaseName);
                Write_Int(PrstK.key.entity);
                Write_Double(PrstD.dTension);
                Write_Double(PrstD.dDistanceI);
                Write_Double(PrstD.dDistanceM);
                Write_Double(PrstD.dDistanceJ);
            }
            m_DataNum.iPbldGrNum++;
        }
    }

    // Ptns(Pretension Load)
    m_DataNum.iPtnsGrNum=0;
    for(int i=0; i<nSize; i++) arKeyList[i].RemoveAll();

    T_PTNS_K PtnsK; T_PTNS_D PtnsD;
    pos = m_pDoc->m_pAttrCtrl->GetStartPtns();
    while (pos) 
    {
        int nIX = 0;
        PtnsD.Initialize();
        m_pDoc->m_pAttrCtrl->GetNextPtns(pos, PtnsK, PtnsD);
        BOOL bExist = KeyPosMap.Lookup(PtnsD.LoadCaseKey, nIX);
        ASSERT(bExist);
        arKeyList[nIX].Add(PtnsK.keymap);
    }

    for(int i=0; i<nSize; i++)
    {
        T_STLD_K StldK = StldKList[i];
        int nCount = arKeyList[i].GetSize();
        if(nCount<1) continue;

        StldD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetStld(StldK, StldD)) continue;

        T_LDGR_D LdgrD;
        qsort((void*)arKeyList[i].GetData(), nCount, sizeof(UINT), CCompFunc::i20x12Asc);
        for (int j=0; j<nCount; j++)
        {
            PtnsK.keymap = arKeyList[i].GetAt(j);
            PtnsD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetPtns(PtnsK, PtnsD)) continue;

            LdgrD.Initialize();
            if (PtnsD.GroupKey == 0) 
            {
                PtnsD.GroupKey = 99999999;
                LdgrD.GroupName = _T("Default");
            }
            else m_pDoc->m_pAttrCtrl->GetLdgr(PtnsD.GroupKey, LdgrD);

            if(bWrite)
            {
                Write_Command(_T("GR_PTNS"));
                Write_Int(PtnsD.GroupKey);
                Write_String(LdgrD.GroupName);
                Write_String(StldD.LoadCaseType);
                Write_String(StldD.LoadCaseName);
                Write_Int(PtnsK.key.entity);
                Write_Double(PtnsD.dTension);
            }
            m_DataNum.iPtnsGrNum++;
        }
    }

    // Stmp
    m_DataNum.iStmpGrNum=0;
    for(int i=0; i<nSize; i++) arKeyList[i].RemoveAll();

    T_STMP_K StmpK; T_STMP_D StmpD;
    pos = m_pDoc->m_pAttrCtrl->GetStartStmp();
    while (pos) 
    {
        int nIX = 0;
        StmpD.Initialize();
        m_pDoc->m_pAttrCtrl->GetNextStmp(pos, StmpK, StmpD);
        BOOL bExist = KeyPosMap.Lookup(StmpD.LoadCaseKey, nIX);
        ASSERT(bExist);
        arKeyList[nIX].Add(StmpK);
    }

    for(int i=0; i<nSize; i++)
    {
        T_STLD_K StldK = StldKList[i];
        int nCount = arKeyList[i].GetSize();
        if(nCount<1) continue;

        StldD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetStld(StldK, StldD)) continue;

        T_LDGR_D LdgrD;
        qsort((void*)arKeyList[i].GetData(), nCount, sizeof(UINT), CCompFunc::UINTAsc);
        for (int j=0; j<nCount; j++)
        {
            StmpK = arKeyList[i].GetAt(j);
            StmpD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetStmp(StmpK, StmpD)) continue;

            LdgrD.Initialize();
            if (StmpD.GroupKey == 0) 
            {
                StmpD.GroupKey = 99999999;
                LdgrD.GroupName = _T("Default");
            }
            else m_pDoc->m_pAttrCtrl->GetLdgr(StmpD.GroupKey, LdgrD);

            if(bWrite)
            {
                Write_Command(_T("GR_STMP"));
                Write_Int(StmpD.GroupKey);
                Write_String(LdgrD.GroupName);
                Write_String(StldD.LoadCaseType);
                Write_String(StldD.LoadCaseName);
                double dTemp = StmpD.dblSysTemp - StypD.dblInitTemp;
                Write_Double(dTemp);
            }
            m_DataNum.iStmpGrNum++;
        }
    }

    // Ntmp
    m_DataNum.iNtmpGrNum=0;
    for(int i=0; i<nSize; i++) arKeyList[i].RemoveAll();

    T_NTMP_K NtmpK; T_NTMP_D NtmpD;
    pos = m_pDoc->m_pAttrCtrl->GetStartNtmp();
    while (pos) 
    {
        int nIX = 0;
        NtmpD.Initialize();
        m_pDoc->m_pAttrCtrl->GetNextNtmp(pos, NtmpK, NtmpD);
        BOOL bExist = KeyPosMap.Lookup(NtmpD.LoadCaseKey, nIX);
        ASSERT(bExist);
        arKeyList[nIX].Add(NtmpK.keymap);
    }

    for(int i=0; i<nSize; i++)
    {
        T_STLD_K StldK = StldKList[i];
        int nCount = arKeyList[i].GetSize();
        if(nCount<1) continue;

        StldD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetStld(StldK, StldD)) continue;

        T_LDGR_D LdgrD;
        qsort((void*)arKeyList[i].GetData(), nCount, sizeof(UINT), CCompFunc::i20x12Asc);
        for (int j=0; j<nCount; j++)
        {
            NtmpK.keymap = arKeyList[i].GetAt(j);
            NtmpD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetNtmp(NtmpK, NtmpD)) continue;

            LdgrD.Initialize();
            if (NtmpD.GroupKey == 0) 
            {
                NtmpD.GroupKey = 99999999;
                LdgrD.GroupName = _T("Default");
            }
            else m_pDoc->m_pAttrCtrl->GetLdgr(NtmpD.GroupKey, LdgrD);

            if(bWrite)
            {
                Write_Command(_T("GR_NTMP"));
                Write_Int(NtmpD.GroupKey);
                Write_String(LdgrD.GroupName);
                Write_String(StldD.LoadCaseType);
                Write_String(StldD.LoadCaseName);
                Write_Int(NtmpK.key.entity);
                double dTemp = NtmpD.dblNodalTemp - StypD.dblInitTemp;
                Write_Double(dTemp);
            }
            m_DataNum.iNtmpGrNum++;
        }
    }

    delete[] arKeyList;


    // Settlement Analysis
    m_DataNum.iSmptGrNum=0;
    CArray<UINT,UINT> arSmlcKList; arSmlcKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetSmlcKeyList(arSmlcKList);
    nSize = arSmlcKList.GetSize();

    for(int i=0; i<nSize; i++)
    {
        T_SMLC_K SmlcK = arSmlcKList[i];
        T_SMLC_D SmlcD; SmlcD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetSmlc(SmlcK, SmlcD)) continue;

        int nCount = SmlcD.aPointData.GetSize();
        if(nCount<1) continue;

        qsort((void*)SmlcD.aPointData.GetData(), nCount, sizeof(UINT), CCompFunc::i20x12Asc);
        for (int j=0; j<nCount; j++)
        {
            T_SMPT_K SmptK = SmlcD.aPointData.GetAt(j);
            T_SMPT_D SmptD; SmptD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetSmpt(SmptK, SmptD)) continue;

            for(int k=0; k<SmptD.aNodeData.GetSize(); k++)
            {
                if(bWrite)
                {
                    Write_Command(_T("GR_SMPT"));
                    Write_Int(SmptD.GroupId);
                    Write_String(SmptD.GroupName);
                    Write_String(_T("SET"));
                    Write_String(SmlcD.LoadCaseName);
                    Write_Int(SmptD.aNodeData.GetAt(k));
                    Write_Double(SmptD.dSettle*SmlcD.dFactor);
                }
                m_DataNum.iSmptGrNum++;
            }
        }
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_StagInfo(T_STAG_K StagK, T_STAG_D& StagD)
{
    Write_Command(_T("SG_DATA"));
    Write_Int(StagK);
    Write_String(StagD.StageName);
    Write_Double(StagD.dDuration);
    return TRUE;
}

BOOL CCRCExportFile::Write_ActiveElemGrup(T_STAG_K StagK, T_STAG_D& StagD)
{

    T_STAG_AELEM_BASE ElemGrup;
    for(int i=0; i<StagD.aActiveElem.GetSize(); i++)
    {
        if(i==0)
        {
            Write_Command(_T("SE_AELE"));
            Write_Int(StagK);
        }
        ElemGrup = StagD.aActiveElem.GetAt(i);
        if(!Write_Enable(24))
        {
            Write_Command(_T("SE_AELE"), TRUE);
            Write_NullInt();
            Write_Int((ElemGrup.GrupKey==0 ? 99999999 : ElemGrup.GrupKey));
            Write_Double(ElemGrup.dAge);
        }
        else
        {
            Write_Int((ElemGrup.GrupKey==0 ? 99999999 : ElemGrup.GrupKey));
            Write_Double(ElemGrup.dAge);
        }
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_InactiveElemGrup(T_STAG_K StagK, T_STAG_D& StagD)
{

    T_STAG_DELEM_BASE ElemGrup;
    for(int i=0; i<StagD.aDeactiveElem.GetSize(); i++)
    {
        if(i==0)
        {
            Write_Command(_T("SE_DELE"));
            Write_Int(StagK);
        }
        ElemGrup = StagD.aDeactiveElem.GetAt(i);
        if(!Write_Int((ElemGrup.GrupKey==0 ? 99999999 : ElemGrup.GrupKey)))
        {
            Write_Command(_T("SE_DELE"), TRUE);
            Write_NullInt();
            Write_Int((ElemGrup.GrupKey==0 ? 99999999 : ElemGrup.GrupKey));
        }
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_ActiveSprtGrup(T_STAG_K StagK, T_STAG_D& StagD)
{

    T_STAG_BNDR_BASE BndrGrup;
    for(int i=0; i<StagD.aActiveBndr.GetSize(); i++)
    {
        if(i==0)
        {
            Write_Command(_T("SN_ASUP"));
            Write_Int(StagK);
        }
        BndrGrup = StagD.aActiveBndr.GetAt(i);
        if(!Write_Int((BndrGrup.BngrKey==0 ? 99999999 : BndrGrup.BngrKey)))
        {
            Write_Command(_T("SN_ASUP"), TRUE);
            Write_NullInt();
            Write_Int((BndrGrup.BngrKey==0 ? 99999999 : BndrGrup.BngrKey));
        }
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_InactiveSprtGrup(T_STAG_K StagK, T_STAG_D& StagD)
{

    for(int i=0; i<StagD.aDeactiveBndr.GetSize(); i++)
    {
        if(i==0)
        {
            Write_Command(_T("SN_DSUP"));
            Write_Int(StagK);
        }
        T_BNGR_K BngrK = StagD.aDeactiveBndr.GetAt(i);
        if(!Write_Int((BngrK==0 ? 99999999 : BngrK)))
        {
            Write_Command(_T("SN_DSUP"), TRUE);
            Write_NullInt();
            Write_Int((BngrK==0 ? 99999999 : BngrK));
        }
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_ActiveLoadGrup(T_STAG_K StagK, T_STAG_D& StagD)
{

    T_STAG_LOAD_BASE LoadGrup;
    for(int i=0; i<StagD.aActiveLoad.GetSize(); i++)
    {
        if(i==0)
        {
            Write_Command(_T("SL_ALGD"));
            Write_Int(StagK);
        }
        LoadGrup = StagD.aActiveLoad.GetAt(i);
        double dDay = LoadGrup.dDay;
        if(dDay == -1.0) dDay = StagD.dDuration;
        if(!Write_Enable(24))
        {
            Write_Command(_T("SL_ALGD"), TRUE);
            Write_NullInt();
            Write_Int((LoadGrup.LdgrKey==0 ? 99999999 : LoadGrup.LdgrKey));
            Write_Double(dDay);
        }
        else
        {
            Write_Int((LoadGrup.LdgrKey==0 ? 99999999 : LoadGrup.LdgrKey));
            Write_Double(dDay);
        }
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_InactiveLoadGrup(T_STAG_K StagK, T_STAG_D& StagD)
{

    T_STAG_LOAD_BASE LoadGrup;
    for(int i=0; i<StagD.aDeactiveLoad.GetSize(); i++)
    {
        if(i==0)
        {
            Write_Command(_T("SL_DLGD"));
            Write_Int(StagK);
        }
        LoadGrup = StagD.aDeactiveLoad.GetAt(i);
        double dDay = LoadGrup.dDay;
        if(dDay == -1.0) dDay = StagD.dDuration;
        if(!Write_Enable(24))
        {
            Write_Command(_T("SL_DLGD"), TRUE);
            Write_NullInt();
            Write_Int((LoadGrup.LdgrKey==0 ? 99999999 : LoadGrup.LdgrKey));
            Write_Double(dDay);
        }
        else
        {
            Write_Int((LoadGrup.LdgrKey==0 ? 99999999 : LoadGrup.LdgrKey));
            Write_Double(dDay);
        }
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_TndnGrupData()
{
    T_STCT_D StctD; StctD.Initialize();
    m_pDoc->m_pAttrCtrl->GetStct(StctD);

    T_TDNA_K TdnaK; T_TDNA_D TdnaD;

    CArray<T_TDNA_K, T_TDNA_K> TdnaKList; TdnaKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetTdnaKeyList(TdnaKList);

    _DGN_TDGR_CRC TdgrCrc;
    for(int i=0; i<TdnaKList.GetSize(); i++)
    {
        TdgrCrc.Initialize();
        TdnaK = TdnaKList.GetAt(i);
        TdnaD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetTdna(TdnaK,TdnaD))	continue;
        if(m_DataCtrl.m_amTdnaK.Lookup(TdnaK,TdgrCrc)==0)	continue;
        T_TENDON_INFO TendonInfo;
        if(!m_pDoc->m_pPostCtrl->GetTendonInfo()->GetTendonInfo(TdnaK, TendonInfo)) continue;

        double dNoTensionBeginLen = TendonInfo.dNoTensionBeginLen;
        double dNoTensionEndLen = TendonInfo.dNoTensionEndLen;
        if(!StctD.bConsiderEndPartStress)
        {
            dNoTensionBeginLen = 0.0;
            dNoTensionEndLen = 0.0;
        }

        double dInsertPointX = 0.0, dInsertPointY=0.0, dInsertPointZ=0.0;
        if(TdnaD.nTendonShape == 0 || TdnaD.nTendonShape == 1)
        {
            dInsertPointX = TdnaD.dInsertPointX;
            dInsertPointY = TdnaD.dInsertPointY;
            dInsertPointZ = TdnaD.dInsertPointZ;
        }
        else if(TdnaD.nTendonShape == 2) //Element
        {
            T_ELEM_D ElemD; ElemD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetElem(TdnaD.InsertElemKey, ElemD)) 
            {
                ASSERT(0); continue;
            }
            T_NODE_D NodeD; NodeD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[TdnaD.nInsertPoint], NodeD))
            {
                ASSERT(0); continue;
            }
            dInsertPointX = NodeD.x;
            dInsertPointY = NodeD.y;
            dInsertPointZ = NodeD.z;
        }
        else ASSERT(0);

        Write_Command(_T("GR_TNDN"));
        Write_Int(TdgrCrc.DgnTdgrK);
        Write_String(TdgrCrc.strTdgrNa);

        Write_Int(TdnaK);
        Write_String(TdnaD.TendonName);
        Write_Int(TdnaD.TendonTypeKey);

        Write_Double(dNoTensionBeginLen);
        Write_Double(dNoTensionEndLen);

        Write_Double(dInsertPointX);
        Write_Double(dInsertPointY);
        Write_Double(dInsertPointZ);
        // Add by ZINU.('06.02.13). 대표텐던.
        Write_Double(TdnaD.dTendonNum);
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_TndnProfileData()
{
    T_TDNA_K TdnaK; T_TDNA_D TdnaD;

    CArray<T_TDNA_K, T_TDNA_K> TdnaKList; TdnaKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetTdnaKeyList(TdnaKList);

    for(int i=0; i<TdnaKList.GetSize(); i++)
    {
        TdnaK = TdnaKList.GetAt(i);
        TdnaD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetTdna(TdnaK, TdnaD)) continue;

        if(TdnaD.nCurveType==0) // 0:Spline
        {
            if(TdnaD.nInputType==0) //2D
            {
                for(int k=0; k<2; k++)
                {
                    Write_Command(_T("T_S2D"));
                    Write_Int(TdnaK);
                    Write_Int(k+1);

                    CArray<T_TDNA_BASE_2D, T_TDNA_BASE_2D&> aProfile;
                    aProfile.RemoveAll();
                    if(k==0)  aProfile.Copy(TdnaD.aProfileY);
                    else      aProfile.Copy(TdnaD.aProfileZ);
                    for(int l=0; l<aProfile.GetSize(); l++)
                    {
                        if(l!=0)
                        {
                            Write_Command(_T("T_S2D"), TRUE);
                            Write_NullInt();
                            Write_NullInt();
                        }
                        Write_Double(aProfile.GetAt(l).x);
                        Write_Double(aProfile.GetAt(l).yz);
                        Write_Int((aProfile.GetAt(l).bFix ? 1 : 0));
                        Write_Double(aProfile.GetAt(l).dR*DEGTORAD); //Deg
                        // Add by ZINU.('06.05.09). 하연입력 옵션처리.
                        Write_Double(aProfile.GetAt(l).bBottomZ);
                    }
                }
            }
            else //3D
            {
                Write_Command(_T("T_S3D"));
                Write_Int(TdnaK);

                for(int k=0; k<TdnaD.aProfile.GetSize(); k++)
                {
                    if(k!=0)
                    {
                        Write_Command(_T("T_S3D"), TRUE);
                        Write_NullInt();
                    }
                    Write_Double(TdnaD.aProfile.GetAt(k).x);
                    Write_Double(TdnaD.aProfile.GetAt(k).y);
                    Write_Double(TdnaD.aProfile.GetAt(k).z);
                    Write_Int((TdnaD.aProfile.GetAt(k).bFix ? 1 : 0));
                    Write_Double(TdnaD.aProfile.GetAt(k).dRy*DEGTORAD); //Deg
                    Write_Double(TdnaD.aProfile.GetAt(k).dRz*DEGTORAD); //Deg
                }
            }
        }
        else if(TdnaD.nCurveType==1) //1:Round
        {
            if(TdnaD.nInputType==0) //2D
            {
                for(int k=0; k<2; k++)
                {
                    Write_Command(_T("T_R2D"));
                    Write_Int(TdnaK);
                    Write_Int(k+1);

                    CArray<T_TDNA_BASE_2D, T_TDNA_BASE_2D&> aProfile;
                    aProfile.RemoveAll();
                    if(k==0)  aProfile.Copy(TdnaD.aProfileY);
                    else      aProfile.Copy(TdnaD.aProfileZ);
                    for(int l=0; l<aProfile.GetSize(); l++)
                    {
                        if(l!=0)
                        {
                            Write_Command(_T("T_R2D"), TRUE);
                            Write_NullInt();
                            Write_NullInt();
                        }
                        Write_Double(aProfile.GetAt(l).x);
                        Write_Double(aProfile.GetAt(l).yz);
                        Write_Double(aProfile.GetAt(l).dRadius);
                        //추가된 사항 입력
                        Write_Int(aProfile.GetAt(l).nAddOpt);
                        Write_Double(aProfile.GetAt(l).dAngle*DEGTORAD);
                        Write_Double(aProfile.GetAt(l).dHeight);
                        Write_Double(aProfile.GetAt(l).dRadius2);
                        // Add by ZINU.('06.05.09). 하연입력 옵션처리.
                        Write_Double(aProfile.GetAt(l).bBottomZ);
                    }
                }
            }
            else //3D
            {
                Write_Command(_T("T_R3D"));
                Write_Int(TdnaK);

                for(int k=0; k<TdnaD.aProfile.GetSize(); k++)
                {
                    if(k!=0)
                    {
                        Write_Command(_T("T_R3D"), TRUE);
                        Write_NullInt();
                    }
                    Write_Double(TdnaD.aProfile.GetAt(k).x);
                    Write_Double(TdnaD.aProfile.GetAt(k).y);
                    Write_Double(TdnaD.aProfile.GetAt(k).z);
                    Write_Double(TdnaD.aProfile.GetAt(k).dRadius);
                }
            }
        }
        else ASSERT(0);
    }
    return TRUE;
}

BOOL CCRCExportFile::Write_TndnForceData()
{
    if(!m_bStag) return TRUE;

    CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> ElemKMap;
    ElemKMap.RemoveAll();
    ElemKMap.InitHashTable(m_PSCBeamPostD.PElemKList.GetSize());
    for(int i=0; i<m_PSCBeamPostD.PElemKList.GetSize(); i++)
    {
        T_ELEM_K ElemK = m_PSCBeamPostD.PElemKList.GetAt(i);
        ElemKMap.SetAt(ElemK,ElemK);
    }

    T_TDNA_K TdnaK; T_TDNA_D TdnaD;

    int iFinalStepNo = m_pDoc->m_pPostCtrl->GetMaxStep_SG();

    CArray<T_TDNA_K, T_TDNA_K> TdnaKList; TdnaKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetTdnaKeyList(TdnaKList);

    for(int i=0; i<TdnaKList.GetSize(); i++)
    {
        TdnaK = TdnaKList.GetAt(i);
        TdnaD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetTdna(TdnaK, TdnaD)) continue;

        T_TDNT_D TdntD; TdntD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetTdnt(TdnaD.TendonTypeKey, TdntD)) continue;
        double dTdntArea = TdntD.dTendonArea;
        if(dTdntArea==0.0) continue;

        for(int j=0; j<TdnaD.aElemList.GetSize(); j++)
        {
            T_ELEM_K ElemK = TdnaD.aElemList.GetAt(j);
            T_ELEM_K rElemK = 0;
            if(!ElemKMap.Lookup(ElemK, rElemK)) continue;

            double dLen	= m_pDoc->calcLAVElem(ElemK);

			CArray<T_TDFI_D, T_TDFI_D&> aTdfiD;
            CArray<double, double> arFrictionForce;
            arFrictionForce.RemoveAll();
			aTdfiD.RemoveAll();
            //if(!m_pDoc->m_pPostCtrl->GetTdfi(TdnaK, ElemK, 1, arFrictionForce, TRUE)) continue;
			if(!m_pDoc->m_pPostCtrl->GetTdfi(TdnaK, ElemK, 1, aTdfiD, TRUE)) continue;

			int nNumTdfi = aTdfiD.GetSize();
			arFrictionForce.Copy(aTdfiD[nNumTdfi-1].aTdfi); // 마지막 단계
            double dFriForceI = arFrictionForce.GetAt(0);
            double dFriForceJ = arFrictionForce.GetAt(arFrictionForce.GetSize()-1);

            CArray<double, double> arSetForce;
            arSetForce.RemoveAll();
			aTdfiD.RemoveAll();
            //if(!m_pDoc->m_pPostCtrl->GetTdfi(TdnaK, ElemK, 2, arSetForce, TRUE)) continue;
			if(!m_pDoc->m_pPostCtrl->GetTdfi(TdnaK, ElemK, 2, aTdfiD, TRUE)) continue;
			
			nNumTdfi = aTdfiD.GetSize();
			arSetForce.Copy(aTdfiD[nNumTdfi-1].aTdfi);
            double dSetForceI = arSetForce.GetAt(0);
            double dSetForceJ = arSetForce.GetAt(arSetForce.GetSize()-1);

            CArray<T_TDLC_D, T_TDLC_D&> arTendonLoss;
            arTendonLoss.RemoveAll();
            if(!m_pDoc->m_pPostCtrl->GetTdlc(TdnaK, ElemK, iFinalStepNo, arTendonLoss, TRUE)) continue;

            T_TDLC_D ITdlcD = arTendonLoss.GetAt(0);
            T_TDLC_D JTdlcD = arTendonLoss.GetAt(arTendonLoss.GetSize()-1);

            T_TENDON_ELEMINFO TndnElemInfo;
            if(!m_pDoc->m_pPostCtrl->GetTendonInfo()->GetTendonElemInfo(ElemK, TndnElemInfo)) continue;
            int iTndnNum = TndnElemInfo.aTendonElemInfoTendon.GetSize();

            double dLimZero = 1.0E-07;
            double dXPosI=0.0, dYPosI=0.0, dZPosI=0.0, dXPosJ=0.0, dYPosJ=0.0, dZPosJ=0.0;
            BOOL bFindI=FALSE, bFindJ=FALSE;
            for(int l=0; l<iTndnNum; l++)
            {
                T_TENDON_ELEMINFO_TENDON TndnData = TndnElemInfo.aTendonElemInfoTendon.GetAt(l);
                // Add by ZINU.('05.09.14). Check if Prestressed Tendon.
                if(!TndnData.bPrestressed)	continue;
                if(TndnData.KeyTdna == TdnaK)
                {
                    T_TENDON_ELEMINFO_TENDON_BASE TndnBaseI = TndnData.aTendonElemInfoTendonBase.GetAt(0);
                    dXPosI = dLen * TndnBaseI.dPositionRatio_x;

                    T_TENDON_ELEMINFO_TENDON_BASE TndnBaseJ = TndnData.aTendonElemInfoTendonBase.GetAt(TndnData.aTendonElemInfoTendonBase.GetSize()-1);
                    dXPosJ = dLen * TndnBaseJ.dPositionRatio_x;

                    if(dXPosI > dXPosJ) //reverse
                    {
                        double dXPos = dXPosI;
                        dXPosI = dXPosJ;
                        dXPosJ = dXPos;

                        double dFriForce = dFriForceI;
                        dFriForceI = dFriForceJ;
                        dFriForceJ = dFriForce;

                        double dSetForce = dSetForceI;
                        dSetForceI = dSetForceJ;
                        dSetForceJ = dSetForce;

                        T_TDLC_D TdlcD = ITdlcD;
                        ITdlcD = JTdlcD;
                        JTdlcD = TdlcD;

                        T_TENDON_ELEMINFO_TENDON_BASE TndnBase = TndnBaseI;
                        TndnBaseI = TndnBaseJ;
                        TndnBaseJ = TndnBase;
                    }

                    if(dXPosI <= dLimZero)
                    {
                        bFindI = TRUE;
                        dYPosI = TndnBaseI.dDistance_y;
                        dZPosI = TndnBaseI.dDistance_z;
                    }

                    if(dXPosJ >= dLen-dLimZero && dXPosJ <= dLen+dLimZero)
                    {
                        bFindJ = TRUE;
                        dYPosJ = TndnBaseJ.dDistance_y;
                        dZPosJ = TndnBaseJ.dDistance_z;
                    }

                    if(bFindI || bFindJ) break;
                }
            }

            if(!bFindI && !bFindJ) continue;

            //Pos I
            Write_Command(_T("T_RES"));
            Write_Int(TdnaK);
            Write_Int(ElemK);
            Write_Int(1); 
            Write_Double(dYPosI);
            Write_Double(dZPosI);
            Write_Double((bFindI ? ITdlcD.dTendonLoss[0]/dTdntArea : 0.0));
            Write_Double((bFindI ? ITdlcD.dTendonLoss[1]/dTdntArea : 0.0));
            Write_Double((bFindI ? ITdlcD.dTendonLoss[2]/dTdntArea : 0.0));
            Write_Double((bFindI ? dFriForceI/dTdntArea : 0.0));
            Write_Double((bFindI ? dSetForceI/dTdntArea : 0.0));

            //Pos J
            Write_Command(_T("T_RES"), TRUE);
            Write_NullInt();
            Write_NullInt();
            Write_Int(2);
            Write_Double(dYPosJ);
            Write_Double(dZPosJ);
            Write_Double((bFindJ ? JTdlcD.dTendonLoss[0]/dTdntArea : 0.0));
            Write_Double((bFindJ ? JTdlcD.dTendonLoss[1]/dTdntArea : 0.0));
            Write_Double((bFindJ ? JTdlcD.dTendonLoss[2]/dTdntArea : 0.0));
            Write_Double((bFindJ ? dFriForceJ/dTdntArea : 0.0));
            Write_Double((bFindJ ? dSetForceJ/dTdntArea : 0.0));
        }
    }

    return TRUE;
}

//전체 스테이지 중 현재스테이지의 마지막 스탭번호 받아오는 함수
BOOL CCRCExportFile::Get_LastStepInCurStage(T_STAG_K StagK, int& iFinalStepNo)
{
    T_STAG_STEP_INFO stage_info;
    m_pDoc->m_pAttrCtrl->GetStagStepInfo(StagK,stage_info);
    int iStep = stage_info.aInfo.GetSize();
    m_pDoc->m_pPostCtrl->GetStageInfo()->GetStepSerialKey(StagK,iStep,iFinalStepNo);

    return TRUE;
}

BOOL CCRCExportFile::Write_Dgn_Data()
{
    if(!Write_Dgn_AnalData())			return FALSE;
    if(!Write_Dgn_CtrlData())			return FALSE;
    if(!Write_Dgn_ExtTndnData())	return FALSE;
    if(!Write_Dgn_ChkPosData())		return FALSE;
    if(!Write_Dgn_MomReduData())	return FALSE;
    if(!Write_Dgn_LComData())			return FALSE;
    if(!Write_Dgn_RBarData())			return FALSE;
    if(!Write_Dgn_SBarData())			return FALSE;
    if(!Write_Dgn_SheetData())		return FALSE;

    return TRUE;
}

BOOL CCRCExportFile::Write_Dgn_AnalData()
{
    // Add by ZINU.('06.02.16). Write Analysis, Construction Stage Control.
    T_ACTL_D ActlD; ActlD.Initialize();
    m_pDoc->m_pAttrCtrl->GetActl(ActlD);

    T_STCT_D StctD; StctD.Initialize();
    m_pDoc->m_pAttrCtrl->GetStct(StctD);

    Write_Command(_T("A_OPT"));
    Write_Int(ActlD.bConsiderSecf ? 1 : 0);					// 응력도 계산용으로 단면 성능 증감계수를 고려 (OFF=0, ON=1)
    Write_Int(ActlD.bConsiderRbar ? 1 : 0);					// 단면성능에 철근의 영향을 고려 (OFF=0, ON=1)
    Write_Int(ActlD.bConsiderSectOffset ? 1 : 0);		// 단면 편심을 PC강재 긴장력과 표시에만 적용 (OFF=0, ON=1)
    Write_Int(StctD.bTTLECreepShrinkage ? 1 : 0);		// PC강재의 크리프, 건조수축 손실을 고려 (OFF=0, ON=1)
    Write_Int(StctD.bConsiderConfinement ? 1 : 0);	// 철근 구속력을 고려 (OFF=0, ON=1)
    Write_Int(StctD.bVariation ? 1 : 0);						// 압축 강도의 변화를 고려 (OFF=0, ON=1)
    Write_Int(StctD.bTTLEElasticShortening ? 1 : 0);// 탄성변형 손실을 고려 (OFF=0, ON=1)
    Write_Int(StctD.bConsiderEndPartStress ? (StctD.nEndPartStress+1) : 0);	// 무효장 계산 (OFF=0, 선형=1, 일정=2)
    Write_Double(StctD.nEndPartStress==1 ? StctD.dStressRatio : 0.0);				// IC5=2 때에 입력하는 비율

    return TRUE;
}

BOOL CCRCExportFile::Write_Dgn_CtrlData()
{
    T_PSCD_D PscdD; PscdD.Initialize();
    m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);

    Write_Command(_T("G_CTL"));
    Write_Int(PscdD.iDimType+1);
    Write_Int(PscdD.iBrgType+1 + (PscdD.bCmpWeb ? 10 : 0));
    Write_Int(PscdD.iCrackCode+1);
    Write_Int(PscdD.iCrackWidth[0]+1);
    Write_Int(PscdD.iCrackWidth[1]+1);
    Write_Int(PscdD.iPsReduFactor+1);

    return TRUE;
}

BOOL CCRCExportFile::Write_Dgn_ExtTndnData()
{
    // Add by ZINU.('06.02.16). Check Bridge Direction.
    if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

    CArray<T_TDNA_K, T_TDNA_K> TdnaKList; TdnaKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetTdnaKeyList(TdnaKList);

    T_TDNA_D TdnaD;
    T_TDNT_D TdntD;

    int iCount = 0;
    for(int i=0; i<TdnaKList.GetSize(); i++)
    {
        T_TDNA_K TdnaK = TdnaKList.GetAt(i);
        TdnaD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetTdna(TdnaK, TdnaD)) continue;
        TdntD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetTdnt(TdnaD.TendonTypeKey,TdntD)) continue;

        if(iCount == 0) Write_Command(_T("T_EXT"));

        if(!Write_Enable(40))
        {
            Write_Command(_T("T_EXT"));
            Write_Int(TdnaK);
            Write_Double(TdntD.dAlphaExtCable);
        }
        else
        {
            Write_Int(TdnaK);
            Write_Double(TdntD.dAlphaExtCable);
        }
        iCount++;
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_Dgn_ChkPosData()
{
    int iCount = 0;
    T_POSC_D PoscD; 
    for(int i=0; i<m_PSCBeamPostD.PElemKList.GetSize(); i++)
    {
        T_ELEM_K ElemK = m_PSCBeamPostD.PElemKList.GetAt(i);
        ElemPairK EPairK(ElemK, EN_EL_BEAM);
        // Change by ZINU.('04.04.26). Set Default Values.
        PoscD.Initialize();
        if(!m_DataCtrl.Get_CrcPosc(EPairK, PoscD))	continue;

        if(iCount == 0) Write_Command(_T("E_POS"));

        if(!Write_Enable(24))
        {
            Write_Command(_T("E_POS"));
            Write_Int(ElemK);
            Write_Int(PoscD.iMomChk);
            Write_Int(PoscD.iShrChk);
        }
        else
        {
            Write_Int(ElemK);
            Write_Int(PoscD.iMomChk);
            Write_Int(PoscD.iShrChk);
        }

        iCount++;
    }


    return TRUE;
}

BOOL CCRCExportFile::Write_Dgn_MomReduData()
{
    // Add by ZINU.('06.02.16). Check Bridge Direction.
    if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

    T_NMRD_D NmrdD;
    NmrdD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetNmrd(NmrdD)) return TRUE;

    int iCount = 0;
    T_NMRD_CASE_BASE CaseD;
    for(int i=0; i<NmrdD.aCaseInfo.GetSize(); i++)
    {
        CaseD = NmrdD.aCaseInfo.GetAt(i);

        CString strLcType = _T("");
        if(CaseD.nLoadType==D_LCOM_STATIC)
        {
            T_STLD_D StldD; StldD.Initialize();
            if(!m_pDoc->m_pPostCtrl->GetStld(CaseD.LoadCaseK, StldD)) continue;
            strLcType = StldD.LoadCaseName;
        }
        else if(CaseD.nLoadType==D_LCOM_SPECTRUM)
        {
            T_SPLC_D SplcD; SplcD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetSplc(CaseD.LoadCaseK, SplcD)) continue;
            strLcType = SplcD.LoadCaseName;
        }
        else if(CaseD.nLoadType==D_LCOM_HISTORY)
        {
            T_THIS_D ThisD; ThisD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetThis(CaseD.LoadCaseK, ThisD)) continue;
            strLcType = ThisD.LoadCaseName;
        }
        else if(CaseD.nLoadType==D_LCOM_MOVING)
        {
            T_MVCD_D MvcdD;
            if(!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) MvcdD.Initialize();

            switch (MvcdD.nCodeType)
            {
            case D_MOVE_CODE_NONE:
                continue;
            case D_MOVE_CODE_AASHTO_STAN:
            case D_MOVE_CODE_AASHTO_LRFD:
            case D_MOVE_CODE_TAIWAN:
            case D_MOVE_CODE_KOREA:
            case D_MOVE_CODE_CANADA:
            case D_MOVE_CODE_PENDOT:
            case D_MOVE_CODE_EURO_BS:
            case D_MOVE_CODE_RUSSIA:
            case D_MOVE_CODE_KOREA_LRFD_2011:
            case D_MOVE_CODE_AUSTRALIA:
            case D_MOVE_CODE_POLAND:
            case D_MOVE_CODE_SOUTH_AFRICA:
            case D_MOVE_CODE_NEWZEALAND:
            case D_MOVE_CODE_BRAZIL:
                {
                    T_MVLD_D MvldD; MvldD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetMvld(CaseD.LoadCaseK, MvldD)) continue;
                    strLcType = MvldD.LoadCaseName;
                }
                break;
            case D_MOVE_CODE_CHINA:
                {
                    T_MVLDch_D MvldchD; MvldchD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetMvldch(CaseD.LoadCaseK, MvldchD)) continue;
                    strLcType = MvldchD.LoadCaseName;
                }
                break;
            case D_MOVE_CODE_JAPAN:
                {
                    T_MVLDjp_D MvldjpD;
                    if(!m_pDoc->m_pAttrCtrl->GetMvldjp(CaseD.LoadCaseK, MvldjpD)) continue;
                    strLcType = MvldjpD.LoadCaseName;
                }
                break;
            case D_MOVE_CODE_INDIA:
                {
                    T_MVLDid_D MvldidD; MvldidD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetMvldid(CaseD.LoadCaseK, MvldidD)) continue;
                    strLcType = MvldidD.LoadCaseName;
                }
                break;
            case D_MOVE_CODE_BS:
                {
                    T_MVLDbs_D MvldbsD; MvldbsD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetMvldbs(CaseD.LoadCaseK, MvldbsD)) continue;
                    strLcType = MvldbsD.LoadCaseName;
                }
                break;
            case D_MOVE_CODE_FRANCE:
                {
                    T_MVLDfr_D MvldfrD; MvldfrD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetMvldfr(CaseD.LoadCaseK, MvldfrD)) continue;
                    strLcType = MvldfrD.LoadCaseName;
                }
                break;
            case D_MOVE_CODE_TRANS:
                {
                    T_MVLDtr_D MvldtrD; MvldtrD.Initialize();
                    if(!m_pDoc->m_pAttrCtrl->GetMvldtr(CaseD.LoadCaseK, MvldtrD)) continue;
                    strLcType = MvldtrD.LoadCaseName;
                }
                break;
            default:
                ASSERT(0);
                continue;
            }
        }
        else if(CaseD.nLoadType==D_LCOM_SETTLEMENT)
        {
            T_SMLC_D SmlcD; SmlcD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetSmlc(CaseD.LoadCaseK, SmlcD)) continue;
            strLcType = SmlcD.LoadCaseName;
        }
        else if(CaseD.nLoadType==D_LCOM_STAGE)
        {
            T_SGLD_D SgldD; SgldD.Initialize();
            if(!m_pDoc->m_pPostCtrl->GetStageInfo()->GetSgldPost(CaseD.LoadCaseK, SgldD)) continue;
            strLcType = SgldD.LoadCaseName;
        }
        else if(CaseD.nLoadType==D_LCOM_GRIDMODEL)
        {
            T_GILC_D GilcD; GilcD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetGilc(CaseD.LoadCaseK, GilcD)) continue;
            strLcType = GilcD.LoadCaseName;
        }
        iCount++;

        if(iCount==1) Write_Command(_T("N_MDNL"));

        if(!Write_String(strLcType))
        {
            Write_Command(_T("N_MDNL"));
            Write_String(strLcType);
        }
    }

    T_NMRD_NODE_BASE NodeD;
    for(int i=0; i<NmrdD.aNodeInfo.GetSize(); i++)
    {
        NodeD = NmrdD.aNodeInfo.GetAt(i);
        if(i==0) Write_Command(_T("N_MDND"));
        if(Write_Enable(24))
        {
            Write_Int(NodeD.NodeK);
            Write_Double(NodeD.dWidth);
        }
        else
        {
            Write_Command(_T("N_MDND"));
            Write_Int(NodeD.NodeK);
            Write_Double(NodeD.dWidth);
        }
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_Dgn_LComData()
{
    CArray<T_LCOM_K, T_LCOM_K> LcomKList;

    CCRCForceCtrl ForceCtrl;
    ForceCtrl.Set_DataCtrl(&m_DataCtrl);

    LcomKList.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, LcomKList);
    int iTotalLcom = m_pDoc->m_pAttrCtrl->GetCountLcom(D_LCOMTYPE_CONCRETE);

    T_LCOM_D LcomD;
    T_LCOM_D_UL	LcomUlD;
    for(int i=0; i<iTotalLcom; i++)
    {
        T_LCOM_K LcomK = LcomKList.GetAt(i);
        LcomD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, LcomK, LcomD)) continue;

        if(LcomD.LoadCombType != 0) continue; //Add Type인 경우만 진행
        BOOL bMaxMinLcase = m_pDoc->m_pAttrCtrl->IsMaxMinComb(LcomD);

        LcomUlD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->NormalizeLcom(LcomD,LcomUlD)) continue;

        double dIncFac=ForceCtrl.Get_ExtraRatio(LcomUlD);

        // Change by ZINU.('06.02.16). Add dCL(충돌하중), dW(풍하중).
        double dD=0.0, dPS=0.0, dCRSH=0.0, dSD=0.0, dL=0.0, dT=0.0, dSW=0.0, dEQ=0.0, dCL=0.0, dW=0.0; 

        int iCount = 0;
        for(int j=0; j<LcomUlD.Combination.GetSize(); j++)
        {
            T_LCOM_BASE LcomBase = LcomUlD.Combination.GetAt(j);

            if(LcomBase.AnalType==D_LCOM_STATIC)
            {
                T_STLD_D StldD; StldD.Initialize();
                if(!m_pDoc->m_pPostCtrl->GetStld(LcomBase.LoadCaseKey, StldD)) continue;
                CString strLcType = StldD.LoadCaseType;
                if(CDBLib::IsStldDL(strLcType))
                {
                    if(LcomBase.Factor > dD) dD = LcomBase.Factor;
                    iCount++;
                }
                else if(CDBLib::IsStldLL(strLcType))
                {
                    if(LcomBase.Factor > dL) dL = LcomBase.Factor;
                    iCount++;
                }
                else if(strLcType == _T("CR") || strLcType == _T("SH"))
                {
                    if(LcomBase.Factor > dCRSH) dCRSH = LcomBase.Factor;
                    iCount++;
                }
                else if(strLcType == _T("PS"))
                {
                    if(LcomBase.Factor > dPS) dPS = LcomBase.Factor;
                    iCount++;
                }
                else if(strLcType == _T("S"))
                {
                    if(LcomBase.Factor > dSW) dSW = LcomBase.Factor;
                    iCount++;
                }
                else if(CDBLib::IsStldTL(strLcType))
                {
                    if(LcomBase.Factor > dT) dT = LcomBase.Factor;
                    iCount++;
                }
                else if(CDBLib::IsStldEL(strLcType))
                {
                    if(LcomBase.Factor > dEQ) dEQ = LcomBase.Factor;
                    iCount++;
                }
                else if(strLcType == _T("STL"))
                {
                    if(LcomBase.Factor > dSD) dSD = LcomBase.Factor;
                    iCount++;
                }
                else if(strLcType == _T("CO") || strLcType == _T("CT") || strLcType == _T("CV"))
                {
                    if(LcomBase.Factor > dCL) dCL = LcomBase.Factor;
                    iCount++;
                }
                else if(CDBLib::IsStldWL(strLcType))
                {
                    if(LcomBase.Factor > dW) dW = LcomBase.Factor;
                    iCount++;
                }
            }
            else if(LcomBase.AnalType==D_LCOM_SPECTRUM)
            {
                if(LcomBase.Factor > dEQ) dEQ = LcomBase.Factor;
                iCount++;
            }
            else if(LcomBase.AnalType==D_LCOM_ECCENSPEC)
            {
                if(LcomBase.Factor > dEQ) dEQ = LcomBase.Factor;
                iCount++;
            }
            else if(LcomBase.AnalType==D_LCOM_HISTORY)
            {
                if(LcomBase.Factor > dEQ) dEQ = LcomBase.Factor;
                iCount++;
            }
            else if(LcomBase.AnalType==D_LCOM_MOVING)
            {
                if(LcomBase.Factor > dL) dL = LcomBase.Factor;
                iCount++;
            }
            else if(LcomBase.AnalType==D_LCOM_SETTLEMENT)
            {
                if(LcomBase.Factor > dSD) dSD = LcomBase.Factor;
                iCount++;
            }
            else if(LcomBase.AnalType==D_LCOM_STAGE)
            {
                T_STCT_D StctD; StctD.Initialize();
                m_pDoc->m_pAttrCtrl->GetStct(StctD);
                T_SGLD_D SgldD; SgldD.Initialize();
                if(!m_pDoc->m_pPostCtrl->GetStageInfo()->GetSgldPost(LcomBase.LoadCaseKey, SgldD)) continue;
                CString strLcType = SgldD.LoadCaseName;

                if(strLcType == _LS(IDS_DB_SGLD_DL))
                {
                    if(LcomBase.Factor > dD) dD = LcomBase.Factor;
                    iCount++;
                }
                else if(strLcType == _LS(IDS_DB_SGLD_LL))
                {
                    // Change by ZINU.('05.07.02). Consider Type for EL at Stage.
                    if(LcomBase.Factor > dL) dL = LcomBase.Factor;
                    iCount++;
                }
                else if(strLcType == _LS(IDS_DB_SGLD_CP) || strLcType == _LS(IDS_DB_SGLD_CS) || 
                    strLcType == _LS(IDS_DB_SGLD_SP) || strLcType == _LS(IDS_DB_SGLD_SS))
                {
                    if(LcomBase.Factor > dCRSH) dCRSH = LcomBase.Factor;
                    iCount++;
                }
                else if(strLcType == _LS(IDS_DB_SGLD_TP) || strLcType == _LS(IDS_DB_SGLD_TS))
                {
                    if(LcomBase.Factor > dPS) dPS = LcomBase.Factor;
                    iCount++;
                }
                else
                {
                    CString strDgnLCType = _T("");
                    if (ForceCtrl.GetErectionLoadType(strLcType, StctD, strDgnLCType))
                    {
                        if(CDBLib::IsStldDL(strDgnLCType))
                        {
                            if(LcomBase.Factor > dD) dD = LcomBase.Factor;
                        }
                        else if(CDBLib::IsStldLL(strDgnLCType))
                        {
                            if(LcomBase.Factor > dL) dL = LcomBase.Factor;
                        }
                        else if(CDBLib::IsStldTL(strDgnLCType))
                        {
                            if(LcomBase.Factor > dT) dT = LcomBase.Factor;
                        }
                        iCount++;
                    }
                }
            }
            else if(LcomBase.AnalType==D_LCOM_GRIDMODEL)
            {
                T_GILD_D GildD; GildD.Initialize();
                if(!m_pDoc->m_pAttrCtrl->GetGild(LcomBase.LoadCaseKey, GildD)) continue;
                if(GildD.nType == 0 || GildD.nType == 1)
                {
                    if(LcomBase.Factor > dD) dD = LcomBase.Factor;
                    iCount++;
                }
                else
                {
                    if(LcomBase.Factor > dL) dL = LcomBase.Factor;
                    iCount++;
                }
            }
        }

        if(iCount > 0) 
        {
            Write_Command(_T("L_LCOM"));
            Write_String(LcomD.LoadCombName);
            Write_Int(LcomD.nActive);// 0:inactive, 1:active or strength/stress, 2:serviceability
            Write_Double(dIncFac); //할증계수
            Write_Double(dD);
            Write_Double(dPS);
            Write_Double(dCRSH);
            Write_Double(dSD);
            Write_Double(dL);
            Write_Double(dT);
            Write_Double(dSW);
            Write_Double(dEQ);
            Write_Double(dCL);
            Write_Double(dW);
        }

    }

    return TRUE;
}

BOOL CCRCExportFile::Write_Dgn_RBarData()
{
    int iCount = 0;
    T_RPSC_D RpscD;
    for(int i=0; i<m_PSCBeamPostD.PElemKList.GetSize(); i++)
    {
        T_ELEM_K ElemK = m_PSCBeamPostD.PElemKList.GetAt(i);
        T_SECT_K SectK;
        if(!m_PSCBeamPostD.PSectKMap.Lookup(ElemK,SectK)) continue;
        T_SECT_K OrgSectK = m_pDoc->m_pPostCtrl->GetOrgSectKeyPost(SectK);
        RpscD.Initialize();
        // Change by Jaeoh. ('06.03.17) TRUE -> FALSE
        if(!m_pDoc->m_pAttrCtrl->GetRpscByConsiderRbar(FALSE,OrgSectK,RpscD)) continue;

        if(RpscD.bMbarJ)
        {
            for(int j=0; j<2; j++)
            {
                for(int k=0; k<RpscD.Mbar[j].GetSize(); k++)
                {
                    T_RPSC_MBAR MBar = RpscD.Mbar[j].GetAt(k);

                    if(k==0)
                    {
                        Write_Command(_T("E_BAR"));
                        Write_Int(ElemK);
                        Write_Int(j+1);
                    }
                    else
                    {
                        Write_Command(_T("E_BAR"), TRUE);
                        Write_NullInt();
                        Write_NullInt();
                    }

                    Write_Int(k+1);
                    int iDia = _ttoi(MBar.strBarNa.Mid(1));
                    Write_Int(iDia);
                    Write_Int(MBar.iStaYRef);// Y의 배치 기준(0=도심, 1=단면의 맨 왼쪽)
                    Write_Double(MBar.dStaY);//(0=도심에서 양단배치 , 1=단면의 맨 왼쪽에서 오른쪽으로 배치)
                    Write_Int(MBar.iStaZRef);// Z의 배치 기준(0=Top, 1=Bottom : 단면 MBR의 Top, Bottom)
                    Write_Double(MBar.dStaZ);// 배치 위치(Z) // 양수, 상대 거리(iStaZRef)
                    Write_Double(MBar.iBarNum);
                    Write_Double(MBar.dPitch);
                }
            }
        }
        else //I,J양쪽을 같게 넣은 경우
        {
            for(int j=0; j<RpscD.Mbar[0].GetSize(); j++)
            {
                T_RPSC_MBAR MBar = RpscD.Mbar[0].GetAt(j);

                if(j==0)
                {
                    Write_Command(_T("E_BAR"));
                    Write_Int(ElemK);
                    Write_Int(3);
                }
                else
                {
                    Write_Command(_T("E_BAR"), TRUE);
                    Write_NullInt();
                    Write_NullInt();
                }

                Write_Int(j+1);
                int iDia = _ttoi(MBar.strBarNa.Mid(1));
                Write_Int(iDia);
                Write_Int(MBar.iStaYRef);// Y의 배치 기준(0=도심, 1=단면의 맨 왼쪽)
                Write_Double(MBar.dStaY);//(0=도심에서 양단배치 , 1=단면의 맨 왼쪽에서 오른쪽으로 배치)
                Write_Int(MBar.iStaZRef);// Z의 배치 기준(0=Top, 1=Bottom : 단면 MBR의 Top, Bottom)
                Write_Double(MBar.dStaZ);// 배치 위치(Z) // 양수, 상대 거리(iStaZRef)
                Write_Double(MBar.iBarNum);
                Write_Double(MBar.dPitch);
            }
        }
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_Dgn_SBarData()
{
    // Add by ZINU.('06.02.16). Check Bridge Direction.
    if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

    int iCount = 0;
    T_RPSC_D RpscD;
    for(int i=0; i<m_PSCBeamPostD.PElemKList.GetSize(); i++)
    {
        T_ELEM_K ElemK = m_PSCBeamPostD.PElemKList.GetAt(i);
        T_SECT_K SectK;
        if(!m_PSCBeamPostD.PSectKMap.Lookup(ElemK,SectK)) continue;

        T_SECT_K OrgSectK = m_pDoc->m_pPostCtrl->GetOrgSectKeyPost(SectK);
        RpscD.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetRpscByConsiderRbar(FALSE,OrgSectK,RpscD)) continue;

        if(RpscD.bSbarJ)
        {
            for(int j=0; j<2; j++)
            {
                T_RPSC_SBAR SBar = RpscD.Sbar[j];

                Write_Command(_T("E_SBAR"));
                Write_Int(ElemK);
                Write_Int(j+1);

                if(SBar.bSbarExist)
                {
                    Write_Int(1); 
                    Write_Double(SBar.dSbarPitch); 
                    Write_Double(SBar.dSbarTheta*DEGTORAD); 
                    Write_Double(SBar.dSbarAw);
                }
                else
                {
                    Write_Int(0); Write_Double(0.0); Write_Double(0.0); Write_Double(0.0); 
                }

                if(SBar.bTndnExist)
                {
                    Write_Int(1);
                    Write_Double(SBar.dTndnDist);
                    Write_Double(SBar.dTndnTheta*DEGTORAD);
                    Write_Double(SBar.dTndnAp);
                    Write_Double(SBar.dTndnPs);
                }
                else
                {
                    Write_Int(0); Write_Double(0.0); Write_Double(0.0); Write_Double(0.0); Write_Double(0.0);
                }

                if(SBar.bLbarExist)
                {
                    Write_Int(1);
                    Write_Double(SBar.dLbarPitch);
                    Write_Double(SBar.dLbarAwt);
                    Write_Double(SBar.dLbarAlt);
                }
                else
                {
                    Write_Int(0); Write_Double(0.0); Write_Double(0.0); Write_Double(0.0);
                }
                // Add by ZINU.('06.02.13). For Acor.
                Write_Double(SBar.dLbarThick);
            }
        }
        else //I,J양쪽을 같게 넣은 경우
        {
            T_RPSC_SBAR SBar = RpscD.Sbar[0];

            Write_Command(_T("E_SBAR"));
            Write_Int(ElemK);
            Write_Int(3);

            if(SBar.bSbarExist)
            {
                Write_Int(1); 
                Write_Double(SBar.dSbarPitch); 
                Write_Double(SBar.dSbarTheta*DEGTORAD); 
                Write_Double(SBar.dSbarAw);
            }
            else
            {
                Write_Int(0); Write_Double(0.0); Write_Double(0.0); Write_Double(0.0); 
            }

            if(SBar.bTndnExist)
            {
                Write_Int(1);
                Write_Double(SBar.dTndnDist);
                Write_Double(SBar.dTndnTheta*DEGTORAD);
                Write_Double(SBar.dTndnAp);
                Write_Double(SBar.dTndnPs);
            }
            else
            {
                Write_Int(0); Write_Double(0.0); Write_Double(0.0); Write_Double(0.0); Write_Double(0.0);
            }

            if(SBar.bLbarExist)
            {
                Write_Int(1);
                Write_Double(SBar.dLbarPitch);
                Write_Double(SBar.dLbarAwt);
                Write_Double(SBar.dLbarAlt);
            }
            else
            {
                Write_Int(0); Write_Double(0.0); Write_Double(0.0); Write_Double(0.0);
            }
            // Add by ZINU.('06.02.13). For Acor.
            Write_Double(SBar.dLbarThick);
        }
    }

    return TRUE;
}

BOOL CCRCExportFile::Write_Dgn_SheetData()
{
    T_PSCD_D PscdD; PscdD.Initialize();
    m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);

    Write_Command(_T("R_OPT0"));
    Write_Int((PscdD.bEtcPrint[0] ? 1 : 0));
    Write_Int((PscdD.bEtcPrint[1] ? 1 : 0));	// Add by ZINU.('08.02.28). CSV Print Item (Agree with Couno).

    Write_Command(_T("R_OPT1"));
    Write_Int((PscdD.bDgnPrint[0] ? 1 : 0));
    Write_Int((PscdD.bDgnPrint[1] ? 1 : 0));
    Write_Int((PscdD.bDgnPrint[2] ? 1 : 0));
    Write_Int((PscdD.bDgnPrint[3] ? 1 : 0));
    Write_Int((PscdD.bDgnPrint[4] ? 1 : 0));

    Write_Command(_T("R_OPT2"));
    Write_Int((PscdD.bStructPrint[0] ? 1 : 0));
    Write_Int((PscdD.bStructPrint[1] ? 1 : 0));
    Write_Int((PscdD.bStructPrint[2] ? 1 : 0));
    Write_Int((PscdD.bStructPrint[3] ? 1 : 0));
    Write_Int((PscdD.bStructPrint[4] ? 1 : 0));

    Write_Command(_T("R_OPT3"));
    Write_Int((PscdD.bMomPrint[0] ? 1 : 0));
    Write_Int((PscdD.bMomPrint[1] ? 1 : 0));
    Write_Int((PscdD.bMomPrint[2] ? 1 : 0));
    Write_Int((PscdD.bMomPrint[3] ? 1 : 0));
    Write_Int((PscdD.bMomPrint[4] ? 1 : 0));
    Write_Int((PscdD.bMomPrint[5] ? 1 : 0));
    Write_Int((PscdD.bMomPrint[6] ? 1 : 0));
    Write_Int((PscdD.bMomPrint[7] ? 1 : 0));

    // Add by ZINU.('06.02.16). Check Bridge Direction.
    if(m_iBrgDir==0)	// Longitudinal Direction.
    {
        Write_Command(_T("R_OPT4"));
        Write_Int((PscdD.bShrPrint[0] ? 1 : 0));
        Write_Int((PscdD.bShrPrint[1] ? 1 : 0));
        Write_Int((PscdD.bShrPrint[2] ? 1 : 0));
        Write_Int((PscdD.bShrPrint[3] ? 1 : 0));
        Write_Int((PscdD.bShrPrint[4] ? 1 : 0));

        Write_Command(_T("R_OPT5"));
        Write_Int((PscdD.bTorPrint[0] ? 1 : 0));
        Write_Int((PscdD.bTorPrint[1] ? 1 : 0));
        Write_Int((PscdD.bTorPrint[2] ? 1 : 0));
        Write_Int((PscdD.bTorPrint[3] ? 1 : 0));

        //2005.03.03 추가 By RSH
        Write_Command(_T("R_OPT6"));
        Write_Int((PscdD.bBckPrint[0] ? 1 : 0));
        Write_Int((PscdD.bBckPrint[1] ? 1 : 0));
        Write_Int((PscdD.bBckPrint[2] ? 1 : 0));
        Write_Int((PscdD.bBckPrint[3] ? 1 : 0));
    }

    Write_NullLine();
    return TRUE;
}



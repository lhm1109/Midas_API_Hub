// CRCImportFile.cpp: implementation of the CCRCImportFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "..\DgnEngine\Src\DGNe_RUS\DgnCalc_Rus_PSC_Struct.h"
#include "CRCImportFile.h"

#include "..\wg_db\DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_FileCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_StagCtrl.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\PCDesign.h"
#include "..\wg_db\UnitCtrl.h"

#include "..\wg_dbLock\LockOption.h"
#include "..\wg_db\DB_ST_DN_RESULT_PSC.h"
#include "DgnResultFileCtrl.h"
#include "Dgn_PSCManager.h"
#include "DgnPscDataCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//#define _PROFILING
//#include "profilewrite.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCRCImportFile::CCRCImportFile()
{
	m_ElemKList.RemoveAll();

	m_iTotLineNum=0;
	m_FysgKList.RemoveAll();
	m_FypcKList.RemoveAll();
	m_FycmKList.RemoveAll();
	m_BdcwKList.RemoveAll();
	m_TbarKList.RemoveAll();
	m_BdcrKList.RemoveAll();
	m_FssgKList.RemoveAll();
	m_FsmsKList.RemoveAll();
	m_CwmsKList.RemoveAll();
	m_CumsKList.RemoveAll();
	m_CrmsKList.RemoveAll();
	m_FsmtKList.RemoveAll();
	m_CwmtKList.RemoveAll();
	m_CumtKList.RemoveAll();
	m_CrmtKList.RemoveAll();
	m_FassCmpwKList.RemoveAll();
	m_FmssCmpwKList.RemoveAll();
	m_FprtCmpwKList.RemoveAll();
	m_FwhlCmpwKList.RemoveAll();

	m_FypcKsceKList.RemoveAll();
	m_BdcyKsceKList.RemoveAll();
	m_BdczKsceKList.RemoveAll();
	m_CumsKsceKList.RemoveAll();
	m_CrmtKsceKList.RemoveAll();

	m_FypcJtgKList.RemoveAll();
	m_FycmJtgKList.RemoveAll();
	m_FpmsJtgKList.RemoveAll();
	m_FpmtJtgKList.RemoveAll();
	m_TbarJtgKList.RemoveAll();
	m_BdcyJtgKList.RemoveAll();
	m_CumsJtgKList.RemoveAll();
	m_CrmtJtgKList.RemoveAll();
	m_CscsJtgKList.RemoveAll();

	m_DgnTdgrMap.RemoveAll();
	m_DgnTdnaMap.RemoveAll();
	m_FypcBaseKList.RemoveAll();
	m_DgnLcomKMap.RemoveAll();
	m_ComfDMap.RemoveAll();
    m_mComfKey.RemoveAll();
	m_iBrgDir = 0;

	m_iDgnCode = m_ForceCtrl.Get_Code();

	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);

	if (m_pDoc!=NULL)
	{
		m_pPSCDesign = m_pDoc->m_pPostCtrl->GetPCDesign();
	}

	CString strSkipTendon = CProduct::GetTestEnvValue(_ULS(Skip Tendon));
	m_bSkipTendon = (strSkipTendon==_ULS(yes));

	/*
	CString strCalcDll = _T("CVL_CH_Design.dll");  
	if((m_hDllCalc = LoadLibrary(strCalcDll)) == NULL)
	{
		AfxMessageBox(_T("Can't Find CVL_CH_Design.dll"));
	}
	*/

}

CCRCImportFile::~CCRCImportFile()
{

}

BOOL CCRCImportFile::Export_File()
{
	BOOL bExistStage = m_pDoc->m_pStagCtrl->ExistStag();
	BOOL bComposite = m_pDoc->m_pPostCtrl->IsPSCCompositeSect();

#if defined (_US)
	if ( CDBLib::IsPscCodeAASHTO(m_iDgnCode) && bComposite )
    {	//PSC composite ÀÎµ¥ steel composite ¶ôÀ» ¾²°í ÀÖ´Ù..-_-;
        if ( !CDBDoc::GetDocPoint()->AllowCommand(D_OPTN_ID_AASHTO_STL_COMP) )
        {
            AfxMessageBox(_LS(IDS_MAIN_NO_LICENSE_AASHTO_STL_COMP), MB_OK);
            return FALSE;
		}
    }
#endif

	// ·¯½Ã¾Æ ±âÁØÀÌ ¾Æ´Ò°æ¿ì´Â ÇÏ³ª¾¿ È®ÀÎÇÏ°í Ç¬´Ù. 
	BOOL bNoStageCodeApp=FALSE;

    switch ( m_iDgnCode )
    {
    case SNiP_20503_84_PSC:
    case SP_35_13330_11_PSC:
    case SNiP_20503_84_PSC_MKS:
    case SP_35_13330_11_PSC_MKS:
        bNoStageCodeApp = TRUE;
        break;
    case KDS_24_14_21_2022_PSC:
    case KSCE_LSD15_PSC:
        if ( bComposite ) bNoStageCodeApp = TRUE;
        break;
    case EUROCODE2_2_05_PSC:
    case IRC_112_2011_PSC:
    case IRC_112_2020_PSC:
    case IRS_PSC:
	case TMH07_3_1989:
    case AASHTO_LRFD08:
    case AASHTO_LRFD12:
    case AASHTO_LRFD14:
    case AASHTO_LRFD16:
    case AASHTO_LRFD17:
    case AASHTO_LRFD20:
	case AASHTO_LRFD24:
    case BS5400_90_PSC:
    case BD_44_15_PSC:
    case CS_455_PSC:
	case ABNT_NBR_6118_2023_PSC:
	case AREMA_23:
        bNoStageCodeApp = TRUE;
        break;
	case CSA_S6_10:
	case CSA_S6_14:
	case CSA_S6_19:
		break;
    default:	
        ASSERT(0);
        break;
    }

	if (bExistStage==FALSE && bNoStageCodeApp==FALSE)
	{ return FALSE;}

	T_STAG_K CurStag = 0;
	int iCurStep = 0;
	if (bExistStage)
	{
		CurStag  = m_pDoc->m_pStagCtrl->GetCurStag();
		iCurStep = m_pDoc->m_pStagCtrl->GetCurStagStepIndex()+1;
		m_pDoc->m_pStagCtrl->SetFinalStagWithoutUpdateNotify();
	} 

	if(!MakeData()) 
	{
		if (bExistStage) m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);
		return FALSE;
	}

	CFileException FileException;
	CString strPathName=m_pDoc->GetPathName();
	CFileCtrl FileCtrl(strPathName);
	CString strFilePathName=FileCtrl.GetFilePathAndNameWithoutExtension()+_T(".jpo");
	Set_FileName(strFilePathName);

	CString strRptPathName=FileCtrl.GetFilePathAndNameWithoutExtension()+_T(".mrpt"); //add by maxiao
	TRY
	{
		if(!m_cfile.Open(strFilePathName, CFile::modeCreate | CFile::modeWrite, &FileException))
			AfxThrowFileException(FileException.m_cause, FileException.m_lOsError, FileException.m_strFileName);

		if(!m_rptfile.Open(strRptPathName, CFile::modeCreate | CFile::modeWrite, &FileException))//add by maxiao
			AfxThrowFileException(FileException.m_cause, FileException.m_lOsError, FileException.m_strFileName);

		if(!Write_ImportData()) 
		{
			m_cfile.Close();
			m_rptfile.Close();//add by maxiao
			CFileStatus status;
			if(CFile::GetStatus(strFilePathName, status))CFile::Remove(strFilePathName);
			if (bExistStage) m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);
			// 
			m_DataCtrl.GetDgnResultFileCtrl()->DeleteDesignResultFile(D_DESIGN_FORCE_RESULT,FALSE);    
			return FALSE;
		}

		m_cfile.Close();
		m_rptfile.Close();//add by maxiao
		 
		//m_DataCtrl.GetDgnResultFileCtrl()->DeleteDesignResultFile(D_DESIGN_FORCE_RESULT,FALSE);    

	}
	CATCH(CFileException, e)
	{
		e->ReportError();
		m_cfile.Abort();
		m_rptfile.Abort();//add by maxiao
		CFileStatus status;
		if(CFile::GetStatus(strFilePathName, status))CFile::Remove(strFilePathName);
		if (bExistStage) m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);
		// 
		m_DataCtrl.GetDgnResultFileCtrl()->DeleteDesignResultFile(D_DESIGN_FORCE_RESULT,FALSE);    
		return FALSE;
	}
	END_CATCH

	if (bExistStage) m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);

	return TRUE;
}

BOOL CCRCImportFile::Export_File(CCRCDataCtrl& DataCtrl)
{
	// Modify by sshan MNET:2595  '20061201
	// Trial Version µµ DesignÀº µÇ°Ô ÇÑ´Ù (´Ü, Excel°è»ê¼­ Ãâ·Â¸¸ ¸·´Â´Ù)
	//if (CDBDoc::GetDocPoint()->IsLimitedTrialVersion())
	//{
	//  //AfxMessageBox(_LS(IDS_TRIAL_NO_LICENSE));
	//  CDBDoc::GetDocPoint()->ShowLimitedTrialVersionErrMsg();
	//  return FALSE;
	//}

	//m_DataCtrl = DataCtrl;    
	//CCRCDataCtrl* pCtrl = pDataCtrl;    
	

	T_STAG_K CurStag = m_pDoc->m_pStagCtrl->GetCurStag();
	int iCurStep = m_pDoc->m_pStagCtrl->GetCurStagStepIndex()+1;
	m_pDoc->m_pStagCtrl->SetFinalStagWithoutUpdateNotify();

	if(!MakeData_JSCE02()) 
	{
		m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);
		return FALSE;
	}

	CFileException FileException;
	CString strPathName=m_pDoc->GetPathName();
	CFileCtrl FileCtrl(strPathName);
	CString strFilePathName=FileCtrl.GetFilePathAndNameWithoutExtension()+_T(".jpo");
	Set_FileName(strFilePathName);

	TRY
	{
		if(!m_cfile.Open(strFilePathName, CFile::modeCreate | CFile::modeWrite, &FileException))
			AfxThrowFileException(FileException.m_cause, FileException.m_lOsError, FileException.m_strFileName);

		if(!Write_ImportData()) 
		{
			m_cfile.Close();
			CFileStatus status;
			if(CFile::GetStatus(strFilePathName, status))CFile::Remove(strFilePathName);
			m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);
			return FALSE;
		}

		m_cfile.Close();

	}
	CATCH(CFileException, e)
	{
		e->ReportError();
		m_cfile.Abort();
		CFileStatus status;
		if(CFile::GetStatus(strFilePathName, status))CFile::Remove(strFilePathName);
		m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);
		return FALSE;
	}
	END_CATCH

	m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);

	return TRUE;
}

BOOL CCRCImportFile::MakeData_JSCE02()
{
	//m_DataCtrl.Initialize();
	//if(!m_DataCtrl.Cal_LcomDataForDesignAll())	return FALSE;
	if(!m_DataCtrl.Cal_TdgrDataForDesignAll())	return FALSE;

	_DGN_TDGR_CRC TdgrCrc; 

	UINT DgnTdgrK = 0;
	POSITION Pos = m_DataCtrl.m_amTdgrK.GetStartPosition();
	while(Pos)
	{
		TdgrCrc.Initialize();
		m_DataCtrl.m_amTdgrK.GetNextAssoc(Pos, DgnTdgrK, TdgrCrc);
		m_DgnTdgrMap.SetAt(DgnTdgrK, TdgrCrc.OrgTdgrK);
		m_DgnTdnaMap.SetAt(DgnTdgrK, TdgrCrc.TdnaK);
	} 
	// Get ElemList for Design.
	m_ElemKList.RemoveAll();
	if(!m_DataCtrl.Get_CrcElemListForDgn(FALSE,m_ElemKList)) return FALSE;
	// Get LcomData for Design.
	if(!m_DataCtrl.Get_LcomDataMap(m_DgnLcomKMap))	return FALSE;
	// Get Forces for Design.
	if(!m_DataCtrl.Get_ForcDataMap(m_ComfDMap, m_mComfKey))	return FALSE;

	return TRUE;
}

BOOL CCRCImportFile::MakeData()
{
	m_DataCtrl.Initialize();
	if(!m_DataCtrl.Cal_LcomDataForDesignAll())	return FALSE;
	if(!m_DataCtrl.Cal_TdgrDataForDesignAll())	return FALSE;

	_DGN_TDGR_CRC TdgrCrc; 

	UINT DgnTdgrK = 0;
	POSITION Pos = m_DataCtrl.m_amTdgrK.GetStartPosition();
	while(Pos)
	{
		TdgrCrc.Initialize();
		m_DataCtrl.m_amTdgrK.GetNextAssoc(Pos, DgnTdgrK, TdgrCrc);
		m_DgnTdgrMap.SetAt(DgnTdgrK, TdgrCrc.OrgTdgrK);
		m_DgnTdnaMap.SetAt(DgnTdgrK, TdgrCrc.TdnaK);
	} 
	// Get ElemList for Design.
	m_ElemKList.RemoveAll();
	if(CDBLib::IsAdPscCode(m_iDgnCode))
	{
		m_DataCtrl.GetCheckElementData(m_ElemKList);
	}
	else
	{
		if(!m_DataCtrl.Get_CrcElemListForDgn(FALSE,m_ElemKList)) return FALSE;
	}
	
	// Get LcomData for Design.
	if(!m_DataCtrl.Get_LcomDataMap(m_DgnLcomKMap))	return FALSE;
	// Get Forces for Design.
	if(!m_DataCtrl.Get_ForcDataMap(m_ComfDMap, m_mComfKey))	return FALSE;

	BOOL bComposite = m_pDoc->m_pPostCtrl->IsPSCCompositeSect();
	if(bComposite)
	{
		m_DataCtrl.Get_StagePart1Force_PSCComposite(m_ElemKList);
	}

	//!/
	/*
	size_t Tsize = sizeof(double);  
	
	//==================================================================================================
	// Explicit Linkage for DLL : Execute Beam Design. Same to _T("Call Function Beam Design").
	typedef void* (TestFunc)(UINT, size_t, BOOL&);
	TestFunc *pFunc;
	pFunc = (TestFunc*)GetProcAddress(m_hDllCalc, ("TestDesign"));

	BOOL bVersionOK = FALSE;
	T_ELEM_K ElemK=1;
	double* pData = (double*)((*pFunc)(ElemK, Tsize, bVersionOK));        
	*/

	return TRUE;
}

BOOL CCRCImportFile::Write_ImportData()
{
	// Change by ZINU.('04.08.19). Seperate KSCE-USD03 from JSCE02.
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	// Add by ZINU.('06.02.16). Save iBrgDir.
	m_iBrgDir = PscdD.iBrgDir;

    switch ( m_DataCtrl.Get_Code() )
    {
    case JSCE02:
        if ( !Write_ImportData_JSCE() ) return FALSE;
        break;
    case KSCE_USD03:
    case KSCE_USD05:
    case KSCE_USD10:
    case KSCE_RAIL_USD04:
    case KSCE_RAIL_USD11:
        if ( !Write_ImportData_KSCE() ) return FALSE;
        break;
    case JTG_D62_04:
    case CJJ_11_2011:
        if ( !Write_ImportData_JTG() ) return FALSE;
        break;
    case AASHTO_LRFD08:
    case AASHTO_LRFD12:
    case AASHTO_LRFD14:
    case AASHTO_LRFD16:
    case AASHTO_LRFD17:
    case AASHTO_LRFD20:
	case AASHTO_LRFD24:
	case AREMA_23:
        if ( !Write_ImportData_AASHTO() ) return FALSE;
        break;
    case CSA_S6_10:
    case CSA_S6_14:
	case CSA_S6_19:
        if ( !Write_ImportData_CSA() ) return FALSE;
        break;
    case EUROCODE2_2_05_PSC:
        if ( !Write_ImportData_EURO() ) return FALSE;
        break;
    case IRC_112_2011_PSC:
    case IRC_112_2020_PSC:
        if ( !Write_ImportData_IRC() ) return FALSE;
        break;
    case KDS_24_14_21_2022_PSC:
    case KSCE_LSD15_PSC:
        if ( !Write_ImportData_KSCELSD() ) return FALSE;
        break;
    case SNiP_20503_84_PSC:
    case SP_35_13330_11_PSC:
    case SNiP_20503_84_PSC_MKS:
    case SP_35_13330_11_PSC_MKS:
        if ( !Write_ImportData_RUS() ) return FALSE;
        break;
    case AS_5100_5_17_PSC:
        if ( !Write_ImportData_AS() ) return FALSE;
        break;
	case TMH07_3_1989:
    case BS5400_90_PSC:
    case BD_44_15_PSC:
    case CS_455_PSC:
        if ( !Write_ImportData_BS() ) return FALSE;
        break;
    case IRS_PSC:
        if ( !Write_ImportData_IRS() ) return FALSE;
        break;
	case ABNT_NBR_6118_2023_PSC:
		if (!Write_ImportData_BRA()) return FALSE;
		break;
	default:
        ASSERT(0);
        break;
    }

	Write_NullLine();

	return TRUE;
}

BOOL CCRCImportFile::Write_ImportData_JSCE()
{
    T_PSCD_D PscdD; PscdD.Initialize();
    m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);

    if ( !Write_Fysg() ) return FALSE;
    if ( !Write_Fypc() ) return FALSE;
    if ( !Write_Fycm() ) return FALSE;
    if ( !Write_Bdcw() ) return FALSE;
    if ( !Write_Tbar() ) return FALSE;
    if ( !Write_Bdcr() ) return FALSE;
    if ( !Write_Fssg() ) return FALSE;
    if ( !Write_Fsms() ) return FALSE;
    if ( !Write_Cwms() ) return FALSE;
    if ( !Write_Cums() ) return FALSE;
    if ( !Write_Crms() ) return FALSE;
    if ( !Write_Fsmt() ) return FALSE;
    if ( !Write_Cwmt() ) return FALSE;
    if ( !Write_Cumt() ) return FALSE;
    if ( !Write_Crmt() ) return FALSE;
    if ( PscdD.bCmpWeb ) // ÆÄÇüÀ¥.
    {
        if ( !Write_FassCmpw() ) return FALSE;
        if ( !Write_FmssCmpw() ) return FALSE;
        if ( !Write_FprtCmpw() ) return FALSE;
        if ( !Write_FwhlCmpw() ) return FALSE;
    }
    if ( !Write_Fylc() ) return FALSE;
    if ( !Write_Fyst() ) return FALSE;
    if ( !Write_Fsst() ) return FALSE;
    if ( !Write_Fssm() ) return FALSE;
    if ( !Write_Ftms() ) return FALSE;
    if ( !Write_Ftmt() ) return FALSE;
    if ( !Write_Lcba() ) return FALSE;
    if ( !Write_Lcbu() ) return FALSE;

    return TRUE;
}

BOOL CCRCImportFile::Write_ImportData_KSCE()
{
    m_bLineLimit = FALSE;
    if ( !Write_Fysg() ) return FALSE;
    if ( !Write_FypcKsce() ) return FALSE;
    if ( !Write_Fycm() ) return FALSE;
    //if(!Write_Bdcw()) return FALSE;
    if ( !Write_Tbar() ) return FALSE;
    //if(!Write_TndnKsce()) return FALSE;
    if ( !Write_FssgKsce() ) return FALSE;
    if ( !Write_FpmsJtg() ) return FALSE;
    //if(!Write_Fsmt()) return FALSE;
    if ( !Write_BdcyKsce() ) return FALSE;
    if ( !Write_BdczKsce() ) return FALSE;
    if ( !Write_CumsKsce() ) return FALSE;
    if ( !Write_CrmtKsce() ) return FALSE;

    return TRUE;
}

BOOL CCRCImportFile::Write_ImportData_JTG()
{
    m_bLineLimit = FALSE;
    // Change, Jaeoh.(08.01.10)
    //modified by maxiao 2007-12-05, Control the check option.
    //if (PscdD.bMomPrint[0]) {//Ê©¹¤½×¶Î·¨ÏòÑ¹Ó¦Á¦

    //if (PscdD.bShrPrint[1]) {//Ê¹ÓÃ½×¶ÎÕý½ØÃæ¿¹Íä
    if ( !Write_BdcyJtg() )  return FALSE;
    //}
    //if (PscdD.bShrPrint[2]) {//Ê¹ÓÃ½×¶ÎÐ±½ØÃæ¿¹¼ô
    if ( !Write_CumsJtg() )  return FALSE;
    //}
    //if (PscdD.bShrPrint[3]) {//Ê¹ÓÃ½×¶Î¿¹Å¤
    if ( !Write_CrmtJtg() )  return FALSE;

    if ( !Write_Fysg() )     return FALSE;
    //}
    //if (PscdD.bMomPrint[1]) {//ÊÜÀ­Çø¸Ö½îÀ­Ó¦Á¦
        // Change by Jaeoh. (06.09.27) B typeµµ Fully, A type Ã³·³ °è»ê

    if ( !Write_FypcJtg() )  return FALSE;

    if ( !Write_FycmJtg() )  return FALSE;

    if ( !Write_FpmsJtg() )  return FALSE;

    if ( !Write_BdcwJtg() )  return FALSE;
    //}
    if ( !Write_TbarJtg() )  return FALSE;//ÆÕÍ¨¸Ö½î¹ÀËã
    if ( !Write_TndnJtg() )  return FALSE;//Ô¤Ó¦Á¦¸Ö½î¹ÀËã
    //}    
    if ( !Write_CscsJtg() ) return FALSE; // ¿¹Ñ¹ÑéËã 

    return TRUE;
}

BOOL CCRCImportFile::Write_ImportData_AASHTO()
{
    T_PSCD_D PscdD; PscdD.Initialize();
    m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
    // Add by ZINU.('06.02.16). Save iBrgDir.
    m_iBrgDir = PscdD.iBrgDir;
    int iBrgType = PscdD.iBrgType;

	m_DataCtrl.CalcTendonfpsLpPosition(); // Transmission Lp, Lpt À§Ä¡ÀÇ fps °è»êÀ» À§ÇØ..

    m_bLineLimit = FALSE;
    GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_MSGWIN_MESSAGE_START)); //*** Create Data for PSC Design

    if ( !Write_Fysg_AASHTO() ) return FALSE; // Construction stage(½Ã°ø´Ü°èÀÀ·Â)
    if ( !Write_Fypc_AASHTO() ) return FALSE; // Tendon Stress(ÅÙ´øÀÀ·Â)
    if ( !Write_Fycm_AASHTO() ) return FALSE; // After.. cross section stress(ÇÕ¼ºÀÀ·Â)
    if ( !Write_Fssg_AASHTO() ) return FALSE; // Principal stress by construction stage
    if ( !Write_Fpms_AASHTO() ) return FALSE; // Principal stress, shear.
    if ( !Write_Fpmt_AASHTO() ) return FALSE; // Principal stress, Torsion.

    switch ( m_DataCtrl.Get_Code() )
    {
    case AASHTO_LRFD08:
        if ( iBrgType==1 )
        {
            if ( !Write_Bdcw_AASHTO() ) return FALSE; // Crack width(ÈÚ±Õ¿­Æø)
            //if(!Write_Tbar_AASHTO()) return FALSE; // Tension Bar(ÀÎÀåÃ¶±Ù)
        }
        break;
    case AASHTO_LRFD12:
    case AASHTO_LRFD14:
    case AASHTO_LRFD16:
    case AASHTO_LRFD17:
    case AASHTO_LRFD20:
	case AASHTO_LRFD24:
	case AREMA_23:
        if ( !Write_Bdcw_AASHTO() ) return FALSE; // Crack width(ÈÚ±Õ¿­Æø)
        break;
    default:
        ASSERT(0);
        break;
    }

    if ( !Write_Bdcy_AASHTO() ) return FALSE; // Flexural strength(ÈÚ°­µµ) Y-axis
    //if(!Write_Bdcz_AASHTO()) return FALSE; // Flexural strength(ÈÚ°­µµ) Z-axis
    if ( !Write_Cums_AASHTO() ) return FALSE; // Shear strength
    if ( !Write_Crmt_AASHTO() ) return FALSE; // Torsion design.

    if ( CDBLib::IsPSCDgnMode() )
    {
        m_pPSCDesign->MakePSCDesignResultInfo();
        int nTotalTndnSize = m_pPSCDesign->GetDgnTotalTndnSize();
        int nElemSize = m_pPSCDesign->GetPscElementNumber();
        m_pPSCDesign->InitializeBnFilePSC_Design(m_iDgnCode, nElemSize, nTotalTndnSize);

        T_PSCD_DGN_TDS DSize;
        DSize.nTndnSize = nTotalTndnSize;
        //
        m_pPSCDesign->WritePSCDgnDataSize(DSize);
        if ( !Write_PSCDesign() ) return FALSE;
    }

//     // write force....
//     _PSC_AASHTO_DATA aa;
//     // ¿©±â¼­ ¸¸µç´Ù... µ¥ÀÌÅÍ¸¦ 
//     m_pPSCDesign->agd(aa);
//     m_pPSCDesign->WriteForceData();

    m_pPSCDesign->ClosePscDesignResultFile();

    return TRUE;
}


BOOL CCRCImportFile::Write_ImportData_CSA()
{
    m_bLineLimit = FALSE;
    GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_MSGWIN_MESSAGE_START)); //*** Create Data for PSC Design
    if ( !Write_Fysg_AASHTO() ) return FALSE; // Construction stage(½Ã°ø´Ü°èÀÀ·Â)
    if ( !Write_Fypc_AASHTO() ) return FALSE; // Tendon Stress(ÅÙ´øÀÀ·Â)
    if ( !Write_Fycm_AASHTO() ) return FALSE; // After.. cross section stress(ÇÕ¼ºÀÀ·Â)
    if ( !Write_Fssg_AASHTO() ) return FALSE; // Principal stress by construction stage
    if ( !Write_Fpms_AASHTO() ) return FALSE; // Principal stress, shear.
    if ( !Write_Fpmt_AASHTO() ) return FALSE; // Principal stress, Torsion.

    if ( !Write_Bdcw_CSA() ) return FALSE; // Crack width(ÈÚ±Õ¿­Æø)
    //if(!Write_Bdcw_PSC()) return FALSE; // Crack width(ÈÚ±Õ¿­Æø)

    if ( !Write_Bdcy_AASHTO() ) return FALSE; // Flexural strength(ÈÚ°­µµ) Y-axis
    if ( !Write_Cums_AASHTO() ) return FALSE; // Shear strength
    if ( !Write_Crmt_AASHTO() ) return FALSE; // Torsion design.


    m_pPSCDesign->ClosePscDesignResultFile();

    return TRUE;
}

BOOL CCRCImportFile::Write_ImportData_EURO()
{
    m_bLineLimit = FALSE;
    GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_MSGWIN_MESSAGE_START)); //*** Create Data for PSC Design
    if ( !Write_CHECK_PSC() ) return FALSE;
    if ( !Write_Fypc_AASHTO() ) return FALSE; // Tendon Stress(ÅÙ´øÀÀ·Â)

//     if(!Write_Fysg_PSC()) return FALSE; // Construction stage(½Ã°ø´Ü°èÀÀ·Â)
//     if(!Write_Fypc_AASHTO()) return FALSE; // Tendon Stress(ÅÙ´øÀÀ·Â)
//     if(!Write_Fycm_PSC()) return FALSE; // After.. cross section stress(ÇÕ¼ºÀÀ·Â)
//     if(!Write_Fpsg_PSC()) return FALSE; // Principal stress by construction stage
//     if(!Write_Fpms_PSC()) return FALSE; // Principal stress, shear.    
//     if(!Write_Bdcw_PSC()) return FALSE; // Crack width(ÈÚ±Õ¿­Æø)
//     //if(!Write_Tbar_PSC()) return FALSE; // Tension Bar(ÀÎÀåÃ¶±Ù)
//     if(!Write_Bdcr_PSC()) return FALSE; // Flexural strength(ÈÚ°­µµ) Y-axis
//     //if(!Write_Bdcz_PSC()) return FALSE; // Flexural strength(ÈÚ°­µµ) Z-axis
//     if(!Write_Cums_PSC()) return FALSE; // Shear strength
//     if(!Write_Crmt_PSC()) return FALSE; // Torsion design.

    m_pPSCDesign->ClosePscDesignResultFile();

    return TRUE;
}

BOOL CCRCImportFile::Write_ImportData_IRC()
{
    m_bLineLimit = FALSE;
    GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_MSGWIN_MESSAGE_START)); //*** Create Data for PSC Design
    if ( !Write_CHECK_PSC() ) return FALSE;
    if ( !Write_Fypc_IRS() ) return FALSE; // Tendon Stress(ÅÙ´øÀÀ·Â)

    //     if(!Write_Fysg_PSC()) return FALSE; // Construction stage(½Ã°ø´Ü°èÀÀ·Â)
    //     if(!Write_Fypc_AASHTO()) return FALSE; // Tendon Stress(ÅÙ´øÀÀ·Â)
    //     if(!Write_Fycm_PSC()) return FALSE; // After.. cross section stress(ÇÕ¼ºÀÀ·Â)
    //     if(!Write_Fpsg_PSC()) return FALSE; // Principal stress by construction stage
    //     if(!Write_Fpms_PSC()) return FALSE; // Principal stress, shear.    
    //     if(!Write_Bdcw_PSC()) return FALSE; // Crack width(ÈÚ±Õ¿­Æø)
    //     //if(!Write_Tbar_PSC()) return FALSE; // Tension Bar(ÀÎÀåÃ¶±Ù)
    //     if(!Write_Bdcr_PSC()) return FALSE; // Flexural strength(ÈÚ°­µµ) Y-axis
    //     //if(!Write_Bdcz_PSC()) return FALSE; // Flexural strength(ÈÚ°­µµ) Z-axis
    //     if(!Write_Cums_PSC()) return FALSE; // Shear strength
    //     if(!Write_Crmt_PSC()) return FALSE; // Torsion design.

    m_pPSCDesign->ClosePscDesignResultFile();

    return TRUE;
}

BOOL CCRCImportFile::Write_ImportData_KSCELSD()
{
    m_bLineLimit = FALSE;
    GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_MSGWIN_MESSAGE_START)); //*** Create Data for PSC Design
    if ( !Write_CHECK_PSC() ) return FALSE;
    if ( !Write_Fypc_AASHTO() ) return FALSE; // Tendon Stress(ÅÙ´øÀÀ·Â)

//     if(!Write_Fysg_PSC_LSD15()) return FALSE; // Construction stage(½Ã°ø´Ü°èÀÀ·Â)
//     if(!Write_Fypc_AASHTO()) return FALSE; // Tendon Stress(ÅÙ´øÀÀ·Â)
//     if(!Write_Fycm_PSC_LSD15()) return FALSE; // After.. cross section stress(ÇÕ¼ºÀÀ·Â)
//     if(!Write_Fpsg_PSC_LSD15()) return FALSE; // Principal stress by construction stage
//     if(!Write_Fpms_PSC_LSD15()) return FALSE; // Principal stress, shear.    
//     if(!Write_Bdcw_PSC_LSD15()) return FALSE; // Crack width(ÈÚ±Õ¿­Æø)
//     //if(!Write_Tbar_PSC()) return FALSE; // Tension Bar(ÀÎÀåÃ¶±Ù)
//     if(!Write_Bdcr_PSC_LSD15()) return FALSE; // Flexural strength(ÈÚ°­µµ) Y-axis
//     //if(!Write_Bdcz_PSC()) return FALSE; // Flexural strength(ÈÚ°­µµ) Z-axis
//     if(!Write_Cums_PSC_LSD15()) return FALSE; // Shear strength
//     if(!Write_Crmt_PSC_LSD15()) return FALSE; // Torsion design.

    m_pPSCDesign->ClosePscDesignResultFile();

    return TRUE;
}

BOOL CCRCImportFile::Write_ImportData_RUS()
{
    m_bLineLimit = FALSE;

    Write_FirstGroupLS();

    Write_SecondGroupLS();

    m_pPSCDesign->ClosePscDesignResultFile();

    return TRUE;
}

BOOL CCRCImportFile::Write_ImportData_AS()
{
    m_bLineLimit = FALSE;
    GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_MSGWIN_MESSAGE_START)); //*** Create Data for PSC Design
    if ( !Write_CHECK_PSC_AS() ) return FALSE;
    if ( !Write_Fypc_AASHTO() ) return FALSE; // Tendon Stress(ÅÙ´øÀÀ·Â)

    m_pPSCDesign->ClosePscDesignResultFile();

    return TRUE;
}

BOOL CCRCImportFile::Write_ImportData_BS()
{
    m_bLineLimit = FALSE;
    GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_MSGWIN_MESSAGE_START)); //*** Create Data for PSC Design
    if ( !Write_CHECK_PSC_BS() ) return FALSE;
    if ( !Write_Fypc_BS() ) return FALSE; // Tendon Stress(ÅÙ´øÀÀ·Â)

    m_pPSCDesign->ClosePscDesignResultFile();

    return TRUE;
}

BOOL CCRCImportFile::Write_ImportData_IRS()
{
    m_bLineLimit = FALSE;
    GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_MSGWIN_MESSAGE_START)); //*** Create Data for PSC Design
    if ( !Write_CHECK_PSC() ) return FALSE;
    if ( !Write_Fypc_IRS() ) return FALSE; // Tendon Stress(ÅÙ´øÀÀ·Â)

    //     if(!Write_Fysg_PSC()) return FALSE; // Construction stage(½Ã°ø´Ü°èÀÀ·Â)
    //     if(!Write_Fypc_AASHTO()) return FALSE; // Tendon Stress(ÅÙ´øÀÀ·Â)
    //     if(!Write_Fycm_PSC()) return FALSE; // After.. cross section stress(ÇÕ¼ºÀÀ·Â)
    //     if(!Write_Fpsg_PSC()) return FALSE; // Principal stress by construction stage
    //     if(!Write_Fpms_PSC()) return FALSE; // Principal stress, shear.    
    //     if(!Write_Bdcw_PSC()) return FALSE; // Crack width(ÈÚ±Õ¿­Æø)
    //     //if(!Write_Tbar_PSC()) return FALSE; // Tension Bar(ÀÎÀåÃ¶±Ù)
    //     if(!Write_Bdcr_PSC()) return FALSE; // Flexural strength(ÈÚ°­µµ) Y-axis
    //     //if(!Write_Bdcz_PSC()) return FALSE; // Flexural strength(ÈÚ°­µµ) Z-axis
    //     if(!Write_Cums_PSC()) return FALSE; // Shear strength
    //     if(!Write_Crmt_PSC()) return FALSE; // Torsion design.

    m_pPSCDesign->ClosePscDesignResultFile();

    return TRUE;
}

BOOL CCRCImportFile::Write_ImportData_BRA()
{
	m_bLineLimit = FALSE;
	GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_MSGWIN_MESSAGE_START)); //*** Create Data for PSC Design
	if (!Write_CHECK_PSC()) return FALSE;
	if (!Write_Fypc_AASHTO()) return FALSE; // Tendon Stress(ÅÙ´øÀÀ·Â)

	m_pPSCDesign->ClosePscDesignResultFile();

	return TRUE;

}

BOOL CCRCImportFile::Write_Fysg()
{
	//PROFILING(_T("CCRCImportFile::Write_Fysg()"));
	
	T_FYSG_D FysgD; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		FysgD.Initialize();
		if(!m_DataCtrl.Get_CrcFysg(ElemK, FysgD)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_FYSG_BASE FysgBase = FysgD.FysgBase[j];
			if(FysgBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iMax = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("FY_SG"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iMax);
				if(m_iDgnCode==JSCE02)
				{
					T_STAG_D StagD;
					if (!m_pDoc->m_pAttrCtrl->GetStag(FysgBase.StagK, StagD)) continue;
					Write_Int(StagD.nStageId);
				}
				else
				{
					Write_Int(FysgBase.StagK);
				}        
				Write_Double(FysgBase.dFT);
				Write_Double(FysgBase.dFB);
				Write_Double(FysgBase.dFTL);
				Write_Double(FysgBase.dFBL);
				Write_Double(FysgBase.dFTR);
				Write_Double(FysgBase.dFBR);
				Write_Double(FysgBase.dFMAX);
				if(m_iDgnCode==JSCE02)          Write_Double(0.0);
				else if(m_iDgnCode==KSCE_USD03 ||  m_iDgnCode==KSCE_USD05 || m_iDgnCode==KSCE_USD10 || 
								m_iDgnCode==KSCE_RAIL_USD04 || m_iDgnCode==KSCE_RAIL_USD11 ||
								m_iDgnCode==JTG_D62_04 ||m_iDgnCode==CJJ_11_2011 )
				{
					Write_Double(FysgBase.dALW);        
				}
				else ASSERT(0);
				Write_Double(0.0);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_FysgKList.Add(ElemK);
	}
	return TRUE;
}

BOOL CCRCImportFile::Write_Fypc()
{
	T_FYPC_D FypcD; 
	_CRC_FYPC_K CrcFypcK;

	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		FypcD.Initialize();
		if(!m_DataCtrl.Get_CrcFypc(ElemK, FypcD)) continue;

		int iCount = 0;
		for(int j=0; j<2; j++)
		{
			for(int k=0; k<FypcD.FypcBase[j].GetSize(); k++)
			{
				T_FYPC_BASE FypcBase = FypcD.FypcBase[j].GetAt(k);
				if(FypcBase.bCHK)
				{
					int iPos = (j==0 ? 1 : 2);
					Write_Command(_T("FY_PC"));
					Write_Int(ElemK.first);
					Write_Int(iPos);
					Write_Int(FypcBase.DgnTdgrK);
					Write_Double(FypcBase.dFDL);
					Write_Double(FypcBase.dFLL);
					Write_Double(0.0);
					m_iTotLineNum++;
					iCount++;

					CrcFypcK.ElemK = ElemK.first;
					CrcFypcK.DgnTdgrK = FypcBase.DgnTdgrK;
					CrcFypcK.nPos = j;
					m_FypcBaseKList.Add(CrcFypcK);
				}
			}
		}
		if(iCount > 0) m_FypcKList.Add(ElemK.first);
	}
	return TRUE;
}

BOOL CCRCImportFile::Write_Fycm()
{
	T_FYCM_D FycmD; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		FycmD.Initialize();
		if(!m_DataCtrl.Get_CrcFycm(ElemK, FycmD)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_FYCM_BASE FycmBase = FycmD.FycmBase[j];
			if(FycmBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("FY_COM"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(FycmBase.LcomK);
				Write_Int(FycmBase.iKind);
				Write_Int(FycmBase.iMax);
				Write_Double(FycmBase.dFT);
				Write_Double(FycmBase.dFB);
				Write_Double(FycmBase.dFTL);
				Write_Double(FycmBase.dFBL);
				Write_Double(FycmBase.dFTR);
				Write_Double(FycmBase.dFBR);
				Write_Double(FycmBase.dFMAX);
				if(m_iDgnCode==JSCE02)          Write_Double(0.0);
				else if(m_iDgnCode==KSCE_USD03      ||  m_iDgnCode==KSCE_USD05     || m_iDgnCode==KSCE_USD10 ||
								m_iDgnCode==KSCE_RAIL_USD04 || m_iDgnCode==KSCE_RAIL_USD11 || 
								m_iDgnCode==CJJ_11_2011     || m_iDgnCode==JTG_D62_04)
				{
					Write_Double(FycmBase.dALW);
				}
				else ASSERT(0);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_FycmKList.Add(ElemK);
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Bdcw()
{
	T_BDCW_D BdcwD; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BdcwD.Initialize();
		if(!m_DataCtrl.Get_CrcBdcw(ElemK, BdcwD)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_BDCW_BASE BdcwBase = BdcwD.BdcwBase[j];
			if(BdcwBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("BND_CW"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(BdcwBase.LcomK);
				Write_Int(BdcwBase.iKind);
				Write_Int(BdcwBase.iMax);
				Write_Double(BdcwBase.dFT);
				Write_Double(BdcwBase.dFB);
				if(m_iDgnCode==JSCE02)
				{
					Write_Double(0.0);
					Write_Double(0.0);
					Write_Double(0.0);
				}
				else if(m_iDgnCode==KSCE_USD03 || m_iDgnCode==KSCE_USD05 ||
								m_iDgnCode==KSCE_RAIL_USD04 || m_iDgnCode==KSCE_RAIL_USD11 ||
								m_iDgnCode==JTG_D62_04||m_iDgnCode==CJJ_11_2011 )
				{
					Write_Double(BdcwBase.dFRT);
					Write_Double(BdcwBase.dWC );
					Write_Double(BdcwBase.dAWC);
				}
				else ASSERT(0);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_BdcwKList.Add(ElemK);
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Tbar()
{
	T_TBAR_D TbarD; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		TbarD.Initialize();
		if(!m_DataCtrl.Get_CrcTbar(ElemK, TbarD)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_TBAR_BASE TbarBase = TbarD.TbarBase[j];
			if(TbarBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("RBAR_T"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(TbarBase.LcomK);
				Write_Int(TbarBase.iKind);
				Write_Int(TbarBase.iMax);
				if(m_iDgnCode==JSCE02)
				{
					Write_Double(0.0);
					Write_Double(0.0);
					Write_Double(0.0);
				}
				else if(m_iDgnCode==KSCE_USD03      || m_iDgnCode==KSCE_USD05       || m_iDgnCode==KSCE_USD10 || 
								m_iDgnCode==KSCE_RAIL_USD04 || m_iDgnCode==KSCE_RAIL_USD11  ||
								m_iDgnCode==JTG_D62_04      || m_iDgnCode==CJJ_11_2011 )
				{
					Write_Double(TbarBase.dREQ);
					Write_Double(TbarBase.dMIN);
					Write_Double(TbarBase.dUSE);
				}
				else ASSERT(0);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_TbarKList.Add(ElemK);
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Bdcr()
{
	T_BDCR_D BdcrD; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BdcrD.Initialize();
		if(!m_DataCtrl.Get_CrcBdcr(ElemK, BdcrD)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_BDCR_BASE BdcrBase = BdcrD.BdcrBase[j];
			if(BdcrBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("BND_CR"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(BdcrBase.LcomK);
				Write_Int(BdcrBase.iKind);
				Write_Int(BdcrBase.iMax);
				Write_Double(BdcrBase.dAX);
				Write_Double(BdcrBase.dMUY);
				Write_Double(0.0);
				Write_Double(BdcrBase.dRATY);
				Write_Double(BdcrBase.dMUZ);
				Write_Double(0.0);
				Write_Double(BdcrBase.dRATZ);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_BdcrKList.Add(ElemK);
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Fssg()
{
	// Add by ZINU.('06.02.16). Check Bridge Direction.
	if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

	T_FSSG_D FssgD; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		FssgD.Initialize();
		if(!m_DataCtrl.Get_CrcFssg(ElemK, FssgD)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_FSSG_BASE FssgBase = FssgD.FssgBase[j];
			if(FssgBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("FST_SG"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				if(m_iDgnCode==JSCE02)
				{
					T_STAG_D StagD;
					if (!m_pDoc->m_pAttrCtrl->GetStag(FssgBase.StagK, StagD)) continue;
					Write_Int(StagD.nStageId);
				}
				else
				{
					Write_Int(FssgBase.StagK);
				}                
				Write_Double(FssgBase.dFS1);
				Write_Double(FssgBase.dFSG);
				Write_Double(FssgBase.dFS3);
				if(m_iDgnCode==JSCE02)
					Write_Double(0.0);        
				else if(m_iDgnCode==KSCE_USD03 || m_iDgnCode==KSCE_USD05 || m_iDgnCode==KSCE_USD10 || m_iDgnCode==KSCE_RAIL_USD04 || m_iDgnCode==KSCE_RAIL_USD11)
					Write_Double(FssgBase.dAFS);        

				Write_Double(FssgBase.dFST1);
				Write_Double(FssgBase.dFSTG);
				Write_Double(FssgBase.dFST3);
				if(m_iDgnCode==JSCE02)
					Write_Double(0.0);        
				else if(m_iDgnCode==KSCE_USD03 || m_iDgnCode==KSCE_USD05 || m_iDgnCode==KSCE_USD10  || m_iDgnCode==KSCE_RAIL_USD04 || m_iDgnCode==KSCE_RAIL_USD11)
					Write_Double(FssgBase.dAFST);        
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_FssgKList.Add(ElemK);
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Fsms()
{
	// Add by ZINU.('06.02.16). Check Bridge Direction.
	if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

	T_FSMS_D FsmsD; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		FsmsD.Initialize();
		if(!m_DataCtrl.Get_CrcFsms(ElemK, FsmsD)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_FSMS_BASE FsmsBase = FsmsD.FsmsBase[j];
			if(FsmsBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("FST_MS"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(FsmsBase.LcomK);
				Write_Int(FsmsBase.iKind);
				Write_Int(FsmsBase.iMax);
				Write_Double(FsmsBase.dFS1);
				Write_Double(FsmsBase.dFSG);
				Write_Double(FsmsBase.dFS3);
				if(m_iDgnCode==JSCE02)
					Write_Double(0.0);        
				else if(m_iDgnCode==KSCE_USD03 || m_iDgnCode==KSCE_USD05 || m_iDgnCode==KSCE_USD10 || m_iDgnCode==KSCE_RAIL_USD04 || m_iDgnCode==KSCE_RAIL_USD11)
					Write_Double(FsmsBase.dAFS);        
				Write_Double(FsmsBase.dFST1);
				Write_Double(FsmsBase.dFSTG);
				Write_Double(FsmsBase.dFST3);
				if(m_iDgnCode==JSCE02)
					Write_Double(0.0);        
				else if(m_iDgnCode==KSCE_USD03 || m_iDgnCode==KSCE_USD05 || m_iDgnCode==KSCE_USD10 || m_iDgnCode==KSCE_RAIL_USD04 || m_iDgnCode==KSCE_RAIL_USD11)
					Write_Double(FsmsBase.dAFST);        
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_FsmsKList.Add(ElemK);
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Cwms()
{
	// Add by ZINU.('06.02.16). Check Bridge Direction.
	if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

	T_CWMS_D CwmsD; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		CwmsD.Initialize();
		if(!m_DataCtrl.Get_CrcCwms(ElemK, CwmsD)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_CWMS_BASE CwmsBase = CwmsD.CwmsBase[j];
			if(CwmsBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("CW_MS"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(CwmsBase.LcomK);
				Write_Int(CwmsBase.iKind);
				Write_Int(CwmsBase.iMax);
				Write_Double(CwmsBase.dFV);
				Write_Double(CwmsBase.dFVT);
				Write_Double(CwmsBase.dFS);
				Write_Double(CwmsBase.dFST);
				Write_Double(0.0);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_CwmsKList.Add(ElemK);
	}
	return TRUE;
}

BOOL CCRCImportFile::Write_Cums()
{
	// Add by ZINU.('06.02.16). Check Bridge Direction.
	if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

	T_CUMS_D CumsD; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		CumsD.Initialize();
		if(!m_DataCtrl.Get_CrcCums(ElemK, CumsD)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_CUMS_BASE CumsBase = CumsD.CumsBase[j];
			if(CumsBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("CRU_MS"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(CumsBase.LcomK);
				Write_Int(CumsBase.iKind);
				Write_Int(CumsBase.iMax);
				Write_Double(CumsBase.dQU);
				Write_Double(0.0);
				Write_Double(CumsBase.dTU);
				Write_Double(0.0);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_CumsKList.Add(ElemK);
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Crms()
{
	// Add by ZINU.('06.02.16). Check Bridge Direction.
	if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

	T_CRMS_D CrmsD; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		CrmsD.Initialize();
		if(!m_DataCtrl.Get_CrcCrms(ElemK, CrmsD)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_CRMS_BASE CrmsBase = CrmsD.CrmsBase[j];
			if(CrmsBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("CR_MS"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(CrmsBase.LcomK);
				Write_Int(CrmsBase.iKind);
				Write_Int(CrmsBase.iMax);
				Write_Double(CrmsBase.dQU);
				Write_Double(0.0);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_CrmsKList.Add(ElemK);
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Fsmt()
{
	// Add by ZINU.('06.02.16). Check Bridge Direction.
	if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

	T_FSMT_D FsmtD; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		FsmtD.Initialize();
		if(!m_DataCtrl.Get_CrcFsmt(ElemK, FsmtD)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_FSMT_BASE FsmtBase = FsmtD.FsmtBase[j];
			if(FsmtBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("FST_MT"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(FsmtBase.LcomK);
				Write_Int(FsmtBase.iKind);
				Write_Int(FsmtBase.iMax);
				Write_Double(FsmtBase.dFS1);
				Write_Double(FsmtBase.dFSG);
				Write_Double(FsmtBase.dFS3);
				if(m_iDgnCode==JSCE02)
					Write_Double(0.0);        
				else if(m_iDgnCode==KSCE_USD03 || m_iDgnCode==KSCE_USD05 || m_iDgnCode==KSCE_USD10 || m_iDgnCode==KSCE_RAIL_USD04 || m_iDgnCode==KSCE_RAIL_USD11)
					Write_Double(FsmtBase.dAFS);        
				Write_Double(FsmtBase.dFST1);
				Write_Double(FsmtBase.dFSTG);
				Write_Double(FsmtBase.dFST3);
				if(m_iDgnCode==JSCE02)
					Write_Double(0.0);        
				else if(m_iDgnCode==KSCE_USD03 || m_iDgnCode==KSCE_USD05 || m_iDgnCode==KSCE_USD10 || m_iDgnCode==KSCE_RAIL_USD04 || m_iDgnCode==KSCE_RAIL_USD11)
					Write_Double(FsmtBase.dAFST);        
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_FsmtKList.Add(ElemK);
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Cwmt()
{
	// Add by ZINU.('06.02.16). Check Bridge Direction.
	if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

	T_CWMT_D CwmtD; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		CwmtD.Initialize();
		if(!m_DataCtrl.Get_CrcCwmt(ElemK, CwmtD)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_CWMT_BASE CwmtBase = CwmtD.CwmtBase[j];
			if(CwmtBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("CW_MT"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(CwmtBase.LcomK);
				Write_Int(CwmtBase.iKind);
				Write_Int(CwmtBase.iMax);
				Write_Double(CwmtBase.dFV);
				Write_Double(CwmtBase.dFVT);
				Write_Double(CwmtBase.dFS);
				Write_Double(CwmtBase.dFST);
				Write_Double(0.0);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_CwmtKList.Add(ElemK);
	}
	return TRUE;
}

BOOL CCRCImportFile::Write_Cumt()
{
	// Add by ZINU.('06.02.16). Check Bridge Direction.
	if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

	T_CUMT_D CumtD; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		CumtD.Initialize();
		if(!m_DataCtrl.Get_CrcCumt(ElemK, CumtD)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_CUMT_BASE CumtBase = CumtD.CumtBase[j];
			if(CumtBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("CRU_MT"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(CumtBase.LcomK);
				Write_Int(CumtBase.iKind);
				Write_Int(CumtBase.iMax);
				Write_Double(CumtBase.dQU);
				Write_Double(0.0);
				Write_Double(CumtBase.dTU);
				Write_Double(0.0);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_CumtKList.Add(ElemK);
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Crmt()
{
	// Add by ZINU.('06.02.16). Check Bridge Direction.
	if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

	T_CRMT_D CrmtD; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		CrmtD.Initialize();
		if(!m_DataCtrl.Get_CrcCrmt(ElemK, CrmtD)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_CRMT_BASE CrmtBase = CrmtD.CrmtBase[j];
			if(CrmtBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("CR_MT"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(CrmtBase.LcomK);
				Write_Int(CrmtBase.iKind);
				Write_Int(CrmtBase.iMax);
				Write_Double(CrmtBase.dTU);
				Write_Double(0.0);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_CrmtKList.Add(ElemK);
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Fylc()
{
	T_FYLC_D FylcD; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		FylcD.Initialize();
		if(!m_DataCtrl.Get_CrcFylc(ElemK, FylcD)) continue;
		for(int j=0; j<2; j++)
		{
			for(int k=0; k<4; k++)
			{
				for(int l=0; l<2; l++)
				{
					for(int m=0; m<2; m++)
					{
						int iPosTyp = 0;
						if(j==0 && m==0) iPosTyp = 0;
						else if(j==0 && m==1) iPosTyp = 1;
						else if(j==1 && m==0) iPosTyp = 2;
						else if(j==1 && m==1) iPosTyp = 3;
						T_FYLC_BASE FylcBase = FylcD.FylcBase[k][iPosTyp][l];
						if(FylcBase.bCHK)
						{
							Write_Command(_T("FY_LCB"));
							Write_Int(ElemK.first);
							Write_Int(j+1);
							Write_Int(k+1);
							Write_Int(l+1);
							Write_Int(m+1);
							Write_Int(FylcBase.LcomK);
							Write_Int(FylcBase.iMax);
							Write_Double(FylcBase.dFT);
							Write_Double(FylcBase.dFB);
							Write_Double(FylcBase.dFTL);
							Write_Double(FylcBase.dFBL);
							Write_Double(FylcBase.dFTR);
							Write_Double(FylcBase.dFBR);
							Write_Double(FylcBase.dFTP);
							Write_Double(FylcBase.dFBP);
							Write_Double(FylcBase.dFTLP);
							Write_Double(FylcBase.dFBLP);
							Write_Double(FylcBase.dFTRP);
							Write_Double(FylcBase.dFBRP);
						}
					}
				}
			}
		}
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_FassCmpw()
{
	// Add by ZINU.('06.02.16). Check Bridge Direction.
	if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

	T_FASS_CMPW FassCmpw; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		FassCmpw.Initialize();
		if(!m_DataCtrl.Get_CrcFassCmpw(ElemK, FassCmpw)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_FASS_CMPW_BASE FassBase = FassCmpw.FassBase[j];
			if(FassBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("BCK_ASS"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(FassBase.LcomK);
				Write_Int(FassBase.iKind);
				Write_Int(FassBase.iMax);
				Write_Double(FassBase.dRTM);
				Write_Double(0.0);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_FassCmpwKList.Add(ElemK.first);
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_FmssCmpw()
{
	// Add by ZINU.('06.02.16). Check Bridge Direction.
	if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

	T_FMSS_CMPW FmssCmpw; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		FmssCmpw.Initialize();
		if(!m_DataCtrl.Get_CrcFmssCmpw(ElemK, FmssCmpw)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_FMSS_CMPW_BASE FmssBase = FmssCmpw.FmssBase[j];
			if(FmssBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("BCK_MSS"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(FmssBase.LcomK);
				Write_Int(FmssBase.iKind);
				Write_Int(FmssBase.iMax);
				Write_Double(FmssBase.dRTM);
				Write_Double(0.0);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_FmssCmpwKList.Add(ElemK.first);
	}
	return TRUE;
}

BOOL CCRCImportFile::Write_FprtCmpw()
{
	// Add by ZINU.('06.02.16). Check Bridge Direction.
	if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

	T_FPRT_CMPW FprtCmpw; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		FprtCmpw.Initialize();
		if(!m_DataCtrl.Get_CrcFprtCmpw(ElemK, FprtCmpw)) continue;
		int iCount = 0;
		for(int j=0; j<2; j++)
		{
			T_FPRT_CMPW_BASE FprtBase = FprtCmpw.FprtBase[j];
			if(FprtBase.bCHK)
			{
				int iPos = (j==0 ? 1 : 2);
				Write_Command(_T("BCK_PRT"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Double(FprtBase.dRTL);
				Write_Double(FprtBase.dRTY);
				Write_Double(FprtBase.dRMDS);
				Write_Double(0.0);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_FprtCmpwKList.Add(ElemK.first);
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_FwhlCmpw()
{
	// Add by ZINU.('06.02.16). Check Bridge Direction.
	if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

	T_FWHL_CMPW FwhlCmpw; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		FwhlCmpw.Initialize();
		if(!m_DataCtrl.Get_CrcFwhlCmpw(ElemK, FwhlCmpw)) continue;
		int iCount = 0;
		for(int j=0; j<2; j++)
		{
			T_FWHL_CMPW_BASE FwhlBase = FwhlCmpw.FwhlBase[j];
			if(FwhlBase.bCHK)
			{
				int iPos = (j==0 ? 1 : 2);
				Write_Command(_T("BCK_WHL"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Double(FwhlBase.dRTL);
				Write_Double(FwhlBase.dRTY);
				Write_Double(FwhlBase.dRMDS);
				Write_Double(0.0);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_FwhlCmpwKList.Add(ElemK.first);
	}
	
	return TRUE;
}

BOOL CCRCImportFile::Write_Fyst()
{
	T_FYST_D FystD; 
	T_FYST_BASE FystBase;

	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		FystD.Initialize();
		if(!m_DataCtrl.Get_CrcFyst(ElemK, FystD)) continue;

		T_STAG_K StagK;
		for(int j=0; j<2; j++)
		{
			int iPos = (j==0 ? 1 : 2);
			POSITION Pos  = FystD.FystBaseMap[j].GetStartPosition();
			while(Pos)
			{
				FystBase.Initialize();
				FystD.FystBaseMap[j].GetNextAssoc(Pos, StagK, FystBase);

				if(FystBase.bCHK)
				{
					Write_Command(_T("FY_SGT"));
					Write_Int(ElemK.first);
					Write_Int(iPos);
					if(m_iDgnCode==JSCE02)
					{
						T_STAG_D StagD;
						if (!m_pDoc->m_pAttrCtrl->GetStag(StagK, StagD)) continue;
						Write_Int(StagD.nStageId);
					}
					else
					{
						Write_Int(StagK);
					}                  
					Write_Double(FystBase.dFT);
					Write_Double(FystBase.dFB);
					Write_Double(FystBase.dFTL);
					Write_Double(FystBase.dFBL);
					Write_Double(FystBase.dFTR);
					Write_Double(FystBase.dFBR);
				}
			}
		}
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Fsst()
{
	// Add by ZINU.('06.02.16). Check Bridge Direction.
	if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

	T_FSST_D FsstD; 
	T_FSST_BASE FsstBase;

	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		FsstD.Initialize();
		if(!m_DataCtrl.Get_CrcFsst(ElemK, FsstD)) continue;

		T_STAG_K StagK;
		for(int j=0; j<2; j++)
		{
			int iPos = (j==0 ? 1 : 2);
			POSITION Pos  = FsstD.FsstBaseMap[j].GetStartPosition();
			while(Pos)
			{
				FsstBase.Initialize();
				FsstD.FsstBaseMap[j].GetNextAssoc(Pos, StagK, FsstBase);

				if(FsstBase.bCHK)
				{
					Write_Command(_T("FST_SGT"));
					Write_Int(ElemK.first);
					Write_Int(iPos);
					if(m_iDgnCode==JSCE02)
					{
						T_STAG_D StagD;
						if (!m_pDoc->m_pAttrCtrl->GetStag(StagK, StagD)) continue;
						Write_Int(StagD.nStageId);
					}
					else
					{
						Write_Int(StagK);
					}                  
					Write_Double(FsstBase.dFS1);
					Write_Double(FsstBase.dFSG);
					Write_Double(FsstBase.dFS3);
					Write_Double(FsstBase.dFST1);
					Write_Double(FsstBase.dFSTG);
					Write_Double(FsstBase.dFST3);
				}
			}
		}
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Fssm()
{
	// Add by ZINU.('06.02.16). Check Bridge Direction.
	if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

	T_FSSM_D FssmD; 
	T_FSSM_BASE FssmBase;

	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		FssmD.Initialize();
		if(!m_DataCtrl.Get_CrcFssm(ElemK, FssmD)) continue;

		T_STAG_K StagK;
		for(int j=0; j<2; j++)
		{
			int iPos = (j==0 ? 1 : 2);

			POSITION Pos  = FssmD.FssmBaseMap[j].GetStartPosition();
			while(Pos)
			{
				FssmBase.Initialize();
				FssmD.FssmBaseMap[j].GetNextAssoc(Pos, StagK, FssmBase);

				if(FssmBase.bCHK)
				{
					for(int k=0; k<3; k++)
					{
						if(k == 0)
						{
							Write_Command(_T("FST_SMT"));
							Write_Int(ElemK.first);
							Write_Int(iPos);
							if(m_iDgnCode==JSCE02)
							{
								T_STAG_D StagD;
								if (!m_pDoc->m_pAttrCtrl->GetStag(StagK, StagD)) continue;
								Write_Int(StagD.nStageId);
							}
							else
							{
								Write_Int(StagK);
							}                  
						}
						else
						{
							Write_Command(_T("FST_SMT"), TRUE);
							Write_NullInt();
							Write_NullInt();
							Write_NullInt();
						}
						Write_Int(k+1);
						Write_Double(FssmBase.dFX[k]);
						Write_Double(FssmBase.dFZ[k]);
						Write_Double(FssmBase.dTS[k]);
						Write_Double(FssmBase.dTT[k]);
						Write_Double(FssmBase.dTP[k]);
					}
				}
			}
		}
	}
	return TRUE;
}

BOOL CCRCImportFile::Write_Ftms()
{
	// Add by ZINU.('06.02.16). Check Bridge Direction.
	if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

	T_FTMS_D FtmsD; 
	T_FTMS_BASE FtmsBase;

	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		FtmsD.Initialize();
		if(!m_DataCtrl.Get_CrcFtms(ElemK, FtmsD)) continue;

		for(int j=0; j<2; j++)
		{
			for(int k=0; k<4; k++)
			{
				for(int l=0; l<2; l++)
				{
					int iPosTyp = 0;
					if(j==0 && l==0) iPosTyp = 0;
					else if(j==0 && l==1) iPosTyp = 1;
					else if(j==1 && l==0) iPosTyp = 2;
					else if(j==1 && l==1) iPosTyp = 3;

					FtmsBase = FtmsD.FtmsBase[k][iPosTyp];
					if(FtmsBase.bCHK)
					{
						for(int m=0; m<3; m++)
						{
							if(m == 0)
							{
								Write_Command(_T("FST_MST"));
								Write_Int(ElemK.first);
								Write_Int(j+1);
								Write_Int(k+1);
								Write_Int(l+1);
								Write_Int(FtmsBase.LcomK);
								Write_Int(FtmsBase.iMax);
								Write_Int(m+1);
								Write_Double(FtmsBase.dFX[m]);
								Write_Double(FtmsBase.dFZ[m]);
								Write_Double(FtmsBase.dTS[m]);
								Write_Double(FtmsBase.dTT[m]);
								Write_Double(FtmsBase.dTP[m]);
								Write_Double(FtmsBase.dSN[m]);
								Write_Double(FtmsBase.dST[m]);
								Write_Double(FtmsBase.dFxx);
								//Write_Double(FtmsBase.dFyy);
								Write_Double(FtmsBase.dFzz);
								Write_Double(FtmsBase.dMux);
								Write_Double(FtmsBase.dMuy);
								Write_Double(FtmsBase.dMuz);
							}
							else
							{
								Write_Command(_T("FST_MST"), TRUE);
								Write_NullInt();
								Write_NullInt();
								Write_NullInt();
								Write_NullInt();
								Write_NullInt();
								Write_NullInt();
								Write_Int(m+1);
								Write_Double(FtmsBase.dFX[m]);
								Write_Double(FtmsBase.dFZ[m]);
								Write_Double(FtmsBase.dTS[m]);
								Write_Double(FtmsBase.dTT[m]);
								Write_Double(FtmsBase.dTP[m]);
								Write_Double(FtmsBase.dSN[m]);
								Write_Double(FtmsBase.dST[m]);
								/*
								Write_Double(FtmsBase.dFxx);
								Write_Double(FtmsBase.dFyy);
								Write_Double(FtmsBase.dFzz);
								Write_Double(FtmsBase.dMux);
								Write_Double(FtmsBase.dMuy);
								Write_Double(FtmsBase.dMuz);
								*/
							}
						}
					}
				}
			}
		}
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Ftmt()
{
	// Add by ZINU.('06.02.16). Check Bridge Direction.
	if(m_iBrgDir==1)	return TRUE;	// Transverse Direction.

	T_FTMT_D FtmtD; 
	T_FTMT_BASE FtmtBase;

	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		FtmtD.Initialize();
		if(!m_DataCtrl.Get_CrcFtmt(ElemK, FtmtD)) continue;

		for(int j=0; j<2; j++)
		{
			for(int k=0; k<4; k++)
			{
				for(int l=0; l<2; l++)
				{
					int iPosTyp = 0;
					if(j==0 && l==0) iPosTyp = 0;
					else if(j==0 && l==1) iPosTyp = 1;
					else if(j==1 && l==0) iPosTyp = 2;
					else if(j==1 && l==1) iPosTyp = 3;

					FtmtBase = FtmtD.FtmtBase[k][iPosTyp];
					if(FtmtBase.bCHK)
					{
						for(int m=0; m<3; m++)
						{
							if(m == 0)
							{
								Write_Command(_T("FST_MTT"));
								Write_Int(ElemK.first);
								Write_Int(j+1);
								Write_Int(k+1);
								Write_Int(l+1);
								Write_Int(FtmtBase.LcomK);
								Write_Int(FtmtBase.iMax);
								Write_Int(m+1);
								Write_Double(FtmtBase.dFX[m]);
								Write_Double(FtmtBase.dFZ[m]);
								Write_Double(FtmtBase.dTS[m]);
								Write_Double(FtmtBase.dTT[m]);
								Write_Double(FtmtBase.dTP[m]);
								Write_Double(FtmtBase.dSN[m]);
								Write_Double(FtmtBase.dST[m]);
								Write_Double(FtmtBase.dFxx);
								//Write_Double(FtmtBase.dFyy);
								Write_Double(FtmtBase.dFzz);
								Write_Double(FtmtBase.dMux);
								Write_Double(FtmtBase.dMuy);
								Write_Double(FtmtBase.dMuz);
							}
							else
							{
								Write_Command(_T("FST_MTT"), TRUE);
								Write_NullInt();
								Write_NullInt();
								Write_NullInt();
								Write_NullInt();
								Write_NullInt();
								Write_NullInt();
								Write_Int(m+1);
								Write_Double(FtmtBase.dFX[m]);
								Write_Double(FtmtBase.dFZ[m]);
								Write_Double(FtmtBase.dTS[m]);
								Write_Double(FtmtBase.dTT[m]);
								Write_Double(FtmtBase.dTP[m]);
								Write_Double(FtmtBase.dSN[m]);
								Write_Double(FtmtBase.dST[m]);
								/*
								Write_Double(FtmtBase.dFxx);
								Write_Double(FtmtBase.dFyy);
								Write_Double(FtmtBase.dFzz);
								Write_Double(FtmtBase.dMux);
								Write_Double(FtmtBase.dMuy);
								Write_Double(FtmtBase.dMuz);
								*/
							}
						}
					}
				}
			}
		}
	}
	return TRUE;
}

BOOL CCRCImportFile::Write_Lcba()
{
	T_LCBA_D LcbaD; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		LcbaD.Initialize();
		if(!m_DataCtrl.Get_CrcLcba(ElemK, LcbaD)) continue;
		int iCount = 0;
		for(int j=0; j<2; j++)
		{
			T_LCBA_BASE LcbaBase = LcbaD.LcbaBase[j];
			if(LcbaBase.bCHK)
			{
				for(int k=0; k<3; k++)
				{
					if(k==0)
					{
						Write_Command(_T("LCB_ALW"));
						Write_Int(ElemK.first);
						Write_Int(j+1);
						for(int m=0; m<4; m++)
						{
							Write_Int(LcbaBase.MmaxLcomK[m][0]);
							Write_Int(LcbaBase.MminLcomK[m][0]);
						}
						for(int m=0; m<4; m++)
						{
							Write_Int(LcbaBase.MmaxLcomK[m][1]);
							Write_Int(LcbaBase.MminLcomK[m][1]);
						}
					}
					else
					{
						Write_Command(_T("LCB_ALW"), TRUE);
						Write_NullInt();
						Write_NullInt();
						for(int m=0; m<4; m++)
						{
							Write_Int(k==1 ? LcbaBase.QmaxLcomK[m] : LcbaBase.TmaxLcomK[m]);
							Write_Int(k==1 ? LcbaBase.QminLcomK[m] : LcbaBase.TminLcomK[m]);
						}
					}
				}
			}
		}
	}
	return TRUE;
}

BOOL CCRCImportFile::Write_Lcbu()
{
	T_LCBU_D LcbuD; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		LcbuD.Initialize();
		if(!m_DataCtrl.Get_CrcLcbu(ElemK, LcbuD)) continue;
		int iCount = 0;
		for(int j=0; j<2; j++)
		{
			T_LCBU_BASE LcbuBase = LcbuD.LcbuBase[j];
			if(LcbuBase.bCHK)
			{
				for(int k=0; k<3; k++)
				{
					if(k==0)
					{
						Write_Command(_T("LCB_ULT"));
						Write_Int(ElemK.first);
						Write_Int(j+1);
						for(int m=0; m<3; m++)
						{
							Write_Int(LcbuBase.MmaxLcomK[m]);
							Write_Int(LcbuBase.MminLcomK[m]);
						}
					}
					else
					{
						Write_Command(_T("LCB_ULT"), TRUE);
						Write_NullInt();
						Write_NullInt();
						for(int m=0; m<3; m++)
						{
							Write_Int(k==1 ? LcbuBase.QmaxLcomK[m] : LcbuBase.TmaxLcomK[m]);
							Write_Int(k==1 ? LcbuBase.QminLcomK[m] : LcbuBase.TminLcomK[m]);
						}
					}
				}
			}
		}
	}
	return TRUE;
}


BOOL CCRCImportFile::Write_FypcKsce()
{
	if (m_bSkipTendon==TRUE) return TRUE;

	T_FYPC_KSCE FypcKsce;

	CArray<T_TDNA_K,T_TDNA_K> arTdnaK;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdnaKeyList(arTdnaK);

	for(int i=0; i<arTdnaK.GetSize(); i++)
	{
		T_TDNA_K TdnaK = arTdnaK.GetAt(i);
		FypcKsce.Initialize();
		if(!m_DataCtrl.Get_KsceFypc(TdnaK, FypcKsce)) continue;
		if(!FypcKsce.bCHK)  continue;
		Write_Command(_T("FYPC_KS"));  // _KS = KSCE-USD03.
		Write_Int(TdnaK);
		Write_Double(FypcKsce.dFDL1);
		Write_Double(FypcKsce.dFDL2);
		Write_Double(FypcKsce.dFLL);
		Write_Double(FypcKsce.dAFDL1);
		Write_Double(FypcKsce.dAFDL2);
		Write_Double(FypcKsce.dAFLL);
		m_iTotLineNum++;
		m_FypcKsceKList.Add(TdnaK);
	}
	return TRUE;
}

BOOL CCRCImportFile::Write_FssgKsce()
{
	T_FPMT_JTG FpmtKsce; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		FpmtKsce.Initialize();
		if(!m_DataCtrl.Get_KsceFssg(ElemK, FpmtKsce)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_FPMT_JTG_BASE FpmtBase = FpmtKsce.FpmtBase[j];
			if(FpmtBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("FPMT_JT"));                      
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(FpmtBase.LcomK); // LcomK=StageK
				Write_Int(FpmtBase.iMax);
				Write_Double(FpmtBase.dFP1 );
				Write_Double(FpmtBase.dFP2 );	   
				Write_Double(FpmtBase.dFP3 );		   
				Write_Double(FpmtBase.dFP4 );         	
				Write_Double(FpmtBase.dFP5 );
				Write_Double(FpmtBase.dFP6 );	   
				Write_Double(FpmtBase.dFP7 );		   
				Write_Double(FpmtBase.dFP8 );         	
				Write_Double(FpmtBase.dFP9 );
				Write_Double(FpmtBase.dFP10);	   
				Write_Double(FpmtBase.dFMAX);		   
				Write_Double(FpmtBase.dAFP );         	
				m_iTotLineNum++;
				iCount++;
			 }
		}
		if(iCount > 0) m_FpmtJtgKList.Add(ElemK);
	}
	return TRUE;
}
BOOL CCRCImportFile::Write_BdcyKsce()
{
	T_BDCY_KSCE BdcyKsce; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);

		BOOL bCompoSect = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);
		if(bCompoSect) return FALSE;

		BdcyKsce.Initialize();
		if(!m_DataCtrl.Get_KsceBdcy(ElemK, BdcyKsce)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_BDCY_KSCE_BASE BdcyBase = BdcyKsce.BdcyBase[j];
			if(BdcyBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("BDCY_KS"));                      
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(BdcyBase.LcomK);
				Write_Int(BdcyBase.iMax);
				Write_Double(BdcyBase.dMuy);
				Write_Double(BdcyBase.dpMny);
				Write_Double(BdcyBase.dRatM);        
				Write_Double(BdcyBase.dqp);		   
				Write_Double(BdcyBase.d036Beta1);
				Write_Double(BdcyBase.d12Mcr);
				Write_Double(BdcyBase.d43Muy);
				///////////////////////////////////
				Write_Double(BdcyBase.dPe);		
				Write_Double(BdcyBase.da);		   
				Write_Double(BdcyBase.dIy);		   
				Write_Double(BdcyBase.db);		   
				Write_Double(BdcyBase.dh);		   
				Write_Double(BdcyBase.dyt);		
				Write_Double(BdcyBase.dfck);	
				Write_Double(BdcyBase.dAc);		
				Write_Double(BdcyBase.dfpe);	   
				Write_Double(BdcyBase.dfps);	   
				Write_Double(BdcyBase.dAps);	   
				Write_Double(BdcyBase.deps);	
				Write_Double(BdcyBase.ddps);	   
				Write_Double(BdcyBase.drp); 	
				Write_Double(BdcyBase.dfy);   
				Write_Double(BdcyBase.dAst);	   
				Write_Double(BdcyBase.dpst);	   
				Write_Double(BdcyBase.ddst);	   
				Write_Double(BdcyBase.dAsc);	
				Write_Double(BdcyBase.dpsc);	
				Write_Double(BdcyBase.ddsc);        
				Write_Double(BdcyBase.dbeta1);
				Write_Double(BdcyBase.dphi); 	
				m_iTotLineNum++;
				iCount++;
			 }
		}
		if(iCount > 0) m_BdcyKsceKList.Add(ElemK);
	}
	return TRUE;
}

BOOL CCRCImportFile::Write_BdczKsce()
{
	T_BDCZ_KSCE BdczKsce; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BdczKsce.Initialize();
		if(!m_DataCtrl.Get_KsceBdcz(ElemK, BdczKsce)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_BDCZ_KSCE_BASE BdczBase = BdczKsce.BdczBase[j];
			if(BdczBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("BDCZ_KS"));                        
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(BdczBase.LcomK);
				Write_Int(BdczBase.iMax);
				Write_Double(BdczBase.dMuy);
				Write_Double(BdczBase.dpMny);	   
				Write_Double(BdczBase.dqp);		   
				Write_Double(BdczBase.d036Beta1);
				Write_Double(BdczBase.d12Mcr);
				///////////////////////////////////
				Write_Double(BdczBase.da);		   
				Write_Double(BdczBase.dIy);		   
				Write_Double(BdczBase.dyt);		
				Write_Double(BdczBase.dfck);	
				Write_Double(BdczBase.dAc);		
				Write_Double(BdczBase.dfpe);	   
				Write_Double(BdczBase.dfps);	   
				Write_Double(BdczBase.dAps);	   
				Write_Double(BdczBase.deps);	
				Write_Double(BdczBase.ddps);	   
				Write_Double(BdczBase.drp); 	
				Write_Double(BdczBase.dfy);   
				Write_Double(BdczBase.dAst);	   
				Write_Double(BdczBase.dpst);	   
				Write_Double(BdczBase.ddst);	   
				Write_Double(BdczBase.dAsc);	
				Write_Double(BdczBase.dpsc);	
				Write_Double(BdczBase.ddsc);	
				//Write_Double(BdczBase.dRatM);	   
				//Write_Double(BdczBase.dPe);		
				//Write_Double(BdczBase.dbeta1);
				//Write_Double(BdczBase.dphi); 	
				m_iTotLineNum++;
				iCount++;
			 }
		}
		if(iCount > 0) m_BdczKsceKList.Add(ElemK);
	}
	return TRUE;
}

BOOL CCRCImportFile::Write_CumsKsce()
{
	T_CUMS_KSCE CumsKsce; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		CumsKsce.Initialize();
		if(!m_DataCtrl.Get_KsceCums(ElemK, CumsKsce)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_CUMS_KSCE_BASE CumsBase = CumsKsce.CumsBase[j];
			if(CumsBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("CUMS_KS"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(CumsBase.LcomK);
				Write_Int(CumsBase.iMax);
				Write_Double(CumsBase.dVu);	 
				Write_Double(CumsBase.dVci); 
				Write_Double(CumsBase.dVcw); 
				Write_Double(CumsBase.dVc);	 
				Write_Double(CumsBase.dVs);	 
				Write_Double(CumsBase.dpVn);
				Write_Double(CumsBase.dAv);	 
				Write_Double(CumsBase.dAvmin);
				Write_Double(CumsBase.dAvreq);
				///////////////////////////
				Write_Double(CumsBase.dbw);		
				Write_Double(CumsBase.dd);		
				Write_Double(CumsBase.dIy); 	
				Write_Double(CumsBase.dh); 	
				Write_Double(CumsBase.dyt);		
				Write_Double(CumsBase.dfd);   
				Write_Double(CumsBase.dVd);		
				Write_Double(CumsBase.dVi); 	
				Write_Double(CumsBase.dVp);   
				Write_Double(CumsBase.dMmax);	
				Write_Double(CumsBase.dMu);		
				Write_Double(CumsBase.dMd);		
				Write_Double(CumsBase.dMcr);	
				Write_Double(CumsBase.dfck);	
				Write_Double(CumsBase.dfpe);	
				Write_Double(CumsBase.dfpc);	
				Write_Double(CumsBase.dsv); 	
				Write_Double(CumsBase.dThetaV);
				Write_Double(CumsBase.dsp); 	
				Write_Double(CumsBase.dThetap);
				Write_Double(CumsBase.dAp); 	
				//Write_Double(CumsBase.dRatV);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_CumsKsceKList.Add(ElemK);
	}
	return TRUE;
}

BOOL CCRCImportFile::Write_CrmtKsce()
{
	T_CRMT_KSCE CrmtKsce; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		CrmtKsce.Initialize();
		if(!m_DataCtrl.Get_KsceCrmt(ElemK, CrmtKsce)) continue;
		int iCount = 0;
		for(int j=0; j<6; j++)
		{
			T_CRMT_KSCE_BASE CrmtBase = CrmtKsce.CrmtBase[j];
			if(CrmtBase.bCHK)
			{
				int iPos = (j==0 || j==1 || j==2 ? 1 : 2);
				// 0=I-V-MAX, 1=I-V-MIN, 2=I-T-MAX, 3=J-V-MAX, 4=J-V-MIN, 5=J-T-MAX.
				int iTyp = 0;
				if( j==0 || j==3)      iTyp = 1;
				else if( j==1 || j==4) iTyp = 2;
				else                   iTyp = 3;
				Write_Command(_T("CRMT_KS"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(CrmtBase.LcomK);
				Write_Int(CrmtBase.iMax);
				Write_Double(CrmtBase.dTu);		 
				Write_Double(CrmtBase.dTcr);   //1/4Tcr
				Write_Double(CrmtBase.dpTn);	 
				Write_Double(CrmtBase.dvVT);	 
				Write_Double(CrmtBase.dvVTmax);
				Write_Double(CrmtBase.dAtmin);
				Write_Double(CrmtBase.dAt);		 
				Write_Double(CrmtBase.dAlmin);
				Write_Double(CrmtBase.dAluse);
				Write_Double(CrmtBase.dbw);		 
				Write_Double(CrmtBase.dAoh);   
				Write_Double(CrmtBase.dPh);		 
				Write_Double(CrmtBase.dMu);
				Write_Double(CrmtBase.dVu);		 
				Write_Double(CrmtBase.dVc);	 	 
				Write_Double(CrmtBase.dfck);	 
				Write_Double(CrmtBase.dfyv);	 
				Write_Double(CrmtBase.dfyl);
				Write_Double(CrmtBase.dAcp);	 
				Write_Double(CrmtBase.dPcp);	 
				Write_Double(CrmtBase.dfpc); 	 
				Write_Double(CrmtBase.dt); 		 
				Write_Double(CrmtBase.dThetaC);
				//Write_Double(CrmtBase.dRatT);  
				//Write_Double(CrmtBase.dAlreq); 
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_CrmtKsceKList.Add(ElemK);
	}
	return TRUE;
}

BOOL CCRCImportFile::Write_TndnKsce()
{
	T_TNDN_KSCE TndnKsce; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		TndnKsce.Initialize();
		if(!m_DataCtrl.Get_KsceTndn(ElemK, TndnKsce)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_TNDN_KSCE_BASE TndnBase = TndnKsce.TndnBase[j];
			if(TndnBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("TNDN_KS"));                      
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(TndnBase.StagK);
				Write_Int(TndnBase.LcomKSer);
				Write_Int(TndnBase.iMaxSer);
				Write_Int(TndnBase.LcomK);
				Write_Int(TndnBase.iMax);
				Write_Double(TndnBase.dMd);
				Write_Double(TndnBase.dMa);	   
				Write_Double(TndnBase.de);         	
				Write_Double(TndnBase.dPimin);	   
				Write_Double(TndnBase.dApmin);		   
				Write_Double(TndnBase.dApuse);         	
				m_iTotLineNum++;
				iCount++;
			 }
		}
		if(iCount > 0) m_TndnKsceKList.Add(ElemK.first);
	}
	return TRUE;
}

// JTG D62-04
BOOL CCRCImportFile::Write_FypcJtg()
{
	//PROFILING(_T("CCRCImportFile::Write_FypcJtg()"));

	T_FYPC_JTG FypcJtg;

	CArray<T_TDNA_K,T_TDNA_K> arTdnaK;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdnaKeyList(arTdnaK);

	for(int i=0; i<arTdnaK.GetSize(); i++)
	{
		T_TDNA_K TdnaK = arTdnaK.GetAt(i);
		FypcJtg.Initialize();
		if(!m_DataCtrl.Get_JtgFypc(TdnaK, FypcJtg)) continue;
		if(!FypcJtg.bCHK)  continue;
		Write_Command(_T("FYPC_JT"));  
		Write_Int(TdnaK);
		Write_Double(FypcJtg.dFDL1);
		Write_Double(FypcJtg.dFDL2);
		Write_Double(FypcJtg.dFLL);
		Write_Double(FypcJtg.dAFDL1);
		Write_Double(FypcJtg.dAFDL2);
		Write_Double(FypcJtg.dAFLL);
		m_iTotLineNum++;
		m_FypcJtgKList.Add(TdnaK);
	}
	return TRUE;
}

BOOL CCRCImportFile::Write_FypcJtg_Btype()
{
	//PROFILING(_T("CCRCImportFile::Write_FypcJtg()"));
	int i;
	CArray<T_TDNA_K,T_TDNA_K> arTdnaK;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdnaKeyList(arTdnaK);
	
	CMap<T_TDNA_K,T_TDNA_K,T_FYPC_JTG,T_FYPC_JTG> mapFypcJtg;
	mapFypcJtg.InitHashTable(__max(1, arTdnaK.GetSize()));

	T_FYPC_JTG FypcJtg;
	for(i=0; i<arTdnaK.GetSize(); i++)
	{
		T_TDNA_K TdnaK = arTdnaK.GetAt(i);
		FypcJtg.Initialize();
		mapFypcJtg.SetAt(TdnaK,FypcJtg);    
	}

	if(!m_DataCtrl.Get_JtgFypc(mapFypcJtg)) return FALSE;

	for(i=0; i<arTdnaK.GetSize(); i++)
	{
		T_TDNA_K TdnaK = arTdnaK.GetAt(i);
		FypcJtg.Initialize();
		if(!mapFypcJtg.Lookup(TdnaK, FypcJtg)) continue;
		if(!FypcJtg.bCHK)  continue;
		Write_Command(_T("FYPC_JT"));  
		Write_Int(TdnaK);
		Write_Double(FypcJtg.dFDL1);
		Write_Double(FypcJtg.dFDL2);
		Write_Double(FypcJtg.dFLL);
		Write_Double(FypcJtg.dAFDL1);
		Write_Double(FypcJtg.dAFDL2);
		Write_Double(FypcJtg.dAFLL);
		m_iTotLineNum++;
		m_FypcJtgKList.Add(TdnaK);
	}
	return TRUE;
}

BOOL CCRCImportFile::Write_FycmJtg()
{
	//PROFILING(_T("CCRCImportFile::Write_FycmJtg()"));
	BOOL bPrintTitle = TRUE;//by maxiao
	T_FYCM_JTG FycmJtg; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		FycmJtg.Initialize();
		T_DETAIL_STR DetailStr; DetailStr.Initialize();//
		if(!m_DataCtrl.Get_JtgFycm(ElemK, FycmJtg,DetailStr)) continue;
		int iCount = 0;
		for(int j=0; j<6; j++)
		{
			T_FYCM_JTG_BASE FycmBase = FycmJtg.FycmBase[j];
			if(FycmBase.bCHK)
			{
				int iPos = (j==0 || j==1 || j==4 ? 1 : 2);
				int iTyp = 0;
				if(j>=0 && j<=3)
				{
					iTyp = (j==0 || j==2 ? 1 : 2);
				}
				else
				{
					iTyp = 3; //!/
				}
				  // Add by Maxiao.(2007-12-13).
				T_PSCD_D PscdD; PscdD.Initialize();
				m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
				if (!PscdD.bMomPrint[2] && iTyp!=1) continue;
				if (!PscdD.bMomPrint[4] && iTyp==1) continue;

				Write_Command(_T("FYCM_JT"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(FycmBase.LcomK);
				Write_Int(FycmBase.iLoadTerm);
				Write_Int(FycmBase.iMax);
				Write_Double(FycmBase.dFT);
				Write_Double(FycmBase.dFB);
				Write_Double(FycmBase.dFTL);
				Write_Double(FycmBase.dFBL);
				Write_Double(FycmBase.dFTR);
				Write_Double(FycmBase.dFBR);
				Write_Double(FycmBase.dFMAX);
				if(m_iDgnCode==JTG_D62_04||m_iDgnCode==CJJ_11_2011)
				{
					Write_Double(FycmBase.dALW);
				}
				else ASSERT(0);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_FycmJtgKList.Add(ElemK); //!/
		//////////////////////////////////////////////////////////////add by maxiao
		BOOL bPrintItem = FALSE;//m_DataCtrl.Get_JtgRptOption(ElemK,0);//0(M) 1(shear) 2(torsion) 3(rebarsM)
		if(bPrintTitle && bPrintItem)
		{
			m_rptfile.WriteString(_T("**BEGIN"));
			bPrintTitle = FALSE;
		}
		if(bPrintItem)
		{
			for(int irp=0; irp<DetailStr.arString.GetSize(); irp++)
			{
				m_rptfile.WriteString(DetailStr.CW2A(irp));
			}
		}//////////////////////////////////////////////////////////////////////////
	}
	if(!bPrintTitle) m_rptfile.WriteString(_T("\n**END\n"));//add by maxiao
	return TRUE;
}

BOOL CCRCImportFile::Write_FpmsJtg()
{
	//PROFILING(_T("CCRCImportFile::Write_FpmsJtg()"));
	BOOL bPrintTitle = TRUE;//by maxiao
	T_FPMS_JTG FpmsJtg; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		FpmsJtg.Initialize();
		T_DETAIL_STR DetailStr; DetailStr.Initialize();//
		if(!m_DataCtrl.Get_JtgFpms(ElemK, FpmsJtg,DetailStr)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_FPMS_JTG_BASE FpmsBase = FpmsJtg.FpmsBase[j];
			if(FpmsBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				// Change, Jaeoh(2008-01-23)
			  // Add by Maxiao.(2007-12-13).
				/*
				T_PSCD_D PscdD; PscdD.Initialize();
				m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
				if (!PscdD.bMomPrint[3] && iTyp==2) continue;
				if (!PscdD.bMomPrint[5] && iTyp==1) continue;
				*/

				Write_Command(_T("FPMS_JT"));                      
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(FpmsBase.LcomK);
				Write_Int(FpmsBase.iMax);
				Write_Double(FpmsBase.dFP1 );
				Write_Double(FpmsBase.dFP2 );	   
				Write_Double(FpmsBase.dFP3 );		   
				Write_Double(FpmsBase.dFP4 );         	
				Write_Double(FpmsBase.dFP5 );
				Write_Double(FpmsBase.dFP6 );	   
				Write_Double(FpmsBase.dFP7 );		   
				Write_Double(FpmsBase.dFP8 );         	
				Write_Double(FpmsBase.dFP9 );
				Write_Double(FpmsBase.dFP10);	   
				Write_Double(FpmsBase.dFMAX);		   
				Write_Double(FpmsBase.dAFP );         	
				m_iTotLineNum++;
				iCount++;
			 }
		}
		if(iCount > 0) m_FpmsJtgKList.Add(ElemK);
		//////////////////////////////////////////////////////////////add by maxiao
		BOOL bPrintItem = FALSE;//m_DataCtrl.Get_JtgRptOption(ElemK,0);//0(M) 1(shear) 2(torsion) 3(rebarsM)
		if(bPrintTitle && bPrintItem)
		{
			m_rptfile.WriteString(_T("**BEGIN"));
			bPrintTitle = FALSE;
		}
		if(bPrintItem)
		{
			for(int irp=0; irp<DetailStr.arString.GetSize(); irp++)
			{
				m_rptfile.WriteString(DetailStr.CW2A(irp));
			}
		}//////////////////////////////////////////////////////////////////////////
	}
	if(!bPrintTitle) m_rptfile.WriteString(_T("\n**END\n"));//add by maxiao
	return TRUE;
}

BOOL CCRCImportFile::Write_FpmtJtg()
{
	//PROFILING(_T("CCRCImportFile::Write_FpmtJtg()"));

	T_FPMT_JTG FpmtJtg; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		FpmtJtg.Initialize();
		if(!m_DataCtrl.Get_JtgFpmt(ElemK, FpmtJtg)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_FPMT_JTG_BASE FpmtBase = FpmtJtg.FpmtBase[j];
			if(FpmtBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("FPMT_JT"));                      
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(FpmtBase.LcomK);
				Write_Int(FpmtBase.iMax);
				Write_Double(FpmtBase.dFP1 );
				Write_Double(FpmtBase.dFP2 );	   
				Write_Double(FpmtBase.dFP3 );		   
				Write_Double(FpmtBase.dFP4 );         	
				Write_Double(FpmtBase.dFP5 );
				Write_Double(FpmtBase.dFP6 );	   
				Write_Double(FpmtBase.dFP7 );		   
				Write_Double(FpmtBase.dFP8 );         	
				Write_Double(FpmtBase.dFP9 );
				Write_Double(FpmtBase.dFP10);	   
				Write_Double(FpmtBase.dFMAX);		   
				Write_Double(FpmtBase.dAFP );         	
				m_iTotLineNum++;
				iCount++;
			 }
		}
		if(iCount > 0) m_FpmtJtgKList.Add(ElemK);
	}
	return TRUE;
}

BOOL CCRCImportFile::Write_BdcwJtg()
{
	//PROFILING(_T("CCRCImportFile::Write_BdcwJtg()"));
	BOOL bPrintTitle = TRUE;//by maxiao
	T_BDCW_D BdcwD; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BdcwD.Initialize();
		T_DETAIL_STR DetailStr; DetailStr.Initialize();//
		if(!m_DataCtrl.Get_JtgBdcw(ElemK, BdcwD, DetailStr)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_BDCW_BASE BdcwBase = BdcwD.BdcwBase[j];
			if(BdcwBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("BND_CW"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(BdcwBase.LcomK);
				Write_Int(BdcwBase.iKind);
				Write_Int(BdcwBase.iMax);
				Write_Double(BdcwBase.dFT);
				Write_Double(BdcwBase.dFB);
				Write_Double(BdcwBase.dFRT);
				Write_Double(BdcwBase.dWC );
				Write_Double(BdcwBase.dAWC);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_BdcwKList.Add(ElemK);
		//////////////////////////////////////////////////////////////add by maxiao
		BOOL bPrintItem = FALSE;//m_DataCtrl.Get_JtgRptOption(ElemK,0);//0(M) 1(shear) 2(torsion) 3(rebarsM)
		if(bPrintTitle && bPrintItem)
		{
			m_rptfile.WriteString(_T("**BEGIN"));
			bPrintTitle = FALSE;
		}
		if(bPrintItem)
		{
			for(int irp=0; irp<DetailStr.arString.GetSize(); irp++)
			{
				m_rptfile.WriteString(DetailStr.CW2A(irp));
			}
		}//////////////////////////////////////////////////////////////////////////
	}
	if(!bPrintTitle) m_rptfile.WriteString(_T("\n**END\n"));//add by maxiao
	return TRUE;
}

BOOL CCRCImportFile::Write_TbarJtg()
{
	//PROFILING(_T("CCRCImportFile::Write_TbarJtg()"));
	BOOL bPrintTitle = TRUE;//by maxiao
	T_TBAR_JTG TbarJtg; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		TbarJtg.Initialize();
		T_DETAIL_STR DetailStr; DetailStr.Initialize();//
		if(!m_DataCtrl.Get_JtgTbar(ElemK, TbarJtg, DetailStr)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_TBAR_JTG_BASE TbarBase = TbarJtg.TbarBase[j];
			if(TbarBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("TBAR_JT"));                      
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(TbarBase.LcomK);
				Write_Int(TbarBase.iMax);
				Write_Double(TbarBase.dMj);
				Write_Double(TbarBase.dREQ);	   
				Write_Double(TbarBase.dMIN);		   
				Write_Double(TbarBase.dUSE);         	
				m_iTotLineNum++;
				iCount++;
			 }
		}
		if(iCount > 0) m_TbarJtgKList.Add(ElemK);
		//////////////////////////////////////////////////////////////add by maxiao
		BOOL bPrintItem = m_DataCtrl.Get_JtgRptOption(ElemK,3);//0(M) 1(shear) 2(torsion) 3(rebarsM)
		if(bPrintTitle && bPrintItem)
		{
			m_rptfile.WriteString(_T("**BEGIN"));
			bPrintTitle = FALSE;
		}
		if(bPrintItem)
		{
			for(int irp=0; irp<DetailStr.arString.GetSize(); irp++)
			{
				m_rptfile.WriteString(DetailStr.CW2A(irp));
			}
		}//////////////////////////////////////////////////////////////////////////
	}
	if(!bPrintTitle) m_rptfile.WriteString(_T("\n**END\n"));//add by maxiao
	return TRUE;
}

BOOL CCRCImportFile::Write_TndnJtg()
{
	//PROFILING(_T("CCRCImportFile::Write_TndnJtg()"));
	BOOL bPrintTitle = TRUE;//by maxiao
	T_TNDN_JTG TndnJtg; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		TndnJtg.Initialize();
		T_DETAIL_STR DetailStr; DetailStr.Initialize();//
		if(!m_DataCtrl.Get_JtgTndn(ElemK, TndnJtg, DetailStr)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_TNDN_JTG_BASE TndnBase = TndnJtg.TndnBase[j];
			if(TndnBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("TNDN_JT"));                      
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(TndnBase.StagK);
				Write_Int(TndnBase.LcomKSer);
				Write_Int(TndnBase.iMaxSer);
				Write_Int(TndnBase.LcomK);
				Write_Int(TndnBase.iMax);
				Write_Int(TndnBase.LCombKCrack);
				Write_Int(TndnBase.iMaxCrack);
				Write_Double(TndnBase.dMg1);
				Write_Double(TndnBase.dMsum);	   
				Write_Double(TndnBase.dMj);		   
				Write_Double(TndnBase.dey);         	
				Write_Double(TndnBase.dNymin);	   
				Write_Double(TndnBase.dAymin);		   
				Write_Double(TndnBase.dAyuse);         	

				Write_Double(TndnBase.dMcr);		   //add by juhonghua 2012-3-27.
				Write_Double(TndnBase.dMud);     
				m_iTotLineNum++;
				iCount++;
			 }
		}
		if(iCount > 0) m_TndnJtgKList.Add(ElemK.first);
		//////////////////////////////////////////////////////////////add by maxiao
		BOOL bPrintItem = m_DataCtrl.Get_JtgRptOption(ElemK,3);//0(M) 1(shear) 2(torsion) 3(rebarsM)
		if(bPrintTitle && bPrintItem)
		{
			m_rptfile.WriteString(_T("**BEGIN"));
			bPrintTitle = FALSE;
		}
		if(bPrintItem)
		{
			for(int irp=0; irp<DetailStr.arString.GetSize(); irp++)
			{
				m_rptfile.WriteString(DetailStr.CW2A(irp));
			}
		}//////////////////////////////////////////////////////////////////////////
	}
	if(!bPrintTitle) m_rptfile.WriteString(_T("\n**END\n"));//add by maxiao
	return TRUE;
}

BOOL CCRCImportFile::Write_BdcyJtg()
{
	//PROFILING(_T("CCRCImportFile::Write_BdcyJtg()"));
	BOOL bPrintTitle = TRUE;//by maxiao
	T_BDCY_JTG BdcyJtg; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BdcyJtg.Initialize();
		T_DETAIL_STR DetailStr; DetailStr.Initialize();//
		if(!m_DataCtrl.Get_JtgBdcy(ElemK, BdcyJtg, DetailStr)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_BDCY_JTG_BASE BdcyBase = BdcyJtg.BdcyBase[j];
			if(BdcyBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("BDCY_JT"));                      
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(BdcyBase.LcomK);
				Write_Int(BdcyBase.iMax);
				Write_Double(BdcyBase.dMuy);
				Write_Double(BdcyBase.drMuy);	   
				Write_Double(BdcyBase.dMny);		   
				Write_Double(BdcyBase.dRatY);         
				//add by juhonghua
				Write_Double(BdcyBase.dx);	   
				Write_Double(BdcyBase.dEb);		   
				Write_Double(BdcyBase.d2a);    

				Write_Double(BdcyBase.dReBarRatio);		   
				Write_Double(BdcyBase.dMinReBarRatio);    

				m_iTotLineNum++;
				iCount++;
			 }
		}
		if(iCount > 0) m_BdcyJtgKList.Add(ElemK);
		//////////////////////////////////////////////////////////////add by maxiao
		BOOL bPrintItem = m_DataCtrl.Get_JtgRptOption(ElemK,0);//0(M) 1(shear) 2(torsion) 3(rebarsM)
		if(bPrintTitle && bPrintItem)
		{
			m_rptfile.WriteString(_T("**BEGIN"));
			bPrintTitle = FALSE;
		}
		if(bPrintItem)
		{
			for(int irp=0; irp<DetailStr.arString.GetSize(); irp++)
			{
				m_rptfile.WriteString(DetailStr.CW2A(irp));
			}
		}//////////////////////////////////////////////////////////////////////////
	}
	if(!bPrintTitle) m_rptfile.WriteString(_T("\n**END\n"));//add by maxiao
	return TRUE;
}

BOOL CCRCImportFile::Write_CumsJtg()
{
	//PROFILING(_T("CCRCImportFile::Write_CumsJtg()"));
	BOOL bPrintTitle = TRUE;//by maxiao
	T_CUMS_JTG CumsJtg; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		CumsJtg.Initialize();
		T_DETAIL_STR DetailStr; DetailStr.Initialize();//
		if(!m_DataCtrl.Get_JtgCums(ElemK, CumsJtg, DetailStr)) continue;
		int iCount = 0;
		for(int j=0; j<4; j++)
		{
			T_CUMS_JTG_BASE CumsBase = CumsJtg.CumsBase[j];
			if(CumsBase.bCHK)
			{
				int iPos = (j==0 || j==1 ? 1 : 2);
				int iTyp = (j==0 || j==2 ? 1 : 2);
				Write_Command(_T("CUMS_JT"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(CumsBase.LcomK);
				Write_Int(CumsBase.iMax);
				Write_Double(CumsBase.dVu);	 
				Write_Double(CumsBase.drVu); 
				Write_Double(CumsBase.dMu); 
				Write_Double(CumsBase.dVn);	 
				Write_Double(CumsBase.dRatV);
				Write_Double(CumsBase.dShrSect);
				Write_Double(CumsBase.dShrLoad);
				Write_Double(CumsBase.dMu_P);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_CumsJtgKList.Add(ElemK);
		//////////////////////////////////////////////////////////////add by maxiao
		BOOL bPrintItem = m_DataCtrl.Get_JtgRptOption(ElemK,1);//0(M) 1(shear) 2(torsion) 3(rebarsM)
		if(bPrintTitle && bPrintItem)
		{
			m_rptfile.WriteString(_T("**BEGIN"));
			bPrintTitle = FALSE;
		}
		if(bPrintItem)
		{
			for(int irp=0; irp<DetailStr.arString.GetSize(); irp++)
			{
				m_rptfile.WriteString(DetailStr.CW2A(irp));
			}
		}//////////////////////////////////////////////////////////////////////////
	}
	if(!bPrintTitle) m_rptfile.WriteString(_T("\n**END\n"));//add by maxiao
	return TRUE;
}

BOOL CCRCImportFile::Write_CrmtJtg()
{
	//PROFILING(_T("CCRCImportFile::Write_CrmtJtg()"));
	BOOL bPrintTitle = TRUE;//by maxiao
	T_CRMT_JTG CrmtJtg; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		CrmtJtg.Initialize();
		T_DETAIL_STR DetailStr; DetailStr.Initialize();//
		if(!m_DataCtrl.Get_JtgCrmt(ElemK, CrmtJtg,DetailStr)) continue;
		int iCount = 0;
		for(int j=0; j<6; j++)
		{
			T_CRMT_JTG_BASE CrmtBase = CrmtJtg.CrmtBase[j];
			if(CrmtBase.bCHK)
			{
				int iPos = (j==0 || j==1 || j==2 ? 1 : 2);
				// 0=I-V-MAX, 1=I-V-MIN, 2=I-T-MAX, 3=J-V-MAX, 4=J-V-MIN, 5=J-T-MAX.
				int iTyp = 0;
				if( j==0 || j==3)      iTyp = 1;
				else if( j==1 || j==4) iTyp = 2;
				else                   iTyp = 3;
				Write_Command(_T("CRMT_JT"));
				Write_Int(ElemK.first);
				Write_Int(iPos);
				Write_Int(iTyp);
				Write_Int(CrmtBase.LcomK);
				Write_Int(CrmtBase.iMax);
				Write_Double(CrmtBase.dTu);		 
				Write_Double(CrmtBase.drTu);   //1/4Tcr
				Write_Double(CrmtBase.dTn);	 
				Write_Double(CrmtBase.dMu);	 
				Write_Double(CrmtBase.dVu);
				Write_Double(CrmtBase.drVu);
				Write_Double(CrmtBase.dVn);		 
				Write_Double(CrmtBase.dRatT);
				Write_Double(CrmtBase.dShrTorVal);
				Write_Double(CrmtBase.dTorSect);
				Write_Double(CrmtBase.dTorLoad);
				Write_Double(CrmtBase.dMu_P);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_CrmtJtgKList.Add(ElemK);
		//////////////////////////////////////////////////////////////add by maxiao
		BOOL bPrintItem = m_DataCtrl.Get_JtgRptOption(ElemK,2);//0(M) 1(shear) 2(torsion) 3(rebarsM)
		if(bPrintTitle && bPrintItem)
		{
			m_rptfile.WriteString(_T("**BEGIN"));
			bPrintTitle = FALSE;
		}
		if(bPrintItem)
		{
			for(int irp=0; irp<DetailStr.arString.GetSize(); irp++)
			{
				m_rptfile.WriteString(DetailStr.CW2A(irp));
			}
		}//////////////////////////////////////////////////////////////////////////
	}
	if(!bPrintTitle) m_rptfile.WriteString(_T("\n**END\n"));//add by maxiao
	return TRUE;
}

BOOL CCRCImportFile::Write_CscsJtg()
{
	//PROFILING(_T("CCRCImportFile::Write_CscsJtg()"));
	BOOL bPrintTitle = TRUE;//by maxiao
	T_CSTCS_CH CscsJtg; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		CscsJtg.Initialize();
		T_DETAIL_STR DetailStr; DetailStr.Initialize();//
		if(!m_DataCtrl.Get_JtgCscs(ElemK, CscsJtg,DetailStr)) continue;
		int iCount = 0;
		for(int j=0; j<10; j++)
		{
			T_CSTCS_CH_BASE CscsBase = CscsJtg.cstcsBase[j];
			if(CscsBase.bCHK)
			{        
				Write_Command(_T("CSCS_JT"));
				Write_Int(ElemK.first);
				Write_Int(j/5);
				Write_Int(j%5);//Î»ÖÃ±êÊ¾// // 0=ÖáÐÄ,1/2,Fx×éÄÚÁ¦µÄÆ«ÐÄ,3/4My×éÄÚÁ¦µÄÆ«ÐÄ.(0-4:i½ØÃæ; 5-9:j½ØÃæ)
				Write_Int(CscsBase.LcomK);
				Write_Double(CscsBase.dx);
				Write_Double(CscsBase.dNd);
				Write_Double(CscsBase.drNd);
				Write_Double(CscsBase.drNde);
				Write_Double(CscsBase.drNde1);
				Write_Double(CscsBase.dNn);
				Write_Double(CscsBase.dNne);
				Write_Double(CscsBase.dNne1);
				m_iTotLineNum++;
				iCount++;
			}
		}
		if(iCount > 0) m_CscsJtgKList.Add(ElemK);
		//////////////////////////////////////////////////////////////add by maxiao
		BOOL bPrintItem = m_DataCtrl.Get_JtgRptOption(ElemK,0);//0(M) 1(shear) 2(torsion) 3(rebarsM)
		if(bPrintTitle && bPrintItem)
		{
			m_rptfile.WriteString(_T("**BEGIN"));
			bPrintTitle = FALSE;
		}
		if(bPrintItem)
		{
			for(int irp=0; irp<DetailStr.arString.GetSize(); irp++)
			{
				m_rptfile.WriteString(DetailStr.CW2A(irp));
			}
		}//////////////////////////////////////////////////////////////////////////
	}
	if(!bPrintTitle) m_rptfile.WriteString(_T("\n**END\n"));//add by maxiao
	return TRUE;
}

BOOL CCRCImportFile::Write_Fysg_AASHTO()
{
	CDgn_PSCManager PscManager;
	clock_t TimeSta = clock();
	int nElemSize = m_ElemKList.GetSize();
	for(int i=0; i<nElemSize; i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BOOL bComposite = m_DataCtrl.IsCompositePSCSect(ElemK);
		//Message Window ¼³°è ÁøÇà »óÈ² Ç¥½Ã
		int nElemNum = i+1;
		int nPercent = (nElemNum)*100/nElemSize;
		GSaveHistoryFormatNFSamePos(_LS(IDS_DGN_PSC_MSGWIN_PERCENT_FYSG),nElemNum,nElemSize,nPercent);
		if (!bComposite)
		{
			T_FYSG_AASHTO rData;
			T_FYSG_D FysgD;
			FysgD.Initialize();
			rData.Initialize();
			if(!m_DataCtrl.Get_CrcFysg(ElemK, FysgD)) continue;    
			PscManager.ConvertFysg_FysgAASHTO(FysgD, rData);
			m_pPSCDesign->WriteFysg_AASHTO(ElemK, rData);
		}
		else if (bComposite)
		{
			T_FYSG_AASHTO rData_Gr;   rData_Gr.Initialize();
			T_FYSG_D      FysgD_Gr;   FysgD_Gr.Initialize();
			T_FYSG_AASHTO rData_Sl;   rData_Sl.Initialize();
			T_FYSG_D      FysgD_Sl;   FysgD_Sl.Initialize();
			
			if(!m_DataCtrl.Get_CrcFysg_Composite(ElemK, FysgD_Gr, FysgD_Sl)) continue;    
			//Girder ¿Í Slab µû·Î binary¿¡ ÀúÀå
			PscManager.ConvertFysg_FysgAASHTO(FysgD_Gr, rData_Gr);
			m_pPSCDesign->WriteFysg_AASHTO_Composite_Girder(ElemK, rData_Gr);
			PscManager.ConvertFysg_FysgAASHTO(FysgD_Sl, rData_Sl);
			m_pPSCDesign->WriteFysg_AASHTO_Composite_Slab(ElemK, rData_Sl);
		}
		else ASSERT(0);
	}
	//TEST
	clock_t TimeEnd = clock();
	double dTimeSpan = (double)(TimeEnd-TimeSta) / CLOCKS_PER_SEC;
	//CString strTxtTime=_T("");
 // strTxtTime.Format(_T("    PSC Design Time for FYSG: %8.2f [sec]"), dTimeSpan);
	GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_MSGWIN_DGNTIME),dTimeSpan);
	return TRUE;
}

BOOL CCRCImportFile::Write_Fypc_AASHTO()
{
	if (m_bSkipTendon==TRUE) return TRUE;

	clock_t TimeSta = clock();
	CArray<T_TDNA_K,T_TDNA_K> arTdnaK;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdnaKeyList(arTdnaK);

	T_DRST_D Drst;
    switch (m_iDgnCode)
    {
    case AS_5100_5_17_PSC:
	case TMH07_3_1989:
    case BS5400_90_PSC:
    case BD_44_15_PSC:
    case CS_455_PSC:
        {
            m_pPSCDesign->ReadDrst(Drst);
        }
    default:
        break;
    }

	T_FYPC_AASHTO rData;
	//T_FYPC_KSCE FypcD;
	int nTendonSize = arTdnaK.GetSize();
	for(int i=0; i<nTendonSize; i++)
	{
		T_TDNA_K TdnaK = arTdnaK.GetAt(i);
		//Message Window ¼³°è ÁøÇà »óÈ² Ç¥½Ã
		int nTendonNum = i+1;
		int nPercent = (nTendonNum)*100/nTendonSize;
		GSaveHistoryFormatNFSamePos(_LS(IDS_DGN_PSC_MSGWIN_PERCENT_FYPC),nTendonNum,nTendonSize,nPercent);
		//FypcD.Initialize();
		rData.Initialize();
		if(!m_DataCtrl.Get_FypcData(TdnaK, rData)) continue;
		//if(!FypcD.bCHK)  continue;
		//ConvertFypcKSCE_FysgAASHTO(FypcD, rData); // ÇÒ´ç ¾ÈÇÏ´Â Data °è»êÇØ¾ßÇÔ. 
		if (m_iDgnCode==AS_5100_5_17_PSC)
		{
			if (Drst.bFypc==FALSE) Drst.bFypc = rData.bCHK;
			T_FYPC_PSC_AS FypcAS;
			m_DataCtrl.Convert_FypcToAS(rData, FypcAS);
			m_pPSCDesign->WriteFypc_AS(TdnaK, FypcAS);
		}
		else
		{
			m_pPSCDesign->WriteFypc_AASHTO(TdnaK, rData);
		}
	}

	m_pPSCDesign->WriteDrst(Drst);
	
	//TEST
	clock_t TimeEnd = clock();
	double dTimeSpan = (double)(TimeEnd-TimeSta) / CLOCKS_PER_SEC;
	GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_MSGWIN_DGNTIME),dTimeSpan);

	return TRUE;
}

BOOL CCRCImportFile::Write_Fypc_BS()
{
    if ( m_bSkipTendon==TRUE ) return TRUE;

    clock_t TimeSta = clock();
    CArray<T_TDNA_K, T_TDNA_K> arTdnaK;
    CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdnaKeyList(arTdnaK);

    T_DRST_D Drst;
    switch ( m_iDgnCode )
    {
    case BS5400_90_PSC:
	case TMH07_3_1989:
    case BD_44_15_PSC:
    case CS_455_PSC:
        {
            m_pPSCDesign->ReadDrst(Drst);
        }
    default:
        break;
    }

    T_FYPC_PSC_BS rData;
    int nTendonSize = arTdnaK.GetSize();
    for ( int i=0; i<nTendonSize; i++ )
    {
        T_TDNA_K TdnaK = arTdnaK.GetAt(i);
        //Message Window ¼³°è ÁøÇà »óÈ² Ç¥½Ã
        int nTendonNum = i+1;
        int nPercent = (nTendonNum)*100/nTendonSize;
        GSaveHistoryFormatNFSamePos(_LS(IDS_DGN_PSC_MSGWIN_PERCENT_FYPC), nTendonNum, nTendonSize, nPercent);
        rData.Initialize();
        if ( !m_DataCtrl.Get_FypcDataBS(TdnaK, rData) ) continue;
        if ( m_iDgnCode==BS5400_90_PSC || m_iDgnCode==BD_44_15_PSC || m_iDgnCode==CS_455_PSC )
        {
            if ( Drst.bFypc==FALSE ) Drst.bFypc = rData.bCHK;
            m_pPSCDesign->WriteFypc_BS(TdnaK, rData);
        }
		else if (m_iDgnCode == TMH07_3_1989) {
			if (Drst.bFypc == FALSE) Drst.bFypc = rData.bCHK;
			m_pPSCDesign->WriteFypc_BS(TdnaK, rData);
		}
        else ASSERT(0);
    }

    m_pPSCDesign->WriteDrst(Drst);

    //TEST
    clock_t TimeEnd = clock();
    double dTimeSpan = (double)(TimeEnd-TimeSta) / CLOCKS_PER_SEC;
    GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_MSGWIN_DGNTIME), dTimeSpan);

    return TRUE;
}


BOOL CCRCImportFile::Write_Fycm_AASHTO()
{
	CDgn_PSCManager PscManager;
	clock_t TimeSta = clock();
	int nElemSize = m_ElemKList.GetSize();
	for(int i=0; i<nElemSize; i++)
	{

		auto ElemK = m_ElemKList.GetAt(i);
		BOOL bComposite = m_DataCtrl.IsCompositePSCSect(ElemK);
		//Message Window ¼³°è ÁøÇà »óÈ² Ç¥½Ã
		int nElemNum = i+1;
		int nPercent = (nElemNum)*100/nElemSize;
		GSaveHistoryFormatNFSamePos(_LS(IDS_DGN_PSC_MSGWIN_PERCENT_FYCM),nElemNum,nElemSize,nPercent);
		if (!bComposite)
		{
			T_FYCM_AASHTO rData;
			T_FYCM_D FycmD;
			FycmD.Initialize();
			rData.Initialize();
			if(!m_DataCtrl.Get_CrcFycm(ElemK, FycmD)) continue;    
			PscManager.ConvertFycm_ToEngineAASHTO(FycmD, rData);
			m_pPSCDesign->WriteFycm_AASHTO(ElemK, rData);
		}
		else if (bComposite)
		{
			T_FYCM_AASHTO rData_Gr;   rData_Gr.Initialize();
			T_FYCM_D      FycmD_Gr;   FycmD_Gr.Initialize();
			T_FYCM_AASHTO rData_Sl;   rData_Sl.Initialize();
			T_FYCM_D      FycmD_Sl;   FycmD_Sl.Initialize();

			if(!m_DataCtrl.Get_CrcFycm_Composite(ElemK, FycmD_Gr, FycmD_Sl)) continue;    
			//Girder ¿Í Slab µû·Î binary¿¡ ÀúÀå
			PscManager.ConvertFycm_ToEngineAASHTO(FycmD_Gr, rData_Gr);
			m_pPSCDesign->WriteFycm_AASHTO_Composite_Girder(ElemK, rData_Gr);
			PscManager.ConvertFycm_ToEngineAASHTO(FycmD_Sl, rData_Sl);
			m_pPSCDesign->WriteFycm_AASHTO_Composite_Slab(ElemK, rData_Sl);

		}
		else ASSERT(0);
	}
	clock_t TimeEnd = clock();
	double dTimeSpan = (double)(TimeEnd-TimeSta) / CLOCKS_PER_SEC;
	GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_MSGWIN_DGNTIME),dTimeSpan);

	return TRUE;
}

BOOL CCRCImportFile::Write_Fssg_AASHTO()
{
	T_FPMT_JTG rData;
	clock_t TimeSta = clock();
	int nElemSize = m_ElemKList.GetSize();
	for(int i=0; i<nElemSize; i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		//Message Window ¼³°è ÁøÇà »óÈ² Ç¥½Ã
		int nElemNum = i+1;
		int nPercent = (nElemNum)*100/nElemSize;
		GSaveHistoryFormatNFSamePos(_LS(IDS_DGN_PSC_MSGWIN_PERCENT_FSSG),nElemNum,nElemSize,nPercent);

		rData.Initialize();
		if(!m_DataCtrl.Get_KsceFssg(ElemK, rData)) continue;
		m_pPSCDesign->WriteFssg_AASHTO(ElemK, rData);
	}
	clock_t TimeEnd = clock();
	double dTimeSpan = (double)(TimeEnd-TimeSta) / CLOCKS_PER_SEC;
	GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_MSGWIN_DGNTIME),dTimeSpan);

	return TRUE;
}

BOOL CCRCImportFile::Write_Fpms_AASHTO()
{
	clock_t TimeSta = clock();
	int nElemSize = m_ElemKList.GetSize();
	int iShrTor=1; // iShrTor (1=Shear, 2=Torsion, 3=Shear+Torsion, 4=Principal).
	T_FPMS_JTG rData;
	for(int i=0; i<nElemSize; i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		//Message Window ¼³°è ÁøÇà »óÈ² Ç¥½Ã
		int nElemNum = i+1;
		int nPercent = (nElemNum)*100/nElemSize;
		GSaveHistoryFormatNFSamePos(_LS(IDS_DGN_PSC_MSGWIN_PERCENT_FPMS),nElemNum,nElemSize,nPercent);

		rData.Initialize();
		T_DETAIL_STR DetailStr; DetailStr.Initialize();//
		if(!m_DataCtrl.Get_JtgFpms(ElemK, rData, DetailStr, iShrTor)) continue;
		m_pPSCDesign->WriteFpms_AASHTO(ElemK, rData);
	}
	clock_t TimeEnd = clock();
	double dTimeSpan = (double)(TimeEnd-TimeSta) / CLOCKS_PER_SEC;
	GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_MSGWIN_DGNTIME),dTimeSpan);

	return TRUE;
}

BOOL CCRCImportFile::Write_Fpmt_AASHTO()
{
	T_FPMS_JTG rData;
	clock_t TimeSta = clock();
	int nElemSize = m_ElemKList.GetSize();
	for(int i=0; i<nElemSize; i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		//Message Window ¼³°è ÁøÇà »óÈ² Ç¥½Ã
		int nElemNum = i+1;
		int nPercent = (nElemNum)*100/nElemSize;
		GSaveHistoryFormatNFSamePos(_LS(IDS_DGN_PSC_MSGWIN_PERCENT_FPMT),nElemNum,nElemSize,nPercent);

		rData.Initialize();
		T_DETAIL_STR DetailStr; DetailStr.Initialize();//
		if(!m_DataCtrl.Get_JtgFpms(ElemK, rData, DetailStr)) continue;
		m_pPSCDesign->WriteFpmt_AASHTO(ElemK, rData);
	}
	clock_t TimeEnd = clock();
	double dTimeSpan = (double)(TimeEnd-TimeSta) / CLOCKS_PER_SEC;
	GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_MSGWIN_DGNTIME),dTimeSpan);
	return TRUE;
}

BOOL CCRCImportFile::Write_Bdcw_AASHTO()
{
	T_BDCW_AASHTO rData;
	clock_t TimeSta = clock();
	int nElemSize = m_ElemKList.GetSize();
	for(int i=0; i<nElemSize; i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		//Message Window ¼³°è ÁøÇà »óÈ² Ç¥½Ã
		int nElemNum = i+1;
		int nPercent = (nElemNum)*100/nElemSize;
		GSaveHistoryFormatNFSamePos(_LS(IDS_DGN_PSC_MSGWIN_PERCENT_BDCW),nElemNum,nElemSize,nPercent);

		rData.Initialize();
		if(!m_DataCtrl.Get_BdcwData(ElemK, rData)) continue;
		m_pPSCDesign->WriteBdcw_AASHTO(ElemK, rData);
	}
	clock_t TimeEnd = clock();
	double dTimeSpan = (double)(TimeEnd-TimeSta) / CLOCKS_PER_SEC;
	GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_MSGWIN_DGNTIME),dTimeSpan);

	return TRUE;
}
//
BOOL CCRCImportFile::Write_Bdcw_CSA()
{
	T_BDCW_AASHTO rData_aashto; 
	T_BDCW_PSC rData; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		rData.Initialize();
		PSC_DATA_MEMB PscMembD;
		m_DataCtrl.m_pPscDataCtrl->Get_PscDataPosi(ElemK,0,PscMembD.Pos[0]);
		m_DataCtrl.m_pPscDataCtrl->Get_PscDataPosi(ElemK,1,PscMembD.Pos[1]);

		if(!m_DataCtrl.Get_BdcwDataPSC(ElemK, PscMembD, rData)) continue;
		ConvertBDCW_PSCtoAASHTO(rData, rData_aashto);

		m_pPSCDesign->WriteBdcw_CSA(ElemK, rData);   
		m_pPSCDesign->WriteBdcw_AASHTO(ElemK, rData_aashto);
		
	}
	
	return TRUE;
}
//

BOOL CCRCImportFile::ConvertBDCW_PSCtoAASHTO(T_BDCW_PSC& rData, T_BDCW_AASHTO& rData_aashto)
{  
	
	for(int i=0; i<4; i++)
	{
		const T_BDCW_PSC_BASE &BdcwBase = rData.BdcwBase[i];
		T_BDCW_AASHTO_BASE &BdcwBaseAASHTO = rData_aashto.BdcwBase[i];

		BdcwBaseAASHTO.Initialize();    
		BdcwBaseAASHTO.bCHK   = BdcwBase.bCHK   ;	
		BdcwBaseAASHTO.LcomK  = BdcwBase.LcomK  ;	
		BdcwBaseAASHTO.iKind  = BdcwBase.iKind  ;  
		BdcwBaseAASHTO.iMax   = BdcwBase.iMax   ;
		BdcwBaseAASHTO.dFT    = (-1.0)*BdcwBase.dft    ;  // (-) ÀÎÀå. (+) ¾ÐÃà	
		BdcwBaseAASHTO.dFB    = (-1.0)*BdcwBase.dfb    ;	
		BdcwBaseAASHTO.dFSS   = (-1.0)*BdcwBase.dFSS   ;	
		BdcwBaseAASHTO.ds_use = BdcwBase.dWk ;	
		BdcwBaseAASHTO.ds_max = BdcwBase.dWmax ;	
		BdcwBaseAASHTO.dSig_use = BdcwBase.dWk ;	
		BdcwBaseAASHTO.dSig_max = BdcwBase.dWmax ;	
		BdcwBaseAASHTO.bOK    = BdcwBase.bOK    ;    
	}

	return TRUE;

}


BOOL CCRCImportFile::Write_Tbar_AASHTO()
{
	T_TBAR_AASHTO rData; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		rData.Initialize();
		if(!m_DataCtrl.Get_TbarData(ElemK, rData)) continue;
		m_pPSCDesign->WriteTbar_AASHTO(ElemK, rData);
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Bdcy_AASHTO()
{
	T_BDCY_AASHTO rData;
    T_PSCD_DGN_INFO DgnD;
	clock_t TimeSta = clock();
	int nElemSize = m_ElemKList.GetSize();
	for(int i=0; i<nElemSize; i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		//Message Window ¼³°è ÁøÇà »óÈ² Ç¥½Ã
		int nElemNum = i+1;
		int nPercent = (nElemNum)*100/nElemSize;
		GSaveHistoryFormatNFSamePos(_LS(IDS_DGN_PSC_MSGWIN_PERCENT_BDCY),nElemNum,nElemSize,nPercent);

		rData.Initialize();
        DgnD.Initialize();
		if(!m_DataCtrl.Get_BdcyData(ElemK, rData, DgnD)) continue;
		m_pPSCDesign->WriteBdcy_AASHTO(ElemK, rData);
        m_pPSCDesign->SetPSCDesignInfo(ElemK, DgnD);
	}
	clock_t TimeEnd = clock();
	double dTimeSpan = (double)(TimeEnd-TimeSta) / CLOCKS_PER_SEC;
	GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_MSGWIN_DGNTIME),dTimeSpan);

	return TRUE;
}

BOOL CCRCImportFile::Write_Bdcz_AASHTO()
{
	/*
	T_BDCY_AASHTO rData; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		T_ELEM_K ElemK = m_ElemKList.GetAt(i);
		rData.Initialize();
		if(!m_DataCtrl.Get_BdczData(ElemK, rData)) continue;
		m_pPSCDesign->WriteBdcz_AASHTO(ElemK, rData);
	}
	*/
	return TRUE;
}

BOOL CCRCImportFile::Write_Cums_AASHTO()
{
	T_CUMS_AASHTO rData;
	clock_t TimeSta = clock();
	int nElemSize = m_ElemKList.GetSize();
	for(int i=0; i<nElemSize; i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		//Message Window ¼³°è ÁøÇà »óÈ² Ç¥½Ã
		int nElemNum = i+1;
		int nPercent = (nElemNum)*100/nElemSize;
		GSaveHistoryFormatNFSamePos(_LS(IDS_DGN_PSC_MSGWIN_PERCENT_CUMS),nElemNum,nElemSize,nPercent);

		rData.Initialize();
		if(!m_DataCtrl.Get_CumsData(ElemK, rData)) continue;
		m_pPSCDesign->WriteCums_AASHTO(ElemK, rData);
	}
	clock_t TimeEnd = clock();
	double dTimeSpan = (double)(TimeEnd-TimeSta) / CLOCKS_PER_SEC;
	GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_MSGWIN_DGNTIME),dTimeSpan);

	return TRUE;
}

BOOL CCRCImportFile::Write_Crmt_AASHTO()
{
	T_CRMT_AASHTO rData;
	clock_t TimeSta = clock();
	int nElemSize = m_ElemKList.GetSize();
	for(int i=0; i<nElemSize; i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		//Message Window ¼³°è ÁøÇà »óÈ² Ç¥½Ã
		int nElemNum = i+1;
		int nPercent = (nElemNum)*100/nElemSize;
		GSaveHistoryFormatNFSamePos(_LS(IDS_DGN_PSC_MSGWIN_PERCENT_CRMT),nElemNum,nElemSize,nPercent);

		rData.Initialize();
		if(!m_DataCtrl.Get_CrmtData(ElemK, rData)) continue;
		m_pPSCDesign->WriteCrmt_AASHTO(ElemK, rData);
	}
	clock_t TimeEnd = clock();
	double dTimeSpan = (double)(TimeEnd-TimeSta) / CLOCKS_PER_SEC;
	GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_MSGWIN_DGNTIME),dTimeSpan);

	return TRUE;
}

BOOL CCRCImportFile::Write_PSCDesign()
{
    T_PSCD_DGN_INFO rData;

    clock_t TimeSta = clock();
    INT_PTR nElemSize = m_ElemKList.GetSize();
    for ( INT_PTR i=0; i<nElemSize; i++ )
    {
		auto ElemK = m_ElemKList.GetAt(i);
        rData.Initialize();
        if ( m_pPSCDesign->GetPSCDesignInfo(ElemK, rData) )
        {
            m_pPSCDesign->WritePSCDesign(ElemK, rData);
        } 

        T_PSCD_DGN_DS DS;
        if ( m_pPSCDesign->GetPSCDgnDataSize(ElemK, DS) )
        {
            m_pPSCDesign->WirtePSCDgnElemDataSize(ElemK, DS);
        }
    }

    return TRUE;
}

BOOL CCRCImportFile::Write_Fysg_PSC()
{
	T_FYSG_PSC rData;
	T_FYSG_PSC_COMP rDataComp;

	CDgn_PSCManager PscManager;
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BOOL bComposite = m_DataCtrl.IsCompositePSCSect(ElemK);

		rData.Initialize();
		//if(!m_DataCtrl.Get_FysgDataPSC(ElemK, rData)) continue;    
		m_pPSCDesign->WriteFysg_PSC(ElemK, rData);

	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Fypc_PSC()
{
	Write_Fypc_AASHTO();

	return TRUE;
}

BOOL CCRCImportFile::Write_Fycm_PSC()
{
	T_FYCM_PSC rData;
	T_FYCM_PSC_COMP rDataComp;

	//T_FYCM_D FycmD;
	CDgn_PSCManager PscManager;
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BOOL bComposite = m_DataCtrl.IsCompositePSCSect(ElemK);

//     if(bComposite)
//     {
//       rDataComp.Initialize();
//       if(!m_DataCtrl.Get_FycmDataPSC_Comp(ElemK, rDataComp)) continue;    
//       m_pPSCDesign->WriteFycm_PSC_Comp(ElemK, rDataComp);
//     }
//     else
//     {
//       rData.Initialize();
//       if(!m_DataCtrl.Get_FycmDataPSC(ElemK, rData)) continue;    
//       m_pPSCDesign->WriteFycm_PSC(ElemK, rData);
//     }  
		rData.Initialize();
		//if(!m_DataCtrl.Get_FycmDataPSC(ElemK, rData)) continue;    
		m_pPSCDesign->WriteFycm_PSC(ElemK, rData);

	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Fpsg_PSC()
{
	T_FPSG_PSC rData;
	T_FPSG_PSC_COMP rDataComp;
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BOOL bComposite = m_DataCtrl.IsCompositePSCSect(ElemK);

//     if(bComposite)
//     {
//       rData.Initialize();
//       if(!m_DataCtrl.Get_FpsgDataPSC_Comp(ElemK, rDataComp)) continue;    
//       m_pPSCDesign->WriteFpsg_PSC_Comp(ElemK, rDataComp);
//     }
//     else
//     {
//       rData.Initialize();
//       if(!m_DataCtrl.Get_FpsgDataPSC(ElemK, rData)) continue;    
//       m_pPSCDesign->WriteFpsg_PSC(ElemK, rData);
//     }
		rData.Initialize();
		//if(!m_DataCtrl.Get_FpsgDataPSC(ElemK, rData)) continue;    
		m_pPSCDesign->WriteFpsg_PSC(ElemK, rData);

	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Fpms_PSC()
{
	T_FPMS_PSC rData;
	T_FPMS_PSC_COMP rDataComp;
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BOOL bComposite = m_DataCtrl.IsCompositePSCSect(ElemK);

//     if(bComposite)
//     {
//       rDataComp.Initialize();
//       if(!m_DataCtrl.Get_FpmsDataPSC_Comp(ElemK, rDataComp)) continue;    
//       m_pPSCDesign->WriteFpms_PSC_Comp(ElemK, rDataComp);
//     }
//     else
//     {
//       rData.Initialize();
//       if(!m_DataCtrl.Get_FpmsDataPSC(ElemK, rData)) continue;    
//       m_pPSCDesign->WriteFpms_PSC(ElemK, rData);
//     }    
		rData.Initialize();
		//if(!m_DataCtrl.Get_FpmsDataPSC(ElemK, rData)) continue;    
		m_pPSCDesign->WriteFpms_PSC(ElemK, rData);

	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Bdcw_PSC()
{
	T_BDCW_PSC rData; 
	T_BDCW_PSC_COMP rDataComp; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BOOL bComposite = m_DataCtrl.IsCompositePSCSect(ElemK);

//     if(bComposite)
//     {
//       rDataComp.Initialize();
//       if(!m_DataCtrl.Get_BdcwDataPSC_Comp(ElemK, rDataComp)) continue;
//       m_pPSCDesign->WriteBdcw_PSC_Comp(ElemK, rDataComp);
// 
//     }
//     else
//     {
//       rData.Initialize();
//       if(!m_DataCtrl.Get_BdcwDataPSC(ElemK, rData)) continue;
//       m_pPSCDesign->WriteBdcw_PSC(ElemK, rData);
//     }
		rData.Initialize();
		//if(!m_DataCtrl.Get_BdcwDataPSC(ElemK, rData)) continue;
		m_pPSCDesign->WriteBdcw_PSC(ElemK, rData); 
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Bdcr_PSC()
{
	T_BDCR_PSC rData; 
	T_BDCR_PSC_COMP rDataComp; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BOOL bComposite = m_DataCtrl.IsCompositePSCSect(ElemK);

//     if(bComposite)
//     {
//       rDataComp.Initialize();
//       if(!m_DataCtrl.Get_BdcrDataPSC_Comp(ElemK, rDataComp)) continue;
//       m_pPSCDesign->WriteBdcr_PSC_Comp(ElemK, rDataComp);
//     }
//     else
//     {
//       rData.Initialize();
//       if(!m_DataCtrl.Get_BdcrDataPSC(ElemK, rData)) continue;
//       m_pPSCDesign->WriteBdcr_PSC(ElemK, rData);
//     }
		rData.Initialize();
		//if(!m_DataCtrl.Get_BdcrDataPSC(ElemK, rData)) continue;
		m_pPSCDesign->WriteBdcr_PSC(ElemK, rData);

	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Cums_PSC()
{
	T_CUMS_PSC rData; 
	T_CUMS_PSC_COMP rDataComp; 

	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BOOL bComposite = m_DataCtrl.IsCompositePSCSect(ElemK);

//     if(bComposite)
//     {
//       rData.Initialize();
//       if(!m_DataCtrl.Get_CumsDataPSC_Comp(ElemK, rDataComp)) continue;
//       m_pPSCDesign->WriteCums_PSC_Comp(ElemK, rDataComp);
//     }
//     else
//     {
//       rData.Initialize();
//       if(!m_DataCtrl.Get_CumsDataPSC(ElemK, rData)) continue;
//       m_pPSCDesign->WriteCums_PSC(ElemK, rData);
//     }
		rData.Initialize();
		//if(!m_DataCtrl.Get_CumsDataPSC(ElemK, rData)) continue;
		m_pPSCDesign->WriteCums_PSC(ElemK, rData);
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Crmt_PSC()
{
	T_CRMT_PSC rData; 
	T_CRMT_PSC_COMP rDataComp; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BOOL bComposite = m_DataCtrl.IsCompositePSCSect(ElemK);

//     if(bComposite)
//     {
//       rDataComp.Initialize();
//       if(!m_DataCtrl.Get_CrmtDataPSC_Comp(ElemK, rDataComp)) continue;
//       m_pPSCDesign->WriteCrmt_PSC_Comp(ElemK, rDataComp);
//     }
//     else
//     {
//       rData.Initialize();
//       if(!m_DataCtrl.Get_CrmtDataPSC(ElemK, rData)) continue;
//       m_pPSCDesign->WriteCrmt_PSC(ElemK, rData);
//     }
		rData.Initialize();
		//if(!m_DataCtrl.Get_CrmtDataPSC(ElemK, rData)) continue;
		m_pPSCDesign->WriteCrmt_PSC(ElemK, rData);

	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// LSD15 , Composite PSC
BOOL CCRCImportFile::Write_CHECK_PSC()
{
	int nElemKSize=m_ElemKList.GetSize();
	for(int i=0; i<nElemKSize; ++i)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		//Message Window ¼³°è ÁøÇà »óÈ² Ç¥½Ã
		int nElemNum = i+1;
		int nPercent = (nElemNum)*100/nElemKSize;
		GSaveHistoryFormatNFSamePos(_LS(IDS_DGN_PSC_MSGWIN_PERFORM),nElemNum,nElemKSize,nPercent);
		if(!m_DataCtrl.Get_PSCCheckResult(ElemK)) continue;
	}
	GSaveHistoryFormatNF(_T(""));

	return TRUE;
}

BOOL CCRCImportFile::Write_CHECK_PSC_AS()
{
	T_DRST_D Drst;
    m_DataCtrl.CalcTendonfpsLpPosition(); // Transmission Lp, Lpt À§Ä¡ÀÇ fps °è»êÀ» À§ÇØ..
	int nElemKSize=m_ElemKList.GetSize();
	for(int i=0; i<nElemKSize; ++i)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		//Message Window ¼³°è ÁøÇà »óÈ² Ç¥½Ã
		int nElemNum = i+1;
		int nPercent = (nElemNum)*100/nElemKSize;
		GSaveHistoryFormatNFSamePos(_LS(IDS_DGN_PSC_MSGWIN_PERFORM),nElemNum,nElemKSize,nPercent);
		if(!m_DataCtrl.Get_PSCCheckResultAS(ElemK, Drst)) continue;
	}
	GSaveHistoryFormatNF(_T(""));

	m_pPSCDesign->WriteDrst(Drst);

	return TRUE;
}

BOOL CCRCImportFile::Write_CHECK_PSC_BS()
{
    T_DRST_D Drst;
    int nElemKSize=m_ElemKList.GetSize();
    for(int i=0; i<nElemKSize; ++i)
    {
		auto ElemK = m_ElemKList.GetAt(i);
        //Message Window ¼³°è ÁøÇà »óÈ² Ç¥½Ã
        int nElemNum = i+1;
        int nPercent = (nElemNum)*100/nElemKSize;
        GSaveHistoryFormatNFSamePos(_LS(IDS_DGN_PSC_MSGWIN_PERFORM),nElemNum,nElemKSize,nPercent);
        if(!m_DataCtrl.Get_PSCCheckResultBS(ElemK, Drst)) continue;
    }
    GSaveHistoryFormatNF(_T(""));

    m_pPSCDesign->WriteDrst(Drst);

    return TRUE;
}

BOOL CCRCImportFile::Write_Fysg_PSC_LSD15()
{
	T_FYSG_PSC rData;
	T_FYSG_PSC_COMP rDataComp;
	
	CDgn_PSCManager PscManager;
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BOOL bComposite = m_DataCtrl.IsCompositePSCSect(ElemK);
		if(bComposite)
		{
			rDataComp.Initialize();
			//if(!m_DataCtrl.Get_FysgDataPSC_Comp(ElemK, rDataComp)) continue;    
			m_pPSCDesign->WriteFysg_PSC_Comp(ElemK, rDataComp);
			
		}
		else
		{
			rDataComp.Initialize();
			rData.Initialize();
			//if(!m_DataCtrl.Get_FysgDataPSC(ElemK, rData)) continue;    
			m_DataCtrl.Convert_FysgDataPSC_to_Compo(rData, rDataComp);

			m_pPSCDesign->WriteFysg_PSC(ElemK, rData);
			m_pPSCDesign->WriteFysg_PSC_Comp(ElemK, rDataComp);
		}
	}
	
	return TRUE;
}

BOOL CCRCImportFile::Write_Fypc_PSC_LSD15()
{
	Write_Fypc_AASHTO();
	
	return TRUE;
}

BOOL CCRCImportFile::Write_Fycm_PSC_LSD15()
{
	T_FYCM_PSC rData;
	T_FYCM_PSC_COMP rDataComp;
	
	//T_FYCM_D FycmD;
	CDgn_PSCManager PscManager;
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BOOL bComposite = m_DataCtrl.IsCompositePSCSect(ElemK);
		
		if(bComposite)
		{
			rDataComp.Initialize();
			//if(!m_DataCtrl.Get_FycmDataPSC_Comp(ElemK, rDataComp)) continue;    
			m_pPSCDesign->WriteFycm_PSC_Comp(ElemK, rDataComp);
		}
		else
		{
			rDataComp.Initialize();
			rData.Initialize();
			//if(!m_DataCtrl.Get_FycmDataPSC(ElemK, rData)) continue;    
			m_DataCtrl.Convert_FycmDataPSC_to_Compo(rData, rDataComp);

			m_pPSCDesign->WriteFycm_PSC(ElemK, rData);     
			m_pPSCDesign->WriteFycm_PSC_Comp(ElemK, rDataComp);
		}  
	}
	
	return TRUE;
}

BOOL CCRCImportFile::Write_Fpsg_PSC_LSD15()
{
	T_FPSG_PSC rData;
	T_FPSG_PSC_COMP rDataComp;
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BOOL bComposite = m_DataCtrl.IsCompositePSCSect(ElemK);

		if(bComposite)
		{
			rData.Initialize();
			//if(!m_DataCtrl.Get_FpsgDataPSC_Comp(ElemK, rDataComp)) continue;    
			m_pPSCDesign->WriteFpsg_PSC_Comp(ElemK, rDataComp);
		}
		else
		{
			rData.Initialize();
			//if(!m_DataCtrl.Get_FpsgDataPSC(ElemK, rData)) continue;    
			m_DataCtrl.Convert_FpsgDataPSC_to_Compo(rData, rDataComp);

			m_pPSCDesign->WriteFpsg_PSC(ElemK, rData);
			m_pPSCDesign->WriteFpsg_PSC_Comp(ElemK, rDataComp);

		}
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Fpms_PSC_LSD15()
{
	T_FPMS_PSC rData;
	T_FPMS_PSC_COMP rDataComp;
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BOOL bComposite = m_DataCtrl.IsCompositePSCSect(ElemK);

		if(bComposite)
		{
			rDataComp.Initialize();
			//if(!m_DataCtrl.Get_FpmsDataPSC_Comp(ElemK, rDataComp)) continue;    
			m_pPSCDesign->WriteFpms_PSC_Comp(ElemK, rDataComp);
		}
		else
		{
			rDataComp.Initialize();
			rData.Initialize();
			//if(!m_DataCtrl.Get_FpmsDataPSC(ElemK, rData)) continue;    
			m_DataCtrl.Convert_FpmsDataPSC_to_Compo(rData, rDataComp);

			m_pPSCDesign->WriteFpms_PSC(ElemK, rData);
			m_pPSCDesign->WriteFpms_PSC_Comp(ElemK, rDataComp);

		}    
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Bdcw_PSC_LSD15()
{
	T_BDCW_PSC rData; 
	T_BDCW_PSC_COMP rDataComp; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BOOL bComposite = m_DataCtrl.IsCompositePSCSect(ElemK);

		if(bComposite)
		{
			rDataComp.Initialize();
			//if(!m_DataCtrl.Get_BdcwDataPSC_Comp(ElemK, rDataComp)) continue;
			m_pPSCDesign->WriteBdcw_PSC_Comp(ElemK, rDataComp);

		}
		else
		{
			rDataComp.Initialize();
			rData.Initialize();
			//if(!m_DataCtrl.Get_BdcwDataPSC(ElemK, rData)) continue;
			m_DataCtrl.Convert_BdcwDataPSC_to_Compo(rData, rDataComp);

			m_pPSCDesign->WriteBdcw_PSC(ElemK, rData);     
			m_pPSCDesign->WriteBdcw_PSC_Comp(ElemK, rDataComp);

		}
	 
	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Bdcr_PSC_LSD15()
{
	T_BDCR_PSC rData; 
	T_BDCR_PSC_COMP rDataComp; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BOOL bComposite = m_DataCtrl.IsCompositePSCSect(ElemK);

		if(bComposite)
		{
			rDataComp.Initialize();
			//if(!m_DataCtrl.Get_BdcrDataPSC_Comp(ElemK, rDataComp)) continue;
			m_pPSCDesign->WriteBdcr_PSC_Comp(ElemK, rDataComp);
		}
		else
		{
			rDataComp.Initialize();
			rData.Initialize();
			//if(!m_DataCtrl.Get_BdcrDataPSC(ElemK, rData)) continue;
			m_DataCtrl.Convert_BdcrDataPSC_to_Compo(rData, rDataComp);

			m_pPSCDesign->WriteBdcr_PSC(ElemK, rData);
			m_pPSCDesign->WriteBdcr_PSC_Comp(ElemK, rDataComp);

		}

	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Cums_PSC_LSD15()
{
	T_CUMS_PSC rData; 
	T_CUMS_PSC_COMP rDataComp; 

	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BOOL bComposite = m_DataCtrl.IsCompositePSCSect(ElemK);

		if(bComposite)
		{
			rDataComp.Initialize();
			//if(!m_DataCtrl.Get_CumsDataPSC_Comp(ElemK, rDataComp)) continue;
			m_pPSCDesign->WriteCums_PSC_Comp(ElemK, rDataComp);
		}
		else
		{
			rDataComp.Initialize();
			rData.Initialize();
			//if(!m_DataCtrl.Get_CumsDataPSC(ElemK, rData)) continue;
			m_DataCtrl.Convert_CumsDataPSC_to_Compo(rData, rDataComp);

			m_pPSCDesign->WriteCums_PSC(ElemK, rData);     
			m_pPSCDesign->WriteCums_PSC_Comp(ElemK, rDataComp);
		}

	}

	return TRUE;
}

BOOL CCRCImportFile::Write_Crmt_PSC_LSD15()
{
	T_CRMT_PSC rData; 
	T_CRMT_PSC_COMP rDataComp; 
	for(int i=0; i<m_ElemKList.GetSize(); i++)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		BOOL bComposite = m_DataCtrl.IsCompositePSCSect(ElemK);

		if(bComposite)
		{
			rDataComp.Initialize();
			//if(!m_DataCtrl.Get_CrmtDataPSC_Comp(ElemK, rDataComp)) continue;
			m_pPSCDesign->WriteCrmt_PSC_Comp(ElemK, rDataComp);
		}
		else
		{
			rDataComp.Initialize();
			rData.Initialize();
			//if(!m_DataCtrl.Get_CrmtDataPSC(ElemK, rData)) continue;
			m_DataCtrl.Convert_CrmtDataPSC_to_Compo(rData, rDataComp);

			m_pPSCDesign->WriteCrmt_PSC(ElemK, rData);
			m_pPSCDesign->WriteCrmt_PSC_Comp(ElemK, rDataComp);
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// SNiP 2.05.03-84*, SP 35.13330.2011
BOOL CCRCImportFile::Write_FirstGroupLS()
{
	T_BDCR_SNIP BcdrD;
	T_CUMS_SNIP CumsD;
	T_CRMT_SNIP CrmtD;
	T_FATI_SNIP FatiD;
	int nElemListSize = m_ElemKList.GetSize();
	for(int i=0; i<nElemListSize; ++i)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		if(!m_DataCtrl.CheckFirstGroupLS(ElemK, BcdrD, CumsD, CrmtD, FatiD)) continue;
		m_pPSCDesign->WriteBdcr_SNiP(ElemK, BcdrD);
		m_pPSCDesign->WriteCums_SNiP(ElemK, CumsD);
		m_pPSCDesign->WriteCrmt_SNiP(ElemK, CrmtD);
		m_pPSCDesign->WriteFati_SNiP(ElemK, FatiD);
	}
	return TRUE;
}

BOOL CCRCImportFile::Write_SecondGroupLS()
{
	T_FYSG_SNIP FysgD;
	T_FYCM_SNIP FycmD;
	T_FPSG_SNIP FpsgD;
	T_FPMS_SNIP FpmsD;
	T_BDCW_SNIP BdcwD;
	int nElemListSize = m_ElemKList.GetSize();
	for(int i=0; i<nElemListSize; ++i)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		if(!m_DataCtrl.CheckSecondGroupLS(ElemK, FysgD, FycmD, FpsgD, FpmsD, BdcwD)) continue;
		m_pPSCDesign->WriteFysg_SNiP(ElemK, FysgD);
		m_pPSCDesign->WriteFycm_SNiP(ElemK, FycmD);
		m_pPSCDesign->WriteFpsg_SNiP(ElemK, FpsgD);
		m_pPSCDesign->WriteFpms_SNiP(ElemK, FpmsD);
		m_pPSCDesign->WriteBdcw_SNiP(ElemK, BdcwD);
	}

	if (m_bSkipTendon==TRUE) return TRUE;

	CArray<T_TDNA_K,T_TDNA_K> arTdnaK;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdnaKeyList(arTdnaK);

	int nTndaSize = arTdnaK.GetSize();
	T_FYPC_SNIP FypcD;
	//T_FYPC_KSCE FypcD;
	for(int i=0; i<nTndaSize; ++i)
	{
		T_TDNA_K TdnaK = arTdnaK.GetAt(i);    
		FypcD.Initialize();
		if(!m_DataCtrl.Get_FypcDataSNiP(TdnaK, FypcD)) continue;        
		m_pPSCDesign->WriteFypc_SNiP(TdnaK, FypcD);
	}
	
	return TRUE;
}

BOOL CCRCImportFile::Write_Bdcw_SNiP()
{
	T_BDCW_SNIP rData; 
	int nElemListSize = m_ElemKList.GetSize();
	for(int i=0; i<nElemListSize; ++i)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		rData.Initialize();
		if(!m_DataCtrl.Get_BdcwDataSNiP(ElemK, rData)) continue;
		m_pPSCDesign->WriteBdcw_SNiP(ElemK, rData);
	}
	return TRUE;
}

BOOL CCRCImportFile::Write_Bdcr_SNiP()
{
	T_BDCR_SNIP rData; 
	int nElemListSize = m_ElemKList.GetSize();
	for(int i=0; i<nElemListSize; ++i)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		rData.Initialize();
		if(!m_DataCtrl.Get_BdcrDataSNiP(ElemK, rData)) continue;
		m_pPSCDesign->WriteBdcr_SNiP(ElemK, rData);
	}
	return TRUE;
}

BOOL CCRCImportFile::Write_Cums_SNiP()
{
	T_CUMS_SNIP rCums; 
	T_CRMT_SNIP rCrmt;
	int nElemListSize = m_ElemKList.GetSize();
	for(int i=0; i<nElemListSize; ++i)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		rCums.Initialize(); rCrmt.Initialize();
		if(!m_DataCtrl.Get_CumsDataSNiP(ElemK, rCums, rCrmt)) continue;
		m_pPSCDesign->WriteCums_SNiP(ElemK, rCums);
		m_pPSCDesign->WriteCrmt_SNiP(ElemK, rCrmt);
	}
	return TRUE;
}

BOOL CCRCImportFile::Write_Fati_SNiP()
{
	T_FATI_SNIP rData; 
	int nElemListSize = m_ElemKList.GetSize();
	for(int i=0; i<nElemListSize; ++i)
	{
		auto ElemK = m_ElemKList.GetAt(i);
		rData.Initialize();
		if(!m_DataCtrl.Get_FatiDataSNiP(ElemK, rData)) continue;
		m_pPSCDesign->WriteFati_SNiP(ElemK, rData);
	}
	return TRUE;
}

BOOL CCRCImportFile::Write_Fypc_IRS()
{
	//if (m_bSkipTendon==TRUE) return TRUE;

	clock_t TimeSta = clock();
	CArray<T_TDNA_K,T_TDNA_K> arTdnaK;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdnaKeyList(arTdnaK);

	T_DRST_D Drst;
	switch (m_iDgnCode)
	{
	case AS_5100_5_17_PSC:
	case BS5400_90_PSC:
	case TMH07_3_1989:
	case BD_44_15_PSC:
    case CS_455_PSC:
	case IRS_PSC:
		{
			m_pPSCDesign->ReadDrst(Drst);
		}
	default:
		break;
	}

	T_FYPC_AASHTO rData;
	//T_FYPC_KSCE FypcD;
	int nTendonSize = arTdnaK.GetSize();
	for(int i=0; i<nTendonSize; i++)
	{
		T_TDNA_K TdnaK = arTdnaK.GetAt(i);
		//Message Window ¼³°è ÁøÇà »óÈ² Ç¥½Ã
		int nTendonNum = i+1;
		int nPercent = (nTendonNum)*100/nTendonSize;
		GSaveHistoryFormatNFSamePos(_LS(IDS_DGN_PSC_MSGWIN_PERCENT_FYPC),nTendonNum,nTendonSize,nPercent);
		//FypcD.Initialize();
		rData.Initialize();
		if(!m_DataCtrl.Get_FypcData(TdnaK, rData)) continue;
		//if(!FypcD.bCHK)  continue;
		//ConvertFypcKSCE_FysgAASHTO(FypcD, rData); // ÇÒ´ç ¾ÈÇÏ´Â Data °è»êÇØ¾ßÇÔ. 
		if (m_iDgnCode==AS_5100_5_17_PSC)
		{
			if (Drst.bFypc==FALSE) Drst.bFypc = rData.bCHK;
			T_FYPC_PSC_AS FypcAS;
			m_DataCtrl.Convert_FypcToAS(rData, FypcAS);
			m_pPSCDesign->WriteFypc_AS(TdnaK, FypcAS);
		}
		else if (m_iDgnCode==IRS_PSC)
		{
			if (Drst.bFypc==FALSE) Drst.bFypc = rData.bCHK;
			T_FYPC_IRS FypcAS;
			m_DataCtrl.Convert_FypcToIRS(rData, FypcAS);
			m_pPSCDesign->WriteFypc_IRS(TdnaK, FypcAS);
		}
		else
		{
			m_pPSCDesign->WriteFypc_AASHTO(TdnaK, rData);
		}


	}

	m_pPSCDesign->WriteDrst(Drst);

	//TEST
	clock_t TimeEnd = clock();
	double dTimeSpan = (double)(TimeEnd-TimeSta) / CLOCKS_PER_SEC;
	GSaveHistoryFormatNF(_LS(IDS_DGN_PSC_MSGWIN_DGNTIME),dTimeSpan);

	return TRUE;
}
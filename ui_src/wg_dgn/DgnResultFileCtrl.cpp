// DgnResultFileCtrl.cpp: implementation of the CDgnResultFileCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DgnResultFileCtrl.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\BnFile.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_dbLock\LockMgr.h"
#include "..\wg_base\wg_base_UtilFunc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// static member

CString CDgnResultFileCtrl::m_sDesignExt;


CDgnResultFileCtrl::CDgnResultFileCtrl()
{
	m_sDesignExt = _T(".cdf");

	ClearResultFlag();
}

CDgnResultFileCtrl::~CDgnResultFileCtrl()
{
	CloseDesignResultFile(D_DESIGN_ANALYSIS_ALL);
}

void CDgnResultFileCtrl::Initialize()
{
}

BOOL CDgnResultFileCtrl::IsEnableDesignResult(int nDesignResultId)
{
	CFileStatus status;

TRY
{
	switch(nDesignResultId)
	{
	case D_DESIGN_FORCE_RESULT:
	case D_DESIGN_LCASE_FORCE_RESULT:
	case D_DESIGN_LOAD_CASE_FORCE:
	case D_DESIGN_LOAD_CASE_PART_FORCE:

		if(m_ResultFile[nDesignResultId].GetStatus(status))return TRUE;
	case D_DESIGN_ANALYSIS_ALL:
	default:
		ASSERT(FALSE);
	}
}
CATCH( CException, e ) e->ReportError();
END_CATCH

	return FALSE;
}

BOOL CDgnResultFileCtrl::OpenDesignResultFile(int nDesignResultId)
{
	if(!CloseDesignResultFile(nDesignResultId))return FALSE;

	CFileException ex;
	CFileStatus status;
	CString strResultFileName;

TRY
{
	switch(nDesignResultId)
	{
	case D_DESIGN_FORCE_RESULT:
	case D_DESIGN_LCASE_FORCE_RESULT:
	case D_DESIGN_LOAD_CASE_FORCE:
	case D_DESIGN_LOAD_CASE_PART_FORCE:

		strResultFileName = GetResultFileName(nDesignResultId);
		if(!CFile::GetStatus(strResultFileName, status))break;
		if(!m_ResultFile[nDesignResultId].Open(strResultFileName, CFile::modeRead|CFile::shareDenyNone, &ex))
				AfxThrowFileException(ex.m_cause, ex.m_lOsError, ex.m_strFileName);
		break;
	case D_DESIGN_ANALYSIS_ALL:
	default:
		ASSERT(FALSE);
		return FALSE;
	}
	return TRUE;
}
CATCH( CException, e ) e->ReportError();
END_CATCH

	return FALSE;
}

BOOL CDgnResultFileCtrl::CloseDesignResultFile(int nDesignResultId)
{
	switch(nDesignResultId)
	{
	case D_DESIGN_FORCE_RESULT:
	case D_DESIGN_LCASE_FORCE_RESULT:
	case D_DESIGN_LOAD_CASE_FORCE:
	case D_DESIGN_LOAD_CASE_PART_FORCE:

		m_ResultFile[nDesignResultId].Abort();
		break;
	case D_DESIGN_ANALYSIS_ALL:
		{
			for(int i=0; i<D_DESIGN_FILE_ARRAY_MAX; i++)m_ResultFile[i].Abort();
		}
		break;
	default:
		ASSERT(FALSE);
		return FALSE;
	}
	return TRUE;
}

BOOL CDgnResultFileCtrl::DeleteDesignResultFile(int nDesignResultId, BOOL bUpdateView)
{
	if(!CloseDesignResultFile(nDesignResultId))return FALSE;

	CFileStatus status;
	CString strResultFileName;

TRY
{
	switch(nDesignResultId)
	{
	case D_DESIGN_FORCE_RESULT:
	case D_DESIGN_LCASE_FORCE_RESULT:
	case D_DESIGN_LOAD_CASE_FORCE:
	case D_DESIGN_LOAD_CASE_PART_FORCE:

		strResultFileName = GetResultFileName(nDesignResultId);
		if(CFile::GetStatus(strResultFileName, status))CFile::Remove(strResultFileName);
		break;
	case D_DESIGN_ANALYSIS_ALL:
		{
			for(int i=0; i<D_DESIGN_FILE_ARRAY_MAX; i++)
			{
				strResultFileName = GetResultFileName(i);
				if(CFile::GetStatus(strResultFileName, status))CFile::Remove(strResultFileName);
			}
		}
		break;
	default:
		ASSERT(FALSE);
		return FALSE;
	}
	if(bUpdateView)
	{
		//LPARAM lUpdateType = GetViewUpdateType(nDesignResultId);
		//CDBDoc::GetDocPoint()->UpdateViews(lUpdateType);
	}
	return TRUE;
}
CATCH( CException, e ) e->ReportError();
END_CATCH

	return FALSE;
}

BOOL CDgnResultFileCtrl::ExistDesignResultFile(int nDesignResultId)
{
	CFileException ex;
	CFileStatus status;
	CString strResultFileName;

TRY
{
	switch(nDesignResultId)
	{
	case D_DESIGN_FORCE_RESULT:
	case D_DESIGN_LCASE_FORCE_RESULT:
	case D_DESIGN_LOAD_CASE_FORCE:
	case D_DESIGN_LOAD_CASE_PART_FORCE:

		strResultFileName = GetResultFileName(nDesignResultId);
		if(CFile::GetStatus(strResultFileName, status))return TRUE;
		break;
	case D_DESIGN_ANALYSIS_ALL:
	default:
		ASSERT(FALSE);
		return FALSE;
	}
}
CATCH( CException, e ) e->ReportError();
END_CATCH

	return FALSE;
}

BOOL CDgnResultFileCtrl::BackupDesignResultFile(int nDesignResultId)
{
	CFileException ex;
	CFileStatus status;
	CString strResultFileName, strResultBackupFileName;

	/*
TRY
{
	switch(nDesignResultId)
	{
	case D_RESULT_GRIDMODEL_INFLUENCE:
	case D_RESULT_GRIDMODEL_INFLUENCE_BACKUP:
		strResultFileName = GetResultFileName(D_RESULT_GRIDMODEL_INFLUENCE);
		strResultBackupFileName = GetResultFileName(D_RESULT_GRIDMODEL_INFLUENCE_BACKUP);
		if(!CFile::GetStatus(strResultFileName, status))return FALSE;
		if(CFile::GetStatus(strResultBackupFileName, status))CFile::Remove(strResultBackupFileName);
		CFile::Rename(strResultFileName, strResultBackupFileName);
		break;
	default:
		ASSERT(FALSE);
		return FALSE;
	}
	return TRUE;
}
CATCH( CException, e ) e->ReportError();
END_CATCH

*/
	return FALSE;
}

BOOL CDgnResultFileCtrl::RestoreDesignResultFile(int nDesignResultId)
{
	CFileException ex;
	CFileStatus status;
	CString strResultFileName, strResultBackupFileName;
/*
TRY
{
	switch(nDesignResultId)
	{
	case D_RESULT_GRIDMODEL_INFLUENCE:
	case D_RESULT_GRIDMODEL_INFLUENCE_BACKUP:
		strResultFileName = GetResultFileName(D_RESULT_GRIDMODEL_INFLUENCE);
		strResultBackupFileName = GetResultFileName(D_RESULT_GRIDMODEL_INFLUENCE_BACKUP);
		if(!CFile::GetStatus(strResultBackupFileName, status))return FALSE;
		if(CFile::GetStatus(strResultFileName, status))CFile::Remove(strResultFileName);
		CFile::Rename(strResultBackupFileName, strResultFileName);
		break;
	default:
		ASSERT(FALSE);
		return FALSE;
	}
	return TRUE;
}
CATCH( CException, e ) e->ReportError();
END_CATCH
*/
	return FALSE;
}

CString CDgnResultFileCtrl::GetResultFileName(int nDesignResultId)
{
	CString strPathName=CDBDoc::GetDocPoint()->GetPathName();
	CFileCtrl FileCtrl(strPathName);
	CString strResultFileName=FileCtrl.GetFilePathAndNameWithoutExtension()+m_sDesignExt;
	//CString strDesignFileName=FileCtrl.GetFilePathAndNameWithoutExtension()+m_sDesignExt;
	//CString strDgnMdulFileName=FileCtrl.GetFilePathAndNameWithoutExtension()+m_sDgnMdulExt;

	CString strResult;
	switch(nDesignResultId)
	{
	case D_DESIGN_FORCE_RESULT:
		strResult.Format(_T("%s%s"), strResultFileName, _T("1"));
		return strResult; //_T(strResultFileName)+_T("1");
		
	case D_DESIGN_LCASE_FORCE_RESULT:
		strResult.Format(_T("%s%s"), strResultFileName, _T("2"));
		return strResult; //_T(strResultFileName)+_T("2");

	case D_DESIGN_LOAD_CASE_FORCE:
		strResult.Format(_T("%s%s"), strResultFileName, _T("3"));
		return strResult; //_T(strResultFileName)+_T("3");

	case D_DESIGN_LOAD_CASE_PART_FORCE:
		strResult.Format(_T("%s%s"), strResultFileName, _T("4"));
		return strResult; //_T(strResultFileName)+_T("4");

	case D_DESIGN_ANALYSIS_ALL:
	default:
		ASSERT(FALSE);
		return strResultFileName;
	}
	return strResultFileName;
}

LPARAM CDgnResultFileCtrl::GetViewUpdateType(int nDesignResultId)
{
	LPARAM lUpdateType = D_UPDATE_DEFAULT;
	CString strPathName=CDBDoc::GetDocPoint()->GetPathName();
	CFileCtrl FileCtrl(strPathName);
	CString strResultFileName=FileCtrl.GetFilePathAndNameWithoutExtension();

/*
	switch(nDesignResultId)
	{
	case D_RESULT_GRIDMODEL_INFLUENCE:
		lUpdateType=D_UPDATE_REMOVE_INFLGRID;
		break;
	case D_RESULT_GRIDMODEL_ANALYSIS:
		lUpdateType=D_UPDATE_REMOVE_GRIDMODEL;
		break;
	case D_RESULT_PUSHOVER_ANALYSIS:
		lUpdateType=D_UPDATE_REMOVE_PUSHOVER;
		break;
	case D_RESULT_PCDESIGN_ANALYSIS:
		lUpdateType=D_UPDATE_REMOVE_PCDESIGN;
		break;
	case D_RESULT_DESIGN_IMPORT_FORCE:     
		lUpdateType=D_UPDATE_REMOVE_IMPDGNFORCE;
		break;
	case D_RESULT_DESIGN_BRIDGE_LOAD_RATING:     
		lUpdateType=D_UPDATE_REMOVE_LOADRATING;
		break;
	case D_RESULT_DESIGN_PLATE_GIRDER:     
		lUpdateType=D_UPDATE_REMOVE_PLATEGIRDER;
		break;
// [24/11/2006 : maxiao] For CIVIL(CH) 2006
	case D_RESULT_CHRC_BEAMDESIGN_ANALYSIS:
		lUpdateType=D_UPDATE_REMOVE_CHRC_BEAMDESIGN;
		break;
	case D_RESULT_CHRC_COLUMNDESIGN_ANALYSIS:
		lUpdateType=D_UPDATE_REMOVE_CHRC_COLUMNDESIGN;
		break;
		
	case D_RESULT_TIMEHISTORY_EXTENDED_ANALYSIS:
	case D_RESULT_TIMEHISTORY_ANIMATION_ANALYSIS:
	case D_RESULT_TIMEHISTORY_PUSHOVER_ANALYSIS:  
		return D_UPDATE_DEFAULT;  
		break; 
	case D_DESIGN_ANALYSIS_ALL:
	default:
		ASSERT(FALSE);
		return D_UPDATE_DEFAULT;
	}
	*/
	return lUpdateType;
}

CFile* CDgnResultFileCtrl::GetDgnForceResultFilePoint(int nDesignResultId)
{
	return &m_ResultFile[nDesignResultId];
}

void CDgnResultFileCtrl::SetResultFlag(int nResultFlagId, BOOL bResultFlag)
{
	if(nResultFlagId < 0 || nResultFlagId >= D_DESIGN_FLAG_ARRAY_MAX){ASSERT(FALSE); return;}
	m_bResultFlag[nResultFlagId]=bResultFlag;
}

BOOL CDgnResultFileCtrl::GetResultFlag(int nResultFlagId)
{
	if(nResultFlagId < 0 || nResultFlagId >= D_DESIGN_FLAG_ARRAY_MAX){ASSERT(FALSE); return FALSE;}
	return m_bResultFlag[nResultFlagId];
}

void CDgnResultFileCtrl::ClearResultFlag()
{
	for(int i=0; i<D_DESIGN_FLAG_ARRAY_MAX; i++)m_bResultFlag[i]=FALSE;
}

int CDgnResultFileCtrl::GetCountResultFlagTrue()
{
	int nCount=0;
	for(int i=0; i<D_DESIGN_FLAG_ARRAY_MAX; i++)if(m_bResultFlag[i])nCount++;
	return nCount;
}

void CDgnResultFileCtrl::DisplayErrorMessage(LPCTSTR lpszErrorMsg, int nErrorCode)
{
	if(CLockMgr::Get_IsRegressionTest()) return;

	CString strTemp, strError(lpszErrorMsg);
	CBnFile::GetErrorMessage(nErrorCode, strTemp);
	
	strError=strError+_T(" ")+strTemp;
	TimedMessageBox(NULL, NULL, MB_ICONSTOP, 1000*60, strError);  // 1분 기다림
}


// VerifyDgnResultFile.cpp: implementation of the CVerifyDgnResultFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VerifyDgnResultFile.h"
#include "DgnResultFileCtrl.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\ResultDataFilePosition.h"
#include "..\wg_db\PCDesign.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_base\product.h"
#include "..\wg_base\wg_base_UtilFunc.h"
#include "CRCForceCtrl.h"
#include "..\wg_dbLock\LockMgr.h"
//#include "CRCDataCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define GETSTEP_BYCASE_PO(SerialLdc) m_aPushOverLoadStepByUser[SerialLdc-1]
																				 

#define CHECK_FORM_TRY \
	try \
	{

#ifdef _DEBUG
#define CHECK_FORM_CATCH \
	} \
	catch(int iercod) \
	{ \
		GSaveHistoryFormatNF(_T("Error No. = %d"), iercod); \
		return TRUE; \
	}
#else
#define CHECK_FORM_CATCH \
	} \
	catch(int iercod) \
	{ \
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod); \
		return FALSE; \
	}
#endif

// 데이터 없으면 오류처리
#ifdef _DEBUG
#define CHECK_FORM_BNFILE_READ_GOOD \
	int iercod=m_pBnFile->READ_F(ID_M, ID_S, IST_R, LNG_R, NUM_R); \
	if(iercod != BN_ER_GOOD)GSaveHistoryFormatNF(_T("iercod = %d"), iercod); \
	if(iercod != BN_ER_GOOD)throw (int)iercod;
#else
#define CHECK_FORM_BNFILE_READ_GOOD \
	int iercod=m_pBnFile->READ_F(ID_M, ID_S, IST_R, LNG_R, NUM_R); \
	if(iercod != BN_ER_GOOD)throw (int)iercod;
#endif

// 데이터 없으면 없으면 스킵
#ifdef _DEBUG
#define CHECK_FORM_BNFILE_READ_SKIP \
	int iercod=m_pBnFile->READ_F(ID_M, ID_S, IST_R, LNG_R, NUM_R); \
	if(iercod == BN_ER_NONE)return TRUE; \
	if(iercod != BN_ER_GOOD)GSaveHistoryFormatNF(_T("iercod = %d"), iercod); \
	if(iercod != BN_ER_GOOD)throw (int)iercod;
#else
#define CHECK_FORM_BNFILE_READ_SKIP \
	int iercod=m_pBnFile->READ_F(ID_M, ID_S, IST_R, LNG_R, NUM_R); \
	if(iercod == BN_ER_NONE)return TRUE; \
	if(iercod != BN_ER_GOOD)throw (int)iercod;
#endif

// 레코드 개수와 크기가 모두 맞아야 함
#ifdef _DEBUG
#define CHECK_FORM_BNFILE_EQ(NUM_RECORD, STRUCT_NAME) \
	CHECK_FORM_BNFILE_READ_GOOD \
	if(NUM_R != NUM_RECORD)GSaveHistoryFormatNF(_T("NUM_R = %d, NUM_RECORD=%d"), NUM_R, NUM_RECORD); \
	if(LNG_R*4 != sizeof(STRUCT_NAME))GSaveHistoryFormatNF(_T("LNG_R*4 = %d, sizeof(STRUCT_NAME) = %d"), LNG_R*4, sizeof(STRUCT_NAME)); \
	if(NUM_R != NUM_RECORD)throw (int)BN_ER_FORM; \
	if(LNG_R*4 != sizeof(STRUCT_NAME))throw (int)BN_ER_FORM;
#else
#define CHECK_FORM_BNFILE_EQ(NUM_RECORD, STRUCT_NAME) \
	CHECK_FORM_BNFILE_READ_GOOD \
	if(NUM_R != NUM_RECORD)throw (int)BN_ER_FORM; \
	if(LNG_R*4 != sizeof(STRUCT_NAME))throw (int)BN_ER_FORM;
#endif

// 레코드 개수는 같거나 적은것만 허용, 크기는 맞아야 함
#ifdef _DEBUG
#define CHECK_FORM_BNFILE_LE(NUM_RECORD, STRUCT_NAME) \
	CHECK_FORM_BNFILE_READ_GOOD \
	if(NUM_R > NUM_RECORD)GSaveHistoryFormatNF(_T("NUM_R = %d, NUM_RECORD=%d"), NUM_R, NUM_RECORD); \
	if(LNG_R*4 != sizeof(STRUCT_NAME))GSaveHistoryFormatNF(_T("LNG_R*4 = %d, sizeof(STRUCT_NAME)=%d"), LNG_R*4, sizeof(STRUCT_NAME)); \
	if(NUM_R > NUM_RECORD)throw (int)BN_ER_FORM; \
	if(LNG_R*4 != sizeof(STRUCT_NAME))throw (int)BN_ER_FORM;
#else
#define CHECK_FORM_BNFILE_LE(NUM_RECORD, STRUCT_NAME) \
	CHECK_FORM_BNFILE_READ_GOOD \
	if(NUM_R > NUM_RECORD)throw (int)BN_ER_FORM; \
	if(LNG_R*4 != sizeof(STRUCT_NAME))throw (int)BN_ER_FORM;
#endif

// 레코드 개수는 무시, 크기는 맞아야 함
#ifdef _DEBUG
#define CHECK_FORM_BNFILE_SKIP(STRUCT_NAME) \
	CHECK_FORM_BNFILE_READ_SKIP \
	if(LNG_R*4 != sizeof(STRUCT_NAME))GSaveHistoryFormatNF(_T("LNG_R*4 = %d, sizeof(STRUCT_NAME)=%d"), LNG_R*4, sizeof(STRUCT_NAME)); \
	if(LNG_R*4 != sizeof(STRUCT_NAME))throw (int)BN_ER_FORM;
#else
#define CHECK_FORM_BNFILE_SKIP(STRUCT_NAME) \
	CHECK_FORM_BNFILE_READ_SKIP \
	if(LNG_R*4 != sizeof(STRUCT_NAME))throw (int)BN_ER_FORM;
#endif

// 레코드 개수는맞아야 함, 크기는 무시
#ifdef _DEBUG
#define CHECK_FORM_BNFILE_EQ_COUNT(NUM_RECORD) \
	CHECK_FORM_BNFILE_READ_GOOD \
	if(NUM_R != NUM_RECORD)GSaveHistoryFormatNF(_T("NUM_R = %d, NUM_RECORD=%d"), NUM_R, NUM_RECORD); \
	if(NUM_R != NUM_RECORD)throw (int)BN_ER_FORM;
#else
#define CHECK_FORM_BNFILE_EQ_COUNT(NUM_RECORD) \
	CHECK_FORM_BNFILE_READ_GOOD \
	if(NUM_R != NUM_RECORD)throw (int)BN_ER_FORM;
#endif

#ifdef _DEBUG
#define CHECK_FORM_READ(STRUCT_NAME) \
	STRUCT_NAME TData; \
	unsigned int nByte; \
	nByte=m_pFile->Read(&TData, sizeof(TData)); \
	if(nByte != sizeof(TData))GSaveHistoryFormatNF(_T("nByte = %d, sizeof(STRUCT_NAME)=%d"), nByte, sizeof(TData)); \
	if(nByte != sizeof(TData))throw (int)BN_ER_READ;
#else
#define CHECK_FORM_READ(STRUCT_NAME) \
	STRUCT_NAME TData; \
	unsigned int nByte; \
	nByte=m_pFile->Read(&TData, sizeof(TData)); \
	if(nByte != sizeof(TData))throw (int)BN_ER_READ;
#endif

#ifdef _DEBUG
#define CHECK_FORM_READ_SEEK(STRUCT_NAME) \
	STRUCT_NAME TData; \
	unsigned int nByte; \
	auto lOffset = sizeof(TData)*(NUM_R-1); \
	auto lActual = Seek(m_pFile, lOffset, CFile::current ); \
	nByte=m_pFile->Read(&TData, sizeof(TData)); \
	if(nByte != sizeof(TData))GSaveHistoryFormatNF(_T("nByte = %d, sizeof(STRUCT_NAME)=%d"), nByte, sizeof(TData)); \
	if(nByte != sizeof(TData))throw (int)BN_ER_READ;
#else
#define CHECK_FORM_READ_SEEK(STRUCT_NAME) \
	STRUCT_NAME TData; \
	unsigned int nByte; \
	auto lOffset = sizeof(TData)*(NUM_R-1); \
	auto lActual = Seek(m_pFile, lOffset, CFile::current ); \
	nByte=m_pFile->Read(&TData, sizeof(TData)); \
	if(nByte != sizeof(TData))throw (int)BN_ER_READ;
#endif

#ifdef _DEBUG
#define CHECK_FORM_READ_SEEK_END() \
	BYTE TData; \
	unsigned int nByte; \
	auto lOffset = sizeof(TData)*NUM_R-1; \
	auto lActual = Seek(m_pFile, lOffset, CFile::current ); \
	nByte=m_pFile->Read(&TData, sizeof(TData)); \
	if(nByte != sizeof(TData))GSaveHistoryFormatNF(_T("nByte = %d, sizeof(BYTE)=%d"), nByte, sizeof(TData)); \
	if(nByte != sizeof(TData))throw (int)BN_ER_READ;
#else
#define CHECK_FORM_READ_SEEK_END() \
	BYTE TData; \
	unsigned int nByte; \
	auto lOffset = sizeof(TData)*NUM_R-1; \
	auto lActual = Seek(m_pFile, lOffset, CFile::current ); \
	nByte=m_pFile->Read(&TData, sizeof(TData)); \
	if(nByte != sizeof(TData))throw (int)BN_ER_READ;
#endif

#define CHECK_FORM_SETRDFP(FILEPOINT_ID) \
	m_pRdfp->SetRdfp(FILEPOINT_ID, 0, IST_R, LNG_R*CBnFile::m_MULT_LNG_R, NUM_R);
#define CHECK_FORM_SETRDFP_SUBID(FILEPOINT_ID, SUBID) \
	m_pRdfp->SetRdfp(FILEPOINT_ID, SUBID, IST_R, LNG_R*CBnFile::m_MULT_LNG_R, NUM_R);

// 데이터 개수, 크기가 맞아야 함
#define CHECK_FORM_GENERAL_EQ(NUM_RECORD, STRUCT_NAME, FILEPOINT_ID) \
	CHECK_FORM_TRY \
	CHECK_FORM_BNFILE_EQ(NUM_RECORD, STRUCT_NAME) \
	CHECK_FORM_READ_SEEK(STRUCT_NAME) \
	CHECK_FORM_SETRDFP(FILEPOINT_ID) \
	CHECK_FORM_CATCH
// 데이터 개수가 같거나 적으면 허용, 크기가 맞아야 함
#define CHECK_FORM_GENERAL_LE(NUM_RECORD, STRUCT_NAME, FILEPOINT_ID) \
	CHECK_FORM_TRY \
	CHECK_FORM_BNFILE_LE(NUM_RECORD, STRUCT_NAME) \
	CHECK_FORM_READ_SEEK(STRUCT_NAME) \
	CHECK_FORM_SETRDFP(FILEPOINT_ID) \
	CHECK_FORM_CATCH
// 데이터가 없을수도 있는 경우, 크기가 맞아야 함
#define CHECK_FORM_GENERAL_SKIP(STRUCT_NAME, FILEPOINT_ID) \
	CHECK_FORM_TRY \
	CHECK_FORM_BNFILE_SKIP(STRUCT_NAME) \
	CHECK_FORM_READ_SEEK(STRUCT_NAME) \
	CHECK_FORM_SETRDFP(FILEPOINT_ID) \
	CHECK_FORM_CATCH
// 크기는 모르고 데이터 개수가 맞아야 함
#define CHECK_FORM_GENERAL_EQ_COUNT(NUM_RECORD, FILEPOINT_ID) \
	CHECK_FORM_TRY \
	CHECK_FORM_BNFILE_EQ_COUNT(NUM_RECORD) \
	CHECK_FORM_READ_SEEK_END() \
	CHECK_FORM_SETRDFP(FILEPOINT_ID) \
	CHECK_FORM_CATCH



CVerifyDgnResultFile::CVerifyDgnResultFile()
{
	m_pFile = NULL;
	m_pBnFile = new CBnFileResult;

	//m_pDataCount=NULL;
	m_pRdfp=NULL;
	m_bDisplayMessage=FALSE;

	m_pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
}

CVerifyDgnResultFile::~CVerifyDgnResultFile()
{
	delete m_pBnFile;
}

void CVerifyDgnResultFile::Set_DataCtrlPoint(CCRCDataCtrl* pDataCtrl)
{
	m_pDataCtrl = pDataCtrl;
}

BOOL CVerifyDgnResultFile::CheckAndGetFilePosition(int nDesignResultId, BOOL bDisplayHistory, CResultDataFilePosition* pRdfp)
{
	CWaitCursor WaitCursor;

	SetGlobalVariable(bDisplayHistory, pRdfp);

	switch(nDesignResultId)
	{
	case D_DESIGN_FORCE_RESULT:
		if(CProduct::GetTestEnvValue(_ULS(Design Output)) == _ULS(yes)) 
			SaveHistoryNF(_T("*** Check Design Force"));     
		break;

	case D_DESIGN_LCASE_FORCE_RESULT:
		if(CProduct::GetTestEnvValue(_ULS(Design Output)) == _ULS(yes)) 
			SaveHistoryNF(_T("*** Check Design Force by Load case"));     
		break;

	case D_DESIGN_LOAD_CASE_FORCE:
		if(CProduct::GetTestEnvValue(_ULS(Design Output)) == _ULS(yes)) 
			SaveHistoryNF(_T("*** Check Design Force by Load case"));     
		break;

	case D_DESIGN_LOAD_CASE_PART_FORCE:
		if(CProduct::GetTestEnvValue(_ULS(Design Output)) == _ULS(yes)) 
			SaveHistoryNF(_T("*** Check Design Part Force by Load case"));     
		break;

	default:
		ASSERT(FALSE);
		return FALSE;
	}

	CFile ResultFile;
	CFileException ex;
	TRY
	{
		CString strPathName=CDgnResultFileCtrl::GetResultFileName(nDesignResultId);
		CFileCtrl FileCtrl(strPathName);
		if(!FileCtrl.FileExists())goto END;
		
		if(!ResultFile.Open(strPathName, CFile::modeRead|CFile::shareDenyNone, &ex))
			AfxThrowFileException(ex.m_cause, ex.m_lOsError, ex.m_strFileName);

		switch(nDesignResultId)
		{
		case D_DESIGN_FORCE_RESULT:
			if(CheckDesignForceResultData(&ResultFile)){ResultFile.Abort(); return TRUE;}
			break;

		case D_DESIGN_LCASE_FORCE_RESULT:
		case D_DESIGN_LOAD_CASE_FORCE:
			if(CheckDesignLCaseForceResultData(&ResultFile)){ResultFile.Abort(); return TRUE;}
			break;
		
		case D_DESIGN_LOAD_CASE_PART_FORCE:
			if (CheckDesignPartLoadCaseForceResultData(&ResultFile)){ResultFile.Abort(); return TRUE;}
			break;

		default:
			ASSERT(FALSE);
			break;
		}
	}
	CATCH( CException, e )
	{
		e->ReportError();
	}
	END_CATCH

END:;
	ClearFilePosition(nDesignResultId, pRdfp);
	ResultFile.Abort();
	return FALSE;
}

BOOL CVerifyDgnResultFile::ClearFilePosition(int nDesignResultId, CResultDataFilePosition* pRdfp)
{
	switch(nDesignResultId)
	{
	case D_DESIGN_FORCE_RESULT:
		pRdfp->DelRdfpPart(D_FP_DFS_START, D_FP_DFS_END);
		return TRUE;

	case D_DESIGN_LCASE_FORCE_RESULT:
		pRdfp->DelRdfpPart(D_FP_LCFS_START, D_FP_LCFS_END);
		return TRUE;
	 
	default:
		ASSERT(FALSE);
	}
	return FALSE;
}

void CVerifyDgnResultFile::SetGlobalVariable(BOOL bDisplayHistory, CResultDataFilePosition* pRdfp)
{
	//m_pDataCount=pDataCount;
	m_pRdfp=pRdfp;
	m_bDisplayMessage=bDisplayHistory;
	//ASSERT(m_pDataCount);
	ASSERT(m_pRdfp);
}

LONGLONG CVerifyDgnResultFile::Seek(CFile* pFile, LONGLONG lOff, UINT nFrom)
{
	LARGE_INTEGER nSeekByte; 
	nSeekByte.QuadPart = lOff;
	nSeekByte.LowPart = ::SetFilePointer((HANDLE)pFile->m_hFile, nSeekByte.LowPart, &nSeekByte.HighPart, nFrom);
	//if(nResultByte.LowPart == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR )nResultByte.QuadPart = -1;
	return nSeekByte.QuadPart;
}

void CVerifyDgnResultFile::DisplayErrorMessage(LPCTSTR lpszErrorMsg, int nErrorCode)
{
	if(CLockMgr::Get_IsRegressionTest()) return;

	//CResultFileCtrl::DisplayErrorMessage(lpszErrorMsg, nErrorCode);
	CString strTemp, strError(lpszErrorMsg);
	if(m_pBnFile) m_pBnFile->GetErrorMessage(nErrorCode, strTemp);
	
	strError=strError+_T(" ")+strTemp;
	TimedMessageBox(NULL, NULL, MB_ICONSTOP, 1000*60, strError);  // 1분 기다림
}

void CVerifyDgnResultFile::SaveHistoryNF(CString String)
{
	if(m_bDisplayMessage)GSaveHistoryNFNA(String);
}

BOOL CVerifyDgnResultFile::CheckBnFileHead(CFile* pFile)
{
	m_pFile=pFile;
	m_pBnFile->INIT_F(pFile, CBnFileResult::m_HeadName, CBnFileResult::m_FileType, CBnFileResult::m_FileVersion);

	int nError=m_pBnFile->CHKHED_F(m_RetFileVersion);
	if(nError != BN_ER_GOOD && nError != BN_ER_VRLO)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN1_ER_READ), nError);
		return FALSE;
	}
	return TRUE;
}

BOOL CVerifyDgnResultFile::CheckDesignForceResultData(CFile* pFile)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	m_iDgnCode     = m_pDataCtrl->Get_Code(); //pDoc->GetCivilCodeDgn();
//   if(m_pDataCtrl->IsAdPSCCode()) m_iChkElem = m_pPostCtrl->GetPCDesign()->GetPscElementNumber();
//   else                           m_iChkElem = m_pDataCtrl->Get_DgnElemNum();
	m_iChkElem = m_pDataCtrl->Get_DgnElemNum();

	if(!CheckBnFileHead(pFile))return FALSE;

	if(!RB_SQR0_FORCE()) return FALSE;
	if(!RB_Design_Elem()) return FALSE;
	if(!RB_Force_Lcom()) return FALSE;  
	if(!RB_Force_Lcom_Part1()) return FALSE;  
	if(!RB_Force_Lcom_Part2()) return FALSE;  
	if(!RB_Force_Lcom_ExTp()) return FALSE;  
	if(!RB_Force_Stage()) return FALSE;
	if(!RB_Shrs_Stage()) return FALSE;
	if(!RB_Force_Tnpr()) return FALSE;
	if(!RB_Force_Tnsc()) return FALSE;
	if(!RB_Stress_Add()) return FALSE;
	if(!RB_Force_LTerm()) return FALSE;
	if(!RB_Force_STerm()) return FALSE;
	if(!RB_Force_JTG_Tndn()) return FALSE;
	if(!RB_Stress_Lcom()) return FALSE;
	if(!RB_Stress_Lcom_Part1()) return FALSE;
	if(!RB_Stress_Lcom_Part2()) return FALSE;
    if(!RB_Stress_LTerm_Part2()) return FALSE;
    if(!RB_Stress_STerm_Part2()) return FALSE;
	if(!RB_Stress_Lcom_Part_ScfpCr()) return FALSE;
	if(!RB_Stress_Lcom_Part_ScfpSh()) return FALSE;
	if(!RB_Stress_Lcom_Part_ScfpTe()) return FALSE;
	if(!RB_Stress_Lcom_Part_ScfpRe()) return FALSE;
	if(!RB_Force_Lcom_ScfpSum()) return FALSE;
	if(!RB_Stress_Tnpr()) return FALSE;
	if(!RB_Stress_Tnsc()) return FALSE;
	if(!RB_Stress_LTerm()) return FALSE;
	if(!RB_Stress_STerm()) return FALSE;
	if (m_pDataCtrl->IsAdPSCCode())
	{
		if(!RB_Stress_TnprPart2()) return FALSE;
		if(!RB_Stress_TnscPart2()) return FALSE;
	}
	if(!RB_Stss_Lcom()) return FALSE;
	if(m_iDgnCode==TB_10002_3_05 || m_iDgnCode==RC_TB_10002_3_05)
	{
		if(!RB_Force_DL_TB05()) return FALSE;
		if(!RB_Force_LL_TB05()) return FALSE;
		if(!RB_Stress_DL_TB05()) return FALSE;
		if(!RB_Stress_LL_TB05()) return FALSE;
		if(!RB_Force_CSTP_TB05()) return FALSE;
		if(!RB_Stress_CSTP_TB05()) return FALSE;
	}
	else if(m_iDgnCode==RC_JTG_B02_01_2008)
	{
		if(!RB_Force_EQE2_JTG()) return FALSE; 
		if(!RB_ForceWeight_JTG()) return FALSE;
	}
	else if(m_iDgnCode==RC_CJJ_166_2011)
	{
		if(!RB_Force_EQE2_JTG()) return FALSE; 
		if(!RB_ForceWeight_JTG()) return FALSE;
	}

	// 5 Position Expansion
	if(m_pDataCtrl->Get_ForceCtrlPointer()->Get_Write5PositionDgnForce())
	{
	  if(!RB_Force_Lcom_EX()) return FALSE;  
	  if(!RB_Force_Lcom_Part1_EX()) return FALSE;  
	  if(!RB_Force_Lcom_Part2_EX()) return FALSE;  
	  if(!RB_Force_Lcom_ExTp_EX()) return FALSE;  
	  if(!RB_Force_Stage_EX()) return FALSE;
	  if(!RB_Shrs_Stage_EX()) return FALSE;
	  if(!RB_Force_Tnpr_EX()) return FALSE;
	  if(!RB_Force_Tnsc_EX()) return FALSE;
	  if(!RB_Stress_Add_EX()) return FALSE;
	  if(!RB_Force_LTerm_EX()) return FALSE;
	  if(!RB_Force_JTG_Tndn_EX()) return FALSE;
	  if(!RB_Stress_Lcom_EX()) return FALSE;
	  if(!RB_Stress_Lcom_Part1_EX()) return FALSE;
	  if(!RB_Stress_Lcom_Part2_EX()) return FALSE;
	  if(!RB_Stress_Lcom_Part_ScfpCr_EX()) return FALSE;
	  if(!RB_Stress_Lcom_Part_ScfpSh_EX()) return FALSE;
	  if(!RB_Stress_Lcom_Part_ScfpTe_EX()) return FALSE;
	  if(!RB_Stress_Lcom_Part_ScfpRe_EX()) return FALSE;
	  if(!RB_Force_Lcom_ScfpSum_EX()) return FALSE;
	  if(!RB_Stress_Tnpr_EX()) return FALSE;
	  if(!RB_Stress_Tnsc_EX()) return FALSE;
	  if(!RB_Stress_LTerm_EX()) return FALSE;
	  if (m_pDataCtrl->IsAdPSCCode())
	  {
		  if(!RB_Stress_TnprPart2_EX()) return FALSE;
		  if(!RB_Stress_TnscPart2_EX()) return FALSE;
	  }
	  if(!RB_Stss_Lcom_EX()) return FALSE;
	  if(m_iDgnCode==TB_10002_3_05 || m_iDgnCode==RC_TB_10002_3_05)
	  {
		  if(!RB_Force_DL_TB05_EX()) return FALSE;
		  if(!RB_Force_LL_TB05_EX()) return FALSE;
		  if(!RB_Stress_DL_TB05_EX()) return FALSE;
		  if(!RB_Stress_LL_TB05_EX()) return FALSE;
		  if(!RB_Force_CSTP_TB05_EX()) return FALSE;
		  if(!RB_Stress_CSTP_TB05_EX()) return FALSE;
	  }
	  else if(m_iDgnCode==RC_JTG_B02_01_2008)
	  {
		  if(!RB_Force_EQE2_JTG_EX()) return FALSE; 
		  if(!RB_ForceWeight_JTG_EX()) return FALSE;
	  }
	  else if(m_iDgnCode==RC_CJJ_166_2011)
	  {
		  if(!RB_Force_EQE2_JTG_EX()) return FALSE; 
		  if(!RB_ForceWeight_JTG_EX()) return FALSE;
	  }
	}

	return TRUE;
}



BOOL CVerifyDgnResultFile::RB_SQR0_FORCE()
{
	memcpy(ID_M,("SQR0"),4);
	ID_S=1;
	int nNumRecord = 1;

	CHECK_FORM_TRY
	CHECK_FORM_BNFILE_EQ(nNumRecord, T_SQPG_D)
	CHECK_FORM_READ_SEEK(T_SQPG_D)
	m_SqpgD = TData;
	CHECK_FORM_SETRDFP(D_FP_DFS_SQR0) \
	CHECK_FORM_CATCH

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Design_Elem()
{
	ID_S=1;
	int nNumRecord = m_SqpgD.nElemNum;

	memcpy(ID_M,("DFEL"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, T_SEQD_D, D_FP_DFS_ELEM)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Lcom()
{  
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DFFL"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_CH, D_FP_DFS_FORCE_LCOM)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Lcom_Part1()
{  
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("DFP1"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_CH, D_FP_DFS_FORCE_LCOM_PART1)
		
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Lcom_Part2()
{  
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("DFP2"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_CH, D_FP_DFS_FORCE_LCOM_PART2)
		
		return TRUE;
}


BOOL CVerifyDgnResultFile::RB_Force_Lcom_ExTp()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DFET"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_CH, D_FP_DFS_FORCE_LCOM_EXTP)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Stage()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("DFFS"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_CH, D_FP_DFS_FORCE_STAG)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Shrs_Stage()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("DFSS"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SHRS_ADD, D_FP_DFS_SHRS_STAG)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Tnpr()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("DFTP"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_CH, D_FP_DFS_FORCE_TNPR)
	return TRUE;  
}

BOOL CVerifyDgnResultFile::RB_Force_Tnsc()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("DFTS"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_CH, D_FP_DFS_FORCE_TNSC)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_Add()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("DFSA"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_ADD, D_FP_DFS_STRESS_ADD)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_LTerm()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("DFLT"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_CH, D_FP_DFS_FORCE_LTERM)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_STerm()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DFST"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_CH, D_FP_DFS_FORCE_STERM)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_JTG_Tndn()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("DFJT"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_CH, D_FP_DFS_FORCE_JTG_TNDN)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_Lcom()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("DFSL"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_CH, D_FP_DFS_STRESS_LCOM)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_Lcom_Part1()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("DSP1"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_CH, D_FP_DFS_STRESS_LCOM_PART1)
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_Lcom_Part2()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("DSP2"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_CH, D_FP_DFS_STRESS_LCOM_PART2)
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_LTerm_Part2()
{
    ID_S=1;
    int iChkElem = m_iChkElem;
    int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
    int nNumRecord = iChkElem*iLcomNum;

    memcpy(ID_M, ("LSP2"), 4);
    CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_CH, D_FP_DFS_STRESS_LTERM_PART2)
    return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_STerm_Part2()
{
    ID_S=1;
    int iChkElem = m_iChkElem;
    int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
    int nNumRecord = iChkElem*iLcomNum;

    memcpy(ID_M, ("SSP2"), 4);
    CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_CH, D_FP_DFS_STRESS_STERM_PART2)
    return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_Lcom_Part_ScfpCr()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("SCCR"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SCFP_PART_LCASE, D_FP_DFS_STRESS_PART_SCFP_CR_LCOM)
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_Lcom_Part_ScfpSh()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("SCSH"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SCFP_PART_LCASE, D_FP_DFS_STRESS_PART_SCFP_SH_LCOM)
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_Lcom_Part_ScfpTe()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("SCTE"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SCFP_PART_LCASE, D_FP_DFS_STRESS_PART_SCFP_TE_LCOM)
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_Lcom_Part_ScfpRe()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("SCRE"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SCFP_PART_LCASE, D_FP_DFS_STRESS_PART_SCFP_RE_LCOM)
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Lcom_ScfpSum()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("SCFO"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_CH, D_FP_DFS_FORCE_SCFP_SUM_LCOM)
	return TRUE;  
}

BOOL CVerifyDgnResultFile::RB_Stress_Tnpr()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("DSTP"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_CH, D_FP_DFS_STRESS_TNPR)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_Tnsc()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("DSTS"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_CH, D_FP_DFS_STRESS_TNSC)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_LTerm()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("DSLT"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_CH, D_FP_DFS_STRESS_LTERM)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_STerm()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DSST"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_CH, D_FP_DFS_STRESS_STERM)
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_TnprPart2()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("STP2"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_CH, D_FP_DFS_STRESS_TNPR_PART2)
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_TnscPart2()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("STS2"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_CH, D_FP_DFS_STRESS_TNSC_PART2)
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stss_Lcom()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("DSSL"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SHRS_ADD, D_FP_DFS_STSS_LCOM)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_DL_TB05()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("FDTB"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_CH, D_FP_DFS_FORCE_DL_TB05)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_LL_TB05()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("FLTB"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_CH, D_FP_DFS_FORCE_LL_TB05)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_DL_TB05()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("SDTB"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_CH, D_FP_DFS_STRESS_DL_TB05)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_LL_TB05()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("SLTB"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_CH, D_FP_DFS_STRESS_LL_TB05)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_CSTP_TB05()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("FCTP"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_CH, D_FP_DFS_FORCE_CSTP_TB05)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_CSTP_TB05()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("SCTP"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_CH, D_FP_DFS_STRESS_CSTP_TB05)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_EQE2_JTG()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("FEJT"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_CH, D_FP_DFS_FORCE_EQE2_JTG)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_ForceWeight_JTG()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;
	
	memcpy(ID_M,("FWJT"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_CH, D_FP_DFS_FORCEWEIGHT_JTG)
		return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Expansion Force & Stress Position : 2 Positions -> 5 Positions
//////////////////////////////////////////////////////////////////////////

BOOL CVerifyDgnResultFile::RB_Force_Lcom_EX()
{  
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DFFX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_EX_CH, D_FP_DFS_FORCE_LCOM_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Lcom_Part1_EX()
{  
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DFX1"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_EX_CH, D_FP_DFS_FORCE_LCOM_PART1_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Lcom_Part2_EX()
{  
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DFX2"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_EX_CH, D_FP_DFS_FORCE_LCOM_PART2_EX)

	return TRUE;
}


BOOL CVerifyDgnResultFile::RB_Force_Lcom_ExTp_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DFEX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_EX_CH, D_FP_DFS_FORCE_LCOM_EXTP_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Stage_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DSFX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_EX_CH, D_FP_DFS_FORCE_STAG_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Shrs_Stage_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DFSX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SHRS_ADD_EX, D_FP_DFS_SHRS_STAG_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Tnpr_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DFTX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_EX_CH, D_FP_DFS_FORCE_TNPR_EX)
	return TRUE;  
}

BOOL CVerifyDgnResultFile::RB_Force_Tnsc_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DFTE"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_EX_CH, D_FP_DFS_FORCE_TNSC_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_Add_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DASX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_ADD_EX, D_FP_DFS_STRESS_ADD_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_LTerm_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DFLX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_EX_CH, D_FP_DFS_FORCE_LTERM_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_JTG_Tndn_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DFJX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_EX_CH, D_FP_DFS_FORCE_JTG_TNDN_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_Lcom_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DFSE"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_EX_CH, D_FP_DFS_STRESS_LCOM_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_Lcom_Part1_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DSX1"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_EX_CH, D_FP_DFS_STRESS_LCOM_PART1_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_Lcom_Part2_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DSX2"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_EX_CH, D_FP_DFS_STRESS_LCOM_PART2_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_Lcom_Part_ScfpCr_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("SCCX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SCFP_PART_LCASE_EX, D_FP_DFS_STRESS_PART_SCFP_CR_LCOM_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_Lcom_Part_ScfpSh_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("SCSX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SCFP_PART_LCASE_EX, D_FP_DFS_STRESS_PART_SCFP_SH_LCOM_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_Lcom_Part_ScfpTe_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("SCTX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SCFP_PART_LCASE_EX, D_FP_DFS_STRESS_PART_SCFP_TE_LCOM_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_Lcom_Part_ScfpRe_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("SCRX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SCFP_PART_LCASE_EX, D_FP_DFS_STRESS_PART_SCFP_RE_LCOM_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Lcom_ScfpSum_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("SCXO"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_EX_CH, D_FP_DFS_FORCE_SCFP_SUM_LCOM_EX)
	return TRUE;  
}

BOOL CVerifyDgnResultFile::RB_Stress_Tnpr_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DSTX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_EX_CH, D_FP_DFS_STRESS_TNPR_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_Tnsc_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DSTE"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_EX_CH, D_FP_DFS_STRESS_TNSC_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_LTerm_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DSLX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_EX_CH, D_FP_DFS_STRESS_LTERM_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_TnprPart2_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("STPX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_EX_CH, D_FP_DFS_STRESS_TNPR_PART2_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_TnscPart2_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("STSX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_EX_CH, D_FP_DFS_STRESS_TNSC_PART2_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stss_Lcom_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("DSSX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SHRS_ADD_EX, D_FP_DFS_STSS_LCOM_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_DL_TB05_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("FDTX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_EX_CH, D_FP_DFS_FORCE_DL_TB05_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_LL_TB05_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("FLTX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_EX_CH, D_FP_DFS_FORCE_LL_TB05_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_DL_TB05_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("SDTX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_EX_CH, D_FP_DFS_STRESS_DL_TB05_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_LL_TB05_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("SLTX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_EX_CH, D_FP_DFS_STRESS_LL_TB05_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_CSTP_TB05_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("FCTX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_EX_CH, D_FP_DFS_FORCE_CSTP_TB05_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Stress_CSTP_TB05_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("SSTX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRS_CRC_EX_CH, D_FP_DFS_STRESS_CSTP_TB05_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_EQE2_JTG_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("FEJX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_EX_CH, D_FP_DFS_FORCE_EQE2_JTG_EX)
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_ForceWeight_JTG_EX()
{
	ID_S=1;
	int iChkElem = m_iChkElem;  
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	int nNumRecord = iChkElem*iLcomNum;

	memcpy(ID_M,("FWJX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_FORC_CRC_EX_CH, D_FP_DFS_FORCEWEIGHT_JTG_EX)
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

//++++++
BOOL CVerifyDgnResultFile::CheckDesignLCaseForceResultData(CFile* pFile)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	m_iDgnCode     = m_pDataCtrl->Get_Code();
	m_iChkElem = m_pDataCtrl->Get_DgnElemNum();

	if(!CheckBnFileHead(pFile))return FALSE;

	if(!RB_SQLC_Lcase_Force()) return FALSE;
	if(!RB_Design_Lcase_Elem()) return FALSE;
//   if(!RB_Force_Lcase_Stld()) return FALSE;
//   if(!RB_Force_Lcase_Splc()) return FALSE;
//   if(!RB_Force_Lcase_Espc()) return FALSE;
//   if(!RB_Force_Lcase_This()) return FALSE;
//   if(!RB_Force_Lcase_Sgld()) return FALSE;
//   //if(!RB_Force_Lcase_Step()) return FALSE;
//   if(!RB_Force_Lcase_Mvld()) return FALSE;
//   if(!RB_Force_Lcase_Smlc()) return FALSE;
//   if(!RB_Force_Lcase_GilcStrb()) return FALSE;
//   if(!RB_Force_Lcase_GilcSbcf()) return FALSE;
//   if(!RB_Force_Lcase_LcomStrb()) return FALSE;
//   if(!RB_Force_Lcase_LcomSbcf()) return FALSE;

	if(!RB_LoadCase_Stld()) return FALSE;
	if(!RB_LoadCase_Splc()) return FALSE;
	if(!RB_LoadCase_Espc()) return FALSE;
	if(!RB_LoadCase_This()) return FALSE;
	if(!RB_LoadCase_Sgld()) return FALSE;
	if(!RB_LoadCase_Mvld()) return FALSE;
    if(!RB_LoadCase_MvldBS()) return FALSE; // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
	if(!RB_LoadCase_Smlc()) return FALSE;
	if(!RB_LoadCase_GilcStrb()) return FALSE;
	if(!RB_LoadCase_GilcSbcf()) return FALSE;
	if(!RB_LoadCase_LcomStrb()) return FALSE;
	if(!RB_LoadCase_LcomStrbMin()) return FALSE;
	if(!RB_LoadCase_LcomSbcf()) return FALSE;


	// Expansion Force : 2 Position -> 5 Position for each Element

	if(m_pDataCtrl->Get_ForceCtrlPointer()->Get_Write5PositionDgnForce())
	{
	  if(!RB_LoadCase_Stld_EX()) return FALSE;
	  if(!RB_LoadCase_Splc_EX()) return FALSE;
	  if(!RB_LoadCase_Espc_EX()) return FALSE;
	  if(!RB_LoadCase_This_EX()) return FALSE;
	  if(!RB_LoadCase_Sgld_EX()) return FALSE;
	  //if(!RB_LoadCase_Step_EX()) return FALSE;    //STEP Binary File Error 발생
	  if(!RB_LoadCase_Mvld_EX()) return FALSE;
      if(!RB_LoadCase_MvldBS_EX()) return FALSE;
	  if(!RB_LoadCase_Smlc_EX()) return FALSE;
	  if(!RB_LoadCase_GilcStrb_EX()) return FALSE;
	  if(!RB_LoadCase_GilcSbcf_EX()) return FALSE;
	  if(!RB_LoadCase_LcomStrb_EX()) return FALSE;
	  if(!RB_LoadCase_LcomStrbMin_EX()) return FALSE;
	  if(!RB_LoadCase_LcomSbcf_EX()) return FALSE;  

	}

	return TRUE;
}

BOOL CVerifyDgnResultFile::CheckDesignPartLoadCaseForceResultData(CFile* pFile)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	
	m_iDgnCode     = m_pDataCtrl->Get_Code();
	m_iChkElem = m_pDataCtrl->Get_DgnElemNum();
	
	if(!CheckBnFileHead(pFile))return FALSE;
	
	if(!RB_SQLC_Lcase_Force()) return FALSE;
	if(!RB_Design_Lcase_Elem()) return FALSE;  
	if(!RB_PartLoadCase_Stld()) return FALSE;
	if(!RB_PartLoadCase_Splc()) return FALSE;
	if(!RB_PartLoadCase_Espc()) return FALSE;
	if(!RB_PartLoadCase_This()) return FALSE;
	if(!RB_PartLoadCase_Sgld()) return FALSE;
	if(!RB_PartLoadCase_Mvld()) return FALSE;
    if(!RB_PartLoadCase_MvldBS()) return FALSE; // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
	if(!RB_PartLoadCase_Smlc()) return FALSE;
	if(!RB_PartLoadCase_GilcStrb()) return FALSE;
	if(!RB_PartLoadCase_GilcSbcf()) return FALSE;
	if(!RB_PartLoadCase_LcomStrb()) return FALSE;
	if(!RB_PartLoadCase_LcomStrbMin()) return FALSE;
	if(!RB_PartLoadCase_LcomSbcf()) return FALSE;
	if(!RB_PartLoadCase_StldScfp()) return FALSE;
	if(!RB_PartLoadCase_SgldScfp()) return FALSE;

	// Expansion Part Force : 2 Position -> 5 Position for each Element
	if(m_pDataCtrl->Get_ForceCtrlPointer()->Get_Write5PositionDgnForce())
	{
	  if(!RB_PartLoadCase_Stld_EX()) return FALSE;
	  if(!RB_PartLoadCase_Splc_EX()) return FALSE;  
	  if(!RB_PartLoadCase_Espc_EX()) return FALSE;
	  if(!RB_PartLoadCase_This_EX()) return FALSE;
	  if(!RB_PartLoadCase_Sgld_EX()) return FALSE;
	  //if(!RB_PartLoadCase_Step_EX()) return FALSE;    //STEP Binary File Error 발생
	  if(!RB_PartLoadCase_Mvld_EX()) return FALSE;
      if(!RB_PartLoadCase_MvldBS_EX()) return FALSE;
	  if(!RB_PartLoadCase_Smlc_EX()) return FALSE;
	  if(!RB_PartLoadCase_GilcStrb_EX()) return FALSE;
	  if(!RB_PartLoadCase_GilcSbcf_EX()) return FALSE;
	  if(!RB_PartLoadCase_LcomStrb_EX()) return FALSE;
	  if(!RB_PartLoadCase_LcomStrbMin_EX()) return FALSE;
	  if(!RB_PartLoadCase_LcomSbcf_EX()) return FALSE;
	  if(!RB_PartLoadCase_StldScfp_EX()) return FALSE;
	  if(!RB_PartLoadCase_SgldScfp_EX()) return FALSE;  
	}
	
	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_SQLC_Lcase_Force()
{
	memcpy(ID_M,("SQLC"),4);
	ID_S=1;
	int nNumRecord = 1;

	CHECK_FORM_TRY
	CHECK_FORM_BNFILE_EQ(nNumRecord, T_SQLC_D)
	CHECK_FORM_READ_SEEK(T_SQLC_D)
	m_SqlcD = TData;
	CHECK_FORM_SETRDFP(D_FP_LCFS_SQL0) \
	CHECK_FORM_CATCH
	//  

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Design_Lcase_Elem()
{
	ID_S=1;
	int nNumRecord = m_SqlcD.nElemNum;

	memcpy(ID_M,("DLFE"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, T_SEQD_D, D_FP_LCFS_ELEM)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Lcase_Stld()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapStldK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("STLD"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, T_STLD_LCASE, D_FP_LCFS_STLD)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Lcase_Splc()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapSplcK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("SPLC"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, T_STRB_LCASE, D_FP_LCFS_SPLC)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Lcase_Espc()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapEspcK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("ESPC"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, T_STRB_LCASE, D_FP_LCFS_ESPC)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Lcase_This()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapThisK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("THIS"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, T_STRB_LCASE, D_FP_LCFS_THIS)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Lcase_Sgld()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapSgldK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("SGLD"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, T_STRB_LCASE, D_FP_LCFS_SGLD)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Lcase_Step()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapStepK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("STEP"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, T_STRB_LCASE, D_FP_LCFS_STEP)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Lcase_Mvld()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapMvldK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("MVLD"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, T_SBCF_LCASE, D_FP_LCFS_MVLD)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Lcase_Smlc()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapSmlcK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("SMLC"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, T_SBCF_LCASE, D_FP_LCFS_SMLC)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Lcase_GilcStrb()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapGilcStrbK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("GSTR"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, T_STRB_LCASE, D_FP_LCFS_GILC_STRB)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Lcase_GilcSbcf()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapGilcSbcfK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("GSBC"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, T_SBCF_LCASE, D_FP_LCFS_GILC_SBCF)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Lcase_LcomStrb()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapLcomStrbK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("LSTR"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, T_STRB_LCASE, D_FP_LCFS_LSTR)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_Force_Lcase_LcomSbcf()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapLcomSbcfK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("LSBC"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, T_SBCF_LCASE, D_FP_LCFS_LSBC)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_Stld()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapStldK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("STLD"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_LCASE_CH, D_FP_LCFS_STLD)
		
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_Splc()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapSplcK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("SPLC"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_LCASE_CH, D_FP_LCFS_SPLC)
		
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_Espc()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapEspcK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("ESPC"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_LCASE_CH, D_FP_LCFS_ESPC)
		
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_This()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapThisK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("THIS"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_LCASE_CH, D_FP_LCFS_THIS)
		
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_Sgld()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapSgldK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("SGLD"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_LCASE_CH, D_FP_LCFS_SGLD)
		
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_Step()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapStepK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("STEP"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_LCASE_CH, D_FP_LCFS_STEP)
		
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_Mvld()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapMvldK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("MVLD"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SBCFFORC_LCASE_CH, D_FP_LCFS_MVLD)
		
		return TRUE;
}

 // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
BOOL CVerifyDgnResultFile::RB_LoadCase_MvldBS()
{
    ID_S = 1;
    int iChkElem = m_iChkElem;
    int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapMvldK.GetCount();
    int nNumRecord = iChkElem*iLcaseNum;

    if (nNumRecord==0) return TRUE;

    memcpy(ID_M, ("MVBS"), 4);
    CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SBCFFORC_LCASE_CH, D_FP_LCFS_MVLD_BS_SPC)

        return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_Smlc()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapSmlcK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("SMLC"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SBCFFORC_LCASE_CH, D_FP_LCFS_SMLC)
		
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_GilcStrb()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapGilcStrbK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("GSTR"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_LCASE_CH, D_FP_LCFS_GILC_STRB)
		
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_GilcSbcf()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapGilcSbcfK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("GSBC"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SBCFFORC_LCASE_CH, D_FP_LCFS_GILC_SBCF)
		
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_LcomStrb()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapLcomStrbK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("LSTR"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_LCASE_CH, D_FP_LCFS_LSTR)
		
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_LcomStrbMin()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapLcomStrbK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("LSTM"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_LCASE_CH, D_FP_LCFS_LSTR_MIN)
		
		return TRUE;
}


BOOL CVerifyDgnResultFile::RB_LoadCase_LcomSbcf()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapLcomSbcfK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("LSBC"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SBCFFORC_LCASE_CH, D_FP_LCFS_LSBC)
		
		return TRUE;
}


BOOL CVerifyDgnResultFile::RB_PartLoadCase_Stld()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapStldK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("STLD"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_PART_LCASE_CH, D_FP_LCFS_STLD_PA)
		
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_Splc()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapSplcK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("SPLC"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_PART_LCASE_CH, D_FP_LCFS_SPLC_PA)
		
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_Espc()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapEspcK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("ESPC"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_PART_LCASE_CH, D_FP_LCFS_ESPC_PA)
		
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_This()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapThisK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("THIS"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_PART_LCASE_CH, D_FP_LCFS_THIS_PA)
		
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_Sgld()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapSgldK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("SGLD"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_PART_LCASE_CH, D_FP_LCFS_SGLD_PA)
		
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_Step()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapStepK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("STEP"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_PART_LCASE_CH, D_FP_LCFS_STEP_PA)
		
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_Mvld()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapMvldK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("MVLD"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SBCFFORC_PART_LCASE_CH, D_FP_LCFS_MVLD_PA)
		
		return TRUE;
}

 // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
BOOL CVerifyDgnResultFile::RB_PartLoadCase_MvldBS()
{
    ID_S = 1;
    int iChkElem = m_iChkElem;
    int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapMvldK.GetCount();
    int nNumRecord = iChkElem*iLcaseNum;

    if (nNumRecord==0) return TRUE;

    memcpy(ID_M, ("MVBS"), 4);
    CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SBCFFORC_PART_LCASE_CH, D_FP_LCFS_MVLD_BS_SPC_PA)

        return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_Smlc()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapSmlcK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("SMLC"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SBCFFORC_PART_LCASE_CH, D_FP_LCFS_SMLC_PA)
		
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_GilcStrb()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapGilcStrbK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("GSTR"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_PART_LCASE_CH, D_FP_LCFS_GILC_STRB_PA)
		
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_GilcSbcf()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapGilcSbcfK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("GSBC"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SBCFFORC_PART_LCASE_CH, D_FP_LCFS_GILC_SBCF_PA)
		
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_LcomStrb()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapLcomStrbK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("LSTR"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_PART_LCASE_CH, D_FP_LCFS_LSTR_PA)
		
		return TRUE;
}


BOOL CVerifyDgnResultFile::RB_PartLoadCase_LcomStrbMin()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapLcomStrbK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("LSTM"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_PART_LCASE_CH, D_FP_LCFS_LSTR_PA_MIN)

		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_LcomSbcf()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapLcomSbcfK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;
	
	if(nNumRecord==0) return TRUE;
	
	memcpy(ID_M,("LSBC"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SBCFFORC_PART_LCASE_CH, D_FP_LCFS_LSBC_PA)
		
		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_StldScfp()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapStldScfpK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("STSC"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SCFP_PART_LCASE, D_FP_LCFS_STLD_SCFP_PA)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_SgldScfp()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapSgldScfpK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("SGSC"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SCFP_PART_LCASE, D_FP_LCFS_SGLD_SCFP_PA)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_Stld_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapStldK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("STLX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_LCASE_EX_CH, D_FP_LCFS_STLD_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_Splc_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapSplcK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("SPLX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_LCASE_EX_CH, D_FP_LCFS_SPLC_EX)

	return TRUE;
}


BOOL CVerifyDgnResultFile::RB_LoadCase_Espc_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapEspcK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("ESPX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_LCASE_EX_CH, D_FP_LCFS_ESPC_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_This_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapThisK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("THIX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_LCASE_EX_CH, D_FP_LCFS_THIS_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_Sgld_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapSgldK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("SGLX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_LCASE_EX_CH, D_FP_LCFS_SGLD_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_Step_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapStepK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("STEX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_LCASE_EX_CH, D_FP_LCFS_STEP_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_Mvld_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapMvldK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("MVLX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SBCFFORC_LCASE_EX_CH, D_FP_LCFS_MVLD_EX)

	return TRUE;
}

// Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
BOOL CVerifyDgnResultFile::RB_LoadCase_MvldBS_EX()
{
    ID_S = 1;
    int iChkElem = m_iChkElem;
    int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapMvldK.GetCount();
    int nNumRecord = iChkElem*iLcaseNum;

    if (nNumRecord==0) return TRUE;

    memcpy(ID_M, ("MVBX"), 4);
    CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SBCFFORC_LCASE_EX_CH, D_FP_LCFS_MVLD_BS_SPC_EX)

    return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_Smlc_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapSmlcK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("SMLX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SBCFFORC_LCASE_EX_CH, D_FP_LCFS_SMLC_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_GilcStrb_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapGilcStrbK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("GSTX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_LCASE_EX_CH, D_FP_LCFS_GILC_STRB_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_GilcSbcf_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapGilcSbcfK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("GSBX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SBCFFORC_LCASE_EX_CH, D_FP_LCFS_GILC_SBCF_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_LcomStrb_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapLcomStrbK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("LSTX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_LCASE_EX_CH, D_FP_LCFS_LSTR_EX)

		return TRUE;
}

BOOL CVerifyDgnResultFile::RB_LoadCase_LcomStrbMin_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapLcomStrbK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("LSTE"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_LCASE_EX_CH, D_FP_LCFS_LSTR_MIN_EX)

	return TRUE;
}


BOOL CVerifyDgnResultFile::RB_LoadCase_LcomSbcf_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapLcomSbcfK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("LSBX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SBCFFORC_LCASE_EX_CH, D_FP_LCFS_LSBC_EX)

	return TRUE;
}



BOOL CVerifyDgnResultFile::RB_PartLoadCase_Stld_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapStldK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("STLX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_PART_LCASE_EX_CH, D_FP_LCFS_STLD_PA_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_Splc_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapSplcK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("SPLX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_PART_LCASE_EX_CH, D_FP_LCFS_SPLC_PA_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_Espc_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapEspcK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("ESPX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_PART_LCASE_EX_CH, D_FP_LCFS_ESPC_PA_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_This_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapThisK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("THIX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_PART_LCASE_EX_CH, D_FP_LCFS_THIS_PA_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_Sgld_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapSgldK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("SGLX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_PART_LCASE_EX_CH, D_FP_LCFS_SGLD_PA_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_Step_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapStepK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("STEX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_PART_LCASE_EX_CH, D_FP_LCFS_STEP_PA_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_Mvld_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapMvldK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("MVLX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SBCFFORC_PART_LCASE_EX_CH, D_FP_LCFS_MVLD_PA_EX)

	return TRUE;
}

// Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
BOOL CVerifyDgnResultFile::RB_PartLoadCase_MvldBS_EX()
{
    ID_S = 1;
    int iChkElem = m_iChkElem;
    int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapMvldK.GetCount();
    int nNumRecord = iChkElem*iLcaseNum;

    if (nNumRecord==0) return TRUE;

    memcpy(ID_M, ("MVBX"), 4);
    CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SBCFFORC_PART_LCASE_EX_CH, D_FP_LCFS_MVLD_BS_SPC_PA_EX)

    return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_Smlc_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapSmlcK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("SMLX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SBCFFORC_PART_LCASE_EX_CH, D_FP_LCFS_SMLC_PA_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_GilcStrb_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapGilcStrbK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("GSTX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_PART_LCASE_EX_CH, D_FP_LCFS_GILC_STRB_PA_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_GilcSbcf_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapGilcSbcfK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("GSBX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SBCFFORC_PART_LCASE_EX_CH, D_FP_LCFS_GILC_SBCF_PA_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_LcomStrb_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapLcomStrbK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("LSTX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_PART_LCASE_EX_CH, D_FP_LCFS_LSTR_PA_EX)

	return TRUE;
}


BOOL CVerifyDgnResultFile::RB_PartLoadCase_LcomStrbMin_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapLcomStrbK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("LSTE"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_STRBFORC_PART_LCASE_EX_CH, D_FP_LCFS_LSTR_PA_MIN_EX)

	return TRUE;
}

BOOL CVerifyDgnResultFile::RB_PartLoadCase_LcomSbcf_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapLcomSbcfK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("LSBX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SBCFFORC_PART_LCASE_EX_CH, D_FP_LCFS_LSBC_PA_EX)

	return TRUE;
}


BOOL CVerifyDgnResultFile::RB_PartLoadCase_StldScfp_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapStldScfpK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("STSX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SCFP_PART_LCASE_EX, D_FP_LCFS_STLD_SCFP_PA_EX)

	return TRUE;
}


BOOL CVerifyDgnResultFile::RB_PartLoadCase_SgldScfp_EX()
{  
	ID_S=1;
	int iChkElem  = m_iChkElem;
	int iLcaseNum = m_pDataCtrl->Get_ForceCtrlPointer()->m_arMapSgldScfpK.GetCount();
	int nNumRecord = iChkElem*iLcaseNum;

	if(nNumRecord==0) return TRUE;

	memcpy(ID_M,("SGSX"),4);
	CHECK_FORM_GENERAL_EQ(nNumRecord, _DGN_SCFP_PART_LCASE_EX, D_FP_LCFS_SGLD_SCFP_PA_EX)

	return TRUE;
}
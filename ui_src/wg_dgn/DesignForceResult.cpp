// DesignForceResult.cpp: implementation of the CDesignForceResult class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DesignForceResult.h"
#include "..\wg_db\BnFile.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\ResultDataFilePosition.h"
#include "..\wg_base\wg_base_UtilFunc.h"

//#include "DgnResultDataFilePosition.h"
#include "DgnResultFileCtrl.h"


#define CHECK_FORM_TRY \
	try \
	{
#define CHECK_FORM_CATCH \
	} \
	catch(int iercod) \
	{ \
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod); \
		return FALSE; \
	}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDesignForceResult::CDesignForceResult()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	m_pResultFileCtrl = new CDgnResultFileCtrl;
	//m_pSeqIdInfo=NULL;
	//m_pDataCount=NULL;
	m_pRdfp=pDoc->m_pPostCtrl->GetPointRdfp();
	m_pPostCtrl = pDoc->m_pPostCtrl;
}

CDesignForceResult::CDesignForceResult(CCRCDataCtrl* pDataCtrl)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	m_pDataCtrl = pDataCtrl;
	//
	m_pForceCtrl = m_pDataCtrl->Get_ForceCtrlPointer();

	m_pResultFileCtrl = new CDgnResultFileCtrl;
	//m_pSeqIdInfo=NULL;
	//m_pDataCount=NULL;
	m_pRdfp=pDoc->m_pPostCtrl->GetPointRdfp();
	m_pPostCtrl = pDoc->m_pPostCtrl;
}

CDesignForceResult::~CDesignForceResult()
{
	if(m_pResultFileCtrl) delete m_pResultFileCtrl;
}

void CDesignForceResult::Initialize(CCRCDataCtrl* pDataCtrl)
{
	m_pDataCtrl = pDataCtrl;
}

BOOL CDesignForceResult::GetForceLcomResult(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_LCOM, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial = m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		//if(m_pDataCtrl->IsAdPSCCode()) KeySerial=m_pPostCtrl->GetSerialElemConcrete(ElemK);
		//else                           KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		
		_DGN_FORC_CRC_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceLcomResult_Part1(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_LCOM_PART1, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);;
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;
		
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		
		_DGN_FORC_CRC_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceLcomResult_Part2(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_LCOM_PART2, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);;
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;
		
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		
		_DGN_FORC_CRC_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetForceLcomExTpResult(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force)
{  
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_LCOM_EXTP, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);;
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		
		_DGN_FORC_CRC_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceStageResult(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_STAG, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		
		_DGN_FORC_CRC_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetShrsStageResult(ElemPairK ElemK, int iSerialLcom, _DGN_SHRS_ADD& ShrsAdd)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	ShrsAdd.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_SHRS_STAG, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		pFile->Read(&ShrsAdd, sizeof(ShrsAdd));        
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceTnprResult(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_TNPR, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		
		_DGN_FORC_CRC_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceTnscResult(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_TNSC, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		
		_DGN_FORC_CRC_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressAddResult(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_ADD& StrsAdd)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	StrsAdd.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_ADD, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		pFile->Read(&StrsAdd, sizeof(StrsAdd));        
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceLTermResult(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_LTERM, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		
		_DGN_FORC_CRC_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceSTermResult(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force)
{
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_STERM, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_FORC_CRC_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceJtgTndnResult(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_JTG_TNDN, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		
		_DGN_FORC_CRC_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressLcomResult(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_LCOM, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		
		_DGN_STRS_CRC_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressLcomResult_Part1(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_LCOM_PART1, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;
		
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		
		_DGN_STRS_CRC_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetStressLcomResult_Part2(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_LCOM_PART2, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;
		
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		
		_DGN_STRS_CRC_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetStressLcomLTerm_Part2(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress)
{
    Stress.Initialize();
    CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
    LONGLONG nStartRecord, nRecordLength;
    if ( !m_pRdfp->GetRdfp(D_FP_DFS_STRESS_LTERM_PART2, 0, nStartRecord, nRecordLength) )return FALSE;

    try
    {
        T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
        UINT nSerialDgnLcom = iSerialLcom;
        UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
        LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

        CBnFile::SeekBN(pFile, nRecord, nRecordLength);

        _DGN_STRS_CRC_CH StressCH;
        pFile->Read(&StressCH, sizeof(StressCH));
        StressCH.ConvertToString(Stress);
    }
    catch ( int iercod )
    {
        DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
        return FALSE;
    }

    return TRUE;
}

BOOL CDesignForceResult::GetStressLcomSTerm_Part2(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress)
{
    Stress.Initialize();
    CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
    LONGLONG nStartRecord, nRecordLength;
    if ( !m_pRdfp->GetRdfp(D_FP_DFS_STRESS_STERM_PART2, 0, nStartRecord, nRecordLength) )return FALSE;

    try
    {
        T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
        UINT nSerialDgnLcom = iSerialLcom;
        UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
        LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

        CBnFile::SeekBN(pFile, nRecord, nRecordLength);

        _DGN_STRS_CRC_CH StressCH;
        pFile->Read(&StressCH, sizeof(StressCH));
        StressCH.ConvertToString(Stress);
    }
    catch ( int iercod )
    {
        DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
        return FALSE;
    }

    return TRUE;
}


BOOL CDesignForceResult::GetStressLcomResult_PartScfpCr(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE& Stress)
{
	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_PART_SCFP_CR_LCOM, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		pFile->Read(&Stress, sizeof(Stress));
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressLcomResult_PartScfpSh(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE& Stress)
{
	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_PART_SCFP_SH_LCOM, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		pFile->Read(&Stress, sizeof(Stress));
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressLcomResult_PartScfpTe(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE& Stress)
{
	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_PART_SCFP_TE_LCOM, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		pFile->Read(&Stress, sizeof(Stress));
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressLcomResult_PartScfpRe(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE& Stress)
{
	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_PART_SCFP_RE_LCOM, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		pFile->Read(&Stress, sizeof(Stress));
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceLcomResult_ScfpSum(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force)
{
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_SCFP_SUM_LCOM, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_FORC_CRC_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceLcomResult_ScfpCrp(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force)
{
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_SCFP_CRP_LCOM, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_FORC_CRC_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceLcomResult_ScfpShr(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force)
{
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_SCFP_SHR_LCOM, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_FORC_CRC_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressTnprResult(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_TNPR, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_STRS_CRC_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressTnscResult(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_TNSC, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_STRS_CRC_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressLTermResult(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_LTERM, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_STRS_CRC_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressSTermResult(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_STERM, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_STRS_CRC_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressTnprPart2Result(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_TNPR_PART2, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_STRS_CRC_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressTnscPart2Result(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_TNSC_PART2, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_STRS_CRC_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStssLcomResult(ElemPairK ElemK, int iSerialLcom, _DGN_SHRS_ADD& ShrsAdd)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	ShrsAdd.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STSS_LCOM, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		pFile->Read(&ShrsAdd, sizeof(ShrsAdd));        
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

void CDesignForceResult::DisplayErrorMessage(LPCTSTR lpszErrorMsg, int nErrorCode)
{
	CDgnResultFileCtrl::DisplayErrorMessage(lpszErrorMsg, nErrorCode);
}

BOOL CDesignForceResult::GetForceDL_TB05Result(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_DL_TB05, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		
		_DGN_FORC_CRC_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceLL_TB05Result(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_LL_TB05, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		
		_DGN_FORC_CRC_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressDL_TB05Result(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_DL_TB05, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_STRS_CRC_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressLL_TB05Result(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_LL_TB05, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_STRS_CRC_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceCSTP_TB05Result(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force)
{
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_CSTP_TB05, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		
		_DGN_FORC_CRC_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressCSTP_TB05Result(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress)
{
	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_CSTP_TB05, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_STRS_CRC_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceEQE2_JTGResult(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force)
{	
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_EQE2_JTG, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		
		_DGN_FORC_CRC_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceWeight_JTGResult(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force)
{	
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;
	
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCEWEIGHT_JTG, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;
		
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		
		_DGN_FORC_CRC_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Expansion Force & Stress Position : 2 Positions -> 5 Positions
//////////////////////////////////////////////////////////////////////////

BOOL CDesignForceResult::GetForceLcomResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_LCOM_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_FORC_CRC_EX_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceLcomResult_Part1_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_LCOM_PART1_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_FORC_CRC_EX_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceLcomResult_Part2_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_LCOM_PART2_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_FORC_CRC_EX_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceLcomExTpResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force)
{  
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_LCOM_EXTP_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_FORC_CRC_EX_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceStageResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_STAG_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_FORC_CRC_EX_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetShrsStageResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_SHRS_ADD_EX& ShrsAdd)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	ShrsAdd.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_SHRS_STAG_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		pFile->Read(&ShrsAdd, sizeof(ShrsAdd));        
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceTnprResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_TNPR_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_FORC_CRC_EX_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceTnscResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_TNSC_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_FORC_CRC_EX_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressAddResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_ADD_EX& StrsAdd)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	StrsAdd.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_ADD_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		pFile->Read(&StrsAdd, sizeof(StrsAdd));        
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceLTermResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_LTERM_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_FORC_CRC_EX_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceJtgTndnResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_JTG_TNDN_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_FORC_CRC_EX_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressLcomResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_LCOM_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_STRS_CRC_EX_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressLcomResult_Part1_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_LCOM_PART1_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_STRS_CRC_EX_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressLcomResult_Part2_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_LCOM_PART2_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_STRS_CRC_EX_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressLcomResult_PartScfpCr_EX(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE_EX& Stress)
{
	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_PART_SCFP_CR_LCOM_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		pFile->Read(&Stress, sizeof(Stress));
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressLcomResult_PartScfpSh_EX(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE_EX& Stress)
{
	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_PART_SCFP_SH_LCOM_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		pFile->Read(&Stress, sizeof(Stress));
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressLcomResult_PartScfpTe_EX(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE_EX& Stress)
{
	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_PART_SCFP_TE_LCOM_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		pFile->Read(&Stress, sizeof(Stress));
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressLcomResult_PartScfpRe_EX(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE_EX& Stress)
{
	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_PART_SCFP_RE_LCOM_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		pFile->Read(&Stress, sizeof(Stress));
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceLcomResult_ScfpSum_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force)
{
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_SCFP_SUM_LCOM_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_FORC_CRC_EX_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceLcomResult_ScfpCrp_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force)
{
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_SCFP_CRP_LCOM_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_FORC_CRC_EX_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceLcomResult_ScfpShr_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force)
{
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_SCFP_SHR_LCOM_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_FORC_CRC_EX_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressTnprResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_TNPR_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_STRS_CRC_EX_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressTnscResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_TNSC_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_STRS_CRC_EX_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressLTermResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_LTERM_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_STRS_CRC_EX_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressTnprPart2Result_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_TNPR_PART2_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_STRS_CRC_EX_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressTnscPart2Result_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_TNSC_PART2_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_STRS_CRC_EX_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStssLcomResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_SHRS_ADD_EX& ShrsAdd)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	ShrsAdd.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STSS_LCOM_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		pFile->Read(&ShrsAdd, sizeof(ShrsAdd));        
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceDL_TB05Result_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_DL_TB05_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_FORC_CRC_EX_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceLL_TB05Result_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_LL_TB05_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_FORC_CRC_EX_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressDL_TB05Result_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_DL_TB05_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_STRS_CRC_EX_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressLL_TB05Result_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress)
{
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_LL_TB05_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_STRS_CRC_EX_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceCSTP_TB05Result_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force)
{
	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_CSTP_TB05_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_FORC_CRC_EX_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetStressCSTP_TB05Result_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress)
{
	Stress.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_STRESS_CSTP_TB05_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_STRS_CRC_EX_CH StressCH;
		pFile->Read(&StressCH, sizeof(StressCH));
		StressCH.ConvertToString(Stress);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceEQE2_JTGResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force)
{	
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCE_EQE2_JTG_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_FORC_CRC_EX_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetForceWeight_JTGResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force)
{	
	//CPostCtrl* pPostCtrl = CDBDoc::GetDocPoint()->m_pPostCtrl;

	Force.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_DFS_FORCEWEIGHT_JTG_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialDgnLcom = iSerialLcom;    
		UINT nNumDgnLcom = m_pDataCtrl->Get_DgnLcomNum();
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nNumDgnLcom+nSerialDgnLcom-1;

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);

		_DGN_FORC_CRC_EX_CH ForceCH;
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(Force);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// Load Case Unit
//////////////////////////////////////////////////////////////////////////

BOOL CDesignForceResult::GetLcaseForceStld(ElemPairK ElemK, T_STLD_K StldK, T_STLD_LCASE& StldLcase)
{   
	StldLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LCASE_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_STLD, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapStldK.Lookup(StldK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapStldK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		pFile->Read(&StldLcase, sizeof(StldLcase));    
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLcaseForceSplc(ElemPairK ElemK, T_SPLC_K SplcK, T_STRB_LCASE& StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LCASE_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_SPLC, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapSplcK.Lookup(SplcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapSplcK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		pFile->Read(&StrbLcase, sizeof(StrbLcase));    
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLcaseForceEspc(ElemPairK ElemK, T_ESPC_K EspcK, T_STRB_LCASE& StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LCASE_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_ESPC, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapEspcK.Lookup(EspcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapEspcK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		pFile->Read(&StrbLcase, sizeof(StrbLcase));    
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLcaseForceThis(ElemPairK ElemK, T_THIS_K ThisK, T_STRB_LCASE& StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LCASE_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_THIS, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapThisK.Lookup(ThisK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapThisK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		pFile->Read(&StrbLcase, sizeof(StrbLcase));    
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLcaseForceSgld(ElemPairK ElemK, T_SGLD_K SgldK, T_STRB_LCASE& StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LCASE_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_SGLD, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapSgldK.Lookup(SgldK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapSgldK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		pFile->Read(&StrbLcase, sizeof(StrbLcase));    
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLcaseForceStep(ElemPairK ElemK, T_STEP_K StepK, T_STRB_LCASE& StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LCASE_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_STEP, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapStepK.Lookup(StepK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapStepK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		pFile->Read(&StrbLcase, sizeof(StrbLcase));    
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLcaseForceMvld(ElemPairK ElemK, T_MVLD_K MvldK, T_SBCF_LCASE& SbcfLcase)
{   
	SbcfLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LCASE_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_MVLD, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapMvldK.Lookup(MvldK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapMvldK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		pFile->Read(&SbcfLcase, sizeof(SbcfLcase));    
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLcaseForceSmlc(ElemPairK ElemK, T_SMLC_K SmlcK, T_SBCF_LCASE& SbcfLcase)
{   
	SbcfLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LCASE_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_SMLC, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapSmlcK.Lookup(SmlcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapSmlcK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		pFile->Read(&SbcfLcase, sizeof(SbcfLcase));    
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLcaseForceGilcStrb(ElemPairK ElemK, T_GILC_K GilcK, T_STRB_LCASE& StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LCASE_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_GILC_STRB, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapGilcStrbK.Lookup(GilcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapGilcStrbK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		pFile->Read(&StrbLcase, sizeof(StrbLcase));    
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLcaseForceGilcSbcf(ElemPairK ElemK, T_GILC_K GilcK, T_SBCF_LCASE& SbcfLcase)
{   
	SbcfLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LCASE_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_GILC_SBCF, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapGilcSbcfK.Lookup(GilcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapGilcSbcfK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		pFile->Read(&SbcfLcase, sizeof(SbcfLcase));    
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLcaseForceLcomStrb(ElemPairK ElemK, T_LCOM_K LcomK, T_STRB_LCASE& StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LCASE_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_LSTR, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapLcomStrbK.Lookup(LcomK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapLcomStrbK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		pFile->Read(&StrbLcase, sizeof(StrbLcase));    
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLcaseForceLcomSbcf(ElemPairK ElemK, T_LCOM_K LcomK, T_SBCF_LCASE& SbcfLcase)
{   
	SbcfLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LCASE_FORCE_RESULT);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_LSBC, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapLcomSbcfK.Lookup(LcomK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapLcomSbcfK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		pFile->Read(&SbcfLcase, sizeof(SbcfLcase));    
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceStld(ElemPairK ElemK, T_STLD_K StldK, _DGN_STRBFORC_LCASE &StldLcase)
{   
	StldLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_STLD, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapStldK.Lookup(StldK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapStldK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_STRBFORC_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StldLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceSplc(ElemPairK ElemK, T_SPLC_K SplcK, _DGN_STRBFORC_LCASE &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_SPLC, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapSplcK.Lookup(SplcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapSplcK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_STRBFORC_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceEspc(ElemPairK ElemK, T_ESPC_K EspcK, _DGN_STRBFORC_LCASE &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_ESPC, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapEspcK.Lookup(EspcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapEspcK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_STRBFORC_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceThis(ElemPairK ElemK, T_THIS_K ThisK, _DGN_STRBFORC_LCASE &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_THIS, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapThisK.Lookup(ThisK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapThisK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_STRBFORC_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceSgld(ElemPairK ElemK, T_SGLD_K SgldK, _DGN_STRBFORC_LCASE &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_SGLD, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapSgldK.Lookup(SgldK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapSgldK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_STRBFORC_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceStep(ElemPairK ElemK, T_STEP_K StepK, _DGN_STRBFORC_LCASE &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_STEP, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapStepK.Lookup(StepK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapStepK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_STRBFORC_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceMvld(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_LCASE &SbcfLcase)
{   
	SbcfLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_MVLD, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapMvldK.Lookup(MvldK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapMvldK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_SBCFFORC_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(SbcfLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

 // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
BOOL CDesignForceResult::GetLoadCaseForceMvldBS(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_LCASE &SbcfLcase)
{
    SbcfLcase.Initialize();
    CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
    LONGLONG nStartRecord, nRecordLength;
    if (!m_pRdfp->GetRdfp(D_FP_LCFS_MVLD_BS_SPC, 0, nStartRecord, nRecordLength))return FALSE;

    try
    {
        T_ELEM_K KeySerial = m_pDataCtrl->Get_SerialCheckElemData(ElemK);
        UINT nSerialLcaseK = 0;
        m_pForceCtrl->m_arMapMvldK.Lookup(MvldK, nSerialLcaseK);
        UINT nLcaseNum = m_pForceCtrl->m_arMapMvldK.GetCount();
        LONGLONG nRecord = nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

        _DGN_SBCFFORC_LCASE_CH ForceCH;
        CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
        ForceCH.ConvertToString(SbcfLcase);
    }
    catch (int iercod)
    {
        DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
        return FALSE;
    }

    return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceSmlc(ElemPairK ElemK, T_SMLC_K SmlcK, _DGN_SBCFFORC_LCASE &SbcfLcase)
{   
	SbcfLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_SMLC, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapSmlcK.Lookup(SmlcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapSmlcK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_SBCFFORC_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(SbcfLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceGilcStrb(ElemPairK ElemK, T_GILC_K GilcK, _DGN_STRBFORC_LCASE &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_GILC_STRB, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapGilcStrbK.Lookup(GilcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapGilcStrbK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_STRBFORC_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceGilcSbcf(ElemPairK ElemK, T_GILC_K GilcK, _DGN_SBCFFORC_LCASE &SbcfLcase)
{   
	SbcfLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_GILC_SBCF, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapGilcSbcfK.Lookup(GilcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapGilcSbcfK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_SBCFFORC_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(SbcfLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceLcomStrb(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_LCASE &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_LSTR, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapLcomStrbK.Lookup(LcomK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapLcomStrbK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_STRBFORC_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceLcomStrbMin(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_LCASE &StrbLcase)
{
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_LSTR_MIN, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapLcomStrbK.Lookup(LcomK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapLcomStrbK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_STRBFORC_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceLcomSbcf(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_SBCFFORC_LCASE &SbcfLcase)
{   
	SbcfLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_LSBC, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapLcomSbcfK.Lookup(LcomK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapLcomSbcfK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_SBCFFORC_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(SbcfLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}


BOOL CDesignForceResult::GetPartLoadCaseForceStld(ElemPairK ElemK, T_STLD_K StldK, _DGN_STRBFORC_PART_LCASE &StldLcase)
{   
	StldLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_STLD_PA, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapStldK.Lookup(StldK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapStldK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_STRBFORC_PART_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StldLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceSplc(ElemPairK ElemK, T_SPLC_K SplcK, _DGN_STRBFORC_PART_LCASE &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_SPLC_PA, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapSplcK.Lookup(SplcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapSplcK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_STRBFORC_PART_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceEspc(ElemPairK ElemK, T_ESPC_K EspcK, _DGN_STRBFORC_PART_LCASE &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_ESPC_PA, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapEspcK.Lookup(EspcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapEspcK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_STRBFORC_PART_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceThis(ElemPairK ElemK, T_THIS_K ThisK, _DGN_STRBFORC_PART_LCASE &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_THIS_PA, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapThisK.Lookup(ThisK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapThisK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_STRBFORC_PART_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceSgld(ElemPairK ElemK, T_SGLD_K SgldK, _DGN_STRBFORC_PART_LCASE &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_SGLD_PA, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapSgldK.Lookup(SgldK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapSgldK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_STRBFORC_PART_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceStep(ElemPairK ElemK, T_STEP_K StepK, _DGN_STRBFORC_PART_LCASE &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_STEP_PA, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapStepK.Lookup(StepK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapStepK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_STRBFORC_PART_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceMvld(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_PART_LCASE &SbcfLcase)
{   
	SbcfLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_MVLD_PA, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapMvldK.Lookup(MvldK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapMvldK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_SBCFFORC_PART_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(SbcfLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

 // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
BOOL CDesignForceResult::GetPartLoadCaseForceMvldBS(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_PART_LCASE &SbcfLcase)
{
    SbcfLcase.Initialize();
    CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
    LONGLONG nStartRecord, nRecordLength;
    if (!m_pRdfp->GetRdfp(D_FP_LCFS_MVLD_BS_SPC_PA, 0, nStartRecord, nRecordLength))return FALSE;

    try
    {
        T_ELEM_K KeySerial = m_pDataCtrl->Get_SerialCheckElemData(ElemK);
        UINT nSerialLcaseK = 0;
        m_pForceCtrl->m_arMapMvldK.Lookup(MvldK, nSerialLcaseK);
        UINT nLcaseNum = m_pForceCtrl->m_arMapMvldK.GetCount();
        LONGLONG nRecord = nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

        _DGN_SBCFFORC_PART_LCASE_CH ForceCH;
        CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
        ForceCH.ConvertToString(SbcfLcase);
    }
    catch (int iercod)
    {
        DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
        return FALSE;
    }

    return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceSmlc(ElemPairK ElemK, T_SMLC_K SmlcK, _DGN_SBCFFORC_PART_LCASE &SbcfLcase)
{   
	SbcfLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_SMLC_PA, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapSmlcK.Lookup(SmlcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapSmlcK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_SBCFFORC_PART_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(SbcfLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceGilcStrb(ElemPairK ElemK, T_GILC_K GilcK, _DGN_STRBFORC_PART_LCASE &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_GILC_STRB_PA, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapGilcStrbK.Lookup(GilcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapGilcStrbK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_STRBFORC_PART_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceGilcSbcf(ElemPairK ElemK, T_GILC_K GilcK, _DGN_SBCFFORC_PART_LCASE &SbcfLcase)
{   
	SbcfLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_GILC_SBCF_PA, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapGilcSbcfK.Lookup(GilcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapGilcSbcfK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_SBCFFORC_PART_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(SbcfLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceLcomStrb(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_PART_LCASE &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_LSTR_PA, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapLcomStrbK.Lookup(LcomK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapLcomStrbK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_STRBFORC_PART_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceLcomStrbMin(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_PART_LCASE &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_LSTR_PA_MIN, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapLcomStrbK.Lookup(LcomK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapLcomStrbK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_STRBFORC_PART_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}


BOOL CDesignForceResult::GetPartLoadCaseForceLcomSbcf(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_SBCFFORC_PART_LCASE &SbcfLcase)
{   
	SbcfLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_LSBC_PA, 0, nStartRecord, nRecordLength))return FALSE;
	
	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapLcomSbcfK.Lookup(LcomK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapLcomSbcfK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		_DGN_SBCFFORC_PART_LCASE_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(SbcfLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceStldScfp(ElemPairK ElemK, T_STLD_K StldK, _DGN_SCFP_PART_LCASE &ScfpLcase)
{
	ScfpLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_STLD_SCFP_PA, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapStldScfpK.Lookup(StldK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapStldScfpK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		pFile->Read(&ScfpLcase, sizeof(ScfpLcase));    
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceSgldScfp(ElemPairK ElemK, T_SGLD_K SgldK, _DGN_SCFP_PART_LCASE &ScfpLcase)
{
	ScfpLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_SGLD_SCFP_PA, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapSgldScfpK.Lookup(SgldK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapSgldScfpK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1
		
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		pFile->Read(&ScfpLcase, sizeof(ScfpLcase));        
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}


// Expansion Force (2 Position -> 5 Position)

BOOL CDesignForceResult::GetLoadCaseForceStld_EX(ElemPairK ElemK, T_STLD_K StldK, _DGN_STRBFORC_LCASE_EX &StldLcase)
{   
	StldLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_STLD_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapStldK.Lookup(StldK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapStldK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_STRBFORC_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StldLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceSplc_EX(ElemPairK ElemK, T_SPLC_K SplcK, _DGN_STRBFORC_LCASE_EX &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_SPLC_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapSplcK.Lookup(SplcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapSplcK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_STRBFORC_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}


BOOL CDesignForceResult::GetLoadCaseForceEspc_EX(ElemPairK ElemK, T_ESPC_K EspcK, _DGN_STRBFORC_LCASE_EX &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_ESPC_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapEspcK.Lookup(EspcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapEspcK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_STRBFORC_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceThis_EX(ElemPairK ElemK, T_THIS_K ThisK, _DGN_STRBFORC_LCASE_EX &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_THIS_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapThisK.Lookup(ThisK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapThisK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_STRBFORC_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceSgld_EX(ElemPairK ElemK, T_SGLD_K SgldK, _DGN_STRBFORC_LCASE_EX &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_SGLD_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapSgldK.Lookup(SgldK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapSgldK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_STRBFORC_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceStep_EX(ElemPairK ElemK, T_STEP_K StepK, _DGN_STRBFORC_LCASE_EX &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_STEP_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapStepK.Lookup(StepK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapStepK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_STRBFORC_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceMvld_EX(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_LCASE_EX &SbcfLcase)
{   
	SbcfLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_MVLD_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapMvldK.Lookup(MvldK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapMvldK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_SBCFFORC_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(SbcfLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

// Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
BOOL CDesignForceResult::GetLoadCaseForceMvldBS_EX(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_LCASE_EX &SbcfLcase)
{
    SbcfLcase.Initialize();
    CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
    LONGLONG nStartRecord, nRecordLength;
    if (!m_pRdfp->GetRdfp(D_FP_LCFS_MVLD_BS_SPC_EX, 0, nStartRecord, nRecordLength))return FALSE;

    try
    {
        T_ELEM_K KeySerial = m_pDataCtrl->Get_SerialCheckElemData(ElemK);
        UINT nSerialLcaseK = 0;
        m_pForceCtrl->m_arMapMvldK.Lookup(MvldK, nSerialLcaseK);
        UINT nLcaseNum = m_pForceCtrl->m_arMapMvldK.GetCount();
        LONGLONG nRecord = nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

        _DGN_SBCFFORC_LCASE_EX_CH ForceCH;
        CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
        ForceCH.ConvertToString(SbcfLcase);
    }
    catch (int iercod)
    {
        DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
        return FALSE;
    }

    return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceSmlc_EX(ElemPairK ElemK, T_SMLC_K SmlcK, _DGN_SBCFFORC_LCASE_EX &SbcfLcase)
{   
	SbcfLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_SMLC_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapSmlcK.Lookup(SmlcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapSmlcK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_SBCFFORC_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(SbcfLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceGilcStrb_EX(ElemPairK ElemK, T_GILC_K GilcK, _DGN_STRBFORC_LCASE_EX &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_GILC_STRB_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapGilcStrbK.Lookup(GilcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapGilcStrbK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_STRBFORC_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceGilcSbcf_EX(ElemPairK ElemK, T_GILC_K GilcK, _DGN_SBCFFORC_LCASE_EX &SbcfLcase)
{   
	SbcfLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_GILC_SBCF_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapGilcSbcfK.Lookup(GilcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapGilcSbcfK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_SBCFFORC_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(SbcfLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceLcomStrb_EX(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_LCASE_EX &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_LSTR_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapLcomStrbK.Lookup(LcomK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapLcomStrbK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_STRBFORC_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceLcomStrbMin_EX(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_LCASE_EX &StrbLcase)
{
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_LSTR_MIN_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapLcomStrbK.Lookup(LcomK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapLcomStrbK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_STRBFORC_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetLoadCaseForceLcomSbcf_EX(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_SBCFFORC_LCASE_EX &SbcfLcase)
{   
	SbcfLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_LSBC_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapLcomSbcfK.Lookup(LcomK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapLcomSbcfK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_SBCFFORC_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(SbcfLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}


// Expansion Part Force (2 Position -> 5 Position)

BOOL CDesignForceResult::GetPartLoadCaseForceStld_EX(ElemPairK ElemK, T_STLD_K StldK, _DGN_STRBFORC_PART_LCASE_EX &StldLcase)
{   
	StldLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_STLD_PA_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapStldK.Lookup(StldK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapStldK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_STRBFORC_PART_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StldLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceSplc_EX(ElemPairK ElemK, T_SPLC_K SplcK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_SPLC_PA_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapSplcK.Lookup(SplcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapSplcK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_STRBFORC_PART_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}


BOOL CDesignForceResult::GetPartLoadCaseForceEspc_EX(ElemPairK ElemK, T_ESPC_K EspcK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_ESPC_PA_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapEspcK.Lookup(EspcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapEspcK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_STRBFORC_PART_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceThis_EX(ElemPairK ElemK, T_THIS_K ThisK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_THIS_PA_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapThisK.Lookup(ThisK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapThisK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_STRBFORC_PART_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceSgld_EX(ElemPairK ElemK, T_SGLD_K SgldK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_SGLD_PA_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapSgldK.Lookup(SgldK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapSgldK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_STRBFORC_PART_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceStep_EX(ElemPairK ElemK, T_STEP_K StepK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_STEP_PA_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapStepK.Lookup(StepK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapStepK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_STRBFORC_PART_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceMvld_EX(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_PART_LCASE_EX &SbcfLcase)
{   
	SbcfLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_MVLD_PA_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapMvldK.Lookup(MvldK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapMvldK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_SBCFFORC_PART_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(SbcfLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

// Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
BOOL CDesignForceResult::GetPartLoadCaseForceMvldBS_EX(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_PART_LCASE_EX &SbcfLcase)
{
    SbcfLcase.Initialize();
    CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
    LONGLONG nStartRecord, nRecordLength;
    if (!m_pRdfp->GetRdfp(D_FP_LCFS_MVLD_BS_SPC_PA_EX, 0, nStartRecord, nRecordLength))return FALSE;

    try
    {
        T_ELEM_K KeySerial = m_pDataCtrl->Get_SerialCheckElemData(ElemK);
        UINT nSerialLcaseK = 0;
        m_pForceCtrl->m_arMapMvldK.Lookup(MvldK, nSerialLcaseK);
        UINT nLcaseNum = m_pForceCtrl->m_arMapMvldK.GetCount();
        LONGLONG nRecord = nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

        _DGN_SBCFFORC_PART_LCASE_EX_CH ForceCH;
        CBnFile::SeekBN(pFile, nRecord, nRecordLength);
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
        ForceCH.ConvertToString(SbcfLcase);
    }
    catch (int iercod)
    {
        DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
        return FALSE;
    }

    return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceSmlc_EX(ElemPairK ElemK, T_SMLC_K SmlcK, _DGN_SBCFFORC_PART_LCASE_EX &SbcfLcase)
{   
	SbcfLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_SMLC_PA_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapSmlcK.Lookup(SmlcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapSmlcK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_SBCFFORC_PART_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(SbcfLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceGilcStrb_EX(ElemPairK ElemK, T_GILC_K GilcK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_GILC_STRB_PA_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapGilcStrbK.Lookup(GilcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapGilcStrbK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_STRBFORC_PART_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceGilcSbcf_EX(ElemPairK ElemK, T_GILC_K GilcK, _DGN_SBCFFORC_PART_LCASE_EX &SbcfLcase)
{   
	SbcfLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_GILC_SBCF_PA_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapGilcSbcfK.Lookup(GilcK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapGilcSbcfK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_SBCFFORC_PART_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(SbcfLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceLcomStrb_EX(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_LSTR_PA_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapLcomStrbK.Lookup(LcomK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapLcomStrbK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_STRBFORC_PART_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceLcomStrbMin_EX(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase)
{   
	StrbLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_LSTR_PA_MIN_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapLcomStrbK.Lookup(LcomK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapLcomStrbK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_STRBFORC_PART_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(StrbLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}


BOOL CDesignForceResult::GetPartLoadCaseForceLcomSbcf_EX(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_SBCFFORC_PART_LCASE_EX &SbcfLcase)
{   
	SbcfLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_LSBC_PA_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapLcomSbcfK.Lookup(LcomK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapLcomSbcfK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		_DGN_SBCFFORC_PART_LCASE_EX_CH ForceCH;
		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		UINT bytesRead = pFile->Read(&ForceCH, sizeof(ForceCH));
		if(bytesRead == 0) {
			// 파일 끝(EOF) 또는 읽을 데이터가 없음
			ForceCH.Initialize();
		}
		else if(bytesRead < sizeof(ForceCH)) {
			// 일부만 읽었음. 파일 끝에 도달했을 수도 있음.
			ForceCH.Initialize();
		}
		ForceCH.ConvertToString(SbcfLcase);
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}


BOOL CDesignForceResult::GetPartLoadCaseForceStldScfp_EX(ElemPairK ElemK, T_STLD_K StldK, _DGN_SCFP_PART_LCASE_EX &ScfpLcase)
{
	ScfpLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_STLD_SCFP_PA_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapStldScfpK.Lookup(StldK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapStldScfpK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		pFile->Read(&ScfpLcase, sizeof(ScfpLcase));    
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}

BOOL CDesignForceResult::GetPartLoadCaseForceSgldScfp_EX(ElemPairK ElemK, T_SGLD_K SgldK, _DGN_SCFP_PART_LCASE_EX &ScfpLcase)
{
	ScfpLcase.Initialize();
	CFile* pFile = m_pDataCtrl->GetDgnResultFileCtrl()->GetDgnForceResultFilePoint(D_DESIGN_LOAD_CASE_PART_FORCE);
	LONGLONG nStartRecord, nRecordLength;
	if(!m_pRdfp->GetRdfp(D_FP_LCFS_SGLD_SCFP_PA_EX, 0, nStartRecord, nRecordLength))return FALSE;

	try
	{
		T_ELEM_K KeySerial=m_pDataCtrl->Get_SerialCheckElemData(ElemK);
		UINT nSerialLcaseK=0;  
		m_pForceCtrl->m_arMapSgldScfpK.Lookup(SgldK,nSerialLcaseK);
		UINT nLcaseNum = m_pForceCtrl->m_arMapSgldScfpK.GetCount();    
		LONGLONG nRecord=nStartRecord+(KeySerial-1)*nLcaseNum+nSerialLcaseK; //-1

		CBnFile::SeekBN(pFile, nRecord, nRecordLength);    
		pFile->Read(&ScfpLcase, sizeof(ScfpLcase));        
	}
	catch(int iercod)
	{
		DisplayErrorMessage(_LS(IDS_DB_BN2_ER_RESULT), iercod);
		return FALSE;
	}

	return TRUE;
}
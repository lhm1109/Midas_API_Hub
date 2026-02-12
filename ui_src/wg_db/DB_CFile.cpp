// DB_CFile.cpp: implementation of the DB_CFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DB_CFile.h"

#include "DB_CDataClass.h"  // for T_VD_DATA_PLIST

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Build 끝난 후 아래 두줄 막을 것
//#define D_FORMAL_2_TRIAL  // 정식 버전 읽어서 Trial 버전으로 저장할 때
//#define D_TRIAL_2_FORMAL  // Trial 버전 읽어서 정식 버전 저장할 때
//#define D_FORMAL_2_EDU // 정식 버전 읽어서 EDU 버전으로 저장할때
//#define D_EDU_2_FORMAL // EDU 버전 읽어서 정식 버전으로 저장할 때

#define D_MAX_DATA_HDR_NUM_TRIAL  5         // Trial Version
#define D_MAX_DATA_HDR_NUM_EDU   10
#define D_MAX_DATA_HDR_NUM_FORMAL 20        // Licensed Version

#if defined(_TRIAL)
#define D_MAX_DATA_HDR_NUM  D_MAX_DATA_HDR_NUM_TRIAL
#elif defined(_EDU)
#define D_MAX_DATA_HDR_NUM  D_MAX_DATA_HDR_NUM_EDU
#else
#define D_MAX_DATA_HDR_NUM  D_MAX_DATA_HDR_NUM_FORMAL
#endif

#define D_DB_FILE_MAX_PWLEN   1024

#define D_DB_ERR_FILE_WRITE     _LS(IDS_DB_ERR_FILE_WRITE)
#define D_DB_ERR_INVALID_PW     _LS(IDS_DB_ERR_INVALID_PW)
#define D_DB_ERR_INVALID_PWLEN  _LS(IDS_DB_ERR_INVALID_PWLEN)
#define D_DB_ERR_INVALID_FMT    _LS(IDS_DB_ERR_INVALID_FMT)
#if defined(_EDU)
	#define D_DB_ERR_NOT_COMPATIBLE _LS(IDS_DB_ERR_NOT_COMPATIBLE_EDU)
#else
	#define D_DB_ERR_NOT_COMPATIBLE _LS(IDS_DB_ERR_NOT_COMPATIBLE)
#endif
#define D_DB_ERR_CANCEL_READING _LS(IDS_DB_ERR_CANCEL_READING)

#define D_DB_UPPER_VERSION_FILE _LS(IDS_DB_UPPER_VERSION_FILE)


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DB_CFile::DB_CFile()
{
	memset(m_FileHdr.HEAD, ' ', sizeof(m_FileHdr.HEAD));
	m_FileHdr.FVER = D_VER(0,0,0,0);
	m_FileHdr.DVER = D_VER(0,0,0,0);
	m_FileHdr.TYPE = 0;
	m_FileHdr.ENCR = 0;
	m_FileHdr.COMP = 0;
	m_FileHdr.RES1 = GetRandomValue() ^ 0x19701007;
	m_FileHdr.RES2 = GetRandomValue() ^ 0x19710829;
	m_FileHdr.RES3 = GetRandomValue() ^ 0x20000220;
	m_FileHdr.RES4 = GetRandomValue() ^ 0x20020607;
	m_FileHdr.HEAD_PWLEN = 0;
	m_FileHdr.SIZE = sizeof(m_FileHdr);
	m_csPassword = "";
	m_nFileHdrSize = sizeof(m_FileHdr);

	m_DBHdr.LEN = 0;
	m_DBHdr.NDB = 0;

	m_nLastDBHdrPos = 0;
	m_nLastDataHdrInfoPos = 0;
	m_nNewDataHdrPos = 0;

#if defined(_TRIAL)
	m_nFactor1 = 4;
	m_nFactor2 = 0x0F;
#elif defined(_EDU)
	m_nFactor1 = 6;
	m_nFactor2 = 0x3F;
#else
	m_nFactor1 = 5;
	m_nFactor2 = 0x1F;
#endif

	m_cmName2DataHdr.InitHashTable(31);
}

DB_CFile::~DB_CFile()
{

}

void DB_CFile::InitFile(CFile* pFile, char* HeadName, UINT nDataVersion)
{
	m_pFile = pFile;
	memcpy(m_FileHdr.HEAD, HeadName, 4);
	m_FileHdr.DVER = nDataVersion;
}

UINT DB_CFile::GetRandomValue()
{
	static BOOL bInit=FALSE;
	if (!bInit)
	{
		srand( (unsigned)time( NULL ) );
		bInit = TRUE;
	}

	UINT v1, v2;
	v1 = rand();
	v2 = rand();

	UINT nRandom = (v1 << 16) | v2;
	return nRandom;
}

void DB_CFile::SetEncryption(UINT nMethod)
{
	m_FileHdr.ENCR = nMethod;
	switch(nMethod)
	{
	case D_FILE_ENCR_NONE: 
		break;
	case D_FILE_ENCR_BASIC:
		{
			m_FileHdr.RES1 = GetRandomValue();
			m_FileHdr.RES2 = m_FileHdr.RES1 ^ 0x19710829; 
		}
		break;
	default:
		VERIFY(0);
		break;
	}
}

void DB_CFile::EncryptDataHdr(T_DB_DATA_HDR& rDataHdr)
{
	switch(m_FileHdr.ENCR)
	{
	case D_FILE_ENCR_NONE:
		break;
	case D_FILE_ENCR_BASIC:
		rDataHdr.SP1 = rDataHdr.SP1^(m_FileHdr.RES1^0x19701007);
		rDataHdr.SP2 = rDataHdr.SP2^(m_FileHdr.RES1^0x19701007);
		rDataHdr.NOD = rDataHdr.NOD^(m_FileHdr.RES1^0x19701007);
		break;
	default:
		VERIFY(0);
		break;
	}
}

void DB_CFile::DecryptDataHdr(T_DB_DATA_HDR& rDataHdr)
{
	switch(m_FileHdr.ENCR)
	{
	case D_FILE_ENCR_NONE:
		break;
	case D_FILE_ENCR_BASIC:
		rDataHdr.SP1 = rDataHdr.SP1^(m_FileHdr.RES1^0x19701007);
		rDataHdr.SP2 = rDataHdr.SP2^(m_FileHdr.RES1^0x19701007);
		rDataHdr.NOD = rDataHdr.NOD^(m_FileHdr.RES1^0x19701007);
		break;
	default:
		VERIFY(0);
		break;
	}
}

//----------------------------------------------------------------------
BOOL DB_CFile::WriteFileHdr()
{
#if defined(_TRIAL)
	m_FileHdr.TYPE = 1;
#elif defined(_EDU)
	m_FileHdr.TYPE = 2;
#endif

#if defined(D_FORMAL_2_TRIAL)
	m_FileHdr.TYPE = 1;
#elif defined(D_FORMAL_2_EDU)
	m_FileHdr.TYPE = 2;
#endif

	Seek(0, CFile::begin);
	Write(&m_FileHdr, sizeof(m_FileHdr));
	if (m_FileHdr.HEAD_PWLEN > 0)
	{
		if (m_FileHdr.HEAD_PWLEN > D_DB_FILE_MAX_PWLEN)
		{
			m_Error.SetLastError(1, D_DB_ERR_INVALID_PWLEN);
			return FALSE;
		}
		Write(m_csPassword, m_FileHdr.HEAD_PWLEN);
	}
	return TRUE;
}

BOOL DB_CFile::WriteDBHdr(LPCTSTR lpszDBName, LPCTSTR lpszDBDesc)
{
	T_DB_DB_HDR DBHdr;
	if (m_nLastDBHdrPos == 0)
	{
		Seek(m_nFileHdrSize, CFile::begin);
	}
	else
	{
		Seek(m_nLastDBHdrPos, CFile::begin);
		if (!Read(&DBHdr, sizeof(DBHdr)))
		{
			m_Error.SetLastError(1, D_DB_ERR_FILE_WRITE);
			return FALSE;
		}
		ASSERT(DBHdr.NDB == 0);
		DBHdr.NDB = GetLength();
		Seek(m_nLastDBHdrPos, CFile::begin);
		Write(&DBHdr, sizeof(DBHdr));

		Seek(0, CFile::end);
	}

	CString csName = lpszDBName;
	CString csDesc = lpszDBDesc;
	m_nLastDBHdrPos = GetPosition();
	DBHdr.NDB = 0;
	DBHdr.LEN = csName.GetLength();
	DBHdr.LEN2 = csDesc.GetLength();
	Write(&DBHdr, sizeof(DBHdr));
	if (DBHdr.LEN > 0) Write(lpszDBName, DBHdr.LEN);
	if (DBHdr.LEN2 > 0) Write(lpszDBDesc, DBHdr.LEN2);

	WriteDataHdrInfo();

	return TRUE;
}

BOOL DB_CFile::WriteDataHdrInfo()
{
	m_nLastDataHdrInfoPos = GetPosition();
	T_DB_DATA_HDR_INFO DataHdrInfo;

	#if defined(D_FORMAL_2_TRIAL)
		DataHdrInfo.MH = D_MAX_DATA_HDR_NUM_TRIAL;
	#elif defined(D_FORMAL_2_EDU)
		DataHdrInfo.MH = D_MAX_DATA_HDR_NUM_EDU;
	#else
		DataHdrInfo.MH = D_MAX_DATA_HDR_NUM;
	#endif
	DataHdrInfo.NOH = 0;
	DataHdrInfo.NH = 0;
	Write(&DataHdrInfo, sizeof(DataHdrInfo));

	m_nNewDataHdrPos = GetPosition();
	T_DB_DATA_HDR DataHdr;
	memset(DataHdr.ID, ' ', sizeof(DataHdr.ID));
	DataHdr.SP1 = 0;
	DataHdr.SP2 = 0;
	DataHdr.NOD = 0;
	for (UINT i = 0; i < DataHdrInfo.MH; i++)
		Write(&DataHdr, sizeof(DataHdr));

	return TRUE;
}

BOOL DB_CFile::WriteDataHdr(const char ID[8], UINT nCount)
{
	T_DB_DATA_HDR_INFO DataHdrInfo;
	Seek(m_nLastDataHdrInfoPos, CFile::begin);
	if (!Read(&DataHdrInfo, sizeof(DataHdrInfo)))
	{
		m_Error.SetLastError(1, D_DB_ERR_FILE_WRITE);
		return FALSE;
	}
	// 헤더가 다 찼으면 다음 헤더를 작성한다.
	if (DataHdrInfo.NOH == DataHdrInfo.MH)
	{
		Seek(m_nLastDataHdrInfoPos, CFile::begin);
		DataHdrInfo.NH = GetLength();
		Write(&DataHdrInfo, sizeof(DataHdrInfo));
		Seek(0, CFile::end);
		WriteDataHdrInfo();

		#if defined(D_FORMAL_2_TRIAL)
			DataHdrInfo.MH = D_MAX_DATA_HDR_NUM_TRIAL;
		#elif defined(D_FORMAL_2_EDU)
			DataHdrInfo.MH = D_MAX_DATA_HDR_NUM_EDU;
		#else
			DataHdrInfo.MH = D_MAX_DATA_HDR_NUM;
		#endif
		DataHdrInfo.NH = 0;
		DataHdrInfo.NOH = 0;
	}

	DataHdrInfo.NOH++;
	Seek(m_nLastDataHdrInfoPos, CFile::begin);
	Write(&DataHdrInfo, sizeof(DataHdrInfo));

	Seek(m_nNewDataHdrPos, CFile::begin);
	T_DB_DATA_HDR DataHdr;
	memcpy(DataHdr.ID, ID, sizeof(DataHdr.ID));
	UINT nPos = GetLength();

	#if defined(D_FORMAL_2_TRIAL)
		DataHdr.SP1 = nPos>>4;       // nPOS/16 or nPOS/32
		DataHdr.SP2 = nPos&0x0f;     // nPOS%16 or nPOS%32
	#elif defined(D_FORMAL_2_EDU)
		DataHdr.SP1 = nPos>>6;       // nPOS/16 or nPOS/32
		DataHdr.SP2 = nPos&0x3f;     // nPOS%16 or nPOS%32
	#else
		DataHdr.SP1 = nPos>>m_nFactor1;    // nPOS/16 or nPOS/32
		DataHdr.SP2 = nPos&m_nFactor2;     // nPOS%16 or nPOS%32
	#endif

	DataHdr.NOD = nCount;
	EncryptDataHdr(DataHdr);
	Write(&DataHdr, sizeof(DataHdr));
	m_nNewDataHdrPos = GetPosition();

	Seek(0, CFile::end);

	return TRUE;
}

//----------------------------------------------------------------------
BOOL DB_CFile::ReadFileHdr()
{
#if defined(_TRIAL)
	m_FileHdr.TYPE = 1;
#elif defined(_EDU)
	m_FileHdr.TYPE = 2;
#endif

#if defined(D_TRIAL_2_FORMAL)
	m_FileHdr.TYPE = 1;
#elif defined(D_EDU_2_FORMAL)
	m_FileHdr.TYPE = 2;
#endif


	Seek(0, CFile::begin);
	T_DB_FILE_HDR FileHdr;
	CString csPassword;
	if (!Read(&FileHdr, sizeof(FileHdr)))
	{
		m_Error.SetLastError(1, D_DB_ERR_INVALID_FMT);
		return FALSE;
	}
	if (FileHdr.SIZE != sizeof(FileHdr))
	{
		m_Error.SetLastError(1, D_DB_ERR_INVALID_FMT);
		return FALSE;
	}
	if (FileHdr.HEAD_PWLEN > 0)
	{
		if (FileHdr.HEAD_PWLEN > D_DB_FILE_MAX_PWLEN)
		{
			m_Error.SetLastError(1, D_DB_ERR_INVALID_PWLEN);
			return FALSE;
		}
		if (!Read(csPassword.GetBuffer(FileHdr.HEAD_PWLEN+1), m_FileHdr.HEAD_PWLEN))
		{
			csPassword.ReleaseBuffer();
			m_Error.SetLastError(1, D_DB_ERR_INVALID_FMT);
			return FALSE;
		}
		csPassword.ReleaseBuffer();
		if (CheckPassword(csPassword) == FALSE)
		{
			m_Error.SetLastError(1, D_DB_ERR_INVALID_PW);
			return FALSE;
		}
	}
	if (m_FileHdr.DVER < FileHdr.DVER)
	{
		if (AfxMessageBox(D_DB_UPPER_VERSION_FILE, MB_YESNO) == IDNO)
		{
			m_Error.SetLastError(0, D_DB_ERR_CANCEL_READING);
			return FALSE;
		}
	}
	if (m_FileHdr.TYPE != FileHdr.TYPE)
	{
		m_Error.SetLastError(1, D_DB_ERR_NOT_COMPATIBLE);
		return FALSE;
	}
	m_FileHdr = FileHdr;
	return TRUE;
}

BOOL DB_CFile::ReadDBHdr(CString& rcsDBName, CString& rcsDBDesc)
{
	if (m_nLastDBHdrPos == 0)
		m_nLastDBHdrPos = GetFirstDBHdrPos();
	else
	{
		if (m_DBHdr.NDB == 0) return FALSE;
		m_nLastDBHdrPos = m_DBHdr.NDB;
	}
	Seek(m_nLastDBHdrPos, CFile::begin);

	T_DB_DB_INFO DBInfo;
	if (!ReadDBInfo(m_DBHdr, DBInfo)) return FALSE;
	if (!ReadAllDataHdr(DBInfo)) return FALSE;
	rcsDBName = DBInfo.csName;
	rcsDBDesc = DBInfo.csDesc;

	return TRUE;
}

BOOL DB_CFile::ReadDataHdr(const char ID[8], UINT& nCount)
{
	char szName[9];
	szName[8] = 0;
	memcpy(szName, ID, 8);
	
	T_DB_DATA_HDR DataHdr;
	if (!m_cmName2DataHdr.Lookup(szName, DataHdr)) return FALSE;

	nCount = DataHdr.NOD;

	#if defined(D_TRIAL_2_FORMAL)
		UINT nPos = (DataHdr.SP1 << 4) + DataHdr.SP2; // SP1*16+SP2 or SP1*32+SP2
	#elif defined(D_EDU_2_FORMAL)
		UINT nPos = (DataHdr.SP1 << 6) + DataHdr.SP2; // SP1*16+SP2 or SP1*32+SP2
	#else
		UINT nPos = (DataHdr.SP1 << m_nFactor1) + DataHdr.SP2; // SP1*16+SP2 or SP1*32+SP2
	#endif
	Seek(nPos, CFile::begin);
	return TRUE;
}

BOOL DB_CFile::ReadAllDBHdr(T_DB_DB_INFO_LIST& raDBInfo)
{
	raDBInfo.RemoveAll();

	LONG nFirstDBHdrPos = GetFirstDBHdrPos();
	Seek(nFirstDBHdrPos, CFile::begin);

	T_DB_DB_HDR DBHdr;
	T_DB_DB_INFO DBInfo;
	while (1)
	{
		if (!ReadDBInfo(DBHdr, DBInfo)) return FALSE;
		raDBInfo.Add(DBInfo);

		if (DBHdr.NDB == 0) break;
		Seek(DBHdr.NDB, CFile::begin);
	}
	return TRUE;
}

BOOL DB_CFile::ReadAllDataHdr(T_DB_DB_INFO& rDBInfo)
{
	if (GetPosition() != rDBInfo.nPosDataHdrInfo)
		Seek(rDBInfo.nPosDataHdrInfo, CFile::begin);

	UINT nMaxHdrNum;
	#if defined (D_TRIAL_2_FORMAL)
		nMaxHdrNum = D_MAX_DATA_HDR_NUM_TRIAL;
	#elif defined(D_EDU_2_FORMAL)
		nMaxHdrNum = D_MAX_DATA_HDR_NUM_EDU;
	#else
		nMaxHdrNum = D_MAX_DATA_HDR_NUM;
	#endif

	m_cmName2DataHdr.RemoveAll();
	T_DB_DATA_HDR_INFO DataHdrInfo;
	T_DB_DATA_HDR DataHdr;
	char szName[9];
	szName[8] = 0;
	while (1)
	{
		if (!Read(&DataHdrInfo, sizeof(DataHdrInfo)))
		{
			m_Error.SetLastError(1, D_DB_ERR_INVALID_FMT);
			return FALSE;
		}
		if ((DataHdrInfo.MH != nMaxHdrNum)                      ||
				(DataHdrInfo.MH != DataHdrInfo.NOH && DataHdrInfo.NH != 0)   )
		{
			m_Error.SetLastError(1, D_DB_ERR_INVALID_FMT);
			return FALSE;
		}
		for (UINT i = 0; i < DataHdrInfo.NOH; i++)
		{
			if (!Read(&DataHdr, sizeof(DataHdr)))
			{
				m_Error.SetLastError(1, D_DB_ERR_INVALID_FMT);
				return FALSE;
			}
			DecryptDataHdr(DataHdr);
			memcpy(szName, DataHdr.ID, sizeof(DataHdr.ID));
			m_cmName2DataHdr.SetAt(szName, DataHdr);
		}
		if (DataHdrInfo.NH == 0) break;
		Seek(DataHdrInfo.NH, CFile::begin);
	}
	return TRUE;
}

//----------------------------------------------------------------------
LONG DB_CFile::GetFirstDBHdrPos()
{
	return sizeof(m_FileHdr)+m_FileHdr.HEAD_PWLEN;
}

BOOL DB_CFile::ReadDBInfo(T_DB_DB_HDR& rDBHdr, T_DB_DB_INFO& rDBInfo)
{
	rDBInfo.Initiaize();
	if (!Read(&rDBHdr, sizeof(rDBHdr)))
	{
		m_Error.SetLastError(1, D_DB_ERR_INVALID_FMT);
		return FALSE;
	}
	if (rDBHdr.LEN > 0)
	{
		if (!Read(rDBInfo.csName.GetBuffer(rDBHdr.LEN+1), rDBHdr.LEN))
		{
			rDBInfo.csName.ReleaseBuffer();
			m_Error.SetLastError(1, D_DB_ERR_INVALID_FMT);
			return FALSE;
		}
		rDBInfo.csName.ReleaseBuffer();
		rDBInfo.csName = rDBInfo.csName.Left(rDBHdr.LEN);
	}
	if (rDBHdr.LEN2 > 0)
	{
		if (!Read(rDBInfo.csDesc.GetBuffer(rDBHdr.LEN2+1), rDBHdr.LEN2))
		{
			rDBInfo.csDesc.ReleaseBuffer();
			m_Error.SetLastError(1, D_DB_ERR_INVALID_FMT);
			return FALSE;
		}
		rDBInfo.csDesc.ReleaseBuffer();
		rDBInfo.csDesc = rDBInfo.csDesc.Left(rDBHdr.LEN2);
	}
	rDBInfo.nPosDataHdrInfo = m_pFile->GetPosition();
	return TRUE;
}

BOOL DB_CFile::CheckPassword(LPCTSTR lpszPassword)
{
	// Dialog를 띄워서 물어본다.
	return FALSE;
}

//----------------------------------------------------------------------
BOOL DB_CFile::Read(void* lpBuf, UINT nCount)
{
	if (m_pFile->Read(lpBuf, nCount) != nCount) return FALSE;
	return TRUE;
}

BOOL DB_CFile::Write(const void* lpBuf, UINT nCount)
{
	m_pFile->Write(lpBuf, nCount);
	return TRUE;
}

LONG DB_CFile::Seek(LONG lOffset, UINT nFrom)
{
	return m_pFile->Seek(lOffset, nFrom);
}

DWORD DB_CFile::GetPosition()
{
	return m_pFile->GetPosition();
}

DWORD DB_CFile::GetLength()
{
	return m_pFile->GetLength();
}

BOOL DB_CFile::ReadFD(void* lpBuf, UINT nCount)
{
	DWORD CurPos = GetPosition();
	DWORD StartMark, EndMark;
	if (m_pFile->Read(&StartMark, sizeof(DWORD)) != sizeof(DWORD)) return FALSE;
	if (CurPos != StartMark) return FALSE;
	if (m_pFile->Read(lpBuf, nCount) != nCount) return FALSE;
	if (m_pFile->Read(&EndMark, sizeof(DWORD)) != sizeof(DWORD)) return FALSE;
	if (StartMark + nCount != EndMark) return FALSE;
	return TRUE;
}

BOOL DB_CFile::WriteFD(const void* lpBuf, UINT nCount)
{
	DWORD StartMark = GetPosition();
	DWORD EndMark = StartMark + nCount;

	m_pFile->Write(&StartMark, sizeof(DWORD));
	m_pFile->Write(lpBuf, nCount);
	m_pFile->Write(&EndMark, sizeof(DWORD));

	return TRUE;
}

BOOL DB_CFile::ReadVD(T_VD_DATA_PLIST* paVD)
{
	// 64bit에서 T_VD_DATA::pAddr -> nDymmy로 변경하면서 아래 함수는 무효.
	// 이 함수 호출하는 곳이 없으므로 이렇게 처리함.  by Hsshim.
	ASSERT(0);  
	return FALSE;

//   if (paVD == 0) return TRUE;
//   UINT nRead;
//   UINT nCount, nWrittenCount;
//   int nSize = paVD->GetSize();
//   for (int i = 0; i < nSize; i++)
//   {
//     if ((*paVD)[i]->nSize != ~((*paVD)[i]->rSize)   ||
//         (*paVD)[i]->nCount != ~((*paVD)[i]->rCount)  )
//         return FALSE;
//     nCount = (*paVD)[i]->nSize * (*paVD)[i]->nCount;
//     if (nCount == 0) continue;
//     if (m_pFile->Read(&nWrittenCount, sizeof(nWrittenCount)) != sizeof(nWrittenCount)) return FALSE;
//     if (nWrittenCount != nCount) return FALSE;
//     nRead = m_pFile->Read((*paVD)[i]->pAddr, nCount);
//     if (nRead != nCount) return FALSE;
//   }
//   return TRUE;
}

BOOL DB_CFile::WriteVD(T_VD_DATA_PLIST* paVD)
{
	// 64bit에서 T_VD_DATA::pAddr -> nDymmy로 변경하면서 아래 함수는 무효.
	// 이 함수 호출하는 곳이 없으므로 이렇게 처리함.  by Hsshim.
	ASSERT(0);  
	return FALSE;

//   if (paVD == 0) return TRUE;
// 
//   UINT nCount;
//   int nSize = paVD->GetSize();
//   for (int i = 0; i < nSize; i++)
//   {
//     nCount = (*paVD)[i]->nSize * (*paVD)[i]->nCount;
//     if (nCount == 0) continue;
//     m_pFile->Write(&nCount, sizeof(nCount));
//     m_pFile->Write((*paVD)[i]->pAddr, nCount);
//   }
//   return TRUE;
}

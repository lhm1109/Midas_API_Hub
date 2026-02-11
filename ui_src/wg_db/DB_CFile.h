// DB_CFile.h: interface for the DB_CFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DB_CFILE_H__)
#define __DB_CFILE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "..\MIDAS_lib\MIDAS_Error.h"
#include "..\wg_base\wg_base_Error.h"

#define D_VER(n1,n2,n3,n4) ((n1<<24)+(n2<<16)+(n3<<8)+n4)

#define D_FILE_ENCR_NONE    0
#define D_FILE_ENCR_BASIC   1

#pragma pack(1)
/*
* File Header
HEAD    (4)    Product Head  : MGEN, MSDS, MCVL, MTOW, MADS
FVER    (4)    File Version  : D_VER(1,0,0,0)
DVER    (4)    Data Version  : D_VER(5,3,1,0)
TYPE    (4)    File Type     : 0:Formal, 1:Trial
ENCR    (4)    Data Encryption Method : 0(no encryption)
COMP    (4)    Compression Method     : 0(no compression)
RES1    (4)    Reserved
RES2    (4)    Reserved
RES3    (4)    Reserved
RES4    (4)    Reserved
HEAD_PWLEN   (4)    Password Length
SIZE    (4)    Header Size
PW      (V)    Encrypted Password
*/
struct T_DB_FILE_HDR
{
	char HEAD[4];
	UINT FVER;
	UINT DVER;
	UINT TYPE;
	UINT ENCR;
	UINT COMP;
	UINT RES1;
	UINT RES2;
	UINT RES3;
	UINT RES4;
	UINT HEAD_PWLEN;
	UINT SIZE;
};

/*
* DB Header
NDB     (4)    Next DB Position
LEN     (4)    DB Name Length
LEN2    (4)    DB Desc Length
DBNAME  (V)    DB Name
DBDESC  (V)    DB Desc
*/
struct T_DB_DB_HDR
{
	UINT NDB;
	UINT LEN;
	UINT LEN2;
};

/*
* Data Header Info
MH      (4)    Maximum Header
NOH     (4)    Number of Header
NH      (4)    Next Header Position
*/
struct T_DB_DATA_HDR_INFO
{
	UINT MH;
	UINT NOH;
	UINT NH;
};

/*
* Data Header
ID      (4)    Data ID
SP1     (4)    Start Position   16 or 32로 나눈 몫
SP2     (4)    Start Position   16 or 32로 나눈 나머지
NOD     (4)    Number of Data
*/
struct T_DB_DATA_HDR
{
	char ID[8];
	UINT SP1;
	UINT SP2;
	UINT NOD;
};

#pragma pack()

struct T_DB_DB_INFO
{
	CString csName;
	CString csDesc;
	UINT    nPosDataHdrInfo;
	void Initiaize()
	{
		csName.Empty();
		csDesc.Empty();
		nPosDataHdrInfo = 0;
	}
};
typedef CArray<T_DB_DB_INFO, T_DB_DB_INFO&> T_DB_DB_INFO_LIST;

// forward declaration
struct T_VD_DATA;
typedef CArray<T_VD_DATA*, T_VD_DATA*> T_VD_DATA_PLIST;

class DB_CFile  
{
public:
	DB_CFile();
	virtual ~DB_CFile();
	CError m_Error;

public:
	void InitFile(CFile* pFile, char* HeadName, UINT nDataVersion);
	UINT GetRandomValue();
	void SetEncryption(UINT nMethod);
	void EncryptDataHdr(T_DB_DATA_HDR& rDataHdr);
	void DecryptDataHdr(T_DB_DATA_HDR& rDataHdr);
public:
	BOOL WriteFileHdr();
	BOOL WriteDBHdr(LPCTSTR lpszDBName, LPCTSTR lpszDBDesc);
	BOOL WriteDataHdr(const char ID[8], UINT nCount);
	CFile* GetRawFile() { return m_pFile; }

protected:
	BOOL WriteDataHdrInfo();

public:
	BOOL ReadFileHdr();
	BOOL ReadDBHdr(CString& rcsDBName, CString& rcsDBDesc);
	BOOL ReadDataHdr(const char ID[8], UINT& nCount);

public:
	BOOL ReadAllDBHdr(T_DB_DB_INFO_LIST& raDBInfo);
	BOOL ReadAllDataHdr(T_DB_DB_INFO& rDBInfo);

protected:
	LONG GetFirstDBHdrPos();
	BOOL ReadDBInfo(T_DB_DB_HDR& rDBHdr, T_DB_DB_INFO& rDBInfo);
	BOOL CheckPassword(LPCTSTR lpszPassword);

protected:
	BOOL Read(void* lpBuf, UINT nCount);
	BOOL Write(const void* lpBuf, UINT nCount);
	LONG Seek(LONG lOffset, UINT nFrom);
	DWORD GetPosition();
	DWORD GetLength();

public:
	BOOL ReadFD(void* lpBuf, UINT nCount);
	BOOL WriteFD(const void* lpBuf, UINT nCount);
	BOOL ReadVD(T_VD_DATA_PLIST* paVD);
	BOOL WriteVD(T_VD_DATA_PLIST* paVD);

protected:
	CFile* m_pFile;

	UINT                m_nFileHdrSize;
	T_DB_FILE_HDR       m_FileHdr;
	CString             m_csPassword;

	T_DB_DB_HDR         m_DBHdr;

	UINT                m_nLastDBHdrPos;
	UINT                m_nLastDataHdrInfoPos;
	UINT                m_nNewDataHdrPos;

	int                 m_nFactor1;  // Formal/Trial File의 물리적 구분
	int                 m_nFactor2;

	CMap<CStringA, LPCSTR, T_DB_DATA_HDR, T_DB_DATA_HDR&> m_cmName2DataHdr;
};

#endif // !defined(__DB_CFILE_H__)

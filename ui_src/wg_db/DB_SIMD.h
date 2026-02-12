#ifndef __DB_SIMD_DB_H__
#define __DB_SIMD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_THIS;
class CDB_THCR;
class CDB_SIPA;
class CDB_SIGR;
class CDB_SIGR_LOWER;

class CDB_SIMD
{
public:
	CDB_SIMD();
	virtual ~CDB_SIMD();
	CDBDoc* m_pDoc;

public:
	T_SIMD_K m_nStartNum;
	T_SIMD_K m_nLastNum;

public:
	void Add(T_SIMD_K Key, T_SIMD_D& rData, CDB_SIPA* pSipa, CDB_SIGR* pSigr, CDB_SIGR_LOWER* pSigrLower, CDB_THIS* pThis);
	BOOL Del(T_SIMD_K Key, CDB_SIPA* pSipa, CDB_SIGR* pSigr, CDB_SIGR_LOWER* pSigrLower, CDB_THIS* pThis);

public:
	BOOL Get(T_SIMD_K Key, T_SIMD_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIMD_K& rKey, T_SIMD_D& rData);

protected:
	CMap<T_SIMD_K, T_SIMD_K, T_SIMD_D, T_SIMD_D&>m_simd;
};



#endif
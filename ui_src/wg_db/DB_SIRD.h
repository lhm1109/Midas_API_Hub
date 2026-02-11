#ifndef __DB_SIRD_DB_H__
#define __DB_SIRD_DB_H__

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

class CDB_SIRD
{
public:
	CDB_SIRD();
	virtual ~CDB_SIRD();
	CDBDoc* m_pDoc;

public:
	T_SIRD_K m_nStartNum;
	T_SIRD_K m_nLastNum;

public:
	void Add(T_SIRD_K Key, T_SIRD_D& rData, CDB_SIPA* pSipa, CDB_SIGR* pSigr, CDB_SIGR_LOWER* pSigrLower, CDB_THIS* pThis);
	BOOL Del(T_SIRD_K Key, CDB_SIPA* pSipa, CDB_SIGR* pSigr, CDB_SIGR_LOWER* pSigrLower, CDB_THIS* pThis);

public:
	BOOL Get(T_SIRD_K Key, T_SIRD_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIRD_K& rKey, T_SIRD_D& rData);

protected:
	CMap<T_SIRD_K, T_SIRD_K, T_SIRD_D, T_SIRD_D&>m_sird;
};



#endif
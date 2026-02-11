#ifndef __DB_SIET_DB_H__
#define __DB_SIET_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SIPA;
class CDB_SIGR;
class CDB_SIGR_LOWER;

class CDB_SIET
{
public:
	CDB_SIET();
	virtual ~CDB_SIET();
	CDBDoc* m_pDoc;

public:
	T_SIET_K m_nStartNum;
	T_SIET_K m_nLastNum;

public:
	void Add(T_SIET_K Key, T_SIET_D& rData, CDB_SIPA* pSipa, CDB_SIGR* pSigr, CDB_SIGR_LOWER* pSigrLower);
	BOOL Del(T_SIET_K Key, CDB_SIPA* pSipa, CDB_SIGR* pSigr, CDB_SIGR_LOWER* pSigrLower);

public:
	BOOL Get(T_SIET_K Key, T_SIET_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIET_K& rKey, T_SIET_D& rData);

protected:
	CMap<T_SIET_K, T_SIET_K, T_SIET_D, T_SIET_D&>m_siet;
};



#endif
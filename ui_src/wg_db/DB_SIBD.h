#ifndef __DB_SIBD_DB_H__
#define __DB_SIBD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SIPA;
class CDB_SIGR;
class CDB_SIGR_LOWER;

class CDB_SIBD
{
public:
	CDB_SIBD();
	virtual ~CDB_SIBD();
	CDBDoc* m_pDoc;

public:
	T_SIBD_K m_nStartNum;
	T_SIBD_K m_nLastNum;

public:
	void Add(T_SIBD_K Key, T_SIBD_D& rData, CDB_SIPA* pSipa, CDB_SIGR* pSigr);
	BOOL Del(T_SIBD_K Key, CDB_SIPA* pSipa, CDB_SIGR* pSigr);

public:
	BOOL Get(T_SIBD_K Key, T_SIBD_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIBD_K& rKey, T_SIBD_D& rData);

protected:
	CMap<T_SIBD_K, T_SIBD_K, T_SIBD_D, T_SIBD_D&>m_sibd;
};



#endif
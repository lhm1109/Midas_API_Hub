#ifndef __DB_SIGR_USER_DB_H__
#define __DB_SIGR_USER_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_SIGR;

class CDB_SIGR_USER
{
public:
	CDB_SIGR_USER();
	virtual ~CDB_SIGR_USER();
	CDBDoc* m_pDoc;

public:
	T_SIGR_USER_K m_nStartNum;
	T_SIGR_USER_K m_nLastNum;

public:
	void Add(T_SIGR_USER_K Key, T_SIGR_USER_D& rData, CDB_ELEM* pElem, CDB_SIGR* pSigr);
	BOOL Del(T_SIGR_USER_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_SIGR_USER_K Key, T_SIGR_USER_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIGR_USER_K& rKey, T_SIGR_USER_D& rData);

protected:
	CMap<T_SIGR_USER_K, T_SIGR_USER_K, T_SIGR_USER_D, T_SIGR_USER_D&>m_sigrUser;
};

#endif
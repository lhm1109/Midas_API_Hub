#ifndef __DB_SISR_USER_DB_H__
#define __DB_SISR_USER_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_SISR;

class CDB_SISR_USER
{
public:
	CDB_SISR_USER();
	virtual ~CDB_SISR_USER();
	CDBDoc* m_pDoc;

public:
	T_SISR_USER_K m_nStartNum;
	T_SISR_USER_K m_nLastNum;

public:
	void Add(T_SISR_USER_K Key, T_SISR_USER_D& rData, CDB_SISR* pSisr, CDB_ELEM* pElem);
	BOOL Del(T_SISR_USER_K Key, CDB_SISR* pSisr, CDB_ELEM* pElem);

public:
	BOOL Get(T_SISR_USER_K Key, T_SISR_USER_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SISR_USER_K& rKey, T_SISR_USER_D& rData);

protected:
	CMap<T_SISR_USER_K, T_SISR_USER_K, T_SISR_USER_D, T_SISR_USER_D&>m_sisrUser;
};



#endif
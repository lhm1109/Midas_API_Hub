#ifndef __DB_EBMW_DB_H__
#define __DB_EBMW_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STOR;

class CDB_EBMW
{
public:
	CDB_EBMW();
	virtual ~CDB_EBMW();
	CDBDoc* m_pDoc;

public:
	void Add(T_EBMW_K Key,T_EBMW_D& rData,CDB_STOR* pStor);
	BOOL Del(T_EBMW_K Key,CDB_STOR* pStor);

public:
	BOOL Get(T_EBMW_K Key, T_EBMW_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_EBMW_K& rKey, T_EBMW_D& rData);

protected:
	CMap<T_EBMW_KEY,T_EBMW_KEY, T_EBMW_D, T_EBMW_D&>m_ebmw;
};

#endif

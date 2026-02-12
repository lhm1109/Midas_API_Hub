#ifndef __DB_SERD_DB_H__
#define __DB_SERD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDBDoc;
class CDB_ELEM;
class CDB_SERD
{
public:
	CDB_SERD();
	virtual ~CDB_SERD();
	CDBDoc* m_pDoc;

public:
	void Add(T_SERD_K Key,T_SERD_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SERD_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SERD_K Key,T_SERD_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_SERD_K& rKey,T_SERD_D& rData);

protected:
	CMap<T_SERD_K,T_SERD_K,T_SERD_D,T_SERD_D&>m_SERD;
};

#endif
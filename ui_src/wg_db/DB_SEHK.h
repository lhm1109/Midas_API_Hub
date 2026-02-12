#ifndef __DB_SEHK_DB_H__
#define __DB_SEHK_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDBDoc;
class CDB_ELEM;
class CDB_SEHK
{
public:
	CDB_SEHK();
	virtual ~CDB_SEHK();
	CDBDoc* m_pDoc;

public:
	void Add(T_SEHK_K Key,T_SEHK_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SEHK_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SEHK_K Key,T_SEHK_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_SEHK_K& rKey,T_SEHK_D& rData);

protected:
	CMap<T_SEHK_K,T_SEHK_K,T_SEHK_D,T_SEHK_D&>m_SEHK;
};

#endif
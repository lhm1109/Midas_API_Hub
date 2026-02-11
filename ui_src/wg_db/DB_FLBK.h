#ifndef __DB_FLBK_DB_H__
#define __DB_FLBK_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_FLBK
{
public:
	CDB_FLBK();
	virtual ~CDB_FLBK();
	CDBDoc* m_pDoc;

public:
	void Add(T_FLBK_K Key,T_FLBK_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_FLBK_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_FLBK_K Key,T_FLBK_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_FLBK_K& rKey,T_FLBK_D& rData);

protected:
	CMap<T_FLBK_K,T_FLBK_K,T_FLBK_D,T_FLBK_D&>m_flbk;
};

#endif


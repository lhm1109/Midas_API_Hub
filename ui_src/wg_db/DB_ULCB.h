#ifndef __DB_ULCB_DB_H__
#define __DB_ULCB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_LDGR;

class CDB_ULCB
{
public:
	CDB_ULCB();
	virtual ~CDB_ULCB();
	CDBDoc* m_pDoc;

public:
	T_ULCB_K m_nLastNum;
	T_ULCB_K m_nStartNum;

public:
	void Add(T_ULCB_K Key,T_ULCB_D& rData,CDB_ELEM* pElem,CDB_LDGR* pLdgr);
	BOOL Del(T_ULCB_K Key,CDB_ELEM* pElem,CDB_LDGR* pLdgr);

public:
	BOOL Get(T_ULCB_K Key,T_ULCB_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_ULCB_K& rKey,T_ULCB_D& rData);

protected:
	CMap<T_ULCB_K,T_ULCB_K,T_ULCB_D,T_ULCB_D&>m_ulcb;
};

#endif
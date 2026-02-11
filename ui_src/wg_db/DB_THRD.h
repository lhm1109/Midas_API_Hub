#ifndef __DB_THRD_DB_H__
#define __DB_THRD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;
class CDB_ELEM;

class CDB_THRD
{
public:
	CDB_THRD();
	virtual ~CDB_THRD();
	CDBDoc* m_pDoc;

public:
	T_THRD_K m_nStartNum;
	T_THRD_K m_nLastNum;

public:
	void Add(T_THRD_K Key,T_THRD_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem);
	BOOL Del(T_THRD_K Key,CDB_NODE* pNode,CDB_ELEM* pElem);

public:
	BOOL Get(T_THRD_K Key,T_THRD_D& rData);
		//{return m_thrd.Lookup(Key,rData);}
	int GetCount();
		//{return m_thrd.GetCount();}
	POSITION GetStart();
		//{return m_thrd.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_THRD_K& rKey,T_THRD_D& rData);
		//{m_thrd.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_THRD_K,T_THRD_K,T_THRD_D,T_THRD_D&>m_thrd;
};

#endif
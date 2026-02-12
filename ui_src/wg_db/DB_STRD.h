#ifndef __DB_STRD_DB_H__
#define __DB_STRD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;
class CDB_ELEM;

class CDB_STRD
{
public:
	CDB_STRD();
	virtual ~CDB_STRD();
	CDBDoc* m_pDoc;

public:
	T_STRD_K m_nStartNum;
	T_STRD_K m_nLastNum;

public:
	void Add(T_STRD_K Key,T_STRD_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem);
	BOOL Del(T_STRD_K Key,CDB_NODE* pNode,CDB_ELEM* pElem);

public:
	BOOL Get(T_STRD_K Key,T_STRD_D& rData);
		//{return m_strd.Lookup(Key,rData);}
	int GetCount();
		//{return m_strd.GetCount();}
	POSITION GetStart();
		//{return m_strd.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_STRD_K& rKey,T_STRD_D& rData);
		//{m_strd.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_STRD_K,T_STRD_K,T_STRD_D,T_STRD_D&>m_strd;
};

#endif
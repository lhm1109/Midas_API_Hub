#ifndef __DB_P1LA_DB_H__
#define __DB_P1LA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

#include "MultiKeyIndex.h"

class CDBDoc;
class CDB_ELEM;

class CDB_P1LA
{
public:
	CDB_P1LA();
	virtual ~CDB_P1LA();
	CDBDoc* m_pDoc;

public:
	T_P1LA_K m_nStartNum;
	T_P1LA_K m_nLastNum;

public:
	void Add(T_P1LA_K Key,T_P1LA_D& rData,CDB_P1LT* pP1lt,CDB_NODE* pNode,CDB_ELEM* pElem);
	BOOL Del(T_P1LA_K Key,CDB_P1LT* pP1lt,CDB_NODE* pNode,CDB_ELEM* pElem);

public:
	BOOL Get(T_P1LA_K Key,T_P1LA_D& rData);
		//{return m_p1la.Lookup(Key,rData);}
	int GetCount();
		//{return m_p1la.GetCount();}
	POSITION GetStart();
		//{return m_p1la.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_P1LA_K& rKey,T_P1LA_D& rData);
		//{m_p1la.GetNextAssoc(rNextPosition,rKey,rData);}

	T_P1LA_K GetKey(int nLoadItem, UINT NodeElemK)
	{
		return m_index.GetKey(T_UINT12UINT20_INDEX_K(nLoadItem, NodeElemK));
	}

protected:
	CMap<T_P1LA_K,T_P1LA_K,T_P1LA_D,T_P1LA_D&>m_p1la;
	CMultiKeyIndex<T_UINT12UINT20_INDEX_K, T_P1LA_K> m_index;
};

#endif


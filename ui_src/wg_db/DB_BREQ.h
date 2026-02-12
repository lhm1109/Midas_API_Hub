#ifndef __DB_BREQ_DB_H__
#define __DB_BREQ_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"
#include "MultiKeyIndex.h"   // for T_UINT2_INDEX_K

class CDBDoc;
class CDB_ELEM;
class CDB_NODE;

struct T_BREQ_K_LIST
{
	CArray<T_BREQ_K, T_BREQ_K> aBreqK;
	void Initialize()
	{
		aBreqK.RemoveAll();
	}
	T_BREQ_K_LIST()  { Initialize(); }
	T_BREQ_K_LIST(T_BREQ_K_LIST& src) {*this = src;}
	T_BREQ_K_LIST& operator=(T_BREQ_K_LIST& src)
	{
		aBreqK.Copy(src.aBreqK);
		return *this;
	}  
};

class CDB_BREQ
{
public:
	CDB_BREQ();
	virtual ~CDB_BREQ();
	CDBDoc* m_pDoc;

public:
	T_BREQ_K m_nStartNum;
	T_BREQ_K m_nLastNum;

public:
	void Add(T_BREQ_K Key,T_BREQ_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem);
	BOOL Del(T_BREQ_K Key,CDB_NODE* pNode,CDB_ELEM* pElem);

public:
	BOOL Get(T_BREQ_K Key,T_BREQ_D& rData);
	//{return m_breq.Lookup(Key,rData);}
	int GetCount();
	//{return m_breq.GetCount();}
	POSITION GetStart();
	//{return m_breq.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_BREQ_K& rKey,T_BREQ_D& rData);
	//{m_cmft.GetNextAssoc(rNextPosition,rKey,rData);}  
	BOOL GetBreqAssigned(T_ELEM_K KeyElem, CArray<T_BREQ_K, T_BREQ_K>& aKey);   // element가 속한 breq의 key를 찾아준다.
	BOOL GetBreqAssignedNode(T_NODE_K KeyNode, CArray<T_BREQ_K, T_BREQ_K>& aKey, CArray<T_BREQ_D, T_BREQ_D&>& aData);   // Node 속한 breq의 key와 데이터를 찾아준다.
	//{return m_elemtobreq.Lookup(KeyElem, rKeyBreq);}
protected:
	CMap<T_BREQ_K,T_BREQ_K,T_BREQ_D,T_BREQ_D&> m_breq;
	CMap<T_UINT2_INDEX_K,T_UINT2_INDEX_K&,T_BREQ_K,T_BREQ_K>  m_NodeElemToBreq; // NodeK와 ElemK를 key로 사용한다.
	CMap<T_ELEM_K,T_ELEM_K,T_BREQ_K_LIST,T_BREQ_K_LIST>  m_ElemToBreqList;
	CMap<T_NODE_K,T_NODE_K,T_BREQ_K_LIST,T_BREQ_K_LIST>  m_NodeToBreqList;
};

#endif


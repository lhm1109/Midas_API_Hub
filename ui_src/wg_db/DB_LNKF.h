#ifndef __DB_LNKF_DB_H__
#define __DB_LNKF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

#include "MultiKeyIndex.h"

class CDBDoc;

class CDB_LNKF
{
public:
	CDB_LNKF();
	virtual ~CDB_LNKF();
	CDBDoc* m_pDoc;

public:
	T_LNKF_K m_nStartNum;
	T_LNKF_K m_nLastNum;

public:
	void Add(T_LNKF_K Key,T_LNKF_D& rData,CDB_P1LT* pP1lt);
	BOOL Del(T_LNKF_K Key,CDB_P1LT* pP1lt);

public:
	BOOL Get(T_LNKF_K Key,T_LNKF_D& rData);
		//{return m_lnkf.Lookup(Key,rData);}
	int GetCount();
		//{return m_lnkf.GetCount();}
	POSITION GetStart();
		//{return m_lnkf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_LNKF_K& rKey,T_LNKF_D& rData);
		//{m_lnkf.GetNextAssoc(rNextPosition,rKey,rData);}

	T_LNKF_K GetKey(int nLinkType, UINT LinkK, int nFactorKind)
	{
		return m_index.GetKey(T_UINT3_INDEX_K(nLinkType, LinkK, nFactorKind, CHashKeySet::UINT3_2b22b8b));
	}

protected:
	CMap<T_LNKF_K,T_LNKF_K,T_LNKF_D,T_LNKF_D&>m_lnkf;
	CMultiKeyIndex<T_UINT3_INDEX_K, T_LNKF_K> m_index;
};

#endif


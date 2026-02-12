#ifndef __DB_EQMF_DB_H__
#define __DB_EQMF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

#include "MultiKeyIndex.h"

class CDBDoc;

class CDB_EQMF
{
public:
	CDB_EQMF();
	virtual ~CDB_EQMF();
	CDBDoc* m_pDoc;

public:
	T_EQMF_K m_nStartNum;
	T_EQMF_K m_nLastNum;

public:
	void Add(T_EQMF_K Key, T_EQMF_D& rData);
	BOOL Del(T_EQMF_K Key);

public:
	BOOL Get(T_EQMF_K Key, T_EQMF_D& rData);
		//{return m_eqmf.Lookup(1,rData);}
	int GetCount();
		//{return m_eqmf.GetCount();}
	POSITION GetStart();
		//{return m_eqmf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_EQMF_K& rKey,T_EQMF_D& rData);
		//{m_eqmf.GetNextAssoc(rNextPosition,rKey,rData);}

	T_EQMF_K GetKey(int nTgtType, UINT nTgtKey)
	{
		return m_index.GetKey(T_UINT12UINT20_INDEX_K(nTgtType, nTgtKey));
	}

protected:
	CMap<T_EQMF_K,T_EQMF_K,T_EQMF_D,T_EQMF_D&> m_eqmf;
	CMultiKeyIndex<T_UINT12UINT20_INDEX_K, T_EQMF_K> m_index;
};

#endif
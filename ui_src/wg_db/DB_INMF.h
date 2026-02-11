#ifndef __DB_INMF_DB_H__
#define __DB_INMF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

#include "MultiKeyIndex.h"

class CDBDoc;

class CDB_INMF
{
public:
	CDB_INMF();
	virtual ~CDB_INMF();
	CDBDoc* m_pDoc;

public:
	T_INMF_K m_nStartNum;
	T_INMF_K m_nLastNum;

public:
	void Add(T_INMF_K Key, T_INMF_D& rData);
	BOOL Del(T_INMF_K Key);

public:
	BOOL Get(T_INMF_K Key, T_INMF_D& rData);
		//{return m_inmf.Lookup(1,rData);}
	int GetCount();
		//{return m_inmf.GetCount();}
	POSITION GetStart();
		//{return m_inmf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_INMF_K& rKey,T_INMF_D& rData);
		//{m_inmf.GetNextAssoc(rNextPosition,rKey,rData);}

	T_INMF_K GetKey(int nTgtType, UINT nTgtKey)
	{
		return m_index.GetKey(T_UINT12UINT20_INDEX_K(nTgtType, nTgtKey));
	}

protected:
	CMap<T_INMF_K,T_INMF_K,T_INMF_D,T_INMF_D&> m_inmf;
	CMultiKeyIndex<T_UINT12UINT20_INDEX_K, T_INMF_K> m_index;
};

#endif
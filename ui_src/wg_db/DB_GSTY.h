#ifndef __DB_GSTY_DB_H__
#define __DB_GSTY_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

#include "MultiKeyIndex.h"

class CDBDoc;
class CDB_ELEM;

class CDB_GSTY
{
public:
	CDB_GSTY();
	virtual ~CDB_GSTY();
	CDBDoc* m_pDoc;

public:
	T_GSTY_K m_nStartNum;
	T_GSTY_K m_nLastNum;

public:
	void Add(T_GSTY_K Key,T_GSTY_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_GSTY_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_GSTY_K Key,T_GSTY_D& rData);
		//{return m_gsty.Lookup(Key,rData);}
	int GetCount();
		//{return m_gsty.GetCount();}
	POSITION GetStart();
		//{return m_gsty.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_GSTY_K& rKey,T_GSTY_D& rData);
		//{m_gsty.GetNextAssoc(rNextPosition,rKey,rData);}

	T_GSTY_K GetKey(int nTgtType, UINT nTgtKey)
	{
		return m_index.GetKey(T_UINT16UINT16_INDEX_K(nTgtType, nTgtKey));
	}

protected:
	CMap<T_GSTY_K,T_GSTY_K,T_GSTY_D,T_GSTY_D&>m_gsty;
	CMultiKeyIndex<T_UINT16UINT16_INDEX_K, T_GSTY_K> m_index;
};

#endif


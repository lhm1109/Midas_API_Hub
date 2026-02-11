#ifndef __DB_JIMP_DB_H__ 
#define __DB_JIMP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

#include "MultiKeyIndex.h"

class CDBDoc;
class CDB_ELEM;
class CDB_NODe;

class CDB_JIMP
{
public:
	CDB_JIMP();
	virtual ~CDB_JIMP();
	CDBDoc* m_pDoc;

public:
	T_JIMP_K m_nStartNum;
	T_JIMP_K m_nLastNum;

public:
	void Add(T_JIMP_K Key,T_JIMP_D& rData,CDB_ELEM* pElem,CDB_NODE* pNode);
	BOOL Del(T_JIMP_K Key,CDB_ELEM* pElem,CDB_NODE* pNode);

public:
	BOOL Get(T_JIMP_K Key,T_JIMP_D& rData);
		//{return m_jimp.Lookup(Key,rData);}
	int GetCount();
		//{return m_jimp.GetCount();}
	POSITION GetStart();
		//{return m_jimp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_JIMP_K& rKey,T_JIMP_D& rData);
		//{m_jimp.GetNextAssoc(rNextPosition,rKey,rData);}

// 	T_JIMP_K GetKey(int nTgtType, UINT nTgtKey)
// 	{
// 		return m_index.GetKey(T_UINT16UINT16_INDEX_K(nTgtType, nTgtKey)); 
// 	}
	T_JIMP_K GetKey(UINT nTgtType, UINT nTgtKey, UINT nFactorType)
	{
		return m_index.GetKey(T_UINT3_INDEX_K(nTgtType, nTgtKey, nFactorType, CHashKeySet::UINT3_2b22b8b));
	}

protected:
	CMap<T_JIMP_K,T_JIMP_K,T_JIMP_D,T_JIMP_D&>m_jimp;
	CMultiKeyIndex<T_UINT3_INDEX_K, T_JIMP_K> m_index;
};

#endif


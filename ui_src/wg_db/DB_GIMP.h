#ifndef __DB_GIMP_DB_H__
#define __DB_GIMP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

#include "MultiKeyIndex.h"

class CDBDoc;
class CDB_ELEM;
class CDB_NODe;

class CDB_GIMP
{
public:
	CDB_GIMP();
	virtual ~CDB_GIMP();
	CDBDoc* m_pDoc;

public:
	T_GIMP_K m_nStartNum;
	T_GIMP_K m_nLastNum;

public:
	void Add(T_GIMP_K Key,T_GIMP_D& rData,CDB_ELEM* pElem,CDB_NODE* pNode);
	BOOL Del(T_GIMP_K Key,CDB_ELEM* pElem,CDB_NODE* pNode);

public:
	BOOL Get(T_GIMP_K Key,T_GIMP_D& rData);
		//{return m_gimp.Lookup(Key,rData);}
	int GetCount();
		//{return m_gimp.GetCount();}
	POSITION GetStart();
		//{return m_gimp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_GIMP_K& rKey,T_GIMP_D& rData);
		//{m_gimp.GetNextAssoc(rNextPosition,rKey,rData);}

	T_GIMP_K GetKey(int nTgtType, UINT nTgtKey)
	{
		return m_index.GetKey(T_UINT16UINT16_INDEX_K(nTgtType, nTgtKey));
	}

protected:
	CMap<T_GIMP_K,T_GIMP_K,T_GIMP_D,T_GIMP_D&>m_gimp;
	CMultiKeyIndex<T_UINT16UINT16_INDEX_K, T_GIMP_K> m_index;
};

#endif


#ifndef __DB_PFSLU_DB_H__
#define __DB_PFSLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PFSLU
{
public:
	CDB_PFSLU()
	{
		m_mapData.InitHashTable(HASHSIZE_PFSL);
	}
	virtual ~CDB_PFSLU(){};

public:
	void Add(T_PFSL_K Key,T_PFSL_UDRD_D& rData)
		{m_mapData.SetAt(Key,rData);}
	BOOL Del(T_PFSL_K Key)
		{return m_mapData.RemoveKey(Key);}
	void DelAll()
		{m_mapData.RemoveAll();}
	BOOL Get(T_PFSL_K Key,T_PFSL_UDRD_D& rData)
		{return m_mapData.Lookup(Key,rData);}
	int GetCount()
		{return m_mapData.GetCount();}
	POSITION GetStart()
		{return m_mapData.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PFSL_K& rKey,T_PFSL_UDRD_D& rData)
		{m_mapData.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_PFSL_K,T_PFSL_K, T_PFSL_UDRD_D, T_PFSL_UDRD_D&> m_mapData;
};

#endif
#ifndef __DB_WNATU_DB_H__
#define __DB_WNATU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_WNATU
{
public:
	CDB_WNATU()
	{
		m_wnat.InitHashTable(HASHSIZEWNAT);
	}
	virtual ~CDB_WNATU(){};
	
public:
	void Add(T_UDRD_KEY Key,T_WNAT_UDRD_D& rData)
	{m_wnat.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_wnat.RemoveKey(Key);}
	void DelAll()
	{m_wnat.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_WNAT_UDRD_D& rData)
	{return m_wnat.Lookup(Key,rData);}
	int GetCount()
	{return m_wnat.GetCount();}
	POSITION GetStart()
	{return m_wnat.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_WNAT_UDRD_D& rData)
	{m_wnat.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_WNAT_UDRD_D,T_WNAT_UDRD_D&>m_wnat;
};

#endif
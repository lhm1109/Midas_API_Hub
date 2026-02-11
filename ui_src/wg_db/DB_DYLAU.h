#ifndef __DB_DYLAU_DB_H__
#define __DB_DYLAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DYLAU
{
public:
	CDB_DYLAU()
	{
		m_dyla.InitHashTable(HASHSIZEDYLA);
	}
	virtual ~CDB_DYLAU(){};

public:
	void Add(T_UDRD_KEY Key,T_DYLA_UDRD_D& rData)
	{m_dyla.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_dyla.RemoveKey(Key);}
	void DelAll()
	{m_dyla.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DYLA_UDRD_D& rData)
	{return m_dyla.Lookup(Key,rData);}
	int GetCount()
	{return m_dyla.GetCount();}
	POSITION GetStart()
	{return m_dyla.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DYLA_UDRD_D& rData)
	{m_dyla.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_DYLA_UDRD_D,T_DYLA_UDRD_D&>m_dyla;
};

#endif
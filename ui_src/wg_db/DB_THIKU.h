#ifndef __DB_THIKU_DB_H__
#define __DB_THIKU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_THIKU
{
public:
	CDB_THIKU()
	{
		m_thik.InitHashTable(HASHSIZETHIK);
	}
	virtual ~CDB_THIKU(){};

public:
	void Add(T_UDRD_KEY Key,T_THIK_UDRD_D& rData)
		{m_thik.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_thik.RemoveKey(Key);}
	void DelAll()
		{m_thik.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_THIK_UDRD_D& rData)
		{return m_thik.Lookup(Key,rData);}
	int GetCount()
		{return m_thik.GetCount();}
	POSITION GetStart()
		{return m_thik.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_THIK_UDRD_D& rData)
		{m_thik.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_THIK_UDRD_D,T_THIK_UDRD_D&>m_thik;
};

#endif
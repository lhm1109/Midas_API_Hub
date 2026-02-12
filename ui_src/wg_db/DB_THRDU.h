#ifndef __DB_THRDU_DB_H__
#define __DB_THRDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_THRDU
{
public:
	CDB_THRDU()
	{
		m_thrd.InitHashTable(HASHSIZETHRD);
	}
	virtual ~CDB_THRDU(){};

public:
	void Add(T_UDRD_KEY Key,T_THRD_UDRD_D& rData)
		{m_thrd.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_thrd.RemoveKey(Key);}
	void DelAll()
		{m_thrd.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_THRD_UDRD_D& rData)
		{return m_thrd.Lookup(Key,rData);}
	int GetCount()
		{return m_thrd.GetCount();}
	POSITION GetStart()
		{return m_thrd.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_THRD_UDRD_D& rData)
		{m_thrd.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_THRD_UDRD_D,T_THRD_UDRD_D&>m_thrd;
};

#endif
#ifndef __DB_RIVTU_DB_H__
#define __DB_RIVTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RIVTU
{
public:
	CDB_RIVTU()
	{
		m_rivt.InitHashTable(HASHSIZERIVT);
	}
	virtual ~CDB_RIVTU(){};

public:
	void Add(T_UDRD_KEY Key,T_RIVT_UDRD_D& rData)
		{m_rivt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rivt.RemoveKey(Key);}
	void DelAll()
		{m_rivt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RIVT_UDRD_D& rData)
		{return m_rivt.Lookup(Key,rData);}
	int GetCount()
		{return m_rivt.GetCount();}
	POSITION GetStart()
		{return m_rivt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RIVT_UDRD_D& rData)
		{m_rivt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RIVT_UDRD_D,T_RIVT_UDRD_D&>m_rivt;
};

#endif
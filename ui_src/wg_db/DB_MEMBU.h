#ifndef __DB_MEMBU_DB_H__
#define __DB_MEMBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MEMBU
{
public:
	CDB_MEMBU()
	{
		m_memb.InitHashTable(HASHSIZEMEMB);
	}
	virtual ~CDB_MEMBU(){};

public:
	void Add(T_UDRD_KEY Key,T_MEMB_UDRD_D& rData)
		{m_memb.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_memb.RemoveKey(Key);}
	void DelAll()
		{m_memb.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MEMB_UDRD_D& rData)
		{return m_memb.Lookup(Key,rData);}
	int GetCount()
		{return m_memb.GetCount();}
	POSITION GetStart()
		{return m_memb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MEMB_UDRD_D& rData)
		{m_memb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MEMB_UDRD_D,T_MEMB_UDRD_D&> m_memb;
};

#endif

#ifndef __DB_REBTU_DB_H__
#define __DB_REBTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_REBTU
{
public:
	CDB_REBTU()
	{
		m_rebt.InitHashTable(HASHSIZEREBT);
	}
	virtual ~CDB_REBTU(){};

public:
	void Add(T_UDRD_KEY Key,T_REBT_UDRD_D& rData)
		{m_rebt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rebt.RemoveKey(Key);}
	void DelAll()
		{m_rebt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_REBT_UDRD_D& rData)
		{return m_rebt.Lookup(Key,rData);}
	int GetCount()
		{return m_rebt.GetCount();}
	POSITION GetStart()
		{return m_rebt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_REBT_UDRD_D& rData)
		{m_rebt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_REBT_UDRD_D,T_REBT_UDRD_D&>m_rebt;
};

#endif
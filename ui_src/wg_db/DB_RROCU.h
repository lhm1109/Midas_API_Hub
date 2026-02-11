#ifndef __DB_RROCU_DB_H__
#define __DB_RROCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RROCU
{
public:
	CDB_RROCU()
	{
		m_rroc.InitHashTable(HASHSIZERROC);
	}
	virtual ~CDB_RROCU(){};

public:
	void Add(T_UDRD_KEY Key,T_RROC_UDRD_D& rData)
	{m_rroc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_rroc.RemoveKey(Key);}
	void DelAll()
	{m_rroc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RROC_UDRD_D& rData)
	{return m_rroc.Lookup(Key,rData);}
	int GetCount()
	{return m_rroc.GetCount();}
	POSITION GetStart()
	{return m_rroc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RROC_UDRD_D& rData)
	{m_rroc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_RROC_UDRD_D,T_RROC_UDRD_D&>m_rroc;
};

#endif
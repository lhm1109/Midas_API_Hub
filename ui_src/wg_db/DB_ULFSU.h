#ifndef __DB_ULFSU_DB_H__
#define __DB_ULFSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ULFSU
{
public:
	CDB_ULFSU()
	{
		m_ulfs.InitHashTable(HASHSIZEULFS);
	}
	virtual ~CDB_ULFSU(){};

public:
	void Add(T_UDRD_KEY Key,T_ULFS_UDRD_D& rData)
		{m_ulfs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ulfs.RemoveKey(Key);}
	void DelAll()
		{m_ulfs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ULFS_UDRD_D& rData)
		{return m_ulfs.Lookup(Key,rData);}
	int GetCount()
		{return m_ulfs.GetCount();}
	POSITION GetStart()
		{return m_ulfs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_ULFS_UDRD_D& rData)
		{m_ulfs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_ULFS_UDRD_D,T_ULFS_UDRD_D&>m_ulfs;
};

#endif
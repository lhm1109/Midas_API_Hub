#ifndef __DB_LDFSU_DB_H__
#define __DB_LDFSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LDFSU
{
public:
	CDB_LDFSU()
	{
		m_ldfs.InitHashTable(HASHSIZELDFS);
	}
	virtual ~CDB_LDFSU(){};

public:
	void Add(T_UDRD_KEY Key,T_LDFS_UDRD_D& rData)
		{m_ldfs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ldfs.RemoveKey(Key);}
	void DelAll()
		{m_ldfs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LDFS_UDRD_D& rData)
		{return m_ldfs.Lookup(Key,rData);}
	int GetCount()
		{return m_ldfs.GetCount();}
	POSITION GetStart()
		{return m_ldfs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LDFS_UDRD_D& rData)
		{m_ldfs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LDFS_UDRD_D,T_LDFS_UDRD_D&>m_ldfs;
};

#endif
#ifndef __DB_MDFSU_DB_H__
#define __DB_MDFSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MDFSU
{
public:
	CDB_MDFSU()
	{
		m_mdfs.InitHashTable(HASHSIZEMDFS);
	}
	virtual ~CDB_MDFSU(){};

public:
	void Add(T_UDRD_KEY Key,T_MDFS_UDRD_D& rData)
		{m_mdfs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mdfs.RemoveKey(Key);}
	void DelAll()
		{m_mdfs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MDFS_UDRD_D& rData)
		{return m_mdfs.Lookup(Key,rData);}
	int GetCount()
		{return m_mdfs.GetCount();}
	POSITION GetStart()
		{return m_mdfs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MDFS_UDRD_D& rData)
		{m_mdfs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MDFS_UDRD_D,T_MDFS_UDRD_D&>m_mdfs;
};

#endif
#ifndef __DB_STORU_DB_H__
#define __DB_STORU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STORU
{
public:
	CDB_STORU()
	{
		m_stor.InitHashTable(HASHSIZESTOR);
	}
	virtual ~CDB_STORU(){};

public:
	void Add(T_UDRD_KEY Key,T_STOR_UDRD_D& rData)
		{m_stor.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_stor.RemoveKey(Key);}
	void DelAll()
		{m_stor.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_STOR_UDRD_D& rData)
		{return m_stor.Lookup(Key,rData);}
	int GetCount()
		{return m_stor.GetCount();}
	POSITION GetStart()
		{return m_stor.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_STOR_UDRD_D& rData)
		{m_stor.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STOR_UDRD_D,T_STOR_UDRD_D&>m_stor;
};

#endif
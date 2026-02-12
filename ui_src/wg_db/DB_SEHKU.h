#ifndef __DB_SEHKU_DB_H__
#define __DB_SEHKU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_SEHKU
{
public:
	CDB_SEHKU()
	{
		m_SEHK.InitHashTable(HASHSIZESEHK);
	}
	virtual ~CDB_SEHKU(){};

public:
	void Add(T_UDRD_KEY Key,T_SEHK_UDRD_D& rData)
	{m_SEHK.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_SEHK.RemoveKey(Key);}
	void DelAll()
	{m_SEHK.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SEHK_UDRD_D& rData)
	{return m_SEHK.Lookup(Key,rData);}
	int GetCount()
	{return m_SEHK.GetCount();}
	POSITION GetStart()
	{return m_SEHK.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SEHK_UDRD_D& rData)
	{m_SEHK.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_SEHK_UDRD_D,T_SEHK_UDRD_D&>m_SEHK;
};

#endif

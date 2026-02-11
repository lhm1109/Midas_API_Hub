#ifndef __DB_GUELU_DB_H__
#define __DB_GUELU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GUELU
{
public:
	CDB_GUELU()
	{
		m_guel.InitHashTable(HASHSIZEGUEL);
	}
	virtual ~CDB_GUELU(){};

public:
	void Add(T_UDRD_KEY Key,T_GUEL_UDRD_D& rData)
		{m_guel.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_guel.RemoveKey(Key);}
	void DelAll()
		{m_guel.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GUEL_UDRD_D& rData)
		{return m_guel.Lookup(Key,rData);}
	int GetCount()
		{return m_guel.GetCount();}
	POSITION GetStart()
		{return m_guel.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GUEL_UDRD_D& rData)
		{m_guel.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GUEL_UDRD_D,T_GUEL_UDRD_D&>m_guel;
};

#endif
#ifndef __DB_FBLAU_DB_H__
#define __DB_FBLAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_FBLAU
{
public:
	CDB_FBLAU()
	{
		m_fbla.InitHashTable(HASHSIZEFBLA);
	}
	virtual ~CDB_FBLAU(){};

public:
	void Add(T_UDRD_KEY Key,T_FBLA_UDRD_D& rData)
		{m_fbla.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_fbla.RemoveKey(Key);}
	void DelAll()
		{m_fbla.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_FBLA_UDRD_D& rData)
		{return m_fbla.Lookup(Key,rData);}
	int GetCount()
		{return m_fbla.GetCount();}
	POSITION GetStart()
		{return m_fbla.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_FBLA_UDRD_D& rData)
		{m_fbla.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_FBLA_UDRD_D,T_FBLA_UDRD_D&>m_fbla;
};

#endif
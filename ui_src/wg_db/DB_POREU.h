#ifndef __DB_POREU_DB_H__
#define __DB_POREU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_POREU
{
public:
	CDB_POREU()
	{
		m_pore.InitHashTable(HASHSIZEPORE);
	}
	virtual ~CDB_POREU(){};

public:
	void Add(T_UDRD_KEY Key,T_PORE_UDRD_D& rData)
		{m_pore.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pore.RemoveKey(Key);}
	void DelAll()
		{m_pore.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PORE_UDRD_D& rData)
		{return m_pore.Lookup(Key,rData);}
	int GetCount()
		{return m_pore.GetCount();}
	POSITION GetStart()
		{return m_pore.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PORE_UDRD_D& rData)
		{m_pore.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PORE_UDRD_D,T_PORE_UDRD_D&>m_pore;
};

#endif
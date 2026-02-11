#ifndef __DB_OFFSU_DB_H__
#define __DB_OFFSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_OFFSU
{
public:
	CDB_OFFSU()
	{
		m_offs.InitHashTable(HASHSIZEOFFS);
	}
	virtual ~CDB_OFFSU(){};

public:
	void Add(T_UDRD_KEY Key,T_OFFS_UDRD_D& rData)
		{m_offs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_offs.RemoveKey(Key);}
	void DelAll()
		{m_offs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_OFFS_UDRD_D& rData)
		{return m_offs.Lookup(Key,rData);}
	int GetCount()
		{return m_offs.GetCount();}
	POSITION GetStart()
		{return m_offs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_OFFS_UDRD_D& rData)
		{m_offs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_OFFS_UDRD_D,T_OFFS_UDRD_D&>m_offs;
};

#endif
#ifndef __DB_SLRSU_DB_H__
#define __DB_SLRSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SLRSU
{
public:
	CDB_SLRSU()
	{
		m_slrs.InitHashTable(HASHSIZESLRS);
	}
	virtual ~CDB_SLRSU(){};

public:
	void Add(T_UDRD_KEY Key,T_SLRS_UDRD_D& rData)
		{m_slrs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_slrs.RemoveKey(Key);}
	void DelAll()
		{m_slrs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SLRS_UDRD_D& rData)
		{return m_slrs.Lookup(Key,rData);}
	int GetCount()
		{return m_slrs.GetCount();}
	POSITION GetStart()
		{return m_slrs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SLRS_UDRD_D& rData)
		{m_slrs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SLRS_UDRD_D,T_SLRS_UDRD_D&>m_slrs;
};

#endif
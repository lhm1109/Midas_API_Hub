#ifndef __DB_NSPRU_DB_H__
#define __DB_NSPRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_NSPRU
{
public:
	CDB_NSPRU()
	{
		m_nspr.InitHashTable(HASHSIZENSPR);
	}
	virtual ~CDB_NSPRU(){};

public:
	void Add(T_UDRD_KEY Key,T_NSPR_UDRD_D& rData)
		{m_nspr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_nspr.RemoveKey(Key);}
	void DelAll()
		{m_nspr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_NSPR_UDRD_D& rData)
		{return m_nspr.Lookup(Key,rData);}
	int GetCount()
		{return m_nspr.GetCount();}
	POSITION GetStart()
		{return m_nspr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_NSPR_UDRD_D& rData)
		{m_nspr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_NSPR_UDRD_D,T_NSPR_UDRD_D&>m_nspr;
};

#endif
#ifndef __DB_FIBRU_DB_H__
#define __DB_FIBRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_FIBRU
{
public:
	CDB_FIBRU()
	{
		m_fibr.InitHashTable(HASHSIZEFIBR);
	}
	virtual ~CDB_FIBRU(){};

public:
	void Add(T_UDRD_KEY Key,T_FIBR_UDRD_D& rData)
		{m_fibr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_fibr.RemoveKey(Key);}
	void DelAll()
		{m_fibr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_FIBR_UDRD_D& rData)
		{return m_fibr.Lookup(Key,rData);}
	int GetCount()
		{return m_fibr.GetCount();}
	POSITION GetStart()
		{return m_fibr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_FIBR_UDRD_D& rData)
		{m_fibr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_FIBR_UDRD_D,T_FIBR_UDRD_D&>m_fibr;
};

#endif
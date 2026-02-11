#ifndef __DB_THCRU_DB_H__
#define __DB_THCRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_THCRU
{
public:
	CDB_THCRU()
	{
		m_thcr.InitHashTable(HASHSIZETHCR);
	}
	virtual ~CDB_THCRU(){};

public:
	void Add(T_UDRD_KEY Key,T_THCR_UDRD_D& rData)
		{m_thcr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_thcr.RemoveKey(Key);}
	void DelAll()
		{m_thcr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_THCR_UDRD_D& rData)
		{return m_thcr.Lookup(Key,rData);}
	int GetCount()
		{return m_thcr.GetCount();}
	POSITION GetStart()
		{return m_thcr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_THCR_UDRD_D& rData)
		{m_thcr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_THCR_UDRD_D,T_THCR_UDRD_D&>m_thcr;
};

#endif
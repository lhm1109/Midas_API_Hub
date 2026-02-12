#ifndef __DB_BRPRU_DB_H__
#define __DB_BRPRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BRPRU
{
public:
	CDB_BRPRU()
	{
		m_brpr.InitHashTable(HASHSIZEBRPR);
	}
	virtual ~CDB_BRPRU(){};

public:
	void Add(T_UDRD_KEY Key,T_BRPR_UDRD_D& rData)
	{m_brpr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_brpr.RemoveKey(Key);}
	void DelAll()
	{m_brpr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_BRPR_UDRD_D& rData)
	{return m_brpr.Lookup(Key,rData);}
	int GetCount()
	{return m_brpr.GetCount();}
	POSITION GetStart()
	{return m_brpr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_BRPR_UDRD_D& rData)
	{m_brpr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_BRPR_UDRD_D,T_BRPR_UDRD_D&>m_brpr;
};

#endif


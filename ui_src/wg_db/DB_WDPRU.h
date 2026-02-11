#ifndef __DB_WDPRU_DB_H__
#define __DB_WDPRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_WDPRU
{
public:
	CDB_WDPRU()
	{
		m_wdpr.InitHashTable(HASHSIZEWDPR);
	}
	virtual ~CDB_WDPRU(){};

public:
	void Add(T_UDRD_KEY Key,T_WDPR_UDRD_D& rData)
		{m_wdpr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_wdpr.RemoveKey(Key);}
	void DelAll()
		{m_wdpr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_WDPR_UDRD_D& rData)
		{return m_wdpr.Lookup(Key,rData);}
	int GetCount()
		{return m_wdpr.GetCount();}
	POSITION GetStart()
		{return m_wdpr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_WDPR_UDRD_D& rData)
		{m_wdpr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_WDPR_UDRD_D,T_WDPR_UDRD_D&>m_wdpr;
};

#endif
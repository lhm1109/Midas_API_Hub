#ifndef __DB_ARPRU_DB_H__
#define __DB_ARPRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ARPRU
{
public:
	CDB_ARPRU()
	{
		m_arpr.InitHashTable(HASHSIZEARPR);
	}
	virtual ~CDB_ARPRU(){};

public:
	void Add(T_UDRD_KEY Key,T_ARPR_UDRD_D& rData)
		{m_arpr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_arpr.RemoveKey(Key);}
	void DelAll()
		{m_arpr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ARPR_UDRD_D& rData)
		{return m_arpr.Lookup(Key,rData);}
	int GetCount()
		{return m_arpr.GetCount();}
	POSITION GetStart()
		{return m_arpr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_ARPR_UDRD_D& rData)
		{m_arpr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_ARPR_UDRD_D,T_ARPR_UDRD_D&>m_arpr;
};

#endif
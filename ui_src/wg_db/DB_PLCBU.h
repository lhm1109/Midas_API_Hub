#ifndef __DB_PLCBU_DB_H__
#define __DB_PLCBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PLCBU
{
public:
	CDB_PLCBU()
	{
		m_plcb.InitHashTable(HASHSIZEPLCB);
	}
	virtual ~CDB_PLCBU(){};

public:
	void Add(T_UDRD_KEY Key,T_PLCB_UDRD_D& rData)
		{m_plcb.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_plcb.RemoveKey(Key);}
	void DelAll()
		{m_plcb.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PLCB_UDRD_D& rData)
		{return m_plcb.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_PLCB_UDRD_D,T_PLCB_UDRD_D&>m_plcb;
};

#endif
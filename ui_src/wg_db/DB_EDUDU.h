#ifndef __DB_EDUDU_DB_H__
#define __DB_EDUDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_EDUDU
{
public:
	CDB_EDUDU()
	{
		m_EDUD.InitHashTable(HASHSIZEEDUD);
	}
	virtual ~CDB_EDUDU(){};

public:
	void Add(T_UDRD_KEY Key,T_EDUD_UDRD_D& rData)
	{m_EDUD.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_EDUD.RemoveKey(Key);}
	void DelAll()
	{m_EDUD.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_EDUD_UDRD_D& rData)
	{return m_EDUD.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_EDUD_UDRD_D,T_EDUD_UDRD_D&> m_EDUD;
};

#endif
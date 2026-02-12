#ifndef __DB_DASCU_DB_H__
#define __DB_DASCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DASCU
{
public:
	CDB_DASCU()
	{
		m_dasc.InitHashTable(HASHSIZEDASC);
	}
	virtual ~CDB_DASCU(){};

public:
	void Add(T_UDRD_KEY Key,T_DASC_UDRD_D& rData)
		{m_dasc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dasc.RemoveKey(Key);}
	void DelAll()
		{m_dasc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DASC_UDRD_D& rData)
		{return m_dasc.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_DASC_UDRD_D,T_DASC_UDRD_D&>m_dasc;
};

#endif
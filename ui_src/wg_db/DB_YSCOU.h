#ifndef __DB_YSCOU_DB_H__
#define __DB_YSCOU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_YSCOU
{
public:
	CDB_YSCOU()
	{
		m_ysco.InitHashTable(HASHSIZEYSCO);
	}
	virtual ~CDB_YSCOU(){};

public:
	void Add(T_UDRD_KEY Key,T_YSCO_UDRD_D& rData)
		{m_ysco.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ysco.RemoveKey(Key);}
	void DelAll()
		{m_ysco.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_YSCO_UDRD_D& rData)
		{return m_ysco.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_YSCO_UDRD_D,T_YSCO_UDRD_D&>m_ysco;
};

#endif
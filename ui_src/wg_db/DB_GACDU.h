#ifndef __DB_GACDU_DB_H__
#define __DB_GACDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GACDU
{
public:
	CDB_GACDU()
	{
		m_GACD.InitHashTable(HASHSIZEGACD);
	}
	virtual ~CDB_GACDU(){};

public:
	void Add(T_UDRD_KEY Key,T_GACD_UDRD_D& rData)
	{m_GACD.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_GACD.RemoveKey(Key);}
	void DelAll()
	{m_GACD.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GACD_UDRD_D& rData)
	{return m_GACD.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_GACD_UDRD_D,T_GACD_UDRD_D&>m_GACD;
};

#endif
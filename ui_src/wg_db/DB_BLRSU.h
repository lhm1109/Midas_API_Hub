#ifndef __DB_BLRSU_DB_H__
#define __DB_BLRSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BLRSU
{
public:
	CDB_BLRSU()
	{
		m_blrs.InitHashTable(HASHSIZEBLRS);
	}
	virtual ~CDB_BLRSU(){};

public:
	void Add(T_UDRD_KEY Key,T_BLRS_UDRD_D& rData)
		{m_blrs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_blrs.RemoveKey(Key);}
	void DelAll()
		{m_blrs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_BLRS_UDRD_D& rData)
		{return m_blrs.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_BLRS_UDRD_D,T_BLRS_UDRD_D&>m_blrs;
};

#endif
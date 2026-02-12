#ifndef __DB_POGDU_DB_H__
#define __DB_POGDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_POGDU
{
public:
	CDB_POGDU()
	{
		m_pogd.InitHashTable(HASHSIZEPOGD);
	}
	virtual ~CDB_POGDU(){};

public:
	void Add(T_UDRD_KEY Key,T_POGD_UDRD_D& rData)
		{m_pogd.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pogd.RemoveKey(Key);}
	void DelAll()
		{m_pogd.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_POGD_UDRD_D& rData)
		{return m_pogd.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_POGD_UDRD_D,T_POGD_UDRD_D&>m_pogd;
};

#endif
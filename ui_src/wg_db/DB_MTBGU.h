#ifndef __DB_MTBGU_DB_H__
#define __DB_MTBGU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MTBGU
{
public:
	CDB_MTBGU()
	{
		m_mtbg.InitHashTable(HASHSIZEMTBG);
	}
	virtual ~CDB_MTBGU(){};

public:
	void Add(T_UDRD_KEY Key,T_MTBG_UDRD_D& rData)
		{m_mtbg.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mtbg.RemoveKey(Key);}
	void DelAll()
		{m_mtbg.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MTBG_UDRD_D& rData)
		{return m_mtbg.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MTBG_UDRD_D,T_MTBG_UDRD_D&>m_mtbg;
};

#endif
#ifndef __DB_JDDSU_DB_H__
#define __DB_JDDSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDDSU
{
public:
	CDB_JDDSU()
	{
		m_JDDS.InitHashTable(HASHSIZEJDDS);
	}
	virtual ~CDB_JDDSU(){};

public:
	void Add(T_UDRD_KEY Key,T_JDDS_UDRD_D& rData)
		{m_JDDS.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_JDDS.RemoveKey(Key);}
	void DelAll()
		{m_JDDS.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JDDS_UDRD_D& rData)
		{return m_JDDS.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_JDDS_UDRD_D,T_JDDS_UDRD_D&> m_JDDS;
};

#endif
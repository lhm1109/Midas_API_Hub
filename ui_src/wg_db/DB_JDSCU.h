#ifndef __DB_JDSCU_DB_H__
#define __DB_JDSCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDSCU
{
public:
	CDB_JDSCU()
	{
		m_JDSC.InitHashTable(HASHSIZEJDSC);
	}
	virtual ~CDB_JDSCU(){};

public:
	void Add(T_UDRD_KEY Key,T_JDSC_UDRD_D& rData)
		{m_JDSC.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_JDSC.RemoveKey(Key);}
	void DelAll()
		{m_JDSC.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JDSC_UDRD_D& rData)
		{return m_JDSC.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_JDSC_UDRD_D,T_JDSC_UDRD_D&> m_JDSC;
};

#endif
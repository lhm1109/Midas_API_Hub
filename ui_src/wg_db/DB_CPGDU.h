#ifndef __DB_CPGDU_DB_H__
#define __DB_CPGDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CPGDU
{
public:
	CDB_CPGDU()
	{
		m_cpgd.InitHashTable(HASHSIZECPGD);
	}
	virtual ~CDB_CPGDU(){};

public:
	void Add(T_UDRD_KEY Key,T_CPGD_UDRD_D& rData)
		{m_cpgd.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cpgd.RemoveKey(Key);}
	void DelAll()
		{m_cpgd.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CPGD_UDRD_D& rData)
		{return m_cpgd.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CPGD_UDRD_D,T_CPGD_UDRD_D&>m_cpgd;
};

#endif
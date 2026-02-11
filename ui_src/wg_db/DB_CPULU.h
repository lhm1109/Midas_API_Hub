#ifndef __DB_CPULU_DB_H__
#define __DB_CPULU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CPULU
{
public:
	CDB_CPULU()
	{
		m_cpul.InitHashTable(HASHSIZECPUL);
	}
	virtual ~CDB_CPULU(){};

public:
	void Add(T_UDRD_KEY Key,T_CPUL_UDRD_D& rData)
		{m_cpul.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cpul.RemoveKey(Key);}
	void DelAll()
		{m_cpul.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CPUL_UDRD_D& rData)
		{return m_cpul.Lookup(Key,rData);}
	int GetCount()
		{return m_cpul.GetCount();}
	POSITION GetStart()
		{return m_cpul.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CPUL_UDRD_D& rData)
		{m_cpul.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CPUL_UDRD_D,T_CPUL_UDRD_D&>m_cpul;
};

#endif
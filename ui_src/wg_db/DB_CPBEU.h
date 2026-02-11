#ifndef __DB_CPBEU_DB_H__
#define __DB_CPBEU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CPBEU
{
public:
	CDB_CPBEU()
	{
		m_cpbe.InitHashTable(HASHSIZECPBE);
	}
	virtual ~CDB_CPBEU(){};

public:
	void Add(T_UDRD_KEY Key,T_CPBE_UDRD_D& rData)
		{m_cpbe.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cpbe.RemoveKey(Key);}
	void DelAll()
		{m_cpbe.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CPBE_UDRD_D& rData)
		{return m_cpbe.Lookup(Key,rData);}
	int GetCount()
		{return m_cpbe.GetCount();}
	POSITION GetStart()
		{return m_cpbe.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CPBE_UDRD_D& rData)
		{m_cpbe.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CPBE_UDRD_D,T_CPBE_UDRD_D&>m_cpbe;
};

#endif
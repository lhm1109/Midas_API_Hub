#ifndef __DB_CPPLU_DB_H__
#define __DB_CPPLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CPPLU
{
public:
	CDB_CPPLU()
	{
		m_cppl.InitHashTable(HASHSIZECPPL);
	}
	virtual ~CDB_CPPLU(){};

public:
	void Add(T_UDRD_KEY Key,T_CPPL_UDRD_D& rData)
		{m_cppl.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cppl.RemoveKey(Key);}
	void DelAll()
		{m_cppl.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CPPL_UDRD_D& rData)
		{return m_cppl.Lookup(Key,rData);}
	int GetCount()
		{return m_cppl.GetCount();}
	POSITION GetStart()
		{return m_cppl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CPPL_UDRD_D& rData)
		{m_cppl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CPPL_UDRD_D,T_CPPL_UDRD_D&>m_cppl;
};

#endif
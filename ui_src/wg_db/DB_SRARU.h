#ifndef __DB_SRARU_DB_H__
#define __DB_SRARU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SRARU
{
public:
	CDB_SRARU()
	{
		m_srar.InitHashTable(HASHSIZESRAR);
	}
	virtual ~CDB_SRARU(){};

public:
	void Add(T_UDRD_KEY Key,T_SRAR_UDRD_D& rData)
		{m_srar.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_srar.RemoveKey(Key);}
	void DelAll()
		{m_srar.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SRAR_UDRD_D& rData)
		{return m_srar.Lookup(Key,rData);}
	int GetCount()
		{return m_srar.GetCount();}
	POSITION GetStart()
		{return m_srar.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SRAR_UDRD_D& rData)
		{m_srar.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SRAR_UDRD_D,T_SRAR_UDRD_D&>m_srar;
};

#endif
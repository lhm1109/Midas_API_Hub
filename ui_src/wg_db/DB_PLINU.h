#ifndef __DB_PLINU_DB_H__
#define __DB_PLINU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PLINU
{
public:
	CDB_PLINU()
	{
		m_plin.InitHashTable(HASHSIZEPLIN);
	}
	virtual ~CDB_PLINU(){};

public:
	void Add(T_UDRD_KEY Key,T_PLIN_UDRD_D& rData)
		{m_plin.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_plin.RemoveKey(Key);}
	void DelAll()
		{m_plin.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PLIN_UDRD_D& rData)
		{return m_plin.Lookup(Key,rData);}
	int GetCount()
		{return m_plin.GetCount();}
	POSITION GetStart()
		{return m_plin.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PLIN_UDRD_D& rData)
		{m_plin.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PLIN_UDRD_D,T_PLIN_UDRD_D&>m_plin;
};

#endif
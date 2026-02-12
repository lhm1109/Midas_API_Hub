#ifndef __DB_SUEQU_DB_H__
#define __DB_SUEQU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SUEQU
{
public:
	CDB_SUEQU()
	{
		m_sueq.InitHashTable(HASHSIZESUEQ);
	}
	virtual ~CDB_SUEQU(){};

public:
	void Add(T_UDRD_KEY Key,T_SUEQ_UDRD_D& rData)
		{m_sueq.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sueq.RemoveKey(Key);}
	void DelAll()
		{m_sueq.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SUEQ_UDRD_D& rData)
		{return m_sueq.Lookup(Key,rData);}
	int GetCount()
		{return m_sueq.GetCount();}
	POSITION GetStart()
		{return m_sueq.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SUEQ_UDRD_D& rData)
		{m_sueq.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SUEQ_UDRD_D,T_SUEQ_UDRD_D&>m_sueq;
};

#endif
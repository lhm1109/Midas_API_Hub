#ifndef __DB_MPCTU_DB_H__
#define __DB_MPCTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MPCTU
{
public:
	CDB_MPCTU()
	{
		m_mpct.InitHashTable(HASHSIZEMPCT);
	}
	virtual ~CDB_MPCTU(){};

public:
	void Add(T_UDRD_KEY Key,T_MPCT_UDRD_D& rData)
		{m_mpct.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mpct.RemoveKey(Key);}
	void DelAll()
		{m_mpct.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MPCT_UDRD_D& rData)
		{return m_mpct.Lookup(Key,rData);}
	int GetCount()
		{return m_mpct.GetCount();}
	POSITION GetStart()
		{return m_mpct.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MPCT_UDRD_D& rData)
		{m_mpct.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MPCT_UDRD_D,T_MPCT_UDRD_D&>m_mpct;
};

#endif
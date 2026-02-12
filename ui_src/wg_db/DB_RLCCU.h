#ifndef __DB_RLCCU_DB_H__
#define __DB_RLCCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RLCCU
{
public:
	CDB_RLCCU()
	{
		m_rlcc.InitHashTable(HASHSIZERLCC);
	}
	virtual ~CDB_RLCCU(){};

public:
	void Add(T_UDRD_KEY Key,T_RLCC_UDRD_D& rData)
		{m_rlcc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rlcc.RemoveKey(Key);}
	void DelAll()
		{m_rlcc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RLCC_UDRD_D& rData)
		{return m_rlcc.Lookup(Key,rData);}
	int GetCount()
		{return m_rlcc.GetCount();}
	POSITION GetStart()
		{return m_rlcc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RLCC_UDRD_D& rData)
		{m_rlcc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RLCC_UDRD_D,T_RLCC_UDRD_D&>m_rlcc;
};

#endif
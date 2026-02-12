#ifndef __DB_DCBCU_DB_H__
#define __DB_DCBCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DCBCU
{
public:
	CDB_DCBCU()
	{
		m_dcbc.InitHashTable(HASHSIZEDCBC);
	}
	virtual ~CDB_DCBCU(){};

public:
	void Add(T_UDRD_KEY Key,T_DCBC_UDRD_D& rData)
		{m_dcbc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dcbc.RemoveKey(Key);}
	void DelAll()
		{m_dcbc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DCBC_UDRD_D& rData)
		{return m_dcbc.Lookup(Key,rData);}
	int GetCount()
		{return m_dcbc.GetCount();}
	POSITION GetStart()
		{return m_dcbc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DCBC_UDRD_D& rData)
		{m_dcbc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DCBC_UDRD_D,T_DCBC_UDRD_D&>m_dcbc;
};

#endif
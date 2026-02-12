#ifndef __DB_RBSTU_DB_H__
#define __DB_RBSTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RBSTU
{
public:
	CDB_RBSTU()
	{
		m_rbst.InitHashTable(HASHSIZERBST);
	}
	virtual ~CDB_RBSTU(){};

public:
	void Add(T_UDRD_KEY Key,T_RBST_UDRD_D& rData)
		{m_rbst.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rbst.RemoveKey(Key);}
	void DelAll()
		{m_rbst.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RBST_UDRD_D& rData)
		{return m_rbst.Lookup(Key,rData);}
	int GetCount()
		{return m_rbst.GetCount();}
	POSITION GetStart()
		{return m_rbst.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RBST_UDRD_D& rData)
		{m_rbst.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RBST_UDRD_D,T_RBST_UDRD_D&>m_rbst;
};

#endif
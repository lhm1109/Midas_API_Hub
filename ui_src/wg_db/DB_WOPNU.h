#ifndef __DB_WOPNU_DB_H__
#define __DB_WOPNU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_WOPNU
{
public:
	CDB_WOPNU()
	{
		m_wopn.InitHashTable(HASHSIZEWOPN);
	}
	virtual ~CDB_WOPNU(){};

public:
	void Add(T_UDRD_KEY Key,T_WOPN_UDRD_D& rData)
		{m_wopn.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_wopn.RemoveKey(Key);}
	void DelAll()
		{m_wopn.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_WOPN_UDRD_D& rData)
		{return m_wopn.Lookup(Key,rData);}
	int GetCount()
		{return m_wopn.GetCount();}
	POSITION GetStart()
		{return m_wopn.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_WOPN_UDRD_D& rData)
		{m_wopn.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_WOPN_UDRD_D,T_WOPN_UDRD_D&>m_wopn;
};

#endif
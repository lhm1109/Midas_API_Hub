#ifndef __DB_LLANU_DB_H__
#define __DB_LLANU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LLANU
{
public:
	CDB_LLANU()
	{
		m_llan.InitHashTable(HASHSIZELLAN);
	}
	virtual ~CDB_LLANU(){};

public:
	void Add(T_UDRD_KEY Key,T_LLAN_UDRD_D& rData)
		{m_llan.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_llan.RemoveKey(Key);}
	void DelAll()
		{m_llan.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LLAN_UDRD_D& rData)
		{return m_llan.Lookup(Key,rData);}
	int GetCount()
		{return m_llan.GetCount();}
	POSITION GetStart()
		{return m_llan.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LLAN_UDRD_D& rData)
		{m_llan.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LLAN_UDRD_D,T_LLAN_UDRD_D&>m_llan;
};

#endif

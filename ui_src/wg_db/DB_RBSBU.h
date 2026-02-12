#ifndef __DB_RBSBU_DB_H__
#define __DB_RBSBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RBSBU
{
public:
	CDB_RBSBU()
	{
		m_rbsb.InitHashTable(HASHSIZERBSB);
	}
	virtual ~CDB_RBSBU(){};

public:
	void Add(T_UDRD_KEY Key,T_RBSB_UDRD_D& rData)
		{m_rbsb.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rbsb.RemoveKey(Key);}
	void DelAll()
		{m_rbsb.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RBSB_UDRD_D& rData)
		{return m_rbsb.Lookup(Key,rData);}
	int GetCount()
		{return m_rbsb.GetCount();}
	POSITION GetStart()
		{return m_rbsb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RBSB_UDRD_D& rData)
		{m_rbsb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RBSB_UDRD_D,T_RBSB_UDRD_D&>m_rbsb;
};

#endif
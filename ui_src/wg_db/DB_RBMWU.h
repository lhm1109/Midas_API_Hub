#ifndef __DB_RBMWU_DB_H__
#define __DB_RBMWU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RBMWU
{
public:
	CDB_RBMWU()
	{
		m_rbmw.InitHashTable(HASHSIZERBMW);
	}
	virtual ~CDB_RBMWU(){};

public:
	void Add(T_UDRD_KEY Key,T_RBMW_UDRD_D& rData)
		{m_rbmw.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rbmw.RemoveKey(Key);}
	void DelAll()
		{m_rbmw.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RBMW_UDRD_D& rData)
		{return m_rbmw.Lookup(Key,rData);}
	int GetCount()
		{return m_rbmw.GetCount();}
	POSITION GetStart()
		{return m_rbmw.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RBMW_UDRD_D& rData)
		{m_rbmw.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RBMW_UDRD_D,T_RBMW_UDRD_D&>m_rbmw;
};

#endif
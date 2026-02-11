#ifndef __DB_RROVU_DB_H__
#define __DB_RROVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RROVU
{
public:
	CDB_RROVU()
	{
		m_rrov.InitHashTable(HASHSIZERROV);
	}
	virtual ~CDB_RROVU(){};

public:
	void Add(T_UDRD_KEY Key,T_RROV_UDRD_D& rData)
		{m_rrov.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rrov.RemoveKey(Key);}
	void DelAll()
		{m_rrov.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RROV_UDRD_D& rData)
		{return m_rrov.Lookup(Key,rData);}
	int GetCount()
		{return m_rrov.GetCount();}
	POSITION GetStart()
		{return m_rrov.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RROV_UDRD_D& rData)
		{m_rrov.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RROV_UDRD_D,T_RROV_UDRD_D&>m_rrov;
};

#endif
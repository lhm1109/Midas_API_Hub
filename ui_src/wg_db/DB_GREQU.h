#ifndef __DB_GREQU_DB_H__
#define __DB_GREQU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GREQU
{
public:
	CDB_GREQU()
	{
		m_greq.InitHashTable(HASHSIZEGREQ);
	}
	virtual ~CDB_GREQU(){};

public:
	void Add(T_UDRD_KEY Key,T_GREQ_UDRD_D& rData)
		{m_greq.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_greq.RemoveKey(Key);}
	void DelAll()
		{m_greq.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GREQ_UDRD_D& rData)
		{return m_greq.Lookup(Key,rData);}
	int GetCount()
		{return m_greq.GetCount();}
	POSITION GetStart()
		{return m_greq.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GREQ_UDRD_D& rData)
		{m_greq.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GREQ_UDRD_D,T_GREQ_UDRD_D&>m_greq;
};

#endif
#ifndef __DB_REBBU_DB_H__
#define __DB_REBBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_REBBU
{
public:
	CDB_REBBU()
	{
		m_rebb.InitHashTable(HASHSIZEREBB);
	}
	virtual ~CDB_REBBU(){};

public:
	void Add(T_UDRD_KEY Key,T_REBB_UDRD_D& rData)
		{m_rebb.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rebb.RemoveKey(Key);}
	void DelAll()
		{m_rebb.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_REBB_UDRD_D& rData)
		{return m_rebb.Lookup(Key,rData);}
	int GetCount()
		{return m_rebb.GetCount();}
	POSITION GetStart()
		{return m_rebb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_REBB_UDRD_D& rData)
		{m_rebb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_REBB_UDRD_D,T_REBB_UDRD_D&>m_rebb;
};

#endif
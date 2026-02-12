#ifndef __DB_REDUU_DB_H__
#define __DB_REDUU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_REDUU
{
public:
	CDB_REDUU()
	{
		m_redu.InitHashTable(HASHSIZEREDU);
	}
	virtual ~CDB_REDUU(){};

public:
	void Add(T_UDRD_KEY Key,T_REDU_UDRD_D& rData)
		{m_redu.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_redu.RemoveKey(Key);}
	void DelAll()
		{m_redu.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_REDU_UDRD_D& rData)
		{return m_redu.Lookup(Key,rData);}
	int GetCount()
		{return m_redu.GetCount();}
	POSITION GetStart()
		{return m_redu.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_REDU_UDRD_D& rData)
		{m_redu.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_REDU_UDRD_D,T_REDU_UDRD_D&>m_redu;
};

#endif
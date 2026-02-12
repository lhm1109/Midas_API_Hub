#ifndef __DB_GILDU_DB_H__
#define __DB_GILDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GILDU
{
public:
	CDB_GILDU()
	{
		m_gild.InitHashTable(HASHSIZEGILD);
	}
	virtual ~CDB_GILDU(){};

public:
	void Add(T_UDRD_KEY Key,T_GILD_UDRD_D& rData)
		{m_gild.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_gild.RemoveKey(Key);}
	void DelAll()
		{m_gild.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GILD_UDRD_D& rData)
		{return m_gild.Lookup(Key,rData);}
	int GetCount()
		{return m_gild.GetCount();}
	POSITION GetStart()
		{return m_gild.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GILD_UDRD_D& rData)
		{m_gild.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GILD_UDRD_D,T_GILD_UDRD_D&>m_gild;
};

#endif
#ifndef __DB_GUNOU_DB_H__
#define __DB_GUNOU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GUNOU
{
public:
	CDB_GUNOU()
	{
		m_guno.InitHashTable(HASHSIZEGUNO);
	}
	virtual ~CDB_GUNOU(){};

public:
	void Add(T_UDRD_KEY Key,T_GUNO_UDRD_D& rData)
		{m_guno.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_guno.RemoveKey(Key);}
	void DelAll()
		{m_guno.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GUNO_UDRD_D& rData)
		{return m_guno.Lookup(Key,rData);}
	int GetCount()
		{return m_guno.GetCount();}
	POSITION GetStart()
		{return m_guno.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GUNO_UDRD_D& rData)
		{m_guno.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GUNO_UDRD_D,T_GUNO_UDRD_D&>m_guno;
};

#endif
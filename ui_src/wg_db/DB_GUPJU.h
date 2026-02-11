#ifndef __DB_GUPJU_DB_H__
#define __DB_GUPJU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GUPJU
{
public:
	CDB_GUPJU()
	{
		m_gupj.InitHashTable(HASHSIZEGUPJ);
	}
	virtual ~CDB_GUPJU(){};

public:
	void Add(T_UDRD_KEY Key,T_GUPJ_UDRD_D& rData)
		{m_gupj.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_gupj.RemoveKey(Key);}
	void DelAll()
		{m_gupj.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GUPJ_UDRD_D& rData)
		{return m_gupj.Lookup(Key,rData);}
	int GetCount()
		{return m_gupj.GetCount();}
	POSITION GetStart()
		{return m_gupj.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GUPJ_UDRD_D& rData)
		{m_gupj.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GUPJ_UDRD_D,T_GUPJ_UDRD_D&>m_gupj;
};

#endif
#ifndef __DB_STUSU_DB_H__
#define __DB_STUSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STUSU
{
public:
	CDB_STUSU()
	{
		m_stus.InitHashTable(HASHSIZESTUS);
	}
	virtual ~CDB_STUSU(){};

public:
	void Add(T_UDRD_KEY Key,T_STUS_UDRD_D& rData)
		{m_stus.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_stus.RemoveKey(Key);}
	void DelAll()
		{m_stus.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_STUS_UDRD_D& rData)
		{return m_stus.Lookup(Key,rData);}
	int GetCount()
		{return m_stus.GetCount();}
	POSITION GetStart()
		{return m_stus.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_STUS_UDRD_D& rData)
		{m_stus.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STUS_UDRD_D,T_STUS_UDRD_D&>m_stus;
};

#endif
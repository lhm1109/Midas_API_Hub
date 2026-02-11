#ifndef __DB_CLRSU_DB_H__
#define __DB_CLRSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CLRSU
{
public:
	CDB_CLRSU()
	{
		m_clrs.InitHashTable(HASHSIZECLRS);
	}
	virtual ~CDB_CLRSU(){};

public:
	void Add(T_UDRD_KEY Key,T_CLRS_UDRD_D& rData)
		{m_clrs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_clrs.RemoveKey(Key);}
	void DelAll()
		{m_clrs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CLRS_UDRD_D& rData)
		{return m_clrs.Lookup(Key,rData);}
	int GetCount()
		{return m_clrs.GetCount();}
	POSITION GetStart()
		{return m_clrs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CLRS_UDRD_D& rData)
		{m_clrs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CLRS_UDRD_D,T_CLRS_UDRD_D&>m_clrs;
};

#endif
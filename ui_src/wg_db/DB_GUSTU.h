#ifndef __DB_GUSTU_DB_H__
#define __DB_GUSTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GUSTU
{
public:
	CDB_GUSTU()
	{
		m_gust.InitHashTable(HASHSIZEGUST);
	}
	virtual ~CDB_GUSTU(){};

public:
	void Add(T_UDRD_KEY Key,T_GUST_UDRD_D& rData)
		{m_gust.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_gust.RemoveKey(Key);}
	void DelAll()
		{m_gust.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GUST_UDRD_D& rData)
		{return m_gust.Lookup(Key,rData);}
	int GetCount()
		{return m_gust.GetCount();}
	POSITION GetStart()
		{return m_gust.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GUST_UDRD_D& rData)
		{m_gust.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GUST_UDRD_D,T_GUST_UDRD_D&>m_gust;
};

#endif
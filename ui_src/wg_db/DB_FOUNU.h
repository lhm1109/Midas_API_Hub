#ifndef __DB_FOUNU_DB_H__
#define __DB_FOUNU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_FOUNU
{
public:
	CDB_FOUNU()
	{
		m_foun.InitHashTable(HASHSIZEFOUN);
	}
	virtual ~CDB_FOUNU(){};

public:
	void Add(T_UDRD_KEY Key,T_FOUN_UDRD_D& rData)
		{m_foun.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_foun.RemoveKey(Key);}
	void DelAll()
		{m_foun.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_FOUN_UDRD_D& rData)
		{return m_foun.Lookup(Key,rData);}
	int GetCount()
		{return m_foun.GetCount();}
	POSITION GetStart()
		{return m_foun.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_FOUN_UDRD_D& rData)
		{m_foun.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_FOUN_UDRD_D,T_FOUN_UDRD_D&>m_foun;
};

#endif
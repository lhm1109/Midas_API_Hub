#ifndef __DB_ANEVU_DB_H__
#define __DB_ANEVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ANEVU
{
public:
	CDB_ANEVU()
	{
		m_anev.InitHashTable(HASHSIZEANEV);
	}
	virtual ~CDB_ANEVU(){};

public:
	void Add(T_UDRD_KEY Key,T_ANEV_UDRD_D& rData)
	{m_anev.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_anev.RemoveKey(Key);}
	void DelAll()
	{m_anev.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ANEV_UDRD_D& rData)
	{return m_anev.Lookup(Key,rData);}
	int GetCount()
	{return m_anev.GetCount();}
	POSITION GetStart()
	{return m_anev.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_ANEV_UDRD_D& rData)
	{m_anev.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_ANEV_UDRD_D,T_ANEV_UDRD_D&>m_anev;
};

#endif


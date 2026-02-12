#ifndef __DB_P1LAU_DB_H__
#define __DB_P1LAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_P1LAU
{
public:
	CDB_P1LAU()
	{
		m_p1la.InitHashTable(HASHSIZEP1LA);
	}
	virtual ~CDB_P1LAU(){};

public:
	void Add(T_UDRD_KEY Key,T_P1LA_UDRD_D& rData)
		{m_p1la.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_p1la.RemoveKey(Key);}
	void DelAll()
		{m_p1la.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_P1LA_UDRD_D& rData)
		{return m_p1la.Lookup(Key,rData);}
	int GetCount()
		{return m_p1la.GetCount();}
	POSITION GetStart()
		{return m_p1la.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_P1LA_UDRD_D& rData)
		{m_p1la.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_P1LA_UDRD_D,T_P1LA_UDRD_D&>m_p1la;
};

#endif
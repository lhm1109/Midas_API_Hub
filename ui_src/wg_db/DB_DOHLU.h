#ifndef __DB_DOHLU_DB_H__
#define __DB_DOHLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DOHLU
{
public:
	CDB_DOHLU()
	{
		m_dohl.InitHashTable(HASHSIZEDOHL);
	}
	virtual ~CDB_DOHLU(){};

public:
	void Add(T_UDRD_KEY Key,T_DOHL_UDRD_D& rData)
		{m_dohl.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dohl.RemoveKey(Key);}
	void DelAll()
		{m_dohl.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DOHL_UDRD_D& rData)
		{return m_dohl.Lookup(Key,rData);}
	int GetCount()
		{return m_dohl.GetCount();}
	POSITION GetStart()
		{return m_dohl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DOHL_UDRD_D& rData)
		{m_dohl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DOHL_UDRD_D,T_DOHL_UDRD_D&>m_dohl;
};

#endif
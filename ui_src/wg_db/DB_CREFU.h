#ifndef __DB_CREFU_DB_H__
#define __DB_CREFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CREFU
{
public:
	CDB_CREFU()
	{
		m_cref.InitHashTable(HASHSIZECREF);
	}
	virtual ~CDB_CREFU(){};

public:
	void Add(T_UDRD_KEY Key,T_CREF_UDRD_D& rData)
		{m_cref.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cref.RemoveKey(Key);}
	void DelAll()
		{m_cref.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CREF_UDRD_D& rData)
		{return m_cref.Lookup(Key,rData);}
	int GetCount()
		{return m_cref.GetCount();}
	POSITION GetStart()
		{return m_cref.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CREF_UDRD_D& rData)
		{m_cref.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CREF_UDRD_D,T_CREF_UDRD_D&>m_cref;
};

#endif

#ifndef __DB_PTTNU_DB_H__
#define __DB_PTTNU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_DESIGN.H"

class CDB_PTTNU
{
public:
	CDB_PTTNU()
	{
		m_pttn.InitHashTable(HASHSIZEPTTN);
	}
	virtual ~CDB_PTTNU(){};

public:
	void Add(T_UDRD_KEY Key, T_PTTN_UDRD_D& rData)
		{m_pttn.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pttn.RemoveKey(Key);}
	void DelAll()
		{m_pttn.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key, T_PTTN_UDRD_D& rData)
		{return m_pttn.Lookup(Key,rData);}
	int GetCount()
		{return m_pttn.GetCount();}
	POSITION GetStart()
		{return m_pttn.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_PTTN_UDRD_D& rData)
		{m_pttn.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_PTTN_UDRD_D, T_PTTN_UDRD_D&> m_pttn;
};

#endif

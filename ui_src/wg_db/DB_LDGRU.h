#ifndef __DB_LDGRU_DB_H__
#define __DB_LDGRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LDGRU
{
public:
	CDB_LDGRU()
	{
		m_ldgr.InitHashTable(HASHSIZELDGR);
	}
	virtual ~CDB_LDGRU(){};

public:
	void Add(T_UDRD_KEY Key,T_LDGR_UDRD_D& rData)
		{m_ldgr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ldgr.RemoveKey(Key);}
	void DelAll()
		{m_ldgr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LDGR_UDRD_D& rData)
		{return m_ldgr.Lookup(Key,rData);}
	int GetCount()
		{return m_ldgr.GetCount();}
	POSITION GetStart()
		{return m_ldgr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LDGR_UDRD_D& rData)
		{m_ldgr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LDGR_UDRD_D,T_LDGR_UDRD_D&>m_ldgr;
};

#endif
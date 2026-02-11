#ifndef __DB_LDLNU_DB_H__
#define __DB_LDLNU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LDLNU
{
public:
	CDB_LDLNU()
	{
		m_ldln.InitHashTable(HASHSIZELDLN);
	}
	virtual ~CDB_LDLNU(){};

public:
	void Add(T_UDRD_KEY Key,T_LDLN_UDRD_D& rData)
		{m_ldln.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ldln.RemoveKey(Key);}
	void DelAll()
		{m_ldln.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LDLN_UDRD_D& rData)
		{return m_ldln.Lookup(Key,rData);}
	int GetCount()
		{return m_ldln.GetCount();}
	POSITION GetStart()
		{return m_ldln.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LDLN_UDRD_D& rData)
		{m_ldln.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LDLN_UDRD_D,T_LDLN_UDRD_D&>m_ldln;
};

#endif

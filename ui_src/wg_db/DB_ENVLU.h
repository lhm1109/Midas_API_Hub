#ifndef __DB_ENVLU_DB_H__
#define __DB_ENVLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ENVLU
{
public:
	CDB_ENVLU()
	{
		m_envl.InitHashTable(HASHSIZEENVL);
	}
	virtual ~CDB_ENVLU(){};

public:
	void Add(T_UDRD_KEY Key,T_ENVL_UDRD_D& rData)
		{m_envl.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_envl.RemoveKey(Key);}
	void DelAll()
		{m_envl.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ENVL_UDRD_D& rData)
		{return m_envl.Lookup(Key,rData);}
	int GetCount()
		{return m_envl.GetCount();}
	POSITION GetStart()
		{return m_envl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_ENVL_UDRD_D& rData)
		{m_envl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_ENVL_UDRD_D,T_ENVL_UDRD_D&>m_envl;
};

#endif

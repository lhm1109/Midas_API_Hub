#ifndef __DB_PRLSU_DB_H__
#define __DB_PRLSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PRLSU
{
public:
	CDB_PRLSU()
	{
		m_prls.InitHashTable(HASHSIZEPRLS);
	}
	virtual ~CDB_PRLSU(){};

public:
	void Add(T_UDRD_KEY Key,T_PRLS_UDRD_D& rData)
		{m_prls.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_prls.RemoveKey(Key);}
	void DelAll()
		{m_prls.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PRLS_UDRD_D& rData)
		{return m_prls.Lookup(Key,rData);}
	int GetCount()
		{return m_prls.GetCount();}
	POSITION GetStart()
		{return m_prls.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PRLS_UDRD_D& rData)
		{m_prls.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PRLS_UDRD_D,T_PRLS_UDRD_D&>m_prls;
};

#endif
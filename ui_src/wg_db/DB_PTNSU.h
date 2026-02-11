#ifndef __DB_PTNSU_DB_H__
#define __DB_PTNSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PTNSU
{
public:
	CDB_PTNSU()
	{
		m_ptns.InitHashTable(HASHSIZEPTNS);
	}
	virtual ~CDB_PTNSU(){};

public:
	void Add(T_UDRD_KEY Key,T_PTNS_UDRD_D& rData)
		{m_ptns.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ptns.RemoveKey(Key);}
	void DelAll()
		{m_ptns.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PTNS_UDRD_D& rData)
		{return m_ptns.Lookup(Key,rData);}
	int GetCount()
		{return m_ptns.GetCount();}
	POSITION GetStart()
		{return m_ptns.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PTNS_UDRD_D& rData)
		{m_ptns.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PTNS_UDRD_D,T_PTNS_UDRD_D&>m_ptns;
};

#endif
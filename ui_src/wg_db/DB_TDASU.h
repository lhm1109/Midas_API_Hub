#ifndef __DB_TDASU_DB_H__
#define __DB_TDASU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TDASU
{
public:
	CDB_TDASU()
	{
		m_tdas.InitHashTable(HASHSIZETDAS);
	}
	virtual ~CDB_TDASU(){};

public:
	void Add(T_UDRD_KEY Key,T_TDAS_UDRD_D& rData)
		{m_tdas.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_tdas.RemoveKey(Key);}
	void DelAll()
		{m_tdas.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_TDAS_UDRD_D& rData)
		{return m_tdas.Lookup(Key,rData);}
	int GetCount()
		{return m_tdas.GetCount();}
	POSITION GetStart()
		{return m_tdas.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_TDAS_UDRD_D& rData)
		{m_tdas.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_TDAS_UDRD_D,T_TDAS_UDRD_D&>m_tdas;
};

#endif
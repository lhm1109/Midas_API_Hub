#ifndef __DB_ETFCU_DB_H__
#define __DB_ETFCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ETFCU
{
public:
	CDB_ETFCU()
	{
		m_etfc.InitHashTable(HASHSIZEETFC);
	}
	virtual ~CDB_ETFCU(){};

public:
	void Add(T_UDRD_KEY Key,T_ETFC_UDRD_D& rData)
		{m_etfc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_etfc.RemoveKey(Key);}
	void DelAll()
		{m_etfc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ETFC_UDRD_D& rData)
		{return m_etfc.Lookup(Key,rData);}
	int GetCount()
		{return m_etfc.GetCount();}
	POSITION GetStart()
		{return m_etfc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_ETFC_UDRD_D& rData)
		{m_etfc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_ETFC_UDRD_D,T_ETFC_UDRD_D&>m_etfc;
};

#endif
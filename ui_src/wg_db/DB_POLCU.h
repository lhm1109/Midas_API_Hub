#ifndef __DB_POLCU_DB_H__
#define __DB_POLCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_POLCU
{
public:
	CDB_POLCU()
	{
		m_polc.InitHashTable(HASHSIZEPOLC);
	}
	virtual ~CDB_POLCU(){};

public:
	void Add(T_UDRD_KEY Key,T_POLC_UDRD_D& rData)
		{m_polc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_polc.RemoveKey(Key);}
	void DelAll()
		{m_polc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_POLC_UDRD_D& rData)
		{return m_polc.Lookup(Key,rData);}
	int GetCount()
		{return m_polc.GetCount();}
	POSITION GetStart()
		{return m_polc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_POLC_UDRD_D& rData)
		{m_polc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_POLC_UDRD_D,T_POLC_UDRD_D&>m_polc;
};

#endif
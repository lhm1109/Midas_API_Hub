#ifndef __DB_GILCU_DB_H__
#define __DB_GILCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GILCU
{
public:
	CDB_GILCU()
	{
		m_gilc.InitHashTable(HASHSIZEGILC);
	}
	virtual ~CDB_GILCU(){};

public:
	void Add(T_UDRD_KEY Key,T_GILC_UDRD_D& rData)
		{m_gilc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_gilc.RemoveKey(Key);}
	void DelAll()
		{m_gilc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GILC_UDRD_D& rData)
		{return m_gilc.Lookup(Key,rData);}
	int GetCount()
		{return m_gilc.GetCount();}
	POSITION GetStart()
		{return m_gilc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GILC_UDRD_D& rData)
		{m_gilc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GILC_UDRD_D,T_GILC_UDRD_D&>m_gilc;
};

#endif
#ifndef __DB_STDCU_DB_H__
#define __DB_STDCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STDCU
{
public:
	CDB_STDCU()
	{
		m_stdc.InitHashTable(HASHSIZESTDC);
	}
	virtual ~CDB_STDCU(){};

public:
	void Add(T_UDRD_KEY Key,T_STDC_UDRD_D& rData)
		{m_stdc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_stdc.RemoveKey(Key);}
	void DelAll()
		{m_stdc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_STDC_UDRD_D& rData)
		{return m_stdc.Lookup(Key,rData);}
	int GetCount()
		{return m_stdc.GetCount();}
	POSITION GetStart()
		{return m_stdc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_STDC_UDRD_D& rData)
		{m_stdc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STDC_UDRD_D,T_STDC_UDRD_D&>m_stdc;
};

#endif
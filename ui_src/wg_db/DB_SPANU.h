#ifndef __DB_SPANU_DB_H__
#define __DB_SPANU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SPANU
{
public:
	CDB_SPANU()
	{
		m_span.InitHashTable(HASHSIZESPAN);
	}
	virtual ~CDB_SPANU(){};

public:
	void Add(T_UDRD_KEY Key,T_SPAN_UDRD_D& rData)
		{m_span.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_span.RemoveKey(Key);}
	void DelAll()
		{m_span.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SPAN_UDRD_D& rData)
		{return m_span.Lookup(Key,rData);}
	int GetCount()
		{return m_span.GetCount();}
	POSITION GetStart()
		{return m_span.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SPAN_UDRD_D& rData)
		{m_span.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SPAN_UDRD_D,T_SPAN_UDRD_D&>m_span;
};

#endif

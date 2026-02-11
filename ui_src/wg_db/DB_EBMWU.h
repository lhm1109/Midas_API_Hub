#ifndef __DB_EBMWU_DB_H__
#define __DB_EBMWU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_EBMWU
{
public:
	CDB_EBMWU()
	{
		m_ebmw.InitHashTable(HASHSIZEEBMW);
	}
	virtual ~CDB_EBMWU(){};

public:
	void Add(T_UDRD_KEY Key,T_EBMW_UDRD_D& rData)
		{m_ebmw.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ebmw.RemoveKey(Key);}
	void DelAll()
		{m_ebmw.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_EBMW_UDRD_D& rData)
		{return m_ebmw.Lookup(Key,rData);}
	int GetCount()
		{return m_ebmw.GetCount();}
	POSITION GetStart()
		{return m_ebmw.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_EBMW_UDRD_D& rData)
		{m_ebmw.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_EBMW_UDRD_D,T_EBMW_UDRD_D&>m_ebmw;
};

#endif
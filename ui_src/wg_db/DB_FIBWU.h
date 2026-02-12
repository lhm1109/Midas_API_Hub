#ifndef __DB_FIBWU_DB_H__
#define __DB_FIBWU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_FIBWU
{
public:
	CDB_FIBWU()
	{
		m_fibw.InitHashTable(HASHSIZEFIBW);
	}
	virtual ~CDB_FIBWU(){};

public:
	void Add(T_UDRD_KEY Key,T_FIBW_UDRD_D& rData)
		{m_fibw.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_fibw.RemoveKey(Key);}
	void DelAll()
		{m_fibw.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_FIBW_UDRD_D& rData)
		{return m_fibw.Lookup(Key,rData);}
	int GetCount()
		{return m_fibw.GetCount();}
	POSITION GetStart()
		{return m_fibw.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_FIBW_UDRD_D& rData)
		{m_fibw.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_FIBW_UDRD_D,T_FIBW_UDRD_D&>m_fibw;
};

#endif
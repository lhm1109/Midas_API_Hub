#ifndef __DB_MBTVU_DB_H__
#define __DB_MBTVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MBTVU
{
public:
	CDB_MBTVU()
	{
		m_mbtv.InitHashTable(HASHSIZEMBTV);
	}
	virtual ~CDB_MBTVU(){};

public:
	void Add(T_UDRD_KEY Key,T_MBTV_UDRD_D& rData)
		{m_mbtv.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mbtv.RemoveKey(Key);}
	void DelAll()
		{m_mbtv.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MBTV_UDRD_D& rData)
		{return m_mbtv.Lookup(Key,rData);}
	int GetCount()
		{return m_mbtv.GetCount();}
	POSITION GetStart()
		{return m_mbtv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MBTV_UDRD_D& rData)
		{m_mbtv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MBTV_UDRD_D,T_MBTV_UDRD_D&>m_mbtv;
};

#endif
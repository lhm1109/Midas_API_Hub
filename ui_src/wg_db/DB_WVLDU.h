#ifndef __DB_WVLDU_DB_H__
#define __DB_WVLDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_WVLDU
{
public:
	CDB_WVLDU()
	{
		m_wvld.InitHashTable(HASHSIZEWVLD);
	}
	virtual ~CDB_WVLDU(){};

public:
	void Add(T_UDRD_KEY Key,T_WVLD_UDRD_D& rData)
		{m_wvld.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_wvld.RemoveKey(Key);}
	void DelAll()
		{m_wvld.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_WVLD_UDRD_D& rData)
		{return m_wvld.Lookup(Key,rData);}
	int GetCount()
		{return m_wvld.GetCount();}
	POSITION GetStart()
		{return m_wvld.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_WVLD_UDRD_D& rData)
		{m_wvld.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_WVLD_UDRD_D,T_WVLD_UDRD_D&>m_wvld;
};

#endif

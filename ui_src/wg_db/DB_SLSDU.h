#ifndef __DB_SLSDU_DB_H__
#define __DB_SLSDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SLSDU
{
public:
	CDB_SLSDU()
	{
		m_msld.InitHashTable(HASHSIZESLSD);
	}
	virtual ~CDB_SLSDU(){};

public:
	void Add(T_UDRD_KEY Key,T_SLSD_UDRD_D& rData)
		{m_msld.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_msld.RemoveKey(Key);}
	void DelAll()
		{m_msld.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SLSD_UDRD_D& rData)
		{return m_msld.Lookup(Key,rData);}
	int GetCount()
		{return m_msld.GetCount();}
	POSITION GetStart()
		{return m_msld.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SLSD_UDRD_D& rData)
		{m_msld.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SLSD_UDRD_D,T_SLSD_UDRD_D&>m_msld;
};

#endif
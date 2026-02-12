#ifndef __DB_REBWU_DB_H__
#define __DB_REBWU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_REBWU
{
public:
	CDB_REBWU()
	{
		m_rebw.InitHashTable(HASHSIZEREBW);
	}
	virtual ~CDB_REBWU(){};

public:
	void Add(T_UDRD_KEY Key,T_REBW_UDRD_D& rData)
		{m_rebw.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rebw.RemoveKey(Key);}
	void DelAll()
		{m_rebw.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_REBW_UDRD_D& rData)
		{return m_rebw.Lookup(Key,rData);}
	int GetCount()
		{return m_rebw.GetCount();}
	POSITION GetStart()
		{return m_rebw.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_REBW_UDRD_D& rData)
		{m_rebw.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_REBW_UDRD_D,T_REBW_UDRD_D&>m_rebw;
};

#endif
#ifndef __DB_STRDU_DB_H__
#define __DB_STRDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STRDU
{
public:
	CDB_STRDU()
	{
		m_strd.InitHashTable(HASHSIZESTRD);
	}
	virtual ~CDB_STRDU(){};

public:
	void Add(T_UDRD_KEY Key,T_STRD_UDRD_D& rData)
		{m_strd.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_strd.RemoveKey(Key);}
	void DelAll()
		{m_strd.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_STRD_UDRD_D& rData)
		{return m_strd.Lookup(Key,rData);}
	int GetCount()
		{return m_strd.GetCount();}
	POSITION GetStart()
		{return m_strd.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_STRD_UDRD_D& rData)
		{m_strd.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STRD_UDRD_D,T_STRD_UDRD_D&>m_strd;
};

#endif
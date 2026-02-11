#ifndef __DB_PBDDU_DB_H__
#define __DB_PBDDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PBDDU
{
public:
	CDB_PBDDU()
	{
		m_pbdd.InitHashTable(HASHSIZEPBDD);
	}
	virtual ~CDB_PBDDU(){};

public:
	void Add(T_UDRD_KEY Key,T_PBDD_UDRD_D& rData)
		{m_pbdd.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pbdd.RemoveKey(Key);}
	void DelAll()
		{m_pbdd.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PBDD_UDRD_D& rData)
		{return m_pbdd.Lookup(Key,rData);}
	int GetCount()
		{return m_pbdd.GetCount();}
	POSITION GetStart()
		{return m_pbdd.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PBDD_UDRD_D& rData)
		{m_pbdd.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PBDD_UDRD_D,T_PBDD_UDRD_D&>m_pbdd;
};

#endif
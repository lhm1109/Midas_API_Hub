#ifndef __DB_ELNKU_DB_H__
#define __DB_ELNKU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELNKU
{
public:
	CDB_ELNKU()
	{
		m_elnk.InitHashTable(HASHSIZEELNK);
	}
	virtual ~CDB_ELNKU(){};

public:
	void Add(T_UDRD_KEY Key,T_ELNK_UDRD_D& rData)
		{m_elnk.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_elnk.RemoveKey(Key);}
	void DelAll()
		{m_elnk.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ELNK_UDRD_D& rData)
		{return m_elnk.Lookup(Key,rData);}
	int GetCount()
		{return m_elnk.GetCount();}
	POSITION GetStart()
		{return m_elnk.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_ELNK_UDRD_D& rData)
		{m_elnk.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_ELNK_UDRD_D,T_ELNK_UDRD_D&>m_elnk;
};

#endif
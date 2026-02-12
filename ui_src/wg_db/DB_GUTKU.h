#ifndef __DB_GUTKU_DB_H__
#define __DB_GUTKU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GUTKU
{
public:
	CDB_GUTKU()
	{
		m_gutk.InitHashTable(HASHSIZEGUTK);
	}
	virtual ~CDB_GUTKU(){};

public:
	void Add(T_UDRD_KEY Key,T_GUTK_UDRD_D& rData)
		{m_gutk.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_gutk.RemoveKey(Key);}
	void DelAll()
		{m_gutk.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GUTK_UDRD_D& rData)
		{return m_gutk.Lookup(Key,rData);}
	int GetCount()
		{return m_gutk.GetCount();}
	POSITION GetStart()
		{return m_gutk.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GUTK_UDRD_D& rData)
		{m_gutk.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GUTK_UDRD_D,T_GUTK_UDRD_D&>m_gutk;
};

#endif
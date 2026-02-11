#ifndef __DB_STLDU_DB_H__
#define __DB_STLDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STLDU
{
public:
	CDB_STLDU()
	{
		m_stld.InitHashTable(HASHSIZESTLD);
	}
	virtual ~CDB_STLDU(){};

public:
	void Add(T_UDRD_KEY Key,T_STLD_UDRD_D& rData)
		{m_stld.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_stld.RemoveKey(Key);}
	void DelAll()
		{m_stld.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_STLD_UDRD_D& rData)
		{return m_stld.Lookup(Key,rData);}
	int GetCount()
		{return m_stld.GetCount();}
	POSITION GetStart()
		{return m_stld.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_STLD_UDRD_D& rData)
		{m_stld.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STLD_UDRD_D,T_STLD_UDRD_D&>m_stld;
};

#endif
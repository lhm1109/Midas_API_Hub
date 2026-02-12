#ifndef __DB_MVLDU_DB_H__
#define __DB_MVLDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MVLDU
{
public:
	CDB_MVLDU()
	{
		m_mvld.InitHashTable(HASHSIZEMVLD);
	}
	virtual ~CDB_MVLDU(){};

public:
	void Add(T_UDRD_KEY Key,T_MVLD_UDRD_D& rData)
		{m_mvld.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mvld.RemoveKey(Key);}
	void DelAll()
		{m_mvld.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MVLD_UDRD_D& rData)
		{return m_mvld.Lookup(Key,rData);}
	int GetCount()
		{return m_mvld.GetCount();}
	POSITION GetStart()
		{return m_mvld.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MVLD_UDRD_D& rData)
		{m_mvld.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MVLD_UDRD_D,T_MVLD_UDRD_D&>m_mvld;
};

#endif
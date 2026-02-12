#ifndef __DB_AWEDU_DB_H__
#define __DB_AWEDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_AWEDU
{
public:
	CDB_AWEDU()
	{
		m_awed.InitHashTable(HASHSIZEAWED);
	}
	virtual ~CDB_AWEDU(){};

public:
	void Add(T_UDRD_KEY Key,T_AWED_UDRD_D& rData)
		{m_awed.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_awed.RemoveKey(Key);}
	void DelAll()
		{m_awed.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_AWED_UDRD_D& rData)
		{return m_awed.Lookup(Key,rData);}
	int GetCount()
		{return m_awed.GetCount();}
	POSITION GetStart()
		{return m_awed.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_AWED_UDRD_D& rData)
		{m_awed.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_AWED_UDRD_D,T_AWED_UDRD_D&>m_awed;
};

#endif
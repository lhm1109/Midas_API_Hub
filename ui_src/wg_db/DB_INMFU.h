#ifndef __DB_INMFU_DB_H__
#define __DB_INMFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_INMFU
{
public:
	CDB_INMFU()
	{
		m_inmf.InitHashTable(HASHSIZEINMF);
	}
	virtual ~CDB_INMFU(){};

public:
	void Add(T_UDRD_KEY Key,T_INMF_UDRD_D& rData)
		{m_inmf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_inmf.RemoveKey(Key);}
	void DelAll()
		{m_inmf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_INMF_UDRD_D& rData)
		{return m_inmf.Lookup(Key,rData);}
	int GetCount()
		{return m_inmf.GetCount();}
	POSITION GetStart()
		{return m_inmf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_INMF_UDRD_D& rData)
		{m_inmf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_INMF_UDRD_D,T_INMF_UDRD_D&>m_inmf;
};

#endif
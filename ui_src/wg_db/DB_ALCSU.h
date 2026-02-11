#ifndef __DB_ALCSU_DB_H__
#define __DB_ALCSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ALCSU
{
public:
	CDB_ALCSU()
	{
		m_alcs.InitHashTable(HASHSIZEALCS);
	}
	virtual ~CDB_ALCSU(){};

public:
	void Add(T_UDRD_KEY Key,T_ALCS_UDRD_D& rData)
		{m_alcs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_alcs.RemoveKey(Key);}
	void DelAll()
		{m_alcs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ALCS_UDRD_D& rData)
		{return m_alcs.Lookup(Key,rData);}
	int GetCount()
		{return m_alcs.GetCount();}
	POSITION GetStart()
		{return m_alcs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_ALCS_UDRD_D& rData)
		{m_alcs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_ALCS_UDRD_D,T_ALCS_UDRD_D&>m_alcs;
};

#endif
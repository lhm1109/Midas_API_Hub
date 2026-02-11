#ifndef __DB_NUCSU_DB_H__
#define __DB_NUCSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_NUCSU
{
public:
	CDB_NUCSU()
	{
		m_nucs.InitHashTable(HASHSIZENUCS);
	}
	virtual ~CDB_NUCSU(){};

public:
	void Add(T_UDRD_KEY Key,T_NUCS_UDRD_D& rData)
		{m_nucs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_nucs.RemoveKey(Key);}
	void DelAll()
		{m_nucs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_NUCS_UDRD_D& rData)
		{return m_nucs.Lookup(Key,rData);}
	int GetCount()
		{return m_nucs.GetCount();}
	POSITION GetStart()
		{return m_nucs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_NUCS_UDRD_D& rData)
		{m_nucs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_NUCS_UDRD_D,T_NUCS_UDRD_D&>m_nucs;
};

#endif
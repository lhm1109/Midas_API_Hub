#ifndef __DB_PFMCU_DB_H__
#define __DB_PFMCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PFMCU
{
public:
	CDB_PFMCU()
	{
		m_pfmc.InitHashTable(HASHSIZEPFMC);
	}
	virtual ~CDB_PFMCU(){};

public:
	void Add(T_UDRD_KEY Key,T_PFMC_UDRD_D& rData)
		{m_pfmc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pfmc.RemoveKey(Key);}
	void DelAll()
		{m_pfmc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PFMC_UDRD_D& rData)
		{return m_pfmc.Lookup(Key,rData);}
	int GetCount()
		{return m_pfmc.GetCount();}
	POSITION GetStart()
		{return m_pfmc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PFMC_UDRD_D& rData)
		{m_pfmc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PFMC_UDRD_D,T_PFMC_UDRD_D&>m_pfmc;
};

#endif
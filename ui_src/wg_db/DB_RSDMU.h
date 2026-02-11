#ifndef __DB_RSDMU_DB_H__
#define __DB_RSDMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RSDMU
{
public:
	CDB_RSDMU()
	{
		m_rsdm.InitHashTable(HASHSIZERSDM);
	}
	virtual ~CDB_RSDMU(){};

public:
	void Add(T_UDRD_KEY Key,T_RSDM_UDRD_D& rData)
		{m_rsdm.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rsdm.RemoveKey(Key);}
	void DelAll()
		{m_rsdm.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RSDM_UDRD_D& rData)
		{return m_rsdm.Lookup(Key,rData);}
	int GetCount()
		{return m_rsdm.GetCount();}
	POSITION GetStart()
		{return m_rsdm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RSDM_UDRD_D& rData)
		{m_rsdm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RSDM_UDRD_D,T_RSDM_UDRD_D&>m_rsdm;
};

#endif
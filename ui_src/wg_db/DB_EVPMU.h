#ifndef __DB_EVPMU_DB_H__
#define __DB_EVPMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_EVPMU
{
public:
	CDB_EVPMU()
	{
		m_evpm.InitHashTable(HASHSIZEEVPM);
	}
	virtual ~CDB_EVPMU(){};

public:
	void Add(T_UDRD_KEY Key,T_EVPM_UDRD_D& rData)
		{m_evpm.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_evpm.RemoveKey(Key);}
	void DelAll()
		{m_evpm.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_EVPM_UDRD_D& rData)
		{return m_evpm.Lookup(Key,rData);}
	int GetCount()
		{return m_evpm.GetCount();}
	POSITION GetStart()
		{return m_evpm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_EVPM_UDRD_D& rData)
		{m_evpm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_EVPM_UDRD_D,T_EVPM_UDRD_D&>m_evpm;
};

#endif
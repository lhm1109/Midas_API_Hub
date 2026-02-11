#ifndef __DB_PMCMU_DB_H__
#define __DB_PMCMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_PMCMU
{
public:
	CDB_PMCMU()
	{
		m_PMCM.InitHashTable(HASHSIZEPMCM);
	}
	virtual ~CDB_PMCMU(){};

public:
	void Add(T_UDRD_KEY Key,T_PMCM_UDRD_D& rData)
	{m_PMCM.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_PMCM.RemoveKey(Key);}
	void DelAll()
	{m_PMCM.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PMCM_UDRD_D& rData)
	{return m_PMCM.Lookup(Key,rData);}
	int GetCount()
	{return m_PMCM.GetCount();}
	POSITION GetStart()
	{return m_PMCM.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PMCM_UDRD_D& rData)
	{m_PMCM.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_PMCM_UDRD_D,T_PMCM_UDRD_D&>m_PMCM;
};

#endif

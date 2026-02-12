#ifndef __DB_PMCRU_DB_H__
#define __DB_PMCRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PMCRU
{
public:
	CDB_PMCRU()
	{
		m_pmcr.InitHashTable(HASHSIZEPMCR);
	}
	virtual ~CDB_PMCRU(){};

public:
	void Add(T_UDRD_KEY Key,T_PMCR_UDRD_D& rData)
		{m_pmcr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pmcr.RemoveKey(Key);}
	void DelAll()
		{m_pmcr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PMCR_UDRD_D& rData)
		{return m_pmcr.Lookup(Key,rData);}
	int GetCount()
		{return m_pmcr.GetCount();}
	POSITION GetStart()
		{return m_pmcr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PMCR_UDRD_D& rData)
		{m_pmcr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PMCR_UDRD_D,T_PMCR_UDRD_D&>m_pmcr;
};

#endif
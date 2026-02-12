#ifndef __DB_PMFMU_DB_H__
#define __DB_PMFMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PMFMU
{
public:
	CDB_PMFMU()
	{
		m_pmfm.InitHashTable(HASHSIZEPMFM);
	}
	virtual ~CDB_PMFMU(){};

public:
	void Add(T_UDRD_KEY Key,T_PMFM_UDRD_D& rData)
		{m_pmfm.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pmfm.RemoveKey(Key);}
	void DelAll()
		{m_pmfm.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PMFM_UDRD_D& rData)
		{return m_pmfm.Lookup(Key,rData);}
	int GetCount()
		{return m_pmfm.GetCount();}
	POSITION GetStart()
		{return m_pmfm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PMFM_UDRD_D& rData)
		{m_pmfm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PMFM_UDRD_D,T_PMFM_UDRD_D&>m_pmfm;
};

#endif

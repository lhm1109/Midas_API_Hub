#include "stdafx.h"
#include "DB_THGC.h"

CDB_THGC::CDB_THGC()
{
	m_thgc.InitHashTable(HASHSIZETHGC);
}

CDB_THGC::~CDB_THGC()
{

}

void CDB_THGC::Add(T_THGC_D& rData)
{
	m_thgc.SetAt(1,rData);
}

BOOL CDB_THGC::Del()
{
	T_THGC_D Data;
	BOOL bExist=m_thgc.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_thgc.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

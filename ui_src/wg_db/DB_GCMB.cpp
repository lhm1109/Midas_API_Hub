#include "stdafx.h"
#include "DB_GCMB.h"

CDB_GCMB::CDB_GCMB()
{
	m_gcmb.InitHashTable(HASHSIZEGCMB);
}

CDB_GCMB::~CDB_GCMB()
{

}

void CDB_GCMB::Add(T_GCMB_D& rData)
{
	m_gcmb.SetAt(1,rData);
}

BOOL CDB_GCMB::Del()
{
	T_GCMB_D Data;
	BOOL bExist=m_gcmb.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_gcmb.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

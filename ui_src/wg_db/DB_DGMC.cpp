#include "stdafx.h"
#include "DB_MATL.h"
#include "DB_DGMC.h"

CDB_DGMC::CDB_DGMC()
{
	m_dgmc.InitHashTable(HASHSIZEDGMC);
}

CDB_DGMC::~CDB_DGMC()
{

}


void CDB_DGMC::Add(T_DGMC_K Key,T_DGMC_D& rData)
{
	m_dgmc.SetAt(Key,rData);
}

BOOL CDB_DGMC::Del(T_DGMC_K Key)
{
	T_DGMC_D Data;
	BOOL bExist=m_dgmc.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_dgmc.RemoveKey(Key);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}



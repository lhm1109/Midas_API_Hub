#include "stdafx.h"
#include "DB_DYAS.h"

CDB_DYAS::CDB_DYAS()
{
	m_dyas.InitHashTable(HASHSIZEDYAS);
}

CDB_DYAS::~CDB_DYAS()
{

}

void CDB_DYAS::Add(T_DYAS_D& rData)
{
	m_dyas.SetAt(1,rData);
}

BOOL CDB_DYAS::Del()
{
	T_DYAS_D Data;
	BOOL bExist=m_dyas.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_dyas.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
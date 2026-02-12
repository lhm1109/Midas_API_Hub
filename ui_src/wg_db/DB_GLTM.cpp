#include "stdafx.h"
#include "DB_GLTM.h"

CDB_GLTM::CDB_GLTM()
{
	m_gltm.InitHashTable(HASHSIZEGLTM);
}

CDB_GLTM::~CDB_GLTM()
{

}

void CDB_GLTM::Add(T_GLTM_D& rData)
{
	m_gltm.SetAt(1,rData);
}

BOOL CDB_GLTM::Del()
{
	T_GLTM_D Data;
	BOOL bExist=m_gltm.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_gltm.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


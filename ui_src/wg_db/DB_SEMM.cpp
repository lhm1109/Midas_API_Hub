#include "stdafx.h"
#include "DB_MATL.h"
#include "DB_SEMM.h"

CDB_SEMM::CDB_SEMM()
{
	m_semm.InitHashTable(HASHSIZESEMM);
}

CDB_SEMM::~CDB_SEMM()
{

}

void CDB_SEMM::Add(T_SEMM_K Key,T_SEMM_D& rData,CDB_MATL* pMatl)
{
	T_SEMM_D Data;
	BOOL bExist=m_semm.Lookup(Key, Data);
	m_semm.SetAt(Key,rData);
	if(!bExist && pMatl != NULL)pMatl->AddListItem(Key, LT_SEMM_CMD, Key);
}

BOOL CDB_SEMM::Del(T_SEMM_K Key,CDB_MATL* pMatl)
{
	T_SEMM_D Data;
	BOOL bExist=m_semm.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_semm.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pMatl != NULL)VERIFY(pMatl->DelListItem(Key, LT_SEMM_CMD, Key));
		}
		return ret;
	}
	return bExist;
}



#include "stdafx.h"
#include "DB_MATL.h"
#include "DB_PMFM.h"

CDB_PMFM::CDB_PMFM()
{
	m_pmfm.InitHashTable(HASHSIZEPMFM);
}

CDB_PMFM::~CDB_PMFM()
{

}

void CDB_PMFM::Add(T_PMFM_K Key,T_PMFM_D& rData,CDB_MATL* pMatl)
{
	T_PMFM_D Data;
	BOOL bExist=m_pmfm.Lookup(Key, Data);
	m_pmfm.SetAt(Key,rData);
	if(!bExist && pMatl != NULL)pMatl->AddListItem(Key, LT_PMFM_CMD, Key);
}

BOOL CDB_PMFM::Del(T_PMFM_K Key,CDB_MATL* pMatl)
{
	T_PMFM_D Data;
	BOOL bExist=m_pmfm.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_pmfm.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pMatl != NULL)VERIFY(pMatl->DelListItem(Key, LT_PMFM_CMD, Key));
		}
		return ret;
	}
	return bExist;
}



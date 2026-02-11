#include "stdafx.h"
#include "DB_MATL.h"
#include "DB_SECM.h"

CDB_SECM::CDB_SECM()
{
	m_secm.InitHashTable(HASHSIZESECM);
}

CDB_SECM::~CDB_SECM()
{

}

void CDB_SECM::Add(T_SECM_K Key,T_SECM_D& rData,CDB_MATL* pMatl)
{
	T_SECM_D Data;
	BOOL bExist=m_secm.Lookup(Key, Data);
	m_secm.SetAt(Key,rData);
	if(!bExist && pMatl != NULL)pMatl->AddListItem(Key, LT_SECM_CMD, Key);
}

BOOL CDB_SECM::Del(T_SECM_K Key,CDB_MATL* pMatl)
{
	T_SECM_D Data;
	BOOL bExist=m_secm.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_secm.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pMatl != NULL)VERIFY(pMatl->DelListItem(Key, LT_SECM_CMD, Key));
		}
		return ret;
	}
	return bExist;
}



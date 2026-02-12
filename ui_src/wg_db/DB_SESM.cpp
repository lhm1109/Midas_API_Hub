#include "stdafx.h"
#include "DB_MATL.h"
#include "DB_SESM.h"

CDB_SESM::CDB_SESM()
{
	m_sesm.InitHashTable(HASHSIZESESM);
}

CDB_SESM::~CDB_SESM()
{

}

void CDB_SESM::Add(T_SESM_K Key,T_SESM_D& rData,CDB_MATL* pMatl)
{
	T_SESM_D Data;
	BOOL bExist=m_sesm.Lookup(Key, Data);
	m_sesm.SetAt(Key,rData);
	if(!bExist && pMatl != NULL)pMatl->AddListItem(Key, LT_SESM_CMD, Key);
}

BOOL CDB_SESM::Del(T_SESM_K Key,CDB_MATL* pMatl)
{
	T_SESM_D Data;
	BOOL bExist=m_sesm.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_sesm.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pMatl != NULL)VERIFY(pMatl->DelListItem(Key, LT_SESM_CMD, Key));
		}
		return ret;
	}
	return bExist;
}



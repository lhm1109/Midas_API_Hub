#include "stdafx.h"
#include "DB_MATL.h"
#include "DB_MPHG.h"

CDB_MPHG::CDB_MPHG()
{
	m_mphg.InitHashTable(HASHSIZEMPHG);
}

CDB_MPHG::~CDB_MPHG()
{

}

void CDB_MPHG::Add(T_MPHG_K Key,T_MPHG_D& rData,CDB_MATL* pMatl)
{
	T_MPHG_D Data;
	BOOL bExist=m_mphg.Lookup(Key, Data);
	m_mphg.SetAt(Key,rData);
	if(!bExist && pMatl != NULL)pMatl->AddListItem(Key, LT_MPHG_CMD, Key);
}

BOOL CDB_MPHG::Del(T_MPHG_K Key,CDB_MATL* pMatl)
{
	T_MPHG_D Data;
	BOOL bExist=m_mphg.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_mphg.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pMatl != NULL)VERIFY(pMatl->DelListItem(Key, LT_MPHG_CMD, Key));
		}
		return ret;
	}
	return bExist;
}



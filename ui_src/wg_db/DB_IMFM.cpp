#include "stdafx.h"
#include "DB_MATL.h"
#include "DB_IMFM.h"

CDB_IMFM::CDB_IMFM()
{
	m_imfm.InitHashTable(HASHSIZEIMFM);
}

CDB_IMFM::~CDB_IMFM()
{

}

void CDB_IMFM::Add(T_IMFM_K Key,T_IMFM_D& rData,CDB_MATL* pMatl)
{
	T_IMFM_D Data;
	BOOL bExist=m_imfm.Lookup(Key, Data);
	m_imfm.SetAt(Key,rData);
	if(!bExist && pMatl != NULL)pMatl->AddListItem(Key, LT_IMFM_CMD, Key);
}

BOOL CDB_IMFM::Del(T_IMFM_K Key,CDB_MATL* pMatl)
{
	T_IMFM_D Data;
	BOOL bExist=m_imfm.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_imfm.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pMatl != NULL)VERIFY(pMatl->DelListItem(Key, LT_IMFM_CMD, Key));
		}
		return ret;
	}
	return bExist;
}



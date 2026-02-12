#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_EFWD.h"

CDB_EFWD::CDB_EFWD()
{
	m_EFWD.InitHashTable(HASHSIZEEFWD);
}

CDB_EFWD::~CDB_EFWD()
{

}

void CDB_EFWD::Add(T_EFWD_K Key,T_EFWD_D& rData,CDB_SECT* pSect)
{
	T_EFWD_D Data;
	BOOL bExist=m_EFWD.Lookup(Key, Data);
	m_EFWD.SetAt(Key,rData);
	if(!bExist && pSect != NULL)pSect->AddListItem(Key, LT_EFWD_CMD, Key);
}

BOOL CDB_EFWD::Del(T_EFWD_K Key,CDB_SECT* pSect)
{
	T_EFWD_D Data;
	BOOL bExist=m_EFWD.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_EFWD.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pSect != NULL)VERIFY(pSect->DelListItem(Key, LT_EFWD_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

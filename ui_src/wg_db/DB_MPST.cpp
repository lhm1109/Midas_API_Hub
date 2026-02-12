#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_MPST.h"

CDB_MPST::CDB_MPST()
{
	m_mpst.InitHashTable(HASHSIZEMPST);
}

CDB_MPST::~CDB_MPST()
{

}

void CDB_MPST::Add(T_MPST_K Key,T_MPST_D& rData,CDB_SECT* pSect)
{
	T_MPST_D Data;
	BOOL bExist=m_mpst.Lookup(Key, Data);
	m_mpst.SetAt(Key,rData);
	if(!bExist && pSect != NULL)pSect->AddListItem(Key, LT_MPST_CMD, Key);
}

BOOL CDB_MPST::Del(T_MPST_K Key,CDB_SECT* pSect)
{
	T_MPST_D Data;
	BOOL bExist=m_mpst.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_mpst.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pSect != NULL)VERIFY(pSect->DelListItem(Key, LT_MPST_CMD, Key));
		}
		return ret;
	}
	return bExist;
}



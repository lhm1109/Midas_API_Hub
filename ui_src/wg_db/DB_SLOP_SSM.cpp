#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_SLOP_SSM.h"

CDB_SLOP_SSM::CDB_SLOP_SSM()
{
	m_slop_ssm.InitHashTable(HASHSIZESLOP_SSM);
}

CDB_SLOP_SSM::~CDB_SLOP_SSM()
{

}

void CDB_SLOP_SSM::Add(T_SLOP_SSM_K Key,T_SLOP_SSM_D& rData,CDB_SECT* pSect)
{
	T_SLOP_SSM_D Data;
	BOOL bExist=m_slop_ssm.Lookup(Key, Data);
	m_slop_ssm.SetAt(Key,rData);
	if(!bExist && pSect != NULL)pSect->AddListItem(Key, LT_SLOP_SSM_CMD, Key);
}

BOOL CDB_SLOP_SSM::Del(T_SLOP_SSM_K Key,CDB_SECT* pSect)
{
	T_SLOP_SSM_D Data;
	BOOL bExist=m_slop_ssm.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_slop_ssm.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pSect != NULL)VERIFY(pSect->DelListItem(Key, LT_SLOP_SSM_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

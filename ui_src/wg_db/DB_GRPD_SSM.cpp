#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_GRPD_SSM.h"

CDB_GRPD_SSM::CDB_GRPD_SSM()
{
	m_grpd_ssm.InitHashTable(HASHSIZEGRPDSSM);
}

CDB_GRPD_SSM::~CDB_GRPD_SSM()
{

}

void CDB_GRPD_SSM::Add(T_GRPD_SSM_K Key,T_GRPD_SSM_D& rData,CDB_SECT* pSect)
{
	T_GRPD_SSM_D Data;
	BOOL bExist=m_grpd_ssm.Lookup(Key, Data);
	m_grpd_ssm.SetAt(Key,rData);
	if(!bExist && pSect != NULL)pSect->AddListItem(Key, LT_GRPD_SSM_CMD, Key);
}

BOOL CDB_GRPD_SSM::Del(T_GRPD_SSM_K Key,CDB_SECT* pSect)
{
	T_GRPD_SSM_D Data;
	BOOL bExist=m_grpd_ssm.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_grpd_ssm.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pSect != NULL)VERIFY(pSect->DelListItem(Key, LT_GRPD_SSM_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

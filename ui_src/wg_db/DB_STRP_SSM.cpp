#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_STRP_SSM.h"

CDB_STRP_SSM::CDB_STRP_SSM()
{
	m_strp_ssm.InitHashTable(HASHSIZESTRPSSM);
}

CDB_STRP_SSM::~CDB_STRP_SSM()
{

}

void CDB_STRP_SSM::Add(T_STRP_SSM_K Key,T_STRP_SSM_D& rData,CDB_SECT* pSect)
{
	T_STRP_SSM_D Data;
	BOOL bExist=m_strp_ssm.Lookup(Key, Data);
	m_strp_ssm.SetAt(Key,rData);
	if(!bExist && pSect != NULL)pSect->AddListItem(Key, LT_STRP_SSM_CMD, Key);
}

BOOL CDB_STRP_SSM::Del(T_STRP_SSM_K Key,CDB_SECT* pSect)
{
	T_STRP_SSM_D Data;
	BOOL bExist=m_strp_ssm.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_strp_ssm.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pSect != NULL)VERIFY(pSect->DelListItem(Key, LT_STRP_SSM_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

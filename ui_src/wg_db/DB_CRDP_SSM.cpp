#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_CRDP_SSM.h"

CDB_CRDP_SSM::CDB_CRDP_SSM()
{
	m_crdp_ssm.InitHashTable(HASHSIZERPSCSSM);
}

CDB_CRDP_SSM::~CDB_CRDP_SSM()
{

}

void CDB_CRDP_SSM::Add(T_CRDP_SSM_K Key,T_CRDP_SSM_D& rData,CDB_SECT* pSect)
{
	T_CRDP_SSM_D Data;
	BOOL bExist=m_crdp_ssm.Lookup(Key, Data);
	m_crdp_ssm.SetAt(Key,rData);
	if(!bExist && pSect != NULL)pSect->AddListItem(Key, LT_CRDP_SSM_CMD, Key);
}

BOOL CDB_CRDP_SSM::Del(T_CRDP_SSM_K Key,CDB_SECT* pSect)
{
	T_CRDP_SSM_D Data;
	BOOL bExist=m_crdp_ssm.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_crdp_ssm.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pSect != NULL)VERIFY(pSect->DelListItem(Key, LT_CRDP_SSM_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_RPSC_SSM.h"

CDB_RPSC_SSM::CDB_RPSC_SSM()
{
	m_rpsc_ssm.InitHashTable(HASHSIZERPSCSSM);
	m_rpsc_ssmDesign.InitHashTable(HASHSIZERPSCSSM);
}

CDB_RPSC_SSM::~CDB_RPSC_SSM()
{

}

void CDB_RPSC_SSM::Add(T_RPSC_SSM_K Key,T_RPSC_SSM_D& rData,CDB_SECT* pSect)
{
	T_RPSC_SSM_D Data;
	BOOL bExist=m_rpsc_ssm.Lookup(Key, Data);
	m_rpsc_ssm.SetAt(Key,rData);
	if(!bExist && pSect != NULL)pSect->AddListItem(Key, LT_RPSC_SSM_CMD, Key);
}

BOOL CDB_RPSC_SSM::Del(T_RPSC_SSM_K Key,CDB_SECT* pSect)
{
	T_RPSC_SSM_D Data;
	BOOL bExist=m_rpsc_ssm.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_rpsc_ssm.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pSect != NULL)VERIFY(pSect->DelListItem(Key, LT_RPSC_SSM_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

void CDB_RPSC_SSM::AddDesign(T_RPSC_SSM_K Key,T_RPSC_SSM_D& rData,CDB_SECT* pSect)
{
	T_RPSC_SSM_D Data;
	BOOL bExist=m_rpsc_ssmDesign.Lookup(Key, Data);
	m_rpsc_ssmDesign.SetAt(Key,rData);
	if(!bExist && pSect != NULL)pSect->AddListItem(Key, LT_RPSD_SSM_CMD, Key);
}

BOOL CDB_RPSC_SSM::DelDesign(T_RPSC_SSM_K Key,CDB_SECT* pSect)
{
	T_RPSC_SSM_D Data;
	BOOL bExist=m_rpsc_ssmDesign.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_rpsc_ssmDesign.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pSect != NULL)VERIFY(pSect->DelListItem(Key, LT_RPSD_SSM_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

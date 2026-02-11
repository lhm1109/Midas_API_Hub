#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_STRN_SSM.h"

CDB_STRN_SSM::CDB_STRN_SSM()
{
	m_strn_ssm.InitHashTable(HASHSIZERPSCSSM);
}

CDB_STRN_SSM::~CDB_STRN_SSM()
{

}

void CDB_STRN_SSM::Add(T_STRN_SSM_K Key,T_STRN_SSM_D& rData,CDB_SECT* pSect)
{
	T_STRN_SSM_D Data;
	BOOL bExist=m_strn_ssm.Lookup(Key, Data);
	m_strn_ssm.SetAt(Key,rData);
	if(!bExist && pSect != NULL)pSect->AddListItem(Key, LT_STRN_SSM_CMD, Key);
}

BOOL CDB_STRN_SSM::Del(T_STRN_SSM_K Key,CDB_SECT* pSect)
{
	T_STRN_SSM_D Data;
	BOOL bExist=m_strn_ssm.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_strn_ssm.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pSect != NULL)VERIFY(pSect->DelListItem(Key, LT_STRN_SSM_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

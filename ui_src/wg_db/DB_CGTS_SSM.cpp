#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_CGTS_SSM.h"

CDB_CGTS_SSM::CDB_CGTS_SSM()
{
	m_cgts_ssm.InitHashTable(HASHSIZECGTSSSM);
}

CDB_CGTS_SSM::~CDB_CGTS_SSM()
{

}

void CDB_CGTS_SSM::Add(T_CGTS_SSM_K Key,T_CGTS_SSM_D& rData,CDB_SECT* pSect)
{
	T_CGTS_SSM_D Data;
	BOOL bExist=m_cgts_ssm.Lookup(Key, Data);
	m_cgts_ssm.SetAt(Key,rData);
	if(!bExist && pSect != NULL)pSect->AddListItem(Key, LT_CGTS_SSM_CMD, Key);
}

BOOL CDB_CGTS_SSM::Del(T_CGTS_SSM_K Key,CDB_SECT* pSect)
{
	T_CGTS_SSM_D Data;
	BOOL bExist=m_cgts_ssm.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_cgts_ssm.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pSect != NULL)VERIFY(pSect->DelListItem(Key, LT_CGTS_SSM_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

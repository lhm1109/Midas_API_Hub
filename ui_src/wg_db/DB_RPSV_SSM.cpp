#include "stdafx.h"
#include "DB_SECV.h"
#include "DB_RPSV_SSM.h"

CDB_RPSV_SSM::CDB_RPSV_SSM()
{
	m_rpsv_ssm.InitHashTable(HASHSIZERPSVSSM);
}

CDB_RPSV_SSM::~CDB_RPSV_SSM()
{

}

void CDB_RPSV_SSM::Add(T_RPSV_SSM_K Key,T_RPSV_SSM_D& rData, CDB_SECV* pSecv)
{
	T_RPSV_SSM_D Data;
	BOOL bExist=m_rpsv_ssm.Lookup(Key, Data);
	m_rpsv_ssm.SetAt(Key,rData);
	if(!bExist && pSecv != NULL)pSecv->AddListItem(Key, LT_RPSV_SSM_CMD, Key);
}

BOOL CDB_RPSV_SSM::Del(T_RPSV_SSM_K Key,CDB_SECV* pSecv)
{
	T_RPSV_SSM_D Data;
	BOOL bExist=m_rpsv_ssm.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_rpsv_ssm.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pSecv != NULL)VERIFY(pSecv->DelListItem(Key, LT_RPSV_SSM_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

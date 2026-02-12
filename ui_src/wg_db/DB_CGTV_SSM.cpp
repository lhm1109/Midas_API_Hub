#include "stdafx.h"
#include "DB_CGTV_SSM.h"

CDB_CGTV_SSM::CDB_CGTV_SSM()
{
	m_cgtv_ssm.InitHashTable(HASHSIZECGTVSSM);
}

CDB_CGTV_SSM::~CDB_CGTV_SSM()
{

}

void CDB_CGTV_SSM::Add(T_CGTV_SSM_K Key,T_CGTV_SSM_D& rData)
{
	T_CGTV_SSM_D Data;
	BOOL bExist=m_cgtv_ssm.Lookup(Key, Data);
	m_cgtv_ssm.SetAt(Key,rData);
}

BOOL CDB_CGTV_SSM::Del(T_CGTV_SSM_K Key)
{
	BOOL ret = m_cgtv_ssm.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

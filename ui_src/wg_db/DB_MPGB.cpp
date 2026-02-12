#include "stdafx.h"
#include "DB_MPGB.h"

CDB_MPGB::CDB_MPGB()
{
	m_mpgb.InitHashTable(HASHSIZEMPGB);
}

CDB_MPGB::~CDB_MPGB()
{

}

void CDB_MPGB::Add(T_MPGB_D& rData)
{
	m_mpgb.SetAt(1,rData);
}

BOOL CDB_MPGB::Del()
{
	T_MPGB_D Data;
	BOOL bExist=m_mpgb.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_mpgb.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


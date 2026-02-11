#include "stdafx.h"
#include "DB_DAGB.h"


CDB_DAGB::CDB_DAGB()
{
	m_Damping.InitHashTable(HASHSIZEDAGB);
}

CDB_DAGB::~CDB_DAGB()
{

}

void CDB_DAGB::Add(T_DAGB_D& rData)
{
	m_Damping.SetAt(1, rData);
}

BOOL CDB_DAGB::Del()
{
	T_DAGB_D Data;
	BOOL bExist = m_Damping.Lookup(1, Data);
	if (bExist)
	{
		BOOL ret = m_Damping.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

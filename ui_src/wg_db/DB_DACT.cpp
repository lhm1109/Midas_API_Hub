#include "stdafx.h"
#include "DB_DACT.h"

CDB_DACT::CDB_DACT()
{
	m_dact.InitHashTable(HASHSIZEDACT);
}

CDB_DACT::~CDB_DACT()
{

}

void CDB_DACT::Add(T_DACT_D& rData)
{
	m_dact.SetAt(1, rData);
}

BOOL CDB_DACT::Del()
{
	T_DACT_D Data;
	BOOL bExist = m_dact.Lookup(1, Data);
	if (bExist)
	{
		BOOL ret = m_dact.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
#include "stdafx.h"
#include "DB_DYBS.h"

CDB_DYBS::CDB_DYBS()
{
	m_dybs.InitHashTable(HASHSIZEDYBS);
}

CDB_DYBS::~CDB_DYBS()
{

}

void CDB_DYBS::Add(T_DYBS_D& rData)
{
	m_dybs.SetAt(1, rData);
}

BOOL CDB_DYBS::Del()
{
	T_DYBS_D Data;
	BOOL bExist = m_dybs.Lookup(1, Data);
	if (bExist)
	{
		BOOL ret = m_dybs.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


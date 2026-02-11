#include "stdafx.h"
#include "DB_IFBZ.h"

CDB_IFBZ::CDB_IFBZ()
{
	m_ifbz.InitHashTable(HASHSIZEDSFC);
}

CDB_IFBZ::~CDB_IFBZ()
{

}

void CDB_IFBZ::Add(T_IFBZ_D& rData)
{
	m_ifbz.SetAt(1, rData);
}

BOOL CDB_IFBZ::Del()
{
	T_IFBZ_D Data;
	BOOL bExist = m_ifbz.Lookup(1, Data);
	if (bExist)
	{
		BOOL ret = m_ifbz.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


#include "stdafx.h"
#include "DB_MNFC.h"

CDB_MNFC::CDB_MNFC()
{
	m_mnfc.InitHashTable(HASHSIZEMNFC);
}

CDB_MNFC::~CDB_MNFC()
{

}

void CDB_MNFC::Add(T_MNFC_D& rData)
{
	m_mnfc.SetAt(1, rData);
}

BOOL CDB_MNFC::Del()
{
	T_MNFC_D Data;
	BOOL bExist = m_mnfc.Lookup(1, Data);
	if (bExist)
	{
		BOOL ret = m_mnfc.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
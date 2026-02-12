#include "stdafx.h"
#include "DB_DSFC.h"

CDB_DSFC::CDB_DSFC()
{
	m_dsfc.InitHashTable(HASHSIZEDSFC);
}

CDB_DSFC::~CDB_DSFC()
{

}

void CDB_DSFC::Add(T_DSFC_D& rData)
{
	m_dsfc.SetAt(1, rData);
}

BOOL CDB_DSFC::Del()
{
	T_DSFC_D Data;
	BOOL bExist = m_dsfc.Lookup(1, Data);
	if (bExist)
	{
		BOOL ret = m_dsfc.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


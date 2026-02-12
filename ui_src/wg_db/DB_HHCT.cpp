#include "stdafx.h"
#include "DB_HHCT.h"

CDB_HHCT::CDB_HHCT()
{
	m_hhct.InitHashTable(HASHSIZEHHCT);
}

CDB_HHCT::~CDB_HHCT()
{

}

void CDB_HHCT::Add(T_HHCT_D& rData)
{
	T_HHCT_D Data;
	m_hhct.SetAt(1,rData);
}

BOOL CDB_HHCT::Del()
{
	T_HHCT_D Data;
	BOOL bExist=m_hhct.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_hhct.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


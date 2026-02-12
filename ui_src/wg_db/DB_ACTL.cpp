#include "stdafx.h"
#include "DB_ACTL.h"

CDB_ACTL::CDB_ACTL()
{
	m_actl.InitHashTable(HASHSIZEACTL);
}

CDB_ACTL::~CDB_ACTL()
{

}

void CDB_ACTL::Add(T_ACTL_D& rData)
{
	m_actl.SetAt(1,rData);
}

BOOL CDB_ACTL::Del()
{
	T_ACTL_D Data;
	BOOL bExist=m_actl.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_actl.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


#include "stdafx.h"
#include "DB_CCTL.h"

CDB_CCTL::CDB_CCTL()
{
	m_cctl.InitHashTable(HASHSIZECCTL);
}

CDB_CCTL::~CDB_CCTL()
{

}

void CDB_CCTL::Add(T_CCTL_D& rData)
{
	m_cctl.SetAt(1,rData);
}

BOOL CDB_CCTL::Del()
{
	T_CCTL_D Data;
	BOOL bExist=m_cctl.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_cctl.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


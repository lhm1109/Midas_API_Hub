#include "stdafx.h"
#include "DB_NMRD.h"

CDB_NMRD::CDB_NMRD()
{
	m_nmrd.InitHashTable(HASHSIZENMRD);
}

CDB_NMRD::~CDB_NMRD()
{

}

void CDB_NMRD::Add(T_NMRD_D& rData)
{
	m_nmrd.SetAt(1,rData);
}

BOOL CDB_NMRD::Del()
{
	T_NMRD_D Data;
	BOOL bExist=m_nmrd.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_nmrd.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

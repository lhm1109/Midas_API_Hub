#include "stdafx.h"
#include "DB_IUHP.h"

CDB_IUHP::CDB_IUHP()
{
	m_iuhp.InitHashTable(HASHSIZEIUHP);
}

CDB_IUHP::~CDB_IUHP()
{

}

void CDB_IUHP::Add(T_IUHP_D& rData)
{
	m_iuhp.SetAt(1,rData);
}

BOOL CDB_IUHP::Del()
{
	T_IUHP_D Data;
	BOOL bExist=m_iuhp.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_iuhp.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


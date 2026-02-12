#include "stdafx.h"
#include "DB_BCCT.h"

CDB_BCCT::CDB_BCCT()
{
	m_bcct.InitHashTable(HASHSIZEBCCT);
}

CDB_BCCT::~CDB_BCCT()
{

}

void CDB_BCCT::Add(T_BCCT_D& rData)
{
	m_bcct.SetAt(1,rData);
}

BOOL CDB_BCCT::Del()
{
	T_BCCT_D Data;
	BOOL bExist=m_bcct.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_bcct.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

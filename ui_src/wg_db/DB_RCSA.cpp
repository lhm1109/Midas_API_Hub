#include "stdafx.h"
#include "DB_RCSA.h"

CDB_RCSA::CDB_RCSA()
{
	m_rcsa.InitHashTable(HASHSIZERCSA);
}

CDB_RCSA::~CDB_RCSA()
{

}

void CDB_RCSA::Add(T_RCSA_D& rData)
{
	m_rcsa.SetAt(1,rData);
}

BOOL CDB_RCSA::Del()
{
	T_RCSA_D Data;
	BOOL bExist=m_rcsa.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_rcsa.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

#include "stdafx.h"
#include "DB_CESO.h"

CDB_CESO::CDB_CESO()
{
	m_ceso.InitHashTable(HASHSIZECESO);
}

CDB_CESO::~CDB_CESO()
{

}

void CDB_CESO::Add(T_CESO_D& rData)
{
	m_ceso.SetAt(1,rData);
}

BOOL CDB_CESO::Del()
{
	T_CESO_D Data;
	BOOL bExist=m_ceso.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_ceso.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


#include "stdafx.h"
#include "DB_STCI.h"

CDB_STCI::CDB_STCI()
{
	m_stci.InitHashTable(HASHSIZESTCI);
}

CDB_STCI::~CDB_STCI()
{

}

void CDB_STCI::Add(T_STCI_D& rData)
{
	m_stci.SetAt(1,rData);
}

BOOL CDB_STCI::Del()
{
	T_STCI_D Data;
	BOOL bExist=m_stci.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_stci.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}



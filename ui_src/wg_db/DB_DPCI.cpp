#include "stdafx.h"
#include "DB_DPCI.h"


CDB_DPCI::CDB_DPCI()
{
	m_mapDpci.InitHashTable(HASHSIZEISGB);
}

CDB_DPCI::~CDB_DPCI()
{

}

void CDB_DPCI::Add(T_DPCI_D& rData)
{
	m_mapDpci.SetAt(1, rData);
}

BOOL CDB_DPCI::Del()
{
	T_DPCI_D Data;
	BOOL bExist = m_mapDpci.Lookup(1, Data);
	if (bExist)
	{
		BOOL ret = m_mapDpci.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

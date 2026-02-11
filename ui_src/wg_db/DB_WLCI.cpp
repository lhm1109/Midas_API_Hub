#include "stdafx.h"
#include "DB_WLCI.h"

CDB_WLCI::CDB_WLCI()
{
	m_wlci.InitHashTable(HASHSIZEWLCI);
}

CDB_WLCI::~CDB_WLCI()
{

}

void CDB_WLCI::Add(T_WLCI_D& rData)
{
	m_wlci.SetAt(1,rData);
}

BOOL CDB_WLCI::Del()
{
	T_WLCI_D Data;
	BOOL bExist=m_wlci.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_wlci.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}



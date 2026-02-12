#include "stdafx.h"
#include "DB_EDUD.h"

CDB_EDUD::CDB_EDUD()
{
	m_EDUD.InitHashTable(HASHSIZEEDUD);
}

CDB_EDUD::~CDB_EDUD()
{

}

void CDB_EDUD::Add(T_EDUD_D& rData)
{
	m_EDUD.SetAt(1,rData);
}

BOOL CDB_EDUD::Del()
{
	T_EDUD_D Data;
	BOOL bExist = m_EDUD.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret = m_EDUD.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

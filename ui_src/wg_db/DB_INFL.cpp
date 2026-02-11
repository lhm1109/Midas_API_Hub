#include "stdafx.h"
#include "DB_INFL.h"

CDB_INFL::CDB_INFL()
{
	m_infl.InitHashTable(HASHSIZEINFL);
}

CDB_INFL::~CDB_INFL()
{

}

void CDB_INFL::Add(T_INFL_D& rData)
{
	m_infl.SetAt(1,rData);
}

BOOL CDB_INFL::Del()
{
	T_INFL_D Data;
	BOOL bExist=m_infl.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_infl.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


#include "stdafx.h"
#include "DB_SELC.h"

CDB_SELC::CDB_SELC()
{
	m_selc.InitHashTable(HASHSIZESELC);
}

CDB_SELC::~CDB_SELC()
{

}

void CDB_SELC::Add(T_SELC_D& rData)
{
	m_selc.SetAt(1,rData);
}

BOOL CDB_SELC::Del()
{
	T_SELC_D Data;
	BOOL bExist=m_selc.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_selc.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


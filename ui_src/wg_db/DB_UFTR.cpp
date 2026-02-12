#include "stdafx.h"
#include "DB_UFTR.h"

CDB_UFTR::CDB_UFTR()
{
	m_uftr.InitHashTable(HASHSIZEUFTR);
}

CDB_UFTR::~CDB_UFTR()
{
	
}

void CDB_UFTR::Add(T_UFTR_D& rData)
{
	m_uftr.SetAt(1,rData);
}

BOOL CDB_UFTR::Del()
{
	T_UFTR_D Data;
	BOOL bExist=m_uftr.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_uftr.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


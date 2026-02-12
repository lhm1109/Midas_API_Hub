#include "stdafx.h"
#include "DB_JDSC.h"

CDB_JDSC::CDB_JDSC()
{
	m_JDSC.InitHashTable(HASHSIZEJDSC);
}

CDB_JDSC::~CDB_JDSC()
{
	
}

void CDB_JDSC::Add(T_JDSC_D& rData)
{
	m_JDSC.SetAt(1,rData);
}

BOOL CDB_JDSC::Del()
{
	T_JDSC_D Data;
	BOOL bExist = m_JDSC.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret = m_JDSC.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

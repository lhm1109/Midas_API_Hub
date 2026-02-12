#include "stdafx.h"
#include "DB_JDRC.h"

CDB_JDRC::CDB_JDRC()
{
	m_JDRC.InitHashTable(HASHSIZEJDRC);
}

CDB_JDRC::~CDB_JDRC()
{
	
}

void CDB_JDRC::Add(T_JDRC_D& rData)
{
	m_JDRC.SetAt(1,rData);
}

BOOL CDB_JDRC::Del()
{
	T_JDRC_D Data;
	BOOL bExist = m_JDRC.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret = m_JDRC.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

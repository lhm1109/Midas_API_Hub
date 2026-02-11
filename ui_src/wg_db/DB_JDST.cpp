#include "stdafx.h"
#include "DB_JDST.h"

CDB_JDST::CDB_JDST()
{
	m_JDST.InitHashTable(HASHSIZEJDST);
}

CDB_JDST::~CDB_JDST()
{
	
}

void CDB_JDST::Add(T_JDST_D& rData)
{
	m_JDST.SetAt(1,rData);
}

BOOL CDB_JDST::Del()
{
	T_JDST_D Data;
	BOOL bExist = m_JDST.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret = m_JDST.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

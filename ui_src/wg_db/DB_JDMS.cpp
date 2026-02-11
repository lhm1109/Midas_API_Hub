#include "stdafx.h"
#include "DB_JDMS.h"

CDB_JDMS::CDB_JDMS()
{
	m_JDMS.InitHashTable(HASHSIZEJDMS);
}

CDB_JDMS::~CDB_JDMS()
{

}

void CDB_JDMS::Add(T_JDMS_D& rData)
{
	m_JDMS.SetAt(1,rData);
}

BOOL CDB_JDMS::Del()
{
	T_JDMS_D Data;
	BOOL bExist = m_JDMS.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret = m_JDMS.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

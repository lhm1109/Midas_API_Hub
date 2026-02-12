#include "stdafx.h"
#include "DB_IEHC.h"

CDB_IEHC::CDB_IEHC()
{
	m_iehc.InitHashTable(HASHSIZEIEHC);
}

CDB_IEHC::~CDB_IEHC()
{

}

void CDB_IEHC::Add(T_IEHC_D& rData)
{
	m_iehc.SetAt(1,rData);
}

BOOL CDB_IEHC::Del()
{
	T_IEHC_D Data;
	BOOL bExist=m_iehc.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_iehc.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


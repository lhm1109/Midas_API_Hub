#include "stdafx.h"
#include "DB_BOLT.h"

CDB_BOLT::CDB_BOLT()
{
	m_bolt.InitHashTable(HASHSIZEBOLT);
}

CDB_BOLT::~CDB_BOLT()
{

}

void CDB_BOLT::Add(T_BOLT_D& rData)
{
	m_bolt.SetAt(1,rData);
}

BOOL CDB_BOLT::Del()
{
	T_BOLT_D Data;
	BOOL bExist=m_bolt.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_bolt.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

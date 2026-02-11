#include "stdafx.h"
#include "DB_TLGD.h"

CDB_TLGD::CDB_TLGD()
{
	m_tlgd.InitHashTable(HASHSIZETLGD);
}

CDB_TLGD::~CDB_TLGD()
{

}

void CDB_TLGD::Add(T_TLGD_D& rData)
{
	m_tlgd.SetAt(1,rData);
}

BOOL CDB_TLGD::Del()
{
	T_TLGD_D Data;
	BOOL bExist=m_tlgd.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_tlgd.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

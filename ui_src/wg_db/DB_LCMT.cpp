#include "stdafx.h"
#include "DB_LCMT.h"

CDB_LCMT::CDB_LCMT()
{
	m_lcmt.InitHashTable(HASHSIZELCMT);
}

CDB_LCMT::~CDB_LCMT()
{

}

void CDB_LCMT::Add(T_LCMT_D& rData)
{
	m_lcmt.SetAt(1,rData);
}

BOOL CDB_LCMT::Del()
{
	T_LCMT_D Data;
	BOOL bExist=m_lcmt.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_lcmt.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


#include "stdafx.h"
#include "DB_ACOP.h"

CDB_ACOP::CDB_ACOP()
{
	m_acop.InitHashTable(HASHSIZEACOP);
}

CDB_ACOP::~CDB_ACOP()
{

}

void CDB_ACOP::Add(T_ACOP_D& rData)
{
	m_acop.SetAt(1,rData);
}

BOOL CDB_ACOP::Del()
{
	T_ACOP_D Data;
	BOOL bExist=m_acop.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_acop.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


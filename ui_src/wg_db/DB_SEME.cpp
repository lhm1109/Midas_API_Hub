#include "stdafx.h"
#include "DB_SEME.h"

CDB_SEME::CDB_SEME()
{
	m_seme.InitHashTable(HASHSIZESEME);
}

CDB_SEME::~CDB_SEME()
{

}

void CDB_SEME::Add(T_SEME_D& rData)
{
	m_seme.SetAt(1,rData);
}

BOOL CDB_SEME::Del()
{
	T_SEME_D Data;
	BOOL bExist=m_seme.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_seme.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


#include "stdafx.h"
#include "DB_SBRI.h"

CDB_SBRI::CDB_SBRI()
{
	m_sbri.InitHashTable(HASHSIZESBRI);
}

CDB_SBRI::~CDB_SBRI()
{

}

void CDB_SBRI::Add(T_SBRI_D& rData)
{
	m_sbri.SetAt(1,rData);
}

BOOL CDB_SBRI::Del()
{
	T_SBRI_D Data;
	BOOL bExist=m_sbri.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_sbri.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


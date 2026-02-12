#include "stdafx.h"
#include "DB_LTOM.h"

CDB_LTOM::CDB_LTOM()
{
	m_ltom.InitHashTable(HASHSIZELTOM);
}

CDB_LTOM::~CDB_LTOM()
{

}

void CDB_LTOM::Add(T_LTOM_D& rData)
{
	m_ltom.SetAt(1,rData);
}

BOOL CDB_LTOM::Del()
{
	T_LTOM_D Data;
	BOOL bExist=m_ltom.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_ltom.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


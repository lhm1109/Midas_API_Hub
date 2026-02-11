#include "stdafx.h"
#include "DB_DYFG.h"

CDB_DYFG::CDB_DYFG()
{
	m_dyfg.InitHashTable(HASHSIZEDYFG);
}

CDB_DYFG::~CDB_DYFG()
{

}

void CDB_DYFG::Add(T_DYFG_D& rData)
{
	m_dyfg.SetAt(1,rData);
}

BOOL CDB_DYFG::Del()
{
	T_DYFG_D Data;
	BOOL bExist=m_dyfg.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_dyfg.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


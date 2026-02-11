#include "stdafx.h"
#include "DB_SECC.h"

CDB_SECC::CDB_SECC()
{
	m_secc.InitHashTable(HASHSIZESECC);
}

CDB_SECC::~CDB_SECC()
{

}

void CDB_SECC::Add(T_SECC_D& rData)
{
	m_secc.SetAt(1,rData);
}

BOOL CDB_SECC::Del()
{
	T_SECC_D Data;
	BOOL bExist=m_secc.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_secc.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}



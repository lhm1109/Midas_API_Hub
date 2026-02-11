#include "stdafx.h"
#include "DB_ECUC.h"

CDB_ECUC::CDB_ECUC()
{
	m_ecuc.InitHashTable(HASHSIZEECUC);
}

CDB_ECUC::~CDB_ECUC()
{

}

void CDB_ECUC::Add(T_ECUC_D& rData)
{
	m_ecuc.SetAt(1,rData);
}

BOOL CDB_ECUC::Del()
{
	T_ECUC_D Data;
	BOOL bExist=m_ecuc.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_ecuc.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


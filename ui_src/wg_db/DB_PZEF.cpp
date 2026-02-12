#include "stdafx.h"
#include "DB_PZEF.h"

CDB_PZEF::CDB_PZEF()
{
	m_pzef.InitHashTable(HASHSIZEPZEF);
}

CDB_PZEF::~CDB_PZEF()
{

}

void CDB_PZEF::Add(T_PZEF_D& rData)
{
	m_pzef.SetAt(1,rData);
}

BOOL CDB_PZEF::Del()
{
	T_PZEF_D Data;
	BOOL bExist=m_pzef.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_pzef.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


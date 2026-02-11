#include "stdafx.h"
#include "DB_THRO.h"

CDB_THRO::CDB_THRO()
{
	m_thro.InitHashTable(HASHSIZETHRO);
}

CDB_THRO::~CDB_THRO()
{

}

void CDB_THRO::Add(T_THRO_D& rData)
{
	m_thro.SetAt(1,rData);
}

BOOL CDB_THRO::Del()
{
	T_THRO_D Data;
	BOOL bExist=m_thro.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_thro.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

#include "stdafx.h"
#include "DB_CAMB.h"

CDB_CAMB::CDB_CAMB()
{
	m_camb.InitHashTable(HASHSIZECAMB);
}

CDB_CAMB::~CDB_CAMB()
{

}

void CDB_CAMB::Add(T_CAMB_D& rData)
{
	m_camb.SetAt(1,rData);
}

BOOL CDB_CAMB::Del()
{
	T_CAMB_D Data;
	BOOL bExist=m_camb.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_camb.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

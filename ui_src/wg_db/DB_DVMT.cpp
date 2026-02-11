#include "stdafx.h"
#include "DB_DVMT.h"

CDB_DVMT::CDB_DVMT()
{
	m_dvmt.InitHashTable(HASHSIZEDVMT);
}

CDB_DVMT::~CDB_DVMT()
{

}

void CDB_DVMT::Add(T_DVMT_D& rData)
{
	m_dvmt.SetAt(1,rData);
}

BOOL CDB_DVMT::Del()
{
	T_DVMT_D Data;
	BOOL bExist=m_dvmt.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_dvmt.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

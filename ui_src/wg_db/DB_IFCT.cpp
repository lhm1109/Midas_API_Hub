#include "stdafx.h"
#include "DB_IFCT.h"

CDB_IFCT::CDB_IFCT()
{
	m_ifct.InitHashTable(HASHSIZEIFCT);
}

CDB_IFCT::~CDB_IFCT()
{

}

void CDB_IFCT::Add(T_IFCT_D& rData)
{
	m_ifct.SetAt(1,rData);
}

BOOL CDB_IFCT::Del()
{
	T_IFCT_D Data;
	BOOL bExist=m_ifct.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_ifct.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


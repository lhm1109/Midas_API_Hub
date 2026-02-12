#include "stdafx.h"
#include "DB_STYP.h"

CDB_STYP::CDB_STYP()
{
	m_styp.InitHashTable(HASHSIZESTYP);
}

CDB_STYP::~CDB_STYP()
{

}

void CDB_STYP::Add(T_STYP_D& rData)
{
	m_styp.SetAt(1,rData);
}

BOOL CDB_STYP::Del()
{
	T_STYP_D Data;
	BOOL bExist=m_styp.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_styp.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}



#include "stdafx.h"
#include "DB_NCRN.h"

CDB_NCRN::CDB_NCRN()
{
	m_ncrn.InitHashTable(HASHSIZENCRN);
}

CDB_NCRN::~CDB_NCRN()
{

}

void CDB_NCRN::Add(T_NCRN_D& rData)
{
	m_ncrn.SetAt(1,rData);
}

BOOL CDB_NCRN::Del()
{
	T_NCRN_D Data;
	BOOL bExist=m_ncrn.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_ncrn.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


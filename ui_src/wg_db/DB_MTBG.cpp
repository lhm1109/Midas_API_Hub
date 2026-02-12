#include "stdafx.h"
#include "DB_MTBG.h"

CDB_MTBG::CDB_MTBG()
{
	m_mtbg.InitHashTable(HASHSIZEMTBG);
}

CDB_MTBG::~CDB_MTBG()
{

}

void CDB_MTBG::Add(T_MTBG_D& rData)
{
	m_mtbg.SetAt(1,rData);
}

BOOL CDB_MTBG::Del()
{
	T_MTBG_D Data;
	BOOL bExist=m_mtbg.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_mtbg.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

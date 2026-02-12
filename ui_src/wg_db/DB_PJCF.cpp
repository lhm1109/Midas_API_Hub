#include "stdafx.h"
#include "DB_PJCF.h"

CDB_PJCF::CDB_PJCF()
{
	m_pjcf.InitHashTable(HASHSIZEPJCF);
}

CDB_PJCF::~CDB_PJCF()
{

}

void CDB_PJCF::Add(T_PJCF_D& rData)
{
	m_pjcf.SetAt(1,rData);
}

BOOL CDB_PJCF::Del()
{
	T_PJCF_D Data;
	BOOL bExist=m_pjcf.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_pjcf.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


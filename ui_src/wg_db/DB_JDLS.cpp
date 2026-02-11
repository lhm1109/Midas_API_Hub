#include "stdafx.h"
#include "DB_JDLS.h"

CDB_JDLS::CDB_JDLS()
{
	m_JDLS.InitHashTable(HASHSIZEJDLS);
}

CDB_JDLS::~CDB_JDLS()
{

}

void CDB_JDLS::Add(T_JDLS_D& rData)
{
	m_JDLS.SetAt(1,rData);
}

BOOL CDB_JDLS::Del()
{
	T_JDLS_D Data;
	BOOL bExist = m_JDLS.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret = m_JDLS.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

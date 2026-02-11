#include "stdafx.h"
#include "DB_HPEA.h"

CDB_HPEA::CDB_HPEA()
{
	m_hpea.InitHashTable(HASHSIZEHPEA);
}

CDB_HPEA::~CDB_HPEA()
{

}

void CDB_HPEA::Add(T_HPEA_D& rData)
{
	m_hpea.SetAt(1,rData);
}

BOOL CDB_HPEA::Del()
{
	T_HPEA_D Data;
	BOOL bExist=m_hpea.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_hpea.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


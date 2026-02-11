#include "stdafx.h"
#include "DB_IPFO.h"

CDB_IPFO::CDB_IPFO()
{
	m_ipfo.InitHashTable(HASHSIZEIPFO);
}

CDB_IPFO::~CDB_IPFO()
{

}

void CDB_IPFO::Add(T_IPFO_D& rData)
{
	m_ipfo.SetAt(1,rData);
}

BOOL CDB_IPFO::Del()
{
	T_IPFO_D Data;
	BOOL bExist=m_ipfo.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_ipfo.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

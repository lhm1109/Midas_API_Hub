#include "stdafx.h"
#include "DB_MCCS.h"

CDB_MCCS::CDB_MCCS()
{
	m_mccs.InitHashTable(HASHSIZEMCCS);
}

CDB_MCCS::~CDB_MCCS()
{

}

void CDB_MCCS::Add(T_MCCS_D& rData)
{
	m_mccs.SetAt(1,rData);
}

BOOL CDB_MCCS::Del()
{
	T_MCCS_D Data;
	BOOL bExist= m_mccs.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret= m_mccs.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


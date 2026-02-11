#include "stdafx.h"
#include "DB_POAS.h"

CDB_POAS::CDB_POAS()
{
	m_poas.InitHashTable(HASHSIZEPOAS);
}

CDB_POAS::~CDB_POAS()
{

}

void CDB_POAS::Add(T_POAS_D& rData)
{
	m_poas.SetAt(1,rData);
}

BOOL CDB_POAS::Del()
{
	T_POAS_D Data;
	BOOL bExist=m_poas.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_poas.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}


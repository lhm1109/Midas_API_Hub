#include "stdafx.h"
#include "DB_ANOP.h"

#include "DBDoc.h"

CDB_ANOP::CDB_ANOP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_anop.InitHashTable(HASHSIZEANOP);
}

CDB_ANOP::~CDB_ANOP()
{

}

void CDB_ANOP::Add(T_ANOP_D& rData)
{
	m_anop.SetAt(1, rData);
}

BOOL CDB_ANOP::Del()
{
	T_ANOP_D Data;
	BOOL bExist = m_anop.Lookup(1, Data);
	if (bExist)
	{
		BOOL ret = m_anop.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_ANOP::Get(T_ANOP_D& rData)
{
	ASSERT_VALID(&m_anop);
	return m_anop.Lookup(1, rData);
}

int CDB_ANOP::GetCount()
{
	return m_anop.GetCount();
}
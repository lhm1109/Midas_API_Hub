#include "stdafx.h"
#include "DB_RDPO.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RDPO::CDB_RDPO()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rdpo.InitHashTable(HASHSIZERDPO);
}

CDB_RDPO::~CDB_RDPO()
{

}

void CDB_RDPO::Add(T_RDPO_D& rData)
{
	m_rdpo.SetAt(1,rData);
}

BOOL CDB_RDPO::Del()
{
	T_RDPO_D Data;
	BOOL bExist=m_rdpo.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_rdpo.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_RDPO::Get(T_RDPO_D& rData)
{
	return m_rdpo.Lookup(1,rData);
}

int CDB_RDPO::GetCount()
{
	return m_rdpo.GetCount();
}

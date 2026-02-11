#include "stdafx.h"
#include "DB_DAOP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DAOP::CDB_DAOP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_daop.InitHashTable(HASHSIZEDAOP);
}

CDB_DAOP::~CDB_DAOP()
{

}

void CDB_DAOP::Add(T_DAOP_D& rData)
{
	m_daop.SetAt(1,rData);
}

BOOL CDB_DAOP::Del()
{
	T_DAOP_D Data;
	BOOL bExist=m_daop.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_daop.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_DAOP::Get(T_DAOP_D& rData)
{
	return m_daop.Lookup(1,rData);
}

int CDB_DAOP::GetCount()
{
	return m_daop.GetCount();
}

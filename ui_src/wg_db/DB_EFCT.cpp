#include "stdafx.h"
#include "DB_EFCT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_EFCT::CDB_EFCT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_efct.InitHashTable(HASHSIZEEFCT);
}

CDB_EFCT::~CDB_EFCT()
{

}

void CDB_EFCT::Add(T_EFCT_D& rData)
{
	m_efct.SetAt(1,rData);
}

BOOL CDB_EFCT::Del()
{
	T_EFCT_D Data;
	BOOL bExist=m_efct.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_efct.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_EFCT::Get(T_EFCT_D& rData)
{  
	return m_efct.Lookup(1,rData);
}

int CDB_EFCT::GetCount()
{
	return m_efct.GetCount();
}


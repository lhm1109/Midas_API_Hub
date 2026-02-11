#include "stdafx.h"
#include "DB_AETL.h"
#include "DBDoc.h"
#include "StagCtrl.h"

CDB_AETL::CDB_AETL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_aetl.InitHashTable(HASHSIZEAETL);
}

CDB_AETL::~CDB_AETL()
{
 
}

void CDB_AETL::Add(T_AETL_D& rData)
{
	m_aetl.SetAt(1,rData);
}

BOOL CDB_AETL::Del()
{
	T_AETL_D Data;
	BOOL bExist=m_aetl.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_aetl.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
BOOL CDB_AETL::Get(T_AETL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_aetl.Lookup(1,rData);
}

int CDB_AETL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_aetl.GetCount();
}
BOOL CDB_AETL::GetFromBaseDB(T_AETL_D& rData)
{
	return m_aetl.Lookup(1,rData);
}
int CDB_AETL::GetCountFromBaseDB()
{
	return m_aetl.GetCount();
}
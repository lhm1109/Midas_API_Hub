#include "stdafx.h"
#include "DB_SDPA.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SDPA::CDB_SDPA()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sdpa.InitHashTable(HASHSIZESDPA);
}

CDB_SDPA::~CDB_SDPA()
{

}

void CDB_SDPA::Add(T_SDPA_D& rData)
{
	m_sdpa.SetAt(1,rData);
}

BOOL CDB_SDPA::Del()
{
	T_SDPA_D Data;
	BOOL bExist=m_sdpa.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_sdpa.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_SDPA::Get(T_SDPA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_sdpa.Lookup(1,rData);
}

int CDB_SDPA::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_sdpa.GetCount();
}

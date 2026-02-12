#include "stdafx.h"
#include "DB_GACD.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_GACD::CDB_GACD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_GACD.InitHashTable(HASHSIZEGACD);
}

CDB_GACD::~CDB_GACD()
{

}

void CDB_GACD::Add(T_GACD_D& rData)
{
	m_GACD.SetAt(1,rData);
}

BOOL CDB_GACD::Del()
{
	T_GACD_D Data;
	BOOL bExist=m_GACD.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_GACD.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_GACD::Get(T_GACD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_GACD.Lookup(1,rData);
}

int CDB_GACD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_GACD.GetCount();
}

#include "stdafx.h"
#include "DB_RDPA.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RDPA::CDB_RDPA()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rdpa.InitHashTable(HASHSIZERDPA);
}

CDB_RDPA::~CDB_RDPA()
{

}

void CDB_RDPA::Add(T_RDPA_D& rData)
{
	m_rdpa.SetAt(1, rData);
}

BOOL CDB_RDPA::Del()
{
	T_RDPA_D Data;
	BOOL bExist = m_rdpa.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret = m_rdpa.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_RDPA::Get(T_RDPA_D& rData)
{
	if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rdpa.Lookup(1, rData);
}

int CDB_RDPA::GetCount()
{
	if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rdpa.GetCount();
}

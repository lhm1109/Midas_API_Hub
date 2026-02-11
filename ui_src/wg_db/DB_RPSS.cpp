#include "stdafx.h"
#include "DB_RPSS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RPSS::CDB_RPSS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rpss.InitHashTable(HASHSIZERPSS);
}

CDB_RPSS::~CDB_RPSS()
{

}

void CDB_RPSS::Add(T_RPSS_D& rData)
{
	m_rpss.SetAt(1, rData);
}

BOOL CDB_RPSS::Del()
{
	T_RPSS_D Data;
	BOOL bExist = m_rpss.Lookup(1, Data);
	if (bExist)
	{
		BOOL ret = m_rpss.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_RPSS::Get(T_RPSS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rpss.Lookup(1, rData);
}

int CDB_RPSS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rpss.GetCount();
}

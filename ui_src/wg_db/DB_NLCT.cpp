#include "stdafx.h"
#include "DB_NLCT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_NLCT::CDB_NLCT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nlct.InitHashTable(HASHSIZENLCT);
}

CDB_NLCT::~CDB_NLCT()
{

}

void CDB_NLCT::Add(T_NLCT_D& rData)
{
	m_nlct.SetAt(1, rData);
}

BOOL CDB_NLCT::Del()
{
	T_NLCT_D Data;
	BOOL bExist = m_nlct.Lookup(1, Data);
	if (bExist)
	{
		BOOL ret = m_nlct.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_NLCT::Get(T_NLCT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_nlct.Lookup(1, rData);
}

int CDB_NLCT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_nlct.GetCount();
}

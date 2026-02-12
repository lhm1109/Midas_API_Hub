#include "stdafx.h"
#include "DB_RLCO.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RLCO::CDB_RLCO()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rlco.InitHashTable(HASHSIZERLCO);
}

CDB_RLCO::~CDB_RLCO()
{

}

void CDB_RLCO::Add(T_RLCO_D& rData)
{
	m_rlco.SetAt(1,rData);
}

BOOL CDB_RLCO::Del()
{
	T_RLCO_D Data;
	BOOL bExist=m_rlco.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_rlco.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_RLCO::Get(T_RLCO_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rlco.Lookup(1,rData);
}

int CDB_RLCO::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rlco.GetCount();
}

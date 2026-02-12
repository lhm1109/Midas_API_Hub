#include "stdafx.h"
#include "DB_PSCD.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PSCD::CDB_PSCD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pscd.InitHashTable(HASHSIZEPSCD);
}

CDB_PSCD::~CDB_PSCD()
{

}

void CDB_PSCD::Add(T_PSCD_D& rData)
{
	m_pscd.SetAt(1,rData);
}

BOOL CDB_PSCD::Del()
{
	T_PSCD_D Data;
	BOOL bExist=m_pscd.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_pscd.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_PSCD::Get(T_PSCD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_pscd.Lookup(1,rData);
}

int CDB_PSCD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_pscd.GetCount();
}

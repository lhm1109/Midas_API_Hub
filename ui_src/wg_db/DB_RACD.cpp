#include "stdafx.h"
#include "DB_RACD.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RACD::CDB_RACD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_racd.InitHashTable(HASHSIZERACD);
}

CDB_RACD::~CDB_RACD()
{

}

void CDB_RACD::Add(T_RACD_D& rData)
{
	m_racd.SetAt(1,rData);
}

BOOL CDB_RACD::Del()
{
	T_RACD_D Data;
	BOOL bExist=m_racd.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_racd.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_RACD::Get(T_RACD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_racd.Lookup(1,rData);
}

int CDB_RACD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_racd.GetCount();
}

#include "stdafx.h"
#include "DB_POGD.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_POGD::CDB_POGD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pogd.InitHashTable(HASHSIZEPOCT);
}

CDB_POGD::~CDB_POGD()
{

}

void CDB_POGD::Add(T_POGD_D& rData)
{
	m_pogd.SetAt(1,rData);
}

BOOL CDB_POGD::Del()
{
	T_POGD_D Data;
	BOOL bExist=m_pogd.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_pogd.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_POGD::Get(T_POGD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_pogd.Lookup(1,rData);
}

int CDB_POGD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_pogd.GetCount();
}

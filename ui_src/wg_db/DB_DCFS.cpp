#include "stdafx.h"
#include "DB_DCFS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DCFS::CDB_DCFS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dcfs.InitHashTable(HASHSIZEDCFS);
}

CDB_DCFS::~CDB_DCFS()
{

}

void CDB_DCFS::Add(T_DCFS_D& rData)
{
	m_dcfs.SetAt(1,rData);
}

BOOL CDB_DCFS::Del()
{
	T_DCFS_D Data;
	BOOL bExist=m_dcfs.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_dcfs.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_DCFS::Get(T_DCFS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_dcfs.Lookup(1,rData);
}

int CDB_DCFS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dcfs.GetCount();
}

#include "stdafx.h"
#include "DB_DSTL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DSTL::CDB_DSTL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dstl.InitHashTable(HASHSIZEDSTL);
}

CDB_DSTL::~CDB_DSTL()
{

}

void CDB_DSTL::Add(T_DSTL_D& rData)
{
	m_dstl.SetAt(1,rData);
}

BOOL CDB_DSTL::Del()
{
	T_DSTL_D Data;
	BOOL bExist=m_dstl.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_dstl.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_DSTL::Get(T_DSTL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_dstl.Lookup(1,rData);
}

int CDB_DSTL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dstl.GetCount();
}

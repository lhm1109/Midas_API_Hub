#include "stdafx.h"
#include "DB_EIGV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_EIGV::CDB_EIGV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_eigv.InitHashTable(HASHSIZEEIGV);
}

CDB_EIGV::~CDB_EIGV()
{

}

void CDB_EIGV::Add(T_EIGV_D& rData)
{
	m_eigv.SetAt(1,rData);
}

BOOL CDB_EIGV::Del()
{
	T_EIGV_D Data;
	BOOL bExist=m_eigv.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_eigv.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_EIGV::Get(T_EIGV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_eigv.Lookup(1,rData);
}

int CDB_EIGV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_eigv.GetCount();
}

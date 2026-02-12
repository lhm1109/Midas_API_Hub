#include "stdafx.h"
#include "DB_MFRD.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MFRD::CDB_MFRD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mfrd.InitHashTable(HASHSIZEMFRD);
}

CDB_MFRD::~CDB_MFRD()
{

}

void CDB_MFRD::Add(T_MFRD_D& rData)
{
	m_mfrd.SetAt(1,rData);
}

BOOL CDB_MFRD::Del()
{
	T_MFRD_D Data;
	BOOL bExist=m_mfrd.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_mfrd.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_MFRD::Get(T_MFRD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_mfrd.Lookup(1,rData);
}

int CDB_MFRD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mfrd.GetCount();
}

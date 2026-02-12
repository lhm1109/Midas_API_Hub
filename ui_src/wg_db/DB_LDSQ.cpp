#include "stdafx.h"
#include "DB_LDSQ.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LDSQ::CDB_LDSQ()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ldsq.InitHashTable(HASHSIZELDSQ);
}

CDB_LDSQ::~CDB_LDSQ()
{

}

void CDB_LDSQ::Add(T_LDSQ_D& rData)
{
	m_ldsq.SetAt(1,rData);
}

BOOL CDB_LDSQ::Del()
{
	T_LDSQ_D Data;
	BOOL bExist=m_ldsq.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_ldsq.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_LDSQ::Get(T_LDSQ_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_ldsq.Lookup(1,rData);
}

int CDB_LDSQ::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_ldsq.GetCount();
}

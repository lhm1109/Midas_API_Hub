#include "stdafx.h"
#include "DB_MVCT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MVCT::CDB_MVCT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mvct.InitHashTable(HASHSIZEMVCT);
}

CDB_MVCT::~CDB_MVCT()
{

}

void CDB_MVCT::Add(T_MVCT_D& rData)
{
	m_mvct.SetAt(1,rData);
}

BOOL CDB_MVCT::Del()
{
	T_MVCT_D Data;
	BOOL bExist=m_mvct.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_mvct.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_MVCT::Get(T_MVCT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_mvct.Lookup(1,rData);
}

int CDB_MVCT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mvct.GetCount();
}

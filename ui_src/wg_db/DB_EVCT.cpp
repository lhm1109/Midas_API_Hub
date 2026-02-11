#include "stdafx.h"
#include "DB_EVCT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_EVCT::CDB_EVCT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_evct.InitHashTable(HASHSIZEEVCT);
}

CDB_EVCT::~CDB_EVCT()
{

}

void CDB_EVCT::Add(T_EVCT_D& rData)
{
	m_evct.SetAt(1,rData);
}

BOOL CDB_EVCT::Del()
{
	T_EVCT_D Data;
	BOOL bExist=m_evct.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_evct.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_EVCT::Get(T_EVCT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_evct.Lookup(1,rData);
}

int CDB_EVCT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_evct.GetCount();
}

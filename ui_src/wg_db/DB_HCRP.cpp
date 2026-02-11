#include "stdafx.h"
#include "DB_HCRP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_HCRP::CDB_HCRP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_hcrp.InitHashTable(HASHSIZEHCRP);
}

CDB_HCRP::~CDB_HCRP()
{

}

void CDB_HCRP::Add(T_HCRP_D& rData)
{
	m_hcrp.SetAt(1,rData);
}

BOOL CDB_HCRP::Del()
{
	T_HCRP_D Data;
	BOOL bExist=m_hcrp.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_hcrp.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_HCRP::Get(T_HCRP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_hcrp.Lookup(1,rData);
}

int CDB_HCRP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_hcrp.GetCount();
}

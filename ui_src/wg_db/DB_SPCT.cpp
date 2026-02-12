#include "stdafx.h"
#include "DB_SPCT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SPCT::CDB_SPCT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_spct.InitHashTable(HASHSIZESPCT);
}

CDB_SPCT::~CDB_SPCT()
{

}

void CDB_SPCT::Add(T_SPCT_D& rData)
{
	m_spct.SetAt(1,rData);
}

BOOL CDB_SPCT::Del()
{
	T_SPCT_D Data;
	BOOL bExist=m_spct.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_spct.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_SPCT::Get(T_SPCT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_spct.Lookup(1,rData);
}

int CDB_SPCT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_spct.GetCount();
}

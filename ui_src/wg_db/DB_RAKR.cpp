#include "stdafx.h"
#include "DB_RAKR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RAKR::CDB_RAKR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rakr.InitHashTable(HASHSIZERAKR);
}

CDB_RAKR::~CDB_RAKR()
{

}

void CDB_RAKR::Add(T_RAKR_D& rData)
{
	m_rakr.SetAt(1,rData);
}

BOOL CDB_RAKR::Del()
{
	T_RAKR_D Data;
	BOOL bExist=m_rakr.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_rakr.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_RAKR::Get(T_RAKR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rakr.Lookup(1,rData);
}

int CDB_RAKR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rakr.GetCount();
}

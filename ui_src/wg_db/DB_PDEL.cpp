#include "stdafx.h"
#include "DB_PDEL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PDEL::CDB_PDEL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pdel.InitHashTable(HASHSIZEPDEL);
}

CDB_PDEL::~CDB_PDEL()
{

}

void CDB_PDEL::Add(T_PDEL_D& rData)
{
	m_pdel.SetAt(1,rData);
}

BOOL CDB_PDEL::Del()
{
	T_PDEL_D Data;
	BOOL bExist=m_pdel.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_pdel.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_PDEL::Get(T_PDEL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_pdel.Lookup(1,rData);
}

int CDB_PDEL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_pdel.GetCount();
}


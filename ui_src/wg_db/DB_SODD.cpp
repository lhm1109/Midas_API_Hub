#include "stdafx.h"
#include "DB_SODD.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SODD::CDB_SODD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sodd.InitHashTable(HASHSIZESODD);
}

CDB_SODD::~CDB_SODD()
{

}

void CDB_SODD::Add(T_SODD_D& rData)
{
	m_sodd.SetAt(1,rData);
}

BOOL CDB_SODD::Del()
{
	T_SODD_D Data;
	BOOL bExist=m_sodd.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_sodd.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_SODD::Get(T_SODD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_sodd.Lookup(1,rData);
}

BOOL CDB_SODD::Get(T_SODD_K Key,T_SODD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_sodd.Lookup(Key,rData);
}

int CDB_SODD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_sodd.GetCount();
}

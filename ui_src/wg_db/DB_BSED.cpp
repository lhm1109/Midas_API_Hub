#include "stdafx.h"
#include "DB_BSED.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_BSED::CDB_BSED()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_bsed.InitHashTable(HASHSIZEBSED);
}

CDB_BSED::~CDB_BSED()
{

}

void CDB_BSED::Add(T_BSED_D& rData)
{
	m_bsed.SetAt(1,rData);
}

BOOL CDB_BSED::Del()
{
	T_BSED_D Data;
	BOOL bExist=m_bsed.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_bsed.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_BSED::Get(T_BSED_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_bsed.Lookup(1,rData);
}

BOOL CDB_BSED::Get(T_BSED_K Key,T_BSED_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_bsed.Lookup(Key,rData);
}

int CDB_BSED::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_bsed.GetCount();
}

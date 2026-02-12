#include "stdafx.h"
#include "DB_PMCM.h"
#include "DB_ELEM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PMCM::CDB_PMCM()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_PMCM.InitHashTable(HASHSIZEPMCM);
}

CDB_PMCM::~CDB_PMCM()
{

}

void CDB_PMCM::Add(T_PMCM_K Key,T_PMCM_D& rData,CDB_ELEM* pElem)
{
	T_PMCM_D Data;
	BOOL bExist=m_PMCM.Lookup(Key, Data);
	m_PMCM.SetAt(Key,rData);
	if(!bExist && pElem)pElem->AddListItem(Key, LT_PMCM_CMD, Key);
}

BOOL CDB_PMCM::Del(T_PMCM_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_PMCM.RemoveKey(Key);
	ASSERT(ret);
	if(ret && pElem)VERIFY(pElem->DelListItem(Key, LT_PMCM_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_PMCM::Get(T_PMCM_K Key,T_PMCM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_PMCM.Lookup(Key,rData);
}

int CDB_PMCM::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_PMCM.GetCount();
}

POSITION CDB_PMCM::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_PMCM.GetStartPosition();
}

void CDB_PMCM::GetNext(POSITION& rNextPosition,T_PMCM_K& rKey,T_PMCM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{ 
		ASSERT(0);
		return; 
	}
	m_PMCM.GetNextAssoc(rNextPosition,rKey,rData);
}

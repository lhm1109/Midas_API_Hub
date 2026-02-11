#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_PMCR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PMCR::CDB_PMCR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pmcr.InitHashTable(HASHSIZEPMCR);
}

CDB_PMCR::~CDB_PMCR()
{

}

void CDB_PMCR::Add(T_PMCR_K Key,T_PMCR_D& rData,CDB_ELEM* pElem)
{
	T_PMCR_D Data;
	BOOL bExist=m_pmcr.Lookup(Key, Data);
	m_pmcr.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_PMCR_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_PMCR_CMD, Key);
	}
}

BOOL CDB_PMCR::Del(T_PMCR_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_pmcr.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_PMCR_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_PMCR_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_PMCR::Get(T_PMCR_K Key,T_PMCR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_pmcr->Lookup(Key, uTemp)) return FALSE;
	}
	return m_pmcr.Lookup(Key,rData);
}

int CDB_PMCR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_pmcr->GetCount();
	}
	return m_pmcr.GetCount();
}

POSITION CDB_PMCR::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_pmcr->GetStartPosition();
	}
	return m_pmcr.GetStartPosition();
}

void CDB_PMCR::GetNext(POSITION& rNextPosition,T_PMCR_K& rKey,T_PMCR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_pmcr->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_pmcr.Lookup(rKey, rData);
		return;
	}
	m_pmcr.GetNextAssoc(rNextPosition,rKey,rData);
}

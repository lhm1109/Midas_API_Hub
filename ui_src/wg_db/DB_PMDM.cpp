#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_PMDM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PMDM::CDB_PMDM()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pmdm.InitHashTable(HASHSIZEPMDM);
}

CDB_PMDM::~CDB_PMDM()
{

}

void CDB_PMDM::Add(T_PMDM_K Key,T_PMDM_D& rData,CDB_ELEM* pElem)
{
	T_PMDM_D Data;
	BOOL bExist=m_pmdm.Lookup(Key, Data);
	m_pmdm.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_PMDM_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_PMDM_CMD, Key);
	}
}

BOOL CDB_PMDM::Del(T_PMDM_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_pmdm.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_PMDM_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_PMDM_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_PMDM::Get(T_PMDM_K Key,T_PMDM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_pmdm->Lookup(Key, uTemp)) return FALSE;
	}
	return m_pmdm.Lookup(Key,rData);
}

int CDB_PMDM::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_pmdm->GetCount();
	}
	return m_pmdm.GetCount();
}

POSITION CDB_PMDM::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_pmdm->GetStartPosition();
	}
	return m_pmdm.GetStartPosition();
}

void CDB_PMDM::GetNext(POSITION& rNextPosition,T_PMDM_K& rKey,T_PMDM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_pmdm->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_pmdm.Lookup(rKey, rData);
		return;
	}
	m_pmdm.GetNextAssoc(rNextPosition,rKey,rData);
}


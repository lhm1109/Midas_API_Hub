#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SDAP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SDAP::CDB_SDAP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sdap.InitHashTable(HASHSIZESDAP);
}

CDB_SDAP::~CDB_SDAP()
{

}

void CDB_SDAP::Add(T_SDAP_K Key,T_SDAP_D& rData,CDB_ELEM* pElem)
{
	T_SDAP_D Data;
	BOOL bExist=m_sdap.Lookup(Key, Data);
	m_sdap.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_SDAP_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SDAP_CMD, Key);
	}
}

BOOL CDB_SDAP::Del(T_SDAP_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_sdap.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_SDAP_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SDAP_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SDAP::Get(T_SDAP_K Key,T_SDAP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_sdap->Lookup(Key, uTemp)) return FALSE;
	}
	return m_sdap.Lookup(Key,rData);
}

int CDB_SDAP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sdap->GetCount();
	}
	return m_sdap.GetCount();
}

POSITION CDB_SDAP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sdap->GetStartPosition();
	}
	return m_sdap.GetStartPosition();
}

void CDB_SDAP::GetNext(POSITION& rNextPosition,T_SDAP_K& rKey,T_SDAP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_sdap->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_sdap.Lookup(rKey, rData);
		return;
	}
	m_sdap.GetNextAssoc(rNextPosition,rKey,rData);
}


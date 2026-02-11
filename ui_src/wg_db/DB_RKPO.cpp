#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_RKPO.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RKPO::CDB_RKPO()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rkpo.InitHashTable(HASHSIZERKPO);
}

CDB_RKPO::~CDB_RKPO()
{

}

void CDB_RKPO::Add(T_RKPO_K Key,T_RKPO_D& rData,CDB_ELEM* pElem)
{
	T_RKPO_D Data;
	BOOL bExist=m_rkpo.Lookup(Key, Data);
	m_rkpo.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_RKPO_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_RKPO_CMD, Key);
	}
}

BOOL CDB_RKPO::Del(T_RKPO_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_rkpo.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_RKPO_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_RKPO_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RKPO::Get(T_RKPO_K Key,T_RKPO_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_rkpo->Lookup(Key, uTemp)) return FALSE;
	}
	return m_rkpo.Lookup(Key,rData);
}

int CDB_RKPO::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rkpo->GetCount();
	}
	return m_rkpo.GetCount();
}

POSITION CDB_RKPO::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rkpo->GetStartPosition();
	}
	return m_rkpo.GetStartPosition();
}

void CDB_RKPO::GetNext(POSITION& rNextPosition,T_RKPO_K& rKey,T_RKPO_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_rkpo->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_rkpo.Lookup(rKey, rData);
		return;
	}
	m_rkpo.GetNextAssoc(rNextPosition,rKey,rData);
}


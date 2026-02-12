#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_RBTE.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RBTE::CDB_RBTE()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rbte.InitHashTable(HASHSIZERBTE);
}

CDB_RBTE::~CDB_RBTE()
{

}

void CDB_RBTE::Add(T_RBTE_K Key,T_RBTE_D& rData,CDB_ELEM* pElem)
{
	T_RBTE_D Data;
	BOOL bExist=m_rbte.Lookup(Key, Data);
	m_rbte.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_RBTE_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_RBTE_CMD, Key);
	}
}

BOOL CDB_RBTE::Del(T_RBTE_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_rbte.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_RBTE_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_RBTE_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RBTE::Get(T_RBTE_K Key,T_RBTE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_rbte->Lookup(Key, uTemp)) return FALSE;
	}
	return m_rbte.Lookup(Key,rData);
}

int CDB_RBTE::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rbte->GetCount();
	}
	return m_rbte.GetCount();
}

POSITION CDB_RBTE::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rbte->GetStartPosition();
	}
	return m_rbte.GetStartPosition();
}

void CDB_RBTE::GetNext(POSITION& rNextPosition,T_RBTE_K& rKey,T_RBTE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_rbte->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_rbte.Lookup(rKey, rData);
		return;
	}
	m_rbte.GetNextAssoc(rNextPosition,rKey,rData);
}

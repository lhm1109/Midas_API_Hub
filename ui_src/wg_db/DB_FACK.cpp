#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_FACK.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_FACK::CDB_FACK()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_fack.InitHashTable(HASHSIZEFACK);
}

CDB_FACK::~CDB_FACK()
{
	
}

void CDB_FACK::Add(T_FACK_K Key,T_FACK_D& rData,CDB_ELEM* pElem)
{
	T_FACK_D Data;
	BOOL bExist=m_fack.Lookup(Key, Data);
	m_fack.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_FACK_CMD, Key);
	
	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_FACK_CMD, Key);
	}
}

BOOL CDB_FACK::Del(T_FACK_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_fack.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_FACK_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_FACK_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_FACK::Get(T_FACK_K Key,T_FACK_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_fack->Lookup(Key, uTemp)) return FALSE;
	}
	return m_fack.Lookup(Key,rData);
}

int CDB_FACK::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_fack->GetCount();
	}
	return m_fack.GetCount();
}

POSITION CDB_FACK::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_fack->GetStartPosition();
	}
	return m_fack.GetStartPosition();
}

void CDB_FACK::GetNext(POSITION& rNextPosition,T_FACK_K& rKey,T_FACK_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_fack->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_fack.Lookup(rKey, rData);
		return;
	}
	m_fack.GetNextAssoc(rNextPosition,rKey,rData);
}


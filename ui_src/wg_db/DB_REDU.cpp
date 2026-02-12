#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_REDU.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_REDU::CDB_REDU()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_redu.InitHashTable(HASHSIZEREDU);
}

CDB_REDU::~CDB_REDU()
{

}

void CDB_REDU::Add(T_REDU_K Key,T_REDU_D& rData,CDB_ELEM* pElem)
{
	T_REDU_D Data;
	BOOL bExist=m_redu.Lookup(Key, Data);
	m_redu.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_REDU_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_REDU_CMD, Key);
	}
}

BOOL CDB_REDU::Del(T_REDU_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_redu.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_REDU_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_REDU_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_REDU::Get(T_REDU_K Key,T_REDU_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_redu->Lookup(Key, uTemp)) return FALSE;
	}
	return m_redu.Lookup(Key,rData);
}

int CDB_REDU::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_redu->GetCount();
	}
	return m_redu.GetCount();
}

POSITION CDB_REDU::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_redu->GetStartPosition();
	}
	return m_redu.GetStartPosition();
}

void CDB_REDU::GetNext(POSITION& rNextPosition,T_REDU_K& rKey,T_REDU_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_redu->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_redu.Lookup(rKey, rData);
		return;
	}
	m_redu.GetNextAssoc(rNextPosition,rKey,rData);
}

#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_GREQ.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_GREQ::CDB_GREQ()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_greq.InitHashTable(HASHSIZEGREQ);
}

CDB_GREQ::~CDB_GREQ()
{

}

void CDB_GREQ::Add(T_GREQ_K Key,T_GREQ_D& rData,CDB_ELEM* pElem)
{
	T_GREQ_D Data;
	BOOL bExist=m_greq.Lookup(Key, Data);
	m_greq.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_GREQ_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_GREQ_CMD, Key);
	}
}

BOOL CDB_GREQ::Del(T_GREQ_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_greq.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_GREQ_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_GREQ_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_GREQ::Get(T_GREQ_K Key,T_GREQ_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_greq->Lookup(Key, uTemp)) return FALSE;
	}
	return m_greq.Lookup(Key,rData);
}

int CDB_GREQ::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_greq->GetCount();
	}
	return m_greq.GetCount();
}

POSITION CDB_GREQ::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_greq->GetStartPosition();
	}
	return m_greq.GetStartPosition();
}

void CDB_GREQ::GetNext(POSITION& rNextPosition,T_GREQ_K& rKey,T_GREQ_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_greq->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_greq.Lookup(rKey, rData);
		return;
	}
	m_greq.GetNextAssoc(rNextPosition,rKey,rData);
}

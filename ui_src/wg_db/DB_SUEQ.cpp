#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SUEQ.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SUEQ::CDB_SUEQ()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sueq.InitHashTable(HASHSIZESUEQ);
}

CDB_SUEQ::~CDB_SUEQ()
{

}

void CDB_SUEQ::Add(T_SUEQ_K Key,T_SUEQ_D& rData,CDB_ELEM* pElem)
{
	T_SUEQ_D Data;
	BOOL bExist=m_sueq.Lookup(Key, Data);
	m_sueq.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_SUEQ_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SUEQ_CMD, Key);
	}
}

BOOL CDB_SUEQ::Del(T_SUEQ_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_sueq.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_SUEQ_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SUEQ_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SUEQ::Get(T_SUEQ_K Key,T_SUEQ_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_sueq->Lookup(Key, uTemp)) return FALSE;
	}
	return m_sueq.Lookup(Key,rData);
}

int CDB_SUEQ::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sueq->GetCount();
	}
	return m_sueq.GetCount();
}

POSITION CDB_SUEQ::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sueq->GetStartPosition();
	}
	return m_sueq.GetStartPosition();
}

void CDB_SUEQ::GetNext(POSITION& rNextPosition,T_SUEQ_K& rKey,T_SUEQ_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_sueq->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_sueq.Lookup(rKey, rData);
		return;
	}
	m_sueq.GetNextAssoc(rNextPosition,rKey,rData);
}

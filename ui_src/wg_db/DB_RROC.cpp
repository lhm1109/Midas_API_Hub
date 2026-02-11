#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_RROC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RROC::CDB_RROC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rroc.InitHashTable(HASHSIZERROC);
}

CDB_RROC::~CDB_RROC()
{

}

void CDB_RROC::Add(T_RROC_K Key,T_RROC_D& rData,CDB_ELEM* pElem)
{
	T_RROC_D Data;
	BOOL bExist=m_rroc.Lookup(Key, Data);
	m_rroc.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_RROC_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_RROC_CMD, Key);
	}
}

BOOL CDB_RROC::Del(T_RROC_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_rroc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_RROC_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_RROC_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RROC::Get(T_RROC_K Key,T_RROC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_rroc->Lookup(Key, uTemp)) return FALSE;
	}
	return m_rroc.Lookup(Key,rData);
}

int CDB_RROC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rroc->GetCount();
	}
	return m_rroc.GetCount();
}

POSITION CDB_RROC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rroc->GetStartPosition();
	}
	return m_rroc.GetStartPosition();
}

void CDB_RROC::GetNext(POSITION& rNextPosition,T_RROC_K& rKey,T_RROC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_rroc->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_rroc.Lookup(rKey, rData);
		return;
	}
	m_rroc.GetNextAssoc(rNextPosition,rKey,rData);
}


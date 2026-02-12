#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_RKCE.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RKCE::CDB_RKCE()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rkce.InitHashTable(HASHSIZERKCE);
}

CDB_RKCE::~CDB_RKCE()
{

}

void CDB_RKCE::Add(T_RKCE_K Key,T_RKCE_D& rData,CDB_ELEM* pElem)
{
	T_RKCE_D Data;
	BOOL bExist=m_rkce.Lookup(Key, Data);
	m_rkce.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_RKCE_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_RKCE_CMD, Key);
	}
}

BOOL CDB_RKCE::Del(T_RKCE_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_rkce.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_RKCE_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_RKCE_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RKCE::Get(T_RKCE_K Key,T_RKCE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_rkce->Lookup(Key, uTemp)) return FALSE;
	}
	return m_rkce.Lookup(Key,rData);
}

int CDB_RKCE::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rkce->GetCount();
	}
	return m_rkce.GetCount();
}

POSITION CDB_RKCE::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rkce->GetStartPosition();
	}
	return m_rkce.GetStartPosition();
}

void CDB_RKCE::GetNext(POSITION& rNextPosition,T_RKCE_K& rKey,T_RKCE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_rkce->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_rkce.Lookup(rKey, rData);
		return;
	}
	m_rkce.GetNextAssoc(rNextPosition,rKey,rData);
}


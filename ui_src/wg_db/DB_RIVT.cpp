#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_RIVT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RIVT::CDB_RIVT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rivt.InitHashTable(HASHSIZERIVT);
}

CDB_RIVT::~CDB_RIVT()
{

}

void CDB_RIVT::Add(T_RIVT_K Key,T_RIVT_D& rData,CDB_ELEM* pElem)
{
	T_RIVT_D Data;
	BOOL bExist=m_rivt.Lookup(Key, Data);
	m_rivt.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_RIVT_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_RIVT_CMD, Key);
	}
}

BOOL CDB_RIVT::Del(T_RIVT_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_rivt.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_RIVT_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_RIVT_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RIVT::Get(T_RIVT_K Key,T_RIVT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_rivt->Lookup(Key, uTemp)) return FALSE;
	}
	return m_rivt.Lookup(Key,rData);
}

int CDB_RIVT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rivt->GetCount();
	}
	return m_rivt.GetCount();
}

POSITION CDB_RIVT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rivt->GetStartPosition();
	}
	return m_rivt.GetStartPosition();
}

void CDB_RIVT::GetNext(POSITION& rNextPosition,T_RIVT_K& rKey,T_RIVT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_rivt->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_rivt.Lookup(rKey, rData);
		return;
	}
	m_rivt.GetNextAssoc(rNextPosition,rKey,rData);
}

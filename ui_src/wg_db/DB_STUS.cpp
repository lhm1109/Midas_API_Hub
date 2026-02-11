#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_STUS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_STUS::CDB_STUS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_stus.InitHashTable(HASHSIZESTUS);
}

CDB_STUS::~CDB_STUS()
{

}

void CDB_STUS::Add(T_STUS_K Key,T_STUS_D& rData,CDB_ELEM* pElem)
{
	T_STUS_D Data;
	BOOL bExist=m_stus.Lookup(Key, Data);
	m_stus.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_STUS_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_STUS_CMD, Key);
	}
}

BOOL CDB_STUS::Del(T_STUS_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_stus.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_STUS_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_STUS_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_STUS::Get(T_STUS_K Key,T_STUS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_stus->Lookup(Key, uTemp)) return FALSE;
	}
	return m_stus.Lookup(Key,rData);
}

int CDB_STUS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_stus->GetCount();
	}
	return m_stus.GetCount();
}

POSITION CDB_STUS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_stus->GetStartPosition();
	}
	return m_stus.GetStartPosition();
}

void CDB_STUS::GetNext(POSITION& rNextPosition,T_STUS_K& rKey,T_STUS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_stus->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_stus.Lookup(rKey, rData);
		return;
	}
	m_stus.GetNextAssoc(rNextPosition,rKey,rData);
}

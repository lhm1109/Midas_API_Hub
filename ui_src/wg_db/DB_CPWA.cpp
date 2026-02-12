#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CPWA.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CPWA::CDB_CPWA()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cpwa.InitHashTable(HASHSIZECPWA);
}

CDB_CPWA::~CDB_CPWA()
{

}

void CDB_CPWA::Add(T_CPWA_K Key,T_CPWA_D& rData,CDB_ELEM* pElem)
{
	T_CPWA_D Data;
	BOOL bExist=m_cpwa.Lookup(Key, Data);
	m_cpwa.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_CPWA_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_CPWA_CMD, Key);
	}
}

BOOL CDB_CPWA::Del(T_CPWA_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_cpwa.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_CPWA_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_CPWA_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CPWA::Get(T_CPWA_K Key,T_CPWA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_cpwa->Lookup(Key, uTemp)) return FALSE;
	}
	return m_cpwa.Lookup(Key,rData);
}

int CDB_CPWA::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cpwa->GetCount();
	}
	return m_cpwa.GetCount();
}

POSITION CDB_CPWA::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cpwa->GetStartPosition();
	}
	return m_cpwa.GetStartPosition();
}

void CDB_CPWA::GetNext(POSITION& rNextPosition,T_CPWA_K& rKey,T_CPWA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_cpwa->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_cpwa.Lookup(rKey, rData);
		return;
	}
	m_cpwa.GetNextAssoc(rNextPosition,rKey,rData);
}

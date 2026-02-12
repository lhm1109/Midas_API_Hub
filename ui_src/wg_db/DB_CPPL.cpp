#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CPPL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CPPL::CDB_CPPL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cppl.InitHashTable(HASHSIZECPPL);
}

CDB_CPPL::~CDB_CPPL()
{

}

void CDB_CPPL::Add(T_CPPL_K Key,T_CPPL_D& rData,CDB_ELEM* pElem)
{
	T_CPPL_D Data;
	BOOL bExist=m_cppl.Lookup(Key, Data);
	m_cppl.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_CPPL_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_CPPL_CMD, Key);
	}
}

BOOL CDB_CPPL::Del(T_CPPL_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_cppl.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_CPPL_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_CPPL_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CPPL::Get(T_CPPL_K Key,T_CPPL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_cppl->Lookup(Key, uTemp)) return FALSE;
	}
	return m_cppl.Lookup(Key,rData);
}

int CDB_CPPL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cppl->GetCount();
	}
	return m_cppl.GetCount();
}

POSITION CDB_CPPL::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cppl->GetStartPosition();
	}
	return m_cppl.GetStartPosition();
}

void CDB_CPPL::GetNext(POSITION& rNextPosition,T_CPPL_K& rKey,T_CPPL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_cppl->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_cppl.Lookup(rKey, rData);
		return;
	}
	m_cppl.GetNextAssoc(rNextPosition,rKey,rData);
}

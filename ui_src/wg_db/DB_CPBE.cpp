#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CPBE.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CPBE::CDB_CPBE()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cpbe.InitHashTable(HASHSIZECPBE);
}

CDB_CPBE::~CDB_CPBE()
{

}

void CDB_CPBE::Add(T_CPBE_K Key,T_CPBE_D& rData,CDB_ELEM* pElem)
{
	T_CPBE_D Data;
	BOOL bExist=m_cpbe.Lookup(Key, Data);
	m_cpbe.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_CPBE_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_CPBE_CMD, Key);
	}
}

BOOL CDB_CPBE::Del(T_CPBE_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_cpbe.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_CPBE_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_CPBE_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CPBE::Get(T_CPBE_K Key,T_CPBE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_cpbe->Lookup(Key, uTemp)) return FALSE;
	}
	return m_cpbe.Lookup(Key,rData);
}

int CDB_CPBE::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cpbe->GetCount();
	}
	return m_cpbe.GetCount();
}

POSITION CDB_CPBE::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cpbe->GetStartPosition();
	}
	return m_cpbe.GetStartPosition();
}

void CDB_CPBE::GetNext(POSITION& rNextPosition,T_CPBE_K& rKey,T_CPBE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_cpbe->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_cpbe.Lookup(rKey, rData);
		return;
	}
	m_cpbe.GetNextAssoc(rNextPosition,rKey,rData);
}

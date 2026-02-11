#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CPCO.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CPCO::CDB_CPCO()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cpco.InitHashTable(HASHSIZECPCO);
}

CDB_CPCO::~CDB_CPCO()
{

}

void CDB_CPCO::Add(T_CPCO_K Key,T_CPCO_D& rData,CDB_ELEM* pElem)
{
	T_CPCO_D Data;
	BOOL bExist=m_cpco.Lookup(Key, Data);
	m_cpco.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_CPCO_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_CPCO_CMD, Key);
	}
}

BOOL CDB_CPCO::Del(T_CPCO_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_cpco.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_CPCO_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_CPCO_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CPCO::Get(T_CPCO_K Key,T_CPCO_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_cpco->Lookup(Key, uTemp)) return FALSE;
	}
	return m_cpco.Lookup(Key,rData);
}

int CDB_CPCO::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cpco->GetCount();
	}
	return m_cpco.GetCount();
}

POSITION CDB_CPCO::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_cpco->GetStartPosition();
	}
	return m_cpco.GetStartPosition();
}

void CDB_CPCO::GetNext(POSITION& rNextPosition,T_CPCO_K& rKey,T_CPCO_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_cpco->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_cpco.Lookup(rKey, rData);
		return;
	}
	m_cpco.GetNextAssoc(rNextPosition,rKey,rData);
}

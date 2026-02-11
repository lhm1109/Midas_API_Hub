#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SRAR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SRAR::CDB_SRAR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_srar.InitHashTable(HASHSIZESRAR);
}

CDB_SRAR::~CDB_SRAR()
{

}

void CDB_SRAR::Add(T_SRAR_K Key, T_SRAR_D& rData, CDB_ELEM* pElem)
{
	T_SRAR_D Data;
	BOOL bExist = m_srar.Lookup(Key, Data);
	m_srar.SetAt(Key, rData);
	if (!bExist)pElem->AddListItem(Key, LT_SRAR_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SRAR_CMD, Key);
	}
}

BOOL CDB_SRAR::Del(T_SRAR_K Key, CDB_ELEM* pElem)
{
	BOOL ret = m_srar.RemoveKey(Key);
	ASSERT(ret);
	if (ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_SRAR_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SRAR_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SRAR::Get(T_SRAR_K Key, T_SRAR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_srar->Lookup(Key, uTemp)) return FALSE;
	}
	return m_srar.Lookup(Key, rData);
}

int CDB_SRAR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_srar->GetCount();
	}
	return m_srar.GetCount();
}

POSITION CDB_SRAR::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_srar->GetStartPosition();
	}
	return m_srar.GetStartPosition();
}

void CDB_SRAR::GetNext(POSITION& rNextPosition, T_SRAR_K& rKey, T_SRAR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_srar->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_srar.Lookup(rKey, rData);
		return;
	}
	m_srar.GetNextAssoc(rNextPosition, rKey, rData);
}


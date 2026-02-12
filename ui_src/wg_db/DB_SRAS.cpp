#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SRAS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SRAS::CDB_SRAS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sras.InitHashTable(HASHSIZESRAS);
}

CDB_SRAS::~CDB_SRAS()
{

}

void CDB_SRAS::Add(T_SRAS_K Key, T_SRAS_D& rData, CDB_ELEM* pElem)
{
	T_SRAS_D Data;
	BOOL bExist = m_sras.Lookup(Key, Data);
	m_sras.SetAt(Key, rData);
	if (!bExist)pElem->AddListItem(Key, LT_SRAS_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SRAS_CMD, Key);
	}
}

BOOL CDB_SRAS::Del(T_SRAS_K Key, CDB_ELEM* pElem)
{
	BOOL ret = m_sras.RemoveKey(Key);
	ASSERT(ret);
	if (ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_SRAS_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SRAS_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SRAS::Get(T_SRAS_K Key, T_SRAS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_sras->Lookup(Key, uTemp)) return FALSE;
	}
	return m_sras.Lookup(Key, rData);
}

int CDB_SRAS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sras->GetCount();
	}
	return m_sras.GetCount();
}

POSITION CDB_SRAS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sras->GetStartPosition();
	}
	return m_sras.GetStartPosition();
}

void CDB_SRAS::GetNext(POSITION& rNextPosition, T_SRAS_K& rKey, T_SRAS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_sras->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_sras.Lookup(rKey, rData);
		return;
	}
	m_sras.GetNextAssoc(rNextPosition, rKey, rData);
}


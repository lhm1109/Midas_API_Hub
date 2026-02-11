#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_EBSL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_EBSL::CDB_EBSL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ebsl.InitHashTable(HASHSIZEEBSL);
}

CDB_EBSL::~CDB_EBSL()
{

}

void CDB_EBSL::Add(T_EBSL_K Key, T_EBSL_D& rData, CDB_ELEM* pElem)
{
	T_EBSL_D Data;
	BOOL bExist = m_ebsl.Lookup(Key, Data);
	m_ebsl.SetAt(Key, rData);
	if (!bExist)pElem->AddListItem(Key, LT_EBSL_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_EBSL_CMD, Key);
	}
}

BOOL CDB_EBSL::Del(T_EBSL_K Key, CDB_ELEM* pElem)
{
	BOOL ret = m_ebsl.RemoveKey(Key);
	ASSERT(ret);
	if (ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_EBSL_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_EBSL_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_EBSL::Get(T_EBSL_K Key, T_EBSL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_ebsl->Lookup(Key, uTemp)) return FALSE;
	}
	return m_ebsl.Lookup(Key, rData);
}

int CDB_EBSL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_ebsl->GetCount();
	}
	return m_ebsl.GetCount();
}

POSITION CDB_EBSL::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_ebsl->GetStartPosition();
	}
	return m_ebsl.GetStartPosition();
}

void CDB_EBSL::GetNext(POSITION& rNextPosition, T_EBSL_K& rKey, T_EBSL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_ebsl->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_ebsl.Lookup(rKey, rData);
		return;
	}
	m_ebsl.GetNextAssoc(rNextPosition, rKey, rData);
}


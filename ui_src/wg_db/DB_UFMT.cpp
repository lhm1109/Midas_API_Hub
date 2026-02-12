#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_UFMT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_UFMT::CDB_UFMT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ufmt.InitHashTable(HASHSIZEUFMT);
}

CDB_UFMT::~CDB_UFMT()
{

}

void CDB_UFMT::Add(T_UFMT_K Key,T_UFMT_D& rData,CDB_ELEM* pElem)
{
	T_UFMT_D Data;
	BOOL bExist=m_ufmt.Lookup(Key, Data);
	m_ufmt.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_UFMT_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_UFMT_CMD, Key);
	}
}

BOOL CDB_UFMT::Del(T_UFMT_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_ufmt.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_UFMT_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_UFMT_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_UFMT::Get(T_UFMT_K Key,T_UFMT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_ufmt->Lookup(Key, uTemp)) return FALSE;
	}
	return m_ufmt.Lookup(Key,rData);
}

int CDB_UFMT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_ufmt->GetCount();
	}
	return m_ufmt.GetCount();
}

POSITION CDB_UFMT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_ufmt->GetStartPosition();
	}
	return m_ufmt.GetStartPosition();
}

void CDB_UFMT::GetNext(POSITION& rNextPosition,T_UFMT_K& rKey,T_UFMT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_ufmt->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_ufmt.Lookup(rKey, rData);
		return;
	}
	m_ufmt.GetNextAssoc(rNextPosition,rKey,rData);
}

#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SPLN.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SPLN::CDB_SPLN()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_spln.InitHashTable(HASHSIZESPLN);
}

CDB_SPLN::~CDB_SPLN()
{

}

void CDB_SPLN::Add(T_SPLN_K Key,T_SPLN_D& rData,CDB_ELEM* pElem)
{
	T_SPLN_D Data;
	BOOL bExist=m_spln.Lookup(Key, Data);
	m_spln.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_SPLN_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SPLN_CMD, Key);
	}
}

BOOL CDB_SPLN::Del(T_SPLN_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_spln.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_SPLN_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SPLN_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SPLN::Get(T_SPLN_K Key,T_SPLN_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_spln->Lookup(Key, uTemp)) return FALSE;
	}
	return m_spln.Lookup(Key,rData);
}

int CDB_SPLN::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_spln->GetCount();
	}
	return m_spln.GetCount();
}

POSITION CDB_SPLN::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_spln->GetStartPosition();
	}
	return m_spln.GetStartPosition();
}

void CDB_SPLN::GetNext(POSITION& rNextPosition,T_SPLN_K& rKey,T_SPLN_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_spln->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_spln.Lookup(rKey, rData);
		return;
	}
	m_spln.GetNextAssoc(rNextPosition,rKey,rData);
}

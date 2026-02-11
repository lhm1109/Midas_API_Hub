#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_STRE.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_STRE::CDB_STRE()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_stre.InitHashTable(HASHSIZESTRE);
}

CDB_STRE::~CDB_STRE()
{

}

void CDB_STRE::Add(T_STRE_K Key,T_STRE_D& rData,CDB_ELEM* pElem)
{
	T_STRE_D Data;
	BOOL bExist=m_stre.Lookup(Key, Data);
	m_stre.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_STRE_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_STRE_CMD, Key);
	}
}

BOOL CDB_STRE::Del(T_STRE_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_stre.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_STRE_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_STRE_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_STRE::Get(T_STRE_K Key,T_STRE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_stre->Lookup(Key, uTemp)) return FALSE;
	}
	return m_stre.Lookup(Key,rData);
}

int CDB_STRE::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_stre->GetCount();
	}
	return m_stre.GetCount();
}

POSITION CDB_STRE::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_stre->GetStartPosition();
	}
	return m_stre.GetStartPosition();
}

void CDB_STRE::GetNext(POSITION& rNextPosition,T_STRE_K& rKey,T_STRE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_stre->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_stre.Lookup(rKey, rData);
		return;
	}
	m_stre.GetNextAssoc(rNextPosition,rKey,rData);
}

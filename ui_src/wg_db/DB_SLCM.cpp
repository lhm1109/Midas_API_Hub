#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SLCM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SLCM::CDB_SLCM()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_slcm.InitHashTable(HASHSIZESLCM);
}

CDB_SLCM::~CDB_SLCM()
{

}

void CDB_SLCM::Add(T_SLCM_K Key,T_SLCM_D& rData,CDB_ELEM* pElem)
{
	T_SLCM_D Data;
	BOOL bExist=m_slcm.Lookup(Key, Data);
	m_slcm.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_SLCM_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SLCM_CMD, Key);
	}
}

BOOL CDB_SLCM::Del(T_SLCM_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_slcm.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_SLCM_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SLCM_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SLCM::Get(T_SLCM_K Key,T_SLCM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_slcm->Lookup(Key, uTemp)) return FALSE;
	}
	return m_slcm.Lookup(Key,rData);
}

int CDB_SLCM::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_slcm->GetCount();
	}
	return m_slcm.GetCount();
}

POSITION CDB_SLCM::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_slcm->GetStartPosition();
	}
	return m_slcm.GetStartPosition();
}

void CDB_SLCM::GetNext(POSITION& rNextPosition,T_SLCM_K& rKey,T_SLCM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_slcm->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_slcm.Lookup(rKey, rData);
		return;
	}
	m_slcm.GetNextAssoc(rNextPosition,rKey,rData);
}

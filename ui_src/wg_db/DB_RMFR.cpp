#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_RMFR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RMFR::CDB_RMFR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rmfr.InitHashTable(HASHSIZERMFR);
}

CDB_RMFR::~CDB_RMFR()
{

}

void CDB_RMFR::Add(T_RMFR_K Key,T_RMFR_D& rData,CDB_ELEM* pElem)
{
	T_RMFR_D Data;
	BOOL bExist=m_rmfr.Lookup(Key, Data);
	m_rmfr.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_RMFR_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_RMFR_CMD, Key);
	}
}

BOOL CDB_RMFR::Del(T_RMFR_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_rmfr.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_RMFR_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_RMFR_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RMFR::Get(T_RMFR_K Key,T_RMFR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_rmfr->Lookup(Key, uTemp)) return FALSE;
	}
	return m_rmfr.Lookup(Key,rData);
}

int CDB_RMFR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rmfr->GetCount();
	}
	return m_rmfr.GetCount();
}

POSITION CDB_RMFR::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rmfr->GetStartPosition();
	}
	return m_rmfr.GetStartPosition();
}

void CDB_RMFR::GetNext(POSITION& rNextPosition,T_RMFR_K& rKey,T_RMFR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_rmfr->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_rmfr.Lookup(rKey, rData);
		return;
	}
	m_rmfr.GetNextAssoc(rNextPosition,rKey,rData);
}

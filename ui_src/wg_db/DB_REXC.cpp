#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_REXC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_REXC::CDB_REXC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rexc.InitHashTable(HASHSIZEREXC);
}

CDB_REXC::~CDB_REXC()
{

}

void CDB_REXC::Add(T_REXC_K Key,T_REXC_D& rData,CDB_ELEM* pElem)
{
	T_REXC_D Data;
	BOOL bExist=m_rexc.Lookup(Key, Data);
	m_rexc.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_REXC_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_REXC_CMD, Key);
	}
}

BOOL CDB_REXC::Del(T_REXC_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_rexc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_REXC_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_REXC_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_REXC::Get(T_REXC_K Key,T_REXC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_rexc->Lookup(Key, uTemp)) return FALSE;
	}
	return m_rexc.Lookup(Key,rData);
}

int CDB_REXC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rexc->GetCount();
	}
	return m_rexc.GetCount();
}

POSITION CDB_REXC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rexc->GetStartPosition();
	}
	return m_rexc.GetStartPosition();
}

void CDB_REXC::GetNext(POSITION& rNextPosition,T_REXC_K& rKey,T_REXC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_rexc->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_rexc.Lookup(rKey, rData);
		return;
	}
	m_rexc.GetNextAssoc(rNextPosition,rKey,rData);
}


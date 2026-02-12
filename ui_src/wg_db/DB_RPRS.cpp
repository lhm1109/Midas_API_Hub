#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_RPRS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RPRS::CDB_RPRS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rprs.InitHashTable(HASHSIZERPRS);
}

CDB_RPRS::~CDB_RPRS()
{

}

void CDB_RPRS::Add(T_RPRS_K Key,T_RPRS_D& rData,CDB_ELEM* pElem)
{
	T_RPRS_D Data;
	BOOL bExist=m_rprs.Lookup(Key, Data);
	m_rprs.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_RPRS_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_RPRS_CMD, Key);
	}
}

BOOL CDB_RPRS::Del(T_RPRS_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_rprs.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_RPRS_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_RPRS_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RPRS::Get(T_RPRS_K Key,T_RPRS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_rprs->Lookup(Key, uTemp)) return FALSE;
	}
	return m_rprs.Lookup(Key,rData);
}

int CDB_RPRS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rprs->GetCount();
	}
	return m_rprs.GetCount();
}

POSITION CDB_RPRS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rprs->GetStartPosition();
	}
	return m_rprs.GetStartPosition();
}

void CDB_RPRS::GetNext(POSITION& rNextPosition,T_RPRS_K& rKey,T_RPRS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_rprs->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_rprs.Lookup(rKey, rData);
		return;
	}
	m_rprs.GetNextAssoc(rNextPosition,rKey,rData);
}


#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SDSW.h"

#include "DBDoc.h"
#include "StagCtrl.h"
CDB_SDSW::CDB_SDSW()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sdsw.InitHashTable(HASHSIZESDSW);
}

CDB_SDSW::~CDB_SDSW()
{
}

void CDB_SDSW::Add(T_SDSW_K Key,T_SDSW_D& rData,CDB_ELEM* pElem)
{
	T_SDSW_D Data;
	BOOL bExist=m_sdsw.Lookup(Key, Data);
	m_sdsw.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_SDSW_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SDSW_CMD, Key);
	}
}

BOOL CDB_SDSW::Del(T_SDSW_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_sdsw.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_SDSW_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SDSW_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SDSW::Get(T_SDSW_K Key,T_SDSW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_sdsw->Lookup(Key, uTemp)) return FALSE;
	}
	return m_sdsw.Lookup(Key,rData);
}

int CDB_SDSW::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sdsw->GetCount();
	}
	return m_sdsw.GetCount();
}

POSITION CDB_SDSW::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sdsw->GetStartPosition();
	}
	return m_sdsw.GetStartPosition();
}

void CDB_SDSW::GetNext(POSITION& rNextPosition,T_SDSW_K& rKey,T_SDSW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_sdsw->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_sdsw.Lookup(rKey, rData);
		return;
	}
	m_sdsw.GetNextAssoc(rNextPosition,rKey,rData);
}

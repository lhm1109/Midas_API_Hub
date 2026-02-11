#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SDTS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SDTS::CDB_SDTS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sdts.InitHashTable(HASHSIZESDTS);
}

CDB_SDTS::~CDB_SDTS()
{

}

void CDB_SDTS::Add(T_SDTS_K Key,T_SDTS_D& rData,CDB_ELEM* pElem)
{
	T_SDTS_D Data;
	BOOL bExist=m_sdts.Lookup(Key, Data);
	m_sdts.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_SDTS_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SDTS_CMD, Key);
	}
}

BOOL CDB_SDTS::Del(T_SDTS_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_sdts.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_SDTS_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SDTS_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SDTS::Get(T_SDTS_K Key,T_SDTS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_sdts->Lookup(Key, uTemp)) return FALSE;
	}
	return m_sdts.Lookup(Key,rData);
}

int CDB_SDTS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sdts->GetCount();
	}
	return m_sdts.GetCount();
}

POSITION CDB_SDTS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sdts->GetStartPosition();
	}
	return m_sdts.GetStartPosition();
}

void CDB_SDTS::GetNext(POSITION& rNextPosition,T_SDTS_K& rKey,T_SDTS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_sdts->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_sdts.Lookup(rKey, rData);
		return;
	}
	m_sdts.GetNextAssoc(rNextPosition,rKey,rData);
}


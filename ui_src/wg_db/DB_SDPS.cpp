#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SDPS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SDPS::CDB_SDPS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sdps.InitHashTable(HASHSIZESDPS);
}

CDB_SDPS::~CDB_SDPS()
{

}

void CDB_SDPS::Add(T_SDPS_K Key,T_SDPS_D& rData,CDB_ELEM* pElem)
{
	T_SDPS_D Data;
	BOOL bExist=m_sdps.Lookup(Key, Data);
	m_sdps.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_SDPS_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SDPS_CMD, Key);
	}
}

BOOL CDB_SDPS::Del(T_SDPS_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_sdps.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_SDPS_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SDPS_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SDPS::Get(T_SDPS_K Key,T_SDPS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_sdps->Lookup(Key, uTemp)) return FALSE;
	}
	return m_sdps.Lookup(Key,rData);
}

int CDB_SDPS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sdps->GetCount();
	}
	return m_sdps.GetCount();
}

POSITION CDB_SDPS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sdps->GetStartPosition();
	}
	return m_sdps.GetStartPosition();
}

void CDB_SDPS::GetNext(POSITION& rNextPosition,T_SDPS_K& rKey,T_SDPS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_sdps->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_sdps.Lookup(rKey, rData);
		return;
	}
	m_sdps.GetNextAssoc(rNextPosition,rKey,rData);
}


#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SDDO.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SDDO::CDB_SDDO()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sddo.InitHashTable(HASHSIZESDDO);
}

CDB_SDDO::~CDB_SDDO()
{

}

void CDB_SDDO::Add(T_SDDO_K Key,T_SDDO_D& rData,CDB_ELEM* pElem)
{
	T_SDDO_D Data;
	BOOL bExist=m_sddo.Lookup(Key, Data);
	m_sddo.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_SDDO_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SDDO_CMD, Key);
	}
}

BOOL CDB_SDDO::Del(T_SDDO_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_sddo.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_SDDO_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SDDO_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SDDO::Get(T_SDDO_K Key,T_SDDO_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_sddo->Lookup(Key, uTemp)) return FALSE;
	}
	return m_sddo.Lookup(Key,rData);
}

int CDB_SDDO::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sddo->GetCount();
	}
	return m_sddo.GetCount();
}

POSITION CDB_SDDO::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sddo->GetStartPosition();
	}
	return m_sddo.GetStartPosition();
}

void CDB_SDDO::GetNext(POSITION& rNextPosition,T_SDDO_K& rKey,T_SDDO_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_sddo->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_sddo.Lookup(rKey, rData);
		return;
	}
	m_sddo.GetNextAssoc(rNextPosition,rKey,rData);
}


#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SDFC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SDFC::CDB_SDFC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sdfc.InitHashTable(HASHSIZESDFC);
}

CDB_SDFC::~CDB_SDFC()
{

}

void CDB_SDFC::Add(T_SDFC_K Key,T_SDFC_D& rData,CDB_ELEM* pElem)
{
	T_SDFC_D Data;
	BOOL bExist=m_sdfc.Lookup(Key, Data);
	m_sdfc.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_SDFC_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SDFC_CMD, Key);
	}
}

BOOL CDB_SDFC::Del(T_SDFC_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_sdfc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_SDFC_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SDFC_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SDFC::Get(T_SDFC_K Key,T_SDFC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_sdfc->Lookup(Key, uTemp)) return FALSE;
	}
	return m_sdfc.Lookup(Key,rData);
}

int CDB_SDFC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sdfc->GetCount();
	}
	return m_sdfc.GetCount();
}

POSITION CDB_SDFC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sdfc->GetStartPosition();
	}
	return m_sdfc.GetStartPosition();
}

void CDB_SDFC::GetNext(POSITION& rNextPosition,T_SDFC_K& rKey,T_SDFC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_sdfc->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_sdfc.Lookup(rKey, rData);
		return;
	}
	m_sdfc.GetNextAssoc(rNextPosition,rKey,rData);
}


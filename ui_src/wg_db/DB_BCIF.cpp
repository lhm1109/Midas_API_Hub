#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_BCIF.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_BCIF::CDB_BCIF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_bcif.InitHashTable(HASHSIZEBCIF);
}

CDB_BCIF::~CDB_BCIF()
{

}

void CDB_BCIF::Add(T_BCIF_K Key,T_BCIF_D& rData,CDB_ELEM* pElem)
{
	T_BCIF_D Data;
	BOOL bExist=m_bcif.Lookup(Key, Data);
	m_bcif.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_BCIF_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_BCIF_CMD, Key);
	}
}

BOOL CDB_BCIF::Del(T_BCIF_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_bcif.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_BCIF_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_BCIF_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_BCIF::Get(T_BCIF_K Key,T_BCIF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_bcif->Lookup(Key, uTemp)) return FALSE;
	}
	return m_bcif.Lookup(Key,rData);
}

int CDB_BCIF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_bcif->GetCount();
	}
	return m_bcif.GetCount();
}

POSITION CDB_BCIF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_bcif->GetStartPosition();
	}
	return m_bcif.GetStartPosition();
}

void CDB_BCIF::GetNext(POSITION& rNextPosition,T_BCIF_K& rKey,T_BCIF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_bcif->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_bcif.Lookup(rKey, rData);
		return;
	}
	m_bcif.GetNextAssoc(rNextPosition,rKey,rData);
}

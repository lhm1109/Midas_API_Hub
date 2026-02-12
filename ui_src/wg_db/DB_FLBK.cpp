#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_FLBK.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_FLBK::CDB_FLBK()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_flbk.InitHashTable(HASHSIZEFLBK);
}

CDB_FLBK::~CDB_FLBK()
{

}

void CDB_FLBK::Add(T_FLBK_K Key,T_FLBK_D& rData,CDB_ELEM* pElem)
{
	T_FLBK_D Data;
	BOOL bExist=m_flbk.Lookup(Key, Data);
	m_flbk.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_FLBK_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_FLBK_CMD, Key);
	}
}

BOOL CDB_FLBK::Del(T_FLBK_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_flbk.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_FLBK_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_FLBK_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_FLBK::Get(T_FLBK_K Key,T_FLBK_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_flbk->Lookup(Key, uTemp)) return FALSE;
	}
	return m_flbk.Lookup(Key,rData);
}

int CDB_FLBK::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_flbk->GetCount();
	}
	return m_flbk.GetCount();
}

POSITION CDB_FLBK::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_flbk->GetStartPosition();
	}
	return m_flbk.GetStartPosition();
}

void CDB_FLBK::GetNext(POSITION& rNextPosition,T_FLBK_K& rKey,T_FLBK_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_flbk->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_flbk.Lookup(rKey, rData);
		return;
	}
	m_flbk.GetNextAssoc(rNextPosition,rKey,rData);
}


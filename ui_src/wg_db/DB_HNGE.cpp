#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_HNGE.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_HNGE::CDB_HNGE()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_hnge.InitHashTable(HASHSIZEHNGE);
}

CDB_HNGE::~CDB_HNGE()
{

}

void CDB_HNGE::Add(T_HNGE_K Key,T_HNGE_D& rData,CDB_ELEM* pElem)
{
	T_HNGE_D Data;
	BOOL bExist=m_hnge.Lookup(Key.keymap, Data);
	m_hnge.SetAt(Key.keymap,rData);
	if(!bExist)
	{
		if(pElem) pElem->AddListItem(Key.key.entity, LT_HNGE_CMD, Key.keymap);
	}
	
	// Final Stage에서 Add/Del/Mod 될 수 있으므로
	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_HNGE_CMD, Key.keymap);
	}
}

BOOL CDB_HNGE::Del(T_HNGE_K Key,CDB_ELEM* pElem)
{
	T_HNGE_D Data;
	m_hnge.Lookup(Key.keymap, Data);
	BOOL ret=m_hnge.RemoveKey(Key.keymap);
	ASSERT(ret);
	if(ret)
	{
		if(pElem) pElem->DelListItem(Key.key.entity, LT_HNGE_CMD, Key.keymap);
		// Final Stage에서 Add/Del/Mod 될 수 있으므로
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
		{
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_HNGE_CMD, Key.keymap);
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_HNGE::Get(T_HNGE_K Key,T_HNGE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_hnge->Lookup(Key.keymap, uTemp)) return FALSE;
	}
	return m_hnge.Lookup(Key.keymap,rData);
}

int CDB_HNGE::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_hnge->GetCount();
	}
	return m_hnge.GetCount();
}

POSITION CDB_HNGE::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_hnge->GetStartPosition();
	}
	return m_hnge.GetStartPosition();
}

void CDB_HNGE::GetNext(POSITION& rNextPosition,T_HNGE_K& rKey,T_HNGE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_hnge->GetNextAssoc(rNextPosition, rKey.keymap, uTemp);
		m_hnge.Lookup(rKey.keymap, rData);
		return;
	}
	m_hnge.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_PRLS.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PRLS::CDB_PRLS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_prls.InitHashTable(HASHSIZEPRLS);
}

CDB_PRLS::~CDB_PRLS()
{

}

void CDB_PRLS::Add(T_PRLS_K Key,T_PRLS_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_PRLS_D Data;
	BOOL bExist=m_prls.Lookup(Key.keymap, Data);
	if (bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->DelListItem(Data.GroupKey, LT_PRLS_CMD, Key.keymap);
	m_prls.SetAt(Key.keymap,rData);
	if (bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->AddListItem(rData.GroupKey, LT_PRLS_CMD, Key.keymap);
	if(!bExist)
	{
		pElem->AddListItem(Key.key.entity, LT_PRLS_CMD, Key.keymap);
		pBngr->AddListItem(rData.GroupKey, LT_PRLS_CMD, Key.keymap);
	}

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_PRLS_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_PRLS::Del(T_PRLS_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_PRLS_D Data;
	m_prls.Lookup(Key.keymap, Data);
	BOOL ret=m_prls.RemoveKey(Key.keymap);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key.key.entity, LT_PRLS_CMD, Key.keymap));
		VERIFY(pBngr->DelListItem(Data.GroupKey, LT_PRLS_CMD, Key.keymap));

		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveBndr(LT_PRLS_CMD, Key.keymap);
		}
	}
	return ret;
}

//--------------------------------------------------------------------------
BOOL CDB_PRLS::Get(T_PRLS_K Key,T_PRLS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_prls->Lookup(Key.keymap, StagBngrKey)) return FALSE;
	}
	return m_prls.Lookup(Key.keymap,rData);
}

int CDB_PRLS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_prls->GetCount();
	}
	return m_prls.GetCount();
}

POSITION CDB_PRLS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_prls->GetStartPosition();
	}
	return m_prls.GetStartPosition();
}

void CDB_PRLS::GetNext(POSITION& rNextPosition,T_PRLS_K& rKey,T_PRLS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_prls->GetNextAssoc(rNextPosition, rKey.keymap, StagBngrKey);
		m_prls.Lookup(rKey.keymap, rData);
		return;
	}
	m_prls.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

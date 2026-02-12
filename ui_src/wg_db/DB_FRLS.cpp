#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_FRLS.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_FRLS::CDB_FRLS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_frls.InitHashTable(HASHSIZEFRLS);
}

CDB_FRLS::~CDB_FRLS()
{

}

void CDB_FRLS::Add(T_FRLS_K Key,T_FRLS_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_FRLS_D Data;
	BOOL bExist=m_frls.Lookup(Key.keymap, Data);
	if (bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->DelListItem(Data.GroupKey, LT_FRLS_CMD, Key.keymap);
	m_frls.SetAt(Key.keymap,rData);
	if (bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->AddListItem(rData.GroupKey, LT_FRLS_CMD, Key.keymap);
	if(!bExist)
	{
		if (pElem) pElem->AddListItem(Key.key.entity, LT_FRLS_CMD, Key.keymap);
		if (pBngr) pBngr->AddListItem(rData.GroupKey, LT_FRLS_CMD, Key.keymap);
	}

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_FRLS_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_FRLS::Del(T_FRLS_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_FRLS_D Data;
	m_frls.Lookup(Key.keymap, Data);
	BOOL ret=m_frls.RemoveKey(Key.keymap);
	ASSERT(ret);
	if(ret)
	{
		if (pElem) VERIFY(pElem->DelListItem(Key.key.entity, LT_FRLS_CMD, Key.keymap));
		if (pBngr) VERIFY(pBngr->DelListItem(Data.GroupKey, LT_FRLS_CMD, Key.keymap));

		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveBndr(LT_FRLS_CMD, Key.keymap);
		}
	}
	return ret;
}

//--------------------------------------------------------------------------
BOOL CDB_FRLS::Get(T_FRLS_K Key,T_FRLS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_frls->Lookup(Key.keymap, StagBngrKey)) return FALSE;
	}
	return m_frls.Lookup(Key.keymap,rData);
}

int CDB_FRLS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_frls->GetCount();
	}
	return m_frls.GetCount();
}

POSITION CDB_FRLS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_frls->GetStartPosition();
	}
	return m_frls.GetStartPosition();
}

void CDB_FRLS::GetNext(POSITION& rNextPosition,T_FRLS_K& rKey,T_FRLS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_frls->GetNextAssoc(rNextPosition, rKey.keymap, StagBngrKey);
		m_frls.Lookup(rKey.keymap, rData);
		return;
	}
	m_frls.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

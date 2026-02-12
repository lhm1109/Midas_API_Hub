#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_TMLD.h"
#include "DB_LDGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_TMLD::CDB_TMLD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_tmld.InitHashTable(HASHSIZETMLD);
}

CDB_TMLD::~CDB_TMLD()
{

}

void CDB_TMLD::Add(T_TMLD_K Key,T_TMLD_D& rData,CDB_ELEM* pElem,CDB_LDGR* pLdgr)
{
	T_TMLD_D Data;
	BOOL bExist=m_tmld.Lookup(Key.keymap, Data);
	if(bExist) pLdgr->DelListItem(Data.GroupKey, LT_TMLD_CMD, Key.keymap);
	m_tmld.SetAt(Key.keymap,rData);
	pLdgr->AddListItem(rData.GroupKey, LT_TMLD_CMD, Key.keymap);
	if(!bExist && pElem)pElem->AddListItem(Key.key.entity, LT_TMLD_CMD, Key.keymap);

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_TMLD_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_TMLD::Del(T_TMLD_K Key,CDB_ELEM* pElem,CDB_LDGR* pLdgr)
{
	T_TMLD_D Data;
	BOOL bExist = m_tmld.Lookup(Key.keymap, Data);
	if (bExist)
	{
		BOOL ret=m_tmld.RemoveKey(Key.keymap);
		ASSERT(ret);
		if (ret)
		{
			if(pLdgr)VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_TMLD_CMD, Key.keymap));
			if(pElem)VERIFY(pElem->DelListItem(Key.key.entity, LT_TMLD_CMD, Key.keymap));

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_TMLD_CMD, Key.keymap);
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_TMLD::Get(T_TMLD_K Key,T_TMLD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_tmld->Lookup(Key.keymap, StagLdgrKey)) return FALSE;
	}
	return m_tmld.Lookup(Key.keymap,rData);
}

int CDB_TMLD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_tmld->GetCount();
	}
	return m_tmld.GetCount();
}

POSITION CDB_TMLD::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_tmld->GetStartPosition();
	}
	return m_tmld.GetStartPosition();
}

void CDB_TMLD::GetNext(POSITION& rNextPosition,T_TMLD_K& rKey,T_TMLD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_tmld->GetNextAssoc(rNextPosition, rKey.keymap, StagLdgrKey);
		m_tmld.Lookup(rKey.keymap, rData);
		return;
	}
	m_tmld.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

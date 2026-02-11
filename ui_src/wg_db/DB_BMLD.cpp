#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_ELEM.h"
#include "DB_BMLD.h"
#include "DB_LDGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_BMLD::CDB_BMLD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_bmld.InitHashTable(HASHSIZEBMLD);
}

CDB_BMLD::~CDB_BMLD()
{

}

void CDB_BMLD::Add(T_BMLD_K Key,T_BMLD_D& rData,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr)
{
	T_BMLD_D Data;
	BOOL bExist=m_bmld.Lookup(Key.keymap, Data);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_BMLD_CMD, Key.keymap));
	if(bExist && Data.GroupKey != rData.GroupKey)
		VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_BMLD_CMD, Key.keymap));
	m_bmld.SetAt(Key.keymap,rData);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		pStld->AddListItem(rData.LoadCaseKey, LT_BMLD_CMD, Key.keymap);
	if(bExist && Data.GroupKey != rData.GroupKey)
		pLdgr->AddListItem(rData.GroupKey, LT_BMLD_CMD, Key.keymap);
	if(!bExist)
	{
		pStld->AddListItem(rData.LoadCaseKey, LT_BMLD_CMD, Key.keymap);
		pElem->AddListItem(Key.key.entity, LT_BMLD_CMD, Key.keymap);
		pLdgr->AddListItem(rData.GroupKey, LT_BMLD_CMD, Key.keymap);
	}

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Load는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_BMLD_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_BMLD::Del(T_BMLD_K Key,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr)
{
	T_BMLD_D Data;
	BOOL bExist=m_bmld.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_bmld.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_BMLD_CMD, Key.keymap));
			VERIFY(pElem->DelListItem(Key.key.entity, LT_BMLD_CMD, Key.keymap));
			VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_BMLD_CMD, Key.keymap));

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_BMLD_CMD, Key.keymap);
			}
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_BMLD::Get(T_BMLD_K Key,T_BMLD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_bmld->Lookup(Key.keymap, StagLdgrKey)) return FALSE;
	}
	return m_bmld.Lookup(Key.keymap,rData);
}

int CDB_BMLD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_bmld->GetCount();
	}
	return m_bmld.GetCount();
}

POSITION CDB_BMLD::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_bmld->GetStartPosition();
	}
	return m_bmld.GetStartPosition();
}

void CDB_BMLD::GetNext(POSITION& rNextPosition,T_BMLD_K& rKey,T_BMLD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_bmld->GetNextAssoc(rNextPosition, rKey.keymap, StagLdgrKey);
		m_bmld.Lookup(rKey.keymap, rData);
		return;
	}
	m_bmld.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

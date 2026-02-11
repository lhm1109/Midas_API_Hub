#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_ELEM.h"
#include "DB_PTNS.h"
#include "DB_LDGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PTNS::CDB_PTNS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ptns.InitHashTable(HASHSIZEPTNS);
}

CDB_PTNS::~CDB_PTNS()
{

}

void CDB_PTNS::Add(T_PTNS_K Key,T_PTNS_D& rData,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr)
{
	T_PTNS_D Data;
	BOOL bExist=m_ptns.Lookup(Key.keymap, Data);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_PTNS_CMD, Key.keymap));
	if(bExist && Data.GroupKey != rData.GroupKey)
		VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_PTNS_CMD, Key.keymap));
	m_ptns.SetAt(Key.keymap,rData);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		pStld->AddListItem(rData.LoadCaseKey, LT_PTNS_CMD, Key.keymap);
	if(bExist && Data.GroupKey != rData.GroupKey)
		pLdgr->AddListItem(rData.GroupKey, LT_PTNS_CMD, Key.keymap);
	if(!bExist)
	{
		pStld->AddListItem(rData.LoadCaseKey, LT_PTNS_CMD, Key.keymap);
		pElem->AddListItem(Key.key.entity, LT_PTNS_CMD, Key.keymap);
		pLdgr->AddListItem(rData.GroupKey, LT_PTNS_CMD, Key.keymap);
	}
	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Load는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_PTNS_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_PTNS::Del(T_PTNS_K Key,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr)
{
	T_PTNS_D Data;
	BOOL bExist=m_ptns.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_ptns.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_PTNS_CMD, Key.keymap));
			VERIFY(pElem->DelListItem(Key.key.entity, LT_PTNS_CMD, Key.keymap));
			VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_PTNS_CMD, Key.keymap));
			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_PTNS_CMD, Key.keymap);
			}
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_PTNS::Get(T_PTNS_K Key,T_PTNS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_ptns->Lookup(Key.keymap, StagLdgrKey)) return FALSE;
	}
	return m_ptns.Lookup(Key.keymap,rData);
}

int CDB_PTNS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_ptns->GetCount();
	}
	return m_ptns.GetCount();
}

POSITION CDB_PTNS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_ptns->GetStartPosition();
	}
	return m_ptns.GetStartPosition();
}

void CDB_PTNS::GetNext(POSITION& rNextPosition,T_PTNS_K& rKey,T_PTNS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_ptns->GetNextAssoc(rNextPosition, rKey.keymap, StagLdgrKey);
		m_ptns.Lookup(rKey.keymap, rData);
		return;
	}
	m_ptns.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

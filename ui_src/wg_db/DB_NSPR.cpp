#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_NSPR.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_NSPR::CDB_NSPR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nspr.InitHashTable(HASHSIZENSPR);
}

CDB_NSPR::~CDB_NSPR()
{

}

void CDB_NSPR::Add(T_NSPR_K Key,T_NSPR_D& rData,CDB_NODE* pNode,CDB_BNGR* pBngr)
{
	T_NSPR_D Data;
	BOOL bExist=m_nspr.Lookup(Key.keymap, Data);
	if(bExist && Data.GroupKey != rData.GroupKey && pBngr)
		VERIFY(pBngr->DelListItem(Data.GroupKey, LT_NSPR_CMD, Key.keymap));

	m_nspr.SetAt(Key.keymap,rData);
	if(bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->AddListItem(rData.GroupKey, LT_NSPR_CMD, Key.keymap);
	if(!bExist)
	{
		if(pNode)pNode->AddListItem(Key.key.entity, LT_NSPR_CMD, Key.keymap);
		if(pBngr)pBngr->AddListItem(rData.GroupKey, LT_NSPR_CMD, Key.keymap);
	}

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_NSPR_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_NSPR::Del(T_NSPR_K Key,CDB_NODE* pNode,CDB_BNGR* pBngr)
{
	T_NSPR_D Data;
	m_nspr.Lookup(Key.keymap, Data);
	BOOL ret=m_nspr.RemoveKey(Key.keymap);
	ASSERT(ret);
	if(ret)
	{
		if (pNode)VERIFY(pNode->DelListItem(Key.key.entity, LT_NSPR_CMD, Key.keymap));
		if (pBngr)VERIFY(pBngr->DelListItem(Data.GroupKey, LT_NSPR_CMD, Key.keymap));

		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveBndr(LT_NSPR_CMD, Key.keymap);
		}
	}
	return ret;
}

//--------------------------------------------------------------------------
BOOL CDB_NSPR::Get(T_NSPR_K Key,T_NSPR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_nspr->Lookup(Key.keymap, StagBngrKey)) return FALSE;
	}
	return m_nspr.Lookup(Key.keymap,rData);
}

int CDB_NSPR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_nspr->GetCount();
	}
	return m_nspr.GetCount();
}

POSITION CDB_NSPR::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_nspr->GetStartPosition();
	}
	return m_nspr.GetStartPosition();
}

void CDB_NSPR::GetNext(POSITION& rNextPosition,T_NSPR_K& rKey,T_NSPR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_nspr->GetNextAssoc(rNextPosition, rKey.keymap, StagBngrKey);
		m_nspr.Lookup(rKey.keymap, rData);
		return;
	}
	m_nspr.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

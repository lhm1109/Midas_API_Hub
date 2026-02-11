#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_CONS.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CONS::CDB_CONS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cons.InitHashTable(HASHSIZECONS);
}

CDB_CONS::~CDB_CONS()
{

}

void CDB_CONS::Add(T_CONS_K Key,T_CONS_D& rData,CDB_NODE* pNode,CDB_BNGR* pBngr)
{
	T_CONS_D Data;
	BOOL bExist=m_cons.Lookup(Key.keymap, Data);
	if(bExist && Data.GroupKey != rData.GroupKey && pBngr)
		VERIFY(pBngr->DelListItem(Data.GroupKey, LT_CONS_CMD, Key.keymap));
	m_cons.SetAt(Key.keymap,rData);
	if(bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->AddListItem(rData.GroupKey, LT_CONS_CMD, Key.keymap);
	if(!bExist)
	{
		if(pNode) pNode->AddListItem(Key.key.entity, LT_CONS_CMD, Key.keymap);
		if(pBngr) pBngr->AddListItem(rData.GroupKey, LT_CONS_CMD, Key.keymap);
	}

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_CONS_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_CONS::Del(T_CONS_K Key,CDB_NODE* pNode,CDB_BNGR* pBngr)
{
	T_CONS_D Data;
	m_cons.Lookup(Key.keymap, Data);
	BOOL ret=m_cons.RemoveKey(Key.keymap);
	ASSERT(ret);
	if(ret)
	{
		if(pNode) pNode->DelListItem(Key.key.entity, LT_CONS_CMD, Key.keymap);
		if(pBngr) pBngr->DelListItem(Data.GroupKey, LT_CONS_CMD, Key.keymap);

		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveBndr(LT_CONS_CMD, Key.keymap);
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CONS::Get(T_CONS_K Key,T_CONS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_cons->Lookup(Key.keymap, StagBngrKey)) return FALSE;
	}
	return m_cons.Lookup(Key.keymap,rData);
}

int CDB_CONS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_cons->GetCount();
	}
	return m_cons.GetCount();
}

POSITION CDB_CONS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_cons->GetStartPosition();
	}
	return m_cons.GetStartPosition();
}

void CDB_CONS::GetNext(POSITION& rNextPosition,T_CONS_K& rKey,T_CONS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_cons->GetNextAssoc(rNextPosition, rKey.keymap, StagBngrKey);
		m_cons.Lookup(rKey.keymap, rData);
		return;
	}
	m_cons.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

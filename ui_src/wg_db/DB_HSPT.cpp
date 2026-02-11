#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_HSPT.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_HSPT::CDB_HSPT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_hspt.InitHashTable(HASHSIZEHSPT);
}

CDB_HSPT::~CDB_HSPT()
{

}

void CDB_HSPT::Add(T_HSPT_K Key,T_HSPT_D& rData,CDB_NODE* pNode,CDB_BNGR* pBngr)
{
	T_HSPT_D Data;
	BOOL bExist=m_hspt.Lookup(Key.keymap, Data);
	if(bExist) pBngr->DelListItem(Data.GroupKey, LT_HSPT_CMD, Key.keymap);
	m_hspt.SetAt(Key.keymap,rData);
	pBngr->AddListItem(rData.GroupKey, LT_HSPT_CMD, Key.keymap);
	if(!bExist && pNode)pNode->AddListItem(Key.key.entity, LT_HSPT_CMD, Key.keymap);

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_HSPT_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_HSPT::Del(T_HSPT_K Key,CDB_NODE* pNode,CDB_BNGR* pBngr)
{
	T_HSPT_D Data;
	BOOL bExist = m_hspt.Lookup(Key.keymap, Data);
	if (bExist)
	{
		BOOL ret=m_hspt.RemoveKey(Key.keymap);
		ASSERT(ret);
		if (ret)
		{
			if(pBngr)VERIFY(pBngr->DelListItem(Data.GroupKey, LT_HSPT_CMD, Key.keymap));
			if(pNode)VERIFY(pNode->DelListItem(Key.key.entity, LT_HSPT_CMD, Key.keymap));

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveBndr(LT_HSPT_CMD, Key.keymap);
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_HSPT::Get(T_HSPT_K Key,T_HSPT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_hspt->Lookup(Key.keymap, StagBngrKey)) return FALSE;
	}
	return m_hspt.Lookup(Key.keymap,rData);
}

int CDB_HSPT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_hspt->GetCount();
	}
	return m_hspt.GetCount();
}

POSITION CDB_HSPT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_hspt->GetStartPosition();
	}
	return m_hspt.GetStartPosition();
}

void CDB_HSPT::GetNext(POSITION& rNextPosition,T_HSPT_K& rKey,T_HSPT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_hspt->GetNextAssoc(rNextPosition, rKey.keymap, StagBngrKey);
		m_hspt.Lookup(rKey.keymap, rData);
		return;
	}
	m_hspt.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

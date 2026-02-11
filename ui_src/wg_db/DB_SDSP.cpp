#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_NODE.h"
#include "DB_SDSP.h"
#include "DB_LDGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SDSP::CDB_SDSP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sdsp.InitHashTable(HASHSIZESDSP);
}

CDB_SDSP::~CDB_SDSP()
{

}

void CDB_SDSP::Add(T_SDSP_K Key,T_SDSP_D& rData,CDB_STLD* pStld,CDB_NODE* pNode, CDB_LDGR* pLdgr)
{
	T_SDSP_D Data;
	BOOL bExist=m_sdsp.Lookup(Key.keymap, Data);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey && pStld)
		VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_SDSP_CMD, Key.keymap));
	if(bExist && Data.GroupKey != rData.GroupKey && pLdgr)
		VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_SDSP_CMD, Key.keymap));
	m_sdsp.SetAt(Key.keymap,rData);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey && pStld)
		pStld->AddListItem(rData.LoadCaseKey, LT_SDSP_CMD, Key.keymap);
	if(bExist && Data.GroupKey != rData.GroupKey && pLdgr)
		pLdgr->AddListItem(rData.GroupKey, LT_SDSP_CMD, Key.keymap);
	if(!bExist)
	{
		if (pStld) pStld->AddListItem(rData.LoadCaseKey, LT_SDSP_CMD, Key.keymap);
		if (pNode) pNode->AddListItem(Key.key.entity, LT_SDSP_CMD, Key.keymap);
		if (pLdgr) pLdgr->AddListItem(rData.GroupKey, LT_SDSP_CMD, Key.keymap);
	}
	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Load는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_SDSP_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_SDSP::Del(T_SDSP_K Key,CDB_STLD* pStld,CDB_NODE* pNode, CDB_LDGR* pLdgr)
{
	T_SDSP_D Data;
	BOOL bExist=m_sdsp.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_sdsp.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)
		{
			if (pStld) VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_SDSP_CMD, Key.keymap));
			if (pNode) VERIFY(pNode->DelListItem(Key.key.entity, LT_SDSP_CMD, Key.keymap));
			if (pLdgr) VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_SDSP_CMD, Key.keymap));
			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_SDSP_CMD, Key.keymap);
			}
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_SDSP::Get(T_SDSP_K Key,T_SDSP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_sdsp->Lookup(Key.keymap, StagLdgrKey)) return FALSE;
	}
	return m_sdsp.Lookup(Key.keymap,rData);
}

int CDB_SDSP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_sdsp->GetCount();
	}
	return m_sdsp.GetCount();
}

POSITION CDB_SDSP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_sdsp->GetStartPosition();
	}
	return m_sdsp.GetStartPosition();
}

void CDB_SDSP::GetNext(POSITION& rNextPosition,T_SDSP_K& rKey,T_SDSP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_sdsp->GetNextAssoc(rNextPosition, rKey.keymap, StagLdgrKey);
		m_sdsp.Lookup(rKey.keymap, rData);
		return;
	}
	m_sdsp.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

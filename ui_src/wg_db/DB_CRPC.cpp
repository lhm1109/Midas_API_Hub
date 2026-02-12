#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CRPC.h"
#include "DB_LDGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CRPC::CDB_CRPC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_crpc.InitHashTable(HASHSIZECRPC);
}

CDB_CRPC::~CDB_CRPC()
{

}

void CDB_CRPC::Add(T_CRPC_K Key,T_CRPC_D& rData,CDB_ELEM* pElem,CDB_LDGR* pLdgr)
{
	T_CRPC_D Data;
	BOOL bExist=m_crpc.Lookup(Key.keymap, Data);
	if(bExist) pLdgr->DelListItem(Data.GroupKey, LT_CRPC_CMD, Key.keymap);
	m_crpc.SetAt(Key.keymap,rData);
	pLdgr->AddListItem(rData.GroupKey, LT_CRPC_CMD, Key.keymap);
	if(!bExist && pElem)pElem->AddListItem(Key.key.entity, LT_CRPC_CMD, Key.keymap);

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_CRPC_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_CRPC::Del(T_CRPC_K Key,CDB_ELEM* pElem,CDB_LDGR* pLdgr)
{
	T_CRPC_D Data;
	BOOL bExist = m_crpc.Lookup(Key.keymap, Data);
	if (bExist)
	{
		BOOL ret=m_crpc.RemoveKey(Key.keymap);
		ASSERT(ret);
		if (ret)
		{
			if(pLdgr)VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_CRPC_CMD, Key.keymap));
			if(pElem)VERIFY(pElem->DelListItem(Key.key.entity, LT_CRPC_CMD, Key.keymap));

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_CRPC_CMD, Key.keymap);
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_CRPC::Get(T_CRPC_K Key,T_CRPC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_crpc->Lookup(Key.keymap, StagLdgrKey)) return FALSE;
	}
	return m_crpc.Lookup(Key.keymap,rData);
}

int CDB_CRPC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_crpc->GetCount();
	}
	return m_crpc.GetCount();
}

POSITION CDB_CRPC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_crpc->GetStartPosition();
	}
	return m_crpc.GetStartPosition();
}

void CDB_CRPC::GetNext(POSITION& rNextPosition,T_CRPC_K& rKey,T_CRPC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_crpc->GetNextAssoc(rNextPosition, rKey.keymap, StagLdgrKey);
		m_crpc.Lookup(rKey.keymap, rData);
		return;
	}
	m_crpc.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

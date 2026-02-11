#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_NODE.h"
#include "DB_NTMP.h"
#include "DB_LDGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_NTMP::CDB_NTMP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ntmp.InitHashTable(HASHSIZENTMP);
}

CDB_NTMP::~CDB_NTMP()
{

}

void CDB_NTMP::Add(T_NTMP_K Key,T_NTMP_D& rData,CDB_STLD* pStld,CDB_NODE* pNode,CDB_LDGR* pLdgr)
{
	T_NTMP_D Data;
	BOOL bExist=m_ntmp.Lookup(Key.keymap, Data);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_NTMP_CMD, Key.keymap));
	if(bExist && Data.GroupKey != rData.GroupKey)
		VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_NTMP_CMD, Key.keymap));
	m_ntmp.SetAt(Key.keymap,rData);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		pStld->AddListItem(rData.LoadCaseKey, LT_NTMP_CMD, Key.keymap);
	if(bExist && Data.GroupKey != rData.GroupKey)
		pLdgr->AddListItem(rData.GroupKey, LT_NTMP_CMD, Key.keymap);
	if(!bExist)
	{
		pStld->AddListItem(rData.LoadCaseKey, LT_NTMP_CMD, Key.keymap);
		pNode->AddListItem(Key.key.entity, LT_NTMP_CMD, Key.keymap);
		pLdgr->AddListItem(rData.GroupKey, LT_NTMP_CMD, Key.keymap);
	}
	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Load는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_NTMP_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_NTMP::Del(T_NTMP_K Key,CDB_STLD* pStld,CDB_NODE* pNode,CDB_LDGR* pLdgr)
{
	T_NTMP_D Data;
	BOOL bExist=m_ntmp.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_ntmp.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_NTMP_CMD, Key.keymap));
			VERIFY(pNode->DelListItem(Key.key.entity, LT_NTMP_CMD, Key.keymap));
			VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_NTMP_CMD, Key.keymap));
			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_NTMP_CMD, Key.keymap);
			}
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_NTMP::Get(T_NTMP_K Key,T_NTMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_ntmp->Lookup(Key.keymap, StagLdgrKey)) return FALSE;
	}
	return m_ntmp.Lookup(Key.keymap,rData);
}

int CDB_NTMP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_ntmp->GetCount();
	}
	return m_ntmp.GetCount();
}

POSITION CDB_NTMP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_ntmp->GetStartPosition();
	}
	return m_ntmp.GetStartPosition();
}

void CDB_NTMP::GetNext(POSITION& rNextPosition,T_NTMP_K& rKey,T_NTMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_ntmp->GetNextAssoc(rNextPosition, rKey.keymap, StagLdgrKey);
		m_ntmp.Lookup(rKey.keymap, rData);
		return;
	}
	m_ntmp.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

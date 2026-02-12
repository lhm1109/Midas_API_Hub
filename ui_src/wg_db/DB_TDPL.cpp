#include "stdafx.h"
#include "DB_TDPL.h"

#include "DB_STLD.h"
#include "DB_LDGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_TDPL::CDB_TDPL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_tdpl.InitHashTable(HASHSIZETDPL);
}

CDB_TDPL::~CDB_TDPL()
{

}

void CDB_TDPL::Add(T_TDPL_K Key,T_TDPL_D& rData,CDB_STLD* pStld,CDB_LDGR* pLdgr)
{
	T_TDPL_D Data;
	BOOL bExist=m_tdpl.Lookup(Key.keymap, Data);
	if(bExist)
	{
		pStld->DelListItem(Data.LoadCaseKey, LT_TDPL_CMD, Key.keymap);
		pLdgr->DelListItem(Data.GroupKey, LT_TDPL_CMD, Key.keymap);
	}
	m_tdpl.SetAt(Key.keymap,rData);
	pStld->AddListItem(rData.LoadCaseKey, LT_TDPL_CMD, Key.keymap);
	pLdgr->AddListItem(rData.GroupKey, LT_TDPL_CMD, Key.keymap);

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Load는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_TDPL_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_TDPL::Del(T_TDPL_K Key,CDB_STLD* pStld,CDB_LDGR* pLdgr)
{
	T_TDPL_D Data;
	BOOL bExist=m_tdpl.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_tdpl.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_TDPL_CMD, Key.keymap));
			VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_TDPL_CMD, Key.keymap));

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_TDPL_CMD, Key.keymap);
			}
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_TDPL::Get(T_TDPL_K Key,T_TDPL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_tdpl->Lookup(Key.keymap, StagLdgrKey)) return FALSE;
	}
	return m_tdpl.Lookup(Key.keymap,rData);
}

int CDB_TDPL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_tdpl->GetCount();
	}
	return m_tdpl.GetCount();
}

POSITION CDB_TDPL::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_tdpl->GetStartPosition();
	}
	return m_tdpl.GetStartPosition();
}

void CDB_TDPL::GetNext(POSITION& rNextPosition,T_TDPL_K& rKey,T_TDPL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_tdpl->GetNextAssoc(rNextPosition, rKey.keymap, StagLdgrKey);
		m_tdpl.Lookup(rKey.keymap, rData);
		return;
	}
	m_tdpl.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}


#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_ELEM.h"
#include "DB_GTMP.h"
#include "DB_LDGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_GTMP::CDB_GTMP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_gtmp.InitHashTable(HASHSIZEGTMP);
}

CDB_GTMP::~CDB_GTMP()
{

}

void CDB_GTMP::Add(T_GTMP_K Key,T_GTMP_D& rData,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr)
{
	T_GTMP_D Data;
	BOOL bExist=m_gtmp.Lookup(Key.keymap, Data);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_GTMP_CMD, Key.keymap));
	if(bExist && Data.GroupKey != rData.GroupKey)
		VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_GTMP_CMD, Key.keymap));
	m_gtmp.SetAt(Key.keymap,rData);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		pStld->AddListItem(rData.LoadCaseKey, LT_GTMP_CMD, Key.keymap);
	if(bExist && Data.GroupKey != rData.GroupKey)
		pLdgr->AddListItem(rData.GroupKey, LT_GTMP_CMD, Key.keymap);
	if(!bExist)
	{
		pStld->AddListItem(rData.LoadCaseKey, LT_GTMP_CMD, Key.keymap);
		pElem->AddListItem(Key.key.entity, LT_GTMP_CMD, Key.keymap);
		pLdgr->AddListItem(rData.GroupKey, LT_GTMP_CMD, Key.keymap);
	}

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Load는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_GTMP_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_GTMP::Del(T_GTMP_K Key,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr)
{
	T_GTMP_D Data;
	BOOL bExist=m_gtmp.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_gtmp.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_GTMP_CMD, Key.keymap));
			VERIFY(pElem->DelListItem(Key.key.entity, LT_GTMP_CMD, Key.keymap));
			VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_GTMP_CMD, Key.keymap));

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_GTMP_CMD, Key.keymap);
			}
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_GTMP::Get(T_GTMP_K Key,T_GTMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_gtmp->Lookup(Key.keymap, StagLdgrKey)) return FALSE;
	}
	return m_gtmp.Lookup(Key.keymap,rData);
}

int CDB_GTMP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_gtmp->GetCount();
	}
	return m_gtmp.GetCount();
}

POSITION CDB_GTMP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_gtmp->GetStartPosition();
	}
	return m_gtmp.GetStartPosition();
}

void CDB_GTMP::GetNext(POSITION& rNextPosition,T_GTMP_K& rKey,T_GTMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_gtmp->GetNextAssoc(rNextPosition, rKey.keymap, StagLdgrKey);
		m_gtmp.Lookup(rKey.keymap, rData);
		return;
	}
	m_gtmp.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

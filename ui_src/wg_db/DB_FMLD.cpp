#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_ELEM.h"
#include "DB_FMLD.h"
#include "DB_LDGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_FMLD::CDB_FMLD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_fmld.InitHashTable(HASHSIZEFMLD);
}

CDB_FMLD::~CDB_FMLD()
{

}

void CDB_FMLD::Add(T_FMLD_K Key,T_FMLD_D& rData,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr)
{
	T_FMLD_D Data;
	BOOL bExist=m_fmld.Lookup(Key.keymap, Data);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_FMLD_CMD, Key.keymap));
	if(bExist && Data.GroupKey != rData.GroupKey)
		VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_FMLD_CMD, Key.keymap));
	m_fmld.SetAt(Key.keymap,rData);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		pStld->AddListItem(rData.LoadCaseKey, LT_FMLD_CMD, Key.keymap);
	if(bExist && Data.GroupKey != rData.GroupKey)
		pLdgr->AddListItem(rData.GroupKey, LT_FMLD_CMD, Key.keymap);
	if(!bExist)
	{
		pStld->AddListItem(rData.LoadCaseKey, LT_FMLD_CMD, Key.keymap);
		pElem->AddListItem(Key.key.entity, LT_FMLD_CMD, Key.keymap);
		pLdgr->AddListItem(rData.GroupKey, LT_FMLD_CMD, Key.keymap);
	}

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Load는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_FMLD_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_FMLD::Del(T_FMLD_K Key,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr)
{
	T_FMLD_D Data;
	BOOL bExist=m_fmld.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_fmld.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_FMLD_CMD, Key.keymap));
			VERIFY(pElem->DelListItem(Key.key.entity, LT_FMLD_CMD, Key.keymap));
			VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_FMLD_CMD, Key.keymap));

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_FMLD_CMD, Key.keymap);
			}
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_FMLD::Get(T_FMLD_K Key,T_FMLD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_fmld->Lookup(Key.keymap, StagLdgrKey)) return FALSE;
	}
	return m_fmld.Lookup(Key.keymap,rData);
}

int CDB_FMLD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_fmld->GetCount();
	}
	return m_fmld.GetCount();
}

POSITION CDB_FMLD::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_fmld->GetStartPosition();
	}
	return m_fmld.GetStartPosition();
}

void CDB_FMLD::GetNext(POSITION& rNextPosition,T_FMLD_K& rKey,T_FMLD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_fmld->GetNextAssoc(rNextPosition, rKey.keymap, StagLdgrKey);
		m_fmld.Lookup(rKey.keymap, rData);
		return;
	}
	m_fmld.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

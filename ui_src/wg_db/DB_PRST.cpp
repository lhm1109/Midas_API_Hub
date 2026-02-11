#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_ELEM.h"
#include "DB_PRST.h"
#include "DB_LDGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PRST::CDB_PRST()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_prst.InitHashTable(HASHSIZEPRST);
}

CDB_PRST::~CDB_PRST()
{

}

void CDB_PRST::Add(T_PRST_K Key,T_PRST_D& rData,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr)
{
	T_PRST_D Data;
	BOOL bExist=m_prst.Lookup(Key.keymap, Data);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_PRST_CMD, Key.keymap));
	if(bExist && Data.GroupKey != rData.GroupKey)
		VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_PRST_CMD, Key.keymap));
	m_prst.SetAt(Key.keymap,rData);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		pStld->AddListItem(rData.LoadCaseKey, LT_PRST_CMD, Key.keymap);
	if(bExist && Data.GroupKey != rData.GroupKey)
		pLdgr->AddListItem(rData.GroupKey, LT_PRST_CMD, Key.keymap);
	if(!bExist)
	{
		pStld->AddListItem(rData.LoadCaseKey, LT_PRST_CMD, Key.keymap);
		pElem->AddListItem(Key.key.entity, LT_PRST_CMD, Key.keymap);
		pLdgr->AddListItem(rData.GroupKey, LT_PRST_CMD, Key.keymap);
	}
	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Load는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_PRST_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_PRST::Del(T_PRST_K Key,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr)
{
	T_PRST_D Data;
	BOOL bExist=m_prst.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_prst.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_PRST_CMD, Key.keymap));
			VERIFY(pElem->DelListItem(Key.key.entity, LT_PRST_CMD, Key.keymap));
			VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_PRST_CMD, Key.keymap));
			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_PRST_CMD, Key.keymap);
			}
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_PRST::Get(T_PRST_K Key,T_PRST_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_prst->Lookup(Key.keymap, StagLdgrKey)) return FALSE;
	}
	return m_prst.Lookup(Key.keymap,rData);
}

int CDB_PRST::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_prst->GetCount();
	}
	return m_prst.GetCount();
}

POSITION CDB_PRST::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_prst->GetStartPosition();
	}
	return m_prst.GetStartPosition();
}

void CDB_PRST::GetNext(POSITION& rNextPosition,T_PRST_K& rKey,T_PRST_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_prst->GetNextAssoc(rNextPosition, rKey.keymap, StagLdgrKey);
		m_prst.Lookup(rKey.keymap, rData);
		return;
	}
	m_prst.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

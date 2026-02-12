#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_ELEM.h"
#include "DB_BTMP.h"
#include "DB_LDGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_BTMP::CDB_BTMP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_btmp.InitHashTable(HASHSIZEBTMP);
}

CDB_BTMP::~CDB_BTMP()
{

}

void CDB_BTMP::Add(T_BTMP_K Key,T_BTMP_D& rData,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr)
{
	T_BTMP_D Data;
	BOOL bExist=m_btmp.Lookup(Key.keymap, Data);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_BTMP_CMD, Key.keymap));
	if(bExist && Data.GroupKey != rData.GroupKey)
		VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_BTMP_CMD, Key.keymap));
	m_btmp.SetAt(Key.keymap,rData);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		pStld->AddListItem(rData.LoadCaseKey, LT_BTMP_CMD, Key.keymap);
	if(bExist && Data.GroupKey != rData.GroupKey)
		pLdgr->AddListItem(rData.GroupKey, LT_BTMP_CMD, Key.keymap);
	if(!bExist)
	{
		pStld->AddListItem(rData.LoadCaseKey, LT_BTMP_CMD, Key.keymap);
		pElem->AddListItem(Key.key.entity, LT_BTMP_CMD, Key.keymap);
		pLdgr->AddListItem(rData.GroupKey, LT_BTMP_CMD, Key.keymap);
	}
	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Load는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_BTMP_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_BTMP::Del(T_BTMP_K Key,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr)
{
	T_BTMP_D Data;
	BOOL bExist=m_btmp.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_btmp.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_BTMP_CMD, Key.keymap));
			VERIFY(pElem->DelListItem(Key.key.entity, LT_BTMP_CMD, Key.keymap));
			VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_BTMP_CMD, Key.keymap));
			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_BTMP_CMD, Key.keymap);
			}
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_BTMP::Get(T_BTMP_K Key,T_BTMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_btmp->Lookup(Key.keymap, StagLdgrKey)) return FALSE;
	}
	return m_btmp.Lookup(Key.keymap,rData);
}

int CDB_BTMP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_btmp->GetCount();
	}
	return m_btmp.GetCount();
}

POSITION CDB_BTMP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_btmp->GetStartPosition();
	}
	return m_btmp.GetStartPosition();
}

void CDB_BTMP::GetNext(POSITION& rNextPosition,T_BTMP_K& rKey,T_BTMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_btmp->GetNextAssoc(rNextPosition, rKey.keymap, StagLdgrKey);
		m_btmp.Lookup(rKey.keymap, rData);
		return;
	}
	m_btmp.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

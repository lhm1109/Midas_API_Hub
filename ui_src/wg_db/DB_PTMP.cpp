#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_ELEM.h"
#include "DB_PTMP.h"
#include "DB_LDGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PTMP::CDB_PTMP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ptmp.InitHashTable(HASHSIZEPTMP);
}

CDB_PTMP::~CDB_PTMP()
{

}

void CDB_PTMP::Add(T_PTMP_K Key,T_PTMP_D& rData,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr)
{
	T_PTMP_D Data;
	BOOL bExist=m_ptmp.Lookup(Key.keymap, Data);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_PTMP_CMD, Key.keymap));
	if(bExist && Data.GroupKey != rData.GroupKey)
		VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_PTMP_CMD, Key.keymap));
	m_ptmp.SetAt(Key.keymap,rData);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		pStld->AddListItem(rData.LoadCaseKey, LT_PTMP_CMD, Key.keymap);
	if(bExist && Data.GroupKey != rData.GroupKey)
		pLdgr->AddListItem(rData.GroupKey, LT_PTMP_CMD, Key.keymap);
	if(!bExist)
	{
		pStld->AddListItem(rData.LoadCaseKey, LT_PTMP_CMD, Key.keymap);
		pElem->AddListItem(Key.key.entity, LT_PTMP_CMD, Key.keymap);
		pLdgr->AddListItem(rData.GroupKey, LT_PTMP_CMD, Key.keymap);
	}
	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Load는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_PTMP_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_PTMP::Del(T_PTMP_K Key,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr)
{
	T_PTMP_D Data;
	BOOL bExist=m_ptmp.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_ptmp.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_PTMP_CMD, Key.keymap));
			VERIFY(pElem->DelListItem(Key.key.entity, LT_PTMP_CMD, Key.keymap));
			VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_PTMP_CMD, Key.keymap));
			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_PTMP_CMD, Key.keymap);
			}
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_PTMP::Get(T_PTMP_K Key,T_PTMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_ptmp->Lookup(Key.keymap, StagLdgrKey)) return FALSE;
	}
	return m_ptmp.Lookup(Key.keymap,rData);
}

int CDB_PTMP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_ptmp->GetCount();
	}
	return m_ptmp.GetCount();
}

POSITION CDB_PTMP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_ptmp->GetStartPosition();
	}
	return m_ptmp.GetStartPosition();
}

void CDB_PTMP::GetNext(POSITION& rNextPosition,T_PTMP_K& rKey,T_PTMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_ptmp->GetNextAssoc(rNextPosition, rKey.keymap, StagLdgrKey);
		m_ptmp.Lookup(rKey.keymap, rData);
		return;
	}
	m_ptmp.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

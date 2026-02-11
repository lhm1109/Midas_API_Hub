#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_ELEM.h"
#include "DB_PRES.h"
#include "DB_LDGR.h"
#include "DB_PSLT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PRES::CDB_PRES()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pres.InitHashTable(HASHSIZEPRES);
}

CDB_PRES::~CDB_PRES()
{

}

void CDB_PRES::Add(T_PRES_K Key,T_PRES_D& rData,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr,CDB_PSLT* pPslt/* =NULL */)
{
	T_PRES_D Data;
	BOOL bExist=m_pres.Lookup(Key.keymap, Data);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_PRES_CMD, Key.keymap));
	if(bExist && Data.GroupKey != rData.GroupKey)
		VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_PRES_CMD, Key.keymap));
	if(pPslt && bExist && Data.PSltKey != rData.PSltKey)
	{
		VERIFY(pPslt->DelListItem(Data.PSltKey, LT_PRES_CMD, Key.keymap));
		ASSERT(rData.PSltKey == 0);
		rData.PSltKey = 0;
	}
	m_pres.SetAt(Key.keymap,rData);
	if(bExist && Data.LoadCaseKey != rData.LoadCaseKey)
		pStld->AddListItem(rData.LoadCaseKey, LT_PRES_CMD, Key.keymap);
	if(bExist && Data.GroupKey != rData.GroupKey)
		pLdgr->AddListItem(rData.GroupKey, LT_PRES_CMD, Key.keymap);
	if(!bExist)
	{
		pStld->AddListItem(rData.LoadCaseKey, LT_PRES_CMD, Key.keymap);
		pElem->AddListItem(Key.key.entity, LT_PRES_CMD, Key.keymap);
		pLdgr->AddListItem(rData.GroupKey, LT_PRES_CMD, Key.keymap);
		if(pPslt && rData.PSltKey != 0)
			pPslt->AddListItem(rData.PSltKey, LT_PRES_CMD, Key.keymap);
	}
	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Load는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveLoad(LT_PRES_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_PRES::Del(T_PRES_K Key,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr,CDB_PSLT* pPslt/* =NULL */)
{
	T_PRES_D Data;
	BOOL bExist=m_pres.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_pres.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Data.LoadCaseKey, LT_PRES_CMD, Key.keymap));
			VERIFY(pElem->DelListItem(Key.key.entity, LT_PRES_CMD, Key.keymap));
			VERIFY(pLdgr->DelListItem(Data.GroupKey, LT_PRES_CMD, Key.keymap));
			if(pPslt && Data.PSltKey != 0)
				VERIFY(pPslt->DelListItem(Data.PSltKey, LT_PRES_CMD, Key.keymap));
			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveLoad(LT_PRES_CMD, Key.keymap);
			}
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_PRES::Get(T_PRES_K Key,T_PRES_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		if (!m_pDoc->m_pStagCtrl->m_pres->Lookup(Key.keymap, StagLdgrKey)) return FALSE;
	}
	return m_pres.Lookup(Key.keymap,rData);
}

int CDB_PRES::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_pres->GetCount();
	}
	return m_pres.GetCount();
}

POSITION CDB_PRES::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_pres->GetStartPosition();
	}
	return m_pres.GetStartPosition();
}

void CDB_PRES::GetNext(POSITION& rNextPosition,T_PRES_K& rKey,T_PRES_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagLdgrKey;
		m_pDoc->m_pStagCtrl->m_pres->GetNextAssoc(rNextPosition, rKey.keymap, StagLdgrKey);
		m_pres.Lookup(rKey.keymap, rData);
		return;
	}
	m_pres.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

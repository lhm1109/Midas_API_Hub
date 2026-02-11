#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_HECB.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_HECB::CDB_HECB()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_hecb.InitHashTable(HASHSIZEHECB);
}

CDB_HECB::~CDB_HECB()
{

}

void CDB_HECB::Add(T_HECB_K Key,T_HECB_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_HECB_D Data;
	BOOL bExist=m_hecb.Lookup(Key.keymap, Data);
	if(bExist) pBngr->DelListItem(Data.GroupKey, LT_HECB_CMD, Key.keymap);
	m_hecb.SetAt(Key.keymap,rData);
	pBngr->AddListItem(rData.GroupKey, LT_HECB_CMD, Key.keymap);
	if(!bExist && pElem)pElem->AddListItem(Key.key.entity, LT_HECB_CMD, Key.keymap);

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_HECB_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_HECB::Del(T_HECB_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_HECB_D Data;
	BOOL bExist = m_hecb.Lookup(Key.keymap, Data);
	if (bExist)
	{
		BOOL ret=m_hecb.RemoveKey(Key.keymap);
		ASSERT(ret);
		if (ret)
		{
			if(pBngr)VERIFY(pBngr->DelListItem(Data.GroupKey, LT_HECB_CMD, Key.keymap));
			if(pElem)VERIFY(pElem->DelListItem(Key.key.entity, LT_HECB_CMD, Key.keymap));

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveBndr(LT_HECB_CMD, Key.keymap);
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_HECB::Get(T_HECB_K Key,T_HECB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_hecb->Lookup(Key.keymap, StagBngrKey)) return FALSE;
	}
	return m_hecb.Lookup(Key.keymap,rData);
}

int CDB_HECB::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_hecb->GetCount();
	}
	return m_hecb.GetCount();
}

POSITION CDB_HECB::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_hecb->GetStartPosition();
	}
	return m_hecb.GetStartPosition();
}

void CDB_HECB::GetNext(POSITION& rNextPosition,T_HECB_K& rKey,T_HECB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_hecb->GetNextAssoc(rNextPosition, rKey.keymap, StagBngrKey);
		m_hecb.Lookup(rKey.keymap, rData);
		return;
	}
	m_hecb.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

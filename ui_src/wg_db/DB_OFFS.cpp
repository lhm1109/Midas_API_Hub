#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_OFFS.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_OFFS::CDB_OFFS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_offs.InitHashTable(HASHSIZEOFFS);
}

CDB_OFFS::~CDB_OFFS()
{

}

void CDB_OFFS::Add(T_OFFS_K Key,T_OFFS_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_OFFS_D Data;
	BOOL bExist=m_offs.Lookup(Key.keymap, Data);
	if (bExist && Data.GroupKey != rData.GroupKey)
		pBngr->DelListItem(Data.GroupKey, LT_OFFS_CMD, Key.keymap);
	m_offs.SetAt(Key.keymap,rData);
	if (bExist && Data.GroupKey != rData.GroupKey)
		pBngr->AddListItem(rData.GroupKey, LT_OFFS_CMD, Key.keymap);
	if(!bExist)
	{
		pElem->AddListItem(Key.key.entity, LT_OFFS_CMD, Key.keymap);
		pBngr->AddListItem(rData.GroupKey, LT_OFFS_CMD, Key.keymap);
	}

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_OFFS_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_OFFS::Del(T_OFFS_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_OFFS_D Data;
	m_offs.Lookup(Key.keymap, Data);
	BOOL ret=m_offs.RemoveKey(Key.keymap);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key.key.entity, LT_OFFS_CMD, Key.keymap));
		VERIFY(pBngr->DelListItem(Data.GroupKey, LT_OFFS_CMD, Key.keymap));

		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveBndr(LT_OFFS_CMD, Key.keymap);
		}
	}
	return ret;
}

//--------------------------------------------------------------------------
BOOL CDB_OFFS::Get(T_OFFS_K Key,T_OFFS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_offs->Lookup(Key.keymap, StagBngrKey)) return FALSE;
	}
	return m_offs.Lookup(Key.keymap,rData);
}

int CDB_OFFS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_offs->GetCount();
	}
	return m_offs.GetCount();
}

POSITION CDB_OFFS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_offs->GetStartPosition();
	}
	return m_offs.GetStartPosition();
}

void CDB_OFFS::GetNext(POSITION& rNextPosition,T_OFFS_K& rKey,T_OFFS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_offs->GetNextAssoc(rNextPosition, rKey.keymap, StagBngrKey);
		m_offs.Lookup(rKey.keymap, rData);
		return;
	}
	m_offs.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

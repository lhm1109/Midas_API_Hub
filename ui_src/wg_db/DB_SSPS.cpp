#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SSPS.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"


CDB_SSPS::CDB_SSPS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ssps.InitHashTable(HASHSIZEPRES);
}

CDB_SSPS::~CDB_SSPS()
{

}

void CDB_SSPS::Add(T_SSPS_K Key,T_SSPS_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_SSPS_D Data;
	BOOL bExist=m_ssps.Lookup(Key.keymap, Data);
	if(bExist && Data.GroupKey != rData.GroupKey && pBngr)
		VERIFY(pBngr->DelListItem(Data.GroupKey, LT_SSPS_CMD, Key.keymap));

	m_ssps.SetAt(Key.keymap,rData);
	if(bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->AddListItem(rData.GroupKey, LT_SSPS_CMD, Key.keymap);

	if(!bExist)
	{
		if(pElem)pElem->AddListItem(Key.key.entity, LT_SSPS_CMD, Key.keymap);
		if(pBngr)pBngr->AddListItem(rData.GroupKey, LT_SSPS_CMD, Key.keymap);
	}
	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_SSPS_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_SSPS::Del(T_SSPS_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_SSPS_D Data;
	BOOL bExist=m_ssps.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_ssps.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)
		{
			if (pElem)VERIFY(pElem->DelListItem(Key.key.entity, LT_SSPS_CMD, Key.keymap));
			if (pBngr)VERIFY(pBngr->DelListItem(Data.GroupKey, LT_SSPS_CMD, Key.keymap));

			// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
			if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
			{
				m_pDoc->m_pStagCtrl->DeactiveBndr(LT_SSPS_CMD, Key.keymap);
			}
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_SSPS::Get(T_SSPS_K Key,T_SSPS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_ssps->Lookup(Key.keymap, StagBngrKey)) return FALSE;
	}
	return m_ssps.Lookup(Key.keymap,rData);
}

int CDB_SSPS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_ssps->GetCount();
	}
	return m_ssps.GetCount();
}

POSITION CDB_SSPS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_ssps->GetStartPosition();
	}
	return m_ssps.GetStartPosition();
}

void CDB_SSPS::GetNext(POSITION& rNextPosition,T_SSPS_K& rKey,T_SSPS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_ssps->GetNextAssoc(rNextPosition, rKey.keymap, StagBngrKey);
		m_ssps.Lookup(rKey.keymap, rData);
		return;
	}
	m_ssps.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}


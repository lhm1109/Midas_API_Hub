#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_EWSC.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_EWSC::CDB_EWSC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ewsc.InitHashTable(HASHSIZEEWSC);
}

CDB_EWSC::~CDB_EWSC()
{

}

void CDB_EWSC::Add(T_EWSC_K Key,T_EWSC_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_EWSC_D Data;
	BOOL bExist=m_ewsc.Lookup(Key.keymap, Data);
	if (bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->DelListItem(Data.GroupKey, LT_EWSC_CMD, Key.keymap);
	m_ewsc.SetAt(Key.keymap,rData);
	if (bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->AddListItem(rData.GroupKey, LT_EWSC_CMD, Key.keymap);
	if(!bExist)
	{
		if (pElem) pElem->AddListItem(Key.key.entity, LT_EWSC_CMD, Key.keymap);
		if (pBngr) pBngr->AddListItem(rData.GroupKey, LT_EWSC_CMD, Key.keymap);
	}

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_EWSC_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_EWSC::Del(T_EWSC_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_EWSC_D Data;
	m_ewsc.Lookup(Key.keymap, Data);
	BOOL ret=m_ewsc.RemoveKey(Key.keymap);
	ASSERT(ret);
	if(ret)
	{
		if (pElem) VERIFY(pElem->DelListItem(Key.key.entity, LT_EWSC_CMD, Key.keymap));
		if (pBngr) VERIFY(pBngr->DelListItem(Data.GroupKey, LT_EWSC_CMD, Key.keymap));

		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveBndr(LT_EWSC_CMD, Key.keymap);
		}
	}
	return ret;
}

void CDB_EWSC::AddDesign(T_EWSC_K Key,T_EWSC_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_EWSC_D Data;
	BOOL bExist=m_ewscDesign.Lookup(Key.keymap, Data);
	if (bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->DelListItem(Data.GroupKey, LT_EWCD_CMD, Key.keymap);
	m_ewscDesign.SetAt(Key.keymap,rData);
	if (bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->AddListItem(rData.GroupKey, LT_EWCD_CMD, Key.keymap);
	if(!bExist)
	{
		if (pElem) pElem->AddListItem(Key.key.entity, LT_EWCD_CMD, Key.keymap);
		if (pBngr) pBngr->AddListItem(rData.GroupKey, LT_EWCD_CMD, Key.keymap);
	}

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_EWCD_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_EWSC::DelDesign(T_EWSC_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_EWSC_D Data;
	m_ewscDesign.Lookup(Key.keymap, Data);
	BOOL ret=m_ewscDesign.RemoveKey(Key.keymap);
	ASSERT(ret);
	if(ret)
	{
		if (pElem) VERIFY(pElem->DelListItem(Key.key.entity, LT_EWCD_CMD, Key.keymap));
		if (pBngr) VERIFY(pBngr->DelListItem(Data.GroupKey, LT_EWCD_CMD, Key.keymap));

		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveBndr(LT_EWCD_CMD, Key.keymap);
		}
	}
	return ret;
}

//--EWSC------------------------------------------------------------------------
BOOL CDB_EWSC::Get(T_EWSC_K Key,T_EWSC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_ewsc->Lookup(Key.keymap, StagBngrKey)) return FALSE;
	}
	return m_ewsc.Lookup(Key.keymap,rData);
}

int CDB_EWSC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_ewsc->GetCount();
	}
	return m_ewsc.GetCount();
}

POSITION CDB_EWSC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_ewsc->GetStartPosition();
	}
	return m_ewsc.GetStartPosition();
}

void CDB_EWSC::GetNext(POSITION& rNextPosition,T_EWSC_K& rKey,T_EWSC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_ewsc->GetNextAssoc(rNextPosition, rKey.keymap, StagBngrKey);
		m_ewsc.Lookup(rKey.keymap, rData);
		return;
	}
	m_ewsc.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

//--EWSD------------------------------------------------------------------------
BOOL CDB_EWSC::GetDesign(T_EWSC_K Key,T_EWSC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_ewscDesign->Lookup(Key.keymap, StagBngrKey)) return FALSE;
	}
	return m_ewscDesign.Lookup(Key.keymap,rData);
}

int CDB_EWSC::GetCountDesign()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_ewscDesign->GetCount();
	}
	return m_ewscDesign.GetCount();
}

POSITION CDB_EWSC::GetStartDesign()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_ewscDesign->GetStartPosition();
	}
	return m_ewscDesign.GetStartPosition();
}

void CDB_EWSC::GetNextDesign(POSITION& rNextPosition,T_EWSC_K& rKey,T_EWSC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_ewscDesign->GetNextAssoc(rNextPosition, rKey.keymap, StagBngrKey);
		m_ewscDesign.Lookup(rKey.keymap, rData);
		return;
	}
	m_ewscDesign.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}
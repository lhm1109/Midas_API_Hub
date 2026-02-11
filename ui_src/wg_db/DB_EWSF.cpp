#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_EWSF.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_EWSF::CDB_EWSF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ewsf.InitHashTable(HASHSIZEEWSF);
}

CDB_EWSF::~CDB_EWSF()
{

}

void CDB_EWSF::Add(T_EWSF_K Key,T_EWSF_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_EWSF_D Data;
	BOOL bExist=m_ewsf.Lookup(Key.keymap, Data);
	if (bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->DelListItem(Data.GroupKey, LT_EWSF_CMD, Key.keymap);
	m_ewsf.SetAt(Key.keymap,rData);
	if (bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->AddListItem(rData.GroupKey, LT_EWSF_CMD, Key.keymap);
	if(!bExist)
	{
		if (pElem) pElem->AddListItem(Key.key.entity, LT_EWSF_CMD, Key.keymap);
		if (pBngr) pBngr->AddListItem(rData.GroupKey, LT_EWSF_CMD, Key.keymap);
	}

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_EWSF_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_EWSF::Del(T_EWSF_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_EWSF_D Data;
	m_ewsf.Lookup(Key.keymap, Data);
	BOOL ret=m_ewsf.RemoveKey(Key.keymap);
	ASSERT(ret);
	if(ret)
	{
		if (pElem) VERIFY(pElem->DelListItem(Key.key.entity, LT_EWSF_CMD, Key.keymap));
		if (pBngr) VERIFY(pBngr->DelListItem(Data.GroupKey, LT_EWSF_CMD, Key.keymap));

		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveBndr(LT_EWSF_CMD, Key.keymap);
		}
	}
	return ret;
}

void CDB_EWSF::AddDesign(T_EWSF_K Key,T_EWSF_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_EWSF_D Data;
	BOOL bExist=m_ewsfDesign.Lookup(Key.keymap, Data);
	if (bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->DelListItem(Data.GroupKey, LT_EWSD_CMD, Key.keymap);
	m_ewsfDesign.SetAt(Key.keymap,rData);
	if (bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->AddListItem(rData.GroupKey, LT_EWSD_CMD, Key.keymap);
	if(!bExist)
	{
		if (pElem) pElem->AddListItem(Key.key.entity, LT_EWSD_CMD, Key.keymap);
		if (pBngr) pBngr->AddListItem(rData.GroupKey, LT_EWSD_CMD, Key.keymap);
	}

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_EWSD_CMD, Key.keymap, rData.GroupKey);
	}
}

BOOL CDB_EWSF::DelDesign(T_EWSF_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_EWSF_D Data;
	m_ewsfDesign.Lookup(Key.keymap, Data);
	BOOL ret=m_ewsfDesign.RemoveKey(Key.keymap);
	ASSERT(ret);
	if(ret)
	{
		if (pElem) VERIFY(pElem->DelListItem(Key.key.entity, LT_EWSD_CMD, Key.keymap));
		if (pBngr) VERIFY(pBngr->DelListItem(Data.GroupKey, LT_EWSD_CMD, Key.keymap));

		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveBndr(LT_EWSD_CMD, Key.keymap);
		}
	}
	return ret;
}

//--EWSF------------------------------------------------------------------------
BOOL CDB_EWSF::Get(T_EWSF_K Key,T_EWSF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_ewsf->Lookup(Key.keymap, StagBngrKey)) return FALSE;
	}
	return m_ewsf.Lookup(Key.keymap,rData);
}

int CDB_EWSF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_ewsf->GetCount();
	}
	return m_ewsf.GetCount();
}

POSITION CDB_EWSF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_ewsf->GetStartPosition();
	}
	return m_ewsf.GetStartPosition();
}

void CDB_EWSF::GetNext(POSITION& rNextPosition,T_EWSF_K& rKey,T_EWSF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_ewsf->GetNextAssoc(rNextPosition, rKey.keymap, StagBngrKey);
		m_ewsf.Lookup(rKey.keymap, rData);
		return;
	}
	m_ewsf.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

//--EWSD------------------------------------------------------------------------
BOOL CDB_EWSF::GetDesign(T_EWSF_K Key,T_EWSF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_ewsfDesign->Lookup(Key.keymap, StagBngrKey)) return FALSE;
	}
	return m_ewsfDesign.Lookup(Key.keymap,rData);
}

int CDB_EWSF::GetCountDesign()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_ewsfDesign->GetCount();
	}
	return m_ewsfDesign.GetCount();
}

POSITION CDB_EWSF::GetStartDesign()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_ewsfDesign->GetStartPosition();
	}
	return m_ewsfDesign.GetStartPosition();
}

void CDB_EWSF::GetNextDesign(POSITION& rNextPosition,T_EWSF_K& rKey,T_EWSF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_ewsfDesign->GetNextAssoc(rNextPosition, rKey.keymap, StagBngrKey);
		m_ewsfDesign.Lookup(rKey.keymap, rData);
		return;
	}
	m_ewsfDesign.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}
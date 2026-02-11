#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_WSSF.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

// 코멘트1: 시공단계에서 없다. PostCS에만 있다. 시공단계 입력 안됨

CDB_WSSF::CDB_WSSF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_wssf.InitHashTable(HASHSIZEWSSF);
}

CDB_WSSF::~CDB_WSSF()
{

}

void CDB_WSSF::Add(T_WSSF_K Key,T_WSSF_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_WSSF_D Data;
	BOOL bExist=m_wssf.Lookup(Key.keymap, Data);
	if(bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->DelListItem(Data.GroupKey, LT_WSSF_CMD, Key.keymap);
	m_wssf.SetAt(Key.keymap,rData);
	if(bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->AddListItem(rData.GroupKey, LT_WSSF_CMD, Key.keymap);
	if(!bExist)
	{
		if(pElem) pElem->AddListItem(Key.key.entity, LT_WSSF_CMD, Key.keymap);
		if(pBngr) pBngr->AddListItem(rData.GroupKey, LT_WSSF_CMD, Key.keymap);
	}

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	/* 코멘트1 참조
	if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if(!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_WSSF_CMD, Key.keymap, rData.GroupKey);
	}
	*/
}

BOOL CDB_WSSF::Del(T_WSSF_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_WSSF_D Data;
	m_wssf.Lookup(Key.keymap, Data);
	BOOL ret=m_wssf.RemoveKey(Key.keymap);
	ASSERT(ret);
	if(ret)
	{
		if (pElem) VERIFY(pElem->DelListItem(Key.key.entity, LT_WSSF_CMD, Key.keymap));
		if (pBngr) VERIFY(pBngr->DelListItem(Data.GroupKey, LT_WSSF_CMD, Key.keymap));

		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		/* 코멘트 1 참조
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveBndr(LT_WSSF_CMD, Key.keymap);
		}
		*/
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_WSSF::Get(T_WSSF_K Key,T_WSSF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_wssf->Lookup(Key.keymap, StagBngrKey)) return FALSE;
	}
	return m_wssf.Lookup(Key.keymap,rData);
}

int CDB_WSSF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_wssf->GetCount();
	}
	return m_wssf.GetCount();
}

POSITION CDB_WSSF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_wssf->GetStartPosition();
	}
	return m_wssf.GetStartPosition();
}

void CDB_WSSF::GetNext(POSITION& rNextPosition,T_WSSF_K& rKey,T_WSSF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }    
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_wssf->GetNextAssoc(rNextPosition,rKey.keymap,StagBngrKey);
		m_wssf.Lookup(rKey.keymap, rData);
		return;
	}
	m_wssf.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

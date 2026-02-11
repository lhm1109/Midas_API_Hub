#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_ESSF.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

// 코멘트1: 시공단계에서 없다. PostCS에만 있다. 시공단계 입력 안됨

CDB_ESSF::CDB_ESSF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_essf.InitHashTable(HASHSIZEESSF);
}

CDB_ESSF::~CDB_ESSF()
{

}

void CDB_ESSF::Add(T_ESSF_K Key,T_ESSF_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_ESSF_D Data;
	BOOL bExist=m_essf.Lookup(Key.keymap, Data);
	if(bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->DelListItem(Data.GroupKey, LT_ESSF_CMD, Key.keymap);
	m_essf.SetAt(Key.keymap,rData);
	if(bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->AddListItem(rData.GroupKey, LT_ESSF_CMD, Key.keymap);
	if(!bExist)
	{
		if(pElem) pElem->AddListItem(Key.key.entity, LT_ESSF_CMD, Key.keymap);
		if(pBngr) pBngr->AddListItem(rData.GroupKey, LT_ESSF_CMD, Key.keymap);
	}

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	/* 코멘트1 참조
	if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if(!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_ESSF_CMD, Key.keymap, rData.GroupKey);
	}
	*/
}

BOOL CDB_ESSF::Del(T_ESSF_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr)
{
	T_ESSF_D Data;
	m_essf.Lookup(Key.keymap, Data);
	BOOL ret=m_essf.RemoveKey(Key.keymap);
	ASSERT(ret);
	if(ret)
	{
		if (pElem) VERIFY(pElem->DelListItem(Key.key.entity, LT_ESSF_CMD, Key.keymap));
		if (pBngr) VERIFY(pBngr->DelListItem(Data.GroupKey, LT_ESSF_CMD, Key.keymap));

		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		/* 코멘트 1 참조
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveBndr(LT_ESSF_CMD, Key.keymap);
		}
		*/
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_ESSF::Get(T_ESSF_K Key,T_ESSF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_essf->Lookup(Key.keymap, StagBngrKey)) return FALSE;
	}
	return m_essf.Lookup(Key.keymap,rData);
}

int CDB_ESSF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_essf->GetCount();
	}
	return m_essf.GetCount();
}

POSITION CDB_ESSF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_essf->GetStartPosition();
	}
	return m_essf.GetStartPosition();
}

void CDB_ESSF::GetNext(POSITION& rNextPosition,T_ESSF_K& rKey,T_ESSF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_essf->GetNextAssoc(rNextPosition,rKey.keymap,StagBngrKey);
		m_essf.Lookup(rKey.keymap, rData);
		return;
	}
	m_essf.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}

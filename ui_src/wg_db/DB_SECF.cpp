#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_SECF.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SECF::CDB_SECF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_secf.InitHashTable(HASHSIZESECF);
}

CDB_SECF::~CDB_SECF()
{

}

void CDB_SECF::Add(T_SECF_K Key,T_SECF_D& rData,CDB_SECT* pSect,CDB_BNGR* pBngr)
{
	T_SECF_D Data;
	BOOL bExist=m_secf.Lookup(Key.keymap, Data);
	if(bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->DelListItem(Data.GroupKey, LT_SECF_CMD, Key.keymap);
	m_secf.SetAt(Key.keymap,rData);
	if(bExist && Data.GroupKey != rData.GroupKey && pBngr)
		pBngr->AddListItem(rData.GroupKey, LT_SECF_CMD, Key.keymap);
	if(!bExist)
	{
		if(pSect) pSect->AddListItem(Key.key.entity, LT_SECF_CMD, Key.keymap);
		if(pBngr) pBngr->AddListItem(rData.GroupKey, LT_SECF_CMD, Key.keymap);
	}

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	/* 시공단계중에 추가/수정/삭제 안됨
	if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if(!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_SECF_CMD, Key.keymap, rData.GroupKey);
	}
	*/
}

BOOL CDB_SECF::Del(T_SECF_K Key,CDB_SECT* pSect,CDB_BNGR* pBngr)
{
	T_SECF_D Data;
	m_secf.Lookup(Key.keymap, Data);
	BOOL ret=m_secf.RemoveKey(Key.keymap);
	ASSERT(ret);
	if(ret)
	{
		if (pSect) VERIFY(pSect->DelListItem(Key.key.entity, LT_SECF_CMD, Key.keymap));
		if (pBngr) VERIFY(pBngr->DelListItem(Data.GroupKey, LT_SECF_CMD, Key.keymap));

		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		/* 시공단계중에 추가/수정/삭제 안됨
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveBndr(LT_SECF_CMD, Key.keymap);
		}
		*/
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SECF::Get(T_SECF_K Key,T_SECF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_secf->Lookup(Key.keymap, StagBngrKey)) return FALSE;
	}
	return m_secf.Lookup(Key.keymap,rData);
}

int CDB_SECF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_secf->GetCount();
	}
	return m_secf.GetCount();
}

POSITION CDB_SECF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_secf->GetStartPosition();
	}
	return m_secf.GetStartPosition();
}

void CDB_SECF::GetNext(POSITION& rNextPosition,T_SECF_K& rKey,T_SECF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_secf->GetNextAssoc(rNextPosition,rKey.keymap,StagBngrKey);
		m_secf.Lookup(rKey.keymap, rData);
		return;
	}
	m_secf.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}


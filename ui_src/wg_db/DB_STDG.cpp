#include "stdafx.h"
#include "DB_STDG.h"

#include "DB_STOR.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"


CDB_STDG::CDB_STDG()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_stdg.InitHashTable(HASHSIZESTDG);
}

CDB_STDG::~CDB_STDG()
{

}

void CDB_STDG::Add(T_STDG_K Key,T_STDG_D& rData,CDB_STOR* pStor,CDB_BNGR* pBngr)
{
	T_STDG_D Data;
	BOOL bExist=m_stdg.Lookup(Key, Data);
	m_stdg.SetAt(Key,rData);
	if(bExist && pBngr) pBngr->DelListItem(Data.GroupKey, LT_STDG_CMD, Key);
	if(!bExist && pStor != NULL) pStor->AddListItem(Key, LT_STDG_CMD, Key);
	if(pBngr) pBngr->AddListItem(rData.GroupKey, LT_STDG_CMD, Key);

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (bExist) m_pDoc->m_pStagCtrl->DeactiveBndr(LT_STDG_CMD, Key);
		m_pDoc->m_pStagCtrl->ActiveBndr(LT_STDG_CMD, Key, rData.GroupKey);
	}
}

BOOL CDB_STDG::Del(T_STDG_K Key,CDB_STOR* pStor,CDB_BNGR* pBngr)
{
	T_STDG_D Data;
	m_stdg.Lookup(Key, Data);
	BOOL ret=m_stdg.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if (pStor)VERIFY(pStor->DelListItem(Key, LT_STDG_CMD, Key));
		if (pBngr)VERIFY(pBngr->DelListItem(Data.GroupKey, LT_STDG_CMD, Key));

		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		if (m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveBndr(LT_STDG_CMD, Key);
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_STDG::Get(T_STDG_K Key,T_STDG_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		if (!m_pDoc->m_pStagCtrl->m_stdg->Lookup(Key, StagBngrKey)) return FALSE;
	}
	return m_stdg.Lookup(Key,rData);
}

int CDB_STDG::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_stdg->GetCount();
	}
	return m_stdg.GetCount();
}

POSITION CDB_STDG::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_stdg->GetStartPosition();
	}
	return m_stdg.GetStartPosition();
}

void CDB_STDG::GetNext(POSITION& rNextPosition,T_STDG_K& rKey,T_STDG_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		T_KEY2 StagBngrKey;
		m_pDoc->m_pStagCtrl->m_stdg->GetNextAssoc(rNextPosition,rKey,StagBngrKey);
		m_stdg.Lookup(rKey, rData);
		return;
	}
	m_stdg.GetNextAssoc(rNextPosition,rKey,rData);
}


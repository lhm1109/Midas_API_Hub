#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_GMAS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_GMAS::CDB_GMAS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_gmas.InitHashTable(HASHSIZEGMAS);
}

CDB_GMAS::~CDB_GMAS()
{

}

void CDB_GMAS::Add(T_GMAS_K Key,T_GMAS_D& rData,CDB_NODE* pNode)
{
	T_GMAS_D Data;
	BOOL bExist=m_gmas.Lookup(Key, Data);
	m_gmas.SetAt(Key,rData);
	if(!bExist && pNode != NULL)pNode->AddListItem(Key, LT_GMAS_CMD, Key);
	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_GMAS_CMD, Key);
	}
}

BOOL CDB_GMAS::Del(T_GMAS_K Key,CDB_NODE* pNode)
{
	BOOL ret=m_gmas.RemoveKey(Key);
	ASSERT(ret);
	if(ret && pNode != NULL)
	{
		VERIFY(pNode->DelListItem(Key, LT_GMAS_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveEtc(LT_GMAS_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_GMAS::Get(T_GMAS_K Key,T_GMAS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_gmas->Lookup(Key, uTemp)) return FALSE;
	}
	return m_gmas.Lookup(Key,rData);
}

int CDB_GMAS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_gmas->GetCount();
	}
	return m_gmas.GetCount();
}

POSITION CDB_GMAS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_gmas->GetStartPosition();
	}
	return m_gmas.GetStartPosition();
}

void CDB_GMAS::GetNext(POSITION& rNextPosition,T_GMAS_K& rKey,T_GMAS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_gmas->GetNextAssoc(rNextPosition,rKey,uTemp);
		m_gmas.Lookup(rKey, rData);
		return;
	}
	m_gmas.GetNextAssoc(rNextPosition,rKey,rData);
}

#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_SKEW.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SKEW::CDB_SKEW()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_skew.InitHashTable(HASHSIZESKEW);
}

CDB_SKEW::~CDB_SKEW()
{

}

void CDB_SKEW::Add(T_SKEW_K Key,T_SKEW_D& rData,CDB_NODE* pNode)
{
	T_SKEW_D Data;
	BOOL bExist=m_skew.Lookup(Key, Data);
	m_skew.SetAt(Key,rData);
	if(!bExist && pNode != NULL)pNode->AddListItem(Key, LT_SKEW_CMD, Key);

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_SKEW_CMD, Key, 0);
	}
}

BOOL CDB_SKEW::Del(T_SKEW_K Key,CDB_NODE* pNode)
{
	BOOL ret=m_skew.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if (pNode != NULL)VERIFY(pNode->DelListItem(Key, LT_SKEW_CMD, Key));
		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveBndr(LT_SKEW_CMD, Key);
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SKEW::Get(T_SKEW_K Key,T_SKEW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_skew->Lookup(Key, uTemp)) return FALSE;
	}
	return m_skew.Lookup(Key,rData);
}

int CDB_SKEW::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_skew->GetCount();
	}
	return m_skew.GetCount();
}

POSITION CDB_SKEW::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_skew->GetStartPosition();
	}
	return m_skew.GetStartPosition();
}

void CDB_SKEW::GetNext(POSITION& rNextPosition,T_SKEW_K& rKey,T_SKEW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_skew->GetNextAssoc(rNextPosition,rKey,uTemp);
		m_skew.Lookup(rKey, rData);
		return;
	}
	m_skew.GetNextAssoc(rNextPosition,rKey,rData);
}

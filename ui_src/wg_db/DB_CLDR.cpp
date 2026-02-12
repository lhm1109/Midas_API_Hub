#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_CLDR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CLDR::CDB_CLDR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cldr.InitHashTable(HASHSIZECLDR);
}

CDB_CLDR::~CDB_CLDR()
{

}

void CDB_CLDR::Add(T_CLDR_K Key,T_CLDR_D& rData,CDB_NODE* pNode)
{
	T_CLDR_D Data;
	BOOL bExist=m_cldr.Lookup(Key, Data);
	m_cldr.SetAt(Key,rData);
	if(!bExist && pNode != NULL)pNode->AddListItem(Key, LT_CLDR_CMD, Key);

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_CLDR_CMD, Key, 0);
	}
}

BOOL CDB_CLDR::Del(T_CLDR_K Key,CDB_NODE* pNode)
{
	BOOL ret=m_cldr.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if (pNode != NULL)VERIFY(pNode->DelListItem(Key, LT_CLDR_CMD, Key));
		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveBndr(LT_CLDR_CMD, Key);
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CLDR::Get(T_CLDR_K Key,T_CLDR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_cldr->Lookup(Key, uTemp)) return FALSE;
	}
	return m_cldr.Lookup(Key,rData);
}

int CDB_CLDR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_cldr->GetCount();
	}
	return m_cldr.GetCount();
}

POSITION CDB_CLDR::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_cldr->GetStartPosition();
	}
	return m_cldr.GetStartPosition();
}

void CDB_CLDR::GetNext(POSITION& rNextPosition,T_CLDR_K& rKey,T_CLDR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_cldr->GetNextAssoc(rNextPosition,rKey,uTemp);
		m_cldr.Lookup(rKey, rData);
		return;
	}
	m_cldr.GetNextAssoc(rNextPosition,rKey,rData);
}

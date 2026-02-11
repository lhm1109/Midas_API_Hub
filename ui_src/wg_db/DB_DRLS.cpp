#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_DRLS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DRLS::CDB_DRLS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_drls.InitHashTable(HASHSIZEDRLS);
}

CDB_DRLS::~CDB_DRLS()
{

}

void CDB_DRLS::Add(T_DRLS_K Key,T_DRLS_D& rData,CDB_NODE* pNode)
{
	T_DRLS_D Data;
	BOOL bExist=m_drls.Lookup(Key, Data);
	m_drls.SetAt(Key,rData);
	if(!bExist && pNode != NULL)pNode->AddListItem(Key, LT_DRLS_CMD, Key);

	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	// Boundary는 현재 Active된 그룹에 속하는 것만 Add될 수 있으므로
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveBndr(LT_DRLS_CMD, Key, 0);
	}
}

BOOL CDB_DRLS::Del(T_DRLS_K Key,CDB_NODE* pNode)
{
	BOOL ret=m_drls.RemoveKey(Key);
	ASSERT(ret);
	if (ret)
	{
		if(pNode != NULL)VERIFY(pNode->DelListItem(Key, LT_DRLS_CMD, Key));
		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveBndr(LT_DRLS_CMD, Key);
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_DRLS::Get(T_DRLS_K Key,T_DRLS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		// Stage에서도 되도록 한다.
		//if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_drls->Lookup(Key, uTemp)) return FALSE;
	}
	return m_drls.Lookup(Key,rData);
}

int CDB_DRLS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		// Stage에서도 되도록 한다.
		//if(!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_drls->GetCount();
	}
	return m_drls.GetCount();
}

POSITION CDB_DRLS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		// Stage에서도 되도록 한다.
		//if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_drls->GetStartPosition();
	}
	return m_drls.GetStartPosition();
}

void CDB_DRLS::GetNext(POSITION& rNextPosition,T_DRLS_K& rKey,T_DRLS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{ 
		// Stage에서도 되도록 한다.
		//if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_drls->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_drls.Lookup(rKey, rData);
		return;
	}
	m_drls.GetNextAssoc(rNextPosition,rKey,rData);
}
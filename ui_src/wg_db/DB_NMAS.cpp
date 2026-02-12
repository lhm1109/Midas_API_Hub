#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_NMAS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_NMAS::CDB_NMAS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nmas.InitHashTable(HASHSIZENMAS);
}

CDB_NMAS::~CDB_NMAS()
{

}

void CDB_NMAS::Add(T_NMAS_K Key,T_NMAS_D& rData,CDB_NODE* pNode)
{
	T_NMAS_D Data;
	BOOL bExist=m_nmas.Lookup(Key, Data);
	m_nmas.SetAt(Key,rData);
	if(!bExist && pNode != NULL)pNode->AddListItem(Key, LT_NMAS_CMD, Key);
}

BOOL CDB_NMAS::Del(T_NMAS_K Key,CDB_NODE* pNode)
{
	BOOL ret=m_nmas.RemoveKey(Key);
	ASSERT(ret);
	if(ret && pNode != NULL)VERIFY(pNode->DelListItem(Key, LT_NMAS_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_NMAS::Get(T_NMAS_K Key,T_NMAS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_nmas->Lookup(Key, uTemp)) return FALSE;
	}
	return m_nmas.Lookup(Key,rData);
}

int CDB_NMAS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_nmas->GetCount();
	}
	return m_nmas.GetCount();
}

POSITION CDB_NMAS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_nmas->GetStartPosition();
	}
	return m_nmas.GetStartPosition();
}

void CDB_NMAS::GetNext(POSITION& rNextPosition,T_NMAS_K& rKey,T_NMAS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_nmas->GetNextAssoc(rNextPosition,rKey,uTemp);
		m_nmas.Lookup(rKey, rData);
		return;
	}
	m_nmas.GetNextAssoc(rNextPosition,rKey,rData);
}

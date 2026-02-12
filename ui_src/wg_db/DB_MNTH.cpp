#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_MNTH.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MNTH::CDB_MNTH()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mnth.InitHashTable(HASHSIZEMNTH);
}

CDB_MNTH::~CDB_MNTH()
{

}

void CDB_MNTH::Add(T_MNTH_K Key,T_MNTH_D& rData,CDB_ELEM* pElem)
{
	T_MNTH_D Data;
	BOOL bExist=m_mnth.Lookup(Key, Data);
	m_mnth.SetAt(Key,rData);
	if(!bExist)
	{
		if(pElem) pElem->AddListItem(Key, LT_MNTH_CMD, Key);
	}
}

BOOL CDB_MNTH::Del(T_MNTH_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_mnth.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(pElem) pElem->DelListItem(Key, LT_MNTH_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_MNTH::Get(T_MNTH_K Key,T_MNTH_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_mnth->Lookup(Key, uTemp)) return FALSE;
	}
	return m_mnth.Lookup(Key,rData);
}

int CDB_MNTH::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mnth->GetCount();
	}
	return m_mnth.GetCount();
}

POSITION CDB_MNTH::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mnth->GetStartPosition();
	}
	return m_mnth.GetStartPosition();
}

void CDB_MNTH::GetNext(POSITION& rNextPosition,T_MNTH_K& rKey,T_MNTH_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_mnth->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_mnth.Lookup(rKey, rData);
		return;
	}
	m_mnth.GetNextAssoc(rNextPosition,rKey,rData);
}

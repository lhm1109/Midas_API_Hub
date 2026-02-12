#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_UTCF.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_UTCF::CDB_UTCF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_utcf.InitHashTable(HASHSIZEUTCF);
}

CDB_UTCF::~CDB_UTCF()
{

}

void CDB_UTCF::Add(T_UTCF_K Key,T_UTCF_D& rData,CDB_ELEM* pElem)
{
	T_UTCF_D Data;
	BOOL bExist=m_utcf.Lookup(Key, Data);
	m_utcf.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_UTCF_CMD, Key);
}

BOOL CDB_UTCF::Del(T_UTCF_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_utcf.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pElem->DelListItem(Key, LT_UTCF_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_UTCF::Get(T_UTCF_K Key,T_UTCF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_utcf->Lookup(Key, uTemp)) return FALSE;
	}
	return m_utcf.Lookup(Key,rData);
}

int CDB_UTCF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_utcf->GetCount();
	}
	return m_utcf.GetCount();
}

POSITION CDB_UTCF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_utcf->GetStartPosition();
	}
	return m_utcf.GetStartPosition();
}

void CDB_UTCF::GetNext(POSITION& rNextPosition,T_UTCF_K& rKey,T_UTCF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_utcf->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_utcf.Lookup(rKey, rData);
		return;
	}
	m_utcf.GetNextAssoc(rNextPosition,rKey,rData);
}

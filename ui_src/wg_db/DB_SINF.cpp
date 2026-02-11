#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SINF.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SINF::CDB_SINF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sinf.InitHashTable(HASHSIZESINF);
}

CDB_SINF::~CDB_SINF()
{

}

void CDB_SINF::Add(T_SINF_K Key,T_SINF_D& rData,CDB_ELEM* pElem)
{
	T_SINF_D Data;
	BOOL bExist=m_sinf.Lookup(Key, Data);
	m_sinf.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_SINF_CMD, Key);
}

BOOL CDB_SINF::Del(T_SINF_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_sinf.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pElem->DelListItem(Key, LT_SINF_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SINF::Get(T_SINF_K Key,T_SINF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_sinf->Lookup(Key, uTemp)) return FALSE;
	}
	return m_sinf.Lookup(Key,rData);
}

int CDB_SINF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sinf->GetCount();
	}
	return m_sinf.GetCount();
}

POSITION CDB_SINF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_sinf->GetStartPosition();
	}
	return m_sinf.GetStartPosition();
}

void CDB_SINF::GetNext(POSITION& rNextPosition,T_SINF_K& rKey,T_SINF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_sinf->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_sinf.Lookup(rKey, rData);
		return;
	}
	m_sinf.GetNextAssoc(rNextPosition,rKey,rData);
}

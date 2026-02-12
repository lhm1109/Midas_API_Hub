#include "stdafx.h"
#include "DB_POVC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_POVC::CDB_POVC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_povc.InitHashTable(HASHSIZEPOVC);
}

CDB_POVC::~CDB_POVC()
{

}

void CDB_POVC::Add(T_POVC_K Key, T_POVC_D& rData)
{
	T_POVC_D Data;
	BOOL bExist = m_povc.Lookup(Key, Data);
	m_povc.SetAt(Key, rData);
}

BOOL CDB_POVC::Del(T_POVC_K Key)
{
	BOOL ret = m_povc.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_POVC::Get(T_POVC_K Key, T_POVC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_povc.Lookup(Key, rData);
}

int CDB_POVC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_povc.GetCount();
}

POSITION CDB_POVC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_povc.GetStartPosition();
}

void CDB_POVC::GetNext(POSITION& rNextPosition, T_POVC_K& rKey, T_POVC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_povc.GetNextAssoc(rNextPosition, rKey, rData);
}


#include "stdafx.h"
#include "DB_FMAV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_FMAV::CDB_FMAV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_fmav.InitHashTable(HASHSIZEFMAV);
}

CDB_FMAV::~CDB_FMAV()
{

}

void CDB_FMAV::Add(T_FMAV_K Key, T_FMAV_D& rData)
{
	T_FMAV_D Data;
	BOOL bExist = m_fmav.Lookup(Key, Data);
	m_fmav.SetAt(Key, rData);
}

BOOL CDB_FMAV::Del(T_FMAV_K Key)
{
	BOOL ret = m_fmav.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_FMAV::Get(T_FMAV_K Key, T_FMAV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_fmav.Lookup(Key, rData);
}

int CDB_FMAV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_fmav.GetCount();
}

POSITION CDB_FMAV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_fmav.GetStartPosition();
}

void CDB_FMAV::GetNext(POSITION& rNextPosition, T_FMAV_K& rKey, T_FMAV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_fmav.GetNextAssoc(rNextPosition, rKey, rData);
}

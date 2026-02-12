#include "stdafx.h"
#include "DB_CMFV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CMFV::CDB_CMFV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cmfv.InitHashTable(HASHSIZECMFV);
}

CDB_CMFV::~CDB_CMFV()
{

}

void CDB_CMFV::Add(T_CMFV_K Key, T_CMFV_D& rData)
{
	T_CMFV_D Data;
	BOOL bExist = m_cmfv.Lookup(Key, Data);
	m_cmfv.SetAt(Key, rData);
}

BOOL CDB_CMFV::Del(T_CMFV_K Key)
{
	BOOL ret = m_cmfv.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CMFV::Get(T_CMFV_K Key, T_CMFV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cmfv.Lookup(Key, rData);
}

int CDB_CMFV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cmfv.GetCount();
}

POSITION CDB_CMFV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cmfv.GetStartPosition();
}

void CDB_CMFV::GetNext(POSITION& rNextPosition, T_CMFV_K& rKey, T_CMFV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cmfv.GetNextAssoc(rNextPosition, rKey, rData);
}
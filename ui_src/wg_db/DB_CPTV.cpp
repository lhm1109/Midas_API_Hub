#include "stdafx.h"
#include "DB_CPTV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CPTV::CDB_CPTV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cptv.InitHashTable(HASHSIZECPTV);
}

CDB_CPTV::~CDB_CPTV()
{

}

void CDB_CPTV::Add(T_CPTV_K Key, T_CPTV_D& rData)
{
	T_CPTV_D Data;
	BOOL bExist = m_cptv.Lookup(Key, Data);
	m_cptv.SetAt(Key, rData);
}

BOOL CDB_CPTV::Del(T_CPTV_K Key)
{
	BOOL ret = m_cptv.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CPTV::Get(T_CPTV_K Key, T_CPTV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cptv.Lookup(Key, rData);
}

int CDB_CPTV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cptv.GetCount();
}

POSITION CDB_CPTV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cptv.GetStartPosition();
}

void CDB_CPTV::GetNext(POSITION& rNextPosition, T_CPTV_K& rKey, T_CPTV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cptv.GetNextAssoc(rNextPosition, rKey, rData);
}

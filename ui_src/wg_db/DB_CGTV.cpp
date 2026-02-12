#include "stdafx.h"
#include "DB_CGTV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CGTV::CDB_CGTV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cgtv.InitHashTable(HASHSIZECGTV);
}

CDB_CGTV::~CDB_CGTV()
{

}

void CDB_CGTV::Add(T_CGTV_K Key, T_CGTV_D& rData)
{
	T_CGTV_D Data;
	BOOL bExist = m_cgtv.Lookup(Key, Data);
	m_cgtv.SetAt(Key, rData);
}

BOOL CDB_CGTV::Del(T_CGTV_K Key)
{
	BOOL ret = m_cgtv.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CGTV::Get(T_CGTV_K Key, T_CGTV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cgtv.Lookup(Key, rData);
}

int CDB_CGTV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgtv.GetCount();
}

POSITION CDB_CGTV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgtv.GetStartPosition();
}

void CDB_CGTV::GetNext(POSITION& rNextPosition, T_CGTV_K& rKey, T_CGTV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cgtv.GetNextAssoc(rNextPosition, rKey, rData);
}
